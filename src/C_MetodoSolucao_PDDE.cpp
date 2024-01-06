#include "C_MetodoSolucao.h"

#include "C_EntradaSaidaDados.h"

#include <chrono>

#include <algorithm>

#include "mpi.h"

void MetodoSolucao::executarPDDE(EntradaSaidaDados a_entradaSaidaDados, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, ModeloOtimizacao &a_modeloOtimizacao){

	try {

		const IdEstagio estagio_inicial = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
		const IdEstagio estagio_final = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

		const IdIteracao iteracao_inicial       = a_modeloOtimizacao.arranjoResolucao.getAtributo(AttComumArranjoResolucao_iteracao_inicial,  IdIteracao());
		const IdIteracao iteracao_final         = a_modeloOtimizacao.arranjoResolucao.getAtributo(AttComumArranjoResolucao_iteracao_final,    IdIteracao());
		const IdIteracao iteracao_numero_maximo = a_modeloOtimizacao.arranjoResolucao.getAtributo(AttComumArranjoResolucao_iteracao_numero_maximo,    IdIteracao());
		
		if (a_idProcesso == IdProcesso_mestre) {
			std::cout << std::endl;
			std::cout << "Metodo Solucao Iniciado" << std::endl;
		}

		for (IdIteracao idIteracao = iteracao_inicial; idIteracao <= iteracao_final; idIteracao++) {

			bool simulacao = false;

			if (idIteracao == IdIteracao_0)
				simulacao = true;

			if ((iteracao_numero_maximo < idIteracao) || (IdIteracao_0 == iteracao_final)) {
				simulacao = true;
				a_modeloOtimizacao.setAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario, true);
				a_modeloOtimizacao.setAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, true);
			} // if (iteracao_numero_maximo < idIteracao){

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

		const IdIteracao iteracao_inicial = a_modeloOtimizacao.arranjoResolucao.getMenorId(IdIteracao());

		if ((a_idIteracao == iteracao_inicial) && (a_idProcesso == IdProcesso_mestre)) {

			a_modeloOtimizacao.imprimirEstagios(a_entradaSaidaDados);

			a_modeloOtimizacao.imprimirVariaveisEstado(a_entradaSaidaDados);

			a_modeloOtimizacao.imprimirVariaveisRealizacao(a_entradaSaidaDados);

			//a_modeloOtimizacao.imprimirRestricoesCenario(a_entradaSaidaDados);

			a_modeloOtimizacao.imprimirVariaveisRealizacaoInterna(a_entradaSaidaDados);

			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getString(AttVetorEstagio_selecao_solucao_proxy) + ".csv", IdEstagio_Nenhum, a_modeloOtimizacao, AttVetorEstagio_selecao_solucao_proxy);

		} // if ((a_idIteracao == iteracao_inicial) && (a_idProcesso == IdProcesso_mestre)) {

		if (a_idIteracao == iteracao_inicial)
			a_modeloOtimizacao.importarCorteBenders_AcoplamentoPosEstudo(tSS, a_idProcesso, IdIteracao_Nenhum, diretorio_selecao_corte, a_entradaSaidaDados);

		const IdCenario menor_cenario = a_modeloOtimizacao.arranjoResolucao.getAtributo(a_idIteracao, a_idProcesso, AttComumProcesso_menor_cenario, IdCenario());
		const IdCenario maior_cenario = a_modeloOtimizacao.arranjoResolucao.getAtributo(a_idIteracao, a_idProcesso, AttComumProcesso_maior_cenario, IdCenario());

		int numero_cenarios = 0;
		if (menor_cenario != IdCenario_Nenhum)
			numero_cenarios = int(maior_cenario - menor_cenario) + 1;

		SmartEnupla<IdEstagio, std::vector<double>> custo_inferior(a_estagio_inicial, std::vector<std::vector<double>>(1, std::vector<double>()));
		SmartEnupla<IdEstagio, std::vector<double>> custo_inferior_problema_mestre(a_estagio_inicial, std::vector<std::vector<double>>(1, std::vector<double>()));
		custo_inferior.at(a_estagio_inicial).reserve(numero_cenarios);
		custo_inferior_problema_mestre.at(a_estagio_inicial).reserve(numero_cenarios);

		SmartEnupla<IdEstagio, std::vector<double>> custo_superior(a_estagio_inicial, std::vector<std::vector<double>>(int(a_estagio_final - a_estagio_inicial) + 1, std::vector<double>()));
		SmartEnupla<IdEstagio, std::vector<double>> custo_superior_problema_mestre(a_estagio_inicial, std::vector<std::vector<double>>(int(a_estagio_final - a_estagio_inicial) + 1, std::vector<double>()));

		const double multiplicador_tempo_limite_resolucao = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_multiplicador_tempo_limite_resolucao, double());

		double tempo_medio_solver_bw = 1000;
		if ((a_idIteracao > IdIteracao(iteracao_inicial + 1)) || ((a_idIteracao > iteracao_inicial) && iteracao_inicial > IdIteracao_0))
			tempo_medio_solver_bw = getElementoVetor(AttVetorMetodoSolucao_tempo_medio_solver_bw, IdIteracao(a_idIteracao - 1), double());

		const double tempo_limite = multiplicador_tempo_limite_resolucao * tempo_medio_solver_bw;

		a_entradaSaidaDados.setDiretorioSaida(diretorio_iteracao);
		a_modeloOtimizacao.gerarRealizacoes(a_idIteracao, a_idProcesso, a_entradaSaidaDados);
		a_entradaSaidaDados.setDiretorioSaida(diretorio);

		double cont_tempo_otimizacao = 0.0;
		int cont_numero_otimizacao = 0;
		for (IdEstagio idEstagio = a_estagio_inicial; idEstagio <= a_estagio_final; idEstagio++) {

			const IdEstagio idEstagio_seguinte = IdEstagio(idEstagio + 1);

			executarPDDE_distribuirEstadosEntreProcessos(a_idIteracao, idEstagio, a_modeloOtimizacao);

			if (idEstagio < a_estagio_final)
				a_modeloOtimizacao.alocarVariaveisEstado(a_idIteracao, idEstagio_seguinte);

			if (numero_cenarios > 0) {

				custo_superior.at(idEstagio).reserve(numero_cenarios);
				custo_superior_problema_mestre.at(idEstagio).reserve(numero_cenarios);


				//if ((idEstagio == a_estagio_inicial) && (a_estagio_inicial > IdEstagio_1))
					//a_modeloOtimizacao.importarVariaveisEstado_AcoplamentoPreEstudo(a_idProcesso, a_maiorIdProcesso, a_idIteracao, menor_cenario, maior_cenario, a_entradaSaidaDados);

				a_modeloOtimizacao.setTempoLimiteOtimizacao(tSS, idEstagio, -1.0);

				std::vector<std::vector<std::string>> valores_tempo(2, std::vector<std::string>());
				std::vector<std::vector<std::string>> valores_tempo_otimizacao(2, std::vector<std::string>());
				std::vector<std::vector<std::string>> valores_tratamento(2, std::vector<std::string>());

				if (imprimir_tempos) {
					valores_tempo.at(1).reserve(numero_cenarios + 1);
					valores_tempo.at(1).push_back(getFullString(idEstagio) + ";" + getFullString(a_idProcesso) + ";");

					valores_tempo_otimizacao.at(1).reserve(numero_cenarios + 1);
					valores_tempo_otimizacao.at(1).push_back(getFullString(idEstagio) + ";" + getFullString(a_idProcesso) + ";");

					valores_tratamento.at(1).reserve(numero_cenarios + 1);
					valores_tratamento.at(1).push_back(getFullString(idEstagio) + ";" + getFullString(a_idProcesso) + ";");
				} // if (imprimir_tempos) {

				bool algum_cenario = false;
				for (int c = 0; c < numero_cenarios; c++) {

					const IdCenario idCenario = IdCenario(menor_cenario + c);

					const IdCenario idCenario_estado = a_modeloOtimizacao.arranjoResolucao.getElementoMatriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_cenario_estado_por_cenario, idCenario, idEstagio, IdCenario());

					if (idCenario_estado != IdCenario_Nenhum) {

						try {

							auto start_clock_cenario = std::chrono::high_resolution_clock::now();

							a_modeloOtimizacao.atualizarModeloOtimizacaoComVariavelEstado(idEstagio, idCenario_estado);

							a_modeloOtimizacao.atualizarModeloOtimizacaoComVariavelRealizacao(idEstagio, idCenario);

							a_modeloOtimizacao.atualizarModeloOtimizacaoComRestricaoCenario(idEstagio, idCenario);

							a_modeloOtimizacao.otimizarProblema(tSS, a_idProcesso, a_idIteracao, idEstagio, idCenario, diretorio_pl);

							if (idEstagio == a_estagio_inicial)
								custo_inferior.at(idEstagio).push_back(a_modeloOtimizacao.getCustoTotal(idEstagio));

							if (idEstagio < a_estagio_final)
								custo_superior.at(idEstagio).push_back(a_modeloOtimizacao.getCustoImediato(idEstagio));
							else
								custo_superior.at(idEstagio).push_back(a_modeloOtimizacao.getCustoTotal(idEstagio));

							if (true) {
								double custo_geral = 0.0;
								double custo_individual = 0.0;
								a_modeloOtimizacao.calcularCustoPrimalViaSubproblemaMestre(TipoSubproblemaSolver_mestre, a_idIteracao, idEstagio, idCenario, diretorio_pl, custo_geral, custo_individual);

								if (idEstagio == a_estagio_inicial)
									custo_inferior_problema_mestre.at(idEstagio).push_back(custo_individual);

								custo_superior_problema_mestre.at(idEstagio).push_back(custo_individual);

								if (idEstagio > a_estagio_inicial)
									custo_superior_problema_mestre.at(idEstagio).at(custo_superior_problema_mestre.at(idEstagio).size() - 1) -= custo_geral;

							}

							if (idEstagio < a_estagio_final)
								a_modeloOtimizacao.atualizarVariavelEstadoComSolucaoPrimal(a_idIteracao, idEstagio_seguinte, idCenario);

							auto stop_clock_cenario = std::chrono::high_resolution_clock::now();

							std::chrono::duration<double> tempo_cenario = stop_clock_cenario - start_clock_cenario;

							const double tempo_otimizacao = a_modeloOtimizacao.getTempoOtimizacao(tSS, idEstagio);

							cont_tempo_otimizacao += tempo_otimizacao;

							cont_numero_otimizacao++;

							algum_cenario = true;

							if (imprimir_tempos) {
								valores_tempo.at(1).push_back(getString(tempo_cenario.count()));
								valores_tempo_otimizacao.at(1).push_back(getString(tempo_otimizacao));
								valores_tratamento.at(1).push_back(getString(a_modeloOtimizacao.getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, idEstagio, int())));
							} // if (imprimir_tempos) {

						} // try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro no problema de " + getFullString(idEstagio) + " " + getFullString(idCenario) + ". \n" + std::string(erro.what())); }

						if (idEstagio == a_estagio_inicial) {
							if (custo_inferior_problema_mestre.at(idEstagio).at(custo_inferior_problema_mestre.at(idEstagio).size() - 1) > custo_inferior.at(idEstagio).at(custo_inferior.at(idEstagio).size() - 1))
								custo_inferior.at(idEstagio).at(custo_inferior.at(idEstagio).size() - 1) = custo_inferior_problema_mestre.at(idEstagio).at(custo_inferior_problema_mestre.at(idEstagio).size() - 1);
						}
						if (custo_superior_problema_mestre.at(idEstagio).at(custo_superior_problema_mestre.at(idEstagio).size() - 1) > custo_superior.at(idEstagio).at(custo_superior.at(idEstagio).size() - 1))
							custo_superior.at(idEstagio).at(custo_superior.at(idEstagio).size() - 1) = custo_superior_problema_mestre.at(idEstagio).at(custo_superior_problema_mestre.at(idEstagio).size() - 1);

					} // if (idCenario_estado != IdCenario_Nenhum) {
				} // for (int c = 0; c < numero_cenarios; c++) {

				if (imprimir_tempos) {
					a_entradaSaidaDados.setDiretorioSaida(diretorio_tempos);
					a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(getFullString(a_idProcesso) + "_tempos_cenario.csv", valores_tempo);
					a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(getFullString(a_idProcesso) + "_tempos_otimizacao_cenario.csv", valores_tempo_otimizacao);
					a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(getFullString(a_idProcesso) + "_tratamento_inviabilidade.csv", valores_tratamento);
				} // if (imprimir_tempos) {

				a_entradaSaidaDados.setDiretorioSaida(diretorio_resultado);

				if (idEstagio == a_estagio_inicial)
					a_entradaSaidaDados.setAppendArquivo(false);
				else
					a_entradaSaidaDados.setAppendArquivo(true);

				if (!algum_cenario)
					a_modeloOtimizacao.armazenarValoresSolver(tSS, a_idIteracao, idEstagio, IdCenario_Nenhum, IdRealizacao_Nenhum);

				a_modeloOtimizacao.imprimirSolucaoPorEstagioPorCenario_porEstagio(a_idProcesso, "", a_entradaSaidaDados);

			} // if (numero_cenarios > 0) {

		} // for (IdEstagio idEstagio = a_estagio_inicial; idEstagio <= a_estagio_final; idEstagio++) {


		executarPDDE_atualizarCustoInferior(a_idIteracao, custo_inferior, a_modeloOtimizacao);
		executarPDDE_atualizarCustoSuperior(a_idIteracao, custo_superior, a_modeloOtimizacao);

		a_entradaSaidaDados.setAppendArquivo(false);
		a_entradaSaidaDados.setDiretorioSaida(diretorio_resultado);

		int cont_numero_otimizacao_global;
		MPI_Allreduce(&cont_numero_otimizacao, &cont_numero_otimizacao_global, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

		double cont_tempo_otimizacao_global;
		MPI_Allreduce(&cont_tempo_otimizacao, &cont_tempo_otimizacao_global, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

		addElemento(AttVetorMetodoSolucao_tempo_medio_solver_fw, a_idIteracao, cont_tempo_otimizacao_global / cont_numero_otimizacao_global);

		auto stop_clock = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> tempo = stop_clock - start_clock;

		if (a_idProcesso == IdProcesso_mestre)
			incrementarTempoExecucao(a_idIteracao, tempo.count() / 60.0);

		a_entradaSaidaDados.setDiretorioSaida(diretorio_iteracao);
		executarPDDE_imprimirEstados(a_entradaSaidaDados, a_idIteracao, a_idProcesso, a_modeloOtimizacao);

		if (a_simulacao)
			return;

		a_entradaSaidaDados.setDiretorioSaida(diretorio);

		if (a_idProcesso == IdProcesso_mestre)
			a_modeloOtimizacao.exportarVariaveisEstado_AcoplamentoPosEstudo(a_idIteracao, a_entradaSaidaDados);

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

		const double multiplicador_tempo_limite_resolucao = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_multiplicador_tempo_limite_resolucao, double());

		const double tempo_medio_solver_fw = getElementoVetor(AttVetorMetodoSolucao_tempo_medio_solver_fw, a_idIteracao, double());

		const double tempo_limite = multiplicador_tempo_limite_resolucao * tempo_medio_solver_fw;

		a_modeloOtimizacao.importarCorteBenders_AcoplamentoPosEstudo(tSS, a_idProcesso, a_idIteracao, diretorio_selecao_corte, a_entradaSaidaDados);

		IdEstagio estagio_inicial = a_estagio_inicial;
		if (estagio_inicial == IdEstagio_1)
			estagio_inicial++;

		double cont_tempo_otimizacao = 0.0;
		int cont_numero_otimizacao = 0;
		for (IdEstagio idEstagio = a_estagio_final; idEstagio >= estagio_inicial; idEstagio--) {

			const std::vector<IdCenario> cenarios_estados = a_modeloOtimizacao.arranjoResolucao.getIdsCenarioEstadoFromAberturas(a_idProcesso, a_idIteracao, idEstagio);
			const int numero_cenarios_estados = int(cenarios_estados.size());

			const int numero_aberturas = a_modeloOtimizacao.arranjoResolucao.getNumeroAberturas(a_idProcesso, a_idIteracao, idEstagio);

			const int numero_estados = int(a_modeloOtimizacao.getMaiorId(idEstagio, IdVariavelEstado()));

			const int numero_variaveis_dinamicas = a_modeloOtimizacao.getNumeroVariavelDinamica(tSS, idEstagio);

			const int numero_processos = int(a_maiorIdProcesso);

			const int numero_aberturas_global = int(a_modeloOtimizacao.getAtributo(idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()));

			EstruturaResultados<int> numero_soma_custo_abertura(1);
			numero_soma_custo_abertura.setSize(0, numero_aberturas_global);
			EstruturaResultados<double> soma_custo_abertura(1);
			soma_custo_abertura.setSize(0, numero_aberturas_global);
			for (int a = 0; a < numero_aberturas_global; a++) {
				soma_custo_abertura.vetor.at(a) = 0.0;
				numero_soma_custo_abertura.vetor.at(a) = 0;
			}

			EstruturaResultados<int> status_otimizacao(1);
			status_otimizacao.setSize(0, numero_cenarios_estados, numero_aberturas);

			EstruturaResultados<double> custo_total(1);
			custo_total.setSize(0, numero_cenarios_estados, numero_aberturas);

			EstruturaResultados<double> sol_inf_var_dinamica(1);
			sol_inf_var_dinamica.setSize(0, numero_cenarios_estados, numero_aberturas);

			EstruturaResultados<double> solucao_dual_var_dinamica(1);
			solucao_dual_var_dinamica.setSize(0, numero_cenarios_estados, numero_aberturas, numero_variaveis_dinamicas);

			EstruturaResultados<double> limite_inferior_var_dinamica(1);
			limite_inferior_var_dinamica.setSize(0, numero_cenarios_estados, numero_aberturas, numero_variaveis_dinamicas);

			EstruturaResultados<double> limite_superior_var_dinamica(1);
			limite_superior_var_dinamica.setSize(0, numero_cenarios_estados, numero_aberturas, numero_variaveis_dinamicas);

			EstruturaResultados<double> vlr_var_estado(1);
			vlr_var_estado.setSize(0, numero_cenarios_estados, numero_estados);

			EstruturaResultados<double> sol_dual_var_estado(1);
			sol_dual_var_estado.setSize(0, numero_cenarios_estados, numero_aberturas, numero_estados);

			// Identificação das aberturas com solução proxy a partir do custo médio do estágio
			if (true) {
				SmartEnupla<IdRealizacao, int> realizacao_solucao_proxy(IdRealizacao_1, std::vector<int>(a_modeloOtimizacao.getAtributo(idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()), 0));
				const std::vector<int> odernacao_custo_medio = getOrdenacaoMaiorParaMenor(a_modeloOtimizacao.getElementosMatriz(AttMatrizModeloOtimizacao_custo_medio, idEstagio, IdRealizacao(), double()).getElementos());
				for (int i = int(odernacao_custo_medio.size()); i > 0; i--) {
					if (a_modeloOtimizacao.getElementoVetor(idEstagio, AttVetorEstagio_selecao_solucao_proxy, i, int()) == 1)
						realizacao_solucao_proxy.setElemento(IdRealizacao(odernacao_custo_medio.at(i - 1) + 1), 1);
				}
				a_modeloOtimizacao.setElementos(AttMatrizModeloOtimizacao_realizacao_solucao_proxy, idEstagio, realizacao_solucao_proxy);
			} // if (true) {

			for (int c = 0; c < int(cenarios_estados.size()); c++) {

				const IdCenario idCenario_estado = cenarios_estados.at(c);

				const IdAbertura menor_abertura_em_cenario_estado = a_modeloOtimizacao.arranjoResolucao.getElementoMatriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_menor_abertura_por_cenario_estado, idCenario_estado, idEstagio, IdAbertura());
				const IdAbertura maior_abertura_em_cenario_estado = a_modeloOtimizacao.arranjoResolucao.getElementoMatriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_maior_abertura_por_cenario_estado, idCenario_estado, idEstagio, IdAbertura());

				if (tempo_limite > 0)
					a_modeloOtimizacao.setTempoLimiteOtimizacao(tSS, idEstagio, tempo_limite);

				if ((idEstagio == a_estagio_final) && (idCenario_estado == cenarios_estados.at(0)))
					a_entradaSaidaDados.setAppendArquivo(false);
				else
					a_entradaSaidaDados.setAppendArquivo(true);

				std::vector<std::vector<std::string>> valores_tempo(2, std::vector<std::string>());
				std::vector<std::vector<std::string>> valores_tempo_otimizacao(2, std::vector<std::string>());
				std::vector<std::vector<std::string>> valores_tratamento(2, std::vector<std::string>());

				if (imprimir_tempos) {
					valores_tempo.at(1).reserve(int(numero_aberturas) + 1);
					valores_tempo.at(1).push_back(getFullString(idEstagio) + ";" + getFullString(a_idProcesso) + ";" + getFullString(idCenario_estado) + ";");

					valores_tempo_otimizacao.at(1).reserve(int(numero_aberturas) + 1);
					valores_tempo_otimizacao.at(1).push_back(getFullString(idEstagio) + ";" + getFullString(a_idProcesso) + ";" + getFullString(idCenario_estado) + ";");

					valores_tratamento.at(1).reserve(int(numero_aberturas));
					valores_tratamento.at(1).push_back(getFullString(idEstagio) + ";" + getFullString(a_idProcesso) + ";" + getFullString(idCenario_estado) + ";");
				} // if (imprimir_tempos) {

				auto start_clock_cenario = std::chrono::high_resolution_clock::now();

				a_modeloOtimizacao.atualizarModeloOtimizacaoComVariavelEstado(idEstagio, idCenario_estado, vlr_var_estado);

				for (IdAbertura idAbertura = menor_abertura_em_cenario_estado; idAbertura <= maior_abertura_em_cenario_estado; idAbertura++) {

					try {

						auto start_clock_realizacao = std::chrono::high_resolution_clock::now();

						a_modeloOtimizacao.atualizarModeloOtimizacaoComVariavelRealizacao(idEstagio, IdRealizacao(idAbertura));

						a_modeloOtimizacao.atualizarModeloOtimizacaoComRestricaoCenario(idEstagio, idCenario_estado, IdRealizacao(idAbertura));

						const bool otimizacao = a_modeloOtimizacao.otimizarProblema(tSS, a_idProcesso, a_idIteracao, idEstagio, idCenario_estado, IdRealizacao(idAbertura), sol_inf_var_dinamica, solucao_dual_var_dinamica, limite_inferior_var_dinamica, limite_superior_var_dinamica, sol_dual_var_estado, diretorio_pl);

						const double custo_total_otimizacao = a_modeloOtimizacao.getCustoTotal(idEstagio);

						soma_custo_abertura.vetor.at(int(idAbertura) - 1) += custo_total_otimizacao;
						numero_soma_custo_abertura.vetor.at(int(idAbertura) - 1)++;

						const double tempo_otimizacao = a_modeloOtimizacao.getTempoOtimizacao(tSS, idEstagio);

						custo_total.add(custo_total_otimizacao);

						if (otimizacao) {
							cont_tempo_otimizacao += tempo_otimizacao;
							cont_numero_otimizacao++;
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

					} // try {
					catch (const std::exception& erro) { throw std::invalid_argument("Erro no problema de " + getFullString(a_idProcesso) + " " + getFullString(idEstagio) + " " + getFullString(idCenario_estado) + " " + getFullString(idAbertura) + ". \n" + std::string(erro.what())); }

				} // for (IdAbertura idAbertura = menor_abertura_em_cenario_estado; idAbertura <= maior_abertura_em_cenario_estado; idAbertura++) {

				if (imprimir_tempos) {

					auto stop_clock_cenario = std::chrono::high_resolution_clock::now();

					std::chrono::duration<double> tempo_cenario = stop_clock_cenario - start_clock_cenario;

					valores_tempo.at(1).push_back(";" + getString(tempo_cenario.count()));

					a_entradaSaidaDados.setDiretorioSaida(diretorio_tempos);
					a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(getFullString(a_idProcesso) + "_tempos_cenario.csv", valores_tempo);
					a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(getFullString(a_idProcesso) + "_tempos_otimizacao_cenario.csv", valores_tempo_otimizacao);
					a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(getFullString(a_idProcesso) + "_tratamento_inviabilidade.csv", valores_tratamento);

				} // if (imprimir_tempos) {

			} // for (int c = 0; c < int(cenarios_estados.size()); c++) {

			auto start_clock_corte = std::chrono::high_resolution_clock::now();

			if ((idEstagio > IdEstagio_1) || (a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_calcular_custo_primal_via_subproblema_mestre, bool())))
				executarPDDE_calcularCorteBenders_new(tSS, a_idIteracao, idEstagio, status_otimizacao, custo_total, sol_inf_var_dinamica, solucao_dual_var_dinamica, limite_inferior_var_dinamica, limite_superior_var_dinamica, sol_dual_var_estado, vlr_var_estado, a_modeloOtimizacao);

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
			soma_custo_abertura_all.setSize(0, numero_aberturas_global);
			MPI_Allreduce(&soma_custo_abertura.vetor[0], &soma_custo_abertura_all.vetor[0], soma_custo_abertura.total, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

			EstruturaResultados<int> numero_soma_custo_abertura_all(1);
			numero_soma_custo_abertura_all.setSize(0, numero_aberturas_global);
			MPI_Allreduce(&numero_soma_custo_abertura.vetor[0], &numero_soma_custo_abertura_all.vetor[0], numero_soma_custo_abertura.total, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

			for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= IdRealizacao(numero_aberturas_global); idRealizacao++)
				a_modeloOtimizacao.setElemento(AttMatrizModeloOtimizacao_custo_medio, idEstagio, idRealizacao, soma_custo_abertura_all.vetor.at(int(idRealizacao) - 1) / double(numero_soma_custo_abertura_all.vetor.at(int(idRealizacao) - 1)));

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

		for (IdEstagio idEstagio = a_estagio_final; idEstagio >= estagio_inicial; idEstagio--) {

			if (a_idProcesso == IdProcesso_mestre) {
				if ((a_idIteracao == a_modeloOtimizacao.arranjoResolucao.getAtributo(AttComumArranjoResolucao_iteracao_final, IdIteracao())) || (a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_imprimir_corte_por_iteracao, bool()))) {
					a_entradaSaidaDados.setAppendArquivo(false);
					a_entradaSaidaDados.setDiretorioSaida(diretorio_iteracao);
					a_modeloOtimizacao.exportarCorteBenders(a_idProcesso, idEstagio, a_entradaSaidaDados);
				}
				if (idEstagio == a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_estagio_acoplamento_pre_estudo, IdEstagio())) {
					a_entradaSaidaDados.setAppendArquivo(false);
					a_entradaSaidaDados.setDiretorioSaida(diretorio);
					a_modeloOtimizacao.exportarCorteBenders_AcoplamentoPreEstudo(a_idIteracao, a_entradaSaidaDados);
				}

				if (a_idIteracao == a_modeloOtimizacao.arranjoResolucao.getAtributo(AttComumArranjoResolucao_iteracao_final, IdIteracao())) {
					a_entradaSaidaDados.setAppendArquivo(false);
					a_entradaSaidaDados.setDiretorioSaida(diretorio_iteracao);
					a_modeloOtimizacao.exportarVersaoAlternativaCorteBenders(a_idProcesso, idEstagio, a_entradaSaidaDados);
				}

			} // if (a_idProcesso == IdProcesso_mestre) {

			if (a_idIteracao == a_modeloOtimizacao.arranjoResolucao.getAtributo(AttComumArranjoResolucao_iteracao_final, IdIteracao()))
				a_modeloOtimizacao.removerCorteBenders(idEstagio);

		} // for (IdEstagio idEstagio = a_estagio_final; idEstagio >= a_estagio_inicial; idEstagio--) {

		int cont_numero_otimizacao_global;
		MPI_Allreduce(&cont_numero_otimizacao, &cont_numero_otimizacao_global, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

		double cont_tempo_otimizacao_global;
		MPI_Allreduce(&cont_tempo_otimizacao, &cont_tempo_otimizacao_global, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

		addElemento(AttVetorMetodoSolucao_tempo_medio_solver_bw, a_idIteracao, cont_tempo_otimizacao_global / cont_numero_otimizacao_global);

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


void MetodoSolucao::executarPDDE_atualizarCustoSuperior(const IdIteracao a_idIteracao, const SmartEnupla<IdEstagio, std::vector<double>>& a_custo_superior, ModeloOtimizacao& a_modeloOtimizacao) {

	try {

		const IdEstagio menor_estagio = a_custo_superior.getIteradorInicial();
		const IdEstagio maior_estagio = a_custo_superior.getIteradorFinal();

		const IdProcesso idProcesso_local = a_modeloOtimizacao.arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());

		if (idProcesso_local == IdProcesso_mestre) {

			if (getSizeMatriz(AttMatrizMetodoSolucao_custo_superior) == 0)
				setMatriz_forced(AttMatrizMetodoSolucao_custo_superior, SmartEnupla<IdIteracao, SmartEnupla<IdCenario, double>>(a_idIteracao, std::vector<SmartEnupla<IdCenario, double>>(1, SmartEnupla<IdCenario, double>(IdCenario_1, std::vector<double>(a_modeloOtimizacao.arranjoResolucao.getAtributo(AttComumArranjoResolucao_maior_cenario, IdCenario()), 0.0)))));

			const IdCenario menor_cenario_iteracao = a_modeloOtimizacao.arranjoResolucao.getAtributo(a_idIteracao, AttComumIteracao_menor_cenario, IdCenario());
			const IdCenario maior_cenario_iteracao = a_modeloOtimizacao.arranjoResolucao.getAtributo(a_idIteracao, AttComumIteracao_maior_cenario, IdCenario());
			SmartEnupla<IdEstagio, SmartEnupla<IdCenario, double>> compilacao_custo_superior(menor_estagio, std::vector<SmartEnupla<IdCenario, double>>(int(maior_estagio - menor_estagio) + 1, SmartEnupla<IdCenario, double>()));

			for (IdEstagio idEstagio = menor_estagio; idEstagio <= maior_estagio; idEstagio++) {

				compilacao_custo_superior.at(idEstagio) = SmartEnupla<IdCenario, double>(menor_cenario_iteracao, std::vector<double>(int(maior_cenario_iteracao - menor_cenario_iteracao) + 1, NAN));

				if (a_custo_superior.size() > 0) {
					if (a_custo_superior.at(idEstagio).size() > 0){
						int c = 0;
						const IdCenario menor_cenario = a_modeloOtimizacao.arranjoResolucao.getIterador1Inicial(a_idIteracao, idProcesso_local, AttMatrizProcesso_cenario_estado_por_cenario, IdCenario());
						const IdCenario maior_cenario = a_modeloOtimizacao.arranjoResolucao.getIterador1Final(a_idIteracao, idProcesso_local, AttMatrizProcesso_cenario_estado_por_cenario, IdCenario());
						for (IdCenario idCenario = menor_cenario; idCenario <= maior_cenario; idCenario++) {
							if (a_modeloOtimizacao.arranjoResolucao.getElementoMatriz(a_idIteracao, idProcesso_local, AttMatrizProcesso_cenario_estado_por_cenario, idCenario, idEstagio, IdCenario()) != IdCenario_Nenhum) {
								compilacao_custo_superior.at(idEstagio).at(idCenario) = a_custo_superior.at(idEstagio).at(c);
								c++;
							}
						}

						if (c != int(a_custo_superior.at(idEstagio).size()))
							throw std::invalid_argument("Erro em " + getFullString(idProcesso_local));

					}
				} // if (a_custo_superior.size() > 0) {

				for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= a_modeloOtimizacao.arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++) {

					int numero_cenarios_outro = 0;
					MPI_Recv(&numero_cenarios_outro, 1, MPI_INT, getRank(idProcesso), getRank(idProcesso), MPI_COMM_WORLD, MPI_STATUS_IGNORE);

					if (numero_cenarios_outro > 0) {
						double* custo_superior_outro = new double[numero_cenarios_outro];

						MPI_Recv(custo_superior_outro, numero_cenarios_outro, MPI_DOUBLE, getRank(idProcesso), getRank(idProcesso), MPI_COMM_WORLD, MPI_STATUS_IGNORE);

						int c = 0;
						const IdCenario menor_cenario = a_modeloOtimizacao.arranjoResolucao.getIterador1Inicial(a_idIteracao, idProcesso, AttMatrizProcesso_cenario_estado_por_cenario, IdCenario());
						const IdCenario maior_cenario = a_modeloOtimizacao.arranjoResolucao.getIterador1Final(a_idIteracao, idProcesso, AttMatrizProcesso_cenario_estado_por_cenario, IdCenario());
						for (IdCenario idCenario = menor_cenario; idCenario <= maior_cenario; idCenario++) {
							if (a_modeloOtimizacao.arranjoResolucao.getElementoMatriz(a_idIteracao, idProcesso, AttMatrizProcesso_cenario_estado_por_cenario, idCenario, idEstagio, IdCenario()) != IdCenario_Nenhum) {
								compilacao_custo_superior.at(idEstagio).at(idCenario) = custo_superior_outro[c];
								c++;
							}
						}

						if (c != numero_cenarios_outro)
							throw std::invalid_argument("Erro em " + getFullString(idProcesso));

						delete[] custo_superior_outro;
					}
				} // for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= a_maiorIdProcesso; idProcesso++) {

			} // for (IdEstagio idEstagio = menor_estagio; idEstagio <= maior_estagio; idEstagio++) {

			for (IdCenario idCenario = menor_cenario_iteracao; idCenario <= maior_cenario_iteracao; idCenario++) {

				IdCenario idCenario_estado = IdCenario_Nenhum;

				for (IdProcesso idProcesso_cenario_estado = IdProcesso_mestre; idProcesso_cenario_estado <= a_modeloOtimizacao.arranjoResolucao.getMaiorId(IdProcesso()); idProcesso_cenario_estado++) {
					if (a_modeloOtimizacao.arranjoResolucao.getSize1Matriz(a_idIteracao, idProcesso_cenario_estado, AttMatrizProcesso_cenario_estado_por_cenario) > 0) {
						if ((a_modeloOtimizacao.arranjoResolucao.getIterador1Inicial(a_idIteracao, idProcesso_cenario_estado, AttMatrizProcesso_cenario_estado_por_cenario, IdCenario()) <= idCenario) && (idCenario <= a_modeloOtimizacao.arranjoResolucao.getIterador1Final(a_idIteracao, idProcesso_cenario_estado, AttMatrizProcesso_cenario_estado_por_cenario, IdCenario()))) {
							idCenario_estado = a_modeloOtimizacao.arranjoResolucao.getElementoMatriz(a_idIteracao, idProcesso_cenario_estado, AttMatrizProcesso_cenario_estado_por_cenario, idCenario, maior_estagio, IdCenario());
							break;
						}
					}
				}

				for (IdEstagio idEstagio = maior_estagio; idEstagio > menor_estagio; idEstagio--) {

					const IdEstagio idEstagio_anterior = IdEstagio(idEstagio - 1);

					if (idCenario_estado != IdCenario_Nenhum)
						compilacao_custo_superior.at(idEstagio_anterior).at(idCenario) = compilacao_custo_superior.at(idEstagio_anterior).at(idCenario_estado);
					else
						break;

					for (IdProcesso idProcesso_cenario_estado = IdProcesso_mestre; idProcesso_cenario_estado <= a_modeloOtimizacao.arranjoResolucao.getMaiorId(IdProcesso()); idProcesso_cenario_estado++) {
						if (a_modeloOtimizacao.arranjoResolucao.getSize1Matriz(a_idIteracao, idProcesso_cenario_estado, AttMatrizProcesso_cenario_estado_por_cenario) > 0) {
							if ((a_modeloOtimizacao.arranjoResolucao.getIterador1Inicial(a_idIteracao, idProcesso_cenario_estado, AttMatrizProcesso_cenario_estado_por_cenario, IdCenario()) <= idCenario_estado) && (idCenario_estado <= a_modeloOtimizacao.arranjoResolucao.getIterador1Final(a_idIteracao, idProcesso_cenario_estado, AttMatrizProcesso_cenario_estado_por_cenario, IdCenario()))) {
								idCenario_estado = a_modeloOtimizacao.arranjoResolucao.getElementoMatriz(a_idIteracao, idProcesso_cenario_estado, AttMatrizProcesso_cenario_estado_por_cenario, idCenario_estado, idEstagio_anterior, IdCenario());
								break;
							}
						}
					}

				} // for (IdEstagio idEstagio = maior_estagio; idEstagio > menor_estagio; idEstagio--) {

			} // for (IdCenario idCenario = menor_cenario_iteracao; idCenario <= maior_cenario_iteracao; idCenario++) {

			SmartEnupla<IdCenario, double> custo_superior(menor_cenario_iteracao, std::vector<double>(int(maior_cenario_iteracao - menor_cenario_iteracao) + 1, 0.0));

			for (IdCenario idCenario = menor_cenario_iteracao; idCenario <= maior_cenario_iteracao; idCenario++) {

				bool is_todos_nan = true;
				for (IdEstagio idEstagio = menor_estagio; idEstagio <= maior_estagio; idEstagio++) {
					custo_superior.at(idCenario) += compilacao_custo_superior.at(idEstagio).at(idCenario);
					if (!isnan(compilacao_custo_superior.at(idEstagio).at(idCenario)))
						is_todos_nan = false;
				}

				if (!isnan(custo_superior.at(idCenario)))
					setElemento(AttMatrizMetodoSolucao_custo_superior, a_idIteracao, idCenario, custo_superior.at(idCenario));
				else if (!is_todos_nan)
					throw std::invalid_argument("Erro na compilacao do custo superior do cenario.");

			}

			const double custo_superior_medio = getMedia_noNAN(custo_superior);
			addElemento(AttVetorMetodoSolucao_custo_superior, a_idIteracao, custo_superior_medio);

		} // if (a_idProcesso == IdProcesso_mestre) {

		else {

			for (IdEstagio idEstagio = menor_estagio; idEstagio <= maior_estagio; idEstagio++) {

				int numero_cenarios = 0;
				if (a_custo_superior.size() > 0)
					numero_cenarios = int(a_custo_superior.at(idEstagio).size());

				MPI_Send(&numero_cenarios, 1, MPI_INT, getRank(IdProcesso_mestre), getRank(idProcesso_local), MPI_COMM_WORLD);

				if (numero_cenarios > 0)
					MPI_Send(&a_custo_superior.at(idEstagio)[0], numero_cenarios, MPI_DOUBLE, getRank(IdProcesso_mestre), getRank(idProcesso_local), MPI_COMM_WORLD);

			} // for (IdEstagio idEstagio = menor_estagio; idEstagio <= maior_estagio; idEstagio++) {

		} // else {

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_atualizarCustoSuperior(" + getFullString(a_idIteracao) + ",a_custo_superior,a_probabilidade_cenario,a_modeloOtimizacao): \n" + std::string(erro.what())); }

} // void MetodoSolucao::executarPDDE_atualizarCustoSuperior(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso) {


void MetodoSolucao::executarPDDE_atualizarCustoInferior(const IdIteracao a_idIteracao, const SmartEnupla<IdEstagio, std::vector<double>>& a_custo_inferior, ModeloOtimizacao& a_modeloOtimizacao){

	try{

		const IdEstagio menor_estagio = a_custo_inferior.getIteradorInicial();

		const IdProcesso idProcesso_local = a_modeloOtimizacao.arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());

		if (idProcesso_local == IdProcesso_mestre) {

			if (getSizeMatriz(AttMatrizMetodoSolucao_custo_inferior) == 0)
				setMatriz_forced(AttMatrizMetodoSolucao_custo_inferior, SmartEnupla<IdIteracao, SmartEnupla<IdCenario, double>>(a_idIteracao, std::vector<SmartEnupla<IdCenario, double>>(1, SmartEnupla<IdCenario, double>(IdCenario_1, std::vector<double>(a_modeloOtimizacao.arranjoResolucao.getAtributo(AttComumArranjoResolucao_maior_cenario, IdCenario()), 0.0)))));

			const IdCenario menor_cenario_iteracao = a_modeloOtimizacao.arranjoResolucao.getAtributo(a_idIteracao, AttComumIteracao_menor_cenario, IdCenario());
			const IdCenario maior_cenario_iteracao = a_modeloOtimizacao.arranjoResolucao.getAtributo(a_idIteracao, AttComumIteracao_maior_cenario, IdCenario());

			SmartEnupla<IdCenario, double> custo_inferior(menor_cenario_iteracao, std::vector<double>(int(maior_cenario_iteracao - menor_cenario_iteracao) + 1, NAN));

			if (a_custo_inferior.size() > 0) {

				int c = 0;
				const IdCenario menor_cenario = a_modeloOtimizacao.arranjoResolucao.getIterador1Inicial(a_idIteracao, idProcesso_local, AttMatrizProcesso_cenario_estado_por_cenario, IdCenario());
				const IdCenario maior_cenario = a_modeloOtimizacao.arranjoResolucao.getIterador1Final(a_idIteracao, idProcesso_local, AttMatrizProcesso_cenario_estado_por_cenario, IdCenario());
				for (IdCenario idCenario = menor_cenario; idCenario <= maior_cenario; idCenario++) {
					const IdCenario idCenario_estado = a_modeloOtimizacao.arranjoResolucao.getElementoMatriz(a_idIteracao, idProcesso_local, AttMatrizProcesso_cenario_estado_por_cenario, idCenario, menor_estagio, IdCenario());
					if (idCenario_estado != IdCenario_Nenhum) {
						setElemento(AttMatrizMetodoSolucao_custo_inferior, a_idIteracao, idCenario, a_custo_inferior.at(menor_estagio).at(c));
						custo_inferior.at(idCenario) = a_custo_inferior.at(menor_estagio).at(c);
						c++;
					}
				}

				if (c != int(a_custo_inferior.size()))
					throw std::invalid_argument("Erro em " + getFullString(idProcesso_local));

			} // if (a_custo_inferior.size() > 0) {

			for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= a_modeloOtimizacao.arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++) {

				int numero_cenarios_outro = 0;
				MPI_Recv(&numero_cenarios_outro, 1, MPI_INT, getRank(idProcesso), getRank(idProcesso), MPI_COMM_WORLD, MPI_STATUS_IGNORE);

				if (numero_cenarios_outro > 0) {
					double* custo_inferior_outro = new double[numero_cenarios_outro];

					MPI_Recv(custo_inferior_outro, numero_cenarios_outro, MPI_DOUBLE, getRank(idProcesso), getRank(idProcesso), MPI_COMM_WORLD, MPI_STATUS_IGNORE);

					int c = 0;
					const IdCenario menor_cenario = a_modeloOtimizacao.arranjoResolucao.getIterador1Inicial(a_idIteracao, idProcesso, AttMatrizProcesso_cenario_estado_por_cenario, IdCenario());
					const IdCenario maior_cenario = a_modeloOtimizacao.arranjoResolucao.getIterador1Final(a_idIteracao, idProcesso, AttMatrizProcesso_cenario_estado_por_cenario, IdCenario());
					for (IdCenario idCenario = menor_cenario; idCenario <= maior_cenario; idCenario++) {
						if (a_modeloOtimizacao.arranjoResolucao.getElementoMatriz(a_idIteracao, idProcesso, AttMatrizProcesso_cenario_estado_por_cenario, idCenario, menor_estagio, IdCenario()) != IdCenario_Nenhum) {
							setElemento(AttMatrizMetodoSolucao_custo_inferior, a_idIteracao, idCenario, custo_inferior_outro[c]);
							custo_inferior.at(idCenario) = custo_inferior_outro[c];
							c++;
						}
					}

					if (c != numero_cenarios_outro)
						throw std::invalid_argument("Erro em " + getFullString(idProcesso));

					delete[] custo_inferior_outro;
				}

			} // for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= a_maiorIdProcesso; idProcesso++) {

			const double custo_inferior_medio = getMedia_noNAN(custo_inferior);
			addElemento(AttVetorMetodoSolucao_custo_inferior, a_idIteracao, custo_inferior_medio);

		} // if (a_idProcesso == IdProcesso_mestre) {

		else {

			int numero_cenarios = int(a_custo_inferior.at(menor_estagio).size());

			MPI_Send(&numero_cenarios, 1, MPI_INT, getRank(IdProcesso_mestre), getRank(idProcesso_local), MPI_COMM_WORLD);

			if (numero_cenarios > 0)
				MPI_Send(&a_custo_inferior.at(menor_estagio)[0], numero_cenarios, MPI_DOUBLE, getRank(IdProcesso_mestre), getRank(idProcesso_local), MPI_COMM_WORLD);

		} // else {

	} // try {
	catch (const std::exception&erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_atualizarCustoInferior(" + getFullString(a_idIteracao) + ",a_custo_inferior,a_modeloOtimizacao): \n" + std::string(erro.what())); }

} // void MetodoSolucao::executarPDDE_atualizarCustoInferior(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdRealizacao a_idRealizacao, const IdRealizacao a_maiorIdRealizacao, double a_custo_inferior){



void MetodoSolucao::executarPDDE_avaliarSolucao(EntradaSaidaDados a_entradaSaidaDados, const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, ModeloOtimizacao &a_modeloOtimizacao){

	try {

		if (a_idProcesso == IdProcesso_mestre) {

			const double tempo_execucao       = getElementoVetor(AttVetorMetodoSolucao_tempo_execucao, a_idIteracao, double());

			const double tempo_execucao_total = getAtributo(AttComumMetodoSolucao_tempo_execucao_total, double());
			addElemento(AttVetorMetodoSolucao_tempo_execucao_acumulado, a_idIteracao, tempo_execucao_total);

			const double custo_inferior = getElementoVetor(AttVetorMetodoSolucao_custo_inferior, a_idIteracao, double());

			double custo_superior = getElementoVetor(AttVetorMetodoSolucao_custo_superior, a_idIteracao, double());

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

			else if (a_idIteracao > a_modeloOtimizacao.arranjoResolucao.getAtributo(AttComumArranjoResolucao_iteracao_numero_maximo, IdIteracao())) {
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

			if (a_idIteracao > a_modeloOtimizacao.arranjoResolucao.getAtributo(AttComumArranjoResolucao_iteracao_inicial, IdIteracao()))
				a_entradaSaidaDados.setAppendArquivo(true);

			a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("custos_inferior_e_superior.csv", *this, AttMatrizMetodoSolucao_custo_inferior);
			a_entradaSaidaDados.setAppendArquivo(true);
			a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("custos_inferior_e_superior.csv", *this, AttMatrizMetodoSolucao_custo_superior);

			//
			// Esvaziamento de Matrizes
			//

			setMatriz(AttMatrizMetodoSolucao_custo_inferior, SmartEnupla<IdIteracao, SmartEnupla<IdCenario, double>>());
			setMatriz(AttMatrizMetodoSolucao_custo_superior, SmartEnupla<IdIteracao, SmartEnupla<IdCenario, double>>());

		} // if (a_idProcesso == IdProcesso_mestre) {

	} // try {
	catch (const std::exception&erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_avaliarSolucao(" + getFullString(a_idIteracao) + "," + getFullString(a_idProcesso) + ",a_modeloOtimizacao): \n" + std::string(erro.what())); }

} // void MetodoSolucao::executarPDDE_avaliarSolucao(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, IdModeloOtimizacao a_idModeloOtimizacao){


void MetodoSolucao::executarPDDE_calcularCorteBenders_new(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, EstruturaResultados<int>& a_status_otimizacao, EstruturaResultados<double>& a_custo_total, EstruturaResultados<double>& a_sol_inf_var_dinamica, EstruturaResultados<double>& a_solucao_dual_var_dinamica, EstruturaResultados<double>& a_limite_inferior_var_dinamica, EstruturaResultados<double>& a_limite_superior_var_dinamica, EstruturaResultados<double>& a_sol_dual_var_estado, EstruturaResultados<double>& a_vlr_var_estado, ModeloOtimizacao& a_modeloOtimizacao){
	try{

		executarPDDE_sincronizarResultadosParaCorteBenders(a_TSS, a_idIteracao, a_idEstagio, a_status_otimizacao, a_custo_total, a_sol_inf_var_dinamica, a_solucao_dual_var_dinamica, a_limite_inferior_var_dinamica, a_limite_superior_var_dinamica, a_sol_dual_var_estado, a_vlr_var_estado, a_modeloOtimizacao);

		EstruturaResultados<int> map_solucao_dual_proxy;

		executarPDDE_mapearSolucaoProxy(a_idIteracao, a_idEstagio, a_status_otimizacao, a_custo_total, a_sol_inf_var_dinamica, a_solucao_dual_var_dinamica, a_limite_inferior_var_dinamica, a_limite_superior_var_dinamica, a_sol_dual_var_estado, map_solucao_dual_proxy, a_modeloOtimizacao);

		if (a_modeloOtimizacao.getAtributo(a_idEstagio, AttComumEstagio_cortes_multiplos, int()) == 0)
			executarPDDE_calcularCorteBendersSingleCut(a_idIteracao, a_idEstagio, a_custo_total, a_sol_dual_var_estado, map_solucao_dual_proxy, a_vlr_var_estado, a_modeloOtimizacao);

		else
			executarPDDE_calcularCorteBendersMultiCut_new(a_idIteracao, a_idEstagio, a_custo_total, a_sol_dual_var_estado, map_solucao_dual_proxy, a_vlr_var_estado, a_modeloOtimizacao);


	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_calcularCorteBenders_new(" + getFullString(a_idEstagio) + ",...,a_modeloOtimizacao): \n" + std::string(erro.what())); }

} // void MetodoSolucao::executarPDDE_calcularCorteBenders_new(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, double* a_custo_total, double* a_sol_inf_var_dinamica, double* a_solucao_dual_var_dinamica, double* a_limite_inferior_var_dinamica, double* a_limite_superior_var_dinamica, double* a_sol_dual_var_estado, ModeloOtimizacao& a_modeloOtimizacao){

void MetodoSolucao::executarPDDE_sincronizarResultadosParaCorteBenders(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, EstruturaResultados<int>& a_status_otimizacao, EstruturaResultados<double>& a_custo_total, EstruturaResultados<double>& a_sol_inf_var_dinamica, EstruturaResultados<double>& a_solucao_dual_var_dinamica, EstruturaResultados<double>& a_limite_inferior_var_dinamica, EstruturaResultados<double>& a_limite_superior_var_dinamica, EstruturaResultados<double>& a_sol_dual_var_estado, EstruturaResultados<double>& a_vlr_var_estado, ModeloOtimizacao& a_modeloOtimizacao) {
	try {

		const int numero_estados = int(a_modeloOtimizacao.getMaiorId(a_idEstagio, IdVariavelEstado()));
		const int numero_variaveis_dinamicas = a_modeloOtimizacao.getNumeroVariavelDinamica(a_TSS, a_idEstagio);
		const int numero_processos = int(a_modeloOtimizacao.arranjoResolucao.getMaiorId(IdProcesso()));

		EstruturaResultados<int> status_otimizacao_all(numero_processos);
		EstruturaResultados<double> custo_total_all(numero_processos);
		EstruturaResultados<double> sol_inf_var_dinamica_all(numero_processos);

		EstruturaResultados<double> solucao_dual_var_dinamica_all(numero_processos);
		EstruturaResultados<double> limite_inferior_var_dinamica_all(numero_processos);
		EstruturaResultados<double> limite_superior_var_dinamica_all(numero_processos);

		EstruturaResultados<double> vlr_var_estado_all(numero_processos);
		EstruturaResultados<double> sol_dual_var_estado_all(numero_processos);

		for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_modeloOtimizacao.arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++) {

			const int rank = getRank(idProcesso);

			const int numero_cenarios_estados = int(a_modeloOtimizacao.arranjoResolucao.getIdsCenarioEstadoFromAberturas(idProcesso, a_idIteracao, a_idEstagio).size());

			const int numero_aberturas = a_modeloOtimizacao.arranjoResolucao.getNumeroAberturas(idProcesso, a_idIteracao, a_idEstagio);

			status_otimizacao_all.setSize(rank, numero_cenarios_estados, numero_aberturas);
			custo_total_all.setSize(rank, numero_cenarios_estados, numero_aberturas);
			sol_inf_var_dinamica_all.setSize(rank, numero_cenarios_estados, numero_aberturas);

			solucao_dual_var_dinamica_all.setSize(rank, numero_cenarios_estados, numero_aberturas, numero_variaveis_dinamicas);
			limite_inferior_var_dinamica_all.setSize(rank, numero_cenarios_estados, numero_aberturas, numero_variaveis_dinamicas);
			limite_superior_var_dinamica_all.setSize(rank, numero_cenarios_estados, numero_aberturas, numero_variaveis_dinamicas);

			vlr_var_estado_all.setSize(rank, numero_cenarios_estados, numero_estados);
			sol_dual_var_estado_all.setSize(rank, numero_cenarios_estados, numero_aberturas, numero_estados);

		} // for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_modeloOtimizacao.arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++) {

		MPI_Allgatherv(&a_status_otimizacao.vetor[0], a_status_otimizacao.sizeT.at(0) , MPI_INT, &status_otimizacao_all.vetor[0], &status_otimizacao_all.sizeT[0], &status_otimizacao_all.displ[0], MPI_INT, MPI_COMM_WORLD);
		a_status_otimizacao = status_otimizacao_all;

		MPI_Allgatherv(&a_custo_total.vetor[0], a_custo_total.sizeT.at(0), MPI_DOUBLE, &custo_total_all.vetor[0], &custo_total_all.sizeT[0], &custo_total_all.displ[0], MPI_DOUBLE, MPI_COMM_WORLD);
		a_custo_total = custo_total_all;

		MPI_Allgatherv(&a_sol_inf_var_dinamica.vetor[0], a_sol_inf_var_dinamica.sizeT.at(0), MPI_DOUBLE, &sol_inf_var_dinamica_all.vetor[0], &sol_inf_var_dinamica_all.sizeT[0], &sol_inf_var_dinamica_all.displ[0], MPI_DOUBLE, MPI_COMM_WORLD);
		a_sol_inf_var_dinamica = sol_inf_var_dinamica_all;

		MPI_Allgatherv(&a_solucao_dual_var_dinamica.vetor[0], a_solucao_dual_var_dinamica.sizeT.at(0), MPI_DOUBLE, &solucao_dual_var_dinamica_all.vetor[0], &solucao_dual_var_dinamica_all.sizeT[0], &solucao_dual_var_dinamica_all.displ[0], MPI_DOUBLE, MPI_COMM_WORLD);
		a_solucao_dual_var_dinamica = solucao_dual_var_dinamica_all;

		MPI_Allgatherv(&a_limite_inferior_var_dinamica.vetor[0], a_limite_inferior_var_dinamica.sizeT.at(0), MPI_DOUBLE, &limite_inferior_var_dinamica_all.vetor[0], &limite_inferior_var_dinamica_all.sizeT[0], &limite_inferior_var_dinamica_all.displ[0], MPI_DOUBLE, MPI_COMM_WORLD);
		a_limite_inferior_var_dinamica = limite_inferior_var_dinamica_all;

		MPI_Allgatherv(&a_limite_superior_var_dinamica.vetor[0], a_limite_superior_var_dinamica.sizeT.at(0), MPI_DOUBLE, &limite_superior_var_dinamica_all.vetor[0], &limite_superior_var_dinamica_all.sizeT[0], &limite_superior_var_dinamica_all.displ[0], MPI_DOUBLE, MPI_COMM_WORLD);
		a_limite_superior_var_dinamica = limite_superior_var_dinamica_all;

		MPI_Allgatherv(&a_vlr_var_estado.vetor[0], a_vlr_var_estado.sizeT.at(0), MPI_DOUBLE, &vlr_var_estado_all.vetor[0], &vlr_var_estado_all.sizeT[0], &vlr_var_estado_all.displ[0], MPI_DOUBLE, MPI_COMM_WORLD);
		a_vlr_var_estado = vlr_var_estado_all;

		MPI_Allgatherv(&a_sol_dual_var_estado.vetor[0], a_sol_dual_var_estado.sizeT.at(0), MPI_DOUBLE, &sol_dual_var_estado_all.vetor[0], &sol_dual_var_estado_all.sizeT[0], &sol_dual_var_estado_all.displ[0], MPI_DOUBLE, MPI_COMM_WORLD);
		a_sol_dual_var_estado = sol_dual_var_estado_all;

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_sincronizarResultadosParaCorteBenders(" + getFullString(a_idEstagio) + ",...,a_modeloOtimizacao): \n" + std::string(erro.what())); }
}


void MetodoSolucao::executarPDDE_calcularCorteBendersSingleCut(const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, EstruturaResultados<double>& a_custo_total, EstruturaResultados<double>& a_sol_dual_var_estado, EstruturaResultados<int>& a_map_solucao_dual_proxy, EstruturaResultados<double>& a_vlr_var_estado, ModeloOtimizacao& a_modeloOtimizacao) {
	try {

		const double lambda_CVAR = a_modeloOtimizacao.getAtributo(a_idEstagio, AttComumEstagio_lambda_CVAR, double());
		const double  alpha_CVAR = a_modeloOtimizacao.getAtributo(a_idEstagio, AttComumEstagio_alpha_CVAR, double());

		const IdCenario menor_cenario = a_modeloOtimizacao.arranjoResolucao.getAtributo(a_idIteracao, AttComumIteracao_menor_cenario, IdCenario());
		const IdCenario maior_cenario = a_modeloOtimizacao.arranjoResolucao.getAtributo(a_idIteracao, AttComumIteracao_maior_cenario, IdCenario());

		const int numero_cenarios = int(maior_cenario - menor_cenario) + 1;

		const IdAbertura maior_abertura = IdAbertura(a_modeloOtimizacao.getAtributo(a_idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()));

		SmartEnupla<IdCenario, SmartEnupla<IdVariavelEstado, double>>  lista_estado(menor_cenario, std::vector<SmartEnupla<IdVariavelEstado, double>>(numero_cenarios, SmartEnupla<IdVariavelEstado, double>()));

		SmartEnupla<IdVariavelEstado, double> inicia_estado(IdVariavelEstado_1, std::vector<double>(int(a_modeloOtimizacao.getMaiorId(a_idEstagio, IdVariavelEstado()) - IdVariavelEstado_1) + 1, 0.0));

		SmartEnupla<IdCenario, SmartEnupla<IdAbertura, double>> lista_custo_total(menor_cenario, std::vector<SmartEnupla<IdAbertura, double>>(numero_cenarios, SmartEnupla<IdAbertura, double>()));
		SmartEnupla<IdAbertura, double> inicia_rhs(IdAbertura_1, std::vector<double>(int(maior_abertura - IdAbertura_1) + 1, 0.0));

		SmartEnupla<IdCenario, SmartEnupla<IdAbertura, SmartEnupla<IdVariavelEstado, double>>> lista_sol_dual(menor_cenario, std::vector<SmartEnupla<IdAbertura, SmartEnupla<IdVariavelEstado, double>>>(numero_cenarios, SmartEnupla<IdAbertura, SmartEnupla<IdVariavelEstado, double>>()));
		SmartEnupla<IdAbertura, SmartEnupla<IdVariavelEstado, double>> inicia_coeficiente(IdAbertura_1, std::vector<SmartEnupla<IdVariavelEstado, double>>(int(a_modeloOtimizacao.getAtributo(a_idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()) - IdAbertura_1) + 1, SmartEnupla<IdVariavelEstado, double>()));

		SmartEnupla<IdCenario, int> sem_corte(menor_cenario, std::vector<int>(numero_cenarios, 1));

		const int nr = a_custo_total.bloco;

		for (int r = 0; r < nr; r++) {

			// Teste se processo calculou alguma abertura.
			if (a_custo_total.size1.at(r) > 0) {

				const int nc = a_custo_total.size1.at(r);
				const int na = a_custo_total.size2.at(r);
				const int ne = a_sol_dual_var_estado.size3.at(r);

				const IdProcesso idProcesso = IdProcesso(r + 1);

				const std::vector<IdCenario> cenarios_estado = a_modeloOtimizacao.arranjoResolucao.getIdsCenarioEstadoFromAberturas(idProcesso, a_idIteracao, a_idEstagio);

				for (int c = 0; c < nc; c++) {

					const int indice_inicial_estado = a_vlr_var_estado.displ.at(r) + c * ne;
					const IdCenario idCenario_estado = cenarios_estado.at(c);
					const IdAbertura menor_abertura = a_modeloOtimizacao.arranjoResolucao.getElementoMatriz(a_idIteracao, idProcesso, AttMatrizProcesso_menor_abertura_por_cenario_estado, idCenario_estado, a_idEstagio, IdAbertura());

					for (int a = 0; a < na; a++) {

						const IdAbertura idAbertura = IdAbertura(menor_abertura + a);

						const int indice_custo_total = a_custo_total.displ.at(r) + c * na + a;

						int indice_inicial_sol_dual = a_sol_dual_var_estado.displ.at(r) + c * na * ne + a * ne;
						if (a_map_solucao_dual_proxy.vetor.at(indice_custo_total) == -2)
							break;
						else if (a_map_solucao_dual_proxy.vetor.at(indice_custo_total) > -1)
							indice_inicial_sol_dual = a_map_solucao_dual_proxy.vetor.at(indice_custo_total);

						sem_corte.at(idCenario_estado) = 0;

						if (lista_estado.at(idCenario_estado).size() == 0) {
							lista_estado.at(idCenario_estado) = inicia_estado;
							for (int e = 0; e < ne; e++)
								lista_estado.at(idCenario_estado).at(IdVariavelEstado(e + 1)) = a_vlr_var_estado.vetor.at(indice_inicial_estado + e);
						}

						if (lista_custo_total.at(idCenario_estado).size() == 0)
							lista_custo_total.at(idCenario_estado) = inicia_rhs;
						lista_custo_total.at(idCenario_estado).at(idAbertura) = a_custo_total.vetor.at(indice_custo_total);

						if (lista_sol_dual.at(idCenario_estado).size() == 0)
							lista_sol_dual.at(idCenario_estado) = inicia_coeficiente;
						if (lista_sol_dual.at(idCenario_estado).at(idAbertura).size() == 0)
							lista_sol_dual.at(idCenario_estado).at(idAbertura) = inicia_estado;

						for (int e = 0; e < ne; e++)
							lista_sol_dual.at(idCenario_estado).at(idAbertura).at(IdVariavelEstado(e + 1)) = a_sol_dual_var_estado.vetor.at(indice_inicial_sol_dual + e);

					} // for (int a = 0; a < na; a++) {
				} // for (int c = 0; c < nc; c++) {
			} // if (a_custo_total.size1.at(r) > 0) {
		} // for (int r = 0; r < nr; r++) {

		a_custo_total.esvaziar();
		a_vlr_var_estado.esvaziar();
		a_sol_dual_var_estado.esvaziar();

		for (IdCenario idCenario = menor_cenario; idCenario <= maior_cenario; idCenario++) {

			if (sem_corte.at(idCenario) == 0) {

				std::vector<IdAbertura> idAbertura_custo_maior_para_menor;
				idAbertura_custo_maior_para_menor.reserve(int(maior_abertura));

				for (IdAbertura idAbertura = IdAbertura_1; idAbertura <= maior_abertura; idAbertura++) {
					if (idAbertura_custo_maior_para_menor.size() == 0)
						idAbertura_custo_maior_para_menor.push_back(idAbertura);
					else {
						for (int i = 0; i < int(idAbertura_custo_maior_para_menor.size()); i++) {
							if (lista_custo_total.at(idCenario).at(idAbertura) > lista_custo_total.at(idCenario).at(idAbertura_custo_maior_para_menor.at(i))) {
								idAbertura_custo_maior_para_menor.insert(idAbertura_custo_maior_para_menor.begin() + i, idAbertura);
								break;
							}
							else if (i == int(idAbertura_custo_maior_para_menor.size() - 1)) {
								idAbertura_custo_maior_para_menor.push_back(idAbertura);
								break;
							}
						}
					}
				}

				if (idAbertura_custo_maior_para_menor.size() != int(maior_abertura))
					throw std::invalid_argument("Erro na ordenacao dos custos.");

				SmartEnupla<IdRealizacao, double> lista_enupla_rhs(IdRealizacao_1, std::vector<double>(1, 0.0));
				SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> lista_enupla_coeficiente(IdRealizacao_1, std::vector<SmartEnupla<IdVariavelEstado, double>>(1, inicia_estado));

				double probabilidade_acumulada = 0.0;
				for (int i = 0; i < int(idAbertura_custo_maior_para_menor.size()); i++) {

					const IdAbertura idAbertura = idAbertura_custo_maior_para_menor.at(i);

					const double probabilidade_original = a_modeloOtimizacao.getProbabilidadeAbertura(a_idEstagio, IdRealizacao(idAbertura));

					probabilidade_acumulada += probabilidade_original;

					double probabilidade_CVaR = probabilidade_original;
					if (probabilidade_acumulada <= alpha_CVAR)
						probabilidade_CVaR = probabilidade_original * (1.0 - lambda_CVAR) + probabilidade_original * (lambda_CVAR / alpha_CVAR);
					else if ((probabilidade_acumulada > alpha_CVAR) && (alpha_CVAR >= a_modeloOtimizacao.getProbabilidadeAbertura(a_idEstagio, IdRealizacao(idAbertura_custo_maior_para_menor.at(0)))))
						probabilidade_CVaR = probabilidade_original * (1.0 - lambda_CVAR);

					lista_enupla_rhs.at(IdRealizacao_1) += lista_custo_total.at(idCenario).at(idAbertura) * probabilidade_CVaR;

					for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado < inicia_estado.getIteradorFinal(); idVariavelEstado++) {
						lista_enupla_rhs.at(IdRealizacao_1) -= lista_sol_dual.at(idCenario).at(idAbertura).at(idVariavelEstado) * lista_estado.at(idCenario).at(idVariavelEstado) * probabilidade_CVaR;
						lista_enupla_coeficiente.at(IdRealizacao_1).at(idVariavelEstado) += lista_sol_dual.at(idCenario).at(idAbertura).at(idVariavelEstado) * probabilidade_CVaR;
					} // for (int e = 0; e < ne; e++) {
				} // for (IdAbertura idAbertura = IdAbertura_1; idAbertura <= maior_abertura; idAbertura++) {

				a_modeloOtimizacao.instanciarCorteBenders(a_idEstagio, lista_enupla_rhs, lista_enupla_coeficiente, lista_estado.at(idCenario));

			} // if (sem_corte.at(idCenario) == 0) {
		} // for (IdCenario idCenario = menor_cenario; idCenario <= maior_cenario; idCenario++) {


	} // try {

	catch (const std::exception& erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_calcularCorteBendersSingleCut(" + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "a_custo_total,a_solucao_dual_variavel_estado,a_modeloOtimizacao): \n" + std::string(erro.what())); }
}

void MetodoSolucao::executarPDDE_calcularCorteBendersMultiCut_new(const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, EstruturaResultados<double>& a_custo_total, EstruturaResultados<double>& a_sol_dual_var_estado, EstruturaResultados<int>& a_map_solucao_dual_proxy, EstruturaResultados<double>& a_vlr_var_estado, ModeloOtimizacao& a_modeloOtimizacao) {
	try {

		/*

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
			if (a_custo_total.size1.at(r) == 0)
				r = 0;

			const int nc = a_custo_total.size1.at(r);
			const int ne = a_sol_dual_var_estado.size3.at(r);

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
						if ((nc > a_custo_total.size1.at(r_busca)) && (a_custo_total.size1.at(r_busca) > 0))
							throw std::invalid_argument("Estrutura nao compativel com busca global.");
					} // if ((a_busca_global) && (r != r_busca)){

					const int na_busca = a_custo_total.size2.at(r_busca);

					IdProcesso idProcesso_busca = IdProcesso(r + 1);

					if (a_busca_global) {
						idProcesso_busca = IdProcesso(r_busca + 1);
						idCenario = a_modeloOtimizacao.getCenarioInicial(idProcesso_busca, a_idIteracao);;
					} // if (a_busca_global) {

					const IdAbertura abertura_inicial = IdAbertura_Nenhum;// = a_modeloOtimizacao.getElementoMatriz(a_idIteracao, idProcesso_busca, AttMatrizProcesso_mapeamento_abertura_inicial_por_cenario_por_estagio, idCenario, a_idEstagio, IdAbertura());

					const int indice_inicial_estado = a_vlr_var_estado.displ.at(r_busca) + c * ne;
					for (int a_busca = 0; a_busca < na_busca; a_busca++) {

						const IdRealizacao idRealizacao = IdRealizacao(abertura_inicial + a_busca);

						if (enupla_coeficiente.at(idRealizacao).size() == 0)
							enupla_coeficiente.at(idRealizacao) = SmartEnupla<IdVariavelEstado, double>(IdVariavelEstado_1, std::vector<double>(ne, 0.0));

						const int indice_custo_total = a_custo_total.displ.at(r_busca) + c * na_busca + a_busca;

						int indice_inicial_sol_dual = a_sol_dual_var_estado.displ.at(r_busca) + c * na_busca * ne + a_busca * ne;
						if (a_map_solucao_dual_proxy.vetor.at(indice_custo_total) == -2) {
							sem_corte = true;
							break;
						}
						else if (a_map_solucao_dual_proxy.vetor.at(indice_custo_total) > -1)
							indice_inicial_sol_dual = a_map_solucao_dual_proxy.vetor.at(indice_custo_total);

						enupla_rhs.at(idRealizacao) = a_custo_total.vetor.at(indice_custo_total);

						for (int e = 0; e < ne; e++) {
							const IdVariavelEstado idVariavelEstado = IdVariavelEstado(e + 1);

							enupla_rhs.at(idRealizacao) -= a_sol_dual_var_estado.vetor.at(indice_inicial_sol_dual + e) * a_vlr_var_estado.vetor.at(indice_inicial_estado + e);
							enupla_coeficiente.at(idRealizacao).at(idVariavelEstado) += a_sol_dual_var_estado.vetor.at(indice_inicial_sol_dual + e);
							if (first_round)
								enupla_estado.at(idVariavelEstado) = a_vlr_var_estado.vetor.at(indice_inicial_estado + e);
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
		*/
	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_calcularCorteBendersMultiCut_new(" + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + ",a_custo_total,a_solucao_dual_variavel_estado,a_modeloOtimizacao): \n" + std::string(erro.what())); }
}

void MetodoSolucao::executarPDDE_mapearSolucaoProxy(const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, EstruturaResultados<int>& a_status_otimizacao, EstruturaResultados<double>& a_custo_total, EstruturaResultados<double>& a_sol_inf_var_dinamica, EstruturaResultados<double>& a_solucao_dual_var_dinamica, EstruturaResultados<double>& a_limite_inferior_var_dinamica, EstruturaResultados<double>& a_limite_superior_var_dinamica, EstruturaResultados<double>& a_sol_dual_var_estado, EstruturaResultados<int>& a_map_solucao_dual_proxy, ModeloOtimizacao& a_modeloOtimizacao) {

	try {
	 
		const TipoSelecaoSolucaoProxy tipo_selecao_solucao_proxy = a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_tipo_selecao_solucao_proxy, TipoSelecaoSolucaoProxy());
	
		const int agrupar_aberturas = a_modeloOtimizacao.arranjoResolucao.getElementoVetor(AttVetorArranjoResolucao_agrupar_aberturas, a_idEstagio, int());

		const int nr = a_custo_total.bloco;

		EstruturaResultados<int> map_solucao_dual_proxy(nr);
		for (int r = 0; r < nr; r++)
			map_solucao_dual_proxy.setSize(r, a_custo_total.size1.at(r), a_custo_total.size2.at(r));

		for (int r = 0; r < nr; r++) {

			const int nc = a_custo_total.size1.at(r);
			const int na = a_custo_total.size2.at(r);
			const int ni = a_solucao_dual_var_dinamica.size3.at(r);
			const int ne = a_sol_dual_var_estado.size3.at(r);

			int r_min_busca = r;
			int r_max_busca = r + 1;
			if (agrupar_aberturas == 1) {
				r_min_busca = 0;
				r_max_busca = nr;
			} // if (agrupar_aberturas == 1) {

			for (int c = 0; c < nc; c++) {

				bool alguma_solucao_proxy = false;
				for (int r_busca = r_min_busca; r_busca < r_max_busca; r_busca++) {

					const int na_busca = a_custo_total.size2.at(r_busca);

					if ((agrupar_aberturas == 1) && (r != r_busca)) {
						if ((nc > a_custo_total.size1.at(r_busca)) && (a_custo_total.size1.at(r_busca) > 0))
							throw std::invalid_argument("Estrutura nao compativel com agrupamento de aberturas.");
					} // if ((agrupar_aberturas == 1) && (r != r_busca)){

					for (int a_busca = 0; a_busca < na_busca; a_busca++) {
						if (a_status_otimizacao.vetor.at(a_status_otimizacao.displ.at(r_busca) + c * na_busca + a_busca) > 0) {
							alguma_solucao_proxy = true;
							break;
						}
					} // for (int a_busca = 0; a_busca < na_busca; a_busca++) {
				} // for (int r_busca = r_min_busca; r_busca < r_max_busca; r_busca++) {

				for (int a = 0; a < na; a++) {

					if (!alguma_solucao_proxy)
						map_solucao_dual_proxy.vetor.at(map_solucao_dual_proxy.displ.at(r) + c * na + a) = -2;

					else if (a_status_otimizacao.vetor.at(a_status_otimizacao.displ.at(r) + c * na + a) == 0) {

						int                      a_selec = -1;
						int    indice_solucao_dual_selec = -1;
						double         custo_total_selec = 0.0;

						int a_maior_custo_primal = -1;

						for (int r_busca = r_min_busca; r_busca < r_max_busca; r_busca++) {
							const int na_busca = a_custo_total.size2.at(r_busca);

							for (int a_busca = 0; a_busca < na_busca; a_busca++) {

								int a_candidata = a_busca;

								if (a_status_otimizacao.vetor.at(a_status_otimizacao.displ.at(r_busca) + c * na_busca + a_busca) > 0) {

									if (tipo_selecao_solucao_proxy == TipoSelecaoSolucaoProxy_aleatoria) {}
									if (tipo_selecao_solucao_proxy == TipoSelecaoSolucaoProxy_maior_custo_primal_proxy) {

										if (a_maior_custo_primal == -1) {
											double custo_primal = 0.0;
											for (int a_custo_primal = 0; a_custo_primal < na_busca; a_custo_primal++) {
												const int idx_a_custo_primal = a_custo_total.displ.at(r_busca) + c * na_busca + a_custo_primal;
												if (a_status_otimizacao.vetor.at(idx_a_custo_primal) > 0) {
													if (a_custo_total.vetor.at(idx_a_custo_primal) > custo_primal) {
														custo_primal = a_custo_total.vetor.at(idx_a_custo_primal);
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

										const int idx_atual     = a_limite_inferior_var_dinamica.displ.at(r) + c * na * ni + a * ni + i;
										const int idx_candidata = a_solucao_dual_var_dinamica.displ.at(r_busca) + c * na_busca * ni + a_candidata * ni + i;

										const double solucao_dual_var_dinamica = a_solucao_dual_var_dinamica.vetor.at(idx_candidata);

										if (solucao_dual_var_dinamica > 0.0) {
											const double limite_inferior = a_limite_inferior_var_dinamica.vetor.at(idx_atual);
											if (limite_inferior > 0.0)
												custo_dinamico_proxy_positivo += solucao_dual_var_dinamica * limite_inferior;
											else if (limite_inferior < 0.0)
												custo_dinamico_proxy_negativo += solucao_dual_var_dinamica * limite_inferior;
										}
										else if (solucao_dual_var_dinamica < 0.0) {
											const double limite_superior = a_limite_superior_var_dinamica.vetor.at(idx_atual);
											if (limite_superior > 0.0)
												custo_dinamico_proxy_negativo += solucao_dual_var_dinamica * limite_superior;
											else if (limite_superior < 0.0)
												custo_dinamico_proxy_positivo += solucao_dual_var_dinamica * limite_superior;
										}

									} // for (int i = 0; i < ni; i++) {

									const double solucao_inferior_var_dinamica = a_sol_inf_var_dinamica.vetor.at(a_sol_inf_var_dinamica.displ.at(r_busca) + c * na_busca + a_candidata);
									double custo_total_proxy = solucao_inferior_var_dinamica + custo_dinamico_proxy_positivo + custo_dinamico_proxy_negativo;

									if (custo_total_proxy < 0.0)
										custo_total_proxy = 0.0;

									if (custo_total_proxy >= custo_total_selec) {
										indice_solucao_dual_selec = a_sol_dual_var_estado.displ.at(r_busca) + c * na_busca * ne + a_candidata * ne;
										custo_total_selec = custo_total_proxy;
										a_selec = a_candidata;
									} // if (custo_total_proxy < custo_total_proxy_selecionado) {

									if ((tipo_selecao_solucao_proxy == TipoSelecaoSolucaoProxy_aleatoria) || (tipo_selecao_solucao_proxy == TipoSelecaoSolucaoProxy_maior_custo_primal_proxy))
										break;

								} // if (a_status_otimizacao.vetor.at(idx_a_busca) > 0) {
							} // for (int a_busca = 0; a_busca < na; a_busca++) {
						} // for (int r_busca = r_min_busca; r_busca < r_max_busca; r_busca++) {

						if (a_selec < 0)
							throw std::invalid_argument("Nao foi encontrada solucao proxy para utilizacao em r" + getString(r) + ", a" + getString(a) + ".");

						a_custo_total.vetor.at(a_custo_total.displ.at(r) + c * na + a) = custo_total_selec;
						map_solucao_dual_proxy.vetor.at(a_custo_total.displ.at(r) + c * na + a) = indice_solucao_dual_selec;

					} // if (a_status_otimizacao.vetor.at(idx_status) == 0) {
					else
						map_solucao_dual_proxy.vetor.at(a_custo_total.displ.at(r) + c * na + a) = -1;

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

	catch (const std::exception& erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_mapearSolucaoProxy(" + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + ",...,a_modeloOtimizacao): \n" + std::string(erro.what())); }

}

void MetodoSolucao::executarPDDE_requestCorteBenders(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, ModeloOtimizacao& a_modeloOtimizacao, const std::string a_diretorio, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		a_modeloOtimizacao.requestCorteBenders(a_idProcesso, a_idEstagio, a_diretorio, a_entradaSaidaDados);

	} // try {
	catch (const std::exception & erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_requestCorteBenders(" + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + "," + getFullString(a_idEstagio) + ",a_modeloOtimizacao): \n" + std::string(erro.what())); }

} // void MetodoSolucao::executarPDDE_requestCorteBenders(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdEstagio a_idEstagio, ModeloOtimizacao &a_modeloOtimizacao){



void MetodoSolucao::executarPDDE_distribuirEstadosEntreProcessos(const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, ModeloOtimizacao& a_modeloOtimizacao){

	try {

		const int numero_estados = int(a_modeloOtimizacao.getMaiorId(a_idEstagio, IdVariavelEstado()));

		const IdProcesso idProcesso_local = a_modeloOtimizacao.arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());
		const IdProcesso maior_processo = a_modeloOtimizacao.arranjoResolucao.getMaiorId(IdProcesso());

		for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= maior_processo; idProcesso++) {

			if (idProcesso == idProcesso_local) {

				for (IdProcesso idProcesso_para = IdProcesso_mestre; idProcesso_para <= maior_processo; idProcesso_para++) {

					if (idProcesso_para != idProcesso) {
						std::vector<IdCenario> lista_idCenarios_estado = a_modeloOtimizacao.arranjoResolucao.getIdsCenarioEstado(idProcesso_local, idProcesso_para, a_idIteracao, a_idEstagio);

						const int numero_cenarios_estado = int(lista_idCenarios_estado.size());

						if (numero_cenarios_estado > 0) {

							double* valores = new double[numero_cenarios_estado * numero_estados];

							int i = 0;
							for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= a_modeloOtimizacao.getMaiorId(a_idEstagio, IdVariavelEstado()); idVariavelEstado++) {
								if (a_modeloOtimizacao.isVariavelEstadoInstanciada(a_idEstagio, idVariavelEstado)) {
									for (int c = 0; c < numero_cenarios_estado; c++) {
										const IdCenario idCenario_estado = lista_idCenarios_estado.at(c);
										valores[i] = a_modeloOtimizacao.getElementoVetor(a_idEstagio, idVariavelEstado, AttVetorVariavelEstado_valor, idCenario_estado, double());
										i++;
									}
								}
							}

							MPI_Send(valores, i, MPI_DOUBLE, getRank(idProcesso_para), 0, MPI_COMM_WORLD);

							delete[] valores;

						} // if (lista_idCenarios_estado.size() > 0) {

					} // if (idProcesso_para != idProcesso) {

				} // for (IdProcesso idProcesso_para = IdProcesso_mestre; idProcesso_para <= maior_processo; idProcesso_para++) {

			} // if (idProcesso == idProcesso_local) {

			else if (idProcesso != idProcesso_local) {

				std::vector<IdCenario> lista_idCenarios_estado = a_modeloOtimizacao.arranjoResolucao.getIdsCenarioEstado(idProcesso, idProcesso_local, a_idIteracao, a_idEstagio);

				const int numero_cenarios_estado = int(lista_idCenarios_estado.size());

				if (numero_cenarios_estado > 0) {

					double* valores = new double[numero_cenarios_estado * numero_estados];

					MPI_Recv(valores, numero_cenarios_estado * numero_estados, MPI_DOUBLE, getRank(idProcesso), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

					int e = 0;
					for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= a_modeloOtimizacao.getMaiorId(a_idEstagio, IdVariavelEstado()); idVariavelEstado++) {
						if (a_modeloOtimizacao.isVariavelEstadoInstanciada(a_idEstagio, idVariavelEstado)) {
							for (int c = 0; c < numero_cenarios_estado; c++) {
								const IdCenario idCenario_estado = lista_idCenarios_estado.at(c);
								a_modeloOtimizacao.addValorVariavelEstado(a_idEstagio, idVariavelEstado, idCenario_estado, valores[e * numero_cenarios_estado + c]);
							}
							e++;
						}
					}

					delete[] valores;

				} // if (lista_idCenarios_estado.size() > 0) {

			} // else if (idProcesso != idProcesso_local) {

		} // for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= maior_processo; idProcesso++) {

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_distribuirEstadosEntreProcessos(" + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "," + ",a_valor_estados,a_modeloOtimizacao): \n" + std::string(erro.what())); }

} // void MetodoSolucao::executarPDDE_distribuirEstadosEntreProcessos(const IdProcesso a_idProcesso, const IdProcesso a_maior_processo, const SmartEnupla<IdCenario, SmartEnupla<IdVariavelEstado, double>>& a_estados, ModeloOtimizacao& a_modeloOtimizacao){


void MetodoSolucao::executarPDDE_operacao_final(EntradaSaidaDados a_entradaSaidaDados, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, ModeloOtimizacao& a_modeloOtimizacao){

	try{

		const IdIteracao iteracao_final = a_modeloOtimizacao.arranjoResolucao.getAtributo(AttComumArranjoResolucao_iteracao_final, IdIteracao());

		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida();

		const TipoSubproblemaSolver tSS = TipoSubproblemaSolver_geral;

		for (IdIteracao idIteracao = IdIteracao(iteracao_final + 1); idIteracao >= a_modeloOtimizacao.arranjoResolucao.getAtributo(AttComumArranjoResolucao_iteracao_inicial, IdIteracao()); idIteracao--) {

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
			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("VariaveisEstado_valores_" + getFullString(a_idProcesso) + ".csv", IdEstagio_Nenhum, IdVariavelEstado_Nenhum,     a_modeloOtimizacao, AttVetorVariavelEstado_valor);

	} // try {
	catch (const std::exception&erro) { throw std::invalid_argument("MetodoSolucao(" + getString(getIdObjeto()) + ")::executarPDDE_imprimirEstados(a_entradaSaidaDados," + getFullString(a_idIteracao) + "," + getFullString(a_idProcesso) + ",a_modeloOtimizacao): \n" + std::string(erro.what())); }

} //void MetodoSolucao::executarPDDE_imprimirResultados(EntradaSaidaDados a_entradaSaidaDados, const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, ModeloOtimizacao & a_modeloOtimizacao){





