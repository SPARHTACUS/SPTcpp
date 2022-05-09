#include "C_Intercambio.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Intercambio::Intercambio(){
	try { INICIALIZA_SMART_ELEMENTO(Intercambio, SMART_ELEMENTO_INTERCAMBIO) }
	catch (const std::exception&erro) { throw std::invalid_argument("Intercambio::Intercambio(): \n" + std::string(erro.what())); }
}

Intercambio::Intercambio(const Intercambio &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(Intercambio, SMART_ELEMENTO_INTERCAMBIO) }
	catch (const std::exception&erro) { throw std::invalid_argument("Intercambio::Intercambio(const Intercambio &instanciaCopiar): \n" + std::string(erro.what())); }
}

void Intercambio::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(Intercambio, SMART_ELEMENTO_INTERCAMBIO) }
	catch (const std::exception& erro) { throw std::invalid_argument("Intercambio::esvaziar(): \n" + std::string(erro.what())); }
}

Intercambio::~Intercambio(){ EXCLUI_SMART_ELEMENTO(Intercambio, SMART_ELEMENTO_INTERCAMBIO) }

void Intercambio::validacaoDadosAttComum(){
	try {
		if (getAtributo(AttComumIntercambio_submercado_origem, IdSubmercado()) == getAtributo(AttComumIntercambio_submercado_destino, IdSubmercado()))
			throw std::invalid_argument("Submercado Origem igual ao Submercado Destino em Intercambio " + getString(getIdObjeto()) + ".");

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("Dados::validacaoDadosAttComum(): \n" + std::string(erro.what())); }
} // void Intercambio::validacaoDadosAttComum(){
