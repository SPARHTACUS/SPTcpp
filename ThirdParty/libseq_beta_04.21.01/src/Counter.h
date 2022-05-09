/////////////////////////////////////////////////////////////////////////////
//                                                                         
// Counter.h, 1.8.99, Ilja Friedel                       
//
/////////////////////////////////////////////////////////////////////////////
//                                                                         
// Status: hopefully finally working
//
/////////////////////////////////////////////////////////////////////////////
//
// Description: <base>-ary counter with <NumDigits> digits 
//
/////////////////////////////////////////////////////////////////////////////

#ifndef COUNTER_USED
#define COUNTER_USED

/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <assert.h>
#include "options.h"
#include "own_types.h"
#include "Permutation.h"
#include "Ring.h"
using namespace std;
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class Counter
{
 protected:
   Ring *  R;
   UL_int  memory;

   UL_int  *x;
   R_Elem  *diff;
   RandomPermutation ** psi;

 public:
   UL_int  base;
   UL_int  LastChangingDigit;
   UL_int  NumDigits;

   virtual      ~Counter();
                 Counter(Ring *,UL_int);
                 Counter(Ring *,UL_int,RandomPermutation **);
          void   init(Ring *,UL_int, RandomPermutation **);

   UL_int        Base       (void){return(base);}
   UL_int        memory_used(void){return(memory);}
   inline UL_int query     (void);
          void   set       (UL_int);
          void   reset     (void);
          void   operator++(void){increment_digit(0);}
          void   operator--(void){decrement_digit(0);}
	  void   increment_digit(UL_int);
	  void   decrement_digit(UL_int);
   inline UL_int operator[](const UL_int);
   inline UL_int difference(const UL_int);
   inline void   print(void);
};

/////////////////////////////////////////////////////////////////////////////

inline UL_int Counter::query(void)
{
   L_int i;
   UL_int res;

#ifdef WARNINGS_EXTERNAL
   cerr << "Warning: Will not notice overflow! (Counter::query())"<<endl;
#endif

   res=0;
   for(i=(L_int)NumDigits-1;i>=0;i--)
     {
       res=res*base+x[i];
     }
   return(res);
}

/////////////////////////////////////////////////////////////////////////////

inline void Counter::print(void)
{
   L_int i;

   cout << "Counter base=" << base << " <";
   for(i=(L_int)NumDigits-1;i>=0;i--) cerr << x[i];
   cout << ">" << endl;
}

/////////////////////////////////////////////////////////////////////////////

inline UL_int Counter::operator[](const UL_int j)
{
#ifdef DEBUG
   if(j>=NumDigits)
     {
       cerr << "Access out of range! ("<<j<<">=NumDigits="<<NumDigits;
       cerr << " in Counter::operator[])"<<endl;
       exit(1);
     }
#endif
   if(psi) return((*(psi[j]))[x[j]]);
   else    return(x[j]);
}

/////////////////////////////////////////////////////////////////////////////

inline UL_int Counter::difference(const UL_int j)
{
#ifdef DEBUG
   if(j>=NumDigits)
     {
       cerr << "Access out of range! ("<<j<<">=NumDigits="<<NumDigits;
       cerr << "in Counter::difference())" << endl;
       exit(1);
     }
#endif
   return(diff[j]);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#endif
