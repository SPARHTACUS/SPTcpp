#include "C_MetodoSolucao.h"

#include "C_EntradaSaidaDados.h"

#include <chrono>

#include <algorithm>

#include "mpi.h"

void MetodoSolucao::executarPSD(EntradaSaidaDados a_entradaSaidaDados, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, ModeloOtimizacao& a_modeloOtimizacao) {

	try {

		auto start_clock = std::chrono::high_resolution_clock::now();

		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida() + "//PSD";
		const std::string diretorio_pl =  diretorio + "//PLs";
		const std::string diretorio_resultado = diretorio + "//Resultados";
	
		const IdEstagio estagio_inicial = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
		const IdEstagio estagio_final = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

		SmartEnupla<IdEstagio, double> custo_total;

		for (IdEstagio idEstagio = estagio_inicial; idEstagio >= estagio_final; idEstagio--) {			

			try {

				a_modeloOtimizacao.atualizarVariavelEstadoComSolucaoPrimal(IdIteracao_0, idEstagio, IdCenario_1);

				a_modeloOtimizacao.atualizarModeloOtimizacaoComVariavelEstado(idEstagio, IdCenario_1);

				a_modeloOtimizacao.atualizarModeloOtimizacaoComVariavelRealizacao(idEstagio, IdCenario_1);

				a_modeloOtimizacao.otimizarProblema(TipoSubproblemaSolver_geral, IdIteracao_0, idEstagio, diretorio_pl);

				custo_total.addElemento(idEstagio, a_modeloOtimizacao.getCustoTotal(idEstagio));

				if (idEstagio == estagio_inicial)
					a_entradaSaidaDados.setAppendArquivo(false);
				else
					a_entradaSaidaDados.setAppendArquivo(true);

				a_entradaSaidaDados.setDiretorioSaida(diretorio_resultado);
				a_modeloOtimizacao.imprimirSolucaoPorEstagioPorCenario_porEstagio(a_idProcesso, "", a_entradaSaidaDados);
			
			} // try {
				
			catch (const std::exception & erro) { throw std::invalid_argument("Erro no problema de " + getFullString(idEstagio)  + ". \n" + std::string(erro.what())); }

		} // for (IdEstagio idEstagio = a_estagio_final; idEstagio >= a_estagio_inicial; idEstagio--) {

		
		auto stop_clock = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> tempo = stop_clock - start_clock;

		if (a_idProcesso == IdProcesso_mestre) {

			const double tempo_execucao = tempo.count() / 60;				   
					   
			
			// Impressão na Tela
			//

			std::cout << std::endl;
			std::cout << "---------------------------------------------------------" << std::endl;
			std::cout << "Avaliacao da Solucao " << std::endl;
			std::cout << "Modelo: " << getString(a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_idModeloOtimizacao, IdModeloOtimizacao())) << std::endl;
			std::cout << std::endl;
			std::cout << "Custo Total (milhoes): " + getString(custo_total.getElemento(IdEstagio_1) / 1000000) << std::endl;
			std::cout << std::endl;
			std::cout << "Tempo Execucao Iteracao  (minutos): " + getString(tempo_execucao) << std::endl;
			std::cout << "---------------------------------------------------------" << std::endl;
			
		}
	} // try {
	catch (const std::exception & erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarMILP(a_entradaSaidaDados," + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + ",a_modeloOtimizacao): \n" + std::string(erro.what())); }

} // void MetodoSolucao::executarPDDE_backward(const IdEstagio a_estagio_inicial, const IdEstagio a_estagio_final, const IdCenario a_cenario_inicial, const IdCenario a_cenario_final, const IdIteracao a_idIteracao, EntradaSaidaDados a_entradaSaidaDados, const IdProcesso a_idProcesso, ModeloOtimizacao & a_modeloOtimizacao){

