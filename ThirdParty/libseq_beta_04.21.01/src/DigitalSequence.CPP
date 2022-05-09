//////////////////////////////////////////////////////////////////////////////
//
//  DigitalSequence.C, 1.8.99, Ilja Friedel
//
//////////////////////////////////////////////////////////////////////////////
//
// Status:  nearly finished
//
//////////////////////////////////////////////////////////////////////////////
//
// Warning: using function "new_DigitalSequence" will cause
//          memory leakage, because no call to ~DigitalSequence_base_2()
//          or ~DigitalSequence_advanced() etc. will occur
//
//////////////////////////////////////////////////////////////////////////////

#include "DigitalSequence.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

DigitalSequence * new_DigitalSequence(C_matrix *C)
{
   if(C->R->base==2) return (new DigitalSequence_base_2(C));
   else
     {
#ifdef DEBUG
       cerr << "INFO: DEBUG is switched on. Using DigitalSequence_classic ";
       cerr << "instead of DigitalSequence_advanced in ";
       cerr << "new_DigitalSequence()!"<<endl;
       fflush(0);
       return (new DigitalSequence_classic(C));
#else
       return (new DigitalSequence_advanced(C));
#endif
     }
}

//////////////////////////////////////////////////////////////////////////////

DigitalSequence * new_DigitalSequence(C_matrix *C, UL_int dim)
{
   if(C->R->base==2) return (new DigitalSequence_base_2(C,dim));
   else
     {
#ifdef DEBUG
       cerr << "INFO: DEBUG is switched on. Using DigitalSequence_classic ";
       cerr << "instead of DigitalSequence_advanced in ";
       cerr << "new_DigitalSequence()!" << endl;
       fflush(0);
       return (new DigitalSequence_classic(C,dim));
#else
       return (new DigitalSequence_advanced(C,dim));
#endif
     }
}

//////////////////////////////////////////////////////////////////////////////

DigitalSequence * new_DigitalSequence(C_matrix *C, UL_int dim, UL_int offset)
{
   if(C->R->base==2) return (new DigitalSequence_base_2(C,dim,offset));
   else
     {
#ifdef DEBUG
       cerr << "INFO: DEBUG is switched on. Using DigitalSequence_classic ";
       cerr << "instead of DigitalSequence_advanced in ";
       cerr << "new_DigitalSequence()"<<endl;
       fflush(0);
       return (new DigitalSequence_classic(C,dim,offset));
#else
       return (new DigitalSequence_advanced(C,dim,offset));
#endif
     }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

DigitalSequence::DigitalSequence(UL_int dim):Sequence(dim)
{
   eta          = nullptr;
   psi          = nullptr;
   N            = nullptr;
   R            = nullptr;
   CM           = nullptr;
   Digit        = nullptr;
   sprintf(    name,"DigitalSequence");
   sprintf(filename,"<<Not a File! (DigitalSequence)>>");
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void DigitalSequence::check_arguments(void)
{
   if(dimension>CM->dimension)
     {
       cerr << "Error: Dimension of generator C_matrix is not big enough! ";
       cerr << "(DigitalSequence::check_arguments(void))" << endl;
       cerr << "(dimension=" << dimension;
       cerr << ", CM->dimension=" << CM->dimension << ")" << endl;
       exit(1);
     }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

DigitalSequence::~DigitalSequence()
{
   L_int i;

   if(N) delete N;
   N=nullptr;
   if(Digit)
     {
       for(i=0;i<dimension;i++)
	 {
	   delete[] (Digit[i]);
	 }
       delete[] Digit;
     }
   Digit=nullptr;
#ifdef DESTRUCTOR
   cerr << "Info: Destructor DigitalSequence::~DigitalSequence()"
	<< " was called." << endl;
#endif
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
