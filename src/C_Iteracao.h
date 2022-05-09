#ifndef ITERACAO_H
#define ITERACAO_H

#include "C_SmartElemento.h"
#include "C_Processo.h"

#define ATT_COMUM_ITERACAO(m)  \
	  m(Iteracao,  AttComum,    idIteracao,     IdIteracao,         min,         max,           min,      sim)
//     c_classe,   smrtAtt,     nomeAtributo,          tipo,  lowerBound,  upperBound,  initialValue, mustRead?

#define ATT_VETOR_ITERACAO(m)  \
	  m(Iteracao,  AttVetor,  tipo_processamento_paralelo,       TipoProcessamentoParalelo,        min,          max,           min,  IdEstagio) 

#define MEMBRO_ITERACAO(m)     \
	m(Iteracao, Processo)  

#define SMART_ELEMENTO_ITERACAO(m) \
	  m(Iteracao, AttComum, ATT_COMUM_ITERACAO)\
	  m(Iteracao, AttVetor, ATT_VETOR_ITERACAO)\
	  m(Iteracao, Membro, MEMBRO_ITERACAO) 

DEFINE_SMART_ELEMENTO(Iteracao, SMART_ELEMENTO_ITERACAO)

class Iteracao : public SmartDados {
public:

	Iteracao();
	Iteracao(const Iteracao &instanciaCopiar);
	void esvaziar();
	virtual ~Iteracao();

	DECLARA_SMART_ELEMENTO(Iteracao, SMART_ELEMENTO_ITERACAO)

	void setAberturasInicialFinalMapeamento(const IdProcesso a_processo, const IdCenario a_cenario, const IdEstagio a_estagio, const IdAbertura a_abertura_inicial, const IdAbertura a_abertura_final);

};

GET_STRING_FROM_CLASS(Iteracao)

#endif
