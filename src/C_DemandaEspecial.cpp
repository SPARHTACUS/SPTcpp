#include "C_DemandaEspecial.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DemandaEspecial::DemandaEspecial() {
	try {
		INICIALIZA_SMART_ELEMENTO(DemandaEspecial, SMART_ELEMENTO_DEMANDA_ESPECIAL)

	}
	catch (const std::exception&erro) { throw std::invalid_argument("DemandaEspecial::DemandaEspecial(): \n" + std::string(erro.what())); }
}

DemandaEspecial::DemandaEspecial(const DemandaEspecial &instanciaCopiar) {
	try { COPIA_SMART_ELEMENTO(DemandaEspecial, SMART_ELEMENTO_DEMANDA_ESPECIAL) }
	catch (const std::exception&erro) { throw std::invalid_argument("DemandaEspecial::DemandaEspecial(const DemandaEspecial &instanciaCopiar): \n" + std::string(erro.what())); }
}

void DemandaEspecial::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(DemandaEspecial, SMART_ELEMENTO_DEMANDA_ESPECIAL) }
	catch (const std::exception& erro) { throw std::invalid_argument("DemandaEspecial::esvaziar(): \n" + std::string(erro.what())); }
}

DemandaEspecial::~DemandaEspecial() {
	EXCLUI_SMART_ELEMENTO(DemandaEspecial, SMART_ELEMENTO_DEMANDA_ESPECIAL)
}
