//////////////////////////////////////////////////////////////////////////////
//
// HaltonSequence.h, 6.8.99, Ilja Friedel
//
//////////////////////////////////////////////////////////////////////////////
//
// Status: just for completeness the Halton sequence. Main code ripped
//         with consent of the original authors.
//
// Commun. ACM Vol.7 Nr.12 Dec 1964
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Copyright (C) 1992-1997 by Alex Keller (keller@informatik.uni-kl.de)     //
// 				and Johannes Timmer	                    //
//                                                                          //
// All rights reserved                                                      //
//                                                                          //
// This software may be freely copied, modified, and redistributed          //
// provided that this copyright notice is preserved on all copies.          //
//                                                                          //
// You may not distribute this software, in whole or in part, as part of    //
// any commercial product without the express consent of the authors.       //
//                                                                          //
// There is no warranty or other guarantee of fitness of this software      //
// for any purpose. It is provided solely "as is".                          //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#ifndef HALTONSEQUENCE_USED
#define HALTONSEQUENCE_USED

//////////////////////////////////////////////////////////////////////////////
// standard include files
//////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

//////////////////////////////////////////////////////////////////////////////
// own include files
//////////////////////////////////////////////////////////////////////////////

#include "Sequence.h"
using namespace std;
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

class HaltonSequence : public Sequence
{
   double *Radical;
   int    *Base;
   char    ownBase;

   int    *FirstPrimes(const long N) const;   
   void    SetInstance(const unsigned long Instance);
public:

   virtual     ~HaltonSequence();
		//                HaltonSequence(int, int *Primes = nullptr);
                HaltonSequence(UL_int, UL_int offset = 0, int *Primes = nullptr);
   virtual void operator++();

//////////////////////////////////////////////////////////////////////////////

   void restart(void)
     {
       SetInstance(n0);
     }

//////////////////////////////////////////////////////////////////////////////

   void random_restart(void)
     {
       cerr << "Error: random_restart() not possible! ";
       cerr << "(HaltonSequence::random_restart())" << endl;
       exit(1);
     }
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif
