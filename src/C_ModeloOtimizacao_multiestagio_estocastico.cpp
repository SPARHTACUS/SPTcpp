#include "C_ModeloOtimizacao.h"
#include "C_EntradaSaidaDados.h"
#include <chrono>

#include "mpi.h"

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////


void ModeloOtimizacao::formularModeloOtimizacao(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados) {


	try {
		
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

		const Periodo menor_periodo_estagio_acoplamento_pre_estudo = horizonte_estudo.getIteradorInicial();

		const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> horizon = a_dados.getMatriz(AttMatrizDados_horizonte_estudo, Periodo(), IdPatamarCarga(), double());

		//
		// Pré alocação de vetores e matrizes
		//


		if (true) {

			SmartEnupla<IdEstagio, SmartEnupla<IdRealizacao, double>> custo_medio(estagio_inicial, std::vector<SmartEnupla<IdRealizacao, double>>(int(estagio_final) - int(estagio_inicial) + 1, SmartEnupla<IdRealizacao, double>()));

			for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

				const Periodo period_stage = getAtributo(idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

				custo_medio.at(idEstagio) = SmartEnupla<IdRealizacao, double>(IdRealizacao_1, std::vector<double>(getAtributo(idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()), 0.0));		

				if (getSize1Matriz(IdProcessoEstocastico_1, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral) > 0) {
					const SmartEnupla<Periodo, IdRealizacao> horizonte_processo_estocastico_espaco_amostral_completo = getElementosMatriz(IdProcessoEstocastico_1, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario_1, Periodo(), IdRealizacao());
					const std::vector<Periodo> horizonte_processo_estocastico_espaco_amostral = horizonte_processo_estocastico_espaco_amostral_completo.getIteradores(period_stage);

					for (int i = 0; i < int(horizonte_processo_estocastico_espaco_amostral.size()); i++)
						horizonte_processo_estocastico_hidrologico_por_estagio.at(idEstagio).addElemento(horizonte_processo_estocastico_espaco_amostral.at(i), getSize2Matriz(IdProcessoEstocastico_1, IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_residuo_espaco_amostral, horizonte_processo_estocastico_espaco_amostral.at(i)));

				}

				for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
					if (horizonte_estudo.at(periodo) == idEstagio)
						horizonte_estudo_por_estagio.at(idEstagio).addElemento(periodo, 0.0);
				}

				int is_any_relaxacao_truncamento = 0;
				for (Periodo periodo = horizonte_processo_estocastico_hidrologico_por_estagio.at(idEstagio).getIteradorInicial(); periodo <= horizonte_processo_estocastico_hidrologico_por_estagio.at(idEstagio).getIteradorFinal(); horizonte_processo_estocastico_hidrologico_por_estagio.at(idEstagio).incrementarIterador(periodo)) {
					for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= getMaiorId(IdProcessoEstocastico_1, IdVariavelAleatoria()); idVar++) {
						if ((getElementoVetor(IdProcessoEstocastico_1, idVar, AttVetorVariavelAleatoria_tipo_relaxacao, periodo, TipoRelaxacaoVariavelAleatoria()) == TipoRelaxacaoVariavelAleatoria_truncamento) || \
							(getElementoVetor(IdProcessoEstocastico_1, idVar, AttVetorVariavelAleatoria_tipo_relaxacao, periodo, TipoRelaxacaoVariavelAleatoria()) == TipoRelaxacaoVariavelAleatoria_truncamento_penalizacao)) {
							is_any_relaxacao_truncamento = 1;
							break;
						}
					}
				}
				addElemento(AttVetorModeloOtimizacao_alguma_variavel_aleatoria_hidrologica_com_truncamento, idEstagio, is_any_relaxacao_truncamento);

			} // for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

			setMatriz(AttMatrizModeloOtimizacao_horizonte_estudo, horizonte_estudo_por_estagio);
			setMatriz(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, horizonte_processo_estocastico_hidrologico_por_estagio);

			setVetor_forced(AttVetorModeloOtimizacao_custo_total, SmartEnupla<IdEstagio, double>(estagio_inicial, std::vector<double>(int(estagio_final) - int(estagio_inicial) + 1, NAN)));
			setVetor_forced(AttVetorModeloOtimizacao_custo_imediato, SmartEnupla<IdEstagio, double>(estagio_inicial, std::vector<double>(int(estagio_final) - int(estagio_inicial) + 1, NAN)));
			setVetor_forced(AttVetorModeloOtimizacao_custo_futuro, SmartEnupla<IdEstagio, double>(estagio_inicial, std::vector<double>(int(estagio_final) - int(estagio_inicial) + 1, NAN)));

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

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::formularModeloOtimizacao(): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::formularModeloOtimizacao(Dados &a_dados) {

void ModeloOtimizacao::formularModeloOtimizacao(const SmartEnupla<IdEstagio, std::vector<TipoSubproblemaSolver>>& a_listaTSS, Dados& a_dados, const IdEstagio a_idEstagioIni, const IdEstagio a_idEstagioEnd, const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>& a_horizon) {

	try{

		for (IdEstagio idEstagio = a_idEstagioIni; idEstagio <= a_idEstagioEnd; idEstagio++) {
			//if(true){
			if (arranjoResolucao.isAnyCenarioEstado(idEstagio) || arranjoResolucao.isAnyAberturas(idEstagio)) {

				const SmartEnupla<Periodo, int> horizonSP = getElementosMatriz(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, idEstagio, Periodo(), int());
				const SmartEnupla<Periodo, double> horizon_stage = a_dados.getElementosMatriz(AttMatrizDados_percentual_duracao_horizonte_estudo, idEstagio, Periodo(), double());

				Periodo period_stage = a_dados.getElementoVetor(AttVetorDados_horizonte_otimizacao, idEstagio, Periodo());
				Periodo periodIni_stage = horizon_stage.getIteradorInicial();
				Periodo periodEnd_stage = horizon_stage.getIteradorFinal();

				for (int i = 0; i < int(a_listaTSS.at(idEstagio).size()); i++) {

					for (Periodo period = periodIni_stage; period <= periodEnd_stage; a_horizon.incrementarIterador(period)) {

						const IdPatamarCarga idPatEnd = a_horizon.at_rIt(period).getIteradorFinal();

						Periodo periodPrev = period;
						a_horizon.decrementarIterador(periodPrev);

						Periodo periodNext = period;
						a_horizon.incrementarIterador(periodNext);

						for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEnd; idPat++) {

							criarRestricoesCusto(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, periodIni_stage, period, idPat);

							criarTermeletricas(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarProcessoEstocasticoHidrologico(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, periodIni_stage, period_stage, periodEnd_stage, period, idPat, horizonSP, a_horizon);

							criarUsinasElevatorias(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarHidreletricas(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, periodIni_stage, periodEnd_stage, periodPrev, period, periodNext, idPat, a_horizon);

							criarDemanda(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarDemandaEspecial(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarDeficit(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarRenovaveis(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarUsinaNaoSimulada(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarIntercambios(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarAgrupamentoIntercambio(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarAtendimentoDemanda(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarContratos(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, periodIni_stage, periodPrev, period, periodNext, idPat);

							criarRestricoesEletricas(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, periodIni_stage, periodPrev, period, periodNext, idPat);

							criarRestricoesHidraulicas(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, periodIni_stage, periodPrev, period, periodNext, idPat);

							criarRestricoesHidraulicaEspecial_vazao_defluente(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

							criarReservaPotencia(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, idPat);

						}// for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

						criarRestricoesHidraulicaEspecial_vazao_afluente(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period);

						criarRestricoesHidraulicaEspecial_volume_armazenado(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, periodIni_stage, periodEnd_stage, period, periodNext, a_horizon);

						criarRestricoesHidraulicaEspecial_energia_armazenada(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, periodEnd_stage, period, periodNext);

						criarControleCotaVazao(a_listaTSS.at(idEstagio).at(i), a_dados, idEstagio, period, a_horizon);

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

void ModeloOtimizacao::criarAgrupamentoIntercambio(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period, const IdPatamarCarga a_idPat) {

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
					for (int i = 1; i <= numero_intercambio; i++) {
						const IdIntercambio idINT = a_dados.getElementoVetor(idAgrupamentoIntercambio, AttVetorAgrupamentoIntercambio_intercambio, i, IdIntercambio());
						const IdSubmercado idSSE_orig = a_dados.getAtributo(idINT, AttComumIntercambio_submercado_origem, IdSubmercado());
						const IdSubmercado idSSE_dest = a_dados.getAtributo(idINT, AttComumIntercambio_submercado_destino, IdSubmercado());
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_period, a_idPat, idINT, idSSE_orig, idSSE_dest), posIneAI, -a_dados.getElementoVetor(idAgrupamentoIntercambio, AttVetorAgrupamentoIntercambio_fator_ponderacao, i, double()));
					}
				} // if (potencia_maxima > 0.0){

			} // if (a_dados.getSize1Matriz(idAgrupamentoIntercambio, AttMatrizAgrupamentoIntercambio_potencia_maxima) > 0) {

		} // for (IdAgrupamentoIntercambio idAgrupamentoIntercambio = IdAgrupamentoIntercambio_1; idAgrupamentoIntercambio < idAgrInOut; idAgrupamentoIntercambio++) {

	}//try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarAgrupamentoIntercambio(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
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



void ModeloOtimizacao::criarUsinasElevatorias(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const IdUsinaElevatoria idUsiElHIni = a_dados.getMenorId(IdUsinaElevatoria());
		const IdUsinaElevatoria idUsiElHOut = a_dados.getIdOut(IdUsinaElevatoria());
		for (IdUsinaElevatoria idUsinaElevatoria = idUsiElHIni; idUsinaElevatoria < idUsiElHOut; a_dados.vetorUsinaElevatoria.incr(idUsinaElevatoria)) {


			//Variáveis para bombeamento QBOM

			if (a_dados.getElementoVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_fator_disponibilidade, a_period, double()) > 0.0) {

				if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

					addVarDecisao_PBOMDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria, 0.0, infinito, 0.0);

				} // if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

				// QBOM
				addVarDecisao_QBOM(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria, 0.0, a_dados.getElementoVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_vazao_bombeada_maxima, a_period, double()), 0.0);

				// QBOM_FINF
				if (a_dados.getElementoVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_vazao_bombeada_minima, a_period, double()) > 0.0)
					addVarDecisao_QBOM_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria, 0.0, infinito, 0.0);

				// QBOMDISP
				addVarDecisao_QBOMDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria, 0.0, a_dados.getElementoMatriz(idUsinaElevatoria, AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_maxima, a_period, a_idPat, double()), 0.0);

				// QBOMDISP_FINF
				if (a_dados.getElementoMatriz(idUsinaElevatoria, AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_minima, a_period, a_idPat, double()) > 0.0)
					addVarDecisao_QBOMDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria, 0.0, infinito, 0.0);

			} // if (a_dados.getElementoVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_fator_disponibilidade, a_period, double()) > 0.0) {

			if ((a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) && (getVarDecisao_QBOMseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria) > -1)) {

				const IdPatamarCarga idPatEnd = a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, a_period, IdPatamarCarga());
				const double perc_dur_pat = a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, a_idPat, double());

				const int posEquBOM_pat = addEquLinear_PBOMDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquBOM_pat, 0.0);

				const int varPBOMDISP_pat = getVarDecisao_PBOMDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPBOMDISP_pat, posEquBOM_pat, 1.0);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria), posEquBOM_pat, -a_dados.getAtributo(idUsinaElevatoria, AttComumUsinaElevatoria_taxa_de_consumo, double()) * a_dados.getElementoVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_fator_disponibilidade, a_period, double()));

				int varPBOMDISP = getVarDecisao_PBOMDISPseExistir(a_TSS, a_idEstagio, a_period, idUsinaElevatoria);
				if (idPatEnd > IdPatamarCarga_1) {
					if (varPBOMDISP == -1)
						varPBOMDISP = addVarDecisao_PBOMDISP(a_TSS, a_idEstagio, a_period, idUsinaElevatoria, 0.0, infinito, 0.0);

					if (getEquLinear_PBOMDISPseExistir(a_TSS, a_idEstagio, a_period, idUsinaElevatoria) == -1) {

						const int posEquPBOMDISP = addEquLinear_PBOMDISP(a_TSS, a_idEstagio, a_period, idUsinaElevatoria);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPBOMDISP, 0.0);
						// Variável PBOMDISP Media
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PBOMDISP(a_TSS, a_idEstagio, a_period, idUsinaElevatoria), posEquPBOMDISP, 1.0);

					} // if (getEquLinear_PBOMDISPseExistir(a_idEstagio, a_period, idUsinaElevatoria) == -1) {

					// Variável PBOMDISP
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PBOMDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria), getEquLinear_PBOMDISP(a_TSS, a_idEstagio, a_period, idUsinaElevatoria), -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, a_idPat, double()));

				} // if (idPatEnd > IdPatamarCarga_1) {

				const IdSubmercado idSSE = a_dados.getAtributo(idUsinaElevatoria, AttComumUsinaElevatoria_submercado, IdSubmercado());

				int varPBOMDISP_sse = getVarDecisao_PBOMDISPseExistir(a_TSS, a_idEstagio, a_period, idSSE);
				int equPBOMDISP_sse = getEquLinear_PBOMDISPseExistir(a_TSS, a_idEstagio, a_period, idSSE);

				if (varPBOMDISP_sse == -1) {
					varPBOMDISP_sse = addVarDecisao_PBOMDISP(a_TSS, a_idEstagio, a_period, idSSE, 0.0, infinito, 0.0);
					equPBOMDISP_sse = addEquLinear_PBOMDISP(a_TSS, a_idEstagio, a_period, idSSE);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPBOMDISP_sse, equPBOMDISP_sse, 1.0);

					int varPBOMDISP_sin = getVarDecisao_PBOMDISPseExistir(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);
					int equPBOMDISP_sin = getEquLinear_PBOMDISPseExistir(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);

					if (varPBOMDISP_sin == -1) {
						varPBOMDISP_sin = addVarDecisao_PBOMDISP(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN, 0.0, infinito, 0.0);
						equPBOMDISP_sin = addEquLinear_PBOMDISP(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPBOMDISP_sin, equPBOMDISP_sin, 1.0);
					}
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPBOMDISP_sse, equPBOMDISP_sin, -1.0);
				} // if (varPBOMDISP_sse == -1) {

				if (varPBOMDISP > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPBOMDISP, equPBOMDISP_sse, -1.0);
				else if (varPBOMDISP_pat > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPBOMDISP_pat, equPBOMDISP_sse, -1.0);

			} // if (getVarDecisao_QBOMseExistir(a_idEstagio, a_period, a_idPat, idUsinaElevatoria) > -1) {

			if (getVarDecisao_QBOM_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria) > -1) {

				const int posIneQBOM_FINF = addIneLinear_QBOM_LINF(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneQBOM_FINF, a_dados.getElementoVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_vazao_bombeada_minima, a_period, double()));

				// Variável QBOM_FINF
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria), posIneQBOM_FINF, 1.0);

				// Variável QBOM
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria), posIneQBOM_FINF, 1.0);

			} // if (getVarDecisao_QBOM_FINFseExistir(a_idEstagio, a_period, a_idPat, idUsinaElevatoria) > -1) {

			if (getVarDecisao_QBOMDISP_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria) > -1) {

				const int posIneQBOMDISP_FINF = addIneLinear_QBOMDISP_LINF(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneQBOMDISP_FINF, a_dados.getElementoMatriz(idUsinaElevatoria, AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_minima, a_period, a_idPat, double()));

				// Variável QBOMDISP_FINF
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOMDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria), posIneQBOMDISP_FINF, 1.0);

				// Variável QBOMDISP
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOMDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria), posIneQBOMDISP_FINF, 1.0);

			} // if (getVarDecisao_QBOMDISP_FINFseExistir(a_idEstagio, a_period, a_idPat, idUsinaElevatoria) > -1) {

			// Restrição QBOMDISP
			if (getVarDecisao_QBOMDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria) > -1) {

				const int posEquQBOMDISP = addEquLinear_QBOMDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQBOMDISP, 0.0);

				// QBOMDISP
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOMDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria), posEquQBOMDISP, 1.0);

				// QBOM
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria), posEquQBOMDISP, -a_dados.getElementoVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_fator_disponibilidade, a_period, double()));

			}//if (getVarDecisao_QBDISPseExistir(a_idEstagio, a_period, a_idPat, idUsinaElevatoria) > -1) {

			if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica) {
				// Variável QBOM_FINF
				if (getVarDecisao_QBOM_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria), getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getAtributo(idUsinaElevatoria, AttComumUsinaElevatoria_penalidade_vazao_bombeada_minima, double()));

				// Variável QBOMDISP_FINF
				if (getVarDecisao_QBOMDISP_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOMDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria), getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getAtributo(idUsinaElevatoria, AttComumUsinaElevatoria_penalidade_vazao_bombeada_minima, double()));
			}

		}//for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria < a_dados.getMaiorId(IdUsinaElevatoria()); idUsinaElevatoria++) {

	}//try {

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarUsinasElevatorias(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }

}


void ModeloOtimizacao::criarProcessoEstocasticoHidrologico(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodIni_stage, Periodo& a_period_stage, Periodo& a_periodEnd_stage, Periodo& a_period, const IdPatamarCarga a_idPat, const SmartEnupla<Periodo, int>& a_horizonSP, const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>& a_horizon){
	
	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_idPat > IdPatamarCarga_1)
			return;

		const bool viabilidade_hidraulica = getAtributo(AttComumModeloOtimizacao_relaxar_afluencia_incremental_com_viabilidade_hidraulica, bool());

		const IdProcessoEstocastico idProcEstocastico = IdProcessoEstocastico_1;

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= getMaiorId(idProcEstocastico, IdVariavelAleatoria()); idVariavelAleatoria++) {

			const std::vector<IdHidreletrica> lista_hidreletrica = vetorProcessoEstocastico.at(idProcEstocastico).getIdFisicoFromIdVariavelAleatoria(idVariavelAleatoria, IdHidreletrica());

			if (lista_hidreletrica.size() == 0)
				throw std::invalid_argument("Nao foram encontradas hidreletricas associada a :" + getFullString(idProcEstocastico) + " " + getFullString(idVariavelAleatoria));

			if (a_period == a_periodIni_stage){

				for (Periodo periodSP = a_horizonSP.getIteradorInicial(); periodSP <= a_horizonSP.getIteradorFinal(); a_horizonSP.incrementarIterador(periodSP)) {

					//
					//
					// Parcela referente ao processo estocastico
					//
					//

					// varRP
					const int varRP = addVarDecisao_RP(a_TSS, a_idEstagio, periodSP, idProcEstocastico, idVariavelAleatoria, -infinito, infinito, 0.0);

					if (getSize2Matriz(idProcEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodSP) > 1) {

						vetorEstagio.at(a_idEstagio).addVariavelRealizacao(a_TSS, getNomeSolverVarDecisao_RP(a_TSS, a_idEstagio, periodSP, idProcEstocastico, idVariavelAleatoria), varRP, idProcEstocastico, idVariavelAleatoria, periodSP, 1.0);

						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->addVarDinamica(varRP);

					}
					else {
						const double residuo = getElementoMatriz(idProcEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodSP, IdRealizacao_1, double());
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(varRP, residuo);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(varRP, residuo);
					}


					// varYP
					const int varYP = addVarDecisao_YP(a_TSS, a_idEstagio, periodSP, idProcEstocastico, idVariavelAleatoria, -infinito, infinito, 0.0);

					// Restricao YP (YP = RP + SOMA(fp*YPt-1))
					const int posEquYP = addEquLinear_YP(a_TSS, a_idEstagio, periodSP, idProcEstocastico, idVariavelAleatoria);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYP, posEquYP, 1.0);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRP, posEquYP, -1.0);

					const double grau_liberdade_var_aleatoria = vetorProcessoEstocastico.at(idProcEstocastico).getGrauLiberdade(idVariavelAleatoria);

					// Variável do Processo Estocástico Hidrológico YP lag = 0 ... lag = np
					if (getSize1Matriz(idProcEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao) > 0) {

						for (int lag = 1; lag <= getSize2Matriz(idProcEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, periodSP); lag++) {

							if (getElementoMatriz(idProcEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, periodSP, lag, double()) != 0.0) {

								Periodo periodo_lag = periodSP - lag;

								int var_YP_LAG = criarVariaveisDecisao_VariaveisEstado_Restricoes_YP(a_TSS, a_dados, a_idEstagio, idProcEstocastico, idVariavelAleatoria, periodo_lag, grau_liberdade_var_aleatoria);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(var_YP_LAG, posEquYP, -getElementoMatriz(idProcEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, periodSP, lag, double()));

							} // if (getElementoMatriz(idProcEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, periodSP, lag, double()) > 0.0){
						} // for (int lag = 1; lag <= getSize2Matriz(idProcEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao_sazonal, periodSP.getMes()); lag++) {
					} // if (getSize1Matriz(idProcEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao) > 0) {

				} // for (Periodo periodSP = periodo_processo_estocastico_inicial; periodSP <= periodo_processo_estocastico_final; a_horizonte_processo_estocastico_estagio.incrementarIterador(periodSP)) {

			} // if (a_period == a_periodIni_stage){


			//
			//
			// Parcela referente às afluencias físicas
			//
			//

			bool sobreposicao_encontrada = false;
			for (Periodo periodSP = a_horizonSP.getIteradorInicial(); periodSP <= a_horizonSP.getIteradorFinal(); a_horizonSP.incrementarIterador(periodSP)) {

				const double sobreposicao = a_period.sobreposicao(periodSP);

				if (sobreposicao > 0.0) {

					const TipoRelaxacaoVariavelAleatoria tipo_relaxacao = getElementoVetor(idProcEstocastico, idVariavelAleatoria, AttVetorVariavelAleatoria_tipo_relaxacao, periodSP, TipoRelaxacaoVariavelAleatoria());

					const int varDecYP = getVarDecisao_YP(a_TSS, a_idEstagio, periodSP, idProcEstocastico, idVariavelAleatoria);
					const int posEquYP = getEquLinear_YP(a_TSS, a_idEstagio, periodSP, idProcEstocastico, idVariavelAleatoria);

					for (int i = 0; i < int(lista_hidreletrica.size()); i++) {
						const IdHidreletrica idUHE = lista_hidreletrica.at(i);

						const IdVariavelAleatoriaInterna idVarInt = vetorProcessoEstocastico.at(idProcEstocastico).getIdVariavelAleatoriaInternaFromIdVariavelAleatoriaIdFisico(idVariavelAleatoria, idUHE);

						const double grau_liberdade = getAtributo(idProcEstocastico, idVariavelAleatoria, idVarInt, AttComumVariavelAleatoriaInterna_grau_liberdade, double());
						const double coeficiente_participacao = getElementoVetor(idProcEstocastico, idVariavelAleatoria, idVarInt, AttVetorVariavelAleatoriaInterna_coeficiente_participacao, periodSP, double());

						// Afluencia Incremental (QINC): definida por periodo estudo
						const int varQINC = addVarDecisao_QINC(a_TSS, a_idEstagio, a_period, idUHE, -infinito, infinito, 0.0);

						// Restricao Afluencia Incremental: definida por periodo estudo
						const int posEquQINC = addEquLinear_QINC(a_TSS, a_idEstagio, a_period, idUHE);

						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQINC, -grau_liberdade * sobreposicao);

						// Variável QINC
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQINC, posEquQINC, 1.0);

						// Variável YP
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varDecYP, posEquQINC, -coeficiente_participacao * sobreposicao);

						if (tipo_relaxacao != TipoRelaxacaoVariavelAleatoria_sem_relaxacao) {

							if (true) {
								// Variável Afluencia Incremental Folga (QINC_FINF): definida por periodo estudo (Quando penalizada ou utilizando viabilidade hidráulica)
								int varQINC_FINF = getVarDecisao_QINC_FINFseExistir(a_TSS, a_idEstagio, a_period, idUHE);

								if (varQINC_FINF == -1) {

									if (viabilidade_hidraulica) {

										if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica)
											varQINC_FINF = addVarDecisao_QINC_FINF(a_TSS, a_idEstagio, a_period, idUHE, 0.0, 0.0, 0.0);

										else if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
											varQINC_FINF = addVarDecisao_QINC_FINF(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 100.0);

										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->addVarDinamica(varQINC_FINF);

										vetorEstagio.at(a_idEstagio).addVariavelRealizacaoInterna(a_TSS, getNomeSolverVarDecisao_QINC_FINF(a_TSS, a_idEstagio, a_period, idUHE), varQINC_FINF, idProcEstocastico, idVariavelAleatoria, idVarInt, periodSP, sobreposicao, TipoValor_positivo, 1.0, 1.0);

									} // if (viabilidade_hidraulica) {

									else if (tipo_relaxacao == TipoRelaxacaoVariavelAleatoria_penalizacao)
										varQINC_FINF = addVarDecisao_QINC_FINF(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);


								} // if (varQINC_FINF == -1) {

								// Variável Afluencia Incremental Folga (QINC_FINF): definida por periodo estudo
								if (varQINC_FINF > -1) {
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQINC_FINF, posEquQINC, -sobreposicao);
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQINC_FINF, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_afluencia_incremental, double()));
								}

							} // if (true) {

							// Variável Afluencia Proc. Estocastico Folga (YP_FINF): definida por periodo proc. estocastico (Quando truncada)
							int varYP_FINF = getVarDecisao_YP_FINFseExistir(a_TSS, a_idEstagio, periodSP, idProcEstocastico, idVariavelAleatoria);

							if (varYP_FINF == -1) {
								if ((tipo_relaxacao == TipoRelaxacaoVariavelAleatoria_truncamento) || (tipo_relaxacao == TipoRelaxacaoVariavelAleatoria_truncamento_penalizacao)) {
									varYP_FINF = addVarDecisao_YP_FINF(a_TSS, a_idEstagio, periodSP, idProcEstocastico, idVariavelAleatoria, 0.0, 0.0, 0.0);
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->addVarDinamica(varYP_FINF);
									vetorEstagio.at(a_idEstagio).addVariavelRealizacaoInterna(a_TSS, getNomeSolverVarDecisao_YP_FINF(a_TSS, a_idEstagio, periodSP, idProcEstocastico, idVariavelAleatoria), varYP_FINF, idProcEstocastico, idVariavelAleatoria, IdVariavelAleatoriaInterna_Nenhum, periodSP, sobreposicao, TipoValor_positivo, 1.0, 1.0);
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYP_FINF, posEquYP, -1.0);
								}

							} // if (varYP_FINF == -1) {

							if ((varYP_FINF > -1) && (getElementoVetor(AttVetorModeloOtimizacao_alguma_variavel_aleatoria_hidrologica_com_truncamento, a_idEstagio, int()) == 1)) {

								if (tipo_relaxacao == TipoRelaxacaoVariavelAleatoria_truncamento_penalizacao) {
									// Variável YP_FINF Truncamento
									double penalidade = 0.0;
									for (IdVariavelAleatoriaInterna idVarInt = IdVariavelAleatoriaInterna_1; idVarInt <= getMaiorId(idProcEstocastico, idVariavelAleatoria, IdVariavelAleatoriaInterna()); idVarInt++) {
										const IdHidreletrica idUHE = vetorProcessoEstocastico.at(idProcEstocastico).getIdFisicoFromIdVariavelAleatoriaIdVariavelAleatoriaInterna(idVariavelAleatoria, idVarInt, IdHidreletrica());
										penalidade += a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_afluencia_incremental, double()) * getElementoVetor(idProcEstocastico, idVariavelAleatoria, idVarInt, AttVetorVariavelAleatoriaInterna_coeficiente_participacao, periodSP, double());
									}
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYP_FINF, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -sobreposicao * penalidade);

								} // if (tipo_relaxacao == TipoRelaxacaoVariavelAleatoria_truncamento_penalizacao) {
							} // if ((varYP_FINF > -1) && (getElementoVetor(AttVetorModeloOtimizacao_alguma_variavel_aleatoria_hidrologica_com_truncamento, a_idEstagio, int()) == 1)) {
						} // if (tipo_relaxacao != TipoRelaxacaoVariavelAleatoria_sem_relaxacao) {
					} // for (int i = 0; i < int(lista_hidreletrica.size()); i++) {

					if (!sobreposicao_encontrada)
						sobreposicao_encontrada = true;
				}
				else if ((sobreposicao == 0.0) && (sobreposicao_encontrada))
					break;

			} // for (Periodo a_period = a_horizon.getIteradorInicial(); a_period <= a_horizon.getIteradorFinal(); a_horizon.incrementarIterador(a_period)) {

		} // for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= getMaiorId(idProcEstocastico, IdVariavelAleatoria()); idVariavelAleatoria++) {

	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarProcessoEstocasticoHidrologico(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarBalancoHidraulicoPorVazao(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodIni_stage, Periodo& a_periodEnd_stage, Periodo& a_periodPrev, Periodo& a_period, Periodo& a_periodNext, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPat, const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>& a_horizon) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		// --------------------
		//
		// Vazao Montante
		// 
		// --------------------

		try {

			const int posEquQMON = addEquLinear_QMON(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQMON, 0.0);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QMON(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquQMON, 1.0);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QINC(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posEquQMON, -1.0);

			// Variáveis de Hidreletricas à Montante

			const int numero_hidreletricas_montante = a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_montante);
			for (int m = 1; m <= numero_hidreletricas_montante; m++) {
				const IdHidreletrica idHidreletrica_montante = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_montante, m, IdHidreletrica());

				const int tempo_viagem_agua = a_dados.getAtributo(idHidreletrica_montante, AttComumHidreletrica_tempo_viagem_agua, int());

				if (tempo_viagem_agua > 0) {
					const Periodo periodo_ref = a_dados.getElementoVetor(idHidreletrica_montante, AttVetorHidreletrica_horizonte_defluencia_viajante, a_period, Periodo());
					if (periodo_ref != Periodo(IdAno_1900)) {
						Periodo periodo_lag = Periodo(getString(periodo_ref.getDuration()), Periodo("h", periodo_ref) - tempo_viagem_agua);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(criarVariaveisDecisao_VariaveisEstado_Restricoes_QDEF(a_TSS, a_dados, a_idEstagio, idHidreletrica_montante, periodo_lag, a_horizon), posEquQMON, -1.0);
					}
					else
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, a_idPat, idHidreletrica_montante), posEquQMON, -1.0);
				}
				else {

					// Variável QD
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, a_idPat, idHidreletrica_montante), posEquQMON, -1.0);

				}//else {

			} // for (int m = 1; m <= numero_hidreletricas_montante; m++) {

		}// try

		catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::Erro Restricao Vazao Montante: \n" + std::string(erro.what())); }

		// -------------------------------
		//
		// Balanço Hídrico no Reservatório
		// 
		// -------------------------------

		const double conversor_vazao_volume = a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, a_period, a_idPat, double());

		const int posEquBH = addEquLinear_BH_VAZ(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);

		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquBH, 0.0);

		// Variável VI
		if (getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1)
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posEquBH, -1.0 / conversor_vazao_volume);

		//VARIÁVEL EVAPORAÇÃO
		const int varQEVA = getVarDecisao_QEVAseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica);
		if (varQEVA > -1)
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQEVA, posEquBH, 1.0);

		// Variáveis de Hidreletricas à Montante desvio
		const int numero_hidreletricas_montante_desvio = a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_montante_desvio);
		for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

			const IdHidreletrica idHidreletrica_montante_desvio = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_montante_desvio, m, IdHidreletrica());

			// Variável QDES que "entra" na usina
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_period, a_idPat, idHidreletrica_montante_desvio, a_idHidreletrica), posEquBH, -1.0);

		}//for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

		// Variável QDES que "sai" da usina
		if (a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica()) != IdHidreletrica_Nenhum)
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posEquBH, 1.0);

		// Variável QMON
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QMON(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquBH, -1.0);

		// Variável QDEF
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquBH, 1.0);

		//QRET
		if (getVarDecisao_QRETseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1)
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QRET(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquBH, 1.0);


		///////////////////////////
		//Usinas de bombeamento
		///////////////////////////
		//Nota: as usinas a montante = destino e jusante = origem

		const std::vector<IdUsinaElevatoria> idUsinaElevatoria_origem = a_dados.vetorUsinaElevatoria.getIdObjetos(AttComumUsinaElevatoria_usina_origem_bombeamento, a_idHidreletrica);

		const int idUsinaElevatoria_origem_size = int(idUsinaElevatoria_origem.size());

		for (int pos = 0; pos < idUsinaElevatoria_origem_size; pos++)
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria_origem.at(pos)), posEquBH, a_dados.getElementoVetor(idUsinaElevatoria_origem.at(pos), AttVetorUsinaElevatoria_fator_disponibilidade, a_period, double()));

		const std::vector<IdUsinaElevatoria> idUsinaElevatoria_destino = a_dados.vetorUsinaElevatoria.getIdObjetos(AttComumUsinaElevatoria_usina_destino_bombeamento, a_idHidreletrica);

		const int idUsinaElevatoria_destino_size = int(idUsinaElevatoria_destino.size());

		for (int pos = 0; pos < idUsinaElevatoria_destino_size; pos++)
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria_destino.at(pos)), posEquBH, -a_dados.getElementoVetor(idUsinaElevatoria_destino.at(pos), AttVetorUsinaElevatoria_fator_disponibilidade, a_period, double()));


	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarBalancoHidraulicoPorVazao(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarBalancoHidraulicoPorVolume(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodIni_stage, Periodo& a_periodEnd_stage, Periodo& a_periodPrev, Periodo& a_period, Periodo& a_periodNext, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPat, const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>& a_horizon) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		// --------------------
		//
		// Vazao Afluente Montante
		// 
		// --------------------

		const double conversor_vazao_volume = a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, a_period, a_idPat, double());
		const double conversor_vazao_volume_periodo = a_dados.getElementoVetor(AttVetorDados_conversor_vazao_volume, a_period, double());

		try {

			const int posEquQMON = addEquLinear_QMON(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQMON, 0.0);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QMON(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquQMON, 1.0);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QINC(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posEquQMON, -1.0);

			// Variáveis de Hidreletricas à Montante

			const int numero_hidreletricas_montante = a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_montante);
			for (int m = 1; m <= numero_hidreletricas_montante; m++) {
				const IdHidreletrica idHidreletrica_montante = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_montante, m, IdHidreletrica());

				const int tempo_viagem_agua = a_dados.getAtributo(idHidreletrica_montante, AttComumHidreletrica_tempo_viagem_agua, int());

				if (tempo_viagem_agua > 0) {
					const Periodo periodo_ref = a_dados.getElementoVetor(idHidreletrica_montante, AttVetorHidreletrica_horizonte_defluencia_viajante, a_period, Periodo());
					if (periodo_ref != Periodo(IdAno_1900)) {
						Periodo periodo_lag = Periodo(getString(periodo_ref.getDuration()), Periodo("h", periodo_ref) - tempo_viagem_agua);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(criarVariaveisDecisao_VariaveisEstado_Restricoes_QDEF(a_TSS, a_dados, a_idEstagio, idHidreletrica_montante, periodo_lag, a_horizon), posEquQMON, -1.0);
					}
					else
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, a_idPat, idHidreletrica_montante), posEquQMON, -1.0);
				}
				else
					// Variável QD
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, a_idPat, idHidreletrica_montante), posEquQMON, -1.0);

			} // for (int m = 1; m <= numero_hidreletricas_montante; m++) {
		}// try

		catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::Erro Restricao Vazao Montante: \n" + std::string(erro.what())); }


		// -------------------------------
		//
		// Balanço Hídrico no Reservatório
		// 
		// -------------------------------

		if (a_idPat == IdPatamarCarga_1) {

			//EQUAÇÃO DE BALANÇO HIDRAULICO
			const int posEquBH = addEquLinear_BH_VOL(a_TSS, a_idEstagio, a_period, a_idHidreletrica);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquBH, 0.0);

			//VARIÁVEL EVAPORAÇÃO
			const int varQEVA = getVarDecisao_QEVAseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica);
			if (varQEVA > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQEVA, posEquBH, conversor_vazao_volume_periodo);

			// Variável VI
			if (getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posEquBH, -1.0);

			if (a_period < a_periodEnd_stage) {

				if (getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_periodNext, a_idHidreletrica) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, a_idHidreletrica), posEquBH, 1.0);
			}
			else if ((a_period == a_periodEnd_stage && (getVarDecisao_VFseExistir(a_TSS, a_idEstagio, a_period, a_idHidreletrica) > -1)))
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_period, a_idHidreletrica), posEquBH, 1.0);

		} // if (a_idPat == IdPatamarCarga_1){

		const int posEquBH = getEquLinear_BH_VOL(a_TSS, a_idEstagio, a_period, a_idHidreletrica);

		// Variáveis de Hidreletricas à Montante desvio
		const int numero_hidreletricas_montante_desvio = a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_montante_desvio);
		for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

			const IdHidreletrica idHidreletrica_montante_desvio = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_montante_desvio, m, IdHidreletrica());

			// Variável QDES que "entra" na usina
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_period, a_idPat, idHidreletrica_montante_desvio, a_idHidreletrica), posEquBH, -conversor_vazao_volume);

		}//for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

		// Variável QDES que "sai" da usina
		if(a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica()) != IdHidreletrica_Nenhum)
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posEquBH, conversor_vazao_volume);


		// Variável QDEF
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquBH, conversor_vazao_volume);

		// Variável QMON
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QMON(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquBH, -conversor_vazao_volume);

		//QRET
		if (getVarDecisao_QRETseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1)
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QRET(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posEquBH, conversor_vazao_volume);

		//Usinas de bombeamento
		//Nota: as usinas a montante = destino e jusante = origem

		const std::vector<IdUsinaElevatoria> idUsinaElevatoria_origem = a_dados.vetorUsinaElevatoria.getIdObjetos(AttComumUsinaElevatoria_usina_origem_bombeamento, a_idHidreletrica);

		const int idUsinaElevatoria_origem_size = int(idUsinaElevatoria_origem.size());

		for (int pos = 0; pos < idUsinaElevatoria_origem_size; pos++)
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria_origem.at(pos)), posEquBH, conversor_vazao_volume * a_dados.getElementoVetor(idUsinaElevatoria_origem.at(pos), AttVetorUsinaElevatoria_fator_disponibilidade, a_period, double()));

		const std::vector<IdUsinaElevatoria> idUsinaElevatoria_destino = a_dados.vetorUsinaElevatoria.getIdObjetos(AttComumUsinaElevatoria_usina_destino_bombeamento, a_idHidreletrica);

		const int idUsinaElevatoria_destino_size = int(idUsinaElevatoria_destino.size());

		for (int pos = 0; pos < idUsinaElevatoria_destino_size; pos++)
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria_destino.at(pos)), posEquBH, -conversor_vazao_volume * a_dados.getElementoVetor(idUsinaElevatoria_destino.at(pos), AttVetorUsinaElevatoria_fator_disponibilidade, a_period, double()));


	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarBalancoHidraulicoPorVolume(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarAtendimentoDemanda(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period, const IdPatamarCarga a_idPat) {

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

		const IdRenovavel idEolIni = a_dados.getMenorId(IdRenovavel());
		const IdRenovavel IdEolOut = a_dados.getIdOut(IdRenovavel());

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

			for (IdRenovavel idRenovavel = idEolIni; idRenovavel < IdEolOut; a_dados.vetorRenovavel.incr(idRenovavel))
				if (a_dados.getAtributo(idRenovavel, AttComumRenovavel_submercado, IdSubmercado()) == idSSE)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PR(a_TSS, a_idEstagio, a_period, a_idPat, idRenovavel), posEquAD, 1.0);


			// Variaveis Patamares Deficit
			const IdPatamarDeficit idPatDefIni = a_dados.getMenorId(idSSE, IdPatamarDeficit());
			const IdPatamarDeficit idPatDefOut = a_dados.getIdOut(idSSE, IdPatamarDeficit());

			for (IdPatamarDeficit idPatamarDeficit = idPatDefIni; idPatamarDeficit < idPatDefOut; a_dados.vetorSubmercado.at(idSSE).vetorPatamarDeficit.incr(idPatamarDeficit)) {

				// Variável PD
				if (getVarDecisao_PDseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, idPatamarDeficit) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PD(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, idPatamarDeficit), posEquAD, 1.0);

			} // for (IdPatamarDeficit idPatamarDeficit = IdPatamarDeficit_1; idPatamarDeficit <= maiorIdPatamarDeficit; idPatamarDeficit++) {


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

					const IdUnidadeUTE idUnUTEIni = a_dados.getMenorId(idUTE, IdUnidadeUTE());
					const IdUnidadeUTE idUnUTEOut = a_dados.getIdOut(idUTE, IdUnidadeUTE());

					for (IdUnidadeUTE idUnidadeUTE = idUnUTEIni; idUnidadeUTE < idUnUTEOut; a_dados.incr(idUTE, idUnidadeUTE)) {

						// Variável PTDISP
						if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE) > -1) {

							if (a_dados.getAtributo(idUTE, idUnidadeUTE, AttComumUnidadeUTE_submercado, IdSubmercado()) == idSSE) {
								if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE) > -1)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE), posEquAD, 1.0);
							}

						} // if (getVarDecisao_PTDISPseExistir(a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE) > -1) {

					} // for (IdUnidadeUTE idUnidadeUTE = idUnUTEIni; idUnidadeUTE < idUnUTEOut; a_dados.incr(idUTE, idUnidadeUTE)) {

				} // else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {

			} // for (IdTermeletrica idUTE = a_dados.getMenorId(IdTermeletrica()); idUTE <= maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idUTE)) {


			// Variaveis Hidreletricas
			for (IdHidreletrica idUHE = idUHEIni; idUHE < idUHEOut; a_dados.vetorHidreletrica.incr(idUHE)) {

				const TipoDetalhamentoProducaoHidreletrica tipo_detalhamento_producao = a_dados.getAtributo(idUHE, AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica());

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

				const IdSubmercado idSSE_orig = a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_origem, IdSubmercado());
				const IdSubmercado idSSE_dest = a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_destino, IdSubmercado());

				// Variável PI
				if (getVarDecisao_PIseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio, idSSE_orig, idSSE_dest) > -1) {

					if (idSSE_orig == idSSE)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio, idSSE_orig, idSSE_dest), posEquAD, -1.0);

					else if (idSSE_dest == idSSE)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio, idSSE_orig, idSSE_dest), posEquAD, 1.0);

				} // if (getVarDecisao_PIseExistir(a_idEstagio, a_period, a_idPat, idIntercambio) > -1){

			} // for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= maiorIdIntercambio; idIntercambio++) {

			//Consumo das usinas elevatórias (QBOM)

			for (IdUsinaElevatoria idUsinaElevatoria = idUsiElHIni; idUsinaElevatoria < idUsiElHOut; a_dados.vetorUsinaElevatoria.incr(idUsinaElevatoria)) {

				if (a_dados.getAtributo(idUsinaElevatoria, AttComumUsinaElevatoria_submercado, IdSubmercado()) == idSSE) {

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PBOMDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria), posEquAD, -1.0);

				}//if (a_dados.getAtributo(idUsinaElevatoria, AttComumUsinaElevatoria_submercado, IdSubmercado()) == idSSE) {

			}//for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria <= a_maiorIdUsinaElevatoria; idUsinaElevatoria++) {

		} // for (IdSubmercado idSSE = idSSEIni; idSSE < idSSEOut; a_dados.vetorSubmercado.incr(idSSE)) {

	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarAtendimentoDemanda(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarFuncaoProducaoHidreletrica(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPat, Periodo &a_period, Periodo& a_periodNext, Periodo& a_periodEnd_stage) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;


		const TipoDetalhamentoProducaoHidreletrica tipo_detalhamento_producao = a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica());

		if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina) {

			const int numero_planos = a_dados.getSize2Matriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, a_period);

			if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica) > -1) {

				if ((a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_vazao_turbinada_maxima, a_period, double()) > 0) && \
					(a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_potencia_maxima, a_period, double()) > 0)) {

					for (int i = 1; i <= numero_planos; i++) {

						int posRestricaoPH = -1;

						if (numero_planos == 1) {
							posRestricaoPH = addEquLinear_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);
							if (a_dados.getSize1Matriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_SH) > 0) {
								if (a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_SH, a_period, i, double()) != 0.0) {
									int varPH_FPH_FINF = getVarDecisao_PH_FPH_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);
									if (varPH_FPH_FINF == -1) {
										varPH_FPH_FINF = addVarDecisao_PH_FPH_FINF(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, 0.0, vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPH_FPH_FINF, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_penalidade_potencia_minima, double()));
									}
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPH_FPH_FINF, posRestricaoPH, 1.0);
								}
							}
						}
						else {
							posRestricaoPH = addIneLinear_PH(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, i);
							int varPH_FPH_FINF = getVarDecisao_PH_FPH_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica);
							if (varPH_FPH_FINF == -1) {
								varPH_FPH_FINF = addVarDecisao_PH_FPH_FINF(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, 0.0, vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPH_FPH_FINF, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_penalidade_potencia_minima, double()));
							}

							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPH_FPH_FINF, posRestricaoPH, 1.0);
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
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_QH, a_period, i, double()));
						else
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_QH, a_period, i, double()));

						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QVER(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_SH, a_period, i, double()));

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

			if (a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_vazao_turbinada_maxima, a_period, double()) > 0.0) {

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
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_QH, a_period, i, double()));
								else
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_QH, a_period, i, double()));

								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QVER(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_SH, a_period, i, double()));

								// VAZÃO TURBINADA DOS DEMAIS CONJUNTOS, É CONSIDERADA COMO VERTIMENTO PARA O ATUAL CONJUNTO
								for (IdConjuntoHidraulico idConjunto = IdConjuntoHidraulico_1; idConjunto <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjunto++)
									if (idConjunto != idConjuntoHidraulico)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjunto), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_SH, a_period, i, double()));

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
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_QH, a_period, i, double()));
									else
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_QH, a_period, i, double()));

									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QVER(a_TSS, a_idEstagio, a_period, a_idPat, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_SH, a_period, i, double()));

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
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarFuncaoProducaoHidreletrica(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesCusto(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodIni_stage, Periodo& a_period, const IdPatamarCarga a_idPat) {

	try {

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;


		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();


		if ((a_period == a_periodIni_stage) && (a_idPat == IdPatamarCarga_1)){


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
		} // if ((a_period == a_periodIni_stage) && (a_idPat == IdPatamarCarga_1)){



		if (a_idPat == IdPatamarCarga_1) {
			if (a_TSS != TipoSubproblemaSolver_mestre) {

				// Custo Operacao Valor Presente (ZO0p) Por PeriodoEstudo
				addVarDecisao_ZO0(a_TSS, a_idEstagio, a_period, 0.0, infinito, 0.0);


				// Custo Operacao (ZOp) Por PeriodoEstudo	
				addVarDecisao_ZO(a_TSS, a_idEstagio, a_period, 0.0, infinito, 0.0);


				// Custo Penalidade Valor Presente (ZP0p) Por PeriodoEstudo
				addVarDecisao_ZP0(a_TSS, a_idEstagio, a_period, 0.0, infinito, 0.0);


				// CUSTO DE PENALIDADE (ZP) POR PeriodoEstudo (contabiliza as penalidades das variáveis que não dependem do patamar de carga, ex, restrições de volume, evaporação)
				addVarDecisao_ZP(a_TSS, a_idEstagio, a_period, 0.0, infinito, 0.0);




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
				catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::Restricao Custo Total: \n" + std::string(erro.what())); }


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

									const double probabilidade_abertura = getElementoMatriz(IdProcessoEstocastico_1, AttMatrizProcessoEstocastico_probabilidade_realizacao, periodo_otimizacao_seguinte, idRealizacao, double());

									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZF(a_TSS, a_idEstagio, idRealizacao), posEquZF, -probabilidade_abertura);

								}//else if (multiplicidade_corte > 0) {

							}// for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getAtributo(idEstagio_seguinte, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); idRealizacao++) {

						} // if (posEquZF < 0) {

					} // if (a_idEstagio < getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio())) {

				} // try{
				catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::Restricao Custo Futuro: \n" + std::string(erro.what())); }

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
				catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::Restricao Custo Presente: \n" + std::string(erro.what())); }


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
				catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::Restricao ZP0: \n" + std::string(erro.what())); }


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
				catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::Restricao ZO0: \n" + std::string(erro.what())); }

			} // if (a_TSS != TipoSubproblemaSolver_mestre) {

			else if (a_TSS == TipoSubproblemaSolver_mestre) {

				// ---------------------------------------------------------------------------------------------------------------------
				//                                  Restrição Custo Total no problema Mestre (ZT = ZT_IdRealizacao_1 + .... + ZT_IdRealizacao_N)
				// ---------------------------------------------------------------------------------------------------------------------

				try {

					int posEquZT = getEquLinear_ZT_MESTREseExistir(a_TSS, a_idEstagio);

					if (posEquZT < 0) {

						Periodo period_stage = getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

						const SmartEnupla<IdRealizacao, double> probabilidade_realizacao = getElementosMatriz(IdProcessoEstocastico_1, AttMatrizProcessoEstocastico_probabilidade_realizacao, period_stage, IdRealizacao(), double());

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
				catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::Restricao Custo Total no problema Mestre: \n" + std::string(erro.what())); }

			} // else if (a_TSS == TipoSubproblemaSolver_mestre) {
		}

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		// CUSTO OPERAÇÃO (ZO) POR PeriodoEstudo PorPatamarCarga
		addVarDecisao_ZO(a_TSS, a_idEstagio, a_period, a_idPat, 0.0, infinito, 0.0);

		// CUSTO DE PENALIDADE (ZP) POR PeriodoEstudo PorPatamarCarga
		addVarDecisao_ZP(a_TSS, a_idEstagio, a_period, a_idPat, 0.0, infinito, 0.0);


		if (true) {

			int posEquZPp = getEquLinear_ZPseExistir(a_TSS, a_idEstagio, a_period);

			if (posEquZPp < 0) {

				posEquZPp = addEquLinear_ZP(a_TSS, a_idEstagio, a_period);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZPp, 0.0);

				// Variável ZP Por PeriodoEstudo
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZP(a_TSS, a_idEstagio, a_period), posEquZPp, 1.0);

			} // if (posEquZPp < 0) {

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZP(a_TSS, a_idEstagio, a_period, a_idPat), posEquZPp, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, a_idPat, double()));

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

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZO(a_TSS, a_idEstagio, a_period, a_idPat), posEquZOp, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, a_idPat, double()));

		}


		const int posEquZO = addEquLinear_ZO(a_TSS, a_idEstagio, a_period, a_idPat);

		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZO, 0.0);

		// Variável ZO
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZO(a_TSS, a_idEstagio, a_period, a_idPat), posEquZO, 1.0);


		const int posEquZP = addEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat);

		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZP, 0.0);

		// Variável ZP
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZP(a_TSS, a_idEstagio, a_period, a_idPat), posEquZP, 1.0);

	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesCusto(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}

void ModeloOtimizacao::criarHidreletricas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodIni_stage, Periodo& a_periodEnd_stage, Periodo& a_periodPrev, Periodo& a_period, Periodo& a_periodNext, const IdPatamarCarga a_idPat, const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>& a_horizon) {

	try {

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const IdEstagio idStageIni = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
		const IdEstagio idStageEnd = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

		const IdPatamarCarga idPatEnd = a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, a_period, IdPatamarCarga());
		const double perc_dur_pat = a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, a_idPat, double());

		const double vlr_max = getdoubleFromChar("max");

		const IdHidreletrica idUHEIni = a_dados.getMenorId(IdHidreletrica());
		const IdHidreletrica idUHEOut = a_dados.getIdOut(IdHidreletrica());
		for (IdHidreletrica idUHE = idUHEIni; idUHE < idUHEOut; a_dados.vetorHidreletrica.incr(idUHE)) {

			// ------------------------------------------------------------------------------------------------------------------------------------------------------
			//
			//                                                                    Reservatorio
			//
			// ------------------------------------------------------------------------------------------------------------------------------------------------------
			if (a_idPat == IdPatamarCarga_1) {

				const double volume_minimo = a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_volume_minimo, a_period, double());

				const int regularizacao_period = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_regularizacao, a_period, int());

				double volume_util_minimo = 0.0;
				double volume_util_maximo = 0.0;			
				if ((regularizacao_period > 0) && ((a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) || (a_idEstagio != idStageEnd))) {
					volume_util_minimo = a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_volume_util_minimo, a_period, double());
					volume_util_maximo = a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, a_period, double());
				}

				const double volume_util_inicial = a_dados.getAtributo(idUHE, IdReservatorio_1, AttComumReservatorio_volume_util_inicial, double());

				int regularizacao_periodPrev = 0;

				double volume_util_minimo_prev = volume_util_inicial;
				double volume_util_maximo_prev = volume_util_inicial;
				if ((a_idEstagio > idStageIni) || (a_periodIni_stage < a_period)) {
					regularizacao_periodPrev = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_regularizacao, a_periodPrev, int());
					if ((a_period == a_periodIni_stage) && (regularizacao_periodPrev == 0)) {
						volume_util_minimo_prev = 0.0;
						volume_util_maximo_prev = 0.0;
					}
					else {
						volume_util_minimo_prev = a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_volume_util_minimo, a_periodPrev, double());
						volume_util_maximo_prev = a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, a_periodPrev, double());
					}
				}

				//
				// VI: Volume Inicial no periodo
				// 
				int varVI = getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_period, idUHE);
				if (varVI == -1)
					varVI = addVarDecisao_VI(a_TSS, a_idEstagio, a_period, idUHE, volume_util_minimo_prev, volume_util_maximo_prev, 0.0);

				//
				// VF: Volume Final no periodo
				//
				int varVF = -1;
				if (a_period < a_periodEnd_stage)
					varVF = addVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, idUHE, volume_util_minimo, volume_util_maximo, 0.0);
				else if (a_period == a_periodEnd_stage)
					varVF = addVarDecisao_VF(a_TSS, a_idEstagio, a_period, idUHE, volume_util_minimo, volume_util_maximo, 0.0);


				//ENCHENDO VOLUME MORTO
				if (a_dados.getSizeVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_enchendo_volume_morto) > 0) {
					if (a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_enchendo_volume_morto, a_period, int()) == 1) {
						const double volume_minimo = a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_volume_minimo, a_period, double());
						if (volume_minimo > 0.0) {
							const int varVMORTO_FINF = addVarDecisao_VMORTO_FINF(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varVMORTO_FINF, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_volume_minimo, double()));
							const int ineVMORTO_ENCH = addIneLinear_VMORTO_ENCH(a_TSS, a_idEstagio, a_period, idUHE);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineVMORTO_ENCH, volume_minimo);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varVF, ineVMORTO_ENCH, 1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varVMORTO_FINF, ineVMORTO_ENCH, 1.0);
						}
					}
				}


				//
				// VF_prev: VI incorpora Volume Final do periodo anterior via variavel de estado
				// 
				if ((a_period == a_periodIni_stage) && (volume_util_maximo_prev > 0.0) && (regularizacao_periodPrev || ((a_idEstagio == idStageIni) && (idStageIni > IdEstagio_1)))) {
					if ((a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica) || (getVarDecisao_VFseExistir(a_TSS, IdEstagio(a_idEstagio - 1), a_periodPrev, idUHE) == -1))
						vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeSolverVarDecisao_VI(a_TSS, a_idEstagio, a_period, idUHE) + "," + getString(volume_util_maximo_prev)), varVI, -1);
					else
						vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeSolverVarDecisao_VI(a_TSS, a_idEstagio, a_period, idUHE) + "," + getString(volume_util_maximo_prev)), varVI, getVarDecisao_VF(a_TSS, IdEstagio(a_idEstagio - 1), a_periodPrev, idUHE));
				} // if ((a_period == a_periodIni_stage) && (volume_util_maximo_prev > 0.0) && (regularizacao_periodPrev || ((a_idEstagio == idStageIni) && (idStageIni > IdEstagio_1)))) {

				//
				// VMED: Volume medio no periodo
				//
				const int var_VMED = addVarDecisao_VMED(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);
				const int equ_VMED = addEquLinear_VMED(a_TSS, a_idEstagio, a_period, idUHE);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(var_VMED, equ_VMED, 1.0);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varVI, equ_VMED, -0.5);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varVF, equ_VMED, -0.5);

				//
				// VH_FINF: Folga para volume minimo
				//

				if (volume_util_minimo > 0.0) {

					int ineVH_FINF = getIneLinear_VH_LINFseExistir(a_TSS, a_idEstagio, a_period, idUHE);

					if (ineVH_FINF == -1) {

						ineVH_FINF = addIneLinear_VH_LINF(a_TSS, a_idEstagio, a_period, idUHE);
						int varVH_FINF = getVarDecisao_VH_FINFseExistir(a_TSS, a_idEstagio, a_period, idUHE);
						if (varVH_FINF == -1)
							varVH_FINF = addVarDecisao_VH_FINF(a_TSS, a_idEstagio, a_period, idUHE, 0.0, volume_util_minimo, 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineVH_FINF, volume_util_minimo);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varVH_FINF, ineVH_FINF, 1.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varVF, ineVH_FINF, 1.0);

						if (a_dados.getAtributo(AttComumDados_mes_penalizacao_volume_util_minimo, IdMes()) != IdMes_Nenhum) {
							Periodo periodo_penalizacao;
							criarVariaveisDecisao_VariaveisEstado_Restricoes_ZP0_VH_LINF(a_TSS, a_dados, a_idEstagio, periodo_penalizacao);
						}
						else {
							int equZP0_VH_LINF = getEquLinear_ZP0_VH_LINFseExistir(a_TSS, a_idEstagio, a_period);
							int varZP0_VH_LINF = getVarDecisao_ZP0_VH_LINFseExistir(a_TSS, a_idEstagio, a_period);
							if (equZP0_VH_LINF == -1) {
								equZP0_VH_LINF = addEquLinear_ZP0_VH_LINF(a_TSS, a_idEstagio, a_period);
								varZP0_VH_LINF = addVarDecisao_ZP0_VH_LINF(a_TSS, a_idEstagio, a_period, 0.0, infinito, 0.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varZP0_VH_LINF, equZP0_VH_LINF, 1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varZP0_VH_LINF, getEquLinear_ZP0(a_TSS, a_idEstagio), -1.0);
							}
							const double valor_penalidade = a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_volume_util_minimo, double()) * a_dados.getElementoMatriz(AttMatrizDados_desagio_acumulado_horizonte_estudo, a_idEstagio, a_period, double());
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varVH_FINF, equZP0_VH_LINF, -valor_penalidade);
						} // else {

					}//if (ineVH_FINF == -1) {
				} // if (idStage_VH != IdEstagio_Nenhum) {


				//
				// QEVAP: Vazao Evaporada Reservatorio
				//
				const double coef_linear_evaporacao_0 = a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_coef_linear_evaporacao_0, a_period, double());
				const double coef_linear_evaporacao_1 = a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_coef_linear_evaporacao_1, a_period, double());
				if ((coef_linear_evaporacao_0 != 0.0) || (coef_linear_evaporacao_1 != 0)) {

					const double coef_fixo = coef_linear_evaporacao_1 * volume_minimo + coef_linear_evaporacao_0;

					const int varQEVA = addVarDecisao_QEVA(a_TSS, a_idEstagio, a_period, idUHE, -infinito, infinito, 0.0);
					const int equQEVA = addEquLinear_QEVA(a_TSS, a_idEstagio, a_period, idUHE);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(equQEVA, coef_fixo);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQEVA, equQEVA, 1.0);

					if (coef_fixo > 0.0) {
						const int varQEVA_FINF = addVarDecisao_QEVA_FINF(a_TSS, a_idEstagio, a_period, idUHE, 0.0, coef_fixo, 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQEVA_FINF, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_evaporacao, double()));
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQEVA_FINF, equQEVA, 1.0);
					}

					if (getVarDecisao_VMEDseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VMED(a_TSS, a_idEstagio, a_period, idUHE), equQEVA, -coef_linear_evaporacao_1);

				} // if ((coef_linear_evaporacao_0 != 0.0) || (coef_linear_evaporacao_1 != 0)) {
			}//if (a_idPat == IdPatamarCarga_1) {

			// ------------------------------------------------------------------------------------------------------------------------------------------------------
			//
			//                                                                    Produção
			//
			// ------------------------------------------------------------------------------------------------------------------------------------------------------

			const TipoDetalhamentoProducaoHidreletrica tipo_detalhamento_producao_hidreletrica = a_dados.getAtributo(idUHE, AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica());

			bool disponivel = false;

			//
			// Por Hidreletrica
			//
			if (tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_usina) {

				const double disponibilidade = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_disponibilidade, a_period, double());

				if (disponibilidade > 0.0) {

					disponivel = true;

					const double vazao_turbinada_minima = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_turbinada_minima, a_period, double());
					const double vazao_turbinada_maxima = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_turbinada_maxima, a_period, double());

					//
					// QH: Turbinamento Hidreletrico
					//
					const int varQTUR_pat = addVarDecisao_QTUR(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, vazao_turbinada_maxima, 0.0);
					if (vazao_turbinada_minima > 0.0) {
						const int varQTUR_FINF_pat = addVarDecisao_QTUR_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, vazao_turbinada_minima, 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQTUR_FINF_pat, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_turbinamento_minimo, double()));
						const int ineQTUR_LINF_pat = addIneLinear_QTUR_LINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQTUR_pat, ineQTUR_LINF_pat, 1.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQTUR_FINF_pat, ineQTUR_LINF_pat, 1.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineQTUR_LINF_pat, vazao_turbinada_minima);
					} // if (vazao_turbinada_minima > 0.0) {

					const int varQTURDISP_pat = addVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, vazao_turbinada_minima * disponibilidade, vazao_turbinada_maxima * disponibilidade, 0.0);
					const int equQTURDISP_pat = addEquLinear_QTURDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQTURDISP_pat, equQTURDISP_pat, 1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQTUR_pat, equQTURDISP_pat, -disponibilidade);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQTURDISP_pat, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_turbinamento, double()));

					if (idPatEnd > IdPatamarCarga_1) {

						int varQTURDISP = getVarDecisao_QTURDISPseExistir(a_TSS, a_idEstagio, a_period, idUHE);
						if (varQTURDISP == -1)
							varQTURDISP = addVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);

						int equQTURDISP = getEquLinear_QTURDISPseExistir(a_TSS, a_idEstagio, a_period, idUHE);
						if (equQTURDISP == -1) {
							equQTURDISP = addEquLinear_QTURDISP(a_TSS, a_idEstagio, a_period, idUHE);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQTURDISP, equQTURDISP, 1.0);
						}
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQTURDISP_pat, equQTURDISP, -perc_dur_pat);

					} // if (idPatEnd > IdPatamarCarga_1) {

					if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

						const double potencia_minima = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_potencia_minima, a_period, double());
						const double potencia_maxima = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_potencia_maxima, a_period, double());

						//
						// PH: Potencia Hidreletrica
						//
						const int varPH = addVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, potencia_maxima, 0.0);
						if (potencia_minima > 0.0) {
							const int varPH_FINF = addVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, potencia_minima, 0.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPH_FINF, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_potencia_minima, double()));
							const int inePH_LINF = addIneLinear_PH_LINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPH, inePH_LINF, 1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPH_FINF, inePH_LINF, 1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(inePH_LINF, potencia_minima);
						} // if (potencia_minima > 0.0) {

						//
						// PHDISP: Potencia Disponivel Hidreletrica
						//
						double potencia_disponivel_maxima = potencia_maxima * disponibilidade;
						if (a_dados.getSize1Matriz(idUHE, AttMatrizHidreletrica_potencia_disponivel_maxima)) {
							if (potencia_disponivel_maxima > a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_disponivel_maxima, a_period, a_idPat, double()))
								potencia_disponivel_maxima = a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_disponivel_maxima, a_period, a_idPat, double());
						}

						const int varPHDISP_pat = addVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, potencia_disponivel_maxima, 0.0);
						const int equPHDISP_pat = addEquLinear_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP_pat, equPHDISP_pat,  1.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPH, equPHDISP_pat, -disponibilidade);

						double potencia_disponivel_minima = potencia_minima * disponibilidade;
						if (a_dados.getSize1Matriz(idUHE, AttMatrizHidreletrica_potencia_disponivel_minima)) {
							if (potencia_disponivel_minima < a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_disponivel_minima, a_period, a_idPat, double()))
								potencia_disponivel_minima = a_dados.getElementoMatriz(idUHE, AttMatrizHidreletrica_potencia_disponivel_minima, a_period, a_idPat, double());
						}

						if (potencia_disponivel_minima > 0.0) {
							const int varPHDISP_FINF = addVarDecisao_PHDISP_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, potencia_disponivel_minima, 0.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP_FINF, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_potencia_minima, double()));
							const int inePHDISP_LINF = addIneLinear_PHDISP_LINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP_pat, inePHDISP_LINF, 1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP_FINF, inePHDISP_LINF, 1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(inePHDISP_LINF, potencia_disponivel_minima);
						} // if (potencia_disponivel_minima > 0.0) {


						if (idPatEnd > IdPatamarCarga_1) {

							int varPHDISP = getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, idUHE);
							if (varPHDISP == -1)
								varPHDISP = addVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);

							int equPHDISP = getEquLinear_PHDISPseExistir(a_TSS, a_idEstagio, a_period, idUHE);
							if (equPHDISP == -1) {
								equPHDISP = addEquLinear_PHDISP(a_TSS, a_idEstagio, a_period, idUHE);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP, equPHDISP, 1.0);
							}
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP_pat, equPHDISP, -perc_dur_pat);

						} // if (idPatEnd > IdPatamarCarga_1) {

					} // if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {
				} // if (a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_disponibilidade, a_period, double()) > 0.0) {

			} // if (tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_usina) {

			//
			// Por Conjunto Hidraulico
			//
			else if (tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

				for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idUHE, IdConjuntoHidraulico()); idConjuntoHidraulico++) {
					throw std::invalid_argument("Not available.");
				} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idUHE, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

			} // if (tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {


			//
			// Por Unidade Hidraulica
			//
			else if (tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_unidade) {

				for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idUHE, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

					for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(idUHE, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {
						throw std::invalid_argument("Not available.");
					} // for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= maiorIdUnidadeUHE; idUnidadeUHE++) {
				} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {

			} // else if (tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_unidade) {

			//
			// Potencia Disponivel por submercado
			//
			const int varPHDISP_pat = getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE);
			if ((varPHDISP_pat > -1) && (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica)) {

				const IdSubmercado idSSE = a_dados.getAtributo(idUHE, AttComumHidreletrica_submercado, IdSubmercado());

				int varPHDISP_pat_sse = getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
				int equPHDISP_pat_sse = getEquLinear_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
				if (varPHDISP_pat_sse == -1) {
					varPHDISP_pat_sse = addVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, 0.0, infinito, 0.0);
					equPHDISP_pat_sse = addEquLinear_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP_pat_sse, equPHDISP_pat_sse, 1.0);
				} // if (varPHDISP_sse == -1) {
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP_pat, equPHDISP_pat_sse, -1.0);

				int varPHDISP_pat_sin = getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
				int equPHDISP_pat_sin = getEquLinear_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
				if (varPHDISP_pat_sin == -1) {
					varPHDISP_pat_sin = addVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN, 0.0, infinito, 0.0);
					equPHDISP_pat_sin = addEquLinear_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP_pat_sin, equPHDISP_pat_sin, 1.0);
				} // if (varPHDISP_sse == -1) {
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP_pat_sse, equPHDISP_pat_sin, -1.0);

				// Variável media
				if (idPatEnd > IdPatamarCarga_1) {

					int varPHDISP_sse = getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, idSSE);
					int equPHDISP_sse = getEquLinear_PHDISPseExistir(a_TSS, a_idEstagio, a_period, idSSE);
					if (varPHDISP_sse == -1) {
						varPHDISP_sse = addVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, idSSE, 0.0, infinito, 0.0);
						equPHDISP_sse = addEquLinear_PHDISP(a_TSS, a_idEstagio, a_period, idSSE);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP_sse, equPHDISP_sse, 1.0);
					} // 
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP_pat_sse, equPHDISP_sse, -perc_dur_pat);

					int varPHDISP_sin = getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);
					int equPHDISP_sin = getEquLinear_PHDISPseExistir(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);
					if (varPHDISP_sin == -1) {
						varPHDISP_sin = addVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN, 0.0, infinito, 0.0);
						equPHDISP_sin = addEquLinear_PHDISP(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP_sin, equPHDISP_sin, 1.0);
					} // 
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP_pat_sin, equPHDISP_sin, -perc_dur_pat);

				} // if (idPatEnd > IdPatamarCarga_1) {
			} // if ((varPHDISP_pat > -1) && (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica)) {

			// ------------------------------------------------------------------------------------------------------------------------------------------------------
			//
			//                                                                    Demais variaveis hidraulicas
			//
			// ------------------------------------------------------------------------------------------------------------------------------------------------------

			//
			// QMON: Vazao Montante Hidreletrica
			//
			const int varQMON_pat = addVarDecisao_QMON(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, -infinito, infinito, 0.0);
			if (idPatEnd > IdPatamarCarga_1) {
				int equQMON = getEquLinear_QMONseExistir(a_TSS, a_idEstagio, a_period, idUHE);
				if (equQMON == -1) {
					equQMON = addEquLinear_QMON(a_TSS, a_idEstagio, a_period, idUHE);
					const int varQMON = addVarDecisao_QMON(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQMON, equQMON, 1.0);
				}
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQMON_pat, equQMON, -perc_dur_pat);
			} // if (idPatEnd > IdPatamarCarga_1) {
			
			//
			// QVER: Vazao Vertida Hidreletrica
			//
			const int varQVER_pat = addVarDecisao_QVER(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, infinito, 0.0);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQVER_pat, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_vertimento, double()));
			if (idPatEnd > IdPatamarCarga_1) {
				int equQVER = getEquLinear_QVERseExistir(a_TSS, a_idEstagio, a_period, idUHE);
				if (equQVER == -1) {
					equQVER = addEquLinear_QVER(a_TSS, a_idEstagio, a_period, idUHE);
					const int varQVER = addVarDecisao_QVER(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQVER, equQVER, 1.0);
				}
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQVER_pat, equQVER, -perc_dur_pat);
			} // if (idPatEnd > IdPatamarCarga_1) {

			//
			// QDEF: Vazao Defluente Hidreletrica
			//
			const int equQDEF_pat = addEquLinear_QDEF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE);
			int varQDEF_pat = getVarDecisao_QDEFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE);
			// varQDEF_pat pode tambem ter sido criada no método recursivo que cria QDEF viajante no balanco hidrico
			if (varQDEF_pat == -1)
				varQDEF_pat = addVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, infinito, 0.0);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDEF_pat, equQDEF_pat, 1.0);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQVER_pat, equQDEF_pat, -1.0);
			const int varQTURDISP_pat = getVarDecisao_QTURDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE);
			if (varQTURDISP_pat > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQTURDISP_pat, equQDEF_pat, -1.0);

			if (a_dados.getSizeVetor(idUHE, AttVetorHidreletrica_vazao_defluente_minima) > 0) {
				const double vazao_defluente_minima = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_defluente_minima, a_period, double());
				if (vazao_defluente_minima > 0.0) {
					const int varQDEF_FINF_pat = addVarDecisao_QDEF_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, vazao_defluente_minima, 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDEF_FINF_pat, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_vazao_defluente_minima, double()));
					const int ineQDEF_LINF_pat = addIneLinear_QDEF_LINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDEF_pat, ineQDEF_LINF_pat, 1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDEF_FINF_pat, ineQDEF_LINF_pat, 1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineQDEF_LINF_pat, vazao_defluente_minima);
				} // if (vazao_defluente_minima > 0.0) {
			} // if (a_dados.getSizeVetor(idUHE, AttVetorHidreletrica_vazao_defluente_minima) > 0) {

			if (a_dados.getSizeVetor(idUHE, AttVetorHidreletrica_vazao_defluente_maxima) > 0) {
				const double vazao_defluente_maxima = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_defluente_maxima, a_period, double());
				if (vazao_defluente_maxima < vlr_max) {
					const int varQDEF_FSUP_pat = addVarDecisao_QDEF_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, vazao_defluente_maxima, 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDEF_FSUP_pat, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_vazao_defluente_maxima, double()));
					const int ineQDEF_LSUP_pat = addIneLinear_QDEF_LSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDEF_pat, ineQDEF_LSUP_pat, -1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDEF_FSUP_pat, ineQDEF_LSUP_pat, 1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineQDEF_LSUP_pat, -vazao_defluente_maxima);
				} // if (vazao_defluente_maxima < vlr_max) {
			} // if (a_dados.getSizeVetor(idUHE, AttVetorHidreletrica_vazao_defluente_maxima) > 0) {

			if (idPatEnd > IdPatamarCarga_1) {
				int equQDEF = getEquLinear_QDEFseExistir(a_TSS, a_idEstagio, a_period, idUHE);
				if (equQDEF == -1) {
					equQDEF = addEquLinear_QDEF(a_TSS, a_idEstagio, a_period, idUHE);
					const int varQDEF = addVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDEF, equQDEF, 1.0);
				}
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDEF_pat, equQDEF, -perc_dur_pat);
			} // if (idPatEnd > IdPatamarCarga_1) {

			//
			// QRET: Vazao Retirada Hidreletrica
			//
			if (a_dados.getSizeVetor(idUHE, AttVetorHidreletrica_vazao_retirada) > 0) {
				const double vazao_retirada = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_retirada, a_period, double());
				int varQRET_pat = -1;
				if (vazao_retirada > 0.0) {
					varQRET_pat = addVarDecisao_QRET(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, vazao_retirada, 0.0);
					const int varQRET_FINF_pat = addVarDecisao_QRET_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, 0.0, vazao_retirada, 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQRET_FINF_pat, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_vazao_retirada, double()));
					const int ineQRET_LINF_pat = addIneLinear_QRET_LINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQRET_pat, ineQRET_LINF_pat, 1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQRET_FINF_pat, ineQRET_LINF_pat, 1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineQRET_LINF_pat, vazao_retirada);
				} // if (vazao_retirada > 0.0) {
				else if (vazao_retirada < 0.0)
					varQRET_pat = addVarDecisao_QRET(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, vazao_retirada, vazao_retirada, 0.0);

				if ((idPatEnd > IdPatamarCarga_1) && (varQRET_pat > -1)) {
					int equQRET = getEquLinear_QRETseExistir(a_TSS, a_idEstagio, a_period, idUHE);
					if (equQRET == -1) {
						equQRET = addEquLinear_QRET(a_TSS, a_idEstagio, a_period, idUHE);
						const int varQRET = addVarDecisao_QRET(a_TSS, a_idEstagio, a_period, idUHE, -infinito, infinito, 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQRET, equQRET, 1.0);
					}
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQRET_pat, equQRET, -perc_dur_pat);
				} // if ((idPatEnd > IdPatamarCarga_1) && (varQRET_pat > -1)) {
			} // if (a_dados.getSizeVetor(idUHE, AttVetorHidreletrica_vazao_retirada) > 0) {

			//
			// QDES: Vazao Desviada via canal a entrar na vazao montante da usina jusante desvio (sem considerar tempo de viagem da agua)
			//
			const IdHidreletrica idUHE_jus_desv = a_dados.getAtributo(idUHE, AttComumHidreletrica_jusante_desvio, IdHidreletrica());
			if (idUHE_jus_desv != IdHidreletrica_Nenhum) {
				const double vazao_desviada_minima = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_desviada_minima, a_period, double());
				const double vazao_desviada_maxima = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_vazao_desviada_maxima, a_period, double());
				const int varQDES_pat = addVarDecisao_QDES(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idUHE_jus_desv, 0.0, vazao_desviada_maxima, 0.0);

				if (vazao_desviada_minima > 0.0) {
					const int varQDES_FINF_pat = addVarDecisao_QDES_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idUHE_jus_desv, 0.0, vazao_desviada_minima, 0.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDES_FINF_pat, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_vazao_desviada_minima, double()));
					const int ineQDES_LINF_pat = addIneLinear_QDES_LINF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idUHE_jus_desv);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDES_pat, ineQDES_LINF_pat, 1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDES_FINF_pat, ineQDES_LINF_pat, 1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineQDES_LINF_pat, vazao_desviada_minima);
				} // if (vazao_desviada_minima > 0.0) {

				if (idPatEnd > IdPatamarCarga_1) {
					int equQDES = getEquLinear_QDESseExistir(a_TSS, a_idEstagio, a_period, idUHE, idUHE_jus_desv);
					if (equQDES == -1) {
						equQDES = addEquLinear_QDES(a_TSS, a_idEstagio, a_period, idUHE, idUHE_jus_desv);
						const int varQDES = addVarDecisao_QDES(a_TSS, a_idEstagio, a_period, idUHE, idUHE_jus_desv, 0.0, infinito, 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDES, equQDES, 1.0);
					}
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDES_pat, equQDES, -perc_dur_pat);
				} // if (idPatEnd > IdPatamarCarga_1) {
			} // if (idUHE_jus_desv != IdHidreletrica_Nenhum) {

		} // for (IdHidreletrica idUHE = idUHEIni; idUHE < idUHEOut; a_dados.vetorHidreletrica.incr(idUHE)) {

		// ------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//                                                                   Balanco Hidraulico
		//
		// ------------------------------------------------------------------------------------------------------------------------------------------------------
		for (IdHidreletrica idUHE = idUHEIni; idUHE < idUHEOut; a_dados.vetorHidreletrica.incr(idUHE)) {

			//
			// FPH: Funcao de Producao Hidreletrica
			//
			if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
				criarFuncaoProducaoHidreletrica(a_TSS, a_dados, a_idEstagio, idUHE, a_idPat, a_period, a_periodNext, a_periodEnd_stage);

			const double volume_util_maximo = a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, a_period, double());
			double volume_util_maximo_next = volume_util_maximo;
			if ((a_period < a_periodEnd_stage) || (a_idEstagio < idStageEnd))
				volume_util_maximo_next = a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, a_periodNext, double());

			// Restrições Balanço Hidraulico Por Volume (BH_VOL)
			if ((volume_util_maximo > 0.0) || (volume_util_maximo_next > 0.0) || (a_dados.getAtributo(AttComumDados_representar_todos_balancos_hidricos_por_volume, bool())))
				criarBalancoHidraulicoPorVolume(a_TSS, a_dados, a_idEstagio, a_periodIni_stage, a_periodEnd_stage, a_periodPrev, a_period, a_periodNext, idUHE, a_idPat, a_horizon);
			
			// Restrições Balanço Hidraulico Por Vazão (BH_VAZ)
			else
				criarBalancoHidraulicoPorVazao(a_TSS, a_dados, a_idEstagio, a_periodIni_stage, a_periodEnd_stage, a_periodPrev, a_period, a_periodNext, idUHE, a_idPat, a_horizon);

		} // for (IdHidreletrica idUHE = idUHEIni; idUHE < idUHEOut; a_dados.vetorHidreletrica.incr(idUHE)) {

	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarHidreletricas(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPat) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarDemandaEspecial(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat) {
	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const IdDemandaEspecial idDEIni = a_dados.getMenorId(IdDemandaEspecial());
		const IdDemandaEspecial idDEOut = a_dados.getIdOut(IdDemandaEspecial());

		for (IdDemandaEspecial idDemandaEspecial = idDEIni; idDemandaEspecial < idDEOut; a_dados.vetorDemandaEspecial.incr(idDemandaEspecial))
			addVarDecisao_PLE(a_TSS, a_idEstagio, a_period, a_idPat, idDemandaEspecial, a_dados.getElementoMatriz(idDemandaEspecial, AttMatrizDemandaEspecial_demanda, a_period, a_idPat, double()), a_dados.getElementoMatriz(idDemandaEspecial, AttMatrizDemandaEspecial_demanda, a_period, a_idPat, double()), 0.0);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarDemandaEspecials(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPat) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarUsinaNaoSimulada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat) {
	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;


		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const IdPatamarCarga idPatEnd = a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, a_period, IdPatamarCarga());
		const double perc_dur_pat = a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, a_idPat, double());

		const IdSubmercado idSSEIni = a_dados.getMenorId(IdSubmercado());
		const IdSubmercado idSSEOut = a_dados.getIdOut(IdSubmercado());

		for (IdSubmercado idSSE = idSSEIni; idSSE < idSSEOut; a_dados.vetorSubmercado.incr(idSSE)) {

			const IdUsinaNaoSimulada menorIdUsinaNaoSimulada = a_dados.getMenorId(idSSE, IdUsinaNaoSimulada());
			const IdUsinaNaoSimulada maiorIdUsinaNaoSimulada = a_dados.getIdOut(idSSE, IdUsinaNaoSimulada());

			for (IdUsinaNaoSimulada idUsinaNaoSimulada = menorIdUsinaNaoSimulada; idUsinaNaoSimulada < maiorIdUsinaNaoSimulada; a_dados.incr(idSSE, idUsinaNaoSimulada)) {
				if (idUsinaNaoSimulada != IdUsinaNaoSimulada_Nenhum) {
					const int varPN_pat = addVarDecisao_PN(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, idUsinaNaoSimulada, a_dados.getElementoMatriz(idSSE, idUsinaNaoSimulada, AttMatrizUsinaNaoSimulada_potencia_minima, a_period, a_idPat, double()), a_dados.getElementoMatriz(idSSE, idUsinaNaoSimulada, AttMatrizUsinaNaoSimulada_potencia_maxima, a_period, a_idPat, double()), 0.0);

					// Var Pat SSE
					int varPN_pat_sse = getVarDecisao_PNseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
					int equPN_pat_sse = getEquLinear_PNseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
					if (varPN_pat_sse == -1) {
						varPN_pat_sse = addVarDecisao_PN(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, 0.0, infinito, 0.0);
						equPN_pat_sse = addEquLinear_PN(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPN_pat_sse, equPN_pat_sse, 1.0);
						// PL_LIQ
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPN_pat_sse, getEquLinear_PL_LIQ(a_TSS, a_idEstagio, a_period, a_idPat, idSSE), 1.0);
					} // if (varPN_sse == -1) {
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPN_pat, equPN_pat_sse, -1.0);

					// Var Pat SIN
					int varPN_pat_sin = getVarDecisao_PNseExistir(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
					int equPN_pat_sin = getEquLinear_PNseExistir(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
					if (varPN_pat_sin == -1) {
						varPN_pat_sin = addVarDecisao_PN(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN, 0.0, infinito, 0.0);
						equPN_pat_sin = addEquLinear_PN(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPN_pat_sin, equPN_pat_sin, 1.0);
					}
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPN_pat_sse, equPN_pat_sin, -1.0);

					// Variáveis Médias
					if (idPatEnd > IdPatamarCarga_1) {

						int varPN = getVarDecisao_PNseExistir(a_TSS, a_idEstagio, a_period, idSSE, idUsinaNaoSimulada);
						if (varPN == -1)
							varPN = addVarDecisao_PN(a_TSS, a_idEstagio, a_period, idSSE, idUsinaNaoSimulada, 0.0, infinito, 0.0);
						int equPN = getEquLinear_PNseExistir(a_TSS, a_idEstagio, a_period, idSSE, idUsinaNaoSimulada);
						if (equPN == -1) {
							equPN = addEquLinear_PN(a_TSS, a_idEstagio, a_period, idSSE, idUsinaNaoSimulada);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPN, equPN, 1.0);
						}
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPN_pat, equPN, -perc_dur_pat);

						int varPN_sse = getVarDecisao_PNseExistir(a_TSS, a_idEstagio, a_period, idSSE);
						if (varPN_sse == -1)
							varPN_sse = addVarDecisao_PN(a_TSS, a_idEstagio, a_period, idSSE, 0.0, infinito, 0.0);
						int equPN_sse = getEquLinear_PNseExistir(a_TSS, a_idEstagio, a_period, idSSE);
						if (equPN_sse == -1) {
							equPN_sse = addEquLinear_PN(a_TSS, a_idEstagio, a_period, idSSE);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPN_sse, equPN_sse, 1.0);
						}
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPN_pat_sse, equPN_sse, -perc_dur_pat);

						int varPN_sin = getVarDecisao_PNseExistir(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);
						if (varPN_sin == -1)
							varPN_sin = addVarDecisao_PN(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN, 0.0, infinito, 0.0);
						int equPN_sin = getEquLinear_PNseExistir(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);
						if (equPN_sin == -1) {
							equPN_sin = addEquLinear_PN(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPN_sin, equPN_sin, 1.0);
						}
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPN_pat_sin, equPN_sin, -perc_dur_pat);

					} // if (idPatEnd > IdPatamarCarga_1) {

				}//if (idUsinaNaoSimulada != IdUsinaNaoSimulada_Nenhum) {
			}//for(IdUsinaNaoSimulada idUsinaNaoSimulada = menorIdUsinaNaoSimulada; idUsinaNaoSimulada < maiorIdUsinaNaoSimulada; a_dados.incr(idSSE, idUsinaNaoSimulada)) {

		}

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarUsinaNaoSimulada(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPat) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}

void ModeloOtimizacao::criarIntercambios(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat) {
	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const IdPatamarCarga idPatEnd = a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, a_period, IdPatamarCarga());
		const double perc_dur_pat = a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, a_idPat, double());

		const IdIntercambio idInterIni = a_dados.getMenorId(IdIntercambio());
		const IdIntercambio idInterOut = a_dados.getIdOut(IdIntercambio());

		for (IdIntercambio idIntercambio = idInterIni; idIntercambio < idInterOut; a_dados.vetorIntercambio.incr(idIntercambio)) {

			const IdSubmercado idSSE_orig = a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_origem, IdSubmercado());
			const IdSubmercado idSSE_dest = a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_destino, IdSubmercado());

			const int varPI_pat = addVarDecisao_PI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio, idSSE_orig, idSSE_dest, 0.0, a_dados.getElementoMatriz(idIntercambio, AttMatrizIntercambio_potencia_maxima, a_period, a_idPat, double()), 0.0);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPI_pat, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getAtributo(idIntercambio, AttComumIntercambio_penalidade_intercambio, double()));

			if (a_dados.getElementoMatriz(idIntercambio, AttMatrizIntercambio_potencia_minima, a_period, a_idPat, double()) > 0.0) {
				//Potência Intercambios Folga Mínima (PI_FINF)
				const int varPI_FINF_pat = addVarDecisao_PI_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio, 0.0, infinito, 0.0);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPI_FINF_pat, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getAtributo(idIntercambio, AttComumIntercambio_penalidade_intercambio_minimo, double()));
			}//if (a_dados.getElementoMatriz(idIntercambio, AttMatrizIntercambio_potencia_minima, a_period, a_idPat, double()) > 0.0) {

			IdSubmercado idSSE = idSSE_orig;

			int varPI_pat_sse = getVarDecisao_PIseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
			int equPI_pat_sse = getEquLinear_PIseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
			if (varPI_pat_sse == -1) {
				varPI_pat_sse = addVarDecisao_PI(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, -infinito, infinito, 0.0);
				equPI_pat_sse = addEquLinear_PI(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPI_pat_sse, equPI_pat_sse, 1.0);
			} // if (varPI_sse == -1) {
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPI_pat, equPI_pat_sse, 1.0);

			// Variável media
			if (idPatEnd > IdPatamarCarga_1) {

				int varPI = getVarDecisao_PIseExistir(a_TSS, a_idEstagio, a_period, idIntercambio, idSSE_orig, idSSE_dest);
				if (varPI == -1)
					varPI = addVarDecisao_PI(a_TSS, a_idEstagio, a_period, idIntercambio, idSSE_orig, idSSE_dest, 0.0, infinito, 0.0);

				int equPI = getEquLinear_PIseExistir(a_TSS, a_idEstagio, a_period, idIntercambio, idSSE_orig, idSSE_dest);
				if (equPI == -1) {
					equPI = addEquLinear_PI(a_TSS, a_idEstagio, a_period, idIntercambio, idSSE_orig, idSSE_dest);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPI, equPI, 1.0);
				}
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPI_pat, equPI, -perc_dur_pat);

				int varPI_sse = getVarDecisao_PIseExistir(a_TSS, a_idEstagio, a_period, idSSE);
				int equPI_sse = getEquLinear_PIseExistir(a_TSS, a_idEstagio, a_period, idSSE);
				if (varPI_sse == -1) {
					varPI_sse = addVarDecisao_PI(a_TSS, a_idEstagio, a_period, idSSE, -infinito, infinito, 0.0);
					equPI_sse = addEquLinear_PI(a_TSS, a_idEstagio, a_period, idSSE);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPI_sse, equPI_sse, 1.0);
				} // 
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPI_pat_sse, equPI_sse, -perc_dur_pat);

			} // if (idPatEnd > IdPatamarCarga_1) {

			idSSE = idSSE_dest;

			varPI_pat_sse = getVarDecisao_PIseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
			equPI_pat_sse = getEquLinear_PIseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
			if (varPI_pat_sse == -1) {
				varPI_pat_sse = addVarDecisao_PI(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, -infinito, infinito, 0.0);
				equPI_pat_sse = addEquLinear_PI(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPI_pat_sse, equPI_pat_sse, 1.0);
			} // if (varPI_sse == -1) {
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPI_pat, equPI_pat_sse, -1.0);

			// Variável media
			if (idPatEnd > IdPatamarCarga_1) {

				int varPI = getVarDecisao_PIseExistir(a_TSS, a_idEstagio, a_period, idIntercambio, idSSE_orig, idSSE_dest);
				if (varPI == -1)
					varPI = addVarDecisao_PI(a_TSS, a_idEstagio, a_period, idIntercambio, idSSE_orig, idSSE_dest, 0.0, infinito, 0.0);

				int equPI = getEquLinear_PIseExistir(a_TSS, a_idEstagio, a_period, idIntercambio, idSSE_orig, idSSE_dest);
				if (equPI == -1) {
					equPI = addEquLinear_PI(a_TSS, a_idEstagio, a_period, idIntercambio, idSSE_orig, idSSE_dest);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPI, equPI, 1.0);
				}
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPI_pat, equPI, -perc_dur_pat);

				int varPI_sse = getVarDecisao_PIseExistir(a_TSS, a_idEstagio, a_period, idSSE);
				int equPI_sse = getEquLinear_PIseExistir(a_TSS, a_idEstagio, a_period, idSSE);
				if (varPI_sse == -1) {
					varPI_sse = addVarDecisao_PI(a_TSS, a_idEstagio, a_period, idSSE, -infinito, infinito, 0.0);
					equPI_sse = addEquLinear_PI(a_TSS, a_idEstagio, a_period, idSSE);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPI_sse, equPI_sse, 1.0);
				} // 
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPI_pat_sse, equPI_sse, -perc_dur_pat);

			} // if (idPatEnd > IdPatamarCarga_1) {


			// Restrição PI_REL
			if (getVarDecisao_PI_FINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio) > -1) {

				const IdSubmercado idSSE_orig = a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_origem, IdSubmercado());
				const IdSubmercado idSSE_dest = a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_destino, IdSubmercado());

				const int posInePI_FINF = addIneLinear_PI_REL(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posInePI_FINF, a_dados.getElementoMatriz(idIntercambio, AttMatrizIntercambio_potencia_minima, a_period, a_idPat, double()));

				// PI
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio, idSSE_orig, idSSE_dest), posInePI_FINF, 1.0);

				// PI_FINF
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idIntercambio), posInePI_FINF, 1.0);

			} // if (getVarDecisao_PI_FINFseExistir(a_idEstagio, a_period, a_idPat, idIntercambio) > -1) {

		} // for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio < a_maiorIdIntercambio; idIntercambio++) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarIntercambios(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPat) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarDemanda(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat) {
	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const IdPatamarCarga idPatEnd = a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, a_period, IdPatamarCarga());
		const double perc_dur_pat = a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, a_idPat, double());

		const IdSubmercado idSSEIni = a_dados.getMenorId(IdSubmercado());
		const IdSubmercado idSSEOut = a_dados.getIdOut(IdSubmercado());

		for (IdSubmercado idSSE = idSSEIni; idSSE < idSSEOut; a_dados.vetorSubmercado.incr(idSSE)) {

			// Variável PL_LIQ_pat
			int varPL_LIQ_pat_sse = getVarDecisao_PL_LIQseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
			int equPL_LIQ_pat_sse = getEquLinear_PL_LIQseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
			if (varPL_LIQ_pat_sse == -1) {
				varPL_LIQ_pat_sse = addVarDecisao_PL_LIQ(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, -infinito, infinito, 0.0);
				equPL_LIQ_pat_sse = addEquLinear_PL_LIQ(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPL_LIQ_pat_sse, equPL_LIQ_pat_sse, 1.0);
			} // if (varPL_sse == -1) {

			int varPL_LIQ_pat_sin = getVarDecisao_PL_LIQseExistir(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
			int equPL_LIQ_pat_sin = getEquLinear_PL_LIQseExistir(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
			if (varPL_LIQ_pat_sin == -1) {
				varPL_LIQ_pat_sin = addVarDecisao_PL_LIQ(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN, 0.0, infinito, 0.0);
				equPL_LIQ_pat_sin = addEquLinear_PL_LIQ(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPL_LIQ_pat_sin, equPL_LIQ_pat_sin, 1.0);
			} // if (varPL_LIQ_pat_sin == -1) {
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPL_LIQ_pat_sse, equPL_LIQ_pat_sin, -1.0);

			// Variável PL media
			if (idPatEnd > IdPatamarCarga_1) {

				int varPL_LIQ_sse = getVarDecisao_PL_LIQseExistir(a_TSS, a_idEstagio, a_period, idSSE);
				if (varPL_LIQ_sse == -1)
					varPL_LIQ_sse = addVarDecisao_PL_LIQ(a_TSS, a_idEstagio, a_period, idSSE, -infinito, infinito, 0.0);

				int equPL_LIQ_sse = getEquLinear_PL_LIQseExistir(a_TSS, a_idEstagio, a_period, idSSE);
				if (equPL_LIQ_sse == -1) {
					equPL_LIQ_sse = addEquLinear_PL_LIQ(a_TSS, a_idEstagio, a_period, idSSE);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPL_LIQ_sse, equPL_LIQ_sse, 1.0);
				}
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPL_LIQ_pat_sse, equPL_LIQ_sse, -perc_dur_pat);

				int varPL_LIQ_sin = getVarDecisao_PL_LIQseExistir(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);
				int equPL_LIQ_sin = getEquLinear_PL_LIQseExistir(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);
				if (varPL_LIQ_sin == -1) {
					varPL_LIQ_sin = addVarDecisao_PL_LIQ(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN, 0.0, infinito, 0.0);
					equPL_LIQ_sin = addEquLinear_PL_LIQ(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPL_LIQ_sin, equPL_LIQ_sin, 1.0);
				} // 
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPL_LIQ_pat_sin, equPL_LIQ_sin, -perc_dur_pat);

			} // if (getVarDecisao_PTseExistir(a_idEstagio, a_period, a_idPat) > -1) {	

			// VARIAVEIS POTÊNCIA PL (PL)
			if (a_dados.getSize1Matriz(idSSE, AttMatrizSubmercado_demanda) > 0) {
				if (a_dados.getSize2Matriz(idSSE, AttMatrizSubmercado_demanda, a_period) > 0) {
					if (a_dados.getElementoMatriz(idSSE, AttMatrizSubmercado_demanda, a_period, a_idPat, double()) > 0.0) {

						const int varPL_pat_sse = addVarDecisao_PL(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, a_dados.getElementoMatriz(idSSE, AttMatrizSubmercado_demanda, a_period, a_idPat, double()), a_dados.getElementoMatriz(idSSE, AttMatrizSubmercado_demanda, a_period, a_idPat, double()), 0.0);

						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPL_pat_sse, equPL_LIQ_pat_sse, -1.0);

						const double denominador = a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, a_idPat, double()) * double(double(a_period.getMinutos()) / 60.0);

						if (denominador == 0) { throw std::invalid_argument("Denominador = 0.0"); }

						setNormalizacaoDualVarDecisao_PL(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, 1.0 / denominador);

						int varPL_pat_sin = getVarDecisao_PLseExistir(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
						int equPL_pat_sin = getEquLinear_PLseExistir(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
						if (varPL_pat_sin == -1) {
							varPL_pat_sin = addVarDecisao_PL(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN, 0.0, infinito, 0.0);
							equPL_pat_sin = addEquLinear_PL(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPL_pat_sin, equPL_pat_sin, 1.0);
						} // if (varPL_sse == -1) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPL_pat_sse, equPL_pat_sin, -1.0);

						// Variável media
						if (idPatEnd > IdPatamarCarga_1) {

							int varPL = getVarDecisao_PLseExistir(a_TSS, a_idEstagio, a_period, idSSE);
							if (varPL == -1)
								varPL = addVarDecisao_PL(a_TSS, a_idEstagio, a_period, idSSE, 0.0, infinito, 0.0);

							int equPL = getEquLinear_PLseExistir(a_TSS, a_idEstagio, a_period, idSSE);
							if (equPL == -1) {
								equPL = addEquLinear_PL(a_TSS, a_idEstagio, a_period, idSSE);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPL, equPL, 1.0);
							}
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPL_pat_sse, equPL, -perc_dur_pat);

							int varPL_sin = getVarDecisao_PLseExistir(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);
							int equPL_sin = getEquLinear_PLseExistir(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);
							if (varPL_sin == -1) {
								varPL_sin = addVarDecisao_PL(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN, 0.0, infinito, 0.0);
								equPL_sin = addEquLinear_PL(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPL_sin, equPL_sin, 1.0);
							} // 
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPL_pat_sin, equPL_sin, -perc_dur_pat);

						} // if (getVarDecisao_PTseExistir(a_idEstagio, a_period, a_idPat) > -1) {				

					} // if (a_dados.getElementoMatriz(idSSE, AttMatrizSubmercado_demanda, a_period, a_idPat, double()) > 0){
				} // if (a_dados.getSize2Matriz(idSSE, AttMatrizSubmercado_demanda, a_period) > 0) {
			} // if (a_dados.getSize1Matriz(idSSE, AttMatrizSubmercado_demanda) > 0) {

		} // for (IdSubmercado idSSE = idSSEIni; idSSE < idSSEOut; a_dados.vetorSubmercado.incr(idSSE)) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarDemanda(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPat) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}



void ModeloOtimizacao::criarDeficit(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat) {
	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const IdPatamarCarga idPatEnd = a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, a_period, IdPatamarCarga());
		const double perc_dur_pat = a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, a_idPat, double());

		const IdSubmercado idSSEIni = a_dados.getMenorId(IdSubmercado());
		const IdSubmercado idSSEOut = a_dados.getIdOut(IdSubmercado());

		for (IdSubmercado idSSE = idSSEIni; idSSE < idSSEOut; a_dados.vetorSubmercado.incr(idSSE)) {

			const IdPatamarDeficit idPatDefIni = a_dados.getMenorId(idSSE, IdPatamarDeficit());
			const IdPatamarDeficit idPatDefOut = a_dados.getIdOut(idSSE, IdPatamarDeficit());

			for (IdPatamarDeficit idPatamarDeficit = idPatDefIni; idPatamarDeficit < idPatDefOut; a_dados.vetorSubmercado.at(idSSE).vetorPatamarDeficit.incr(idPatamarDeficit)) {
				if (a_dados.getElementoMatriz(idSSE, idPatamarDeficit, AttMatrizPatamarDeficit_potencia_maxima, a_period, a_idPat, double()) > 0.0) {

					const int varPD_pat = addVarDecisao_PD(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, idPatamarDeficit, 0.0, a_dados.getElementoMatriz(idSSE, idPatamarDeficit, AttMatrizPatamarDeficit_potencia_maxima, a_period, a_idPat, double()), 0.0);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPD_pat, getEquLinear_ZO(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getElementoMatriz(idSSE, idPatamarDeficit, AttMatrizPatamarDeficit_custo, a_period, a_idPat, double()));

					// Var Pat SSE
					int varPD_pat_sse = getVarDecisao_PDseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
					int equPD_pat_sse = getEquLinear_PDseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
					if (varPD_pat_sse == -1) {
						varPD_pat_sse = addVarDecisao_PD(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, 0.0, infinito, 0.0);
						equPD_pat_sse = addEquLinear_PD(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPD_pat_sse, equPD_pat_sse, 1.0);
					} // if (varPD_sse == -1) {
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPD_pat, equPD_pat_sse, -1.0);

					// Var Pat SIN
					int varPD_pat_sin = getVarDecisao_PDseExistir(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
					int equPD_pat_sin = getEquLinear_PDseExistir(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
					if (varPD_pat_sin == -1) {
						varPD_pat_sin = addVarDecisao_PD(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN, 0.0, infinito, 0.0);
						equPD_pat_sin = addEquLinear_PD(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPD_pat_sin, equPD_pat_sin, 1.0);
					}
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPD_pat_sse, equPD_pat_sin, -1.0);

					// Variáveis Médias
					if (idPatEnd > IdPatamarCarga_1) {

						int varPD = getVarDecisao_PDseExistir(a_TSS, a_idEstagio, a_period, idSSE, idPatamarDeficit);
						if (varPD == -1)
							varPD = addVarDecisao_PD(a_TSS, a_idEstagio, a_period, idSSE, idPatamarDeficit, 0.0, infinito, 0.0);
						int equPD = getEquLinear_PDseExistir(a_TSS, a_idEstagio, a_period, idSSE, idPatamarDeficit);
						if (equPD == -1) {
							equPD = addEquLinear_PD(a_TSS, a_idEstagio, a_period, idSSE, idPatamarDeficit);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPD, equPD, 1.0);
						}
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPD_pat, equPD, -perc_dur_pat);

						int varPD_sse = getVarDecisao_PDseExistir(a_TSS, a_idEstagio, a_period, idSSE);
						if (varPD_sse == -1)
							varPD_sse = addVarDecisao_PD(a_TSS, a_idEstagio, a_period, idSSE, 0.0, infinito, 0.0);
						int equPD_sse = getEquLinear_PDseExistir(a_TSS, a_idEstagio, a_period, idSSE);
						if (equPD_sse == -1) {
							equPD_sse = addEquLinear_PD(a_TSS, a_idEstagio, a_period, idSSE);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPD_sse, equPD_sse, 1.0);
						}
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPD_pat_sse, equPD_sse, -perc_dur_pat);

						int varPD_sin = getVarDecisao_PDseExistir(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);
						if (varPD_sin == -1)
							varPD_sin = addVarDecisao_PD(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN, 0.0, infinito, 0.0);
						int equPD_sin = getEquLinear_PDseExistir(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);
						if (equPD_sin == -1) {
							equPD_sin = addEquLinear_PD(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPD_sin, equPD_sin, 1.0);
						}
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPD_pat_sin, equPD_sin, -perc_dur_pat);

					} // if (idPatEnd > IdPatamarCarga_1) {


				} // if (a_dados.getElementoMatriz(idSSE, idPatamarDeficit, AttMatrizPatamarDeficit_potencia_maxima, a_period, idPat, double()) > 0.0) {
			} // for (IdPatamarDeficit idPatamarDeficit = IdPatamarDeficit_1; idPatamarDeficit < maiorIdPatamarDeficit; idPatamarDeficit++) {
		}

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarDeficit(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPat) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRenovaveis(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat) {
	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const IdRenovavel idRenovavelIni = a_dados.getMenorId(IdRenovavel());
		const IdRenovavel IdRenovavelOut = a_dados.getIdOut(IdRenovavel());

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const IdPatamarCarga idPatEnd = a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, a_period, IdPatamarCarga());
		const double perc_dur_pat = a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, a_idPat, double());

		for (IdRenovavel idRenovavel = idRenovavelIni; idRenovavel < IdRenovavelOut; a_dados.vetorRenovavel.incr(idRenovavel)) {

			int varPR_pat = -1;

			if (a_dados.getAtributo(idRenovavel, AttComumRenovavel_constrained_off, bool()))
				varPR_pat = addVarDecisao_PR(a_TSS, a_idEstagio, a_period, a_idPat, idRenovavel, 0.0, a_dados.getElementoMatriz(idRenovavel, AttMatrizRenovavel_geracao, a_period, a_idPat, double()), 0.0);
			else
				varPR_pat = addVarDecisao_PR(a_TSS, a_idEstagio, a_period, a_idPat, idRenovavel, a_dados.getElementoMatriz(idRenovavel, AttMatrizRenovavel_geracao, a_period, a_idPat, double()), a_dados.getElementoMatriz(idRenovavel, AttMatrizRenovavel_geracao, a_period, a_idPat, double()), 0.0);

			if (varPR_pat > -1) {

				const IdSubmercado idSSE = a_dados.getAtributo(idRenovavel, AttComumRenovavel_submercado, IdSubmercado());

				// Var Pat SSE
				int varPR_pat_sse = getVarDecisao_PRseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
				int equPR_pat_sse = getEquLinear_PRseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
				if (varPR_pat_sse == -1) {
					varPR_pat_sse = addVarDecisao_PR(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, 0.0, infinito, 0.0);
					equPR_pat_sse = addEquLinear_PR(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPR_pat_sse, equPR_pat_sse, 1.0);
					// PL_LIQ
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPR_pat_sse, getEquLinear_PL_LIQ(a_TSS, a_idEstagio, a_period, a_idPat, idSSE), 1.0);
				} // if (varPR_sse == -1) {
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPR_pat, equPR_pat_sse, -1.0);

				// Var Pat SIN
				int varPR_pat_sin = getVarDecisao_PRseExistir(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
				int equPR_pat_sin = getEquLinear_PRseExistir(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
				if (varPR_pat_sin == -1) {
					varPR_pat_sin = addVarDecisao_PR(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN, 0.0, infinito, 0.0);
					equPR_pat_sin = addEquLinear_PR(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPR_pat_sin, equPR_pat_sin, 1.0);
				}
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPR_pat_sse, equPR_pat_sin, -1.0);

				// Variáveis Médias
				if (idPatEnd > IdPatamarCarga_1) {

					int varPR = getVarDecisao_PRseExistir(a_TSS, a_idEstagio, a_period, idRenovavel);
					if (varPR == -1)
						varPR = addVarDecisao_PR(a_TSS, a_idEstagio, a_period, idRenovavel, 0.0, infinito, 0.0);
					int equPR = getEquLinear_PRseExistir(a_TSS, a_idEstagio, a_period, idRenovavel);
					if (equPR == -1) {
						equPR = addEquLinear_PR(a_TSS, a_idEstagio, a_period, idRenovavel);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPR, equPR, 1.0);
					}
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPR_pat, equPR, -perc_dur_pat);

					int varPR_sse = getVarDecisao_PRseExistir(a_TSS, a_idEstagio, a_period, idSSE);
					if (varPR_sse == -1)
						varPR_sse = addVarDecisao_PR(a_TSS, a_idEstagio, a_period, idSSE, 0.0, infinito, 0.0);
					int equPR_sse = getEquLinear_PRseExistir(a_TSS, a_idEstagio, a_period, idSSE);
					if (equPR_sse == -1) {
						equPR_sse = addEquLinear_PR(a_TSS, a_idEstagio, a_period, idSSE);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPR_sse, equPR_sse, 1.0);
					}
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPR_pat_sse, equPR_sse, -perc_dur_pat);

					int varPR_sin = getVarDecisao_PRseExistir(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);
					if (varPR_sin == -1)
						varPR_sin = addVarDecisao_PR(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN, 0.0, infinito, 0.0);
					int equPR_sin = getEquLinear_PRseExistir(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);
					if (equPR_sin == -1) {
						equPR_sin = addEquLinear_PR(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPR_sin, equPR_sin, 1.0);
					}
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPR_pat_sin, equPR_sin, -perc_dur_pat);

				} // if (idPatEnd > IdPatamarCarga_1) {

			} // if (varPR_pat > -1) {

		} // for (IdRenovavel idRenovavel = idRenovavelIni; idRenovavel < IdRenovavelOut; a_dados.vetorRenovavel.incr(idRenovavel)) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRenovaveis(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPat) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarTermeletricas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat) {
	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const IdPatamarCarga idPatEnd = a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, a_period, IdPatamarCarga());
		const double perc_dur_pat = a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, a_idPat, double());

		const IdTermeletrica idUTEIni = a_dados.getMenorId(IdTermeletrica());
		const IdTermeletrica idUTEOut = a_dados.getIdOut(IdTermeletrica());
		for (IdTermeletrica idUTE = idUTEIni; idUTE < idUTEOut; a_dados.vetorTermeletrica.incr(idUTE)) {

				IdUnidadeUTE idUnUTE_equiv = IdUnidadeUTE_Nenhum;

				const TipoDetalhamentoProducaoTermeletrica tipo_detalhamento_producao = a_dados.getAtributo(idUTE, AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica());
				const bool representacao_discreta_producao = a_dados.getAtributo(idUTE, AttComumTermeletrica_representacao_discreta_producao, bool());
				if ((representacao_discreta_producao) && (idPatEnd > IdPatamarCarga_1))
					throw std::invalid_argument("Discrete operation cannot be represented in " + getFullString(idUTE) + " and " + getFullString(a_period));

				const int lag_mensal_potencia_disponivel_comandada = a_dados.getAtributo(idUTE, AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada, int());

				bool disponivel = false;

				// ------------------------------------------------------------------------------------------------------------------------------------------------------
				//
				//                                                                    Produção Por Termeletrica
				//
				// ------------------------------------------------------------------------------------------------------------------------------------------------------

				if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina) {

					const double disponibilidade = a_dados.getElementoVetor(idUTE, AttVetorTermeletrica_disponibilidade, a_period, double());

					if (disponibilidade > 0.0) {

						disponivel = true;

						const double potencia_maxima = a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_minima, a_period, a_idPat, double()) + a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_util, a_period, a_idPat, double());
				
						const int varPT_pat = addVarDecisao_PT(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, 0.0, potencia_maxima, 0.0);
						const int varPTDISP_pat = addVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, 0.0, infinito, 0.0);

						// PTDISP = DISP * PT
						const int equPTDISP_pat = addEquLinear_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_pat, equPTDISP_pat, 1.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPT_pat, equPTDISP_pat, -disponibilidade);

						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_pat, getEquLinear_ZO(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_custo_de_operacao, a_period, a_idPat, double()));

						// PTDISP + PTDISP_LINF_FINF >= PTDISP_LINF
						const int inePTDISP_LINF_pat = addIneLinear_PTDISP_LINF(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);
						const int varPTDISP_LINF_pat = addVarDecisao_PTDISP_LINF(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, 0.0, 0.0, 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_pat, inePTDISP_LINF_pat, 1.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LINF_pat, inePTDISP_LINF_pat, -1.0);

						// PTDISP - PTDISP_LSUP_FSUP <= PTDISP_LSUP
						const int inePTDISP_LSUP_pat = addIneLinear_PTDISP_LSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);
						const int varPTDISP_LSUP_pat = addVarDecisao_PTDISP_LSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, 0.0, infinito, 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_pat, inePTDISP_LSUP_pat, -1.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LSUP_pat, inePTDISP_LSUP_pat, 1.0);

						if (representacao_discreta_producao) {

							// Criar métodos recursivos para determinar estados de varPTDISP_LINF e varPTDISP_LSUP a partir de subproblema binário em t=0

						} // if (representacao_discreta_producao) {

						else if (!representacao_discreta_producao) {

							if (lag_mensal_potencia_disponivel_comandada > 0) {
								// Folgas são adicionadas para o caso de comando divergir de limites disponíveis no período e patamar
								const int varPTDISP_LINF_FINF_pat = addVarDecisao_PTDISP_LINF_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, 0.0, infinito, 0.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LINF_FINF_pat, inePTDISP_LINF_pat, 1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LINF_FINF_pat, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getAtributo(idUTE, AttComumTermeletrica_penalidade_violacao_potencia, double()));

								const int varPTDISP_LSUP_FSUP_pat = addVarDecisao_PTDISP_LSUP_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, 0.0, infinito, 0.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LSUP_FSUP_pat, inePTDISP_LSUP_pat, 1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LSUP_FSUP_pat, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getAtributo(idUTE, AttComumTermeletrica_penalidade_violacao_potencia, double()));
							} // if (lag_mensal_potencia_disponivel_comandada > 0) {

							double potencia_disponivel_minima = 0.0;
							if (a_dados.getSize1Matriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_minima) > 0) {
								if (a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_minima, a_period, a_idPat, double()) > potencia_disponivel_minima)
									potencia_disponivel_minima = a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_minima, a_period, a_idPat, double());
							}

							double potencia_disponivel_maxima = potencia_maxima * disponibilidade;
							if (a_dados.getSize1Matriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_maxima) > 0) {
								if (a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_maxima, a_period, a_idPat, double()) < potencia_disponivel_maxima) 
									potencia_disponivel_maxima = a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_maxima, a_period, a_idPat, double());
							}

							if (potencia_disponivel_minima > potencia_disponivel_maxima)
								throw std::invalid_argument("Error with power in " + getFullString(idUTE) + " on " + getFullString(a_period) + " and " + getFullString(a_idPat));

							if (potencia_disponivel_minima > 0) {
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(varPTDISP_LINF_pat, potencia_disponivel_minima);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(varPTDISP_LINF_pat, potencia_disponivel_minima);
							}

							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(varPTDISP_LSUP_pat, potencia_disponivel_maxima);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(varPTDISP_LSUP_pat, potencia_disponivel_maxima);

						} // else if (!representacao_discreta_producao) {

					} // if (disponibilidade > 0.0) {
				} // if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina) {

				// ------------------------------------------------------------------------------------------------------------------------------------------------------
				//
				//                                                                    Produção Por Unidade Termeletrica
				//
				// ------------------------------------------------------------------------------------------------------------------------------------------------------

				else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {

					const IdUnidadeUTE idUnUTEIni = a_dados.getMenorId(idUTE, IdUnidadeUTE());
					const IdUnidadeUTE idUnUTEOut = a_dados.getIdOut(idUTE, IdUnidadeUTE());

					if (!a_dados.getAtributo(idUTE, AttComumTermeletrica_unidades_simultaneas, bool())) {
						// Define unidade equivalente com maior capacidade de producao
						double potencia_disponivel_maxima_equ = 0.0;
						for (IdUnidadeUTE idUnidadeUTE = idUnUTEIni; idUnidadeUTE < idUnUTEOut; a_dados.incr(idUTE, idUnidadeUTE)) {
							const double disponibilidade = a_dados.getElementoVetor(idUTE, idUnidadeUTE, AttVetorUnidadeUTE_disponibilidade, a_period, double());
							if (disponibilidade > 0.0) {
								const double potencia_disponivel_maxima = disponibilidade * (a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_minima, a_period, a_idPat, double()) + a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_util, a_period, a_idPat, double()));
								if (potencia_disponivel_maxima > potencia_disponivel_maxima_equ) {
									potencia_disponivel_maxima_equ = potencia_disponivel_maxima;
									idUnUTE_equiv = idUnidadeUTE;
								}
							}
						}
					} // if (a_dados.getAtributo(idUTE, AttComumTermeletrica_unidades_simultaneas, bool())) {

					

					for (IdUnidadeUTE idUnidadeUTE = idUnUTEIni; idUnidadeUTE < idUnUTEOut; a_dados.incr(idUTE, idUnidadeUTE)) {

						if ((idUnUTE_equiv == IdUnidadeUTE_Nenhum) || ((idUnidadeUTE == idUnUTE_equiv) && (!representacao_discreta_producao)) || (representacao_discreta_producao)) {

							const double disponibilidade = a_dados.getElementoVetor(idUTE, idUnidadeUTE, AttVetorUnidadeUTE_disponibilidade, a_period, double());

							if (disponibilidade > 0.0) {

								disponivel = true;

								const double potencia_maxima = a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_minima, a_period, a_idPat, double()) + a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_util, a_period, a_idPat, double());

								const int varPT_pat = addVarDecisao_PT(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE, 0.0, potencia_maxima, 0.0);
								const int varPTDISP_pat = addVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE, 0.0, infinito, 0.0);

								// PTDISP = DISP * PT
								const int equPTDISP_pat = addEquLinear_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_pat, equPTDISP_pat, 1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPT_pat, equPTDISP_pat, -disponibilidade);

								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_pat, getEquLinear_ZO(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_custo_de_operacao, a_period, a_idPat, double()));

								// PTDISP + PTDISP_LINF_FINF(se necessario) >= PTDISP_LINF
								const int inePTDISP_LINF_pat = addIneLinear_PTDISP_LINF(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE);
								const int varPTDISP_LINF_pat = addVarDecisao_PTDISP_LINF(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE, 0.0, 0.0, 0.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_pat, inePTDISP_LINF_pat, 1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LINF_pat, inePTDISP_LINF_pat, -1.0);

								// PTDISP - PTDISP_LSUP_FSUP(se necessario) <= PTDISP_LSUP
								const int inePTDISP_LSUP_pat = addIneLinear_PTDISP_LSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE);
								const int varPTDISP_LSUP_pat = addVarDecisao_PTDISP_LSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE, 0.0, infinito, 0.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_pat, inePTDISP_LSUP_pat, -1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LSUP_pat, inePTDISP_LSUP_pat, 1.0);

								if (representacao_discreta_producao) {

									// Criar métodos recursivos para determinar estados de varPTDISP_LINF e varPTDISP_LSUP a partir de subproblema binário em t=0

								} // if (representacao_discreta_producao) {

								else if (!representacao_discreta_producao) {

									if (lag_mensal_potencia_disponivel_comandada > 0) {
										// Folgas são adicionadas para o caso de comando divergir de limites disponíveis no período e patamar
										const int varPTDISP_LINF_FINF_pat = addVarDecisao_PTDISP_LINF_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE, 0.0, infinito, 0.0);
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LINF_FINF_pat, inePTDISP_LINF_pat, 1.0);
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LINF_FINF_pat, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getAtributo(idUTE, AttComumTermeletrica_penalidade_violacao_potencia, double()));

										const int varPTDISP_LSUP_FSUP_pat = addVarDecisao_PTDISP_LSUP_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUnidadeUTE, 0.0, infinito, 0.0);
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LSUP_FSUP_pat, inePTDISP_LSUP_pat, 1.0);
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LSUP_FSUP_pat, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getAtributo(idUTE, AttComumTermeletrica_penalidade_violacao_potencia, double()));
									} // if (lag_mensal_potencia_disponivel_comandada > 0) {

									double potencia_disponivel_minima = 0.0;
									if (a_dados.getSize1Matriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_minima) > 0) {
										if (a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_minima, a_period, a_idPat, double()) > potencia_disponivel_minima)
											potencia_disponivel_minima = a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_minima, a_period, a_idPat, double());
									}
									double potencia_disponivel_maxima = potencia_maxima * disponibilidade;
									if (a_dados.getSize1Matriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_maxima) > 0) {
										if (a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_maxima, a_period, a_idPat, double()) < potencia_disponivel_maxima)
											potencia_disponivel_maxima = a_dados.getElementoMatriz(idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_maxima, a_period, a_idPat, double());
									}

									if (potencia_disponivel_minima > 0) {
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(varPTDISP_LINF_pat, potencia_disponivel_minima);
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(varPTDISP_LINF_pat, potencia_disponivel_minima);
									}

									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(varPTDISP_LSUP_pat, potencia_disponivel_maxima);
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(varPTDISP_LSUP_pat, potencia_disponivel_maxima);

								} // else if (!representacao_discreta_producao) {


								// Variaveis para contabilizacao do despacho global da UTE

								double potencia_disponivel_minima_ute = 0.0;
								if (a_dados.getSize1Matriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_minima) > 0) {
									if (a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_minima, a_period, a_idPat, double()) > potencia_disponivel_minima_ute)
										potencia_disponivel_minima_ute = a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_minima, a_period, a_idPat, double());
								}

								double potencia_disponivel_maxima_ute = infinito;
								if (a_dados.getSize1Matriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_maxima) > 0) {
									if (a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_maxima, a_period, a_idPat, double()) < potencia_disponivel_maxima_ute)
										potencia_disponivel_maxima_ute = a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_maxima, a_period, a_idPat, double());
								}

								int equPT_pat_ute = getEquLinear_PTseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);
								if (equPT_pat_ute == -1) {
									equPT_pat_ute = addEquLinear_PT(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);
									const int varPT_pat_ute = addVarDecisao_PT(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, 0.0, infinito, 0.0);
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPT_pat_ute, equPT_pat_ute, 1.0);
								}
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPT_pat, equPT_pat_ute, -1.0);

								int equPTDISP_pat_ute = getEquLinear_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);
								if (equPTDISP_pat_ute == -1) {
									equPTDISP_pat_ute = addEquLinear_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);
									const int varPTDISP_pat_ute = addVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, potencia_disponivel_minima_ute, potencia_disponivel_maxima_ute, 0.0);
									if (potencia_disponivel_minima_ute > 0.0) {
										const int varPTDISP_LINF_FINF_pat_ute = addVarDecisao_PTDISP_LINF_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, 0.0, infinito, 0.0);
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LINF_FINF_pat_ute, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getAtributo(idUTE, AttComumTermeletrica_penalidade_violacao_potencia, double()));
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LINF_FINF_pat_ute, equPTDISP_pat_ute, -1.0);
									}
									if (potencia_disponivel_maxima_ute < infinito) {
										const int varPTDISP_LSUP_FSUP_pat_ute = addVarDecisao_PTDISP_LSUP_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, 0.0, infinito, 0.0);
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LSUP_FSUP_pat_ute, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getAtributo(idUTE, AttComumTermeletrica_penalidade_violacao_potencia, double()));
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LSUP_FSUP_pat_ute, equPTDISP_pat_ute, 1.0);
									}
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_pat_ute, equPTDISP_pat_ute, 1.0);
								}
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_pat, equPTDISP_pat_ute, -1.0);

								int equPTDISP_LINF_pat_ute = getEquLinear_PTDISP_LINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);
								if (equPTDISP_LINF_pat_ute == -1) {
									equPTDISP_LINF_pat_ute = addEquLinear_PTDISP_LINF(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);
									const int varPTDISP_LINF_pat_ute = addVarDecisao_PTDISP_LINF(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, 0.0, infinito, 0.0);
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LINF_pat_ute, equPTDISP_LINF_pat_ute, 1.0);
								}
								if (potencia_disponivel_minima_ute == 0.0)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LINF_pat, equPTDISP_LINF_pat_ute, -1.0);
								else
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(equPTDISP_LINF_pat_ute, potencia_disponivel_minima_ute);

								int equPTDISP_LSUP_pat_ute = getEquLinear_PTDISP_LSUPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);
								if (equPTDISP_LSUP_pat_ute == -1) {
									equPTDISP_LSUP_pat_ute = addEquLinear_PTDISP_LSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);
									const int varPTDISP_LSUP_pat_ute = addVarDecisao_PTDISP_LSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, 0.0, infinito, 0.0);
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LSUP_pat_ute, equPTDISP_LSUP_pat_ute, 1.0);
								}
								if (potencia_disponivel_maxima_ute == infinito)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LSUP_pat, equPTDISP_LSUP_pat_ute, -1.0);
								else
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(equPTDISP_LSUP_pat_ute, potencia_disponivel_maxima_ute);

							} // if (disponibilidade > 0.0) {
						} // if ((idUnUTE_equiv == IdUnidadeUTE_Nenhum) || (idUnidadeUTE == idUnUTE_equiv)) {
					} // for (IdUnidadeUTE idUnidadeUTE = idUnUTEIni; idUnidadeUTE < idUnUTEOut; a_dados.incr(idUTE, idUnidadeUTE)) {
				} // else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {

				//
				// Potencia Disponivel Termeletrica
				//

				if (disponivel) {

					const IdSubmercado idSSE = a_dados.getAtributo(idUTE, AttComumTermeletrica_submercado, IdSubmercado());

					// varPTDISP
					const int varPTDISP_pat = getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);
					int varPTDISP_pat_sse = getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
					int equPTDISP_pat_sse = getEquLinear_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
					if (varPTDISP_pat_sse == -1) {
						varPTDISP_pat_sse = addVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, 0.0, infinito, 0.0);
						equPTDISP_pat_sse = addEquLinear_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_pat_sse, equPTDISP_pat_sse, 1.0);
					} // if (varPTDISP_sse == -1) {
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_pat, equPTDISP_pat_sse, -1.0);

					int varPTDISP_pat_sin = getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
					int equPTDISP_pat_sin = getEquLinear_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
					if (varPTDISP_pat_sin == -1) {
						varPTDISP_pat_sin = addVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN, 0.0, infinito, 0.0);
						equPTDISP_pat_sin = addEquLinear_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_pat_sin, equPTDISP_pat_sin, 1.0);
					} // if (varPTDISP_sse == -1) {
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_pat_sse, equPTDISP_pat_sin, -1.0);

					// varPTDISP_LINF
					const int varPTDISP_LINF_pat = getVarDecisao_PTDISP_LINF(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);
					int varPTDISP_LINF_pat_sse = getVarDecisao_PTDISP_LINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
					int equPTDISP_LINF_pat_sse = getEquLinear_PTDISP_LINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
					if (varPTDISP_LINF_pat_sse == -1) {
						varPTDISP_LINF_pat_sse = addVarDecisao_PTDISP_LINF(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, 0.0, infinito, 0.0);
						equPTDISP_LINF_pat_sse = addEquLinear_PTDISP_LINF(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LINF_pat_sse, equPTDISP_LINF_pat_sse, 1.0);
					} // if (varPTDISP_LINF_sse == -1) {
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LINF_pat, equPTDISP_LINF_pat_sse, -1.0);

					int varPTDISP_LINF_pat_sin = getVarDecisao_PTDISP_LINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
					int equPTDISP_LINF_pat_sin = getEquLinear_PTDISP_LINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
					if (varPTDISP_LINF_pat_sin == -1) {
						varPTDISP_LINF_pat_sin = addVarDecisao_PTDISP_LINF(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN, 0.0, infinito, 0.0);
						equPTDISP_LINF_pat_sin = addEquLinear_PTDISP_LINF(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LINF_pat_sin, equPTDISP_LINF_pat_sin, 1.0);
					} // if (varPTDISP_LINF_sse == -1) {
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LINF_pat_sse, equPTDISP_LINF_pat_sin, -1.0);

					// varPTDISP_LSUP
					const int varPTDISP_LSUP_pat = getVarDecisao_PTDISP_LSUP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);
					int varPTDISP_LSUP_pat_sse = getVarDecisao_PTDISP_LSUPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
					int equPTDISP_LSUP_pat_sse = getEquLinear_PTDISP_LSUPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
					if (varPTDISP_LSUP_pat_sse == -1) {
						varPTDISP_LSUP_pat_sse = addVarDecisao_PTDISP_LSUP(a_TSS, a_idEstagio, a_period, a_idPat, idSSE, 0.0, infinito, 0.0);
						equPTDISP_LSUP_pat_sse = addEquLinear_PTDISP_LSUP(a_TSS, a_idEstagio, a_period, a_idPat, idSSE);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LSUP_pat_sse, equPTDISP_LSUP_pat_sse, 1.0);
					} // if (varPTDISP_LSUP_sse == -1) {
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LSUP_pat, equPTDISP_LSUP_pat_sse, -1.0);

					int varPTDISP_LSUP_pat_sin = getVarDecisao_PTDISP_LSUPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
					int equPTDISP_LSUP_pat_sin = getEquLinear_PTDISP_LSUPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
					if (varPTDISP_LSUP_pat_sin == -1) {
						varPTDISP_LSUP_pat_sin = addVarDecisao_PTDISP_LSUP(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN, 0.0, infinito, 0.0);
						equPTDISP_LSUP_pat_sin = addEquLinear_PTDISP_LSUP(a_TSS, a_idEstagio, a_period, a_idPat, IdSubmercado_SIN);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LSUP_pat_sin, equPTDISP_LSUP_pat_sin, 1.0);
					} // if (varPTDISP_LSUP_sse == -1) {
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_LSUP_pat_sse, equPTDISP_LSUP_pat_sin, -1.0);

					// Variável media
					if (idPatEnd > IdPatamarCarga_1) {

						const int varPT_pat = getVarDecisao_PT(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);
						int varPT = getVarDecisao_PTseExistir(a_TSS, a_idEstagio, a_period, idUTE);
						int equPT = getEquLinear_PTseExistir(a_TSS, a_idEstagio, a_period, idUTE);
						if (varPT == -1) {
							varPT = addVarDecisao_PT(a_TSS, a_idEstagio, a_period, idUTE, 0.0, infinito, 0.0);
							equPT = addEquLinear_PT(a_TSS, a_idEstagio, a_period, idUTE);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPT, equPT, 1.0);
						} // 
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPT_pat, equPT, -perc_dur_pat);

						int varPTDISP = getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, idUTE);
						int equPTDISP = getEquLinear_PTDISPseExistir(a_TSS, a_idEstagio, a_period, idUTE);
						if (varPTDISP == -1) {
							varPTDISP = addVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, idUTE, 0.0, infinito, 0.0);
							equPTDISP = addEquLinear_PTDISP(a_TSS, a_idEstagio, a_period, idUTE);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP, equPTDISP, 1.0);
						} // 
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_pat, equPTDISP, -perc_dur_pat);

						int varPTDISP_sse = getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, idSSE);
						int equPTDISP_sse = getEquLinear_PTDISPseExistir(a_TSS, a_idEstagio, a_period, idSSE);
						if (varPTDISP_sse == -1) {
							varPTDISP_sse = addVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, idSSE, 0.0, infinito, 0.0);
							equPTDISP_sse = addEquLinear_PTDISP(a_TSS, a_idEstagio, a_period, idSSE);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_sse, equPTDISP_sse, 1.0);
						} // 
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_pat_sse, equPTDISP_sse, -perc_dur_pat);

						int varPTDISP_sin = getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);
						int equPTDISP_sin = getEquLinear_PTDISPseExistir(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);
						if (varPTDISP_sin == -1) {
							varPTDISP_sin = addVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN, 0.0, infinito, 0.0);
							equPTDISP_sin = addEquLinear_PTDISP(a_TSS, a_idEstagio, a_period, IdSubmercado_SIN);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_sin, equPTDISP_sin, 1.0);
						} // 
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP_pat_sin, equPTDISP_sin, -perc_dur_pat);

					} // if (idPatEnd > IdPatamarCarga_1) {

					//////////////////////////////////////////////////////////
					// 
					// Despacho com LAG
					//
					//////////////////////////////////////////////////////////

					if (a_idPat == idPatEnd)
						criarComandoTermeletricas(a_TSS, a_dados, a_idEstagio, a_period, idUTE, idUnUTE_equiv);

				} // if (disponivel) {
		} // for (IdTermeletrica idUTE = a_dados.getMenorId(IdTermeletrica()); idUTE < maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idUTE)) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarTermeletricas(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPat) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}

void ModeloOtimizacao::criarComandoTermeletricas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdTermeletrica a_idUTE, const IdUnidadeUTE a_idUnUTE_equiv){
	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const int lag_mensal_potencia_disponivel_comandada = a_dados.getAtributo(a_idUTE, AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada, int());

		if (lag_mensal_potencia_disponivel_comandada == 0)
			return;

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const IdPatamarCarga idPatEnd = a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, a_period, IdPatamarCarga());

		const TipoDetalhamentoProducaoTermeletrica tipo_detalhamento_producao = a_dados.getAtributo(a_idUTE, AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica());

		int varPTDISP = getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, a_idUTE);
		if (idPatEnd > IdPatamarCarga_1)
			varPTDISP = getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idUTE);

		if (varPTDISP == -1)
			return;

		Periodo periodIni_com_regr;

		//
		// Formulacao
		// 
		//   PTDISP_per = sobr(per,perPreCom) * sumPat(durPat * PTDISPCOM_perPreCom) + sobr(per,perCom) * PTDISPCOM_perCom
		//

		// Períodos precom com Potencia já comandada. Variáveis por patamar de carga
		if (a_dados.getSize1Matriz(a_idUTE, AttMatrizTermeletrica_potencia_disponivel_comandada) > 0) {

			const Periodo periodEnd_precom = a_dados.getIterador1Final(a_idUTE, AttMatrizTermeletrica_potencia_disponivel_comandada, Periodo());
			const Periodo periodNext_precom = Periodo("m", periodEnd_precom + 1);
			const Periodo periodNext = a_period + 1;

			if ((periodNext_precom >= periodNext) || (periodNext_precom > a_period)) {

				if ((periodNext_precom < periodNext) && (periodNext_precom > a_period))
					periodIni_com_regr = periodNext_precom;

				double sobreposicao_acumulada = 0.0;
				const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> horizonte_potencia_disponivel_comandada = a_dados.getMatriz(a_idUTE, AttMatrizTermeletrica_potencia_disponivel_comandada, Periodo(), IdPatamarCarga(), double());
				for (Periodo period_preCom = horizonte_potencia_disponivel_comandada.getIteradorInicial(); period_preCom <= horizonte_potencia_disponivel_comandada.getIteradorFinal(); horizonte_potencia_disponivel_comandada.incrementarIterador(period_preCom)) {

					const Periodo period_preCom_next = period_preCom + 1;

					if (((a_period <= period_preCom) && (period_preCom < periodNext)) || ((a_period < period_preCom_next) && (period_preCom_next <= periodNext))) {

						const double sobreposicao = a_period.sobreposicao(period_preCom);

						if (sobreposicao > 0.0) {

							for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEnd; idPat++) {

								const double perc_dur_pat = a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double());

								const double potencia_disponivel_comandada = horizonte_potencia_disponivel_comandada.at(period_preCom).at(idPat);

								int varPTDISPCOM_pat = getVarDecisao_PTDISPCOMseExistir(a_TSS, a_idEstagio, period_preCom, idPat, a_idUTE);
								if (varPTDISPCOM_pat == -1)
									varPTDISPCOM_pat = addVarDecisao_PTDISPCOM(a_TSS, a_idEstagio, period_preCom, idPat, a_idUTE, potencia_disponivel_comandada, potencia_disponivel_comandada, 0.0);

								int equPTDISPCOM = getEquLinear_PTDISPCOMseExistir(a_TSS, a_idEstagio, a_period, a_idUTE);
								if (equPTDISPCOM == -1) {
									equPTDISPCOM = addEquLinear_PTDISPCOM(a_TSS, a_idEstagio, a_period, a_idUTE);
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP, equPTDISPCOM, 1.0);
								}
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISPCOM_pat, equPTDISPCOM, -sobreposicao * perc_dur_pat);

							} // for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEnd; idPat++) {

							sobreposicao_acumulada += sobreposicao;

							if (doubleCompara(1e-6, sobreposicao_acumulada, 1.0))
								return;

						}
						else if ((sobreposicao == 0.0) && (sobreposicao_acumulada > 0.0))
							break;

					} // if (((a_period <= period_preCom) && (period_preCom < periodNext)) || ((a_period < period_preCom_next) && (period_preCom_next <= periodNext))){
				} // for (Periodo period_preCom = horizonte_potencia_disponivel_comandada.getIteradorInicial(); period_preCom <= horizonte_potencia_disponivel_comandada.getIteradorFinal(); horizonte_potencia_disponivel_comandada.incrementarIterador(period_preCom)) {
			} // if ((periodNext_precom >= periodNext) || (periodNext_precom > a_period)) {
		} // if (a_dados.getSize1Matriz(a_idUTE, AttMatrizTermeletrica_potencia_disponivel_comandada) > 0) {

		if (!periodIni_com_regr.isValido())
			return;

		// Períodos mensais com Potencia a comandar recursivamente

		const Periodo period_min_end = Periodo("m", a_period + 1) - 1;
		const Periodo period_month_end = Periodo("M", period_min_end.getMes(), period_min_end.getAno());
		SmartEnupla<Periodo, double> periods_month;

		Periodo period_month = Periodo("M", periodIni_com_regr.getMes(), periodIni_com_regr.getAno());
		periods_month.addElemento(period_month, a_period.sobreposicao(period_month));

		if (period_month < period_month_end) {
			for (Periodo period = period_month + 1; period <= period_month_end; period++)
				periods_month.addElemento(period, a_period.sobreposicao(period));
		}

		for (Periodo period_month = periods_month.getIteradorInicial(); period_month <= periods_month.getIteradorFinal(); periods_month.incrementarIterador(period_month)) {

			int varPTDISPCOM = getVarDecisao_PTDISPCOMseExistir(a_TSS, a_idEstagio, period_month, a_idUTE);
			if (varPTDISPCOM == -1) {

				const Periodo period_month_next = period_month + 1;

				double potencia_disponivel_minima_UTE = 0.0;
				double potencia_disponivel_maxima_UTE = 0.0;
				if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina) {

					double sobreposicao_acumulada = 0.0;
					const SmartEnupla<Periodo, double> horz_disponibilidade = a_dados.getVetor(a_idUTE, AttVetorTermeletrica_disponibilidade, Periodo(), double());
					for (Periodo period = horz_disponibilidade.getIteradorInicial(); period <= horz_disponibilidade.getIteradorFinal(); horz_disponibilidade.incrementarIterador(period)) {

						Periodo periodHrz_next = period + 1;

						if (((period_month <= period) && (period < period_month_next)) || ((period_month < periodHrz_next) && (periodHrz_next <= period_month_next))) {

							const double sobreposicao = period_month.sobreposicao(period);
							const double disponibilidade = horz_disponibilidade.at_rIt(period);
							if ((disponibilidade > 0.0) && (sobreposicao > 0.0)) {

								for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, period, IdPatamarCarga()); idPat++) {

									const double perc_dur_pat = a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, period, idPat, double());

									const double potencia_minima = a_dados.getElementoMatriz(a_idUTE, AttMatrizTermeletrica_potencia_minima, period, idPat, double());
									const double potencia_maxima = a_dados.getElementoMatriz(a_idUTE, AttMatrizTermeletrica_potencia_minima, period, idPat, double()) + a_dados.getElementoMatriz(a_idUTE, AttMatrizTermeletrica_potencia_util, period, idPat, double());

									double potencia_disponivel_minima = potencia_minima * disponibilidade;
									if (a_dados.getSize1Matriz(a_idUTE, AttMatrizTermeletrica_potencia_disponivel_minima) > 0) {
										if (a_dados.getElementoMatriz(a_idUTE, AttMatrizTermeletrica_potencia_disponivel_minima, period, idPat, double()) > potencia_disponivel_minima)
											potencia_disponivel_minima = potencia_disponivel_minima;
									}
									double potencia_disponivel_maxima = potencia_maxima * disponibilidade;
									if (a_dados.getSize1Matriz(a_idUTE, AttMatrizTermeletrica_potencia_disponivel_maxima) > 0) {
										if (a_dados.getElementoMatriz(a_idUTE, AttMatrizTermeletrica_potencia_disponivel_maxima, period, idPat, double()) < potencia_disponivel_maxima)
											potencia_disponivel_maxima = potencia_disponivel_maxima;
									}

									potencia_disponivel_minima_UTE += sobreposicao * perc_dur_pat * potencia_disponivel_minima;
									potencia_disponivel_maxima_UTE += sobreposicao * perc_dur_pat * potencia_disponivel_maxima;

								} // for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEnd; idPat++) {

								sobreposicao_acumulada += sobreposicao;

							} // if ((disponibilidade.at_rIt(period) > 0.0) && (sobreposicao > 0.0)) {

							else if ((sobreposicao == 0.0) && (sobreposicao_acumulada > 0.0))
								break;
						} // if (((period_month <= period) && (period < period_month_next)) || ((period_month < periodHrz_next) && (periodHrz_next <= period_month_next))) {
					} // for (Periodo period = a_period; period <= disponibilidade.getIteradorFinal(); disponibilidade.incrementarIterador(period)) {
				} // if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina) {

				else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {

					const IdUnidadeUTE idUnUTEIni = a_dados.getMenorId(a_idUTE, IdUnidadeUTE());
					const IdUnidadeUTE idUnUTEOut = a_dados.getIdOut(a_idUTE, IdUnidadeUTE());

					for (IdUnidadeUTE idUnidadeUTE = idUnUTEIni; idUnidadeUTE < idUnUTEOut; a_dados.incr(a_idUTE, idUnidadeUTE)) {

						if ((a_idUnUTE_equiv == IdUnidadeUTE_Nenhum) || (idUnidadeUTE == a_idUnUTE_equiv)) {

							double sobreposicao_acumulada = 0.0;
							const SmartEnupla<Periodo, double> horz_disponibilidade = a_dados.getVetor(a_idUTE, idUnidadeUTE, AttVetorUnidadeUTE_disponibilidade, Periodo(), double());
							for (Periodo period = a_period; period <= horz_disponibilidade.getIteradorFinal(); horz_disponibilidade.incrementarIterador(period)) {

								Periodo periodHrz_next = period + 1;

								if (((period_month <= period) && (period < period_month_next)) || ((period_month < periodHrz_next) && (periodHrz_next <= period_month_next))) {

									const double sobreposicao = period_month.sobreposicao(period);
									const double disponibilidade = horz_disponibilidade.at_rIt(period);
									if ((disponibilidade > 0.0) && (sobreposicao > 0.0)) {

										for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, period, IdPatamarCarga()); idPat++) {

											const double perc_dur_pat = a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, period, idPat, double());

											const double potencia_minima = a_dados.getElementoMatriz(a_idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_minima, period, idPat, double());
											const double potencia_maxima = a_dados.getElementoMatriz(a_idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_minima, period, idPat, double()) + a_dados.getElementoMatriz(a_idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_util, period, idPat, double());

											double potencia_disponivel_minima = potencia_minima * disponibilidade;
											if (a_dados.getSize1Matriz(a_idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_minima) > 0) {
												if (a_dados.getElementoMatriz(a_idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_minima, period, idPat, double()) > potencia_disponivel_minima)
													potencia_disponivel_minima = potencia_disponivel_minima;
											}
											double potencia_disponivel_maxima = potencia_maxima * disponibilidade;
											if (a_dados.getSize1Matriz(a_idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_maxima) > 0) {
												if (a_dados.getElementoMatriz(a_idUTE, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_maxima, period, idPat, double()) < potencia_disponivel_maxima)
													potencia_disponivel_maxima = potencia_disponivel_maxima;
											}

											potencia_disponivel_minima_UTE += sobreposicao * perc_dur_pat * potencia_disponivel_minima;
											potencia_disponivel_maxima_UTE += sobreposicao * perc_dur_pat * potencia_disponivel_maxima;

										} // for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEnd; idPat++) {

										sobreposicao_acumulada += sobreposicao;

									} // if ((disponibilidade.at_rIt(period) > 0.0) && (sobreposicao > 0.0)) {

									else if ((sobreposicao == 0.0) && (sobreposicao_acumulada > 0.0))
										break;

								} // if (((period_month <= period) && (period < period_month_next)) || ((period_month < periodHrz_next) && (periodHrz_next <= period_month_next))) {
							} // for (Periodo period = a_period; period <= disponibilidade.getIteradorFinal(); disponibilidade.incrementarIterador(period)) {
						}//if ((a_idUnUTE_equiv == IdUnidadeUTE_Nenhum) || (idUnidadeUTE == a_idUnUTE_equiv)) {
					} // for (IdUnidadeUTE idUnidadeUTE = idUnUTEIni; idUnidadeUTE < idUnUTEOut; a_dados.incr(a_idUTE, idUnidadeUTE)) {
				} // else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {

				varPTDISPCOM = criarVariaveisDecisao_VariaveisEstado_Restricoes_PTDISPCOM(a_TSS, a_dados, a_idEstagio, period_month, a_idUTE, potencia_disponivel_minima_UTE, potencia_disponivel_maxima_UTE);

				int varPTDISPCOM = getVarDecisao_PTDISPCOMseExistir(a_TSS, a_idEstagio, period_month, a_idUTE);

				int equPTDISPCOM = getEquLinear_PTDISPCOMseExistir(a_TSS, a_idEstagio, a_period, a_idUTE);
				if (equPTDISPCOM == -1) {
					equPTDISPCOM = addEquLinear_PTDISPCOM(a_TSS, a_idEstagio, a_period, a_idUTE);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP, equPTDISPCOM, 1.0);
				}
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISPCOM, equPTDISPCOM, -periods_month.at_rIt(period_month));

			} // if (varPTDISPCOM == -1) {
		} // for (Periodo period_month = periods_month.getIteradorInicial(); period_month <= periods_month.getIteradorFinal(); periods_month.incrementarIterador(period_month)) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarComandoTermeletricas(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_period) + "," + getFullString(a_idUTE) + "): \n" + std::string(erro.what())); }
}


int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_PTDISPCOM(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodMonth, const IdTermeletrica a_idUTE, const double a_potencia_disponivel_minima, const double a_potencia_disponivel_maxima, bool a_isVarEstadoExterna) {

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

		const int lag_mensal_potencia_comandada = a_dados.getAtributo(a_idUTE, AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada, int());
		if (lag_mensal_potencia_comandada == 0)
			throw std::invalid_argument(getFullString(a_idUTE) + " com atributo " + getFullString(AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada) + " igual a zero.");

		const Periodo periodo_comando_minuto = Periodo("m", a_periodMonth - lag_mensal_potencia_comandada);
		const Periodo periodo_otimizacao = getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());
		const Periodo periodo_otimizacao_next = Periodo("m", periodo_otimizacao + 1);

		if (periodo_comando_minuto >= periodo_otimizacao_next)
			return -1;

		int varPTDISPCOM = getVarDecisao_PTDISPCOMseExistir(a_TSS, a_idEstagio, a_periodMonth, a_idUTE);

		if (varPTDISPCOM > -1)
			return varPTDISPCOM;

		varPTDISPCOM = addVarDecisao_PTDISPCOM(a_TSS, a_idEstagio, a_periodMonth, a_idUTE, a_potencia_disponivel_minima, a_potencia_disponivel_maxima, 0.0);

		const int varPTDISPCOM_anterior = criarVariaveisDecisao_VariaveisEstado_Restricoes_PTDISPCOM(a_TSS, a_dados, IdEstagio(a_idEstagio - 1), a_periodMonth, a_idUTE, a_potencia_disponivel_minima, a_potencia_disponivel_maxima, a_isVarEstadoExterna);

		if (varPTDISPCOM_anterior != -1)
			vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeSolverVarDecisao_PTDISPCOM(a_TSS, a_idEstagio, a_periodMonth, a_idUTE) + "," + getString(a_potencia_disponivel_minima) + "," + getString(a_potencia_disponivel_maxima)), varPTDISPCOM, varPTDISPCOM_anterior, a_isVarEstadoExterna);

		return varPTDISPCOM;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisDecisao_VariaveisEstado_Restricoes_PTDISPCOM(a_dados," + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getString(a_periodMonth) + "," + getFullString(a_idUTE) + "," + getFullString(a_potencia_disponivel_minima) + "," + getFullString(a_potencia_disponivel_maxima) + "): \n" + std::string(erro.what())); }

} // int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_QDLAG(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodMonth, const IdHidreletrica a_idHidreletrica) {



int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_ZP0_VH_LINF(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodo_penalizacao, bool a_isVarEstadoExterna) {

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

			const Periodo periodo_minuto_otimizacao_final = Periodo("m", periodo_otimizacao + 1) - 1;
			const Periodo periodo_minuto_otimizacao_inicial = Periodo("m", periodo_otimizacao);

			const IdMes idMes_inicio = periodo_minuto_otimizacao_inicial.getMes();
			const IdAno idAno_inicio = periodo_minuto_otimizacao_inicial.getAno();

			Periodo periodo_penalizacao = Periodo("m", Periodo(mes_penalizacao_volume_util_minimo, idAno_inicio) + 1) -1;
			if (idMes_inicio > mes_penalizacao_volume_util_minimo)
				periodo_penalizacao = Periodo("m", Periodo(mes_penalizacao_volume_util_minimo, IdAno(int(idAno_inicio) + 1)) + 1) - 1;

			if ((periodo_minuto_otimizacao_inicial > periodo_penalizacao) || (periodo_minuto_otimizacao_final < periodo_penalizacao))
				return -1;

			const SmartEnupla<Periodo, double> horizonte_estudo = getElementosMatriz(AttMatrizModeloOtimizacao_horizonte_estudo, a_idEstagio, Periodo(), double());

			for (Periodo periodo = periodo_penalizacao; periodo <= periodo_minuto_otimizacao_final; periodo++) {

				// Adiciona penalizacao na função objetivo
				if (periodo == periodo_penalizacao) {
					
					const int varZPO_VH_FINF = criarVariaveisDecisao_VariaveisEstado_Restricoes_ZP0_VH_LINF(a_TSS, a_dados, a_idEstagio, periodo_penalizacao);
					
					if (varZPO_VH_FINF > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varZPO_VH_FINF, getEquLinear_ZP0(a_TSS, a_idEstagio), -1.0);

					periodo_penalizacao = Periodo("m", Periodo("a", periodo_penalizacao) + 1) - 1;

					if (periodo_minuto_otimizacao_final < periodo_penalizacao)
						return -1;

				} // if (periodo == periodo_penalizacao) {

			} // for (Periodo periodo = periodo_penalizacao; periodo <= periodo_minuto_otimizacao_final; periodo++) {

			return -1;

		} // if (!a_periodo_penalizacao.isValido()) {

		
		// Recursivamente cria variaveis e restrições para contabilizar ZPO_VH_FINF

		const Periodo periodo_final_horizonte_estudo = getIterador2Final(AttMatrizModeloOtimizacao_horizonte_estudo, a_idEstagio, Periodo());
		const SmartEnupla<Periodo, double> horizonte_estudo = getElementosMatriz(AttMatrizModeloOtimizacao_horizonte_estudo, a_idEstagio, Periodo(), double());
		const Periodo periodo_inicial_horizonte_estudo = horizonte_estudo.getIteradorInicial();

		if (periodo_inicial_horizonte_estudo > a_periodo_penalizacao)
			throw std::invalid_argument("Inicio do horizonte de estudo " + getFullString(periodo_inicial_horizonte_estudo) + " nao pode ser maior que periodo penalizacao.");

		const Periodo periodo_penalizacao_anterior = Periodo("m", Periodo("a", a_periodo_penalizacao) - 1);
		if ((Periodo(periodo_final_horizonte_estudo + 1) <= periodo_penalizacao_anterior))
			return -1;

		int varZP0_VH_LINF = getVarDecisao_ZP0_VH_LINFseExistir(a_TSS, a_idEstagio, a_periodo_penalizacao);
		if (varZP0_VH_LINF > -1)
			return varZP0_VH_LINF;

		const int varZP0_VH_LINF_ant = criarVariaveisDecisao_VariaveisEstado_Restricoes_ZP0_VH_LINF(a_TSS, a_dados, IdEstagio(a_idEstagio - 1), a_periodo_penalizacao, a_isVarEstadoExterna);

		// Atualmente VF é criada somente no periodo final do horizonte.
		const Periodo periodo_inicial_varredura_horizonte_estudo = periodo_final_horizonte_estudo;

		for (Periodo periodo = periodo_inicial_varredura_horizonte_estudo; periodo <= periodo_final_horizonte_estudo; horizonte_estudo.incrementarIterador(periodo)) {

			const Periodo periodo_minuto_final = Periodo("m", Periodo(periodo + 1)) - 1;

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
				percentual_inclusao_periodo = double((a_periodo_penalizacao - Periodo("m", periodo)) + 1.0) / double(periodo.getMinutos());

			else if (periodo > a_periodo_penalizacao)
				return varZP0_VH_LINF;

			if (percentual_inclusao_periodo > 0.0) {

				for (IdHidreletrica idUHE = a_dados.getMenorId(IdHidreletrica()); idUHE < a_dados.getIdOut(IdHidreletrica()); a_dados.vetorHidreletrica.incr(idUHE)) {

					int varVH_FINF = getVarDecisao_VH_FINFseExistir(a_TSS, a_idEstagio, periodo, idUHE);

					// Cria variável quando nao houve necessidade de formular o modelo do estágio
					if (varVH_FINF == -1) {
						if (a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_volume_util_minimo, periodo, double()) > 0.0)
							varVH_FINF = addVarDecisao_VH_FINF(a_TSS, a_idEstagio, periodo, idUHE, 0.0, a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_volume_util_minimo, periodo, double()), 0.0);
					}

					if (varVH_FINF > -1) {

						if (getVarDecisao_ZP0_VH_LINFseExistir(a_TSS, a_idEstagio, periodo, a_periodo_penalizacao) == -1) {

							if (getEquLinear_ZP0_VH_LINFseExistir(a_TSS, a_idEstagio, a_periodo_penalizacao) == -1) {
								addEquLinear_ZP0_VH_LINF(a_TSS, a_idEstagio, a_periodo_penalizacao);

								varZP0_VH_LINF = addVarDecisao_ZP0_VH_LINF(a_TSS, a_idEstagio, a_periodo_penalizacao, 0.0, vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);

								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varZP0_VH_LINF, getEquLinear_ZP0_VH_LINF(a_TSS, a_idEstagio, a_periodo_penalizacao), 1.0);

								if (varZP0_VH_LINF_ant != -1) {
									const int varZP0_VH_LINF_ADD = addVarDecisao_ZP0_VH_LINF_ADD(a_TSS, a_idEstagio, a_periodo_penalizacao, 0.0, 0.0, 0.0);
									vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeSolverVarDecisao_ZP0_VH_LINF(a_TSS, a_idEstagio, a_periodo_penalizacao)), varZP0_VH_LINF_ADD, varZP0_VH_LINF_ant, a_isVarEstadoExterna);

									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varZP0_VH_LINF_ADD, getEquLinear_ZP0_VH_LINF(a_TSS, a_idEstagio, a_periodo_penalizacao), -1.0);
								}

							} // if (getEquLinear_ZP0_VH_LINFseExistir(a_TSS, a_idEstagio, a_periodo_penalizacao) == -1) {

							const int varZP0_VH_LINF_per = addVarDecisao_ZP0_VH_LINF(a_TSS, a_idEstagio, periodo, a_periodo_penalizacao, 0.0, vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);
							const int equZP0_VH_LINF_per = addEquLinear_ZP0_VH_LINF(a_TSS, a_idEstagio, periodo, a_periodo_penalizacao);

							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varZP0_VH_LINF_per, equZP0_VH_LINF_per, 1.0);

							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varZP0_VH_LINF_per, getEquLinear_ZP0_VH_LINF(a_TSS, a_idEstagio, a_periodo_penalizacao), -1.0);

						} // if (getVarDecisao_ZP0_VH_LINFseExistir(a_TSS, a_idEstagio, periodo, a_periodo_penalizacao) == -1) {

						const double valor_penalidade = a_dados.getAtributo(idUHE, AttComumHidreletrica_penalidade_volume_util_minimo, double()) * a_dados.getElementoMatriz(AttMatrizDados_desagio_acumulado_horizonte_estudo, a_idEstagio, periodo, double());
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VH_FINF(a_TSS, a_idEstagio, periodo, idUHE), getEquLinear_ZP0_VH_LINF(a_TSS, a_idEstagio, periodo, a_periodo_penalizacao), -valor_penalidade);

					} // if (varVH_FINF > -1) {

				} // for (IdHidreletrica idUHE = a_dados.getMenorId(IdHidreletrica()); idUHE < a_dados.getMaiorId(IdHidreletrica()); a_dados.vetorHidreletrica.incr(idUHE)) {

			} // if (percentual_inclusao_periodo > 0.0) {

		} // for (Periodo periodo = periodo_inicial_horizonte_estudo; periodo <= periodo_final_horizonte_estudo; horizonte_estudo.incrementarIterador(periodo)) {

		return varZP0_VH_LINF;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisDecisao_VariaveisEstado_Restricoes_ZP0_VH_LINF(a_dados," + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getString(a_periodo_penalizacao) + "): \n" + std::string(erro.what())); }

} // int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_ZP0_VH_LINF(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo) {

int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_QDEF(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdHidreletrica a_idHidreletrica, Periodo &a_periodo_lag, const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>& a_horizon, bool a_isVarEstadoExterna) {

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

		int varQDEF = getVarDecisao_QDEFseExistir(a_TSS, a_idEstagio, a_periodo_lag, a_idHidreletrica);
		if (varQDEF > -1)
			return varQDEF;

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		varQDEF = addVarDecisao_QDEF(a_TSS, a_idEstagio, a_periodo_lag, a_idHidreletrica,  0.0, infinito, 0.0);

		// ADD any compound of QD in the current stage.
		int equQDEF = -1;
		const double sobreposicao_periodo_otimizacao = periodo_otimizacao.sobreposicao(a_periodo_lag);
		if (sobreposicao_periodo_otimizacao > 0.0) {

			equQDEF = getEquLinear_QDEFseExistir(a_TSS, a_idEstagio, a_periodo_lag, a_idHidreletrica);
			if (equQDEF == -1) {
				equQDEF = addEquLinear_QDEF(a_TSS, a_idEstagio, a_periodo_lag, a_idHidreletrica);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDEF, equQDEF, 1.0);
			}

			bool sobreposicao_encontrada = false;
			for (Periodo periodo = periodIni_stage; periodo <= periodEnd_stage; a_horizon.incrementarIterador(periodo)) {

				const double sobreposicao = a_periodo_lag.sobreposicao(periodo);

				if (sobreposicao > 0.0) {
					for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, periodo, IdPatamarCarga()); idPat++) {
						int varQDEF_pat = getVarDecisao_QDEFseExistir(a_TSS, a_idEstagio, periodo, idPat, a_idHidreletrica);
						if (varQDEF_pat < 0)
							varQDEF_pat = addVarDecisao_QDEF(a_TSS, a_idEstagio, periodo, idPat, a_idHidreletrica, 0.0, infinito, 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDEF_pat, equQDEF, -sobreposicao * a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, periodo, idPat, double()));
					}
					if (!sobreposicao_encontrada)
						sobreposicao_encontrada = true;
				}
				else if ((sobreposicao == 0.0) && (sobreposicao_encontrada))
					break;

			} // for (Periodo periodo = periodIni_stage; periodo <= periodEnd_stage; horiz_stage.incrementarIterador(periodo)) {
			
			if (a_periodo_lag >= periodIni_stage)
				return varQDEF;

		} // if (sobreposicao_periodo_otimizacao > 0.0) {

		// ADD any deterministic (past) compound of QDEF.
		// Only in stage where a_period belongs
		if (((a_periodo_lag < periodIni_stageIni) && (stageIni == a_idEstagio)) || (a_periodo_lag + 1 <= periodIni_stageIni)) {

			const SmartEnupla<Periodo, double> vazao_defluencia = a_dados.getVetor(a_idHidreletrica, IdDefluencia_passada, AttVetorDefluencia_vazao_defluencia, Periodo(), double());

			if (vazao_defluencia.getIteradorInicial() > a_periodo_lag)
				throw std::invalid_argument("Necessario mais periodos de defluencia passada anteriores a " + getString(a_periodo_lag) + " em " + getFullString(AttVetorDefluencia_vazao_defluencia) + " de " + getFullString(a_idHidreletrica));

			bool sobreposicao_encontrada = false;
			for (Periodo periodo = vazao_defluencia.getIteradorInicial(); periodo <= vazao_defluencia.getIteradorFinal(); vazao_defluencia.incrementarIterador(periodo)) {

				const double sobreposicao = a_periodo_lag.sobreposicao(periodo);

				if (sobreposicao > 0.0) {

					int varQDEF_prev = getVarDecisao_QDEFseExistir(a_TSS, a_idEstagio, periodo, a_idHidreletrica);

					if (varQDEF_prev == -1)
						varQDEF_prev = addVarDecisao_QDEF(a_TSS, a_idEstagio, periodo, a_idHidreletrica, vazao_defluencia.at_rIt(periodo), vazao_defluencia.at_rIt(periodo), 0.0);
					else {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(varQDEF_prev, vazao_defluencia.at_rIt(periodo));
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(varQDEF_prev, vazao_defluencia.at_rIt(periodo));
						if (periodo == a_periodo_lag)
							break;
					}

					int equQDEF = getEquLinear_QDEFseExistir(a_TSS, a_idEstagio, a_periodo_lag, a_idHidreletrica);

					if (equQDEF == -1) {
						equQDEF = addEquLinear_QDEF(a_TSS, a_idEstagio, a_periodo_lag, a_idHidreletrica);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDEF, equQDEF, 1.0);
					}

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDEF_prev, equQDEF, -sobreposicao);

					if (!sobreposicao_encontrada)
						sobreposicao_encontrada = true;

				} // if (sobreposicao > 0.0) {
				else if ((sobreposicao == 0.0) && (sobreposicao_encontrada))
					break;

			} // for (Periodo periodo = vazao_defluencia.getIteradorInicial(); periodo <= vazao_defluencia.getIteradorFinal(); vazao_defluencia.incrementarIterador(periodo)) {

			// All QDEF of QDEF are deterministic
			if (a_periodo_lag + 1 <= periodIni_stageIni)
				return varQDEF;

		} // if ((a_periodo_lag < periodIni_stageIni) && (stageIni == IdEstagio_1) && (periodIni_stage <= a_periodo) && (a_periodo <= periodEnd_stage)) {

		// Recursively ADD any compound of QDEF in past stages.
		const IdEstagio idEstagio_anterior = IdEstagio(a_idEstagio - 1);

		int varQDEF_anterior = -1;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica) {
			for (TipoSubproblemaSolver tss = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tss < TipoSubproblemaSolver_Excedente; tss++) {
				if (tss != TipoSubproblemaSolver_viabilidade_hidraulica)
					varQDEF_anterior = criarVariaveisDecisao_VariaveisEstado_Restricoes_QDEF(tss, a_dados, idEstagio_anterior, a_idHidreletrica, a_periodo_lag, a_horizon, a_isVarEstadoExterna);
				if (varQDEF_anterior > -1)
					break;
			}
		}
		else
			varQDEF_anterior = criarVariaveisDecisao_VariaveisEstado_Restricoes_QDEF(a_TSS, a_dados, idEstagio_anterior, a_idHidreletrica, a_periodo_lag, a_horizon, a_isVarEstadoExterna);

		if (varQDEF_anterior > -1){

			if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
				varQDEF_anterior = -1;

			// Variáveis de estado a compor lag
			if (sobreposicao_periodo_otimizacao > 0.0) {
				const int varQDEF_ADD = addVarDecisao_QDEF_ADD(a_TSS, a_idEstagio, a_periodo_lag, a_idHidreletrica, 0.0, infinito, 0.0);
				vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, getNomeSolverVarDecisao_QDEF(a_TSS, a_idEstagio, a_periodo_lag, a_idHidreletrica), varQDEF_ADD, varQDEF_anterior, a_isVarEstadoExterna);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDEF_ADD, equQDEF, -1.0);
			}

			// Variáveis de estado a repassar lag
			else
				vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, getNomeSolverVarDecisao_QDEF(a_TSS, a_idEstagio, a_periodo_lag, a_idHidreletrica), varQDEF, varQDEF_anterior, a_isVarEstadoExterna);

		} // if (varQDLAG_anterior > -1){

		return varQDEF;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisDecisao_VariaveisEstado_Restricoes_QDLAG(a_dados," + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idHidreletrica) + "," + getString(a_periodo_lag) + "): \n" + std::string(erro.what())); }

} // int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_QDLAG(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo, const IdHidreletrica a_idHidreletrica) {

int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_HQ(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodo_lag, const IdControleCotaVazao a_idConHQ, const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>& a_horizon, bool a_isVarEstadoExterna){
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

		const Periodo periodIni_stage = getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_estudo, a_idEstagio, Periodo());
		const Periodo periodIni_stageIni = getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_estudo, stageIni, Periodo());

		int varHQ = getVarDecisao_HQseExistir(a_TSS, a_idEstagio, a_periodo_lag, a_idConHQ);
		if (varHQ > -1)
			return varHQ;

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		varHQ = addVarDecisao_HQ(a_TSS, a_idEstagio, a_periodo_lag, a_idConHQ, 0.0, infinito, 0.0);

		// ADD any compound of QD in the current stage.
		int equHQ = -1;
		const double sobreposicao_periodo_otimizacao = periodo_otimizacao.sobreposicao(a_periodo_lag);
		if (sobreposicao_periodo_otimizacao > 0.0) {

			equHQ = addEquLinear_HQ(a_TSS, a_idEstagio, a_periodo_lag, a_idConHQ);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varHQ, equHQ, 1.0);

			bool sobreposicao_encontrada = false;
			for (Periodo periodo = periodIni_stage; periodo <= periodEnd_stage; a_horizon.incrementarIterador(periodo)) {

				const double sobreposicao = a_periodo_lag.sobreposicao(periodo);

				if (sobreposicao > 0.0) {
					int varHQ_per = getVarDecisao_HQseExistir(a_TSS, a_idEstagio, periodo, a_idConHQ);
					if (varHQ_per == -1)
						varHQ_per = addVarDecisao_HQ(a_TSS, a_idEstagio, periodo, a_idConHQ, 0.0, infinito, 0.0);

					int equHQ_per = getEquLinear_HQseExistir(a_TSS, a_idEstagio, periodo, a_idConHQ);
					if (equHQ_per == -1)
						equHQ_per = addEquLinear_HQ(a_TSS, a_idEstagio, periodo, a_idConHQ);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(equHQ_per, a_dados.getElementoVetor(a_idConHQ, AttVetorControleCotaVazao_coef_linear_cota_vazao_0, periodo, double()));
					for (int h = a_dados.getIteradorInicial(a_idConHQ, AttVetorControleCotaVazao_hidreletrica_montante, int()); h <= a_dados.getIteradorFinal(a_idConHQ, AttVetorControleCotaVazao_hidreletrica_montante, int()); h++) {
						const IdHidreletrica idUHE = a_dados.getElementoVetor(a_idConHQ, AttVetorControleCotaVazao_hidreletrica_montante, h, IdHidreletrica());
						const int tempo_viagem_agua = a_dados.getAtributo(idUHE, AttComumHidreletrica_tempo_viagem_agua, int());
						bool tva_considerado = false;
						if (tempo_viagem_agua > 0) {
							const Periodo periodo_ref = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_horizonte_defluencia_viajante, periodo, Periodo());
							if (periodo_ref != Periodo(IdAno_1900)) {
								Periodo periodo_lag = Periodo(getString(periodo_ref.getDuration()), Periodo("h", periodo_ref) - tempo_viagem_agua);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(criarVariaveisDecisao_VariaveisEstado_Restricoes_QDEF(a_TSS, a_dados, a_idEstagio, idUHE, periodo_lag, a_horizon), equHQ_per, -a_dados.getElementoVetor(a_idConHQ, AttVetorControleCotaVazao_fator_participacao, h, double()) * a_dados.getElementoVetor(a_idConHQ, AttVetorControleCotaVazao_coef_linear_cota_vazao_1, periodo, double()));
								tva_considerado = true;
							}
						}
						if (!tva_considerado) {
							if (a_horizon.at_rIt(periodo).getIteradorFinal() == IdPatamarCarga_1)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, periodo, IdPatamarCarga_1, idUHE), equHQ_per, -a_dados.getElementoVetor(a_idConHQ, AttVetorControleCotaVazao_fator_participacao, h, double()) * a_dados.getElementoVetor(a_idConHQ, AttVetorControleCotaVazao_coef_linear_cota_vazao_1, periodo, double()));
							else
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, periodo, idUHE), equHQ_per, -a_dados.getElementoVetor(a_idConHQ, AttVetorControleCotaVazao_fator_participacao, h, double()) * a_dados.getElementoVetor(a_idConHQ, AttVetorControleCotaVazao_coef_linear_cota_vazao_1, periodo, double()));
						}
					}

					if (sobreposicao < 1.0) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varHQ_per, equHQ, -sobreposicao);
						if (!sobreposicao_encontrada)
							sobreposicao_encontrada = true;
					}
					else if (sobreposicao == 1.0)
						return varHQ;

				}
				else if ((sobreposicao == 0.0) && (sobreposicao_encontrada))
					break;

			} // for (Periodo periodo = periodIni_stage; periodo <= periodEnd_stage; horiz_stage.incrementarIterador(periodo)) {

			if (a_periodo_lag >= periodIni_stage)
				return varHQ;

		} // if (sobreposicao_periodo_otimizacao > 0.0) {

		// ADD any deterministic (past) compound of HQ.
		// Only in stage where a_period belongs
		if (((a_periodo_lag < periodIni_stageIni) && (stageIni == a_idEstagio)) || (a_periodo_lag + 1 <= periodIni_stageIni)) {

			const SmartEnupla<Periodo, double> cota_anterior = a_dados.getVetor(a_idConHQ, AttVetorControleCotaVazao_cota_anterior, Periodo(), double());

			if (cota_anterior.getIteradorInicial() > a_periodo_lag)
				throw std::invalid_argument("Necessario mais periodos de defluencia passada anteriores a " + getString(a_periodo_lag) + " em " + getFullString(AttVetorControleCotaVazao_cota_anterior) + " de " + getFullString(a_idConHQ));

			bool sobreposicao_encontrada = false;
			for (Periodo periodo = cota_anterior.getIteradorInicial(); periodo <= cota_anterior.getIteradorFinal(); cota_anterior.incrementarIterador(periodo)) {

				const double sobreposicao = a_periodo_lag.sobreposicao(periodo);

				if (sobreposicao > 0.0) {

					int varHQ_past = getVarDecisao_HQseExistir(a_TSS, a_idEstagio, periodo, a_idConHQ);

					if (varHQ_past == -1)
						varHQ_past = addVarDecisao_HQ(a_TSS, a_idEstagio, periodo, a_idConHQ, cota_anterior.at(periodo), cota_anterior.at(periodo), 0.0);
					else {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(varHQ_past, cota_anterior.at(periodo));
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(varHQ_past, cota_anterior.at(periodo));
					}

					if (varHQ_past != varHQ) {

						int equHQ = getEquLinear_HQseExistir(a_TSS, a_idEstagio, a_periodo_lag, a_idConHQ);

						if (equHQ == -1) {
							equHQ = addEquLinear_HQ(a_TSS, a_idEstagio, a_periodo_lag, a_idConHQ);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varHQ, equHQ, 1.0);
						}

						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varHQ_past, equHQ, -sobreposicao);

					}

					if (!sobreposicao_encontrada)
						sobreposicao_encontrada = true;

				} // if (sobreposicao > 0.0) {
				else if ((sobreposicao == 0.0) && (sobreposicao_encontrada))
					break;

			} // for (Periodo periodo = cota_anterior.getIteradorInicial(); periodo <= cota_anterior.getIteradorFinal(); cota_anterior.incrementarIterador(periodo)) {

			// All HQ of HQ are deterministic
			if (a_periodo_lag + 1 <= periodIni_stageIni)
				return varHQ;

		} // if (((a_periodo_lag < periodIni_stageIni) && (stageIni == a_idEstagio)) || (a_periodo_lag + 1 <= periodIni_stageIni)) {

		// Recursively ADD any compound of HQ in past stages.
		const IdEstagio idEstagio_anterior = IdEstagio(a_idEstagio - 1);

		int varHQ_anterior = -1;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica) {
			for (TipoSubproblemaSolver tss = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tss < TipoSubproblemaSolver_Excedente; tss++) {
				if (tss != TipoSubproblemaSolver_viabilidade_hidraulica)
					varHQ_anterior = criarVariaveisDecisao_VariaveisEstado_Restricoes_HQ(tss, a_dados, idEstagio_anterior, a_periodo_lag, a_idConHQ, a_horizon, a_isVarEstadoExterna);
				if (varHQ_anterior > -1)
					break;
			}
		}
		else
			varHQ_anterior = criarVariaveisDecisao_VariaveisEstado_Restricoes_HQ(a_TSS, a_dados, idEstagio_anterior, a_periodo_lag, a_idConHQ, a_horizon, a_isVarEstadoExterna);

		if (varHQ_anterior > -1) {

			if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
				varHQ_anterior = -1;

			// Variáveis de estado a compor lag
			if (sobreposicao_periodo_otimizacao > 0.0) {
				const int varHQ_ADD = addVarDecisao_HQ_ADD(a_TSS, a_idEstagio, a_periodo_lag, a_idConHQ, 0.0, infinito, 0.0);
				vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeSolverVarDecisao_HQ(a_TSS, a_idEstagio, a_periodo_lag, a_idConHQ) + "," + a_dados.getAtributo(a_idConHQ, AttComumControleCotaVazao_nome, std::string())), varHQ_ADD, varHQ_anterior, a_isVarEstadoExterna);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varHQ_ADD, equHQ, -1.0);
			}

			// Variáveis de estado a repassar lag
			else
				vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeSolverVarDecisao_HQ(a_TSS, a_idEstagio, a_periodo_lag, a_idConHQ) + "," + a_dados.getAtributo(a_idConHQ, AttComumControleCotaVazao_nome, std::string())), varHQ, varHQ_anterior, a_isVarEstadoExterna);

		} // if (varQDLAG_anterior > -1){

		return varHQ;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisDecisao_VariaveisEstado_Restricoes_HQ(a_dados," + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getString(a_periodo_lag) + "," + getFullString(a_idConHQ) + "): \n" + std::string(erro.what())); }

}

int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_YP(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria, Periodo &a_periodo_lag, const double a_grau_liberdade, std::vector<IdHidreletrica> a_listaIdHidreletrica, bool a_isVarEstadoExterna) {

	try {

		const IdEstagio menor_estagio = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());

		if (a_idEstagio < menor_estagio)
			return -1;

		if ((!vetorEstagio.at(a_idEstagio).isSolverInstanciado(a_TSS)) || (a_TSS == TipoSubproblemaSolver_mestre))
			return -1;

		const Periodo periodo_otimizacao = getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

		const Periodo periodo_final_proc_estoc_hidrologico = getIterador2Final(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, a_idEstagio, Periodo());

		// Verifica se periodo não é mais necessario para compor ou repassar lag
		if (periodo_final_proc_estoc_hidrologico + 1 <= a_periodo_lag)
			return -1;

		const SmartEnupla<Periodo, int> proc_estoc_hidrologico = getElementosMatriz(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, a_idEstagio, Periodo(), int());
		const Periodo periodo_inicial_proc_estoc_hidrologico = proc_estoc_hidrologico.getIteradorInicial();

		const IdProcessoEstocastico idProcessoEstocastico_modelo = IdProcessoEstocastico_1;

		int varYP = -1;

		std::vector<IdVariavelAleatoria> idVarEquiv;
		std::vector<std::vector<IdVariavelAleatoriaInterna>> idVarIntEquiv;

		// Assume-se que processo estocastico já instanciado no modelo é identico ao processo estocastico repassado via argumento. 
		if (a_listaIdHidreletrica.size() == 0) {
			varYP = getVarDecisao_YPseExistir(a_TSS, a_idEstagio, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria);
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
				vetorProcessoEstocastico.at(idProcessoEstocastico_modelo).getIdVariavelAleatoriaIdVariavelAleatoriaInternaFromIdFisico(idVar, idVarInt, a_listaIdHidreletrica.at(i));
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
				if (vectorCompara(a_listaIdHidreletrica, vetorProcessoEstocastico.at(idProcessoEstocastico_modelo).getIdFisicoFromIdVariavelAleatoria(idVarEquiv.at(0), IdHidreletrica())))
					varYP = getVarDecisao_YPseExistir(a_TSS, a_idEstagio, a_periodo_lag, idProcessoEstocastico_modelo, idVarEquiv.at(0));
				if (varYP > -1)
					return varYP;
			}
			else if (idVarEquiv.size() == 0)
				throw std::invalid_argument("Nao foi encontrada equivalencia entre processo estocastico instanciado e processo estocastico repassado via argumento.");
		}

		varYP = getVarDecisao_YPseExistir(a_TSS, a_idEstagio, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria);
		if (varYP > -1)
			return varYP;

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();
		varYP = addVarDecisao_YP(a_TSS, a_idEstagio, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria, -infinito, infinito, 0.0);

		// Composicao de lag com periodos do horizonte de estudo
		int equYP = -1;
		double rhs_equYP = 0.0;
		const double sobreposicao_periodo_otimizacao = a_periodo_lag.sobreposicao(periodo_otimizacao);
		if (sobreposicao_periodo_otimizacao > 0.0) {

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

						int varYP_compose_lag = getVarDecisao_YPseExistir(a_TSS, a_idEstagio, periodo, idProcessoEstocastico_modelo, idVarEquiv.at(i));
						if (varYP_compose_lag > -1) {
							if (equYP == -1) {
								equYP = addEquLinear_YP(a_TSS, a_idEstagio, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYP, equYP, 1.0);
							}
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYP_compose_lag, equYP, -sobreposicao * coeficiente_participacao);
						}
						rhs_equYP += (a_grau_liberdade - grau_liberdade) * sobreposicao;
					}

					if (!sobreposicao_encontrada)
						sobreposicao_encontrada = true;
				}
				else if ((sobreposicao == 0.0) && (sobreposicao_encontrada))
					break;

			} // for (Periodo periodo = periodo_inicial_proc_estoc_hidrologico; periodo <= periodo_final_proc_estoc_hidrologico; proc_estoc_hidrologico.incrementarIterador(periodo)) {

			if (equYP > -1)
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(equYP, rhs_equYP);

			if (sobreposicao_periodo_otimizacao == 1.0)
				return varYP;

		} // if (sobreposicao_periodo_otimizacao > 0.0) {

		// No estagio 1, variáveis de estado são criadas para as variáveis QINC pré-estudo.
		if (a_idEstagio == IdEstagio_1) {

			std::vector<IdHidreletrica> listaIdHidreletrica = a_listaIdHidreletrica;
			if (listaIdHidreletrica.size() == 0)
				listaIdHidreletrica = vetorProcessoEstocastico.at(a_idProcessoEstocastico).getIdFisicoFromIdVariavelAleatoria(a_idVariavelAleatoria, IdHidreletrica());

			IdVariavelAleatoria idVar; IdVariavelAleatoriaInterna idVarInt;
			vetorProcessoEstocastico.at(a_idProcessoEstocastico).getIdVariavelAleatoriaIdVariavelAleatoriaInternaFromIdFisico(idVar, idVarInt, listaIdHidreletrica.at(0));

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
						vetorProcessoEstocastico.at(a_idProcessoEstocastico).getIdVariavelAleatoriaIdVariavelAleatoriaInternaFromIdFisico(idVar, idVarInt, idUHE);

						int varQINC = getVarDecisao_QINCseExistir(a_TSS, a_idEstagio, periodo, idUHE);

						if (varQINC == -1) {
							if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {
								const double tendencia = getElementoVetor(idProcessoEstocastico_modelo, idVar, idVarInt, AttVetorVariavelAleatoriaInterna_tendencia_temporal, periodo, double());
								varQINC = addVarDecisao_QINC(a_TSS, a_idEstagio, periodo, idUHE, tendencia, tendencia, 0.0);
							}
							else
								varQINC = addVarDecisao_QINC(a_TSS, a_idEstagio, periodo, idUHE, -infinito, infinito, 0.0);
						}

						equYP = getEquLinear_YPseExistir(a_TSS, a_idEstagio, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria);

						if (equYP == -1) {
							equYP = addEquLinear_YP(a_TSS, a_idEstagio, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYP, equYP, 1.0);
						}

						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQINC, equYP, -sobreposicao);

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
						varYP_anterior = criarVariaveisDecisao_VariaveisEstado_Restricoes_YP(tss, a_dados, idEstagio_anterior, a_idProcessoEstocastico, a_idVariavelAleatoria, a_periodo_lag, a_grau_liberdade, a_listaIdHidreletrica, a_isVarEstadoExterna);
					if (varYP_anterior > -1)
						break;
				}
			}
			else
				varYP_anterior = criarVariaveisDecisao_VariaveisEstado_Restricoes_YP(a_TSS, a_dados, idEstagio_anterior, a_idProcessoEstocastico, a_idVariavelAleatoria, a_periodo_lag, a_grau_liberdade, a_listaIdHidreletrica, a_isVarEstadoExterna);

			if ((varYP_anterior > -1) || ((idEstagio_anterior == menor_estagio) && (periodo_inicial_proc_estoc_hidrologico > a_periodo_lag))) {

				if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
					varYP_anterior = -1;

				std::vector<IdHidreletrica> listaIdHidreletrica = a_listaIdHidreletrica;
				if (listaIdHidreletrica.size() == 0)
					listaIdHidreletrica = vetorProcessoEstocastico.at(a_idProcessoEstocastico).getIdFisicoFromIdVariavelAleatoria(a_idVariavelAleatoria, IdHidreletrica());

				// Variáveis de estado a repassar lag
				if (sobreposicao_periodo_otimizacao == 0.0)
					vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeSolverVarDecisao_YP(a_TSS, a_idEstagio, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria) + "," + getString(a_grau_liberdade) + "," + getStringFromLista(listaIdHidreletrica, ",", false)), varYP, varYP_anterior, a_isVarEstadoExterna);

				// Variáveis de estado a compor lag
				else if (sobreposicao_periodo_otimizacao > 0.0) {
					const int varYP_ADD = addVarDecisao_YP_ADD(a_TSS, a_idEstagio, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria, -infinito, infinito, 0.0);

					if (equYP == -1) {
						equYP = addEquLinear_YP(a_TSS, a_idEstagio, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYP, equYP, 1.0);
					}

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYP_ADD, equYP, -1.0);
					vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeSolverVarDecisao_YP(a_TSS, a_idEstagio, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria) + "," + getString(a_grau_liberdade) + "," + getStringFromLista(listaIdHidreletrica, ",", false)), varYP_ADD, varYP_anterior, a_isVarEstadoExterna);
				}

			} // if ((varYP_anterior > -1) || ((idEstagio_anterior == menor_estagio) && (periodo_inicial_proc_estoc_hidrologico > a_idVariavelAleatoria))) {
		} // else if (a_idEstagio > IdEstagio_1) {

		return varYP;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisDecisao_VariaveisEstado_Restricoes_YP(a_dados," + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idProcessoEstocastico) + "," + getFullString(a_idVariavelAleatoria) + "," + getString(a_periodo_lag) + "," + getString(a_grau_liberdade) + "): \n" + std::string(erro.what())); }

}


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

		indx_IneLinear_CB_ZF_3_1.at(a_TSS).at(a_idEstagio_anterior).erase(indx_IneLinear_CB_ZF_3_1.at(a_TSS).at(a_idEstagio_anterior).begin() + pos);
		name_IneLinear_CB_ZF_3_1.at(a_TSS).at(a_idEstagio_anterior).erase(name_IneLinear_CB_ZF_3_1.at(a_TSS).at(a_idEstagio_anterior).begin() + pos);

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

		indx_IneLinear_CB_ZT_3_1.at(a_TSS).at(a_idEstagio).erase(indx_IneLinear_CB_ZT_3_1.at(a_TSS).at(a_idEstagio).begin() + pos);
		name_IneLinear_CB_ZT_3_1.at(a_TSS).at(a_idEstagio).erase(name_IneLinear_CB_ZT_3_1.at(a_TSS).at(a_idEstagio).begin() + pos);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::remCorteBendersFromZF(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCorteBenders) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::remCorteBendersFromZT(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders){


bool ModeloOtimizacao::isNecessarioInstanciarSolver(const IdEstagio a_idEstagio, const TipoSubproblemaSolver a_tipoSubproblemaSolver) {
	try {

		if (a_tipoSubproblemaSolver == TipoSubproblemaSolver_viabilidade_hidraulica) {

			if (!getAtributo(AttComumModeloOtimizacao_relaxar_afluencia_incremental_com_viabilidade_hidraulica, bool()))
				return false;

			SmartEnupla<Periodo, int> horizonte_processo_estocastico_hidrologico = getElementosMatriz(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, a_idEstagio, Periodo(), int());

			for (Periodo periodo = horizonte_processo_estocastico_hidrologico.getIteradorInicial(); periodo <= horizonte_processo_estocastico_hidrologico.getIteradorFinal(); horizonte_processo_estocastico_hidrologico.incrementarIterador(periodo)) {
				for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= getMaiorId(IdProcessoEstocastico_1, IdVariavelAleatoria()); idVar++) {
					if (getElementoVetor(IdProcessoEstocastico_1, idVar, AttVetorVariavelAleatoria_tipo_relaxacao, periodo, TipoRelaxacaoVariavelAleatoria()) != TipoRelaxacaoVariavelAleatoria_sem_relaxacao) {
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


void ModeloOtimizacao::criarRestricoesHidraulicas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodIni_stage, Periodo& a_periodPrev, Periodo& a_period, Periodo& a_periodNext, const IdPatamarCarga a_idPat) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		const std::vector<IdRestricaoOperativaUHE> idsRH = a_dados.vetorRestricaoOperativaUHE.getIdObjetos(AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica_Nenhum);

		const int numero_restricoes = int(idsRH.size());

		if (numero_restricoes == 0)
			return;

		const IdEstagio stageIni = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());

		const IdPatamarCarga idPatEndPer = a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, a_period, IdPatamarCarga());

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const double vlr_min = getdoubleFromChar("min");
		const double vlr_max = getdoubleFromChar("max");

		for (int pos = 0; pos < numero_restricoes; pos++) {

			const IdRestricaoOperativaUHE idRH = idsRH.at(pos);

			const double penalidade = a_dados.getAtributo(idRH, AttComumRestricaoOperativaUHE_penalidade, double());

			///////////////////////////////////////////////////////////////////
			//Restrições Hidráulicas (RH)
			///////////////////////////////////////////////////////////////////

			//******************************************
			//Formulacao: RH + RH_FINF >= lim_inf
			//******************************************

			if (a_dados.getSize1Matriz(idRH, AttMatrizRestricaoOperativaUHE_lim_inf) > 0) {
				const IdPatamarCarga idPatEnd = a_dados.getIterador2Final(idRH, AttMatrizRestricaoOperativaUHE_lim_inf, a_period, IdPatamarCarga());

				if (idPatEnd >= a_idPat) {

					const double lim_inf = a_dados.getElementoMatriz(idRH, AttMatrizRestricaoOperativaUHE_lim_inf, a_period, a_idPat, double());

					if ((vlr_min != lim_inf) && (vlr_max != lim_inf)) {

						const IdPatamarCarga idPatEnd = a_dados.getIterador2Final(idRH, AttMatrizRestricaoOperativaUHE_lim_inf, a_period, IdPatamarCarga());

						if (idPatEnd == IdPatamarCarga_1) {
							const int varRH = criarRestricoesHidraulicas(a_TSS, a_dados, a_idEstagio, a_period, a_periodNext, IdPatamarCarga_Nenhum, idRH);
							if (getIneLinear_RH_LINFseExistir(a_TSS, a_idEstagio, a_period, idRH) == -1) {
								const int ineRH_LINF = addIneLinear_RH_LINF(a_TSS, a_idEstagio, a_period, idRH);
								const int varRH_FINF = addVarDecisao_RH_LINF_FINF(a_TSS, a_idEstagio, a_period, idRH, 0.0, infinito, 0.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_FINF, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -penalidade);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineRH_LINF, lim_inf);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH, ineRH_LINF, 1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_FINF, ineRH_LINF, 1.0);
							} // if (getIneLinear_RH_LINFseExistir(a_TSS, a_idEstagio, a_period, idRH) == -1) {
						}
						else if (idPatEnd == idPatEndPer) {
							const int varRH_pat = criarRestricoesHidraulicas(a_TSS, a_dados, a_idEstagio, a_period, a_periodNext, a_idPat, idRH);
							if (getIneLinear_RH_LINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idRH) == -1) {
								const int ineRH_LINF = addIneLinear_RH_LINF(a_TSS, a_idEstagio, a_period, a_idPat, idRH);
								const int varRH_FINF = addVarDecisao_RH_LINF_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idRH, 0.0, infinito, 0.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_FINF, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -penalidade);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineRH_LINF, lim_inf);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_pat, ineRH_LINF, 1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_FINF, ineRH_LINF, 1.0);
							} // if (getIneLinear_RH_LINFseExistir(a_TSS, a_idEstagio, a_period, idRH) == -1) {
							criarRestricoesHidraulicas(a_TSS, a_dados, a_idEstagio, a_period, a_periodNext, IdPatamarCarga_Nenhum, idRH);
							const int equRH = getEquLinear_RH(a_TSS, a_idEstagio, a_period, idRH);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_pat, equRH, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, a_idPat, double()));
						}
						else
							throw std::invalid_argument("Ivalid maximum pat");

					} // if ((vlr_min != lim_inf) && (vlr_max != lim_inf)) {
				} // if (idPatEnd >= a_idPat) {
			} // if (a_dados.getSize1Matriz(idRH, AttMatrizRestricaoOperativaUHE_lim_inf) > 0) {

			//******************************************
			//Formulacao: RH - RH_FSUP <= lim_sup
			//******************************************

			if (a_dados.getSize1Matriz(idRH, AttMatrizRestricaoOperativaUHE_lim_sup) > 0) {
				const IdPatamarCarga idPatEnd = a_dados.getIterador2Final(idRH, AttMatrizRestricaoOperativaUHE_lim_sup, a_period, IdPatamarCarga());

				if (idPatEnd >= a_idPat) {

					const double lim_sup = a_dados.getElementoMatriz(idRH, AttMatrizRestricaoOperativaUHE_lim_sup, a_period, a_idPat, double());

					if ((vlr_min != lim_sup) && (vlr_max != lim_sup)) {

						if (idPatEnd == IdPatamarCarga_1) {
							const int varRH = criarRestricoesHidraulicas(a_TSS, a_dados, a_idEstagio, a_period, a_periodNext, IdPatamarCarga_Nenhum, idRH);
							if (getIneLinear_RH_LSUPseExistir(a_TSS, a_idEstagio, a_period, idRH) == -1) {
								const int ineRH_LSUP = addIneLinear_RH_LSUP(a_TSS, a_idEstagio, a_period, idRH);
								const int varRH_FSUP = addVarDecisao_RH_LSUP_FSUP(a_TSS, a_idEstagio, a_period, idRH, 0.0, infinito, 0.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_FSUP, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -penalidade);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineRH_LSUP, -lim_sup);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH, ineRH_LSUP, -1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_FSUP, ineRH_LSUP, 1.0);
							} // if (getIneLinear_RH_LSUPseExistir(a_TSS, a_idEstagio, a_period, idRH) == -1) {
						}
						else if (idPatEnd == idPatEndPer) {
							const int varRH_pat = criarRestricoesHidraulicas(a_TSS, a_dados, a_idEstagio, a_period, a_periodNext, a_idPat, idRH);
							if (getIneLinear_RH_LSUPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idRH) == -1) {
								const int ineRH_LSUP = addIneLinear_RH_LSUP(a_TSS, a_idEstagio, a_period, a_idPat, idRH);
								const int varRH_FSUP = addVarDecisao_RH_LSUP_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idRH, 0.0, infinito, 0.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_FSUP, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -penalidade);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineRH_LSUP, -lim_sup);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_pat, ineRH_LSUP, -1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_FSUP, ineRH_LSUP, 1.0);
							} // if (getIneLinear_RH_LSUPseExistir(a_TSS, a_idEstagio, a_period, idRH) == -1) {
							criarRestricoesHidraulicas(a_TSS, a_dados, a_idEstagio, a_period, a_periodNext, IdPatamarCarga_Nenhum, idRH);
							const int equRH = getEquLinear_RH(a_TSS, a_idEstagio, a_period, idRH);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_pat, equRH, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, a_idPat, double()));
						}
						else
							throw std::invalid_argument("Ivalid maximum pat");

					} // if (lim_inf > vlr_max) {
				} // if (idPatEnd >= a_idPat) {
			} // if (a_dados.getSize1Matriz(idRH, AttMatrizRestricaoOperativaUHE_lim_sup) > 0) {

			if (a_idPat == idPatEndPer) {

				//******************************************************
				//Formulacao: RH + RH_FINF >= RH_lag1 + var_abs_inf
				//******************************************************

				if (a_dados.getSize1Matriz(idRH, AttMatrizRestricaoOperativaUHE_var_abs_inf) > 0) {
					const double var_abs_inf = a_dados.getElementoMatriz(idRH, AttMatrizRestricaoOperativaUHE_var_abs_inf, a_period, IdPatamarCarga_1, double());

					if ((vlr_min != var_abs_inf) && (vlr_max != var_abs_inf)) {

						const int varRH = criarRestricoesHidraulicas(a_TSS, a_dados, a_idEstagio, a_period, a_periodNext, IdPatamarCarga_Nenhum, idRH);
						if (getIneLinear_RH_VAINFseExistir(a_TSS, a_idEstagio, a_period, idRH) == -1) {
							const int ineRH_VINF = addIneLinear_RH_VAINF(a_TSS, a_idEstagio, a_period, idRH);
							const int varRH_FINF = addVarDecisao_RH_VAINF_FINF(a_TSS, a_idEstagio, a_period, idRH, 0.0, infinito, 0.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_FINF, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -penalidade);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH, ineRH_VINF, 1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_FINF, ineRH_VINF, 1.0);

							if ((a_idEstagio == stageIni) && (a_period == a_periodIni_stage))
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineRH_VINF, a_dados.getAtributo(idRH, AttComumRestricaoOperativaUHE_vlr_ini, double()) + var_abs_inf);
							else {
								int varRH_prev = getVarDecisao_RHseExistir(a_TSS, a_idEstagio, a_periodPrev, idRH);
								if ((varRH_prev == -1) && (a_periodPrev >= a_periodIni_stage))
									varRH_prev = criarRestricoesHidraulicas(a_TSS, a_dados, a_idEstagio, a_periodPrev, a_period, IdPatamarCarga_Nenhum, idRH);
								else if (varRH_prev == -1) {
									varRH_prev = addVarDecisao_RH(a_TSS, a_idEstagio, a_periodPrev, idRH, 0.0, infinito, 0.0);
									vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, criarRestricoesHidraulicas_nomeVarEstado(a_TSS, a_dados, a_idEstagio, a_periodPrev, a_period, idRH), varRH_prev, criarRestricoesHidraulicas(a_TSS, a_dados, IdEstagio(a_idEstagio - 1), a_periodPrev, a_period, IdPatamarCarga_Nenhum, idRH));
								}
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineRH_VINF, var_abs_inf);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_prev, ineRH_VINF, -1.0);
							}
						} // if (getIneLinear_RH_VINFseExistir(a_TSS, a_idEstagio, a_period, idRH) == -1) {

					} // if ((vlr_min != var_abs_inf) && (vlr_max != var_abs_inf)) {
				} // if (a_dados.getSize1Matriz(idRH, AttMatrizRestricaoOperativaUHE_lim_inf) > 0) {

				//******************************************************
				//Formulacao: RH + RH_FSUP <= RH_lag1 + var_abs_sup
				//******************************************************

				if (a_dados.getSize1Matriz(idRH, AttMatrizRestricaoOperativaUHE_var_abs_sup) > 0) {
					const double var_abs_sup = a_dados.getElementoMatriz(idRH, AttMatrizRestricaoOperativaUHE_var_abs_sup, a_period, IdPatamarCarga_1, double());

					if ((vlr_min != var_abs_sup) && (vlr_max != var_abs_sup)) {

						const int varRH = criarRestricoesHidraulicas(a_TSS, a_dados, a_idEstagio, a_period, a_periodNext, IdPatamarCarga_Nenhum, idRH);
						if (getIneLinear_RH_VASUPseExistir(a_TSS, a_idEstagio, a_period, idRH) == -1) {
							const int ineRH_VSUP = addIneLinear_RH_VASUP(a_TSS, a_idEstagio, a_period, idRH);
							const int varRH_FSUP = addVarDecisao_RH_VASUP_FSUP(a_TSS, a_idEstagio, a_period, idRH, 0.0, infinito, 0.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_FSUP, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -penalidade);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH, ineRH_VSUP, -1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_FSUP, ineRH_VSUP, 1.0);

							if ((a_idEstagio == stageIni) && (a_period == a_periodIni_stage))
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineRH_VSUP, -a_dados.getAtributo(idRH, AttComumRestricaoOperativaUHE_vlr_ini, double()) - var_abs_sup);
							else {
								int varRH_prev = getVarDecisao_RHseExistir(a_TSS, a_idEstagio, a_periodPrev, idRH);
								if ((varRH_prev == -1) && (a_periodPrev >= a_periodIni_stage))
									varRH_prev = criarRestricoesHidraulicas(a_TSS, a_dados, a_idEstagio, a_periodPrev, a_period, IdPatamarCarga_Nenhum, idRH);
								else if (varRH_prev == -1) {
									varRH_prev = addVarDecisao_RH(a_TSS, a_idEstagio, a_periodPrev, idRH, 0.0, infinito, 0.0);
									vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, criarRestricoesHidraulicas_nomeVarEstado(a_TSS, a_dados, a_idEstagio, a_periodPrev, a_period, idRH), varRH_prev, criarRestricoesHidraulicas(a_TSS, a_dados, IdEstagio(a_idEstagio - 1), a_periodPrev, a_period, IdPatamarCarga_Nenhum, idRH));
								}
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineRH_VSUP, -var_abs_sup);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_prev, ineRH_VSUP, 1.0);
							}
						} // if (getIneLinear_RH_VSUPseExistir(a_TSS, a_idEstagio, a_period, idRH) == -1) {

					} // if ((vlr_min != var_abs_sup) && (vlr_max != var_abs_sup)) {
				} // if (a_dados.getSize1Matriz(idRH, AttMatrizRestricaoOperativaUHE_lim_sup) > 0) {

				//************************************************************
				//Formulacao: RH + RH_FINF >= RH_lag1 * var_rel_inf
				//************************************************************

				if (a_dados.getSize1Matriz(idRH, AttMatrizRestricaoOperativaUHE_var_rel_inf) > 0) {
					const double var_rel_inf = a_dados.getElementoMatriz(idRH, AttMatrizRestricaoOperativaUHE_var_rel_inf, a_period, IdPatamarCarga_1, double());

					if ((vlr_min != var_rel_inf) && (vlr_max != var_rel_inf)) {

						const int varRH = criarRestricoesHidraulicas(a_TSS, a_dados, a_idEstagio, a_period, a_periodNext, IdPatamarCarga_Nenhum, idRH);
						if (getIneLinear_RH_VRINFseExistir(a_TSS, a_idEstagio, a_period, idRH) == -1) {
							const int ineRH_VINF = addIneLinear_RH_VRINF(a_TSS, a_idEstagio, a_period, idRH);
							const int varRH_FINF = addVarDecisao_RH_VRINF_FINF(a_TSS, a_idEstagio, a_period, idRH, 0.0, infinito, 0.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_FINF, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -penalidade);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH, ineRH_VINF, 1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_FINF, ineRH_VINF, 1.0);

							if ((a_idEstagio == stageIni) && (a_period == a_periodIni_stage))
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineRH_VINF, a_dados.getAtributo(idRH, AttComumRestricaoOperativaUHE_vlr_ini, double()) * var_rel_inf);
							else {
								int varRH_prev = getVarDecisao_RHseExistir(a_TSS, a_idEstagio, a_periodPrev, idRH);
								if ((varRH_prev == -1) && (a_periodPrev >= a_periodIni_stage))
									varRH_prev = criarRestricoesHidraulicas(a_TSS, a_dados, a_idEstagio, a_periodPrev, a_period, IdPatamarCarga_Nenhum, idRH);
								else if (varRH_prev == -1) {
									varRH_prev = addVarDecisao_RH(a_TSS, a_idEstagio, a_periodPrev, idRH, 0.0, infinito, 0.0);
									vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, criarRestricoesHidraulicas_nomeVarEstado(a_TSS, a_dados, a_idEstagio, a_periodPrev, a_period, idRH), varRH_prev, criarRestricoesHidraulicas(a_TSS, a_dados, IdEstagio(a_idEstagio - 1), a_periodPrev, a_period, IdPatamarCarga_Nenhum, idRH));
								}
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_prev, ineRH_VINF, - var_rel_inf);
							}
						} // if (getIneLinear_RH_VINFseExistir(a_TSS, a_idEstagio, a_period, idRH) == -1) {

					} // if ((vlr_min != var_rel_inf) && (vlr_max != var_rel_inf)) {
				} // if (a_dados.getSize1Matriz(idRH, AttMatrizRestricaoOperativaUHE_lim_inf) > 0) {

				//************************************************************
				//Formulacao: RH + RH_FSUP <= RH_lag1 * var_rel_sup
				//************************************************************

				if (a_dados.getSize1Matriz(idRH, AttMatrizRestricaoOperativaUHE_var_rel_sup) > 0) {
					const double var_rel_sup = a_dados.getElementoMatriz(idRH, AttMatrizRestricaoOperativaUHE_var_rel_sup, a_period, IdPatamarCarga_1, double());

					if ((vlr_min != var_rel_sup) && (vlr_max != var_rel_sup)) {

						const int varRH = criarRestricoesHidraulicas(a_TSS, a_dados, a_idEstagio, a_period, a_periodNext, IdPatamarCarga_Nenhum, idRH);
						if (getIneLinear_RH_VRSUPseExistir(a_TSS, a_idEstagio, a_period, idRH) == -1) {
							const int ineRH_VSUP = addIneLinear_RH_VRSUP(a_TSS, a_idEstagio, a_period, idRH);
							const int varRH_FSUP = addVarDecisao_RH_VRSUP_FSUP(a_TSS, a_idEstagio, a_period, idRH, 0.0, infinito, 0.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_FSUP, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -penalidade);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH, ineRH_VSUP, -1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_FSUP, ineRH_VSUP, 1.0);

							if ((a_idEstagio == stageIni) && (a_period == a_periodIni_stage))
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineRH_VSUP, -a_dados.getAtributo(idRH, AttComumRestricaoOperativaUHE_vlr_ini, double()) * var_rel_sup);
							else {
								int varRH_prev = getVarDecisao_RHseExistir(a_TSS, a_idEstagio, a_periodPrev, idRH);
								if ((varRH_prev == -1) && (a_periodPrev >= a_periodIni_stage))
									varRH_prev = criarRestricoesHidraulicas(a_TSS, a_dados, a_idEstagio, a_periodPrev, a_period, IdPatamarCarga_Nenhum, idRH);
								else if (varRH_prev == -1) {
									varRH_prev = addVarDecisao_RH(a_TSS, a_idEstagio, a_periodPrev, idRH, 0.0, infinito, 0.0);
									vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, criarRestricoesHidraulicas_nomeVarEstado(a_TSS, a_dados, a_idEstagio, a_periodPrev, a_period, idRH), varRH_prev, criarRestricoesHidraulicas(a_TSS, a_dados, IdEstagio(a_idEstagio - 1), a_periodPrev, a_period, IdPatamarCarga_Nenhum, idRH));
								}
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH_prev, ineRH_VSUP, var_rel_sup);
							}
						} // if (getIneLinear_RH_VSUPseExistir(a_TSS, a_idEstagio, a_period, idRH) == -1) {

					} // if ((vlr_min != var_rel_sup) && (vlr_max != var_rel_sup)) {
				} // if (a_dados.getSize1Matriz(idRH, AttMatrizRestricaoOperativaUHE_lim_sup) > 0) {

			} // if (a_idPat == idPatEndPer) {

		}// for (int pos = 0; pos < numero_restricoes; pos++) {

	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesHidraulicas(" + getFullString(a_TSS) + ",Dados& a_dados, " + getFullString(a_idEstagio) + "," + getFullString(a_periodIni_stage) + "," + getFullString(a_periodPrev) + "," + getFullString(a_period) + "," + getFullString(a_periodNext) + "," + getFullString(a_idPat) + "): \n" + std::string(erro.what())); }

}//void ModeloOtimizacao::criarRestricoesHidraulicas(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados)


std::string ModeloOtimizacao::criarRestricoesHidraulicas_nomeVarEstado(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodPrev, Periodo& a_period, const IdRestricaoOperativaUHE a_idRH, const bool a_fp_local) {

	try {

		std::string nome = getNomeSolverVarDecisao_RH(a_TSS, a_idEstagio, a_periodPrev, a_idRH);

		const IdElementoSistema idElemEnd = a_dados.getMaiorId(a_idRH, IdElementoSistema());
		for (IdElementoSistema idElem = IdElementoSistema_1; idElem <= idElemEnd; idElem++) {
			const TipoVariavelRestricaoOperativa tipoVarRH = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());
			double fator_participacao = 0.0;
			if (a_fp_local)
				fator_participacao = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_fator_participacao, double());
			else
			    fator_participacao = a_dados.getElementoMatriz(a_idRH, idElem, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double());

			nome += ",FP," + getString(fator_participacao) + "," + getString(tipoVarRH);

			// 
			// Nivel
			//

			if (tipoVarRH == TipoVariavelRestricaoOperativa_nivel_final) {
				const IdHidreletrica idUHE = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
				nome += "," + getString(idUHE);
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_nivel_final) {

			// 
			// Volume
			//

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_volume_final) {
				const IdHidreletrica idUHE = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
				nome += "," + getString(idUHE);
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_volume_final) {

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_volume_defluente) {
				const IdHidreletrica idUHE = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
				nome += "," + getString(idUHE);
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_volume_defluente) {

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_volume_desviado) {
				const IdHidreletrica idUHE = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
				const IdHidreletrica idUHE_jus = a_dados.getAtributo(idUHE, AttComumHidreletrica_jusante_desvio, IdHidreletrica());
				nome += "," + getString(idUHE) + "," + getString(idUHE_jus);
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_volume_defluente) {

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_volume_bombeado) {
				const IdUsinaElevatoria idBOM = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());
				const IdHidreletrica idUHE = a_dados.getAtributo(idBOM, AttComumUsinaElevatoria_usina_origem_bombeamento, IdHidreletrica());
				const IdHidreletrica idUHE_jus = a_dados.getAtributo(idBOM, AttComumUsinaElevatoria_usina_destino_bombeamento, IdHidreletrica());
				nome += "," + getString(idUHE) + "," + getString(idUHE_jus);
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_volume_bombeado) {

			// 
			// Vazao
			//

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_afluente) {
				const IdHidreletrica idUHE = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
				nome += "," + getString(idUHE);
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_afluente) {

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_defluente) {
				const IdHidreletrica idUHE = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
				nome += "," + getString(idUHE);
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_defluente) {

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_turbinada) {
				const IdHidreletrica idUHE = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
				nome += "," + getString(idUHE);
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_turbinada) {

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_vertida) {
				const IdHidreletrica idUHE = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
				nome += "," + getString(idUHE);
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_vertida) {

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_desviada) {
				const IdHidreletrica idUHE = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
				const IdHidreletrica idUHE_jus = a_dados.getAtributo(idUHE, AttComumHidreletrica_jusante_desvio, IdHidreletrica());
				nome += "," + getString(idUHE) + "," + getString(idUHE_jus);
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_desviada) {

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_bombeada) {
				const IdUsinaElevatoria idBOM = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());
				const IdHidreletrica idUHE = a_dados.getAtributo(idBOM, AttComumUsinaElevatoria_usina_origem_bombeamento, IdHidreletrica());
				const IdHidreletrica idUHE_jus = a_dados.getAtributo(idBOM, AttComumUsinaElevatoria_usina_destino_bombeamento, IdHidreletrica());
				nome += "," + getString(idUHE) + "," + getString(idUHE_jus);
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_bombeada) {

			// 
			// Potencia
			//

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_potencia) {
				const IdHidreletrica idUHE = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
				nome += "," + getString(idUHE);
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_potencia) {

			// 
			// Potencia Disponivel
			//

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_potencia_disponivel) {
				const IdHidreletrica idUHE = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
				nome += "," + getString(idUHE);
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_potencia_disponivel) {

			else
				throw std::invalid_argument("Tipo invalido");

		} // for (IdElementoSistema idElem = IdElementoSistema_1; idElem <= idElemEnd; idElem++) {

		return nome;

	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesHidraulicas_nomeVarEstado(" + getFullString(a_TSS) + ",Dados& a_dados, " + getFullString(a_idEstagio) + "," + getFullString(a_periodPrev) + "," + getFullString(a_period) + "," + getFullString(a_idRH) + "): \n" + std::string(erro.what())); }

}//void ModeloOtimizacao::criarRestricoesHidraulicas(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados)


int ModeloOtimizacao::criarRestricoesHidraulicas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idStage, IdRestricaoOperativaUHE& a_idRH, const std::vector<std::string>& a_stateVar, const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>& a_horizon) {

	try {

		if (a_stateVar.at(0) != "RH")
			throw std::invalid_argument("Not RH.");

		Periodo periodo = Periodo(a_stateVar.at(2));

		std::vector<std::vector<std::string>> elemsRH = std::vector<std::vector<std::string>>();
		elemsRH.reserve(1);

		int cont = 4;
		while (cont < a_stateVar.size()) {
			if (a_stateVar.at(cont) != "FP")
				throw std::invalid_argument("Error FP do " + getFullString(a_idStage));
			elemsRH.push_back(std::vector<std::string>());
			elemsRH.at(elemsRH.size() - 1).reserve(4);
			elemsRH.at(elemsRH.size() - 1).push_back(a_stateVar.at(cont));
			for (int i = cont + 1; i < a_stateVar.size(); i++) {
				if (a_stateVar.at(i) == "FP")
					break;
				elemsRH.at(elemsRH.size() - 1).push_back(a_stateVar.at(i));

			}
			cont += int(elemsRH.at(elemsRH.size() - 1).size());
		}

		std::vector<Periodo> periods = a_horizon.getIteradores(periodo);

		bool period_fit = false;
		if (periods.size() == 1) {
			if (periods.at(0) == periodo)
				period_fit = true;
		}

		a_idRH = IdRestricaoOperativaUHE_Nenhum;
		if (period_fit) {
			const IdRestricaoOperativaUHE idRHIni = a_dados.getMenorId(IdRestricaoOperativaUHE());
			const IdRestricaoOperativaUHE idRHOut = a_dados.getIdOut(IdRestricaoOperativaUHE());
			for (IdRestricaoOperativaUHE idRH = idRHIni; idRH < idRHOut; a_dados.incr(idRH)) {
				if (a_dados.vetorRestricaoOperativaUHE.at(idRH).vetorElementoSistema.numObjetos() == elemsRH.size()) {
					for (IdElementoSistema idElem = IdElementoSistema_1; idElem <= a_dados.getMaiorId(idRH, IdElementoSistema()); idElem++) {
						bool idElemFound = false;
						for (int e = 0; e < int(elemsRH.size()); e++) {
							if (a_dados.getSize1Matriz(idRH, idElem, AttMatrizElementoSistema_fator_participacao) > 0) {
								if (a_dados.getElementoMatriz(idRH, idElem, AttMatrizElementoSistema_fator_participacao, periodo, IdPatamarCarga_1, double()) == getdoubleFromChar(elemsRH.at(e).at(1).c_str())) {
									if (a_dados.getAtributo(idRH, idElem, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa()) == getTipoVariavelRestricaoOperativaFromChar(elemsRH.at(e).at(2).c_str())) {
										const IdHidreletrica idUHE = a_dados.getAtributo(idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
										if ((idUHE != IdHidreletrica_Nenhum) && (idUHE == getIdHidreletricaFromChar(elemsRH.at(e).at(3).c_str()))) {
											if (elemsRH.at(e).size() > 4) {
												if (a_dados.getAtributo(idRH, idElem, AttComumElementoSistema_conjuntohidraulico, IdConjuntoHidraulico()) == getIdConjuntoHidraulicoFromChar(elemsRH.at(e).at(4).c_str())) {
													if (elemsRH.at(e).size() > 5) {
														if (a_dados.getAtributo(idRH, idElem, AttComumElementoSistema_unidadehidraulica, IdUnidadeUHE()) == getIdUnidadeUHEFromChar(elemsRH.at(e).at(5).c_str())) {
															if (elemsRH.at(e).size() > 6)
																throw std::invalid_argument("Error Elem de RH do " + getFullString(a_idStage));
															else {
																idElemFound = true;
																break;
															}
														}
													}
													else {
														idElemFound = true;
														break;
													}
												}
											}
											else {
												idElemFound = true;
												break;
											}
										}
									}
								}
							}
						} // for (int e = 0; e < int(elemsRH.size()); e++) {
						if (!idElemFound) {
							a_idRH = IdRestricaoOperativaUHE_Nenhum;
							break;
						}
						else
							a_idRH = idRH;
					}
				}
			}
		}

		if (a_idRH == IdRestricaoOperativaUHE_Nenhum) {
			a_idRH = a_dados.getIdOut(IdRestricaoOperativaUHE());
			RestricaoOperativaUHE rh;
			rh.setAtributo(AttComumRestricaoOperativaUHE_idRestricaoOperativaUHE, a_idRH);
			a_dados.vetorRestricaoOperativaUHE.add(rh);

			IdElementoSistema idElem = IdElementoSistema_1;
			for (int e = 0; e < int(elemsRH.size()); e++) {
				ElementoSistema elem;
				elem.setAtributo(AttComumElementoSistema_idElementoSistema, idElem);
				a_dados.vetorRestricaoOperativaUHE.at(a_idRH).vetorElementoSistema.add(elem);
				a_dados.vetorRestricaoOperativaUHE.at(a_idRH).vetorElementoSistema.at(idElem).setAtributo(AttComumElementoSistema_fator_participacao, getdoubleFromChar(elemsRH.at(e).at(1).c_str()));
				a_dados.vetorRestricaoOperativaUHE.at(a_idRH).vetorElementoSistema.at(idElem).setAtributo(AttComumElementoSistema_tipoVariavelRestricaoOperativa, getTipoVariavelRestricaoOperativaFromChar(elemsRH.at(e).at(2).c_str()));
				a_dados.vetorRestricaoOperativaUHE.at(a_idRH).vetorElementoSistema.at(idElem).setAtributo(AttComumElementoSistema_hidreletrica, getIdHidreletricaFromChar(elemsRH.at(e).at(3).c_str()));
				if (elemsRH.at(e).size() > 4) {
					a_dados.vetorRestricaoOperativaUHE.at(a_idRH).vetorElementoSistema.at(idElem).setAtributo(AttComumElementoSistema_conjuntohidraulico, getIdConjuntoHidraulicoFromChar(elemsRH.at(e).at(4).c_str()));
					if (elemsRH.at(e).size() > 5) {
						a_dados.vetorRestricaoOperativaUHE.at(a_idRH).vetorElementoSistema.at(idElem).setAtributo(AttComumElementoSistema_unidadehidraulica, getIdUnidadeUHEFromChar(elemsRH.at(e).at(5).c_str()));
						if (elemsRH.at(e).size() > 6)
							throw std::invalid_argument("Error Elem de RH do " + getFullString(a_idStage));
					}
				}
				idElem++;
			} // for (int e = 0; e < int(elemsRH.size()); e++) {
		} // if (a_idRH == IdRestricaoOperativaUHE_Nenhum) {

		return criarVariaveisDecisao_VariaveisEstado_Restricoes_RH(a_TSS, a_dados, a_idStage, periodo, a_idRH, a_horizon, true);


	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesHidraulicas(" + getFullString(a_TSS) + ",Dados& a_dados, " + getFullString(a_idStage) + "): \n" + std::string(erro.what())); }

}


int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_RH(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idStage, Periodo& a_period_lag, const IdRestricaoOperativaUHE a_idRH, const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>& a_horizon, bool a_isVarEstadoExterna) {

	try {

		const IdEstagio stageIni = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());

		if (a_idStage < stageIni)
			return -1;

		if ((!vetorEstagio.at(a_idStage).isSolverInstanciado(a_TSS)) || (a_TSS == TipoSubproblemaSolver_mestre))
			return -1;

		const Periodo periodo_otimizacao = getAtributo(a_idStage, AttComumEstagio_periodo_otimizacao, Periodo());

		const Periodo periodEnd_stage = getIterador2Final(AttMatrizModeloOtimizacao_horizonte_estudo, a_idStage, Periodo());

		// Verifica se periodo não é mais necessario para compor ou repassar lag
		if (periodEnd_stage + 1 <= a_period_lag)
			return -1;

		const Periodo periodIni_stage = getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_estudo, a_idStage, Periodo());
		const Periodo periodIni_stageIni = getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_estudo, stageIni, Periodo());

		int varRH = getVarDecisao_RHseExistir(a_TSS, a_idStage, a_period_lag, a_idRH);
		if (varRH > -1)
			return varRH;

		const double infinito = vetorEstagio.at(a_idStage).getSolver(a_TSS)->getInfinito();

		varRH = addVarDecisao_RH(a_TSS, a_idStage, a_period_lag, a_idRH, -infinito, infinito, 0.0);

		// ADD any compound of QD in the current stage.
		int equRH = -1;
		const double sobreposicao_periodo_otimizacao = periodo_otimizacao.sobreposicao(a_period_lag);
		if (sobreposicao_periodo_otimizacao > 0.0) {

			equRH = addEquLinear_RH(a_TSS, a_idStage, a_period_lag, a_idRH);
			vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(varRH, equRH, 1.0);

			bool sobreposicao_encontrada = false;
			for (Periodo periodo = periodIni_stage; periodo <= periodEnd_stage; a_horizon.incrementarIterador(periodo)) {

				const double sobreposicao = a_period_lag.sobreposicao(periodo);

				if (sobreposicao > 0.0) {
					int varRH_per = getVarDecisao_RHseExistir(a_TSS, a_idStage, periodo, a_idRH);
					if (varRH_per == -1)
						varRH_per = addVarDecisao_RH(a_TSS, a_idStage, periodo, a_idRH, 0.0, infinito, 0.0);

					int equRH_per = getEquLinear_RHseExistir(a_TSS, a_idStage, periodo, a_idRH);
					if (equRH_per == -1) {
						equRH_per = addEquLinear_RH(a_TSS, a_idStage, periodo, a_idRH);
						vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(varRH_per, equRH_per, 1.0);


						for (IdElementoSistema idElem = IdElementoSistema_1; idElem < a_dados.vetorRestricaoOperativaUHE.at(a_idRH).vetorElementoSistema.getIdOut(); a_dados.vetorRestricaoOperativaUHE.at(a_idRH).vetorElementoSistema.incr(idElem)) {

							const double fp = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_fator_participacao, double());
							const TipoVariavelRestricaoOperativa tipo = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());
							const IdHidreletrica idUHE = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
							const IdConjuntoHidraulico idConj = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_conjuntohidraulico, IdConjuntoHidraulico());
							const IdUnidadeUHE idUnd = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_unidadehidraulica, IdUnidadeUHE());

							if (tipo == TipoVariavelRestricaoOperativa_vazao_vertida) {
								if ((idConj != IdConjuntoHidraulico_Nenhum) || (idUnd != IdUnidadeUHE_Nenhum))
									throw std::invalid_argument("Not available for " + getFullString(tipo) + " in " + getFullString(idElem) + " of " + getFullString(a_idRH) + ".");
								if (getVarDecisao_QVERseExistir(a_TSS, a_idStage, periodo, idUHE) > -1)
									vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QVER(a_TSS, a_idStage, periodo, idUHE), equRH_per, -fp);
								else
									vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QVER(a_TSS, a_idStage, periodo, IdPatamarCarga_1, idUHE), equRH_per, -fp);
							}

							else if (tipo == TipoVariavelRestricaoOperativa_vazao_defluente) {
								if ((idConj != IdConjuntoHidraulico_Nenhum) || (idUnd != IdUnidadeUHE_Nenhum))
									throw std::invalid_argument("Not available for " + getFullString(tipo) + " in " + getFullString(idElem) + " of " + getFullString(a_idRH) + ".");
								if (getVarDecisao_QDEFseExistir(a_TSS, a_idStage, periodo, idUHE) > -1)
									vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idStage, periodo, idUHE), equRH_per, -fp);
								else
									vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idStage, periodo, IdPatamarCarga_1, idUHE), equRH_per, -fp);
							}

							else if (tipo == TipoVariavelRestricaoOperativa_vazao_afluente) {
								if ((idConj != IdConjuntoHidraulico_Nenhum) || (idUnd != IdUnidadeUHE_Nenhum))
									throw std::invalid_argument("Not available for " + getFullString(tipo) + " in " + getFullString(idElem) + " of " + getFullString(a_idRH) + ".");
								vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QINC(a_TSS, a_idStage, periodo, idUHE), equRH_per, -fp);
							}

							else if (tipo == TipoVariavelRestricaoOperativa_vazao_turbinada) {
								if ((idConj != IdConjuntoHidraulico_Nenhum) && (idUnd != IdUnidadeUHE_Nenhum)) {
									if (a_dados.getElementoVetor(idUHE, idConj, idUnd, AttVetorUnidadeUHE_disponibilidade, periodo, double()) > 0.0) {
										if (getVarDecisao_QTURDISPseExistir(a_TSS, a_idStage, periodo, idUHE, idConj, idUnd) > -1)
											vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idStage, periodo, idUHE, idConj, idUnd), equRH_per, -fp);
										else
											vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idStage, periodo, IdPatamarCarga_1, idUHE, idConj, idUnd), equRH_per, -fp);
									}
								}
								else if (idConj != IdConjuntoHidraulico_Nenhum) {
									if (a_dados.getElementoVetor(idUHE, idConj, AttVetorConjuntoHidraulico_disponibilidade, periodo, double()) > 0.0) {
										if (getVarDecisao_QTURDISPseExistir(a_TSS, a_idStage, periodo, idUHE, idConj) > -1)
											vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idStage, periodo, idUHE, idConj), equRH_per, -fp);
										else
											vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idStage, periodo, IdPatamarCarga_1, idUHE, idConj), equRH_per, -fp);
									}
								}
								else {
									if (a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_disponibilidade, periodo, double()) > 0.0) {
										if (getVarDecisao_QTURDISPseExistir(a_TSS, a_idStage, periodo, idUHE) > -1)
											vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idStage, periodo, idUHE), equRH_per, -fp);
										else
											vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idStage, periodo, IdPatamarCarga_1, idUHE), equRH_per, -fp);
									}
								}
							}
							else if (tipo == TipoVariavelRestricaoOperativa_potencia_disponivel) {
								if ((idConj != IdConjuntoHidraulico_Nenhum) && (idUnd != IdUnidadeUHE_Nenhum)) {
									if (a_dados.getElementoVetor(idUHE, idConj, idUnd, AttVetorUnidadeUHE_disponibilidade, periodo, double()) > 0.0) {
										if (getVarDecisao_PHDISPseExistir(a_TSS, a_idStage, periodo, idUHE, idConj, idUnd) > -1)
											vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idStage, periodo, idUHE, idConj, idUnd), equRH_per, -fp);
										else
											vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idStage, periodo, IdPatamarCarga_1, idUHE, idConj, idUnd), equRH_per, -fp);
									}
								}
								else if (idConj != IdConjuntoHidraulico_Nenhum) {
									if (a_dados.getElementoVetor(idUHE, idConj, AttVetorConjuntoHidraulico_disponibilidade, periodo, double()) > 0.0) {
										if (getVarDecisao_PHDISPseExistir(a_TSS, a_idStage, periodo, idUHE, idConj) > -1)
											vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idStage, periodo, idUHE, idConj), equRH_per, -fp);
										else
											vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idStage, periodo, IdPatamarCarga_1, idUHE, idConj), equRH_per, -fp);
									}
								}
								else {
									if (a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_disponibilidade, periodo, double()) > 0.0) {
										if (getVarDecisao_PHDISPseExistir(a_TSS, a_idStage, periodo, idUHE) > -1)
											vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idStage, periodo, idUHE), equRH_per, -fp);
										else
											vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idStage, periodo, IdPatamarCarga_1, idUHE), equRH_per, -fp);
									}
								}
							}
							else
								throw std::invalid_argument("Not available");

						}

					} // if (equRH_per == -1) {

					if (sobreposicao < 1.0) {
						vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(varRH_per, equRH, -sobreposicao);
						if (!sobreposicao_encontrada)
							sobreposicao_encontrada = true;
					}
					else if (sobreposicao == 1.0)
						return varRH;

				}
				else if ((sobreposicao == 0.0) && (sobreposicao_encontrada))
					break;

			} // for (Periodo periodo = periodIni_stage; periodo <= periodEnd_stage; horiz_stage.incrementarIterador(periodo)) {

			if (a_period_lag >= periodIni_stage)
				return varRH;

		} // if (sobreposicao_periodo_otimizacao > 0.0) {

		// ADD any deterministic (past) compound of RH.
		// Only in stage where a_period belongs
		if (((a_period_lag < periodIni_stageIni) && (stageIni == a_idStage)) || (a_period_lag + 1 <= periodIni_stageIni)) {

			equRH = getEquLinear_RHseExistir(a_TSS, a_idStage, a_period_lag, a_idRH);

			if (equRH == 0) {
				equRH = addEquLinear_RH(a_TSS, a_idStage, a_period_lag, a_idRH);
				vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(varRH, equRH, 1.0);
			}

			const double vlr_ini = a_dados.getAtributo(a_idRH, AttComumRestricaoOperativaUHE_vlr_ini, double());

			const double vlr_min = getdoubleFromChar("min");
			const double vlr_max = getdoubleFromChar("max");

			if ((vlr_min >= vlr_ini) || (vlr_ini <= vlr_max))
				throw std::invalid_argument("Initial value not available");

			vetorEstagio.at(a_idStage).getSolver(a_TSS)->setRHSRestricao(equRH, vlr_ini);

			return varRH;

		} // if (((a_period_lag < periodIni_stageIni) && (stageIni == a_idStage)) || (a_period_lag + 1 <= periodIni_stageIni)) {

		// Recursively ADD any compound of RH in past stages.
		const IdEstagio idEstagio_anterior = IdEstagio(a_idStage - 1);

		int varRH_anterior = -1;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica) {
			for (TipoSubproblemaSolver tss = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tss < TipoSubproblemaSolver_Excedente; tss++) {
				if (tss != TipoSubproblemaSolver_viabilidade_hidraulica)
					varRH_anterior = criarVariaveisDecisao_VariaveisEstado_Restricoes_RH(tss, a_dados, idEstagio_anterior, a_period_lag, a_idRH, a_horizon, a_isVarEstadoExterna);
				if (varRH_anterior > -1)
					break;
			}
		}
		else
			varRH_anterior = criarVariaveisDecisao_VariaveisEstado_Restricoes_RH(a_TSS, a_dados, idEstagio_anterior, a_period_lag, a_idRH, a_horizon, a_isVarEstadoExterna);

		if (varRH_anterior > -1) {

			if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
				varRH_anterior = -1;

			Periodo periodAux = a_period_lag;

			// Variáveis de estado a compor lag
			if (sobreposicao_periodo_otimizacao > 0.0) {
				const int varRH_ADD = addVarDecisao_RH_ADD(a_TSS, a_idStage, a_period_lag, a_idRH, 0.0, infinito, 0.0);
				vetorEstagio.at(a_idStage).addVariavelEstado(a_TSS, criarRestricoesHidraulicas_nomeVarEstado(a_TSS, a_dados, a_idStage, a_period_lag, periodAux, a_idRH, true), varRH_ADD, varRH_anterior, a_isVarEstadoExterna);
				vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(varRH_ADD, equRH, -1.0);
			}

			// Variáveis de estado a repassar lag
			else
				vetorEstagio.at(a_idStage).addVariavelEstado(a_TSS, criarRestricoesHidraulicas_nomeVarEstado(a_TSS, a_dados, a_idStage, a_period_lag, periodAux, a_idRH, true), varRH, varRH_anterior, a_isVarEstadoExterna);

		} // if (varQDLAG_anterior > -1){

		return varRH;



	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisDecisao_VariaveisEstado_Restricoes_RH(" + getFullString(a_TSS) + ",Dados& a_dados, " + getFullString(a_idStage) + "," + getFullString(a_period_lag) + "," + getFullString(a_idRH) + "): \n" + std::string(erro.what())); }

}



int ModeloOtimizacao::criarRestricoesHidraulicas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, Periodo& a_periodNext, const IdPatamarCarga a_idPat, const IdRestricaoOperativaUHE a_idRH) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return -1;

		int equRH = -1;
		int varRH = -1;

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const IdPatamarCarga idPatEndPer = a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, a_period, IdPatamarCarga());

		if (a_idPat != IdPatamarCarga_Nenhum) {
			equRH = getEquLinear_RHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idRH);
			if (equRH > -1) { return getVarDecisao_RH(a_TSS, a_idEstagio, a_period, a_idPat, a_idRH); }
			equRH = addEquLinear_RH(a_TSS, a_idEstagio, a_period, a_idPat, a_idRH);
			varRH = addVarDecisao_RH(a_TSS, a_idEstagio, a_period, a_idPat, a_idRH, -infinito, infinito, 0.0);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH, equRH, 1.0);
		}
		else {
			equRH = getEquLinear_RHseExistir(a_TSS, a_idEstagio, a_period, a_idRH);
			if (equRH > -1) { return getVarDecisao_RH(a_TSS, a_idEstagio, a_period, a_idRH); }
			equRH = addEquLinear_RH(a_TSS, a_idEstagio, a_period, a_idRH);
			varRH = addVarDecisao_RH(a_TSS, a_idEstagio, a_period, a_idRH, -infinito, infinito, 0.0);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRH, equRH, 1.0);
		}

		const IdElementoSistema idElemEnd = a_dados.getMaiorId(a_idRH, IdElementoSistema());
		for (IdElementoSistema idElem = IdElementoSistema_1; idElem <= idElemEnd; idElem++) {
			const TipoVariavelRestricaoOperativa tipoVarRH = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());
			
			double fator_participacao = a_dados.getElementoMatriz(a_idRH, idElem, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double());
			if (a_idPat != IdPatamarCarga_Nenhum)
				fator_participacao = a_dados.getElementoMatriz(a_idRH, idElem, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double());

			// 
			// Nivel
			//

			if (tipoVarRH == TipoVariavelRestricaoOperativa_nivel_final) {
				const IdHidreletrica idUHE = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());

				if (a_idPat != IdPatamarCarga_Nenhum)
					throw std::invalid_argument("Variavel " + getFullString(tipoVarRH) + " nao compativel com patamar em " + getFullString(idUHE) + " em " + getFullString(idElem) + " em " + getFullString(a_idRH));

				int varHF = getVarDecisao_HFseExistir(a_TSS, a_idEstagio, a_period, idUHE);

				if (varHF == -1) {
					int varVF = getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_periodNext, idUHE);
					if (varVF == -1) { varVF = getVarDecisao_VFseExistir(a_TSS, a_idEstagio, a_period, idUHE); }
					if (varVF == -1) { throw std::invalid_argument("Variavel Decisao de Volume para " + getFullString(tipoVarRH) + " nao encontrada em " + getFullString(idUHE) + " em " + getFullString(idElem) + " em " + getFullString(a_idRH)); }

					varHF = addVarDecisao_HF(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);
					const int equHF = addEquLinear_HF(a_TSS, a_idEstagio, a_period, idUHE);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varHF, equHF, 1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varVF, equHF, -a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_coef_linear_cota_montante_1, a_period, double()));
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(equHF, a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_coef_linear_cota_montante_0, a_period, double()) + a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_coef_linear_cota_montante_1, a_period, double()) * a_dados.getElementoVetor(idUHE, IdReservatorio_1, AttVetorReservatorio_volume_minimo, a_period, double()));
				}
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varHF, equRH, -fator_participacao);
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_nivel_final) {

			// 
			// Volume
			//

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_volume_final) {
				const IdHidreletrica idUHE = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
				int varVF = getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_periodNext, idUHE);
				if (varVF == -1) { varVF = getVarDecisao_VFseExistir(a_TSS, a_idEstagio, a_period, idUHE); }
				if (varVF == -1) { throw std::invalid_argument("Variavel Decisao para " + getFullString(tipoVarRH) + " nao encontrada em " + getFullString(idUHE) + " em " + getFullString(idElem) + " em " + getFullString(a_idRH)); }

				if (a_idPat != IdPatamarCarga_Nenhum)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varVF, equRH, -fator_participacao*a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, a_idPat, double()));
				else
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varVF, equRH, -fator_participacao);
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_volume_final) {

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_volume_defluente) {
				const IdHidreletrica idUHE = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
				if (a_idPat == IdPatamarCarga_Nenhum) {
					if (getVarDecisao_QDEFseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, idUHE), equRH, -a_dados.getElementoVetor(AttVetorDados_conversor_vazao_volume, a_period, double()));
					else if (idPatEndPer == IdPatamarCarga_1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE), equRH, -a_dados.getElementoVetor(AttVetorDados_conversor_vazao_volume, a_period, double()));
					else {
						const int varQDEF = addVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);
						const int equQDEF = addEquLinear_QDEF(a_TSS, a_idEstagio, a_period, idUHE);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDEF, equQDEF, 1.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDEF, equRH, -fator_participacao*a_dados.getElementoVetor(AttVetorDados_conversor_vazao_volume, a_period, double()));
						for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, idPat, idUHE), equQDEF, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
					}
				}
				else
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), equRH, -fator_participacao*a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, a_period, a_idPat, double()));
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_volume_defluente) {

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_volume_desviado) {
				const IdHidreletrica idUHE = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
				const IdHidreletrica idUHE_jus = a_dados.getAtributo(idUHE, AttComumHidreletrica_jusante_desvio, IdHidreletrica());
				if (a_idPat == IdPatamarCarga_Nenhum) {
					if (getVarDecisao_QDESseExistir(a_TSS, a_idEstagio, a_period, idUHE, idUHE_jus) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_period, idUHE, idUHE_jus), equRH, -fator_participacao*a_dados.getElementoVetor(AttVetorDados_conversor_vazao_volume, a_period, double()));
					else if (idPatEndPer == IdPatamarCarga_1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE, idUHE_jus), equRH, -fator_participacao*a_dados.getElementoVetor(AttVetorDados_conversor_vazao_volume, a_period, double()));
					else {
						const int varQDES = addVarDecisao_QDES(a_TSS, a_idEstagio, a_period, idUHE, idUHE_jus, 0.0, infinito, 0.0);
						const int equQDES = addEquLinear_QDES(a_TSS, a_idEstagio, a_period, idUHE, idUHE_jus);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDES, equQDES, 1.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDES, equRH, -a_dados.getElementoVetor(AttVetorDados_conversor_vazao_volume, a_period, double()));
						for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_period, idPat, idUHE, idUHE_jus), equQDES, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
					}
				}
				else
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idUHE_jus), equRH, -fator_participacao*a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, a_period, a_idPat, double()));
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_volume_defluente) {

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_volume_bombeado) {
				const IdUsinaElevatoria idBOM = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());
				if (a_idPat == IdPatamarCarga_Nenhum) {
					if (getVarDecisao_QBOMseExistir(a_TSS, a_idEstagio, a_period, idBOM) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_period, idBOM), equRH, -fator_participacao*a_dados.getElementoVetor(AttVetorDados_conversor_vazao_volume, a_period, double()));
					else if (getVarDecisao_QBOMseExistir(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idBOM) > -1) {
						if (idPatEndPer == IdPatamarCarga_1)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idBOM), equRH, -fator_participacao * a_dados.getElementoVetor(AttVetorDados_conversor_vazao_volume, a_period, double()));
						else {
							const int varQBOM = addVarDecisao_QBOM(a_TSS, a_idEstagio, a_period, idBOM, 0.0, infinito, 0.0);
							const int equQBOM = addEquLinear_QBOM(a_TSS, a_idEstagio, a_period, idBOM);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQBOM, equQBOM, 1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQBOM, equRH, -a_dados.getElementoVetor(AttVetorDados_conversor_vazao_volume, a_period, double()));
							for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_period, idPat, idBOM), equQBOM, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
						}
					}
				}
				else if (getVarDecisao_QBOMseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idBOM) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_period, a_idPat, idBOM), equRH, -fator_participacao * a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, a_period, a_idPat, double()));
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_volume_bombeado) {

			// 
			// Vazao
			//

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_afluente) {
				const IdHidreletrica idUHE = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
				if (a_idPat != IdPatamarCarga_Nenhum)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QINC(a_TSS, a_idEstagio, a_period, idUHE), equRH, -fator_participacao);
				else
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QINC(a_TSS, a_idEstagio, a_period, idUHE), equRH, -fator_participacao);
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_afluente) {

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_defluente) {
				const IdHidreletrica idUHE = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
				if (a_idPat == IdPatamarCarga_Nenhum) {
					if (getVarDecisao_QDEFseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, idUHE), equRH, -fator_participacao);
					else if (idPatEndPer == IdPatamarCarga_1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE), equRH, -fator_participacao);
					else {
						const int varQDEF = addVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);
						const int equQDEF = addEquLinear_QDEF(a_TSS, a_idEstagio, a_period, idUHE);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDEF, equQDEF, 1.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDEF, equRH, -fator_participacao);
						for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, idPat, idUHE), equQDEF, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
					}
				}
				else 
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), equRH, -fator_participacao);
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_defluente) {

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_turbinada) {
				const IdHidreletrica idUHE = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
				if (a_idPat == IdPatamarCarga_Nenhum) {
					if (a_dados.getAtributo(AttComumDados_representar_turbinamento_disponivel_em_restricoes_hidraulicas, bool())) {
						if (getVarDecisao_QTURDISPseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_period, idUHE), equRH, -fator_participacao);
						else if (getVarDecisao_QTURDISPseExistir(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE) > -1) {
							if (idPatEndPer == IdPatamarCarga_1)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE), equRH, -fator_participacao);
							else {
								const int varQTURDISP = addVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);
								const int equQTURDISP = addEquLinear_QTURDISP(a_TSS, a_idEstagio, a_period, idUHE);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQTURDISP, equQTURDISP, 1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQTURDISP, equRH, -fator_participacao);
								for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_period, idPat, idUHE), equQTURDISP, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
							}
						}
					}
					else {
						if (getVarDecisao_QTURseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_period, idUHE), equRH, -fator_participacao);
						else if (getVarDecisao_QTURseExistir(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE) > -1) {
							if (idPatEndPer == IdPatamarCarga_1)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE), equRH, -fator_participacao);
							else {
								const int varQTUR = addVarDecisao_QTUR(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);
								const int equQTUR = addEquLinear_QTUR(a_TSS, a_idEstagio, a_period, idUHE);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQTUR, equQTUR, 1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQTUR, equRH, -fator_participacao);
								for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_period, idPat, idUHE), equQTUR, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
							}
						}
					}
				}
				else {
					int varQTUR = -1;
					if (a_dados.getAtributo(AttComumDados_representar_turbinamento_disponivel_em_restricoes_hidraulicas, bool()) && (getVarDecisao_QTURDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1))
						varQTUR = getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE);
					else if (getVarDecisao_QTURseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
						varQTUR = getVarDecisao_QTUR(a_TSS, a_idEstagio, a_period, a_idPat, idUHE);
					if (varQTUR > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQTUR, equRH, -fator_participacao);
				}
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_turbinada) {

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_vertida) {
				const IdHidreletrica idUHE = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
				if (a_idPat == IdPatamarCarga_Nenhum) {
					if (getVarDecisao_QVERseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QVER(a_TSS, a_idEstagio, a_period, idUHE), equRH, -fator_participacao);
					else if (idPatEndPer == IdPatamarCarga_1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QVER(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE), equRH, -fator_participacao);
					else {
						const int varQVER = addVarDecisao_QVER(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);
						const int equQVER = addEquLinear_QVER(a_TSS, a_idEstagio, a_period, idUHE);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQVER, equQVER, 1.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQVER, equRH, -fator_participacao);
						for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QVER(a_TSS, a_idEstagio, a_period, idPat, idUHE), equQVER, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
					}
				}
				else
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QVER(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), equRH, -fator_participacao);
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_vertida) {

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_desviada) {
				const IdHidreletrica idUHE = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
				const IdHidreletrica idUHE_jus = a_dados.getAtributo(idUHE, AttComumHidreletrica_jusante_desvio, IdHidreletrica());
				if (a_idPat == IdPatamarCarga_Nenhum) {
					if (getVarDecisao_QDESseExistir(a_TSS, a_idEstagio, a_period, idUHE, idUHE_jus) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_period, idUHE, idUHE_jus), equRH, -fator_participacao);
					else if (idPatEndPer == IdPatamarCarga_1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE, idUHE_jus), equRH, -fator_participacao);
					else {
						const int varQDES = addVarDecisao_QDES(a_TSS, a_idEstagio, a_period, idUHE, idUHE_jus, 0.0, infinito, 0.0);
						const int equQDES = addEquLinear_QDES(a_TSS, a_idEstagio, a_period, idUHE, idUHE_jus);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDES, equQDES, 1.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDES, equRH, -fator_participacao);
						for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_period, idPat, idUHE, idUHE_jus), equQDES, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
					}
				}
				else
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idUHE_jus), equRH, -fator_participacao);
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_desviada) {

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_bombeada) {
				const IdUsinaElevatoria idBOM = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());
				if (a_idPat == IdPatamarCarga_Nenhum) {
					if (getVarDecisao_QBOMseExistir(a_TSS, a_idEstagio, a_period, idBOM) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_period, idBOM), equRH, -fator_participacao);
					else if (getVarDecisao_QBOMseExistir(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idBOM) > -1) {
						if (idPatEndPer == IdPatamarCarga_1)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idBOM), equRH, -fator_participacao);
						else {
							const int varQBOM = addVarDecisao_QBOM(a_TSS, a_idEstagio, a_period, idBOM, 0.0, infinito, 0.0);
							const int equQBOM = addEquLinear_QBOM(a_TSS, a_idEstagio, a_period, idBOM);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQBOM, equQBOM, 1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQBOM, equRH, -fator_participacao);
							for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_period, idPat, idBOM), equQBOM, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
						}
					}
				}
				else if (getVarDecisao_QBOMseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idBOM) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_period, a_idPat, idBOM), equRH, -fator_participacao);
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_vazao_bombeada) {

			// 
			// Potencia
			//

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_potencia) {
				const IdHidreletrica idUHE = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
				if (a_idPat == IdPatamarCarga_Nenhum) {
					if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, idUHE), equRH, -fator_participacao);
					else if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE) > -1) {
						if (idPatEndPer == IdPatamarCarga_1)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE), equRH, -fator_participacao);
						else {
							const int varPH = addVarDecisao_PH(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);
							const int equPH = addEquLinear_PH(a_TSS, a_idEstagio, a_period, idUHE);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPH, equPH, 1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPH, equRH, -fator_participacao);
							for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, idPat, idUHE), equPH, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
						}
					}
				}
				else if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), equRH, -fator_participacao);
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_potencia) {

			// 
			// Potencia Disponivel
			//

			else if (tipoVarRH == TipoVariavelRestricaoOperativa_potencia_disponivel) {
				const IdHidreletrica idUHE = a_dados.getAtributo(a_idRH, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
				if (a_idPat == IdPatamarCarga_Nenhum) {
					if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, idUHE), equRH, -fator_participacao);
					else if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE) > -1) {
						if (idPatEndPer == IdPatamarCarga_1)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE), equRH, -fator_participacao);
						else {
							const int varPHDISP = addVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);
							const int equPHDISP = addEquLinear_PHDISP(a_TSS, a_idEstagio, a_period, idUHE);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP, equPHDISP, 1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP, equRH, -fator_participacao);
							for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, idPat, idUHE), equPHDISP, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
						}
					}
				}
				else if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), equRH, -fator_participacao);
			} // if (tipoVarRH == TipoVariavelRestricaoOperativa_potencia_disponivel) {

			else
				throw std::invalid_argument("Tipo invalido");

		} // for (IdElementoSistema idElem = IdElementoSistema_1; idElem <= idElemEnd; idElem++) {

		return varRH;

	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesHidraulicas(" + getFullString(a_TSS) + ",Dados& a_dados, " + getFullString(a_idEstagio) + "," + getFullString(a_period) + "," + getFullString(a_periodNext) + "," + getFullString(a_idPat) + "," + getFullString(a_idRH) + "): \n" + std::string(erro.what())); }

}//void ModeloOtimizacao::criarRestricoesHidraulicas(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados)


void ModeloOtimizacao::criarContratos(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodIni_stage, Periodo& a_periodPrev, Periodo& a_period, Periodo& a_periodNext, const IdPatamarCarga a_idPat) {
	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const IdEstagio stageIni = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());

		const IdPatamarCarga idPatEndPer = a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, a_period, IdPatamarCarga());

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const double perc_dur_pat = a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, a_idPat, double());

		const double vlr_min = getdoubleFromChar("min");
		const double vlr_max = getdoubleFromChar("max");

		const IdContrato idConIni = a_dados.getMenorId(IdContrato());
		const IdContrato idConOut = a_dados.getIdOut(IdContrato());

		for (IdContrato idCon = idConIni; idCon < idConOut; a_dados.incr(idCon)) {

			const IdSubmercado idSSE = a_dados.getAtributo(idCon, AttComumContrato_submercado, IdSubmercado());
			const TipoContrato tipo_contrato = a_dados.getAtributo(idCon, AttComumContrato_tipo_contrato, TipoContrato());

			const double custo = a_dados.getElementoMatriz(idCon, AttMatrizContrato_custo, a_period, a_idPat, double());

			//******************************************
			//Formulacao: lim_inf <= PCON <= lim_sup
			//******************************************

			if ((a_dados.getSize1Matriz(idCon, AttMatrizContrato_lim_inf) > 0) && (a_dados.getSize1Matriz(idCon, AttMatrizContrato_lim_sup) > 0)) {
				const IdPatamarCarga idPatEnd = a_dados.getIterador2Final(idCon, AttMatrizContrato_lim_inf, a_period, IdPatamarCarga());

				if (idPatEnd != a_dados.getIterador2Final(idCon, AttMatrizContrato_lim_sup, a_period, IdPatamarCarga()))
					throw std::invalid_argument("Error Pat");

				if (idPatEnd >= a_idPat) {

					const double lim_inf = a_dados.getElementoMatriz(idCon, AttMatrizContrato_lim_inf, a_period, a_idPat, double());
					const double lim_sup = a_dados.getElementoMatriz(idCon, AttMatrizContrato_lim_sup, a_period, a_idPat, double());

					if (((vlr_min != lim_inf) && (vlr_max != lim_inf)) || ((vlr_min != lim_sup) && (vlr_max != lim_sup))) {

						const IdPatamarCarga idPatEnd = a_dados.getIterador2Final(idCon, AttMatrizContrato_lim_inf, a_period, IdPatamarCarga());

						if (idPatEnd == IdPatamarCarga_1) {
							const int varPCON = addVarDecisao_PCON(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato, lim_inf, lim_sup, 0.0);
							for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++) {
								if (tipo_contrato == TipoContrato_exportacao) {
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON, getEquLinear_ZO(a_TSS, a_idEstagio, a_period, idPat), custo);
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PCON(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato), getEquLinear_PL(a_TSS, a_idEstagio, a_period, idPat, idSSE), -1.0);
								}
								else {
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON, getEquLinear_ZO(a_TSS, a_idEstagio, a_period, idPat), -custo);
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PCON(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato), getEquLinear_PL(a_TSS, a_idEstagio, a_period, idPat, idSSE), 1.0);
								}
							}
						}
						else if (idPatEnd == idPatEndPer) {
							const int varPCON_pat = addVarDecisao_PCON(a_TSS, a_idEstagio, a_period, a_idPat, idCon, tipo_contrato, lim_inf, lim_sup, 0.0);
							int varPCON = getVarDecisao_PCONseExistir(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato);
							int equPCON = getEquLinear_PCONseExistir(a_TSS, a_idEstagio, a_period, idCon);
							if (varPCON == -1)
								varPCON = addVarDecisao_PCON(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato, 0.0, infinito, 0.0);
							if (equPCON == -1) {
								equPCON = addEquLinear_PCON(a_TSS, a_idEstagio, a_period, idCon);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON, equPCON, 1.0);
							}
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON_pat, equPCON, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, a_idPat, double()));
							if (tipo_contrato == TipoContrato_exportacao) {
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON_pat, getEquLinear_ZO(a_TSS, a_idEstagio, a_period, a_idPat), custo);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PCON(a_TSS, a_idEstagio, a_period, a_idPat, idCon, tipo_contrato), getEquLinear_PL(a_TSS, a_idEstagio, a_period, a_idPat, idSSE), -1.0);
							}
							else {
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON_pat, getEquLinear_ZO(a_TSS, a_idEstagio, a_period, a_idPat), -custo);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PCON(a_TSS, a_idEstagio, a_period, a_idPat, idCon, tipo_contrato), getEquLinear_PL(a_TSS, a_idEstagio, a_period, a_idPat, idSSE), 1.0);
							}
						}
						else
							throw std::invalid_argument("Ivalid maximum pat");

					} // if (((vlr_min != lim_inf) && (vlr_max != lim_inf)) || ((vlr_min != lim_sup) && (vlr_max != lim_sup))) {
				} // if (idPatEnd >= a_idPat) {
			} // if ((a_dados.getSize1Matriz(idCon, AttMatrizContrato_lim_inf) > 0) && (a_dados.getSize1Matriz(idCon, AttMatrizContrato_lim_sup) > 0)) {


			if (a_idPat == idPatEndPer) {

				//******************************************************
				//Formulacao: PCON >= PCON_prev + var_abs_inf
				//******************************************************

				if (a_dados.getSize1Matriz(idCon, AttMatrizContrato_var_abs_inf) > 0) {
					const double var_abs_inf = a_dados.getElementoMatriz(idCon, AttMatrizContrato_var_abs_inf, a_period, IdPatamarCarga_1, double());

					if ((vlr_min != var_abs_inf) && (vlr_max != var_abs_inf)) {
						int varPCON = getVarDecisao_PCONseExistir(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato);
						if (varPCON == -1) {
							varPCON = addVarDecisao_PCON(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato, 0.0, infinito, 0.0);
							for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= a_idPat; idPat++) {
								if (tipo_contrato == TipoContrato_exportacao) {
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON, getEquLinear_ZO(a_TSS, a_idEstagio, a_period, idPat), custo);
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PCON(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato), getEquLinear_PL(a_TSS, a_idEstagio, a_period, idPat, idSSE), -1.0);
								}
								else {
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON, getEquLinear_ZO(a_TSS, a_idEstagio, a_period, idPat), -custo);
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PCON(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato), getEquLinear_PL(a_TSS, a_idEstagio, a_period, idPat, idSSE), 1.0);
								}
							}
						}

						const int inePCON_VINF = addIneLinear_PCON_VAINF(a_TSS, a_idEstagio, a_period, idCon);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON, inePCON_VINF, 1.0);

						if ((a_idEstagio == stageIni) && (a_period == a_periodIni_stage))
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(inePCON_VINF, a_dados.getAtributo(idCon, AttComumContrato_vlr_ini, double()) + var_abs_inf);
						else {
							int varPCON_prev = getVarDecisao_PCONseExistir(a_TSS, a_idEstagio, a_periodPrev, idCon, tipo_contrato);
							if ((varPCON_prev == -1) && (a_periodPrev >= a_periodIni_stage))
								varPCON_prev = addVarDecisao_PCON(a_TSS, a_idEstagio, a_periodPrev, idCon, tipo_contrato, 0.0, infinito, 0.0);
							else if (varPCON_prev == -1) {
								varPCON_prev = addVarDecisao_PCON(a_TSS, a_idEstagio, a_periodPrev, idCon, tipo_contrato, 0.0, infinito, 0.0);
								int varPCON_prev_lastStage = getVarDecisao_PCONseExistir(a_TSS, IdEstagio(a_idEstagio - 1), a_periodPrev, idCon, tipo_contrato);
								if (varPCON_prev_lastStage == -1)
									varPCON_prev_lastStage = addVarDecisao_PCON(a_TSS, IdEstagio(a_idEstagio - 1), a_periodPrev, idCon, tipo_contrato, 0.0, infinito, 0.0);
								vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeSolverVarDecisao_PCON(a_TSS, a_idEstagio, a_periodPrev, idCon, tipo_contrato)), varPCON_prev, varPCON_prev_lastStage);
							}
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(inePCON_VINF, var_abs_inf);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON_prev, inePCON_VINF, -1.0);
						}

					} // if ((var_abs_inf > vlr_min) && (var_abs_inf < vlr_max)) {
				} // if (a_dados.getSize1Matriz(idCon, AttMatrizContrato_lim_inf) > 0) {

				//******************************************************
				//Formulacao: PCON <= PCON_lag1 + var_abs_sup
				//******************************************************

				if (a_dados.getSize1Matriz(idCon, AttMatrizContrato_var_abs_sup) > 0) {
					const double var_abs_sup = a_dados.getElementoMatriz(idCon, AttMatrizContrato_var_abs_sup, a_period, IdPatamarCarga_1, double());

					if ((vlr_min != var_abs_sup) && (vlr_max != var_abs_sup)) {
						int varPCON = getVarDecisao_PCONseExistir(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato);
						if (varPCON == -1) {
							varPCON = addVarDecisao_PCON(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato, 0.0, infinito, 0.0);
							for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= a_idPat; idPat++) {
								if (tipo_contrato == TipoContrato_exportacao) {
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON, getEquLinear_ZO(a_TSS, a_idEstagio, a_period, idPat), custo);
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PCON(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato), getEquLinear_PL(a_TSS, a_idEstagio, a_period, idPat, idSSE), -1.0);
								}
								else {
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON, getEquLinear_ZO(a_TSS, a_idEstagio, a_period, idPat), -custo);
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PCON(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato), getEquLinear_PL(a_TSS, a_idEstagio, a_period, idPat, idSSE), 1.0);
								}
							}
						}

						const int inePCON_VSUP = addIneLinear_PCON_VASUP(a_TSS, a_idEstagio, a_period, idCon);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON, inePCON_VSUP, -1.0);

						if ((a_idEstagio == stageIni) && (a_period == a_periodIni_stage))
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(inePCON_VSUP, -a_dados.getAtributo(idCon, AttComumContrato_vlr_ini, double()) - var_abs_sup);
						else {
							int varPCON_prev = getVarDecisao_PCONseExistir(a_TSS, a_idEstagio, a_periodPrev, idCon, tipo_contrato);
							if ((varPCON_prev == -1) && (a_periodPrev >= a_periodIni_stage))
								varPCON_prev = addVarDecisao_PCON(a_TSS, a_idEstagio, a_periodPrev, idCon, tipo_contrato, 0.0, infinito, 0.0);
							else if (varPCON_prev == -1) {
								varPCON_prev = addVarDecisao_PCON(a_TSS, a_idEstagio, a_periodPrev, idCon, tipo_contrato, 0.0, infinito, 0.0);
								int varPCON_prev_lastStage = getVarDecisao_PCONseExistir(a_TSS, IdEstagio(a_idEstagio - 1), a_periodPrev, idCon, tipo_contrato);
								if (varPCON_prev_lastStage == -1)
									varPCON_prev_lastStage = addVarDecisao_PCON(a_TSS, IdEstagio(a_idEstagio - 1), a_periodPrev, idCon, tipo_contrato, 0.0, infinito, 0.0);
								vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeSolverVarDecisao_PCON(a_TSS, a_idEstagio, a_periodPrev, idCon, tipo_contrato)), varPCON_prev, varPCON_prev_lastStage);
							}
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(inePCON_VSUP, -var_abs_sup);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON_prev, inePCON_VSUP, 1.0);
						}

					} // if ((var_abs_sup > vlr_min) && (var_abs_sup < vlr_max)) {
				} // if (a_dados.getSize1Matriz(idCon, AttMatrizContrato_lim_sup) > 0) {

				//************************************************************
				//Formulacao: PCON >= PCON_lag1 * var_rel_inf
				//************************************************************

				if (a_dados.getSize1Matriz(idCon, AttMatrizContrato_var_rel_inf) > 0) {
					const double var_rel_inf = a_dados.getElementoMatriz(idCon, AttMatrizContrato_var_rel_inf, a_period, IdPatamarCarga_1, double());

					if ((vlr_min != var_rel_inf) && (vlr_max != var_rel_inf)) {
						int varPCON = getVarDecisao_PCONseExistir(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato);
						if (varPCON == -1) {
							varPCON = addVarDecisao_PCON(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato, 0.0, infinito, 0.0);
							for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= a_idPat; idPat++) {
								if (tipo_contrato == TipoContrato_exportacao) {
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON, getEquLinear_ZO(a_TSS, a_idEstagio, a_period, idPat), custo);
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PCON(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato), getEquLinear_PL(a_TSS, a_idEstagio, a_period, idPat, idSSE), -1.0);
								}
								else {
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON, getEquLinear_ZO(a_TSS, a_idEstagio, a_period, idPat), -custo);
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PCON(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato), getEquLinear_PL(a_TSS, a_idEstagio, a_period, idPat, idSSE), 1.0);
								}
							}
						}

						const int inePCON_VINF = addIneLinear_PCON_VRINF(a_TSS, a_idEstagio, a_period, idCon);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON, inePCON_VINF, 1.0);

						if ((a_idEstagio == stageIni) && (a_period == a_periodIni_stage))
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(inePCON_VINF, a_dados.getAtributo(idCon, AttComumContrato_vlr_ini, double()) * var_rel_inf);
						else {
							int varPCON_prev = getVarDecisao_PCONseExistir(a_TSS, a_idEstagio, a_periodPrev, idCon, tipo_contrato);
							if ((varPCON_prev == -1) && (a_periodPrev >= a_periodIni_stage))
								varPCON_prev = addVarDecisao_PCON(a_TSS, a_idEstagio, a_periodPrev, idCon, tipo_contrato, 0.0, infinito, 0.0);
							else if (varPCON_prev == -1) {
								varPCON_prev = addVarDecisao_PCON(a_TSS, a_idEstagio, a_periodPrev, idCon, tipo_contrato, 0.0, infinito, 0.0);
								int varPCON_prev_lastStage = getVarDecisao_PCONseExistir(a_TSS, IdEstagio(a_idEstagio - 1), a_periodPrev, idCon, tipo_contrato);
								if (varPCON_prev_lastStage == -1)
									varPCON_prev_lastStage = addVarDecisao_PCON(a_TSS, IdEstagio(a_idEstagio - 1), a_periodPrev, idCon, tipo_contrato, 0.0, infinito, 0.0);
								vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeSolverVarDecisao_PCON(a_TSS, a_idEstagio, a_periodPrev, idCon, tipo_contrato)), varPCON_prev, varPCON_prev_lastStage);
							}
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON_prev, inePCON_VINF, -var_rel_inf);
						}

					} // if ((var_rel_inf > vlr_min) && (var_rel_inf < vlr_max)) {
				} // if (a_dados.getSize1Matriz(idCon, AttMatrizContrato_lim_inf) > 0) {

				//************************************************************
				//Formulacao: PCON <= PCON_lag1 * var_rel_sup
				//************************************************************

				if (a_dados.getSize1Matriz(idCon, AttMatrizContrato_var_rel_sup) > 0) {
					const double var_rel_sup = a_dados.getElementoMatriz(idCon, AttMatrizContrato_var_rel_sup, a_period, IdPatamarCarga_1, double());

					if ((vlr_min != var_rel_sup) && (vlr_max != var_rel_sup)) {
						int varPCON = getVarDecisao_PCONseExistir(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato);
						if (varPCON == -1) {
							varPCON = addVarDecisao_PCON(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato, 0.0, infinito, 0.0);
							for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= a_idPat; idPat++) {
								if (tipo_contrato == TipoContrato_exportacao) {
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON, getEquLinear_ZO(a_TSS, a_idEstagio, a_period, idPat), custo);
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PCON(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato), getEquLinear_PL(a_TSS, a_idEstagio, a_period, idPat, idSSE), -1.0);
								}
								else {
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON, getEquLinear_ZO(a_TSS, a_idEstagio, a_period, idPat), -custo);
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PCON(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato), getEquLinear_PL(a_TSS, a_idEstagio, a_period, idPat, idSSE), 1.0);
								}
							}
						}

						const int inePCON_VSUP = addIneLinear_PCON_VRSUP(a_TSS, a_idEstagio, a_period, idCon);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON, inePCON_VSUP, -1.0);

						if ((a_idEstagio == stageIni) && (a_period == a_periodIni_stage))
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(inePCON_VSUP, -a_dados.getAtributo(idCon, AttComumContrato_vlr_ini, double()) * var_rel_sup);
						else {
							int varPCON_prev = getVarDecisao_PCONseExistir(a_TSS, a_idEstagio, a_periodPrev, idCon, tipo_contrato);
							if ((varPCON_prev == -1) && (a_periodPrev >= a_periodIni_stage))
								varPCON_prev = addVarDecisao_PCON(a_TSS, a_idEstagio, a_periodPrev, idCon, tipo_contrato, 0.0, infinito, 0.0);
							else if (varPCON_prev == -1) {
								varPCON_prev = addVarDecisao_PCON(a_TSS, a_idEstagio, a_periodPrev, idCon, tipo_contrato, 0.0, infinito, 0.0);
								int varPCON_prev_lastStage = getVarDecisao_PCONseExistir(a_TSS, IdEstagio(a_idEstagio - 1), a_periodPrev, idCon, tipo_contrato);
								if (varPCON_prev_lastStage == -1)
									varPCON_prev_lastStage = addVarDecisao_PCON(a_TSS, IdEstagio(a_idEstagio - 1), a_periodPrev, idCon, tipo_contrato, 0.0, infinito, 0.0);
								vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeSolverVarDecisao_PCON(a_TSS, a_idEstagio, a_periodPrev, idCon, tipo_contrato)), varPCON_prev, varPCON_prev_lastStage);
							}
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON_prev, inePCON_VSUP, var_rel_sup);
						}

					} // if ((var_rel_sup > vlr_min) && (var_rel_sup < vlr_max)) {
				} // if (a_dados.getSize1Matriz(idCon, AttMatrizContrato_lim_sup) > 0) {

			} // if (a_idPat == idPatEndPer) {

			// Variável PCON media
			int varPCON_pat = getVarDecisao_PCONseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idCon, tipo_contrato);
			int varPCON = getVarDecisao_PCONseExistir(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato);
			if ((varPCON_pat > -1) && (idPatEndPer > IdPatamarCarga_1)) {

				if (varPCON == -1)
					varPCON = addVarDecisao_PCON(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato, 0.0, infinito, 0.0);

				int equPCON = getEquLinear_PCONseExistir(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato);
				if (equPCON == -1) {
					equPCON = addEquLinear_PCON(a_TSS, a_idEstagio, a_period, idCon, tipo_contrato);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON, equPCON, 1.0);
				}

				// Variável PCON
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON_pat, equPCON, -perc_dur_pat);

			} // if ((varPCON_pat > -1) && (idPatEndPer > IdPatamarCarga_1)) {


			int varPCON_sse = getVarDecisao_PCONseExistir(a_TSS, a_idEstagio, a_period, idSSE);
			int equPCON_sse = getEquLinear_PCONseExistir(a_TSS, a_idEstagio, a_period, idSSE);

			if (varPCON_sse == -1) {
				varPCON_sse = addVarDecisao_PCON(a_TSS, a_idEstagio, a_period, idSSE, -infinito, infinito, 0.0);
				equPCON_sse = addEquLinear_PCON(a_TSS, a_idEstagio, a_period, idSSE);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON_sse, equPCON_sse, 1.0);
			} // if (varPCON_sse == -1) {

			if (tipo_contrato == TipoContrato_importacao) {
				if (varPCON > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON, equPCON_sse, -1.0);
				else if (varPCON_pat > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON_pat, equPCON_sse, -1.0);
			}
			else {
				if (varPCON > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON, equPCON_sse, 1.0);
				else if (varPCON_pat > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON_pat, equPCON_sse, 1.0);
			}

		}// for (IdContrato idCon = idConIni; idCon < idConOut; a_dados.incr(idCon)) {



	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisContratos(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPat) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}


std::string ModeloOtimizacao::criarRestricoesEletricas_nomeVarEstado(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodPrev, Periodo& a_period, const IdRestricaoEletrica a_idRE, const bool a_fp_local){

	try {

		std::string nome = getNomeSolverVarDecisao_RE(a_TSS, a_idEstagio, a_periodPrev, a_idRE);

		const IdElementoSistema idElemEnd = a_dados.getMaiorId(a_idRE, IdElementoSistema());
		for (IdElementoSistema idElem = IdElementoSistema_1; idElem <= idElemEnd; idElem++) {
			const TipoVariavelRestricaoOperativa tipoVarRE = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

			if ((tipoVarRE != TipoVariavelRestricaoOperativa_potencia) && (tipoVarRE != TipoVariavelRestricaoOperativa_potencia_disponivel))
				throw std::invalid_argument("Tipo invalido");

			double fator_participacao = 0.0;
			if (a_fp_local)
				fator_participacao = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_fator_participacao, double());
			else
				fator_participacao = a_dados.getElementoMatriz(a_idRE, idElem, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double());

			nome += ",FP," + getString(fator_participacao) + "," + getString(tipoVarRE);

			const IdHidreletrica       idUHE = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
			const IdConjuntoHidraulico idConUHE = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_conjuntohidraulico, IdConjuntoHidraulico());
			const IdUnidadeUHE         idUniUHE = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_unidadehidraulica, IdUnidadeUHE());
			const IdTermeletrica       idUTE = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_termeletrica, IdTermeletrica());
			const IdUnidadeUTE         idUniUTE = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_unidadeTermeletrica, IdUnidadeUTE());
			const IdIntercambio        idINT = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_intercambio, IdIntercambio());
			const IdContrato           idCON = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_contrato, IdContrato());
			const IdDemandaEspecial    idDES = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_demandaEspecial, IdDemandaEspecial());
			const IdRenovavel          idREN = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_renovavel, IdRenovavel());
			const IdUsinaElevatoria    idELE = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());
			const IdSubmercado         idDEM = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_demanda, IdSubmercado());
			const IdSubmercado         idDEMLIQ = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_demandaliquida, IdSubmercado());

			a_dados.vetorRestricaoEletrica.at(a_idRE).vetorElementoSistema.at(idElem).validar();

			// Hidreletrica
			if ((idUHE != IdHidreletrica_Nenhum) && (idConUHE == IdConjuntoHidraulico_Nenhum) && (idUniUHE == IdUnidadeUHE_Nenhum))
				nome += "," + getFullString(idUHE);

			// ConjuntoHidraulico
			else if ((idUHE != IdHidreletrica_Nenhum) && (idConUHE != IdConjuntoHidraulico_Nenhum) && (idUniUHE == IdUnidadeUHE_Nenhum))
				nome += "," + getFullString(idUHE) + "," + getString(idConUHE);

			// UnidadeUHE
			else if ((idUHE != IdHidreletrica_Nenhum) && (idConUHE != IdConjuntoHidraulico_Nenhum) && (idUniUHE != IdUnidadeUHE_Nenhum))
				nome += "," + getFullString(idUHE) + "," + getString(idConUHE) + "," + getString(idUniUHE);


			// Termeletrica
			else if ((idUTE != IdTermeletrica_Nenhum) && (idUniUTE == IdUnidadeUTE_Nenhum))
				nome += "," + getFullString(idUTE);

			// Unidade UTE Termeletrica
			else if ((idUTE != IdTermeletrica_Nenhum) && (idUniUTE != IdUnidadeUTE_Nenhum))
				nome += "," + getFullString(idUTE) + "," + getString(idUniUTE);

			// Intercambio
			else if (idINT != IdIntercambio_Nenhum){
				const IdSubmercado idSSE_orig = a_dados.getAtributo(idINT, AttComumIntercambio_submercado_origem, IdSubmercado());
				const IdSubmercado idSSE_dest = a_dados.getAtributo(idINT, AttComumIntercambio_submercado_destino, IdSubmercado());
				nome += "," + getFullString(idSSE_orig) + "," + getString(idSSE_dest);
			}

			// Contrato
			else if (idCON != IdContrato_Nenhum){
				const TipoContrato tipo_contrato = a_dados.getAtributo(idCON, AttComumContrato_tipo_contrato, TipoContrato());
				const std::string nome_contrato = a_dados.getAtributo(idCON, AttComumContrato_nome, std::string());
				const IdSubmercado submercado = a_dados.getAtributo(idCON, AttComumContrato_submercado, IdSubmercado());
				nome += "," + getFullString(idCON) + "," + nome_contrato + "," + getString(submercado) + "," + getString(tipo_contrato);
			}

			// Demanda Especial
			else if (idDES != IdDemandaEspecial_Nenhum){
				const std::string nome_de = a_dados.getAtributo(idDES, AttComumDemandaEspecial_nome, std::string());
				nome += "," + getFullString(idDES) + "," + nome_de;
			}

			// Demanda
			else if (idDEM != IdSubmercado_Nenhum) {
				nome += "," + getFullString(idDEM);
			}

			// Demanda Liquida
			else if (idDEMLIQ != IdSubmercado_Nenhum) {
				nome += "," + getFullString(idDEMLIQ) + ",liquida";
			}

			// Renovavel
			else if (idREN != IdRenovavel_Nenhum){
				const std::string tipo_usina = a_dados.getAtributo(idREN, AttComumRenovavel_tipo_usina, std::string());
				const std::string nome_renovavel = a_dados.getAtributo(idREN, AttComumRenovavel_nome, std::string());
				const IdSubmercado submercado = a_dados.getAtributo(idREN, AttComumRenovavel_submercado, IdSubmercado());
				nome += "," + getFullString(idREN) + "," + nome_renovavel + "," + getString(submercado) + "," + getString(tipo_usina);
			}

			// Elevatoria
			else if (idELE != IdUsinaElevatoria_Nenhum) {
				const IdHidreletrica idUHE_orig = a_dados.getAtributo(idELE, AttComumUsinaElevatoria_usina_origem_bombeamento, IdHidreletrica());
				const IdHidreletrica idUHE_dest = a_dados.getAtributo(idELE, AttComumUsinaElevatoria_usina_destino_bombeamento, IdHidreletrica());
				nome += "," + getFullString(idELE) + "," + getString(idUHE_orig) + "," + getString(idUHE_dest);
			}
			else
				throw std::invalid_argument("Combinacao invalida para RE em " + getFullString(idElem));

		} // for (IdElementoSistema idElem = IdElementoSistema_1; idElem <= idElemEnd; idElem++) {

		return nome;

	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesEletricas_nomeVarEstado(" + getFullString(a_TSS) + ",Dados& a_dados, " + getFullString(a_idEstagio) + "," + getFullString(a_periodPrev) + "," + getFullString(a_period) + "," + getFullString(a_idRE) + "): \n" + std::string(erro.what())); }

}

void ModeloOtimizacao::criarRestricoesEletricas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodIni_stage, Periodo& a_periodPrev, Periodo& a_period, Periodo& a_periodNext, const IdPatamarCarga a_idPat) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const IdEstagio stageIni = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());

		const IdPatamarCarga idPatEndPer = a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, a_period, IdPatamarCarga());

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const double vlr_min = getdoubleFromChar("min");
		const double vlr_max = getdoubleFromChar("max");

		const IdRestricaoEletrica idREIni = a_dados.getMenorId(IdRestricaoEletrica());
		const IdRestricaoEletrica idREOut = a_dados.getIdOut(IdRestricaoEletrica());

		for (IdRestricaoEletrica idRE = idREIni; idRE < idREOut; a_dados.incr(idRE)) {

			const double penalidade = a_dados.getAtributo(idRE, AttComumRestricaoEletrica_penalidade, double());

			///////////////////////////////////////////////////////////////////
			//Restrições Hidráulicas (RE)
			///////////////////////////////////////////////////////////////////

			//******************************************
			//Formulacao: RE + RE_FINF >= lim_inf
			//******************************************

			if (a_dados.getSize1Matriz(idRE, AttMatrizRestricaoEletrica_lim_inf) > 0) {
				const IdPatamarCarga idPatEnd = a_dados.getIterador2Final(idRE, AttMatrizRestricaoEletrica_lim_inf, a_period, IdPatamarCarga());

				if (idPatEnd >= a_idPat) {

					const double lim_inf = a_dados.getElementoMatriz(idRE, AttMatrizRestricaoEletrica_lim_inf, a_period, a_idPat, double());

					if ((vlr_min != lim_inf) && (vlr_max != lim_inf)){

						const IdPatamarCarga idPatEnd = a_dados.getIterador2Final(idRE, AttMatrizRestricaoEletrica_lim_inf, a_period, IdPatamarCarga());

						if (idPatEnd == IdPatamarCarga_1) {
							const int varRE = criarRestricoesEletricas(a_TSS, a_dados, a_idEstagio, a_period, a_periodNext, IdPatamarCarga_Nenhum, idRE);
							if (getIneLinear_RE_LINFseExistir(a_TSS, a_idEstagio, a_period, idRE) == -1) {
								const int ineRE_LINF = addIneLinear_RE_LINF(a_TSS, a_idEstagio, a_period, idRE);
								const int varRE_FINF = addVarDecisao_RE_LINF_FINF(a_TSS, a_idEstagio, a_period, idRE, 0.0, infinito, 0.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE_FINF, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -penalidade);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineRE_LINF, lim_inf);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE, ineRE_LINF, 1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE_FINF, ineRE_LINF, 1.0);
							} // if (getIneLinear_RE_LINFseExistir(a_TSS, a_idEstagio, a_period, idRE) == -1) {
						}
						else if (idPatEnd == idPatEndPer) {
							const int varRE_pat = criarRestricoesEletricas(a_TSS, a_dados, a_idEstagio, a_period, a_periodNext, a_idPat, idRE);
							if (getIneLinear_RE_LINFseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idRE) == -1) {
								const int ineRE_LINF = addIneLinear_RE_LINF(a_TSS, a_idEstagio, a_period, a_idPat, idRE);
								const int varRE_FINF = addVarDecisao_RE_LINF_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idRE, 0.0, infinito, 0.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE_FINF, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -penalidade);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineRE_LINF, lim_inf);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE_pat, ineRE_LINF, 1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE_FINF, ineRE_LINF, 1.0);
							} // if (getIneLinear_RE_LINFseExistir(a_TSS, a_idEstagio, a_period, idRE) == -1) {
							if ((getVarDecisao_REseExistir(a_TSS, a_idEstagio, a_period, idRE) == -1) && (idPatEnd == a_idPat)) {
								const int varRE = addVarDecisao_RE(a_TSS, a_idEstagio, a_period, idRE, -infinito, infinito, 0.0);
								const int equRE = addEquLinear_RE(a_TSS, a_idEstagio, a_period, idRE);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE, equRE, 1.0);
								for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEnd; idPat++)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RE(a_TSS, a_idEstagio, a_period, idPat, idRE), equRE, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
							}
						}
						else
							throw std::invalid_argument("Ivalid maximum pat");

					} // if ((vlr_min != lim_inf) && (vlr_max != lim_inf)){
				} // if (idPatEnd >= a_idPat) {
			} // if (a_dados.getSize1Matriz(idRE, AttMatrizRestricaoEletrica_lim_inf) > 0) {

			//******************************************
			//Formulacao: RE - RE_FSUP <= lim_sup
			//******************************************

			if (a_dados.getSize1Matriz(idRE, AttMatrizRestricaoEletrica_lim_sup) > 0) {
				const IdPatamarCarga idPatEnd = a_dados.getIterador2Final(idRE, AttMatrizRestricaoEletrica_lim_sup, a_period, IdPatamarCarga());

				if (idPatEnd >= a_idPat) {

					const double lim_sup = a_dados.getElementoMatriz(idRE, AttMatrizRestricaoEletrica_lim_sup, a_period, a_idPat, double());

					if ((vlr_min != lim_sup) && (vlr_max != lim_sup)) {

						if (idPatEnd == IdPatamarCarga_1) {
							const int varRE = criarRestricoesEletricas(a_TSS, a_dados, a_idEstagio, a_period, a_periodNext, IdPatamarCarga_Nenhum, idRE);
							if (getIneLinear_RE_LSUPseExistir(a_TSS, a_idEstagio, a_period, idRE) == -1) {
								const int ineRE_LSUP = addIneLinear_RE_LSUP(a_TSS, a_idEstagio, a_period, idRE);
								const int varRE_FSUP = addVarDecisao_RE_LSUP_FSUP(a_TSS, a_idEstagio, a_period, idRE, 0.0, infinito, 0.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE_FSUP, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -penalidade);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineRE_LSUP, -lim_sup);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE, ineRE_LSUP, -1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE_FSUP, ineRE_LSUP, 1.0);
							} // if (getIneLinear_RE_LSUPseExistir(a_TSS, a_idEstagio, a_period, idRE) == -1) {
						}
						else if (idPatEnd == idPatEndPer) {
							const int varRE_pat = criarRestricoesEletricas(a_TSS, a_dados, a_idEstagio, a_period, a_periodNext, a_idPat, idRE);
							if (getIneLinear_RE_LSUPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idRE) == -1) {
								const int ineRE_LSUP = addIneLinear_RE_LSUP(a_TSS, a_idEstagio, a_period, a_idPat, idRE);
								const int varRE_FSUP = addVarDecisao_RE_LSUP_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idRE, 0.0, infinito, 0.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE_FSUP, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -penalidade);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineRE_LSUP, -lim_sup);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE_pat, ineRE_LSUP, -1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE_FSUP, ineRE_LSUP, 1.0);
							} // if (getIneLinear_RE_LSUPseExistir(a_TSS, a_idEstagio, a_period, idRE) == -1) {
							if ((getVarDecisao_REseExistir(a_TSS, a_idEstagio, a_period, idRE) == -1) && (idPatEnd == a_idPat)) {
								const int varRE = addVarDecisao_RE(a_TSS, a_idEstagio, a_period, idRE, -infinito, infinito, 0.0);
								const int equRE = addEquLinear_RE(a_TSS, a_idEstagio, a_period, idRE);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE, equRE, 1.0);
								for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEnd; idPat++)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RE(a_TSS, a_idEstagio, a_period, idPat, idRE), equRE, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
							}
						}
						else
							throw std::invalid_argument("Ivalid maximum pat");

					} // if ((vlr_min != lim_inf) && (vlr_max != lim_inf)){
				} // if (idPatEnd >= a_idPat) {
			} // if (a_dados.getSize1Matriz(idRE, AttMatrizRestricaoEletrica_lim_sup) > 0) {

			if (a_idPat == idPatEndPer) {

				//******************************************************
				//Formulacao: RE + RE_FINF >= RE_lag1 + var_abs_inf
				//******************************************************

				if (a_dados.getSize1Matriz(idRE, AttMatrizRestricaoEletrica_var_abs_inf) > 0) {
					const double var_abs_inf = a_dados.getElementoMatriz(idRE, AttMatrizRestricaoEletrica_var_abs_inf, a_period, IdPatamarCarga_1, double());

					if ((vlr_min != var_abs_inf) && (vlr_max != var_abs_inf)) {

						const int varRE = criarRestricoesEletricas(a_TSS, a_dados, a_idEstagio, a_period, a_periodNext, IdPatamarCarga_Nenhum, idRE);
						if (getIneLinear_RE_VAINFseExistir(a_TSS, a_idEstagio, a_period, idRE) == -1) {
							const int ineRE_VINF = addIneLinear_RE_VAINF(a_TSS, a_idEstagio, a_period, idRE);
							const int varRE_FINF = addVarDecisao_RE_VAINF_FINF(a_TSS, a_idEstagio, a_period, idRE, 0.0, infinito, 0.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE_FINF, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -penalidade);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE, ineRE_VINF, 1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE_FINF, ineRE_VINF, 1.0);

							if ((a_idEstagio == stageIni) && (a_period == a_periodIni_stage))
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineRE_VINF, a_dados.getAtributo(idRE, AttComumRestricaoEletrica_vlr_ini, double()) + var_abs_inf);
							else {
								int varRE_prev = getVarDecisao_REseExistir(a_TSS, a_idEstagio, a_periodPrev, idRE);
								if ((varRE_prev == -1) && (a_periodPrev >= a_periodIni_stage))
									varRE_prev = criarRestricoesEletricas(a_TSS, a_dados, a_idEstagio, a_periodPrev, a_period, IdPatamarCarga_Nenhum, idRE);
								else if (varRE_prev == -1) {
									varRE_prev = addVarDecisao_RE(a_TSS, a_idEstagio, a_periodPrev, idRE, 0.0, infinito, 0.0);
									vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, criarRestricoesEletricas_nomeVarEstado(a_TSS, a_dados, a_idEstagio, a_periodPrev, a_period, idRE), varRE_prev, criarRestricoesEletricas(a_TSS, a_dados, IdEstagio(a_idEstagio - 1), a_periodPrev, a_period, IdPatamarCarga_Nenhum, idRE));
								}
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineRE_VINF, var_abs_inf);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE_prev, ineRE_VINF, -1.0);
							}
						} // if (getIneLinear_RE_VINFseExistir(a_TSS, a_idEstagio, a_period, idRE) == -1) {

					} // if ((vlr_min != var_abs_inf) && (vlr_max != var_abs_inf)) {
				} // if (a_dados.getSize1Matriz(idRE, AttMatrizRestricaoEletrica_lim_inf) > 0) {

				//******************************************************
				//Formulacao: RE + RE_FSUP <= RE_lag1 + var_abs_sup
				//******************************************************

				if (a_dados.getSize1Matriz(idRE, AttMatrizRestricaoEletrica_var_abs_sup) > 0) {
					const double var_abs_sup = a_dados.getElementoMatriz(idRE, AttMatrizRestricaoEletrica_var_abs_sup, a_period, IdPatamarCarga_1, double());

					if ((vlr_min != var_abs_sup) && (vlr_max != var_abs_sup)) {

						const int varRE = criarRestricoesEletricas(a_TSS, a_dados, a_idEstagio, a_period, a_periodNext, IdPatamarCarga_Nenhum, idRE);
						if (getIneLinear_RE_VASUPseExistir(a_TSS, a_idEstagio, a_period, idRE) == -1) {
							const int ineRE_VSUP = addIneLinear_RE_VASUP(a_TSS, a_idEstagio, a_period, idRE);
							const int varRE_FSUP = addVarDecisao_RE_VASUP_FSUP(a_TSS, a_idEstagio, a_period, idRE, 0.0, infinito, 0.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE_FSUP, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -penalidade);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE, ineRE_VSUP, -1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE_FSUP, ineRE_VSUP, 1.0);

							if ((a_idEstagio == stageIni) && (a_period == a_periodIni_stage))
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineRE_VSUP, -a_dados.getAtributo(idRE, AttComumRestricaoEletrica_vlr_ini, double()) - var_abs_sup);
							else {
								int varRE_prev = getVarDecisao_REseExistir(a_TSS, a_idEstagio, a_periodPrev, idRE);
								if ((varRE_prev == -1) && (a_periodPrev >= a_periodIni_stage))
									varRE_prev = criarRestricoesEletricas(a_TSS, a_dados, a_idEstagio, a_periodPrev, a_period, IdPatamarCarga_Nenhum, idRE);
								else if (varRE_prev == -1) {
									varRE_prev = addVarDecisao_RE(a_TSS, a_idEstagio, a_periodPrev, idRE, 0.0, infinito, 0.0);
									vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, criarRestricoesEletricas_nomeVarEstado(a_TSS, a_dados, a_idEstagio, a_periodPrev, a_period, idRE), varRE_prev, criarRestricoesEletricas(a_TSS, a_dados, IdEstagio(a_idEstagio - 1), a_periodPrev, a_period, IdPatamarCarga_Nenhum, idRE));
								}
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineRE_VSUP, -var_abs_sup);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE_prev, ineRE_VSUP, 1.0);
							}
						} // if (getIneLinear_RE_VSUPseExistir(a_TSS, a_idEstagio, a_period, idRE) == -1) {

					} // if ((vlr_min != var_abs_sup) && (vlr_max != var_abs_sup)) {
				} // if (a_dados.getSize1Matriz(idRE, AttMatrizRestricaoEletrica_lim_sup) > 0) {

				//************************************************************
				//Formulacao: RE + RE_FINF >= RE_lag1 * var_rel_inf
				//************************************************************

				if (a_dados.getSize1Matriz(idRE, AttMatrizRestricaoEletrica_var_rel_inf) > 0) {
					const double var_rel_inf = a_dados.getElementoMatriz(idRE, AttMatrizRestricaoEletrica_var_rel_inf, a_period, IdPatamarCarga_1, double());

					if (vlr_max != var_rel_inf) {

						const int varRE = criarRestricoesEletricas(a_TSS, a_dados, a_idEstagio, a_period, a_periodNext, IdPatamarCarga_Nenhum, idRE);
						if (getIneLinear_RE_VRINFseExistir(a_TSS, a_idEstagio, a_period, idRE) == -1) {
							const int ineRE_VINF = addIneLinear_RE_VRINF(a_TSS, a_idEstagio, a_period, idRE);
							const int varRE_FINF = addVarDecisao_RE_VRINF_FINF(a_TSS, a_idEstagio, a_period, idRE, 0.0, infinito, 0.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE_FINF, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -penalidade);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE, ineRE_VINF, 1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE_FINF, ineRE_VINF, 1.0);

							if ((a_idEstagio == stageIni) && (a_period == a_periodIni_stage))
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineRE_VINF, a_dados.getAtributo(idRE, AttComumRestricaoEletrica_vlr_ini, double()) * var_rel_inf);
							else {
								int varRE_prev = getVarDecisao_REseExistir(a_TSS, a_idEstagio, a_periodPrev, idRE);
								if ((varRE_prev == -1) && (a_periodPrev >= a_periodIni_stage))
									varRE_prev = criarRestricoesEletricas(a_TSS, a_dados, a_idEstagio, a_periodPrev, a_period, IdPatamarCarga_Nenhum, idRE);
								else if (varRE_prev == -1) {
									varRE_prev = addVarDecisao_RE(a_TSS, a_idEstagio, a_periodPrev, idRE, 0.0, infinito, 0.0);
									vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, criarRestricoesEletricas_nomeVarEstado(a_TSS, a_dados, a_idEstagio, a_periodPrev, a_period, idRE), varRE_prev, criarRestricoesEletricas(a_TSS, a_dados, IdEstagio(a_idEstagio - 1), a_periodPrev, a_period, IdPatamarCarga_Nenhum, idRE));
								}
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE_prev, ineRE_VINF, -var_rel_inf);
							}
						} // if (getIneLinear_RE_VINFseExistir(a_TSS, a_idEstagio, a_period, idRE) == -1) {

					} // if (vlr_max != var_rel_inf) {
				} // if (a_dados.getSize1Matriz(idRE, AttMatrizRestricaoEletrica_lim_inf) > 0) {

				//************************************************************
				//Formulacao: RE + RE_FSUP <= RE_lag1 * var_rel_sup
				//************************************************************

				if (a_dados.getSize1Matriz(idRE, AttMatrizRestricaoEletrica_var_rel_sup) > 0) {
					const double var_rel_sup = a_dados.getElementoMatriz(idRE, AttMatrizRestricaoEletrica_var_rel_sup, a_period, IdPatamarCarga_1, double());

					if ((vlr_min != var_rel_sup) && (vlr_max != var_rel_sup)) {

						const int varRE = criarRestricoesEletricas(a_TSS, a_dados, a_idEstagio, a_period, a_periodNext, IdPatamarCarga_Nenhum, idRE);
						if (getIneLinear_RE_VRSUPseExistir(a_TSS, a_idEstagio, a_period, idRE) == -1) {
							const int ineRE_VSUP = addIneLinear_RE_VRSUP(a_TSS, a_idEstagio, a_period, idRE);
							const int varRE_FSUP = addVarDecisao_RE_VRSUP_FSUP(a_TSS, a_idEstagio, a_period, idRE, 0.0, infinito, 0.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE_FSUP, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -penalidade);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE, ineRE_VSUP, -1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE_FSUP, ineRE_VSUP, 1.0);

							if ((a_idEstagio == stageIni) && (a_period == a_periodIni_stage))
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineRE_VSUP, -a_dados.getAtributo(idRE, AttComumRestricaoEletrica_vlr_ini, double()) * var_rel_sup);
							else {
								int varRE_prev = getVarDecisao_REseExistir(a_TSS, a_idEstagio, a_periodPrev, idRE);
								if ((varRE_prev == -1) && (a_periodPrev >= a_periodIni_stage))
									varRE_prev = criarRestricoesEletricas(a_TSS, a_dados, a_idEstagio, a_periodPrev, a_period, IdPatamarCarga_Nenhum, idRE);
								else if (varRE_prev == -1) {
									varRE_prev = addVarDecisao_RE(a_TSS, a_idEstagio, a_periodPrev, idRE, 0.0, infinito, 0.0);
									vetorEstagio.at(a_idEstagio).addVariavelEstado(a_TSS, criarRestricoesEletricas_nomeVarEstado(a_TSS, a_dados, a_idEstagio, a_periodPrev, a_period, idRE), varRE_prev, criarRestricoesEletricas(a_TSS, a_dados, IdEstagio(a_idEstagio - 1), a_periodPrev, a_period, IdPatamarCarga_Nenhum, idRE));
								}
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE_prev, ineRE_VSUP, var_rel_sup);
							}
						} // if (getIneLinear_RE_VSUPseExistir(a_TSS, a_idEstagio, a_period, idRE) == -1) {

					} // if ((vlr_min != var_rel_sup) && (vlr_max != var_rel_sup)) {
				} // if (a_dados.getSize1Matriz(idRE, AttMatrizRestricaoEletrica_lim_sup) > 0) {

			} // if (a_idPat == idPatEndPer) {

		}// for (IdRestricaoEletrica idRE = idREIni; idRE < idREOut; a_dados.incr(idRE)) {

	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesEletricas(" + getFullString(a_TSS) + ",Dados& a_dados, " + getFullString(a_idEstagio) + "," + getFullString(a_periodIni_stage) + "," + getFullString(a_periodPrev) + "," + getFullString(a_period) + "," + getFullString(a_periodNext) + "," + getFullString(a_idPat) + "): \n" + std::string(erro.what())); }

}//void ModeloOtimizacao::criarRestricoesEletricas(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados)

int ModeloOtimizacao::criarRestricoesEletricas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, Periodo& a_periodNext, const IdPatamarCarga a_idPat, const IdRestricaoEletrica a_idRE) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return -1;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return -1;

		int equRE = -1;
		int varRE = -1;

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const IdPatamarCarga idPatEndPer = a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, a_period, IdPatamarCarga());

		if (a_idPat != IdPatamarCarga_Nenhum) {
			equRE = getEquLinear_REseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idRE);
			if (equRE > -1) { return getVarDecisao_RE(a_TSS, a_idEstagio, a_period, a_idPat, a_idRE); }
			equRE = addEquLinear_RE(a_TSS, a_idEstagio, a_period, a_idPat, a_idRE);
			varRE = addVarDecisao_RE(a_TSS, a_idEstagio, a_period, a_idPat, a_idRE, -infinito, infinito, 0.0);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE, equRE, 1.0);
		}
		else {
			equRE = getEquLinear_REseExistir(a_TSS, a_idEstagio, a_period, a_idRE);
			if (equRE > -1) { return getVarDecisao_RE(a_TSS, a_idEstagio, a_period, a_idRE); }
			equRE = addEquLinear_RE(a_TSS, a_idEstagio, a_period, a_idRE);
			varRE = addVarDecisao_RE(a_TSS, a_idEstagio, a_period, a_idRE, -infinito, infinito, 0.0);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRE, equRE, 1.0);
		}

		const IdElementoSistema idElemEnd = a_dados.getMaiorId(a_idRE, IdElementoSistema());
		for (IdElementoSistema idElem = IdElementoSistema_1; idElem <= idElemEnd; idElem++) {
			const TipoVariavelRestricaoOperativa tipoVarRE = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

			if ((tipoVarRE != TipoVariavelRestricaoOperativa_potencia) && (tipoVarRE != TipoVariavelRestricaoOperativa_potencia_disponivel))
				throw std::invalid_argument("Tipo invalido");

			double fator_participacao = a_dados.getElementoMatriz(a_idRE, idElem, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double());
			if (a_idPat != IdPatamarCarga_Nenhum)
				fator_participacao = a_dados.getElementoMatriz(a_idRE, idElem, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double());

			const IdHidreletrica       idUHE     = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
			const IdConjuntoHidraulico idConUHE  = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_conjuntohidraulico, IdConjuntoHidraulico());
			const IdUnidadeUHE         idUniUHE = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_unidadehidraulica, IdUnidadeUHE());
			const IdTermeletrica       idUTE = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_termeletrica, IdTermeletrica());
			const IdUnidadeUTE         idUniUTE = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_unidadeTermeletrica, IdUnidadeUTE());
			const IdIntercambio        idINT = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_intercambio, IdIntercambio());
			const IdContrato           idCON = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_contrato, IdContrato());
			const IdDemandaEspecial    idDES = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_demandaEspecial, IdDemandaEspecial());
			const IdRenovavel          idREN = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_renovavel, IdRenovavel());
			const IdUsinaElevatoria    idELE = a_dados.getAtributo(a_idRE, idElem, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());


			// Hidreletrica
			if ((idUHE != IdHidreletrica_Nenhum) && (idConUHE == IdConjuntoHidraulico_Nenhum) && (idUniUHE == IdUnidadeUHE_Nenhum) && (idUTE == IdTermeletrica_Nenhum) && (idUniUTE == IdUnidadeUTE_Nenhum) && (idINT == IdIntercambio_Nenhum) && (idCON == IdContrato_Nenhum) && (idDES == IdDemandaEspecial_Nenhum) && (idREN == IdRenovavel_Nenhum) && (idELE == IdUsinaElevatoria_Nenhum)) {
				if (tipoVarRE == TipoVariavelRestricaoOperativa_potencia_disponivel) {
					if (a_idPat == IdPatamarCarga_Nenhum) {
						if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, idUHE), equRE, -fator_participacao);
						else if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE) > -1) {
							if (idPatEndPer == IdPatamarCarga_1)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE), equRE, -fator_participacao);
							else {
								const int varPHDISP = addVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);
								const int equPHDISP = addEquLinear_PHDISP(a_TSS, a_idEstagio, a_period, idUHE);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP, equPHDISP, 1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP, equRE, -fator_participacao);
								for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, idPat, idUHE), equPHDISP, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
							}
						}
					}
					else if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), equRE, -fator_participacao);
				}
				if (tipoVarRE == TipoVariavelRestricaoOperativa_potencia) {
					if (a_idPat == IdPatamarCarga_Nenhum) {
						if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, idUHE) > -1)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, idUHE), equRE, -fator_participacao);
						else if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE) > -1) {
							if (idPatEndPer == IdPatamarCarga_1)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE), equRE, -fator_participacao);
							else {
								const int varPH = addVarDecisao_PH(a_TSS, a_idEstagio, a_period, idUHE, 0.0, infinito, 0.0);
								const int equPH = addEquLinear_PH(a_TSS, a_idEstagio, a_period, idUHE);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPH, equPH, 1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPH, equRE, -fator_participacao);
								for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, idPat, idUHE), equPH, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
							}
						}
					}
					else if(getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), equRE, -fator_participacao);
				}
			}

			// ConjuntoHidraulico
			else if ((idUHE != IdHidreletrica_Nenhum) && (idConUHE != IdConjuntoHidraulico_Nenhum) && (idUniUHE == IdUnidadeUHE_Nenhum) && (idUTE == IdTermeletrica_Nenhum) && (idUniUTE == IdUnidadeUTE_Nenhum) && (idINT == IdIntercambio_Nenhum) && (idCON == IdContrato_Nenhum) && (idDES == IdDemandaEspecial_Nenhum) && (idREN == IdRenovavel_Nenhum) && (idELE == IdUsinaElevatoria_Nenhum)) {
				if (tipoVarRE == TipoVariavelRestricaoOperativa_potencia_disponivel) {
					if (a_idPat == IdPatamarCarga_Nenhum) {
						if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, idUHE, idConUHE) > -1)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, idUHE, idConUHE), equRE, -fator_participacao);
						else if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE, idConUHE) > -1) {
							if (idPatEndPer == IdPatamarCarga_1)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE, idConUHE), equRE, -fator_participacao);
							else {
								const int varPHDISP = addVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, idUHE, idConUHE, 0.0, infinito, 0.0);
								const int equPHDISP = addEquLinear_PHDISP(a_TSS, a_idEstagio, a_period, idUHE, idConUHE);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP, equPHDISP, 1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP, equRE, -fator_participacao);
								for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, idPat, idUHE, idConUHE), equPHDISP, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
							}
						}
					}
					else if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConUHE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConUHE), equRE, -fator_participacao);
				}
				if (tipoVarRE == TipoVariavelRestricaoOperativa_potencia) {
					if (a_idPat == IdPatamarCarga_Nenhum) {
						if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, idUHE, idConUHE) > -1)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, idUHE, idConUHE), equRE, -fator_participacao);
						else if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE, idConUHE) > -1) {
							if (idPatEndPer == IdPatamarCarga_1)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE, idConUHE), equRE, -fator_participacao);
							else {
								const int varPH = addVarDecisao_PH(a_TSS, a_idEstagio, a_period, idUHE, idConUHE, 0.0, infinito, 0.0);
								const int equPH = addEquLinear_PH(a_TSS, a_idEstagio, a_period, idUHE, idConUHE);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPH, equPH, 1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPH, equRE, -fator_participacao);
								for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, idPat, idUHE, idConUHE), equPH, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
							}
						}
					}
					else if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConUHE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConUHE), equRE, -fator_participacao);
				}
			}

			// UnidadeUHE
			else if ((idUHE != IdHidreletrica_Nenhum) && (idConUHE != IdConjuntoHidraulico_Nenhum) && (idUniUHE != IdUnidadeUHE_Nenhum) && (idUTE == IdTermeletrica_Nenhum) && (idUniUTE == IdUnidadeUTE_Nenhum) && (idINT == IdIntercambio_Nenhum) && (idCON == IdContrato_Nenhum) && (idDES == IdDemandaEspecial_Nenhum) && (idREN == IdRenovavel_Nenhum) && (idELE == IdUsinaElevatoria_Nenhum)) {
				if (tipoVarRE == TipoVariavelRestricaoOperativa_potencia_disponivel) {
					if (a_idPat == IdPatamarCarga_Nenhum) {
						if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, idUHE, idConUHE, idUniUHE) > -1)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, idUHE, idConUHE, idUniUHE), equRE, -fator_participacao);
						else if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE, idConUHE, idUniUHE) > -1) {
							if (idPatEndPer == IdPatamarCarga_1)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE, idConUHE, idUniUHE), equRE, -fator_participacao);
							else {
								const int varPHDISP = addVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, idUHE, idConUHE, idUniUHE, 0.0, infinito, 0.0);
								const int equPHDISP = addEquLinear_PHDISP(a_TSS, a_idEstagio, a_period, idUHE, idConUHE, idUniUHE);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP, equPHDISP, 1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP, equRE, -fator_participacao);
								for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, idPat, idUHE, idConUHE, idUniUHE), equPHDISP, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
							}
						}
					}
					else if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConUHE, idUniUHE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConUHE, idUniUHE), equRE, -fator_participacao);
				}
				if (tipoVarRE == TipoVariavelRestricaoOperativa_potencia) {
					if (a_idPat == IdPatamarCarga_Nenhum) {
						if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, idUHE, idConUHE, idUniUHE) > -1)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, idUHE, idConUHE, idUniUHE), equRE, -fator_participacao);
						else if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE, idConUHE, idUniUHE) > -1) {
							if (idPatEndPer == IdPatamarCarga_1)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUHE, idConUHE, idUniUHE), equRE, -fator_participacao);
							else {
								const int varPH = addVarDecisao_PH(a_TSS, a_idEstagio, a_period, idUHE, idConUHE, idUniUHE, 0.0, infinito, 0.0);
								const int equPH = addEquLinear_PH(a_TSS, a_idEstagio, a_period, idUHE, idConUHE, idUniUHE);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPH, equPH, 1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPH, equRE, -fator_participacao);
								for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, idPat, idUHE, idConUHE, idUniUHE), equPH, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
							}
						}
					}
					else if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConUHE, idUniUHE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConUHE, idUniUHE), equRE, -fator_participacao);
				}
			}

			// Termeletrica
			else if ((idUHE == IdHidreletrica_Nenhum) && (idConUHE == IdConjuntoHidraulico_Nenhum) && (idUniUHE == IdUnidadeUHE_Nenhum) && (idUTE != IdTermeletrica_Nenhum) && (idUniUTE == IdUnidadeUTE_Nenhum) && (idINT == IdIntercambio_Nenhum) && (idCON == IdContrato_Nenhum) && (idDES == IdDemandaEspecial_Nenhum) && (idREN == IdRenovavel_Nenhum) && (idELE == IdUsinaElevatoria_Nenhum)) {
				if (tipoVarRE == TipoVariavelRestricaoOperativa_potencia_disponivel) {
					if (a_idPat == IdPatamarCarga_Nenhum) {
						if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, idUTE) > -1)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, idUTE), equRE, -fator_participacao);
						else if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUTE) > -1){
							if (idPatEndPer == IdPatamarCarga_1)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUTE), equRE, -fator_participacao);
							else {
								const int varPTDISP = addVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, idUTE, 0.0, infinito, 0.0);
								const int equPTDISP = addEquLinear_PTDISP(a_TSS, a_idEstagio, a_period, idUTE);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP, equPTDISP, 1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP, equRE, -fator_participacao);
								for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, idPat, idUTE), equPTDISP, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
							}
						}
					}
					else if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), equRE, -fator_participacao);
				}
				if (tipoVarRE == TipoVariavelRestricaoOperativa_potencia) {
					if (a_idPat == IdPatamarCarga_Nenhum) {
						if (getVarDecisao_PTseExistir(a_TSS, a_idEstagio, a_period, idUTE) > -1)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_period, idUTE), equRE, -fator_participacao);
						else if (getVarDecisao_PTseExistir(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUTE) > -1) {
							if (idPatEndPer == IdPatamarCarga_1)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUTE), equRE, -fator_participacao);
							else {
								const int varPT = addVarDecisao_PT(a_TSS, a_idEstagio, a_period, idUTE, 0.0, infinito, 0.0);
								const int equPT = addEquLinear_PT(a_TSS, a_idEstagio, a_period, idUTE);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPT, equPT, 1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPT, equRE, -fator_participacao);
								for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_period, idPat, idUTE), equPT, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
							}
						}
					}
					else if (getVarDecisao_PTseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_period, a_idPat, idUTE), equRE, -fator_participacao);
				}
			}

			// Unidade UTE Termeletrica
			else if ((idUHE == IdHidreletrica_Nenhum) && (idConUHE == IdConjuntoHidraulico_Nenhum) && (idUniUHE == IdUnidadeUHE_Nenhum) && (idUTE != IdTermeletrica_Nenhum) && (idUniUTE != IdUnidadeUTE_Nenhum) && (idINT == IdIntercambio_Nenhum) && (idCON == IdContrato_Nenhum) && (idDES == IdDemandaEspecial_Nenhum) && (idREN == IdRenovavel_Nenhum) && (idELE == IdUsinaElevatoria_Nenhum)) {
				if (tipoVarRE == TipoVariavelRestricaoOperativa_potencia_disponivel) {
					if (a_idPat == IdPatamarCarga_Nenhum) {
						if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, idUTE, idUniUTE) > -1)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, idUTE, idUniUTE), equRE, -fator_participacao);
						else if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUTE, idUniUTE) > -1) {
							if (idPatEndPer == IdPatamarCarga_1)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUTE, idUniUTE), equRE, -fator_participacao);
							else {
								const int varPTDISP = addVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, idUTE, idUniUTE, 0.0, infinito, 0.0);
								const int equPTDISP = addEquLinear_PTDISP(a_TSS, a_idEstagio, a_period, idUTE, idUniUTE);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP, equPTDISP, 1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP, equRE, -fator_participacao);
								for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, idPat, idUTE, idUniUTE), equPTDISP, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
							}
						}
					}
					else if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUniUTE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUniUTE), equRE, -fator_participacao);
				}
				if (tipoVarRE == TipoVariavelRestricaoOperativa_potencia) {
					if (a_idPat == IdPatamarCarga_Nenhum) {
						if (getVarDecisao_PTseExistir(a_TSS, a_idEstagio, a_period, idUTE, idUniUTE) > -1)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_period, idUTE, idUniUTE), equRE, -fator_participacao);
						else if (getVarDecisao_PTseExistir(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUTE, idUniUTE) > -1) {
							if (idPatEndPer == IdPatamarCarga_1)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idUTE, idUniUTE), equRE, -fator_participacao);
							else {
								const int varPT = addVarDecisao_PT(a_TSS, a_idEstagio, a_period, idUTE, idUniUTE, 0.0, infinito, 0.0);
								const int equPT = addEquLinear_PT(a_TSS, a_idEstagio, a_period, idUTE, idUniUTE);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPT, equPT, 1.0);
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPT, equRE, -fator_participacao);
								for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_period, idPat, idUTE, idUniUTE), equPT, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
							}
						}
					}
					else if (getVarDecisao_PTseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUniUTE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUniUTE), equRE, -fator_participacao);
				}
			}

			// Intercambio
			else if ((idUHE == IdHidreletrica_Nenhum) && (idConUHE == IdConjuntoHidraulico_Nenhum) && (idUniUHE == IdUnidadeUHE_Nenhum) && (idUTE == IdTermeletrica_Nenhum) && (idUniUTE == IdUnidadeUTE_Nenhum) && (idINT != IdIntercambio_Nenhum) && (idCON == IdContrato_Nenhum) && (idDES == IdDemandaEspecial_Nenhum) && (idREN == IdRenovavel_Nenhum) && (idELE == IdUsinaElevatoria_Nenhum)) {
				const IdSubmercado idSSE_orig = a_dados.getAtributo(idINT, AttComumIntercambio_submercado_origem, IdSubmercado());
				const IdSubmercado idSSE_dest = a_dados.getAtributo(idINT, AttComumIntercambio_submercado_destino, IdSubmercado());
				if (a_idPat == IdPatamarCarga_Nenhum) {
					if (getVarDecisao_PIseExistir(a_TSS, a_idEstagio, a_period, idINT, idSSE_orig, idSSE_dest) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_period, idINT, idSSE_orig, idSSE_dest), equRE, -fator_participacao);
					else if (getVarDecisao_PIseExistir(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idINT, idSSE_orig, idSSE_dest) > -1) {
						if (idPatEndPer == IdPatamarCarga_1)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idINT, idSSE_orig, idSSE_dest), equRE, -fator_participacao);
						else {
							const int varPI = addVarDecisao_PI(a_TSS, a_idEstagio, a_period, idINT, idSSE_orig, idSSE_dest, 0.0, infinito, 0.0);
							const int equPI = addEquLinear_PI(a_TSS, a_idEstagio, a_period, idINT, idSSE_orig, idSSE_dest);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPI, equPI, 1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPI, equRE, -fator_participacao);
							for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_period, idPat, idINT, idSSE_orig, idSSE_dest), equPI, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
						}
					}
				}
				else if (getVarDecisao_PIseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idINT, idSSE_orig, idSSE_dest) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_period, a_idPat, idINT, idSSE_orig, idSSE_dest), equRE, -fator_participacao);
			}

			// Contrato
			else if ((idUHE == IdHidreletrica_Nenhum) && (idConUHE == IdConjuntoHidraulico_Nenhum) && (idUniUHE == IdUnidadeUHE_Nenhum) && (idUTE == IdTermeletrica_Nenhum) && (idUniUTE == IdUnidadeUTE_Nenhum) && (idINT == IdIntercambio_Nenhum) && (idCON != IdContrato_Nenhum) && (idDES == IdDemandaEspecial_Nenhum) && (idREN == IdRenovavel_Nenhum) && (idELE == IdUsinaElevatoria_Nenhum)) {
				
				const TipoContrato tipo_contrato = a_dados.getAtributo(idCON, AttComumContrato_tipo_contrato, TipoContrato());
				if (a_idPat == IdPatamarCarga_Nenhum) {
					if (getVarDecisao_PCONseExistir(a_TSS, a_idEstagio, a_period, idCON, tipo_contrato) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PCON(a_TSS, a_idEstagio, a_period, idCON, tipo_contrato), equRE, -fator_participacao);
					else if (getVarDecisao_PCONseExistir(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idCON, tipo_contrato) > -1) {
						if (idPatEndPer == IdPatamarCarga_1)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PCON(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idCON, tipo_contrato), equRE, -fator_participacao);
						else {
							const int varPCON = addVarDecisao_PCON(a_TSS, a_idEstagio, a_period, idCON, tipo_contrato, 0.0, infinito, 0.0);
							const int equPCON = addEquLinear_PCON(a_TSS, a_idEstagio, a_period, idCON);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON, equPCON, 1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPCON, equRE, -fator_participacao);
							for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PCON(a_TSS, a_idEstagio, a_period, idPat, idCON, tipo_contrato), equPCON, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
						}
					}
				}
				else if (getVarDecisao_PCONseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idCON, tipo_contrato) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PCON(a_TSS, a_idEstagio, a_period, a_idPat, idCON, tipo_contrato), equRE, -fator_participacao);
					
			}

			// Demanda Especial
			else if ((idUHE == IdHidreletrica_Nenhum) && (idConUHE == IdConjuntoHidraulico_Nenhum) && (idUniUHE == IdUnidadeUHE_Nenhum) && (idUTE == IdTermeletrica_Nenhum) && (idUniUTE == IdUnidadeUTE_Nenhum) && (idINT == IdIntercambio_Nenhum) && (idCON == IdContrato_Nenhum) && (idDES != IdDemandaEspecial_Nenhum) && (idREN == IdRenovavel_Nenhum) && (idELE == IdUsinaElevatoria_Nenhum)) {
				if (a_idPat == IdPatamarCarga_Nenhum) {
					if (getVarDecisao_PLEseExistir(a_TSS, a_idEstagio, a_period, idDES) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PLE(a_TSS, a_idEstagio, a_period, idDES), equRE, -fator_participacao);
					else if (getVarDecisao_PLEseExistir(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idDES) > -1) {
						if (idPatEndPer == IdPatamarCarga_1)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PLE(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idDES), equRE, -fator_participacao);
						else {
							const int varPLE = addVarDecisao_PLE(a_TSS, a_idEstagio, a_period, idDES, 0.0, infinito, 0.0);
							const int equPLE = addEquLinear_PLE(a_TSS, a_idEstagio, a_period, idDES);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPLE, equPLE, 1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPLE, equRE, -fator_participacao);
							for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PLE(a_TSS, a_idEstagio, a_period, idPat, idDES), equPLE, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
						}
					}
				}
				else if (getVarDecisao_PLEseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idDES) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PLE(a_TSS, a_idEstagio, a_period, a_idPat, idDES), equRE, -fator_participacao);
			}

			// Renovavel
			else if ((idUHE == IdHidreletrica_Nenhum) && (idConUHE == IdConjuntoHidraulico_Nenhum) && (idUniUHE == IdUnidadeUHE_Nenhum) && (idUTE == IdTermeletrica_Nenhum) && (idUniUTE == IdUnidadeUTE_Nenhum) && (idINT == IdIntercambio_Nenhum) && (idCON == IdContrato_Nenhum) && (idDES == IdDemandaEspecial_Nenhum) && (idREN != IdRenovavel_Nenhum) && (idELE == IdUsinaElevatoria_Nenhum)) {
				if (a_idPat == IdPatamarCarga_Nenhum) {
					if (getVarDecisao_PRseExistir(a_TSS, a_idEstagio, a_period, idREN) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PR(a_TSS, a_idEstagio, a_period, idREN), equRE, -fator_participacao);
					else if (getVarDecisao_PRseExistir(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idREN) > -1) {
						if (idPatEndPer == IdPatamarCarga_1)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PR(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idREN), equRE, -fator_participacao);
						else {
							const int varPR = addVarDecisao_PR(a_TSS, a_idEstagio, a_period, idREN, 0.0, infinito, 0.0);
							const int equPR = addEquLinear_PR(a_TSS, a_idEstagio, a_period, idREN);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPR, equPR, 1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPR, equRE, -fator_participacao);
							for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PR(a_TSS, a_idEstagio, a_period, idPat, idREN), equPR, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
						}
					}
				}
				else if (getVarDecisao_PRseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idREN) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PR(a_TSS, a_idEstagio, a_period, a_idPat, idREN), equRE, -fator_participacao);
			}

			// Elevatoria
			else if ((idUHE == IdHidreletrica_Nenhum) && (idConUHE == IdConjuntoHidraulico_Nenhum) && (idUniUHE == IdUnidadeUHE_Nenhum) && (idUTE == IdTermeletrica_Nenhum) && (idUniUTE == IdUnidadeUTE_Nenhum) && (idINT == IdIntercambio_Nenhum) && (idCON == IdContrato_Nenhum) && (idDES == IdDemandaEspecial_Nenhum) && (idREN == IdRenovavel_Nenhum) && (idELE != IdUsinaElevatoria_Nenhum)) {
				if (a_idPat == IdPatamarCarga_Nenhum) {
					if (getVarDecisao_PBOMDISPseExistir(a_TSS, a_idEstagio, a_period, idELE) > -1)
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PBOMDISP(a_TSS, a_idEstagio, a_period, idELE), equRE, -fator_participacao);
					else if (getVarDecisao_PBOMDISPseExistir(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idELE) > -1) {
						if (idPatEndPer == IdPatamarCarga_1)
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PBOMDISP(a_TSS, a_idEstagio, a_period, IdPatamarCarga_1, idELE), equRE, -fator_participacao);
						else {
							const int varPBOMDISP = addVarDecisao_PBOMDISP(a_TSS, a_idEstagio, a_period, idELE, 0.0, infinito, 0.0);
							const int equPBOMDISP = addEquLinear_PBOMDISP(a_TSS, a_idEstagio, a_period, idELE);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPBOMDISP, equPBOMDISP, 1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPBOMDISP, equRE, -fator_participacao);
							for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= idPatEndPer; idPat++)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PBOMDISP(a_TSS, a_idEstagio, a_period, idPat, idELE), equPBOMDISP, -a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double()));
						}
					}
				}
				else if (getVarDecisao_PBOMDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idELE) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PBOMDISP(a_TSS, a_idEstagio, a_period, a_idPat, idELE), equRE, -fator_participacao);
			}
			else
				throw std::invalid_argument("Combinacao invalida para RE em " + getFullString(idElem));

		} // for (IdElementoSistema idElem = IdElementoSistema_1; idElem <= idElemEnd; idElem++) {

		return varRE;

	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesEletricas(" + getFullString(a_TSS) + ",Dados& a_dados, " + getFullString(a_idEstagio) + "," + getFullString(a_period) + "," + getFullString(a_periodNext) + "," + getFullString(a_idPat) + "," + getFullString(a_idRE) + "): \n" + std::string(erro.what())); }

}

int ModeloOtimizacao::criarRestricoesEletricas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idStage, IdRestricaoEletrica& a_idRE, const std::vector<std::string>& a_stateVar, const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>& a_horizon) {

	try {

		if (a_stateVar.at(0) != "RE")
			throw std::invalid_argument("Not RE.");

		Periodo periodo = Periodo(a_stateVar.at(2));

		std::vector<std::vector<std::string>> elemsRE = std::vector<std::vector<std::string>>();
		elemsRE.reserve(1);

		int cont = 4;
		while (cont < a_stateVar.size()) {
			if (a_stateVar.at(cont) != "FP")
				throw std::invalid_argument("Error FP do " + getFullString(a_idStage));
			elemsRE.push_back(std::vector<std::string>());
			elemsRE.at(elemsRE.size() - 1).reserve(4);
			elemsRE.at(elemsRE.size() - 1).push_back(a_stateVar.at(cont));
			for (int i = cont + 1; i < a_stateVar.size(); i++) {
				if (a_stateVar.at(i) == "FP")
					break;
				elemsRE.at(elemsRE.size() - 1).push_back(a_stateVar.at(i));

			}
			cont += int(elemsRE.at(elemsRE.size() - 1).size());
		}

		std::vector<Periodo> periods = a_horizon.getIteradores(periodo);

		bool period_fit = false;
		if (periods.size() == 1) {
			if (periods.at(0) == periodo)
				period_fit = true;
		}

		a_idRE = IdRestricaoEletrica_Nenhum;
		if (period_fit) {
			const IdRestricaoEletrica idREIni = a_dados.getMenorId(IdRestricaoEletrica());
			const IdRestricaoEletrica idREOut = a_dados.getIdOut(IdRestricaoEletrica());
			for (IdRestricaoEletrica idRE = idREIni; idRE < idREOut; a_dados.incr(idRE)) {
				if (a_dados.vetorRestricaoEletrica.at(idRE).vetorElementoSistema.numObjetos() == elemsRE.size()) {
					for (IdElementoSistema idElem = IdElementoSistema_1; idElem <= a_dados.getMaiorId(idRE, IdElementoSistema()); idElem++) {
						bool idElemFound = false;
						for (int e = 0; e < int(elemsRE.size()); e++) {
							if (a_dados.getSize1Matriz(idRE, idElem, AttMatrizElementoSistema_fator_participacao) > 0) {
								if (a_dados.getElementoMatriz(idRE, idElem, AttMatrizElementoSistema_fator_participacao, periodo, IdPatamarCarga_1, double()) == getdoubleFromChar(elemsRE.at(e).at(1).c_str())) {
									if (a_dados.getAtributo(idRE, idElem, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa()) == getTipoVariavelRestricaoOperativaFromChar(elemsRE.at(e).at(2).c_str())) {
										
										const IdHidreletrica idUHE = a_dados.getAtributo(idRE, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
										if ((idUHE != IdHidreletrica_Nenhum) && (idUHE == getIdHidreletricaFromChar(elemsRE.at(e).at(3).c_str()))) {
											if (elemsRE.at(e).size() > 4) {
												if (a_dados.getAtributo(idRE, idElem, AttComumElementoSistema_conjuntohidraulico, IdConjuntoHidraulico()) == getIdConjuntoHidraulicoFromChar(elemsRE.at(e).at(4).c_str())) {
													if (elemsRE.at(e).size() > 5) {
														if (a_dados.getAtributo(idRE, idElem, AttComumElementoSistema_unidadehidraulica, IdUnidadeUHE()) == getIdUnidadeUHEFromChar(elemsRE.at(e).at(5).c_str())) {
															if (elemsRE.at(e).size() > 6)
																throw std::invalid_argument("Error Elem de RE do " + getFullString(a_idStage));
															else {
																idElemFound = true;
															}
														}
													}
													else {
														idElemFound = true;
													}
												}
											}
											else {
												idElemFound = true;
											}
										} // if ((idUHE != IdHidreletrica_Nenhum) && (idUHE == getIdHidreletricaFromChar(elemsRE.at(e).at(3).c_str()))) {

										const IdTermeletrica idUTE = a_dados.getAtributo(idRE, idElem, AttComumElementoSistema_termeletrica, IdTermeletrica());
										if ((idUTE != IdTermeletrica_Nenhum) && (idUTE == getIdTermeletricaFromChar(elemsRE.at(e).at(3).c_str()))) {
											if (idElemFound)
												throw std::invalid_argument("Error Elem de RE do " + getFullString(a_idStage));
											if (elemsRE.at(e).size() > 4) {
												if (a_dados.getAtributo(idRE, idElem, AttComumElementoSistema_unidadeTermeletrica, IdUnidadeUTE()) == getIdUnidadeUTEFromChar(elemsRE.at(e).at(4).c_str())) {
													if (elemsRE.at(e).size() > 5) {
														throw std::invalid_argument("Error Elem de RE do " + getFullString(a_idStage));
													}
													else {
														idElemFound = true;
													}
												}
											}
											else {
												idElemFound = true;
											}
										} // if ((idUTE != IdTermeletrica_Nenhum) && (idUTE == getIdTermeletricaFromChar(elemsRE.at(e).at(3).c_str()))) {

										const IdDemandaEspecial idDE = a_dados.getAtributo(idRE, idElem, AttComumElementoSistema_demandaEspecial, IdDemandaEspecial());
										if ((idDE != IdDemandaEspecial_Nenhum) && (idDE == getIdDemandaEspecialFromChar(elemsRE.at(e).at(3).c_str()))) {
											if ((idElemFound) || (elemsRE.at(e).size() > 4))
												throw std::invalid_argument("Error Elem de RE do " + getFullString(a_idStage));
											idElemFound = true;
										}

										const IdContrato idContrato = a_dados.getAtributo(idRE, idElem, AttComumElementoSistema_contrato, IdContrato());
										if ((idContrato != IdContrato_Nenhum) && (idContrato == getIdContratoFromChar(elemsRE.at(e).at(3).c_str()))) {
											if ((idElemFound) || (elemsRE.at(e).size() > 4))
												throw std::invalid_argument("Error Elem de RE do " + getFullString(a_idStage));
											idElemFound = true;
										}

										const IdIntercambio idIntercambio = a_dados.getAtributo(idRE, idElem, AttComumElementoSistema_intercambio, IdIntercambio());
										if ((idIntercambio != IdIntercambio_Nenhum) && (idIntercambio == getIdIntercambioFromChar(elemsRE.at(e).at(3).c_str()))) {
											if ((idElemFound) || (elemsRE.at(e).size() > 4))
												throw std::invalid_argument("Error Elem de RE do " + getFullString(a_idStage));
											idElemFound = true;
										}

										const IdRenovavel idRenovavel = a_dados.getAtributo(idRE, idElem, AttComumElementoSistema_renovavel, IdRenovavel());
										if ((idRenovavel != IdRenovavel_Nenhum) && (idRenovavel == getIdRenovavelFromChar(elemsRE.at(e).at(3).c_str()))) {
											if ((idElemFound) || (elemsRE.at(e).size() > 4))
												throw std::invalid_argument("Error Elem de RE do " + getFullString(a_idStage));
											idElemFound = true;
										}

										const IdUsinaElevatoria idUsinaElevatoria = a_dados.getAtributo(idRE, idElem, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());
										if ((idUsinaElevatoria != IdUsinaElevatoria_Nenhum) && (idUsinaElevatoria == getIdUsinaElevatoriaFromChar(elemsRE.at(e).at(3).c_str()))) {
											if ((idElemFound) || (elemsRE.at(e).size() > 4))
												throw std::invalid_argument("Error Elem de RE do " + getFullString(a_idStage));
											idElemFound = true;
										}

										const IdSubmercado idSubmercadoLiq = a_dados.getAtributo(idRE, idElem, AttComumElementoSistema_demandaliquida, IdSubmercado());
										if ((idSubmercadoLiq != IdSubmercado_Nenhum) && (idSubmercadoLiq == getIdSubmercadoFromChar(elemsRE.at(e).at(3).c_str()))) {
											if (elemsRE.at(e).size() > 4) {
												if ("liquida" == elemsRE.at(e).at(4).c_str()) {
													if (idElemFound)
														throw std::invalid_argument("Error Elem de RE do " + getFullString(a_idStage));
													idElemFound = true;
												}
											}
										}

										const IdSubmercado idSubmercado = a_dados.getAtributo(idRE, idElem, AttComumElementoSistema_demanda, IdSubmercado());
										if ((idSubmercado != IdSubmercado_Nenhum) && (idSubmercado == getIdSubmercadoFromChar(elemsRE.at(e).at(3).c_str()))) {
											if (idElemFound)
												throw std::invalid_argument("Error Elem de RE do " + getFullString(a_idStage));
											idElemFound = true;
										}


										if (idElemFound)
											break;

									}
								}
							}
						} // for (int e = 0; e < int(elemsRE.size()); e++) {
						if (!idElemFound) {
							a_idRE = IdRestricaoEletrica_Nenhum;
							break;
						}
						else
							a_idRE = idRE;
					}
				}
			}
		}

		if (a_idRE == IdRestricaoEletrica_Nenhum) {
			a_idRE = a_dados.getIdOut(IdRestricaoEletrica());
			RestricaoEletrica RE;
			RE.setAtributo(AttComumRestricaoEletrica_idRestricaoEletrica, a_idRE);
			a_dados.vetorRestricaoEletrica.add(RE);

			IdElementoSistema idElem = IdElementoSistema_1;
			for (int e = 0; e < int(elemsRE.size()); e++) {
				ElementoSistema elem;
				elem.setAtributo(AttComumElementoSistema_idElementoSistema, idElem);
				a_dados.vetorRestricaoEletrica.at(a_idRE).vetorElementoSistema.add(elem);
				a_dados.vetorRestricaoEletrica.at(a_idRE).vetorElementoSistema.at(idElem).setAtributo(AttComumElementoSistema_fator_participacao, getdoubleFromChar(elemsRE.at(e).at(1).c_str()));

				const TipoVariavelRestricaoOperativa tipoVarRE = getTipoVariavelRestricaoOperativaFromChar(elemsRE.at(e).at(2).c_str());
				if ((tipoVarRE != TipoVariavelRestricaoOperativa_potencia) && (tipoVarRE != TipoVariavelRestricaoOperativa_potencia_disponivel))
					throw std::invalid_argument("Tipo invalido");

				a_dados.vetorRestricaoEletrica.at(a_idRE).vetorElementoSistema.at(idElem).setAtributo(AttComumElementoSistema_tipoVariavelRestricaoOperativa, tipoVarRE);

				const IdHidreletrica idUHE = getIdHidreletricaFromChar(elemsRE.at(e).at(3).c_str());
				if (idUHE != IdHidreletrica_Nenhum) {
					a_dados.vetorRestricaoEletrica.at(a_idRE).vetorElementoSistema.at(idElem).setAtributo(AttComumElementoSistema_hidreletrica, idUHE);
					if (elemsRE.at(e).size() > 4) {
						a_dados.vetorRestricaoEletrica.at(a_idRE).vetorElementoSistema.at(idElem).setAtributo(AttComumElementoSistema_conjuntohidraulico, getIdConjuntoHidraulicoFromChar(elemsRE.at(e).at(4).c_str()));
						if (elemsRE.at(e).size() > 5) {
							a_dados.vetorRestricaoEletrica.at(a_idRE).vetorElementoSistema.at(idElem).setAtributo(AttComumElementoSistema_unidadehidraulica, getIdUnidadeUHEFromChar(elemsRE.at(e).at(5).c_str()));
							if (elemsRE.at(e).size() > 6)
								throw std::invalid_argument("Error Elem de RE do " + getFullString(a_idStage));
						}
					}
				} // if (idUHE != IdHidreletrica_Nenhum) {

				const IdTermeletrica idUTE = getIdTermeletricaFromChar(elemsRE.at(e).at(3).c_str());
				if (idUTE != IdTermeletrica_Nenhum) {
					a_dados.vetorRestricaoEletrica.at(a_idRE).vetorElementoSistema.at(idElem).setAtributo(AttComumElementoSistema_termeletrica, idUTE);
					if (elemsRE.at(e).size() > 4) {
						a_dados.vetorRestricaoEletrica.at(a_idRE).vetorElementoSistema.at(idElem).setAtributo(AttComumElementoSistema_unidadeTermeletrica, getIdUnidadeUTEFromChar(elemsRE.at(e).at(4).c_str()));
						if (elemsRE.at(e).size() > 5) {
							throw std::invalid_argument("Error Elem de RE do " + getFullString(a_idStage));
						}
					}
				} // if (idUTE != IdTermeletrica_Nenhum) {

				a_dados.vetorRestricaoEletrica.at(a_idRE).vetorElementoSistema.at(idElem).setAtributo(AttComumElementoSistema_demandaEspecial,  getIdDemandaEspecialFromChar(elemsRE.at(e).at(3).c_str()));
				a_dados.vetorRestricaoEletrica.at(a_idRE).vetorElementoSistema.at(idElem).setAtributo(AttComumElementoSistema_contrato,         getIdContratoFromChar(elemsRE.at(e).at(3).c_str()));
				a_dados.vetorRestricaoEletrica.at(a_idRE).vetorElementoSistema.at(idElem).setAtributo(AttComumElementoSistema_intercambio,      getIdIntercambioFromChar(elemsRE.at(e).at(3).c_str()));
				a_dados.vetorRestricaoEletrica.at(a_idRE).vetorElementoSistema.at(idElem).setAtributo(AttComumElementoSistema_renovavel,        getIdRenovavelFromChar(elemsRE.at(e).at(3).c_str()));
				a_dados.vetorRestricaoEletrica.at(a_idRE).vetorElementoSistema.at(idElem).setAtributo(AttComumElementoSistema_usina_elevatoria, getIdUsinaElevatoriaFromChar(elemsRE.at(e).at(3).c_str()));
				a_dados.vetorRestricaoEletrica.at(a_idRE).vetorElementoSistema.at(idElem).setAtributo(AttComumElementoSistema_demanda,          getIdSubmercadoFromChar(elemsRE.at(e).at(3).c_str()));
				a_dados.vetorRestricaoEletrica.at(a_idRE).vetorElementoSistema.at(idElem).setAtributo(AttComumElementoSistema_demandaliquida,   getIdSubmercadoFromChar(elemsRE.at(e).at(3).c_str()));

			} // for (int e = 0; e < int(elemsRE.size()); e++) {
		} // if (a_idRE == IdRestricaoEletrica_Nenhum) {

		return criarVariaveisDecisao_VariaveisEstado_Restricoes_RE(a_TSS, a_dados, a_idStage, periodo, a_idRE, a_horizon, true);


	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesEletricas(" + getFullString(a_TSS) + ",Dados& a_dados, " + getFullString(a_idStage) + "): \n" + std::string(erro.what())); }

}

bool ModeloOtimizacao::tryIncorporarRestricaoEletricaHidreletrica(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idStage, Periodo& a_period, const IdRestricaoEletrica a_idRE, const IdElementoSistema a_idElem, const int a_equRE_per) {

	try {

		const double fp = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_fator_participacao, double());
		const TipoVariavelRestricaoOperativa tipo = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

		if ((tipo != TipoVariavelRestricaoOperativa_potencia_disponivel) && (tipo != TipoVariavelRestricaoOperativa_potencia))
			throw std::invalid_argument("Invalid type " + getFullString(a_idRE) + " " + getFullString(a_idElem) + " and " + getFullString(a_idStage));

		const IdHidreletrica idUHE = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
		const IdConjuntoHidraulico idConj = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_conjuntohidraulico, IdConjuntoHidraulico());
		const IdUnidadeUHE idUnd = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_unidadehidraulica, IdUnidadeUHE());

		if (idUHE != IdHidreletrica_Nenhum) {
			if ((idConj != IdConjuntoHidraulico_Nenhum) && (idUnd != IdUnidadeUHE_Nenhum)) {
				if (a_dados.getElementoVetor(idUHE, idConj, idUnd, AttVetorUnidadeUHE_disponibilidade, a_period, double()) > 0.0) {
					if (tipo == TipoVariavelRestricaoOperativa_potencia_disponivel) {
						if (getVarDecisao_PHDISPseExistir(a_TSS, a_idStage, a_period, idUHE, idConj, idUnd) > -1)
							vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idStage, a_period, idUHE, idConj, idUnd), a_equRE_per, -fp);
						else
							vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idStage, a_period, IdPatamarCarga_1, idUHE, idConj, idUnd), a_equRE_per, -fp);
						return true;
					}
					else if (tipo == TipoVariavelRestricaoOperativa_potencia) {
						if (getVarDecisao_PHseExistir(a_TSS, a_idStage, a_period, idUHE, idConj, idUnd) > -1)
							vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idStage, a_period, idUHE, idConj, idUnd), a_equRE_per, -fp);
						else
							vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idStage, a_period, IdPatamarCarga_1, idUHE, idConj, idUnd), a_equRE_per, -fp);
						return true;
					}
				}
			}
			else if (idConj != IdConjuntoHidraulico_Nenhum) {
				if (a_dados.getElementoVetor(idUHE, idConj, AttVetorConjuntoHidraulico_disponibilidade, a_period, double()) > 0.0) {
					if (tipo == TipoVariavelRestricaoOperativa_potencia_disponivel) {
						if (getVarDecisao_PHDISPseExistir(a_TSS, a_idStage, a_period, idUHE, idConj) > -1)
							vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idStage, a_period, idUHE, idConj), a_equRE_per, -fp);
						else
							vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idStage, a_period, IdPatamarCarga_1, idUHE, idConj), a_equRE_per, -fp);
						return true;
					}
					else if (tipo == TipoVariavelRestricaoOperativa_potencia) {
						if (getVarDecisao_PHseExistir(a_TSS, a_idStage, a_period, idUHE, idConj) > -1)
							vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idStage, a_period, idUHE, idConj), a_equRE_per, -fp);
						else
							vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idStage, a_period, IdPatamarCarga_1, idUHE, idConj), a_equRE_per, -fp);
						return true;
					}
				}
			}
			else {
				if (a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_disponibilidade, a_period, double()) > 0.0) {
					if (tipo == TipoVariavelRestricaoOperativa_potencia_disponivel) {
						if (getVarDecisao_PHDISPseExistir(a_TSS, a_idStage, a_period, idUHE) > -1)
							vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idStage, a_period, idUHE), a_equRE_per, -fp);
						else
							vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idStage, a_period, IdPatamarCarga_1, idUHE), a_equRE_per, -fp);
						return true;
					}
					else if (tipo == TipoVariavelRestricaoOperativa_potencia) {
						if (getVarDecisao_PHseExistir(a_TSS, a_idStage, a_period, idUHE) > -1)
							vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idStage, a_period, idUHE), a_equRE_per, -fp);
						else
							vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idStage, a_period, IdPatamarCarga_1, idUHE), a_equRE_per, -fp);
						return true;

					}
				}
			}
			return false;
		}
		return false;
	}
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::tryIncorporarRestricaoEletricaHidreletrica(" + getFullString(a_TSS) + ",Dados& a_dados, " + getFullString(a_idStage) + "," + getFullString(a_period) + "," + getFullString(a_idRE) + "," + getFullString(a_idElem) + "," + getFullString(a_equRE_per) + "): \n" + std::string(erro.what())); }
}

bool ModeloOtimizacao::tryIncorporarRestricaoEletricaTermeletrica(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idStage, Periodo& a_period, const IdRestricaoEletrica a_idRE, const IdElementoSistema a_idElem, const int a_equRE_per) {

	try {
		const double fp = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_fator_participacao, double());
		const TipoVariavelRestricaoOperativa tipo = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

		if ((tipo != TipoVariavelRestricaoOperativa_potencia_disponivel) && (tipo != TipoVariavelRestricaoOperativa_potencia))
			throw std::invalid_argument("Invalid type " + getFullString(a_idRE) + " " + getFullString(a_idElem) + " and " + getFullString(a_idStage));

		const IdTermeletrica idUTE = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_termeletrica, IdTermeletrica());
		const IdUnidadeUTE idUnd = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_unidadeTermeletrica, IdUnidadeUTE());

		if (idUTE != IdTermeletrica_Nenhum) {
			if (idUnd != IdUnidadeUTE_Nenhum) {
				if (a_dados.getElementoVetor(idUTE, idUnd, AttVetorUnidadeUTE_disponibilidade, a_period, double()) > 0.0) {
					if (tipo == TipoVariavelRestricaoOperativa_potencia_disponivel) {
						if (getVarDecisao_PTDISPseExistir(a_TSS, a_idStage, a_period, idUTE, idUnd) > -1)
							vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idStage, a_period, idUTE, idUnd), a_equRE_per, -fp);
						else
							vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idStage, a_period, IdPatamarCarga_1, idUTE, idUnd), a_equRE_per, -fp);
						return true;
					}
					else if (tipo == TipoVariavelRestricaoOperativa_potencia) {
						if (getVarDecisao_PTseExistir(a_TSS, a_idStage, a_period, idUTE, idUnd) > -1)
							vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idStage, a_period, idUTE, idUnd), a_equRE_per, -fp);
						else
							vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idStage, a_period, IdPatamarCarga_1, idUTE, idUnd), a_equRE_per, -fp);
						return true;
					}
				}
			}
			else {
				if (a_dados.getElementoVetor(idUTE, AttVetorTermeletrica_disponibilidade, a_period, double()) > 0.0) {
					if (tipo == TipoVariavelRestricaoOperativa_potencia_disponivel) {
						if (getVarDecisao_PTDISPseExistir(a_TSS, a_idStage, a_period, idUTE) > -1)
							vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idStage, a_period, idUTE), a_equRE_per, -fp);
						else
							vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idStage, a_period, IdPatamarCarga_1, idUTE), a_equRE_per, -fp);
						return true;
					}
					else if (tipo == TipoVariavelRestricaoOperativa_potencia) {
						if (getVarDecisao_PTseExistir(a_TSS, a_idStage, a_period, idUTE) > -1)
							vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idStage, a_period, idUTE), a_equRE_per, -fp);
						else
							vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idStage, a_period, IdPatamarCarga_1, idUTE), a_equRE_per, -fp);
						return true;

					}
				}
			}
			return false;
		}
		return false;
	}
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::tryIncorporarRestricaoEletricaTermeletrica(" + getFullString(a_TSS) + ",Dados& a_dados, " + getFullString(a_idStage) + "," + getFullString(a_period) + "," + getFullString(a_idRE) + "," + getFullString(a_idElem) + "," + getFullString(a_equRE_per) + "): \n" + std::string(erro.what())); }
}

bool ModeloOtimizacao::tryIncorporarRestricaoEletricaDemanda(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idStage, Periodo& a_period, const IdRestricaoEletrica a_idRE, const IdElementoSistema a_idElem, const int a_equRE_per) {

	try {

		const double fp = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_fator_participacao, double());
		const TipoVariavelRestricaoOperativa tipo = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

		if ((tipo != TipoVariavelRestricaoOperativa_potencia_disponivel) && (tipo != TipoVariavelRestricaoOperativa_potencia))
			throw std::invalid_argument("Invalid type " + getFullString(a_idRE) + " " + getFullString(a_idElem) + " and " + getFullString(a_idStage));

		const IdSubmercado idDemanda = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_demanda, IdSubmercado());
		const IdSubmercado idDemandaLiq = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_demandaliquida, IdSubmercado());
		const IdDemandaEspecial idDemandaEsp = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_demandaEspecial, IdDemandaEspecial());

		if (idDemanda != IdSubmercado_Nenhum) {
			if (getVarDecisao_PLseExistir(a_TSS, a_idStage, a_period, idDemanda) > -1)
				vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PL(a_TSS, a_idStage, a_period, idDemanda), a_equRE_per, -fp);
			else
				vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PL(a_TSS, a_idStage, a_period, IdPatamarCarga_1, idDemanda), a_equRE_per, -fp);
			return true;
		}
		else if (idDemandaLiq != IdSubmercado_Nenhum) {
			if (getVarDecisao_PL_LIQseExistir(a_TSS, a_idStage, a_period, idDemandaLiq) > -1)
				vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PL_LIQ(a_TSS, a_idStage, a_period, idDemandaLiq), a_equRE_per, -fp);
			else
				vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PL_LIQ(a_TSS, a_idStage, a_period, IdPatamarCarga_1, idDemandaLiq), a_equRE_per, -fp);
			return true;
		}
		else if (idDemandaEsp != IdDemandaEspecial_Nenhum) {
			if (getVarDecisao_PLEseExistir(a_TSS, a_idStage, a_period, idDemandaEsp) > -1)
				vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PLE(a_TSS, a_idStage, a_period, idDemandaEsp), a_equRE_per, -fp);
			else
				vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PLE(a_TSS, a_idStage, a_period, IdPatamarCarga_1, idDemandaEsp), a_equRE_per, -fp);
			return true;
		}
		return false;
	}
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::tryIncorporarRestricaoEletricaDemanda(" + getFullString(a_TSS) + ",Dados& a_dados, " + getFullString(a_idStage) + "," + getFullString(a_period) + "," + getFullString(a_idRE) + "," + getFullString(a_idElem) + "," + getFullString(a_equRE_per) + "): \n" + std::string(erro.what())); }
}

bool ModeloOtimizacao::tryIncorporarRestricaoEletricaRenovavel(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idStage, Periodo& a_period, const IdRestricaoEletrica a_idRE, const IdElementoSistema a_idElem, const int a_equRE_per) {

	try {

		const double fp = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_fator_participacao, double());
		const TipoVariavelRestricaoOperativa tipo = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

		if ((tipo != TipoVariavelRestricaoOperativa_potencia_disponivel) && (tipo != TipoVariavelRestricaoOperativa_potencia))
			throw std::invalid_argument("Invalid type " + getFullString(a_idRE) + " " + getFullString(a_idElem) + " and " + getFullString(a_idStage));

		const IdRenovavel idRenovavel = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_renovavel, IdRenovavel());

		if (idRenovavel != IdRenovavel_Nenhum) {
			if (getVarDecisao_PRseExistir(a_TSS, a_idStage, a_period, idRenovavel) > -1)
				vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PR(a_TSS, a_idStage, a_period, idRenovavel), a_equRE_per, -fp);
			else
				vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PR(a_TSS, a_idStage, a_period, IdPatamarCarga_1, idRenovavel), a_equRE_per, -fp);
			return true;
		}
		return false;
	}
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::tryIncorporarRestricaoEletricaRenovaveis(" + getFullString(a_TSS) + ",Dados& a_dados, " + getFullString(a_idStage) + "," + getFullString(a_period) + "," + getFullString(a_idRE) + "," + getFullString(a_idElem) + "," + getFullString(a_equRE_per) + "): \n" + std::string(erro.what())); }
}

bool ModeloOtimizacao::tryIncorporarRestricaoEletricaIntercambio(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idStage, Periodo& a_period, const IdRestricaoEletrica a_idRE, const IdElementoSistema a_idElem, const int a_equRE_per) {

	try {

		const double fp = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_fator_participacao, double());
		const TipoVariavelRestricaoOperativa tipo = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

		if ((tipo != TipoVariavelRestricaoOperativa_potencia_disponivel) && (tipo != TipoVariavelRestricaoOperativa_potencia))
			throw std::invalid_argument("Invalid type " + getFullString(a_idRE) + " " + getFullString(a_idElem) + " and " + getFullString(a_idStage));

		const IdIntercambio idIntercambio = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_intercambio, IdIntercambio());
		const IdSubmercado idSSE_orig = a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_origem, IdSubmercado());
		const IdSubmercado idSSE_dest = a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_destino, IdSubmercado());

		if (idIntercambio != IdIntercambio_Nenhum) {
			if (getVarDecisao_PIseExistir(a_TSS, a_idStage, a_period, idIntercambio, idSSE_orig, idSSE_dest) > -1)
				vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idStage, a_period, idIntercambio, idSSE_orig, idSSE_dest), a_equRE_per, -fp);
			else
				vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idStage, a_period, IdPatamarCarga_1, idIntercambio, idSSE_orig, idSSE_dest), a_equRE_per, -fp);
			return true;
		}
		return false;
	}
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::tryIncorporarRestricaoEletricaIntercambio(" + getFullString(a_TSS) + ",Dados& a_dados, " + getFullString(a_idStage) + "," + getFullString(a_period) + "," + getFullString(a_idRE) + "," + getFullString(a_idElem) + "," + getFullString(a_equRE_per) + "): \n" + std::string(erro.what())); }
}

bool ModeloOtimizacao::tryIncorporarRestricaoEletricaContrato(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idStage, Periodo& a_period, const IdRestricaoEletrica a_idRE, const IdElementoSistema a_idElem, const int a_equRE_per) {

	try {

		const double fp = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_fator_participacao, double());
		const TipoVariavelRestricaoOperativa tipo = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

		if ((tipo != TipoVariavelRestricaoOperativa_potencia_disponivel) && (tipo != TipoVariavelRestricaoOperativa_potencia))
			throw std::invalid_argument("Invalid type " + getFullString(a_idRE) + " " + getFullString(a_idElem) + " and " + getFullString(a_idStage));

		const IdContrato idContrato = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_contrato, IdContrato());
		const TipoContrato tipo_contrato = a_dados.getAtributo(idContrato, AttComumContrato_tipo_contrato, TipoContrato());

		if (idContrato != IdContrato_Nenhum) {
			if (getVarDecisao_PCONseExistir(a_TSS, a_idStage, a_period, idContrato, tipo_contrato) > -1)
				vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PCON(a_TSS, a_idStage, a_period, idContrato, tipo_contrato), a_equRE_per, -fp);
			else
				vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PCON(a_TSS, a_idStage, a_period, IdPatamarCarga_1, idContrato, tipo_contrato), a_equRE_per, -fp);
			return true;
		}
		return false;
	}
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::tryIncorporarRestricaoEletricaContrato(" + getFullString(a_TSS) + ",Dados& a_dados, " + getFullString(a_idStage) + "," + getFullString(a_period) + "," + getFullString(a_idRE) + "," + getFullString(a_idElem) + "," + getFullString(a_equRE_per) + "): \n" + std::string(erro.what())); }
}

bool ModeloOtimizacao::tryIncorporarRestricaoEletricaUsinaElevatoria(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idStage, Periodo& a_period, const IdRestricaoEletrica a_idRE, const IdElementoSistema a_idElem, const int a_equRE_per) {

	try {

		const double fp = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_fator_participacao, double());
		const TipoVariavelRestricaoOperativa tipo = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

		if ((tipo != TipoVariavelRestricaoOperativa_potencia_disponivel) && (tipo != TipoVariavelRestricaoOperativa_potencia))
			throw std::invalid_argument("Invalid type " + getFullString(a_idRE) + " " + getFullString(a_idElem) + " and " + getFullString(a_idStage));

		const IdUsinaElevatoria idUsinaElevatoria = a_dados.getAtributo(a_idRE, a_idElem, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());

		if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {
			if (getVarDecisao_PBOMDISPseExistir(a_TSS, a_idStage, a_period, idUsinaElevatoria) > -1)
				vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PBOMDISP(a_TSS, a_idStage, a_period, idUsinaElevatoria), a_equRE_per, -fp);
			else
				vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PBOMDISP(a_TSS, a_idStage, a_period, IdPatamarCarga_1, idUsinaElevatoria), a_equRE_per, -fp);
			return true;
		}
		return false;
	}
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::tryIncorporarRestricaoEletricaUsinaElevatoria(" + getFullString(a_TSS) + ",Dados& a_dados, " + getFullString(a_idStage) + "," + getFullString(a_period) + "," + getFullString(a_idRE) + "," + getFullString(a_idElem) + "," + getFullString(a_equRE_per) + "): \n" + std::string(erro.what())); }
}


int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_RE(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idStage, Periodo& a_period_lag, const IdRestricaoEletrica a_idRE, const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>& a_horizon, bool a_isVarEstadoExterna) {

	try {

		const IdEstagio stageIni = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());

		if (a_idStage < stageIni)
			return -1;

		if ((!vetorEstagio.at(a_idStage).isSolverInstanciado(a_TSS)) || (a_TSS == TipoSubproblemaSolver_mestre))
			return -1;

		const Periodo periodo_otimizacao = getAtributo(a_idStage, AttComumEstagio_periodo_otimizacao, Periodo());

		const Periodo periodEnd_stage = getIterador2Final(AttMatrizModeloOtimizacao_horizonte_estudo, a_idStage, Periodo());

		// Verifica se periodo não é mais necessario para compor ou repassar lag
		if (periodEnd_stage + 1 <= a_period_lag)
			return -1;

		const Periodo periodIni_stage = getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_estudo, a_idStage, Periodo());
		const Periodo periodIni_stageIni = getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_estudo, stageIni, Periodo());

		int varRE = getVarDecisao_REseExistir(a_TSS, a_idStage, a_period_lag, a_idRE);
		if (varRE > -1)
			return varRE;

		const double infinito = vetorEstagio.at(a_idStage).getSolver(a_TSS)->getInfinito();

		varRE = addVarDecisao_RE(a_TSS, a_idStage, a_period_lag, a_idRE, -infinito, infinito, 0.0);

		// ADD any compound of QD in the current stage.
		int equRE = -1;
		const double sobreposicao_periodo_otimizacao = periodo_otimizacao.sobreposicao(a_period_lag);
		if (sobreposicao_periodo_otimizacao > 0.0) {

			equRE = addEquLinear_RE(a_TSS, a_idStage, a_period_lag, a_idRE);
			vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(varRE, equRE, 1.0);

			bool sobreposicao_encontrada = false;
			for (Periodo period = periodIni_stage; period <= periodEnd_stage; a_horizon.incrementarIterador(period)) {

				const double sobreposicao = a_period_lag.sobreposicao(period);

				if (sobreposicao > 0.0) {
					int varRE_per = getVarDecisao_REseExistir(a_TSS, a_idStage, period, a_idRE);
					if (varRE_per == -1)
						varRE_per = addVarDecisao_RE(a_TSS, a_idStage, period, a_idRE, 0.0, infinito, 0.0);

					int equRE_per = getEquLinear_REseExistir(a_TSS, a_idStage, period, a_idRE);
					if (equRE_per == -1) {
						equRE_per = addEquLinear_RE(a_TSS, a_idStage, period, a_idRE);
						vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(varRE_per, equRE_per, 1.0);

						for (IdElementoSistema idElem = IdElementoSistema_1; idElem < a_dados.vetorRestricaoEletrica.at(a_idRE).vetorElementoSistema.getIdOut(); a_dados.vetorRestricaoEletrica.at(a_idRE).vetorElementoSistema.incr(idElem)) {

							a_dados.vetorRestricaoEletrica.at(a_idRE).vetorElementoSistema.at(idElem).validar();

							if (tryIncorporarRestricaoEletricaHidreletrica(a_TSS, a_dados, a_idStage, period, a_idRE, idElem, equRE_per))
								break;

							else if (tryIncorporarRestricaoEletricaTermeletrica(a_TSS, a_dados, a_idStage, period, a_idRE, idElem, equRE_per))
								break;

							else if (tryIncorporarRestricaoEletricaDemanda(a_TSS, a_dados, a_idStage, period, a_idRE, idElem, equRE_per))
								break;

							else if (tryIncorporarRestricaoEletricaRenovavel(a_TSS, a_dados, a_idStage, period, a_idRE, idElem, equRE_per))
								break;

							else if (tryIncorporarRestricaoEletricaContrato(a_TSS, a_dados, a_idStage, period, a_idRE, idElem, equRE_per))
								break;

							else if (tryIncorporarRestricaoEletricaIntercambio(a_TSS, a_dados, a_idStage, period, a_idRE, idElem, equRE_per))
								break;

							else if (tryIncorporarRestricaoEletricaUsinaElevatoria(a_TSS, a_dados, a_idStage, period, a_idRE, idElem, equRE_per))
								break;

							else
								throw std::invalid_argument("Invalid incorporation.");

						} // for (IdElementoSistema idElem = IdElementoSistema_1; idElem < a_dados.vetorRestricaoEletrica.at(a_idRE).vetorElementoSistema.getIdOut(); a_dados.vetorRestricaoEletrica.at(a_idRE).vetorElementoSistema.incr(idElem)) {

					} // if (equRE_per == -1) {

					if (sobreposicao < 1.0) {
						vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(varRE_per, equRE, -sobreposicao);
						if (!sobreposicao_encontrada)
							sobreposicao_encontrada = true;
					}
					else if (sobreposicao == 1.0)
						return varRE;

				}
				else if ((sobreposicao == 0.0) && (sobreposicao_encontrada))
					break;

			} // for (Periodo periodo = periodIni_stage; periodo <= periodEnd_stage; horiz_stage.incrementarIterador(periodo)) {

			if (a_period_lag >= periodIni_stage)
				return varRE;

		} // if (sobreposicao_periodo_otimizacao > 0.0) {

		// ADD any deterministic (past) compound of RE.
		// Only in stage where a_period belongs
		if (((a_period_lag < periodIni_stageIni) && (stageIni == a_idStage)) || (a_period_lag + 1 <= periodIni_stageIni)) {

			equRE = getEquLinear_REseExistir(a_TSS, a_idStage, a_period_lag, a_idRE);

			if (equRE == 0) {
				equRE = addEquLinear_RE(a_TSS, a_idStage, a_period_lag, a_idRE);
				vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(varRE, equRE, 1.0);
			}

			const double vlr_ini = a_dados.getAtributo(a_idRE, AttComumRestricaoEletrica_vlr_ini, double());

			const double vlr_min = getdoubleFromChar("min");
			const double vlr_max = getdoubleFromChar("max");

			if ((vlr_min >= vlr_ini) || (vlr_ini <= vlr_max))
				throw std::invalid_argument("Initial value not available");

			vetorEstagio.at(a_idStage).getSolver(a_TSS)->setRHSRestricao(equRE, vlr_ini);

			return varRE;

		} // if (((a_period_lag < periodIni_stageIni) && (stageIni == a_idStage)) || (a_period_lag + 1 <= periodIni_stageIni)) {

		// Recursively ADD any compound of RE in past stages.
		const IdEstagio idEstagio_anterior = IdEstagio(a_idStage - 1);

		int varRE_anterior = -1;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica) {
			for (TipoSubproblemaSolver tss = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tss < TipoSubproblemaSolver_Excedente; tss++) {
				if (tss != TipoSubproblemaSolver_viabilidade_hidraulica)
					varRE_anterior = criarVariaveisDecisao_VariaveisEstado_Restricoes_RE(tss, a_dados, idEstagio_anterior, a_period_lag, a_idRE, a_horizon, a_isVarEstadoExterna);
				if (varRE_anterior > -1)
					break;
			}
		}
		else
			varRE_anterior = criarVariaveisDecisao_VariaveisEstado_Restricoes_RE(a_TSS, a_dados, idEstagio_anterior, a_period_lag, a_idRE, a_horizon, a_isVarEstadoExterna);

		if (varRE_anterior > -1) {

			if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
				varRE_anterior = -1;

			Periodo periodAux = a_period_lag;

			// Variáveis de estado a compor lag
			if (sobreposicao_periodo_otimizacao > 0.0) {
				const int varRE_ADD = addVarDecisao_RE_ADD(a_TSS, a_idStage, a_period_lag, a_idRE, 0.0, infinito, 0.0);
				vetorEstagio.at(a_idStage).addVariavelEstado(a_TSS, criarRestricoesEletricas_nomeVarEstado(a_TSS, a_dados, a_idStage, a_period_lag, periodAux, a_idRE, true), varRE_ADD, varRE_anterior, a_isVarEstadoExterna);
				vetorEstagio.at(a_idStage).getSolver(a_TSS)->setCofRestricao(varRE_ADD, equRE, -1.0);
			}

			// Variáveis de estado a repassar lag
			else
				vetorEstagio.at(a_idStage).addVariavelEstado(a_TSS, criarRestricoesEletricas_nomeVarEstado(a_TSS, a_dados, a_idStage, a_period_lag, periodAux, a_idRE, true), varRE, varRE_anterior, a_isVarEstadoExterna);

		} // if (varQDLAG_anterior > -1){

		return varRE;

	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisDecisao_VariaveisEstado_Restricoes_RE(" + getFullString(a_TSS) + ",Dados& a_dados, " + getFullString(a_idStage) + "," + getFullString(a_period_lag) + "," + getFullString(a_idRE) + "): \n" + std::string(erro.what())); }

}



void ModeloOtimizacao::criarControleCotaVazao(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>& a_horizon) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_dados.vetorControleCotaVazao.numObjetos() == 0)
			return;

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const double vlr_min = getdoubleFromChar("min");
		const double vlr_max = getdoubleFromChar("max");

		const IdControleCotaVazao idConHQIni = a_dados.getMenorId(IdControleCotaVazao());
		const IdControleCotaVazao idConHQOut = a_dados.getIdOut(IdControleCotaVazao());

		for (IdControleCotaVazao idConHQ = idConHQIni; idConHQ < idConHQOut; a_dados.incr(idConHQ)) {

			const double penalidade = a_dados.getAtributo(idConHQ, AttComumControleCotaVazao_penalidade, double());
			int varHQ = getVarDecisao_HQseExistir(a_TSS, a_idEstagio, a_period, idConHQ);

			//
			// Variaveis e Restricao HQ = a + b * sum(QDEF)
			//
			if (varHQ == -1)
				varHQ = criarVariaveisDecisao_VariaveisEstado_Restricoes_HQ(a_TSS, a_dados, a_idEstagio, a_period, idConHQ, a_horizon);

			const SmartEnupla<Periodo, double> cota_anterior = a_dados.getVetor(idConHQ, AttVetorControleCotaVazao_cota_anterior, Periodo(), double());

			//
			// Restricoes var_abs_inf <= HQ - HQ_lag <= var_abs_sup para todos HQ_lag até lag_max
			//
			for (int i = a_dados.getIterador2Inicial(idConHQ, AttMatrizControleCotaVazao_num_horas_lag, a_period, int()); i <= a_dados.getIterador2Final(idConHQ, AttMatrizControleCotaVazao_num_horas_lag, a_period, int()); i++) {

				double var_abs_inf = vlr_min;
				if (a_dados.getSize1Matriz(idConHQ, AttMatrizControleCotaVazao_var_abs_inf) > 0)
					var_abs_inf = a_dados.getElementoMatriz(idConHQ, AttMatrizControleCotaVazao_var_abs_inf, a_period, i, double());

				double var_abs_sup = vlr_max;
				if (a_dados.getSize1Matriz(idConHQ, AttMatrizControleCotaVazao_var_abs_sup) > 0)
					var_abs_sup = a_dados.getElementoMatriz(idConHQ, AttMatrizControleCotaVazao_var_abs_sup, a_period, i, double());

				if ((vlr_min < var_abs_inf) || (var_abs_sup < vlr_max)) {

					const double num_horas_lag_max = a_dados.getElementoMatriz(idConHQ, AttMatrizControleCotaVazao_num_horas_lag, a_period, i, double());
					const int num_minutos_lag_max = int(num_horas_lag_max * 60);

					Periodo period_lag_max = Periodo(getString(a_period.getDuration()), Periodo("m", a_period) - num_minutos_lag_max);

					Periodo period_lag = a_period;
					if (num_minutos_lag_max > 0)
						a_horizon.decrementarIterador(period_lag);
					while (true) {

						// Restricao entra como absoluta no período atual
						int var_HQ_LAG = -1;
						if (num_minutos_lag_max > 0) {
							var_HQ_LAG = criarVariaveisDecisao_VariaveisEstado_Restricoes_HQ(a_TSS, a_dados, a_idEstagio, period_lag, idConHQ, a_horizon);
							if (var_HQ_LAG == -1)
								throw std::invalid_argument("Error getting HQ_LAG " + getFullString(period_lag) + " for " + getFullString(idConHQ) + " in " + getFullString(a_period) + " and lag " + getFullString(num_horas_lag_max));
						} // if (num_minutos_lag > 0) {

						if (vlr_min < var_abs_inf) {
							const int varHQ_VAINF_FINF = addVarDecisao_HQ_VAINF_FINF(a_TSS, a_idEstagio, a_period, period_lag, idConHQ, num_horas_lag_max, 0.0, infinito, 0.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varHQ_VAINF_FINF, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -penalidade);
							const int ineHQ_VAINF = addIneLinear_HQ_VAINF(a_TSS, a_idEstagio, a_period, period_lag, idConHQ, num_horas_lag_max);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineHQ_VAINF, var_abs_inf);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varHQ, ineHQ_VAINF, 1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varHQ_VAINF_FINF, ineHQ_VAINF, 1.0);
							if (var_HQ_LAG > -1)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(var_HQ_LAG, ineHQ_VAINF, -1.0);
						}

						if (var_abs_sup < vlr_max) {
							const int varHQ_VASUP_FSUP = addVarDecisao_HQ_VASUP_FSUP(a_TSS, a_idEstagio, a_period, period_lag, idConHQ, num_horas_lag_max, 0.0, infinito, 0.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varHQ_VASUP_FSUP, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -penalidade);
							const int ineHQ_VASUP = addIneLinear_HQ_VASUP(a_TSS, a_idEstagio, a_period, period_lag, idConHQ, num_horas_lag_max);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(ineHQ_VASUP, -var_abs_sup);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varHQ, ineHQ_VASUP, -1.0);
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varHQ_VASUP_FSUP, ineHQ_VASUP, 1.0);
							if (var_HQ_LAG > -1)
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(var_HQ_LAG, ineHQ_VASUP, 1.0);
						}

						if (num_minutos_lag_max == 0)
							break;
						else if (((period_lag < period_lag_max) && (period_lag.sobreposicao(period_lag_max) == 0.0)) || (period_lag == period_lag_max))
							break;
						else if (period_lag > a_horizon.getIteradorInicial())
							a_horizon.decrementarIterador(period_lag);
						else if (period_lag == a_horizon.getIteradorInicial())
							period_lag = cota_anterior.getIteradorFinal();
						else
							cota_anterior.decrementarIterador(period_lag);

					} // while (true) {

				} // if ((vlr_min < var_abs_inf) || (var_abs_sup < vlr_max)) {
			} // for (int i = a_dados.getIterador2Inicial(idConHQ, AttMatrizControleCotaVazao_num_horas_lag, a_period, int()); i <= a_dados.getIterador2Final(idConHQ, AttMatrizControleCotaVazao_num_horas_lag, a_period, int()); i++) {

		}// for (IdControleCotaVazao idConHQ = idConHQIni; idConHQ < idConHQOut; a_dados.incr(idConHQ)) {


	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarControleCotaVazao(" + getFullString(a_TSS) + ",Dados& a_dados, " + getFullString(a_idEstagio) + "," + getFullString(a_period) + "): \n" + std::string(erro.what())); }
}
//void ModeloOtimizacao::criarReservaPotencia(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados)





void ModeloOtimizacao::criarReservaPotencia(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat){

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const IdReservaPotencia idPRESIni = a_dados.getMenorId(IdReservaPotencia());
		const IdReservaPotencia idPRESOut = a_dados.getIdOut(IdReservaPotencia());

		if (idPRESIni > IdReservaPotencia_Nenhum)
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->atualizar();

		for (IdReservaPotencia idPRES = idPRESIni; idPRES < idPRESOut; a_dados.incr(idPRES)) {

			///////////////////////////////////////////////////////////////////
			//Restrições Reserva de Potência (RE)
			///////////////////////////////////////////////////////////////////

			//******************************************
			//Formulacao: PRES >= reserva_minima
			//            PRES = FP*PRES_elm1 + FP*PRES_elm2 + ...
			//******************************************

			const int varPRES = criarReservaPotencia(a_TSS, a_dados, a_idEstagio, a_period, a_idPat, idPRES);

		}// for (IdReservaPotencia idPRES = idPRESIni; idPRES < idPRESOut; a_dados.incr(idPRES)) {

	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarReservaPotencia(" + getFullString(a_TSS) + ",Dados& a_dados, " + getFullString(a_idEstagio) + "," + getFullString(a_period) + "," + getFullString(a_idPat) + "): \n" + std::string(erro.what())); }


}


int ModeloOtimizacao::criarReservaPotencia(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat, const IdReservaPotencia a_idPRES){

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return -1;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return -1;

		int equPRES_pat = -1;
		int varPRES_pat = -1;

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const IdPatamarCarga idPatEnd = a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, a_period, IdPatamarCarga());
		const double perc_dur_pat = a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, a_idPat, double());

		equPRES_pat = getEquLinear_PRESseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idPRES);
		if (equPRES_pat > -1) { return getVarDecisao_PRES(a_TSS, a_idEstagio, a_period, a_idPat, a_idPRES); }
		equPRES_pat = addEquLinear_PRES(a_TSS, a_idEstagio, a_period, a_idPat, a_idPRES);
		varPRES_pat = addVarDecisao_PRES(a_TSS, a_idEstagio, a_period, a_idPat, a_idPRES, a_dados.getElementoMatriz(a_idPRES, AttMatrizReservaPotencia_reserva_minima, a_period, a_idPat, double()), infinito, 0.0);
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPRES_pat, equPRES_pat, 1.0);
		const int inePRES_pat = addIneLinear_PRES(a_TSS, a_idEstagio, a_period, a_idPat, a_idPRES);
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPRES_pat, inePRES_pat, 1.0);
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(inePRES_pat, a_dados.getElementoMatriz(a_idPRES, AttMatrizReservaPotencia_reserva_minima, a_period, a_idPat, double()));

		if (idPatEnd > IdPatamarCarga_1) {

			int varPRES = getVarDecisao_PRESseExistir(a_TSS, a_idEstagio, a_period, a_idPRES);
			if (varPRES == -1)
				varPRES = addVarDecisao_PRES(a_TSS, a_idEstagio, a_period, a_idPRES, 0.0, infinito, 0.0);
			int equPRES = getEquLinear_PRESseExistir(a_TSS, a_idEstagio, a_period, a_idPRES);
			if (equPRES == -1) {
				equPRES = addEquLinear_PRES(a_TSS, a_idEstagio, a_period, a_idPRES);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPRES, equPRES, 1.0);
			}
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPRES_pat, equPRES, -perc_dur_pat);

		} // if (idPatEnd > IdPatamarCarga_1) {

		const IdElementoSistema idElemEnd = a_dados.getMaiorId(a_idPRES, IdElementoSistema());
		for (IdElementoSistema idElem = IdElementoSistema_1; idElem <= idElemEnd; idElem++) {
			const TipoVariavelRestricaoOperativa tipoVarPRES = a_dados.getAtributo(a_idPRES, idElem, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

			if ((tipoVarPRES!= TipoVariavelRestricaoOperativa_potencia) && (tipoVarPRES!= TipoVariavelRestricaoOperativa_potencia_disponivel))
				throw std::invalid_argument("Tipo invalido");

			const double fator_participacao = a_dados.getElementoMatriz(a_idPRES, idElem, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double());

			const IdHidreletrica       idUHE = a_dados.getAtributo(a_idPRES, idElem, AttComumElementoSistema_hidreletrica, IdHidreletrica());
			const IdConjuntoHidraulico idConUHE = a_dados.getAtributo(a_idPRES, idElem, AttComumElementoSistema_conjuntohidraulico, IdConjuntoHidraulico());
			const IdUnidadeUHE         idUniUHE = a_dados.getAtributo(a_idPRES, idElem, AttComumElementoSistema_unidadehidraulica, IdUnidadeUHE());
			const IdTermeletrica       idUTE = a_dados.getAtributo(a_idPRES, idElem, AttComumElementoSistema_termeletrica, IdTermeletrica());
			const IdUnidadeUTE         idUniUTE = a_dados.getAtributo(a_idPRES, idElem, AttComumElementoSistema_unidadeTermeletrica, IdUnidadeUTE());
			const IdIntercambio        idINT = a_dados.getAtributo(a_idPRES, idElem, AttComumElementoSistema_intercambio, IdIntercambio());
			const IdRenovavel          idREN = a_dados.getAtributo(a_idPRES, idElem, AttComumElementoSistema_renovavel, IdRenovavel());
			const IdSubmercado         idDEM = a_dados.getAtributo(a_idPRES, idElem, AttComumElementoSistema_demanda, IdSubmercado());
			const IdSubmercado         idDEMLIQ = a_dados.getAtributo(a_idPRES, idElem, AttComumElementoSistema_demandaliquida, IdSubmercado());

			int varPRES_elem_pat = getVarDecisao_PRESseExistir(a_TSS, a_idEstagio, a_period, a_idPat, a_idPRES, idElem);
			if (varPRES_elem_pat == -1) {
				int varPot = -1;
				// Hidreletrica
				if ((idUHE != IdHidreletrica_Nenhum) && (idConUHE == IdConjuntoHidraulico_Nenhum) && (idUniUHE == IdUnidadeUHE_Nenhum) && (idUTE == IdTermeletrica_Nenhum) && (idUniUTE == IdUnidadeUTE_Nenhum) && (idINT == IdIntercambio_Nenhum) && (idREN == IdRenovavel_Nenhum) && (idDEM == IdSubmercado_Nenhum) && (idDEMLIQ == IdSubmercado_Nenhum)) {
					if (tipoVarPRES == TipoVariavelRestricaoOperativa_potencia_disponivel)
						varPot = getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE);
					else if (tipoVarPRES == TipoVariavelRestricaoOperativa_potencia)
						varPot = getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE);
				}

				// ConjuntoHidraulico
				else if ((idUHE != IdHidreletrica_Nenhum) && (idConUHE != IdConjuntoHidraulico_Nenhum) && (idUniUHE == IdUnidadeUHE_Nenhum) && (idUTE == IdTermeletrica_Nenhum) && (idUniUTE == IdUnidadeUTE_Nenhum) && (idINT == IdIntercambio_Nenhum) && (idREN == IdRenovavel_Nenhum) && (idDEM == IdSubmercado_Nenhum) && (idDEMLIQ == IdSubmercado_Nenhum)) {
					if (tipoVarPRES == TipoVariavelRestricaoOperativa_potencia_disponivel)
						varPot = getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConUHE);
					else if (tipoVarPRES == TipoVariavelRestricaoOperativa_potencia)
						varPot = getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConUHE);
				}

				// UnidadeUHE
				else if ((idUHE != IdHidreletrica_Nenhum) && (idConUHE != IdConjuntoHidraulico_Nenhum) && (idUniUHE != IdUnidadeUHE_Nenhum) && (idUTE == IdTermeletrica_Nenhum) && (idUniUTE == IdUnidadeUTE_Nenhum) && (idINT == IdIntercambio_Nenhum) && (idREN == IdRenovavel_Nenhum) && (idDEM == IdSubmercado_Nenhum) && (idDEMLIQ == IdSubmercado_Nenhum)) {
					if (tipoVarPRES == TipoVariavelRestricaoOperativa_potencia_disponivel)
						varPot = getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConUHE, idUniUHE);
					else if (tipoVarPRES == TipoVariavelRestricaoOperativa_potencia)
						varPot = getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, idConUHE, idUniUHE);
				}

				// Termeletrica
				else if ((idUHE == IdHidreletrica_Nenhum) && (idConUHE == IdConjuntoHidraulico_Nenhum) && (idUniUHE == IdUnidadeUHE_Nenhum) && (idUTE != IdTermeletrica_Nenhum) && (idUniUTE == IdUnidadeUTE_Nenhum) && (idINT == IdIntercambio_Nenhum) && (idREN == IdRenovavel_Nenhum) && (idDEM == IdSubmercado_Nenhum) && (idDEMLIQ == IdSubmercado_Nenhum)) {
					if (tipoVarPRES == TipoVariavelRestricaoOperativa_potencia_disponivel)
						varPot = getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);
					else if (tipoVarPRES == TipoVariavelRestricaoOperativa_potencia)
						varPot = getVarDecisao_PTseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE);
				}

				// Unidade UTE Termeletrica
				else if ((idUHE == IdHidreletrica_Nenhum) && (idConUHE == IdConjuntoHidraulico_Nenhum) && (idUniUHE == IdUnidadeUHE_Nenhum) && (idUTE != IdTermeletrica_Nenhum) && (idUniUTE != IdUnidadeUTE_Nenhum) && (idINT == IdIntercambio_Nenhum) && (idREN == IdRenovavel_Nenhum) && (idDEM == IdSubmercado_Nenhum) && (idDEMLIQ == IdSubmercado_Nenhum)) {
					if (tipoVarPRES == TipoVariavelRestricaoOperativa_potencia_disponivel)
						varPot = getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUniUTE);
					else if (tipoVarPRES == TipoVariavelRestricaoOperativa_potencia)
						varPot = getVarDecisao_PTseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUTE, idUniUTE);
				}

				// Intercambio
				else if ((idUHE == IdHidreletrica_Nenhum) && (idConUHE == IdConjuntoHidraulico_Nenhum) && (idUniUHE == IdUnidadeUHE_Nenhum) && (idUTE == IdTermeletrica_Nenhum) && (idUniUTE == IdUnidadeUTE_Nenhum) && (idINT != IdIntercambio_Nenhum) && (idREN == IdRenovavel_Nenhum) && (idDEM == IdSubmercado_Nenhum) && (idDEMLIQ == IdSubmercado_Nenhum)) {
					const IdSubmercado idSSE_orig = a_dados.getAtributo(idINT, AttComumIntercambio_submercado_origem, IdSubmercado());
					const IdSubmercado idSSE_dest = a_dados.getAtributo(idINT, AttComumIntercambio_submercado_destino, IdSubmercado());
					varPot = getVarDecisao_PI(a_TSS, a_idEstagio, a_period, a_idPat, idINT, idSSE_orig, idSSE_dest);
				}

				// Renovavel
				else if ((idUHE == IdHidreletrica_Nenhum) && (idConUHE == IdConjuntoHidraulico_Nenhum) && (idUniUHE == IdUnidadeUHE_Nenhum) && (idUTE == IdTermeletrica_Nenhum) && (idUniUTE == IdUnidadeUTE_Nenhum) && (idINT == IdIntercambio_Nenhum) && (idREN != IdRenovavel_Nenhum) && (idDEM == IdSubmercado_Nenhum) && (idDEMLIQ == IdSubmercado_Nenhum)) {
					varPot = getVarDecisao_PRseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idREN);
				}

				// Demanda
				else if ((idUHE == IdHidreletrica_Nenhum) && (idConUHE == IdConjuntoHidraulico_Nenhum) && (idUniUHE == IdUnidadeUHE_Nenhum) && (idUTE == IdTermeletrica_Nenhum) && (idUniUTE == IdUnidadeUTE_Nenhum) && (idINT == IdIntercambio_Nenhum) && (idREN == IdRenovavel_Nenhum) && (idDEM != IdSubmercado_Nenhum) && (idDEMLIQ == IdSubmercado_Nenhum)) {
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PL(a_TSS, a_idEstagio, a_period, a_idPat, idDEM), inePRES_pat, -fator_participacao);
				}

				// Demanda Liquida
				else if ((idUHE == IdHidreletrica_Nenhum) && (idConUHE == IdConjuntoHidraulico_Nenhum) && (idUniUHE == IdUnidadeUHE_Nenhum) && (idUTE == IdTermeletrica_Nenhum) && (idUniUTE == IdUnidadeUTE_Nenhum) && (idINT == IdIntercambio_Nenhum) && (idREN == IdRenovavel_Nenhum) && (idDEM == IdSubmercado_Nenhum) && (idDEMLIQ != IdSubmercado_Nenhum)) {
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PL_LIQ(a_TSS, a_idEstagio, a_period, a_idPat, idDEMLIQ), inePRES_pat, -fator_participacao);
				}

				else
					throw std::invalid_argument("Combinacao invalida para PRES em " + getFullString(idElem));

				if (varPot > -1) {
					varPRES_elem_pat = addVarDecisao_PRES(a_TSS, a_idEstagio, a_period, a_idPat, a_idPRES, idElem, 0.0, infinito, 0.0);
					const int equPRES_elem_pat = addEquLinear_PRES(a_TSS, a_idEstagio, a_period, a_idPat, a_idPRES, idElem);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPRES_elem_pat, equPRES_elem_pat, 1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPot, equPRES_elem_pat, 1.0);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(equPRES_elem_pat, vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getLimSuperior(varPot));
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPRES_elem_pat, equPRES_pat, -fator_participacao);
				}
			}
		} // for (IdElementoSistema idElem = IdElementoSistema_1; idElem <= idElemEnd; idElem++) {

		return varPRES_pat;

	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarReservaPotencia(" + getFullString(a_TSS) + ",Dados& a_dados, " + getFullString(a_idEstagio) + "," + getFullString(a_period) + "," + getFullString(a_idPat) + "," + getFullString(a_idPRES) + "): \n" + std::string(erro.what())); }


}



void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_vazao_afluente(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period)
{

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const std::vector<IdRestricaoOperativaUHE> idRestricaoOperativaUHE_vazao_afluente = a_dados.vetorRestricaoOperativaUHE.getIdObjetos(AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica_vazao_afluente);

		const int numero_restricoes = int(idRestricaoOperativaUHE_vazao_afluente.size());

		for (int pos = 0; pos < numero_restricoes; pos++) {

			const IdRestricaoOperativaUHE idRestricaoOperativaUHE = idRestricaoOperativaUHE_vazao_afluente.at(pos);


			// Vazão Afluente
			//
			if (a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_vazao_afluente) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_inf, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("min")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum) {
						const int varRHA_FINF = addVarDecisao_RHA_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE, 0.0, infinito, 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRHA_FINF, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_penalidade, double()));
					}

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_inf, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("min")) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_sup, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("max")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum) {
						const int varRHA_FSUP = addVarDecisao_RHA_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE, 0.0, infinito, 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRHA_FSUP, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_penalidade, double()));
					}

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_sup, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("max")) {
			}

			//////////////////////////////////////////////////////////////////////////////////////////
			//Restrição Hidráulica de Vazão Afluente (RHA)
			//Manual referencia DC Seção 5.2.5.2:
			//Amin <= Aflu + (Qmontante+Smontante) + Qdesvio <= Amax
			//Filosofia: cria uma restrição por periodo se existir um valor diferente a max no limite
			//////////////////////////////////////////////////////////////////////////////////////////

			//******************************************
			//Restrição <=
			//******************************************

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_inf, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("min")) {

				if (getVarDecisao_RHA_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

					const int posIneRHA = addIneLinear_RHA_LINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHA, a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_inf, a_period, IdPatamarCarga_1, double()));

					// Variável de folga RHA_FINF				
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHA_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), posIneRHA, 1.0);

					const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
					for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

						const IdHidreletrica	idUHE = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());

						const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, a_period, IdPatamarCarga());

						if (idUHE != IdHidreletrica_Nenhum) {

							// Parcela QINC vazão incremental							
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QINC(a_TSS, a_idEstagio, a_period, idUHE), posIneRHA, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double()));


							// Parcela QDEF (Turbinamento + Vertimento das usinas hidrelétricas a montante)

							const int numero_hidreletricas_montante = a_dados.getSizeVetor(idUHE, AttVetorHidreletrica_montante);

							for (int m = 1; m <= numero_hidreletricas_montante; m++) {

								const IdHidreletrica idHidreletrica_montante = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_montante, m, IdHidreletrica());

								for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

									const double duracao_patamar = a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double());

									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, idPat, idHidreletrica_montante), posIneRHA, duracao_patamar * a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, idPat, double()));

								}//// for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

							} // for (int m = 1; m <= numero_hidreletricas_montante; m++) {

							//QDES
							// Variáveis de Hidreletricas à Montante desvio
							const int numero_hidreletricas_montante_desvio = a_dados.getSizeVetor(idUHE, AttVetorHidreletrica_montante_desvio);
							for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

								const IdHidreletrica idHidreletrica_montante_desvio = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_montante_desvio, m, IdHidreletrica());

								// Variável QDES que "entra" na usina

								for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

									const double duracao_patamar = a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double());
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_period, idPat, idHidreletrica_montante_desvio, idUHE), posIneRHA, duracao_patamar * a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, idPat, double()));

								}//for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

							}//for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

						} // if (idUHE != IdHidreletrica_Nenhum) {
						else
							throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

					} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

				} // if (getVarDecisao_RHA_FINF(a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_inf, a_period, IdPatamarCarga_1, double()) > 0) {

			//******************************************
			//Restrição >=
			//******************************************

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_sup, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("max")) {

				if (getVarDecisao_RHA_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

					const int posIneRHA = addIneLinear_RHA_LSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHA, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_sup, a_period, IdPatamarCarga_1, double()));

					// Variável de folga RHA_FSUP					
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHA_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), posIneRHA, 1.0);

					const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
					for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

						const IdHidreletrica	idUHE = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());

						const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, a_period, IdPatamarCarga());

						if (idUHE != IdHidreletrica_Nenhum) {

							// Parcela QINC vazão incremental
							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QINC(a_TSS, a_idEstagio, a_period, idUHE), posIneRHA, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double()));

							// Parcela QDEF (Turbinamento + Vertimento das usinas hidrelétricas a montante)

							const int numero_hidreletricas_montante = a_dados.getSizeVetor(idUHE, AttVetorHidreletrica_montante);

							for (int m = 1; m <= numero_hidreletricas_montante; m++) {

								const IdHidreletrica idHidreletrica_montante = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_montante, m, IdHidreletrica());

								for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

									const double duracao_patamar = a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double());

									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, idPat, idHidreletrica_montante), posIneRHA, -duracao_patamar * a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, idPat, double()));

								}//// for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

							} // for (int m = 1; m <= numero_hidreletricas_montante; m++) {

							//QDES
							// Variáveis de Hidreletricas à Montante desvio
							const int numero_hidreletricas_montante_desvio = a_dados.getSizeVetor(idUHE, AttVetorHidreletrica_montante_desvio);
							for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

								const IdHidreletrica idHidreletrica_montante_desvio = a_dados.getElementoVetor(idUHE, AttVetorHidreletrica_montante_desvio, m, IdHidreletrica());

								// Variável QDES que "entra" na usina

								for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

									const double duracao_patamar = a_dados.getElementoMatriz(AttMatrizDados_horizonte_estudo, a_period, idPat, double());
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_period, idPat, idHidreletrica_montante_desvio, idUHE), posIneRHA, -duracao_patamar * a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, idPat, double()));

								}//for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

							}//for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

						} // if (idUHE != IdHidreletrica_Nenhum) {
						else
							throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

					} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

				} // if (getVarDecisao_RHA_FSUP(a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_sup, a_period, IdPatamarCarga_1, double()) < getdoubleFromChar("max")) {

		}// for (int pos = 0; pos < numero_restricoes; pos++) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesHidraulicaEspecial_vazao_afluente(" + getFullString(a_TSS) + ",Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados): \n" + std::string(erro.what())); }

}//void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_vazao_afluente(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados)


void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_vazao_defluente(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_period, const IdPatamarCarga a_idPat) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const std::vector<IdRestricaoOperativaUHE> idRestricaoOperativaUHE_vazao_defluente = a_dados.vetorRestricaoOperativaUHE.getIdObjetos(AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica_vazao_defluente);

		const int numero_restricoes = int(idRestricaoOperativaUHE_vazao_defluente.size());

		for (int pos = 0; pos < numero_restricoes; pos++) {

			const IdRestricaoOperativaUHE idRestricaoOperativaUHE = idRestricaoOperativaUHE_vazao_defluente.at(pos);

			// Vazão Defluente
//
			if (a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_vazao_defluente) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_inf, a_period, a_idPat, double()) != getdoubleFromChar("min")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum) {
						const int RHQ_FINF = addVarDecisao_RHQ_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE, 0.0, infinito, 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(RHQ_FINF, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_penalidade, double()));
					}

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_inf, a_period, a_idPat, double()) != 0.0) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_sup, a_period, a_idPat, double()) != getdoubleFromChar("max")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum) {
						const int RHQ_FSUP = addVarDecisao_RHQ_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE, 0.0, infinito, 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(RHQ_FSUP, getEquLinear_ZP(a_TSS, a_idEstagio, a_period, a_idPat), -a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_penalidade, double()));
					}

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_sup, a_period, a_idPat, double()) != getdoubleFromChar("max")) {

			}//if (a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_vazao_afluente) {	

			///////////////////////////////////////////////////////////////////
			//Restrição Hidráulica de Vazão Defluente (RHQ)
			//Manual referencia Seção 5.2.5.2:
			//Qmin <= Qdefluencia + Qdesvio + Qbombeada <= Qmax
			///////////////////////////////////////////////////////////////////


			//******************************************
			//Restrição <=
			//******************************************

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_inf, a_period, a_idPat, double()) != getdoubleFromChar("min")) {

				if (getVarDecisao_RHQ_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE) > -1) {

					const int posIneRHQ = addIneLinear_RHQ_LINF(a_TSS, a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHQ, a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_inf, a_period, a_idPat, double()));

					// Variável RHQ_FINF
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHQ_FINF(a_TSS, a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE), posIneRHQ, 1.0);

					const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
					for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

						const IdHidreletrica       idUHE = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
						const IdUsinaElevatoria idUsinaElevatoria = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());

						const TipoVariavelRestricaoOperativa tipoVariavelRestricaoOperativa = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

						if (idUHE != IdHidreletrica_Nenhum) {

							// Variável QDEF (Vazão defluente das usinas hidrelétricas)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_defluente) {

								if (a_dados.getAtributo(AttComumDados_representar_defluencia_disponivel_em_restricoes_hidraulicas, bool()))
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posIneRHQ, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));
								else
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posIneRHQ, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_defluente) {

							// Variável QTUR
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_turbinada) {

								if (a_dados.getAtributo(AttComumDados_representar_turbinamento_disponivel_em_restricoes_hidraulicas, bool())) {
									if (getVarDecisao_QTURDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posIneRHQ, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));
								}
								else {
									if (getVarDecisao_QTURseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posIneRHQ, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));
								}

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_defluente) {

							// Variável QDES
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_desviada) {

								// Variável QDES que "sai" na usina
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, a_dados.getAtributo(idUHE, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posIneRHQ, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_desviada) {

						} // if (idUHE != IdHidreletrica_Nenhum) {
						else if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {

							// Variável QBOM (Vazao bombeada)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_bombeada) {

								if (getVarDecisao_QBOMseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria) > -1)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria), posIneRHQ, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_bombeado) {

						}//else if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {
						else
							throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

					} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

				} // if (getVarDecisao_RHQseExistir(a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE) > -1) {

			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_inf, a_period, a_idPat, double()) > 0) {

			//******************************************
			//Restrição >=
			//******************************************

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_sup, a_period, a_idPat, double()) != getdoubleFromChar("max")) {

				if (getVarDecisao_RHQ_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE) > -1) {

					const int posIneRHQ = addIneLinear_RHQ_LSUP(a_TSS, a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHQ, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_sup, a_period, a_idPat, double()));

					// Variável RHQ_FSUP
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHQ_FSUP(a_TSS, a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE), posIneRHQ, 1.0);

					const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
					for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

						const IdHidreletrica       idUHE = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
						const IdUsinaElevatoria idUsinaElevatoria = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());

						const TipoVariavelRestricaoOperativa tipoVariavelRestricaoOperativa = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

						if (idUHE != IdHidreletrica_Nenhum) {

							// Variável QDEF (Vazão defluente das usinas hidrelétricas)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_defluente) {

								if (a_dados.getAtributo(AttComumDados_representar_defluencia_disponivel_em_restricoes_hidraulicas, bool()))
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posIneRHQ, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));
								else
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posIneRHQ, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_defluente) {

							// Variável QTUR
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_turbinada) {

								if (a_dados.getAtributo(AttComumDados_representar_turbinamento_disponivel_em_restricoes_hidraulicas, bool())) {
									if (getVarDecisao_QTURDISPseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posIneRHQ, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));
								}
								else {
									if (getVarDecisao_QTURseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_period, a_idPat, idUHE), posIneRHQ, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));
								}
							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_defluente) {

							// Variável QDES
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_desviada) {

								//QDES
								vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_period, a_idPat, idUHE, a_dados.getAtributo(idUHE, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posIneRHQ, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_desviada) {

						} // if (idUHE != IdHidreletrica_Nenhum) {
						else if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {

							// Variável QBOM (Vazão bombeada)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_bombeada) {

								if (getVarDecisao_QBOMseExistir(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria) > -1)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_period, a_idPat, idUsinaElevatoria), posIneRHQ, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, a_idPat, double()));

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_bombeado) {

						}//else if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {
						else
							throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

					} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

				} // if (getVarDecisao_RHQseExistir(a_idEstagio, a_period, a_idPat, idRestricaoOperativaUHE) > -1) {

			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_sup, a_period, a_idPat, double()) > getdoubleFromChar("max")) {


		}// for (int pos = 0; pos < numero_restricoes; pos++) {

	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesHidraulicaEspecial_vazao_defluente(" + getFullString(a_TSS) + ",Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados): \n" + std::string(erro.what())); }

}//void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_vazao_defluente(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados)


void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_volume_armazenado(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodIni_stage, Periodo& a_periodEnd_stage, Periodo &a_period, Periodo& a_periodNext, const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>& a_horizon) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const std::vector<IdRestricaoOperativaUHE> idRestricaoOperativaUHE_volume_armazenado = a_dados.vetorRestricaoOperativaUHE.getIdObjetos(AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica_volume_armazenado);

		const int numero_restricoes = int(idRestricaoOperativaUHE_volume_armazenado.size());

		for (int pos = 0; pos < numero_restricoes; pos++) {

			const IdRestricaoOperativaUHE idRestricaoOperativaUHE = idRestricaoOperativaUHE_volume_armazenado.at(pos);


			// Volume armazenado
			//
			if (a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_volume_armazenado) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_inf, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("min")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum) {
						const int varRHV_FINF = addVarDecisao_RHV_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE, 0.0, infinito, 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRHV_FINF, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_penalidade, double()));
					}

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_inf, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("min")) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_sup, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("max")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum) {
						const int varRHV_FSUP = addVarDecisao_RHV_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE, 0.0, infinito, 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRHV_FSUP, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_penalidade, double()));
					}

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_sup, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("max")) {

			}//if (a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_volume_armazenado) {						



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

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_inf, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("min")) {

				if (getVarDecisao_RHV_FINFseExistir(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

					const int posIneRHV = addIneLinear_RHV_LINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHV, a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_inf, a_period, IdPatamarCarga_1, double()));

					//Variável RHV_FINF				
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHV_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), posIneRHV, 1.0);

					const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
					for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

						const IdHidreletrica       idUHE = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
						const IdUsinaElevatoria idUsinaElevatoria = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());

						const TipoVariavelRestricaoOperativa tipoVariavelRestricaoOperativa = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

						if (idUHE != IdHidreletrica_Nenhum) {

							// Variável VI ou VF (Volume armazenado nos reservatórios)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_final) {

								if (a_period == a_periodEnd_stage) {

									if (getVarDecisao_VF(a_TSS, a_idEstagio, a_period, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_period, idUHE), posIneRHV, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double()));

								}//if (a_period == a_horizonte_estudo.getIteradorFinal()) {
								else {

									if (getVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, idUHE), posIneRHV, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double()));

								}//else {

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_final) {

							//Nota: A vazão defluente/desviada é o somatório de todas as vazões por periodo dentro de um estágio_DC

							// Variável QDEF (Volume defluente das usinas hidrelétricas)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_defluente) {

								for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizon.incrementarIterador(periodo_auxiliar)) {

									const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, periodo_auxiliar, IdPatamarCarga());

									for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, periodo_auxiliar, idPat, idUHE), posIneRHV, a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPat, double()));

									}//// for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

								}//for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizonte_estudo.incrementarIterador(periodo_auxiliar)) {																													

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_defluente) {

							// Variável QDES (Volume desviado)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_desviado) {

								for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizon.incrementarIterador(periodo_auxiliar)) {

									const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, periodo_auxiliar, IdPatamarCarga());

									for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

										//QDES
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_period, idPat, idUHE, a_dados.getAtributo(idUHE, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posIneRHV, a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPat, double()));

									}//// for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

								}//for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizonte_estudo.incrementarIterador(periodo_auxiliar)) {																													

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_desviado) {

						} // if (idUHE != IdHidreletrica_Nenhum) {
						else if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {

							//Nota: A vazão bombeada é o somatório de todas as vazões por periodo dentro de um estágio_DC

							// Variável QBOM (Volume bombeado)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_bombeado) {

								for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizon.incrementarIterador(periodo_auxiliar)) {

									const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, periodo_auxiliar, IdPatamarCarga());

									for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

										if (getVarDecisao_QBOMseExistir(a_TSS, a_idEstagio, periodo_auxiliar, idPat, idUsinaElevatoria) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, periodo_auxiliar, idPat, idUsinaElevatoria), posIneRHV, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, periodo_auxiliar, idPat, double()) * a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPat, double()));

									}//// for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

								}//for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizonte_estudo.incrementarIterador(periodo_auxiliar)) {																													

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_desviado) {

						}//if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {
						else
							throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

					} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

				} // if (getVarDecisao_RHVseExistir(a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_inf, a_period, IdPatamarCarga_1, double()) > 0) {

			//******************************************
			//Restrição >=
			//******************************************

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_sup, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("max")) {

				if (getVarDecisao_RHV_FSUPseExistir(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

					const int posIneRHV = addIneLinear_RHV_LSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHV, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_sup, a_period, IdPatamarCarga_1, double()));

					// Variável RHV
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHV_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), posIneRHV, 1.0);

					const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
					for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

						const IdHidreletrica       idUHE = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
						const IdUsinaElevatoria idUsinaElevatoria = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());

						const TipoVariavelRestricaoOperativa tipoVariavelRestricaoOperativa = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

						if (idUHE != IdHidreletrica_Nenhum) {

							// Variável VF ou VI (Volume armazenado nos reservatórios)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_final) {

								if (a_period == a_periodEnd_stage) {

									if (getVarDecisao_VF(a_TSS, a_idEstagio, a_period, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_period, idUHE), posIneRHV, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double()));

								}//if (a_period == a_horizonte_estudo.getIteradorFinal()) {
								else {

									if (getVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, idUHE), posIneRHV, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double()));

								}//else {
							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_final) {

							//Nota: A vazão defluente/desviada é o somatório de todas as vazões por periodo dentro de um estágio_DC

							// Variável QDEF (Volume defluente das usinas hidrelétricas)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_defluente) {

								for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizon.incrementarIterador(periodo_auxiliar)) {

									const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, periodo_auxiliar, IdPatamarCarga());

									for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, periodo_auxiliar, idPat, idUHE), posIneRHV, -a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPat, double()));

									}//// for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

								}//for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizonte_estudo.incrementarIterador(periodo_auxiliar)) {																													

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_defluente) {

							// Intercambios Hidráulicos (Volume desviado)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_desviado) {

								for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizon.incrementarIterador(periodo_auxiliar)) {

									const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, periodo_auxiliar, IdPatamarCarga());

									for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

										//QDES
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_period, idPat, idUHE, a_dados.getAtributo(idUHE, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posIneRHV, -a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPat, double()));

									}//// for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

								}//for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizonte_estudo.incrementarIterador(periodo_auxiliar)) {																													

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_desviado) {

						} // if (idUHE != IdHidreletrica_Nenhum) {
						else if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {

							//Nota: A vazão bombeada é o somatório de todas as vazões por periodo dentro de um estágio_DC

							// Variável QBOM (Volume bombeado)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_bombeado) {

								for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizon.incrementarIterador(periodo_auxiliar)) {

									const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_horizonte_estudo, periodo_auxiliar, IdPatamarCarga());

									for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

										if (getVarDecisao_QBOMseExistir(a_TSS, a_idEstagio, periodo_auxiliar, idPat, idUsinaElevatoria) > -1)
											vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, periodo_auxiliar, idPat, idUsinaElevatoria), posIneRHV, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, periodo_auxiliar, idPat, double()) * a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPat, double()));

									}//// for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= maiorIdPatamarCarga; idPat++) {

								}//for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_period; a_horizonte_estudo.incrementarIterador(periodo_auxiliar)) {																													

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_desviado) {

						}//if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {
						else
							throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

					} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

				} // if (getVarDecisao_RHVseExistir(a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_sup, a_period, IdPatamarCarga_1, double()) < getdoubleFromChar("max")) {

			//Atualiza o periodo onde começa o estágio da leitura dos dados
			if (a_period < a_periodEnd_stage) {//Signica um periodo que corresponde a o fim de um estagio_DC mas não do estagio_SPT
				periodo_inicial_dos_dados = a_period;
				a_horizon.incrementarIterador(periodo_inicial_dos_dados);
			}//if (a_period < a_horizonte_estudo.getIteradorFinal()) {	

		}// for (int pos = 0; pos < numero_restricoes; pos++) {

	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesHidraulicaEspecial_volume_armazenado(" + getFullString(a_TSS) + ",Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados): \n" + std::string(erro.what())); }

}//void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_volume_armazenado(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados)

void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_energia_armazenada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodEnd_stage, Periodo &a_period, Periodo& a_periodNext) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		const double infinito = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

		const std::vector<IdRestricaoOperativaUHE> idRestricaoOperativaUHE_energia_armazenada = a_dados.vetorRestricaoOperativaUHE.getIdObjetos(AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica_energia_armazenada);

		const int numero_restricoes = int(idRestricaoOperativaUHE_energia_armazenada.size());

		for (int pos = 0; pos < numero_restricoes; pos++) {

			const IdRestricaoOperativaUHE idRestricaoOperativaUHE = idRestricaoOperativaUHE_energia_armazenada.at(pos);

			// Energia armazenada
//
			if (a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_energia_armazenada) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_inf, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("min")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum && a_dados.getElementoVetor(idRestricaoOperativaUHE, AttVetorRestricaoOperativaUHE_lim_sup_folga_inf, a_period, double()) > 0.0) {
						const int varRHE_FINF = addVarDecisao_RHE_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE, 0.0, infinito, 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRHE_FINF, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_penalidade, double()));
					}

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_inf, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("min")) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_sup, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("max")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum) {
						const int varRHE_FSUP = addVarDecisao_RHE_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE, 0.0, infinito, 0.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRHE_FSUP, getEquLinear_ZP(a_TSS, a_idEstagio, a_period), -a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_penalidade, double()));
					}

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_sup, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("max")) {

			}//if (a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_energia_armazenad) {		


			/////////////////////////////////////////////////////////////////////
			//Restrição Hidráulica de Volume Armazenado (RHE)
			// %EAR_REE_max <= produtibilidade_acumulada_EAR * Volume
			// 
			// Nota: produtibilidade_acumulada_EAR é calculada com base nas regras de cálculo da conversão dos cortes
			/////////////////////////////////////////////////////////////////////
			//******************************************
			//Restrição <=
			//******************************************

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_inf, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("min")) {

				const int posIneRHE = addIneLinear_RHE_LINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE);

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHE, a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_inf, a_period, IdPatamarCarga_1, double()));

				//Variável RHE_FINF
				if (getVarDecisao_RHE_FINFseExistir(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHE_FINF(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), posIneRHE, 1.0);

				const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
				for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

					const IdHidreletrica       idUHE = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
					const TipoVariavelRestricaoOperativa tipoVariavelRestricaoOperativa = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

					if (idUHE != IdHidreletrica_Nenhum) {

						// Variável VI ou VF (Volume armazenado nos reservatórios)
						if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_final) {

							if (a_period == a_periodEnd_stage) {

								if (getVarDecisao_VF(a_TSS, a_idEstagio, a_period, idUHE) > -1)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_period, idUHE), posIneRHE, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double()));

							}//if (a_period == a_horizonte_estudo.getIteradorFinal()) {
							else {

								if (getVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, idUHE) > -1)
									vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, idUHE), posIneRHE, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double()));

							}//else {

						}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_final) {

					} // if (idUHE != IdHidreletrica_Nenhum) {
					else
						throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

				} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){


			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_inf, a_period, IdPatamarCarga_1, double()) > 0) {

			//******************************************
			//Restrição >=
			//******************************************

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_sup, a_period, IdPatamarCarga_1, double()) != getdoubleFromChar("max")) {

				if (getVarDecisao_RHE_FSUPseExistir(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

					const int posIneRHE = addIneLinear_RHE_LSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE);

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHE, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_sup, a_period, IdPatamarCarga_1, double()));

					// Variável RHE
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHE_FSUP(a_TSS, a_idEstagio, a_period, idRestricaoOperativaUHE), posIneRHE, 1.0);

					const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
					for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

						const IdHidreletrica       idUHE = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
						const IdUsinaElevatoria idUsinaElevatoria = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());

						const TipoVariavelRestricaoOperativa tipoVariavelRestricaoOperativa = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

						if (idUHE != IdHidreletrica_Nenhum) {

							// Variável VF ou VI (Volume armazenado nos reservatórios)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_final) {

								if (a_period == a_periodEnd_stage) {

									if (getVarDecisao_VF(a_TSS, a_idEstagio, a_period, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_period, idUHE), posIneRHE, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double()));

								}//if (a_period == a_horizonte_estudo.getIteradorFinal()) {
								else {

									if (getVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, idUHE) > -1)
										vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodNext, idUHE), posIneRHE, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_period, IdPatamarCarga_1, double()));

								}//else {
							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_final) {

						} // if (idUHE != IdHidreletrica_Nenhum) {
						else
							throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

					} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

				} // if (getVarDecisao_RHEseExistir(a_idEstagio, a_period, idRestricaoOperativaUHE) > -1) {

			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_lim_sup, a_period, IdPatamarCarga_1, double()) < getdoubleFromChar("max")) {


		}// for (int pos = 0; pos < numero_restricoes; pos++) {

	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesHidraulicaEspecial_energia_armazenada(" + getFullString(a_TSS) + ",Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados): \n" + std::string(erro.what())); }

}//void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_energia_armazenada(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados)


