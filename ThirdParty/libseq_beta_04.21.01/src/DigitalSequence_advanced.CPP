//////////////////////////////////////////////////////////////////////////////
//
// DigitalSequence_advanced.C, 8.8.99, Ilja Friedel
//
//////////////////////////////////////////////////////////////////////////////
//
// Status:  nearly finished, get* functions 
// 
//////////////////////////////////////////////////////////////////////////////
//
// Warning: using function "new_DigitalSequence" will cause
//          memory leakage, because no call to ~DigitalSequence_advanced()
//          will occur
//
/////////////////////////////////////////////////////////////////////:-(//////

#include "DigitalSequence.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// The constants in the following table tell how many digits
// (or ring elements) are computed in parallel.
//
// Important: get_log_BufferLenForBase(UL_int base) must be a multiple of
//            get_times_for_base(UL_int base) !
// (It is save for this function to be (constant) 1. But then no work
//  is done parallel.)
//////////////////////////////////////////////////////////////////////////////

UL_int DigitalSequence_advanced::get_times_for_base(UL_int base)
{
  if(base== 2) return 6; 
  if(base== 3) return 4;
  if(base== 4) return 3;
  if(base== 5) return 2;
  if(base== 6) return 2;
  if(base== 7) return 2;
  if(base== 8) return 2;
  if(base== 9) return 1;
  if(base==10) return 1;
  if(base<=MAX_BUFFER_LEN) return 1;

  return 1;
}

//////////////////////////////////////////////////////////////////////////////

UL_int DigitalSequence_advanced::get_log_BufferLenForBase(UL_int base)
{

   if((base>= 2)&&(base<=10)) return int_log(get_BufferLenForBase(base),base);
   if(base<=MAX_BUFFER_LEN) return 1;

   return 0;
}

//////////////////////////////////////////////////////////////////////////////
// the constants in the following table can be tweaked to adjust for the
// cache size of the computer used
//
// Important: see remark for get_times_for_base(UL_int base) !
//////////////////////////////////////////////////////////////////////////////

UL_int DigitalSequence_advanced::get_BufferLenForBase(UL_int base)
{

   if(base== 2) return 64;
   if(base== 3) return 81;
   if(base== 4) return 64;
   if(base== 5) return 25;
   if(base== 6) return 36;
   if(base== 7) return 49;
   if(base== 8) return 64;
   if(base== 9) return 81;
   if(base==10) return 100;
   
   if(base<=MAX_BUFFER_LEN) return base;

   return 1;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

DigitalSequence_advanced::DigitalSequence_advanced(C_matrix *C):DigitalSequence(C->dimension)
{
   init(C,C->dimension,0);
}

//////////////////////////////////////////////////////////////////////////////

DigitalSequence_advanced::DigitalSequence_advanced(C_matrix *C, UL_int dimension):DigitalSequence(dimension)
{
   init(C,dimension,0);
}

//////////////////////////////////////////////////////////////////////////////

DigitalSequence_advanced::DigitalSequence_advanced(C_matrix *C, UL_int dimension, UL_int offset):DigitalSequence(dimension)
{
   init(C,dimension,offset);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void DigitalSequence_advanced::init(C_matrix *C, UL_int dim, UL_int offset)
{
   L_int i,k;
   UL_int base;
   UL_int times;
   UL_int base_pow_times;

#ifdef DEBUG
   check_own_types();
#endif

   R              = C->R;
   base           = R->base;
   times          = get_times_for_base(base);
   VCM            = new VectorMatrix(C,times);
   VR             = new VectorRing(R,times);
   base_pow_times = VR->base_pow_times;

   inv_base_pow_times = 1.0/((double)base_pow_times);

   CM          = C;
   NumDigits   = VCM->NumDigits;
   NumVectors  = VCM->NumVectors;
   dimension   = dim;
   check_arguments();

   n0            = offset;
   n             = n0;
   BufferFull    = 0;
   BufferLen     = get_BufferLenForBase(base_pow_times);
   log_BufferLen = get_log_BufferLenForBase(base);//base_pow_times);

   init_psi_and_eta();

//////////////////////////////////////////////////////////////////////////////
// get new Counter and DigitBuffer
//////////////////////////////////////////////////////////////////////////////
   N           = new Counter(R,NumDigits,psi);
   DigitBuffer = new RVector ** [BufferLen];
   if(!(N&&DigitBuffer))
      {
	cerr << "Error: Out of memory!" << endl;
	exit(1);
      }

   for(k=0;k<BufferLen;k++)
     {
       DigitBuffer[k] = new RVector * [dimension];
       if(!(DigitBuffer[k]))
	 {
	   cerr << "Error: Out of memory! ";
	   cerr << "(DigitalSequence_advanced::init())" << endl;
	   exit(1);
	 }
       for(i=0;i<dimension;i++)
	 {
	   DigitBuffer[k][i] = new RVector[NumVectors];
	   if(!(DigitBuffer[k][i]))
	     {
	       cerr << "Error: Out of memory! ";
	       cerr << "(DigitalSequence_advanced::init())" << endl;
	       exit(1);
	     }
	 }
     }

   clear_DigitBuffer();

//////////////////////////////////////////////////////////////////////////////
// calculate the next vector, actualize memory consumption
//////////////////////////////////////////////////////////////////////////////

   N->set(n0);
   operator++();

   memory     += sizeof(DigitalSequence_advanced);
   memory     += sizeof(UL_int)*dimension*NumVectors*BufferLen;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void DigitalSequence_advanced::operator++()
{
   L_int row,column,akdim,i,k;
   double dtmp;
   RVector ** LastDigit;

   check_next_dim();
   reset_next_dim();
//////////////////////////////////////////////////////////////////////////////
// we have to reset next_dim in every <<::operator++()>>
//////////////////////////////////////////////////////////////////////////////

   Digit = DigitBuffer[n % BufferLen];
   if(!(BufferFull))
     {
       LastDigit = DigitBuffer[(n-1)%BufferLen];
       for(column=0;column<=N->LastChangingDigit;column++)	 
	 {
	   if((N->difference(column))!=0)
	     {
	       for(akdim=0;akdim<dimension;akdim++)
		 {	      
		   for(row=0;row<NumVectors;row++) // row<=C->LastDigitNotNull
		     {	       
		       Digit[akdim][row]=VR->add(LastDigit[akdim][row],
					   VR->mult(N->difference(column),
					     VCM->query(akdim,row,column)						  ));
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

	   ++(*N);//N->increment_digit(log_BufferLen);	   
	 }
       else ++(*N);
       n--;
     }

//////////////////////////////////////////////////////////////////////////////

   else if((n%BufferLen)==0)
     {
       for(akdim=0;akdim<dimension;akdim++)
	 {	      
	   for(row=0;row<NumVectors;row++) // row<=C->LastDigitNotNull
	     {	       
	       RVector change;
	       change=0;
	       for(column=(L_int)log_BufferLen;column<=N->LastChangingDigit;column++)
 		 {
		   if((N->difference(column))!=0)
		     {
		       change=VR->add(change,
				VR->mult(N->difference(column),
				  VCM->query(akdim,row,column)));
		     }   
		 }
	       if(change!=0) for(k=0;k<BufferLen;k++)
		 {
		   DigitBuffer[k][akdim][row]=VR->add(change,
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

       for(i=(L_int)NumVectors-1;i>=0;i--)
	 {  
	   dtmp=(dtmp+
		 (double)(*(eta[akdim*(NumVectors+1)+i]))[Digit[akdim][i]]
		 )*inv_base_pow_times;
	 }       
       X[akdim]=dtmp;
     }
   n++;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

DigitalSequence_advanced::~DigitalSequence_advanced()
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
   Digit      =nullptr;

//////////////////////////////////////////////////////////////////////////////

   for(i=0;i<=NumDigits;i++)
     {
       if(psi) delete psi[i];
     }
   if(psi) delete[] psi;
   psi=nullptr;

//////////////////////////////////////////////////////////////////////////////

   for(i=0;i<=NumVectors;i++)
     {
       if(eta) for(j=0;j<dimension;j++) {delete eta[j*(NumVectors+1)+i];}
     }
   if(eta) delete[] eta;
   eta=nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void DigitalSequence_advanced::random_restart(void) 
{
   L_int i,j;
   
   for(i=0;i<=NumDigits;i++) 
     {
       ++(*(psi[i]));
     }
   for(i=0;i<=NumVectors;i++)
   { 
      for(j=0;j<dimension;j++)
      {
         ++(*(eta[j*(NumVectors+1)+i]));
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

void DigitalSequence_advanced::restart(void) 
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

void DigitalSequence_advanced::init_psi_and_eta(void)
{
    L_int i,j;
   UL_int base;
   UL_int times;
   
   base=R->base;
   times=get_times_for_base(base);
   
//////////////////////////////////////////////////////////////////////////////

   psi=new RandomPermutation * [NumDigits+1];
   memory+=sizeof(RandomPermutation *) *(NumDigits+1);
   
   if(!psi)
     {
       cerr << "Error: Out of memory! ";
       cerr << "(DigitalSequence_advanced::init_psi_and_eta())" << endl;
       exit(1);
     }
   
//////////////////////////////////////////////////////////////////////////////

   for(i=0;i<=NumDigits;i++)
     { 
       psi[i]=new RandomPermutation(base);
       memory+=psi[i]->memory_used();
       
       if(!psi[i])
	 {
	   cerr << "Error: Out of memory! ";
	   cerr << "(DigitalSequence_advanced::init_psi_and_eta())" << endl;
	   exit(1);
	 } 
     }

//////////////////////////////////////////////////////////////////////////////

   eta    =new RandomPermutation * [dimension*(NumVectors+1)];
   memory+=sizeof(RandomPermutation *)*(NumVectors+1)*dimension;
   
   if(!eta)
     {
       cerr << "Error: Out of memory! ";
       cerr << "(DigitalSequence_advanced::init_psi_and_eta())" << endl;
       exit(1);
     }
   
//////////////////////////////////////////////////////////////////////////////

   for(i=0;i<=NumVectors;i++)
     { 
       for(j=0;j<dimension;j++)
	 {
	   
	   eta[j*(NumVectors+1)+i]=new VectorRandomPermutation(base,times);
	   memory+=eta[i]->memory_used();
	   
	   if(eta[j*(NumVectors+1)+i]==nullptr)
	     {
	       cerr << "Error: Out of memory! ";
	       cerr << "(DigitalSequence_advanced::init_psi_and_eta())" <<endl;
	       exit(1);
	     }
	 }
     }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void DigitalSequence_advanced::clear_DigitBuffer(void)
{
   L_int k,i,j;

   for(k=0;k<BufferLen;k++)
     {
       for(i=0;i<dimension;i++)
	 {
	   for(j=0;j<NumVectors;j++) DigitBuffer[k][i][j]=0;
	 }
     }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
