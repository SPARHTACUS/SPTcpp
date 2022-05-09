/////////////////////////////////////////////////////////////////
//                                                             
// own_types.C, 09/12/00, Ilja Friedel                         
//
/////////////////////////////////////////////////////////////////
//                                                             
// Status: ok
//                                                             
/////////////////////////////////////////////////////////////////

#include "own_types.h"
using namespace std;

/////////////////////////////////////////////////////////////////

void check_own_types(void)
{
   int ok;
   ok=1;

   if(sizeof(US_int)!=2)
     {
       cerr << "Fatal Error: sizeof(US_int) is " << 
	 sizeof(US_int) << " and should be " <<2<< endl;
       ok=0;
     }
   if(sizeof(UL_int)!=4)
     {
       cerr << "Fatal Error: sizeof(UL_int) is " << 
	 sizeof(UL_int) << " and should be " <<4<< endl;
       ok=0;
     }
   if(sizeof(ULL_int)!=8)
     {
       cerr << "Fatal Error: sizeof(ULL_int) is " << 
	 sizeof(ULL_int) << " and should be " <<8<< endl;
       ok=0;
     }

   if(sizeof(S_int)!=2)
     {
       cerr << "Fatal Error: sizeof(S_int) is " << 
	 sizeof(S_int) << " and should be " <<2<< endl;
       ok=0;
     }
   if(sizeof(L_int)!=4)
     {
       cerr << "Fatal Error: sizeof(L_int) is " << 
	 sizeof(L_int) << " and should be " <<4<< endl;
       ok=0;
     }
   if(sizeof(LL_int)!=8)
     {
       cerr << "Fatal Error: sizeof(LL_int) is " << 
	 sizeof(LL_int) << " and should be " <<8<< endl;
       ok=0;
     }
      
   if(!ok)
     {
       cerr << "Please change definitions in file 'lib/own_types.C' or " <<
               "choose another compiler (g++)!" << endl;
       exit(1);
     }
}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
