#include "C_Submercado.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Submercado::Submercado(){
	try { INICIALIZA_SMART_ELEMENTO(Submercado, SMART_ELEMENTO_SUBMERCADO) }
	catch (const std::exception&erro) { throw std::invalid_argument("Submercado::Submercado(): \n" + std::string(erro.what())); }
}

Submercado::Submercado(const Submercado &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(Submercado, SMART_ELEMENTO_SUBMERCADO) }
	catch (const std::exception&erro) { throw std::invalid_argument("Submercado::Submercado(const Submercado &instanciaCopiar): \n" + std::string(erro.what())); }
}

void Submercado::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(Submercado, SMART_ELEMENTO_SUBMERCADO) }
	catch (const std::exception& erro) { throw std::invalid_argument("Submercado::esvaziar(): \n" + std::string(erro.what())); }
}

Submercado::~Submercado(){ EXCLUI_SMART_ELEMENTO(Submercado, SMART_ELEMENTO_SUBMERCADO) }



