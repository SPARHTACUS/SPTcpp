#ifndef VARIAVEL_REALIZACAO
#define VARIAVEL_REALIZACAO

#include "C_SmartElemento.h"

#define ATT_COMUM_VARIAVEL_REALIZACAO(m)  \
	  m(VariavelRealizacao,  AttComum,        idVariavelRealizacao,      IdVariavelRealizacao,         min,          max,           min,      sim) \
	  m(VariavelRealizacao,  AttComum,                        nome,                    string,         min,          max,           min,      sim) \
	  m(VariavelRealizacao,  AttComum,                     periodo,                   Periodo,         min,          max,           min,      nao) \
	  m(VariavelRealizacao,  AttComum,                       fator,                    double,         0.0,          1.0,           1.0,      nao) \
	  m(VariavelRealizacao,  AttComum,       idProcessoEstocastico,     IdProcessoEstocastico,         min,          max,           min,      nao) \
	  m(VariavelRealizacao,  AttComum,         idVariavelAleatoria,       IdVariavelAleatoria,         min,          max,           min,      nao) 

//     c_classe,   smrtAtt,                           nomeAtributo,                      tipo,  lowerBound,   upperBound,  initialValue, mustRead?

#define ATT_VETOR_VARIAVEL_REALIZACAO(m)  \
	  m(VariavelRealizacao,   AttVetor,  idVariavelDecisao,     int,        -1,          max,             -1,  TipoSubproblemaSolver) \
	  m(VariavelRealizacao,   AttVetor,            fator_0,     double,     min,         max,            0.0,  IdRealizacao) \
	  m(VariavelRealizacao,   AttVetor,            fator_1,     double,     min,         max,            0.0,  IdRealizacao) 
//            c_classe,    smrtAtt,       nomeAtributo,    Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador

#define SMART_ELEMENTO_VARIAVEL_REALIZACAO(m) \
	m(VariavelRealizacao, AttComum, ATT_COMUM_VARIAVEL_REALIZACAO)  \
	m(VariavelRealizacao, AttVetor, ATT_VETOR_VARIAVEL_REALIZACAO)  
 

DEFINE_SMART_ELEMENTO(VariavelRealizacao, SMART_ELEMENTO_VARIAVEL_REALIZACAO)

class VariavelRealizacao : public SmartDados {

public:

	VariavelRealizacao();
	VariavelRealizacao(const VariavelRealizacao &instanciaCopiar);
	void esvaziar();
	virtual ~VariavelRealizacao();

	DECLARA_SMART_ELEMENTO(VariavelRealizacao, SMART_ELEMENTO_VARIAVEL_REALIZACAO)

};

GET_STRING_FROM_CLASS(VariavelRealizacao)

#endif 
