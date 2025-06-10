#ifndef UNIDADE_UTE
#define UNIDADE_UTE

#include "C_SmartElemento.h"

#define ATT_COMUM_UNIDADE_UTE(m)  \
	  m(UnidadeUTE,  AttComum,                 idUnidadeUTE,         IdUnidadeUTE,         min,         max,           min,      sim) \
	  m(UnidadeUTE,  AttComum,                         nome,               string,         min,         max,           min,      nao) \
	  m(UnidadeUTE,  AttComum,                   submercado,         IdSubmercado,         min,         max,           min,     nao) \
	  m(UnidadeUTE,  AttComum,              potencia_minima,               double,           0,         max,             0,     nao) \
	  m(UnidadeUTE,  AttComum,              potencia_maxima,               double,           0,         max,             0,     nao) \
	  m(UnidadeUTE,  AttComum,              disponibilidade,               double,           0,           1,             1,     nao) \
      m(UnidadeUTE,  AttComum,            custo_de_operacao,               double,           0,        2000,             0,     nao) \
      m(UnidadeUTE,  AttComum,                      ramp_up,               double,           0,         max,           max,     nao) \
      m(UnidadeUTE,  AttComum,                    ramp_down,               double,           0,         max,           max,     nao) \
      m(UnidadeUTE,  AttComum,                  min_time_up,               double,           0,       10000,             0,     nao) \
      m(UnidadeUTE,  AttComum,                min_time_down,               double,           0,       10000,             0,     nao) \
      m(UnidadeUTE,  AttComum,                     time_ini,               double,         min,         max,           min,     nao) \
      m(UnidadeUTE,  AttComum,                    power_ini,               double,           0,       10000,             0,     nao)
//     c_classe,    smrtAtt,                   nomeAtributo,           tipo,  lowerBound,  upperBound,  initialValue, mustRead?


#define ATT_VETOR_UNIDADE_UTE(m)  \
	  m(UnidadeUTE,  AttVetor,     disponibilidade,            double,          0,            1,       0,      Periodo) \
      m(UnidadeUTE,  AttVetor,       ramp_shutdown,            double,          0,          max,       0,          int) \
      m(UnidadeUTE,  AttVetor,         ramp_turnon,            double,          0,          max,       0,          int) \
      m(UnidadeUTE,  AttVetor,  representacao_discreta_producao,  int,          0,            1,       0,      Periodo)

#define ATT_MATRIZ_UNIDADE_UTE(m)  \
	  m(UnidadeUTE,  AttMatriz,          custo_de_operacao,   double,         0,          max,             0,  Periodo, IdPatamarCarga) \
	  m(UnidadeUTE,  AttMatriz,              potencia_util,   double,         0,          max,             0,  Periodo, IdPatamarCarga) \
	  m(UnidadeUTE,  AttMatriz,            potencia_minima,   double,         0,          max,             0,  Periodo, IdPatamarCarga) \
	  m(UnidadeUTE,  AttMatriz,            potencia_maxima,   double,         0,          max,             0,  Periodo, IdPatamarCarga) \
	  m(UnidadeUTE,  AttMatriz, potencia_disponivel_minima,   double,         0,          max,             0,  Periodo, IdPatamarCarga) \
	  m(UnidadeUTE,  AttMatriz, potencia_disponivel_maxima,   double,         0,          max,             0,  Periodo, IdPatamarCarga) 

//     c_classe,      smrtAtt,     nomeAtributo,    Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador



#define SMART_ELEMENTO_UNIDADE_UTE(m)\
	m(UnidadeUTE,   AttComum,   ATT_COMUM_UNIDADE_UTE)  \
	m(UnidadeUTE,   AttVetor,   ATT_VETOR_UNIDADE_UTE)  \
	m(UnidadeUTE,   AttMatriz,  ATT_MATRIZ_UNIDADE_UTE) \

 
DEFINE_SMART_ELEMENTO(UnidadeUTE, SMART_ELEMENTO_UNIDADE_UTE)

class UnidadeUTE : public SmartDados {
public:

	UnidadeUTE();
	UnidadeUTE(const UnidadeUTE &instanciaCopiar);
	void esvaziar();
	virtual ~UnidadeUTE();

	DECLARA_SMART_ELEMENTO(UnidadeUTE, SMART_ELEMENTO_UNIDADE_UTE)

};

GET_STRING_FROM_CLASS(UnidadeUTE)

#endif // !defined(C_UNIDADE_UTE)
