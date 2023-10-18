#ifndef RESTRICAO_REALIZACAO
#define RESTRICAO_REALIZACAO

#include "C_SmartElemento.h"

#define ATT_COMUM_RESTRICAO_REALIZACAO(m)  \
	  m(RestricaoRealizacao,  AttComum,        idRestricaoRealizacao,      IdRestricaoRealizacao,         min,          max,           min,      sim) \
	  m(RestricaoRealizacao,  AttComum,                        nome,                    string,         min,          max,           min,      sim)

//     c_classe,   smrtAtt,                           nomeAtributo,                      tipo,  lowerBound,   upperBound,  initialValue, mustRead?

#define ATT_VETOR_RESTRICAO_REALIZACAO(m)  \
	  m(RestricaoRealizacao,   AttVetor,          idRestricao,     int,        -1,          max,             -1,  TipoSubproblemaSolver) \
	  m(RestricaoRealizacao,   AttVetor,                rhs,     double,     min,         max,            0.0,  IdRealizacao) 
//            c_classe,    smrtAtt,       nomeAtributo,    Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador

#define ATT_MATRIZ_RESTRICAO_REALIZACAO(m)  \
      m(RestricaoRealizacao,  AttMatriz,          coeficiente,     double,        min,     max,          0,  int, IdRealizacao) 

#define SMART_ELEMENTO_RESTRICAO_REALIZACAO(m) \
	m(RestricaoRealizacao, AttComum, ATT_COMUM_RESTRICAO_REALIZACAO)  \
	m(RestricaoRealizacao, AttVetor, ATT_VETOR_RESTRICAO_REALIZACAO)  \
	m(RestricaoRealizacao, AttMatriz, ATT_MATRIZ_RESTRICAO_REALIZACAO)  
 

DEFINE_SMART_ELEMENTO(RestricaoRealizacao, SMART_ELEMENTO_RESTRICAO_REALIZACAO)

class RestricaoRealizacao : public SmartDados {

public:

	RestricaoRealizacao();
	RestricaoRealizacao(const RestricaoRealizacao &instanciaCopiar);
	void esvaziar();
	virtual ~RestricaoRealizacao();

	DECLARA_SMART_ELEMENTO(RestricaoRealizacao, SMART_ELEMENTO_RESTRICAO_REALIZACAO)

};

GET_STRING_FROM_CLASS(RestricaoRealizacao)

#endif 
