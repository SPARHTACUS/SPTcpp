CC = g++ 

#CCFLAGS = -ansi -Wall -pedantic -shared -O2
#CCFLAGS = -ansi -fullwarn -shared -O2 
CCFLAGS = -O2 -funroll-loops 

CFILES =	own_types.C \
		digit_gen.C \
		LatinSupercube.C \
		LatinHypercube.C \
		RandomizedTSSequence_base_2.C \
		RandomizedTSSequence.C \
		HaltonSequence.C \
		DigitalSequence.C \
		DigitalSequence_base_2.C \
		DigitalSequence_classic.C \
		DigitalSequence_medium_base.C \
		DigitalSequence_advanced.C \
		Sequence.C \
		Polynomial.C \
		Permutation.C \
		Counter.C \
		VectorMatrix.C \
		VectorRing.C \
		C_matrix.C \
		Ring.C 


OFILES =	own_types.o \
		digit_gen.o \
		LatinSupercube.o \
		LatinHypercube.o \
		RandomizedTSSequence_base_2.o \
		RandomizedTSSequence.o \
		HaltonSequence.o \
		DigitalSequence.o \
		DigitalSequence_base_2.o \
		DigitalSequence_classic.o \
		DigitalSequence_medium_base.o \
		DigitalSequence_advanced.o \
		Sequence.o \
		Polynomial.o \
		Permutation.o \
		Counter.o \
		VectorMatrix.o \
		VectorRing.o \
		C_matrix.o \
		Ring.o 

HFILES =	LatinSupercube.h \
		LatinHypercube.h \
		RandomizedTSSequence.h \
		HaltonSequence.h \
		DigitalSequence.h \
		Sequence.h \
		Permutation.h \
		Counter.h \
		digit_gen.h \
		options.h \
		own_types.h \
		VectorRing.h \
		VectorMatrix.h \
		C_matrix.h \
		Polynomial.h \
		Ring.h 

.C.o: ; $(CC) $(CCFLAGS) $<

libseq.a: $(CFILES) $(HFILES) makefile
	$(CC) $(CCFLAGS) -c $(CFILES)
	ar r libseq.a $(OFILES)
	ranlib libseq.a
	true libseq.a
	$(CC) $(CCFLAGS) SequenceTest.C -o SequenceTest -lseq -L.

