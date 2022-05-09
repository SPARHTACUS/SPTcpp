//////////////////////////////////////////////////////////////////////////////
//
// HaltonSequence.C, 8.8.99, Ilja Friedel
//
//////////////////////////////////////////////////////////////////////////////
//
// Status: not tested in context of class Sequence
//
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Copyright (C) 1992 - 1996, Alex Keller (keller@informatik.uni-kl.de)     //
// 							                    //
// All rights reserved                                                      //
//                                                                          //
// This software may be freely copied, modified, and redistributed          //
// provided that this copyright notice is preserved on all copies.          //
//                                                                          //
// You may not distribute this software, in whole or in part, as part of    //
// any commercial product without the express consent of the authors.       //
//                                                                          //
// There is no warranty or other guarantee of fitness of this software      //
// for any purpose.  It is provided solely "as is".                         //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#include "HaltonSequence.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

HaltonSequence::HaltonSequence(UL_int Dimension, UL_int offset, int *Primes)
   : Sequence(Dimension)
{
   n       = offset;
   n0      = offset;

   Radical = new double[dimension];
   assert(Radical);
   
   ownBase = (! Primes);   
   if(ownBase)
      Base = FirstPrimes((L_int)dimension);
   else
      Base = Primes;
   
   for(int j = 0; j < dimension; j++)
   {
      Radical[j] = 1.0 / (double) Base[j];
      X[j] = 0.0;
   }
   SetInstance(n0);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

HaltonSequence::~HaltonSequence()
{
  if(ownBase) delete [] Base;
  Base=nullptr;
  delete [] Radical;
  Radical=nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void HaltonSequence::operator++()
{
   const double one = 1.0 - 1e-10;
   double h, hh, remainder;
   
   n++;
   
   if(n & 8191)
   {
      for(int j = 0; j < dimension; j++)
      {
	 remainder = one - X[j];
	 
	 if(remainder < 0.0)
	    X[j] = 0.0;
	 else if(Radical[j] < remainder)
	    X[j] += Radical[j];
	 else
	 {
	    h = Radical[j];
	    
	    do
	    {
	       hh = h;
	       h *= Radical[j];
	    }
	    while(h >= remainder);
	    
	    X[j] += hh + h - 1.0;
	 }
      }
      /* updated 30.april 1996
	 if((h = X[j] + Radical[j]) <= one)
	    X[j] = h;
	 else
	 {
	    if(X[j] >= one)
	       X[j] = 0;
	    else
	    {
	       h = Radical[j];
	       
	       do
	       {
		  h *= Radical[j];
	       }
	       while(X[j] + h >= one); 
	       
	       X[j] += h / Radical[j] + h - 1.0;
	    }
	 }*/
   }
   else 
      /// change 5.10.94
      if(n >= 1073741824)   // == 2^30
	 SetInstance(0);
      else
	 /// change 5.10.94
	 SetInstance(n);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void HaltonSequence::SetInstance(const unsigned long Instance)
{
   unsigned long im;
   int           b;
   double        fac;
   
   n = Instance;
   
   for(int j = 0; j < dimension; j++)
   {
      X[j] = 0.0;
      fac = Radical[j];
      b = Base[j];
      
      for(im = n; im > 0; im /= b, fac *= Radical[j])
	 X[j] += fac * (double) (im % b);
   }
} 

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

int * HaltonSequence::FirstPrimes(const long N) const
{
   int *Prime, i, j, p, b;
   
   if(N == 0)
      return nullptr;
   
   Prime = new int[N];
   assert(Prime);
   
   Prime[0] = 2;
   
   for(p = 3, i = 1; i < N; p += 2)
   {
      Prime[i] = p;
      
      for(j = 1; (b = Prime[j] <= p / Prime[j]) && (p % Prime[j]) ; j++);
      
      if(b == 0)
	 i++;
   }
   
   return Prime;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

