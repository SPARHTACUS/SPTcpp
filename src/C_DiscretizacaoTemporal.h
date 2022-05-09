#ifndef DISCRETIZACAO_TEMPORAL_H
#define DISCRETIZACAO_TEMPORAL_H

#include "C_SmartElemento.h"


#define ATT_COMUM_DISCRETIZACAO_TEMPORAL(m)  \
	  m(Discretizacao,  AttComum,        idDiscretizacao,   IdDiscretizacao,         min,         max,           min,      sim) \
	  m(Discretizacao,  AttComum,         inicio_periodo,           Periodo,     01/1900,     01/2100,       01/1900,      nao) \
	  m(Discretizacao,  AttComum,        duracao_periodo,            double,           0,         max,             0,      nao) \
	  m(Discretizacao,  AttComum,                   rede,              bool,       false,        true,         false,      nao) \
	  m(Discretizacao,  AttComum,                patamar,       TipoPatamar,      Nenhum,         max,        Nenhum,      nao) 
//       c_classe,               smrtAtt,           nomeAtributo,              tipo,  lowerBound,  upperBound,  initialValue, mustRead?


#define SMART_ELEMENTO_DISCRETIZACAO_TEMPORAL(m) \
	m(Discretizacao, AttComum, ATT_COMUM_DISCRETIZACAO_TEMPORAL) 

DEFINE_SMART_ELEMENTO(Discretizacao, SMART_ELEMENTO_DISCRETIZACAO_TEMPORAL)


class Discretizacao : public SmartDados {

	friend class Dados;
	friend class Hidreletrica;

public:
	 
	Discretizacao();
	Discretizacao(const Discretizacao &instanciaCopiar);
	void esvaziar();
	virtual ~Discretizacao();

	DECLARA_SMART_ELEMENTO(Discretizacao, SMART_ELEMENTO_DISCRETIZACAO_TEMPORAL)

};

GET_STRING_FROM_CLASS(Discretizacao)

#endif // AFLUENCIA_H
