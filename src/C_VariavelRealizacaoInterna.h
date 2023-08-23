#ifndef VARIAVEL_REALIZACAO_INTERNA
#define VARIAVEL_REALIZACAO_INTERNA

#include "C_SmartElemento.h"

#define ATT_COMUM_VARIAVEL_REALIZACAO_INTERNA(m)  \
	  m(VariavelRealizacaoInterna,  AttComum, idVariavelRealizacaoInterna,      IdVariavelRealizacaoInterna,         min,          max,           min,      sim) \
	  m(VariavelRealizacaoInterna,  AttComum,                        nome,                           string,         min,          max,           min,      sim) \
	  m(VariavelRealizacaoInterna,  AttComum,                  tipo_valor,                        TipoValor,         min,          max,           min,      sim) \
	  m(VariavelRealizacaoInterna,  AttComum,       percentual_a_utilizar,                           double,           0,            3,             0,      sim) \
	  m(VariavelRealizacaoInterna,  AttComum,          percentual_inicial,                           double,           0,            1,             0,      sim) \
	  m(VariavelRealizacaoInterna,  AttComum,            percentual_passo,                           double,        0.01,            1,           0.1,      sim) \
	  m(VariavelRealizacaoInterna,  AttComum,                       fator,                           double,        0.00,            1,             1,      sim) \
	  m(VariavelRealizacaoInterna,  AttComum,                     periodo,                          Periodo,         min,          max,           min,      sim) \
	  m(VariavelRealizacaoInterna,  AttComum,       idProcessoEstocastico,            IdProcessoEstocastico,         min,          max,           min,      sim) \
	  m(VariavelRealizacaoInterna,  AttComum,         idVariavelAleatoria,              IdVariavelAleatoria,         min,          max,           min,      sim) \
	  m(VariavelRealizacaoInterna,  AttComum,  idVariavelAleatoriaInterna,       IdVariavelAleatoriaInterna,      Nenhum,          max,           min,      sim) 
//                       c_classe,   smrtAtt,                nomeAtributo,                             tipo,  lowerBound,   upperBound,  initialValue, mustRead?


#define ATT_VETOR_VARIAVEL_REALIZACAO_INTERNA(m)  \
	  m(VariavelRealizacaoInterna,   AttVetor,  idVariavelDecisao,     int,        -1,          max,             -1,  TipoSubproblemaSolver)
//            c_classe,    smrtAtt,       nomeAtributo,    Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador

#define SMART_ELEMENTO_VARIAVEL_REALIZACAO_INTERNA(m) \
	m(VariavelRealizacaoInterna, AttComum, ATT_COMUM_VARIAVEL_REALIZACAO_INTERNA)  \
	m(VariavelRealizacaoInterna, AttVetor, ATT_VETOR_VARIAVEL_REALIZACAO_INTERNA)
 

DEFINE_SMART_ELEMENTO(VariavelRealizacaoInterna, SMART_ELEMENTO_VARIAVEL_REALIZACAO_INTERNA)

class VariavelRealizacaoInterna : public SmartDados {

public:

	VariavelRealizacaoInterna();
	VariavelRealizacaoInterna(const VariavelRealizacaoInterna &instanciaCopiar);
	void esvaziar();
	virtual ~VariavelRealizacaoInterna();

	DECLARA_SMART_ELEMENTO(VariavelRealizacaoInterna, SMART_ELEMENTO_VARIAVEL_REALIZACAO_INTERNA)

};

GET_STRING_FROM_CLASS(VariavelRealizacaoInterna)

#endif 
