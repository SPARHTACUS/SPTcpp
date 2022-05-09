//////////////////////////////////////////////////////////////////////////////
//
// LatinSupercube.h, 25.7.99, Ilja Friedel
//
//////////////////////////////////////////////////////////////////////////////
//
// Status: tested, ok
//
//////////////////////////////////////////////////////////////////////////////

#ifndef LATINSUPERCUBE_USED
#define LATINSUPERCUBE_USED

//////////////////////////////////////////////////////////////////////////////
// standard include files
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <assert.h>
#include <cmath>

//////////////////////////////////////////////////////////////////////////////
// own include files
//////////////////////////////////////////////////////////////////////////////

#include "options.h"
#include "own_types.h"
#include "Ring.h"
#include "C_matrix.h"
#include "DigitalSequence.h"
#include "RandomizedTSSequence.h"

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

class LatinSupercube : public Sequence
{
   RandomizedTSSequence  ** GenList; // Generator list

   UL_int * gen_at_dim;
   UL_int   length;
   UL_int   GenNumber;
   UL_int * LocalDim;

   inline   void init(C_matrix * C,UL_int GN, UL_int * LD, UL_int len);

public:
//////////////////////////////////////////////////////////////////////////////

   inline        LatinSupercube(C_matrix *, UL_int, UL_int *, UL_int);

//////////////////////////////////////////////////////////////////////////////
 
   inline        LatinSupercube(C_matrix *, UL_int, UL_int, UL_int);
   inline        LatinSupercube(C_matrix *, UL_int, UL_int, UL_int, UL_int, UL_int);
   inline        LatinSupercube(C_matrix *, UL_int, UL_int, UL_int, UL_int, UL_int, UL_int, UL_int);
   inline        LatinSupercube(C_matrix *, UL_int, UL_int, UL_int, UL_int, UL_int, UL_int, UL_int, UL_int, UL_int);

//////////////////////////////////////////////////////////////////////////////

   virtual      ~LatinSupercube();

         void    operator++(void);
         void    random_restart(void);
         void    restart(void);
};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

inline LatinSupercube::LatinSupercube(C_matrix *C, UL_int dim, UL_int * LD, UL_int len):Sequence(dim)
{
   UL_int i;
   UL_int sum;

   sum = 0;
   i   = 0;
   assert(LD);
   while(sum<dim)
     {
       if(LD[i]==0)
	 {
	   cerr << "Error: trying to allocate a Sequence with ";
	   cerr << "dimension 0! ";
	   cerr << "(LatinSupercube::LatinSupercube(C_matrix *C, ";
	   cerr << "UL_int dim, UL_int * LD, UL_int len)" << endl;
	   exit(1);
	 }
       sum+=LD[i];
       i++;
     }
   if(sum!=dim)
     {
       cerr << "Error: Sum of local dimensions is not equal to dimension! ";
       cerr << "(LatinSupercube::LatinSupercube(C_matrix *C, ";
       cerr << "UL_int dim, UL_int * LD, UL_int len)" << endl;
       exit(1);
     }
   
   init(C,i,LD,len);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

inline LatinSupercube::LatinSupercube(C_matrix *C, UL_int dim0, UL_int dim1, UL_int len):Sequence(dim0+dim1)
{
   UL_int * LD;

   LD=new UL_int[2];
   assert(LD);
   LD[0]=dim0;
   LD[1]=dim1;
   
   init(C,2,LD,len);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

inline LatinSupercube::LatinSupercube(C_matrix *C, UL_int dim0, UL_int dim1, UL_int dim2, UL_int dim3, UL_int len):Sequence(dim0+dim1+dim2+dim3)
{
   UL_int * LD;

   LD=new UL_int[4];
   assert(LD);
   LD[0]=dim0;
   LD[1]=dim1;
   LD[2]=dim2;
   LD[3]=dim3;
   
   init(C,4,LD,len);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

inline LatinSupercube::LatinSupercube(C_matrix *C, UL_int dim0, UL_int dim1, UL_int dim2, UL_int dim3, UL_int dim4, UL_int dim5, UL_int len):Sequence(dim0+dim1+dim2+dim3+dim4+dim5)
{
   UL_int * LD;

   LD=new UL_int[8];
   assert(LD);

   LD[0]=dim0;
   LD[1]=dim1;
   LD[2]=dim2;
   LD[3]=dim3;
   LD[4]=dim4;
   LD[5]=dim5;
   
   init(C,6,LD,len);   
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
inline LatinSupercube::LatinSupercube(C_matrix *C, UL_int dim0, UL_int dim1, UL_int dim2, UL_int dim3, UL_int dim4, UL_int dim5, UL_int dim6, UL_int dim7, UL_int len):Sequence(dim0+dim1+dim2+dim3+dim4+dim5+dim6+dim7)
{
   UL_int * LD;

   LD=new UL_int[8];
   assert(LD);

   LD[0]=dim0;
   LD[1]=dim1;
   LD[2]=dim2;
   LD[3]=dim3;
   LD[4]=dim4;
   LD[5]=dim5;
   LD[6]=dim6;
   LD[7]=dim7;
   
   init(C,8,LD,len);   
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

inline void LatinSupercube::init(C_matrix * C,UL_int GN, UL_int * LD, UL_int len)
{
   int i,j,k;

   GenNumber = GN;
   length    = len;
   n0        = 0;
   n         = 0;

   GenList = new RandomizedTSSequence * [GenNumber];

   gen_at_dim = new UL_int [dimension];
   LocalDim   = new UL_int [dimension];

   if(!(GenList&&gen_at_dim&&LocalDim))
   {
      cerr << "Error: Out of memory! (LatinSupercube::LatinSupercube)" << endl;
      exit(1);
   }

   k=0;

#ifdef DEBUG
   cerr << "LatinSupercube::init(): always using RandomizedTSSequence()"<<endl;
#endif
   for(i=0;i<GenNumber;i++)
   {
      if((C->R->base)==2) GenList[i]=new RandomizedTSSequence_base_2(C,LD[i],length,1);
      else GenList[i]=new RandomizedTSSequence(C,LD[i],length,1);

      if(!(GenList[i]))
      {
         cerr << "Error: Out of memory! (LatinSupercube::LatinSupercube)" 
	      << endl;
         exit(1);
      }

      for(j=0;j<LD[i];j++)
      {
         gen_at_dim[k] = i;
         LocalDim  [k] = j;
         k++;
      }
   }

   n=0;
   n--;
   operator++();
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#endif
