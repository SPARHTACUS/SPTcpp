#include "C_UsinaElevatoria.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UsinaElevatoria::UsinaElevatoria(){
	try {
		INICIALIZA_SMART_ELEMENTO(UsinaElevatoria, SMART_ELEMENTO_USINA_ELEVATORIA)

	}
	catch (const std::exception&erro) { throw std::invalid_argument("UsinaElevatoria::UsinaElevatoria(): \n" + std::string(erro.what())); }
}

UsinaElevatoria::UsinaElevatoria(const UsinaElevatoria &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(UsinaElevatoria, SMART_ELEMENTO_USINA_ELEVATORIA) }
	catch (const std::exception&erro) { throw std::invalid_argument("UsinaElevatoria::UsinaElevatoria(const UsinaElevatoria &instanciaCopiar): \n" + std::string(erro.what())); }
}

void UsinaElevatoria::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(UsinaElevatoria, SMART_ELEMENTO_USINA_ELEVATORIA) }
	catch (const std::exception& erro) { throw std::invalid_argument("UsinaElevatoria::esvaziar(): \n" + std::string(erro.what())); }
}

UsinaElevatoria::~UsinaElevatoria(){
	EXCLUI_SMART_ELEMENTO(UsinaElevatoria, SMART_ELEMENTO_USINA_ELEVATORIA)
}
