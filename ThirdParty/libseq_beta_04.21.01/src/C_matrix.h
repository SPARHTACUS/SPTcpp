//////////////////////////////////////////////////////////////////////////////
//
// C_matrix.h, 11.6.99, Ilja Friedel
//                                                                        
//////////////////////////////////////////////////////////////////////////////
//                                                                        
// Status: looks good, load/save not very well tested
//         load probably buggy
//
//////////////////////////////////////////////////////////////////////////////

#ifndef C_MATRIX_USED
#define C_MATRIX_USED

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <assert.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////////////

#include "options.h"
#include "own_types.h"
#include "digit_gen.h"
#include "Ring.h"
#include "Polynomial.h"
using namespace std;
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

class C_matrix
{
protected:
    void   get_Niederreiter();
    void   get_Faure()           {cerr <<"Not implemented!"<< endl; exit(1);}
    void   get_Sobol()           {cerr <<"Not implemented!"<< endl; exit(1);}
    void   get_Schmid()          {cerr <<"Not implemented!"<< endl; exit(1);}
    void   get_NiederreiterXing(){cerr <<"Not implemented!"<< endl; exit(1);}

    void   ignore_comments(FILE * fid);
    UL_int memory_used; 

public:

    Ring   * R;
    UL_int * c;

    UL_int dimension;  
    UL_int NumDigits;          
    char   name    [256];
    char   filename[256];

    void   load(char * fname);
    void   save(char * fname);

//////////////////////////////////////////////////////////////////////////////

    UL_int used_memory(void){return(memory_used);}

//////////////////////////////////////////////////////////////////////////////

    UL_int query(UL_int dim,UL_int row,UL_int column)
           {
#ifdef DEBUG
	     if(dim>=dimension)
             {
	       cerr << "Error: Dimension out of range! (C_matrix::query())" << endl;
	       cerr << "dim="         << dim;
	       cerr << ", row="       << row;
	       cerr << ", column="    << column << endl;
	       cerr << "dimension="   << dimension;
	       cerr << ", NumDigits=" << NumDigits << endl;
	       exit(1);
	     }
	     if(row>=NumDigits)
             {
	       cerr << "Error: Row out of range! (C_matrix::query())" << endl;
	       cerr << "dim="         << dim;
	       cerr << ", row="       << row; 
	       cerr << ", column="    << column << endl;
	       cerr << "dimension="   << dimension;
	       cerr << ", NumDigits=" << NumDigits << endl;
	       exit(1);
	     }
	     if(column>=NumDigits)
             {
	       cerr << "Error: Column out of range! (C_matrix::query())" << endl;
	       cerr << "dim="         << dim;
	       cerr << ", row="       << row;
	       cerr << ", column="    << column << endl;
	       cerr << "dimension="   << dimension;
               cerr << ", NumDigits=" << NumDigits << endl;
	       exit(1);
	     }
#endif 
	     return(c[(dim*NumDigits+row)*NumDigits+column]);
           }  

//////////////////////////////////////////////////////////////////////////////

    void   set(UL_int dim, UL_int row, UL_int column, UL_int val)
           {
#ifdef DEBUG
	     if(dim>=dimension)
             {
	       cerr << "Error: Dimension out of range! (C_matrix::set())" << endl;
	       cerr << "dim="         << dim;
	       cerr << ", row="       << row;
	       cerr << ", column="    << column << endl;
	       cerr << "dimension="   << dimension;
	       cerr << ", NumDigits=" << NumDigits << endl;
	       exit(1);
	     }
	     if(row>=NumDigits)
             {
	       cerr << "Error: Row out of range! (C_matrix::set())" << endl;
	       cerr << "dim="         << dim;
	       cerr << ", row="       << row; 
	       cerr << ", column="    << column << endl;
	       cerr << "dimension="   << dimension;
	       cerr << ", NumDigits=" << NumDigits << endl;
	       exit(1);
	     }
	     if(column>=NumDigits)
             {
	       cerr << "Error: Column out of range! (C_matrix::set())" << endl;
	       cerr << "dim="         << dim;
	       cerr << ", row="       << row;
	       cerr << ", column="    << column << endl;
	       cerr << "dimension="   << dimension;
               cerr << ", NumDigits=" << NumDigits << endl;
	       exit(1);
	     }
	     if(val>=R->base)
             {
	       cerr << "Error: Val out of range! (C_matrix::set())" << endl;
	       cerr << "dim="         << dim;
	       cerr << ", row="       << row;
	       cerr << ", column="    << column << endl;
	       cerr << "dimension="   << dimension;
               cerr << ", NumDigits=" << NumDigits << endl;
	       exit(1);
	     }
#endif  
             c[(dim*NumDigits+row)*NumDigits+column]=val;
           }  

//////////////////////////////////////////////////////////////////////////////      
           UL_int operator[] (const UL_int pos) const
           {
#ifdef DEBUG
	     if(pos>=dimension*NumDigits*NumDigits)
             {
	       cerr << "Error: Pos out of range! (C_matrix::operator[])" << endl;
	       exit(1);
	     }
#endif 
	     return(c[pos]);
           }

//////////////////////////////////////////////////////////////////////////////

           C_matrix(Ring * S,char * s,UL_int dim)
           {
	      init(S,s,dim,
              (UL_int)(ceil(log(4294967296.0)/log((double)S->base))));
	   }
  
//////////////////////////////////////////////////////////////////////////////

           C_matrix(Ring * S,char * s,UL_int dim,UL_int ND)
           {
	      init(S,s,dim,ND);
	   }

//////////////////////////////////////////////////////////////////////////////

     void  init(Ring * S,char * s,UL_int dim,UL_int ND)
           {
			  sprintf(filename,"%s",s);
              R         = S;
              dimension = dim;
              NumDigits = ND;
            
              c=new UL_int[NumDigits*NumDigits*dim];
	      if(!c)
		 {
		   cerr << "Error: Out of memory! (C_matrix::C_matrix())"<<endl;
		   exit(1);
		 }
	      memory_used=sizeof(UL_int)*NumDigits*NumDigits*dim
		+sizeof(C_matrix);

              if(strncmp(s,"FILE:",5)==0)         {load(s+5);return;}
              if(strncmp(s,"Niederreiter",12)==0) {get_Niederreiter();return;}
              if(strncmp(s,"Faure",5)==0)         {get_Faure();return;}
              if(strncmp(s,"Sobol",5)==0)         {get_Sobol();return;}

              cerr << "Error: Don't know how to handle C_matrix('";
	      cerr << s << "')!" << endl;
              exit(1);
           }

//////////////////////////////////////////////////////////////////////////////

           virtual ~C_matrix();
};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#endif
