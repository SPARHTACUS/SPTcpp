#include "C_RestricaoOperativaUHE.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RestricaoOperativaUHE::RestricaoOperativaUHE() {
	try { INICIALIZA_SMART_ELEMENTO(RestricaoOperativaUHE, SMART_ELEMENTO_REST_OPER_LIMITE_UHE) }
	catch (const std::exception&erro) { throw std::invalid_argument("RestricaoOperativaUHE::RestricaoOperativaUHE(): \n" + std::string(erro.what())); }
}

RestricaoOperativaUHE::RestricaoOperativaUHE(const RestricaoOperativaUHE &instanciaCopiar) {
	try { COPIA_SMART_ELEMENTO(RestricaoOperativaUHE, SMART_ELEMENTO_REST_OPER_LIMITE_UHE) }
	catch (const std::exception&erro) { throw std::invalid_argument("RestricaoOperativaUHE::RestricaoOperativaUHE(const RestricaoOperativaUHE &instanciaCopiar): \n" + std::string(erro.what())); }
}

RestricaoOperativaUHE& RestricaoOperativaUHE::operator=(const RestricaoOperativaUHE& instanciaCopiar) {
	try {
		COPIA_SMART_ELEMENTO(RestricaoOperativaUHE, SMART_ELEMENTO_REST_OPER_LIMITE_UHE)
			return *this;
	}
	catch (const std::exception& erro) { throw std::invalid_argument("RestricaoOperativaUHE(" + getString(getIdObjeto()) + ")::operator=(const RestricaoOperativaUHE &instanciaCopiar): \n" + std::string(erro.what())); }
}

void RestricaoOperativaUHE::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(RestricaoOperativaUHE, SMART_ELEMENTO_REST_OPER_LIMITE_UHE) }
	catch (const std::exception& erro) { throw std::invalid_argument("RestricaoOperativaUHE::esvaziar(): \n" + std::string(erro.what())); }
}

RestricaoOperativaUHE::~RestricaoOperativaUHE() { EXCLUI_SMART_ELEMENTO(RestricaoOperativaUHE, SMART_ELEMENTO_REST_OPER_LIMITE_UHE) }
