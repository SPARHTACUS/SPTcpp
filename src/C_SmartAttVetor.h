#ifndef C_SMART_ATT_VETOR_H
#define C_SMART_ATT_VETOR_H

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
// Macros para criação de SMART ATRIBUTO AttVetor 
//
// ----------------------------------------------


#define DEFINE_ATRIBUTO_SMART_ATRIBUTO_VETOR(Classe, SmartAtributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, TipoIterador) SmartAtributo##Classe##_##nomeAtributo,
#define DEFINE_CHAR_SMART_ATRIBUTO_VETOR(Classe, SmartAtributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, TipoIterador) #nomeAtributo,
#define DEFINE_CHARS_SMART_ATRIBUTO_VETOR(Classe, SmartAtributo, valores) static const char* SmartAtributo##Classe##Array [] = { "Nenhum", valores(DEFINE_CHAR_SMART_ATRIBUTO_VETOR) "Excedente" };

#define DEFINE_PRE_DATA_SMART_ATRIBUTO_VETOR(Classe, SmartAtributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, TipoIterador) \
TipoValor nomeAtributo##_LB;\
TipoValor nomeAtributo##_UB;\
TipoValor nomeAtributo##_IV;

#define DEFINE_PRE_DATA_CONSTRUTOR_SMART_ATRIBUTO_VETOR(Classe, SmartAtributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, TipoIterador) \
nomeAtributo##_LB = get##TipoValor##FromChar(#valorMinimo);\
nomeAtributo##_UB = get##TipoValor##FromChar(#valorMaximo);\
nomeAtributo##_IV = get##TipoValor##FromChar(#valorInicial);

#define DEFINE_ESTRUTURA_PRE_DATA_SMART_ATRIBUTO_VETOR(Classe, SmartAtributo, valores) \
struct  SmartAtributo##Classe##PreDataStruct { \
	valores(DEFINE_PRE_DATA_SMART_ATRIBUTO_VETOR)  \
	SmartAtributo##Classe##PreDataStruct(){ valores(DEFINE_PRE_DATA_CONSTRUTOR_SMART_ATRIBUTO_VETOR) }; \
};

#define DEFINE_SMART_AttVetor(Classe, SmartAtributo, valores) \
enum SmartAtributo##Classe { SmartAtributo##Classe##_Nenhum, valores(DEFINE_ATRIBUTO_SMART_ATRIBUTO_VETOR) SmartAtributo##Classe##_Excedente }; \
DEFINE_CHARS_SMART_ATRIBUTO_VETOR(Classe, SmartAtributo, valores) \
DEFINE_ESTRUTURA_PRE_DATA_SMART_ATRIBUTO_VETOR(Classe, SmartAtributo, valores) \
DEFINE_GET_SMART_ENUM_FROM_CHAR(SmartAtributo##Classe) \
DEFINE_GET_STRING_SMART_ENUM(SmartAtributo##Classe) \
DEFINE_GET_FULL_STRING_SMART_ENUM(SmartAtributo##Classe) \
DEFINE_GET_STRING_TIPO_SMART_ENUM(SmartAtributo##Classe) \
DEFINE_OPERADOR_SMART_ENUM_MAIS_MAIS(SmartAtributo##Classe) \


// -------------------------------------------------------
//
// Macros para criação dos métodos de AttVetor nas classes
//
// -------------------------------------------------------


#define DEFINE_METODOS_AttVetor(Classe, Atributo) \
virtual void addDadoAttVetor(const std::string a_idMembro, const std::string a_iterador, const std::string a_atributo, const std::string a_vlrDado){  \
	try { addDadoAttVetor(a_idMembro, a_iterador, a_atributo, a_vlrDado, 1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadoAttVetor(" + a_idMembro + "," + a_iterador + "," + a_atributo + ","  + a_vlrDado + "): \n" + std::string(erro.what())); }\
}; \
virtual void addDadoAttVetor(const std::string a_idMembro, const std::string a_iterador, const std::string a_atributo, const std::string a_vlrDado, const int a_alocacao){  \
	try { \
		if ((a_iterador == "") && (a_vlrDado == "")) { \
			validacaoDadosAttVetor(std::vector<std::string>{a_atributo}); \
			return;  \
		} \
		else if (getIdObjeto() != getId##Classe##FromChar(a_idMembro.c_str())) \
			throw std::invalid_argument("Inconsistencia na alocacao de dados. Membro Argumento - " + a_idMembro + " - Membro Destino - " + getString(getIdObjeto())); \
		else { \
			const Atributo##Classe atributo = get##Atributo##Classe##FromChar(a_atributo.c_str()); \
			addElementoFromString(atributo, a_iterador, a_vlrDado, a_alocacao); \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadoAttVetor(" + a_idMembro + "," + a_iterador + "," + a_atributo + ","  + a_vlrDado + "," + getFullString(a_alocacao) + "): \n" + std::string(erro.what())); }\
}; \
virtual void addDadoAttVetor(const std::string a_iterador, const std::string a_atributo, const std::string a_vlrDado){  \
	try { addDadoAttVetor(a_iterador, a_atributo, a_vlrDado, 1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadoAttVetor(" + a_iterador + "," + a_atributo + ","  + a_vlrDado + "): \n" + std::string(erro.what())); }\
}; \
virtual void addDadoAttVetor(const std::string a_iterador, const std::string a_atributo, const std::string a_vlrDado, const int a_alocacao){  \
	try { \
		if ((a_iterador == "") && (a_vlrDado == "")) { \
			validacaoDadosAttVetor(std::vector<std::string>{a_atributo}); \
			return;  \
		} \
		else { \
			const Atributo##Classe atributo = get##Atributo##Classe##FromChar(a_atributo.c_str()); \
			addElementoFromString(atributo, a_iterador, a_vlrDado, a_alocacao); \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadoAttVetor(" + a_iterador + "," + a_atributo + ","  + a_vlrDado + "," + getFullString(a_alocacao) + "): \n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttVetor(const bool a_incluirIteradores, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_atributo){  \
	try { \
		if (a_atributo.size() == 0) {  throw std::invalid_argument("Necessario informar um atributo AttVetor."); } \
		else { \
			const int numAtributos = int(a_atributo.size()); \
			std::vector<std::vector<std::string>> dados; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			for (int i = 0; i < numAtributos; i++){ \
				const Atributo##Classe atributo = get##Atributo##Classe##FromChar(a_atributo.at(i).c_str()); \
				if (i == 0) \
					dados = getStringsFromElementos(a_incluirAtributo, atributo, a_incluirIteradores, a_iteradorInicial, a_iteradorFinal); \
				else \
					dados = getStringsFromElementos(a_incluirAtributo, atributo, false, a_iteradorInicial, a_iteradorFinal); \
				matrizRetorno.insert(matrizRetorno.end(), dados.begin(), dados.end()); \
			} \
			return matrizRetorno; \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttVetor(" + getString(a_incluirIteradores) + "," + a_iteradorInicial + "," + a_iteradorFinal + "," + getString(a_incluirIteradores) + ",a_vetorAtributo): \n" + std::string(erro.what())); }\
}; \
void clear(const Atributo##Classe a_tipoAtributo){  \
	try { lista##Atributo##Classe.at(a_tipoAtributo)->clear();  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::clear(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "): \n" + std::string(erro.what())); }\
}; \
void addElemento(const Atributo##Classe a_tipoAtributo){  \
	try { lista##Atributo##Classe.at(a_tipoAtributo)->addElemento();  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addElemento(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "): \n" + std::string(erro.what())); }\
}; \
template<typename TipoIterador, typename TipoValor> \
void addElemento(const Atributo##Classe a_tipoAtributo, const TipoIterador a_iterador, const TipoValor  a_valor){  \
	try { lista##Atributo##Classe.at(a_tipoAtributo)->addElemento(a_iterador, a_valor);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addElemento(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }\
}; \
void addElementoFromString(const Atributo##Classe a_tipoAtributo, const std::string a_iterador, const std::string a_valor){  \
	try { lista##Atributo##Classe.at(a_tipoAtributo)->addElementoFromString(a_iterador, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addElementoFromString(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + a_iterador + "," + a_valor + "): \n" + std::string(erro.what())); }\
}; \
void addElementoFromString(const Atributo##Classe a_tipoAtributo, const std::string a_iterador, const std::string a_valor, const int a_alocacao){  \
	try { lista##Atributo##Classe.at(a_tipoAtributo)->addElementoFromString(a_iterador, a_valor, a_alocacao); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addElementoFromString(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + a_iterador + "," + a_valor + "," + getFullString(a_alocacao) + "): \n" + std::string(erro.what())); }\
}; \
template<typename TipoIterador> \
void setElemento(const Atributo##Classe a_tipoAtributo, const TipoIterador a_iterador){  \
	try { lista##Atributo##Classe.at(a_tipoAtributo)->setElemento(a_iterador);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::setElemento(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador) + "): \n" + std::string(erro.what())); }\
}; \
template<typename TipoIterador, typename TipoValor> \
void setElemento(const Atributo##Classe a_tipoAtributo, const TipoIterador a_iterador, const TipoValor  a_valor){  \
	try { lista##Atributo##Classe.at(a_tipoAtributo)->setElemento(a_iterador, a_valor);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::setElemento(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }\
}; \
std::vector<std::vector<std::string>> getStringsFromElementos(const bool a_incluirAtributo, const Atributo##Classe a_tipoAtributo, const bool a_incluirIteradores, const std::string a_iteradorInicial, const std::string a_iteradorFinal){  \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getStringsFromElementos(a_incluirAtributo, a_incluirIteradores, a_iteradorInicial, a_iteradorFinal); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getStringsFromElementos(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_incluirIteradores) + "," + a_iteradorInicial + "," + a_iteradorFinal + "): \n" + std::string(erro.what())); }\
}; \
template<typename TipoIterador> \
TipoIterador getIteradorInicial(const Atributo##Classe a_tipoAtributo, const TipoIterador a_iterador)const{  \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getIteradorInicial(a_iterador);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIteradorFinal(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador) + "): \n" + std::string(erro.what())); }\
}; \
template<typename TipoIterador> \
TipoIterador getIteradorFinal(const Atributo##Classe a_tipoAtributo, const TipoIterador a_iterador)const{  \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getIteradorFinal(a_iterador);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIteradorInicial(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador) + "): \n" + std::string(erro.what())); }\
}; \
template<typename TipoIterador, typename TipoValor> \
TipoValor getElementoVetor(const Atributo##Classe a_tipoAtributo, const TipoIterador a_iterador, const TipoValor  a_valor)const{  \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getElementoVetor(a_iterador, a_valor);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getElementoVetor(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador) + ",TipoValor): \n" + std::string(erro.what())); }\
}; \
int getSizeVetor(const Atributo##Classe a_tipoAtributo) const {return lista##Atributo##Classe.at(a_tipoAtributo)->getSizeVetor(); }; \
template<typename TipoIterador, typename TipoValor> \
SmartEnupla<TipoIterador,TipoValor> getVetor(const Atributo##Classe a_tipoAtributo, TipoIterador a_iterador, TipoValor a_valor) const{ \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getVetor(a_iterador, a_valor);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getVetor(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_iterador) + ",TipoValor): \n" + std::string(erro.what())); }\
}; \
template<typename TipoIterador, typename TipoValor> \
bool setVetor(const Atributo##Classe a_tipoAtributo, const SmartEnupla<TipoIterador, TipoValor> &a_vetor) { \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->setVetor(a_vetor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::setVetor(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + ",a_vetor): \n" + std::string(erro.what())); }\
}; \
template<typename TipoIterador, typename TipoValor> \
bool setVetor_forced(const Atributo##Classe a_tipoAtributo, const SmartEnupla<TipoIterador, TipoValor> & a_vetor) {	\
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->setVetor_forced(a_vetor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::setVetor_forced(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + ",a_vetor): \n" + std::string(erro.what())); }\
}; 


// ------------------------------------------------------------------
//
// Macros para Declaração de Structs com Métodos Virtuais de AttComum
//
// ------------------------------------------------------------------


#define DECLARA_STRUCT_AttVetor_DERIVADA(Classe, Atributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, TipoIterador) \
struct Atributo##Classe##_##nomeAtributo##_Struct: Atributo##Classe##_Struct{ \
	virtual bool clear() { \
		try{ \
			dados = SmartEnupla<TipoIterador,TipoValor>(); \
			return true;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::clear(): \n" + std::string(erro.what())); }\
	};\
	virtual bool addElemento() { \
		try{ \
			const TipoValor valor = Atributo##Classe##PreData.nomeAtributo##_IV;\
			if (valor < Atributo##Classe##PreData.nomeAtributo##_LB) { throw std::invalid_argument("Valor inicial inferior ao valor minimo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); } \
			else if (valor > Atributo##Classe##PreData.nomeAtributo##_UB) { throw std::invalid_argument("Valor inicial superior ao valor maximo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); } \
			else { dados.addElemento(valor); } \
			return true;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::addElemento(): \n" + std::string(erro.what())); }\
	};\
	virtual bool addElemento(const TipoIterador a_iterador, const TipoValor a_valor) { \
		try{ \
			if (a_valor < Atributo##Classe##PreData.nomeAtributo##_LB) { throw std::invalid_argument("Valor inferior ao valor minimo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else if (a_valor > Atributo##Classe##PreData.nomeAtributo##_UB) { throw std::invalid_argument("Valor superior ao valor maximo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else { dados.addElemento(a_iterador,a_valor); } \
			return true;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::addElemento(" + getString(a_iterador) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }\
	};\
	virtual bool addElementoFromString(const std::string a_iterador, const std::string a_valor) { \
		try{ return addElementoFromString(a_iterador, a_valor, 1); }\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::addElementoFromString(" + a_iterador + "," + a_valor + "): \n" + std::string(erro.what())); }\
	};\
	virtual bool addElementoFromString(const std::string a_iterador, const std::string a_valor, const int a_alocacao) { \
		try{ \
			if (getSizeVetor() == 0) { dados = SmartEnupla<TipoIterador,TipoValor>(a_alocacao); }; \
			if (strCompara(a_valor.c_str(), "NaN") || strCompara(a_valor.c_str(), "-nan(ind)")) { throw std::invalid_argument("Valor NaN."); }\
			const TipoValor valor = get##TipoValor##FromChar(a_valor.c_str());\
			if (valor < Atributo##Classe##PreData.nomeAtributo##_LB) { throw std::invalid_argument("Valor inferior ao valor minimo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else if (valor > Atributo##Classe##PreData.nomeAtributo##_UB) { throw std::invalid_argument("Valor superior ao valor maximo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else { dados.addElemento(get##TipoIterador##FromChar(a_iterador.c_str()), valor); } \
			return true;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::addElementoFromString(" + a_iterador + "," + a_valor + "," + getFullString(a_alocacao) + "): \n" + std::string(erro.what())); }\
	};\
	virtual std::vector<std::vector<std::string>> getStringsFromElementos(const bool a_incluirAtributo, const bool a_incluirIteradores, const std::string a_iteradorInicial, const std::string a_iteradorFinal) const { \
		try{ \
			TipoIterador iteradorInicial; \
			TipoIterador iteradorFinal; \
			SmartEnupla<TipoIterador, bool> vetor_auxiliar_iterador; \
			const SmartEnupla<TipoIterador,TipoValor> vetor = getVetor(TipoIterador(), TipoValor()); \
			if ((a_iteradorInicial != "") || (a_iteradorFinal != "")){ \
				iteradorInicial = get##TipoIterador##FromChar(a_iteradorInicial.c_str()); \
				iteradorFinal   = get##TipoIterador##FromChar(a_iteradorFinal.c_str()); \
				if (iteradorInicial > iteradorFinal) \
					throw std::invalid_argument("Iterador inicial maior que final."); \
				if (getString(TipoIterador()) == getString(Periodo())){ \
					if (getSizeVetor() > 0){ \
						for (TipoIterador iter = iteradorInicial; iter <= iteradorFinal; vetor.incrementarIterador(iter)) \
							vetor_auxiliar_iterador.addElemento(iter, true); \
					} \
					else {\
						std::vector<std::vector<std::string>> vetorRetorno; \
						if ((a_incluirAtributo) && (a_incluirIteradores))	\
							vetorRetorno.push_back(std::vector<std::string>{std::string(#Atributo)}); \
						if (a_incluirAtributo) \
							vetorRetorno.push_back(std::vector<std::string>{std::string(#nomeAtributo)}); \
						return vetorRetorno; \
					} \
				}\
				else \
					vetor_auxiliar_iterador = SmartEnupla<TipoIterador, bool>(iteradorInicial, std::vector<bool>(int(iteradorFinal - iteradorInicial) + 1, true)); \
			} \
			else if (getSizeVetor() > 0){ \
				iteradorInicial = getIteradorInicial(TipoIterador()); \
				iteradorFinal   = getIteradorFinal(TipoIterador()); \
				if (getString(TipoIterador()) == getString(Periodo())){ \
					for (TipoIterador iter = iteradorInicial; iter <= iteradorFinal; vetor.incrementarIterador(iter)) \
						vetor_auxiliar_iterador.addElemento(iter, true); \
				} \
				else \
					vetor_auxiliar_iterador = SmartEnupla<TipoIterador, bool>(iteradorInicial, std::vector<bool>(int(iteradorFinal - iteradorInicial) + 1, true)); \
			} \
			else {\
				std::vector<std::vector<std::string>> vetorRetorno; \
				if ((a_incluirAtributo) && (a_incluirIteradores))	\
					vetorRetorno.push_back(std::vector<std::string>{std::string(#Atributo)}); \
				if (a_incluirAtributo) \
					vetorRetorno.push_back(std::vector<std::string>{std::string(#nomeAtributo)}); \
				return vetorRetorno; \
			} \
			int espacoAtributo = 0; \
			if (a_incluirAtributo) \
				espacoAtributo = 1; \
			int numLinhas = 1; \
			if (a_incluirIteradores) \
				numLinhas = 2; \
			std::vector<std::vector<std::string>> vetorRetorno(numLinhas, std::vector<std::string>(espacoAtributo + vetor_auxiliar_iterador.size(), "")); \
			if (getSizeVetor() > 0){ \
				const TipoIterador iteradorInicial_local = getIteradorInicial(TipoIterador()); \
				const TipoIterador iteradorFinal_local   = getIteradorFinal(TipoIterador()); \
				int i = 0; \
				for (TipoIterador iter = iteradorInicial; iter <= iteradorFinal; vetor_auxiliar_iterador.incrementarIterador(iter)) { \
					if ((iter < iteradorInicial_local) || (iter > iteradorFinal_local))\
						vetorRetorno.at(numLinhas-1).at(espacoAtributo + i) = ""; \
					else \
						vetorRetorno.at(numLinhas-1).at(espacoAtributo + i) = getString(dados.getElemento(iter)); \
					if (a_incluirIteradores) \
						vetorRetorno.at(0).at(espacoAtributo + i) = getString(iter); \
					i++; \
				} \
			} \
			if ((a_incluirAtributo) && (a_incluirIteradores)){ \
				vetorRetorno.at(0).at(0) = std::string(#Atributo);  \
				vetorRetorno.at(1).at(0) = std::string(#nomeAtributo);  \
			} \
			else if (a_incluirAtributo) \
				vetorRetorno.at(0).at(0) = std::string(#nomeAtributo); \
			return vetorRetorno;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::getStringsFromElementos(" + getString(a_incluirIteradores) + "," + getString(a_iteradorInicial) + "," + getString(a_iteradorFinal) + "): \n" + std::string(erro.what())); }\
	};\
	virtual bool setElemento(const TipoIterador a_iterador) { \
		try{ \
			const TipoValor valor = Atributo##Classe##PreData.nomeAtributo##_IV;\
			if (valor < Atributo##Classe##PreData.nomeAtributo##_LB) { throw std::invalid_argument("Valor inicial inferior ao valor minimo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else if (valor > Atributo##Classe##PreData.nomeAtributo##_UB) { throw std::invalid_argument("Valor inicial superior ao valor maximo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else { dados.setElemento(a_iterador, valor); } \
			return true;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::setElemento(" + getString(a_iterador) + "): \n" + std::string(erro.what())); }\
	};\
	virtual bool setElemento(const TipoIterador a_iterador, const TipoValor a_valor) { \
		try{ \
			if (a_valor < Atributo##Classe##PreData.nomeAtributo##_LB) { throw std::invalid_argument("Valor inferior ao valor minimo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else if (a_valor > Atributo##Classe##PreData.nomeAtributo##_UB) { throw std::invalid_argument("Valor superior ao valor maximo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else { dados.setElemento(a_iterador, a_valor); } \
			return true;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::setElemento(" + getString(a_iterador) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }\
	};\
	virtual TipoValor getElementoVetor(const TipoIterador a_iterador, const TipoValor a_valor) const { \
		try{\
			return dados.getElemento(a_iterador); \
        }\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::getElementoVetor(" + getString(a_iterador) + ",TipoValor): \n" + std::string(erro.what())); }\
	};\
	virtual TipoIterador getIteradorInicial(TipoIterador const a_iterador) const { \
		try{\
			return dados.getIteradorInicial(); \
        }\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::getIteradorInicial(a_iterador): \n" + std::string(erro.what())); }\
	};\
	virtual TipoIterador getIteradorFinal(TipoIterador const a_iterador) const { \
		try{\
			return dados.getIteradorFinal(); \
        }\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::getIteradorFinal(a_iterador): \n" + std::string(erro.what())); }\
	};\
	virtual int getSizeVetor()const{ return dados.size(); }\
	virtual SmartEnupla<TipoIterador,TipoValor> getVetor(TipoIterador a_iterador, TipoValor a_valor) const { return dados; }; \
	virtual bool setVetor(const SmartEnupla<TipoIterador,TipoValor> &a_vetor) { \
	try{\
		if (a_vetor.size() > 0){ \
			const TipoIterador iteradorInicial = a_vetor.getIteradorInicial(); \
			const TipoIterador iteradorFinal   = a_vetor.getIteradorFinal(); \
			for (TipoIterador iter = iteradorInicial; iter <= iteradorFinal; a_vetor.incrementarIterador(iter)){ \
				TipoValor valor = a_vetor.at(iter); \
				if (valor < Atributo##Classe##PreData.nomeAtributo##_LB) { throw std::invalid_argument("Valor inferior ao valor minimo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
				else if (valor > Atributo##Classe##PreData.nomeAtributo##_UB) { throw std::invalid_argument("Valor superior ao valor maximo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			} \
		} \
		dados = a_vetor; \
		return true; \
        }\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::setVetor(a_vetor): \n" + std::string(erro.what())); }\
	}; \
	virtual bool setVetor_forced(const SmartEnupla<TipoIterador,TipoValor> &a_vetor) { \
	try{\
		dados = a_vetor; \
		return true; \
        }\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::setVetor_forced(a_vetor): \n" + std::string(erro.what())); }\
	 }; \
	~Atributo##Classe##_##nomeAtributo##_Struct(){ clear(); };\
	SmartEnupla<TipoIterador,TipoValor> dados;\
};


#define DECLARA_STRUCT_AttVetor_NENHUM(Classe, Atributo) \
struct Atributo##Classe##_Nenhum_Struct: Atributo##Classe##_Struct{ ~Atributo##Classe##_Nenhum_Struct(){}; };



#define DECLARA_METODOS_POR_ITERADOR_POR_TIPO_ARGUMENTO_ELEMENTAR_AttVetor(TipoIterador) \
virtual bool addElemento(const TipoIterador a_iterador, const      double a_valor) { throw std::invalid_argument("AttVetor_Nenhum_Struct::addElemento(): \nEstrutura de dados nao definida."); }; \
virtual bool addElemento(const TipoIterador a_iterador, const         int a_valor) { throw std::invalid_argument("AttVetor_Nenhum_Struct::addElemento(): \nEstrutura de dados nao definida."); }; \
virtual bool addElemento(const TipoIterador a_iterador, const        bool a_valor) { throw std::invalid_argument("AttVetor_Nenhum_Struct::addElemento(): \nEstrutura de dados nao definida."); }; \
virtual bool addElemento(const TipoIterador a_iterador, const std::string a_valor) { throw std::invalid_argument("AttVetor_Nenhum_Struct::addElemento(): \nEstrutura de dados nao definida."); }; \
virtual bool addElemento(const TipoIterador a_iterador, const     Periodo a_valor) { throw std::invalid_argument("AttVetor_Nenhum_Struct::addElemento(): \nEstrutura de dados nao definida."); }; \
\
virtual bool setElemento(const TipoIterador a_iterador, const      double a_valor) { throw std::invalid_argument("AttVetor_Nenhum_Struct::setElemento(): \nEstrutura de dados nao definida."); }; \
virtual bool setElemento(const TipoIterador a_iterador, const         int a_valor) { throw std::invalid_argument("AttVetor_Nenhum_Struct::setElemento(): \nEstrutura de dados nao definida."); }; \
virtual bool setElemento(const TipoIterador a_iterador, const        bool a_valor) { throw std::invalid_argument("AttVetor_Nenhum_Struct::setElemento(): \nEstrutura de dados nao definida."); }; \
virtual bool setElemento(const TipoIterador a_iterador, const std::string a_valor) { throw std::invalid_argument("AttVetor_Nenhum_Struct::setElemento(): \nEstrutura de dados nao definida."); }; \
virtual bool setElemento(const TipoIterador a_iterador, const     Periodo a_valor) { throw std::invalid_argument("AttVetor_Nenhum_Struct::setElemento(): \nEstrutura de dados nao definida."); }; \
\
virtual TipoIterador getIteradorInicial(TipoIterador a_iterador) const{ throw std::invalid_argument("AttVetor_Nenhum_Struct::getIteradorInicial(): \nEstrutura de dados nao definida.");}; \
virtual TipoIterador getIteradorFinal  (TipoIterador a_iterador) const{ throw std::invalid_argument("AttVetor_Nenhum_Struct::getIteradorFinal():   \nEstrutura de dados nao definida.");}; \
\
virtual      double getElementoVetor(const TipoIterador a_iterador, const      double a_valor)const { throw std::invalid_argument("AttVetor_Nenhum_Struct::getElementoVetor(): \nEstrutura de dados nao definida.");}; \
virtual         int getElementoVetor(const TipoIterador a_iterador, const         int a_valor)const { throw std::invalid_argument("AttVetor_Nenhum_Struct::getElementoVetor(): \nEstrutura de dados nao definida.");}; \
virtual        bool getElementoVetor(const TipoIterador a_iterador, const        bool a_valor)const { throw std::invalid_argument("AttVetor_Nenhum_Struct::getElementoVetor(): \nEstrutura de dados nao definida.");}; \
virtual std::string getElementoVetor(const TipoIterador a_iterador, const std::string a_valor)const { throw std::invalid_argument("AttVetor_Nenhum_Struct::getElementoVetor(): \nEstrutura de dados nao definida.");}; \
virtual     Periodo getElementoVetor(const TipoIterador a_iterador, const     Periodo a_valor)const { throw std::invalid_argument("AttVetor_Nenhum_Struct::getElementoVetor(): \nEstrutura de dados nao definida.");}; \
\
virtual bool setVetor(const SmartEnupla<TipoIterador,      double> &a_vetor){ throw std::invalid_argument("AttVetor_Nenhum_Struct::setVetor(): \nEstrutura de dados nao definida."); }; \
virtual bool setVetor(const SmartEnupla<TipoIterador,         int> &a_vetor){ throw std::invalid_argument("AttVetor_Nenhum_Struct::setVetor(): \nEstrutura de dados nao definida."); }; \
virtual bool setVetor(const SmartEnupla<TipoIterador,        bool> &a_vetor){ throw std::invalid_argument("AttVetor_Nenhum_Struct::setVetor(): \nEstrutura de dados nao definida."); }; \
virtual bool setVetor(const SmartEnupla<TipoIterador, std::string> &a_vetor){ throw std::invalid_argument("AttVetor_Nenhum_Struct::setVetor(): \nEstrutura de dados nao definida."); }; \
virtual bool setVetor(const SmartEnupla<TipoIterador,     Periodo> &a_vetor){ throw std::invalid_argument("AttVetor_Nenhum_Struct::setVetor(): \nEstrutura de dados nao definida."); }; \
\
virtual bool setVetor_forced(const SmartEnupla<TipoIterador,      double> &a_vetor){ throw std::invalid_argument("AttVetor_Nenhum_Struct::setVetor_forced(): \nEstrutura de dados nao definida."); }; \
virtual bool setVetor_forced(const SmartEnupla<TipoIterador,         int> &a_vetor){ throw std::invalid_argument("AttVetor_Nenhum_Struct::setVetor_forced(): \nEstrutura de dados nao definida."); }; \
virtual bool setVetor_forced(const SmartEnupla<TipoIterador,        bool> &a_vetor){ throw std::invalid_argument("AttVetor_Nenhum_Struct::setVetor_forced(): \nEstrutura de dados nao definida."); }; \
virtual bool setVetor_forced(const SmartEnupla<TipoIterador, std::string> &a_vetor){ throw std::invalid_argument("AttVetor_Nenhum_Struct::setVetor_forced(): \nEstrutura de dados nao definida."); }; \
virtual bool setVetor_forced(const SmartEnupla<TipoIterador,     Periodo> &a_vetor){ throw std::invalid_argument("AttVetor_Nenhum_Struct::setVetor_forced(): \nEstrutura de dados nao definida."); }; \
\
virtual SmartEnupla<TipoIterador,      double> getVetor(TipoIterador a_iterador,      double a_valor)const{ throw std::invalid_argument("AttVetor_Nenhum_Struct::getVetor(): \nEstrutura de dados nao definida.");}; \
virtual SmartEnupla<TipoIterador,         int> getVetor(TipoIterador a_iterador,         int a_valor)const{ throw std::invalid_argument("AttVetor_Nenhum_Struct::getVetor(): \nEstrutura de dados nao definida.");}; \
virtual SmartEnupla<TipoIterador,        bool> getVetor(TipoIterador a_iterador,        bool a_valor)const{ throw std::invalid_argument("AttVetor_Nenhum_Struct::getVetor(): \nEstrutura de dados nao definida.");}; \
virtual SmartEnupla<TipoIterador, std::string> getVetor(TipoIterador a_iterador, std::string a_valor)const{ throw std::invalid_argument("AttVetor_Nenhum_Struct::getVetor(): \nEstrutura de dados nao definida.");}; \
virtual SmartEnupla<TipoIterador,     Periodo> getVetor(TipoIterador a_iterador,     Periodo a_valor)const{ throw std::invalid_argument("AttVetor_Nenhum_Struct::getVetor(): \nEstrutura de dados nao definida.");}; \



#define DECLARA_METODOS_AttVetor(Classe, Atributo, valores) \
public: \
struct Atributo##Classe##_Struct{\
	Atributo##Classe##PreDataStruct Atributo##Classe##PreData; \
\
	virtual ~Atributo##Classe##_Struct(){}; \
\
	virtual bool addElementoFromString(const std::string a_iterador, const std::string a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addElementoFromString(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool addElementoFromString(const std::string a_iterador, const std::string a_valor, const int a_alocacao) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addElementoFromString(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool clear()                                                                        { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::clear(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool addElemento()                                                                  { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual std::vector<std::vector<std::string>> getStringsFromElementos(const bool a_incluirAtributo, const bool a_incluirIteradores, const std::string a_iteradorInicial, const std::string a_iteradorFinal) const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getStringsFromElementos(): \nEstrutura de dados nao definida.");}; \
	virtual int getSizeVetor()const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida.");}; \
\
	virtual bool addElemento(const Periodo     a_iterador,             const          IdRealizacao a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool addElemento(const Periodo     a_iterador,             const          TipoRelaxacaoVariavelAleatoria a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool addElemento(const Periodo     a_iterador,             const          TipoDetalhamentoProducaoHidreletrica a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool addElemento(const Periodo     a_iterador,             const          TipoDetalhamentoProducaoTermeletrica a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool addElemento(const Periodo     a_iterador,             const             IdEstagio a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool addElemento(const IdCenario   a_iterador,             const             IdCenario a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool addElemento(const int         a_iterador,             const      IdUnidadeRealUTE a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool addElemento(const int         a_iterador,             const        IdHidreletrica a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool addElemento(const int         a_iterador,             const        IdTermeletrica a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addElemento(): \nEstrutura de dados nao definida."); return false; }; \
    virtual bool addElemento(const int         a_iterador,             const         IdIntercambio a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool addElemento(const int         a_iterador,             const          IdSubmercado a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool addElemento(const IdNo        a_iterador,             const		  IdRealizacao a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool addElemento(const int         a_iterador,             const		IdCorteBenders a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool addElemento(const int         a_iterador,             const	  IdVariavelEstado a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool addElemento(const int         a_iterador,             const	  IdRealizacao     a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool addElemento(const IdProcesso  a_iterador,             const	  IdCenario     a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool addElemento(const IdEstagio   a_iterador,             const	  TipoProcessamentoParalelo     a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::addElemento(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setElemento(const Periodo   a_iterador,             const          IdRealizacao a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setElemento(const Periodo   a_iterador,             const          TipoRelaxacaoVariavelAleatoria a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setElemento(const Periodo   a_iterador,             const          TipoDetalhamentoProducaoHidreletrica a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setElemento(const Periodo   a_iterador,             const          TipoDetalhamentoProducaoTermeletrica a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setElemento(const Periodo   a_iterador,             const             IdEstagio a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setElemento(const IdCenario a_iterador,             const             IdCenario a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setElemento(const IdProcesso a_iterador,             const             IdCenario a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setElemento(const int       a_iterador,             const       IdUnidadeRealUTE a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setElemento(const int       a_iterador,             const        IdHidreletrica a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setElemento(const int       a_iterador,             const        IdTermeletrica a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setElemento(const int       a_iterador,             const         IdIntercambio a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setElemento(const int       a_iterador,             const          IdSubmercado a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setElemento(const IdNo      a_iterador,             const          IdRealizacao a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setElemento(const int       a_iterador,             const        IdCorteBenders a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setElemento(const int       a_iterador,             const      IdVariavelEstado a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setElemento(const int       a_iterador,             const      IdRealizacao     a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setElemento(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setElemento(const IdEstagio a_iterador,             const      TipoProcessamentoParalelo     a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setElemento(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual IdRealizacao          getElementoVetor(const Periodo   a_iterador, const          IdRealizacao a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getElementoVetor(): \nEstrutura de dados nao definida.");}; \
	virtual TipoRelaxacaoVariavelAleatoria             getElementoVetor(const Periodo   a_iterador, const             TipoRelaxacaoVariavelAleatoria a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getElementoVetor(): \nEstrutura de dados nao definida.");}; \
	virtual IdEstagio             getElementoVetor(const Periodo   a_iterador, const             IdEstagio a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getElementoVetor(): \nEstrutura de dados nao definida.");}; \
	virtual IdCenario             getElementoVetor(const IdCenario a_iterador, const             IdCenario a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getElementoVetor(): \nEstrutura de dados nao definida.");}; \
	virtual IdCenario             getElementoVetor(const IdProcesso a_iterador, const             IdCenario a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getElementoVetor(): \nEstrutura de dados nao definida.");}; \
	virtual TipoDetalhamentoProducaoHidreletrica  getElementoVetor(const Periodo   a_iterador, const        TipoDetalhamentoProducaoHidreletrica a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getElementoVetor(): \nEstrutura de dados nao definida.");}; \
	virtual IdHidreletrica        getElementoVetor(const int       a_iterador, const        IdHidreletrica a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getElementoVetor(): \nEstrutura de dados nao definida.");}; \
	virtual IdUnidadeRealUTE      getElementoVetor(const int       a_iterador, const      IdUnidadeRealUTE a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getElementoVetor(): \nEstrutura de dados nao definida.");}; \
	virtual IdTermeletrica        getElementoVetor(const int       a_iterador, const        IdTermeletrica a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getElementoVetor(): \nEstrutura de dados nao definida.");}; \
	virtual TipoDetalhamentoProducaoTermeletrica getElementoVetor(const Periodo   a_iterador, const     TipoDetalhamentoProducaoTermeletrica a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getElementoVetor(): \nEstrutura de dados nao definida.");}; \
	virtual IdIntercambio         getElementoVetor(const int       a_iterador, const         IdIntercambio a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getElementoVetor(): \nEstrutura de dados nao definida.");}; \
	virtual IdSubmercado          getElementoVetor(const int       a_iterador, const          IdSubmercado a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getElementoVetor(): \nEstrutura de dados nao definida.");}; \
	virtual IdRealizacao		  getElementoVetor(const IdNo      a_iterador, const		  IdRealizacao a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getElementoVetor(): \nEstrutura de dados nao definida.");}; \
	virtual IdCorteBenders		  getElementoVetor(const int       a_iterador, const		IdCorteBenders a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getElementoVetor(): \nEstrutura de dados nao definida.");}; \
	virtual IdVariavelEstado	  getElementoVetor(const int       a_iterador, const	  IdVariavelEstado a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getElementoVetor(): \nEstrutura de dados nao definida.");}; \
	virtual IdRealizacao     	  getElementoVetor(const int       a_iterador, const	  IdRealizacao     a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getElementoVetor(): \nEstrutura de dados nao definida.");}; \
	virtual TipoProcessamentoParalelo getElementoVetor(const IdEstagio       a_iterador, const	  TipoProcessamentoParalelo     a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getElementoVetor(): \nEstrutura de dados nao definida.");}; \
\
	virtual bool setVetor(const SmartEnupla<Periodo,       TipoRelaxacaoVariavelAleatoria> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor(const SmartEnupla<Periodo,       TipoDetalhamentoProducaoTermeletrica> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor(const SmartEnupla<Periodo,       TipoDetalhamentoProducaoHidreletrica> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor(const SmartEnupla<Periodo,       IdRealizacao> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor(const SmartEnupla<Periodo,          IdEstagio> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor(const SmartEnupla<IdCenario,        IdCenario> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor(const SmartEnupla<IdProcesso,        IdCenario> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor(const SmartEnupla<int,         IdHidreletrica> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor(const SmartEnupla<int,       IdUnidadeRealUTE> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor(const SmartEnupla<int,         IdTermeletrica> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor(const SmartEnupla<int,           IdSubmercado> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor(const SmartEnupla<int,          IdIntercambio> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor(const SmartEnupla<IdNo,          IdRealizacao> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor(const SmartEnupla<int,         IdCorteBenders> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor(const SmartEnupla<int,       IdVariavelEstado> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor(const SmartEnupla<int,           IdRealizacao> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor(const SmartEnupla<IdEstagio,           TipoProcessamentoParalelo> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual bool setVetor_forced(const SmartEnupla<Periodo,       TipoRelaxacaoVariavelAleatoria> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor_forced(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor_forced(const SmartEnupla<Periodo,       TipoDetalhamentoProducaoTermeletrica> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor_forced(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor_forced(const SmartEnupla<Periodo,       TipoDetalhamentoProducaoHidreletrica> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor_forced(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor_forced(const SmartEnupla<Periodo,       IdRealizacao> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor_forced(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor_forced(const SmartEnupla<Periodo,          IdEstagio> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor_forced(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor_forced(const SmartEnupla<IdCenario,        IdCenario> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor_forced(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor_forced(const SmartEnupla<IdProcesso,        IdCenario> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor_forced(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor_forced(const SmartEnupla<int,         IdHidreletrica> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor_forced(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor_forced(const SmartEnupla<int,       IdUnidadeRealUTE> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor_forced(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor_forced(const SmartEnupla<int,         IdTermeletrica> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor_forced(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor_forced(const SmartEnupla<int,           IdSubmercado> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor_forced(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor_forced(const SmartEnupla<int,          IdIntercambio> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor_forced(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor_forced(const SmartEnupla<IdNo,          IdRealizacao> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor_forced(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor_forced(const SmartEnupla<int,         IdCorteBenders> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor_forced(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor_forced(const SmartEnupla<int,       IdVariavelEstado> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor_forced(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor_forced(const SmartEnupla<int,           IdRealizacao> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor_forced(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setVetor_forced(const SmartEnupla<IdEstagio,           TipoProcessamentoParalelo> &a_vetor){ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setVetor_forced(): \nEstrutura de dados nao definida."); return false; }; \
\
	virtual SmartEnupla<Periodo,       TipoRelaxacaoVariavelAleatoria> getVetor(Periodo   a_iterador,          TipoRelaxacaoVariavelAleatoria a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getVetor(): \nEstrutura de dados nao definida."); }; \
	virtual SmartEnupla<Periodo,       TipoDetalhamentoProducaoTermeletrica> getVetor(Periodo   a_iterador,          TipoDetalhamentoProducaoTermeletrica a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getVetor(): \nEstrutura de dados nao definida."); }; \
	virtual SmartEnupla<Periodo,       TipoDetalhamentoProducaoHidreletrica> getVetor(Periodo   a_iterador,          TipoDetalhamentoProducaoHidreletrica a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getVetor(): \nEstrutura de dados nao definida."); }; \
	virtual SmartEnupla<Periodo,       IdRealizacao> getVetor(Periodo   a_iterador,          IdRealizacao a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getVetor(): \nEstrutura de dados nao definida."); }; \
	virtual SmartEnupla<Periodo,          IdEstagio> getVetor(Periodo   a_iterador,             IdEstagio a_valor)const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getVetor(): \nEstrutura de dados nao definida."); }; \
	virtual SmartEnupla<IdCenario,        IdCenario> getVetor(IdCenario a_iterador,             IdCenario a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getVetor(): \nEstrutura de dados nao definida.");}; \
	virtual SmartEnupla<IdProcesso,        IdCenario> getVetor(IdProcesso a_iterador,             IdCenario a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getVetor(): \nEstrutura de dados nao definida.");}; \
	virtual SmartEnupla<int,       IdUnidadeRealUTE> getVetor(int       a_iterador,      IdUnidadeRealUTE a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getVetor(): \nEstrutura de dados nao definida.");}; \
	virtual SmartEnupla<int,         IdHidreletrica> getVetor(int       a_iterador,        IdHidreletrica a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getVetor(): \nEstrutura de dados nao definida.");}; \
	virtual SmartEnupla<int,         IdTermeletrica> getVetor(int       a_iterador,        IdTermeletrica a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getVetor(): \nEstrutura de dados nao definida.");}; \
	virtual SmartEnupla<int,          IdIntercambio> getVetor(int       a_iterador,         IdIntercambio a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getVetor(): \nEstrutura de dados nao definida.");}; \
	virtual SmartEnupla<int,           IdSubmercado> getVetor(int       a_iterador,          IdSubmercado a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getVetor(): \nEstrutura de dados nao definida.");}; \
	virtual SmartEnupla<IdNo,          IdRealizacao> getVetor(IdNo      a_iterador,          IdRealizacao a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getVetor(): \nEstrutura de dados nao definida.");}; \
	virtual SmartEnupla<int,         IdCorteBenders> getVetor(int       a_iterador,        IdCorteBenders a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getVetor(): \nEstrutura de dados nao definida.");}; \
	virtual SmartEnupla<int,       IdVariavelEstado> getVetor(int       a_iterador,      IdVariavelEstado a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getVetor(): \nEstrutura de dados nao definida.");}; \
	virtual SmartEnupla<int,           IdRealizacao> getVetor(int       a_iterador,          IdRealizacao a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getVetor(): \nEstrutura de dados nao definida.");}; \
	virtual SmartEnupla<IdEstagio, TipoProcessamentoParalelo> getVetor(IdEstagio       a_iterador,          TipoProcessamentoParalelo a_valor)const{ throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getVetor(): \nEstrutura de dados nao definida.");}; \
\
LISTA_ID            (DECLARA_METODOS_POR_ITERADOR_POR_TIPO_ARGUMENTO_ELEMENTAR_AttVetor) \
LISTA_TIPO          (DECLARA_METODOS_POR_ITERADOR_POR_TIPO_ARGUMENTO_ELEMENTAR_AttVetor) \
LISTA_TIPO_ELEMENTAR(DECLARA_METODOS_POR_ITERADOR_POR_TIPO_ARGUMENTO_ELEMENTAR_AttVetor) \
};\
valores(DECLARA_STRUCT_AttVetor_DERIVADA) DECLARA_STRUCT_AttVetor_NENHUM(Classe, Atributo) \
DEFINE_METODOS_AttVetor(Classe, Atributo) \
private: \
std::vector<Atributo##Classe##_Struct*> lista##Atributo##Classe; \
public:


#define DEFINE_PTR_STRUCT_AttVetor(Classe, Atributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, TipoIterador) Atributo##Classe##_##nomeAtributo##_Struct* Atributo##Classe##_##nomeAtributo##_struct = new Atributo##Classe##_##nomeAtributo##_Struct;
#define PTR_STRUCT_AttVetor(Classe, Atributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, TipoIterador) Atributo##Classe##_##nomeAtributo##_struct,

#define INICIALIZA_AttVetor(Classe, Atributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, TipoIterador) 

#define INICIALIZAR_AttVetor(Classe, Atributo, valores) \
try{ \
	valores(DEFINE_PTR_STRUCT_AttVetor) \
	Atributo##Classe##_Nenhum_Struct* Atributo##Classe##_Nenhum_struct = new Atributo##Classe##_Nenhum_Struct; \
	Atributo##Classe##_Nenhum_Struct* Atributo##Classe##_Excedente_struct = new Atributo##Classe##_Nenhum_Struct; \
	lista##Atributo##Classe = {Atributo##Classe##_Nenhum_struct, valores(PTR_STRUCT_AttVetor) Atributo##Classe##_Excedente_struct}; \
	valores(INICIALIZA_AttVetor) \
} \
catch (const std::exception & erro) { throw std::invalid_argument("INICIALIZAR_" + std::string(#Atributo) + std::string(#Classe) + " " + std::string(erro.what())); }

#define EXCLUIR_AttVetor(Classe, Atributo, valores)\
for (int i = 1; i < int(lista##Atributo##Classe.size()); i++) { \
	if (lista##Atributo##Classe.at(i) != nullptr){\
		delete lista##Atributo##Classe.at(i); \
		lista##Atributo##Classe.at(i) = nullptr; \
	}\
}

#define TRANSFERIR_VALORES_AttVetor(Classe, Atributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, TipoIterador) \
try{ \
	if (instanciaCopiar.lista##Atributo##Classe.size() > 0) \
		lista##Atributo##Classe.at(Atributo##Classe##_##nomeAtributo)->setVetor_forced(instanciaCopiar.lista##Atributo##Classe.at(Atributo##Classe##_##nomeAtributo)->getVetor(TipoIterador(), TipoValor())); \
} \
catch (const std::exception & erro) { throw std::invalid_argument("TRANSFERIR_VALORES_" + std::string(#Atributo) + std::string(#Classe) + " " + std::string(erro.what())); }

#define COPIAR_AttVetor(Classe, Atributo, valores)\
try { \
	EXCLUIR_AttVetor(Classe, Atributo, valores)\
	INICIALIZAR_AttVetor(Classe, Atributo, valores) \
	valores(TRANSFERIR_VALORES_AttVetor)\
}\
catch (const std::exception& erro) { throw std::invalid_argument("COPIAR_" + std::string(#Atributo) + std::string(#Classe) + " " + std::string(erro.what())); }\




#endif