#ifndef AGRUPAMENTO_INTERCAMBIO_H
#define AGRUPAMENTO_INTERCAMBIO_H

#include "C_SmartElemento.h"

#define ATT_COMUM_AGRUPAMENTO_INTERCAMBIO(m)  \
	  m(AgrupamentoIntercambio,  AttComum, idAgrupamentoIntercambio, IdAgrupamentoIntercambio,         min,          max,           min,      sim) \
	  m(AgrupamentoIntercambio,  AttComum,                     nome,                   string,         min,          max,        Nenhum,      sim) \
      m(AgrupamentoIntercambio,  AttComum,          potencia_minima,                   double,           0,          max,             0,      nao) \
      m(AgrupamentoIntercambio,  AttComum,          potencia_maxima,                   double,           0,          max,             0,      nao) 
//                    c_classe,   smrtAtt,             nomeAtributo,                     tipo,  lowerBound,   upperBound,  initialValue, mustRead?


#define ATT_VETOR_AGRUPAMENTO_INTERCAMBIO(m)  \
	  m(AgrupamentoIntercambio,  AttVetor,       intercambio,       IdIntercambio,        min,          max,           min,      int) \
	  m(AgrupamentoIntercambio,  AttVetor,      fator_ponderacao,          double,          0,            1,             0,      int) \
	  m(AgrupamentoIntercambio,  AttVetor,       potencia_minima,          double,          0,          max,             0,  Periodo) \
	  m(AgrupamentoIntercambio,  AttVetor,       potencia_maxima,          double,          0,          max,             0,  Periodo) 
//                    c_classe,   smrtAtt,          nomeAtributo,            Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador


#define ATT_MATRIZ_AGRUPAMENTO_INTERCAMBIO(m)  \
	  m(AgrupamentoIntercambio,  AttMatriz, percentual_variacao_patamar_carga,     double,          0,           10,             0,  Periodo, IdPatamarCarga) \
	  m(AgrupamentoIntercambio,  AttMatriz,                   potencia_minima,     double,          0,          max,             0,  Periodo, IdPatamarCarga) \
	  m(AgrupamentoIntercambio,  AttMatriz,                   potencia_maxima,     double,          0,          max,             0,  Periodo, IdPatamarCarga) 
//                    c_classe,    smrtAtt,                      nomeAtributo,       Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador


#define SMART_ELEMENTO_AGRUPAMENTO_INTERCAMBIO(m) \
	m(AgrupamentoIntercambio,  AttComum, ATT_COMUM_AGRUPAMENTO_INTERCAMBIO) \
	m(AgrupamentoIntercambio,  AttVetor, ATT_VETOR_AGRUPAMENTO_INTERCAMBIO) \
	m(AgrupamentoIntercambio, AttMatriz, ATT_MATRIZ_AGRUPAMENTO_INTERCAMBIO) 

DEFINE_SMART_ELEMENTO(AgrupamentoIntercambio, SMART_ELEMENTO_AGRUPAMENTO_INTERCAMBIO)

class AgrupamentoIntercambio : public SmartDados {
public:

	AgrupamentoIntercambio();
	AgrupamentoIntercambio(const AgrupamentoIntercambio &instanciaCopiar);
	void esvaziar();
	virtual ~AgrupamentoIntercambio();

	DECLARA_SMART_ELEMENTO(AgrupamentoIntercambio, SMART_ELEMENTO_AGRUPAMENTO_INTERCAMBIO)

};

GET_STRING_FROM_CLASS(AgrupamentoIntercambio)

#endif // !defined(C_AGRUPAMENTO_INTERCAMBIO_H)
