#include "C_VariavelEstado.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VariavelEstado::VariavelEstado(){
	try {

		INICIALIZA_SMART_ELEMENTO(VariavelEstado, SMART_ELEMENTO_VARIAVEL_ESTADO)
	}
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelEstado::VariavelEstado(): \n" + std::string(erro.what())); }
}

VariavelEstado::VariavelEstado(const VariavelEstado &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(VariavelEstado, SMART_ELEMENTO_VARIAVEL_ESTADO) }
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelEstado::VariavelEstado(const VariavelEstado &instanciaCopiar): \n" + std::string(erro.what())); }
}

void VariavelEstado::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(VariavelEstado, SMART_ELEMENTO_VARIAVEL_ESTADO) }
	catch (const std::exception& erro) { throw std::invalid_argument("VariavelEstado::esvaziar(): \n" + std::string(erro.what())); }
}

VariavelEstado::~VariavelEstado(){ EXCLUI_SMART_ELEMENTO(VariavelEstado, SMART_ELEMENTO_VARIAVEL_ESTADO) }
