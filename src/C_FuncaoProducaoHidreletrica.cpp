#include "C_FuncaoProducaoHidreletrica.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FuncaoProducaoHidreletrica::FuncaoProducaoHidreletrica(){
	try { INICIALIZA_SMART_ELEMENTO(FuncaoProducaoHidreletrica, SMART_ELEMENTO_FUNCAO_PRODUCAO_HIDRELETRICA) }
	catch (const std::exception&erro) { throw std::invalid_argument("FuncaoProducaoHidreletrica::FuncaoProducaoHidreletrica(): \n" + std::string(erro.what())); }
}

FuncaoProducaoHidreletrica::FuncaoProducaoHidreletrica(const FuncaoProducaoHidreletrica &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(FuncaoProducaoHidreletrica, SMART_ELEMENTO_FUNCAO_PRODUCAO_HIDRELETRICA) }
	catch (const std::exception&erro) { throw std::invalid_argument("FuncaoProducaoHidreletrica::FuncaoProducaoHidreletrica(const FuncaoProducaoHidreletrica &instanciaCopiar): \n" + std::string(erro.what())); }
}

void FuncaoProducaoHidreletrica::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(FuncaoProducaoHidreletrica, SMART_ELEMENTO_FUNCAO_PRODUCAO_HIDRELETRICA) }
	catch (const std::exception& erro) { throw std::invalid_argument("FuncaoProducaoHidreletrica::esvaziar(const FuncaoProducaoHidreletrica &instanciaCopiar): \n" + std::string(erro.what())); }
}

FuncaoProducaoHidreletrica::~FuncaoProducaoHidreletrica(){ EXCLUI_SMART_ELEMENTO(FuncaoProducaoHidreletrica, SMART_ELEMENTO_FUNCAO_PRODUCAO_HIDRELETRICA) }
