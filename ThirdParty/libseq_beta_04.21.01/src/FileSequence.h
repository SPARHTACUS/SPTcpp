//////////////////////////////////////////////////////////////////////////////
//
// FileSequence.h, 28.7.99, Ilja Friedel
//
//////////////////////////////////////////////////////////////////////////////
//
// Status: unclear. Supposed usage is loading/saving sequences to disk.
//         Don't know if anybody needs this.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef FILESEQUENCE_USED
#define FILESEQUENCE_USED

//////////////////////////////////////////////////////////////////////////////
// Allgemeine Includefiles
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <assert.h>
#include <cmath>
#include "Sequence.h"
#include "digit_gen.h"

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

class FileSequence:public Sequence
{
   double * Seq;
   UL_int   length;

//////////////////////////////////////////////////////////////////////////////

public: 

   FileSequence(Sequence *S, UL_int dim, UL_int len):Sequence(dim)
     {
       UL_int i,akdim;
       
       n0     = 0;
       n      = 0;
       length = len;
       Seq    = new double[dimension*length];
       assert(Seq);
       
       for(i=0;i<length;i++) 
	 for(akdim=0;akdim<dim;akdim++)
	   {
	     Seq[i*dimension+akdim]=(*Seq)[akdim];
	     ++(*Seq);
	   }
     }

//////////////////////////////////////////////////////////////////////////////

   void random_restart(void)
     {
       cerr << "Error: FileSequence::random_restart() not possible!"<<endl;
       exit(1);
     }
   
//////////////////////////////////////////////////////////////////////////////

   void restart(void)
     {
       UL_int i;
       
       n=0;
       for(i=0;i<dimension;i++) X[i]=Seq[n*dimension+i];
     }

//////////////////////////////////////////////////////////////////////////////

   operator++()
     {
       int i;
       check_next_dim();
       reset_next_dim();
       
       n++;
       for(i=0;i<dimension;i++) X[i]=Seq[n*dimension+i];
     }

//////////////////////////////////////////////////////////////////////////////

   sort(void)
     {
     }

//////////////////////////////////////////////////////////////////////////////

   randomize_order(void)
     {
     }

//////////////////////////////////////////////////////////////////////////////

   save(char * filename);
   
//////////////////////////////////////////////////////////////////////////////

   load(char * filename);

//////////////////////////////////////////////////////////////////////////////

   FileSequence(char * filename);

};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#endif
