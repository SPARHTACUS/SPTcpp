#include "C_IntercambioHidraulico.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IntercambioHidraulico::IntercambioHidraulico(){
	try {
		INICIALIZA_SMART_ELEMENTO(IntercambioHidraulico, SMART_ELEMENTO_INTERCAMBIO_HIDRAULICO)

	}
	catch (const std::exception&erro) { throw std::invalid_argument("IntercambioHidraulico::IntercambioHidraulico(): \n" + std::string(erro.what())); }
}

IntercambioHidraulico::IntercambioHidraulico(const IntercambioHidraulico &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(IntercambioHidraulico, SMART_ELEMENTO_INTERCAMBIO_HIDRAULICO) }
	catch (const std::exception&erro) { throw std::invalid_argument("IntercambioHidraulico::IntercambioHidraulico(const IntercambioHidraulico &instanciaCopiar): \n" + std::string(erro.what())); }
}

void IntercambioHidraulico::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(IntercambioHidraulico, SMART_ELEMENTO_INTERCAMBIO_HIDRAULICO) }
	catch (const std::exception& erro) { throw std::invalid_argument("IntercambioHidraulico::esvaziar(): \n" + std::string(erro.what())); }
}

IntercambioHidraulico::~IntercambioHidraulico(){
	EXCLUI_SMART_ELEMENTO(IntercambioHidraulico, SMART_ELEMENTO_INTERCAMBIO_HIDRAULICO)
}
