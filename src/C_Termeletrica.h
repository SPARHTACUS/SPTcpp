#ifndef C_TERMELETRICA_H
#define C_TERMELETRICA_H

#include "C_UnidadeUTE.h"

#include "C_SmartElemento.h"


#define ATT_COMUM_TERMELETRICA(m)  \
      m(Termeletrica,  AttComum,                      idTermeletrica,                       IdTermeletrica,        min,          max,             1,      sim) \
	  m(Termeletrica,  AttComum,                                nome,                               string,        min,          max,             1,      sim) \
	  m(Termeletrica,  AttComum,                          submercado,                         IdSubmercado,     Nenhum,          max,        Nenhum,      sim) \
	  m(Termeletrica,  AttComum,                    tipo_combustivel,                      TipoCombustivel,     Nenhum,          max,        Nenhum,         nao) \
      m(Termeletrica,  AttComum,          tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica,        min,          max,     por_usina,      nao) \
	  m(Termeletrica,  AttComum,                    considerar_usina,                                 bool,      false,         true,          true,      nao) \
	  m(Termeletrica,  AttComum,     representacao_discreta_producao,                                 bool,      false,         true,         false,      nao) \
	  m(Termeletrica,  AttComum,                     potencia_maxima,                               double,          0,        50000,             0,      nao) \
	  m(Termeletrica,  AttComum,                     potencia_minima,                               double,          0,        50000,             0,      nao) \
	  m(Termeletrica,  AttComum,                   custo_de_operacao,                               double,          0,        10000,             0,      nao) \
	  m(Termeletrica,  AttComum,                 fator_de_capacidade,                               double,          0,            1,             1,      nao) \
	  m(Termeletrica,  AttComum,           indisponibilidade_forcada,                               double,          0,            1,             0,      nao) \
	  m(Termeletrica,  AttComum,        indisponibilidade_programada,                               double,          0,          1.0,             0,      nao) \
	  m(Termeletrica,  AttComum, lag_mensal_potencia_disponivel_comandada,                             int,          0,           12,             0,         nao) \
	  m(Termeletrica,  AttComum,                        codigo_usina,                                  int,          0,          999,             1,         nao) \
      m(Termeletrica,  AttComum,                    entrada_operacao,                              Periodo,     01/1900,     01/2100,       01/1900,         nao) \
      m(Termeletrica,  AttComum,         penalidade_violacao_comando,                               double,          0,          max,             0,      nao) \
      m(Termeletrica,  AttComum,        penalidade_violacao_potencia,                               double,          0,          max,             0,      nao) 

//          c_classe,   smrtAtt,              nomeAtributo,                   tipo, lowerBound,   upperBound,  initialValue, mustRead?


#define ATT_VETOR_TERMELETRICA(m)  \
	  m(Termeletrica,  AttVetor,            tipo_detalhamento_producao,  TipoDetalhamentoProducaoTermeletrica,         min,              max,           min,        Periodo) \
	  m(Termeletrica,  AttVetor,     potencia_disponivel_pre_comandada,                                double,           0,              max,             0,        Periodo) \
	  m(Termeletrica,  AttVetor,  potencia_disponivel_comandada_minima,                                double,           0,              max,             0,        Periodo) \
	  m(Termeletrica,  AttVetor,  potencia_disponivel_comandada_maxima,                                double,           0,              max,             0,        Periodo) \
	  m(Termeletrica,  AttVetor,                 potencia_minima_media,                                double,           0,              max,             0,        Periodo) \
	  m(Termeletrica,  AttVetor,                 potencia_maxima_media,                                double,           0,              max,             0,        Periodo) \
	  m(Termeletrica,  AttVetor,      potencia_disponivel_minima_media,                                double,           0,              max,             0,        Periodo) \
	  m(Termeletrica,  AttVetor,      potencia_disponivel_maxima_media,                                double,           0,              max,             0,        Periodo) \
	  m(Termeletrica,  AttVetor,                   potencia_util_media,                                double,           0,              max,             0,        Periodo) \
	  m(Termeletrica,  AttVetor,               custo_de_operacao_media,                                double,           0,              max,             0,        Periodo) \
	  m(Termeletrica,  AttVetor,                   fator_de_capacidade,                                double,           0,            1.001,             0,        Periodo) \
	  m(Termeletrica,  AttVetor,                       disponibilidade,                                double,           0,            1.001,             0,        Periodo) \
	  m(Termeletrica,  AttVetor,             indisponibilidade_forcada,                                double,           0,            1.001,             0,        Periodo) \
	  m(Termeletrica,  AttVetor,          indisponibilidade_programada,                                double,           0,            1.001,             0,        Periodo)
//          c_classe,   smrtAtt,                          nomeAtributo,                                  Tipo,  lowerBound,   upperBound,  initialValue,   TipoIterador


#define ATT_MATRIZ_TERMELETRICA(m)  \
	  m(Termeletrica,  AttMatriz,              custo_de_operacao,        double,          0,          max,             0,  Periodo,   IdPatamarCarga) \
	  m(Termeletrica,  AttMatriz,                  potencia_util,        double,          0,          max,             0,  Periodo,   IdPatamarCarga) \
	  m(Termeletrica,  AttMatriz,                potencia_minima,        double,          0,          max,             0,  Periodo,   IdPatamarCarga) \
	  m(Termeletrica,  AttMatriz,                potencia_maxima,        double,          0,          max,             0,  Periodo,   IdPatamarCarga) \
	  m(Termeletrica,  AttMatriz,     potencia_disponivel_minima,        double,          0,          max,             0,  Periodo,   IdPatamarCarga) \
	  m(Termeletrica,  AttMatriz,     potencia_disponivel_maxima,        double,          0,          max,             0,  Periodo,   IdPatamarCarga) \
	  m(Termeletrica,  AttMatriz,       potencia_disponivel_meta,        double,          0,          max,             0,  IdCenario, Periodo) 
//          c_classe,    smrtAtt,                   nomeAtributo,          Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador

#define MEMBRO_TERMELETRICA(m)          \
	m(Termeletrica, UnidadeUTE)         

#define SMART_ELEMENTO_TERMELETRICA(m) \
	m(Termeletrica, AttComum,  ATT_COMUM_TERMELETRICA)  \
	m(Termeletrica, AttVetor,  ATT_VETOR_TERMELETRICA)  \
	m(Termeletrica, AttMatriz, ATT_MATRIZ_TERMELETRICA) \
	m(Termeletrica,   Membro,    MEMBRO_TERMELETRICA)

DEFINE_SMART_ELEMENTO(Termeletrica, SMART_ELEMENTO_TERMELETRICA)


class Termeletrica: public SmartDados{

public:
	
	Termeletrica();			// Construtor
	Termeletrica(const Termeletrica &instanciaCopiar);
	void esvaziar();
	virtual ~Termeletrica();	// Destrutor

	DECLARA_SMART_ELEMENTO(Termeletrica, SMART_ELEMENTO_TERMELETRICA)

};

GET_STRING_FROM_CLASS(Termeletrica)

#endif // !defined(C_TERMELETRICA_H)
