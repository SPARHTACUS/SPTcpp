#ifndef UNIDADE_UHE
#define UNIDADE_UHE

#include "C_FuncaoProducaoHidreletrica.h"

#include "C_SmartElemento.h"

#define ATT_COMUM_UNIDADE_UHE(m)  \
	  m(UnidadeUHE,  AttComum,             idUnidadeUHE,        IdUnidadeUHE,         min,         max,           min,      sim) \
	  m(UnidadeUHE,  AttComum,               submercado,        IdSubmercado,         min,         max,           min,      sim) \
	  m(UnidadeUHE,  AttComum, tipo_de_perda_hidraulica, TipoPerdaHidraulica,         min,         max,           min,      nao) \
	  m(UnidadeUHE,  AttComum,         perda_hidraulica,              double,           0,         100,             0,      nao) \
	  m(UnidadeUHE,  AttComum,        fator_de_producao,              double,           0,        0.01,             0,      nao) \
	  m(UnidadeUHE,  AttComum,          disponibilidade,              double,           0,           1,             1,      nao) \
	  m(UnidadeUHE,  AttComum,   vazao_turbinada_minima,              double,           0,         max,             0,      nao) \
	  m(UnidadeUHE,  AttComum,   vazao_turbinada_maxima,              double,           0,         max,             0,      nao) \
	  m(UnidadeUHE,  AttComum,          potencia_minima,              double,           0,         max,             0,      nao) \
	  m(UnidadeUHE,  AttComum,          potencia_maxima,              double,           0,         max,             0,      nao) 
//       c_classe,    smrtAtt,             nomeAtributo,                tipo,  lowerBound,  upperBound,  initialValue, mustRead?


#define ATT_VETOR_UNIDADE_UHE(m)  \
	  m(UnidadeUHE,  AttVetor,            disponibilidade,  double,          0,            1,             1,      Periodo)\
	  m(UnidadeUHE,  AttVetor,     vazao_turbinada_maxima,  double,          0,          max,             0,      Periodo)\
	  m(UnidadeUHE,  AttVetor,     vazao_turbinada_minima,  double,          0,          max,             0,      Periodo)
//     c_classe,      smrtAtt,               nomeAtributo,    Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador


#define ATT_MATRIZ_UNIDADE_UHE(m)  \
      m(UnidadeUHE,  AttMatriz,                      potencia_minima,     double,          0,        50000,             0,  Periodo, IdPatamarCarga) \
      m(UnidadeUHE,  AttMatriz,                      potencia_maxima,     double,          0,        50000,             0,  Periodo, IdPatamarCarga) \
      m(UnidadeUHE,  AttMatriz,           potencia_disponivel_minima,     double,          0,        50000,             0,  Periodo, IdPatamarCarga) \
      m(UnidadeUHE,  AttMatriz,           potencia_disponivel_maxima,     double,          0,        50000,             0,  Periodo, IdPatamarCarga) 
//        c_classe,    smrtAtt,                         nomeAtributo,       Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador


#define MEMBRO_UNIDADE_UHE(m)                  \
	m(UnidadeUHE, FuncaoProducaoHidreletrica) 

#define SMART_ELEMENTO_UNIDADE_UHE(m)                   \
	m(UnidadeUHE,    AttComum,  ATT_COMUM_UNIDADE_UHE)  \
	m(UnidadeUHE,    AttVetor,  ATT_VETOR_UNIDADE_UHE)  \
	m(UnidadeUHE,   AttMatriz,  ATT_MATRIZ_UNIDADE_UHE) \
	m(UnidadeUHE,     Membro,   MEMBRO_UNIDADE_UHE) 

 
DEFINE_SMART_ELEMENTO(UnidadeUHE, SMART_ELEMENTO_UNIDADE_UHE)

class UnidadeUHE : public SmartDados {
public:

	UnidadeUHE();
	UnidadeUHE(const UnidadeUHE &instanciaCopiar);
	void esvaziar();
	virtual ~UnidadeUHE();

	DECLARA_SMART_ELEMENTO(UnidadeUHE, SMART_ELEMENTO_UNIDADE_UHE)

};

GET_STRING_FROM_CLASS(UnidadeUHE)

#endif // !defined(C_UNIDADE_UHE)
