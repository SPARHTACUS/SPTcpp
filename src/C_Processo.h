#ifndef PROCESSO_H
#define PROCESSO_H

#include "C_SmartElemento.h"

#define ATT_COMUM_PROCESSO(m)  \
	  m(Processo,  AttComum,    idProcesso,     IdProcesso,         min,         max,           min,      sim) \
	  m(Processo,  AttComum, menor_cenario,      IdCenario,         Nenhum,         max,           Nenhum,      sim) \
	  m(Processo,  AttComum, maior_cenario,      IdCenario,         Nenhum,         max,           Nenhum,      sim) 
//     c_classe,   smrtAtt,     nomeAtributo,          tipo,  lowerBound,  upperBound,  initialValue, mustRead?


#define ATT_MATRIZ_PROCESSO(m)  \
      m(Processo,  AttMatriz,  menor_abertura_por_cenario_estado,        IdAbertura,     Nenhum,          max,           Nenhum,  IdCenario,  IdEstagio) \
      m(Processo,  AttMatriz,  maior_abertura_por_cenario_estado,        IdAbertura,     Nenhum,          max,           Nenhum,  IdCenario,  IdEstagio) \
      m(Processo,  AttMatriz,  cenario_estado_por_cenario,         IdCenario,     Nenhum,          max,           Nenhum,  IdCenario,  IdEstagio) 


#define SMART_ELEMENTO_PROCESSO(m) \
	  m(Processo, AttComum, ATT_COMUM_PROCESSO) \
	  m(Processo, AttMatriz, ATT_MATRIZ_PROCESSO)

DEFINE_SMART_ELEMENTO(Processo, SMART_ELEMENTO_PROCESSO)

class Processo : public SmartDados {
public:

	Processo();
	Processo(const Processo &instanciaCopiar);
	void esvaziar();
	virtual ~Processo();

	DECLARA_SMART_ELEMENTO(Processo, SMART_ELEMENTO_PROCESSO)


};

GET_STRING_FROM_CLASS(Processo)

#endif
