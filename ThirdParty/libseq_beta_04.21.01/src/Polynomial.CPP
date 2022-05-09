//////////////////////////////////////////////////////////////////////////////
//
// Polynomial.C, 29.7.99, Ilja Friedel
//
//////////////////////////////////////////////////////////////////////////////
//
// Status: operators "-", "/" and "%" not extensivly tested
//
//////////////////////////////////////////////////////////////////////////////

#include "Polynomial.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// This constructor should always return the zero polynomial.
//////////////////////////////////////////////////////////////////////////////

Polynomial::Polynomial(Ring *T)
{
   R        =  T;
   deg      =  0;
   digit[0] =  0;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// This constructor computes the polynomial from a natural number  
//////////////////////////////////////////////////////////////////////////////

Polynomial::Polynomial(Ring *T, UL_int n)
{
   R        =  T;

   deg      =  0;
   digit[0] =  0;

   if(n!=0)
     {
       while(n!=0)
	 {
	   digit[deg]=n%(R->base);
	   n=n/(R->base);
	   deg++;
	 }
       deg--;
     }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

Polynomial Polynomial::operator + (const Polynomial& b) const
{
    L_int i;
   UL_int lmax;
   UL_int lmin;
   Polynomial P;

   if(R!=b.R)
     {
       cerr << "Error: Can't add polynomials over different rings!" << endl;
       exit(1);
     }

   lmax=deg;
   if(lmax<b.deg) lmax=b.deg;
   lmin=deg;
   if(lmin>b.deg) lmin=b.deg;

   P.R = R;
   for(i=0;i<=lmin;i++) (P.digit)[i]=R->add(digit[i],b.digit[i]);
   if(deg > b.deg) for(i=(L_int)lmin+1;i<=lmax;i++) (P.digit)[i]=digit[i];
              else for(i=(L_int)lmin+1;i<=lmax;i++) (P.digit)[i]=b.digit[i];
   P.deg=lmax;
   if(lmin==lmax) 
     while(lmax>0 && P.digit[lmax]==0) lmax--;
   P.deg=lmax;
     
   return(P);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

Polynomial Polynomial::operator - (const Polynomial& b) const
{
    L_int i;
   UL_int lmax;
   UL_int lmin;
   Polynomial P;

   if(R!=b.R)
     {
       cerr << "Error: Can't subtract polynomials over different rings!" 
	    << endl;
       exit(1);
     }

   lmax=deg;
   if(lmax<b.deg) lmax=b.deg;
   lmin=deg;
   if(lmin>b.deg) lmin=b.deg;

   P.R = R;
   for(i=0;i<=lmin;i++) (P.digit)[i]=R->add(digit[i],R->minus(b.digit[i]));
   if(deg > b.deg) for(i=(L_int)lmin+1;i<=lmax;i++) (P.digit)[i]=digit[i];
   else 
     for(i=(L_int)lmin+1;i<=lmax;i++) 
       (P.digit)[i]=R->minus(b.digit[i]);

   P.deg=lmax;
   if(lmin==lmax) 
     while(lmax>0 && P.digit[lmax]==0) lmax--;
   P.deg=lmax;
     
   return(P);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

Polynomial Polynomial::operator * (const Polynomial& b) const
{
   int i,j;
   UL_int l;
   Polynomial P;

   if(R!=b.R)
     {
       cerr << "Error: Can't multiply polynomials over different rings!";
       cerr << endl;
       exit(1);
     }
   l=deg+b.deg;
   if(l>=MAX_POLY_LEN)
     {
       cerr << "Error: Overflow while multipying two polynomials!" << endl;
       cerr << "Please increase MAX_POLY_LEN. (options.h)" << endl;
       exit(1);
     }
   P.R = R;

   for(i=0;i<=l;i++) (P.digit)[i]=0;
   for(i=0;i<=deg;i++)
   {
      for(j=0;j<=b.deg;j++)
      {
         (P.digit)[i+j]=R->add((P.digit)[i+j],
                R->mult(digit[i],b.digit[j]));
      }
   }
   P.deg=l;
   while(P.deg>0 && P.digit[P.deg]==0) P.deg--;

   return(P);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

Polynomial Polynomial::operator / (const Polynomial& b) const
{
   int i,j;
   UL_int l;
   Polynomial P;
   R_Elem c,d;
   R_Elem h[MAX_POLY_LEN];

   if(R!=b.R)
     {
       cerr << "Error: Can't divide polynomials over different rings!";
       cerr << endl;
       exit(1);
     }

   if(b.deg>deg) return(Polynomial(R)); // the zero polynomial

   l=deg-b.deg;
   if(l>=MAX_POLY_LEN)
     {
       cerr << "Error: Overflow while dividing two polynomials!" << endl;
       cerr << "Very strange." << endl;
       exit(1);
     }
   if(b.digit[b.deg]==0)
     {
       cerr << "Error: Trying to divide by a polynomial with leading "
	    << "coefficient=0!" << endl;
       exit(1);
     }

   P.R = R;
   for(i=0;i<=deg;i++) h[i]=digit[i];

   for(i=l;i>=0;i--)
     {
       P.digit[i]=R->mult(R->invert(b.digit[b.deg]),h[deg-l+i]);
       for(j=b.deg;j>=0;j--)
	 {
	   h[i+j]=R->add(h[i+j],
			 R->minus(R->mult(P.digit[i],b.digit[j])));
	 }
     }
   P.deg=l;
   
   return(P);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

Polynomial Polynomial::operator % (const Polynomial& b) const
{
   int i,j;
   UL_int l;
   Polynomial P;

   if(R!=b.R)
     {
       cerr << "Error: Can't modulo polynomials over different rings!";
       cerr << endl;
       exit(1);
     }

   P=*this-b*(*this/b);

   return(P);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Bijection between Polynomial and UL_int
//////////////////////////////////////////////////////////////////////////////

UL_int monic_poly_to_ulong(const Polynomial& a)
{
   L_int i;
   UL_int n;
   UL_int b;
   UL_int count;

   if(a.digit[a.deg]!=1)
     {
       cerr << "Error: Polynomial ";
       for(i=(L_int)a.deg;i>=0;i--) cerr << " " << a.digit[i];
       cerr << " isn't monic! (monic_poly_to_ulong())" << endl;
       exit(1);
     }

   if(a.deg<1)
     {
       cerr << "Error: Polynomial must have positive degree!" << endl;
       exit(1);
     }

   b=a.R->base;
   count=0;
   for(i=0;i<a.deg-1;i++) count=(count+1)*b;

//////////////////////////////////////////////////////////////////////////////
//
// Theoretically we could easily get unnoticed overflow. But in the context
// of this library it shouldn't happen at all.
//
//////////////////////////////////////////////////////////////////////////////

   n=a.digit[a.deg-1];
   for(i=(L_int)a.deg-2;i>=0;i--) n=n*b+a.digit[i];
   n=n+count;

   return(n);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Bijection between UL_int and Polynomial
//////////////////////////////////////////////////////////////////////////////

Polynomial ulong_to_monic_poly(UL_int n, Ring * R)
{
   Polynomial P;
   UL_int tmp[64];
    L_int i;
   UL_int l;
   UL_int deg;
   UL_int count;

   deg  =0;
   count=0;

   while((L_int)n-(L_int)((count+1)*R->base)>=0)
   {
      deg++;
      count=(count+1)*R->base;
   }

   n=n-count;
   i=0;
   while((i<=deg))
   {
      tmp[i]=n%R->base;
      n     =n/R->base;
      i++;
   }
   tmp[i]=1;
   l=(UL_int)i;

   P.R=R;
   P.deg   =l;
   for(i=0;i<=l;i++) (P.digit)[i]=tmp[i];

   return(P);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// The following function returns the first n (lexicalically ordered)
// irreducible polynomials over a field. (IT ONLY WORKS WITH A FIELD!)
// The basic idea is the same as the sieve of Erathostenes.
// For that we use the canonical bijections ulong_to_monic_poly and
// monic_poly_to_ulong.
//////////////////////////////////////////////////////////////////////////////

Polynomial * first_irr_polynomials(UL_int n, Ring * R)
{
   char    * table;
   Polynomial * irr_poly;
   Polynomial P,Q,K,S;
   UL_int table_length_deg;
   UL_int i_deg, j_deg;
   UL_int i,j,tmp;

   table=new char[TABLE_LENGTH];

   for(i=0;i<TABLE_LENGTH;i++)
   {
      table[i]=1;
   }

//////////////////////////////////////////////////////////////////////////////
// The following loop could (as in the standard sieve) be reduced
// to about sqrt(TABLE_LENGTH). An important difference is, that
// the degrees of the polynomials have to be used instead sqrt.
//////////////////////////////////////////////////////////////////////////////

   P=ulong_to_monic_poly(TABLE_LENGTH,R);
   table_length_deg=P.deg;

   for(i=0;i<TABLE_LENGTH;i++)
   {
      if(table[i])
      {
         j=0;

/////////////////////////////////////////////////////////////////////////////
// I think the stopping criterion in the next 20 lines is ok now.
/////////////////////////////////////////////////////////////////////////////

	 Q=ulong_to_monic_poly(i,R);
	 K=ulong_to_monic_poly(j,R);
	 i_deg =  Q.deg;
	 j_deg =  K.deg;

	 while(i_deg + j_deg <= table_length_deg)
	   {
	     S = Q * K;
	     tmp=monic_poly_to_ulong(S);
	     if(tmp<TABLE_LENGTH)
	       {
		 table[tmp]=0;
	       }
	     
	     j++;
	     K=ulong_to_monic_poly(j,R);
	     j_deg=K.deg;
	   }
      }
   }

//////////////////////////////////////////////////////////////////////////////
// The computation stops here. We have to convert the 1's in the table
// back to monic polynomials. If we couldn't find enough, we have
// to increase the table size.
//////////////////////////////////////////////////////////////////////////////

   i=0;
   j=0;

   irr_poly=new Polynomial[n];

   while((i<n)&&(j<TABLE_LENGTH))
   {
      if(table[j])
      {
	irr_poly[i]=ulong_to_monic_poly(j,R);
	i++;
      }
      j++;
   }

   if(i!=n)
     {
       cerr << "Error: Please choose a bigger value for TABLE_LENGTH!" << endl;
       exit(1);
     }

   delete[] table;
   table=nullptr;

   return(irr_poly);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void print_poly(const Polynomial& P)
{
   L_int j;

   cout << "Polynomial"; 
   for(j=(L_int)P.deg;j>=0;j--)
     cout << " " << (P.digit)[j];
   cout << "  (degree=" << P.deg << ") in decr. order" << endl;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// The one polynomial.
//////////////////////////////////////////////////////////////////////////////

Polynomial one(Ring * R)
{
   Polynomial P;
   P.deg=0;
   P.digit[0]=1;
   P.R=R;
   return(P);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

Polynomial::~Polynomial()
{
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// compare function with is_prime()
//////////////////////////////////////////////////////////////////////////////
int Polynomial::is_irreducible(void)
{
  UL_int i,tmp;

  if(deg==0) return 0;
  if(deg==1) return 1;

  i=R->base;
  tmp=int_pow(R->base,deg/2+1);

  while(i<tmp) 
    {
      if(((*this)%Polynomial(R,i)).return_int()==0) return(0);
      i++;
    }
  return(1);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

Polynomial get_irr_poly_of_deg(UL_int n, Ring * R)
{
  int i;
  Polynomial P;

  cout << int_pow(R->base,n) << "  " << int_pow(R->base,n+1) << endl;

  for(i=int_pow(R->base,n);i<int_pow(R->base,n+1);i++)
    {
      cout << i << " ";
      P=Polynomial(R,i);
      if(P.is_irreducible()) return(P);
    }
  
  cerr << "Error: Couldn't find irreducible poly of degree n="<<n<<"! "
       << "(get_irr_poly_of_deg(UL_int n, Ring * R))"<<endl;
  exit(1);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
