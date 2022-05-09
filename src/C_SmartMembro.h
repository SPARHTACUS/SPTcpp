#ifndef C_SMARTATRIBUTOS_H
#define C_SMARTATRIBUTOS_H

#include "C_Auxiliar.h"
#include "C_SmartEnum.h"

#include "C_Periodo.h"
#include "C_SmartAttComum.h"
#include "C_SmartAttVetor.h"
#include "C_SmartAttMatriz.h"

#include <vector>
#include <string>


#define DEFINE_STRUCT_MEMBROS(Classe, Membro) \
struct Vetor##Membro##Em##Classe {\
friend class Classe;\
friend class Dados;\
friend class LeituraCEPEL;\
	public:\
	Vetor##Membro##Em##Classe(){ \
		maiorId = Id##Membro##_Nenhum;\
		vetorObjeto = std::vector<std::vector<Membro>>();\
        numero_objetos = 0; \
	} \
	Vetor##Membro##Em##Classe(const Vetor##Membro##Em##Classe &instanciaCopiar){ \
		maiorId = instanciaCopiar.maiorId;\
		vetorObjeto = instanciaCopiar.vetorObjeto;\
        numero_objetos = instanciaCopiar.numero_objetos; \
	} \
	~Vetor##Membro##Em##Classe(){ \
		maiorId = Id##Membro##_Nenhum;\
		for (int i = 0; i < int(vetorObjeto.size()); i++) {\
			if (vetorObjeto.at(i).size() > 0) \
				vetorObjeto.at(i).at(0).esvaziar(); \
		} \
		vetorObjeto = std::vector<std::vector<Membro>>();\
		numero_objetos = 0; \
	} \
	void esvaziar(){ \
		try { \
			maiorId = Id##Membro##_Nenhum;\
			for (int i = 0; i < int(vetorObjeto.size()); i++) {\
				if (vetorObjeto.at(i).size() > 0) \
					vetorObjeto.at(i).at(0).esvaziar(); \
			} \
			vetorObjeto = std::vector<std::vector<Membro>>();\
			numero_objetos = 0; \
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::Vetor" + std::string(#Membro) + "::esvaziar(): \n" + std::string(erro.what())); }\
	} \
	void alocar(const int a_numero_objetos){ \
		try { \
			if (vetorObjeto.size() > 1) \
				throw std::invalid_argument("Alocacao nao pode ocorrer com objetos ja instanciados."); \
			vetorObjeto = std::vector<std::vector<Membro>>(a_numero_objetos + 1, std::vector<Membro>()); \
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::Vetor" + std::string(#Membro) + "::alocar(" + getFullString(a_numero_objetos) + "): \n" + std::string(erro.what())); }\
	}; \
	int numObjetos() const { return numero_objetos; };\
	template<typename Atributo, typename Valor> \
	int numObjetos(Atributo a_atributo, Valor a_valor) { \
		if (maiorId == Id##Membro##_Nenhum) \
			return 0; \
		int numCont = 0; \
		for (Id##Membro id = Id##Membro(1); id <= maiorId; id++) {\
			if (isInstanciado(id)) { \
				if (vetorObjeto.at(id).at(0).getAtributo(a_atributo, Valor()) == a_valor) \
					numCont++; \
			}\
		} \
		return numCont; \
	};\
	Id##Membro getMaiorId()const{ return maiorId; };\
	std::vector<Id##Membro> getIdObjetos()const { \
		std::vector<Id##Membro> vetorIdMembro; \
		vetorIdMembro.reserve(int(maiorId));\
		for (Id##Membro id = Id##Membro(1); id <= maiorId; id++) {\
			if (isInstanciado(id)) \
				vetorIdMembro.push_back(id); \
		} \
		return vetorIdMembro; \
	};\
	template<typename Atributo, typename Valor> \
	std::vector<Id##Membro> getIdObjetos(const Atributo a_atributo, const Valor a_valor)const{ \
		if (maiorId == Id##Membro##_Nenhum) \
			return std::vector<Id##Membro>(); \
		std::vector<Id##Membro> vetorIdMembros_filtro; \
		vetorIdMembros_filtro.reserve(int(maiorId));\
		for (Id##Membro id = Id##Membro(1); id <= maiorId; id++) {\
			if (isInstanciado(id)) {\
				if (vetorObjeto.at(id).at(0).getAtributo(a_atributo, Valor()) == a_valor) \
					vetorIdMembros_filtro.push_back(id); \
			} \
		} \
		return vetorIdMembros_filtro; \
	};\
	void add(const Membro &a_objeto) {\
		try { \
			const Id##Membro idObjeto = a_objeto.getIdObjeto(); \
			if (idObjeto < Id##Membro(1)) \
				throw std::invalid_argument("Id invalido de objeto: menor que inicial."); \
			else if (idObjeto >= Id##Membro##_Excedente) \
				throw std::invalid_argument("Id invalido de objeto: maior ou igual a Excedente."); \
			if (idObjeto <= maiorId){ \
				if (isInstanciado(idObjeto))\
					throw std::invalid_argument("Objeto ja instanciado."); \
				else \
					vetorObjeto.at(idObjeto).push_back(a_objeto); \
			} \
			else{ \
				if (vetorObjeto.size() == 0) \
					vetorObjeto.push_back(std::vector<Membro>()); \
				for (Id##Membro id = Id##Membro(vetorObjeto.size()); id <= idObjeto; id++) \
					vetorObjeto.push_back(std::vector<Membro>()); \
				vetorObjeto.at(idObjeto).push_back(a_objeto); \
				maiorId = idObjeto;\
			} \
			numero_objetos++; \
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::Vetor" + std::string(#Membro) + "::add(Id" + std::string(#Membro) + "_" + getString(a_objeto.getIdObjeto()) + "): \n" + std::string(erro.what())); }\
	};\
	void rem(const Id##Membro a_idObjeto){ \
		try { \
			if (a_idObjeto == maiorId) { \
				for (Id##Membro id = maiorId; id >= Id##Membro(1); id--) {\
					if (isInstanciado(id)){ \
						maiorId = id; \
						break; \
					} \
				} \
            } \
			att(a_idObjeto).esvaziar(); \
			vetorObjeto.at(a_idObjeto) = std::vector<Membro>(); \
			numero_objetos--;\
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::Vetor" + std::string(#Membro) + "::rem(Id" + std::string(#Membro) + "_" + getString(a_idObjeto) + "): \n" + std::string(erro.what())); }\
	} \
	void set(const Membro a_objeto) {\
		try { \
			if (!isInstanciado(a_objeto.getIdObjeto())) \
				throw std::invalid_argument("Objeto nao instanciado."); \
			vetorObjeto.at(a_objeto.getIdObjeto()).at(0) = a_objeto; \
		}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::Vetor" + std::string(#Membro) + "::set(Id" + std::string(#Membro) + "_" + getString(a_objeto.getIdObjeto()) + "): \n" + std::string(erro.what())); }\
	};\
	Membro at(const Id##Membro a_idObjeto)const{\
		try { \
			if (!isInstanciado(a_idObjeto)) \
				throw std::invalid_argument("Objeto nao instanciado."); \
			return vetorObjeto.at(a_idObjeto).at(0); \
			}\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::Vetor" + std::string(#Membro) + "::at(Id" + std::string(#Membro) + "_" + getString(a_idObjeto) + "): \n" + std::string(erro.what())); }\
	};\
	bool isInstanciado(const Id##Membro a_idObjeto) const{ \
		try{ \
            if (vetorObjeto.size() <= a_idObjeto) \
				return false; \
			else if (vetorObjeto.at(a_idObjeto).size() > 0) \
				return true; \
			else \
				return false; \
		} \
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::Vetor" + std::string(#Membro) + "::isInstanciado(" + getFullString(a_idObjeto) + "): \n" + std::string(erro.what())); }\
	};\
	private:\
	Membro& att(const Id##Membro a_idObjeto){\
		try { \
			if (!isInstanciado(a_idObjeto)) \
				throw std::invalid_argument("Objeto nao instanciado."); \
			return vetorObjeto.at(a_idObjeto).at(0); \
		 }\
		catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::Vetor" + std::string(#Membro) + "::att(Id" + std::string(#Membro) + "_" + getString(a_idObjeto) + "): \n" + std::string(erro.what())); }\
	};\
	std::vector<std::vector<Membro>> vetorObjeto;\
	Id##Membro maiorId;\
	int numero_objetos; \
};


// -----------------------------------
// 
// Macros para criação de SMART MEMBRO 
//
// -----------------------------------


#define DEFINE_MEMBRO_SMART_MEMBRO(Classe, Membro) membro##Classe##_##Membro,
#define DEFINE_CHAR_SMART_MEMBRO(Classe, Membro) #Membro,
#define DEFINE_CHARS_SMART_MEMBRO(Classe, valores) static const char* membro##Classe##Array [] = { "Nenhum", valores(DEFINE_CHAR_SMART_MEMBRO) "Excedente" };

#define DEFINE_GET_SMART_MEMBRO_FROM_CHAR(Classe)\
static membro##Classe getMembro##Classe##FromChar(const char* a_char){ \
	for (int i = 1; i < membro##Classe##_Excedente; i++){ \
		if (strCompara(std::string(membro##Classe##Array[i]), std::string(a_char) )) { return membro##Classe(i); }\
	}\
	return membro##Classe##_Nenhum; \
};

#define DEFINE_GET_STRING_SMART_MEMBRO(Classe)\
static std::string getString(membro##Classe a_membro){ \
	if (a_membro >= membro##Classe##_Excedente) \
		return std::string(membro##Classe##Array[membro##Classe##_Excedente]); \
	else if (a_membro <= membro##Classe##_Nenhum) \
		return std::string(membro##Classe##Array[membro##Classe##_Nenhum]); \
	else \
		return std::string(membro##Classe##Array[a_membro]); \
};

#define DEFINE_SMART_Membro(Classe, SmartMembro, valores)\
enum membro##Classe { membro##Classe##_Nenhum, valores(DEFINE_MEMBRO_SMART_MEMBRO) membro##Classe##_Excedente };\
DEFINE_CHARS_SMART_MEMBRO(Classe, valores) \
DEFINE_GET_SMART_MEMBRO_FROM_CHAR(Classe) \
DEFINE_GET_STRING_SMART_MEMBRO(Classe) \
valores(DEFINE_STRUCT_MEMBROS)


// ---------------------------------------------
// 
// Listagem de Smart Membros e Métodos de Acesso
//
// ---------------------------------------------


#define DEFINE_METODOS_MEMBROS(Classe, Membro) \
Vetor##Membro##Em##Classe vetor##Membro; \
bool addDadosAttComum##Membro(const std::string a_membro, const std::string a_idMembro, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			if ((a_vetorAttComuns.size() == 0) && (a_vetorValores.size() == 0)) \
				throw std::invalid_argument("Faltando informacoes dos argumentos a_vetorAttComuns e a_vetorValores"); \
			else{ \
				const Id##Membro id##Membro = getId####Membro##FromChar(a_idMembro.c_str()); \
				if (!vetor##Membro.isInstanciado(id##Membro)){ \
					Membro objeto##Membro; \
					objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idMembro); \
					vetor##Membro.add(objeto##Membro);\
				} \
				vetor##Membro.att(id##Membro).addDadosAttComum(a_idMembro, a_vetorAttComuns, a_vetorValores); \
				vetor##Membro.att(id##Membro).validacaoDadosAttComum(); \
				return true; \
			} \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttComum" + std::string(#Membro) + "(" + a_membro + "," + a_idMembro + ",a_vetorAttComuns,a_vetorValores):\n" + std::string(erro.what())); } \
}; \
bool addDadosAttComum##Membro##_Membro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			if ((a_vetorAttComuns.size() == 0) && (a_vetorValores.size() == 0))\
				throw std::invalid_argument("Faltando informacoes dos argumentos a_vetorAttComuns e a_vetorValores"); \
			else{ \
				const Id##Membro id##Membro = getId####Membro##FromChar(a_idMembro.c_str()); \
				if (!vetor##Membro.isInstanciado(id##Membro)){ \
					Membro objeto##Membro; \
					objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idMembro); \
					vetor##Membro.add(objeto##Membro); \
				} \
				vetor##Membro.att(id##Membro).addDadosAttComumMembro(a_membroMembro, a_idMembroMembro, a_vetorAttComuns, a_vetorValores); \
				return true; \
			} \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttComum" + std::string(#Membro) + "_Membro("  + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro +  ",a_vetorAttComuns,a_vetorValores):\n" + std::string(erro.what())); } \
}; \
bool addDadosAttComum##Membro##_MembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			if ((a_vetorAttComuns.size() == 0) && (a_vetorValores.size() == 0)) \
				throw std::invalid_argument("Faltando informacoes dos argumentos a_vetorAttComuns e a_vetorValores"); \
			else{ \
				const Id##Membro id##Membro = getId####Membro##FromChar(a_idMembro.c_str()); \
				if (!vetor##Membro.isInstanciado(id##Membro)){ \
					Membro objeto##Membro; \
					objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idMembro); \
					vetor##Membro.add(objeto##Membro); \
				} \
				vetor##Membro.att(id##Membro).addDadosAttComumMembroMembro(a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_vetorAttComuns, a_vetorValores); \
				return true; \
			} \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttComum" + std::string(#Membro) + "_MembroMembro("  + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + ",a_vetorAttComuns,a_vetorValores):\n" + std::string(erro.what())); } \
}; \
bool addDadosAttComum##Membro##_MembroMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_membroMembroMembroMembro, const std::string a_idMembroMembroMembroMembro, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			if ((a_vetorAttComuns.size() == 0) && (a_vetorValores.size() == 0)) \
				throw std::invalid_argument("Faltando informacoes dos argumentos a_vetorAttComuns e a_vetorValores"); \
			else{ \
				const Id##Membro id##Membro = getId####Membro##FromChar(a_idMembro.c_str()); \
				if (!vetor##Membro.isInstanciado(id##Membro)){ \
					Membro objeto##Membro; \
					objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idMembro); \
					vetor##Membro.add(objeto##Membro); \
				} \
				vetor##Membro.att(id##Membro).addDadosAttComumMembroMembroMembro(a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_membroMembroMembroMembro, a_idMembroMembroMembroMembro, a_vetorAttComuns, a_vetorValores); \
				return true; \
			} \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttComum" + std::string(#Membro) + "_MembroMembroMembro("  + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_membroMembroMembroMembro + "," + a_idMembroMembroMembroMembro + ",a_vetorAttComuns,a_vetorValores):\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttComum##Membro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::vector<std::string> a_vetorAttComum){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			std::vector<std::vector<std::string>> dados; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idMembro.c_str()); \
			std::vector<Id##Membro> vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (idMembro != Id##Membro##_Nenhum) {\
				if (!vetor##Membro.isInstanciado(idMembro)) \
					throw std::invalid_argument("Objeto nao instanciado.");\
				vetorIdMembro = std::vector<Id##Membro>{idMembro}; \
			} \
			else \
				vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (vetorIdMembro.size() > 0){ \
				for (int i = 0; i < int(vetorIdMembro.size()); i++) {\
					if (i == 0) \
						dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttComum(a_incluirCabecalho, a_vetorAttComum); \
					else \
						dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttComum(false, a_vetorAttComum); \
					matrizRetorno.insert(matrizRetorno.end(), dados.begin(), dados.end()); \
				} \
				return matrizRetorno; \
			} \
			else \
				return std::vector<std::vector<std::string>>(1, std::vector<std::string>()); \
		} \
		else \
			return std::vector<std::vector<std::string>>(); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttComum" + std::string(#Membro) + "(" + a_membro + "," + a_idMembro + ",a_vetorAttComum):\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttComum##Membro##_Membro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::vector<std::string> a_vetorAttComum){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			bool primeiroDado = false; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idMembro.c_str()); \
			std::vector<Id##Membro> vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (idMembro != Id##Membro##_Nenhum) {\
				if (!vetor##Membro.isInstanciado(idMembro)) \
					throw std::invalid_argument("Objeto nao instanciado.");\
				vetorIdMembro = std::vector<Id##Membro>{idMembro}; \
			} \
			else \
				vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (vetorIdMembro.size() > 0){ \
				for (int i = 0; i < int(vetorIdMembro.size()); i++) {\
					if (primeiroDado == false) {\
						std::vector<std::vector<std::string>> dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttComumMembro(a_incluirCabecalho, a_membroMembro, a_idMembroMembro, a_vetorAttComum); \
						if (dados.at(0).size() > 0){ \
							for (int lin = 0; lin < int(dados.size()); lin++){\
								if ((lin == 0) && (a_incluirCabecalho)) \
									dados.at(lin).insert(dados.at(lin).begin(), std::string("id" + std::string(#Membro)));\
								else \
									dados.at(lin).insert(dados.at(lin).begin(), getString(getAtributo(vetorIdMembro.at(i), AttComum##Membro##_id##Membro, Id##Membro())));\
							} \
							matrizRetorno = dados;\
							primeiroDado = true; \
						} \
					} \
					else {\
						std::vector<std::vector<std::string>> dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttComumMembro(false, a_membroMembro, a_idMembroMembro, a_vetorAttComum); \
						if (dados.at(0).size() > 0){ \
							for (int lin = 0; lin < int(dados.size()); lin++)\
								dados.at(lin).insert(dados.at(lin).begin(), getString(getAtributo(vetorIdMembro.at(i), AttComum##Membro##_id##Membro, Id##Membro())));\
							matrizRetorno.insert(matrizRetorno.end(), dados.begin(), dados.end()); \
						} \
					} \
				} \
				return matrizRetorno; \
			} \
			else \
				return std::vector<std::vector<std::string>>(1, std::vector<std::string>()); \
		} \
		else \
			return std::vector<std::vector<std::string>>(); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttComum" + std::string(#Membro) + "_Membro("+ getString(a_incluirCabecalho) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + ",a_vetorAttComum):\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttComum##Membro##_MembroMembro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::vector<std::string> a_vetorAttComum){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			bool primeiroDado = false; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idMembro.c_str()); \
			std::vector<Id##Membro> vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (idMembro != Id##Membro##_Nenhum) {\
				if (!vetor##Membro.isInstanciado(idMembro)) \
					throw std::invalid_argument("Objeto nao instanciado.");\
				vetorIdMembro = std::vector<Id##Membro>{idMembro}; \
			} \
			else \
				vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (vetorIdMembro.size() > 0){ \
				for (int i = 0; i < int(vetorIdMembro.size()); i++) {\
					if (primeiroDado == false) {\
						std::vector<std::vector<std::string>> dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttComumMembroMembro(a_incluirCabecalho, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_vetorAttComum); \
						if (dados.at(0).size() > 0){ \
							for (int lin = 0; lin < int(dados.size()); lin++){\
								if ((lin == 0) && (a_incluirCabecalho)) \
									dados.at(lin).insert(dados.at(lin).begin(), std::string("id" + std::string(#Membro)));\
								else \
									dados.at(lin).insert(dados.at(lin).begin(), getString(getAtributo(vetorIdMembro.at(i), AttComum##Membro##_id##Membro, Id##Membro())));\
							} \
							matrizRetorno = dados;\
							primeiroDado = true; \
						} \
					} \
					else {\
						std::vector<std::vector<std::string>> dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttComumMembroMembro(false, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_vetorAttComum); \
						if (dados.at(0).size() > 0){ \
							for (int lin = 0; lin < int(dados.size()); lin++)\
								dados.at(lin).insert(dados.at(lin).begin(), getString(getAtributo(vetorIdMembro.at(i), AttComum##Membro##_id##Membro, Id##Membro())));\
							matrizRetorno.insert(matrizRetorno.end(), dados.begin(), dados.end()); \
						} \
					} \
				} \
				return matrizRetorno; \
			} \
			else \
				return std::vector<std::vector<std::string>>(1, std::vector<std::string>()); \
		} \
		else \
			return std::vector<std::vector<std::string>>(); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttComum" + std::string(#Membro) + "_MembroMembro("+ getString(a_incluirCabecalho) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + ",a_vetorAttComum):\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttComum##Membro##_MembroMembroMembro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_membroMembroMembroMembro, const std::string a_idMembroMembroMembroMembro, const std::vector<std::string> a_vetorAttComum){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			bool primeiroDado = false; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idMembro.c_str()); \
			std::vector<Id##Membro> vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (idMembro != Id##Membro##_Nenhum) {\
				if (!vetor##Membro.isInstanciado(idMembro)) \
					throw std::invalid_argument("Objeto nao instanciado.");\
				vetorIdMembro = std::vector<Id##Membro>{idMembro}; \
			} \
			else \
				vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (vetorIdMembro.size() > 0){ \
				for (int i = 0; i < int(vetorIdMembro.size()); i++) {\
					if (primeiroDado == false) {\
						std::vector<std::vector<std::string>> dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttComumMembroMembroMembro(a_incluirCabecalho, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_membroMembroMembroMembro, a_idMembroMembroMembroMembro, a_vetorAttComum); \
						if (dados.at(0).size() > 0){ \
							for (int lin = 0; lin < int(dados.size()); lin++){\
								if ((lin == 0) && (a_incluirCabecalho)) \
									dados.at(lin).insert(dados.at(lin).begin(), std::string("id" + std::string(#Membro)));\
								else \
									dados.at(lin).insert(dados.at(lin).begin(), getString(getAtributo(vetorIdMembro.at(i), AttComum##Membro##_id##Membro, Id##Membro())));\
							} \
							matrizRetorno = dados;\
							primeiroDado = true; \
						} \
					} \
					else {\
						std::vector<std::vector<std::string>> dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttComumMembroMembroMembro(false, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_membroMembroMembroMembro, a_idMembroMembroMembroMembro, a_vetorAttComum); \
						if (dados.at(0).size() > 0){ \
							for (int lin = 0; lin < int(dados.size()); lin++)\
								dados.at(lin).insert(dados.at(lin).begin(), getString(getAtributo(vetorIdMembro.at(i), AttComum##Membro##_id##Membro, Id##Membro())));\
							matrizRetorno.insert(matrizRetorno.end(), dados.begin(), dados.end()); \
						} \
					} \
				} \
				return matrizRetorno; \
			} \
			else \
				return std::vector<std::vector<std::string>>(1, std::vector<std::string>()); \
		} \
		else \
			return std::vector<std::vector<std::string>>(); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttComum" + std::string(#Membro) + "_MembroMembroMembro("+ getString(a_incluirCabecalho) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_membroMembroMembroMembro + "," + a_idMembroMembroMembroMembro + ",a_vetorAttComum):\n" + std::string(erro.what())); } \
}; \
template<typename Atributo, typename Valor> \
Valor getAtributo(const Id##Membro a_membro, const Atributo a_atributo, const Valor a_valor){ \
	try { return vetor##Membro.att(a_membro).getAtributo(a_atributo, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getAtributo(" + getString(a_membro) + "," + getString(a_atributo) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdMembroMembro, typename Atributo, typename Valor> \
Valor getAtributo(const Id##Membro a_membro, const IdMembroMembro a_membroMembro, const Atributo a_atributo, const Valor a_valor){ \
	try { return vetor##Membro.att(a_membro).getAtributo(a_membroMembro, a_atributo, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getAtributo(" + getString(a_membro) + "," + getString(a_membroMembro) + "," + getString(a_atributo) +  "): \n" + std::string(erro.what())); } \
}; \
template<typename IdMembroMembro, typename IdMembroMembroMembro, typename Atributo, typename Valor> \
Valor getAtributo(const Id##Membro a_membro, const IdMembroMembro a_membroMembro, const IdMembroMembroMembro a_membroMembroMembro, const Atributo a_atributo, const Valor a_valor) { \
	try { return vetor##Membro.att(a_membro).getAtributo(a_membroMembro, a_membroMembroMembro, a_atributo, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getAtributo(" + getString(a_membro) + "," + getString(a_membroMembro) + "," + getString(a_membroMembroMembro) + "," + getString(a_atributo) + "): \n" + std::string(erro.what())); } \
}; \
template<typename IdMembroMembro, typename IdMembroMembroMembro, typename IdMembroMembroMembroMembro, typename Atributo, typename Valor> \
Valor getAtributo(const Id##Membro a_membro, const IdMembroMembro a_membroMembro, const IdMembroMembroMembro a_membroMembroMembro, const IdMembroMembroMembroMembro a_membroMembroMembroMembro, const Atributo a_atributo, const Valor a_valor) { \
	try { return vetor##Membro.att(a_membro).getAtributo(a_membroMembro, a_membroMembroMembro, a_membroMembroMembroMembro, a_atributo, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getAtributo(" + getString(a_membro) + "," + getString(a_membroMembro) + "," + getString(a_membroMembroMembro) + "," + getString(a_membroMembroMembroMembro) + "," + getString(a_atributo) + "): \n" + std::string(erro.what())); } \
}; \
bool addDadoAttVetor##Membro(const std::string a_membro, const std::string a_idMembro, const std::string a_iterador, const std::string a_attVetor, const std::string a_valor, const int a_alocacao){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			if ((a_iterador == "") || (a_valor == "")) \
				throw std::invalid_argument("Faltando informacoes dos argumentos a_iterador e a_valor"); \
			else{ \
				const Id##Membro id##Membro = getId####Membro##FromChar(a_idMembro.c_str()); \
				if (!vetor##Membro.isInstanciado(id##Membro)){ \
					Membro objeto##Membro; \
					objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idMembro); \
					vetor##Membro.add(objeto##Membro);\
				} \
				vetor##Membro.att(id##Membro).addDadoAttVetor(a_idMembro, a_iterador, a_attVetor, a_valor, a_alocacao); \
				vetor##Membro.att(id##Membro).validacaoDadosAttVetor(std::vector<std::string>{a_attVetor}); \
				return true; \
			} \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttVetor" + std::string(#Membro) + "(" + a_membro + "," + a_idMembro + "," + a_iterador + "," + a_attVetor + "," + a_valor + "," + getFullString(a_alocacao) + "):\n" + std::string(erro.what())); } \
}; \
bool addDadoAttVetor##Membro##_Membro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_iterador, const std::string a_attVetor, const std::string a_valor, const int a_alocacao){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			if ((a_iterador == "") && (a_valor == "")) \
				throw std::invalid_argument("Faltando informacoes dos argumentos a_iterador e a_valor"); \
			else{ \
				const Id##Membro id##Membro = getId####Membro##FromChar(a_idMembro.c_str()); \
				if (!vetor##Membro.isInstanciado(id##Membro)){ \
					Membro objeto##Membro; \
					objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idMembro); \
					vetor##Membro.add(objeto##Membro);\
				} \
				vetor##Membro.att(id##Membro).addDadoAttVetorMembro(a_membroMembro, a_idMembroMembro, a_iterador, a_attVetor, a_valor, a_alocacao); \
				return true; \
			} \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadoAttVetor" + std::string(#Membro) + "_Membro("  + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_iterador + "," + a_attVetor + "," + a_valor + "," + getFullString(a_alocacao) + "):\n" + std::string(erro.what())); } \
}; \
bool addDadoAttVetor##Membro##_MembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_iterador, const std::string a_attVetor, const std::string a_valor, const int a_alocacao){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			if ((a_iterador == "") && (a_valor == "")) \
				throw std::invalid_argument("Faltando informacoes dos argumentos a_iterador e a_valor"); \
			else{ \
				const Id##Membro id##Membro = getId####Membro##FromChar(a_idMembro.c_str()); \
				if (!vetor##Membro.isInstanciado(id##Membro)){ \
					Membro objeto##Membro; \
					objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idMembro); \
					vetor##Membro.add(objeto##Membro);\
				} \
				vetor##Membro.att(id##Membro).addDadoAttVetorMembroMembro(a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_iterador, a_attVetor, a_valor, a_alocacao); \
				return true; \
			} \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadoAttVetor" + std::string(#Membro) + "_MembroMembro("  + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_iterador + "," + a_attVetor + "," + a_valor + "," + getFullString(a_alocacao) + "):\n" + std::string(erro.what())); } \
}; \
bool addDadoAttVetor##Membro##_MembroMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_membroMembroMembroMembro, const std::string a_idMembroMembroMembroMembro, const std::string a_iterador, const std::string a_attVetor, const std::string a_valor, const int a_alocacao){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			if ((a_iterador == "") && (a_valor == "")) \
				throw std::invalid_argument("Faltando informacoes dos argumentos a_iterador e a_valor"); \
			else{ \
				const Id##Membro id##Membro = getId####Membro##FromChar(a_idMembro.c_str()); \
				if (!vetor##Membro.isInstanciado(id##Membro)){ \
					Membro objeto##Membro; \
					objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idMembro); \
					vetor##Membro.add(objeto##Membro);\
				} \
				vetor##Membro.att(id##Membro).addDadoAttVetorMembroMembroMembro(a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_membroMembroMembroMembro, a_idMembroMembroMembroMembro, a_iterador, a_attVetor, a_valor, a_alocacao); \
				return true; \
			} \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadoAttVetor" + std::string(#Membro) + "_MembroMembroMembro("  + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_membroMembroMembroMembro + "," + a_idMembroMembroMembroMembro + "," + a_iterador + "," + a_attVetor + "," + a_valor + "," + getFullString(a_alocacao) + "):\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttVetor##Membro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			std::vector<std::vector<std::string>> dados; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idMembro.c_str()); \
			std::vector<Id##Membro> vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (idMembro != Id##Membro##_Nenhum) {\
				if (!vetor##Membro.isInstanciado(idMembro)) \
					throw std::invalid_argument("Objeto nao instanciado.");\
				vetorIdMembro = std::vector<Id##Membro>{idMembro}; \
			} \
			else \
				vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (vetorIdMembro.size() > 0){ \
				for (int i = 0; i < int(vetorIdMembro.size()); i++) {\
					const std::string idMembroStr = getString(getAtributo(vetorIdMembro.at(i), AttComum##Membro##_id##Membro, Id##Membro())); \
					if (i == 0){ \
						dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttVetor(a_incluirCabecalho, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
						if (a_incluirCabecalho) \
							dados.at(0).insert(dados.at(0).begin(), std::string("id" + std::string(#Membro))); \
					} \
					else\
						dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttVetor(false, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
					for (int k = 0; k < int(dados.size()); k++){ \
						if ((k == 0) && (i == 0) && (a_incluirCabecalho)){} \
						else \
							dados.at(k).insert(dados.at(k).begin(), idMembroStr); \
					} \
					matrizRetorno.insert(matrizRetorno.end(), dados.begin(), dados.end()); \
				} \
				return matrizRetorno; \
			} \
			else \
				return std::vector<std::vector<std::string>>(1, std::vector<std::string>()); \
		} \
		else \
			return std::vector<std::vector<std::string>>(); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttVetor" + std::string(#Membro) + "(" + getString(a_incluirCabecalho) + "," + a_membro + "," + a_idMembro + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_vetorAttVetor):\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttVetor##Membro##_Membro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			bool primeiroDado = false; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idMembro.c_str()); \
			std::vector<Id##Membro> vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (idMembro != Id##Membro##_Nenhum) {\
				if (!vetor##Membro.isInstanciado(idMembro)) \
					throw std::invalid_argument("Objeto nao instanciado.");\
				vetorIdMembro = std::vector<Id##Membro>{idMembro}; \
			} \
			else \
				vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (vetorIdMembro.size() > 0){ \
				for (int i = 0; i < int(vetorIdMembro.size()); i++) {\
					if (primeiroDado == false) {\
						std::vector<std::vector<std::string>> dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttVetorMembro(a_incluirCabecalho, a_membroMembro, a_idMembroMembro, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
						if (dados.at(0).size() > 0){ \
							for (int lin = 0; lin < int(dados.size()); lin++){\
								if ((lin == 0) && (a_incluirCabecalho)) \
									dados.at(0).insert(dados.at(0).begin(), std::string("id" + std::string(#Membro)));\
								else \
									dados.at(lin).insert(dados.at(lin).begin(), getString(getAtributo(vetorIdMembro.at(i), AttComum##Membro##_id##Membro, Id##Membro())));\
							} \
							matrizRetorno = dados;\
							primeiroDado = true; \
						} \
					} \
					else {\
						std::vector<std::vector<std::string>> dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttVetorMembro(false, a_membroMembro, a_idMembroMembro, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
						if (dados.at(0).size() > 0){ \
							for (int lin = 0; lin < int(dados.size()); lin++)\
								dados.at(lin).insert(dados.at(lin).begin(), getString(getAtributo(vetorIdMembro.at(i), AttComum##Membro##_id##Membro, Id##Membro())));\
							matrizRetorno.insert(matrizRetorno.end(), dados.begin(), dados.end()); \
						} \
					} \
				} \
				return matrizRetorno; \
			} \
			else \
				return std::vector<std::vector<std::string>>(1, std::vector<std::string>()); \
		} \
		else \
			return std::vector<std::vector<std::string>>(); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttVetor" + std::string(#Membro) + "_Membro("+ getString(a_incluirCabecalho) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_vetorAttVetor):\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttVetor##Membro##_MembroMembro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			bool primeiroDado = false; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idMembro.c_str()); \
			std::vector<Id##Membro> vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (idMembro != Id##Membro##_Nenhum) {\
				if (!vetor##Membro.isInstanciado(idMembro)) \
					throw std::invalid_argument("Objeto nao instanciado.");\
				vetorIdMembro = std::vector<Id##Membro>{idMembro}; \
			} \
			else \
				vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (vetorIdMembro.size() > 0){ \
				for (int i = 0; i < int(vetorIdMembro.size()); i++) {\
					if (primeiroDado == false) {\
						std::vector<std::vector<std::string>> dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttVetorMembroMembro(a_incluirCabecalho, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
						if (dados.at(0).size() > 0){ \
							for (int lin = 0; lin < int(dados.size()); lin++){\
								if ((lin == 0) && (a_incluirCabecalho)) \
									dados.at(0).insert(dados.at(0).begin(), std::string("id" + std::string(#Membro)));\
								else \
									dados.at(lin).insert(dados.at(lin).begin(), getString(getAtributo(vetorIdMembro.at(i), AttComum##Membro##_id##Membro, Id##Membro())));\
							} \
							matrizRetorno = dados;\
							primeiroDado = true; \
						} \
					} \
					else {\
						std::vector<std::vector<std::string>> dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttVetorMembroMembro(false, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
						if (dados.at(0).size() > 0){ \
							for (int lin = 0; lin < int(dados.size()); lin++)\
								dados.at(lin).insert(dados.at(lin).begin(), getString(getAtributo(vetorIdMembro.at(i), AttComum##Membro##_id##Membro, Id##Membro())));\
							matrizRetorno.insert(matrizRetorno.end(), dados.begin(), dados.end()); \
						} \
					} \
				} \
				return matrizRetorno; \
			} \
			else \
				return std::vector<std::vector<std::string>>(1, std::vector<std::string>()); \
		} \
		else \
			return std::vector<std::vector<std::string>>(); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttVetor" + std::string(#Membro) + "_MembroMembro("+ getString(a_incluirCabecalho) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_vetorAttVetor):\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttVetor##Membro##_MembroMembroMembro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_membroMembroMembroMembro, const std::string a_idMembroMembroMembroMembro, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			bool primeiroDado = false; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idMembro.c_str()); \
			std::vector<Id##Membro> vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (idMembro != Id##Membro##_Nenhum) {\
				if (!vetor##Membro.isInstanciado(idMembro)) \
					throw std::invalid_argument("Objeto nao instanciado.");\
				vetorIdMembro = std::vector<Id##Membro>{idMembro}; \
			} \
			else \
				vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (vetorIdMembro.size() > 0){ \
				for (int i = 0; i < int(vetorIdMembro.size()); i++) {\
					if (primeiroDado == false) {\
						std::vector<std::vector<std::string>> dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttVetorMembroMembroMembro(a_incluirCabecalho, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_membroMembroMembroMembro, a_idMembroMembroMembroMembro, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
						if (dados.at(0).size() > 0){ \
							for (int lin = 0; lin < int(dados.size()); lin++){\
								if ((lin == 0) && (a_incluirCabecalho)) \
									dados.at(0).insert(dados.at(0).begin(), std::string("id" + std::string(#Membro)));\
								else \
									dados.at(lin).insert(dados.at(lin).begin(), getString(getAtributo(vetorIdMembro.at(i), AttComum##Membro##_id##Membro, Id##Membro())));\
							} \
							matrizRetorno = dados;\
							primeiroDado = true; \
						} \
					} \
					else {\
						std::vector<std::vector<std::string>> dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttVetorMembroMembroMembro(false, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_membroMembroMembroMembro, a_idMembroMembroMembroMembro, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
						if (dados.at(0).size() > 0){ \
							for (int lin = 0; lin < int(dados.size()); lin++)\
								dados.at(lin).insert(dados.at(lin).begin(), getString(getAtributo(vetorIdMembro.at(i), AttComum##Membro##_id##Membro, Id##Membro())));\
							matrizRetorno.insert(matrizRetorno.end(), dados.begin(), dados.end()); \
						} \
					} \
				} \
				return matrizRetorno; \
			} \
			else \
				return std::vector<std::vector<std::string>>(1, std::vector<std::string>()); \
		} \
		else \
			return std::vector<std::vector<std::string>>(); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttVetor" + std::string(#Membro) + "_MembroMembroMembro("+ getString(a_incluirCabecalho) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_membroMembroMembroMembro + "," + a_idMembroMembroMembroMembro + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_vetorAttVetor):\n" + std::string(erro.what())); } \
}; \
Id##Membro getMaiorId(const Id##Membro a_membro)const{ \
	try { return vetor##Membro.getMaiorId(); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getMaiorId(" + getString(a_membro) + "): \n" + std::string(erro.what())); } \
};\
std::vector<Id##Membro> getIdObjetos(const Id##Membro a_membro)const{ \
	try { return vetor##Membro.getIdObjetos(); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIdObjetos(" + getString(a_membro) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Valor> \
std::vector<Id##Membro> getIdObjetos(const Id##Membro a_membro, const Atributo a_atributo, const Valor a_valor)const{ \
	try { return vetor##Membro.getIdObjetos(a_atributo, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIdObjetos(" + getString(a_membro) + "," + getFullString(a_atributo) + "," + getFullString(a_valor) + "): \n" + std::string(erro.what())); } \
};\
template<typename AttVetor, typename TipoIterador, typename TipoValor> \
SmartEnupla<TipoIterador,TipoValor> getVetor(const Id##Membro a_idMembro, const AttVetor a_attVetor, const TipoIterador a_iterador, const TipoValor a_tipoValor){ \
	try { return vetor##Membro.att(a_idMembro).getVetor(a_attVetor, a_iterador, a_tipoValor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getVetor(" + getString(a_idMembro) + getString(a_attVetor) + ",a_iterador,a_tipoValor): \n" + std::string(erro.what())); } \
};\
int numObjetos(const Membro a_membro){ \
	try { return vetor##Membro.numObjetos(); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::numObjetos(" + getString(a_membro) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo> \
int getSizeVetor(const Id##Membro a_membro, const Atributo a_atributo){ \
	try { return vetor##Membro.att(a_membro).getSizeVetor(a_atributo); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeVetor(" + getString(a_membro) + "," + getString(a_atributo) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador> \
Iterador getIteradorInicial(const Id##Membro a_membro, const Atributo a_atributo, const Iterador a_iterador){ \
	try { return vetor##Membro.att(a_membro).getIteradorInicial(a_atributo, a_iterador); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIteradorInicial(" + getString(a_membro) + "," + getString(a_atributo) + "," + getString(a_iterador) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador> \
Iterador getIteradorFinal(const Id##Membro a_membro, const Atributo a_atributo, const Iterador a_iterador){ \
	try { return vetor##Membro.att(a_membro).getIteradorFinal(a_atributo, a_iterador); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIteradorFinal(" + getString(a_membro) + "," + getString(a_atributo) + "," + getString(a_iterador) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador, typename Valor> \
Valor getElementoVetor(const Id##Membro a_membro, const Atributo a_atributo, const Iterador a_iterador, const Valor a_valor){ \
	try { return vetor##Membro.att(a_membro).getElementoVetor(a_atributo, a_iterador, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getElementoVetor(" + getString(a_membro) + "," + getString(a_atributo) + "," + getString(a_iterador) + ",Valor): \n" + std::string(erro.what())); } \
};\
template<typename IdMembroMembro> \
IdMembroMembro getMaiorId(const Id##Membro a_membro, const IdMembroMembro a_membroMembro){ \
	try { return vetor##Membro.att(a_membro).getMaiorId(a_membroMembro); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getMaiorId(" + getString(a_membro) + "," + getString(a_membroMembro) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdMembroMembro> \
std::vector<IdMembroMembro> getIdObjetos(const Id##Membro a_membro, const IdMembroMembro a_membroMembro){ \
	try { return vetor##Membro.att(a_membro).getIdObjetos(a_membroMembro); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIdObjetos(" + getString(a_membro) + "," + getString(a_membroMembro) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdMembroMembro, typename Atributo, typename Valor> \
std::vector<IdMembroMembro> getIdObjetos(const Id##Membro a_membro, const IdMembroMembro a_membroMembro, const Atributo a_atributo, const Valor a_valor){ \
	try { return vetor##Membro.att(a_membro).getIdObjetos(a_membroMembro, a_atributo, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIdObjetos(" + getString(a_membro) + "," + getString(a_membroMembro) + "," + getFullString(a_atributo) + "," + getFullString(a_valor) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdMembroMembro, typename AttVetor, typename TipoIterador, typename TipoValor> \
SmartEnupla<TipoIterador,TipoValor> getVetor(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const AttVetor a_attVetor, const TipoIterador a_iterador, const TipoValor a_tipoValor){ \
	try { return vetor##Membro.att(a_idMembro).getVetor(a_idMembroMembro, a_attVetor, a_iterador, a_tipoValor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getVetor(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_attVetor) + ",a_iterador,a_tipoValor): \n" + std::string(erro.what())); } \
};\
template<typename MembroMembro> \
int numObjetos(const Id##Membro a_idMembro, const MembroMembro a_membroMembro){ \
	try { return vetor##Membro.att(a_idMembro).numObjetos(a_membroMembro); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::numObjetos(" + getString(a_idMembro) + "," + getString(a_membroMembro) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdMembroMembro, typename Atributo> \
int getSizeVetor(const Id##Membro a_membro, const IdMembroMembro a_membroMembro, const Atributo a_atributo){ \
	try { return vetor##Membro.att(a_membro).getSizeVetor(a_membroMembro, a_atributo); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeVetor(" + getString(a_membro) + "," + getString(a_membroMembro) + "," + getString(a_atributo) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdMembroMembro, typename Atributo, typename Iterador> \
Iterador getIteradorInicial(const Id##Membro a_membro, const IdMembroMembro a_membroMembro, const Atributo a_atributo, const Iterador a_iterador){ \
	try { return vetor##Membro.att(a_membro).getIteradorInicial(a_membroMembro, a_atributo, a_iterador); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIteradorInicial(" + getString(a_membro) + "," + getString(a_membroMembro) + "," + getString(a_atributo) + "," + getString(a_iterador) + "): \n" + std::string(erro.what())); } \
}; \
template<typename IdMembroMembro, typename Atributo, typename Iterador> \
Iterador getIteradorFinal(const Id##Membro a_membro, const IdMembroMembro a_membroMembro, const Atributo a_atributo, const Iterador a_iterador){ \
	try { return vetor##Membro.att(a_membro).getIteradorFinal(a_membroMembro, a_atributo, a_iterador); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIteradorFinal(" + getString(a_membro) + "," + getString(a_membroMembro) + "," + getString(a_atributo) + "," + getString(a_iterador) + "): \n" + std::string(erro.what())); } \
}; \
template<typename IdMembroMembro, typename Atributo, typename Iterador, typename Valor> \
Valor getElementoVetor(const Id##Membro a_membro, const IdMembroMembro a_membroMembro, const Atributo a_atributo, const Iterador a_iterador, const Valor a_valor){ \
	try { return vetor##Membro.att(a_membro).getElementoVetor(a_membroMembro, a_atributo, a_iterador, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getElementoVetor(" + getString(a_membro) + "," + getString(a_membroMembro) + "," + getString(a_atributo) + "," + getString(a_iterador) + ",Valor): \n" + std::string(erro.what())); } \
}; \
template<typename IdMembroMembro, typename IdMembroMembroMembro> \
IdMembroMembroMembro getMaiorId(const Id##Membro a_membro, const IdMembroMembro a_membroMembro, const IdMembroMembroMembro a_membroMembroMembro){ \
	try { return vetor##Membro.att(a_membro).getMaiorId(a_membroMembro, a_membroMembroMembro); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getMaiorId(" + getString(a_membro) + "," + getString(a_membroMembro) + "," + getString(a_membroMembroMembro) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdMembroMembro, typename IdMembroMembroMembro, typename AttVetor, typename TipoIterador, typename TipoValor> \
SmartEnupla<TipoIterador,TipoValor> getVetor(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const IdMembroMembroMembro a_idMembroMembroMembro, const AttVetor a_attVetor, const TipoIterador a_iterador, const TipoValor a_tipoValor){ \
	try { return vetor##Membro.att(a_idMembro).getVetor(a_idMembroMembro, a_idMembroMembroMembro, a_attVetor, a_iterador, a_tipoValor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getVetor(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_idMembroMembroMembro) + "," + getString(a_attVetor) + ",a_iterador,a_tipoValor): \n" + std::string(erro.what())); } \
};\
template<typename IdMembroMembro, typename MembroMembroMembro> \
int numObjetos(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const MembroMembroMembro a_membroMembroMembro){ \
	try { return vetor##Membro.att(a_idMembro).numObjetos(a_idMembroMembro, a_membroMembroMembro); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::numObjetos(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_membroMembroMembro) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdMembroMembro, typename IdMembroMembroMembro, typename Atributo> \
int getSizeVetor(const Id##Membro a_membro, const IdMembroMembro a_membroMembro, const IdMembroMembroMembro a_membroMembroMembro, const Atributo a_atributo){ \
	try { return vetor##Membro.att(a_membro).getSizeVetor(a_membroMembro, a_membroMembroMembro, a_atributo); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeVetor(" + getString(a_membro) + "," + getString(a_membroMembro) + "," + getString(a_membroMembroMembro) + "," + getString(a_atributo) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdMembroMembro, typename IdMembroMembroMembro, typename Atributo, typename Iterador> \
Iterador getIteradorInicial(const Id##Membro a_membro, const IdMembroMembro a_membroMembro, const IdMembroMembroMembro a_membroMembroMembro, const Atributo a_atributo, const Iterador a_iterador) { \
	try { return vetor##Membro.att(a_membro).getIteradorInicial(a_membroMembro, a_membroMembroMembro, a_atributo, a_iterador); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIteradorInicial(" + getString(a_membro) + "," + getString(a_membroMembro) + "," + getString(a_membroMembroMembro) + "," + getString(a_atributo) + "," + getString(a_iterador) + "): \n" + std::string(erro.what())); } \
}; \
template<typename IdMembroMembro, typename IdMembroMembroMembro, typename Atributo, typename Iterador> \
Iterador getIteradorFinal(const Id##Membro a_membro, const IdMembroMembro a_membroMembro, const IdMembroMembroMembro a_membroMembroMembro, const Atributo a_atributo, const Iterador a_iterador) { \
	try { return vetor##Membro.att(a_membro).getIteradorFinal(a_membroMembro, a_membroMembroMembro, a_atributo, a_iterador); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIteradorFinal(" + getString(a_membro) + "," + getString(a_membroMembro) + "," + getString(a_membroMembroMembro) + "," + getString(a_atributo) + "," + getString(a_iterador) + "): \n" + std::string(erro.what())); } \
}; \
template<typename IdMembroMembro, typename IdMembroMembroMembro, typename Atributo, typename Iterador, typename Valor> \
Valor getElementoVetor(const Id##Membro a_membro, const IdMembroMembro a_membroMembro, const IdMembroMembroMembro a_membroMembroMembro, const Atributo a_atributo, const Iterador a_iterador, const Valor a_valor) { \
	try { return vetor##Membro.att(a_membro).getElementoVetor(a_membroMembro, a_membroMembroMembro, a_atributo, a_iterador, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getElementoVetor(" + getString(a_membro) + "," + getString(a_membroMembro) + "," + getString(a_membroMembroMembro) + "," + getString(a_atributo) + "," + getString(a_iterador) + ",Valor): \n" + std::string(erro.what())); } \
}; \
template<typename Id##MembroMembro, typename IdMembroMembroMembro, typename IdMembroMembroMembroMembro> \
IdMembroMembroMembroMembro getMaiorId(const Id##Membro a_membro, const Id##MembroMembro a_membroMembro, const IdMembroMembroMembro a_membroMembroMembro, const IdMembroMembroMembroMembro a_membroMembroMembroMembro){ \
	try { return vetor##Membro.att(a_membro).getMaiorId(a_membroMembro, a_membroMembroMembro, a_membroMembroMembroMembro); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getMaiorId(" + getString(a_membro) + "," + getString(a_membroMembro) + "," + getString(a_membroMembroMembro) + "," + getString(a_membroMembroMembroMembro) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdMembroMembro, typename IdMembroMembroMembro, typename IdMembroMembroMembroMembro, typename AttVetor, typename TipoIterador, typename TipoValor> \
SmartEnupla<TipoIterador,TipoValor> getVetor(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const IdMembroMembroMembro a_idMembroMembroMembro, const IdMembroMembroMembroMembro a_idMembroMembroMembroMembro, const AttVetor a_attVetor, const TipoIterador a_iterador, const TipoValor a_tipoValor){ \
	try { return vetor##Membro.att(a_idMembro).getVetor(a_idMembroMembro, a_idMembroMembroMembro, a_idMembroMembroMembroMembro, a_attVetor, a_iterador, a_tipoValor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getVetor(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_idMembroMembroMembro) + "," + getString(a_idMembroMembroMembroMembro) + "," + getString(a_attVetor) + ",a_iterador,a_tipoValor): \n" + std::string(erro.what())); } \
};\
template<typename IdMembroMembro, typename IdMembroMembroMembro, typename MembroMembroMembroMembro> \
int numObjetos(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const IdMembroMembroMembro a_idMembroMembroMembro, const MembroMembroMembroMembro a_membroMembroMembroMembro){ \
	try { return vetor##Membro.att(a_idMembro).numObjetos(a_idMembroMembro, a_idMembroMembroMembro, a_membroMembroMembroMembro); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::numObjetos(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_idMembroMembroMembro) + "," + getString(a_membroMembroMembroMembro) + "): \n" + std::string(erro.what())); } \
};\
template<typename Id##MembroMembro, typename IdMembroMembroMembro, typename IdMembroMembroMembroMembro, typename Atributo> \
int getSizeVetor(const Id##Membro a_membro, const IdMembroMembro a_membroMembro, const IdMembroMembroMembro a_membroMembroMembro, const IdMembroMembroMembroMembro a_membroMembroMembroMembro, const Atributo a_atributo){ \
	try { return vetor##Membro.att(a_membro).getSizeVetor(a_membroMembro, a_membroMembroMembro, a_membroMembroMembroMembro, a_atributo); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeVetor(" + getString(a_membro) + "," + getString(a_membroMembro) + "," + getString(a_membroMembroMembro) + "," + getString(a_membroMembroMembroMembro) + "," + getString(a_atributo) + "): \n" + std::string(erro.what())); } \
};\
template<typename Id##MembroMembro, typename IdMembroMembroMembro, typename IdMembroMembroMembroMembro, typename Atributo, typename Iterador> \
Iterador getIteradorInicial(const Id##Membro a_membro, const IdMembroMembro a_membroMembro, const IdMembroMembroMembro a_membroMembroMembro, const IdMembroMembroMembroMembro a_membroMembroMembroMembro, const Atributo a_atributo, const Iterador a_iterador) { \
	try { return vetor##Membro.att(a_membro).getIteradorInicial(a_membroMembro, a_membroMembroMembro, a_membroMembroMembroMembro, a_atributo, a_iterador); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIteradorInicial(" + getString(a_membro) + "," + getString(a_membroMembro) + "," + getString(a_membroMembroMembro) + "," + getString(a_membroMembroMembroMembro) + "," + getString(a_atributo) + "," + getString(a_iterador) + "): \n" + std::string(erro.what())); } \
}; \
template<typename Id##MembroMembro, typename IdMembroMembroMembro, typename IdMembroMembroMembroMembro, typename Atributo, typename Iterador> \
Iterador getIteradorFinal(const Id##Membro a_membro, const IdMembroMembro a_membroMembro, const IdMembroMembroMembro a_membroMembroMembro, const IdMembroMembroMembroMembro a_membroMembroMembroMembro, const Atributo a_atributo, const Iterador a_iterador) { \
	try { return vetor##Membro.att(a_membro).getIteradorFinal(a_membroMembro, a_membroMembroMembro, a_membroMembroMembroMembro, a_atributo, a_iterador); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIteradorFinal(" + getString(a_membro) + "," + getString(a_membroMembro) + "," + getString(a_membroMembroMembro) + "," + getString(a_membroMembroMembroMembro) + "," + getString(a_atributo) + "," + getString(a_iterador) + "): \n" + std::string(erro.what())); } \
}; \
template<typename Id##MembroMembro, typename IdMembroMembroMembro, typename IdMembroMembroMembroMembro, typename Atributo, typename Iterador, typename Valor> \
Valor getElementoVetor(const Id##Membro a_membro, const IdMembroMembro a_membroMembro, const IdMembroMembroMembro a_membroMembroMembro, const IdMembroMembroMembroMembro a_membroMembroMembroMembro, const Atributo a_atributo, const Iterador a_iterador, const Valor a_valor) { \
	try { return vetor##Membro.att(a_membro).getElementoVetor(a_membroMembro, a_membroMembroMembro, a_membroMembroMembroMembro, a_atributo, a_iterador, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getElementoVetor(" + getString(a_membro) + "," + getString(a_membroMembro) + "," + getString(a_membroMembroMembro) + "," + getString(a_membroMembroMembroMembro) + "," + getString(a_atributo) + "," + getString(a_iterador) + ",Valor): \n" + std::string(erro.what())); } \
}; \
bool addDadoAttMatriz##Membro(const std::string a_membro, const std::string a_idMembro, const std::string a_iterador1, const std::string a_iterador2, const std::string a_attMatriz, const std::string a_valor, const int a_alocacao_1, const int a_alocacao_2){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			if (((a_iterador1 != "") && (a_iterador2 != "") && (a_valor != "")) || \
                ((a_iterador1 != "") && (a_iterador2 == "") && (a_valor == ""))){ \
				const Id##Membro id##Membro = getId####Membro##FromChar(a_idMembro.c_str()); \
				if (!vetor##Membro.isInstanciado(id##Membro)){ \
					Membro objeto##Membro; \
					objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idMembro); \
					vetor##Membro.add(objeto##Membro);\
				} \
				vetor##Membro.att(id##Membro).addDadoAttMatriz(a_iterador1, a_iterador2, a_attMatriz, a_valor, a_alocacao_1, a_alocacao_2); \
				vetor##Membro.att(id##Membro).validacaoDadosAttMatriz(std::vector<std::string>{a_attMatriz}); \
				return true; \
			} \
			else \
				throw std::invalid_argument("Faltando informacoes dos argumentos a_iterador1, a_iterador2 e a_valor"); \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttMatriz" + std::string(#Membro) + "(" + a_membro + "," + a_idMembro + "," + a_iterador1 + "," + a_iterador2 + "," + a_attMatriz + "," + a_valor + "):\n" + std::string(erro.what())); } \
}; \
bool addDadoAttMatriz##Membro##_Membro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_iterador1, const std::string a_iterador2, const std::string a_attMatriz, const std::string a_valor, const int a_alocacao_1, const int a_alocacao_2){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			const Id##Membro id##Membro = getId####Membro##FromChar(a_idMembro.c_str()); \
			if (!vetor##Membro.isInstanciado(id##Membro)){ \
				Membro objeto##Membro; \
				objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idMembro); \
				vetor##Membro.add(objeto##Membro);\
			} \
			vetor##Membro.att(id##Membro).addDadoAttMatrizMembro(a_membroMembro, a_idMembroMembro, a_iterador1, a_iterador2, a_attMatriz, a_valor, a_alocacao_1, a_alocacao_2); \
			return true; \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadoAttMatriz" + std::string(#Membro) + "_Membro("  + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_iterador1 + "," + a_iterador2 + "," + a_attMatriz + "," + a_valor + "):\n" + std::string(erro.what())); } \
}; \
bool addDadoAttMatriz##Membro##_MembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_iterador1, const std::string a_iterador2, const std::string a_attMatriz, const std::string a_valor, const int a_alocacao_1, const int a_alocacao_2){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			const Id##Membro id##Membro = getId####Membro##FromChar(a_idMembro.c_str()); \
			if (!vetor##Membro.isInstanciado(id##Membro)){ \
				Membro objeto##Membro; \
				objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idMembro); \
				vetor##Membro.add(objeto##Membro);\
			} \
			vetor##Membro.att(id##Membro).addDadoAttMatrizMembroMembro(a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_iterador1, a_iterador2, a_attMatriz, a_valor, a_alocacao_1, a_alocacao_2); \
			return true; \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadoAttMatriz" + std::string(#Membro) + "_MembroMembro("  + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_iterador1 + "," + a_iterador2 + "," + a_attMatriz + "," + a_valor +  "):\n" + std::string(erro.what())); } \
}; \
bool addDadoAttMatriz##Membro##_MembroMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_membroMembroMembroMembro, const std::string a_idMembroMembroMembroMembro, const std::string a_iterador1, const std::string a_iterador2, const std::string a_attMatriz, const std::string a_valor, const int a_alocacao_1, const int a_alocacao_2){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			const Id##Membro id##Membro = getId####Membro##FromChar(a_idMembro.c_str()); \
			if (!vetor##Membro.isInstanciado(id##Membro)){ \
				Membro objeto##Membro; \
				objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idMembro); \
				vetor##Membro.add(objeto##Membro);\
			} \
			vetor##Membro.att(id##Membro).addDadoAttMatrizMembroMembroMembro(a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_membroMembroMembroMembro, a_idMembroMembroMembroMembro, a_iterador1, a_iterador2, a_attMatriz, a_valor, a_alocacao_1, a_alocacao_2); \
			return true; \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadoAttMatriz" + std::string(#Membro) + "_MembroMembroMembro("  + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_membroMembroMembroMembro + "," + a_idMembroMembroMembroMembro + "," + a_iterador1 + "," + a_iterador2 + "," + a_attMatriz + "," + a_valor +  "):\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttMatriz##Membro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			std::vector<std::vector<std::string>> dados; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idMembro.c_str()); \
			std::vector<Id##Membro> vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (idMembro != Id##Membro##_Nenhum) {\
				if (!vetor##Membro.isInstanciado(idMembro)) \
					throw std::invalid_argument("Objeto nao instanciado.");\
				vetorIdMembro = std::vector<Id##Membro>{idMembro}; \
			} \
			else \
				vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (vetorIdMembro.size() > 0){ \
				for (int i = 0; i < int(vetorIdMembro.size()); i++) {\
					const std::string idMembroStr = getString(getAtributo(vetorIdMembro.at(i), AttComum##Membro##_id##Membro, Id##Membro())); \
					if (i == 0){ \
						dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttMatriz(a_incluirCabecalho, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
						if (a_incluirCabecalho) \
							dados.at(0).insert(dados.at(0).begin(), std::string("id" + std::string(#Membro))); \
					} \
					else\
						dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttMatriz(false, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
					for (int k = 0; k < int(dados.size()); k++){ \
						if ((k == 0) && (i == 0) && (a_incluirCabecalho)){} \
						else \
							dados.at(k).insert(dados.at(k).begin(), idMembroStr); \
					} \
					matrizRetorno.insert(matrizRetorno.end(), dados.begin(), dados.end()); \
				} \
				return matrizRetorno; \
			} \
			else \
				return std::vector<std::vector<std::string>>(1, std::vector<std::string>()); \
		} \
		else \
			return std::vector<std::vector<std::string>>(); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttMatriz" + std::string(#Membro) + "(" + getString(a_incluirCabecalho) + "," + a_membro + "," + a_idMembro + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + "," + getString(a_incluirAtributo) + ",a_vetorAttMatriz):\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttMatriz##Membro##_Membro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			bool primeiroDado = false; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idMembro.c_str()); \
			std::vector<Id##Membro> vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (idMembro != Id##Membro##_Nenhum) {\
				if (!vetor##Membro.isInstanciado(idMembro)) \
					throw std::invalid_argument("Objeto nao instanciado.");\
				vetorIdMembro = std::vector<Id##Membro>{idMembro}; \
			} \
			else \
				vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (vetorIdMembro.size() > 0){ \
				for (int i = 0; i < int(vetorIdMembro.size()); i++) {\
					if (primeiroDado == false) {\
						std::vector<std::vector<std::string>> dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttMatrizMembro(a_incluirCabecalho, a_membroMembro, a_idMembroMembro, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
						if (dados.at(0).size() > 0){ \
							for (int lin = 0; lin < int(dados.size()); lin++){\
								if ((lin == 0) && (a_incluirCabecalho)) \
									dados.at(0).insert(dados.at(0).begin(), std::string("id" + std::string(#Membro)));\
								else \
									dados.at(lin).insert(dados.at(lin).begin(), getString(getAtributo(vetorIdMembro.at(i), AttComum##Membro##_id##Membro, Id##Membro())));\
							} \
							matrizRetorno = dados;\
							primeiroDado = true; \
						} \
					} \
					else {\
						std::vector<std::vector<std::string>> dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttMatrizMembro(false, a_membroMembro, a_idMembroMembro, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
						if (dados.at(0).size() > 0){ \
							for (int lin = 0; lin < int(dados.size()); lin++)\
								dados.at(lin).insert(dados.at(lin).begin(), getString(getAtributo(vetorIdMembro.at(i), AttComum##Membro##_id##Membro, Id##Membro())));\
							matrizRetorno.insert(matrizRetorno.end(), dados.begin(), dados.end()); \
						} \
					} \
				} \
				return matrizRetorno; \
			} \
			else \
				return std::vector<std::vector<std::string>>(1, std::vector<std::string>()); \
		} \
		else \
			return std::vector<std::vector<std::string>>(); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttMatriz" + std::string(#Membro) + "_Membro("+ getString(a_incluirCabecalho) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + "," + getString(a_incluirAtributo) + ",a_vetorAttMatriz):\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttMatriz##Membro##_MembroMembro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			bool primeiroDado = false; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idMembro.c_str()); \
			std::vector<Id##Membro> vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (idMembro != Id##Membro##_Nenhum) {\
				if (!vetor##Membro.isInstanciado(idMembro)) \
					throw std::invalid_argument("Objeto nao instanciado.");\
				vetorIdMembro = std::vector<Id##Membro>{idMembro}; \
			} \
			else \
				vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (vetorIdMembro.size() > 0){ \
				for (int i = 0; i < int(vetorIdMembro.size()); i++) {\
					if (primeiroDado == false) {\
						std::vector<std::vector<std::string>> dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttMatrizMembroMembro(a_incluirCabecalho, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
						if (dados.at(0).size() > 0){ \
							for (int lin = 0; lin < int(dados.size()); lin++){\
								if ((lin == 0) && (a_incluirCabecalho)) \
									dados.at(0).insert(dados.at(0).begin(), std::string("id" + std::string(#Membro)));\
								else \
									dados.at(lin).insert(dados.at(lin).begin(), getString(getAtributo(vetorIdMembro.at(i), AttComum##Membro##_id##Membro, Id##Membro())));\
							} \
							matrizRetorno = dados;\
							primeiroDado = true; \
						} \
					} \
					else {\
						std::vector<std::vector<std::string>> dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttMatrizMembroMembro(false, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
						if (dados.at(0).size() > 0){ \
							for (int lin = 0; lin < int(dados.size()); lin++)\
								dados.at(lin).insert(dados.at(lin).begin(), getString(getAtributo(vetorIdMembro.at(i), AttComum##Membro##_id##Membro, Id##Membro())));\
							matrizRetorno.insert(matrizRetorno.end(), dados.begin(), dados.end()); \
						} \
					} \
				} \
				return matrizRetorno; \
			} \
			else \
				return std::vector<std::vector<std::string>>(1, std::vector<std::string>()); \
		} \
		else \
			return std::vector<std::vector<std::string>>(); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttMatriz" + std::string(#Membro) + "_MembroMembro("+ getString(a_incluirCabecalho) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + "," + getString(a_incluirAtributo) + ",a_vetorAttMatriz):\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttMatriz##Membro##_MembroMembroMembro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_membroMembroMembroMembro, const std::string a_idMembroMembroMembroMembro, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz){ \
	try{ \
		if (strCompara(a_membro, std::string(#Membro))){ \
			bool primeiroDado = false; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idMembro.c_str()); \
			std::vector<Id##Membro> vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (idMembro != Id##Membro##_Nenhum) {\
				if (!vetor##Membro.isInstanciado(idMembro)) \
					throw std::invalid_argument("Objeto nao instanciado.");\
				vetorIdMembro = std::vector<Id##Membro>{idMembro}; \
			} \
			else \
				vetorIdMembro = vetor##Membro.getIdObjetos(); \
			if (vetorIdMembro.size() > 0){ \
				for (int i = 0; i < int(vetorIdMembro.size()); i++) {\
					if (primeiroDado == false) {\
						std::vector<std::vector<std::string>> dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttMatrizMembroMembroMembro(a_incluirCabecalho, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_membroMembroMembroMembro, a_idMembroMembroMembroMembro, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
						if (dados.at(0).size() > 0){ \
							for (int lin = 0; lin < int(dados.size()); lin++){\
								if ((lin == 0) && (a_incluirCabecalho)) \
									dados.at(0).insert(dados.at(0).begin(), std::string("id" + std::string(#Membro)));\
								else \
									dados.at(lin).insert(dados.at(lin).begin(), getString(getAtributo(vetorIdMembro.at(i), AttComum##Membro##_id##Membro, Id##Membro())));\
							} \
							matrizRetorno = dados;\
							primeiroDado = true; \
						} \
					} \
					else {\
						std::vector<std::vector<std::string>> dados = vetor##Membro.att(vetorIdMembro.at(i)).getDadosAttMatrizMembroMembroMembro(false, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_membroMembroMembroMembro, a_idMembroMembroMembroMembro, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
						if (dados.at(0).size() > 0){ \
							for (int lin = 0; lin < int(dados.size()); lin++)\
								dados.at(lin).insert(dados.at(lin).begin(), getString(getAtributo(vetorIdMembro.at(i), AttComum##Membro##_id##Membro, Id##Membro())));\
							matrizRetorno.insert(matrizRetorno.end(), dados.begin(), dados.end()); \
						} \
					} \
				} \
				return matrizRetorno; \
			} \
			else \
				return std::vector<std::vector<std::string>>(1, std::vector<std::string>()); \
		} \
		else \
			return std::vector<std::vector<std::string>>(); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttMatriz" + std::string(#Membro) + "_MembroMembroMembro("+ getString(a_incluirCabecalho) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_membroMembroMembroMembro + "," + a_idMembroMembroMembroMembro + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + "," + getString(a_incluirAtributo) + ",a_vetorAttMatriz):\n" + std::string(erro.what())); } \
}; \
template<typename Atributo, typename Iterador1> \
Iterador1 getIterador1Inicial(const Id##Membro a_idMembro, const Atributo a_atributo, const Iterador1 a_iterador1){ \
	try { return vetor##Membro.att(a_idMembro).getIterador1Inicial(a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador1Inicial(" + getString(a_idMembro) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1> \
Iterador1 getIterador1Final(const Id##Membro a_idMembro, const Atributo a_atributo, const Iterador1 a_iterador1){ \
	try { return vetor##Membro.att(a_idMembro).getIterador1Final(a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador1Final(" + getString(a_idMembro) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename IdMembroMembro> \
Iterador1 getIterador1Inicial(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const Atributo a_atributo, const Iterador1 a_iterador1){ \
	try { return vetor##Membro.att(a_idMembro).getIterador1Inicial(a_idMembroMembro, a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador1Inicial(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename IdMembroMembro> \
Iterador1 getIterador1Final(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const Atributo a_atributo, const Iterador1 a_iterador1){ \
	try { return vetor##Membro.att(a_idMembro).getIterador1Final(a_idMembroMembro, a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador1Final(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename IdMembroMembro, typename IdMembroMembroMembro> \
Iterador1 getIterador1Inicial(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const IdMembroMembroMembro a_idMembroMembroMembro, const Atributo a_atributo, const Iterador1 a_iterador1){ \
	try { return vetor##Membro.att(a_idMembro).getIterador1Inicial(a_idMembroMembro, a_idMembroMembroMembro, a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador1Inicial(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_idMembroMembroMembro) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename IdMembroMembro, typename IdMembroMembroMembro> \
Iterador1 getIterador1Final(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const IdMembroMembroMembro a_idMembroMembroMembro, const Atributo a_atributo, const Iterador1 a_iterador1){ \
	try { return vetor##Membro.att(a_idMembro).getIterador1Final(a_idMembroMembro, a_idMembroMembroMembro, a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador1Final(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_idMembroMembroMembro) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename IdMembroMembro, typename IdMembroMembroMembro, typename IdMembroMembroMembroMembro> \
Iterador1 getIterador1Inicial(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const IdMembroMembroMembro a_idMembroMembroMembro, const IdMembroMembroMembroMembro a_idMembroMembroMembroMembro, const Atributo a_atributo, const Iterador1 a_iterador1){ \
	try { return vetor##Membro.att(a_idMembro).getIterador1Inicial(a_idMembroMembro, a_idMembroMembroMembro, a_idMembroMembroMembroMembro, a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador1Inicial(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_idMembroMembroMembro) + "," + getString(a_idMembroMembroMembroMembro) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename IdMembroMembro, typename IdMembroMembroMembro, typename IdMembroMembroMembroMembro> \
Iterador1 getIterador1Final(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const IdMembroMembroMembro a_idMembroMembroMembro, const IdMembroMembroMembroMembro a_idMembroMembroMembroMembro, const Atributo a_atributo, const Iterador1 a_iterador1){ \
	try { return vetor##Membro.att(a_idMembro).getIterador1Final(a_idMembroMembro, a_idMembroMembroMembro, a_idMembroMembroMembroMembro, a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador1Final(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_idMembroMembroMembro) + "," + getString(a_idMembroMembroMembroMembro) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2> \
Iterador2 getIterador2Inicial(const Id##Membro a_idMembro, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2) { \
	try { return vetor##Membro.att(a_idMembro).getIterador2Inicial(a_atributo, a_iterador1, a_iterador2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador2Inicial(" + getString(a_idMembro) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2> \
Iterador2 getIterador2Final(const Id##Membro a_idMembro, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2){ \
	try { return vetor##Membro.att(a_idMembro).getIterador2Final(a_atributo, a_iterador1, a_iterador2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador2Final(" + getString(a_idMembro) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2, typename IdMembroMembro> \
Iterador2 getIterador2Inicial(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2){ \
	try { return vetor##Membro.att(a_idMembro).getIterador2Inicial(a_idMembroMembro, a_atributo, a_iterador1, a_iterador2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador2Inicial(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2, typename IdMembroMembro> \
Iterador2 getIterador2Final(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2){ \
	try { return vetor##Membro.att(a_idMembro).getIterador2Final(a_idMembroMembro, a_atributo, a_iterador1, a_iterador2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador2Final(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2, typename IdMembroMembro, typename IdMembroMembroMembro> \
Iterador2 getIterador2Inicial(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const IdMembroMembroMembro a_idMembroMembroMembro, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2){ \
	try { return vetor##Membro.att(a_idMembro).getIterador2Inicial(a_idMembroMembro, a_idMembroMembroMembro, a_atributo, a_iterador1, a_iterador2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador2Inicial(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_idMembroMembroMembro) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2, typename IdMembroMembro, typename IdMembroMembroMembro> \
Iterador2 getIterador2Final(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const IdMembroMembroMembro a_idMembroMembroMembro, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2){ \
	try { return vetor##Membro.att(a_idMembro).getIterador2Final(a_idMembroMembro, a_idMembroMembroMembro, a_atributo, a_iterador1, a_iterador2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador2Final(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_idMembroMembroMembro) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2, typename IdMembroMembro, typename IdMembroMembroMembro, typename IdMembroMembroMembroMembro> \
Iterador2 getIterador2Inicial(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const IdMembroMembroMembro a_idMembroMembroMembro, const IdMembroMembroMembroMembro a_idMembroMembroMembroMembro, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2){ \
	try { return vetor##Membro.att(a_idMembro).getIterador2Inicial(a_idMembroMembro, a_idMembroMembroMembro, a_idMembroMembroMembroMembro, a_atributo, a_iterador1, a_iterador2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador2Inicial(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_idMembroMembroMembro) + "," + getString(a_idMembroMembroMembroMembro) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2, typename IdMembroMembro, typename IdMembroMembroMembro, typename IdMembroMembroMembroMembro> \
Iterador2 getIterador2Final(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const IdMembroMembroMembro a_idMembroMembroMembro, const IdMembroMembroMembroMembro a_idMembroMembroMembroMembro, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2){ \
	try { return vetor##Membro.att(a_idMembro).getIterador2Final(a_idMembroMembro, a_idMembroMembroMembro, a_idMembroMembroMembroMembro, a_atributo, a_iterador1, a_iterador2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador2Final(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_idMembroMembroMembro) + "," + getString(a_idMembroMembroMembroMembro) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2, typename Valor> \
Valor getElementoMatriz(const Id##Membro a_membro, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2, const Valor a_valor){ \
	try { return vetor##Membro.att(a_membro).getElementoMatriz(a_atributo, a_iterador1, a_iterador2, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getElementoMatriz(" + getString(a_membro) + "," + getString(a_atributo) + "," + getString(a_iterador1) + "," + getString(a_iterador2) + ",Valor): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2, typename Valor, typename IdMembroMembro> \
Valor getElementoMatriz(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2, const Valor a_valor){ \
	try { return vetor##Membro.att(a_idMembro).getElementoMatriz(a_idMembroMembro, a_atributo, a_iterador1, a_iterador2, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getElementoMatriz(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_atributo) + "," + getString(a_iterador1) + "," + getString(a_iterador2) + ",Valor): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2, typename Valor, typename IdMembroMembro, typename IdMembroMembroMembro> \
Valor getElementoMatriz(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const IdMembroMembroMembro a_idMembroMembroMembro, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2, const Valor a_valor){ \
	try { return vetor##Membro.att(a_idMembro).getElementoMatriz(a_idMembroMembro, a_idMembroMembroMembro, a_atributo, a_iterador1, a_iterador2, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getElementoMatriz(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_idMembroMembroMembro) + "," + getString(a_atributo) + "," + getString(a_iterador1) + "," + getString(a_iterador2) + ",Valor): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2, typename Valor, typename IdMembroMembro, typename IdMembroMembroMembro, typename IdMembroMembroMembroMembro> \
Valor getElementoMatriz(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const IdMembroMembroMembro a_idMembroMembroMembro, const IdMembroMembroMembroMembro a_idMembroMembroMembroMembro, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2, const Valor a_valor){ \
	try { return vetor##Membro.att(a_idMembro).getElementoMatriz(a_idMembroMembro, a_idMembroMembroMembro, a_idMembroMembroMembroMembro, a_atributo, a_iterador1, a_iterador2, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getElementoMatriz(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_idMembroMembroMembro) + "," + getString(a_idMembroMembroMembroMembro) + "," + getString(a_atributo) + "," + getString(a_iterador1) + "," + getString(a_iterador2) + ",Valor): \n" + std::string(erro.what())); } \
};\
template<typename Atributo> \
int getSize1Matriz(const Id##Membro a_idMembro, const Atributo a_atributo){ \
	try { return vetor##Membro.att(a_idMembro).getSizeMatriz(a_atributo); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeMatriz(" + getString(a_idMembro) + "," + getString(a_atributo) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename IdMembroMembro> \
int getSize1Matriz(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const Atributo a_atributo){ \
	try { return vetor##Membro.att(a_idMembro).getSize1Matriz(a_idMembroMembro, a_atributo); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeMatriz(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_atributo) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename IdMembroMembro, typename IdMembroMembroMembro> \
int getSize1Matriz(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const IdMembroMembroMembro a_idMembroMembroMembro, const Atributo a_atributo){ \
	try { return vetor##Membro.att(a_idMembro).getSize1Matriz(a_idMembroMembro, a_idMembroMembroMembro, a_atributo); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeMatriz(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_idMembroMembroMembro) + "," + getString(a_atributo) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename IdMembroMembro, typename IdMembroMembroMembro, typename IdMembroMembroMembroMembro> \
int getSize1Matriz(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const IdMembroMembroMembro a_idMembroMembroMembro, const IdMembroMembroMembroMembro a_idMembroMembroMembroMembro, const Atributo a_atributo){ \
	try { return vetor##Membro.att(a_idMembro).getSize1Matriz(a_idMembroMembro, a_idMembroMembroMembro, a_idMembroMembroMembroMembro, a_atributo); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeMatriz(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_idMembroMembroMembro) + "," + getString(a_idMembroMembroMembroMembro) + "," + getString(a_atributo) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1> \
int getSize2Matriz(const Id##Membro a_idMembro, const Atributo a_atributo, Iterador1 a_iterador1){ \
	try { return vetor##Membro.att(a_idMembro).getSizeMatriz(a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeMatriz(" + getString(a_idMembro) + "," + getString(a_atributo) + "," + getString(a_iterador1) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename IdMembroMembro> \
int getSize2Matriz(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const Atributo a_atributo, Iterador1 a_iterador1){ \
	try { return vetor##Membro.att(a_idMembro).getSize2Matriz(a_idMembroMembro, a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeMatriz(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_atributo) + "," + getString(a_iterador1) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename IdMembroMembro, typename IdMembroMembroMembro> \
int getSize2Matriz(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const IdMembroMembroMembro a_idMembroMembroMembro, const Atributo a_atributo, Iterador1 a_iterador1){ \
	try { return vetor##Membro.att(a_idMembro).getSize2Matriz(a_idMembroMembro, a_idMembroMembroMembro, a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeMatriz(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_idMembroMembroMembro) + "," + getString(a_atributo) + "," + getString(a_iterador1) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename IdMembroMembro, typename IdMembroMembroMembro, typename IdMembroMembroMembroMembro> \
int getSize2Matriz(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const IdMembroMembroMembro a_idMembroMembroMembro, const IdMembroMembroMembroMembro a_idMembroMembroMembroMembro, const Atributo a_atributo, Iterador1 a_iterador1){ \
	try { return vetor##Membro.att(a_idMembro).getSize2Matriz(a_idMembroMembro, a_idMembroMembroMembro, a_idMembroMembroMembroMembro, a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeMatriz(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_idMembroMembroMembro) + "," + getString(a_idMembroMembroMembroMembro) + "," + getString(a_atributo) + "," + getString(a_iterador1) + "): \n" + std::string(erro.what())); } \
};\
template<typename AttMatriz, typename TipoIterador1, typename TipoIterador2, typename TipoValor> \
SmartEnupla<TipoIterador2, TipoValor> getElementosMatriz(const Id##Membro a_idMembro, const AttMatriz a_attMatriz, const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2, const TipoValor a_tipoValor){ \
	try { return vetor##Membro.att(a_idMembro).getElementosMatriz(a_attMatriz, a_iterador1, a_iterador2, a_tipoValor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getElementosMatriz(" + getString(a_idMembro) + "," + getString(a_attMatriz) + ",a_iterador1,a_iterador2,a_tipoValor): \n" + std::string(erro.what())); } \
};\
template<typename IdMembroMembro, typename AttMatriz, typename TipoIterador1, typename TipoIterador2, typename TipoValor> \
SmartEnupla<TipoIterador2, TipoValor> getElementosMatriz(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const AttMatriz a_attMatriz, const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2, const TipoValor a_tipoValor){ \
	try { return vetor##Membro.att(a_idMembro).getElementosMatriz(a_idMembroMembro, a_attMatriz, a_iterador1, a_iterador2, a_tipoValor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getElementosMatriz(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_attMatriz) + ",a_iterador1,a_iterador2,a_tipoValor): \n" + std::string(erro.what())); } \
};\
template<typename IdMembroMembroMembro, typename IdMembroMembro, typename AttMatriz, typename TipoIterador1, typename TipoIterador2, typename TipoValor> \
SmartEnupla<TipoIterador2, TipoValor> getElementosMatriz(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const IdMembroMembroMembro a_idMembroMembroMembro, const AttMatriz a_attMatriz, const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2, const TipoValor a_tipoValor){ \
	try { return vetor##Membro.att(a_idMembro).getElementosMatriz(a_idMembroMembro, a_idMembroMembroMembro, a_attMatriz, a_iterador1, a_iterador2, a_tipoValor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getElementosMatriz(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_idMembroMembroMembro) + "," + getString(a_attMatriz) + ",a_iterador1,a_iterador2,a_tipoValor): \n" + std::string(erro.what())); } \
};\
template<typename AttMatriz, typename TipoIterador1, typename TipoIterador2, typename TipoValor> \
SmartEnupla<TipoIterador1, SmartEnupla<TipoIterador2, TipoValor>> getMatriz(const Id##Membro a_idMembro, const AttMatriz a_attMatriz, const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2, const TipoValor a_tipoValor){ \
	try { return vetor##Membro.att(a_idMembro).getMatriz(a_attMatriz, a_iterador1, a_iterador2, a_tipoValor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getMatriz(" + getString(a_idMembro) + "," + getString(a_attMatriz) + ",a_iterador1,a_iterador2,a_tipoValor): \n" + std::string(erro.what())); } \
};\
template<typename IdMembroMembro, typename AttMatriz, typename TipoIterador1, typename TipoIterador2, typename TipoValor> \
SmartEnupla<TipoIterador1, SmartEnupla<TipoIterador2, TipoValor>> getMatriz(const Id##Membro a_idMembro, const IdMembroMembro a_idMembroMembro, const AttMatriz a_attMatriz, const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2, const TipoValor a_tipoValor){ \
	try { return vetor##Membro.att(a_idMembro).getMatriz(a_idMembroMembro, a_attMatriz, a_iterador1, a_iterador2, a_tipoValor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getMatriz(" + getString(a_idMembro) + "," + getString(a_idMembroMembro) + "," + getString(a_attMatriz) + ",a_iterador1,a_iterador2,a_tipoValor): \n" + std::string(erro.what())); } \
};\

//
// AttComum
//

#define DEFINE_addDadosAttComumMembro(Classe, Membro)\
if (addDadosAttComum##Membro(a_membro, a_idMembro, a_vetorAttComuns, a_vetorValores)) \
	return; 

#define DEFINE_addDadosAttComumMembroMembro(Classe, Membro)\
if (addDadosAttComum##Membro##_Membro(a_membro, a_idMembro, a_membroMembro, a_idMembroMembro, a_vetorAttComuns, a_vetorValores)) \
	return; 

#define DEFINE_addDadosAttComumMembroMembroMembro(Classe, Membro)\
if (addDadosAttComum##Membro##_MembroMembro(a_membro, a_idMembro, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_vetorAttComuns, a_vetorValores)) \
	return; 

#define DEFINE_addDadosAttComumMembroMembroMembroMembro(Classe, Membro)\
if (addDadosAttComum##Membro##_MembroMembroMembro(a_membro, a_idMembro, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_membroMembroMembroMembro, a_idMembroMembroMembroMembro, a_vetorAttComuns, a_vetorValores)) \
	return; 

#define DEFINE_getDadosAttComumMembro(Classe, Membro)\
matrizRetorno = getDadosAttComum##Membro(a_incluirCabecalho, a_membro, a_idMembro, a_vetorAttComum); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno; 

#define DEFINE_getDadosAttComumMembroMembro(Classe, Membro)\
matrizRetorno = getDadosAttComum##Membro##_Membro(a_incluirCabecalho, a_membro, a_idMembro, a_membroMembro, a_idMembroMembro, a_vetorAttComum); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno; 

#define DEFINE_getDadosAttComumMembroMembroMembro(Classe, Membro)\
matrizRetorno = getDadosAttComum##Membro##_MembroMembro(a_incluirCabecalho, a_membro, a_idMembro, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_vetorAttComum); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno; 

#define DEFINE_getDadosAttComumMembroMembroMembroMembro(Classe, Membro)\
matrizRetorno = getDadosAttComum##Membro##_MembroMembroMembro(a_incluirCabecalho, a_membro, a_idMembro, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_membroMembroMembroMembro, a_idMembroMembroMembroMembro, a_vetorAttComum); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno; 

//
// AttVetor
//

#define DEFINE_addDadoAttVetorMembro(Classe, Membro)\
if (addDadoAttVetor##Membro(a_membro, a_idMembro, a_iterador, a_attVetor, a_valor, a_alocacao)) \
	return; 

#define DEFINE_addDadoAttVetorMembroMembro(Classe, Membro)\
if (addDadoAttVetor##Membro##_Membro(a_membro, a_idMembro, a_membroMembro, a_idMembroMembro, a_iterador, a_attVetor, a_valor, a_alocacao)) \
	return; 

#define DEFINE_addDadoAttVetorMembroMembroMembro(Classe, Membro)\
if (addDadoAttVetor##Membro##_MembroMembro(a_membro, a_idMembro, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_iterador, a_attVetor, a_valor, a_alocacao)) \
	return; 

#define DEFINE_addDadoAttVetorMembroMembroMembroMembro(Classe, Membro)\
if (addDadoAttVetor##Membro##_MembroMembroMembro(a_membro, a_idMembro, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_membroMembroMembroMembro, a_idMembroMembroMembroMembro, a_iterador, a_attVetor, a_valor, a_alocacao)) \
	return; 

#define DEFINE_getDadosAttVetorMembro(Classe, Membro)\
matrizRetorno = getDadosAttVetor##Membro(a_incluirCabecalho, a_membro, a_idMembro, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno;

#define DEFINE_getDadosAttVetorMembroMembro(Classe, Membro)\
matrizRetorno = getDadosAttVetor##Membro##_Membro(a_incluirCabecalho, a_membro, a_idMembro, a_membroMembro, a_idMembroMembro, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno;

#define DEFINE_getDadosAttVetorMembroMembroMembro(Classe, Membro)\
matrizRetorno = getDadosAttVetor##Membro##_MembroMembro(a_incluirCabecalho, a_membro, a_idMembro, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno;

#define DEFINE_getDadosAttVetorMembroMembroMembroMembro(Classe, Membro)\
matrizRetorno = getDadosAttVetor##Membro##_MembroMembroMembro(a_incluirCabecalho, a_membro, a_idMembro, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_membroMembroMembroMembro, a_idMembroMembroMembroMembro, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno;

//
// AttMatriz
//

#define DEFINE_addDadoAttMatrizMembro(Classe, Membro)\
if (addDadoAttMatriz##Membro(a_membro, a_idMembro, a_iterador1, a_iterador2, a_attMatriz, a_valor, a_alocacao_1, a_alocacao_2)) \
	return; 

#define DEFINE_addDadoAttMatrizMembroMembro(Classe, Membro)\
if (addDadoAttMatriz##Membro##_Membro(a_membro, a_idMembro, a_membroMembro, a_idMembroMembro, a_iterador1, a_iterador2, a_attMatriz, a_valor, a_alocacao_1, a_alocacao_2)) \
	return; 

#define DEFINE_addDadoAttMatrizMembroMembroMembro(Classe, Membro)\
if (addDadoAttMatriz##Membro##_MembroMembro(a_membro, a_idMembro, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_iterador1, a_iterador2, a_attMatriz, a_valor, a_alocacao_1, a_alocacao_2)) \
	return; 

#define DEFINE_addDadoAttMatrizMembroMembroMembroMembro(Classe, Membro)\
if (addDadoAttMatriz##Membro##_MembroMembroMembro(a_membro, a_idMembro, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_membroMembroMembroMembro, a_idMembroMembroMembroMembro, a_iterador1, a_iterador2, a_attMatriz, a_valor, a_alocacao_1, a_alocacao_2)) \
	return; 

#define DEFINE_getDadosAttMatrizMembro(Classe, Membro)\
matrizRetorno = getDadosAttMatriz##Membro(a_incluirCabecalho, a_membro, a_idMembro, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno;

#define DEFINE_getDadosAttMatrizMembroMembro(Classe, Membro)\
matrizRetorno = getDadosAttMatriz##Membro##_Membro(a_incluirCabecalho, a_membro, a_idMembro, a_membroMembro, a_idMembroMembro, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno;

#define DEFINE_getDadosAttMatrizMembroMembroMembro(Classe, Membro)\
matrizRetorno = getDadosAttMatriz##Membro##_MembroMembro(a_incluirCabecalho, a_membro, a_idMembro, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno;

#define DEFINE_getDadosAttMatrizMembroMembroMembroMembro(Classe, Membro)\
matrizRetorno = getDadosAttMatriz##Membro##_MembroMembroMembro(a_incluirCabecalho, a_membro, a_idMembro, a_membroMembro, a_idMembroMembro, a_membroMembroMembro, a_idMembroMembroMembro, a_membroMembroMembroMembro, a_idMembroMembroMembroMembro, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno;

#define DECLARA_METODOS_Membro(Classe, SmartElemento, valores)\
valores(DEFINE_METODOS_MEMBROS) \
virtual void addDadosAttComumMembro(const std::string a_membro, const std::string a_idMembro, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores){ \
	try{ \
		valores(DEFINE_addDadosAttComumMembro) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttComumMembro:(" + a_membro + "," + a_idMembro + ",a_vetorAttComuns,a_vetorValores):\n" + std::string(erro.what())); }\
}; \
virtual void addDadosAttComumMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores) {\
	try { \
		valores(DEFINE_addDadosAttComumMembroMembro) \
		throw std::invalid_argument("Membro ou Membro Filho nao encontrados."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttComumMembroMembro:(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + ",a_vetorAttComuns,a_vetorValores):\n" + std::string(erro.what())); }\
}; \
virtual void addDadosAttComumMembroMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores) {\
	try { \
		valores(DEFINE_addDadosAttComumMembroMembroMembro) \
		throw std::invalid_argument("Membro ou Membro Filho nao encontrados."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttComumMembroMembroMembro:(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + ",a_vetorAttComuns,a_vetorValores):\n" + std::string(erro.what())); }\
}; \
virtual void addDadosAttComumMembroMembroMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_membroMembroMembroMembro, const std::string a_idMembroMembroMembroMembro, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores) {\
	try { \
		valores(DEFINE_addDadosAttComumMembroMembroMembroMembro) \
		throw std::invalid_argument("Membro ou Membro Filho nao encontrados."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttComumMembroMembroMembroMembro:(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_membroMembroMembroMembro + "," + a_idMembroMembroMembroMembro + ",a_vetorAttComuns,a_vetorValores):\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttComumMembro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::vector<std::string> a_vetorAttComum){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttComumMembro) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttComumMembro:(" + getString(a_incluirCabecalho) + "," + a_membro + "," + a_idMembro + ",a_vetorAttComum):\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttComumMembroMembro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::vector<std::string> a_vetorAttComum){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttComumMembroMembro) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttComumMembroMembro:(" + getString(a_incluirCabecalho) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + ",a_vetorAttComum):\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttComumMembroMembroMembro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::vector<std::string> a_vetorAttComum){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttComumMembroMembroMembro) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttComumMembroMembroMembro:(" + getString(a_incluirCabecalho) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + ",a_vetorAttComum):\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttComumMembroMembroMembroMembro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_membroMembroMembroMembro, const std::string a_idMembroMembroMembroMembro, const std::vector<std::string> a_vetorAttComum){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttComumMembroMembroMembroMembro) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttComumMembroMembroMembroMembro:(" + getString(a_incluirCabecalho) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_membroMembroMembroMembro + "," + a_idMembroMembroMembroMembro + ",a_vetorAttComum):\n" + std::string(erro.what())); }\
}; \
virtual void addDadoAttVetorMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_iterador, const std::string a_attVetor, const std::string a_valor, const int a_alocacao){ \
	try{ \
		valores(DEFINE_addDadoAttVetorMembro) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttVetorMembro:(" + a_membro + "," + a_idMembro + "," + a_iterador + "," + a_attVetor + "," + a_valor + "):\n" + std::string(erro.what())); }\
}; \
virtual void addDadoAttVetorMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_iterador, const std::string a_attVetor, const std::string a_valor, const int a_alocacao) {\
	try { \
		valores(DEFINE_addDadoAttVetorMembroMembro) \
		throw std::invalid_argument("Membro ou Membro Filho nao encontrados."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttVetorMembroMembro:(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_iterador + "," + a_attVetor + "," + a_valor + "):\n" + std::string(erro.what())); }\
}; \
virtual void addDadoAttVetorMembroMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_iterador, const std::string a_attVetor, const std::string a_valor, const int a_alocacao) {\
	try { \
		valores(DEFINE_addDadoAttVetorMembroMembroMembro) \
		throw std::invalid_argument("Membro ou Membro Filho nao encontrados."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttVetorMembroMembroMembro:(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_iterador + "," + a_attVetor + "," + a_valor + "):\n" + std::string(erro.what())); }\
}; \
virtual void addDadoAttVetorMembroMembroMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_membroMembroMembroMembro, const std::string a_idMembroMembroMembroMembro, const std::string a_iterador, const std::string a_attVetor, const std::string a_valor, const int a_alocacao) {\
	try { \
		valores(DEFINE_addDadoAttVetorMembroMembroMembroMembro) \
		throw std::invalid_argument("Membro ou Membro Filho nao encontrados."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadoAttVetorMembroMembroMembroMembro:(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_membroMembroMembroMembro + "," + a_idMembroMembroMembroMembro + "," + a_iterador + "," + a_attVetor + "," + a_valor + "):\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttVetorMembro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttVetorMembro) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttVetorMembro:(" + getString(a_incluirCabecalho) + "," + a_membro + "," + a_idMembro + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_vetorAttVetor):\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttVetorMembroMembro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttVetorMembroMembro) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttVetorMembroMembro:(" + getString(a_incluirCabecalho) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_vetorAttVetor):\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttVetorMembroMembroMembro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttVetorMembroMembroMembro) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttVetorMembroMembro:(" + getString(a_incluirCabecalho) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_vetorAttVetor):\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttVetorMembroMembroMembroMembro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_membroMembroMembroMembro, const std::string a_idMembroMembroMembroMembro, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttVetorMembroMembroMembroMembro) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttVetorMembroMembroMembroMembro:(" + getString(a_incluirCabecalho) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_membroMembroMembroMembro + "," + a_idMembroMembroMembroMembro + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_vetorAttVetor):\n" + std::string(erro.what())); }\
}; \
virtual void addDadoAttMatrizMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_iterador1, const std::string a_iterador2, const std::string a_attMatriz, const std::string a_valor, const int a_alocacao_1, const int a_alocacao_2){ \
	try{ \
		valores(DEFINE_addDadoAttMatrizMembro) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttMatrizMembro:(" + a_membro + "," + a_idMembro + "," + a_iterador1 + "," + a_iterador2 + "," + a_attMatriz + "," + a_valor + "):\n" + std::string(erro.what())); }\
}; \
virtual void addDadoAttMatrizMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_iterador1, const std::string a_iterador2, const std::string a_attMatriz, const std::string a_valor, const int a_alocacao_1, const int a_alocacao_2) {\
	try { \
		valores(DEFINE_addDadoAttMatrizMembroMembro) \
		throw std::invalid_argument("Membro ou Membro Filho nao encontrados."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttMatrizMembroMembro:(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_iterador1 + "," + a_iterador2 + "," + a_attMatriz + "," + a_valor + "):\n" + std::string(erro.what())); }\
}; \
virtual void addDadoAttMatrizMembroMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_iterador1, const std::string a_iterador2, const std::string a_attMatriz, const std::string a_valor, const int a_alocacao_1, const int a_alocacao_2) {\
	try { \
		valores(DEFINE_addDadoAttMatrizMembroMembroMembro) \
		throw std::invalid_argument("Membro ou Membro Filho nao encontrados."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttMatrizMembroMembroMembro:(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_iterador1 + "," + a_iterador2 + "," + a_attMatriz + "," + a_valor + "):\n" + std::string(erro.what())); }\
}; \
virtual void addDadoAttMatrizMembroMembroMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_membroMembroMembroMembro, const std::string a_idMembroMembroMembroMembro, const std::string a_iterador1, const std::string a_iterador2, const std::string a_attMatriz, const std::string a_valor, const int a_alocacao_1, const int a_alocacao_2) {\
	try { \
		valores(DEFINE_addDadoAttMatrizMembroMembroMembroMembro) \
		throw std::invalid_argument("Membro ou Membro Filho nao encontrados."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadoAttMatrizMembroMembroMembroMembro:(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_membroMembroMembroMembro + "," + a_idMembroMembroMembroMembro + "," + a_iterador1 + "," + a_iterador2 + "," + a_attMatriz + "," + a_valor + "):\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttMatrizMembro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttMatrizMembro) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttMatrizMembro:(" + getString(a_incluirCabecalho) + "," + a_membro + "," + a_idMembro + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + ",a_vetorAttMatriz):\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttMatrizMembroMembro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttMatrizMembroMembro) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttMatrizMembroMembro:(" + getString(a_incluirCabecalho) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + ",a_vetorAttMatriz):\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttMatrizMembroMembroMembro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttMatrizMembroMembroMembro) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttMatrizMembroMembro:(" + getString(a_incluirCabecalho) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + ",a_vetorAttMatriz):\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttMatrizMembroMembroMembroMembro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_membroMembroMembroMembro, const std::string a_idMembroMembroMembroMembro, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttMatrizMembroMembroMembroMembro) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttMatrizMembroMembroMembroMembro:(" + getString(a_incluirCabecalho) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_membroMembroMembroMembro + "," + a_idMembroMembroMembroMembro + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + ",a_vetorAttMatriz):\n" + std::string(erro.what())); }\
}; \

#define INICIALIZAR_Membro(Classe, Elemento, valores)

#define EXCLUIR_VALORES_Membro(Classe, Membro) \
vetor##Membro.esvaziar();

#define EXCLUIR_Membro(Classe, Elemento, valores) \
valores(EXCLUIR_VALORES_Membro) 

#define TRANSFERIR_VALORES_Membro(Classe, Membro) \
vetor##Membro = instanciaCopiar.vetor##Membro;

#define COPIAR_Membro(Classe, Elemento, valores)\
valores(TRANSFERIR_VALORES_Membro)


#endif  // SMARTENUM_H