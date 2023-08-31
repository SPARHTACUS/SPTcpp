#include "C_CorteBenders.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CorteBenders::CorteBenders(){
	try { INICIALIZA_SMART_ELEMENTO(CorteBenders, SMART_ELEMENTO_CORTE_BENDERS) }
	catch (const std::exception&erro) { throw std::invalid_argument("CorteBenders::CorteBenders(): \n" + std::string(erro.what())); }
}

CorteBenders::CorteBenders(const CorteBenders &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(CorteBenders, SMART_ELEMENTO_CORTE_BENDERS) }
	catch (const std::exception&erro) { throw std::invalid_argument("CorteBenders::CorteBenders(const CorteBenders &instanciaCopiar): \n" + std::string(erro.what())); }
}

void CorteBenders::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(CorteBenders, SMART_ELEMENTO_CORTE_BENDERS) }
	catch (const std::exception& erro) { throw std::invalid_argument("CorteBenders::esvaziar(): \n" + std::string(erro.what())); }
}

CorteBenders::~CorteBenders(){ EXCLUI_SMART_ELEMENTO(CorteBenders, SMART_ELEMENTO_CORTE_BENDERS) }



void CorteBenders::anularVariavelEstado(const IdVariavelEstado a_idVariavelEstado){

	try {

		double valor_estado = 0.0;

		if (getSizeVetor(AttVetorCorteBenders_estado, a_idVariavelEstado) > 0) {
			valor_estado = getElementoVetor(AttVetorCorteBenders_estado, a_idVariavelEstado, double());
			setElemento(AttVetorCorteBenders_estado, a_idVariavelEstado, 0.0);
		}

		const IdRealizacao menorIdRealizacao = getIterador1Inicial(AttMatrizCorteBenders_coeficiente, IdRealizacao());
		const IdRealizacao maiorIdRealizacao = getIterador1Final(AttMatrizCorteBenders_coeficiente, IdRealizacao());

		for (IdRealizacao idRealizacao = menorIdRealizacao; idRealizacao <= maiorIdRealizacao; idRealizacao++) {

			const double coeficiente = getElementoMatriz(AttMatrizCorteBenders_coeficiente, idRealizacao, a_idVariavelEstado, double());
			setElemento(AttMatrizCorteBenders_coeficiente, idRealizacao, a_idVariavelEstado, 0.0);

			const double parcela_rhs = valor_estado * coeficiente;

			const double rhs = getElementoVetor(AttVetorCorteBenders_rhs, idRealizacao, double());

			setElemento(AttVetorCorteBenders_rhs, idRealizacao, rhs - parcela_rhs);

			if (getSizeVetor(AttVetorCorteBenders_rhs_estado) > 0) {

				const double rhs_estado = getElementoVetor(AttVetorCorteBenders_rhs_estado, idRealizacao, double());

				setElemento(AttVetorCorteBenders_rhs_estado, idRealizacao, rhs_estado - parcela_rhs);

			} // if (getSizeVetor(AttVetorCorteBenders_rhs_estado) > 0) {

		} // for (IdRealizacao idRealizacao = menorIdRealizacao; idRealizacao <= maiorIdRealizacao; idRealizacao++) {

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("CorteBenders::anularVariavelEstado(" + getFullString(a_idVariavelEstado) + "): \n" + std::string(erro.what())); }

} // void CorteBenders::anularVariavelEstado(const IdVariavelEstado a_idVariavelEstado){

void CorteBenders::sequencializaOrdenacaoCorteDominante(const SmartEnupla<IdCorteBenders, IdCorteBenders>& a_mapeamento_cortes) {

	try {

		if (getSizeMatriz(AttMatrizCorteBenders_ordenacao_cortes_dominantes) > 0) {
			for (IdRealizacao idReal = IdRealizacao_1; idReal <= getIterador1Final(AttMatrizCorteBenders_ordenacao_cortes_dominantes, IdRealizacao()); idReal++) {
				if (getSizeMatriz(AttMatrizCorteBenders_ordenacao_cortes_dominantes, idReal) > 0) {
					for (int i = getIterador2Inicial(AttMatrizCorteBenders_ordenacao_cortes_dominantes, idReal, int()); i <= getIterador2Final(AttMatrizCorteBenders_ordenacao_cortes_dominantes, idReal, int()); i++) {
						const IdCorteBenders idCorteBenders_old = getElementoMatriz(AttMatrizCorteBenders_ordenacao_cortes_dominantes, idReal, i, IdCorteBenders());
						for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= a_mapeamento_cortes.getIteradorFinal(); idCorteBenders++) {
							if (idCorteBenders_old == a_mapeamento_cortes.at(idCorteBenders)) {
								setElemento(AttMatrizCorteBenders_ordenacao_cortes_dominantes, idReal, i, idCorteBenders);
								break;
							}
						}
					}
				} // if (getSizeMatriz(AttMatrizEstagio_cortes_selecionados, idReal) > 0) {
			} // for (IdRealizacao idReal = IdRealizacao_1; idReal <= getIterador1Final(AttMatrizEstagio_cortes_selecionados, IdRealizacao()); idReal++) {
		} // if (getSizeMatriz(AttMatrizEstagio_cortes_selecionados) > 0) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("CorteBenders::sequencializaOrdenacaoCorteDominante(): \n" + std::string(erro.what())); }

} // void CorteBenders::sequencializaOrdenacaoCorteDominante(const SmartEnupla<IdCorteBenders, IdCorteBenders> &a_mapeamento_cortes) {
