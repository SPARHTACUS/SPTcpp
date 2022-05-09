//////////////////////////////////////////////////////////////////////////////
//
// Sequence.h, 27.8.99, Ilja Friedel
//
//////////////////////////////////////////////////////////////////////////////
//
// Status: checked, ok
//
// to do: warnings, private/public
//////////////////////////////////////////////////////////////////////////////

#ifndef SEQUENCE_USED
#define SEQUENCE_USED

//////////////////////////////////////////////////////////////////////////////
// general include files
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <assert.h>
#include <cmath>
#include "options.h"
#include "own_types.h"
using namespace std;
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

class Sequence
{
public:

   UL_int   dimension;
   UL_int   next_dim;
   double * X;
   UL_int   memory;
   UL_int   n;
   UL_int   n0;   
   char     name    [256];
   char     filename[256];

          void   check_next_dim(void)
	    {
#ifdef WARNINGS_EXTERNAL
	       if(!((next_dim==0) || (next_dim==dimension)))
		 {
		   cerr << "Warning: "
			<< "get_next_dim() used, but not examined the whole "
			<< "vector! (0<next_dim=" << next_dim
			<< dimension <<"=dimension)"<<endl;

		   cerr << "(Sequence::check_next_dim())"<<endl;
		 }
#endif
	    }

//////////////////////////////////////////////////////////////////////////////

public: 
   virtual      ~Sequence();
   inline        Sequence(UL_int);

   virtual void  operator++(void)=0;
   inline double operator[](const UL_int) const;

          char * query_name    (void){return(name);}
          char * query_filename(void){return(filename);}

	  UL_int query_dimension(void){return(dimension);}
	  UL_int memory_used(void){return(memory);}
	  UL_int vector_number(void){return(n-n0);}

//////////////////////////////////////////////////////////////////////////////

	  virtual void  restart(void)
	    {
	      cerr << "Error: Sequence::restart() will not work here!" << endl;
	      cerr << "Please consider Sequence::random_restart() instead.";
	      cerr << endl;
	      exit(1);
	    }

	  virtual void  random_restart(void)=0;

//////////////////////////////////////////////////////////////////////////////

          void   reset_next_dim(void)
	    {
	      next_dim=0;
	    }  

//////////////////////////////////////////////////////////////////////////////

          double get_next_dim(void)
	    {
	      if(next_dim==dimension)
		{
		  cerr << "Error: get_next_dim() out of range!" << endl;
		  cerr << "Dimension=" << dimension;
		  cerr << " <= next_dim=" << next_dim << endl;
		  if(!this) cerr << "<this> is nullptr!" << endl;
		  else      cerr << "(<this> is not nullptr)" << endl;

		  exit(1);
		}
	      return(X[next_dim++]);
	    }
};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

inline Sequence::Sequence(UL_int dim) 
{
   next_dim  = 0;
   dimension =dim;
   if(dim==0)
      {
	 cerr << "Error: Dimension of sequence is 0! (Sequence::Sequence())";
	 cerr << endl;
	 exit(1);
      }

   X=new double[dim];
   if(!X)
   {
      cerr << "Error: Out of memory! (Sequence::Sequence("<< dim;
      cerr << "))" << endl;
      exit(1);
   }
   memory=sizeof(Sequence)+dim*sizeof(double);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

inline double Sequence::operator[](const UL_int j) const
{
   if(j >= dimension)
     {
       cerr << "Error: Index out of range! (Sequence::operator[])" << endl;
       cerr << "<dimension>=" << dimension;
       cerr << ", <j>=" << j << endl;
       exit(1);
     }
   return (X[j]);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#endif
