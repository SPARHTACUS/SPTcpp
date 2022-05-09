#include "C_VariavelRealizacaoInterna.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VariavelRealizacaoInterna::VariavelRealizacaoInterna(){
	try {
		INICIALIZA_SMART_ELEMENTO(VariavelRealizacaoInterna, SMART_ELEMENTO_VARIAVEL_REALIZACAO_INTERNA)
	}
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelRealizacaoInterna::VariavelRealizacaoInterna(): \n" + std::string(erro.what())); }
}

VariavelRealizacaoInterna::VariavelRealizacaoInterna(const VariavelRealizacaoInterna &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(VariavelRealizacaoInterna, SMART_ELEMENTO_VARIAVEL_REALIZACAO_INTERNA) }
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelRealizacaoInterna::VariavelRealizacaoInterna(const VariavelRealizacaoInterna &instanciaCopiar): \n" + std::string(erro.what())); }
}

void VariavelRealizacaoInterna::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(VariavelRealizacaoInterna, SMART_ELEMENTO_VARIAVEL_REALIZACAO_INTERNA) }
	catch (const std::exception& erro) { throw std::invalid_argument("VariavelRealizacaoInterna::esvaziar(): \n" + std::string(erro.what())); }
}

VariavelRealizacaoInterna::~VariavelRealizacaoInterna(){ EXCLUI_SMART_ELEMENTO(VariavelRealizacaoInterna, SMART_ELEMENTO_VARIAVEL_REALIZACAO_INTERNA) }
