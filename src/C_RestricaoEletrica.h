#ifndef RESTRICAO_ELETRICA_H
#define RESTRICAO_ELETRICA_H

#include "C_ElementoSistema.h"
#include "C_SmartElemento.h"

#define ATT_COMUM_RESTRICAO_ELETRICA(m)  \
	  m(RestricaoEletrica,  AttComum, idRestricaoEletrica, IdRestricaoEletrica,         min,          max,           min,      sim) \
	  m(RestricaoEletrica,  AttComum,                nome,              string,         min,          max,        Nenhum,      sim) \
	  m(RestricaoEletrica,  AttComum,     potencia_minima,              double,           0,          max,             0,      nao) \
	  m(RestricaoEletrica,  AttComum,     potencia_maxima,              double,           0,          max,             0,      nao) \
	  m(RestricaoEletrica,  AttComum, penalidade_restricao_eletrica,    double,           0,          max,          5000,      sim) 
//               c_classe,   smrtAtt,        nomeAtributo,                tipo,  lowerBound,   upperBound,  initialValue, mustRead?


#define ATT_VETOR_RESTRICAO_ELETRICA(m)  \
	  m(RestricaoEletrica,  AttVetor,  potencia_minima,             double,        min,          max,             0,      Periodo) \
	  m(RestricaoEletrica,  AttVetor,  potencia_maxima,             double,          0,          max,           max,      Periodo) \
	  m(RestricaoEletrica,  AttVetor,  potencia_minima_media,       double,          0,          max,           max,      Periodo) \
	  m(RestricaoEletrica,  AttVetor,  potencia_maxima_media,       double,          0,          max,           max,      Periodo) 
//               c_classe,   smrtAtt,     nomeAtributo,               Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador


#define ATT_MATRIZ_RESTRICAO_ELETRICA(m)  \
	  m(RestricaoEletrica,  AttMatriz, percentual_variacao_patamar_carga,     double,          0,           10,             0,  Periodo, IdPatamarCarga) \
	  m(RestricaoEletrica,  AttMatriz,                   potencia_minima,     double,        min,          max,             0,  Periodo, IdPatamarCarga) \
      m(RestricaoEletrica,  AttMatriz,                   potencia_maxima,     double,          0,          max,             0,  Periodo, IdPatamarCarga) \
//               c_classe,    smrtAtt,                      nomeAtributo,       Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador

#define MEMBRO_RESTRICAO_ELETRICA(m)          \
	m(RestricaoEletrica, ElementoSistema)      

#define SMART_ELEMENTO_RESTRICAO_ELETRICA(m) \
	m(RestricaoEletrica,  AttComum, ATT_COMUM_RESTRICAO_ELETRICA) \
	m(RestricaoEletrica,  AttVetor, ATT_VETOR_RESTRICAO_ELETRICA) \
	m(RestricaoEletrica, AttMatriz, ATT_MATRIZ_RESTRICAO_ELETRICA) \
	m(RestricaoEletrica, Membro, MEMBRO_RESTRICAO_ELETRICA) 

DEFINE_SMART_ELEMENTO(RestricaoEletrica, SMART_ELEMENTO_RESTRICAO_ELETRICA)

class RestricaoEletrica : public SmartDados {
public:

	RestricaoEletrica();
	RestricaoEletrica(const RestricaoEletrica &instanciaCopiar);
	RestricaoEletrica& operator=(const RestricaoEletrica& instanciaCopiar);
	void esvaziar();
	virtual ~RestricaoEletrica();

	DECLARA_SMART_ELEMENTO(RestricaoEletrica, SMART_ELEMENTO_RESTRICAO_ELETRICA)

};

GET_STRING_FROM_CLASS(RestricaoEletrica)

#endif // !defined(C_RESTRICAO_ELETRICA_H)
