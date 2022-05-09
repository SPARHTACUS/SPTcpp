#include "C_BaciaHidrografica.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BaciaHidrografica::BaciaHidrografica(){
	try { INICIALIZA_SMART_ELEMENTO(BaciaHidrografica, SMART_ELEMENTO_BACIA_HIDROGRAFICA) }
	catch (const std::exception&erro) { throw std::invalid_argument("BaciaHidrografica::BaciaHidrografica(): \n" + std::string(erro.what())); }
}

BaciaHidrografica::BaciaHidrografica(const BaciaHidrografica &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(BaciaHidrografica, SMART_ELEMENTO_BACIA_HIDROGRAFICA) }
	catch (const std::exception&erro) { throw std::invalid_argument("BaciaHidrografica::BaciaHidrografica(const BaciaHidrografica &instanciaCopiar): \n" + std::string(erro.what())); }
}

void BaciaHidrografica::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(BaciaHidrografica, SMART_ELEMENTO_BACIA_HIDROGRAFICA) }
	catch (const std::exception& erro) { throw std::invalid_argument("BaciaHidrografica::esvaziar(): \n" + std::string(erro.what())); }
}

BaciaHidrografica::~BaciaHidrografica(){ EXCLUI_SMART_ELEMENTO(BaciaHidrografica, SMART_ELEMENTO_BACIA_HIDROGRAFICA) }
