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


#define DEFINE_ATRIBUTO_SMART_ATRIBUTO_MATRIZ(Classe, SmartAtributo, nomeAtributo, TVlr, valorMinimo, valorMaximo, valorInicial, TIter1, TIter2) SmartAtributo##Classe##_##nomeAtributo,
#define DEFINE_CHAR_SMART_ATRIBUTO_MATRIZ(Classe, SmartAtributo, nomeAtributo, TVlr, valorMinimo, valorMaximo, valorInicial, TIter1, TIter2) #nomeAtributo,
#define DEFINE_CHARS_SMART_ATRIBUTO_MATRIZ(Classe, SmartAtributo, valores) static const char* SmartAtributo##Classe##Array [] = { "Nenhum", valores(DEFINE_CHAR_SMART_ATRIBUTO_MATRIZ) "Excedente" };

#define DEFINE_PRE_DATA_SMART_ATRIBUTO_MATRIZ(Classe, SmartAtributo, nomeAtributo, TVlr, valorMinimo, valorMaximo, valorInicial, TIter1, TIter2) \
TVlr nomeAtributo##_LB;\
TVlr nomeAtributo##_UB;\
TVlr nomeAtributo##_IV;

#define DEFINE_PRE_DATA_CONSTRUTOR_SMART_ATRIBUTO_MATRIZ(Classe, SmartAtributo, nomeAtributo, TVlr, valorMinimo, valorMaximo, valorInicial, TIter1, TIter2) \
nomeAtributo##_LB = get##TVlr##FromChar(#valorMinimo);\
nomeAtributo##_UB = get##TVlr##FromChar(#valorMaximo);\
nomeAtributo##_IV = get##TVlr##FromChar(#valorInicial);

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
virtual void addDadoAttMatriz(const std::vector<std::string> &a_iter1, const std::vector<std::string> &a_iter2, const std::string &a_att, const std::vector<std::vector<std::string>> &a_vlr){  \
	try { \
		const Atributo##Classe att = get##Atributo##Classe##FromChar(a_att.c_str()); \
		lista##Atributo##Classe.at(att)->addMatrizFromString(a_iter1, a_iter2, a_vlr);\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::addDadoAttMatriz(): \n" + std::string(erro.what())); }\
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
template<typename TIter1, typename TIter2, typename TVlr> \
void addElemento(const Atributo##Classe a_tipoAtributo, const TIter1 a_iterador1, const TIter2 a_iterador2, const TVlr  a_valor){  \
	try { lista##Atributo##Classe.at(a_tipoAtributo)->addElemento(a_iterador1, a_iterador2, a_valor);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::addElemento(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador1) + "," + getString(a_iterador2) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }\
}; \
template<typename TIter1, typename TIter2> \
void setElemento(const Atributo##Classe a_tipoAtributo, const TIter1 a_iterador1, const TIter2 a_iterador2){  \
	try { lista##Atributo##Classe.at(a_tipoAtributo)->setElemento(a_iterador1, a_iterador2);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::setElemento(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador1) + "," + getString(a_iterador2) + "): \n" + std::string(erro.what())); }\
}; \
template<typename TIter1, typename TIter2, typename TVlr> \
void setElemento(const Atributo##Classe a_tipoAtributo, const TIter1 a_iterador1, const TIter2 a_iterador2, const TVlr  a_valor){  \
	try { lista##Atributo##Classe.at(a_tipoAtributo)->setElemento(a_iterador1, a_iterador2, a_valor);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::setElemento(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador1) + "," + getString(a_iterador2) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }\
}; \
template<typename TIter1, typename TIter2, typename TVlr> \
void setElementos(const Atributo##Classe a_tipoAtributo, const TIter1 a_iterador1, const SmartEnupla<TIter2, TVlr> &a_valores){  \
	try { lista##Atributo##Classe.at(a_tipoAtributo)->setElementos(a_iterador1, a_valores);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::setElementos(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador1) + ",a_valores): \n" + std::string(erro.what())); }\
}; \
template<typename TIter1> \
TIter1 getIterador1Inicial(const Atributo##Classe a_tipoAtributo, const TIter1 a_iterador1)const{  \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getIterador1Inicial(a_iterador1);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::getIterador1Inicial(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + ",TIter1): \n" + std::string(erro.what())); }\
}; \
template<typename TIter1> \
TIter1 getIterador1Final(const Atributo##Classe a_tipoAtributo, const TIter1 a_iterador1)const{  \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getIterador1Final(a_iterador1);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::getIterador1Final(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + ",TIter1): \n" + std::string(erro.what())); }\
}; \
template<typename TIter1, typename TIter2> \
TIter2 getIterador2Inicial(const Atributo##Classe a_tipoAtributo, const TIter1 a_iterador1, const TIter2 a_iterador2)const{  \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getIterador2Inicial(a_iterador1,a_iterador2);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::getIterador2Inicial(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador1) + ",TIter2): \n" + std::string(erro.what())); }\
}; \
template<typename TIter1, typename TIter2> \
TIter2 getIterador2Final(const Atributo##Classe a_tipoAtributo, const TIter1 a_iterador1, const TIter2 a_iterador2)const{  \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getIterador2Final(a_iterador1,a_iterador2);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::getIterador2Final(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador1) + ",TIter2): \n" + std::string(erro.what())); }\
}; \
template<typename TIter1, typename TIter2, typename TVlr> \
TVlr getElementoMatriz(const Atributo##Classe a_tipoAtributo, const TIter1 a_iterador1, const TIter2 a_iterador2, const TVlr  a_valor)const{  \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getElementoMatriz(a_iterador1, a_iterador2, a_valor);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::getElementoMatriz(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador1) + "," + getString(a_iterador2) + ",TVlr): \n" + std::string(erro.what())); }\
}; \
template<typename TIter1, typename TIter2, typename TVlr> \
SmartEnupla<TIter2, TVlr> getElementosMatriz(const Atributo##Classe a_tipoAtributo, const TIter1 a_iterador1, const TIter2 a_iterador2, const TVlr a_valor)const{  \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getElementosMatriz(a_iterador1, a_iterador2, a_valor);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::getElementosMatriz(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador1) + ",TIter2,TVlr): \n" + std::string(erro.what())); }\
}; \
template<typename TIter1, typename TIter2, typename TVlr> \
TVlr* getReferenciaElementosMatriz(const Atributo##Classe a_tipoAtributo, const TIter1 a_iterador1, const TIter2 a_iter2_ini, const TIter2 a_iter2_fim, const TVlr a_valor){  \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getReferenciaElementosMatriz(a_iterador1, a_iter2_ini, a_iter2_fim, a_valor);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::getReferenciaElementosMatriz(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador1) + ",TIter2,TVlr): \n" + std::string(erro.what())); }\
}; \
int getSizeMatriz(const Atributo##Classe a_tipoAtributo) const {return lista##Atributo##Classe.at(a_tipoAtributo)->getSizeMatriz(); }; \
template<typename TIter1> \
int getSizeMatriz(const Atributo##Classe a_tipoAtributo, const TIter1 a_iterador1) const {return lista##Atributo##Classe.at(a_tipoAtributo)->getSizeMatriz(a_iterador1); }; \
template<typename TIter1, typename TIter2, typename TVlr> \
SmartEnupla<TIter1,SmartEnupla<TIter2,TVlr>> getMatriz(const Atributo##Classe a_tipoAtributo, const TIter1 a_iterador1, const TIter2 a_iterador2, const TVlr a_valor) const{ \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getMatriz(a_iterador1, a_iterador2, a_valor);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::getMatriz(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + ",TIter1,TIter2,TVlr): \n" + std::string(erro.what())); }\
}; \
template<typename TIter1, typename TIter2, typename TVlr> \
bool setMatriz(const Atributo##Classe a_tipoAtributo, const SmartEnupla<TIter1, SmartEnupla<TIter2, TVlr>> &a_matriz) { \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->setMatriz(a_matriz);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::setMatriz(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + ",a_matriz): \n" + std::string(erro.what())); }\
}; \
template<typename TIter1, typename TIter2, typename TVlr> \
bool setMatriz_forced(const Atributo##Classe a_tipoAtributo, const SmartEnupla<TIter1, SmartEnupla<TIter2, TVlr>> & a_matriz) {	\
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->setMatriz_forced(a_matriz);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "(" + getFullString(getIdObjeto()) + ")::setMatriz(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + ",a_matriz): \n" + std::string(erro.what())); }\
};

// ------------------------------------------------------------------
//
// Macros para Declaração de Structs com Métodos Virtuais de AttMatriz
//
// ------------------------------------------------------------------


#define DECLARA_STRUCT_AttMatriz_DERIVADA(Classe, Atributo, nomeAtributo, TVlr, valorMinimo, valorMaximo, valorInicial, TIter1, TIter2) \
struct Atributo##Classe##_##nomeAtributo##_Struct: Atributo##Classe##_Struct{ \
	virtual bool addElemento(const TIter1 a_iterador1, const TIter2 a_iterador2, const TVlr a_valor) { \
		 return addElemento(a_iterador1, a_iterador2, a_valor, 1, 1); \
	}; \
	virtual bool addElemento(const TIter1 a_iterador1, const TIter2 a_iterador2, const TVlr a_valor, const int a_alocacao_1, const int a_alocacao_2) { \
		try{ \
			if (a_valor < Atributo##Classe##PreData.nomeAtributo##_LB) { throw std::invalid_argument("Valor inferior ao valor minimo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else if (a_valor > Atributo##Classe##PreData.nomeAtributo##_UB) { throw std::invalid_argument("Valor superior ao valor maximo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else { \
				if (dados.size() == 0){ \
					dados = SmartEnupla<TIter1, SmartEnupla<TIter2, TVlr>>(a_alocacao_1); \
					SmartEnupla <TIter2, TVlr> dados_linha(a_alocacao_2); \
					dados_linha.addElemento(a_iterador2, a_valor); \
					dados.addElemento(a_iterador1, dados_linha); \
				} \
				else {\
					const TIter1 iteradorFinal = dados.getIteradorFinal(); \
					if (a_iterador1 <= iteradorFinal) {\
						if (dados.at(a_iterador1).size() == 0) \
							dados.at(a_iterador1) = SmartEnupla <TIter2, TVlr>(a_alocacao_2);\
						dados.at(a_iterador1).addElemento(a_iterador2, a_valor); \
					} \
					else { \
						SmartEnupla <TIter2, TVlr> dados_linha(a_alocacao_2); \
						dados_linha.addElemento(a_iterador2, a_valor); \
						dados.addElemento(a_iterador1, dados_linha); \
					} \
				} \
			} \
			return true;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::addElemento(" + getString(a_iterador1) + "," + getString(a_iterador2) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }\
	};\
	virtual bool addMatrizFromString(const std::vector<std::string> &a_iter1, const std::vector<std::string> &a_iter2, const std::vector<std::vector<std::string>> &a_vlr){  \
		try { \
				SmartEnupla<TIter1, SmartEnupla<TIter2, TVlr>> matrix(int(a_iter1.size())); \
				for (int it1 = 0; it1 < int(a_iter1.size()); it1++) { \
						SmartEnupla<TIter2, TVlr> enupla(int(a_vlr.at(it1).size())); \
						for (int it2 = 0; it2 < int(a_vlr.at(it1).size()); it2++)\
							enupla.addElemento(get##TIter2##FromChar(a_iter2.at(it2).c_str()), get##TVlr##FromChar(a_vlr.at(it1).at(it2).c_str())); \
						matrix.addElemento(get##TIter1##FromChar(a_iter1.at(it1).c_str()), enupla); \
				}\
					if (getSizeMatriz() == 0)\
						setMatriz(matrix); \
					else {\
							SmartEnupla<TIter1, SmartEnupla<TIter2, TVlr>> matrix_prev = dados; \
							matrix_prev.addElemento(matrix); \
							setMatriz(matrix_prev); \
					}\
				return true;\
			} \
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::addMatrizFromString(): \n" + std::string(erro.what())); }\
	}; \
	virtual bool clear() { \
		try{ \
			dados = SmartEnupla<TIter1, SmartEnupla<TIter2, TVlr>>(); \
			return true;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::clear(): \n" + std::string(erro.what())); }\
	};\
	virtual bool addElemento(const TIter1 a_iterador1) { \
		try{ \
			dados.addElemento(a_iterador1, SmartEnupla <TIter2, TVlr>()); \
			return true;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::addElemento(" + getString(a_iterador1) + "): \n" + std::string(erro.what())); }\
	};\
	virtual std::vector<std::vector<std::string>> getStringsFromElementos(const bool a_incluirAtributo, const bool a_incluirCabecalho, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final) const { \
		try{ \
			TIter1 iterador1Inicial; \
			TIter1 iterador1Final; \
			SmartEnupla<TIter1, bool> vetor_auxiliar_iter1; \
			const SmartEnupla<TIter1,SmartEnupla<TIter2,TVlr>> matriz = getMatriz(TIter1(), TIter2(), TVlr()); \
			if ((a_iterador1Inicial != "") || (a_iterador1Final != "")){ \
				iterador1Inicial = get##TIter1##FromChar(a_iterador1Inicial.c_str()); \
				iterador1Final   = get##TIter1##FromChar(a_iterador1Final.c_str()); \
				if (iterador1Inicial > iterador1Final) \
					throw std::invalid_argument("Iterador 1 inicial maior que final."); \
				if (getString(TIter1()) == getString(Periodo())){ \
					if (getSizeMatriz() > 0){ \
						for (TIter1 iter = iterador1Inicial; iter <= iterador1Final; matriz.incrementarIterador(iter)) \
							vetor_auxiliar_iter1.addElemento_rIt(iter, true); \
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
					vetor_auxiliar_iter1 = SmartEnupla<TIter1, bool>(iterador1Inicial, std::vector<bool>(int(iterador1Final - iterador1Inicial) + 1, true)); \
			} \
			else if (getSizeMatriz() > 0){ \
				iterador1Inicial = getIterador1Inicial(TIter1()); \
				iterador1Final   = getIterador1Final(TIter1()); \
				if (getString(TIter1()) == getString(Periodo())){ \
					for (TIter1 iter = iterador1Inicial; iter <= iterador1Final; matriz.incrementarIterador(iter)) \
						vetor_auxiliar_iter1.addElemento_rIt(iter, true); \
				} \
				else \
					vetor_auxiliar_iter1 = SmartEnupla<TIter1, bool>(iterador1Inicial, std::vector<bool>(int(iterador1Final - iterador1Inicial) + 1, true)); \
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
			TIter2 iterador2Inicial = get##TIter2##FromChar("min"); \
			TIter2 iterador2Final = get##TIter2##FromChar("min"); \
			bool iter2_Periodo = false; \
			SmartEnupla<TIter2, bool> vetor_auxiliar_iter2; \
			if (getString(TIter2()) == getString(Periodo())){ \
				iter2_Periodo = true; \
				TIter1 iter1_primeiro; \
				for (TIter1 iter = getIterador1Inicial(TIter1()); iter <= getIterador1Final(TIter1()); matriz.incrementarIterador(iter)){ \
					if (getSizeMatriz(iter) > 0){ \
						for (TIter2 iter2 = getIterador2Inicial(iter, TIter2()); iter2 <= getIterador2Final(iter, TIter2()); matriz.at_rIt(iter).incrementarIterador(iter2)){ \
							if ((vetor_auxiliar_iter2.size() == 0) || (iter == iter1_primeiro)){ \
								vetor_auxiliar_iter2.addElemento_rIt(iter2, true); \
								iter1_primeiro = iter; \
							} \
							else{ \
								if (vetor_auxiliar_iter2.isProximoIterador(iter2)) \
									vetor_auxiliar_iter2.addElemento_rIt(iter2, true); \
								else {\
									bool iter2_found = false; \
									for (TIter2 iter2_local = vetor_auxiliar_iter2.getIteradorInicial(); iter2_local <= vetor_auxiliar_iter2.getIteradorFinal(); vetor_auxiliar_iter2.incrementarIterador(iter2_local)) { \
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
				iterador2Inicial = get##TIter2##FromChar(a_iterador2Inicial.c_str()); \
				iterador2Final   = get##TIter2##FromChar(a_iterador2Final.c_str()); \
				if (iterador2Inicial > iterador2Final) \
					throw std::invalid_argument("Iterador 2 inicial maior que final."); \
				if (!iter2_Periodo) \
					vetor_auxiliar_iter2 = SmartEnupla<TIter2, bool>(iterador2Inicial, std::vector<bool>(int(iterador2Final - iterador2Inicial) + 1, true)); \
				else{ \
					bool iter2Inicial_found = false; \
					bool iter2Final_found   = false; \
					for (TIter2 iter2_local = vetor_auxiliar_iter2.getIteradorInicial(); iter2_local <= vetor_auxiliar_iter2.getIteradorFinal(); vetor_auxiliar_iter2.incrementarIterador(iter2_local)) { \
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
					for (TIter1 iter1 = getIterador1Inicial(TIter1()); iter1 <= getIterador1Final(TIter1()); matriz.incrementarIterador(iter1)){ \
						if (getSizeMatriz(iter1) > 0) { \
							TIter2 iterador2Inicial_local = getIterador2Inicial(iter1, TIter2()); \
							TIter2 iterador2Final_local   = getIterador2Final  (iter1, TIter2()); \
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
						vetor_auxiliar_iter2 = SmartEnupla<TIter2, bool>(iterador2Inicial, std::vector<bool>(int(iterador2Final - iterador2Inicial) + 1, true)); \
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
				const TIter1 iterador1Inicial_local = getIterador1Inicial(TIter1()); \
				const TIter1 iterador1Final_local   = getIterador1Final  (TIter1()); \
				int i1 = 0; \
				for (TIter1 iter1 = iterador1Inicial; iter1 <= iterador1Final; vetor_auxiliar_iter1.incrementarIterador(iter1)){ \
					if ((iter1 >= iterador1Inicial_local) && (iter1 <= iterador1Final_local)) { \
						if (getSizeMatriz(iter1) > 0) { \
							const TIter2 iterador2Inicial_local = getIterador2Inicial(iter1, TIter2()); \
							const TIter2 iterador2Final_local   = getIterador2Final  (iter1, TIter2()); \
							int i2 = 0; \
							for (TIter2 iter2 = iterador2Inicial; iter2 <= iterador2Final; vetor_auxiliar_iter2.incrementarIterador(iter2)){ \
								if ((iter2 < iterador2Inicial_local) || (iter2 > iterador2Final_local)) \
									matrizRetorno.at(i1 + espacoCabecalho).at(i2 + espacoAtributo + espacoIterador1) = ""; \
								else \
									matrizRetorno.at(i1 + espacoCabecalho).at(i2 + espacoAtributo + espacoIterador1) = getString(dados.at_rIt(iter1).getElemento_rIt(iter2)); \
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
						for (TIter2 iter2 = iterador2Inicial; iter2 <= iterador2Final; vetor_auxiliar_iter2.incrementarIterador(iter2)) { \
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
	virtual bool setElemento(const TIter1 a_iterador1, const TIter2 a_iterador2) { \
		try{ \
			const TVlr valor = Atributo##Classe##PreData.nomeAtributo##_IV;\
			if (valor < Atributo##Classe##PreData.nomeAtributo##_LB) { throw std::invalid_argument("Valor inicial inferior ao valor minimo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else if (valor > Atributo##Classe##PreData.nomeAtributo##_UB) { throw std::invalid_argument("Valor inicial superior ao valor maximo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else { dados.at(a_iterador1).setElemento(a_iterador2, valor); } \
			return true;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::setElemento(" + getString(a_iterador1) + "): \n" + std::string(erro.what())); }\
	};\
	virtual bool setElemento(const TIter1 a_iterador1, const TIter2 a_iterador2, const TVlr a_valor) { \
		try{ \
			if (a_valor < Atributo##Classe##PreData.nomeAtributo##_LB) { throw std::invalid_argument("Valor inferior ao valor minimo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else if (a_valor > Atributo##Classe##PreData.nomeAtributo##_UB) { throw std::invalid_argument("Valor superior ao valor maximo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else { dados.at(a_iterador1).setElemento(a_iterador2, a_valor); } \
			return true;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::setElemento(" + getString(a_iterador1) + "," + getString(a_iterador2) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }\
	};\
	virtual bool setElementos(const TIter1 a_iterador1, const SmartEnupla<TIter2, TVlr> &a_valores) { \
		try{ \
			if (a_valores.size() == 0) { dados.at(a_iterador1) = a_valores; } \
			else { \
				const TIter2 iteradorInicial = a_valores.getIteradorInicial(); \
				const TIter2 iteradorFinal   = a_valores.getIteradorFinal(); \
				for (TIter2 iterador = iteradorInicial; iterador <= iteradorFinal; iterador++) { \
					if (a_valores.at_rIt(iterador) < Atributo##Classe##PreData.nomeAtributo##_LB) { throw std::invalid_argument("Valor inferior ao valor minimo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
					else if (a_valores.at_rIt(iterador) > Atributo##Classe##PreData.nomeAtributo##_UB) { throw std::invalid_argument("Valor superior ao valor maximo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
				} \
				dados.at(a_iterador1) = a_valores; \
			} \
			return true;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::setElementos(" + getString(a_iterador1) + ",a_valores): \n" + std::string(erro.what())); }\
	};\
	virtual TVlr getElementoMatriz(const TIter1 a_iterador1, const TIter2 a_iterador2, const TVlr a_valor) const { \
		try{\
			return dados.at(a_iterador1).getElemento(a_iterador2); \
        }\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::getElementoMatriz(" + getString(a_iterador1) + "," + getString(a_iterador2) + ",TVlr): \n" + std::string(erro.what())); }\
	};\
	virtual SmartEnupla<TIter2,TVlr> getElementosMatriz(const TIter1 a_iterador1, const TIter2 a_iterador2, const TVlr a_valor)const { \
		try{\
			return dados.at(a_iterador1); \
        }\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::getElementoMatriz(" + getString(a_iterador1) + ",TIter2,TVlr): \n" + std::string(erro.what())); }\
	};\
	virtual TVlr* getReferenciaElementosMatriz(const TIter1 a_iter1, const TIter2 a_iter2_ini, const TIter2 a_iter2Fim, const TVlr a_valor) { \
		try{\
			if (a_iter2_ini < dados.at(a_iter1).getIteradorInicial()) \
				throw std::invalid_argument("Iterador 2 inicial inferior a SmartEnupla.");\
			if (a_iter2Fim > dados.at(a_iter1).getIteradorFinal()) \
				throw std::invalid_argument("Iterador 2 final superior a SmartEnupla.");\
			const int indice = int(a_iter2_ini - dados.at(a_iter1).getIteradorInicial());\
			return &dados.at(a_iter1).getElementos_referencia()[indice]; \
        }\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::getReferenciaElementosMatriz(" + getString(a_iter1) + "," + getString(a_iter2_ini) + "," + getString(a_iter2Fim) + "TVlr): \n" + std::string(erro.what())); }\
	};\
	virtual TIter1 getIterador1Inicial(TIter1 const a_iterador1) const { \
		try{\
			return dados.getIteradorInicial(); \
        }\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::getIterador1Inicial(TIter1): \n" + std::string(erro.what())); }\
	};\
	virtual TIter2 getIterador2Inicial(TIter1 const a_iterador1, TIter2 const a_iterador2) const { \
		try{\
			return dados.at(a_iterador1).getIteradorInicial(); \
        }\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::getIterador2Inicial(" + getString(a_iterador1) + ",TIter2): \n" + std::string(erro.what())); }\
	};\
	virtual TIter1 getIterador1Final(TIter1 const a_iterador1) const { \
		try{\
			return dados.getIteradorFinal(); \
        }\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::getIterador1Final(TIter1): \n" + std::string(erro.what())); }\
	};\
	virtual TIter2 getIterador2Final(TIter1 const a_iterador1, TIter2 const a_iterador2) const { \
		try{\
			return dados.at(a_iterador1).getIteradorFinal(); \
        }\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::getIterador2Final(" + getString(a_iterador1) + ",TIter2): \n" + std::string(erro.what())); }\
	};\
	virtual int getSizeMatriz()const{ return dados.size(); }\
	virtual int getSizeMatriz(TIter1 const a_iterador1)const{ return dados.at(a_iterador1).size(); }\
	virtual SmartEnupla<TIter1,SmartEnupla<TIter2,TVlr>> getMatriz(TIter1 a_iterador1, TIter2 a_iterador2, TVlr a_valor) const { return dados; }; \
	virtual bool setMatriz(const SmartEnupla<TIter1,SmartEnupla<TIter2,TVlr>> &a_matriz) { \
		if (a_matriz.size() > 0){ \
			const TIter1 iterador1Inicial = a_matriz.getIteradorInicial(); \
			const TIter1 iterador1Final   = a_matriz.getIteradorFinal(); \
			for (TIter1 iter1 = iterador1Inicial; iter1 <= iterador1Final; a_matriz.incrementarIterador(iter1)){ \
				if (a_matriz.at_rIt(iter1).size() > 0){ \
					const TIter2 iterador2Inicial = a_matriz.at_rIt(iter1).getIteradorInicial(); \
					const TIter2 iterador2Final   = a_matriz.at_rIt(iter1).getIteradorFinal(); \
					for (TIter2 iter2 = iterador2Inicial; iter2 <= iterador2Final; a_matriz.at_rIt(iter1).incrementarIterador(iter2)){ \
						TVlr valor = a_matriz.at_rIt(iter1).at_rIt(iter2); \
						if ((valor >= Atributo##Classe##PreData.nomeAtributo##_LB) && (valor <= Atributo##Classe##PreData.nomeAtributo##_UB)){}\
						else{\
							if (valor < Atributo##Classe##PreData.nomeAtributo##_LB) { throw std::invalid_argument("Valor " + getFullString(valor) + " inferior ao valor minimo " + getFullString(Atributo##Classe##PreData.nomeAtributo##_LB) + " do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + " em " + getFullString(iter1) + " e " + getFullString(iter2)); }\
							else if (valor > Atributo##Classe##PreData.nomeAtributo##_UB) { throw std::invalid_argument("Valor " + getFullString(valor) + " superior ao valor maximo " + getFullString(Atributo##Classe##PreData.nomeAtributo##_UB) + " do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + " em " + getFullString(iter1) + " e " + getFullString(iter2)); }\
						} \
					} \
				} \
			} \
		} \
		dados = a_matriz; \
		return true; \
	}; \
	virtual bool setMatriz_forced(const SmartEnupla<TIter1,SmartEnupla<TIter2,TVlr>> &a_matriz) { \
		dados = a_matriz; \
		return true; \
	}; \
	~Atributo##Classe##_##nomeAtributo##_Struct(){ clear();	}\
	SmartEnupla<TIter1, SmartEnupla<TIter2, TVlr>> dados;\
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
	virtual bool addMatrizFromString(const std::vector<std::string> &a_iter1, const std::vector<std::string> &a_iter2, const std::vector<std::vector<std::string>> &a_vlr) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addMatrizFromString(): \nEstrutura de dados nao definida."); return false; }; \
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
	virtual int getSizeMatriz(const IdMes                  a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
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
	virtual int getSizeMatriz(const int                        a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual int getSizeMatriz(const IdProcesso                 a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
\
	virtual bool   addElemento(const IdCenario			 a_iterador1, const Periodo             a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdCenario			 a_iterador1, const Periodo             a_iterador2, const IdRealizacao        a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const Periodo        	 a_iterador1, const IdPatamarCarga      a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const Periodo        	 a_iterador1, const int                 a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const Periodo        	 a_iterador1, const IdCenario           a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdHidreletrica 	 a_iterador1, const Periodo             a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdMes			 a_iterador1, const int                 a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdMes			 a_iterador1, const IdVariavelAleatoria a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdVariavelAleatoria        a_iterador1, const IdMes           a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   addElemento(const IdVariavelAleatoriaInterna a_iterador1, const IdMes           a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
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
	virtual bool   addElemento(const Periodo			 a_iterador1, const IdRealizacao        a_iterador2, const double              a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addAtributo(): \nEstrutura de dados nao definida."); return false; }; \
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
	virtual bool   setElemento(const Periodo             a_iterador1, const IdPatamarCarga      a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const Periodo             a_iterador1, const int                 a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const Periodo             a_iterador1, const IdCenario           a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdHidreletrica      a_iterador1, const Periodo             a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdMes           a_iterador1, const int                 a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdMes           a_iterador1, const IdVariavelAleatoria a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdVariavelAleatoria        a_iterador1, const IdMes           a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElemento(const IdVariavelAleatoriaInterna a_iterador1, const IdMes           a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
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
	virtual bool   setElemento(const Periodo             a_iterador1, const IdRealizacao        a_iterador2, const double         a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
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
	virtual bool   setElementos(const Periodo             a_iterador1, const SmartEnupla<IdPatamarCarga,      double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const Periodo             a_iterador1, const SmartEnupla<int,                 double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const Periodo             a_iterador1, const SmartEnupla<IdCenario,           double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdHidreletrica      a_iterador1, const SmartEnupla<Periodo,             double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdMes           a_iterador1, const SmartEnupla<int,                 double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdMes           a_iterador1, const SmartEnupla<IdVariavelAleatoria, double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdVariavelAleatoria        a_iterador1, const SmartEnupla<IdMes,           double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool   setElementos(const IdVariavelAleatoriaInterna a_iterador1, const SmartEnupla<IdMes,           double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
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
	virtual bool   setElementos(const Periodo             a_iterador1, const SmartEnupla<IdRealizacao,        double> &a_valor)      { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
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
	virtual double         getElementoMatriz(const Periodo             a_iterador1, const IdPatamarCarga      a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const Periodo             a_iterador1, const int                 a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const Periodo             a_iterador1, const IdCenario           a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const IdHidreletrica      a_iterador1, const Periodo             a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const IdMes           a_iterador1, const int                 a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const IdMes           a_iterador1, const IdVariavelAleatoria a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const IdVariavelAleatoria        a_iterador1, const IdMes    a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual double         getElementoMatriz(const IdVariavelAleatoriaInterna a_iterador1, const IdMes    a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
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
	virtual double         getElementoMatriz(const Periodo             a_iterador1, const IdRealizacao        a_iterador2, const double           a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
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
	virtual IdMes           getIterador1Inicial(const IdMes           a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
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
	virtual int                 getIterador1Inicial(const int               a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdProcesso          getIterador1Inicial(const IdProcesso          a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
\
	virtual IdCenario           getIterador1Final  (const IdCenario           a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdMes           getIterador1Final  (const IdMes           a_iterador1)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
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
	virtual int                 getIterador2Inicial(const IdMes           a_iterador1, const int                 a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdVariavelAleatoria getIterador2Inicial(const IdMes           a_iterador1, const IdVariavelAleatoria a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdMes           getIterador2Inicial(const IdVariavelAleatoria a_iterador1, const IdMes           a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdMes           getIterador2Inicial(const IdVariavelAleatoriaInterna a_iterador1, const IdMes    a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
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
	virtual int					getIterador2Final  (const IdMes           a_iterador1, const int                 a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdVariavelAleatoria getIterador2Final  (const IdMes           a_iterador1, const IdVariavelAleatoria a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdMes			getIterador2Final  (const IdVariavelAleatoria a_iterador1, const IdMes           a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual IdMes			getIterador2Final  (const IdVariavelAleatoriaInterna a_iterador1, const IdMes           a_iterador2)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
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
	virtual SmartEnupla<IdPatamarCarga,      double> getElementosMatriz(const Periodo             a_iterador1, const IdPatamarCarga      a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<int,                 double> getElementosMatriz(const Periodo             a_iterador1, const int                 a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdCenario,           double> getElementosMatriz(const Periodo             a_iterador1, const IdCenario           a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<Periodo,             double> getElementosMatriz(const IdHidreletrica      a_iterador1, const Periodo             a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<int,                 double> getElementosMatriz(const IdMes           a_iterador1, const int                 a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdVariavelAleatoria, double> getElementosMatriz(const IdMes           a_iterador1, const IdVariavelAleatoria a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdMes,           double> getElementosMatriz(const IdVariavelAleatoria a_iterador1, const IdMes           a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdMes,           double> getElementosMatriz(const IdVariavelAleatoriaInterna a_iterador1, const IdMes           a_iterador2, const double         a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
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
	virtual double* getReferenciaElementosMatriz(const Periodo             a_iterador1, const IdPatamarCarga      a_iter2_ini, const IdPatamarCarga a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double* getReferenciaElementosMatriz(const Periodo             a_iterador1, const int                 a_iter2_ini, const int a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double* getReferenciaElementosMatriz(const Periodo             a_iterador1, const IdCenario                 a_iter2_ini, const IdCenario a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double* getReferenciaElementosMatriz(const IdHidreletrica      a_iterador1, const Periodo             a_iter2_ini, const Periodo a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double* getReferenciaElementosMatriz(const IdMes           a_iterador1, const int                 a_iter2_ini, const int a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double* getReferenciaElementosMatriz(const IdMes           a_iterador1, const IdVariavelAleatoria a_iter2_ini, const IdVariavelAleatoria a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double* getReferenciaElementosMatriz(const IdVariavelAleatoria a_iterador1, const IdMes           a_iter2_ini, const IdMes a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual double* getReferenciaElementosMatriz(const IdVariavelAleatoriaInterna a_iterador1, const IdMes           a_iter2_ini, const IdMes a_iter2_fim, const double         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getReferenciaElementosMatriz(): \nEstrutura de dados nao definida.");}; \
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
	virtual bool setMatriz(const SmartEnupla<Periodo,             SmartEnupla<IdPatamarCarga,      double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<Periodo,             SmartEnupla<int,                 double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<Periodo,             SmartEnupla<IdCenario,           double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdHidreletrica,      SmartEnupla<Periodo,             double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdMes,           SmartEnupla<int,                 double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdMes,           SmartEnupla<IdVariavelAleatoria, double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdVariavelAleatoria, SmartEnupla<IdMes,           double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz(const SmartEnupla<IdVariavelAleatoriaInterna, SmartEnupla<IdMes,           double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
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
	virtual bool setMatriz(const SmartEnupla<Periodo,             SmartEnupla<IdRealizacao,        double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz(): \nEstrutura de dados nao definida."); return false; }; \
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
	virtual bool setMatriz_forced(const SmartEnupla<Periodo,             SmartEnupla<IdPatamarCarga,      double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<Periodo,             SmartEnupla<int,                 double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<Periodo,             SmartEnupla<IdCenario,           double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdHidreletrica,      SmartEnupla<Periodo,             double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdMes,           SmartEnupla<int,                 double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdMes,           SmartEnupla<IdVariavelAleatoria, double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdVariavelAleatoria, SmartEnupla<IdMes,           double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setMatriz_forced(const SmartEnupla<IdVariavelAleatoriaInterna, SmartEnupla<IdMes,           double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
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
	virtual bool setMatriz_forced(const SmartEnupla<Periodo,             SmartEnupla<IdRealizacao,        double>> &a_matriz){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setMatriz_forced(): \nEstrutura de dados nao definida."); return false; }; \
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
	virtual SmartEnupla<Periodo,             SmartEnupla<IdPatamarCarga,      double>> getMatriz(const Periodo             a_iterador1, const IdPatamarCarga      a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<Periodo,             SmartEnupla<int,                 double>> getMatriz(const Periodo             a_iterador1, const int                 a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<Periodo,             SmartEnupla<IdCenario,           double>> getMatriz(const Periodo             a_iterador1, const IdCenario           a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdHidreletrica,      SmartEnupla<Periodo,             double>> getMatriz(const IdHidreletrica      a_iterador1, const Periodo             a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdMes,           SmartEnupla<int,                 double>> getMatriz(const IdMes           a_iterador1, const int                 a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdMes,           SmartEnupla<IdVariavelAleatoria, double>> getMatriz(const IdMes           a_iterador1, const IdVariavelAleatoria a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdVariavelAleatoria, SmartEnupla<IdMes,           double>> getMatriz(const IdVariavelAleatoria a_iterador1, const IdMes           a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
\
	virtual SmartEnupla<IdVariavelAleatoriaInterna, SmartEnupla<IdMes,           double>> getMatriz(const IdVariavelAleatoriaInterna a_iterador1, const IdMes           a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
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
	virtual SmartEnupla<Periodo,             SmartEnupla<IdRealizacao,        double>> getMatriz(const Periodo             a_iterador1, const IdRealizacao        a_iterador2, const double         a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getMatriz(): \nEstrutura de dados nao definida.");}; \
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


#define DEFINE_PTR_STRUCT_AttMatriz(Classe, Atributo, nomeAtributo, TVlr, valorMinimo, valorMaximo, valorInicial, TIter1, TIter2) Atributo##Classe##_##nomeAtributo##_Struct* Atributo##Classe##_##nomeAtributo##_struct = new Atributo##Classe##_##nomeAtributo##_Struct;
#define PTR_STRUCT_AttMatriz(Classe, Atributo, nomeAtributo, TVlr, valorMinimo, valorMaximo, valorInicial, TIter1, TIter2) Atributo##Classe##_##nomeAtributo##_struct,

#define INICIALIZA_AttMatriz(Classe, Atributo, nomeAtributo, TVlr, valorMinimo, valorMaximo, valorInicial, TIter1, TIter2) 

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

#define TRANSFERIR_VALORES_AttMatriz(Classe, Atributo, nomeAtributo, TVlr, valorMinimo, valorMaximo, valorInicial, TIter1, TIter2) \
try{ \
	if (instanciaCopiar.lista##Atributo##Classe.size() > 0) \
		lista##Atributo##Classe.at(Atributo##Classe##_##nomeAtributo)->setMatriz_forced(instanciaCopiar.lista##Atributo##Classe.at(Atributo##Classe##_##nomeAtributo)->getMatriz(TIter1(), TIter2(), TVlr())); \
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