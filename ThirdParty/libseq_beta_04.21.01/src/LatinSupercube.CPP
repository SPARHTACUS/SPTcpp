//////////////////////////////////////////////////////////////////////////////
//
// LatinSupercube.C, 25.7.99, Ilja Friedel
//
//////////////////////////////////////////////////////////////////////////////
//
// Status: ok
//
//////////////////////////////////////////////////////////////////////////////

#include "LatinSupercube.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void LatinSupercube::restart(void)
{
   int i;

   n=0;
   n--;
   for(i=0;i<GenNumber;i++) (GenList[i])->restart();
   operator++();
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void LatinSupercube::random_restart(void)
{
   int i;

   n=0;
   n--;
   for(i=0;i<GenNumber;i++) (GenList[i])->random_restart();
   operator++();
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void LatinSupercube::operator++(void)
{
   int i;

   check_next_dim();
   reset_next_dim();
//////////////////////////////////////////////////////////////////////////////
// we have to reset next_dim in every <<::operator++()>>
//////////////////////////////////////////////////////////////////////////////

   n++;
   if(n<length)
     {
       for(i=0;i<dimension;i++)
	 {
	   X[i]=(*(GenList[gen_at_dim[i]]))[LocalDim[i]];
	 }
       for(i=0;i<GenNumber;i++) ++(*(GenList[i]));
     }
     else
     {
        
        if(n==length) 
          {
#ifdef WARNINGS_EXTERNAL
	    cerr << "Reaching end of sequence!(LatinSupercube::operator++())"
		 << endl;
#endif
          }
        else
          {
	    cerr << "Error: End of sequence! (LatinSupercube::operator++())";
	    cerr << endl;
	    exit(1);
          }
    }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

LatinSupercube::~LatinSupercube()
{
   int i;

   if(gen_at_dim) delete [] gen_at_dim;
   gen_at_dim=nullptr;

   if(LocalDim)   delete [] LocalDim;
   LocalDim=nullptr;

   if(GenList)
     {
       for(i=0;i<GenNumber;i++) if(GenList[i]) delete(GenList[i]);
       delete [] GenList;
     }
   GenList=nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
