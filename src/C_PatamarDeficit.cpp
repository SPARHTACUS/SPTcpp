#include "C_PatamarDeficit.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PatamarDeficit::PatamarDeficit(){
	try { INICIALIZA_SMART_ELEMENTO(PatamarDeficit, SMART_ELEMENTO_PATAMAR_DEFICIT) }
	catch (const std::exception&erro) { throw std::invalid_argument("PatamarDeficit::PatamarDeficit(): \n" + std::string(erro.what())); }
}

PatamarDeficit::PatamarDeficit(const PatamarDeficit &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(PatamarDeficit, SMART_ELEMENTO_PATAMAR_DEFICIT) }
	catch (const std::exception&erro) { throw std::invalid_argument("PatamarDeficit::PatamarDeficit(const PatamarDeficit &instanciaCopiar): \n" + std::string(erro.what())); }
}

void PatamarDeficit::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(PatamarDeficit, SMART_ELEMENTO_PATAMAR_DEFICIT) }
	catch (const std::exception& erro) { throw std::invalid_argument("PatamarDeficit::esvaziar(): \n" + std::string(erro.what())); }
}

PatamarDeficit::~PatamarDeficit(){ EXCLUI_SMART_ELEMENTO(PatamarDeficit, SMART_ELEMENTO_PATAMAR_DEFICIT) }
