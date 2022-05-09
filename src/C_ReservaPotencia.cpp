#include "C_ReservaPotencia.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ReservaPotencia::ReservaPotencia(){
	try { INICIALIZA_SMART_ELEMENTO(ReservaPotencia, SMART_ELEMENTO_RESERVA_POTENCIA) }
	catch (const std::exception&erro) { throw std::invalid_argument("ReservaPotencia::ReservaPotencia(): \n" + std::string(erro.what())); }
}

ReservaPotencia::ReservaPotencia(const ReservaPotencia &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(ReservaPotencia, SMART_ELEMENTO_RESERVA_POTENCIA) }
	catch (const std::exception&erro) { throw std::invalid_argument("ReservaPotencia::ReservaPotencia(const ReservaPotencia &instanciaCopiar): \n" + std::string(erro.what())); }
}

void ReservaPotencia::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(ReservaPotencia, SMART_ELEMENTO_RESERVA_POTENCIA) }
	catch (const std::exception& erro) { throw std::invalid_argument("ReservaPotencia::esvaziar(): \n" + std::string(erro.what())); }
}

ReservaPotencia::~ReservaPotencia(){ EXCLUI_SMART_ELEMENTO(ReservaPotencia, SMART_ELEMENTO_RESERVA_POTENCIA) }
