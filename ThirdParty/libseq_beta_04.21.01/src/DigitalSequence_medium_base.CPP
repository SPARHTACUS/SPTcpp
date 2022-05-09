//////////////////////////////////////////////////////////////////////////////
//
// DigitalSequence_medium_base.C, 8.8.99, Ilja Friedel
//
//////////////////////////////////////////////////////////////////////////////
//
// Status: get_BufferLenForBase(UL_int base) tuned, ok
//
//////////////////////////////////////////////////////////////////////////////

#include "DigitalSequence.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////////

UL_int DigitalSequence_medium_base::get_log_BufferLenForBase(UL_int base)
{

   if((base>= 2)&&(base<=10)) return int_log(get_BufferLenForBase(base),base);
   if(base<=MAX_BUFFER_LEN) return 1;

   return 0;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// the constants in the following table can be tweaked to adjust for the
// cache size of the computer used
//////////////////////////////////////////////////////////////////////////////

UL_int DigitalSequence_medium_base::get_BufferLenForBase(UL_int base)
{

   if(base== 2) return 64;
   if(base== 3) return 27; 
   if(base== 4) return 16; 
   if(base== 5) return 25; 
   if(base== 6) return 36; 
   if(base== 7) return 49; 
   if(base== 8) return 64;
   if(base== 9) return  9;
   if(base==10) return 10;
   
   if(base<=MAX_BUFFER_LEN) return base;

   return 1;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

DigitalSequence_medium_base::DigitalSequence_medium_base(C_matrix *C):DigitalSequence(C->dimension)
{
   init(C,C->dimension,0);
}

//////////////////////////////////////////////////////////////////////////////

DigitalSequence_medium_base::DigitalSequence_medium_base(C_matrix *C,UL_int dimension):DigitalSequence(dimension)
{
   init(C,dimension,0);
}

//////////////////////////////////////////////////////////////////////////////

DigitalSequence_medium_base::DigitalSequence_medium_base(C_matrix *C,UL_int dimension, UL_int offset):DigitalSequence(dimension)
{
   init(C,dimension,offset);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void DigitalSequence_medium_base::init(C_matrix *C, UL_int dim, UL_int offset)
{
   L_int i,k;
   UL_int base;

#ifdef DEBUG
   check_own_types();
#endif

   R             = C->R;
   base          = R->base;
   inv_base      = 1.0/((double)base);
   CM            = C;
   NumDigits     = C->NumDigits; 
   dimension     = dim;
   check_arguments();

   n0            = offset;
   n             = n0;
   BufferFull    = 0;
   BufferLen     = get_BufferLenForBase(base);
   log_BufferLen = get_log_BufferLenForBase(base);

   init_psi_and_eta();

   N             = new Counter(R,NumDigits,psi);
   DigitBuffer   = new UL_int ** [BufferLen];
   if(!(N&&DigitBuffer))
      {
	cerr << "Error: Out of memory!" << endl;
	exit(1);
      }

   for(k=0;k<BufferLen;k++)
     {
       DigitBuffer[k] = new UL_int * [dimension];
       if(!(DigitBuffer[k]))
	 {
	   cerr << "Error: Out of memory!" << endl;
	   exit(1);
	 }
       for(i=0;i<dimension;i++)
	 {
	   DigitBuffer[k][i] = new UL_int[NumDigits];
	   if(!(DigitBuffer[k][i]))
	     {
	       cerr << "Error: Out of memory!" << endl;
	       exit(1);
	     }
	 }
     }

   clear_DigitBuffer();

   N->set(n0);
   operator++();

   memory     += sizeof(DigitalSequence_medium_base);
   memory     += sizeof(UL_int)*dimension*NumDigits*BufferLen;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void DigitalSequence_medium_base::operator++()
{
   L_int row,column,akdim,i,k;
   double dtmp;
   UL_int ** LastDigit;

   check_next_dim();
   reset_next_dim();
//////////////////////////////////////////////////////////////////////////////
// we have to reset next_dim in every <<::operator++()>>
//////////////////////////////////////////////////////////////////////////////

   Digit = DigitBuffer[n%BufferLen];

   if(!(BufferFull))
     {
       LastDigit = DigitBuffer[(n-1)%BufferLen];
       for(column=0;column<=N->LastChangingDigit;column++)	 
	 {
	   if((N->difference(column))!=0)
	     {
	       for(akdim=0;akdim<dimension;akdim++)
		 {	      
		   for(row=0;row<NumDigits;row++) // row<=C->LastDigitNotNull
		     {	       
		       Digit[akdim][row]=R->add(LastDigit[akdim][row],
					   R->mult(CM->query(akdim,row,column),
					      N->difference(column)));
		     }   
		 }
	       LastDigit=Digit; // different only for initialization of the
	                        // new array
	     }       
	 }

       n++;
       if(((n-n0)/BufferLen>=1)&&((n%BufferLen)==0))
	 {
	   // switch to faster calculation via Buffer recycling
	   BufferFull=1;
	   N->increment_digit(log_BufferLen);	   
	 }
       else ++(*N);
       n--;
     }

//////////////////////////////////////////////////////////////////////////////

   else if((n%BufferLen)==0)
     {
       for(akdim=0;akdim<dimension;akdim++)
	 {	      
	   for(row=0;row<NumDigits;row++) // row<=C->LastDigitNotNull
	     {	       
	       R_Elem change;
	       change=0;
	       for(column=(L_int)log_BufferLen;column<=N->LastChangingDigit;column++)
 		 {
		   if((N->difference(column))!=0)
		     {
		       change=R->add(change,
				     R->mult(CM->query(akdim,row,column),
					     N->difference(column)));
		     }   
		 }
	       if(change!=0) for(k=0;k<BufferLen;k++)
		 {
		   DigitBuffer[k][akdim][row]=R->add(change,
						  DigitBuffer[k][akdim][row]);
		 }
	     }       
	 }
       N->increment_digit(log_BufferLen);
     }

//////////////////////////////////////////////////////////////////////////////

   for(akdim=0;akdim<dimension;akdim++)
     {
       dtmp=0.0;

       for(i=(L_int)NumDigits-1;i>=0;i--)
	 {  
	   dtmp=(dtmp+
		 (double)(*(eta[akdim*(NumDigits+1)+i]))[Digit[akdim][i]]
		 )*inv_base;
	 }       
       X[akdim]=dtmp;
     }
   n++;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

DigitalSequence_medium_base::~DigitalSequence_medium_base()
{
   L_int i,j,k;

   for(k=0;k<BufferLen;k++)
     {
       for(i=0;i<dimension;i++)
	 { 
	   delete[] (DigitBuffer[k][i]);
	 }
       delete[] (DigitBuffer[k]);
     }
   delete[] DigitBuffer;
   DigitBuffer=nullptr;
   Digit=nullptr;

   for(i=0;i<=NumDigits;i++)
     {
       if(psi) delete psi[i];
       if(eta) 
	 for(j=0;j<dimension;j++) 
	   {
	     if(eta[j*(NumDigits+1)+i]) delete eta[j*(NumDigits+1)+i];
	   }
     }
   if(psi) delete[] psi;
   if(eta) delete[] eta;
   psi=nullptr;
   eta=nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void DigitalSequence_medium_base::random_restart() 
{
   L_int i,j;
   
   for(i=0;i<=NumDigits;i++)
   { 
      ++(*(psi[i]));
      for(j=0;j<dimension;j++)
      {
         ++(*(eta[j*(NumDigits+1)+i]));
      }
   }

   n = n0;
   N->reset();
//////////////////////////////////////////////////////////////////////////////
// N->reset() must be _after_ choosing new permutations!!! 
//////////////////////////////////////////////////////////////////////////////
   N->set(n0);
   clear_DigitBuffer();
   BufferFull=0;
   operator++();
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void DigitalSequence_medium_base::restart(void) 
{
   n = n0;
   N->reset();
   N->set(n);
   clear_DigitBuffer();
   BufferFull=0;

   operator++();
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void DigitalSequence_medium_base::init_psi_and_eta(void)
{
  
   L_int i,j;
   UL_int base;
   
   base=R->base;
   
   psi=new RandomPermutation * [NumDigits+1];
   memory+=sizeof(RandomPermutation *)*(NumDigits+1);
   
   if(!psi)
     {
       cerr << "Error: Out of memory! ";
       cerr << "(DigitalSequence_medium_base::init_psi_and_eta())" << endl;
       exit(1);
     }
   
   eta=new RandomPermutation * [dimension*(NumDigits+1)];
   memory+=sizeof(RandomPermutation *)*(NumDigits+1)*dimension;
   
   if(!eta)
     {
       cerr << "Error: Out of memory! ";
       cerr << "(DigitalSequence_medium_base::init_psi_and_eta())" << endl;
       exit(1);
     }
   
   for(i=0;i<=NumDigits;i++)
     { 
       psi[i]=new RandomPermutation(base);
       memory+=psi[i]->memory_used();
       
       if(!psi[i])
	 {
	   cerr << "Error: Out of memory! ";
	   cerr << "(DigitalSequence_medium_base::init_psi_and_eta())" << endl;
	   exit(1);
	 } 
       for(j=0;j<dimension;j++)
	 {
	   
	   eta[j*(NumDigits+1)+i]=new RandomPermutation(base);
	   memory+=eta[i]->memory_used();
	   
	   if(eta[j*(NumDigits+1)+i]==nullptr)
	     {
	       cerr << "Error: Out of memory! ";
	       cerr << "(DigitalSequence_medium_base::init_psi_and_eta())";
	       cerr << endl;
	       exit(1);
	     }
	 }
     }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void DigitalSequence_medium_base::clear_DigitBuffer(void)
{
   L_int k,i,j;

   for(k=0;k<BufferLen;k++)
     {
       for(i=0;i<dimension;i++)
	 {
	   for(j=0;j<NumDigits;j++) DigitBuffer[k][i][j]=0;
	 }
     }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
