#ifndef ITERACAO_H
#define ITERACAO_H

#include "C_SmartElemento.h"
#include "C_Processo.h"

#define ATT_COMUM_ITERACAO(m)  \
	  m(Iteracao,  AttComum,       idIteracao,    IdIteracao,         min,         max,           min,      sim) \
	  m(Iteracao,  AttComum,    menor_cenario,     IdCenario,         Nenhum,         max,           Nenhum,      sim) \
	  m(Iteracao,  AttComum,    maior_cenario,     IdCenario,         Nenhum,         max,           Nenhum,      sim) 
//     c_classe,   smrtAtt,     nomeAtributo,          tipo,  lowerBound,  upperBound,  initialValue, mustRead?

#define MEMBRO_ITERACAO(m) \
m(Iteracao, Processo)     

#define SMART_ELEMENTO_ITERACAO(m) \
	  m(Iteracao, AttComum, ATT_COMUM_ITERACAO)\
	  m(Iteracao, Membro, MEMBRO_ITERACAO)

DEFINE_SMART_ELEMENTO(Iteracao, SMART_ELEMENTO_ITERACAO)

class Iteracao : public SmartDados {
public:

	Iteracao();
	Iteracao(const Iteracao &instanciaCopiar);
	void esvaziar();
	virtual ~Iteracao();

	DECLARA_SMART_ELEMENTO(Iteracao, SMART_ELEMENTO_ITERACAO)

};

GET_STRING_FROM_CLASS(Iteracao)

#endif
