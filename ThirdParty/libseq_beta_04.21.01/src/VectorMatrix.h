//////////////////////////////////////////////////////////////////////////////
//
// VectorMatrix.h, 28.7.99, Ilja Friedel
//                                                                        
//////////////////////////////////////////////////////////////////////////////
//                                                                        
// Status: 
//
//////////////////////////////////////////////////////////////////////////////

#ifndef VECTORMATRIX_USED
#define VECTORMATRIX_USED

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <assert.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////////////
// own include files 
//////////////////////////////////////////////////////////////////////////////

#include "options.h"
#include "own_types.h"
#include "C_matrix.h"

using namespace std;


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

class VectorMatrix
{
    UL_int memory_used; 
    RVector * c;
    UL_int base;
public:

    UL_int dimension;  
    UL_int NumDigits; 
    UL_int times;
    UL_int NumVectors;

    char   name    [256];
    char   filename[256];

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

    RVector array2vector(R_Elem * a)
      {
	int i;
	RVector result;
	
	result=0;
	for(i=0;i<int(times);i++) result=result*base+a[i];
	
	return(result);
      }

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

    RVector query(UL_int dim,UL_int row,UL_int column)
           {
#ifdef DEBUG
	     if(dim>=dimension)
             {
	       cerr << "Error: Dimension out of range! (VectorMatrix::query())";
	       cerr << endl;
	       cerr << "dim="         << dim;
               cerr << ", row="       << row;
               cerr << ", column="    << column    << endl;
	       cerr << "dimension="   << dimension;
	       cerr << ", NumDigits=" << NumDigits << endl;
	       exit(1);
	     }
	     if(row>=NumVectors)
             {
	       cerr << "Error: Row out of range! (VectorMatrix::query())");
	       cerr << endl;
	       cerr << "dim="         << dim;
               cerr << ", row="       << row;
               cerr << ", column="    << column     << endl;
	       cerr << "dimension="   << dimension;
	       cerr << ", NumVectors="<< NumVectors << endl;
	       exit(1);
	     }
	     if(column>=NumDigits)
             {
	       cerr << "Error: Column out of range! (VectorMatrix::query())");
	       cerr << endl;
	       cerr << "dim="         << dim;
               cerr << ", row="       << row;
               cerr << ", column="    << column    << endl;
	       cerr << "dimension="   << dimension;
	       cerr << ", NumDigits=" << NumDigits << endl;
	       exit(1);
	     }
#endif 
	     return(c[(dim*NumVectors+row)*NumDigits+column]);
           }  

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

    void   set(UL_int dim,UL_int row,UL_int column,RVector val)
           {
#ifdef DEBUG
	     if(dim>=dimension)
             {
	       cerr << "Error: Dimension out of range! (VectorMatrix::set())";
	       cerr << endl;
	       cerr << "dim="         << dim;
               cerr << ", row="       << row;
               cerr << ", column="    << column    << endl;
	       cerr << "dimension="   << dimension;
	       cerr << ", NumDigits=" << NumDigits << endl;
	       exit(1);
	     }
	     if(row>=NumVectors)
             {
	       cerr << "Error: Row out of range! (VectorMatrix::set())");
	       cerr << endl;
	       cerr << "dim="         << dim;
               cerr << ", row="       << row;
               cerr << ", column="    << column     << endl;
	       cerr << "dimension="   << dimension;
	       cerr << ", NumVectors="<< NumVectors << endl;
	       exit(1);
	     }
	     if(column>=NumDigits)
             {
	       cerr << "Error: Column out of range! (VectorMatrix::set())");
	       cerr << endl;
	       cerr << "dim="         << dim;
               cerr << ", row="       << row;
               cerr << ", column="    << column    << endl;
	       cerr << "dimension="   << dimension;
	       cerr << ", NumDigits=" << NumDigits << endl;
	       exit(1);
	     }

//////////////////////////////////////////////////////////////////////////////
// *** to do *** NumDigits/NumVectors, check below
//////////////////////////////////////////////////////////////////////////////

	     /*
	     if(val>=R->base)
             {
	       printf("Val out of range! (VectorMatrix::set())\n");
	       printf("dim=%d, row=%d, column=%d, val=%d\n",dim,row,column,val);
	       printf("dimension=%d, NumDigits=%d\n",dimension,NumDigits);
	       exit(1);
	     }
	     */
#endif  
             c[(dim*NumVectors+row)*NumDigits+column]=val;
           }

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

           VectorMatrix(C_matrix *C,UL_int t)
           {
	     R_Elem a[64];
	     int akdim,column;

		 UL_int row, l;

	     dimension  = C->dimension;
	     NumDigits  = C->NumDigits;
	     base       = C->R->base;
	     times      = t;
	     NumVectors = NumDigits/times;

	     if(NumDigits%times) {NumVectors++;}

	     c=new RVector[dimension*NumVectors*NumDigits];
	     if(!c)
	       {
		 cerr << "Error: Out of memory!" << endl;
		 exit(1);
	       }
	     for(akdim=0;akdim<int(dimension);akdim++)
	       {
		 for(column=0;column<int(NumDigits);column++)
		   {
		     for(row=0;row<NumDigits/times;row++)
		       {
			 for(l=0;l<times;l++) a[l]=C->query(akdim,times*row+l,column);
			 set(akdim,row,column,array2vector(a));
		       }
		     if(NumDigits%times)
		       {
			 for(l=0;l<times;l++) a[l]=0;
			 for(l=0;l<NumDigits%times;l++) a[l]=C->query(akdim,times*(NumDigits/times)+l,column);
			 set(akdim,NumDigits/times,column,array2vector(a));
		       }
		   }
	       }
	   }
  
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

           virtual ~VectorMatrix();

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

};

#endif
