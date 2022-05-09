#include "C_Contrato.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Contrato::Contrato() {
	try {
		INICIALIZA_SMART_ELEMENTO(Contrato, SMART_ELEMENTO_CONTRATO)

	}
	catch (const std::exception&erro) { throw std::invalid_argument("Contrato::Contrato(): \n" + std::string(erro.what())); }
}

Contrato::Contrato(const Contrato &instanciaCopiar) {
	try { COPIA_SMART_ELEMENTO(Contrato, SMART_ELEMENTO_CONTRATO) }
	catch (const std::exception&erro) { throw std::invalid_argument("Contrato::Contrato(const Contrato &instanciaCopiar): \n" + std::string(erro.what())); }
}

void Contrato::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(Contrato, SMART_ELEMENTO_CONTRATO) }
	catch (const std::exception& erro) { throw std::invalid_argument("Contrato::esvaziar(): \n" + std::string(erro.what())); }
}

Contrato::~Contrato() {
	EXCLUI_SMART_ELEMENTO(Contrato, SMART_ELEMENTO_CONTRATO)
}
