//////////////////////////////////////////////////////////////////////////////
//                                                                        
// DigitalSequence.h, 9.8.99, Ilja Friedel               
//                                                                     
//////////////////////////////////////////////////////////////////////////////
//                                                                        
// Status: psi and eta should be configured internally
//         distinguishing with nullptr
//
//         error/inaccuracy in restart() possible, not completely tested
//
//////////////////////////////////////////////////////////////////////////////
//
// A source for these classes is for instance "Implementation  
// and Tests of Low-Discrepancy Sequences" von Bratley, Fox, Niederreiter 
// im "ACM Transactions on Modeling and Computer Simulation", Vol.2,     
// No.3, July 1992, Pages 195-213                                        
//
//////////////////////////////////////////////////////////////////////////////

#ifndef DIGITALSEQUENCE_USED
#define DIGITALSEQUENCE_USED

//////////////////////////////////////////////////////////////////////////////
// standard include files
//////////////////////////////////////////////////////////////////////////////

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
#include "digit_gen.h"

#include "Ring.h"
#include "VectorRing.h"
#include "Polynomial.h"
#include "Permutation.h"
#include "C_matrix.h"
#include "VectorMatrix.h"
#include "Sequence.h"
#include "Counter.h"

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

class  DigitalSequence;
extern DigitalSequence * new_DigitalSequence(C_matrix *);
extern DigitalSequence * new_DigitalSequence(C_matrix *, UL_int);
extern DigitalSequence * new_DigitalSequence(C_matrix *, UL_int, UL_int);

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

class DigitalSequence:public Sequence
{
protected:

    double     inv_base;
    Ring     * R;
    C_matrix * CM;

    UL_int NumDigits; // number of digits (to compute) in base b
    Counter *N;       // n digitwise in base b
    UL_int **Digit;   // digits of computed vector in base b

    RandomPermutation ** eta;  // [0..dim, 0..NumDigits]
    RandomPermutation ** psi;  // [0..NumDigits]

    void check_arguments(void);
public:

    virtual ~DigitalSequence();
             DigitalSequence(UL_int);
};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

class DigitalSequence_classic : public DigitalSequence
{
   void init(C_matrix *, UL_int, UL_int);
   void init_psi_and_eta(void);

public:
   void operator++();
   void random_restart(void);
   void restart(void);

   virtual ~DigitalSequence_classic();
            DigitalSequence_classic(C_matrix *);
            DigitalSequence_classic(C_matrix *, UL_int);
            DigitalSequence_classic(C_matrix *, UL_int, UL_int);
};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

class DigitalSequence_medium_base : public DigitalSequence
{
   UL_int *** DigitBuffer; // as digit, just for several vectors
   UL_int BufferFull;
   UL_int BufferLen;
   UL_int log_BufferLen;

   UL_int get_log_BufferLenForBase(UL_int);
   UL_int get_BufferLenForBase(UL_int);
   void   clear_DigitBuffer(void);
   void   init(C_matrix *, UL_int, UL_int);
   void   init_psi_and_eta(void);

public: 
   void  operator++();
   void  random_restart(void);
   void  restart(void);

   virtual ~DigitalSequence_medium_base();
            DigitalSequence_medium_base(C_matrix *);
            DigitalSequence_medium_base(C_matrix *, UL_int);
            DigitalSequence_medium_base(C_matrix *, UL_int, UL_int);
};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

class DigitalSequence_advanced : public DigitalSequence
{
   VectorRing   * VR;
   VectorMatrix * VCM;

   RVector *** DigitBuffer; // as digit, just for several vectors
   UL_int NumVectors;

   UL_int BufferFull;
   UL_int BufferLen;
   UL_int log_BufferLen;
   double inv_base_pow_times;

   UL_int get_times_for_base(UL_int);
   UL_int get_log_BufferLenForBase(UL_int);
   UL_int get_BufferLenForBase(UL_int);
   void   clear_DigitBuffer(void);
   void   init(C_matrix *, UL_int, UL_int);
   void   init_psi_and_eta(void);

public: 
   void  operator++();
   void  random_restart(void);
   void  restart(void);

   virtual ~DigitalSequence_advanced();
            DigitalSequence_advanced(C_matrix *);
            DigitalSequence_advanced(C_matrix *, UL_int);
            DigitalSequence_advanced(C_matrix *, UL_int, UL_int);
};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

class DigitalSequence_base_2 : public DigitalSequence
{
   UL_int     old_n;
   FixPoint * C;
   FixPoint * bitmap;

   FixPoint * eta;
   FixPoint   psi;

   void     transfer_C(void);
   void     init(C_matrix *, UL_int);

public:
   FixPoint query_bitmap(UL_int dim);
   void     random_restart(void);
   void     restart(void);
   void     calc_vector(UL_int tmp);
   void     operator++();
   virtual ~DigitalSequence_base_2();
            DigitalSequence_base_2(C_matrix *);
            DigitalSequence_base_2(C_matrix *, UL_int);
            DigitalSequence_base_2(C_matrix *, UL_int, UL_int);
};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#endif

