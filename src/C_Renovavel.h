#ifndef RENOVAVEL_H
#define RENOVAVEL_H

#include "C_SmartElemento.h"


#define ATT_COMUM_RENOVAVEL(m)  \
	  m(Renovavel,  AttComum,         idRenovavel,  IdRenovavel,         min,         max,           min,      sim) \
	  m(Renovavel,  AttComum,            submercado,   IdSubmercado,         min,         max,           min,      nao) \
	  m(Renovavel,  AttComum,                  nome,         string,         min,         max,        Nenhum,      nao) \
	  m(Renovavel,  AttComum,                 barra,            int,           0,         max,             0,      nao) \
	  m(Renovavel,  AttComum,       constrained_off,           bool,       false,        true,         false,      nao) \
	  m(Renovavel,  AttComum,      fator_capacidade,         double,           0,         max,             0,      nao) \
	  m(Renovavel,  AttComum,        geracao_maxima,         double,           0,         max,             0,      nao) 
//       c_classe,   smrtAtt,           nomeAtributo,           tipo,  lowerBound,  upperBound,  initialValue, mustRead?


#define ATT_MATRIZ_RENOVAVEL(m)  \
	  m(Renovavel,  AttMatriz,       geracao,     double,          0,          max,             0,  Periodo, IdPatamarCarga) 
//         c_classe,   smrtAtt,  nomeAtributo,       Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador


#define SMART_ELEMENTO_RENOVAVEL(m) \
	m(Renovavel, AttComum, ATT_COMUM_RENOVAVEL) \
	m(Renovavel, AttMatriz, ATT_MATRIZ_RENOVAVEL) 

DEFINE_SMART_ELEMENTO(Renovavel, SMART_ELEMENTO_RENOVAVEL)


class Renovavel : public SmartDados {

	friend class Dados;
	friend class Hidreletrica;

public:
	 
	Renovavel();
	Renovavel(const Renovavel &instanciaCopiar);
	void esvaziar();
	virtual ~Renovavel();

	DECLARA_SMART_ELEMENTO(Renovavel, SMART_ELEMENTO_RENOVAVEL)

};

GET_STRING_FROM_CLASS(Renovavel)

#endif // AFLUENCIA_H
