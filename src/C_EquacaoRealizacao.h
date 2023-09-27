#ifndef EQUACAO_REALIZACAO
#define EQUACAO_REALIZACAO

#include "C_SmartElemento.h"

#define ATT_COMUM_EQUACAO_REALIZACAO(m)  \
	  m(EquacaoRealizacao,  AttComum,        idEquacaoRealizacao,      IdEquacaoRealizacao,         min,          max,           min,      sim) \
	  m(EquacaoRealizacao,  AttComum,                        nome,                    string,         min,          max,           min,      sim)

//     c_classe,   smrtAtt,                           nomeAtributo,                      tipo,  lowerBound,   upperBound,  initialValue, mustRead?

#define ATT_VETOR_EQUACAO_REALIZACAO(m)  \
	  m(EquacaoRealizacao,   AttVetor,          idEquacao,     int,        -1,          max,             -1,  TipoSubproblemaSolver) \
	  m(EquacaoRealizacao,   AttVetor,                rhs,     double,     min,         max,            0.0,  IdRealizacao) 
//            c_classe,    smrtAtt,       nomeAtributo,    Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador

#define ATT_MATRIZ_EQUACAO_REALIZACAO(m)  \
      m(EquacaoRealizacao,  AttMatriz,          coeficiente,     double,        min,     max,          0,  int, IdRealizacao) 

#define SMART_ELEMENTO_EQUACAO_REALIZACAO(m) \
	m(EquacaoRealizacao, AttComum, ATT_COMUM_EQUACAO_REALIZACAO)  \
	m(EquacaoRealizacao, AttVetor, ATT_VETOR_EQUACAO_REALIZACAO)  \
	m(EquacaoRealizacao, AttMatriz, ATT_MATRIZ_EQUACAO_REALIZACAO)  
 

DEFINE_SMART_ELEMENTO(EquacaoRealizacao, SMART_ELEMENTO_EQUACAO_REALIZACAO)

class EquacaoRealizacao : public SmartDados {

public:

	EquacaoRealizacao();
	EquacaoRealizacao(const EquacaoRealizacao &instanciaCopiar);
	void esvaziar();
	virtual ~EquacaoRealizacao();

	DECLARA_SMART_ELEMENTO(EquacaoRealizacao, SMART_ELEMENTO_EQUACAO_REALIZACAO)

};

GET_STRING_FROM_CLASS(EquacaoRealizacao)

#endif 
