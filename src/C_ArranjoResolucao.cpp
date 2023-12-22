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



void ArranjoResolucao::instanciarProcessos(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso){

	try {

		setAtributo(AttComumArranjoResolucao_idProcesso, a_idProcesso);

		for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) {

			Processo processo;
			processo.setAtributo(AttComumProcesso_idProcesso, idProcesso);

			vetorProcesso.add(processo);

		} // for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= getMaiorId(IdProcesso()); idProcesso++) {

		
	}
	catch (const std::exception& erro) { throw std::invalid_argument("ArranjoResolucao::instanciarProcessos(" + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + "): \n" + std::string(erro.what())); }

}

bool ArranjoResolucao::isIdsCenarioEstadoDiferentesEmAberturasAndCenarios(const IdIteracao a_idIteracao, const IdEstagio a_idEstagio){

	try {

		for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= vetorProcesso.getMaiorId(); idProcesso++) {

			const std::vector<IdCenario> lista_cenario_estado_from_cenario  = getIdsCenarioEstadoFromCenarios(idProcesso, a_idIteracao, a_idEstagio);
			const std::vector<IdCenario> lista_cenario_estado_from_abertura = getIdsCenarioEstadoFromAberturas(idProcesso, a_idIteracao, a_idEstagio);

			if (lista_cenario_estado_from_cenario.size() != lista_cenario_estado_from_abertura.size())
				return false;

			for (int i = 0; i < int(lista_cenario_estado_from_cenario.size()); i++) {
				if (lista_cenario_estado_from_cenario.at(i) != lista_cenario_estado_from_abertura.at(i))
					return false;
			}

		} // for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= getMaiorId(IdProcesso()); idProcesso++) {

		return true;

	}
	catch (const std::exception& erro) { throw std::invalid_argument("ArranjoResolucao::isIdsCenarioEstadoDiferentesEmAberturasAndCenarios(" + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
}

std::vector<IdCenario> ArranjoResolucao::getIdsCenarioEstadoFromCenarios(const IdProcesso a_idProcesso, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio){
	try {

		// IdsCenarioEstado obtidos da matriz de abertura
		if (getSize1Matriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_cenario_estado_por_cenario) > 0) {

			IdCenario menor_idCenario = getIterador1Inicial(a_idIteracao, a_idProcesso, AttMatrizProcesso_cenario_estado_por_cenario, IdCenario());
			IdCenario maior_idCenario = getIterador1Final(a_idIteracao, a_idProcesso, AttMatrizProcesso_cenario_estado_por_cenario, IdCenario());

			vector<IdCenario> lista_retorno;
			lista_retorno.reserve(int(maior_idCenario - menor_idCenario) + 1);

			for (IdCenario idCenario = menor_idCenario; idCenario <= maior_idCenario; idCenario++) {

				const IdCenario idCenario_estado = getElementoMatriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_cenario_estado_por_cenario, idCenario, a_idEstagio, IdCenario());

				if (idCenario_estado != IdCenario_Nenhum) {

					for (int i = 0; i < int(lista_retorno.size()); i++) {
						if (lista_retorno.at(i) == idCenario_estado)
							break;
						else if (lista_retorno.at(i) > idCenario_estado) {
							lista_retorno.insert(lista_retorno.begin() + i, idCenario_estado);
							break;
						}
						else if (i == int(lista_retorno.size() - 1))
							lista_retorno.push_back(idCenario_estado);
					}

					if (lista_retorno.size() == 0)
						lista_retorno.push_back(idCenario_estado);

				} // if (idCenario_estado != IdCenario_Nenhum) {

			} // for (IdCenario idCenario = menor_idCenario; idCenario <= maior_idCenario; idCenario++) {

			return lista_retorno;

		} // if (getSize1Matriz(a_idIteracao, a_idProcessoPARA, AttMatrizProcesso_menor_cenario) > 0) {

		return std::vector<IdCenario>();

	}
	catch (const std::exception& erro) { throw std::invalid_argument("ArranjoResolucao::getIdsCenarioEstadoFromCenarios(" + getFullString(a_idProcesso) + "," + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
}

std::vector<IdCenario> ArranjoResolucao::getIdsCenarioEstadoFromAberturas(const IdProcesso a_idProcesso, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio){


	try {

		// IdsCenarioEstado obtidos da matriz de abertura
		if (getSize1Matriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_menor_abertura_por_cenario_estado) > 0) {

			IdCenario menor_idCenario_estado = getIterador1Inicial(a_idIteracao, a_idProcesso, AttMatrizProcesso_menor_abertura_por_cenario_estado, IdCenario());
			IdCenario maior_idCenario_estado = getIterador1Final(a_idIteracao, a_idProcesso, AttMatrizProcesso_menor_abertura_por_cenario_estado, IdCenario());

			vector<IdCenario> lista_retorno;
			lista_retorno.reserve(int(maior_idCenario_estado - menor_idCenario_estado) + 1);

			for (IdCenario idCenario_estado = menor_idCenario_estado; idCenario_estado <= maior_idCenario_estado; idCenario_estado++) {

				const IdAbertura idAbertura = getElementoMatriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_menor_abertura_por_cenario_estado, idCenario_estado, a_idEstagio, IdAbertura());

				if (idAbertura != IdAbertura_Nenhum)
					lista_retorno.push_back(idCenario_estado);

			} // for (IdCenario idCenario_estado = menor_idCenario_estado; idCenario_estado <= maior_idCenario_estado; idCenario_estado++) {

			return lista_retorno;

		} // if (getSize1Matriz(a_idIteracao, a_idProcessoPARA, AttMatrizProcesso_menor_abertura_por_cenario_estado) > 0) {

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

		std::vector<IdCenario> lista_cenario_estado_para;

		// IdsCenarioEstado obtidos da matriz de abertura
		if (getSize1Matriz(a_idIteracao, a_idProcessoPARA, AttMatrizProcesso_menor_abertura_por_cenario_estado) > 0) 
			lista_cenario_estado_para = getIdsCenarioEstadoFromAberturas(a_idProcessoPARA, a_idIteracao, a_idEstagio);

		else if (getSize1Matriz(a_idIteracao, a_idProcessoPARA, AttMatrizProcesso_cenario_estado_por_cenario) > 0)
			lista_cenario_estado_para = getIdsCenarioEstadoFromCenarios(a_idProcessoPARA, a_idIteracao, a_idEstagio);

		if (lista_cenario_estado_para.size() > 0){

			vector<IdCenario> lista_retorno;
			lista_retorno.reserve(lista_cenario_estado_para.size());

			for (int c = 0; c < int(lista_cenario_estado_para.size()); c++) {

				const IdCenario idcenario_estado = lista_cenario_estado_para.at(c);

				if ((menor_idCenarioDE <= idcenario_estado) && (idcenario_estado <= maior_idCenarioDE))
					lista_retorno.push_back(idcenario_estado);

			} // for (int c = 0; c < int(lista_cenario_estado_para.size()); c++) {

			return lista_retorno;

		} // if (lista_cenario_estado_para.size() > 0){

		return vector<IdCenario>();

	}
	catch (const std::exception& erro) { throw std::invalid_argument("ArranjoResolucao::getIdsCenarioEstado(" + getFullString(a_idProcessoDE) + "," + getFullString(a_idProcessoPARA) + "," + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
}

std::vector<IdCenario> ArranjoResolucao::getIdsCenarioEstado(const IdProcesso a_idProcessoDE, const IdProcesso a_idProcessoPARA, const IdIteracao a_idIteracao){

	try {

		const IdCenario menor_idCenarioDE = getAtributo(a_idIteracao, a_idProcessoDE, AttComumProcesso_menor_cenario, IdCenario());
		const IdCenario maior_idCenarioDE = getAtributo(a_idIteracao, a_idProcessoDE, AttComumProcesso_maior_cenario, IdCenario());

		if (menor_idCenarioDE == IdCenario_Nenhum)
			return vector<IdCenario>();

		const IdCenario menor_cenario_para = getAtributo(a_idIteracao, a_idProcessoPARA, AttComumProcesso_menor_cenario, IdCenario());
		if (menor_cenario_para == IdCenario_Nenhum)
			return vector<IdCenario>();

		std::vector<IdCenario> lista_cenario_estado_para_temp;
		std::vector<IdCenario> lista_cenario_estado_para;

		for (IdEstagio idEstagio = getIterador2Inicial(a_idIteracao, a_idProcessoPARA, AttMatrizProcesso_cenario_estado_por_cenario, menor_cenario_para, IdEstagio()); idEstagio <= getIterador2Final(a_idIteracao, a_idProcessoPARA, AttMatrizProcesso_cenario_estado_por_cenario, menor_cenario_para, IdEstagio()); idEstagio++) {
			// IdsCenarioEstado obtidos da matriz de abertura
			if (getSize1Matriz(a_idIteracao, a_idProcessoPARA, AttMatrizProcesso_menor_abertura_por_cenario_estado) > 0)
				lista_cenario_estado_para_temp = getIdsCenarioEstadoFromAberturas(a_idProcessoPARA, a_idIteracao, idEstagio);

			else if (getSize1Matriz(a_idIteracao, a_idProcessoPARA, AttMatrizProcesso_cenario_estado_por_cenario) > 0)
				lista_cenario_estado_para_temp = getIdsCenarioEstadoFromCenarios(a_idProcessoPARA, a_idIteracao, idEstagio);

			lista_cenario_estado_para.reserve(lista_cenario_estado_para_temp.size());
			for (int i = 0; i < int(lista_cenario_estado_para_temp.size()); i++) {

				bool encontrado = false;
				for (int j = 0; j < int(lista_cenario_estado_para.size()); j++) {
					if (lista_cenario_estado_para_temp.at(i) == lista_cenario_estado_para.at(j)) {
						encontrado = true;
						break;
					}
				}

				if (!encontrado)
					lista_cenario_estado_para.push_back(lista_cenario_estado_para_temp.at(i));
			} // for (int i = 0; i < int(lista_cenario_estado_para_temp.size()); i++) {
		}

		if (lista_cenario_estado_para.size() > 0) {

			vector<IdCenario> lista_retorno;
			lista_retorno.reserve(lista_cenario_estado_para.size());

			for (int c = 0; c < int(lista_cenario_estado_para.size()); c++) {

				const IdCenario idcenario_estado = lista_cenario_estado_para.at(c);

				if ((menor_idCenarioDE <= idcenario_estado) && (idcenario_estado <= maior_idCenarioDE))
					lista_retorno.push_back(idcenario_estado);

			} // for (int c = 0; c < int(lista_cenario_estado_para.size()); c++) {

			return lista_retorno;

		} // if (lista_cenario_estado_para.size() > 0){

		return vector<IdCenario>();

	}
	catch (const std::exception& erro) { throw std::invalid_argument("ArranjoResolucao::getIdsCenarioEstado(" + getFullString(a_idProcessoDE) + "," + getFullString(a_idProcessoPARA) + "," + getFullString(a_idIteracao) + "): \n" + std::string(erro.what())); }
}

int ArranjoResolucao::getNumeroAberturas(const IdProcesso a_idProcesso, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio) {
	try {

		// IdsCenarioEstado obtidos da matriz de abertura
		if (getSize1Matriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_menor_abertura_por_cenario_estado) > 0) {

			IdCenario menor_idCenario_estado = getIterador1Inicial(a_idIteracao, a_idProcesso, AttMatrizProcesso_menor_abertura_por_cenario_estado, IdCenario());
			IdCenario maior_idCenario_estado = getIterador1Final(a_idIteracao, a_idProcesso, AttMatrizProcesso_menor_abertura_por_cenario_estado, IdCenario());

			for (IdCenario idCenario_estado = menor_idCenario_estado; idCenario_estado <= maior_idCenario_estado; idCenario_estado++) {

				const IdAbertura menor_abertura = getElementoMatriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_menor_abertura_por_cenario_estado, idCenario_estado, a_idEstagio, IdAbertura());
				const IdAbertura maior_abertura = getElementoMatriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_maior_abertura_por_cenario_estado, idCenario_estado, a_idEstagio, IdAbertura());

				if (menor_abertura != IdAbertura_Nenhum)
					return int(maior_abertura - menor_abertura) + 1;

			} // for (IdCenario idCenario_estado = menor_idCenario_estado; idCenario_estado <= maior_idCenario_estado; idCenario_estado++) {

			return 0;

		} // if (getSize1Matriz(a_idIteracao, a_idProcessoPARA, AttMatrizProcesso_menor_abertura_por_cenario_estado) > 0) {

		throw std::invalid_argument("Erro");
	}

	catch (const std::exception& erro) { throw std::invalid_argument("ArranjoResolucao::getNumeroAberturas(" + getFullString(a_idProcesso) + "," + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
}

