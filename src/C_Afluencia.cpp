#include "C_Afluencia.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Afluencia::Afluencia(){
	try {
		INICIALIZA_SMART_ELEMENTO(Afluencia, SMART_ELEMENTO_AFLUENCIA)

	}
	catch (const std::exception&erro) { throw std::invalid_argument("Afluencia::Afluencia(): \n" + std::string(erro.what())); }
}

Afluencia::Afluencia(const Afluencia &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(Afluencia, SMART_ELEMENTO_AFLUENCIA) }
	catch (const std::exception&erro) { throw std::invalid_argument("Afluencia::Afluencia(const Afluencia &instanciaCopiar): \n" + std::string(erro.what())); }
}

void Afluencia::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(Afluencia, SMART_ELEMENTO_AFLUENCIA) }
	catch (const std::exception& erro) { throw std::invalid_argument("Afluencia::esvaziar(): \n" + std::string(erro.what())); }
}

Afluencia::~Afluencia(){
	EXCLUI_SMART_ELEMENTO(Afluencia, SMART_ELEMENTO_AFLUENCIA)
}
