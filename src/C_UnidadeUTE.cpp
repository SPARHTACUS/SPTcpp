#include "C_UnidadeUTE.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UnidadeUTE::UnidadeUTE(){
	try { INICIALIZA_SMART_ELEMENTO(UnidadeUTE, SMART_ELEMENTO_UNIDADE_UTE) }
	catch (const std::exception&erro) { throw std::invalid_argument("UnidadeUTE::UnidadeUTE(): \n" + std::string(erro.what())); }
}

UnidadeUTE::UnidadeUTE(const UnidadeUTE &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(UnidadeUTE, SMART_ELEMENTO_UNIDADE_UTE) }
	catch (const std::exception&erro) { throw std::invalid_argument("UnidadeUTE::UnidadeUTE(const UnidadeUTE &instanciaCopiar): \n" + std::string(erro.what())); }
}

void UnidadeUTE::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(UnidadeUTE, SMART_ELEMENTO_UNIDADE_UTE) }
	catch (const std::exception& erro) { throw std::invalid_argument("UnidadeUTE::esvaziar(): \n" + std::string(erro.what())); }
}

UnidadeUTE::~UnidadeUTE(){ EXCLUI_SMART_ELEMENTO(UnidadeUTE, SMART_ELEMENTO_UNIDADE_UTE) }
