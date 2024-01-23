#ifndef AFLUENCIA_H
#define AFLUENCIA_H

#include "C_SmartElemento.h"


#define ATT_COMUM_AFLUENCIA(m)  \
	  m(Afluencia,  AttComum,                idAfluencia,IdAfluencia,         min,         max,           min,      sim) 
//       c_classe,   smrtAtt,               nomeAtributo,          tipo,  lowerBound,  upperBound,  initialValue, mustRead?


#define ATT_VETOR_AFLUENCIA(m)  \
	  m(Afluencia,  AttVetor,          media_mensal_longo_termo,        int,          0,          max,             0,  IdMes)     \
	  m(Afluencia,  AttVetor,                 natural_historico,     double,        min,          max,             0,  Periodo)   \
	  m(Afluencia,  AttVetor,              r2_natural_historico,     double,        min,          max,             0,  IdHidreletrica)   \
	  m(Afluencia,  AttVetor,          r2_incremental_historico,     double,        min,          max,             0,  IdHidreletrica)   \
	  m(Afluencia,  AttVetor,             incremental_historico,     double,        min,          max,             0,  Periodo) \
	  m(Afluencia,  AttVetor,                 natural_tendencia,     double,        min,          max,             0,  Periodo) \
	  m(Afluencia,  AttVetor,             incremental_tendencia,     double,        min,          max,             0,  Periodo) 
//       c_classe,   smrtAtt,                      nomeAtributo,       Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador


#define ATT_MATRIZ_AFLUENCIA(m)  \
	  m(Afluencia,  AttMatriz,                   natural,     double,       min,          max,             0,  IdCenario, Periodo) \
	  m(Afluencia,  AttMatriz,               incremental,     double,       min,          max,             0,  IdCenario, Periodo) \
	  m(Afluencia,  AttMatriz,              regularizada,     double,       min,          max,             0,  IdCenario, Periodo) 
//       c_classe,   smrtAtt,               nomeAtributo,       Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador


#define SMART_ELEMENTO_AFLUENCIA(m) \
	m(Afluencia, AttComum, ATT_COMUM_AFLUENCIA) \
    m(Afluencia, AttVetor, ATT_VETOR_AFLUENCIA) \
	m(Afluencia, AttMatriz, ATT_MATRIZ_AFLUENCIA) \

DEFINE_SMART_ELEMENTO(Afluencia, SMART_ELEMENTO_AFLUENCIA)


class Afluencia : public SmartDados {

	friend class Dados;
	friend class Hidreletrica;

public:
	 
	Afluencia();
	Afluencia(const Afluencia &instanciaCopiar);
	void esvaziar();
	virtual ~Afluencia();

	DECLARA_SMART_ELEMENTO(Afluencia, SMART_ELEMENTO_AFLUENCIA)

};

GET_STRING_FROM_CLASS(Afluencia)

#endif // AFLUENCIA_H
