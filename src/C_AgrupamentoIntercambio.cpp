#include "C_AgrupamentoIntercambio.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AgrupamentoIntercambio::AgrupamentoIntercambio(){
	try { INICIALIZA_SMART_ELEMENTO(AgrupamentoIntercambio, SMART_ELEMENTO_AGRUPAMENTO_INTERCAMBIO) }
	catch (const std::exception&erro) { throw std::invalid_argument("AgrupamentoIntercambio::AgrupamentoIntercambio(): \n" + std::string(erro.what())); }
}

AgrupamentoIntercambio::AgrupamentoIntercambio(const AgrupamentoIntercambio &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(AgrupamentoIntercambio, SMART_ELEMENTO_AGRUPAMENTO_INTERCAMBIO) }
	catch (const std::exception&erro) { throw std::invalid_argument("AgrupamentoIntercambio::AgrupamentoIntercambio(const AgrupamentoIntercambio &instanciaCopiar): \n" + std::string(erro.what())); }
}

void AgrupamentoIntercambio::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(AgrupamentoIntercambio, SMART_ELEMENTO_AGRUPAMENTO_INTERCAMBIO) }
	catch (const std::exception& erro) { throw std::invalid_argument("AgrupamentoIntercambio::esvaziar(): \n" + std::string(erro.what())); }
}

AgrupamentoIntercambio::~AgrupamentoIntercambio(){ EXCLUI_SMART_ELEMENTO(AgrupamentoIntercambio, SMART_ELEMENTO_AGRUPAMENTO_INTERCAMBIO) }
