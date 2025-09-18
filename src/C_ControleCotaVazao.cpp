#include "C_ControleCotaVazao.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ControleCotaVazao::ControleCotaVazao() {
	try { INICIALIZA_SMART_ELEMENTO(ControleCotaVazao, SMART_ELEMENTO_CONTROLE_COTA_VAZAO) }
	catch (const std::exception&erro) { throw std::invalid_argument("ControleCotaVazao::ControleCotaVazao(): \n" + std::string(erro.what())); }
}

ControleCotaVazao::ControleCotaVazao(const ControleCotaVazao &instanciaCopiar) {
	try { COPIA_SMART_ELEMENTO(ControleCotaVazao, SMART_ELEMENTO_CONTROLE_COTA_VAZAO) }
	catch (const std::exception&erro) { throw std::invalid_argument("ControleCotaVazao::ControleCotaVazao(const ControleCotaVazao &instanciaCopiar): \n" + std::string(erro.what())); }
}

void ControleCotaVazao::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(ControleCotaVazao, SMART_ELEMENTO_CONTROLE_COTA_VAZAO) }
	catch (const std::exception& erro) { throw std::invalid_argument("ControleCotaVazao::esvaziar(): \n" + std::string(erro.what())); }
}

ControleCotaVazao::~ControleCotaVazao() { EXCLUI_SMART_ELEMENTO(ControleCotaVazao, SMART_ELEMENTO_CONTROLE_COTA_VAZAO) }

