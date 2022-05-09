#include "C_ElementoSistema.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ElementoSistema::ElementoSistema(){
	try { INICIALIZA_SMART_ELEMENTO(ElementoSistema, SMART_ELEMENTO_ELEMENTO_SISTEMA)	}
	catch (const std::exception&erro) { throw std::invalid_argument("ElementoSistema::ElementoSistema(): \n" + std::string(erro.what())); }
}

ElementoSistema::ElementoSistema(const ElementoSistema &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(ElementoSistema, SMART_ELEMENTO_ELEMENTO_SISTEMA) }
	catch (const std::exception&erro) { throw std::invalid_argument("ElementoSistema::ElementoSistema(const ElementoSistema &instanciaCopiar): \n" + std::string(erro.what())); }
}

void ElementoSistema::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(ElementoSistema, SMART_ELEMENTO_ELEMENTO_SISTEMA) }
	catch (const std::exception& erro) { throw std::invalid_argument("ElementoSistema::esvaziar(): \n" + std::string(erro.what())); }
}


ElementoSistema::~ElementoSistema(){ EXCLUI_SMART_ELEMENTO(ElementoSistema, SMART_ELEMENTO_ELEMENTO_SISTEMA) }


