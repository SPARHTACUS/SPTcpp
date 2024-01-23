#include "C_RestricaoCenario.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RestricaoCenario::RestricaoCenario(){
	try {
		INICIALIZA_SMART_ELEMENTO(RestricaoCenario, SMART_ELEMENTO_RESTRICAO_CENARIO) 
	}
	catch (const std::exception&erro) { throw std::invalid_argument("RestricaoCenario::RestricaoCenario(): \n" + std::string(erro.what())); }
}

RestricaoCenario::RestricaoCenario(const RestricaoCenario &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(RestricaoCenario, SMART_ELEMENTO_RESTRICAO_CENARIO) }
	catch (const std::exception&erro) { throw std::invalid_argument("RestricaoCenario::RestricaoCenario(const RestricaoCenario &instanciaCopiar): \n" + std::string(erro.what())); }
}

void RestricaoCenario::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(RestricaoCenario, SMART_ELEMENTO_RESTRICAO_CENARIO) }
	catch (const std::exception& erro) { throw std::invalid_argument("RestricaoCenario::esvaziar(): \n" + std::string(erro.what())); }
}

RestricaoCenario::~RestricaoCenario(){ EXCLUI_SMART_ELEMENTO(RestricaoCenario, SMART_ELEMENTO_RESTRICAO_CENARIO) }
