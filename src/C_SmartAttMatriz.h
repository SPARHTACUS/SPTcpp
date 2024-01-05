#ifndef C_SMART_ATT_MATRIZ_H
#define C_SMART_ATT_MATRIZ_H

#include "C_Auxiliar.h"
#include "C_SmartEnum.h"
#include "C_Periodo.h"

#include "C_SmartEnupla.h"

#include <vector>
#include <string>
#include <algorithm>

typedef std::string string;



// ----------------------------------------------
// 
// Macros para criação de SMART ATRIBUTO AttMatriz 
//
// ----------------------------------------------


#define DEFINE_ATRIBUTO_SMART_ATRIBUTO_MATRIZ(Classe, SmartAtributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, TipoIterador1, TipoIterador2) SmartAtributo##Classe##_##nomeAtributo,
#define DEFINE_CHAR_SMART_ATRIBUTO_MATRIZ(Classe, SmartAtributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, TipoIterador1, TipoIterador2) #nomeAtributo,
#define DEFINE_CHARS_SMART_ATRIBUTO_MATRIZ(Classe, SmartAtributo, valores) static const char* SmartAtributo##Classe##Array [] = { "Nenhum", valores(DEFINE_CHAR_SMART_ATRIBUTO_MATRIZ) "Excedente" };

#define DEFINE_PRE_DATA_SMART_ATRIBUTO_MATRIZ(Classe, SmartAtributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, TipoIterador1, TipoIterador2) \
TipoValor nomeAtributo##_LB;\
TipoValor nomeAtributo##_UB;\
TipoValor nomeAtributo##_IV;

#define DEFINE_PRE_DATA_CONSTRUTOR_SMART_ATRIBUTO_MATRIZ(Classe, SmartAtributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, TipoIterador1, TipoIterador2) \
nomeAtributo##_LB = get##TipoValor##FromChar(#valorMinimo);\
nomeAtributo##_UB = get##TipoValor##FromChar(#valorMaximo);\
nomeAtributo##_IV = get##TipoValor##FromChar(#valorInicial);

#define DEFINE_ESTRUTURA_PRE_DATA_SMART_ATRIBUTO_MATRIZ(Classe, SmartAtributo, valores) \
struct SmartAtributo##Classe##PreDataStruct { \
	valores(DEFINE_PRE_DATA_SMART_ATRIBUTO_MATRIZ) \
	SmartAtributo##Classe##PreDataStruct(){ valores(DEFINE_PRE_DATA_CONSTRUTOR_SMART_ATRIBUTO_MATRIZ) }; \
};

#define DEFINE_SMART_AttMatriz(Classe, SmartAtributo, valores) \
enum SmartAtributo##Classe { SmartAtributo##Classe##_Nenhum, valores(DEFINE_ATRIBUTO_SMART_ATRIBUTO_MATRIZ) SmartAtributo##Classe##_Excedente }; \
DEFINE_CHARS_SMART_ATRIBUTO_MATRIZ(Classe, SmartAtributo, valores) \
DEFINE_ESTRUTURA_PRE_DATA_SMART_ATRIBUTO_MATRIZ(Classe, SmartAtributo, valores) \
DEFINE_GET_SMART_ENUM_FROM_CHAR(SmartAtributo##Classe) \
DEFINE_GET_STRING_SMART_ENUM(SmartAtributo##Classe) \
DEFINE_GET_FULL_STRING_SMART_ENUM(SmartAtributo##Classe) \
DEFINE_GET_STRING_TIPO_SMART_ENUM(SmartAtributo##Classe) \
DEFINE_OPERADOR_SMART_ENUM_MAIS_MAIS(SmartAtributo##Classe) \


// -------------------------------------------------------
//
// Macros para criação dos métodos de AttMatriz nas classes
//
// -------------------------------------------------------


#define DEFINE_METODOS_AttMatriz(Classe, Atributo) \
virtual void addDadoAttMatriz(const std::string a_iterador1, const std::string a_iterador2, const std::string a_atributo, const std::string a_vlrDado){  \
	addDadoAttMatriz(a_iterador1, a_iterador2, a_atributo, a_vlrDado, 1, 1);\
};\
virtual void addDadoAttMatriz(const std::string a_iterador1, const std::string a_iterador2, const std::string a_atributo, const std::string a_vlrDado, const int a_alocacao_1, const int a_alocacao_2){  \
	try { \
		const Atributo##Classe atributo = get##Atributo##Classe##FromChar(a_atributo.c_str()); \
		if ((a_iterador1 != "") && (a_iterador2 == "") && (a_vlrDado == "")) \
			addElementoFromString(atributo, a_iterador1, a_iterador2, a_vlrDado); \
		else if ((a_iterador1 == "") && (a_iterador2 == "") && (a_vlrDado == "")) \
			validacaoDadosAttMatriz(std::vector<std::string>{a_atributo}); \
		else if ((a_iterador1 == "") || (a_iterador2 == "") || (a_vlrDado == "")) \
			throw std::invalid_argument("Argumentos invalidos"); \
		else \
			addElementoFromString(atributo, a_iterador1, a_iterador2, a_vlrDado, a_alocacao_1, a_alocacao_2); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::addDadoAttMatriz(" + a_iterador1 + "," + a_iterador2 + "," + a_atributo + ","  + a_vlrDado + "): \n" + std::string(erro.what())); }\
}; \
void addElementoFromString(const Atributo##Classe a_tipoAtributo, const std::string a_iterador1, const std::string a_iterador2, const std::string a_valor){  \
	try { lista##Atributo##Classe.at(a_tipoAtributo)->addElementoFromString(a_iterador1, a_iterador2, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::addElementoFromString(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + a_iterador1 + "," + a_iterador2 + "," + a_valor + "): \n" + std::string(erro.what())); }\
}; \
void addElementoFromString(const Atributo##Classe a_tipoAtributo, const std::string a_iterador1, const std::string a_iterador2, const std::string a_valor, const int a_alocacao_1, const int a_alocacao_2){  \
	try { lista##Atributo##Classe.at(a_tipoAtributo)->addElementoFromString(a_iterador1, a_iterador2, a_valor, a_alocacao_1, a_alocacao_2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::addElementoFromString(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + a_iterador1 + "," + a_iterador2 + "," + a_valor + "): \n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttMatriz(const bool a_incluirCabecalho, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, bool a_incluirAtributo, const std::vector<std::string> a_atributo)const{  \
	try { \
		if (a_atributo.size() == 0) {  throw std::invalid_argument("Necessario informar um atributo AttMatriz."); } \
		else { \
			const int numAtributos = int(a_atributo.size()); \
			std::vector<std::vector<std::string>> dados; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			for (int i = 0; i < numAtributos; i++){ \
				const Atributo##Classe atributo = get##Atributo##Classe##FromChar(a_atributo.at(i).c_str()); \
				if (i == 0)\
					dados = getStringsFromElementos(a_incluirAtributo, atributo, a_incluirCabecalho, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final); \
				else \
					dados = getStringsFromElementos(a_incluirAtributo, atributo, false, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final); \
				matrizRetorno.insert(matrizRetorno.end(), dados.begin(), dados.end()); \
			} \
			return matrizRetorno; \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::getDadosAttMatriz(" + getString(a_incluirCabecalho) + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + "," + getString(a_incluirAtributo) + ",a_vetorAtributo): \n" + std::string(erro.what())); }\
}; \
std::vector<std::vector<std::string>> getStringsFromElementos(const bool a_incluirAtributo, const Atributo##Classe a_tipoAtributo, const bool a_incluirCabecalho, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final)const{  \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getStringsFromElementos(a_incluirAtributo, a_incluirCabecalho, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::getStringsFromElementos(" + getString(a_incluirAtributo) + "," + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_incluirCabecalho) + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + "): \n" + std::string(erro.what())); }\
}; \
void clear(const Atributo##Classe a_tipoAtributo){  \
	try { lista##Atributo##Classe.at(a_tipoAtributo)->clear();  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::clear(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "): \n" + std::string(erro.what())); }\
}; \
template<typename TipoIterador1, typename TipoIterador2, typename TipoValor> \
void addElemento(const Atributo##Classe a_tipoAtributo, const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2, const TipoValor  a_valor){  \
	try { lista##Atributo##Classe.at(a_tipoAtributo)->addElemento(a_iterador1, a_iterador2, a_valor);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::addElemento(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador1) + "," + getString(a_iterador2) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }\
}; \
template<typename TipoIterador1, typename TipoIterador2> \
void setElemento(const Atributo##Classe a_tipoAtributo, const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2){  \
	try { lista##Atributo##Classe.at(a_tipoAtributo)->setElemento(a_iterador1, a_iterador2);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::setElemento(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador1) + "," + getString(a_iterador2) + "): \n" + std::string(erro.what())); }\
}; \
template<typename TipoIterador1, typename TipoIterador2, typename TipoValor> \
void setElemento(const Atributo##Classe a_tipoAtributo, const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2, const TipoValor  a_valor){  \
	try { lista##Atributo##Classe.at(a_tipoAtributo)->setElemento(a_iterador1, a_iterador2, a_valor);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::setElemento(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador1) + "," + getString(a_iterador2) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }\
}; \
template<typename TipoIterador1, typename TipoIterador2, typename TipoValor> \
void setElementos(const Atributo##Classe a_tipoAtributo, const TipoIterador1 a_iterador1, const SmartEnupla<TipoIterador2, TipoValor> &a_valores){  \
	try { lista##Atributo##Classe.at(a_tipoAtributo)->setElementos(a_iterador1, a_valores);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::setElementos(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador1) + ",a_valores): \n" + std::string(erro.what())); }\
}; \
template<typename TipoIterador1> \
TipoIterador1 getIterador1Inicial(const Atributo##Classe a_tipoAtributo, const TipoIterador1 a_iterador1)const{  \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getIterador1Inicial(a_iterador1);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::getIterador1Inicial(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + ",TipoIterador1): \n" + std::string(erro.what())); }\
}; \
template<typename TipoIterador1> \
TipoIterador1 getIterador1Final(const Atributo##Classe a_tipoAtributo, const TipoIterador1 a_iterador1)const{  \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getIterador1Final(a_iterador1);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::getIterador1Final(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + ",TipoIterador1): \n" + std::string(erro.what())); }\
}; \
template<typename TipoIterador1, typename TipoIterador2> \
TipoIterador2 getIterador2Inicial(const Atributo##Classe a_tipoAtributo, const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2)const{  \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getIterador2Inicial(a_iterador1,a_iterador2);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::getIterador2Inicial(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador1) + ",TipoIterador2): \n" + std::string(erro.what())); }\
}; \
template<typename TipoIterador1, typename TipoIterador2> \
TipoIterador2 getIterador2Final(const Atributo##Classe a_tipoAtributo, const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2)const{  \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getIterador2Final(a_iterador1,a_iterador2);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::getIterador2Final(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador1) + ",TipoIterador2): \n" + std::string(erro.what())); }\
}; \
template<typename TipoIterador1, typename TipoIterador2, typename TipoValor> \
TipoValor getElementoMatriz(const Atributo##Classe a_tipoAtributo, const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2, const TipoValor  a_valor)const{  \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getElementoMatriz(a_iterador1, a_iterador2, a_valor);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::getElementoMatriz(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador1) + "," + getString(a_iterador2) + ",TipoValor): \n" + std::string(erro.what())); }\
}; \
template<typename TipoIterador1, typename TipoIterador2, typename TipoValor> \
SmartEnupla<TipoIterador2, TipoValor> getElementosMatriz(const Atributo##Classe a_tipoAtributo, const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2, const TipoValor a_valor)const{  \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getElementosMatriz(a_iterador1, a_iterador2, a_valor);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::getElementosMatriz(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador1) + ",TipoIterador2,TipoValor): \n" + std::string(erro.what())); }\
}; \
template<typename TipoIterador1, typename TipoIterador2, typename TipoValor> \
TipoValor* getReferenciaElementosMatriz(const Atributo##Classe a_tipoAtributo, const TipoIterador1 a_iterador1, const TipoIterador2 a_iter2_ini, const TipoIterador2 a_iter2_fim, const TipoValor a_valor){  \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getReferenciaElementosMatriz(a_iterador1, a_iter2_ini, a_iter2_fim, a_valor);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::getReferenciaElementosMatriz(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador1) + ",TipoIterador2,TipoValor): \n" + std::string(erro.what())); }\
}; \
int getSizeMatriz(const Atributo##Classe a_tipoAtributo) const {return lista##Atributo##Classe.at(a_tipoAtributo)->getSizeMatriz(); }; \
template<typename TipoIterador1> \
int getSizeMatriz(const Atributo##Classe a_tipoAtributo, const TipoIterador1 a_iterador1) const {return lista##Atributo##Classe.at(a_tipoAtributo)->getSizeMatriz(a_iterador1); }; \
template<typename TipoIterador1, typename TipoIterador2, typename TipoValor> \
SmartEnupla<TipoIterador1,SmartEnupla<TipoIterador2,TipoValor>> getMatriz(const Atributo##Classe a_tipoAtributo, const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2, const TipoValor a_valor) const{ \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getMatriz(a_iterador1, a_iterador2, a_valor);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::getMatriz(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + ",TipoIterador1,TipoIterador2,TipoValor): \n" + std::string(erro.what())); }\
}; \
template<typename TipoIterador1, typename TipoIterador2, typename TipoValor> \
bool setMatriz(const Atributo##Classe a_tipoAtributo, const SmartEnupla<TipoIterador1, SmartEnupla<TipoIterador2, TipoValor>> &a_matriz) { \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->setMatriz(a_matriz);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::setMatriz(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + ",a_matriz): \n" + std::string(erro.what())); }\
}; \
template<typename TipoIterador1, typename TipoIterador2, typename TipoValor> \
bool setMatriz_forced(const Atributo##Classe a_tipoAtributo, const SmartEnupla<TipoIterador1, SmartEnupla<TipoIterador2, TipoValor>> & a_matriz) {	\
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->setMatriz_forced(a_matriz);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::setMatriz(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + ",a_matriz): \n" + std::string(erro.what())); }\
};

// ------------------------------------------------------------------
//
// Macros para Declaração de Structs com Métodos Virtuais de AttMatriz
//
// ------------------------------------------------------------------


#define DECLARA_STRUCT_AttMatriz_DERIVADA(Classe, Atributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, TipoIterador1, TipoIterador2) \
struct Atributo##Classe##_##nomeAtributo##_Struct: Atributo##Classe##_Struct{ \
	virtual bool addElemento(const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2, const TipoValor a_valor) { \
		 return addElemento(a_iterador1, a_iterador2, a_valor, 1, 1); \
	}; \
	virtual bool addElemento(const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2, const TipoValor a_valor, const int a_alocacao_1, const int a_alocacao_2) { \
		try{ \
			if (a_valor < Atributo##Classe##PreData.nomeAtributo##_LB) { throw std::invalid_argument("Valor inferior ao valor minimo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else if (a_valor > Atributo##Classe##PreData.nomeAtributo##_UB) { throw std::invalid_argument("Valor superior ao valor maximo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else { \
				if (dados.size() == 0){ \
					dados = SmartEnupla<TipoIterador1, SmartEnupla<TipoIterador2, TipoValor>>(a_alocacao_1); \
					SmartEnupla <TipoIterador2, TipoValor> dados_linha(a_alocacao_2); \
					dados_linha.addElemento(a_iterador2, a_valor); \
					dados.addElemento(a_iterador1, dados_linha); \
				} \
				else {\
					const TipoIterador1 iteradorFinal = dados.getIteradorFinal(); \
					if (a_iterador1 <= iteradorFinal) {\
						if (dados.at(a_iterador1).size() == 0) \
							dados.at(a_iterador1) = SmartEnupla <TipoIterador2, TipoValor>(a_alocacao_2);\
						dados.at(a_iterador1).addElemento(a_iterador2, a_valor); \
					} \
					else { \
						SmartEnupla <TipoIterador2, TipoValor> dados_linha(a_alocacao_2); \
						dados_linha.addElemento(a_iterador2, a_valor); \
						dados.addElemento(a_iterador1, dados_linha); \
					} \
				} \
			} \
			return true;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::addElemento(" + getString(a_iterador1) + "," + getString(a_iterador2) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }\
	};\
	virtual bool clear() { \
		try{ \
			dados = SmartEnupla<TipoIterador1, SmartEnupla<TipoIterador2, TipoValor>>(); \
			return true;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::clear(): \n" + std::string(erro.what())); }\
	};\
	virtual bool addElemento(const TipoIterador1 a_iterador1) { \
		try{ \
			dados.addElemento(a_iterador1, SmartEnupla <TipoIterador2, TipoValor>()); \
			return true;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::addElemento(" + getString(a_iterador1) + "): \n" + std::string(erro.what())); }\
	};\
	virtual bool addElementoFromString(const std::string a_iterador1, const std::string a_iterador2, const std::string a_valor) { \
		try{ \
			if ((a_iterador1 != "") && (a_iterador2 == "") && (a_valor == "")) \
				 addElemento(get##TipoIterador1##FromChar(a_iterador1.c_str())); \
			else { \
				if (strCompara(a_valor.c_str(), "NaN") || strCompara(a_valor.c_str(), "-nan(ind)")) { throw std::invalid_argument("Valor NaN."); }\
				const TipoValor valor = get##TipoValor##FromChar(a_valor.c_str());\
				if (valor < Atributo##Classe##PreData.nomeAtributo##_LB) { throw std::invalid_argument("Valor inferior ao valor minimo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
				else if (valor > Atributo##Classe##PreData.nomeAtributo##_UB) { throw std::invalid_argument("Valor superior ao valor maximo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
				else { addElemento(get##TipoIterador1##FromChar(a_iterador1.c_str()), get##TipoIterador2##FromChar(a_iterador2.c_str()), valor); } \
			} \
			return true;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::addElementoFromString(" + a_iterador1 + "," + a_iterador2 + "," + a_valor + "): \n" + std::string(erro.what())); }\
	};\
	virtual bool addElementoFromString(const std::string a_iterador1, const std::string a_iterador2, const std::string a_valor, const int a_alocacao_1, const int a_alocacao_2) { \
		try{ \
			if ((a_iterador1 != "") && (a_iterador2 == "") && (a_valor == "")) \
				 addElemento(get##TipoIterador1##FromChar(a_iterador1.c_str())); \
			else { \
				if (strCompara(a_valor.c_str(), "NaN") || strCompara(a_valor.c_str(), "-nan(ind)")) { throw std::invalid_argument("Valor NaN."); }\
				const TipoValor valor = get##TipoValor##FromChar(a_valor.c_str());\
				if (valor < Atributo##Classe##PreData.nomeAtributo##_LB) { throw std::invalid_argument("Valor inferior ao valor minimo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
				else if (valor > Atributo##Classe##PreData.nomeAtributo##_UB) { throw std::invalid_argument("Valor superior ao valor maximo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
				else { addElemento(get##TipoIterador1##FromChar(a_iterador1.c_str()), get##TipoIterador2##FromChar(a_iterador2.c_str()), valor, a_alocacao_1, a_alocacao_2); } \
			} \
			return true;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::addElementoFromString(" + a_iterador1 + "," + a_iterador2 + "," + a_valor + "): \n" + std::string(erro.what())); }\
	};\
	virtual std::vector<std::vector<std::string>> getStringsFromElementos(const bool a_incluirAtributo, const bool a_incluirCabecalho, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final) const { \
		try{ \
			TipoIterador1 iterador1Inicial; \
			TipoIterador1 iterador1Final; \
			SmartEnupla<TipoIterador1, bool> vetor_auxiliar_iter1; \
			const SmartEnupla<TipoIterador1,SmartEnupla<TipoIterador2,TipoValor>> matriz = getMatriz(TipoIterador1(), TipoIterador2(), TipoValor()); \
			if ((a_iterador1Inicial != "") || (a_iterador1Final != "")){ \
				iterador1Inicial = get##TipoIterador1##FromChar(a_iterador1Inicial.c_str()); \
				iterador1Final   = get##TipoIterador1##FromChar(a_iterador1Final.c_str()); \
				if (iterador1Inicial > iterador1Final) \
					throw std::invalid_argument("Iterador 1 inicial maior que final."); \
				if (getString(TipoIterador1()) == getString(Periodo())){ \
					if (getSizeMatriz() > 0){ \
						for (TipoIterador1 iter = iterador1Inicial; iter <= iterador1Final; matriz.incrementarIterador(iter)) \
							vetor_auxiliar_iter1.addElemento(iter, true); \
					} \
					else{ \
						std::vector<std::vector<std::string>> vetorRetorno; \
						if ((a_incluirAtributo) && (a_incluirCabecalho))	\
							vetorRetorno.push_back(std::vector<std::string>{std::string(#Atributo)}); \
						if (a_incluirAtributo) \
							vetorRetorno.push_back(std::vector<std::string>{std::string(#nomeAtributo)}); \
						return vetorRetorno; \
					} \
				}\
				else \
					vetor_auxiliar_iter1 = SmartEnupla<TipoIterador1, bool>(iterador1Inicial, std::vector<bool>(int(iterador1Final - iterador1Inicial) + 1, true)); \
			} \
			else if (getSizeMatriz() > 0){ \
				iterador1Inicial = getIterador1Inicial(TipoIterador1()); \
				iterador1Final   = getIterador1Final(TipoIterador1()); \
				if (getString(TipoIterador1()) == getString(Periodo())){ \
					for (TipoIterador1 iter = iterador1Inicial; iter <= iterador1Final; matriz.incrementarIterador(iter)) \
						vetor_auxiliar_iter1.addElemento(iter, true); \
				} \
				else \
					vetor_auxiliar_iter1 = SmartEnupla<TipoIterador1, bool>(iterador1Inicial, std::vector<bool>(int(iterador1Final - iterador1Inicial) + 1, true)); \
			} \
			else {\
				std::vector<std::vector<std::string>> vetorRetorno; \
				if ((a_incluirAtributo) && (a_incluirCabecalho))	\
					vetorRetorno.push_back(std::vector<std::string>{std::string(#Atributo)}); \
				if (a_incluirAtributo) \
					vetorRetorno.push_back(std::vector<std::string>{std::string(#nomeAtributo)}); \
				return vetorRetorno; \
			} \
			int espacoCabecalho = 0; \
			if (a_incluirCabecalho) \
				espacoCabecalho = 1; \
			TipoIterador2 iterador2Inicial = get##TipoIterador2##FromChar("min"); \
			TipoIterador2 iterador2Final = get##TipoIterador2##FromChar("min"); \
			bool iter2_Periodo = false; \
			SmartEnupla<TipoIterador2, bool> vetor_auxiliar_iter2; \
			if (getString(TipoIterador2()) == getString(Periodo())){ \
				iter2_Periodo = true; \
				TipoIterador1 iter1_primeiro; \
				for (TipoIterador1 iter = getIterador1Inicial(TipoIterador1()); iter <= getIterador1Final(TipoIterador1()); matriz.incrementarIterador(iter)){ \
					if (getSizeMatriz(iter) > 0){ \
						for (TipoIterador2 iter2 = getIterador2Inicial(iter, TipoIterador2()); iter2 <= getIterador2Final(iter, TipoIterador2()); matriz.at(iter).incrementarIterador(iter2)){ \
							if ((vetor_auxiliar_iter2.size() == 0) || (iter == iter1_primeiro)){ \
								vetor_auxiliar_iter2.addElemento(iter2, true); \
								iter1_primeiro = iter; \
							} \
							else{ \
								if (vetor_auxiliar_iter2.isProximoIterador(iter2)) \
									vetor_auxiliar_iter2.addElemento(iter2, true); \
								else {\
									bool iter2_found = false; \
									for (TipoIterador2 iter2_local = vetor_auxiliar_iter2.getIteradorInicial(); iter2_local <= vetor_auxiliar_iter2.getIteradorFinal(); vetor_auxiliar_iter2.incrementarIterador(iter2_local)) { \
										if (iter2_local == iter2){ \
											iter2_found = true; \
											break; \
										} \
									} \
									if (!iter2_found) \
										throw std::invalid_argument("Iterador 2 " + getString(iter2) + " em Iterador 1 " + getString(iter) + " nao compativel com dinamica de iteracao da matriz."); \
								} \
							} \
						} \
					} \
					if (vetor_auxiliar_iter2.size() == 0){ \
						std::vector<std::vector<std::string>> vetorRetorno; \
						if ((a_incluirAtributo) && (a_incluirCabecalho))	\
							vetorRetorno.push_back(std::vector<std::string>{std::string(#Atributo)}); \
						if (a_incluirAtributo) \
							vetorRetorno.push_back(std::vector<std::string>{std::string(#nomeAtributo)}); \
						return vetorRetorno; \
					} \
					else{ \
						iterador2Inicial = vetor_auxiliar_iter2.getIteradorInicial(); \
						iterador2Final   = vetor_auxiliar_iter2.getIteradorFinal(); \
					} \
				} \
			} \
			if ((a_iterador2Inicial != "") || (a_iterador2Final != "")){ \
				iterador2Inicial = get##TipoIterador2##FromChar(a_iterador2Inicial.c_str()); \
				iterador2Final   = get##TipoIterador2##FromChar(a_iterador2Final.c_str()); \
				if (iterador2Inicial > iterador2Final) \
					throw std::invalid_argument("Iterador 2 inicial maior que final."); \
				if (!iter2_Periodo) \
					vetor_auxiliar_iter2 = SmartEnupla<TipoIterador2, bool>(iterador2Inicial, std::vector<bool>(int(iterador2Final - iterador2Inicial) + 1, true)); \
				else{ \
					bool iter2Inicial_found = false; \
					bool iter2Final_found   = false; \
					for (TipoIterador2 iter2_local = vetor_auxiliar_iter2.getIteradorInicial(); iter2_local <= vetor_auxiliar_iter2.getIteradorFinal(); vetor_auxiliar_iter2.incrementarIterador(iter2_local)) { \
						if (iterador2Inicial == iter2_local) \
							iter2Inicial_found = true; \
						if (iterador2Final == iter2_local) \
							iter2Final_found = true; \
						if ((iter2Inicial_found) && (iter2Final_found)) \
							break; \
					} \
					if ((!iter2Inicial_found) || (!iter2Final_found)) \
						throw std::invalid_argument("Iteradores 2 Inicial " + getString(iterador2Inicial) + " e Final " + getString(iterador2Final) + " nao compativeis com dinamica de iteracao da matriz."); \
				} \
			} \
			else if (!iter2_Periodo) { \
				bool iter2Inicial_set = false; \
				bool iter2Final_set = false; \
				if (getSizeMatriz() > 0){ \
					for (TipoIterador1 iter1 = getIterador1Inicial(TipoIterador1()); iter1 <= getIterador1Final(TipoIterador1()); matriz.incrementarIterador(iter1)){ \
						if (getSizeMatriz(iter1) > 0) { \
							TipoIterador2 iterador2Inicial_local = getIterador2Inicial(iter1, TipoIterador2()); \
							TipoIterador2 iterador2Final_local   = getIterador2Final  (iter1, TipoIterador2()); \
							if ((iterador2Inicial_local < iterador2Inicial) || (!iter2Inicial_set)){ \
								iter2Inicial_set = true; \
								iterador2Inicial = iterador2Inicial_local; \
							} \
							if ((iterador2Final_local > iterador2Final) || (!iter2Final_set)) {\
								iter2Final_set = true; \
								iterador2Final = iterador2Final_local; \
							} \
						} \
					}\
					if ((iter2Inicial_set) && (iter2Final_set)) \
						vetor_auxiliar_iter2 = SmartEnupla<TipoIterador2, bool>(iterador2Inicial, std::vector<bool>(int(iterador2Final - iterador2Inicial) + 1, true)); \
				}\
				if ((!iter2Inicial_set) || (!iter2Final_set)){\
					std::vector<std::vector<std::string>> vetorRetorno; \
					if ((a_incluirAtributo) && (a_incluirCabecalho))	\
						vetorRetorno.push_back(std::vector<std::string>{std::string(#Atributo)}); \
					if (a_incluirAtributo) \
						vetorRetorno.push_back(std::vector<std::string>{std::string(#nomeAtributo)}); \
					return vetorRetorno; \
				} \
			} \
			int espacoIterador1 = 1; \
			int espacoAtributo = 0; \
			if (a_incluirAtributo) \
				espacoAtributo = 1; \
			std::vector<std::vector<std::string>> matrizRetorno(espacoCabecalho + vetor_auxiliar_iter1.size(), std::vector<std::string>(espacoAtributo + espacoIterador1 + vetor_auxiliar_iter2.size(), "")); \
			if (getSizeMatriz() > 0) {	\
				const TipoIterador1 iterador1Inicial_local = getIterador1Inicial(TipoIterador1()); \
				const TipoIterador1 iterador1Final_local   = getIterador1Final  (TipoIterador1()); \
				int i1 = 0; \
				for (TipoIterador1 iter1 = iterador1Inicial; iter1 <= iterador1Final; vetor_auxiliar_iter1.incrementarIterador(iter1)){ \
					if ((iter1 >= iterador1Inicial_local) && (iter1 <= iterador1Final_local)) { \
						if (getSizeMatriz(iter1) > 0) { \
							const TipoIterador2 iterador2Inicial_local = getIterador2Inicial(iter1, TipoIterador2()); \
							const TipoIterador2 iterador2Final_local   = getIterador2Final  (iter1, TipoIterador2()); \
							int i2 = 0; \
							for (TipoIterador2 iter2 = iterador2Inicial; iter2 <= iterador2Final; vetor_auxiliar_iter2.incrementarIterador(iter2)){ \
								if ((iter2 < iterador2Inicial_local) || (iter2 > iterador2Final_local)) \
									matrizRetorno.at(i1 + espacoCabecalho).at(i2 + espacoAtributo + espacoIterador1) = ""; \
								else \
									matrizRetorno.at(i1 + espacoCabecalho).at(i2 + espacoAtributo + espacoIterador1) = getString(dados.at(iter1).getElemento(iter2)); \
								i2++; \
							}\
						} \
					} \
					if (espacoAtributo > 0) \
						matrizRetorno.at(i1 + espacoCabecalho).at(0) = std::string(#nomeAtributo); \
					if (espacoIterador1 > 0) \
						matrizRetorno.at(i1 + espacoCabecalho).at(espacoAtributo) = getString(iter1); \
					if ((espacoCabecalho > 0) && (iter1 == iterador1Inicial)){ \
						int i2 = 0; \
						for (TipoIterador2 iter2 = iterador2Inicial; iter2 <= iterador2Final; vetor_auxiliar_iter2.incrementarIterador(iter2)) { \
							matrizRetorno.at(0).at(i2 + espacoAtributo + espacoIterador1) = getString(iter2); \
							i2++; \
						} \
					} \
					i1++; \
				}\
			} \
			if ((espacoAtributo > 0) && (espacoCabecalho > 0)) \
				matrizRetorno.at(0).at(0) = std::string(#Atributo); \
			if ((espacoIterador1 > 0) && (espacoCabecalho > 0)) \
				matrizRetorno.at(0).at(espacoAtributo) = "Iteradores"; \
			return matrizRetorno;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::getStringsFromElementos(" + getString(a_incluirAtributo) + "," + getString(a_incluirCabecalho) + "," + getString(a_iterador1Inicial) + "," + getString(a_iterador1Final) + "," + getString(a_iterador2Inicial) + "," + getString(a_iterador2Final) + "): \n" + std::string(erro.what())); }\
	};\
	virtual bool setElemento(const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2) { \
		try{ \
			const TipoValor valor = Atributo##Classe##PreData.nomeAtributo##_IV;\
			if (valor < Atributo##Classe##PreData.nomeAtributo##_LB) { throw std::invalid_argument("Valor inicial inferior ao valor minimo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else if (valor > Atributo##Classe##PreData.nomeAtributo##_UB) { throw std::invalid_argument("Valor inicial superior ao valor maximo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else { dados.at(a_iterador1).setElemento(a_iterador2, valor); } \
			return true;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::setElemento(" + getString(a_iterador1) + "): \n" + std::string(erro.what())); }\
	};\
	virtual bool setElemento(const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2, const TipoValor a_valor) { \
		try{ \
			if (a_valor < Atributo##Classe##PreData.nomeAtributo##_LB) { throw std::invalid_argument("Valor inferior ao valor minimo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else if (a_valor > Atributo##Classe##PreData.nomeAtributo##_UB) { throw std::invalid_argument("Valor superior ao valor maximo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else { dados.at(a_iterador1).setElemento(a_iterador2, a_valor); } \
			return true;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::setElemento(" + getString(a_iterador1) + "," + getString(a_iterador2) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }\
	};\
	virtual bool setElementos(const TipoIterador1 a_iterador1, const SmartEnupla<TipoIterador2, TipoValor> &a_valores) { \
		try{ \
			if (a_valores.size() == 0) { dados.at(a_iterador1) = a_valores; } \
			else { \
				const TipoIterador2 iteradorInicial = a_valores.getIteradorInicial(); \
				const TipoIterador2 iteradorFinal   = a_valores.getIteradorFinal(); \
				for (TipoIterador2 iterador = iteradorInicial; iterador <= iteradorFinal; iterador++) { \
					if (a_valores.at(iterador) < Atributo##Classe##PreData.nomeAtributo##_LB) { throw std::invalid_argument("Valor inferior ao valor minimo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
					else if (a_valores.at(iterador) > Atributo##Classe##PreData.nomeAtributo##_UB) { throw std::invalid_argument("Valor superior ao valor maximo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
				} \
				dados.at(a_iterador1) = a_valores; \
			} \
			return true;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::setElementos(" + getString(a_iterador1) + ",a_valores): \n" + std::string(erro.what())); }\
	};\
	virtual TipoValor getElementoMatriz(const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2, const TipoValor a_valor) const { \
		try{\
			return dados.at(a_iterador1).getElemento(a_iterador2); \
        }\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::getElementoMatriz(" + getString(a_iterador1) + "," + getString(a_iterador2) + ",TipoValor): \n" + std::string(erro.what())); }\
	};\
	virtual SmartEnupla<TipoIterador2,TipoValor> getElementosMatriz(const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2, const TipoValor a_valor)const { \
		try{\
			return dados.at(a_iterador1); \
        }\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::getElementoMatriz(" + getString(a_iterador1) + ",TipoIterador2,TipoValor): \n" + std::string(erro.what())); }\
	};\
	virtual TipoValor* getReferenciaElementosMatriz(const TipoIterador1 a_iter1, const TipoIterador2 a_iter2_ini, const TipoIterador2 a_iter2Fim, const TipoValor a_valor) { \
		try{\
			if (a_iter2_ini < dados.at(a_iter1).getIteradorInicial()) \
				throw std::invalid_argument("Iterador 2 inicial inferior a SmartEnupla.");\
			if (a_iter2Fim > dados.at(a_iter1).getIteradorFinal()) \
				throw std::invalid_argument("Iterador 2 final superior a SmartEnupla.");\
			const int indice = int(a_iter2_ini - dados.at(a_iter1).getIteradorInicial());\
			return &dados.at(a_iter1).getElementos_referencia()[indice]; \
        }\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::getReferenciaElementosMatriz(" + getString(a_iter1) + "," + getString(a_iter2_ini) + "," + getString(a_iter2Fim) + "TipoValor): \n" + std::string(erro.what())); }\
	};\
	virtual TipoIterador1 getIterador1Inicial(TipoIterador1 const a_iterador1) const { \
		try{\
			return dados.getIteradorInicial(); \
        }\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::getIterador1Inicial(TipoIterador1): \n" + std::string(erro.what())); }\
	};\
	virtual TipoIterador2 getIterador2Inicial(TipoIterador1 const a_iterador1, TipoIterador2 const a_iterador2) const { \
		try{\
			return dados.at(a_iterador1).getIteradorInicial(); \
        }\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::getIterador2Inicial(" + getString(a_iterador1) + ",TipoIterador2): \n" + std::string(erro.what())); }\
	};\
	virtual TipoIterador1 getIterador1Final(TipoIterador1 const a_iterador1) const { \
		try{\
			return dados.getIteradorFinal(); \
        }\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::getIterador1Final(TipoIterador1): \n" + std::string(erro.what())); }\
	};\
	virtual TipoIterador2 getIterador2Final(TipoIterador1 const a_iterador1, TipoIterador2 const a_iterador2) const { \
		try{\
			return dados.at(a_iterador1).getIteradorFinal(); \
        }\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::getIterador2Final(" + getString(a_iterador1) + ",TipoIterador2): \n" + std::string(erro.what())); }\
	};\
	virtual int getSizeMatriz()const{ return dados.size(); }\
	virtual int getSizeMatriz(TipoIterador1 const a_iterador1)const{ return dados.at(a_iterador1).size(); }\
	virtual SmartEnupla<TipoIterador1,SmartEnupla<TipoIterador2,TipoValor>> getMatriz(TipoIterador1 a_iterador1, TipoIterador2 a_iterador2, TipoValor a_valor) const { return dados; }; \
	virtual bool setMatriz(const SmartEnupla<TipoIterador1,SmartEnupla<TipoIterador2,TipoValor>> &a_matriz) { \
		if (a_matriz.size() > 0){ \
			const TipoIterador1 iterador1Inicial = a_matriz.getIteradorInicial(); \
			const TipoIterador1 iterador1Final   = a_matriz.getIteradorFinal(); \
			for (TipoIterador1 iter1 = iterador1Inicial; iter1 <= iterador1Final; a_matriz.incrementarIterador(iter1)){ \
				if (a_matriz.at(iter1).size() > 0){ \
					const TipoIterador2 iterador2Inicial = a_matriz.at(iter1).getIteradorInicial(); \
					const TipoIterador2 iterador2Final   = a_matriz.at(iter1).getIteradorFinal(); \
					for (TipoIterador2 iter2 = iterador2Inicial; iter2 <= iterador2Final; a_matriz.at(iter1).incrementarIterador(iter2)){ \
						TipoValor valor = a_matriz.at(iter1).at(iter2); \
						if (valor < Atributo##Classe##PreData.nomeAtributo##_LB) { throw std::invalid_argument("Valor inferior ao valor minimo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
						else if (valor > Atributo##Classe##PreData.nomeAtributo##_UB) { throw std::invalid_argument("Valor superior ao valor maximo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
					} \
				} \
			} \
		} \
		dados = a_matriz; \
		return true; \
	}; \
	virtual bool setMatriz_forced(const SmartEnupla<TipoIterador1,SmartEnupla<TipoIterador2,TipoValor>> &a_matriz) { \
		dados = a_matriz; \
		return true; \
	}; \
	~Atributo##Classe##_##nomeAtributo##_Struct(){ clear();	}\
	SmartEnupla<TipoIterador1, SmartEnupla<TipoIterador2, TipoValor>> dados;\
};


#define DECLARA_STRUCT_AttMatriz_NENHUM(Classe, Atributo) \
struct Atributo##Classe##_Nenhum_Struct: Atributo##Classe##_Struct{ \
	~Atributo##Classe##_Nenhum_Struct(){}; \
};



#define DECLARA_METODOS_AttMatriz(Classe, Atributo, valores) \
public: \
struct Atributo##Classe##_Struct{\
	Atributo##Classe##PreDataStruct Atributo##Classe##PreData; \
\
	virtual ~Atributo##Classe##_Struct(){}; \
\
	virtual bool addElementoFromString(const std::string a_iterador1, const std::string a_iterador2, const std::string a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributoFromString(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool addElementoFromString(const std::string a_iterador1, const std::string a_iterador2, const std::string a_valor, const int a_alocacao_1, const int a_alocacao_2) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributoFromString(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool clear() { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::clear(): \nEstrutura de dados nao definida."); return false; }; \
\
\
	virtual std::vector<std::vector<std::string>> getStringsFromElementos(const bool a_incluirAtributo, const bool a_incluirCabecalho, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final) const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
\
	virtual int getSizeMatriz()const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
\
	virtual int getSizeMatriz(const IdCenario                  a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int getSizeMatriz(const IdEstacao                  a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int getSizeMatriz(const IdRealizacao               a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int getSizeMatriz(const IdVariavelAleatoria        a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int getSizeMatriz(const IdVariavelAleatoriaInterna a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int getSizeMatriz(const Periodo                    a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int getSizeMatriz(const IdHidreletrica             a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int getSizeMatriz(const IdEstagio                  a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int getSizeMatriz(const IdIteracao                 a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int getSizeMatriz(const IdMes                      a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int getSizeMatriz(const int                        a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int getSizeMatriz(const IdProcesso                 a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
\
	virtual bool   addElemento(const IdCenario			 a_iterador1, const Periodo             a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdCenario			 a_iterador1, const Periodo             a_iterador2, const IdRealizacao        a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdCenario			 a_iterador1, const Periodo             a_iterador2, const IdNo                a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const Periodo        	 a_iterador1, const IdPatamarCarga      a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const Periodo        	 a_iterador1, const int                 a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const Periodo        	 a_iterador1, const IdCenario           a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdHidreletrica 	 a_iterador1, const Periodo             a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdEstacao			 a_iterador1, const int                 a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdEstacao			 a_iterador1, const IdVariavelAleatoria a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdVariavelAleatoria        a_iterador1, const IdEstacao           a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdVariavelAleatoriaInterna a_iterador1, const IdEstacao           a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdRealizacao		 a_iterador1, const IdVariavelAleatoria	a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdRealizacao		 a_iterador1, const IdVariavelEstado    a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdRealizacao		 a_iterador1, const int                 a_iterador2, const IdCorteBenders      a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdRealizacao		 a_iterador1, const int                 a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const int	       	     a_iterador1, const int                 a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const int	       	     a_iterador1, const IdRealizacao        a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const int	       	     a_iterador1, const IdCenario        a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdRealizacao		 a_iterador1, const int                 a_iterador2, const int                 a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdCenario			 a_iterador1, const IdEstagio		    a_iterador2, const IdNo                a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const Periodo			 a_iterador1, const IdRealizacao        a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const Periodo			 a_iterador1, const IdNo                a_iterador2, const IdRealizacao        a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const Periodo			 a_iterador1, const IdNo                a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const Periodo			 a_iterador1, const IdNo                a_iterador2, const IdNo                a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdEstagio			 a_iterador1, const Periodo             a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdEstagio			 a_iterador1, const Periodo             a_iterador2, const int                a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdEstagio			 a_iterador1, const IdRealizacao         a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdEstagio			 a_iterador1, const IdRealizacao         a_iterador2, const int              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdEstagio			 a_iterador1, const IdCenario           a_iterador2, const int              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdIteracao			 a_iterador1, const IdCenario           a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdIteracao			 a_iterador1, const IdRealizacao        a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdIteracao			 a_iterador1, const int                 a_iterador2, const IdCenario           a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdMes			     a_iterador1, const IdPatamarCarga      a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdCenario			 a_iterador1, const IdEstagio           a_iterador2, const IdAbertura              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdCenario			 a_iterador1, const IdEstagio           a_iterador2, const IdCenario              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdProcesso			 a_iterador1, const IdIteracao           a_iterador2, const IdCenario              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdProcesso			 a_iterador1, const IdCenario            a_iterador2, const double                a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
\
	virtual bool   setElemento(const IdCenario           a_iterador1, const Periodo             a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdCenario           a_iterador1, const Periodo             a_iterador2, const IdRealizacao   a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdCenario           a_iterador1, const Periodo             a_iterador2, const IdNo           a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const Periodo             a_iterador1, const IdPatamarCarga      a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const Periodo             a_iterador1, const int                 a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const Periodo             a_iterador1, const IdCenario           a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdHidreletrica      a_iterador1, const Periodo             a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdEstacao           a_iterador1, const int                 a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdEstacao           a_iterador1, const IdVariavelAleatoria a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdVariavelAleatoria        a_iterador1, const IdEstacao           a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdVariavelAleatoriaInterna a_iterador1, const IdEstacao           a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdRealizacao		 a_iterador1, const IdVariavelAleatoria	a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdRealizacao		 a_iterador1, const IdVariavelEstado	a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdRealizacao		 a_iterador1, const int             	a_iterador2, const IdCorteBenders a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdRealizacao		 a_iterador1, const int             	a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const int		         a_iterador1, const int             	a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const int		         a_iterador1, const IdRealizacao      	a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const int		         a_iterador1, const IdCenario      	a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdRealizacao		 a_iterador1, const int             	a_iterador2, const int            a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdCenario			 a_iterador1, const IdEstagio		    a_iterador2, const IdNo           a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const Periodo             a_iterador1, const IdRealizacao        a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const Periodo             a_iterador1, const IdNo                a_iterador2, const IdRealizacao   a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const Periodo             a_iterador1, const IdNo                a_iterador2, const IdNo           a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const Periodo             a_iterador1, const IdNo                a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdEstagio           a_iterador1, const Periodo             a_iterador2, const int         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdEstagio           a_iterador1, const Periodo             a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdEstagio           a_iterador1, const IdRealizacao         a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdEstagio           a_iterador1, const IdRealizacao         a_iterador2, const int         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdEstagio           a_iterador1, const IdCenario            a_iterador2, const int         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdIteracao          a_iterador1, const IdCenario           a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdIteracao          a_iterador1, const IdRealizacao        a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdIteracao          a_iterador1, const int                 a_iterador2, const IdCenario      a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdMes               a_iterador1, const IdPatamarCarga      a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdCenario			 a_iterador1, const IdEstagio           a_iterador2, const IdAbertura              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdCenario			 a_iterador1, const IdEstagio           a_iterador2, const IdCenario              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdProcesso			 a_iterador1, const IdIteracao           a_iterador2, const IdCenario              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdProcesso			 a_iterador1, const IdCenario            a_iterador2, const double                a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
\
	virtual bool   setElementos(const IdCenario           a_iterador1, const SmartEnupla<Periodo,             double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdCenario           a_iterador1, const SmartEnupla<Periodo,       IdRealizacao> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdCenario           a_iterador1, const SmartEnupla<Periodo,               IdNo> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const Periodo             a_iterador1, const SmartEnupla<IdPatamarCarga,      double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const Periodo             a_iterador1, const SmartEnupla<int,                 double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const Periodo             a_iterador1, const SmartEnupla<IdCenario,           double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdHidreletrica      a_iterador1, const SmartEnupla<Periodo,             double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdEstacao           a_iterador1, const SmartEnupla<int,                 double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdEstacao           a_iterador1, const SmartEnupla<IdVariavelAleatoria, double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdVariavelAleatoria        a_iterador1, const SmartEnupla<IdEstacao,           double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdVariavelAleatoriaInterna a_iterador1, const SmartEnupla<IdEstacao,           double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdRealizacao		  a_iterador1, const SmartEnupla<IdVariavelAleatoria, double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdRealizacao		  a_iterador1, const SmartEnupla<IdVariavelEstado,    double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdRealizacao		  a_iterador1, const SmartEnupla<int,         IdCorteBenders> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdRealizacao		  a_iterador1, const SmartEnupla<int,                 double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const int		          a_iterador1, const SmartEnupla<int,                 double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const int		          a_iterador1, const SmartEnupla<IdRealizacao,        double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const int		          a_iterador1, const SmartEnupla<IdCenario,        double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdRealizacao		  a_iterador1, const SmartEnupla<int,                    int> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdCenario			  a_iterador1, const SmartEnupla<IdEstagio,             IdNo> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const Periodo             a_iterador1, const SmartEnupla<IdRealizacao,        double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const Periodo             a_iterador1, const SmartEnupla<IdNo,          IdRealizacao> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const Periodo             a_iterador1, const SmartEnupla<IdNo,                  IdNo> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const Periodo             a_iterador1, const SmartEnupla<IdNo,                double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdEstagio           a_iterador1, const SmartEnupla<Periodo,             double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdEstagio           a_iterador1, const SmartEnupla<Periodo,             int> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdEstagio           a_iterador1, const SmartEnupla<IdRealizacao,         double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdEstagio           a_iterador1, const SmartEnupla<IdRealizacao,         int> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdEstagio           a_iterador1, const SmartEnupla<IdCenario,             int> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdIteracao          a_iterador1, const SmartEnupla<IdCenario,           double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdIteracao          a_iterador1, const SmartEnupla<IdRealizacao,        double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdIteracao          a_iterador1, const SmartEnupla<int,              IdCenario> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdMes               a_iterador1, const SmartEnupla<IdPatamarCarga,      double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdCenario			 a_iterador1, const SmartEnupla<IdEstagio,      IdAbertura> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdCenario			 a_iterador1, const SmartEnupla<IdEstagio,      IdCenario> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdProcesso			 a_iterador1, const SmartEnupla<IdIteracao,      IdCenario> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdProcesso			 a_iterador1, const SmartEnupla<IdCenario,          double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
\
	virtual double         getElementoMatriz(const IdCenario           a_iterador1, const Periodo             a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdRealizacao   getElementoMatriz(const IdCenario           a_iterador1, const Periodo             a_iterador2, const IdRealizacao     a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdNo           getElementoMatriz(const IdCenario           a_iterador1, const Periodo             a_iterador2, const IdNo             a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const Periodo             a_iterador1, const IdPatamarCarga      a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const Periodo             a_iterador1, const int                 a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const Periodo             a_iterador1, const IdCenario           a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const IdHidreletrica      a_iterador1, const Periodo             a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const IdEstacao           a_iterador1, const int                 a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const IdEstacao           a_iterador1, const IdVariavelAleatoria a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const IdVariavelAleatoria        a_iterador1, const IdEstacao    a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const IdVariavelAleatoriaInterna a_iterador1, const IdEstacao    a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const IdRealizacao        a_iterador1, const IdVariavelAleatoria a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const IdRealizacao        a_iterador1, const IdVariavelEstado    a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdCorteBenders getElementoMatriz(const IdRealizacao        a_iterador1, const int                 a_iterador2, const IdCorteBenders   a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const IdRealizacao        a_iterador1, const int                 a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const int                 a_iterador1, const int                 a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const int                 a_iterador1, const IdRealizacao        a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const int                 a_iterador1, const IdCenario        a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int            getElementoMatriz(const IdRealizacao        a_iterador1, const int                 a_iterador2, const int              a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdNo           getElementoMatriz(const IdCenario		   a_iterador1, const IdEstagio		      a_iterador2, const IdNo             a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const Periodo             a_iterador1, const IdRealizacao        a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdRealizacao   getElementoMatriz(const Periodo             a_iterador1, const IdNo                a_iterador2, const IdRealizacao     a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdNo           getElementoMatriz(const Periodo             a_iterador1, const IdNo                a_iterador2, const IdNo             a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const Periodo             a_iterador1, const IdNo                a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const IdEstagio           a_iterador1, const Periodo             a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int            getElementoMatriz(const IdEstagio           a_iterador1, const Periodo             a_iterador2, const int           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int            getElementoMatriz(const IdEstagio           a_iterador1, const IdRealizacao        a_iterador2, const int              a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const IdEstagio           a_iterador1, const IdRealizacao        a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int           getElementoMatriz(const IdEstagio           a_iterador1, const IdCenario           a_iterador2, const int             a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const IdIteracao          a_iterador1, const IdCenario           a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const IdIteracao          a_iterador1, const IdRealizacao        a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdCenario      getElementoMatriz(const IdIteracao          a_iterador1, const int                 a_iterador2, const IdCenario        a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const IdMes               a_iterador1, const IdPatamarCarga      a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdAbertura     getElementoMatriz(const IdCenario		   a_iterador1, const IdEstagio           a_iterador2, const IdAbertura       a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdCenario      getElementoMatriz(const IdCenario		   a_iterador1, const IdEstagio           a_iterador2, const IdCenario       a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdCenario      getElementoMatriz(const IdProcesso		   a_iterador1, const IdIteracao          a_iterador2, const IdCenario        a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const IdProcesso		   a_iterador1, const IdCenario           a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida.");}; \
\
\
	virtual IdCenario           getIterador1Inicial(const IdCenario           a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdEstacao           getIterador1Inicial(const IdEstacao           a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdVariavelAleatoria getIterador1Inicial(const IdVariavelAleatoria a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
    virtual IdVariavelAleatoriaInterna getIterador1Inicial(const IdVariavelAleatoriaInterna a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdRealizacao		getIterador1Inicial(const IdRealizacao		  a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual Periodo             getIterador1Inicial(const Periodo             a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdHidreletrica      getIterador1Inicial(const IdHidreletrica      a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdEstagio           getIterador1Inicial(const IdEstagio           a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdIteracao          getIterador1Inicial(const IdIteracao          a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdMes               getIterador1Inicial(const IdMes               a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int                 getIterador1Inicial(const int               a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdProcesso          getIterador1Inicial(const IdProcesso          a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
\
	virtual IdCenario           getIterador1Final  (const IdCenario           a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdEstacao           getIterador1Final  (const IdEstacao           a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdVariavelAleatoria getIterador1Final  (const IdVariavelAleatoria a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdVariavelAleatoriaInterna getIterador1Final  (const IdVariavelAleatoriaInterna a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdRealizacao		getIterador1Final  (const IdRealizacao		  a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual Periodo             getIterador1Final  (const Periodo             a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdHidreletrica      getIterador1Final  (const IdHidreletrica      a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdEstagio           getIterador1Final  (const IdEstagio           a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdIteracao          getIterador1Final  (const IdIteracao          a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdMes               getIterador1Final  (const IdMes               a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int                 getIterador1Final  (const int               a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdProcesso          getIterador1Final  (const IdProcesso          a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
\
	virtual Periodo             getIterador2Inicial(const IdCenario           a_iterador1, const Periodo             a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdPatamarCarga      getIterador2Inicial(const Periodo             a_iterador1, const IdPatamarCarga      a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int                 getIterador2Inicial(const Periodo             a_iterador1, const int                 a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdCenario           getIterador2Inicial(const Periodo             a_iterador1, const IdCenario           a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual Periodo             getIterador2Inicial(const IdHidreletrica      a_iterador1, const Periodo             a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int                 getIterador2Inicial(const IdEstacao           a_iterador1, const int                 a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdVariavelAleatoria getIterador2Inicial(const IdEstacao           a_iterador1, const IdVariavelAleatoria a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdEstacao           getIterador2Inicial(const IdVariavelAleatoria a_iterador1, const IdEstacao           a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdEstacao           getIterador2Inicial(const IdVariavelAleatoriaInterna a_iterador1, const IdEstacao    a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdVariavelAleatoria	getIterador2Inicial(const IdRealizacao		  a_iterador1, const IdVariavelAleatoria a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdVariavelEstado	getIterador2Inicial(const IdRealizacao		  a_iterador1, const IdVariavelEstado    a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int                 getIterador2Inicial(const IdRealizacao		  a_iterador1, const int                 a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int                 getIterador2Inicial(const IdCenario		  a_iterador1, const int                 a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdEstagio		    getIterador2Inicial(const IdCenario			  a_iterador1, const IdEstagio		     a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdRealizacao        getIterador2Inicial(const Periodo             a_iterador1, const IdRealizacao        a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdNo                getIterador2Inicial(const Periodo             a_iterador1, const IdNo                a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual Periodo             getIterador2Inicial(const IdEstagio           a_iterador1, const Periodo             a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdRealizacao        getIterador2Inicial(const IdEstagio           a_iterador1, const IdRealizacao        a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdRealizacao        getIterador2Inicial(const int                 a_iterador1, const IdRealizacao        a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdCenario           getIterador2Inicial(const IdEstagio           a_iterador1, const IdCenario           a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdCenario           getIterador2Inicial(const IdIteracao          a_iterador1, const IdCenario           a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdRealizacao        getIterador2Inicial(const IdIteracao          a_iterador1, const IdRealizacao        a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int                 getIterador2Inicial(const IdIteracao          a_iterador1, const int                 a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdPatamarCarga      getIterador2Inicial(const IdMes               a_iterador1, const IdPatamarCarga      a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdIteracao          getIterador2Inicial(const IdProcesso         a_iterador1, const IdIteracao           a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdCenario          getIterador2Inicial(const IdProcesso         a_iterador1, const IdCenario           a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
\
	virtual Periodo				getIterador2Final  (const IdCenario           a_iterador1, const Periodo             a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdPatamarCarga  	getIterador2Final  (const Periodo             a_iterador1, const IdPatamarCarga      a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int             	getIterador2Final  (const Periodo             a_iterador1, const int                 a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdCenario         	getIterador2Final  (const Periodo             a_iterador1, const IdCenario           a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual Periodo				getIterador2Final  (const IdHidreletrica      a_iterador1, const Periodo             a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int					getIterador2Final  (const IdEstacao           a_iterador1, const int                 a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdVariavelAleatoria getIterador2Final  (const IdEstacao           a_iterador1, const IdVariavelAleatoria a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdEstacao			getIterador2Final  (const IdVariavelAleatoria a_iterador1, const IdEstacao           a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdEstacao			getIterador2Final  (const IdVariavelAleatoriaInterna a_iterador1, const IdEstacao           a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdVariavelAleatoria	getIterador2Final  (const IdRealizacao		  a_iterador1, const IdVariavelAleatoria a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdVariavelEstado	getIterador2Final  (const IdRealizacao		  a_iterador1, const IdVariavelEstado    a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int                 getIterador2Final  (const IdRealizacao		  a_iterador1, const int                 a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int                 getIterador2Final  (const IdCenario		  a_iterador1, const int                 a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdEstagio			getIterador2Final  (const IdCenario			  a_iterador1, const IdEstagio		     a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdRealizacao        getIterador2Final  (const Periodo             a_iterador1, const IdRealizacao        a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdRealizacao        getIterador2Final  (const int                 a_iterador1, const IdRealizacao        a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdNo                getIterador2Final  (const Periodo             a_iterador1, const IdNo                a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual Periodo             getIterador2Final  (const IdEstagio           a_iterador1, const Periodo             a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdRealizacao        getIterador2Final  (const IdEstagio           a_iterador1, const IdRealizacao        a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdCenario           getIterador2Final  (const IdEstagio           a_iterador1, const IdCenario           a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdCenario           getIterador2Final  (const IdIteracao          a_iterador1, const IdCenario           a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdRealizacao        getIterador2Final  (const IdIteracao          a_iterador1, const IdRealizacao        a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int                 getIterador2Final  (const IdIteracao          a_iterador1, const int                 a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdPatamarCarga      getIterador2Final  (const IdMes               a_iterador1, const IdPatamarCarga      a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdIteracao           getIterador2Final  (const IdProcesso        a_iterador1, const IdIteracao           a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdCenario           getIterador2Final  (const IdProcesso        a_iterador1, const IdCenario           a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
\
	virtual SmartEnupla<Periodo,             double> getElementosMatriz(const IdCenario           a_iterador1, const Periodo             a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<Periodo,       IdRealizacao> getElementosMatriz(const IdCenario           a_iterador1, const Periodo             a_iterador2, const IdRealizacao   a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<Periodo,               IdNo> getElementosMatriz(const IdCenario           a_iterador1, const Periodo             a_iterador2, const IdNo           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdPatamarCarga,      double> getElementosMatriz(const Periodo             a_iterador1, const IdPatamarCarga      a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<int,                 double> getElementosMatriz(const Periodo             a_iterador1, const int                 a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdCenario,           double> getElementosMatriz(const Periodo             a_iterador1, const IdCenario           a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<Periodo,             double> getElementosMatriz(const IdHidreletrica      a_iterador1, const Periodo             a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<int,                 double> getElementosMatriz(const IdEstacao           a_iterador1, const int                 a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdVariavelAleatoria, double> getElementosMatriz(const IdEstacao           a_iterador1, const IdVariavelAleatoria a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdEstacao,           double> getElementosMatriz(const IdVariavelAleatoria a_iterador1, const IdEstacao           a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdEstacao,           double> getElementosMatriz(const IdVariavelAleatoriaInterna a_iterador1, const IdEstacao           a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdVariavelAleatoria, double> getElementosMatriz(const IdRealizacao		  a_iterador1, const IdVariavelAleatoria a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdVariavelEstado,    double> getElementosMatriz(const IdRealizacao		  a_iterador1, const IdVariavelEstado    a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<int,         IdCorteBenders> getElementosMatriz(const IdRealizacao		  a_iterador1, const int                 a_iterador2, const IdCorteBenders a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<int,                 double> getElementosMatriz(const IdRealizacao		  a_iterador1, const int                 a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<int,                 double> getElementosMatriz(const int		          a_iterador1, const int                 a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdRealizacao,        double> getElementosMatriz(const int		          a_iterador1, const IdRealizacao        a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdCenario,        double> getElementosMatriz(const int		          a_iterador1, const IdCenario        a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<int,                    int> getElementosMatriz(const IdRealizacao		  a_iterador1, const int                 a_iterador2, const int            a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdEstagio,              int> getElementosMatriz(const IdCenario			  a_iterador1, const IdEstagio           a_iterador2, const int            a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdRealizacao,        double> getElementosMatriz(const Periodo             a_iterador1, const IdRealizacao        a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdNo,          IdRealizacao> getElementosMatriz(const Periodo             a_iterador1, const IdNo                a_iterador2, const IdRealizacao   a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdNo,                   IdNo> getElementosMatriz(const Periodo             a_iterador1, const IdNo                a_iterador2, const IdNo   a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdNo,                 double> getElementosMatriz(const Periodo             a_iterador1, const IdNo                a_iterador2, const double a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<Periodo,             double> getElementosMatriz(const IdEstagio           a_iterador1, const Periodo             a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<Periodo,             int> getElementosMatriz(const IdEstagio           a_iterador1, const Periodo             a_iterador2, const int         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdRealizacao,         double> getElementosMatriz(const IdEstagio           a_iterador1, const IdRealizacao         a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdRealizacao,         int> getElementosMatriz(const IdEstagio           a_iterador1, const IdRealizacao         a_iterador2, const int         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdCenario,             int> getElementosMatriz(const IdEstagio           a_iterador1, const IdCenario           a_iterador2, const int           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdCenario,           double> getElementosMatriz(const IdIteracao          a_iterador1, const IdCenario           a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdRealizacao,        double> getElementosMatriz(const IdIteracao          a_iterador1, const IdRealizacao        a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<int,              IdCenario> getElementosMatriz(const IdIteracao          a_iterador1, const int                 a_iterador2, const IdCenario      a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdPatamarCarga,      double> getElementosMatriz(const IdMes               a_iterador1, const IdPatamarCarga      a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdEstagio,      IdAbertura> getElementosMatriz(const IdCenario               a_iterador1, const IdEstagio      a_iterador2, const IdAbertura         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdEstagio,      IdCenario> getElementosMatriz(const IdCenario               a_iterador1, const IdEstagio      a_iterador2, const IdCenario         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdIteracao,      IdCenario> getElementosMatriz(const IdProcesso               a_iterador1, const IdIteracao      a_iterador2, const IdCenario         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdCenario,      double> getElementosMatriz(const IdProcesso               a_iterador1, const IdCenario      a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
\
	virtual double*       getReferenciaElementosMatriz(const IdCenario           a_iterador1, const Periodo             a_iter2_ini, const Periodo a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdRealizacao* getReferenciaElementosMatriz(const IdCenario           a_iterador1, const Periodo             a_iter2_ini, const Periodo a_iter2_fim, const IdRealizacao   a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdNo*         getReferenciaElementosMatriz(const IdCenario           a_iterador1, const Periodo             a_iter2_ini, const Periodo a_iter2_fim, const IdNo           a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double* getReferenciaElementosMatriz(const Periodo             a_iterador1, const IdPatamarCarga      a_iter2_ini, const IdPatamarCarga a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double* getReferenciaElementosMatriz(const Periodo             a_iterador1, const int                 a_iter2_ini, const int a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double* getReferenciaElementosMatriz(const Periodo             a_iterador1, const IdCenario                 a_iter2_ini, const IdCenario a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double* getReferenciaElementosMatriz(const IdHidreletrica      a_iterador1, const Periodo             a_iter2_ini, const Periodo a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double* getReferenciaElementosMatriz(const IdEstacao           a_iterador1, const int                 a_iter2_ini, const int a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double* getReferenciaElementosMatriz(const IdEstacao           a_iterador1, const IdVariavelAleatoria a_iter2_ini, const IdVariavelAleatoria a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double* getReferenciaElementosMatriz(const IdVariavelAleatoria a_iterador1, const IdEstacao           a_iter2_ini, const IdEstacao a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double* getReferenciaElementosMatriz(const IdVariavelAleatoriaInterna a_iterador1, const IdEstacao           a_iter2_ini, const IdEstacao a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double* getReferenciaElementosMatriz(const IdRealizacao		  a_iterador1, const IdVariavelAleatoria a_iter2_ini, const IdVariavelAleatoria a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double* getReferenciaElementosMatriz(const IdRealizacao		  a_iterador1, const IdVariavelEstado    a_iter2_ini, const IdVariavelEstado a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdCorteBenders* getReferenciaElementosMatriz(const IdRealizacao		  a_iterador1, const int                 a_iter2_ini, const int a_iter2_fim, const IdCorteBenders a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double*         getReferenciaElementosMatriz(const IdRealizacao		  a_iterador1, const int                 a_iter2_ini, const int a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double*         getReferenciaElementosMatriz(const int		          a_iterador1, const int                 a_iter2_ini, const int a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double*         getReferenciaElementosMatriz(const int		          a_iterador1, const IdRealizacao        a_iter2_ini, const IdRealizacao a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double*         getReferenciaElementosMatriz(const int		          a_iterador1, const IdCenario        a_iter2_ini, const IdCenario a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual int*            getReferenciaElementosMatriz(const IdRealizacao		  a_iterador1, const int                 a_iter2_ini, const int a_iter2_fim, const int            a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual int*            getReferenciaElementosMatriz(const IdCenario			  a_iterador1, const IdEstagio           a_iter2_ini, const IdEstagio a_iter2_fim, const int            a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double*         getReferenciaElementosMatriz(const Periodo             a_iterador1, const IdRealizacao        a_iter2_ini, const IdRealizacao a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdRealizacao*   getReferenciaElementosMatriz(const Periodo             a_iterador1, const IdNo                a_iter2_ini, const IdNo a_iter2_fim, const IdRealizacao   a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdNo*   getReferenciaElementosMatriz(const Periodo             a_iterador1, const IdNo                a_iter2_ini, const IdNo a_iter2_fim, const IdNo   a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double* getReferenciaElementosMatriz(const Periodo             a_iterador1, const IdNo                a_iter2_ini, const IdNo a_iter2_fim, const double a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double* getReferenciaElementosMatriz(const IdEstagio           a_iterador1, const Periodo             a_iter2_ini, const Periodo a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual int* getReferenciaElementosMatriz(const IdEstagio           a_iterador1, const Periodo             a_iter2_ini, const Periodo a_iter2_fim, const int         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double* getReferenciaElementosMatriz(const IdEstagio           a_iterador1, const IdRealizacao         a_iter2_ini, const IdRealizacao a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual int*    getReferenciaElementosMatriz(const IdEstagio           a_iterador1, const IdRealizacao         a_iter2_ini, const IdRealizacao a_iter2_fim, const int         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual int*    getReferenciaElementosMatriz(const IdEstagio           a_iterador1, const IdCenario           a_iter2_ini, const IdCenario a_iter2_fim, const int           a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double* getReferenciaElementosMatriz(const IdIteracao          a_iterador1, const IdCenario           a_iter2_ini, const IdCenario a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double* getReferenciaElementosMatriz(const IdIteracao          a_iterador1, const IdRealizacao        a_iter2_ini, const IdRealizacao a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdCenario*  getReferenciaElementosMatriz(const IdIteracao          a_iterador1, const int                 a_iter2_ini, const int a_iter2_fim, const IdCenario      a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double*     getReferenciaElementosMatriz(const IdMes               a_iterador1, const IdPatamarCarga      a_iter2_ini, const IdPatamarCarga a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdAbertura* getReferenciaElementosMatriz(const IdCenario               a_iterador1, const IdEstagio      a_iter2_ini, const IdEstagio a_iter2_fim, const IdAbertura         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdCenario* getReferenciaElementosMatriz(const IdCenario               a_iterador1, const IdEstagio      a_iter2_ini, const IdEstagio a_iter2_fim, const IdCenario         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdCenario*  getReferenciaElementosMatriz(const IdProcesso               a_iterador1, const IdIteracao      a_iter2_ini, const IdIteracao a_iter2_fim, const IdCenario         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double*     getReferenciaElementosMatriz(const IdProcesso               a_iterador1, const IdCenario      a_iter2_ini, const IdCenario a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
\
	virtual bool setMatriz(const SmartEnupla<IdCenario,           SmartEnupla<Periodo,             double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdCenario,           SmartEnupla<Periodo,       IdRealizacao>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdCenario,           SmartEnupla<Periodo,               IdNo>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<Periodo,             SmartEnupla<IdPatamarCarga,      double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<Periodo,             SmartEnupla<int,                 double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<Periodo,             SmartEnupla<IdCenario,           double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdHidreletrica,      SmartEnupla<Periodo,             double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdEstacao,           SmartEnupla<int,                 double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdEstacao,           SmartEnupla<IdVariavelAleatoria, double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdVariavelAleatoria, SmartEnupla<IdEstacao,           double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdVariavelAleatoriaInterna, SmartEnupla<IdEstacao,           double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdRealizacao,		  SmartEnupla<IdVariavelAleatoria, double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdRealizacao,		  SmartEnupla<IdVariavelEstado,    double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdRealizacao,		  SmartEnupla<int,         IdCorteBenders>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdRealizacao,		  SmartEnupla<int,                 double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<int,		          SmartEnupla<int,                 double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<int,		          SmartEnupla<IdRealizacao,        double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<int,		          SmartEnupla<IdCenario,        double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdRealizacao,		  SmartEnupla<int,                    int>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdCenario,			  SmartEnupla<IdEstagio,             IdNo>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<Periodo,             SmartEnupla<IdRealizacao,        double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<Periodo,             SmartEnupla<IdNo,           IdRealizacao>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<Periodo,             SmartEnupla<IdNo,                   IdNo>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<Periodo,             SmartEnupla<IdNo,                 double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdEstagio,           SmartEnupla<Periodo,             double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdEstagio,           SmartEnupla<Periodo,             int>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdEstagio,           SmartEnupla<IdRealizacao,         double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdEstagio,           SmartEnupla<IdRealizacao,         int>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdEstagio,           SmartEnupla<IdCenario,         int>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdIteracao,          SmartEnupla<IdCenario,           double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdIteracao,          SmartEnupla<IdRealizacao,        double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdIteracao,          SmartEnupla<int,              IdCenario>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdMes,               SmartEnupla<IdPatamarCarga,      double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdCenario,          SmartEnupla<IdEstagio,       IdAbertura>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdCenario,          SmartEnupla<IdEstagio,       IdCenario>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdProcesso,          SmartEnupla<IdIteracao,       IdCenario>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdProcesso,          SmartEnupla<IdCenario,       double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
\
	virtual bool setMatriz_forced(const SmartEnupla<IdCenario,           SmartEnupla<Periodo,             double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdCenario,           SmartEnupla<Periodo,       IdRealizacao>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdCenario,           SmartEnupla<Periodo,               IdNo>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<Periodo,             SmartEnupla<IdPatamarCarga,      double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<Periodo,             SmartEnupla<int,                 double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<Periodo,             SmartEnupla<IdCenario,           double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdHidreletrica,      SmartEnupla<Periodo,             double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdEstacao,           SmartEnupla<int,                 double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdEstacao,           SmartEnupla<IdVariavelAleatoria, double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdVariavelAleatoria, SmartEnupla<IdEstacao,           double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdVariavelAleatoriaInterna, SmartEnupla<IdEstacao,           double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdRealizacao,		  SmartEnupla<IdVariavelAleatoria, double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdRealizacao,		  SmartEnupla<IdVariavelEstado,    double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdRealizacao,		  SmartEnupla<int,         IdCorteBenders>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdRealizacao,		  SmartEnupla<int,                 double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdRealizacao,		  SmartEnupla<int,                    int>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<int,		          SmartEnupla<IdRealizacao,        double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<int,		          SmartEnupla<IdCenario,        double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<int,		          SmartEnupla<int,                 double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdCenario,			  SmartEnupla<IdEstagio,             IdNo>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<Periodo,             SmartEnupla<IdRealizacao,        double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<Periodo,             SmartEnupla<IdNo,          IdRealizacao>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<Periodo,             SmartEnupla<IdNo,                  IdNo>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<Periodo,             SmartEnupla<IdNo,                double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdEstagio,           SmartEnupla<Periodo,             double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdEstagio,           SmartEnupla<Periodo,             int>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdEstagio,           SmartEnupla<IdRealizacao,         double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdEstagio,           SmartEnupla<IdRealizacao,         int>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdEstagio,           SmartEnupla<IdCenario,         int>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdIteracao,          SmartEnupla<IdCenario,           double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdIteracao,          SmartEnupla<IdRealizacao,        double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdIteracao,          SmartEnupla<int,              IdCenario>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdMes,               SmartEnupla<IdPatamarCarga,      double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdCenario,          SmartEnupla<IdEstagio,       IdAbertura>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdCenario,          SmartEnupla<IdEstagio,       IdCenario>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdProcesso,          SmartEnupla<IdIteracao,       IdCenario>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdProcesso,          SmartEnupla<IdCenario,       double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
\
	virtual SmartEnupla<IdCenario,           SmartEnupla<Periodo,             double>> getMatriz(const IdCenario           a_iterador1, const Periodo             a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdCenario,           SmartEnupla<Periodo,       IdRealizacao>> getMatriz(const IdCenario           a_iterador1, const Periodo             a_iterador2, const IdRealizacao   a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdCenario,           SmartEnupla<Periodo,               IdNo>> getMatriz(const IdCenario           a_iterador1, const Periodo             a_iterador2, const IdNo           a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<Periodo,             SmartEnupla<IdPatamarCarga,      double>> getMatriz(const Periodo             a_iterador1, const IdPatamarCarga      a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<Periodo,             SmartEnupla<int,                 double>> getMatriz(const Periodo             a_iterador1, const int                 a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<Periodo,             SmartEnupla<IdCenario,           double>> getMatriz(const Periodo             a_iterador1, const IdCenario           a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdHidreletrica,      SmartEnupla<Periodo,             double>> getMatriz(const IdHidreletrica      a_iterador1, const Periodo             a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdEstacao,           SmartEnupla<int,                 double>> getMatriz(const IdEstacao           a_iterador1, const int                 a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdEstacao,           SmartEnupla<IdVariavelAleatoria, double>> getMatriz(const IdEstacao           a_iterador1, const IdVariavelAleatoria a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdVariavelAleatoria, SmartEnupla<IdEstacao,           double>> getMatriz(const IdVariavelAleatoria a_iterador1, const IdEstacao           a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdVariavelAleatoriaInterna, SmartEnupla<IdEstacao,           double>> getMatriz(const IdVariavelAleatoriaInterna a_iterador1, const IdEstacao           a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdRealizacao,		 SmartEnupla<IdVariavelAleatoria, double>> getMatriz(const IdRealizacao		   a_iterador1, const IdVariavelAleatoria a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdRealizacao,		 SmartEnupla<IdVariavelEstado,    double>> getMatriz(const IdRealizacao		   a_iterador1, const IdVariavelEstado    a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdRealizacao,		 SmartEnupla<int,         IdCorteBenders>> getMatriz(const IdRealizacao		   a_iterador1, const int                 a_iterador2, const IdCorteBenders a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdRealizacao,		 SmartEnupla<int,                 double>> getMatriz(const IdRealizacao		   a_iterador1, const int                 a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<int,		         SmartEnupla<int,                 double>> getMatriz(const int		   a_iterador1, const int                 a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<int,		         SmartEnupla<IdRealizacao,                 double>> getMatriz(const int		   a_iterador1, const IdRealizacao                 a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<int,		         SmartEnupla<IdCenario,                 double>> getMatriz(const int		   a_iterador1, const IdCenario                 a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdRealizacao,		 SmartEnupla<int,                    int>> getMatriz(const IdRealizacao		   a_iterador1, const int                 a_iterador2, const int            a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdCenario,			 SmartEnupla<IdEstagio,		        IdNo>> getMatriz(const IdCenario		   a_iterador1, const IdEstagio		      a_iterador2, const IdNo           a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<Periodo,             SmartEnupla<IdRealizacao,        double>> getMatriz(const Periodo             a_iterador1, const IdRealizacao        a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<Periodo,             SmartEnupla<IdNo,          IdRealizacao>> getMatriz(const Periodo             a_iterador1, const IdNo                a_iterador2, const IdRealizacao   a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<Periodo,             SmartEnupla<IdNo,                  IdNo>> getMatriz(const Periodo             a_iterador1, const IdNo                a_iterador2, const IdNo           a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<Periodo,             SmartEnupla<IdNo,                double>> getMatriz(const Periodo             a_iterador1, const IdNo                a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdEstagio,           SmartEnupla<Periodo,             double>> getMatriz(const IdEstagio           a_iterador1, const Periodo             a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdEstagio,           SmartEnupla<Periodo,             int>> getMatriz(const IdEstagio           a_iterador1, const Periodo             a_iterador2, const int         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdEstagio,           SmartEnupla<IdRealizacao,         double>> getMatriz(const IdEstagio          a_iterador1, const IdRealizacao         a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdEstagio,           SmartEnupla<IdRealizacao,         int>> getMatriz(const IdEstagio             a_iterador1, const IdRealizacao         a_iterador2, const int         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdEstagio,           SmartEnupla<IdCenario,           int>> getMatriz(const IdEstagio             a_iterador1, const IdCenario         a_iterador2, const int         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdIteracao,          SmartEnupla<IdCenario,           double>> getMatriz(const IdIteracao          a_iterador1, const IdCenario           a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdIteracao,          SmartEnupla<IdRealizacao,        double>> getMatriz(const IdIteracao          a_iterador1, const IdRealizacao        a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdIteracao,          SmartEnupla<int,              IdCenario>> getMatriz(const IdIteracao          a_iterador1, const int                 a_iterador2, const IdCenario      a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdMes,               SmartEnupla<IdPatamarCarga,      double>> getMatriz(const IdMes               a_iterador1, const IdPatamarCarga      a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdCenario,          SmartEnupla<IdEstagio,       IdAbertura>> getMatriz(const IdCenario          a_iterador1, const IdEstagio           a_iterador2, const IdAbertura     a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdCenario,          SmartEnupla<IdEstagio,       IdCenario>> getMatriz(const IdCenario          a_iterador1, const IdEstagio           a_iterador2, const IdCenario     a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdProcesso,          SmartEnupla<IdIteracao,       IdCenario>> getMatriz(const IdProcesso          a_iterador1, const IdIteracao           a_iterador2, const IdCenario     a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdProcesso,          SmartEnupla<IdCenario,       double>> getMatriz(const IdProcesso          a_iterador1, const IdCenario           a_iterador2, const double     a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
};\
valores(DECLARA_STRUCT_AttMatriz_DERIVADA) DECLARA_STRUCT_AttMatriz_NENHUM(Classe, Atributo) \
DEFINE_METODOS_AttMatriz(Classe, Atributo) \
private: \
public: \
std::vector<Atributo##Classe##_Struct*> lista##Atributo##Classe; \
public:


#define DEFINE_PTR_STRUCT_AttMatriz(Classe, Atributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, TipoIterador1, TipoIterador2) Atributo##Classe##_##nomeAtributo##_Struct* Atributo##Classe##_##nomeAtributo##_struct = new Atributo##Classe##_##nomeAtributo##_Struct;
#define PTR_STRUCT_AttMatriz(Classe, Atributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, TipoIterador1, TipoIterador2) Atributo##Classe##_##nomeAtributo##_struct,

#define INICIALIZA_AttMatriz(Classe, Atributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, TipoIterador1, TipoIterador2) 

#define INICIALIZAR_AttMatriz(Classe, Atributo, valores) \
try{ \
valores(DEFINE_PTR_STRUCT_AttMatriz) \
Atributo##Classe##_Nenhum_Struct* Atributo##Classe##_Nenhum_struct = new Atributo##Classe##_Nenhum_Struct; \
Atributo##Classe##_Nenhum_Struct* Atributo##Classe##_Excedente_struct = new Atributo##Classe##_Nenhum_Struct; \
lista##Atributo##Classe = {Atributo##Classe##_Nenhum_struct, valores(PTR_STRUCT_AttMatriz) Atributo##Classe##_Excedente_struct}; \
valores(INICIALIZA_AttMatriz)  \
} \
catch (const std::exception & erro) { throw std::invalid_argument("INICIALIZAR_" + std::string(#Atributo) + std::string(#Classe) + " " + std::string(erro.what())); }

#define EXCLUIR_AttMatriz(Classe, Atributo, valores)\
for (int i = 1; i < int(lista##Atributo##Classe.size()); i++) { \
	if (lista##Atributo##Classe.at(i) != nullptr){\
		delete lista##Atributo##Classe.at(i); \
		lista##Atributo##Classe.at(i) = nullptr; \
	}\
}

#define TRANSFERIR_VALORES_AttMatriz(Classe, Atributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, TipoIterador1, TipoIterador2) \
try{ \
	if (instanciaCopiar.lista##Atributo##Classe.size() > 0) \
		lista##Atributo##Classe.at(Atributo##Classe##_##nomeAtributo)->setMatriz_forced(instanciaCopiar.lista##Atributo##Classe.at(Atributo##Classe##_##nomeAtributo)->getMatriz(TipoIterador1(), TipoIterador2(), TipoValor())); \
} \
catch (const std::exception & erro) { throw std::invalid_argument("TRANSFERIR_VALORES_" + std::string(#Atributo) + std::string(#Classe) + " " + std::string(erro.what())); }

#define COPIAR_AttMatriz(Classe, Atributo, valores)\
try{ \
	EXCLUIR_AttMatriz(Classe, Atributo, valores)\
	INICIALIZAR_AttMatriz(Classe, Atributo, valores) \
	valores(TRANSFERIR_VALORES_AttMatriz)\
}\
catch (const std::exception& erro) { throw std::invalid_argument("COPIAR_" + std::string(#Atributo) + std::string(#Classe) + " " + std::string(erro.what())); }


#endif