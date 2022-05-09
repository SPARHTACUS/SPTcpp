#include "C_DiscretizacaoTemporal.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Discretizacao::Discretizacao(){
	try {
		INICIALIZA_SMART_ELEMENTO(Discretizacao, SMART_ELEMENTO_DISCRETIZACAO_TEMPORAL)

	}
	catch (const std::exception&erro) { throw std::invalid_argument("Discretizacao::Discretizacao(): \n" + std::string(erro.what())); }
}

Discretizacao::Discretizacao(const Discretizacao &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(Discretizacao, SMART_ELEMENTO_DISCRETIZACAO_TEMPORAL) }
	catch (const std::exception&erro) { throw std::invalid_argument("Discretizacao::Discretizacao(const Discretizacao &instanciaCopiar): \n" + std::string(erro.what())); }
}

void Discretizacao::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(Discretizacao, SMART_ELEMENTO_DISCRETIZACAO_TEMPORAL) }
	catch (const std::exception& erro) { throw std::invalid_argument("Discretizacao::esvaziar(): \n" + std::string(erro.what())); }
}

Discretizacao::~Discretizacao(){
	EXCLUI_SMART_ELEMENTO(Discretizacao, SMART_ELEMENTO_DISCRETIZACAO_TEMPORAL)
}
