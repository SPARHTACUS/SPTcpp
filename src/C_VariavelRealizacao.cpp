#include "C_VariavelRealizacao.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VariavelRealizacao::VariavelRealizacao(){
	try {
		INICIALIZA_SMART_ELEMENTO(VariavelRealizacao, SMART_ELEMENTO_VARIAVEL_REALIZACAO) 
	}
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelRealizacao::VariavelRealizacao(): \n" + std::string(erro.what())); }
}

VariavelRealizacao::VariavelRealizacao(const VariavelRealizacao &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(VariavelRealizacao, SMART_ELEMENTO_VARIAVEL_REALIZACAO) }
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelRealizacao::VariavelRealizacao(const VariavelRealizacao &instanciaCopiar): \n" + std::string(erro.what())); }
}

void VariavelRealizacao::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(VariavelRealizacao, SMART_ELEMENTO_VARIAVEL_REALIZACAO) }
	catch (const std::exception& erro) { throw std::invalid_argument("VariavelRealizacao::esvaziar(): \n" + std::string(erro.what())); }
}

VariavelRealizacao::~VariavelRealizacao(){ EXCLUI_SMART_ELEMENTO(VariavelRealizacao, SMART_ELEMENTO_VARIAVEL_REALIZACAO) }
