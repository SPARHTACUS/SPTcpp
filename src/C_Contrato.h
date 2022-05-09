#ifndef CONTRATO_H
#define CONTRATO_H

#include "C_SmartElemento.h"


#define ATT_COMUM_CONTRATO(m)  \
	  m(Contrato,  AttComum,                       idContrato,                    IdContrato,     Nenhum,         max,        Nenhum,       CPPD) \
	  m(Contrato,  AttComum,                    tipo_contrato,                  TipoContrato,     Nenhum,         max,        Nenhum,       CPPD) \
	  m(Contrato,  AttComum,                             nome,                        string,         min,         max,           min,      CPPD) \
	  m(Contrato,  AttComum,                       submercado,                  IdSubmercado,      Nenhum,         max,        Nenhum,      CPPD) \
	  m(Contrato,  AttComum,                   tipo_restricao,         TipoRestricaoContrato,      Nenhum,         max,        Nenhum,      CPPD) \
	  m(Contrato,  AttComum,                     tipo_unidade,  TipoUnidadeRestricaoContrato,      Nenhum,         max,        Nenhum,      CPPD) \
	  m(Contrato,  AttComum, energia_imp_exp_periodo_anterior,                        double,           0,         max,             0,      CPPD) 
//       c_classe,   smrtAtt,                    nomeAtributo,                         tipo,  lowerBound,  upperBound,  initialValue, mustRead?


#define ATT_VETOR_CONTRATO(m)  \
	  m(Contrato,  AttVetor,  variacao_acrescimo_maxima,     double,          0,          max,             0,  Periodo) \
	  m(Contrato,  AttVetor, variacao_decrescimo_maxima,     double,          0,          max,             0,  Periodo) 
//       c_classe,   smrtAtt,               nomeAtributo,       Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador

#define ATT_MATRIZ_CONTRATO(m)  \
	  m(Contrato,  AttMatriz,                   energia_minima,               double,            0,           max,             0,  Periodo,        IdPatamarCarga) \
	  m(Contrato,  AttMatriz,                   energia_maxima,               double,            0,           max,             0,  Periodo,        IdPatamarCarga) \
	  m(Contrato,  AttMatriz,            preco_energia_imp_exp,               double,            0,           max,             0,  Periodo,        IdPatamarCarga)
//        c_classe,    smrtAtt,                      nomeAtributo,     Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador

#define SMART_ELEMENTO_CONTRATO(m) \
	m(Contrato, AttComum,   ATT_COMUM_CONTRATO) \
	m(Contrato, AttVetor,   ATT_VETOR_CONTRATO) \
    m(Contrato, AttMatriz,  ATT_MATRIZ_CONTRATO)

DEFINE_SMART_ELEMENTO(Contrato, SMART_ELEMENTO_CONTRATO)


class Contrato : public SmartDados {

	friend class Dados;
	friend class Hidreletrica;

public:
	 
	Contrato();
	Contrato(const Contrato &instanciaCopiar);
	void esvaziar();
	virtual ~Contrato();

	DECLARA_SMART_ELEMENTO(Contrato, SMART_ELEMENTO_CONTRATO)

};

GET_STRING_FROM_CLASS(Contrato)

#endif // AFLUENCIA_H
