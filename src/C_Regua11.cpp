#include "C_Regua11.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Regua11::Regua11() {
	try { INICIALIZA_SMART_ELEMENTO(Regua11, SMART_ELEMENTO_REGUA_11) }
	catch (const std::exception&erro) { throw std::invalid_argument("Regua11::Regua11(): \n" + std::string(erro.what())); }
}

Regua11::Regua11(const Regua11 &instanciaCopiar) {
	try { COPIA_SMART_ELEMENTO(Regua11, SMART_ELEMENTO_REGUA_11) }
	catch (const std::exception&erro) { throw std::invalid_argument("Regua11::Regua11(const Regua11 &instanciaCopiar): \n" + std::string(erro.what())); }
}

void Regua11::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(Regua11, SMART_ELEMENTO_REGUA_11) }
	catch (const std::exception& erro) { throw std::invalid_argument("Regua11::esvaziar(): \n" + std::string(erro.what())); }
}

Regua11::~Regua11() { EXCLUI_SMART_ELEMENTO(Regua11, SMART_ELEMENTO_REGUA_11) }

