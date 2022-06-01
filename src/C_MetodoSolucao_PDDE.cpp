#include "C_MetodoSolucao.h"

#include "C_EntradaSaidaDados.h"

#include <chrono>

#include <algorithm>

#include "mpi.h"

void MetodoSolucao::executarPDDE(EntradaSaidaDados a_entradaSaidaDados, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, ModeloOtimizacao &a_modeloOtimizacao){

	try {

		const IdEstagio estagio_inicial = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
		const IdEstagio estagio_final = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

		const IdIteracao iteracao_inicial  = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_iteracao_inicial,  IdIteracao());
		IdIteracao iteracao_final    = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_iteracao_final,    IdIteracao());
		
		IdIteracao iteracao_simulacao_final = IdIteracao_Nenhum;
		if (a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_realizar_simulacao_final, bool())) {
			iteracao_simulacao_final = IdIteracao(iteracao_final + 1);
			iteracao_final = iteracao_simulacao_final;
		}
		else if (iteracao_final == IdIteracao_0)
			iteracao_simulacao_final = iteracao_final;

		for (IdIteracao idIteracao = iteracao_inicial; idIteracao <= iteracao_final; idIteracao++) {

			bool simulacao = false;

			if (idIteracao == IdIteracao_0)
				simulacao = true;

			if (idIteracao == iteracao_simulacao_final){
				simulacao = true;
				a_modeloOtimizacao.setAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario, true);
				a_modeloOtimizacao.setAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, true);
			} // if (idIteracao == iteracao_simulacao_final){

			executarPDDE_forward(a_entradaSaidaDados, estagio_inicial, estagio_final, simulacao, idIteracao, a_idProcesso, a_maiorIdProcesso, a_modeloOtimizacao);

			if (!simulacao)
				executarPDDE_backward_new(a_entradaSaidaDados, estagio_inicial, estagio_final, idIteracao, a_idProcesso, a_maiorIdProcesso, a_modeloOtimizacao);

			executarPDDE_avaliarSolucao(a_entradaSaidaDados, idIteracao, a_idProcesso, a_modeloOtimizacao);

		} // for (IdIteracao idIteracao = iteracao_inicial; idIteracao <= iteracao_final; idIteracao++) {

		executarPDDE_operacao_final(a_entradaSaidaDados, a_idProcesso, a_maiorIdProcesso, a_modeloOtimizacao);

		if (true) {
			int barreira = 0;

			if (a_idProcesso == IdProcesso_mestre) {
				for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= a_maiorIdProcesso; idProcesso++)
					MPI_Send(&barreira, 1, MPI_INT, getRank(idProcesso), 0, MPI_COMM_WORLD);
			}
			else
				MPI_Recv(&barreira, 1, MPI_INT, getRank(IdProcesso_mestre), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}

	} // try {
	catch (const std::exception&erro) { 
		
		if (a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_idModeloOtimizacao, IdModeloOtimizacao()) == IdModeloOtimizacao_multiestagio_estocastico_otimizacao)
			a_modeloOtimizacao.exportarCorteBenders(a_idProcesso, a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio()), a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio()), a_entradaSaidaDados);

		throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE(a_entradaSaidaDados, " + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + ", a_modeloOtimizacao): \n" + std::string(erro.what()));
	
	} // catch (const std::exception&erro) { 

} // void MetodoSolucao::executarPDDE(Dados &a_dados, EntradaSaidaDados a_entradaSaidaDados, const IdModeloOtimizacao a_idModeloOtimizacao){


void MetodoSolucao::executarPDDE_forward(EntradaSaidaDados a_entradaSaidaDados, const IdEstagio a_estagio_inicial, const IdEstagio a_estagio_final, const bool a_simulacao, const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, ModeloOtimizacao &a_modeloOtimizacao){

	try {

		auto start_clock = std::chrono::high_resolution_clock::now();

		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida();
		const std::string diretorio_iteracao = diretorio + "//" + getFullString(a_idIteracao) + "//Foward";
		const std::string diretorio_resultado = diretorio_iteracao + "//Resultados";
		const std::string diretorio_pl = diretorio_iteracao + "//PLs";
		const std::string diretorio_tempos = diretorio_iteracao + "//Tempos";
		const std::string diretorio_selecao_corte = diretorio_iteracao + "//Selecao_Cortes";

		const bool imprimir_tempos = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_imprimir_tempos_metodo_solucao, bool());

		const TipoSubproblemaSolver tSS = TipoSubproblemaSolver_geral;

		const IdIteracao iteracao_inicial = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_iteracao_inicial, IdIteracao());

		if ((a_idIteracao == iteracao_inicial) && (a_idProcesso == IdProcesso_mestre)) {

			a_modeloOtimizacao.imprimirEstagios(a_entradaSaidaDados);

			a_modeloOtimizacao.imprimirVariaveisEstado(a_entradaSaidaDados);

			a_modeloOtimizacao.imprimirVariaveisRealizacao(a_entradaSaidaDados);

			a_modeloOtimizacao.imprimirVariaveisRealizacaoInterna(a_entradaSaidaDados);

			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getString(AttVetorEstagio_selecao_solucao_proxy) + ".csv", IdEstagio_Nenhum, a_modeloOtimizacao, AttVetorEstagio_selecao_solucao_proxy);

		} // if ((a_idIteracao == iteracao_inicial) && (a_idProcesso == IdProcesso_mestre)) {

		if (a_idIteracao == IdIteracao_0)
			a_modeloOtimizacao.importarCorteBenders_AcoplamentoPosEstudo(tSS, a_idProcesso, a_idIteracao, diretorio_selecao_corte, a_entradaSaidaDados);

		const IdCenario cenario_inicial = a_modeloOtimizacao.getCenarioInicial(a_idProcesso, a_idIteracao);
		const IdCenario cenario_final = a_modeloOtimizacao.getCenarioFinal(a_idProcesso, a_idIteracao);

		const int numero_cenarios = a_modeloOtimizacao.getNumeroCenarios(cenario_inicial, cenario_final);

		double tempo_medio_otimizacao_solver = 0.0;

		SmartEnupla<IdCenario, double> custo_inferior_via_mestre;
		SmartEnupla<IdCenario, double> custo_inferior;
		SmartEnupla<IdCenario, double> custo_superior_via_mestre;
		SmartEnupla<IdCenario, double> custo_superior;
		SmartEnupla<IdCenario, double> probabilidade_cenario;

		if (numero_cenarios > 0) {

			custo_inferior_via_mestre = SmartEnupla<IdCenario, double>(cenario_inicial, std::vector<double>(numero_cenarios, 0.0));
			custo_inferior = SmartEnupla<IdCenario, double>(cenario_inicial, std::vector<double>(numero_cenarios, 0.0));
			custo_superior_via_mestre = SmartEnupla<IdCenario, double>(cenario_inicial, std::vector<double>(numero_cenarios, 0.0));
			custo_superior = SmartEnupla<IdCenario, double>(cenario_inicial, std::vector<double>(numero_cenarios, 0.0));
			probabilidade_cenario = SmartEnupla<IdCenario, double>(cenario_inicial, std::vector<double>(numero_cenarios, 1.0));

			const int numero_estagios = int(a_estagio_final - a_estagio_inicial) + 1;

			const double multiplicador_tempo_limite_resolucao = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_multiplicador_tempo_limite_resolucao, double());

			double tempo_medio_solver_bw = 1000;

			if ((a_idIteracao > IdIteracao(iteracao_inicial + 1)) || ((a_idIteracao > iteracao_inicial) && iteracao_inicial > IdIteracao_0))
				tempo_medio_solver_bw = getElementoVetor(AttVetorMetodoSolucao_tempo_medio_solver_bw, IdIteracao(a_idIteracao - 1), double());

			const double tempo_limite = multiplicador_tempo_limite_resolucao * tempo_medio_solver_bw;

			a_entradaSaidaDados.setDiretorioSaida(diretorio_iteracao);
			a_modeloOtimizacao.gerarRealizacoes(a_idProcesso, a_maiorIdProcesso, cenario_inicial, cenario_final, a_entradaSaidaDados);
			a_entradaSaidaDados.setDiretorioSaida(diretorio);

			for (IdEstagio idEstagio = a_estagio_inicial; idEstagio <= a_estagio_final; idEstagio++) {

				if ((idEstagio == a_estagio_inicial) && (a_estagio_inicial == a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_estagio_acoplamento_pre_estudo, IdEstagio()))) {
					if (a_idIteracao < a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_iteracao_final, IdIteracao()))
						a_modeloOtimizacao.importarVariaveisEstado_AcoplamentoPreEstudo(a_idProcesso, a_maiorIdProcesso, a_idIteracao, cenario_inicial, cenario_final, a_entradaSaidaDados);
					else
						a_modeloOtimizacao.importarVariaveisEstado_AcoplamentoPreEstudo(a_idProcesso, a_maiorIdProcesso, a_idIteracao, a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()), a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()), a_entradaSaidaDados);
				}

				a_modeloOtimizacao.setTempoLimiteOtimizacao(tSS, idEstagio, -1.0);

				std::vector<std::vector<std::string>> valores_tempo(2, std::vector<std::string>());
				std::vector<std::vector<std::string>> valores_tempo_otimizacao(2, std::vector<std::string>());
				std::vector<std::vector<std::string>> valores_tratamento(2, std::vector<std::string>());

				if (imprimir_tempos) {
					valores_tempo.at(1).reserve(int(cenario_final - cenario_inicial) + 2);
					valores_tempo.at(1).push_back(getFullString(idEstagio) + ";" + getFullString(a_idProcesso) + ";");

					valores_tempo_otimizacao.at(1).reserve(int(cenario_final - cenario_inicial) + 2);
					valores_tempo_otimizacao.at(1).push_back(getFullString(idEstagio) + ";" + getFullString(a_idProcesso) + ";");

					valores_tratamento.at(1).reserve(int(cenario_final - cenario_inicial) + 2);
					valores_tratamento.at(1).push_back(getFullString(idEstagio) + ";" + getFullString(a_idProcesso) + ";");
				} // if (imprimir_tempos) {
				for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

					try {

						auto start_clock_cenario = std::chrono::high_resolution_clock::now();

						a_modeloOtimizacao.atualizarModeloOtimizacaoComVariavelEstado(idEstagio, a_idProcesso, a_maiorIdProcesso, idCenario);

						a_modeloOtimizacao.atualizarModeloOtimizacaoComVariavelRealizacao(idEstagio, idCenario);

						a_modeloOtimizacao.otimizarProblema(tSS, a_idProcesso, a_idIteracao, idEstagio, idCenario, diretorio_pl);

						if (idEstagio == a_estagio_inicial)
							custo_inferior.at(idCenario) = a_modeloOtimizacao.getCustoTotal(idEstagio);

						if (idEstagio < a_estagio_final)
							custo_superior.at(idCenario) += a_modeloOtimizacao.getCustoImediato(idEstagio);
						else
							custo_superior.at(idCenario) += a_modeloOtimizacao.getCustoTotal(idEstagio);

						if (true) {
							double custo_geral = 0.0;
							double custo_individual = 0.0;
							a_modeloOtimizacao.calcularCustoPrimalViaSubproblemaMestre(TipoSubproblemaSolver_mestre, idEstagio, idCenario, diretorio_pl, custo_geral, custo_individual);

							if (idEstagio == a_estagio_inicial)
								custo_inferior_via_mestre.at(idCenario) = custo_individual;

							if (idEstagio > a_estagio_inicial)
								custo_superior_via_mestre.at(idCenario) -= custo_geral;

							custo_superior_via_mestre.at(idCenario) += custo_individual;
							
						}

						//Probabilidade acumulada do cenário					
						probabilidade_cenario.at(idCenario) *= a_modeloOtimizacao.getProbabilidadeAbertura(idEstagio, idCenario);

						if ((idEstagio < a_estagio_final) || (IdEstagio(a_estagio_final + 1) <= a_modeloOtimizacao.getMaiorId(IdEstagio()))) {
							bool resetar = false;
							if (idCenario == cenario_inicial)
								resetar = true;
							a_modeloOtimizacao.atualizarVariavelEstadoComSolucaoPrimal(resetar, a_idIteracao, a_idProcesso, a_maiorIdProcesso, IdEstagio(idEstagio + 1), idCenario);
						} // if ((idEstagio < a_estagio_final) || (IdEstagio(a_estagio_final + 1) <= a_modeloOtimizacao.getMaiorId(IdEstagio()))) {

						auto stop_clock_cenario = std::chrono::high_resolution_clock::now();

						std::chrono::duration<double> tempo_cenario = stop_clock_cenario - start_clock_cenario;

						const double tempo_otimizacao = a_modeloOtimizacao.getTempoOtimizacao(tSS, idEstagio);

						tempo_medio_otimizacao_solver += tempo_otimizacao;

						if (imprimir_tempos) {
							valores_tempo.at(1).push_back(getString(tempo_cenario.count()));
							valores_tempo_otimizacao.at(1).push_back(getString(tempo_otimizacao));
							valores_tratamento.at(1).push_back(getString(a_modeloOtimizacao.getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, idEstagio, int())));
						} // if (imprimir_tempos) {

					} // try {
					catch (const std::exception& erro) { throw std::invalid_argument("Erro no problema de " + getFullString(idEstagio) + " " + getFullString(idCenario) + ". \n" + std::string(erro.what())); }

				} // for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

				if (idEstagio == a_estagio_inicial)
					a_entradaSaidaDados.setAppendArquivo(false);
				else
					a_entradaSaidaDados.setAppendArquivo(true);

				if (imprimir_tempos) {
					a_entradaSaidaDados.setDiretorioSaida(diretorio_tempos);
					a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(getFullString(a_idProcesso) + "_tempos_cenario.csv", valores_tempo);
					a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(getFullString(a_idProcesso) + "_tempos_otimizacao_cenario.csv", valores_tempo_otimizacao);
					a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(getFullString(a_idProcesso) + "_tratamento_inviabilidade.csv", valores_tratamento);
				} // if (imprimir_tempos) {

				a_entradaSaidaDados.setDiretorioSaida(diretorio_resultado);
				a_modeloOtimizacao.imprimirSolucaoPorEstagioPorCenario_porEstagio(a_idProcesso, "", a_entradaSaidaDados);

			} // for (IdEstagio idEstagio = a_estagio_inicial; idEstagio <= a_estagio_final; idEstagio++) {

			tempo_medio_otimizacao_solver /= double(numero_cenarios * numero_estagios * int(a_maiorIdProcesso));

		} // if (numero_cenarios > 0) {

		for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {
			if (custo_superior_via_mestre.at(idCenario) > custo_superior.at(idCenario))
				custo_superior.at(idCenario) = custo_superior_via_mestre.at(idCenario);
			if (custo_inferior_via_mestre.at(idCenario) > custo_inferior.at(idCenario))
				custo_inferior.at(idCenario) = custo_inferior_via_mestre.at(idCenario);
		}

		executarPDDE_atualizarCustoInferior(a_idIteracao, a_idProcesso, a_maiorIdProcesso, custo_inferior, a_modeloOtimizacao);
		executarPDDE_atualizarCustoSuperior(a_idIteracao, a_idProcesso, a_maiorIdProcesso, custo_superior, probabilidade_cenario, a_modeloOtimizacao);

		a_entradaSaidaDados.setAppendArquivo(false);
		a_entradaSaidaDados.setDiretorioSaida(diretorio_resultado);

		double tempo_solver_fw_global;
		MPI_Allreduce(&tempo_medio_otimizacao_solver, &tempo_solver_fw_global, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

		addElemento(AttVetorMetodoSolucao_tempo_medio_solver_fw, a_idIteracao, tempo_solver_fw_global);

		auto stop_clock = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> tempo = stop_clock - start_clock;

		if (a_idProcesso == IdProcesso_mestre)
			incrementarTempoExecucao(a_idIteracao, tempo.count() / 60.0);

		a_entradaSaidaDados.setDiretorioSaida(diretorio_iteracao);
		executarPDDE_imprimirEstados(a_entradaSaidaDados, a_idIteracao, a_idProcesso, a_modeloOtimizacao);

		a_entradaSaidaDados.setDiretorioSaida(diretorio);

		if ((!a_simulacao) || (!strCompara(a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_diretorio_importacao_pos_estudo, std::string()), "nenhum")))
			executarPDDE_distribuirEstadosEntreProcessos(a_idProcesso, a_maiorIdProcesso, a_idIteracao, a_modeloOtimizacao);

		if (a_idProcesso == IdProcesso_mestre)
			a_modeloOtimizacao.exportarVariaveisEstado_AcoplamentoPosEstudo(a_idProcesso, a_maiorIdProcesso, a_idIteracao, a_entradaSaidaDados);

	} // try {
	catch (const std::exception&erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_forward(a_entradaSaidaDados," + getFullString(a_estagio_inicial) + "," + getFullString(a_estagio_final) + "," + getFullString(a_idIteracao) + "," + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + ",a_modeloOtimizacao): \n" + std::string(erro.what())); }

} // void MetodoSolucao::executarPDDE_forward(const IdEstagio a_estagio_inicial, const IdEstagio a_estagio_final, const IdCenario a_cenario_inicial, const IdCenario a_cenario_final, const IdIteracao a_idIteracao, EntradaSaidaDados a_entradaSaidaDados, const IdProcesso a_idProcesso, ModeloOtimizacao & a_modeloOtimizacao){

void MetodoSolucao::executarPDDE_backward_new(EntradaSaidaDados a_entradaSaidaDados, const IdEstagio a_estagio_inicial, const IdEstagio a_estagio_final, const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, ModeloOtimizacao& a_modeloOtimizacao) {

	try {

		const TipoSubproblemaSolver tSS = TipoSubproblemaSolver_geral;

		const bool imprimir_tempos = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_imprimir_tempos_metodo_solucao, bool());

		auto start_clock = std::chrono::high_resolution_clock::now();

		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida();
		const std::string diretorio_iteracao = diretorio + "//" + getFullString(a_idIteracao) + "//Backward";
		const std::string diretorio_resultado = diretorio_iteracao + "//Resultados";
		const std::string diretorio_pl = diretorio_iteracao + "//PLs";
		const std::string diretorio_selecao_corte = diretorio_iteracao + "//Selecao_Cortes";
		const std::string diretorio_tempos = diretorio_iteracao + "//Tempos";

		const int numero_estagios = int(a_estagio_final - a_estagio_inicial) + 1;

		double tempo_medio_otimizacao_solver = 0.0;
		   int     numero_otimizacoes_solver = 0;

		const double multiplicador_tempo_limite_resolucao = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_multiplicador_tempo_limite_resolucao, double());

		const double tempo_medio_solver_fw = getElementoVetor(AttVetorMetodoSolucao_tempo_medio_solver_fw, a_idIteracao, double());

		const double tempo_limite = multiplicador_tempo_limite_resolucao * tempo_medio_solver_fw;

		const IdCenario cenario_inicial_processo = a_modeloOtimizacao.getCenarioInicial(a_idProcesso, a_idIteracao);
		const IdCenario cenario_final_processo = a_modeloOtimizacao.getCenarioFinal(a_idProcesso, a_idIteracao);
		const int numero_cenarios_processo = a_modeloOtimizacao.getNumeroCenarios(cenario_inicial_processo, cenario_final_processo);

		a_modeloOtimizacao.importarCorteBenders_AcoplamentoPosEstudo(tSS, a_idProcesso, a_idIteracao, diretorio_selecao_corte, a_entradaSaidaDados);

		executarPDDE_distribuirRealizacoesEntreProcessos(a_idProcesso, a_maiorIdProcesso, a_idIteracao, a_modeloOtimizacao);

		for (IdEstagio idEstagio = a_estagio_final; idEstagio >= a_estagio_inicial; idEstagio--) {

			const TipoProcessamentoParalelo tipo_processamento_paralelo = a_modeloOtimizacao.getElementoVetor(a_idIteracao, AttVetorIteracao_tipo_processamento_paralelo, idEstagio, TipoProcessamentoParalelo());

			const IdAbertura abertura_final_estagio = IdAbertura(a_modeloOtimizacao.getAtributo(idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()));

			const int numero_aberturas_estagio = int(a_modeloOtimizacao.getAtributo(idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()));

			int numero_aberturas_efetivas = numero_aberturas_estagio;
			const int cortes_multiplos = a_modeloOtimizacao.getElementoVetor(AttVetorModeloOtimizacao_cortes_multiplos, idEstagio, int());
			if (cortes_multiplos > 0)
				numero_aberturas_efetivas = cortes_multiplos;

			const int menor_numero_aberturas_estagio = a_modeloOtimizacao.getMenorNumeroAberturasEstagio(a_maiorIdProcesso, idEstagio, a_idIteracao);
			const int maior_numero_aberturas_estagio = a_modeloOtimizacao.getMaiorNumeroAberturasEstagio(a_maiorIdProcesso, idEstagio, a_idIteracao);
			const int maior_numero_aberturas_processo_estagio = a_modeloOtimizacao.getMaiorNumeroAberturasProcessoEstagio(a_idProcesso, idEstagio, a_idIteracao);

			const int numero_estados = int(a_modeloOtimizacao.getMaiorId(idEstagio, IdVariavelEstado()));
			const int numero_total_cenarios = a_modeloOtimizacao.getNumeroTotalCenariosEmEstados(a_idProcesso, a_maiorIdProcesso, idEstagio, a_idIteracao);
			const int numero_variaveis_dinamicas = a_modeloOtimizacao.getNumeroVariavelDinamica(tSS, idEstagio);

			const int numero_processos = int(a_maiorIdProcesso);

			EstruturaResultados<double> soma_custo_abertura(1);
			soma_custo_abertura.setSize(0, numero_aberturas_estagio);
			for (int a = 0; a < numero_aberturas_estagio; a++)
				soma_custo_abertura.vetor[a] = 0.0;

			EstruturaResultados<int> numero_soma_custo_abertura(1);
			numero_soma_custo_abertura.setSize(0, numero_aberturas_estagio);
			for (int a = 0; a < numero_aberturas_estagio; a++)
				numero_soma_custo_abertura.vetor[a] = 0;

			EstruturaResultados<int> status_otimizacao(1);
			status_otimizacao.setSize(0, numero_total_cenarios, maior_numero_aberturas_processo_estagio);

			EstruturaResultados<double> custo_total(1);
			custo_total.setSize(0, numero_total_cenarios, maior_numero_aberturas_processo_estagio);

			EstruturaResultados<double> sol_inf_var_dinamica(1);
			sol_inf_var_dinamica.setSize(0, numero_total_cenarios, maior_numero_aberturas_processo_estagio);

			EstruturaResultados<double> solucao_dual_var_dinamica(1);
			solucao_dual_var_dinamica.setSize(0, numero_total_cenarios, maior_numero_aberturas_processo_estagio, numero_variaveis_dinamicas);

			EstruturaResultados<double> limite_inferior_var_dinamica(1);
			limite_inferior_var_dinamica.setSize(0, numero_total_cenarios, maior_numero_aberturas_processo_estagio, numero_variaveis_dinamicas);

			EstruturaResultados<double> limite_superior_var_dinamica(1);
			limite_superior_var_dinamica.setSize(0, numero_total_cenarios, maior_numero_aberturas_processo_estagio, numero_variaveis_dinamicas);

			EstruturaResultados<double> vlr_var_estado(1);
			vlr_var_estado.setSize(0, numero_total_cenarios, numero_estados);

			EstruturaResultados<double> sol_dual_var_estado(1);
			sol_dual_var_estado.setSize(0, numero_total_cenarios, maior_numero_aberturas_processo_estagio, numero_estados);

			// Identificação das aberturas com solução proxy a partir do custo médio do estágio
			if (true) {
				SmartEnupla<IdRealizacao, int> realizacao_solucao_proxy(IdRealizacao_1, std::vector<int>(a_modeloOtimizacao.getAtributo(idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()), 0));
				const std::vector<int> odernacao_custo_medio = getOrdenacaoMaiorParaMenor(a_modeloOtimizacao.getElementosMatriz(AttMatrizModeloOtimizacao_custo_medio, idEstagio, IdRealizacao(), double()).getElementos());
				for (int i = int(odernacao_custo_medio.size()); i > 0; i--) {
					if (a_modeloOtimizacao.getElementoVetor(idEstagio, AttVetorEstagio_selecao_solucao_proxy, i, bool()))
						realizacao_solucao_proxy.setElemento(IdRealizacao(odernacao_custo_medio.at(i - 1) + 1), 1);
				}
				a_modeloOtimizacao.setElementos(AttMatrizModeloOtimizacao_realizacao_solucao_proxy, idEstagio, realizacao_solucao_proxy);
			} // if (true) {

			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) {

				if ((a_modeloOtimizacao.getSize2Matriz(idEstagio, IdVariavelEstado_1, AttMatrizVariavelEstado_valor, idProcesso) > 0) && ((idProcesso == a_idProcesso) || (tipo_processamento_paralelo == TipoProcessamentoParalelo_por_abertura))) {

					const IdCenario cenario_inicial = a_modeloOtimizacao.getCenarioInicial(idProcesso, a_idIteracao);
					const IdCenario cenario_final = a_modeloOtimizacao.getCenarioFinal(idProcesso, a_idIteracao);
					const int numero_cenarios = a_modeloOtimizacao.getNumeroCenarios(cenario_inicial, cenario_final);

					if (numero_cenarios > 0) {

						if (tempo_limite > 0)
							a_modeloOtimizacao.setTempoLimiteOtimizacao(tSS, idEstagio, tempo_limite);

						for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

							try {

								IdAbertura abertura_inicial_cenario = IdAbertura_Nenhum;
								IdAbertura abertura_final_cenario = IdAbertura_Nenhum;

								if (menor_numero_aberturas_estagio < numero_aberturas_efetivas) {
									abertura_inicial_cenario = a_modeloOtimizacao.getElementoMatriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_mapeamento_abertura_inicial_por_cenario_por_estagio, a_modeloOtimizacao.getCenarioInicial(a_idProcesso, a_idIteracao), idEstagio, IdAbertura());
									abertura_final_cenario = a_modeloOtimizacao.getElementoMatriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_mapeamento_abertura_final_por_cenario_por_estagio, a_modeloOtimizacao.getCenarioInicial(a_idProcesso, a_idIteracao), idEstagio, IdAbertura());
								} // if (menor_numero_aberturas_estagio < numero_aberturas_efetivas) {

								else if (menor_numero_aberturas_estagio == numero_aberturas_efetivas) {
									abertura_inicial_cenario = a_modeloOtimizacao.getElementoMatriz(a_idIteracao, idProcesso, AttMatrizProcesso_mapeamento_abertura_inicial_por_cenario_por_estagio, idCenario, idEstagio, IdAbertura());
									abertura_final_cenario = a_modeloOtimizacao.getElementoMatriz(a_idIteracao, idProcesso, AttMatrizProcesso_mapeamento_abertura_final_por_cenario_por_estagio, idCenario, idEstagio, IdAbertura());
								} // else if (menor_numero_aberturas_estagio == numero_aberturas_efetivas) {

								int numero_aberturas_cenario = 0;
								if ((abertura_inicial_cenario != IdAbertura_Nenhum) && (abertura_final_cenario != IdAbertura_Nenhum))
									numero_aberturas_cenario = int(abertura_final_cenario - abertura_inicial_cenario) + 1;

								if ((idEstagio == a_estagio_final) && (idCenario == cenario_inicial))
									a_entradaSaidaDados.setAppendArquivo(false);

								else
									a_entradaSaidaDados.setAppendArquivo(true);

								std::vector<std::vector<std::string>> valores_tempo(2, std::vector<std::string>());
								std::vector<std::vector<std::string>> valores_tempo_otimizacao(2, std::vector<std::string>());
								std::vector<std::vector<std::string>> valores_tratamento(2, std::vector<std::string>());

								if (imprimir_tempos) {
									valores_tempo.at(1).reserve(int(numero_aberturas_cenario) + 1);
									valores_tempo.at(1).push_back(getFullString(idEstagio) + ";" + getFullString(a_idProcesso) + ";" + getFullString(idCenario) + ";");

									valores_tempo_otimizacao.at(1).reserve(int(numero_aberturas_cenario) + 1);
									valores_tempo_otimizacao.at(1).push_back(getFullString(idEstagio) + ";" + getFullString(a_idProcesso) + ";" + getFullString(idCenario) + ";");

									valores_tratamento.at(1).reserve(int(numero_aberturas_cenario));
									valores_tratamento.at(1).push_back(getFullString(idEstagio) + ";" + getFullString(a_idProcesso) + ";" + getFullString(idCenario) + ";");
								} // if (imprimir_tempos) {

								auto start_clock_cenario = std::chrono::high_resolution_clock::now();

								if (numero_aberturas_cenario > 0)
									a_modeloOtimizacao.atualizarModeloOtimizacaoComVariavelEstado(idEstagio, idProcesso, a_maiorIdProcesso, idCenario, vlr_var_estado);

								for (int a = 0; a < numero_aberturas_cenario; a++) {

									const IdAbertura idAbertura = IdAbertura(abertura_inicial_cenario + a);

									auto start_clock_realizacao = std::chrono::high_resolution_clock::now();

									a_modeloOtimizacao.atualizarModeloOtimizacaoComVariavelRealizacao(idEstagio, IdRealizacao(idAbertura));

									const bool otimizacao = a_modeloOtimizacao.otimizarProblema(tSS, idProcesso, a_idIteracao, idEstagio, idCenario, IdRealizacao(idAbertura), sol_inf_var_dinamica, solucao_dual_var_dinamica, limite_inferior_var_dinamica, limite_superior_var_dinamica, sol_dual_var_estado, diretorio_pl);

									const double custo_total_otimizacao = a_modeloOtimizacao.getCustoTotal(idEstagio);

									soma_custo_abertura.vetor[int(idAbertura) - 1] += custo_total_otimizacao;
									numero_soma_custo_abertura.vetor[int(idAbertura) - 1]++;

									const double tempo_otimizacao = a_modeloOtimizacao.getTempoOtimizacao(tSS, idEstagio);

									custo_total.add(custo_total_otimizacao);

									if (otimizacao) {
										tempo_medio_otimizacao_solver += tempo_otimizacao;
										numero_otimizacoes_solver++;
										status_otimizacao.add(1);
									}
									else
										status_otimizacao.add(0);

									if (imprimir_tempos) {
										auto stop_clock_realizacao = std::chrono::high_resolution_clock::now();
										std::chrono::duration<double> tempo_realizacao = stop_clock_realizacao - start_clock_realizacao;

										valores_tempo.at(1).push_back(getString(tempo_realizacao.count()));
										valores_tempo_otimizacao.at(1).push_back(getString(tempo_otimizacao));
										valores_tratamento.at(1).push_back(getString(a_modeloOtimizacao.getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, idEstagio, int())));
									} // if (imprimir_tempos) {

								} // for (int a = 0; a < numero_aberturas_processo; a++) {

								if (imprimir_tempos) {

									auto stop_clock_cenario = std::chrono::high_resolution_clock::now();

									std::chrono::duration<double> tempo_cenario = stop_clock_cenario - start_clock_cenario;

									valores_tempo.at(1).push_back(";" + getString(tempo_cenario.count()));

									a_entradaSaidaDados.setDiretorioSaida(diretorio_tempos);
									a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(getFullString(a_idProcesso) + "_tempos_cenario.csv", valores_tempo);
									a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(getFullString(a_idProcesso) + "_tempos_otimizacao_cenario.csv", valores_tempo_otimizacao);
									a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(getFullString(a_idProcesso) + "_tratamento_inviabilidade.csv", valores_tratamento);

								} // if (imprimir_tempos) {

							} // try {
							catch (const std::exception& erro) { throw std::invalid_argument("Erro no problema de " + getFullString(idEstagio) + " " + getFullString(idCenario) + ". \n" + std::string(erro.what())); }

						} // for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {
					} // if (numero_cenarios > 0)
				} // if (a_modeloOtimizacao.getSize2Matriz(idEstagio, IdVariavelEstado_1, AttMatrizVariavelEstado_valor, idProcesso) > 0) {
			} // for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) {

			auto start_clock_corte = std::chrono::high_resolution_clock::now();

			if ((idEstagio > IdEstagio_1) || (a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_calcular_custo_primal_via_subproblema_mestre, bool())))
				executarPDDE_calcularCorteBenders_new(tSS, a_idIteracao, a_idProcesso, a_maiorIdProcesso, idEstagio, status_otimizacao, custo_total, sol_inf_var_dinamica, solucao_dual_var_dinamica, limite_inferior_var_dinamica, limite_superior_var_dinamica, sol_dual_var_estado, vlr_var_estado, a_modeloOtimizacao);

			auto stop_clock_corte = std::chrono::high_resolution_clock::now();

			std::vector<std::vector<std::string>> valores_tempo(2, std::vector<std::string>());

			if (imprimir_tempos) {

				std::chrono::duration<double> tempo_corte = stop_clock_corte - start_clock_corte;
				valores_tempo.at(0) = std::vector<std::string>{ getFullString(idEstagio) + ";" + getFullString(a_idProcesso) + ";CorteBenders;" + getString(tempo_corte.count()) + ";" };

			} // if (imprimir_tempos) {

			a_entradaSaidaDados.setAppendArquivo(false);
			executarPDDE_requestCorteBenders(a_idProcesso, a_maiorIdProcesso, idEstagio, a_modeloOtimizacao, diretorio_selecao_corte, a_entradaSaidaDados);

			if (imprimir_tempos) {
				auto stop_clock_request = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> tempo_request = stop_clock_request - stop_clock_corte;
				valores_tempo.at(1) = std::vector<std::string>{ getFullString(idEstagio) + ";" + getFullString(a_idProcesso) + ";RequestCorteBenders;" + getString(tempo_request.count()) + ";" };
			} // if (imprimir_tempos) {

			EstruturaResultados<double> soma_custo_abertura_all(1);
			soma_custo_abertura_all.setSize(0, numero_aberturas_estagio);
			MPI_Allreduce(soma_custo_abertura.vetor, soma_custo_abertura_all.vetor, soma_custo_abertura.total, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

			EstruturaResultados<int> numero_soma_custo_abertura_all(1);
			numero_soma_custo_abertura_all.setSize(0, numero_aberturas_estagio);
			MPI_Allreduce(numero_soma_custo_abertura.vetor, numero_soma_custo_abertura_all.vetor, numero_soma_custo_abertura.total, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

			for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= IdRealizacao(numero_aberturas_estagio); idRealizacao++)
				a_modeloOtimizacao.setElemento(AttMatrizModeloOtimizacao_custo_medio, idEstagio, idRealizacao, soma_custo_abertura_all.vetor[int(idRealizacao) - 1] / double(numero_soma_custo_abertura_all.vetor[int(idRealizacao) - 1]));

			if (idEstagio == a_estagio_final)
				a_entradaSaidaDados.setAppendArquivo(false);
			else
				a_entradaSaidaDados.setAppendArquivo(true);

			if (imprimir_tempos) {
				a_entradaSaidaDados.setDiretorioSaida(diretorio_tempos);
				a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(getFullString(a_idProcesso) + "_tempos_BW_calculo_corte.csv", valores_tempo);
				a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(getFullString(a_idProcesso) + "_tempos_BW_request_corte.csv", valores_tempo);
			} // if (imprimir_tempos) {

			if (idEstagio == a_estagio_final)
				a_entradaSaidaDados.setAppendArquivo(false);
			else
				a_entradaSaidaDados.setAppendArquivo(true);

			a_entradaSaidaDados.setDiretorioSaida(diretorio_resultado);
			a_modeloOtimizacao.imprimirSolucaoPorEstagioPorCenarioPorRealizacao_porEstagio(a_idProcesso, "", a_entradaSaidaDados);

		} // for (IdEstagio idEstagio = a_estagio_final; idEstagio >= a_estagio_inicial; idEstagio--) {

		for (IdEstagio idEstagio = a_estagio_final; idEstagio >= a_estagio_inicial; idEstagio--) {

			if (a_idProcesso == IdProcesso_mestre) {
				if ((a_idIteracao == a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_iteracao_final, IdIteracao())) || (a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_imprimir_corte_por_iteracao, bool()))) {
					a_entradaSaidaDados.setAppendArquivo(false);
					a_entradaSaidaDados.setDiretorioSaida(diretorio_iteracao);
					a_modeloOtimizacao.exportarCorteBenders(a_idProcesso, idEstagio, a_entradaSaidaDados);
				}
				if (idEstagio == a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_estagio_acoplamento_pre_estudo, IdEstagio())) {
					a_entradaSaidaDados.setAppendArquivo(false);
					a_entradaSaidaDados.setDiretorioSaida(diretorio);
					a_modeloOtimizacao.exportarCorteBenders_AcoplamentoPreEstudo(a_idIteracao, a_entradaSaidaDados);
				}

				if (a_idIteracao == a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_iteracao_final, IdIteracao())) {
					a_entradaSaidaDados.setAppendArquivo(false);
					a_entradaSaidaDados.setDiretorioSaida(diretorio_iteracao);
					a_modeloOtimizacao.exportarVersaoAlternativaCorteBenders(a_idProcesso, idEstagio, a_entradaSaidaDados);
				}

			} // if (a_idProcesso == IdProcesso_mestre) {

			if (a_idIteracao == a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_iteracao_final, IdIteracao()))
				a_modeloOtimizacao.removerCorteBenders(idEstagio);

		} // for (IdEstagio idEstagio = a_estagio_final; idEstagio >= a_estagio_inicial; idEstagio--) {

		tempo_medio_otimizacao_solver /= double(numero_otimizacoes_solver);

		double tempo_solver_bw_global;
		MPI_Allreduce(&tempo_medio_otimizacao_solver, &tempo_solver_bw_global, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

		addElemento(AttVetorMetodoSolucao_tempo_medio_solver_bw, a_idIteracao, tempo_solver_bw_global / int(a_maiorIdProcesso));

		a_entradaSaidaDados.setAppendArquivo(false);
		a_entradaSaidaDados.setDiretorioSaida(diretorio_iteracao);

		a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz(getFullString(a_idProcesso) + "_" + getString(AttMatrizModeloOtimizacao_custo_medio) + "_" + getFullString(a_idIteracao) + ".csv", a_modeloOtimizacao, AttMatrizModeloOtimizacao_custo_medio);
		a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz(getFullString(a_idProcesso) + "_" + getString(AttMatrizModeloOtimizacao_realizacao_solucao_proxy) + "_" + getFullString(a_idIteracao) + ".csv", a_modeloOtimizacao, AttMatrizModeloOtimizacao_realizacao_solucao_proxy);

		auto stop_clock = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> tempo = stop_clock - start_clock;

		if (a_idProcesso == IdProcesso_mestre)
			incrementarTempoExecucao(a_idIteracao, tempo.count() / 60);

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_backward(a_entradaSaidaDados," + getFullString(a_estagio_inicial) + "," + getFullString(a_estagio_final) + "," + getFullString(a_idIteracao) + "," + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + ",a_modeloOtimizacao): \n" + std::string(erro.what())); }

} // void MetodoSolucao::executarPDDE_backward(const IdEstagio a_estagio_inicial, const IdEstagio a_estagio_final, const IdCenario a_cenario_inicial, const IdCenario a_cenario_final, const IdIteracao a_idIteracao, EntradaSaidaDados a_entradaSaidaDados, const IdProcesso a_idProcesso, ModeloOtimizacao & a_modeloOtimizacao){


void MetodoSolucao::executarPDDE_atualizarCustoSuperior(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const SmartEnupla<IdCenario, double>& a_custo_superior, const SmartEnupla<IdCenario, double>& a_probabilidade_cenario, ModeloOtimizacao &a_modeloOtimizacao) {
	
	try {

		if (a_idProcesso == IdProcesso_mestre) {

			if (getSizeMatriz(AttMatrizMetodoSolucao_custo_superior) == 0)
				setMatriz_forced(AttMatrizMetodoSolucao_custo_superior, SmartEnupla<IdIteracao, SmartEnupla<IdCenario, double>>(a_idIteracao, std::vector<SmartEnupla<IdCenario, double>>(1, SmartEnupla<IdCenario, double>(IdCenario_1, std::vector<double>(IdCenario(a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_numero_cenarios, int())), NAN)))));

			if (getSizeMatriz(AttMatrizMetodoSolucao_probabilidade_cenario) == 0)
				setMatriz_forced(AttMatrizMetodoSolucao_probabilidade_cenario, SmartEnupla<IdIteracao, SmartEnupla<IdCenario, double>>(a_idIteracao, std::vector<SmartEnupla<IdCenario, double>>(1, SmartEnupla<IdCenario, double>(IdCenario_1, std::vector<double>(IdCenario(a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_numero_cenarios, int())), NAN)))));

			if (a_custo_superior.size() != a_probabilidade_cenario.size())
				throw std::invalid_argument("Os vetores de custo e probabilidade devem possuir as mesmas dimensoes.");

			if (a_custo_superior.size() > 0) {

				if (a_custo_superior.getIteradorInicial() != a_probabilidade_cenario.getIteradorInicial())
					throw std::invalid_argument("Os vetores de custo e probabilidade devem possuir os mesmos cenarios.");

				for (IdCenario idCenario = a_custo_superior.getIteradorInicial(); idCenario <= a_custo_superior.getIteradorFinal(); idCenario++) {
					setElemento(AttMatrizMetodoSolucao_custo_superior,        a_idIteracao, idCenario, a_custo_superior.at(idCenario));
					setElemento(AttMatrizMetodoSolucao_probabilidade_cenario, a_idIteracao, idCenario, a_probabilidade_cenario.at(idCenario));
				}

			} // if (a_custo_superior.size() > 0) {

			for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= a_maiorIdProcesso; idProcesso++) {

				int* numero_cenarios_cenario_inicial_outro = new int[2];

				MPI_Recv(numero_cenarios_cenario_inicial_outro, 2, MPI_INT, getRank(idProcesso), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

				if (numero_cenarios_cenario_inicial_outro[0] > 0) {

					double* custo_superior_outro        = new double[numero_cenarios_cenario_inicial_outro[0]];
					double* probabilidade_cenario_outro = new double[numero_cenarios_cenario_inicial_outro[0]];

					MPI_Recv(custo_superior_outro,        numero_cenarios_cenario_inicial_outro[0], MPI_DOUBLE, getRank(idProcesso), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					MPI_Recv(probabilidade_cenario_outro, numero_cenarios_cenario_inicial_outro[0], MPI_DOUBLE, getRank(idProcesso), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

					for (int i = 0; i < numero_cenarios_cenario_inicial_outro[0]; i++) {
						setElemento(AttMatrizMetodoSolucao_custo_superior,        a_idIteracao, IdCenario(i + numero_cenarios_cenario_inicial_outro[1]),        custo_superior_outro[i]);
						setElemento(AttMatrizMetodoSolucao_probabilidade_cenario, a_idIteracao, IdCenario(i + numero_cenarios_cenario_inicial_outro[1]), probabilidade_cenario_outro[i]);
					} // for (int i = 0; i < numero_cenarios_cenario_inicial_outro[0]; i++) {

					delete[] custo_superior_outro;
					delete[] probabilidade_cenario_outro;

				} // if (numero_cenarios_cenario_inicial_outro[0] > 0) {

				delete[] numero_cenarios_cenario_inicial_outro;

			} // for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= a_maiorIdProcesso; idProcesso++) {

		} // if (a_idProcesso == IdProcesso_mestre) {

		else {

			if (a_custo_superior.size() != a_probabilidade_cenario.size())
				throw std::invalid_argument("Os vetores de custo e probabilidade devem possuir as mesmas dimensoes.");

			int* numero_cenarios_cenario_inicial = new int[2];

			const int numero_cenarios = a_custo_superior.size();

			numero_cenarios_cenario_inicial[0] = numero_cenarios;

			if (numero_cenarios > 0)
				numero_cenarios_cenario_inicial[1] = int(a_custo_superior.getIteradorInicial());

			MPI_Send(numero_cenarios_cenario_inicial, 2, MPI_INT, getRank(IdProcesso_mestre), 0, MPI_COMM_WORLD);

			delete[] numero_cenarios_cenario_inicial;

			if (numero_cenarios > 0) {

				if (a_custo_superior.getIteradorInicial() != a_probabilidade_cenario.getIteradorInicial())
					throw std::invalid_argument("Os vetores de custo e probabilidade devem possuir os mesmos cenarios.");

				MPI_Send(&a_custo_superior.getElementos()[0],        numero_cenarios, MPI_DOUBLE, getRank(IdProcesso_mestre), 0, MPI_COMM_WORLD);
				MPI_Send(&a_probabilidade_cenario.getElementos()[0], numero_cenarios, MPI_DOUBLE, getRank(IdProcesso_mestre), 0, MPI_COMM_WORLD);

			} // if (numero_cenarios > 0) {

		} // else {

	} // try {
	catch (const std::exception & erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_atualizarCustoInferior(" + getFullString(a_idIteracao) + "," + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + ",a_custo_superior,a_probabilidade_cenario,a_modeloOtimizacao): \n" + std::string(erro.what())); }

} // void MetodoSolucao::executarPDDE_atualizarCustoSuperior(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso) {


void MetodoSolucao::executarPDDE_atualizarCustoInferior(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const SmartEnupla<IdCenario, double> &a_custo_inferior, ModeloOtimizacao& a_modeloOtimizacao){

	try{

		if (a_idProcesso == IdProcesso_mestre) {   

			if (getSizeMatriz(AttMatrizMetodoSolucao_custo_inferior) == 0)
				setMatriz_forced(AttMatrizMetodoSolucao_custo_inferior, SmartEnupla<IdIteracao, SmartEnupla<IdCenario, double>>(a_idIteracao, std::vector<SmartEnupla<IdCenario, double>>(1, SmartEnupla<IdCenario, double>(IdCenario_1, std::vector<double>(IdCenario(a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_numero_cenarios, int())), NAN)))));
		
			if (a_custo_inferior.size() > 0) {

				for (IdCenario idCenario = a_custo_inferior.getIteradorInicial(); idCenario <= a_custo_inferior.getIteradorFinal(); idCenario++)
					setElemento(AttMatrizMetodoSolucao_custo_inferior, a_idIteracao, idCenario, a_custo_inferior.at(idCenario));

			} // if (a_custo_inferior.size() > 0) {

			for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= a_maiorIdProcesso; idProcesso++) {

				int* numero_cenarios_cenario_inicial_outro = new int[2];

				MPI_Recv(numero_cenarios_cenario_inicial_outro, 2, MPI_INT, getRank(idProcesso), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

				if (numero_cenarios_cenario_inicial_outro[0] > 0) {

					double* custo_inferior_outro = new double[numero_cenarios_cenario_inicial_outro[0]];

					MPI_Recv(custo_inferior_outro, numero_cenarios_cenario_inicial_outro[0], MPI_DOUBLE, getRank(idProcesso), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

					for (int i = 0; i < numero_cenarios_cenario_inicial_outro[0]; i++)
						setElemento(AttMatrizMetodoSolucao_custo_inferior, a_idIteracao, IdCenario(i + numero_cenarios_cenario_inicial_outro[1]), custo_inferior_outro[i]);

					delete[] custo_inferior_outro;

				} // if (numero_cenarios_cenario_inicial_outro[0] > 0) {

				delete[] numero_cenarios_cenario_inicial_outro;

			} // for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= a_maiorIdProcesso; idProcesso++) {

		} // if (a_idProcesso == IdProcesso_mestre) {

		else {

			int* numero_cenarios_cenario_inicial = new int[2];

			const int numero_cenarios = a_custo_inferior.size();

			numero_cenarios_cenario_inicial[0] = numero_cenarios;

			if (numero_cenarios > 0)
				numero_cenarios_cenario_inicial[1] = int(a_custo_inferior.getIteradorInicial());

			MPI_Send(numero_cenarios_cenario_inicial, 2, MPI_INT, getRank(IdProcesso_mestre), 0, MPI_COMM_WORLD);

			delete[] numero_cenarios_cenario_inicial;

			if (numero_cenarios > 0)
				MPI_Send(&a_custo_inferior.getElementos()[0], numero_cenarios, MPI_DOUBLE, getRank(IdProcesso_mestre), 0, MPI_COMM_WORLD);

		} // else {

	} // try {
	catch (const std::exception&erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_atualizarCustoInferior(" + getFullString(a_idIteracao) + "," + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + ",a_custo_inferior,a_modeloOtimizacao): \n" + std::string(erro.what())); }

} // void MetodoSolucao::executarPDDE_atualizarCustoInferior(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdRealizacao a_idRealizacao, const IdRealizacao a_maiorIdRealizacao, double a_custo_inferior){



void MetodoSolucao::executarPDDE_avaliarSolucao(EntradaSaidaDados a_entradaSaidaDados, const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, ModeloOtimizacao &a_modeloOtimizacao){

	try {

		if (a_idProcesso == IdProcesso_mestre) {

			const double tempo_execucao       = getElementoVetor(AttVetorMetodoSolucao_tempo_execucao, a_idIteracao, double());

			const double tempo_execucao_total = getAtributo(AttComumMetodoSolucao_tempo_execucao_total, double());
			addElemento(AttVetorMetodoSolucao_tempo_execucao_acumulado, a_idIteracao, tempo_execucao_total);

			const double custo_inferior = getMedia_noNAN(getMatriz(AttMatrizMetodoSolucao_custo_inferior, IdIteracao(),  IdCenario(), double()).at(a_idIteracao));
			addElemento(AttVetorMetodoSolucao_custo_inferior, a_idIteracao, custo_inferior);

			double custo_superior = 0.0;

			const double soma_probabilidades = getSoma_noNAN(getMatriz(AttMatrizMetodoSolucao_probabilidade_cenario, IdIteracao(), IdCenario(), double()).at(a_idIteracao));

			if (!doubleCompara(soma_probabilidades, 1.0))
				custo_superior = getMedia_noNAN(getMatriz(AttMatrizMetodoSolucao_custo_superior,  IdIteracao(), IdCenario(), double()).at(a_idIteracao));
			
			else {
				const IdCenario cenario_final = getIterador2Final(AttMatrizMetodoSolucao_custo_superior, a_idIteracao, IdCenario());
				for (IdCenario idCenario = IdCenario_1; idCenario <= cenario_final; idCenario++)
					custo_superior += getElementoMatriz(AttMatrizMetodoSolucao_custo_superior, a_idIteracao, idCenario, double()) * getElementoMatriz(AttMatrizMetodoSolucao_probabilidade_cenario, a_idIteracao, idCenario, double());
			}
			
			addElemento(AttVetorMetodoSolucao_custo_superior, a_idIteracao, custo_superior);

			const double brecha = 100.0*(custo_superior - custo_inferior) / custo_superior;


			//
			// Impressão na Tela
			//

			std::cout << std::endl;
			std::cout << "---------------------------------------------------------" << std::endl;
			std::cout << "Avaliacao da Solucao da PDDE" << std::endl;
			std::cout << "Modelo: " << getString(a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_idModeloOtimizacao, IdModeloOtimizacao())) << std::endl;
			std::cout << std::endl;

			if (a_idIteracao == IdIteracao_0) {
				std::cout << "Simulacao Inicial: " << std::endl;
				std::cout << std::endl;
				double tempo_solver_fw = getElementoVetor(AttVetorMetodoSolucao_tempo_medio_solver_fw, a_idIteracao, double());
				std::cout << "Tempo Medio Solver no Forward  (s): " + getString(tempo_solver_fw) << std::endl;

			}

			else if (a_idIteracao == IdIteracao(a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_iteracao_final, IdIteracao()) + 1)) {
				std::cout << "Simulacao Final: " << std::endl;
				std::cout << std::endl;
				double tempo_solver_fw = getElementoVetor(AttVetorMetodoSolucao_tempo_medio_solver_fw, a_idIteracao, double());
				std::cout << "Tempo Medio Solver no Forward  (s): " + getString(tempo_solver_fw) << std::endl;
			}

			else {
				std::cout << "Iteracao: " + getString(a_idIteracao) << std::endl;
				std::cout << std::endl;
				double tempo_solver_fw = getElementoVetor(AttVetorMetodoSolucao_tempo_medio_solver_fw, a_idIteracao, double());
				double tempo_solver_bw = getElementoVetor(AttVetorMetodoSolucao_tempo_medio_solver_bw, a_idIteracao, double());
				std::cout << "Tempo Medio Solver no Forward  (s): " + getString(tempo_solver_fw) << std::endl;
				std::cout << "Tempo Medio Solver no Backward (s): " + getString(tempo_solver_bw) << std::endl;
			}

			std::cout << std::endl;
			std::cout << "Custo Superior (milhoes): " + getString(custo_superior / 1000000) << std::endl;
			std::cout << "Custo Inferior (milhoes): " + getString(custo_inferior / 1000000) << std::endl;
			std::cout << std::endl;
			std::cout << "Brecha (%): " + getString(brecha) << std::endl;
			std::cout << std::endl;
			std::cout << std::endl;
			std::cout << "Tempo Execucao Iteracao  (minutos): " + getString(tempo_execucao) << std::endl;
			std::cout << "Tempo Execucao Total     (minutos): " + getString(tempo_execucao_total) << std::endl;
			std::cout << "---------------------------------------------------------" << std::endl;

			//
			// Impressão em Arquivo
			//
			a_entradaSaidaDados.setAppendArquivo(false);
			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("resultado_por_iteracao.csv", *this, std::vector<AttVetorMetodoSolucao>{AttVetorMetodoSolucao_tempo_execucao, AttVetorMetodoSolucao_custo_inferior, AttVetorMetodoSolucao_custo_superior});

			if (a_idIteracao > a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_iteracao_inicial, IdIteracao()))
				a_entradaSaidaDados.setAppendArquivo(true);

			a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("custos_inferior_e_superior.csv", *this, AttMatrizMetodoSolucao_custo_inferior);
			a_entradaSaidaDados.setAppendArquivo(true);
			a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("custos_inferior_e_superior.csv", *this, AttMatrizMetodoSolucao_custo_superior);

			//
			// Esvaziamento de Matrizes
			//

			setMatriz(AttMatrizMetodoSolucao_custo_inferior, SmartEnupla<IdIteracao, SmartEnupla<IdCenario, double>>());
			setMatriz(AttMatrizMetodoSolucao_custo_superior, SmartEnupla<IdIteracao, SmartEnupla<IdCenario, double>>());
			setMatriz(AttMatrizMetodoSolucao_probabilidade_cenario, SmartEnupla<IdIteracao, SmartEnupla<IdCenario, double>>());

		} // if (a_idProcesso == IdProcesso_mestre) {

	} // try {
	catch (const std::exception&erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_avaliarSolucao(" + getFullString(a_idIteracao) + "," + getFullString(a_idProcesso) + ",a_modeloOtimizacao): \n" + std::string(erro.what())); }

} // void MetodoSolucao::executarPDDE_avaliarSolucao(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, IdModeloOtimizacao a_idModeloOtimizacao){


void MetodoSolucao::executarPDDE_calcularCorteBenders_new(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, EstruturaResultados<int>& a_status_otimizacao, EstruturaResultados<double>& a_custo_total, EstruturaResultados<double>& a_sol_inf_var_dinamica, EstruturaResultados<double>& a_solucao_dual_var_dinamica, EstruturaResultados<double>& a_limite_inferior_var_dinamica, EstruturaResultados<double>& a_limite_superior_var_dinamica, EstruturaResultados<double>& a_sol_dual_var_estado, EstruturaResultados<double>& a_vlr_var_estado, ModeloOtimizacao& a_modeloOtimizacao){
	try{

		bool busca_abertura_em_todos_processos = false;
		if (a_modeloOtimizacao.getElementoVetor(a_idIteracao, AttVetorIteracao_tipo_processamento_paralelo, a_idEstagio, TipoProcessamentoParalelo()) == TipoProcessamentoParalelo_por_abertura)
			busca_abertura_em_todos_processos = true;		

		executarPDDE_sincronizarResultadosParaCorteBenders(a_TSS, a_idIteracao, a_idProcesso, a_maiorIdProcesso, a_idEstagio, a_status_otimizacao, a_custo_total, a_sol_inf_var_dinamica, a_solucao_dual_var_dinamica, a_limite_inferior_var_dinamica, a_limite_superior_var_dinamica, a_sol_dual_var_estado, a_vlr_var_estado, a_modeloOtimizacao, busca_abertura_em_todos_processos);

		EstruturaResultados<int> map_solucao_dual_proxy;

		executarPDDE_mapearSolucaoProxy(a_status_otimizacao, a_custo_total, a_sol_inf_var_dinamica, a_solucao_dual_var_dinamica, a_limite_inferior_var_dinamica, a_limite_superior_var_dinamica, a_sol_dual_var_estado, map_solucao_dual_proxy, a_modeloOtimizacao, busca_abertura_em_todos_processos);

		if (a_modeloOtimizacao.getElementoVetor(AttVetorModeloOtimizacao_cortes_multiplos, a_idEstagio, int()) == 0) {

			const TipoAversaoRisco tipo_aversao_a_risco = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_tipo_aversao_a_risco, TipoAversaoRisco());

			if (tipo_aversao_a_risco == TipoAversaoRisco_valor_esperado)
				executarPDDE_calcularCorteBendersValorEsperado_new(a_idIteracao, a_idProcesso, a_maiorIdProcesso, a_idEstagio, a_custo_total, a_sol_dual_var_estado, map_solucao_dual_proxy, a_vlr_var_estado, a_modeloOtimizacao, busca_abertura_em_todos_processos);
			else if (tipo_aversao_a_risco == TipoAversaoRisco_CVAR)
				executarPDDE_calcularCorteBendersCVaR_new(a_idIteracao, a_idProcesso, a_maiorIdProcesso, a_idEstagio, a_custo_total, a_sol_dual_var_estado, map_solucao_dual_proxy, a_vlr_var_estado, a_modeloOtimizacao, busca_abertura_em_todos_processos);
			else
				throw std::invalid_argument("Tipo de aversao a risco nao implementado.");

		} // if (a_modeloOtimizacao.getElementoVetor(AttVetorModeloOtimizacao_cortes_multiplos, a_idEstagio, int()) == 0) {

		else
			executarPDDE_calcularCorteBendersMultiCut_new(a_idIteracao, a_idProcesso, a_maiorIdProcesso, a_idEstagio, a_custo_total, a_sol_dual_var_estado, map_solucao_dual_proxy, a_vlr_var_estado, a_modeloOtimizacao, busca_abertura_em_todos_processos);


	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_calcularCorteBenders_new(" + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + "," + getFullString(a_idEstagio) + ",...,a_modeloOtimizacao): \n" + std::string(erro.what())); }

} // void MetodoSolucao::executarPDDE_calcularCorteBenders_new(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, double* a_custo_total, double* a_sol_inf_var_dinamica, double* a_solucao_dual_var_dinamica, double* a_limite_inferior_var_dinamica, double* a_limite_superior_var_dinamica, double* a_sol_dual_var_estado, ModeloOtimizacao& a_modeloOtimizacao){

void MetodoSolucao::executarPDDE_sincronizarResultadosParaCorteBenders(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, EstruturaResultados<int>& a_status_otimizacao, EstruturaResultados<double>& a_custo_total, EstruturaResultados<double>& a_sol_inf_var_dinamica, EstruturaResultados<double>& a_solucao_dual_var_dinamica, EstruturaResultados<double>& a_limite_inferior_var_dinamica, EstruturaResultados<double>& a_limite_superior_var_dinamica, EstruturaResultados<double>& a_sol_dual_var_estado, EstruturaResultados<double>& a_vlr_var_estado, ModeloOtimizacao& a_modeloOtimizacao, const bool a_busca_global) {
	try {

		const int numero_estados = int(a_modeloOtimizacao.getMaiorId(a_idEstagio, IdVariavelEstado()));
		int numero_total_cenarios_em_estados = 0;
		
		if (a_busca_global)
			numero_total_cenarios_em_estados = a_modeloOtimizacao.getNumeroTotalCenariosEmEstados(a_idProcesso, a_maiorIdProcesso, a_idEstagio, a_idIteracao);

		const int numero_variaveis_dinamicas = a_modeloOtimizacao.getNumeroVariavelDinamica(a_TSS, a_idEstagio);
		const int numero_processos = int(a_maiorIdProcesso);

		EstruturaResultados<int> status_otimizacao_all(numero_processos);
		EstruturaResultados<double> custo_total_all(numero_processos);
		EstruturaResultados<double> sol_inf_var_dinamica_all(numero_processos);

		EstruturaResultados<double> solucao_dual_var_dinamica_all(numero_processos);
		EstruturaResultados<double> limite_inferior_var_dinamica_all(numero_processos);
		EstruturaResultados<double> limite_superior_var_dinamica_all(numero_processos);

		EstruturaResultados<double> vlr_var_estado_all(numero_processos);
		EstruturaResultados<double> sol_dual_var_estado_all(numero_processos);

		for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) {

			const int rank = getRank(idProcesso);

			const int maior_numero_aberturas_processo_estagio = a_modeloOtimizacao.getMaiorNumeroAberturasProcessoEstagio(idProcesso, a_idEstagio, a_idIteracao);

			const IdCenario cenario_inicial = a_modeloOtimizacao.getCenarioInicial(idProcesso, a_idIteracao);
			const IdCenario cenario_final = a_modeloOtimizacao.getCenarioFinal(idProcesso, a_idIteracao);

			int numero_cenarios = a_modeloOtimizacao.getNumeroCenarios(cenario_inicial, cenario_final);
			if (a_busca_global)
				numero_cenarios = numero_total_cenarios_em_estados;

			status_otimizacao_all.setSize(rank, numero_cenarios, maior_numero_aberturas_processo_estagio);
			custo_total_all.setSize(rank, numero_cenarios, maior_numero_aberturas_processo_estagio);
			sol_inf_var_dinamica_all.setSize(rank, numero_cenarios, maior_numero_aberturas_processo_estagio);

			solucao_dual_var_dinamica_all.setSize(rank, numero_cenarios, maior_numero_aberturas_processo_estagio, numero_variaveis_dinamicas);
			limite_inferior_var_dinamica_all.setSize(rank, numero_cenarios, maior_numero_aberturas_processo_estagio, numero_variaveis_dinamicas);
			limite_superior_var_dinamica_all.setSize(rank, numero_cenarios, maior_numero_aberturas_processo_estagio, numero_variaveis_dinamicas);

			vlr_var_estado_all.setSize(rank, numero_cenarios, numero_estados);
			sol_dual_var_estado_all.setSize(rank, numero_cenarios, maior_numero_aberturas_processo_estagio, numero_estados);

		} // for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) {

		MPI_Allgatherv(a_status_otimizacao.vetor, a_status_otimizacao.sizeT[0], MPI_INT, status_otimizacao_all.vetor, status_otimizacao_all.sizeT, status_otimizacao_all.displ, MPI_INT, MPI_COMM_WORLD);
		a_status_otimizacao = status_otimizacao_all;

		MPI_Allgatherv(a_custo_total.vetor, a_custo_total.sizeT[0], MPI_DOUBLE, custo_total_all.vetor, custo_total_all.sizeT, custo_total_all.displ, MPI_DOUBLE, MPI_COMM_WORLD);
		a_custo_total = custo_total_all;

		MPI_Allgatherv(a_sol_inf_var_dinamica.vetor, a_sol_inf_var_dinamica.sizeT[0], MPI_DOUBLE, sol_inf_var_dinamica_all.vetor, sol_inf_var_dinamica_all.sizeT, sol_inf_var_dinamica_all.displ, MPI_DOUBLE, MPI_COMM_WORLD);
		a_sol_inf_var_dinamica = sol_inf_var_dinamica_all;

		MPI_Allgatherv(a_solucao_dual_var_dinamica.vetor, a_solucao_dual_var_dinamica.sizeT[0], MPI_DOUBLE, solucao_dual_var_dinamica_all.vetor, solucao_dual_var_dinamica_all.sizeT, solucao_dual_var_dinamica_all.displ, MPI_DOUBLE, MPI_COMM_WORLD);
		a_solucao_dual_var_dinamica = solucao_dual_var_dinamica_all;

		MPI_Allgatherv(a_limite_inferior_var_dinamica.vetor, a_limite_inferior_var_dinamica.sizeT[0], MPI_DOUBLE, limite_inferior_var_dinamica_all.vetor, limite_inferior_var_dinamica_all.sizeT, limite_inferior_var_dinamica_all.displ, MPI_DOUBLE, MPI_COMM_WORLD);
		a_limite_inferior_var_dinamica = limite_inferior_var_dinamica_all;

		MPI_Allgatherv(a_limite_superior_var_dinamica.vetor, a_limite_superior_var_dinamica.sizeT[0], MPI_DOUBLE, limite_superior_var_dinamica_all.vetor, limite_superior_var_dinamica_all.sizeT, limite_superior_var_dinamica_all.displ, MPI_DOUBLE, MPI_COMM_WORLD);
		a_limite_superior_var_dinamica = limite_superior_var_dinamica_all;

		MPI_Allgatherv(a_vlr_var_estado.vetor, a_vlr_var_estado.sizeT[0], MPI_DOUBLE, vlr_var_estado_all.vetor, vlr_var_estado_all.sizeT, vlr_var_estado_all.displ, MPI_DOUBLE, MPI_COMM_WORLD);
		a_vlr_var_estado = vlr_var_estado_all;

		MPI_Allgatherv(a_sol_dual_var_estado.vetor, a_sol_dual_var_estado.sizeT[0], MPI_DOUBLE, sol_dual_var_estado_all.vetor, sol_dual_var_estado_all.sizeT, sol_dual_var_estado_all.displ, MPI_DOUBLE, MPI_COMM_WORLD);
		a_sol_dual_var_estado = sol_dual_var_estado_all;

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_sincronizarResultadosParaCorteBenders(" + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + "," + getFullString(a_idEstagio) + ",...,a_modeloOtimizacao): \n" + std::string(erro.what())); }
}

void MetodoSolucao::executarPDDE_calcularCorteBendersValorEsperado_new(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, EstruturaResultados<double>& a_custo_total, EstruturaResultados<double>& a_sol_dual_var_estado, EstruturaResultados<int> & a_map_solucao_dual_proxy, EstruturaResultados<double>& a_vlr_var_estado, ModeloOtimizacao& a_modeloOtimizacao, const bool a_busca_global) {
	try {

		const int nr = a_custo_total.bloco;

		// Inicialização com inclusão de número de cortes igual a nr * nc
		int r_min = 0;
		int r_max = nr;
		// Em caso de busca global, numero de cortes igual a nc
		if (a_busca_global) {
			r_min = getRank(a_idProcesso);
			r_max = getRank(a_idProcesso) + 1;
		} // if (a_busca_global) {

		for (int r_aux = r_min; r_aux < r_max; r_aux++) {

			int r = r_aux;
			// Caso processo nao tenha calculado nenhuma abertura, o mesmo procede instanciamento de cortes igual ao do mestre.
			if (a_custo_total.size1[r] == 0)
				r = 0;

			const int nc = a_custo_total.size1[r];
			const int ne = a_sol_dual_var_estado.size3[r];

			// Inicialização com busca de abertura somente em r
			int r_min_busca = r;
			int r_max_busca = r + 1;
			// Em caso de busca global, busca-se aberturas em todo nr
			if (a_busca_global) {
				r_min_busca = 0;
				r_max_busca = nr;
			} // if (a_busca_global) {

			for (int c = 0; c < nc; c++) {

				IdCenario idCenario = IdCenario_Nenhum;

				if (!a_busca_global)
					idCenario = IdCenario(a_modeloOtimizacao.getCenarioInicial(IdProcesso(r + 1), a_idIteracao) + c);

				SmartEnupla<IdRealizacao, double> enupla_rhs(IdRealizacao_1, std::vector<double>(1, 0.0));
				SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> enupla_coeficiente(IdRealizacao_1, std::vector<SmartEnupla<IdVariavelEstado, double>>(1, SmartEnupla<IdVariavelEstado, double>(IdVariavelEstado_1, std::vector<double>(ne, 0.0))));
				SmartEnupla<IdVariavelEstado, double> enupla_estado(IdVariavelEstado_1, std::vector<double>(ne, 0.0));

				bool sem_corte = false;

				bool first_round = true;
				for (int r_busca = r_min_busca; r_busca < r_max_busca; r_busca++) {

					if ((a_busca_global) && (r != r_busca)) {
						if ((nc > a_custo_total.size1[r_busca]) && (a_custo_total.size1[r_busca] > 0))
							throw std::invalid_argument("Estrutura nao compativel com busca global.");
					} // if ((a_busca_global) && (r != r_busca)){

					const int na_busca = a_custo_total.size2[r_busca];

					IdProcesso idProcesso_busca = IdProcesso(r + 1);
					if (a_busca_global) {
						idProcesso_busca = IdProcesso(r_busca + 1);					
						idCenario = a_modeloOtimizacao.getCenarioInicial(idProcesso_busca, a_idIteracao);
					} // if (a_busca_global) {

					const IdAbertura abertura_inicial = a_modeloOtimizacao.getElementoMatriz(a_idIteracao, idProcesso_busca, AttMatrizProcesso_mapeamento_abertura_inicial_por_cenario_por_estagio, idCenario, a_idEstagio, IdAbertura());

					const int indice_inicial_estado = a_vlr_var_estado.displ[r_busca] + c * ne;
					for (int a_busca = 0; a_busca < na_busca; a_busca++) {
						const int indice_custo_total = a_custo_total.displ[r_busca] + c * na_busca + a_busca;

						int indice_inicial_sol_dual = a_sol_dual_var_estado.displ[r_busca] + c * na_busca * ne + a_busca * ne;
						if (a_map_solucao_dual_proxy.vetor[indice_custo_total] == -2) {
							sem_corte = true;
							break;
						}
						else if (a_map_solucao_dual_proxy.vetor[indice_custo_total] > -1)
							indice_inicial_sol_dual = a_map_solucao_dual_proxy.vetor[indice_custo_total];

						const double probabilidade_original = a_modeloOtimizacao.getProbabilidadeAbertura(a_idEstagio, IdRealizacao(abertura_inicial + a_busca));

						enupla_rhs.at(IdRealizacao_1) += a_custo_total.vetor[indice_custo_total] * probabilidade_original;

						for (int e = 0; e < ne; e++) {
							const IdVariavelEstado idVariavelEstado = IdVariavelEstado(e + 1);

							enupla_rhs.at(IdRealizacao_1) -= a_sol_dual_var_estado.vetor[indice_inicial_sol_dual + e] * a_vlr_var_estado.vetor[indice_inicial_estado + e] * probabilidade_original;
							enupla_coeficiente.at(IdRealizacao_1).at(idVariavelEstado) += a_sol_dual_var_estado.vetor[indice_inicial_sol_dual + e] * probabilidade_original;
							if (first_round)
								enupla_estado.at(idVariavelEstado) = a_vlr_var_estado.vetor[indice_inicial_estado + e];
						} // for (int e = 0; e < ne; e++) {

						first_round = false;
					} // for (int a_busca = 0; a_busca < na_busca; a_busca++) {

					if (sem_corte)
						break;

				} // for (int r_busca = r_min_busca; r_busca < r_max_busca; r_busca++) {

				if (!sem_corte)
					a_modeloOtimizacao.instanciarCorteBenders(a_idEstagio, enupla_rhs, enupla_coeficiente, enupla_estado);

			} // for (int c = 0; c < nc; c++) {

		} // for (int r = r_min; r < r_max; r++) {

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_calcularCorteBendersValorEsperado_new(" + getFullString(a_idIteracao) + "," + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + "," + getFullString(a_idEstagio) + "a_custo_total,a_solucao_dual_variavel_estado,a_modeloOtimizacao): \n" + std::string(erro.what())); }
}

void MetodoSolucao::executarPDDE_calcularCorteBendersCVaR_new(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, EstruturaResultados<double>& a_custo_total, EstruturaResultados<double>& a_sol_dual_var_estado, EstruturaResultados<int>& a_map_solucao_dual_proxy, EstruturaResultados<double>& a_vlr_var_estado, ModeloOtimizacao& a_modeloOtimizacao, const bool a_busca_global) {
	try {

		const double lambda_CVAR = a_modeloOtimizacao.getElementoVetor(AttVetorModeloOtimizacao_lambda_CVAR, a_idEstagio, double());
		const double  alpha_CVAR = a_modeloOtimizacao.getElementoVetor(AttVetorModeloOtimizacao_alpha_CVAR, a_idEstagio, double());
		const int numero_total_aberturas_estagio = int(a_modeloOtimizacao.getAtributo(a_idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()));

		const int nr = a_custo_total.bloco;

		// Inicialização com inclusão de número de cortes igual a nr * nc
		int r_min = 0;
		int r_max = nr;
		// Em caso de busca global, numero de cortes igual a nc
		if (a_busca_global) {
			r_min = getRank(a_idProcesso);
			r_max = getRank(a_idProcesso) + 1;
		} // if (a_busca_global) {

		for (int r_aux = r_min; r_aux < r_max; r_aux++) {

			int r = r_aux;
			// Caso processo nao tenha calculado nenhuma abertura, o mesmo procede instanciamento de cortes igual ao do mestre.
			if (a_custo_total.size1[r] == 0)
				r = 0;

			const int nc = a_custo_total.size1[r];
			const int ne = a_sol_dual_var_estado.size3[r];

			// Inicialização com busca de abertura somente em r
			int r_min_busca = r;
			int r_max_busca = r + 1;
			// Em caso de busca global, busca-se aberturas em todo nr
			if (a_busca_global) {
				r_min_busca = 0;
				r_max_busca = nr;
			} // if (a_busca_global) {

			for (int c = 0; c < nc; c++) {

				IdCenario idCenario = IdCenario_Nenhum;

				if (!a_busca_global)
					idCenario = IdCenario(a_modeloOtimizacao.getCenarioInicial(IdProcesso(r + 1), a_idIteracao) + c);

				bool sem_corte = false;

				std::vector<double> probabilidade_maior_para_menor;
				std::vector<double> custo_total_maior_para_menor;
				std::vector<int>    r_custo_total_maior_para_menor;
				std::vector<int>    a_custo_total_maior_para_menor;
				std::vector<int>    na_custo_total_maior_para_menor;
				probabilidade_maior_para_menor.reserve(numero_total_aberturas_estagio);
				r_custo_total_maior_para_menor.reserve(numero_total_aberturas_estagio);
				a_custo_total_maior_para_menor.reserve(numero_total_aberturas_estagio);
				na_custo_total_maior_para_menor.reserve(numero_total_aberturas_estagio);

				for (int r_busca = r_min_busca; r_busca < r_max_busca; r_busca++) {

					if ((a_busca_global) && (r != r_busca)) {
						if ((nc > a_custo_total.size1[r_busca]) && (a_custo_total.size1[r_busca] > 0))
							throw std::invalid_argument("Estrutura nao compativel com busca global.");
					} // if ((a_busca_global) && (r != r_busca)){

					const int na_busca = a_custo_total.size2[r_busca];

					IdProcesso idProcesso_busca = IdProcesso(r + 1);

					if (a_busca_global) {
						idProcesso_busca = IdProcesso(r_busca + 1);
						idCenario = a_modeloOtimizacao.getCenarioInicial(idProcesso_busca, a_idIteracao);
					} // if (a_busca_global) {

					const IdAbertura abertura_inicial = a_modeloOtimizacao.getElementoMatriz(a_idIteracao, idProcesso_busca, AttMatrizProcesso_mapeamento_abertura_inicial_por_cenario_por_estagio, idCenario, a_idEstagio, IdAbertura());

					for (int a_busca = 0; a_busca < na_busca; a_busca++) {

						const int indice_custo_total = a_custo_total.displ[r_busca] + c * na_busca + a_busca;

						if (a_map_solucao_dual_proxy.vetor[indice_custo_total] == -2) {
							sem_corte = true;
							break;
						}

						const double probabilidade_original = a_modeloOtimizacao.getProbabilidadeAbertura(a_idEstagio, IdRealizacao(abertura_inicial + a_busca));

						const double custo_total = a_custo_total.vetor[indice_custo_total];

						bool custo_maio_inserido = false;
						for (int i = 0; i < int(custo_total_maior_para_menor.size()); i++) {
							if (custo_total > custo_total_maior_para_menor.at(i)) {
								custo_total_maior_para_menor.insert(custo_total_maior_para_menor.begin() + i, custo_total);
								probabilidade_maior_para_menor.insert(probabilidade_maior_para_menor.begin() + i, probabilidade_original);
								r_custo_total_maior_para_menor.insert(r_custo_total_maior_para_menor.begin() + i, r_busca);
								a_custo_total_maior_para_menor.insert(a_custo_total_maior_para_menor.begin() + i, a_busca);
								na_custo_total_maior_para_menor.insert(na_custo_total_maior_para_menor.begin() + i, na_busca);
								custo_maio_inserido = true;
								break;
							} // if (custo_total > custo_total_maior_para_menor.at(i)) {
						} // for (int i = 0; i < int(custo_total_maior_para_menor.size()); i++) {

						if (!custo_maio_inserido) {
							custo_total_maior_para_menor.push_back(custo_total);
							probabilidade_maior_para_menor.push_back(probabilidade_original);
							r_custo_total_maior_para_menor.push_back(r_busca);
							a_custo_total_maior_para_menor.push_back(a_busca);
							na_custo_total_maior_para_menor.push_back(na_busca);
						} // if (!custo_maio_inserido) {

					} // for (int a_busca = 0; a_busca < na_busca; a_busca++) {

				} // for (int r_busca = r_min_busca; r_busca < r_max_busca; r_busca++) {

				if (!sem_corte) {

					SmartEnupla<IdRealizacao, double> enupla_rhs(IdRealizacao_1, std::vector<double>(1, 0.0));
					SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> enupla_coeficiente(IdRealizacao_1, std::vector<SmartEnupla<IdVariavelEstado, double>>(1, SmartEnupla<IdVariavelEstado, double>(IdVariavelEstado_1, std::vector<double>(ne, 0.0))));
					SmartEnupla<IdVariavelEstado, double> enupla_estado(IdVariavelEstado_1, std::vector<double>(ne, 0.0));

					double probabilidade_acumulada = 0.0;
					for (int i = 0; i < int(custo_total_maior_para_menor.size()); i++) {

						const double probabilidade_original = probabilidade_maior_para_menor.at(i);

						probabilidade_acumulada += probabilidade_original;

						const int r_busca = r_custo_total_maior_para_menor.at(i);
						const int a_busca = a_custo_total_maior_para_menor.at(i);
						const int na_busca = na_custo_total_maior_para_menor.at(i);

						const int indice_inicial_estado = a_vlr_var_estado.displ[r_busca] + c * ne;

						const int indice_custo_total = a_custo_total.displ[r_busca] + c * na_busca + a_busca;

						int indice_inicial_sol_dual = a_sol_dual_var_estado.displ[r_busca] + c * na_busca * ne + a_busca * ne;
						if (a_map_solucao_dual_proxy.vetor[indice_custo_total] == -2) {
							sem_corte = true;
							break;
						}
						else if (a_map_solucao_dual_proxy.vetor[indice_custo_total] > -1)
							indice_inicial_sol_dual = a_map_solucao_dual_proxy.vetor[indice_custo_total];

						double probabilidade_CVaR = probabilidade_original;
						if (probabilidade_acumulada <= alpha_CVAR)
							probabilidade_CVaR = probabilidade_original * (1.0 - lambda_CVAR) + probabilidade_original * (lambda_CVAR / alpha_CVAR);
						else if ((probabilidade_acumulada > alpha_CVAR) && (alpha_CVAR >= probabilidade_maior_para_menor.at(0)))
							probabilidade_CVaR = probabilidade_original * (1.0 - lambda_CVAR);

						enupla_rhs.at(IdRealizacao_1) += a_custo_total.vetor[indice_custo_total] * probabilidade_CVaR;

						for (int e = 0; e < ne; e++) {
							const IdVariavelEstado idVariavelEstado = IdVariavelEstado(e + 1);

							enupla_rhs.at(IdRealizacao_1) -= a_sol_dual_var_estado.vetor[indice_inicial_sol_dual + e] * a_vlr_var_estado.vetor[indice_inicial_estado + e] * probabilidade_CVaR;
							enupla_coeficiente.at(IdRealizacao_1).at(idVariavelEstado) += a_sol_dual_var_estado.vetor[indice_inicial_sol_dual + e] * probabilidade_CVaR;
							if (i == 0)
								enupla_estado.at(idVariavelEstado) = a_vlr_var_estado.vetor[indice_inicial_estado + e];
						} // for (int e = 0; e < ne; e++) {

					} // for (int i = 0; i < int(custo_total_maior_para_menor.size()); i++) {

					a_modeloOtimizacao.instanciarCorteBenders(a_idEstagio, enupla_rhs, enupla_coeficiente, enupla_estado);

				} // if (!sem_corte)

			} // for (int c = 0; c < nc; c++) {

		} // for (int r = r_min; r < r_max; r++) {

	} // try {

	catch (const std::exception& erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_calcularCorteBendersCVaR_new(" + getFullString(a_idIteracao) + "," + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + "," + getFullString(a_idEstagio) + "a_custo_total,a_solucao_dual_variavel_estado,a_modeloOtimizacao): \n" + std::string(erro.what())); }
}

void MetodoSolucao::executarPDDE_calcularCorteBendersMultiCut_new(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, EstruturaResultados<double>& a_custo_total, EstruturaResultados<double>& a_sol_dual_var_estado, EstruturaResultados<int>& a_map_solucao_dual_proxy, EstruturaResultados<double>& a_vlr_var_estado, ModeloOtimizacao& a_modeloOtimizacao, const bool a_busca_global) {
	try {

		const int nr = a_custo_total.bloco;

		const int numero_total_aberturas = int(a_modeloOtimizacao.getAtributo(a_idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()));

		// Inicialização com inclusão de número de cortes igual a nr * nc
		int r_min = 0;
		int r_max = nr;
		// Em caso de busca global, numero de cortes igual a nc
		if (a_busca_global) {
			r_min = getRank(a_idProcesso);
			r_max = getRank(a_idProcesso) + 1;
		} // if (a_busca_global) {

		for (int r_aux = r_min; r_aux < r_max; r_aux++) {

			int r = r_aux;
			// Caso processo nao tenha calculado nenhuma abertura, o mesmo procede instanciamento de cortes igual ao do mestre.
			if (a_custo_total.size1[r] == 0)
				r = 0;

			const int nc = a_custo_total.size1[r];
			const int ne = a_sol_dual_var_estado.size3[r];

			// Inicialização com busca de abertura somente em r
			int r_min_busca = r;
			int r_max_busca = r + 1;
			// Em caso de busca global, busca-se aberturas em todo nr
			if (a_busca_global) {
				r_min_busca = 0;
				r_max_busca = nr;
			} // if (a_busca_global) {

			for (int c = 0; c < nc; c++) {

				IdCenario idCenario = IdCenario_Nenhum;
				if (!a_busca_global)
					idCenario = IdCenario(a_modeloOtimizacao.getCenarioInicial(IdProcesso(r + 1), a_idIteracao) + c);

				SmartEnupla<IdRealizacao, double> enupla_rhs(IdRealizacao_1, std::vector<double>(numero_total_aberturas, 0.0));
				SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> enupla_coeficiente(IdRealizacao_1, std::vector<SmartEnupla<IdVariavelEstado, double>>(numero_total_aberturas, SmartEnupla<IdVariavelEstado, double>()));
				SmartEnupla<IdVariavelEstado, double> enupla_estado(IdVariavelEstado_1, std::vector<double>(ne, 0.0));

				bool sem_corte = false;

				bool first_round = true;
				for (int r_busca = r_min_busca; r_busca < r_max_busca; r_busca++) {

					if ((a_busca_global) && (r != r_busca)) {
						if ((nc > a_custo_total.size1[r_busca]) && (a_custo_total.size1[r_busca] > 0))
							throw std::invalid_argument("Estrutura nao compativel com busca global.");
					} // if ((a_busca_global) && (r != r_busca)){

					const int na_busca = a_custo_total.size2[r_busca];

					IdProcesso idProcesso_busca = IdProcesso(r + 1);

					if (a_busca_global) {
						idProcesso_busca = IdProcesso(r_busca + 1);
						idCenario = a_modeloOtimizacao.getCenarioInicial(idProcesso_busca, a_idIteracao);;
					} // if (a_busca_global) {

					const IdAbertura abertura_inicial = a_modeloOtimizacao.getElementoMatriz(a_idIteracao, idProcesso_busca, AttMatrizProcesso_mapeamento_abertura_inicial_por_cenario_por_estagio, idCenario, a_idEstagio, IdAbertura());

					const int indice_inicial_estado = a_vlr_var_estado.displ[r_busca] + c * ne;
					for (int a_busca = 0; a_busca < na_busca; a_busca++) {

						const IdRealizacao idRealizacao = IdRealizacao(abertura_inicial + a_busca);

						if (enupla_coeficiente.at(idRealizacao).size() == 0)
							enupla_coeficiente.at(idRealizacao) = SmartEnupla<IdVariavelEstado, double>(IdVariavelEstado_1, std::vector<double>(ne, 0.0));

						const int indice_custo_total = a_custo_total.displ[r_busca] + c * na_busca + a_busca;

						int indice_inicial_sol_dual = a_sol_dual_var_estado.displ[r_busca] + c * na_busca * ne + a_busca * ne;
						if (a_map_solucao_dual_proxy.vetor[indice_custo_total] == -2) {
							sem_corte = true;
							break;
						}
						else if (a_map_solucao_dual_proxy.vetor[indice_custo_total] > -1)
							indice_inicial_sol_dual = a_map_solucao_dual_proxy.vetor[indice_custo_total];

						enupla_rhs.at(idRealizacao) = a_custo_total.vetor[indice_custo_total];

						for (int e = 0; e < ne; e++) {
							const IdVariavelEstado idVariavelEstado = IdVariavelEstado(e + 1);

							enupla_rhs.at(idRealizacao) -= a_sol_dual_var_estado.vetor[indice_inicial_sol_dual + e] * a_vlr_var_estado.vetor[indice_inicial_estado + e];
							enupla_coeficiente.at(idRealizacao).at(idVariavelEstado) += a_sol_dual_var_estado.vetor[indice_inicial_sol_dual + e];
							if (first_round)
								enupla_estado.at(idVariavelEstado) = a_vlr_var_estado.vetor[indice_inicial_estado + e];
						} // for (int e = 0; e < ne; e++) {

						first_round = false;
					} // for (int a_busca = 0; a_busca < na_busca; a_busca++) {

					if (sem_corte)
						break;

				} // for (int r_busca = r_min_busca; r_busca < r_max_busca; r_busca++) {

				if (!sem_corte)
					a_modeloOtimizacao.instanciarCorteBenders(a_idEstagio, enupla_rhs, enupla_coeficiente, enupla_estado);

			} // for (int c = 0; c < nc; c++) {

		} // for (int r = r_min; r < r_max; r++) {

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_calcularCorteBendersMultiCut_new(" + getFullString(a_idIteracao) + "," + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + "," + getFullString(a_idEstagio) + ",a_custo_total,a_solucao_dual_variavel_estado,a_modeloOtimizacao): \n" + std::string(erro.what())); }
}

void MetodoSolucao::executarPDDE_mapearSolucaoProxy(EstruturaResultados<int>& a_status_otimizacao, EstruturaResultados<double>& a_custo_total, EstruturaResultados<double>& a_sol_inf_var_dinamica, EstruturaResultados<double>& a_solucao_dual_var_dinamica, EstruturaResultados<double>& a_limite_inferior_var_dinamica, EstruturaResultados<double>& a_limite_superior_var_dinamica, EstruturaResultados<double>& a_sol_dual_var_estado, EstruturaResultados<int>& a_map_solucao_dual_proxy, ModeloOtimizacao& a_modeloOtimizacao, const bool a_busca_global) {

	try {
	 
		const TipoSelecaoSolucaoProxy tipo_selecao_solucao_proxy = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_tipo_selecao_solucao_proxy, TipoSelecaoSolucaoProxy());
		
		const int nr = a_custo_total.bloco;

		EstruturaResultados<int> map_solucao_dual_proxy(nr);
		for (int r = 0; r < nr; r++)
			map_solucao_dual_proxy.setSize(r, a_custo_total.size1[r], a_custo_total.size2[r]);

		for (int r = 0; r < nr; r++) {

			const int nc = a_custo_total.size1[r];
			const int na = a_custo_total.size2[r];
			const int ni = a_solucao_dual_var_dinamica.size3[r];
			const int ne = a_sol_dual_var_estado.size3[r];

			int r_min_busca = r;
			int r_max_busca = r + 1;
			if (a_busca_global) {
				r_min_busca = 0;
				r_max_busca = nr;
			} // if (a_busca_global) {

			for (int c = 0; c < nc; c++) {

				bool alguma_solucao_proxy = false;
				for (int r_busca = r_min_busca; r_busca < r_max_busca; r_busca++) {

					const int na_busca = a_custo_total.size2[r_busca];

					if ((a_busca_global) && (r != r_busca)) {
						if ((nc > a_custo_total.size1[r_busca]) && (a_custo_total.size1[r_busca] > 0))
							throw std::invalid_argument("Estrutura nao compativel com busca global.");
					} // if ((a_busca_global) && (r != r_busca)){

					for (int a_busca = 0; a_busca < na_busca; a_busca++) {
						if (a_status_otimizacao.vetor[a_status_otimizacao.displ[r_busca] + c * na_busca + a_busca] > 0) {
							alguma_solucao_proxy = true;
							break;
						}
					} // for (int a_busca = 0; a_busca < na_busca; a_busca++) {
				} // for (int r_busca = r_min_busca; r_busca < r_max_busca; r_busca++) {

				for (int a = 0; a < na; a++) {

					if (!alguma_solucao_proxy)
						map_solucao_dual_proxy.vetor[map_solucao_dual_proxy.displ[r] + c * na + a] = -2;

					else if (a_status_otimizacao.vetor[a_status_otimizacao.displ[r] + c * na + a] == 0) {

						int                      a_selec = -1;
						int    indice_solucao_dual_selec = -1;
						double         custo_total_selec = 0.0;

						int a_maior_custo_primal = -1;

						for (int r_busca = r_min_busca; r_busca < r_max_busca; r_busca++) {
							const int na_busca = a_custo_total.size2[r_busca];

							for (int a_busca = 0; a_busca < na_busca; a_busca++) {

								int a_candidata = a_busca;

								if (a_status_otimizacao.vetor[a_status_otimizacao.displ[r_busca] + c * na_busca + a_busca] > 0) {

									if (tipo_selecao_solucao_proxy == TipoSelecaoSolucaoProxy_aleatoria) {}
									if (tipo_selecao_solucao_proxy == TipoSelecaoSolucaoProxy_maior_custo_primal_proxy) {

										if (a_maior_custo_primal == -1) {
											double custo_primal = 0.0;
											for (int a_custo_primal = 0; a_custo_primal < na_busca; a_custo_primal++) {
												const int idx_a_custo_primal = a_custo_total.displ[r_busca] + c * na_busca + a_custo_primal;
												if (a_status_otimizacao.vetor[idx_a_custo_primal] > 0) {
													if (a_custo_total.vetor[idx_a_custo_primal] > custo_primal) {
														custo_primal = a_custo_total.vetor[idx_a_custo_primal];
														a_maior_custo_primal = a_custo_primal;
													}
												} // 
											} // for (IdAbertura idAbertura_custo_primal = abertura_inicial; idAbertura_custo_primal <= abertura_final; idAbertura_custo_primal++) {
										} // if (a_maior_custo_primal == -1) {
										if (a_maior_custo_primal > -1)
											a_candidata = a_maior_custo_primal;
										else
											throw std::invalid_argument("Nao foi possivel encontrar a maior solucao primal proxy.");

									} // if (tipo_selecao_solucao_proxy == TipoSelecaoSolucaoProxy_maior_custo_primal_proxy) {

									double custo_dinamico_proxy_positivo = 0.0;
									double custo_dinamico_proxy_negativo = 0.0;

									for (int i = 0; i < ni; i++) {

										const int idx_atual     = a_limite_inferior_var_dinamica.displ[r] + c * na * ni + a * ni + i;
										const int idx_candidata = a_solucao_dual_var_dinamica.displ[r_busca] + c * na_busca * ni + a_candidata * ni + i;

										const double solucao_dual_var_dinamica = a_solucao_dual_var_dinamica.vetor[idx_candidata];

										if (solucao_dual_var_dinamica > 0.0) {
											const double limite_inferior = a_limite_inferior_var_dinamica.vetor[idx_atual];
											if (limite_inferior > 0.0)
												custo_dinamico_proxy_positivo += solucao_dual_var_dinamica * limite_inferior;
											else if (limite_inferior < 0.0)
												custo_dinamico_proxy_negativo += solucao_dual_var_dinamica * limite_inferior;
										}
										else if (solucao_dual_var_dinamica < 0.0) {
											const double limite_superior = a_limite_superior_var_dinamica.vetor[idx_atual];
											if (limite_superior > 0.0)
												custo_dinamico_proxy_negativo += solucao_dual_var_dinamica * limite_superior;
											else if (limite_superior < 0.0)
												custo_dinamico_proxy_positivo += solucao_dual_var_dinamica * limite_superior;
										}

									} // for (int i = 0; i < ni; i++) {

									const double solucao_inferior_var_dinamica = a_sol_inf_var_dinamica.vetor[a_sol_inf_var_dinamica.displ[r_busca] + c * na_busca + a_candidata];
									double custo_total_proxy = solucao_inferior_var_dinamica + custo_dinamico_proxy_positivo + custo_dinamico_proxy_negativo;

									if (custo_total_proxy < 0.0)
										custo_total_proxy = 0.0;

									if (custo_total_proxy >= custo_total_selec) {
										indice_solucao_dual_selec = a_sol_dual_var_estado.displ[r_busca] + c * na_busca * ne + a_candidata * ne;
										custo_total_selec = custo_total_proxy;
										a_selec = a_candidata;
									} // if (custo_total_proxy < custo_total_proxy_selecionado) {

									if ((tipo_selecao_solucao_proxy == TipoSelecaoSolucaoProxy_aleatoria) || (tipo_selecao_solucao_proxy == TipoSelecaoSolucaoProxy_maior_custo_primal_proxy))
										break;

								} // if (a_status_otimizacao.vetor[idx_a_busca] > 0) {
							} // for (int a_busca = 0; a_busca < na; a_busca++) {
						} // for (int r_busca = r_min_busca; r_busca < r_max_busca; r_busca++) {

						if (a_selec < 0)
							throw std::invalid_argument("Nao foi encontrada solucao proxy para utilizacao em r" + getString(r) + ", a" + getString(a) + ".");

						a_custo_total.vetor[a_custo_total.displ[r] + c * na + a] = custo_total_selec;
						map_solucao_dual_proxy.vetor[a_custo_total.displ[r] + c * na + a] = indice_solucao_dual_selec;

					} // if (a_status_otimizacao.vetor[idx_status] == 0) {
					else
						map_solucao_dual_proxy.vetor[a_custo_total.displ[r] + c * na + a] = -1;

				} // for (int a = 0; a < na; a++) {
			} // for (int c = 0; c < nc; c++) {
		} // for (int r = 0; r < nr; r++) {

		a_map_solucao_dual_proxy = map_solucao_dual_proxy;

		a_status_otimizacao.esvaziar();
		a_sol_inf_var_dinamica.esvaziar();
		a_solucao_dual_var_dinamica.esvaziar();
		a_limite_inferior_var_dinamica.esvaziar();
		a_limite_superior_var_dinamica.esvaziar();

	} // try {

	catch (const std::exception& erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_mapearSolucaoProxy(" + getFullString(a_busca_global) +  ",...,a_modeloOtimizacao): \n" + std::string(erro.what())); }

}


void MetodoSolucao::executarPDDE_calcularCorteBendersCVaR(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, const SmartEnupla <IdCenario, SmartEnupla<IdRealizacao, double>>& a_custo_total, const SmartEnupla <IdCenario, SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>>>& a_solucao_dual_variavel_estado, const SmartEnupla <IdCenario, SmartEnupla<IdVariavelEstado, double>>& a_valor_variavel_estado, ModeloOtimizacao& a_modeloOtimizacao) {

	try {

		const IdCenario cenario_inicial = a_custo_total.getIteradorInicial();
		const IdCenario cenario_final = a_custo_total.getIteradorFinal();

		const IdVariavelEstado maiorIdVariavelEstado = a_valor_variavel_estado.at(cenario_inicial).getIteradorFinal();

		const IdRealizacao maiorIdRealizacao = a_modeloOtimizacao.getAtributo(a_idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao());

		const int numero_realizacao  = a_custo_total.at(cenario_inicial).size();
		const int numero_estado      = a_valor_variavel_estado.at(cenario_inicial).size();
		
		int numero_cenario     = int(cenario_final - cenario_inicial) + 1;
		const int numero_processo = int(a_maiorIdProcesso);

		int* numero_cenario_por_processo = new int[numero_processo];

		MPI_Allgather(&numero_cenario, 1, MPI_INT, numero_cenario_por_processo, 1, MPI_INT, MPI_COMM_WORLD);

		int maior_numero_cenario = 0;
		for (int rank = 0; rank < numero_processo; rank++) {
			if (maior_numero_cenario < numero_cenario_por_processo[rank])
				maior_numero_cenario = numero_cenario_por_processo[rank];
		} // for (int rank = 0; rank < numero_processo; rank++) {

		int* multiplicidade_otimizada_por_cenario = new int[maior_numero_cenario];

		double* vetor_estado      = new double[maior_numero_cenario * numero_estado];
		double* vetor_rhs         = new double[maior_numero_cenario];
		double* vetor_rhs_primal  = new double[maior_numero_cenario];
		double* vetor_rhs_estado  = new double[maior_numero_cenario];
		double* vetor_coeficiente = new double[maior_numero_cenario * numero_estado];

		
		const double lambda_CVAR = a_modeloOtimizacao.getElementoVetor(AttVetorModeloOtimizacao_lambda_CVAR, a_idEstagio, double());
		const double  alpha_CVAR = a_modeloOtimizacao.getElementoVetor(AttVetorModeloOtimizacao_alpha_CVAR, a_idEstagio, double());

		SmartEnupla<IdRealizacao, double> probabilidade_naoCVAR(IdRealizacao_1, std::vector<double>(maiorIdRealizacao, 0.0));
		SmartEnupla<IdRealizacao, double> probabilidade_CVAR(IdRealizacao_1, std::vector<double>(maiorIdRealizacao, 0.0));
		
		for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao; idRealizacao++) {

			const double probabilidade_original = a_modeloOtimizacao.getProbabilidadeAbertura(a_idEstagio, idRealizacao);

			probabilidade_naoCVAR.setElemento(idRealizacao, probabilidade_original * (1.0 - lambda_CVAR));
			probabilidade_CVAR.setElemento(idRealizacao, probabilidade_original * (1.0 - lambda_CVAR) + probabilidade_original * (lambda_CVAR / alpha_CVAR));

		}//for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao; idRealizacao++) {
			

		for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

			const int c = int(idCenario - cenario_inicial);

			multiplicidade_otimizada_por_cenario[c] = 1;

			SmartEnupla<IdRealizacao, double> probabilidade(IdRealizacao_1, std::vector<double>(maiorIdRealizacao, 0.0));

			const std::vector<int> ordenacao = getOrdenacaoMaiorParaMenor(a_custo_total.at(idCenario).getElementos());

			const int i_CVAR = int(alpha_CVAR * double(maiorIdRealizacao));
		
			for (int i = 1; i <= int(ordenacao.size()); i++) {

				const IdRealizacao idRealizacao = IdRealizacao(ordenacao.at(i - 1) + 1);

				double probabilidade = probabilidade_naoCVAR.getElemento(idRealizacao);

				if (i <= i_CVAR)
					probabilidade = probabilidade_CVAR.getElemento(idRealizacao);

				if (numero_realizacao == 1)
					probabilidade = 1.0;

				if (i == 1)
					vetor_rhs_primal[c] = 0;

				vetor_rhs_primal[c] += a_custo_total.at(idCenario).at(idRealizacao) * probabilidade;

				for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {
					const int e = int(idVariavelEstado - IdVariavelEstado_1);

					if (i == 1)
						vetor_coeficiente[c * numero_estado + e] = 0;

					vetor_coeficiente[c * numero_estado + e] += a_solucao_dual_variavel_estado.at(idCenario).at(idRealizacao).at(idVariavelEstado) * probabilidade;

				} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

			} // for (int i = 1; i <= int(ordenacao.size()); i++) {

			for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {
				const int e = int(idVariavelEstado - IdVariavelEstado_1);

				vetor_estado[c * numero_estado + e] = a_valor_variavel_estado.at(idCenario).at(idVariavelEstado);

				if (e == 0)
					vetor_rhs_estado[c] = 0;

				vetor_rhs_estado[c] -= vetor_coeficiente[c * numero_estado + e] * vetor_estado[c * numero_estado + e];
				 
			} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

			vetor_rhs[c] = vetor_rhs_primal[c] + vetor_rhs_estado[c];

		} // for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

		executarPDDE_adicionarCorteBenders(a_idIteracao, a_idProcesso, a_maiorIdProcesso, a_idEstagio, vetor_estado, vetor_rhs, vetor_rhs_primal, vetor_rhs_estado, vetor_coeficiente, numero_cenario_por_processo, maior_numero_cenario, multiplicidade_otimizada_por_cenario, 1, numero_estado, a_modeloOtimizacao);
		
		delete[]multiplicidade_otimizada_por_cenario;
		delete[]numero_cenario_por_processo;
		delete[]vetor_estado;
		delete[]vetor_rhs;
		delete[]vetor_rhs_estado;
		delete[]vetor_rhs_primal;
		delete[]vetor_coeficiente;
		
	} // try {
	catch (const std::exception & erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_calcularCorteBendersCVaR(" + getFullString(a_idIteracao) + "," + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + "," + getFullString(a_idEstagio) + "a_custo_total,a_solucao_dual_variavel_estado,a_modeloOtimizacao): \n" + std::string(erro.what())); }

} // void MetodoSolucao::executarPDDE_calcularCorteBendersCVaR(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, const SmartEnupla<IdRealizacao, double>& a_custo_total, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>>& a_solucao_dual_variavel_estado, SmartEnupla<IdVariavelEstado, double>& a_valor_variavel_estado, ModeloOtimizacao& a_modeloOtimizacao){

void MetodoSolucao::executarPDDE_calcularCorteBendersMultiCut(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, const SmartEnupla<IdCenario, SmartEnupla<IdRealizacao, double>>& a_custo_total, const SmartEnupla<IdCenario, SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>>>& a_solucao_dual_variavel_estado, const SmartEnupla<IdCenario, SmartEnupla<IdVariavelEstado, double>>& a_valor_variavel_estado, ModeloOtimizacao& a_modeloOtimizacao){

	try{

		const IdCenario cenario_inicial = a_custo_total.getIteradorInicial();
		const IdCenario cenario_final = a_custo_total.getIteradorFinal();

		const IdVariavelEstado maiorIdVariavelEstado = a_valor_variavel_estado.at(cenario_inicial).getIteradorFinal();

		const IdRealizacao maiorIdRealizacao = a_modeloOtimizacao.getAtributo(a_idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao());

		const int multiplicidade = int(maiorIdRealizacao);

		const int numero_realizacao = a_custo_total.at(cenario_inicial).size();
		const int numero_estado = a_valor_variavel_estado.at(cenario_inicial).size();

		int numero_cenario = int(cenario_final - cenario_inicial) + 1;
		const int numero_processo = int(a_maiorIdProcesso);

		int* numero_cenario_por_processo = new int[numero_processo];

		MPI_Allgather(&numero_cenario, 1, MPI_INT, numero_cenario_por_processo, 1, MPI_INT, MPI_COMM_WORLD);

		int maior_numero_cenario = 0;
		for (int rank = 0; rank < numero_processo; rank++) {
			if (maior_numero_cenario < numero_cenario_por_processo[rank])
				maior_numero_cenario = numero_cenario_por_processo[rank];
		} // for (int rank = 0; rank < numero_processo; rank++) {

		int* multiplicidade_otimizada_por_cenario = new int[maior_numero_cenario * multiplicidade];

		double* vetor_estado      = new double[maior_numero_cenario * numero_estado];
		double* vetor_rhs         = new double[maior_numero_cenario * multiplicidade];
		double* vetor_rhs_primal  = new double[maior_numero_cenario * multiplicidade];
		double* vetor_rhs_estado  = new double[maior_numero_cenario * multiplicidade];
		double* vetor_coeficiente = new double[maior_numero_cenario * multiplicidade * numero_estado];

		for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

			const int c = int(idCenario - cenario_inicial);

			for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao; idRealizacao++) {

				const int m = int(idRealizacao - IdRealizacao_1);

				if (a_solucao_dual_variavel_estado.at(idCenario).at(idRealizacao).size() > 0) {

					multiplicidade_otimizada_por_cenario[c * multiplicidade + m] = 1;

					vetor_rhs_primal[c * multiplicidade + m] = a_custo_total.at(idCenario).at(idRealizacao);

					vetor_rhs_estado[c * multiplicidade + m] = 0.0;

					for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {
						const int e = int(idVariavelEstado - IdVariavelEstado_1);
						vetor_coeficiente[c * multiplicidade * numero_estado + m * numero_estado + e] = a_solucao_dual_variavel_estado.at(idCenario).at(idRealizacao).at(idVariavelEstado);

						if (m == 0)
							vetor_estado[c * numero_estado + e] = a_valor_variavel_estado.at(idCenario).at(idVariavelEstado);

						vetor_rhs_estado[c * multiplicidade + m] -= a_solucao_dual_variavel_estado.at(idCenario).at(idRealizacao).at(idVariavelEstado) * a_valor_variavel_estado.at(idCenario).at(idVariavelEstado);

					} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

					vetor_rhs[c * multiplicidade + m] = vetor_rhs_primal[c * multiplicidade + m] + vetor_rhs_estado[c * multiplicidade + m];

				} // if (a_solucao_dual_variavel_estado.at(idCenario).at(idRealizacao).size() > 0) {

				else
					multiplicidade_otimizada_por_cenario[c * multiplicidade + m] = 0;

			} // for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao; idRealizacao++) {
		} // for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

		executarPDDE_adicionarCorteBenders(a_idIteracao, a_idProcesso, a_maiorIdProcesso, a_idEstagio, vetor_estado, vetor_rhs, vetor_rhs_primal, vetor_rhs_estado, vetor_coeficiente, numero_cenario_por_processo, maior_numero_cenario, multiplicidade_otimizada_por_cenario, multiplicidade, numero_estado, a_modeloOtimizacao);

		delete[]multiplicidade_otimizada_por_cenario;
		delete[]numero_cenario_por_processo;
		delete[]vetor_estado;
		delete[]vetor_rhs;
		delete[]vetor_rhs_estado;
		delete[]vetor_rhs_primal;
		delete[]vetor_coeficiente;

	} // try {
	catch (const std::exception & erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_calcularCorteBendersMultiCut(" + getFullString(a_idIteracao) + "," + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + "," + getFullString(a_idEstagio) + "a_custo_total,a_solucao_dual_variavel_estado,a_modeloOtimizacao): \n" + std::string(erro.what())); }

} // void MetodoSolucao::executarPDDE_calcularCorteBendersMultiCut(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, const SmartEnupla<IdCenario, SmartEnupla<IdRealizacao, double>>& a_custo_total, const SmartEnupla<IdCenario, SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>>>& a_solucao_dual_variavel_estado, const SmartEnupla<IdCenario, SmartEnupla<IdVariavelEstado, double>>& a_valor_variavel_estado, ModeloOtimizacao& a_modeloOtimizacao){

void MetodoSolucao::executarPDDE_adicionarCorteBenders(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, double* a_vetor_estado, double* a_vetor_rhs, double* a_vetor_rhs_primal, double* a_vetor_rhs_estado, double* a_vetor_coeficiente, int* a_numero_cenario_por_processo, const int a_maior_numero_cenario, int* a_abertura_otimizada_por_cenario, const int a_multiplicidade_corte, const int a_numero_estado, ModeloOtimizacao& a_modeloOtimizacao){

	try {

		const int numero_processo = int(a_maiorIdProcesso);

		int* abertura_otimizada = new int[numero_processo * a_maior_numero_cenario * a_multiplicidade_corte];

		MPI_Allgather(a_abertura_otimizada_por_cenario, a_maior_numero_cenario * a_multiplicidade_corte, MPI_INT, abertura_otimizada, a_maior_numero_cenario * a_multiplicidade_corte, MPI_INT, MPI_COMM_WORLD);

		double* estado = new double[numero_processo * a_maior_numero_cenario * a_numero_estado];

		MPI_Allgather(a_vetor_estado, a_maior_numero_cenario * a_numero_estado, MPI_DOUBLE, estado, a_maior_numero_cenario * a_numero_estado, MPI_DOUBLE, MPI_COMM_WORLD);

		double* rhs = new double[numero_processo * a_maior_numero_cenario * a_multiplicidade_corte];

		MPI_Allgather(a_vetor_rhs, a_maior_numero_cenario * a_multiplicidade_corte, MPI_DOUBLE, rhs, a_maior_numero_cenario * a_multiplicidade_corte, MPI_DOUBLE, MPI_COMM_WORLD);

		double* rhs_primal = new double[numero_processo * a_maior_numero_cenario * a_multiplicidade_corte];

		MPI_Allgather(a_vetor_rhs_primal, a_maior_numero_cenario * a_multiplicidade_corte, MPI_DOUBLE, rhs_primal, a_maior_numero_cenario * a_multiplicidade_corte, MPI_DOUBLE, MPI_COMM_WORLD);

		double* rhs_estado = new double[numero_processo * a_maior_numero_cenario * a_multiplicidade_corte];

		MPI_Allgather(a_vetor_rhs_estado, a_maior_numero_cenario * a_multiplicidade_corte, MPI_DOUBLE, rhs_estado, a_maior_numero_cenario * a_multiplicidade_corte, MPI_DOUBLE, MPI_COMM_WORLD);

		double* coeficiente = new double[numero_processo * a_maior_numero_cenario * a_multiplicidade_corte * a_numero_estado];

		MPI_Allgather(a_vetor_coeficiente, a_maior_numero_cenario * a_multiplicidade_corte * a_numero_estado, MPI_DOUBLE, coeficiente, a_maior_numero_cenario * a_multiplicidade_corte * a_numero_estado, MPI_DOUBLE, MPI_COMM_WORLD);

		for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) {

			const int rank = getRank(idProcesso);

			const int c_ini_estado      = rank * a_maior_numero_cenario * a_numero_estado;
			const int c_ini_rhs         = rank * a_maior_numero_cenario * a_multiplicidade_corte;
			const int c_ini_coeficiente = rank * a_maior_numero_cenario * a_multiplicidade_corte * a_numero_estado;
			const int c_ini_abertura    = rank * a_maior_numero_cenario * a_multiplicidade_corte;

			for (int c = 0; c < a_numero_cenario_por_processo[rank]; c++) {

				SmartEnupla<IdVariavelEstado, double> enupla_estado(IdVariavelEstado_1, std::vector<double>(&estado[c_ini_estado + c * a_numero_estado], &estado[c_ini_estado + c * a_numero_estado] + a_numero_estado));

				SmartEnupla<IdRealizacao, double> enupla_rhs(IdRealizacao_1, std::vector<double>(a_multiplicidade_corte, 0.0));

				SmartEnupla<IdRealizacao, double> enupla_rhs_primal(IdRealizacao_1, std::vector<double>(a_multiplicidade_corte, 0.0));
				SmartEnupla<IdRealizacao, double> enupla_rhs_estado(IdRealizacao_1, std::vector<double>(a_multiplicidade_corte, 0.0));
				
				SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> enupla_coeficiente(IdRealizacao_1, std::vector<SmartEnupla<IdVariavelEstado, double>>(a_multiplicidade_corte, SmartEnupla<IdVariavelEstado, double>()));

				for (int a = 0; a < a_multiplicidade_corte; a++) {

					if (abertura_otimizada[c_ini_abertura + c * a_multiplicidade_corte + a] == 1) {

						const IdRealizacao idRealizacao = IdRealizacao(a + 1);

						enupla_rhs.at(idRealizacao)        = rhs       [c_ini_rhs + c * a_multiplicidade_corte + a];
						enupla_rhs_primal.at(idRealizacao) = rhs_primal[c_ini_rhs + c * a_multiplicidade_corte + a];
						enupla_rhs_estado.at(idRealizacao) = rhs_estado[c_ini_rhs + c * a_multiplicidade_corte + a];

						enupla_coeficiente.at(idRealizacao) = SmartEnupla<IdVariavelEstado, double>(IdVariavelEstado_1, std::vector<double>(&coeficiente[c_ini_coeficiente + c * a_multiplicidade_corte * a_numero_estado + a * a_numero_estado], &coeficiente[c_ini_coeficiente + c * a_multiplicidade_corte * a_numero_estado + a * a_numero_estado] + a_numero_estado));
						
					} // if (abertura_otimizada[c_ini_abertura + c * a_multiplicidade_corte + a] == 1) {

				} // for (int a = 0; a < a_multiplicidade_corte; a++) {

				a_modeloOtimizacao.instanciarCorteBenders(a_idEstagio, enupla_rhs, enupla_rhs_primal, enupla_rhs_estado, enupla_coeficiente, enupla_estado);

			} // for (int c = 0; c < a_numero_cenario; c++) {

		} // for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) {

		delete[]abertura_otimizada;
		delete[]estado;
		delete[]rhs;
		delete[]rhs_estado;
		delete[]rhs_primal;
		delete[]coeficiente;

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_adicionarCorteBenders(" + getFullString(a_idIteracao) + "," + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + "," + getFullString(a_idEstagio) + ",a_rhs,a_rhs_primal,a_rhs_estado,a_coeficiente,a_estado,a_modeloOtimizacao): \n" + std::string(erro.what())); }

} // void MetodoSolucao::executarPDDE_adicionarCorteBenders_multi(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, const double a_rhs, const SmartEnupla<IdVariavelEstado, double>& a_coeficientes_variavel_estado, const SmartEnupla<IdVariavelEstado, double>& a_valor_variavel_estado, ModeloOtimizacao & a_modeloOtimizacao){



void MetodoSolucao::executarPDDE_requestCorteBenders(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, ModeloOtimizacao& a_modeloOtimizacao, const std::string a_diretorio, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		a_modeloOtimizacao.requestCorteBenders(a_idProcesso, a_idEstagio, a_diretorio, a_entradaSaidaDados);

	} // try {
	catch (const std::exception & erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_requestCorteBenders(" + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + "," + getFullString(a_idEstagio) + ",a_modeloOtimizacao): \n" + std::string(erro.what())); }

} // void MetodoSolucao::executarPDDE_requestCorteBenders(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdEstagio a_idEstagio, ModeloOtimizacao &a_modeloOtimizacao){


void MetodoSolucao::executarPDDE_distribuirEstadosEntreProcessos(const IdProcesso a_idProcesso, const IdProcesso a_maior_processo, const IdIteracao a_idIteracao, ModeloOtimizacao& a_modeloOtimizacao){

	try {

		const IdEstagio estagio_inicial = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
		const IdEstagio estagio_final = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());
		const IdEstagio maior_estagio = a_modeloOtimizacao.getMaiorId(IdEstagio());

		bool pre_estudo = false;
		if (!strCompara(a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_diretorio_importacao_pos_estudo, std::string()), "nenhum"))
			pre_estudo = true;

		int numero_total_estados = 0;
		for (IdEstagio idEstagio = estagio_inicial; idEstagio <= maior_estagio; idEstagio++) {
			bool considera_estagio = false;

			if (idEstagio <= estagio_final) {
				if (a_modeloOtimizacao.getElementoVetor(a_idIteracao, AttVetorIteracao_tipo_processamento_paralelo, idEstagio, TipoProcessamentoParalelo()) == TipoProcessamentoParalelo_por_abertura)
					considera_estagio = true;
			}
			else if ((idEstagio > estagio_final) && (pre_estudo) && (numero_total_estados > 0))
				considera_estagio = true;

			if (considera_estagio)
				numero_total_estados += int(a_modeloOtimizacao.getMaiorId(idEstagio, IdVariavelEstado()));
		}


		if (numero_total_estados == 0)
			return;

		const int numero_processos = int(a_maior_processo);

		EstruturaResultados<double> estados(1);
		EstruturaResultados<double> estados_all(numero_processos);

		for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maior_processo; idProcesso++) {
			const int rank = getRank(idProcesso);	

			const IdCenario cenario_inicial = a_modeloOtimizacao.getCenarioInicial(idProcesso, a_idIteracao);
			const IdCenario cenario_final = a_modeloOtimizacao.getCenarioFinal(idProcesso, a_idIteracao);

			int numero_cenarios = int(cenario_final - cenario_inicial) + 1;
			if ((cenario_inicial == IdCenario_Nenhum) && (cenario_final == IdCenario_Nenhum))
				numero_cenarios = 0;

			estados_all.setSize(rank, numero_total_estados, numero_cenarios);

			if (idProcesso == a_idProcesso) {

				estados.setSize(0, numero_total_estados, numero_cenarios);

				if (estados.total > 0) {
					int i = 0;
					for (IdEstagio idEstagio = estagio_inicial; idEstagio <= maior_estagio; idEstagio++) {
						for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= a_modeloOtimizacao.getMaiorId(idEstagio, IdVariavelEstado()); idVariavelEstado++) {
							if (a_modeloOtimizacao.isVariavelEstadoInstanciada(idEstagio, idVariavelEstado)) {
								for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {
									estados.vetor[i] = a_modeloOtimizacao.getElementoMatriz(idEstagio, idVariavelEstado, AttMatrizVariavelEstado_valor, a_idProcesso, idCenario, double());
										i++;
								}
							}
						}
					} // for (IdEstagio idEstagio = estagio_inicial; idEstagio <= maior_estagio; idEstagio++) {
				} // if (estados.total > 0) {

			} // if (idProcesso == a_idProcesso) {

		} // for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maior_processo; idProcesso++) {

		MPI_Allgatherv(estados.vetor, estados.total, MPI_DOUBLE, estados_all.vetor, estados_all.sizeT, estados_all.displ, MPI_DOUBLE, MPI_COMM_WORLD);

		estados.esvaziar();

		for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maior_processo; idProcesso++) {

			if (idProcesso != a_idProcesso) {

				const int rank = getRank(idProcesso);

				const IdCenario cenario_inicial = a_modeloOtimizacao.getCenarioInicial(idProcesso, a_idIteracao);
				const IdCenario cenario_final = a_modeloOtimizacao.getCenarioFinal(idProcesso, a_idIteracao);

				const int numero_cenarios = a_modeloOtimizacao.getNumeroCenarios(cenario_inicial, cenario_final);

				if (numero_cenarios > 0) {

					int e = 0;
					for (IdEstagio idEstagio = estagio_inicial; idEstagio <= maior_estagio; idEstagio++) {

						bool considera_estagio = false;

						if (idEstagio <= estagio_final) {
							if (a_modeloOtimizacao.getElementoVetor(a_idIteracao, AttVetorIteracao_tipo_processamento_paralelo, idEstagio, TipoProcessamentoParalelo()) == TipoProcessamentoParalelo_por_abertura)
								considera_estagio = true;
						}
						else if ((idEstagio > estagio_final) && (pre_estudo) && (numero_total_estados > 0))
							considera_estagio = true;

						if (considera_estagio) {
							for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= a_modeloOtimizacao.getMaiorId(idEstagio, IdVariavelEstado()); idVariavelEstado++) {
								if (a_modeloOtimizacao.isVariavelEstadoInstanciada(idEstagio, idVariavelEstado)) {
									for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {
										const int c = int(idCenario - cenario_inicial);

										const double valor = estados_all.vetor[estados_all.displ[rank] + e * numero_cenarios + c];

										a_modeloOtimizacao.addValorVariavelEstado(idEstagio, idVariavelEstado, idProcesso, a_maior_processo, idCenario, valor);

									} // for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {
									e++;
								} // if (a_modeloOtimizacao.isVariavelEstadoInstanciada(idEstagio, idVariavelEstado)) {
							} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= a_modeloOtimizacao.getMaiorId(idEstagio, IdVariavelEstado()); idVariavelEstado++) {
						} // if (considera_estagio){
					} // for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {
				} // if (numero_cenarios > 0) {
			} // if (idProcesso != a_idProcesso) {
		} // for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) {

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_distribuirEstadosEntreProcessos(" + getFullString(a_idProcesso) + "," + getFullString(a_maior_processo) + "," + ",a_valor_estados,a_modeloOtimizacao): \n" + std::string(erro.what())); }

} // void MetodoSolucao::executarPDDE_distribuirEstadosEntreProcessos(const IdProcesso a_idProcesso, const IdProcesso a_maior_processo, const SmartEnupla<IdCenario, SmartEnupla<IdVariavelEstado, double>>& a_estados, ModeloOtimizacao& a_modeloOtimizacao){


void MetodoSolucao::executarPDDE_distribuirRealizacoesEntreProcessos(const IdProcesso a_idProcesso, const IdProcesso a_maior_processo, const IdIteracao a_idIteracao, ModeloOtimizacao& a_modeloOtimizacao){

	try {

		if ((a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_tipo_relaxacao_afluencia_incremental, TipoRelaxacaoAfluenciaIncremental()) == TipoRelaxacaoAfluenciaIncremental_truncamento) || \
			(a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_tipo_relaxacao_afluencia_incremental, TipoRelaxacaoAfluenciaIncremental()) == TipoRelaxacaoAfluenciaIncremental_viabilidade_hidraulica_truncamento) || \
			(a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_tipo_relaxacao_afluencia_incremental, TipoRelaxacaoAfluenciaIncremental()) == TipoRelaxacaoAfluenciaIncremental_viabilidade_hidraulica_penalizacao)) {}
		else
			return;

		const IdProcessoEstocastico idProcessoEstocastico = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

		const IdVariavelAleatoria maiorIdVariavelAleatoria = a_modeloOtimizacao.getMaiorId(idProcessoEstocastico, IdVariavelAleatoria());

		const SmartEnupla<Periodo, double> periodos = a_modeloOtimizacao.realizacoes.at(idProcessoEstocastico).at(a_idProcesso).at(IdVariavelAleatoria_1).at(a_modeloOtimizacao.realizacoes.at(idProcessoEstocastico).at(a_idProcesso).at(IdVariavelAleatoria_1).getIteradorInicial());

		if (true) {

			int numero_periodos = 0;

			const IdEstagio estagio_inicial = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
			const IdEstagio estagio_final = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

			for (Periodo periodo = periodos.getIteradorInicial(); periodo <= periodos.getIteradorFinal(); periodos.incrementarIterador(periodo)) {
				for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {
					const Periodo periodo_otimizacao = a_modeloOtimizacao.getAtributo(idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());
					if (periodo_otimizacao == periodo) {
						if (a_modeloOtimizacao.getElementoVetor(a_idIteracao, AttVetorIteracao_tipo_processamento_paralelo, idEstagio, TipoProcessamentoParalelo()) == TipoProcessamentoParalelo_por_abertura)
							numero_periodos++;
						break;
					} // if (periodo_otimizacao == periodo) {
				} // for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {		
			} // for (Periodo periodo = periodos.getIteradorInicial(); periodo <= periodos.getIteradorFinal(); periodos.incrementarIterador(periodo)) {

			if (numero_periodos == 0)
				return;

		} // if (true) {

		const int numero_periodos = periodos.size();

		const int numero_processos = int(a_maior_processo);
		const int numero_variaveis = int(maiorIdVariavelAleatoria);

		EstruturaResultados<double> realizacoes(1);
		EstruturaResultados<double> realizacoes_all(numero_processos);

		for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maior_processo; idProcesso++) {
			const int rank = getRank(idProcesso);

			const IdCenario cenario_inicial = a_modeloOtimizacao.getCenarioInicial(idProcesso, a_idIteracao);
			const IdCenario cenario_final = a_modeloOtimizacao.getCenarioFinal(idProcesso, a_idIteracao);

			int numero_cenarios = int(cenario_final - cenario_inicial) + 1;
			if ((cenario_inicial == IdCenario_Nenhum) && (cenario_final == IdCenario_Nenhum))
				numero_cenarios = 0;

			realizacoes_all.setSize(rank, numero_variaveis, numero_cenarios, numero_periodos);

			if (idProcesso == a_idProcesso) {

				realizacoes.setSize(0, numero_variaveis, numero_cenarios, numero_periodos);

				if (realizacoes.total > 0) {
					int i = 0;
					for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= maiorIdVariavelAleatoria; idVariavelAleatoria++) {
						for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {
							for (Periodo periodo = periodos.getIteradorInicial(); periodo <= periodos.getIteradorFinal(); periodos.incrementarIterador(periodo)) {
								realizacoes.vetor[i] = a_modeloOtimizacao.realizacoes.at(idProcessoEstocastico).at(idProcesso).at(idVariavelAleatoria).at(idCenario).at(periodo);
								i++;
							}
						}
					}
				} // if (realizacoes.total > 0) {
			} // if (idProcesso == a_idProcesso) {

		} // for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maior_processo; idProcesso++) {

		MPI_Allgatherv(realizacoes.vetor, realizacoes.total, MPI_DOUBLE, realizacoes_all.vetor, realizacoes_all.sizeT, realizacoes_all.displ, MPI_DOUBLE, MPI_COMM_WORLD);

		realizacoes.esvaziar();

		for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maior_processo; idProcesso++) {

			if (idProcesso != a_idProcesso) {

				const int rank = getRank(idProcesso);

				const IdCenario cenario_inicial = a_modeloOtimizacao.getCenarioInicial(idProcesso, a_idIteracao);
				const IdCenario cenario_final = a_modeloOtimizacao.getCenarioFinal(idProcesso, a_idIteracao);

				int numero_cenarios = int(cenario_final - cenario_inicial) + 1;
				if ((cenario_inicial == IdCenario_Nenhum) && (cenario_final == IdCenario_Nenhum))
					numero_cenarios = 0;

				if (numero_cenarios > 0) {

					a_modeloOtimizacao.realizacoes.at(idProcessoEstocastico).at(idProcesso) = SmartEnupla<IdVariavelAleatoria, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>>(IdVariavelAleatoria_1, std::vector<SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>>(maiorIdVariavelAleatoria, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>()));

					for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= maiorIdVariavelAleatoria; idVariavelAleatoria++) {

						a_modeloOtimizacao.realizacoes.at(idProcessoEstocastico).at(idProcesso).at(idVariavelAleatoria) = SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>(cenario_inicial, std::vector<SmartEnupla<Periodo, double>>(numero_cenarios, SmartEnupla<Periodo, double>(periodos, 0.0)));

						const int v = int(idVariavelAleatoria) - 1;

						for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {
							const int c = int(idCenario - cenario_inicial);

							int p = 0;
							for (Periodo periodo = periodos.getIteradorInicial(); periodo <= periodos.getIteradorFinal(); periodos.incrementarIterador(periodo)) {

								const double valor = realizacoes_all.vetor[realizacoes_all.displ[rank] + v * numero_cenarios * numero_periodos + c * numero_periodos + p];

								a_modeloOtimizacao.realizacoes.at(idProcessoEstocastico).at(idProcesso).at(idVariavelAleatoria).at(idCenario).at(periodo) = valor;

								p++;

							} // for (Periodo periodo = periodos.getIteradorInicial(); periodo <= periodos.getIteradorFinal(); periodos.incrementarIterador(periodo)) {

						} // for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {
					} // for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= maiorIdVariavelAleatoria; idVariavelAleatoria++) {
				} // if (numero_cenarios > 0) {
			} // if (idProcesso != a_idProcesso) {
		} // for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) {

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_distribuirRealizacoesEntreProcessos(" + getFullString(a_idProcesso) + "," + getFullString(a_maior_processo) + ",a_valor_estados,a_modeloOtimizacao): \n" + std::string(erro.what())); }

}


void MetodoSolucao::executarPDDE_operacao_final(EntradaSaidaDados a_entradaSaidaDados, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, ModeloOtimizacao& a_modeloOtimizacao){

	try{

		const IdIteracao iteracao_final = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_iteracao_final, IdIteracao());

		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida();

		const TipoSubproblemaSolver tSS = TipoSubproblemaSolver_geral;

		for (IdIteracao idIteracao = IdIteracao(iteracao_final + 1); idIteracao >= a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_iteracao_inicial, IdIteracao()); idIteracao--) {

			const std::string diretorio_FW = diretorio + "//" + getFullString(idIteracao) + "//Foward//Resultados";

			a_entradaSaidaDados.setDiretorioSaida(diretorio_FW);
			a_modeloOtimizacao.consolidarResultados(a_idProcesso, a_maiorIdProcesso, a_entradaSaidaDados);

			const std::string diretorio_BW = diretorio + "//" + getFullString(idIteracao) + "//Backward//Resultados";
			a_entradaSaidaDados.setDiretorioSaida(diretorio_BW);
			a_modeloOtimizacao.consolidarResultados(a_idProcesso, a_maiorIdProcesso, a_entradaSaidaDados);

		} // for (IdIteracao idIteracao = IdIteracao(iteracao_final + 1); idIteracao >= a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_iteracao_inicial,  IdIteracao()); idIteracao--) {


	} // try {
	catch (const std::exception & erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_operacao_final(" + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + ",a_modeloOtimizacao): \n" + std::string(erro.what())); }

} // void MetodoSolucao::executarPDDE_operacao_final(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados){




void MetodoSolucao::executarPDDE_imprimirEstados(EntradaSaidaDados a_entradaSaidaDados, const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, ModeloOtimizacao & a_modeloOtimizacao){

	try {

		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//Estados");

		if (a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_imprimir_estado_por_iteracao, bool()))
			a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("VariaveisEstado_valores_" + getFullString(a_idProcesso) + ".csv", IdEstagio_Nenhum, IdVariavelEstado_Nenhum,     a_modeloOtimizacao, AttMatrizVariavelEstado_valor);

	} // try {
	catch (const std::exception&erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_imprimirEstados(a_entradaSaidaDados," + getFullString(a_idIteracao) + "," + getFullString(a_idProcesso) + ",a_modeloOtimizacao): \n" + std::string(erro.what())); }

} //void MetodoSolucao::executarPDDE_imprimirResultados(EntradaSaidaDados a_entradaSaidaDados, const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, ModeloOtimizacao & a_modeloOtimizacao){





