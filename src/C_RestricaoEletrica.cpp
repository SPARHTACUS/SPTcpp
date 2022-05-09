#include "C_RestricaoEletrica.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RestricaoEletrica::RestricaoEletrica(){
	try { INICIALIZA_SMART_ELEMENTO(RestricaoEletrica, SMART_ELEMENTO_RESTRICAO_ELETRICA) }
	catch (const std::exception&erro) { throw std::invalid_argument("RestricaoEletrica::RestricaoEletrica(): \n" + std::string(erro.what())); }
}

RestricaoEletrica::RestricaoEletrica(const RestricaoEletrica &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(RestricaoEletrica, SMART_ELEMENTO_RESTRICAO_ELETRICA) }
	catch (const std::exception&erro) { throw std::invalid_argument("RestricaoEletrica::RestricaoEletrica(const RestricaoEletrica &instanciaCopiar): \n" + std::string(erro.what())); }
}

RestricaoEletrica& RestricaoEletrica::operator=(const RestricaoEletrica& instanciaCopiar) {
	try {
		COPIA_SMART_ELEMENTO(RestricaoEletrica, SMART_ELEMENTO_RESTRICAO_ELETRICA)
			return *this;
	}
	catch (const std::exception& erro) { throw std::invalid_argument("RestricaoEletrica(" + getString(getIdObjeto()) + ")::operator=(const RestricaoEletrica &instanciaCopiar): \n" + std::string(erro.what())); }
}

void RestricaoEletrica::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(RestricaoEletrica, SMART_ELEMENTO_RESTRICAO_ELETRICA) }
	catch (const std::exception& erro) { throw std::invalid_argument("RestricaoEletrica::esvaziar(): \n" + std::string(erro.what())); }
}

RestricaoEletrica::~RestricaoEletrica(){ EXCLUI_SMART_ELEMENTO(RestricaoEletrica, SMART_ELEMENTO_RESTRICAO_ELETRICA) }
