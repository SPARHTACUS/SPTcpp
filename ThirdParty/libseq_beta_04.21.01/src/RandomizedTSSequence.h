//////////////////////////////////////////////////////////////////////////////
//
// RandomizedTSSequence.h, 8.8.99, Ilja Friedel
//
//////////////////////////////////////////////////////////////////////////////
//
// Status: tested
//         
//////////////////////////////////////////////////////////////////////////////

// disable warning C4806: '&' : unsafe operation: no value of type 'bool' promoted to type 'unsigned __int64' can equal the given constant
// i don't know if it is okay to ignore
#pragma warning(disable: 4806)
#pragma warning(disable: 4554)

#ifndef RANDOMIZEDTSSEQUENCE_USED
#define RANDOMIZEDTSSEQUENCE_USED

//////////////////////////////////////////////////////////////////////////////
// Basic include files
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <assert.h>
#include <cmath>

//////////////////////////////////////////////////////////////////////////////
// Sequence include files
//////////////////////////////////////////////////////////////////////////////

#include "options.h"
#include "own_types.h"
#include "Sequence.h"
#include "Counter.h"
#include "Permutation.h"
#include "C_matrix.h"
#include "DigitalSequence.h"

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

class  RandomizedTSSequence;
extern RandomizedTSSequence * new_RandomizedTSSequence(C_matrix *, UL_int len);
extern RandomizedTSSequence * new_RandomizedTSSequence(C_matrix *, UL_int, UL_int);
extern RandomizedTSSequence * new_RandomizedTSSequence(C_matrix *, UL_int, UL_int, UL_int);

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

class RandomizedTSSequence:public Sequence
{
   LazyRandomPermutation * Perm[65];
   double                  scale;
   double                  powers[65];

//////////////////////////////////////////////////////////////////////////////

protected:

   FixPoint      ** Seq;  // arrays Seq und P must have the same type 
   UL_int        ** P;    // and size for LatinSupercube
   unsigned char ** ChangingDigit;

   UL_int           length;   
   UL_int           base;
   UL_int           NumDigits;
   UL_int           Component_of_LatinSupercube;
   double           inv_Max_int;

//////////////////////////////////////////////////////////////////////////////

           void     get_memory(UL_int,UL_int);
           void     sort_sequence_and_generate_permutation(void);

   inline  void     fixed_point_to_digits(FixPoint, UL_int, UL_int *);
   inline  FixPoint digits_to_fixed_point(UL_int, UL_int *);
           UL_int   partition(UL_int, UL_int, UL_int);
           void     stochastic_quicksort(UL_int, UL_int, UL_int);
           void     apply_permutation_for_latin_supercube(void);
           //void     apply_permutation_for_latin_supercube2(void);

   virtual void     calc_changing_digits(void);
           void     init(C_matrix *, UL_int, UL_int, UL_int);
   virtual void     randomize_sequence(void);

//////////////////////////////////////////////////////////////////////////////

public: 
   virtual         ~RandomizedTSSequence();
                    RandomizedTSSequence(UL_int dim):Sequence(dim)
		      {
			Perm[1]=nullptr;
		      }
		    // just forwarding dimension to Sequence for RTSS_base_2
                    RandomizedTSSequence(C_matrix *, UL_int len);
                    RandomizedTSSequence(C_matrix *, UL_int, UL_int);
                    RandomizedTSSequence(C_matrix *, UL_int, UL_int, UL_int);

   virtual void     operator++(void);
   virtual void     random_restart(void);
   virtual void     restart(void);

};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

class RandomizedTSSequence_base_2:public RandomizedTSSequence
{
           void calc_changing_digits(void);
           void init(C_matrix *, UL_int, UL_int, UL_int);
           void randomize_sequence(void);

public: 
                RandomizedTSSequence_base_2(C_matrix *, UL_int len);
                RandomizedTSSequence_base_2(C_matrix *, UL_int dim, UL_int len);
                RandomizedTSSequence_base_2(C_matrix *, UL_int, UL_int, UL_int);
           void random_restart(void);
};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

inline void RandomizedTSSequence::fixed_point_to_digits(FixPoint fp, UL_int base, UL_int * p)
{
   UL_int i;
   ULL_int tmp;
   ULL_int precision;

   tmp       = (ULL_int)fp;
   precision = 1;
   i         = 0;
   while(!(precision&(4294967296ULL * 4294967295ULL)))
   {
      i++;
      precision=precision*base;
      tmp=tmp*base;
      p[i]=(UL_int)(tmp>>32);            // digit
      tmp=tmp&4294967295ULL;
   }
   p[0]=i;                       // number of digits
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

inline FixPoint RandomizedTSSequence::digits_to_fixed_point(UL_int base, UL_int * p)
{
   UL_int i;
   ULL_int tmp;

   tmp       = 0;
   for(i=p[0];i>0;i--)
   {
      tmp=(((ULL_int)(p[i]))<<32)|tmp;
      tmp=tmp/base;
   }

   if(tmp != tmp &4294967295ULL){
      cerr << "Error in RandomizedTSSequence::digits_to_fixed_point()" << endl;
      exit(1);
   }
   return((UL_int)tmp);  
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#endif
