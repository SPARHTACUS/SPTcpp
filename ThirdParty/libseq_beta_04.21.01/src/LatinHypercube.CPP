//////////////////////////////////////////////////////////////////////////////
//
// LatinHypercube.C, 6.8.99, Ilja Friedel
//
//////////////////////////////////////////////////////////////////////////////
//
// Status: tested, ok
//
//////////////////////////////////////////////////////////////////////////////

#include "LatinHypercube.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

LatinHypercube::LatinHypercube(UL_int dim, UL_int len) : Sequence(dim) 
{
   int i;

   n          = 0;
   n0         = 0;
   length     = len;
   inv_length = 1.0/((double)length);

   PermList = new LazyRandomPermutation * [dimension];
   if(!(PermList))
     {
       cerr << "Error: Out of memory! (LatinHypercube::LatinHypercube)"<<endl;
       exit(1);
     }

   for(i=0;i<dimension;i++)
     {
       PermList[i]=new LazyRandomPermutation(length);
       
       if(!(PermList[i]))
	 {
	   cerr << "Error: Out of memory! (LatinHypercube::LatinHypercube)";
	   cerr << endl;
	   exit(1);
	 }
     }
   random_restart();
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

LatinHypercube::~LatinHypercube()
{
   L_int i;

   if(PermList)
     {
       for(i=0;i<dimension;i++) if(PermList[i]) delete (PermList[i]);
       delete [] PermList;
     }
   PermList=nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void LatinHypercube::operator++()
{
   L_int i;

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
	   X[i]=((double)((*(PermList[i]))[n])+drand47())*inv_length;
	 }
     }
   else
     {
       if(n==length) 
	 {
#ifdef WARNINGS_EXTERNAL
	   cerr << "Reaching end of sequence!(LatinHypercube::operator++())"
		<< endl;	   
#endif
	 }
       else
	 {
	   cerr << "Error: End of sequence! (LatinHypercube::operator++())";
	   cerr << endl;
	   exit(1);
	 }
     }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void LatinHypercube::random_restart(void)
{
   int i;

   n=0;
   n--;
   for(i=0;i<dimension;i++) ++(*(PermList[i]));
   operator++();
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


