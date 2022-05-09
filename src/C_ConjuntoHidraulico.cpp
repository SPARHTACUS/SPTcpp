#include "C_ConjuntoHidraulico.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ConjuntoHidraulico::ConjuntoHidraulico() {
	try { INICIALIZA_SMART_ELEMENTO(ConjuntoHidraulico, SMART_ELEMENTO_CONJUNTO_HIDRAULICO) }
	catch (const std::exception&erro) { throw std::invalid_argument("ConjuntoHidraulico::ConjuntoHidraulico(): \n" + std::string(erro.what())); }
}

ConjuntoHidraulico::ConjuntoHidraulico(const ConjuntoHidraulico & instanciaCopiar) {
	try { COPIA_SMART_ELEMENTO(ConjuntoHidraulico, SMART_ELEMENTO_CONJUNTO_HIDRAULICO) }
	catch (const std::exception&erro) { throw std::invalid_argument("ConjuntoHidraulico::ConjuntoHidraulico(const ConjuntoHidraulico &instanciaCopiar): \n" + std::string(erro.what())); }
}

void ConjuntoHidraulico::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(ConjuntoHidraulico, SMART_ELEMENTO_CONJUNTO_HIDRAULICO) }
	catch (const std::exception& erro) { throw std::invalid_argument("ConjuntoHidraulico::esvaziar(): \n" + std::string(erro.what())); }
}

ConjuntoHidraulico::~ConjuntoHidraulico() {
	EXCLUI_SMART_ELEMENTO(ConjuntoHidraulico, SMART_ELEMENTO_CONJUNTO_HIDRAULICO)
}



