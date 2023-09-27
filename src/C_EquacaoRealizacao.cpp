#include "C_EquacaoRealizacao.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

EquacaoRealizacao::EquacaoRealizacao(){
	try {
		INICIALIZA_SMART_ELEMENTO(EquacaoRealizacao, SMART_ELEMENTO_EQUACAO_REALIZACAO) 
	}
	catch (const std::exception&erro) { throw std::invalid_argument("EquacaoRealizacao::EquacaoRealizacao(): \n" + std::string(erro.what())); }
}

EquacaoRealizacao::EquacaoRealizacao(const EquacaoRealizacao &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(EquacaoRealizacao, SMART_ELEMENTO_EQUACAO_REALIZACAO) }
	catch (const std::exception&erro) { throw std::invalid_argument("EquacaoRealizacao::EquacaoRealizacao(const EquacaoRealizacao &instanciaCopiar): \n" + std::string(erro.what())); }
}

void EquacaoRealizacao::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(EquacaoRealizacao, SMART_ELEMENTO_EQUACAO_REALIZACAO) }
	catch (const std::exception& erro) { throw std::invalid_argument("EquacaoRealizacao::esvaziar(): \n" + std::string(erro.what())); }
}

EquacaoRealizacao::~EquacaoRealizacao(){ EXCLUI_SMART_ELEMENTO(EquacaoRealizacao, SMART_ELEMENTO_EQUACAO_REALIZACAO) }
