#include "C_ElementoSistema.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ElementoSistema::ElementoSistema(){
	try { INICIALIZA_SMART_ELEMENTO(ElementoSistema, SMART_ELEMENTO_ELEMENTO_SISTEMA)	}
	catch (const std::exception&erro) { throw std::invalid_argument("ElementoSistema(" + getString(getIdObjeto()) + ")::ElementoSistema(): \n" + std::string(erro.what())); }
}

ElementoSistema::ElementoSistema(const ElementoSistema &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(ElementoSistema, SMART_ELEMENTO_ELEMENTO_SISTEMA) }
	catch (const std::exception&erro) { throw std::invalid_argument("ElementoSistema(" + getString(getIdObjeto()) + ")::ElementoSistema(const ElementoSistema &instanciaCopiar): \n" + std::string(erro.what())); }
}

void ElementoSistema::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(ElementoSistema, SMART_ELEMENTO_ELEMENTO_SISTEMA) }
	catch (const std::exception& erro) { throw std::invalid_argument("ElementoSistema(" + getString(getIdObjeto()) + ")::esvaziar(): \n" + std::string(erro.what())); }
}


ElementoSistema::~ElementoSistema(){ EXCLUI_SMART_ELEMENTO(ElementoSistema, SMART_ELEMENTO_ELEMENTO_SISTEMA) }

void ElementoSistema::validar() {
	try {

		const IdHidreletrica       idUHE = getAtributo(AttComumElementoSistema_hidreletrica, IdHidreletrica());
		const IdConjuntoHidraulico idConUHE = getAtributo(AttComumElementoSistema_conjuntohidraulico, IdConjuntoHidraulico());
		const IdUnidadeUHE         idUniUHE = getAtributo(AttComumElementoSistema_unidadehidraulica, IdUnidadeUHE());
		const IdTermeletrica       idUTE = getAtributo(AttComumElementoSistema_termeletrica, IdTermeletrica());
		const IdUnidadeUTE         idUniUTE = getAtributo(AttComumElementoSistema_unidadeTermeletrica, IdUnidadeUTE());
		const IdIntercambio        idINT = getAtributo(AttComumElementoSistema_intercambio, IdIntercambio());
		const IdContrato           idCON = getAtributo(AttComumElementoSistema_contrato, IdContrato());
		const IdDemandaEspecial    idDES = getAtributo(AttComumElementoSistema_demandaEspecial, IdDemandaEspecial());
		const IdSubmercado         idDEM = getAtributo(AttComumElementoSistema_demanda, IdSubmercado());
		const IdSubmercado         idDEMLIQ = getAtributo(AttComumElementoSistema_demandaliquida, IdSubmercado());
		const IdRenovavel          idREN = getAtributo(AttComumElementoSistema_renovavel, IdRenovavel());
		const IdUsinaElevatoria    idELE = getAtributo(AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria());

		bool any_identified = false;

		if (idUHE != IdHidreletrica_Nenhum) {
			if ((idUniUHE != IdUnidadeUHE_Nenhum) && (idConUHE == IdConjuntoHidraulico_Nenhum))
				throw std::invalid_argument("Invalid UHE members.");
			any_identified = true;
		}
		else if ((idUniUHE != IdUnidadeUHE_Nenhum) || (idConUHE != IdConjuntoHidraulico_Nenhum))
			throw std::invalid_argument("Invalid UHE.");

		if (idUTE != IdTermeletrica_Nenhum) {
			if (any_identified)
				throw std::invalid_argument(getFullString(idUTE) + " is multiple element.");
			any_identified = true;
		}
		else if (idUniUTE != IdUnidadeUTE_Nenhum)
			throw std::invalid_argument(getFullString(idUniUTE) + " is invalid.");

		if (idINT != IdIntercambio_Nenhum) {
			if (any_identified)
				throw std::invalid_argument(getFullString(idINT) + " is multiple element.");
			any_identified = true;
		}

		if (idCON != IdContrato_Nenhum) {
			if (any_identified)
				throw std::invalid_argument(getFullString(idCON) + " is multiple element.");
			any_identified = true;
		}

		if (idELE != IdUsinaElevatoria_Nenhum) {
			if (any_identified)
				throw std::invalid_argument(getFullString(idELE) + " is multiple element.");
			any_identified = true;
		}

		if (idDES != IdDemandaEspecial_Nenhum) {
			if (any_identified)
				throw std::invalid_argument(getFullString(idDES) + " is multiple element.");
			any_identified = true;
		}

		if (idDEM != IdSubmercado_Nenhum) {
			if (any_identified)
				throw std::invalid_argument(getFullString(idDEM) + " is multiple element.");
			any_identified = true;
		}

		if (idDEMLIQ != IdSubmercado_Nenhum) {
			if (any_identified)
				throw std::invalid_argument(getFullString(idDEMLIQ) + " is multiple element.");
			any_identified = true;
		}

		if (idREN != IdRenovavel_Nenhum) {
			if (any_identified)
				throw std::invalid_argument(getFullString(idREN) + " is multiple element.");
			any_identified = true;
		}

	}
	catch (const std::exception& erro) { throw std::invalid_argument("ElementoSistema(" + getString(getIdObjeto()) + ")::validar(): \n" + std::string(erro.what())); }
}
