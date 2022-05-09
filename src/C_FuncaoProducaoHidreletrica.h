#ifndef FUNCAO_PRODUCAO_HIDRELETRICA_H
#define FUNCAO_PRODUCAO_HIDRELETRICA_H

#include "C_SmartElemento.h"

#define ATT_COMUM_FUNCAO_PRODUCAO_HIDRELETRICA(m)  \
	  m(FuncaoProducaoHidreletrica,  AttComum, idFuncaoProducaoHidreletrica, IdFuncaoProducaoHidreletrica,        min,         max,           min,      sim)\
      m(FuncaoProducaoHidreletrica,  AttComum,            percentual_volume,                       double,        0.0,         1.0,           0.2,      sim) 
//                        c_classe,   smrtAtt,                 nomeAtributo,                         tipo, lowerBound,  upperBound,  initialValue, mustRead?


#define ATT_VETOR_FUNCAO_PRODUCAO_HIDRELETRICA(m)  \
	  m(FuncaoProducaoHidreletrica,  AttVetor,     percentual_volume_minimo,                       double,        0.0,        1.0,              0,    Periodo) \
	  m(FuncaoProducaoHidreletrica,  AttVetor,     percentual_volume_maximo,                       double,        0.0,        1.0,              0,    Periodo) \
	  m(FuncaoProducaoHidreletrica,  AttVetor,                volume_minimo,                       double,        min,        max,              0,    Periodo) \
	  m(FuncaoProducaoHidreletrica,  AttVetor,                volume_maximo,                       double,        min,        max,              0,    Periodo) \
	  m(FuncaoProducaoHidreletrica,  AttVetor,                          RHS,                       double,        min,        max,              0,    int) \
	  m(FuncaoProducaoHidreletrica,  AttVetor,                           FC,                       double,        0.0,        max,              0,    int) \
	  m(FuncaoProducaoHidreletrica,  AttVetor,                           VH,                       double,        min,        max,              0,    int) \
	  m(FuncaoProducaoHidreletrica,  AttVetor,                           QH,                       double,        min,        max,              0,    int) \
	  m(FuncaoProducaoHidreletrica,  AttVetor,                           SH,                       double,        min,        max,              0,    int) 
//          c_classe,   smrtAtt,                       nomeAtributo,                                  Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador


#define ATT_MATRIZ_FUNCAO_PRODUCAO_HIDRELETRICA(m)  \
	  m(FuncaoProducaoHidreletrica,  AttMatriz,                        RHS,                       double,        min,        max,              0,    Periodo, int) \
	  m(FuncaoProducaoHidreletrica,  AttMatriz,                         FC,                       double,        min,        max,              0,    Periodo, int) \
	  m(FuncaoProducaoHidreletrica,  AttMatriz,                         VH,                       double,        min,        max,              0,    Periodo, int) \
	  m(FuncaoProducaoHidreletrica,  AttMatriz,                         QH,                       double,        min,        max,              0,    Periodo, int) \
	  m(FuncaoProducaoHidreletrica,  AttMatriz,                         SH,                       double,        min,        max,              0,    Periodo, int) 
//          c_classe,    smrtAtt,                             nomeAtributo,                         Tipo, lowerBound,   upperBound,  initialValue, TipoIterador1, TipoIterador2

#define SMART_ELEMENTO_FUNCAO_PRODUCAO_HIDRELETRICA(m) \
	m(FuncaoProducaoHidreletrica,  AttComum, ATT_COMUM_FUNCAO_PRODUCAO_HIDRELETRICA) \
	m(FuncaoProducaoHidreletrica,  AttVetor, ATT_VETOR_FUNCAO_PRODUCAO_HIDRELETRICA) \
	m(FuncaoProducaoHidreletrica, AttMatriz, ATT_MATRIZ_FUNCAO_PRODUCAO_HIDRELETRICA) 

DEFINE_SMART_ELEMENTO(FuncaoProducaoHidreletrica, SMART_ELEMENTO_FUNCAO_PRODUCAO_HIDRELETRICA)

class FuncaoProducaoHidreletrica : public SmartDados {
public:

	FuncaoProducaoHidreletrica();
	FuncaoProducaoHidreletrica(const FuncaoProducaoHidreletrica &instanciaCopiar);
	void esvaziar();
	virtual ~FuncaoProducaoHidreletrica();

	DECLARA_SMART_ELEMENTO(FuncaoProducaoHidreletrica, SMART_ELEMENTO_FUNCAO_PRODUCAO_HIDRELETRICA)

};

GET_STRING_FROM_CLASS(FuncaoProducaoHidreletrica)

#endif // !defined(C_FUNCAO_PRODUCAO_HIDRELETRICA_H)
