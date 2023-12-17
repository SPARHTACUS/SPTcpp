#include "C_ArranjoResolucao.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ArranjoResolucao::ArranjoResolucao() {
	try { INICIALIZA_SMART_ELEMENTO(ArranjoResolucao, SMART_ELEMENTO_ARRANJO_RESOLUCAO) }
	catch (const std::exception&erro) { throw std::invalid_argument("ArranjoResolucao::ArranjoResolucao(): \n" + std::string(erro.what())); }
}

ArranjoResolucao::ArranjoResolucao(const ArranjoResolucao &instanciaCopiar) {
	try { COPIA_SMART_ELEMENTO(ArranjoResolucao, SMART_ELEMENTO_ARRANJO_RESOLUCAO) }
	catch (const std::exception&erro) { throw std::invalid_argument("ArranjoResolucao::ArranjoResolucao(const ArranjoResolucao &instanciaCopiar): \n" + std::string(erro.what())); }
}

ArranjoResolucao& ArranjoResolucao::operator=(const ArranjoResolucao& instanciaCopiar) {
	try {
		COPIA_SMART_ELEMENTO(ArranjoResolucao, SMART_ELEMENTO_ARRANJO_RESOLUCAO)
			return *this;
	}
	catch (const std::exception& erro) { throw std::invalid_argument("ArranjoResolucao(" + getString(getIdObjeto()) + ")::operator=(const ArranjoResolucao &instanciaCopiar): \n" + std::string(erro.what())); }
}

void ArranjoResolucao::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(ArranjoResolucao, SMART_ELEMENTO_ARRANJO_RESOLUCAO) }
	catch (const std::exception& erro) { throw std::invalid_argument("ArranjoResolucao::esvaziar(): \n" + std::string(erro.what())); }
}

ArranjoResolucao::~ArranjoResolucao() { EXCLUI_SMART_ELEMENTO(ArranjoResolucao, SMART_ELEMENTO_ARRANJO_RESOLUCAO) }



bool ArranjoResolucao::isIdsCenarioEstadoDiferentesEmAberturasAndCenarios(const IdIteracao a_idIteracao, const IdEstagio a_idEstagio){

	try {

		for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= vetorProcesso.getMaiorId(); idProcesso++) {

			// IdsCenarioEstado obtidos da matriz de abertura
			if ((getSize1Matriz(a_idIteracao, idProcesso, AttMatrizProcesso_menor_cenario) > 0) && (getSize1Matriz(a_idIteracao, idProcesso, AttMatrizProcesso_menor_abertura))) {

				if ((getIterador1Inicial(a_idIteracao, idProcesso, AttMatrizProcesso_menor_cenario, IdCenario()) == getIterador1Inicial(a_idIteracao, idProcesso, AttMatrizProcesso_menor_abertura, IdCenario())) &&
					(getIterador1Final(a_idIteracao, idProcesso, AttMatrizProcesso_menor_cenario, IdCenario()) == getIterador1Final(a_idIteracao, idProcesso, AttMatrizProcesso_menor_cenario, IdCenario())))
					return true;

			} // if ((getSize1Matriz(a_idIteracao, idProcesso, AttMatrizProcesso_menor_cenario) > 0) && (getSize1Matriz(a_idIteracao, idProcesso, AttMatrizProcesso_menor_abertura))) {

		} // for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= getMaiorId(IdProcesso()); idProcesso++) {

		return false;

	}
	catch (const std::exception& erro) { throw std::invalid_argument("ArranjoResolucao::isIdsCenarioEstadoDiferentesEmAberturasAndCenarios(" + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
}

std::vector<IdCenario> ArranjoResolucao::getIdsCenarioEstadoFromCenarios(const IdProcesso a_idProcesso, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio){
	try {

		// IdsCenarioEstado obtidos da matriz de abertura
		if (getSize1Matriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_menor_cenario) > 0) {

			IdCenario menor_idCenario_estado = getIterador1Inicial(a_idIteracao, a_idProcesso, AttMatrizProcesso_menor_cenario, IdCenario());
			IdCenario maior_idCenario_estado = getIterador1Final(a_idIteracao, a_idProcesso, AttMatrizProcesso_menor_cenario, IdCenario());

			vector<IdCenario> lista_retorno;
			lista_retorno.reserve(int(maior_idCenario_estado - menor_idCenario_estado) + 1);

			for (IdCenario idCenario_estado = menor_idCenario_estado; idCenario_estado <= maior_idCenario_estado; idCenario_estado++) {

				const IdCenario idCenario = getElementoMatriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_menor_cenario, idCenario_estado, a_idEstagio, IdCenario());

				if (idCenario != IdCenario_Nenhum)
					lista_retorno.push_back(idCenario_estado);

			} // for (IdCenario idCenario_estado = menor_idCenario_estado; idCenario_estado <= maior_idCenario_estado; idCenario_estado++) {

			return lista_retorno;

		} // if (getSize1Matriz(a_idIteracao, a_idProcessoPARA, AttMatrizProcesso_menor_cenario) > 0) {

		return std::vector<IdCenario>();

	}
	catch (const std::exception& erro) { throw std::invalid_argument("ArranjoResolucao::getIdsCenarioEstadoFromCenarios(" + getFullString(a_idProcesso) + "," + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
}

std::vector<IdCenario> ArranjoResolucao::getIdsCenarioEstadoFromAberturas(const IdProcesso a_idProcesso, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio){


	try {

		// IdsCenarioEstado obtidos da matriz de abertura
		if (getSize1Matriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_menor_abertura) > 0) {

			IdCenario menor_idCenario_estado = getIterador1Inicial(a_idIteracao, a_idProcesso, AttMatrizProcesso_menor_abertura, IdCenario());
			IdCenario maior_idCenario_estado = getIterador1Final(a_idIteracao, a_idProcesso, AttMatrizProcesso_menor_abertura, IdCenario());

			vector<IdCenario> lista_retorno;
			lista_retorno.reserve(int(maior_idCenario_estado - menor_idCenario_estado) + 1);

			for (IdCenario idCenario_estado = menor_idCenario_estado; idCenario_estado <= maior_idCenario_estado; idCenario_estado++) {

				const IdAbertura idAbertura = getElementoMatriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_menor_abertura, idCenario_estado, a_idEstagio, IdAbertura());

				if (idAbertura != IdAbertura_Nenhum)
					lista_retorno.push_back(idCenario_estado);

			} // for (IdCenario idCenario_estado = menor_idCenario_estado; idCenario_estado <= maior_idCenario_estado; idCenario_estado++) {

			return lista_retorno;

		} // if (getSize1Matriz(a_idIteracao, a_idProcessoPARA, AttMatrizProcesso_menor_abertura) > 0) {

		return std::vector<IdCenario>();

	}
	catch (const std::exception& erro) { throw std::invalid_argument("ArranjoResolucao::getIdsCenarioEstadoFromAberturas(" + getFullString(a_idProcesso) + "," + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
}

std::vector<IdCenario> ArranjoResolucao::getIdsCenarioEstado(const IdProcesso a_idProcessoDE, const IdProcesso a_idProcessoPARA, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio){

	try {

		const IdCenario menor_idCenarioDE = getAtributo(a_idIteracao, a_idProcessoDE, AttComumProcesso_menor_cenario, IdCenario());
		const IdCenario maior_idCenarioDE = getAtributo(a_idIteracao, a_idProcessoDE, AttComumProcesso_maior_cenario, IdCenario());

		if (menor_idCenarioDE == IdCenario_Nenhum)
			return vector<IdCenario>();

		// IdsCenarioEstado obtidos da matriz de abertura
		if (getSize1Matriz(a_idIteracao, a_idProcessoPARA, AttMatrizProcesso_menor_abertura) > 0) {

			IdCenario menor_idCenario_estado = getIterador1Inicial(a_idIteracao, a_idProcessoPARA, AttMatrizProcesso_menor_abertura, IdCenario());
			IdCenario maior_idCenario_estado = getIterador1Final(a_idIteracao, a_idProcessoPARA, AttMatrizProcesso_menor_abertura, IdCenario());

			if (menor_idCenario_estado < menor_idCenarioDE)
				menor_idCenario_estado = menor_idCenarioDE;

			if (maior_idCenario_estado > maior_idCenarioDE)
				maior_idCenario_estado = maior_idCenarioDE;

			vector<IdCenario> lista_retorno;
			lista_retorno.reserve(int(maior_idCenario_estado - menor_idCenario_estado) + 1);

			for (IdCenario idCenario_estado = menor_idCenario_estado; idCenario_estado <= maior_idCenario_estado; idCenario_estado++) {

				const IdAbertura idAbertura = getElementoMatriz(a_idIteracao, a_idProcessoPARA, AttMatrizProcesso_menor_abertura, idCenario_estado, a_idEstagio, IdAbertura());

				if (idAbertura != IdAbertura_Nenhum)
					lista_retorno.push_back(idCenario_estado);

			} // for (IdCenario idCenario_estado = menor_idCenario_estado; idCenario_estado <= maior_idCenario_estado; idCenario_estado++) {
		
			return lista_retorno;

		} // if (getSize1Matriz(a_idIteracao, a_idProcessoPARA, AttMatrizProcesso_menor_abertura) > 0) {

		else {

			IdCenario menor_idCenario_estado = getIterador1Inicial(a_idIteracao, a_idProcessoPARA, AttMatrizProcesso_menor_cenario, IdCenario());
			IdCenario maior_idCenario_estado = getIterador1Final(a_idIteracao, a_idProcessoPARA, AttMatrizProcesso_maior_cenario, IdCenario());

			if (menor_idCenario_estado < menor_idCenarioDE)
				menor_idCenario_estado = menor_idCenarioDE;

			if (maior_idCenario_estado > maior_idCenarioDE)
				maior_idCenario_estado = maior_idCenarioDE;

			vector<IdCenario> lista_retorno;
			lista_retorno.reserve(int(maior_idCenario_estado - menor_idCenario_estado) + 1);

			for (IdCenario idCenario_estado = menor_idCenario_estado; idCenario_estado <= maior_idCenario_estado; idCenario_estado++) {

				const IdCenario idcenario = getElementoMatriz(a_idIteracao, a_idProcessoPARA, AttMatrizProcesso_menor_cenario, idCenario_estado, a_idEstagio, IdCenario());

				if (idcenario != IdCenario_Nenhum)
					lista_retorno.push_back(idCenario_estado);

			} // for (IdCenario idCenario_estado = menor_idCenario_estado; idCenario_estado <= maior_idCenario_estado; idCenario_estado++) {

			return lista_retorno;

		}

		return vector<IdCenario>();

	}
	catch (const std::exception& erro) { throw std::invalid_argument("ArranjoResolucao::getIdsCenarioEstado(" + getFullString(a_idProcessoDE) + "," + getFullString(a_idProcessoPARA) + "," + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
}

int ArranjoResolucao::getNumeroAberturas(const IdProcesso a_idProcesso, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio) {
	try {

		// IdsCenarioEstado obtidos da matriz de abertura
		if (getSize1Matriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_menor_abertura) > 0) {

			IdCenario menor_idCenario_estado = getIterador1Inicial(a_idIteracao, a_idProcesso, AttMatrizProcesso_menor_abertura, IdCenario());
			IdCenario maior_idCenario_estado = getIterador1Final(a_idIteracao, a_idProcesso, AttMatrizProcesso_menor_abertura, IdCenario());

			for (IdCenario idCenario_estado = menor_idCenario_estado; idCenario_estado <= maior_idCenario_estado; idCenario_estado++) {

				const IdAbertura menor_abertura = getElementoMatriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_menor_abertura, idCenario_estado, a_idEstagio, IdAbertura());
				const IdAbertura maior_abertura = getElementoMatriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_maior_abertura, idCenario_estado, a_idEstagio, IdAbertura());

				if (menor_abertura != IdAbertura_Nenhum)
					return int(maior_abertura - menor_abertura) + 1;

			} // for (IdCenario idCenario_estado = menor_idCenario_estado; idCenario_estado <= maior_idCenario_estado; idCenario_estado++) {

			return 0;

		} // if (getSize1Matriz(a_idIteracao, a_idProcessoPARA, AttMatrizProcesso_menor_abertura) > 0) {

		throw std::invalid_argument("Erro");
	}

	catch (const std::exception& erro) { throw std::invalid_argument("ArranjoResolucao::getNumeroAberturas(" + getFullString(a_idProcesso) + "," + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
}

