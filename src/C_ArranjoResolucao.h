#ifndef ARRANJO_RESOLUCAO_H
#define ARRANJO_RESOLUCAO_H

#include "C_SmartElemento.h"
#include "C_Processo.h"
#include "C_Iteracao.h"

#define ATT_COMUM_ARRANJO_RESOLUCAO(m)  \
	  m(ArranjoResolucao,  AttComum,     idArranjoResolucao,     IdArranjoResolucao,         min,         max,           min,      sim) \
	  m(ArranjoResolucao,  AttComum,             idProcesso,             IdProcesso,         min,         max,           min,      sim) \
	  m(ArranjoResolucao,  AttComum,       iteracao_inicial,             IdIteracao,         min,         max,           min,      sim) \
	  m(ArranjoResolucao,  AttComum, iteracao_numero_maximo,             IdIteracao,         min,         max,           min,      sim) \
	  m(ArranjoResolucao,  AttComum,         iteracao_final,             IdIteracao,         min,         max,           min,      sim) \
	  m(ArranjoResolucao,  AttComum,          maior_cenario,              IdCenario,         min,         max,           min,      sim) 
//     c_classe,   smrtAtt,     nomeAtributo,          tipo,  lowerBound,  upperBound,  initialValue, mustRead?

#define ATT_VETOR_ARRANJO_RESOLUCAO(m)  \
	m(ArranjoResolucao, AttVetor, agrupar_aberturas, int, 0, 1, 0, IdEstagio)

#define MEMBRO_ARRANJO_RESOLUCAO(m) \
m(ArranjoResolucao, Processo) \
m(ArranjoResolucao, Iteracao)     


#define SMART_ELEMENTO_ARRANJO_RESOLUCAO(m) \
	  m(ArranjoResolucao, AttComum, ATT_COMUM_ARRANJO_RESOLUCAO)\
	  m(ArranjoResolucao, AttVetor, ATT_VETOR_ARRANJO_RESOLUCAO)\
	  m(ArranjoResolucao, Membro, MEMBRO_ARRANJO_RESOLUCAO) 

DEFINE_SMART_ELEMENTO(ArranjoResolucao, SMART_ELEMENTO_ARRANJO_RESOLUCAO)

class ArranjoResolucao : public SmartDados {
public:

	ArranjoResolucao();
	ArranjoResolucao(const ArranjoResolucao &instanciaCopiar);
	ArranjoResolucao& operator=(const ArranjoResolucao& instanciaCopiar);
	void esvaziar();
	virtual ~ArranjoResolucao();

	DECLARA_SMART_ELEMENTO(ArranjoResolucao, SMART_ELEMENTO_ARRANJO_RESOLUCAO)


	void instanciarProcessos(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso);
	bool isIdsCenarioEstadoDiferentesEmAberturasAndCenarios(const IdIteracao a_idIteracao, const IdEstagio a_idEstagio);
	std::vector<IdCenario> getIdsCenarioEstadoFromCenarios(const IdProcesso a_idProcesso, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio);
	std::vector<IdCenario> getIdsCenarioEstadoFromAberturas(const IdProcesso a_idProcesso, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio);
	std::vector<IdCenario> getIdsCenarioEstado(const IdProcesso a_idProcessoDE, const IdProcesso a_idProcessoPARA, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio);
	std::vector<IdCenario> getIdsCenarioEstado(const IdProcesso a_idProcessoDE, const IdProcesso a_idProcessoPARA, const IdIteracao a_idIteracao);
	int getNumeroAberturas(const IdProcesso a_idProcesso, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio);

};

GET_STRING_FROM_CLASS(ArranjoResolucao)

#endif
