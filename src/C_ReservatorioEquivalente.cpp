#include "C_ReservatorioEquivalente.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ReservatorioEquivalente::ReservatorioEquivalente() {
	try { INICIALIZA_SMART_ELEMENTO(ReservatorioEquivalente, SMART_ELEMENTO_REE) }
	catch (const std::exception&erro) { throw std::invalid_argument("ReservatorioEquivalente::ReservatorioEquivalente(): \n" + std::string(erro.what())); }
}

ReservatorioEquivalente::ReservatorioEquivalente(const ReservatorioEquivalente &instanciaCopiar) {
	try { COPIA_SMART_ELEMENTO(ReservatorioEquivalente, SMART_ELEMENTO_REE) }
	catch (const std::exception&erro) { throw std::invalid_argument("ReservatorioEquivalente::ReservatorioEquivalente(const ReservatorioEquivalente &instanciaCopiar): \n" + std::string(erro.what())); }
}

void ReservatorioEquivalente::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(ReservatorioEquivalente, SMART_ELEMENTO_REE) }
	catch (const std::exception& erro) { throw std::invalid_argument("ReservatorioEquivalente::esvaziar(): \n" + std::string(erro.what())); }
}

ReservatorioEquivalente::~ReservatorioEquivalente() { EXCLUI_SMART_ELEMENTO(ReservatorioEquivalente, SMART_ELEMENTO_REE) }

