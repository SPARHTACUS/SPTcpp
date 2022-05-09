#ifndef SMART_ATT_COMUM_H
#define SMART_ATT_COMUM_H

#include "C_Auxiliar.h"
#include "C_SmartEnum.h"
#include "C_Periodo.h"

#include <vector>
#include <string>

typedef std::string string;


// ----------------------------------------------
// 
// Macros para criação de SMART ATRIBUTO AttComum 
//
// ----------------------------------------------


#define DEFINE_ATRIBUTO_SMART_ATRIBUTO(Classe, SmartAtributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, valorObrigatorio) SmartAtributo##Classe##_##nomeAtributo,
#define DEFINE_CHAR_SMART_ATRIBUTO(Classe, SmartAtributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, valorObrigatorio) #nomeAtributo,
#define DEFINE_CHARS_SMART_ATRIBUTO(Classe, SmartAtributo, valores) static const char* SmartAtributo##Classe##Array [] = { "Nenhum", valores(DEFINE_CHAR_SMART_ATRIBUTO) "Excedente" };

#define DEFINE_PRE_DATA_SMART_ATRIBUTO(Classe, SmartAtributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, valorObrigatorio) \
TipoValor nomeAtributo##_LB;\
TipoValor nomeAtributo##_UB;\
TipoValor nomeAtributo##_IV;\
bool      nomeAtributo##_MR;

#define DEFINE_PRE_DATA_CONSTRUTOR_SMART_ATRIBUTO(Classe, SmartAtributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, valorObrigatorio) \
nomeAtributo##_LB = get##TipoValor##FromChar(#valorMinimo);\
nomeAtributo##_UB = get##TipoValor##FromChar(#valorMaximo);\
nomeAtributo##_IV = get##TipoValor##FromChar(#valorInicial);\
nomeAtributo##_MR = getboolFromChar(#valorObrigatorio);

#define DEFINE_CHAR_VALUEMR_SMART_ATRIBUTO(Classe, SmartAtributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, valorObrigatorio) #valorObrigatorio,

#define DEFINE_ESTRUTURA_PRE_DATA_SMART_ATRIBUTO(Classe, SmartAtributo, valores) \
struct  SmartAtributo##Classe##PreDataStruct { \
	valores(DEFINE_PRE_DATA_SMART_ATRIBUTO) \
	SmartAtributo##Classe##PreDataStruct(){ valores(DEFINE_PRE_DATA_CONSTRUTOR_SMART_ATRIBUTO) }; \
}; \
static const char* SmartAtributo##Classe##Array_MR[] = { "nao", valores(DEFINE_CHAR_VALUEMR_SMART_ATRIBUTO) "nao" };

#define DEFINE_SMART_AttComum(Classe, SmartAtributo, valores) \
enum SmartAtributo##Classe { SmartAtributo##Classe##_Nenhum, valores(DEFINE_ATRIBUTO_SMART_ATRIBUTO) SmartAtributo##Classe##_Excedente }; \
DEFINE_CHARS_SMART_ATRIBUTO(Classe, SmartAtributo, valores) \
DEFINE_ESTRUTURA_PRE_DATA_SMART_ATRIBUTO(Classe, SmartAtributo, valores) \
DEFINE_GET_SMART_ENUM_FROM_CHAR(SmartAtributo##Classe) \
DEFINE_GET_STRING_SMART_ENUM(SmartAtributo##Classe) \
DEFINE_GET_FULL_STRING_SMART_ENUM(SmartAtributo##Classe) \
DEFINE_GET_STRING_TIPO_SMART_ENUM(SmartAtributo##Classe) \
DEFINE_OPERADOR_SMART_ENUM_MAIS_MAIS(SmartAtributo##Classe) \


// -------------------------------------------------------
//
// Macros para criação dos métodos de AttComum nas classes
//
// -------------------------------------------------------


#define DEFINE_METODOS_AttComum(Classe, Atributo) \
Id##Classe getIdObjeto()const{ \
	try { return lista##Atributo##Classe.at(Atributo##Classe##_id##Classe)->getAtributo(Id##Classe()); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIdObjeto(): \n" + std::string(erro.what())); }\
}; \
virtual void addDadosAttComum(const std::string a_idMembro, const std::vector<std::string> &a_atributo, const std::vector<std::string> &a_vlrDado){  \
	std::string atributoStr = ""; \
	std::string vlrDadoStr  = ""; \
	try { \
		const int numAtributos = int(a_atributo.size()); \
		std::vector<bool> atributosLidos = std::vector<bool>(int(AttComum##Classe##_Excedente) + 1, false); \
		AttComum##Classe atributo = AttComum##Classe##_id##Classe; \
		if ((numAtributos == 0) || (a_vlrDado.size() == 0)) { throw std::invalid_argument("Agumentos a_atributo e a_vlrDados sem valores."); } \
		else if (numAtributos != int(a_vlrDado.size())) { throw std::invalid_argument("Vetores de atributos e valores de diferente tamanhos."); } \
		else { \
			setAtributoFromString(atributo, a_idMembro); \
			atributosLidos.at(atributo) = true; \
			for (int i = 0; i < numAtributos; i++) { \
				atributoStr = a_atributo.at(i); \
				vlrDadoStr  =  a_vlrDado.at(i); \
				atributo = getAttComum##Classe##FromChar(atributoStr.c_str()); \
				setAtributoFromString(atributo, vlrDadoStr); \
				atributosLidos.at(atributo) = true; \
			} \
			for (int i = 1; i < AttComum##Classe##_Excedente; i++){ \
				if ((getboolFromChar(AttComum##Classe##Array_MR[i]) == true) && (atributosLidos.at(i) == false)){ \
					atributoStr = ""; \
					vlrDadoStr  = ""; \
					throw std::invalid_argument("Atributo Comum em " + std::string(#Classe) + " - " + getString(AttComum##Classe(i)) + " - nao foi informado."); \
				} \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttComum(" + atributoStr + ","  + vlrDadoStr + "): \n" + std::string(erro.what())); }\
}; \
virtual void addDadosAttComum(const std::vector<std::string> &a_atributo, const std::vector<std::string> &a_vlrDado){  \
	std::string atributoStr = ""; \
	std::string vlrDadoStr  = ""; \
	try { \
		const int numAtributos = int(a_atributo.size()); \
		std::vector<bool> atributosLidos = std::vector<bool>(int(AttComum##Classe##_Excedente) + 1, false); \
		atributosLidos.at(AttComum##Classe##_id##Classe) = true; \
		AttComum##Classe atributo = AttComum##Classe##_Nenhum; \
		if ((numAtributos == 0) && (a_vlrDado.size() == 0)) { throw std::invalid_argument("Agumentos a_atributo e a_vlrDados sem valores."); } \
		else if (numAtributos != int(a_vlrDado.size())) { throw std::invalid_argument("Vetores de atributos e valores de diferente tamanhos."); } \
		else { \
			for (int i = 0; i < numAtributos; i++) { \
				atributoStr = a_atributo.at(i); \
				vlrDadoStr  =  a_vlrDado.at(i); \
				atributo = getAttComum##Classe##FromChar(atributoStr.c_str()); \
				setAtributoFromString(atributo, vlrDadoStr); \
				atributosLidos.at(atributo) = true; \
			} \
			for (int i = 1; i < AttComum##Classe##_Excedente; i++){ \
				if ((getboolFromChar(AttComum##Classe##Array_MR[i]) == true) && (atributosLidos.at(i) == false)){ \
					atributoStr = ""; \
					vlrDadoStr  = ""; \
					throw std::invalid_argument("Atributo Comum em " + std::string(#Classe) + " - " + getString(AttComum##Classe(i)) + " - nao foi informado."); \
				} \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttComum(" + atributoStr + ","  + vlrDadoStr + "): \n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttComum(const bool a_incluirCabecalho){  \
	try { \
			int numLinhas = 1; \
			if (a_incluirCabecalho) \
				numLinhas = 2; \
			std::vector<std::vector<std::string>> matrizRetorno(numLinhas, std::vector<std::string>(int(AttComum##Classe##_Excedente) - 1, "")); \
			for (int i = 1; i < AttComum##Classe##_Excedente; i++) {\
				if (a_incluirCabecalho) \
					matrizRetorno.at(0).at(i-1) = getString(AttComum##Classe(i)); \
				matrizRetorno.at(numLinhas-1).at(i-1) = getStringFromAtributo(AttComum##Classe(i)); \
			} \
			return matrizRetorno; \
		} \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttComum(" + getString(a_incluirCabecalho) + "): \n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttComum(const bool a_incluirCabecalho, const std::vector<std::string> &a_atributos){  \
	try { \
			if (a_atributos.size() == 0)\
				return getDadosAttComum(a_incluirCabecalho); \
			const int numAtributos = int(a_atributos.size()); \
			int numLinhas = 1; \
			if (a_incluirCabecalho) \
				numLinhas = 2; \
			std::vector<std::vector<std::string>> matrizRetorno(numLinhas, std::vector<std::string>(numAtributos, "")); \
			for (int i = 0; i < numAtributos; i++){ \
				if (a_incluirCabecalho) \
					matrizRetorno.at(0).at(i) = a_atributos.at(i); \
				AttComum##Classe atributo = get##AttComum##Classe##FromChar(a_atributos.at(i).c_str()); \
				matrizRetorno.at(numLinhas-1).at(i) = getStringFromAtributo(atributo); \
			} \
			return matrizRetorno; \
		} \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttComum(" + getString(a_incluirCabecalho) + ",a_atributos): \n" + std::string(erro.what())); }\
}; \
void setAtributoFromString(const Atributo##Classe a_tipoAtributo, const std::string a_valor){  \
	try { lista##Atributo##Classe.at(a_tipoAtributo)->setAtributoFromString(a_valor);  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::setAtributoFromString(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + ","  + a_valor + "): \n" + std::string(erro.what())); }\
}; \
void setAtributo(const Atributo##Classe a_tipoAtributo){ \
	try { lista##Atributo##Classe.at(a_tipoAtributo)->setAtributo(); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::setAtributo(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "): \n" + std::string(erro.what())); }\
}; \
template<typename TipoValor> \
void setAtributo(const Atributo##Classe a_tipoAtributo, const TipoValor a_valor){ \
	try { lista##Atributo##Classe.at(a_tipoAtributo)->setAtributo(a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::setAtributo(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }\
}; \
std::string getStringFromAtributo(const Atributo##Classe a_tipoAtributo){  \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getStringFromAtributo();  } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getStringFromAtributo(" + std::string(#Atributo) + "): \n" + std::string(erro.what())); }\
}; \
template<typename TipoValor> \
TipoValor getAtributo(const Atributo##Classe a_tipoAtributo, const TipoValor a_valor)const{ \
	try { return lista##Atributo##Classe.at(a_tipoAtributo)->getAtributo(a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getAtributo(" + std::string(#Atributo)  + "_" + getString(a_tipoAtributo) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }\
}; \


// ------------------------------------------------------------------
//
// Macros para Declaração de Structs com Métodos Virtuais de AttComum
//
// ------------------------------------------------------------------


#define DECLARA_STRUCT_AttComum_DERIVADA(Classe, Atributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, valorObrigatorio) \
struct Atributo##Classe##_##nomeAtributo##_Struct: Atributo##Classe##_Struct{ \
	virtual bool setAtributoFromString(const std::string a_valor) { \
		try{ \
			if (strCompara(a_valor.c_str(), "NaN") || strCompara(a_valor.c_str(), "-nan(ind)")) { throw std::invalid_argument("Valor NaN."); }\
			const TipoValor valor = get##TipoValor##FromChar(a_valor.c_str()); \
			if (valor < Atributo##Classe##PreData.nomeAtributo##_LB) { throw std::invalid_argument("Valor inferior ao valor minimo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else if (valor > Atributo##Classe##PreData.nomeAtributo##_UB) { throw std::invalid_argument("Valor superior ao valor maximo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else { dados = valor; } \
			alreadySet = true; \
			return true;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::setAtributoFromString(" + std::string(a_valor) + "): \n" + std::string(erro.what())); }\
	};\
	virtual bool setAtributo() { \
		try{ \
			const TipoValor valor = Atributo##Classe##PreData.nomeAtributo##_IV;\
			if (valor < Atributo##Classe##PreData.nomeAtributo##_LB) { throw std::invalid_argument("Valor inicial inferior ao valor minimo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else if (valor > Atributo##Classe##PreData.nomeAtributo##_UB) { throw std::invalid_argument("Valor inicial superior ao valor maximo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else { dados = valor; } \
			alreadySet = true; \
			return true; \
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::setAtributo(): \n" + std::string(erro.what())); }\
	}; \
	virtual bool setAtributo(const TipoValor a_valor) { \
		try { \
			if (a_valor < Atributo##Classe##PreData.nomeAtributo##_LB) { throw std::invalid_argument("Valor inferior ao valor minimo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else if (a_valor > Atributo##Classe##PreData.nomeAtributo##_UB) { throw std::invalid_argument("Valor superior ao valor maximo do atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) ); }\
			else { dados = a_valor; } \
			alreadySet = true; \
			return true;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::setAtributo(a_valor): \n" + std::string(erro.what())); }\
	}; \
	virtual std::string getStringFromAtributo()const { \
		try{\
			if (alreadySet == false) { throw std::invalid_argument("O atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + " nao foi inicializado."); } \
			return getString(dados); \
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::getStringFromAtributo(): \n" + std::string(erro.what())); }\
	}; \
	virtual TipoValor getAtributo(const TipoValor a_valor)const { \
		try{\
			if (alreadySet == false) { throw std::invalid_argument("O atributo " + std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + " nao foi inicializado."); } \
			return dados; \
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_" + std::string(#nomeAtributo) + "_Struct::getAtributo(a_valor): \n" + std::string(erro.what())); }\
	}; \
	~Atributo##Classe##_##nomeAtributo##_Struct(){};\
	TipoValor dados; \
	bool alreadySet = false; \
};

#define DECLARA_STRUCT_AttComum_NENHUM(Classe, Atributo) \
struct Atributo##Classe##_Nenhum_Struct: Atributo##Classe##_Struct{ ~Atributo##Classe##_Nenhum_Struct(){}; };


#define DECLARA_METODOS_TIPO_ARGUMENTO_AttComum(TipoArgumento) \
virtual bool          setAtributo(const TipoArgumento a_valor)      { throw std::invalid_argument("AttComum_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); }; \
virtual TipoArgumento getAtributo(const TipoArgumento a_valor)const { throw std::invalid_argument("AttComum_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida."); }; \


#define DECLARA_METODOS_AttComum(Classe, Atributo, valores) \
public: \
struct Atributo##Classe##_Struct{\
	Atributo##Classe##PreDataStruct Atributo##Classe##PreData; \
\
	virtual ~Atributo##Classe##_Struct(){};\
	virtual bool setAtributoFromString(const std::string         a_valor) { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributoFromString(): \nEstrutura de dados nao definida."); return false; }; \
	virtual bool setAtributo()                                            { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::setAtributo(): \nEstrutura de dados nao definida."); return false; }; \
	virtual std::string  getStringFromAtributo()                    const { throw std::invalid_argument(std::string(#Atributo) + std::string(#Classe) + "_Nenhum_Struct::getAtributo(): \nEstrutura de dados nao definida."); }; \
    LISTA_ID            (DECLARA_METODOS_TIPO_ARGUMENTO_AttComum) \
	LISTA_TIPO          (DECLARA_METODOS_TIPO_ARGUMENTO_AttComum) \
	LISTA_TIPO_ELEMENTAR(DECLARA_METODOS_TIPO_ARGUMENTO_AttComum) \
};\
valores(DECLARA_STRUCT_AttComum_DERIVADA) DECLARA_STRUCT_AttComum_NENHUM(Classe, Atributo) \
DEFINE_METODOS_AttComum(Classe, Atributo) \
private: \
std::vector<Atributo##Classe##_Struct*> lista##Atributo##Classe; \
public:


#define DEFINE_PTR_STRUCT_AttComum(Classe, Atributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, valorObrigatorio) Atributo##Classe##_##nomeAtributo##_Struct* Atributo##Classe##_##nomeAtributo##_struct = new Atributo##Classe##_##nomeAtributo##_Struct;
#define PTR_STRUCT_AttComum(Classe, Atributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, valorObrigatorio) Atributo##Classe##_##nomeAtributo##_struct,

#define INICIALIZA_AttComum(Classe, Atributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, valorObrigatorio) lista##Atributo##Classe.at(Atributo##Classe##_##nomeAtributo)->setAtributo();

#define INICIALIZAR_AttComum(Classe, Atributo, valores) \
try{ \
	valores(DEFINE_PTR_STRUCT_AttComum) \
	Atributo##Classe##_Nenhum_Struct* Atributo##Classe##_Nenhum_struct = new Atributo##Classe##_Nenhum_Struct; \
	Atributo##Classe##_Nenhum_Struct* Atributo##Classe##_Excedente_struct = new Atributo##Classe##_Nenhum_Struct; \
	lista##Atributo##Classe = {Atributo##Classe##_Nenhum_struct, valores(PTR_STRUCT_AttComum) Atributo##Classe##_Excedente_struct}; \
	valores(INICIALIZA_AttComum) \
} \
catch (const std::exception & erro) { throw std::invalid_argument("INICIALIZAR_" + std::string(#Atributo) + std::string(#Classe) + " " + std::string(erro.what())); }

#define EXCLUIR_AttComum(Classe, Atributo, valores)\
for (int i = 1; i < int(lista##Atributo##Classe.size()); i++) { \
	if (lista##Atributo##Classe.at(i) != nullptr){\
		delete lista##Atributo##Classe.at(i); \
		lista##Atributo##Classe.at(i) = nullptr; \
	}\
} 

#define TRANSFERIR_VALORES_AttComum(Classe, Atributo, nomeAtributo, TipoValor, valorMinimo, valorMaximo, valorInicial, valorObrigatorio) \
try{ \
	if (instanciaCopiar.lista##Atributo##Classe.size() > 0) \
		lista##Atributo##Classe.at(Atributo##Classe##_##nomeAtributo)->setAtributo(instanciaCopiar.lista##Atributo##Classe.at(Atributo##Classe##_##nomeAtributo)->getAtributo(TipoValor())); \
} \
catch (const std::exception & erro) { throw std::invalid_argument("TRANSFERIR_VALORES_" + std::string(#Atributo) + std::string(#Classe) + " " + std::string(erro.what())); }

#define COPIAR_AttComum(Classe, Atributo, valores)\
try{ \
	EXCLUIR_AttComum(Classe, Atributo, valores)\
	INICIALIZAR_AttComum(Classe, Atributo, valores) \
	valores(TRANSFERIR_VALORES_AttComum)\
}\
catch (const std::exception& erro) { throw std::invalid_argument("COPIAR_" + std::string(#Atributo) + std::string(#Classe) + " " + std::string(erro.what())); }


#endif  