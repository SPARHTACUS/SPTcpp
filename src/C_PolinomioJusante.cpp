#include "C_PolinomioJusante.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PolinomioJusante::PolinomioJusante() {
	try { INICIALIZA_SMART_ELEMENTO(PolinomioJusante, SMART_ELEMENTO_POLINOMIO_JUSANTE) }
	catch (const std::exception&erro) { throw std::invalid_argument("PolinomioJusante::PolinomioJusante(): \n" + std::string(erro.what())); }
}

PolinomioJusante::PolinomioJusante(const PolinomioJusante & instanciaCopiar) {
	try { COPIA_SMART_ELEMENTO(PolinomioJusante, SMART_ELEMENTO_POLINOMIO_JUSANTE) }
	catch (const std::exception&erro) { throw std::invalid_argument("PolinomioJusante::PolinomioJusante(const PolinomioJusante &instanciaCopiar): \n" + std::string(erro.what())); }
}

void PolinomioJusante::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(PolinomioJusante, SMART_ELEMENTO_POLINOMIO_JUSANTE) }
	catch (const std::exception& erro) { throw std::invalid_argument("PolinomioJusante::esvaziar(): \n" + std::string(erro.what())); }
}

PolinomioJusante::~PolinomioJusante() {
	EXCLUI_SMART_ELEMENTO(PolinomioJusante, SMART_ELEMENTO_POLINOMIO_JUSANTE)
}
