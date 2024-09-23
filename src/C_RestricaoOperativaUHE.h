#ifndef REST_OPER_LIMITE_UHE
#define REST_OPER_LIMITE_UHE

#include "C_ElementoSistema.h"
#include "C_SmartElemento.h"

#define ATT_COMUM_REST_OPER_LIMITE_UHE(m)  \
	  m(RestricaoOperativaUHE,  AttComum,        idRestricaoOperativaUHE, IdRestricaoOperativaUHE,         min,         max,           min,       sim) \
      m(RestricaoOperativaUHE,  AttComum,                           nome,                  string,         min,          max,        Nenhum,      sim) \
	  m(RestricaoOperativaUHE,  AttComum,                        lim_inf,                  double,           0,      100000,             0,       nao) \
	  m(RestricaoOperativaUHE,  AttComum,                        lim_sup,                  double,           0,      100000,        100000,       nao) \
	  m(RestricaoOperativaUHE,  AttComum,              lim_sup_folga_inf,                  double,           0,         max,           max,       nao) \
      m(RestricaoOperativaUHE,  AttComum,                  valor_inicial,                  double,           0,      100000,        100000,       nao) \
	  m(RestricaoOperativaUHE,  AttComum,                 periodo_inicio,                 Periodo,         min,         max,           min,       nao) \
	  m(RestricaoOperativaUHE,  AttComum,                  periodo_final,                 Periodo,         min,         max,           min,       nao) \
      m(RestricaoOperativaUHE,  AttComum,                  num_restricao,                     int,           0,      100000,             0,       nao) \
      m(RestricaoOperativaUHE,  AttComum,        tipoRestricaoHidraulica,  TipoRestricaoHidraulica,     nenhum,         max,           min,       sim) \
      m(RestricaoOperativaUHE,  AttComum,                        vlr_ini,                   double,        min,        max,            max,       nao) \
      m(RestricaoOperativaUHE,  AttComum,                     penalidade,                   double,          0,        max,          10000,       sim) \
      m(RestricaoOperativaUHE,  AttComum, tipoUnidadeRestricaoHidraulica,    TipoUnidadeRestricaoHidraulica,        min,         max,           min,       nao)
//     c_classe,             smrtAtt,                 nomeAtributo,                  tipo,  lowerBound,  upperBound,  initialValue, mustRead?

#define ATT_VETOR_REST_OPER_LIMITE_UHE(m)  \
	  m(RestricaoOperativaUHE,  AttVetor,     lim_inf,             double,        min,          max,           max,      Periodo) \
	  m(RestricaoOperativaUHE,  AttVetor,     lim_sup,             double,        min,          max,           max,      Periodo) \
	  m(RestricaoOperativaUHE,  AttVetor,     lim_sup_folga_inf,   double,          0,          max,           max,      Periodo) 
//               c_classe,   smrtAtt,     nomeAtributo,                   Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador

#define ATT_MATRIZ_REST_OPER_LIMITE_UHE(m)  \
	  m(RestricaoOperativaUHE,  AttMatriz,    var_abs_inf,     double,          min,          max,             max,  Periodo, IdPatamarCarga) \
	  m(RestricaoOperativaUHE,  AttMatriz,    var_abs_sup,     double,          min,          max,             max,  Periodo, IdPatamarCarga) \
	  m(RestricaoOperativaUHE,  AttMatriz,    var_rel_inf,     double,          min,          max,             max,  Periodo, IdPatamarCarga) \
	  m(RestricaoOperativaUHE,  AttMatriz,    var_rel_sup,     double,          min,          max,             max,  Periodo, IdPatamarCarga) \
	  m(RestricaoOperativaUHE,  AttMatriz,             lim_inf,     double,          min,          max,             max,  Periodo, IdPatamarCarga) \
      m(RestricaoOperativaUHE,  AttMatriz,             lim_sup,     double,          min,          max,             max,  Periodo, IdPatamarCarga) 

//Nota: lim_inf/lim_sup com valor max não é considerado dentro do modelo

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
