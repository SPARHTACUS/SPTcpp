#include "C_MetodoSolucao.h"

#include "C_EntradaSaidaDados.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MetodoSolucao::MetodoSolucao(EntradaSaidaDados a_entradaSaidaDados, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdMetodoSolucao a_idMetodoSolucao, ModeloOtimizacao &a_modeloOtimizacao){
	try {

		INICIALIZA_SMART_ELEMENTO(MetodoSolucao, SMART_ELEMENTO_METODO_SOLUCAO)

		if ((a_idProcesso <= IdProcesso_Nenhum) || (a_idProcesso >= IdProcesso_Excedente))
			throw std::invalid_argument("Processos invalidos.");
		else if ((a_maiorIdProcesso <= IdProcesso_Nenhum) || (a_maiorIdProcesso >= IdProcesso_Excedente))
			throw std::invalid_argument("Processos invalidos.");
		else if (a_maiorIdProcesso < a_idProcesso)
			throw std::invalid_argument("Processos invalidos.");

		const IdModeloOtimizacao idModeloOtimizacao = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_idModeloOtimizacao, IdModeloOtimizacao());

		if (a_idMetodoSolucao == IdMetodoSolucao_PDDE) {

			if ((idModeloOtimizacao != IdModeloOtimizacao_multiestagio_estocastico_otimizacao) && (idModeloOtimizacao != IdModeloOtimizacao_multiestagio_estocastico_simulacao))
				throw std::invalid_argument("Modelo de otimizacao " + getFullString(idModeloOtimizacao) + " nao compativel com metodo de solucao.");

			executarPDDE(a_entradaSaidaDados, a_idProcesso, a_maiorIdProcesso, a_modeloOtimizacao);

		} // if (a_idMetodoSolucao == IdMetodoSolucao_PDDE) {

		else if (a_idMetodoSolucao == IdMetodoSolucao_MILP) {

 			if ((idModeloOtimizacao != IdModeloOtimizacao_multiestagio_estocastico_otimizacao) && (idModeloOtimizacao != IdModeloOtimizacao_multiestagio_estocastico_simulacao))
				throw std::invalid_argument("Modelo de otimizacao " + getFullString(idModeloOtimizacao) + " nao compativel com metodo de solucao.");

			executarPSD(a_entradaSaidaDados, a_idProcesso, a_maiorIdProcesso, a_modeloOtimizacao);

		} // if (a_idMetodoSolucao == IdMetodoSolucao_PDDE) {

		else
			throw std::invalid_argument("Metodo de solucao " + getString(a_idMetodoSolucao) + " nao implementado.");

	}
	catch (const std::exception&erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::MetodoSolucao(a_entradaSaidaDados," + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + "," + getFullString(a_idMetodoSolucao) + ",a_modeloOtimizacao): \n" + std::string(erro.what())); }
} // MetodoSolucao::MetodoSolucao(EntradaSaidaDados a_entradaSaidaDados, const IdProcesso a_idProcesso, const IdMetodoSolucao a_idMetodoSolucao, ModeloOtimizacao &a_modeloOtimizacao){


MetodoSolucao::MetodoSolucao(const MetodoSolucao & instanciaCopiar){
	try{

	COPIA_SMART_ELEMENTO(MetodoSolucao, SMART_ELEMENTO_METODO_SOLUCAO)

		throw std::invalid_argument("Uma instancia de MetodoSolucao nao eh passivel de copia.");

	}
	catch (const std::exception&erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::MetodoSolucao(const MetodoSolucao &instanciaCopiar): \n" + std::string(erro.what())); }

}

void MetodoSolucao::esvaziar() {
	try {
		EXCLUI_SMART_ELEMENTO(MetodoSolucao, SMART_ELEMENTO_METODO_SOLUCAO)
	}
	catch (const std::exception& erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::esvaziar(): \n" + std::string(erro.what())); }

}

MetodoSolucao::~MetodoSolucao(){
	EXCLUI_SMART_ELEMENTO(MetodoSolucao, SMART_ELEMENTO_METODO_SOLUCAO)
}


void MetodoSolucao::incrementarTempoExecucao(const IdIteracao a_idIteracao, const double a_tempo_adicional) {

	try{

		if (getSizeVetor(AttVetorMetodoSolucao_tempo_execucao) == 0)
			addElemento(AttVetorMetodoSolucao_tempo_execucao, a_idIteracao, a_tempo_adicional);
		else {
			if (getIteradorFinal(AttVetorMetodoSolucao_tempo_execucao, IdIteracao()) < a_idIteracao)
				addElemento(AttVetorMetodoSolucao_tempo_execucao, a_idIteracao, a_tempo_adicional);
			else {
				const double tempo_anterior = getElementoVetor(AttVetorMetodoSolucao_tempo_execucao, a_idIteracao, double());
				setElemento(AttVetorMetodoSolucao_tempo_execucao, a_idIteracao, a_tempo_adicional + tempo_anterior);
			}
		}

		const double tempo_anterior = getAtributo(AttComumMetodoSolucao_tempo_execucao_total, double());
		setAtributo(AttComumMetodoSolucao_tempo_execucao_total, tempo_anterior + a_tempo_adicional);

	} // try {
	catch (const std::exception&erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::incrementarTempoExecucao(" + getFullString(a_idIteracao) + "," + getString(a_tempo_adicional) + "): \n" + std::string(erro.what())); }

} // void MetodoSolucao::incrementarTempoExecucao(const IdIteracao a_idIteracao, const double a_tempo_adicional) {


