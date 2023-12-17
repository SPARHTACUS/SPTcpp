#include "C_Iteracao.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Iteracao::Iteracao() {
	try { INICIALIZA_SMART_ELEMENTO(Iteracao, SMART_ELEMENTO_ITERACAO) }
	catch (const std::exception&erro) { throw std::invalid_argument("Iteracao::Iteracao(): \n" + std::string(erro.what())); }
}

Iteracao::Iteracao(const Iteracao &instanciaCopiar) {
	try { COPIA_SMART_ELEMENTO(Iteracao, SMART_ELEMENTO_ITERACAO) }
	catch (const std::exception&erro) { throw std::invalid_argument("Iteracao::Iteracao(const Iteracao &instanciaCopiar): \n" + std::string(erro.what())); }
}

void Iteracao::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(Iteracao, SMART_ELEMENTO_ITERACAO) }
	catch (const std::exception& erro) { throw std::invalid_argument("Iteracao::esvaziar(): \n" + std::string(erro.what())); }
}

Iteracao::~Iteracao() { EXCLUI_SMART_ELEMENTO(Iteracao, SMART_ELEMENTO_ITERACAO) }



