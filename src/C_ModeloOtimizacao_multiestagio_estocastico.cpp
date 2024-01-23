#include "C_ModeloOtimizacao.h"
#include "C_EntradaSaidaDados.h"
#include <chrono>

#include "mpi.h"

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////


void ModeloOtimizacao::formularModeloOtimizacao(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados) {


	try {
		
		const IdSubmercado            maiorIdSubmercado = a_dados.getMaiorId(IdSubmercado());
		const IdTermeletrica          maiorIdTermeletrica = a_dados.getMaiorId(IdTermeletrica());
		const IdHidreletrica          maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());
		const IdIntercambio           maiorIdIntercambio = a_dados.getMaiorId(IdIntercambio());
		const IdRestricaoEletrica     maiorIdRestricaoEletrica = a_dados.getMaiorId(IdRestricaoEletrica());
		const IdRestricaoOperativaUHE maiorIdRestricaoOperativaUHE = a_dados.getMaiorId(IdRestricaoOperativaUHE());
		const IdIntercambioHidraulico maiorIdIntercambioHidraulico = a_dados.getMaiorId(IdIntercambioHidraulico());
		const IdUsinaElevatoria       maiorIdUsinaElevatoria = a_dados.getMaiorId(IdUsinaElevatoria());
		const IdContrato              maiorIdContrato = a_dados.getMaiorId(IdContrato());
		const IdDemandaEspecial       maiorIdDemandaEspecial = a_dados.getMaiorId(IdDemandaEspecial());
		const IdUsinaEolica           maiorIdUsinaEolica = a_dados.getMaiorId(IdUsinaEolica());

		const IdEstagio idEstagio_acoplamento = a_dados.getAtributo(AttComumDados_estagio_acoplamento_pre_estudo, IdEstagio());

		const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
		const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

		const IdAgrupamentoIntercambio maiorIdAgrupamentoIntercambio = a_dados.getMaiorId(IdAgrupamentoIntercambio());

		const Periodo periodo_otimizacao_inicial = getAtributo(AttComumModeloOtimizacao_periodo_otimizacao_inicial, Periodo());
		const Periodo periodo_otimizacao_final = getAtributo(AttComumModeloOtimizacao_periodo_otimizacao_final, Periodo());

		const Periodo periodo_estudo_inicial = getAtributo(AttComumModeloOtimizacao_periodo_estudo_inicial, Periodo());
		const Periodo periodo_estudo_final = getAtributo(AttComumModeloOtimizacao_periodo_estudo_final, Periodo());

		const IdProcessoEstocastico   tipo_processo_estocastico_hidrologico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

		TipoSolver tipoSolver = a_dados.getAtributo(AttComumDados_tipo_solver, TipoSolver());

		SmartEnupla<Periodo, IdEstagio> horizonte_estudo;

		SmartEnupla<IdEstagio, SmartEnupla<Periodo, double>> horizonte_estudo_por_estagio(estagio_inicial, std::vector<SmartEnupla<Periodo, double>>(int(estagio_final - estagio_inicial) + 1, SmartEnupla<Periodo, double>()));
		SmartEnupla<IdEstagio, SmartEnupla<Periodo, int>> horizonte_processo_estocastico_hidrologico_por_estagio(estagio_inicial, std::vector<SmartEnupla<Periodo, int>>(int(estagio_final - estagio_inicial) + 1, SmartEnupla<Periodo, int>()));

		// Instanciamento Estagio

		if (true) {

			const SmartEnupla<Periodo, IdEstagio> horizonte_estudo_original = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

			// Instancia Estagio

			for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

				const Periodo periodo_otimizacao = a_dados.getElementoVetor(AttVetorDados_horizonte_otimizacao, idEstagio, Periodo());

				if (true) {
					Estagio estagio;
					estagio.setAtributo(AttComumEstagio_idEstagio, idEstagio);
					vetorEstagio.add(estagio);
				} // if (true) {

				vetorEstagio.att(idEstagio).setAtributo(AttComumEstagio_periodo_otimizacao, periodo_otimizacao);

				vetorEstagio.att(idEstagio).setAtributo(AttComumEstagio_maiorIdRealizacao, IdRealizacao(a_dados.getElementoVetor(AttVetorDados_numero_aberturas, idEstagio, int())));

				vetorEstagio.att(idEstagio).setAtributo(AttComumEstagio_cortes_multiplos, a_dados.getElementoVetor(AttVetorDados_cortes_multiplos, idEstagio, int()));

				vetorEstagio.att(idEstagio).setAtributo(AttComumEstagio_lambda_CVAR, a_dados.getElementoVetor(AttVetorDados_lambda_CVAR, idEstagio, double()));
				vetorEstagio.att(idEstagio).setAtributo(AttComumEstagio_alpha_CVAR, a_dados.getElementoVetor(AttVetorDados_alpha_CVAR, idEstagio, double()));

				vetorEstagio.att(idEstagio).selecaoSolucaoProxy(a_dados.getElementoVetor(AttVetorDados_numero_aberturas_solucao_proxy, idEstagio, int()));

				if (idEstagio == idEstagio_acoplamento)
					vetorEstagio.att(idEstagio).setAtributo(AttComumEstagio_selecao_cortes_nivel_dominancia, 0);
				else
					vetorEstagio.att(idEstagio).setAtributo(AttComumEstagio_selecao_cortes_nivel_dominancia, getAtributo(AttComumModeloOtimizacao_selecao_cortes_nivel_dominancia, int()));

				if (strCompara(getAtributo(AttComumModeloOtimizacao_diretorio_importacao_cortes, std::string()), "nenhum"))
					vetorEstagio.att(idEstagio).vetorCorteBenders.alocar(getAtributo(AttComumModeloOtimizacao_numero_cenarios, int()));

				for (Periodo periodo = horizonte_estudo_original.getIteradorInicial(); periodo <= horizonte_estudo_original.getIteradorFinal(); horizonte_estudo_original.incrementarIterador(periodo)) {
					if (horizonte_estudo_original.at(periodo) == idEstagio)
						horizonte_estudo.addElemento(periodo, idEstagio);
				}


			} // for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

		} // if (true) {

		//
		// Identificação do menor_periodo_estagio_acoplamento_pre_estudo
		//

		Periodo periodo_acoplamento;
		if (idEstagio_acoplamento > estagio_final) {
			const SmartEnupla<Periodo, double> horizonte_estudo_estagio = a_dados.getElementosMatriz(AttMatrizDados_percentual_duracao_horizonte_estudo, estagio_final, Periodo(), double());
			periodo_acoplamento = horizonte_estudo_estagio.getIteradorFinal() + 1;
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


		//
		// Pré alocação de vetores e matrizes
		//


		if (true) {

			SmartEnupla<IdEstagio, SmartEnupla<IdRealizacao, double>> custo_medio(estagio_inicial, std::vector<SmartEnupla<IdRealizacao, double>>(int(estagio_final) - int(estagio_inicial) + 1, SmartEnupla<IdRealizacao, double>()));

			for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

				const Periodo periodo_otimizacao = getAtributo(idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

				custo_medio.at(idEstagio) = SmartEnupla<IdRealizacao, double>(IdRealizacao_1, std::vector<double>(getAtributo(idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()), 0.0));		

				if (getSize1Matriz(tipo_processo_estocastico_hidrologico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral) > 0) {
					const SmartEnupla<Periodo, IdRealizacao> horizonte_processo_estocastico_espaco_amostral_completo = getElementosMatriz(tipo_processo_estocastico_hidrologico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario_1, Periodo(), IdRealizacao());
					const std::vector<Periodo> horizonte_processo_estocastico_espaco_amostral = horizonte_processo_estocastico_espaco_amostral_completo.getIteradores(periodo_otimizacao);

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

			setVetor(AttVetorModeloOtimizacao_horizonte_estudo, horizonte_estudo);

			setMatriz(AttMatrizModeloOtimizacao_horizonte_estudo, horizonte_estudo_por_estagio);
			setMatriz(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, horizonte_processo_estocastico_hidrologico_por_estagio);

			setVetor(AttVetorModeloOtimizacao_custo_total, SmartEnupla<IdEstagio, double>(estagio_inicial, std::vector<double>(int(estagio_final) - int(estagio_inicial) + 1, NAN)));
			setVetor(AttVetorModeloOtimizacao_custo_imediato, SmartEnupla<IdEstagio, double>(estagio_inicial, std::vector<double>(int(estagio_final) - int(estagio_inicial) + 1, NAN)));
			setVetor(AttVetorModeloOtimizacao_custo_futuro, SmartEnupla<IdEstagio, double>(estagio_inicial, std::vector<double>(int(estagio_final) - int(estagio_inicial) + 1, NAN)));

			setMatriz(AttMatrizModeloOtimizacao_realizacao_solucao_proxy, SmartEnupla<IdEstagio, SmartEnupla<IdRealizacao, int>>(estagio_inicial, std::vector<SmartEnupla<IdRealizacao, int>>(int(estagio_final) - int(estagio_inicial) + 1, SmartEnupla<IdRealizacao, int>())));

			setVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, SmartEnupla<IdEstagio, int>(estagio_inicial, std::vector<int>(int(estagio_final) - int(estagio_inicial) + 1, 2)));

			setMatriz(AttMatrizModeloOtimizacao_custo_medio, custo_medio);

			setMatriz(AttMatrizModeloOtimizacao_percentual_duracao_patamar_carga, a_dados.getMatriz(AttMatrizDados_percentual_duracao_patamar_carga, Periodo(), IdPatamarCarga(), double()));

		} // if (true) {


		// Inicia Formulação do Modelo

		for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

			std::vector<TipoSubproblemaSolver> listaTipoSubproblemaSolver;
			if (true) {
				bool isViabilidadeHidraulicaInstanciado = false;
				for (TipoSubproblemaSolver tipoSubproblemaSolver = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tipoSubproblemaSolver < TipoSubproblemaSolver_Excedente; tipoSubproblemaSolver++) {
					if (isNecessarioInstanciarSolver(idEstagio, tipoSubproblemaSolver)) {
						if (tipoSubproblemaSolver == TipoSubproblemaSolver_viabilidade_hidraulica)
							isViabilidadeHidraulicaInstanciado = true;
						else
							listaTipoSubproblemaSolver.push_back(tipoSubproblemaSolver);
					}
				} // for (TipoSubproblemaSolver tipoSubproblemaSolver = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tipoSubproblemaSolver < TipoSubproblemaSolver_Excedente; tipoSubproblemaSolver++) {
				// Instancia viabilidade hidraulica por último para compatibilização com criação de estados.
				if (isViabilidadeHidraulicaInstanciado)
					listaTipoSubproblemaSolver.push_back(TipoSubproblemaSolver_viabilidade_hidraulica);
			}

			// Instancia Solver

			for (int i = 0; i < int(listaTipoSubproblemaSolver.size()); i++) {
				vetorEstagio.att(idEstagio).instanciarSolver(listaTipoSubproblemaSolver.at(i), tipoSolver);
				vetorEstagio.att(idEstagio).setAtributo(AttComumEstagio_tipo_solver, tipoSolver);
				vetorEstagio.att(idEstagio).getSolver(listaTipoSubproblemaSolver.at(i))->exibirNaTela(a_dados.getAtributo(AttComumDados_exibir_na_tela_resultado_solver, bool()));
				vetorEstagio.att(idEstagio).getSolver(listaTipoSubproblemaSolver.at(i))->setMetodoPadrao(a_dados.getAtributo(AttComumDados_metodo_solver, TipoMetodoSolver()));
				vetorEstagio.att(idEstagio).getSolver(listaTipoSubproblemaSolver.at(i))->setToleranciaOtimalidade(a_dados.getAtributo(AttComumDados_tolerancia_otimalidade, double()));
				vetorEstagio.att(idEstagio).getSolver(listaTipoSubproblemaSolver.at(i))->setToleranciaViabilidade(a_dados.getAtributo(AttComumDados_tolerancia_viabilidade, double()));
				vetorEstagio.att(idEstagio).getSolver(listaTipoSubproblemaSolver.at(i))->setFocoNumerico(a_dados.getAtributo(AttComumDados_foco_numerico, int()));
				//vetorEstagio.att(idEstagio).getSolver(listaTipoSubproblemaSolver.at(i))->setEscalonamento(a_dados.getAtributo(AttComumDados_escalonamento, int()));

				if (listaTipoSubproblemaSolver.at(i) != TipoSubproblemaSolver_mestre) {

					//alocarVarDecisao_YH_3(listaTipoSubproblemaSolver.at(i), idEstagio, alocacao_afluencia_incremental.at(idEstagio));
					//alocarVarDecisao_YHF_3(listaTipoSubproblemaSolver.at(i), idEstagio, alocacao_afluencia_incremental.at(idEstagio));

					//alocarVarDecisao_QDEF_3(listaTipoSubproblemaSolver.at(i), idEstagio, alocacao_defluencia.at(idEstagio));

				} // if (listaTipoSubproblemaSolver.at(i) != TipoSubproblemaSolver_mestre) {

			} // for (int i = 0; i < int(listaTipoSubproblemaSolver.size()); i++) {

			SmartEnupla<Periodo, int> horizonte_processo_estocastico = getElementosMatriz(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, idEstagio, Periodo(), int());

			const Periodo periodo_otimizacao = a_dados.getElementoVetor(AttVetorDados_horizonte_otimizacao, idEstagio, Periodo());

			const SmartEnupla<Periodo, double> horizonte_estudo_estagio = a_dados.getElementosMatriz(AttMatrizDados_percentual_duracao_horizonte_estudo, idEstagio, Periodo(), double());


			// **************************************************************************************************************************
			//
			//                                             Constroi a Função Objetivo e o Limites das Variaveis
			//
			// **************************************************************************************************************************

			// VARIAVEIS HIDRAULICAS
			auto start_clock = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < int(listaTipoSubproblemaSolver.size()); i++) {
				start_clock = std::chrono::high_resolution_clock::now();
				criarVariaveisHidraulicas(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio);
				tempoCriarVariaveis.variaveisHidreletrica += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;

				//VARIÁVEIS ASSOCIADAS AO HORIZONTE DE ESTUDO
				start_clock = std::chrono::high_resolution_clock::now();
				criarVariaveisAssociadasHorizonteEstudo(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, maiorIdDemandaEspecial, maiorIdUsinaEolica, maiorIdContrato, maiorIdSubmercado, maiorIdIntercambio, maiorIdRestricaoEletrica, maiorIdTermeletrica, maiorIdRestricaoOperativaUHE, maiorIdIntercambioHidraulico, maiorIdUsinaElevatoria, horizonte_estudo_estagio, tempoCriarVariaveis);
				tempoCriarVariaveis.variaveisAssociadasHorizonteEstudo += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;


				//VARIÁVEIS ASSOCIADAS AO HORIZONTE DE OTIMIZAÇÃO
				start_clock = std::chrono::high_resolution_clock::now();
				criarVariaveisAssociadasHorizonteOtimizacao(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, estagio_final, estagio_inicial, periodo_otimizacao);
				tempoCriarVariaveis.variaveisAssociadasHorizonteOtimizacao += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;


				//VARIÁVEIS ASSOCIADAS AO HORIZONTE DO PROCESSO ESTOCASTICO
				start_clock = std::chrono::high_resolution_clock::now();
				//criarVariaveisAssociadasHorizonteProcessoEstocastico(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, tipo_relaxacao_afluencia_incremental, tipo_processo_estocastico_hidrologico, estagio_inicial, cenario_final, cenario_inicial);
				criarVariaveisDecisao_Restricoes_ProcessoEstocasticoHidrologico(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio);
				tempoCriarVariaveis.variaveisAssociadasHorizonteProcessoEstocastico += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;


				//RESTRIÇÕES DE CUSTO TOTAL IMEDIADO e FUTURO
				start_clock = std::chrono::high_resolution_clock::now();
				criarRestricoesCusto_Total_Imediato_Futuro(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, estagio_final, maiorIdHidreletrica, periodo_otimizacao, horizonte_estudo_estagio, tipo_processo_estocastico_hidrologico);
				tempoCriarVariaveis.restricaoIgualdadeHorizonteOtimizacao += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;


				//RESTRIÇÕES DE IGUALDADE ASSOCIOADAS AO HORIZONTE DE ESTUDO
				for (Periodo periodo_estudo = horizonte_estudo_estagio.getIteradorInicial(); periodo_estudo <= horizonte_estudo_estagio.getIteradorFinal(); horizonte_estudo_estagio.incrementarIterador(periodo_estudo)) {

					//RESTRIÇÕES CUSTO POR PERIODO
					start_clock = std::chrono::high_resolution_clock::now();
					criarRestricoesCusto_periodo(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, periodo_estudo);
					tempoCriarVariaveis.restricaoCusto_periodo += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;

					//RESTRIÇÕES CUSTO VALOR PRESENTE POR PERIODO
					start_clock = std::chrono::high_resolution_clock::now();
					criarRestricoesCustoValorPresente_periodo(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, periodo_estudo);
					tempoCriarVariaveis.restricaoCustoValorPresente_periodo += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;

					//RESTRIÇÕES HIDRAULICA ESPECIAL VAZÃO AFLUENTE 
					start_clock = std::chrono::high_resolution_clock::now();
					criarRestricoesHidraulicaEspecial_vazao_afluente(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, periodo_estudo, horizonte_processo_estocastico, horizonte_estudo_estagio);
					tempoCriarVariaveis.restricaoHidraulicaEspecial_vazao_afluente += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;


					//RESTRIÇÕES HIDRAULICA ESPECIAL VOLUME ARMAZENADO
					start_clock = std::chrono::high_resolution_clock::now();
					criarRestricoesHidraulicaEspecial_volume_armazenado(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, periodo_estudo, horizonte_estudo_estagio);
					tempoCriarVariaveis.restricaoHidraulicaEspecial_volume_armazenado += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;

					//RESTRIÇÕES HIDRAULICA ESPECIAL ENERGIA ARMAZENADA
					start_clock = std::chrono::high_resolution_clock::now();
					criarRestricoesHidraulicaEspecial_energia_armazenada(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, periodo_estudo, horizonte_estudo_estagio);
					tempoCriarVariaveis.restricaoHidraulicaEspecial_volume_armazenado += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;

					const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_estudo, IdPatamarCarga());

					//  Restrição Custo Penalidade (ZP) Por PeriodoEstudo (penalidade de variáveis que não dependem do patamar de carga)
					criarRestricoesCustoPenalidade_periodoEstudo(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, periodo_estudo, maiorIdRestricaoOperativaUHE, maiorIdHidreletrica);

					for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						// Restrição Custo Operacao (ZO) - Por PeriodoEstudo Por PatamarCarga
						start_clock = std::chrono::high_resolution_clock::now();
						criarRestricoesCustoOperacao_periodoEstudo_patamarCarga(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, periodo_estudo, maiorIdSubmercado, maiorIdTermeletrica, idPatamarCarga);
						tempoCriarVariaveis.restricaoCusto_periodoEstudo_patamarCarga += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;


						//  Restrição Custo Penalidade (ZP) Por PeriodoEstudo Por PatamarCarga
						start_clock = std::chrono::high_resolution_clock::now();
						criarRestricoesCustoPenalidade_periodoEstudo_patamarCarga(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, periodo_estudo, maiorIdSubmercado, maiorIdIntercambio, maiorIdRestricaoEletrica, maiorIdTermeletrica, maiorIdRestricaoOperativaUHE, maiorIdHidreletrica, maiorIdIntercambioHidraulico, maiorIdUsinaElevatoria, periodo_otimizacao, idPatamarCarga);
						tempoCriarVariaveis.restricaoCustoPenalidade_periodoEstudo_patamarCarga += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;


						//RESTRIÇÕES HIDRAULICA ESPECIAL VAZÃO DEFLUENTE 
						start_clock = std::chrono::high_resolution_clock::now();
						criarRestricoesHidraulicaEspecial_vazao_defluente(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, idPatamarCarga, periodo_estudo);
						tempoCriarVariaveis.restricaoHidraulicaEspecial_vazao_defluente += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;


						// RESTRIÇÃO DAS USINAS HIDRELETRICAS 
						for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

							// RESTRIÇÃO DE BALANÇO HIDRAULICO 
							if ((a_dados.getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, periodo_estudo, double()) > 0.0) || (a_dados.getAtributo(AttComumDados_representar_todos_balancos_hidricos_por_volume, bool())) || \
								(getVarDecisao_VIseExistir(listaTipoSubproblemaSolver.at(i), idEstagio, periodo_estudo, idHidreletrica) > -1) || (getVarDecisao_VFseExistir(listaTipoSubproblemaSolver.at(i), idEstagio, periodo_estudo, idHidreletrica) > -1)) {

								// Restrições Balanço Hidraulico Por Volume (BH_VOL)
								start_clock = std::chrono::high_resolution_clock::now();
								criarRestricoesBalancoHidraulicoUsinaByVolume(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, idEstagio_acoplamento, periodo_acoplamento, periodo_estudo, periodo_otimizacao_final, periodo_estudo_inicial, idHidreletrica, idPatamarCarga, horizonte_processo_estocastico, horizonte_estudo_estagio);
								tempoCriarVariaveis.restricaoBalancoHidraulicoUsinaRegularizacao += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;
							}
							else {
								// Restrições Balanço Hidraulico Por Vazão (BH_VAZ)
								start_clock = std::chrono::high_resolution_clock::now();
								criarRestricoesBalancoHidraulicoUsinaByVazao(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, idEstagio_acoplamento, periodo_acoplamento, periodo_estudo, periodo_otimizacao_final, periodo_estudo_inicial, idHidreletrica, idPatamarCarga, horizonte_processo_estocastico, horizonte_estudo_estagio);
								tempoCriarVariaveis.restricaoBalancoHidraulicoUsinaFioAgua += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;

							}

							// Restriçao Volume de espera
							if (idPatamarCarga == IdPatamarCarga_1) {

								// CRIA RESTRIÇÕES DE VOLUME UTIL
								start_clock = std::chrono::high_resolution_clock::now();
								criarRestricoesVolumeUtil_e_Penalidade(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, periodo_estudo, idHidreletrica);
								tempoCriarVariaveis.restricaoVolumeUtil += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;


							}//if (idPatamarCarga == IdPatamarCarga_1) {

							// Restriçao Potência Hidraulica Disponível
							criarRestricoesPotenciaHidraulicaDisponivel(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, idPatamarCarga, periodo_estudo, idHidreletrica, horizonte_estudo_estagio);

							//FPH
							start_clock = std::chrono::high_resolution_clock::now();
							criarRestricoesFuncaoProducaoHidreletrica(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, idHidreletrica, idPatamarCarga, horizonte_estudo_estagio, periodo_estudo);
							tempoCriarVariaveis.restricaoProducaoHidreletrica_porFPH += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;


							// CRIA RESTRIÇÕES DE VAZÃO DEFLUENTE 
							start_clock = std::chrono::high_resolution_clock::now();
							criarRestricoesVazaoDefluente(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, periodo_estudo, idHidreletrica, idPatamarCarga);
							tempoCriarVariaveis.restricaoVazaoDefluente += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;

							// CRIA RESTRIÇÕES DE VAZÃO DESVIADA
							start_clock = std::chrono::high_resolution_clock::now();
							criarRestricoesVazaoDesviada(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, periodo_estudo, idHidreletrica, idPatamarCarga);
							tempoCriarVariaveis.restricaoVazaoDesviada += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;

							// CRIA RESTRIÇÕES DE VAZÃO RETIRADA
							start_clock = std::chrono::high_resolution_clock::now();
							criarRestricoesVazaoRetirada(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, periodo_estudo, idHidreletrica, idPatamarCarga);
							tempoCriarVariaveis.restricaoVazaoRetirada += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;


							// CRIA RESTRIÇÕES DE TURBINAMENTO HIDRAULICO
							start_clock = std::chrono::high_resolution_clock::now();
							criarRestricoesTurbinamentoHidraulico(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, periodo_estudo, idHidreletrica, idPatamarCarga);
							tempoCriarVariaveis.restricaoTurbinamentoHidraulico += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;

						}// for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {


						// Restrições: Potencia Termelétrica (Por Produtibilidade), Potencia Termeletrica Relaxada, Potencia Termeletrica Disponível e Potencia Termeletrica Disponível Relaxada
						start_clock = std::chrono::high_resolution_clock::now();
						criarRestricoesProducaoTermeletrica_porProdutibilidade(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, idPatamarCarga, periodo_estudo, maiorIdTermeletrica, horizonte_estudo_estagio);
						tempoCriarVariaveis.restricaoProducaoTermeletrica_porProdutibilidade += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;


						// **************************************************************************************************************************
						//                              Constroi as restrições de desigualdade
						// **************************************************************************************************************************

						// CRIA RESTRIÇÃO ELETRICA (RE)
						start_clock = std::chrono::high_resolution_clock::now();
						criarRestricoesEletricas(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, maiorIdRestricaoEletrica, periodo_estudo, idPatamarCarga);
						tempoCriarVariaveis.restricoesEletricas += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;


						// CRIA RESTRIÇOES AGRUPAMENTO INTERCAMBIO
						start_clock = std::chrono::high_resolution_clock::now();
						criarRestricoesAgrupamentoIntercambio(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, maiorIdAgrupamentoIntercambio, periodo_estudo, idPatamarCarga);
						tempoCriarVariaveis.restricoesAgrupamentoIntercambio += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;

						// CRIA RESTRIÇAO RELAXADA DO INTERCAMBIO MÍNIMO
						for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= maiorIdIntercambio; idIntercambio++)
							criarRestricoesIntercambioRelaxado(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, periodo_estudo, idIntercambio, idPatamarCarga);

						for (IdSubmercado idSubmercado = a_dados.getMenorId(IdSubmercado()); idSubmercado <= maiorIdSubmercado; a_dados.vetorSubmercado.incr(idSubmercado)) {

							//RESTIÇÃO DE PRODUÇÃO MÉDIA
							start_clock = std::chrono::high_resolution_clock::now();

							criarRestricoesProducaoMedia(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, periodo_estudo, idSubmercado, maiorIdIntercambio, maiorIdTermeletrica, maiorIdHidreletrica, idPatamarCarga);
							tempoCriarVariaveis.restricaoProducaoMedia += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;


							//RESTRIÇÃO DE ATENDIMANTO A DEMANDA (AD)
							start_clock = std::chrono::high_resolution_clock::now();
							criarRestricoesAtendimentoDemanda(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, periodo_estudo, maiorIdUsinaEolica, maiorIdContrato, idSubmercado, maiorIdIntercambio, maiorIdTermeletrica, maiorIdHidreletrica, maiorIdUsinaElevatoria, idPatamarCarga);
							tempoCriarVariaveis.restricaoAtendimentoDemanda += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;

						}//for (IdSubmercado idSubmercado = a_dados.getMenorId(IdSubmercado()); idSubmercado <= maiorIdSubmercado; a_dados.vetorSubmercado.incr(idSubmercado)) {

						//RESTRIÇÕES DO INTERCÂMBIO HIDRÁULICO RETIRADAS QOUT
						start_clock = std::chrono::high_resolution_clock::now();
						for (IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico_1; idIntercambioHidraulico <= a_dados.getMaiorId(IdIntercambioHidraulico()); idIntercambioHidraulico++) {

							const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

							if (((tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada) || (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_pereira_barreto)) && a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo, periodo_estudo, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo, periodo_estudo, double()) > 0) {
								criarRestricoesIntercambioHidraulicooRetiradaRelaxada(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, periodo_estudo, idPatamarCarga, idIntercambioHidraulico);

							}//if (a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo, periodo_estudo, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo, periodo_estudo, double()) > 0) {

						}//for (IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico_1; idIntercambioHidraulico <= a_dados.getMaiorId(IdIntercambioHidraulico()); idIntercambioHidraulico++) {

						for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria <= a_dados.getMaiorId(IdUsinaElevatoria()); idUsinaElevatoria++) {
							criarRestricoesBombeamentoHidraulico(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, periodo_estudo, idPatamarCarga, idUsinaElevatoria);
							criarRestricoesVazaoBombeada(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, periodo_estudo, idUsinaElevatoria, idPatamarCarga);

						}//for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria <= a_dados.getMaiorId(IdUsinaElevatoria()); idUsinaElevatoria++) {

						tempoCriarVariaveis.restricaoIntercambioHidraulicoRetiradas += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;

					}// for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

				} // for (Periodo periodo_estudo = horizonte_estudo_estagio.getIteradorInicial(); periodo_estudo <= horizonte_estudo_estagio.getIteradorFinal(); horizonte_estudo_estagio.incrementarIterador(periodo_estudo)) {


				 // CRIA CORTES DE BENDERS EM CUSTO FUTURO (ZF)
				start_clock = std::chrono::high_resolution_clock::now();
				if (idEstagio < estagio_final)
					criarRestricoesCorteBendersEmCustoFuturo(listaTipoSubproblemaSolver.at(i), idEstagio);
				tempoCriarVariaveis.restricaoCortebendersEmCustoFuturo += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock).count() / 60;

				criarRestricoesCorteBendersEmCustoTotal(listaTipoSubproblemaSolver.at(i), idEstagio);

				//VARIÁVEIS DE FOLGA = 0 (se considerar_variaveis_folga = false)
				if (!a_dados.getAtributo(AttComumDados_considerar_variaveis_folga, bool()))
					zerarVariaveisFolga(listaTipoSubproblemaSolver.at(i), a_dados, idEstagio, maiorIdIntercambio, maiorIdRestricaoEletrica, maiorIdTermeletrica, maiorIdRestricaoOperativaUHE, maiorIdIntercambioHidraulico, maiorIdUsinaElevatoria, horizonte_estudo_estagio);


			} // for (int i = 0; i < int(listaTipoSubproblemaSolver.size()); i++){

		} // for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++){


		const IdProcesso idProcesso = a_dados.arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());
		const IdProcesso maiorIdProcesso = a_dados.arranjoResolucao.getMaiorId(IdProcesso());

		importarVariaveisEstado_AcoplamentoPosEstudo(TipoSubproblemaSolver_geral, a_dados, idProcesso, a_entradaSaidaDados);

		importarCorteBenders(TipoSubproblemaSolver_geral, a_dados, idProcesso, std::string(a_entradaSaidaDados.getDiretorioSaida() + "//" + getFullString(a_dados.arranjoResolucao.getAtributo(AttComumArranjoResolucao_iteracao_inicial, IdIteracao())) + "//Selecao_Cortes_Importacao"), a_entradaSaidaDados);

		if (idProcesso == IdProcesso_mestre)
			exportarVariaveisEstado_AcoplamentoPreEstudo(a_entradaSaidaDados);

		if ((idProcesso == IdProcesso_mestre) && (a_dados.getAtributo(AttComumDados_imprimir_tempos_construcao_modelo, bool()))) {

			std::cout << std::endl << std::endl;
			std::cout << "TEMPO PARA CRIAR AS VARIAVEIS DE VOLUME                                  = " << tempoCriarVariaveis.variaveisVolume << std::endl;
			std::cout << "TEMPO PARA CRIAR AS VARIAVEIS DAS HIDRELETRICAS                          = " << tempoCriarVariaveis.variaveisHidreletrica << std::endl;
			std::cout << "TEMPO PARA CRIAR AS VARIAVEIS DAS TERMELETRICAS                          = " << tempoCriarVariaveis.variaveisTermeletrica << std::endl;
			std::cout << "TEMPO PARA CRIAR AS VARIAVEIS DE DEFICIT                                 = " << tempoCriarVariaveis.variaveisDeficit << std::endl;
			std::cout << "TEMPO PARA CRIAR AS VARIAVEIS DE DEMANDA                                 = " << tempoCriarVariaveis.variaveisDemanda << std::endl;
			std::cout << "TEMPO PARA CRIAR AS VARIAVEIS DAS USINAS NAO SIMULADAS                   = " << tempoCriarVariaveis.variaveisUsinaNSimulada << std::endl;
			std::cout << "TEMPO PARA CRIAR AS VARIAVEIS DE INTERCAMBIO                             = " << tempoCriarVariaveis.variaveisIntercambio << std::endl;
			std::cout << "TEMPO PARA CRIAR AS VARIAVEIS DAS RESTRICOES OPERATIVAS                  = " << tempoCriarVariaveis.variaveisRestOperativaUHE << std::endl;
			std::cout << "TEMPO PARA CRIAR AS VARIAVEIS DAS RESTRICOES ELETRICAS DE FOLGA          = " << tempoCriarVariaveis.variaveisRestEleticaFolga << std::endl;
			std::cout << "TEMPO PARA CRIAR AS VARIAVEIS DOS INTERCAMBIOS HIDRAULICOS               = " << tempoCriarVariaveis.variaveisIntercambioHidraulico << std::endl;
			std::cout << "TEMPO TOTAL PARA CRIAR AS VARIAVEIS DO HORIZONTE OTIMIZACAO              = " << tempoCriarVariaveis.variaveisAssociadasHorizonteOtimizacao << std::endl;
			std::cout << "TEMPO TOTAL PARA CRIAR AS VARIAVEIS DO HORIZONTE ESTUDO                  = " << tempoCriarVariaveis.variaveisAssociadasHorizonteEstudo << std::endl;
			std::cout << "TEMPO TOTAL PARA CRIAR AS VARIAVEIS DO PROCESSO ESTOCASTICO              = " << tempoCriarVariaveis.variaveisAssociadasHorizonteProcessoEstocastico << std::endl;
			std::cout << "TEMPO RESTRICOES DE IGUALDADE HORIZONTE OTIMIZACAO                       = " << tempoCriarVariaveis.restricaoIgualdadeHorizonteOtimizacao << std::endl;
			std::cout << "TEMPO RESTRICOES DE CUSTO DE OPERACAO POR PERIODO POR PATAMAR DE CARGA   = " << tempoCriarVariaveis.restricaoCusto_periodoEstudo_patamarCarga << std::endl;
			std::cout << "TEMPO RESTRICOES DE CUSTO DE PENALIDADE VALOR PRESENTE POR PERIODO       = " << tempoCriarVariaveis.restricaoCustoValorPresente_periodo << std::endl;
			std::cout << "TEMPO RESTRICOES DE CUSTO DE PENALIDADE POR PERIODO POR PATAMAR DE CARGA = " << tempoCriarVariaveis.restricaoCustoPenalidade_periodoEstudo_patamarCarga << std::endl;
			std::cout << "TEMPO RESTRICOES DE PRODUCAO MEDIA                                       = " << tempoCriarVariaveis.restricaoProducaoMedia << std::endl;
			std::cout << "TEMPO RESTRICOES DE ATENDIMENTO A DEMANDA                                = " << tempoCriarVariaveis.restricaoAtendimentoDemanda << std::endl;
			std::cout << "TEMPO RESTRICOES DE BALANCO HIDRAULICO USINAS COM REGULARIZACAO          = " << tempoCriarVariaveis.restricaoBalancoHidraulicoUsinaRegularizacao << std::endl;
			std::cout << "TEMPO RESTRICOES DE BALANCO HIDRAULICO USINAS FIO D'AGUA                 = " << tempoCriarVariaveis.restricaoBalancoHidraulicoUsinaFioAgua << std::endl;
			std::cout << "TEMPO RESTRICOES DE TURBINAMENTO HIDRAULICO                              = " << tempoCriarVariaveis.restricaoTurbinamentoHidraulico << std::endl;
			std::cout << "TEMPO RESTRICOES DE PRODUCAO HIDRELETRICA POR PRODUTIBILIDADE            = " << tempoCriarVariaveis.restricaoProducaoHidreletrica_porProdutibilidade << std::endl;
			std::cout << "TEMPO RESTRICOES DE PRODUCAO HIDRELETRICA FPH                            = " << tempoCriarVariaveis.restricaoProducaoHidreletrica_porFPH << std::endl;
			std::cout << "TEMPO RESTRICOES DE PRODUCAO TERMELETRICA POR PRODUTIBILIDADE            = " << tempoCriarVariaveis.restricaoProducaoTermeletrica_porProdutibilidade << std::endl;
			std::cout << "TEMPO RESTRICOES DE VAZAO DEFLUENTE                                      = " << tempoCriarVariaveis.restricaoVazaoDefluente << std::endl;
			std::cout << "TEMPO RESTRICOES HIDRAULICA ESPECIAL DE VAZAO DEFLUENTE                  = " << tempoCriarVariaveis.restricaoHidraulicaEspecial_vazao_defluente << std::endl;
			std::cout << "TEMPO RESTRICOES HIDRAULICA ESPECIAL DE VAZAO AFLUENTE                   = " << tempoCriarVariaveis.restricaoHidraulicaEspecial_vazao_afluente << std::endl;
			std::cout << "TEMPO RESTRICOES HIDRAULICA ESPECIAL DE VOLUME ARMAZENADO                = " << tempoCriarVariaveis.restricaoHidraulicaEspecial_volume_armazenado << std::endl;
			std::cout << "TEMPO RESTRICOES ELETRICAS                                               = " << tempoCriarVariaveis.restricoesEletricas << std::endl;
			std::cout << "TEMPO RESTRICOES AGRUPAMENTO DE INTERCAMBIO                              = " << tempoCriarVariaveis.restricoesAgrupamentoIntercambio << std::endl;
			std::cout << "TEMPO RESTRICOES AFLUENCIA INCREMENTAL                                   = " << tempoCriarVariaveis.restricaoAfluenciaIncremental_porPeriodo_horizonteProcessoEstocastico << std::endl;
			std::cout << "TEMPO RESTRICOES PROCESSO ESTOCASTICO HIDROLOGICO                        = " << tempoCriarVariaveis.restricaoProcessoEstocasticoHidrologico_porPeriodo_horizonteProcessoEstocastico << std::endl;
			std::cout << "TEMPO RESTRICOES CORTES DE BENDERS                                       = " << tempoCriarVariaveis.restricaoCortebendersEmCustoFuturo << std::endl;
			std::cout << "TEMPO RESTRICOES INTERCAMBIO HIDRAULICO RETIRADAS                        = " << tempoCriarVariaveis.restricaoIntercambioHidraulicoRetiradas << std::endl;
			std::cout << "TEMPO RESTRICOES SUPERPOSICAO PERIODOS TEMPO DE VIAGEM D'AGUA            = " << tempoCriarVariaveis.restricaoSuperposicaoPeriodosTempoViagemAgua << std::endl;

		}

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::formularModeloOtimizacao(): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::formularModeloOtimizacao(Dados &a_dados) {


void ModeloOtimizacao::criarRestricoesAgrupamentoIntercambio(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdAgrupamentoIntercambio a_maiorIdAgrupamentoIntercambio, const Periodo a_periodo_estudo, const IdPatamarCarga a_idPatamarCarga) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		for (IdAgrupamentoIntercambio idAgrupamentoIntercambio = IdAgrupamentoIntercambio_1; idAgrupamentoIntercambio <= a_maiorIdAgrupamentoIntercambio; idAgrupamentoIntercambio++) {

			if (a_dados.getSize1Matriz(idAgrupamentoIntercambio, AttMatrizAgrupamentoIntercambio_potencia_maxima) > 0) {

				const double potencia_maxima = a_dados.getElementoMatriz(idAgrupamentoIntercambio, AttMatrizAgrupamentoIntercambio_potencia_maxima, a_periodo_estudo, a_idPatamarCarga, double());

				if (potencia_maxima > 0.0) {

					const int posIneAI = addIneLinear_AGRUPAMENTO_INTERCAMBIO(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idAgrupamentoIntercambio);

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneAI, -potencia_maxima);

					const int numero_intercambio = a_dados.getSizeVetor(idAgrupamentoIntercambio, AttVetorAgrupamentoIntercambio_intercambio);
					for (int i = 1; i <= numero_intercambio; i++)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_dados.getElementoVetor(idAgrupamentoIntercambio, AttVetorAgrupamentoIntercambio_intercambio, i, IdIntercambio())), posIneAI, -a_dados.getElementoVetor(idAgrupamentoIntercambio, AttVetorAgrupamentoIntercambio_fator_ponderacao, i, double()));

				} // if (potencia_maxima > 0.0){

			} // if (a_dados.getSize1Matriz(idAgrupamentoIntercambio, AttMatrizAgrupamentoIntercambio_potencia_maxima) > 0) {

		} // for (IdAgrupamentoIntercambio idAgrupamentoIntercambio = IdAgrupamentoIntercambio_1; idAgrupamentoIntercambio <= maiorIdAgrupamentoIntercambio; idAgrupamentoIntercambio++) {

	}//try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesAgrupamentoIntercambio(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesEletricas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdRestricaoEletrica a_maiorIdRestricaoEletrica, const Periodo a_periodo_estudo, const IdPatamarCarga a_idPatamarCarga) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= a_maiorIdRestricaoEletrica; idRestricaoEletrica++) {

			if (getVarDecisao_RE_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoEletrica) > -1) {

				const int posIneRE = addIneLinear_RESTRICAO_ELETRICA_LIMITE_INFERIOR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoEletrica);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRE, a_dados.getElementoMatriz(idRestricaoEletrica, AttMatrizRestricaoEletrica_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()));

				// Variável RE_FINF
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RE_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoEletrica), posIneRE, 1.0);

				const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoEletrica, IdElementoSistema());
				for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

					const IdHidreletrica       idHidreletrica = a_dados.getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
					const IdConjuntoHidraulico idConjuntoHidraulico = a_dados.getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_conjuntohidraulico, IdConjuntoHidraulico());
					const IdUnidadeUHE         idUnidadeUHE = a_dados.getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_unidadehidraulica, IdUnidadeUHE());
					const IdTermeletrica       idTermeletrica = a_dados.getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_termeletrica, IdTermeletrica());
					const IdUnidadeUTE         idUnidadeUTE = a_dados.getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_unidadeTermeletrica, IdUnidadeUTE());
					const IdIntercambio        idIntercambio = a_dados.getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_intercambio, IdIntercambio());
					const IdContrato           idContrato = a_dados.getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_contrato, IdContrato());
					const IdDemandaEspecial    idDemandaEspecial = a_dados.getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_demandaEspecial, IdDemandaEspecial());
					const IdUsinaEolica        idUsinaEolica = a_dados.getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_eolica, IdUsinaEolica());

					if (idHidreletrica != IdHidreletrica_Nenhum) {

						const TipoDetalhamentoProducaoHidreletrica tipo_detalhamento_producao = a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica());

						// RESTRIÇÃO POR CONJUNTO
						if (idConjuntoHidraulico != IdConjuntoHidraulico_Nenhum) {

							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina)
								throw std::invalid_argument(" Nao e possivel representar a restricao eletrica" + getFullString(idRestricaoEletrica) + " do" + getFullString(idConjuntoHidraulico) + "da" + getFullString(idHidreletrica) + " pois o tipo de detalhamento da producao e " + getFullString(tipo_detalhamento_producao));

							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

								if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

									if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico), posIneRE, a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

								}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
								else {

									if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico), posIneRE, a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

								}//else {

							}//if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

							if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) && (idUnidadeUHE == IdUnidadeUHE_Nenhum)) {

								for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= a_dados.getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidade++) {

									if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

										if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidade) > -1)
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidade), posIneRE, a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

									}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
									else {

										if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidade) > -1)
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidade), posIneRE, a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

									}//else {

								}//for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= a_dados.getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidade++) {

							}//if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) && (idUnidadeUHE == IdUnidadeUHE_Nenhum)) {


							// RESTRIÇÃO POR UNIDADE
							if (idUnidadeUHE != IdUnidadeUHE_Nenhum) {

								if (tipo_detalhamento_producao != TipoDetalhamentoProducaoHidreletrica_por_unidade)
									throw std::invalid_argument(" Nao e possivel representar a restricao eletrica" + getFullString(idRestricaoEletrica) + "da" + getFullString(idUnidadeUHE) + " do" + getFullString(idConjuntoHidraulico) + "da" + getFullString(idHidreletrica) + " pois o tipo de detalhamento da producao e " + getFullString(tipo_detalhamento_producao));

								if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

									if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posIneRE, a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

								}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
								else {

									if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posIneRE, a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

								}//else {

							} // if (idUnidadeUHE != IdUnidadeUHE_Nenhum) {

						}//if (idConjuntoHidraulico != IdConjuntoHidraulico_Nenhum) {


						// RESTRIÇÃO POR HIDRELETRICA
						else if (idConjuntoHidraulico == IdConjuntoHidraulico_Nenhum) {

							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina) {

								if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

									if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posIneRE, a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

								}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
								else {

									if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posIneRE, a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

								}//else {

							}//if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina) {

							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

								for (IdConjuntoHidraulico idConjunto = IdConjuntoHidraulico_1; idConjunto <= a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjunto++) {

									if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

										if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjunto) > -1)
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjunto), posIneRE, a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

									}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
									else {

										if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjunto) > -1)
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjunto), posIneRE, a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

									}//else {

								}//for (IdConjuntoHidraulico idConjunto = IdConjuntoHidraulico_1; idConjunto <= a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjunto++) {

							}//if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

							if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {

								for (IdConjuntoHidraulico idConjunto = IdConjuntoHidraulico_1; idConjunto <= a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjunto++) {

									for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= a_dados.getMaiorId(idHidreletrica, idConjunto, IdUnidadeUHE()); idUnidade++) {

										if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

											if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjunto, idUnidade) > -1)
												vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjunto, idUnidade), posIneRE, a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));


										}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
										else {

											if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjunto, idUnidade) > -1)
												vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjunto, idUnidade), posIneRE, a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

										}//else {

									}//for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= a_dados.getMaiorId(idHidreletrica, idConjunto, IdUnidadeUHE()); idUnidade++) {

								}//for (IdConjuntoHidraulico idConjunto = IdConjuntoHidraulico_1; idConjunto <= a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjunto++) {

							}//if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {

						}//else if (idConjuntoHidraulico == IdConjuntoHidraulico_Nenhum) {

					} // if (idHidreletrica != IdHidreletrica_Nenhum) {

					// RESTRIÇÃO PARA AS TERMELETRICAS 
					else if (idTermeletrica != IdTermeletrica_Nenhum) {

						const TipoDetalhamentoProducaoTermeletrica tipo_detalhamento_producao = a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica());

						// RESTIÇÃO POR UNIDADE
						if (idUnidadeUTE != IdUnidadeUTE_Nenhum) {

							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina)
								throw std::invalid_argument(" Nao e possivel representar a restricao eletrica" + getFullString(idRestricaoEletrica) + "da" + getFullString(idUnidadeUTE) + "da" + getFullString(idTermeletrica) + " pois o tipo de detalhamento da producao e " + getFullString(tipo_detalhamento_producao));

							if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {

								if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE) > -1)
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posIneRE, a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

							}//if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {
							else {

								if (getVarDecisao_PTseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE) > -1)
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posIneRE, a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

							}//else {

						} // if (idUnidadeUTE != IdUnidadeUTE_Nenhum) {


						// RESTRIÇÃO POR USINA
						else if (idUnidadeUTE == IdUnidadeUTE_Nenhum) {

							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {

								for (IdUnidadeUTE idUnidade = IdUnidadeUTE_1; idUnidade <= a_dados.getMaiorId(idTermeletrica, IdUnidadeUTE()); idUnidade++) {

									if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {

										if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidade) > -1)
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidade), posIneRE, a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

									}//if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {
									else {

										if (getVarDecisao_PTseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidade) > -1)
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidade), posIneRE, a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

									}//else {

								}//for (IdUnidadeUTE idUnidade = IdUnidadeUTE_1; idUnidade <= a_dados.getMaiorId(idTermeletrica, IdUnidadeUTE()); idUnidade++) {

							}//if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {

							else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina) {

								if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {

									if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), posIneRE, a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

								}//if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {
								else {

									if (getVarDecisao_PTseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), posIneRE, a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

								}//else {

							}//else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina) {

						} // else if (idUnidadeUTE == IdUnidadeUTE_Nenhum) {

					} // else if (idTermeletrica != IdTermeletrica_Nenhum) {

					// RESTRIÇÃO DE INTERCAMBIO
					else if (idIntercambio != IdIntercambio_Nenhum) {
						if (getVarDecisao_PIseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambio))
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambio), posIneRE, a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

					}//else if (idIntercambio != IdIntercambio_Nenhum) {

					// CONTRATOS
					else if (idContrato != IdContrato_Nenhum) {

						if ((getVarDecisao_CEXPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idContrato) > -1))
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_CEXP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idContrato), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

						else if ((getVarDecisao_CIMPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idContrato) > -1))
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_CIMP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idContrato), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

						else
							throw std::invalid_argument("Nao encontrado " + getFullString(idContrato) + " na " + getFullString(idRestricaoEletrica));
					}//else if (idContrato != IdContrato_Nenhum) {		

					// DEMANDA ESPECIAL
					else if (idDemandaEspecial != IdDemandaEspecial_Nenhum)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_DE(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idDemandaEspecial), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

					// DEMANDA ESPECIAL
					else if (idUsinaEolica != IdUsinaEolica_Nenhum)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PE(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaEolica), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

					else
						throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoEletrica));

				} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

			} // if (getVarDecisao_RE_FINFseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoEletrica) > -1){


			// RESTRIÇÃO DE FOLGA SUPERIOR
			if (getVarDecisao_RE_FSUPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoEletrica) > -1) {

				const int posIneRE = addIneLinear_RESTRICAO_ELETRICA_LIMITE_SUPERIOR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoEletrica);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, AttMatrizRestricaoEletrica_potencia_maxima, a_periodo_estudo, a_idPatamarCarga, double()));

				// Variável RE_FSUP
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RE_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoEletrica), posIneRE, 1.0);

				const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoEletrica, IdElementoSistema());
				for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

					const IdHidreletrica       idHidreletrica = a_dados.getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
					const IdConjuntoHidraulico idConjuntoHidraulico = a_dados.getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_conjuntohidraulico, IdConjuntoHidraulico());
					const IdUnidadeUHE         idUnidadeUHE = a_dados.getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_unidadehidraulica, IdUnidadeUHE());
					const IdTermeletrica       idTermeletrica = a_dados.getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_termeletrica, IdTermeletrica());
					const IdContrato           idContrato = a_dados.getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_contrato, IdContrato());
					const IdUnidadeUTE         idUnidadeUTE = a_dados.getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_unidadeTermeletrica, IdUnidadeUTE());
					const IdIntercambio        idIntercambio = a_dados.getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_intercambio, IdIntercambio());
					const IdDemandaEspecial    idDemandaEspecial = a_dados.getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_demandaEspecial, IdDemandaEspecial());
					const IdUsinaEolica        idUsinaEolica = a_dados.getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_eolica, IdUsinaEolica());

					if (idHidreletrica != IdHidreletrica_Nenhum) {

						const TipoDetalhamentoProducaoHidreletrica tipo_detalhamento_producao = a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica());

						// RESTRIÇÃO POR CONJUNTO
						if (idConjuntoHidraulico != IdConjuntoHidraulico_Nenhum) {

							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina)
								throw std::invalid_argument(" Nao e possivel representar a restricao eletrica" + getFullString(idRestricaoEletrica) + " do" + getFullString(idConjuntoHidraulico) + "da" + getFullString(idHidreletrica) + " pois o tipo de detalhamento da producao e " + getFullString(tipo_detalhamento_producao));

							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

								if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

									if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

								}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
								else {

									if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

								}//else {

							}

							if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) && (idUnidadeUHE == IdUnidadeUHE_Nenhum)) {

								for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= a_dados.getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidade++) {

									if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

										if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidade) > -1)
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidade), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

									}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
									else {

										if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidade) > -1)
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidade), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

									}//else {

								}//for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= a_dados.getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidade++)

							}//if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) && (idUnidadeUHE == IdUnidadeUTE_Nenhum)) {


							// RESTRIÇÃO POR UNIDADE
							if (idUnidadeUHE != IdUnidadeUHE_Nenhum) {

								if (tipo_detalhamento_producao != TipoDetalhamentoProducaoHidreletrica_por_unidade)
									throw std::invalid_argument(" Nao e possivel representar a restricao eletrica" + getFullString(idRestricaoEletrica) + "da" + getFullString(idUnidadeUHE) + " do" + getFullString(idConjuntoHidraulico) + "da" + getFullString(idHidreletrica) + " pois o tipo de detalhamento da producao e " + getFullString(tipo_detalhamento_producao));

								if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

									if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

								}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
								else {

									if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

								}//else {

							} // if (idUnidadeUTE != IdUnidadeUHE_Nenhum) {

						}//if (idConjuntoHidraulico != IdConjuntoHidraulico_Nenhum) {


						// RESTRIÇÃO POR HIDRELETRICA
						else if (idConjuntoHidraulico == IdConjuntoHidraulico_Nenhum) {

							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina) {

								if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

									if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

								}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
								else {

									if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

								}//else {

							}//if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina) {


							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

								for (IdConjuntoHidraulico idConjunto = IdConjuntoHidraulico_1; idConjunto <= a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjunto++) {

									if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

										if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjunto) > -1)
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjunto), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

									}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
									else {

										if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjunto) > -1)
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjunto), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

									}//else {

								}//for (IdConjuntoHidraulico idConjunto = IdConjuntoHidraulico_1; idConjunto <= a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjunto++) {

							}//if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

							if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {

								for (IdConjuntoHidraulico idConjunto = IdConjuntoHidraulico_1; idConjunto <= a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjunto++) {

									for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= a_dados.getMaiorId(idHidreletrica, idConjunto, IdUnidadeUHE()); idUnidade++) {

										if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {

											if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjunto, idUnidade) > -1)
												vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjunto, idUnidade), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

										}//if (a_dados.getAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, bool())) {
										else {

											if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjunto, idUnidade) > -1)
												vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjunto, idUnidade), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

										}//else {									

									}//for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= a_dados.getMaiorId(idHidreletrica, idConjunto, IdUnidadeUHE()); idUnidade++) {

								}//for (IdConjuntoHidraulico idConjunto = IdConjuntoHidraulico_1; idConjunto <= a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjunto++) {

							}//if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {

						}//else if (idConjuntoHidraulico == IdConjuntoHidraulico_Nenhum) {

					} // if (idHidreletrica != IdHidreletrica_Nenhum) {


					// RESTRIÇÃO PARA AS TERMELETRICAS 
					else if (idTermeletrica != IdTermeletrica_Nenhum) {

						const TipoDetalhamentoProducaoTermeletrica tipo_detalhamento_producao = a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica());

						// RESTIÇÃO POR UNIDADE
						if (idUnidadeUTE != IdUnidadeUTE_Nenhum) {

							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina)
								throw std::invalid_argument(" Nao e possivel representar a restricao eletrica" + getFullString(idRestricaoEletrica) + "da" + getFullString(idUnidadeUTE) + "da" + getFullString(idTermeletrica) + " pois o tipo de detalhamento da producao e " + getFullString(tipo_detalhamento_producao));

							if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {

								if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE) > -1)
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

							}//if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {
							else {

								if (getVarDecisao_PTseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE) > -1)
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

							}//else {					

						} // if (idUnidadeUTE != IdUnidadeUTE_Nenhum) {


						// RESTRIÇÃO POR USINA
						else if (idUnidadeUTE == IdUnidadeUTE_Nenhum) {

							if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {

								for (IdUnidadeUTE idUnidade = IdUnidadeUTE_1; idUnidade <= a_dados.getMaiorId(idTermeletrica, IdUnidadeUTE()); idUnidade++) {

									if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {

										if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidade) > -1)
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidade), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

									}//if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {
									else {

										if (getVarDecisao_PTseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidade) > -1)
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidade), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

									}//else {

								}//for (IdUnidadeUTE idUnidade = IdUnidadeUTE_1; idUnidade <= a_dados.getMaiorId(idTermeletrica, IdUnidadeUTE()); idUnidade++) {

							}//if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {

							else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina) {

								if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {

									if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

								}//if (a_dados.getAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, bool())) {
								else {

									if (getVarDecisao_PTseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

								}//else {

							}//else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina) {

						} // else if (idUnidadeUTE == IdUnidadeUTE_Nenhum) {

					} // else if (idTermeletrica != IdTermeletrica_Nenhum) {


					// RESTRIÇÃO DE INTERCAMBIO
					else if (idIntercambio != IdIntercambio_Nenhum) {
						if (getVarDecisao_PIseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambio))
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambio), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));
					}//else if (idIntercambio != IdIntercambio_Nenhum) {


					// CONTRATOS
					else if (idContrato != IdContrato_Nenhum) {

						if ((getVarDecisao_CEXPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idContrato) > -1))
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_CEXP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idContrato), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

						else if ((getVarDecisao_CIMPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idContrato) > -1))
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_CIMP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idContrato), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

						else
							throw std::invalid_argument("Nao encontrado " + getFullString(idContrato) + " na " + getFullString(idRestricaoEletrica));
					}//else if (idContrato != IdContrato_Nenhum) {

					// DEMANDA ESPECIAL
					else if (idDemandaEspecial != IdDemandaEspecial_Nenhum)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_DE(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idDemandaEspecial), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

					// DEMANDA ESPECIAL
					else if (idUsinaEolica != IdUsinaEolica_Nenhum)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PE(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaEolica), posIneRE, -a_dados.getElementoMatriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));



					else
						throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoEletrica));

				} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

			} // if (getVarDecisao_RE_FINFseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoEletrica) > -1){

		} // for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= maiorIdRestricaoEletrica; idRestricaoEletrica++) {

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

		const Periodo periodo_otimizacao = getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

		if (multiplicidade_corte == 0) {

			const int posIneZF = addIneLinear_CB_ZF(a_TSS, a_idEstagio, periodo_otimizacao, IdRealizacao_1, IdCorteBenders_1);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneZF, 0.0);

			if (getVarDecisao_ZFseExistir(a_TSS, a_idEstagio, periodo_otimizacao, IdRealizacao_1) == -1)
				addVarDecisao_ZF(a_TSS, a_idEstagio, periodo_otimizacao, IdRealizacao_1, 0.0, vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZF(a_TSS, a_idEstagio, periodo_otimizacao, IdRealizacao_1), posIneZF, 1.0);

		} // if (multiplicidade_corte == 0) {

		else if (multiplicidade_corte > 0) {

			const Periodo periodo_otimizacao = getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

			for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getAtributo(idEstagio_seguinte, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); idRealizacao++) {
				const int posIneZF = addIneLinear_CB_ZF(a_TSS, a_idEstagio, periodo_otimizacao, idRealizacao, IdCorteBenders_1);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneZF, 0.0);

				if (getVarDecisao_ZFseExistir(a_TSS, a_idEstagio, periodo_otimizacao, idRealizacao) == -1)
					addVarDecisao_ZF(a_TSS, a_idEstagio, periodo_otimizacao, idRealizacao, 0.0, vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZF(a_TSS, a_idEstagio, periodo_otimizacao, idRealizacao), posIneZF, 1.0);
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

		const Periodo periodo_otimizacao = getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

		if (multiplicidade_corte == 0) {

			const int posIneZT = addIneLinear_CB_ZT(a_TSS, a_idEstagio, periodo_otimizacao, IdRealizacao_1, IdCorteBenders_1);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneZT, 0.0);

			if (getVarDecisao_ZTseExistir(a_TSS, a_idEstagio, periodo_otimizacao, IdRealizacao_1) == -1)
				addVarDecisao_ZT(a_TSS, a_idEstagio, periodo_otimizacao, IdRealizacao_1, 0.0, vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZT(a_TSS, a_idEstagio, periodo_otimizacao, IdRealizacao_1), posIneZT, 1.0);

		} // if (multiplicidade_corte == 0) {

		else if (multiplicidade_corte > 0) {

			for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getAtributo(a_idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); idRealizacao++) {
				const int posIneZT = addIneLinear_CB_ZT(a_TSS, a_idEstagio, periodo_otimizacao, idRealizacao, IdCorteBenders_1);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneZT, 0.0);

				if (getVarDecisao_ZTseExistir(a_TSS, a_idEstagio, periodo_otimizacao, idRealizacao) == -1)
					addVarDecisao_ZT(a_TSS, a_idEstagio, periodo_otimizacao, idRealizacao, 0.0, vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZT(a_TSS, a_idEstagio, periodo_otimizacao, idRealizacao), posIneZT, 1.0);
			} // for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getAtributo(a_idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); idRealizacao++) {

		} // if (multiplicidade_corte > 0) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesCorteBendersEmCustoTotal(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }

}


void ModeloOtimizacao::criarRestricoesVazaoBombeada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdUsinaElevatoria a_idUsinaElevatoria, const IdPatamarCarga a_idPatamarCarga) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (getVarDecisao_QBOM_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idUsinaElevatoria) > -1) {

			const int posIneQBOM_FINF = addIneLinear_VAZAO_BOMBEADA_FOLGA_INFERIOR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idUsinaElevatoria);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneQBOM_FINF, a_dados.getElementoVetor(a_idUsinaElevatoria, AttVetorUsinaElevatoria_vazao_bombeada_minima, a_periodo_estudo, double()));

			// Variável QBOM_FINF
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idUsinaElevatoria), posIneQBOM_FINF, 1.0);

			// Variável QBOM
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idUsinaElevatoria), posIneQBOM_FINF, 1.0);

		} // if (getVarDecisao_QBOM_FINFseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idUsinaElevatoria) > -1) {

		if (getVarDecisao_QBOMDISP_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idUsinaElevatoria) > -1) {

			const int posIneQBOMDISP_FINF = addIneLinear_VAZAO_BOMBEADA_DISPONIVEL_FOLGA_INFERIOR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idUsinaElevatoria);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneQBOMDISP_FINF, a_dados.getElementoMatriz(a_idUsinaElevatoria, AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_minima, a_periodo_estudo, a_idPatamarCarga, double()));

			// Variável QBOMDISP_FINF
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOMDISP_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idUsinaElevatoria), posIneQBOMDISP_FINF, 1.0);

			// Variável QBOMDISP
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOMDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idUsinaElevatoria), posIneQBOMDISP_FINF, 1.0);

		} // if (getVarDecisao_QBOMDISP_FINFseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idUsinaElevatoria) > -1) {

		// Restrição VAZAO_BOMBEADA_DISPONIVEL
		if (getVarDecisao_QBOMDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idUsinaElevatoria) > -1) {

			const int posEquQBOMDISP = addEquLinear_VAZAO_BOMBEADA_DISPONIVEL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idUsinaElevatoria);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQBOMDISP, 0.0);

			// QBOMDISP
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOMDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idUsinaElevatoria), posEquQBOMDISP, 1.0);

			// QBOM
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idUsinaElevatoria), posEquQBOMDISP, -a_dados.getElementoVetor(a_idUsinaElevatoria, AttVetorUsinaElevatoria_fator_disponibilidade, a_periodo_estudo, double()));

		}//if (getVarDecisao_QBOMDISPseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idUsinaElevatoria) > -1) {

	}

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesVazaoBombeada(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesVazaoRetirada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (getVarDecisao_QOUT_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1) {

			const int posEquQOUT_FINF = addEquLinear_VAZAO_RETIRADA_FOLGA_INFERIOR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQOUT_FINF, a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_vazao_retirada, a_periodo_estudo, double()));

			// Variável QOUT_FINF
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QOUT_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquQOUT_FINF, 1.0);

			// Variável QOUT
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QOUT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquQOUT_FINF, 1.0);

		} // if (getVarDecisao_QOUT_FINFseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1) {

	}

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesvazaoRetirada(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesVazaoDesviada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (getVarDecisao_QDES_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica())) > -1) {

			const int posEquQDES_FINF = addIneLinear_VAZAO_DESVIADA_FOLGA_INFERIOR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica()));

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQDES_FINF, a_dados.getElementoMatriz(a_idHidreletrica, AttMatrizHidreletrica_vazao_desviada_minima, a_periodo_estudo, a_idPatamarCarga, double()));

			// Variável QDES_FINF
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posEquQDES_FINF, 1.0);

			// Variável QDES
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posEquQDES_FINF, 1.0);

		} // if (getVarDecisao_QDES_FINFseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica())) > -1) {

	}

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesvazaoDesviada(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesVolumeUtil_e_Penalidade(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdHidreletrica a_idHidreletrica) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (getVarDecisao_VF_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1) {

			const int posIneVF_FINF = addIneLinear_VOLUME_UTIL_FOLGA_INFERIOR(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneVF_FINF, a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_minimo, a_periodo_estudo, double()));

			// Variável VF_FINF
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posIneVF_FINF, 1.0);

			// Variável VF
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posIneVF_FINF, 1.0);


			const IdMes mes_penalizacao_volume_util_minimo = a_dados.getAtributo(AttComumDados_mes_penalizacao_volume_util_minimo, IdMes());
			if (mes_penalizacao_volume_util_minimo != IdMes_Nenhum) {
				if (a_periodo_estudo == getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_estudo, a_idEstagio, Periodo()))
					criarVariaveisDecisao_VariaveisEstado_Restricoes_ZP0_VF_FINF(a_TSS, a_dados, a_idEstagio, Periodo());
			}

			else {

				const Periodo periodo_otimizacao = getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

				if (getVarDecisao_ZP0_VF_FINFseExistir(a_TSS, a_idEstagio, periodo_otimizacao) == -1) {

					addEquLinear_CUSTO_PENALIDADE_VOLUME_INFERIOR_0(a_TSS, a_idEstagio, periodo_otimizacao);

					addVarDecisao_ZP0_VF_FINF(a_TSS, a_idEstagio, periodo_otimizacao, 0.0, vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZP0_VF_FINF(a_TSS, a_idEstagio, periodo_otimizacao), getEquLinear_CUSTO_PENALIDADE_VOLUME_INFERIOR_0(a_TSS, a_idEstagio, periodo_otimizacao), 1.0);

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZP0_VF_FINF(a_TSS, a_idEstagio, periodo_otimizacao), getEquLinear_CUSTO_PENALIDADE_0(a_TSS, a_idEstagio, periodo_otimizacao), -1.0);

				} // if (getVarDecisao_ZP0_VF_FINFseExistir(a_TSS, a_idEstagio, periodo_otimizacao) == -1) {

				const double valor_penalidade = a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_penalidade_volume_util_minimo, double()) * a_dados.getElementoMatriz(AttMatrizDados_desagio_acumulado_horizonte_estudo, a_idEstagio, a_periodo_estudo, double());
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), getEquLinear_CUSTO_PENALIDADE_VOLUME_INFERIOR_0(a_TSS, a_idEstagio, periodo_otimizacao), -valor_penalidade);

			} // else {


		}//if (getVarDecisao_VI_FINFseExistir(a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1) {

	}

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesVolumeUtil_e_Penalidade(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesVazaoDefluente(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		// Por Periodo

		if (a_idPatamarCarga == IdPatamarCarga_1) {

			if (getVarDecisao_QDEF_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1) {

				const int posIneQDEF_FINF = addIneLinear_VAZAO_DEFLUENTE_FOLGA_INFERIOR(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneQDEF_FINF, a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_vazao_defluente_minima, a_periodo_estudo, double()));

				// Variável QDEF_FINF
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posIneQDEF_FINF, 1.0);

				// Variável QDEF
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posIneQDEF_FINF, 1.0);

			} // if (getVarDecisao_QDEF_FINFseExistir(a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1){

			if (getVarDecisao_QDEF_FSUPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1) {

				const int posIneQDEF_FSUP = addIneLinear_VAZAO_DEFLUENTE_FOLGA_SUPERIOR(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneQDEF_FSUP, -a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_vazao_defluente_maxima, a_periodo_estudo, double()));

				// Variável QDEF_FSUP
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posIneQDEF_FSUP, 1.0);

				// Variável QDEF
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posIneQDEF_FSUP, -1.0);

			} // if (getVarDecisao_QDEF_FSUPseExistir(a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1){

		} // if (a_idPatamarCarga == IdPatamarCarga_1){

		// Por Patamar

		if (getVarDecisao_QDEF_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1) {

			const int posIneQDEF_FINF = addIneLinear_VAZAO_DEFLUENTE_FOLGA_INFERIOR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneQDEF_FINF, a_dados.getElementoMatriz(a_idHidreletrica, AttMatrizHidreletrica_vazao_defluente_minima, a_periodo_estudo, a_idPatamarCarga, double()));

			// Variável QDEF_FINF
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posIneQDEF_FINF, 1.0);

			// Variável QDEF
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posIneQDEF_FINF, 1.0);

		} // if (getVarDecisao_QDEF_FINFseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1) {

		if (getVarDecisao_QDEF_FSUPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1) {

			const int posIneQDEF_FSUP = addIneLinear_VAZAO_DEFLUENTE_FOLGA_SUPERIOR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneQDEF_FSUP, -a_dados.getElementoMatriz(a_idHidreletrica, AttMatrizHidreletrica_vazao_defluente_maxima, a_periodo_estudo, a_idPatamarCarga, double()));

			// Variável QDEF_FSUP
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posIneQDEF_FSUP, 1.0);

			// Variável QDEF
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posIneQDEF_FSUP, -1.0);

		} // if (getVarDecisao_QDEF_FSUPseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1){


	}

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesvazaoDefluente(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }
}

void ModeloOtimizacao::criarRestricoesBombeamentoHidraulico(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdPatamarCarga a_idPatamarCarga, const IdUsinaElevatoria a_idUsinaElevatoria) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		if (getVarDecisao_QBOMseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idUsinaElevatoria) > -1) {

			const int posEquBOM = addEquLinear_POTENCIA_BOMBEAMENTO_DISPONIVEL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idUsinaElevatoria);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquBOM, 0.0);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PBOMDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idUsinaElevatoria), posEquBOM, 1.0);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idUsinaElevatoria), posEquBOM, -a_dados.getAtributo(a_idUsinaElevatoria, AttComumUsinaElevatoria_taxa_de_consumo, double()) * a_dados.getElementoVetor(a_idUsinaElevatoria, AttVetorUsinaElevatoria_fator_disponibilidade, a_periodo_estudo, double()));

		} // if (getVarDecisao_QBOMseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaElevatoria) > -1) {

	}//try {

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesBombeamentoHidraulico(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }

}


void ModeloOtimizacao::criarRestricoesIntercambioRelaxado(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdIntercambio a_idIntercambio, const IdPatamarCarga a_idPatamarCarga) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		// Restrição VAZAO_TURBINADA_DISPONIVEL_RELAXADA
		if (getVarDecisao_PI_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idIntercambio) > -1) {

			const int posInePI_FINF = addIneLinear_INTERCAMBIO_RELAXADO(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idIntercambio);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posInePI_FINF, a_dados.getElementoMatriz(a_idIntercambio, AttMatrizIntercambio_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()));

			// PI
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idIntercambio), posInePI_FINF, 1.0);

			// PI_FINF
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idIntercambio), posInePI_FINF, 1.0);

		} // if (getVarDecisao_PI_FINFseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idIntercambio) > -1) {

	}//try {

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesIntercambioRelaxado(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }

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

		const IdHidreletrica idHidreletrica = getIdHidreletricaFromChar(valor.c_str());
		if (idHidreletrica == IdHidreletrica_Nenhum)
			throw std::invalid_argument("Hidreletrica invalida em " + getFullString(a_idVariavelAleatoriaInterna));

		return idHidreletrica;

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


void ModeloOtimizacao::criarRestricoesTurbinamentoHidraulico(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		const TipoDetalhamentoProducaoHidreletrica tipo_detalhamento_producao = a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica());

		//
		// Turbinamento Por Hidreletrica
		//

		// Restrição VAZAO_TURBINADA
		if (getVarDecisao_QTURseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1) {

			if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) || (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {

				const int posEquQTUR = addEquLinear_VAZAO_TURBINADA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQTUR, 0.0);

				// Variável Turbinamento Hidraulico QTUR
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquQTUR, 1.0);

				for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posEquQTUR, -1.0);

			} // if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) || (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {

		} // if (getVarDecisao_QTURseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1){

		// Restrição VAZAO_TURBINADA_RELAXADA
		if (getVarDecisao_QTUR_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1) {

			const int posIneQTUR_FINF = addIneLinear_VAZAO_TURBINADA_RELAXADA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneQTUR_FINF, a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_vazao_turbinada_minima, a_periodo_estudo, double()));

			// Variável Turbinamento Hidraulico QTUR
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posIneQTUR_FINF, 1.0);

			// Variável Turbinamento Hidraulico Folga Minima QTUR_FINF
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posIneQTUR_FINF, 1.0);

		} // if (getVarDecisao_QTUR_RELseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1){

		// Restrição VAZAO_TURBINADA_DISPONIVEL_RELAXADA
		if (getVarDecisao_QTURDISP_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1) {

			const int posIneQTURDISP_FINF = addIneLinear_VAZAO_TURBINADA_DISPONIVEL_RELAXADA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneQTURDISP_FINF, a_dados.getElementoMatriz(a_idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_minima, a_periodo_estudo, a_idPatamarCarga, double()));

			// Variável Turbinamento Hidraulico QTURDISP
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posIneQTURDISP_FINF, 1.0);

			// Variável Turbinamento Hidraulico Folga Minima QTURDISP_FINF
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posIneQTURDISP_FINF, 1.0);

		} // if (getVarDecisao_QTURDISP_RELseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1){

		// Restrição VAZAO_TURBINADA_DISPONIVEL
		if (getVarDecisao_QTURDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1) {

			const int posEquQTURDISP = addEquLinear_VAZAO_TURBINADA_DISPONIVEL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQTURDISP, 0.0);

			// Variável Turbinamento Hidraulico QTURDISP
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquQTURDISP, 1.0);

			if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquQTURDISP, -a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_disponibilidade, a_periodo_estudo, double()));

			else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {
				for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posEquQTURDISP, -1.0);
			} // else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

			else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) {
				for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {
					for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquQTURDISP, -1.0);
				}
			} // else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) {

		} // if (getVarDecisao_QTURDISPseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1){

		//
		// Turbinamento Por Conjunto Hidraulico
		//

		for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

			// Restrição VAZAO_TURBINADA
			if (getVarDecisao_QTURseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico) > -1) {

				if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) {

					const int posEquQTUR = addEquLinear_VAZAO_TURBINADA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico);

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQTUR, 0.0);

					// Variável Turbinamento Hidraulico QTUR
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posEquQTUR, 1.0);

					for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquQTUR, -1.0);

				} // if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) {

			} // if (getVarDecisao_QTURseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico) > -1){

			// Restrição VAZAO_TURBINADA_RELAXADA
			if (getVarDecisao_QTUR_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico) > -1) {

				const int posEquQTUR_FINF = addEquLinear_VAZAO_TURBINADA_RELAXADA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQTUR_FINF, a_dados.getElementoVetor(a_idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_minima, a_periodo_estudo, double()));

				// Variável Turbinamento Hidraulico QTUR
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posEquQTUR_FINF, -1.0);

				// Variável Turbinamento Hidraulico Folga Minima QTUR_FINF
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posEquQTUR_FINF, -1.0);

			} // if (getVarDecisao_QTUR_FINFseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico) > -1) {

			// Restrição VAZAO_TURBINADA_DISPONIVEL
			if (getVarDecisao_QTURDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico) > -1) {

				const int posEquQTURDISP = addEquLinear_VAZAO_TURBINADA_DISPONIVEL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQTURDISP, 0.0);

				// Variável Turbinamento Hidraulico QTURDISP
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posEquQTURDISP, 1.0);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posEquQTURDISP, -a_dados.getElementoVetor(a_idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_disponibilidade, a_periodo_estudo, double()));

			} // if (getVarDecisao_QTURDISPseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico) > -1){

			//
			// Turbinamento Por Unidade UHE
			//

			for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {

				// Restrição VAZAO_TURBINADA_RELAXADA
				if (getVarDecisao_QTUR_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

					const int posEquQTUR_FINF = addEquLinear_VAZAO_TURBINADA_RELAXADA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE);

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQTUR_FINF, a_dados.getElementoVetor(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_vazao_turbinada_minima, a_periodo_estudo, double()));

					// Variável Turbinamento Hidraulico QTUR
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquQTUR_FINF, 1.0);

					// Variável Turbinamento Hidraulico Folga Minima QTUR_FINF
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquQTUR_FINF, 1.0);

				} // if (getVarDecisao_QTUR_FINFseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

				// Restrição VAZAO_TURBINADA_DISPONIVEL
				if (getVarDecisao_QTURDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

					const int posEquQTURDISP = addEquLinear_VAZAO_TURBINADA_DISPONIVEL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE);

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQTURDISP, 0.0);

					// Variável Turbinamento Hidraulico QTURDISP
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquQTURDISP, 1.0);

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquQTURDISP, -a_dados.getElementoVetor(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_disponibilidade, a_periodo_estudo, double()));

				} // if (getVarDecisao_QTURDISPseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1){

			} // for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {
		} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

	}//try {

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesTurbinamentoHidraulico(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }

}

void ModeloOtimizacao::criarVariaveisDecisao_Restricoes_ProcessoEstocasticoHidrologico(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		const bool viabilidade_hidraulica = getAtributo(AttComumModeloOtimizacao_relaxar_afluencia_incremental_com_viabilidade_hidraulica, bool());

		const IdProcessoEstocastico idProcEstocastico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

		const Periodo periodo_otimizacao = getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

		SmartEnupla<Periodo, double> horizonte_estudo            = getElementosMatriz(AttMatrizModeloOtimizacao_horizonte_estudo, a_idEstagio, Periodo(), double());
		SmartEnupla<Periodo, int> horizonte_processo_estocastico = getElementosMatriz(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, a_idEstagio, Periodo(), int());

		const TipoLagAutocorrelacao tipo_lag_autocorrelacao = getAtributo(idProcEstocastico, AttComumProcessoEstocastico_tipo_lag_autocorrelacao, TipoLagAutocorrelacao());

		const double infinito = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getInfinito();

		for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= getMaiorId(idProcEstocastico, IdVariavelAleatoria()); idVariavelAleatoria++) {

			const std::vector<IdHidreletrica> lista_hidreletrica = getIdHidreletricaFromIdProcessoEstocasticoIdVariavelAleatoria(idProcEstocastico, idVariavelAleatoria);

			if (lista_hidreletrica.size() == 0)
				throw std::invalid_argument("Nao foram encontradas hidreletricas associada a :" + getFullString(idProcEstocastico) + " " + getFullString(idVariavelAleatoria));

			for (Periodo periodo_processo_estocastico = horizonte_processo_estocastico.getIteradorInicial(); periodo_processo_estocastico <= horizonte_processo_estocastico.getIteradorFinal(); horizonte_processo_estocastico.incrementarIterador(periodo_processo_estocastico)) {

				const TipoRelaxacaoVariavelAleatoria tipo_relaxacao = getElementoVetor(idProcEstocastico, idVariavelAleatoria, AttVetorVariavelAleatoria_tipo_relaxacao, periodo_processo_estocastico, TipoRelaxacaoVariavelAleatoria());

				//
				//
				// Parcela referente ao processo estocastico
				//
				//

				// varRP
				const int varRP = addVarDecisao_RP(a_TSS, a_idEstagio, periodo_otimizacao, periodo_processo_estocastico, idProcEstocastico, idVariavelAleatoria, -infinito, infinito, 0.0);

				vetorEstagio.att(a_idEstagio).addVariavelRealizacao(a_TSS, getNomeVarDecisao_RP(a_TSS, a_idEstagio, periodo_otimizacao, periodo_processo_estocastico, idProcEstocastico, idVariavelAleatoria), varRP, idProcEstocastico, idVariavelAleatoria, periodo_processo_estocastico, 1.0);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->addVarDinamica(varRP);

				// varYP
				const int varYP = addVarDecisao_YP(a_TSS, a_idEstagio, periodo_otimizacao, periodo_processo_estocastico, idProcEstocastico, idVariavelAleatoria, -infinito, infinito, 0.0);

				// Restricao YP (YP = RP + SOMA(fp*YPt-1))
				const int posEquYP = addEquLinear_YP(a_TSS, a_idEstagio, periodo_otimizacao, periodo_processo_estocastico, idProcEstocastico, idVariavelAleatoria);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYP, posEquYP, 1.0);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varRP, posEquYP, -1.0);

				const double grau_liberdade_var_aleatoria = vetorProcessoEstocastico.att(idProcEstocastico).getGrauLiberdade(idVariavelAleatoria);

				// Variável do Processo Estocástico Hidrológico YP lag = 0 ... lag = np
				if (getSize1Matriz(idProcEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao) > 0) {

					for (int lag = 1; lag <= getSize2Matriz(idProcEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, periodo_processo_estocastico); lag++) {

						if (getElementoMatriz(idProcEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, periodo_processo_estocastico, lag, double()) != 0.0) {
				
							Periodo periodo_lag = periodo_processo_estocastico - lag;

							int var_YP_LAG = criarVariaveisDecisao_VariaveisEstado_Restricoes_YP(a_TSS, a_dados, a_idEstagio, periodo_otimizacao, idProcEstocastico, idVariavelAleatoria, periodo_lag, grau_liberdade_var_aleatoria);
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(var_YP_LAG, posEquYP, -getElementoMatriz(idProcEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, periodo_processo_estocastico, lag, double()));

						} // if (getElementoMatriz(idProcEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, periodo_processo_estocastico, lag, double()) > 0.0){
					} // for (int lag = 1; lag <= getSize2Matriz(idProcEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao_sazonal, periodo_processo_estocastico.getEstacao()); lag++) {
				} // if (getSize1Matriz(idProcEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao) > 0) {

				//
				//
				// Parcela referente às afluencias do balanço hídrico
				//
				//

				bool sobreposicao_encontrada = false;
				for (Periodo periodo_estudo = horizonte_estudo.getIteradorInicial(); periodo_estudo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo_estudo)) {

					const double sobreposicao = periodo_estudo.sobreposicao(periodo_processo_estocastico);

					if (sobreposicao > 0.0) {

						for (int i = 0; i < int(lista_hidreletrica.size()); i++) {
							const IdHidreletrica idHidreletrica = lista_hidreletrica.at(i);

							const IdVariavelAleatoriaInterna idVarInt = getIdVariavelAleatoriaInternaFromIdVariavelAleatoriaIdHidreletrica(idProcEstocastico, idVariavelAleatoria, idHidreletrica);

							const double grau_liberdade = getAtributo(idProcEstocastico, idVariavelAleatoria, idVarInt, AttComumVariavelAleatoriaInterna_grau_liberdade, double());
							const double coeficiente_participacao = getElementoVetor(idProcEstocastico, idVariavelAleatoria, idVarInt, AttVetorVariavelAleatoriaInterna_coeficiente_participacao, periodo_processo_estocastico, double());

							// Afluencia Incremental (YH): definida por periodo estudo
							const int varYH = addVarDecisao_YH(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica, -infinito, infinito, 0.0);

							// Restricao Afluencia Incremental: definida por periodo estudo
							const int posEquYH = addEquLinear_AFLUENCIA_INCREMENTAL(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica);

							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquYH, -grau_liberdade * sobreposicao);

							// Variável YH
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYH, posEquYH, 1.0);

							// Variável YP
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYP, posEquYH, -coeficiente_participacao * sobreposicao);

							if (tipo_relaxacao != TipoRelaxacaoVariavelAleatoria_sem_relaxacao) {

								if (true) {
									// Variável Afluencia Incremental Folga (YHF): definida por periodo estudo (Quando penalizada ou utilizando viabilidade hidráulica)
									int varYHF = getVarDecisao_YHFseExistir(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica);

									if (varYHF == -1) {

										if (viabilidade_hidraulica) {

											if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica)
												varYHF = addVarDecisao_YHF(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica, 0.0, 0.0, 0.0);

											else if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
												varYHF = addVarDecisao_YHF(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica, 0.0, infinito, 100.0);

											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->addVarDinamica(varYHF);

											vetorEstagio.att(a_idEstagio).addVariavelRealizacaoInterna(a_TSS, getNomeVarDecisao_YHF(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica), varYHF, idProcEstocastico, idVariavelAleatoria, idVarInt, periodo_processo_estocastico, sobreposicao, TipoValor_positivo, 1.0, 1.0);

										} // if (viabilidade_hidraulica) {

										else if (tipo_relaxacao == TipoRelaxacaoVariavelAleatoria_penalizacao)
											varYHF = addVarDecisao_YHF(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica, 0.0, infinito, 0.0);


									} // if (varYHF == -1) {

									// Variável Afluencia Incremental Folga (YHF): definida por periodo estudo
									if (varYHF > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYHF, posEquYH, -sobreposicao);

								} // if (true) {

								// Variável Afluencia Proc. Estocastico Folga (YPF): definida por periodo proc. estocastico (Quando truncada)
								int varYPF = getVarDecisao_YPFseExistir(a_TSS, a_idEstagio, periodo_otimizacao, periodo_processo_estocastico, idProcEstocastico, idVariavelAleatoria);

								if (varYPF == -1) {
									if ((tipo_relaxacao == TipoRelaxacaoVariavelAleatoria_truncamento) || (tipo_relaxacao == TipoRelaxacaoVariavelAleatoria_truncamento_penalizacao)) {
										varYPF = addVarDecisao_YPF(a_TSS, a_idEstagio, periodo_otimizacao, periodo_processo_estocastico, idProcEstocastico, idVariavelAleatoria, 0.0, 0.0, 0.0);
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->addVarDinamica(varYPF);
										vetorEstagio.att(a_idEstagio).addVariavelRealizacaoInterna(a_TSS, getNomeVarDecisao_YPF(a_TSS, a_idEstagio, periodo_otimizacao, periodo_processo_estocastico, idProcEstocastico, idVariavelAleatoria), varYPF, idProcEstocastico, idVariavelAleatoria, IdVariavelAleatoriaInterna_Nenhum, periodo_processo_estocastico, sobreposicao, TipoValor_positivo, 1.0, 1.0);
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYPF, posEquYP, -1.0);
									}

								} // if (varYPF == -1) {

							} // if (tipo_relaxacao != TipoRelaxacaoVariavelAleatoria_sem_relaxacao) {
						} // for (int i = 0; i < int(lista_hidreletrica.size()); i++) {

						if (!sobreposicao_encontrada)
							sobreposicao_encontrada = true;
					}
					else if ((sobreposicao == 0.0) && (sobreposicao_encontrada))
						break;

				} // for (Periodo periodo_estudo = horizonte_estudo.getIteradorInicial(); periodo_estudo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo_estudo)) {

			} // for (Periodo periodo_processo_estocastico = periodo_processo_estocastico_inicial; periodo_processo_estocastico <= periodo_processo_estocastico_final; a_horizonte_processo_estocastico_estagio.incrementarIterador(periodo_processo_estocastico)) {

		} // for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= getMaiorId(idProcEstocastico, IdVariavelAleatoria()); idVariavelAleatoria++) {

	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisDecisao_Restricoes_ProcessoEstocasticoHidrologico(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesCusto_Total_Imediato_Futuro(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdEstagio a_estagio_final, const IdHidreletrica a_maiorIdHidreletrica, const Periodo a_periodo_otimizacao, const SmartEnupla<Periodo, double> a_horizonte_estudo_estagio, const  IdProcessoEstocastico a_tipo_processo_estocastico_hidrologico) {

	try {

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		if (a_TSS != TipoSubproblemaSolver_mestre) {

			// ---------------------------------------------------------------------------------------------------------------------
			//                                  Restrição Custo Total (ZT = ZI + ZF)
			// ---------------------------------------------------------------------------------------------------------------------

			try {

				const int posEquZT = addEquLinear_CUSTO_TOTAL(a_TSS, a_idEstagio, a_periodo_otimizacao);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZT, 0.0);

				// Variável ZT
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZT(a_TSS, a_idEstagio, a_periodo_otimizacao), posEquZT, 1.0);

				// Variável ZI
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZI(a_TSS, a_idEstagio, a_periodo_otimizacao), posEquZT, -1.0);

				// Variável ZF
				if (getVarDecisao_ZFseExistir(a_TSS, a_idEstagio, a_periodo_otimizacao) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZF(a_TSS, a_idEstagio, a_periodo_otimizacao), posEquZT, -1.0);


			} // try{
			catch (const std::exception& erro) { throw std::invalid_argument("Restricao Custo Total: \n" + std::string(erro.what())); }


			// ---------------------------------------------------------------------------------------------------------------------
			//                                  Restrição Custo Futuro (ZF = ZF_IdRealizacao_1 + .... + ZF_IdRealizacao_N)
			// ---------------------------------------------------------------------------------------------------------------------

			try {

				if (a_idEstagio < getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio())) {

					const IdEstagio idEstagio_seguinte = IdEstagio(a_idEstagio + 1);

					const int multiplicidade_corte = a_dados.getElementoVetor(AttVetorDados_cortes_multiplos, idEstagio_seguinte, int());

					const Periodo periodo_otimizacao_seguinte = getAtributo(idEstagio_seguinte, AttComumEstagio_periodo_otimizacao, Periodo());

					const int posEquZF = addEquLinear_CUSTO_FUTURO(a_TSS, a_idEstagio, a_periodo_otimizacao);

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZF, 0.0);

					// Variável ZF
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZF(a_TSS, a_idEstagio, a_periodo_otimizacao), posEquZF, 1.0);

					for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getAtributo(idEstagio_seguinte, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); idRealizacao++) {

						if (multiplicidade_corte == 0) {//Single cut
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZF(a_TSS, a_idEstagio, a_periodo_otimizacao, idRealizacao), posEquZF, -1.0);
							break;
						}
						else if (multiplicidade_corte > 0) {//Multi cut

							const double probabilidade_abertura = getElementoMatriz(a_tipo_processo_estocastico_hidrologico, AttMatrizProcessoEstocastico_probabilidade_realizacao, periodo_otimizacao_seguinte, idRealizacao, double());

							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZF(a_TSS, a_idEstagio, a_periodo_otimizacao, idRealizacao), posEquZF, -probabilidade_abertura);

						}//else if (multiplicidade_corte > 0) {

					}// for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getAtributo(idEstagio_seguinte, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); idRealizacao++) {

				} // if (a_idEstagio < getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio())) {

			} // try{
			catch (const std::exception& erro) { throw std::invalid_argument("Restricao Custo Futuro: \n" + std::string(erro.what())); }

			// ---------------------------------------------------------------------------------------------------------------------
			//                                  Restrição Custo Imediato (ZI = ZP0 + ZO0)
			// ---------------------------------------------------------------------------------------------------------------------

			try {

				const int posEquZI = addEquLinear_CUSTO_IMEDIATO(a_TSS, a_idEstagio, a_periodo_otimizacao);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZI, 0.0);

				// Variável ZI
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZI(a_TSS, a_idEstagio, a_periodo_otimizacao), posEquZI, 1.0);

				// Variável ZP0
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZP0(a_TSS, a_idEstagio, a_periodo_otimizacao), posEquZI, -1.0);

				// Variável ZO0
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZO0(a_TSS, a_idEstagio, a_periodo_otimizacao), posEquZI, -1.0);

			} // try{
			catch (const std::exception& erro) { throw std::invalid_argument("Restricao Custo Presente: \n" + std::string(erro.what())); }


			// ---------------------------------------------------------------------------------------------------------------------
			//                                  Restrição Custo Penalidade (ZP0 = ZP0p) Por Periodo Otimizacao
			// ---------------------------------------------------------------------------------------------------------------------

			try {

				const int posEquZP0 = addEquLinear_CUSTO_PENALIDADE_0(a_TSS, a_idEstagio, a_periodo_otimizacao);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZP0, 0.0);

				// Variável ZP0
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZP0(a_TSS, a_idEstagio, a_periodo_otimizacao), posEquZP0, 1.0);

				// Variável ZP0p
				for (Periodo periodo_estudo = a_horizonte_estudo_estagio.getIteradorInicial(); periodo_estudo <= a_horizonte_estudo_estagio.getIteradorFinal(); a_horizonte_estudo_estagio.incrementarIterador(periodo_estudo)) {

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZP0p(a_TSS, a_idEstagio, periodo_estudo), posEquZP0, -double(double(periodo_estudo.getSegundos()) / 3600.0));
				}

			} // try{
			catch (const std::exception& erro) { throw std::invalid_argument("Restricao CUSTO_PENALIDADE_0: \n" + std::string(erro.what())); }


			// ---------------------------------------------------------------------------------------------------------------------
			//                                  Restrição Custo Operacao (ZO0 = soma(ZO0p)) Por Periodo Otimizacao
			// ---------------------------------------------------------------------------------------------------------------------

			try {

				const int posEquZO0 = addEquLinear_CUSTO_OPERACAO_0(a_TSS, a_idEstagio, a_periodo_otimizacao);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZO0, 0.0);

				// Variável ZO0
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZO0(a_TSS, a_idEstagio, a_periodo_otimizacao), posEquZO0, 1.0);

				// Variável ZO0p
				for (Periodo periodo_estudo = a_horizonte_estudo_estagio.getIteradorInicial(); periodo_estudo <= a_horizonte_estudo_estagio.getIteradorFinal(); a_horizonte_estudo_estagio.incrementarIterador(periodo_estudo))
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZO0p(a_TSS, a_idEstagio, periodo_estudo), posEquZO0, -double(double(periodo_estudo.getSegundos()) / 3600.0));

			} // try{
			catch (const std::exception& erro) { throw std::invalid_argument("Restricao CUSTO_OPERACAO_0: \n" + std::string(erro.what())); }

		} // if (a_TSS != TipoSubproblemaSolver_mestre) {

		else if (a_TSS == TipoSubproblemaSolver_mestre) {

			// ---------------------------------------------------------------------------------------------------------------------
			//                                  Restrição Custo Total no problema Mestre (ZT = ZT_IdRealizacao_1 + .... + ZT_IdRealizacao_N)
			// ---------------------------------------------------------------------------------------------------------------------

			try {

				const int multiplicidade_corte = a_dados.getElementoVetor(AttVetorDados_cortes_multiplos, a_idEstagio, int());

				const int posEquZT = addEquLinear_CUSTO_TOTAL_MESTRE(a_TSS, a_idEstagio, a_periodo_otimizacao);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZT, 0.0);

				// Variável ZT
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZT(a_TSS, a_idEstagio, a_periodo_otimizacao), posEquZT, 1.0);

				for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getAtributo(a_idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); idRealizacao++) {

					if (multiplicidade_corte == 0){//Single cut{
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZT(a_TSS, a_idEstagio, a_periodo_otimizacao, idRealizacao), posEquZT, -1.0);
						break;
					}
					else if (multiplicidade_corte > 0) {//Multi cut

						const double probabilidade_abertura = getElementoMatriz(a_tipo_processo_estocastico_hidrologico, AttMatrizProcessoEstocastico_probabilidade_realizacao, a_periodo_otimizacao, idRealizacao, double());

						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZT(a_TSS, a_idEstagio, a_periodo_otimizacao, idRealizacao), posEquZT, -probabilidade_abertura);

					}//else if (multiplicidade_corte > 0) {

				}// for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getAtributo(idEstagio_seguinte, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); idRealizacao++) {

			} // try{
			catch (const std::exception& erro) { throw std::invalid_argument("Restricao Custo Total no problema Mestre: \n" + std::string(erro.what())); }

		} // else if (a_TSS == TipoSubproblemaSolver_mestre) {

	}// try {

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesCusto_Total_Imediato_Futuro(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesCusto_periodo(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		// ---------------------------------------------------------------------------------------------------------------------
		//                                  Restrição Custo Penalidades (ZPp = ZP_IdPatamarCarga1 + ... ZP_IdPatamarCargaN) Por Periodo Estudo
		// ---------------------------------------------------------------------------------------------------------------------

		const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, IdPatamarCarga());

		if (true) {

			const int posEquZPp = addEquLinear_CUSTO_PENALIDADE_p(a_TSS, a_idEstagio, a_periodo_estudo);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZPp, 0.0);

			// Variável ZP Por PeriodoEstudo
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZPp(a_TSS, a_idEstagio, a_periodo_estudo), posEquZPp, 1.0);

			// Variável ZP Por PeriodoEstudo Por PatamarCarga
			for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZP(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga), posEquZPp, -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, idPatamarCarga, double()));

			// Variável ZP Por PeriodoEstudo (para variáveis que não dependem do patamar de carga, ex, restrições de volume mínimo, evaporação)
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZP(a_TSS, a_idEstagio, a_periodo_estudo), posEquZPp, -1);

		}

		// ---------------------------------------------------------------------------------------------------------------------
		//                                  Restrição Custo Operacao (ZOp = ZO_IdPatamarCarga1 + ... ZO_IdPatamarCargaN) Por Periodo Estudo
		// ---------------------------------------------------------------------------------------------------------------------

		if (true) {

			const int posEquZOp = addEquLinear_CUSTO_OPERACAO_p(a_TSS, a_idEstagio, a_periodo_estudo);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZOp, 0.0);

			// Variável ZO Por PeriodoEstudo
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZOp(a_TSS, a_idEstagio, a_periodo_estudo), posEquZOp, 1.0);

			// Variável ZO Por PeriodoEstudo Por PatamarCarga
			for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZO(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga), posEquZOp, -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, idPatamarCarga, double()));

		}

	}//try {

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesCustoOperacao_periodo(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }

}


void ModeloOtimizacao::criarRestricoesCustoValorPresente_periodo(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		// ---------------------------------------------------------------------------------------------------------------------
		//                                  Restrição Custo Penalidade Valor Presente (ZP0p = desagio * ZPp) Por Periodo Estudo
		// ---------------------------------------------------------------------------------------------------------------------

		if (true) {

			const int posEquZP0p = addEquLinear_CUSTO_PENALIDADE_0p(a_TSS, a_idEstagio, a_periodo_estudo);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZP0p, 0.0);

			// Variável ZP0p
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZP0p(a_TSS, a_idEstagio, a_periodo_estudo), posEquZP0p, 1.0);

			// Variável ZPp
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZPp(a_TSS, a_idEstagio, a_periodo_estudo), posEquZP0p, -a_dados.getElementoMatriz(AttMatrizDados_desagio_acumulado_horizonte_estudo, a_idEstagio, a_periodo_estudo, double()));

		}

		// ---------------------------------------------------------------------------------------------------------------------
		//                                  Restrição Custo Operacao Valor Presente (ZO0p = desagio * ZOp) Por Periodo Estudo
		// ---------------------------------------------------------------------------------------------------------------------

		if (true) {

			const int posEquZO0p = addEquLinear_CUSTO_OPERACAO_0p(a_TSS, a_idEstagio, a_periodo_estudo);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZO0p, 0.0);

			// Variável ZO0p
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZO0p(a_TSS, a_idEstagio, a_periodo_estudo), posEquZO0p, 1.0);

			// Variável ZOp
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZOp(a_TSS, a_idEstagio, a_periodo_estudo), posEquZO0p, -a_dados.getElementoMatriz(AttMatrizDados_desagio_acumulado_horizonte_estudo, a_idEstagio, a_periodo_estudo, double()));

		}

	}//try {

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesCustoValorPresente_periodo(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }

}


void ModeloOtimizacao::criarRestricoesPotenciaHidraulicaDisponivel(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdPatamarCarga a_idPatamarCarga, const Periodo a_periodo_estudo, const IdHidreletrica a_idHidreletrica, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const TipoDetalhamentoProducaoHidreletrica tipo_detalhamento_producao = a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica());

		//
		// Por Hidreletrica
		//
		Periodo proximo_periodo_estudo = a_periodo_estudo;
		a_horizonte_estudo_estagio.incrementarIterador(proximo_periodo_estudo);

		// Restrição POTENCIA_HIDRAULICA
		if ((getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1) && (tipo_detalhamento_producao != TipoDetalhamentoProducaoHidreletrica_por_usina)) {

			const int posEquPH = addEquLinear_POTENCIA_HIDRAULICA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPH, 0.0);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquPH, 1.0);
			for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++)
				if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posEquPH, -1.0);

		} // if (getVarDecisao_PHseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1){


		// Restrição POTENCIA_HIDRAULICA_RELAXADA
		if (getVarDecisao_PH_RELseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1) {

			const int posEquPH_REL = addEquLinear_POTENCIA_HIDRAULICA_RELAXADA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPH_REL, 0.0);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquPH_REL, 1.0);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquPH_REL, -1.0);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquPH_REL, -1.0);

		} // if (getVarDecisao_PH_RELseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1) {

		// Restrição POTENCIA_HIDRAULICA_DISPONIVEL
		if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1) {

			const int posEquPHDISP = addEquLinear_POTENCIA_HIDRAULICA_DISPONIVEL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPHDISP, 0.0);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquPHDISP, 1.0);

			if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina) {

				//if (a_dados.getAtributo(AttComumDados_representar_producao_hidreletrica_com_turbinamento_disponivel, bool()))
					//vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquPHDISP, -1.0);
				//else	
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquPHDISP, -a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_disponibilidade, a_periodo_estudo, double()));

			}

			else {
				for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++)
					if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico) > -1)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posEquPHDISP, -1.0);
			}

		} // if (getVarDecisao_PHDISPseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1) {

		// Restrição POTENCIA_HIDRAULICA_DISPONIVEL_RELAXADA
		if (getVarDecisao_PHDISP_RELseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1) {

			const int posEquPHDISP_REL = addEquLinear_POTENCIA_HIDRAULICA_DISPONIVEL_RELAXADA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPHDISP_REL, 0.0);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquPHDISP_REL, 1.0);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquPHDISP_REL, -1.0);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquPHDISP_REL, -1.0);

		} // if (getVarDecisao_PHDISP_RELseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1) {

		//
		// Por Conjunto Hidraulico
		//

		for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

			// Restrição POTENCIA_HIDRAULICA
			if ((getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico) > -1) && (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {

				const int posEquPH = addEquLinear_POTENCIA_HIDRAULICA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPH, 0.0);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posEquPH, 1.0);

				for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++)
					if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquPH, -1.0);

			} // if (getVarDecisao_PHseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico) > -1){

			// Restrição POTENCIA_HIDRAULICA_RELAXADA
			if (getVarDecisao_PH_RELseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico) > -1) {

				const int posEquPH_REL = addEquLinear_POTENCIA_HIDRAULICA_RELAXADA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPH_REL, 0.0);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posEquPH_REL, 1.0);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posEquPH_REL, -1.0);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posEquPH_REL, -1.0);

			} // if (getVarDecisao_PH_RELseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico) > -1) {

			// Restrição POTENCIA_HIDRAULICA_DISPONIVEL
			if ((getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico) > -1) && (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto)) {

				const int posEquPHDISP = addEquLinear_POTENCIA_HIDRAULICA_DISPONIVEL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPHDISP, 0.0);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posEquPHDISP, 1.0);

				if (a_dados.getAtributo(AttComumDados_representar_producao_hidreletrica_com_turbinamento_disponivel, bool()))
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posEquPHDISP, -1.0);
				else
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posEquPHDISP, -a_dados.getElementoVetor(a_idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_disponibilidade, a_periodo_estudo, double()));

			} // if (getVarDecisao_PHDISPseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico) > -1) {

			else if ((getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico) > -1) && (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {

				const int posEquPHDISP = addEquLinear_POTENCIA_HIDRAULICA_DISPONIVEL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPHDISP, 0.0);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posEquPHDISP, 1.0);

				for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++)
					if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquPHDISP, -1.0);

			} // if (getVarDecisao_PHDISPseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico) > -1) {

			// Restrição POTENCIA_HIDRAULICA_DISPONIVEL_RELAXADA
			if (getVarDecisao_PHDISP_RELseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico) > -1) {

				const int posEquPHDISP_REL = addEquLinear_POTENCIA_HIDRAULICA_DISPONIVEL_RELAXADA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPHDISP_REL, 0.0);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posEquPHDISP_REL, 1.0);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posEquPHDISP_REL, -1.0);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posEquPHDISP_REL, -1.0);

			} // if (getVarDecisao_PHDISP_RELseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico) > -1) {

		} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

		//
		// Producao Por Unidade UHE
		//

		for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {
			for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {

				// Restrição POTENCIA_HIDRAULICA_RELAXADA
				if (getVarDecisao_PH_RELseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

					const int posEquPH_REL = addEquLinear_POTENCIA_HIDRAULICA_RELAXADA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE);

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPH_REL, 0.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquPH_REL, 1.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquPH_REL, -1.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquPH_REL, -1.0);

				} // if (getVarDecisao_PH_RELseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

				// Restrição POTENCIA_HIDRAULICA_DISPONIVEL
				if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

					const int posEquPHDISP = addEquLinear_POTENCIA_HIDRAULICA_DISPONIVEL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE);

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPHDISP, 0.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquPHDISP, 1.0);

					if (a_dados.getAtributo(AttComumDados_representar_producao_hidreletrica_com_turbinamento_disponivel, bool()))
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquPHDISP, -1.0);
					else
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquPHDISP, -a_dados.getElementoVetor(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_disponibilidade, a_periodo_estudo, double()));

				} // if (getVarDecisao_PHDISPseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

				// Restrição POTENCIA_HIDRAULICA_DISPONIVEL_RELAXADA
				if (getVarDecisao_PHDISP_RELseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

					const int posEquPHDISP_REL = addEquLinear_POTENCIA_HIDRAULICA_DISPONIVEL_RELAXADA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE);

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPHDISP_REL, 0.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquPHDISP_REL, 1.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquPHDISP_REL, -1.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquPHDISP_REL, -1.0);

				} // if (getVarDecisao_PHDISP_RELseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

			} // for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {
		} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {


	}//try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesProducaoHidreletrica_porProdutibilidade(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }

}


void ModeloOtimizacao::criarRestricoesProducaoTermeletrica_porProdutibilidade(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdPatamarCarga a_idPatamarCarga, const Periodo a_periodo_estudo, const IdTermeletrica a_maiorIdTermeletrica, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio) {
	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());
		const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
		const SmartEnupla<Periodo, double> horizonte_inicio_estudo = a_dados.getElementosMatriz(AttMatrizDados_percentual_duracao_horizonte_estudo, estagio_inicial, Periodo(), double());
		const SmartEnupla<Periodo, double> horizonte_inicio_estagio = a_dados.getElementosMatriz(AttMatrizDados_percentual_duracao_horizonte_estudo, estagio_inicial, Periodo(), double());

		for (IdTermeletrica idTermeletrica = a_dados.getMenorId(IdTermeletrica()); idTermeletrica <= a_maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idTermeletrica)) {

			const TipoDetalhamentoProducaoTermeletrica tipo_detalhamento_producao = a_dados.getElementoVetor(idTermeletrica, AttVetorTermeletrica_tipo_detalhamento_producao, a_periodo_estudo, TipoDetalhamentoProducaoTermeletrica());


			//
			// Por Termeletrica
			//

			// RESTRIÇÃO POTENCIA_TERMICA
			if (getVarDecisao_PT_UTILseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) > -1) {

				//EQUAÇÃO  POTÊNCIA GERADA (PT - PT_UTIL- POTENCIA_MINIMA*LIG_T == 0) 
				const int posEquPT_UTIL = addEquLinear_POTENCIA_TERMICA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPT_UTIL, 0.0);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), posEquPT_UTIL, 1.0);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_UTIL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), posEquPT_UTIL, -1.0);

				if (getVarDecisao_LIG_TseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_LIG_T(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), posEquPT_UTIL, -a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()));


				if (a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0) {
					const int posEquPT_REL = addEquLinear_POTENCIA_TERMICA_RELAXADA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), posEquPT_REL, 1.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), posEquPT_REL, -1.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), posEquPT_REL, -1.0);
				}


				else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {

					if (getVarDecisao_PTDISP_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) > -1) {
						const int posEquPTDISP_FINF = addIneLinear_POTENCIA_TERMICA_DISPONIVEL_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPTDISP_FINF, a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()));
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), posEquPTDISP_FINF, 1.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), posEquPTDISP_FINF, 1.0);
					}

					if (getVarDecisao_PTDISP_FSUPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) > -1) {
						const int posEquPTDISP_FSUP = addIneLinear_POTENCIA_TERMICA_DISPONIVEL_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPTDISP_FSUP, -a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_maxima, a_periodo_estudo, a_idPatamarCarga, double()));
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), posEquPTDISP_FSUP, 1.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), posEquPTDISP_FSUP, -1.0);
					}
				}

			} // if (getVarDecisao_PT_RELseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) > -1) {



			// Restrição POTENCIA_TERMICA_DISPONIVEL
			if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) > -1) {

				if (true) {

					const int posEquPTDISP = addEquLinear_POTENCIA_TERMICA_DISPONIVEL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica);

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPTDISP, 0.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), posEquPTDISP, 1.0);

					if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), posEquPTDISP, -a_dados.getElementoVetor(idTermeletrica, AttVetorTermeletrica_disponibilidade, a_periodo_estudo, double()));

					else {
						for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= a_dados.getMaiorId(idTermeletrica, IdUnidadeUTE()); idUnidadeUTE++)
							if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE) > -1)
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posEquPTDISP, -1.0);
					}
				}

			} // if (getVarDecisao_PTDISPseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) > -1) {



			// Restrição Comando GNL

			if (getVarDecisao_PTDISPCOMseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) > -1) {

				const int equPTDISPCOM_pat = addEquLinear_POTENCIA_TERMICA_DISPONIVEL_COMANDADA_RELAXADA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISPCOM(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), equPTDISPCOM_pat, 1.0);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISPCOM_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), equPTDISPCOM_pat, 1.0);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISPCOM_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), equPTDISPCOM_pat, -1.0);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), equPTDISPCOM_pat, -1.0);

			} // if (getVarDecisao_PTDISPCOMseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) > -1) {


			for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= a_dados.getMaiorId(idTermeletrica, IdUnidadeUTE()); idUnidadeUTE++) {

				// Restrição POTENCIA_TERMICA
				if (getVarDecisao_PTseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE) > -1) {


					//EQUAÇÃO  POTÊNCIA GERADA (PT - PT_UTIL- POTENCIA_UTIL*LIG_T == 0)
					const int posEquPT_UTIL = addEquLinear_POTENCIA_TERMICA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPT_UTIL, 0.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posEquPT_UTIL, 1.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_UTIL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posEquPT_UTIL, -1.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_LIG_T(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posEquPT_UTIL, -a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()));


					// REPRESENTAÇÃO DISCRETA (COM VARIAVEL BINÁRIA)
					if (a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_representacao_discreta_producao, bool())) {


						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_TRAJ_ACIO(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posEquPT_UTIL, -1.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_TRAJ_DESL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posEquPT_UTIL, -1.0);


						Periodo periodo_estudo_anterior = a_periodo_estudo;
						a_horizonte_estudo_estagio.decrementarIterador(periodo_estudo_anterior);

						int tOn = a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_t_on, int()) - int((a_dados.getSizeVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_rampa_acionamento) + a_dados.getSizeVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_rampa_desligamento)) / 2);
						int tOf = a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_t_of, int()) + int((a_dados.getSizeVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_rampa_acionamento) + a_dados.getSizeVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_rampa_desligamento)) / 2);

						// SET VARIAVEIS COMO BINÁRIA
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_LIG_T(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setTipoVariavelBinaria(getVarDecisao_LIG_T(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE));
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setTipoVariavelBinaria(getVarDecisao_DESL_T(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE));
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setTipoVariavelBinaria(getVarDecisao_ACIO_T(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE));


						// RELAÇÃO ENTRE AS VARIAVEIS BINÁRIAS 
						const int posEquVAR_BINARIAS = addEquLinear_RELACAO_VARIAVEIS_BINARIAS_TERMICA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquVAR_BINARIAS, 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ACIO_T(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posEquVAR_BINARIAS, 1.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_DESL_T(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posEquVAR_BINARIAS, -1.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_LIG_T(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posEquVAR_BINARIAS, -1.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_LIG_T(a_TSS, a_idEstagio, periodo_estudo_anterior, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posEquVAR_BINARIAS, 1.0);


						// CONDIÇÕES INICIAIS
						//*************************************************************************************************************************************************************
						if (a_periodo_estudo == (a_horizonte_estudo_estagio.getIteradorInicial())) {

							// SET VARIAVEIS BINÁRIAS
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setTipoVariavelBinaria(getVarDecisao_LIG_T(a_TSS, a_idEstagio, periodo_estudo_anterior, a_idPatamarCarga, idTermeletrica, idUnidadeUTE));
							//vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setTipoVariavelBinaria(getVarDecisao_ACIO_T(a_idEstagio, periodo_estudo_anterior, a_idPatamarCarga, idTermeletrica, idUnidadeUTE));

							// ESTADO DA UNIDADE NO PERIODO ANTERIOR AO ESTUDO (p0 -1)
							if (a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_status, TipoStatusUnidadeUTE()) == TipoStatusUnidadeUTE_ligada) {
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_LIG_T(a_TSS, a_idEstagio, periodo_estudo_anterior, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), 1.0);
								//vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PT_UTIL(a_idEstagio, periodo_estudo_anterior, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_geracao_inicial, double()) - a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()));
								//vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PT_UTIL(a_idEstagio, periodo_estudo_anterior, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_geracao_inicial, double()) - a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()));
							}

							else if (a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_status, TipoStatusUnidadeUTE()) == TipoStatusUnidadeUTE_desligada) {
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_LIG_T(a_TSS, a_idEstagio, periodo_estudo_anterior, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), 0.0);
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_LIG_T(a_TSS, a_idEstagio, periodo_estudo_anterior, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), 0.0);
								//vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PT_UTIL(a_idEstagio, periodo_estudo_anterior, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), 0.0);
								//vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PT_UTIL(a_idEstagio, periodo_estudo_anterior, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), 0.0);
							}

							//	// VERIFICA SE A UNIDADE FOI ACIONADA NO PERIODO ANTERIOR AO ESTUDO (p0 - 1)
							//	if (((a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_horas_neste_status, int()) == 1) || ((a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_horas_neste_status, int()) == 1) && (a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_min_neste_status, int()) == 30))) && (a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_status, TipoStatusUnidadeUTE()) == TipoStatusUnidadeUTE_ligada))
							//		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_ACIO_T(a_idEstagio, periodo_estudo_anterior, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), 1.0);

							//	else
							//		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_ACIO_T(a_idEstagio, periodo_estudo_anterior, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), 0.0);

						}// if (a_periodo_estudo == (a_horizonte_estudo_estagio.getIteradorInicial())) {


						//INEQUAÇÃO QUE ASSEGURA GERAÇÃO MINIMA QUANDO A UNIDADE É LIGADA  (PT_UTIL - POTENCIA_UTIL*LIG_T + POTENCIA_UTIL*ACIO_T <= 0) 
						//*************************************************************************************************************************************************************
						const int posInePT_PARTIDA = addIneLinear_POTENCIA_TERMELETRICA_PARTIDA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posInePT_PARTIDA, 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_UTIL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_PARTIDA, -1.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_LIG_T(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_PARTIDA, +a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_util, a_periodo_estudo, a_idPatamarCarga, double()));

						//
						////INEQUAÇÃO QUE ASSEGURA GERAÇÃO MINIMA QUANDO A UNIDADE É LIGADA  (PT_UTIL - POTENCIA_UTIL*LIG_T + POTENCIA_UTIL*ACIO_T <= 0) 
						////*************************************************************************************************************************************************************
						//const int posInePT_PARTIDA = addIneLinear_POTENCIA_TERMELETRICA_PARTIDA(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE);
						//vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posInePT_PARTIDA, 0.0);
						//vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_UTIL(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_PARTIDA, - 1.0);
						//vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_LIG_T(  a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_PARTIDA, + a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_util, a_periodo_estudo, a_idPatamarCarga, double()));
						//vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ACIO_T( a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_PARTIDA, - a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_util, a_periodo_estudo, a_idPatamarCarga, double()));


						////INEQUAÇÃO QUE ASSEGURA GERAÇÃO MINIMA QUANDO A UNIDADE É DESLIGADA  
						////*************************************************************************************************************************************************************
						//if (tOn <= 1) {	// PARA UNIDADES COM TLIG <= 1 (PT_UTIL(p-1) - POTENCIA_UTIL*LIG_T(p-1) + POTENCIA_UTIL*DESL_T <= 0)				 				

						//	const int posInePT_DESLIGAMENTO = addIneLinear_POTENCIA_TERMELETRICA_DESLIGAMENTO(a_idEstagio, a_periodo_estudo , a_idPatamarCarga, idTermeletrica, idUnidadeUTE);
						//	vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posInePT_DESLIGAMENTO, 0.0);
						//	vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_UTIL(a_idEstagio, periodo_estudo_anterior, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_DESLIGAMENTO, - 1.0);
						//	vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_LIG_T(  a_idEstagio, periodo_estudo_anterior, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_DESLIGAMENTO,  a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_util, a_periodo_estudo, a_idPatamarCarga, double()));
						//	vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_DESL_T( a_idEstagio,        a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_DESLIGAMENTO, - a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_util, a_periodo_estudo, a_idPatamarCarga, double()));
						//												
						//} //if (tOn <= 1) {					
						//
						//else if (tOn > 1) {	 // PARA UNIDADES COM TLIG > 1 (PT_UTIL(p-1) - POTENCIA_UTIL*LIG_T(p-1)  + POTENCIA_UTIL*ACIO_T(p-1) + POTENCIA_UTIL*DESL_T <= 0)
						//	
						//		const int posInePT_DESLIGAMENTO = addIneLinear_POTENCIA_TERMELETRICA_DESLIGAMENTO(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE);
						//		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posInePT_DESLIGAMENTO, 0.0);
						//		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_UTIL(a_idEstagio, periodo_estudo_anterior, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_DESLIGAMENTO, - 1.0);
						//		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_LIG_T(  a_idEstagio, periodo_estudo_anterior, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_DESLIGAMENTO, a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_util, a_periodo_estudo, a_idPatamarCarga, double()));
						//		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ACIO_T( a_idEstagio, periodo_estudo_anterior, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_DESLIGAMENTO, - a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_util, a_periodo_estudo, a_idPatamarCarga, double()));
						//		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_DESL_T( a_idEstagio,        a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_DESLIGAMENTO, - a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_util, a_periodo_estudo, a_idPatamarCarga, double()));

						//} //else if (tOn > 1) {


						// RESTRIÇÃO DE MINIMUN UP/DOWNTIME
						//****************************************************************************************************************************************************************
						if (a_periodo_estudo >= a_horizonte_estudo_estagio.getIteradorInicial() + IdHor(tOn + 1)) {

							// UPTIME
							const int posIneUP_TIME = addIneLinear_TEMPO_MINIMO_LIGADA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE);
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneUP_TIME, 0.0);
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_LIG_T(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posIneUP_TIME, 1.0);

							// DOWNTIME
							const int posIneDOWN_TIME = addIneLinear_TEMPO_MINIMO_DESLIGADA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE);
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneDOWN_TIME, -1);
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_LIG_T(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posIneDOWN_TIME, -1.0);


							Periodo periodo_inicial = a_horizonte_estudo_estagio.getIteradorInicial();

							for (Periodo periodo_aux = a_horizonte_estudo_estagio.getIteradorInicial(); periodo_aux <= a_horizonte_estudo_estagio.getIteradorFinal(); a_horizonte_estudo_estagio.incrementarIterador(periodo_aux))
								if (periodo_aux <= (a_periodo_estudo - IdHor(tOn + 1)))
									periodo_inicial = periodo_aux;

							for (Periodo periodo = periodo_inicial; periodo <= a_periodo_estudo; a_horizonte_estudo_estagio.incrementarIterador(periodo)) {
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ACIO_T(a_TSS, a_idEstagio, periodo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posIneUP_TIME, -1.0);
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_DESL_T(a_TSS, a_idEstagio, periodo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posIneDOWN_TIME, -1.0);
							}

						}//if (a_periodo_estudo >= a_periodo_estudo + IdHor(tOn+1)) {


						else if (a_periodo_estudo < a_horizonte_estudo_estagio.getIteradorInicial() + IdHor(tOn + 1)) {

							// UPTIME
							const int posIneUP_TIME = addIneLinear_TEMPO_MINIMO_LIGADA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE);
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneUP_TIME, 0.0);
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_LIG_T(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posIneUP_TIME, 1.0);


							// DOWNTIME
							const int posIneDOWN_TIME = addIneLinear_TEMPO_MINIMO_DESLIGADA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE);
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneDOWN_TIME, -1);
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_LIG_T(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posIneDOWN_TIME, -1.0);

							for (Periodo periodo = a_horizonte_estudo_estagio.getIteradorInicial(); periodo <= a_periodo_estudo; a_horizonte_estudo_estagio.incrementarIterador(periodo)) {
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ACIO_T(a_TSS, a_idEstagio, periodo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posIneUP_TIME, -1.0);
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_DESL_T(a_TSS, a_idEstagio, periodo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posIneDOWN_TIME, -1.0);
							}

						}// else if (a_periodo_estudo >= a_periodo_estudo + IdHor(tOn+1)) {



							// CONDIÇÕES INICIAIS (SET NOS BOUNDS DAS VARIAVEIS BINÁRIAS)
							//*********************************************************************************************************************************************************
						if (a_periodo_estudo == a_horizonte_estudo_estagio.getIteradorFinal()) {

							// PARA UNIDADES LIGADAS
							if (a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_status, TipoStatusUnidadeUTE()) == TipoStatusUnidadeUTE_ligada) {

								if (a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_horas_neste_status, int()) <= tOn) {

									Periodo periodo_final = a_horizonte_estudo_estagio.getIteradorInicial() + IdHor(tOn) - IdHor(a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_horas_neste_status, int())) - IdMin(a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_min_neste_status, int()));

									if (periodo_final <= a_horizonte_estudo_estagio.getIteradorFinal())
										for (Periodo periodo = a_horizonte_estudo_estagio.getIteradorInicial(); periodo <= periodo_final; a_horizonte_estudo_estagio.incrementarIterador(periodo))
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_LIG_T(a_TSS, a_idEstagio, periodo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), 1.0);

									else if (periodo_final > a_horizonte_estudo_estagio.getIteradorFinal())
										for (Periodo periodo = a_horizonte_estudo_estagio.getIteradorInicial(); periodo <= a_horizonte_estudo_estagio.getIteradorFinal(); a_horizonte_estudo_estagio.incrementarIterador(periodo))
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_LIG_T(a_TSS, a_idEstagio, periodo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), 1.0);

								}//if (a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_horas_neste_status, int()) <= tOn) {
							}//if (a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_status, TipoStatusUnidadeUTE()) == TipoStatusUnidadeUTE_ligada) {


							// PARA UNIDADES DESLIGADAS
							else if (a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_status, TipoStatusUnidadeUTE()) == TipoStatusUnidadeUTE_desligada) {

								if (a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_horas_neste_status, int()) <= tOf) {

									Periodo periodo_final = a_horizonte_estudo_estagio.getIteradorInicial() + IdHor(tOf) - IdHor(a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_horas_neste_status, int())) - IdMin(a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_min_neste_status, int()));

									if (periodo_final <= a_horizonte_estudo_estagio.getIteradorFinal())
										for (Periodo periodo = a_horizonte_estudo_estagio.getIteradorInicial(); periodo <= periodo_final; a_horizonte_estudo_estagio.incrementarIterador(periodo))
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_DESL_T(a_TSS, a_idEstagio, periodo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), 0.0);

									else if (periodo_final > a_horizonte_estudo_estagio.getIteradorFinal())
										for (Periodo periodo = a_horizonte_estudo_estagio.getIteradorInicial(); periodo <= a_horizonte_estudo_estagio.getIteradorFinal(); a_horizonte_estudo_estagio.incrementarIterador(periodo))
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_DESL_T(a_TSS, a_idEstagio, periodo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), 0.0);


								}//if (a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_horas_neste_status, int()) <= tOf) {								

							}//else if (a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_status, TipoStatusUnidadeUTE()) == TipoStatusUnidadeUTE_desligada) {

						}// if (a_periodo_estudo == a_horizonte_estudo_estagio.getIteradorFicial()) {									   						 


						// RESTRIÇÃO PARA TRAJETORIA DE ACIONAMENTO
						//*************************************************************************************************************************************************************

						int numero_trajetorias = (a_dados.getSizeVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_rampa_acionamento));

						if (numero_trajetorias == 0)
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PT_TRAJ_ACIO(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), 0.0);

						else {

							Periodo periodo = a_periodo_estudo;
							Periodo periodoTipoMeiaHora = Periodo(TipoPeriodo_meia_hora, a_periodo_estudo.getDia(), a_periodo_estudo.getMes(), a_periodo_estudo.getAno(), a_periodo_estudo.getHora(), a_periodo_estudo.getMinuto());
							Periodo periodoAcio_aux = periodoTipoMeiaHora + numero_trajetorias;
							Periodo periodoAcio;

							for (Periodo periodoAux = a_horizonte_estudo_estagio.getIteradorInicial(); periodoAux <= a_horizonte_estudo_estagio.getIteradorFinal(); a_horizonte_estudo_estagio.incrementarIterador(periodoAux)) {
								if (periodoAux > periodoAcio_aux) { break; }
								if (periodoAux <= periodoAcio_aux) { periodoAcio = periodoAux; }
								if ((periodoAux < periodoAcio_aux) && (a_periodo_estudo == a_horizonte_estudo_estagio.getIteradorInicial())) {
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_ACIO_T(a_TSS, a_idEstagio, periodoAux, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), 0.0);
								}
							}

							//RESTRIÇÃO QUE OBRIGA PT_TRAJ_DESL SER IGUAL A 0 QUANDO NÃO ESTA EM TRAJETÓRIA DE ACIONAMENTO
							// (- PT_TRAJ_ACIO - ACIO_T(p-nTrajetorias; p) * PtMax >=0 )
							const int posInePT_ACIO_SOMA = addIneLinear_PT_TRAJETORIA_ACIONAMENTO(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE, a_periodo_estudo);
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posInePT_ACIO_SOMA, 0.0);
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_TRAJ_ACIO(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_ACIO_SOMA, -1);

							for (int trajetoria = 0; trajetoria < numero_trajetorias; trajetoria++) {

								Periodo periodoTrajetoria = periodoTipoMeiaHora + trajetoria;

								if (periodoTrajetoria >= periodo) {

									if (periodo <= a_horizonte_estudo_estagio.getIteradorFinal()) {

										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ACIO_T(a_TSS, a_idEstagio, periodo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_ACIO_SOMA, a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima, periodo, a_idPatamarCarga, double()));

										// LIMITE INFERIOR (PT_TRAJ_LIG - TRAJ_LIG_T * rampa >=0 )
										const int posInePT_ACIO = addIneLinear_PT_TRAJETORIA_ACIONAMENTO_INF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE, periodo);
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posInePT_ACIO, 0.0);
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_TRAJ_ACIO(a_TSS, a_idEstagio, periodo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_ACIO, 1);
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ACIO_T(a_TSS, a_idEstagio, periodoAcio, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_ACIO, -a_dados.getElementoVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_rampa_acionamento, trajetoria, double()));

										// LIMITE SUPERIOR (-PT_TRAJ_LIG + TRAJ_LIG_T * rampa + TRAJ_LIG_T * PT_MIN >= PT_MIN )
										const int posInePT_ACIO_SUP = addIneLinear_PT_TRAJETORIA_ACIONAMENTO_SUP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE, periodo);
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posInePT_ACIO_SUP, -a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima, periodo, a_idPatamarCarga, double()));
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_TRAJ_ACIO(a_TSS, a_idEstagio, periodo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_ACIO_SUP, -1);
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ACIO_T(a_TSS, a_idEstagio, periodoAcio, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_ACIO_SUP, -(a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima, periodo, a_idPatamarCarga, double()) - a_dados.getElementoVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_rampa_acionamento, trajetoria, double())));
										a_horizonte_estudo_estagio.incrementarIterador(periodo);

									}//if (periodo <= a_horizonte_estudo_estagio.getIteradorFinal()) {
								}
							}//for (int trajetoria = 0; trajetoria < numero_trajetorias; trajetoria++) {

							if (periodo <= a_horizonte_estudo_estagio.getIteradorFinal())
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ACIO_T(a_TSS, a_idEstagio, periodo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_ACIO_SOMA, a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima, periodo, a_idPatamarCarga, double()));

						}// else


						// RESTRIÇÃO PARA TRAJETORIA DE DESLIGAMENTO
						//*************************************************************************************************************************************************************
						numero_trajetorias = (a_dados.getSizeVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_rampa_desligamento));
						if (numero_trajetorias == 0)
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PT_TRAJ_DESL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), 0.0);

						else {

							//RESTRIÇÃO QUE OBRIGA PT_TRAJ_DESL SER IGUAL A 0 QUANDO NÃO ESTA EM TRAJETÓRIA DE DESLIGAMANTO
							// (- PT_TRAJ_DESL - DESL_T(p-nTrajetorias; p) * PtMax >=0 )
							const int posInePT_DESL_SOMA = addIneLinear_PT_TRAJETORIA_DESLIGAMENTO(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE, a_periodo_estudo, 1);
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posInePT_DESL_SOMA, 0.0);
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_TRAJ_DESL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_DESL_SOMA, -1);

							Periodo periodo = a_periodo_estudo;
							Periodo periodoTipoMeiaHora = Periodo(TipoPeriodo_meia_hora, a_periodo_estudo.getDia(), a_periodo_estudo.getMes(), a_periodo_estudo.getAno(), a_periodo_estudo.getHora(), a_periodo_estudo.getMinuto());

							for (int trajetoria = 0; trajetoria < numero_trajetorias; trajetoria++) {

								Periodo periodoTrajetoria = periodoTipoMeiaHora + trajetoria;
								Periodo periodoPT = periodoTipoMeiaHora + (trajetoria - numero_trajetorias + 1);
								Periodo periodoPt = periodoPT;

								for (Periodo periodoAux = a_horizonte_estudo_estagio.getIteradorInicial(); periodoAux <= a_horizonte_estudo_estagio.getIteradorFinal(); a_horizonte_estudo_estagio.incrementarIterador(periodoAux)) {
									if (periodoAux > periodoPT) { break; }
									if (periodoAux <= periodoPT) { periodoPt = periodoAux; }
								}


								//RESTRIÇÃO QUE OBRIGA PT_TRAJ_DESL SER IGUAL A 0 QUANDO NÃO ESTA EM TRAJETÓRIA DE DESLIGAMANTO
								if (periodoPt >= a_horizonte_estudo_estagio.getIteradorInicial())
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_DESL_T(a_TSS, a_idEstagio, periodoPt, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_DESL_SOMA, a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima, periodoPt, a_idPatamarCarga, double()));

								if (periodoTrajetoria >= periodo) {

									if (periodo <= a_horizonte_estudo_estagio.getIteradorFinal()) {

										// LIMITE INFERIOR (PT_TRAJ_DESL(1;nt) - DESL_T * rampa >=0 )
										const int posInePT_DESL_INF = addIneLinear_PT_TRAJETORIA_DESLIGAMENTO_INF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE, periodo, trajetoria + 1);
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posInePT_DESL_INF, 0.0);
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_TRAJ_DESL(a_TSS, a_idEstagio, periodo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_DESL_INF, 1);
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_DESL_T(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_DESL_INF, -a_dados.getElementoVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_rampa_desligamento, trajetoria, double()));

										// LIMITE SUPERIOR (-PT_TRAJ_DESL + DESL_T * (rampa -  PT_MAX) - PT_MAX >= 0 )
										const int posInePT_DESL_SUP = addIneLinear_PT_TRAJETORIA_DESLIGAMENTO_SUP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE, periodo, trajetoria + 1);
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posInePT_DESL_SUP, -a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima, periodo, a_idPatamarCarga, double()));
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_TRAJ_DESL(a_TSS, a_idEstagio, periodo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_DESL_SUP, -1);
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_DESL_T(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posInePT_DESL_SUP, (a_dados.getElementoVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_rampa_desligamento, trajetoria, double()) - a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima, periodo, a_idPatamarCarga, double())));

										a_horizonte_estudo_estagio.incrementarIterador(periodo);
									}
								}

							}//for (int trajetoria = 0; trajetoria < numero_trajetorias; trajetoria++) {

						} //else {






						//// CONDIÇÕES INICIAIS TRAJETÓRIA DE ACIONAMENTO 
						//if (a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_status, TipoStatusUnidadeUTE()) == TipoStatusUnidadeUTE_desligada) {

						//	int numero_trajetorias = (a_dados.getSizeVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_rampa_acionamento));

						//	if (a_periodo_estudo < a_horizonte_estudo_estagio.getIteradorInicial() + numero_trajetorias)
						//		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_ACIO_T(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), 0.0);
						//}


						//// CONDIÇÕES INICIAIS TRAJETÓRIA DE DESLIGAMENTO 
						//if (a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_status, TipoStatusUnidadeUTE()) == TipoStatusUnidadeUTE_ligada) {

						//	int numero_trajetorias = (a_dados.getSizeVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_rampa_desligamento));

						//	if (a_periodo_estudo < a_horizonte_estudo_estagio.getIteradorInicial() + numero_trajetorias)
						//		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_DESL_T(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), 0.0);
						//}



					}// if (a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_representacao_discreta_producao, bool())) {

				} // if (getVarDecisao_PTseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE) > -1) {


				//			   				 
				// Restrição POTENCIA_TERMICA_DISPONIVEL
				if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE) > -1) {

					const int posEquPTDISP = addEquLinear_POTENCIA_TERMICA_DISPONIVEL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE);

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPTDISP, 0.0);

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posEquPTDISP, 1.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posEquPTDISP, -a_dados.getElementoVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_disponibilidade, a_periodo_estudo, double()));

				} // if (getVarDecisao_PTDISPseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE) > -1) {

				//// Restrição POTENCIA_TERMICA_DISPONIVEL_RELAXADA
				//if (getVarDecisao_PTDISP_RELseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE) > -1) {

				//	const int posEquPTDISP_REL = addEquLinear_POTENCIA_TERMICA_DISPONIVEL_RELAXADA(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE);

				//	vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPTDISP_REL, 0.0);

				//	vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP_REL(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posEquPTDISP_REL, 1.0);
				//	vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posEquPTDISP_REL, -1.0);
				//	vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP_FINF(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posEquPTDISP_REL, -1.0);

				//} // if (getVarDecisao_PTDISP_RELseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE) > -1) {

			} // for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= a_dados.getMaiorId(idTermeletrica, IdUnidadeUTE()); idUnidadeUTE++) {



			// RESTRIÇÃO PARA UTEs A CICLO COMBINADO QUE SÃO MODELADAS COMO UNIDADES EQUIVALENTES, ONDE, APENAS UMA UNIDADE PODE ESTAR EM OPERAÇÃO.
			int posIneUN_EQUIVALENTE = -1;

			for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= a_dados.getMaiorId(idTermeletrica, IdUnidadeUTE()); idUnidadeUTE++) {

				if (a_dados.getAtributo(idTermeletrica, IdUnidadeUTE_1, AttComumUnidadeUTE_unidade_equivalente, bool())) {

					if (idUnidadeUTE == IdUnidadeUTE_1) {
						posIneUN_EQUIVALENTE = addIneLinear_RELACAO_UNIDADES_EQUIVALENTE_TERMICA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneUN_EQUIVALENTE, -1);
					}

					if (getVarDecisao_LIG_TseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE) > -1)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_LIG_T(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posIneUN_EQUIVALENTE, -1);
				}


			}//if (a_dados.getAtributo(idTermeletrica, IdUnidadeUTE_1, AttComumUnidadeUTE_unidade_equivalente, bool())) {


		} // for (IdTermeletrica idTermeletrica = a_dados.getMenorId(IdTermeletrica()); idTermeletrica <= a_maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idTermeletrica)) {

	}//try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesProducaoTermeletrica_porProdutibilidade(" + getFullString(a_idEstagio) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }

}


void ModeloOtimizacao::criarVariaveisAssociadasHorizonteOtimizacao(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdEstagio a_estagio_final, const IdEstagio a_estagio_inicial, const Periodo a_periodo_otimizacao) {

	try {

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const double infinito = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getInfinito();

		if (a_TSS != TipoSubproblemaSolver_mestre) {

			//
			// Custo Total (ZT)
			//

			addVarDecisao_ZT(a_TSS, a_idEstagio, a_periodo_otimizacao, 0.0, infinito, 1.0);

			//
			// Custo Imediato (ZI)
			//

			addVarDecisao_ZI(a_TSS, a_idEstagio, a_periodo_otimizacao, 0.0, infinito, 0.0);

			//
			// Custo Penalidade Valor Presente (ZP0)
			//

			addVarDecisao_ZP0(a_TSS, a_idEstagio, a_periodo_otimizacao, 0.0, infinito, 0.0);

			//
			// Custo Operacao  Valor Presente (ZO0)
			//

			addVarDecisao_ZO0(a_TSS, a_idEstagio, a_periodo_otimizacao, 0.0, infinito, 0.0);

			//
			// Custo Futuro (ZF)
			//

			if (a_idEstagio < getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio())) {

				const IdEstagio idEstagio_seguinte = IdEstagio(a_idEstagio + 1);

				const int multiplicidade_corte = a_dados.getElementoVetor(AttVetorDados_cortes_multiplos, idEstagio_seguinte, int());

				addVarDecisao_ZF(a_TSS, a_idEstagio, a_periodo_otimizacao, 0.0, infinito, 0.0);

				for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getAtributo(idEstagio_seguinte, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); idRealizacao++) {
					addVarDecisao_ZF(a_TSS, a_idEstagio, a_periodo_otimizacao, idRealizacao, 0.0, infinito, 0.0);
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

			addVarDecisao_ZT(a_TSS, a_idEstagio, a_periodo_otimizacao, 0.0, infinito, 0.0);

			for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getAtributo(a_idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); idRealizacao++) {
				addVarDecisao_ZT(a_TSS, a_idEstagio, a_periodo_otimizacao, idRealizacao, 0.0, infinito, 0.0);
				if (multiplicidade_corte == 0)
					break;
			} // for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getAtributo(a_idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); idRealizacao++) {

		} // else if (a_TSS == TipoSubproblemaSolver_mestre) {

	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisAssociadasHorizonteOtimizacao(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesBalancoHidraulicoUsinaByVazao(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdEstagio a_idEstagio_acoplamento, const Periodo a_periodo_acoplamento, const Periodo a_periodo_estudo, const Periodo a_periodo_otimizacao_final, const Periodo a_periodo_estudo_inicial, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga, const SmartEnupla<Periodo, int>& a_horizonte_processo_estocastico, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		Periodo proximo_periodo_estudo = a_periodo_estudo;
		Periodo periodo_anterior = a_periodo_estudo;
		a_horizonte_estudo_estagio.incrementarIterador(proximo_periodo_estudo);
		a_horizonte_estudo_estagio.decrementarIterador(periodo_anterior);

		// --------------------
		//
		// Vazao Montante
		// 
		// --------------------

		try {

			const int posEquQMONT = addEquLinear_VAZAO_MONTANTE(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQMONT, 0.0);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QMON(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquQMONT, 1.0);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_YH(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posEquQMONT, -1.0);

		}// try

		catch (const std::exception& erro) { throw std::invalid_argument("Erro Restricao Vazao Montante: \n" + std::string(erro.what())); }

		// --------------------------
		//
		// Vazao Defluente
		// 
		// --------------------------

		try {

			const int posEquQDEF = addEquLinear_VAZAO_DEFLUENTE(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQDEF, 0.0);

			//QDEF
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquQDEF, 1.0);

			//QTURDISP
			if (getVarDecisao_QTURDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquQDEF, -1.0);

			//QVER
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QVER(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquQDEF, -1.0);


		}// try

		catch (const std::exception& erro) { throw std::invalid_argument("Erro Restricao Vazao Defluente Disponivel: \n" + std::string(erro.what())); }


		// -------------------------------
		//
		// Balanço Hídrico no Reservatório
		// 
		// -------------------------------

		const double conversor_vazao_volume = a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, a_periodo_estudo, a_idPatamarCarga, double());

		const int posEquBH = addEquLinear_BALANCO_HIDRAULICO_VAZ(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica);

		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquBH, 0.0);

		// ENCHENDO VOLUME MORTO 
		if (a_periodo_estudo > a_horizonte_estudo_estagio.getIteradorInicial()) {

			if (a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_enchendo_volume_morto, periodo_anterior, int()) == 1) {
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquBH, a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo, periodo_anterior, double()) / conversor_vazao_volume);
				const int posIneVM = addIneLinear_META_ENCHENDO_VOLUME_MORTO(a_TSS, a_idEstagio, periodo_anterior, a_idPatamarCarga, a_idHidreletrica);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneVM, a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_meta_enchimento_volume_morto, periodo_anterior, double()));
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posEquBH, 1.0);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VMORTO_FINF(a_TSS, a_idEstagio, periodo_anterior, a_idHidreletrica), posEquBH, 1.0);
			}
		}//if (a_periodo_estudo > a_horizonte_estudo_estagio.getIteradorInicial()) {

		// Variável VI
		if (getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1)
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posEquBH, -1.0 / conversor_vazao_volume);

		//VARIÁVEL EVAPORAÇÃO
		if (getVarDecisao_QEV_RELseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1) {
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QEV_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posEquBH, 1.0);

			// CRIA RESTRIÇÃO EVAPORAÇÃO
			if (a_idPatamarCarga == IdPatamarCarga_1 && a_dados.getSize1Matriz(a_idHidreletrica, AttMatrizHidreletrica_vazao_evaporada_meta) == 0)
				criarRestricoesEvaporacao(a_TSS, a_dados, a_idEstagio, a_periodo_estudo, a_idHidreletrica, proximo_periodo_estudo);
		}

		// Variáveis de Hidreletricas à Montante

		const int numero_hidreletricas_montante = a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_montante);
		for (int m = 1; m <= numero_hidreletricas_montante; m++) {
			const IdHidreletrica idHidreletrica_montante = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_montante, m, IdHidreletrica());

			const int tempo_viagem_agua = a_dados.getAtributo(idHidreletrica_montante, AttComumHidreletrica_tempo_viagem_agua, int());

			if (tempo_viagem_agua > 0) {
				const Periodo periodo_lag = Periodo(a_periodo_estudo.getTipoPeriodo(), Periodo(TipoPeriodo_horario, a_periodo_estudo) - tempo_viagem_agua);
				// Variável QDEFLAG
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(criarVariaveisDecisao_VariaveisEstado_Restricoes_QDEFLAG(a_TSS, a_dados, a_idEstagio, a_periodo_estudo, idHidreletrica_montante, periodo_lag), posEquBH, -1.0);
			}
			else {

				// Variável QDEF
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica_montante), posEquBH, -1.0);

			}//else {

		} // for (int m = 1; m <= numero_hidreletricas_montante; m++) {

		// Variáveis de Hidreletricas à Montante desvio
		const int numero_hidreletricas_montante_desvio = a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_montante_desvio);
		for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

			const IdHidreletrica idHidreletrica_montante_desvio = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_montante_desvio, m, IdHidreletrica());

			// Variável QDES que "entra" na usina
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica_montante_desvio, a_idHidreletrica), posEquBH, -1.0);

		}//for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

		// Variável QDES que "sai" da usina
		if (a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica()) != IdHidreletrica_Nenhum)
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posEquBH, 1.0);

		// Variável QMON
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QMON(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquBH, -1.0);

		// Variável QDEF
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquBH, 1.0);

		//QOUT
		if (getVarDecisao_QOUT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1)
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QOUT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquBH, 1.0);

		///////////////////////////
		//Intercâmbios Hidráulicos
		///////////////////////////
		//Nota:
		//Variável QDES (desvios de água por meio de canais -> jusante_desvio do HIDR.dat)
		//Variável QOUT (irrigação, retiradas)

		// hidreletrica_origem

		const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_origem = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_origem, a_idHidreletrica);

		const int idIntercambioHidraulico_origem_size = int(idIntercambioHidraulico_origem.size());

		for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

			const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

			if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

				if (getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())), posEquBH, 1.0);

			}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

			else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada) {

				if (a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_minimo, a_periodo_estudo, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_maximo, a_periodo_estudo, double()) > 0) {

					//Nesta condição o QOUT_REL entra no balanço hídrico
					if (getVarDecisao_QOUT_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())) > -1)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QOUT_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posEquBH, 1.0);

				}//if (a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_minimo, a_periodo_estudo, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_maximo, a_periodo_estudo, double()) > 0) {
				else {

					//Se o QOUT for negativo, não tem problema de inviabilizar o balanço hídrico
					if (getVarDecisao_QOUT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())) > -1)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QOUT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posEquBH, 1.0);

				}//else {

			}//else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada) {

			else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_pereira_barreto) {

				if (a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_minimo, a_periodo_estudo, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_maximo, a_periodo_estudo, double()) > 0) {

					//Nesta condição o QILS_TRI_REL entra no balanço hídrico
					if (getVarDecisao_QILS_TRI_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos)) > -1)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QILS_TRI_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos)), posEquBH, 1.0);

				}//if (a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_minimo, a_periodo_estudo, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_maximo, a_periodo_estudo, double()) > 0) {
				else {

					//Se o QILS_TRI for negativo, não tem problema de inviabilizar o balanço hídrico
					if (getVarDecisao_QILS_TRI(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos)) > -1)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QILS_TRI(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos)), posEquBH, 1.0);

				}//else {

			}//else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_pereira_barreto) {

		}//for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

		// hidreletrica_destino

		const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_destino = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_destino, a_idHidreletrica);

		const int idIntercambioHidraulico_destino_size = int(idIntercambioHidraulico_destino.size());

		for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {

			const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

			if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

				if (getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())), posEquBH, -1.0);

			}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

			else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_pereira_barreto) {

				if (getVarDecisao_QILS_TRI(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_destino.at(pos)) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QILS_TRI(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_destino.at(pos)), posEquBH, -1.0);

			}//else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_pereira_barreto) {

		}//for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {

		///////////////////////////
		//Usinas de bombeamento
		///////////////////////////
		//Nota: as usinas a montante = destino e jusante = origem

		const std::vector<IdUsinaElevatoria> idUsinaElevatoria_origem = a_dados.vetorUsinaElevatoria.getIdObjetos(AttComumUsinaElevatoria_usina_origem_bombeamento, a_idHidreletrica);

		const int idUsinaElevatoria_origem_size = int(idUsinaElevatoria_origem.size());

		for (int pos = 0; pos < idUsinaElevatoria_origem_size; pos++)
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaElevatoria_origem.at(pos)), posEquBH, a_dados.getElementoVetor(idUsinaElevatoria_origem.at(pos), AttVetorUsinaElevatoria_fator_disponibilidade, a_periodo_estudo, double()));

		const std::vector<IdUsinaElevatoria> idUsinaElevatoria_destino = a_dados.vetorUsinaElevatoria.getIdObjetos(AttComumUsinaElevatoria_usina_destino_bombeamento, a_idHidreletrica);

		const int idUsinaElevatoria_destino_size = int(idUsinaElevatoria_destino.size());

		for (int pos = 0; pos < idUsinaElevatoria_destino_size; pos++)
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaElevatoria_destino.at(pos)), posEquBH, -a_dados.getElementoVetor(idUsinaElevatoria_destino.at(pos), AttVetorUsinaElevatoria_fator_disponibilidade, a_periodo_estudo, double()));


	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesBalancoHidraulicoUsinaByVazao(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesBalancoHidraulicoUsinaByVolume(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdEstagio a_idEstagio_acoplamento, const Periodo a_periodo_acoplamento, const Periodo a_periodo_estudo, const Periodo a_periodo_otimizacao_final, const Periodo a_periodo_estudo_inicial, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga, const SmartEnupla<Periodo, int>& a_horizonte_processo_estocastico, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		// --------------------
		//
		// Vazao Afluente Montante
		// 
		// --------------------

		const double conversor_vazao_volume = a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, a_periodo_estudo, a_idPatamarCarga, double());

		try {

			const int posEquQMONT = addEquLinear_VAZAO_MONTANTE(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQMONT, 0.0);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QMON(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquQMONT, 1.0);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_YH(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posEquQMONT, -1.0);

		}// try

		catch (const std::exception& erro) { throw std::invalid_argument("Erro Restricao Vazao Montante: \n" + std::string(erro.what())); }

		// --------------------------
		//
		// Vazao Defluente 
		// 
		// --------------------------

		try {

			const int posEquQDEF = addEquLinear_VAZAO_DEFLUENTE(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQDEF, 0.0);

			//QDEF
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquQDEF, 1.0);

			//QTURDISP
			if (getVarDecisao_QTURDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquQDEF, -1.0);

			//QVER
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QVER(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquQDEF, -1.0);


		}// try

		catch (const std::exception& erro) { throw std::invalid_argument("Erro Restricao Vazao Defluente Disponivel: \n" + std::string(erro.what())); }


		// -------------------------------
		//
		// Balanço Hídrico no Reservatório
		// 
		// -------------------------------

		if (a_idPatamarCarga == IdPatamarCarga_1) {

			Periodo proximo_periodo_estudo = a_periodo_estudo;
			Periodo periodo_anterior = a_periodo_estudo;
			a_horizonte_estudo_estagio.incrementarIterador(proximo_periodo_estudo);
			a_horizonte_estudo_estagio.decrementarIterador(periodo_anterior);


			//EQUAÇÃO DE BALANÇO HIDRAULICO
			const int posEquBH = addEquLinear_BALANCO_HIDRAULICO_VOL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquBH, 0.0);

			// ENCHENDO VOLUME MORTO 
			if (a_periodo_estudo > a_horizonte_estudo_estagio.getIteradorInicial()) {

				if (a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_enchendo_volume_morto, periodo_anterior, int()) == 1) {

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquBH, a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo, periodo_anterior, double()));

					const int posIneVM = addIneLinear_META_ENCHENDO_VOLUME_MORTO(a_TSS, a_idEstagio, periodo_anterior, a_idPatamarCarga, a_idHidreletrica);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneVM, a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_meta_enchimento_volume_morto, periodo_anterior, double()));
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posEquBH, 1.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VMORTO_FINF(a_TSS, a_idEstagio, periodo_anterior, a_idHidreletrica), posEquBH, 1.0);
				}
			}//if (a_periodo_estudo > a_horizonte_estudo_estagio.getIteradorInicial()) {


			//VARIÁVEL EVAPORAÇÃO
			const double conversor_vazao_volume_periodo = a_dados.getElementoVetor(AttVetorDados_conversor_vazao_volume, a_periodo_estudo, double());

			if (getVarDecisao_QEV_RELseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1) {

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QEV_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posEquBH, conversor_vazao_volume_periodo);

				if (a_dados.getSize1Matriz(a_idHidreletrica, AttMatrizHidreletrica_vazao_evaporada_meta) == 0)
					criarRestricoesEvaporacao(a_TSS, a_dados, a_idEstagio, a_periodo_estudo, a_idHidreletrica, proximo_periodo_estudo);
			}

			// Variável VI
			if (getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posEquBH, -1.0);

			if (a_periodo_estudo < a_horizonte_estudo_estagio.getIteradorFinal()) {

				if (getVarDecisao_VIseExistir(a_TSS, a_idEstagio, proximo_periodo_estudo, a_idHidreletrica) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, proximo_periodo_estudo, a_idHidreletrica), posEquBH, 1.0);
			}
			else if ((a_periodo_estudo == a_horizonte_estudo_estagio.getIteradorFinal() && (getVarDecisao_VFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1)))
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posEquBH, 1.0);

			// Variáveis de Hidreletricas à Montante

			const int numero_hidreletricas_montante = a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_montante);
			for (int m = 1; m <= numero_hidreletricas_montante; m++) {
				const IdHidreletrica idHidreletrica_montante = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_montante, m, IdHidreletrica());

				const int tempo_viagem_agua = a_dados.getAtributo(idHidreletrica_montante, AttComumHidreletrica_tempo_viagem_agua, int());

				if (tempo_viagem_agua > 0) {
					const Periodo periodo_lag = Periodo(a_periodo_estudo.getTipoPeriodo(), Periodo(TipoPeriodo_horario, a_periodo_estudo) - tempo_viagem_agua);
					// Variável QDEFLAG
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(criarVariaveisDecisao_VariaveisEstado_Restricoes_QDEFLAG(a_TSS, a_dados, a_idEstagio, a_periodo_estudo, idHidreletrica_montante, periodo_lag), posEquBH, -conversor_vazao_volume_periodo);
				}

				else

					// Variável QDEF
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica_montante), posEquBH, -conversor_vazao_volume_periodo);


			} // for (int m = 1; m <= numero_hidreletricas_montante; m++) {

		} // if (a_idPatamarCarga == IdPatamarCarga_1){

		const int posEquBH = getEquLinear_BALANCO_HIDRAULICO_VOL(a_TSS, a_idEstagio, a_periodo_estudo, IdPatamarCarga_1, a_idHidreletrica);

		// Variáveis de Hidreletricas à Montante desvio
		const int numero_hidreletricas_montante_desvio = a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_montante_desvio);
		for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

			const IdHidreletrica idHidreletrica_montante_desvio = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_montante_desvio, m, IdHidreletrica());

			// Variável QDES que "entra" na usina
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica_montante_desvio, a_idHidreletrica), posEquBH, -conversor_vazao_volume);

		}//for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

		// Variável QDES que "sai" da usina
		if(a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica()) != IdHidreletrica_Nenhum)
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posEquBH, conversor_vazao_volume);


		// Variável QDEF
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquBH, conversor_vazao_volume);

		// Variável QMON
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QMON(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquBH, -conversor_vazao_volume);

		//QOUT
		if (getVarDecisao_QOUT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1)
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QOUT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posEquBH, conversor_vazao_volume);

		///////////////////////////
		//Intercâmbios Hidráulicos
		///////////////////////////
		//Nota:
		//Variável QDES (desvios de água por meio de canais -> jusante_desvio do HIDR.dat)
		//Variável QOUT (irrigação, retiradas)

		// hidreletrica_origem

		const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_origem = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_origem, a_idHidreletrica);

		const int idIntercambioHidraulico_origem_size = int(idIntercambioHidraulico_origem.size());

		for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

			const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

			if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

				if (getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())), posEquBH, conversor_vazao_volume);

			}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

			else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada) {

				if (a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_minimo, a_periodo_estudo, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_maximo, a_periodo_estudo, double()) > 0) {

					//Nesta condição o QOUT_REL entra no balanço hídrico
					if (getVarDecisao_QOUT_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())) > -1)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QOUT_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posEquBH, conversor_vazao_volume);

				}//if (a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_minimo, a_periodo_estudo, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_maximo, a_periodo_estudo, double()) > 0) {
				else {

					//Se o QOUT for negativo, não tem problema de inviabilizar o balanço hídrico
					if (getVarDecisao_QOUT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())) > -1)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QOUT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posEquBH, conversor_vazao_volume);

				}//else {

			}//else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada) {
			else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_pereira_barreto) {

				if (a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_minimo, a_periodo_estudo, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_maximo, a_periodo_estudo, double()) > 0) {

					//Nesta condição o QILS_TRI_REL entra no balanço hídrico
					if (getVarDecisao_QILS_TRI_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos)) > -1)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QILS_TRI_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos)), posEquBH, conversor_vazao_volume);

				}//if (a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_minimo, a_periodo_estudo, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico_origem.at(pos), AttVetorIntercambioHidraulico_desvio_agua_maximo, a_periodo_estudo, double()) > 0) {
				else {

					//Se o QILS_TRI for negativo, não tem problema de inviabilizar o balanço hídrico
					if (getVarDecisao_QILS_TRI(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos)) > -1)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QILS_TRI(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos)), posEquBH, conversor_vazao_volume);

				}//else {


			}//else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_pereira_barreto) {

		}//for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

		// hidreletrica_destino

		const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_destino = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_destino, a_idHidreletrica);

		const int idIntercambioHidraulico_destino_size = int(idIntercambioHidraulico_destino.size());

		for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {

			const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

			if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

				if (getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())), posEquBH, -conversor_vazao_volume);

			}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

			else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_pereira_barreto) {

				if (getVarDecisao_QILS_TRI(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_destino.at(pos)) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QILS_TRI(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_destino.at(pos)), posEquBH, -conversor_vazao_volume);

			}//else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_pereira_barreto) {

		}//for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {

		///////////////////////////

		//Usinas de bombeamento
		//Nota: as usinas a montante = destino e jusante = origem

		const std::vector<IdUsinaElevatoria> idUsinaElevatoria_origem = a_dados.vetorUsinaElevatoria.getIdObjetos(AttComumUsinaElevatoria_usina_origem_bombeamento, a_idHidreletrica);

		const int idUsinaElevatoria_origem_size = int(idUsinaElevatoria_origem.size());

		for (int pos = 0; pos < idUsinaElevatoria_origem_size; pos++)
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaElevatoria_origem.at(pos)), posEquBH, conversor_vazao_volume * a_dados.getElementoVetor(idUsinaElevatoria_origem.at(pos), AttVetorUsinaElevatoria_fator_disponibilidade, a_periodo_estudo, double()));

		const std::vector<IdUsinaElevatoria> idUsinaElevatoria_destino = a_dados.vetorUsinaElevatoria.getIdObjetos(AttComumUsinaElevatoria_usina_destino_bombeamento, a_idHidreletrica);

		const int idUsinaElevatoria_destino_size = int(idUsinaElevatoria_destino.size());

		for (int pos = 0; pos < idUsinaElevatoria_destino_size; pos++)
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaElevatoria_destino.at(pos)), posEquBH, -conversor_vazao_volume * a_dados.getElementoVetor(idUsinaElevatoria_destino.at(pos), AttVetorUsinaElevatoria_fator_disponibilidade, a_periodo_estudo, double()));


	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesBalancoHidraulicoUsinaByVolume(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesEvaporacao(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdHidreletrica a_idHidreletrica, const Periodo a_proximo_periodo_estudo) {

	try {
		const double conversor_vazao_volume = a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, a_periodo_estudo, IdPatamarCarga_1, double());
		// EVAPORAÇÃO
		const int posEquEV = addEquLinear_VAZAO_EVAPORADA(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica);

		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquEV, a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_coef_linear_evaporacao_0, a_periodo_estudo, double()));
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QEV(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posEquEV, 1.0);

		if (getVarDecisao_VMEDseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1)
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VMED(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posEquEV, -a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_coef_linear_evaporacao_1, a_periodo_estudo, double()));

		//EVAPORAÇÃO RELAXADA 
		const int posEquEV_REL = addEquLinear_VAZAO_EVAPORADA_RELAXADA(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica);
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquEV_REL, 0.0);
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QEV(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posEquEV_REL, -1.0);
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QEV_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posEquEV_REL, 1.0);
		if (getVarDecisao_QEV_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1) {
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QEV_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posEquEV_REL, 1.0);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QEV_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), 1.2 * a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_coef_linear_evaporacao_0, a_periodo_estudo, double()));
		}

	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesEvaporacao(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesAtendimentoDemanda(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdUsinaEolica a_maiorIdUsinaEolica, const IdContrato a_maiorIdContrato, const IdSubmercado a_idSubmercado, const IdIntercambio a_maiorIdIntercambio, const IdTermeletrica a_maiorIdTermeletrica, const IdHidreletrica a_maiorIdHidreletrica, const IdUsinaElevatoria a_maiorIdUsinaElevatoria, const IdPatamarCarga a_idPatamarCarga) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const int posEquAD = addEquLinear_ATENDIMENTO_DEMANDA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idSubmercado);
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquAD, 0.0);

		// Variável PL
		if (getVarDecisao_PLseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idSubmercado) > -1)
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idSubmercado), posEquAD, -1.0);

		//
		// Variável Potência Usina Nao Simulada PN
		//

		for (IdUsinaNaoSimulada idUsinaNaoSimulada = IdUsinaNaoSimulada(IdUsinaNaoSimulada_Nenhum + 1); idUsinaNaoSimulada < IdUsinaNaoSimulada_Excedente; idUsinaNaoSimulada++) {

			if (getVarDecisao_PNseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idSubmercado, idUsinaNaoSimulada) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PN(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idSubmercado, idUsinaNaoSimulada), posEquAD, 1.0);

		} // for (IdUsinaNaoSimulada idUsinaNaoSimulada = IdUsinaNaoSimulada(IdUsinaNaoSimulada_Nenhum + 1); idUsinaNaoSimulada < IdUsinaNaoSimulada_Excedente; idUsinaNaoSimulada++) {

		//USINAS EOLICAS 
		for (IdUsinaEolica idUsinaEolica = IdUsinaEolica_1; idUsinaEolica < a_maiorIdUsinaEolica; idUsinaEolica++)
			if (a_dados.getAtributo(idUsinaEolica, AttComumUsinaEolica_submercado, IdSubmercado()) == a_idSubmercado)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PE(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaEolica), posEquAD, 1.0);


		// Variaveis Patamares Deficit
		const IdPatamarDeficit maiorIdPatamarDeficit = a_dados.getMaiorId(a_idSubmercado, IdPatamarDeficit());
		for (IdPatamarDeficit idPatamarDeficit = IdPatamarDeficit_1; idPatamarDeficit <= maiorIdPatamarDeficit; idPatamarDeficit++) {

			// Variável PD
			if (getVarDecisao_PDseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idSubmercado, idPatamarDeficit) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PD(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idSubmercado, idPatamarDeficit), posEquAD, 1.0);

		} // for (IdPatamarDeficit idPatamarDeficit = IdPatamarDeficit_1; idPatamarDeficit <= maiorIdPatamarDeficit; idPatamarDeficit++) {


		// CONTRATOS
		for (IdContrato idContrato = IdContrato_1; idContrato <= a_maiorIdContrato; idContrato++) {

			if ((getVarDecisao_CEXPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idContrato) > -1) && a_dados.getAtributo(idContrato, AttComumContrato_submercado, IdSubmercado()) == a_idSubmercado)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_CEXP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idContrato), posEquAD, -1.0);

			if ((getVarDecisao_CIMPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idContrato) > -1) && a_dados.getAtributo(idContrato, AttComumContrato_submercado, IdSubmercado()) == a_idSubmercado)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_CIMP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idContrato), posEquAD, 1.0);

		}//for (IdContrato idContrato = IdContrato_1; idContrato <= maiorIdContrato; idContrato++) {



		// Variaveis Termeletricas
		for (IdTermeletrica idTermeletrica = a_dados.getMenorId(IdTermeletrica()); idTermeletrica <= a_maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idTermeletrica)) {

			const TipoDetalhamentoProducaoTermeletrica tipo_detalhamento_producao = a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica());

			if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina) {

				// Variável PTDISP
				if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) > -1) {

					if (a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_submercado, IdSubmercado()) == a_idSubmercado) {
						if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) > -1)
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), posEquAD, 1.0);
					}

				} // if (getVarDecisao_PFseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) > -1){

			} // if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina) {

			else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {

				for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= a_dados.getMaiorId(idTermeletrica, IdUnidadeUTE()); idUnidadeUTE++) {

					// Variável PTDISP
					if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE) > -1) {

						if (a_dados.getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_submercado, IdSubmercado()) == a_idSubmercado) {
							if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE) > -1)
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posEquAD, 1.0);
						}

					} // if (getVarDecisao_PTDISPseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE) > -1) {

				} // for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= a_dados.getMaiorId(idTermeletrica, IdUnidadeUTE()); idUnidadeUTE++) {

			} // else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {

		} // for (IdTermeletrica idTermeletrica = a_dados.getMenorId(IdTermeletrica()); idTermeletrica <= maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idTermeletrica)) {


		// Variaveis Hidreletricas

		for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= a_maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			const TipoDetalhamentoProducaoHidreletrica tipo_detalhamento_producao = a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_tipo_detalhamento_producao, a_periodo_estudo, TipoDetalhamentoProducaoHidreletrica());

			//
			// Producao Por Hidreletrica
			//

			if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina) {

				// Variável PHDISP
				if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1) {

					if (a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_submercado, IdSubmercado()) == a_idSubmercado)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posEquAD, 1.0);

				} // if (getVarDecisao_PHDISPseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1) {

			} // if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina){

			//
			// Producao Por Conjunto Hidraulico
			//

			else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {
				for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

					// Variável PHDISP
					if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico) > -1) {

						if (a_dados.getAtributo(idHidreletrica, idConjuntoHidraulico, AttComumConjuntoHidraulico_submercado, IdSubmercado()) == a_idSubmercado)
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico), posEquAD, 1.0);

					} // if (getVarDecisao_PHDISPseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1) {

				} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {
			} // else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

			//
			// Producao Por Unidade UHE
			//

			else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) {
				for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {
					for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {

						// Variável PHDISP
						if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

							if (a_dados.getAtributo(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttComumUnidadeUHE_submercado, IdSubmercado()) == a_idSubmercado)
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquAD, 1.0);

						} // if (getVarDecisao_PHDISPseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

					} // for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {
				} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {
			} // else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) {

		} // for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {


		// Variaveis Intercambio
		for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= a_maiorIdIntercambio; idIntercambio++) {

			// Variável PI
			if (getVarDecisao_PIseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambio) > -1) {

				if (a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_origem, IdSubmercado()) == a_idSubmercado)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambio), posEquAD, -1.0);

				else if (a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_destino, IdSubmercado()) == a_idSubmercado)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambio), posEquAD, 1.0);

			} // if (getVarDecisao_PIseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambio) > -1){

		} // for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= maiorIdIntercambio; idIntercambio++) {

		//Consumo das usinas elevatórias (QBOM)

		for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria <= a_maiorIdUsinaElevatoria; idUsinaElevatoria++) {

			if (a_dados.getAtributo(idUsinaElevatoria, AttComumUsinaElevatoria_submercado, IdSubmercado()) == a_idSubmercado) {

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PBOMDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaElevatoria), posEquAD, -1.0);

			}//if (a_dados.getAtributo(idUsinaElevatoria, AttComumUsinaElevatoria_submercado, IdSubmercado()) == a_idSubmercado) {

		}//for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria <= a_maiorIdUsinaElevatoria; idUsinaElevatoria++) {

	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesAtendimentoDemanda(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesProducaoMedia(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdSubmercado a_idSubmercado, const IdIntercambio a_maiorIdIntercambio, const IdTermeletrica a_maiorIdTermeletrica, const IdHidreletrica a_maiorIdHidreletrica, const IdPatamarCarga a_idPatamarCarga) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

			// DEMANDA
			if (true) {

				if (getVarDecisao_PLseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idSubmercado) > -1) {

					int posEquDemanda = getEquLinear_DEMANDAseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idSubmercado);

					if (posEquDemanda < 0) {
						posEquDemanda = addEquLinear_DEMANDA(a_TSS, a_idEstagio, a_periodo_estudo, a_idSubmercado);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquDemanda, 0.0);
						// Variável PL Media
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PL(a_TSS, a_idEstagio, a_periodo_estudo, a_idSubmercado), posEquDemanda, 1.0);
					}

					// Variável PL
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idSubmercado), posEquDemanda, -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, a_idPatamarCarga, double()));

				} // if (getVarDecisao_PLseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idSubmercado) > -1) {

			} // if (true) {

			// DEFICT
			if (true) {

				const IdPatamarDeficit maiorIdPatamarDeficit = a_dados.getMaiorId(a_idSubmercado, IdPatamarDeficit());
				for (IdPatamarDeficit idPatamarDeficit = IdPatamarDeficit_1; idPatamarDeficit <= maiorIdPatamarDeficit; idPatamarDeficit++) {

					if (getVarDecisao_PDseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idSubmercado, idPatamarDeficit) > -1) {

						int posEquDeficit = getEquLinear_DEFICITseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idSubmercado);

						if (posEquDeficit < 0) {
							posEquDeficit = addEquLinear_DEFICIT(a_TSS, a_idEstagio, a_periodo_estudo, a_idSubmercado);
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquDeficit, 0.0);
							// Variável PL Media
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PD(a_TSS, a_idEstagio, a_periodo_estudo, a_idSubmercado), posEquDeficit, 1.0);
						}

						// Variável PD
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PD(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idSubmercado, idPatamarDeficit), posEquDeficit, -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, a_idPatamarCarga, double()));

					} // if (getVarDecisao_PDseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idSubmercado, idPatamarDeficit) > -1){

				} // for (IdPatamarDeficit idPatamarDeficit = IdPatamarDeficit_1; idPatamarDeficit <= maiorIdPatamarDeficit; idPatamarDeficit++) {

			} // if (true) {

			// BOMBEAMENTO
			if (true) {

				for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria <= a_dados.getMaiorId(IdUsinaElevatoria()); idUsinaElevatoria++) {

					if (getVarDecisao_PBOMDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaElevatoria) > -1) {

						if (getEquLinear_POTENCIA_BOMBEAMENTO_DISPONIVELseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idUsinaElevatoria) == -1) {

							const int posEquPBOMDISP = addEquLinear_POTENCIA_BOMBEAMENTO_DISPONIVEL(a_TSS, a_idEstagio, a_periodo_estudo, idUsinaElevatoria);
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPBOMDISP, 0.0);
							// Variável PBOMDISP Media
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PBOMDISP(a_TSS, a_idEstagio, a_periodo_estudo, idUsinaElevatoria), posEquPBOMDISP, 1.0);

						} // if (getEquLinear_POTENCIA_BOMBEAMENTO_DISPONIVELseExistir(a_idEstagio, a_periodo_estudo, idUsinaElevatoria) == -1) {

						// Variável PHDISP
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PBOMDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaElevatoria), getEquLinear_POTENCIA_BOMBEAMENTO_DISPONIVEL(a_TSS, a_idEstagio, a_periodo_estudo, idUsinaElevatoria), -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, a_idPatamarCarga, double()));

					}

				} // for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria <= a_dados.getMaiorId(IdUsinaElevatoria()); idUsinaElevatoria++) {
			} // if (true) {

			// INTERCAMBIO
			if (true) {

				for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= a_maiorIdIntercambio; idIntercambio++) {

					if (getVarDecisao_PIseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambio) > -1) {

						if ((a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_origem, IdSubmercado()) == a_idSubmercado) || (a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_destino, IdSubmercado()) == a_idSubmercado)) {

							int posEquIntercambio = getEquLinear_INTERCAMBIOseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idSubmercado);

							if (posEquIntercambio < 0) {
								posEquIntercambio = addEquLinear_INTERCAMBIO(a_TSS, a_idEstagio, a_periodo_estudo, a_idSubmercado);
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquIntercambio, 0.0);
								// Variável PI Media
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_periodo_estudo, a_idSubmercado), posEquIntercambio, 1.0);
							}

							// Variável PI
							if (a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_origem, IdSubmercado()) == a_idSubmercado)
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambio), posEquIntercambio, a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, a_idPatamarCarga, double()));

							else if (a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_destino, IdSubmercado()) == a_idSubmercado)
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambio), posEquIntercambio, -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, a_idPatamarCarga, double()));

						} // if ((a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_origem, IdSubmercado()) == a_idSubmercado) || (a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_destino, IdSubmercado()) == a_idSubmercado)) {

					} // if (getVarDecisao_PIseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambio) > -1){

				} // for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= a_maiorIdIntercambio; idIntercambio++) {

			} // if (true) {

		} // if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica)

		// POTENCIA_HIDRAULICA_DISPONIVEL
		if (true) {

			for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= a_maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

				if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

					// Variável PHDISP
					if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1) {

						if (true) {
							if (getEquLinear_POTENCIA_HIDRAULICA_DISPONIVELseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica) == -1) {
								const int posEquProducaoHidreletrica = addEquLinear_POTENCIA_HIDRAULICA_DISPONIVEL(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica);
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquProducaoHidreletrica, 0.0);
								// Variável PHDISP Media
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica), posEquProducaoHidreletrica, 1.0);
							}

							// Variável PHDISP
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), getEquLinear_POTENCIA_HIDRAULICA_DISPONIVEL(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica), -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, a_idPatamarCarga, double()));
						}

					} // if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1) {

				} // if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

				if (getVarDecisao_QTURDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1) {

					if (true) {
						if (getEquLinear_VAZAO_TURBINADA_DISPONIVELseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica) == -1) {
							const int posEquVazaoTurbinadaHidreletrica = addEquLinear_VAZAO_TURBINADA_DISPONIVEL(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica);
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquVazaoTurbinadaHidreletrica, 0.0);
							// Variável QTURDISP Media
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica), posEquVazaoTurbinadaHidreletrica, 1.0);
						}
						// Variável QTURDISP
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), getEquLinear_VAZAO_TURBINADA_DISPONIVEL(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica), -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, a_idPatamarCarga, double()));
					}

				} // if (getVarDecisao_QTURDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1) {


				// Vazão Defluente Disponível

				if (getEquLinear_VAZAO_DEFLUENTEseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica) == -1) {
					const int posEquVazaoDefluenteDispHidreletrica = addEquLinear_VAZAO_DEFLUENTE(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquVazaoDefluenteDispHidreletrica, 0.0);
					// Variável QDEF Media
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica), posEquVazaoDefluenteDispHidreletrica, 1.0);
				}

				// Variável QDEF
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), getEquLinear_VAZAO_DEFLUENTE(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica), -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, a_idPatamarCarga, double()));


				// Vazão Vertida

				if (getEquLinear_VAZAO_VERTIDAseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica) == -1) {
					const int posEquVazaoVertidaHidreletrica = addEquLinear_VAZAO_VERTIDA(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquVazaoVertidaHidreletrica, 0.0);
					// Variável QVER Media
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QVER(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica), posEquVazaoVertidaHidreletrica, 1.0);
				}

				// Variável QVER
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QVER(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), getEquLinear_VAZAO_VERTIDA(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica), -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, a_idPatamarCarga, double()));

			} // for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= a_maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

		} // if (true) {


		if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

			// PRODUCAO_TERMELETRICA
			if (true) {

				for (IdTermeletrica idTermeletrica = a_dados.getMenorId(IdTermeletrica()); idTermeletrica <= a_maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idTermeletrica)) {

					// Variável PTDISP
					if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) > -1) {

						if (getEquLinear_POTENCIA_TERMICA_DISPONIVELseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idTermeletrica) == -1) {
							const int posEquProducaoTermeletrica = addEquLinear_POTENCIA_TERMICA_DISPONIVEL(a_TSS, a_idEstagio, a_periodo_estudo, idTermeletrica);
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquProducaoTermeletrica, 0.0);
							// Variável PTDISP Media
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, idTermeletrica), posEquProducaoTermeletrica, 1.0);
						}

						// Variável PTDISP
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), getEquLinear_POTENCIA_TERMICA_DISPONIVEL(a_TSS, a_idEstagio, a_periodo_estudo, idTermeletrica), -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, a_idPatamarCarga, double()));
					} // if (getVarDecisao_PTseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) > -1) {

				} // for (IdTermeletrica idTermeletrica = a_dados.getMenorId(IdTermeletrica()); idTermeletrica <= a_maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idTermeletrica)) {

			} // if (true) {

			// PRODUCAO_USINA_NAO_SIMULADA
			if (true) {

				const IdUsinaNaoSimulada maiorIdUsinaNaoSimulada = a_dados.getMaiorId(a_idSubmercado, IdUsinaNaoSimulada());
				for (IdUsinaNaoSimulada idUsinaNaoSimulada = IdUsinaNaoSimulada(IdUsinaNaoSimulada_Nenhum + 1); idUsinaNaoSimulada < IdUsinaNaoSimulada_Excedente; idUsinaNaoSimulada++) {

					if (getVarDecisao_PNseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idSubmercado, idUsinaNaoSimulada) > -1) {

						int posEquDeficit = getEquLinear_PRODUCAO_USINA_NAO_SIMULADAseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idSubmercado);

						if (posEquDeficit < 0) {
							posEquDeficit = addEquLinear_PRODUCAO_USINA_NAO_SIMULADA(a_TSS, a_idEstagio, a_periodo_estudo, a_idSubmercado);
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquDeficit, 0.0);
							// Variável PN Media
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PN(a_TSS, a_idEstagio, a_periodo_estudo, a_idSubmercado), posEquDeficit, 1.0);
						}

						// Variável PN
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PN(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idSubmercado, idUsinaNaoSimulada), posEquDeficit, -a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, a_idPatamarCarga, double()));

					} // if (getVarDecisao_PNseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idSubmercado, idUsinaNaoSimulada) > -1){

				} // for (IdUsinaNaoSimulada idUsinaNaoSimulada = IdUsinaNaoSimulada(IdUsinaNaoSimulada_Nenhum + 1); idUsinaNaoSimulada < IdUsinaNaoSimulada_Excedente; idUsinaNaoSimulada++) {

			} // if (true) {

		} // if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesProducaoMedia(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarRestricoesCustoOperacao_periodoEstudo_patamarCarga(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdSubmercado a_maiorIdSubmercado, const IdTermeletrica a_maiorIdTermeletrica, const IdPatamarCarga a_idPatamarCarga) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const int posEquZO = addEquLinear_CUSTO_OPERACAO(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga);

		IdContrato maiorIdContrato = a_dados.getMaiorId(IdContrato());

		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZO, 0.0);

		// Variável ZO
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZO(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga), posEquZO, 1.0);

		// CONTRATOS
		for (IdContrato idContrato = IdContrato_1; idContrato <= maiorIdContrato; idContrato++) {

			if (getVarDecisao_CEXPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idContrato) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_CEXP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idContrato), posEquZO, a_dados.getElementoMatriz(idContrato, AttMatrizContrato_preco_energia_imp_exp, a_periodo_estudo, a_idPatamarCarga, double()));

			else if (getVarDecisao_CIMPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idContrato) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_CIMP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idContrato), posEquZO, -a_dados.getElementoMatriz(idContrato, AttMatrizContrato_preco_energia_imp_exp, a_periodo_estudo, a_idPatamarCarga, double()));

			else
				throw std::invalid_argument("Não encontrada variavel de contrato");

		}//for (IdContrato idContrato = IdContrato_1; idContrato <= maiorIdContrato; idContrato++) {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_otimizacao = vetorEstagio.att(a_idEstagio).getAtributo(AttComumEstagio_periodo_otimizacao, Periodo());

		// Variável PF
		for (IdTermeletrica idTermeletrica = a_dados.getMenorId(IdTermeletrica()); idTermeletrica <= a_maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idTermeletrica)) {

			const TipoDetalhamentoProducaoTermeletrica tipo_detalhamento_producao = a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica());
			const double custo_de_operacao = a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_custo_de_operacao, a_periodo_estudo, a_idPatamarCarga, double());

			if (custo_de_operacao > 0.0) {

				IdUnidadeUTE maiorIdUnidadeUTE = a_dados.getMaiorId(idTermeletrica, IdUnidadeUTE());

				if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina)
					maiorIdUnidadeUTE = IdUnidadeUTE_1;

				for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= maiorIdUnidadeUTE; idUnidadeUTE++) {

					int varPTDISP = -1;

					if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina)
						varPTDISP = getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica);
					else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade)
						varPTDISP = getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE);

					if (varPTDISP > -1)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISP, posEquZO, -custo_de_operacao);

					if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina)
						break;

				} // for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= a_dados.getMaiorId(idTermeletrica, IdUnidadeUTE()); idUnidadeUTE++) {
			} // if (custo_de_operacao > 0.0){
		} // for (IdTermeletrica idTermeletrica = a_dados.getMenorId(IdTermeletrica()); idTermeletrica <= maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idTermeletrica)) {

		// Variáveis Déficit
		for (IdSubmercado idSubmercado = a_dados.getMenorId(IdSubmercado()); idSubmercado <= a_maiorIdSubmercado; a_dados.vetorSubmercado.incr(idSubmercado)) {

			const IdPatamarDeficit maiorIdPatamarDeficit = a_dados.getMaiorId(idSubmercado, IdPatamarDeficit());
			for (IdPatamarDeficit idPatamarDeficit = IdPatamarDeficit_1; idPatamarDeficit <= maiorIdPatamarDeficit; idPatamarDeficit++) {

				// Variável PD
				if (getVarDecisao_PDseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idSubmercado, idPatamarDeficit) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PD(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idSubmercado, idPatamarDeficit), posEquZO, -a_dados.getElementoMatriz(idSubmercado, idPatamarDeficit, AttMatrizPatamarDeficit_custo, a_periodo_estudo, a_idPatamarCarga, double()));

			} // for (IdPatamarDeficit idPatamarDeficit = IdPatamarDeficit_1; idPatamarDeficit <= maiorIdPatamarDeficit; idPatamarDeficit++) {

		} // for (IdSubmercado idSubmercado = a_dados.getMenorId(IdSubmercado()); idSubmercado <= maiorIdSubmercado; a_dados.vetorSubmercado.incr(idSubmercado)){

	}// try {
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesCustoOperacao_periodoEstudo_patamarCarga(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }

}

void ModeloOtimizacao::criarRestricoesCustoPenalidade_periodoEstudo(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdRestricaoOperativaUHE a_maiorIdRestricaoOperativaUHE, const IdHidreletrica a_maiorIdHidreletrica) {
	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const int posEquZP = addEquLinear_CUSTO_PENALIDADE(a_TSS, a_idEstagio, a_periodo_estudo);

		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZP, 0.0);

		// Variável ZP
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZP(a_TSS, a_idEstagio, a_periodo_estudo), posEquZP, 1.0);

		// Variáveis Restrição Operativa UHE
		for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= a_maiorIdRestricaoOperativaUHE; idRestricaoOperativaUHE++) {

			//
			// Volume Armazenado
			//
			
			if (a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_volume_armazenado) {

				// Variável RHV_FINF
				if (getVarDecisao_RHV_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHV_FINF(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE), posEquZP, -a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_penalidade_restricao_operativa_UHE, double()));

				// Variável RHV_FSUP
				if (getVarDecisao_RHV_FSUPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHV_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE), posEquZP, -a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_penalidade_restricao_operativa_UHE, double()));

			}//if (a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_volume_armazenado) {
	

			//
			// Energia Armazenada
			//

			if (a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_energia_armazenada) {

				// Variável RHE_FINF
				if (getVarDecisao_RHE_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHE_FINF(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE), posEquZP, -a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_penalidade_restricao_operativa_UHE, double()));

				// Variável RHE_FSUP
				if (getVarDecisao_RHE_FSUPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHE_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE), posEquZP, -a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_penalidade_restricao_operativa_UHE, double()));

			}//if (a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_energia_armazenada) {


		} // for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= maiorIdRestricaoOperativaUHE; idRestricaoOperativaUHE++) {


		// Variáveis Hidreletricas
		for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= a_maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			// Variável QEV_FINF
			if ((getVarDecisao_QEV_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica) > -1))
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QEV_FINF(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica), posEquZP, -a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_evaporacao, double()));

			// Variável YHF Penalizacao e / ou Viabilidade Hidraulica
			if (getVarDecisao_YHFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_YHF(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica), posEquZP, -a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_afluencia_incremental, double()));

		} // for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

		if (getElementoVetor(AttVetorModeloOtimizacao_alguma_variavel_aleatoria_hidrologica_com_truncamento, a_idEstagio, int()) == 1) {

			const IdProcessoEstocastico idPEhidrologico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

			const Periodo periodo_otimizacao = getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

			// Variáveis Hidreletricas Folga Afluencia Incremental
			const SmartEnupla<Periodo, int> horizonte_espaco_amostral_hidrologico = getElementosMatriz(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, a_idEstagio, Periodo(), int());
			bool sobreposicao_encontrada = false;
			for (Periodo periodo = horizonte_espaco_amostral_hidrologico.getIteradorInicial(); periodo <= horizonte_espaco_amostral_hidrologico.getIteradorFinal(); horizonte_espaco_amostral_hidrologico.incrementarIterador(periodo)) {
				const double sobreposicao = a_periodo_estudo.sobreposicao(periodo);
				if (sobreposicao > 0.0) {

					for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= getMaiorId(idPEhidrologico, IdVariavelAleatoria()); idVar++) {
						if (getElementoVetor(idPEhidrologico, idVar, AttVetorVariavelAleatoria_tipo_relaxacao, periodo, TipoRelaxacaoVariavelAleatoria()) == TipoRelaxacaoVariavelAleatoria_truncamento_penalizacao) {
							// Variável YPF Truncamento
							const int YPF = getVarDecisao_YPFseExistir(a_TSS, a_idEstagio, periodo_otimizacao, periodo, idPEhidrologico, idVar);
							if (YPF > -1) {
								double penalidade = 0.0;
								for (IdVariavelAleatoriaInterna idVarInt = IdVariavelAleatoriaInterna_1; idVarInt <= getMaiorId(idPEhidrologico, idVar, IdVariavelAleatoriaInterna()); idVarInt++) {
									const IdHidreletrica idHidreletrica = getIdHidreletricaFromIdProcessoEstocasticoIdVariavelAleatoriaIdVariavelAleatoriaInterna(idPEhidrologico, idVar, idVarInt);
									penalidade += a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_afluencia_incremental, double()) * getElementoVetor(idPEhidrologico, idVar, idVarInt, AttVetorVariavelAleatoriaInterna_coeficiente_participacao, periodo, double());
								}
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(YPF, posEquZP, -sobreposicao * penalidade);
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

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesCustoPenalidade_periodoEstudo(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }

}


void ModeloOtimizacao::criarRestricoesCustoPenalidade_periodoEstudo_patamarCarga(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdSubmercado a_maiorIdSubmercado, const IdIntercambio a_maiorIdIntercambio, const IdRestricaoEletrica a_maiorIdRestricaoEletrica, const IdTermeletrica a_maiorIdTermeletrica, const IdRestricaoOperativaUHE a_maiorIdRestricaoOperativaUHE, const IdHidreletrica a_maiorIdHidreletrica, const IdIntercambioHidraulico a_maiorIdIntercambioHidraulico, const IdUsinaElevatoria a_maiorIdUsinaElevatoria, const Periodo a_periodo_otimizacao, const IdPatamarCarga a_idPatamarCarga) {
	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const int posEquZP = addEquLinear_CUSTO_PENALIDADE(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga);

		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquZP, 0.0);

		// Variável ZP
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga), posEquZP, 1.0);


		// Variável PI
		for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= a_maiorIdIntercambio; idIntercambio++) {

			if (getVarDecisao_PIseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambio) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambio), posEquZP, -a_dados.getAtributo(idIntercambio, AttComumIntercambio_penalidade_intercambio, double()));

			//PI_FINF
			if (getVarDecisao_PI_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambio) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PI_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambio), posEquZP, -a_dados.getAtributo(idIntercambio, AttComumIntercambio_penalidade_intercambio_minimo, double()));


		}//for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= a_maiorIdIntercambio; idIntercambio++) {

		// Variáveis Restrição Elétrica
		for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= a_maiorIdRestricaoEletrica; idRestricaoEletrica++) {

			// Variável RE_FINF
			if (getVarDecisao_RE_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoEletrica) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RE_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoEletrica), posEquZP, -a_dados.getAtributo(idRestricaoEletrica, AttComumRestricaoEletrica_penalidade_restricao_eletrica, double()));

			// Variável RE_FSUP
			if (getVarDecisao_RE_FSUPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoEletrica) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RE_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoEletrica), posEquZP, -a_dados.getAtributo(idRestricaoEletrica, AttComumRestricaoEletrica_penalidade_restricao_eletrica, double()));


		} // for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= maiorIdRestricaoEletrica; idRestricaoEletrica++) {

		// Variáveis Restrição Operativa UHE
		for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= a_maiorIdRestricaoOperativaUHE; idRestricaoOperativaUHE++) {

			//
			// Vazão Afluente
			//

			if (a_idPatamarCarga == IdPatamarCarga_1 && a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_vazao_afluente) {

				// Variável RHA_FINF
				if (getVarDecisao_RHA_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHA_FINF(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE), posEquZP, -a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_penalidade_restricao_operativa_UHE, double()));

				// Variável RHA_FSUP
				if (getVarDecisao_RHA_FSUPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHA_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE), posEquZP, -a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_penalidade_restricao_operativa_UHE, double()));

			}//if (a_idPatamarCarga == IdPatamarCarga_1 && a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_vazao_afluente) {

			//
			// Vazão Defluente
			//

			if (a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_vazao_defluente) {

				// Variável RHQ_FINF
				if (getVarDecisao_RHQ_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoOperativaUHE) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHQ_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoOperativaUHE), posEquZP, -a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_penalidade_restricao_operativa_UHE, double()));

				// Variável RHQ_FSUP
				if (getVarDecisao_RHQ_FSUPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoOperativaUHE) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHQ_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoOperativaUHE), posEquZP, -a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_penalidade_restricao_operativa_UHE, double()));

			}//if (a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_vazao_defluente) {

		} // for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= maiorIdRestricaoOperativaUHE; idRestricaoOperativaUHE++) {

		const IdEstagio estagio_acoplamento_pre_estudo = getAtributo(AttComumModeloOtimizacao_estagio_acoplamento_pre_estudo, IdEstagio());

		// Variáveis Hidreletricas
		for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= a_maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			// Variável QTUR / QTURDISP (penaliza a variável que está na FPH)

			if (a_dados.getAtributo(AttComumDados_representar_producao_hidreletrica_com_turbinamento_disponivel, bool())) {

				if (getVarDecisao_QTURDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posEquZP, -a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_turbinamento, double()));

			}//if (!a_dados.getAtributo(AttComumDados_representar_producao_hidreletrica_com_turbinamento_disponivel, bool())) {
			else {

				if (getVarDecisao_QTURseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posEquZP, -a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_turbinamento, double()));

			}//else {


			// Variável QVER
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QVER(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posEquZP, -a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_vertimento, double()));

			// Variável QDEF_FINF
			if (getVarDecisao_QDEF_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posEquZP, -a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_vazao_defluente_minima, double()));

			// Variável QDEF_FSUP
			if (getVarDecisao_QDEF_FSUPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posEquZP, -a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_vazao_defluente_maxima, double()));

			// Variável QDEF_FINF
			if (getVarDecisao_QDEF_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posEquZP, -a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_vazao_defluente_minima, double()));

			// Variável QDEF_FSUP
			if (getVarDecisao_QDEF_FSUPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posEquZP, -a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_vazao_defluente_maxima, double()));


			// Variável QDES_FINF
			if (getVarDecisao_QDES_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica())) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posEquZP, -a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_vazao_desviada_minima, double()));

			// Variável QOUT_FINF
			if (getVarDecisao_QOUT_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QOUT_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posEquZP, -a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_vazao_retirada, double()));

			
			// Variável VMORTO_FINF
			if ((getVarDecisao_VMORTO_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica) > -1) && (a_idPatamarCarga == IdPatamarCarga_1))
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VMORTO_FINF(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica), posEquZP, -a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_volume_minimo, double()));

			//
			// Produção Hidreletrica
			//

			// Variável PH_FINF
			if (getVarDecisao_PH_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posEquZP, -a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_potencia_minima, double()));

			// Variável PHDISP_FINF
			if (getVarDecisao_PHDISP_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posEquZP, -a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_potencia_minima, double()));

			// Variável QTUR_FINF
			if (getVarDecisao_QTUR_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posEquZP, -a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_turbinamento_minimo, double()));

			// Variável QTURDISP_FINF
			if (getVarDecisao_QTURDISP_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posEquZP, -a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_turbinamento_minimo, double()));


			if (a_dados.getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_meta) > 0) {

				if (a_idPatamarCarga == IdPatamarCarga_1) {

					if (getVarDecisao_PHDISPMETA_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica) > -1)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISPMETA_FINF(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica), posEquZP, -5000);

					if (getVarDecisao_PHDISPMETA_FSUPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica) > -1)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISPMETA_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica), posEquZP, -5000);


				}//if (a_idPatamarCarga == IdPatamarCarga_1) {

			}//if (a_dados.getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_meta) > 0) {

			for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

				//
				// Produção Conjunto Hidraulico
				//

				// Variável PH_FINF
				if (getVarDecisao_PH_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico), posEquZP, -a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_potencia_minima, double()));

				// Variável PHDISP_FINF
				if (getVarDecisao_PHDISP_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico), posEquZP, -a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_potencia_minima, double()));

				// Variável QTUR_FINF
				if (getVarDecisao_QTUR_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico), posEquZP, -a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_turbinamento_minimo, double()));

				for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {

					//
					// Produção Unidade Hidreletrica
					//

					// Variável PH_FINF
					if (getVarDecisao_PH_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquZP, -a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_potencia_minima, double()));

					// Variável PHDISP_FINF
					if (getVarDecisao_PHDISP_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquZP, -a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_potencia_minima, double()));

					// Variável QTUR_FINF
					if (getVarDecisao_QTUR_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posEquZP, -a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_turbinamento_minimo, double()));

				} // for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {

			} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

		} // for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

		// Variáveis Termeletricas
		for (IdTermeletrica idTermeletrica = a_dados.getMenorId(IdTermeletrica()); idTermeletrica <= a_maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idTermeletrica)) {

			const int codigo_usina = a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_codigo_usina, int());

			//
			// Produção Termeletrica
			//

			// Variável PT_FINF
			if (getVarDecisao_PT_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), posEquZP, -a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_penalidade_violacao_potencia, double()));

			// Variável PTDISP_FINF
			if (getVarDecisao_PTDISP_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), posEquZP, -a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_penalidade_violacao_potencia, double()));

			// Variável PTDISP_FSUP
			if (getVarDecisao_PTDISP_FSUPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISP_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), posEquZP, -a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_penalidade_violacao_potencia, double()));

			// Variável PTDISPCOM_FINF
			if (getVarDecisao_PTDISPCOM_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISPCOM_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), posEquZP, -a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_penalidade_violacao_comando, double()));

			// Variável PTDISP_FSUP
			if (getVarDecisao_PTDISPCOM_FSUPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PTDISPCOM_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), posEquZP, -a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_penalidade_violacao_comando, double()));

			for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= a_dados.getMaiorId(idTermeletrica, IdUnidadeUTE()); idUnidadeUTE++) {

				//
				// Produção Unidade Termeletrica
				//

				// Variável PT_FINF
				if (getVarDecisao_PT_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PT_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), posEquZP, -a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_penalidade_violacao_potencia, double()));

			} // for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= a_dados.getMaiorId(idTermeletrica, IdUnidadeUTE()); idUnidadeUTE++) {

		} // for (IdTermeletrica idTermeletrica = a_dados.getMenorId(IdTermeletrica()); idTermeletrica <= a_maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idTermeletrica)) {

		//Variáveis Intercambio Hidráulico

		for (IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico_1; idIntercambioHidraulico <= a_maiorIdIntercambioHidraulico; idIntercambioHidraulico++) {

			// Variável QOUT_FINF
			if (getVarDecisao_QOUT_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico, a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QOUT_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico, a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posEquZP, -a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_penalidade_retirada, double()));

			// Variável QILS_TRI_FINF
			if (getVarDecisao_QILS_TRI_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QILS_TRI_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico), posEquZP, -a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_penalidade_retirada, double()));



		}//for (IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico_1; idIntercambioHidraulico <= a_maiorIdIntercambioHidraulico; idIntercambioHidraulico++) {


		for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria <= a_maiorIdUsinaElevatoria; idUsinaElevatoria++) {

			// Variável QBOM_FINF
			if (getVarDecisao_QBOM_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaElevatoria) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaElevatoria), posEquZP, -a_dados.getAtributo(idUsinaElevatoria, AttComumUsinaElevatoria_penalidade_vazao_bombeada_minima, double()));

			// Variável QBOMDISP_FINF
			if (getVarDecisao_QBOMDISP_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaElevatoria) > -1)
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOMDISP_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaElevatoria), posEquZP, -a_dados.getAtributo(idUsinaElevatoria, AttComumUsinaElevatoria_penalidade_vazao_bombeada_minima, double()));


		}//for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria <= a_maiorIdUsinaElevatoria; idUsinaElevatoria++) {


	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesCustoPenalidade_periodoEstudo_patamarCarga(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }

}


void ModeloOtimizacao::criarRestricoesFuncaoProducaoHidreletrica(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga, const SmartEnupla<Periodo, double>a_horizonte_estudo_estagio, const Periodo a_periodo_estudo) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const TipoDetalhamentoProducaoHidreletrica tipo_detalhamento_producao = a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica());
		Periodo periodo_estudo_seguinte = a_periodo_estudo;
		a_horizonte_estudo_estagio.incrementarIterador(periodo_estudo_seguinte);

		if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina) {

			const int numero_planos = a_dados.getSize2Matriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, a_periodo_estudo);

			if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1) {

				if ((a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_vazao_turbinada_maxima, a_periodo_estudo, double()) > 0) && (a_dados.getElementoMatriz(a_idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima, a_periodo_estudo, a_idPatamarCarga, double()) > 0) && \
					(a_dados.getElementoMatriz(a_idHidreletrica, AttMatrizHidreletrica_potencia_maxima, a_periodo_estudo, a_idPatamarCarga, double()) > 0) && (a_dados.getElementoMatriz(a_idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_maxima, a_periodo_estudo, a_idPatamarCarga, double()) > 0)) {

					for (int i = 1; i <= numero_planos; i++) {

						int posRestricaoPH = -1;

						if (numero_planos == 1)
							posRestricaoPH = addEquLinear_POTENCIA_HIDRAULICA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica);
						else {
							posRestricaoPH = addIneLinear_POTENCIA_HIDRAULICA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, i);
						}

						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, a_periodo_estudo, i, double()));


						if (a_dados.getAtributo(AttComumDados_representar_producao_hidreletrica_com_turbinamento_disponivel, bool())) {

							if (a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_FC, a_periodo_estudo, i, double()) == 0.0)
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posRestricaoPH, -1.0);
							else
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posRestricaoPH, -1.0 / a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_FC, a_periodo_estudo, i, double()));


						}//if (a_dados.getAtributo(AttComumDados_representar_producao_hidreletrica_com_turbinamento_disponivel, bool())) {
						else {

							if (a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_FC, a_periodo_estudo, i, double()) == 0.0)
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posRestricaoPH, -1.0);
							else
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posRestricaoPH, -1.0 / a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_FC, a_periodo_estudo, i, double()));


						}//else {


						if (a_dados.getAtributo(AttComumDados_representar_producao_hidreletrica_com_turbinamento_disponivel, bool()))
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_QH, a_periodo_estudo, i, double()));
						else
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_QH, a_periodo_estudo, i, double()));

						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QVER(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_SH, a_periodo_estudo, i, double()));

						if (a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_periodo_estudo, i, double()) > 0.0)
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VMED(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_periodo_estudo, i, double()));

					} // for (int i = 1; i <= numero_planos; i++) {

				}//if ((a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_vazao_turbinada_maxima, a_periodo_estudo, double()) > 0) && (a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_vazao_turbinada_disponivel_maxima, a_periodo_estudo, double()) > 0)) {
				else {

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), 0.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), 0.0);

				}//else {

			} // if (getVarDecisao_PHseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica) > -1) {

		} // if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina){	


		else if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) || (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {

			if ((a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_vazao_turbinada_maxima, a_periodo_estudo, double()) > 0) && (a_dados.getElementoMatriz(a_idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima, a_periodo_estudo, a_idPatamarCarga, double()) > 0)) {

				for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

					if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

						const int numero_planos = a_dados.getSize2Matriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, a_periodo_estudo);

						if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico) > -1) {

							for (int i = 1; i <= numero_planos; i++) {

								int posRestricaoPH = -1;

								if (numero_planos == 1)
									posRestricaoPH = addEquLinear_POTENCIA_HIDRAULICA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico);
								else {
									posRestricaoPH = addIneLinear_POTENCIA_HIDRAULICA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, i);
								}

								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, a_periodo_estudo, i, double()));

								if (a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_FC, a_periodo_estudo, i, double()) == 0.0)
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posRestricaoPH, -1.0);
								else
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posRestricaoPH, -1.0 / a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_FC, a_periodo_estudo, i, double()));

								if (a_dados.getAtributo(AttComumDados_representar_producao_hidreletrica_com_turbinamento_disponivel, bool()))
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_QH, a_periodo_estudo, i, double()));
								else
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_QH, a_periodo_estudo, i, double()));

								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QVER(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_SH, a_periodo_estudo, i, double()));

								// VAZÃO TURBINADA DOS DEMAIS CONJUNTOS, É CONSIDERADA COMO VERTIMENTO PARA O ATUAL CONJUNTO
								for (IdConjuntoHidraulico idConjunto = IdConjuntoHidraulico_1; idConjunto <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjunto++)
									if (idConjunto != idConjuntoHidraulico)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjunto), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_SH, a_periodo_estudo, i, double()));

								if (a_periodo_estudo < a_horizonte_estudo_estagio.getIteradorFinal()) {
									if ((getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1) && (getVarDecisao_VIseExistir(a_TSS, a_idEstagio, periodo_estudo_seguinte, a_idHidreletrica) > -1)) {
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posRestricaoPH, 0.5 * a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_periodo_estudo, i, double()));
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, periodo_estudo_seguinte, a_idHidreletrica), posRestricaoPH, 0.5 * a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_periodo_estudo, i, double()));
									}
									else if ((getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1))
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_periodo_estudo, i, double()));

									else if ((getVarDecisao_VIseExistir(a_TSS, a_idEstagio, periodo_estudo_seguinte, a_idHidreletrica) > -1))
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, periodo_estudo_seguinte, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_periodo_estudo, i, double()));
								}

								else if (a_periodo_estudo == a_horizonte_estudo_estagio.getIteradorFinal()) {
									if ((getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1) && (getVarDecisao_VFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1)) {
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posRestricaoPH, 0.5 * a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_periodo_estudo, i, double()));
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posRestricaoPH, 0.5 * a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_periodo_estudo, i, double()));
									}
									else if (getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_periodo_estudo, i, double()));

									else if (getVarDecisao_VFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_periodo_estudo, i, double()));
								}

							} // for (int i = 1; i <= numero_planos; i++) {

						} // if (getVarDecisao_PHseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico) > -1) {

					} // if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {


					else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) {

						for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {

							const int numero_planos = a_dados.getSize2Matriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, a_periodo_estudo);

							if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

								for (int i = 1; i <= numero_planos; i++) {

									int posRestricaoPH = -1;

									if (numero_planos == 1)
										posRestricaoPH = addEquLinear_POTENCIA_HIDRAULICA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE);
									else {
										posRestricaoPH = addIneLinear_POTENCIA_HIDRAULICA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, i);
									}

									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, a_periodo_estudo, i, double()));

									if (a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_FC, a_periodo_estudo, i, double()) == 0.0)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posRestricaoPH, -1.0);
									else
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posRestricaoPH, -1.0 / a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_FC, a_periodo_estudo, i, double()));

									if (a_dados.getAtributo(AttComumDados_representar_producao_hidreletrica_com_turbinamento_disponivel, bool()))
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_QH, a_periodo_estudo, i, double()));
									else
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_QH, a_periodo_estudo, i, double()));

									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QVER(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_SH, a_periodo_estudo, i, double()));

									if (a_periodo_estudo < a_horizonte_estudo_estagio.getIteradorFinal()) {
										if ((getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1) && (getVarDecisao_VIseExistir(a_TSS, a_idEstagio, periodo_estudo_seguinte, a_idHidreletrica) > -1)) {
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posRestricaoPH, 0.5 * a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_periodo_estudo, i, double()));
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, periodo_estudo_seguinte, a_idHidreletrica), posRestricaoPH, 0.5 * a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_periodo_estudo, i, double()));
										}
										else if ((getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1))
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_periodo_estudo, i, double()));

										else if ((getVarDecisao_VIseExistir(a_TSS, a_idEstagio, periodo_estudo_seguinte, a_idHidreletrica) > -1))
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, periodo_estudo_seguinte, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_periodo_estudo, i, double()));
									}

									else if (a_periodo_estudo == a_horizonte_estudo_estagio.getIteradorFinal()) {
										if ((getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1) && (getVarDecisao_VFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1)) {
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posRestricaoPH, 0.5 * a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_periodo_estudo, i, double()));
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posRestricaoPH, 0.5 * a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_periodo_estudo, i, double()));
										}
										else if (getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1)
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_periodo_estudo, i, double()));

										else if (getVarDecisao_VFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica) > -1)
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_periodo_estudo, a_idHidreletrica), posRestricaoPH, a_dados.getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_VH, a_periodo_estudo, i, double()));
									}

								} // for (int i = 1; i <= numero_planos; i++) {

							} // if (getVarDecisao_PHseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1) {

						} // for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {

					} // else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) {

				} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {

			}//if ((a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_vazao_turbinada_maxima, a_periodo_estudo, double()) > 0) && (a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_vazao_turbinada_disponivel_maxima, a_periodo_estudo, double()) > 0)) {
			else {

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), 0.0);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idHidreletrica), 0.0);

			}//else {
					
		} // else if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) || (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {


	}// try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesFuncaoProducaoHidreletrica(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarVariaveisVolume(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const SmartEnupla<Periodo, double> &a_horizonte_estudo_estagio, const SmartEnupla<Periodo, IdEstagio> &a_horizonte_estudo_vetor) {

	try {

		const double infinito = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getInfinito();
		const IdHidreletrica maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

		const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
		const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());
		const Periodo   periodo_otimizacao = a_dados.getElementoVetor(AttVetorDados_horizonte_otimizacao, a_idEstagio, Periodo());
		const IdEstagio idEstagioAnterior = IdEstagio(a_idEstagio - 1);

		for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			if (a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_considerar_usina, bool())) {

				const int regularizacao_periodo_estudo = a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_regularizacao, a_periodo_estudo, int());

				//ENCHENDO VOLUME MORTO
				if (a_dados.getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_enchendo_volume_morto) > 0)
					if (a_dados.getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_enchendo_volume_morto, a_periodo_estudo, int()) == 1)
						addVarDecisao_VMORTO_FINF(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica, 0.0, infinito, 0.0);

				Periodo periodo_estudo_anterior = a_periodo_estudo;
				a_horizonte_estudo_vetor.decrementarIterador(periodo_estudo_anterior);

				int regularizacao_periodo_estudo_anterior = 0;


				// No primeiro período o VI é considerado para todas as hidrelétricas, visto a possibilidade de regularização no período pré-estudo quando modelos são acoplados.
				if ((a_idEstagio == estagio_inicial) && (a_periodo_estudo == a_horizonte_estudo_estagio.getIteradorInicial()))
					regularizacao_periodo_estudo_anterior = 1;

				else
					regularizacao_periodo_estudo_anterior = a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_regularizacao, periodo_estudo_anterior, int());

				// Variável Volume Inicial (VI)

				// No período inicial de cada estágio os limites de VI serão atualizados dinamicamente com variáveis de estado.
				//Se a usina tem regularizacao_periodo_estudo_anterior = 1 pega o estado do estágio anterior
				//Se a usina tem regularizacao_periodo_estudo_anterior = 0 faz o VI = 0
				//Se o periodo > a_horizonte_estudo_estagio.getIteradorInicial(), limita com o volume_maximo do periodo_estudo_anterior

				if ((a_idEstagio == estagio_inicial) && (a_periodo_estudo == a_horizonte_estudo_estagio.getIteradorInicial())) {

					if (estagio_inicial == IdEstagio_1)
						addVarDecisao_VI(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica, a_dados.getAtributo(idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_util_inicial, double()), a_dados.getAtributo(idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_util_inicial, double()), 0.0);
					else {
						const double volume_util_maximo = a_dados.getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, a_periodo_estudo, double());
						if (volume_util_maximo > 0.0) {
							const double volume_minimo = a_dados.getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo, a_periodo_estudo, double());
							vetorEstagio.att(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeVarDecisao_VI(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica) + "," + getString(volume_minimo) + "," + getString(volume_util_maximo)), addVarDecisao_VI(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica, 0.0, 0.0, 0.0), -1);
						}
					}

				} // if ((a_idEstagio == estagio_inicial) && (a_periodo_estudo == a_horizonte_estudo_estagio.getIteradorInicial())) {

				else if (a_periodo_estudo == a_horizonte_estudo_estagio.getIteradorInicial() && regularizacao_periodo_estudo_anterior) {

					const IdEstagio idEstagioAnterior = IdEstagio(a_idEstagio - 1);

					const Periodo periodo_estudo_final_estagio_anterior = a_dados.getElementosMatriz(AttMatrizDados_percentual_duracao_horizonte_estudo, idEstagioAnterior, Periodo(), double()).getIteradorFinal();
					
					int varVF = -1;
					if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica)
						varVF = getVarDecisao_VF(a_TSS, idEstagioAnterior, periodo_estudo_final_estagio_anterior, idHidreletrica);

					const double volume_util_maximo = a_dados.getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, a_periodo_estudo, double());
					const double volume_minimo = a_dados.getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo, a_periodo_estudo, double());
					
					vetorEstagio.att(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeVarDecisao_VI(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica) + "," + getString(volume_minimo) + "," + getString(volume_util_maximo)), addVarDecisao_VI(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica, 0.0, 0.0, 0.0), varVF);
					
				} // else if (a_periodo_estudo == a_horizonte_estudo_estagio.getIteradorInicial()) {

				else if (a_periodo_estudo == a_horizonte_estudo_estagio.getIteradorInicial() && !regularizacao_periodo_estudo_anterior)
					addVarDecisao_VI(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica, 0.0, 0.0, 0.0);

				else
					addVarDecisao_VI(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica, 0.0, a_dados.getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, periodo_estudo_anterior, double()), 0.0);


				// Variável Volume Final (VF)
				if (a_periodo_estudo == a_horizonte_estudo_estagio.getIteradorFinal()) {

					const int varDecisao_VF = addVarDecisao_VF(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica, 0.0, a_dados.getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, a_periodo_estudo, double()), 0.0);

					if ((!regularizacao_periodo_estudo) || (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)) // Se a usina nao tem regularizaçao entao zera o limite final
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(varDecisao_VF, 0.0);

				} // if ((regularizacao_periodo_estudo) && (a_periodo_estudo == a_horizonte_estudo_estagio.getIteradorFinal())) {

				const int varVI = getVarDecisao_VIseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica);
				const int varVF = getVarDecisao_VFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica);

				// Vaiável Volume Médio (VM)
				if ((varVI > -1) || (varVF > -1)) {

					int var_VMED = getVarDecisao_VMEDseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica);
					int equ_VMED = getEquLinear_VOLUME_MEDIOseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica);

					if (var_VMED == -1) {
						var_VMED = addVarDecisao_VMED(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica, 0.0, vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);
						equ_VMED = addEquLinear_VOLUME_MEDIO(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(var_VMED, equ_VMED, 1.0);
					}

					if (varVI > -1) {
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varVI, equ_VMED, -0.5);

						if (a_periodo_estudo > a_horizonte_estudo_estagio.getIteradorInicial())
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varVI, getEquLinear_VOLUME_MEDIO(a_TSS, a_idEstagio, periodo_estudo_anterior, idHidreletrica), -0.5);
					}


					if (varVF > -1)
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varVF, equ_VMED, -0.5);

				}

				if ((a_dados.getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_minimo, a_periodo_estudo, double()) > 0.0) && (getVarDecisao_VFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica) > -1))
					addVarDecisao_VF_FINF(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica, 0.0, a_dados.getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_minimo, a_periodo_estudo, double()), 0.0);

			} // if (a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_considerar_usina, bool())){

		} // for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

	}// try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisVolume(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarVariaveisAssociadasHorizonteEstudo(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdDemandaEspecial a_maiorIdDemandaEspecial, const IdUsinaEolica a_maiorIdUsinaEolica, const IdContrato a_maiorIdContrato, const IdSubmercado a_maiorIdSubmercado, const IdIntercambio a_maiorIdIntercambio, const IdRestricaoEletrica a_maiorIdRestricaoEletrica, const IdTermeletrica a_maiorIdTermeletrica, const IdRestricaoOperativaUHE a_maiorIdRestricaoOperativaUHE, const IdIntercambioHidraulico a_maiorIdIntercambioHidraulico, const IdUsinaElevatoria a_maiorIdUsinaElevatoria, const SmartEnupla<Periodo, double>a_horizonte_estudo_estagio, TempoVariaveis& a_tempoCriarVariaveis) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		SmartEnupla<IdSubmercado, SmartEnupla<int, IdUsinaNaoSimulada>> usinaNaoSimulada_total(a_dados.getMenorId(IdSubmercado()), std::vector<SmartEnupla<int, IdUsinaNaoSimulada>>(a_maiorIdSubmercado, SmartEnupla<int, IdUsinaNaoSimulada>()));

		for (IdSubmercado idSubmercado = a_dados.getMenorId(IdSubmercado()); idSubmercado <= a_maiorIdSubmercado; a_dados.vetorSubmercado.incr(idSubmercado)) {

			if (a_dados.getMaiorId(idSubmercado, IdUsinaNaoSimulada()) > IdUsinaNaoSimulada_Nenhum)
				usinaNaoSimulada_total.at(idSubmercado) = SmartEnupla<int, IdUsinaNaoSimulada>(1, a_dados.getIdObjetos(idSubmercado, IdUsinaNaoSimulada()));

		}//for (IdSubmercado idSubmercado = a_dados.getMenorId(IdSubmercado()); idSubmercado <= a_maiorIdSubmercado; a_dados.vetorSubmercado.incr(idSubmercado)) {

		auto start_clock = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> tempo;

		const double infinito = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getInfinito();

		for (Periodo a_periodo_estudo = a_horizonte_estudo_estagio.getIteradorInicial(); a_periodo_estudo <= a_horizonte_estudo_estagio.getIteradorFinal(); a_horizonte_estudo_estagio.incrementarIterador(a_periodo_estudo)) {

			// Custo Operacao Valor Presente (ZO0p) Por PeriodoEstudo
			addVarDecisao_ZO0p(a_TSS, a_idEstagio, a_periodo_estudo, 0.0, infinito, 0.0);


			// Custo Operacao (ZOp) Por PeriodoEstudo	
			addVarDecisao_ZOp(a_TSS, a_idEstagio, a_periodo_estudo, 0.0, infinito, 0.0);


			// Custo Penalidade Valor Presente (ZP0p) Por PeriodoEstudo
			addVarDecisao_ZP0p(a_TSS, a_idEstagio, a_periodo_estudo, 0.0, infinito, 0.0);


			// Custo Penalidade (ZPp) Por PeriodoEstudo
			addVarDecisao_ZPp(a_TSS, a_idEstagio, a_periodo_estudo, 0.0, infinito, 0.0);
			
			// CUSTO DE PENALIDADE (ZP) POR PeriodoEstudo (contabiliza as penalidades das variáveis que não dependem do patamar de carga, ex, restrições de volume, evaporação)
			addVarDecisao_ZP(a_TSS, a_idEstagio, a_periodo_estudo, 0.0, infinito, 0.0);

			const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, IdPatamarCarga());
			for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {


				// CUSTO OPERAÇÃO (ZO) POR PeriodoEstudo PorPatamarCarga
				addVarDecisao_ZO(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, 0.0, infinito, 0.0);


				// CUSTO DE PENALIDADE (ZP) POR PeriodoEstudo PorPatamarCarga
				addVarDecisao_ZP(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, 0.0, infinito, 0.0);

				// VARIAVEIS DE CONTRATOD CIMP/CEXP
				criarVariaveisContrato(a_TSS, a_dados, a_idEstagio, idPatamarCarga, a_periodo_estudo, a_maiorIdContrato, a_horizonte_estudo_estagio);

				// VARIAVEIS USINAS EOLICAS
				criarVariaveisEolicas(a_TSS, a_dados, a_idEstagio, idPatamarCarga, a_periodo_estudo, a_maiorIdUsinaEolica, a_horizonte_estudo_estagio);

				// VARIAVEIS DE DEMANDA ESPECIAL
				criarVariaveisDemandaEspecial(a_TSS, a_dados, a_idEstagio, idPatamarCarga, a_periodo_estudo, a_maiorIdDemandaEspecial, a_horizonte_estudo_estagio);


				// VARIAVEIS TERMELETRICAS
				start_clock = std::chrono::high_resolution_clock::now();
				criarVariaveisTermeletricas(a_TSS, a_dados, a_idEstagio, idPatamarCarga, a_periodo_estudo, a_maiorIdTermeletrica, a_horizonte_estudo_estagio);
				tempo = std::chrono::high_resolution_clock::now() - start_clock;
				a_tempoCriarVariaveis.variaveisTermeletrica += tempo.count() / 60;


				for (IdSubmercado idSubmercado = a_dados.getMenorId(IdSubmercado()); idSubmercado <= a_maiorIdSubmercado; a_dados.vetorSubmercado.incr(idSubmercado)) {

					// VARIAVEIS POÊNCIA DEFICIT (PD)
					start_clock = std::chrono::high_resolution_clock::now();
					const IdPatamarDeficit maiorIdPatamarDeficit = a_dados.getMaiorId(idSubmercado, IdPatamarDeficit());
					for (IdPatamarDeficit idPatamarDeficit = IdPatamarDeficit_1; idPatamarDeficit <= maiorIdPatamarDeficit; idPatamarDeficit++) {
						if (a_dados.getElementoMatriz(idSubmercado, idPatamarDeficit, AttMatrizPatamarDeficit_potencia_maxima, a_periodo_estudo, idPatamarCarga, double()) > 0.0) {

							addVarDecisao_PD(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, idSubmercado, idPatamarDeficit, 0.0, a_dados.getElementoMatriz(idSubmercado, idPatamarDeficit, AttMatrizPatamarDeficit_potencia_maxima, a_periodo_estudo, idPatamarCarga, double()), 0.0);
							if (getVarDecisao_PDseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idSubmercado) < 0)
								addVarDecisao_PD(a_TSS, a_idEstagio, a_periodo_estudo, idSubmercado, 0.0, infinito, 0.0);

						} // if (a_dados.getElementoMatriz(idSubmercado, idPatamarDeficit, AttMatrizPatamarDeficit_potencia_maxima, a_periodo_estudo, idPatamarCarga, double()) > 0.0) {
					} // for (IdPatamarDeficit idPatamarDeficit = IdPatamarDeficit_1; idPatamarDeficit <= maiorIdPatamarDeficit; idPatamarDeficit++) {
					tempo = std::chrono::high_resolution_clock::now() - start_clock;
					a_tempoCriarVariaveis.variaveisDeficit += tempo.count() / 60;


					// VARIAVEIS POTÊNCIA DEMANDA (PL)
					start_clock = std::chrono::high_resolution_clock::now();
					if (a_dados.getSize1Matriz(idSubmercado, AttMatrizSubmercado_demanda) > 0) {
						if (a_dados.getSize2Matriz(idSubmercado, AttMatrizSubmercado_demanda, a_periodo_estudo) > 0) {
							if (a_dados.getElementoMatriz(idSubmercado, AttMatrizSubmercado_demanda, a_periodo_estudo, idPatamarCarga, double()) > 0.0) {

								addVarDecisao_PL(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, idSubmercado, a_dados.getElementoMatriz(idSubmercado, AttMatrizSubmercado_demanda, a_periodo_estudo, idPatamarCarga, double()), a_dados.getElementoMatriz(idSubmercado, AttMatrizSubmercado_demanda, a_periodo_estudo, idPatamarCarga, double()), 0.0);

								setNormalizacaoDualVarDecisao_PL(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, idSubmercado, 1.0 / (a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, idPatamarCarga, double()) * double(a_periodo_estudo.getHoras())));

								if (getVarDecisao_PLseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idSubmercado) < 0)
									addVarDecisao_PL(a_TSS, a_idEstagio, a_periodo_estudo, idSubmercado, 0.0, infinito, 0.0);

							} // if (a_dados.getElementoMatriz(idSubmercado, AttMatrizSubmercado_demanda, a_periodo_estudo, idPatamarCarga, double()) > 0){
						} // if (a_dados.getSize2Matriz(idSubmercado, AttMatrizSubmercado_demanda, a_periodo_estudo) > 0) {
					} // if (a_dados.getSize1Matriz(idSubmercado, AttMatrizSubmercado_demanda) > 0) {
					tempo = std::chrono::high_resolution_clock::now() - start_clock;
					a_tempoCriarVariaveis.variaveisDemanda += tempo.count() / 60;


					// VARIAVEIS POTÊNCIA USINAS NÃO SIMULADAS (PN)
					start_clock = std::chrono::high_resolution_clock::now();

					for (int tipo_usina = 1; tipo_usina <= usinaNaoSimulada_total.at(idSubmercado).size(); tipo_usina++) {
						IdUsinaNaoSimulada idUsinaNaoSimulada = usinaNaoSimulada_total.at(idSubmercado).at(tipo_usina);
						addVarDecisao_PN(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, idSubmercado, idUsinaNaoSimulada, a_dados.getElementoMatriz(idSubmercado, idUsinaNaoSimulada, AttMatrizUsinaNaoSimulada_potencia_minima, a_periodo_estudo, idPatamarCarga, double()), a_dados.getElementoMatriz(idSubmercado, idUsinaNaoSimulada, AttMatrizUsinaNaoSimulada_potencia_maxima, a_periodo_estudo, idPatamarCarga, double()), 0.0);
						if (getVarDecisao_PNseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idSubmercado) < 0)
							addVarDecisao_PN(a_TSS, a_idEstagio, a_periodo_estudo, idSubmercado, 0.0, infinito, 0.0);
					}

					tempo = std::chrono::high_resolution_clock::now() - start_clock;
					a_tempoCriarVariaveis.variaveisUsinaNSimulada += tempo.count() / 60;

				} // for (IdSubmercado idSubmercado = a_dados.getMenorId(IdSubmercado()); idSubmercado <= a_maiorIdSubmercado; a_dados.vetorSubmercado.incr(idSubmercado)) {


				// Potência Intercambios (PI)
				start_clock = std::chrono::high_resolution_clock::now();
				SmartEnupla<IdSubmercado, bool> criar_variavel_PI(SmartEnupla<IdSubmercado, bool>(a_dados.getMenorId(IdSubmercado()), std::vector< bool>(a_maiorIdSubmercado, false)));

				for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= a_maiorIdIntercambio; idIntercambio++) {
					
					addVarDecisao_PI(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, idIntercambio, 0.0, a_dados.getElementoMatriz(idIntercambio, AttMatrizIntercambio_potencia_maxima, a_periodo_estudo, idPatamarCarga, double()), 0.0);
					
					if (a_dados.getElementoMatriz(idIntercambio, AttMatrizIntercambio_potencia_minima, a_periodo_estudo, idPatamarCarga, double()) > 0.0) {
						//Potência Intercambios Folga Mínima (PI_FINF)
						addVarDecisao_PI_FINF(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, idIntercambio, 0.0, infinito, 0.0);
					}//if (a_dados.getElementoMatriz(idIntercambio, AttMatrizIntercambio_potencia_minima, a_periodo_estudo, idPatamarCarga, double()) > 0.0) {
					
					if ((getVarDecisao_PIseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_origem, IdSubmercado())) < 0) || (getVarDecisao_PIseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_destino, IdSubmercado())) < 0))
						criar_variavel_PI.setElemento(a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_origem, IdSubmercado()), true);
				} // for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= a_maiorIdIntercambio; idIntercambio++) {

				for (IdSubmercado idSubmercado = a_dados.getMenorId(IdSubmercado()); idSubmercado <= a_maiorIdSubmercado; a_dados.vetorSubmercado.incr(idSubmercado)) {
					if (criar_variavel_PI.getElemento(idSubmercado))
						addVarDecisao_PI(a_TSS, a_idEstagio, a_periodo_estudo, idSubmercado, -infinito, infinito, 0.0);
				}
				tempo = std::chrono::high_resolution_clock::now() - start_clock;
				a_tempoCriarVariaveis.variaveisIntercambio += tempo.count() / 60;


				//
				// Restrição Eletrica Folgas 
				//

				start_clock = std::chrono::high_resolution_clock::now();
				for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= a_maiorIdRestricaoEletrica; idRestricaoEletrica++) {

					//RE_FINF

					if (a_dados.getElementoMatriz(idRestricaoEletrica, AttMatrizRestricaoEletrica_potencia_minima, a_periodo_estudo, idPatamarCarga, double()) > 0.0) {

						if (a_dados.getMaiorId(idRestricaoEletrica, IdElementoSistema()) != IdElementoSistema_Nenhum)
							addVarDecisao_RE_FINF(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, idRestricaoEletrica, 0.0, infinito, 0.0);

					} // if (a_dados.getElementoMatriz(idRestricaoEletrica, AttMatrizRestricaoEletrica_potencia_minima, a_periodo_estudo, idPatamarCarga, double()) > 0.0) {

					//RE_FSUP

					if (a_dados.getElementoMatriz(idRestricaoEletrica, AttMatrizRestricaoEletrica_potencia_maxima, a_periodo_estudo, idPatamarCarga, double()) >= 0.0 && \
						a_dados.getElementoMatriz(idRestricaoEletrica, AttMatrizRestricaoEletrica_potencia_maxima, a_periodo_estudo, idPatamarCarga, double()) != getdoubleFromChar("max")) {

						if (a_dados.getMaiorId(idRestricaoEletrica, IdElementoSistema()) != IdElementoSistema_Nenhum)
							addVarDecisao_RE_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, idRestricaoEletrica, 0.0, infinito, 0.0);

					}

				} // for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= a_maiorIdRestricaoEletrica; idRestricaoEletrica++) {
				tempo = std::chrono::high_resolution_clock::now() - start_clock;
				a_tempoCriarVariaveis.variaveisRestEleticaFolga += tempo.count() / 60;

			} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

		}//for (Periodo a_periodo_estudo = a_horizonte_estudo_estagio.getIteradorInicial(); a_periodo_estudo <= a_horizonte_estudo_estagio.getIteradorFinal(); a_horizonte_estudo_estagio.incrementarIterador(a_periodo_estudo)) {

	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisAssociadasHorizonteEstudo(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
}

void ModeloOtimizacao::setVolumeMeta(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const SmartEnupla<Periodo, double> &a_horizonte_estudo_estagio)
{
	try {

		const IdCenario menor_cenario = IdCenario_1;
		const IdCenario maior_cenario = arranjoResolucao.getAtributo(AttComumArranjoResolucao_maior_cenario, IdCenario());

		for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			if (a_dados.getSize1Matriz(idHidreletrica, IdReservatorio_1, AttMatrizReservatorio_volume_meta) > 0) {

				for (Periodo a_periodo_estudo = a_horizonte_estudo_estagio.getIteradorInicial(); a_periodo_estudo <= a_horizonte_estudo_estagio.getIteradorFinal(); a_horizonte_estudo_estagio.incrementarIterador(a_periodo_estudo)) {

					if (a_dados.getElementoMatriz(idHidreletrica, IdReservatorio_1, AttMatrizReservatorio_volume_meta, IdCenario_1, a_periodo_estudo, double()) != getdoubleFromChar("max")) {

						IdVariavelEstado idVariavelEstado = IdVariavelEstado_Nenhum;

						if (a_periodo_estudo == a_horizonte_estudo_estagio.getIteradorFinal()) {

							const int varDecisao_VF = getVarDecisao_VF(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica);
							idVariavelEstado = vetorEstagio.att(a_idEstagio).addVariavelEstado(a_TSS, getNomeVarDecisao_VF(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica), varDecisao_VF, -1);

						}//if (a_periodo_estudo == a_horizonte_estudo_estagio.getIteradorFinal()) {
						else {
							//Pega o VI do periodo_seguinte
							Periodo periodo_estudo_seguinte = a_periodo_estudo;
							a_horizonte_estudo_estagio.incrementarIterador(periodo_estudo_seguinte);

							const int varDecisao_VI = getVarDecisao_VI(a_TSS, a_idEstagio, periodo_estudo_seguinte, idHidreletrica);
							idVariavelEstado = vetorEstagio.att(a_idEstagio).addVariavelEstado(a_TSS, getNomeVarDecisao_VI(a_TSS, a_idEstagio, periodo_estudo_seguinte, idHidreletrica), varDecisao_VI, -1);

						}//else {

						vetorEstagio.att(a_idEstagio).addValorVariavelEstado(idVariavelEstado, menor_cenario, menor_cenario, maior_cenario, 0.0);
						for (IdCenario idCenario = menor_cenario; idCenario <= maior_cenario; idCenario++)
							vetorEstagio.att(a_idEstagio).addValorVariavelEstado(idVariavelEstado, idCenario, IdCenario_Nenhum, IdCenario_Nenhum, a_dados.getElementoMatriz(idHidreletrica, IdReservatorio_1, AttMatrizReservatorio_volume_meta, idCenario, a_periodo_estudo, double()));

					}//if (a_dados.getElementoMatriz(idHidreletrica, IdReservatorio_1, AttMatrizReservatorio_volume_meta, IdCenario_1, a_periodo_estudo, double()) != getdoubleFromChar("min")) {

				}//for (Periodo a_periodo_estudo = a_horizonte_estudo_estagio.getIteradorInicial(); a_periodo_estudo <= a_horizonte_estudo_estagio.getIteradorFinal(); a_horizonte_estudo_estagio.incrementarIterador(a_periodo_estudo))

			} // if (a_dados.getSize1Matriz(idHidreletrica, IdReservatorio_1, AttMatrizReservatorio_volume_meta) > 0) {

		}//for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= a_maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {


	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::setVolumeMeta(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }

}


void ModeloOtimizacao::criarVariaveisHidraulicas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio){

	try{

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorModeloOtimizacao_horizonte_estudo, Periodo(), IdEstagio());

		const SmartEnupla<Periodo, double> horizonte_estudo_estagio = getElementosMatriz(AttMatrizModeloOtimizacao_horizonte_estudo, a_idEstagio, Periodo(), double());

		for (Periodo a_periodo_estudo = horizonte_estudo_estagio.getIteradorInicial(); a_periodo_estudo <= horizonte_estudo_estagio.getIteradorFinal(); horizonte_estudo_estagio.incrementarIterador(a_periodo_estudo)) {

			// CRIA VARIAVEIS VOLUME
			criarVariaveisVolume(a_TSS, a_dados, a_idEstagio, a_periodo_estudo, horizonte_estudo_estagio, horizonte_estudo);

			for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, IdPatamarCarga()); idPatamarCarga++) {

				// CRIA VARIAVEIS HIDRELETRICAS
				criarVariaveisHidraulicas_porPatamar(a_TSS, a_dados, a_idEstagio, idPatamarCarga, a_periodo_estudo);

			} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, IdPatamarCarga()); idPatamarCarga++) {

		} // for (Periodo a_periodo_estudo = horizonte_estudo_estagio.getIteradorInicial(); a_periodo_estudo <= horizonte_estudo_estagio.getIteradorFinal(); horizonte_estudo_estagio.incrementarIterador(a_periodo_estudo)) {

		setVolumeMeta(a_TSS, a_dados, a_idEstagio, horizonte_estudo_estagio);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("criarVariaveisHidraulicas(" + getString(getIdObjeto()) + ")::criarVariaveisHidraulicas(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }

}

void ModeloOtimizacao::criarVariaveisHidraulicas_porPatamar(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdPatamarCarga a_idPatamarCarga, const Periodo a_periodo_estudo) {

	try {

		const double infinito = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getInfinito();

		for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			if (a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_considerar_usina, bool())) {


				if (a_idPatamarCarga == IdPatamarCarga_1) {

					if (a_dados.getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_evaporada_meta) > 0) {

						//Evaporacao_meta

						// EVAPORAÇÃO RELAXADA(QEV_REL)
						const int varQEV_REL = addVarDecisao_QEV_REL(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica, -infinito, infinito, 0.0);

						if (a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_evaporada_meta, IdCenario_1, a_periodo_estudo, double()) != getdoubleFromChar("max")) {

							const IdVariavelEstado idVariavelEstado = vetorEstagio.att(a_idEstagio).addVariavelEstado(a_TSS, getNomeVarDecisao_QEV_REL(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica), varQEV_REL, -1);

							const IdProcesso idProcesso = a_dados.arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());
							const IdCenario menor_cenario = a_dados.arranjoResolucao.getAtributo(idProcesso, AttComumProcesso_menor_cenario, IdCenario());
							const IdCenario maior_cenario = a_dados.arranjoResolucao.getAtributo(idProcesso, AttComumProcesso_maior_cenario, IdCenario());

							vetorEstagio.att(a_idEstagio).addValorVariavelEstado(idVariavelEstado, menor_cenario, menor_cenario, maior_cenario, 0.0);
							for (IdCenario idCenario = menor_cenario; idCenario <= maior_cenario; idCenario++)
								vetorEstagio.att(a_idEstagio).addValorVariavelEstado(idVariavelEstado, idCenario, IdCenario_Nenhum, IdCenario_Nenhum, a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_evaporada_meta, idCenario, a_periodo_estudo, double()));

						} // if (a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_evaporada_meta, IdCenario_1, a_periodo_estudo, double()) != getdoubleFromChar("max")) {

					} // if (a_dados.getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_evaporada_meta > 0)) {
					else {

						if ((a_dados.getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_coef_linear_evaporacao_0, a_periodo_estudo, double()) != 0) || (a_dados.getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_coef_linear_evaporacao_1, a_periodo_estudo, double()) != 0)) {

							// EVAPORAÇÃO (QEV)
							addVarDecisao_QEV(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica, -infinito, infinito, 0.0);

							// EVAPORAÇÃO RELAXADA(QEV_REL)
							const int varQEV_REL = addVarDecisao_QEV_REL(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica, -infinito, infinito, 0.0);

							// CRIA VARIAVEIS DE FOLGA APENAS PARA RETIRADA DE AGUA 
							if ((a_dados.getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_coef_linear_evaporacao_0, a_periodo_estudo, double()) > 0))
								addVarDecisao_QEV_FINF(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica, 0.0, 0.0, 0.0);

						}//if ((a_dados.getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_coef_linear_evaporacao_0, a_periodo_estudo, double()) != 0) || (a_dados.getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_coef_linear_evaporacao_1, a_periodo_estudo, double()) != 0)) {

					}//else {


					const int varQVER = addVarDecisao_QVER(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica, 0.0, infinito, 0.0);

					if (a_dados.getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_vertida_meta)) {

						if (a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_vertida_meta, IdCenario_1, a_periodo_estudo, double()) != getdoubleFromChar("max")) {

							const IdVariavelEstado idVariavelEstado = vetorEstagio.att(a_idEstagio).addVariavelEstado(a_TSS, getNomeVarDecisao_QVER(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica), varQVER, -1);

							const IdProcesso idProcesso = a_dados.arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());
							const IdCenario menor_cenario = a_dados.arranjoResolucao.getAtributo(idProcesso, AttComumProcesso_menor_cenario, IdCenario());
							const IdCenario maior_cenario = a_dados.arranjoResolucao.getAtributo(idProcesso, AttComumProcesso_maior_cenario, IdCenario());

							vetorEstagio.att(a_idEstagio).addValorVariavelEstado(idVariavelEstado, menor_cenario, menor_cenario, maior_cenario, 0.0);
							for (IdCenario idCenario = menor_cenario; idCenario <= maior_cenario; idCenario++)
								vetorEstagio.att(a_idEstagio).addValorVariavelEstado(idVariavelEstado, idCenario, IdCenario_Nenhum, IdCenario_Nenhum, a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_vertida_meta, idCenario, a_periodo_estudo, double()));

						} // if (a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_vertida_meta, IdCenario_1, a_periodo_estudo, double()) != getdoubleFromChar("max")) {

					} // if (a_dados.getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_vertida_meta)) {

					// Vazão defluente Hidreletrica (QDEF)
					addVarDecisao_QDEF(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica, 0.0, infinito, 0.0);

					// Defluencia Hidrelétrica Folga Minima (QDEF_FINF)
					if (a_dados.getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_defluente_minima) > 0.0) {
						if (a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_defluente_minima, a_periodo_estudo, double()) > 0.0)
							addVarDecisao_QDEF_FINF(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica, 0.0, infinito, 0.0);
					}

					// Defluencia Hidrelétrica Folga Maxima (QDEF_FSUP)
					if (a_dados.getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_defluente_maxima) > 0.0) {
						if ((a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_defluente_maxima, a_periodo_estudo, double()) > 0.0) && (a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_defluente_maxima, a_periodo_estudo, double()) != getdoubleFromChar("max")))
							addVarDecisao_QDEF_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica, 0.0, infinito, 0.0);
					}

				}//if (a_idPatamarCarga == IdPatamarCarga_1) {

				// Vazão Montante Hidreletrica (QMON)
				addVarDecisao_QMON(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, -infinito, infinito, 0.0);

				// Vertimento Hidreletrica (QVER)
				addVarDecisao_QVER(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, 0.0, infinito, 0.0);

				// Vazão defluente Hidreletrica por patamar (QDEF)
				addVarDecisao_QDEF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, 0.0, infinito, 0.0);

				// Defluencia Hidrelétrica Folga Minima (QDEF_FINF)
				if (a_dados.getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_defluente_minima) > 0.0) {
					if (a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_defluente_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0)
						addVarDecisao_QDEF_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, 0.0, infinito, 0.0);
				}

				// Defluencia Hidrelétrica Folga Maxima (QDEF_FSUP)
				if (a_dados.getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_defluente_maxima) > 0.0) {
					if ((a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_defluente_maxima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0) && (a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_defluente_maxima, a_periodo_estudo, a_idPatamarCarga, double()) != getdoubleFromChar("max")))
						addVarDecisao_QDEF_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, 0.0, infinito, 0.0);
				}

				if (a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica()) != IdHidreletrica_Nenhum) {

					// Vazão desviada Hidreletrica por patamar (QDES)
					addVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica()), 0.0, a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_desviada_maxima, a_periodo_estudo, a_idPatamarCarga, double()), 0.0);

					// Desvio Hidrelétrica Folga Minima (QDES_FINF)
					if (a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_desviada_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0)
						addVarDecisao_QDES_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica()), 0.0, infinito, 0.0);

				}//if (a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica()) != IdHidreletrica_Nenhum) {


				// Vazão retirada Hidreletrica por patamar (QOUT)

				// Vazão retirada Hidrelétrica Folga Minima (QOUT_FINF)
				if (a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_retirada, a_periodo_estudo, double()) > 0.0) {
					addVarDecisao_QOUT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, 0.0, a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_retirada, a_periodo_estudo, double()), 0.0);
					addVarDecisao_QOUT_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, 0.0, infinito, 0.0);
				}//if (a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_retirada, a_periodo_estudo, double()) > 0.0) {
				else
					addVarDecisao_QOUT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_retirada, a_periodo_estudo, double()), a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_retirada, a_periodo_estudo, double()), 0.0);


				const TipoDetalhamentoProducaoHidreletrica tipo_detalhamento_producao_hidreletrica = a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica());

				bool disponivel = false;
				bool todo_membro_disponivel_possui_potencia_minima = true;
				bool todo_membro_disponivel_possui_vazao_turbinada_minima = true;

				// ------------------------------------------------------------------------------------------------------------------------------------------------------
				//
				//                                                                    Produção Por Hidrelétrica
				//
				// ------------------------------------------------------------------------------------------------------------------------------------------------------

				if (tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_usina) {

					if (a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_disponibilidade, a_periodo_estudo, double()) > 0.0) {

						disponivel = true;

						if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

							//
							// Potencia Hidreletrica
							//

							// Potencia Hidreletrica (PH)
							addVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, 0.0, a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_maxima, a_periodo_estudo, a_idPatamarCarga, double()), 0.0);

							if (a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0) {

								// Potencia Hidreletrica Relaxada (PH_REL)
								addVarDecisao_PH_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()), infinito, 0.0);

								// Potencia Hidreletrica Folga Minima (PH_FINF)
								addVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, 0.0, infinito, 0.0);

							} // if (a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0) {

							else
								todo_membro_disponivel_possui_potencia_minima = false;

							addVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, 0.0, a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_disponibilidade, a_periodo_estudo, double()) * a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_maxima, a_periodo_estudo, a_idPatamarCarga, double()), 0.0);

						} // if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

						//
						// Turbinamento Hidreletrico
						//

						// Turbinamento Hidreletrico (QTUR)
						addVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, 0.0, a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_maxima, a_periodo_estudo, double()), 0.0);

						if (a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_minima, a_periodo_estudo, double()) > 0.0) {
							// Turbinamento Hidreletrico Folga Minima (QTUR_FINF)
							addVarDecisao_QTUR_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, 0.0, infinito, 0.0);
						} // if (a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_minima, a_periodo_estudo, double()) > 0.0) {

						else
							todo_membro_disponivel_possui_vazao_turbinada_minima = false;

						// Turbinamento Hidreletrico Disponivel (QTURDISP)

						addVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, 0.0, a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima, a_periodo_estudo, a_idPatamarCarga, double()), 0.0);

						if (a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0) {
							// Turbinamento Hidreletrico Folga Minima (QTURDISP_FINF)
							addVarDecisao_QTURDISP_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, 0.0, infinito, 0.0);
						} // if (a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_disponivel_minima, a_periodo_estudo, double()) > 0.0) {


					} // if (a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_disponibilidade, a_periodo_estudo, double()) > 0.0) {

				} // if (tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_usina) {


					// ------------------------------------------------------------------------------------------------------------------------------------------------------
					//
					//                                                                    Produção Por Conjunto Hidráulico
					//
					// ------------------------------------------------------------------------------------------------------------------------------------------------------

				if (tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

					for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

						if (a_dados.getElementoVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_disponibilidade, a_periodo_estudo, double()) > 0.0) {

							disponivel = true;

							if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

								//
								// Potencia ConjuntoHidraulico
								//

								// Potencia ConjuntoHidraulico (PH)
								addVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, 0.0, a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_maxima, a_periodo_estudo, a_idPatamarCarga, double()), 0.0);

								if (a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0) {

									// Potencia ConjuntoHidraulico Relaxada (PH_REL)
									addVarDecisao_PH_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()), infinito, 0.0);

									// Potencia ConjuntoHidraulico Folga Minima (PH_FINF)
									addVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, 0.0, infinito, 0.0);

								} // if (a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0) {

								else
									todo_membro_disponivel_possui_potencia_minima = false;

								// Potencia UnidadeUHE Disponivel (PHDISP)
								addVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, 0.0, a_dados.getElementoVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_disponibilidade, a_periodo_estudo, double()) * a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_maxima, a_periodo_estudo, a_idPatamarCarga, double()), 0.0);

							} // if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

							//
							// Turbinamento ConjuntoHidraulico
							//

							// Turbinamento ConjuntoHidraulico (QTUR)
							addVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, 0.0, a_dados.getElementoVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_maxima, a_periodo_estudo, double()), 0.0);

							if (a_dados.getElementoVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_minima, a_periodo_estudo, double()) > 0.0) {
								// Turbinamento ConjuntoHidraulico Folga Minima (QTUR_FINF)
								addVarDecisao_QTUR_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, 0.0, infinito, 0.0);
							} // if (a_dados.getElementoVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_minima, a_periodo_estudo, double()) > 0.0) {

							else
								todo_membro_disponivel_possui_vazao_turbinada_minima = false;

							// Turbinamento Hidreletrico Disponivel (QTURDISP)
							addVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, 0.0, infinito, 0.0);

						} // if (a_dados.getElementoVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_disponibilidade, a_periodo_estudo, double()) > 0.0) {

					} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

				} // if (tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {


				// ------------------------------------------------------------------------------------------------------------------------------------------------------
				//
				//                                                                    Produção Unidade Hidrelétrica
				//
				// ------------------------------------------------------------------------------------------------------------------------------------------------------

				else if (tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_unidade) {

					for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

						for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {

							if (a_dados.getElementoVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_disponibilidade, a_periodo_estudo, double()) > 0.0) {

								disponivel = true;

								if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

									//
									// Potencia UnidadeUHE
									//

									// Potencia UnidadeUHE (PH)
									addVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, 0.0, a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_maxima, a_periodo_estudo, a_idPatamarCarga, double()), 0.0);

									if (a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0) {

										// Potencia UnidadeUHE Relaxada (PH_REL)
										addVarDecisao_PH_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()), infinito, 0.0);

										// Potencia UnidadeUHE Folga Minima (PH_FINF)
										addVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, 0.0, infinito, 0.0);

									} // if (a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0) {

									else
										todo_membro_disponivel_possui_potencia_minima = false;

									// Potencia UnidadeUHE Disponivel (PHDISP)
									addVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, 0.0, a_dados.getElementoVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_disponibilidade, a_periodo_estudo, double()) * a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_maxima, a_periodo_estudo, a_idPatamarCarga, double()), 0.0);

									if (a_dados.getSize1Matriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_minima) > 0) {
										if ((a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0) && (getVarDecisao_PH_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) == -1)) {

											// Potencia UnidadeUHE Disponivel Relaxada (PHDISP_REL)
											addVarDecisao_PHDISP_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_minima, a_periodo_estudo, a_idPatamarCarga, double()), infinito, 0.0);

											// Potencia UnidadeUHE Disponivel Folga Minima (PHDISP_FINF)
											addVarDecisao_PHDISP_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, 0.0, infinito, 0.0);

										} // if ((a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0) && (getVarDecisao_PH_FINFseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) == -1)) {

										else if ((a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0) && (getVarDecisao_PH_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE) > -1))
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_minima, a_periodo_estudo, a_idPatamarCarga, double()));

									} // if (a_dados.getSize1Matriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_minima) > 0) {

									if (a_dados.getSize1Matriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_maxima) > 0)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE), a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_maxima, a_periodo_estudo, a_idPatamarCarga, double()));

								}

								//
								// Turbinamento UnidadeUHE
								//

								// Turbinamento UnidadeUHE (QTUR)
								addVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, 0.0, a_dados.getElementoVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_vazao_turbinada_maxima, a_periodo_estudo, double()), 0.0);

								if (a_dados.getElementoVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_vazao_turbinada_minima, a_periodo_estudo, double()) > 0.0) {
									// Turbinamento UnidadeUHE Folga Minima (QTUR_FINF)
									addVarDecisao_QTUR_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, 0.0, infinito, 0.0);
								} // if (a_dados.getElementoVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_vazao_turbinada_minima, a_periodo_estudo, double()) > 0.0) {

								else
									todo_membro_disponivel_possui_vazao_turbinada_minima = false;

								// Turbinamento Hidreletrico Disponivel (QTURDISP)
								addVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, 0.0, infinito, 0.0);

							} // if (a_dados.getElementoVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_disponibilidade, a_periodo_estudo, double()) > 0.0) {

						} // for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= maiorIdUnidadeUHE; idUnidadeUHE++) {
					} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {

				} // else if (tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_unidade) {


				if (disponivel) {

					if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

						//
						// Potencia Hidreletrica (PH)
						//

						if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) == -1) {

							addVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, 0.0, infinito, 0.0);

							if (a_dados.getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_minima) > 0) {

								if ((a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0) && (todo_membro_disponivel_possui_potencia_minima))
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()));

								else if ((a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0) && (!todo_membro_disponivel_possui_potencia_minima)) {

									// Potencia Hidreletrica Relaxada (PH_REL)
									addVarDecisao_PH_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()), infinito, 0.0);

									// Potencia Hidreletrica Folga Minima (PH_FINF)
									addVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, 0.0, infinito, 0.0);

								} // else if ((a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0) && (!todo_membro_disponivel_possui_potencia_minima)) {
							} // if (a_dados.getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_minima) > 0) {

							if (a_dados.getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_maxima) > 0)
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_maxima, a_periodo_estudo, a_idPatamarCarga, double()));

						} // if (getVarDecisao_PHseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) == -1) {

						//
						// Potencia Hidreletrica Disponivel (PHDISP)
						//

						if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) == -1)
							addVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, 0.0, infinito, 0.0);

						if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica) == -1) {

							const int varPHDISP = addVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica, 0.0, infinito, 0.0);

							if (a_dados.getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_meta) > 0) {

								if (a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_meta, IdCenario_1, a_periodo_estudo, double()) != getdoubleFromChar("max")) {

									const int varPHDISPMETA = addVarDecisao_PHDISPMETA(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica, 0.0, infinito, 0.0);
									const int varPHDISPMETA_FINF = addVarDecisao_PHDISPMETA_FINF(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica, 0.0, infinito, 0.0);
									const int varPHDISPMETA_FSUP = addVarDecisao_PHDISPMETA_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica, 0.0, infinito, 0.0);

									const IdVariavelEstado idVariavelEstado = vetorEstagio.att(a_idEstagio).addVariavelEstado(a_TSS, getNomeVarDecisao_PHDISPMETA(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica), varPHDISPMETA, -1);

									const IdProcesso idProcesso = a_dados.arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());
									const IdCenario menor_cenario = a_dados.arranjoResolucao.getAtributo(idProcesso, AttComumProcesso_menor_cenario, IdCenario());
									const IdCenario maior_cenario = a_dados.arranjoResolucao.getAtributo(idProcesso, AttComumProcesso_maior_cenario, IdCenario());

									vetorEstagio.att(a_idEstagio).addValorVariavelEstado(idVariavelEstado, menor_cenario, menor_cenario, maior_cenario, 0.0);
									for (IdCenario idCenario = menor_cenario; idCenario <= maior_cenario; idCenario++)
										vetorEstagio.att(a_idEstagio).addValorVariavelEstado(idVariavelEstado, idCenario, IdCenario_Nenhum, IdCenario_Nenhum, a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_meta, idCenario, a_periodo_estudo, double()));

									//Adiciona Restriçao PHDISP = PHDISPMETA + PHDISPMETA_FSUP - PHDISPMETA_FINF
									const int posEquPHDISP = addEquLinear_POTENCIA_DISPONIVEL_META(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica);
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquPHDISP, 0.0);

									//PHDISP
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISP, posEquPHDISP, 1.0);

									//PHDISPMETA
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISPMETA, posEquPHDISP, -1.0);

									//PHDISPMETA_FSUP
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISPMETA_FSUP, posEquPHDISP, -1.0);

									//PHDISPMETA_FINF
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPHDISPMETA_FINF, posEquPHDISP, 1.0);

								} // if (a_dados.getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_meta)) {

							} // if (a_dados.getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_meta) > 0) {
						} // if (getVarDecisao_PHDISPseExistir(a_idEstagio, a_periodo_estudo, idHidreletrica) == -1) {

						if (a_dados.getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_minima) > 0) {

							if (a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_minima, a_periodo_estudo, a_idPatamarCarga, double()) > a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()) * a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_disponibilidade, a_periodo_estudo, double())) {

								if (todo_membro_disponivel_possui_potencia_minima)
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_minima, a_periodo_estudo, a_idPatamarCarga, double()));

								else if (!todo_membro_disponivel_possui_potencia_minima) {

									// Potencia Hidreletrica Disponivel Relaxada (PHDISP_REL)
									addVarDecisao_PHDISP_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_minima, a_periodo_estudo, a_idPatamarCarga, double()), infinito, 0.0);

									// Potencia Hidreletrica Disponivel Folga Minima (PHDISP_FINF)
									addVarDecisao_PHDISP_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, 0.0, infinito, 0.0);

								} // else if ((a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0) && (!todo_membro_disponivel_possui_potencia_minima)) {
							}
						} // if (a_dados.getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_minima) > 0) {

						if (a_dados.getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_maxima) > 0) {
							if (a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_maxima, a_periodo_estudo, a_idPatamarCarga, double()) < a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_maxima, a_periodo_estudo, a_idPatamarCarga, double()) * a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_disponibilidade, a_periodo_estudo, double()))
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_maxima, a_periodo_estudo, a_idPatamarCarga, double()));
						}

					} // if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

					//
					// Turbinamento Hidreletrico
					//

					// Turbinamento Hidreletrico (QTUR)

					if (getVarDecisao_QTURseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) == -1) {

						addVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, 0.0, infinito, 0.0);

						if (a_dados.getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_minima) > 0) {

							if ((a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_minima, a_periodo_estudo, double()) > 0.0) && (todo_membro_disponivel_possui_vazao_turbinada_minima))
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_minima, a_periodo_estudo, double()));

							else if ((a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_minima, a_periodo_estudo, double()) > 0.0) && (!todo_membro_disponivel_possui_vazao_turbinada_minima)) {

								// Turbinamento Hidreletrico Folga Minima (QTUR_FINF)
								addVarDecisao_QTUR_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, 0.0, infinito, 0.0);

							} // else if ((a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_minima, a_periodo_estudo, double()) > 0.0) && (!todo_membro_disponivel_possui_vazao_turbinada_minima)) {
						} // if (a_dados.getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_minima) > 0) {

						if (a_dados.getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_maxima) > 0)
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_maxima, a_periodo_estudo, double()));

					} // if (getVarDecisao_QTURseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) == -1) {

					// Turbinamento Hidreletrico Disponivel (QTURDISP)
					if (getVarDecisao_QTURDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) == -1)
						addVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, 0.0, infinito, 0.0);

					if (getVarDecisao_QTURDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica) == -1) {

						const int varQTURDISP = addVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica, 0.0, infinito, 0.0);

						if (a_dados.getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_meta) > 0) {

							if (a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_meta, IdCenario_1, a_periodo_estudo, double()) != getdoubleFromChar("max")) {

								const IdVariavelEstado idVariavelEstado = vetorEstagio.att(a_idEstagio).addVariavelEstado(a_TSS, getNomeVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica), varQTURDISP, -1);

								const IdProcesso idProcesso = a_dados.arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());
								const IdCenario menor_cenario = a_dados.arranjoResolucao.getAtributo(idProcesso, AttComumProcesso_menor_cenario, IdCenario());
								const IdCenario maior_cenario = a_dados.arranjoResolucao.getAtributo(idProcesso, AttComumProcesso_maior_cenario, IdCenario());

								vetorEstagio.att(a_idEstagio).addValorVariavelEstado(idVariavelEstado, menor_cenario, menor_cenario, maior_cenario, 0.0);
								for (IdCenario idCenario = menor_cenario; idCenario <= maior_cenario; idCenario++)
									vetorEstagio.att(a_idEstagio).addValorVariavelEstado(idVariavelEstado, idCenario, IdCenario_Nenhum, IdCenario_Nenhum, a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_meta, idCenario, a_periodo_estudo, double()));

							} // if (a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_meta, IdCenario_1, a_periodo_estudo, double()) != getdoubleFromChar("max")) {

						} // if (a_dados.getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_meta) > 0) {

					} // if (getVarDecisao_QTURDISPseExistir(a_idEstagio, a_periodo_estudo, idHidreletrica) == -1) {

					if ((tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_conjunto) || (tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {

						for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

							if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

								//
								// Potencia ConjuntoHidraulico (PH)
								//

								if (getVarDecisao_PHseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico) == -1) {

									addVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, 0.0, infinito, 0.0);

									if (a_dados.getSize1Matriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima) > 0) {

										if ((a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0) && (todo_membro_disponivel_possui_potencia_minima))
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico), a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()));

										else if ((a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0) && (!todo_membro_disponivel_possui_potencia_minima)) {

											// Potencia ConjuntoHidraulico Relaxada (PH_REL)
											addVarDecisao_PH_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()), infinito, 0.0);

											// Potencia ConjuntoHidraulico Folga Minima (PH_FINF)
											addVarDecisao_PH_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, 0.0, infinito, 0.0);

										} // else if ((a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0) && (!todo_membro_disponivel_possui_potencia_minima)) {
									} // if (a_dados.getSize1Matriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima) > 0) {

									if (a_dados.getSize1Matriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_maxima) > 0)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PH(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico), a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_maxima, a_periodo_estudo, a_idPatamarCarga, double()));

								} // if (getVarDecisao_PHseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico) == -1) {

								//
								// Potencia ConjuntoHidraulico Disponivel (PHDISP)
								//

								if (getVarDecisao_PHDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico) == -1)
									addVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, 0.0, infinito, 0.0);

								if (a_dados.getSize1Matriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_minima) > 0) {

									if ((a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0) && (todo_membro_disponivel_possui_potencia_minima))
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico), a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_minima, a_periodo_estudo, a_idPatamarCarga, double()));

									else if ((a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0) && (!todo_membro_disponivel_possui_potencia_minima)) {

										// Potencia ConjuntoHidraulico Disponivel Relaxada (PHDISP_REL)
										addVarDecisao_PHDISP_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_minima, a_periodo_estudo, a_idPatamarCarga, double()), infinito, 0.0);

										// Potencia ConjuntoHidraulico Disponivel Folga Minima (PHDISP_FINF)
										addVarDecisao_PHDISP_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, 0.0, infinito, 0.0);

									} // else if ((a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0) && (!todo_membro_disponivel_possui_potencia_minima)) {
								} // if (a_dados.getSize1Matriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_minima) > 0) {

								if (a_dados.getSize1Matriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_maxima) > 0)
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PHDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico), a_dados.getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_maxima, a_periodo_estudo, a_idPatamarCarga, double()));

							} // if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

							//
							// Turbinamento ConjuntoHidraulico
							//

							// Turbinamento ConjuntoHidraulico (QTUR)

							if (getVarDecisao_QTURseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico) == -1) {

								addVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, 0.0, infinito, 0.0);

								if (a_dados.getSizeVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_minima) > 0) {

									if ((a_dados.getElementoVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_minima, a_periodo_estudo, double()) > 0.0) && (todo_membro_disponivel_possui_vazao_turbinada_minima))
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico), a_dados.getElementoVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_minima, a_periodo_estudo, double()));

									else if ((a_dados.getElementoVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_minima, a_periodo_estudo, double()) > 0.0) && (!todo_membro_disponivel_possui_vazao_turbinada_minima)) {

										// Turbinamento ConjuntoHidraulico Folga Minima (QTUR_FINF)
										addVarDecisao_QTUR_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, 0.0, infinito, 0.0);

									} // else if ((a_dados.getElementoVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_minima, a_periodo_estudo, double()) > 0.0) && (!todo_membro_disponivel_possui_vazao_turbinada_minima)) {
								} // if (a_dados.getSizeVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_minima) > 0) {

								if (a_dados.getSizeVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_maxima) > 0)
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico), a_dados.getElementoVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_maxima, a_periodo_estudo, double()));

							} // if (getVarDecisao_QTURseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico) == -1) {

							// Turbinamento ConjuntoHidraulico Disponivel (QTURDISP)
							if (getVarDecisao_QTURDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico) == -1)
								addVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, idConjuntoHidraulico, 0.0, infinito, 0.0);

						} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

					} // if ((tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_conjunto) || (tipo_detalhamento_producao_hidreletrica == TipoDetalhamentoProducaoHidreletrica_por_unidade)) {

				} // if (disponivel) {

			} // if (a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_considerar_usina, bool())){
		} // for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {


		// Restrição Operativa UHE Folgas (RHX_FSUP/RHX_FINF)
		for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= a_dados.getMaiorId(IdRestricaoOperativaUHE()); idRestricaoOperativaUHE++) {


			// Vazão Afluente
			//
			if (a_idPatamarCarga == IdPatamarCarga_1 && a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_vazao_afluente) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_periodo_estudo, a_idPatamarCarga, double()) != getdoubleFromChar("min")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum)
						addVarDecisao_RHA_FINF(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE, 0.0, infinito, 0.0);

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_periodo_estudo, a_idPatamarCarga, double()) != getdoubleFromChar("min")) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_periodo_estudo, a_idPatamarCarga, double()) != getdoubleFromChar("max")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum)
						addVarDecisao_RHA_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE, 0.0, infinito, 0.0);

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_periodo_estudo, a_idPatamarCarga, double()) != getdoubleFromChar("max")) {

			}//if (a_idPatamarCarga == IdPatamarCarga_1 && a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_vazao_afluente) {						


			// Vazão Defluente
			//
			if (a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_vazao_defluente) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_periodo_estudo, a_idPatamarCarga, double()) != getdoubleFromChar("min")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum)
						addVarDecisao_RHQ_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoOperativaUHE, 0.0, infinito, 0.0);

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_periodo_estudo, a_idPatamarCarga, double()) != 0.0) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_periodo_estudo, a_idPatamarCarga, double()) != getdoubleFromChar("max")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum)
						addVarDecisao_RHQ_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoOperativaUHE, 0.0, infinito, 0.0);

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_periodo_estudo, a_idPatamarCarga, double()) != getdoubleFromChar("max")) {

			}//if (a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_vazao_afluente) {						


			// Volume armazenado
			//
			if (a_idPatamarCarga == IdPatamarCarga_1 && a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_volume_armazenado) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_periodo_estudo, a_idPatamarCarga, double()) != getdoubleFromChar("min")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum)
						addVarDecisao_RHV_FINF(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE, 0.0, infinito, 0.0);

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_periodo_estudo, a_idPatamarCarga, double()) != getdoubleFromChar("min")) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_periodo_estudo, a_idPatamarCarga, double()) != getdoubleFromChar("max")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum)
						addVarDecisao_RHV_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE, 0.0, infinito, 0.0);

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_periodo_estudo, a_idPatamarCarga, double()) != getdoubleFromChar("max")) {

			}//if (a_idPatamarCarga == IdPatamarCarga_1 && a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_volume_armazenado) {						

			// Energia armazenada
			//
			if (a_idPatamarCarga == IdPatamarCarga_1 && a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_energia_armazenada) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_periodo_estudo, a_idPatamarCarga, double()) != getdoubleFromChar("min")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum && a_dados.getElementoVetor(idRestricaoOperativaUHE, AttVetorRestricaoOperativaUHE_limite_superior_folga_inferior, a_periodo_estudo, double()) > 0.0)
						addVarDecisao_RHE_FINF(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE, 0.0, infinito, 0.0);

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_periodo_estudo, a_idPatamarCarga, double()) != getdoubleFromChar("min")) {

				if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_periodo_estudo, a_idPatamarCarga, double()) != getdoubleFromChar("max")) {

					if (a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema()) != IdElementoSistema_Nenhum)
						addVarDecisao_RHE_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE, 0.0, infinito, 0.0);

				}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_periodo_estudo, a_idPatamarCarga, double()) != getdoubleFromChar("max")) {

			}//if (a_idPatamarCarga == IdPatamarCarga_1 && a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()) == TipoRestricaoHidraulica_energia_armazenad) {		


		} // for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= a_maiorIdRestricaoOperativaUHE; idRestricaoOperativaUHE++) {

		//Variáveis associadas aos intercambios hidráulicos
		for (IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico_1; idIntercambioHidraulico <= a_dados.getMaiorId(IdIntercambioHidraulico()); idIntercambioHidraulico++) {

			const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

			if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio)
				addVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico, a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica()), a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo, a_periodo_estudo, double()), a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo, a_periodo_estudo, double()), 0.0);

			else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada) {

				addVarDecisao_QOUT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico, a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo, a_periodo_estudo, double()), a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo, a_periodo_estudo, double()), 0.0);

				if (a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo, a_periodo_estudo, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo, a_periodo_estudo, double()) > 0) {

					addVarDecisao_QOUT_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico, a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), 0.0, infinito, 0.0);
					addVarDecisao_QOUT_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico, a_dados.getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), 0.0, a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo, a_periodo_estudo, double()), 0.0);

				}//if(a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo, periodo_estudo, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo, periodo_estudo, double()) > 0)

			}//else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada) {

			else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_pereira_barreto) {

				addVarDecisao_QILS_TRI(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico, a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo, a_periodo_estudo, double()), a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo, a_periodo_estudo, double()), 0.0);

				if (a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo, a_periodo_estudo, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo, a_periodo_estudo, double()) > 0) {

					addVarDecisao_QILS_TRI_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico, 0.0, infinito, 0.0);
					addVarDecisao_QILS_TRI_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico, 0.0, a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo, a_periodo_estudo, double()), 0.0);

				}//if(a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo, periodo_estudo, double()) > 0 && a_dados.getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo, periodo_estudo, double()) > 0)

			}//else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada) {

		}//for (IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico_1; idIntercambioHidraulico <= a_maiorIdIntercambioHidraulico; idIntercambioHidraulico++) {

		//Variáveis para bombeamento QBOM

		for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria <= a_dados.getMaiorId(IdUsinaElevatoria()); idUsinaElevatoria++) {

			if (a_dados.getElementoVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_fator_disponibilidade, a_periodo_estudo, double()) > 0.0) {

				if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

					addVarDecisao_PBOMDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaElevatoria, 0.0, infinito, 0.0);

					if (getVarDecisao_PBOMDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idUsinaElevatoria) == -1)
						addVarDecisao_PBOMDISP(a_TSS, a_idEstagio, a_periodo_estudo, idUsinaElevatoria, 0.0, infinito, 0.0);

				} // if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {

				// QBOM
				addVarDecisao_QBOM(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaElevatoria, 0.0, a_dados.getElementoVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_vazao_bombeada_maxima, a_periodo_estudo, double()), 0.0);

				// QBOM_FINF
				if (a_dados.getElementoVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_vazao_bombeada_minima, a_periodo_estudo, double()) > 0.0)
					addVarDecisao_QBOM_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaElevatoria, 0.0, infinito, 0.0);

				// QBOMDISP
				addVarDecisao_QBOMDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaElevatoria, 0.0, a_dados.getElementoMatriz(idUsinaElevatoria, AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_maxima, a_periodo_estudo, a_idPatamarCarga, double()), 0.0);

				// QBOMDISP_FINF
				if (a_dados.getElementoMatriz(idUsinaElevatoria, AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0)
					addVarDecisao_QBOMDISP_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaElevatoria, 0.0, infinito, 0.0);

			} // if (a_dados.getElementoVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_fator_disponibilidade, a_periodo_estudo, double()) > 0.0) {
		} // for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria <= a_maiorIdUsinaElevatoria; idUsinaElevatoria++) {

	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisHidraulicas_porPatamar(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPatamarCarga) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarVariaveisDemandaEspecial(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdPatamarCarga a_idPatamarCarga, const Periodo a_periodo_estudo, const IdDemandaEspecial a_maiorIdDemandaEspecial, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio) {
	try {

		for (IdDemandaEspecial idDemandaEspecial = IdDemandaEspecial_1; idDemandaEspecial <= a_maiorIdDemandaEspecial; idDemandaEspecial++)
			addVarDecisao_DE(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idDemandaEspecial, a_dados.getElementoMatriz(idDemandaEspecial, AttMatrizDemandaEspecial_demanda, a_periodo_estudo, a_idPatamarCarga, double()), a_dados.getElementoMatriz(idDemandaEspecial, AttMatrizDemandaEspecial_demanda, a_periodo_estudo, a_idPatamarCarga, double()), 0.0);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisDemandaEspecials(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPatamarCarga) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }
}

void ModeloOtimizacao::criarVariaveisContrato(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdPatamarCarga a_idPatamarCarga, const Periodo a_periodo_estudo, const IdContrato a_maiorIdContrato, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio) {
	try {

		for (IdContrato idContrato = IdContrato_1; idContrato <= a_maiorIdContrato; idContrato++) {

			if (a_dados.getAtributo(idContrato, AttComumContrato_tipo_contrato, TipoContrato()) == TipoContrato_exportacao)
				addVarDecisao_CEXP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idContrato, a_dados.getElementoMatriz(idContrato, AttMatrizContrato_energia_minima, a_periodo_estudo, a_idPatamarCarga, double()), a_dados.getElementoMatriz(idContrato, AttMatrizContrato_energia_maxima, a_periodo_estudo, a_idPatamarCarga, double()), 0.0);

			else if (a_dados.getAtributo(idContrato, AttComumContrato_tipo_contrato, TipoContrato()) == TipoContrato_importacao)
				addVarDecisao_CIMP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idContrato, a_dados.getElementoMatriz(idContrato, AttMatrizContrato_energia_minima, a_periodo_estudo, a_idPatamarCarga, double()), a_dados.getElementoMatriz(idContrato, AttMatrizContrato_energia_maxima, a_periodo_estudo, a_idPatamarCarga, double()), 0.0);

			else
				throw std::invalid_argument("Tipo de contrato não identificado");

		}

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisContratos(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPatamarCarga) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }
}

void ModeloOtimizacao::criarVariaveisEolicas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdPatamarCarga a_idPatamarCarga, const Periodo a_periodo_estudo, const IdUsinaEolica a_maiorIdUsinaEolica, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio) {
	try {

		for (IdUsinaEolica idUsinaEolica = IdUsinaEolica_1; idUsinaEolica <= a_maiorIdUsinaEolica; idUsinaEolica++) {

			if (a_dados.getAtributo(idUsinaEolica, AttComumUsinaEolica_constrained_off, bool()))
				addVarDecisao_PE(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaEolica, 0.0, a_dados.getElementoMatriz(idUsinaEolica, AttMatrizUsinaEolica_geracao, a_periodo_estudo, a_idPatamarCarga, double()), 0.0);
			else
				addVarDecisao_PE(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaEolica, a_dados.getElementoMatriz(idUsinaEolica, AttMatrizUsinaEolica_geracao, a_periodo_estudo, a_idPatamarCarga, double()), a_dados.getElementoMatriz(idUsinaEolica, AttMatrizUsinaEolica_geracao, a_periodo_estudo, a_idPatamarCarga, double()), 0.0);
		}

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisEolicas(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPatamarCarga) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarVariaveisTermeletricas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdPatamarCarga a_idPatamarCarga, const Periodo a_periodo_estudo, const IdTermeletrica a_maiorIdTermeletrica, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio) {
	try {
		const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());
		const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
		const SmartEnupla<Periodo, double> horizonte_estudo_estagio_inicial = a_dados.getElementosMatriz(AttMatrizDados_percentual_duracao_horizonte_estudo, estagio_inicial, Periodo(), double());

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_estudo_inicial = getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_estudo, estagio_inicial, Periodo());
		const Periodo periodo_estudo_final = getIterador2Final(AttMatrizModeloOtimizacao_horizonte_estudo, estagio_final, Periodo());

		const double infinito = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getInfinito();

		for (IdTermeletrica idTermeletrica = a_dados.getMenorId(IdTermeletrica()); idTermeletrica <= a_maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idTermeletrica)) {

			if (a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_considerar_usina, bool())) {

				const TipoDetalhamentoProducaoTermeletrica tipo_detalhamento_producao = a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica());

				bool disponivel = false;

				// ------------------------------------------------------------------------------------------------------------------------------------------------------
				//
			//                                                                    Produção Por Termeletrica
			//
			// ------------------------------------------------------------------------------------------------------------------------------------------------------

				if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina) {

					if (a_dados.getElementoVetor(idTermeletrica, AttVetorTermeletrica_disponibilidade, a_periodo_estudo, double()) > 0.0) {

						disponivel = true;

						//POTÊNCIA DAS TERMELÉTRICAS
						addVarDecisao_PT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, 0.0, a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()) + a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_util, a_periodo_estudo, a_idPatamarCarga, double()), 0.0);
						addVarDecisao_PT_UTIL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, 0.0, a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_util, a_periodo_estudo, a_idPatamarCarga, double()), 0.0);
						addVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, 0.0, infinito, 0.0);

						if (a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0)
							addVarDecisao_LIG_T(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, 1.0, 1.0, 0.0);


						// Restrição Comando GNL

						const int lag_mensal_potencia_disponivel_comandada = a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada, int());

						if (lag_mensal_potencia_disponivel_comandada > 0) {

							const int varPTDISPCOM_pat = criarVariaveisDecisao_VariaveisEstado_Restricoes_PTDISPCOM(a_TSS, a_dados, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_minima, a_periodo_estudo, a_idPatamarCarga, double()), a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_maxima, a_periodo_estudo, a_idPatamarCarga, double()));

							const int varPTDISPCOM_FINF_pat = addVarDecisao_PTDISPCOM_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, 0.0, vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);
							const int varPTDISPCOM_FSUP_pat = addVarDecisao_PTDISPCOM_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, 0.0, vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);

						} // if (lag_mensal_potencia_disponivel_comandada > 0) {

					} // if (a_dados.getElementoVetor(idTermeletrica, AttVetorTermeletrica_disponibilidade, a_periodo_estudo, double()) > 0.0) {

				} // if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_usina) {

				// ------------------------------------------------------------------------------------------------------------------------------------------------------
				//
				//                                                                    Produção Por Unidade Termeletrica
				//
				// ------------------------------------------------------------------------------------------------------------------------------------------------------

				else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {

					for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= a_dados.getMaiorId(idTermeletrica, IdUnidadeUTE()); idUnidadeUTE++) {

						//if (a_dados.getElementoVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_disponibilidade, a_periodo_estudo, double()) > 0.0) {
						if (true) {
							disponivel = true;

							// REPRESENTAÇÃO DISCRETA 
							if (a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_representacao_discreta_producao, bool())) {

								//VARIAVEIS DO ESTADO INICIAL DO PROBLEMA
								if (a_periodo_estudo == a_horizonte_estudo_estagio.getIteradorInicial()) {

									Periodo periodo_estudo_anterior = a_periodo_estudo;
									a_horizonte_estudo_estagio.decrementarIterador(periodo_estudo_anterior);

									addVarDecisao_LIG_T(a_TSS, a_idEstagio, periodo_estudo_anterior, a_idPatamarCarga, idTermeletrica, idUnidadeUTE, 1.0, 1.0, 0.0);
									//addVarDecisao_PT_UTIL(a_idEstagio, periodo_estudo_anterior, a_idPatamarCarga, idTermeletrica, idUnidadeUTE, 0.0, infinito, 0.0);
									//addVarDecisao_ACIO_T(a_idEstagio, periodo_estudo_anterior, a_idPatamarCarga, idTermeletrica, idUnidadeUTE, 0.0, 1.0, 0.0);

								}//if (a_periodo_estudo == a_horizonte_estudo_estagio.getIteradorInicial()) {

								addVarDecisao_DESL_T(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE, 0.0, 1.0, 0.0);
								addVarDecisao_ACIO_T(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE, 0.0, 1.0, 0.0);

								addVarDecisao_PT_TRAJ_ACIO(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE, 0.0, a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()), 0.0);
								addVarDecisao_PT_TRAJ_DESL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE, 0.0, a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()), 0.0);

							}//if (a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_representacao_discreta_producao,  bool())) {


							// Potencia Por UnidadeUTE
							//
							addVarDecisao_PT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE, 0.0, a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima, a_periodo_estudo, a_idPatamarCarga, double()), 0.0);
							addVarDecisao_PT_UTIL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE, 0.0, a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_util, a_periodo_estudo, a_idPatamarCarga, double()), 0.0);
							addVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE, 0.0, infinito, 0.0);
							addVarDecisao_LIG_T(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE, 1.0, 1.0, 0.0);


							//
							// Potencia Disponivel Por UnidadeUTE
							//

							if (a_dados.getSize1Matriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_maxima) > 0)
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, idUnidadeUTE), a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_maxima, a_periodo_estudo, a_idPatamarCarga, double()));

						} // if (a_dados.getElementoVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_disponibilidade, a_periodo_estudo, double()) > 0.0) {

					} // for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= a_dados.getMaiorId(idTermeletrica, IdUnidadeUTE()); idUnidadeUTE++){

				} // else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {

				//
				// Potencia Disponivel Termeletrica
				//

				if (disponivel) {

					//if ((a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada, int()) > 0) && (getVarDecisao_PTCOM_FSUPseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) == -1)) {
						//addVarDecisao_PTCOM_FSUP(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, 0.0, infinito, 0.0);
					//	addVarDecisao_PTCOM_FINF(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, 0.0, infinito, 0.0);
					//}


					if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica) == -1)
						addVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, 0.0, infinito, 0.0);


					if (getVarDecisao_PTDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idTermeletrica) == -1) {

						const int varPTDISP = addVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, idTermeletrica, 0.0, infinito, 0.0);

						if (a_dados.getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_meta) > 0) {

							if (a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_meta, IdCenario_1, a_periodo_estudo, double()) != getdoubleFromChar("max")) {

								const IdVariavelEstado idVariavelEstado = vetorEstagio.att(a_idEstagio).addVariavelEstado(a_TSS, getNomeVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, idTermeletrica), varPTDISP, -1);

								const IdProcesso idProcesso = a_dados.arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());
								const IdCenario menor_cenario = a_dados.arranjoResolucao.getAtributo(idProcesso, AttComumProcesso_menor_cenario, IdCenario());
								const IdCenario maior_cenario = a_dados.arranjoResolucao.getAtributo(idProcesso, AttComumProcesso_maior_cenario, IdCenario());

								vetorEstagio.att(a_idEstagio).addValorVariavelEstado(idVariavelEstado, menor_cenario, menor_cenario, maior_cenario, 0.0);
								for (IdCenario idCenario = menor_cenario; idCenario <= maior_cenario; idCenario++)
									vetorEstagio.att(a_idEstagio).addValorVariavelEstado(idVariavelEstado, idCenario, IdCenario_Nenhum, IdCenario_Nenhum, a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_meta, idCenario, a_periodo_estudo, double()));

							} // if (a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_meta, IdCenario_1, a_periodo_estudo, double()) != getdoubleFromChar("max")) {

						} // if (a_dados.getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_meta) > 0) {
					} // if (getVarDecisao_PTDISPseExistir(a_idEstagio, a_periodo_estudo, idTermeletrica) == -1) {


					if (tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_por_unidade) {
						if (a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0)
							addVarDecisao_PTDISP_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, 0.0, infinito, 0.0);
						if (a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_maxima, a_periodo_estudo, a_idPatamarCarga, double()) > 0.0)
							addVarDecisao_PTDISP_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, 0.0, infinito, 0.0);
					}

					else {

						double potencia_disponivel_minima = -1.0;
						double potencia_disponivel_maxima = -1.0;

						if (a_dados.getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_minima) != 0)
							if (a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_minima, a_periodo_estudo, a_idPatamarCarga, double()) > a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()) * a_dados.getElementoVetor(idTermeletrica, AttVetorTermeletrica_disponibilidade, a_periodo_estudo, double()))
								potencia_disponivel_minima = a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_minima, a_periodo_estudo, a_idPatamarCarga, double());

						if (a_dados.getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_maxima) != 0)
							if (a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_maxima, a_periodo_estudo, a_idPatamarCarga, double()) < (a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double()) + a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_util, a_periodo_estudo, a_idPatamarCarga, double())) * a_dados.getElementoVetor(idTermeletrica, AttVetorTermeletrica_disponibilidade, a_periodo_estudo, double()))
								potencia_disponivel_maxima = a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_maxima, a_periodo_estudo, a_idPatamarCarga, double());

						if (potencia_disponivel_minima > -1.0)
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_minima, a_periodo_estudo, a_idPatamarCarga, double()));

						if (potencia_disponivel_maxima > -1.0)
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PTDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica), a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_maxima, a_periodo_estudo, a_idPatamarCarga, double()));

						const double potencia_minima = a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_minima, a_periodo_estudo, a_idPatamarCarga, double());

						if ((potencia_minima > 0.0) || (potencia_disponivel_minima > -1.0)) {

							addVarDecisao_PT_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, potencia_minima, infinito, 0.0);

							addVarDecisao_PT_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idTermeletrica, 0.0, potencia_minima, 0.0);

						} // if (potencia_minima > 0.0) {
					}

				} // if (disponivel) {

			} // if (a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_considerar_usina, bool())) {
		} // for (IdTermeletrica idTermeletrica = a_dados.getMenorId(IdTermeletrica()); idTermeletrica <= maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idTermeletrica)) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisTermeletricas(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idPatamarCarga) + "," + getFullString(a_periodo_estudo) + "): \n" + std::string(erro.what())); }
}

int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_ZP0_VF_FINF(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_penalizacao) {

	try {

		const IdEstagio menor_estagio = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());

		if (a_idEstagio < menor_estagio) {
			if (menor_estagio == IdEstagio_1)
				return -1;
			else
				return -2;
		}

		if ((!vetorEstagio.att(a_idEstagio).isSolverInstanciado(a_TSS)) || (a_TSS == TipoSubproblemaSolver_mestre))
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
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varZPO_VF_FINF, getEquLinear_CUSTO_PENALIDADE_0(a_TSS, a_idEstagio, periodo_otimizacao), -1.0);

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

				for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); a_dados.vetorHidreletrica.incr(idHidreletrica)) {
					if (getVarDecisao_VF_FINFseExistir(a_TSS, a_idEstagio, periodo, idHidreletrica) > -1) {

						if (getVarDecisao_ZP0_VF_FINFseExistir(a_TSS, a_idEstagio, periodo, a_periodo_penalizacao) == -1) {

							if (getEquLinear_CUSTO_PENALIDADE_VOLUME_INFERIOR_0seExistir(a_TSS, a_idEstagio, a_periodo_penalizacao) == -1) {
								addEquLinear_CUSTO_PENALIDADE_VOLUME_INFERIOR_0(a_TSS, a_idEstagio, a_periodo_penalizacao);

								varZP0_VF_FINF = addVarDecisao_ZP0_VF_FINF(a_TSS, a_idEstagio, a_periodo_penalizacao, 0.0, vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);

								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varZP0_VF_FINF, getEquLinear_CUSTO_PENALIDADE_VOLUME_INFERIOR_0(a_TSS, a_idEstagio, a_periodo_penalizacao), 1.0);

								if (varZP0_VF_FINF_ant != -1) {
									const int varZP0_VF_FINF_ADD = addVarDecisao_ZP0_VF_FINF_ADD(a_TSS, a_idEstagio, a_periodo_penalizacao, 0.0, 0.0, 0.0);
									vetorEstagio.att(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeVarDecisao_ZP0_VF_FINF(a_TSS, a_idEstagio, a_periodo_penalizacao)), varZP0_VF_FINF_ADD, varZP0_VF_FINF_ant);

									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varZP0_VF_FINF_ADD, getEquLinear_CUSTO_PENALIDADE_VOLUME_INFERIOR_0(a_TSS, a_idEstagio, a_periodo_penalizacao), -1.0);
								}

							} // if (getEquLinear_CUSTO_PENALIDADE_VOLUME_INFERIOR_0seExistir(a_TSS, a_idEstagio, a_periodo_penalizacao) == -1) {

							const int varZP0_VF_FINF_per = addVarDecisao_ZP0_VF_FINF(a_TSS, a_idEstagio, periodo, a_periodo_penalizacao, 0.0, vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);
							const int equZP0_VF_FINF_per = addEquLinear_CUSTO_PENALIDADE_VOLUME_INFERIOR_0(a_TSS, a_idEstagio, periodo, a_periodo_penalizacao);

							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varZP0_VF_FINF_per, equZP0_VF_FINF_per, 1.0);

							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varZP0_VF_FINF_per, getEquLinear_CUSTO_PENALIDADE_VOLUME_INFERIOR_0(a_TSS, a_idEstagio, a_periodo_penalizacao), -1.0);

						} // if (getVarDecisao_ZP0_VF_FINFseExistir(a_TSS, a_idEstagio, periodo, a_periodo_penalizacao) == -1) {

						const double valor_penalidade = a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_volume_util_minimo, double()) * a_dados.getElementoMatriz(AttMatrizDados_desagio_acumulado_horizonte_estudo, a_idEstagio, periodo, double());
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF_FINF(a_TSS, a_idEstagio, periodo, idHidreletrica), getEquLinear_CUSTO_PENALIDADE_VOLUME_INFERIOR_0(a_TSS, a_idEstagio, periodo, a_periodo_penalizacao), -valor_penalidade);

					} // if (getVarDecisao_VF_FINFseExistir(a_TSS, a_idEstagio, periodo, idHidreletrica) > -1) {
				} // for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			} // if (percentual_inclusao_periodo > 0.0) {

		} // for (Periodo periodo = periodo_inicial_horizonte_estudo; periodo <= periodo_final_horizonte_estudo; horizonte_estudo.incrementarIterador(periodo)) {

		return varZP0_VF_FINF;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisDecisao_VariaveisEstado_Restricoes_ZP0_VF_FINF(a_dados," + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getString(a_periodo_penalizacao) + "): \n" + std::string(erro.what())); }

} // int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_ZP0_VF_FINF(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo) {

int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_QDEFLAG(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo, const IdHidreletrica a_idHidreletrica, const Periodo a_periodo_lag) {

	try {

		const IdEstagio menor_estagio = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());

		if (a_idEstagio < menor_estagio)
			return -1;

		if ((!vetorEstagio.att(a_idEstagio).isSolverInstanciado(a_TSS)) || (a_TSS == TipoSubproblemaSolver_mestre))
			return -1;

		const Periodo periodo_otimizacao = getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

		const Periodo periodo_final_horizonte_estudo = getIterador2Final(AttMatrizModeloOtimizacao_horizonte_estudo, a_idEstagio, Periodo());

		// Verifica se periodo não é mais necessario para compor ou repassar lag
		if (periodo_final_horizonte_estudo + 1 <= a_periodo_lag)
			return -1;

		const SmartEnupla<Periodo, double> horizonte_estudo = getElementosMatriz(AttMatrizModeloOtimizacao_horizonte_estudo, a_idEstagio, Periodo(), double());
		const Periodo periodo_inicial_horizonte_estudo = horizonte_estudo.getIteradorInicial();


		int varQDEFLAG = getVarDecisao_QDEFLAGseExistir(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idHidreletrica);
		if (varQDEFLAG > -1)
			return varQDEFLAG;

		const double infinito = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getInfinito();

		varQDEFLAG = addVarDecisao_QDEFLAG(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idHidreletrica,  0.0, infinito, 0.0);

		// Composicao de lag com periodos do horizonte de estudo
		int equQDEFLAG = -1;
		const double sobreposicao_periodo_otimizacao = periodo_otimizacao.sobreposicao(a_periodo_lag);
		if (sobreposicao_periodo_otimizacao > 0.0) {

			equQDEFLAG = addEquLinear_VAZAO_DEFLUENTE_LAG(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idHidreletrica);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDEFLAG, equQDEFLAG, 1.0);

			bool sobreposicao_encontrada = false;
			for (Periodo periodo = periodo_inicial_horizonte_estudo; periodo <= periodo_final_horizonte_estudo; horizonte_estudo.incrementarIterador(periodo)) {

				const double sobreposicao = a_periodo_lag.sobreposicao(periodo);

				if (sobreposicao > 0.0) {
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, periodo, a_idHidreletrica), equQDEFLAG, -sobreposicao);
					if (!sobreposicao_encontrada)
						sobreposicao_encontrada = true;
				}
				else if ((sobreposicao == 0.0) && (sobreposicao_encontrada))
					break;

			} // for (Periodo periodo = periodo_inicial_horizonte_estudo; periodo <= periodo_final_horizonte_estudo; horizonte_estudo.incrementarIterador(periodo)) {
		} // if (sobreposicao_periodo_otimizacao > 0.0) {

		// No estagio 1, variáveis de estado são criadas para as variáveis QDEF pré-estudo.
		if (a_idEstagio == IdEstagio_1) {

			const SmartEnupla<Periodo, double> vazao_defluencia = a_dados.getVetor(a_idHidreletrica, IdDefluencia_passada, AttVetorDefluencia_vazao_defluencia, Periodo(), double());

			if (vazao_defluencia.getIteradorInicial() > a_periodo_lag)
				throw std::invalid_argument("Necessario mais periodos de defluencia passada anteriores a " + getString(a_periodo_lag) + " em " + getFullString(AttVetorDefluencia_vazao_defluencia) + " de " + getFullString(a_idHidreletrica));

			bool sobreposicao_encontrada = false;
			for (Periodo periodo = vazao_defluencia.getIteradorInicial(); periodo <= vazao_defluencia.getIteradorFinal(); vazao_defluencia.incrementarIterador(periodo)) {

				const double sobreposicao = a_periodo_lag.sobreposicao(periodo);

				if (sobreposicao > 0.0) {

					int varQDEF = getVarDecisao_QDEFseExistir(a_TSS, a_idEstagio, periodo, a_idHidreletrica);

					if (varQDEF == -1)
						varQDEF = addVarDecisao_QDEF(a_TSS, a_idEstagio, periodo, a_idHidreletrica, vazao_defluencia.at(periodo), vazao_defluencia.at(periodo), 0.0);

					int equQDEFLAG = getEquLinear_VAZAO_DEFLUENTE_LAGseExistir(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idHidreletrica);

					if (equQDEFLAG == -1) {
						equQDEFLAG = addEquLinear_VAZAO_DEFLUENTE_LAG(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idHidreletrica);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDEFLAG, equQDEFLAG, 1.0);
					}

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDEF, equQDEFLAG, -sobreposicao);

					if (!sobreposicao_encontrada)
						sobreposicao_encontrada = true;

				} // if (sobreposicao > 0.0) {
				else if ((sobreposicao == 0.0) && (sobreposicao_encontrada))
					break;

			} // for (Periodo periodo = vazao_defluencia.getIteradorInicial(); periodo <= vazao_defluencia.getIteradorFinal(); vazao_defluencia.incrementarIterador(periodo)) {
		} // if (a_idEstagio == IdEstagio_1) {

		else if (a_idEstagio > IdEstagio_1) {

			const IdEstagio idEstagio_anterior = IdEstagio(a_idEstagio - 1);

			int varQDEFLAG_anterior = -1;

			if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica) {
				for (TipoSubproblemaSolver tss = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tss < TipoSubproblemaSolver_Excedente; tss++) {
					if (tss != TipoSubproblemaSolver_viabilidade_hidraulica)
						varQDEFLAG_anterior = criarVariaveisDecisao_VariaveisEstado_Restricoes_QDEFLAG(tss, a_dados, idEstagio_anterior, a_periodo, a_idHidreletrica, a_periodo_lag);
					if (varQDEFLAG_anterior > -1)
						break;
				}
			}
			else
				varQDEFLAG_anterior = criarVariaveisDecisao_VariaveisEstado_Restricoes_QDEFLAG(a_TSS, a_dados, idEstagio_anterior, a_periodo, a_idHidreletrica, a_periodo_lag);

			if ((varQDEFLAG_anterior > -1) || ((idEstagio_anterior == menor_estagio) && (periodo_inicial_horizonte_estudo > a_periodo_lag))) {

				if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
					varQDEFLAG_anterior = -1;

				// Variáveis de estado a repassar lag
				if (sobreposicao_periodo_otimizacao == 0.0)
					vetorEstagio.att(a_idEstagio).addVariavelEstado(a_TSS, getNomeVarDecisao_QDEFLAG(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idHidreletrica), varQDEFLAG, varQDEFLAG_anterior);

				// Variáveis de estado a compor lag
				else if (sobreposicao_periodo_otimizacao > 0.0) {
					const int varQDEFLAG_ADD = addVarDecisao_QDEFLAG_ADD(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idHidreletrica, 0.0, infinito, 0.0);
					vetorEstagio.att(a_idEstagio).addVariavelEstado(a_TSS, getNomeVarDecisao_QDEFLAG(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idHidreletrica), varQDEFLAG_ADD, varQDEFLAG_anterior);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varQDEFLAG_ADD, equQDEFLAG, -1.0);
				}

			} // if ((varQDEFLAG_anterior > -1) || ((idEstagio_anterior == menor_estagio) && (periodo_inicial_horizonte_estudo > a_periodo_lag))) {
		} // else if (a_idEstagio > IdEstagio_1) {
		
		return varQDEFLAG;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisDecisao_VariaveisEstado_Restricoes_QDEFLAG(a_dados," + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getString(a_periodo) + "," + getFullString(a_idHidreletrica) + "," + getString(a_periodo_lag) + "): \n" + std::string(erro.what())); }

} // int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_QDEFLAG(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo, const IdHidreletrica a_idHidreletrica) {

int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_YP(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo, const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria, const Periodo a_periodo_lag, const double a_grau_liberdade, std::vector<IdHidreletrica> a_listaIdHidreletrica) {

	try {

		if ((a_idProcessoEstocastico != IdProcessoEstocastico_hidrologico_hidreletrica) && (a_idProcessoEstocastico != IdProcessoEstocastico_hidrologico_bacia))
			throw std::invalid_argument("O argumento processo estocastico deve ser hidrologico");

		const IdEstagio menor_estagio = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());

		if (a_idEstagio < menor_estagio)
			return -1;

		if ((!vetorEstagio.att(a_idEstagio).isSolverInstanciado(a_TSS)) && (a_TSS == TipoSubproblemaSolver_mestre))
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

		const double infinito = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getInfinito();
		varYP = addVarDecisao_YP(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria, -infinito, infinito, 0.0);

		// Composicao de lag com periodos do horizonte de estudo
		int equYP = -1;
		double rhs_equYP = 0.0;
		const double sobreposicao_periodo_otimizacao = a_periodo_lag.sobreposicao(periodo_otimizacao);
		if (sobreposicao_periodo_otimizacao > 0.0) {

			equYP = addEquLinear_YP(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYP, equYP, 1.0);

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

						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_YP(a_TSS, a_idEstagio, periodo_otimizacao, periodo, idProcessoEstocastico_modelo, idVarEquiv.at(i)), equYP, -sobreposicao * coeficiente_participacao);
						rhs_equYP += (a_grau_liberdade - grau_liberdade) * sobreposicao;
					}

					if (!sobreposicao_encontrada)
						sobreposicao_encontrada = true;
				}
				else if ((sobreposicao == 0.0) && (sobreposicao_encontrada))
					break;

			} // for (Periodo periodo = periodo_inicial_proc_estoc_hidrologico; periodo <= periodo_final_proc_estoc_hidrologico; proc_estoc_hidrologico.incrementarIterador(periodo)) {

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(equYP, rhs_equYP);

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

						const IdHidreletrica idHidreletrica = listaIdHidreletrica.at(i);
						getIdVariavelAleatoriaIdVariavelAleatoriaInternaFromIdHidreletrica(idProcessoEstocastico_modelo, idVar, idVarInt, idHidreletrica);

						int varYH = getVarDecisao_YHseExistir(a_TSS, a_idEstagio, periodo, idHidreletrica);

						if (varYH == -1) {
							if (a_TSS != TipoSubproblemaSolver_viabilidade_hidraulica) {
								const double tendencia = getElementoVetor(idProcessoEstocastico_modelo, idVar, idVarInt, AttVetorVariavelAleatoriaInterna_tendencia_temporal, periodo, double());
								varYH = addVarDecisao_YH(a_TSS, a_idEstagio, periodo, idHidreletrica, tendencia, tendencia, 0.0);
							}
							else
								varYH = addVarDecisao_YH(a_TSS, a_idEstagio, periodo, idHidreletrica, -infinito, infinito, 0.0);
						}

						equYP = getEquLinear_YPseExistir(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria);

						if (equYP == -1) {
							equYP = addEquLinear_YP(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria);
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYP, equYP, 1.0);
						}

						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYH, equYP, -sobreposicao);

					} // for (int i = 0; i < int(a_listaIdHidreletrica.size()); i++) {

					rhs_equYP += (a_grau_liberdade)*sobreposicao;

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(equYP, rhs_equYP);

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
					vetorEstagio.att(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeVarDecisao_YP(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria) + "," + getString(a_grau_liberdade) + "," + getStringFromLista(a_listaIdHidreletrica, ",", false)), varYP, varYP_anterior);

				// Variáveis de estado a compor lag
				else if (sobreposicao_periodo_otimizacao > 0.0) {
					const int varYP_ADD = addVarDecisao_YP_ADD(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria, -infinito, infinito, 0.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varYP_ADD, equYP, -1.0);
					vetorEstagio.att(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeVarDecisao_YP(a_TSS, a_idEstagio, a_periodo, a_periodo_lag, a_idProcessoEstocastico, a_idVariavelAleatoria) + "," + getString(a_grau_liberdade) + "," + getStringFromLista(a_listaIdHidreletrica, ",", false)), varYP_ADD, varYP_anterior);
				}

			} // if ((varYP_anterior > -1) || ((idEstagio_anterior == menor_estagio) && (periodo_inicial_proc_estoc_hidrologico > a_idVariavelAleatoria))) {
		} // else if (a_idEstagio > IdEstagio_1) {

		return varYP;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisDecisao_VariaveisEstado_Restricoes_YP(a_dados," + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getString(a_periodo) + "," + getFullString(a_idProcessoEstocastico) + "," + getFullString(a_idVariavelAleatoria) + "," + getString(a_periodo_lag) + "," + getString(a_grau_liberdade) + "): \n" + std::string(erro.what())); }

}

int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_PTDISPCOM(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo, const IdPatamarCarga a_idPatamarCarga, const IdTermeletrica a_idTermeletrica, const double a_potencia_disponivel_minima, const double a_potencia_disponivel_maxima) {

	try {

		const IdEstagio menor_estagio = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());

		if (a_idEstagio < menor_estagio) {
			if (menor_estagio == IdEstagio_1)
				return -1;
			else
				return -2;
		}

		if ((!vetorEstagio.att(a_idEstagio).isSolverInstanciado(a_TSS)) || (a_TSS == TipoSubproblemaSolver_mestre) || (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica))
			return -1;

		const int lag_mensal_potencia_comandada = a_dados.getAtributo(a_idTermeletrica, AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada, int());
		if (lag_mensal_potencia_comandada == 0)
			throw std::invalid_argument(getFullString(a_idTermeletrica) + " com atributo " + getFullString(AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada) + " igual a zero.");

		// Quando patamar é inexistente, cria-se PTDISPCOMLAG recursivamente
		if (a_idPatamarCarga == IdPatamarCarga_Nenhum) {

			const Periodo periodo_comando_minuto = Periodo(TipoPeriodo_minuto, a_periodo);
			const Periodo periodo_otimizacao = getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

			int varPTDISPCOM = getVarDecisao_PTDISPCOMseExistir(a_TSS, a_idEstagio, a_periodo, a_idTermeletrica);

			if (varPTDISPCOM > -1)
				return varPTDISPCOM;
			else if ((periodo_comando_minuto.sobreposicao(periodo_otimizacao) == 0.0) && ((periodo_otimizacao < periodo_comando_minuto) && (periodo_comando_minuto < (Periodo(TipoPeriodo_minuto, getAtributo(getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio()), AttComumEstagio_periodo_otimizacao, Periodo()) + 1) - 1))))
				return -1;

			varPTDISPCOM = addVarDecisao_PTDISPCOM(a_TSS, a_idEstagio, a_periodo, a_idTermeletrica, 0.0, vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);

			const int varPTDISPCOM_anterior = criarVariaveisDecisao_VariaveisEstado_Restricoes_PTDISPCOM(a_TSS, a_dados, IdEstagio(a_idEstagio - 1), a_periodo, a_idPatamarCarga, a_idTermeletrica, a_potencia_disponivel_minima, a_potencia_disponivel_maxima);

			if (varPTDISPCOM_anterior < 0) {
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(varPTDISPCOM, a_potencia_disponivel_minima);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(varPTDISPCOM, a_potencia_disponivel_maxima);
			}
			
			if (varPTDISPCOM_anterior != -1)
				vetorEstagio.att(a_idEstagio).addVariavelEstado(a_TSS, std::string(getNomeVarDecisao_PTDISPCOM(a_TSS, a_idEstagio, a_periodo, a_idTermeletrica) + "," + getString(a_potencia_disponivel_minima) + "," + getString(a_potencia_disponivel_maxima)), varPTDISPCOM, varPTDISPCOM_anterior);

			return varPTDISPCOM;

		} // if (a_idPatamarCarga == IdPatamarCarga_Nenhum) {

		// Quando patamar é existente, cria-se PTDISPCOMLAG_pat e PTDISPCOMLAG recursivamente (caso necessário)

		int varPTDISPCOM_pat = getVarDecisao_PTDISPCOMseExistir(a_TSS, a_idEstagio, a_periodo, a_idPatamarCarga, a_idTermeletrica);
		if (varPTDISPCOM_pat > -1)
			return varPTDISPCOM_pat;

		varPTDISPCOM_pat = addVarDecisao_PTDISPCOM(a_TSS, a_idEstagio, a_periodo, a_idPatamarCarga, a_idTermeletrica, 0.0, vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);

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
						const double potencia_disponivel_comandada = horizonte_potencia_disponivel_comandada.at(periodo).at(a_idPatamarCarga);

						// Com sobreposicao ~exata, restrição POTENCIA_TERMICA_DISPONIVEL_COMANDADA_pat não é criada
						if (doubleCompara(1e-6, sobreposicao, 1.0)) {
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(varPTDISPCOM_pat, potencia_disponivel_comandada);
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(varPTDISPCOM_pat, potencia_disponivel_comandada);
							return varPTDISPCOM_pat;
						}

						int equPTDISPCOM_pat = getEquLinear_POTENCIA_TERMICA_DISPONIVEL_COMANDADAseExistir(a_TSS, a_idEstagio, a_periodo, a_idPatamarCarga, a_idTermeletrica);

						if (equPTDISPCOM_pat == -1) {
							equPTDISPCOM_pat = addEquLinear_POTENCIA_TERMICA_DISPONIVEL_COMANDADA(a_TSS, a_idEstagio, a_periodo, a_idPatamarCarga, a_idTermeletrica);
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISPCOM_pat, equPTDISPCOM_pat, 1.0);
						}

						int varPTDISPPRECOM_pat = addVarDecisao_PTDISPPRECOM(a_TSS, a_idEstagio, periodo, a_idPatamarCarga, a_idTermeletrica, potencia_disponivel_comandada, potencia_disponivel_comandada, 0.0);

						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISPPRECOM_pat, equPTDISPCOM_pat, -sobreposicao);

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

			const Periodo periodo_comando = Periodo(idMes_inicio, idAno_inicio);

			const int varPTDISPCOM = criarVariaveisDecisao_VariaveisEstado_Restricoes_PTDISPCOM(a_TSS, a_dados, a_idEstagio, periodo_comando, IdPatamarCarga_Nenhum, a_idTermeletrica, a_potencia_disponivel_minima, a_potencia_disponivel_maxima);

			int equPTDISPCOM_pat = getEquLinear_POTENCIA_TERMICA_DISPONIVEL_COMANDADAseExistir(a_TSS, a_idEstagio, a_periodo, a_idPatamarCarga, a_idTermeletrica);

			if (equPTDISPCOM_pat == -1) {
				equPTDISPCOM_pat = addEquLinear_POTENCIA_TERMICA_DISPONIVEL_COMANDADA(a_TSS, a_idEstagio, a_periodo, a_idPatamarCarga, a_idTermeletrica);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISPCOM_pat, equPTDISPCOM_pat, 1.0);
			}

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISPCOM, equPTDISPCOM_pat, -sobreposicao_complementar);

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

				const Periodo periodo_comando = Periodo(idMes_ultimo, idAno_ultimo);

				const int varPTDISPCOM = criarVariaveisDecisao_VariaveisEstado_Restricoes_PTDISPCOM(a_TSS, a_dados, a_idEstagio, periodo_comando, IdPatamarCarga_Nenhum, a_idTermeletrica, a_potencia_disponivel_minima, a_potencia_disponivel_maxima);

				int equPTDISPCOM_pat = getEquLinear_POTENCIA_TERMICA_DISPONIVEL_COMANDADAseExistir(a_TSS, a_idEstagio, a_periodo, a_idPatamarCarga, a_idTermeletrica);

				if (equPTDISPCOM_pat == -1) {
					equPTDISPCOM_pat = addEquLinear_POTENCIA_TERMICA_DISPONIVEL_COMANDADA(a_TSS, a_idEstagio, a_periodo, a_idPatamarCarga, a_idTermeletrica);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISPCOM_pat, equPTDISPCOM_pat, 1.0);
				}

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(varPTDISPCOM, equPTDISPCOM_pat, -sobreposicao_complementar);

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
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarVariaveisDecisao_VariaveisEstado_Restricoes_PTDISPCOM(a_dados," + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getString(a_periodo) + "," + getFullString(a_idPatamarCarga) + "," + getFullString(a_idTermeletrica) + "," + getFullString(a_potencia_disponivel_minima) + "," + getFullString(a_potencia_disponivel_maxima) + "): \n" + std::string(erro.what())); }

} // int ModeloOtimizacao::criarVariaveisDecisao_VariaveisEstado_Restricoes_QDEFLAG(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo, const IdHidreletrica a_idHidreletrica) {


void ModeloOtimizacao::addCorteBendersToZF(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdEstagio a_idEstagio_anterior, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders) {

	try {

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const Periodo periodo_otimizacao_anterior = getAtributo(a_idEstagio_anterior, AttComumEstagio_periodo_otimizacao, Periodo());

		int posIneZF = getIneLinear_CB_ZFseExistir(a_TSS, a_idEstagio_anterior, periodo_otimizacao_anterior, a_idRealizacao, a_idCorteBenders);

		if ((posIneZF > -1) && (a_idCorteBenders != IdCorteBenders_1))
			throw std::invalid_argument("Corte de benders ja adicionado.");
		else if (posIneZF == -1)
			posIneZF = addIneLinear_CB_ZF(a_TSS, a_idEstagio_anterior, periodo_otimizacao_anterior, a_idRealizacao, a_idCorteBenders);

		vetorEstagio.att(a_idEstagio_anterior).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZF(a_TSS, a_idEstagio_anterior, periodo_otimizacao_anterior, a_idRealizacao), posIneZF, 1.0);

		setCorteBendersToZF(a_TSS, a_idEstagio, a_idEstagio_anterior, a_idRealizacao, a_idCorteBenders);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::addCorteBendersToZF(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idEstagio_anterior) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCorteBenders) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::addCorteBenders(const IdEstagio a_idEstagio, const IdCorteBenders a_idCorteBenders, const double a_rhs, const SmartEnupla<IdVariavelEstado, double>& a_coeficiente){


void ModeloOtimizacao::setCorteBendersToZF(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdEstagio a_idEstagio_anterior, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders) {

	try {

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const Periodo periodo_otimizacao_anterior = vetorEstagio.att(a_idEstagio_anterior).getAtributo(AttComumEstagio_periodo_otimizacao, Periodo());

		const int posIneZF = getIneLinear_CB_ZF(a_TSS, a_idEstagio_anterior, periodo_otimizacao_anterior, a_idRealizacao, a_idCorteBenders);

		for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= getMaiorId(a_idEstagio, IdVariavelEstado()); idVariavelEstado++) {

			if (vetorEstagio.att(a_idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

				const int idVariavelDecisaoEstagioAnterior = getAtributo(a_idEstagio, idVariavelEstado, AttComumVariavelEstado_idVariavelDecisaoEstagioAnterior, int());

				if (idVariavelDecisaoEstagioAnterior < 0)
					vetorEstagio.att(a_idEstagio).anularVariavelEstadoCorteBenders(idVariavelEstado);
				else
					vetorEstagio.att(a_idEstagio_anterior).getSolver(a_TSS)->setCofRestricao(idVariavelDecisaoEstagioAnterior, posIneZF, -getElementoMatriz(a_idEstagio, a_idCorteBenders, AttMatrizCorteBenders_coeficiente, a_idRealizacao, idVariavelEstado, double()));

			} // if (vetorEstagio.att(a_idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

			else
				vetorEstagio.att(a_idEstagio).anularVariavelEstadoCorteBenders(idVariavelEstado);

		} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= getMaiorId(a_idEstagio, IdVariavelEstado()); idVariavelEstado++) {

		vetorEstagio.att(a_idEstagio_anterior).getSolver(a_TSS)->setRHSRestricao(posIneZF, getElementoVetor(a_idEstagio, a_idCorteBenders, AttVetorCorteBenders_rhs, a_idRealizacao, double()));

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::setCorteBendersToZF(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idEstagio_anterior) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCorteBenders) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::setCorteBendersToZF(const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders, const int a_posicao_corte, const double a_rhs, const SmartEnupla<IdVariavelEstado, double>& a_coeficiente){


void ModeloOtimizacao::remCorteBendersFromZF(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdEstagio a_idEstagio_anterior, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders) {

	try {

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const Periodo periodo_otimizacao_anterior = vetorEstagio.att(a_idEstagio_anterior).getAtributo(AttComumEstagio_periodo_otimizacao, Periodo());

		const int posIneZF = getIneLinear_CB_ZFseExistir(a_TSS, a_idEstagio_anterior, periodo_otimizacao_anterior, a_idRealizacao, a_idCorteBenders);

		if (posIneZF == -1)
			return;

		vetorEstagio.att(a_idEstagio_anterior).getSolver(a_TSS)->remRestricao(posIneZF);

		idx_IneLinear_CB_ZF_4.at(a_TSS).at(a_idEstagio_anterior).at(periodo_otimizacao_anterior).at(a_idRealizacao).at(a_idCorteBenders) = SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>();

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::remCorteBendersFromZF(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio_anterior) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCorteBenders) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::remCorteBendersToZF(const IdEstagio a_idEstagio, const IdEstagio a_idEstagio_anterior, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders){



void ModeloOtimizacao::addCorteBendersToZT(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders){

	try {

		const Periodo periodo_otimizacao = getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

		int posIneZT = getIneLinear_CB_ZTseExistir(a_TSS, a_idEstagio, periodo_otimizacao, a_idRealizacao, a_idCorteBenders);

		if ((posIneZT > -1) && (a_idCorteBenders != IdCorteBenders_1))
			throw std::invalid_argument("Corte de benders ja adicionado.");
		else if (posIneZT == -1)
			posIneZT = addIneLinear_CB_ZT(a_TSS, a_idEstagio, periodo_otimizacao, a_idRealizacao, a_idCorteBenders);

		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZT(a_TSS, a_idEstagio, periodo_otimizacao, a_idRealizacao), posIneZT, 1.0);

		setCorteBendersToZT(a_TSS, a_idEstagio, a_idRealizacao, a_idCorteBenders);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::addCorteBendersToZT(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCorteBenders) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::addCorteBendersToZT(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders, const double a_rhs, const SmartEnupla<IdVariavelEstado, double>& a_coeficiente){

void ModeloOtimizacao::setCorteBendersToZT(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders){

	try {

		const Periodo periodo_otimizacao = vetorEstagio.att(a_idEstagio).getAtributo(AttComumEstagio_periodo_otimizacao, Periodo());

		const int posIneZT = getIneLinear_CB_ZT(a_TSS, a_idEstagio, periodo_otimizacao, a_idRealizacao, a_idCorteBenders);

		for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= getMaiorId(a_idEstagio, IdVariavelEstado()); idVariavelEstado++) {

			if (vetorEstagio.att(a_idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

				const int idVariavelDecisao = getElementoVetor(a_idEstagio, idVariavelEstado, AttVetorVariavelEstado_idVariavelDecisao, a_TSS, int());

				if (idVariavelDecisao < 0)
					vetorEstagio.att(a_idEstagio).anularVariavelEstadoCorteBenders(idVariavelEstado);
				else
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(idVariavelDecisao, posIneZT, -getElementoMatriz(a_idEstagio, a_idCorteBenders, AttMatrizCorteBenders_coeficiente, a_idRealizacao, idVariavelEstado, double()));

			} // if (vetorEstagio.att(a_idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

			else
				vetorEstagio.att(a_idEstagio).anularVariavelEstadoCorteBenders(idVariavelEstado);

		} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= a_coeficiente.getIteradorFinal(); idVariavelEstado++) {

		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneZT, getElementoVetor(a_idEstagio, a_idCorteBenders, AttVetorCorteBenders_rhs, a_idRealizacao, double()));

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::setCorteBendersToZT(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCorteBenders) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::setCorteBendersToZT(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders, const double a_rhs, const SmartEnupla<IdVariavelEstado, double>& a_coeficiente){

void ModeloOtimizacao::remCorteBendersFromZT(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders){

	try {

		const Periodo periodo_otimizacao = vetorEstagio.att(a_idEstagio).getAtributo(AttComumEstagio_periodo_otimizacao, Periodo());

		const int posIneZT = getIneLinear_CB_ZTseExistir(a_TSS, a_idEstagio, periodo_otimizacao, a_idRealizacao, a_idCorteBenders);

		if (posIneZT == -1)
			return;

		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->remRestricao(posIneZT);

		idx_IneLinear_CB_ZT_4.at(a_TSS).at(a_idEstagio).at(periodo_otimizacao).at(a_idRealizacao).at(a_idCorteBenders) = SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>();

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


void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_vazao_afluente(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const SmartEnupla<Periodo, int>& a_horizonte_processo_estocastico, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio)
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

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_periodo_estudo, IdPatamarCarga_1, double()) != getdoubleFromChar("min")) {

				if (getVarDecisao_RHA_FINF(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE) > -1) {

					const int posIneRHA = addIneLinear_RESTRICAO_HIDRAULICA_AFLUENTE_LIMITE_INFERIOR(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE);

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHA, a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_periodo_estudo, IdPatamarCarga_1, double()));

					// Variável de folga RHA_FINF				
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHA_FINF(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE), posIneRHA, 1.0);

					const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
					for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

						const IdHidreletrica	idHidreletrica = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());

						const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, IdPatamarCarga());

						if (idHidreletrica != IdHidreletrica_Nenhum) {

							// Parcela YH vazão incremental							

							Periodo proximo_periodo_estudo = a_periodo_estudo;
							a_horizonte_estudo_estagio.incrementarIterador(proximo_periodo_estudo);

							for (Periodo periodo = a_horizonte_processo_estocastico.getIteradorInicial(); periodo <= a_horizonte_processo_estocastico.getIteradorFinal(); a_horizonte_processo_estocastico.incrementarIterador(periodo)) {

								Periodo proximo_periodo_processo_estocastico = periodo;
								a_horizonte_processo_estocastico.incrementarIterador(proximo_periodo_processo_estocastico);

								if ((periodo <= a_periodo_estudo) && (a_periodo_estudo < proximo_periodo_processo_estocastico)) {
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_YH(a_TSS, a_idEstagio, periodo, idHidreletrica), posIneRHA, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, IdPatamarCarga_1, double()));
									break;
								}

								else if ((a_periodo_estudo <= periodo) && (periodo < proximo_periodo_estudo)) {
									const double fator_tempo = double(periodo.getMinutos()) / double(a_periodo_estudo.getMinutos());
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_YH(a_TSS, a_idEstagio, periodo, idHidreletrica), posIneRHA, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, IdPatamarCarga_1, double()) * fator_tempo);
								}
							}


							// Parcela QDEF (Turbinamento + Vertimento das usinas hidrelétricas a montante)

							const int numero_hidreletricas_montante = a_dados.getSizeVetor(idHidreletrica, AttVetorHidreletrica_montante);

							for (int m = 1; m <= numero_hidreletricas_montante; m++) {

								const IdHidreletrica idHidreletrica_montante = a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_montante, m, IdHidreletrica());

								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

									const double duracao_patamar = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, idPatamarCarga, double());

									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, idHidreletrica_montante), posIneRHA, duracao_patamar * a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, idPatamarCarga, double()));

								}//// for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

							} // for (int m = 1; m <= numero_hidreletricas_montante; m++) {

							//QDES
							// Variáveis de Hidreletricas à Montante desvio
							const int numero_hidreletricas_montante_desvio = a_dados.getSizeVetor(idHidreletrica, AttVetorHidreletrica_montante_desvio);
							for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

								const IdHidreletrica idHidreletrica_montante_desvio = a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_montante_desvio, m, IdHidreletrica());

								// Variável QDES que "entra" na usina

								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

									const double duracao_patamar = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, idPatamarCarga, double());
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, idHidreletrica_montante_desvio, idHidreletrica), posIneRHA, duracao_patamar * a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, idPatamarCarga, double()));

								}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

							}//for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

							//Parcela dos intercámbios que "chegam" na hidrelétrica
							// hidreletrica_destino

							const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_destino = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_destino, idHidreletrica);

							const int idIntercambioHidraulico_destino_size = int(idIntercambioHidraulico_destino.size());

							for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {

								const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

								if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

									for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

										const double duracao_patamar = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, idPatamarCarga, double());

										if (getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())) > -1)
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())), posIneRHA, duracao_patamar * a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, idPatamarCarga, double()));

									}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

								}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

							}//for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {


							//Parcela de QOUT com porcentagem_retorno > 0 

							const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_origem = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_origem, idHidreletrica);

							const int idIntercambioHidraulico_origem_size = int(idIntercambioHidraulico_origem.size());

							for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

								const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

								if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada && a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()) > 0) {

									for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

										const double duracao_patamar = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, idPatamarCarga, double());

										if (getVarDecisao_QOUT(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())) > -1)
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QOUT(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posIneRHA, duracao_patamar * a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()) * a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, idPatamarCarga, double()));

									}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

								}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada && a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()) > 0) {

							}//for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

						} // if (idHidreletrica != IdHidreletrica_Nenhum) {
						else
							throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

					} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

				} // if (getVarDecisao_RHA_FINF(a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE) > -1) {

			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_periodo_estudo, IdPatamarCarga_1, double()) > 0) {

			//******************************************
			//Restrição >=
			//******************************************

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_periodo_estudo, IdPatamarCarga_1, double()) != getdoubleFromChar("max")) {

				if (getVarDecisao_RHA_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE) > -1) {

					const int posIneRHA = addIneLinear_RESTRICAO_HIDRAULICA_AFLUENTE_LIMITE_SUPERIOR(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE);

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHA, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_periodo_estudo, IdPatamarCarga_1, double()));

					// Variável de folga RHA_FSUP					
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHA_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE), posIneRHA, 1.0);

					const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
					for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

						const IdHidreletrica	idHidreletrica = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());

						const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, IdPatamarCarga());

						if (idHidreletrica != IdHidreletrica_Nenhum) {

							// Parcela YH vazão incremental
							Periodo proximo_periodo_estudo = a_periodo_estudo;
							a_horizonte_estudo_estagio.incrementarIterador(proximo_periodo_estudo);

							for (Periodo periodo = a_horizonte_processo_estocastico.getIteradorInicial(); periodo <= a_horizonte_processo_estocastico.getIteradorFinal(); a_horizonte_processo_estocastico.incrementarIterador(periodo)) {

								Periodo proximo_periodo_processo_estocastico = periodo;
								a_horizonte_processo_estocastico.incrementarIterador(proximo_periodo_processo_estocastico);

								if ((periodo <= a_periodo_estudo) && (a_periodo_estudo < proximo_periodo_processo_estocastico)) {
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_YH(a_TSS, a_idEstagio, periodo, idHidreletrica), posIneRHA, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, IdPatamarCarga_1, double()));
									break;
								}

								else if ((a_periodo_estudo <= periodo) && (periodo < proximo_periodo_estudo)) {
									const double fator_tempo = double(periodo.getMinutos()) / double(a_periodo_estudo.getMinutos());
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_YH(a_TSS, a_idEstagio, periodo, idHidreletrica), posIneRHA, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, IdPatamarCarga_1, double()) * fator_tempo);
								}
							}

							// Parcela QDEF (Turbinamento + Vertimento das usinas hidrelétricas a montante)

							const int numero_hidreletricas_montante = a_dados.getSizeVetor(idHidreletrica, AttVetorHidreletrica_montante);

							for (int m = 1; m <= numero_hidreletricas_montante; m++) {

								const IdHidreletrica idHidreletrica_montante = a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_montante, m, IdHidreletrica());

								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

									const double duracao_patamar = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, idPatamarCarga, double());

									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, idHidreletrica_montante), posIneRHA, -duracao_patamar * a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, idPatamarCarga, double()));

								}//// for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

							} // for (int m = 1; m <= numero_hidreletricas_montante; m++) {

							//QDES
							// Variáveis de Hidreletricas à Montante desvio
							const int numero_hidreletricas_montante_desvio = a_dados.getSizeVetor(idHidreletrica, AttVetorHidreletrica_montante_desvio);
							for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

								const IdHidreletrica idHidreletrica_montante_desvio = a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_montante_desvio, m, IdHidreletrica());

								// Variável QDES que "entra" na usina

								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

									const double duracao_patamar = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, idPatamarCarga, double());
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, idHidreletrica_montante_desvio, idHidreletrica), posIneRHA, -duracao_patamar * a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, idPatamarCarga, double()));

								}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

							}//for (int m = 1; m <= numero_hidreletricas_montante_desvio; m++) {

							//Parcela dos intercámbios que "chegam" na hidrelétrica
							// hidreletrica_destino

							const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_destino = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_destino, idHidreletrica);

							const int idIntercambioHidraulico_destino_size = int(idIntercambioHidraulico_destino.size());

							for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {

								const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

								if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

									for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

										const double duracao_patamar = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, idPatamarCarga, double());

										if (getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())) > -1)
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())), posIneRHA, -duracao_patamar * a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, idPatamarCarga, double()));

									}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

								}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

							}//for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {

							//Parcela de QOUT com porcentagem_retorno > 0 

							const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_origem = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_origem, idHidreletrica);

							const int idIntercambioHidraulico_origem_size = int(idIntercambioHidraulico_origem.size());

							for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

								const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

								if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada && a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()) > 0) {

									for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

										const double duracao_patamar = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, idPatamarCarga, double());

										if (getVarDecisao_QOUT(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())) > -1)
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QOUT(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posIneRHA, -duracao_patamar * a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()) * a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, idPatamarCarga, double()));

									}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

								}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada && a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()) > 0) {

							}//for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

						} // if (idHidreletrica != IdHidreletrica_Nenhum) {
						else
							throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

					} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

				} // if (getVarDecisao_RHA_FSUP(a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE) > -1) {

			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_periodo_estudo, IdPatamarCarga_1, double()) < getdoubleFromChar("max")) {

		}// for (int pos = 0; pos < numero_restricoes; pos++) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::criarRestricoesHidraulicaEspecial_vazao_afluente(" + getFullString(a_TSS) + ",Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados): \n" + std::string(erro.what())); }

}//void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_vazao_afluente(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados)


void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_vazao_defluente(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdPatamarCarga a_idPatamarCarga, const Periodo a_periodo_estudo) {

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

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_periodo_estudo, a_idPatamarCarga, double()) != getdoubleFromChar("min")) {

				if (getVarDecisao_RHQ_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoOperativaUHE) > -1) {

					const int posIneRHQ = addIneLinear_RESTRICAO_HIDRAULICA_DEFLUENCIA_LIMITE_INFERIOR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoOperativaUHE);

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHQ, a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_periodo_estudo, a_idPatamarCarga, double()));

					// Variável RHQ_FINF
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHQ_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoOperativaUHE), posIneRHQ, 1.0);

					const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
					for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

						const IdHidreletrica       idHidreletrica = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
						const IdUsinaElevatoria idUsinaElevatoria = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());

						const TipoVariavelRestricaoOperativa tipoVariavelRestricaoOperativa = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

						if (idHidreletrica != IdHidreletrica_Nenhum) {

							// Variável QDEF (Vazão defluente das usinas hidrelétricas)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_defluente) {

								if (a_dados.getAtributo(AttComumDados_representar_defluencia_disponivel_em_restricoes_hidraulicas, bool()))
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posIneRHQ, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));
								else
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posIneRHQ, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_defluente) {

							// Variável QTUR
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_turbinada) {

								if (a_dados.getAtributo(AttComumDados_representar_turbinamento_disponivel_em_restricoes_hidraulicas, bool())) {
									if (getVarDecisao_QTURDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posIneRHQ, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));
								}
								else {
									if (getVarDecisao_QTURseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posIneRHQ, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));
								}

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_defluente) {

							// Variável QDES
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_desviada) {

								// Variável QDES que "sai" na usina
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posIneRHQ, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

								// hidreletrica_origem

								const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_origem = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_origem, idHidreletrica);

								const int idIntercambioHidraulico_origem_size = int(idIntercambioHidraulico_origem.size());

								for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

									const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

									if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

										if (getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())) > -1)
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())), posIneRHQ, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

									}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

								}//for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_desviada) {

						} // if (idHidreletrica != IdHidreletrica_Nenhum) {
						else if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {

							// Variável QBOM (Vazao bombeada)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_bombeada) {

								if (getVarDecisao_QBOMseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaElevatoria) > -1)
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaElevatoria), posIneRHQ, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_bombeado) {

						}//else if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {
						else
							throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

					} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

				} // if (getVarDecisao_RHQseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoOperativaUHE) > -1) {

			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_periodo_estudo, a_idPatamarCarga, double()) > 0) {

			//******************************************
			//Restrição >=
			//******************************************

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_periodo_estudo, a_idPatamarCarga, double()) != getdoubleFromChar("max")) {

				if (getVarDecisao_RHQ_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoOperativaUHE) > -1) {

					const int posIneRHQ = addIneLinear_RESTRICAO_HIDRAULICA_DEFLUENCIA_LIMITE_SUPERIOR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoOperativaUHE);

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHQ, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_periodo_estudo, a_idPatamarCarga, double()));

					// Variável RHQ_FSUP
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHQ_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoOperativaUHE), posIneRHQ, 1.0);

					const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
					for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

						const IdHidreletrica       idHidreletrica = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
						const IdUsinaElevatoria idUsinaElevatoria = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());

						const TipoVariavelRestricaoOperativa tipoVariavelRestricaoOperativa = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

						if (idHidreletrica != IdHidreletrica_Nenhum) {

							// Variável QDEF (Vazão defluente das usinas hidrelétricas)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_defluente) {

								if (a_dados.getAtributo(AttComumDados_representar_defluencia_disponivel_em_restricoes_hidraulicas, bool()))
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posIneRHQ, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));
								else
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posIneRHQ, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_defluente) {

							// Variável QTUR
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_turbinada) {

								if (a_dados.getAtributo(AttComumDados_representar_turbinamento_disponivel_em_restricoes_hidraulicas, bool())) {
									if (getVarDecisao_QTURDISPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTURDISP(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posIneRHQ, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));
								}
								else {
									if (getVarDecisao_QTURseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QTUR(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica), posIneRHQ, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));
								}
							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_defluente) {

							// Variável QDES
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_desviada) {

								//QDES
								vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idHidreletrica, a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posIneRHQ, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

								// hidreletrica_origem

								const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_origem = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_origem, idHidreletrica);

								const int idIntercambioHidraulico_origem_size = int(idIntercambioHidraulico_origem.size());

								for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

									const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

									if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

										if (getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())) > -1)
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())), posIneRHQ, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

									}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

								}//for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_desviada) {

						} // if (idHidreletrica != IdHidreletrica_Nenhum) {
						else if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {

							// Variável QBOM (Vazão bombeada)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_vazao_bombeada) {

								if (getVarDecisao_QBOMseExistir(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaElevatoria) > -1)
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idUsinaElevatoria), posIneRHQ, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, a_idPatamarCarga, double()));

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_bombeado) {

						}//else if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {
						else
							throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

					} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

				} // if (getVarDecisao_RHQseExistir(a_idEstagio, a_periodo_estudo, a_idPatamarCarga, idRestricaoOperativaUHE) > -1) {

			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_periodo_estudo, a_idPatamarCarga, double()) > getdoubleFromChar("max")) {


		}// for (int pos = 0; pos < numero_restricoes; pos++) {

	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("criarRestricoesHidraulicaEspecial_vazao_defluente(" + getFullString(a_TSS) + ",Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados): \n" + std::string(erro.what())); }

}//void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_vazao_defluente(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados)


void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_volume_armazenado(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const SmartEnupla<Periodo, double>a_horizonte_estudo_estagio) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		const std::vector<IdRestricaoOperativaUHE> idRestricaoOperativaUHE_volume_armazenado = a_dados.vetorRestricaoOperativaUHE.getIdObjetos(AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica_volume_armazenado);

		const int numero_restricoes = int(idRestricaoOperativaUHE_volume_armazenado.size());

		for (int pos = 0; pos < numero_restricoes; pos++) {

			const IdRestricaoOperativaUHE idRestricaoOperativaUHE = idRestricaoOperativaUHE_volume_armazenado.at(pos);

			Periodo periodo_inicial_dos_dados = a_horizonte_estudo_estagio.getIteradorInicial();

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

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_periodo_estudo, IdPatamarCarga_1, double()) != getdoubleFromChar("min")) {

				if (getVarDecisao_RHV_FINF(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE) > -1) {

					const int posIneRHV = addIneLinear_RESTRICAO_HIDRAULICA_VOLUME_LIMITE_INFERIOR(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE);

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHV, a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_periodo_estudo, IdPatamarCarga_1, double()));

					//Variável RHV_FINF				
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHV_FINF(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE), posIneRHV, 1.0);

					const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
					for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

						const IdHidreletrica       idHidreletrica = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
						const IdUsinaElevatoria idUsinaElevatoria = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());

						const TipoVariavelRestricaoOperativa tipoVariavelRestricaoOperativa = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

						if (idHidreletrica != IdHidreletrica_Nenhum) {

							// Variável VI ou VF (Volume armazenado nos reservatórios)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_util) {

								if (a_periodo_estudo == a_horizonte_estudo_estagio.getIteradorFinal()) {

									if (getVarDecisao_VF(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica), posIneRHV, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, IdPatamarCarga_1, double()));

								}//if (a_periodo_estudo == a_horizonte_estudo.getIteradorFinal()) {
								else {

									Periodo periodo_seguinte = a_periodo_estudo;

									a_horizonte_estudo_estagio.incrementarIterador(periodo_seguinte); //O volume final do estágio t é igual ao volume inicial de t+1

									if (getVarDecisao_VI(a_TSS, a_idEstagio, periodo_seguinte, idHidreletrica) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, periodo_seguinte, idHidreletrica), posIneRHV, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, IdPatamarCarga_1, double()));

								}//else {

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_util) {

							//Nota: A vazão defluente/desviada é o somatório de todas as vazões por periodo dentro de um estágio_DC

							// Variável QDEF (Volume defluente das usinas hidrelétricas)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_defluente) {

								for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_periodo_estudo; a_horizonte_estudo_estagio.incrementarIterador(periodo_auxiliar)) {

									const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_auxiliar, IdPatamarCarga());

									for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, periodo_auxiliar, idPatamarCarga, idHidreletrica), posIneRHV, a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPatamarCarga, double()));

									}//// for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

								}//for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_periodo_estudo; a_horizonte_estudo.incrementarIterador(periodo_auxiliar)) {																													

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_defluente) {

							// Variável QDES (Volume desviado)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_desviado) {

								for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_periodo_estudo; a_horizonte_estudo_estagio.incrementarIterador(periodo_auxiliar)) {

									const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_auxiliar, IdPatamarCarga());

									for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

										//QDES
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, idHidreletrica, a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posIneRHV, a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPatamarCarga, double()));


										// hidreletrica_destino

										const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_destino = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_destino, idHidreletrica);

										const int idIntercambioHidraulico_destino_size = int(idIntercambioHidraulico_destino.size());

										for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {

											const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

											if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

												if (getVarDecisao_QDES(a_TSS, a_idEstagio, periodo_auxiliar, idPatamarCarga, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())) > -1)
													vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, periodo_auxiliar, idPatamarCarga, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())), posIneRHV, a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPatamarCarga, double()));

											}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

										}//for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {


										//Parcela de QOUT com porcentagem_retorno > 0 

										const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_origem = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_origem, idHidreletrica);

										const int idIntercambioHidraulico_origem_size = int(idIntercambioHidraulico_origem.size());

										for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

											const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

											if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada && a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()) > 0) {

												if (getVarDecisao_QOUT(a_TSS, a_idEstagio, periodo_auxiliar, idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())) > -1)
													vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QOUT(a_TSS, a_idEstagio, periodo_auxiliar, idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posIneRHV, a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()) * a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPatamarCarga, double()));

											}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada && a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()) > 0) {

										}//for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

									}//// for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

								}//for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_periodo_estudo; a_horizonte_estudo.incrementarIterador(periodo_auxiliar)) {																													

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_desviado) {

						} // if (idHidreletrica != IdHidreletrica_Nenhum) {
						else if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {

							//Nota: A vazão bombeada é o somatório de todas as vazões por periodo dentro de um estágio_DC

							// Variável QBOM (Volume bombeado)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_bombeado) {

								for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_periodo_estudo; a_horizonte_estudo_estagio.incrementarIterador(periodo_auxiliar)) {

									const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_auxiliar, IdPatamarCarga());

									for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

										if (getVarDecisao_QBOMseExistir(a_TSS, a_idEstagio, periodo_auxiliar, idPatamarCarga, idUsinaElevatoria) > -1)
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, periodo_auxiliar, idPatamarCarga, idUsinaElevatoria), posIneRHV, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, periodo_auxiliar, idPatamarCarga, double()) * a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPatamarCarga, double()));

									}//// for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

								}//for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_periodo_estudo; a_horizonte_estudo.incrementarIterador(periodo_auxiliar)) {																													

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_desviado) {

						}//if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {
						else
							throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

					} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

				} // if (getVarDecisao_RHVseExistir(a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE) > -1) {

			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_periodo_estudo, IdPatamarCarga_1, double()) > 0) {

			//******************************************
			//Restrição >=
			//******************************************

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_periodo_estudo, IdPatamarCarga_1, double()) != getdoubleFromChar("max")) {

				if (getVarDecisao_RHV_FSUPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE) > -1) {

					const int posIneRHV = addIneLinear_RESTRICAO_HIDRAULICA_VOLUME_LIMITE_SUPERIOR(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE);

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHV, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_periodo_estudo, IdPatamarCarga_1, double()));

					// Variável RHV
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHV_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE), posIneRHV, 1.0);

					const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
					for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

						const IdHidreletrica       idHidreletrica = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
						const IdUsinaElevatoria idUsinaElevatoria = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());

						const TipoVariavelRestricaoOperativa tipoVariavelRestricaoOperativa = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

						if (idHidreletrica != IdHidreletrica_Nenhum) {

							// Variável VF ou VI (Volume armazenado nos reservatórios)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_util) {

								if (a_periodo_estudo == a_horizonte_estudo_estagio.getIteradorFinal()) {

									if (getVarDecisao_VF(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica), posIneRHV, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, IdPatamarCarga_1, double()));

								}//if (a_periodo_estudo == a_horizonte_estudo.getIteradorFinal()) {
								else {

									Periodo periodo_seguinte = a_periodo_estudo;

									a_horizonte_estudo_estagio.incrementarIterador(periodo_seguinte); //O volume final do estágio t é igual ao volume inicial de t+1

									if (getVarDecisao_VI(a_TSS, a_idEstagio, periodo_seguinte, idHidreletrica) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, periodo_seguinte, idHidreletrica), posIneRHV, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, IdPatamarCarga_1, double()));

								}//else {
							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_util) {

							//Nota: A vazão defluente/desviada é o somatório de todas as vazões por periodo dentro de um estágio_DC

							// Variável QDEF (Volume defluente das usinas hidrelétricas)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_defluente) {

								for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_periodo_estudo; a_horizonte_estudo_estagio.incrementarIterador(periodo_auxiliar)) {

									const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_auxiliar, IdPatamarCarga());

									for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDEF(a_TSS, a_idEstagio, periodo_auxiliar, idPatamarCarga, idHidreletrica), posIneRHV, -a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPatamarCarga, double()));

									}//// for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

								}//for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_periodo_estudo; a_horizonte_estudo.incrementarIterador(periodo_auxiliar)) {																													

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_defluente) {

							// Intercambios Hidráulicos (Volume desviado)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_desviado) {

								for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_periodo_estudo; a_horizonte_estudo_estagio.incrementarIterador(periodo_auxiliar)) {

									const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_auxiliar, IdPatamarCarga());

									for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

										//QDES
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, a_periodo_estudo, idPatamarCarga, idHidreletrica, a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica())), posIneRHV, -a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPatamarCarga, double()));

										// hidreletrica_destino

										const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_destino = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_destino, idHidreletrica);

										const int idIntercambioHidraulico_destino_size = int(idIntercambioHidraulico_destino.size());

										for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {

											const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

											if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

												if (getVarDecisao_QDES(a_TSS, a_idEstagio, periodo_auxiliar, idPatamarCarga, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())) > -1)
													vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QDES(a_TSS, a_idEstagio, periodo_auxiliar, idPatamarCarga, idIntercambioHidraulico_destino.at(pos), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica()), a_dados.getAtributo(idIntercambioHidraulico_destino.at(pos), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica())), posIneRHV, -a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPatamarCarga, double()));

											}//if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_desvio) {

										}//for (int pos = 0; pos < idIntercambioHidraulico_destino_size; pos++) {

										//Parcela de QOUT com porcentagem_retorno > 0 

										const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_origem = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_origem, idHidreletrica);

										const int idIntercambioHidraulico_origem_size = int(idIntercambioHidraulico_origem.size());

										for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

											const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

											if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada && a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()) > 0) {

												if (getVarDecisao_QOUT(a_TSS, a_idEstagio, periodo_auxiliar, idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())) > -1)
													vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QOUT(a_TSS, a_idEstagio, periodo_auxiliar, idPatamarCarga, idIntercambioHidraulico_origem.at(pos), a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posIneRHV, -a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPatamarCarga, double()) * a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()));

											}//	if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada && a_dados.getAtributo(idIntercambioHidraulico_origem.at(pos), AttComumIntercambioHidraulico_percentual_retorno_do_desvio, double()) > 0) {

										}//for (int pos = 0; pos < idIntercambioHidraulico_origem_size; pos++) {

									}//// for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

								}//for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_periodo_estudo; a_horizonte_estudo.incrementarIterador(periodo_auxiliar)) {																													

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_desviado) {

						} // if (idHidreletrica != IdHidreletrica_Nenhum) {
						else if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {

							//Nota: A vazão bombeada é o somatório de todas as vazões por periodo dentro de um estágio_DC

							// Variável QBOM (Volume bombeado)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_bombeado) {

								for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_periodo_estudo; a_horizonte_estudo_estagio.incrementarIterador(periodo_auxiliar)) {

									const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_auxiliar, IdPatamarCarga());

									for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

										if (getVarDecisao_QBOMseExistir(a_TSS, a_idEstagio, periodo_auxiliar, idPatamarCarga, idUsinaElevatoria) > -1)
											vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QBOM(a_TSS, a_idEstagio, periodo_auxiliar, idPatamarCarga, idUsinaElevatoria), posIneRHV, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, periodo_auxiliar, idPatamarCarga, double()) * a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodo_auxiliar, idPatamarCarga, double()));

									}//// for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

								}//for (Periodo periodo_auxiliar = periodo_inicial_dos_dados; periodo_auxiliar <= a_periodo_estudo; a_horizonte_estudo.incrementarIterador(periodo_auxiliar)) {																													

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_desviado) {

						}//if (idUsinaElevatoria != IdUsinaElevatoria_Nenhum) {
						else
							throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

					} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

				} // if (getVarDecisao_RHVseExistir(a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE) > -1) {

			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_periodo_estudo, IdPatamarCarga_1, double()) < getdoubleFromChar("max")) {

			//Atualiza o periodo onde começa o estágio da leitura dos dados
			if (a_periodo_estudo < a_horizonte_estudo_estagio.getIteradorFinal()) {//Signica um periodo que corresponde a o fim de um estagio_DC mas não do estagio_SPT
				periodo_inicial_dos_dados = a_periodo_estudo;
				a_horizonte_estudo_estagio.incrementarIterador(periodo_inicial_dos_dados);
			}//if (a_periodo_estudo < a_horizonte_estudo.getIteradorFinal()) {	

		}// for (int pos = 0; pos < numero_restricoes; pos++) {

	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("criarRestricoesHidraulicaEspecial_volume_armazenado(" + getFullString(a_TSS) + ",Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados): \n" + std::string(erro.what())); }

}//void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_volume_armazenado(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados)

void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_energia_armazenada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const SmartEnupla<Periodo, double>a_horizonte_estudo_estagio) {

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

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_periodo_estudo, IdPatamarCarga_1, double()) != getdoubleFromChar("min")) {

				const int posIneRHE = addIneLinear_RESTRICAO_HIDRAULICA_ENERGIA_LIMITE_INFERIOR(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE);

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHE, a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_periodo_estudo, IdPatamarCarga_1, double()));

				//Variável RHE_FINF
				if (getVarDecisao_RHE_FINFseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE) > -1)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHE_FINF(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE), posIneRHE, 1.0);

				const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
				for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

					const IdHidreletrica       idHidreletrica = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
					const TipoVariavelRestricaoOperativa tipoVariavelRestricaoOperativa = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

					if (idHidreletrica != IdHidreletrica_Nenhum) {

						// Variável VI ou VF (Volume armazenado nos reservatórios)
						if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_util) {

							if (a_periodo_estudo == a_horizonte_estudo_estagio.getIteradorFinal()) {

								if (getVarDecisao_VF(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica) > -1)
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica), posIneRHE, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, IdPatamarCarga_1, double()));

							}//if (a_periodo_estudo == a_horizonte_estudo.getIteradorFinal()) {
							else {

								Periodo periodo_seguinte = a_periodo_estudo;

								a_horizonte_estudo_estagio.incrementarIterador(periodo_seguinte); //O volume final do estágio t é igual ao volume inicial de t+1

								if (getVarDecisao_VI(a_TSS, a_idEstagio, periodo_seguinte, idHidreletrica) > -1)
									vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, periodo_seguinte, idHidreletrica), posIneRHE, a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, IdPatamarCarga_1, double()));

							}//else {

						}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_util) {

					} // if (idHidreletrica != IdHidreletrica_Nenhum) {
					else
						throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

				} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){


			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, a_periodo_estudo, IdPatamarCarga_1, double()) > 0) {

			//******************************************
			//Restrição >=
			//******************************************

			if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_periodo_estudo, IdPatamarCarga_1, double()) != getdoubleFromChar("max")) {

				if (getVarDecisao_RHE_FSUPseExistir(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE) > -1) {

					const int posIneRHE = addIneLinear_RESTRICAO_HIDRAULICA_ENERGIA_LIMITE_SUPERIOR(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE);

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posIneRHE, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_periodo_estudo, IdPatamarCarga_1, double()));

					// Variável RHE
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_RHE_FSUP(a_TSS, a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE), posIneRHE, 1.0);

					const IdElementoSistema maiorElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());
					for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorElementoSistema; idElementoSistema++) {

						const IdHidreletrica       idHidreletrica = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
						const IdUsinaElevatoria idUsinaElevatoria = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());

						const TipoVariavelRestricaoOperativa tipoVariavelRestricaoOperativa = a_dados.getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());

						if (idHidreletrica != IdHidreletrica_Nenhum) {

							// Variável VF ou VI (Volume armazenado nos reservatórios)
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_util) {

								if (a_periodo_estudo == a_horizonte_estudo_estagio.getIteradorFinal()) {

									if (getVarDecisao_VF(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VF(a_TSS, a_idEstagio, a_periodo_estudo, idHidreletrica), posIneRHE, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, IdPatamarCarga_1, double()));

								}//if (a_periodo_estudo == a_horizonte_estudo.getIteradorFinal()) {
								else {

									Periodo periodo_seguinte = a_periodo_estudo;

									a_horizonte_estudo_estagio.incrementarIterador(periodo_seguinte); //O volume final do estágio t é igual ao volume inicial de t+1

									if (getVarDecisao_VI(a_TSS, a_idEstagio, periodo_seguinte, idHidreletrica) > -1)
										vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_VI(a_TSS, a_idEstagio, periodo_seguinte, idHidreletrica), posIneRHE, -a_dados.getElementoMatriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao, a_periodo_estudo, IdPatamarCarga_1, double()));

								}//else {
							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_util) {

						} // if (idHidreletrica != IdHidreletrica_Nenhum) {
						else
							throw std::invalid_argument(getFullString(idElementoSistema) + " nao possui atributo valido para " + getFullString(idRestricaoOperativaUHE));

					} // for (IdElementoSistema idElementoSistema; idElementoSistema <= maiorElementoSistema; idElementoSistema++){

				} // if (getVarDecisao_RHEseExistir(a_idEstagio, a_periodo_estudo, idRestricaoOperativaUHE) > -1) {

			}//if (a_dados.getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, a_periodo_estudo, IdPatamarCarga_1, double()) < getdoubleFromChar("max")) {


		}// for (int pos = 0; pos < numero_restricoes; pos++) {

	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("criarRestricoesHidraulicaEspecial_energia_armazenada(" + getFullString(a_TSS) + ",Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados): \n" + std::string(erro.what())); }

}//void ModeloOtimizacao::criarRestricoesHidraulicaEspecial_energia_armazenada(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados)


void ModeloOtimizacao::criarRestricoesIntercambioHidraulicooRetiradaRelaxada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdPatamarCarga a_idPatamarCarga, const IdIntercambioHidraulico a_idIntercambioHidraulico) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const TipoIntercambioHidraulico tipo_intercambio_hidraulico = a_dados.getAtributo(a_idIntercambioHidraulico, AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

		const int posEquQOUT_REL = addEquLinear_INTERCAMBIO_HIDRAULICO_RETIRADA_RELAXADA(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idIntercambioHidraulico);

		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquQOUT_REL, 0.0);

		if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_retirada) {
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QOUT_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idIntercambioHidraulico, a_dados.getAtributo(a_idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posEquQOUT_REL, 1.0);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QOUT(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idIntercambioHidraulico, a_dados.getAtributo(a_idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posEquQOUT_REL, -1.0);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QOUT_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idIntercambioHidraulico, a_dados.getAtributo(a_idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica())), posEquQOUT_REL, 1.0);
		}

		else if (tipo_intercambio_hidraulico == TipoIntercambioHidraulico_pereira_barreto) {
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QILS_TRI_REL(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idIntercambioHidraulico), posEquQOUT_REL, 1.0);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QILS_TRI(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idIntercambioHidraulico), posEquQOUT_REL, -1.0);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setCofRestricao(getVarDecisao_QILS_TRI_FINF(a_TSS, a_idEstagio, a_periodo_estudo, a_idPatamarCarga, a_idIntercambioHidraulico), posEquQOUT_REL, 1.0);
		}

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarRestricoesIntercambioHidraulicooRetiradaRelaxada(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_periodo_estudo) + "," + getFullString(a_idIntercambioHidraulico) + "): \n" + std::string(erro.what())); }

}//void ModeloOtimizacao::criarRestricoesIntercambioHidraulicooRetiradaRelaxada(Dados& a_dados, const IdEstagio a_idEstagio, const IdIntercambioHidraulico a_idIntercambioHidraulico, const Periodo a_periodo_estudo, const IdPatamarCarga a_idPatamarCarga){

void ModeloOtimizacao::zerarVariaveisFolga(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdIntercambio a_maiorIdIntercambio, const IdRestricaoEletrica a_maiorIdRestricaoEletrica, const IdTermeletrica a_maiorIdTermeletrica, const IdRestricaoOperativaUHE a_maiorIdRestricaoOperativaUHE, const IdIntercambioHidraulico a_maiorIdIntercambioHidraulico, const IdUsinaElevatoria a_maiorIdUsinaElevatoria, const SmartEnupla<Periodo, double>a_horizonte_estudo_estagio) {

	try {

		if (a_TSS == TipoSubproblemaSolver_mestre)
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		for (Periodo periodo_estudo = a_horizonte_estudo_estagio.getIteradorInicial(); periodo_estudo <= a_horizonte_estudo_estagio.getIteradorFinal(); a_horizonte_estudo_estagio.incrementarIterador(periodo_estudo)) {

			//Variáveis de folga associadas às hidrelétricas
			for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); a_dados.vetorHidreletrica.incr(idHidreletrica)) {

				//*********************
				//VARIAVEL_DECISAO_3
				//*********************

				
				if (getVarDecisao_QEV_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica) > -1) {
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QEV_FINF(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica), 0.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QEV_FINF(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica), 0.0);
				}//if (getVarDecisao_QEV_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica) > -1) {
				
				////////////

				if (getVarDecisao_QDEF_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica) > -1) {
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QDEF_FINF(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica), 0.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QDEF_FINF(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica), 0.0);
				}//if (getVarDecisao_QDEF_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica) > -1) {

				if (getVarDecisao_QDEF_FSUPseExistir(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica) > -1) {
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QDEF_FSUP(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica), 0.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QDEF_FSUP(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica), 0.0);
				}//if (getVarDecisao_QDEF_FSUPseExistir(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica) > -1) {

				////////////

				if (getVarDecisao_VF_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica) > -1) {
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_VF_FINF(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica), 0.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_VF_FINF(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica), 0.0);
				}//if (getVarDecisao_VF_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica) > -1) {

				if (getVarDecisao_VMORTO_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica) > -1) {
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_VMORTO_FINF(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica), 0.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_VMORTO_FINF(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica), 0.0);
				}//if (getVarDecisao_VMORTO_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idHidreletrica) > -1) {

				//*********************
				//VARIAVEL_DECISAO_4
				//*********************

				for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_estudo, IdPatamarCarga()); idPatamarCarga++) {

					if (getVarDecisao_PH_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica) > -1) {
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PH_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica), 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PH_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica), 0.0);
					}//if (getVarDecisao_PH_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica) > -1) {

					////////////

					if (getVarDecisao_PHDISP_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica) > -1) {
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PHDISP_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica), 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PHDISP_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica), 0.0);
					}//if (getVarDecisao_PHDISP_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica) > -1) {

					////////////

					if (getVarDecisao_QTURDISP_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica) > -1) {
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QTURDISP_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica), 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QTURDISP_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica), 0.0);
					}//if (getVarDecisao_QTURDISP_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica) > -1) {

					////////////

					if (getVarDecisao_QTUR_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica) > -1) {
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QTUR_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica), 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QTUR_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica), 0.0);
					}//if (getVarDecisao_QTUR_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica) > -1) {

					////////////

					if (getVarDecisao_QDEF_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica) > -1) {
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QDEF_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica), 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QDEF_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica), 0.0);
					}//if (getVarDecisao_QDEF_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica) > -1) {

					if (getVarDecisao_QDEF_FSUPseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica) > -1) {
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QDEF_FSUP(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica), 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QDEF_FSUP(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica), 0.0);
					}//if (getVarDecisao_QDEF_FSUPseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica) > -1) {

					////////////

					if (getVarDecisao_QOUT_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica) > -1) {
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QOUT_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica), 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QOUT_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica), 0.0);
					}//if (getVarDecisao_QOUT_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica) > -1) {


					//*********************
					//VARIAVEL_DECISAO_5
					//*********************

					for (IdHidreletrica idHidreletrica_aux = a_dados.getMenorId(IdHidreletrica()); idHidreletrica_aux <= a_dados.getMaiorId(IdHidreletrica()); a_dados.vetorHidreletrica.incr(idHidreletrica_aux)) {

						if (getVarDecisao_QDES_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica, idHidreletrica_aux) > -1) {
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QDES_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica, idHidreletrica_aux), 0.0);
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QDES_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica, idHidreletrica_aux), 0.0);
						}//if (getVarDecisao_QOUT_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica) > -1) {

					}//for (IdHidreletrica idHidreletrica_aux = a_dados.getMenorId(IdHidreletrica()); idHidreletrica_aux <= a_dados.getMaiorId(IdHidreletrica()); a_dados.vetorHidreletrica.incr(idHidreletrica_aux)) {


				}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo_estudo, IdPatamarCarga()); idPatamarCarga++) {


			}//for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			//Variáveis de folga associadas às termelétricas
			for (IdTermeletrica idTermeletrica = a_dados.getMenorId(IdTermeletrica()); idTermeletrica <= a_maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idTermeletrica)) {

				//*********************
				//VARIAVEL_DECISAO_4
				//*********************

				for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_estudo, IdPatamarCarga()); idPatamarCarga++) {

					if (getVarDecisao_PT_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica) > -1) {
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PT_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica), 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PT_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica), 0.0);
					}//if (getVarDecisao_PT_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica) > -1) {

					if (getVarDecisao_PT_FSUPseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica) > -1) {
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PT_FSUP(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica), 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PT_FSUP(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica), 0.0);
					}//if (getVarDecisao_PT_FSUPseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica) > -1) {

					//////////////

					if (getVarDecisao_PTDISP_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica) > -1) {
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PTDISP_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica), 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PTDISP_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica), 0.0);
					}//if (getVarDecisao_PTDISP_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica) > -1) {

					if (getVarDecisao_PTDISP_FSUPseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica) > -1) {
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PTDISP_FSUP(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica), 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PTDISP_FSUP(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica), 0.0);
					}//if (getVarDecisao_PTDISP_FSUPseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica) > -1) {

					//////////////

					if (getVarDecisao_PTDISPCOM_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica) > -1) {
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PTDISPCOM_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica), 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PTDISPCOM_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica), 0.0);
					}//if (getVarDecisao_PTDISPCOM_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica) > -1) {

					if (getVarDecisao_PTDISPCOM_FSUPseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica) > -1) {
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PTDISPCOM_FSUP(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica), 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PTDISPCOM_FSUP(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica), 0.0);
					}//if (getVarDecisao_PTDISPCOM_FSUPseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idTermeletrica) > -1) {

				}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_estudo, IdPatamarCarga()); idPatamarCarga++) {

			}//for (IdTermeletrica idTermeletrica = a_dados.getMenorId(IdTermeletrica()); idTermeletrica <= a_maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idTermeletrica)) {

			//Variáveis de folga associadas às usinas elevatórias
			for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria <= a_maiorIdUsinaElevatoria; idUsinaElevatoria++) {

				//*********************
				//VARIAVEL_DECISAO_4
				//*********************

				for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_estudo, IdPatamarCarga()); idPatamarCarga++) {

					if (getVarDecisao_QBOM_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idUsinaElevatoria) > -1) {
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QBOM_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idUsinaElevatoria), 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QBOM_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idUsinaElevatoria), 0.0);
					}//if (getVarDecisao_QBOM_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idUsinaElevatoria) > -1) {

					/////////////

					if (getVarDecisao_QBOMDISP_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idUsinaElevatoria) > -1) {
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QBOMDISP_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idUsinaElevatoria), 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QBOMDISP_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idUsinaElevatoria), 0.0);
					}//if (getVarDecisao_QBOMDISP_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idUsinaElevatoria) > -1) {

				}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_estudo, IdPatamarCarga()); idPatamarCarga++) {

			}//for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria <= a_dados.getMaiorId(IdUsinaElevatoria()); idUsinaElevatoria++) {

			//Variáveis de folga associadas às restrições hidráulicas
			for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= a_maiorIdRestricaoOperativaUHE; idRestricaoOperativaUHE++) {

				//*********************
				//VARIAVEL_DECISAO_3
				//*********************

				if (getVarDecisao_RHA_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE) > -1) {
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_RHA_FINF(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE), 0.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_RHA_FINF(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE), 0.0);
				}//if (getVarDecisao_RHA_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE) > -1) {

				if (getVarDecisao_RHA_FSUPseExistir(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE) > -1) {
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_RHA_FSUP(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE), 0.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_RHA_FSUP(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE), 0.0);
				}//if (getVarDecisao_RHA_FSUPseExistir(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE) > -1) {

				/////////////

				if (getVarDecisao_RHV_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE) > -1) {
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_RHV_FINF(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE), 0.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_RHV_FINF(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE), 0.0);
				}//if (getVarDecisao_RHV_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE) > -1) {

				if (getVarDecisao_RHV_FSUPseExistir(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE) > -1) {
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_RHV_FSUP(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE), 0.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_RHV_FSUP(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE), 0.0);
				}//if (getVarDecisao_RHV_FSUPseExistir(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE) > -1) {

				/////////////

				if (getVarDecisao_RHE_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE) > -1) {
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_RHE_FINF(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE), 0.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_RHE_FINF(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE), 0.0);
				}//if (getVarDecisao_RHE_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE) > -1) {

				if (getVarDecisao_RHE_FSUPseExistir(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE) > -1) {
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_RHE_FSUP(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE), 0.0);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_RHE_FSUP(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE), 0.0);
				}//if (getVarDecisao_RHE_FSUPseExistir(a_TSS, a_idEstagio, periodo_estudo, idRestricaoOperativaUHE) > -1) {


				//*********************
				//VARIAVEL_DECISAO_4
				//*********************

				for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_estudo, IdPatamarCarga()); idPatamarCarga++) {

					if (getVarDecisao_RHQ_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idRestricaoOperativaUHE) > -1) {
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_RHQ_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idRestricaoOperativaUHE), 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_RHQ_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idRestricaoOperativaUHE), 0.0);
					}//if (getVarDecisao_RHQ_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idRestricaoEletrica) > -1) {

					if (getVarDecisao_RHQ_FSUPseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idRestricaoOperativaUHE) > -1) {
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_RHQ_FSUP(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idRestricaoOperativaUHE), 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_RHQ_FSUP(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idRestricaoOperativaUHE), 0.0);
					}//if (getVarDecisao_RHQ_FSUPseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idRestricaoEletrica) > -1) {

				}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_estudo, IdPatamarCarga()); idPatamarCarga++) {

			} // for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= maiorIdRestricaoOperativaUHE; idRestricaoOperativaUHE++) {

			//Variáveis de folga associadas às restrições elétricas
			for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= a_maiorIdRestricaoEletrica; idRestricaoEletrica++) {

				//*********************
				//VARIAVEL_DECISAO_4
				//*********************

				for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_estudo, IdPatamarCarga()); idPatamarCarga++) {

					if (getVarDecisao_RE_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idRestricaoEletrica) > -1) {
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_RE_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idRestricaoEletrica), 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_RE_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idRestricaoEletrica), 0.0);
					}//if (getVarDecisao_RE_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idRestricaoEletrica) > -1) {

					if (getVarDecisao_RE_FSUPseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idRestricaoEletrica) > -1) {
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_RE_FSUP(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idRestricaoEletrica), 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_RE_FSUP(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idRestricaoEletrica), 0.0);
					}//if (getVarDecisao_RE_FSUPseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idRestricaoEletrica) > -1) {

					////////////

				}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_estudo, IdPatamarCarga()); idPatamarCarga++) {


			}//for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= a_maiorIdRestricaoEletrica; idRestricaoEletrica++) {

			//Variáveis de folga associadas a intercambios
			for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= a_maiorIdIntercambio; idIntercambio++) {

				//*********************
				//VARIAVEL_DECISAO_4
				//*********************

				for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_estudo, IdPatamarCarga()); idPatamarCarga++) {

					if (getVarDecisao_PI_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idIntercambio) > -1) {
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_PI_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idIntercambio), 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_PI_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idIntercambio), 0.0);
					}//if (getVarDecisao_PI_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idHidreletrica) > -1) {

				}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_estudo, IdPatamarCarga()); idPatamarCarga++) {

			}//for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= a_maiorIdIntercambio; idIntercambio++) {

			//Variáveis de folga associadas a intercambios hidráulicos
			for (IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico_1; idIntercambioHidraulico <= a_maiorIdIntercambioHidraulico; idIntercambioHidraulico++) {

				//*********************
				//VARIAVEL_DECISAO_4
				//*********************

				for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_estudo, IdPatamarCarga()); idPatamarCarga++) {

					if (getVarDecisao_QILS_TRI_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idIntercambioHidraulico) > -1) {
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QILS_TRI_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idIntercambioHidraulico), 0.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QILS_TRI_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idIntercambioHidraulico), 0.0);
					}//if (getVarDecisao_QILS_TRI_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idIntercambioHidraulico) > -1) {

					//*********************
					//VARIAVEL_DECISAO_5
					//*********************

					for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); a_dados.vetorHidreletrica.incr(idHidreletrica)) {

						if (getVarDecisao_QOUT_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idIntercambioHidraulico, idHidreletrica) > -1) {
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(getVarDecisao_QOUT_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idIntercambioHidraulico, idHidreletrica), 0.0);
							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(getVarDecisao_QOUT_FINF(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idIntercambioHidraulico, idHidreletrica), 0.0);
						}//if (getVarDecisao_QOUT_FINFseExistir(a_TSS, a_idEstagio, periodo_estudo, idPatamarCarga, idIntercambioHidraulico, idHidreletrica) > -1) {

					}//for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); a_dados.vetorHidreletrica.incr(idHidreletrica)) {

				}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_estudo, IdPatamarCarga()); idPatamarCarga++) {

			}//for (IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico_1; idIntercambioHidraulico <= a_maiorIdIntercambioHidraulico; idIntercambioHidraulico++) {

		}//for (Periodo a_periodo_estudo = a_horizonte_estudo_estagio.getIteradorInicial(); a_periodo_estudo <= a_horizonte_estudo_estagio.getIteradorFinal(); a_horizonte_estudo_estagio.incrementarIterador(a_periodo_estudo)) {

	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::zerarVariaveisFolga(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
}

