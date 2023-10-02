#include "C_RestricaoRealizacao.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RestricaoRealizacao::RestricaoRealizacao(){
	try {
		INICIALIZA_SMART_ELEMENTO(RestricaoRealizacao, SMART_ELEMENTO_RESTRICAO_REALIZACAO) 
	}
	catch (const std::exception&erro) { throw std::invalid_argument("RestricaoRealizacao::RestricaoRealizacao(): \n" + std::string(erro.what())); }
}

RestricaoRealizacao::RestricaoRealizacao(const RestricaoRealizacao &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(RestricaoRealizacao, SMART_ELEMENTO_RESTRICAO_REALIZACAO) }
	catch (const std::exception&erro) { throw std::invalid_argument("RestricaoRealizacao::RestricaoRealizacao(const RestricaoRealizacao &instanciaCopiar): \n" + std::string(erro.what())); }
}

void RestricaoRealizacao::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(RestricaoRealizacao, SMART_ELEMENTO_RESTRICAO_REALIZACAO) }
	catch (const std::exception& erro) { throw std::invalid_argument("RestricaoRealizacao::esvaziar(): \n" + std::string(erro.what())); }
}

RestricaoRealizacao::~RestricaoRealizacao(){ EXCLUI_SMART_ELEMENTO(RestricaoRealizacao, SMART_ELEMENTO_RESTRICAO_REALIZACAO) }
