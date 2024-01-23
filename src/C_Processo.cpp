#include "C_Processo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Processo::Processo() {
	try { INICIALIZA_SMART_ELEMENTO(Processo, SMART_ELEMENTO_PROCESSO) }
	catch (const std::exception&erro) { throw std::invalid_argument("Processo::Processo(): \n" + std::string(erro.what())); }
}

Processo::Processo(const Processo &instanciaCopiar) {
	try { COPIA_SMART_ELEMENTO(Processo, SMART_ELEMENTO_PROCESSO) }
	catch (const std::exception&erro) { throw std::invalid_argument("Processo::Processo(const Processo &instanciaCopiar): \n" + std::string(erro.what())); }
}

void Processo::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(Processo, SMART_ELEMENTO_PROCESSO) }
	catch (const std::exception& erro) { throw std::invalid_argument("Processo::esvaziar(): \n" + std::string(erro.what())); }
}

Processo::~Processo() { EXCLUI_SMART_ELEMENTO(Processo, SMART_ELEMENTO_PROCESSO) }

