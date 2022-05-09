#ifndef INTERCAMBIO_H
#define INTERCAMBIO_H

#include "C_SmartElemento.h"

#define ATT_COMUM_INTERCAMBIO(m)  \
	  m(Intercambio,  AttComum,          idIntercambio,  IdIntercambio,         min,          max,           min,      sim) \
	  m(Intercambio,  AttComum,                   nome,         string,         min,          max,        Nenhum,      sim) \
	  m(Intercambio,  AttComum,      submercado_origem,   IdSubmercado,         min,          max,           min,      sim) \
	  m(Intercambio,  AttComum,     submercado_destino,   IdSubmercado,         min,          max,           min,      sim) \
      m(Intercambio,  AttComum,        potencia_minima,         double,           0,          max,             0,      nao) \
      m(Intercambio,  AttComum,        potencia_maxima,         double,           0,          max,             0,      nao) \
      m(Intercambio,  AttComum, penalidade_intercambio,         double,           0,          max,         0.001,      sim) \
      m(Intercambio,  AttComum, penalidade_intercambio_minimo,  double,           0,          max,          5000,      sim)
//         c_classe,   smrtAtt,           nomeAtributo,           tipo,  lowerBound,   upperBound,  initialValue, mustRead?


#define ATT_VETOR_INTERCAMBIO(m)  \
	  m(Intercambio,  AttVetor,   potencia_minima_media,  double,          0,          max,             0,  Periodo) \
	  m(Intercambio,  AttVetor,   potencia_maxima_media,  double,          0,          max,             0,  Periodo) \
	  m(Intercambio,  AttVetor,         potencia_maxima,  double,          0,          max,             0,  Periodo) \
	  m(Intercambio,  AttVetor,         potencia_minima,  double,          0,          max,             0,  Periodo)
//     c_classe,   smrtAtt,          nomeAtributo,    Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador


#define ATT_MATRIZ_INTERCAMBIO(m)  \
	  m(Intercambio,  AttMatriz, percentual_variacao_patamar_carga,     double,          0,           10,             0,  Periodo, IdPatamarCarga) \
	  m(Intercambio,  AttMatriz,                   potencia_maxima,     double,          0,          max,             0,  Periodo, IdPatamarCarga) \
	  m(Intercambio,  AttMatriz,                   potencia_minima,     double,          0,          max,             0,  Periodo, IdPatamarCarga) \
      m(Intercambio,  AttMatriz, fator_perda_centro_gravidade_da_carga, double,          0,            1,             0,    IdMes, IdPatamarCarga)
//          c_classe,    smrtAtt,                     nomeAtributo,       Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador


#define SMART_ELEMENTO_INTERCAMBIO(m) \
	m(Intercambio, AttComum,  ATT_COMUM_INTERCAMBIO) \
	m(Intercambio, AttVetor,  ATT_VETOR_INTERCAMBIO) \
	m(Intercambio, AttMatriz, ATT_MATRIZ_INTERCAMBIO) 

DEFINE_SMART_ELEMENTO(Intercambio, SMART_ELEMENTO_INTERCAMBIO)

class Intercambio : public SmartDados {
public:

	Intercambio();
	Intercambio(const Intercambio &instanciaCopiar);
	void esvaziar();
	virtual ~Intercambio();

	DECLARA_SMART_ELEMENTO(Intercambio, SMART_ELEMENTO_INTERCAMBIO)

		virtual void validacaoDadosAttComum();
};

GET_STRING_FROM_CLASS(Intercambio)

#endif // !defined(C_INTERCAMBIO_H)
