/*******************************************************
/                                                      *
/    Packet SparseMatrix                               *
/                                                      *
/    Storage sparse matrixes and solve linear systems  *
/                                                      *
/    This pack uses the Strategy Pattern to add        *
/       linear systems solution methods                *
/                                                      *
/       03/2001                                        *
/       JMarco                                         *
/       MNAgostini                                     *
/                                                      *
/       Update in 11/2001 (SEPOPE paper)               *
/          - English language                          *
/          - classes names                             *
/          - ()                                        *
/          - etc.                                      *
/                                                      *
/       Update in 07/2002 (MNAgostini)                 *
/          - inicialization of valarrays of            *
/            SparseMatrix in zero                      *
/          - do not insert new pairs<int,T> in Sparse- *
/            Matrix if T is zero                       *
/                                                      *
/       Update in 10/08/2006 (Felipe Trevisan)         *
/          - matrix-matrix multiplication              *
/                                                      *
/******************************************************/


#ifndef _SparseMatrix
#define _SparseMatrix



// Disable warning C4786 in Visual Basic
#ifdef _MSC_VER     // Developer -> defined automatically
	#pragma warning (disable:4786)
#endif



// ========== C/C++ Necessary inclusions ==========
#ifndef _PGI
#include <cstdlib>
#include <cstdio>
#endif
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <valarray>
#include <complex>
// ================================================

// Incorporate all members of the std namespace (standard library)




// Classes declaration
template <class T> class SparseMatrix;
template <class T> class Pos_Mat;

template <class T> class LS_Strategy;



// Class Matrix "Positioner" (Pos_Mat)
// "Run" the matrix
template <class T>
class Pos_Mat {

	// SparseMatrix is friend of Pos_Mat
	friend class SparseMatrix<T>;


  private:

	SparseMatrix<T> *itMat;	// pointer to the matrix

	int itLin; // wanted line;
	int itCol; // wanted column;

	T itValue;
	T itValue_old;

	std::map<int,T>* ptr_map_line;   // pointer to line required


	// Constructor
	Pos_Mat (SparseMatrix<T> *mat) {

		itMat = mat;
		ptr_map_line = nullptr;

		// Reset itValue (s)
		itValue = (T) 0.0;
		itValue_old = (T) 0.0;

	}

	// Destructor
	~Pos_Mat () {};


  public:

	// Reutilize operator = (const T &e)
	Pos_Mat<T>& operator = (Pos_Mat<T> &itmat) {

		// If the assertion is between elements of the same matrix
		if (&itmat==this) *this = itmat.itValue_old;

		// If different matrixes
		else *this = itmat.itValue;


		return *this;

	}


  public:

	//! Acesso direto aos elementos da matriz.
	Pos_Mat<T>& operator [] (const int &c) {

		int aux_int;
		typename std::map<int,T>::const_iterator   map_T_it;

		// Set column
		itCol = c;

		// Test dimension
		if (itMat->ncol) {

			if (itCol < itMat->ncol) {

				if (itLin < itMat->nlin) {

					// If is element of the diagonal
					if (itCol == itLin) {
						itValue = itMat->diag[c];
					}

					// If not diagonal
					else {

						if (itMat->flagsym)  // only superior diagonal
							if ( itCol < itLin) {

								aux_int = itLin;
								itLin = itCol;
								itCol = aux_int;

								ptr_map_line = &(itMat->inilin[itLin]);

							}

						// Test if exist the element
						map_T_it = ptr_map_line->find(itCol);
						if (map_T_it!=ptr_map_line->end()) itValue = (*map_T_it).second;

					}  // if diagonal

				} else {  // if (itLin < itMat->nlin) {

					std::cout << "ERROR: Line out of range. (" << itLin << " >= " << itMat->nlin << ")" << std::endl;
					itLin = itMat->nlin;
					ptr_map_line = nullptr;
					itValue = (T) 0.0;
					itValue_old = (T) 0.0;

				}  // if (itLin < itMat->nlin) {

			} else { // if (itCol < itMat->ncol) {

				std::cout << "ERROR: Column out of range. (" << itCol << " >= " << itMat->ncol << ")" << std::endl;
				itCol = itMat->ncol;
				itValue = (T) 0.0;
				itValue_old = (T) 0.0;

			}  // if (itCol < itMat->ncol) {

		} else {  // if (itMat->ncol) {

			std::cout << "ERROR: Matrix has not dimensions. (" << itCol << "," << itLin << ")" << std::endl;
			itLin = 0;
			itCol = 0;
			itValue = (T) 0.0;
			itValue_old = (T) 0.0;

		}  // if (itMat->ncol) {

		return *this;

	}


	// operator T() - cast Pos_Mat to T
	operator T() { return itValue; }


	// operator =
	Pos_Mat<T>& operator = (const T &e) {

		typename std::map<int,T>::iterator   map_T_it;


		if ((itLin < itMat->nlin) && (itCol < itMat->ncol)) {

			// If it is diagonal
			if (itLin == itCol) {
				itMat->diag[itCol] = e;
			}

			// If not diagonal
			else {

				// Test if the element exists, and update
				map_T_it = ptr_map_line->find(itCol);
				if (map_T_it!=ptr_map_line->end()) {
					(*map_T_it).second = e;
				}

				// if not, insert a new (if it's not zero)
				else if ( e != (T) 0.0) {
					ptr_map_line->insert( ptr_map_line->end() , std::pair<int,T>(itCol,e) );
					itMat->nnz++;
					if (itMat->flagsym) itMat->nnz++;
				}

			} // if not diagonal

			// Update itValue - lets linkage of assertions
			itValue = e;

			// Set the value in the strategy
			if (itMat->strategy) itMat->strategy->Set_Val(itLin,itCol,e);


		}  // if ((itLin < itMat->nlin) && (itCol < itMat->ncol)) {

		return *this;

	}  // Pos_Mat<T>& operator= (const T &e) {


}; // class Pos_Mat



//! Class Sparse Matrix (SparseMatrix)
/*! Stores (represents) sparse matrices
*/
template <class T>
class SparseMatrix {

	// Pos_Mat and others are friend of SparseMatrix
	friend class Pos_Mat<T>;


  private:

	
	int nlin;                       //!< number of lines
	int ncol;                       //!< number of columns
	int nnz;                        /*!< number of non-zero elements
	                                   (include all elements: diagonal and both upper and lower triangle)
									*/
	short int flagsym;              //!< flag that indicates if the matrix is symmetric (1) or not (0)
	std::valarray<T> diag;               //!< std::vector of diagonal values (l = c)

	std::vector< std::map<int,T> >  inilin;   //!< std::vector of maps; each std::map contain one line

	Pos_Mat<T> *it;                 //!< auxiliary variable used at the operators overloading (position on matrix)

	LS_Strategy<T> *strategy;       //!< strategy to solve linear systems


  public:

	//! Default Constructor
	SparseMatrix (const int &nl = 0, const int &nc = 0,const int &fl = 0) {

		nlin = 0;
		ncol = 0;
		nnz = 0;

		// Create "positioner" to the matrix
		it = new Pos_Mat<T>(this);

		strategy = nullptr;

		// Dimensiona strategy
		Dim(nl,nc,fl);

	}

	//! Copy Constructor (content)
	/*! == ATTENTION == It doesn't copy the strategy!!! */
	SparseMatrix (const SparseMatrix<T> &mat) {

		nlin = 0;
		ncol = 0;
		nnz = 0;

		// Create "positioner" to the matrix
		it = new Pos_Mat<T>(this);

		strategy = nullptr;

		// Do assignment
		*this = mat;

	}

	//! Copy Constructor (pointer)
	/* == ATTENTION == It doesn't copy the strategy!!! */
	SparseMatrix (SparseMatrix<T> *mat) {

		nlin = 0;
		ncol = 0;
		nnz = 0;

		// Create "positioner" to the matrix
		it = new Pos_Mat<T>(this);

		strategy = nullptr;

		// Do assignment
		*this = *mat;

	}


	//! Destructor
	~SparseMatrix() 
	{

		// Delete positioner - already exists
		delete it;

		// Delete the strategy
		if (strategy) delete strategy;

	}


	//! Set what strategy will be used to solve linear systems
	void Set_strategy (LS_Strategy<T> *st) {

		// If the matrix already has a strategy, delete it
		if (strategy) delete strategy;

		// Set strategy
		strategy = st;

		// Initialize the strategy
		st->Initialize(*this);

	}


	//! Method to solve linear systems (use the strategy)
	void Solve_LinSys (const std::valarray<T>& b, std::valarray<T>& x) {

		// Test dimension
		if (!nlin || !ncol) {
			std::cout << "Error: Matrix without dimension." << std::endl;
			getchar();
		}

		// Resize x, if necessary
		if (!x.size()) x.resize(b.size());

		// Test strategy
		if (strategy) strategy->Solve(b,x,*this);
		else {
			std::cout << "Error: Matrix without strategy to solve linear systems." << std::endl;
			getchar();
		}

	}


	// Interface

	//! Retorna numero de linhas.
	int  Get_nlin    ()  const  { return nlin; }
	//! Retorna numero de colunas.
	int  Get_ncol    ()  const  { return ncol; }
	//! Retorna numero de elementos nao-nulos.
	int  Get_nnz     ()  const  { return nnz; }
	//! Retorna valor do "flag de simetria" 
	int  Get_flagsym ()  const  { return flagsym; }

	//! Retorna proximo elemento nao-nulo.
	/*!
		TODO: Descricao detalhada do metodo...
	*/
	T  Get_Elem_Next (const int &l, int &c)  const  {

		int ll,cc;
		const std::map<int,T> *ptr_map_line;
		typename std::map<int,T>::const_iterator   map_T_it;


		if (c < ncol) {

			if (l < nlin) {

				if (flagsym && l>c) {  // is the matrix symmetric?
				// it isn't so efficient

					ll = c;
					cc = l;

					while (ll<nlin) {

						ptr_map_line = &(inilin[ll]);   // got line

						map_T_it = ptr_map_line->find(cc);
						if (map_T_it!=ptr_map_line->end()) {
							c = ll;
							return (*map_T_it).second;
						}
						ll++;

					}

				} else {  //if (flagsym) {  // is the matrix symmetric?
				// this is good!!!

					ptr_map_line = &(inilin[l]);   // got line
					for (map_T_it=ptr_map_line->begin();map_T_it!=ptr_map_line->end();map_T_it++) {
						if ((*map_T_it).first >= c ) {
							c = (*map_T_it).first;
							return (*map_T_it).second;
						}
					}

				}  // if (flagsym) {  // is the matrix symmetric?

				c = ncol;
				return (T) 0.0;


			}  // if (l < nlin) {
//			} else std::cout << "ERROR: Line out of range. (" << l << " >= " << nlin << ")" << std::endl;

		}  // if (c < ncol) {
//		} else std::cout << "ERROR: Column out of range. (" << c << " >= " << ncol << ")" << std::endl;

		return (T) 0.0;

	} // Get_Elem_Next


	//! Dimension matrix
	void Dim (const int &nl, const int &nc,const int &fl = 0) {

		// Test dimensions
		if (nl!=nc && flagsym==1) {
			std::cout << "Error: Rectangular matrix can't be symmetric." << std::endl;
			exit(0);
		}

		// If it already has dimensions, reset matrix
		if (nlin) Reset();

		if (nl && nc) {

			// Dimensions
			flagsym = fl;
			nlin = nl;
			ncol = nc;

			// Initialize nnz (number of diagonal elements)
			// Allocate diagonal
			if (nlin<=ncol) {
				nnz = nlin;
				diag.resize(nlin, (T) 0.0 );
			} else {
				nnz = ncol;
				diag.resize(ncol, (T) 0.0 );
			}

			// Allocate rows maps
			inilin.resize(nlin);
		}

	} //Dim


	//! Reset matrix
	void Reset () {

		if (nlin && ncol) {
			//diag.free();
			diag.resize(0);
			inilin.clear();
			inilin.resize(0);
		}

		// Destructor code

		// Delete positioner - already exists
		delete it;

		// Delete the strategy
		if (strategy) delete strategy;


		// Constructor code
		nlin = 0;
		ncol = 0;
		nnz = 0;

		// Create "positioner" to the matrix
		it = new Pos_Mat<T>(this);

		strategy = nullptr;

	}  // Reset


	//! operator =
	// == ATTENTION == It doesn't copy the strategy!!!
	SparseMatrix<T>& operator= (const SparseMatrix<T> &mat) {

		int i,j;
		T aux;
		std::map<int,T> *ptr_map_line_this;
		const std::map<int,T> *ptr_map_line_original;
		typename std::map<int,T>::const_iterator   map_T_it;


		// Reset matrix
		Reset();

		// Dimension matrix
		Dim(mat.nlin,mat.ncol,mat.flagsym);
		nnz = mat.nnz;

		if (nlin && ncol) {

			// Copy diagonal
			if (nlin < ncol) for (i=0;i<nlin;i++) diag[i] = mat.diag[i];
			else for (i=0;i<ncol;i++) diag[i] = mat.diag[i];

			// Lines
			for (i=0;i<nlin;i++) {

				// Get line (original)
				ptr_map_line_original = &((mat.inilin)[i]);

				// Get line (this)
				ptr_map_line_this = &(inilin[i]);

				// Columns
				for (map_T_it=ptr_map_line_original->begin();map_T_it!=ptr_map_line_original->end();map_T_it++) {

					// Set element
					j = (*map_T_it).first;
					aux = (*map_T_it).second;
					ptr_map_line_this->insert( ptr_map_line_this->end() , std::pair<int,T>(j,aux ) );

				}  // for (map_T_it=ptr_map_line_original->begin();map_T_it!=ptr_map_line_original->end();map_T_it++) {
				   // columns

			}  // for (i=0;i<nlin;i++) {  -> lines


		} // if (nlin && ncol) {

		return *this;

	}  // operator =



	//! operator []
	Pos_Mat<T>& operator[] (const int &l) {

		it->itLin = l;

		if (nlin && l<nlin) {

			// Set line
			it->ptr_map_line = &(inilin[l]);

			// Reset data
			// When a search initializes, Pos_Mat must be reseted
			it->itValue_old = it->itValue;
			it->itValue = (T) 0.0;

		}

		return *it;

	}

	// operator ()
	Pos_Mat<T>& operator () (const int &l, const int &c) {
	
		// Reutilize operator []
		return (*this)[l][c];

	}


	//! Overload operator * to (SparseMatrix * std::valarray)
	std::valarray<T> operator * (const std::valarray<T>& vec) {

		int i;
		int sq;
		std::valarray<T> vec_aux;
		const std::map<int,T> *ptr_map_line;
		typename std::map<int,T>::const_iterator   map_T_it;


		// Initialize std::vector
		vec_aux.resize(nlin);
		for (i=0;i<vec_aux.size();i++) vec_aux[i] = (T) 0.0;

		// Test dimensions
		if (ncol != vec.size()) {
			std::cout << "Error: With dimensions in operator SparseMatrix<T> * std::valarray<T>. " << std::endl;
			std::cout << "       Matrix: " << ncol << "  ,  Vector: " << vec.size() << std::endl;
			getchar();
			return vec_aux;
		}

		// Test symmetry
		if (flagsym) {
			std::cout << "Error: In operator SparseMatrix<T> * std::valarray<T>" << std::endl;
			std::cout << "       Matrix can't be symmetric (ATTENTION: waiting implementation)." << std::endl;
			getchar();
			return vec_aux;
		}

		// Get minor dimension
		sq = nlin;
		if (sq>ncol) sq = ncol;

		for (i=0;i<nlin;i++) {

			if (i<sq) vec_aux[i] = diag[i] * vec[i];  // diagonal

			ptr_map_line = &(inilin[i]);   // got line
			for (map_T_it=ptr_map_line->begin();map_T_it!=ptr_map_line->end();map_T_it++) {

				vec_aux[i] = vec_aux[i] + (*map_T_it).second * vec[(*map_T_it).first];

			}

		}  // for (i=0;i<nlin;i++)

		return vec_aux;


	}  // std::valarray<T> operator * (const std::valarray<T> &vec)


	// FT, 08/2006
	//! Overload operator * to (SparseMatrix * SparseMatrix)
	/*!
		Implementa multiplicacao de matrizes esparsas.
	*/
	SparseMatrix<T> operator * (const SparseMatrix<T>& mat) {
		
		int i,j;
		SparseMatrix<T> mat_aux;
		const std::map<int,T> *ptr_map_line;			// std::map pointer
		typename std::map<int,T>::const_iterator   map_T_it;	// iterator
		T val;
		int lin_aux;
		int sq;	
		
		// Initialize new matrix
		mat_aux.Dim(nlin,mat.ncol);
		
		// Test dimensions:
		// verify: Anxm X Bmxo = Rnxo
		if (ncol != mat.nlin) {
			std::cout << "Error: With dimensions in operator SparseMatrix<T> * SparseMatrix<T>. " << std::endl;
			std::cout << "       MatrixA_ncol: " << ncol << "  ,  MatrixB_nlin: " << mat.nlin << std::endl;
			getchar();
			return mat_aux;
					
		} //if (ncol != mat.nlin) 
		
		// Test symmetry
		if (flagsym) {
			std::cout << "Error: In operator SparseMatrix<T> * SparseMatrix<T>" << std::endl;
			std::cout << "       Matrix can't be symmetric (ATTENTION: waiting implementation)." << std::endl;
			getchar();
			return mat_aux;
		} // if (flasym)
		
		// Get minor dimension
		sq = nlin;
		if (sq>ncol) sq = ncol;

		for (i=0;i<nlin;i++) {

			// Diagonal elements:
			if (i<sq) {
				
				// diagonal * diagonal
				mat_aux[i][i] = mat_aux[i][i] + diag[i] * mat.diag[i];  
				
				// diagonal * non-null elements
				j = 0;
				while ( j < mat.ncol ) {
					val = mat.Get_Elem_Next(i,j);
										
					if (j < mat.ncol ) 
						mat_aux[i][j] = mat_aux[i][j] + diag[i] * val;
					
					j++;

				}	
			}
			
			j = 0;
			// non-null elements * diagonal
			while ( j < ncol ) {
				val = Get_Elem_Next(i,j);	
				if (j < ncol)
					mat_aux[i][j] = mat_aux[i][j] + val*mat.diag[j];
				j++;
			} 

			// non-null elements * non-null elements 
			ptr_map_line = &(inilin[i]);   // got line
			for (map_T_it=ptr_map_line->begin(); map_T_it!=ptr_map_line->end(); map_T_it++) {
				
				lin_aux = (*map_T_it).first;
				
				j = 0;
				while ( j < mat.ncol ) {
					val = mat.Get_Elem_Next(lin_aux,j);

					// if Get_Elem_Next() found element
					if ( j < mat.ncol ) {
						mat_aux[i][j] = mat_aux[i][j] + (*map_T_it).second * val;
					}	
					j++;					
				}
			}
		}  // for (i=0;i<nlin;i++)
		

		return mat_aux;
	} // SparseMatrix<T> operator * (const SparseMatrix<T>& mat)

	
	// FT, 08/2006
	//! Overload operator + to (SparseMatrix + SparseMatrix)
	/*! Devido a maneira como foi implementado, este operador
	   fornece resultados com menor esforço computacional se
	   a matriz passada como referencia "mat" for mais esparsa
	   do que a matriz que "chama" o operador, ou seja:
	   No caso de existirem duas matrizes:
	      A: esparsa;
	      B: mais densa que A;
	   A maneira mais eficiente de se realizar a soma eh:
	    C = A + B;
	*/
	SparseMatrix<T> operator + (const SparseMatrix<T>& mat) {
		
		int i,j;
		SparseMatrix<T> mat_aux(nlin,ncol); 
		T val;
				
		// Test dimensions:
		// booth matrixes with the same dimensions
		if ( (ncol != mat.ncol) && (nlin != mat.ncol) ) {
			std::cout << "Error: With dimensions in operator SparseMatrix<T> + SparseMatrix<T>. " << std::endl;
			std::cout << "       Different dimensions " << std::endl;
			getchar();
			return mat_aux;
					
		} //if (ncol != mat.nlin) 
		
		// Test symmetry
		if (flagsym) {
			std::cout << "Error: In operator SparseMatrix<T> + SparseMatrix<T>" << std::endl;
			std::cout << "       Matrix can't be symmetric (ATTENTION: waiting implementation)." << std::endl;
			getchar();
			return mat_aux;
		} // if (flasym)
		
		mat_aux = mat;
		for ( i=0 ; i < Get_nlin() ; i++)  {
			// Diagonal elements
			mat_aux[i][i] = mat_aux[i][i] + diag[i];
						
			j = 0;
			val = Get_Elem_Next (i,j);   
			
			while ( j < Get_ncol() ) {
				// non-diagonal elements
				mat_aux[i][j] = mat_aux[i][j] + val;
				j++;
				val = Get_Elem_Next(i,j);	
			}
		}
		

		return mat_aux;
	} // SparseMatrix<T> operator + (const SparseMatrix<T>& mat)

	// FT, 08/2006
	//! Overload operator ^ to (SparseMatrix ^ int)
	/*! "exp" deve ser um inteiro maior ou igual a zero. */
	SparseMatrix<T> operator ^ (const int& exp) {
		 
		int i;
		SparseMatrix<T> mat_aux(nlin,ncol);
				
		// Test Dimensions:
		// Matrix must be square
		if (ncol != nlin) {
			std::cout << "Error: With dimensions in operator SparseMatrix<T> ^ int. " << std::endl;
			std::cout << "       Matrix must be square." << std::endl;
			getchar();
			return mat_aux;
					
		} //if (ncol != nlin) 
		
		// Test symmetry
		if (flagsym) {
			std::cout << "Error: In operator SparseMatrix<T> ^ int." << std::endl;
			std::cout << "       Matrix can't be symmetric (ATTENTION: waiting implementation)." << std::endl;
			getchar();
			return mat_aux;
		} // if (flasym)


		// Exp must be > 0
		if (exp < 0) {
			std::cout << "Error: In operator SparseMatrix<T> ^ int." << std::endl;
			std::cout << "       Exponent must be greater than zero." << std::endl;
			getchar();
			return mat_aux;
		} else if (exp == 0) {
			// Identity matrix
			for (i=0; i<nlin; i++)
				mat_aux.diag[i] = 1;
			return mat_aux;
		} // if (exp < 0)
		
		mat_aux = *this;
		for ( i=0 ; i < exp-1 ; i++)  {
			mat_aux = mat_aux * (*this);
		}
		
		return mat_aux;
	} // SparseMatrix<T> operator ^ (const int& exp)

	//! Overload operator * to (SparseMatrix * T)
	/*! Multiplicacao de Matriz por vetor. */
	SparseMatrix<T> operator * (const T& val) {

		int i,sq;
		SparseMatrix<T> mat_aux;
		const std::map<int,T> *ptr_map_line;
		typename std::map<int,T>::const_iterator   map_T_it;


		// Initialize matrix
		mat_aux.Dim(nlin,ncol,flagsym);

		// Do!!!
		sq = nlin;
		if (sq>ncol) sq = ncol;

		for (i=0;i<nlin;i++) {

			if (i<sq) mat_aux(i,i) = val * diag[i];  // diagonal

			ptr_map_line = &(inilin[i]);   // got line
			for (map_T_it=ptr_map_line->begin();map_T_it!=ptr_map_line->end();map_T_it++) {

				mat_aux(i,(*map_T_it).first) = val * (*map_T_it).second;

			}

		}  // for (i=0;i<nlin;i++)

		return mat_aux;

	}  // SparseMatrix<T> operator * (const T val)



}; // Class SparseMatrix




// Base class to strategies to solve linear systems
// Abstract class
// Uses Strategy Pattern
template <class T>
class LS_Strategy {

	friend class Pos_Mat<T>;
	friend class SparseMatrix<T>;


  protected:

	int dim;         // system dimension


  public:

	// Constructor
	LS_Strategy () {
		dim = 0;
	}

	// Destructor
	virtual ~LS_Strategy () {};



  protected:

	// Virtual Methods to be inherit by the concrete strategies

	// Initialize strategy
//	virtual void Initialize (const SparseMatrix<T>& SM) = 0;
	virtual void Initialize (SparseMatrix<T>& SM) = 0;

	// Set the values in the strategies
	virtual void Set_Val (const int &i, const int &j, const T &v) = 0;

	// Solve the linear system (complete method)
//	virtual void Solve (const std::valarray<T>& b , std::valarray<T>& x , const SparseMatrix<T>& SM) = 0;
	virtual void Solve (const std::valarray<T>& b , std::valarray<T>& x , SparseMatrix<T>& SM) = 0;


};   // class LS_Strategy



// "In" for Sparse Matrix - square matrix only
template <class T>
std::istream& operator >> (std::istream &stream, SparseMatrix<T> &Mat) {

	char line[81];
	T value;
	int dim;
	int nnz;   // include all elements: diagonal and symmetrics
	int end;
	int l,c;
	int flagsym;
	int i;

	stream >> line;         // "Matriz bidimensional quadrada dimensao: "
	if (line != strstr(line,"Matriz")) {
		std::cout << "Error: Matrix read error." << std::endl;
		getchar();
		exit(0);
	}
	stream >> dim;

	stream >> line;         // "Numero de elementos diferentes de zero: "
	stream >> nnz;

	stream >> line;         // "Eh simetrica: "
	stream >> flagsym;

	// Reset and (re)dimension the matrix
	Mat.Reset();
	Mat.Dim(dim,dim,flagsym);

	stream >> line;         // "Elementos diagonais: (posicao  valor)"
	for (i=0;i<(Mat.Get_nlin());i++) {
		stream >> l;
		stream >> value;
		Mat(l,l) = value;
	}

	if (flagsym) end = (nnz-dim)/2;
	else end = (nnz-dim);

	stream >> line;         // "Elementos nao-diagonais: (linha  coluna  valor)"
	for (i=0;i<end;i++) {
		stream >> l >> c >> value;
		Mat(l,c) = value;
	}

	return stream;

}; // operator >>



// "OUT" for SparseMatrix - square matrix only
template <class T>
std::ostream& operator << (std::ostream &stream, SparseMatrix<T> &Mat) {

	T value;
	int dim;
	int nnz;   // include all elements: diagonal and symmetrics
//	int end;
	int l,c;
	int flagsym;
//	int i;
	int prec;

	// Set precision
	prec = stream.precision();
	stream.precision(15);

	// Test dimensions - square matrix only
	if (Mat.Get_nlin() == Mat.Get_ncol()) {

		// Dimension
		dim = Mat.Get_nlin();  // or ncol - square matrix only

		// Is the matrix symmetric?
		flagsym = Mat.Get_flagsym();

		// Get (count) number of non-zero elements
		nnz = Mat.Get_nnz();

		// Print in the stream
		stream << "Matriz_bidimensional_quadrada_dimensao:" << std::endl;
		stream << dim << std::endl << std::endl;
		stream << "Numero_de_elementos_diferentes_de_zero:" << std::endl;
		stream << nnz << std::endl << std::endl;
		stream << "Eh_simetrica:" << std::endl;
		stream << flagsym << std::endl << std::endl;

		stream << "Elementos_diagonais:_(posicao__valor)" << std::endl;
		for (int i=0;i<dim;i++)
			stream << i << "  " << (T) Mat(i,i) << std::endl;
		stream << std::endl;

		stream << "Elementos_nao-diagonais:_(linha__coluna__valor)" << std::endl;
		for (l=0;l<dim;l++) {
			c = 0;
			while (c < dim) {
				value = Mat.Get_Elem_Next(l,c);
				if (c < dim) stream << l << "  " << c << "  " << value << std::endl;
				c++;
			}
		}

	} else { // if (Mat.Get_nlin() == Mat.Get_ncol) {

		std::cout << "ERROR: Operator << overloaded only for square matrixes." << std::endl;
		getchar();

	}  // if (Mat.Get_nlin() == Mat.Get_ncol) {


	// Return to original precision
	stream.precision(prec);

	return stream;


}  // operator <<



// "OUT" for valarrays
template <class T>
std::ostream& operator << (std::ostream &stream, std::valarray<T> &vec) {

	int prec;

	// Set precision
	prec = stream.precision();
	stream.precision(15);

	stream << "Vector:" << std::endl;
	for (int i=0;i<vec.size();i++) stream << vec[i] << std::endl;

	stream.precision(prec);

	return stream;

}  // std::ostream& operator << (std::ostream &stream, std::valarray<T> &vec) {



// "IN" for valarrays
// Vector must be "dimensioned" before
template <class T>
std::istream& operator >> (std::istream &stream, std::valarray<T> &vec) {

	std::string mline;
#ifdef _PGI
	char mline_PGI[81];
#endif

	// Read lines until find "Vector:"
	do {  // jump comments
#ifdef _PGI
		stream.getline(mline_PGI,80);
		mline = mline_PGI;
#else
		getline(stream,mline);
#endif
	} while ( mline.substr(0,7) != "Vector:" );

	for (int i=0;i<vec.size();i++) {
		stream >> vec[i];
	}

	return stream;

}  // std::istream& operator >> (std::istream &stream, std::valarray<T> &vec) {



#endif   // _SparseMatrix

