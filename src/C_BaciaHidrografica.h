#ifndef BACIA_HIDROGRAFICA_H
#define BACIA_HIDROGRAFICA_H

#include "C_SmartElemento.h"


#define ATT_COMUM_BACIA_HIDROGRAFICA(m)  \
	  m(BaciaHidrografica,  AttComum,       idBaciaHidrografica, IdBaciaHidrografica,         min,          max,           min,      sim) \
	  m(BaciaHidrografica,  AttComum,                      nome,              string,         min,          max,        Nenhum,      sim)  
//     c_classe,             smrtAtt,              nomeAtributo,                tipo,  lowerBound,   upperBound,  initialValue, mustRead?



#define ATT_MATRIZ_BACIA_HIDROGRAFICA(m)  \
	  m(BaciaHidrografica,  AttMatriz,   produtibilidade_media_acumulada,               double,            0,            10,             0,  IdHidreletrica,        Periodo) \
	  m(BaciaHidrografica,  AttMatriz,   produtibilidade_equiv_acumulada,               double,            0,            10,             0,  IdHidreletrica,        Periodo) 
//               c_classe,    smrtAtt,                      nomeAtributo,     Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador


#define SMART_ELEMENTO_BACIA_HIDROGRAFICA(m) \
	m(BaciaHidrografica, AttComum,   ATT_COMUM_BACIA_HIDROGRAFICA)  \
	m(BaciaHidrografica, AttMatriz,  ATT_MATRIZ_BACIA_HIDROGRAFICA)  


DEFINE_SMART_ELEMENTO(BaciaHidrografica, SMART_ELEMENTO_BACIA_HIDROGRAFICA)

class BaciaHidrografica : public SmartDados {
public:

	BaciaHidrografica();
	BaciaHidrografica(const BaciaHidrografica &instanciaCopiar);
	void esvaziar();
	virtual ~BaciaHidrografica();

	DECLARA_SMART_ELEMENTO(BaciaHidrografica, SMART_ELEMENTO_BACIA_HIDROGRAFICA)

};

GET_STRING_FROM_CLASS(BaciaHidrografica)

#endif // !defined(C_BACIA_HIDROGRAFICA_H)
