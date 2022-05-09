#include "C_Processo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Processo::Processo() {
	try { INICIALIZA_SMART_ELEMENTO(Processo, SMART_ELEMENTO_PROCESSO) }
	catch (const std::exception&erro) { throw std::invalid_argument("Processo::Processo(): \n" + std::string(erro.what())); }
}

Processo::Processo(const Processo &instanciaCopiar) {
	try { COPIA_SMART_ELEMENTO(Processo, SMART_ELEMENTO_PROCESSO) }
	catch (const std::exception&erro) { throw std::invalid_argument("Processo::Processo(const Processo &instanciaCopiar): \n" + std::string(erro.what())); }
}

void Processo::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(Processo, SMART_ELEMENTO_PROCESSO) }
	catch (const std::exception& erro) { throw std::invalid_argument("Processo::esvaziar(): \n" + std::string(erro.what())); }
}

Processo::~Processo() { EXCLUI_SMART_ELEMENTO(Processo, SMART_ELEMENTO_PROCESSO) }

void Processo::setAberturasInicialFinalMapeamento(const IdCenario a_cenario, const IdEstagio a_estagio, const IdAbertura a_abertura_inicial, const IdAbertura a_abertura_final) {
	try {
		
		setElemento(AttMatrizProcesso_mapeamento_abertura_inicial_por_cenario_por_estagio, a_cenario, a_estagio, a_abertura_inicial);
		setElemento(AttMatrizProcesso_mapeamento_abertura_final_por_cenario_por_estagio, a_cenario, a_estagio, a_abertura_final);

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Processo::setAberturasInicialFinalMapeamento(" + getFullString(a_cenario) + "," + getFullString(a_estagio) + getFullString(a_abertura_inicial) + "," + getFullString(a_abertura_final) + "): \n" + std::string(erro.what())); }
} // void Processo::setAberturasInicialFinalMapeamento(const IdCenario a_cenario, const IdEstagio a_estagio, const IdAbertura a_abertura_inicial, const IdAbertura a_abertura_final) {

