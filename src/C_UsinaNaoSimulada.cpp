#include "C_UsinaNaoSimulada.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UsinaNaoSimulada::UsinaNaoSimulada(){
	try { INICIALIZA_SMART_ELEMENTO(UsinaNaoSimulada, SMART_ELEMENTO_USINA_NAO_SIMULADA) }
	catch (const std::exception&erro) { throw std::invalid_argument("UsinaNaoSimulada::UsinaNaoSimulada(): \n" + std::string(erro.what())); }
}

UsinaNaoSimulada::UsinaNaoSimulada(const UsinaNaoSimulada &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(UsinaNaoSimulada, SMART_ELEMENTO_USINA_NAO_SIMULADA) }
	catch (const std::exception&erro) { throw std::invalid_argument("UsinaNaoSimulada::UsinaNaoSimulada(const UsinaNaoSimulada &instanciaCopiar): \n" + std::string(erro.what())); }
}

void UsinaNaoSimulada::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(UsinaNaoSimulada, SMART_ELEMENTO_USINA_NAO_SIMULADA) }
	catch (const std::exception& erro) { throw std::invalid_argument("UsinaNaoSimulada::esvaziar(): \n" + std::string(erro.what())); }
}

UsinaNaoSimulada::~UsinaNaoSimulada(){ EXCLUI_SMART_ELEMENTO(UsinaNaoSimulada, SMART_ELEMENTO_USINA_NAO_SIMULADA) }


