//////////////////////////////////////////////////////////////////////////////
//                                                                        
// Polynomial.C, 29.7.98, Ilja Friedel                       
//                                                                        
//////////////////////////////////////////////////////////////////////////////

#ifndef POLYNOMIAL_USED
#define POLYNOMIAL_USED

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <assert.h>
#include "own_types.h"
#include "options.h"
#include "Ring.h"
using namespace std;
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

class Polynomial;

extern UL_int        monic_poly_to_ulong(const Polynomial& a);
extern Polynomial    ulong_to_monic_poly(const UL_int n, const Ring * R);

extern Polynomial  * first_irr_polynomials(UL_int n, Ring * R);
extern Polynomial    get_irr_poly_of_deg(UL_int n, Ring * R);
extern void          print_poly(const Polynomial& P);
extern Polynomial    one(Ring * R);

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Canonical operations for a polynomial ring.
//////////////////////////////////////////////////////////////////////////////

class Polynomial
{

public:

   Ring    *R;
   UL_int   deg;
   R_Elem   digit[MAX_POLY_LEN]; 

   virtual ~Polynomial();
   Polynomial(Ring *);
   Polynomial(Ring *,UL_int);
   Polynomial()
     {
       R=nullptr;
       deg = 0;
     }

   UL_int return_int(void)
     {
       int i;
       UL_int n;

       n=0;
       for(i=deg;i>=0;i--) n=(R->base)*n+digit[i];
       return(n);
     }

   R_Elem apply(R_Elem x)
     {
       int i;
       R_Elem p;

       p=0;
       for(i=deg;i>=0;i--) p=R->add(R->mult(x,p),digit[i]);
       return(p);
     }

   void print(void)
     {
       int i;

       for(i=deg;i>0;i--) if(digit[i]) cout << digit[i] << "*x^"<<i<<" + ";
       cout << digit[0] << endl;
     }

   int is_irreducible(void);

   Polynomial operator + (const Polynomial&) const;
   Polynomial operator - (const Polynomial&) const;
   Polynomial operator * (const Polynomial&) const;
   Polynomial operator / (const Polynomial&) const;
   Polynomial operator % (const Polynomial&) const;
};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#endif
