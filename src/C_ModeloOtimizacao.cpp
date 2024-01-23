#include "C_ModeloOtimizacao.h"

#include "C_EntradaSaidaDados.h"

#include "C_ProcessoEstocastico.h"

#include <time.h>

#include <stdio.h>

#include "mpi.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ModeloOtimizacao::ModeloOtimizacao(const IdModeloOtimizacao a_idModeloOtimizacao, Dados &a_dados, EntradaSaidaDados a_entradaSaidaDados) {

	try { 
		   
		lista_VarDecisao_instanciadas = std::vector<std::vector<std::string>>(TipoSubproblemaSolver_Excedente, std::vector<std::string>());
		lista_EquLinear_instanciadas = std::vector<std::vector<std::string>>(TipoSubproblemaSolver_Excedente, std::vector<std::string>());
		lista_IneLinear_instanciadas = std::vector<std::vector<std::string>>(TipoSubproblemaSolver_Excedente, std::vector<std::string>());

		contadorLog10005 = 0;
		escreverLog10005(TipoSubproblemaSolver_Nenhum, IdIteracao_Nenhum, IdEstagio_Nenhum, IdCenario_Nenhum, IdRealizacao_Nenhum, std::string());

		INICIALIZA_SMART_ELEMENTO(ModeloOtimizacao, SMART_ELEMENTO_MODELO_OTIMIZACAO)

		setAtributo(AttComumModeloOtimizacao_idModeloOtimizacao, a_idModeloOtimizacao);

		criarModeloOtimizacao(a_dados, a_entradaSaidaDados);

		a_dados.esvaziar();

	}
	catch (const std::exception&erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::ModeloOtimizacao(" + getString(a_idModeloOtimizacao) + ",a_dados,a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // ModeloOtimizacao::ModeloOtimizacao(const IdModeloOtimizacao a_idModeloOtimizacao, Dados &a_dados) {

ModeloOtimizacao::ModeloOtimizacao(const ModeloOtimizacao & instanciaCopiar){
	try {
		
		COPIA_SMART_ELEMENTO(ModeloOtimizacao, SMART_ELEMENTO_MODELO_OTIMIZACAO)

		throw std::invalid_argument("Uma instancia de ModeloOtimizacao nao eh passivel de copia.");
	
	}
	catch (const std::exception&erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::ModeloOtimizacao(const ModeloOtimizacao &instanciaCopiar): \n" + std::string(erro.what())); }

}

void ModeloOtimizacao::esvaziar() {
	try {

		EXCLUI_SMART_ELEMENTO(ModeloOtimizacao, SMART_ELEMENTO_MODELO_OTIMIZACAO)

	}
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::esvaziar(): \n" + std::string(erro.what())); }

}
//////////////////////////////////////////////////////////////////////

ModeloOtimizacao::~ModeloOtimizacao() {

	try {
		EXCLUI_SMART_ELEMENTO(ModeloOtimizacao, SMART_ELEMENTO_MODELO_OTIMIZACAO)
	} // try
	catch (const std::exception& erro) { std::cout << "ModeloOtimizacao::~ModeloOtimizacao(): \n" << erro.what() << std::endl; }

} // ModeloOtimizacao::~ModeloOtimizacao() {


//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

void ModeloOtimizacao::instanciarCorteBenders(const IdEstagio a_idEstagio, const SmartEnupla<IdRealizacao, double> &a_rhs, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> &a_coeficiente, const SmartEnupla<IdVariavelEstado, double> &a_estado){

	try{ 

		SmartEnupla<IdCorteBenders, IdCorteBenders> mapeamento_cortes_sequenciados = vetorEstagio.att(a_idEstagio).sequenciarCorteBenders();

		if (mapeamento_cortes_sequenciados.size() > 0) {

			const IdEstagio estagio_anterior = IdEstagio(a_idEstagio - 1);

			for (TipoSubproblemaSolver tss = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tss < TipoSubproblemaSolver_Excedente; tss++) {

				if (idx_IneLinear_CB_ZF_4.at(tss).size() > 0) {
					if (idx_IneLinear_CB_ZF_4.at(tss).at(estagio_anterior).size() > 0) {

						for (Periodo periodo = idx_IneLinear_CB_ZF_4.at(tss).at(estagio_anterior).getIteradorInicial(); periodo <= idx_IneLinear_CB_ZF_4.at(tss).at(estagio_anterior).getIteradorFinal(); idx_IneLinear_CB_ZF_4.at(tss).at(estagio_anterior).incrementarIterador(periodo)) {
							if (idx_IneLinear_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).size() > 0) {
								for (IdRealizacao idReal = idx_IneLinear_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).getIteradorInicial(); idReal <= idx_IneLinear_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).getIteradorFinal(); idReal++) {

									if (idx_IneLinear_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).at(idReal).size() > 0) {

										SmartEnupla<IdEstagio, SmartEnupla <Periodo, SmartEnupla <IdRealizacao, SmartEnupla<IdCorteBenders, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>> idx_ine_CB_ZF_4_novo;
										
										for (IdCorteBenders idCorte = IdCorteBenders_1; idCorte <= mapeamento_cortes_sequenciados.getIteradorFinal(); idCorte++) {

											const IdCorteBenders idCorte_old = mapeamento_cortes_sequenciados.at(idCorte);

											if ((idx_IneLinear_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).at(idReal).getIteradorInicial() <= idCorte_old) && (idCorte_old <= idx_IneLinear_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).at(idReal).getIteradorFinal())) {
												if (idx_IneLinear_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).at(idReal).at(idCorte_old).size() > 0) {

													double idxCB = idx_IneLinear_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).at(idReal).at(idCorte_old).at(1).at(1).at(1).at(1).at(1).at(1).at(IdRealizacao_1).at(IdCenario_1);

													addConteudoIters_12(idx_ine_CB_ZF_4_novo, idxCB, estagio_anterior, periodo, idReal, idCorte, 1, 1, 1, 1, 1, 1, IdRealizacao_1, IdCenario_1);

													vetorEstagio.att(estagio_anterior).getSolver(tss)->setNomeRestricao(int(idxCB), getNomeIneLinear_CB_ZF(tss, estagio_anterior, periodo, idReal, idCorte));
												} // if (idx_IneLinear_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).at(idReal).at(idCorte_old).size() > 0) {
											}
										} // for (IdCorteBenders idCorte = idx_ine_CB_ZF_4_novo.getIteradorInicial(); idCorte <= idx_ine_CB_ZF_4_novo.getIteradorFinal(); idCorte++) {

										idx_IneLinear_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).at(idReal) = idx_ine_CB_ZF_4_novo.at(estagio_anterior).at(periodo).at(idReal);

									} // if (idx_IneLinear_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).at(idReal).size() > 0) {
								} // for (IdRealizacao idReal = idx_IneLinear_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).getIteradorInicial(); idReal <= idx_IneLinear_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).getIteradorFinal(); idReal++) {
							} // if (idx_IneLinear_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo) > 0) {
						} // for (Periodo periodo = idx_IneLinear_CB_ZF_4.at(tss).at(estagio_anterior).getIteradorInicial(); periodo <= idx_IneLinear_CB_ZF_4.at(tss).at(estagio_anterior).getIteradorFinal(); idx_IneLinear_CB_ZF_4.at(tss).at(estagio_anterior).incrementarIterador(periodo)) {
					} // if (idx_IneLinear_CB_ZF_4.at(tss).at(estagio_anterior).size() > 0){
				} // if (idx_IneLinear_CB_ZF_4.at(tss).size() > 0) {

				if (idx_IneLinear_CB_ZT_4.at(tss).size() > 0) {
					if (idx_IneLinear_CB_ZT_4.at(tss).at(a_idEstagio).size() > 0) {

						for (Periodo periodo = idx_IneLinear_CB_ZT_4.at(tss).at(a_idEstagio).getIteradorInicial(); periodo <= idx_IneLinear_CB_ZT_4.at(tss).at(a_idEstagio).getIteradorFinal(); idx_IneLinear_CB_ZT_4.at(tss).at(a_idEstagio).incrementarIterador(periodo)) {
							if (idx_IneLinear_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo).size() > 0) {
								for (IdRealizacao idReal = idx_IneLinear_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo).getIteradorInicial(); idReal <= idx_IneLinear_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo).getIteradorFinal(); idReal++) {

									if (idx_IneLinear_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo).at(idReal).size() > 0) {

										SmartEnupla<IdEstagio, SmartEnupla <Periodo, SmartEnupla <IdRealizacao, SmartEnupla<IdCorteBenders, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>> idx_ine_CB_ZT_4_novo;

										for (IdCorteBenders idCorte = IdCorteBenders_1; idCorte <= mapeamento_cortes_sequenciados.getIteradorFinal(); idCorte++) {

											const IdCorteBenders idCorte_old = mapeamento_cortes_sequenciados.at(idCorte);

											if ((idx_IneLinear_CB_ZT_4.at(tss).at(estagio_anterior).at(periodo).at(idReal).getIteradorInicial() <= idCorte_old) && (idCorte_old <= idx_IneLinear_CB_ZT_4.at(tss).at(estagio_anterior).at(periodo).at(idReal).getIteradorFinal())) {
												if (idx_IneLinear_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo).at(idReal).at(idCorte_old).size() > 0) {

													double idxCB = idx_IneLinear_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo).at(idReal).at(idCorte_old).at(1).at(1).at(1).at(1).at(1).at(1).at(IdRealizacao_1).at(IdCenario_1);

													addConteudoIters_12(idx_ine_CB_ZT_4_novo, idxCB, a_idEstagio, periodo, idReal, idCorte, 1, 1, 1, 1, 1, 1, IdRealizacao_1, IdCenario_1);

													vetorEstagio.att(a_idEstagio).getSolver(tss)->setNomeRestricao(int(idxCB), getNomeIneLinear_CB_ZT(tss, a_idEstagio, periodo, idReal, idCorte));

												} // if (idx_IneLinear_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo).at(idReal).at(idCorte_old).size() > 0) {
											}
										} // for (IdCorteBenders idCorte = idx_ine_CB_ZT_4_novo.getIteradorInicial(); idCorte <= idx_ine_CB_ZT_4_novo.getIteradorFinal(); idCorte++) {

										idx_IneLinear_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo).at(idReal) = idx_ine_CB_ZT_4_novo.at(estagio_anterior).at(periodo).at(idReal);

									} // if (idx_IneLinear_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo).at(idReal).size() > 0) {
								} // for (IdRealizacao idReal = idx_IneLinear_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo).getIteradorInicial(); idReal <= idx_IneLinear_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo).getIteradorFinal(); idReal++) {
							} // if (idx_IneLinear_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo) > 0) {
						} // for (Periodo periodo = idx_IneLinear_CB_ZT_4.at(tss).at(a_idEstagio).getIteradorInicial(); periodo <= idx_IneLinear_CB_ZT_4.at(tss).at(a_idEstagio).getIteradorFinal(); idx_IneLinear_CB_ZT_4.at(tss).at(a_idEstagio).incrementarIterador(periodo)) {
					} // if (idx_IneLinear_CB_ZT_4.at(tss).at(a_idEstagio).size() > 0){
				} // if (idx_IneLinear_CB_ZT_4.at(tss).size() > 0) {

			} // for (TipoSubproblemaSolver tss = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tss < TipoSubproblemaSolver_Excedente; tss++) {

		} // if (mapeamento_cortes_sequenciados.size() > 0) {

		vetorEstagio.att(a_idEstagio).instanciarCorteBenders(a_rhs, a_coeficiente, a_estado);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::instanciaCorteBenders(" + getFullString(a_idEstagio) + ",a_rhs,a_coeficiente,a_estado): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::addCorteBenders(const IdEstagio a_idEstagio, const SmartEnupla<IdRealizacao, double> &a_rhs, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> &a_coeficiente, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> &a_estado){

void ModeloOtimizacao::instanciarCorteBenders(const IdEstagio a_idEstagio, const SmartEnupla<IdRealizacao, double>& a_rhs, const SmartEnupla<IdRealizacao, double>& a_rhs_primal, const SmartEnupla<IdRealizacao, double>& a_rhs_estado, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>>& a_coeficiente, const SmartEnupla<IdVariavelEstado, double>& a_estado) {

	try {

		vetorEstagio.att(a_idEstagio).instanciarCorteBenders(a_rhs, a_rhs_primal, a_rhs_estado, a_coeficiente, a_estado);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::instanciaCorteBenders(" + getFullString(a_idEstagio) + ",a_rhs,a_rhs_primal,a_rhs_estado,a_coeficiente,a_estado): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::addCorteBenders(const IdEstagio a_idEstagio, const SmartEnupla<IdRealizacao, double> &a_rhs, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> &a_coeficiente, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> &a_estado){


void ModeloOtimizacao::removerCortesBendersDominados(const IdEstagio a_idEstagio, const IdProcesso a_idProcesso, EntradaSaidaDados a_entradaSaidaDados){

	try{

		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//Cortes_dominados");

		bool primeiro_corte = false;
		for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= getMaiorId(a_idEstagio, IdCorteBenders()); idCorteBenders++) {

			if (vetorEstagio.att(a_idEstagio).vetorCorteBenders.isInstanciado(idCorteBenders)) {

				if (getAtributo(a_idEstagio, idCorteBenders, AttComumCorteBenders_is_corte_dominado, int()) == 1) {

					if ((a_idProcesso == IdProcesso_mestre) && (getAtributo(AttComumModeloOtimizacao_imprimir_selecao_corte_por_iteracao, bool()))) {

						a_entradaSaidaDados.setAppendArquivo(primeiro_corte);

						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_idEstagio) + "_corteBenders_rhs.csv", idCorteBenders, vetorEstagio.att(a_idEstagio), AttVetorCorteBenders_rhs);
						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_idEstagio) + "_corteBenders_rhs_primal.csv", idCorteBenders, vetorEstagio.att(a_idEstagio), AttVetorCorteBenders_rhs_primal);
						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_idEstagio) + "_corteBenders_rhs_estado.csv", idCorteBenders, vetorEstagio.att(a_idEstagio), AttVetorCorteBenders_rhs_estado);
						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_idEstagio) + "_corteBenders_estado.csv", idCorteBenders, vetorEstagio.att(a_idEstagio), AttVetorCorteBenders_estado);
						a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz(getFullString(a_idEstagio) + "_corteBenders_coeficientes.csv", idCorteBenders, vetorEstagio.att(a_idEstagio), AttMatrizCorteBenders_coeficiente);

					} // if ((a_idProcesso == IdProcesso_mestre) && (getAtributo(AttComumModeloOtimizacao_imprimir_selecao_corte_por_iteracao, bool()))) {

					vetorEstagio.att(a_idEstagio).vetorCorteBenders.rem(idCorteBenders);

					if (!primeiro_corte)
						primeiro_corte = true;

				} // if (getAtributo(a_idEstagio, idCorteBenders, AttComumCorteBenders_is_corte_dominado, int())) {

			} // if (vetorEstagio.att(a_idEstagio).vetorCorteBenders.isInstanciado(idCorteBenders)) {

		} // for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= getMaiorId(a_idEstagio, IdCorteBenders()); idCorteBenders++) {

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::removerCortesBendersDominados(" + getFullString(a_idEstagio) + "," + getFullString(a_idProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::removerCortesBendersDominados(const IdEstagio a_idEstagio, EntradaSaidaDados a_entradaSaidaDados){


void ModeloOtimizacao::requestCorteBenders(const IdProcesso a_idProcesso, const IdEstagio a_idEstagio, const std::string a_diretorio, EntradaSaidaDados a_entradaSaidaDados){

	try {

		const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());

		const IdEstagio idEstagio_anterior = IdEstagio(a_idEstagio - 1);

		std::vector<TipoSubproblemaSolver> lista_subproblemaSolver; 

		if (a_idEstagio > getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio()))
			lista_subproblemaSolver = vetorEstagio.att(idEstagio_anterior).getListaTipoSubproblemaSolver();
		else
			lista_subproblemaSolver = vetorEstagio.att(a_idEstagio).getListaTipoSubproblemaSolver();

		if ((idEstagio_anterior < estagio_inicial) && (!vetorEstagio.att(a_idEstagio).isSolverInstanciado(TipoSubproblemaSolver_mestre)))
			return;

		if (lista_subproblemaSolver.size() == 0)
			return;

		selecionarCorteBenders(a_idEstagio);

		a_entradaSaidaDados.setDiretorioSaida(a_diretorio);

		if (a_idProcesso == IdProcesso_mestre) {

			if (getAtributo(AttComumModeloOtimizacao_imprimir_selecao_corte_por_iteracao, bool())) {

				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("corteBenders_dominantes_" + getFullString(a_idEstagio) + ".csv", a_idEstagio, IdCorteBenders_Nenhum, *this, std::vector<AttMatrizCorteBenders>{AttMatrizCorteBenders_custo_cortes_dominantes, AttMatrizCorteBenders_ordenacao_cortes_dominantes});

				a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("corteBenders_request_" + getFullString(a_idEstagio) + ".csv", a_idEstagio, *this, std::vector<AttVetorEstagio>{AttVetorEstagio_request_cortes_selecionados});

				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("corteBenders_selecionados_" + getFullString(a_idEstagio) + ".csv", a_idEstagio, *this, std::vector<AttMatrizEstagio>{AttMatrizEstagio_cortes_selecionados, AttMatrizEstagio_acao_cortes_selecionados});
				
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("corteBenders_deselecionados_" + getFullString(a_idEstagio) + ".csv", a_idEstagio, *this, AttMatrizEstagio_cortes_deselecionados);

			} // if (a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_imprimir_selecao_corte_por_iteracao, bool())) {

		}

		if (getSize1Matriz(a_idEstagio, AttMatrizEstagio_cortes_selecionados) > 0) {

			const IdRealizacao maiorIdRealizacao = getIterador1Final(a_idEstagio, AttMatrizEstagio_cortes_selecionados, IdRealizacao());

			for (IdRealizacao idRealizacao = maiorIdRealizacao; idRealizacao >= IdRealizacao_1; idRealizacao--) {

				const int request_cortes_selecionados = getElementoVetor(a_idEstagio, AttVetorEstagio_request_cortes_selecionados, idRealizacao, int());

				if (request_cortes_selecionados == 1) {

					const int numero_cortes_selecionados = getIterador2Final(a_idEstagio, AttMatrizEstagio_cortes_selecionados, idRealizacao, int());

					for (int i = numero_cortes_selecionados; i >= 1; i--) {

						const int acao_corte_selecionado = getElementoMatriz(a_idEstagio, AttMatrizEstagio_acao_cortes_selecionados, idRealizacao, i, int());

						if (acao_corte_selecionado == 0) {

							const IdCorteBenders idCorteBenders = getElementoMatriz(a_idEstagio, AttMatrizEstagio_cortes_selecionados, idRealizacao, i, IdCorteBenders());

							const IdRealizacao menorIdRealizacao_corte = getIterador1Inicial(a_idEstagio, idCorteBenders, AttMatrizCorteBenders_coeficiente, IdRealizacao());
							const IdRealizacao maiorIdRealizacao_corte = getIterador1Final(a_idEstagio, idCorteBenders, AttMatrizCorteBenders_coeficiente, IdRealizacao());

							if ((menorIdRealizacao_corte <= idRealizacao) && (idRealizacao <= maiorIdRealizacao_corte)) {

								if (getSize1Matriz(a_idEstagio, idCorteBenders, AttMatrizCorteBenders_coeficiente) > 0) {
									if (getSize2Matriz(a_idEstagio, idCorteBenders, AttMatrizCorteBenders_coeficiente, idRealizacao) > 0) {
										for (int i = 0; i < int(lista_subproblemaSolver.size()); i++) {
											if ((idEstagio_anterior >= estagio_inicial) && (lista_subproblemaSolver.at(i) != TipoSubproblemaSolver_mestre))
												addCorteBendersToZF(lista_subproblemaSolver.at(i), a_idEstagio, idEstagio_anterior, idRealizacao, idCorteBenders);
											if (lista_subproblemaSolver.at(i) == TipoSubproblemaSolver_mestre)
												addCorteBendersToZT(lista_subproblemaSolver.at(i), a_idEstagio, idRealizacao, idCorteBenders);
										}
									} // if (getSize2Matriz(a_idEstagio, idCorteBenders, AttMatrizCorteBenders_coeficiente, idRealizacao) > 0) {
								} // if (getSize1Matriz(a_idEstagio, idCorteBenders, AttMatrizCorteBenders_coeficiente) > 0) {

							} // if ((menorIdRealizacao_corte <= idRealizacao) && (idRealizacao <= maiorIdRealizacao_corte)) {

						} // if (acao_corte_selecionado > -1) {

					} // for (int i = 1; i <= numero_cortes_selecionados; i++) {

					if (getSize2Matriz(a_idEstagio, AttMatrizEstagio_cortes_deselecionados, idRealizacao) > 0) {

						const int numero_cortes_deselecionados = getIterador2Final(a_idEstagio, AttMatrizEstagio_cortes_deselecionados, idRealizacao, int());

						for (int i = 1; i <= numero_cortes_deselecionados; i++) {
							for (int j = 0; j < int(lista_subproblemaSolver.size()); j++) {
								if ((idEstagio_anterior >= estagio_inicial) && (lista_subproblemaSolver.at(j) != TipoSubproblemaSolver_mestre))
									remCorteBendersFromZF(lista_subproblemaSolver.at(j), a_idEstagio, idEstagio_anterior, idRealizacao, getElementoMatriz(a_idEstagio, AttMatrizEstagio_cortes_deselecionados, idRealizacao, i, IdCorteBenders()));
								if (lista_subproblemaSolver.at(j) == TipoSubproblemaSolver_mestre)
									remCorteBendersFromZT(lista_subproblemaSolver.at(j), a_idEstagio, idRealizacao, getElementoMatriz(a_idEstagio, AttMatrizEstagio_cortes_deselecionados, idRealizacao, i, IdCorteBenders()));
							}
						}
					} // if (getSize2Matriz(a_idEstagio, AttMatrizEstagio_cortes_deselecionados, idRealizacao) > 0) {

					requestCorteBendersFinalizado(a_idEstagio, idRealizacao);

				} // if (request_cortes_selecionados == 1) {

			} // for (IdRealizacao idRealizacao = maiorIdRealizacao; idRealizacao >= IdRealizacao_1; idRealizacao--) {

		} // if (getSize1Matriz(a_idEstagio, AttMatrizEstagio_cortes_selecionados) > 0) {

		if ((getAtributo(AttComumModeloOtimizacao_remover_cortes_dominados, bool())) && (a_idEstagio != getAtributo(AttComumModeloOtimizacao_estagio_acoplamento_pre_estudo, IdEstagio())))
			removerCortesBendersDominados(a_idEstagio, a_idProcesso, a_entradaSaidaDados);

	} // try {
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::requestCorteBenders(" + getFullString(a_idProcesso) + "," + getFullString(a_idEstagio) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::requestCorteBenders(const IdProcesso a_idProcesso, const IdEstagio a_idEstagio, const std::string a_diretorio, EntradaSaidaDados a_entradaSaidaDados){


void ModeloOtimizacao::requestCorteBenders(const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders, double & a_rhs, SmartEnupla<IdVariavelEstado, double>& a_coeficiente){

	try { vetorEstagio.att(a_idEstagio).requestCorteBenders(a_idRealizacao, a_idCorteBenders, a_rhs, a_coeficiente); } // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::requestCorteBenders(" + getFullString(a_idEstagio) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCorteBenders) + ",a_rhs,a_coeficiente,a_estado): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::requestCorteBenders(const IdEstagio a_idEstagio, double & a_rhs, const SmartEnupla<IdVariavelEstado, double>& a_coeficiente){


void ModeloOtimizacao::requestCorteBendersFinalizado(const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao){

	try { vetorEstagio.att(a_idEstagio).requestCorteBendersFinalizado(a_idRealizacao); } // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::requestCorteBendersFinalizado(" + getFullString(a_idEstagio) + "," + getFullString(a_idRealizacao) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::requestCorteBendersFinalizado(const IdRealizacao a_idRealizacao){

void ModeloOtimizacao::selecionarCorteBenders(const IdEstagio a_idEstagio){
	try { vetorEstagio.att(a_idEstagio).selecionarCorteBenders(); } // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::selecionarCorteBenders(" + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
} // void ModeloOtimizacao::selecionarCorteBenders(const IdEstagio a_idEstagio){



void ModeloOtimizacao::gerarRealizacoes(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, EntradaSaidaDados a_entradaSaidaDados){

	try {

		const IdProcessoEstocastico idPE = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

		bool algum_truncamento = false;
		for (IdEstagio idEstagio = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio()); idEstagio <= getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio()); idEstagio++) {
			if (getElementoVetor(AttVetorModeloOtimizacao_alguma_variavel_aleatoria_hidrologica_com_truncamento, idEstagio, int()) == 1) {
				algum_truncamento = true;
				break;
			}
		}

		const bool viabilidade_hidraulica = getAtributo(AttComumModeloOtimizacao_relaxar_afluencia_incremental_com_viabilidade_hidraulica, bool());

		const bool imprimir_cenarios = getAtributo(AttComumModeloOtimizacao_imprimir_cenario_hidrologico_pre_otimizacao, bool());

		if (imprimir_cenarios || algum_truncamento || viabilidade_hidraulica) {

			const IdCenario menor_cenario_iteracao = arranjoResolucao.getAtributo(a_idIteracao, AttComumIteracao_menor_cenario, IdCenario());
			const IdCenario maior_cenario_iteracao = arranjoResolucao.getAtributo(a_idIteracao, AttComumIteracao_maior_cenario, IdCenario());

			int semente_geracao_cenario_hidrologico = -1;

			bool gerar_cenarios_internos = false;
			if ((imprimir_cenarios) && (a_idProcesso == IdProcesso_mestre)) {
				gerar_cenarios_internos = true;
				a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//ProcessoEstocasticoHidrologico");
			}

			vetorProcessoEstocastico.att(idPE).gerarCenariosPorSorteio(a_entradaSaidaDados, imprimir_cenarios, true, gerar_cenarios_internos, getAtributo(AttComumModeloOtimizacao_numero_cenarios, int()), menor_cenario_iteracao, maior_cenario_iteracao, TipoSorteio_uniforme, semente_geracao_cenario_hidrologico);

		}

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::gerarRealizacoes(" + getFullString(a_idIteracao) + "," + getFullString(a_idProcesso) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::gerarRealizacoes(const a_idIteracao, const IdProcesso a_idProcesso, EntradaSaidaDados a_entradaSaidaDados){


void ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelEstado(const IdEstagio a_idEstagio, const IdCenario a_idCenario){

	try {

		EstruturaResultados<double> retorno_estados;

		atualizarModeloOtimizacaoComVariavelEstado(a_idEstagio, a_idCenario, retorno_estados);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::atualizarModeloOtimizacaoComVariavelEstado(" + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelEstado(const IdEstagio a_idEstagio, const IdCenario a_idCenario){

void ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelEstado(const IdEstagio a_idEstagio, const IdCenario a_idCenario, EstruturaResultados<double>& a_retorno_estados){

	try {

		const IdVariavelEstado maiorIdVariavelEstado = getMaiorId(a_idEstagio, IdVariavelEstado());

		if (maiorIdVariavelEstado == IdVariavelEstado_Nenhum)
			return;

		const IdEstagio estagio_anterior = IdEstagio(a_idEstagio - 1);

		for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

			const TipoSubproblemaSolver tSS_anterior = getAtributo(a_idEstagio, idVariavelEstado, AttComumVariavelEstado_tipoSubproblemaSolverEstagioAnterior, TipoSubproblemaSolver());

			const double valor = getElementoVetor(a_idEstagio, idVariavelEstado, AttVetorVariavelEstado_valor, a_idCenario, double());

			if (a_retorno_estados.total > 0)
				a_retorno_estados.add(valor);

			for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++) {

				if ((tSS == TipoSubproblemaSolver_mestre) && (vetorEstagio.att(a_idEstagio).isSolverInstanciado(tSS))) {
					if (getElementoVetor(a_idEstagio, idVariavelEstado, AttVetorVariavelEstado_idVariavelDecisao, tSS, int()) == -1)
						vetorEstagio.att(a_idEstagio).addVariavelEstado(tSS, getAtributo(a_idEstagio, idVariavelEstado, AttComumVariavelEstado_nome, string()), addVarDecisao_ESTADO(tSS, a_idEstagio, idVariavelEstado, 0.0, 0.0, 0.0), -1);
				} // if ((tSS == TipoSubproblemaSolver_mestre) && (vetorEstagio.att(a_idEstagio).isSolverInstanciado(tSS))) {

				const int idVariavelDecisao = getElementoVetor(a_idEstagio, idVariavelEstado, AttVetorVariavelEstado_idVariavelDecisao, tSS, int());

				if (idVariavelDecisao > -1) {

					vetorEstagio.att(a_idEstagio).getSolver(tSS)->setLimInferior(idVariavelDecisao, valor);
					vetorEstagio.att(a_idEstagio).getSolver(tSS)->setLimSuperior(idVariavelDecisao, valor);

				} // if (idVariavelDecisao > -1) {

			}  // for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++) {

		} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::atualizarModeloOtimizacaoComVariavelEstado(" + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }


} // void ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelEstado(const IdEstagio a_idEstagio, const IdProcesso a_idProcesso, const IdCenario a_idCenario, int& a_indice, double* a_array){


void ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelRealizacao(const IdEstagio a_idEstagio, const IdCenario a_idCenario) {

	try {

		const IdVariavelRealizacao maiorIdVariavelRealizacao = getMaiorId(a_idEstagio, IdVariavelRealizacao());

		for (IdVariavelRealizacao idVariavelRealizacao = IdVariavelRealizacao_1; idVariavelRealizacao <= maiorIdVariavelRealizacao; idVariavelRealizacao++) {

			const Periodo periodo_horizonte_processo_estocastico = getAtributo(a_idEstagio, idVariavelRealizacao, AttComumVariavelRealizacao_periodo, Periodo());

			const IdVariavelAleatoria   idVariavelAleatoria = getAtributo(a_idEstagio, idVariavelRealizacao, AttComumVariavelRealizacao_idVariavelAleatoria, IdVariavelAleatoria());

			const IdProcessoEstocastico idProcessoEstocastico = getAtributo(a_idEstagio, idVariavelRealizacao, AttComumVariavelRealizacao_idProcessoEstocastico, IdProcessoEstocastico());

			const IdRealizacao idRealizacao = getElementoMatriz(idProcessoEstocastico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, periodo_horizonte_processo_estocastico, IdRealizacao());

			const double valor = getElementoMatriz(idProcessoEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodo_horizonte_processo_estocastico, idRealizacao, double());

			for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++) {

				const int idVariavelDecisao = getElementoVetor(a_idEstagio, idVariavelRealizacao, AttVetorVariavelRealizacao_idVariavelDecisao, tSS, int());

				if (idVariavelDecisao > -1) {

					vetorEstagio.att(a_idEstagio).getSolver(tSS)->setLimInferior(idVariavelDecisao, valor);
					vetorEstagio.att(a_idEstagio).getSolver(tSS)->setLimSuperior(idVariavelDecisao, valor);

					if (idVariavelRealizacao == maiorIdVariavelRealizacao)
						vetorEstagio.att(a_idEstagio).getSolver(tSS)->atualizar();

				} // if (idVariavelDecisao > -1) {

			} // for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++) {

		} // for (IdVariavelRealizacao idVariavelRealizacao = IdVariavelRealizacao_1; idVariavelRealizacao <= maiorIdVariavelRealizacao; idVariavelRealizacao++) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::atualizarModeloOtimizacaoComVariavelRealizacao(" + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelRealizacao(const IdEstagio a_idEstagio, const IdCenario a_idCenario){


void ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelRealizacao(const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao){

	try {

		const IdVariavelRealizacao maiorIdVariavelRealizacao = getMaiorId(a_idEstagio, IdVariavelRealizacao());

		for (IdVariavelRealizacao idVariavelRealizacao = IdVariavelRealizacao_1; idVariavelRealizacao <= maiorIdVariavelRealizacao; idVariavelRealizacao++) {

			const Periodo periodo_horizonte_processo_estocastico = getAtributo(a_idEstagio, idVariavelRealizacao, AttComumVariavelRealizacao_periodo, Periodo());

			const IdVariavelAleatoria     idVariavelAleatoria = getAtributo(a_idEstagio, idVariavelRealizacao, AttComumVariavelRealizacao_idVariavelAleatoria, IdVariavelAleatoria());
			const IdProcessoEstocastico idProcessoEstocastico = getAtributo(a_idEstagio, idVariavelRealizacao, AttComumVariavelRealizacao_idProcessoEstocastico, IdProcessoEstocastico());

			const double valor = getElementoMatriz(idProcessoEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodo_horizonte_processo_estocastico, a_idRealizacao, double());

			for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++) {

				const int idVariavelDecisao = getElementoVetor(a_idEstagio, idVariavelRealizacao, AttVetorVariavelRealizacao_idVariavelDecisao, tSS, int());

				if (idVariavelDecisao > -1) {

					vetorEstagio.att(a_idEstagio).getSolver(tSS)->setLimInferior(idVariavelDecisao, valor);
					vetorEstagio.att(a_idEstagio).getSolver(tSS)->setLimSuperior(idVariavelDecisao, valor);

					if (idVariavelRealizacao == maiorIdVariavelRealizacao)
						vetorEstagio.att(a_idEstagio).getSolver(tSS)->atualizar();

				} // if (idVariavelDecisao > -1) {

			} // for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++) {

		} // for (IdVariavelRealizacao idVariavelRealizacao = IdVariavelRealizacao_1; idVariavelRealizacao <= maiorIdVariavelRealizacao; idVariavelRealizacao++) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::atualizarModeloOtimizacaoComVariavelRealizacao(" + getFullString(a_idEstagio) + "," + getFullString(a_idRealizacao) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelRealizacao(const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao){



void ModeloOtimizacao::atualizarModeloOtimizacaoComRestricaoCenario(const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) {

	try {

		const Periodo periodo_PE = getIterador2Final(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, a_idEstagio, Periodo());

		const IdProcessoEstocastico idProcessoEstocastico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

		const IdCenario menor_cenario = getIterador1Inicial(idProcessoEstocastico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario());
		const IdCenario maior_cenario = getIterador1Final(idProcessoEstocastico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario());

		SmartEnupla<Periodo, IdRealizacao> horizonte_espaco_amostral = getElementosMatriz(idProcessoEstocastico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, menor_cenario, Periodo(), IdRealizacao());

		IdCenario idCenario_mapeamento = IdCenario_Nenhum;

		for (IdCenario idCenario = menor_cenario; idCenario <= maior_cenario; idCenario++) {

			if (a_idRealizacao == getElementoMatriz(idProcessoEstocastico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, idCenario, periodo_PE, IdRealizacao())) {

				Periodo periodo_anterior = periodo_PE;
				horizonte_espaco_amostral.decrementarIterador(periodo_anterior);

				for (Periodo periodo = periodo_anterior; periodo >= horizonte_espaco_amostral.getIteradorInicial(); horizonte_espaco_amostral.decrementarIterador(periodo)) {
					if (getElementoMatriz(idProcessoEstocastico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, periodo, IdRealizacao()) != getElementoMatriz(idProcessoEstocastico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, idCenario, periodo, IdRealizacao()))
						break;
					else if (periodo == horizonte_espaco_amostral.getIteradorInicial())
						idCenario_mapeamento = idCenario;
				}
				if (idCenario_mapeamento != IdCenario_Nenhum)
					break;
			}

		}

		if (idCenario_mapeamento == IdCenario_Nenhum)
			throw std::invalid_argument("Cenario nao encontrado para combinacao de realizacao e cenario passado informado.");

		atualizarModeloOtimizacaoComRestricaoCenario(a_idEstagio, idCenario_mapeamento);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::atualizarModeloOtimizacaoComRestricaoCenario(" + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "," + getFullString(a_idRealizacao) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelRealizacao(const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao){



void ModeloOtimizacao::atualizarModeloOtimizacaoComRestricaoCenario(const IdEstagio a_idEstagio, const IdCenario a_idCenario) {

	try {

		const IdRestricaoCenario maiorIdRestricaoCenario = getMaiorId(a_idEstagio, IdRestricaoCenario());

		for (IdRestricaoCenario idRestricaoCenario = IdRestricaoCenario_1; idRestricaoCenario <= maiorIdRestricaoCenario; idRestricaoCenario++) {

			for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++) {

				const int idEquacao = getElementoVetor(a_idEstagio, idRestricaoCenario, AttVetorRestricaoCenario_idRestricao, tSS, int());

				if (idEquacao > -1) {

					if (getSizeVetor(a_idEstagio, idRestricaoCenario, AttVetorRestricaoCenario_rhs) > 0)
						vetorEstagio.att(a_idEstagio).getSolver(tSS)->setRHSRestricao(idEquacao, getElementoVetor(a_idEstagio, idRestricaoCenario, AttVetorRestricaoCenario_rhs, a_idCenario, double()));

					if (getSize1Matriz(a_idEstagio, idRestricaoCenario, AttMatrizRestricaoCenario_coeficiente) > 0) {

						const int menorVar = getIterador1Inicial(a_idEstagio, idRestricaoCenario, AttMatrizRestricaoCenario_coeficiente, int());
						const int maiorVar = getIterador1Final(a_idEstagio, idRestricaoCenario, AttMatrizRestricaoCenario_coeficiente, int());

						for (int var = menorVar; var <= maiorVar; var++) {

							if (getSize2Matriz(a_idEstagio, idRestricaoCenario, AttMatrizRestricaoCenario_coeficiente, var) > 0)
								vetorEstagio.att(a_idEstagio).getSolver(tSS)->setCofRestricao(var, idEquacao,  getElementoMatriz(a_idEstagio, idRestricaoCenario, AttMatrizRestricaoCenario_coeficiente, var, a_idCenario, double()));

						} // for (int var = menorVar; var <= maiorVar; var++) {

					} // if (getSize1Matriz(a_idEstagio, idRestricaoCenario, AttMatrizRestricaoCenario_coeficiente) > 0) {

					if (idRestricaoCenario == maiorIdRestricaoCenario)
						vetorEstagio.att(a_idEstagio).getSolver(tSS)->atualizar();

				} // if (idEquacao > -1) {

			} // for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++) {

		} // for (IdRestricaoCenario idRestricaoCenario = IdRestricaoCenario_1; idRestricaoCenario <= maiorIdRestricaoCenario; idRestricaoCenario++) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::atualizarModeloOtimizacaoComRestricaoCenario(" + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::atualizarModeloOtimizacaoComRestricaoCenario(const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao){


bool ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelRealizacaoInterna(const TipoSubproblemaSolver a_TSS_destino, const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const std::string a_diretorio) {
	try {

		const IdVariavelRealizacaoInterna maiorIdVariavelRealizacaoInterna = getMaiorId(a_idEstagio, IdVariavelRealizacaoInterna());

		if (maiorIdVariavelRealizacaoInterna == IdVariavelRealizacaoInterna_Nenhum)
			return true;
		
		for (IdVariavelRealizacaoInterna idVariavelRealizacaoInterna = IdVariavelRealizacaoInterna_1; idVariavelRealizacaoInterna <= maiorIdVariavelRealizacaoInterna; idVariavelRealizacaoInterna++) {

			const IdProcessoEstocastico           idProcessoEstocastico = getAtributo(a_idEstagio, idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_idProcessoEstocastico, IdProcessoEstocastico());
			const IdVariavelAleatoria               idVariavelAleatoria = getAtributo(a_idEstagio, idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_idVariavelAleatoria, IdVariavelAleatoria());
			const IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = getAtributo(a_idEstagio, idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_idVariavelAleatoriaInterna, IdVariavelAleatoriaInterna());

			if (idVariavelAleatoriaInterna > IdVariavelAleatoriaInterna_Nenhum) {

				const Periodo periodo = getAtributo(a_idEstagio, idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_periodo, Periodo());

				double valor = NAN;

				if (a_idRealizacao == IdRealizacao_Nenhum)
					valor = vetorProcessoEstocastico.att(idProcessoEstocastico).calcularRealizacaoInterna(idVariavelAleatoria, idVariavelAleatoriaInterna, periodo, vetorProcessoEstocastico.att(idProcessoEstocastico).getElementoMatriz(idVariavelAleatoria, AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral, periodo, a_idCenario, double()));
				else
					valor = vetorProcessoEstocastico.att(idProcessoEstocastico).calcularRealizacaoInterna(idVariavelAleatoria, idVariavelAleatoriaInterna, periodo, vetorProcessoEstocastico.att(idProcessoEstocastico).calcularRealizacao(idVariavelAleatoria, a_idCenario, a_idRealizacao, periodo));

				double valor_novo = 0.0;

				if (valor < 1.0)
					valor_novo = 1.0 - valor;

				const int idVariavelDecisao_origem = getElementoVetor(a_idEstagio, idVariavelRealizacaoInterna, AttVetorVariavelRealizacaoInterna_idVariavelDecisao, a_TSS, int());

				//vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(idVariavelDecisao_origem, 0.0);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(idVariavelDecisao_origem, valor_novo);

				if (idVariavelRealizacaoInterna == maiorIdVariavelRealizacaoInterna)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->atualizar();

			} // if (idVariavelAleatoriaInterna > IdVariavelAleatoriaInterna_Nenhum) {
			 
		} // for (IdVariavelRealizacaoInterna idVariavelRealizacaoInterna = IdVariavelRealizacaoInterna_1; idVariavelRealizacaoInterna <= maiorIdVariavelRealizacaoInterna; idVariavelRealizacaoInterna++) {

		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->otimizar();

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_nao_otimalidade) {
			//otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, 1, a_idEstagio);
			otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, a_idEstagio);
		}

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

			if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao == IdRealizacao_Nenhum)) {

				VARIAVEL_DECISAO_2(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					VARIAVEL_DECISAO_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					VARIAVEL_DECISAO_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					VARIAVEL_DECISAO_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					VARIAVEL_DECISAO_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)

			} // if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao == IdRealizacao_Nenhum)) {

			else if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario_por_realizacao, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao != IdRealizacao_Nenhum)) {

				VARIAVEL_DECISAO_2(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					VARIAVEL_DECISAO_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					VARIAVEL_DECISAO_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					VARIAVEL_DECISAO_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					VARIAVEL_DECISAO_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)

			} // else if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario_por_realizacao, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao != IdRealizacao_Nenhum)) {

			if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario_por_realizacao, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao == IdRealizacao_Nenhum)) {

					EQUACAO_LINEAR_2(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					EQUACAO_LINEAR_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					EQUACAO_LINEAR_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					EQUACAO_LINEAR_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					EQUACAO_LINEAR_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)

					INEQUACAO_LINEAR_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					INEQUACAO_LINEAR_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					INEQUACAO_LINEAR_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					INEQUACAO_LINEAR_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					INEQUACAO_LINEAR_7(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)

			} // if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario_por_realizacao, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao == IdRealizacao_Nenhum)) {

			else if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario_por_realizacao, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao != IdRealizacao_Nenhum)) {

					EQUACAO_LINEAR_2(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					EQUACAO_LINEAR_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					EQUACAO_LINEAR_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					EQUACAO_LINEAR_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					EQUACAO_LINEAR_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)

					INEQUACAO_LINEAR_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					INEQUACAO_LINEAR_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					INEQUACAO_LINEAR_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					INEQUACAO_LINEAR_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					INEQUACAO_LINEAR_7(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)

			} // else if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario_por_realizacao, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao != IdRealizacao_Nenhum)) {

			if ((getAtributo(AttComumModeloOtimizacao_imprimir_solver, bool())) && (getAtributo(AttComumModeloOtimizacao_imprimir_solver_viabilidade_hidraulica, bool()))) {
				criarDiretorio(a_diretorio + "//" + getString(a_TSS));
				if (a_idRealizacao == IdRealizacao_Nenhum)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP(a_diretorio + "//" + getString(a_TSS) + "//PL_" + getString(a_idEstagio) + "_" + getString(a_idCenario));
				else
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP(a_diretorio + "//" + getString(a_TSS) + "//PL_" + getString(a_idEstagio) + "_" + getString(a_idCenario) + "_" + getString(a_idRealizacao));
			} // if ((getAtributo(AttComumModeloOtimizacao_imprimir_solver, bool())) && (getAtributo(AttComumModeloOtimizacao_imprimir_solver_viabilidade_hidraulica, bool()))) {

			for (IdVariavelRealizacaoInterna idVariavelRealizacaoInterna = IdVariavelRealizacaoInterna_1; idVariavelRealizacaoInterna <= maiorIdVariavelRealizacaoInterna; idVariavelRealizacaoInterna++) {

				const IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = getAtributo(a_idEstagio, idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_idVariavelAleatoriaInterna, IdVariavelAleatoriaInterna());

				if (idVariavelAleatoriaInterna > IdVariavelAleatoriaInterna_Nenhum) {

					const int idVariavelDecisao_origem = getElementoVetor(a_idEstagio, idVariavelRealizacaoInterna, AttVetorVariavelRealizacaoInterna_idVariavelDecisao, a_TSS, int());

					double valor = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorPrimal(idVariavelDecisao_origem);

					if (valor > 0.0) {

						const int idVariavelDecisao = getElementoVetor(a_idEstagio, idVariavelRealizacaoInterna, AttVetorVariavelRealizacaoInterna_idVariavelDecisao, a_TSS_destino, int());

						vetorEstagio.att(a_idEstagio).getSolver(a_TSS_destino)->setLimInferior(idVariavelDecisao, valor + 1.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS_destino)->setLimSuperior(idVariavelDecisao, valor + 1.0);

					} // if (valor > 0.0) {

				} // if (idVariavelAleatoriaInterna > IdVariavelAleatoriaInterna_Nenhum) {

				if (idVariavelRealizacaoInterna == maiorIdVariavelRealizacaoInterna)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS_destino)->atualizar();

			} // for (IdVariavelRealizacaoInterna idVariavelRealizacaoInterna = IdVariavelRealizacaoInterna_1; idVariavelRealizacaoInterna <= maiorIdVariavelRealizacaoInterna; idVariavelRealizacaoInterna++) {

			return true;

		} // if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

		const std::string codigo = getString(int(time(0)));
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + codigo);
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + codigo);
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + codigo);
		throw std::invalid_argument("Erro em otimizar, codigo identificador do problema linear " + codigo);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::atualizarModeloOtimizacaoComVariavelRealizacaoInterna(" + getFullString(a_TSS_destino) + "," + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "," + getFullString(a_idRealizacao) + "): \n" + std::string(erro.what())); }

} // bool ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelRealizacaoInterna(const TipoSubproblemaSolver a_TSS, const TipoSubproblemaSolver a_TSS_origem, const IdEstagio a_idEstagio, const IdCenario a_idCenario) {


bool ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelRealizacaoInterna(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) {

	try {

		const IdVariavelRealizacaoInterna maiorIdVariavelRealizacaoInterna = getMaiorId(a_idEstagio, IdVariavelRealizacaoInterna());

		if (maiorIdVariavelRealizacaoInterna == IdVariavelRealizacaoInterna_Nenhum)
			return true;

		const double valor_viabilidade = 1.0;

		for (IdVariavelRealizacaoInterna idVariavelRealizacaoInterna = IdVariavelRealizacaoInterna_1; idVariavelRealizacaoInterna <= maiorIdVariavelRealizacaoInterna; idVariavelRealizacaoInterna++) {

			const IdProcessoEstocastico           idProcessoEstocastico = getAtributo(a_idEstagio, idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_idProcessoEstocastico, IdProcessoEstocastico());
			const IdVariavelAleatoria               idVariavelAleatoria = getAtributo(a_idEstagio, idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_idVariavelAleatoria, IdVariavelAleatoria());
			const IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = getAtributo(a_idEstagio, idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_idVariavelAleatoriaInterna, IdVariavelAleatoriaInterna());

			const Periodo periodo = getAtributo(a_idEstagio, idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_periodo, Periodo());

			const TipoRelaxacaoVariavelAleatoria tipo_relaxacao = getElementoVetor(idProcessoEstocastico, idVariavelAleatoria, AttVetorVariavelAleatoria_tipo_relaxacao, periodo, TipoRelaxacaoVariavelAleatoria());

			if (idVariavelAleatoria == IdVariavelAleatoria_122)
				int aa = 1;

			if (tipo_relaxacao != TipoRelaxacaoVariavelAleatoria_sem_relaxacao) {

				const int idVariavelDecisao = getElementoVetor(a_idEstagio, idVariavelRealizacaoInterna, AttVetorVariavelRealizacaoInterna_idVariavelDecisao, a_TSS, int());

				double novo_valor_inf = 0.0;
				double novo_valor_sup = 0.0;

				// Penalizacao
				if (tipo_relaxacao == TipoRelaxacaoVariavelAleatoria_penalizacao) {

					if (getAtributo(AttComumModeloOtimizacao_relaxar_afluencia_incremental_com_viabilidade_hidraulica, bool()) && (a_idRealizacao != IdRealizacao_Nenhum))
						novo_valor_sup = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getInfinito();

					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(idVariavelDecisao, novo_valor_inf);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(idVariavelDecisao, novo_valor_sup);

				}

				// Truncamento
				else if ((tipo_relaxacao == TipoRelaxacaoVariavelAleatoria_truncamento) || (tipo_relaxacao == TipoRelaxacaoVariavelAleatoria_truncamento_penalizacao)) {

					if (getAtributo(AttComumModeloOtimizacao_relaxar_afluencia_incremental_com_viabilidade_hidraulica, bool()) && (a_idRealizacao == IdRealizacao_Nenhum)) {
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(idVariavelDecisao, novo_valor_inf);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(idVariavelDecisao, novo_valor_sup);
					}

					else {

						double realizacao = 0.0;

						// forward
						if (a_idRealizacao == IdRealizacao_Nenhum)
							realizacao = vetorProcessoEstocastico.att(idProcessoEstocastico).getElementoMatriz(idVariavelAleatoria, AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral, periodo, a_idCenario, double());

						// backward
						else if (a_idRealizacao > IdRealizacao_Nenhum)
							realizacao = vetorProcessoEstocastico.att(idProcessoEstocastico).calcularRealizacao(idVariavelAleatoria, a_idCenario, a_idRealizacao, periodo);

						const double valor_minimo_convexo = vetorProcessoEstocastico.att(idProcessoEstocastico).calcularRealizacaoParaValor(idVariavelAleatoria, 0.0, periodo);

						if (realizacao < valor_minimo_convexo) {
							const double realizacao_flex = valor_minimo_convexo - realizacao;
							novo_valor_sup = valor_viabilidade + realizacao_flex;
							if (tipo_relaxacao == TipoRelaxacaoVariavelAleatoria_truncamento)
								novo_valor_inf = novo_valor_sup;
						}

						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(idVariavelDecisao, novo_valor_inf);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(idVariavelDecisao, novo_valor_sup);

					}
				} // else if (tipo_relaxacao == TipoRelaxacaoVariavelAleatoria_truncamento) {

			} // else if ((tipo_relaxacao == TipoRelaxacaoVariavelAleatoria_truncamento) || (tipo_relaxacao == TipoRelaxacaoVariavelAleatoria_truncamento_penalizacao)) {

		} // for (IdVariavelRealizacaoInterna idVariavelRealizacaoInterna = IdVariavelRealizacaoInterna_1; idVariavelRealizacaoInterna <= maiorIdVariavelRealizacaoInterna; idVariavelRealizacaoInterna++) {

		return false;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::atualizarModeloOtimizacaoComVariavelRealizacaoInterna(" + getFullString(a_TSS) + "," + getFullString(a_idIteracao) + "," + getFullString(a_idProcesso) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "," + getFullString(a_idRealizacao) + "): \n" + std::string(erro.what())); }


} // bool ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelRealizacaoInterna(const IdEstagio a_idEstagio, const IdCenario a_idCenario){


void ModeloOtimizacao::resetarVariavelRealizacaoInterna(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio) {

	try {

		const IdVariavelRealizacaoInterna maiorIdVariavelRealizacaoInterna = getMaiorId(a_idEstagio, IdVariavelRealizacaoInterna());
		for (IdVariavelRealizacaoInterna idVariavelRealizacaoInterna = IdVariavelRealizacaoInterna_1; idVariavelRealizacaoInterna <= maiorIdVariavelRealizacaoInterna; idVariavelRealizacaoInterna++)
			vetorEstagio.att(a_idEstagio).resetarValorVariavelRealizacaoInterna(a_TSS, idVariavelRealizacaoInterna);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::resetarVariavelRealizacaoInterna(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::resetarVariavelRealizacaoInterna(){


void ModeloOtimizacao::atualizarVariavelEstadoComSolucaoPrimal(const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario){

	try {

		const TipoSubproblemaSolver tSS = TipoSubproblemaSolver_geral;

		const IdEstagio idEstagio_anterior = IdEstagio(a_idEstagio - 1);

		const IdVariavelEstado maiorIdVariavelEstado = getMaiorId(a_idEstagio, IdVariavelEstado());

		for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

			if (vetorEstagio.att(a_idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

				try {

					const int idVariavelDecisaoEstagioAnterior = getAtributo(a_idEstagio, idVariavelEstado, AttComumVariavelEstado_idVariavelDecisaoEstagioAnterior, int());

					if (idVariavelDecisaoEstagioAnterior > 0) {

						double solucao_primal = vetorEstagio.att(idEstagio_anterior).getSolver(tSS)->getValorPrimal(idVariavelDecisaoEstagioAnterior);

						if (solucao_primal > vetorEstagio.att(idEstagio_anterior).getSolver(tSS)->getLimSuperior(idVariavelDecisaoEstagioAnterior))
							solucao_primal = vetorEstagio.att(idEstagio_anterior).getSolver(tSS)->getLimSuperior(idVariavelDecisaoEstagioAnterior);

						else if (solucao_primal < vetorEstagio.att(idEstagio_anterior).getSolver(tSS)->getLimInferior(idVariavelDecisaoEstagioAnterior))
							solucao_primal = vetorEstagio.att(idEstagio_anterior).getSolver(tSS)->getLimInferior(idVariavelDecisaoEstagioAnterior);

						addValorVariavelEstado(a_idEstagio, idVariavelEstado, a_idCenario, solucao_primal);

					} // if (idVariavelDecisaoEstagioAnterior > 0) {

				} // try
				catch (const std::exception& erro) {

					const std::string erro_str = std::string(erro.what());

					if (erro_str.size() > 5) {
						if (erro_str.find("10005") != std::string::npos) {

							escreverLog10005(tSS, a_idIteracao, a_idEstagio, a_idCenario, IdRealizacao_Nenhum, erro_str);

							for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

								if (vetorEstagio.att(a_idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

									const int idVariavelDecisaoEstagioAnterior = getAtributo(a_idEstagio, idVariavelEstado, AttComumVariavelEstado_idVariavelDecisaoEstagioAnterior, int());

									if (idVariavelDecisaoEstagioAnterior > 0) {

										const double limite_superior = vetorEstagio.att(idEstagio_anterior).getSolver(tSS)->getLimSuperior(idVariavelDecisaoEstagioAnterior);
										const double limite_inferior = vetorEstagio.att(idEstagio_anterior).getSolver(tSS)->getLimInferior(idVariavelDecisaoEstagioAnterior);

										const double valor = (limite_superior - limite_inferior) / 2.0;

										addValorVariavelEstado(a_idEstagio, idVariavelEstado, a_idCenario, valor);

									} // if (idVariavelDecisaoEstagioAnterior > 0) {

								} // if (vetorEstagio.att(a_idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {
							} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

						} // if (erro_str.find("10005") != std::string::npos) {					
					} // if (erro_str.size() > 5) {

					return;

				} // catch (const std::exception & erro) { 

			} // if (vetorEstagio.att(a_idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

		} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::atualizarVariavelEstadoComSolucaoPrimal(" + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::atualizarVariavelEstadoComSolucao(const IdEstagio a_idEstagio, const IdCenario a_idCenario){

void ModeloOtimizacao::addValorVariavelEstado(const IdEstagio a_idEstagio, const IdVariavelEstado idVariavelEstado, const IdCenario a_idCenario, const double valor){
	try {

		vetorEstagio.att(a_idEstagio).addValorVariavelEstado(idVariavelEstado, a_idCenario, IdCenario_Nenhum, IdCenario_Nenhum, valor);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::addValorVariavelEstado(" + getFullString(a_idEstagio) + "," + getFullString(idVariavelEstado) + "," + getFullString(a_idCenario) + "," + ",double): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::addValorVariavelEstado(const IdEstagio a_idEstagio, const IdVariavelEstado idVariavelEstado, const IdProcesso a_idProcesso, const IdProcesso a_maior_processo, const IdCenario a_idCenario, const double valor){


void ModeloOtimizacao::alocarVariaveisEstado(const IdIteracao a_idIteracao, const IdEstagio a_idEstagio) {

	try {

		IdCenario menor_cenario_estado = IdCenario_Nenhum;
		IdCenario maior_cenario_estado = IdCenario_Nenhum;

		const IdProcesso idProcesso_local = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());
		if (arranjoResolucao.getSize1Matriz(a_idIteracao, idProcesso_local, AttMatrizProcesso_menor_abertura_por_cenario_estado) > 0) {
			const std::vector<IdCenario> lista_cenario_estado = arranjoResolucao.getIdsCenarioEstadoFromAberturas(idProcesso_local, a_idIteracao, a_idEstagio);
			if (lista_cenario_estado.size() > 0) {
				menor_cenario_estado = lista_cenario_estado.at(0);
				maior_cenario_estado = lista_cenario_estado.at(lista_cenario_estado.size() - 1);
			}
		}
		if (menor_cenario_estado == IdCenario_Nenhum) {
			const std::vector<IdCenario> lista_cenario_estado = arranjoResolucao.getIdsCenarioEstadoFromCenarios(idProcesso_local, a_idIteracao, a_idEstagio);
			if (lista_cenario_estado.size() > 0) {
				menor_cenario_estado = lista_cenario_estado.at(0);
				maior_cenario_estado = lista_cenario_estado.at(lista_cenario_estado.size() - 1);
			}
		}

		if (menor_cenario_estado == IdCenario_Nenhum)
			return;

		vetorEstagio.att(a_idEstagio).alocarVariaveisEstado(menor_cenario_estado, maior_cenario_estado);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::alocarVariaveisEstado(" + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "," + ",double): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::imprimirEstagios(EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
		const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

		for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++)
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(idEstagio) + "_estagio.csv", vetorEstagio.att(idEstagio));

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::imprimirCorteBenders(a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::imprimirEstagios(EntradaSaidaDados a_entradaSaidaDados){


void ModeloOtimizacao::imprimirSolver(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const std::string a_nome_arquivo) {

	try {

		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP(a_nome_arquivo);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::imprimirSolver(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + a_nome_arquivo + "): \n" + std::string(erro.what())); }


}


void ModeloOtimizacao::imprimirVariaveisEstado(EntradaSaidaDados a_entradaSaidaDados) {

	const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
	const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

	for (IdEstagio idEstagio = estagio_inicial; idEstagio <= getMaiorId(IdEstagio()); idEstagio++) {
		if (getMaiorId(idEstagio, IdVariavelEstado()) > IdVariavelEstado_Nenhum) {
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(idEstagio) + "_estado.csv", IdVariavelEstado_Nenhum, vetorEstagio.att(idEstagio));
			if (idEstagio <= estagio_final)
				a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(idEstagio) + "_estado_variaveis.csv", IdVariavelEstado_Nenhum, vetorEstagio.att(idEstagio), AttVetorVariavelEstado_idVariavelDecisao);
		}
	}

} // void ModeloOtimizacao::imprimirVariaveisEstado(EntradaSaidaDados a_entradaSaidaDados){


void ModeloOtimizacao::imprimirVariaveisRealizacao(EntradaSaidaDados a_entradaSaidaDados) {

	const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
	const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

	for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {
		if (getMaiorId(idEstagio, IdVariavelRealizacao()) > IdVariavelRealizacao_Nenhum) {
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(idEstagio) + "_variavel_realizacao.csv", IdVariavelRealizacao_Nenhum, vetorEstagio.att(idEstagio));
			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(idEstagio) + "_variavel_realizacao_variaveis.csv", IdVariavelRealizacao_Nenhum, vetorEstagio.att(idEstagio), AttVetorVariavelRealizacao_idVariavelDecisao);
		}
	}

} // void ModeloOtimizacao::imprimirVariaveisRealizacao(EntradaSaidaDados a_entradaSaidaDados){

void ModeloOtimizacao::imprimirRestricoesCenario(EntradaSaidaDados a_entradaSaidaDados) {

	const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
	const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

	for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {
		if (getMaiorId(idEstagio, IdRestricaoCenario()) > IdRestricaoCenario_Nenhum) {
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(idEstagio) + "_restricao_realizacao.csv", IdRestricaoCenario_Nenhum, vetorEstagio.att(idEstagio));
			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(idEstagio) + "_restricao_realizacao_equacoes.csv", IdRestricaoCenario_Nenhum, vetorEstagio.att(idEstagio), AttVetorRestricaoCenario_idRestricao);
			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(idEstagio) + "_restricao_realizacao_rhs.csv", IdRestricaoCenario_Nenhum, vetorEstagio.att(idEstagio), AttVetorRestricaoCenario_rhs);
			a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz(getFullString(idEstagio) + "_restricao_realizacao_coeficiente.csv", IdRestricaoCenario_Nenhum, vetorEstagio.att(idEstagio), AttMatrizRestricaoCenario_coeficiente);
		}
	}

} // void ModeloOtimizacao::imprimirVariaveisRealizacao(EntradaSaidaDados a_entradaSaidaDados){

void ModeloOtimizacao::imprimirVariaveisRealizacaoInterna(EntradaSaidaDados a_entradaSaidaDados) {

	const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
	const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

	for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {
		if (getMaiorId(idEstagio, IdVariavelRealizacaoInterna()) > IdVariavelRealizacaoInterna_Nenhum) {
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(idEstagio) + "_variavel_realizacao_interna.csv", IdVariavelRealizacaoInterna_Nenhum, vetorEstagio.att(idEstagio));
			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(idEstagio) + "_variavel_realizacao_interna_variaveis.csv", IdVariavelRealizacaoInterna_Nenhum, vetorEstagio.att(idEstagio), AttVetorVariavelRealizacaoInterna_idVariavelDecisao);
		}
	}

} // void ModeloOtimizacao::imprimirVariaveisRealizacaoInterna(EntradaSaidaDados a_entradaSaidaDados){


void ModeloOtimizacao::exportarCorteBenders(const IdProcesso a_idProcesso, const IdEstagio a_estagio_inicial, const IdEstagio a_estagio_final, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		for (IdEstagio idEstagio = a_estagio_inicial; idEstagio <= a_estagio_final; idEstagio++)
			exportarCorteBenders(a_idProcesso, idEstagio, a_entradaSaidaDados);

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::exportarCorteBenders(" + getFullString(a_idProcesso) + "," + getFullString(a_estagio_inicial) + "," + getFullString(a_estagio_final) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::exportarCorteBenders(const IdEstagio a_estagio_inicial, const IdEstagio a_estagio_final, EntradaSaidaDados a_entradaSaidaDados){

void ModeloOtimizacao::exportarCorteBenders(const IdProcesso a_idProcesso, const IdEstagio a_estagio, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const std::string diretorio_saida = a_entradaSaidaDados.getDiretorioSaida();

		if (a_idProcesso == IdProcesso_mestre)
			a_entradaSaidaDados.setDiretorioSaida(diretorio_saida + "//Cortes");
		else
			a_entradaSaidaDados.setDiretorioSaida(diretorio_saida + "//Cortes//" + getFullString(a_idProcesso));

		a_entradaSaidaDados.setAppendArquivo(false);

		if (vetorEstagio.att(a_estagio).vetorCorteBenders.getMaiorId() != IdCorteBenders_Nenhum) {
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(a_estagio) + "_estagio.csv", vetorEstagio.att(a_estagio), std::vector<AttComumEstagio>{ AttComumEstagio_idEstagio, AttComumEstagio_periodo_otimizacao, AttComumEstagio_selecao_cortes_nivel_dominancia, AttComumEstagio_cortes_multiplos, AttComumEstagio_alpha_CVAR, AttComumEstagio_lambda_CVAR});
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(a_estagio) + "_estado.csv", IdVariavelEstado_Nenhum, vetorEstagio.att(a_estagio));
			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_estagio) + "_corteBenders_rhs.csv", IdCorteBenders_Nenhum, vetorEstagio.att(a_estagio), AttVetorCorteBenders_rhs);
			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_estagio) + "_corteBenders_estado.csv", IdCorteBenders_Nenhum, vetorEstagio.att(a_estagio), AttVetorCorteBenders_estado);
			a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz(getFullString(a_estagio) + "_corteBenders_coeficientes.csv", IdCorteBenders_Nenhum, vetorEstagio.att(a_estagio), AttMatrizCorteBenders_coeficiente);
		} // if (vetorEstagio.att(a_estagio).vetorCorteBenders.getMaiorId() != IdCorteBenders_Nenhum) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::exportarCorteBenders(" + getFullString(a_idProcesso) + "," + getFullString(a_estagio) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::exportarCorteBenders(const IdEstagio a_estagio_inicial, const IdEstagio a_estagio_final, EntradaSaidaDados a_entradaSaidaDados){

void ModeloOtimizacao::exportarVersaoAlternativaCorteBenders(const IdProcesso a_idProcesso, const IdEstagio a_estagio, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const IdCorteBenders maiorIdCorteBenders = vetorEstagio.att(a_estagio).vetorCorteBenders.getMaiorId();

		if (maiorIdCorteBenders == IdCorteBenders_Nenhum)
			return;

		if (a_estagio > getAtributo(AttComumModeloOtimizacao_maior_estagio_impressao_versao_alternativa_cortes, IdEstagio()))
			return;

		const IdProcessoEstocastico idProcessoEstocastico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

		if (idProcessoEstocastico != IdProcessoEstocastico_hidrologico_bacia)
			return;

		const IdProcessoEstocastico idProcessoEstocasticoNovo = IdProcessoEstocastico_hidrologico_hidreletrica;
		const std::string idProcessoEstocasticoNovo_str = getFullString(idProcessoEstocasticoNovo);
		const std::string idProcessoEstocasticoNovo_compacto_str = getString(idProcessoEstocasticoNovo);

		const IdVariavelAleatoria maiorIdVariavelAleatoria = getMaiorId(idProcessoEstocastico, IdVariavelAleatoria());

		SmartEnupla<IdVariavelAleatoria, std::vector<std::string>> lista_nome_idVariavelAleatoriaNova(IdVariavelAleatoria_1, std::vector<std::vector<std::string>>(int(maiorIdVariavelAleatoria - IdVariavelAleatoria_1)+ 1, std::vector<std::string>()));
		SmartEnupla<IdVariavelAleatoria, std::vector<std::string>> lista_nome_compacto_idVariavelAleatoriaNova(IdVariavelAleatoria_1, std::vector<std::vector<std::string>>(int(maiorIdVariavelAleatoria - IdVariavelAleatoria_1)+ 1, std::vector<std::string>()));
		SmartEnupla<IdVariavelAleatoria, std::vector<double>>      lista_participacao_idVariavelAleatoriaNova(IdVariavelAleatoria_1, std::vector<std::vector<double>>(int(maiorIdVariavelAleatoria - IdVariavelAleatoria_1)+ 1, std::vector<double>()));

		const SmartEnupla<Periodo, int> horizonte_processo_estocastico_hidrologico = getElementosMatriz(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, a_estagio, Periodo(), int());

		const Periodo periodo_inicial_otimizacao = horizonte_processo_estocastico_hidrologico.getIteradorInicial();
		const Periodo periodo_final_otimizacao = horizonte_processo_estocastico_hidrologico.getIteradorFinal();

		const Periodo periodo_otimizacao = getAtributo(a_estagio, AttComumEstagio_periodo_otimizacao, Periodo());


		for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= getMaiorId(idProcessoEstocastico, IdVariavelAleatoria()); idVariavelAleatoria++) {

			for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= getMaiorId(idProcessoEstocastico, idVariavelAleatoria, IdVariavelAleatoriaInterna()); idVariavelAleatoriaInterna++) {

				const std::string nome = getAtributo(idProcessoEstocastico, idVariavelAleatoria, idVariavelAleatoriaInterna, AttComumVariavelAleatoriaInterna_nome, std::string());

				const IdVariavelAleatoria idVariavelAleatoriaNova = getIdVariavelAleatoriaFromChar(nome.substr(nome.find("_") + 1).c_str());

				if (idVariavelAleatoriaNova == IdVariavelAleatoria_Nenhum)
					throw std::invalid_argument("Erro ao encontrar variavel aleatoria por hidreletrica em " + getFullString(idProcessoEstocastico) + "," + getFullString(idVariavelAleatoria) + "," + getFullString(idVariavelAleatoriaInterna));


				lista_nome_idVariavelAleatoriaNova.at(idVariavelAleatoria).push_back(getFullString(idVariavelAleatoriaNova));
				lista_nome_compacto_idVariavelAleatoriaNova.at(idVariavelAleatoria).push_back(getString(idVariavelAleatoriaNova));
				
				double participacao = 0.0;
				for (Periodo periodo = periodo_inicial_otimizacao; periodo <= periodo_final_otimizacao; horizonte_processo_estocastico_hidrologico.incrementarIterador(periodo)) {
					participacao += double(periodo.getSegundos() / periodo_otimizacao.getSegundos()) * getElementoVetor(idProcessoEstocastico, idVariavelAleatoria, idVariavelAleatoriaInterna, AttVetorVariavelAleatoriaInterna_coeficiente_participacao, periodo_inicial_otimizacao, double());
				}

				lista_participacao_idVariavelAleatoriaNova.at(idVariavelAleatoria).push_back(participacao);

			} // for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= getMaiorId(a_tipo_processo_estocastico_hidrologico, idVariavelAleatoria, IdVariavelAleatoriaInterna()); idVariavelAleatoriaInterna++) {

		} // for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= getMaiorId(idProcessoEstocastico, IdVariavelAleatoria()); idVariavelAleatoria++) {

		Estagio estagio;

		vetorEstagio.att(a_estagio).exportarVersaoAlternativaCorteBenders(estagio, idProcessoEstocasticoNovo_str, idProcessoEstocasticoNovo_compacto_str, lista_nome_idVariavelAleatoriaNova, lista_nome_compacto_idVariavelAleatoriaNova, lista_participacao_idVariavelAleatoriaNova);

		const std::string diretorio_saida = a_entradaSaidaDados.getDiretorioSaida();

		if (a_idProcesso == IdProcesso_mestre)
			a_entradaSaidaDados.setDiretorioSaida(diretorio_saida + "//Cortes_" + getString(idProcessoEstocasticoNovo));
		else
			a_entradaSaidaDados.setDiretorioSaida(diretorio_saida + "//Cortes" + getString(idProcessoEstocasticoNovo) + "//" + getFullString(a_idProcesso));

		a_entradaSaidaDados.setAppendArquivo(false);

		a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(a_estagio) + "_estagio.csv", estagio, std::vector<AttComumEstagio>{ AttComumEstagio_idEstagio, AttComumEstagio_periodo_otimizacao, AttComumEstagio_selecao_cortes_nivel_dominancia, AttComumEstagio_cortes_multiplos, AttComumEstagio_alpha_CVAR, AttComumEstagio_lambda_CVAR});
		a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(a_estagio) + "_estado.csv", IdVariavelEstado_Nenhum, estagio);
		a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_estagio) + "_corteBenders_rhs.csv", IdCorteBenders_Nenhum, estagio, AttVetorCorteBenders_rhs);
		a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_estagio) + "_corteBenders_estado.csv", IdCorteBenders_Nenhum, estagio, AttVetorCorteBenders_estado);
		a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz(getFullString(a_estagio) + "_corteBenders_coeficientes.csv", IdCorteBenders_Nenhum, estagio, AttMatrizCorteBenders_coeficiente);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::exportarVersaoAlternativaCorteBenders(" + getFullString(a_idProcesso) + "," + getFullString(a_estagio) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::exportarVersaoAlternativaCorteBenders(const IdEstagio a_estagio_inicial, const IdEstagio a_estagio_final, EntradaSaidaDados a_entradaSaidaDados){

void ModeloOtimizacao::removerCorteBenders(const IdEstagio a_estagio_inicial, const IdEstagio a_estagio_final) {

	try {

		const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
		const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

		for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++)
			removerCorteBenders(idEstagio);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::removerCorteBenders(" + getFullString(a_estagio_inicial) + "," + getFullString(a_estagio_final) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::removerCorteBenders(const IdEstagio a_estagio){

void ModeloOtimizacao::removerCorteBenders(const IdEstagio a_estagio){

	try {

		for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= getMaiorId(a_estagio, IdCorteBenders()); idCorteBenders++) {
			if (vetorEstagio.att(a_estagio).vetorCorteBenders.isInstanciado(idCorteBenders))
				vetorEstagio.att(a_estagio).vetorCorteBenders.rem(idCorteBenders);
		} // for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= getMaiorId(a_estagio, IdCorteBenders()); idCorteBenders++) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::removerCorteBenders(" + getFullString(a_estagio) + "): \n" + std::string(erro.what())); }


} // void ModeloOtimizacao::removerCorteBenders(const IdEstagio a_estagio){


void ModeloOtimizacao::exportarCorteBenders_AcoplamentoPreEstudo(const IdIteracao a_idIteracao, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const string diretorio_importacao_pre_estudo = getAtributo(AttComumModeloOtimizacao_diretorio_importacao_pre_estudo, std::string());

		if (strCompara(diretorio_importacao_pre_estudo, "nenhum"))
			return;

		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//AcoplamentoPreEstudo");

		const IdEstagio estagio_acoplamento_pre_estudo = getAtributo(AttComumModeloOtimizacao_estagio_acoplamento_pre_estudo, IdEstagio());

		if (estagio_acoplamento_pre_estudo <= getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio())){

			if (vetorEstagio.att(estagio_acoplamento_pre_estudo).vetorCorteBenders.getMaiorId() != IdCorteBenders_Nenhum) {

				for (IdCorteBenders idCorteBenders = IdCorteBenders(maiorIdCorteExportadoPreEstudo + 1); idCorteBenders <= getMaiorId(estagio_acoplamento_pre_estudo, IdCorteBenders()); idCorteBenders++) {
					if (vetorEstagio.att(estagio_acoplamento_pre_estudo).vetorCorteBenders.isInstanciado(idCorteBenders)) {
						if (idCorteBenders == IdCorteBenders(maiorIdCorteExportadoPreEstudo + 1))
							a_entradaSaidaDados.setAppendArquivo(false);
						else
							a_entradaSaidaDados.setAppendArquivo(true);
						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("corteBenders_rhs_" + getFullString(a_idIteracao) + ".csv", idCorteBenders, vetorEstagio.att(estagio_acoplamento_pre_estudo), AttVetorCorteBenders_rhs);
						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("corteBenders_estado_" + getFullString(a_idIteracao) + ".csv", idCorteBenders, vetorEstagio.att(estagio_acoplamento_pre_estudo), AttVetorCorteBenders_estado);
						a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("corteBenders_coeficientes_" + getFullString(a_idIteracao) + ".csv", idCorteBenders, vetorEstagio.att(estagio_acoplamento_pre_estudo), AttMatrizCorteBenders_coeficiente);
					}
				}

				a_entradaSaidaDados.setAppendArquivo(false);
				a_entradaSaidaDados.imprimirArquivoCSV_AttComum("estagio_" + getFullString(a_idIteracao) + ".csv", vetorEstagio.att(estagio_acoplamento_pre_estudo), std::vector<AttComumEstagio>{AttComumEstagio_idEstagio, AttComumEstagio_periodo_otimizacao});

				maiorIdCorteExportadoPreEstudo = getMaiorId(estagio_acoplamento_pre_estudo, IdCorteBenders());

				if (a_idIteracao == arranjoResolucao.getAtributo(AttComumArranjoResolucao_iteracao_final, IdIteracao()))
					a_entradaSaidaDados.imprimirArquivoCSV_AttComum("eop.txt", vetorEstagio.att(estagio_acoplamento_pre_estudo), std::vector<AttComumEstagio>{AttComumEstagio_idEstagio, AttComumEstagio_periodo_otimizacao});

			} // if (vetorEstagio.att(estagio_acoplamento_pre_estudo).vetorCorteBenders.getMaiorId() != IdCorteBenders_Nenhum) {

		} // if (estagio_acoplamento_pre_estudo <= getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio())){

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::imprimirCorteBendersAcoplamento(a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::imprimirCorteBendersAcoplamento(EntradaSaidaDados a_entradaSaidaDados, const bool a_imprimir_estagio_e_estados){


void ModeloOtimizacao::importarCorteBenders(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdProcesso a_idProcesso, const std::string a_diretorio_impressao_selecao_cortes, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const string diretorio_importacao_cortes = getAtributo(AttComumModeloOtimizacao_diretorio_importacao_cortes, std::string());

		if (strCompara(diretorio_importacao_cortes, "nenhum"))
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
		const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

		VetorEstagioEmModeloOtimizacao vetorEstagio_aux;

		const IdCorteBenders maximoIdCorteBenders = getAtributo(AttComumModeloOtimizacao_maior_corte_importado, IdCorteBenders());

		a_entradaSaidaDados.setDiretorioEntrada(diretorio_importacao_cortes);
		for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {
			if ((estagio_inicial != IdEstagio_1) || (idEstagio > estagio_inicial)) {
				vetorEstagio.att(idEstagio).vetorCorteBenders.alocar(int(maximoIdCorteBenders));
				Estagio estagio_aux;
				estagio_aux.setAtributo(AttComumEstagio_idEstagio, idEstagio);
				vetorEstagio_aux.add(estagio_aux);
				a_entradaSaidaDados.carregarArquivoCSV_AttComum(getFullString(idEstagio) + "_estagio.csv", vetorEstagio_aux.att(idEstagio), TipoAcessoInstancia_direto);
				a_entradaSaidaDados.carregarArquivoCSV_AttVetor(getFullString(idEstagio) + "_corteBenders_rhs.csv", vetorEstagio_aux.att(idEstagio), TipoAcessoInstancia_membro);
				vetorEstagio_aux.att(idEstagio).carregarEstadosCortesBenders(a_entradaSaidaDados.getDiretorioEntrada() + "//" + getFullString(idEstagio) + "_corteBenders_estado.csv");
				vetorEstagio_aux.att(idEstagio).carregarCoeficientesCortesBenders(a_entradaSaidaDados.getDiretorioEntrada() + "//" + getFullString(idEstagio) + "_corteBenders_coeficientes.csv");
				a_entradaSaidaDados.carregarArquivoCSV_AttComum(getFullString(idEstagio) + "_estado.csv", vetorEstagio_aux.att(idEstagio), TipoAcessoInstancia_membro);
			} // if ((estagio_inicial != IdEstagio_1) || (idEstagio > estagio_inicial)) {
		} // for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

		for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

			SmartEnupla<IdVariavelAleatoria, std::vector<IdHidreletrica>> isIdHidreletricaNosEstadosYP(IdVariavelAleatoria_1, std::vector<std::vector<IdHidreletrica>>(IdVariavelAleatoria(IdVariavelAleatoria_Excedente - 1), std::vector<IdHidreletrica>()));

			if ((estagio_inicial != IdEstagio_1) || (idEstagio > estagio_inicial)) {

				if (getAtributo(idEstagio, AttComumEstagio_periodo_otimizacao, Periodo()).sobreposicao(vetorEstagio_aux.att(idEstagio).getAtributo(AttComumEstagio_periodo_otimizacao, Periodo())) == 0.0)
					throw std::invalid_argument("Periodo " + getString(vetorEstagio_aux.att(idEstagio).getAtributo(AttComumEstagio_periodo_otimizacao, Periodo())) + " importado nao compativel com periodo do estagio " + getFullString(idEstagio));

				const IdCorteBenders maiorIdCorteBenders = vetorEstagio_aux.att(idEstagio).getMaiorId(IdCorteBenders());

				const IdVariavelEstado maiorIdVarivelEstado = getMaiorId(idEstagio, IdVariavelEstado());

				IdRealizacao maiorIdRealizacao_corte = IdRealizacao_Nenhum;

				IdVariavelEstado maiorIdVarivelEstado_corte = IdVariavelEstado_Nenhum;
				for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {
					if (vetorEstagio_aux.att(idEstagio).vetorCorteBenders.isInstanciado(idCorteBenders)) {
						const IdRealizacao idRealizacao = vetorEstagio_aux.att(idEstagio).getIterador1Final(idCorteBenders, AttMatrizCorteBenders_coeficiente, IdRealizacao());
						if (idRealizacao > maiorIdRealizacao_corte)
							maiorIdRealizacao_corte = idRealizacao;
						if (maiorIdVarivelEstado_corte == IdVariavelEstado_Nenhum)
							maiorIdVarivelEstado_corte = vetorEstagio_aux.att(idEstagio).getIterador2Final(idCorteBenders, AttMatrizCorteBenders_coeficiente, idRealizacao, IdVariavelEstado());
					}
				} // for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {

				if (maiorIdVarivelEstado_corte == IdVariavelEstado_Nenhum)
					throw std::invalid_argument("Nenhum corte foi importado em " + getFullString(idEstagio));

				const int cortes_multiplos_modelo = getAtributo(idEstagio, AttComumEstagio_cortes_multiplos, int());

				if ((cortes_multiplos_modelo == 0) && (maiorIdRealizacao_corte != IdRealizacao_1))
					throw std::invalid_argument("A multiplicidade dos cortes importados nao eh compativel com a multiplicidade do modelo em " + getFullString(idEstagio));

				else if (maiorIdRealizacao_corte != getAtributo(idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()))
					throw std::invalid_argument("A multiplicidade dos cortes importados nao eh compativel com o numero de aberturas do modelo em " + getFullString(idEstagio));

				int numero_variaveis_estado_modelo_encontradas = 0;
				int numero_variaveis_estado_cortes_encontradas = 0;
				SmartEnupla<IdVariavelEstado, IdVariavelEstado> variaveis_estado_modelo_encontradas(IdVariavelEstado_1, std::vector<IdVariavelEstado>(maiorIdVarivelEstado,       IdVariavelEstado_Nenhum));
				SmartEnupla<IdVariavelEstado, IdVariavelEstado> variaveis_estado_cortes_encontradas(IdVariavelEstado_1, std::vector<IdVariavelEstado>(maiorIdVarivelEstado_corte, IdVariavelEstado_Nenhum));

				bool modelo_e_cortes_estados_identicos = true;

				for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVarivelEstado; idVariavelEstado++) {

					IdVariavelEstado idVariavelEstado_corte = IdVariavelEstado_Nenhum;

					try {

						if (idVariavelEstado <= maiorIdVarivelEstado_corte) {

							if (!strCompara(getAtributo(idEstagio, idVariavelEstado, AttComumVariavelEstado_nome, std::string()), vetorEstagio_aux.att(idEstagio).getAtributo(idVariavelEstado, AttComumVariavelEstado_nome, std::string()))) {

								modelo_e_cortes_estados_identicos = false;

								for (idVariavelEstado_corte = IdVariavelEstado_1; idVariavelEstado_corte <= maiorIdVarivelEstado_corte; idVariavelEstado_corte++) {


									const std::vector<std::string> nome_estados     = vetorEstagio.att(idEstagio).getNomeVariavelEstado(idVariavelEstado);
									const std::vector<std::string> nome_estados_aux = vetorEstagio_aux.att(idEstagio).getNomeVariavelEstado(idVariavelEstado_corte);

									if (nome_estados.size() == nome_estados_aux.size()) {
										bool is_nomes_iguais = true;
										for (int i = 0; i < int(nome_estados.size()); i++) {
											if (!strCompara(nome_estados.at(i), nome_estados_aux.at(i))) {
												is_nomes_iguais = false; break;
											}
										}
										if (is_nomes_iguais) {
											variaveis_estado_modelo_encontradas.at(idVariavelEstado) = idVariavelEstado_corte;
											variaveis_estado_cortes_encontradas.at(idVariavelEstado_corte) = idVariavelEstado;
											numero_variaveis_estado_modelo_encontradas++;
											numero_variaveis_estado_cortes_encontradas++;
											break;
										}
									} // if (nome_estados.size() == nome_estados_aux.size()) {

								} // for (IdVariavelEstado idVariavelEstado_corte = IdVariavelEstado_1; idVariavelEstado_corte <= maiorIdVarivelEstado_corte; idVariavelEstado_corte++) {

							} // if (!strCompara(getAtributo(idEstagio, idVariavelEstado, AttComumVariavelEstado_nome, std::string()), vetorEstagio_aux.att(idEstagio).getAtributo(idVariavelEstado, AttComumVariavelEstado_nome, std::string()))) {
							else {
								variaveis_estado_modelo_encontradas.at(idVariavelEstado) = idVariavelEstado;
								variaveis_estado_cortes_encontradas.at(idVariavelEstado) = idVariavelEstado;
								numero_variaveis_estado_modelo_encontradas++;
								numero_variaveis_estado_cortes_encontradas++;
							}
						} // if (idVariavelEstado <= maiorIdVarivelEstado_corte) {

					} // try {

					catch (const std::exception& erro) { 
						throw std::invalid_argument("Erro " + getFullString(idEstagio) + ", " + getFullString(idVariavelEstado) + ", Corte: " + getFullString(idVariavelEstado_corte) + ", \n" + std::string(erro.what()));
					}


				} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVarivelEstado; idVariavelEstado++) {

				//
				// Tratamento de variáveis do modelo que não foram encontradas no corte.
				// 

				if (numero_variaveis_estado_modelo_encontradas != int(maiorIdVarivelEstado)) {

					for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVarivelEstado; idVariavelEstado++) {

						if (variaveis_estado_modelo_encontradas.at(idVariavelEstado) == IdVariavelEstado_Nenhum) {

							const std::vector<std::string> nome = vetorEstagio.att(idEstagio).getNomeVariavelEstado(idVariavelEstado);

							if ((nome.at(0) != "VarDecisaoYP") && (nome.at(0) != "VarDecisaoVI")) {
								throw std::invalid_argument(getFullString(idVariavelEstado) + " " + getAtributo(idEstagio, idVariavelEstado, AttComumVariavelEstado_nome, std::string()) + " presente no modelo, nao consta no corte em " + getFullString(idEstagio));
							}

						} // if (variaveis_estado_modelo_encontradas.at(idVariavelEstado) == IdVariavelEstado_Nenhum) {

					} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVarivelEstado; idVariavelEstado++) {

				} // if (numero_variaveis_estado_modelo_encontradas != int(maiorIdVarivelEstado)) {


				//
				// Tratamento de variáveis do corte que não foram encontradas no modelo.
				// 

				if (numero_variaveis_estado_cortes_encontradas != int(maiorIdVarivelEstado_corte)) {

					modelo_e_cortes_estados_identicos = false;

					for (IdVariavelEstado idVariavelEstado_corte = IdVariavelEstado_1; idVariavelEstado_corte <= maiorIdVarivelEstado_corte; idVariavelEstado_corte++) {

						if (variaveis_estado_cortes_encontradas.at(idVariavelEstado_corte) == IdVariavelEstado_Nenhum) {

							const std::vector<std::string> nome = vetorEstagio_aux.att(idEstagio).getNomeVariavelEstado(idVariavelEstado_corte);

							if ((nome.at(0) != "VarDecisaoYP") && (nome.at(0) != "VarDecisaoVI")) {
								throw std::invalid_argument(getFullString(idVariavelEstado_corte) + " " + vetorEstagio_aux.att(idEstagio).getAtributo(idVariavelEstado_corte, AttComumVariavelEstado_nome, std::string()) + " presente no corte, nao consta no modelo em " + getFullString(idEstagio));
							}


							if (nome.at(0) == "VarDecisaoVI") {

								const Periodo periodo(nome.at(2));

								const IdHidreletrica idHidreletrica = getIdHidreletricaFromChar(nome.at(3).c_str());

								if (periodo.sobreposicao(vetorEstagio.att(idEstagio).getAtributo(AttComumEstagio_periodo_otimizacao, Periodo())) == 0.0)
									throw std::invalid_argument("Periodo " + getFullString(periodo) + " nao compativel com varVI de " + getFullString(idVariavelEstado_corte) + " no corte em " + getFullString(periodo));

								Periodo periodo_estudo_inicial = a_dados.getIterador2Inicial(AttMatrizDados_percentual_duracao_horizonte_estudo, idEstagio, Periodo());

								IdVariavelEstado idVariavelEstado_encontrada = IdVariavelEstado_Nenhum;

								for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVarivelEstado; idVariavelEstado++) {
									const std::string nome_modelo = getAtributo(idEstagio, idVariavelEstado, AttComumVariavelEstado_nome, std::string());
									if (strCompara(nome_modelo, std::string("VarDecisaoVI," + getString(idEstagio) + "," + getString(periodo_estudo_inicial) + "," + getString(idHidreletrica)))) {
										idVariavelEstado_encontrada = idVariavelEstado;
										break;
									}
									else if (strCompara(nome_modelo, std::string("VarDecisaoVI," + getFullString(idEstagio) + "," + getString(periodo_estudo_inicial) + "," + getFullString(idHidreletrica)))) {
										idVariavelEstado_encontrada = idVariavelEstado;
										break;
									}
								}

								if (idVariavelEstado_encontrada != IdVariavelEstado_Nenhum)
									variaveis_estado_modelo_encontradas.setElemento(idVariavelEstado_encontrada, idVariavelEstado_corte);
								else
									vetorEstagio_aux.att(idEstagio).vetorVariavelEstado.rem(idVariavelEstado_corte);

							} // if (nome.at(0) == "VarDecisaoVI") {

							else if (nome.at(0) == "VarDecisaoYP") {

								const Periodo periodo = Periodo(nome.at(2));

								const IdProcessoEstocastico idProcessoEstocastico = getIdProcessoEstocasticoFromChar(nome.at(3).c_str());

								if (idProcessoEstocastico != getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico()))
									throw std::invalid_argument(getFullString(idProcessoEstocastico) + " do acoplamento incompativel com " + getFullString(a_dados.processoEstocastico_hidrologico.getAtributo(AttComumProcessoEstocastico_idProcessoEstocastico, IdProcessoEstocastico())) + " do modelo");

								const IdVariavelAleatoria idVariavelAleatoria = getIdVariavelAleatoriaFromChar(nome.at(4).c_str());

								const Periodo periodo_lag = Periodo(nome.at(5));

								const double grau_liberdade = getdoubleFromChar(nome.at(6).c_str());

								if (nome.size() < 8)
									throw std::invalid_argument(getFullString(idVariavelEstado_corte) + " com termos ausentes de VarDecisaoYP.");

								std::vector<IdHidreletrica> listaHidreletrica;
								std::vector<IdHidreletrica> listaHidreletricaNaoInstanciadaNoModelo;
								for (int i = 7; i < nome.size(); i++) {
									const IdHidreletrica idHidreletrica = getIdHidreletricaFromChar(nome.at(i).c_str());
									if (idHidreletrica == IdHidreletrica_Nenhum)
										throw std::invalid_argument("Nao encontrado IdHidreletrica escrito como " + nome.at(i) + " em " + getFullString(idVariavelEstado_corte));
									else if (!a_dados.vetorHidreletrica.isInstanciado(idHidreletrica))
										listaHidreletricaNaoInstanciadaNoModelo.push_back(idHidreletrica);
									listaHidreletrica.push_back(idHidreletrica);
								}

								if (isIdHidreletricaNosEstadosYP.at(idVariavelAleatoria).size() == 0)
									isIdHidreletricaNosEstadosYP.at(idVariavelAleatoria) = listaHidreletrica;
								else {
									if (!vectorCompara(isIdHidreletricaNosEstadosYP.at(idVariavelAleatoria), listaHidreletrica))
										throw std::invalid_argument("Variavel aleatoria com lista IdHidreletrica diferente de lista ja instanciada em " + getFullString(idVariavelEstado_corte));
								}

								if (listaHidreletricaNaoInstanciadaNoModelo.size() == 1) {
									vetorEstagio_aux.att(idEstagio).anularVariavelEstadoCorteBenders(idVariavelEstado_corte);
									vetorEstagio_aux.att(idEstagio).vetorVariavelEstado.rem(idVariavelEstado_corte);
									if (a_idProcesso == IdProcesso_mestre)
										std::cout << getFullString(listaHidreletricaNaoInstanciadaNoModelo.at(0)) << " nao instanciada. Removendo " << getFullString(idVariavelEstado_corte) << " com abatimento no RHS do corte." << std::endl;
								}
								else if (listaHidreletricaNaoInstanciadaNoModelo.size() > 1)
									throw std::invalid_argument(getFullString(listaHidreletricaNaoInstanciadaNoModelo.at(0)) + " em " + getFullString(idVariavelEstado_corte) + " nao foi instanciado no modelo.");

								else if (listaHidreletricaNaoInstanciadaNoModelo.size() == 0) {
									const int varYP = criarVariaveisDecisao_VariaveisEstado_Restricoes_YP(a_TSS, a_dados, idEstagio, periodo, idProcessoEstocastico, idVariavelAleatoria, periodo_lag, grau_liberdade, listaHidreletrica);
									if (varYP == -1)
										throw std::invalid_argument("Nao foi possivel criar variaveis e restricoes YP de " + getFullString(idVariavelEstado_corte) + " em " + getFullString(idEstagio));
									vetorEstagio_aux.att(idEstagio).setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado_corte, a_TSS, varYP);
								}

							} // else if (nome.at(0) == "VarDecisaoYP") {


						} // if (variaveis_estado_cortes_encontradas.at(idVariavelEstado_corte) == IdVariavelEstado_Nenhum) {

					} // for (IdVariavelEstado idVariavelEstado_corte = IdVariavelEstado_1; idVariavelEstado_corte <= maiorIdVarivelEstado_corte; idVariavelEstado_corte++) {

				} // if (numero_variaveis_estado_cortes_encontradas != int(maiorIdVarivelEstado_corte)) {


				IdCorteBenders idCorteBenders_sequencial = IdCorteBenders_Nenhum;
				for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {
					if (vetorEstagio_aux.att(idEstagio).vetorCorteBenders.isInstanciado(idCorteBenders) && (idCorteBenders <= maximoIdCorteBenders)) {
						if (idCorteBenders_sequencial == IdCorteBenders_Nenhum)
							idCorteBenders_sequencial = IdCorteBenders_1;
						else
							idCorteBenders_sequencial++;

						if (modelo_e_cortes_estados_identicos) {
							CorteBenders corteBenders(vetorEstagio_aux.att(idEstagio).vetorCorteBenders.at(idCorteBenders));
							corteBenders.setAtributo(AttComumCorteBenders_idCorteBenders, idCorteBenders_sequencial);
							vetorEstagio.att(idEstagio).vetorCorteBenders.add(corteBenders);
						} // if (modelo_e_cortes_estados_identicos) {

						else if (!modelo_e_cortes_estados_identicos) {

							CorteBenders corteBenders;
							corteBenders.setAtributo(AttComumCorteBenders_idCorteBenders, idCorteBenders_sequencial);

							const IdRealizacao idRealizacao_inicial = vetorEstagio_aux.att(idEstagio).getIterador1Inicial(idCorteBenders, AttMatrizCorteBenders_coeficiente, IdRealizacao());
							const IdRealizacao idRealizacao_final   = vetorEstagio_aux.att(idEstagio).getIterador1Final(idCorteBenders, AttMatrizCorteBenders_coeficiente, IdRealizacao());

							for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= variaveis_estado_modelo_encontradas.getIteradorFinal(); idVariavelEstado++) {

								if (variaveis_estado_modelo_encontradas.at(idVariavelEstado) == IdVariavelEstado_Nenhum) {
									corteBenders.addElemento(AttVetorCorteBenders_estado, idVariavelEstado, 0.0);

									for (IdRealizacao idRealizacao = idRealizacao_inicial; idRealizacao <= idRealizacao_final; idRealizacao++)
										corteBenders.addElemento(AttMatrizCorteBenders_coeficiente, idRealizacao, idVariavelEstado, 0.0);

								}

								else if (variaveis_estado_modelo_encontradas.at(idVariavelEstado) != IdVariavelEstado_Nenhum) {
									corteBenders.addElemento(AttVetorCorteBenders_estado, idVariavelEstado, vetorEstagio_aux.att(idEstagio).getElementoVetor(idCorteBenders, AttVetorCorteBenders_estado, variaveis_estado_modelo_encontradas.at(idVariavelEstado), double()));

									for (IdRealizacao idRealizacao = idRealizacao_inicial; idRealizacao <= idRealizacao_final; idRealizacao++)
										corteBenders.addElemento(AttMatrizCorteBenders_coeficiente, idRealizacao, idVariavelEstado, vetorEstagio_aux.att(idEstagio).getElementoMatriz(idCorteBenders, AttMatrizCorteBenders_coeficiente, idRealizacao, variaveis_estado_modelo_encontradas.at(idVariavelEstado), double()));

								} // if (variaveis_estado_modelo_encontradas.at(idVariavelEstado) != IdVariavelEstado_Nenhum) {

							} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= variaveis_estado_modelo_encontradas.getIteradorFinal(); idVariavelEstado++) {

							for (IdRealizacao idRealizacao = idRealizacao_inicial; idRealizacao <= idRealizacao_final; idRealizacao++)
								corteBenders.addElemento(AttVetorCorteBenders_rhs, idRealizacao, vetorEstagio_aux.att(idEstagio).getElementoVetor(idCorteBenders, AttVetorCorteBenders_rhs, idRealizacao, double()));

							vetorEstagio.att(idEstagio).vetorCorteBenders.add(corteBenders);

						} // else if (!modelo_e_cortes_estados_identicos) {
					}
				} // for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {

				requestCorteBenders(a_idProcesso, idEstagio, a_diretorio_impressao_selecao_cortes, a_entradaSaidaDados);

				if (getAtributo(idEstagio, AttComumEstagio_selecao_cortes_nivel_dominancia, int()) == 0)
					removerCorteBenders(idEstagio);

			} // if ((estagio_inicial != IdEstagio_1) || (idEstagio > estagio_inicial)) {

		} // for (IdEstagio idEstagio = IdEstagio_1; idEstagio <= estagio_final; idEstagio++) {

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::importarCorteBenders(" + getFullString(a_TSS) + "," + a_diretorio_impressao_selecao_cortes + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::importarCorteBenders(const std::string a_diretorio_recuperacao_cortes, EntradaSaidaDados a_entradaSaidaDados){

void ModeloOtimizacao::importarCorteBenders_AcoplamentoPosEstudo(const TipoSubproblemaSolver a_TSS, const IdProcesso a_idProcesso, const IdIteracao a_idIteracao, const std::string a_diretorio_impressao_selecao_cortes, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const string diretorio_importacao_pos_estudo = getAtributo(AttComumModeloOtimizacao_diretorio_importacao_pos_estudo, std::string());

		if (strCompara(diretorio_importacao_pos_estudo, "nenhum"))
			return;

		const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

		a_entradaSaidaDados.setDiretorioEntrada(diretorio_importacao_pos_estudo);

		IdEstagio idEstagio_futuro = getMaiorId(IdEstagio());

		if (idEstagio_futuro != IdEstagio(estagio_final + 1))
			throw std::invalid_argument("Cortes de Benders pos-estudo devem ser importados em " + getFullString(IdEstagio(estagio_final + 1)) + ".");

		bool impressao = false;

		bool estagio_carregado_sem_iterador = false;

		bool estagio_carregado = false;
		bool rhs_carregado = false;
		bool std_carregado = false;
		bool cof_carregado = false;
		Estagio estagio_auxiliar;
		while (true) {

			bool cortes_encontrados = false;

			std::string prefixo = "";

			for (int pref = 0; pref < int(IdEstagio_Excedente); pref++) {

				if (pref > 0)
					prefixo = getFullString(IdEstagio(pref)) + "_";

				if (a_idIteracao != IdIteracao_Nenhum) {

					if (!estagio_carregado)
						estagio_carregado = a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir(prefixo + "estagio_" + getFullString(a_idIteracao) + ".csv", estagio_auxiliar, TipoAcessoInstancia_direto);

					if (estagio_carregado) {

						rhs_carregado = vetorEstagio.att(idEstagio_futuro).carregarEstadosCortesBenders(a_entradaSaidaDados.getDiretorioEntrada() + "//" + prefixo + "corteBenders_rhs_" + getFullString(a_idIteracao) + ".csv");
						std_carregado = vetorEstagio.att(idEstagio_futuro).carregarEstadosCortesBenders(a_entradaSaidaDados.getDiretorioEntrada() + "//" + prefixo + "corteBenders_estado_" + getFullString(a_idIteracao) + ".csv");
						cof_carregado = vetorEstagio.att(idEstagio_futuro).carregarCoeficientesCortesBenders(a_entradaSaidaDados.getDiretorioEntrada() + "//" + prefixo + "corteBenders_coeficientes_" + getFullString(a_idIteracao) + ".csv");

					} // if (estagio_carregado) {

					if ((rhs_carregado) && (cof_carregado)) {
						cortes_encontrados = true;
						break;
					}

					if (a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir(prefixo + "estagio.csv", estagio_auxiliar, TipoAcessoInstancia_direto))
						return;

				} // if (a_idIteracao != IdIteracao_Nenhum) {

				else {

					if (!estagio_carregado) {
						estagio_carregado_sem_iterador = a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir(prefixo + "estagio.csv", estagio_auxiliar, TipoAcessoInstancia_direto);

						if (estagio_carregado_sem_iterador) {

							vetorEstagio.att(idEstagio_futuro).carregarRHSCortesBenders(a_entradaSaidaDados.getDiretorioEntrada() + "//" + prefixo + "corteBenders_rhs.csv");
							vetorEstagio.att(idEstagio_futuro).carregarEstadosCortesBenders(a_entradaSaidaDados.getDiretorioEntrada() + "//" + prefixo + "corteBenders_estado.csv");
							vetorEstagio.att(idEstagio_futuro).carregarCoeficientesCortesBenders(a_entradaSaidaDados.getDiretorioEntrada() + "//" + prefixo + "corteBenders_coeficientes.csv");

							cortes_encontrados = true;
							break;
						}

					}

				}

			} // for (int pref = 0; pref < int(IdEstagio_Excedente); pref++) {

			if (cortes_encontrados) {
				if (a_idProcesso == IdProcesso_mestre) {
					std::cout << std::endl;
					if (prefixo == "")
						std::cout << "Cortes de Benders pos-estudo detectados." << std::endl;
					else
						std::cout << "Cortes de Benders pos-estudo detectados. Prefixo: " << prefixo << std::endl;
				}
				break;
			}

			if ((!impressao) && (a_idProcesso == IdProcesso_mestre)) {
				if (a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("eop.txt", estagio_auxiliar, TipoAcessoInstancia_direto))
					return;
				std::cout << "Aguardando Cortes de Benders no diretorio: " << diretorio_importacao_pos_estudo << std::endl;
				std::cout << "Caso NAO haja acoplamento pos-estudo, pressione Ctrl+C para sair e altere valor de " << getFullString(AttComumDados_diretorio_importacao_pos_estudo) << " para: Nenhum" << std::endl;
				impressao = true;
			} // if ((!impressao) && (a_idProcesso == IdProcesso_mestre)) {

			aguardarTempo(5000);

		} // while (true) {


		const IdCorteBenders maximoIdCorteBenders = getAtributo(AttComumModeloOtimizacao_maior_corte_importado_pos_estudo, IdCorteBenders());

		IdRealizacao maiorIdRealizacao_corte = IdRealizacao_Nenhum;

		for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maximoIdCorteBenders; idCorteBenders++) {
			if (vetorEstagio.att(idEstagio_futuro).vetorCorteBenders.isInstanciado(idCorteBenders)) {
				const IdRealizacao idRealizacao = vetorEstagio.att(idEstagio_futuro).getIterador1Final(idCorteBenders, AttMatrizCorteBenders_coeficiente, IdRealizacao());
				if (idRealizacao > maiorIdRealizacao_corte)
					maiorIdRealizacao_corte = idRealizacao;
			}
		} // for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {

		if (maiorIdRealizacao_corte == IdRealizacao_Nenhum)
			throw std::invalid_argument("Nenhum corte foi importado.");

		vetorEstagio.att(idEstagio_futuro).setAtributo(AttComumEstagio_maiorIdRealizacao, maiorIdRealizacao_corte);

		if (maximoIdCorteBenders < IdCorteBenders(IdCorteBenders_Excedente - 1)) {
			for (IdCorteBenders idCorteBenders = IdCorteBenders(maximoIdCorteBenders + 1); idCorteBenders <= getMaiorId(idEstagio_futuro, IdCorteBenders()); idCorteBenders++)
				vetorEstagio.att(idEstagio_futuro).vetorCorteBenders.rem(idCorteBenders);
		} // if (maximoIdCorteBenders < IdCorteBenders(IdCorteBenders_Excedente - 1)) {

		for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= getMaiorId(idEstagio_futuro, IdCorteBenders()); idCorteBenders++) {
			if (vetorEstagio.att(idEstagio_futuro).vetorCorteBenders.isInstanciado(idCorteBenders))
				vetorEstagio.att(idEstagio_futuro).avaliarDominanciaCorteBendersParaEstado(idCorteBenders, idCorteBenders);
		}

		const Periodo periodo_otimizacao_estagio_final = getAtributo(estagio_final, AttComumEstagio_periodo_otimizacao, Periodo());

		if (getVarDecisao_ZFseExistir(a_TSS, estagio_final, periodo_otimizacao_estagio_final) == -1) {

			// Variável ZF
			addVarDecisao_ZF(a_TSS, estagio_final, periodo_otimizacao_estagio_final, 0.0, vetorEstagio.att(estagio_final).getSolver(a_TSS)->getInfinito(), 0.0);

			// Variável ZF em EquLinear_CUSTO_TOTAL
			vetorEstagio.att(estagio_final).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZF(a_TSS, estagio_final, periodo_otimizacao_estagio_final), getEquLinear_CUSTO_TOTAL(a_TSS, estagio_final, periodo_otimizacao_estagio_final), -1.0);

			criarRestricoesCorteBendersEmCustoFuturo(a_TSS, estagio_final);

			const int posEquZF = addEquLinear_CUSTO_FUTURO(a_TSS, estagio_final, periodo_otimizacao_estagio_final);

			vetorEstagio.att(estagio_final).getSolver(a_TSS)->setRHSRestricao(posEquZF, 0.0);

			// Variável ZF
			vetorEstagio.att(estagio_final).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZF(a_TSS, estagio_final, periodo_otimizacao_estagio_final), posEquZF, 1.0);

			for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao_corte; idRealizacao++) {
				int varZF = getVarDecisao_ZFseExistir(a_TSS, estagio_final, periodo_otimizacao_estagio_final, idRealizacao);
				if (varZF == -1)
					varZF = addVarDecisao_ZF(a_TSS, estagio_final, periodo_otimizacao_estagio_final, idRealizacao, 0.0, vetorEstagio.att(estagio_final).getSolver(a_TSS)->getInfinito(), 0.0);
				vetorEstagio.att(estagio_final).getSolver(a_TSS)->setCofRestricao(varZF, posEquZF, -(1.0 / double(maiorIdRealizacao_corte)));
			}

		} // if (getVarDecisao_ZFseExistir(estagio_final, periodo_otimizacao_estagio_final) == -1) {
		 
		const int numero_cortes = vetorEstagio.att(idEstagio_futuro).vetorCorteBenders.numObjetos();

		// Todos os cortes são instanciados de maneira estática (maior performance)
		if ((getAtributo(idEstagio_futuro, AttComumEstagio_selecao_cortes_nivel_dominancia, int()) == 0) || (!std_carregado)) {

			const int numero_cortes_total = numero_cortes * int(maiorIdRealizacao_corte);
			int indice_corte = -1;

			if (true) {
				std::vector<std::string> nomes(numero_cortes_total, "");

				int cont = 0;
				for (IdRealizacao idRealizacao = maiorIdRealizacao_corte; idRealizacao >= IdRealizacao_1; idRealizacao--) {
					for (IdCorteBenders idCorteBenders = getMaiorId(idEstagio_futuro, IdCorteBenders()); idCorteBenders >= getMenorId(idEstagio_futuro, IdCorteBenders()); idCorteBenders--) {
						if (vetorEstagio.att(idEstagio_futuro).vetorCorteBenders.isInstanciado(idCorteBenders)) {
							nomes.at(cont) = getNomeIneLinear_CB_ZF(a_TSS, estagio_final, periodo_otimizacao_estagio_final, idRealizacao, idCorteBenders);
							cont++;
						}
					}
				}
				indice_corte = vetorEstagio.att(estagio_final).getSolver(a_TSS)->addConstrsMaior(nomes) - numero_cortes_total + 1;
			} // if (true) {

			for (IdRealizacao idRealizacao = maiorIdRealizacao_corte; idRealizacao >= IdRealizacao_1; idRealizacao--) {

				const int varZF = getVarDecisao_ZF(a_TSS, estagio_final, periodo_otimizacao_estagio_final, idRealizacao);

				for (IdCorteBenders idCorteBenders = getMaiorId(idEstagio_futuro, IdCorteBenders()); idCorteBenders >= getMenorId(idEstagio_futuro, IdCorteBenders()); idCorteBenders--) {

					if (vetorEstagio.att(idEstagio_futuro).vetorCorteBenders.isInstanciado(idCorteBenders)) {

						double indice_corte_dbl = double(indice_corte);

						addConteudoIters_12(idx_IneLinear_CB_ZF_4.at(a_TSS), indice_corte_dbl, estagio_final, periodo_otimizacao_estagio_final, idRealizacao, idCorteBenders, 1, 1, 1, 1, 1, 1, IdRealizacao_1, IdCenario_1);

						vetorEstagio.att(estagio_final).getSolver(a_TSS)->setCofRestricao(varZF, indice_corte, 1.0);

						for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= getMaiorId(idEstagio_futuro, IdVariavelEstado()); idVariavelEstado++) {

							if (vetorEstagio.att(idEstagio_futuro).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

								const int idVariavelDecisaoEstagioAnterior = getAtributo(idEstagio_futuro, idVariavelEstado, AttComumVariavelEstado_idVariavelDecisaoEstagioAnterior, int());

								if (idVariavelDecisaoEstagioAnterior > -1)
									vetorEstagio.att(estagio_final).getSolver(a_TSS)->setCofRestricao(idVariavelDecisaoEstagioAnterior, indice_corte, -getElementoMatriz(idEstagio_futuro, idCorteBenders, AttMatrizCorteBenders_coeficiente, idRealizacao, idVariavelEstado, double()));
								else if (getSizeVetor(idEstagio_futuro, idCorteBenders, AttVetorCorteBenders_estado) > 0)
									vetorEstagio.att(idEstagio_futuro).anularVariavelEstadoCorteBenders(idVariavelEstado, idCorteBenders);

							} // if (vetorEstagio.att(estagio_final).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

							else if (getSizeVetor(idEstagio_futuro, idCorteBenders, AttVetorCorteBenders_estado) > 0)
								vetorEstagio.att(idEstagio_futuro).anularVariavelEstadoCorteBenders(idVariavelEstado, idCorteBenders);

						} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= getMaiorId(idEstagio_futuro, IdVariavelEstado()); idVariavelEstado++) {

						vetorEstagio.att(estagio_final).getSolver(a_TSS)->setRHSRestricao(indice_corte, getElementoVetor(idEstagio_futuro, idCorteBenders, AttVetorCorteBenders_rhs, idRealizacao, double()));

						indice_corte++;

					} // if (vetorEstagio.att(idEstagio_futuro).vetorCorteBenders.isInstanciado(idCorteBenders)) {

				} // for (IdCorteBenders idCorteBenders = getMaiorId(idEstagio_futuro, IdCorteBenders()); idCorteBenders <= getMenorId(idEstagio_futuro, IdCorteBenders()); idCorteBenders--) {
			} // for (IdRealizacao idRealizacao = maiorIdRealizacao_corte; idRealizacao <= IdRealizacao_1; idRealizacao--) {

		} // if (getAtributo(idEstagio_futuro, AttComumEstagio_selecao_cortes_nivel_dominancia, int()) == 0) {
		else
			requestCorteBenders(a_idProcesso, idEstagio_futuro, a_diretorio_impressao_selecao_cortes, a_entradaSaidaDados);

		vetorEstagio.att(idEstagio_futuro).removerTodosCorteBenders();

		if (a_idProcesso == IdProcesso_mestre)
			std::cout << "Cortes de Benders pos-estudo carregados. " << std::endl;

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::importarCorteBenders_AcoplamentoPosEstudo(" + getFullString(a_TSS) + "," + a_diretorio_impressao_selecao_cortes + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::importarCorteBenders_AcoplamentoPosEstudo(const std::string a_diretorio_recuperacao_cortes, EntradaSaidaDados a_entradaSaidaDados){


void ModeloOtimizacao::importarVariaveisEstado_AcoplamentoPreEstudo(const IdProcesso a_idProcesso, const IdProcesso a_maior_processo, const IdIteracao a_idIteracao, const IdCenario a_cenario_inicial, const IdCenario a_cenario_final, EntradaSaidaDados a_entradaSaidaDados) {
	try {

		const string diretorio_importacao_pre_estudo = getAtributo(AttComumModeloOtimizacao_diretorio_importacao_pre_estudo, std::string());

		if (strCompara(diretorio_importacao_pre_estudo, "nenhum"))
			return;

		const IdEstagio estagio_acoplamento_pre_estudo = getAtributo(AttComumModeloOtimizacao_estagio_acoplamento_pre_estudo, IdEstagio());

		a_entradaSaidaDados.setDiretorioEntrada(diretorio_importacao_pre_estudo);

		Estagio estagio;

		bool impressao = false;
		bool estado_valores_carregados = false;
		Estagio estagio_auxiliar;
		while (true) {

			estado_valores_carregados = a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir(std::string("estado_valores_" + getFullString(a_idIteracao) + ".csv"), estagio, TipoAcessoInstancia_membro);

			if (estado_valores_carregados) {
				if (a_idProcesso == IdProcesso_mestre)
					std::cout << "Variaveis Estado pre-estudo detectadas." << std::endl;
				break;
			}
			else {
				if (a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("eop.txt", estagio_auxiliar, TipoAcessoInstancia_direto))
					return;
			}


			if ((!impressao) && (a_idProcesso == IdProcesso_mestre)) {
				std::cout << "Aguardando variaveis de estado no diretorio: " << diretorio_importacao_pre_estudo << std::endl;
				std::cout << "Caso NAO haja acoplamento pre-estudo, pressione Ctrl+C para sair e altere valor de " << getFullString(AttComumDados_diretorio_importacao_pre_estudo) << " para: Nenhum" << std::endl;
				impressao = true;
			} // if ((!impressao) && (a_idProcesso == IdProcesso_mestre)) {

			aguardarTempo(5000);

		} // while (true) {

		const IdProcessoEstocastico tipo_processo_estocastico_hidrologico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

		for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= getMaiorId(estagio_acoplamento_pre_estudo, IdVariavelEstado()); idVariavelEstado++) {

			if (estagio.vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

				if (estagio.getSizeVetor(idVariavelEstado, AttVetorVariavelEstado_valor) > 0) {

					for (IdCenario idCenario = a_cenario_inicial; idCenario <= a_cenario_final; idCenario++) {

						IdProcesso idProcesso_busca = IdProcesso_Nenhum;
						/*
						for (IdProcesso idProcesso = estagio.getIteradorInicial(IdVariavelEstado_1, AttMatrizVariavelEstado_valor, IdProcesso()); idProcesso <= estagio.getIterador1Final(IdVariavelEstado_1, AttMatrizVariavelEstado_valor, IdProcesso()); idProcesso++) {
							if (estagio.getSize2Matriz(IdVariavelEstado_1, AttMatrizVariavelEstado_valor, idProcesso) > 0) {
								if ((estagio.getIterador2Inicial(IdVariavelEstado_1, AttMatrizVariavelEstado_valor, idProcesso, IdCenario()) <= idCenario) && (idCenario <= estagio.getIterador2Final(IdVariavelEstado_1, AttMatrizVariavelEstado_valor, idProcesso, IdCenario()))) {
									idProcesso_busca = idProcesso;
									break;
								}
							}
						}
						*/
						//vetorEstagio.att(estagio_acoplamento_pre_estudo).addValorVariavelEstado(idVariavelEstado, true, a_idProcesso, a_maior_processo, idCenario, estagio.getElementoMatriz(idVariavelEstado, AttMatrizVariavelEstado_valor, idProcesso_busca, idCenario, double()));

					} // for (IdCenario idCenario = a_cenario_inicial; idCenario <= a_cenario_final; idCenario++) {

				} // if (estagio.getSize1Matriz(idVariavelEstado, AttMatrizVariavelEstado_valor) > 0) {

			} // if (estagio.vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

			else if ((!estagio.vetorVariavelEstado.isInstanciado(idVariavelEstado)) && (a_idIteracao == arranjoResolucao.getAtributo(AttComumArranjoResolucao_iteracao_inicial, IdIteracao())) && (a_idProcesso == IdProcesso_mestre))
				std::cout << "Nao foi instanciado " + getFullString(idVariavelEstado) << "." << std::endl;

		} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= getMaiorId(estagio_acoplamento_pre_estudo, IdVariavelEstado()); idVariavelEstado++) {

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::importarVariaveisEstado_AcoplamentoPreEstudo(a_entradaSaidaDados): \n" + std::string(erro.what())); }
} // void ModeloOtimizacao::exportarEstadosAcoplamentoPreEstudo(const IdProcesso a_idProcesso, EntradaSaidaDados a_entradaSaidaDados) {


void ModeloOtimizacao::exportarVariaveisEstado_AcoplamentoPreEstudo(EntradaSaidaDados a_entradaSaidaDados) {
	try {

		const string diretorio_importacao_pre_estudo = getAtributo(AttComumModeloOtimizacao_diretorio_importacao_pre_estudo, std::string());

		if (strCompara(diretorio_importacao_pre_estudo, "nenhum"))
			return;

		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//AcoplamentoPreEstudo");

		const IdEstagio estagio_acoplamento_pre_estudo = getAtributo(AttComumModeloOtimizacao_estagio_acoplamento_pre_estudo, IdEstagio());

		if ((estagio_acoplamento_pre_estudo >= getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio())) && (estagio_acoplamento_pre_estudo <= getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio()))) {

			if (vetorEstagio.att(estagio_acoplamento_pre_estudo).vetorVariavelEstado.getMaiorId() != IdVariavelEstado_Nenhum) {

				if (getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio()) == IdEstagio_1) {

					a_entradaSaidaDados.imprimirArquivoCSV_AttComum("estagio.csv", vetorEstagio.att(estagio_acoplamento_pre_estudo), std::vector<AttComumEstagio>{ AttComumEstagio_idEstagio, AttComumEstagio_periodo_otimizacao, AttComumEstagio_selecao_cortes_nivel_dominancia, AttComumEstagio_cortes_multiplos, AttComumEstagio_alpha_CVAR, AttComumEstagio_lambda_CVAR});
					a_entradaSaidaDados.imprimirArquivoCSV_AttComum("estado.csv", IdVariavelEstado_Nenhum, vetorEstagio.att(estagio_acoplamento_pre_estudo));

				}
				else {
					a_entradaSaidaDados.imprimirArquivoCSV_AttComum("estagio_" + getFullString(arranjoResolucao.getAtributo(AttComumArranjoResolucao_iteracao_inicial, IdIteracao())) + ".csv", vetorEstagio.att(estagio_acoplamento_pre_estudo), std::vector<AttComumEstagio>{ AttComumEstagio_idEstagio, AttComumEstagio_periodo_otimizacao, AttComumEstagio_selecao_cortes_nivel_dominancia, AttComumEstagio_cortes_multiplos, AttComumEstagio_alpha_CVAR, AttComumEstagio_lambda_CVAR});
					a_entradaSaidaDados.imprimirArquivoCSV_AttComum("estado_" + getFullString(arranjoResolucao.getAtributo(AttComumArranjoResolucao_iteracao_inicial, IdIteracao())) + ".csv", IdVariavelEstado_Nenhum, vetorEstagio.att(estagio_acoplamento_pre_estudo));
				}

			} // if (vetorEstagio.att(estagio_acoplamento_pre_estudo).vetorVariavelEstado.getMaiorId() != IdVariavelEstado_Nenhum) {

		} // if (estagio_acoplamento_pre_estudo <= getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio())) {

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::exportarVariaveisEstado_AcoplamentoPreEstudo(a_entradaSaidaDados): \n" + std::string(erro.what())); }
} // void ModeloOtimizacao::exportarEstadosAcoplamentoPreEstudo(const IdProcesso a_idProcesso, EntradaSaidaDados a_entradaSaidaDados) {


void ModeloOtimizacao::importarVariaveisEstado_AcoplamentoPosEstudo(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdProcesso a_idProcesso, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const string diretorio_importacao_pos_estudo = getAtributo(AttComumModeloOtimizacao_diretorio_importacao_pos_estudo, std::string());

		if (strCompara(diretorio_importacao_pos_estudo, "nenhum"))
			return;

		const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
		const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

		a_entradaSaidaDados.setDiretorioEntrada(diretorio_importacao_pos_estudo);

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		// Instanciar estagio adicional a conter os cortes e estados do acoplamento.

		bool estagio_carregado_sem_iterador = false;
		bool estagio_carregado = false;

		IdEstagio idEstagio_futuro = getMaiorId(IdEstagio());

		if (estagio_final == idEstagio_futuro) {

			Estagio estagio;

			std::string prefixo = "";

			bool impressao = false;
			while (true) {

				prefixo = "";

				bool variaveis_encontradas = false;

				for (int pref = 0; pref < int(IdEstagio_Excedente); pref++) {

					if (pref > 0)
						prefixo = getFullString(IdEstagio(pref)) + "_";

					estagio_carregado_sem_iterador = a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir(prefixo + "estagio.csv", estagio, TipoAcessoInstancia_direto);

					if (!estagio_carregado_sem_iterador)
						estagio_carregado = a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir(prefixo + "estagio_" + getFullString(arranjoResolucao.getAtributo(AttComumArranjoResolucao_iteracao_inicial, IdIteracao())) + ".csv", estagio, TipoAcessoInstancia_direto);

					if (estagio_carregado_sem_iterador || estagio_carregado) {
						if (a_idProcesso == IdProcesso_mestre) {
							std::cout << std::endl;
							if (prefixo == "")
								std::cout << "Variaveis Estado pos-estudo detectados." << std::endl;
							else
								std::cout << "Variaveis Estado pos-estudo detectados. Prefixo: " << prefixo << std::endl;
						}
						variaveis_encontradas = true;
						break;
					}

				}
				if (variaveis_encontradas)
					break;


				if ((!impressao) && (a_idProcesso == IdProcesso_mestre)) {
					std::cout << "Aguardando variaveis de estado no diretorio: " << diretorio_importacao_pos_estudo << std::endl;
					std::cout << "Caso NAO haja acoplamento pos-estudo, pressione Ctrl+C para sair e altere valor de " << getFullString(AttComumDados_diretorio_importacao_pos_estudo) << " para: Nenhum" << std::endl;
					impressao = true;
				} // if ((!impressao) && (a_idProcesso == IdProcesso_mestre)) {

				aguardarTempo(5000);

			} // while (true) {

			if (true) {

				const Periodo periodo_otimizacao = getAtributo(estagio_final, AttComumEstagio_periodo_otimizacao, Periodo());
				const Periodo periodo_otimizacao_lido = estagio.getAtributo(AttComumEstagio_periodo_otimizacao, Periodo());

				if (Periodo(periodo_otimizacao_lido.getTipoPeriodo(), periodo_otimizacao + 1) != periodo_otimizacao_lido)
					throw std::invalid_argument("Periodo " + getString(periodo_otimizacao_lido) + " importado nao compativel com final do horizonte de otimizacao.");

			} // if (true) {

			if (estagio_carregado_sem_iterador)
				a_entradaSaidaDados.carregarArquivoCSV_AttComum(prefixo + "estado.csv", estagio, TipoAcessoInstancia_membro);
			else
				a_entradaSaidaDados.carregarArquivoCSV_AttComum(prefixo + "estado_" + getFullString(arranjoResolucao.getAtributo(AttComumArranjoResolucao_iteracao_inicial, IdIteracao())) + ".csv", estagio, TipoAcessoInstancia_membro);


			idEstagio_futuro = IdEstagio(estagio_final + 1);
			const IdEstagio idEstagio = estagio_final;

			estagio.setAtributo(AttComumEstagio_idEstagio, idEstagio_futuro);

			const Periodo periodo_estudo_inicial = getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_estudo, idEstagio, Periodo());
			const Periodo periodo_estudo_final = getIterador2Final(AttMatrizModeloOtimizacao_horizonte_estudo, idEstagio, Periodo());

			const Periodo periodo_horizonte_estudo_inicial = getElementosMatriz(AttMatrizModeloOtimizacao_horizonte_estudo, getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio()), Periodo(), double()).getIteradorInicial();

			SmartEnupla<IdVariavelAleatoria, std::vector<IdHidreletrica>> isIdHidreletricaNosEstadosYP(IdVariavelAleatoria_1, std::vector<std::vector<IdHidreletrica>>(IdVariavelAleatoria(IdVariavelAleatoria_Excedente - 1), std::vector<IdHidreletrica>()));

			bool is_conversao_ENA_carregado = false;

			const IdCenario cenarioInicial = IdCenario_1;
			const IdCenario cenarioFinal = arranjoResolucao.getAtributo(AttComumArranjoResolucao_maior_cenario, IdCenario());
			const SmartEnupla<IdCenario, double>enupla_inicializacao(cenarioInicial, std::vector<double>(int(cenarioFinal - cenarioInicial) + 1, 0.0));

			// Validar se estados possuem variáveis de decisão no modelo e ajustar cortes se necessário.
			for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= estagio.getMaiorId(IdVariavelEstado()); idVariavelEstado++) {

				const std::vector<std::string> nome = estagio.getNomeVariavelEstado(idVariavelEstado);

				//
				// Volume Minimo
				//
				
				if (nome.at(0) == "VarDecisaoZP0_VF_FINF") {
				
					const Periodo periodo_penalizacao(nome.at(2));

					const int varZP0_VF_FINF = criarVariaveisDecisao_VariaveisEstado_Restricoes_ZP0_VF_FINF(a_TSS, a_dados, idEstagio, periodo_penalizacao);

					if (varZP0_VF_FINF > -1)
						estagio.setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado, a_TSS, varZP0_VF_FINF);
				
					else{
						estagio.anularVariavelEstadoCorteBenders(idVariavelEstado);
						estagio.vetorVariavelEstado.rem(idVariavelEstado);
						if (a_idProcesso == IdProcesso_mestre)
							std::cout << "VarDecisaoZP0_VF_FINF " << getFullString(idVariavelEstado) << " nao criada por falta de restricao de volume util minimo no modelo." << std::endl;
					}				

				} // if (nome.at(0) == "VarDecisaoZP0_VF_FINF") {
				

				//
				// Volume Inicial
				//

				else if (nome.at(0) == "VarDecisaoVI") {

					const Periodo periodo(nome.at(2));

					const IdHidreletrica idHidreletrica = getIdHidreletricaFromChar(nome.at(3).c_str());

					if (Periodo(periodo.getTipoPeriodo(), periodo_estudo_final + 1) != periodo)
						throw std::invalid_argument("Periodo estudo invalido " + getFullString(periodo_estudo_final) + " para o acoplamento de varVI de " + getFullString(idVariavelEstado) + " em " + getFullString(periodo));

					estagio.setVariavelDecisaoEmVariavelEstado(idVariavelEstado, a_TSS, -1);

					const int varVF = getVarDecisao_VFseExistir(a_TSS, idEstagio, periodo_estudo_final, idHidreletrica);

					if (varVF > -1)
						estagio.setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado, a_TSS, varVF);

					else if (varVF == -1) {

						const int varVF_nova = addVarDecisao_VF(a_TSS, idEstagio, periodo_estudo_final, idHidreletrica, 0.0, 0.0, 0.0);

						estagio.setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado, a_TSS, varVF_nova);

					} // else if (varVF == -1)

				} // if (nome.at(0) == "VarDecisaoVI") {

				//
				// Afluencia Processo Estocastico
				//

				else if (nome.at(0) == "VarDecisaoYP") {

					const Periodo periodo = Periodo(nome.at(2));

					const Periodo periodo_lag = Periodo(nome.at(3));

					const IdProcessoEstocastico idProcessoEstocastico = getIdProcessoEstocasticoFromChar(nome.at(4).c_str());

					if (idProcessoEstocastico != getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico()))
						throw std::invalid_argument(getFullString(idProcessoEstocastico) + " do acoplamento incompativel com " + getFullString(a_dados.processoEstocastico_hidrologico.getAtributo(AttComumProcessoEstocastico_idProcessoEstocastico, IdProcessoEstocastico())) + " do modelo");

					const IdVariavelAleatoria idVariavelAleatoria = getIdVariavelAleatoriaFromChar(nome.at(5).c_str());

					const double grau_liberdade = getdoubleFromChar(nome.at(6).c_str());

					if (nome.size() < 8)
						throw std::invalid_argument(getFullString(idVariavelEstado) + " com termos ausentes de VarDecisaoYP.");

					std::vector<IdHidreletrica> listaHidreletrica;
					std::vector<IdHidreletrica> listaHidreletricaNaoInstanciadaNoModelo;
					for (int i = 7; i < nome.size(); i++) {
						const IdHidreletrica idHidreletrica = getIdHidreletricaFromChar(nome.at(i).c_str());
						if (idHidreletrica == IdHidreletrica_Nenhum)
							throw std::invalid_argument("Nao encontrado IdHidreletrica escrito como " + nome.at(i) + " em " + getFullString(idVariavelEstado));
						else if (!a_dados.vetorHidreletrica.isInstanciado(idHidreletrica))
							listaHidreletricaNaoInstanciadaNoModelo.push_back(idHidreletrica);
						listaHidreletrica.push_back(idHidreletrica);
					}

					if (isIdHidreletricaNosEstadosYP.at(idVariavelAleatoria).size() == 0)
						isIdHidreletricaNosEstadosYP.at(idVariavelAleatoria) = listaHidreletrica;
					else {
						if (!vectorCompara(isIdHidreletricaNosEstadosYP.at(idVariavelAleatoria), listaHidreletrica))
							throw std::invalid_argument("Variavel aleatoria com lista IdHidreletrica diferente de lista ja instanciada em " + getFullString(idVariavelEstado));
					}

					if (listaHidreletricaNaoInstanciadaNoModelo.size() == 1) {
						estagio.anularVariavelEstadoCorteBenders(idVariavelEstado);
						estagio.vetorVariavelEstado.rem(idVariavelEstado);
						if (a_idProcesso == IdProcesso_mestre)
							std::cout << getFullString(listaHidreletricaNaoInstanciadaNoModelo.at(0)) << " nao instanciada. Removendo " << getFullString(idVariavelEstado) << " com abatimento no RHS do corte." << std::endl;
					}
					else if (listaHidreletricaNaoInstanciadaNoModelo.size() > 1)
						throw std::invalid_argument(getFullString(listaHidreletricaNaoInstanciadaNoModelo.at(0)) + " em " + getFullString(idVariavelEstado) + " nao foi instanciado no modelo.");

					else if (listaHidreletricaNaoInstanciadaNoModelo.size() == 0) {
						const int varYP = criarVariaveisDecisao_VariaveisEstado_Restricoes_YP(a_TSS, a_dados, idEstagio, periodo, idProcessoEstocastico, idVariavelAleatoria, periodo_lag, grau_liberdade, listaHidreletrica);
						if (varYP == -1)
							throw std::invalid_argument("Nao foi possivel criar variaveis e restricoes YP de " + getFullString(idVariavelEstado) + " em " + getFullString(idEstagio));
						estagio.setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado, a_TSS, varYP);
					}
				} // else if (nome.at(0) == "VarDecisaoYP") {

				//
				// Energia Natural Afluente Acoplamento
				//

				else if (nome.at(0) == "VarDecisaoENA") {

					try {

						const Periodo periodo = Periodo(nome.at(2));

						const Periodo periodo_lag = Periodo(nome.at(3));

						const IdReservatorioEquivalente idREE = getIdReservatorioEquivalenteFromChar(nome.at(4).c_str());

						bool any_UHE_REE = false;

						for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); a_dados.vetorHidreletrica.incr(idHidreletrica)) {					


							//
							// Verifica necesidade de carregar fatores de conversão
							//

							bool carregar_conversao_ENA = false;
							if ((!is_conversao_ENA_carregado) && (a_dados.getMaiorId(idHidreletrica, IdReservatorioEquivalente()) == IdReservatorioEquivalente_Nenhum))
								carregar_conversao_ENA = true;
							
							if (carregar_conversao_ENA) {
								if (!a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("HIDRELETRICA_REE_conversao_ENA_acoplamento.csv", a_dados, TipoAcessoInstancia_membroMembro))
									throw std::invalid_argument("Nao foi possivel carregar arquivo HIDRELETRICA_REE_conversao_ENA_acoplamento.csv necessario para conversao de " + getFullString(idVariavelEstado) + " em " + getFullString(idEstagio));
								is_conversao_ENA_carregado = true;

								estagio.vetorRestricaoCenario.alocar(int(IdRestricaoCenario_Excedente) - 1);

							}

							if (a_dados.isInstanciado(idHidreletrica, idREE)) {

								any_UHE_REE = true;

								bool is_valores_0 = false;
								bool is_valores_1 = false;

								if (a_dados.getSize1Matriz(idHidreletrica, idREE, AttMatrizReservatorioEquivalente_conversao_ENA_acoplamento_0) > 0)
									is_valores_0 = true;
								if (a_dados.getSize1Matriz(idHidreletrica, idREE, AttMatrizReservatorioEquivalente_conversao_ENA_acoplamento_1) > 0)
									is_valores_1 = true;

								const int varENA = addVarDecisao_ENA(a_TSS, idEstagio, periodo, periodo_lag, idHidreletrica, idREE, -vetorEstagio.att(idEstagio).getSolver(a_TSS)->getInfinito(), vetorEstagio.att(idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);

								const int equENA = addEquLinear_ENA(a_TSS, idEstagio, periodo, periodo_lag, idHidreletrica, idREE);

								vetorEstagio.att(idEstagio).getSolver(a_TSS)->setCofRestricao(varENA, equENA, 1.0);

								SmartEnupla<IdCenario, double> valores_0;
								SmartEnupla<int, SmartEnupla<IdCenario, double>> valores_1;

								const int varYP = criarVariaveisDecisao_VariaveisEstado_Restricoes_YP(a_TSS, a_dados, idEstagio, periodo, IdProcessoEstocastico_hidrologico_hidreletrica, IdVariavelAleatoria(idHidreletrica), periodo_lag, 0.0, std::vector<IdHidreletrica>{idHidreletrica});
								if (varYP == -1)
									throw std::invalid_argument("Nao foi possivel criar variaveis e restricoes YP de " + getFullString(periodo_lag) + "," + getFullString(idVariavelEstado) + " em " + getFullString(idEstagio));


								for (IdCenario idCenario = cenarioInicial; idCenario <= cenarioFinal; idCenario++) {

									if (is_valores_0) {
										if (valores_0.size() == 0)
											valores_0 = enupla_inicializacao;
										valores_0.at(idCenario) = a_dados.getElementoMatriz(idHidreletrica, idREE, AttMatrizReservatorioEquivalente_conversao_ENA_acoplamento_0, periodo_lag, idCenario, double());
									}

									if (is_valores_1) {
										if (valores_1.size() == 0)
											valores_1 = SmartEnupla<int, SmartEnupla<IdCenario, double>>(varYP, std::vector<SmartEnupla<IdCenario, double>>(1, enupla_inicializacao));

										else if (varYP < valores_1.getIteradorInicial()) {

											SmartEnupla<int, SmartEnupla<IdCenario, double>> valores_1_aux(varYP, std::vector<SmartEnupla<IdCenario, double>>(valores_1.getIteradorFinal() - varYP + 1, SmartEnupla<IdCenario, double>()));

											valores_1_aux.at(varYP) = enupla_inicializacao;

											for (int pos = valores_1.getIteradorInicial(); pos <= valores_1.getIteradorFinal(); pos++)
												valores_1_aux.at(pos) = valores_1.at(pos);

											valores_1 = valores_1_aux;
										}

										else if (valores_1.getIteradorFinal() < varYP) {
											for (int pos = valores_1.getIteradorFinal() + 1; pos < varYP; pos++)
												valores_1.addElemento(pos, SmartEnupla<IdCenario, double>());
											valores_1.addElemento(varYP, enupla_inicializacao);
										}

										valores_1.at(varYP).at(idCenario) = -a_dados.getElementoMatriz(idHidreletrica, idREE, AttMatrizReservatorioEquivalente_conversao_ENA_acoplamento_1, periodo_lag, idCenario, double());
									}

								} // for (IdCenario idCenario = cenarioInicial; idCenario <= cenarioFinal; idCenario++) {


								vetorEstagio.att(idEstagio).addRestricaoCenario(a_TSS, getNomeVarDecisao_ENA(a_TSS, idEstagio, periodo, periodo_lag, idHidreletrica, idREE), equENA, valores_0, valores_1);

								int varENA_REE = getVarDecisao_ENAseExistir(a_TSS, idEstagio, periodo, periodo_lag, idREE);
								int equENA_REE = getEquLinear_ENAseExistir(a_TSS, idEstagio, periodo, periodo_lag, idREE);

								if (varENA_REE == -1) {
									equENA_REE = addEquLinear_ENA(a_TSS, idEstagio, periodo, periodo_lag, idREE);
									varENA_REE = addVarDecisao_ENA(a_TSS, idEstagio, periodo, periodo_lag, idREE, -vetorEstagio.att(idEstagio).getSolver(a_TSS)->getInfinito(), vetorEstagio.att(idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);
									vetorEstagio.att(idEstagio).getSolver(a_TSS)->setCofRestricao(varENA_REE, equENA_REE, 1.0);

									int varENA_SIN = getVarDecisao_ENAseExistir(a_TSS, idEstagio, periodo, periodo_lag);
									int equENA_SIN = getEquLinear_ENAseExistir(a_TSS, idEstagio, periodo, periodo_lag);
									if (varENA_SIN == -1) {
										equENA_SIN = addEquLinear_ENA(a_TSS, idEstagio, periodo, periodo_lag);
										varENA_SIN = addVarDecisao_ENA(a_TSS, idEstagio, periodo, periodo_lag, -vetorEstagio.att(idEstagio).getSolver(a_TSS)->getInfinito(), vetorEstagio.att(idEstagio).getSolver(a_TSS)->getInfinito(), 0.0);
										vetorEstagio.att(idEstagio).getSolver(a_TSS)->setCofRestricao(varENA_SIN, equENA_SIN, 1.0);
									}
									vetorEstagio.att(idEstagio).getSolver(a_TSS)->setCofRestricao(varENA_REE, equENA_SIN, -1.0);

								}

								vetorEstagio.att(idEstagio).getSolver(a_TSS)->setCofRestricao(varENA, equENA_REE, -1.0);

								estagio.setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado, a_TSS, varENA_REE);

							} // if (a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorioEquivalente.isInstanciado(idREE)) {
						}

						if (!any_UHE_REE)
							throw std::invalid_argument("Nao foram encontrados hidreletricas associadas ao REE de " + getFullString(idVariavelEstado) + " em " + getFullString(idEstagio));

					}
					catch (const std::exception& erro) { throw std::invalid_argument("VarDecisaoENA " + getFullString(idVariavelEstado) + " : \n" + std::string(erro.what())); }

				} // else if (nome.at(0) == "VarDecisaoENA") {

				//
				// Defluencia viajante
				//

				else if (nome.at(0) == "VarDecisaoQDEFLAG") {

					const Periodo periodo = Periodo(nome.at(2));
					const Periodo periodo_lag = Periodo(nome.at(3));
					const IdHidreletrica idHidreletrica = getIdHidreletricaFromChar(nome.at(4).c_str());

					if (periodo.getTipoPeriodo() != periodo_lag.getTipoPeriodo())
						throw std::invalid_argument("Ambos os periodos devem ser do mesmo tipo em " + getFullString(idVariavelEstado) + " em " + getFullString(idEstagio));

					const int tempo_viagem_agua_estado = Periodo(TipoPeriodo_horario, periodo) - Periodo(TipoPeriodo_horario, periodo_lag);
					const int tempo_viagem_agua = a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_tempo_viagem_agua, int());

					if (tempo_viagem_agua_estado != tempo_viagem_agua)
						throw std::invalid_argument("Tempo de viagem da agua " + getString(tempo_viagem_agua) + "h em " + getFullString(idHidreletrica) + " nao compativel com " + getString(tempo_viagem_agua_estado) + "h em " + getFullString(idVariavelEstado) + " em " + getFullString(idEstagio));

					const int varQDEFLAG = criarVariaveisDecisao_VariaveisEstado_Restricoes_QDEFLAG(a_TSS, a_dados, idEstagio, periodo, idHidreletrica, periodo_lag);
					if (varQDEFLAG == -1)
						throw std::invalid_argument("Nao foi possivel criar variaveis e restricoes QDEFLAG de " + getFullString(idVariavelEstado) + " em " + getFullString(idEstagio));
					else
						estagio.setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado, a_TSS, varQDEFLAG);

				} // else if (nome.at(0) == "VarDecisaoQDEFLAG") {

				else if (nome.at(0) == "VarDecisaoPTDISPCOM") {

					const Periodo periodo_comando = Periodo(nome.at(2));
					const IdTermeletrica idTermeletrica = getIdTermeletricaFromChar(nome.at(3).c_str());

					const double potencia_minima_disponivel = getdoubleFromChar(nome.at(4).c_str());
					const double potencia_maxima_disponivel = getdoubleFromChar(nome.at(5).c_str());


					const int varPTDISPCOM = criarVariaveisDecisao_VariaveisEstado_Restricoes_PTDISPCOM(a_TSS, a_dados, idEstagio, periodo_comando, IdPatamarCarga_Nenhum, idTermeletrica, potencia_minima_disponivel, potencia_maxima_disponivel);
					if (varPTDISPCOM == -1)
						throw std::invalid_argument("Nao foi possivel criar variaveis e restricoes PTDISPCOM de " + getFullString(idVariavelEstado) + " em " + getFullString(idEstagio));
					else
						estagio.setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado, a_TSS, varPTDISPCOM);

				} // else if (nome.at(0) == "VarDecisaoPTDISPCOM") {

				else
				throw std::invalid_argument("Variavel de estado " + nome.at(0) + " nao compativel com modelo em " + getFullString(idEstagio));

			} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= estagio.getMaiorId(IdVariavelEstado()); idVariavelEstado++) {

			vetorEstagio.add(estagio);

		} // if (estagio_final == idEstagio_futuro) {

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::importarVariaveisEstado_AcoplamentoPosEstudo(" + getFullString(a_TSS) + "," + getFullString(a_idProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::importarVariaveisEstado_AcoplamentoPosEstudo(Dados& a_dados, const IdProcesso a_idProcesso, EntradaSaidaDados a_entradaSaidaDados, const SmartEnupla <Periodo, bool> a_horizonte_defluencia_passada) {


void ModeloOtimizacao::exportarVariaveisEstado_AcoplamentoPosEstudo(const IdIteracao a_idIteracao, EntradaSaidaDados a_entradaSaidaDados) {
	try {

		const string diretorio_importacao_pos_estudo = getAtributo(AttComumModeloOtimizacao_diretorio_importacao_pos_estudo, std::string());

		if (strCompara(diretorio_importacao_pos_estudo, "nenhum"))
			return;

		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//AcoplamentoPosEstudo");

		const IdEstagio idEstagio = getMaiorId(IdEstagio());

		if (a_idIteracao == arranjoResolucao.getAtributo(AttComumArranjoResolucao_iteracao_inicial, IdIteracao())) {
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum("estagio.csv", vetorEstagio.att(idEstagio), std::vector<AttComumEstagio>{AttComumEstagio_idEstagio, AttComumEstagio_periodo_otimizacao});
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum("estado.csv", IdVariavelEstado_Nenhum, vetorEstagio.att(idEstagio));
		} // if (a_idIteracao == getAtributo(AttComumModeloOtimizacao_iteracao_inicial,  IdIteracao())) {

		a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("estado_valores_" + getFullString(a_idIteracao) + ".csv", IdVariavelEstado_Nenhum, vetorEstagio.att(idEstagio), AttVetorVariavelEstado_valor);

		if (a_idIteracao == arranjoResolucao.getAtributo(AttComumArranjoResolucao_iteracao_final, IdIteracao()))
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum("eop.txt", vetorEstagio.att(getMaiorId(IdEstagio())), std::vector<AttComumEstagio>{AttComumEstagio_idEstagio, AttComumEstagio_periodo_otimizacao});

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::exportarVariaveisEstado_AcoplamentoPosEstudo(a_entradaSaidaDados): \n" + std::string(erro.what())); }
} // void ModeloOtimizacao::exportarEstadosAcoplamentoPreEstudo(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados) {

bool ModeloOtimizacao::isVariavelEstadoInstanciada(const IdEstagio a_idEstagio, const IdVariavelEstado a_idVariavelEstado){
	try{

		return vetorEstagio.att(a_idEstagio).vetorVariavelEstado.isInstanciado(a_idVariavelEstado);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::isVariavelEstadoInstanciada(" + getFullString(a_idEstagio) + "," + getFullString(a_idVariavelEstado) + "): \n" + std::string(erro.what())); }
}

VARIAVEL_DECISAO_2(DECLARAR_IMPRIMIR_VALOR)
VARIAVEL_DECISAO_3(DECLARAR_IMPRIMIR_VALOR)
VARIAVEL_DECISAO_4(DECLARAR_IMPRIMIR_VALOR)
VARIAVEL_DECISAO_5(DECLARAR_IMPRIMIR_VALOR)
VARIAVEL_DECISAO_6(DECLARAR_IMPRIMIR_VALOR)

EQUACAO_LINEAR_2(DECLARAR_IMPRIMIR_VALOR)
EQUACAO_LINEAR_3(DECLARAR_IMPRIMIR_VALOR)
EQUACAO_LINEAR_4(DECLARAR_IMPRIMIR_VALOR)
EQUACAO_LINEAR_5(DECLARAR_IMPRIMIR_VALOR)
EQUACAO_LINEAR_6(DECLARAR_IMPRIMIR_VALOR)

INEQUACAO_LINEAR_3(DECLARAR_IMPRIMIR_VALOR)
INEQUACAO_LINEAR_4(DECLARAR_IMPRIMIR_VALOR)
INEQUACAO_LINEAR_5(DECLARAR_IMPRIMIR_VALOR)
INEQUACAO_LINEAR_6(DECLARAR_IMPRIMIR_VALOR)
INEQUACAO_LINEAR_7(DECLARAR_IMPRIMIR_VALOR);




void ModeloOtimizacao::imprimirSolucaoPorEstagioPorCenario_porEstagio(const IdProcesso a_idProcesso, const std::string a_subdiretorio, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida() + "//" + a_subdiretorio;

		for (TipoSubproblemaSolver TSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); TSS < TipoSubproblemaSolver_Excedente; TSS++) {

			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//" + getString(TSS));

			if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, bool())) {

				if ((TSS == TipoSubproblemaSolver_viabilidade_hidraulica) && (!getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()))){}
				else {


					VARIAVEL_DECISAO_2(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO)
						VARIAVEL_DECISAO_3(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO)
						VARIAVEL_DECISAO_4(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO)
						VARIAVEL_DECISAO_5(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO)
						VARIAVEL_DECISAO_6(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO)

				}
			}

			if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario, bool())) {

				EQUACAO_LINEAR_2(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO)
					EQUACAO_LINEAR_3(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO)
					EQUACAO_LINEAR_4(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO)
					EQUACAO_LINEAR_5(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO)
					EQUACAO_LINEAR_6(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO)

					INEQUACAO_LINEAR_3(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO)
					INEQUACAO_LINEAR_4(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO)
					INEQUACAO_LINEAR_5(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO)
					INEQUACAO_LINEAR_6(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO)
					INEQUACAO_LINEAR_7(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO)
			}

		}

	} // try {

	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::imprimirSolucaoPorEstagioPorCenario(" + getString(a_idProcesso) + "," + a_subdiretorio + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::imprimirSolucao(EntradaSaidaDados a_entradaSaidaDados){


void ModeloOtimizacao::imprimirSolucaoPorEstagioPorCenarioPorRealizacao_porEstagio(const IdProcesso a_idProcesso, const std::string a_subdiretorio, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida() + "//" + a_subdiretorio;

		for (TipoSubproblemaSolver TSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); TSS < TipoSubproblemaSolver_Excedente; TSS++) {

			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//" + getString(TSS));

			if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario_por_realizacao, bool())) {

				if ((TSS == TipoSubproblemaSolver_viabilidade_hidraulica) && (!getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()))) {}
				else {


					VARIAVEL_DECISAO_2(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
						VARIAVEL_DECISAO_3(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
						VARIAVEL_DECISAO_4(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
						VARIAVEL_DECISAO_5(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
						VARIAVEL_DECISAO_6(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)

				}
			}

			if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario, bool())) {

				EQUACAO_LINEAR_2(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					EQUACAO_LINEAR_3(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					EQUACAO_LINEAR_4(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					EQUACAO_LINEAR_5(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					EQUACAO_LINEAR_6(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)

					INEQUACAO_LINEAR_3(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					INEQUACAO_LINEAR_4(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					INEQUACAO_LINEAR_5(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					INEQUACAO_LINEAR_6(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					INEQUACAO_LINEAR_7(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
			}

		}

	} // try {

	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::imprimirSolucaoPorEstagioPorCenarioPorRealizacao(" + getString(a_idProcesso) + "," + a_subdiretorio + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::imprimirSolucao(EntradaSaidaDados a_entradaSaidaDados){


bool ModeloOtimizacao::otimizarProblemaComTratamentoNumericoInviabilidade(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio) {
	try {
		const int level = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->otimizarComTratamentoInviabilidade();
		setElemento(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, level);
		return (level > 0);
	}
	catch (const std::exception& erro) {
		throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::otimizaProblemaComTratamentoNumericoInviabilidade(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what()));
	}
}

bool ModeloOtimizacao::otimizarProblemaComTratamentoNumericoInviabilidade(const TipoSubproblemaSolver a_TSS, const int a_level, const IdEstagio a_idEstagio) {

	try {

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS) == nullptr)
			throw std::invalid_argument("Solver invalido (NULO).");

		try {
			if (a_level == 1) {

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->resetar();
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->otimizar();

			} // if (a_level == 1) {

			else {

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setMetodo(TipoMetodoSolver_primal_simplex);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->resetar();
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->otimizar();
			}

		} // try
		catch (const std::exception& erro) {

			const std::string erro_str = std::string(erro.what());

			if (erro_str.size() > 5) {
				if (erro_str.find("10005") != std::string::npos) {
					setElemento(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, -1);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setMetodoPadrao();
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->resetar();
					return false;
				}
			}
		}

		setElemento(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, a_level);

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {
			if (a_level == 0) {
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setMetodoPadrao();
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->resetar();
			}
			return true;
		}

		const int calculoIIS = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->calcularIIS();

		if (calculoIIS == 1) {
			if (a_level == 0) {
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setMetodoPadrao();
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->resetar();
			}
			return false;
		}
		else {
			if (a_level > 0)
				return otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, a_level - 1, a_idEstagio);
			else if (a_level == 0) {
				setElemento(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, -1);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setMetodoPadrao();
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->resetar();
				return false;
			}
		}

		return false;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::otimizaProblemaComTratamentoNumericoInviabilidade(" + getFullString(a_TSS) + "," + getFullString(a_level) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::otimizaProblemaComTratamentoNumericoInviabilidade(Solver* a_ptrSolver){

void ModeloOtimizacao::setTempoLimiteOtimizacao(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const double a_tempo_limite){

	try {

		if (a_tempo_limite <= 0.0)
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setTempoLimite();
		else
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setTempoLimite(a_tempo_limite);

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getTempoOtimizacao(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_tempo_limite) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::setTempoLimiteOtimizacao(const IdEstagio a_idEstagio, const double a_tempo_limite){

double ModeloOtimizacao::getTempoOtimizacao(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio) {

	try {

		return vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getTempoOtimizacao();

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getTempoOtimizacao(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::getTempoOtimizacao(const IdEstagio a_idEstagio){


double ModeloOtimizacao::otimizarProblema(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const std::string a_diretorio) {

	try {

		setElemento(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, 2);

		bool atualizacaoCompletaVariavelRealizacaoInterna = false;
		IdCenario a_idCenario = IdCenario_1;
		resetarVariavelRealizacaoInterna(a_TSS, a_idEstagio);

		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->otimizar();

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() != TipoStatusSolver_nao_otimalidade)
			return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_diretorio);

		//
		// Tratamento Numérico de Inviabilidade
		//

		//otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, 1, a_idEstagio);
		otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, a_idEstagio);

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() != TipoStatusSolver_nao_otimalidade)
			return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_diretorio);

		const std::string codigo = getString(int(time(0)));
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		throw std::invalid_argument("Erro em otimizar, codigo identificador do problema linear " + codigo);

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::otimizarProblema(" + getFullString(a_TSS) + "," + getFullString(a_idIteracao) + "," + getString(a_idEstagio) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::otimizarSubproblema(const IdEstagio a_idEstagio, const IdCenario a_idCenario){



double ModeloOtimizacao::otimizarProblema(const TipoSubproblemaSolver a_TSS, const IdProcesso a_idProcesso, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const std::string a_diretorio){

	try {

		setElemento(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, 2);

		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setMetodoPadrao();

		resetarVariavelRealizacaoInterna(a_TSS, a_idEstagio);

		if ((getElementoVetor(AttVetorModeloOtimizacao_alguma_variavel_aleatoria_hidrologica_com_truncamento, a_idEstagio, int()) == 1) || (getAtributo(AttComumModeloOtimizacao_relaxar_afluencia_incremental_com_viabilidade_hidraulica, bool())))
			atualizarModeloOtimizacaoComVariavelRealizacaoInterna(a_TSS, a_idIteracao, a_idProcesso, a_idEstagio, a_idCenario, IdRealizacao_Nenhum);

		if (getAtributo(AttComumModeloOtimizacao_relaxar_afluencia_incremental_com_viabilidade_hidraulica, bool()))
			atualizarModeloOtimizacaoComVariavelRealizacaoInterna(a_TSS, TipoSubproblemaSolver_viabilidade_hidraulica, a_idIteracao, a_idProcesso, a_idEstagio, a_idCenario, IdRealizacao_Nenhum, a_diretorio);

		try{
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->otimizar();
		} // try
		catch (const std::exception& erro) {

			const std::string erro_str = std::string(erro.what());

			if (erro_str.size() > 5) {
				if (erro_str.find("10005") != std::string::npos) {

					escreverLog10005(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, IdRealizacao_Nenhum, erro_str);

					//otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, 1, a_idEstagio);
					otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, a_idEstagio);

					if (getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int()) >= 0)
						return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_diretorio);

					else if (getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int()) == -1) {

						setElemento(AttVetorModeloOtimizacao_custo_total, a_idEstagio, 0.0);

						if (getVarDecisao_ZIseExistir(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())) > -1)
							setElemento(AttVetorModeloOtimizacao_custo_imediato, a_idEstagio, 0.0);

						if (getVarDecisao_ZFseExistir(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())) > -1)
							setElemento(AttVetorModeloOtimizacao_custo_futuro, a_idEstagio, 0.0);

						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->resetar();

						return 0.0;

					} // else if (getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int()) == 0) {

				} // if (erro_str.find("10005") != std::string::npos) {					
			} // if (erro_str.size() > 5) {

		} // catch (const std::exception & erro) { 

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade)
			return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_diretorio);

		//
		// Tratamento Numérico de Inviabilidade
		//

		//otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, 1, a_idEstagio);
		otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, a_idEstagio);

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade)
			return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_diretorio);

		const std::string codigo = getString(int(time(0)));
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		throw std::invalid_argument("Erro em otimizar, codigo identificador do problema linear " + codigo);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::otimizarProblema(" + getFullString(a_TSS) + "," + getFullString(a_idIteracao) + "," + getString(a_idEstagio) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::otimizarSubproblema(const IdEstagio a_idEstagio, const IdCenario a_idCenario){


bool ModeloOtimizacao::otimizarProblema(const TipoSubproblemaSolver a_TSS, const IdProcesso a_idProcesso, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, EstruturaResultados<double>& a_sol_inf_var_dinamica, EstruturaResultados<double>& a_solucao_dual_var_dinamica, EstruturaResultados<double>& a_limite_inferior_var_dinamica, EstruturaResultados<double>& a_limite_superior_var_dinamica, EstruturaResultados<double>& a_sol_dual_var_estado, const std::string a_diretorio) {

	try {

		bool solucao_proxy = false;

		if (getElementoMatriz(AttMatrizModeloOtimizacao_realizacao_solucao_proxy, a_idEstagio, a_idRealizacao, int()) == 1)
			solucao_proxy = true;

		setElemento(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, 2);

		if ((getElementoVetor(AttVetorModeloOtimizacao_alguma_variavel_aleatoria_hidrologica_com_truncamento, a_idEstagio, int()) == 1) || (getAtributo(AttComumModeloOtimizacao_relaxar_afluencia_incremental_com_viabilidade_hidraulica, bool())))
			atualizarModeloOtimizacaoComVariavelRealizacaoInterna(a_TSS, a_idIteracao, a_idProcesso, a_idEstagio, a_idCenario, a_idRealizacao);

		if (!solucao_proxy)
			return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao, solucao_proxy, a_sol_inf_var_dinamica, a_solucao_dual_var_dinamica, a_limite_inferior_var_dinamica, a_limite_superior_var_dinamica, a_sol_dual_var_estado, a_diretorio);

		try {
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->otimizar();
		}
		catch (const std::exception& erro) {
			const std::string erro_str = std::string(erro.what());
			if (erro_str.size() > 5) {
				if (erro_str.find("10005") != std::string::npos) {
					escreverLog10005(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao, erro_str);
					return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao, false, a_sol_inf_var_dinamica, a_solucao_dual_var_dinamica, a_limite_inferior_var_dinamica, a_limite_superior_var_dinamica, a_sol_dual_var_estado, a_diretorio);
				} // if (erro_str.find("10005") != std::string::npos) {
			} // if (erro_str.size() > 5) {
		} // catch (const std::exception & erro) {

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() != TipoStatusSolver_nao_otimalidade)
			return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao, solucao_proxy, a_sol_inf_var_dinamica, a_solucao_dual_var_dinamica, a_limite_inferior_var_dinamica, a_limite_superior_var_dinamica, a_sol_dual_var_estado, a_diretorio);

		//
		// Tratamento Numérico de Inviabilidade
		//

		//otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, 1, a_idEstagio);
		otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, a_idEstagio);

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() != TipoStatusSolver_nao_otimalidade)
			return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao, solucao_proxy, a_sol_inf_var_dinamica, a_solucao_dual_var_dinamica, a_limite_inferior_var_dinamica, a_limite_superior_var_dinamica, a_sol_dual_var_estado, a_diretorio);

		const std::string codigo = getString(int(time(0)));
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		throw std::invalid_argument("Erro em otimizar, codigo identificador do problema linear " + codigo);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::otimizarProblemaLinear(" + getFullString(a_TSS) + "," + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "," + getFullString(a_idRealizacao) + "): \n" + std::string(erro.what())); }


} // double ModeloOtimizacao::otimizarProblema(const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao){

void ModeloOtimizacao::escreverLog10005(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const std::string a_string){

	try{

		const std::string solverStatus = "SolverStatus.txt";

		std::ofstream escritaStream;

		if ((a_idEstagio == IdEstagio_Nenhum) && (a_idCenario == IdCenario_Nenhum) && (a_idRealizacao == IdRealizacao_Nenhum)) {
			escritaStream.open(solverStatus, std::ios_base::out);
			if (!escritaStream.is_open())
				throw std::invalid_argument("Nao foi possivel abrir o arquivo " + solverStatus + ".");
			escritaStream << "";
		}
		else {

			escritaStream.open(solverStatus, std::ios_base::app);
			if (!escritaStream.is_open())
				throw std::invalid_argument("Nao foi possivel abrir o arquivo " + solverStatus + ".");
			escritaStream << getString(a_TSS) << ";" << getString(a_idIteracao) << ";" << getString(a_idEstagio) << ";" << getString(a_idCenario) << ";" << getString(a_idRealizacao) << ";" << a_string << std::endl;

			contadorLog10005++;

			if (contadorLog10005 <= 1) {
				const std::string codigo = getString(int(time(0)));
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
			} // if (contadorLog10005 <= 20) {

		}
		escritaStream.close();
		escritaStream.clear();


	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::escreverLog10005(" + getFullString(a_TSS) + "," + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "," + getFullString(a_idRealizacao) + "): \n" + std::string(erro.what())); }

}


double ModeloOtimizacao::posOtimizacaoProblema(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const std::string a_diretorio) {

	try {

		IdCenario a_idCenario = IdCenario_1;

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

			try {

				armazenarValoresSolver(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, IdRealizacao_Nenhum);

				if (getAtributo(AttComumModeloOtimizacao_imprimir_solver, bool())) {
					criarDiretorio(a_diretorio + "//" + getString(a_TSS));
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP(a_diretorio + "//" + getString(a_TSS) + "//PL_" + getString(a_idEstagio) + "_" + getString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())));
				}

				const double valorObjetivo = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorObjetivo();

				setElemento(AttVetorModeloOtimizacao_custo_total, a_idEstagio, valorObjetivo);

				if (getVarDecisao_ZIseExistir(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())) > -1)
					setElemento(AttVetorModeloOtimizacao_custo_imediato, a_idEstagio, vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorPrimal(getVarDecisao_ZI(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo()))));

				if (getVarDecisao_ZFseExistir(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())) > -1)
					setElemento(AttVetorModeloOtimizacao_custo_futuro, a_idEstagio, vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorPrimal(getVarDecisao_ZF(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo()))));

				return getCustoTotal(a_idEstagio);

			} // try
			catch (const std::exception & erro) {

				const std::string erro_str = std::string(erro.what());
				if (erro_str.size() > 5) {
					if (erro_str.find("10005") != std::string::npos) {
						escreverLog10005(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, IdRealizacao_Nenhum, erro_str);
					} // if (erro_str.find("10005") != std::string::npos) {
				} // if (erro_str.size() > 5) {

				const std::string codigo = getString(int(time(0)));
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
				throw std::invalid_argument("Codigo identificador do problema linear " + getString(codigo) + " \n" + std::string(erro.what()));

			} // catch (const std::exception & erro) {

		} // if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao()  == TipoStatusSolver_otimalidade) {

		else if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_tempo_excedido) {

			setElemento(AttVetorModeloOtimizacao_custo_total, a_idEstagio, 0.0);

			setElemento(AttVetorModeloOtimizacao_custo_imediato, a_idEstagio, 0.0);

			setElemento(AttVetorModeloOtimizacao_custo_futuro, a_idEstagio, 0.0);

			return NAN;

		}

		else
			return NAN;

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::otimizarProblema(" + getFullString(a_TSS) + "," + getFullString(a_idIteracao) + "," + getString(a_idEstagio) + "): \n" + std::string(erro.what())); }


} // double ModeloOtimizacao::posOtimizacaoProblema(const IdEstagio a_idEstagio, const std::string a_diretorio){

double ModeloOtimizacao::posOtimizacaoProblema(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const std::string a_diretorio) {

	try {

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

			try {

				armazenarValoresSolver(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, IdRealizacao_Nenhum);

				if (getAtributo(AttComumModeloOtimizacao_imprimir_solver, bool())) {
					criarDiretorio(a_diretorio + "//" + getString(a_TSS));
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP(a_diretorio + "//" + getString(a_TSS) + "//PL_" + getString(a_idEstagio) + "_" + getString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())));
				}

				const double valorObjetivo = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorObjetivo();

				setElemento(AttVetorModeloOtimizacao_custo_total, a_idEstagio, valorObjetivo);

				if (getVarDecisao_ZIseExistir(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())) > -1)
					setElemento(AttVetorModeloOtimizacao_custo_imediato, a_idEstagio, vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorPrimal(getVarDecisao_ZI(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo()))));

				if (getVarDecisao_ZFseExistir(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())) > -1) {
					double zf = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorPrimal(getVarDecisao_ZF(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())));
					if ((zf > -0.1) && (zf < 0.0))
						zf = 0.0;
					setElemento(AttVetorModeloOtimizacao_custo_futuro, a_idEstagio, zf);
				}

				return getCustoTotal(a_idEstagio);

			} // try
			catch (const std::exception & erro) {

				const std::string erro_str = std::string(erro.what());

				std::cout << getFullString(a_idEstagio) << " " << getFullString(a_idCenario) << " " << erro_str << std::endl;

				if (erro_str.size() > 5) {
					if (erro_str.find("10005") != std::string::npos) {

						escreverLog10005(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, IdRealizacao_Nenhum, erro_str);

						//otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, 1, a_idEstagio);
						otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, a_idEstagio);

						if (getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int()) >= 0)
							return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_diretorio);

						else if (getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int()) == -1) {

							setElemento(AttVetorModeloOtimizacao_custo_total, a_idEstagio, 0.0);

							if (getVarDecisao_ZIseExistir(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())) > -1)
								setElemento(AttVetorModeloOtimizacao_custo_imediato, a_idEstagio, 0.0);

							if (getVarDecisao_ZFseExistir(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())) > -1)
								setElemento(AttVetorModeloOtimizacao_custo_futuro, a_idEstagio, 0.0);

							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->resetar();

							return 0.0;

						} // else if (getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int()) == 0) {

					} // if (erro_str.find("10005") != std::string::npos) {					
				} // if (erro_str.size() > 5) {

				const std::string codigo = getString(int(time(0)));
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
				throw std::invalid_argument("Codigo identificador do problema linear " + getString(codigo) + " \n" + std::string(erro.what()));

			} // catch (const std::exception & erro) { 

		} // if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

		else
			throw std::invalid_argument("O status " + getFullString(vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao()) + " nao retorna solucao do problema.");

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::posOtimizacaoProblema(" + getFullString(a_TSS) + "," + getFullString(a_idIteracao) + "," + getString(a_idEstagio) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::posOtimizacaoProblema(const IdEstagio a_idEstagio, const IdCenario a_idCenario, const std::string a_diretorio) {


bool ModeloOtimizacao::posOtimizacaoProblema(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, bool a_solucao_proxy, EstruturaResultados<double>& a_sol_inf_var_dinamica, EstruturaResultados<double>& a_solucao_dual_var_dinamica, EstruturaResultados<double>& a_limite_inferior_var_dinamica, EstruturaResultados<double>& a_limite_superior_var_dinamica, EstruturaResultados<double>& a_sol_dual_var_estado, const std::string a_diretorio) {

	try {

		if ((vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) && (a_solucao_proxy)) {

			try {

				armazenarValoresSolver(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao);

				if (getAtributo(AttComumModeloOtimizacao_imprimir_solver, bool())) {
					criarDiretorio(a_diretorio + "//" + getString(a_TSS));
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP(a_diretorio + "//" + getString(a_TSS) + "//PL_" + getString(a_idEstagio) + "_" + getString(a_idCenario) + "_" + getString(a_idRealizacao));
				}

				const double valorObjetivo = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorObjetivo();
				//const double valorObjetivo = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getDualObjValue();

				const double valorSolucaoInferior = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getSolucaoInferiorVarDinamica();

				const double valorSolucaoInferiorVarDinamica = valorObjetivo - valorSolucaoInferior;

				a_sol_inf_var_dinamica.vetor.at(a_sol_inf_var_dinamica.conta) = valorSolucaoInferiorVarDinamica;

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getReducedCostVarDinamica(a_solucao_dual_var_dinamica.conta, &a_solucao_dual_var_dinamica.vetor[0]);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getLimiteInferiorVarDinamica(a_limite_inferior_var_dinamica.conta, &a_limite_inferior_var_dinamica.vetor[0]);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getLimiteSuperiorVarDinamica(a_limite_superior_var_dinamica.conta, &a_limite_superior_var_dinamica.vetor[0]);

				getSolucaoDualVariavelEstado(a_idEstagio, a_sol_dual_var_estado.conta, &a_sol_dual_var_estado.vetor[0]);

				setElemento(AttVetorModeloOtimizacao_custo_total, a_idEstagio, valorObjetivo);

				if (getVarDecisao_ZIseExistir(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())) > -1)
					setElemento(AttVetorModeloOtimizacao_custo_imediato, a_idEstagio, vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorPrimal(getVarDecisao_ZI(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo()))));

				if (getVarDecisao_ZFseExistir(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())) > -1) {
					double zf = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorPrimal(getVarDecisao_ZF(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())));
					if ((zf > -0.1) && (zf < 0.0))
						zf = 0.0;
					setElemento(AttVetorModeloOtimizacao_custo_futuro, a_idEstagio, zf);
				}

				a_sol_inf_var_dinamica.conta++;
				a_solucao_dual_var_dinamica.conta += vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getNumeroVarDinamica();
				a_limite_inferior_var_dinamica.conta += vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getNumeroVarDinamica();
				a_limite_superior_var_dinamica.conta += vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getNumeroVarDinamica();

				a_sol_dual_var_estado.conta += int(getMaiorId(a_idEstagio, IdVariavelEstado()));

				return true;

			} // try
			catch (const std::exception& erro) {

				const std::string erro_str = std::string(erro.what());

				if (erro_str.size() > 5) {
					if (erro_str.find("10005") != std::string::npos) {
						escreverLog10005(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao, erro_str);
						return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao, false, a_sol_inf_var_dinamica, a_solucao_dual_var_dinamica, a_limite_inferior_var_dinamica, a_limite_superior_var_dinamica, a_sol_dual_var_estado, a_diretorio);
					}
				} // if (erro_str.size() > 5) {

				const std::string codigo = getString(int(time(0)));
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + codigo);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + codigo);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + codigo);
				throw std::invalid_argument("Codigo identificador do problema linear " + getString(codigo) + " \n" + std::string(erro.what()));

			} // catch (const std::exception & erro) {

		} // if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

		else if ((vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_tempo_excedido) || (!a_solucao_proxy)) {

			a_sol_inf_var_dinamica.add(0.0);

			for (int i = 0; i < vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getNumeroVarDinamica(); i++)
				a_solucao_dual_var_dinamica.add(0.0);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getLimiteInferiorVarDinamica(a_limite_inferior_var_dinamica.conta, &a_limite_inferior_var_dinamica.vetor[0]);
			a_limite_inferior_var_dinamica.conta += vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getNumeroVarDinamica();

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getLimiteSuperiorVarDinamica(a_limite_superior_var_dinamica.conta, &a_limite_superior_var_dinamica.vetor[0]);
			a_limite_superior_var_dinamica.conta += vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getNumeroVarDinamica();

			for (int i = 0; i < int(getMaiorId(a_idEstagio, IdVariavelEstado())); i++)
				a_sol_dual_var_estado.add(0.0);

			setElemento(AttVetorModeloOtimizacao_custo_total, a_idEstagio, 0.0);

			setElemento(AttVetorModeloOtimizacao_custo_imediato, a_idEstagio, 0.0);

			setElemento(AttVetorModeloOtimizacao_custo_futuro, a_idEstagio, 0.0);

			return false;

		}

		else {
			const std::string codigo = getString(int(time(0)));
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + codigo);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + codigo);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + codigo);
			throw std::invalid_argument("O status " + getFullString(vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao()) + " nao retorna solucao do problema. Codigo identificador do problema linear " + getString(codigo));
		}

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::posOtimizacaoProblema(" + getFullString(a_TSS) + "," + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "," + getFullString(a_idRealizacao) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::posOtimizacaoProblema(const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const std::string a_diretorio){

void ModeloOtimizacao::armazenarValoresSolver(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao){

	try{

		if (a_idRealizacao == IdRealizacao_Nenhum) {

			if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, bool())) {

				int i = 1;

				VARIAVEL_DECISAO_2(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					VARIAVEL_DECISAO_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					VARIAVEL_DECISAO_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					VARIAVEL_DECISAO_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					VARIAVEL_DECISAO_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)

			}

			if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario, bool())) {

				EQUACAO_LINEAR_2(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					EQUACAO_LINEAR_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					EQUACAO_LINEAR_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					EQUACAO_LINEAR_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					EQUACAO_LINEAR_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)

					INEQUACAO_LINEAR_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					INEQUACAO_LINEAR_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					INEQUACAO_LINEAR_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					INEQUACAO_LINEAR_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)
					INEQUACAO_LINEAR_7(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO)

			} // if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, bool())) {

		}
		else {

			if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario_por_realizacao, bool())) {

				VARIAVEL_DECISAO_2(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					VARIAVEL_DECISAO_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					VARIAVEL_DECISAO_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					VARIAVEL_DECISAO_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					VARIAVEL_DECISAO_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)

			}

			if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario_por_realizacao, bool())) {

				EQUACAO_LINEAR_2(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					EQUACAO_LINEAR_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					EQUACAO_LINEAR_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					EQUACAO_LINEAR_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					EQUACAO_LINEAR_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)

					INEQUACAO_LINEAR_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					INEQUACAO_LINEAR_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					INEQUACAO_LINEAR_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					INEQUACAO_LINEAR_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)
					INEQUACAO_LINEAR_7(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO)

			}

		}


	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::armazenarValoresSolver(" + getFullString(a_TSS) + "," + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "," + getFullString(a_idRealizacao) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::armazenarValoresSolver(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao){



void ModeloOtimizacao::calcularCustoPrimalViaSubproblemaMestre(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const std::string a_diretorio, double& a_custo_geral, double& a_custo_individual) {

	try {

		if (vetorEstagio.att(a_idEstagio).isSolverInstanciado(a_TSS)) {

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->otimizar();

			if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

				if (getAtributo(AttComumModeloOtimizacao_imprimir_resultado_mestre, bool()))
					armazenarValoresSolver(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, IdRealizacao_Nenhum);

				if ((getAtributo(AttComumModeloOtimizacao_imprimir_solver, bool())) && (getAtributo(AttComumModeloOtimizacao_imprimir_solver_mestre, bool()))) {
					criarDiretorio(a_diretorio + "//" + getString(a_TSS));
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP(a_diretorio + "//" + getString(a_TSS) + "//PL_" + getString(a_idEstagio) + "_" + getString(a_idCenario));
				} // if ((getAtributo(AttComumModeloOtimizacao_imprimir_solver, bool())) && (getAtributo(AttComumModeloOtimizacao_imprimir_solver_mestre, bool()))) {

				IdRealizacao idRealizacao = IdRealizacao_1;

				const Periodo periodo_processo_estocastico = getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, a_idEstagio, Periodo());

				const IdProcessoEstocastico idProcessoEstocastico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

				if (getSize1Matriz(idProcessoEstocastico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral) > 0) {
					if (periodo_processo_estocastico >= getIterador2Inicial(idProcessoEstocastico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, Periodo()))
						idRealizacao = getElementoMatriz(idProcessoEstocastico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, periodo_processo_estocastico, IdRealizacao());
				}

				const int varZT_r = getVarDecisao_ZT(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo()), idRealizacao);

				a_custo_individual = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorPrimal(varZT_r);

				const int varZT = getVarDecisao_ZT(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo()));

				a_custo_geral = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorPrimal(varZT);

				return;

			} // if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

			const std::string codigo = getString(int(time(0)));
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + codigo);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + codigo);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + codigo);
			throw std::invalid_argument("Erro em otimizar, codigo identificador do problema linear " + codigo);

		} // if (vetorEstagio.att(a_idEstagio).isSolverInstanciado(a_TSS)) {

		return;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::calcularCustoPrimalViaSubproblemaMestre(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::correcaoViabilidadeHidraulica(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio){



void ModeloOtimizacao::getSolucaoDualVariavelEstado(const IdEstagio a_idEstagio, int& a_idx_sol_dual_var_estado, double* a_sol_dual_var_estado){

	try { 

		const TipoSubproblemaSolver tSS = TipoSubproblemaSolver_geral;

		const IdVariavelEstado maiorIdVariavelEstado = getMaiorId(a_idEstagio, IdVariavelEstado());

		if (maiorIdVariavelEstado == IdVariavelEstado_Nenhum)
			return;

		int idx = a_idx_sol_dual_var_estado;
		for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

			const int idVariavelDecisao = getElementoVetor(a_idEstagio, idVariavelEstado, AttVetorVariavelEstado_idVariavelDecisao, tSS, int());

			const double solucao_dual = vetorEstagio.att(a_idEstagio).getSolver(tSS)->getReducedCost(idVariavelDecisao);

			a_sol_dual_var_estado[idx] = solucao_dual;
			idx++;

		} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getSolucaoDualVariavelEstado(" + getString(a_idEstagio) + "): \n" + std::string(erro.what())); }

} // SmartEnupla<IdVariavelEstado, double> ModeloOtimizacao::getSolucaoDualVariavelEstado(const IdEstagio a_idEstagio){

double ModeloOtimizacao::getCustoTotal(const IdEstagio a_idEstagio){

	try{ return getElementoVetor(AttVetorModeloOtimizacao_custo_total, a_idEstagio, double()); } // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getCustoTotal(" + getString(a_idEstagio) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::getCustoTotal(const IdEstagio a_idEstagio){


double ModeloOtimizacao::getCustoImediato(const IdEstagio a_idEstagio){

	try { return getElementoVetor(AttVetorModeloOtimizacao_custo_imediato, a_idEstagio, double()); } // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getCustoImediato(" + getString(a_idEstagio) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::getCustoPresente(const IdEstagio a_idEstagio){


double ModeloOtimizacao::getCustoFuturo(const IdEstagio a_idEstagio){

	try { return getElementoVetor(AttVetorModeloOtimizacao_custo_futuro, a_idEstagio, double()); } // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getCustoFuturo(" + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::getCustoFuturo(const IdEstagio a_idEstagio){



double ModeloOtimizacao::getProbabilidadeAbertura(const IdEstagio a_idEstagio, const IdCenario a_idCenario) {

	try {

		double probabilidade_abertura = 1.0;

		const IdRealizacao maiorIdRealizacao = getAtributo(a_idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao());

		if (maiorIdRealizacao > IdRealizacao_1) {

			const Periodo periodo = getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

			const IdProcessoEstocastico idProcessoEstocastico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

			const IdRealizacao idRealizacao = getElementoMatriz(idProcessoEstocastico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, periodo, IdRealizacao());

			probabilidade_abertura = getElementoMatriz(idProcessoEstocastico, AttMatrizProcessoEstocastico_probabilidade_realizacao, periodo, idRealizacao, double());

		}// if (maiorIdRealizacao > IdRealizacao_1) {

		return probabilidade_abertura;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getProbabilidadeAbertura(" + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::getProbabilidadeAbertura(const IdEstagio a_idEstagio, const IdCenario a_idCenario) {

double ModeloOtimizacao::getProbabilidadeAbertura(const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao) {

	try {

		double probabilidade_abertura = 1.0;

		const IdRealizacao maiorIdRealizacao = getAtributo(a_idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao());

		if (maiorIdRealizacao > IdRealizacao_1) {

			const Periodo periodo = getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

			probabilidade_abertura = getElementoMatriz(getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico()), AttMatrizProcessoEstocastico_probabilidade_realizacao, periodo, a_idRealizacao, double());

		}// if (maiorIdRealizacao > IdRealizacao_1) {

		return probabilidade_abertura;

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getProbabilidadeAbertura(" + getFullString(a_idEstagio) + "," + getFullString(a_idRealizacao) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::getProbabilidadeAbertura(const IdEstagio a_idEstagio, const IdCenario a_idRealizacao) {


void ModeloOtimizacao::consolidarResultados(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados){
	try{

		consolidarVariaveis(a_idProcesso, a_maiorIdProcesso, a_entradaSaidaDados);
		consolidarEquacoes(a_idProcesso, a_maiorIdProcesso, a_entradaSaidaDados);
		consolidarInequacoes(a_idProcesso, a_maiorIdProcesso, a_entradaSaidaDados);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::consolidarVariaveis(" + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }
} // void ModeloOtimizacao::consolidarResultados(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados){

VARIAVEL_DECISAO_2(DECLARAR_CONSOLIDAR_RESULTADOS)
VARIAVEL_DECISAO_3(DECLARAR_CONSOLIDAR_RESULTADOS)
VARIAVEL_DECISAO_4(DECLARAR_CONSOLIDAR_RESULTADOS)
VARIAVEL_DECISAO_5(DECLARAR_CONSOLIDAR_RESULTADOS)
VARIAVEL_DECISAO_6(DECLARAR_CONSOLIDAR_RESULTADOS)

void ModeloOtimizacao::consolidarVariaveis(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados){

	try{

		const string diretorio = a_entradaSaidaDados.getDiretorioSaida();

		for (TipoSubproblemaSolver a_TSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); a_TSS < TipoSubproblemaSolver_Excedente; a_TSS++) {

			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//" + getString(a_TSS));

			int numero_variaveis_consolidar = int(lista_VarDecisao_instanciadas.at(a_TSS).size()) / int(a_maiorIdProcesso);

			const int resto_variaveis_consolidar = int(lista_VarDecisao_instanciadas.at(a_TSS).size()) % int(a_maiorIdProcesso);

			int pos_inicial = -1;
			int pos_final = -1;

			pos_inicial = getRank(a_idProcesso) * numero_variaveis_consolidar;

			if (a_idProcesso < a_maiorIdProcesso) {

				if (numero_variaveis_consolidar == 0.0)
					return;

				pos_final = getRank(IdProcesso(a_idProcesso + 1)) * numero_variaveis_consolidar;

			} // if (a_idProcesso < a_maiorIdProcesso) {

			else
				pos_final = int(lista_VarDecisao_instanciadas.at(a_TSS).size());

			for (int i = pos_inicial; i < pos_final; i++) {

				const std::string VarDecisao_str = lista_VarDecisao_instanciadas.at(a_TSS).at(i);

				VARIAVEL_DECISAO_2(CONSOLIDAR_RESULTADOS)
					VARIAVEL_DECISAO_3(CONSOLIDAR_RESULTADOS)
					VARIAVEL_DECISAO_4(CONSOLIDAR_RESULTADOS)
					VARIAVEL_DECISAO_5(CONSOLIDAR_RESULTADOS)
					VARIAVEL_DECISAO_6(CONSOLIDAR_RESULTADOS)

			} // for (int i = pos_inicial; i < pos_final; i++) {

		} // for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++){

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::consolidarVariaveis(" + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::consolidarVariaveis(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados){

EQUACAO_LINEAR_2(DECLARAR_CONSOLIDAR_RESULTADOS)
EQUACAO_LINEAR_3(DECLARAR_CONSOLIDAR_RESULTADOS)
EQUACAO_LINEAR_4(DECLARAR_CONSOLIDAR_RESULTADOS)
EQUACAO_LINEAR_5(DECLARAR_CONSOLIDAR_RESULTADOS)
EQUACAO_LINEAR_6(DECLARAR_CONSOLIDAR_RESULTADOS)

void ModeloOtimizacao::consolidarEquacoes(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const string diretorio = a_entradaSaidaDados.getDiretorioSaida();

		for (TipoSubproblemaSolver a_TSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); a_TSS < TipoSubproblemaSolver_Excedente; a_TSS++) {

			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//" + getString(a_TSS));

			int numero_equacoes_consolidar = int(lista_EquLinear_instanciadas.at(a_TSS).size()) / int(a_maiorIdProcesso);

			const int resto_equacoes_consolidar = int(lista_EquLinear_instanciadas.at(a_TSS).size()) % int(a_maiorIdProcesso);

			int pos_inicial = -1;
			int pos_final = -1;

			pos_inicial = getRank(a_idProcesso) * numero_equacoes_consolidar;

			if (a_idProcesso < a_maiorIdProcesso) {

				if (numero_equacoes_consolidar == 0.0)
					return;

				pos_final = getRank(IdProcesso(a_idProcesso + 1)) * numero_equacoes_consolidar;

			} // if (a_idProcesso < a_maiorIdProcesso) {

			else
				pos_final = int(lista_EquLinear_instanciadas.at(a_TSS).size());

			for (int i = pos_inicial; i < pos_final; i++) {

				const std::string EquLinear_str = lista_EquLinear_instanciadas.at(a_TSS).at(i);

				    EQUACAO_LINEAR_2(CONSOLIDAR_RESULTADOS)
					EQUACAO_LINEAR_3(CONSOLIDAR_RESULTADOS)
					EQUACAO_LINEAR_4(CONSOLIDAR_RESULTADOS)
					EQUACAO_LINEAR_5(CONSOLIDAR_RESULTADOS)
					EQUACAO_LINEAR_6(CONSOLIDAR_RESULTADOS)

			} // for (int i = pos_inicial; i < pos_final; i++) {

		} // for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++){

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::consolidarEquacoes(" + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::consolidarEquacoes(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados){


INEQUACAO_LINEAR_3(DECLARAR_CONSOLIDAR_RESULTADOS)
INEQUACAO_LINEAR_4(DECLARAR_CONSOLIDAR_RESULTADOS)
INEQUACAO_LINEAR_5(DECLARAR_CONSOLIDAR_RESULTADOS)
INEQUACAO_LINEAR_6(DECLARAR_CONSOLIDAR_RESULTADOS)
INEQUACAO_LINEAR_7(DECLARAR_CONSOLIDAR_RESULTADOS)

void ModeloOtimizacao::consolidarInequacoes(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const string diretorio = a_entradaSaidaDados.getDiretorioSaida();

		for (TipoSubproblemaSolver a_TSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); a_TSS < TipoSubproblemaSolver_Excedente; a_TSS++) {

			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//" + getString(a_TSS));

			int numero_inequacoes_consolidar = int(lista_IneLinear_instanciadas.at(a_TSS).size()) / int(a_maiorIdProcesso);

			const int resto_inequacoes_consolidar = int(lista_IneLinear_instanciadas.at(a_TSS).size()) % int(a_maiorIdProcesso);

			int pos_inicial = -1;
			int pos_final = -1;

			pos_inicial = getRank(a_idProcesso) * numero_inequacoes_consolidar;

			if (a_idProcesso < a_maiorIdProcesso) {

				if (numero_inequacoes_consolidar == 0.0)
					return;

				pos_final = getRank(IdProcesso(a_idProcesso + 1)) * numero_inequacoes_consolidar;

			} // if (a_idProcesso < a_maiorIdProcesso) {

			else
				pos_final = int(lista_IneLinear_instanciadas.at(a_TSS).size());

			for (int i = pos_inicial; i < pos_final; i++) {

				const std::string IneLinear_str = lista_IneLinear_instanciadas.at(a_TSS).at(i);

					INEQUACAO_LINEAR_3(CONSOLIDAR_RESULTADOS)
					INEQUACAO_LINEAR_4(CONSOLIDAR_RESULTADOS)
					INEQUACAO_LINEAR_5(CONSOLIDAR_RESULTADOS)
					INEQUACAO_LINEAR_6(CONSOLIDAR_RESULTADOS)
					INEQUACAO_LINEAR_7(CONSOLIDAR_RESULTADOS)

			} // for (int i = pos_inicial; i < pos_final; i++) {

		} // for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++){

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::consolidarInequacoes(" + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::consolidarIninequacoes(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados){

int ModeloOtimizacao::getNumeroVariavelDinamica(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio){

	try {

		return vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getNumeroVarDinamica();

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getNumeroVariavelDinamica(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }

} // int ModeloOtimizacao::getNumeroVariavelDinamica(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio){



void ModeloOtimizacao::criarModeloOtimizacao(Dados &a_dados, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const IdModeloOtimizacao idModeloOtimizacao = getAtributo(AttComumModeloOtimizacao_idModeloOtimizacao, IdModeloOtimizacao());

		if ((idModeloOtimizacao == IdModeloOtimizacao_multiestagio_estocastico_otimizacao) || (idModeloOtimizacao == IdModeloOtimizacao_multiestagio_estocastico_simulacao)) {

			const IdEstagio estagio_inicial = a_dados.getAtributo(AttComumDados_estagio_inicial, IdEstagio());

			setAtributo(AttComumModeloOtimizacao_estagio_inicial, estagio_inicial);

			const IdEstagio estagio_final = a_dados.getAtributo(AttComumDados_estagio_final, IdEstagio());

			setAtributo(AttComumModeloOtimizacao_estagio_final, estagio_final);

			setAtributo(AttComumModeloOtimizacao_periodo_otimizacao_inicial, a_dados.getElementoVetor(AttVetorDados_horizonte_otimizacao, estagio_inicial, Periodo()));
			setAtributo(AttComumModeloOtimizacao_periodo_otimizacao_final, a_dados.getElementoVetor(AttVetorDados_horizonte_otimizacao, estagio_final, Periodo()));

			setAtributo(AttComumModeloOtimizacao_periodo_estudo_inicial, a_dados.getElementosMatriz(AttMatrizDados_percentual_duracao_horizonte_estudo, estagio_inicial, Periodo(), double()).getIteradorInicial());
			setAtributo(AttComumModeloOtimizacao_periodo_estudo_final, a_dados.getElementosMatriz(AttMatrizDados_percentual_duracao_horizonte_estudo, estagio_final, Periodo(), double()).getIteradorFinal());

			setAtributo(AttComumModeloOtimizacao_estagio_acoplamento_pre_estudo, a_dados.getAtributo(AttComumDados_estagio_acoplamento_pre_estudo, IdEstagio()));
			setAtributo(AttComumModeloOtimizacao_maior_estagio_impressao_versao_alternativa_cortes, a_dados.getAtributo(AttComumDados_maior_estagio_impressao_versao_alternativa_cortes, IdEstagio()));

			setAtributo(AttComumModeloOtimizacao_foco_numerico, a_dados.getAtributo(AttComumDados_foco_numerico, int()));

			setAtributo(AttComumModeloOtimizacao_multiplicador_tempo_limite_resolucao, a_dados.getAtributo(AttComumDados_multiplicador_tempo_limite_resolucao, double()));

			setAtributo(AttComumModeloOtimizacao_tolerancia_otimalidade, a_dados.getAtributo(AttComumDados_tolerancia_otimalidade, double()));
			setAtributo(AttComumModeloOtimizacao_tolerancia_viabilidade, a_dados.getAtributo(AttComumDados_tolerancia_viabilidade, double()));

			setAtributo(AttComumModeloOtimizacao_numero_maximo_tentativas_resolucao, a_dados.getAtributo(AttComumDados_numero_maximo_tentativas_resolucao, int()));

			setAtributo(AttComumModeloOtimizacao_remover_cortes_dominados, a_dados.getAtributo(AttComumDados_remover_cortes_dominados, bool()));

			setAtributo(AttComumModeloOtimizacao_maior_corte_importado,                    a_dados.getAtributo(AttComumDados_maior_corte_importado, IdCorteBenders()));
			setAtributo(AttComumModeloOtimizacao_maior_corte_importado_pos_estudo, a_dados.getAtributo(AttComumDados_maior_corte_importado_pos_estudo, IdCorteBenders()));

			setAtributo(AttComumModeloOtimizacao_tipo_selecao_solucao_proxy, a_dados.getAtributo(AttComumDados_tipo_selecao_solucao_proxy, TipoSelecaoSolucaoProxy()));
			setAtributo(AttComumModeloOtimizacao_solucao_proxy_nula, a_dados.getAtributo(AttComumDados_solucao_proxy_nula, bool()));

			setAtributo(AttComumModeloOtimizacao_imprimir_solver, a_dados.getAtributo(AttComumDados_imprimir_solver, bool()));
			setAtributo(AttComumModeloOtimizacao_imprimir_solver_viabilidade_hidraulica, a_dados.getAtributo(AttComumDados_imprimir_solver_viabilidade_hidraulica, bool()));
			setAtributo(AttComumModeloOtimizacao_imprimir_solver_mestre, a_dados.getAtributo(AttComumDados_imprimir_solver_mestre, bool()));

			setAtributo(AttComumModeloOtimizacao_imprimir_tempos_metodo_solucao, a_dados.getAtributo(AttComumDados_imprimir_tempos_metodo_solucao, bool()));

			setAtributo(AttComumModeloOtimizacao_diretorio_importacao_cortes,              a_dados.getAtributo(AttComumDados_diretorio_importacao_cortes, std::string()));

			setAtributo(AttComumModeloOtimizacao_diretorio_importacao_pre_estudo, a_dados.getAtributo(AttComumDados_diretorio_importacao_pre_estudo, std::string()));
			setAtributo(AttComumModeloOtimizacao_diretorio_importacao_pos_estudo, a_dados.getAtributo(AttComumDados_diretorio_importacao_pos_estudo, std::string()));

			setAtributo(AttComumModeloOtimizacao_calcular_custo_primal_via_subproblema_mestre, a_dados.getAtributo(AttComumDados_calcular_custo_primal_via_subproblema_mestre, bool()));

			setAtributo(AttComumModeloOtimizacao_calcular_cenario_hidrologico_pre_otimizacao, a_dados.getAtributo(AttComumDados_calcular_cenario_hidrologico_pre_otimizacao, bool()));

			setAtributo(AttComumModeloOtimizacao_imprimir_cenario_hidrologico_pre_otimizacao, a_dados.getAtributo(AttComumDados_imprimir_cenario_hidrologico_pre_otimizacao, bool()));

			setAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, a_dados.getAtributo(AttComumDados_imprimir_resultado_por_estagio_por_cenario, bool()));
			setAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario_por_realizacao, a_dados.getAtributo(AttComumDados_imprimir_resultado_por_estagio_por_cenario_por_realizacao, bool()));

			setAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario, a_dados.getAtributo(AttComumDados_imprimir_resultado_por_estagio_por_cenario, bool()));
			setAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario_por_realizacao, a_dados.getAtributo(AttComumDados_imprimir_resultado_por_estagio_por_cenario_por_realizacao, bool()));
			
			setAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, a_dados.getAtributo(AttComumDados_imprimir_resultado_viabilidade_hidraulica, bool()));
			setAtributo(AttComumModeloOtimizacao_imprimir_resultado_mestre, a_dados.getAtributo(AttComumDados_imprimir_resultado_mestre, bool()));

			setAtributo(AttComumModeloOtimizacao_relaxar_afluencia_incremental_com_viabilidade_hidraulica, a_dados.getAtributo(AttComumDados_relaxar_afluencia_incremental_com_viabilidade_hidraulica, bool()));

			setAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, a_dados.getAtributo(AttComumDados_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico()));

			setAtributo(AttComumModeloOtimizacao_imprimir_estado_por_iteracao, a_dados.getAtributo(AttComumDados_imprimir_estado_por_iteracao, bool()));
			setAtributo(AttComumModeloOtimizacao_imprimir_corte_por_iteracao, a_dados.getAtributo(AttComumDados_imprimir_corte_por_iteracao, bool()));
			setAtributo(AttComumModeloOtimizacao_imprimir_selecao_corte_por_iteracao, a_dados.getAtributo(AttComumDados_imprimir_selecao_corte_por_iteracao, bool()));

			if (idModeloOtimizacao == IdModeloOtimizacao_multiestagio_estocastico_otimizacao) {
				setAtributo(AttComumModeloOtimizacao_selecao_cortes_nivel_dominancia, a_dados.getAtributo(AttComumDados_selecao_cortes_nivel_dominancia, int()));
			} // if (idModeloOtimizacao == IdModeloOtimizacao_multiestagio_estocastico_otimizacao) {

			else if (idModeloOtimizacao == IdModeloOtimizacao_multiestagio_estocastico_simulacao) { 
				setAtributo(AttComumModeloOtimizacao_selecao_cortes_nivel_dominancia, 0);
			} // else if (idModeloOtimizacao == IdModeloOtimizacao_multiestagio_estocastico_simulacao) {

			arranjoResolucao = a_dados.arranjoResolucao;

			instanciarProcessoEstocastico(a_dados, a_entradaSaidaDados);

			formularModeloOtimizacao(a_dados, a_entradaSaidaDados);

		} // if ((idModeloOtimizacao == IdModeloOtimizacao_multiestagio_estocastico_otimizacao) || (idModeloOtimizacao == IdModeloOtimizacao_multiestagio_estocastico_simulacao)) {

		else
			throw std::invalid_argument("ModeloOtimizacao invalido");

	} // try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarModeloOtimizacao(): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::criarModeloOtimizacao(){


void ModeloOtimizacao::instanciarProcessoEstocastico(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados) {

	try{

		a_entradaSaidaDados.setDiretorioEntrada(a_entradaSaidaDados.getDiretorioEntrada() + "//ProcessoEstocasticoHidrologico");

		const IdProcessoEstocastico tipo_processo_estocastico_hidrologico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

		ProcessoEstocastico processo_estocastico_hidrologico;
		processo_estocastico_hidrologico.setAtributo(AttComumProcessoEstocastico_idProcessoEstocastico, tipo_processo_estocastico_hidrologico);

		vetorProcessoEstocastico.add(processo_estocastico_hidrologico);

		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("PROCESSO_ESTOCASTICO_AttComumOperacional.csv", vetorProcessoEstocastico.att(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_direto);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("PROCESSO_ESTOCASTICO_" + getString(AttMatrizProcessoEstocastico_probabilidade_realizacao) + ".csv", vetorProcessoEstocastico.att(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_direto);
		
		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("VARIAVEL_ALEATORIA_AttComumOperacional.csv", vetorProcessoEstocastico.att(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("VARIAVEL_ALEATORIA_" + getString(AttVetorVariavelAleatoria_tipo_relaxacao) + ".csv", vetorProcessoEstocastico.att(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("VARIAVEL_ALEATORIA_" + getString(AttMatrizVariavelAleatoria_residuo_espaco_amostral) + ".csv",            vetorProcessoEstocastico.att(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_membro);
		
		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("VARIAVEL_ALEATORIA_" + getString(AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao) + ".csv", vetorProcessoEstocastico.att(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("VARIAVEL_ALEATORIA_INTERNA_AttComumOperacional.csv", vetorProcessoEstocastico.att(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_membroMembro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("VARIAVEL_ALEATORIA_INTERNA_" + getString(AttVetorVariavelAleatoriaInterna_coeficiente_participacao) + ".csv", vetorProcessoEstocastico.att(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_membroMembro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("VARIAVEL_ALEATORIA_INTERNA_" + getString(AttVetorVariavelAleatoriaInterna_tendencia_temporal) + ".csv", vetorProcessoEstocastico.att(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_membroMembro);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("PROCESSO_ESTOCASTICO_" + getString(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral) + ".csv", vetorProcessoEstocastico.att(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_direto);

		
	} // try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::instanciarProcessoEstocastico(a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::instanciarProcessoEstocastico(EntradaSaidaDados a_entradaSaidaDados) {