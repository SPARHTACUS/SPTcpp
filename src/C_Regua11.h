#ifndef REGUA_11_H
#define REGUA_11_H

#include "C_SmartElemento.h"

#define ATT_COMUM_REGUA_11(m)  \
	  m(Regua11,  AttComum,        idRegua11,     IdRegua11,         min,         max,           min,      sim) \
	  m(Regua11,  AttComum,    coeficiente_0,        double,         min,       10000,             0,      nao) \
	  m(Regua11,  AttComum,    coeficiente_1,        double,         min,       10000,             0,      nao) \
	  m(Regua11,  AttComum,    coeficiente_2,        double,         min,       10000,             0,      nao) \
	  m(Regua11,  AttComum,    coeficiente_3,        double,         min,       10000,             0,      nao) \
	  m(Regua11,  AttComum,    coeficiente_4,        double,         min,       10000,             0,      nao) \
	  m(Regua11,  AttComum,    coeficiente_5,        double,         min,       10000,             0,      nao) \
	  m(Regua11,  AttComum,    coeficiente_6,        double,         min,       10000,             0,      nao) 
//     c_classe,   smrtAtt,     nomeAtributo,          tipo,  lowerBound,  upperBound,  initialValue, mustRead?


#define ATT_VETOR_REGUA_11(m)  \
	  m(Regua11,  AttVetor,          nivel_dia_anterior,           double,           0,         max,             0,  Periodo)\
	  m(Regua11,  AttVetor,                hidreletrica,   IdHidreletrica,         min,         max,           min,      int)\
	  m(Regua11,  AttVetor,     variacao_maxima_horaria,           double,           0,         max,             0,  Periodo)\
	  m(Regua11,  AttVetor,      variacao_maxima_diaria,           double,           0,         max,             0,  Periodo)
//     c_classe,   smrtAtt,                nomeAtributo,             Tipo,  lowerBound,  upperBound,  initialValue,  TipoIterador


#define SMART_ELEMENTO_REGUA_11(m) \
	  m(Regua11, AttComum, ATT_COMUM_REGUA_11)\
	  m(Regua11, AttVetor, ATT_VETOR_REGUA_11) \

DEFINE_SMART_ELEMENTO(Regua11, SMART_ELEMENTO_REGUA_11)

class Regua11 : public SmartDados {
public:

	Regua11();
	Regua11(const Regua11 &instanciaCopiar);
	void esvaziar();
	virtual ~Regua11();

	DECLARA_SMART_ELEMENTO(Regua11, SMART_ELEMENTO_REGUA_11)

};

GET_STRING_FROM_CLASS(Regua11)

#endif // !defined(C_REGUA11)
