#include "C_ModeloOtimizacao.h"
#include "C_EntradaSaidaDados.h"
#include <chrono>

#include "mpi.h"

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////


void ModeloOtimizacao::formularModeloOtimizacao(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados) {


	try {
		
		const IdEstagio idEstagio_acoplamento = a_dados.getAtributo(AttComumDados_estagio_acoplamento_pre_estudo, IdEstagio());

		const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
		const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

		Periodo periodo_otimizacao_inicial = getAtributo(AttComumModeloOtimizacao_periodo_otimizacao_inicial, Periodo());
		Periodo periodo_otimizacao_final = getAtributo(AttComumModeloOtimizacao_periodo_otimizacao_final, Periodo());

		Periodo periodo_estudo_inicial = getAtributo(AttComumModeloOtimizacao_periodo_estudo_inicial, Periodo());
		Periodo periodo_estudo_final = getAtributo(AttComumModeloOtimizacao_periodo_estudo_final, Periodo());

		TipoSolver tipoSolver = a_dados.getAtributo(AttComumDados_tipo_solver, TipoSolver());

		SmartEnupla<Periodo, IdEstagio> horizonte_estudo;

		SmartEnupla<IdEstagio, SmartEnupla<Periodo, double>> horizonte_estudo_por_estagio(estagio_inicial, std::vector<SmartEnupla<Periodo, double>>(int(estagio_final - estagio_inicial) + 1, SmartEnupla<Periodo, double>()));
		SmartEnupla<IdEstagio, SmartEnupla<Periodo, int>> horizonte_processo_estocastico_hidrologico_por_estagio(estagio_inicial, std::vector<SmartEnupla<Periodo, int>>(int(estagio_final - estagio_inicial) + 1, SmartEnupla<Periodo, int>()));

		// Instanciamento Estagio

		if (true) {

			const SmartEnupla<Periodo, IdEstagio> horizonte_estudo_original = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

			// Instancia Estagio

			for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

				const Periodo period_stage = a_dados.getElementoVetor(AttVetorDados_horizonte_otimizacao, idEstagio, Periodo());

				if (true) {
					Estagio estagio;
					estagio.setAtributo(AttComumEstagio_idEstagio, idEstagio);
					vetorEstagio.add(estagio);
				} // if (true) {

				vetorEstagio.at(idEstagio).setAtributo(AttComumEstagio_periodo_otimizacao, period_stage);

				vetorEstagio.at(idEstagio).setAtributo(AttComumEstagio_maiorIdRealizacao, IdRealizacao(a_dados.getElementoVetor(AttVetorDados_numero_aberturas, idEstagio, int())));

				vetorEstagio.at(idEstagio).setAtributo(AttComumEstagio_cortes_multiplos, a_dados.getElementoVetor(AttVetorDados_cortes_multiplos, idEstagio, int()));

				vetorEstagio.at(idEstagio).setAtributo(AttComumEstagio_lambda_CVAR, a_dados.getElementoVetor(AttVetorDados_lambda_CVAR, idEstagio, double()));
				vetorEstagio.at(idEstagio).setAtributo(AttComumEstagio_alpha_CVAR, a_dados.getElementoVetor(AttVetorDados_alpha_CVAR, idEstagio, double()));

				vetorEstagio.at(idEstagio).selecaoSolucaoProxy(a_dados.getElementoVetor(AttVetorDados_numero_aberturas_solucao_proxy, idEstagio, int()));

				if (idEstagio == idEstagio_acoplamento)
					vetorEstagio.at(idEstagio).setAtributo(AttComumEstagio_selecao_cortes_nivel_dominancia, 0);
				else
					vetorEstagio.at(idEstagio).setAtributo(AttComumEstagio_selecao_cortes_nivel_dominancia, getAtributo(AttComumModeloOtimizacao_selecao_cortes_nivel_dominancia, int()));
				
				if ((idEstagio > estagio_inicial) || (estagio_inicial != IdEstagio_1)) {
					if (strCompara(getAtributo(AttComumModeloOtimizacao_diretorio_importacao_cortes, std::string()), "nenhum")) {
						if (idEstagio > estagio_inicial) {
							if (arranjoResolucao.isAnyCenarioEstado(IdEstagio(idEstagio - 1)) || arranjoResolucao.isAnyAberturas(IdEstagio(idEstagio - 1)))
								vetorEstagio.at(idEstagio).vetorCorteBenders.alocar(getAtributo(AttComumModeloOtimizacao_numero_cenarios, int()));
						}
						else {
							if (arranjoResolucao.isAnyCenarioEstado(idEstagio) || arranjoResolucao.isAnyAberturas(idEstagio))
								vetorEstagio.at(idEstagio).vetorCorteBenders.alocar(getAtributo(AttComumModeloOtimizacao_numero_cenarios, int()));
						}
					}
				} // if ((idEstagio > estagio_inicial) || (estagio_inicial != IdEstagio_1)) {

				for (Periodo periodo = horizonte_estudo_original.getIteradorInicial(); periodo <= horizonte_estudo_original.getIteradorFinal(); horizonte_estudo_original.incrementarIterador(periodo)) {
					if (horizonte_estudo_original.at(periodo) == idEstagio)
						horizonte_estudo.addElemento(periodo, idEstagio);
				}


			} // for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

		} // if (true) {


		Periodo periodo_acoplamento;
		if (idEstagio_acoplamento > estagio_final) {
			const SmartEnupla<Periodo, double> horizon_stage = a_dados.getElementosMatriz(AttMatrizDados_percentual_duracao_horizonte_estudo, estagio_final, Periodo(), double());
			periodo_acoplamento = horizon_stage.getIteradorFinal() + 1;
		}
		else
			periodo_acoplamento = a_dados.getElementoVetor(AttVetorDados_horizonte_otimizacao, idEstagio_acoplamento, Periodo());

		Periodo menor_periodo_estagio_acoplamento_pre_estudo = periodo_acoplamento;

		for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
			if (horizonte_estudo.getElemento(periodo) == a_dados.getAtributo(AttComumDados_estagio_acoplamento_pre_estudo, IdEstagio())) {
				menor_periodo_estagio_acoplamento_pre_estudo = periodo;
				break;
			}//if (horizonte_estudo_vetor.getElemento(periodo) == a_dados.getAtributo(AttComumDados_estagio_acoplamento_pre_estudo, IdEstagio())) {
		}//for (Periodo periodo = horizonte_estudo_vetor.getIteradorInicial(); periodo <= horizonte_estudo_vetor.getIteradorFinal(); horizonte_estudo_vetor.incrementarIterador(periodo)) {

		const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> horizon = a_dados.getMatriz(AttMatrizDados_percentual_duracao_patamar_carga, Periodo(), IdPatamarCarga(), double());

		//
		// Pré alocação de vetores e matrizes
		//


		if (true) {

			const IdProcessoEstocastico   tipo_processo_estocastico_hidrologico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

			SmartEnupla<IdEstagio, SmartEnupla<IdRealizacao, double>> custo_medio(estagio_inicial, std::vector<SmartEnupla<IdRealizacao, double>>(int(estagio_final) - int(estagio_inicial) + 1, SmartEnupla<IdRealizacao, double>()));

			for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

				const Periodo period_stage = getAtributo(idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

				custo_medio.at(idEstagio) = SmartEnupla<IdRealizacao, double>(IdRealizacao_1, std::vector<double>(getAtributo(idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()), 0.0));		

				if (getSize1Matriz(tipo_processo_estocastico_hidrologico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral) > 0) {
					const SmartEnupla<Periodo, IdRealizacao> horizonte_processo_estocastico_espaco_amostral_completo = getElementosMatriz(tipo_processo_estocastico_hidrologico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario_1, Periodo(), IdRealizacao());
					const std::vector<Periodo> horizonte_processo_estocastico_espaco_amostral = horizonte_processo_estocastico_espaco_amostral_completo.getIteradores(period_stage);

					for (int i = 0; i < int(horizonte_processo_estocastico_espaco_amostral.size()); i++)
						horizonte_processo_estocastico_hidrologico_por_estagio.at(idEstagio).addElemento(horizonte_processo_estocastico_espaco_amostral.at(i), getSize2Matriz(tipo_processo_estocastico_hidrologico, IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_residuo_espaco_amostral, horizonte_processo_estocastico_espaco_amostral.at(i)));

				}

				for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
					if (horizonte_estudo.at(periodo) == idEstagio)
						horizonte_estudo_por_estagio.at(idEstagio).addElemento(periodo, 0.0);
				}

				int is_any_relaxacao_truncamento = 0;
				for (Periodo periodo = horizonte_processo_estocastico_hidrologico_por_estagio.at(idEstagio).getIteradorInicial(); periodo <= horizonte_processo_estocastico_hidrologico_por_estagio.at(idEstagio).getIteradorFinal(); horizonte_processo_estocastico_hidrologico_por_estagio.at(idEstagio).incrementarIterador(periodo)) {
					for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= getMaiorId(tipo_processo_estocastico_hidrologico, IdVariavelAleatoria()); idVar++) {
						if ((getElementoVetor(tipo_processo_estocastico_hidrologico, idVar, AttVetorVariavelAleatoria_tipo_relaxacao, periodo, TipoRelaxacaoVariavelAleatoria()) == TipoRelaxacaoVariavelAleatoria_truncamento) || \
							(getElementoVetor(tipo_processo_estocastico_hidrologico, idVar, AttVetorVariavelAleatoria_tipo_relaxacao, periodo, TipoRelaxacaoVariavelAleatoria()) == TipoRelaxacaoVariavelAleatoria_truncamento_penalizacao)) {
							is_any_relaxacao_truncamento = 1;
							break;
						}
					}
				}
				addElemento(AttVetorModeloOtimizacao_alguma_variavel_aleatoria_hidrologica_com_truncamento, idEstagio, is_any_relaxacao_truncamento);

			} // for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

			setMatriz(AttMatrizModeloOtimizacao_horizonte_estudo, horizonte_estudo_por_estagio);
			setMatriz(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, horizonte_processo_estocastico_hidrologico_por_estagio);

			setVetor(AttVetorModeloOtimizacao_custo_total, SmartEnupla<IdEstagio, double>(estagio_inicial, std::vector<double>(int(estagio_final) - int(estagio_inicial) + 1, NAN)));
			setVetor(AttVetorModeloOtimizacao_custo_imediato, SmartEnupla<IdEstagio, double>(estagio_inicial, std::vector<double>(int(estagio_final) - int(estagio_inicial) + 1, NAN)));
			setVetor(AttVetorModeloOtimizacao_custo_futuro, SmartEnupla<IdEstagio, double>(estagio_inicial, std::vector<double>(int(estagio_final) - int(estagio_inicial) + 1, NAN)));

			setMatriz(AttMatrizModeloOtimizacao_realizacao_solucao_proxy, SmartEnupla<IdEstagio, SmartEnupla<IdRealizacao, int>>(estagio_inicial, std::vector<SmartEnupla<IdRealizacao, int>>(int(estagio_final) - int(estagio_inicial) + 1, SmartEnupla<IdRealizacao, int>())));

			setVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, SmartEnupla<IdEstagio, int>(estagio_inicial, std::vector<int>(int(estagio_final) - int(estagio_inicial) + 1, 2)));

			setMatriz(AttMatrizModeloOtimizacao_custo_medio, custo_medio);

		} // if (true) {

		// Instancia Solver
		SmartEnupla<IdEstagio, std::vector<TipoSubproblemaSolver>> listaTSS(estagio_inicial, std::vector<std::vector<TipoSubproblemaSolver>>(int(estagio_final- estagio_inicial) + 1, std::vector<TipoSubproblemaSolver>()));
		for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

				if (true) {
					bool isViabilidadeHidraulicaInstanciado = false;
					for (TipoSubproblemaSolver tipoSubproblemaSolver = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tipoSubproblemaSolver < TipoSubproblemaSolver_Excedente; tipoSubproblemaSolver++) {
						if (isNecessarioInstanciarSolver(idEstagio, tipoSubproblemaSolver)) {
							if (tipoSubproblemaSolver == TipoSubproblemaSolver_viabilidade_hidraulica)
								isViabilidadeHidraulicaInstanciado = true;
							else
								listaTSS.at(idEstagio).push_back(tipoSubproblemaSolver);
						}
					} // for (TipoSubproblemaSolver tipoSubproblemaSolver = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tipoSubproblemaSolver < TipoSubproblemaSolver_Excedente; tipoSubproblemaSolver++) {
					// Instancia viabilidade hidraulica por último para compatibilização com criação de estados.
					if (isViabilidadeHidraulicaInstanciado)
						listaTSS.at(idEstagio).push_back(TipoSubproblemaSolver_viabilidade_hidraulica);
				}

				for (int i = 0; i < int(listaTSS.at(idEstagio).size()); i++) {
					vetorEstagio.at(idEstagio).instanciarSolver(listaTSS.at(idEstagio).at(i), tipoSolver);
					vetorEstagio.at(idEstagio).setAtributo(AttComumEstagio_tipo_solver, tipoSolver);
					vetorEstagio.at(idEstagio).getSolver(listaTSS.at(idEstagio).at(i))->exibirNaTela(a_dados.getAtributo(AttComumDados_exibir_na_tela_resultado_solver, bool()));
					vetorEstagio.at(idEstagio).getSolver(listaTSS.at(idEstagio).at(i))->setMetodoPadrao(a_dados.getAtributo(AttComumDados_metodo_solver, TipoMetodoSolver()));
					vetorEstagio.at(idEstagio).getSolver(listaTSS.at(idEstagio).at(i))->setToleranciaOtimalidade(a_dados.getAtributo(AttComumDados_tolerancia_otimalidade, double()));
					vetorEstagio.at(idEstagio).getSolver(listaTSS.at(idEstagio).at(i))->setToleranciaViabilidade(a_dados.getAtributo(AttComumDados_tolerancia_viabilidade, double()));
					vetorEstagio.at(idEstagio).getSolver(listaTSS.at(idEstagio).at(i))->setFocoNumerico(a_dados.getAtributo(AttComumDados_foco_numerico, int()));
				} // for (int i = 0; i < int(listaTSS.size()); i++) {

		} // for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

		// **************************************************************************************************************************
		//
		//                                             Formula o modelo de otimizacao
		//
		// **************************************************************************************************************************

		const IdProcesso idProcesso = a_dados.arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());
		
		auto start_timer = std::chrono::high_resolution_clock::now();
		if (idProcesso == IdProcesso_mestre)
			std::cout << "Instanciando modelo otimizacao...";
		formularModeloOtimizacao(listaTSS, a_dados, estagio_inicial, estagio_final, horizon);
		if (idProcesso == IdProcesso_mestre)
			std::cout << "ok (" << std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_timer).count() / 60 << "min)" << std::endl;

		const IdProcesso maiorIdProcesso = a_dados.arranjoResolucao.getMaiorId(IdProcesso());

		importarVariaveisEstado_AcoplamentoPosEstudo(TipoSubproblemaSolver_geral, a_dados, idProcesso, a_entradaSaidaDados);

		importarCorteBenders(TipoSubproblemaSolver_geral, a_dados, idProcesso, std::string(a_entradaSaidaDados.getDiretorioSaida() + "//" + getFullString(a_dados.arranjoResolucao.getAtributo(AttComumArranjoResolucao_iteracao_inicial, IdIteracao())) + "//Selecao_Cortes_Importacao"), a_entradaSaidaDados);

		if (idProcesso == IdProcesso_mestre)
			exportarVariaveisEstado_AcoplamentoPreEstudo(a_entradaSaidaDados);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::formularModeloOtimizacao(): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::formularModeloOtimizacao(Dados &a_dados) {

void ModeloOtimizacao::formularModeloOtimizacao(const SmartEnupla<IdEstagio, std::vector<TipoSubproblemaSolver>>& a_listaTSS, Dados& a_dados, const IdEstagio a_idEstagioIni, const IdEstagio a_idEstagioEnd, const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>& a_horizon) {

	try{

		for (IdEstagio idEstagio = a_idEstagioIni; idEstagio <= a_idEstagioEnd; idEstagio++) {

			if (arranjoResolucao.isAnyCenarioEstado(idEstagio) || arranjoResolucao.isAnyAberturas(idEstagio)) {

				const SmartEnupla<Periodo, int> horizonSP = getElementosMatriz(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, idEstagio, Periodo(), int());
				const SmartEnupla<Periodo, double> horizon_stage = a_dados.getElementosMatriz(AttMatrizDados_percentual_duracao_horizonte_estudo, idEstagio, Periodo(), double());

				Periodo period_stage = a_dados.getElementoVetor(AttVetorDados_horizonte_otimizacao, idEstagio, Periodo());
				Periodo periodIni_stage = horizon_stage.getIteradorInicial();
				Periodo periodEnd_stage = horizon_stage.getIteradorFinal();

				for (int i = 0; i < int(a_listaTSS.at(idEstagio).size()); i++) {

					criarVariaveisCusto(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio);

					criarVariaveisDecisao_Restricoes_ProcessoEstocasticoHidrologico(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, periodIni_stage, period_stage, periodEnd_stage, horizonSP, a_horizon);

					for (Periodo period = periodIni_stage; period <= periodEnd_stage; a_horizon.incrementarIterador(period)) {

						const IdPatamarCarga idPatEnd = a_horizon.at_rIt(period).getIteradorFinal();

						Periodo periodPrev = period;
						a_horizon.decrementarIterador(periodPrev);

						Periodo periodNext = period;
						a_horizon.incrementarIterador(periodNext);

						// Variaveis Decisão

						criarVariaveisCusto(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period);

						criarVariaveisVolume(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, periodIni_stage, periodEnd_stage, periodPrev, period, periodNext);

						// Restrições

						criarRestricoesCusto(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period);

						criarRestricoesHidraulicaEspecial_vazao_afluente(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period);

						criarRestricoesHidraulicaEspecial_volume_armazenado(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, periodIni_stage, periodEnd_stage, period, periodNext, a_horizon);

						criarRestricoesHidraulicaEspecial_energia_armazenada(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, periodEnd_stage, period, periodNext);

						for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEnd; idPat++) {

							// Variaveis Decisão

							criarVariaveisCusto(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarVariaveisContrato(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarVariaveisEolicas(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarVariaveisUsinaNaoSimulada(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarVariaveisTermeletricas(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, periodIni_stage, periodPrev, period, idPat);

							criarVariaveisHidraulicas(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarVariaveisIntercambio(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarVariaveisDemanda(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarVariaveisDemandaEspecial(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarVariaveisDeficit(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarVariaveisRestricaoEletrica(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							// Restrições

							criarRestricoesCusto(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarRestricoesCustoOperacao(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarRestricoesCustoPenalidade(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarRestricoesHidraulicas(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, periodIni_stage, periodEnd_stage, periodPrev, period, periodNext, idPat);

							criarRestricoesHidraulicaEspecial_vazao_defluente(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarRestricoesTermeletrica(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, idPat, periodIni_stage, periodEnd_stage, periodPrev, period, a_horizon);

							criarRestricoesEletricas(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarRestricoesAgrupamentoIntercambio(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarRestricoesIntercambio(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarRestricoesIntercambioHidraulico(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarRestricoesInformacaoMedia(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarRestricoesAtendimentoDemanda(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarRestricoesUsinaElevatoria(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

						}// for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

						criarRestricoesCustoPenalidade(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period);

						tratarVariaveisFolga(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period);

					} // for (Periodo period = horizon_stage.getIteradorInicial(); period <= horizon_stage.getIteradorFinal(); horizon_stage.incrementarIterador(period)) {

					if (idEstagio < a_idEstagioEnd)
						criarRestricoesCorteBendersEmCustoFuturo(a_listaTSS.at(idEstagio).at(i), idEstagio);

					criarRestricoesCorteBendersEmCustoTotal(a_listaTSS.at(idEstagio).at(i), idEstagio);

				} // for (int i = 0; i < int(a_listaTSS.at(idEstagio).size()); i++) {

			} // if (!arranjoResolucao.isAnyCenarioEstado(idEstagio) && !arranjoResolucao.isAnyAberturas(idEstagio)) {

		} // for (IdEstagio idEstagio = a_idEstagioIni; idEstagio <= a_idEstagioEnd; idEstagio++) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::formularModeloOtimizacao(" + getFullString(a_idEstagioIni) + "," + "," + getFullString(a_idEstagioEnd) + "): \n" + std::string(erro.what())); }

}

void ModeloOtimizacao::criarRestricoesAgrupamentoIntercambio(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period, const IdPatamarCarga a_idPat) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const IdAgrupamentoIntercambio idAgrInIni = a_dados.getMenorId(IdAgrupamentoIntercambio());
		const IdAgrupamentoIntercambio idAgrInOut = a_dados.getIdOut(IdAgrupamentoIntercambio());
		for (IdAgrupamentoIntercambio idAgrupamentoIntercambio = idAgrInIni; idAgrupamentoIntercambio < idAgrInOut; a_dados.vetorAgrupamentoIntercambio.incr(idAgrupamentoIntercambio)) {

			if (a_dados.getSize1Matriz(idAgrupamentoIntercambio, AttMatrizAgrupamentoIntercambio_potencia_maxima) > 0) {

				const double potencia_maxima = a_dados.getElementoMatriz(idAgrupamentoIntercambio, AttMatrizAgrupamentoIntercambio_potencia_maxima, a_period, a_idPat, double());

				if (potencia_maxima > 0.0) {

					const int posIneAI = addIneLinear_PI_AGRUP(a_TSS, a_idEstagio, a_period, a_idPat, idAgrupamentoIntercambio);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneAI, -potencia_maxima);

					const int numero_intercambio = a_dados.getSizeVetor(idAgrupamentoIntercambio, AttVetorAgrupamentoIntercambio_intercambio);
					for (int i = 1; i <= numero_intercambio; i++)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_period, a_idPat, a_dados.getElementoVetor(idAgrupamentoIntercambio, AttVetorAgrupamentoIntercambio_intercambio, i, IdIntercambio())), posIneAI, -a_dados.getElementoVetor(idAgrupamentoIntercambio, AttVetorAgrupamentoIntercambio_fator_ponderacao, i, double()));

				} // if (potencia_maxima > 0.0){

			} // if (a_dados.getSize1Matriz(idAgrupamentoIntercambio, AttMatrizAgrupamentoIntercambio_potencia_maxima) > 0) {

		} // for (IdAgrupamentoIntercambio idAgrupamentoIntercambio = IdAgrupamentoIntercambio_1; idAgrupamentoIntercambio < idAgrInOut; idAgrupamentoIntercambio++) {

	}//try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesAgrupamentoIntercambio(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesEletricas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period, const IdPatamarCarga a_idPat) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const IdRestricaoEletrica idREIni = a_dados.getMenorId(IdRestricaoEletrica());
		const IdRestricaoEletrica idREOut = a_dados.getIdOut(IdRestricaoEletrica());

		for (IdRestricaoEletrica idRE = idREIni; idRE < idREOut; a_dados.vetorRestricaoEletrica.incr(idRE)) {

			if (getVarDecisao_RE_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idRE) > -1) {

				const int posIneRE = addIneLinear_RE_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idRE);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRE, a_dados.getElementoMatriz(idRE, AttMatrizRestricaoEletrica_potencia_minima, a_period, a_idPat, double()));

				// Variável RE_FINF
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RE_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idRE), posIneRE, 1.0);

				const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRE, IdElementoSistema());
				for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

					const IdHidreletrica       idUHE = a_dados.getAtributo(idRE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
					const IdConjuntoHidraulico idConjuntoHidraulico = a_dados.getAtributo(idRE, idElementoSistema, AttComumElementoSistema_conjuntohidraulico, IdConjuntoHidraulico());
					const IdUnidadeUHE         idUnidadeUHE = a_dados.getAtributo(idRE, idElementoSistema, AttComumElementoSistema_unidadehidraulica, IdUnidadeUHE());
					const IdTermeletrica       idUTE = a_dados.getAtributo(idRE, idElementoSistema, AttComumElementoSistema_termeletrica, IdTermeletrica());
					const IdUnidadeUTE         idUnidadeUTE = a_dados.getAtributo(idRE, idElementoSistema, AttComumElementoSistema_unidadeTermeletrica, IdUnidadeUTE());
					const IdIntercambio        idIntercambio = a_dados.getAtributo(idRE, idElementoSistema, AttComumElementoSistema_intercambio, IdIntercambio());
					const IdContrato           idContrato = a_dados.getAtributo(idRE, idElementoSistema, AttComumElementoSistema_contrato, IdContrato());
					const IdDemandaEspecial    idDemandaEspecial = a_dados.getAtributo(idRE, idElementoSistema, AttComumElementoSistema_demandaEspecial, IdDemandaEspecial());
					const IdUsinaEolica        idUsinaEolica = a_dados.getAtributo(idRE, idElementoSistema, AttComumElementoSistema_eolica, IdUsinaEolica());

					if (idUHE != IdHidreletrica_Nenhum) {

						const TipoDetalhamentoProducaoHidreletrica tipo_detalhamento_producao = a_dados.getAtributo(idUHE, AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica());

						// RESTRIÇÃO POR CONJUNTO
						if (idConjuntoHidraulico != IdConjuntoHidraulico_Nenhum) {

							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina)
								throw std::invalid_argument(" Nao e possivel representar a restricao eletrica" + getFullString(idRE) + " do" + getFullString(idConjuntoHidraulico) + "da" + getFullString(idUHE) + " pois o tipo de detalhamento da producao e " + getFullString(tipo_detalhamento_producao));

							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

								if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

									if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico), posIneRE, a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

								}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
								else {

									if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico), posIneRE, a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

								}//else {

							}//if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

							if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) && (idUnidadeUHE == IdUnidadeUHE_Nenhum)) {

								for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= a_dados.getMaiorId(idUHE, idConjuntoHidraulico, IdUnidadeUHE()); idUnidade++) {

									if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

										if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidade) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidade), posIneRE, a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

									}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
									else {

										if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidade) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidade), posIneRE, a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

									}//else {

								}//for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= a_dados.getMaiorId(idUHE, idConjuntoHidraulico, IdUnidadeUHE()); idUnidade++) {

							}//if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) && (idUnidadeUHE == IdUnidadeUHE_Nenhum)) {


							// RESTRIÇÃO POR UNIDADE
							if (idUnidadeUHE != IdUnidadeUHE_Nenhum) {

								if (tipo_detalhamento_producao != TipoDetalhamentoProducaoHidreletrica_por_unidade)
									throw std::invalid_argument(" Nao e possivel representar a restricao eletrica" + getFullString(idRE) + "da" + getFullString(idUnidadeUHE) + " do" + getFullString(idConjuntoHidraulico) + "da" + getFullString(idUHE) + " pois o tipo de detalhamento da producao e " + getFullString(tipo_detalhamento_producao));

								if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

									if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE), posIneRE, a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

								}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
								else {

									if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE), posIneRE, a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

								}//else {

							} // if (idUnidadeUHE != IdUnidadeUHE_Nenhum) {

						}//if (idConjuntoHidraulico != IdConjuntoHidraulico_Nenhum) {


						// RESTRIÇÃO POR HIDRELETRICA
						else if (idConjuntoHidraulico == IdConjuntoHidraulico_Nenhum) {

							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina) {

								if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

									if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posIneRE, a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

								}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
								else {

									if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posIneRE, a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

								}//else {

							}//if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina) {

							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

								for (IdConjuntoHidraulico idConjunto = IdConjuntoHidraulico_1; idConjunto <= a_dados.getMaiorId(idUHE, IdConjuntoHidraulico()); idConjunto++) {

									if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

										if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjunto) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjunto), posIneRE, a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

									}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
									else {

										if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjunto) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjunto), posIneRE, a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

									}//else {

								}//for (IdConjuntoHidraulico idConjunto = IdConjuntoHidraulico_1; idConjunto <= a_dados.getMaiorId(idUHE, IdConjuntoHidraulico()); idConjunto++) {

							}//if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

							if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {

								for (IdConjuntoHidraulico idConjunto = IdConjuntoHidraulico_1; idConjunto <= a_dados.getMaiorId(idUHE, IdConjuntoHidraulico()); idConjunto++) {

									for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= a_dados.getMaiorId(idUHE, idConjunto, IdUnidadeUHE()); idUnidade++) {

										if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

											if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjunto, idUnidade) > -1)
												vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjunto, idUnidade), posIneRE, a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));


										}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
										else {

											if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjunto, idUnidade) > -1)
												vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjunto, idUnidade), posIneRE, a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

										}//else {

									}//for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= a_dados.getMaiorId(idUHE, idConjunto, IdUnidadeUHE()); idUnidade++) {

								}//for (IdConjuntoHidraulico idConjunto = IdConjuntoHidraulico_1; idConjunto <= a_dados.getMaiorId(idUHE, IdConjuntoHidraulico()); idConjunto++) {

							}//if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {

						}//else if (idConjuntoHidraulico == IdConjuntoHidraulico_Nenhum) {

					} // if (idUHE != IdHidreletrica_Nenhum) {

					// RESTRIÇÃO PARA AS TERMELETRICAS 
					else if (idUTE != IdTermeletrica_Nenhum) {

						const TipoDetalhamentoProducaoTermeletrica tipo_detalhamento_producao = a_dados.getAtributo(idUTE, AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica());

						// RESTIÇÃO POR UNIDADE
						if (idUnidadeUTE != IdUnidadeUTE_Nenhum) {

							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina)
								throw std::invalid_argument(" Nao e possivel representar a restricao eletrica" + getFullString(idRE) + "da" + getFullString(idUnidadeUTE) + "da" + getFullString(idUTE) + " pois o tipo de detalhamento da producao e " + getFullString(tipo_detalhamento_producao));

							if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {

								if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE) > -1)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posIneRE, a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

							}//if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {
							else {

								if (getVarDecisao_PTseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE) > -1)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posIneRE, a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

							}//else {

						} // if (idUnidadeUTE != IdUnidadeUTE_Nenhum) {


						// RESTRIÇÃO POR USINA
						else if (idUnidadeUTE == IdUnidadeUTE_Nenhum) {

							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {

								for (IdUnidadeUTE idUnidade = IdUnidadeUTE_1; idUnidade <= a_dados.getMaiorId(idUTE, IdUnidadeUTE()); idUnidade++) {

									if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {

										if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidade) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidade), posIneRE, a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

									}//if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {
									else {

										if (getVarDecisao_PTseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidade) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidade), posIneRE, a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

									}//else {

								}//for (IdUnidadeUTE idUnidade = IdUnidadeUTE_1; idUnidade <= a_dados.getMaiorId(idUTE, IdUnidadeUTE()); idUnidade++) {

							}//if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {

							else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina) {

								if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {

									if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), posIneRE, a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

								}//if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {
								else {

									if (getVarDecisao_PTseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), posIneRE, a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

								}//else {

							}//else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina) {

						} // else if (idUnidadeUTE == IdUnidadeUTE_Nenhum) {

					} // else if (idUTE != IdTermeletrica_Nenhum) {

					// RESTRIÇÃO DE PI
					else if (idIntercambio != IdIntercambio_Nenhum) {
						if (getVarDecisao_PIseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio))
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio), posIneRE, a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

					}//else if (idIntercambio != IdIntercambio_Nenhum) {

					// CONTRATOS
					else if (idContrato != IdContrato_Nenhum) {

						if ((getVarDecisao_CEXPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idContrato) > -1))
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_CEXP(a_TSS, a_idEstagio, a_period, a_idPat, idContrato), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

						else if ((getVarDecisao_CIMPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idContrato) > -1))
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_CIMP(a_TSS, a_idEstagio, a_period, a_idPat, idContrato), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

						else
							throw std::invalid_argument("Nao encontrado " + getFullString(idContrato) + " na " + getFullString(idRE));
					}//else if (idContrato != IdContrato_Nenhum) {		

					// PL ESPECIAL
					else if (idDemandaEspecial != IdDemandaEspecial_Nenhum)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PLE(a_TSS, a_idEstagio, a_period, a_idPat, idDemandaEspecial), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

					// PL ESPECIAL
					else if (idUsinaEolica != IdUsinaEolica_Nenhum)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PE(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaEolica), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

					else
						throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRE));

				} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

			} // if (getVarDecisao_RE_FINFseExistir(a_idEstagio, a_period, a_idPat, idRE) > -1){


			// RESTRIÇÃO DE FOLGA SUPERIOR
			if (getVarDecisao_RE_FSUPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idRE) > -1) {

				const int posIneRE = addIneLinear_RE_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idRE);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRE, -a_dados.getElementoMatriz(idRE, AttMatrizRestricaoEletrica_potencia_maxima, a_period, a_idPat, double()));

				// Variável RE_FSUP
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RE_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idRE), posIneRE, 1.0);

				const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRE, IdElementoSistema());
				for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

					const IdHidreletrica       idUHE = a_dados.getAtributo(idRE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
					const IdConjuntoHidraulico idConjuntoHidraulico = a_dados.getAtributo(idRE, idElementoSistema, AttComumElementoSistema_conjuntohidraulico, IdConjuntoHidraulico());
					const IdUnidadeUHE         idUnidadeUHE = a_dados.getAtributo(idRE, idElementoSistema, AttComumElementoSistema_unidadehidraulica, IdUnidadeUHE());
					const IdTermeletrica       idUTE = a_dados.getAtributo(idRE, idElementoSistema, AttComumElementoSistema_termeletrica, IdTermeletrica());
					const IdContrato           idContrato = a_dados.getAtributo(idRE, idElementoSistema, AttComumElementoSistema_contrato, IdContrato());
					const IdUnidadeUTE         idUnidadeUTE = a_dados.getAtributo(idRE, idElementoSistema, AttComumElementoSistema_unidadeTermeletrica, IdUnidadeUTE());
					const IdIntercambio        idIntercambio = a_dados.getAtributo(idRE, idElementoSistema, AttComumElementoSistema_intercambio, IdIntercambio());
					const IdDemandaEspecial    idDemandaEspecial = a_dados.getAtributo(idRE, idElementoSistema, AttComumElementoSistema_demandaEspecial, IdDemandaEspecial());
					const IdUsinaEolica        idUsinaEolica = a_dados.getAtributo(idRE, idElementoSistema, AttComumElementoSistema_eolica, IdUsinaEolica());

					if (idUHE != IdHidreletrica_Nenhum) {

						const TipoDetalhamentoProducaoHidreletrica tipo_detalhamento_producao = a_dados.getAtributo(idUHE, AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica());

						// RESTRIÇÃO POR CONJUNTO
						if (idConjuntoHidraulico != IdConjuntoHidraulico_Nenhum) {

							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina)
								throw std::invalid_argument(" Nao e possivel representar a restricao eletrica" + getFullString(idRE) + " do" + getFullString(idConjuntoHidraulico) + "da" + getFullString(idUHE) + " pois o tipo de detalhamento da producao e " + getFullString(tipo_detalhamento_producao));

							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

								if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

									if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

								}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
								else {

									if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

								}//else {

							}

							if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) && (idUnidadeUHE == IdUnidadeUHE_Nenhum)) {

								for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= a_dados.getMaiorId(idUHE, idConjuntoHidraulico, IdUnidadeUHE()); idUnidade++) {

									if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

										if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidade) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidade), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

									}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
									else {

										if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidade) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidade), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

									}//else {

								}//for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= a_dados.getMaiorId(idUHE, idConjuntoHidraulico, IdUnidadeUHE()); idUnidade++)

							}//if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) && (idUnidadeUHE == IdUnidadeUTE_Nenhum)) {


							// RESTRIÇÃO POR UNIDADE
							if (idUnidadeUHE != IdUnidadeUHE_Nenhum) {

								if (tipo_detalhamento_producao != TipoDetalhamentoProducaoHidreletrica_por_unidade)
									throw std::invalid_argument(" Nao e possivel representar a restricao eletrica" + getFullString(idRE) + "da" + getFullString(idUnidadeUHE) + " do" + getFullString(idConjuntoHidraulico) + "da" + getFullString(idUHE) + " pois o tipo de detalhamento da producao e " + getFullString(tipo_detalhamento_producao));

								if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

									if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

								}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
								else {

									if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

								}//else {

							} // if (idUnidadeUTE != IdUnidadeUHE_Nenhum) {

						}//if (idConjuntoHidraulico != IdConjuntoHidraulico_Nenhum) {


						// RESTRIÇÃO POR HIDRELETRICA
						else if (idConjuntoHidraulico == IdConjuntoHidraulico_Nenhum) {

							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina) {

								if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

									if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

								}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
								else {

									if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

								}//else {

							}//if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina) {


							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

								for (IdConjuntoHidraulico idConjunto = IdConjuntoHidraulico_1; idConjunto <= a_dados.getMaiorId(idUHE, IdConjuntoHidraulico()); idConjunto++) {

									if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

										if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjunto) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjunto), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

									}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
									else {

										if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjunto) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjunto), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

									}//else {

								}//for (IdConjuntoHidraulico idConjunto = IdConjuntoHidraulico_1; idConjunto <= a_dados.getMaiorId(idUHE, IdConjuntoHidraulico()); idConjunto++) {

							}//if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

							if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {

								for (IdConjuntoHidraulico idConjunto = IdConjuntoHidraulico_1; idConjunto <= a_dados.getMaiorId(idUHE, IdConjuntoHidraulico()); idConjunto++) {

									for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= a_dados.getMaiorId(idUHE, idConjunto, IdUnidadeUHE()); idUnidade++) {

										if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

											if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjunto, idUnidade) > -1)
												vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjunto, idUnidade), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

										}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
										else {

											if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjunto, idUnidade) > -1)
												vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjunto, idUnidade), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

										}//else {									

									}//for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= a_dados.getMaiorId(idUHE, idConjunto, IdUnidadeUHE()); idUnidade++) {

								}//for (IdConjuntoHidraulico idConjunto = IdConjuntoHidraulico_1; idConjunto <= a_dados.getMaiorId(idUHE, IdConjuntoHidraulico()); idConjunto++) {

							}//if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {

						}//else if (idConjuntoHidraulico == IdConjuntoHidraulico_Nenhum) {

					} // if (idUHE != IdHidreletrica_Nenhum) {


					// RESTRIÇÃO PARA AS TERMELETRICAS 
					else if (idUTE != IdTermeletrica_Nenhum) {

						const TipoDetalhamentoProducaoTermeletrica tipo_detalhamento_producao = a_dados.getAtributo(idUTE, AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica());

						// RESTIÇÃO POR UNIDADE
						if (idUnidadeUTE != IdUnidadeUTE_Nenhum) {

							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina)
								throw std::invalid_argument(" Nao e possivel representar a restricao eletrica" + getFullString(idRE) + "da" + getFullString(idUnidadeUTE) + "da" + getFullString(idUTE) + " pois o tipo de detalhamento da producao e " + getFullString(tipo_detalhamento_producao));

							if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {

								if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE) > -1)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

							}//if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {
							else {

								if (getVarDecisao_PTseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE) > -1)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

							}//else {					

						} // if (idUnidadeUTE != IdUnidadeUTE_Nenhum) {


						// RESTRIÇÃO POR USINA
						else if (idUnidadeUTE == IdUnidadeUTE_Nenhum) {

							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {

								for (IdUnidadeUTE idUnidade = IdUnidadeUTE_1; idUnidade <= a_dados.getMaiorId(idUTE, IdUnidadeUTE()); idUnidade++) {

									if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {

										if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidade) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidade), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

									}//if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {
									else {

										if (getVarDecisao_PTseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidade) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidade), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

									}//else {

								}//for (IdUnidadeUTE idUnidade = IdUnidadeUTE_1; idUnidade <= a_dados.getMaiorId(idUTE, IdUnidadeUTE()); idUnidade++) {

							}//if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {

							else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina) {

								if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {

									if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

								}//if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {
								else {

									if (getVarDecisao_PTseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

								}//else {

							}//else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina) {

						} // else if (idUnidadeUTE == IdUnidadeUTE_Nenhum) {

					} // else if (idUTE != IdTermeletrica_Nenhum) {


					// RESTRIÇÃO DE PI
					else if (idIntercambio != IdIntercambio_Nenhum) {
						if (getVarDecisao_PIseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio))
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));
					}//else if (idIntercambio != IdIntercambio_Nenhum) {


					// CONTRATOS
					else if (idContrato != IdContrato_Nenhum) {

						if ((getVarDecisao_CEXPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idContrato) > -1))
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_CEXP(a_TSS, a_idEstagio, a_period, a_idPat, idContrato), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

						else if ((getVarDecisao_CIMPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idContrato) > -1))
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_CIMP(a_TSS, a_idEstagio, a_period, a_idPat, idContrato), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

						else
							throw std::invalid_argument("Nao encontrado " + getFullString(idContrato) + " na " + getFullString(idRE));
					}//else if (idContrato != IdContrato_Nenhum) {

					// PL ESPECIAL
					else if (idDemandaEspecial != IdDemandaEspecial_Nenhum)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PLE(a_TSS, a_idEstagio, a_period, a_idPat, idDemandaEspecial), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

					// PL ESPECIAL
					else if (idUsinaEolica != IdUsinaEolica_Nenhum)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PE(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaEolica), posIneRE, -a_dados.getElementoMatriz(idRE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));



					else
						throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRE));

				} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

			} // if (getVarDecisao_RE_FINFseExistir(a_idEstagio, a_period, a_idPat, idRE) > -1){

		} // for (IdRestricaoEletrica idRE = IdRestricaoEletrica_1; idRE < maiorIdRestricaoEletrica; idRE++) {

	}
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesEletricas(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
}

void ModeloOtimizacao::criarRestricoesCorteBendersEmCustoFuturo(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const IdEstagio idEstagio_seguinte = IdEstagio(a_idEstagio + 1);

		const int multiplicidade_corte = getAtributo(idEstagio_seguinte, AttComumEstagio_cortes_multiplos, int());

		if (multiplicidade_corte == 0) {

			const int posIneZF = addIneLinear_CB_ZF(a_TSS, a_idEstagio, IdRealizacao_1, IdCorteBenders_1);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneZF, 0.0);

			if (getVarDecisao_ZFseExistir(a_TSS, a_idEstagio, IdRealizacao_1) == -1)
				addVarDecisao_ZF(a_TSS, a_idEstagio, IdRealizacao_1, 0.0, vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZF(a_TSS, a_idEstagio, IdRealizacao_1), posIneZF, 1.0);

		} // if (multiplicidade_corte == 0) {

		else if (multiplicidade_corte > 0) {

			for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getAtributo(idEstagio_seguinte, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); idRealizacao++) {
				const int posIneZF = addIneLinear_CB_ZF(a_TSS, a_idEstagio, idRealizacao, IdCorteBenders_1);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneZF, 0.0);

				if (getVarDecisao_ZFseExistir(a_TSS, a_idEstagio, idRealizacao) == -1)
					addVarDecisao_ZF(a_TSS, a_idEstagio, idRealizacao, 0.0, vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZF(a_TSS, a_idEstagio, idRealizacao), posIneZF, 1.0);
			} // for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getAtributo(a_idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); idRealizacao++) {

		} // if (multiplicidade_corte > 0) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesCorteBendersEmCustoFuturo(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }

}

void ModeloOtimizacao::criarRestricoesCorteBendersEmCustoTotal(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio) {

	try {

		if (a_TSS != TipoSubproblemaSolver_mestre)
			return;

		const int multiplicidade_corte = getAtributo(a_idEstagio, AttComumEstagio_cortes_multiplos, int());

		if (multiplicidade_corte == 0) {

			const int posIneZT = addIneLinear_CB_ZT(a_TSS, a_idEstagio, IdRealizacao_1, IdCorteBenders_1);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneZT, 0.0);

			if (getVarDecisao_ZTseExistir(a_TSS, a_idEstagio, IdRealizacao_1) == -1)
				addVarDecisao_ZT(a_TSS, a_idEstagio, IdRealizacao_1, 0.0, vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZT(a_TSS, a_idEstagio, IdRealizacao_1), posIneZT, 1.0);

		} // if (multiplicidade_corte == 0) {

		else if (multiplicidade_corte > 0) {

			for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getAtributo(a_idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); idRealizacao++) {
				const int posIneZT = addIneLinear_CB_ZT(a_TSS, a_idEstagio, idRealizacao, IdCorteBenders_1);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneZT, 0.0);

				if (getVarDecisao_ZTseExistir(a_TSS, a_idEstagio, idRealizacao) == -1)
					addVarDecisao_ZT(a_TSS, a_idEstagio, idRealizacao, 0.0, vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZT(a_TSS, a_idEstagio, idRealizacao), posIneZT, 1.0);
			} // for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getAtributo(a_idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); idRealizacao++) {

		} // if (multiplicidade_corte > 0) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesCorteBendersEmCustoTotal(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }

}


void ModeloOtimizacao::criarRestricoesVazaoBombeada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period, const IdPatamarCarga a_idPat, const IdUsinaElevatoria a_idUsinaElevatoria) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (getVarDecisao_QB_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idUsinaElevatoria) > -1) {

			const int posIneQB_FINF = addIneLinear_QB_FINF(a_TSS, a_idEstagio, a_period, a_idPat, a_idUsinaElevatoria);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneQB_FINF, a_dados.getElementoVetor(a_idUsinaElevatoria, AttVetorUsinaElevatoria_vazao_bombeada_minima, a_period, double()));

			// Variável QB_FINF
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QB_FINF(a_TSS, a_idEstagio, a_period, a_idPat, a_idUsinaElevatoria), posIneQB_FINF, 1.0);

			// Variável QB
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QB(a_TSS, a_idEstagio, a_period, a_idPat, a_idUsinaElevatoria), posIneQB_FINF, 1.0);

		} // if (getVarDecisao_QB_FINFseExistir(a_idEstagio, a_period, a_idPat, a_idUsinaElevatoria) > -1) {

		if (getVarDecisao_QBDISP_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idUsinaElevatoria) > -1) {

			const int posIneQBDISP_FINF = addIneLinear_QBDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, a_idUsinaElevatoria);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneQBDISP_FINF, a_dados.getElementoMatriz(a_idUsinaElevatoria, AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_minima, a_period, a_idPat, double()));

			// Variável QBDISP_FINF
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, a_idUsinaElevatoria), posIneQBDISP_FINF, 1.0);

			// Variável QBDISP
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idUsinaElevatoria), posIneQBDISP_FINF, 1.0);

		} // if (getVarDecisao_QBDISP_FINFseExistir(a_idEstagio, a_period, a_idPat, a_idUsinaElevatoria) > -1) {

		// Restrição QBDISP
		if (getVarDecisao_QBDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idUsinaElevatoria) > -1) {

			const int posEquQBDISP = addEquLinear_QBDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idUsinaElevatoria);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQBDISP, 0.0);

			// QBDISP
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idUsinaElevatoria), posEquQBDISP, 1.0);

			// QB
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QB(a_TSS, a_idEstagio, a_period, a_idPat, a_idUsinaElevatoria), posEquQBDISP, -a_dados.getElementoVetor(a_idUsinaElevatoria, AttVetorUsinaElevatoria_fator_disponibilidade, a_period, double()));

		}//if (getVarDecisao_QBDISPseExistir(a_idEstagio, a_period, a_idPat, a_idUsinaElevatoria) > -1) {

	}

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesVazaoBombeada(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesVazaoRetirada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPat) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (getVarDecisao_QI_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1) {

			const int posEquQI_FINF = addEquLinear_QI_FINF(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQI_FINF, a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_vazao_retirada, a_period, double()));

			// Variável QI_FINF
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI_FINF(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquQI_FINF, 1.0);

			// Variável QI
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquQI_FINF, 1.0);

		} // if (getVarDecisao_QI_FINFseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1) {

	}

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesvazaoRetirada(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesVazaoDesviada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPat) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (getVarDecisao_QR_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica())) > -1) {

			const int posEquQR_FINF = addIneLinear_QR_FINF(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica()));

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQR_FINF, a_dados.getElementoMatriz(a_idHidreletrica, AttMatrizHidreletrica_vazao_desviada_minima, a_period, a_idPat, double()));

			// Variável QR_FINF
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QR_FINF(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posEquQR_FINF, 1.0);

			// Variável QR
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QR(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posEquQR_FINF, 1.0);

		} // if (getVarDecisao_QR_FINFseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica())) > -1) {

	}

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesvazaoDesviada(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesVolumeUtil_e_Penalidade(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period, const IdHidreletrica a_idHidreletrica) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (getVarDecisao_VF_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1) {

			const int posIneVF_FINF = addIneLinear_VF_FINF(a_TSS, a_idEstagio, a_period, a_idHidreletrica);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneVF_FINF, a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_minimo, a_period, double()));

			// Variável VF_FINF
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF_FINF(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posIneVF_FINF, 1.0);

			// Variável VF
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posIneVF_FINF, 1.0);


			const IdMes mes_penalizacao_volume_util_minimo = a_dados.getAtributo(AttComumDados_mes_penalizacao_volume_util_minimo, IdMes());
			if (mes_penalizacao_volume_util_minimo != IdMes_Nenhum) {
				Periodo periodo_penalizacao;
				if (a_period == getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_estudo, a_idEstagio, Periodo()))
					criarVariaveisDecisao_VariaveisEstado_Restricoes_ZP0_VF_FINF(a_TSS, a_dados, a_idEstagio, periodo_penalizacao);
			}

			else {

				const Periodo periodo_otimizacao = getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

				if (getVarDecisao_ZP0_VF_FINFseExistir(a_TSS, a_idEstagio, periodo_otimizacao) == -1) {

					addEquLinear_ZP0_VF_FINF(a_TSS, a_idEstagio, periodo_otimizacao);

					addVarDecisao_ZP0_VF_FINF(a_TSS, a_idEstagio, periodo_otimizacao, 0.0, vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZP0_VF_FINF(a_TSS, a_idEstagio, periodo_otimizacao), getEquLinear_ZP0_VF_FINF(a_TSS, a_idEstagio, periodo_otimizacao), 1.0);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZP0_VF_FINF(a_TSS, a_idEstagio, periodo_otimizacao), getEquLinear_ZP0(a_TSS, a_idEstagio), -1.0);

				} // if (getVarDecisao_ZP0_VF_FINFseExistir(a_TSS, a_idEstagio, periodo_otimizacao) == -1) {

				const double valor_penalidade = a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_penalidade_volume_util_minimo, double()) * a_dados.getElementoMatriz(AttMatrizDados_desagio_acumulado_horizonte_estudo, a_idEstagio, a_period, double());
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF_FINF(a_TSS, a_idEstagio, a_period, a_idHidreletrica), getEquLinear_ZP0_VF_FINF(a_TSS, a_idEstagio, periodo_otimizacao), -valor_penalidade);

			} // else {


		}//if (getVarDecisao_VI_FINFseExistir(a_idEstagio, a_period, a_idHidreletrica) > -1) {

	}

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesVolumeUtil_e_Penalidade(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesVazaoDefluente(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPat) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		// Por Periodo

		if (a_idPat == IdPatamarCarga_1) {

			if (getVarDecisao_QD_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1) {

				const int posIneQD_FINF = addIneLinear_QD_FINF(a_TSS, a_idEstagio, a_period, a_idHidreletrica);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneQD_FINF, a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_vazao_defluente_minima, a_period, double()));

				// Variável QD_FINF
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD_FINF(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posIneQD_FINF, 1.0);

				// Variável QD
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posIneQD_FINF, 1.0);

			} // if (getVarDecisao_QD_FINFseExistir(a_idEstagio, a_period, a_idHidreletrica) > -1){

			if (getVarDecisao_QD_FSUPseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1) {

				const int posIneQD_FSUP = addIneLinear_QD_FSUP(a_TSS, a_idEstagio, a_period, a_idHidreletrica);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneQD_FSUP, -a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_vazao_defluente_maxima, a_period, double()));

				// Variável QD_FSUP
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD_FSUP(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posIneQD_FSUP, 1.0);

				// Variável QD
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posIneQD_FSUP, -1.0);

			} // if (getVarDecisao_QD_FSUPseExistir(a_idEstagio, a_period, a_idHidreletrica) > -1){

		} // if (a_idPat == IdPatamarCarga_1){

		// Por Patamar

		if (getVarDecisao_QD_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1) {

			const int posIneQD_FINF = addIneLinear_QD_FINF(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneQD_FINF, a_dados.getElementoMatriz(a_idHidreletrica, AttMatrizHidreletrica_vazao_defluente_minima, a_period, a_idPat, double()));

			// Variável QD_FINF
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD_FINF(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posIneQD_FINF, 1.0);

			// Variável QD
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posIneQD_FINF, 1.0);

		} // if (getVarDecisao_QD_FINFseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1) {

		if (getVarDecisao_QD_FSUPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1) {

			const int posIneQD_FSUP = addIneLinear_QD_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneQD_FSUP, -a_dados.getElementoMatriz(a_idHidreletrica, AttMatrizHidreletrica_vazao_defluente_maxima, a_period, a_idPat, double()));

			// Variável QD_FSUP
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posIneQD_FSUP, 1.0);

			// Variável QD
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posIneQD_FSUP, -1.0);

		} // if (getVarDecisao_QD_FSUPseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1){


	}

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesvazaoDefluente(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}

void ModeloOtimizacao::criarRestricoesBombeamentoHidraulico(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period, const IdPatamarCarga a_idPat, const IdUsinaElevatoria a_idUsinaElevatoria) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		if (getVarDecisao_QBseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idUsinaElevatoria) > -1) {

			const int posEquBOM = addEquLinear_PBDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idUsinaElevatoria);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquBOM, 0.0);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PBDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idUsinaElevatoria), posEquBOM, 1.0);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QB(a_TSS, a_idEstagio, a_period, a_idPat, a_idUsinaElevatoria), posEquBOM, -a_dados.getAtributo(a_idUsinaElevatoria, AttComumUsinaElevatoria_taxa_de_consumo, double()) * a_dados.getElementoVetor(a_idUsinaElevatoria, AttVetorUsinaElevatoria_fator_disponibilidade, a_period, double()));

		} // if (getVarDecisao_QBseExistir(a_idEstagio, a_period, a_idPat, idUsinaElevatoria) > -1) {

	}//try {

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesBombeamentoHidraulico(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }

}


void ModeloOtimizacao::criarRestricoesUsinaElevatoria(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat) {

	try {

		const IdUsinaElevatoria idUsiElHIni = a_dados.getMenorId(IdUsinaElevatoria());
		const IdUsinaElevatoria idUsiElHOut = a_dados.getIdOut(IdUsinaElevatoria());
		for (IdUsinaElevatoria idUsinaElevatoria = idUsiElHIni; idUsinaElevatoria < idUsiElHOut; a_dados.vetorUsinaElevatoria.incr(idUsinaElevatoria)) {
			criarRestricoesBombeamentoHidraulico(a_TSS, a_dados, a_idEstagio, a_period, a_idPat, idUsinaElevatoria);
			criarRestricoesVazaoBombeada(a_TSS, a_dados, a_idEstagio, a_period, a_idPat, idUsinaElevatoria);

		}//for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria < a_dados.getMaiorId(IdUsinaElevatoria()); idUsinaElevatoria++) {

	}//try {

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesUsinaElevatoria(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }

}


void ModeloOtimizacao::criarRestricoesIntercambio(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period, const IdPatamarCarga a_idPat) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const IdIntercambio idInterIni = a_dados.getMenorId(IdIntercambio());
		const IdIntercambio idInterOut = a_dados.getIdOut(IdIntercambio());

		for (IdIntercambio idIntercambio = idInterIni; idIntercambio < idInterOut; a_dados.vetorIntercambio.incr(idIntercambio)) {

			// Restrição QTDISP_REL
			if (getVarDecisao_PI_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio) > -1) {

				const int posInePI_FINF = addIneLinear_PI_REL(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posInePI_FINF, a_dados.getElementoMatriz(idIntercambio, AttMatrizIntercambio_potencia_minima, a_period, a_idPat, double()));

				// PI
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio), posInePI_FINF, 1.0);

				// PI_FINF
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio), posInePI_FINF, 1.0);

			} // if (getVarDecisao_PI_FINFseExistir(a_idEstagio, a_period, a_idPat, idIntercambio) > -1) {

		}

	}//try {

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesIntercambio(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }

}

void ModeloOtimizacao::getIdVariavelAleatoriaIdVariavelAleatoriaInternaFromIdHidreletrica(const IdProcessoEstocastico a_idProcessoEstocastico, IdVariavelAleatoria& a_idVariavelAleatoria, IdVariavelAleatoriaInterna& a_idVariavelAleatoriaInterna, const IdHidreletrica a_hidreletrica){

	try {


		for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= getMaiorId(a_idProcessoEstocastico, IdVariavelAleatoria()); idVar++) {

			for (IdVariavelAleatoriaInterna idVarInterna = IdVariavelAleatoriaInterna_1; idVarInterna <= getMaiorId(a_idProcessoEstocastico, idVar, IdVariavelAleatoriaInterna()); idVarInterna++) {
				if (getIdHidreletricaFromIdProcessoEstocasticoIdVariavelAleatoriaIdVariavelAleatoriaInterna(a_idProcessoEstocastico, idVar, idVarInterna) == a_hidreletrica) {
					a_idVariavelAleatoria = idVar;
					a_idVariavelAleatoriaInterna = idVarInterna;
					return;
				}
			}
		}


		throw std::invalid_argument("Hidreletrica nao encontrada.");

	}
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getIdProcessoEstocasticoIdVariavelAleatoriaIdVariavelAleatoriaInternaFromIdHidreletrica(" + getFullString(a_idProcessoEstocastico) + "," + getFullString(a_idVariavelAleatoria) + "," + getFullString(a_idVariavelAleatoriaInterna) + "," + getFullString(a_hidreletrica) + "): \n" + std::string(erro.what())); }

}

IdVariavelAleatoriaInterna ModeloOtimizacao::getIdVariavelAleatoriaInternaFromIdVariavelAleatoriaIdHidreletrica(const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria, const IdHidreletrica a_hidreletrica) {

	try {

		for (IdVariavelAleatoriaInterna idVarInterna = IdVariavelAleatoriaInterna_1; idVarInterna <= getMaiorId(a_idProcessoEstocastico, a_idVariavelAleatoria, IdVariavelAleatoriaInterna()); idVarInterna++) {
			if (getIdHidreletricaFromIdProcessoEstocasticoIdVariavelAleatoriaIdVariavelAleatoriaInterna(a_idProcessoEstocastico, a_idVariavelAleatoria, idVarInterna) == a_hidreletrica)
				return idVarInterna;
		}

		throw std::invalid_argument("Hidreletrica nao encontrada.");

	}
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getIdVariavelAleatoriaInternaFromIdVariavelAleatoriaIdHidreletrica(" + getFullString(a_idProcessoEstocastico) + "," + getFullString(a_idVariavelAleatoria) + "," + getFullString(a_hidreletrica) + "): \n" + std::string(erro.what())); }

}

IdHidreletrica ModeloOtimizacao::getIdHidreletricaFromIdProcessoEstocasticoIdVariavelAleatoriaIdVariavelAleatoriaInterna(const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria, const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna){

	try {

		std::string valor = getAtributo(a_idProcessoEstocastico, a_idVariavelAleatoria, a_idVariavelAleatoriaInterna, AttComumVariavelAleatoriaInterna_nome, std::string());
		
		const size_t pos_ = valor.find("_");
		if (pos_ != std::string::npos)
			valor = valor.substr(pos_ + 1, valor.length());

		const IdHidreletrica idUHE = getIdHidreletricaFromChar(valor.c_str());
		if (idUHE == IdHidreletrica_Nenhum)
			throw std::invalid_argument("Hidreletrica invalida em " + getFullString(a_idVariavelAleatoriaInterna));

		return idUHE;

	}
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getIdHidreletricaFromIdProcessoEstocasticoIdVariavelAleatoriaIdVariavelAleatoriaInterna(" + getFullString(a_idProcessoEstocastico) + "," + getFullString(a_idVariavelAleatoria) + "," + getFullString(a_idVariavelAleatoriaInterna) + "): \n" + std::string(erro.what())); }

}

std::vector<IdHidreletrica> ModeloOtimizacao::getIdHidreletricaFromIdProcessoEstocasticoIdVariavelAleatoria(const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria){

	try {

		std::vector<IdHidreletrica> listaIdHidreletrica;

		const IdVariavelAleatoriaInterna maiorVarInterna = getMaiorId(a_idProcessoEstocastico, a_idVariavelAleatoria, IdVariavelAleatoriaInterna());

		for (IdVariavelAleatoriaInterna idVarInterna = IdVariavelAleatoriaInterna_1; idVarInterna <= maiorVarInterna; idVarInterna++)
			listaIdHidreletrica.push_back(getIdHidreletricaFromIdProcessoEstocasticoIdVariavelAleatoriaIdVariavelAleatoriaInterna(a_idProcessoEstocastico, a_idVariavelAleatoria, idVarInterna));

		return listaIdHidreletrica;

	}
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getIdHidreletricaFromIdProcessoEstocasticoIdVariavelAleatoria(" + getFullString(a_idProcessoEstocastico) + "," + getFullString(a_idVariavelAleatoria) + "): \n" + std::string(erro.what())); }

}


void ModeloOtimizacao::criarRestricoesTurbinamentoHidraulico(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPat) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		const TipoDetalhamentoProducaoHidreletrica tipo_detalhamento_producao = a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica());

		//
		// Turbinamento Por Hidreletrica
		//

		// Restrição QT
		if (getVarDecisao_QTseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1) {

			if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) || (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {

				const int posEquQT = addEquLinear_QT(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQT, 0.0);

				// Variável Turbinamento Hidraulico QT
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquQT, 1.0);

				for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posEquQT, -1.0);

			} // if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) || (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {

		} // if (getVarDecisao_QTseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1){

		// Restrição QT_REL
		if (getVarDecisao_QT_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1) {

			const int posIneQT_FINF = addIneLinear_QT_REL(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneQT_FINF, a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_vazao_turbinada_minima, a_period, double()));

			// Variável Turbinamento Hidraulico QT
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posIneQT_FINF, 1.0);

			// Variável Turbinamento Hidraulico Folga Minima QT_FINF
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QT_FINF(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posIneQT_FINF, 1.0);

		} // if (getVarDecisao_QT_RELseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1){

		// Restrição QTDISP_REL
		if (getVarDecisao_QTDISP_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1) {

			const int posIneQTDISP_FINF = addIneLinear_QTDISP_REL(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneQTDISP_FINF, a_dados.getElementoMatriz(a_idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_minima, a_period, a_idPat, double()));

			// Variável Turbinamento Hidraulico QTDISP
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posIneQTDISP_FINF, 1.0);

			// Variável Turbinamento Hidraulico Folga Minima QTDISP_FINF
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posIneQTDISP_FINF, 1.0);

		} // if (getVarDecisao_QTDISP_RELseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1){

		// Restrição QTDISP
		if (getVarDecisao_QTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1) {

			const int posEquQTDISP = addEquLinear_QTDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQTDISP, 0.0);

			// Variável Turbinamento Hidraulico QTDISP
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquQTDISP, 1.0);

			if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquQTDISP, -a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_disponibilidade, a_period, double()));

			else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {
				for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posEquQTDISP, -1.0);
			} // else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

			else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) {
				for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {
					for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquQTDISP, -1.0);
				}
			} // else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) {

		} // if (getVarDecisao_QTDISPseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1){

		//
		// Turbinamento Por Conjunto Hidraulico
		//

		for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

			// Restrição QT
			if (getVarDecisao_QTseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico) > -1) {

				if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) {

					const int posEquQT = addEquLinear_QT(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQT, 0.0);

					// Variável Turbinamento Hidraulico QT
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posEquQT, 1.0);

					for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquQT, -1.0);

				} // if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) {

			} // if (getVarDecisao_QTseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico) > -1){

			// Restrição QT_REL
			if (getVarDecisao_QT_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico) > -1) {

				const int posEquQT_FINF = addEquLinear_QT_REL(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQT_FINF, a_dados.getElementoVetor(a_idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_minima, a_period, double()));

				// Variável Turbinamento Hidraulico QT
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posEquQT_FINF, -1.0);

				// Variável Turbinamento Hidraulico Folga Minima QT_FINF
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QT_FINF(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posEquQT_FINF, -1.0);

			} // if (getVarDecisao_QT_FINFseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico) > -1) {

			// Restrição QTDISP
			if (getVarDecisao_QTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico) > -1) {

				const int posEquQTDISP = addEquLinear_QTDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQTDISP, 0.0);

				// Variável Turbinamento Hidraulico QTDISP
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posEquQTDISP, 1.0);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posEquQTDISP, -a_dados.getElementoVetor(a_idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_disponibilidade, a_period, double()));

			} // if (getVarDecisao_QTDISPseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico) > -1){

			//
			// Turbinamento Por Unidade UHE
			//

			for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {

				// Restrição QT_REL
				if (getVarDecisao_QT_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

					const int posEquQT_FINF = addEquLinear_QT_REL(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQT_FINF, a_dados.getElementoVetor(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_vazao_turbinada_minima, a_period, double()));

					// Variável Turbinamento Hidraulico QT
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquQT_FINF, 1.0);

					// Variável Turbinamento Hidraulico Folga Minima QT_FINF
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QT_FINF(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquQT_FINF, 1.0);

				} // if (getVarDecisao_QT_FINFseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

				// Restrição QTDISP
				if (getVarDecisao_QTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

					const int posEquQTDISP = addEquLinear_QTDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQTDISP, 0.0);

					// Variável Turbinamento Hidraulico QTDISP
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquQTDISP, 1.0);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquQTDISP, -a_dados.getElementoVetor(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_disponibilidade, a_period, double()));

				} // if (getVarDecisao_QTDISPseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1){

			} // for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {
		} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

	}//try {

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesTurbinamentoHidraulico(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }

}

void ModeloOtimizacao::criarVariaveisDecisao_Restricoes_ProcessoEstocasticoHidrologico(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodIni_stage, Periodo& a_period_stage, Periodo& a_periodEnd_stage, const SmartEnupla<Periodo, int>& a_horizonSP, const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>& a_horizon){
	
	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		const bool viabilidade_hidraulica = getAtributo(AttComumModeloOtimizacao_relaxar_afluencia_incremental_com_viabilidade_hidraulica, bool());

		const IdProcessoEstocastico idProcEstocastico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

		const TipoLagAutocorrelacao tipo_lag_autocorrelacao = getAtributo(idProcEstocastico, AttComumProcessoEstocastico_tipo_lag_autocorrelacao, TipoLagAutocorrelacao());

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= getMaiorId(idProcEstocastico, IdVariavelAleatoria()); idVariavelAleatoria++) {

			const std::vector<IdHidreletrica> lista_hidreletrica = getIdHidreletricaFromIdProcessoEstocasticoIdVariavelAleatoria(idProcEstocastico, idVariavelAleatoria);

			if (lista_hidreletrica.size() == 0)
				throw std::invalid_argument("Nao foram encontradas hidreletricas associada a :" + getFullString(idProcEstocastico) + " " + getFullString(idVariavelAleatoria));

			for (Periodo periodSP = a_horizonSP.getIteradorInicial(); periodSP <= a_horizonSP.getIteradorFinal(); a_horizonSP.incrementarIterador(periodSP)) {

				const TipoRelaxacaoVariavelAleatoria tipo_relaxacao = getElementoVetor(idProcEstocastico, idVariavelAleatoria, AttVetorVariavelAleatoria_tipo_relaxacao, periodSP, TipoRelaxacaoVariavelAleatoria());

				//
				//
				// Parcela referente ao processo estocastico
				//
				//

				// varRP
				const int varRP = addVarDecisao_RP(a_TSS, a_idEstagio, a_period_stage, periodSP, idProcEstocastico, idVariavelAleatoria, -infinito, infinito, 0.0);

				vetorEstagio.at(a_idEstagio).addVariavelRealizacao(a_TSS, getNomeVarDecisao_RP(a_TSS, a_idEstagio, a_period_stage, periodSP, idProcEstocastico, idVariavelAleatoria), varRP, idProcEstocastico, idVariavelAleatoria, periodSP, 1.0);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->addVarDinamica(varRP);

				// varYP
				const int varYP = addVarDecisao_YP(a_TSS, a_idEstagio, a_period_stage, periodSP, idProcEstocastico, idVariavelAleatoria, -infinito, infinito, 0.0);

				// Restricao YP (YP = RP + SOMA(fp*YPt-1))
				const int posEquYP = addEquLinear_YP(a_TSS, a_idEstagio, a_period_stage, periodSP, idProcEstocastico, idVariavelAleatoria);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYP, posEquYP, 1.0);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRP, posEquYP, -1.0);

				const double grau_liberdade_var_aleatoria = vetorProcessoEstocastico.at(idProcEstocastico).getGrauLiberdade(idVariavelAleatoria);

				// Variável do Processo Estocástico Hidrológico YP lag = 0 ... lag = np
				if (getSize1Matriz(idProcEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao) > 0) {

					for (int lag = 1; lag <= getSize2Matriz(idProcEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, periodSP); lag++) {

						if (getElementoMatriz(idProcEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, periodSP, lag, double()) != 0.0) {
				
							Periodo periodo_lag = periodSP - lag;

							int var_YP_LAG = criarVariaveisDecisao_VariaveisEstado_Restricoes_YP(a_TSS, a_dados, a_idEstagio, a_period_stage, idProcEstocastico, idVariavelAleatoria, periodo_lag, grau_liberdade_var_aleatoria);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(var_YP_LAG, posEquYP, -getElementoMatriz(idProcEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, periodSP, lag, double()));

						} // if (getElementoMatriz(idProcEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, periodSP, lag, double()) > 0.0){
					} // for (int lag = 1; lag <= getSize2Matriz(idProcEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao_sazonal, periodSP.getEstacao()); lag++) {
				} // if (getSize1Matriz(idProcEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao) > 0) {

				//
				//
				// Parcela referente às afluencias físicas
				//
				//

				bool sobreposicao_encontrada = false;
				for (Periodo period = a_periodIni_stage; period <= a_periodEnd_stage; a_horizon.incrementarIterador(period)) {

					const double sobreposicao = period.sobreposicao(periodSP);

					if (sobreposicao > 0.0) {

						for (int i = 0; i < int(lista_hidreletrica.size()); i++) {
							const IdHidreletrica idUHE = lista_hidreletrica.at(i);

							const IdVariavelAleatoriaInterna idVarInt = getIdVariavelAleatoriaInternaFromIdVariavelAleatoriaIdHidreletrica(idProcEstocastico, idVariavelAleatoria, idUHE);

							const double grau_liberdade = getAtributo(idProcEstocastico, idVariavelAleatoria, idVarInt, AttComumVariavelAleatoriaInterna_grau_liberdade, double());
							const double coeficiente_participacao = getElementoVetor(idProcEstocastico, idVariavelAleatoria, idVarInt, AttVetorVariavelAleatoriaInterna_coeficiente_participacao, periodSP, double());

							// Afluencia Incremental (YH): definida por periodo estudo
							const int varYH = addVarDecisao_YH(a_TSS, a_idEstagio, period, idUHE, -infinito, infinito, 0.0);

							// Restricao Afluencia Incremental: definida por periodo estudo
							const int posEquYH = addEquLinear_YH(a_TSS, a_idEstagio, period, idUHE);

							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquYH, -grau_liberdade * sobreposicao);

							// Variável YH
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYH, posEquYH, 1.0);

							// Variável YP
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYP, posEquYH, -coeficiente_participacao * sobreposicao);

							if (tipo_relaxacao != TipoRelaxacaoVariavelAleatoria_sem_relaxacao) {

								if (true) {
									// Variável Afluencia Incremental Folga (YHF): definida por periodo estudo (Quando penalizada ou utilizando viabilidade hidráulica)
									int varYHF = getVarDecisao_YHFseExistir(a_TSS, a_idEstagio, period, idUHE);

									if (varYHF == -1) {

										if (viabilidade_hidraulica) {

											if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica)
												varYHF = addVarDecisao_YHF(a_TSS, a_idEstagio, period, idUHE, 0.0, 0.0, 0.0);

											else if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
												varYHF = addVarDecisao_YHF(a_TSS, a_idEstagio, period, idUHE, 0.0, infinito, 100.0);

											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->addVarDinamica(varYHF);

											vetorEstagio.at(a_idEstagio).addVariavelRealizacaoInterna(a_TSS, getNomeVarDecisao_YHF(a_TSS, a_idEstagio, period, idUHE), varYHF, idProcEstocastico, idVariavelAleatoria, idVarInt, periodSP, sobreposicao, TipoValor_positivo, 1.0, 1.0);

										} // if (viabilidade_hidraulica) {

										else if (tipo_relaxacao == TipoRelaxacaoVariavelAleatoria_penalizacao)
											varYHF = addVarDecisao_YHF(a_TSS, a_idEstagio, period, idUHE, 0.0, infinito, 0.0);


									} // if (varYHF == -1) {

									// Variável Afluencia Incremental Folga (YHF): definida por periodo estudo
									if (varYHF > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYHF, posEquYH, -sobreposicao);

								} // if (true) {

								// Variável Afluencia Proc. Estocastico Folga (YPF): definida por periodo proc. estocastico (Quando truncada)
								int varYPF = getVarDecisao_YPFseExistir(a_TSS, a_idEstagio, a_period_stage, periodSP, idProcEstocastico, idVariavelAleatoria);

								if (varYPF == -1) {
									if ((tipo_relaxacao == TipoRelaxacaoVariavelAleatoria_truncamento) || (tipo_relaxacao == TipoRelaxacaoVariavelAleatoria_truncamento_penalizacao)) {
										varYPF = addVarDecisao_YPF(a_TSS, a_idEstagio, a_period_stage, periodSP, idProcEstocastico, idVariavelAleatoria, 0.0, 0.0, 0.0);
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->addVarDinamica(varYPF);
										vetorEstagio.at(a_idEstagio).addVariavelRealizacaoInterna(a_TSS, getNomeVarDecisao_YPF(a_TSS, a_idEstagio, a_period_stage, periodSP, idProcEstocastico, idVariavelAleatoria), varYPF, idProcEstocastico, idVariavelAleatoria, IdVariavelAleatoriaInterna_Nenhum, periodSP, sobreposicao, TipoValor_positivo, 1.0, 1.0);
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYPF, posEquYP, -1.0);
									}

								} // if (varYPF == -1) {

							} // if (tipo_relaxacao != TipoRelaxacaoVariavelAleatoria_sem_relaxacao) {
						} // for (int i = 0; i < int(lista_hidreletrica.size()); i++) {

						if (!sobreposicao_encontrada)
							sobreposicao_encontrada = true;
					}
					else if ((sobreposicao == 0.0) && (sobreposicao_encontrada))
						break;

				} // for (Periodo period = a_horizon.getIteradorInicial(); period <= a_horizon.getIteradorFinal(); a_horizon.incrementarIterador(period)) {

			} // for (Periodo periodSP = periodo_processo_estocastico_inicial; periodSP <= periodo_processo_estocastico_final; a_horizonte_processo_estocastico_estagio.incrementarIterador(periodSP)) {

		} // for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= getMaiorId(idProcEstocastico, IdVariavelAleatoria()); idVariavelAleatoria++) {

	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisDecisao_Restricoes_ProcessoEstocasticoHidrologico(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesCusto(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period) {

	try {

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		if (a_TSS != TipoSubproblemaSolver_mestre) {

			// ---------------------------------------------------------------------------------------------------------------------
			//                                  Restrição Custo Penalidade Valor Presente (ZP0p = desagio * ZPp) Por Periodo Estudo
			// ---------------------------------------------------------------------------------------------------------------------

			if (true) {

				const int posEquZP0p = addEquLinear_ZP0(a_TSS, a_idEstagio, a_period);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZP0p, 0.0);

				// Variável ZP0p
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZP0(a_TSS, a_idEstagio, a_period), posEquZP0p, 1.0);

				// Variável ZPp
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZP(a_TSS, a_idEstagio, a_period), posEquZP0p, -a_dados.getElementoMatriz(AttMatrizDados_desagio_acumulado_horizonte_estudo, a_idEstagio, a_period, double()));

			}

			// ---------------------------------------------------------------------------------------------------------------------
			//                                  Restrição Custo Operacao Valor Presente (ZO0p = desagio * ZOp) Por Periodo Estudo
			// ---------------------------------------------------------------------------------------------------------------------

			if (true) {

				const int posEquZO0p = addEquLinear_ZO0(a_TSS, a_idEstagio, a_period);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZO0p, 0.0);

				// Variável ZO0p
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZO0(a_TSS, a_idEstagio, a_period), posEquZO0p, 1.0);

				// Variável ZOp
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZO(a_TSS, a_idEstagio, a_period), posEquZO0p, -a_dados.getElementoMatriz(AttMatrizDados_desagio_acumulado_horizonte_estudo, a_idEstagio, a_period, double()));

			}


			// ---------------------------------------------------------------------------------------------------------------------
			//                                  Restrição Custo Total (ZT = ZI + ZF)
			// ---------------------------------------------------------------------------------------------------------------------

			try {

				int posEquZT = getEquLinear_ZTseExistir(a_TSS, a_idEstagio);

				if (posEquZT < 0) {

					posEquZT = addEquLinear_ZT(a_TSS, a_idEstagio);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZT, 0.0);

					// Variável ZT
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZT(a_TSS, a_idEstagio), posEquZT, 1.0);

					// Variável ZI
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZI(a_TSS, a_idEstagio), posEquZT, -1.0);

					// Variável ZF
					if (getVarDecisao_ZFseExistir(a_TSS, a_idEstagio) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZF(a_TSS, a_idEstagio), posEquZT, -1.0);

				} // if (posEquZT < 0) {

			} // try{
			catch (const std::exception& erro) { throw std::invalid_argument("Restricao Custo Total: \n" + std::string(erro.what())); }


			// ---------------------------------------------------------------------------------------------------------------------
			//                                  Restrição Custo Futuro (ZF = ZF_IdRealizacao_1 + .... + ZF_IdRealizacao_N)
			// ---------------------------------------------------------------------------------------------------------------------

			try {

				if (a_idEstagio < getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio())) {

					int posEquZF = getEquLinear_ZFseExistir(a_TSS, a_idEstagio);

					if (posEquZF < 0) {

						posEquZF = addEquLinear_ZF(a_TSS, a_idEstagio);

						const IdEstagio idEstagio_seguinte = IdEstagio(a_idEstagio + 1);

						const int multiplicidade_corte = a_dados.getElementoVetor(AttVetorDados_cortes_multiplos, idEstagio_seguinte, int());

						const Periodo periodo_otimizacao_seguinte = getAtributo(idEstagio_seguinte, AttComumEstagio_periodo_otimizacao, Periodo());

						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZF, 0.0);

						// Variável ZF
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZF(a_TSS, a_idEstagio), posEquZF, 1.0);

						for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getAtributo(idEstagio_seguinte, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); idRealizacao++) {

							if (multiplicidade_corte == 0) {//Single cut
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZF(a_TSS, a_idEstagio, idRealizacao), posEquZF, -1.0);
								break;
							}
							else if (multiplicidade_corte > 0) {//Multi cut

								const double probabilidade_abertura = getElementoMatriz(getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico()), AttMatrizProcessoEstocastico_probabilidade_realizacao, periodo_otimizacao_seguinte, idRealizacao, double());

								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZF(a_TSS, a_idEstagio, idRealizacao), posEquZF, -probabilidade_abertura);

							}//else if (multiplicidade_corte > 0) {

						}// for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getAtributo(idEstagio_seguinte, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); idRealizacao++) {

					} // if (posEquZF < 0) {

				} // if (a_idEstagio < getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio())) {

			} // try{
			catch (const std::exception& erro) { throw std::invalid_argument("Restricao Custo Futuro: \n" + std::string(erro.what())); }

			// ---------------------------------------------------------------------------------------------------------------------
			//                                  Restrição Custo Imediato (ZI = ZP0 + ZO0)
			// ---------------------------------------------------------------------------------------------------------------------

			try {

				int posEquZI = getEquLinear_ZIseExistir(a_TSS, a_idEstagio);
				
				if (posEquZI < 0) {

					posEquZI = addEquLinear_ZI(a_TSS, a_idEstagio);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZI, 0.0);

					// Variável ZI
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZI(a_TSS, a_idEstagio), posEquZI, 1.0);

					// Variável ZP0
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZP0(a_TSS, a_idEstagio), posEquZI, -1.0);

					// Variável ZO0
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZO0(a_TSS, a_idEstagio), posEquZI, -1.0);
				
				} // if (posEquZI < 0) {

			} // try{
			catch (const std::exception& erro) { throw std::invalid_argument("Restricao Custo Presente: \n" + std::string(erro.what())); }


			// ---------------------------------------------------------------------------------------------------------------------
			//                                  Restrição Custo Penalidade (ZP0 = ZP0p) Por Periodo Otimizacao
			// ---------------------------------------------------------------------------------------------------------------------

			try {

				int posEquZP0 = getEquLinear_ZP0seExistir(a_TSS, a_idEstagio);
				
				if (posEquZP0 < 0) {

					posEquZP0 = addEquLinear_ZP0(a_TSS, a_idEstagio);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZP0, 0.0);

					// Variável ZP0
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZP0(a_TSS, a_idEstagio), posEquZP0, 1.0);
				
				} // if (posEquZP0 < 0) {

				// Variável ZP0p
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZP0(a_TSS, a_idEstagio, a_period), posEquZP0, -double(double(a_period.getSegundos()) / 3600.0));

			} // try{
			catch (const std::exception& erro) { throw std::invalid_argument("Restricao ZP0: \n" + std::string(erro.what())); }


			// ---------------------------------------------------------------------------------------------------------------------
			//                                  Restrição Custo Operacao (ZO0 = soma(ZO0p)) Por Periodo Otimizacao
			// ---------------------------------------------------------------------------------------------------------------------

			try {

				int posEquZO0 = getEquLinear_ZO0seExistir(a_TSS, a_idEstagio);
				
				if (posEquZO0 < 0) {

					posEquZO0 = addEquLinear_ZO0(a_TSS, a_idEstagio);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZO0, 0.0);

					// Variável ZO0
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZO0(a_TSS, a_idEstagio), posEquZO0, 1.0);

				} // if (posEquZO0 < 0) {

				// Variável ZO0p
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZO0(a_TSS, a_idEstagio, a_period), posEquZO0, -double(double(a_period.getSegundos()) / 3600.0));

			} // try{
			catch (const std::exception& erro) { throw std::invalid_argument("Restricao ZO0: \n" + std::string(erro.what())); }

		} // if (a_TSS != TipoSubproblemaSolver_mestre) {

		else if (a_TSS == TipoSubproblemaSolver_mestre) {

			// ---------------------------------------------------------------------------------------------------------------------
			//                                  Restrição Custo Total no problema Mestre (ZT = ZT_IdRealizacao_1 + .... + ZT_IdRealizacao_N)
			// ---------------------------------------------------------------------------------------------------------------------

			try {

				int posEquZT = getEquLinear_ZT_MESTREseExistir(a_TSS, a_idEstagio);
				
				if (posEquZT < 0) {

					Periodo period_stage = getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

					const SmartEnupla<IdRealizacao, double> probabilidade_realizacao = getElementosMatriz(getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico()), AttMatrizProcessoEstocastico_probabilidade_realizacao, period_stage, IdRealizacao(), double());

					posEquZT = addEquLinear_ZT_MESTRE(a_TSS, a_idEstagio);

					const int multiplicidade_corte = a_dados.getElementoVetor(AttVetorDados_cortes_multiplos, a_idEstagio, int());

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZT, 0.0);

					// Variável ZT
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZT(a_TSS, a_idEstagio), posEquZT, 1.0);

					for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getAtributo(a_idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); idRealizacao++) {

						if (multiplicidade_corte == 0) {//Single cut{
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZT(a_TSS, a_idEstagio, idRealizacao), posEquZT, -1.0);
							break;
						}
						else if (multiplicidade_corte > 0) //Multi cut
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZT(a_TSS, a_idEstagio, idRealizacao), posEquZT, -probabilidade_realizacao.at(idRealizacao));

					}// for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getAtributo(idEstagio_seguinte, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); idRealizacao++) {

				} // if (posEquZT < 0) {

			} // try{
			catch (const std::exception& erro) { throw std::invalid_argument("Restricao Custo Total no problema Mestre: \n" + std::string(erro.what())); }

		} // else if (a_TSS == TipoSubproblemaSolver_mestre) {

	}// try {

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesCusto(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesCusto(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period, const IdPatamarCarga a_idPat) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		// ---------------------------------------------------------------------------------------------------------------------
		//                                  Restrição Custo Penalidades (ZPp = ZP_IdPatamarCarga1 + ... ZP_IdPatamarCargaN) Por Periodo Estudo
		// ---------------------------------------------------------------------------------------------------------------------

		if (true) {

			int posEquZPp = getEquLinear_ZPseExistir(a_TSS, a_idEstagio, a_period);
			
			if (posEquZPp < 0) {

				posEquZPp = addEquLinear_ZP(a_TSS, a_idEstagio, a_period);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZPp, 0.0);

				// Variável ZP Por PeriodoEstudo
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZP(a_TSS, a_idEstagio, a_period), posEquZPp, 1.0);

			} // if (posEquZPp < 0) {

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZP(a_TSS, a_idEstagio, a_period, a_idPat), posEquZPp, -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, a_idPat, double()));

		}

		// ---------------------------------------------------------------------------------------------------------------------
		//                                  Restrição Custo Operacao (ZOp = ZO_IdPatamarCarga1 + ... ZO_IdPatamarCargaN) Por Periodo Estudo
		// ---------------------------------------------------------------------------------------------------------------------

		if (true) {

			int posEquZOp = getEquLinear_ZOseExistir(a_TSS, a_idEstagio, a_period);
			
			if (posEquZOp < 0) {

				posEquZOp = addEquLinear_ZO(a_TSS, a_idEstagio, a_period);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZOp, 0.0);

				// Variável ZO Por PeriodoEstudo
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZO(a_TSS, a_idEstagio, a_period), posEquZOp, 1.0);

			} // if (posEquZOp < 0) {

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZO(a_TSS, a_idEstagio, a_period, a_idPat), posEquZOp, -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, a_idPat, double()));

		}

	}//try {

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesCustoOperacao_periodo(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }

}


void ModeloOtimizacao::criarRestricoesPotenciaHidraulicaDisponivel(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdPatamarCarga a_idPat, Periodo &a_period, const IdHidreletrica a_idHidreletrica) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const TipoDetalhamentoProducaoHidreletrica tipo_detalhamento_producao = a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica());

		//
		// Por Hidreletrica
		//

		// Restrição PH
		if ((getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1) && (tipo_detalhamento_producao != TipoDetalhamentoProducaoHidreletrica_por_usina)) {

			const int posEquPH = addEquLinear_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPH, 0.0);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquPH, 1.0);
			for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++)
				if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posEquPH, -1.0);

		} // if (getVarDecisao_PHseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1){


		// Restrição PH_REL
		if (getVarDecisao_PH_RELseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1) {

			const int posEquPH_REL = addEquLinear_PH_REL(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPH_REL, 0.0);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH_REL(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquPH_REL, 1.0);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquPH_REL, -1.0);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquPH_REL, -1.0);

		} // if (getVarDecisao_PH_RELseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1) {

		// Restrição PHDISP
		if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1) {

			const int posEquPHDISP = addEquLinear_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPHDISP, 0.0);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquPHDISP, 1.0);

			if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina) {

				//if (a_dados.getAtributo(AttComumDados_representar_producao_hidreletrica_com_turbinamento_disponivel, bool()))
					//vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquPHDISP, -1.0);
				//else	
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquPHDISP, -a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_disponibilidade, a_period, double()));

			}

			else {
				for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++)
					if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posEquPHDISP, -1.0);
			}

		} // if (getVarDecisao_PHDISPseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1) {

		// Restrição PHDISP_REL
		if (getVarDecisao_PHDISP_RELseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1) {

			const int posEquPHDISP_REL = addEquLinear_PHDISP_REL(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPHDISP_REL, 0.0);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP_REL(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquPHDISP_REL, 1.0);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquPHDISP_REL, -1.0);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquPHDISP_REL, -1.0);

		} // if (getVarDecisao_PHDISP_RELseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1) {

		//
		// Por Conjunto Hidraulico
		//

		for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

			// Restrição PH
			if ((getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico) > -1) && (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {

				const int posEquPH = addEquLinear_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPH, 0.0);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posEquPH, 1.0);

				for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++)
					if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquPH, -1.0);

			} // if (getVarDecisao_PHseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico) > -1){

			// Restrição PH_REL
			if (getVarDecisao_PH_RELseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico) > -1) {

				const int posEquPH_REL = addEquLinear_PH_REL(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPH_REL, 0.0);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH_REL(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posEquPH_REL, 1.0);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posEquPH_REL, -1.0);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posEquPH_REL, -1.0);

			} // if (getVarDecisao_PH_RELseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico) > -1) {

			// Restrição PHDISP
			if ((getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico) > -1) && (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto)) {

				const int posEquPHDISP = addEquLinear_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPHDISP, 0.0);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posEquPHDISP, 1.0);

				if (a_dados.getAtributo(AttComumDados_representar_producao_hidreletrica_com_turbinamento_disponivel, bool()))
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posEquPHDISP, -1.0);
				else
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posEquPHDISP, -a_dados.getElementoVetor(a_idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_disponibilidade, a_period, double()));

			} // if (getVarDecisao_PHDISPseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico) > -1) {

			else if ((getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico) > -1) && (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {

				const int posEquPHDISP = addEquLinear_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPHDISP, 0.0);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posEquPHDISP, 1.0);

				for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++)
					if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquPHDISP, -1.0);

			} // if (getVarDecisao_PHDISPseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico) > -1) {

			// Restrição PHDISP_REL
			if (getVarDecisao_PHDISP_RELseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico) > -1) {

				const int posEquPHDISP_REL = addEquLinear_PHDISP_REL(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPHDISP_REL, 0.0);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP_REL(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posEquPHDISP_REL, 1.0);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posEquPHDISP_REL, -1.0);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posEquPHDISP_REL, -1.0);

			} // if (getVarDecisao_PHDISP_RELseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico) > -1) {

		} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

		//
		// Producao Por Unidade UHE
		//

		for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {
			for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {

				// Restrição PH_REL
				if (getVarDecisao_PH_RELseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

					const int posEquPH_REL = addEquLinear_PH_REL(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPH_REL, 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH_REL(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquPH_REL, 1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquPH_REL, -1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquPH_REL, -1.0);

				} // if (getVarDecisao_PH_RELseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

				// Restrição PHDISP
				if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

					const int posEquPHDISP = addEquLinear_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPHDISP, 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquPHDISP, 1.0);

					if (a_dados.getAtributo(AttComumDados_representar_producao_hidreletrica_com_turbinamento_disponivel, bool()))
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquPHDISP, -1.0);
					else
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquPHDISP, -a_dados.getElementoVetor(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_disponibilidade, a_period, double()));

				} // if (getVarDecisao_PHDISPseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

				// Restrição PHDISP_REL
				if (getVarDecisao_PHDISP_RELseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

					const int posEquPHDISP_REL = addEquLinear_PHDISP_REL(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPHDISP_REL, 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP_REL(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquPHDISP_REL, 1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquPHDISP_REL, -1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquPHDISP_REL, -1.0);

				} // if (getVarDecisao_PHDISP_RELseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

			} // for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {
		} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {


	}//try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesProducaoHidreletrica_porProdutibilidade(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }

}


void ModeloOtimizacao::criarRestricoesTermeletrica(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdPatamarCarga a_idPat, Periodo& a_periodIni_stage, Periodo& a_periodEnd_stage, Periodo& a_periodPrev, Periodo &a_period, const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>& a_horizon) {
	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const IdTermeletrica idUTEIni = a_dados.getMenorId(IdTermeletrica());
		const IdTermeletrica idUTEOut = a_dados.getIdOut(IdTermeletrica());
		for (IdTermeletrica idUTE = idUTEIni; idUTE < idUTEOut; a_dados.vetorTermeletrica.incr(idUTE)) {

			const TipoDetalhamentoProducaoTermeletrica tipo_detalhamento_producao = a_dados.getElementoVetor(idUTE, AttVetorTermeletrica_tipo_detalhamento_producao, a_period, TipoDetalhamentoProducaoTermeletrica());


			//
			// Por Termeletrica
			//

			// RESTRIÇÃO PT
			if (getVarDecisao_PTUTILseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE) > -1) {

				//EQUAÇÃO  POTÊNCIA GERADA (PT - PTUTIL- POTENCIA_MINIMA*UTE_ON_T == 0) 
				const int posEquPTUTIL = addEquLinear_PT(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPTUTIL, 0.0);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), posEquPTUTIL, 1.0);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTUTIL(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), posEquPTUTIL, -1.0);

				if (getVarDecisao_UTE_ON_TseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_UTE_ON_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), posEquPTUTIL, -a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_minima, a_period, a_idPat, double()));


				if (a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_minima, a_period, a_idPat, double()) > 0.0) {
					const int posEquPT_REL = addEquLinear_PT_REL(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_REL(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), posEquPT_REL, 1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), posEquPT_REL, -1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), posEquPT_REL, -1.0);
				}


				else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {

					if (getVarDecisao_PTDISP_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE) > -1) {
						const int posEquPTDISP_FINF = addIneLinear_PTDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPTDISP_FINF, a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_minima, a_period, a_idPat, double()));
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), posEquPTDISP_FINF, 1.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), posEquPTDISP_FINF, 1.0);
					}

					if (getVarDecisao_PTDISP_FSUPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE) > -1) {
						const int posEquPTDISP_FSUP = addIneLinear_PTDISP_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPTDISP_FSUP, -a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_maxima, a_period, a_idPat, double()));
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), posEquPTDISP_FSUP, 1.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), posEquPTDISP_FSUP, -1.0);
					}
				}

			} // if (getVarDecisao_PT_RELseExistir(a_idEstagio, a_period, a_idPat, idUTE) > -1) {



			// Restrição PTDISP
			if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE) > -1) {

				if (true) {

					const int posEquPTDISP = addEquLinear_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPTDISP, 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), posEquPTDISP, 1.0);

					if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), posEquPTDISP, -a_dados.getElementoVetor(idUTE, AttVetorTermeletrica_disponibilidade, a_period, double()));

					else {
						for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= a_dados.getMaiorId(idUTE, IdUnidadeUTE()); idUnidadeUTE++)
							if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE) > -1)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posEquPTDISP, -1.0);
					}
				}

			} // if (getVarDecisao_PTDISPseExistir(a_idEstagio, a_period, a_idPat, idUTE) > -1) {



			// Restrição Comando GNL

			if (getVarDecisao_PTDISPCOMseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE) > -1) {

				const int equPTDISPCOM_pat = addEquLinear_PTDISPCOM_REL(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISPCOM(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), equPTDISPCOM_pat, 1.0);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISPCOM_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), equPTDISPCOM_pat, 1.0);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISPCOM_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), equPTDISPCOM_pat, -1.0);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), equPTDISPCOM_pat, -1.0);

			} // if (getVarDecisao_PTDISPCOMseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE) > -1) {


			for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= a_dados.getMaiorId(idUTE, IdUnidadeUTE()); idUnidadeUTE++) {

				// Restrição PT
				if (getVarDecisao_PTseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE) > -1) {


					//EQUAÇÃO  POTÊNCIA GERADA (PT - PTUTIL- POTENCIA_UTIL*UTE_ON_T == 0)
					const int posEquPTUTIL = addEquLinear_PT(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPTUTIL, 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posEquPTUTIL, 1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTUTIL(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posEquPTUTIL, -1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_UTE_ON_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posEquPTUTIL, -a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_minima, a_period, a_idPat, double()));


					// REPRESENTAÇÃO DISCRETA (COM VARIAVEL BINÁRIA)
					if (a_dados.getAtributo(idUTE, AttComumTermeletrica_representacao_discreta_producao, bool())) {


						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_TRAJ_ACIO(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posEquPTUTIL, -1.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_TRAJ_DESL(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posEquPTUTIL, -1.0);


						int tOn = a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_t_on, int()) - int((a_dados.getSizeVetor(idUTE, idUnidadeUTE, AttVetorUnidadeUTE_rampa_acionamento) + a_dados.getSizeVetor(idUTE, idUnidadeUTE, AttVetorUnidadeUTE_rampa_desligamento)) / 2);
						int tOf = a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_t_of, int()) + int((a_dados.getSizeVetor(idUTE, idUnidadeUTE, AttVetorUnidadeUTE_rampa_acionamento) + a_dados.getSizeVetor(idUTE, idUnidadeUTE, AttVetorUnidadeUTE_rampa_desligamento)) / 2);

						// SET VARIAVEIS COMO BINÁRIA
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_UTE_ON_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setTipoVariavelBinaria(getVarDecisao_UTE_ON_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE));
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setTipoVariavelBinaria(getVarDecisao_UTE_OFF_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE));
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setTipoVariavelBinaria(getVarDecisao_ACIO_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE));


						// RELAÇÃO ENTRE AS VARIAVEIS BINÁRIAS 
						const int posEquVAR_BINARIAS = addEquLinear_UTE_BIN(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquVAR_BINARIAS, 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ACIO_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posEquVAR_BINARIAS, 1.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_UTE_OFF_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posEquVAR_BINARIAS, -1.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_UTE_ON_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posEquVAR_BINARIAS, -1.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_UTE_ON_T(a_TSS, a_idEstagio, a_periodPrev, a_idPat, idUTE, idUnidadeUTE), posEquVAR_BINARIAS, 1.0);


						// CONDIÇÕES INICIAIS
						//*************************************************************************************************************************************************************
						if (a_period == (a_periodIni_stage)) {

							// SET VARIAVEIS BINÁRIAS
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setTipoVariavelBinaria(getVarDecisao_UTE_ON_T(a_TSS, a_idEstagio, a_periodPrev, a_idPat, idUTE, idUnidadeUTE));
							//vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setTipoVariavelBinaria(getVarDecisao_ACIO_T(a_idEstagio, a_periodPrev, a_idPat, idUTE, idUnidadeUTE));

							// ESTADO DA UNIDADE NO PERIODO ANTERIOR AO ESTUDO (p0 -1)
							if (a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_status, TipoStatusUnidadeUTE()) == TipoStatusUnidadeUTE_ligada) {
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_UTE_ON_T(a_TSS, a_idEstagio, a_periodPrev, a_idPat, idUTE, idUnidadeUTE), 1.0);
								//vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PTUTIL(a_idEstagio, a_periodPrev, a_idPat, idUTE, idUnidadeUTE), a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_geracao_inicial, double()) - a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_minima, a_period, a_idPat, double()));
								//vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PTUTIL(a_idEstagio, a_periodPrev, a_idPat, idUTE, idUnidadeUTE), a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_geracao_inicial, double()) - a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_minima, a_period, a_idPat, double()));
							}

							else if (a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_status, TipoStatusUnidadeUTE()) == TipoStatusUnidadeUTE_desligada) {
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_UTE_ON_T(a_TSS, a_idEstagio, a_periodPrev, a_idPat, idUTE, idUnidadeUTE), 0.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_UTE_ON_T(a_TSS, a_idEstagio, a_periodPrev, a_idPat, idUTE, idUnidadeUTE), 0.0);
								//vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PTUTIL(a_idEstagio, a_periodPrev, a_idPat, idUTE, idUnidadeUTE), 0.0);
								//vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PTUTIL(a_idEstagio, a_periodPrev, a_idPat, idUTE, idUnidadeUTE), 0.0);
							}

							//	// VERIFICA SE A UNIDADE FOI ACIONADA NO PERIODO ANTERIOR AO ESTUDO (p0 - 1)
							//	if (((a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_horas_neste_status, int()) == 1) || ((a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_horas_neste_status, int()) == 1) && (a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_min_neste_status, int()) == 30))) && (a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_status, TipoStatusUnidadeUTE()) == TipoStatusUnidadeUTE_ligada))
							//		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_ACIO_T(a_idEstagio, a_periodPrev, a_idPat, idUTE, idUnidadeUTE), 1.0);

							//	else
							//		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_ACIO_T(a_idEstagio, a_periodPrev, a_idPat, idUTE, idUnidadeUTE), 0.0);

						}// if (a_period == (a_periodIni_stage)) {


						//INEQUAÇÃO QUE ASSEGURA GERAÇÃO MINIMA QUANDO A UNIDADE É LIGADA  (PTUTIL - POTENCIA_UTIL*UTE_ON_T + POTENCIA_UTIL*ACIO_T <= 0) 
						//*************************************************************************************************************************************************************
						const int posInePT_PARTIDA = addIneLinear_PT_ON(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posInePT_PARTIDA, 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTUTIL(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posInePT_PARTIDA, -1.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_UTE_ON_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posInePT_PARTIDA, +a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_util, a_period, a_idPat, double()));

						//
						////INEQUAÇÃO QUE ASSEGURA GERAÇÃO MINIMA QUANDO A UNIDADE É LIGADA  (PTUTIL - POTENCIA_UTIL*UTE_ON_T + POTENCIA_UTIL*ACIO_T <= 0) 
						////*************************************************************************************************************************************************************
						//const int posInePT_PARTIDA = addIneLinear_PT_ON(a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE);
						//vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posInePT_PARTIDA, 0.0);
						//vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTUTIL(a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posInePT_PARTIDA, - 1.0);
						//vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_UTE_ON_T(  a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posInePT_PARTIDA, + a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_util, a_period, a_idPat, double()));
						//vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ACIO_T( a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posInePT_PARTIDA, - a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_util, a_period, a_idPat, double()));


						////INEQUAÇÃO QUE ASSEGURA GERAÇÃO MINIMA QUANDO A UNIDADE É DESLIGADA  
						////*************************************************************************************************************************************************************
						//if (tOn <= 1) {	// PARA UNIDADES COM TLIG <= 1 (PTUTIL(p-1) - POTENCIA_UTIL*UTE_ON_T(p-1) + POTENCIA_UTIL*UTE_OFF_T <= 0)				 				

						//	const int posInePT_DESLIGAMENTO = addIneLinear_PT_OFF(a_idEstagio, a_period , a_idPat, idUTE, idUnidadeUTE);
						//	vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posInePT_DESLIGAMENTO, 0.0);
						//	vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTUTIL(a_idEstagio, a_periodPrev, a_idPat, idUTE, idUnidadeUTE), posInePT_DESLIGAMENTO, - 1.0);
						//	vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_UTE_ON_T(  a_idEstagio, a_periodPrev, a_idPat, idUTE, idUnidadeUTE), posInePT_DESLIGAMENTO,  a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_util, a_period, a_idPat, double()));
						//	vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_UTE_OFF_T( a_idEstagio,        a_period, a_idPat, idUTE, idUnidadeUTE), posInePT_DESLIGAMENTO, - a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_util, a_period, a_idPat, double()));
						//												
						//} //if (tOn <= 1) {					
						//
						//else if (tOn > 1) {	 // PARA UNIDADES COM TLIG > 1 (PTUTIL(p-1) - POTENCIA_UTIL*UTE_ON_T(p-1)  + POTENCIA_UTIL*ACIO_T(p-1) + POTENCIA_UTIL*UTE_OFF_T <= 0)
						//	
						//		const int posInePT_DESLIGAMENTO = addIneLinear_PT_OFF(a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE);
						//		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posInePT_DESLIGAMENTO, 0.0);
						//		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTUTIL(a_idEstagio, a_periodPrev, a_idPat, idUTE, idUnidadeUTE), posInePT_DESLIGAMENTO, - 1.0);
						//		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_UTE_ON_T(  a_idEstagio, a_periodPrev, a_idPat, idUTE, idUnidadeUTE), posInePT_DESLIGAMENTO, a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_util, a_period, a_idPat, double()));
						//		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ACIO_T( a_idEstagio, a_periodPrev, a_idPat, idUTE, idUnidadeUTE), posInePT_DESLIGAMENTO, - a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_util, a_period, a_idPat, double()));
						//		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_UTE_OFF_T( a_idEstagio,        a_period, a_idPat, idUTE, idUnidadeUTE), posInePT_DESLIGAMENTO, - a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_util, a_period, a_idPat, double()));

						//} //else if (tOn > 1) {


						// RESTRIÇÃO DE MINIMUN UP/DOWNTIME
						//****************************************************************************************************************************************************************
						if (a_period >= a_periodIni_stage + IdHor(tOn + 1)) {

							// UPTIME
							const int posIneUP_TIME = addIneLinear_UTE_ON_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneUP_TIME, 0.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_UTE_ON_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posIneUP_TIME, 1.0);

							// DOWNTIME
							const int posIneDOWN_TIME = addIneLinear_UTE_OFF_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneDOWN_TIME, -1);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_UTE_ON_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posIneDOWN_TIME, -1.0);


							Periodo periodo_inicial = a_periodIni_stage;

							for (Periodo periodo_aux = a_periodIni_stage; periodo_aux <= a_periodEnd_stage; a_horizon.incrementarIterador(periodo_aux))
								if (periodo_aux <= (a_period - IdHor(tOn + 1)))
									periodo_inicial = periodo_aux;

							for (Periodo periodo = periodo_inicial; periodo <= a_period; a_horizon.incrementarIterador(periodo)) {
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ACIO_T(a_TSS, a_idEstagio, periodo, a_idPat, idUTE, idUnidadeUTE), posIneUP_TIME, -1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_UTE_OFF_T(a_TSS, a_idEstagio, periodo, a_idPat, idUTE, idUnidadeUTE), posIneDOWN_TIME, -1.0);
							}

						}//if (a_period >= a_period + IdHor(tOn+1)) {


						else if (a_period < a_periodIni_stage + IdHor(tOn + 1)) {

							// UPTIME
							const int posIneUP_TIME = addIneLinear_UTE_ON_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneUP_TIME, 0.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_UTE_ON_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posIneUP_TIME, 1.0);


							// DOWNTIME
							const int posIneDOWN_TIME = addIneLinear_UTE_OFF_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneDOWN_TIME, -1);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_UTE_ON_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posIneDOWN_TIME, -1.0);

							for (Periodo periodo = a_periodIni_stage; periodo <= a_period; a_horizon.incrementarIterador(periodo)) {
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ACIO_T(a_TSS, a_idEstagio, periodo, a_idPat, idUTE, idUnidadeUTE), posIneUP_TIME, -1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_UTE_OFF_T(a_TSS, a_idEstagio, periodo, a_idPat, idUTE, idUnidadeUTE), posIneDOWN_TIME, -1.0);
							}

						}// else if (a_period >= a_period + IdHor(tOn+1)) {



							// CONDIÇÕES INICIAIS (SET NOS BOUNDS DAS VARIAVEIS BINÁRIAS)
							//*********************************************************************************************************************************************************
						if (a_period == a_periodEnd_stage) {

							// PARA UNIDADES LIGADAS
							if (a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_status, TipoStatusUnidadeUTE()) == TipoStatusUnidadeUTE_ligada) {

								if (a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_horas_neste_status, int()) <= tOn) {

									Periodo periodo_final = a_periodIni_stage + IdHor(tOn) - IdHor(a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_horas_neste_status, int())) - IdMin(a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_min_neste_status, int()));

									if (periodo_final <= a_periodEnd_stage)
										for (Periodo periodo = a_periodIni_stage; periodo <= periodo_final; a_horizon.incrementarIterador(periodo))
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_UTE_ON_T(a_TSS, a_idEstagio, periodo, a_idPat, idUTE, idUnidadeUTE), 1.0);

									else if (periodo_final > a_periodEnd_stage)
										for (Periodo periodo = a_periodIni_stage; periodo <= a_periodEnd_stage; a_horizon.incrementarIterador(periodo))
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_UTE_ON_T(a_TSS, a_idEstagio, periodo, a_idPat, idUTE, idUnidadeUTE), 1.0);

								}//if (a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_horas_neste_status, int()) <= tOn) {
							}//if (a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_status, TipoStatusUnidadeUTE()) == TipoStatusUnidadeUTE_ligada) {


							// PARA UNIDADES DESLIGADAS
							else if (a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_status, TipoStatusUnidadeUTE()) == TipoStatusUnidadeUTE_desligada) {

								if (a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_horas_neste_status, int()) <= tOf) {

									Periodo periodo_final = a_periodIni_stage + IdHor(tOf) - IdHor(a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_horas_neste_status, int())) - IdMin(a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_min_neste_status, int()));

									if (periodo_final <= a_periodEnd_stage)
										for (Periodo periodo = a_periodIni_stage; periodo <= periodo_final; a_horizon.incrementarIterador(periodo))
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_UTE_OFF_T(a_TSS, a_idEstagio, periodo, a_idPat, idUTE, idUnidadeUTE), 0.0);

									else if (periodo_final > a_periodEnd_stage)
										for (Periodo periodo = a_periodIni_stage; periodo <= a_periodEnd_stage; a_horizon.incrementarIterador(periodo))
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_UTE_OFF_T(a_TSS, a_idEstagio, periodo, a_idPat, idUTE, idUnidadeUTE), 0.0);


								}//if (a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_horas_neste_status, int()) <= tOf) {								

							}//else if (a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_status, TipoStatusUnidadeUTE()) == TipoStatusUnidadeUTE_desligada) {

						}// if (a_period == a_horizon.getIteradorFicial()) {									   						 


						// RESTRIÇÃO PARA TRAJETORIA DE ACIONAMENTO
						//*************************************************************************************************************************************************************

						int numero_trajetorias = (a_dados.getSizeVetor(idUTE, idUnidadeUTE, AttVetorUnidadeUTE_rampa_acionamento));

						if (numero_trajetorias == 0)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PT_TRAJ_ACIO(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), 0.0);

						else {

							Periodo periodo = a_period;
							Periodo periodoTipoMeiaHora = Periodo(TipoPeriodo_meia_hora, a_period.getDia(), a_period.getMes(), a_period.getAno(), a_period.getHora(), a_period.getMinuto());
							Periodo periodoAcio_aux = periodoTipoMeiaHora + numero_trajetorias;
							Periodo periodoAcio;

							for (Periodo periodoAux = a_periodIni_stage; periodoAux <= a_periodEnd_stage; a_horizon.incrementarIterador(periodoAux)) {
								if (periodoAux > periodoAcio_aux) { break; }
								if (periodoAux <= periodoAcio_aux) { periodoAcio = periodoAux; }
								if ((periodoAux < periodoAcio_aux) && (a_period == a_periodIni_stage)) {
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_ACIO_T(a_TSS, a_idEstagio, periodoAux, a_idPat, idUTE, idUnidadeUTE), 0.0);
								}
							}

							//RESTRIÇÃO QUE OBRIGA PT_TRAJ_DESL SER IGUAL A 0 QUANDO NÃO ESTA EM TRAJETÓRIA DE ACIONAMENTO
							// (- PT_TRAJ_ACIO - ACIO_T(p-nTrajetorias; p) * PtMax >=0 )
							const int posInePT_ACIO_SOMA = addIneLinear_UTE_ON_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE, a_period);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posInePT_ACIO_SOMA, 0.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_TRAJ_ACIO(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posInePT_ACIO_SOMA, -1);

							for (int trajetoria = 0; trajetoria < numero_trajetorias; trajetoria++) {

								Periodo periodoTrajetoria = periodoTipoMeiaHora + trajetoria;

								if (periodoTrajetoria >= periodo) {

									if (periodo <= a_periodEnd_stage) {

										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ACIO_T(a_TSS, a_idEstagio, periodo, a_idPat, idUTE, idUnidadeUTE), posInePT_ACIO_SOMA, a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima, periodo, a_idPat, double()));

										// LIMITE INFERIOR (PT_TRAJ_LIG - TRAJ_UTE_ON_T * rampa >=0 )
										const int posInePT_ACIO = addIneLinear_UTE_ON_T_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE, periodo);
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posInePT_ACIO, 0.0);
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_TRAJ_ACIO(a_TSS, a_idEstagio, periodo, a_idPat, idUTE, idUnidadeUTE), posInePT_ACIO, 1);
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ACIO_T(a_TSS, a_idEstagio, periodoAcio, a_idPat, idUTE, idUnidadeUTE), posInePT_ACIO, -a_dados.getElementoVetor(idUTE, idUnidadeUTE, AttVetorUnidadeUTE_rampa_acionamento, trajetoria, double()));

										// LIMITE SUPERIOR (-PT_TRAJ_LIG + TRAJ_UTE_ON_T * rampa + TRAJ_UTE_ON_T * PT_MIN >= PT_MIN )
										const int posInePT_ACIO_SUP = addIneLinear_UTE_ON_T_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE, periodo);
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posInePT_ACIO_SUP, -a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima, periodo, a_idPat, double()));
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_TRAJ_ACIO(a_TSS, a_idEstagio, periodo, a_idPat, idUTE, idUnidadeUTE), posInePT_ACIO_SUP, -1);
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ACIO_T(a_TSS, a_idEstagio, periodoAcio, a_idPat, idUTE, idUnidadeUTE), posInePT_ACIO_SUP, -(a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima, periodo, a_idPat, double()) - a_dados.getElementoVetor(idUTE, idUnidadeUTE, AttVetorUnidadeUTE_rampa_acionamento, trajetoria, double())));
										a_horizon.incrementarIterador(periodo);

									}//if (periodo <= a_periodEnd_stage) {
								}
							}//for (int trajetoria = 0; trajetoria < numero_trajetorias; trajetoria++) {

							if (periodo <= a_periodEnd_stage)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ACIO_T(a_TSS, a_idEstagio, periodo, a_idPat, idUTE, idUnidadeUTE), posInePT_ACIO_SOMA, a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima, periodo, a_idPat, double()));

						}// else


						// RESTRIÇÃO PARA TRAJETORIA DE DESLIGAMENTO
						//*************************************************************************************************************************************************************
						numero_trajetorias = (a_dados.getSizeVetor(idUTE, idUnidadeUTE, AttVetorUnidadeUTE_rampa_desligamento));
						if (numero_trajetorias == 0)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PT_TRAJ_DESL(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), 0.0);

						else {

							//RESTRIÇÃO QUE OBRIGA PT_TRAJ_DESL SER IGUAL A 0 QUANDO NÃO ESTA EM TRAJETÓRIA DE DESLIGAMANTO
							// (- PT_TRAJ_DESL - UTE_OFF_T(p-nTrajetorias; p) * PtMax >=0 )
							const int posInePT_DESL_SOMA = addIneLinear_UTE_OFF_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE, a_period, 1);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posInePT_DESL_SOMA, 0.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_TRAJ_DESL(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posInePT_DESL_SOMA, -1);

							Periodo periodo = a_period;
							Periodo periodoTipoMeiaHora = Periodo(TipoPeriodo_meia_hora, a_period.getDia(), a_period.getMes(), a_period.getAno(), a_period.getHora(), a_period.getMinuto());

							for (int trajetoria = 0; trajetoria < numero_trajetorias; trajetoria++) {

								Periodo periodoTrajetoria = periodoTipoMeiaHora + trajetoria;
								Periodo periodoPT = periodoTipoMeiaHora + (trajetoria - numero_trajetorias + 1);
								Periodo periodoPt = periodoPT;

								for (Periodo periodoAux = a_periodIni_stage; periodoAux <= a_periodEnd_stage; a_horizon.incrementarIterador(periodoAux)) {
									if (periodoAux > periodoPT) { break; }
									if (periodoAux <= periodoPT) { periodoPt = periodoAux; }
								}


								//RESTRIÇÃO QUE OBRIGA PT_TRAJ_DESL SER IGUAL A 0 QUANDO NÃO ESTA EM TRAJETÓRIA DE DESLIGAMANTO
								if (periodoPt >= a_periodIni_stage)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_UTE_OFF_T(a_TSS, a_idEstagio, periodoPt, a_idPat, idUTE, idUnidadeUTE), posInePT_DESL_SOMA, a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima, periodoPt, a_idPat, double()));

								if (periodoTrajetoria >= periodo) {

									if (periodo <= a_periodEnd_stage) {

										// LIMITE INFERIOR (PT_TRAJ_DESL(1;nt) - UTE_OFF_T * rampa >=0 )
										const int posInePT_DESL_INF = addIneLinear_UTE_OFF_T_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE, periodo, trajetoria + 1);
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posInePT_DESL_INF, 0.0);
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_TRAJ_DESL(a_TSS, a_idEstagio, periodo, a_idPat, idUTE, idUnidadeUTE), posInePT_DESL_INF, 1);
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_UTE_OFF_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posInePT_DESL_INF, -a_dados.getElementoVetor(idUTE, idUnidadeUTE, AttVetorUnidadeUTE_rampa_desligamento, trajetoria, double()));

										// LIMITE SUPERIOR (-PT_TRAJ_DESL + UTE_OFF_T * (rampa -  PT_MAX) - PT_MAX >= 0 )
										const int posInePT_DESL_SUP = addIneLinear_UTE_OFF_T_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE, periodo, trajetoria + 1);
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posInePT_DESL_SUP, -a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima, periodo, a_idPat, double()));
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_TRAJ_DESL(a_TSS, a_idEstagio, periodo, a_idPat, idUTE, idUnidadeUTE), posInePT_DESL_SUP, -1);
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_UTE_OFF_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posInePT_DESL_SUP, (a_dados.getElementoVetor(idUTE, idUnidadeUTE, AttVetorUnidadeUTE_rampa_desligamento, trajetoria, double()) - a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima, periodo, a_idPat, double())));

										a_horizon.incrementarIterador(periodo);
									}
								}

							}//for (int trajetoria = 0; trajetoria < numero_trajetorias; trajetoria++) {

						} //else {






						//// CONDIÇÕES INICIAIS TRAJETÓRIA DE ACIONAMENTO 
						//if (a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_status, TipoStatusUnidadeUTE()) == TipoStatusUnidadeUTE_desligada) {

						//	int numero_trajetorias = (a_dados.getSizeVetor(idUTE, idUnidadeUTE, AttVetorUnidadeUTE_rampa_acionamento));

						//	if (a_period < a_periodIni_stage + numero_trajetorias)
						//		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_ACIO_T(a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), 0.0);
						//}


						//// CONDIÇÕES INICIAIS TRAJETÓRIA DE DESLIGAMENTO 
						//if (a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_status, TipoStatusUnidadeUTE()) == TipoStatusUnidadeUTE_ligada) {

						//	int numero_trajetorias = (a_dados.getSizeVetor(idUTE, idUnidadeUTE, AttVetorUnidadeUTE_rampa_desligamento));

						//	if (a_period < a_periodIni_stage + numero_trajetorias)
						//		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_UTE_OFF_T(a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), 0.0);
						//}



					}// if (a_dados.getAtributo(idUTE, AttComumTermeletrica_representacao_discreta_producao, bool())) {

				} // if (getVarDecisao_PTseExistir(a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE) > -1) {


				//			   				 
				// Restrição PTDISP
				if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE) > -1) {

					const int posEquPTDISP = addEquLinear_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPTDISP, 0.0);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posEquPTDISP, 1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posEquPTDISP, -a_dados.getElementoVetor(idUTE, idUnidadeUTE, AttVetorUnidadeUTE_disponibilidade, a_period, double()));

				} // if (getVarDecisao_PTDISPseExistir(a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE) > -1) {

				//// Restrição PTDISP_REL
				//if (getVarDecisao_PTDISP_RELseExistir(a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE) > -1) {

				//	const int posEquPTDISP_REL = addEquLinear_PTDISP_REL(a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE);

				//	vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPTDISP_REL, 0.0);

				//	vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP_REL(a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posEquPTDISP_REL, 1.0);
				//	vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posEquPTDISP_REL, -1.0);
				//	vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP_FINF(a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posEquPTDISP_REL, -1.0);

				//} // if (getVarDecisao_PTDISP_RELseExistir(a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE) > -1) {

			} // for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= a_dados.getMaiorId(idUTE, IdUnidadeUTE()); idUnidadeUTE++) {



			// RESTRIÇÃO PARA UTEs A CICLO COMBINADO QUE SÃO MODELADAS COMO UNIDADES EQUIVALENTES, ONDE, APENAS UMA UNIDADE PODE ESTAR EM OPERAÇÃO.
			int posIneUN_EQUIVALENTE = -1;

			for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= a_dados.getMaiorId(idUTE, IdUnidadeUTE()); idUnidadeUTE++) {

				if (a_dados.getAtributo(idUTE, IdUnidadeUTE_1, AttComumUnidadeUTE_unidade_equivalente, bool())) {

					if (idUnidadeUTE == IdUnidadeUTE_1) {
						posIneUN_EQUIVALENTE = addIneLinear_UTE_EQV(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneUN_EQUIVALENTE, -1);
					}

					if (getVarDecisao_UTE_ON_TseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_UTE_ON_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posIneUN_EQUIVALENTE, -1);
				}


			}//if (a_dados.getAtributo(idUTE, IdUnidadeUTE_1, AttComumUnidadeUTE_unidade_equivalente, bool())) {


		} // for (IdTermeletrica idUTE = a_dados.getMenorId(IdTermeletrica()); idUTE <= a_maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idUTE)) {

	}//try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesTermeletrica(" + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }

}


void ModeloOtimizacao::criarVariaveisCusto(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio) {

	try {

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		if (a_TSS != TipoSubproblemaSolver_mestre) {

			//
			// Custo Total (ZT)
			//

			addVarDecisao_ZT(a_TSS, a_idEstagio, 0.0, infinito, 1.0);

			//
			// Custo Imediato (ZI)
			//

			addVarDecisao_ZI(a_TSS, a_idEstagio, 0.0, infinito, 0.0);

			//
			// Custo Penalidade Valor Presente (ZP0)
			//

			addVarDecisao_ZP0(a_TSS, a_idEstagio, 0.0, infinito, 0.0);

			//
			// Custo Operacao  Valor Presente (ZO0)
			//

			addVarDecisao_ZO0(a_TSS, a_idEstagio, 0.0, infinito, 0.0);

			//
			// Custo Futuro (ZF)
			//

			if (a_idEstagio < getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio())) {

				const IdEstagio idEstagio_seguinte = IdEstagio(a_idEstagio + 1);

				const int multiplicidade_corte = a_dados.getElementoVetor(AttVetorDados_cortes_multiplos, idEstagio_seguinte, int());

				addVarDecisao_ZF(a_TSS, a_idEstagio, 0.0, infinito, 0.0);

				for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getAtributo(idEstagio_seguinte, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); idRealizacao++) {
					addVarDecisao_ZF(a_TSS, a_idEstagio, idRealizacao, 0.0, infinito, 0.0);
					if (multiplicidade_corte == 0)
						break;
				} // for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getAtributo(idEstagio_seguinte, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); idRealizacao++) {

			} // if (a_idEstagio < getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio())) {

		} // if (a_TSS != TipoSubproblemaSolver_mestre) {

		else if (a_TSS == TipoSubproblemaSolver_mestre) {

			//
			// Custo Total (ZT)
			//

			const int multiplicidade_corte = a_dados.getElementoVetor(AttVetorDados_cortes_multiplos, a_idEstagio, int());

			addVarDecisao_ZT(a_TSS, a_idEstagio, 0.0, infinito, 0.0);

			for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getAtributo(a_idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); idRealizacao++) {
				addVarDecisao_ZT(a_TSS, a_idEstagio, idRealizacao, 0.0, infinito, 0.0);
				if (multiplicidade_corte == 0)
					break;
			} // for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getAtributo(a_idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); idRealizacao++) {

		} // else if (a_TSS == TipoSubproblemaSolver_mestre) {

	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisCusto(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
}



void ModeloOtimizacao::criarRestricoesHidraulicas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodIni_stage, Periodo& a_periodEnd_stage, Periodo& a_periodPrev, Periodo& a_period, Periodo& a_periodNext, const IdPatamarCarga a_idPat) {

	try {

		const IdHidreletrica idUHEIni = a_dados.getMenorId(IdHidreletrica());
		const IdHidreletrica idUHEOut = a_dados.getIdOut(IdHidreletrica());

		for (IdHidreletrica idUHE = idUHEIni; idUHE < idUHEOut; a_dados.vetorHidreletrica.incr(idUHE)) {

			// RESTRIÇÃO DE BALANÇO HIDRAULICO 
			if ((a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, a_period, double()) > 0.0) || (a_dados.getAtributo(AttComumDados_representar_todos_balancos_hidricos_por_volume, bool())) || \
				(getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1) || (getVarDecisao_VFseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1)) {

				// Restrições Balanço Hidraulico Por Volume (BH_VOL)
				criarRestricoesBalancoHidraulicoUsinaByVolume(a_TSS, a_dados, a_idEstagio, a_periodIni_stage, a_periodEnd_stage, a_periodPrev, a_period, a_periodNext, idUHE, a_idPat);
			}
			else {
				// Restrições Balanço Hidraulico Por Vazão (BH_VAZ)
				criarRestricoesBalancoHidraulicoUsinaByVazao(a_TSS, a_dados, a_idEstagio, a_periodIni_stage, a_periodEnd_stage, a_periodPrev, a_period, a_periodNext, idUHE, a_idPat);
			}


			// Restriçao Volume de espera
			if (a_idPat == IdPatamarCarga_1) {

				// CRIA RESTRIÇÕES DE VOLUME UTIL
				criarRestricoesVolumeUtil_e_Penalidade(a_TSS, a_dados, a_idEstagio, a_period, idUHE);

			}//if (idPat == IdPatamarCarga_1) {

			// Restriçao Potência Hidraulica Disponível
			criarRestricoesPotenciaHidraulicaDisponivel(a_TSS, a_dados, a_idEstagio, a_idPat, a_period, idUHE);

			//FPH
			criarRestricoesFuncaoProducaoHidreletrica(a_TSS, a_dados, a_idEstagio, idUHE, a_idPat, a_period, a_periodNext, a_periodEnd_stage);

			// CRIA RESTRIÇÕES DE VAZÃO DEFLUENTE 
			criarRestricoesVazaoDefluente(a_TSS, a_dados, a_idEstagio, a_period, idUHE, a_idPat);

			// CRIA RESTRIÇÕES DE VAZÃO DESVIADA
			criarRestricoesVazaoDesviada(a_TSS, a_dados, a_idEstagio, a_period, idUHE, a_idPat);

			// CRIA RESTRIÇÕES DE VAZÃO RETIRADA
			criarRestricoesVazaoRetirada(a_TSS, a_dados, a_idEstagio, a_period, idUHE, a_idPat);

			// CRIA RESTRIÇÕES DE TURBINAMENTO HIDRAULICO
			criarRestricoesTurbinamentoHidraulico(a_TSS, a_dados, a_idEstagio, a_period, idUHE, a_idPat);


		} // for (IdHidreletrica idUHE = idUHEIni; idUHE < idUHEOut; a_dados.vetorHidreletrica.incr(idUHE)) {

	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesHidraulicas(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }


}


void ModeloOtimizacao::criarRestricoesBalancoHidraulicoUsinaByVazao(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodIni_stage, Periodo& a_periodEnd_stage, Periodo& a_periodPrev, Periodo & a_period, Periodo& a_periodNext, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPat) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		// --------------------
		//
		// Vazao Montante
		// 
		// --------------------

		try {

			const int posEquQMT = addEquLinear_QM(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQMT, 0.0);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QM(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquQMT, 1.0);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_YH(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posEquQMT, -1.0);

		}// try

		catch (const std::exception& erro) { throw std::invalid_argument("Erro Restricao Vazao Montante: \n" + std::string(erro.what())); }

		// --------------------------
		//
		// Vazao Defluente
		// 
		// --------------------------

		try {

			const int posEquQD = addEquLinear_QD(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQD, 0.0);

			//QD
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquQD, 1.0);

			//QTDISP
			if (getVarDecisao_QTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquQD, -1.0);

			//QV
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QV(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquQD, -1.0);


		}// try

		catch (const std::exception& erro) { throw std::invalid_argument("Erro Restricao Vazao Defluente Disponivel: \n" + std::string(erro.what())); }


		// -------------------------------
		//
		// Balanço Hídrico no Reservatório
		// 
		// -------------------------------

		const double conversor_vazao_volume = a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, a_period, a_idPat, double());

		const int posEquBH = addEquLinear_BH_VAZ(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);

		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquBH, 0.0);

		// ENCHENDO VOLUME MORTO 
		if (a_period > a_periodIni_stage) {

			if (a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_enchendo_volume_morto, a_periodPrev, int()) == 1) {
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquBH, a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo, a_periodPrev, double()) / conversor_vazao_volume);
				const int posIneVM = addIneLinear_VMORTO_ENCH(a_TSS, a_idEstagio, a_periodPrev, a_idPat, a_idHidreletrica);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneVM, a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_meta_enchimento_volume_morto, a_periodPrev, double()));
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posEquBH, 1.0);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VMORTO_FINF(a_TSS, a_idEstagio, a_periodPrev, a_idHidreletrica), posEquBH, 1.0);
			}
		}//if (a_period > periodIni_stage) {

		// Variável VI
		if (getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1)
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posEquBH, -1.0 / conversor_vazao_volume);

		//VARIÁVEL EVAPORAÇÃO
		if (getVarDecisao_QE_RELseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1) {
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QE_REL(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posEquBH, 1.0);

			// CRIA RESTRIÇÃO EVAPORAÇÃO
			if (a_idPat == IdPatamarCarga_1 && a_dados.getSize1Matriz(a_idHidreletrica, AttMatrizHidreletrica_vazao_evaporada_meta) == 0)
				criarRestricoesEvaporacao(a_TSS, a_dados, a_idEstagio, a_period, a_idHidreletrica, a_periodNext);
		}

		// Variáveis de Hidreletricas à Montante

		const int numero_hidreletricas_montante = a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_montante);
		for (int m = 1; m <= numero_hidreletricas_montante; m++) {
			const IdHidreletrica idHidreletrica_montante = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_montante, m, IdHidreletrica());

			const int tempo_viagem_agua = a_dados.getAtributo(idHidreletrica_montante, AttComumHidreletrica_tempo_viagem_agua, int());

			if (tempo_viagem_agua > 0) {
				Periodo periodo_lag = Periodo(a_period.getTipoPeriodo(), Periodo(TipoPeriodo_horario, a_period) - tempo_viagem_agua);
				// Variável QDLAG
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(criarVariaveisDecisao_VariaveisEstado_Restricoes_QDLAG(a_TSS, a_dados, a_idEstagio, a_period, idHidreletrica_montante, periodo_lag), posEquBH, -1.0);
			}
			else {

				// Variável QD
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD(a_TSS, a_idEstagio, a_period, a_idPat, idHidreletrica_montante), posEquBH, -1.0);

			}//else {

		} // for (int m = 1; m <= numero_hidreletricas_montante; m++) {

		// Variáveis de Hidreletricas à Montante desvio
		const int numero_hidreletricas_montante_desvio = a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_montante_desvio);
		for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

			const IdHidreletrica idHidreletrica_montante_desvio = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_montante_desvio, m, IdHidreletrica());

			// Variável QR que "entra" na usina
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QR(a_TSS, a_idEstagio, a_period, a_idPat, idHidreletrica_montante_desvio, a_idHidreletrica), posEquBH, -1.0);

		}//for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

		// Variável QR que "sai" da usina
		if (a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica()) != IdHidreletrica_Nenhum)
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QR(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posEquBH, 1.0);

		// Variável QM
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QM(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquBH, -1.0);

		// Variável QD
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquBH, 1.0);

		//QI
		if (getVarDecisao_QIseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1)
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquBH, 1.0);

		///////////////////////////
		//Intercâmbios Hidráulicos
		///////////////////////////
		//Nota:
		//Variável QR (desvios de água por meio de canais -> jusante_desvio do HIDR.dat)
		//Variável QI (irrigação, retiradas)

		// hidreletrica_origem

		const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_origem = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_origem, a_idHidreletrica);

		const int idIntercambioHidraulico_origem_size = int(idIntercambioHidraulico_origem.size());

		for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

			const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

			if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

				if (getVarDecisao_QIseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())), posEquBH, 1.0);

			}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

			else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada) {

				if (a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_minimo, a_period, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_maximo, a_period, double()) > 0) {

					//Nesta condição o QI_REL entra no balanço hídrico
					if (getVarDecisao_QI_RELseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI_REL(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posEquBH, 1.0);

				}//if (a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_minimo, a_period, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_maximo, a_period, double()) > 0) {
				else {

					//Se o QI for negativo, não tem problema de inviabilizar o balanço hídrico
					if (getVarDecisao_QIseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posEquBH, 1.0);

				}//else {

			}//else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada) {

			else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_pereira_barreto) {

				if (a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_minimo, a_period, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_maximo, a_period, double()) > 0) {

					//Nesta condição o QILS_TRI_REL entra no balanço hídrico
					if (getVarDecisao_QILS_TRI_REL(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos)) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QILS_TRI_REL(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos)), posEquBH, 1.0);

				}//if (a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_minimo, a_period, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_maximo, a_period, double()) > 0) {
				else {

					//Se o QILS_TRI for negativo, não tem problema de inviabilizar o balanço hídrico
					if (getVarDecisao_QILS_TRI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos)) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QILS_TRI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos)), posEquBH, 1.0);

				}//else {

			}//else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_pereira_barreto) {

		}//for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

		// hidreletrica_destino

		const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_destino = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_destino, a_idHidreletrica);

		const int idIntercambioHidraulico_destino_size = int(idIntercambioHidraulico_destino.size());

		for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {

			const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

			if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

				if (getVarDecisao_QIseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())), posEquBH, -1.0);

			}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

			else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_pereira_barreto) {

				if (getVarDecisao_QILS_TRIseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_destino.at(pos)) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QILS_TRI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_destino.at(pos)), posEquBH, -1.0);

			}//else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_pereira_barreto) {

		}//for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {

		///////////////////////////
		//Usinas de bombeamento
		///////////////////////////
		//Nota: as usinas a montante = destino e jusante = origem

		const std::vector<IdUsinaElevatoria> idUsinaElevatoria_origem = a_dados.vetorUsinaElevatoria.getIdObjetos(AttComumUsinaElevatoria_usina_origem_bombeamento, a_idHidreletrica);

		const int idUsinaElevatoria_origem_size = int(idUsinaElevatoria_origem.size());

		for (int pos = 0; pos < idUsinaElevatoria_origem_size; pos++)
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QB(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria_origem.at(pos)), posEquBH, a_dados.getElementoVetor(idUsinaElevatoria_origem.at(pos), AttVetorUsinaElevatoria_fator_disponibilidade, a_period, double()));

		const std::vector<IdUsinaElevatoria> idUsinaElevatoria_destino = a_dados.vetorUsinaElevatoria.getIdObjetos(AttComumUsinaElevatoria_usina_destino_bombeamento, a_idHidreletrica);

		const int idUsinaElevatoria_destino_size = int(idUsinaElevatoria_destino.size());

		for (int pos = 0; pos < idUsinaElevatoria_destino_size; pos++)
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QB(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria_destino.at(pos)), posEquBH, -a_dados.getElementoVetor(idUsinaElevatoria_destino.at(pos), AttVetorUsinaElevatoria_fator_disponibilidade, a_period, double()));


	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesBalancoHidraulicoUsinaByVazao(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesBalancoHidraulicoUsinaByVolume(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodIni_stage, Periodo& a_periodEnd_stage, Periodo& a_periodPrev, Periodo& a_period, Periodo& a_periodNext, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPat) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		// --------------------
		//
		// Vazao Afluente Montante
		// 
		// --------------------

		const double conversor_vazao_volume = a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, a_period, a_idPat, double());

		try {

			const int posEquQMT = addEquLinear_QM(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQMT, 0.0);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QM(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquQMT, 1.0);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_YH(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posEquQMT, -1.0);

		}// try

		catch (const std::exception& erro) { throw std::invalid_argument("Erro Restricao Vazao Montante: \n" + std::string(erro.what())); }

		// --------------------------
		//
		// Vazao Defluente 
		// 
		// --------------------------

		try {

			const int posEquQD = addEquLinear_QD(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQD, 0.0);

			//QD
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquQD, 1.0);

			//QTDISP
			if (getVarDecisao_QTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquQD, -1.0);

			//QV
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QV(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquQD, -1.0);


		}// try

		catch (const std::exception& erro) { throw std::invalid_argument("Erro Restricao Vazao Defluente Disponivel: \n" + std::string(erro.what())); }


		// -------------------------------
		//
		// Balanço Hídrico no Reservatório
		// 
		// -------------------------------

		if (a_idPat == IdPatamarCarga_1) {

			//EQUAÇÃO DE BALANÇO HIDRAULICO
			const int posEquBH = addEquLinear_BH_VOL(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquBH, 0.0);

			// ENCHENDO VOLUME MORTO 
			if (a_period > a_periodIni_stage) {

				if (a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_enchendo_volume_morto, a_periodPrev, int()) == 1) {

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquBH, a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo, a_periodPrev, double()));

					const int posIneVM = addIneLinear_VMORTO_ENCH(a_TSS, a_idEstagio, a_periodPrev, a_idPat, a_idHidreletrica);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneVM, a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_meta_enchimento_volume_morto, a_periodPrev, double()));
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posEquBH, 1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VMORTO_FINF(a_TSS, a_idEstagio, a_periodPrev, a_idHidreletrica), posEquBH, 1.0);
				}
			}//if (a_period > periodIni_stage) {


			//VARIÁVEL EVAPORAÇÃO
			const double conversor_vazao_volume_periodo = a_dados.getElementoVetor(AttVetorDados_conversor_vazao_volume, a_period, double());

			if (getVarDecisao_QE_RELseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1) {

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QE_REL(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posEquBH, conversor_vazao_volume_periodo);

				if (a_dados.getSize1Matriz(a_idHidreletrica, AttMatrizHidreletrica_vazao_evaporada_meta) == 0)
					criarRestricoesEvaporacao(a_TSS, a_dados, a_idEstagio, a_period, a_idHidreletrica, a_periodNext);
			}

			// Variável VI
			if (getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posEquBH, -1.0);

			if (a_period < a_periodEnd_stage) {

				if (getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_periodNext, a_idHidreletrica) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, a_idHidreletrica), posEquBH, 1.0);
			}
			else if ((a_period == a_periodEnd_stage && (getVarDecisao_VFseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1)))
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posEquBH, 1.0);

			// Variáveis de Hidreletricas à Montante

			const int numero_hidreletricas_montante = a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_montante);
			for (int m = 1; m <= numero_hidreletricas_montante; m++) {
				const IdHidreletrica idHidreletrica_montante = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_montante, m, IdHidreletrica());

				const int tempo_viagem_agua = a_dados.getAtributo(idHidreletrica_montante, AttComumHidreletrica_tempo_viagem_agua, int());

				if (tempo_viagem_agua > 0) {
					Periodo periodo_lag = Periodo(a_period.getTipoPeriodo(), Periodo(TipoPeriodo_horario, a_period) - tempo_viagem_agua);
					// Variável QDLAG
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(criarVariaveisDecisao_VariaveisEstado_Restricoes_QDLAG(a_TSS, a_dados, a_idEstagio, a_period, idHidreletrica_montante, periodo_lag), posEquBH, -conversor_vazao_volume_periodo);
				}

				else

					// Variável QD
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD(a_TSS, a_idEstagio, a_period, a_idPat, idHidreletrica_montante), posEquBH, -conversor_vazao_volume_periodo);


			} // for (int m = 1; m <= numero_hidreletricas_montante; m++) {

		} // if (a_idPat == IdPatamarCarga_1){

		const int posEquBH = getEquLinear_BH_VOL(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, a_idHidreletrica);

		// Variáveis de Hidreletricas à Montante desvio
		const int numero_hidreletricas_montante_desvio = a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_montante_desvio);
		for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

			const IdHidreletrica idHidreletrica_montante_desvio = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_montante_desvio, m, IdHidreletrica());

			// Variável QR que "entra" na usina
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QR(a_TSS, a_idEstagio, a_period, a_idPat, idHidreletrica_montante_desvio, a_idHidreletrica), posEquBH, -conversor_vazao_volume);

		}//for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

		// Variável QR que "sai" da usina
		if(a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica()) != IdHidreletrica_Nenhum)
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QR(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posEquBH, conversor_vazao_volume);


		// Variável QD
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquBH, conversor_vazao_volume);

		// Variável QM
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QM(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquBH, -conversor_vazao_volume);

		//QI
		if (getVarDecisao_QIseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1)
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquBH, conversor_vazao_volume);

		///////////////////////////
		//Intercâmbios Hidráulicos
		///////////////////////////
		//Nota:
		//Variável QR (desvios de água por meio de canais -> jusante_desvio do HIDR.dat)
		//Variável QI (irrigação, retiradas)

		// hidreletrica_origem

		const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_origem = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_origem, a_idHidreletrica);

		const int idIntercambioHidraulico_origem_size = int(idIntercambioHidraulico_origem.size());

		for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

			const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

			if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

				if (getVarDecisao_QIseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())), posEquBH, conversor_vazao_volume);

			}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

			else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada) {

				if (a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_minimo, a_period, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_maximo, a_period, double()) > 0) {

					//Nesta condição o QI_REL entra no balanço hídrico
					if (getVarDecisao_QI_RELseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI_REL(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posEquBH, conversor_vazao_volume);

				}//if (a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_minimo, a_period, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_maximo, a_period, double()) > 0) {
				else {

					//Se o QI for negativo, não tem problema de inviabilizar o balanço hídrico
					if (getVarDecisao_QIseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posEquBH, conversor_vazao_volume);

				}//else {

			}//else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada) {
			else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_pereira_barreto) {

				if (a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_minimo, a_period, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_maximo, a_period, double()) > 0) {

					//Nesta condição o QILS_TRI_REL entra no balanço hídrico
					if (getVarDecisao_QILS_TRI_REL(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos)) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QILS_TRI_REL(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos)), posEquBH, conversor_vazao_volume);

				}//if (a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_minimo, a_period, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_maximo, a_period, double()) > 0) {
				else {

					//Se o QILS_TRI for negativo, não tem problema de inviabilizar o balanço hídrico
					if (getVarDecisao_QILS_TRI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos)) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QILS_TRI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos)), posEquBH, conversor_vazao_volume);

				}//else {


			}//else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_pereira_barreto) {

		}//for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

		// hidreletrica_destino

		const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_destino = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_destino, a_idHidreletrica);

		const int idIntercambioHidraulico_destino_size = int(idIntercambioHidraulico_destino.size());

		for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {

			const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

			if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

				if (getVarDecisao_QIseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())), posEquBH, -conversor_vazao_volume);

			}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

			else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_pereira_barreto) {

				if (getVarDecisao_QILS_TRIseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_destino.at(pos)) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QILS_TRI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_destino.at(pos)), posEquBH, -conversor_vazao_volume);

			}//else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_pereira_barreto) {

		}//for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {

		///////////////////////////

		//Usinas de bombeamento
		//Nota: as usinas a montante = destino e jusante = origem

		const std::vector<IdUsinaElevatoria> idUsinaElevatoria_origem = a_dados.vetorUsinaElevatoria.getIdObjetos(AttComumUsinaElevatoria_usina_origem_bombeamento, a_idHidreletrica);

		const int idUsinaElevatoria_origem_size = int(idUsinaElevatoria_origem.size());

		for (int pos = 0; pos < idUsinaElevatoria_origem_size; pos++)
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QB(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria_origem.at(pos)), posEquBH, conversor_vazao_volume * a_dados.getElementoVetor(idUsinaElevatoria_origem.at(pos), AttVetorUsinaElevatoria_fator_disponibilidade, a_period, double()));

		const std::vector<IdUsinaElevatoria> idUsinaElevatoria_destino = a_dados.vetorUsinaElevatoria.getIdObjetos(AttComumUsinaElevatoria_usina_destino_bombeamento, a_idHidreletrica);

		const int idUsinaElevatoria_destino_size = int(idUsinaElevatoria_destino.size());

		for (int pos = 0; pos < idUsinaElevatoria_destino_size; pos++)
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QB(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria_destino.at(pos)), posEquBH, -conversor_vazao_volume * a_dados.getElementoVetor(idUsinaElevatoria_destino.at(pos), AttVetorUsinaElevatoria_fator_disponibilidade, a_period, double()));


	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesBalancoHidraulicoUsinaByVolume(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesEvaporacao(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period, const IdHidreletrica a_idHidreletrica, Periodo &a_proximo_periodo_estudo) {

	try {
		const double conversor_vazao_volume = a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, a_period, IdPatamarCarga_1, double());
		// EVAPORAÇÃO
		const int posEquEV = addEquLinear_QE(a_TSS, a_idEstagio, a_period, a_idHidreletrica);

		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquEV, a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_coef_linear_evaporacao_0, a_period, double()));
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QE(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posEquEV, 1.0);

		if (getVarDecisao_VMEDseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1)
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VMED(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posEquEV, -a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_coef_linear_evaporacao_1, a_period, double()));

		//EVAPORAÇÃO RELAXADA 
		const int posEquEV_REL = addEquLinear_QE_REL(a_TSS, a_idEstagio, a_period, a_idHidreletrica);
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquEV_REL, 0.0);
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QE(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posEquEV_REL, -1.0);
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QE_REL(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posEquEV_REL, 1.0);
		if (getVarDecisao_QE_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1) {
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QE_FINF(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posEquEV_REL, 1.0);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QE_FINF(a_TSS, a_idEstagio, a_period, a_idHidreletrica), 1.2 * a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_coef_linear_evaporacao_0, a_period, double()));
		}

	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesEvaporacao(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesAtendimentoDemanda(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period, const IdPatamarCarga a_idPat) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const IdUsinaElevatoria idUsiElHIni = a_dados.getMenorId(IdUsinaElevatoria());
		const IdUsinaElevatoria idUsiElHOut = a_dados.getIdOut(IdUsinaElevatoria());

		const IdIntercambio idInterIni = a_dados.getMenorId(IdIntercambio());
		const IdIntercambio idInterOut = a_dados.getIdOut(IdIntercambio());

		const IdHidreletrica idUHEIni = a_dados.getMenorId(IdHidreletrica());
		const IdHidreletrica idUHEOut = a_dados.getIdOut(IdHidreletrica());

		const IdTermeletrica idUTEIni = a_dados.getMenorId(IdTermeletrica());
		const IdTermeletrica idUTEOut = a_dados.getIdOut(IdTermeletrica());

		const IdContrato idContratoIni = a_dados.getMenorId(IdContrato());
		const IdContrato idContratoOut = a_dados.getIdOut(IdContrato());

		const IdUsinaEolica idEolIni = a_dados.getMenorId(IdUsinaEolica());
		const IdUsinaEolica IdEolOut = a_dados.getIdOut(IdUsinaEolica());

		const IdSubmercado idSSEIni = a_dados.getMenorId(IdSubmercado());
		const IdSubmercado idSSEOut = a_dados.getIdOut(IdSubmercado());
		for (IdSubmercado idSSE = idSSEIni; idSSE < idSSEOut; a_dados.vetorSubmercado.incr(idSSE)) {

			const int posEquAD = addEquLinear_PL(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquAD, 0.0);

			// Variável PL
			if (getVarDecisao_PLseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PL(a_TSS, a_idEstagio, a_period, a_idPat, idSSE), posEquAD, -1.0);

			//
			// Variável Potência Usina Nao Simulada PN
			//

			const IdUsinaNaoSimulada menorIdUsinaNaoSimulada = a_dados.getMenorId(idSSE, IdUsinaNaoSimulada());
			const IdUsinaNaoSimulada maiorIdUsinaNaoSimulada = a_dados.getIdOut(idSSE, IdUsinaNaoSimulada());

			for (IdUsinaNaoSimulada idUsinaNaoSimulada = menorIdUsinaNaoSimulada; idUsinaNaoSimulada < maiorIdUsinaNaoSimulada; a_dados.incr(idSSE, idUsinaNaoSimulada)) {

				if (getVarDecisao_PNseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, idUsinaNaoSimulada) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PN(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, idUsinaNaoSimulada), posEquAD, 1.0);

			} // for (IdUsinaNaoSimulada idUsinaNaoSimulada = menorIdUsinaNaoSimulada; idUsinaNaoSimulada < maiorIdUsinaNaoSimulada; a_dados.incr(idSSE, idUsinaNaoSimulada)) { {

			//USINAS EOLICAS 

			for (IdUsinaEolica idUsinaEolica = idEolIni; idUsinaEolica < IdEolOut; a_dados.vetorUsinaEolica.incr(idUsinaEolica))
				if (a_dados.getAtributo(idUsinaEolica, AttComumUsinaEolica_submercado, IdSubmercado()) == idSSE)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PE(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaEolica), posEquAD, 1.0);


			// Variaveis Patamares Deficit
			const IdPatamarDeficit idPatDefIni = a_dados.getMenorId(idSSE, IdPatamarDeficit());
			const IdPatamarDeficit idPatDefOut = a_dados.getIdOut(idSSE, IdPatamarDeficit());

			for (IdPatamarDeficit idPatamarDeficit = idPatDefIni; idPatamarDeficit < idPatDefOut; a_dados.vetorSubmercado.at(idSSE).vetorPatamarDeficit.incr(idPatamarDeficit)) {

				// Variável PD
				if (getVarDecisao_PDseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, idPatamarDeficit) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PD(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, idPatamarDeficit), posEquAD, 1.0);

			} // for (IdPatamarDeficit idPatamarDeficit = IdPatamarDeficit_1; idPatamarDeficit <= maiorIdPatamarDeficit; idPatamarDeficit++) {


			// CONTRATOS
			for (IdContrato idContrato = idContratoIni; idContrato < idContratoOut; a_dados.vetorContrato.incr(idContrato)) {

				if ((getVarDecisao_CEXPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idContrato) > -1) && a_dados.getAtributo(idContrato, AttComumContrato_submercado, IdSubmercado()) == idSSE)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_CEXP(a_TSS, a_idEstagio, a_period, a_idPat, idContrato), posEquAD, -1.0);

				if ((getVarDecisao_CIMPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idContrato) > -1) && a_dados.getAtributo(idContrato, AttComumContrato_submercado, IdSubmercado()) == idSSE)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_CIMP(a_TSS, a_idEstagio, a_period, a_idPat, idContrato), posEquAD, 1.0);

			}//for (IdContrato idContrato = IdContrato_1; idContrato < maiorIdContrato; idContrato++) {

			// Variaveis Termeletricas

			for (IdTermeletrica idUTE = idUTEIni; idUTE < idUTEOut; a_dados.vetorTermeletrica.incr(idUTE)) {

				const TipoDetalhamentoProducaoTermeletrica tipo_detalhamento_producao = a_dados.getAtributo(idUTE, AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica());

				if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina) {

					// Variável PTDISP
					if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE) > -1) {

						if (a_dados.getAtributo(idUTE, AttComumTermeletrica_submercado, IdSubmercado()) == idSSE) {
							if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE) > -1)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), posEquAD, 1.0);
						}

					} // if (getVarDecisao_PFseExistir(a_idEstagio, a_period, a_idPat, idUTE) > -1){

				} // if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina) {

				else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {

					for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= a_dados.getMaiorId(idUTE, IdUnidadeUTE()); idUnidadeUTE++) {

						// Variável PTDISP
						if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE) > -1) {

							if (a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_submercado, IdSubmercado()) == idSSE) {
								if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE) > -1)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posEquAD, 1.0);
							}

						} // if (getVarDecisao_PTDISPseExistir(a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE) > -1) {

					} // for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= a_dados.getMaiorId(idUTE, IdUnidadeUTE()); idUnidadeUTE++) {

				} // else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {

			} // for (IdTermeletrica idUTE = a_dados.getMenorId(IdTermeletrica()); idUTE <= maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idUTE)) {


			// Variaveis Hidreletricas
			for (IdHidreletrica idUHE = idUHEIni; idUHE < idUHEOut; a_dados.vetorHidreletrica.incr(idUHE)) {

				const TipoDetalhamentoProducaoHidreletrica tipo_detalhamento_producao = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_tipo_detalhamento_producao, a_period, TipoDetalhamentoProducaoHidreletrica());

				//
				// Producao Por Hidreletrica
				//

				if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina) {

					// Variável PHDISP
					if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1) {

						if (a_dados.getAtributo(idUHE, AttComumHidreletrica_submercado, IdSubmercado()) == idSSE)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posEquAD, 1.0);

					} // if (getVarDecisao_PHDISPseExistir(a_idEstagio, a_period, a_idPat, idUHE) > -1) {

				} // if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina){

				//
				// Producao Por Conjunto Hidraulico
				//

				else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {
					for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idUHE, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

						// Variável PHDISP
						if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico) > -1) {

							if (a_dados.getAtributo(idUHE, idConjuntoHidraulico, AttComumConjuntoHidraulico_submercado, IdSubmercado()) == idSSE)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico), posEquAD, 1.0);

						} // if (getVarDecisao_PHDISPseExistir(a_idEstagio, a_period, a_idPat, idUHE) > -1) {

					} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idUHE, IdConjuntoHidraulico()); idConjuntoHidraulico++) {
				} // else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

				//
				// Producao Por Unidade UHE
				//

				else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) {
					for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idUHE, IdConjuntoHidraulico()); idConjuntoHidraulico++) {
						for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(idUHE, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {

							// Variável PHDISP
							if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE) > -1) {

								if (a_dados.getAtributo(idUHE, idConjuntoHidraulico, idUnidadeUHE, AttComumUnidadeUHE_submercado, IdSubmercado()) == idSSE)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE), posEquAD, 1.0);

							} // if (getVarDecisao_PHDISPseExistir(a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE) > -1) {

						} // for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(idUHE, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {
					} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idUHE, IdConjuntoHidraulico()); idConjuntoHidraulico++) {
				} // else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) {

			} // for (IdHidreletrica idUHE = a_dados.getMenorId(IdHidreletrica()); idUHE <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idUHE)) {


			// Variaveis Intercambio
			for (IdIntercambio idIntercambio = idInterIni; idIntercambio < idInterOut; a_dados.vetorIntercambio.incr(idIntercambio)) {

				// Variável PI
				if (getVarDecisao_PIseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio) > -1) {

					if (a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_origem, IdSubmercado()) == idSSE)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio), posEquAD, -1.0);

					else if (a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_destino, IdSubmercado()) == idSSE)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio), posEquAD, 1.0);

				} // if (getVarDecisao_PIseExistir(a_idEstagio, a_period, a_idPat, idIntercambio) > -1){

			} // for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= maiorIdIntercambio; idIntercambio++) {

			//Consumo das usinas elevatórias (QB)

			for (IdUsinaElevatoria idUsinaElevatoria = idUsiElHIni; idUsinaElevatoria < idUsiElHOut; a_dados.vetorUsinaElevatoria.incr(idUsinaElevatoria)) {

				if (a_dados.getAtributo(idUsinaElevatoria, AttComumUsinaElevatoria_submercado, IdSubmercado()) == idSSE) {

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PBDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria), posEquAD, -1.0);

				}//if (a_dados.getAtributo(idUsinaElevatoria, AttComumUsinaElevatoria_submercado, IdSubmercado()) == idSSE) {

			}//for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria <= a_maiorIdUsinaElevatoria; idUsinaElevatoria++) {

		} // for (IdSubmercado idSSE = idSSEIni; idSSE < idSSEOut; a_dados.vetorSubmercado.incr(idSSE)) {

	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesAtendimentoDemanda(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesInformacaoMedia(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period, const IdPatamarCarga a_idPat) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_period, IdPatamarCarga()) == IdPatamarCarga_1)
			return;

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

			// PL
			const IdSubmercado idSSEIni = a_dados.getMenorId(IdSubmercado());
			const IdSubmercado idSSEOut = a_dados.getIdOut(IdSubmercado());

			for (IdSubmercado idSSE = idSSEIni; idSSE < idSSEOut; a_dados.vetorSubmercado.incr(idSSE)) {

				if (getVarDecisao_PLseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE) > -1) {

					if (getVarDecisao_PLseExistir(a_TSS, a_idEstagio, a_period, idSSE) < 0)
						addVarDecisao_PL(a_TSS, a_idEstagio, a_period, idSSE, 0.0, infinito, 0.0);

					int posEquDemanda = getEquLinear_PLseExistir(a_TSS, a_idEstagio, a_period, idSSE);

					if (posEquDemanda < 0) {
						posEquDemanda = addEquLinear_PL(a_TSS, a_idEstagio, a_period, idSSE);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquDemanda, 0.0);
						// Variável PL Media
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PL(a_TSS, a_idEstagio, a_period, idSSE), posEquDemanda, 1.0);
					}

					// Variável PL
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PL(a_TSS, a_idEstagio, a_period, a_idPat, idSSE), posEquDemanda, -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, a_idPat, double()));

				} // if (getVarDecisao_PLseExistir(a_idEstagio, a_period, a_idPat, idSSE) > -1) {

				const IdPatamarDeficit idPatDefIni = a_dados.getMenorId(idSSE, IdPatamarDeficit());
				const IdPatamarDeficit idPatDefOut = a_dados.getIdOut(idSSE, IdPatamarDeficit());

				for (IdPatamarDeficit idPatamarDeficit = idPatDefIni; idPatamarDeficit < idPatDefOut; a_dados.vetorSubmercado.at(idSSE).vetorPatamarDeficit.incr(idPatamarDeficit)) {

					if (getVarDecisao_PDseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, idPatamarDeficit) > -1) {

						if (getVarDecisao_PDseExistir(a_TSS, a_idEstagio, a_period, idSSE) < 0)
							addVarDecisao_PD(a_TSS, a_idEstagio, a_period, idSSE, 0.0, infinito, 0.0);

						int posEquDeficit = getEquLinear_PDseExistir(a_TSS, a_idEstagio, a_period, idSSE);

						if (posEquDeficit < 0) {
							posEquDeficit = addEquLinear_PD(a_TSS, a_idEstagio, a_period, idSSE);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquDeficit, 0.0);
							// Variável PL Media
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PD(a_TSS, a_idEstagio, a_period, idSSE), posEquDeficit, 1.0);
						}

						// Variável PD
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PD(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, idPatamarDeficit), posEquDeficit, -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, a_idPat, double()));

					} // if (getVarDecisao_PDseExistir(a_idEstagio, a_period, a_idPat, idSSE, idPatamarDeficit) > -1){

				} // for (IdPatamarDeficit idPatamarDeficit = IdPatamarDeficit_1; idPatamarDeficit <= maiorIdPatamarDeficit; idPatamarDeficit++) {

				const IdIntercambio idInterIni = a_dados.getMenorId(IdIntercambio());
				const IdIntercambio idInterOut = a_dados.getIdOut(IdIntercambio());

				for (IdIntercambio idIntercambio = idInterIni; idIntercambio < idInterOut; a_dados.vetorIntercambio.incr(idIntercambio)) {

					if (getVarDecisao_PIseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio) > -1) {

						if ((a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_origem, IdSubmercado()) == idSSE) || (a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_destino, IdSubmercado()) == idSSE)) {

							if (getVarDecisao_PIseExistir(a_TSS, a_idEstagio, a_period, idSSE) < 0)
								addVarDecisao_PI(a_TSS, a_idEstagio, a_period, idSSE, -infinito, infinito, 0.0);

							int posEquIntercambio = getEquLinear_PIseExistir(a_TSS, a_idEstagio, a_period, idSSE);

							if (posEquIntercambio < 0) {
								posEquIntercambio = addEquLinear_PI(a_TSS, a_idEstagio, a_period, idSSE);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquIntercambio, 0.0);
								// Variável PI Media
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_period, idSSE), posEquIntercambio, 1.0);
							}

							// Variável PI
							if (a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_origem, IdSubmercado()) == idSSE)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio), posEquIntercambio, a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, a_idPat, double()));

							else if (a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_destino, IdSubmercado()) == idSSE)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio), posEquIntercambio, -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, a_idPat, double()));

						} // if ((a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_origem, IdSubmercado()) == idSSE) || (a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_destino, IdSubmercado()) == idSSE)) {

					} // if (getVarDecisao_PIseExistir(a_idEstagio, a_period, a_idPat, idIntercambio) > -1){

				} // for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio < a_maiorIdIntercambio; idIntercambio++) {

				const IdUsinaNaoSimulada menorIdUsinaNaoSimulada = a_dados.getMenorId(idSSE, IdUsinaNaoSimulada());
				const IdUsinaNaoSimulada maiorIdUsinaNaoSimulada = a_dados.getIdOut(idSSE, IdUsinaNaoSimulada());

				for (IdUsinaNaoSimulada idUsinaNaoSimulada = menorIdUsinaNaoSimulada; idUsinaNaoSimulada < maiorIdUsinaNaoSimulada; a_dados.incr(idSSE, idUsinaNaoSimulada)) {

					if (getVarDecisao_PNseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, idUsinaNaoSimulada) > -1) {

						if (getVarDecisao_PNseExistir(a_TSS, a_idEstagio, a_period, idSSE) < 0)
							addVarDecisao_PN(a_TSS, a_idEstagio, a_period, idSSE, 0.0, infinito, 0.0);

						int posEquDeficit = getEquLinear_PNseExistir(a_TSS, a_idEstagio, a_period, idSSE);

						if (posEquDeficit < 0) {
							posEquDeficit = addEquLinear_PN(a_TSS, a_idEstagio, a_period, idSSE);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquDeficit, 0.0);
							// Variável PN Media
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PN(a_TSS, a_idEstagio, a_period, idSSE), posEquDeficit, 1.0);
						}

						// Variável PN
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PN(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, idUsinaNaoSimulada), posEquDeficit, -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, a_idPat, double()));

					} // if (getVarDecisao_PNseExistir(a_idEstagio, a_period, a_idPat, idSSE, idUsinaNaoSimulada) > -1){

				} // for (IdUsinaNaoSimulada idUsinaNaoSimulada = menorIdUsinaNaoSimulada; idUsinaNaoSimulada < maiorIdUsinaNaoSimulada; a_dados.incr(idSSE, idUsinaNaoSimulada)) {
			} // for (IdSubmercado idSSE = idSSEIni; idSSE < idSSEOut; a_dados.vetorSubmercado.incr(idSSE)) {

			// BOMBEAMENTO
			if (true) {

				const IdUsinaElevatoria idUsiElHIni = a_dados.getMenorId(IdUsinaElevatoria());
				const IdUsinaElevatoria idUsiElHOut = a_dados.getIdOut(IdUsinaElevatoria());
				for (IdUsinaElevatoria idUsinaElevatoria = idUsiElHIni; idUsinaElevatoria < idUsiElHOut; a_dados.vetorUsinaElevatoria.incr(idUsinaElevatoria)) {

					if (getVarDecisao_PBDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria) > -1) {

						if (getVarDecisao_PBDISPseExistir(a_TSS, a_idEstagio, a_period, idUsinaElevatoria) == -1)
							addVarDecisao_PBDISP(a_TSS, a_idEstagio, a_period, idUsinaElevatoria, 0.0, infinito, 0.0);

						if (getEquLinear_PBDISPseExistir(a_TSS, a_idEstagio, a_period, idUsinaElevatoria) == -1) {

							const int posEquPBDISP = addEquLinear_PBDISP(a_TSS, a_idEstagio, a_period, idUsinaElevatoria);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPBDISP, 0.0);
							// Variável PBDISP Media
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PBDISP(a_TSS, a_idEstagio, a_period, idUsinaElevatoria), posEquPBDISP, 1.0);

						} // if (getEquLinear_PBDISPseExistir(a_idEstagio, a_period, idUsinaElevatoria) == -1) {

						// Variável PHDISP
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PBDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria), getEquLinear_PBDISP(a_TSS, a_idEstagio, a_period, idUsinaElevatoria), -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, a_idPat, double()));

					}

				} // for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria < a_dados.getMaiorId(IdUsinaElevatoria()); idUsinaElevatoria++) {
			} // if (true) {

		} // if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica)

		// PHDISP
		if (true) {

			const IdHidreletrica idUHEIni = a_dados.getMenorId(IdHidreletrica());
			const IdHidreletrica idUHEOut = a_dados.getIdOut(IdHidreletrica());

			for (IdHidreletrica idUHE = idUHEIni; idUHE < idUHEOut; a_dados.vetorHidreletrica.incr(idUHE)) {

				if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

					// Variável PHDISP
					if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1) {

						if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, idUHE) < 0)
							addVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);

						if (true) {
							if (getEquLinear_PHDISPseExistir(a_TSS, a_idEstagio, a_period, idUHE) == -1) {
								const int posEquProducaoHidreletrica = addEquLinear_PHDISP(a_TSS, a_idEstagio, a_period, idUHE);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquProducaoHidreletrica, 0.0);
								// Variável PHDISP Media
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, idUHE), posEquProducaoHidreletrica, 1.0);
							}

							// Variável PHDISP
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), getEquLinear_PHDISP(a_TSS, a_idEstagio, a_period, idUHE), -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, a_idPat, double()));
						}

					} // if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1) {

				} // if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

				if (getVarDecisao_QTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1) {

					if (true) {

						if (getVarDecisao_QTDISPseExistir(a_TSS, a_idEstagio, a_period, idUHE) < 0)
							addVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);

						if (getEquLinear_QTDISPseExistir(a_TSS, a_idEstagio, a_period, idUHE) == -1) {
							const int posEquVazaoTurbinadaHidreletrica = addEquLinear_QTDISP(a_TSS, a_idEstagio, a_period, idUHE);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquVazaoTurbinadaHidreletrica, 0.0);
							// Variável QTDISP Media
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, idUHE), posEquVazaoTurbinadaHidreletrica, 1.0);
						}
						// Variável QTDISP
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), getEquLinear_QTDISP(a_TSS, a_idEstagio, a_period, idUHE), -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, a_idPat, double()));
					}

				} // if (getVarDecisao_QTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1) {

				if (getVarDecisao_QVseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1) {

					if (getVarDecisao_QVseExistir(a_TSS, a_idEstagio, a_period, idUHE) < 0)
						addVarDecisao_QV(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);

					if (getEquLinear_QVseExistir(a_TSS, a_idEstagio, a_period, idUHE) == -1) {
						const int posEquQV = addEquLinear_QV(a_TSS, a_idEstagio, a_period, idUHE);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQV, 0.0);
						// Variável QV Media
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QV(a_TSS, a_idEstagio, a_period, idUHE), posEquQV, 1.0);
					}

					// Variável QV
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QV(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), getEquLinear_QV(a_TSS, a_idEstagio, a_period, idUHE), -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, a_idPat, double()));

				} // if (getVarDecisao_QVseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1) {

				if (getVarDecisao_QDseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1) {

					if (getVarDecisao_QDseExistir(a_TSS, a_idEstagio, a_period, idUHE) < 0)
						addVarDecisao_QD(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);

					if (getEquLinear_QDseExistir(a_TSS, a_idEstagio, a_period, idUHE) == -1) {
						const int posEquQD = addEquLinear_QD(a_TSS, a_idEstagio, a_period, idUHE);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQD, 0.0);
						// Variável QD Media
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD(a_TSS, a_idEstagio, a_period, idUHE), posEquQD, 1.0);
					}

					// Variável QD
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), getEquLinear_QD(a_TSS, a_idEstagio, a_period, idUHE), -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, a_idPat, double()));

				} // if (getVarDecisao_QDseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1) {

			} // for (IdHidreletrica idUHE = a_dados.getMenorId(IdHidreletrica()); idUHE < a_maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idUHE)) {

		} // if (true) {


		if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

			// PRODUCAO_TERMELETRICA
			if (true) {

				const IdTermeletrica idUTEIni = a_dados.getMenorId(IdTermeletrica());
				const IdTermeletrica idUTEOut = a_dados.getIdOut(IdTermeletrica());
				for (IdTermeletrica idUTE = idUTEIni; idUTE < idUTEOut; a_dados.vetorTermeletrica.incr(idUTE)) {

					if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, idUTE) < 0)
						addVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, idUTE, 0.0, infinito, 0.0);

					// Variável PTDISP
					if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE) > -1) {

						if (getEquLinear_PTDISPseExistir(a_TSS, a_idEstagio, a_period, idUTE) == -1) {
							const int posEquProducaoTermeletrica = addEquLinear_PTDISP(a_TSS, a_idEstagio, a_period, idUTE);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquProducaoTermeletrica, 0.0);
							// Variável PTDISP Media
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, idUTE), posEquProducaoTermeletrica, 1.0);
						}

						// Variável PTDISP
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), getEquLinear_PTDISP(a_TSS, a_idEstagio, a_period, idUTE), -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, a_idPat, double()));
					} // if (getVarDecisao_PTseExistir(a_idEstagio, a_period, a_idPat, idUTE) > -1) {

				} // for (IdTermeletrica idUTE = a_dados.getMenorId(IdTermeletrica()); idUTE < a_maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idUTE)) {

			} // if (true) {

		} // if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesInformacaoMedia(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}




void ModeloOtimizacao::criarRestricoesCustoOperacao(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period, const IdPatamarCarga a_idPat) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const int posEquZO = addEquLinear_ZO(a_TSS, a_idEstagio, a_period, a_idPat);

		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZO, 0.0);

		// Variável ZO
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZO(a_TSS, a_idEstagio, a_period, a_idPat), posEquZO, 1.0);

		const IdContrato idContratoIni = a_dados.getMenorId(IdContrato());
		const IdContrato idContratoOut = a_dados.getIdOut(IdContrato());

		// CONTRATOS
		for (IdContrato idContrato = idContratoIni; idContrato < idContratoOut; a_dados.vetorContrato.incr(idContrato)) {

			if (getVarDecisao_CEXPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idContrato) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_CEXP(a_TSS, a_idEstagio, a_period, a_idPat, idContrato), posEquZO, a_dados.getElementoMatriz(idContrato, AttMatrizContrato_preco_energia_imp_exp, a_period, a_idPat, double()));

			else if (getVarDecisao_CIMPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idContrato) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_CIMP(a_TSS, a_idEstagio, a_period, a_idPat, idContrato), posEquZO, -a_dados.getElementoMatriz(idContrato, AttMatrizContrato_preco_energia_imp_exp, a_period, a_idPat, double()));

			else
				throw std::invalid_argument("Não encontrada variavel de contrato");

		}//for (IdContrato idContrato = idContratoIni; idContrato < idContratoOut; idContrato++) {

		const IdTermeletrica idUTEIni = a_dados.getMenorId(IdTermeletrica());
		const IdTermeletrica idUTEOut = a_dados.getIdOut(IdTermeletrica());

		// Variável PF
		for (IdTermeletrica idUTE = idUTEIni; idUTE < idUTEOut; a_dados.vetorTermeletrica.incr(idUTE)) {

			const TipoDetalhamentoProducaoTermeletrica tipo_detalhamento_producao = a_dados.getAtributo(idUTE, AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica());
			const double custo_de_operacao = a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_custo_de_operacao, a_period, a_idPat, double());

			if (custo_de_operacao > 0.0) {

				IdUnidadeUTE maiorIdUnidadeUTE = a_dados.getMaiorId(idUTE, IdUnidadeUTE());

				if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina)
					maiorIdUnidadeUTE = IdUnidadeUTE_1;

				for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= maiorIdUnidadeUTE; idUnidadeUTE++) {

					int varPTDISP = -1;

					if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina)
						varPTDISP = getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);
					else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade)
						varPTDISP = getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE);

					if (varPTDISP > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP, posEquZO, -custo_de_operacao);

					if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina)
						break;

				} // for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= a_dados.getMaiorId(idUTE, IdUnidadeUTE()); idUnidadeUTE++) {
			} // if (custo_de_operacao > 0.0){
		} // for (IdTermeletrica idUTE = idUTEIni; idUTE < idUTEOut; a_dados.vetorTermeletrica.incr(idUTE)) {

		const IdSubmercado idSSEIni = a_dados.getMenorId(IdSubmercado());
		const IdSubmercado idSSEOut = a_dados.getIdOut(IdSubmercado());

		// Variáveis Déficit
		for (IdSubmercado idSSE = idSSEIni; idSSE < idSSEOut; a_dados.vetorSubmercado.incr(idSSE)) {

			const IdPatamarDeficit maiorIdPatamarDeficit = a_dados.getMaiorId(idSSE, IdPatamarDeficit());
			for (IdPatamarDeficit idPatamarDeficit = IdPatamarDeficit_1; idPatamarDeficit <= maiorIdPatamarDeficit; idPatamarDeficit++) {

				// Variável PD
				if (getVarDecisao_PDseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, idPatamarDeficit) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PD(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, idPatamarDeficit), posEquZO, -a_dados.getElementoMatriz(idSSE, idPatamarDeficit, AttMatrizPatamarDeficit_custo, a_period, a_idPat, double()));

			} // for (IdPatamarDeficit idPatamarDeficit = IdPatamarDeficit_1; idPatamarDeficit <= maiorIdPatamarDeficit; idPatamarDeficit++) {

		} // for (IdSubmercado idSSE = idSSEIni; idSSE < idSSEOut; a_dados.vetorSubmercado.incr(idSSE)){

	}// try {
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesCustoOperacao(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }

}

void ModeloOtimizacao::criarRestricoesCustoPenalidade(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period) {
	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const int posEquZP = getEquLinear_ZP(a_TSS, a_idEstagio, a_period);

		const IdRestricaoOperativaUHE idResOpIni = a_dados.getMenorId(IdRestricaoOperativaUHE());
		const IdRestricaoOperativaUHE idResOpOut = a_dados.getIdOut(IdRestricaoOperativaUHE());

		// Variáveis Restrição Operativa UHE
		for (IdRestricaoOperativaUHE idResOp = idResOpIni; idResOp < idResOpOut; a_dados.vetorRestricaoOperativaUHE.incr(idResOp)) {

			//
			// Volume Armazenado
			//
			
			if (a_dados.getAtributo(idResOp, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_volume_armazenado) {

				// Variável RHV_FINF
				if (getVarDecisao_RHV_FINFseExistir(a_TSS, a_idEstagio, a_period, idResOp) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHV_FINF(a_TSS, a_idEstagio, a_period, idResOp), posEquZP, -a_dados.getAtributo(idResOp, AttComumRestricaoOperativaUHE_penalidade_restricao_operativa_UHE, double()));

				// Variável RHV_FSUP
				if (getVarDecisao_RHV_FSUPseExistir(a_TSS, a_idEstagio, a_period, idResOp) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHV_FSUP(a_TSS, a_idEstagio, a_period, idResOp), posEquZP, -a_dados.getAtributo(idResOp, AttComumRestricaoOperativaUHE_penalidade_restricao_operativa_UHE, double()));

			}//if (a_dados.getAtributo(idResOp, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_volume_armazenado) {
	

			//
			// Energia Armazenada
			//

			if (a_dados.getAtributo(idResOp, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_energia_armazenada) {

				// Variável RHE_FINF
				if (getVarDecisao_RHE_FINFseExistir(a_TSS, a_idEstagio, a_period, idResOp) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHE_FINF(a_TSS, a_idEstagio, a_period, idResOp), posEquZP, -a_dados.getAtributo(idResOp, AttComumRestricaoOperativaUHE_penalidade_restricao_operativa_UHE, double()));

				// Variável RHE_FSUP
				if (getVarDecisao_RHE_FSUPseExistir(a_TSS, a_idEstagio, a_period, idResOp) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHE_FSUP(a_TSS, a_idEstagio, a_period, idResOp), posEquZP, -a_dados.getAtributo(idResOp, AttComumRestricaoOperativaUHE_penalidade_restricao_operativa_UHE, double()));

			}//if (a_dados.getAtributo(idResOp, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_energia_armazenada) {


		} // for (IdRestricaoOperativaUHE idResOp = IdRestricaoOperativaUHE_1; idResOp < maiorIdRestricaoOperativaUHE; idResOp++) {

		const IdHidreletrica idUHEIni = a_dados.getMenorId(IdHidreletrica());
		const IdHidreletrica idUHEOut = a_dados.getIdOut(IdHidreletrica());

		// Variáveis Hidreletricas
		for (IdHidreletrica idUHE = idUHEIni; idUHE < idUHEOut; a_dados.vetorHidreletrica.incr(idUHE)) {

			// Variável QE_FINF
			if ((getVarDecisao_QE_FINFseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1))
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QE_FINF(a_TSS, a_idEstagio, a_period, idUHE), posEquZP, -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_evaporacao, double()));

			// Variável YHF Penalizacao e / ou Viabilidade Hidraulica
			if (getVarDecisao_YHFseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_YHF(a_TSS, a_idEstagio, a_period, idUHE), posEquZP, -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_afluencia_incremental, double()));

		} // for (IdHidreletrica idUHE = a_dados.getMenorId(IdHidreletrica()); idUHE < maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idUHE)) {

		if (getElementoVetor(AttVetorModeloOtimizacao_alguma_variavel_aleatoria_hidrologica_com_truncamento, a_idEstagio, int()) == 1) {

			const IdProcessoEstocastico idPEhidrologico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

			const Periodo periodo_otimizacao = getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

			// Variáveis Hidreletricas Folga Afluencia Incremental
			const SmartEnupla<Periodo, int> horizonte_espaco_amostral_hidrologico = getElementosMatriz(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, a_idEstagio, Periodo(), int());
			bool sobreposicao_encontrada = false;
			for (Periodo periodo = horizonte_espaco_amostral_hidrologico.getIteradorInicial(); periodo <= horizonte_espaco_amostral_hidrologico.getIteradorFinal(); horizonte_espaco_amostral_hidrologico.incrementarIterador(periodo)) {
				const double sobreposicao = a_period.sobreposicao(periodo);
				if (sobreposicao > 0.0) {

					for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= getMaiorId(idPEhidrologico, IdVariavelAleatoria()); idVar++) {
						if (getElementoVetor(idPEhidrologico, idVar, AttVetorVariavelAleatoria_tipo_relaxacao, periodo, TipoRelaxacaoVariavelAleatoria()) == TipoRelaxacaoVariavelAleatoria_truncamento_penalizacao) {
							// Variável YPF Truncamento
							const int YPF = getVarDecisao_YPFseExistir(a_TSS, a_idEstagio, periodo_otimizacao, periodo, idPEhidrologico, idVar);
							if (YPF > -1) {
								double penalidade = 0.0;
								for (IdVariavelAleatoriaInterna idVarInt = IdVariavelAleatoriaInterna_1; idVarInt <= getMaiorId(idPEhidrologico, idVar, IdVariavelAleatoriaInterna()); idVarInt++) {
									const IdHidreletrica idUHE = getIdHidreletricaFromIdProcessoEstocasticoIdVariavelAleatoriaIdVariavelAleatoriaInterna(idPEhidrologico, idVar, idVarInt);
									penalidade += a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_afluencia_incremental, double()) * getElementoVetor(idPEhidrologico, idVar, idVarInt, AttVetorVariavelAleatoriaInterna_coeficiente_participacao, periodo, double());
								}
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(YPF, posEquZP, -sobreposicao * penalidade);
							}
						}
					}

					if (!sobreposicao_encontrada)
						sobreposicao_encontrada = true;
				}
				else if (sobreposicao_encontrada)
					break;
			}
		}


	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesCustoPenalidade(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }

}


void ModeloOtimizacao::criarRestricoesCustoPenalidade(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period, const IdPatamarCarga a_idPat) {
	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const int posEquZP = addEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat);

		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZP, 0.0);

		// Variável ZP
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZP(a_TSS, a_idEstagio, a_period, a_idPat), posEquZP, 1.0);


		// Variável PI
		const IdIntercambio idInterIni = a_dados.getMenorId(IdIntercambio());
		const IdIntercambio idInterOut = a_dados.getIdOut(IdIntercambio());

		for (IdIntercambio idIntercambio = idInterIni; idIntercambio < idInterOut; a_dados.vetorIntercambio.incr(idIntercambio)) {

			if (getVarDecisao_PIseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio), posEquZP, -a_dados.getAtributo(idIntercambio, AttComumIntercambio_penalidade_intercambio, double()));

			//PI_FINF
			if (getVarDecisao_PI_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio), posEquZP, -a_dados.getAtributo(idIntercambio, AttComumIntercambio_penalidade_intercambio_minimo, double()));


		}//for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio < a_maiorIdIntercambio; idIntercambio++) {

		// Variáveis Restrição Elétrica
		const IdRestricaoEletrica idREIni = a_dados.getMenorId(IdRestricaoEletrica());
		const IdRestricaoEletrica idREOut = a_dados.getIdOut(IdRestricaoEletrica());

		for (IdRestricaoEletrica idRE = idREIni; idRE < idREOut; a_dados.vetorRestricaoEletrica.incr(idRE)) {

			// Variável RE_FINF
			if (getVarDecisao_RE_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idRE) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RE_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idRE), posEquZP, -a_dados.getAtributo(idRE, AttComumRestricaoEletrica_penalidade_restricao_eletrica, double()));

			// Variável RE_FSUP
			if (getVarDecisao_RE_FSUPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idRE) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RE_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idRE), posEquZP, -a_dados.getAtributo(idRE, AttComumRestricaoEletrica_penalidade_restricao_eletrica, double()));


		} // for (IdRestricaoEletrica idRE = IdRestricaoEletrica_1; idRE < maiorIdRestricaoEletrica; idRE++) {

				// Variáveis Restrição Operativa UHE
		const IdRestricaoOperativaUHE idResOpIni = a_dados.getMenorId(IdRestricaoOperativaUHE());
		const IdRestricaoOperativaUHE idResOpOut = a_dados.getIdOut(IdRestricaoOperativaUHE());
		for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = idResOpIni; idRestricaoOperativaUHE < idResOpOut; a_dados.vetorRestricaoOperativaUHE.incr(idRestricaoOperativaUHE)) {

			//
			// Vazão Afluente
			//

			if (a_idPat == IdPatamarCarga_1 && a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_vazao_afluente) {

				// Variável RHA_FINF
				if (getVarDecisao_RHA_FINFseExistir(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHA_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), posEquZP, -a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_penalidade_restricao_operativa_UHE, double()));

				// Variável RHA_FSUP
				if (getVarDecisao_RHA_FSUPseExistir(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHA_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), posEquZP, -a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_penalidade_restricao_operativa_UHE, double()));

			}//if (a_idPat == IdPatamarCarga_1 && a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_vazao_afluente) {

			//
			// Vazão Defluente
			//

			if (a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_vazao_defluente) {

				// Variável RHQ_FINF
				if (getVarDecisao_RHQ_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHQ_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE), posEquZP, -a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_penalidade_restricao_operativa_UHE, double()));

				// Variável RHQ_FSUP
				if (getVarDecisao_RHQ_FSUPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHQ_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE), posEquZP, -a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_penalidade_restricao_operativa_UHE, double()));

			}//if (a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_vazao_defluente) {

		} // for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE < maiorIdRestricaoOperativaUHE; idRestricaoOperativaUHE++) {



		// Variáveis Hidreletricas
		const IdHidreletrica idUHEIni = a_dados.getMenorId(IdHidreletrica());
		const IdHidreletrica idUHEOut = a_dados.getIdOut(IdHidreletrica());
		for (IdHidreletrica idUHE = idUHEIni; idUHE < idUHEOut; a_dados.vetorHidreletrica.incr(idUHE)) {

			// Variável QT / QTDISP (penaliza a variável que está na FPH)

			if (a_dados.getAtributo(AttComumDados_representar_producao_hidreletrica_com_turbinamento_disponivel, bool())) {

				if (getVarDecisao_QTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posEquZP, -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_turbinamento, double()));

			}//if (!a_dados.getAtributo(AttComumDados_representar_producao_hidreletrica_com_turbinamento_disponivel, bool())) {
			else {

				if (getVarDecisao_QTseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posEquZP, -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_turbinamento, double()));

			}//else {


			// Variável QV
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QV(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posEquZP, -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_vertimento, double()));

			// Variável QD_FINF
			if (getVarDecisao_QD_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posEquZP, -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_vazao_defluente_minima, double()));

			// Variável QD_FSUP
			if (getVarDecisao_QD_FSUPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posEquZP, -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_vazao_defluente_maxima, double()));

			// Variável QD_FINF
			if (getVarDecisao_QD_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posEquZP, -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_vazao_defluente_minima, double()));

			// Variável QD_FSUP
			if (getVarDecisao_QD_FSUPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posEquZP, -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_vazao_defluente_maxima, double()));


			// Variável QR_FINF
			if (getVarDecisao_QR_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, a_dados.getAtributo(idUHE, AttComumHidreletrica_jusante_desvio, IdHidreletrica())) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QR_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, a_dados.getAtributo(idUHE, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posEquZP, -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_vazao_desviada_minima, double()));

			// Variável QI_FINF
			if (getVarDecisao_QI_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posEquZP, -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_vazao_retirada, double()));

			
			// Variável VMORTO_FINF
			if ((getVarDecisao_VMORTO_FINFseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1) && (a_idPat == IdPatamarCarga_1))
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VMORTO_FINF(a_TSS, a_idEstagio, a_period, idUHE), posEquZP, -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_volume_minimo, double()));

			//
			// Produção Hidreletrica
			//

			// Variável PH_FINF
			if (getVarDecisao_PH_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posEquZP, -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_potencia_minima, double()));

			// Variável PHDISP_FINF
			if (getVarDecisao_PHDISP_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posEquZP, -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_potencia_minima, double()));

			// Variável QT_FINF
			if (getVarDecisao_QT_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QT_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posEquZP, -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_turbinamento_minimo, double()));

			// Variável QTDISP_FINF
			if (getVarDecisao_QTDISP_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posEquZP, -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_turbinamento_minimo, double()));


			if (a_dados.getSize1Matriz(idUHE, AttMatrizHidreletrica_potencia_disponivel_meta) > 0) {

				if (a_idPat == IdPatamarCarga_1) {

					if (getVarDecisao_PHDISPMETA_FINFseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISPMETA_FINF(a_TSS, a_idEstagio, a_period, idUHE), posEquZP, -5000);

					if (getVarDecisao_PHDISPMETA_FSUPseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISPMETA_FSUP(a_TSS, a_idEstagio, a_period, idUHE), posEquZP, -5000);


				}//if (a_idPat == IdPatamarCarga_1) {

			}//if (a_dados.getSize1Matriz(idUHE, AttMatrizHidreletrica_potencia_disponivel_meta) > 0) {

			for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idUHE, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

				//
				// Produção Conjunto Hidraulico
				//

				// Variável PH_FINF
				if (getVarDecisao_PH_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico), posEquZP, -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_potencia_minima, double()));

				// Variável PHDISP_FINF
				if (getVarDecisao_PHDISP_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico), posEquZP, -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_potencia_minima, double()));

				// Variável QT_FINF
				if (getVarDecisao_QT_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QT_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico), posEquZP, -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_turbinamento_minimo, double()));

				for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(idUHE, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {

					//
					// Produção Unidade Hidreletrica
					//

					// Variável PH_FINF
					if (getVarDecisao_PH_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE), posEquZP, -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_potencia_minima, double()));

					// Variável PHDISP_FINF
					if (getVarDecisao_PHDISP_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE), posEquZP, -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_potencia_minima, double()));

					// Variável QT_FINF
					if (getVarDecisao_QT_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QT_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE), posEquZP, -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_turbinamento_minimo, double()));

				} // for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(idUHE, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {

			} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idUHE, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

		} // for (IdHidreletrica idUHE = a_dados.getMenorId(IdHidreletrica()); idUHE < maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idUHE)) {



		// Variáveis Termeletricas
		const IdTermeletrica idUTEIni = a_dados.getMenorId(IdTermeletrica());
		const IdTermeletrica idUTEOut = a_dados.getIdOut(IdTermeletrica());
		for (IdTermeletrica idUTE = idUTEIni; idUTE < idUTEOut; a_dados.vetorTermeletrica.incr(idUTE)) {

			const int codigo_usina = a_dados.getAtributo(idUTE, AttComumTermeletrica_codigo_usina, int());

			//
			// Produção Termeletrica
			//

			// Variável PT_FINF
			if (getVarDecisao_PT_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), posEquZP, -a_dados.getAtributo(idUTE, AttComumTermeletrica_penalidade_violacao_potencia, double()));

			// Variável PTDISP_FINF
			if (getVarDecisao_PTDISP_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), posEquZP, -a_dados.getAtributo(idUTE, AttComumTermeletrica_penalidade_violacao_potencia, double()));

			// Variável PTDISP_FSUP
			if (getVarDecisao_PTDISP_FSUPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), posEquZP, -a_dados.getAtributo(idUTE, AttComumTermeletrica_penalidade_violacao_potencia, double()));

			// Variável PTDISPCOM_FINF
			if (getVarDecisao_PTDISPCOM_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISPCOM_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), posEquZP, -a_dados.getAtributo(idUTE, AttComumTermeletrica_penalidade_violacao_comando, double()));

			// Variável PTDISP_FSUP
			if (getVarDecisao_PTDISPCOM_FSUPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISPCOM_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), posEquZP, -a_dados.getAtributo(idUTE, AttComumTermeletrica_penalidade_violacao_comando, double()));

			for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= a_dados.getMaiorId(idUTE, IdUnidadeUTE()); idUnidadeUTE++) {

				//
				// Produção Unidade Termeletrica
				//

				// Variável PT_FINF
				if (getVarDecisao_PT_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posEquZP, -a_dados.getAtributo(idUTE, AttComumTermeletrica_penalidade_violacao_potencia, double()));

			} // for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= a_dados.getMaiorId(idUTE, IdUnidadeUTE()); idUnidadeUTE++) {

		} // for (IdTermeletrica idUTE = a_dados.getMenorId(IdTermeletrica()); idUTE < a_maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idUTE)) {

		//Variáveis Intercambio Hidráulico

		const IdIntercambioHidraulico idInterHIni = a_dados.getMenorId(IdIntercambioHidraulico());
		const IdIntercambioHidraulico idInterHOut = a_dados.getIdOut(IdIntercambioHidraulico());
		for (IdIntercambioHidraulico idIntercambioHidraulico = idInterHIni; idIntercambioHidraulico < idInterHOut; a_dados.vetorIntercambioHidraulico.incr(idIntercambioHidraulico)) {

			// Variável QI_FINF
			if (getVarDecisao_QI_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico, a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico, a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posEquZP, -a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_penalidade_retirada, double()));

			// Variável QILS_TRI_FINF
			if (getVarDecisao_QILS_TRI_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QILS_TRI_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico), posEquZP, -a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_penalidade_retirada, double()));



		}//for (IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico_1; idIntercambioHidraulico < a_maiorIdIntercambioHidraulico; idIntercambioHidraulico++) {

		const IdUsinaElevatoria idUsiElHIni = a_dados.getMenorId(IdUsinaElevatoria());
		const IdUsinaElevatoria idUsiElHOut = a_dados.getIdOut(IdUsinaElevatoria());
		for (IdUsinaElevatoria idUsinaElevatoria = idUsiElHIni; idUsinaElevatoria < idUsiElHOut; a_dados.vetorUsinaElevatoria.incr(idUsinaElevatoria)) {

			// Variável QB_FINF
			if (getVarDecisao_QB_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QB_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria), posEquZP, -a_dados.getAtributo(idUsinaElevatoria, AttComumUsinaElevatoria_penalidade_vazao_bombeada_minima, double()));

			// Variável QBDISP_FINF
			if (getVarDecisao_QBDISP_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria), posEquZP, -a_dados.getAtributo(idUsinaElevatoria, AttComumUsinaElevatoria_penalidade_vazao_bombeada_minima, double()));


		}//for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria < a_maiorIdUsinaElevatoria; idUsinaElevatoria++) {


	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesCustoPenalidade(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }

}


void ModeloOtimizacao::criarRestricoesFuncaoProducaoHidreletrica(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPat, Periodo &a_period, Periodo& a_periodNext, Periodo& a_periodEnd_stage) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;


		const TipoDetalhamentoProducaoHidreletrica tipo_detalhamento_producao = a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica());

		if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina) {

			const int numero_planos = a_dados.getSize2Matriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, a_period);

			if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1) {

				if ((a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_vazao_turbinada_maxima, a_period, double()) > 0) && (a_dados.getElementoMatriz(a_idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima, a_period, a_idPat, double()) > 0) && \
					(a_dados.getElementoMatriz(a_idHidreletrica, AttMatrizHidreletrica_potencia_maxima, a_period, a_idPat, double()) > 0) && (a_dados.getElementoMatriz(a_idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_maxima, a_period, a_idPat, double()) > 0)) {

					for (int i = 1; i <= numero_planos; i++) {

						int posRestricaoPH = -1;

						if (numero_planos == 1)
							posRestricaoPH = addEquLinear_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);
						else {
							posRestricaoPH = addIneLinear_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, i);
						}

						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, a_period, i, double()));


						if (a_dados.getAtributo(AttComumDados_representar_producao_hidreletrica_com_turbinamento_disponivel, bool())) {

							if (a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_FC, a_period, i, double()) == 0.0)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posRestricaoPH, -1.0);
							else
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posRestricaoPH, -1.0 / a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_FC, a_period, i, double()));


						}//if (a_dados.getAtributo(AttComumDados_representar_producao_hidreletrica_com_turbinamento_disponivel, bool())) {
						else {

							if (a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_FC, a_period, i, double()) == 0.0)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posRestricaoPH, -1.0);
							else
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posRestricaoPH, -1.0 / a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_FC, a_period, i, double()));


						}//else {


						if (a_dados.getAtributo(AttComumDados_representar_producao_hidreletrica_com_turbinamento_disponivel, bool()))
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_QH, a_period, i, double()));
						else
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_QH, a_period, i, double()));

						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QV(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_SH, a_period, i, double()));

						if (a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_period, i, double()) > 0.0)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VMED(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_period, i, double()));

					} // for (int i = 1; i <= numero_planos; i++) {

				}//if ((a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_vazao_turbinada_maxima, a_period, double()) > 0) && (a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_vazao_turbinada_disponivel_maxima, a_period, double()) > 0)) {
				else {

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), 0.0);

				}//else {

			} // if (getVarDecisao_PHseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1) {

		} // if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina){	


		else if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) || (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {

			if ((a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_vazao_turbinada_maxima, a_period, double()) > 0) && (a_dados.getElementoMatriz(a_idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima, a_period, a_idPat, double()) > 0)) {

				for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

					if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

						const int numero_planos = a_dados.getSize2Matriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, a_period);

						if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico) > -1) {

							for (int i = 1; i <= numero_planos; i++) {

								int posRestricaoPH = -1;

								if (numero_planos == 1)
									posRestricaoPH = addEquLinear_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico);
								else {
									posRestricaoPH = addIneLinear_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, i);
								}

								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, a_period, i, double()));

								if (a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_FC, a_period, i, double()) == 0.0)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posRestricaoPH, -1.0);
								else
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posRestricaoPH, -1.0 / a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_FC, a_period, i, double()));

								if (a_dados.getAtributo(AttComumDados_representar_producao_hidreletrica_com_turbinamento_disponivel, bool()))
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_QH, a_period, i, double()));
								else
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_QH, a_period, i, double()));

								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QV(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_SH, a_period, i, double()));

								// VAZÃO TURBINADA DOS DEMAIS CONJUNTOS, É CONSIDERADA COMO VERTIMENTO PARA O ATUAL CONJUNTO
								for (IdConjuntoHidraulico idConjunto = IdConjuntoHidraulico_1; idConjunto <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjunto++)
									if (idConjunto != idConjuntoHidraulico)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjunto), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_SH, a_period, i, double()));

								if (a_period < a_periodEnd_stage) {
									if ((getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1) && (getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_periodNext, a_idHidreletrica) > -1)) {
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posRestricaoPH, 0.5 * a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_period, i, double()));
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, a_idHidreletrica), posRestricaoPH, 0.5 * a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_period, i, double()));
									}
									else if ((getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1))
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_period, i, double()));

									else if ((getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_periodNext, a_idHidreletrica) > -1))
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_period, i, double()));
								}

								else if (a_period == a_periodEnd_stage) {
									if ((getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1) && (getVarDecisao_VFseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1)) {
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posRestricaoPH, 0.5 * a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_period, i, double()));
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posRestricaoPH, 0.5 * a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_period, i, double()));
									}
									else if (getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_period, i, double()));

									else if (getVarDecisao_VFseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_period, i, double()));
								}

							} // for (int i = 1; i <= numero_planos; i++) {

						} // if (getVarDecisao_PHseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico) > -1) {

					} // if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {


					else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) {

						for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {

							const int numero_planos = a_dados.getSize2Matriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, a_period);

							if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

								for (int i = 1; i <= numero_planos; i++) {

									int posRestricaoPH = -1;

									if (numero_planos == 1)
										posRestricaoPH = addEquLinear_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE);
									else {
										posRestricaoPH = addIneLinear_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, i);
									}

									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, a_period, i, double()));

									if (a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_FC, a_period, i, double()) == 0.0)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posRestricaoPH, -1.0);
									else
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posRestricaoPH, -1.0 / a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_FC, a_period, i, double()));

									if (a_dados.getAtributo(AttComumDados_representar_producao_hidreletrica_com_turbinamento_disponivel, bool()))
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_QH, a_period, i, double()));
									else
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_QH, a_period, i, double()));

									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QV(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_SH, a_period, i, double()));

									if (a_period < a_periodEnd_stage) {
										if ((getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1) && (getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_periodNext, a_idHidreletrica) > -1)) {
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posRestricaoPH, 0.5 * a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_period, i, double()));
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, a_idHidreletrica), posRestricaoPH, 0.5 * a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_period, i, double()));
										}
										else if ((getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1))
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_period, i, double()));

										else if ((getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_periodNext, a_idHidreletrica) > -1))
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_period, i, double()));
									}

									else if (a_period == a_periodEnd_stage) {
										if ((getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1) && (getVarDecisao_VFseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1)) {
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posRestricaoPH, 0.5 * a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_period, i, double()));
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posRestricaoPH, 0.5 * a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_period, i, double()));
										}
										else if (getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_period, i, double()));

										else if (getVarDecisao_VFseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_period, i, double()));
									}

								} // for (int i = 1; i <= numero_planos; i++) {

							} // if (getVarDecisao_PHseExistir(a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

						} // for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {

					} // else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) {

				} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {

			}//if ((a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_vazao_turbinada_maxima, a_period, double()) > 0) && (a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_vazao_turbinada_disponivel_maxima, a_period, double()) > 0)) {
			else {

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), 0.0);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), 0.0);

			}//else {
					
		} // else if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) || (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {


	}// try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesFuncaoProducaoHidreletrica(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarVariaveisVolume(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodIni_stage, Periodo& a_periodEnd_stage, Periodo& a_periodPrev, Periodo &a_period, Periodo& a_periodNext) {

	try {

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();
		const IdHidreletrica idUHEIni = a_dados.getMenorId(IdHidreletrica());
		const IdHidreletrica idUHEOut = a_dados.getIdOut(IdHidreletrica());

		const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());

		for (IdHidreletrica idUHE = idUHEIni; idUHE < idUHEOut; a_dados.vetorHidreletrica.incr(idUHE)) {

			if (a_dados.getAtributo(idUHE, AttComumHidreletrica_considerar_usina, bool())) {

				const int regularizacao_periodo_estudo = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_regularizacao, a_period, int());

				//ENCHENDO VOLUME MORTO
				if (a_dados.getSizeVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_enchendo_volume_morto) > 0)
					if (a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_enchendo_volume_morto, a_period, int()) == 1)
						addVarDecisao_VMORTO_FINF(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);

				int regularizacao_periodPrev = 0;


				// No primeiro período o VI é considerado para todas as hidrelétricas, visto a possibilidade de regularização no período pré-estudo quando modelos são acoplados.
				if ((a_idEstagio == estagio_inicial) && (a_period == a_periodIni_stage))
					regularizacao_periodPrev = 1;

				else
					regularizacao_periodPrev = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_regularizacao, a_periodPrev, int());

				// Variável Volume Inicial (VI)

				// No período inicial de cada estágio os limites de VI serão atualizados dinamicamente com variáveis de estado.
				//Se a usina tem regularizacao_periodPrev = 1 pega o estado do estágio anterior
				//Se a usina tem regularizacao_periodPrev = 0 faz o VI = 0
				//Se o periodo > a_periodIni_stage, limita com o volume_maximo do a_periodPrev

				if ((a_idEstagio == estagio_inicial) && (a_period == a_periodIni_stage)) {

					if (estagio_inicial == IdEstagio_1)
						addVarDecisao_VI(a_TSS, a_idEstagio, a_period, idUHE, a_dados.getAtributo(idUHE, IdReservatorio_1, AttComumReservatorio_volume_util_inicial, double()), a_dados.getAtributo(idUHE, IdReservatorio_1, AttComumReservatorio_volume_util_inicial, double()), 0.0);
					else {
						const double volume_util_maximo = a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, a_period, double());
						if (volume_util_maximo > 0.0) {
							const double volume_minimo = a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_volume_minimo, a_period, double());
							vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeVarDecisao_VI(a_TSS, a_idEstagio, a_period, idUHE) + "," + getString(volume_minimo) + "," + getString(volume_util_maximo)), addVarDecisao_VI(a_TSS, a_idEstagio, a_period, idUHE, 0.0, 0.0, 0.0), -1);
						}
					}

				} // if ((a_idEstagio == estagio_inicial) && (a_period == a_periodIni_stage)) {

				else if (a_period == a_periodIni_stage && regularizacao_periodPrev) {

					const IdEstagio idEstagioAnterior = IdEstagio(a_idEstagio - 1);

					const Periodo periodo_estudo_final_estagio_anterior = a_dados.getElementosMatriz(AttMatrizDados_percentual_duracao_horizonte_estudo, idEstagioAnterior, Periodo(), double()).getIteradorFinal();
					
					int varVF = -1;
					if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica)
						varVF = getVarDecisao_VFseExistir(a_TSS, idEstagioAnterior, periodo_estudo_final_estagio_anterior, idUHE);

					const double volume_util_maximo = a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, a_period, double());
					const double volume_minimo = a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_volume_minimo, a_period, double());
					
					vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeVarDecisao_VI(a_TSS, a_idEstagio, a_period, idUHE) + "," + getString(volume_minimo) + "," + getString(volume_util_maximo)), addVarDecisao_VI(a_TSS, a_idEstagio, a_period, idUHE, 0.0, 0.0, 0.0), varVF);
					
				} // else if (a_period == a_periodIni_stage) {

				else if (a_period == a_periodIni_stage && !regularizacao_periodPrev)
					addVarDecisao_VI(a_TSS, a_idEstagio, a_period, idUHE, 0.0, 0.0, 0.0);

				if (a_periodNext <= a_periodEnd_stage)
					addVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, idUHE, 0.0, a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, a_period, double()), 0.0);
				
				else if (a_period == a_periodEnd_stage) {

					const int varDecisao_VF = addVarDecisao_VF(a_TSS, a_idEstagio, a_period, idUHE, 0.0, a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, a_period, double()), 0.0);

					if ((!regularizacao_periodo_estudo) || (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)) // Se a usina nao tem regularizaçao entao zera o limite final
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(varDecisao_VF, 0.0);

				} // if ((regularizacao_periodo_estudo) && (a_period == a_periodEnd_stage)) {

				const int varVI      = getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_period, idUHE);
				const int varVI_next = getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_periodNext, idUHE);
				const int varVF      = getVarDecisao_VFseExistir(a_TSS, a_idEstagio, a_period, idUHE);

				// Vaiável Volume Médio (VM)
				if ((varVI > -1) || (varVI_next > -1) || (varVF > -1)) {

					int var_VMED = getVarDecisao_VMEDseExistir(a_TSS, a_idEstagio, a_period, idUHE);
					int equ_VMED = getEquLinear_VMEDseExistir(a_TSS, a_idEstagio, a_period, idUHE);

					if (var_VMED == -1) {
						var_VMED = addVarDecisao_VMED(a_TSS, a_idEstagio, a_period, idUHE, 0.0, vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);
						equ_VMED = addEquLinear_VMED(a_TSS, a_idEstagio, a_period, idUHE);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(var_VMED, equ_VMED, 1.0);
					}

					if (varVI > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varVI, equ_VMED, -0.5);

					if (varVI_next > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varVI_next, equ_VMED, -0.5);

					else if (varVF > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varVF, equ_VMED, -0.5);

				}

				if ((a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_volume_util_minimo, a_period, double()) > 0.0) && (getVarDecisao_VFseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1))
					addVarDecisao_VF_FINF(a_TSS, a_idEstagio, a_period, idUHE, 0.0, a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_volume_util_minimo, a_period, double()), 0.0);

			} // if (a_dados.getAtributo(idUHE, AttComumHidreletrica_considerar_usina, bool())){

			if (a_dados.getSize1Matriz(idUHE, IdReservatorio_1, AttMatrizReservatorio_volume_meta) > 0) {

				if (a_dados.getElementoMatriz(idUHE, IdReservatorio_1, AttMatrizReservatorio_volume_meta, IdCenario_1, a_period, double()) != getdoubleFromChar("max")) {

					IdVariavelEstado idVariavelEstado = IdVariavelEstado_Nenhum;

					if (a_period == a_periodEnd_stage) {

						const int varDecisao_VF = getVarDecisao_VF(a_TSS, a_idEstagio, a_period, idUHE);
						idVariavelEstado = vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, getNomeVarDecisao_VF(a_TSS, a_idEstagio, a_period, idUHE), varDecisao_VF, -1);

					}//if (a_period == a_periodEnd_stage) {
					else {
						//Pega o VI do periodo_seguinte

						const int varDecisao_VI = getVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, idUHE);
						idVariavelEstado = vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, getNomeVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, idUHE), varDecisao_VI, -1);

					}//else {

					const IdCenario menor_cenario = IdCenario_1;
					const IdCenario maior_cenario = arranjoResolucao.getAtributo(AttComumArranjoResolucao_maior_cenario, IdCenario());

					vetorEstagio.at(a_idEstagio).addValorVariavelEstado(idVariavelEstado, menor_cenario, menor_cenario, maior_cenario, 0.0);
					for (IdCenario idCenario = menor_cenario; idCenario <= maior_cenario; idCenario++)
						vetorEstagio.at(a_idEstagio).addValorVariavelEstado(idVariavelEstado, idCenario, IdCenario_Nenhum, IdCenario_Nenhum, a_dados.getElementoMatriz(idUHE, IdReservatorio_1, AttMatrizReservatorio_volume_meta, idCenario, a_period, double()));

				}//if (a_dados.getElementoMatriz(idUHE, IdReservatorio_1, AttMatrizReservatorio_volume_meta, IdCenario_1, a_period, double()) != getdoubleFromChar("min")) {

			} // if (a_dados.getSize1Matriz(idUHE, IdReservatorio_1, AttMatrizReservatorio_volume_meta) > 0) {

		} // for (IdHidreletrica idUHE = a_dados.getMenorId(IdHidreletrica()); idUHE < idUHEOut; a_dados.vetorHidreletrica.incr(idUHE)) {

	}// try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisVolume(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarVariaveisCusto(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;


		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

			// Custo Operacao Valor Presente (ZO0p) Por PeriodoEstudo
			addVarDecisao_ZO0(a_TSS, a_idEstagio, a_period, 0.0, infinito, 0.0);


			// Custo Operacao (ZOp) Por PeriodoEstudo	
			addVarDecisao_ZO(a_TSS, a_idEstagio, a_period, 0.0, infinito, 0.0);


			// Custo Penalidade Valor Presente (ZP0p) Por PeriodoEstudo
			addVarDecisao_ZP0(a_TSS, a_idEstagio, a_period, 0.0, infinito, 0.0);


			// CUSTO DE PENALIDADE (ZP) POR PeriodoEstudo (contabiliza as penalidades das variáveis que não dependem do patamar de carga, ex, restrições de volume, evaporação)
			addVarDecisao_ZP(a_TSS, a_idEstagio, a_period, 0.0, infinito, 0.0);

	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisCusto(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarVariaveisCusto(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;


		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		// CUSTO OPERAÇÃO (ZO) POR PeriodoEstudo PorPatamarCarga
		addVarDecisao_ZO(a_TSS, a_idEstagio, a_period, a_idPat, 0.0, infinito, 0.0);

		// CUSTO DE PENALIDADE (ZP) POR PeriodoEstudo PorPatamarCarga
		addVarDecisao_ZP(a_TSS, a_idEstagio, a_period, a_idPat, 0.0, infinito, 0.0);

	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisCusto(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}

void ModeloOtimizacao::criarVariaveisHidraulicas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period, const IdPatamarCarga a_idPat) {

	try {

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const IdHidreletrica idUHEIni = a_dados.getMenorId(IdHidreletrica());
		const IdHidreletrica idUHEOut = a_dados.getIdOut(IdHidreletrica());
		for (IdHidreletrica idUHE = idUHEIni; idUHE < idUHEOut; a_dados.vetorHidreletrica.incr(idUHE)) {

			if (a_dados.getAtributo(idUHE, AttComumHidreletrica_considerar_usina, bool())) {


				if (a_idPat == IdPatamarCarga_1) {

					if (a_dados.getSize1Matriz(idUHE, AttMatrizHidreletrica_vazao_evaporada_meta) > 0) {

						//Evaporacao_meta

						// EVAPORAÇÃO RELAXADA(QE_REL)
						const int varQE_REL = addVarDecisao_QE_REL(a_TSS, a_idEstagio, a_period, idUHE, -infinito, infinito, 0.0);

						if (a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_vazao_evaporada_meta, IdCenario_1, a_period, double()) != getdoubleFromChar("max")) {

							const IdVariavelEstado idVariavelEstado = vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, getNomeVarDecisao_QE_REL(a_TSS, a_idEstagio, a_period, idUHE), varQE_REL, -1);

							const IdProcesso idProcesso = a_dados.arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());
							const IdCenario menor_cenario = a_dados.arranjoResolucao.getAtributo(idProcesso, AttComumProcesso_menor_cenario, IdCenario());
							const IdCenario maior_cenario = a_dados.arranjoResolucao.getAtributo(idProcesso, AttComumProcesso_maior_cenario, IdCenario());

							vetorEstagio.at(a_idEstagio).addValorVariavelEstado(idVariavelEstado, menor_cenario, menor_cenario, maior_cenario, 0.0);
							for (IdCenario idCenario = menor_cenario; idCenario <= maior_cenario; idCenario++)
								vetorEstagio.at(a_idEstagio).addValorVariavelEstado(idVariavelEstado, idCenario, IdCenario_Nenhum, IdCenario_Nenhum, a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_vazao_evaporada_meta, idCenario, a_period, double()));

						} // if (a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_vazao_evaporada_meta, IdCenario_1, a_period, double()) != getdoubleFromChar("max")) {

					} // if (a_dados.getSize1Matriz(idUHE, AttMatrizHidreletrica_vazao_evaporada_meta > 0)) {
					else {

						if ((a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_coef_linear_evaporacao_0, a_period, double()) != 0) || (a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_coef_linear_evaporacao_1, a_period, double()) != 0)) {

							// EVAPORAÇÃO (QE)
							addVarDecisao_QE(a_TSS, a_idEstagio, a_period, idUHE, -infinito, infinito, 0.0);

							// EVAPORAÇÃO RELAXADA(QE_REL)
							const int varQE_REL = addVarDecisao_QE_REL(a_TSS, a_idEstagio, a_period, idUHE, -infinito, infinito, 0.0);

							// CRIA VARIAVEIS DE FOLGA APENAS PARA RETIRADA DE AGUA 
							if ((a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_coef_linear_evaporacao_0, a_period, double()) > 0))
								addVarDecisao_QE_FINF(a_TSS, a_idEstagio, a_period, idUHE, 0.0, 0.0, 0.0);

						}//if ((a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_coef_linear_evaporacao_0, a_period, double()) != 0) || (a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_coef_linear_evaporacao_1, a_period, double()) != 0)) {

					}//else {

					if (a_dados.getSize1Matriz(idUHE, AttMatrizHidreletrica_vazao_vertida_meta)) {

						if (a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_vazao_vertida_meta, IdCenario_1, a_period, double()) != getdoubleFromChar("max")) {

							int varQV = getVarDecisao_QVseExistir(a_TSS, a_idEstagio, a_period, idUHE);
							if (varQV < 0)
								varQV = addVarDecisao_QV(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);

							if (getEquLinear_QVseExistir(a_TSS, a_idEstagio, a_period, idUHE) == -1) {
								const int posEquQV = addEquLinear_QV(a_TSS, a_idEstagio, a_period, idUHE);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQV, 0.0);
								// Variável QV Media
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QV(a_TSS, a_idEstagio, a_period, idUHE), posEquQV, 1.0);
							}

							const IdVariavelEstado idVariavelEstado = vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, getNomeVarDecisao_QV(a_TSS, a_idEstagio, a_period, idUHE), varQV, -1);

							const IdProcesso idProcesso = a_dados.arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());
							const IdCenario menor_cenario = a_dados.arranjoResolucao.getAtributo(idProcesso, AttComumProcesso_menor_cenario, IdCenario());
							const IdCenario maior_cenario = a_dados.arranjoResolucao.getAtributo(idProcesso, AttComumProcesso_maior_cenario, IdCenario());

							vetorEstagio.at(a_idEstagio).addValorVariavelEstado(idVariavelEstado, menor_cenario, menor_cenario, maior_cenario, 0.0);
							for (IdCenario idCenario = menor_cenario; idCenario <= maior_cenario; idCenario++)
								vetorEstagio.at(a_idEstagio).addValorVariavelEstado(idVariavelEstado, idCenario, IdCenario_Nenhum, IdCenario_Nenhum, a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_vazao_vertida_meta, idCenario, a_period, double()));

						} // if (a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_vazao_vertida_meta, IdCenario_1, a_period, double()) != getdoubleFromChar("max")) {

					} // if (a_dados.getSize1Matriz(idUHE, AttMatrizHidreletrica_vazao_vertida_meta)) {


					// Defluencia Hidrelétrica Folga Minima (QD_FINF)
					if (a_dados.getSizeVetor(idUHE, AttVetorHidreletrica_vazao_defluente_minima) > 0.0) {
						if (a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_defluente_minima, a_period, double()) > 0.0) {
							if (getVarDecisao_QD_FINF(a_TSS, a_idEstagio, a_period, idUHE) < 0)
								addVarDecisao_QD_FINF(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);

							if (getVarDecisao_QDseExistir(a_TSS, a_idEstagio, a_period, idUHE) < 0)
								addVarDecisao_QD(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);

							if (getEquLinear_QDseExistir(a_TSS, a_idEstagio, a_period, idUHE) == -1) {
								const int posEquQD = addEquLinear_QD(a_TSS, a_idEstagio, a_period, idUHE);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQD, 0.0);
								// Variável QD Media
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD(a_TSS, a_idEstagio, a_period, idUHE), posEquQD, 1.0);
							}
						}
					}

					// Defluencia Hidrelétrica Folga Maxima (QD_FSUP)
					if (a_dados.getSizeVetor(idUHE, AttVetorHidreletrica_vazao_defluente_maxima) > 0.0) {
						if ((a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_defluente_maxima, a_period, double()) > 0.0) && (a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_defluente_maxima, a_period, double()) != getdoubleFromChar("max"))) {
							if (getVarDecisao_QD_FSUP(a_TSS, a_idEstagio, a_period, idUHE) < 0)
								addVarDecisao_QD_FSUP(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);

							if (getVarDecisao_QDseExistir(a_TSS, a_idEstagio, a_period, idUHE) < 0)
								addVarDecisao_QD(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);

							if (getEquLinear_QDseExistir(a_TSS, a_idEstagio, a_period, idUHE) == -1) {
								const int posEquQD = addEquLinear_QD(a_TSS, a_idEstagio, a_period, idUHE);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQD, 0.0);
								// Variável QD Media
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD(a_TSS, a_idEstagio, a_period, idUHE), posEquQD, 1.0);
							}
						}
					}

				}//if (a_idPat == IdPatamarCarga_1) {

				// Vazão Montante Hidreletrica (QM)
				addVarDecisao_QM(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, -infinito, infinito, 0.0);

				// Vertimento Hidreletrica (QV)
				addVarDecisao_QV(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, infinito, 0.0);
				if (getEquLinear_QVseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QV(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), getEquLinear_QV(a_TSS, a_idEstagio, a_period, idUHE), -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, a_idPat, double()));

				// Vazão defluente Hidreletrica por patamar (QD)
				// Patamares maiores que 1 podem ser criados no método recursivo que cria QDLAG
				int varQD = -1;
				if (a_dados.getAtributo(idUHE, AttComumHidreletrica_tempo_viagem_agua, int()) > 0)
					varQD = getVarDecisao_QDseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE);

				if (varQD < 0)
					addVarDecisao_QD(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, infinito, 0.0);
				if (getEquLinear_QDseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), getEquLinear_QD(a_TSS, a_idEstagio, a_period, idUHE), -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, a_idPat, double()));

				// Defluencia Hidrelétrica Folga Minima (QD_FINF)
				if (a_dados.getSize1Matriz(idUHE, AttMatrizHidreletrica_vazao_defluente_minima) > 0.0) {
					if (a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_vazao_defluente_minima, a_period, a_idPat, double()) > 0.0)
						addVarDecisao_QD_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, infinito, 0.0);
				}

				// Defluencia Hidrelétrica Folga Maxima (QD_FSUP)
				if (a_dados.getSize1Matriz(idUHE, AttMatrizHidreletrica_vazao_defluente_maxima) > 0.0) {
					if ((a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_vazao_defluente_maxima, a_period, a_idPat, double()) > 0.0) && (a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_vazao_defluente_maxima, a_period, a_idPat, double()) != getdoubleFromChar("max")))
						addVarDecisao_QD_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, infinito, 0.0);
				}

				if (a_dados.getAtributo(idUHE, AttComumHidreletrica_jusante_desvio, IdHidreletrica()) != IdHidreletrica_Nenhum) {

					// Vazão desviada Hidreletrica por patamar (QR)
					addVarDecisao_QR(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, a_dados.getAtributo(idUHE, AttComumHidreletrica_jusante_desvio, IdHidreletrica()), 0.0, a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_vazao_desviada_maxima, a_period, a_idPat, double()), 0.0);

					// Desvio Hidrelétrica Folga Minima (QR_FINF)
					if (a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_vazao_desviada_minima, a_period, a_idPat, double()) > 0.0)
						addVarDecisao_QR_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, a_dados.getAtributo(idUHE, AttComumHidreletrica_jusante_desvio, IdHidreletrica()), 0.0, infinito, 0.0);

				}//if (a_dados.getAtributo(idUHE, AttComumHidreletrica_jusante_desvio, IdHidreletrica()) != IdHidreletrica_Nenhum) {


				// Vazão retirada Hidreletrica por patamar (QI)

				// Vazão retirada Hidrelétrica Folga Minima (QI_FINF)
				if (a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_retirada, a_period, double()) > 0.0) {
					addVarDecisao_QI(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_retirada, a_period, double()), 0.0);
					addVarDecisao_QI_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, infinito, 0.0);
				}//if (a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_retirada, a_period, double()) > 0.0) {
				else if(a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_retirada, a_period, double()) < 0.0)
					addVarDecisao_QI(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_retirada, a_period, double()), a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_retirada, a_period, double()), 0.0);


				const TipoDetalhamentoProducaoHidreletrica tipo_detalhamento_producao_hidreletrica = a_dados.getAtributo(idUHE, AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica());

				bool disponivel = false;
				bool todo_membro_disponivel_possui_potencia_minima = true;
				bool todo_membro_disponivel_possui_vazao_turbinada_minima = true;

				// ------------------------------------------------------------------------------------------------------------------------------------------------------
				//
				//                                                                    Produção Por Hidrelétrica
				//
				// ------------------------------------------------------------------------------------------------------------------------------------------------------

				if (tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_usina) {

					if (a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_disponibilidade, a_period, double()) > 0.0) {

						disponivel = true;

						if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

							//
							// Potencia Hidreletrica
							//

							// Potencia Hidreletrica (PH)
							addVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_maxima, a_period, a_idPat, double()), 0.0);

							if (a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_minima, a_period, a_idPat, double()) > 0.0) {

								// Potencia Hidreletrica Relaxada (PH_REL)
								addVarDecisao_PH_REL(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_minima, a_period, a_idPat, double()), infinito, 0.0);

								// Potencia Hidreletrica Folga Minima (PH_FINF)
								addVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, infinito, 0.0);

							} // if (a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_minima, a_period, a_idPat, double()) > 0.0) {

							else
								todo_membro_disponivel_possui_potencia_minima = false;

							addVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_disponibilidade, a_period, double()) * a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_maxima, a_period, a_idPat, double()), 0.0);

						} // if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

						//
						// Turbinamento Hidreletrico
						//

						// Turbinamento Hidreletrico (QT)
						addVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_turbinada_maxima, a_period, double()), 0.0);

						if (a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_turbinada_minima, a_period, double()) > 0.0) {
							// Turbinamento Hidreletrico Folga Minima (QT_FINF)
							addVarDecisao_QT_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, infinito, 0.0);
						} // if (a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_turbinada_minima, a_period, double()) > 0.0) {

						else
							todo_membro_disponivel_possui_vazao_turbinada_minima = false;

						// Turbinamento Hidreletrico Disponivel (QTDISP)

						addVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima, a_period, a_idPat, double()), 0.0);

						if (a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_vazao_turbinada_disponivel_minima, a_period, a_idPat, double()) > 0.0) {
							// Turbinamento Hidreletrico Folga Minima (QTDISP_FINF)
							addVarDecisao_QTDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, infinito, 0.0);
						} // if (a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_turbinada_disponivel_minima, a_period, double()) > 0.0) {


					} // if (a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_disponibilidade, a_period, double()) > 0.0) {

				} // if (tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_usina) {


					// ------------------------------------------------------------------------------------------------------------------------------------------------------
					//
					//                                                                    Produção Por Conjunto Hidráulico
					//
					// ------------------------------------------------------------------------------------------------------------------------------------------------------

				if (tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

					for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idUHE, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

						if (a_dados.getElementoVetor(idUHE, idConjuntoHidraulico, AttVetorConjuntoHidraulico_disponibilidade, a_period, double()) > 0.0) {

							disponivel = true;

							if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

								//
								// Potencia ConjuntoHidraulico
								//

								// Potencia ConjuntoHidraulico (PH)
								addVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, 0.0, a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_maxima, a_period, a_idPat, double()), 0.0);

								if (a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima, a_period, a_idPat, double()) > 0.0) {

									// Potencia ConjuntoHidraulico Relaxada (PH_REL)
									addVarDecisao_PH_REL(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima, a_period, a_idPat, double()), infinito, 0.0);

									// Potencia ConjuntoHidraulico Folga Minima (PH_FINF)
									addVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, 0.0, infinito, 0.0);

								} // if (a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima, a_period, a_idPat, double()) > 0.0) {

								else
									todo_membro_disponivel_possui_potencia_minima = false;

								// Potencia UnidadeUHE Disponivel (PHDISP)
								addVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, 0.0, a_dados.getElementoVetor(idUHE, idConjuntoHidraulico, AttVetorConjuntoHidraulico_disponibilidade, a_period, double()) * a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_maxima, a_period, a_idPat, double()), 0.0);

							} // if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

							//
							// Turbinamento ConjuntoHidraulico
							//

							// Turbinamento ConjuntoHidraulico (QT)
							addVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, 0.0, a_dados.getElementoVetor(idUHE, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_maxima, a_period, double()), 0.0);

							if (a_dados.getElementoVetor(idUHE, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_minima, a_period, double()) > 0.0) {
								// Turbinamento ConjuntoHidraulico Folga Minima (QT_FINF)
								addVarDecisao_QT_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, 0.0, infinito, 0.0);
							} // if (a_dados.getElementoVetor(idUHE, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_minima, a_period, double()) > 0.0) {

							else
								todo_membro_disponivel_possui_vazao_turbinada_minima = false;

							// Turbinamento Hidreletrico Disponivel (QTDISP)
							addVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, 0.0, infinito, 0.0);

						} // if (a_dados.getElementoVetor(idUHE, idConjuntoHidraulico, AttVetorConjuntoHidraulico_disponibilidade, a_period, double()) > 0.0) {

					} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idUHE, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

				} // if (tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {


				// ------------------------------------------------------------------------------------------------------------------------------------------------------
				//
				//                                                                    Produção Unidade Hidrelétrica
				//
				// ------------------------------------------------------------------------------------------------------------------------------------------------------

				else if (tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_unidade) {

					for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idUHE, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

						for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(idUHE, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {

							if (a_dados.getElementoVetor(idUHE, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_disponibilidade, a_period, double()) > 0.0) {

								disponivel = true;

								if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

									//
									// Potencia UnidadeUHE
									//

									// Potencia UnidadeUHE (PH)
									addVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE, 0.0, a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_maxima, a_period, a_idPat, double()), 0.0);

									if (a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_minima, a_period, a_idPat, double()) > 0.0) {

										// Potencia UnidadeUHE Relaxada (PH_REL)
										addVarDecisao_PH_REL(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE, a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_minima, a_period, a_idPat, double()), infinito, 0.0);

										// Potencia UnidadeUHE Folga Minima (PH_FINF)
										addVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE, 0.0, infinito, 0.0);

									} // if (a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_minima, a_period, a_idPat, double()) > 0.0) {

									else
										todo_membro_disponivel_possui_potencia_minima = false;

									// Potencia UnidadeUHE Disponivel (PHDISP)
									addVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE, 0.0, a_dados.getElementoVetor(idUHE, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_disponibilidade, a_period, double()) * a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_maxima, a_period, a_idPat, double()), 0.0);

									if (a_dados.getSize1Matriz(idUHE, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_minima) > 0) {
										if ((a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_minima, a_period, a_idPat, double()) > 0.0) && (getVarDecisao_PH_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE) == -1)) {

											// Potencia UnidadeUHE Disponivel Relaxada (PHDISP_REL)
											addVarDecisao_PHDISP_REL(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE, a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_minima, a_period, a_idPat, double()), infinito, 0.0);

											// Potencia UnidadeUHE Disponivel Folga Minima (PHDISP_FINF)
											addVarDecisao_PHDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE, 0.0, infinito, 0.0);

										} // if ((a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_minima, a_period, a_idPat, double()) > 0.0) && (getVarDecisao_PH_FINFseExistir(a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE) == -1)) {

										else if ((a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_minima, a_period, a_idPat, double()) > 0.0) && (getVarDecisao_PH_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE) > -1))
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE), a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_minima, a_period, a_idPat, double()));

									} // if (a_dados.getSize1Matriz(idUHE, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_minima) > 0) {

									if (a_dados.getSize1Matriz(idUHE, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_maxima) > 0)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE), a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_maxima, a_period, a_idPat, double()));

								}

								//
								// Turbinamento UnidadeUHE
								//

								// Turbinamento UnidadeUHE (QT)
								addVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE, 0.0, a_dados.getElementoVetor(idUHE, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_vazao_turbinada_maxima, a_period, double()), 0.0);

								if (a_dados.getElementoVetor(idUHE, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_vazao_turbinada_minima, a_period, double()) > 0.0) {
									// Turbinamento UnidadeUHE Folga Minima (QT_FINF)
									addVarDecisao_QT_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE, 0.0, infinito, 0.0);
								} // if (a_dados.getElementoVetor(idUHE, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_vazao_turbinada_minima, a_period, double()) > 0.0) {

								else
									todo_membro_disponivel_possui_vazao_turbinada_minima = false;

								// Turbinamento Hidreletrico Disponivel (QTDISP)
								addVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, idUnidadeUHE, 0.0, infinito, 0.0);

							} // if (a_dados.getElementoVetor(idUHE, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_disponibilidade, a_period, double()) > 0.0) {

						} // for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= maiorIdUnidadeUHE; idUnidadeUHE++) {
					} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {

				} // else if (tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_unidade) {


				if (disponivel) {

					if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

						//
						// Potencia Hidreletrica (PH)
						//

						if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) == -1) {

							addVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, infinito, 0.0);

							if (a_dados.getSize1Matriz(idUHE, AttMatrizHidreletrica_potencia_minima) > 0) {

								if ((a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_minima, a_period, a_idPat, double()) > 0.0) && (todo_membro_disponivel_possui_potencia_minima))
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_minima, a_period, a_idPat, double()));

								else if ((a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_minima, a_period, a_idPat, double()) > 0.0) && (!todo_membro_disponivel_possui_potencia_minima)) {

									// Potencia Hidreletrica Relaxada (PH_REL)
									addVarDecisao_PH_REL(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_minima, a_period, a_idPat, double()), infinito, 0.0);

									// Potencia Hidreletrica Folga Minima (PH_FINF)
									addVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, infinito, 0.0);

								} // else if ((a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_minima, a_period, a_idPat, double()) > 0.0) && (!todo_membro_disponivel_possui_potencia_minima)) {
							} // if (a_dados.getSize1Matriz(idUHE, AttMatrizHidreletrica_potencia_minima) > 0) {

							if (a_dados.getSize1Matriz(idUHE, AttMatrizHidreletrica_potencia_maxima) > 0)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_maxima, a_period, a_idPat, double()));

						} // if (getVarDecisao_PHseExistir(a_idEstagio, a_period, a_idPat, idUHE) == -1) {

						//
						// Potencia Hidreletrica Disponivel (PHDISP)
						//

						if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) == -1)
							addVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, infinito, 0.0);

						if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, idUHE) == -1) {

							if (a_dados.getSize1Matriz(idUHE, AttMatrizHidreletrica_potencia_disponivel_meta) > 0) {

								if (a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_disponivel_meta, IdCenario_1, a_period, double()) != getdoubleFromChar("max")) {

									int varPHDISP = getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, idUHE);

									if (varPHDISP < 0)
										varPHDISP = addVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);

									const int varPHDISPMETA = addVarDecisao_PHDISPMETA(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);
									const int varPHDISPMETA_FINF = addVarDecisao_PHDISPMETA_FINF(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);
									const int varPHDISPMETA_FSUP = addVarDecisao_PHDISPMETA_FSUP(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);

									const IdVariavelEstado idVariavelEstado = vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, getNomeVarDecisao_PHDISPMETA(a_TSS, a_idEstagio, a_period, idUHE), varPHDISPMETA, -1);

									const IdProcesso idProcesso = a_dados.arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());
									const IdCenario menor_cenario = a_dados.arranjoResolucao.getAtributo(idProcesso, AttComumProcesso_menor_cenario, IdCenario());
									const IdCenario maior_cenario = a_dados.arranjoResolucao.getAtributo(idProcesso, AttComumProcesso_maior_cenario, IdCenario());

									vetorEstagio.at(a_idEstagio).addValorVariavelEstado(idVariavelEstado, menor_cenario, menor_cenario, maior_cenario, 0.0);
									for (IdCenario idCenario = menor_cenario; idCenario <= maior_cenario; idCenario++)
										vetorEstagio.at(a_idEstagio).addValorVariavelEstado(idVariavelEstado, idCenario, IdCenario_Nenhum, IdCenario_Nenhum, a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_disponivel_meta, idCenario, a_period, double()));

									//Adiciona Restriçao PHDISP = PHDISPMETA + PHDISPMETA_FSUP - PHDISPMETA_FINF
									const int posEquPHDISP = addEquLinear_PHDISPMETA(a_TSS, a_idEstagio, a_period, idUHE);
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPHDISP, 0.0);

									//PHDISP
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP, posEquPHDISP, 1.0);

									//PHDISPMETA
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISPMETA, posEquPHDISP, -1.0);

									//PHDISPMETA_FSUP
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISPMETA_FSUP, posEquPHDISP, -1.0);

									//PHDISPMETA_FINF
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISPMETA_FINF, posEquPHDISP, 1.0);

								} // if (a_dados.getSize1Matriz(idUHE, AttMatrizHidreletrica_potencia_disponivel_meta)) {

							} // if (a_dados.getSize1Matriz(idUHE, AttMatrizHidreletrica_potencia_disponivel_meta) > 0) {
						} // if (getVarDecisao_PHDISPseExistir(a_idEstagio, a_period, idUHE) == -1) {

						if (a_dados.getSize1Matriz(idUHE, AttMatrizHidreletrica_potencia_disponivel_minima) > 0) {

							if (a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_disponivel_minima, a_period, a_idPat, double()) > a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_minima, a_period, a_idPat, double()) * a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_disponibilidade, a_period, double())) {

								if (todo_membro_disponivel_possui_potencia_minima)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_disponivel_minima, a_period, a_idPat, double()));

								else if (!todo_membro_disponivel_possui_potencia_minima) {

									// Potencia Hidreletrica Disponivel Relaxada (PHDISP_REL)
									addVarDecisao_PHDISP_REL(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_disponivel_minima, a_period, a_idPat, double()), infinito, 0.0);

									// Potencia Hidreletrica Disponivel Folga Minima (PHDISP_FINF)
									addVarDecisao_PHDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, infinito, 0.0);

								} // else if ((a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_disponivel_minima, a_period, a_idPat, double()) > 0.0) && (!todo_membro_disponivel_possui_potencia_minima)) {
							}
						} // if (a_dados.getSize1Matriz(idUHE, AttMatrizHidreletrica_potencia_disponivel_minima) > 0) {

						if (a_dados.getSize1Matriz(idUHE, AttMatrizHidreletrica_potencia_disponivel_maxima) > 0) {
							if (a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_disponivel_maxima, a_period, a_idPat, double()) < a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_maxima, a_period, a_idPat, double()) * a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_disponibilidade, a_period, double()))
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_disponivel_maxima, a_period, a_idPat, double()));
						}

					} // if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

					//
					// Turbinamento Hidreletrico
					//

					// Turbinamento Hidreletrico (QT)

					if (getVarDecisao_QTseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) == -1) {

						addVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, infinito, 0.0);

						if (a_dados.getSizeVetor(idUHE, AttVetorHidreletrica_vazao_turbinada_minima) > 0) {

							if ((a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_turbinada_minima, a_period, double()) > 0.0) && (todo_membro_disponivel_possui_vazao_turbinada_minima))
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_turbinada_minima, a_period, double()));

							else if ((a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_turbinada_minima, a_period, double()) > 0.0) && (!todo_membro_disponivel_possui_vazao_turbinada_minima)) {

								// Turbinamento Hidreletrico Folga Minima (QT_FINF)
								addVarDecisao_QT_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, infinito, 0.0);

							} // else if ((a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_turbinada_minima, a_period, double()) > 0.0) && (!todo_membro_disponivel_possui_vazao_turbinada_minima)) {
						} // if (a_dados.getSizeVetor(idUHE, AttVetorHidreletrica_vazao_turbinada_minima) > 0) {

						if (a_dados.getSizeVetor(idUHE, AttVetorHidreletrica_vazao_turbinada_maxima) > 0)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_turbinada_maxima, a_period, double()));

					} // if (getVarDecisao_QTseExistir(a_idEstagio, a_period, a_idPat, idUHE) == -1) {

					// Turbinamento Hidreletrico Disponivel (QTDISP)
					if (getVarDecisao_QTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) == -1)
						addVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, infinito, 0.0);

					if (getVarDecisao_QTDISPseExistir(a_TSS, a_idEstagio, a_period, idUHE) == -1) {

						if (a_dados.getSize1Matriz(idUHE, AttMatrizHidreletrica_vazao_turbinada_disponivel_meta) > 0) {

							if (a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_vazao_turbinada_disponivel_meta, IdCenario_1, a_period, double()) != getdoubleFromChar("max")) {

								int varQTDISP = getVarDecisao_QTDISPseExistir(a_TSS, a_idEstagio, a_period, idUHE);

								if (varQTDISP < 0)
									varQTDISP = addVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);

								const IdVariavelEstado idVariavelEstado = vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, getNomeVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, idUHE), varQTDISP, -1);

								const IdProcesso idProcesso = a_dados.arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());
								const IdCenario menor_cenario = a_dados.arranjoResolucao.getAtributo(idProcesso, AttComumProcesso_menor_cenario, IdCenario());
								const IdCenario maior_cenario = a_dados.arranjoResolucao.getAtributo(idProcesso, AttComumProcesso_maior_cenario, IdCenario());

								vetorEstagio.at(a_idEstagio).addValorVariavelEstado(idVariavelEstado, menor_cenario, menor_cenario, maior_cenario, 0.0);
								for (IdCenario idCenario = menor_cenario; idCenario <= maior_cenario; idCenario++)
									vetorEstagio.at(a_idEstagio).addValorVariavelEstado(idVariavelEstado, idCenario, IdCenario_Nenhum, IdCenario_Nenhum, a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_vazao_turbinada_disponivel_meta, idCenario, a_period, double()));

							} // if (a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_vazao_turbinada_disponivel_meta, IdCenario_1, a_period, double()) != getdoubleFromChar("max")) {

						} // if (a_dados.getSize1Matriz(idUHE, AttMatrizHidreletrica_vazao_turbinada_disponivel_meta) > 0) {

					} // if (getVarDecisao_QTDISPseExistir(a_idEstagio, a_period, idUHE) == -1) {

					if ((tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_conjunto) || (tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {

						for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idUHE, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

							if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

								//
								// Potencia ConjuntoHidraulico (PH)
								//

								if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico) == -1) {

									addVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, 0.0, infinito, 0.0);

									if (a_dados.getSize1Matriz(idUHE, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima) > 0) {

										if ((a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima, a_period, a_idPat, double()) > 0.0) && (todo_membro_disponivel_possui_potencia_minima))
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico), a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima, a_period, a_idPat, double()));

										else if ((a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima, a_period, a_idPat, double()) > 0.0) && (!todo_membro_disponivel_possui_potencia_minima)) {

											// Potencia ConjuntoHidraulico Relaxada (PH_REL)
											addVarDecisao_PH_REL(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima, a_period, a_idPat, double()), infinito, 0.0);

											// Potencia ConjuntoHidraulico Folga Minima (PH_FINF)
											addVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, 0.0, infinito, 0.0);

										} // else if ((a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima, a_period, a_idPat, double()) > 0.0) && (!todo_membro_disponivel_possui_potencia_minima)) {
									} // if (a_dados.getSize1Matriz(idUHE, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima) > 0) {

									if (a_dados.getSize1Matriz(idUHE, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_maxima) > 0)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico), a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_maxima, a_period, a_idPat, double()));

								} // if (getVarDecisao_PHseExistir(a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico) == -1) {

								//
								// Potencia ConjuntoHidraulico Disponivel (PHDISP)
								//

								if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico) == -1)
									addVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, 0.0, infinito, 0.0);

								if (a_dados.getSize1Matriz(idUHE, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_minima) > 0) {

									if ((a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_minima, a_period, a_idPat, double()) > 0.0) && (todo_membro_disponivel_possui_potencia_minima))
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico), a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_minima, a_period, a_idPat, double()));

									else if ((a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_minima, a_period, a_idPat, double()) > 0.0) && (!todo_membro_disponivel_possui_potencia_minima)) {

										// Potencia ConjuntoHidraulico Disponivel Relaxada (PHDISP_REL)
										addVarDecisao_PHDISP_REL(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_minima, a_period, a_idPat, double()), infinito, 0.0);

										// Potencia ConjuntoHidraulico Disponivel Folga Minima (PHDISP_FINF)
										addVarDecisao_PHDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, 0.0, infinito, 0.0);

									} // else if ((a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_minima, a_period, a_idPat, double()) > 0.0) && (!todo_membro_disponivel_possui_potencia_minima)) {
								} // if (a_dados.getSize1Matriz(idUHE, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_minima) > 0) {

								if (a_dados.getSize1Matriz(idUHE, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_maxima) > 0)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico), a_dados.getElementoMatriz(idUHE, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_maxima, a_period, a_idPat, double()));

							} // if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

							//
							// Turbinamento ConjuntoHidraulico
							//

							// Turbinamento ConjuntoHidraulico (QT)

							if (getVarDecisao_QTseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico) == -1) {

								addVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, 0.0, infinito, 0.0);

								if (a_dados.getSizeVetor(idUHE, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_minima) > 0) {

									if ((a_dados.getElementoVetor(idUHE, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_minima, a_period, double()) > 0.0) && (todo_membro_disponivel_possui_vazao_turbinada_minima))
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico), a_dados.getElementoVetor(idUHE, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_minima, a_period, double()));

									else if ((a_dados.getElementoVetor(idUHE, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_minima, a_period, double()) > 0.0) && (!todo_membro_disponivel_possui_vazao_turbinada_minima)) {

										// Turbinamento ConjuntoHidraulico Folga Minima (QT_FINF)
										addVarDecisao_QT_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, 0.0, infinito, 0.0);

									} // else if ((a_dados.getElementoVetor(idUHE, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_minima, a_period, double()) > 0.0) && (!todo_membro_disponivel_possui_vazao_turbinada_minima)) {
								} // if (a_dados.getSizeVetor(idUHE, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_minima) > 0) {

								if (a_dados.getSizeVetor(idUHE, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_maxima) > 0)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico), a_dados.getElementoVetor(idUHE, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_maxima, a_period, double()));

							} // if (getVarDecisao_QTseExistir(a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico) == -1) {

							// Turbinamento ConjuntoHidraulico Disponivel (QTDISP)
							if (getVarDecisao_QTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico) == -1)
								addVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConjuntoHidraulico, 0.0, infinito, 0.0);

						} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idUHE, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

					} // if ((tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_conjunto) || (tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {

				} // if (disponivel) {

			} // if (a_dados.getAtributo(idUHE, AttComumHidreletrica_considerar_usina, bool())){
		} // for (IdHidreletrica idUHE = a_dados.getMenorId(IdHidreletrica()); idUHE <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idUHE)) {


		// Restrição Operativa UHE Folgas (RHX_FSUP/RHX_FINF)
		const IdRestricaoOperativaUHE idResOpIni = a_dados.getMenorId(IdRestricaoOperativaUHE());
		const IdRestricaoOperativaUHE idResOpOut = a_dados.getIdOut(IdRestricaoOperativaUHE());
		for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = idResOpIni; idRestricaoOperativaUHE < idResOpOut; a_dados.vetorRestricaoOperativaUHE.incr(idRestricaoOperativaUHE)) {


			// Vazão Afluente
			//
			if (a_idPat == IdPatamarCarga_1 && a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_vazao_afluente) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_period, a_idPat, double()) != getdoubleFromChar("min")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum)
						addVarDecisao_RHA_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE, 0.0, infinito, 0.0);

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_period, a_idPat, double()) != getdoubleFromChar("min")) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_period, a_idPat, double()) != getdoubleFromChar("max")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum)
						addVarDecisao_RHA_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE, 0.0, infinito, 0.0);

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_period, a_idPat, double()) != getdoubleFromChar("max")) {

			}//if (a_idPat == IdPatamarCarga_1 && a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_vazao_afluente) {						


			// Vazão Defluente
			//
			if (a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_vazao_defluente) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_period, a_idPat, double()) != getdoubleFromChar("min")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum)
						addVarDecisao_RHQ_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE, 0.0, infinito, 0.0);

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_period, a_idPat, double()) != 0.0) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_period, a_idPat, double()) != getdoubleFromChar("max")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum)
						addVarDecisao_RHQ_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE, 0.0, infinito, 0.0);

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_period, a_idPat, double()) != getdoubleFromChar("max")) {

			}//if (a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_vazao_afluente) {						


			// Volume armazenado
			//
			if (a_idPat == IdPatamarCarga_1 && a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_volume_armazenado) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_period, a_idPat, double()) != getdoubleFromChar("min")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum)
						addVarDecisao_RHV_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE, 0.0, infinito, 0.0);

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_period, a_idPat, double()) != getdoubleFromChar("min")) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_period, a_idPat, double()) != getdoubleFromChar("max")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum)
						addVarDecisao_RHV_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE, 0.0, infinito, 0.0);

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_period, a_idPat, double()) != getdoubleFromChar("max")) {

			}//if (a_idPat == IdPatamarCarga_1 && a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_volume_armazenado) {						

			// Energia armazenada
			//
			if (a_idPat == IdPatamarCarga_1 && a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_energia_armazenada) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_period, a_idPat, double()) != getdoubleFromChar("min")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum && a_dados.getElementoVetor(idRestricaoOperativaUHE, AttVetorRestricaoOperativaUHE_limite_superior_folga_inferior, a_period, double()) > 0.0)
						addVarDecisao_RHE_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE, 0.0, infinito, 0.0);

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_period, a_idPat, double()) != getdoubleFromChar("min")) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_period, a_idPat, double()) != getdoubleFromChar("max")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum)
						addVarDecisao_RHE_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE, 0.0, infinito, 0.0);

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_period, a_idPat, double()) != getdoubleFromChar("max")) {

			}//if (a_idPat == IdPatamarCarga_1 && a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_energia_armazenad) {		


		} // for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE < a_maiorIdRestricaoOperativaUHE; idRestricaoOperativaUHE++) {

		//Variáveis associadas aos intercambios hidráulicos
		const IdIntercambioHidraulico idInterHIni = a_dados.getMenorId(IdIntercambioHidraulico());
		const IdIntercambioHidraulico idInterHOut = a_dados.getIdOut(IdIntercambioHidraulico());
		for (IdIntercambioHidraulico idIntercambioHidraulico = idInterHIni; idIntercambioHidraulico < idInterHOut; a_dados.vetorIntercambioHidraulico.incr(idIntercambioHidraulico)) {

			const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

			if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio)
				addVarDecisao_QI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico, a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica()), a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo, a_period, double()), a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo, a_period, double()), 0.0);

			else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada) {

				addVarDecisao_QI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico, a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo, a_period, double()), a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo, a_period, double()), 0.0);

				if (a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo, a_period, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo, a_period, double()) > 0) {

					addVarDecisao_QI_REL(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico, a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), 0.0, infinito, 0.0);
					addVarDecisao_QI_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico, a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), 0.0, a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo, a_period, double()), 0.0);

				}//if(a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo, periodo_estudo, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo, periodo_estudo, double()) > 0)

			}//else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada) {

			else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_pereira_barreto) {

				addVarDecisao_QILS_TRI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico, a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo, a_period, double()), a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo, a_period, double()), 0.0);

				if (a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo, a_period, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo, a_period, double()) > 0) {

					addVarDecisao_QILS_TRI_REL(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico, 0.0, infinito, 0.0);
					addVarDecisao_QILS_TRI_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico, 0.0, a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo, a_period, double()), 0.0);

				}//if(a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo, periodo_estudo, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo, periodo_estudo, double()) > 0)

			}//else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada) {

		}//for (IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico_1; idIntercambioHidraulico < a_maiorIdIntercambioHidraulico; idIntercambioHidraulico++) {

		//Variáveis para bombeamento QB

		const IdUsinaElevatoria idUsiElHIni = a_dados.getMenorId(IdUsinaElevatoria());
		const IdUsinaElevatoria idUsiElHOut = a_dados.getIdOut(IdUsinaElevatoria());
		for (IdUsinaElevatoria idUsinaElevatoria = idUsiElHIni; idUsinaElevatoria < idUsiElHOut; a_dados.vetorUsinaElevatoria.incr(idUsinaElevatoria)) {

			if (a_dados.getElementoVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_fator_disponibilidade, a_period, double()) > 0.0) {

				if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

					addVarDecisao_PBDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria, 0.0, infinito, 0.0);

				} // if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

				// QB
				addVarDecisao_QB(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria, 0.0, a_dados.getElementoVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_vazao_bombeada_maxima, a_period, double()), 0.0);

				// QB_FINF
				if (a_dados.getElementoVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_vazao_bombeada_minima, a_period, double()) > 0.0)
					addVarDecisao_QB_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria, 0.0, infinito, 0.0);

				// QBDISP
				addVarDecisao_QBDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria, 0.0, a_dados.getElementoMatriz(idUsinaElevatoria, AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_maxima, a_period, a_idPat, double()), 0.0);

				// QBDISP_FINF
				if (a_dados.getElementoMatriz(idUsinaElevatoria, AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_minima, a_period, a_idPat, double()) > 0.0)
					addVarDecisao_QBDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria, 0.0, infinito, 0.0);

			} // if (a_dados.getElementoVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_fator_disponibilidade, a_period, double()) > 0.0) {
		} // for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria < a_maiorIdUsinaElevatoria; idUsinaElevatoria++) {

	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisHidraulicas(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPat) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarVariaveisDemandaEspecial(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat) {
	try {

		const IdDemandaEspecial idDEIni = a_dados.getMenorId(IdDemandaEspecial());
		const IdDemandaEspecial idDEOut = a_dados.getIdOut(IdDemandaEspecial());

		for (IdDemandaEspecial idDemandaEspecial = idDEIni; idDemandaEspecial < idDEOut; a_dados.vetorDemandaEspecial.incr(idDemandaEspecial))
			addVarDecisao_PLE(a_TSS, a_idEstagio, a_period, a_idPat, idDemandaEspecial, a_dados.getElementoMatriz(idDemandaEspecial, AttMatrizDemandaEspecial_demanda, a_period, a_idPat, double()), a_dados.getElementoMatriz(idDemandaEspecial, AttMatrizDemandaEspecial_demanda, a_period, a_idPat, double()), 0.0);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisDemandaEspecials(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPat) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarVariaveisUsinaNaoSimulada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat) {
	try {

		const IdSubmercado idSSEIni = a_dados.getMenorId(IdSubmercado());
		const IdSubmercado idSSEOut = a_dados.getIdOut(IdSubmercado());

		for (IdSubmercado idSSE = idSSEIni; idSSE < idSSEOut; a_dados.vetorSubmercado.incr(idSSE)) {

			const IdUsinaNaoSimulada menorIdUsinaNaoSimulada = a_dados.getMenorId(idSSE, IdUsinaNaoSimulada());
			const IdUsinaNaoSimulada maiorIdUsinaNaoSimulada = a_dados.getIdOut(idSSE, IdUsinaNaoSimulada());

			for (IdUsinaNaoSimulada idUsinaNaoSimulada = menorIdUsinaNaoSimulada; idUsinaNaoSimulada < maiorIdUsinaNaoSimulada; a_dados.incr(idSSE, idUsinaNaoSimulada)) {
				if (idUsinaNaoSimulada != IdUsinaNaoSimulada_Nenhum) {
					addVarDecisao_PN(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, idUsinaNaoSimulada, a_dados.getElementoMatriz(idSSE, idUsinaNaoSimulada, AttMatrizUsinaNaoSimulada_potencia_minima, a_period, a_idPat, double()), a_dados.getElementoMatriz(idSSE, idUsinaNaoSimulada, AttMatrizUsinaNaoSimulada_potencia_maxima, a_period, a_idPat, double()), 0.0);
				}//if (idUsinaNaoSimulada != IdUsinaNaoSimulada_Nenhum) {
			}//for(IdUsinaNaoSimulada idUsinaNaoSimulada = menorIdUsinaNaoSimulada; idUsinaNaoSimulada < maiorIdUsinaNaoSimulada; a_dados.incr(idSSE, idUsinaNaoSimulada)) {

		}

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisUsinaNaoSimulada(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPat) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}

void ModeloOtimizacao::criarVariaveisIntercambio(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat) {
	try {

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const IdIntercambio idInterIni = a_dados.getMenorId(IdIntercambio());
		const IdIntercambio idInterOut = a_dados.getIdOut(IdIntercambio());

		for (IdIntercambio idIntercambio = idInterIni; idIntercambio < idInterOut; a_dados.vetorIntercambio.incr(idIntercambio)) {

			addVarDecisao_PI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio, 0.0, a_dados.getElementoMatriz(idIntercambio, AttMatrizIntercambio_potencia_maxima, a_period, a_idPat, double()), 0.0);

			if (a_dados.getElementoMatriz(idIntercambio, AttMatrizIntercambio_potencia_minima, a_period, a_idPat, double()) > 0.0) {
				//Potência Intercambios Folga Mínima (PI_FINF)
				addVarDecisao_PI_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio, 0.0, infinito, 0.0);
			}//if (a_dados.getElementoMatriz(idIntercambio, AttMatrizIntercambio_potencia_minima, a_period, a_idPat, double()) > 0.0) {

		} // for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio < a_maiorIdIntercambio; idIntercambio++) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisIntercambio(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPat) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarVariaveisDemanda(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat) {
	try {

		const IdSubmercado idSSEIni = a_dados.getMenorId(IdSubmercado());
		const IdSubmercado idSSEOut = a_dados.getIdOut(IdSubmercado());

		for (IdSubmercado idSSE = idSSEIni; idSSE < idSSEOut; a_dados.vetorSubmercado.incr(idSSE)) {

			// VARIAVEIS POTÊNCIA PL (PL)
			if (a_dados.getSize1Matriz(idSSE, AttMatrizSubmercado_demanda) > 0) {
				if (a_dados.getSize2Matriz(idSSE, AttMatrizSubmercado_demanda, a_period) > 0) {
					if (a_dados.getElementoMatriz(idSSE, AttMatrizSubmercado_demanda, a_period, a_idPat, double()) > 0.0) {

						addVarDecisao_PL(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, a_dados.getElementoMatriz(idSSE, AttMatrizSubmercado_demanda, a_period, a_idPat, double()), a_dados.getElementoMatriz(idSSE, AttMatrizSubmercado_demanda, a_period, a_idPat, double()), 0.0);

						const double denominador = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, a_idPat, double()) * double(double(a_period.getMinutos()) / 60.0);

						if (denominador == 0) { throw std::invalid_argument("Denominador = 0.0"); }

						setNormalizacaoDualVarDecisao_PL(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, 1.0 / denominador);

					} // if (a_dados.getElementoMatriz(idSSE, AttMatrizSubmercado_demanda, a_period, a_idPat, double()) > 0){
				} // if (a_dados.getSize2Matriz(idSSE, AttMatrizSubmercado_demanda, a_period) > 0) {
			} // if (a_dados.getSize1Matriz(idSSE, AttMatrizSubmercado_demanda) > 0) {

		}

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisDemanda(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPat) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}



void ModeloOtimizacao::criarVariaveisDeficit(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat) {
	try {

		const IdSubmercado idSSEIni = a_dados.getMenorId(IdSubmercado());
		const IdSubmercado idSSEOut = a_dados.getIdOut(IdSubmercado());

		for (IdSubmercado idSSE = idSSEIni; idSSE < idSSEOut; a_dados.vetorSubmercado.incr(idSSE)) {

			const IdPatamarDeficit idPatDefIni = a_dados.getMenorId(idSSE, IdPatamarDeficit());
			const IdPatamarDeficit idPatDefOut = a_dados.getIdOut(idSSE, IdPatamarDeficit());

			for (IdPatamarDeficit idPatamarDeficit = idPatDefIni; idPatamarDeficit < idPatDefOut; a_dados.vetorSubmercado.at(idSSE).vetorPatamarDeficit.incr(idPatamarDeficit)) {
				if (a_dados.getElementoMatriz(idSSE, idPatamarDeficit, AttMatrizPatamarDeficit_potencia_maxima, a_period, a_idPat, double()) > 0.0) {

					addVarDecisao_PD(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, idPatamarDeficit, 0.0, a_dados.getElementoMatriz(idSSE, idPatamarDeficit, AttMatrizPatamarDeficit_potencia_maxima, a_period, a_idPat, double()), 0.0);

				} // if (a_dados.getElementoMatriz(idSSE, idPatamarDeficit, AttMatrizPatamarDeficit_potencia_maxima, a_period, idPat, double()) > 0.0) {
			} // for (IdPatamarDeficit idPatamarDeficit = IdPatamarDeficit_1; idPatamarDeficit < maiorIdPatamarDeficit; idPatamarDeficit++) {
		}

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisDeficit(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPat) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}





void ModeloOtimizacao::criarVariaveisContrato(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat) {
	try {

		const IdContrato idContratoIni = a_dados.getMenorId(IdContrato());
		const IdContrato idContratoOut = a_dados.getIdOut(IdContrato());

		for (IdContrato idContrato = idContratoIni; idContrato < idContratoOut; a_dados.vetorContrato.incr(idContrato)) {

			if (a_dados.getAtributo(idContrato, AttComumContrato_tipo_contrato, TipoContrato()) == TipoContrato_exportacao)
				addVarDecisao_CEXP(a_TSS, a_idEstagio, a_period, a_idPat, idContrato, a_dados.getElementoMatriz(idContrato, AttMatrizContrato_energia_minima, a_period, a_idPat, double()), a_dados.getElementoMatriz(idContrato, AttMatrizContrato_energia_maxima, a_period, a_idPat, double()), 0.0);

			else if (a_dados.getAtributo(idContrato, AttComumContrato_tipo_contrato, TipoContrato()) == TipoContrato_importacao)
				addVarDecisao_CIMP(a_TSS, a_idEstagio, a_period, a_idPat, idContrato, a_dados.getElementoMatriz(idContrato, AttMatrizContrato_energia_minima, a_period, a_idPat, double()), a_dados.getElementoMatriz(idContrato, AttMatrizContrato_energia_maxima, a_period, a_idPat, double()), 0.0);

			else
				throw std::invalid_argument("Tipo de contrato não identificado");

		}

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisContratos(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPat) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}

void ModeloOtimizacao::criarVariaveisEolicas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat) {
	try {

		const IdUsinaEolica idEolIni = a_dados.getMenorId(IdUsinaEolica());
		const IdUsinaEolica IdEolOut = a_dados.getIdOut(IdUsinaEolica());

		for (IdUsinaEolica idUsinaEolica = idEolIni; idUsinaEolica < IdEolOut; a_dados.vetorUsinaEolica.incr(idUsinaEolica)) {

			if (a_dados.getAtributo(idUsinaEolica, AttComumUsinaEolica_constrained_off, bool()))
				addVarDecisao_PE(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaEolica, 0.0, a_dados.getElementoMatriz(idUsinaEolica, AttMatrizUsinaEolica_geracao, a_period, a_idPat, double()), 0.0);
			else
				addVarDecisao_PE(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaEolica, a_dados.getElementoMatriz(idUsinaEolica, AttMatrizUsinaEolica_geracao, a_period, a_idPat, double()), a_dados.getElementoMatriz(idUsinaEolica, AttMatrizUsinaEolica_geracao, a_period, a_idPat, double()), 0.0);
		}

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisEolicas(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPat) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}

void ModeloOtimizacao::criarVariaveisRestricaoEletrica(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat) {
	try {

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const IdRestricaoEletrica idREIni = a_dados.getMenorId(IdRestricaoEletrica());
		const IdRestricaoEletrica idREOut = a_dados.getIdOut(IdRestricaoEletrica());

		for (IdRestricaoEletrica idRE = idREIni; idRE < idREOut; a_dados.vetorRestricaoEletrica.incr(idRE)) {

			//RE_FINF

			if (a_dados.getElementoMatriz(idRE, AttMatrizRestricaoEletrica_potencia_minima, a_period, a_idPat, double()) > 0.0) {

				if (a_dados.getMaiorId(idRE, IdElementoSistema()) != IdElementoSistema_Nenhum)
					addVarDecisao_RE_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idRE, 0.0, infinito, 0.0);

			} // if (a_dados.getElementoMatriz(idRE, AttMatrizRestricaoEletrica_potencia_minima, a_period, a_idPat, double()) > 0.0) {

			//RE_FSUP

			if (a_dados.getElementoMatriz(idRE, AttMatrizRestricaoEletrica_potencia_maxima, a_period, a_idPat, double()) >= 0.0 && \
				a_dados.getElementoMatriz(idRE, AttMatrizRestricaoEletrica_potencia_maxima, a_period, a_idPat, double()) != getdoubleFromChar("max")) {

				if (a_dados.getMaiorId(idRE, IdElementoSistema()) != IdElementoSistema_Nenhum)
					addVarDecisao_RE_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idRE, 0.0, infinito, 0.0);

			}

		} // for (IdRestricaoEletrica idRE = idREIni; idRE < idREOut; idRE++) {


	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisRestricaoEletrica(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPat) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarVariaveisTermeletricas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodIni_stage, Periodo& a_periodPrev, Periodo& a_period, const IdPatamarCarga a_idPat) {
	try {

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const IdTermeletrica idUTEIni = a_dados.getMenorId(IdTermeletrica());
		const IdTermeletrica idUTEOut = a_dados.getIdOut(IdTermeletrica());
		for (IdTermeletrica idUTE = idUTEIni; idUTE < idUTEOut; a_dados.vetorTermeletrica.incr(idUTE)) {

			if (a_dados.getAtributo(idUTE, AttComumTermeletrica_considerar_usina, bool())) {

				const TipoDetalhamentoProducaoTermeletrica tipo_detalhamento_producao = a_dados.getAtributo(idUTE, AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica());

				bool disponivel = false;

				// ------------------------------------------------------------------------------------------------------------------------------------------------------
				//
				//                                                                    Produção Por Termeletrica
				//
				// ------------------------------------------------------------------------------------------------------------------------------------------------------

				if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina) {

					if (a_dados.getElementoVetor(idUTE, AttVetorTermeletrica_disponibilidade, a_period, double()) > 0.0) {

						disponivel = true;

						//POTÊNCIA DAS TERMELÉTRICAS
						addVarDecisao_PT(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, 0.0, a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_minima, a_period, a_idPat, double()) + a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_util, a_period, a_idPat, double()), 0.0);
						addVarDecisao_PTUTIL(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, 0.0, a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_util, a_period, a_idPat, double()), 0.0);
						addVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, 0.0, infinito, 0.0);

						if (a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_minima, a_period, a_idPat, double()) > 0.0)
							addVarDecisao_UTE_ON_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, 1.0, 1.0, 0.0);


						// Restrição Comando GNL

						const int lag_mensal_potencia_disponivel_comandada = a_dados.getAtributo(idUTE, AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada, int());

						if (lag_mensal_potencia_disponivel_comandada > 0) {

							const int varPTDISPCOM_pat = criarVariaveisDecisao_VariaveisEstado_Restricoes_PTDISPCOM(a_TSS, a_dados, a_idEstagio, a_period, a_idPat, idUTE, a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_minima, a_period, a_idPat, double()), a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_maxima, a_period, a_idPat, double()));

							const int varPTDISPCOM_FINF_pat = addVarDecisao_PTDISPCOM_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, 0.0, vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);
							const int varPTDISPCOM_FSUP_pat = addVarDecisao_PTDISPCOM_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, 0.0, vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);

						} // if (lag_mensal_potencia_disponivel_comandada > 0) {

					} // if (a_dados.getElementoVetor(idUTE, AttVetorTermeletrica_disponibilidade, a_period, double()) > 0.0) {

				} // if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina) {

				// ------------------------------------------------------------------------------------------------------------------------------------------------------
				//
				//                                                                    Produção Por Unidade Termeletrica
				//
				// ------------------------------------------------------------------------------------------------------------------------------------------------------

				else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {

					for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= a_dados.getMaiorId(idUTE, IdUnidadeUTE()); idUnidadeUTE++) {

						//if (a_dados.getElementoVetor(idUTE, idUnidadeUTE, AttVetorUnidadeUTE_disponibilidade, a_period, double()) > 0.0) {
						if (true) {
							disponivel = true;

							// REPRESENTAÇÃO DISCRETA 
							if (a_dados.getAtributo(idUTE, AttComumTermeletrica_representacao_discreta_producao, bool())) {

								//VARIAVEIS DO ESTADO INICIAL DO PROBLEMA
								if (a_period == a_periodIni_stage) {

									addVarDecisao_UTE_ON_T(a_TSS, a_idEstagio, a_periodPrev, a_idPat, idUTE, idUnidadeUTE, 1.0, 1.0, 0.0);
									//addVarDecisao_PTUTIL(a_idEstagio, a_periodPrev, a_idPat, idUTE, idUnidadeUTE, 0.0, infinito, 0.0);
									//addVarDecisao_ACIO_T(a_idEstagio, a_periodPrev, a_idPat, idUTE, idUnidadeUTE, 0.0, 1.0, 0.0);

								}//if (a_period == a_periodIni_stage) {

								addVarDecisao_UTE_OFF_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE, 0.0, 1.0, 0.0);
								addVarDecisao_ACIO_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE, 0.0, 1.0, 0.0);

								addVarDecisao_PT_TRAJ_ACIO(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE, 0.0, a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_minima, a_period, a_idPat, double()), 0.0);
								addVarDecisao_PT_TRAJ_DESL(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE, 0.0, a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_minima, a_period, a_idPat, double()), 0.0);

							}//if (a_dados.getAtributo(idUTE, AttComumTermeletrica_representacao_discreta_producao,  bool())) {


							// Potencia Por UnidadeUTE
							//
							addVarDecisao_PT(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE, 0.0, a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima, a_period, a_idPat, double()), 0.0);
							addVarDecisao_PTUTIL(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE, 0.0, a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_util, a_period, a_idPat, double()), 0.0);
							addVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE, 0.0, infinito, 0.0);
							addVarDecisao_UTE_ON_T(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE, 1.0, 1.0, 0.0);


							//
							// Potencia Disponivel Por UnidadeUTE
							//

							if (a_dados.getSize1Matriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_maxima) > 0)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_maxima, a_period, a_idPat, double()));

						} // if (a_dados.getElementoVetor(idUTE, idUnidadeUTE, AttVetorUnidadeUTE_disponibilidade, a_period, double()) > 0.0) {

					} // for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= a_dados.getMaiorId(idUTE, IdUnidadeUTE()); idUnidadeUTE++){

				} // else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {

				//
				// Potencia Disponivel Termeletrica
				//

				if (disponivel) {

					//if ((a_dados.getAtributo(idUTE, AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada, int()) > 0) && (getVarDecisao_PTCOM_FSUPseExistir(a_idEstagio, a_period, a_idPat, idUTE) == -1)) {
						//addVarDecisao_PTCOM_FSUP(a_idEstagio, a_period, a_idPat, idUTE, 0.0, infinito, 0.0);
					//	addVarDecisao_PTCOM_FINF(a_idEstagio, a_period, a_idPat, idUTE, 0.0, infinito, 0.0);
					//}


					if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE) == -1)
						addVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, 0.0, infinito, 0.0);


						if (a_dados.getSize1Matriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_meta) > 0) {

							if (a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_meta, IdCenario_1, a_period, double()) != getdoubleFromChar("max")) {

								int varPTDISP = getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, idUTE);
								if (varPTDISP < 0)
									varPTDISP = addVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, idUTE, 0.0, infinito, 0.0);

								const IdVariavelEstado idVariavelEstado = vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, getNomeVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, idUTE), varPTDISP, -1);

								const IdProcesso idProcesso = a_dados.arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());
								const IdCenario menor_cenario = a_dados.arranjoResolucao.getAtributo(idProcesso, AttComumProcesso_menor_cenario, IdCenario());
								const IdCenario maior_cenario = a_dados.arranjoResolucao.getAtributo(idProcesso, AttComumProcesso_maior_cenario, IdCenario());

								vetorEstagio.at(a_idEstagio).addValorVariavelEstado(idVariavelEstado, menor_cenario, menor_cenario, maior_cenario, 0.0);
								for (IdCenario idCenario = menor_cenario; idCenario <= maior_cenario; idCenario++)
									vetorEstagio.at(a_idEstagio).addValorVariavelEstado(idVariavelEstado, idCenario, IdCenario_Nenhum, IdCenario_Nenhum, a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_meta, idCenario, a_period, double()));

							} // if (a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_meta, IdCenario_1, a_period, double()) != getdoubleFromChar("max")) {

						} // if (a_dados.getSize1Matriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_meta) > 0) {


					if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {
						if (a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_minima, a_period, a_idPat, double()) > 0.0)
							addVarDecisao_PTDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, 0.0, infinito, 0.0);
						if (a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_maxima, a_period, a_idPat, double()) > 0.0)
							addVarDecisao_PTDISP_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, 0.0, infinito, 0.0);
					}

					else {

						double potencia_disponivel_minima = -1.0;
						double potencia_disponivel_maxima = -1.0;

						if (a_dados.getSize1Matriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_minima) != 0)
							if (a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_minima, a_period, a_idPat, double()) > a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_minima, a_period, a_idPat, double()) * a_dados.getElementoVetor(idUTE, AttVetorTermeletrica_disponibilidade, a_period, double()))
								potencia_disponivel_minima = a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_minima, a_period, a_idPat, double());

						if (a_dados.getSize1Matriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_maxima) != 0)
							if (a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_maxima, a_period, a_idPat, double()) < (a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_minima, a_period, a_idPat, double()) + a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_util, a_period, a_idPat, double())) * a_dados.getElementoVetor(idUTE, AttVetorTermeletrica_disponibilidade, a_period, double()))
								potencia_disponivel_maxima = a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_maxima, a_period, a_idPat, double());

						if (potencia_disponivel_minima > -1.0)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_minima, a_period, a_idPat, double()));

						if (potencia_disponivel_maxima > -1.0)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_maxima, a_period, a_idPat, double()));

						const double potencia_minima = a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_minima, a_period, a_idPat, double());

						if ((potencia_minima > 0.0) || (potencia_disponivel_minima > -1.0)) {

							addVarDecisao_PT_REL(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, potencia_minima, infinito, 0.0);

							addVarDecisao_PT_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, 0.0, potencia_minima, 0.0);

						} // if (potencia_minima > 0.0) {
					}

				} // if (disponivel) {

			} // if (a_dados.getAtributo(idUTE, AttComumTermeletrica_considerar_usina, bool())) {
		} // for (IdTermeletrica idUTE = a_dados.getMenorId(IdTermeletrica()); idUTE < maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idUTE)) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisTermeletricas(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPat) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}

int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_ZP0_VF_FINF(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo_penalizacao) {

	try {

		const IdEstagio menor_estagio = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());

		if (a_idEstagio < menor_estagio) {
			if (menor_estagio == IdEstagio_1)
				return -1;
			else
				return -2;
		}

		if ((!vetorEstagio.at(a_idEstagio).isSolverInstanciado(a_TSS)) || (a_TSS == TipoSubproblemaSolver_mestre))
			return -1;
	
		const Periodo periodo_otimizacao = getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

		// Identifica periodo penalizacao e insere penalizacao da folga inferior do volume final na função objetivo
		if (!a_periodo_penalizacao.isValido()) {

			const IdMes mes_penalizacao_volume_util_minimo = a_dados.getAtributo(AttComumDados_mes_penalizacao_volume_util_minimo, IdMes());

			const Periodo periodo_minuto_otimizacao_final = Periodo(TipoPeriodo_minuto, periodo_otimizacao + 1) - 1;
			const Periodo periodo_minuto_otimizacao_inicial = Periodo(TipoPeriodo_minuto, periodo_otimizacao);

			const IdMes idMes_inicio = periodo_minuto_otimizacao_inicial.getMes();
			const IdAno idAno_inicio = periodo_minuto_otimizacao_inicial.getAno();

			Periodo periodo_penalizacao = Periodo(TipoPeriodo_minuto, Periodo(mes_penalizacao_volume_util_minimo, idAno_inicio) + 1) -1;
			if (idMes_inicio > mes_penalizacao_volume_util_minimo)
				periodo_penalizacao = Periodo(TipoPeriodo_minuto, Periodo(mes_penalizacao_volume_util_minimo, IdAno(int(idAno_inicio) + 1)) + 1) - 1;

			if ((periodo_minuto_otimizacao_inicial > periodo_penalizacao) || (periodo_minuto_otimizacao_final < periodo_penalizacao))
				return -1;

			const SmartEnupla<Periodo, double> horizonte_estudo = getElementosMatriz(AttMatrizModeloOtimizacao_horizonte_estudo, a_idEstagio, Periodo(), double());

			for (Periodo periodo = periodo_penalizacao; periodo <= periodo_minuto_otimizacao_final; periodo++) {

				// Adiciona penalizacao na função objetivo
				if (periodo == periodo_penalizacao) {
					
					const int varZPO_VF_FINF = criarVariaveisDecisao_VariaveisEstado_Restricoes_ZP0_VF_FINF(a_TSS, a_dados, a_idEstagio, periodo_penalizacao);
					
					if (varZPO_VF_FINF > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varZPO_VF_FINF, getEquLinear_ZP0(a_TSS, a_idEstagio), -1.0);

					periodo_penalizacao = Periodo(TipoPeriodo_minuto, Periodo(TipoPeriodo_anual, periodo_penalizacao) + 1) - 1;

					if (periodo_minuto_otimizacao_final < periodo_penalizacao)
						return -1;

				} // if (periodo == periodo_penalizacao) {

			} // for (Periodo periodo = periodo_penalizacao; periodo <= periodo_minuto_otimizacao_final; periodo++) {

			return -1;

		} // if (!a_periodo_penalizacao.isValido()) {

		
		// Recursivamente cria variaveis e restrições para contabilizar ZPO_VF_FINF

		const Periodo periodo_final_horizonte_estudo = getIterador2Final(AttMatrizModeloOtimizacao_horizonte_estudo, a_idEstagio, Periodo());
		const SmartEnupla<Periodo, double> horizonte_estudo = getElementosMatriz(AttMatrizModeloOtimizacao_horizonte_estudo, a_idEstagio, Periodo(), double());
		const Periodo periodo_inicial_horizonte_estudo = horizonte_estudo.getIteradorInicial();

		if (periodo_inicial_horizonte_estudo > a_periodo_penalizacao)
			throw std::invalid_argument("Inicio do horizonte de estudo " + getFullString(periodo_inicial_horizonte_estudo) + " nao pode ser maior que periodo penalizacao.");

		const Periodo periodo_penalizacao_anterior = Periodo(TipoPeriodo_minuto, Periodo(TipoPeriodo_anual, a_periodo_penalizacao) - 1);
		if ((Periodo(periodo_final_horizonte_estudo + 1) <= periodo_penalizacao_anterior))
			return -1;

		int varZP0_VF_FINF = getVarDecisao_ZP0_VF_FINFseExistir(a_TSS, a_idEstagio, a_periodo_penalizacao);
		if (varZP0_VF_FINF > -1)
			return varZP0_VF_FINF;

		const int varZP0_VF_FINF_ant = criarVariaveisDecisao_VariaveisEstado_Restricoes_ZP0_VF_FINF(a_TSS, a_dados, IdEstagio(a_idEstagio - 1), a_periodo_penalizacao);

		// Atualmente VF é criada somente no periodo final do horizonte.
		const Periodo periodo_inicial_varredura_horizonte_estudo = periodo_final_horizonte_estudo;

		for (Periodo periodo = periodo_inicial_varredura_horizonte_estudo; periodo <= periodo_final_horizonte_estudo; horizonte_estudo.incrementarIterador(periodo)) {

			const Periodo periodo_minuto_final = Periodo(TipoPeriodo_minuto, Periodo(periodo + 1)) - 1;

			double percentual_inclusao_periodo = 0.0;

			// Periodo totalmente incluso no periodo de penalizacao anterior
			if (periodo_minuto_final <= periodo_penalizacao_anterior)
				percentual_inclusao_periodo = 0.0;

			// Periodo parcialmente incluso no periodo de penalizacao anterior
			if ((periodo < periodo_penalizacao_anterior) && (periodo_penalizacao_anterior < periodo_minuto_final)) {
				if (periodo_minuto_final <= a_periodo_penalizacao)
					percentual_inclusao_periodo = (double(periodo_minuto_final - (periodo_penalizacao_anterior + 1)) + 1.0) / double(periodo.getMinutos());
				else
					percentual_inclusao_periodo = (double(a_periodo_penalizacao - (periodo_penalizacao_anterior + 1)) + 1.0) / double(periodo.getMinutos());
			}

			// Periodo engloba parcialmente o periodo de penalizacao
			else if ((periodo < a_periodo_penalizacao) && (periodo_minuto_final <= a_periodo_penalizacao))
				percentual_inclusao_periodo = 1.0;

			// Periodo engloba parcialmente o periodo de penalizacao
			else if ((periodo <= a_periodo_penalizacao) && (a_periodo_penalizacao < periodo_minuto_final))
				percentual_inclusao_periodo = double((a_periodo_penalizacao - Periodo(TipoPeriodo_minuto, periodo)) + 1.0) / double(periodo.getMinutos());

			else if (periodo > a_periodo_penalizacao)
				return varZP0_VF_FINF;

			if (percentual_inclusao_periodo > 0.0) {

				for (IdHidreletrica idUHE = a_dados.getMenorId(IdHidreletrica()); idUHE < a_dados.getIdOut(IdHidreletrica()); a_dados.vetorHidreletrica.incr(idUHE)) {
					if (getVarDecisao_VF_FINFseExistir(a_TSS, a_idEstagio, periodo, idUHE) > -1) {

						if (getVarDecisao_ZP0_VF_FINFseExistir(a_TSS, a_idEstagio, periodo, a_periodo_penalizacao) == -1) {

							if (getEquLinear_ZP0_VF_FINFseExistir(a_TSS, a_idEstagio, a_periodo_penalizacao) == -1) {
								addEquLinear_ZP0_VF_FINF(a_TSS, a_idEstagio, a_periodo_penalizacao);

								varZP0_VF_FINF = addVarDecisao_ZP0_VF_FINF(a_TSS, a_idEstagio, a_periodo_penalizacao, 0.0, vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);

								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varZP0_VF_FINF, getEquLinear_ZP0_VF_FINF(a_TSS, a_idEstagio, a_periodo_penalizacao), 1.0);

								if (varZP0_VF_FINF_ant != -1) {
									const int varZP0_VF_FINF_ADD = addVarDecisao_ZP0_VF_FINF_ADD(a_TSS, a_idEstagio, a_periodo_penalizacao, 0.0, 0.0, 0.0);
									vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeVarDecisao_ZP0_VF_FINF(a_TSS, a_idEstagio, a_periodo_penalizacao)), varZP0_VF_FINF_ADD, varZP0_VF_FINF_ant);

									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varZP0_VF_FINF_ADD, getEquLinear_ZP0_VF_FINF(a_TSS, a_idEstagio, a_periodo_penalizacao), -1.0);
								}

							} // if (getEquLinear_ZP0_VF_FINFseExistir(a_TSS, a_idEstagio, a_periodo_penalizacao) == -1) {

							const int varZP0_VF_FINF_per = addVarDecisao_ZP0_VF_FINF(a_TSS, a_idEstagio, periodo, a_periodo_penalizacao, 0.0, vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);
							const int equZP0_VF_FINF_per = addEquLinear_ZP0_VF_FINF(a_TSS, a_idEstagio, periodo, a_periodo_penalizacao);

							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varZP0_VF_FINF_per, equZP0_VF_FINF_per, 1.0);

							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varZP0_VF_FINF_per, getEquLinear_ZP0_VF_FINF(a_TSS, a_idEstagio, a_periodo_penalizacao), -1.0);

						} // if (getVarDecisao_ZP0_VF_FINFseExistir(a_TSS, a_idEstagio, periodo, a_periodo_penalizacao) == -1) {

						const double valor_penalidade = a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_volume_util_minimo, double()) * a_dados.getElementoMatriz(AttMatrizDados_desagio_acumulado_horizonte_estudo, a_idEstagio, periodo, double());
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF_FINF(a_TSS, a_idEstagio, periodo, idUHE), getEquLinear_ZP0_VF_FINF(a_TSS, a_idEstagio, periodo, a_periodo_penalizacao), -valor_penalidade);

					} // if (getVarDecisao_VF_FINFseExistir(a_TSS, a_idEstagio, periodo, idUHE) > -1) {
				} // for (IdHidreletrica idUHE = a_dados.getMenorId(IdHidreletrica()); idUHE < a_dados.getMaiorId(IdHidreletrica()); a_dados.vetorHidreletrica.incr(idUHE)) {

			} // if (percentual_inclusao_periodo > 0.0) {

		} // for (Periodo periodo = periodo_inicial_horizonte_estudo; periodo <= periodo_final_horizonte_estudo; horizonte_estudo.incrementarIterador(periodo)) {

		return varZP0_VF_FINF;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisDecisao_VariaveisEstado_Restricoes_ZP0_VF_FINF(a_dados," + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getString(a_periodo_penalizacao) + "): \n" + std::string(erro.what())); }

} // int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_ZP0_VF_FINF(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo) {

int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_QDLAG(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo, const IdHidreletrica a_idHidreletrica, Periodo &a_periodo_lag) {

	try {

		const IdEstagio stageIni = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());

		if (a_idEstagio < stageIni)
			return -1;

		if ((!vetorEstagio.at(a_idEstagio).isSolverInstanciado(a_TSS)) || (a_TSS == TipoSubproblemaSolver_mestre))
			return -1;

		const Periodo periodo_otimizacao = getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

		const Periodo periodEnd_stage = getIterador2Final(AttMatrizModeloOtimizacao_horizonte_estudo, a_idEstagio, Periodo());

		// Verifica se periodo não é mais necessario para compor ou repassar lag
		if (periodEnd_stage + 1 <= a_periodo_lag)
			return -1;

		const Periodo periodIni_stage    = getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_estudo, a_idEstagio, Periodo());
		const Periodo periodIni_stageIni = getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_estudo, stageIni, Periodo());

		int varQDLAG = getVarDecisao_QDLAGseExistir(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idHidreletrica);
		if (varQDLAG > -1)
			return varQDLAG;

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		varQDLAG = addVarDecisao_QDLAG(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idHidreletrica,  0.0, infinito, 0.0);

		// ADD any compound of QD in the current stage.
		int equQDLAG = -1;
		const double sobreposicao_periodo_otimizacao = periodo_otimizacao.sobreposicao(a_periodo_lag);
		if (sobreposicao_periodo_otimizacao > 0.0) {

			const SmartEnupla<Periodo, IdEstagio> horizon = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

			equQDLAG = addEquLinear_QD_LAG(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idHidreletrica);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDLAG, equQDLAG, 1.0);

			bool sobreposicao_encontrada = false;
			for (Periodo periodo = periodIni_stage; periodo <= periodEnd_stage; horizon.incrementarIterador(periodo)) {

				const double sobreposicao = a_periodo_lag.sobreposicao(periodo);

				if (sobreposicao > 0.0) {
					for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPat++) {
						int varQD = getVarDecisao_QDseExistir(a_TSS, a_idEstagio, periodo, idPat, a_idHidreletrica);
						if (varQD < 0)
							varQD = addVarDecisao_QD(a_TSS, a_idEstagio, periodo, idPat, a_idHidreletrica, 0.0, infinito, 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQD, equQDLAG, -sobreposicao * a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPat, double()));
					}
					if (!sobreposicao_encontrada)
						sobreposicao_encontrada = true;
				}
				else if ((sobreposicao == 0.0) && (sobreposicao_encontrada))
					break;

			} // for (Periodo periodo = periodIni_stage; periodo <= periodEnd_stage; horiz_stage.incrementarIterador(periodo)) {
			
			if (a_periodo_lag >= periodIni_stage)
				return varQDLAG;

		} // if (sobreposicao_periodo_otimizacao > 0.0) {

		// ADD any deterministic (past) compound of QD.
		// Only in stage where a_period belongs
		if (((a_periodo_lag < periodIni_stageIni) && (stageIni == a_idEstagio)) || (a_periodo_lag + 1 <= periodIni_stageIni)) {

			const SmartEnupla<Periodo, double> vazao_defluencia = a_dados.getVetor(a_idHidreletrica, IdDefluencia_passada, AttVetorDefluencia_vazao_defluencia, Periodo(), double());

			if (vazao_defluencia.getIteradorInicial() > a_periodo_lag)
				throw std::invalid_argument("Necessario mais periodos de defluencia passada anteriores a " + getString(a_periodo_lag) + " em " + getFullString(AttVetorDefluencia_vazao_defluencia) + " de " + getFullString(a_idHidreletrica));

			bool sobreposicao_encontrada = false;
			for (Periodo periodo = vazao_defluencia.getIteradorInicial(); periodo <= vazao_defluencia.getIteradorFinal(); vazao_defluencia.incrementarIterador(periodo)) {

				const double sobreposicao = a_periodo_lag.sobreposicao(periodo);

				if (sobreposicao > 0.0) {

					int varQD = getVarDecisao_QDseExistir(a_TSS, a_idEstagio, periodo, a_idHidreletrica);

					if (varQD == -1)
						varQD = addVarDecisao_QD(a_TSS, a_idEstagio, periodo, a_idHidreletrica, vazao_defluencia.at(periodo), vazao_defluencia.at(periodo), 0.0);

					int equQDLAG = getEquLinear_QD_LAGseExistir(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idHidreletrica);

					if (equQDLAG == -1) {
						equQDLAG = addEquLinear_QD_LAG(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idHidreletrica);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDLAG, equQDLAG, 1.0);
					}

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQD, equQDLAG, -sobreposicao);

					if (!sobreposicao_encontrada)
						sobreposicao_encontrada = true;

				} // if (sobreposicao > 0.0) {
				else if ((sobreposicao == 0.0) && (sobreposicao_encontrada))
					break;

			} // for (Periodo periodo = vazao_defluencia.getIteradorInicial(); periodo <= vazao_defluencia.getIteradorFinal(); vazao_defluencia.incrementarIterador(periodo)) {

			// All QD of QDLAG are deterministic
			if (a_periodo_lag + 1 <= periodIni_stageIni)
				return varQDLAG;

		} // if ((a_periodo_lag < periodIni_stageIni) && (stageIni == IdEstagio_1) && (periodIni_stage <= a_periodo) && (a_periodo <= periodEnd_stage)) {

		// Recursively ADD any compound of QD in past stages.
		const IdEstagio idEstagio_anterior = IdEstagio(a_idEstagio - 1);

		int varQDLAG_anterior = -1;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica) {
			for (TipoSubproblemaSolver tss = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tss < TipoSubproblemaSolver_Excedente; tss++) {
				if (tss != TipoSubproblemaSolver_viabilidade_hidraulica)
					varQDLAG_anterior = criarVariaveisDecisao_VariaveisEstado_Restricoes_QDLAG(tss, a_dados, idEstagio_anterior, a_periodo, a_idHidreletrica, a_periodo_lag);
				if (varQDLAG_anterior > -1)
					break;
			}
		}
		else
			varQDLAG_anterior = criarVariaveisDecisao_VariaveisEstado_Restricoes_QDLAG(a_TSS, a_dados, idEstagio_anterior, a_periodo, a_idHidreletrica, a_periodo_lag);

		if (varQDLAG_anterior > -1){

			if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
				varQDLAG_anterior = -1;

			// Variáveis de estado a compor lag
			if (sobreposicao_periodo_otimizacao > 0.0) {
				const int varQDLAG_ADD = addVarDecisao_QDLAG_ADD(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idHidreletrica, 0.0, infinito, 0.0);
				vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, getNomeVarDecisao_QDLAG(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idHidreletrica), varQDLAG_ADD, varQDLAG_anterior);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDLAG_ADD, equQDLAG, -1.0);
			}

			// Variáveis de estado a repassar lag
			else
				vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, getNomeVarDecisao_QDLAG(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idHidreletrica), varQDLAG, varQDLAG_anterior);

		} // if (varQDLAG_anterior > -1){

		return varQDLAG;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisDecisao_VariaveisEstado_Restricoes_QDLAG(a_dados," + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getString(a_periodo) + "," + getFullString(a_idHidreletrica) + "," + getString(a_periodo_lag) + "): \n" + std::string(erro.what())); }

} // int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_QDLAG(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo, const IdHidreletrica a_idHidreletrica) {

int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_YP(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo, const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria, Periodo &a_periodo_lag, const double a_grau_liberdade, std::vector<IdHidreletrica> a_listaIdHidreletrica) {

	try {

		if ((a_idProcessoEstocastico != IdProcessoEstocastico_hidrologico_hidreletrica) && (a_idProcessoEstocastico != IdProcessoEstocastico_hidrologico_bacia))
			throw std::invalid_argument("O argumento processo estocastico deve ser hidrologico");

		const IdEstagio menor_estagio = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());

		if (a_idEstagio < menor_estagio)
			return -1;

		if ((!vetorEstagio.at(a_idEstagio).isSolverInstanciado(a_TSS)) && (a_TSS == TipoSubproblemaSolver_mestre))
			return -1;

		const Periodo periodo_otimizacao = getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

		const Periodo periodo_final_proc_estoc_hidrologico = getIterador2Final(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, a_idEstagio, Periodo());

		// Verifica se periodo não é mais necessario para compor ou repassar lag
		if (periodo_final_proc_estoc_hidrologico + 1 <= a_periodo_lag)
			return -1;

		const SmartEnupla<Periodo, int> proc_estoc_hidrologico = getElementosMatriz(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, a_idEstagio, Periodo(), int());
		const Periodo periodo_inicial_proc_estoc_hidrologico = proc_estoc_hidrologico.getIteradorInicial();

		const IdProcessoEstocastico idProcessoEstocastico_modelo = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

		int varYP = -1;

		std::vector<IdVariavelAleatoria> idVarEquiv;
		std::vector<std::vector<IdVariavelAleatoriaInterna>> idVarIntEquiv;

		// Assume-se que processo estocastico já instanciado no modelo é identico ao processo estocastico repassado via argumento. 
		if (a_listaIdHidreletrica.size() == 0) {
			varYP = getVarDecisao_YPseExistir(a_TSS, a_idEstagio, periodo_otimizacao, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria);
			if (varYP > -1)
				return varYP;
			idVarEquiv.push_back(a_idVariavelAleatoria);
			idVarIntEquiv.push_back(std::vector<IdVariavelAleatoriaInterna>());
			for (IdVariavelAleatoriaInterna idVarInt = IdVariavelAleatoriaInterna_1; idVarInt <= getMaiorId(a_idProcessoEstocastico, a_idVariavelAleatoria, IdVariavelAleatoriaInterna()); idVarInt++)
				idVarIntEquiv.at(0).push_back(idVarInt);
		}
		// Deve ser encontrada a equivalência entre processo estocastico já instanciado no modelo e processo estocastico repassado via argumento.
		else {
			for (int i = 0; i < int(a_listaIdHidreletrica.size()); i++) {
				IdVariavelAleatoria idVar; IdVariavelAleatoriaInterna idVarInt;
				getIdVariavelAleatoriaIdVariavelAleatoriaInternaFromIdHidreletrica(idProcessoEstocastico_modelo, idVar, idVarInt, a_listaIdHidreletrica.at(i));
				if (idVarEquiv.size() == 0) {
					idVarEquiv.push_back(idVar);
					idVarIntEquiv.push_back(std::vector<IdVariavelAleatoriaInterna>(1, idVarInt));
				}
				else {
					for (int j = 0; i < int(idVarEquiv.size()); j++) {
						if (idVarEquiv.at(j) == idVar) {
							idVarIntEquiv.at(i).push_back(idVarInt);
							break;
						}
						else if (i == int(idVarEquiv.size()) - 1) {
							idVarEquiv.push_back(idVar);
							idVarIntEquiv.push_back(std::vector<IdVariavelAleatoriaInterna>(1, idVarInt));
						}
					}

				}
			}
			// Verifica se processo estocastico instanciado é semelhante ao repassado via argumento
			// (Isso implica que uma unica variavel aleatoria em ambos mapeiam as mesmas hidreletricas)
			if (idVarEquiv.size() == 1) {
				if (vectorCompara(a_listaIdHidreletrica, getIdHidreletricaFromIdProcessoEstocasticoIdVariavelAleatoria(idProcessoEstocastico_modelo, idVarEquiv.at(0))))
					varYP = getVarDecisao_YPseExistir(a_TSS, a_idEstagio, periodo_otimizacao, a_periodo_lag, idProcessoEstocastico_modelo, idVarEquiv.at(0));
				if (varYP > -1)
					return varYP;
			}
			else if (idVarEquiv.size() == 0)
				throw std::invalid_argument("Nao foi encontrada equivalencia entre processo estocastico instanciado e processo estocastico repassado via argumento.");
		}

		varYP = getVarDecisao_YPseExistir(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria);
		if (varYP > -1)
			return varYP;

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();
		varYP = addVarDecisao_YP(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria, -infinito, infinito, 0.0);

		// Composicao de lag com periodos do horizonte de estudo
		int equYP = -1;
		double rhs_equYP = 0.0;
		const double sobreposicao_periodo_otimizacao = a_periodo_lag.sobreposicao(periodo_otimizacao);
		if (sobreposicao_periodo_otimizacao > 0.0) {

			equYP = addEquLinear_YP(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYP, equYP, 1.0);

			bool sobreposicao_encontrada = false;
			for (Periodo periodo = periodo_inicial_proc_estoc_hidrologico; periodo <= periodo_final_proc_estoc_hidrologico; proc_estoc_hidrologico.incrementarIterador(periodo)) {

				const double sobreposicao = a_periodo_lag.sobreposicao(periodo);

				if (sobreposicao > 0.0) {

					for (int i = 0; i < int(idVarIntEquiv.size()); i++) {

						double grau_liberdade = 0.0;
						double coeficiente_participacao = 0.0;
						for (int j = 0; j < int(idVarIntEquiv.at(i).size()); j++) {
							coeficiente_participacao += getElementoVetor(idProcessoEstocastico_modelo, idVarEquiv.at(i), idVarIntEquiv.at(i).at(j), AttVetorVariavelAleatoriaInterna_coeficiente_participacao, periodo, double());
							grau_liberdade += getAtributo(idProcessoEstocastico_modelo, idVarEquiv.at(i), idVarIntEquiv.at(i).at(j), AttComumVariavelAleatoriaInterna_grau_liberdade, double());
						}

						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_YP(a_TSS, a_idEstagio, periodo_otimizacao, periodo, idProcessoEstocastico_modelo, idVarEquiv.at(i)), equYP, -sobreposicao * coeficiente_participacao);
						rhs_equYP += (a_grau_liberdade - grau_liberdade) * sobreposicao;
					}

					if (!sobreposicao_encontrada)
						sobreposicao_encontrada = true;
				}
				else if ((sobreposicao == 0.0) && (sobreposicao_encontrada))
					break;

			} // for (Periodo periodo = periodo_inicial_proc_estoc_hidrologico; periodo <= periodo_final_proc_estoc_hidrologico; proc_estoc_hidrologico.incrementarIterador(periodo)) {

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(equYP, rhs_equYP);

		} // if (sobreposicao_periodo_otimizacao > 0.0) {

		// No estagio 1, variáveis de estado são criadas para as variáveis YH pré-estudo.
		if (a_idEstagio == IdEstagio_1) {

			std::vector<IdHidreletrica> listaIdHidreletrica = a_listaIdHidreletrica;
			if (listaIdHidreletrica.size() == 0)
				listaIdHidreletrica = getIdHidreletricaFromIdProcessoEstocasticoIdVariavelAleatoria(a_idProcessoEstocastico, a_idVariavelAleatoria);

			IdVariavelAleatoria idVar; IdVariavelAleatoriaInterna idVarInt;
			getIdVariavelAleatoriaIdVariavelAleatoriaInternaFromIdHidreletrica(idProcessoEstocastico_modelo, idVar, idVarInt, listaIdHidreletrica.at(0));

			const SmartEnupla<Periodo, double> afluencia_tendencia = getVetor(idProcessoEstocastico_modelo, idVar, idVarInt, AttVetorVariavelAleatoriaInterna_tendencia_temporal, Periodo(), double());
			if (afluencia_tendencia.getIteradorInicial() > a_periodo_lag)
				throw std::invalid_argument("Necessario mais periodos de defluencia passada anteriores a " + getString(a_periodo_lag) + " em " + getFullString(AttVetorVariavelAleatoriaInterna_tendencia_temporal) + " de " + getFullString(idProcessoEstocastico_modelo) + " de " + getFullString(idVar) + " de " + getFullString(idVarInt));

			bool sobreposicao_encontrada = false;
			for (Periodo periodo = afluencia_tendencia.getIteradorInicial(); periodo <= afluencia_tendencia.getIteradorFinal(); afluencia_tendencia.incrementarIterador(periodo)) {

				const double sobreposicao = a_periodo_lag.sobreposicao(periodo);

				if (sobreposicao > 0.0) {

					double grau_liberdade = 0.0;
					for (int i = 0; i < int(listaIdHidreletrica.size()); i++) {

						const IdHidreletrica idUHE = listaIdHidreletrica.at(i);
						getIdVariavelAleatoriaIdVariavelAleatoriaInternaFromIdHidreletrica(idProcessoEstocastico_modelo, idVar, idVarInt, idUHE);

						int varYH = getVarDecisao_YHseExistir(a_TSS, a_idEstagio, periodo, idUHE);

						if (varYH == -1) {
							if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {
								const double tendencia = getElementoVetor(idProcessoEstocastico_modelo, idVar, idVarInt, AttVetorVariavelAleatoriaInterna_tendencia_temporal, periodo, double());
								varYH = addVarDecisao_YH(a_TSS, a_idEstagio, periodo, idUHE, tendencia, tendencia, 0.0);
							}
							else
								varYH = addVarDecisao_YH(a_TSS, a_idEstagio, periodo, idUHE, -infinito, infinito, 0.0);
						}

						equYP = getEquLinear_YPseExistir(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria);

						if (equYP == -1) {
							equYP = addEquLinear_YP(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYP, equYP, 1.0);
						}

						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYH, equYP, -sobreposicao);

					} // for (int i = 0; i < int(a_listaIdHidreletrica.size()); i++) {

					rhs_equYP += (a_grau_liberdade)*sobreposicao;

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(equYP, rhs_equYP);

					if (!sobreposicao_encontrada)
						sobreposicao_encontrada = true;

				} // if (sobreposicao > 0.0) {

				else if ((sobreposicao == 0.0) && (sobreposicao_encontrada))
					break;

			} // for (Periodo periodo = afluencia_tendencia.getIteradorInicial(); periodo <= afluencia_tendencia.getIteradorFinal(); afluencia_tendencia.incrementarIterador(periodo)) {

		} // if (a_idEstagio == IdEstagio_1) {

		else if (a_idEstagio > IdEstagio_1) {

			const IdEstagio idEstagio_anterior = IdEstagio(a_idEstagio - 1);

			int varYP_anterior = -1;

			if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica) {
				for (TipoSubproblemaSolver tss = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tss < TipoSubproblemaSolver_Excedente; tss++) {
					if (tss != TipoSubproblemaSolver_viabilidade_hidraulica)
						varYP_anterior = criarVariaveisDecisao_VariaveisEstado_Restricoes_YP(tss, a_dados, idEstagio_anterior, a_periodo, a_idProcessoEstocastico, a_idVariavelAleatoria, a_periodo_lag, a_grau_liberdade, a_listaIdHidreletrica);
					if (varYP_anterior > -1)
						break;
				}
			}
			else
				varYP_anterior = criarVariaveisDecisao_VariaveisEstado_Restricoes_YP(a_TSS, a_dados, idEstagio_anterior, a_periodo, a_idProcessoEstocastico, a_idVariavelAleatoria, a_periodo_lag, a_grau_liberdade, a_listaIdHidreletrica);

			if ((varYP_anterior > -1) || ((idEstagio_anterior == menor_estagio) && (periodo_inicial_proc_estoc_hidrologico > a_periodo_lag))) {

				if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
					varYP_anterior = -1;

				// Variáveis de estado a repassar lag
				if (sobreposicao_periodo_otimizacao == 0.0)
					vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeVarDecisao_YP(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria) + "," + getString(a_grau_liberdade) + "," + getStringFromLista(a_listaIdHidreletrica, ",", false)), varYP, varYP_anterior);

				// Variáveis de estado a compor lag
				else if (sobreposicao_periodo_otimizacao > 0.0) {
					const int varYP_ADD = addVarDecisao_YP_ADD(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria, -infinito, infinito, 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYP_ADD, equYP, -1.0);
					vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeVarDecisao_YP(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria) + "," + getString(a_grau_liberdade) + "," + getStringFromLista(a_listaIdHidreletrica, ",", false)), varYP_ADD, varYP_anterior);
				}

			} // if ((varYP_anterior > -1) || ((idEstagio_anterior == menor_estagio) && (periodo_inicial_proc_estoc_hidrologico > a_idVariavelAleatoria))) {
		} // else if (a_idEstagio > IdEstagio_1) {

		return varYP;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisDecisao_VariaveisEstado_Restricoes_YP(a_dados," + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getString(a_periodo) + "," + getFullString(a_idProcessoEstocastico) + "," + getFullString(a_idVariavelAleatoria) + "," + getString(a_periodo_lag) + "," + getString(a_grau_liberdade) + "): \n" + std::string(erro.what())); }

}

int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_PTDISPCOM(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo, const IdPatamarCarga a_idPat, const IdTermeletrica a_idTermeletrica, const double a_potencia_disponivel_minima, const double a_potencia_disponivel_maxima) {

	try {

		const IdEstagio menor_estagio = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());

		if (a_idEstagio < menor_estagio) {
			if (menor_estagio == IdEstagio_1)
				return -1;
			else
				return -2;
		}

		if ((!vetorEstagio.at(a_idEstagio).isSolverInstanciado(a_TSS)) || (a_TSS == TipoSubproblemaSolver_mestre) || (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica))
			return -1;

		const int lag_mensal_potencia_comandada = a_dados.getAtributo(a_idTermeletrica, AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada, int());
		if (lag_mensal_potencia_comandada == 0)
			throw std::invalid_argument(getFullString(a_idTermeletrica) + " com atributo " + getFullString(AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada) + " igual a zero.");

		// Quando patamar é inexistente, cria-se PTDISPCOMLAG recursivamente
		if (a_idPat == IdPatamarCarga_Nenhum) {

			const Periodo periodo_comando_minuto = Periodo(TipoPeriodo_minuto, a_periodo);
			const Periodo periodo_otimizacao = getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

			int varPTDISPCOM = getVarDecisao_PTDISPCOMseExistir(a_TSS, a_idEstagio, a_periodo, a_idTermeletrica);

			if (varPTDISPCOM > -1)
				return varPTDISPCOM;
			else if ((periodo_comando_minuto.sobreposicao(periodo_otimizacao) == 0.0) && ((periodo_otimizacao < periodo_comando_minuto) && (periodo_comando_minuto < (Periodo(TipoPeriodo_minuto, getAtributo(getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio()), AttComumEstagio_periodo_otimizacao, Periodo()) + 1) - 1))))
				return -1;

			varPTDISPCOM = addVarDecisao_PTDISPCOM(a_TSS, a_idEstagio, a_periodo, a_idTermeletrica, 0.0, vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);

			const int varPTDISPCOM_anterior = criarVariaveisDecisao_VariaveisEstado_Restricoes_PTDISPCOM(a_TSS, a_dados, IdEstagio(a_idEstagio - 1), a_periodo, a_idPat, a_idTermeletrica, a_potencia_disponivel_minima, a_potencia_disponivel_maxima);

			if (varPTDISPCOM_anterior < 0) {
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(varPTDISPCOM, a_potencia_disponivel_minima);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(varPTDISPCOM, a_potencia_disponivel_maxima);
			}
			
			if (varPTDISPCOM_anterior != -1)
				vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeVarDecisao_PTDISPCOM(a_TSS, a_idEstagio, a_periodo, a_idTermeletrica) + "," + getString(a_potencia_disponivel_minima) + "," + getString(a_potencia_disponivel_maxima)), varPTDISPCOM, varPTDISPCOM_anterior);

			return varPTDISPCOM;

		} // if (a_idPat == IdPatamarCarga_Nenhum) {

		// Quando patamar é existente, cria-se PTDISPCOMLAG_pat e PTDISPCOMLAG recursivamente (caso necessário)

		int varPTDISPCOM_pat = getVarDecisao_PTDISPCOMseExistir(a_TSS, a_idEstagio, a_periodo, a_idPat, a_idTermeletrica);
		if (varPTDISPCOM_pat > -1)
			return varPTDISPCOM_pat;

		varPTDISPCOM_pat = addVarDecisao_PTDISPCOM(a_TSS, a_idEstagio, a_periodo, a_idPat, a_idTermeletrica, 0.0, vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);

		// Verifica incidencia de pré comando em dados de entrada
		Periodo ultimo_periodo_sobreposicao;
		double sobreposicao_acumulada = 0.0;
		if (a_dados.getSize1Matriz(a_idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_comandada) > 0) {

			const Periodo periodo_inicial_potencia_disponivel_comandada = a_dados.getIterador1Inicial(a_idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_comandada, Periodo());
			const Periodo periodo_final_potencia_disponivel_comandada = a_dados.getIterador1Final(a_idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_comandada, Periodo());

			if (((periodo_inicial_potencia_disponivel_comandada <= a_periodo) || (periodo_inicial_potencia_disponivel_comandada.sobreposicao(a_periodo) > 0.0)) && \
				((periodo_final_potencia_disponivel_comandada >= a_periodo) || (periodo_final_potencia_disponivel_comandada.sobreposicao(a_periodo) > 0.0))) {

				const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> horizonte_potencia_disponivel_comandada = a_dados.getMatriz(a_idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_comandada, Periodo(), IdPatamarCarga(), double());
				for (Periodo periodo = periodo_inicial_potencia_disponivel_comandada; periodo <= periodo_final_potencia_disponivel_comandada; horizonte_potencia_disponivel_comandada.incrementarIterador(periodo)) {

					const double sobreposicao = a_periodo.sobreposicao(periodo);

					if (sobreposicao > 0.0) {

						ultimo_periodo_sobreposicao = periodo;
						const double potencia_disponivel_comandada = horizonte_potencia_disponivel_comandada.at(periodo).at(a_idPat);

						// Com sobreposicao ~exata, restrição PTDISPCOM_pat não é criada
						if (doubleCompara(1e-6, sobreposicao, 1.0)) {
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(varPTDISPCOM_pat, potencia_disponivel_comandada);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(varPTDISPCOM_pat, potencia_disponivel_comandada);
							return varPTDISPCOM_pat;
						}

						int equPTDISPCOM_pat = getEquLinear_PTDISPCOMseExistir(a_TSS, a_idEstagio, a_periodo, a_idPat, a_idTermeletrica);

						if (equPTDISPCOM_pat == -1) {
							equPTDISPCOM_pat = addEquLinear_PTDISPCOM(a_TSS, a_idEstagio, a_periodo, a_idPat, a_idTermeletrica);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISPCOM_pat, equPTDISPCOM_pat, 1.0);
						}

						int varPTDISPPRECOM_pat = addVarDecisao_PTDISPPRECOM(a_TSS, a_idEstagio, periodo, a_idPat, a_idTermeletrica, potencia_disponivel_comandada, potencia_disponivel_comandada, 0.0);

						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISPPRECOM_pat, equPTDISPCOM_pat, -sobreposicao);

						sobreposicao_acumulada += sobreposicao;

						if (doubleCompara(1e-6, sobreposicao_acumulada, 1.0))
							return varPTDISPCOM_pat;

					}
					else if ((sobreposicao == 0.0) && (sobreposicao_acumulada > 0.0))
						break;

				} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

			}
		} // if (a_dados.getSize1Matriz(a_idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_comandada) > 0) {

		// Cria-se PTDISPCOMLAG recursivamente para complementar PTDISPCOMLAG_pat

		Periodo periodo_complementar_inicio = Periodo(TipoPeriodo_minuto, a_periodo);

		if (sobreposicao_acumulada > 0.0)
			periodo_complementar_inicio = Periodo(TipoPeriodo_minuto, ultimo_periodo_sobreposicao + 1);

		const Periodo periodo_complementar_final = Periodo(TipoPeriodo_minuto, a_periodo + 1) - 1;

		const IdMes idMes_inicio = periodo_complementar_inicio.getMes();
		const IdMes idMes_final = periodo_complementar_final.getMes();
		const IdAno idAno_inicio = periodo_complementar_inicio.getAno();
		const IdAno idAno_final = periodo_complementar_final.getAno();

		double sobreposicao_complementar = 1.0 - sobreposicao_acumulada;

		// Verifica se periodo complementar engloba um único mês (maxima performance)
		if ((idMes_inicio == idMes_final) && (idAno_inicio == idAno_final)) {

			Periodo periodo_comando = Periodo(idMes_inicio, idAno_inicio);

			const int varPTDISPCOM = criarVariaveisDecisao_VariaveisEstado_Restricoes_PTDISPCOM(a_TSS, a_dados, a_idEstagio, periodo_comando, IdPatamarCarga_Nenhum, a_idTermeletrica, a_potencia_disponivel_minima, a_potencia_disponivel_maxima);

			int equPTDISPCOM_pat = getEquLinear_PTDISPCOMseExistir(a_TSS, a_idEstagio, a_periodo, a_idPat, a_idTermeletrica);

			if (equPTDISPCOM_pat == -1) {
				equPTDISPCOM_pat = addEquLinear_PTDISPCOM(a_TSS, a_idEstagio, a_periodo, a_idPat, a_idTermeletrica);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISPCOM_pat, equPTDISPCOM_pat, 1.0);
			}

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISPCOM, equPTDISPCOM_pat, -sobreposicao_complementar);

			return varPTDISPCOM_pat;

		} // if ((idMes_inicio == idMes_final) && (idAno_inicio == idAno_final)) {

		// Verificacao generica do periodo complementar (performance depende do tamanho do período complementar)
		int contador_minuto = 1;
		IdMes idMes_ultimo = idMes_inicio;
		IdAno idAno_ultimo = idAno_inicio;
		for (Periodo periodo = periodo_complementar_inicio; periodo <= periodo_complementar_final + 1; periodo++) {

			const IdMes idMes = periodo.getMes();
			const IdAno idAno = periodo.getAno();

			if ((idMes != idMes_ultimo) || (idAno != idAno_ultimo)) {

				const double parcela_sobreposicao_complementar = double(contador_minuto)  / double(a_periodo.getMinutos());

				Periodo periodo_comando = Periodo(idMes_ultimo, idAno_ultimo);

				const int varPTDISPCOM = criarVariaveisDecisao_VariaveisEstado_Restricoes_PTDISPCOM(a_TSS, a_dados, a_idEstagio, periodo_comando, IdPatamarCarga_Nenhum, a_idTermeletrica, a_potencia_disponivel_minima, a_potencia_disponivel_maxima);

				int equPTDISPCOM_pat = getEquLinear_PTDISPCOMseExistir(a_TSS, a_idEstagio, a_periodo, a_idPat, a_idTermeletrica);

				if (equPTDISPCOM_pat == -1) {
					equPTDISPCOM_pat = addEquLinear_PTDISPCOM(a_TSS, a_idEstagio, a_periodo, a_idPat, a_idTermeletrica);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISPCOM_pat, equPTDISPCOM_pat, 1.0);
				}

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISPCOM, equPTDISPCOM_pat, -sobreposicao_complementar);

				sobreposicao_complementar -= parcela_sobreposicao_complementar;

				contador_minuto = 1;

				if (periodo == periodo_complementar_final + 1) {
					if (!doubleCompara(1e-6, sobreposicao_complementar, 0.0))
						throw std::invalid_argument("Erro calculo sobreposicao complementar.");

					return varPTDISPCOM_pat;

				}

			} // if ((idMes != idMes_ultimo) || (idAno != idAno_ultimo)) {


			idMes_ultimo = idMes;
			idAno_ultimo = idAno;

			contador_minuto++;

		}

		return -1;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisDecisao_VariaveisEstado_Restricoes_PTDISPCOM(a_dados," + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getString(a_periodo) + "," + getFullString(a_idPat) + "," + getFullString(a_idTermeletrica) + "," + getFullString(a_potencia_disponivel_minima) + "," + getFullString(a_potencia_disponivel_maxima) + "): \n" + std::string(erro.what())); }

} // int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_QDLAG(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo, const IdHidreletrica a_idHidreletrica) {


void ModeloOtimizacao::addCorteBendersToZF(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdEstagio a_idEstagio_anterior, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders) {

	try {

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		int posIneZF = getIneLinear_CB_ZFseExistir(a_TSS, a_idEstagio_anterior, a_idRealizacao, a_idCorteBenders);

		if ((posIneZF > -1) && (a_idCorteBenders != IdCorteBenders_1))
			throw std::invalid_argument("Corte de benders ja adicionado.");
		else if (posIneZF == -1)
			posIneZF = addIneLinear_CB_ZF(a_TSS, a_idEstagio_anterior, a_idRealizacao, a_idCorteBenders);

		vetorEstagio.at(a_idEstagio_anterior).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZF(a_TSS, a_idEstagio_anterior, a_idRealizacao), posIneZF, 1.0);

		setCorteBendersToZF(a_TSS, a_idEstagio, a_idEstagio_anterior, a_idRealizacao, a_idCorteBenders);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::addCorteBendersToZF(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idEstagio_anterior) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCorteBenders) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::addCorteBenders(const IdEstagio a_idEstagio, const IdCorteBenders a_idCorteBenders, const double a_rhs, const SmartEnupla<IdVariavelEstado, double>& a_coeficiente){


void ModeloOtimizacao::setCorteBendersToZF(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdEstagio a_idEstagio_anterior, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders) {

	try {

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const Periodo periodo_otimizacao_anterior = vetorEstagio.at(a_idEstagio_anterior).getAtributo(AttComumEstagio_periodo_otimizacao, Periodo());

		const int posIneZF = getIneLinear_CB_ZF(a_TSS, a_idEstagio_anterior, a_idRealizacao, a_idCorteBenders);

		for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= getMaiorId(a_idEstagio, IdVariavelEstado()); idVariavelEstado++) {

			if (vetorEstagio.at(a_idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

				const int idVariavelDecisaoEstagioAnterior = getAtributo(a_idEstagio, idVariavelEstado, AttComumVariavelEstado_idVariavelDecisaoEstagioAnterior, int());

				if (idVariavelDecisaoEstagioAnterior < 0)
					vetorEstagio.at(a_idEstagio).anularVariavelEstadoCorteBenders(idVariavelEstado);
				else
					vetorEstagio.at(a_idEstagio_anterior).getSolver(a_TSS)->setCofRestricao(idVariavelDecisaoEstagioAnterior, posIneZF, -getElementoMatriz(a_idEstagio, a_idCorteBenders, AttMatrizCorteBenders_coeficiente, a_idRealizacao, idVariavelEstado, double()));

			} // if (vetorEstagio.at(a_idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

			else
				vetorEstagio.at(a_idEstagio).anularVariavelEstadoCorteBenders(idVariavelEstado);

		} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= getMaiorId(a_idEstagio, IdVariavelEstado()); idVariavelEstado++) {

		vetorEstagio.at(a_idEstagio_anterior).getSolver(a_TSS)->setRHSRestricao(posIneZF, getElementoVetor(a_idEstagio, a_idCorteBenders, AttVetorCorteBenders_rhs, a_idRealizacao, double()));

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::setCorteBendersToZF(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idEstagio_anterior) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCorteBenders) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::setCorteBendersToZF(const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders, const int a_posicao_corte, const double a_rhs, const SmartEnupla<IdVariavelEstado, double>& a_coeficiente){


void ModeloOtimizacao::remCorteBendersFromZF(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdEstagio a_idEstagio_anterior, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders) {

	try {

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const int posIneZF = getIneLinear_CB_ZFseExistir(a_TSS, a_idEstagio_anterior, a_idRealizacao, a_idCorteBenders);

		if (posIneZF == -1)
			return;

		vetorEstagio.at(a_idEstagio_anterior).getSolver(a_TSS)->remRestricao(posIneZF);

		const int pos = getPosIneLinear_CB_ZF(a_TSS, a_idEstagio_anterior, a_idRealizacao, a_idCorteBenders);

		indx_IneLinear_CB_ZF_3.at(a_TSS).at(a_idEstagio_anterior).erase(indx_IneLinear_CB_ZF_3.at(a_TSS).at(a_idEstagio_anterior).begin() + pos);
		name_IneLinear_CB_ZF_3.at(a_TSS).at(a_idEstagio_anterior).erase(name_IneLinear_CB_ZF_3.at(a_TSS).at(a_idEstagio_anterior).begin() + pos);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::remCorteBendersFromZF(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio_anterior) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCorteBenders) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::remCorteBendersToZF(const IdEstagio a_idEstagio_anterior, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders){



void ModeloOtimizacao::addCorteBendersToZT(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders){

	try {

		int posIneZT = getIneLinear_CB_ZTseExistir(a_TSS, a_idEstagio, a_idRealizacao, a_idCorteBenders);

		if ((posIneZT > -1) && (a_idCorteBenders != IdCorteBenders_1))
			throw std::invalid_argument("Corte de benders ja adicionado.");
		else if (posIneZT == -1)
			posIneZT = addIneLinear_CB_ZT(a_TSS, a_idEstagio, a_idRealizacao, a_idCorteBenders);

		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZT(a_TSS, a_idEstagio, a_idRealizacao), posIneZT, 1.0);

		setCorteBendersToZT(a_TSS, a_idEstagio, a_idRealizacao, a_idCorteBenders);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::addCorteBendersToZT(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCorteBenders) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::addCorteBendersToZT(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders, const double a_rhs, const SmartEnupla<IdVariavelEstado, double>& a_coeficiente){

void ModeloOtimizacao::setCorteBendersToZT(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders){

	try {

		const int posIneZT = getIneLinear_CB_ZT(a_TSS, a_idEstagio, a_idRealizacao, a_idCorteBenders);

		for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= getMaiorId(a_idEstagio, IdVariavelEstado()); idVariavelEstado++) {

			if (vetorEstagio.at(a_idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

				const int idVariavelDecisao = getElementoVetor(a_idEstagio, idVariavelEstado, AttVetorVariavelEstado_idVariavelDecisao, a_TSS, int());

				if (idVariavelDecisao < 0)
					vetorEstagio.at(a_idEstagio).anularVariavelEstadoCorteBenders(idVariavelEstado);
				else
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(idVariavelDecisao, posIneZT, -getElementoMatriz(a_idEstagio, a_idCorteBenders, AttMatrizCorteBenders_coeficiente, a_idRealizacao, idVariavelEstado, double()));

			} // if (vetorEstagio.at(a_idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

			else
				vetorEstagio.at(a_idEstagio).anularVariavelEstadoCorteBenders(idVariavelEstado);

		} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= a_coeficiente.getIteradorFinal(); idVariavelEstado++) {

		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneZT, getElementoVetor(a_idEstagio, a_idCorteBenders, AttVetorCorteBenders_rhs, a_idRealizacao, double()));

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::setCorteBendersToZT(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCorteBenders) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::setCorteBendersToZT(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders, const double a_rhs, const SmartEnupla<IdVariavelEstado, double>& a_coeficiente){

void ModeloOtimizacao::remCorteBendersFromZT(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders){

	try {

		const int posIneZT = getIneLinear_CB_ZTseExistir(a_TSS, a_idEstagio, a_idRealizacao, a_idCorteBenders);

		if (posIneZT == -1)
			return;

		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->remRestricao(posIneZT);

		const int pos = getPosIneLinear_CB_ZF(a_TSS, a_idEstagio, a_idRealizacao, a_idCorteBenders);

		indx_IneLinear_CB_ZT_3.at(a_TSS).at(a_idEstagio).erase(indx_IneLinear_CB_ZT_3.at(a_TSS).at(a_idEstagio).begin() + pos);
		name_IneLinear_CB_ZT_3.at(a_TSS).at(a_idEstagio).erase(name_IneLinear_CB_ZT_3.at(a_TSS).at(a_idEstagio).begin() + pos);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::remCorteBendersFromZF(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCorteBenders) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::remCorteBendersFromZT(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders){


bool ModeloOtimizacao::isNecessarioInstanciarSolver(const IdEstagio a_idEstagio, const TipoSubproblemaSolver a_tipoSubproblemaSolver) {
	try {

		if (a_tipoSubproblemaSolver == TipoSubproblemaSolver_viabilidade_hidraulica) {

			if (!getAtributo(AttComumModeloOtimizacao_relaxar_afluencia_incremental_com_viabilidade_hidraulica, bool()))
				return false;

			const IdProcessoEstocastico idProcEstocastico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());
			SmartEnupla<Periodo, int> horizonte_processo_estocastico_hidrologico = getElementosMatriz(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, a_idEstagio, Periodo(), int());

			for (Periodo periodo = horizonte_processo_estocastico_hidrologico.getIteradorInicial(); periodo <= horizonte_processo_estocastico_hidrologico.getIteradorFinal(); horizonte_processo_estocastico_hidrologico.incrementarIterador(periodo)) {
				for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= getMaiorId(idProcEstocastico, IdVariavelAleatoria()); idVar++) {
					if (getElementoVetor(idProcEstocastico, idVar, AttVetorVariavelAleatoria_tipo_relaxacao, periodo, TipoRelaxacaoVariavelAleatoria()) != TipoRelaxacaoVariavelAleatoria_sem_relaxacao) {
						return true;
					}
				}
			}

			return false;

		} // if (a_tipoSubproblemaSolver == TipoSubproblemaSolver_viabilidade_hidraulica) {

		else if (a_tipoSubproblemaSolver == TipoSubproblemaSolver_geral) {

			return true;

		} // else if (a_tipoSubproblemaSolver == TipoSubproblemaSolver_geral) {

		else if ((a_tipoSubproblemaSolver == TipoSubproblemaSolver_mestre) && (getAtributo(AttComumModeloOtimizacao_calcular_custo_primal_via_subproblema_mestre, bool()))) {
			return true;
		}

		return false;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::isNecessarioInstanciarSolver(" + getFullString(a_idEstagio) + "," + getFullString(a_tipoSubproblemaSolver) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_vazao_afluente(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period)
{

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		const std::vector<IdRestricaoOperativaUHE> idRestricaoOperativaUHE_vazao_afluente = a_dados.vetorRestricaoOperativaUHE.getIdObjetos(AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica_vazao_afluente);

		const int numero_restricoes = int(idRestricaoOperativaUHE_vazao_afluente.size());

		for (int pos = 0; pos < numero_restricoes; pos++) {

			const IdRestricaoOperativaUHE idRestricaoOperativaUHE = idRestricaoOperativaUHE_vazao_afluente.at(pos);


			//////////////////////////////////////////////////////////////////////////////////////////
			//Restrição Hidráulica de Vazão Afluente (RHA)
			//Manual referencia DC Seção 5.2.5.2:
			//Amin <= Aflu + (Qmontante+Smontante) + Qdesvio <= Amax
			//Filosofia: cria uma restrição por periodo se existir um valor diferente a max no limite
			//////////////////////////////////////////////////////////////////////////////////////////

			//******************************************
			//Restrição <=
			//******************************************

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("min")) {

				if (getVarDecisao_RHA_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

					const int posIneRHA = addIneLinear_RHA_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHA, a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_period, IdPatamarCarga_1, double()));

					// Variável de folga RHA_FINF				
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHA_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), posIneRHA, 1.0);

					const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
					for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

						const IdHidreletrica	idUHE = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());

						const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_period, IdPatamarCarga());

						if (idUHE != IdHidreletrica_Nenhum) {

							// Parcela YH vazão incremental							
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_YH(a_TSS, a_idEstagio, a_period, idUHE), posIneRHA, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double()));


							// Parcela QD (Turbinamento + Vertimento das usinas hidrelétricas a montante)

							const int numero_hidreletricas_montante = a_dados.getSizeVetor(idUHE, AttVetorHidreletrica_montante);

							for (int m = 1; m <= numero_hidreletricas_montante; m++) {

								const IdHidreletrica idHidreletrica_montante = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_montante, m, IdHidreletrica());

								for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

									const double duracao_patamar = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, idPat, double());

									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD(a_TSS, a_idEstagio, a_period, idPat, idHidreletrica_montante), posIneRHA, duracao_patamar * a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, idPat, double()));

								}//// for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

							} // for (int m = 1; m <= numero_hidreletricas_montante; m++) {

							//QR
							// Variáveis de Hidreletricas à Montante desvio
							const int numero_hidreletricas_montante_desvio = a_dados.getSizeVetor(idUHE, AttVetorHidreletrica_montante_desvio);
							for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

								const IdHidreletrica idHidreletrica_montante_desvio = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_montante_desvio, m, IdHidreletrica());

								// Variável QR que "entra" na usina

								for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

									const double duracao_patamar = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, idPat, double());
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QR(a_TSS, a_idEstagio, a_period, idPat, idHidreletrica_montante_desvio, idUHE), posIneRHA, duracao_patamar * a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, idPat, double()));

								}//for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

							}//for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

							//Parcela dos intercámbios que "chegam" na hidrelétrica
							// hidreletrica_destino

							const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_destino = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_destino, idUHE);

							const int idIntercambioHidraulico_destino_size = int(idIntercambioHidraulico_destino.size());

							for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {

								const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

								if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

									for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

										const double duracao_patamar = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, idPat, double());

										if (getVarDecisao_QIseExistir(a_TSS, a_idEstagio, a_period, idPat, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI(a_TSS, a_idEstagio, a_period, idPat, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())), posIneRHA, duracao_patamar * a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, idPat, double()));

									}//for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

								}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

							}//for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {


							//Parcela de QI com porcentagem_retorno > 0 

							const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_origem = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_origem, idUHE);

							const int idIntercambioHidraulico_origem_size = int(idIntercambioHidraulico_origem.size());

							for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

								const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

								if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada && a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()) > 0) {

									for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

										const double duracao_patamar = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, idPat, double());

										if (getVarDecisao_QIseExistir(a_TSS, a_idEstagio, a_period, idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI(a_TSS, a_idEstagio, a_period, idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posIneRHA, duracao_patamar * a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()) * a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, idPat, double()));

									}//for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

								}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada && a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()) > 0) {

							}//for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

						} // if (idUHE != IdHidreletrica_Nenhum) {
						else
							throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

					} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

				} // if (getVarDecisao_RHA_FINF(a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_period, IdPatamarCarga_1, double()) > 0) {

			//******************************************
			//Restrição >=
			//******************************************

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("max")) {

				if (getVarDecisao_RHA_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

					const int posIneRHA = addIneLinear_RHA_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHA, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_period, IdPatamarCarga_1, double()));

					// Variável de folga RHA_FSUP					
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHA_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), posIneRHA, 1.0);

					const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
					for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

						const IdHidreletrica	idUHE = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());

						const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_period, IdPatamarCarga());

						if (idUHE != IdHidreletrica_Nenhum) {

							// Parcela YH vazão incremental
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_YH(a_TSS, a_idEstagio, a_period, idUHE), posIneRHA, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double()));

							// Parcela QD (Turbinamento + Vertimento das usinas hidrelétricas a montante)

							const int numero_hidreletricas_montante = a_dados.getSizeVetor(idUHE, AttVetorHidreletrica_montante);

							for (int m = 1; m <= numero_hidreletricas_montante; m++) {

								const IdHidreletrica idHidreletrica_montante = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_montante, m, IdHidreletrica());

								for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

									const double duracao_patamar = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, idPat, double());

									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD(a_TSS, a_idEstagio, a_period, idPat, idHidreletrica_montante), posIneRHA, -duracao_patamar * a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, idPat, double()));

								}//// for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

							} // for (int m = 1; m <= numero_hidreletricas_montante; m++) {

							//QR
							// Variáveis de Hidreletricas à Montante desvio
							const int numero_hidreletricas_montante_desvio = a_dados.getSizeVetor(idUHE, AttVetorHidreletrica_montante_desvio);
							for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

								const IdHidreletrica idHidreletrica_montante_desvio = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_montante_desvio, m, IdHidreletrica());

								// Variável QR que "entra" na usina

								for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

									const double duracao_patamar = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, idPat, double());
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QR(a_TSS, a_idEstagio, a_period, idPat, idHidreletrica_montante_desvio, idUHE), posIneRHA, -duracao_patamar * a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, idPat, double()));

								}//for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

							}//for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

							//Parcela dos intercámbios que "chegam" na hidrelétrica
							// hidreletrica_destino

							const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_destino = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_destino, idUHE);

							const int idIntercambioHidraulico_destino_size = int(idIntercambioHidraulico_destino.size());

							for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {

								const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

								if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

									for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

										const double duracao_patamar = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, idPat, double());

										if (getVarDecisao_QIseExistir(a_TSS, a_idEstagio, a_period, idPat, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI(a_TSS, a_idEstagio, a_period, idPat, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())), posIneRHA, -duracao_patamar * a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, idPat, double()));

									}//for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

								}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

							}//for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {

							//Parcela de QI com porcentagem_retorno > 0 

							const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_origem = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_origem, idUHE);

							const int idIntercambioHidraulico_origem_size = int(idIntercambioHidraulico_origem.size());

							for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

								const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

								if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada && a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()) > 0) {

									for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

										const double duracao_patamar = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_period, idPat, double());

										if (getVarDecisao_QIseExistir(a_TSS, a_idEstagio, a_period, idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI(a_TSS, a_idEstagio, a_period, idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posIneRHA, -duracao_patamar * a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()) * a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, idPat, double()));

									}//for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

								}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada && a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()) > 0) {

							}//for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

						} // if (idUHE != IdHidreletrica_Nenhum) {
						else
							throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

					} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

				} // if (getVarDecisao_RHA_FSUP(a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_period, IdPatamarCarga_1, double()) < getdoubleFromChar("max")) {

		}// for (int pos = 0; pos < numero_restricoes; pos++) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::criarRestricoesHidraulicaEspecial_vazao_afluente(" + getFullString(a_TSS) + ",Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados): \n" + std::string(erro.what())); }

}//void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_vazao_afluente(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados)


void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_vazao_defluente(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period, const IdPatamarCarga a_idPat) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		const std::vector<IdRestricaoOperativaUHE> idRestricaoOperativaUHE_vazao_defluente = a_dados.vetorRestricaoOperativaUHE.getIdObjetos(AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica_vazao_defluente);

		const int numero_restricoes = int(idRestricaoOperativaUHE_vazao_defluente.size());

		for (int pos = 0; pos < numero_restricoes; pos++) {

			const IdRestricaoOperativaUHE idRestricaoOperativaUHE = idRestricaoOperativaUHE_vazao_defluente.at(pos);


			const SmartEnupla<Periodo, double> horizonte_estudo_estagio = a_dados.getElementosMatriz(AttMatrizDados_percentual_duracao_horizonte_estudo, a_idEstagio, Periodo(), double());


			///////////////////////////////////////////////////////////////////
			//Restrição Hidráulica de Vazão Defluente (RHQ)
			//Manual referencia Seção 5.2.5.2:
			//Qmin <= Qdefluencia + Qdesvio + Qbombeada <= Qmax
			///////////////////////////////////////////////////////////////////


			//******************************************
			//Restrição <=
			//******************************************

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_period, a_idPat, double()) != getdoubleFromChar("min")) {

				if (getVarDecisao_RHQ_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE) > -1) {

					const int posIneRHQ = addIneLinear_RHQ_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHQ, a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_period, a_idPat, double()));

					// Variável RHQ_FINF
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHQ_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE), posIneRHQ, 1.0);

					const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
					for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

						const IdHidreletrica       idUHE = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
						const IdUsinaElevatoria idUsinaElevatoria = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());

						const TipoVariavelRestricaoOperativa tipoVariavelRestricaoOperativa = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

						if (idUHE != IdHidreletrica_Nenhum) {

							// Variável QD (Vazão defluente das usinas hidrelétricas)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_defluente) {

								if (a_dados.getAtributo(AttComumDados_representar_defluencia_disponivel_em_restricoes_hidraulicas, bool()))
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posIneRHQ, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));
								else
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posIneRHQ, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_defluente) {

							// Variável QT
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_turbinada) {

								if (a_dados.getAtributo(AttComumDados_representar_turbinamento_disponivel_em_restricoes_hidraulicas, bool())) {
									if (getVarDecisao_QTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posIneRHQ, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));
								}
								else {
									if (getVarDecisao_QTseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posIneRHQ, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));
								}

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_defluente) {

							// Variável QR
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_desviada) {

								// Variável QR que "sai" na usina
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QR(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, a_dados.getAtributo(idUHE, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posIneRHQ, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

								// hidreletrica_origem

								const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_origem = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_origem, idUHE);

								const int idIntercambioHidraulico_origem_size = int(idIntercambioHidraulico_origem.size());

								for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

									const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

									if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

										if (getVarDecisao_QIseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())), posIneRHQ, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

									}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

								}//for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_desviada) {

						} // if (idUHE != IdHidreletrica_Nenhum) {
						else if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {

							// Variável QB (Vazao bombeada)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_bombeada) {

								if (getVarDecisao_QBseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria) > -1)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QB(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria), posIneRHQ, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_bombeado) {

						}//else if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {
						else
							throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

					} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

				} // if (getVarDecisao_RHQseExistir(a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE) > -1) {

			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_period, a_idPat, double()) > 0) {

			//******************************************
			//Restrição >=
			//******************************************

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_period, a_idPat, double()) != getdoubleFromChar("max")) {

				if (getVarDecisao_RHQ_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE) > -1) {

					const int posIneRHQ = addIneLinear_RHQ_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHQ, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_period, a_idPat, double()));

					// Variável RHQ_FSUP
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHQ_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE), posIneRHQ, 1.0);

					const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
					for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

						const IdHidreletrica       idUHE = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
						const IdUsinaElevatoria idUsinaElevatoria = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());

						const TipoVariavelRestricaoOperativa tipoVariavelRestricaoOperativa = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

						if (idUHE != IdHidreletrica_Nenhum) {

							// Variável QD (Vazão defluente das usinas hidrelétricas)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_defluente) {

								if (a_dados.getAtributo(AttComumDados_representar_defluencia_disponivel_em_restricoes_hidraulicas, bool()))
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posIneRHQ, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));
								else
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posIneRHQ, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_defluente) {

							// Variável QT
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_turbinada) {

								if (a_dados.getAtributo(AttComumDados_representar_turbinamento_disponivel_em_restricoes_hidraulicas, bool())) {
									if (getVarDecisao_QTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posIneRHQ, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));
								}
								else {
									if (getVarDecisao_QTseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QT(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posIneRHQ, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));
								}
							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_defluente) {

							// Variável QR
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_desviada) {

								//QR
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QR(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, a_dados.getAtributo(idUHE, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posIneRHQ, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

								// hidreletrica_origem

								const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_origem = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_origem, idUHE);

								const int idIntercambioHidraulico_origem_size = int(idIntercambioHidraulico_origem.size());

								for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

									const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

									if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

										if (getVarDecisao_QIseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())), posIneRHQ, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

									}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

								}//for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_desviada) {

						} // if (idUHE != IdHidreletrica_Nenhum) {
						else if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {

							// Variável QB (Vazão bombeada)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_bombeada) {

								if (getVarDecisao_QBseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria) > -1)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QB(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria), posIneRHQ, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_bombeado) {

						}//else if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {
						else
							throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

					} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

				} // if (getVarDecisao_RHQseExistir(a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE) > -1) {

			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_period, a_idPat, double()) > getdoubleFromChar("max")) {


		}// for (int pos = 0; pos < numero_restricoes; pos++) {

	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("criarRestricoesHidraulicaEspecial_vazao_defluente(" + getFullString(a_TSS) + ",Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados): \n" + std::string(erro.what())); }

}//void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_vazao_defluente(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados)


void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_volume_armazenado(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodIni_stage, Periodo& a_periodEnd_stage, Periodo &a_period, Periodo& a_periodNext, const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>& a_horizon) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		const std::vector<IdRestricaoOperativaUHE> idRestricaoOperativaUHE_volume_armazenado = a_dados.vetorRestricaoOperativaUHE.getIdObjetos(AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica_volume_armazenado);

		const int numero_restricoes = int(idRestricaoOperativaUHE_volume_armazenado.size());

		for (int pos = 0; pos < numero_restricoes; pos++) {

			const IdRestricaoOperativaUHE idRestricaoOperativaUHE = idRestricaoOperativaUHE_volume_armazenado.at(pos);

			Periodo periodo_inicial_dos_dados = a_periodIni_stage;

			if (periodo_inicial_dos_dados < a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_periodo_inicio, Periodo())) //Condição caso a restrição comece em um periodo "intermediário" do estágio SPT 
				periodo_inicial_dos_dados = a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_periodo_inicio, Periodo());


			/////////////////////////////////////////////////////////////////////
			//Restrição Hidráulica de Volume Armazenado (RHV)
			//Manual referencia Seção 5.2.5.3:
			//Vmin <= c*Qdefluencia + c*Qdesvio + c*Qbombeada + V <= Vmax
			/////////////////////////////////////////////////////////////////////

			//Nota: Os coeficientes de participação (coeficiente de cada variável na restrição) das variáveis associadas a turbinamentos 
			//é desconsiderado já que os Decks podem ter periodos diferentes aos considerados no SPT (o conversao_vazao é afetado)

			//******************************************
			//Restrição <=
			//******************************************

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("min")) {

				if (getVarDecisao_RHV_FINFseExistir(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

					const int posIneRHV = addIneLinear_RHV_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHV, a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_period, IdPatamarCarga_1, double()));

					//Variável RHV_FINF				
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHV_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), posIneRHV, 1.0);

					const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
					for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

						const IdHidreletrica       idUHE = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
						const IdUsinaElevatoria idUsinaElevatoria = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());

						const TipoVariavelRestricaoOperativa tipoVariavelRestricaoOperativa = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

						if (idUHE != IdHidreletrica_Nenhum) {

							// Variável VI ou VF (Volume armazenado nos reservatórios)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_util) {

								if (a_period == a_periodEnd_stage) {

									if (getVarDecisao_VF(a_TSS, a_idEstagio, a_period, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_period, idUHE), posIneRHV, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double()));

								}//if (a_period == a_horizonte_estudo.getIteradorFinal()) {
								else {

									if (getVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, idUHE), posIneRHV, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double()));

								}//else {

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_util) {

							//Nota: A vazão defluente/desviada é o somatório de todas as vazões por periodo dentro de um estágio_DC

							// Variável QD (Volume defluente das usinas hidrelétricas)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_defluente) {

								for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizon.incrementarIterador(periodo_auxiliar)) {

									const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_auxiliar, IdPatamarCarga());

									for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD(a_TSS, a_idEstagio, periodo_auxiliar, idPat, idUHE), posIneRHV, a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPat, double()));

									}//// for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

								}//for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizonte_estudo.incrementarIterador(periodo_auxiliar)) {																													

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_defluente) {

							// Variável QR (Volume desviado)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_desviado) {

								for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizon.incrementarIterador(periodo_auxiliar)) {

									const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_auxiliar, IdPatamarCarga());

									for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

										//QR
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QR(a_TSS, a_idEstagio, a_period, idPat, idUHE, a_dados.getAtributo(idUHE, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posIneRHV, a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPat, double()));


										// hidreletrica_destino

										const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_destino = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_destino, idUHE);

										const int idIntercambioHidraulico_destino_size = int(idIntercambioHidraulico_destino.size());

										for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {

											const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

											if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

												if (getVarDecisao_QIseExistir(a_TSS, a_idEstagio, periodo_auxiliar, idPat, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())) > -1)
													vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI(a_TSS, a_idEstagio, periodo_auxiliar, idPat, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())), posIneRHV, a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPat, double()));

											}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

										}//for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {


										//Parcela de QI com porcentagem_retorno > 0 

										const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_origem = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_origem, idUHE);

										const int idIntercambioHidraulico_origem_size = int(idIntercambioHidraulico_origem.size());

										for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

											const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

											if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada && a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()) > 0) {

												if (getVarDecisao_QIseExistir(a_TSS, a_idEstagio, periodo_auxiliar, idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())) > -1)
													vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI(a_TSS, a_idEstagio, periodo_auxiliar, idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posIneRHV, a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()) * a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPat, double()));

											}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada && a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()) > 0) {

										}//for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

									}//// for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

								}//for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizonte_estudo.incrementarIterador(periodo_auxiliar)) {																													

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_desviado) {

						} // if (idUHE != IdHidreletrica_Nenhum) {
						else if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {

							//Nota: A vazão bombeada é o somatório de todas as vazões por periodo dentro de um estágio_DC

							// Variável QB (Volume bombeado)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_bombeado) {

								for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizon.incrementarIterador(periodo_auxiliar)) {

									const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_auxiliar, IdPatamarCarga());

									for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

										if (getVarDecisao_QBseExistir(a_TSS, a_idEstagio, periodo_auxiliar, idPat, idUsinaElevatoria) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QB(a_TSS, a_idEstagio, periodo_auxiliar, idPat, idUsinaElevatoria), posIneRHV, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, periodo_auxiliar, idPat, double()) * a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPat, double()));

									}//// for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

								}//for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizonte_estudo.incrementarIterador(periodo_auxiliar)) {																													

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_desviado) {

						}//if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {
						else
							throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

					} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

				} // if (getVarDecisao_RHVseExistir(a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_period, IdPatamarCarga_1, double()) > 0) {

			//******************************************
			//Restrição >=
			//******************************************

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("max")) {

				if (getVarDecisao_RHV_FSUPseExistir(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

					const int posIneRHV = addIneLinear_RHV_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHV, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_period, IdPatamarCarga_1, double()));

					// Variável RHV
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHV_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), posIneRHV, 1.0);

					const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
					for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

						const IdHidreletrica       idUHE = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
						const IdUsinaElevatoria idUsinaElevatoria = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());

						const TipoVariavelRestricaoOperativa tipoVariavelRestricaoOperativa = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

						if (idUHE != IdHidreletrica_Nenhum) {

							// Variável VF ou VI (Volume armazenado nos reservatórios)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_util) {

								if (a_period == a_periodEnd_stage) {

									if (getVarDecisao_VF(a_TSS, a_idEstagio, a_period, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_period, idUHE), posIneRHV, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double()));

								}//if (a_period == a_horizonte_estudo.getIteradorFinal()) {
								else {

									if (getVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, idUHE), posIneRHV, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double()));

								}//else {
							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_util) {

							//Nota: A vazão defluente/desviada é o somatório de todas as vazões por periodo dentro de um estágio_DC

							// Variável QD (Volume defluente das usinas hidrelétricas)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_defluente) {

								for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizon.incrementarIterador(periodo_auxiliar)) {

									const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_auxiliar, IdPatamarCarga());

									for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QD(a_TSS, a_idEstagio, periodo_auxiliar, idPat, idUHE), posIneRHV, -a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPat, double()));

									}//// for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

								}//for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizonte_estudo.incrementarIterador(periodo_auxiliar)) {																													

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_defluente) {

							// Intercambios Hidráulicos (Volume desviado)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_desviado) {

								for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizon.incrementarIterador(periodo_auxiliar)) {

									const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_auxiliar, IdPatamarCarga());

									for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

										//QR
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QR(a_TSS, a_idEstagio, a_period, idPat, idUHE, a_dados.getAtributo(idUHE, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posIneRHV, -a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPat, double()));

										// hidreletrica_destino

										const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_destino = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_destino, idUHE);

										const int idIntercambioHidraulico_destino_size = int(idIntercambioHidraulico_destino.size());

										for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {

											const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

											if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

												if (getVarDecisao_QIseExistir(a_TSS, a_idEstagio, periodo_auxiliar, idPat, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())) > -1)
													vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI(a_TSS, a_idEstagio, periodo_auxiliar, idPat, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())), posIneRHV, -a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPat, double()));

											}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

										}//for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {

										//Parcela de QI com porcentagem_retorno > 0 

										const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_origem = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_origem, idUHE);

										const int idIntercambioHidraulico_origem_size = int(idIntercambioHidraulico_origem.size());

										for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

											const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

											if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada && a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()) > 0) {

												if (getVarDecisao_QIseExistir(a_TSS, a_idEstagio, periodo_auxiliar, idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())) > -1)
													vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI(a_TSS, a_idEstagio, periodo_auxiliar, idPat, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posIneRHV, -a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPat, double()) * a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()));

											}//	if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada && a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()) > 0) {

										}//for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

									}//// for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

								}//for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizonte_estudo.incrementarIterador(periodo_auxiliar)) {																													

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_desviado) {

						} // if (idUHE != IdHidreletrica_Nenhum) {
						else if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {

							//Nota: A vazão bombeada é o somatório de todas as vazões por periodo dentro de um estágio_DC

							// Variável QB (Volume bombeado)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_bombeado) {

								for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizon.incrementarIterador(periodo_auxiliar)) {

									const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_auxiliar, IdPatamarCarga());

									for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

										if (getVarDecisao_QBseExistir(a_TSS, a_idEstagio, periodo_auxiliar, idPat, idUsinaElevatoria) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QB(a_TSS, a_idEstagio, periodo_auxiliar, idPat, idUsinaElevatoria), posIneRHV, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, periodo_auxiliar, idPat, double()) * a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPat, double()));

									}//// for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

								}//for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizonte_estudo.incrementarIterador(periodo_auxiliar)) {																													

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_desviado) {

						}//if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {
						else
							throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

					} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

				} // if (getVarDecisao_RHVseExistir(a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_period, IdPatamarCarga_1, double()) < getdoubleFromChar("max")) {

			//Atualiza o periodo onde começa o estágio da leitura dos dados
			if (a_period < a_periodEnd_stage) {//Signica um periodo que corresponde a o fim de um estagio_DC mas não do estagio_SPT
				periodo_inicial_dos_dados = a_period;
				a_horizon.incrementarIterador(periodo_inicial_dos_dados);
			}//if (a_period < a_horizonte_estudo.getIteradorFinal()) {	

		}// for (int pos = 0; pos < numero_restricoes; pos++) {

	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("criarRestricoesHidraulicaEspecial_volume_armazenado(" + getFullString(a_TSS) + ",Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados): \n" + std::string(erro.what())); }

}//void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_volume_armazenado(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados)

void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_energia_armazenada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodEnd_stage, Periodo &a_period, Periodo& a_periodNext) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		const std::vector<IdRestricaoOperativaUHE> idRestricaoOperativaUHE_energia_armazenada = a_dados.vetorRestricaoOperativaUHE.getIdObjetos(AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica_energia_armazenada);

		const int numero_restricoes = int(idRestricaoOperativaUHE_energia_armazenada.size());

		for (int pos = 0; pos < numero_restricoes; pos++) {

			const IdRestricaoOperativaUHE idRestricaoOperativaUHE = idRestricaoOperativaUHE_energia_armazenada.at(pos);

			/////////////////////////////////////////////////////////////////////
			//Restrição Hidráulica de Volume Armazenado (RHE)
			// %EAR_REE_max <= produtibilidade_acumulada_EAR * Volume
			// 
			// Nota: produtibilidade_acumulada_EAR é calculada com base nas regras de cálculo da conversão dos cortes
			/////////////////////////////////////////////////////////////////////
			//******************************************
			//Restrição <=
			//******************************************

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("min")) {

				const int posIneRHE = addIneLinear_RHE_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHE, a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_period, IdPatamarCarga_1, double()));

				//Variável RHE_FINF
				if (getVarDecisao_RHE_FINFseExistir(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHE_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), posIneRHE, 1.0);

				const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
				for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

					const IdHidreletrica       idUHE = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
					const TipoVariavelRestricaoOperativa tipoVariavelRestricaoOperativa = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

					if (idUHE != IdHidreletrica_Nenhum) {

						// Variável VI ou VF (Volume armazenado nos reservatórios)
						if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_util) {

							if (a_period == a_periodEnd_stage) {

								if (getVarDecisao_VF(a_TSS, a_idEstagio, a_period, idUHE) > -1)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_period, idUHE), posIneRHE, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double()));

							}//if (a_period == a_horizonte_estudo.getIteradorFinal()) {
							else {

								if (getVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, idUHE) > -1)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, idUHE), posIneRHE, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double()));

							}//else {

						}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_util) {

					} // if (idUHE != IdHidreletrica_Nenhum) {
					else
						throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

				} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){


			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_period, IdPatamarCarga_1, double()) > 0) {

			//******************************************
			//Restrição >=
			//******************************************

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("max")) {

				if (getVarDecisao_RHE_FSUPseExistir(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

					const int posIneRHE = addIneLinear_RHE_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHE, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_period, IdPatamarCarga_1, double()));

					// Variável RHE
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHE_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), posIneRHE, 1.0);

					const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
					for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

						const IdHidreletrica       idUHE = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
						const IdUsinaElevatoria idUsinaElevatoria = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());

						const TipoVariavelRestricaoOperativa tipoVariavelRestricaoOperativa = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

						if (idUHE != IdHidreletrica_Nenhum) {

							// Variável VF ou VI (Volume armazenado nos reservatórios)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_util) {

								if (a_period == a_periodEnd_stage) {

									if (getVarDecisao_VF(a_TSS, a_idEstagio, a_period, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_period, idUHE), posIneRHE, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double()));

								}//if (a_period == a_horizonte_estudo.getIteradorFinal()) {
								else {

									if (getVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, idUHE), posIneRHE, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double()));

								}//else {
							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_util) {

						} // if (idUHE != IdHidreletrica_Nenhum) {
						else
							throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

					} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

				} // if (getVarDecisao_RHEseExistir(a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_period, IdPatamarCarga_1, double()) < getdoubleFromChar("max")) {


		}// for (int pos = 0; pos < numero_restricoes; pos++) {

	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("criarRestricoesHidraulicaEspecial_energia_armazenada(" + getFullString(a_TSS) + ",Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados): \n" + std::string(erro.what())); }

}//void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_energia_armazenada(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados)


void ModeloOtimizacao::criarRestricoesIntercambioHidraulico(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period, const IdPatamarCarga a_idPat) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const IdIntercambioHidraulico idInterHIni = a_dados.getMenorId(IdIntercambioHidraulico());
		const IdIntercambioHidraulico idInterHOut = a_dados.getIdOut(IdIntercambioHidraulico());
		for (IdIntercambioHidraulico idIntercambioHidraulico = idInterHIni; idIntercambioHidraulico < idInterHOut; a_dados.vetorIntercambioHidraulico.incr(idIntercambioHidraulico)) {

			const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

			if (((tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada) || (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_pereira_barreto)) && a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo, a_period, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo, a_period, double()) > 0) {

				const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

				const int posEquQI_REL = addEquLinear_QI_REL(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQI_REL, 0.0);

				if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada) {
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI_REL(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico, a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posEquQI_REL, 1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico, a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posEquQI_REL, -1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QI_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico, a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posEquQI_REL, 1.0);
				}

				else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_pereira_barreto) {
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QILS_TRI_REL(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico), posEquQI_REL, 1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QILS_TRI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico), posEquQI_REL, -1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QILS_TRI_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambioHidraulico), posEquQI_REL, 1.0);
				}

			}//if (a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo, period, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo, period, double()) > 0) {

		}//for (IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico_1; idIntercambioHidraulico < a_dados.getMaiorId(IdIntercambioHidraulico()); idIntercambioHidraulico++) {


	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesIntercambioHidraulico(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }

}//void ModeloOtimizacao::criarRestricoesIntercambioHidraulico(Dados& a_dados, const IdEstagio a_idEstagio, const IdIntercambioHidraulico a_idIntercambioHidraulico, Periodo &a_period, const IdPatamarCarga a_idPat){

void ModeloOtimizacao::tratarVariaveisFolga(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		if (a_dados.getAtributo(AttComumDados_considerar_variaveis_folga, bool()))
			return;

			//Variáveis de folga associadas às hidrelétricas
		const IdHidreletrica idUHEIni = a_dados.getMenorId(IdHidreletrica());
		const IdHidreletrica idUHEOut = a_dados.getIdOut(IdHidreletrica());
			for (IdHidreletrica idUHE = idUHEIni; idUHE < idUHEOut; a_dados.vetorHidreletrica.incr(idUHE)) {

				//*********************
				//VARIAVEL_DECISAO_3
				//*********************

				
				if (getVarDecisao_QE_FINFseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1) {
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QE_FINF(a_TSS, a_idEstagio, a_period, idUHE), 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QE_FINF(a_TSS, a_idEstagio, a_period, idUHE), 0.0);
				}//if (getVarDecisao_QE_FINFseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1) {
				
				////////////

				if (getVarDecisao_QD_FINFseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1) {
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QD_FINF(a_TSS, a_idEstagio, a_period, idUHE), 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QD_FINF(a_TSS, a_idEstagio, a_period, idUHE), 0.0);
				}//if (getVarDecisao_QD_FINFseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1) {

				if (getVarDecisao_QD_FSUPseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1) {
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QD_FSUP(a_TSS, a_idEstagio, a_period, idUHE), 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QD_FSUP(a_TSS, a_idEstagio, a_period, idUHE), 0.0);
				}//if (getVarDecisao_QD_FSUPseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1) {

				////////////

				if (getVarDecisao_VF_FINFseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1) {
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_VF_FINF(a_TSS, a_idEstagio, a_period, idUHE), 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_VF_FINF(a_TSS, a_idEstagio, a_period, idUHE), 0.0);
				}//if (getVarDecisao_VF_FINFseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1) {

				if (getVarDecisao_VMORTO_FINFseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1) {
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_VMORTO_FINF(a_TSS, a_idEstagio, a_period, idUHE), 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_VMORTO_FINF(a_TSS, a_idEstagio, a_period, idUHE), 0.0);
				}//if (getVarDecisao_VMORTO_FINFseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1) {

				//*********************
				//VARIAVEL_DECISAO_4
				//*********************

				for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_period, IdPatamarCarga()); idPat++) {

					if (getVarDecisao_PH_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUHE) > -1) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_period, idPat, idUHE), 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_period, idPat, idUHE), 0.0);
					}//if (getVarDecisao_PH_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUHE) > -1) {

					////////////

					if (getVarDecisao_PHDISP_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUHE) > -1) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PHDISP_FINF(a_TSS, a_idEstagio, a_period, idPat, idUHE), 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PHDISP_FINF(a_TSS, a_idEstagio, a_period, idPat, idUHE), 0.0);
					}//if (getVarDecisao_PHDISP_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUHE) > -1) {

					////////////

					if (getVarDecisao_QTDISP_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUHE) > -1) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QTDISP_FINF(a_TSS, a_idEstagio, a_period, idPat, idUHE), 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QTDISP_FINF(a_TSS, a_idEstagio, a_period, idPat, idUHE), 0.0);
					}//if (getVarDecisao_QTDISP_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUHE) > -1) {

					////////////

					if (getVarDecisao_QT_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUHE) > -1) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QT_FINF(a_TSS, a_idEstagio, a_period, idPat, idUHE), 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QT_FINF(a_TSS, a_idEstagio, a_period, idPat, idUHE), 0.0);
					}//if (getVarDecisao_QT_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUHE) > -1) {

					////////////

					if (getVarDecisao_QD_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUHE) > -1) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QD_FINF(a_TSS, a_idEstagio, a_period, idPat, idUHE), 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QD_FINF(a_TSS, a_idEstagio, a_period, idPat, idUHE), 0.0);
					}//if (getVarDecisao_QD_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUHE) > -1) {

					if (getVarDecisao_QD_FSUPseExistir(a_TSS, a_idEstagio, a_period, idPat, idUHE) > -1) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QD_FSUP(a_TSS, a_idEstagio, a_period, idPat, idUHE), 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QD_FSUP(a_TSS, a_idEstagio, a_period, idPat, idUHE), 0.0);
					}//if (getVarDecisao_QD_FSUPseExistir(a_TSS, a_idEstagio, a_period, idPat, idUHE) > -1) {

					////////////

					if (getVarDecisao_QI_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUHE) > -1) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QI_FINF(a_TSS, a_idEstagio, a_period, idPat, idUHE), 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QI_FINF(a_TSS, a_idEstagio, a_period, idPat, idUHE), 0.0);
					}//if (getVarDecisao_QI_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUHE) > -1) {


					//*********************
					//VARIAVEL_DECISAO_5
					//*********************

					for (IdHidreletrica idHidreletrica_aux = idUHEIni; idHidreletrica_aux < idUHEOut; a_dados.vetorHidreletrica.incr(idHidreletrica_aux)) {

						if (getVarDecisao_QR_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUHE, idHidreletrica_aux) > -1) {
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QR_FINF(a_TSS, a_idEstagio, a_period, idPat, idUHE, idHidreletrica_aux), 0.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QR_FINF(a_TSS, a_idEstagio, a_period, idPat, idUHE, idHidreletrica_aux), 0.0);
						}//if (getVarDecisao_QI_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUHE) > -1) {

					}//for (IdHidreletrica idHidreletrica_aux = a_dados.getMenorId(IdHidreletrica()); idHidreletrica_aux < a_dados.getMaiorId(IdHidreletrica()); a_dados.vetorHidreletrica.incr(idHidreletrica_aux)) {


				}//for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_period, IdPatamarCarga()); idPat++) {


			}//for (IdHidreletrica idUHE = a_dados.getMenorId(IdHidreletrica()); idUHE < a_dados.getMaiorId(IdHidreletrica()); a_dados.vetorHidreletrica.incr(idUHE)) {

			//Variáveis de folga associadas às termelétricas
			const IdTermeletrica idUTEIni = a_dados.getMenorId(IdTermeletrica());
			const IdTermeletrica idUTEOut = a_dados.getIdOut(IdTermeletrica());
			for (IdTermeletrica idUTE = idUTEIni; idUTE < idUTEOut; a_dados.vetorTermeletrica.incr(idUTE)) {

				//*********************
				//VARIAVEL_DECISAO_4
				//*********************

				for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_period, IdPatamarCarga()); idPat++) {

					if (getVarDecisao_PT_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUTE) > -1) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PT_FINF(a_TSS, a_idEstagio, a_period, idPat, idUTE), 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PT_FINF(a_TSS, a_idEstagio, a_period, idPat, idUTE), 0.0);
					}//if (getVarDecisao_PT_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUTE) > -1) {

					if (getVarDecisao_PT_FSUPseExistir(a_TSS, a_idEstagio, a_period, idPat, idUTE) > -1) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PT_FSUP(a_TSS, a_idEstagio, a_period, idPat, idUTE), 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PT_FSUP(a_TSS, a_idEstagio, a_period, idPat, idUTE), 0.0);
					}//if (getVarDecisao_PT_FSUPseExistir(a_TSS, a_idEstagio, a_period, idPat, idUTE) > -1) {

					//////////////

					if (getVarDecisao_PTDISP_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUTE) > -1) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PTDISP_FINF(a_TSS, a_idEstagio, a_period, idPat, idUTE), 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PTDISP_FINF(a_TSS, a_idEstagio, a_period, idPat, idUTE), 0.0);
					}//if (getVarDecisao_PTDISP_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUTE) > -1) {

					if (getVarDecisao_PTDISP_FSUPseExistir(a_TSS, a_idEstagio, a_period, idPat, idUTE) > -1) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PTDISP_FSUP(a_TSS, a_idEstagio, a_period, idPat, idUTE), 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PTDISP_FSUP(a_TSS, a_idEstagio, a_period, idPat, idUTE), 0.0);
					}//if (getVarDecisao_PTDISP_FSUPseExistir(a_TSS, a_idEstagio, a_period, idPat, idUTE) > -1) {

					//////////////

					if (getVarDecisao_PTDISPCOM_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUTE) > -1) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PTDISPCOM_FINF(a_TSS, a_idEstagio, a_period, idPat, idUTE), 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PTDISPCOM_FINF(a_TSS, a_idEstagio, a_period, idPat, idUTE), 0.0);
					}//if (getVarDecisao_PTDISPCOM_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUTE) > -1) {

					if (getVarDecisao_PTDISPCOM_FSUPseExistir(a_TSS, a_idEstagio, a_period, idPat, idUTE) > -1) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PTDISPCOM_FSUP(a_TSS, a_idEstagio, a_period, idPat, idUTE), 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PTDISPCOM_FSUP(a_TSS, a_idEstagio, a_period, idPat, idUTE), 0.0);
					}//if (getVarDecisao_PTDISPCOM_FSUPseExistir(a_TSS, a_idEstagio, a_period, idPat, idUTE) > -1) {

				}//for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_period, IdPatamarCarga()); idPat++) {

			}//for (IdTermeletrica idUTE = a_dados.getMenorId(IdTermeletrica()); idUTE < a_maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idUTE)) {

			//Variáveis de folga associadas às usinas elevatórias
			const IdUsinaElevatoria idUsiElHIni = a_dados.getMenorId(IdUsinaElevatoria());
			const IdUsinaElevatoria idUsiElHOut = a_dados.getIdOut(IdUsinaElevatoria());
			for (IdUsinaElevatoria idUsinaElevatoria = idUsiElHIni; idUsinaElevatoria < idUsiElHOut; a_dados.vetorUsinaElevatoria.incr(idUsinaElevatoria)) {

				//*********************
				//VARIAVEL_DECISAO_4
				//*********************

				for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_period, IdPatamarCarga()); idPat++) {

					if (getVarDecisao_QB_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUsinaElevatoria) > -1) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QB_FINF(a_TSS, a_idEstagio, a_period, idPat, idUsinaElevatoria), 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QB_FINF(a_TSS, a_idEstagio, a_period, idPat, idUsinaElevatoria), 0.0);
					}//if (getVarDecisao_QB_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUsinaElevatoria) > -1) {

					/////////////

					if (getVarDecisao_QBDISP_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUsinaElevatoria) > -1) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QBDISP_FINF(a_TSS, a_idEstagio, a_period, idPat, idUsinaElevatoria), 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QBDISP_FINF(a_TSS, a_idEstagio, a_period, idPat, idUsinaElevatoria), 0.0);
					}//if (getVarDecisao_QBDISP_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUsinaElevatoria) > -1) {

				}//for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_period, IdPatamarCarga()); idPat++) {

			}//for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria < a_dados.getMaiorId(IdUsinaElevatoria()); idUsinaElevatoria++) {

			//Variáveis de folga associadas às restrições hidráulicas
			const IdRestricaoOperativaUHE idResOpIni = a_dados.getMenorId(IdRestricaoOperativaUHE());
			const IdRestricaoOperativaUHE idResOpOut = a_dados.getIdOut(IdRestricaoOperativaUHE());
			for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = idResOpIni; idRestricaoOperativaUHE < idResOpOut; a_dados.vetorRestricaoOperativaUHE.incr(idRestricaoOperativaUHE)) {

				//*********************
				//VARIAVEL_DECISAO_3
				//*********************

				if (getVarDecisao_RHA_FINFseExistir(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_RHA_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_RHA_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), 0.0);
				}//if (getVarDecisao_RHA_FINFseExistir(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

				if (getVarDecisao_RHA_FSUPseExistir(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_RHA_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_RHA_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), 0.0);
				}//if (getVarDecisao_RHA_FSUPseExistir(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

				/////////////

				if (getVarDecisao_RHV_FINFseExistir(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_RHV_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_RHV_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), 0.0);
				}//if (getVarDecisao_RHV_FINFseExistir(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

				if (getVarDecisao_RHV_FSUPseExistir(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_RHV_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_RHV_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), 0.0);
				}//if (getVarDecisao_RHV_FSUPseExistir(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

				/////////////

				if (getVarDecisao_RHE_FINFseExistir(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_RHE_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_RHE_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), 0.0);
				}//if (getVarDecisao_RHE_FINFseExistir(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

				if (getVarDecisao_RHE_FSUPseExistir(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_RHE_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_RHE_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), 0.0);
				}//if (getVarDecisao_RHE_FSUPseExistir(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {


				//*********************
				//VARIAVEL_DECISAO_4
				//*********************

				for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_period, IdPatamarCarga()); idPat++) {

					if (getVarDecisao_RHQ_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idRestricaoOperativaUHE) > -1) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_RHQ_FINF(a_TSS, a_idEstagio, a_period, idPat, idRestricaoOperativaUHE), 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_RHQ_FINF(a_TSS, a_idEstagio, a_period, idPat, idRestricaoOperativaUHE), 0.0);
					}//if (getVarDecisao_RHQ_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idRE) > -1) {

					if (getVarDecisao_RHQ_FSUPseExistir(a_TSS, a_idEstagio, a_period, idPat, idRestricaoOperativaUHE) > -1) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_RHQ_FSUP(a_TSS, a_idEstagio, a_period, idPat, idRestricaoOperativaUHE), 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_RHQ_FSUP(a_TSS, a_idEstagio, a_period, idPat, idRestricaoOperativaUHE), 0.0);
					}//if (getVarDecisao_RHQ_FSUPseExistir(a_TSS, a_idEstagio, a_period, idPat, idRE) > -1) {

				}//for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_period, IdPatamarCarga()); idPat++) {

			} // for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE < maiorIdRestricaoOperativaUHE; idRestricaoOperativaUHE++) {

			//Variáveis de folga associadas às restrições elétricas
			const IdRestricaoEletrica idREIni = a_dados.getMenorId(IdRestricaoEletrica());
			const IdRestricaoEletrica idREOut = a_dados.getIdOut(IdRestricaoEletrica());
			for (IdRestricaoEletrica idRE = idREIni; idRE < idREOut; a_dados.vetorRestricaoEletrica.incr(idRE)) {

				//*********************
				//VARIAVEL_DECISAO_4
				//*********************

				for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_period, IdPatamarCarga()); idPat++) {

					if (getVarDecisao_RE_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idRE) > -1) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_RE_FINF(a_TSS, a_idEstagio, a_period, idPat, idRE), 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_RE_FINF(a_TSS, a_idEstagio, a_period, idPat, idRE), 0.0);
					}//if (getVarDecisao_RE_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idRE) > -1) {

					if (getVarDecisao_RE_FSUPseExistir(a_TSS, a_idEstagio, a_period, idPat, idRE) > -1) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_RE_FSUP(a_TSS, a_idEstagio, a_period, idPat, idRE), 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_RE_FSUP(a_TSS, a_idEstagio, a_period, idPat, idRE), 0.0);
					}//if (getVarDecisao_RE_FSUPseExistir(a_TSS, a_idEstagio, a_period, idPat, idRE) > -1) {

					////////////

				}//for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_period, IdPatamarCarga()); idPat++) {


			}//for (IdRestricaoEletrica idRE = IdRestricaoEletrica_1; idRE < a_maiorIdRestricaoEletrica; idRE++) {

			//Variáveis de folga associadas a intercambios
			const IdIntercambio idInterIni = a_dados.getMenorId(IdIntercambio());
			const IdIntercambio idInterOut = a_dados.getIdOut(IdIntercambio());
			for (IdIntercambio idIntercambio = idInterIni; idIntercambio < idInterOut; a_dados.vetorIntercambio.incr(idIntercambio)) {

				//*********************
				//VARIAVEL_DECISAO_4
				//*********************

				for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_period, IdPatamarCarga()); idPat++) {

					if (getVarDecisao_PI_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idIntercambio) > -1) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PI_FINF(a_TSS, a_idEstagio, a_period, idPat, idIntercambio), 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PI_FINF(a_TSS, a_idEstagio, a_period, idPat, idIntercambio), 0.0);
					}//if (getVarDecisao_PI_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idUHE) > -1) {

				}//for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_period, IdPatamarCarga()); idPat++) {

			}//for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio < a_maiorIdIntercambio; idIntercambio++) {

			//Variáveis de folga associadas a intercambios hidráulicos
			const IdIntercambioHidraulico idInterHidrIni = a_dados.getMenorId(IdIntercambioHidraulico());
			const IdIntercambioHidraulico idInterHidrEnd = a_dados.getIdOut(IdIntercambioHidraulico());
			for (IdIntercambioHidraulico idIntercambioHidraulico = idInterHidrIni; idIntercambioHidraulico < idInterHidrEnd; a_dados.vetorIntercambioHidraulico.incr(idIntercambioHidraulico)) {

				//*********************
				//VARIAVEL_DECISAO_4
				//*********************

				for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_period, IdPatamarCarga()); idPat++) {

					if (getVarDecisao_QILS_TRI_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idIntercambioHidraulico) > -1) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QILS_TRI_FINF(a_TSS, a_idEstagio, a_period, idPat, idIntercambioHidraulico), 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QILS_TRI_FINF(a_TSS, a_idEstagio, a_period, idPat, idIntercambioHidraulico), 0.0);
					}//if (getVarDecisao_QILS_TRI_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idIntercambioHidraulico) > -1) {

					//*********************
					//VARIAVEL_DECISAO_5
					//*********************

					for (IdHidreletrica idUHE = idUHEIni; idUHE < idUHEOut; a_dados.vetorHidreletrica.incr(idUHE)) {

						if (getVarDecisao_QI_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idIntercambioHidraulico, idUHE) > -1) {
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QI_FINF(a_TSS, a_idEstagio, a_period, idPat, idIntercambioHidraulico, idUHE), 0.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QI_FINF(a_TSS, a_idEstagio, a_period, idPat, idIntercambioHidraulico, idUHE), 0.0);
						}//if (getVarDecisao_QI_FINFseExistir(a_TSS, a_idEstagio, a_period, idPat, idIntercambioHidraulico, idUHE) > -1) {

					}//for (IdHidreletrica idUHE = a_dados.getMenorId(IdHidreletrica()); idUHE < idUHEOut; a_dados.vetorHidreletrica.incr(idUHE)) {

				}//for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_period, IdPatamarCarga()); idPat++) {

			}//for (IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico_1; idIntercambioHidraulico < a_maiorIdIntercambioHidraulico; idIntercambioHidraulico++) {

	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::tratarVariaveisFolga(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
}

