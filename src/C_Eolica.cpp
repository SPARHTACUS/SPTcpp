#include "C_Eolica.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UsinaEolica::UsinaEolica(){
	try {
		INICIALIZA_SMART_ELEMENTO(UsinaEolica, SMART_ELEMENTO_USINA_EOLICA)

	}
	catch (const std::exception&erro) { throw std::invalid_argument("UsinaEolica::UsinaEolica(): \n" + std::string(erro.what())); }
}

UsinaEolica::UsinaEolica(const UsinaEolica &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(UsinaEolica, SMART_ELEMENTO_USINA_EOLICA) }
	catch (const std::exception&erro) { throw std::invalid_argument("UsinaEolica::UsinaEolica(const UsinaEolica &instanciaCopiar): \n" + std::string(erro.what())); }
}

void UsinaEolica::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(UsinaEolica, SMART_ELEMENTO_USINA_EOLICA) }
	catch (const std::exception& erro) { throw std::invalid_argument("UsinaEolica::esvaziar(): \n" + std::string(erro.what())); }
}

UsinaEolica::~UsinaEolica(){
	EXCLUI_SMART_ELEMENTO(UsinaEolica, SMART_ELEMENTO_USINA_EOLICA)
}
