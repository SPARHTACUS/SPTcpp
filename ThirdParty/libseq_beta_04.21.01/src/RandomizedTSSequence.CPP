//////////////////////////////////////////////////////////////////////////////
//
// RandomizedTSSequence.C, 8.8.99, Ilja Friedel
//
//////////////////////////////////////////////////////////////////////////////
//
// Status: RANDOMIZED_QUICKSORT was not tested often 
//
//////////////////////////////////////////////////////////////////////////////

#include "RandomizedTSSequence.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

RandomizedTSSequence * new_RandomizedTSSequence(C_matrix *C, UL_int len)
{
   if(C->R->base==2) return (new RandomizedTSSequence_base_2(C,len));
   else
     {
       return (new RandomizedTSSequence(C,len));
     }
}

//////////////////////////////////////////////////////////////////////////////

RandomizedTSSequence * new_RandomizedTSSequence(C_matrix *C, UL_int dim, UL_int len)
{
   if(C->R->base==2) return (new RandomizedTSSequence_base_2(C,dim,len));
   else
     {
       return (new RandomizedTSSequence(C,dim,len));
     }
}

//////////////////////////////////////////////////////////////////////////////

RandomizedTSSequence * new_RandomizedTSSequence(C_matrix *C, UL_int dim, UL_int len, UL_int LSS_flag)
{
   if(C->R->base==2) return (new RandomizedTSSequence_base_2(C,len,dim,LSS_flag));
   else
     {
       return (new RandomizedTSSequence(C,len,dim,LSS_flag));
     }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

RandomizedTSSequence::~RandomizedTSSequence()
{
   L_int i;

   for(i=0;i<dimension;i++)
     {
       delete[] Seq[i];
       delete[] P[i];
       delete[] ChangingDigit[i];
     }

   delete[] Seq;
   Seq=nullptr;
   delete[] P;
   P=nullptr;
   delete[] ChangingDigit;
   ChangingDigit=nullptr;

   if(Perm[1]) 
//////////////////////////////////////////////////////////////////////////////
// hack, see Constructor RandomizedTSSequence(UL_int dim)
//////////////////////////////////////////////////////////////////////////////
     {
       for(i=1;i<=NumDigits;i++)
	 {
	   if(Perm[i]) delete Perm[i];
	   Perm[i]=nullptr;
	 }
     }
#ifdef DESTRUCTOR
   cerr << "Info: Destructor RandomizedTSSequence::~RandomizedTSSequence"
	<< " was called.";
#endif
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

RandomizedTSSequence::RandomizedTSSequence(C_matrix * C, UL_int len):Sequence(C->dimension)
{
   init(C,C->dimension,len,0);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

RandomizedTSSequence::RandomizedTSSequence(C_matrix * C, UL_int dim, UL_int len):Sequence(dim)
{
   init(C,dim,len,0);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

RandomizedTSSequence::RandomizedTSSequence(C_matrix * C, UL_int dim, UL_int len, UL_int LSS_flag):Sequence(dim)
{
   init(C,dim,len,LSS_flag);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void RandomizedTSSequence::init(C_matrix * C, UL_int dim,UL_int len, UL_int LSS_flag)
{
   UL_int akdim,k;
   R_Elem digits[65];
   int i;
   DigitalSequence * GEN;

   n                           = 0;
   base                        = C->R->base;
   dimension                   = dim;
   length                      = len;
   Component_of_LatinSupercube = LSS_flag;
   inv_Max_int                 = 1.0/4294967296.0;

   fixed_point_to_digits(0,base,digits);
   NumDigits=digits[0];
   scale=4294967296.0/(pow((double)base,(double)NumDigits));

   powers[0]=1.0;
   for(i=1;i<=NumDigits;i++)
     {
       powers[i]=powers[i-1]*(double)base;

       Perm  [i]=new LazyRandomPermutation(base);
       if(!(Perm[i]))
	 {
	   cerr << "Error: Out of memory! (RandomizedTSSequence";
	   cerr << "::RandomizedTSSequence())" << endl;
	   exit(1);
	 }
#ifndef RTSS_TEST
       ++(*(Perm[i]));
#endif
     }
#ifdef RTSS_TEST
   cerr << "Info: "
	<< "RTSS_TEST is switched on."
	<< "(RandomizedTSSequence::init())" << endl;
#endif

//////////////////////////////////////////////////////////////////////////////
   if(base==2)
     {
       GEN=new DigitalSequence_base_2(C,dimension);
     }
   else
     {
#ifdef DEBUG
       seqlib_warning("RandomizedTSSequence","init()",
		      "DEBUG is switched on. Using DigitalSequence_classic instead of DigitalSequence_advanced.");

       GEN=new DigitalSequence_classic(C,dimension);
//////////////////////////////////////////////////////////////////////////////
// DigitalSequence_advanced() is more complex than _classic. So there is with
// higher probability a bug. Hence, this should be safer.
//////////////////////////////////////////////////////////////////////////////
#else
       //GEN=new DigitalSequence_medium_base(C,dimension);
       GEN=new DigitalSequence_advanced(C,dimension);
#endif
     }

   if(!(GEN))
     {
       cerr << "Error: Out of memory! (RandomizedTSSequence::";
       cerr << "RandomizedTSSequence())" << endl;
       exit(1);
     }

   GEN->random_restart();
//////////////////////////////////////////////////////////////////////////////
//
// We have roundoff errors in the conversion routine, which can destroy
// the whole result. The <GEN->random_restart();> reduces them for
// short <len> (say up to len=10^7), but it would be better not to have 
// them at all.
//
// This is the most serious (known) problem this library currently has.
//
//////////////////////////////////////////////////////////////////////////////
//
// Detail/Example:
// ===============
//
// For bases 2,4,8,16 etc. there are no (serious) roundoff errors.
//
// Let base=3. Use 2 digits. Computed sequence (digits in base 3) 
// is for instance
//
// A0=0.00, A1=0.10, A2=0.20, A3=0.11, etc.
//
// As we see A1=0.333333... (decimal) is in its correct elementary box,
// but it lies on the border. Converting it to decimal (or binary in the
// programm) it will be rounded to 0.33 decimal. This is close
// to the box it should be. To be picky now some elementary boxes
// have 0 points in them, other 2 (and most have still 1).
// Owen's randomization is a highly nonlinear process. The point that was
// after rounding still close to it's home box, will be after randomization
// far away. The home box (and it's surrounding) might be totally empty,
// other points might be too close.
//
// Now the really bad thing: If the situation described above will happen
// _only_ once, we might end up with a convergence rate of O(N^-1) instead
// of O(N^-1.5).
//
// Now the good thing: GEN->random_restart(); places in "almost all" case
// the point away from the border of the home box more into the center.
// Experimentally this worked really well up to len=10^6..10^7. After that
// it starts to fail.
//
// For bases which are powers of 2 it works fine up to len=2^32 (if you happen
// to have that much memory). 
//
// I could recode the randomization routine such that no such rounding errors
// occur. Most likely this will happen either:
// - at the expense of memory (using 64 bit integers)
// - at the expense of speed (checking if we missed the home box because of
//                            rounding)
// - at the expense of the max length of the sequence (max_len=base^k<=2^32)
// - maybe some change in the DigitalSequence generator?
//
// Please feel free to discuss this issue with the autor (Ilja Friedel,
// ilja@caltech.edu).
//
//////////////////////////////////////////////////////////////////////////////
   get_memory(dimension,length);

   for(k=0;k<length;k++)
     {
       for(akdim=0;akdim<dim;akdim++)
	 {
	   (Seq[akdim])[k]=(FixPoint)(4294967296.0*((*GEN)[akdim]));
	   (P  [akdim])[k]=k;
	 }
       ++(*GEN);
     }
   delete GEN;
   GEN=nullptr;

//////////////////////////////////////////////////////////////////////////////

   sort_sequence_and_generate_permutation();
   calc_changing_digits();
   randomize_sequence();

//////////////////////////////////////////////////////////////////////////////

   n=0;
   n--;
   operator++();
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void RandomizedTSSequence::operator++(void)
{
   UL_int akdim;

   n++;
   if(n<length)
     {
       for(akdim=0;akdim<dimension;akdim++)
	 {
	   X[akdim]=((double)(Seq[akdim])[n])*inv_Max_int;
	 }
     }
   else
     {
       if(n==length) 
	 {
#ifdef WARNINGS_EXTERNAL
	   cerr << "Info: Reaching end of sequence! "
		<< "(RandomizedTSSequence::operator++())"<<endl;
#endif
	 }
       else
	 {
	   cerr << "Error: End of sequence! " << endl;
	   cerr << "(RandomizedTSSequence::operator++())" << endl;
	   exit(1);
	 }
     }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

UL_int RandomizedTSSequence::partition(UL_int dim,UL_int p,UL_int r)
{
 
   UL_int pivot;
   UL_int i,j;
   UL_int tmp;
   FixPoint h;

#ifdef RANDOMIZED_QUICKSORT
   pivot=(Seq[dim])[p+random_Ring_Element(r-p+1)];
#else
   pivot=(Seq[dim])[p];
#endif

   i=p-1;
   j=r+1;

   while(1)
     {
M1:     j--;
        if ((Seq[dim])[j]>pivot) goto M1;  
M2:     i++;
        if ((Seq[dim])[i]<pivot) goto M2;

	if (i<j)
	  {
	    h            =(Seq[dim])[i];
	    (Seq[dim])[i]=(Seq[dim])[j];
	    (Seq[dim])[j]=h;

	    tmp          =(P[dim])[i];
	    (P[dim])[i]  =(P[dim])[j];
	    (P[dim])[j]  =tmp;
	  }
	else return(j);
     }
}

//////////////////////////////////////////////////////////////////////////////

void RandomizedTSSequence::stochastic_quicksort(UL_int dim,UL_int p,UL_int r)
{
   UL_int q;

   if (p<r)
     {
       q=partition(dim,p,r);
       stochastic_quicksort(dim,p,q);
       stochastic_quicksort(dim,q+1,r);
     }
}

//////////////////////////////////////////////////////////////////////////////

void RandomizedTSSequence::sort_sequence_and_generate_permutation(void)
{
   UL_int akdim;

   for(akdim=0;akdim<dimension;akdim++)
     {
       stochastic_quicksort(akdim,0,length-1);

#ifdef DEBUG
       UL_int n;
       for(n=0;n<length-1;n++)
	 {
	   if((Seq[akdim])[n]>(Seq[akdim])[n+1])
	     {
	       cerr << "Error: Quicksort is not working!" << endl;
	       exit(1);
	     }
	 }
#endif
     }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void RandomizedTSSequence::calc_changing_digits(void)
{
   UL_int n,akdim,i;
   UL_int tmp1[65],tmp2[65];

   UL_int * last_digits;
   UL_int * now_digits;
   UL_int * tmp_digits;

   for(akdim=0;akdim<dimension;akdim++)
     {
       last_digits=tmp1;
       now_digits =tmp2;
       fixed_point_to_digits(0,base,last_digits);

       for(n=0;n<length;n++)
	 {
	   fixed_point_to_digits((Seq[akdim])[n],base,now_digits);

	   i=1;
	   while((last_digits[i]==now_digits[i])&&(i<=last_digits[0])) i++;

	   if(i>last_digits[0]) (ChangingDigit[akdim])[n]=0;
           else                 (ChangingDigit[akdim])[n]=(unsigned char) i;

           tmp_digits =last_digits;
	   last_digits=now_digits;
	   now_digits =tmp_digits;
	 }
       (ChangingDigit[akdim])[length]=0;
     }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void RandomizedTSSequence::randomize_sequence(void)
{
   UL_int akdim,i,k;
   UL_int CD,NextCD;
   R_Elem digits[65];
   double value;
   double temp_value[65];
   
   n=0;

   for(akdim=0;akdim<dimension;akdim++)
     {
       value        =       0.0;
       temp_value[0]=     value;
       digits[0]    = NumDigits;
       
       for(i=1;i<=NumDigits;i++)
	 { 
#ifdef RTSS_TEST
	   cout << "[++P]";
#endif
	   ++(*(Perm[i]));

	   digits[i]=0;
	   value=value*(double)base+(double)((*(Perm[i]))[digits[i]]);
	   temp_value[i]=value;
	 }
       value=value*scale;
#ifdef DEBUG_INTERNAL
       if((value>=4294967296.0))
	 {
	   cerr << "Error: <scale> not ok!" << endl;
	   cerr << "(RandomizedTSSequence::randomize_sequence()" << endl;
	   exit(1);
	 }
#endif

//////////////////////////////////////////////////////////////////////////////
// end of initialization for the algorithm
//////////////////////////////////////////////////////////////////////////////

       Seq[akdim][P[akdim][0]]=(FixPoint)value;
#ifdef RTSS_TEST
       for(k=1;k<length;k++) cout << "CD[" << k <<
			       "]=" << ChangingDigit[0][k] << endl;
#endif
       for(k=1;k<length;k++)
	 {
#ifdef RTSS_TEST
	   cout << "<" <<k<< ">  ";
#endif
	   CD    =(ChangingDigit[akdim])[k];
	   i=0;
//////////////////////////////////////////////////////////////////////////////
// search for next CD not equal to zero
//////////////////////////////////////////////////////////////////////////////
	   do
	     {
	       i++;
	       NextCD=(ChangingDigit[akdim])[k+i];
	     }
	   while((NextCD==0)&&(k+i<length));
#ifdef RTSS_TEST
	   cout << "CD=" << CD;
	   cout << ", NextCD=" << NextCD;
#endif
	   if(CD!=0)
	     { 
//////////////////////////////////////////////////////////////////////////////
// we have something to do! (otherwise we store the old <<value>>)
//////////////////////////////////////////////////////////////////////////////

	       digits[CD]++;
#ifdef DEBUG_INTERNAL
	       if(digits[CD]>=base)
		 {
		   cerr << "Error: Fatal program error!" << endl;
		   cerr << "(RandomizedTSSequence::randomize_sequence())";
		   cerr << endl;
		   exit(1);
		 }
#endif
	       value=temp_value[CD-1];
	       value=value*(double)base+(double)((*(Perm[CD]))[digits[CD]]);
	       temp_value[CD]=value;
	       
	       if(CD<NextCD)
		 {
		   for(i=CD+1;i<=NextCD;i++)
		     { 
		       digits[i]=0;

#ifdef RTSS_TEST
		       cout << "[++P]";
#endif
		       ++(*(Perm[i]));

		       value=value*(double)base+
			 (double)((*(Perm[i]))[0/*digits[i]*/]);
		       temp_value[i]=value;
		     }
#ifdef RTSS_TEST
		   cout << "[07]";
#endif
		   value=(value+drand47())*powers[digits[0]-NextCD];
		 }
	       else
		 {
#ifdef RTSS_TEST
		   cout << "[48]";
#endif
		   value=(value+drand47())*powers[digits[0]-CD];
		 }
	       
	       value=value*scale;
#ifdef DEBUG_INTERNAL
	       if((value>=4294967296.0))
		 {
		   cerr << "Error: <scale> not ok!" << endl;
		   cerr << "(RandomizedTSSequence::randomize_sequence()";
		   cerr << endl;
		   exit(1);
		 }
#endif
	     }
	   (Seq[akdim])[(P[akdim])[k]]=(FixPoint)value;
	 }
     }
   apply_permutation_for_latin_supercube(); 
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void RandomizedTSSequence::get_memory(UL_int d,UL_int l)
{
   int i;

   Seq           = new  FixPoint *      [d];
   P             = new  UL_int *        [d];
   ChangingDigit = new  unsigned char * [d];

   if(!(Seq&&P&&ChangingDigit))
     {
       cerr << "Error: Out of memory! (RandomizedTSSequence::";
       cerr << "get_memory(" << d <<", " << l << "):1)" << endl;
       exit(1);
     }

   for(i=0;i<d;i++)
     {
       Seq[i]           = new FixPoint[l];
       P[i]             = new UL_int[l];
       ChangingDigit[i] = new unsigned char[l+1];
       if(!(Seq[i]&&P[i]&&ChangingDigit[i]))
	 {
	   cerr << "Error: Out of memory! (RandomizedTSSequence::";
	   cerr << "get_memory(" << d <<", " << l << "):2)" << endl;
	   exit(1);
	 }
     }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void RandomizedTSSequence::apply_permutation_for_latin_supercube(void)
{
   if(Component_of_LatinSupercube)
     {
#ifdef DEBUG
       cerr << "Info: doing LSS randomization"<<endl;
#endif

//////////////////////////////////////////////////////////////////////////////
// changes the order of the sequence randomly after randomize_sequence()!!!
// (and only, if this object belongs to an LatinSupercube)
//////////////////////////////////////////////////////////////////////////////
       UL_int i,pos,akdim;
       R_Elem tmp;
       
       for(i=length-1;i>=1;i--)
	 {
	   pos=random_Ring_Element(i+1);   
	       
	   for(akdim=0;akdim<dimension;akdim++)
	     {
	       tmp             = Seq[akdim][i];
	       Seq[akdim][i]   = Seq[akdim][pos];
	       Seq[akdim][pos] = tmp;
	     }
	 }
     }
}
 
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void RandomizedTSSequence::random_restart(void)
{
   randomize_sequence();
   n=0;
   n--;
   operator++();
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void RandomizedTSSequence::restart(void)
{
   n=0;
   n--;
   operator++();
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
