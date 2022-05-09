#include "C_Termeletrica.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Termeletrica::Termeletrica(){
	try { INICIALIZA_SMART_ELEMENTO(Termeletrica, SMART_ELEMENTO_TERMELETRICA) }
	catch (const std::exception&erro) { throw std::invalid_argument("Termeletrica::Termeletrica(): \n" + std::string(erro.what())); }
}

Termeletrica::Termeletrica(const Termeletrica & instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(Termeletrica, SMART_ELEMENTO_TERMELETRICA) }
	catch (const std::exception&erro) { throw std::invalid_argument("Termeletrica::Termeletrica(const Termeletrica &instanciaCopiar): \n" + std::string(erro.what())); }
}

void Termeletrica::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(Termeletrica, SMART_ELEMENTO_TERMELETRICA) }
	catch (const std::exception& erro) { throw std::invalid_argument("Termeletrica::esvaziar(): \n" + std::string(erro.what())); }
}

Termeletrica::~Termeletrica(){
	EXCLUI_SMART_ELEMENTO(Termeletrica, SMART_ELEMENTO_TERMELETRICA)
}

