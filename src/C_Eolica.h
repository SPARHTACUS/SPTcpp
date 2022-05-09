#ifndef USINA_EOLICA_H
#define USINA_EOLICA_H

#include "C_SmartElemento.h"


#define ATT_COMUM_USINA_EOLICA(m)  \
	  m(UsinaEolica,  AttComum,         idUsinaEolica,  IdUsinaEolica,         min,         max,           min,      sim) \
	  m(UsinaEolica,  AttComum,            submercado,   IdSubmercado,         min,         max,           min,      nao) \
	  m(UsinaEolica,  AttComum,                  nome,         string,         min,         max,        Nenhum,      nao) \
	  m(UsinaEolica,  AttComum,                 barra,            int,           0,         max,             0,      nao) \
	  m(UsinaEolica,  AttComum,       constrained_off,           bool,       false,        true,         false,      nao) \
	  m(UsinaEolica,  AttComum,      fator_capacidade,         double,           0,         max,             0,      nao) \
	  m(UsinaEolica,  AttComum,        geracao_maxima,         double,           0,         max,             0,      nao) 
//       c_classe,   smrtAtt,           nomeAtributo,           tipo,  lowerBound,  upperBound,  initialValue, mustRead?


#define ATT_MATRIZ_USINA_EOLICA(m)  \
	  m(UsinaEolica,  AttMatriz,       geracao,     double,          0,          max,             0,  Periodo, IdPatamarCarga) 
//         c_classe,   smrtAtt,  nomeAtributo,       Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador


#define SMART_ELEMENTO_USINA_EOLICA(m) \
	m(UsinaEolica, AttComum, ATT_COMUM_USINA_EOLICA) \
	m(UsinaEolica, AttMatriz, ATT_MATRIZ_USINA_EOLICA) \

DEFINE_SMART_ELEMENTO(UsinaEolica, SMART_ELEMENTO_USINA_EOLICA)


class UsinaEolica : public SmartDados {

	friend class Dados;
	friend class Hidreletrica;

public:
	 
	UsinaEolica();
	UsinaEolica(const UsinaEolica &instanciaCopiar);
	void esvaziar();
	virtual ~UsinaEolica();

	DECLARA_SMART_ELEMENTO(UsinaEolica, SMART_ELEMENTO_USINA_EOLICA)

};

GET_STRING_FROM_CLASS(UsinaEolica)

#endif // AFLUENCIA_H
