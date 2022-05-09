#include "C_Iteracao.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Iteracao::Iteracao() {
	try { INICIALIZA_SMART_ELEMENTO(Iteracao, SMART_ELEMENTO_ITERACAO) }
	catch (const std::exception&erro) { throw std::invalid_argument("Iteracao::Iteracao(): \n" + std::string(erro.what())); }
}

Iteracao::Iteracao(const Iteracao &instanciaCopiar) {
	try { COPIA_SMART_ELEMENTO(Iteracao, SMART_ELEMENTO_ITERACAO) }
	catch (const std::exception&erro) { throw std::invalid_argument("Iteracao::Iteracao(const Iteracao &instanciaCopiar): \n" + std::string(erro.what())); }
}

void Iteracao::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(Iteracao, SMART_ELEMENTO_ITERACAO) }
	catch (const std::exception& erro) { throw std::invalid_argument("Iteracao::esvaziar(): \n" + std::string(erro.what())); }
}

Iteracao::~Iteracao() { EXCLUI_SMART_ELEMENTO(Iteracao, SMART_ELEMENTO_ITERACAO) }

void Iteracao::setAberturasInicialFinalMapeamento(const IdProcesso a_processo, const IdCenario a_cenario, const IdEstagio a_estagio, const IdAbertura a_abertura_inicial, const IdAbertura a_abertura_final) {
	try {

		vetorProcesso.att(a_processo).setAberturasInicialFinalMapeamento(a_cenario, a_estagio, a_abertura_inicial, a_abertura_final);

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Processo::setAberturasInicialFinalMapeamento(" + getFullString(a_cenario) + "," + getFullString(a_estagio) + getFullString(a_abertura_inicial) + "," + getFullString(a_abertura_final) + "): \n" + std::string(erro.what())); }
}

