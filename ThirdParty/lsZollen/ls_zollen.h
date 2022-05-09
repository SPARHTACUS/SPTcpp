// **************************************************
// *                                                *
// *  Pacote LS_Zollen - Linear System Solver        *
// *                 Method "Bi-factorization"      *
// *                 (Zollenkopf)                   *
// *     04/2001                                    *
// *     jmarco                                     *
// *                                                *
// **************************************************




#ifndef _LS_Zollen
#define _LS_Zollen


// ========== Inclusoes ==========
#ifndef _PGI
#include <cstdlib>
#endif

#include "sparsematrix.h"

// ===============================
template <class T> class LS_Zollen;
template <class T> class ZolMatrix;
template <class T> class ZolSymMatrix;
template <class T> class ZolSymElem;
template <class T> class ZolAsyMatrix;
template <class T> class ZolAsyElem;


// ===============================

// Abstract class for Zollenkopf matrices
template <class T>
class ZolMatrix {

    friend class LS_Zollen<T>;

  //protected:
public:

	int n;						// matrix dimension
	int *nseq;					// column order
	int *noze;					// number of non-zero elements in each column


	// Constructor e destructor
	ZolMatrix (const int &o) {

   		int i;

		n = o;
		if (o) {
			nseq = new int[n];
			noze = new int[n];
			for (i=0;i<n;i++) {
				*(nseq + i) = i;
				*(noze + i) = 0;
			}
		}
		else {
			nseq = nullptr;
			noze = nullptr;
		}
   
    }

	virtual ~ZolMatrix () {

        if (nseq) {
            delete[] nseq;
		    delete[] noze;
        }
    
    };

	// pure virtual methods

    // method to test if exist one element (even after a factorization)
    virtual int Exist_Elem(const int &l, const int &c) = 0;

    // copies one SparseMatrix
//    virtual void Copy(const SparseMatrix<T> &SM) = 0;
    virtual void Copy(SparseMatrix<T> &SM) = 0;

    // Resets the matrix
    virtual void Reset( ) = 0;

	// Zollenkopf order method (= Tinney 2)
    // order and include the fill-in's
	virtual void Order( ) = 0;

    // factors the matrix
    virtual void Factor( ) = 0;

    // solve the backward and forward substitutions
	virtual void Subst (const std::valarray<T> &b, std::valarray<T> &x) = 0;

};   // class ZolMatrix


// Classe LS_Zollen
template <class T>
class LS_Zollen : public LS_Strategy<T> {

  private:

	int flagfact;          // 0 -> matrix hasn't to be factorized
	                      // 1 -> matrix has to be factorized
	int flagord;          // 0 -> matrix hasn't to be ordered 
	                      // 1 -> matrix has to be ordered 
    ZolMatrix<T>  *A;     // coefficient matrix (symmetric or asymmetric)

  public:

	// Constructor
	LS_Zollen ( )  :
	LS_Strategy<T> () {

        flagord = 1;
        flagfact = 1;

    
    }

	// Destructor
	virtual ~LS_Zollen ( ) {

        delete A;

    }

  private:

	// method from the strategy base class LS_Strategy (by inheritance)
//	void Initialize(const SparseMatrix<T> &SM) {
	void Initialize(SparseMatrix<T> &SM) {

		// sets the strategy dimension (not used in this strategy)
        this->dim = SM.Get_nlin();   // igual a SM.ncol

        // creates the symmetric or asymmetric Zollenkopf matrix
        if (SM.Get_flagsym())
            A = new ZolSymMatrix<T>(this->dim);
        else
            A = new ZolAsyMatrix<T>(this->dim);

    } 

	// Sets one value in the matrix
    // method from the strategy base class LS_Strategy (by inheritance)
	void Set_Val (const int &i, const int &j, const T &v) {

        // if the matrix was changed it must be factorized and ordered
        if (!flagfact) flagfact = 1;

        // tests if it is a new element
        if (i != j ) {
            if (!flagord) {
                if (!A->Exist_Elem(i,j)) {
                    A->Reset();
                    flagord = 1;
                }
            }
        }
    }

    // method from the strategy base class LS_Strategy (by inheritance)
//	void Solve (const std::valarray<T> &b ,std::valarray<T> &x , const SparseMatrix<T> &SM) {
	void Solve (const std::valarray<T> &b ,std::valarray<T> &x , SparseMatrix<T> &SM) {

//        clock_t timer = clock();

        // if the matrizx was changed
        if (flagfact)
            A->Copy(SM);

//        timer = clock() - timer;
//std::cout << "Copy time: " << ((float)timer)/CLOCKS_PER_SEC << std::endl;

//timer = clock();

        if (flagord) {
            A->Order();
            flagord = 0;
        }

//timer = clock() - timer;
//std::cout << "Ordering time: " << ((float)timer)/CLOCKS_PER_SEC << std::endl;

//timer = clock();

        if (flagfact) {
            A->Factor();
            flagfact = 0;
        }
//timer = clock() - timer;
//std::cout << "Factorization time: " << ((float)timer)/CLOCKS_PER_SEC << std::endl;

//timer = clock();

        // solve the backward and forward substitutions
		A->Subst(b,x);

//timer = clock() - timer;
//std::cout << "Substitutionn time: " << ((float)timer)/CLOCKS_PER_SEC << std::endl;

	}


}; // Class LS_Zollen


// Class ZolSymElem
template <class T> class ZolSymElem {

	friend class ZolSymMatrix<T>;   // allows direct access from ZolSymMatrix class
  
  private:

	int itag;					// line for the matrix element
	T ce;						// matrix element
	ZolSymElem<T> *lnxt;		// next element in the column
    int flagfill;               // flag that indicates if it is a fill-in element

	// Constructors
	ZolSymElem ( ) {

        flagfill = 0;
    
    }

	// Destructors
	virtual ~ZolSymElem () { }

}; // Class ZolSymElem


// Classe ZolSymMatrix
template <class T> class ZolSymMatrix : public ZolMatrix<T> {

  friend class LS_Zollen<T>;

  private:

	ZolSymElem<T> **lcol;		// std::vector of pointers to the column's first element

	// Constructor
      ZolSymMatrix (const int &o) :
      ZolMatrix<T> (o) {

		int i;

		if (o) {
			lcol = new ZolSymElem<T>*[this->n];
			for (i=0 ; i<this->n ; i++) {
				*(lcol + i) = nullptr;
			}
		}
		else {
			lcol = nullptr;
		}
	}

	// Destructor
	virtual ~ZolSymMatrix() {
		
		int i;
		ZolSymElem<T> *ptr, *prox;

		for (i=0 ; i<this->n ; i++){
			ptr = *(lcol + i);
			while (ptr){
				prox = ptr->lnxt;
				delete ptr;
				ptr = prox;
			}
		}
		if (lcol) delete[] lcol;
	}

    // method to test if exist one element (even after a factorization)
    int Exist_Elem(const int &l, const int &c) {
        
        ZolSymElem<T> *ptr;
        int Found;

        // seeks in column c
        ptr = *(lcol + c);
        Found = 0;
        while (ptr) {
            if (ptr->itag == l) {
                if (!ptr->flagfill)
                    Found = 1;
                break;
            }
            else if (ptr->itag > l){
                break;
            }
            else {
                ptr = ptr->lnxt;
            }
        }
        if (!Found) {         // seeks in column l
            ptr = *(lcol + l);
            while (ptr) {
                if (ptr->itag == c) {
                    if (!ptr->flagfill)
                        Found = 1;
                    break;
                }
                else if (ptr->itag > c){
                    break;
                }
                else {
                    ptr = ptr->lnxt;
                }
            }
        }

        return Found;

    }

    // Substitutes an element in the diagonal
    // Warning: the element must be previously defined
	void Set_Diag (const int &d, const T &e) {

		ZolSymElem<T> *ptr;

		ptr = *(lcol + d);
		while ( ptr ) {  
			if (d == ptr->itag) { // found
				ptr->ce = e;
				break;
			}
			else { // goes to the next element
				ptr = ptr->lnxt;
			}
		} // while

	} // Set_Diag

    // funcao p/ incluir ou substuir um elemento qualquer (inclusive diagonal)
	void Set_Elem (const int &l, const int &c, const T &e) {

		ZolSymElem<T> *ptr, *ptra, *novo;

		ptra = nullptr;

		if ( !(*(lcol + c)) ){  // inserts in a empty list
			novo = new ZolSymElem<T>;
			novo->lnxt = *(lcol + c);
			*(lcol + c) = novo;
			novo->itag = l;
			novo->ce = e;
			(*(this->noze + c))++;
		}
		else if (l < (*(lcol + c))->itag) { // inserts before the first element
			novo = new ZolSymElem<T>;
			novo->lnxt = *(lcol + c);
			*(lcol + c) = novo;
			novo->itag = l;
			novo->ce = e;
			(*(this->noze+c))++;
		}
		else {
			ptr = *(lcol + c);
			while ( 1 ) {  
				if (l == ptr->itag) { // found - substitutes
					ptr->ce = e;
					break;
				}
				else if (l < ptr->itag) { // inserts in the middle
					novo = new ZolSymElem<T>;
					novo->lnxt = ptr;
					ptra->lnxt = novo;
					novo->itag = l;
					novo->ce = e;
					(*(this->noze+c))++;
					break;
				}
				else if ( ptr->lnxt == nullptr) {  //inserts after the end
					novo = new ZolSymElem<T>();
					novo->lnxt = nullptr;
					ptr->lnxt = novo;
					novo->itag = l;
					novo->ce = e;
					(*(this->noze+c))++;
					break;
				}
				else { // goes to the next element
					ptra = ptr;
					ptr = ptr->lnxt;
				}
			} // while
		}

	} // Set_Elem


    // copies one SparseMatrix
//    void Copy(const SparseMatrix<T> &SM) {
    void Copy(SparseMatrix<T> &SM) {
        
        int i, j;
        int Found;
        T val;
		ZolSymElem<T> *ptr, *ptra;

		ptra = nullptr;

        if ( *this->noze ) { //matrix was copied before (ordered)

            for( i=0 ; i < this->n ; i++) {

                // diagonal
                val = (T) SM[i][i];
                Set_Diag(i,val);

                // sets fill-ins equal zero
                ptr = *(lcol + i);
                while (ptr) {
                    if (ptr->flagfill)
                        ptr->ce = 0;
                    ptr = ptr->lnxt;
                }

                // gets the first element above the diagonal
                j = i + 1;
                val = SM.Get_Elem_Next(i,j);

                while ( j < this->n ) {

                    // seeks in column i
                    ptr = *(lcol + i);
                    Found = 0;
                    while (ptr) {
                        if (ptr->itag == j) {
                            ptr->ce = val;
                            Found = 1;
                            break;
                        }
                        else if (ptr->itag > j){
                            break;
                        }
                        else {
                            ptr = ptr->lnxt;
                        }
                    }
                    if (!Found) { // seeks in column j
                        ptr = *(lcol + j);
                        while (ptr) {
                            if (ptr->itag == i) {
                                ptr->ce = val;
                                break;
                            }
                            else {
                                ptr = ptr->lnxt;
                            }
                        }
                    }


                    // goes to the next element of SM
                    j++;
                    val = SM.Get_Elem_Next(i,j);
                }
            }
        }
        else { // matrix never copied before
            
            // copies the diagonal and inferior part
            for( i=0 ; i < this->n ; i++) {

                // diagonal
                val = (T) SM[i][i];
                ptra = new ZolSymElem<T>;
                *(lcol + i) = ptra;
                ptra->ce = val;
                ptra->itag = i;
                ptra->lnxt = nullptr;
                *(this->noze + i) = 1;

                // inferior part ok Zollenkopf matrix

                // gets the first element in th line
                j = i + 1;
                val = SM.Get_Elem_Next(i,j);

                while ( j < this->n ) {

                    // inserts always at the end of column
                    ptr = new ZolSymElem<T>;
                    ptra->lnxt = ptr;
                    ptr->ce = val;
                    ptr->itag = j;
                    ptr->lnxt = nullptr;
                    (*(this->noze + i))++;
                    ptra = ptr;
                    
                    // goes to the next element
                    j++;
                    val = SM.Get_Elem_Next(i,j);
                }
            }

            // copies the inferior to the superior part of Zollenkopf Matrix
            for( i=0 ; i < this->n  ; i++) {

                ptr = (*(lcol + i ))->lnxt;  // at least it has the diagonal element
                while (ptr) {
                    Set_Elem(i,ptr->itag,ptr->ce);
                    ptr = ptr->lnxt;
                }
            }
        }

    } // Copy

    // Resets the matrix
    void Reset( ) {

		int i;
		ZolSymElem<T> *ptr, *prox;

        for (i=0 ; i<this->n ; i++){
			ptr = *(lcol + i);
			while (ptr){
				prox = ptr->lnxt;
				delete ptr;
				ptr = prox;
			}
			*(lcol + i) = nullptr;
			*(this->nseq + i) = i;
			*(this->noze + i) = 0;
		}
    } // Reset


	// Zollenkopf order method (= Tinney 2)
    // order and include the fill-in's
	void Order( ) {

		int i, j, k, min, m, kp, ip;  
		ZolSymElem<T> *l, *lk, *la, *li, *ln;


        for ( j=0 ; j<(this->n-1) ; j++) {

			k =  *(this->nseq + j);
			min = *(this->noze + k);
			m = j;
			for ( i=j+1 ; i<this->n ; ++i ) {
				k = *(this->nseq + i);
				if ( *(this->noze + k)  < min) {
					min = *(this->noze + k);
					m = i;
				}
			}
			kp = ( *(this->nseq + m) );
			( *(this->nseq + m) ) = ( *(this->nseq + j) );
			( *(this->nseq + j) ) = kp;

			lk = *(lcol + kp);
			do {
				k = lk->itag;
				if (k != kp) {  // isn't the diagonal element
					la = nullptr;
					li = *(lcol + kp);
					ip = li->itag;
					l = *(lcol + k);
					i = l->itag;
					do {
						if (i < ip) {
							la = l;
							l = l->lnxt;
							if (l)
								i = l->itag;
							else
								i = this->n+1;
						}
						else if (i == ip) {
							if (i == kp) {
								ln = l->lnxt;
								if (la)
									la->lnxt = ln;
								else
									*(lcol + k) = ln;
								delete l;
								( *(this->noze + k) )--;
								l = ln;
							}
							else {
								la = l;
								l = l->lnxt;
							}
							if (l)
								i = l->itag;
							else
								i = this->n+1;
							li = li->lnxt;
                            if (li)  ip = li->itag;
						}
						else if (i > ip) {
							ln = new ZolSymElem<T>;
							if (la)
								la->lnxt = ln;
							else
								*(lcol + k) = ln;
							ln->lnxt = l;
							ln->itag = ip;
                            ln->ce = 0;
							( *(this->noze + k) )++;
                            ln->flagfill = 1; // fill-in element 
							la = ln;
							li = li->lnxt;
                            if (li)  ip = li->itag;
						}
					} while (li);
				}
				lk = lk->lnxt;
            } while (lk);
		} // for j

	} // order 


    void Factor( ) {

		int i, j, k, kp, ip;
		int lp;
		ZolSymElem<T> *l, *lk, *li;
		T d, cf;
		T *celf;  // this is different from the paper (LF)
        int maior;

        // sets celf
        maior = *(this->noze);
        for (i=1 ; i<this->n ; i++) {
            if ( maior < *(this->noze + i))
                maior = *(this->noze + i);
        }
        celf = new T[ maior ];

		for ( j=0 ; j<this->n ; j++) {
			kp = *(this->nseq + j);
			lk = *(lcol + kp);
			lp = 0;
            do {
				k = lk->itag;
				if (k != kp) { // not in the diagonal
					 *(celf + lp) = lk->ce;
				}
				else {
					d = (T)1.0 / lk->ce;
					lk->ce = d;
				}
				lk = lk->lnxt;
                if (lk) lp++;
			} while (lk);
			lk = *(lcol + kp);
			do {
				k = lk->itag;
				if (k != kp) { // not in the diagonal
					cf = d * (lk->ce);
					lk->ce = - cf;
					lp = 0;
					li = *(lcol + kp);
					ip = li->itag;
					l = *(lcol + k);
					i = l->itag;
					do {
						if (i < ip) {
							l = l->lnxt;
                            if (l)  i = l->itag;
						}
						else if (i > ip) {
							li = li->lnxt;
                            if (li){
                                ip = li->itag;
								lp++;
                            }
						}
						else if (i == ip) {
							l->ce -= cf * ( *(celf + lp) );
							l = l->lnxt;
							if (l) {
                                i = l->itag;
								li = li->lnxt;
                                if (li){
                                    ip = li->itag;
								    lp++;
                                }
							}
						}
					} while ( l && li);
				}
				lk = lk->lnxt;
			} while (lk);
		} // for j

        // deletes celf
        delete[] celf;

    } // Factor


    // solve the backward and forward substitutions
	void Subst (const std::valarray<T> &b, std::valarray<T> &x) {

		int i, j, k;
		ZolSymElem<T> *l;
		T cf, sum;

        x = b;

		for ( j=0 ; j<this->n ; j++ ) {
			k =  *(this->nseq + j) ;
			cf = x[k];
			x[k] = 0;
			l = *(lcol + k);
            do {
				i = l->itag;
				x[i] += (l->ce * cf);
				l = l->lnxt;
            } while (l);
		}
		for ( j=this->n-2 ; j>=0 ; j-- ) {
			k = *(this->nseq + j);
			sum = x[k];
			l = *(lcol + k);
            do {
				i = l->itag;
				if (i != k)
					sum += (l->ce * x[i]);
				l = l->lnxt;
            } while (l);
            x[k] = sum;
		}

	} // subst

}; // Class ZolSymMatrix



// Class ZolAsyElem
template <class T> class ZolAsyElem {

	friend class ZolAsyMatrix<T>;   // // allows direct access from ZolAsyMatrix class
  
  private:

	int itag;					// line for the matrix element
	T ce;						// matrix elemen 
   	T re;						// oposite matrix elemen (related to the main diagonal)
	ZolAsyElem<T> *lnxt;		// next element in the column
    int flagfill;               // flag that indicates if it is a fill-in element

	// Construtores
	ZolAsyElem ( ) {

        flagfill = 0;
    
    }

	// Destrutor
	virtual ~ZolAsyElem () { }

}; // Class ZolSymElem



// Class ZolAsyMatrix
template <class T> class ZolAsyMatrix : public ZolMatrix<T> {

    friend class LS_Zollen<T>;

  private:

      T *de;                    // diagonal elements
      ZolAsyElem<T> **lcol;		// std::vector of pointers to the first elements in each column

	// Constructor
      ZolAsyMatrix (const int &o) :
      ZolMatrix<T> (o) {

        int i;

		if (o) {
            de = new T[this->n];
			lcol = new ZolAsyElem<T>*[this->n];
			for (i=0 ; i<this->n ; i++) {
				*(lcol + i) = nullptr;
			}
		}
		else {
            de = nullptr;
			lcol = nullptr;
		}

      }

	// Destructor
	virtual ~ZolAsyMatrix() {
		
		int i;
		ZolAsyElem<T> *ptr, *prox;

		for (i=0 ; i<this->n ; i++){
			ptr= *(lcol + i);
			while (ptr){
				prox = ptr->lnxt;
				delete ptr;
				ptr=prox;
			}
		}
        if (lcol) {
		    delete[] lcol;
            delete[] de;
        }

    }

    // method to test if exist one element (even after a factorization)
    int Exist_Elem(const int &l, const int &c) {

        ZolAsyElem<T> *ptr;
        int Found;

        // seeks in column c
        ptr = *(lcol + c);
        Found = 0;
        while (ptr) {
            if (ptr->itag == l) {
                if (!ptr->flagfill)
                    Found = 1;
                break;
            }
            else if (ptr->itag > l){
                break;
            }
            else {
                ptr = ptr->lnxt;
            }
        }
        if (!Found) {         // seeks in column l
            ptr = *(lcol + l);
            while (ptr) {
                if (ptr->itag == c) {
                    if (!ptr->flagfill)
                        Found = 1;
                    break;
                }
                else if (ptr->itag > c){
                    break;
                }
                else {
                    ptr = ptr->lnxt;
                }
            }
        }

        return Found;
        
    }

    // inserts or substitutes an element
	void Set_Elem (const int &l, const int &c, const T &cce, const T &rre) {

		ZolAsyElem<T> *ptr, *ptra, *novo;

		ptra = nullptr;

		if ( (l < this->n) && (c < this->n) ) {

			if ( !(*(lcol+c)) ){  // inserts in a empty list
				novo = new ZolAsyElem<T>;
				novo->lnxt = *(lcol + c);
				*(lcol + c) = novo;
				novo->itag = l;
				novo->ce = cce;
                novo->re = rre;
				(*(this->noze + c))++;
			}
			else if (l < (*(lcol+c))->itag) { // inserts before the first element
				novo = new ZolAsyElem<T>;
				novo->lnxt = *(lcol + c);
				*(lcol + c) = novo;
				novo->itag = l;
				novo->ce = cce;
                novo->re = rre;
				(*(this->noze+c))++;
			}
			else {
				ptr = *(lcol + c);
				while ( 1 ) {
					if (l == ptr->itag) { // found - substitutes
						ptr->ce = cce;
                        ptr->re = rre;
						break;
					}
					else if (l < ptr->itag) { // inserts in the middle
						novo = new ZolAsyElem<T>;
						novo->lnxt = ptr;
						ptra->lnxt = novo;
						novo->itag = l;
						novo->ce = cce;
		                novo->re = rre;
        				(*(this->noze+c))++;
						break;
					}
					else if ( ptr->lnxt == nullptr) {  //inserts after the end
						novo = new ZolAsyElem<T>();
						novo->lnxt = nullptr;
						ptr->lnxt = novo;
						novo->itag = l;
						novo->ce = cce;
		                novo->re = rre;
        				(*(this->noze+c))++;
						break;
					}
					else { // goes to the next element
						ptra = ptr;
						ptr = ptr->lnxt;
					}
				} // while
			}
		}

	} // Set_Elem


    // copies one SparseMatrix
//    void Copy(const SparseMatrix<T> &SM) {
    void Copy(SparseMatrix<T> &SM) {
        
        int i, j, k;
        int Found;
        T valsup, valinf;
		ZolAsyElem<T> *ptr, *ptra;

		ptra = nullptr;

        if ( *this->noze ) { //matrix was copied before (ordered)

            for( i=0 ; i < this->n ; i++) {

                // diagonal
                *(de + i) = (T) SM[i][i];

                // sets fill-ins equal zero
                ptr = *(lcol + i);
                while (ptr) {
                    if (ptr->flagfill) {
                        ptr->ce = 0;
                        ptr->re = 0;
                   }

                    ptr = ptr->lnxt;
                }

                // gets the first element above the diagonal                // pega primeiro elemento acima da diagonal da linha em SM
                j = i + 1;
                valsup = SM.Get_Elem_Next(i,j);

                while ( j < this->n ) {

                    // gets the oposite element in the inferior part
                    valinf = SM[j][i];

                    // seeks in column i
                    ptr = *(lcol + i);
                    Found = 0;
                    while (ptr) {
                        if (ptr->itag == j) {
                            ptr->ce = valinf;
                            ptr->re = valsup;
                            Found = 1;
                            break;
                        }
                        else if (ptr->itag > j){
                            break;
                        }
                        else {
                            ptr = ptr->lnxt;
                        }
                    }
                    if (!Found) { // // seeks in column j
                        ptr = *(lcol + j);
                        while (ptr) {
                            if (ptr->itag == i) {
                                ptr->ce = valsup;
                                ptr->re = valinf;
                                break;
                            }
                            else {
                                ptr = ptr->lnxt;
                            }
                        }
                    }


                    // vai ao proximo elemento de SM
                    j++;
                    valsup = SM.Get_Elem_Next(i,j);
                }
            }

            // seeks for elements that exist only in the inferior part of SM
            for(i=1 ; i<this->n ; i++) {
                j=0;
                valinf = SM.Get_Elem_Next(i,j);
                while (j<i) {
                    k=i;
                    valsup = SM.Get_Elem_Next(j,k);
                    if (k>i) {

                        // seek in column i
                        ptr = *(lcol + i);
                        Found = 0;
                        while (ptr) {
                            if (ptr->itag == j) {
                                ptr->ce = 0;
                                ptr->re = valinf;
                                Found = 1;
                                break;
                            }
                            else if (ptr->itag > j){
                                break;
                            }
                            else {
                                ptr = ptr->lnxt;
                            }
                        }
                        if (!Found) { // seeks in column j
                            ptr = *(lcol + j);
                            while (ptr) {
                                if (ptr->itag == i) {
                                    ptr->ce = valinf;
                                    ptr->re = 0;
                                    break;
                                }
                                else {
                                    ptr = ptr->lnxt;
                                }
                            }
                        }
                    }

                    // goes to the next element of SM
                    j++;
                    valinf = SM.Get_Elem_Next(i,j);
                }

            }

        }
        else { // matrix never copied before

            // copies the diagonal and inferior part
            for( i=0 ; i < this->n ; i++) {

                // diagonal
                *(de + i) = (T) SM[i][i];
                *(this->noze + i) = 1;

                // inferior part of Zollenkopf Matrix

                // gets the first line element in SM
                j = i + 1;
                valsup = SM.Get_Elem_Next(i,j);

                if ( j < this->n) { // inserts the first element in column j

                    // gets the oposite element
                    valinf = SM[j][i];

                    ptra = new ZolAsyElem<T>;
                    *(lcol + i) = ptra;
                    ptra->ce = valinf;
                    ptra->re = valsup;
                    ptra->itag = j;
                    ptra->lnxt = nullptr;
                    (*(this->noze + i))++;

                    // goes to the next element
                    j++;
                    valsup = SM.Get_Elem_Next(i,j);

                }

                while ( j < this->n ) {

                    valinf = SM[j][i];

                    // inserts always at the end of column
                    ptr = new ZolAsyElem<T>;
                    ptra->lnxt = ptr;
                    ptr->ce = valinf;
                    ptr->re = valsup;
                    ptr->itag = j;
                    ptr->lnxt = nullptr;
                    (*(this->noze + i))++;
                    ptra = ptr;

                    // goes to the next element
                    j++;
                    valsup = SM.Get_Elem_Next(i,j);
                }
            }

            // seek elements that exist only in the inferior part
            for(i=1 ; i<this->n ; i++) {
                j=0;
                valinf = SM.Get_Elem_Next(i,j);
                while (j<i) {
                    k=i;
                    valsup = SM.Get_Elem_Next(j,k);
                    if (k>i) 
                        Set_Elem(i,j,valinf,0);

                    j++;
                    valinf = SM.Get_Elem_Next(i,j);
                }
            }

            // copies the inferior to the superior part
            for( i=0 ; i <this->n-1 ; i++) {

                ptr = (*(lcol + i ));   // never gets the diagonal elements
                while (ptr) {
                    Set_Elem(i,ptr->itag,ptr->re,ptr->ce);
                    ptr = ptr->lnxt;
                }
            }
        } 

    } // Copy

    // Resets the matrix
    void Reset( ) {

		int i;
		ZolAsyElem<T> *ptr, *prox;

        for (i=0 ; i<this->n ; i++){
			ptr= *(lcol + i);
			while (ptr){
				prox = ptr->lnxt;
				delete ptr;
				ptr = prox;
			}
			*(lcol + i) = nullptr;
			*(this->nseq + i) = i;
			*(this->noze + i) = 0;
		} 
    } // Reset


	// Zollenkopf order method (= Tinney 2)
    // order and include the fill-in's
	void Order( ) {

		int i, j, k, min, m, kp, ip;  
		ZolAsyElem<T> *l, *lk, *la, *li, *ln;


        for ( j=0 ; j<(this->n-1) ; j++) {

			k =  *(this->nseq + j);
			min = *(this->noze + k);
			m = j;
			for ( i=j+1 ; i<this->n ; ++i ) {
				k = *(this->nseq + i);
				if ( *(this->noze + k)  < min) {
					min = *(this->noze + k);
					m = i;
				}
			}
			kp = ( *(this->nseq + m) );
			( *(this->nseq + m) ) = ( *(this->nseq + j) );
			( *(this->nseq + j) ) = kp;

			lk = *(lcol + kp);
			while (lk) {
				k = lk->itag;

                la = nullptr;
				li = *(lcol + kp);
    			ip = li->itag;
				l = *(lcol + k);
				i = l->itag;
                do {
				    if (i < ip) {
                        if (i == kp) {
							ln = l->lnxt;
							if (la)
								la->lnxt = ln;
							else
								*(lcol + k) = ln;
							delete l;
							( *(this->noze + k) )--;
							l = ln;
						}
						else {
							la = l;
							l = l->lnxt;
						}
						if (l)
							i = l->itag;
						else
							if (li) i = this->n + 1;
                    } else if (i == ip) {
						la = l;
						l = l->lnxt;
						if (l)
							i = l->itag;
						else
							i = this->n + 1;
                        li = li->lnxt;
                        if (li)
                			ip = li->itag;
                        else
                            if (l) ip = this->n + 1;
					}
					else if (i > ip) {
                        if (ip != k) {
						    ln = new ZolAsyElem<T>;
						    if (la)
							    la->lnxt = ln;
						    else
							    *(lcol + k) = ln;
						    ln->lnxt = l;
						    ln->itag = ip;
                            ln->ce = 0;
                            ln->re = 0;
						    ( *(this->noze + k) )++;
                            ln->flagfill = 1; // fill-in element 
    					    la = ln;
                        }
					    li = li->lnxt;
                        if (li)
                			ip = li->itag;
                        else
                            if (l) ip = this->n + 1;

					}
				} while (li || l);

                lk = lk->lnxt;

			} //while (lk)
		} // for j

    } // order 


    void Factor( ) {

		int i, j, k, kp, ip;
		ZolAsyElem<T> *l, *lk, *li;
		T d, cf, rf;

		for ( j=0 ; j<this->n ; j++) {
			kp = *(this->nseq + j);
            d = (T)1.0 / (*(de + kp));
            *(de + kp) = d;
			lk = *(lcol + kp);
            if ( lk ) {
                do {
                    lk->re *= d;
				    lk = lk->lnxt;
                } while (lk);

			    lk = *(lcol + kp);
                do {
				    k = lk->itag;
				    cf = lk->re;
                    rf = lk->ce;
				    li = *(lcol + kp);
				    ip = li->itag;
				    l = *(lcol + k);
                    if (l)
        		        i = l->itag;
                    else
                        i = this->n+1;
 
                    do {

					    if (i < ip) {
						    l = l->lnxt;
                            if (l)
        		                i = l->itag;
                            else
                                i = this->n+1;
					    }
                        else if (i > ip) {
                            if (ip == k)
                                *(de + k) -= (cf * li->ce);
        			        li = li->lnxt;
                            if (li) ip = li->itag;
				        }
                        else if (i == ip) {
						    l->ce -= (cf * li->ce);
						    l->re -= (rf * li->re);
						    l = l->lnxt;
                            if (l)
        		                i = l->itag;
                            else
                                i = this->n+1;
						    li = li->lnxt;
                            if (li) ip = li->itag;
					    }
				    } while (li);

				    lk = lk->lnxt;
			    } while (lk);
            } // if (lk)
		} // for j


    } // Factor


    // solve the backward and forward substitutions
	void Subst (const std::valarray<T> &b, std::valarray<T> &x) {

		int i, j, k;
		ZolAsyElem<T> *l;
		T cf, sum;

        x = b;

		for ( j=0 ; j<this->n ; j++ ) {
			k =  *(this->nseq + j) ;
			cf = *(de + k) * x[k];
			x[k] = cf;
			l = *(lcol + k);
			while (l) {
				i = l->itag;
				x[i] -= (l->ce * cf);
				l = l->lnxt;
			}
		}
		for ( j=this->n-2 ; j>=0 ; j-- ) {
			k = *(this->nseq + j);
			sum = x[k];
			l = *(lcol + k);
			while (l) {
				i = l->itag;
				sum -= (l->re * x[i]);
				l = l->lnxt;
			}
            x[k] = sum;
		} 

	} // subst


}; // Class ZolAsyMatrix





#endif // _LS_Zollen
