#include "C_Defluencia.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Defluencia::Defluencia(){
	try {
		INICIALIZA_SMART_ELEMENTO(Defluencia, SMART_ELEMENTO_DEFLUENCIA)

	}
	catch (const std::exception&erro) { throw std::invalid_argument("Defluencia::Defluencia(): \n" + std::string(erro.what())); }
}

Defluencia::Defluencia(const Defluencia &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(Defluencia, SMART_ELEMENTO_DEFLUENCIA) }
	catch (const std::exception&erro) { throw std::invalid_argument("Defluencia::Defluencia(const Defluencia &instanciaCopiar): \n" + std::string(erro.what())); }
}

void Defluencia::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(Defluencia, SMART_ELEMENTO_DEFLUENCIA) }
	catch (const std::exception& erro) { throw std::invalid_argument("Defluencia::esvaziar(): \n" + std::string(erro.what())); }
}

Defluencia::~Defluencia(){
	EXCLUI_SMART_ELEMENTO(Defluencia, SMART_ELEMENTO_DEFLUENCIA)
}
