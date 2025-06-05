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
		   
		lista_elemento_impressao = std::vector<std::vector<std::string>>(TipoSubproblemaSolver_Excedente, std::vector<std::string>());

		contadorLog10005 = 0;
		escreverLog10005(TipoSubproblemaSolver_Nenhum, IdIteracao_Nenhum, IdEstagio_Nenhum, IdCenario_Nenhum, IdRealizacao_Nenhum, std::string());

		INICIALIZA_SMART_ELEMENTO(ModeloOtimizacao, SMART_ELEMENTO_MODELO_OTIMIZACAO)

		setAtributo(AttComumModeloOtimizacao_idModeloOtimizacao, a_idModeloOtimizacao);

		criarModeloOtimizacao(a_dados, a_entradaSaidaDados);

		///////////////////////////////
		//Se existirem idVariavelEstado associados às ENAs de acoplamento
		//Carrega os AttComumHIDRELETRICA e HIDRELETRICA_REE_AttVetorPremissa_produtibilidade_ENA

		Dados dados_temp;
		if (anyVarEstadoENA()) {

			const IdProcessoEstocastico maiorIdProcessoEstocastico = vetorProcessoEstocastico.getMaiorId();
			const IdProcessoEstocastico menorIdProcessoEstocastico = vetorProcessoEstocastico.getMenorId();

			if (menorIdProcessoEstocastico != maiorIdProcessoEstocastico)
				throw std::invalid_argument("Deve ser instanciado somente um processo estocastico");

			std::string diretorio_att_premissas = "";
			diretorio_att_premissas = a_entradaSaidaDados.getDiretorioEntrada();

			a_dados.validaProdutibilidadeENA(a_entradaSaidaDados, diretorio_att_premissas, vetorProcessoEstocastico.at(maiorIdProcessoEstocastico));

			dados_temp.vetorHidreletrica = a_dados.vetorHidreletrica;
		}//if (anyVarEstadoENA()) {

		///////////////////////////////

		a_dados.esvaziar();

		a_dados.vetorHidreletrica = dados_temp.vetorHidreletrica;

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

void ModeloOtimizacao::anularCortesExternos(const IdEstagio a_idEstagio){

	try {

		if (!vetorEstagio.at(a_idEstagio).anyCorteExterno)
			return;

		const IdEstagio idEstagio_anterior = IdEstagio(a_idEstagio - 1);

		const std::vector<TipoSubproblemaSolver> lista_subproblemaSolver = vetorEstagio.at(idEstagio_anterior).getListaTipoSubproblemaSolver();

		for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= getMaiorId(a_idEstagio, IdCorteBenders()); idCorteBenders++) {

			if (vetorEstagio.at(a_idEstagio).vetorCorteBenders.isInstanciado(idCorteBenders)) {

				if (getAtributo(a_idEstagio, idCorteBenders, AttComumCorteBenders_is_externo, bool())) {

					const IdRealizacao idRealEnd = getIteradorFinal(a_idEstagio, idCorteBenders, AttVetorCorteBenders_rhs, IdRealizacao());

					for (IdRealizacao idReal = IdRealizacao_1; idReal <= idRealEnd; idReal++) {
						const double infinito = vetorEstagio.at(idEstagio_anterior).getSolver(TipoSubproblemaSolver_geral)->getInfinito();
						const int ineCB = getIneLinear_CB_ZFseExistir(TipoSubproblemaSolver_geral, idEstagio_anterior, idReal, idCorteBenders);
						if (ineCB > -1)
							vetorEstagio.at(idEstagio_anterior).getSolver(TipoSubproblemaSolver_geral)->setRHSRestricao(ineCB, -infinito);
					}

				} // if (getAtributo(a_idEstagio, idCorteBenders, AttComumCorteBenders_is_externo, bool())) {

			} // if (vetorEstagio.at(a_idEstagio).vetorCorteBenders.isInstanciado(idCorteBenders)) {
		} // for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= getMaiorId(a_idEstagio, IdCorteBenders()); idCorteBenders++) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::anularCortesExternos(" + getFullString(a_idEstagio) + ") : \n" + std::string(erro.what())); }

}

void ModeloOtimizacao::ativarCortesExternos(const IdEstagio a_idEstagio) {

	try {

		const IdEstagio idEstagio_posterior = IdEstagio(a_idEstagio + 1);

		if (!vetorEstagio.isInstanciado(idEstagio_posterior))
			return;

		if (!vetorEstagio.at(idEstagio_posterior).anyCorteExterno)
			return;

		for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= getMaiorId(idEstagio_posterior, IdCorteBenders()); idCorteBenders++) {

			if (vetorEstagio.at(idEstagio_posterior).vetorCorteBenders.isInstanciado(idCorteBenders)) {

				if (getAtributo(idEstagio_posterior, idCorteBenders, AttComumCorteBenders_is_externo, bool())) {

					const IdRealizacao idRealEnd = getIteradorFinal(idEstagio_posterior, idCorteBenders, AttVetorCorteBenders_rhs, IdRealizacao());

					for (IdRealizacao idReal = IdRealizacao_1; idReal <= idRealEnd; idReal++) {
						const int ineCB = getIneLinear_CB_ZFseExistir(TipoSubproblemaSolver_geral, a_idEstagio, idReal, idCorteBenders);
						if (ineCB > -1)
							vetorEstagio.at(a_idEstagio).getSolver(TipoSubproblemaSolver_geral)->setRHSRestricao(ineCB, getElementoVetor(idEstagio_posterior, idCorteBenders, AttVetorCorteBenders_rhs, idReal, double()));
					}

				} // if (getAtributo(idEstagio_posterior, idCorteBenders, AttComumCorteBenders_is_externo, bool())) {

			} // if (vetorEstagio.at(idEstagio_posterior).vetorCorteBenders.isInstanciado(idCorteBenders)) {
		} // for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= getMaiorId(idEstagio_posterior, IdCorteBenders()); idCorteBenders++) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::ativarCortesExternos(" + getFullString(a_idEstagio) + ") : \n" + std::string(erro.what())); }

}

void ModeloOtimizacao::removerVarEstadoExterna(const IdEstagio a_idEstagio, const IdIteracao a_idIteracao){
	try{

		if ((!vetorEstagio.at(a_idEstagio).anyVarEstadoExterna) || (a_idIteracao < getAtributo(AttComumModeloOtimizacao_iteracao_maxima_cortes_externos, IdIteracao())))
			return;

		const IdVariavelEstado idVarEstadoEnd = vetorEstagio.at(a_idEstagio).vetorVariavelEstado.getMaiorId();
		IdVariavelEstado idVarEstadoEnd_new = IdVariavelEstado_Nenhum;
		for (IdVariavelEstado idVarEstado = idVarEstadoEnd; idVarEstado >= IdVariavelEstado_1; idVarEstado--) {

			if (vetorEstagio.at(a_idEstagio).vetorVariavelEstado.isInstanciado(idVarEstado)) {

				if (getAtributo(a_idEstagio, idVarEstado, AttComumVariavelEstado_is_externa, bool())) {

					if (idVarEstadoEnd_new != IdVariavelEstado_Nenhum)
						throw std::invalid_argument("Var Estado externas devem ser sequencialmente instanciadas ao final da lista, erro em " + getFullString(a_idEstagio) + "," + getFullString(idVarEstado) + ".");

					vetorEstagio.at(a_idEstagio).vetorVariavelEstado.rem(idVarEstado);

				}
				else {
					if (idVarEstadoEnd_new == IdVariavelEstado_Nenhum)
						idVarEstadoEnd_new = idVarEstado;				
				}
			}
		}

		if (idVarEstadoEnd_new < idVarEstadoEnd) {

			const IdCorteBenders idCorteIni = vetorEstagio.at(a_idEstagio).vetorCorteBenders.getMenorId();
			const IdCorteBenders idCorteEnd = vetorEstagio.at(a_idEstagio).vetorCorteBenders.getMaiorId();

			SmartEnupla<IdVariavelEstado, double> estado_ini(IdVariavelEstado_1, std::vector<double>(int(idVarEstadoEnd_new - IdVariavelEstado_1) + 1, 0.0));

			for (IdCorteBenders idCorte = idCorteIni; idCorte <= idCorteEnd; idCorte++) {

				if (vetorEstagio.at(a_idEstagio).vetorCorteBenders.isInstanciado(idCorte)) {

					SmartEnupla<IdVariavelEstado, double> estado;
					if (getSizeVetor(a_idEstagio, idCorte, AttVetorCorteBenders_estado) > 0)
						estado = estado_ini;
						
					const IdRealizacao idRealEnd = getIteradorFinal(a_idEstagio, idCorte, AttVetorCorteBenders_rhs, IdRealizacao());
					SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> coeficiente(IdRealizacao_1, std::vector<SmartEnupla<IdVariavelEstado, double>>(int(idRealEnd - IdRealizacao_1) + 1, estado_ini));

					for (IdVariavelEstado idVarEstado = IdVariavelEstado_1; idVarEstado <= idVarEstadoEnd_new; idVarEstado++) {
						if (estado.size() > 0)
							estado.at(idVarEstado) = getElementoVetor(a_idEstagio, idCorte, AttVetorCorteBenders_estado, idVarEstado, double());
						for (IdRealizacao idReal = IdRealizacao_1; idReal <= idRealEnd; idReal++)
							coeficiente.at(idReal).at(idVarEstado) = getElementoMatriz(a_idEstagio, idCorte, AttMatrizCorteBenders_coeficiente, idReal, idVarEstado, double());
					}

					if (estado.size() > 0)
						vetorEstagio.at(a_idEstagio).vetorCorteBenders.at(idCorte).setVetor_forced(AttVetorCorteBenders_estado, estado);
					
					vetorEstagio.at(a_idEstagio).vetorCorteBenders.at(idCorte).setMatriz_forced(AttMatrizCorteBenders_coeficiente, coeficiente);
					
				}
			}
		}

		vetorEstagio.at(a_idEstagio).anyVarEstadoExterna = false;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::removerVarEstadoExterna(" + getFullString(a_idEstagio) + ",): \n" + std::string(erro.what())); }

}

void ModeloOtimizacao::instanciarCorteBenders(const IdEstagio a_idEstagio, const SmartEnupla<IdRealizacao, double>& a_rhs, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>>& a_coeficiente, const SmartEnupla<IdVariavelEstado, double>& a_estado) {

	try {

		SmartEnupla<IdCorteBenders, IdCorteBenders> mapeamento_cortes_sequenciados = vetorEstagio.at(a_idEstagio).sequenciarCorteBenders();

		if (mapeamento_cortes_sequenciados.size() > 0) {

			const IdEstagio estagio_anterior = IdEstagio(a_idEstagio - 1);

			for (IdCorteBenders idCorte = IdCorteBenders_1; idCorte <= mapeamento_cortes_sequenciados.getIteradorFinal(); idCorte++) {

				for (IdRealizacao idReal = a_rhs.getIteradorInicial(); idReal <= a_rhs.getIteradorFinal(); idReal++) {

					const IdCorteBenders idCorte_old = mapeamento_cortes_sequenciados.at(idCorte);

					for (TipoSubproblemaSolver tss = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tss < TipoSubproblemaSolver_Excedente; tss++) {

						const int pos_zf = getPosIneLinear_CB_ZFseExistir(tss, estagio_anterior, idReal, idCorte_old);

						if (pos_zf > -1) {
							name_IneLinear_CB_ZF_3_1.at(tss).at(estagio_anterior).at(pos_zf) = getNomePrintIneLinear_CB_ZF(tss, estagio_anterior, idReal, idCorte);
							vetorEstagio.at(estagio_anterior).getSolver(tss)->setNomeRestricao(indx_IneLinear_CB_ZF_3_1.at(tss).at(estagio_anterior).at(pos_zf), getNomeSolverIneLinear_CB_ZF(tss, estagio_anterior, idReal, idCorte));
						}

						const int pos_zt = getPosIneLinear_CB_ZTseExistir(tss, estagio_anterior, idReal, idCorte_old);

						if (pos_zt > -1) {
							name_IneLinear_CB_ZT_3_1.at(tss).at(estagio_anterior).at(pos_zt) = getNomePrintIneLinear_CB_ZT(tss, estagio_anterior, idReal, idCorte);
							vetorEstagio.at(estagio_anterior).getSolver(tss)->setNomeRestricao(indx_IneLinear_CB_ZT_3_1.at(tss).at(estagio_anterior).at(pos_zt), getNomeSolverIneLinear_CB_ZT(tss, estagio_anterior, idReal, idCorte));
						}

					} // for (TipoSubproblemaSolver tss = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tss < TipoSubproblemaSolver_Excedente; tss++) {

				} // for (IdRealizacao idReal = a_rhs.getIteradorInicial(); idReal <= a_rhs.getIteradorFinal(); idReal++) {

			} // for (IdCorteBenders idCorte = IdCorteBenders_1; idCorte <= mapeamento_cortes_sequenciados.getIteradorFinal(); idCorte++) {

		} // if (mapeamento_cortes_sequenciados.size() > 0) {

		vetorEstagio.at(a_idEstagio).instanciarCorteBenders(a_rhs, a_coeficiente, a_estado);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::instanciaCorteBenders(" + getFullString(a_idEstagio) + ",a_rhs,a_coeficiente,a_estado): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::addCorteBenders(const IdEstagio a_idEstagio, const SmartEnupla<IdRealizacao, double> &a_rhs, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> &a_coeficiente, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> &a_estado){

void ModeloOtimizacao::instanciarCorteBenders(const IdEstagio a_idEstagio, const SmartEnupla<IdRealizacao, double>& a_rhs, const SmartEnupla<IdRealizacao, double>& a_rhs_primal, const SmartEnupla<IdRealizacao, double>& a_rhs_estado, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>>& a_coeficiente, const SmartEnupla<IdVariavelEstado, double>& a_estado) {

	try {

		vetorEstagio.at(a_idEstagio).instanciarCorteBenders(a_rhs, a_rhs_primal, a_rhs_estado, a_coeficiente, a_estado);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::instanciaCorteBenders(" + getFullString(a_idEstagio) + ",a_rhs,a_rhs_primal,a_rhs_estado,a_coeficiente,a_estado): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::addCorteBenders(const IdEstagio a_idEstagio, const SmartEnupla<IdRealizacao, double> &a_rhs, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> &a_coeficiente, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> &a_estado){


void ModeloOtimizacao::removerCortesBendersDominados(const IdEstagio a_idEstagio, const IdProcesso a_idProcesso, EntradaSaidaDados a_entradaSaidaDados){

	try{

		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//Cortes_dominados");

		bool primeiro_corte = false;
		for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= getMaiorId(a_idEstagio, IdCorteBenders()); idCorteBenders++) {

			if (vetorEstagio.at(a_idEstagio).vetorCorteBenders.isInstanciado(idCorteBenders)) {

				if (getAtributo(a_idEstagio, idCorteBenders, AttComumCorteBenders_is_corte_dominado, int()) == 1) {

					if ((a_idProcesso == IdProcesso_mestre) && (getAtributo(AttComumModeloOtimizacao_imprimir_selecao_corte_por_iteracao, bool()))) {

						a_entradaSaidaDados.setAppendArquivo(primeiro_corte);

						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_idEstagio) + "_corteBenders_rhs.csv", idCorteBenders, vetorEstagio.at(a_idEstagio), AttVetorCorteBenders_rhs);
						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_idEstagio) + "_corteBenders_rhs_primal.csv", idCorteBenders, vetorEstagio.at(a_idEstagio), AttVetorCorteBenders_rhs_primal);
						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_idEstagio) + "_corteBenders_rhs_estado.csv", idCorteBenders, vetorEstagio.at(a_idEstagio), AttVetorCorteBenders_rhs_estado);
						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_idEstagio) + "_corteBenders_estado.csv", idCorteBenders, vetorEstagio.at(a_idEstagio), AttVetorCorteBenders_estado);
						a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz(getFullString(a_idEstagio) + "_corteBenders_coeficientes.csv", idCorteBenders, vetorEstagio.at(a_idEstagio), AttMatrizCorteBenders_coeficiente);

					} // if ((a_idProcesso == IdProcesso_mestre) && (getAtributo(AttComumModeloOtimizacao_imprimir_selecao_corte_por_iteracao, bool()))) {

					vetorEstagio.at(a_idEstagio).vetorCorteBenders.rem(idCorteBenders);

					if (!primeiro_corte)
						primeiro_corte = true;

				} // if (getAtributo(a_idEstagio, idCorteBenders, AttComumCorteBenders_is_corte_dominado, int())) {

			} // if (vetorEstagio.at(a_idEstagio).vetorCorteBenders.isInstanciado(idCorteBenders)) {

		} // for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= getMaiorId(a_idEstagio, IdCorteBenders()); idCorteBenders++) {

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::removerCortesBendersDominados(" + getFullString(a_idEstagio) + "," + getFullString(a_idProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::removerCortesBendersDominados(const IdEstagio a_idEstagio, EntradaSaidaDados a_entradaSaidaDados){


void ModeloOtimizacao::removerCortesBendersExternos(const IdEstagio a_idEstagio, const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		if ((!vetorEstagio.at(a_idEstagio).anyCorteExterno) || (a_idIteracao < getAtributo(AttComumModeloOtimizacao_iteracao_maxima_cortes_externos, IdIteracao())))
			return;

		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//Cortes_externos");

		const IdEstagio idEstagio_anterior = IdEstagio(a_idEstagio - 1);

		const std::vector<TipoSubproblemaSolver> lista_subproblemaSolver = vetorEstagio.at(idEstagio_anterior).getListaTipoSubproblemaSolver();

		bool primeiro_corte = false;
		for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= getMaiorId(a_idEstagio, IdCorteBenders()); idCorteBenders++) {

			if (vetorEstagio.at(a_idEstagio).vetorCorteBenders.isInstanciado(idCorteBenders)) {

				if (getAtributo(a_idEstagio, idCorteBenders, AttComumCorteBenders_is_externo, bool())) {

					const IdRealizacao idRealEnd = getIteradorFinal(a_idEstagio, idCorteBenders, AttVetorCorteBenders_rhs, IdRealizacao());

					a_entradaSaidaDados.setAppendArquivo(primeiro_corte);

					a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_idEstagio) + "_corteBenders_rhs.csv", idCorteBenders, vetorEstagio.at(a_idEstagio), AttVetorCorteBenders_rhs);
					a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_idEstagio) + "_corteBenders_rhs_primal.csv", idCorteBenders, vetorEstagio.at(a_idEstagio), AttVetorCorteBenders_rhs_primal);
					a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_idEstagio) + "_corteBenders_rhs_estado.csv", idCorteBenders, vetorEstagio.at(a_idEstagio), AttVetorCorteBenders_rhs_estado);
					a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_idEstagio) + "_corteBenders_estado.csv", idCorteBenders, vetorEstagio.at(a_idEstagio), AttVetorCorteBenders_estado);
					a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz(getFullString(a_idEstagio) + "_corteBenders_coeficientes.csv", idCorteBenders, vetorEstagio.at(a_idEstagio), AttMatrizCorteBenders_coeficiente);

					vetorEstagio.at(a_idEstagio).vetorCorteBenders.rem(idCorteBenders);

					if (!primeiro_corte)
						primeiro_corte = true;

					for (IdRealizacao idReal = IdRealizacao_1; idReal <= idRealEnd; idReal++) {
						for (int j = 0; j < int(lista_subproblemaSolver.size()); j++)
							remCorteBendersFromZF(lista_subproblemaSolver.at(j), a_idEstagio, idEstagio_anterior, idReal, idCorteBenders);
					}

				} // if (getAtributo(a_idEstagio, idCorteBenders, AttComumCorteBenders_is_externo, bool())) {

			} // if (vetorEstagio.at(a_idEstagio).vetorCorteBenders.isInstanciado(idCorteBenders)) {
		} // for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= getMaiorId(a_idEstagio, IdCorteBenders()); idCorteBenders++) {

		vetorEstagio.at(a_idEstagio).anyCorteExterno = false;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::removerCortesBendersExternos(" + getFullString(a_idEstagio) + "," + getFullString(a_idIteracao) + "," + getFullString(a_idProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::removerCortesBendersDominados(const IdEstagio a_idEstagio, EntradaSaidaDados a_entradaSaidaDados){


void ModeloOtimizacao::requestCorteBenders(const IdProcesso a_idProcesso, const IdEstagio a_idEstagio, const std::string a_diretorio, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());

		const IdEstagio idEstagio_anterior = IdEstagio(a_idEstagio - 1);

		std::vector<TipoSubproblemaSolver> lista_subproblemaSolver; 

		if (a_idEstagio > getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio()))
			lista_subproblemaSolver = vetorEstagio.at(idEstagio_anterior).getListaTipoSubproblemaSolver();
		else
			lista_subproblemaSolver = vetorEstagio.at(a_idEstagio).getListaTipoSubproblemaSolver();

		if ((idEstagio_anterior < estagio_inicial) && (!vetorEstagio.at(a_idEstagio).isSolverInstanciado(TipoSubproblemaSolver_mestre)))
			return;

		if (!arranjoResolucao.isAnyCenarioEstado(idEstagio_anterior) && !arranjoResolucao.isAnyAberturas(idEstagio_anterior))
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

		if ((getAtributo(AttComumModeloOtimizacao_remover_cortes_dominados, bool())))
			removerCortesBendersDominados(a_idEstagio, a_idProcesso, a_entradaSaidaDados);

	} // try {
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::requestCorteBenders(" + getFullString(a_idProcesso) + "," + getFullString(a_idEstagio) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::requestCorteBenders(const IdProcesso a_idProcesso, const IdEstagio a_idEstagio, const std::string a_diretorio, EntradaSaidaDados a_entradaSaidaDados){


void ModeloOtimizacao::requestCorteBenders(const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders, double & a_rhs, SmartEnupla<IdVariavelEstado, double>& a_coeficiente){

	try { vetorEstagio.at(a_idEstagio).requestCorteBenders(a_idRealizacao, a_idCorteBenders, a_rhs, a_coeficiente); } // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::requestCorteBenders(" + getFullString(a_idEstagio) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCorteBenders) + ",a_rhs,a_coeficiente,a_estado): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::requestCorteBenders(const IdEstagio a_idEstagio, double & a_rhs, const SmartEnupla<IdVariavelEstado, double>& a_coeficiente){


void ModeloOtimizacao::requestCorteBendersFinalizado(const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao){

	try { vetorEstagio.at(a_idEstagio).requestCorteBendersFinalizado(a_idRealizacao); } // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::requestCorteBendersFinalizado(" + getFullString(a_idEstagio) + "," + getFullString(a_idRealizacao) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::requestCorteBendersFinalizado(const IdRealizacao a_idRealizacao){

void ModeloOtimizacao::selecionarCorteBenders(const IdEstagio a_idEstagio){
	try { vetorEstagio.at(a_idEstagio).selecionarCorteBenders(); } // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::selecionarCorteBenders(" + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
} // void ModeloOtimizacao::selecionarCorteBenders(const IdEstagio a_idEstagio){



void ModeloOtimizacao::gerarRealizacoes(const IdEstagio a_estagioIni, const IdEstagio a_estagioEnd, const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, EntradaSaidaDados a_entradaSaidaDados){

	try {

		const IdProcessoEstocastico idPE = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

		bool algum_truncamento = false;
		for (IdEstagio idEstagio = a_estagioIni; idEstagio <= a_estagioEnd; idEstagio++) {
			if (getElementoVetor(AttVetorModeloOtimizacao_alguma_variavel_aleatoria_hidrologica_com_truncamento, idEstagio, int()) == 1) {
				algum_truncamento = true;
				break;
			}
		}

		const bool viabilidade_hidraulica = getAtributo(AttComumModeloOtimizacao_relaxar_afluencia_incremental_com_viabilidade_hidraulica, bool());

		const bool imprimir_cenarios = getAtributo(AttComumModeloOtimizacao_imprimir_cenario_hidrologico_pre_otimizacao, bool());

		const bool is_variavelEstado_ENA = anyVarEstadoENA();//Se existem variáveis de ENA de acoplamento precisa para o computo da AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral

		if (imprimir_cenarios || algum_truncamento || viabilidade_hidraulica || is_variavelEstado_ENA) {

			const IdCenario menor_cenario_iteracao = arranjoResolucao.getAtributo(a_idIteracao, AttComumIteracao_menor_cenario, IdCenario());
			const IdCenario maior_cenario_iteracao = arranjoResolucao.getAtributo(a_idIteracao, AttComumIteracao_maior_cenario, IdCenario());

			if (arranjoResolucao.getMenorId(IdIteracao()) < a_idIteracao) {
				const IdIteracao idIteracao_past = IdIteracao(a_idIteracao - 1);
				const IdCenario menor_cenario_iteracao_past = arranjoResolucao.getAtributo(idIteracao_past, AttComumIteracao_menor_cenario, IdCenario());
				const IdCenario maior_cenario_iteracao_past = arranjoResolucao.getAtributo(idIteracao_past, AttComumIteracao_maior_cenario, IdCenario());
				if ((menor_cenario_iteracao == menor_cenario_iteracao_past) && (maior_cenario_iteracao == maior_cenario_iteracao_past))
					return;
			} // if (arranjoResolucao.getMenorId(IdIteracao()) < a_idIteracao) {

			IdCenario menor_cenario = menor_cenario_iteracao;
			IdCenario maior_cenario = maior_cenario_iteracao;

			int semente_geracao_cenario_hidrologico = -1;

			bool gerar_cenarios_internos = false;
			if ((imprimir_cenarios) && (a_idProcesso == IdProcesso_mestre)) {
				gerar_cenarios_internos = true;
				a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//ProcessoEstocasticoHidrologico");
			}

			vetorProcessoEstocastico.at(idPE).gerarCenariosPorSorteio(a_entradaSaidaDados, imprimir_cenarios, true, gerar_cenarios_internos, getAtributo(AttComumModeloOtimizacao_numero_cenarios, int()), menor_cenario, maior_cenario, TipoSorteio_uniforme, semente_geracao_cenario_hidrologico);

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

		for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

			const TipoSubproblemaSolver tSS_anterior = getAtributo(a_idEstagio, idVariavelEstado, AttComumVariavelEstado_tipoSubproblemaSolverEstagioAnterior, TipoSubproblemaSolver());

			const double valor = getElementoVetor(a_idEstagio, idVariavelEstado, AttVetorVariavelEstado_valor, a_idCenario, double());

			if (a_retorno_estados.total > 0)
				a_retorno_estados.add(valor);

			for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++) {

				if ((tSS == TipoSubproblemaSolver_mestre) && (vetorEstagio.at(a_idEstagio).isSolverInstanciado(tSS))) {
					if (getElementoVetor(a_idEstagio, idVariavelEstado, AttVetorVariavelEstado_idVariavelDecisao, tSS, int()) == -1)
						vetorEstagio.at(a_idEstagio).addVariavelEstado(tSS, getAtributo(a_idEstagio, idVariavelEstado, AttComumVariavelEstado_nome, string()), addVarDecisao_ESTADO(tSS, a_idEstagio, idVariavelEstado, 0.0, 0.0, 0.0), -1);
				} // if ((tSS == TipoSubproblemaSolver_mestre) && (vetorEstagio.at(a_idEstagio).isSolverInstanciado(tSS))) {

				const int idVariavelDecisao = getElementoVetor(a_idEstagio, idVariavelEstado, AttVetorVariavelEstado_idVariavelDecisao, tSS, int());

				if (idVariavelDecisao > -1) {

					vetorEstagio.at(a_idEstagio).getSolver(tSS)->setLimInferior(idVariavelDecisao, valor);
					vetorEstagio.at(a_idEstagio).getSolver(tSS)->setLimSuperior(idVariavelDecisao, valor);

				} // if (idVariavelDecisao > -1) {

			}  // for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++) {

		} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::atualizarModeloOtimizacaoComVariavelEstado(" + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }


} // void ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelEstado(const IdEstagio a_idEstagio, const IdProcesso a_idProcesso, const IdCenario a_idCenario, int& a_indice, double* a_array){


void ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelEstado_posEstudo(const IdEstagio a_idEstagio, const IdEstagio a_idEstagio_final, const IdCenario a_idCenario_estado, const IdCenario a_idCenario, IdRealizacao a_idRealizacao, Dados& a_dados) {

	try {

		const IdEstagio idEstagio_futuro = vetorEstagio.getMaiorId();

		if (a_idEstagio == a_idEstagio_final && idEstagio_futuro > a_idEstagio_final) {//Procura variáveis de ENA no idEstagio_futuro (se existir)

			if (a_idRealizacao == IdRealizacao_Nenhum) {
				const Periodo period = getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, a_idEstagio, Periodo());
				a_idRealizacao = getElementoMatriz(getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico()), AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, period, IdRealizacao());
			}

			const Periodo periodo_estudo_inicial = getAtributo(AttComumModeloOtimizacao_periodo_estudo_inicial, Periodo());
			const IdVariavelEstado maiorIdVariavelEstado = getMaiorId(idEstagio_futuro, IdVariavelEstado());

			for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

				if (vetorEstagio.at(idEstagio_futuro).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

					const std::vector<std::string> nome = vetorEstagio.at(idEstagio_futuro).getNomeVariavelEstado(idVariavelEstado);

					if (nome.at(0) == "ENA") {

						//******************************************************************************
						//Define parâmetros necessários para o cálculo das ENAs de acoplamento
						//******************************************************************************

						if (true) {

							const IdProcessoEstocastico idProcessoEstocastico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

							if (int(idHidreletricas_x_usina_calculo_ENA.size()) == 0)//Define para cada idHidreletrica todas as usinas que estão a montante na cascata (necessário para o cálculo da ENA)						
								defineHidreletricasMontanteNaCascataENA(a_dados);

							if (int(lista_codPosto_idHidreletrica.size()) == 0) {

								const IdHidreletrica menorIdHidreletrica = a_dados.getMenorId(IdHidreletrica());
								const IdHidreletrica maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

								lista_codPosto_idHidreletrica = SmartEnupla<int, IdHidreletrica>(1, std::vector<IdHidreletrica>(999, IdHidreletrica_Nenhum));
								mapIdVar = SmartEnupla<IdHidreletrica, IdVariavelAleatoria>(menorIdHidreletrica, std::vector<IdVariavelAleatoria>(int(maiorIdHidreletrica - menorIdHidreletrica) + 1, IdVariavelAleatoria_Nenhum));
								mapIdVarInterna = SmartEnupla<IdHidreletrica, IdVariavelAleatoriaInterna>(menorIdHidreletrica, std::vector<IdVariavelAleatoriaInterna>(int(maiorIdHidreletrica - menorIdHidreletrica) + 1, IdVariavelAleatoriaInterna_Nenhum));

								for (IdHidreletrica idHidreletrica = menorIdHidreletrica; idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {
									lista_codPosto_idHidreletrica.at(a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_codigo_posto, int())) = idHidreletrica;
									IdVariavelAleatoria        idVariavelAleatoria = IdVariavelAleatoria_Nenhum;
									IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_Nenhum;
									vetorProcessoEstocastico.at(idProcessoEstocastico).getIdVariavelAleatoriaIdVariavelAleatoriaInternaFromIdFisico(idVariavelAleatoria, idVariavelAleatoriaInterna, idHidreletrica);
									mapIdVar.at(idHidreletrica) = idVariavelAleatoria;
									mapIdVarInterna.at(idHidreletrica) = idVariavelAleatoriaInterna;
								}//for (IdHidreletrica idHidreletrica = menorIdHidreletrica; idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

							}//if (int(lista_codPosto_idHidreletrica.size()) == 0) {

						}//if (true) {

						//******************************************************************************

						const Periodo periodo_lag = Periodo(nome.at(2));
						const IdReservatorioEquivalente idREE = getIdReservatorioEquivalenteFromChar(nome.at(3).c_str());
						TipoSubproblemaSolver tSS = getAtributo(idEstagio_futuro, idVariavelEstado, AttComumVariavelEstado_tipoSubproblemaSolverEstagioAnterior, TipoSubproblemaSolver());
						const int idVariavelDecisao = getAtributo(idEstagio_futuro, idVariavelEstado, AttComumVariavelEstado_idVariavelDecisaoEstagioAnterior, int());

						if (idVariavelDecisao > -1) {

							double valor = vetorEstagio.at(a_idEstagio).getSolver(tSS)->getLimSuperior(idVariavelDecisao);

							if ((periodo_lag >= periodo_estudo_inicial) || (periodo_lag.sobreposicao(periodo_estudo_inicial) > 0.0) || (valor == 0.0))//Para calcular a ENA de períodos de tendência somente 1 vez
								valor = atualizar_ENA_acoplamento(a_dados, idREE, a_idCenario_estado, a_idRealizacao, periodo_lag);

							vetorEstagio.at(a_idEstagio).getSolver(tSS)->setLimInferior(idVariavelDecisao, valor);
							vetorEstagio.at(a_idEstagio).getSolver(tSS)->setLimSuperior(idVariavelDecisao, valor);

						} // if (idVariavelDecisao > -1) {

					}//if (nome.at(0) == "ENA") {

				}//if (vetorEstagio.at(a_idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

			}//for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

		}//if (a_idEstagio == a_estagio_final && maiorIdEstagio > a_estagio_final) {


	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::atualizarModeloOtimizacaoComVariavelEstado_posEstudo(" + getFullString(a_idEstagio) + "," + getFullString(a_idCenario_estado) + "): \n" + std::string(erro.what())); }


} // void ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelEstado(const IdEstagio a_idEstagio, const IdProcesso a_idProcesso, const IdCenario a_idCenario, int& a_indice, double* a_array){


void ModeloOtimizacao::defineHidreletricasMontanteNaCascataENA(Dados& a_dados) {

	try {

		const IdHidreletrica menorIdHidreletrica = a_dados.getMenorId(IdHidreletrica());
		const IdHidreletrica maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

		//Inicialização SmartEnupla
		idHidreletricas_x_usina_calculo_ENA = SmartEnupla<IdHidreletrica, SmartEnupla<int, IdHidreletrica>>(menorIdHidreletrica, std::vector<SmartEnupla<int, IdHidreletrica>>(int(maiorIdHidreletrica - menorIdHidreletrica) + 1, SmartEnupla<int, IdHidreletrica>()));

		for (IdHidreletrica idHidreletrica = menorIdHidreletrica; idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			/////////////////////////////////////////////////////////////
			//1. Inicializa vetor usinas_calculo_ENA com o próprio idHidreletrica
			/////////////////////////////////////////////////////////////
			//a_dados.vetorHidreletrica.at(idHidreletrica).addElemento(AttVetorHidreletrica_codigo_usinas_calculo_ENA, a_dados.vetorHidreletrica.at(idHidreletrica).getSizeVetor(AttVetorHidreletrica_codigo_usinas_calculo_ENA) + 1, a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(AttComumHidreletrica_codigo_usina, int()));
			idHidreletricas_x_usina_calculo_ENA.at(idHidreletrica).addElemento(int(idHidreletricas_x_usina_calculo_ENA.at(idHidreletrica).size()) + 1, idHidreletrica);

			/////////////////////////////////////////////////////////////
			//2. Completa vetor usinas_calculo_ENA com todas o idHidreletrica 
			// de TODAS as usinas a montante na cascata
			/////////////////////////////////////////////////////////////
			std::vector<IdHidreletrica> vetor_dinamico_controle_codigo_usinas_montante;

			//Inicializa vetor_dinamico_controle_codigo_usinas_montante com as usinas a montante de idHidreletrica

			for (IdHidreletrica idUHE_montante = menorIdHidreletrica; idUHE_montante <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idUHE_montante)) {

				const IdHidreletrica idUHE_jusante = a_dados.getAtributo(idUHE_montante, AttComumHidreletrica_jusante, IdHidreletrica());

				if (idUHE_jusante != IdHidreletrica_Nenhum) {

					if (a_dados.getAtributo(idUHE_jusante,AttComumHidreletrica_codigo_usina, int()) == a_dados.getAtributo(idHidreletrica,AttComumHidreletrica_codigo_usina, int()))
						vetor_dinamico_controle_codigo_usinas_montante.push_back(idUHE_montante);

				}//if (idUHE_jusante != IdHidreletrica_Nenhum) {

			} // for (IdHidreletrica idUHE_montante = menorIdHidreletrica; idUHE_montante <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idUHE_montante)) {

			////////
			//Dinamicamente procura todas as usinas a montante da cascata

			while (int(vetor_dinamico_controle_codigo_usinas_montante.size()) > 0) {

				//Premissa: vai alocando o idHidreletrica da primeira posição do vetor_dinamico_controle_usinas_montante
				//a_dados.vetorHidreletrica.at(idHidreletrica).addElemento(AttVetorHidreletrica_codigo_usinas_calculo_ENA, a_dados.vetorHidreletrica.at(idHidreletrica).getSizeVetor(AttVetorHidreletrica_codigo_usinas_calculo_ENA) + 1, vetor_dinamico_controle_codigo_usinas_montante.at(0));

				idHidreletricas_x_usina_calculo_ENA.at(idHidreletrica).addElemento(int(idHidreletricas_x_usina_calculo_ENA.at(idHidreletrica).size()) + 1, vetor_dinamico_controle_codigo_usinas_montante.at(0));

				//Atualiza vetor_dinamico_controle_usinas_montante com as usinas a montante de vetor_dinamico_controle_usinas_montante.at(0)				
				for (IdHidreletrica idUHE_montante = menorIdHidreletrica; idUHE_montante <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idUHE_montante)) {

					const IdHidreletrica idUHE_jusante = a_dados.getAtributo(idUHE_montante, AttComumHidreletrica_jusante, IdHidreletrica());

					if (idUHE_jusante != IdHidreletrica_Nenhum) {

						if (a_dados.getAtributo(idUHE_jusante,AttComumHidreletrica_codigo_usina, int()) == a_dados.getAtributo(vetor_dinamico_controle_codigo_usinas_montante.at(0), AttComumHidreletrica_codigo_usina, int()))
							vetor_dinamico_controle_codigo_usinas_montante.push_back(idUHE_montante);

					}//if (idUHE_jusante != IdHidreletrica_Nenhum) {

				} // for (IdHidreletrica idUHE_montante = menorIdHidreletrica; idUHE_montante <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idUHE_montante)) {

				//Apaga vetor_dinamico_controle_usinas_montante.at(0)
				vetor_dinamico_controle_codigo_usinas_montante.erase(vetor_dinamico_controle_codigo_usinas_montante.begin());

			}//while (int(vetor_dinamico_controle_codigo_usinas_montante.size()) > 0) {

		} // for (IdHidreletrica idHidreletrica = menorIdHidreletrica; idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::defineHidreletricasMontanteNaCascataENA: \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::defineHidreletricasMontanteNaCascataENA() {

double ModeloOtimizacao::atualizar_ENA_acoplamento(Dados& a_dados, const IdReservatorioEquivalente a_idReservatorioEquivalente, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const Periodo a_periodo_lag) {

	try {

		double valor_ENA = 0.0;

		const IdHidreletrica menorIdHidreletrica = a_dados.getMenorId(IdHidreletrica());
		const IdHidreletrica maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

		const IdProcessoEstocastico idProcessoEstocastico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

		const Periodo periodo_amostra_ini = vetorProcessoEstocastico.at(idProcessoEstocastico).getIterador2Inicial(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario_1, Periodo());

		for (IdHidreletrica idUHE = a_dados.getMenorId(IdHidreletrica()); idUHE <= a_dados.getMaiorId(IdHidreletrica()); a_dados.vetorHidreletrica.incr(idUHE)) {

			if (a_dados.vetorHidreletrica.at(idUHE).vetorReservatorioEquivalente.isInstanciado(a_idReservatorioEquivalente)) {

				const int codigo_posto = a_dados.getAtributo(idUHE, AttComumHidreletrica_codigo_posto, int());
				const int codigo_posto_acoplamento_ENA = a_dados.getAtributo(idUHE, AttComumHidreletrica_codigo_posto_acoplamento_ENA, int());

				//O vetor produtibilidade_ENA é formado pelo horizonte_tendencia + horizonte_processo_estocastico
				const SmartEnupla<Periodo, double> produtibilidade_ENA = a_dados.getVetor(idUHE, a_idReservatorioEquivalente, AttVetorReservatorioEquivalente_produtibilidade_ENA, Periodo(), double());

				const Periodo periodoPE_inicial = produtibilidade_ENA.getIteradorInicial();
				const Periodo periodoPE_final = produtibilidade_ENA.getIteradorFinal();

				bool is_sobreposicao_encontrada = false;
				bool is_precisa_calcular = false;
				double soma_sobreposicao = 0.0;

				for (Periodo periodoPE = periodoPE_inicial; periodoPE <= periodoPE_final; produtibilidade_ENA.incrementarIterador(periodoPE)) {

					if (produtibilidade_ENA.at(periodoPE) > 0.0) {//Evita cálcular afluências para cálculo de ENA desnecessárias

						is_precisa_calcular = true;

						const double sobreposicao = a_periodo_lag.sobreposicao(periodoPE);

						if (sobreposicao > 0.0) {

							soma_sobreposicao += sobreposicao;
							is_sobreposicao_encontrada = true;

							//****************************************************************************************************************************
							//Filosofia: Obter o equacionamento da ENA x usina (termo_independente_calculo_ENA + coeficiente_idHidreletricas_calculo_ENA) 
							// e logo pondera as afluências vezes a produtibilidade_ENA
							//****************************************************************************************************************************					
							std::vector<SmartEnupla<IdHidreletrica, double>> coeficiente_idHidreletricas_calculo_ENA;
							double afluencia_natural = 0.0;

							retorna_equacionamento_regras_afluencia_natural_x_idHidreletrica(a_dados, periodoPE, codigo_posto, codigo_posto_acoplamento_ENA, idUHE, a_idCenario, a_idRealizacao, idProcessoEstocastico, coeficiente_idHidreletricas_calculo_ENA, afluencia_natural);

							for (int pos = 0; pos < int(coeficiente_idHidreletricas_calculo_ENA.size()); pos++) {

								const IdHidreletrica idUHE_ENA = coeficiente_idHidreletricas_calculo_ENA.at(pos).getIteradorInicial();
								const double         coeficiente_idUHE_ENA = coeficiente_idHidreletricas_calculo_ENA.at(pos).at(idUHE_ENA);

								if (periodoPE < periodo_amostra_ini) {//Valores da tendência
									const double aflu_incr = vetorProcessoEstocastico.at(idProcessoEstocastico).getElementoVetor(mapIdVar.at(idUHE_ENA), mapIdVarInterna.at(idUHE_ENA), AttVetorVariavelAleatoriaInterna_tendencia_temporal, periodoPE, double());
									if (aflu_incr > 0.0)
										afluencia_natural += coeficiente_idUHE_ENA * aflu_incr;
								}
								else {//Valores dentro da árvore
									const double aflu_incr = getRealizacaoInterna(mapIdVar.at(idUHE_ENA), mapIdVarInterna.at(idUHE_ENA), a_idCenario, a_idRealizacao, periodoPE); //Depende se estiver na etapa do forward/backward
									if (aflu_incr > 0.0)
										afluencia_natural += coeficiente_idUHE_ENA * aflu_incr;
								}
							}//for (int pos = 0; pos < int(coeficiente_idHidreletricas_calculo_ENA.size()); pos++) {

							if (afluencia_natural < 0.0)
								afluencia_natural = 0.0;

							valor_ENA += sobreposicao * produtibilidade_ENA.at(periodoPE) * afluencia_natural;
							//****************************************************************************************************************************

						}//if (sobreposicao > 0.0) {

						if (is_sobreposicao_encontrada && sobreposicao == 0.0) {//Evita percorrer o horizonte todo					
							const double tol = 1e-6;
							if (abs(1 - soma_sobreposicao) > tol)
								throw std::invalid_argument("Soma de sobreposicao diferente de 1.0, valor encontrado: " + getString(soma_sobreposicao));

							break;
						}//if (is_sobreposicao_encontrada && sobreposicao == 0.0) {


					}//if (produtibilidade_ENA.at(periodoPE) > 0.0) {

				}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; produtibilidade_ENA.incrementarIterador(periodo)) {

				if (!is_sobreposicao_encontrada && is_precisa_calcular)
					throw std::invalid_argument("Nao encontrada sobreposicao do periodo_lag: " + getString(a_periodo_lag));

			} // if (a_dados.vetorHidreletrica.at(idUHE).vetorReservatorioEquivalente.isInstanciado(a_idReservatorioEquivalente)) {

		}//for (IdHidreletrica idUHE = a_dados.getMenorId(IdHidreletrica()); idUHE <= a_dados.getMaiorId(IdHidreletrica()); a_dados.vetorHidreletrica.incr(idUHE)) {

		return valor_ENA;


	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::atualizar_ENA_acoplamento(" + getFullString(a_idReservatorioEquivalente) + "," + getFullString(a_idCenario) + "," + getFullString(a_periodo_lag) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::atualizar_ENA_acoplamento(Dados& a_dados, const IdReservatorioEquivalente a_idReservatorioEquivalente, const IdCenario a_idCenario, const Periodo a_periodo_lag) {

void ModeloOtimizacao::retorna_equacionamento_regras_afluencia_natural_x_idHidreletrica(Dados& a_dados, const Periodo a_periodoPE, const int a_codigo_posto, const int a_codigo_posto_acoplamento_ENA, const IdHidreletrica a_idHidreletrica, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const IdProcessoEstocastico a_idProcessoEstocastico, std::vector<SmartEnupla<IdHidreletrica, double>>& a_coeficiente_idHidreletricas_calculo_ENA, double& a_termo_independente_calculo_ENA) {

	try {

		if (a_codigo_posto_acoplamento_ENA == -1)
			throw std::invalid_argument("Deve ser definido o codigo_posto_acoplamento_ENA para a idHidreletrica: " + getString(a_idHidreletrica));

		if (a_codigo_posto_acoplamento_ENA == 0) //Usinas que não entram no equcionamento da ENA de forma direta (as afluências destas usinas são utilizadas dentro de regras de outras usinas)
			return;

		//Retorna a composição da afluência natural (como é valorada no NW) da usina com a_idHidreletrica
		//Estrutura em afluência incremental: termo_independiente + coef_a * afluencia_usina_a + coef_b * afluencia_usina_b + ... + coef_n * afluencia_usina_n
		//Notas:
		//(i) Precisa de um termo independente porque existem regras operativas que dada uma condição o valor de afluência a valorar é uma constante (e.g. Belo Monte = 13900 se aflu_posto_288 >= Lim)
		//(ii) os cálculos dos postos para cálculo de ENA de acoplamento podem variar ligeramente com as premissas do arquivos regras.dat, p.ex, Itaipu, Simplicio, Ilha Pombos, Nilo Peçanha, P.Passos
		//     IMPORTANTE no REE-Sudeste: a vazão natural de Funil é contabilizada em Nilo Peçanha -> P.Passos e não em Simplicio -> Ilha Pombos (registro Jusena), i.e., muda regra.dat

		const IdHidreletrica menorIdHidreletrica = a_dados.getMenorId(IdHidreletrica());
		const IdHidreletrica maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

		if (a_codigo_posto == a_codigo_posto_acoplamento_ENA) {//Significa que é um posto natural sem regra específica (i.e. a natural valora no NW é composta por todas as incrementais das usinas a montante + a incremental da própria usina)

			const int iterador_inicial = idHidreletricas_x_usina_calculo_ENA.at(a_idHidreletrica).getIteradorInicial();
			const int iterador_final   = idHidreletricas_x_usina_calculo_ENA.at(a_idHidreletrica).getIteradorFinal();

			for (int pos = iterador_inicial; pos <= iterador_final; pos++) {
				a_coeficiente_idHidreletricas_calculo_ENA.push_back(SmartEnupla<IdHidreletrica, double>(idHidreletricas_x_usina_calculo_ENA.at(a_idHidreletrica).at(pos), std::vector<double>(1, 1.0)));
			}

		}//if (a_codigo_posto == a_codigo_posto_acoplamento_ENA) {
		else {//Posto com regra específica para acoplamento com o NW

			//const Periodo periodo_ultimo_sobreposicao = get_periodo_ultimo_sobreposicao_com_horizonte_DC(a_dados); //Necessário para as premissas da cascata do Rio São Francisco

			//Ver arquivo REGRAS.DAT do modelo GEVAZP
			//Posto 169 (Sobradinho natural) é uma propagação das vazões de Três Marias e Queimado + Incremental de Sobradinho
			//codigo_posto_acoplamento_ENA == 976 -> Regra para 176-COMP PAF-MOX

			if (a_codigo_posto_acoplamento_ENA == 37) {//Barra Bonita

				//Regra de acoplamento NW (REGRAS.DAT)
				//afluencia = afluencia_237 - 0.1 * (afluencia_161 - afluencia_117 - afluencia_301) - afluencia_117 - afluencia_301;
				//afluencia = afluencia_237 - 0.1 * afluencia_161 - 0.9 * afluencia_117 - 0.9 * afluencia_301;

				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 237, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 161, -0.1, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 117, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 301, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);

			}//else if (codigo_posto_acoplamento_NW == 37) {//Barra Bonita
			else if (a_codigo_posto_acoplamento_ENA == 38) {//BARIRI (A. S. LIMA)

				//Regra de acoplamento NW (REGRAS.DAT)
				//afluencia = afluencia_238 - 0.1 * (afluencia_161 - afluencia_117 - afluencia_301) - afluencia_117 - afluencia_301;
				//afluencia = afluencia_238 - 0.1 * afluencia_161 - 0.9 * afluencia_117 - 0.9 * afluencia_301;

				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 238, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 161, -0.1, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 117, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 301, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);


			}//else if (codigo_posto_acoplamento_NW == 38) {//BARIRI (A. S. LIMA)
			else if (a_codigo_posto_acoplamento_ENA == 39) {//IBITINGA

				//Regra de acoplamento NW (REGRAS.DAT)
				//afluencia = afluencia_239 - 0.1 * (afluencia_161 - afluencia_117 - afluencia_301) - afluencia_117 - afluencia_301;
				//afluencia = afluencia_239 - 0.1 * afluencia_161 - 0.9 * afluencia_117 - 0.9 * afluencia_301;

				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 239, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 161, -0.1, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 117, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 301, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);


			}//else if (codigo_posto_acoplamento_NW == 39) {//IBITINGA
			else if (a_codigo_posto_acoplamento_ENA == 40) {//PROMISSAO

				//Regra de acoplamento NW (REGRAS.DAT)
				//afluencia = afluencia_240 - 0.1 * (afluencia_161 - afluencia_117 - afluencia_301) - afluencia_117 - afluencia_301;
				//afluencia = afluencia_240 - 0.1 * afluencia_161 - 0.9 * afluencia_117 - 0.9 * afluencia_301;

				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 240, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 161, -0.1, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 117, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 301, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);


			}//else if (codigo_posto_acoplamento_NW == 40) {//PROMISSAO
			else if (a_codigo_posto_acoplamento_ENA == 42) {//NAVANHANDAVA

				//Regra de acoplamento NW (REGRAS.DAT)
				//afluencia = afluencia_242 - 0.1 * (afluencia_161 - afluencia_117 - afluencia_301) - afluencia_117 - afluencia_301;
				//afluencia = afluencia_242 - 0.1 * afluencia_161 - 0.9 * afluencia_117 - 0.9 * afluencia_301;

				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 242, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 161, -0.1, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 117, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 301, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);


			}//else if (codigo_posto_acoplamento_NW == 42) {//NAVANHANDAVA
			else if (a_codigo_posto_acoplamento_ENA == 43) {//Três Irmaos

				//Regra de acoplamento NW (REGRAS.DAT)
				//afluencia = afluencia_243 - 0.1 * (afluencia_161 - afluencia_117 - afluencia_301) - afluencia_117 - afluencia_301;
				//afluencia = afluencia_243 - 0.1 * afluencia_161 - 0.9 * afluencia_117 - 0.9 * afluencia_301;

				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 243, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 161, -0.1, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 117, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 301, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);

			}//if (codigo_posto_acoplamento_NW == 43) {//Três Irmaos	
			else if (a_codigo_posto_acoplamento_ENA == 45) {//JUPIA

				//Regra de acoplamento NW (REGRAS.DAT)
				//afluencia = afluencia_245 - 0.1 * (afluencia_161 - afluencia_117 - afluencia_301) - afluencia_117 - afluencia_301;
				//afluencia = afluencia_245 - 0.1 * afluencia_161 - 0.9 * afluencia_117 - 0.9 * afluencia_301;

				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 245, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 161, -0.1, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 117, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 301, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);

			}//else if (codigo_posto_acoplamento_NW == 45) {//JUPIA
			else if (a_codigo_posto_acoplamento_ENA == 46) {//P. PRIMAVERA

				//Regra de acoplamento NW (REGRAS.DAT)
				//afluencia = afluencia_246 - 0.1 * (afluencia_161 - afluencia_117 - afluencia_301) - afluencia_117 - afluencia_301;
				//afluencia = afluencia_246 - 0.1 * afluencia_161 - 0.9 * afluencia_117 - 0.9 * afluencia_301;

				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 246, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 161, -0.1, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 117, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 301, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);

			}//else if (codigo_posto_acoplamento_NW == 46) {//P. PRIMAVERA
			else if (a_codigo_posto_acoplamento_ENA == 66) {//ITAIPU

				//Regra de acoplamento NW (REGRAS.DAT)
				//afluencia = afluencia_266 - 0.1 * (afluencia_161 - afluencia_117 - afluencia_301) - afluencia_117 - afluencia_301;
				//afluencia = afluencia_266 - 0.1 * afluencia_161 - 0.9 * afluencia_117 - 0.9 * afluencia_301;

				//Regra de acoplamento NW : Atenção: diferente a Regras.dat
				//Aflu_natural_itaipu = Aflu_natural_itaipu - Aflu_natural_I_Solteira - Aflu_natural_Tres_Irmaos - Aflu_natural_Capivara
				//Equivalente a:
				//Aflu_incremental_itaipu = Aflu_incremental_itaipu + Aflu_incremental_jupia + Aflu_incremental_p_primavera + Aflu_incremental_domingos + Aflu_incremental_rosana + Aflu_incremental_taquaruçu

				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 266, 1.0, a_coeficiente_idHidreletricas_calculo_ENA); //Itaipu
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 34, -1.0, a_coeficiente_idHidreletricas_calculo_ENA); //I.Solteira
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 243, -1.0, a_coeficiente_idHidreletricas_calculo_ENA); //Tres Irmaos
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 61, -1.0, a_coeficiente_idHidreletricas_calculo_ENA); //Capivara


			}//else if (codigo_posto_acoplamento_NW == 66) {//ITAIPU
			else if (a_codigo_posto_acoplamento_ENA == 75) {//SEGREDO + DESVIO

				//Regra de acoplamento NW (REGRAS.DAT): VAZ(75) = VAZ(76)+MIN(VAZ(73)-10;173.5)

				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 76, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				const double afluencia_73 = get_afluencia_natural_posto(a_dados, a_periodoPE, 73, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);

				if ((afluencia_73 - 10) < 173.5) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 73, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 10;
				}//if ((afluencia_73 - 10) < 173.5) {
				else
					a_termo_independente_calculo_ENA += 173.5;

			}//else if (codigo_posto_acoplamento_NW == 75) {//SEGREDO + DESVIO
			else if (a_codigo_posto_acoplamento_ENA == 126) {//SIMPLICIO

				//Regra de acoplamento NW (REGRAS.DAT):
				// VAZ(126) = SE(VAZ(127)<=430;MAX(0;VAZ(127)-90);340)+VAZ(123)
				// VAZ(127) = VAZ(129)-vaz(298)-vaz(203)+vaz(304)
				// VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
				// VAZ(304) =vaz(315)-vaz(316)
				// VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				// VAZ(317) = MAX(0;VAZ(201)-25)
				// VAZ(316) = MIN(VAZ(315);190)

				/////////////////////////
				//Postos "reais"
				/////////////////////////
				const double afluencia_129 = get_afluencia_natural_posto(a_dados, a_periodoPE, 129, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_125 = get_afluencia_natural_posto(a_dados, a_periodoPE, 125, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_203 = get_afluencia_natural_posto(a_dados, a_periodoPE, 203, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_201 = get_afluencia_natural_posto(a_dados, a_periodoPE, 201, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_123 = get_afluencia_natural_posto(a_dados, a_periodoPE, 123, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);

				/////////////////////////
				//Postos "fictícios"
				/////////////////////////

				double afluencia_298 = -1.0;

				if (afluencia_125 <= 190)
					afluencia_298 = afluencia_125 * double(119.0 / 190.0);
				else if (afluencia_125 <= 209)
					afluencia_298 = 119;
				else if (afluencia_125 <= 250)
					afluencia_298 = afluencia_125 - 90;
				else
					afluencia_298 = 160;

				///////
				double afluencia_317 = 0;

				if ((afluencia_201 - 25) > afluencia_317)
					afluencia_317 = (afluencia_201 - 25);

				///////
				double afluencia_315 = afluencia_203 - afluencia_201 + afluencia_317 + afluencia_298;

				///////
				double afluencia_316 = 190;

				if (afluencia_315 < afluencia_316)
					afluencia_316 = afluencia_315;

				///////

				double afluencia_304 = afluencia_315 - afluencia_316;

				///////

				double afluencia_127 = afluencia_129 - afluencia_298 - afluencia_203 + afluencia_304;

				///////

				//**********************************************************************
				//VAZ(126) = SE(VAZ(127) <= 430; MAX(0; VAZ(127) - 90); 340) + VAZ(123)
				//**********************************************************************

				// ***Atenção: diferença com regra.dat
				// Substrai VAZ(123): Posto de Funil
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 123, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				//////////
				//VAZ(123)
				//////////
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 123, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				if (afluencia_127 <= 430) {

					if ((afluencia_127 - 90) > 0) {

						a_termo_independente_calculo_ENA -= 90;

						//   VAZ(127) = VAZ(129)-vaz(298)-vaz(203)+vaz(304)
						//   VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
						//   VAZ(304) = vaz(315)-vaz(316)
						//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
						//   VAZ(317) = MAX(0;VAZ(201)-25)
						//   VAZ(316) = MIN(VAZ(315);190)

						////////////
						// +VAZ(129)
						////////////
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 129, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						////////////
						// -VAZ(298)
						////////////

						if (afluencia_125 <= 190)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, -double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
						else if (afluencia_125 <= 209)
							a_termo_independente_calculo_ENA -= 119;
						else if (afluencia_125 <= 250) {
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
							a_termo_independente_calculo_ENA += 90;
						}//else if (afluencia_125 <= 250) {						
						else
							a_termo_independente_calculo_ENA -= 160;



						////////////
						// -VAZ(203)
						////////////
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 203, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						////////////
						// +VAZ(304)
						////////////

						//   VAZ(304) = vaz(315)-vaz(316)
						//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
						//   VAZ(317) = MAX(0;VAZ(201)-25)
						//   VAZ(316) = MIN(VAZ(315);190)

						//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
						// +VAZ(203)

						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 203, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						// -VAZ(201)
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						// +VAZ(317)
						if ((afluencia_201 - 25) > 0) {
							// +VAZ(201)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
							a_termo_independente_calculo_ENA -= 25;

						}//if ((afluencia_201 - 25) > 0) {

						// +VAZ(298)
						if (afluencia_125 <= 190)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
						else if (afluencia_125 <= 209)
							a_termo_independente_calculo_ENA += 119;
						else if (afluencia_125 <= 250) {
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
							a_termo_independente_calculo_ENA -= 90;
						}//else if (afluencia_125 <= 250) {						
						else
							a_termo_independente_calculo_ENA += 160;



						//////////
						//-VAZ(316)
						//VAZ(316) = MIN(VAZ(315); 190)
						//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
						if (afluencia_315 < 190) {

							// -VAZ(203)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 203, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

							// +VAZ(201)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

							// -VAZ(317)
							//   VAZ(317) = MAX(0;VAZ(201)-25)
							if ((afluencia_201 - 25) > 0) {
								// +VAZ(201)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
								a_termo_independente_calculo_ENA += 25;

							}//if ((afluencia_201 - 25) > 0) {

							// -VAZ(298)
							if (afluencia_125 <= 190)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, -double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
							else if (afluencia_125 <= 209)
								a_termo_independente_calculo_ENA -= 119;
							else if (afluencia_125 <= 250) {
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
								a_termo_independente_calculo_ENA += 90;
							}//else if (afluencia_125 <= 250) {						
							else
								a_termo_independente_calculo_ENA -= 160;


						}//if (afluencia_315 < 190) {							
						else
							a_termo_independente_calculo_ENA -= 190;

					}//if ((afluencia_127 - 90) > 0) {

				}//if (afluencia_127 <= 430) {							
				else
					a_termo_independente_calculo_ENA += 340;

			}//else if (codigo_posto_acoplamento_NW == 126) {//SIMPLICIO
			else if (a_codigo_posto_acoplamento_ENA == 131) {//NILO PECANHA

				//Regra de acoplamento NW (REGRAS.DAT): 
				// VAZ(131) = MIN(VAZ(316);144)-VAZ(123)
				// VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
				// VAZ(304) = vaz(315)-vaz(316)
				// VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				// VAZ(317) = MAX(0;VAZ(201)-25)
				// VAZ(316) = MIN(VAZ(315);190)

				/////////////////////////
				//Postos "reais"
				/////////////////////////

				const double afluencia_203 = get_afluencia_natural_posto(a_dados, a_periodoPE, 203, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_201 = get_afluencia_natural_posto(a_dados, a_periodoPE, 201, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_125 = get_afluencia_natural_posto(a_dados, a_periodoPE, 125, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_123 = get_afluencia_natural_posto(a_dados, a_periodoPE, 123, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);

				/////////////////////////
				//Postos "fictícios"
				/////////////////////////

				double afluencia_298 = -1;

				if (afluencia_125 <= 190)
					afluencia_298 = afluencia_125 * double(119.0 / 190.0);
				else if (afluencia_125 <= 209)
					afluencia_298 = 119;
				else if (afluencia_125 <= 250)
					afluencia_298 = afluencia_125 - 90;
				else
					afluencia_298 = 160;

				///////
				double afluencia_317 = 0;

				if ((afluencia_201 - 25) > afluencia_317)
					afluencia_317 = (afluencia_201 - 25);

				///////
				double afluencia_315 = afluencia_203 - afluencia_201 + afluencia_317 + afluencia_298;

				///////
				double afluencia_316 = 190;

				if (afluencia_315 < afluencia_316)
					afluencia_316 = afluencia_315;


				//******************************************
				//VAZ(131) = MIN(VAZ(316); 144) - VAZ(123)
				//******************************************

				// ***Atenção: diferença com regra.dat
				// Soma VAZ(123): Posto de Funil
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 123, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				/////////////
				//   -VAZ(123)
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 123, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				if (afluencia_316 < 144) {

					/////////////
					//   VAZ(316) = MIN(VAZ(315);190)

					if (afluencia_315 < 190) {

						//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
						// +VAZ(203)
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 203, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						// -VAZ(201)
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						// +VAZ(317) 
						//   VAZ(317) = MAX(0;VAZ(201)-25)
						if ((afluencia_201 - 25) > 0) {
							// +VAZ(201)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
							a_termo_independente_calculo_ENA -= 25;

						}//if ((afluencia_201 - 25) > 0) {

						// +VAZ(298)
						//   VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
						if (afluencia_125 <= 190)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
						else if (afluencia_125 <= 209)
							a_termo_independente_calculo_ENA += 119;
						else if (afluencia_125 <= 250) {
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
							a_termo_independente_calculo_ENA -= 90;
						}//else if (afluencia_125 <= 250) {						
						else
							a_termo_independente_calculo_ENA += 160;

					}
					else
						a_termo_independente_calculo_ENA += 190;

				}//if (afluencia_316 < 144) {
				else
					a_termo_independente_calculo_ENA += 144;



			}//else if (codigo_posto_acoplamento_NW == 131) {//NILO PECANHA
			else if (a_codigo_posto_acoplamento_ENA == 299) {//ILHA POMBOS

				//Regra de acoplamento NW (REGRAS.DAT): 
				// VAZ(299) = VAZ(130)-VAZ(298)-VAZ(203)+VAZ(304)+VAZ(123)
				// VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
				// VAZ(304) =vaz(315)-vaz(316)
				// VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				// VAZ(317) = MAX(0;VAZ(201)-25)
				// VAZ(316) = MIN(VAZ(315);190)


				/////////////////////////
				//Postos "reais"
				/////////////////////////

				const double afluencia_130 = get_afluencia_natural_posto(a_dados, a_periodoPE, 130, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_125 = get_afluencia_natural_posto(a_dados, a_periodoPE, 125, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_203 = get_afluencia_natural_posto(a_dados, a_periodoPE, 203, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_201 = get_afluencia_natural_posto(a_dados, a_periodoPE, 201, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_123 = get_afluencia_natural_posto(a_dados, a_periodoPE, 123, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);

				/////////////////////////
				//Postos "fictícios"
				/////////////////////////

				double afluencia_298 = -1;

				if (afluencia_125 <= 190)
					afluencia_298 = afluencia_125 * double(119.0 / 190.0);
				else if (afluencia_125 <= 209)
					afluencia_298 = 119;
				else if (afluencia_125 <= 250)
					afluencia_298 = afluencia_125 - 90;
				else
					afluencia_298 = 160;

				///////
				double afluencia_317 = 0;

				if ((afluencia_201 - 25) > afluencia_317)
					afluencia_317 = (afluencia_201 - 25);

				///////
				double afluencia_315 = afluencia_203 - afluencia_201 + afluencia_317 + afluencia_298;

				///////
				double afluencia_316 = 190;

				if (afluencia_315 < afluencia_316)
					afluencia_316 = afluencia_315;

				///////

				double afluencia_304 = afluencia_315 - afluencia_316;

				//*********************************************************
				// VAZ(299) = VAZ(130)-VAZ(298)-VAZ(203)+VAZ(304)+VAZ(123)
				//*********************************************************

				// ***Atenção: diferença com regra.dat
				// Substrai VAZ(123): Posto de Funil
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 123, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				/////////////
				// +VAZ(130)
				/////////////
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 130, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				/////////////
				// +VAZ(123)
				/////////////
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 123, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				/////////////
				// -VAZ(298)
				/////////////
				if (afluencia_125 <= 190)
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, double(-119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
				else if (afluencia_125 <= 209)
					a_termo_independente_calculo_ENA -= 119;
				else if (afluencia_125 <= 250) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA += 90;
				}//else if (afluencia_125 <= 250) {						
				else
					a_termo_independente_calculo_ENA -= 160;

				/////////////
				// -VAZ(203)
				/////////////
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 203, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				/////////////
				// +VAZ(304)
				/////////////

				//   VAZ(304) = vaz(315)-vaz(316)
				//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				//   VAZ(317) = MAX(0;VAZ(201)-25)
				//   VAZ(316) = MIN(VAZ(315);190)

				//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				// +VAZ(203)
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 203, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				// -VAZ(201)
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				// +VAZ(317)
				if ((afluencia_201 - 25) > 0) {
					// +VAZ(201)
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 25;

				}//if ((afluencia_201 - 25) > 0) {

				// +VAZ(298)
				if (afluencia_125 <= 190)
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
				else if (afluencia_125 <= 209)
					a_termo_independente_calculo_ENA += 119;
				else if (afluencia_125 <= 250) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 90;
				}//else if (afluencia_125 <= 250) {						
				else
					a_termo_independente_calculo_ENA += 160;

				//////////
				//-VAZ(316)
				//VAZ(316) = MIN(VAZ(315); 190)
				//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				if (afluencia_315 < 190) {

					// -VAZ(203)
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 203, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					// +VAZ(201)
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					// -VAZ(317)
					//   VAZ(317) = MAX(0;VAZ(201)-25)
					if ((afluencia_201 - 25) > 0) {
						// +VAZ(201)
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 25;

					}//if ((afluencia_201 - 25) > 0) {

					// -VAZ(298)
					if (afluencia_125 <= 190)
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, double(-119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
					else if (afluencia_125 <= 209)
						a_termo_independente_calculo_ENA -= 119;
					else if (afluencia_125 <= 250) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 90;
					}//else if (afluencia_125 <= 250) {						
					else
						a_termo_independente_calculo_ENA -= 160;
				}//if (afluencia_315 < 190) {
				else
					a_termo_independente_calculo_ENA -= 190;

			}//else if (codigo_posto_acoplamento_NW == 299) {//ILHA POMBOS
			else if (a_codigo_posto_acoplamento_ENA == 303) {//FONTES C

				//Regra de acoplamento NW (REGRAS.DAT): 
				// VAZ(303) = SE(VAZ(132)<17;VAZ(132)+MIN(VAZ(316)-(VAZ(131)+VAZ(123));34);17+MIN(VAZ(316)-(VAZ(131)+VAZ(123));34))
				// VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
				// VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				// VAZ(317) = MAX(0;VAZ(201)-25)
				// VAZ(316) = MIN(VAZ(315);190)
				// VAZ(132) = VAZ(202)+MIN(VAZ(201);25)
				// VAZ(131) = MIN(VAZ(316);144)-VAZ(123)

				/////////////////////////
				//Postos "reais"
				/////////////////////////

				const double afluencia_201 = get_afluencia_natural_posto(a_dados, a_periodoPE, 201, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_202 = get_afluencia_natural_posto(a_dados, a_periodoPE, 202, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_203 = get_afluencia_natural_posto(a_dados, a_periodoPE, 203, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_125 = get_afluencia_natural_posto(a_dados, a_periodoPE, 125, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_123 = get_afluencia_natural_posto(a_dados, a_periodoPE, 123, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);

				/////////////////////////
				//Postos "fictícios"
				/////////////////////////

				double afluencia_298 = -1;

				if (afluencia_125 <= 190)
					afluencia_298 = afluencia_125 * double(119.0 / 190.0);
				else if (afluencia_125 <= 209)
					afluencia_298 = 119;
				else if (afluencia_125 <= 250)
					afluencia_298 = afluencia_125 - 90;
				else
					afluencia_298 = 160;

				///////
				double afluencia_317 = 0;

				if ((afluencia_201 - 25) > afluencia_317)
					afluencia_317 = (afluencia_201 - 25);

				///////
				double afluencia_315 = afluencia_203 - afluencia_201 + afluencia_317 + afluencia_298;

				///////
				double afluencia_316 = 190;

				if (afluencia_315 < afluencia_316)
					afluencia_316 = afluencia_315;


				//////
				double afluencia_132 = 25;

				if (afluencia_201 < afluencia_132)
					afluencia_132 = afluencia_201;

				afluencia_132 += afluencia_202;

				//////
				double afluencia_131 = 144;

				if (afluencia_316 < afluencia_131)
					afluencia_131 = afluencia_316;

				afluencia_131 -= afluencia_123;


				//******************************************************************************************************************
				// VAZ(303) = SE(VAZ(132)<17;VAZ(132)+MIN(VAZ(316)-(VAZ(131)+VAZ(123));34);17+MIN(VAZ(316)-(VAZ(131)+VAZ(123));34))
				//******************************************************************************************************************

				if (afluencia_132 < 17) {

					/////////////
					// +VAZ(132) = VAZ(202)+MIN(VAZ(201);25)
					/////////////

					/////////////
					// +VAZ(202)
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 202, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					if (afluencia_201 < 25) {
						/////////////
						// +VAZ(201)
						/////////////
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
					}//if (afluencia_201 < 25) {
					else
						a_termo_independente_calculo_ENA += 25;

					////////////////////////////////////////
					//MIN(VAZ(316)-(VAZ(131)+VAZ(123));34)
					////////////////////////////////////////
					if ((afluencia_316 - (afluencia_131 + afluencia_123)) < 34) {

						/////////////
						// VAZ(316) = MIN(VAZ(315);190)
						/////////////

						if (afluencia_315 < 190) {

							//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
							// +VAZ(203)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 203, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

							// -VAZ(201)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

							// +VAZ(317) 
							//   VAZ(317) = MAX(0;VAZ(201)-25)
							if ((afluencia_201 - 25) > 0) {
								// +VAZ(201)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
								a_termo_independente_calculo_ENA -= 25;

							}//if ((afluencia_201 - 25) > 0) {

							// +VAZ(298)
							//   VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
							if (afluencia_125 <= 190)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
							else if (afluencia_125 <= 209)
								a_termo_independente_calculo_ENA += 119;
							else if (afluencia_125 <= 250) {
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
								a_termo_independente_calculo_ENA -= 90;
							}//else if (afluencia_125 <= 250) {						
							else
								a_termo_independente_calculo_ENA += 160;

						}
						else
							a_termo_independente_calculo_ENA += 190;

						/////////////
						// -VAZ(131)
						/////////////

						//VAZ(131) = MIN(VAZ(316); 144) - VAZ(123)

						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 123, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						if (afluencia_316 < 144) {

							/////////////
							//   -VAZ(316) = -MIN(VAZ(315);190)

							if (afluencia_315 < 190) {

								//   -VAZ(315) = (-VAZ(203)+VAZ(201))-VAZ(317)-VAZ(298)
								// -VAZ(203)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 203, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

								// +VAZ(201)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

								// -VAZ(317) 
								//   VAZ(317) = MAX(0;VAZ(201)-25)
								if ((afluencia_201 - 25) > 0) {
									// -VAZ(201)
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
									a_termo_independente_calculo_ENA += 25;

								}//if ((afluencia_201 - 25) > 0) {

								// -VAZ(298)
								//   VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
								if (afluencia_125 <= 190)
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, -double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
								else if (afluencia_125 <= 209)
									a_termo_independente_calculo_ENA -= 119;
								else if (afluencia_125 <= 250) {
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
									a_termo_independente_calculo_ENA += 90;
								}//else if (afluencia_125 <= 250) {						
								else
									a_termo_independente_calculo_ENA -= 160;

							}
							else
								a_termo_independente_calculo_ENA -= 190;

						}//if (afluencia_316 < 144) {
						else
							a_termo_independente_calculo_ENA -= 144;

						/////////////
						// -VAZ(123)
						/////////////
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 123, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					}
					else
						a_termo_independente_calculo_ENA += 34;


				}//if (afluencia_132 < 17) {
				else {

					a_termo_independente_calculo_ENA += 17;

					////////////////////////////////////////
					//MIN(VAZ(316)-(VAZ(131)+VAZ(123));34)
					////////////////////////////////////////
					if ((afluencia_316 - (afluencia_131 + afluencia_123)) < 34) {

						/////////////
						// VAZ(316) = MIN(VAZ(315);190)
						/////////////

						if (afluencia_315 < 190) {

							//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
							// +VAZ(203)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 203, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

							// -VAZ(201)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

							// +VAZ(317) 
							//   VAZ(317) = MAX(0;VAZ(201)-25)
							if ((afluencia_201 - 25) > 0) {
								// +VAZ(201)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
								a_termo_independente_calculo_ENA -= 25;

							}//if ((afluencia_201 - 25) > 0) {

							// +VAZ(298)
							//   VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
							if (afluencia_125 <= 190)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
							else if (afluencia_125 <= 209)
								a_termo_independente_calculo_ENA += 119;
							else if (afluencia_125 <= 250) {
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
								a_termo_independente_calculo_ENA -= 90;
							}//else if (afluencia_125 <= 250) {						
							else
								a_termo_independente_calculo_ENA += 160;

						}
						else
							a_termo_independente_calculo_ENA += 190;

						/////////////
						// -VAZ(131)
						/////////////

						//VAZ(131) = MIN(VAZ(316); 144) - VAZ(123)

						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 123, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						if (afluencia_316 < 144) {

							/////////////
							//   -VAZ(316) = -MIN(VAZ(315);190)

							if (afluencia_315 < 190) {

								//   -VAZ(315) = (-VAZ(203)+VAZ(201))-VAZ(317)-VAZ(298)
								// -VAZ(203)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 203, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

								// +VAZ(201)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

								// -VAZ(317) 
								//   VAZ(317) = MAX(0;VAZ(201)-25)
								if ((afluencia_201 - 25) > 0) {
									// -VAZ(201)
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
									a_termo_independente_calculo_ENA += 25;

								}//if ((afluencia_201 - 25) > 0) {

								// -VAZ(298)
								//   VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
								if (afluencia_125 <= 190)
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, -double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
								else if (afluencia_125 <= 209)
									a_termo_independente_calculo_ENA -= 119;
								else if (afluencia_125 <= 250) {
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
									a_termo_independente_calculo_ENA += 90;
								}//else if (afluencia_125 <= 250) {						
								else
									a_termo_independente_calculo_ENA -= 160;

							}
							else
								a_termo_independente_calculo_ENA -= 190;

						}//if (afluencia_316 < 144) {
						else
							a_termo_independente_calculo_ENA -= 144;

						/////////////
						// -VAZ(123)
						/////////////
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 123, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					}
					else
						a_termo_independente_calculo_ENA += 34;

				}//else {

			}//else if (codigo_posto_acoplamento_NW == 303) {//FONTES C
			else if (a_codigo_posto_acoplamento_ENA == 306) {//P.PASSOS

				//Regra de acoplamento NW (REGRAS.DAT): 
				// VAZ(306) = VAZ(303)+VAZ(131)
				// VAZ(303) = SE(VAZ(132)<17;VAZ(132)+MIN(VAZ(316)-(VAZ(131)+VAZ(123));34);17+MIN(VAZ(316)-(VAZ(131)+VAZ(123));34))
				// VAZ(132) = VAZ(202)+MIN(VAZ(201);25)
				// VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
				// VAZ(304) =vaz(315)-vaz(316)
				// VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				// VAZ(317) = MAX(0;VAZ(201)-25)
				// VAZ(316) = MIN(VAZ(315);190)
				// VAZ(131) = MIN(VAZ(316);144)-VAZ(123)

				/////////////////////////
				//Postos "reais"
				/////////////////////////

				const double afluencia_201 = get_afluencia_natural_posto(a_dados, a_periodoPE, 201, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_202 = get_afluencia_natural_posto(a_dados, a_periodoPE, 202, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_203 = get_afluencia_natural_posto(a_dados, a_periodoPE, 203, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_123 = get_afluencia_natural_posto(a_dados, a_periodoPE, 123, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_125 = get_afluencia_natural_posto(a_dados, a_periodoPE, 125, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);

				/////////////////////////
				//Postos "fictícios"
				/////////////////////////

				double afluencia_298 = -1;

				if (afluencia_125 <= 190)
					afluencia_298 = afluencia_125 * double(119.0 / 190.0);
				else if (afluencia_125 <= 209)
					afluencia_298 = 119;
				else if (afluencia_125 <= 250)
					afluencia_298 = afluencia_125 - 90;
				else
					afluencia_298 = 160;

				///////
				double afluencia_317 = 0;

				if ((afluencia_201 - 25) > afluencia_317)
					afluencia_317 = (afluencia_201 - 25);

				///////
				double afluencia_315 = afluencia_203 - afluencia_201 + afluencia_317 + afluencia_298;

				///////
				double afluencia_316 = 190;

				if (afluencia_315 < afluencia_316)
					afluencia_316 = afluencia_315;

				///////

				double afluencia_132 = 25;

				if (afluencia_201 < afluencia_132)
					afluencia_132 = afluencia_201;

				afluencia_132 += afluencia_202;

				//////
				double afluencia_131 = 144;

				if (afluencia_316 < afluencia_131)
					afluencia_131 = afluencia_316;

				afluencia_131 -= afluencia_123;

				//*******************************
				// VAZ(306) = VAZ(303)+VAZ(131)
				//*******************************

				// ***Atenção: diferença com regra.dat
				// Soma VAZ(123): Posto de Funil
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 123, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);


				/////////////
				//VAZ(303) = SE(VAZ(132)<17;VAZ(132)+MIN(VAZ(316)-(VAZ(131)+VAZ(123));34);17+MIN(VAZ(316)-(VAZ(131)+VAZ(123));34))
				/////////////
				if (afluencia_132 < 17) {

					/////////////
					// +VAZ(132) = VAZ(202)+MIN(VAZ(201);25)
					/////////////

					/////////////
					// +VAZ(202)
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 202, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					if (afluencia_201 < 25) {
						/////////////
						// +VAZ(201)
						/////////////
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
					}//if (afluencia_201 < 25) {
					else
						a_termo_independente_calculo_ENA += 25;

					////////////////////////////////////////
					//MIN(VAZ(316)-(VAZ(131)+VAZ(123));34)
					////////////////////////////////////////
					if ((afluencia_316 - (afluencia_131 + afluencia_123)) < 34) {

						/////////////
						// VAZ(316) = MIN(VAZ(315);190)
						/////////////

						if (afluencia_315 < 190) {

							//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
							// +VAZ(203)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 203, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

							// -VAZ(201)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

							// +VAZ(317) 
							//   VAZ(317) = MAX(0;VAZ(201)-25)
							if ((afluencia_201 - 25) > 0) {
								// +VAZ(201)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
								a_termo_independente_calculo_ENA -= 25;

							}//if ((afluencia_201 - 25) > 0) {

							// +VAZ(298)
							//   VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
							if (afluencia_125 <= 190)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
							else if (afluencia_125 <= 209)
								a_termo_independente_calculo_ENA += 119;
							else if (afluencia_125 <= 250) {
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
								a_termo_independente_calculo_ENA -= 90;
							}//else if (afluencia_125 <= 250) {						
							else
								a_termo_independente_calculo_ENA += 160;

						}
						else
							a_termo_independente_calculo_ENA += 190;

						/////////////
						// -VAZ(131)
						/////////////

						//VAZ(131) = MIN(VAZ(316); 144) - VAZ(123)

						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 123, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						if (afluencia_316 < 144) {

							/////////////
							//   -VAZ(316) = -MIN(VAZ(315);190)

							if (afluencia_315 < 190) {

								//   -VAZ(315) = (-VAZ(203)+VAZ(201))-VAZ(317)-VAZ(298)
								// -VAZ(203)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 203, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

								// +VAZ(201)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

								// -VAZ(317) 
								//   VAZ(317) = MAX(0;VAZ(201)-25)
								if ((afluencia_201 - 25) > 0) {
									// -VAZ(201)
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
									a_termo_independente_calculo_ENA += 25;

								}//if ((afluencia_201 - 25) > 0) {

								// -VAZ(298)
								//   VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
								if (afluencia_125 <= 190)
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, -double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
								else if (afluencia_125 <= 209)
									a_termo_independente_calculo_ENA -= 119;
								else if (afluencia_125 <= 250) {
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
									a_termo_independente_calculo_ENA += 90;
								}//else if (afluencia_125 <= 250) {						
								else
									a_termo_independente_calculo_ENA -= 160;

							}
							else
								a_termo_independente_calculo_ENA -= 190;

						}//if (afluencia_316 < 144) {
						else
							a_termo_independente_calculo_ENA -= 144;

						/////////////
						// -VAZ(123)
						/////////////
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 123, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					}
					else
						a_termo_independente_calculo_ENA += 34;


				}//if (afluencia_132 < 17) {
				else {

					a_termo_independente_calculo_ENA += 17;

					////////////////////////////////////////
					//MIN(VAZ(316)-(VAZ(131)+VAZ(123));34)
					////////////////////////////////////////
					if ((afluencia_316 - (afluencia_131 + afluencia_123)) < 34) {

						/////////////
						// VAZ(316) = MIN(VAZ(315);190)
						/////////////

						if (afluencia_315 < 190) {

							//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
							// +VAZ(203)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 203, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

							// -VAZ(201)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

							// +VAZ(317) 
							//   VAZ(317) = MAX(0;VAZ(201)-25)
							if ((afluencia_201 - 25) > 0) {
								// +VAZ(201)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
								a_termo_independente_calculo_ENA -= 25;

							}//if ((afluencia_201 - 25) > 0) {

							// +VAZ(298)
							//   VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
							if (afluencia_125 <= 190)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
							else if (afluencia_125 <= 209)
								a_termo_independente_calculo_ENA += 119;
							else if (afluencia_125 <= 250) {
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
								a_termo_independente_calculo_ENA -= 90;
							}//else if (afluencia_125 <= 250) {						
							else
								a_termo_independente_calculo_ENA += 160;

						}
						else
							a_termo_independente_calculo_ENA += 190;

						/////////////
						// -VAZ(131)
						/////////////

						//VAZ(131) = MIN(VAZ(316); 144) - VAZ(123)

						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 123, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						if (afluencia_316 < 144) {

							/////////////
							//   -VAZ(316) = -MIN(VAZ(315);190)

							if (afluencia_315 < 190) {

								//   -VAZ(315) = (-VAZ(203)+VAZ(201))-VAZ(317)-VAZ(298)
								// -VAZ(203)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 203, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

								// +VAZ(201)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

								// -VAZ(317) 
								//   VAZ(317) = MAX(0;VAZ(201)-25)
								if ((afluencia_201 - 25) > 0) {
									// -VAZ(201)
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
									a_termo_independente_calculo_ENA += 25;

								}//if ((afluencia_201 - 25) > 0) {

								// -VAZ(298)
								//   VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
								if (afluencia_125 <= 190)
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, -double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
								else if (afluencia_125 <= 209)
									a_termo_independente_calculo_ENA -= 119;
								else if (afluencia_125 <= 250) {
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
									a_termo_independente_calculo_ENA += 90;
								}//else if (afluencia_125 <= 250) {						
								else
									a_termo_independente_calculo_ENA -= 160;

							}
							else
								a_termo_independente_calculo_ENA -= 190;

						}//if (afluencia_316 < 144) {
						else
							a_termo_independente_calculo_ENA -= 144;

						/////////////
						// -VAZ(123)
						/////////////
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 123, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					}
					else
						a_termo_independente_calculo_ENA += 34;

				}//else {

				/////////////
				//VAZ(131) = MIN(VAZ(316);144)-VAZ(123)
				/////////////

				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 123, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				if (afluencia_316 < 144) {

					/////////////
					//   VAZ(316) = MIN(VAZ(315);190)

					if (afluencia_315 < 190) {

						//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
						// +VAZ(203)
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 203, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						// -VAZ(201)
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						// +VAZ(317) 
						//   VAZ(317) = MAX(0;VAZ(201)-25)
						if ((afluencia_201 - 25) > 0) {
							// +VAZ(201)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
							a_termo_independente_calculo_ENA -= 25;

						}//if ((afluencia_201 - 25) > 0) {

						// +VAZ(298)
						//   VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
						if (afluencia_125 <= 190)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
						else if (afluencia_125 <= 209)
							a_termo_independente_calculo_ENA += 119;
						else if (afluencia_125 <= 250) {
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
							a_termo_independente_calculo_ENA -= 90;
						}//else if (afluencia_125 <= 250) {						
						else
							a_termo_independente_calculo_ENA += 160;

					}
					else
						a_termo_independente_calculo_ENA += 190;

				}//if (afluencia_316 < 144) {
				else
					a_termo_independente_calculo_ENA += 144;



			}//else if (codigo_posto_acoplamento_NW == 306) {//P.PASSOS
			else if (a_codigo_posto_acoplamento_ENA == 318) {//HENRY BORDEN

				//Regra de acoplamento NW (REGRAS.DAT): 
				// VAZ(318) = VAZ(116)+0.1*(VAZ(161)-VAZ(117)-VAZ(301))+VAZ(117)+VAZ(301)
				// VAZ(116) = VAZ(119)-VAZ(301)
				//POST    MES  CONF    FORMULA
				//	XXX    XX     X    XXXXXXXXXXXXXXXXXXXXXXXXXXX
				//	119     1          VAZ(301) * 1.217 + 0.608
				//	119     2          VAZ(301) * 1.232 + 0.123
				//	119     3          VAZ(301) * 1.311 - 2.359
				//	119     4          VAZ(301) * 1.241 - 0.496
				//	119     5          VAZ(301) * 1.167 + 0.467
				//	119     6          VAZ(301) * 1.333 - 0.533
				//	119     7          VAZ(301) * 1.247 - 0.374
				//	119     8          VAZ(301) * 1.2 + 0.36
				//	119     9          VAZ(301) * 1.292 - 1.292
				//	119    10          VAZ(301) * 1.25 - 0.25
				//	119    11          VAZ(301) * 1.294 - 1.682
				//	119    12          VAZ(301) * 1.215 + 0.729

				/////////////////////////
				//Postos "reais"
				/////////////////////////

				const double afluencia_161 = get_afluencia_natural_posto(a_dados, a_periodoPE, 161, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_117 = get_afluencia_natural_posto(a_dados, a_periodoPE, 117, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_301 = get_afluencia_natural_posto(a_dados, a_periodoPE, 301, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);

				const IdMes idMes_alvo = get_IdMes_operativo(a_periodoPE, Periodo("7d", vetorProcessoEstocastico.at(a_idProcessoEstocastico).getIterador2Inicial(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario_1, Periodo())));

				//*************************************************************************
				// VAZ(318) = VAZ(116)+0.1*(VAZ(161)-VAZ(117)-VAZ(301))+VAZ(117)+VAZ(301)
				// VAZ(318) = VAZ(116)+0.1*VAZ(161)+0.9*VAZ(117)+0.9*VAZ(301)
				//*************************************************************************

				///////////
				// VAZ(116) = VAZ(119)-VAZ(301)
				///////////

				//////////
				//VAZ(119)
				//////////

				if (idMes_alvo == IdMes_1) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 301, 1.217, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA += 0.608;
				}//if (idMes_alvo == IdMes_1) {
				else if (idMes_alvo == IdMes_2) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 301, 1.232, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA += 0.123;
				}//else if (idMes_alvo == IdMes_2) {
				else if (idMes_alvo == IdMes_3) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 301, 1.311, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 2.359;
				}//else if (idMes_alvo == IdMes_3) {					
				else if (idMes_alvo == IdMes_4) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 301, 1.241, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 0.496;
				}//else if (idMes_alvo == IdMes_4) {					
				else if (idMes_alvo == IdMes_5) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 301, 1.167, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA += 0.467;
				}//else if (idMes_alvo == IdMes_5) {					
				else if (idMes_alvo == IdMes_6) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 301, 1.333, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 0.533;
				}//else if (idMes_alvo == IdMes_6) {					
				else if (idMes_alvo == IdMes_7) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 301, 1.247, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 0.374;
				}//else if (idMes_alvo == IdMes_7) {					
				else if (idMes_alvo == IdMes_8) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 301, 1.200, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA += 0.360;
				}//else if (idMes_alvo == IdMes_8) {					
				else if (idMes_alvo == IdMes_9) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 301, 1.292, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 1.292;
				}//else if (idMes_alvo == IdMes_9) {					
				else if (idMes_alvo == IdMes_10) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 301, 1.250, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 0.250;
				}//else if (idMes_alvo == IdMes_10) {				
				else if (idMes_alvo == IdMes_11) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 301, 1.294, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 1.682;
				}//else if (idMes_alvo == IdMes_11) {					
				else if (idMes_alvo == IdMes_12) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 301, 1.215, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA += 0.729;
				}//else if (idMes_alvo == IdMes_12) {					
				else { throw std::invalid_argument("Nao identificado idMes: " + getString(idMes_alvo) + " \n"); }

				//////////
				//-VAZ(301)
				//////////
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 301, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				//////////////////////////

				///////////
				// 0.1 * VAZ(161)
				///////////
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 161, 0.1, a_coeficiente_idHidreletricas_calculo_ENA);

				///////////
				// 0.9 * VAZ(117)
				///////////
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 117, 0.9, a_coeficiente_idHidreletricas_calculo_ENA);

				///////////
				// 0.9 * VAZ(301)
				///////////
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 301, 0.9, a_coeficiente_idHidreletricas_calculo_ENA);


			}//else if (codigo_posto_acoplamento_NW == 318) {//HENRY BORDEN
			else if (a_codigo_posto_acoplamento_ENA == 304) {//SANTANA


				//Regra de acoplamento NW (REGRAS.DAT): 
				// VAZ(304) = vaz(315)-vaz(316)
				// VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
				// VAZ(304) =vaz(315)-vaz(316)
				// VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				// VAZ(317) = MAX(0;VAZ(201)-25)
				// VAZ(316) = MIN(VAZ(315);190)

				/////////////////////////
				//Postos "reais"
				/////////////////////////

				const double afluencia_203 = get_afluencia_natural_posto(a_dados, a_periodoPE, 203, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_201 = get_afluencia_natural_posto(a_dados, a_periodoPE, 201, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_125 = get_afluencia_natural_posto(a_dados, a_periodoPE, 125, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);

				/////////////////////////
				//Postos "fictícios"
				/////////////////////////

				double afluencia_298 = -1;

				if (afluencia_125 <= 190)
					afluencia_298 = afluencia_125 * double(119.0 / 190.0);
				else if (afluencia_125 <= 209)
					afluencia_298 = 119;
				else if (afluencia_125 <= 250)
					afluencia_298 = afluencia_125 - 90;
				else
					afluencia_298 = 160;

				///////
				double afluencia_317 = 0;

				if ((afluencia_201 - 25) > afluencia_317)
					afluencia_317 = (afluencia_201 - 25);

				///////
				double afluencia_315 = afluencia_203 - afluencia_201 + afluencia_317 + afluencia_298;

				//***************************************
				//VAZ(304) = vaz(315) - vaz(316)
				//***************************************

				//   VAZ(304) = vaz(315)-vaz(316)
				//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				//   VAZ(317) = MAX(0;VAZ(201)-25)
				//   VAZ(316) = MIN(VAZ(315);190)

				//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				// +VAZ(203)
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 203, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				// -VAZ(201)
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				// +VAZ(317)
				if ((afluencia_201 - 25) > 0) {
					// +VAZ(201)
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 25;

				}//if ((afluencia_201 - 25) > 0) {

				// +VAZ(298)
				if (afluencia_125 <= 190)
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
				else if (afluencia_125 <= 209)
					a_termo_independente_calculo_ENA += 119;
				else if (afluencia_125 <= 250) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 90;
				}//else if (afluencia_125 <= 250) {						
				else
					a_termo_independente_calculo_ENA += 160;

				//////////
				//-VAZ(316)
				//VAZ(316) = MIN(VAZ(315); 190)
				//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				if (afluencia_315 < 190) {

					// -VAZ(203)
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 203, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					// +VAZ(201)
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					// -VAZ(317)
					//   VAZ(317) = MAX(0;VAZ(201)-25)
					if ((afluencia_201 - 25) > 0) {
						// +VAZ(201)
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 25;

					}//if ((afluencia_201 - 25) > 0) {

					// -VAZ(298)
					if (afluencia_125 <= 190)
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, -double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
					else if (afluencia_125 <= 209)
						a_termo_independente_calculo_ENA -= 119;
					else if (afluencia_125 <= 250) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 125, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 90;
					}//else if (afluencia_125 <= 250) {						
					else
						a_termo_independente_calculo_ENA -= 160;


				}//if (afluencia_315 < 190) {							
				else
					a_termo_independente_calculo_ENA -= 190;


			}//else if (codigo_posto_acoplamento_NW == 304) {//SANTANA
			else if (a_codigo_posto_acoplamento_ENA == 132) {//LAJES

				//Regra de acoplamento NW (REGRAS.DAT): 
				//VAZ(132) = VAZ(202)+MIN(VAZ(201);25)

				/////////////////////////
				//Postos "reais"
				/////////////////////////

				const double afluencia_202 = get_afluencia_natural_posto(a_dados, a_periodoPE, 202, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);
				const double afluencia_201 = get_afluencia_natural_posto(a_dados, a_periodoPE, 201, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);

				////////////
				// +VAZ(202)
				////////////
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 202, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				if (afluencia_201 < 25) {

					////////////
					// +VAZ(201)
					////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				}//if (afluencia_201 < 25) {
				else
					a_termo_independente_calculo_ENA += 25;


			}//else if (codigo_posto_acoplamento_NW == 132) {//LAJES
			else if (a_codigo_posto_acoplamento_ENA == 3) {//GUARAPIRANGA

				//Regra de acoplamento NW (REGRAS.DAT): VAZ(3) = 0
				a_termo_independente_calculo_ENA += 0.0;

			}//else if (codigo_posto_acoplamento_NW == 3) {//GUARAPIRANGA
			else if (a_codigo_posto_acoplamento_ENA == 4) {//BILLINGS

				//Regra de acoplamento NW (REGRAS.DAT): VAZ(4) = 0
				a_termo_independente_calculo_ENA += 0.0;

			}//else if (codigo_posto_acoplamento_NW == 4) {//BILLINGS
			else if (a_codigo_posto_acoplamento_ENA == 5) {//PEDREIRA

				//Regra de acoplamento NW (REGRAS.DAT): VAZ(5) = 0
				a_termo_independente_calculo_ENA += 0.0;

			}//else if (codigo_posto_acoplamento_NW == 5) {//PEDREIRA
			else if (a_codigo_posto_acoplamento_ENA == 13) {//TRAICAO

				//Regra de acoplamento NW (REGRAS.DAT): VAZ(13) = 0
				a_termo_independente_calculo_ENA += 0.0;

			}//else if (codigo_posto_acoplamento_NW == 13) {//TRAICAO
			else if (a_codigo_posto_acoplamento_ENA == 21) {//SANTA CECILIA (RETIRADA DE SANTA CECILIA DA CONFIGURACAO PARA O CALCULO DA ENA)

				//Regra de acoplamento NW (REGRAS.DAT): VAZ(21) = VAZ(123)
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 123, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

			}//else if (codigo_posto_acoplamento_NW == 21) {//SANTA CECILIA (RETIRADA DE SANTA CECILIA DA CONFIGURACAO PARA O CALCULO DA ENA)
			else if (a_codigo_posto_acoplamento_ENA == 19) {//TOCOS (RETIRADA DE TOCOS DA CONFIGURACAO PARA O CALCULO DA ENA)

				//Regra de acoplamento NW (REGRAS.DAT): VAZ(19) = 0
				a_termo_independente_calculo_ENA += 0.0;

			}//else if (codigo_posto_acoplamento_NW == 19) {//TOCOS (RETIRADA DE TOCOS DA CONFIGURACAO PARA O CALCULO DA ENA)
			else if (a_codigo_posto_acoplamento_ENA == 292) {//BELO MONTE

				//////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//288 - Belo Monte
				// Nota: Segue o hidrograma para Belo Monte
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////

				//Regra de acoplamento NW (REGRAS.DAT):
				//POST    MES  CONF    FORMULA
				//	XXX    XX     X    XXXXXXXXXXXXXXXXXXXXXXXXXXX
				//  292     1          SE(VAZ(288) <= 1100; 0; SE(VAZ(288) <= (1100 + 13900); VAZ(288) - 1100; 13900))
				//  292     2          SE(VAZ(288) <= 1600; 0; SE(VAZ(288) <= (1600 + 13900); VAZ(288) - 1600; 13900))
				//	292     3          SE(VAZ(288) <= 3250; 0; SE(VAZ(288) <= (3250 + 13900); VAZ(288) - 3250; 13900))
				//	292     4          SE(VAZ(288) <= 6000; 0; SE(VAZ(288) <= (6000 + 13900); VAZ(288) - 6000; 13900))
				//	292     5          SE(VAZ(288) <= 2900; 0; SE(VAZ(288) <= (2900 + 13900); VAZ(288) - 2900; 13900))
				//	292     6          SE(VAZ(288) <= 1600; 0; SE(VAZ(288) <= (1600 + 13900); VAZ(288) - 1600; 13900))
				//	292     7          SE(VAZ(288) <= 1100; 0; SE(VAZ(288) <= (1100 + 13900); VAZ(288) - 1100; 13900))
				//	292     8          SE(VAZ(288) <= 900; 0; SE(VAZ(288) <= (900 + 13900); VAZ(288) - 900; 13900))
				//	292     9          SE(VAZ(288) <= 750; 0; SE(VAZ(288) <= (750 + 13900); VAZ(288) - 750; 13900))
				//	292    10          SE(VAZ(288) <= 700; 0; SE(VAZ(288) <= (700 + 13900); VAZ(288) - 700; 13900))
				//	292    11          SE(VAZ(288) <= 800; 0; SE(VAZ(288) <= (800 + 13900); VAZ(288) - 800; 13900))
				//	292    12          SE(VAZ(288) <= 900; 0; SE(VAZ(288) <= (900 + 13900); VAZ(288) - 900; 13900))


				/////////////////////////
				//Postos "reais"
				/////////////////////////
				const double afluencia_288 = get_afluencia_natural_posto(a_dados, a_periodoPE, 288, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);

				///////

				const IdMes idMes_alvo = get_IdMes_operativo(a_periodoPE, Periodo("7d", vetorProcessoEstocastico.at(a_idProcessoEstocastico).getIterador2Inicial(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario_1, Periodo())));


				if (idMes_alvo == IdMes_1) {

					if (afluencia_288 <= 1100)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (1100 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 1100;
					}//else if (afluencia_288 <= (1100 + 13900)) {						
					else
						a_termo_independente_calculo_ENA += 13900;

				}//if (idMes_alvo == IdMes_1) {
				else if (idMes_alvo == IdMes_2) {

					if (afluencia_288 <= 1600)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (1600 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 1600;
					}
					else
						a_termo_independente_calculo_ENA += 13900;

				}//else if (idMes_alvo == IdMes_2) {
				else if (idMes_alvo == IdMes_3) {

					if (afluencia_288 <= 3250)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (3250 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 3250;
					}
					else
						a_termo_independente_calculo_ENA += 13900;

				}//else if (idMes_alvo == IdMes_3) {
				else if (idMes_alvo == IdMes_4) {

					if (afluencia_288 <= 6000)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (6000 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 6000;
					}
					else
						a_termo_independente_calculo_ENA += 13900;

				}//else if (idMes_alvo == IdMes_4) {
				else if (idMes_alvo == IdMes_5) {

					if (afluencia_288 <= 2900)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (2900 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 2900;
					}
					else
						a_termo_independente_calculo_ENA += 13900;

				}//else if (idMes_alvo == IdMes_5) {
				else if (idMes_alvo == IdMes_6) {

					if (afluencia_288 <= 1600)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (1600 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 1600;
					}
					else
						a_termo_independente_calculo_ENA += 13900;

				}//else if (idMes_alvo == IdMes_6) {
				else if (idMes_alvo == IdMes_7) {

					if (afluencia_288 <= 1100)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (1100 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 1100;
					}
					else
						a_termo_independente_calculo_ENA += 13900;

				}//else if (idMes_alvo == IdMes_7) {
				else if (idMes_alvo == IdMes_8) {

					if (afluencia_288 <= 900)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (900 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 900;
					}
					else
						a_termo_independente_calculo_ENA += 13900;

				}//else if (idMes_alvo == IdMes_8) {
				else if (idMes_alvo == IdMes_9) {

					if (afluencia_288 <= 750)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (750 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 750;
					}
					else
						a_termo_independente_calculo_ENA += 13900;

				}//else if (idMes_alvo == IdMes_9) {
				else if (idMes_alvo == IdMes_10) {

					if (afluencia_288 <= 700)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (700 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 700;
					}
					else
						a_termo_independente_calculo_ENA += 13900;

				}//else if (idMes_alvo == IdMes_10) {
				else if (idMes_alvo == IdMes_11) {

					if (afluencia_288 <= 800)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (800 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 800;
					}
					else
						a_termo_independente_calculo_ENA += 13900;

				}//else if (idMes_alvo == IdMes_11) {
				else if (idMes_alvo == IdMes_12) {

					if (afluencia_288 <= 900)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (900 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 900;
					}
					else
						a_termo_independente_calculo_ENA += 13900;

				}//else if (idMes_alvo == IdMes_12) {
				else { throw std::invalid_argument("Nao identificado idMes: " + getString(idMes_alvo) + " \n"); }

			}//else if (codigo_posto_acoplamento_NW == 292) {//BELO MONTE
			else if (a_codigo_posto_acoplamento_ENA == 302) {//PIMENTAL

				//////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//314 - Pimental
				// Nota: Segue o hidrograma para Belo Monte
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Regra:
				//VAZ(302) = VAZ(288) - VAZ(292)

				/////////////////////////
				//Postos "reais"
				/////////////////////////
				const double afluencia_288 = get_afluencia_natural_posto(a_dados, a_periodoPE, 288, a_idCenario, a_idRealizacao, a_idProcessoEstocastico);

				//**********************************
				//VAZ(302) = VAZ(288) - VAZ(292)
				//**********************************

				//////////
				//VAZ(288)
				//////////
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				//////////
				//-VAZ(292)
				//////////
				const IdMes idMes_alvo = get_IdMes_operativo(a_periodoPE, Periodo("7d", vetorProcessoEstocastico.at(a_idProcessoEstocastico).getIterador2Inicial(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario_1, Periodo())));

				if (idMes_alvo == IdMes_1) {

					if (afluencia_288 <= 1100)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (1100 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 1100;
					}//else if (afluencia_288 <= (1100 + 13900)) {						
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//if (idMes_alvo == IdMes_1) {
				else if (idMes_alvo == IdMes_2) {

					if (afluencia_288 <= 1600)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (1600 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 1600;
					}
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//else if (idMes_alvo == IdMes_2) {
				else if (idMes_alvo == IdMes_3) {

					if (afluencia_288 <= 3250)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (3250 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 3250;
					}
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//else if (idMes_alvo == IdMes_3) {
				else if (idMes_alvo == IdMes_4) {

					if (afluencia_288 <= 6000)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (6000 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 6000;
					}
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//else if (idMes_alvo == IdMes_4) {
				else if (idMes_alvo == IdMes_5) {

					if (afluencia_288 <= 2900)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (2900 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 2900;
					}
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//else if (idMes_alvo == IdMes_5) {
				else if (idMes_alvo == IdMes_6) {

					if (afluencia_288 <= 1600)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (1600 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 1600;
					}
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//else if (idMes_alvo == IdMes_6) {
				else if (idMes_alvo == IdMes_7) {

					if (afluencia_288 <= 1100)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (1100 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 1100;
					}
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//else if (idMes_alvo == IdMes_7) {
				else if (idMes_alvo == IdMes_8) {

					if (afluencia_288 <= 900)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (900 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 900;
					}
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//else if (idMes_alvo == IdMes_8) {
				else if (idMes_alvo == IdMes_9) {

					if (afluencia_288 <= 750)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (750 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 750;
					}
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//else if (idMes_alvo == IdMes_9) {
				else if (idMes_alvo == IdMes_10) {

					if (afluencia_288 <= 700)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (700 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 700;
					}
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//else if (idMes_alvo == IdMes_10) {
				else if (idMes_alvo == IdMes_11) {

					if (afluencia_288 <= 800)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (800 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 800;
					}
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//else if (idMes_alvo == IdMes_11) {
				else if (idMes_alvo == IdMes_12) {

					if (afluencia_288 <= 900)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (900 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 900;
					}
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//else if (idMes_alvo == IdMes_12) {
				else { throw std::invalid_argument("Nao identificado idMes: " + getString(idMes_alvo) + " \n"); }

			}//else if (codigo_posto_acoplamento_NW == 292) {//BELO MONTE						
			else if (a_codigo_posto_acoplamento_ENA == 169) {//SOBRADINHO - Posto para acoplamento com a FCF

				if (a_periodoPE >= vetorProcessoEstocastico.at(a_idProcessoEstocastico).getIterador2Inicial(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario_1, Periodo())) {

					//Nota: Como sobradinho fictício (posto = 169) está isolado do sistema, devem ser somadas as naturais de Três Marias e Queimado

					/////////////
					// VAZ(156) //Três Marias
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 156, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					/////////////
					// VAZ(158) //Queimado
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 158, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					/////////////
					// VAZ(169) //Sobradinho Fictício
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 169, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				}//if (a_periodoPE >= vetorProcessoEstocastico.at(a_idProcessoEstocastico).getIterador2Inicial(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario_1, Periodo())) {
				else {

					//******************************
					//Tendência
					//******************************
					//Pega a informação da tendência do arquivo VAZOES.DAT (do modelo GEVAZP)-> Por construção esta informação já está em afluência natural
					const int posto = 169;
					a_termo_independente_calculo_ENA += vetorProcessoEstocastico.at(a_idProcessoEstocastico).getElementoVetor(mapIdVar.at(lista_codPosto_idHidreletrica.at(posto)), mapIdVarInterna.at(lista_codPosto_idHidreletrica.at(posto)), AttVetorVariavelAleatoriaInterna_tendencia_temporal, a_periodoPE, double());

				}//else

			}//else if (codigo_posto_acoplamento_NW == 169) {//SOBRADINHO - Posto para acoplamento com a FCF
			else if (a_codigo_posto_acoplamento_ENA == 172) {//ITAPARICA

				//Usina com codigo_posto para otimização setado como 300 (afluência incremental = 0) mas com posto de acoplamento para cálculo das ENAs = posto do hidr.dat

				if (a_periodoPE >= vetorProcessoEstocastico.at(a_idProcessoEstocastico).getIterador2Inicial(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario_1, Periodo())) {

					//Nota: Como sobradinho fictício (posto = 169) está isolado do sistema, devem ser somadas as naturais de Três Marias e Queimado

					/////////////
					// VAZ(156) //Três Marias
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 156, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					/////////////
					// VAZ(158) //Queimado
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 158, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					/////////////
					// VAZ(169) //Sobradinho Fictício
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 169, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				}//if (a_periodoPE >= vetorProcessoEstocastico.at(a_idProcessoEstocastico).getIterador2Inicial(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario_1, Periodo())) {
				else {

					//******************************
					//Tendência
					//******************************
					//Pega a informação da tendência do arquivo VAZOES.DAT (do modelo GEVAZP)-> Por construção esta informação já está em afluência natural
					const int posto = 172;
					a_termo_independente_calculo_ENA += vetorProcessoEstocastico.at(a_idProcessoEstocastico).getElementoVetor(mapIdVar.at(lista_codPosto_idHidreletrica.at(posto)), mapIdVarInterna.at(lista_codPosto_idHidreletrica.at(posto)), AttVetorVariavelAleatoriaInterna_tendencia_temporal, a_periodoPE, double());

				}//else {

			}//else if (codigo_posto_acoplamento_NW == 172) {//ITAPARICA
			else if (a_codigo_posto_acoplamento_ENA == 178) {//XINGO

				//Usina com codigo_posto para otimização setado como 300 (afluência incremental = 0) mas com posto de acoplamento para cálculo das ENAs = posto do hidr.dat

				if (a_periodoPE >= vetorProcessoEstocastico.at(a_idProcessoEstocastico).getIterador2Inicial(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario_1, Periodo())) {

					//Nota: Como sobradinho fictício (posto = 169) está isolado do sistema, devem ser somadas as naturais de Três Marias e Queimado

					/////////////
					// VAZ(156) //Três Marias
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 156, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					/////////////
					// VAZ(158) //Queimado
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 158, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					/////////////
					// VAZ(169) //Sobradinho Fictício
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 169, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				}//if (a_periodoPE >= vetorProcessoEstocastico.at(a_idProcessoEstocastico).getIterador2Inicial(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario_1, Periodo())) {
				else {

					//******************************
					//Tendência
					//******************************
					//Pega a informação da tendência do arquivo VAZOES.DAT (do modelo GEVAZP)-> Por construção esta informação já está em afluência natural
					const int posto = 178;
					a_termo_independente_calculo_ENA += vetorProcessoEstocastico.at(a_idProcessoEstocastico).getElementoVetor(mapIdVar.at(lista_codPosto_idHidreletrica.at(posto)), mapIdVarInterna.at(lista_codPosto_idHidreletrica.at(posto)), AttVetorVariavelAleatoriaInterna_tendencia_temporal, a_periodoPE, double());

				}//else if (a_periodo < a_periodo_inicial_horizonte_estudo) {
			}//else if (codigo_posto_acoplamento_NW == 178) {//XINGO
			else if (a_codigo_posto_acoplamento_ENA == 976) {//176-COMP-MOX-PAFONSO (atribuido o codigo_posto_acoplamento_ENA = 976 para ser compatível com a lógica das regras)

				if (a_periodoPE >= vetorProcessoEstocastico.at(a_idProcessoEstocastico).getIterador2Inicial(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario_1, Periodo())) {


					//Nota: Como sobradinho fictício (posto = 169) está isolado do sistema, devem ser somadas as naturais de Três Marias e Queimado

					/////////////
					// VAZ(156) //Três Marias
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 156, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					/////////////
					// VAZ(158) //Queimado
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 158, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					/////////////
					// VAZ(169) //Sobradinho Fictício
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, IdHidreletrica_Nenhum, 169, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				}//if (a_periodoPE >= vetorProcessoEstocastico.at(a_idProcessoEstocastico).getIterador2Inicial(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario_1, Periodo())) {
				else {

					//******************************
					//Tendência
					//******************************
					//Pega a informação da tendência do arquivo VAZOES.DAT (do modelo GEVAZP)-> Por construção esta informação já está em afluência natural
					const int posto = 176;
					a_termo_independente_calculo_ENA += vetorProcessoEstocastico.at(a_idProcessoEstocastico).getElementoVetor(mapIdVar.at(lista_codPosto_idHidreletrica.at(posto)), mapIdVarInterna.at(lista_codPosto_idHidreletrica.at(posto)), AttVetorVariavelAleatoriaInterna_tendencia_temporal, a_periodoPE, double());

				}//else if (a_periodo < a_periodo_inicial_horizonte_estudo) {

			}//else if (codigo_posto_acoplamento_NW == 176) {
			else if (a_codigo_posto_acoplamento_ENA == 300) {//MOXOTÓ / P.AFONSO 123 / P.AFONSO 4

				//Nota: estas usinas são contabilizadas no posto do 176-COMP.MOX.PAFON
				//******************************
				//posto 300
				//******************************
				a_termo_independente_calculo_ENA += 0.0;

			}//else if (codigo_posto_acoplamento_NW == 300) {//MOXOTÓ / P.AFONSO 123 / P.AFONSO 4
			else { throw std::invalid_argument("Nao implementada regra de calculo de vazao para posto com codigo: " + getString(a_codigo_posto_acoplamento_ENA) + "\n"); }

		}//else {

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::retorna_equacionamento_regras_afluencia_natural_x_idHidreletrica(" + getFullString(a_idHidreletrica) + " periodo: " + getFullString(a_periodoPE) + ") : \n" + std::string(erro.what())); }

}

void ModeloOtimizacao::retorna_equacionamento_afluencia_natural_x_posto(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const int a_codigo_posto, const double a_coeficiente, std::vector<SmartEnupla<IdHidreletrica, double>>& a_coeficiente_idHidreletricas_calculo_ENA) {

	try {

		const IdHidreletrica menorIdHidreletrica = a_dados.getMenorId(IdHidreletrica());
		const IdHidreletrica maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

		if (a_codigo_posto != 300) {//Existem várias idHidrelétricas com o posto 300 (incremental = 0)

			const IdHidreletrica idHidreletrica = lista_codPosto_idHidreletrica.at(a_codigo_posto);

			const int iterador_inicial = idHidreletricas_x_usina_calculo_ENA.at(idHidreletrica).getIteradorInicial();
			const int iterador_final = idHidreletricas_x_usina_calculo_ENA.at(idHidreletrica).getIteradorFinal();

			for (int pos = iterador_inicial; pos <= iterador_final; pos++) {
				a_coeficiente_idHidreletricas_calculo_ENA.push_back(SmartEnupla<IdHidreletrica, double>(idHidreletricas_x_usina_calculo_ENA.at(idHidreletrica).at(pos), std::vector<double>(1, a_coeficiente)));
			}

		}//if (a_codigo_posto != 300) {
		else {

			if (a_idHidreletrica == IdHidreletrica_Nenhum) { throw std::invalid_argument("Deve ser informado um idHidreletrica\n"); }

			const int iterador_inicial = idHidreletricas_x_usina_calculo_ENA.at(a_idHidreletrica).getIteradorInicial();
			const int iterador_final = idHidreletricas_x_usina_calculo_ENA.at(a_idHidreletrica).getIteradorFinal();

			for (int pos = iterador_inicial; pos <= iterador_final; pos++) {
				a_coeficiente_idHidreletricas_calculo_ENA.push_back(SmartEnupla<IdHidreletrica, double>(idHidreletricas_x_usina_calculo_ENA.at(a_idHidreletrica).at(pos), std::vector<double>(1, a_coeficiente)));
			}

		}//else {

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::retorna_equacionamento_afluencia_natural_x_posto: \n" + std::string(erro.what())); }

}

double ModeloOtimizacao::getRealizacaoInterna(const IdVariavelAleatoria a_idVariavelAleatoria, const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const Periodo a_periodoPE) {

	try {

		//*****************************************************************************************************************************************************************************************
		//Para o forward:  pega o valor do AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral
		//Para o backward: Se periodo_PE < periodo_PE_final -> pega o valor do AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral (valor já calculado para o idCenario)
		//                 Se periodo_PE = periodo_PE_final -> calcularRealizacao com o a_idCenario e a_idRealizacao do backward
		//*****************************************************************************************************************************************************************************************

		double afluencia_incremental = 0.0;	
		const IdProcessoEstocastico idProcessoEstocastico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

		// forward
		if (a_idRealizacao == IdRealizacao_Nenhum || a_periodoPE < vetorProcessoEstocastico.at(idProcessoEstocastico).getIterador2Final(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario_1, Periodo()))
			afluencia_incremental = vetorProcessoEstocastico.at(idProcessoEstocastico).getElementoMatriz(a_idVariavelAleatoria, AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral, a_periodoPE, a_idCenario, double());

		// backward
		else if (a_idRealizacao > IdRealizacao_Nenhum)
			afluencia_incremental = vetorProcessoEstocastico.at(idProcessoEstocastico).calcularRealizacao(a_idVariavelAleatoria, a_idCenario, a_idRealizacao, a_periodoPE);

		else
			throw std::invalid_argument("Nao encontrada condicao para atualizar afluencia_incremental");


		afluencia_incremental = vetorProcessoEstocastico.at(idProcessoEstocastico).calcularRealizacaoInterna(a_idVariavelAleatoria, a_idVariavelAleatoriaInterna, a_periodoPE, afluencia_incremental);


		return afluencia_incremental;

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::getRealizacaoInterna: \n" + std::string(erro.what())); }

}

double ModeloOtimizacao::get_afluencia_natural_posto(Dados& a_dados, const Periodo a_periodoPE, const int a_codigo_posto, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const IdProcessoEstocastico a_idProcessoEstocastico) {

	try {

		//Nota: soma as incrementais de todas as usinas que estejam a montante da usina com codigo_posto = a_codigo_posto

		double afluencia_natural_posto = 0.0;

		const IdProcessoEstocastico idProcessoEstocastico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

		const IdHidreletrica idHidreletrica = lista_codPosto_idHidreletrica.at(a_codigo_posto);

		if (idHidreletrica == IdHidreletrica_Nenhum)
			throw std::invalid_argument("Nao encontrada idHidreletrica para o codigo_posto: " + getString(a_codigo_posto));

		const int iterador_inicial = idHidreletricas_x_usina_calculo_ENA.at(idHidreletrica).getIteradorInicial();
		const int iterador_final = idHidreletricas_x_usina_calculo_ENA.at(idHidreletrica).getIteradorFinal();


		for (int pos = iterador_inicial; pos <= iterador_final; pos++) {

			const IdHidreletrica idHidreletrica_aux = idHidreletricas_x_usina_calculo_ENA.at(idHidreletrica).at(pos);

			if (a_periodoPE < vetorProcessoEstocastico.at(a_idProcessoEstocastico).getIterador2Inicial(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario_1, Periodo())) { //Valores da tendência
				const double afluencia_incr = vetorProcessoEstocastico.at(idProcessoEstocastico).getElementoVetor(mapIdVar.at(idHidreletrica_aux), mapIdVarInterna.at(idHidreletrica_aux), AttVetorVariavelAleatoriaInterna_tendencia_temporal, a_periodoPE, double());
				if (afluencia_incr > 0.0)
					afluencia_natural_posto += afluencia_incr;
			}
			else {//Valores dentro da árvore
				const double afluencia_incr = getRealizacaoInterna(mapIdVar.at(idHidreletrica_aux), mapIdVarInterna.at(idHidreletrica_aux), a_idCenario, a_idRealizacao, a_periodoPE);
				if (afluencia_incr > 0.0)
					afluencia_natural_posto += afluencia_incr;
			}
			
		}//for (pos = iterador_inicial; pos <= iterador_final; pos++) {

		return afluencia_natural_posto;

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::get_afluencia_natural_posto: \n" + std::string(erro.what())); }

}

IdMes ModeloOtimizacao::get_IdMes_operativo(const Periodo a_periodoPE, const Periodo a_periodo_inicial_semanal)
{
	try {

		Periodo periodo_teste = a_periodoPE;

		if (a_periodoPE.sobreposicao(a_periodo_inicial_semanal) > 0) {//Para garantir que a primeira semana operativa corresponda ao mês operativo			
			periodo_teste = Periodo("7d", periodo_teste) + 1;		
		}

		return periodo_teste.getMes();

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::get_IdMes_operativo: \n" + std::string(erro.what())); }

}


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

					vetorEstagio.at(a_idEstagio).getSolver(tSS)->setLimInferior(idVariavelDecisao, valor);
					vetorEstagio.at(a_idEstagio).getSolver(tSS)->setLimSuperior(idVariavelDecisao, valor);

					if (idVariavelRealizacao == maiorIdVariavelRealizacao)
						vetorEstagio.at(a_idEstagio).getSolver(tSS)->atualizar();

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

					vetorEstagio.at(a_idEstagio).getSolver(tSS)->setLimInferior(idVariavelDecisao, valor);
					vetorEstagio.at(a_idEstagio).getSolver(tSS)->setLimSuperior(idVariavelDecisao, valor);

					if (idVariavelRealizacao == maiorIdVariavelRealizacao)
						vetorEstagio.at(a_idEstagio).getSolver(tSS)->atualizar();

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
						vetorEstagio.at(a_idEstagio).getSolver(tSS)->setRHSRestricao(idEquacao, getElementoVetor(a_idEstagio, idRestricaoCenario, AttVetorRestricaoCenario_rhs, a_idCenario, double()));

					if (getSize1Matriz(a_idEstagio, idRestricaoCenario, AttMatrizRestricaoCenario_coeficiente) > 0) {

						const int menorVar = getIterador1Inicial(a_idEstagio, idRestricaoCenario, AttMatrizRestricaoCenario_coeficiente, int());
						const int maiorVar = getIterador1Final(a_idEstagio, idRestricaoCenario, AttMatrizRestricaoCenario_coeficiente, int());

						for (int var = menorVar; var <= maiorVar; var++) {

							if (getSize2Matriz(a_idEstagio, idRestricaoCenario, AttMatrizRestricaoCenario_coeficiente, var) > 0)
								vetorEstagio.at(a_idEstagio).getSolver(tSS)->setCofRestricao(var, idEquacao,  getElementoMatriz(a_idEstagio, idRestricaoCenario, AttMatrizRestricaoCenario_coeficiente, var, a_idCenario, double()));

						} // for (int var = menorVar; var <= maiorVar; var++) {

					} // if (getSize1Matriz(a_idEstagio, idRestricaoCenario, AttMatrizRestricaoCenario_coeficiente) > 0) {

					if (idRestricaoCenario == maiorIdRestricaoCenario)
						vetorEstagio.at(a_idEstagio).getSolver(tSS)->atualizar();

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
					valor = vetorProcessoEstocastico.at(idProcessoEstocastico).calcularRealizacaoInterna(idVariavelAleatoria, idVariavelAleatoriaInterna, periodo, vetorProcessoEstocastico.at(idProcessoEstocastico).getElementoMatriz(idVariavelAleatoria, AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral, periodo, a_idCenario, double()));
				else
					valor = vetorProcessoEstocastico.at(idProcessoEstocastico).calcularRealizacaoInterna(idVariavelAleatoria, idVariavelAleatoriaInterna, periodo, vetorProcessoEstocastico.at(idProcessoEstocastico).calcularRealizacao(idVariavelAleatoria, a_idCenario, a_idRealizacao, periodo));

				double valor_novo = 0.0;

				if (valor < 1.0)
					valor_novo = 1.0 - valor;

				const int idVariavelDecisao_origem = getElementoVetor(a_idEstagio, idVariavelRealizacaoInterna, AttVetorVariavelRealizacaoInterna_idVariavelDecisao, a_TSS, int());

				//vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(idVariavelDecisao_origem, 0.0);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(idVariavelDecisao_origem, valor_novo);

				if (idVariavelRealizacaoInterna == maiorIdVariavelRealizacaoInterna)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->atualizar();

			} // if (idVariavelAleatoriaInterna > IdVariavelAleatoriaInterna_Nenhum) {
			 
		} // for (IdVariavelRealizacaoInterna idVariavelRealizacaoInterna = IdVariavelRealizacaoInterna_1; idVariavelRealizacaoInterna <= maiorIdVariavelRealizacaoInterna; idVariavelRealizacaoInterna++) {

		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->otimizar();

		if (vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_nao_otimalidade) {
			//otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, 1, a_idEstagio);
			otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, a_idEstagio);
		}

		if (vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

			if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao == IdRealizacao_Nenhum)) {

				VARIAVEL_DECISAO_1(ARMAZENAR_VALOR)
				VARIAVEL_DECISAO_2(ARMAZENAR_VALOR)
					VARIAVEL_DECISAO_3(ARMAZENAR_VALOR)
					VARIAVEL_DECISAO_4(ARMAZENAR_VALOR)
					VARIAVEL_DECISAO_5(ARMAZENAR_VALOR)
					VARIAVEL_DECISAO_6(ARMAZENAR_VALOR)

			} // if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao == IdRealizacao_Nenhum)) {

			else if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario_por_realizacao, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao != IdRealizacao_Nenhum)) {

				VARIAVEL_DECISAO_1(ARMAZENAR_VALOR)
				VARIAVEL_DECISAO_2(ARMAZENAR_VALOR)
					VARIAVEL_DECISAO_3(ARMAZENAR_VALOR)
					VARIAVEL_DECISAO_4(ARMAZENAR_VALOR)
					VARIAVEL_DECISAO_5(ARMAZENAR_VALOR)
					VARIAVEL_DECISAO_6(ARMAZENAR_VALOR)

			} // else if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario_por_realizacao, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao != IdRealizacao_Nenhum)) {

			if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario_por_realizacao, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao == IdRealizacao_Nenhum)) {

					EQUACAO_LINEAR_1(ARMAZENAR_VALOR)
					EQUACAO_LINEAR_2(ARMAZENAR_VALOR)
					EQUACAO_LINEAR_3(ARMAZENAR_VALOR)
					EQUACAO_LINEAR_4(ARMAZENAR_VALOR)
					EQUACAO_LINEAR_5(ARMAZENAR_VALOR)
					EQUACAO_LINEAR_6(ARMAZENAR_VALOR)

					INEQUACAO_LINEAR_3(ARMAZENAR_VALOR)
					INEQUACAO_LINEAR_4(ARMAZENAR_VALOR)
					INEQUACAO_LINEAR_5(ARMAZENAR_VALOR)
					INEQUACAO_LINEAR_6(ARMAZENAR_VALOR)
					INEQUACAO_LINEAR_7(ARMAZENAR_VALOR)

			} // if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario_por_realizacao, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao == IdRealizacao_Nenhum)) {

			else if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario_por_realizacao, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao != IdRealizacao_Nenhum)) {

					EQUACAO_LINEAR_1(ARMAZENAR_VALOR)
					EQUACAO_LINEAR_2(ARMAZENAR_VALOR)
					EQUACAO_LINEAR_3(ARMAZENAR_VALOR)
					EQUACAO_LINEAR_4(ARMAZENAR_VALOR)
					EQUACAO_LINEAR_5(ARMAZENAR_VALOR)
					EQUACAO_LINEAR_6(ARMAZENAR_VALOR)

					INEQUACAO_LINEAR_3(ARMAZENAR_VALOR)
					INEQUACAO_LINEAR_4(ARMAZENAR_VALOR)
					INEQUACAO_LINEAR_5(ARMAZENAR_VALOR)
					INEQUACAO_LINEAR_6(ARMAZENAR_VALOR)
					INEQUACAO_LINEAR_7(ARMAZENAR_VALOR)

			} // else if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario_por_realizacao, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao != IdRealizacao_Nenhum)) {

			if ((getAtributo(AttComumModeloOtimizacao_imprimir_solver, bool())) && (getAtributo(AttComumModeloOtimizacao_imprimir_solver_viabilidade_hidraulica, bool()))) {
				criarDiretorio(a_diretorio + "//" + getString(a_TSS));
				if (a_idRealizacao == IdRealizacao_Nenhum)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirLP(a_diretorio + "//" + getString(a_TSS) + "//PL_" + getString(a_idEstagio) + "_" + getString(a_idCenario));
				else
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirLP(a_diretorio + "//" + getString(a_TSS) + "//PL_" + getString(a_idEstagio) + "_" + getString(a_idCenario) + "_" + getString(a_idRealizacao));
			} // if ((getAtributo(AttComumModeloOtimizacao_imprimir_solver, bool())) && (getAtributo(AttComumModeloOtimizacao_imprimir_solver_viabilidade_hidraulica, bool()))) {

			for (IdVariavelRealizacaoInterna idVariavelRealizacaoInterna = IdVariavelRealizacaoInterna_1; idVariavelRealizacaoInterna <= maiorIdVariavelRealizacaoInterna; idVariavelRealizacaoInterna++) {

				const IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = getAtributo(a_idEstagio, idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_idVariavelAleatoriaInterna, IdVariavelAleatoriaInterna());

				if (idVariavelAleatoriaInterna > IdVariavelAleatoriaInterna_Nenhum) {

					const int idVariavelDecisao_origem = getElementoVetor(a_idEstagio, idVariavelRealizacaoInterna, AttVetorVariavelRealizacaoInterna_idVariavelDecisao, a_TSS, int());

					double valor = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getValorPrimal(idVariavelDecisao_origem);

					if (valor > 0.0) {

						const int idVariavelDecisao = getElementoVetor(a_idEstagio, idVariavelRealizacaoInterna, AttVetorVariavelRealizacaoInterna_idVariavelDecisao, a_TSS_destino, int());

						vetorEstagio.at(a_idEstagio).getSolver(a_TSS_destino)->setLimInferior(idVariavelDecisao, valor + 1.0);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS_destino)->setLimSuperior(idVariavelDecisao, valor + 1.0);

					} // if (valor > 0.0) {

				} // if (idVariavelAleatoriaInterna > IdVariavelAleatoriaInterna_Nenhum) {

				if (idVariavelRealizacaoInterna == maiorIdVariavelRealizacaoInterna)
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS_destino)->atualizar();

			} // for (IdVariavelRealizacaoInterna idVariavelRealizacaoInterna = IdVariavelRealizacaoInterna_1; idVariavelRealizacaoInterna <= maiorIdVariavelRealizacaoInterna; idVariavelRealizacaoInterna++) {

			return true;

		} // if (vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

		const std::string codigo = getString(int(time(0)));
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + codigo);
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + codigo);
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + codigo);
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
						novo_valor_sup = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getInfinito();

					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(idVariavelDecisao, novo_valor_inf);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(idVariavelDecisao, novo_valor_sup);

				}

				// Truncamento
				else if ((tipo_relaxacao == TipoRelaxacaoVariavelAleatoria_truncamento) || (tipo_relaxacao == TipoRelaxacaoVariavelAleatoria_truncamento_penalizacao)) {

					if (getAtributo(AttComumModeloOtimizacao_relaxar_afluencia_incremental_com_viabilidade_hidraulica, bool()) && (a_idRealizacao == IdRealizacao_Nenhum)) {
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(idVariavelDecisao, novo_valor_inf);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(idVariavelDecisao, novo_valor_sup);
					}

					else {

						double realizacao = 0.0;

						// forward
						if (a_idRealizacao == IdRealizacao_Nenhum)
							realizacao = vetorProcessoEstocastico.at(idProcessoEstocastico).getElementoMatriz(idVariavelAleatoria, AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral, periodo, a_idCenario, double());

						// backward
						else if (a_idRealizacao > IdRealizacao_Nenhum)
							realizacao = vetorProcessoEstocastico.at(idProcessoEstocastico).calcularRealizacao(idVariavelAleatoria, a_idCenario, a_idRealizacao, periodo);

						const double valor_minimo_convexo = vetorProcessoEstocastico.at(idProcessoEstocastico).calcularRealizacaoParaValor(idVariavelAleatoria, 0.0, periodo);

						if (realizacao < valor_minimo_convexo) {
							const double realizacao_flex = valor_minimo_convexo - realizacao;
							novo_valor_sup = valor_viabilidade + realizacao_flex;
							if (tipo_relaxacao == TipoRelaxacaoVariavelAleatoria_truncamento)
								novo_valor_inf = novo_valor_sup;
						}

						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimInferior(idVariavelDecisao, novo_valor_inf);
						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setLimSuperior(idVariavelDecisao, novo_valor_sup);

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
			vetorEstagio.at(a_idEstagio).resetarValorVariavelRealizacaoInterna(a_TSS, idVariavelRealizacaoInterna);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::resetarVariavelRealizacaoInterna(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::resetarVariavelRealizacaoInterna(){


void ModeloOtimizacao::atualizarVariavelEstadoComSolucaoPrimal(const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario){

	try {

		const TipoSubproblemaSolver tSS = TipoSubproblemaSolver_geral;

		const IdEstagio idEstagio_anterior = IdEstagio(a_idEstagio - 1);

		const IdVariavelEstado maiorIdVariavelEstado = getMaiorId(a_idEstagio, IdVariavelEstado());

		for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

			if (vetorEstagio.at(a_idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

				try {

					const int idVariavelDecisaoEstagioAnterior = getAtributo(a_idEstagio, idVariavelEstado, AttComumVariavelEstado_idVariavelDecisaoEstagioAnterior, int());

					if (idVariavelDecisaoEstagioAnterior > 0) {

						double solucao_primal = vetorEstagio.at(idEstagio_anterior).getSolver(tSS)->getValorPrimal(idVariavelDecisaoEstagioAnterior);

						if (solucao_primal > vetorEstagio.at(idEstagio_anterior).getSolver(tSS)->getLimSuperior(idVariavelDecisaoEstagioAnterior))
							solucao_primal = vetorEstagio.at(idEstagio_anterior).getSolver(tSS)->getLimSuperior(idVariavelDecisaoEstagioAnterior);

						else if (solucao_primal < vetorEstagio.at(idEstagio_anterior).getSolver(tSS)->getLimInferior(idVariavelDecisaoEstagioAnterior))
							solucao_primal = vetorEstagio.at(idEstagio_anterior).getSolver(tSS)->getLimInferior(idVariavelDecisaoEstagioAnterior);

						addValorVariavelEstado(a_idEstagio, idVariavelEstado, a_idCenario, solucao_primal);

					} // if (idVariavelDecisaoEstagioAnterior > 0) {

				} // try
				catch (const std::exception& erro) {

					const std::string erro_str = std::string(erro.what());

					if (erro_str.size() > 5) {
						if (erro_str.find("10005") != std::string::npos) {

							escreverLog10005(tSS, a_idIteracao, a_idEstagio, a_idCenario, IdRealizacao_Nenhum, erro_str);

							for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

								if (vetorEstagio.at(a_idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

									const int idVariavelDecisaoEstagioAnterior = getAtributo(a_idEstagio, idVariavelEstado, AttComumVariavelEstado_idVariavelDecisaoEstagioAnterior, int());

									if (idVariavelDecisaoEstagioAnterior > 0) {

										const double limite_superior = vetorEstagio.at(idEstagio_anterior).getSolver(tSS)->getLimSuperior(idVariavelDecisaoEstagioAnterior);
										const double limite_inferior = vetorEstagio.at(idEstagio_anterior).getSolver(tSS)->getLimInferior(idVariavelDecisaoEstagioAnterior);

										const double valor = (limite_superior - limite_inferior) / 2.0;

										addValorVariavelEstado(a_idEstagio, idVariavelEstado, a_idCenario, valor);

									} // if (idVariavelDecisaoEstagioAnterior > 0) {

								} // if (vetorEstagio.at(a_idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {
							} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

						} // if (erro_str.find("10005") != std::string::npos) {					
					} // if (erro_str.size() > 5) {

					return;

				} // catch (const std::exception & erro) { 

			} // if (vetorEstagio.at(a_idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

		} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::atualizarVariavelEstadoComSolucaoPrimal(" + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::atualizarVariavelEstadoComSolucao(const IdEstagio a_idEstagio, const IdCenario a_idCenario){

void ModeloOtimizacao::addValorVariavelEstado(const IdEstagio a_idEstagio, const IdVariavelEstado idVariavelEstado, const IdCenario a_idCenario, const double valor){
	try {

		vetorEstagio.at(a_idEstagio).addValorVariavelEstado(idVariavelEstado, a_idCenario, IdCenario_Nenhum, IdCenario_Nenhum, valor);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::addValorVariavelEstado(" + getFullString(a_idEstagio) + "," + getFullString(idVariavelEstado) + "," + getFullString(a_idCenario) + "," + ",double): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::addValorVariavelEstado(const IdEstagio a_idEstagio, const IdVariavelEstado idVariavelEstado, const IdProcesso a_idProcesso, const IdProcesso a_maior_processo, const IdCenario a_idCenario, const double valor){


void ModeloOtimizacao::alocarVariaveisEstado(const IdIteracao a_idIteracao, const IdEstagio a_idEstagio) {

	try {

		IdCenario menor_cenario_estado = IdCenario_Nenhum;
		IdCenario maior_cenario_estado = IdCenario_Nenhum;

		const IdProcesso idProcesso_local = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());
		const std::vector<IdCenario> lista_cenario_estado = arranjoResolucao.getIdsCenarioEstado(idProcesso_local, a_idIteracao, a_idEstagio);
		if (lista_cenario_estado.size() > 0) {
			menor_cenario_estado = lista_cenario_estado.at(0);
			maior_cenario_estado = lista_cenario_estado.at(lista_cenario_estado.size() - 1);
		}

		if (menor_cenario_estado == IdCenario_Nenhum)
			return;

		vetorEstagio.at(a_idEstagio).alocarVariaveisEstado(menor_cenario_estado, maior_cenario_estado);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::alocarVariaveisEstado(" + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "," + ",double): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::imprimirEstagios(EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
		const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

		for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++)
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(idEstagio) + "_estagio.csv", vetorEstagio.at(idEstagio));

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::imprimirCorteBenders(a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::imprimirEstagios(EntradaSaidaDados a_entradaSaidaDados){


void ModeloOtimizacao::imprimirSolver(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const std::string a_nome_arquivo) {

	try {

		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirLP(a_nome_arquivo);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::imprimirSolver(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + a_nome_arquivo + "): \n" + std::string(erro.what())); }


}


void ModeloOtimizacao::imprimirVariaveisEstado(EntradaSaidaDados a_entradaSaidaDados) {

	const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
	const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

	for (IdEstagio idEstagio = estagio_inicial; idEstagio <= getMaiorId(IdEstagio()); idEstagio++) {
		if (getMaiorId(idEstagio, IdVariavelEstado()) > IdVariavelEstado_Nenhum) {
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(idEstagio) + "_estado.csv", IdVariavelEstado_Nenhum, vetorEstagio.at(idEstagio));
			if (idEstagio <= estagio_final)
				a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(idEstagio) + "_estado_variaveis.csv", IdVariavelEstado_Nenhum, vetorEstagio.at(idEstagio), AttVetorVariavelEstado_idVariavelDecisao);
		}
	}

} // void ModeloOtimizacao::imprimirVariaveisEstado(EntradaSaidaDados a_entradaSaidaDados){


void ModeloOtimizacao::imprimirVariaveisRealizacao(EntradaSaidaDados a_entradaSaidaDados) {

	const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
	const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

	for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {
		if (getMaiorId(idEstagio, IdVariavelRealizacao()) > IdVariavelRealizacao_Nenhum) {
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(idEstagio) + "_variavel_realizacao.csv", IdVariavelRealizacao_Nenhum, vetorEstagio.at(idEstagio));
			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(idEstagio) + "_variavel_realizacao_variaveis.csv", IdVariavelRealizacao_Nenhum, vetorEstagio.at(idEstagio), AttVetorVariavelRealizacao_idVariavelDecisao);
		}
	}

} // void ModeloOtimizacao::imprimirVariaveisRealizacao(EntradaSaidaDados a_entradaSaidaDados){

void ModeloOtimizacao::imprimirRestricoesCenario(EntradaSaidaDados a_entradaSaidaDados) {

	const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
	const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

	for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {
		if (getMaiorId(idEstagio, IdRestricaoCenario()) > IdRestricaoCenario_Nenhum) {
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(idEstagio) + "_restricao_realizacao.csv", IdRestricaoCenario_Nenhum, vetorEstagio.at(idEstagio));
			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(idEstagio) + "_restricao_realizacao_equacoes.csv", IdRestricaoCenario_Nenhum, vetorEstagio.at(idEstagio), AttVetorRestricaoCenario_idRestricao);
			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(idEstagio) + "_restricao_realizacao_rhs.csv", IdRestricaoCenario_Nenhum, vetorEstagio.at(idEstagio), AttVetorRestricaoCenario_rhs);
			a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz(getFullString(idEstagio) + "_restricao_realizacao_coeficiente.csv", IdRestricaoCenario_Nenhum, vetorEstagio.at(idEstagio), AttMatrizRestricaoCenario_coeficiente);
		}
	}

} // void ModeloOtimizacao::imprimirVariaveisRealizacao(EntradaSaidaDados a_entradaSaidaDados){

void ModeloOtimizacao::imprimirVariaveisRealizacaoInterna(EntradaSaidaDados a_entradaSaidaDados) {

	const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
	const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

	for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {
		if (getMaiorId(idEstagio, IdVariavelRealizacaoInterna()) > IdVariavelRealizacaoInterna_Nenhum) {
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(idEstagio) + "_variavel_realizacao_interna.csv", IdVariavelRealizacaoInterna_Nenhum, vetorEstagio.at(idEstagio));
			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(idEstagio) + "_variavel_realizacao_interna_variaveis.csv", IdVariavelRealizacaoInterna_Nenhum, vetorEstagio.at(idEstagio), AttVetorVariavelRealizacaoInterna_idVariavelDecisao);
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
			a_entradaSaidaDados.setDiretorioSaida(diretorio_saida + "//Cortes//" + getFullString(a_idProcesso) + "//");

		a_entradaSaidaDados.setAppendArquivo(false);

		if (vetorEstagio.at(a_estagio).vetorCorteBenders.getMaiorId() != IdCorteBenders_Nenhum) {
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(a_estagio) + "_estagio.csv", vetorEstagio.at(a_estagio), std::vector<AttComumEstagio>{ AttComumEstagio_idEstagio, AttComumEstagio_periodo_otimizacao, AttComumEstagio_selecao_cortes_nivel_dominancia, AttComumEstagio_cortes_multiplos, AttComumEstagio_alpha_CVAR, AttComumEstagio_lambda_CVAR});
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(a_estagio) + "_estado.csv", IdVariavelEstado_Nenhum, vetorEstagio.at(a_estagio));
			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_estagio) + "_corteBenders_rhs.csv", IdCorteBenders_Nenhum, vetorEstagio.at(a_estagio), AttVetorCorteBenders_rhs);
			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_estagio) + "_corteBenders_estado.csv", IdCorteBenders_Nenhum, vetorEstagio.at(a_estagio), AttVetorCorteBenders_estado);
			a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz(getFullString(a_estagio) + "_corteBenders_coeficientes.csv", IdCorteBenders_Nenhum, vetorEstagio.at(a_estagio), AttMatrizCorteBenders_coeficiente);
		} // if (vetorEstagio.at(a_estagio).vetorCorteBenders.getMaiorId() != IdCorteBenders_Nenhum) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::exportarCorteBenders(" + getFullString(a_idProcesso) + "," + getFullString(a_estagio) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::exportarCorteBenders(const IdEstagio a_estagio_inicial, const IdEstagio a_estagio_final, EntradaSaidaDados a_entradaSaidaDados){

void ModeloOtimizacao::excluirArquivosCorteBenders(const IdProcesso a_idProcesso, const IdEstagio a_estagio, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		std::string diretorio_saida = a_entradaSaidaDados.getDiretorioSaida();

		if (a_idProcesso == IdProcesso_mestre)
			diretorio_saida += "//Cortes//";
		else
			diretorio_saida += "//Cortes//" + getFullString(a_idProcesso);

		deletarArquivo(diretorio_saida + getFullString(a_estagio) + "_estagio.csv");
		deletarArquivo(diretorio_saida + getFullString(a_estagio) + "_estado.csv");
		deletarArquivo(diretorio_saida + getFullString(a_estagio) + "_corteBenders_rhs.csv");
		deletarArquivo(diretorio_saida + getFullString(a_estagio) + "_corteBenders_estado.csv");
		deletarArquivo(diretorio_saida + getFullString(a_estagio) + "_corteBenders_coeficientes.csv");

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::excluirArquivosCorteBenders(" + getFullString(a_idProcesso) + "," + getFullString(a_estagio) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::exportarCorteBenders(const IdEstagio a_estagio_inicial, const IdEstagio a_estagio_final, EntradaSaidaDados a_entradaSaidaDados){

void ModeloOtimizacao::exportarVersaoAlternativaCorteBenders(const IdProcesso a_idProcesso, const IdEstagio a_estagio, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const IdCorteBenders maiorIdCorteBenders = vetorEstagio.at(a_estagio).vetorCorteBenders.getMaiorId();

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

		vetorEstagio.at(a_estagio).exportarVersaoAlternativaCorteBenders(estagio, idProcessoEstocasticoNovo_str, idProcessoEstocasticoNovo_compacto_str, lista_nome_idVariavelAleatoriaNova, lista_nome_compacto_idVariavelAleatoriaNova, lista_participacao_idVariavelAleatoriaNova);

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
			if (vetorEstagio.at(a_estagio).vetorCorteBenders.isInstanciado(idCorteBenders))
				vetorEstagio.at(a_estagio).vetorCorteBenders.rem(idCorteBenders);
		} // for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= getMaiorId(a_estagio, IdCorteBenders()); idCorteBenders++) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::removerCorteBenders(" + getFullString(a_estagio) + "): \n" + std::string(erro.what())); }


} // void ModeloOtimizacao::removerCorteBenders(const IdEstagio a_estagio){


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

		bool dif_stages = false;

		a_entradaSaidaDados.setDiretorioEntrada(diretorio_importacao_cortes);
		for (IdEstagio idStage_glob = IdEstagio(1); idStage_glob < IdEstagio_Excedente; idStage_glob++) {

			Estagio estagio_aux;
			if (a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir(getFullString(idStage_glob) + "_estagio.csv", estagio_aux, TipoAcessoInstancia_direto)) {

				Periodo perStage_aux = Periodo("m", estagio_aux.getAtributo(AttComumEstagio_periodo_otimizacao, Periodo()));

				for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

					if (vetorEstagio.isInstanciado(idEstagio)) {

						Periodo perStage = getAtributo(idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

						if (perStage_aux == Periodo("m", perStage)) {

							if (arranjoResolucao.isAnyCenarioEstado(idEstagio) || arranjoResolucao.isAnyAberturas(idEstagio)) {
								if ((estagio_inicial != IdEstagio_1) || (idEstagio > estagio_inicial)) {

									if (idEstagio != estagio_aux.getAtributo(AttComumEstagio_idEstagio, IdEstagio()))
										dif_stages = true;

									vetorEstagio.at(idEstagio).vetorCorteBenders.alocar(int(maximoIdCorteBenders));
									Estagio estagio_load;
									a_entradaSaidaDados.carregarArquivoCSV_AttComum(getFullString(idStage_glob) + "_estagio.csv", estagio_load, TipoAcessoInstancia_direto);
									estagio_load.setAtributo(AttComumEstagio_idEstagio, idEstagio);
									vetorEstagio_aux.add(estagio_load);
									a_entradaSaidaDados.carregarArquivoCSV_AttVetor(getFullString(idStage_glob) + "_corteBenders_rhs.csv", vetorEstagio_aux.at(idEstagio), TipoAcessoInstancia_m1);
									if (getAtributo(idEstagio, AttComumEstagio_selecao_cortes_nivel_dominancia, int()) > 0)
										a_entradaSaidaDados.carregarArquivoCSV_AttVetor(getFullString(idStage_glob) + "_corteBenders_estado.csv", vetorEstagio_aux.at(idEstagio), TipoAcessoInstancia_m1);
									a_entradaSaidaDados.carregarArquivoCSV_AttMatriz(getFullString(idStage_glob) + "_corteBenders_coeficientes.csv", vetorEstagio_aux.at(idEstagio), TipoAcessoInstancia_m1);
									a_entradaSaidaDados.carregarArquivoCSV_AttComum(getFullString(idStage_glob) + "_estado.csv", vetorEstagio_aux.at(idEstagio), TipoAcessoInstancia_m1);
									break;
								} // if ((estagio_inicial != IdEstagio_1) || (idEstagio > estagio_inicial)) {
							}
						}

						else if (perStage > perStage_aux)
							break;

					}
				}
			}
		} // for (IdEstagio idStage_glob = IdEstagio(1); idStage_glob < IdEstagio_Excedente; idStage_glob++) {


		const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> horizon = a_dados.getMatriz(AttMatrizDados_percentual_duracao_patamar_carga, Periodo(), IdPatamarCarga(), double());

		for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

			if (vetorEstagio_aux.isInstanciado(idEstagio)) {

				if (arranjoResolucao.isAnyCenarioEstado(idEstagio) || arranjoResolucao.isAnyAberturas(idEstagio)) {

					if (a_dados.getAtributo(AttComumDados_iteracao_maxima_cortes_externos, IdIteracao()) != IdIteracao_Nenhum) {
						vetorEstagio.at(idEstagio).anyCorteExterno = true;
						vetorEstagio.at(idEstagio).anyVarEstadoExterna = true;
					}

					if (vetorEstagio_aux.at(idEstagio).vetorCorteBenders.getMaiorId() > IdCorteBenders_Nenhum) {

						SmartEnupla<IdVariavelAleatoria, std::vector<IdHidreletrica>> isIdHidreletricaNosEstadosYP(IdVariavelAleatoria_1, std::vector<std::vector<IdHidreletrica>>(IdVariavelAleatoria(IdVariavelAleatoria_Excedente - 1), std::vector<IdHidreletrica>()));

						if ((estagio_inicial != IdEstagio_1) || (idEstagio > estagio_inicial)) {

							if (getAtributo(idEstagio, AttComumEstagio_periodo_otimizacao, Periodo()).sobreposicao(vetorEstagio_aux.at(idEstagio).getAtributo(AttComumEstagio_periodo_otimizacao, Periodo())) == 0.0)
								throw std::invalid_argument("Periodo " + getString(vetorEstagio_aux.at(idEstagio).getAtributo(AttComumEstagio_periodo_otimizacao, Periodo())) + " importado nao compativel com periodo do estagio " + getFullString(idEstagio));

							const IdCorteBenders maiorIdCorteBenders = vetorEstagio_aux.at(idEstagio).getMaiorId(IdCorteBenders());

							const IdVariavelEstado maiorIdVarivelEstado = getMaiorId(idEstagio, IdVariavelEstado());

							IdRealizacao maiorIdRealizacao_corte = IdRealizacao_Nenhum;

							IdVariavelEstado maiorIdVarivelEstado_corte = IdVariavelEstado_Nenhum;
							for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {
								if (vetorEstagio_aux.at(idEstagio).vetorCorteBenders.isInstanciado(idCorteBenders)) {
									const IdRealizacao idRealizacao = vetorEstagio_aux.at(idEstagio).getIterador1Final(idCorteBenders, AttMatrizCorteBenders_coeficiente, IdRealizacao());
									if (idRealizacao > maiorIdRealizacao_corte)
										maiorIdRealizacao_corte = idRealizacao;
									if (maiorIdVarivelEstado_corte == IdVariavelEstado_Nenhum)
										maiorIdVarivelEstado_corte = vetorEstagio_aux.at(idEstagio).getIterador2Final(idCorteBenders, AttMatrizCorteBenders_coeficiente, idRealizacao, IdVariavelEstado());
								}
							} // for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {

							if (maiorIdVarivelEstado_corte == IdVariavelEstado_Nenhum)
								throw std::invalid_argument("Nenhum corte foi importado em " + getFullString(idEstagio));

							if (dif_stages) {
								for (IdVariavelEstado idVariavelEstado_corte = IdVariavelEstado_1; idVariavelEstado_corte <= maiorIdVarivelEstado_corte; idVariavelEstado_corte++) {
									std::string nome_varCorte = vetorEstagio_aux.at(idEstagio).vetorVariavelEstado.at(idVariavelEstado_corte).getAtributo(AttComumVariavelEstado_nome, std::string());
									const size_t pos_1 = nome_varCorte.find_first_of(",");
									const size_t pos_2 = nome_varCorte.find_first_of(",", pos_1 + 1);
									nome_varCorte.replace(pos_1 + 1, pos_2 - pos_1 - 1, getString(idEstagio));
									vetorEstagio_aux.at(idEstagio).vetorVariavelEstado.at(idVariavelEstado_corte).setAtributo(AttComumVariavelEstado_nome, nome_varCorte);
								}
							}

							const int cortes_multiplos_modelo = getAtributo(idEstagio, AttComumEstagio_cortes_multiplos, int());

							if ((cortes_multiplos_modelo == 0) && (maiorIdRealizacao_corte != IdRealizacao_1))
								throw std::invalid_argument("A multiplicidade dos cortes importados nao eh compativel com a multiplicidade do modelo em " + getFullString(idEstagio));

							else if ((cortes_multiplos_modelo > 0) && (maiorIdRealizacao_corte != getAtributo(idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao())))
								throw std::invalid_argument("A multiplicidade dos cortes importados nao eh compativel com o numero de aberturas do modelo em " + getFullString(idEstagio));

							int numero_variaveis_estado_modelo_encontradas = 0;
							int numero_variaveis_estado_cortes_encontradas = 0;
							SmartEnupla<IdVariavelEstado, IdVariavelEstado> variaveis_estado_modelo_encontradas(IdVariavelEstado_1, std::vector<IdVariavelEstado>(maiorIdVarivelEstado, IdVariavelEstado_Nenhum));
							SmartEnupla<IdVariavelEstado, IdVariavelEstado> variaveis_estado_cortes_encontradas(IdVariavelEstado_1, std::vector<IdVariavelEstado>(maiorIdVarivelEstado_corte, IdVariavelEstado_Nenhum));

							bool modelo_e_cortes_estados_identicos = true;

							if (maiorIdVarivelEstado != maiorIdVarivelEstado)
								modelo_e_cortes_estados_identicos = false;

							for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVarivelEstado; idVariavelEstado++) {

								if (vetorEstagio.at(idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

									const std::string nome_estado = vetorEstagio.at(idEstagio).getAtributo(idVariavelEstado, AttComumVariavelEstado_nome, std::string());

									for (IdVariavelEstado idVariavelEstado_corte = IdVariavelEstado_1; idVariavelEstado_corte <= maiorIdVarivelEstado_corte; idVariavelEstado_corte++) {

										if (variaveis_estado_cortes_encontradas.at(idVariavelEstado_corte) == IdVariavelEstado_Nenhum) {

											if (vetorEstagio_aux.at(idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado_corte)) {

												if (nome_estado == vetorEstagio_aux.at(idEstagio).vetorVariavelEstado.at(idVariavelEstado_corte).getAtributo(AttComumVariavelEstado_nome, std::string())) {
													variaveis_estado_modelo_encontradas.at(idVariavelEstado) = idVariavelEstado_corte;
													variaveis_estado_cortes_encontradas.at(idVariavelEstado_corte) = idVariavelEstado;
													numero_variaveis_estado_modelo_encontradas++;
													numero_variaveis_estado_cortes_encontradas++;
													break;
												} // if (nome_estados.size() == nome_estados_aux.size()) {
											}
										}

									} // for (IdVariavelEstado idVariavelEstado_corte = IdVariavelEstado_1; idVariavelEstado_corte <= maiorIdVarivelEstado_corte; idVariavelEstado_corte++) {

									if (variaveis_estado_modelo_encontradas.at(idVariavelEstado) == IdVariavelEstado_Nenhum)
										modelo_e_cortes_estados_identicos = false;

								}
							} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVarivelEstado; idVariavelEstado++) {

							//
							// Tratamento de variáveis do modelo que não foram encontradas no corte.
							// 

							if (numero_variaveis_estado_modelo_encontradas != int(maiorIdVarivelEstado)) {

								for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVarivelEstado; idVariavelEstado++) {

									if (vetorEstagio.at(idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

										if (variaveis_estado_modelo_encontradas.at(idVariavelEstado) == IdVariavelEstado_Nenhum) {

											vetorEstagio.at(idEstagio).anyCorteExterno = true;

										} // if (variaveis_estado_modelo_encontradas.at(idVariavelEstado) == IdVariavelEstado_Nenhum) {

									} // if (vetorEstagio.at(idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

								} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVarivelEstado; idVariavelEstado++) {

							} // if (numero_variaveis_estado_modelo_encontradas != int(maiorIdVarivelEstado)) {


							//
							// Tratamento de variáveis do corte que não foram encontradas no modelo.
							// 

							if (numero_variaveis_estado_cortes_encontradas != int(maiorIdVarivelEstado_corte)) {

								modelo_e_cortes_estados_identicos = false;

								for (IdVariavelEstado idVariavelEstado_corte = IdVariavelEstado_1; idVariavelEstado_corte <= maiorIdVarivelEstado_corte; idVariavelEstado_corte++) {

									if (variaveis_estado_cortes_encontradas.at(idVariavelEstado_corte) == IdVariavelEstado_Nenhum) {

										const std::vector<std::string> nome = vetorEstagio_aux.at(idEstagio).getNomeVariavelEstado(idVariavelEstado_corte);

										vetorEstagio.at(idEstagio).anyCorteExterno = true;
										vetorEstagio.at(idEstagio).anyVarEstadoExterna = true;

										//
										// Defluencia viajante
										//

										if (nome.at(0) == "QDEF") {
											try {
												Periodo periodo_lag = Periodo(nome.at(2));
												const IdHidreletrica idHidreletrica = getIdHidreletricaFromChar(nome.at(3).c_str());

												int varQDEF = criarVariaveisDecisao_VariaveisEstado_Restricoes_QDEF(a_TSS, a_dados, idEstagio, idHidreletrica, periodo_lag, horizon, true);

												const int varQDEF_ADD = getVarDecisao_QDEF_ADDseExistir(a_TSS, idEstagio, periodo_lag, idHidreletrica);
												if (varQDEF_ADD > -1)
													varQDEF = varQDEF_ADD;

												if (varQDEF == -1)
													throw std::invalid_argument("Nao foi possivel criar variaveis e restricoes QDEF de " + getFullString(idVariavelEstado_corte) + " em " + getFullString(idEstagio));

												int varQDEF_prev = -1;

												if (idEstagio > IdEstagio_1)
													varQDEF_prev = getVarDecisao_QDEF(a_TSS, IdEstagio(idEstagio - 1), periodo_lag, idHidreletrica);

												const IdVariavelEstado idVarEstadoNew = vetorEstagio.at(idEstagio).addVariavelEstado(a_TSS, vetorEstagio_aux.at(idEstagio).getAtributo(idVariavelEstado_corte, AttComumVariavelEstado_nome, std::string()), varQDEF, varQDEF_prev, true);

												variaveis_estado_modelo_encontradas.addElemento(idVarEstadoNew, idVariavelEstado_corte);
											} // try
											catch (const std::exception& erro) { throw std::invalid_argument("QDEF: \n" + std::string(erro.what())); }

										} // else if (nome.at(0) == "QDEF") {

										else if (nome.at(0) == "VI") {
											try {
												const Periodo periodo(nome.at(2));

												const IdHidreletrica idHidreletrica = getIdHidreletricaFromChar(nome.at(3).c_str());

												const double vol_util_max = getdoubleFromChar(nome.at(4).c_str());

												if (vol_util_max < 0.0)
													throw std::invalid_argument("Limites de vol invalidos em VI de " + getFullString(idHidreletrica) + " em " + getFullString(idVariavelEstado_corte) + " no corte em " + getFullString(idEstagio));

												else if (Periodo("m", periodo) != Periodo("m", vetorEstagio.at(idEstagio).getAtributo(AttComumEstagio_periodo_otimizacao, Periodo())))
													throw std::invalid_argument("Periodo " + getFullString(periodo) + " nao compativel com VI de " + getFullString(idHidreletrica) + " em " + getFullString(idVariavelEstado_corte) + " no corte em " + getFullString(idEstagio));

												else if (idEstagio == IdEstagio_1)
													throw std::invalid_argument("VI de " + getFullString(idHidreletrica) + " em " + getFullString(idVariavelEstado_corte) + " nao compativel com " + getFullString(idEstagio));

												Periodo periodIni = a_dados.getIterador2Inicial(AttMatrizDados_percentual_duracao_horizonte_estudo, idEstagio, Periodo());
												Periodo periodPrev = periodIni;
												horizon.decrementarIterador(periodPrev);

												const IdEstagio idEstagioPrev = IdEstagio(idEstagio - 1);

												int varVI = getVarDecisao_VIseExistir(a_TSS, idEstagio, periodIni, idHidreletrica);
												int varVF = getVarDecisao_VFseExistir(a_TSS, idEstagioPrev, periodPrev, idHidreletrica);
												if (varVI == -1) {
													if (getEquLinear_BH_VOLseExistir(a_TSS, idEstagio, periodIni, idHidreletrica) > -1)
														vetorEstagio.at(idEstagio).getSolver(a_TSS)->setCofRestricao(addVarDecisao_VI(a_TSS, idEstagio, periodIni, idHidreletrica, 0.0, vol_util_max, 0.0), getEquLinear_BH_VOL(a_TSS, idEstagio, periodIni, idHidreletrica), -1.0);
													else {
														for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= horizon.at_rIt(periodIni).getIteradorFinal(); idPat++) {
															if (getEquLinear_BH_VAZseExistir(a_TSS, idEstagio, periodIni, idPat, idHidreletrica) > -1) {
																if (varVI == -1)
																	varVI = addVarDecisao_VI(a_TSS, idEstagio, periodIni, idHidreletrica, 0.0, vol_util_max, 0.0);
																vetorEstagio.at(idEstagio).getSolver(a_TSS)->setCofRestricao(varVI, getEquLinear_BH_VAZ(a_TSS, idEstagio, periodIni, idPat, idHidreletrica), -(1.0 / a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodIni, idPat, double())));
															}
															else
																throw std::invalid_argument("Nao ha balanco hidraulico para VI de " + getFullString(idHidreletrica) + " em " + getFullString(idVariavelEstado_corte) + " e " + getFullString(idEstagio));
														}
													}
												}
												if (varVF == -1) {
													if (getEquLinear_BH_VOLseExistir(a_TSS, idEstagioPrev, periodPrev, idHidreletrica) > -1)
														vetorEstagio.at(idEstagioPrev).getSolver(a_TSS)->setCofRestricao(addVarDecisao_VF(a_TSS, idEstagioPrev, periodPrev, idHidreletrica, 0.0, vol_util_max, 0.0), getEquLinear_BH_VOL(a_TSS, idEstagioPrev, periodPrev, idHidreletrica), 1.0);
													else {
														for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= horizon.at_rIt(periodPrev).getIteradorFinal(); idPat++) {
															if (getEquLinear_BH_VAZseExistir(a_TSS, idEstagioPrev, periodPrev, idPat, idHidreletrica) > -1) {
																if (varVF == -1)
																	varVF = addVarDecisao_VF(a_TSS, idEstagioPrev, periodPrev, idHidreletrica, 0.0, vol_util_max, 0.0);
																vetorEstagio.at(idEstagioPrev).getSolver(a_TSS)->setCofRestricao(varVF, getEquLinear_BH_VAZ(a_TSS, idEstagioPrev, periodPrev, idPat, idHidreletrica), (1.0 / a_dados.getElementoMatriz(AttMatrizDados_conversor_vazao_volume, periodPrev, idPat, double())));
															}
															else
																throw std::invalid_argument("Nao ha balanco hidraulico para VF de " + getFullString(idHidreletrica) + " em " + getFullString(idVariavelEstado_corte) + " e " + getFullString(idEstagioPrev));
														}
													}
												}

												const IdVariavelEstado idVarEstadoNew = vetorEstagio.at(idEstagio).addVariavelEstado(a_TSS, vetorEstagio_aux.at(idEstagio).getAtributo(idVariavelEstado_corte, AttComumVariavelEstado_nome, std::string()), varVI, varVF, true);

												variaveis_estado_modelo_encontradas.addElemento(idVarEstadoNew, idVariavelEstado_corte);
											} // try
											catch (const std::exception& erro) { throw std::invalid_argument("VI: \n" + std::string(erro.what())); }
										} // if (nome.at(0) == "VI") {

										else if (nome.at(0) == "YP") {
											try {
												const IdProcessoEstocastico idProcessoEstocastico = getIdProcessoEstocasticoFromChar(nome.at(2).c_str());

												if (idProcessoEstocastico != getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico()))
													throw std::invalid_argument(getFullString(idProcessoEstocastico) + " do acoplamento incompativel com " + getFullString(a_dados.processoEstocastico_hidrologico.getAtributo(AttComumProcessoEstocastico_idProcessoEstocastico, IdProcessoEstocastico())) + " do modelo");

												const IdVariavelAleatoria idVariavelAleatoria = getIdVariavelAleatoriaFromChar(nome.at(3).c_str());

												Periodo periodo_lag = Periodo(nome.at(4));

												const double grau_liberdade = getdoubleFromChar(nome.at(5).c_str());

												if (nome.size() < 7)
													throw std::invalid_argument(getFullString(idVariavelEstado_corte) + " com termos ausentes de VarDecisaoYP.");

												std::vector<IdHidreletrica> listaHidreletrica;
												std::vector<IdHidreletrica> listaHidreletricaNaoInstanciadaNoModelo;
												for (int i = 6; i < nome.size(); i++) {
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

												if (listaHidreletricaNaoInstanciadaNoModelo.size() > 0)
													throw std::invalid_argument(getFullString(listaHidreletricaNaoInstanciadaNoModelo.at(0)) + " em " + getFullString(idVariavelEstado_corte) + " nao foi instanciado no modelo.");

												else if (listaHidreletricaNaoInstanciadaNoModelo.size() == 0) {
													const int varYP = criarVariaveisDecisao_VariaveisEstado_Restricoes_YP(a_TSS, a_dados, idEstagio, idProcessoEstocastico, idVariavelAleatoria, periodo_lag, grau_liberdade, listaHidreletrica, true);
													if (varYP == -1)
														throw std::invalid_argument("Nao foi possivel criar variaveis e restricoes YP de " + getFullString(idVariavelEstado_corte) + " em " + getFullString(idEstagio));

													int varYP_past = getVarDecisao_YP(a_TSS, IdEstagio(idEstagio - 1), periodo_lag, idProcessoEstocastico, idVariavelAleatoria);

													const IdVariavelEstado idVarEstadoNew = vetorEstagio.at(idEstagio).addVariavelEstado(a_TSS, vetorEstagio_aux.at(idEstagio).getAtributo(idVariavelEstado_corte, AttComumVariavelEstado_nome, std::string()), varYP, varYP_past, true);

													variaveis_estado_modelo_encontradas.addElemento(idVarEstadoNew, idVariavelEstado_corte);
												}

											} // try
											catch (const std::exception& erro) { throw std::invalid_argument("YP: \n" + std::string(erro.what())); }

										} // else if (nome.at(0) == "YP") {

										else if (nome.at(0) == "PTDISPCOM") {
											try {

												Periodo periodo_comando = Periodo(nome.at(2));
												const IdTermeletrica idTermeletrica = getIdTermeletricaFromChar(nome.at(3).c_str());

												const double potencia_minima_disponivel = getdoubleFromChar(nome.at(4).c_str());
												const double potencia_maxima_disponivel = getdoubleFromChar(nome.at(5).c_str());


												const int varPTDISPCOM = criarVariaveisDecisao_VariaveisEstado_Restricoes_PTDISPCOM(a_TSS, a_dados, idEstagio, periodo_comando, idTermeletrica, potencia_minima_disponivel, potencia_maxima_disponivel, true);
												if (varPTDISPCOM == -1)
													throw std::invalid_argument("Nao foi possivel criar variaveis e restricoes PTDISPCOM de " + getFullString(idVariavelEstado_corte) + " em " + getFullString(idEstagio));

												int varPTDISPCOM_past = getVarDecisao_PTDISPCOM(a_TSS, IdEstagio(idEstagio - 1), periodo_comando, idTermeletrica);

												const IdVariavelEstado idVarEstadoNew = vetorEstagio.at(idEstagio).addVariavelEstado(a_TSS, vetorEstagio_aux.at(idEstagio).getAtributo(idVariavelEstado_corte, AttComumVariavelEstado_nome, std::string()), varPTDISPCOM, varPTDISPCOM_past, true);

												variaveis_estado_modelo_encontradas.addElemento(idVarEstadoNew, idVariavelEstado_corte);

											} // try
											catch (const std::exception& erro) { throw std::invalid_argument("PTDISPCOM: \n" + std::string(erro.what())); }

										} // else if (nome.at(0) == "PTDISPCOM") {


										else if (nome.at(0) == "RH") {

											try {

												Periodo periodo = Periodo(nome.at(2));
												IdRestricaoOperativaUHE idRH;

												const int varRH = criarRestricoesHidraulicas(a_TSS, a_dados, idEstagio, idRH, nome, horizon);
												const int varRH_past = getVarDecisao_RH(a_TSS, IdEstagio(idEstagio - 1), periodo, idRH);

												Periodo periodAux = periodo;
												const IdVariavelEstado idVarEstadoNew = vetorEstagio.at(idEstagio).addVariavelEstado(a_TSS, criarRestricoesHidraulicas_nomeVarEstado(a_TSS, a_dados, idEstagio, periodo, periodAux, idRH, true), varRH, varRH_past, true);

												variaveis_estado_modelo_encontradas.addElemento(idVarEstadoNew, idVariavelEstado_corte);

											} // try
											catch (const std::exception& erro) { throw std::invalid_argument("RH: \n" + std::string(erro.what())); }

										} // else if (nome.at(0) == "RH") {

										else if (nome.at(0) == "RE") {

											try {

												Periodo periodo = Periodo(nome.at(2));
												IdRestricaoEletrica idRE;

												const int varRE = criarRestricoesEletricas(a_TSS, a_dados, idEstagio, idRE, nome, horizon);
												const int varRE_past = getVarDecisao_RE(a_TSS, IdEstagio(idEstagio - 1), periodo, idRE);

												Periodo periodAux = periodo;
												const IdVariavelEstado idVarEstadoNew = vetorEstagio.at(idEstagio).addVariavelEstado(a_TSS, criarRestricoesEletricas_nomeVarEstado(a_TSS, a_dados, idEstagio, periodo, periodAux, idRE, true), varRE, varRE_past, true);

												variaveis_estado_modelo_encontradas.addElemento(idVarEstadoNew, idVariavelEstado_corte);

											} // try
											catch (const std::exception& erro) { throw std::invalid_argument("RE: \n" + std::string(erro.what())); }

										} // else if (nome.at(0) == "RE") {


										else if (nome.at(0) == "HQ") {

											try {
												Periodo periodo = Periodo(nome.at(2));
												IdControleCotaVazao idHQ = IdControleCotaVazao_Nenhum;
												for (IdControleCotaVazao idHQ_ = a_dados.getMenorId(IdControleCotaVazao()); idHQ_ < a_dados.getIdOut(IdControleCotaVazao()); a_dados.incr(idHQ_)) {
													if (a_dados.getAtributo(idHQ_, AttComumControleCotaVazao_nome, std::string()) == nome.at(4)) {
														idHQ = idHQ_;
														break;
													}
												}

												const int varHQ = criarVariaveisDecisao_VariaveisEstado_Restricoes_HQ(a_TSS, a_dados, idEstagio, periodo, idHQ, horizon, true);

												const int varHQ_past = getVarDecisao_HQ(a_TSS, IdEstagio(idEstagio - 1), periodo, idHQ);

												const IdVariavelEstado idVarEstadoNew = vetorEstagio.at(idEstagio).addVariavelEstado(a_TSS, std::string(getNomeSolverVarDecisao_HQ(a_TSS, idEstagio, periodo, idHQ) + "," + nome.at(4)), varHQ, varHQ_past, true);

												//if (idVarEstadoNew <= variaveis_estado_modelo_encontradas.getIteradorFinal())
													//variaveis_estado_modelo_encontradas.setElemento(idVarEstadoNew, idVariavelEstado_corte);
												//else
													variaveis_estado_modelo_encontradas.addElemento(idVarEstadoNew, idVariavelEstado_corte);

											} // try
											catch (const std::exception& erro) { throw std::invalid_argument("HQ: \n" + std::string(erro.what())); }

										} // else if (nome.at(0) == "HQ") {

										else
											throw std::invalid_argument(getFullString(idVariavelEstado_corte) + " " + vetorEstagio_aux.at(idEstagio).getAtributo(idVariavelEstado_corte, AttComumVariavelEstado_nome, std::string()) + " presente no corte, nao consta no modelo em " + getFullString(idEstagio));

									} // if (variaveis_estado_cortes_encontradas.at(idVariavelEstado_corte) == IdVariavelEstado_Nenhum) {

								} // for (IdVariavelEstado idVariavelEstado_corte = IdVariavelEstado_1; idVariavelEstado_corte <= maiorIdVarivelEstado_corte; idVariavelEstado_corte++) {

							} // if (numero_variaveis_estado_cortes_encontradas != int(maiorIdVarivelEstado_corte)) {


							IdCorteBenders idCorteBenders_sequencial = IdCorteBenders_Nenhum;
							for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {
								if (vetorEstagio_aux.at(idEstagio).vetorCorteBenders.isInstanciado(idCorteBenders) && (idCorteBenders <= maximoIdCorteBenders)) {
									if (idCorteBenders_sequencial == IdCorteBenders_Nenhum)
										idCorteBenders_sequencial = IdCorteBenders_1;
									else
										idCorteBenders_sequencial++;

									if (modelo_e_cortes_estados_identicos) {
										CorteBenders corteBenders(vetorEstagio_aux.at(idEstagio).vetorCorteBenders.at(idCorteBenders));
										corteBenders.setAtributo(AttComumCorteBenders_idCorteBenders, idCorteBenders_sequencial);
										vetorEstagio.at(idEstagio).vetorCorteBenders.add(corteBenders);
									} // if (modelo_e_cortes_estados_identicos) {

									else if (!modelo_e_cortes_estados_identicos) {

										CorteBenders corteBenders;
										corteBenders.setAtributo(AttComumCorteBenders_idCorteBenders, idCorteBenders_sequencial);

										const IdRealizacao idRealizacao_inicial = vetorEstagio_aux.at(idEstagio).getIterador1Inicial(idCorteBenders, AttMatrizCorteBenders_coeficiente, IdRealizacao());
										const IdRealizacao idRealizacao_final = vetorEstagio_aux.at(idEstagio).getIterador1Final(idCorteBenders, AttMatrizCorteBenders_coeficiente, IdRealizacao());

										for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= variaveis_estado_modelo_encontradas.getIteradorFinal(); idVariavelEstado++) {

											if (variaveis_estado_modelo_encontradas.at(idVariavelEstado) == IdVariavelEstado_Nenhum) {
												if (vetorEstagio_aux.at(idEstagio).getSizeVetor(idCorteBenders, AttVetorCorteBenders_estado) > 0)
													corteBenders.addElemento(AttVetorCorteBenders_estado, idVariavelEstado, 0.0);

												for (IdRealizacao idRealizacao = idRealizacao_inicial; idRealizacao <= idRealizacao_final; idRealizacao++)
													corteBenders.addElemento(AttMatrizCorteBenders_coeficiente, idRealizacao, idVariavelEstado, 0.0);

											}

											else if (variaveis_estado_modelo_encontradas.at(idVariavelEstado) != IdVariavelEstado_Nenhum) {
												if (vetorEstagio_aux.at(idEstagio).getSizeVetor(idCorteBenders, AttVetorCorteBenders_estado) > 0)
													corteBenders.addElemento(AttVetorCorteBenders_estado, idVariavelEstado, vetorEstagio_aux.at(idEstagio).getElementoVetor(idCorteBenders, AttVetorCorteBenders_estado, variaveis_estado_modelo_encontradas.at(idVariavelEstado), double()));

												for (IdRealizacao idRealizacao = idRealizacao_inicial; idRealizacao <= idRealizacao_final; idRealizacao++)
													corteBenders.addElemento(AttMatrizCorteBenders_coeficiente, idRealizacao, idVariavelEstado, vetorEstagio_aux.at(idEstagio).getElementoMatriz(idCorteBenders, AttMatrizCorteBenders_coeficiente, idRealizacao, variaveis_estado_modelo_encontradas.at(idVariavelEstado), double()));

											} // if (variaveis_estado_modelo_encontradas.at(idVariavelEstado) != IdVariavelEstado_Nenhum) {

										} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= variaveis_estado_modelo_encontradas.getIteradorFinal(); idVariavelEstado++) {

										for (IdRealizacao idRealizacao = idRealizacao_inicial; idRealizacao <= idRealizacao_final; idRealizacao++)
											corteBenders.addElemento(AttVetorCorteBenders_rhs, idRealizacao, vetorEstagio_aux.at(idEstagio).getElementoVetor(idCorteBenders, AttVetorCorteBenders_rhs, idRealizacao, double()));

										if (vetorEstagio.at(idEstagio).anyCorteExterno)
											corteBenders.setAtributo(AttComumCorteBenders_is_externo, true);

										vetorEstagio.at(idEstagio).vetorCorteBenders.add(corteBenders);

									} // else if (!modelo_e_cortes_estados_identicos) {
								}
							} // for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {

							requestCorteBenders(a_idProcesso, idEstagio, a_diretorio_impressao_selecao_cortes, a_entradaSaidaDados);

							if (a_dados.getAtributo(AttComumDados_tipo_estudo, TipoEstudo()) == TipoEstudo_simulacao)
								removerCorteBenders(idEstagio);

						} // 

					} // if (vetorEstagio.at(idEstagio).vetorCorteBenders.getMaiorId() > IdCorteBenders_Nenhum){

					else
						std::cout << "Cortes de Benders nao detectados para : " << getFullString(idEstagio) << std::endl;

				}

			}
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

		const Periodo perStageEnd = getAtributo(estagio_final, AttComumEstagio_periodo_otimizacao, Periodo());
		const Periodo perStageNextMin = Periodo("m", perStageEnd + 1);

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

					if (!estagio_carregado) {
						estagio_carregado = a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir(prefixo + "estagio_" + getFullString(a_idIteracao) + ".csv", estagio_auxiliar, TipoAcessoInstancia_direto);
						if (estagio_carregado) {
							if (perStageNextMin != Periodo("m", estagio_auxiliar.getAtributo(AttComumEstagio_periodo_otimizacao, Periodo())))
								estagio_carregado = false;
						}
					}

					if (estagio_carregado) {

						rhs_carregado = a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir(prefixo + "corteBenders_rhs_" + getFullString(a_idIteracao) + ".csv", vetorEstagio.at(idEstagio_futuro), TipoAcessoInstancia_m1);
						std_carregado = a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir(prefixo + "corteBenders_estado_" + getFullString(a_idIteracao) + ".csv", vetorEstagio.at(idEstagio_futuro), TipoAcessoInstancia_m1);
						cof_carregado = a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir(prefixo + "corteBenders_coeficientes_" + getFullString(a_idIteracao) + ".csv", vetorEstagio.at(idEstagio_futuro), TipoAcessoInstancia_m1);

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
							if (perStageNextMin != Periodo("m", estagio_auxiliar.getAtributo(AttComumEstagio_periodo_otimizacao, Periodo())))
								estagio_carregado_sem_iterador = false;
						}

						if (estagio_carregado_sem_iterador) {

							a_entradaSaidaDados.carregarArquivoCSV_AttVetor(prefixo + "corteBenders_rhs.csv", vetorEstagio.at(idEstagio_futuro), TipoAcessoInstancia_m1);
							a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir(prefixo + "corteBenders_estado.csv", vetorEstagio.at(idEstagio_futuro), TipoAcessoInstancia_m1);
							a_entradaSaidaDados.carregarArquivoCSV_AttMatriz(prefixo + "corteBenders_coeficientes.csv", vetorEstagio.at(idEstagio_futuro), TipoAcessoInstancia_m1);

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
			if (vetorEstagio.at(idEstagio_futuro).vetorCorteBenders.isInstanciado(idCorteBenders)) {
				const IdRealizacao idRealizacao = vetorEstagio.at(idEstagio_futuro).getIterador1Final(idCorteBenders, AttMatrizCorteBenders_coeficiente, IdRealizacao());
				if (idRealizacao > maiorIdRealizacao_corte)
					maiorIdRealizacao_corte = idRealizacao;
			}
		} // for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {

		if (maiorIdRealizacao_corte == IdRealizacao_Nenhum)
			throw std::invalid_argument("Nenhum corte foi importado.");

		vetorEstagio.at(idEstagio_futuro).setAtributo(AttComumEstagio_maiorIdRealizacao, maiorIdRealizacao_corte);

		if (maximoIdCorteBenders < IdCorteBenders(IdCorteBenders_Excedente - 1)) {
			for (IdCorteBenders idCorteBenders = IdCorteBenders(maximoIdCorteBenders + 1); idCorteBenders <= getMaiorId(idEstagio_futuro, IdCorteBenders()); idCorteBenders++)
				vetorEstagio.at(idEstagio_futuro).vetorCorteBenders.rem(idCorteBenders);
		} // if (maximoIdCorteBenders < IdCorteBenders(IdCorteBenders_Excedente - 1)) {

		for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= getMaiorId(idEstagio_futuro, IdCorteBenders()); idCorteBenders++) {
			if (vetorEstagio.at(idEstagio_futuro).vetorCorteBenders.isInstanciado(idCorteBenders))
				vetorEstagio.at(idEstagio_futuro).avaliarDominanciaCorteBendersParaEstado(idCorteBenders, idCorteBenders);
		}

		const Periodo periodo_otimizacao_estagio_final = getAtributo(estagio_final, AttComumEstagio_periodo_otimizacao, Periodo());

		if (getVarDecisao_ZFseExistir(a_TSS, estagio_final) == -1) {

			// Variável ZF
			addVarDecisao_ZF(a_TSS, estagio_final, 0.0, vetorEstagio.at(estagio_final).getSolver(a_TSS)->getInfinito(), 0.0);

			// Variável ZF em EquLinear_ZT
			vetorEstagio.at(estagio_final).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZF(a_TSS, estagio_final), getEquLinear_ZT(a_TSS, estagio_final), -1.0);

			//criarRestricoesCorteBendersEmCustoFuturo(a_TSS, estagio_final);

			const int posEquZF = addEquLinear_ZF(a_TSS, estagio_final);

			vetorEstagio.at(estagio_final).getSolver(a_TSS)->setRHSRestricao(posEquZF, 0.0);

			// Variável ZF
			vetorEstagio.at(estagio_final).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZF(a_TSS, estagio_final), posEquZF, 1.0);

			for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao_corte; idRealizacao++) {
				int varZF = getVarDecisao_ZFseExistir(a_TSS, estagio_final, idRealizacao);
				if (varZF == -1)
					varZF = addVarDecisao_ZF(a_TSS, estagio_final, idRealizacao, 0.0, vetorEstagio.at(estagio_final).getSolver(a_TSS)->getInfinito(), 0.0);
				vetorEstagio.at(estagio_final).getSolver(a_TSS)->setCofRestricao(varZF, posEquZF, -(1.0 / double(maiorIdRealizacao_corte)));
			}

		} // if (getVarDecisao_ZFseExistir(estagio_final) == -1) {
		 
		const int numero_cortes = vetorEstagio.at(idEstagio_futuro).vetorCorteBenders.numObjetos();

		// Todos os cortes são instanciados de maneira estática (maior performance)
		if ((getAtributo(idEstagio_futuro, AttComumEstagio_selecao_cortes_nivel_dominancia, int()) == 0) || (!std_carregado)) {

			const int numero_cortes_total = numero_cortes * int(maiorIdRealizacao_corte);
			int indice_corte = -1;

			if (true) {
				std::vector<std::string> nomes(numero_cortes_total, "");

				int cont = 0;
				for (IdRealizacao idRealizacao = maiorIdRealizacao_corte; idRealizacao >= IdRealizacao_1; idRealizacao--) {
					for (IdCorteBenders idCorteBenders = getMaiorId(idEstagio_futuro, IdCorteBenders()); idCorteBenders >= getMenorId(idEstagio_futuro, IdCorteBenders()); idCorteBenders--) {
						if (vetorEstagio.at(idEstagio_futuro).vetorCorteBenders.isInstanciado(idCorteBenders)) {
							nomes.at(cont) = getNomeSolverIneLinear_CB_ZF(a_TSS, estagio_final, idRealizacao, idCorteBenders);
							cont++;
						}
					}
				}
				indice_corte = vetorEstagio.at(estagio_final).getSolver(a_TSS)->addConstrsMaior(nomes) - numero_cortes_total + 1;
			} // if (true) {

			for (IdRealizacao idRealizacao = maiorIdRealizacao_corte; idRealizacao >= IdRealizacao_1; idRealizacao--) {

				const int varZF = getVarDecisao_ZF(a_TSS, estagio_final, idRealizacao);

				for (IdCorteBenders idCorteBenders = getMaiorId(idEstagio_futuro, IdCorteBenders()); idCorteBenders >= getMenorId(idEstagio_futuro, IdCorteBenders()); idCorteBenders--) {

					if (vetorEstagio.at(idEstagio_futuro).vetorCorteBenders.isInstanciado(idCorteBenders)) {

						if (getIneLinear_CB_ZFseExistir(a_TSS, estagio_final, idRealizacao, idCorteBenders) > -1)
							throw std::invalid_argument("Conteudo ja existente " +  getFullString(idCorteBenders));

						alocIneLinear_CB_ZF_3_1(a_TSS, estagio_final);
						name_IneLinear_CB_ZF_3_1.at(a_TSS).at(estagio_final).push_back(getNomePrintIneLinear_CB_ZF(a_TSS, estagio_final, idRealizacao, idCorteBenders));
						indx_IneLinear_CB_ZF_3_1.at(a_TSS).at(estagio_final).push_back(indice_corte);

						vetorEstagio.at(estagio_final).getSolver(a_TSS)->setCofRestricao(varZF, indice_corte, 1.0);

						for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= getMaiorId(idEstagio_futuro, IdVariavelEstado()); idVariavelEstado++) {

							if (vetorEstagio.at(idEstagio_futuro).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

								const int idVariavelDecisaoEstagioAnterior = getAtributo(idEstagio_futuro, idVariavelEstado, AttComumVariavelEstado_idVariavelDecisaoEstagioAnterior, int());

								if (idVariavelDecisaoEstagioAnterior > -1)
									vetorEstagio.at(estagio_final).getSolver(a_TSS)->setCofRestricao(idVariavelDecisaoEstagioAnterior, indice_corte, -getElementoMatriz(idEstagio_futuro, idCorteBenders, AttMatrizCorteBenders_coeficiente, idRealizacao, idVariavelEstado, double()));
								else if (getSizeVetor(idEstagio_futuro, idCorteBenders, AttVetorCorteBenders_estado) > 0)
									vetorEstagio.at(idEstagio_futuro).anularVariavelEstadoCorteBenders(idVariavelEstado, idCorteBenders);

							} // if (vetorEstagio.at(estagio_final).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

							else if (getSizeVetor(idEstagio_futuro, idCorteBenders, AttVetorCorteBenders_estado) > 0)
								vetorEstagio.at(idEstagio_futuro).anularVariavelEstadoCorteBenders(idVariavelEstado, idCorteBenders);

						} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= getMaiorId(idEstagio_futuro, IdVariavelEstado()); idVariavelEstado++) {

						vetorEstagio.at(estagio_final).getSolver(a_TSS)->setRHSRestricao(indice_corte, getElementoVetor(idEstagio_futuro, idCorteBenders, AttVetorCorteBenders_rhs, idRealizacao, double()));

						indice_corte++;

					} // if (vetorEstagio.at(idEstagio_futuro).vetorCorteBenders.isInstanciado(idCorteBenders)) {

				} // for (IdCorteBenders idCorteBenders = getMaiorId(idEstagio_futuro, IdCorteBenders()); idCorteBenders <= getMenorId(idEstagio_futuro, IdCorteBenders()); idCorteBenders--) {
			} // for (IdRealizacao idRealizacao = maiorIdRealizacao_corte; idRealizacao <= IdRealizacao_1; idRealizacao--) {

		} // if (getAtributo(idEstagio_futuro, AttComumEstagio_selecao_cortes_nivel_dominancia, int()) == 0) {
		else
			requestCorteBenders(a_idProcesso, idEstagio_futuro, a_diretorio_impressao_selecao_cortes, a_entradaSaidaDados);

		vetorEstagio.at(idEstagio_futuro).removerTodosCorteBenders();

		if (a_idProcesso == IdProcesso_mestre)
			std::cout << "Cortes de Benders pos-estudo carregados. " << std::endl;

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::importarCorteBenders_AcoplamentoPosEstudo(" + getFullString(a_TSS) + "," + a_diretorio_impressao_selecao_cortes + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::importarCorteBenders_AcoplamentoPosEstudo(const std::string a_diretorio_recuperacao_cortes, EntradaSaidaDados a_entradaSaidaDados){


void ModeloOtimizacao::importarVariaveisEstado_AcoplamentoPosEstudo(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdProcesso a_idProcesso, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const string diretorio_importacao_pos_estudo = getAtributo(AttComumModeloOtimizacao_diretorio_importacao_pos_estudo, std::string());

		if (strCompara(diretorio_importacao_pos_estudo, "nenhum"))
			return;

		const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
		const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

		const Periodo perStageEnd = getAtributo(estagio_final, AttComumEstagio_periodo_otimizacao, Periodo());
		const Periodo perStageNextMin = Periodo("m", perStageEnd + 1);

		a_entradaSaidaDados.setDiretorioEntrada(diretorio_importacao_pos_estudo);

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

					if (estagio_carregado_sem_iterador) {
						if (perStageNextMin != Periodo("m", estagio.getAtributo(AttComumEstagio_periodo_otimizacao, Periodo())))
							estagio_carregado_sem_iterador = false;
					}

					if (!estagio_carregado_sem_iterador) {
						estagio_carregado = a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir(prefixo + "estagio_" + getFullString(arranjoResolucao.getAtributo(AttComumArranjoResolucao_iteracao_inicial, IdIteracao())) + ".csv", estagio, TipoAcessoInstancia_direto);
						if (estagio_carregado) {
							if (perStageNextMin != Periodo("m", estagio.getAtributo(AttComumEstagio_periodo_otimizacao, Periodo())))
								estagio_carregado = false;
						}
					}

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

				if (Periodo(getString(periodo_otimizacao_lido.getDuration()), periodo_otimizacao + 1) != periodo_otimizacao_lido)
					throw std::invalid_argument("Periodo " + getString(periodo_otimizacao_lido) + " importado nao compativel com final do horizonte de otimizacao.");

			} // if (true) {

			if (estagio_carregado_sem_iterador)
				a_entradaSaidaDados.carregarArquivoCSV_AttComum(prefixo + "estado.csv", estagio, TipoAcessoInstancia_m1);
			else
				a_entradaSaidaDados.carregarArquivoCSV_AttComum(prefixo + "estado_" + getFullString(arranjoResolucao.getAtributo(AttComumArranjoResolucao_iteracao_inicial, IdIteracao())) + ".csv", estagio, TipoAcessoInstancia_m1);


			idEstagio_futuro = IdEstagio(estagio_final + 1);
			const IdEstagio idEstagio = estagio_final;

			estagio.setAtributo(AttComumEstagio_idEstagio, idEstagio_futuro);

			const Periodo periodo_estudo_inicial = getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_estudo, idEstagio, Periodo());
			const Periodo periodo_estudo_final = getIterador2Final(AttMatrizModeloOtimizacao_horizonte_estudo, idEstagio, Periodo());

			const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

			const Periodo periodo_horizonte_estudo_inicial = getElementosMatriz(AttMatrizModeloOtimizacao_horizonte_estudo, getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio()), Periodo(), double()).getIteradorInicial();

			SmartEnupla<IdVariavelAleatoria, std::vector<IdHidreletrica>> isIdHidreletricaNosEstadosYP(IdVariavelAleatoria_1, std::vector<std::vector<IdHidreletrica>>(IdVariavelAleatoria(IdVariavelAleatoria_Excedente - 1), std::vector<IdHidreletrica>()));

			bool is_conversao_ENA_carregado = false;

			const IdCenario cenarioInicial = IdCenario_1;
			const IdCenario cenarioFinal = arranjoResolucao.getAtributo(AttComumArranjoResolucao_maior_cenario, IdCenario());
			const SmartEnupla<IdCenario, double>enupla_inicializacao(cenarioInicial, std::vector<double>(int(cenarioFinal - cenarioInicial) + 1, 0.0));

			const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> horizon = a_dados.getMatriz(AttMatrizDados_percentual_duracao_patamar_carga, Periodo(), IdPatamarCarga(), double());

			// Validar se estados possuem variáveis de decisão no modelo e ajustar cortes se necessário.
			for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= estagio.getMaiorId(IdVariavelEstado()); idVariavelEstado++) {

				const std::vector<std::string> nome = estagio.getNomeVariavelEstado(idVariavelEstado);

				//
				// Volume Minimo
				//
				
				if (nome.at(0) == "ZP0_VH_LINF") {
				
					Periodo periodo_penalizacao(nome.at(2));

					const int varZP0_VH_LINF = criarVariaveisDecisao_VariaveisEstado_Restricoes_ZP0_VH_LINF(a_TSS, a_dados, idEstagio, periodo_penalizacao);

					if (varZP0_VH_LINF > -1)
						estagio.setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado, a_TSS, varZP0_VH_LINF);
				
					else{
						estagio.anularVariavelEstadoCorteBenders(idVariavelEstado);
						estagio.vetorVariavelEstado.rem(idVariavelEstado);
						if (a_idProcesso == IdProcesso_mestre)
							std::cout << "ZP0_VH_LINF " << getFullString(idVariavelEstado) << " nao criada por falta de restricao de volume util minimo no modelo." << std::endl;
					}				

				} // if (nome.at(0) == "ZP0_VH_LINF") {
				

				//
				// Volume Inicial
				//

				else if (nome.at(0) == "VI") {

					const Periodo periodo(nome.at(2));

					const IdHidreletrica idHidreletrica = getIdHidreletricaFromChar(nome.at(3).c_str());

					if (Periodo(getString(periodo.getDuration()), periodo_estudo_final + 1) != periodo)
						throw std::invalid_argument("Periodo estudo invalido " + getFullString(periodo_estudo_final) + " para o acoplamento de varVI de " + getFullString(idVariavelEstado) + " em " + getFullString(periodo));

					estagio.setVariavelDecisaoEmVariavelEstado(idVariavelEstado, a_TSS, -1);

					const int varVF = getVarDecisao_VFseExistir(a_TSS, idEstagio, periodo_estudo_final, idHidreletrica);

					if (varVF > -1)
						estagio.setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado, a_TSS, varVF);

					else if (varVF == -1) {

						const int varVF_nova = addVarDecisao_VF(a_TSS, idEstagio, periodo_estudo_final, idHidreletrica, 0.0, 0.0, 0.0);

						estagio.setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado, a_TSS, varVF_nova);

					} // else if (varVF == -1)

				} // if (nome.at(0) == "VI") {

				//
				// Afluencia Processo Estocastico
				//

				else if (nome.at(0) == "YP") {

					//Periodo periodo = Periodo(nome.at(2));

					Periodo periodo_lag = Periodo(nome.at(2));

					const IdProcessoEstocastico idProcessoEstocastico = getIdProcessoEstocasticoFromChar(nome.at(3).c_str());

					if (idProcessoEstocastico != getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico()))
						throw std::invalid_argument(getFullString(idProcessoEstocastico) + " do acoplamento incompativel com " + getFullString(a_dados.processoEstocastico_hidrologico.getAtributo(AttComumProcessoEstocastico_idProcessoEstocastico, IdProcessoEstocastico())) + " do modelo");

					const IdVariavelAleatoria idVariavelAleatoria = getIdVariavelAleatoriaFromChar(nome.at(4).c_str());

					const double grau_liberdade = getdoubleFromChar(nome.at(5).c_str());

					if (nome.size() < 7)
						throw std::invalid_argument(getFullString(idVariavelEstado) + " com termos ausentes de YP.");

					std::vector<IdHidreletrica> listaHidreletrica;
					std::vector<IdHidreletrica> listaHidreletricaNaoInstanciadaNoModelo;
					for (int i = 6; i < nome.size(); i++) {
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
						const int varYP = criarVariaveisDecisao_VariaveisEstado_Restricoes_YP(a_TSS, a_dados, idEstagio, idProcessoEstocastico, idVariavelAleatoria, periodo_lag, grau_liberdade, listaHidreletrica);
						if (varYP == -1)
							throw std::invalid_argument("Nao foi possivel criar variaveis e restricoes YP de " + getFullString(idVariavelEstado) + " em " + getFullString(idEstagio));
						estagio.setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado, a_TSS, varYP);
					}
				} // else if (nome.at(0) == "YP") {

				//
				// Energia Natural Afluente Acoplamento
				//

				else if (nome.at(0) == "ENA") {

					try {

						const Periodo periodo_lag = Periodo(nome.at(2));

						const IdReservatorioEquivalente idREE = getIdReservatorioEquivalenteFromChar(nome.at(3).c_str());

						int varENA_REE = getVarDecisao_ENAseExistir(a_TSS, estagio_final, periodo_lag, idREE);

						if (varENA_REE == -1) {

							//Instancia a variável VarDecisaoENA = 0.0 -> faz parte da lógica da atualização dinâmica no método de solução
							const double limite_inferior = 0.0;
							const double limite_superior = 0.0;

							varENA_REE = addVarDecisao_ENA(a_TSS, estagio_final, periodo_lag, idREE, limite_inferior, limite_superior, 0.0);

						}


						estagio.setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado, a_TSS, varENA_REE);

					}
					catch (const std::exception& erro) { throw std::invalid_argument("ENA " + getFullString(idVariavelEstado) + " : \n" + std::string(erro.what())); }

				} // else if (nome.at(0) == "ENA") {


				//
				// Defluencia viajante
				//

				else if (nome.at(0) == "QDEF") {

					Periodo periodo_lag = Periodo(nome.at(2));
					const IdHidreletrica idHidreletrica = getIdHidreletricaFromChar(nome.at(3).c_str());

					const int varQDEF = criarVariaveisDecisao_VariaveisEstado_Restricoes_QDEF(a_TSS, a_dados, idEstagio, idHidreletrica, periodo_lag, horizon);
					if (varQDEF == -1)
						throw std::invalid_argument("Nao foi possivel criar variaveis e restricoes QDEF de " + getFullString(idVariavelEstado) + " em " + getFullString(idEstagio));
					else
						estagio.setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado, a_TSS, varQDEF);

				} // else if (nome.at(0) == "QDEF") {


				else if (nome.at(0) == "RH") {

					IdRestricaoOperativaUHE idHQ;

					const int varRH = criarRestricoesHidraulicas(a_TSS, a_dados, idEstagio, idHQ, nome, horizon);

					estagio.setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado, a_TSS, varRH);

				} // else if (nome.at(0) == "RH") {

				else if (nome.at(0) == "HQ") {

					Periodo periodo = Periodo(nome.at(2));
					IdControleCotaVazao idHQ = IdControleCotaVazao_Nenhum;
					for (IdControleCotaVazao idHQ_ = a_dados.getMenorId(IdControleCotaVazao()); idHQ_ < a_dados.getIdOut(IdControleCotaVazao()); a_dados.incr(idHQ_)) {
						if (a_dados.getAtributo(idHQ_, AttComumControleCotaVazao_nome, std::string()) == nome.at(4)) {
							idHQ = idHQ_;
							break;
						}
					}

					const int varHQ = criarVariaveisDecisao_VariaveisEstado_Restricoes_HQ(a_TSS, a_dados, idEstagio, periodo, idHQ, horizon, true);
					estagio.setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado, a_TSS, varHQ);

				} // else if (nome.at(0) == "HQ") {

				else if (nome.at(0) == "PTDISPCOM") {

					Periodo periodo_comando = Periodo(nome.at(2));
					const IdTermeletrica idTermeletrica = getIdTermeletricaFromChar(nome.at(3).c_str());

					const double potencia_minima_disponivel = getdoubleFromChar(nome.at(4).c_str());
					const double potencia_maxima_disponivel = getdoubleFromChar(nome.at(5).c_str());


					const int varPTDISPCOM = criarVariaveisDecisao_VariaveisEstado_Restricoes_PTDISPCOM(a_TSS, a_dados, idEstagio, periodo_comando, idTermeletrica, potencia_minima_disponivel, potencia_maxima_disponivel);
					if (varPTDISPCOM == -1)
						throw std::invalid_argument("Nao foi possivel criar variaveis e restricoes PTDISPCOM de " + getFullString(idVariavelEstado) + " em " + getFullString(idEstagio));
					else
						estagio.setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado, a_TSS, varPTDISPCOM);

				} // else if (nome.at(0) == "PTDISPCOM") {

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
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum("estagio.csv", vetorEstagio.at(idEstagio), std::vector<AttComumEstagio>{AttComumEstagio_idEstagio, AttComumEstagio_periodo_otimizacao});
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum("estado.csv", IdVariavelEstado_Nenhum, vetorEstagio.at(idEstagio));
		} // if (a_idIteracao == getAtributo(AttComumModeloOtimizacao_iteracao_inicial,  IdIteracao())) {

		a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("estado_valores_" + getFullString(a_idIteracao) + ".csv", IdVariavelEstado_Nenhum, vetorEstagio.at(idEstagio), AttVetorVariavelEstado_valor);

		if (a_idIteracao == arranjoResolucao.getAtributo(AttComumArranjoResolucao_iteracao_final, IdIteracao()))
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum("eop.txt", vetorEstagio.at(getMaiorId(IdEstagio())), std::vector<AttComumEstagio>{AttComumEstagio_idEstagio, AttComumEstagio_periodo_otimizacao});

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::exportarVariaveisEstado_AcoplamentoPosEstudo(a_entradaSaidaDados): \n" + std::string(erro.what())); }
} // void ModeloOtimizacao::exportarEstadosAcoplamentoPreEstudo(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados) {

bool ModeloOtimizacao::isVariavelEstadoInstanciada(const IdEstagio a_idEstagio, const IdVariavelEstado a_idVariavelEstado){
	try{

		return vetorEstagio.at(a_idEstagio).vetorVariavelEstado.isInstanciado(a_idVariavelEstado);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::isVariavelEstadoInstanciada(" + getFullString(a_idEstagio) + "," + getFullString(a_idVariavelEstado) + "): \n" + std::string(erro.what())); }
}

VARIAVEL_DECISAO_1(DECLARAR_IMPRIMIR_VALOR)
VARIAVEL_DECISAO_2(DECLARAR_IMPRIMIR_VALOR)
VARIAVEL_DECISAO_3(DECLARAR_IMPRIMIR_VALOR)
VARIAVEL_DECISAO_4(DECLARAR_IMPRIMIR_VALOR)
VARIAVEL_DECISAO_5(DECLARAR_IMPRIMIR_VALOR)
VARIAVEL_DECISAO_6(DECLARAR_IMPRIMIR_VALOR)

EQUACAO_LINEAR_1(DECLARAR_IMPRIMIR_VALOR)
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




void ModeloOtimizacao::imprimirSolucaoPorEstagio(const IdProcesso a_idProcesso, const IdEstagio a_idEstagio, const std::string a_subdiretorio, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		if (!getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, bool()) && !getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario_por_realizacao, bool()) &&
			!getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario, bool()) && !getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario_por_realizacao, bool()))
			return;

		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida() + "//" + a_subdiretorio;

		for (TipoSubproblemaSolver TSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); TSS < TipoSubproblemaSolver_Excedente; TSS++) {

			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//" + getString(TSS));

				if ((TSS == TipoSubproblemaSolver_viabilidade_hidraulica) && (!getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()))) {}
				else {


					VARIAVEL_DECISAO_1(IMPRIMIR_VALOR)
					VARIAVEL_DECISAO_2(IMPRIMIR_VALOR)
						VARIAVEL_DECISAO_3(IMPRIMIR_VALOR)
						VARIAVEL_DECISAO_4(IMPRIMIR_VALOR)
						VARIAVEL_DECISAO_5(IMPRIMIR_VALOR)
						VARIAVEL_DECISAO_6(IMPRIMIR_VALOR)

				}

				EQUACAO_LINEAR_1(IMPRIMIR_VALOR)
				EQUACAO_LINEAR_2(IMPRIMIR_VALOR)
					EQUACAO_LINEAR_3(IMPRIMIR_VALOR)
					EQUACAO_LINEAR_4(IMPRIMIR_VALOR)
					EQUACAO_LINEAR_5(IMPRIMIR_VALOR)
					EQUACAO_LINEAR_6(IMPRIMIR_VALOR)

					INEQUACAO_LINEAR_3(IMPRIMIR_VALOR)
					INEQUACAO_LINEAR_4(IMPRIMIR_VALOR)
					INEQUACAO_LINEAR_5(IMPRIMIR_VALOR)
					INEQUACAO_LINEAR_6(IMPRIMIR_VALOR)
					INEQUACAO_LINEAR_7(IMPRIMIR_VALOR)
		}

	} // try {

	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::imprimirSolucaoPorEstagio(" + getString(a_idProcesso) + "," + a_subdiretorio + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::imprimirSolucao(EntradaSaidaDados a_entradaSaidaDados){


bool ModeloOtimizacao::otimizarProblemaComTratamentoNumericoInviabilidade(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio) {
	try {
		const int level = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->otimizarComTratamentoInviabilidade();
		setElemento(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, level);
		return (level > 0);
	}
	catch (const std::exception& erro) {
		throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::otimizaProblemaComTratamentoNumericoInviabilidade(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what()));
	}
}

bool ModeloOtimizacao::otimizarProblemaComTratamentoNumericoInviabilidade(const TipoSubproblemaSolver a_TSS, const int a_level, const IdEstagio a_idEstagio) {

	try {

		if (vetorEstagio.at(a_idEstagio).getSolver(a_TSS) == nullptr)
			throw std::invalid_argument("Solver invalido (NULO).");

		try {
			if (a_level == 1) {

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->resetar();
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->otimizar();

			} // if (a_level == 1) {

			else {

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setMetodo(TipoMetodoSolver_primal_simplex);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->resetar();
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->otimizar();
			}

		} // try
		catch (const std::exception& erro) {

			const std::string erro_str = std::string(erro.what());

			if (erro_str.size() > 5) {
				if (erro_str.find("10005") != std::string::npos) {
					setElemento(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, -1);
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setMetodoPadrao();
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->resetar();
					return false;
				}
			}
		}

		setElemento(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, a_level);

		if (vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {
			if (a_level == 0) {
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setMetodoPadrao();
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->resetar();
			}
			return true;
		}

		const int calculoIIS = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->calcularIIS();

		if (calculoIIS == 1) {
			if (a_level == 0) {
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setMetodoPadrao();
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->resetar();
			}
			return false;
		}
		else {
			if (a_level > 0)
				return otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, a_level - 1, a_idEstagio);
			else if (a_level == 0) {
				setElemento(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, -1);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setMetodoPadrao();
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->resetar();
				return false;
			}
		}

		return false;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::otimizaProblemaComTratamentoNumericoInviabilidade(" + getFullString(a_TSS) + "," + getFullString(a_level) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::otimizaProblemaComTratamentoNumericoInviabilidade(Solver* a_ptrSolver){

void ModeloOtimizacao::setTempoLimiteOtimizacao(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const double a_tempo_limite){

	try {

		if (!vetorEstagio.at(a_idEstagio).isSolverInstanciado(a_TSS))
			return;

		if (a_tempo_limite <= 0.0)
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setTempoLimite();
		else
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setTempoLimite(a_tempo_limite);

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::setTempoLimiteOtimizacao(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_tempo_limite) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::setTempoLimiteOtimizacao(const IdEstagio a_idEstagio, const double a_tempo_limite){

double ModeloOtimizacao::getTempoOtimizacao(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio) {

	try {

		if (!vetorEstagio.at(a_idEstagio).isSolverInstanciado(a_TSS))
			return 0.0;

		return vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getTempoOtimizacao();

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getTempoOtimizacao(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::getTempoOtimizacao(const IdEstagio a_idEstagio){


double ModeloOtimizacao::otimizarProblema(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const std::string a_diretorio) {

	try {

		setElemento(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, 2);

		bool atualizacaoCompletaVariavelRealizacaoInterna = false;
		IdCenario a_idCenario = IdCenario_1;
		resetarVariavelRealizacaoInterna(a_TSS, a_idEstagio);

		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->otimizar();

		if (vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() != TipoStatusSolver_nao_otimalidade)
			return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_diretorio);

		//
		// Tratamento Numérico de Inviabilidade
		//

		//otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, 1, a_idEstagio);
		otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, a_idEstagio);

		if (vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() != TipoStatusSolver_nao_otimalidade)
			return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_diretorio);

		const std::string codigo = getString(int(time(0)));
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		throw std::invalid_argument("Erro em otimizar, codigo identificador do problema linear " + codigo);

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::otimizarProblema(" + getFullString(a_TSS) + "," + getFullString(a_idIteracao) + "," + getString(a_idEstagio) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::otimizarSubproblema(const IdEstagio a_idEstagio, const IdCenario a_idCenario){



double ModeloOtimizacao::otimizarProblema(const TipoSubproblemaSolver a_TSS, const IdProcesso a_idProcesso, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const std::string a_diretorio){

	try {

		setElemento(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, 2);

		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->setMetodoPadrao();

		resetarVariavelRealizacaoInterna(a_TSS, a_idEstagio);

		if ((getElementoVetor(AttVetorModeloOtimizacao_alguma_variavel_aleatoria_hidrologica_com_truncamento, a_idEstagio, int()) == 1) || (getAtributo(AttComumModeloOtimizacao_relaxar_afluencia_incremental_com_viabilidade_hidraulica, bool())))
			atualizarModeloOtimizacaoComVariavelRealizacaoInterna(a_TSS, a_idIteracao, a_idProcesso, a_idEstagio, a_idCenario, IdRealizacao_Nenhum);

		if (getAtributo(AttComumModeloOtimizacao_relaxar_afluencia_incremental_com_viabilidade_hidraulica, bool()))
			atualizarModeloOtimizacaoComVariavelRealizacaoInterna(a_TSS, TipoSubproblemaSolver_viabilidade_hidraulica, a_idIteracao, a_idProcesso, a_idEstagio, a_idCenario, IdRealizacao_Nenhum, a_diretorio);

		try{
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->otimizar();
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

						if (getVarDecisao_ZIseExistir(a_TSS, a_idEstagio) > -1)
							setElemento(AttVetorModeloOtimizacao_custo_imediato, a_idEstagio, 0.0);

						if (getVarDecisao_ZFseExistir(a_TSS, a_idEstagio) > -1)
							setElemento(AttVetorModeloOtimizacao_custo_futuro, a_idEstagio, 0.0);

						vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->resetar();

						return 0.0;

					} // else if (getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int()) == 0) {

				} // if (erro_str.find("10005") != std::string::npos) {					
			} // if (erro_str.size() > 5) {

		} // catch (const std::exception & erro) { 

		if (vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade)
			return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_diretorio);

		//
		// Tratamento Numérico de Inviabilidade
		//

		otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, a_idEstagio);

		if (vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade)
			return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_diretorio);

		const std::string codigo = getString(int(time(0)));
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
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
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->otimizar();
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

		if (vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() != TipoStatusSolver_nao_otimalidade)
			return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao, solucao_proxy, a_sol_inf_var_dinamica, a_solucao_dual_var_dinamica, a_limite_inferior_var_dinamica, a_limite_superior_var_dinamica, a_sol_dual_var_estado, a_diretorio);

		//
		// Tratamento Numérico de Inviabilidade
		//

		//otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, 1, a_idEstagio);
		otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, a_idEstagio);

		if (vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() != TipoStatusSolver_nao_otimalidade)
			return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao, solucao_proxy, a_sol_inf_var_dinamica, a_solucao_dual_var_dinamica, a_limite_inferior_var_dinamica, a_limite_superior_var_dinamica, a_sol_dual_var_estado, a_diretorio);

		const std::string codigo = getString(int(time(0)));
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
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
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
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

		if (vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

			try {

				armazenarValoresSolver(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, IdRealizacao_Nenhum);

				if (getAtributo(AttComumModeloOtimizacao_imprimir_solver, bool())) {
					criarDiretorio(a_diretorio + "//" + getString(a_TSS));
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirLP(a_diretorio + "//" + getString(a_TSS) + "//PL_" + getString(a_idEstagio) + "_" + getString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())));
				}

				const double valorObjetivo = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getValorObjetivo();

				setElemento(AttVetorModeloOtimizacao_custo_total, a_idEstagio, valorObjetivo);

				if (getVarDecisao_ZIseExistir(a_TSS, a_idEstagio) > -1)
					setElemento(AttVetorModeloOtimizacao_custo_imediato, a_idEstagio, vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getValorPrimal(getVarDecisao_ZI(a_TSS, a_idEstagio)));

				if (getVarDecisao_ZFseExistir(a_TSS, a_idEstagio) > -1)
					setElemento(AttVetorModeloOtimizacao_custo_futuro, a_idEstagio, vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getValorPrimal(getVarDecisao_ZF(a_TSS, a_idEstagio)));

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
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
				throw std::invalid_argument("Codigo identificador do problema linear " + getString(codigo) + " \n" + std::string(erro.what()));

			} // catch (const std::exception & erro) {

		} // if (vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao()  == TipoStatusSolver_otimalidade) {

		else if (vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_tempo_excedido) {

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

		if (vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

			try {

				armazenarValoresSolver(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, IdRealizacao_Nenhum);

				if (getAtributo(AttComumModeloOtimizacao_imprimir_solver, bool())) {
					criarDiretorio(a_diretorio + "//" + getString(a_TSS));
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirLP(a_diretorio + "//" + getString(a_TSS) + "//PL_" + getString(a_idEstagio) + "_" + getString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())));
				}

				const double valorObjetivo = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getValorObjetivo();

				setElemento(AttVetorModeloOtimizacao_custo_total, a_idEstagio, valorObjetivo);

				if (getVarDecisao_ZIseExistir(a_TSS, a_idEstagio) > -1)
					setElemento(AttVetorModeloOtimizacao_custo_imediato, a_idEstagio, vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getValorPrimal(getVarDecisao_ZI(a_TSS, a_idEstagio)));

				if (getVarDecisao_ZFseExistir(a_TSS, a_idEstagio) > -1) {
					double zf = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getValorPrimal(getVarDecisao_ZF(a_TSS, a_idEstagio));
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

							if (getVarDecisao_ZIseExistir(a_TSS, a_idEstagio) > -1)
								setElemento(AttVetorModeloOtimizacao_custo_imediato, a_idEstagio, 0.0);

							if (getVarDecisao_ZFseExistir(a_TSS, a_idEstagio) > -1)
								setElemento(AttVetorModeloOtimizacao_custo_futuro, a_idEstagio, 0.0);

							vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->resetar();

							return 0.0;

						} // else if (getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int()) == 0) {

					} // if (erro_str.find("10005") != std::string::npos) {					
				} // if (erro_str.size() > 5) {

				const std::string codigo = getString(int(time(0)));
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
				throw std::invalid_argument("Codigo identificador do problema linear " + getString(codigo) + " \n" + std::string(erro.what()));

			} // catch (const std::exception & erro) { 

		} // if (vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

		else
			throw std::invalid_argument("O status " + getFullString(vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao()) + " nao retorna solucao do problema.");

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::posOtimizacaoProblema(" + getFullString(a_TSS) + "," + getFullString(a_idIteracao) + "," + getString(a_idEstagio) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::posOtimizacaoProblema(const IdEstagio a_idEstagio, const IdCenario a_idCenario, const std::string a_diretorio) {


bool ModeloOtimizacao::posOtimizacaoProblema(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, bool a_solucao_proxy, EstruturaResultados<double>& a_sol_inf_var_dinamica, EstruturaResultados<double>& a_solucao_dual_var_dinamica, EstruturaResultados<double>& a_limite_inferior_var_dinamica, EstruturaResultados<double>& a_limite_superior_var_dinamica, EstruturaResultados<double>& a_sol_dual_var_estado, const std::string a_diretorio) {

	try {

		if ((vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) && (a_solucao_proxy)) {

			try {

				armazenarValoresSolver(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao);

				if (getAtributo(AttComumModeloOtimizacao_imprimir_solver, bool())) {
					criarDiretorio(a_diretorio + "//" + getString(a_TSS));
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirLP(a_diretorio + "//" + getString(a_TSS) + "//PL_" + getString(a_idEstagio) + "_" + getString(a_idCenario) + "_" + getString(a_idRealizacao));
				}

				const double valorObjetivo = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getValorObjetivo();
				//const double valorObjetivo = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getDualObjValue();

				const double valorSolucaoInferior = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getSolucaoInferiorVarDinamica();

				const double valorSolucaoInferiorVarDinamica = valorObjetivo - valorSolucaoInferior;

				a_sol_inf_var_dinamica.vetor.at(a_sol_inf_var_dinamica.conta) = valorSolucaoInferiorVarDinamica;

				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getReducedCostVarDinamica(a_solucao_dual_var_dinamica.conta, &a_solucao_dual_var_dinamica.vetor[0]);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getLimiteInferiorVarDinamica(a_limite_inferior_var_dinamica.conta, &a_limite_inferior_var_dinamica.vetor[0]);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getLimiteSuperiorVarDinamica(a_limite_superior_var_dinamica.conta, &a_limite_superior_var_dinamica.vetor[0]);

				getSolucaoDualVariavelEstado(a_idEstagio, a_sol_dual_var_estado.conta, &a_sol_dual_var_estado.vetor[0]);

				setElemento(AttVetorModeloOtimizacao_custo_total, a_idEstagio, valorObjetivo);

				if (getVarDecisao_ZIseExistir(a_TSS, a_idEstagio) > -1)
					setElemento(AttVetorModeloOtimizacao_custo_imediato, a_idEstagio, vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getValorPrimal(getVarDecisao_ZI(a_TSS, a_idEstagio)));

				if (getVarDecisao_ZFseExistir(a_TSS, a_idEstagio) > -1) {
					double zf = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getValorPrimal(getVarDecisao_ZF(a_TSS, a_idEstagio));
					if ((zf > -0.1) && (zf < 0.0))
						zf = 0.0;
					setElemento(AttVetorModeloOtimizacao_custo_futuro, a_idEstagio, zf);
				}

				a_sol_inf_var_dinamica.conta++;
				a_solucao_dual_var_dinamica.conta += vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getNumeroVarDinamica();
				a_limite_inferior_var_dinamica.conta += vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getNumeroVarDinamica();
				a_limite_superior_var_dinamica.conta += vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getNumeroVarDinamica();

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
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + codigo);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + codigo);
				vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + codigo);
				throw std::invalid_argument("Codigo identificador do problema linear " + getString(codigo) + " \n" + std::string(erro.what()));

			} // catch (const std::exception & erro) {

		} // if (vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

		else if ((vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_tempo_excedido) || (!a_solucao_proxy)) {

			a_sol_inf_var_dinamica.add(0.0);

			for (int i = 0; i < vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getNumeroVarDinamica(); i++)
				a_solucao_dual_var_dinamica.add(0.0);

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getLimiteInferiorVarDinamica(a_limite_inferior_var_dinamica.conta, &a_limite_inferior_var_dinamica.vetor[0]);
			a_limite_inferior_var_dinamica.conta += vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getNumeroVarDinamica();

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getLimiteSuperiorVarDinamica(a_limite_superior_var_dinamica.conta, &a_limite_superior_var_dinamica.vetor[0]);
			a_limite_superior_var_dinamica.conta += vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getNumeroVarDinamica();

			for (int i = 0; i < int(getMaiorId(a_idEstagio, IdVariavelEstado())); i++)
				a_sol_dual_var_estado.add(0.0);

			setElemento(AttVetorModeloOtimizacao_custo_total, a_idEstagio, 0.0);

			setElemento(AttVetorModeloOtimizacao_custo_imediato, a_idEstagio, 0.0);

			setElemento(AttVetorModeloOtimizacao_custo_futuro, a_idEstagio, 0.0);

			return false;

		}

		else {
			const std::string codigo = getString(int(time(0)));
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + codigo);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + codigo);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + codigo);
			throw std::invalid_argument("O status " + getFullString(vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao()) + " nao retorna solucao do problema. Codigo identificador do problema linear " + getString(codigo));
		}

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::posOtimizacaoProblema(" + getFullString(a_TSS) + "," + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "," + getFullString(a_idRealizacao) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::posOtimizacaoProblema(const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const std::string a_diretorio){

void ModeloOtimizacao::armazenarValoresSolver(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao){

	try{

		if (a_idRealizacao == IdRealizacao_Nenhum) {

			if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, bool())) {

				int i = 1;

				VARIAVEL_DECISAO_1(ARMAZENAR_VALOR)
				VARIAVEL_DECISAO_2(ARMAZENAR_VALOR)
					VARIAVEL_DECISAO_3(ARMAZENAR_VALOR)
					VARIAVEL_DECISAO_4(ARMAZENAR_VALOR)
					VARIAVEL_DECISAO_5(ARMAZENAR_VALOR)
					VARIAVEL_DECISAO_6(ARMAZENAR_VALOR)

			}

			if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario, bool())) {

				EQUACAO_LINEAR_1(ARMAZENAR_VALOR)
				EQUACAO_LINEAR_2(ARMAZENAR_VALOR)
					EQUACAO_LINEAR_3(ARMAZENAR_VALOR)
					EQUACAO_LINEAR_4(ARMAZENAR_VALOR)
					EQUACAO_LINEAR_5(ARMAZENAR_VALOR)
					EQUACAO_LINEAR_6(ARMAZENAR_VALOR)

					INEQUACAO_LINEAR_3(ARMAZENAR_VALOR)
					INEQUACAO_LINEAR_4(ARMAZENAR_VALOR)
					INEQUACAO_LINEAR_5(ARMAZENAR_VALOR)
					INEQUACAO_LINEAR_6(ARMAZENAR_VALOR)
					INEQUACAO_LINEAR_7(ARMAZENAR_VALOR)

			} // if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, bool())) {

		}
		else {

			if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario_por_realizacao, bool())) {

				VARIAVEL_DECISAO_1(ARMAZENAR_VALOR)
				VARIAVEL_DECISAO_2(ARMAZENAR_VALOR)
					VARIAVEL_DECISAO_3(ARMAZENAR_VALOR)
					VARIAVEL_DECISAO_4(ARMAZENAR_VALOR)
					VARIAVEL_DECISAO_5(ARMAZENAR_VALOR)
					VARIAVEL_DECISAO_6(ARMAZENAR_VALOR)

			}

			if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario_por_realizacao, bool())) {

				EQUACAO_LINEAR_1(ARMAZENAR_VALOR)
				EQUACAO_LINEAR_2(ARMAZENAR_VALOR)
					EQUACAO_LINEAR_3(ARMAZENAR_VALOR)
					EQUACAO_LINEAR_4(ARMAZENAR_VALOR)
					EQUACAO_LINEAR_5(ARMAZENAR_VALOR)
					EQUACAO_LINEAR_6(ARMAZENAR_VALOR)

					INEQUACAO_LINEAR_3(ARMAZENAR_VALOR)
					INEQUACAO_LINEAR_4(ARMAZENAR_VALOR)
					INEQUACAO_LINEAR_5(ARMAZENAR_VALOR)
					INEQUACAO_LINEAR_6(ARMAZENAR_VALOR)
					INEQUACAO_LINEAR_7(ARMAZENAR_VALOR)

			}

		}


	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::armazenarValoresSolver(" + getFullString(a_TSS) + "," + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "," + getFullString(a_idRealizacao) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::armazenarValoresSolver(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao){



void ModeloOtimizacao::calcularCustoPrimalViaSubproblemaMestre(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const std::string a_diretorio, double& a_custo_geral, double& a_custo_individual) {

	try {

		if (vetorEstagio.at(a_idEstagio).isSolverInstanciado(a_TSS)) {

			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->otimizar();

			if (vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

				if (getAtributo(AttComumModeloOtimizacao_imprimir_resultado_mestre, bool()))
					armazenarValoresSolver(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, IdRealizacao_Nenhum);

				if ((getAtributo(AttComumModeloOtimizacao_imprimir_solver, bool())) && (getAtributo(AttComumModeloOtimizacao_imprimir_solver_mestre, bool()))) {
					criarDiretorio(a_diretorio + "//" + getString(a_TSS));
					vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirLP(a_diretorio + "//" + getString(a_TSS) + "//PL_" + getString(a_idEstagio) + "_" + getString(a_idCenario));
				} // if ((getAtributo(AttComumModeloOtimizacao_imprimir_solver, bool())) && (getAtributo(AttComumModeloOtimizacao_imprimir_solver_mestre, bool()))) {

				IdRealizacao idRealizacao = IdRealizacao_1;

				const Periodo periodo_processo_estocastico = getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, a_idEstagio, Periodo());

				const IdProcessoEstocastico idProcessoEstocastico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

				if (getSize1Matriz(idProcessoEstocastico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral) > 0) {
					if (periodo_processo_estocastico >= getIterador2Inicial(idProcessoEstocastico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, Periodo()))
						idRealizacao = getElementoMatriz(idProcessoEstocastico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, periodo_processo_estocastico, IdRealizacao());
				}

				const int varZT_r = getVarDecisao_ZT(a_TSS, a_idEstagio, idRealizacao);

				a_custo_individual = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getValorPrimal(varZT_r);

				const int varZT = getVarDecisao_ZT(a_TSS, a_idEstagio);

				a_custo_geral = vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getValorPrimal(varZT);

				return;

			} // if (vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

			const std::string codigo = getString(int(time(0)));
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + codigo);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + codigo);
			vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + codigo);
			throw std::invalid_argument("Erro em otimizar, codigo identificador do problema linear " + codigo);

		} // if (vetorEstagio.at(a_idEstagio).isSolverInstanciado(a_TSS)) {

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

			const double solucao_dual = vetorEstagio.at(a_idEstagio).getSolver(tSS)->getReducedCost(idVariavelDecisao);

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

		MPI_Barrier(MPI_COMM_WORLD);

		if (!getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, bool()) && !getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario_por_realizacao, bool()) &&
			!getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario, bool()) && !getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario_por_realizacao, bool()))
			return;

		const IdProcesso maior_processo = arranjoResolucao.getMaiorId(IdProcesso());

		if ((isPrintElemSync < 2) && (maior_processo > IdProcesso_mestre))  {

			std::vector<std::vector<std::string>> lista_elemento_impressao_sync(TipoSubproblemaSolver_Excedente, std::vector<std::string>());

			const IdProcesso idProcesso_local = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());

			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= maior_processo; idProcesso++) {

				if (idProcesso == idProcesso_local) {

					for (IdProcesso idProcesso_para = IdProcesso_mestre; idProcesso_para <= maior_processo; idProcesso_para++) {

						if (idProcesso_para != idProcesso) {

							int size = 0;

							for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++) {
								size = int(lista_elemento_impressao.at(tSS).size());
								MPI_Send(&size, 1, MPI_INT, getRank(idProcesso_para), 12, MPI_COMM_WORLD);
								
								for (int i = 0; i < size; i++) {
									int size_name = int(lista_elemento_impressao.at(tSS).at(i).size())+1;
									MPI_Send(&size_name, 1, MPI_INT, getRank(idProcesso_para), 13, MPI_COMM_WORLD);
									MPI_Send(lista_elemento_impressao.at(tSS).at(i).c_str(), size_name, MPI_CHAR, getRank(idProcesso_para), 14, MPI_COMM_WORLD);

									if (findStringSensNoVetorReturnPos(lista_elemento_impressao.at(tSS).at(i), lista_elemento_impressao_sync.at(tSS)) == -1)
										lista_elemento_impressao_sync.at(tSS).push_back(lista_elemento_impressao.at(tSS).at(i));
								}
							} // for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++) {

						} // if (idProcesso_para != idProcesso) {

					} // for (IdProcesso idProcesso_para = IdProcesso_mestre; idProcesso_para <= maior_processo; idProcesso_para++) {

				} // if (idProcesso == idProcesso_local) {

				else if (idProcesso != idProcesso_local) {

					int size = 0;

					for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++) {
						MPI_Recv(&size, 1, MPI_INT, getRank(idProcesso), 12, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

						for (int i = 0; i < size; i++) {
							int size_name = 0;
							MPI_Recv(&size_name, 1, MPI_INT, getRank(idProcesso), 13, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
							char* name = new char[size_name];
							MPI_Recv(name, size_name, MPI_CHAR, getRank(idProcesso), 14, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

							std::string str_recv = name;

							if (findStringSensNoVetorReturnPos(str_recv, lista_elemento_impressao_sync.at(tSS)) == -1)
								lista_elemento_impressao_sync.at(tSS).push_back(str_recv);
							
							delete[] name;
						}
					}

				} // else if (idProcesso != idProcesso_local) {

			} // for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= maior_processo; idProcesso++) {

			lista_elemento_impressao = lista_elemento_impressao_sync;

			isPrintElemSync++;

			MPI_Barrier(MPI_COMM_WORLD);

		} // if ((isPrintElemSync < 2) && (maior_processo > IdProcesso_mestre))  {

		const string diretorio = a_entradaSaidaDados.getDiretorioSaida();

		for (TipoSubproblemaSolver a_TSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); a_TSS < TipoSubproblemaSolver_Excedente; a_TSS++) {

			if (lista_elemento_impressao.at(a_TSS).size() > 0) {

				const int numero_elementos_consolidar = int(lista_elemento_impressao.at(a_TSS).size()) / int(a_maiorIdProcesso);
				const int numero_elementos_resto      = int(lista_elemento_impressao.at(a_TSS).size()) % int(a_maiorIdProcesso);

				int pos_inicial = -1;
				int pos_final = -1;

				if (getRank(a_idProcesso) < numero_elementos_resto) {
					pos_inicial = getRank(a_idProcesso) * (numero_elementos_consolidar + 1);
					pos_final = pos_inicial + numero_elementos_consolidar;
				}
				else {
					pos_inicial = numero_elementos_resto + (getRank(a_idProcesso) * numero_elementos_consolidar);
					pos_final = pos_inicial + numero_elementos_consolidar - 1;
				}

				for (int i = pos_inicial; i <= pos_final; i++) {

					const std::string elem_str = lista_elemento_impressao.at(a_TSS).at(i);

					size_t pos = elem_str.find("IdEstagio");

					if (pos == std::string::npos)
						throw std::invalid_argument("Error.");

					a_entradaSaidaDados.setDiretorioSaida(diretorio + "//" + getString(a_TSS) + "//" + elem_str.substr(0, pos - 1));
					std::string nome_arquivo = elem_str;
					nome_arquivo.erase(nome_arquivo.size() - 1); 
					nome_arquivo += ".csv"; 
					std::vector<std::string> lista_arquivos(a_maiorIdProcesso, ""); 
					for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) 
						lista_arquivos.at(getRank(idProcesso)) = elem_str + getFullString(idProcesso) + ".csv";
					a_entradaSaidaDados.imprimirConsolidacaoVerticalCSV(nome_arquivo, lista_arquivos, true, true); 
						
					pos = elem_str.find("FINF");
					if (pos == std::string::npos)
						pos = elem_str.find("FSUP");
					
					if (pos != std::string::npos) {
						a_entradaSaidaDados.setDiretorioSaida(diretorio + "//" + getString(a_TSS));
						a_entradaSaidaDados.imprimirConsolidacaoVerticalCSV(nome_arquivo, lista_arquivos, true, true);
					}		

				} // for (int i = pos_inicial; i < pos_final; i++) {
			}

		} // for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++){

		MPI_Barrier(MPI_COMM_WORLD);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::consolidarResultados(" + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }
} // void ModeloOtimizacao::consolidarResultados(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados){



int ModeloOtimizacao::getNumeroVariavelDinamica(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio){

	try {

		return vetorEstagio.at(a_idEstagio).getSolver(a_TSS)->getNumeroVarDinamica();

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getNumeroVariavelDinamica(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }

} // int ModeloOtimizacao::getNumeroVariavelDinamica(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio){



void ModeloOtimizacao::criarModeloOtimizacao(Dados &a_dados, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const IdModeloOtimizacao idModeloOtimizacao = getAtributo(AttComumModeloOtimizacao_idModeloOtimizacao, IdModeloOtimizacao());

		if ((idModeloOtimizacao == IdModeloOtimizacao_multiestagio_estocastico_otimizacao) || (idModeloOtimizacao == IdModeloOtimizacao_multiestagio_estocastico_simulacao)) {

			const IdEstagio estagio_inicial = IdEstagio_1;

			setAtributo(AttComumModeloOtimizacao_estagio_inicial, estagio_inicial);

			const IdEstagio estagio_final = a_dados.getAtributo(AttComumDados_estagio_final, IdEstagio());

			setAtributo(AttComumModeloOtimizacao_estagio_final, estagio_final);

			setAtributo(AttComumModeloOtimizacao_periodo_otimizacao_inicial, a_dados.getElementoVetor(AttVetorDados_horizonte_otimizacao, estagio_inicial, Periodo()));
			setAtributo(AttComumModeloOtimizacao_periodo_otimizacao_final, a_dados.getElementoVetor(AttVetorDados_horizonte_otimizacao, estagio_final, Periodo()));

			setAtributo(AttComumModeloOtimizacao_periodo_estudo_inicial, a_dados.getElementosMatriz(AttMatrizDados_percentual_duracao_horizonte_estudo, estagio_inicial, Periodo(), double()).getIteradorInicial());
			setAtributo(AttComumModeloOtimizacao_periodo_estudo_final, a_dados.getElementosMatriz(AttMatrizDados_percentual_duracao_horizonte_estudo, estagio_final, Periodo(), double()).getIteradorFinal());

			setAtributo(AttComumModeloOtimizacao_maior_estagio_impressao_versao_alternativa_cortes, a_dados.getAtributo(AttComumDados_maior_estagio_impressao_versao_alternativa_cortes, IdEstagio()));

			setAtributo(AttComumModeloOtimizacao_foco_numerico, a_dados.getAtributo(AttComumDados_foco_numerico, int()));

			setAtributo(AttComumModeloOtimizacao_multiplicador_tempo_limite_resolucao, a_dados.getAtributo(AttComumDados_multiplicador_tempo_limite_resolucao, double()));

			setAtributo(AttComumModeloOtimizacao_tolerancia_otimalidade, a_dados.getAtributo(AttComumDados_tolerancia_otimalidade, double()));
			setAtributo(AttComumModeloOtimizacao_tolerancia_viabilidade, a_dados.getAtributo(AttComumDados_tolerancia_viabilidade, double()));

			setAtributo(AttComumModeloOtimizacao_tolerancia_convergencia, a_dados.getAtributo(AttComumDados_tolerancia_convergencia, double()));
			setAtributo(AttComumModeloOtimizacao_tipo_convergencia, a_dados.getAtributo(AttComumDados_tipo_convergencia, TipoConvergencia()));

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

			setAtributo(AttComumModeloOtimizacao_diretorio_importacao_pos_estudo, a_dados.getAtributo(AttComumDados_diretorio_importacao_pos_estudo, std::string()));

			setAtributo(AttComumModeloOtimizacao_calcular_custo_primal_via_subproblema_mestre, a_dados.getAtributo(AttComumDados_calcular_custo_primal_via_subproblema_mestre, bool()));

			setAtributo(AttComumModeloOtimizacao_calcular_cenario_hidrologico_pre_otimizacao, a_dados.getAtributo(AttComumDados_calcular_cenario_hidrologico_pre_otimizacao, bool()));

			setAtributo(AttComumModeloOtimizacao_imprimir_cenario_hidrologico_pre_otimizacao, a_dados.getAtributo(AttComumDados_imprimir_cenario_hidrologico_pre_otimizacao, bool()));

			setAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, a_dados.getAtributo(AttComumDados_imprimir_variavel_decisao_por_estagio_por_cenario, bool()));
			setAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario_por_realizacao, a_dados.getAtributo(AttComumDados_imprimir_variavel_decisao_por_estagio_por_cenario_por_realizacao, bool()));

			setAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario, a_dados.getAtributo(AttComumDados_imprimir_restricao_por_estagio_por_cenario, bool()));
			setAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario_por_realizacao, a_dados.getAtributo(AttComumDados_imprimir_restricao_por_estagio_por_cenario_por_realizacao, bool()));
			
			setAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, a_dados.getAtributo(AttComumDados_imprimir_resultado_viabilidade_hidraulica, bool()));
			setAtributo(AttComumModeloOtimizacao_imprimir_resultado_mestre, a_dados.getAtributo(AttComumDados_imprimir_resultado_mestre, bool()));

			setAtributo(AttComumModeloOtimizacao_relaxar_afluencia_incremental_com_viabilidade_hidraulica, a_dados.getAtributo(AttComumDados_relaxar_afluencia_incremental_com_viabilidade_hidraulica, bool()));

			setAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, a_dados.getAtributo(AttComumDados_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico()));

			setAtributo(AttComumModeloOtimizacao_imprimir_estado_por_iteracao, a_dados.getAtributo(AttComumDados_imprimir_estado_por_iteracao, bool()));
			setAtributo(AttComumModeloOtimizacao_imprimir_corte_por_iteracao, a_dados.getAtributo(AttComumDados_imprimir_corte_por_iteracao, bool()));
			setAtributo(AttComumModeloOtimizacao_imprimir_selecao_corte_por_iteracao, a_dados.getAtributo(AttComumDados_imprimir_selecao_corte_por_iteracao, bool()));

			setAtributo(AttComumModeloOtimizacao_iteracao_maxima_cortes_externos, a_dados.getAtributo(AttComumDados_iteracao_maxima_cortes_externos, IdIteracao()));

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

		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("PROCESSO_ESTOCASTICO_AttComumOperacional.csv", vetorProcessoEstocastico.at(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_direto);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("PROCESSO_ESTOCASTICO_" + getString(AttMatrizProcessoEstocastico_probabilidade_realizacao) + ".csv", vetorProcessoEstocastico.at(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_direto);
		
		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("VARIAVEL_ALEATORIA_AttComumOperacional.csv", vetorProcessoEstocastico.at(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_m1);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("VARIAVEL_ALEATORIA_" + getString(AttVetorVariavelAleatoria_tipo_relaxacao) + ".csv", vetorProcessoEstocastico.at(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_m1);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("VARIAVEL_ALEATORIA_" + getString(AttMatrizVariavelAleatoria_residuo_espaco_amostral) + ".csv",            vetorProcessoEstocastico.at(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_m1);
		
		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("VARIAVEL_ALEATORIA_" + getString(AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao) + ".csv", vetorProcessoEstocastico.at(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_m1);

		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("VARIAVEL_ALEATORIA_INTERNA_AttComumOperacional.csv", vetorProcessoEstocastico.at(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_m2);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("VARIAVEL_ALEATORIA_INTERNA_" + getString(AttVetorVariavelAleatoriaInterna_coeficiente_participacao) + ".csv", vetorProcessoEstocastico.at(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_m2);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("VARIAVEL_ALEATORIA_INTERNA_" + getString(AttVetorVariavelAleatoriaInterna_tendencia_temporal) + ".csv", vetorProcessoEstocastico.at(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_m2);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("PROCESSO_ESTOCASTICO_" + getString(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral) + ".csv", vetorProcessoEstocastico.at(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_direto);

		
	} // try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::instanciarProcessoEstocastico(a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::instanciarProcessoEstocastico(EntradaSaidaDados a_entradaSaidaDados) {

bool ModeloOtimizacao::anyVarEstadoENA() {

	try {

		bool is_variavelEstado_ENA = false;

		const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());
		const IdEstagio estagio_futuro = IdEstagio(estagio_final + 1);

		IdEstagio maiorIdEstagio = getMaiorId(IdEstagio());

		if (estagio_futuro > maiorIdEstagio) //Significa que não existem cortes futuros
			return is_variavelEstado_ENA;

		const IdVariavelEstado maiorIdVariavelEstado = getMaiorId(estagio_futuro, IdVariavelEstado());

		for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {
			if (vetorEstagio.at(estagio_futuro).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {
				const std::string nome = vetorEstagio.at(estagio_futuro).getAtributo(idVariavelEstado, AttComumVariavelEstado_nome, std::string());
				if (nome.at(0) == 'E' && nome.at(1) == 'N' && nome.at(2) == 'A' && nome.at(3) == ',') {
					is_variavelEstado_ENA = true;
					break;
				}//if (nome.at(0) == "ENA") {
			}
		}//for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

		return is_variavelEstado_ENA;

	} // try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::anyVarEstadoENA(): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::instanciarProcessoEstocastico(EntradaSaidaDados a_entradaSaidaDados) {