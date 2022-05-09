//////////////////////////////////////////////////////////////////////////////
//
// VectorRing.C, 28.7.99, Ilja Friedel
//
//////////////////////////////////////////////////////////////////////////////
//
// Status: please note the dirty trick for times=1!
//
//////////////////////////////////////////////////////////////////////////////

#include "VectorRing.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

VectorRing::VectorRing(Ring * S, UL_int t)
{
   UL_int i,j,k;
   R_Elem vr[64];
   R_Elem va[64];
   R_Elem vb[64];

   R              = S;
   base           = R->base;
   times          = t;
   base_pow_times = 1;
   for(i=0;i<times;i++) base_pow_times*=base;

   if(times>1)
     {
       _add       = new RVector[base_pow_times * base_pow_times];
       _mult      = new RVector[base           * base_pow_times];
       _a_inverse = new RVector[base_pow_times];
       if(!(_add && _mult&& _a_inverse))
	 {
	   cerr << "Error: Out of memory!" << endl;
	   exit(1);
	 }
       
       for(i=0;i<base_pow_times;i++)
	 {
	   vector2array(i,va);
	   
	   for(j=0;j<base_pow_times;j++)
	     {
	       vector2array(j,vb);
	       for(k=0;k<times;k++)
		 {
		   vr[k]=R->add(va[k],vb[k]);
		 }
	       _add[i+j*base_pow_times]=array2vector(vr);
	     }
	   
	   for(j=0;j<base;j++)
	     {
	       for(k=0;k<times;k++)
		 {
		   vr[k]=R->mult(j,va[k]);
		 }
	       _mult[j+base*i]=array2vector(vr);
	     }
	   
	   for(k=0;k<times;k++)
	     {
	       vr[k]=R->minus(va[k]);
	     }
	   _a_inverse[i]=array2vector(vr);
	 }
     }
   else 
     {
       _add       = R->_add;
       _mult      = R->_mult;
       _a_inverse = R->_a_inverse;
     }
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

VectorRing::~VectorRing()
{
  if(times>1)
    {
      if(_add)       delete [] _add;
      if(_mult)      delete [] _mult;
      if(_a_inverse) delete [] _a_inverse;
    }
  _add      =nullptr;
  _mult     =nullptr;
  _a_inverse=nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

