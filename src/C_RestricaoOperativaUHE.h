#ifndef REST_OPER_LIMITE_UHE
#define REST_OPER_LIMITE_UHE

#include "C_ElementoSistema.h"
#include "C_SmartElemento.h"

#define ATT_COMUM_REST_OPER_LIMITE_UHE(m)  \
	  m(RestricaoOperativaUHE,  AttComum,  idRestricaoOperativaUHE, IdRestricaoOperativaUHE,         min,         max,           min,       sim) \
      m(RestricaoOperativaUHE,  AttComum,                     nome,                  string,         min,          max,        Nenhum,      sim) \
	  m(RestricaoOperativaUHE,  AttComum,          limite_inferior,                  double,           0,      100000,             0,       nao) \
	  m(RestricaoOperativaUHE,  AttComum,          limite_superior,                  double,           0,      100000,        100000,       nao) \
      m(RestricaoOperativaUHE,  AttComum,     rampa_min_percentual,                  double,           0,      100000,        100000,       nao) \
      m(RestricaoOperativaUHE,  AttComum,     rampa_max_percentual,                  double,           0,      100000,        100000,       nao) \
      m(RestricaoOperativaUHE,  AttComum,       rampa_min_absoluta,                  double,           0,      100000,        100000,       nao) \
      m(RestricaoOperativaUHE,  AttComum,       rampa_max_absoluta,                  double,           0,      100000,        100000,       nao) \
      m(RestricaoOperativaUHE,  AttComum,            valor_inicial,                  double,           0,      100000,        100000,       nao) \
	  m(RestricaoOperativaUHE,  AttComum,           periodo_inicio,                 Periodo,         min,         max,           min,       nao) \
	  m(RestricaoOperativaUHE,  AttComum,            periodo_final,                 Periodo,         min,         max,           min,       nao) \
      m(RestricaoOperativaUHE,  AttComum,            num_restricao,                     int,           0,      100000,             0,       nao) \
      m(RestricaoOperativaUHE,  AttComum,  tipoRestricaoHidraulica,  TipoRestricaoHidraulica,        min,         max,           min,       sim) \
      m(RestricaoOperativaUHE,  AttComum, penalidade_restricao_operativa_UHE,         double,           0,        max,          5000,       sim) 
//     c_classe,             smrtAtt,                 nomeAtributo,                  tipo,  lowerBound,  upperBound,  initialValue, mustRead?

#define ATT_VETOR_REST_OPER_LIMITE_UHE(m)  \
	  m(RestricaoOperativaUHE,  AttVetor,     limite_inferior,             double,        min,          max,           max,      Periodo) \
	  m(RestricaoOperativaUHE,  AttVetor,     limite_superior,             double,        min,          max,           max,      Periodo) \
	  m(RestricaoOperativaUHE,  AttVetor,     limite_inferior_media,             double,        min,          max,           max,      Periodo) \
	  m(RestricaoOperativaUHE,  AttVetor,     limite_superior_media,             double,        min,          max,           max,      Periodo) 
//               c_classe,   smrtAtt,     nomeAtributo,                   Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador

#define ATT_MATRIZ_REST_OPER_LIMITE_UHE(m)  \
	  m(RestricaoOperativaUHE,  AttMatriz,             limite_inferior,     double,          min,          max,             max,  Periodo, IdPatamarCarga) \
      m(RestricaoOperativaUHE,  AttMatriz,             limite_superior,     double,          min,          max,             max,  Periodo, IdPatamarCarga) 

//Nota: limite_inferior/limite_superior com valor max não é considerado dentro do modelo

#define MEMBRO_REST_OPER_LIMITE_UHE(m)          \
	m(RestricaoOperativaUHE, ElementoSistema)  

#define SMART_ELEMENTO_REST_OPER_LIMITE_UHE(m)\
	m(RestricaoOperativaUHE,   AttComum,  ATT_COMUM_REST_OPER_LIMITE_UHE) \
    m(RestricaoOperativaUHE,   AttVetor, ATT_VETOR_REST_OPER_LIMITE_UHE)  \
    m(RestricaoOperativaUHE,  AttMatriz, ATT_MATRIZ_REST_OPER_LIMITE_UHE) \
	m(RestricaoOperativaUHE,     Membro, MEMBRO_REST_OPER_LIMITE_UHE) 

DEFINE_SMART_ELEMENTO(RestricaoOperativaUHE, SMART_ELEMENTO_REST_OPER_LIMITE_UHE)

class RestricaoOperativaUHE : public SmartDados {
public:

	RestricaoOperativaUHE();
	RestricaoOperativaUHE(const RestricaoOperativaUHE &instanciaCopiar);
	RestricaoOperativaUHE& operator=(const RestricaoOperativaUHE& instanciaCopiar);
	void esvaziar();
	virtual ~RestricaoOperativaUHE();

	DECLARA_SMART_ELEMENTO(RestricaoOperativaUHE, SMART_ELEMENTO_REST_OPER_LIMITE_UHE)

};

GET_STRING_FROM_CLASS(RestricaoOperativaUHE)

#endif // !defined(C_UNIDADE_UTE)
#pragma once
