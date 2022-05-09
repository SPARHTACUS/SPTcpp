#ifndef C_SUBMERCADO_H
#define C_SUBMERCADO_H

#include "C_PatamarDeficit.h"
#include "C_UsinaNaoSimulada.h"

#include "C_SmartElemento.h"

#define ATT_COMUM_SUBMERCADO(m)  \
	  m(Submercado,  AttComum,              idSubmercado,   IdSubmercado,         min,          max,           min,      sim) \
	  m(Submercado,  AttComum,                      nome,         string,         min,          max,        Nenhum,      sim) \
	  m(Submercado,  AttComum,                  ficticio,           bool,           0,            1,             0,      nao) \
      m(Submercado,  AttComum,                   demanda,         double,           0,      1000000,             0,      nao)    
//        c_classe,   smrtAtt,              nomeAtributo,           tipo,  lowerBound,   upperBound,  initialValue, mustRead?


#define ATT_VETOR_SUBMERCADO(m)  \
	  m(Submercado,  AttVetor,              demanda,               double,          0,          max,             0,     Periodo) \
	  m(Submercado,  AttVetor,        demanda_media,               double,          0,          max,             0,     Periodo)
//        c_classe,   smrtAtt,         nomeAtributo,                 Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador

#define ATT_MATRIZ_SUBMERCADO(m)  \
	  m(Submercado,  AttMatriz,                           demanda,               double,            0,           max,             0,  Periodo,        IdPatamarCarga) \
	  m(Submercado,  AttMatriz, percentual_variacao_patamar_carga,               double,            0,            10,             0,  Periodo,        IdPatamarCarga) \
	  m(Submercado,  AttMatriz,   produtibilidade_media_acumulada,               double,            0,            10,             0,  IdHidreletrica,        Periodo) \
	  m(Submercado,  AttMatriz,   produtibilidade_equiv_acumulada,               double,            0,            10,             0,  IdHidreletrica,        Periodo) 
//        c_classe,    smrtAtt,                      nomeAtributo,     Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador

#define MEMBRO_SUBMERCADO(m)        \
	m(Submercado, PatamarDeficit)   \
	m(Submercado, UsinaNaoSimulada)  


#define SMART_ELEMENTO_SUBMERCADO(m) \
	m(Submercado,  AttComum, ATT_COMUM_SUBMERCADO)  \
	m(Submercado,  AttVetor, ATT_VETOR_SUBMERCADO)  \
	m(Submercado, AttMatriz, ATT_MATRIZ_SUBMERCADO) \
	m(Submercado,    Membro, MEMBRO_SUBMERCADO)

DEFINE_SMART_ELEMENTO(Submercado, SMART_ELEMENTO_SUBMERCADO)

class Submercado : public SmartDados {
public:

	Submercado();
	Submercado(const Submercado &instanciaCopiar);
	void esvaziar();
	virtual ~Submercado();

	DECLARA_SMART_ELEMENTO(Submercado, SMART_ELEMENTO_SUBMERCADO)

};

GET_STRING_FROM_CLASS(Submercado)

#endif // !defined(C_SUBMERCADO_H)
