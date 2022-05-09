#ifndef PROCESSO_H
#define PROCESSO_H

#include "C_SmartElemento.h"

#define ATT_COMUM_PROCESSO(m)  \
	  m(Processo,  AttComum,    idProcesso,     IdProcesso,         min,         max,           min,      sim)
//     c_classe,   smrtAtt,     nomeAtributo,          tipo,  lowerBound,  upperBound,  initialValue, mustRead?


#define ATT_MATRIZ_PROCESSO(m)  \
      m(Processo,  AttMatriz,  mapeamento_abertura_inicial_por_cenario_por_estagio,         IdAbertura,     Nenhum,          max,           min,  IdCenario,  IdEstagio)      \
      m(Processo,  AttMatriz,    mapeamento_abertura_final_por_cenario_por_estagio,         IdAbertura,     Nenhum,          max,           min,  IdCenario,  IdEstagio) 

#define SMART_ELEMENTO_PROCESSO(m) \
	  m(Processo, AttComum, ATT_COMUM_PROCESSO)\
	  m(Processo, AttMatriz, ATT_MATRIZ_PROCESSO) 

DEFINE_SMART_ELEMENTO(Processo, SMART_ELEMENTO_PROCESSO)

class Processo : public SmartDados {
public:

	Processo();
	Processo(const Processo &instanciaCopiar);
	void esvaziar();
	virtual ~Processo();

	DECLARA_SMART_ELEMENTO(Processo, SMART_ELEMENTO_PROCESSO)


	void setAberturasInicialFinalMapeamento(const IdCenario a_cenario, const IdEstagio a_estagio, const IdAbertura a_abertura_inicial, const IdAbertura a_abertura_final);

};

GET_STRING_FROM_CLASS(Processo)

#endif
