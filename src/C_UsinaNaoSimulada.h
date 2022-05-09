#ifndef USINA_NAO_SIMULADA_H
#define USINA_NAO_SIMULADA_H

#include "C_SmartElemento.h"

#define ATT_COMUM_USINA_NAO_SIMULADA(m)  \
	  m(UsinaNaoSimulada,  AttComum,   idUsinaNaoSimulada,  IdUsinaNaoSimulada,         min,          max,           min,      sim) \
      m(UsinaNaoSimulada,  AttComum,                 nome,              string,         min,          max,           min,      nao) \
	  m(UsinaNaoSimulada,  AttComum,      potencia_minima,              double,           0,      1000000,             0,      nao) \
	  m(UsinaNaoSimulada,  AttComum,      potencia_maxima,              double,           0,      1000000,             0,      nao) 

//              c_classe,   smrtAtt,        nomeAtributo,                tipo,  lowerBound,   upperBound,  initialValue, mustRead?


#define ATT_VETOR_USINA_NAO_SIMULADA(m)  \
	  m(UsinaNaoSimulada,  AttVetor,        potencia_minima,   double,           0,     1000000,             0,        Periodo) \
	  m(UsinaNaoSimulada,  AttVetor,        potencia_maxima,   double,           0,     1000000,             0,        Periodo) \
	  m(UsinaNaoSimulada,  AttVetor,  potencia_minima_media,   double,           0,     1000000,             0,        Periodo) \
	  m(UsinaNaoSimulada,  AttVetor,  potencia_maxima_media,   double,           0,     1000000,             0,        Periodo) 
//              c_classe,   smrtAtt,            nomeAtributo,    Tipo,  lowerBound,  upperBound,  initialValue,    TipoIterador


#define ATT_MATRIZ_USINA_NAO_SIMULADA(m)  \
	  m(UsinaNaoSimulada,  AttMatriz,                     potencia_minima,         double,            0,      1000000,             0,  Periodo,        IdPatamarCarga) \
	  m(UsinaNaoSimulada,  AttMatriz,                     potencia_maxima,         double,            0,      1000000,             0,  Periodo,        IdPatamarCarga) \
      m(UsinaNaoSimulada,  AttMatriz,   percentual_variacao_patamar_carga,         double,            0,           10,             0,  Periodo,        IdPatamarCarga) 

#define SMART_ELEMENTO_USINA_NAO_SIMULADA(m) \
	m(UsinaNaoSimulada, AttComum, ATT_COMUM_USINA_NAO_SIMULADA) \
	m(UsinaNaoSimulada, AttVetor, ATT_VETOR_USINA_NAO_SIMULADA) \
    m(UsinaNaoSimulada, AttMatriz, ATT_MATRIZ_USINA_NAO_SIMULADA) 

DEFINE_SMART_ELEMENTO(UsinaNaoSimulada, SMART_ELEMENTO_USINA_NAO_SIMULADA)

class UsinaNaoSimulada : public SmartDados {
public:

	UsinaNaoSimulada();
	UsinaNaoSimulada(const UsinaNaoSimulada &instanciaCopiar);
	void esvaziar();
	virtual ~UsinaNaoSimulada();

	DECLARA_SMART_ELEMENTO(UsinaNaoSimulada, SMART_ELEMENTO_USINA_NAO_SIMULADA)

};

GET_STRING_FROM_CLASS(UsinaNaoSimulada)

#endif // !defined(C_USINA_NAO_SIMULADA_H)
