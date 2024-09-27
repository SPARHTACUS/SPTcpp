#ifndef CONTRATO_H
#define CONTRATO_H

#include "C_SmartElemento.h"


#define ATT_COMUM_CONTRATO(m)  \
	  m(Contrato,  AttComum,                       idContrato,                    IdContrato,         min,         max,           min,      CPPD) \
	  m(Contrato,  AttComum,                    tipo_contrato,                  TipoContrato,         min,         max,           min,      CPPD) \
	  m(Contrato,  AttComum,                             nome,                        string,         min,         max,           min,      CPPD) \
	  m(Contrato,  AttComum,                       submercado,                  IdSubmercado,         min,         max,           min,      CPPD) \
	  m(Contrato,  AttComum,                   tipo_restricao,         TipoRestricaoContrato,      Nenhum,         max,        Nenhum,      CPPD) \
	  m(Contrato,  AttComum,                     tipo_unidade,  TipoUnidadeRestricaoContrato,      Nenhum,         max,        Nenhum,      CPPD) \
	  m(Contrato,  AttComum,               titulacao_contrato,                           int,          -1,           4,            -1,      CPPD) \
	  m(Contrato,  AttComum,                          vlr_ini,                        double,           0,         max,             0,      CPPD) 
//       c_classe,   smrtAtt,                    nomeAtributo,                         tipo,  lowerBound,  upperBound,  initialValue, mustRead?


#define ATT_MATRIZ_CONTRATO(m)  \
	  m(Contrato,  AttMatriz,                      var_abs_inf,               double,            0,           max,             0,  Periodo,        IdPatamarCarga) \
	  m(Contrato,  AttMatriz,                      var_abs_sup,               double,            0,           max,             0,  Periodo,        IdPatamarCarga) \
	  m(Contrato,  AttMatriz,                      var_rel_inf,               double,            0,           max,             0,  Periodo,        IdPatamarCarga) \
	  m(Contrato,  AttMatriz,                      var_rel_sup,               double,            0,           max,             0,  Periodo,        IdPatamarCarga) \
	  m(Contrato,  AttMatriz,                          lim_inf,               double,            0,           max,             0,  Periodo,        IdPatamarCarga) \
	  m(Contrato,  AttMatriz,                          lim_sup,               double,            0,           max,             0,  Periodo,        IdPatamarCarga) \
	  m(Contrato,  AttMatriz,                            custo,               double,            0,           max,             0,  Periodo,        IdPatamarCarga)
//        c_classe,    smrtAtt,                      nomeAtributo,     Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador

#define SMART_ELEMENTO_CONTRATO(m) \
	m(Contrato, AttComum,   ATT_COMUM_CONTRATO) \
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
