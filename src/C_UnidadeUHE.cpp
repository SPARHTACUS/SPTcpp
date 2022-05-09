#include "C_UnidadeUHE.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UnidadeUHE::UnidadeUHE(){
	try { INICIALIZA_SMART_ELEMENTO(UnidadeUHE, SMART_ELEMENTO_UNIDADE_UHE) }
	catch (const std::exception&erro) { throw std::invalid_argument("UnidadeUHE::UnidadeUHE(): \n" + std::string(erro.what())); }
}

UnidadeUHE::UnidadeUHE(const UnidadeUHE &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(UnidadeUHE, SMART_ELEMENTO_UNIDADE_UHE) }
	catch (const std::exception&erro) { throw std::invalid_argument("UnidadeUHE::UnidadeUHE(const UnidadeUHE &instanciaCopiar): \n" + std::string(erro.what())); }
}

void UnidadeUHE::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(UnidadeUHE, SMART_ELEMENTO_UNIDADE_UHE) }
	catch (const std::exception& erro) { throw std::invalid_argument("UnidadeUHE::esvaziar(): \n" + std::string(erro.what())); }
}

UnidadeUHE::~UnidadeUHE(){ EXCLUI_SMART_ELEMENTO(UnidadeUHE, SMART_ELEMENTO_UNIDADE_UHE) }
