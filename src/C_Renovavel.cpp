#include "C_Renovavel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Renovavel::Renovavel(){
	try {
		INICIALIZA_SMART_ELEMENTO(Renovavel, SMART_ELEMENTO_RENOVAVEL)

	}
	catch (const std::exception&erro) { throw std::invalid_argument("Renovavel::Renovavel(): \n" + std::string(erro.what())); }
}

Renovavel::Renovavel(const Renovavel &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(Renovavel, SMART_ELEMENTO_RENOVAVEL) }
	catch (const std::exception&erro) { throw std::invalid_argument("Renovavel::Renovavel(const Renovavel &instanciaCopiar): \n" + std::string(erro.what())); }
}

void Renovavel::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(Renovavel, SMART_ELEMENTO_RENOVAVEL) }
	catch (const std::exception& erro) { throw std::invalid_argument("Renovavel::esvaziar(): \n" + std::string(erro.what())); }
}

Renovavel::~Renovavel(){
	EXCLUI_SMART_ELEMENTO(Renovavel, SMART_ELEMENTO_RENOVAVEL)
}
