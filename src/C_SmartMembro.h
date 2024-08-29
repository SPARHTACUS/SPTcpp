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
		menorId = Id##Membro##_Nenhum;\
		maiorId = Id##Membro##_Nenhum;\
		vetorObjeto = std::vector<std::vector<Membro>>();\
        numero_objetos = 0; \
	} \
	Vetor##Membro##Em##Classe(const Vetor##Membro##Em##Classe &instanciaCopiar){ \
		menorId = instanciaCopiar.menorId;\
		maiorId = instanciaCopiar.maiorId;\
		vetorObjeto = instanciaCopiar.vetorObjeto;\
        numero_objetos = instanciaCopiar.numero_objetos; \
	} \
	~Vetor##Membro##Em##Classe(){ \
		menorId = Id##Membro##_Nenhum;\
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
			menorId = Id##Membro##_Nenhum;\
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
		for (Id##Membro id = menorId; id <= maiorId; id++) {\
			if (isInstanciado(id)) { \
				if (vetorObjeto.at(id).at(0).getAtributo(a_atributo, Valor()) == a_valor) \
					numCont++; \
			}\
		} \
		return numCont; \
	};\
	Id##Membro getIdOut()const{ \
		if (maiorId > Id##Membro##_Nenhum)\
			return Id##Membro(maiorId + 1);\
		return maiorId;\
	};\
	Id##Membro getMaiorId()const{ return maiorId; };\
	Id##Membro getMenorId()const{ return menorId; };\
	std::vector<Id##Membro> getIdObjetos()const { \
		std::vector<Id##Membro> vetorIdMembro; \
		if (menorId == Id##Membro##_Nenhum) \
			return vetorIdMembro; \
		vetorIdMembro.reserve(int(maiorId) - int(menorId) + 1);\
		for (Id##Membro id = menorId; id <= maiorId; id++) {\
			if (isInstanciado(id)) \
				vetorIdMembro.push_back(id); \
		} \
		return vetorIdMembro; \
	};\
	void incr(Id##Membro &a_id)const{\
		if (maiorId == Id##Membro(0)){\
			a_id = Id##Membro##_Excedente; \
			return;\
		}\
		Id##Membro idInicial = Id##Membro(int(a_id) + 1); \
		if (idInicial < menorId) \
			idInicial = menorId; \
		for (Id##Membro id = idInicial; id <= maiorId; id++) {\
			if (isInstanciado(id)) {\
				a_id = id; \
				return; \
			} \
		} \
		a_id = Id##Membro##_Excedente; \
	};\
	template<typename Atributo, typename Valor> \
	std::vector<Id##Membro> getIdObjetos(const Atributo a_atributo, const Valor a_valor)const{ \
		if (menorId == Id##Membro##_Nenhum) \
			return std::vector<Id##Membro>(); \
		std::vector<Id##Membro> vetorIdMembros_filtro; \
		vetorIdMembros_filtro.reserve(int(maiorId));\
		for (Id##Membro id = menorId; id <= maiorId; id++) {\
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
			if (idObjeto <= Id##Membro##_Nenhum) \
				throw std::invalid_argument("Id invalido de objeto: menor que inicial."); \
			else if (idObjeto >= Id##Membro##_Excedente) \
				throw std::invalid_argument("Id invalido de objeto: maior ou igual a Excedente."); \
			if (idObjeto <= maiorId){ \
				if (isInstanciado(idObjeto))\
					throw std::invalid_argument("Objeto ja instanciado."); \
				else {\
					vetorObjeto.at(idObjeto).push_back(a_objeto); \
					if (idObjeto < menorId) \
						menorId = idObjeto;\
				}\
			} \
			else{ \
				if (vetorObjeto.size() == 0) {\
					vetorObjeto.push_back(std::vector<Membro>()); \
					menorId = idObjeto;\
				}\
				else if (menorId == Id##Membro##_Nenhum)\
					menorId = idObjeto;\
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
			if (!isInstanciado(a_idObjeto)) return;\
			if ((a_idObjeto == menorId) && (a_idObjeto == maiorId)){\
				maiorId = Id##Membro##_Nenhum;\
				menorId = Id##Membro##_Nenhum;\
			}\
			else if (a_idObjeto == menorId) { \
				for (Id##Membro id = menorId; id <= maiorId; id++) {\
					if (isInstanciado(id)){ \
						menorId = id; \
						break; \
					} \
				} \
            } \
			else if (a_idObjeto == maiorId) { \
				for (Id##Membro id = maiorId; id >= menorId; id--) {\
					if (isInstanciado(id)){ \
						maiorId = id; \
						break; \
					} \
				} \
            } \
			at(a_idObjeto).esvaziar(); \
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
	Membro& at(const Id##Membro a_idObjeto){\
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
	std::vector<std::vector<Membro>> vetorObjeto;\
	Id##Membro maiorId;\
	Id##Membro menorId;\
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
static std::string getString(membro##Classe a_m1){ \
	if (a_m1 >= membro##Classe##_Excedente) \
		return std::string(membro##Classe##Array[membro##Classe##_Excedente]); \
	else if (a_m1 <= membro##Classe##_Nenhum) \
		return std::string(membro##Classe##Array[membro##Classe##_Nenhum]); \
	else \
		return std::string(membro##Classe##Array[a_m1]); \
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
bool addDadosAttComum##Membro(const std::string a_m1, const std::string a_idM1, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			if ((a_vetorAttComuns.size() == 0) && (a_vetorValores.size() == 0)) \
				throw std::invalid_argument("Faltando informacoes dos argumentos a_vetorAttComuns e a_vetorValores"); \
			else{ \
				const Id##Membro id##Membro = getId####Membro##FromChar(a_idM1.c_str()); \
				if (!vetor##Membro.isInstanciado(id##Membro)){ \
					Membro objeto##Membro; \
					objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idM1); \
					vetor##Membro.add(objeto##Membro);\
				} \
				vetor##Membro.at(id##Membro).addDadosAttComum(a_idM1, a_vetorAttComuns, a_vetorValores); \
				vetor##Membro.at(id##Membro).validacaoDadosAttComum(); \
				return true; \
			} \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttComum" + std::string(#Membro) + "(" + a_m1 + "," + a_idM1 + ",a_vetorAttComuns,a_vetorValores):\n" + std::string(erro.what())); } \
}; \
bool addDadosAttComum##Membro##_Membro(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			if ((a_vetorAttComuns.size() == 0) && (a_vetorValores.size() == 0))\
				throw std::invalid_argument("Faltando informacoes dos argumentos a_vetorAttComuns e a_vetorValores"); \
			else{ \
				const Id##Membro id##Membro = getId####Membro##FromChar(a_idM1.c_str()); \
				if (!vetor##Membro.isInstanciado(id##Membro)){ \
					Membro objeto##Membro; \
					objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idM1); \
					vetor##Membro.add(objeto##Membro); \
				} \
				vetor##Membro.at(id##Membro).addDadosAttComumM1(a_m2, a_idM2, a_vetorAttComuns, a_vetorValores); \
				return true; \
			} \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttComum" + std::string(#Membro) + "_Membro("  + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 +  ",a_vetorAttComuns,a_vetorValores):\n" + std::string(erro.what())); } \
}; \
bool addDadosAttComum##Membro##_M2(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			if ((a_vetorAttComuns.size() == 0) && (a_vetorValores.size() == 0)) \
				throw std::invalid_argument("Faltando informacoes dos argumentos a_vetorAttComuns e a_vetorValores"); \
			else{ \
				const Id##Membro id##Membro = getId####Membro##FromChar(a_idM1.c_str()); \
				if (!vetor##Membro.isInstanciado(id##Membro)){ \
					Membro objeto##Membro; \
					objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idM1); \
					vetor##Membro.add(objeto##Membro); \
				} \
				vetor##Membro.at(id##Membro).addDadosAttComumM2(a_m2, a_idM2, a_m3, a_idM3, a_vetorAttComuns, a_vetorValores); \
				return true; \
			} \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttComum" + std::string(#Membro) + "_M2("  + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + ",a_vetorAttComuns,a_vetorValores):\n" + std::string(erro.what())); } \
}; \
bool addDadosAttComum##Membro##_M3(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_m4, const std::string a_idM4, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			if ((a_vetorAttComuns.size() == 0) && (a_vetorValores.size() == 0)) \
				throw std::invalid_argument("Faltando informacoes dos argumentos a_vetorAttComuns e a_vetorValores"); \
			else{ \
				const Id##Membro id##Membro = getId####Membro##FromChar(a_idM1.c_str()); \
				if (!vetor##Membro.isInstanciado(id##Membro)){ \
					Membro objeto##Membro; \
					objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idM1); \
					vetor##Membro.add(objeto##Membro); \
				} \
				vetor##Membro.at(id##Membro).addDadosAttComumM3(a_m2, a_idM2, a_m3, a_idM3, a_m4, a_idM4, a_vetorAttComuns, a_vetorValores); \
				return true; \
			} \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttComum" + std::string(#Membro) + "_M3("  + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + "," + a_m4 + "," + a_idM4 + ",a_vetorAttComuns,a_vetorValores):\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttComum##Membro(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::vector<std::string> a_vetorAttComum){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			std::vector<std::vector<std::string>> dados; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idM1.c_str()); \
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
						dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttComum(a_incluirCabecalho, a_vetorAttComum); \
					else \
						dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttComum(false, a_vetorAttComum); \
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
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttComum" + std::string(#Membro) + "(" + a_m1 + "," + a_idM1 + ",a_vetorAttComum):\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttComum##Membro##_Membro(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::vector<std::string> a_vetorAttComum){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			bool primeiroDado = false; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idM1.c_str()); \
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
						std::vector<std::vector<std::string>> dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttComumM1(a_incluirCabecalho, a_m2, a_idM2, a_vetorAttComum); \
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
						std::vector<std::vector<std::string>> dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttComumM1(false, a_m2, a_idM2, a_vetorAttComum); \
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
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttComum" + std::string(#Membro) + "_Membro("+ getString(a_incluirCabecalho) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + ",a_vetorAttComum):\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttComum##Membro##_M2(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::vector<std::string> a_vetorAttComum){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			bool primeiroDado = false; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idM1.c_str()); \
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
						std::vector<std::vector<std::string>> dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttComumM2(a_incluirCabecalho, a_m2, a_idM2, a_m3, a_idM3, a_vetorAttComum); \
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
						std::vector<std::vector<std::string>> dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttComumM2(false, a_m2, a_idM2, a_m3, a_idM3, a_vetorAttComum); \
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
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttComum" + std::string(#Membro) + "_M2("+ getString(a_incluirCabecalho) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + ",a_vetorAttComum):\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttComum##Membro##_M3(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_m4, const std::string a_idM4, const std::vector<std::string> a_vetorAttComum){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			bool primeiroDado = false; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idM1.c_str()); \
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
						std::vector<std::vector<std::string>> dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttComumM3(a_incluirCabecalho, a_m2, a_idM2, a_m3, a_idM3, a_m4, a_idM4, a_vetorAttComum); \
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
						std::vector<std::vector<std::string>> dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttComumM3(false, a_m2, a_idM2, a_m3, a_idM3, a_m4, a_idM4, a_vetorAttComum); \
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
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttComum" + std::string(#Membro) + "_M3("+ getString(a_incluirCabecalho) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + "," + a_m4 + "," + a_idM4 + ",a_vetorAttComum):\n" + std::string(erro.what())); } \
}; \
template<typename Atributo, typename Valor> \
Valor getAtributo(const Id##Membro a_m1, const Atributo a_atributo, const Valor a_valor){ \
	try { return vetor##Membro.at(a_m1).getAtributo(a_atributo, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getAtributo(" + getString(a_m1) + "," + getString(a_atributo) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdM2, typename Atributo, typename Valor> \
Valor getAtributo(const Id##Membro a_m1, const IdM2 a_m2, const Atributo a_atributo, const Valor a_valor){ \
	try { return vetor##Membro.at(a_m1).getAtributo(a_m2, a_atributo, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getAtributo(" + getString(a_m1) + "," + getString(a_m2) + "," + getString(a_atributo) +  "): \n" + std::string(erro.what())); } \
}; \
template<typename IdM2, typename IdM3, typename Atributo, typename Valor> \
Valor getAtributo(const Id##Membro a_m1, const IdM2 a_m2, const IdM3 a_m3, const Atributo a_atributo, const Valor a_valor) { \
	try { return vetor##Membro.at(a_m1).getAtributo(a_m2, a_m3, a_atributo, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getAtributo(" + getString(a_m1) + "," + getString(a_m2) + "," + getString(a_m3) + "," + getString(a_atributo) + "): \n" + std::string(erro.what())); } \
}; \
template<typename IdM2, typename IdM3, typename IdM4, typename Atributo, typename Valor> \
Valor getAtributo(const Id##Membro a_m1, const IdM2 a_m2, const IdM3 a_m3, const IdM4 a_m4, const Atributo a_atributo, const Valor a_valor) { \
	try { return vetor##Membro.at(a_m1).getAtributo(a_m2, a_m3, a_m4, a_atributo, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getAtributo(" + getString(a_m1) + "," + getString(a_m2) + "," + getString(a_m3) + "," + getString(a_m4) + "," + getString(a_atributo) + "): \n" + std::string(erro.what())); } \
}; \
bool addDadoAttVetor##Membro(const std::string a_m1, const std::string a_idM1, const std::vector<std::string> &a_iterador, const std::string &a_attVetor, const std::vector<std::string> &a_valor){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			if ((a_iterador.size() == 0) || (a_valor.size() == 0)) \
				throw std::invalid_argument("Faltando informacoes dos argumentos a_iterador e a_valor"); \
			else{ \
				const Id##Membro id##Membro = getId####Membro##FromChar(a_idM1.c_str()); \
				if (!vetor##Membro.isInstanciado(id##Membro)){ \
					Membro objeto##Membro; \
					objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idM1); \
					vetor##Membro.add(objeto##Membro);\
				} \
				vetor##Membro.at(id##Membro).addDadoAttVetor(a_iterador, a_attVetor, a_valor); \
				vetor##Membro.at(id##Membro).validacaoDadosAttVetor(std::vector<std::string>{a_attVetor}); \
				return true; \
			} \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttVetor" + std::string(#Membro) + "(" + a_m1 + "," + a_idM1 + "):\n" + std::string(erro.what())); } \
}; \
bool addDadoAttVetor##Membro##_Membro(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::vector<std::string> &a_iterador, const std::string &a_attVetor, const std::vector<std::string> &a_valor){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			if ((a_iterador.size() == 0) || (a_valor.size() == 0)) \
				throw std::invalid_argument("Faltando informacoes dos argumentos a_iterador e a_valor"); \
			else{ \
				const Id##Membro id##Membro = getId####Membro##FromChar(a_idM1.c_str()); \
				if (!vetor##Membro.isInstanciado(id##Membro)){ \
					Membro objeto##Membro; \
					objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idM1); \
					vetor##Membro.add(objeto##Membro);\
				} \
				vetor##Membro.at(id##Membro).addDadoAttVetorM1(a_m2, a_idM2, a_iterador, a_attVetor, a_valor); \
				return true; \
			} \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadoAttVetor" + std::string(#Membro) + "_Membro("  + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "):\n" + std::string(erro.what())); } \
}; \
bool addDadoAttVetor##Membro##_M2(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::vector<std::string> &a_iterador, const std::string &a_attVetor, const std::vector<std::string> &a_valor){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			if ((a_iterador.size() == 0) || (a_valor.size() == 0)) \
				throw std::invalid_argument("Faltando informacoes dos argumentos a_iterador e a_valor"); \
			else{ \
				const Id##Membro id##Membro = getId####Membro##FromChar(a_idM1.c_str()); \
				if (!vetor##Membro.isInstanciado(id##Membro)){ \
					Membro objeto##Membro; \
					objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idM1); \
					vetor##Membro.add(objeto##Membro);\
				} \
				vetor##Membro.at(id##Membro).addDadoAttVetorM2(a_m2, a_idM2, a_m3, a_idM3, a_iterador, a_attVetor, a_valor); \
				return true; \
			} \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadoAttVetor" + std::string(#Membro) + "_M2("  + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + "):\n" + std::string(erro.what())); } \
}; \
bool addDadoAttVetor##Membro##_M3(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_m4, const std::string a_idM4, const std::vector<std::string> &a_iterador, const std::string &a_attVetor, const std::vector<std::string> &a_valor){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			if ((a_iterador.size() == 0) || (a_valor.size() == 0)) \
				throw std::invalid_argument("Faltando informacoes dos argumentos a_iterador e a_valor"); \
			else{ \
				const Id##Membro id##Membro = getId####Membro##FromChar(a_idM1.c_str()); \
				if (!vetor##Membro.isInstanciado(id##Membro)){ \
					Membro objeto##Membro; \
					objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idM1); \
					vetor##Membro.add(objeto##Membro);\
				} \
				vetor##Membro.at(id##Membro).addDadoAttVetorM3(a_m2, a_idM2, a_m3, a_idM3, a_m4, a_idM4, a_iterador, a_attVetor, a_valor); \
				return true; \
			} \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadoAttVetor" + std::string(#Membro) + "_M3("  + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + "," + a_m4 + "," + a_idM4 + "):\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttVetor##Membro(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			std::vector<std::vector<std::string>> dados; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idM1.c_str()); \
			std::vector<Id##Membro> vetorIdMembro;\
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
						dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttVetor(a_incluirCabecalho, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
						if (a_incluirCabecalho) \
							dados.at(0).insert(dados.at(0).begin(), std::string("id" + std::string(#Membro))); \
					} \
					else\
						dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttVetor(false, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
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
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttVetor" + std::string(#Membro) + "(" + getString(a_incluirCabecalho) + "," + a_m1 + "," + a_idM1 + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_vetorAttVetor):\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttVetor##Membro##_Membro(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			bool primeiroDado = false; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idM1.c_str()); \
			std::vector<Id##Membro> vetorIdMembro; \
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
						std::vector<std::vector<std::string>> dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttVetorM1(a_incluirCabecalho, a_m2, a_idM2, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
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
						std::vector<std::vector<std::string>> dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttVetorM1(false, a_m2, a_idM2, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
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
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttVetor" + std::string(#Membro) + "_Membro("+ getString(a_incluirCabecalho) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_vetorAttVetor):\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttVetor##Membro##_M2(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			bool primeiroDado = false; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idM1.c_str()); \
			std::vector<Id##Membro> vetorIdMembro; \
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
						std::vector<std::vector<std::string>> dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttVetorM2(a_incluirCabecalho, a_m2, a_idM2, a_m3, a_idM3, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
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
						std::vector<std::vector<std::string>> dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttVetorM2(false, a_m2, a_idM2, a_m3, a_idM3, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
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
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttVetor" + std::string(#Membro) + "_M2("+ getString(a_incluirCabecalho) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_vetorAttVetor):\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttVetor##Membro##_M3(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_m4, const std::string a_idM4, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			bool primeiroDado = false; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idM1.c_str()); \
			std::vector<Id##Membro> vetorIdMembro; \
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
						std::vector<std::vector<std::string>> dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttVetorM3(a_incluirCabecalho, a_m2, a_idM2, a_m3, a_idM3, a_m4, a_idM4, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
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
						std::vector<std::vector<std::string>> dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttVetorM3(false, a_m2, a_idM2, a_m3, a_idM3, a_m4, a_idM4, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
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
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttVetor" + std::string(#Membro) + "_M3("+ getString(a_incluirCabecalho) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + "," + a_m4 + "," + a_idM4 + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_vetorAttVetor):\n" + std::string(erro.what())); } \
}; \
Id##Membro getMenorId(const Id##Membro a_m1)const{ \
	try { return vetor##Membro.getMenorId(); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getMenorId(" + getString(a_m1) + "): \n" + std::string(erro.what())); } \
};\
Id##Membro getMaiorId(const Id##Membro a_m1)const{ \
	try { return vetor##Membro.getMaiorId(); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getMaiorId(" + getString(a_m1) + "): \n" + std::string(erro.what())); } \
};\
Id##Membro getIdOut(const Id##Membro a_m1)const{ \
	try { return vetor##Membro.getIdOut(); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIdOut(" + getString(a_m1) + "): \n" + std::string(erro.what())); } \
};\
void incr(Id##Membro &a_m1)const{ \
	try { vetor##Membro.incr(a_m1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::incr(" + getString(a_m1) + "): \n" + std::string(erro.what())); } \
};\
bool isInstanciado(const Id##Membro a_m1)const{ \
	try { return vetor##Membro.isInstanciado(a_m1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::isIstanciado(" + getString(a_m1) + "): \n" + std::string(erro.what())); } \
};\
std::vector<Id##Membro> getIdObjetos(const Id##Membro a_m1)const{ \
	try { return vetor##Membro.getIdObjetos(); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIdObjetos(" + getString(a_m1) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Valor> \
std::vector<Id##Membro> getIdObjetos(const Id##Membro a_m1, const Atributo a_atributo, const Valor a_valor)const{ \
	try { return vetor##Membro.getIdObjetos(a_atributo, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIdObjetos(" + getString(a_m1) + "," + getFullString(a_atributo) + "," + getFullString(a_valor) + "): \n" + std::string(erro.what())); } \
};\
template<typename AttVetor, typename TipoIterador, typename TipoValor> \
SmartEnupla<TipoIterador,TipoValor> getVetor(const Id##Membro a_idM1, const AttVetor a_attVetor, const TipoIterador a_iterador, const TipoValor a_tipoValor){ \
	try { return vetor##Membro.at(a_idM1).getVetor(a_attVetor, a_iterador, a_tipoValor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getVetor(" + getString(a_idM1) + getString(a_attVetor) + ",a_iterador,a_tipoValor): \n" + std::string(erro.what())); } \
};\
int numObjetos(const Membro a_m1){ \
	try { return vetor##Membro.numObjetos(); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::numObjetos(" + getString(a_m1) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo> \
int getSizeVetor(const Id##Membro a_m1, const Atributo a_atributo){ \
	try { return vetor##Membro.at(a_m1).getSizeVetor(a_atributo); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeVetor(" + getString(a_m1) + "," + getString(a_atributo) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador> \
Iterador getIteradorInicial(const Id##Membro a_m1, const Atributo a_atributo, const Iterador a_iterador){ \
	try { return vetor##Membro.at(a_m1).getIteradorInicial(a_atributo, a_iterador); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIteradorInicial(" + getString(a_m1) + "," + getString(a_atributo) + "," + getString(a_iterador) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador> \
Iterador getIteradorFinal(const Id##Membro a_m1, const Atributo a_atributo, const Iterador a_iterador){ \
	try { return vetor##Membro.at(a_m1).getIteradorFinal(a_atributo, a_iterador); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIteradorFinal(" + getString(a_m1) + "," + getString(a_atributo) + "," + getString(a_iterador) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador, typename Valor> \
Valor getElementoVetor(const Id##Membro a_m1, const Atributo a_atributo, const Iterador a_iterador, const Valor a_valor){ \
	try { return vetor##Membro.at(a_m1).getElementoVetor(a_atributo, a_iterador, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getElementoVetor(" + getString(a_m1) + "," + getString(a_atributo) + "," + getString(a_iterador) + ",Valor): \n" + std::string(erro.what())); } \
};\
template<typename IdM2> \
IdM2 getMaiorId(const Id##Membro a_m1, const IdM2 a_m2){ \
	try { return vetor##Membro.at(a_m1).getMaiorId(a_m2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getMaiorId(" + getString(a_m1) + "," + getString(a_m2) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdM2> \
IdM2 getIdOut(const Id##Membro a_m1, const IdM2 a_m2){ \
	try { return vetor##Membro.at(a_m1).getIdOut(a_m2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIdOut(" + getString(a_m1) + "," + getString(a_m2) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdM2> \
void incr(const Id##Membro a_m1, IdM2 &a_m2){ \
	try { vetor##Membro.at(a_m1).incr(a_m2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::incr(" + getString(a_m1) + "," + getString(a_m2) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdM2> \
IdM2 getMenorId(const Id##Membro a_m1, const IdM2 a_m2){ \
	try { return vetor##Membro.at(a_m1).getMenorId(a_m2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getMenorId(" + getString(a_m1) + "," + getString(a_m2) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdM2> \
bool isInstanciado(const Id##Membro a_m1, const IdM2 a_m2){ \
	try { return vetor##Membro.at(a_m1).isInstanciado(a_m2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::isInstanciado(" + getString(a_m1) + "," + getString(a_m2) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdM2> \
std::vector<IdM2> getIdObjetos(const Id##Membro a_m1, const IdM2 a_m2){ \
	try { return vetor##Membro.at(a_m1).getIdObjetos(a_m2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIdObjetos(" + getString(a_m1) + "," + getString(a_m2) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdM2, typename Atributo, typename Valor> \
std::vector<IdM2> getIdObjetos(const Id##Membro a_m1, const IdM2 a_m2, const Atributo a_atributo, const Valor a_valor){ \
	try { return vetor##Membro.at(a_m1).getIdObjetos(a_m2, a_atributo, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIdObjetos(" + getString(a_m1) + "," + getString(a_m2) + "," + getFullString(a_atributo) + "," + getFullString(a_valor) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdM2, typename AttVetor, typename TipoIterador, typename TipoValor> \
SmartEnupla<TipoIterador,TipoValor> getVetor(const Id##Membro a_idM1, const IdM2 a_idM2, const AttVetor a_attVetor, const TipoIterador a_iterador, const TipoValor a_tipoValor){ \
	try { return vetor##Membro.at(a_idM1).getVetor(a_idM2, a_attVetor, a_iterador, a_tipoValor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getVetor(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_attVetor) + ",a_iterador,a_tipoValor): \n" + std::string(erro.what())); } \
};\
template<typename M2> \
int numObjetos(const Id##Membro a_idM1, const M2 a_m2){ \
	try { return vetor##Membro.at(a_idM1).numObjetos(a_m2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::numObjetos(" + getString(a_idM1) + "," + getString(a_m2) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdM2, typename Atributo> \
int getSizeVetor(const Id##Membro a_m1, const IdM2 a_m2, const Atributo a_atributo){ \
	try { return vetor##Membro.at(a_m1).getSizeVetor(a_m2, a_atributo); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeVetor(" + getString(a_m1) + "," + getString(a_m2) + "," + getString(a_atributo) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdM2, typename Atributo, typename Iterador> \
Iterador getIteradorInicial(const Id##Membro a_m1, const IdM2 a_m2, const Atributo a_atributo, const Iterador a_iterador){ \
	try { return vetor##Membro.at(a_m1).getIteradorInicial(a_m2, a_atributo, a_iterador); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIteradorInicial(" + getString(a_m1) + "," + getString(a_m2) + "," + getString(a_atributo) + "," + getString(a_iterador) + "): \n" + std::string(erro.what())); } \
}; \
template<typename IdM2, typename Atributo, typename Iterador> \
Iterador getIteradorFinal(const Id##Membro a_m1, const IdM2 a_m2, const Atributo a_atributo, const Iterador a_iterador){ \
	try { return vetor##Membro.at(a_m1).getIteradorFinal(a_m2, a_atributo, a_iterador); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIteradorFinal(" + getString(a_m1) + "," + getString(a_m2) + "," + getString(a_atributo) + "," + getString(a_iterador) + "): \n" + std::string(erro.what())); } \
}; \
template<typename IdM2, typename Atributo, typename Iterador, typename Valor> \
Valor getElementoVetor(const Id##Membro a_m1, const IdM2 a_m2, const Atributo a_atributo, const Iterador a_iterador, const Valor a_valor){ \
	try { return vetor##Membro.at(a_m1).getElementoVetor(a_m2, a_atributo, a_iterador, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getElementoVetor(" + getString(a_m1) + "," + getString(a_m2) + "," + getString(a_atributo) + "," + getString(a_iterador) + ",Valor): \n" + std::string(erro.what())); } \
}; \
template<typename IdM2, typename IdM3> \
IdM3 getMaiorId(const Id##Membro a_m1, const IdM2 a_m2, const IdM3 a_m3){ \
	try { return vetor##Membro.at(a_m1).getMaiorId(a_m2, a_m3); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getMaiorId(" + getString(a_m1) + "," + getString(a_m2) + "," + getString(a_m3) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdM2, typename IdM3> \
IdM3 getMenorId(const Id##Membro a_m1, const IdM2 a_m2, const IdM3 a_m3){ \
	try { return vetor##Membro.at(a_m1).getMenorId(a_m2, a_m3); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getMenorId(" + getString(a_m1) + "," + getString(a_m2) + "," + getString(a_m3) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdM2, typename IdM3, typename AttVetor, typename TipoIterador, typename TipoValor> \
SmartEnupla<TipoIterador,TipoValor> getVetor(const Id##Membro a_idM1, const IdM2 a_idM2, const IdM3 a_idM3, const AttVetor a_attVetor, const TipoIterador a_iterador, const TipoValor a_tipoValor){ \
	try { return vetor##Membro.at(a_idM1).getVetor(a_idM2, a_idM3, a_attVetor, a_iterador, a_tipoValor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getVetor(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_idM3) + "," + getString(a_attVetor) + ",a_iterador,a_tipoValor): \n" + std::string(erro.what())); } \
};\
template<typename IdM2, typename M3> \
int numObjetos(const Id##Membro a_idM1, const IdM2 a_idM2, const M3 a_m3){ \
	try { return vetor##Membro.at(a_idM1).numObjetos(a_idM2, a_m3); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::numObjetos(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_m3) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdM2, typename IdM3, typename Atributo> \
int getSizeVetor(const Id##Membro a_m1, const IdM2 a_m2, const IdM3 a_m3, const Atributo a_atributo){ \
	try { return vetor##Membro.at(a_m1).getSizeVetor(a_m2, a_m3, a_atributo); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeVetor(" + getString(a_m1) + "," + getString(a_m2) + "," + getString(a_m3) + "," + getString(a_atributo) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdM2, typename IdM3, typename Atributo, typename Iterador> \
Iterador getIteradorInicial(const Id##Membro a_m1, const IdM2 a_m2, const IdM3 a_m3, const Atributo a_atributo, const Iterador a_iterador) { \
	try { return vetor##Membro.at(a_m1).getIteradorInicial(a_m2, a_m3, a_atributo, a_iterador); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIteradorInicial(" + getString(a_m1) + "," + getString(a_m2) + "," + getString(a_m3) + "," + getString(a_atributo) + "," + getString(a_iterador) + "): \n" + std::string(erro.what())); } \
}; \
template<typename IdM2, typename IdM3, typename Atributo, typename Iterador> \
Iterador getIteradorFinal(const Id##Membro a_m1, const IdM2 a_m2, const IdM3 a_m3, const Atributo a_atributo, const Iterador a_iterador) { \
	try { return vetor##Membro.at(a_m1).getIteradorFinal(a_m2, a_m3, a_atributo, a_iterador); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIteradorFinal(" + getString(a_m1) + "," + getString(a_m2) + "," + getString(a_m3) + "," + getString(a_atributo) + "," + getString(a_iterador) + "): \n" + std::string(erro.what())); } \
}; \
template<typename IdM2, typename IdM3, typename Atributo, typename Iterador, typename Valor> \
Valor getElementoVetor(const Id##Membro a_m1, const IdM2 a_m2, const IdM3 a_m3, const Atributo a_atributo, const Iterador a_iterador, const Valor a_valor) { \
	try { return vetor##Membro.at(a_m1).getElementoVetor(a_m2, a_m3, a_atributo, a_iterador, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getElementoVetor(" + getString(a_m1) + "," + getString(a_m2) + "," + getString(a_m3) + "," + getString(a_atributo) + "," + getString(a_iterador) + ",Valor): \n" + std::string(erro.what())); } \
}; \
template<typename Id##M2, typename IdM3, typename IdM4> \
IdM4 getMaiorId(const Id##Membro a_m1, const Id##M2 a_m2, const IdM3 a_m3, const IdM4 a_m4){ \
	try { return vetor##Membro.at(a_m1).getMaiorId(a_m2, a_m3, a_m4); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getMaiorId(" + getString(a_m1) + "," + getString(a_m2) + "," + getString(a_m3) + "," + getString(a_m4) + "): \n" + std::string(erro.what())); } \
};\
template<typename Id##M2, typename IdM3, typename IdM4> \
IdM4 getMenorId(const Id##Membro a_m1, const Id##M2 a_m2, const IdM3 a_m3, const IdM4 a_m4){ \
	try { return vetor##Membro.at(a_m1).getMenorId(a_m2, a_m3, a_m4); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getMenorId(" + getString(a_m1) + "," + getString(a_m2) + "," + getString(a_m3) + "," + getString(a_m4) + "): \n" + std::string(erro.what())); } \
};\
template<typename IdM2, typename IdM3, typename IdM4, typename AttVetor, typename TipoIterador, typename TipoValor> \
SmartEnupla<TipoIterador,TipoValor> getVetor(const Id##Membro a_idM1, const IdM2 a_idM2, const IdM3 a_idM3, const IdM4 a_idM4, const AttVetor a_attVetor, const TipoIterador a_iterador, const TipoValor a_tipoValor){ \
	try { return vetor##Membro.at(a_idM1).getVetor(a_idM2, a_idM3, a_idM4, a_attVetor, a_iterador, a_tipoValor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getVetor(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_idM3) + "," + getString(a_idM4) + "," + getString(a_attVetor) + ",a_iterador,a_tipoValor): \n" + std::string(erro.what())); } \
};\
template<typename IdM2, typename IdM3, typename M4> \
int numObjetos(const Id##Membro a_idM1, const IdM2 a_idM2, const IdM3 a_idM3, const M4 a_m4){ \
	try { return vetor##Membro.at(a_idM1).numObjetos(a_idM2, a_idM3, a_m4); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::numObjetos(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_idM3) + "," + getString(a_m4) + "): \n" + std::string(erro.what())); } \
};\
template<typename Id##M2, typename IdM3, typename IdM4, typename Atributo> \
int getSizeVetor(const Id##Membro a_m1, const IdM2 a_m2, const IdM3 a_m3, const IdM4 a_m4, const Atributo a_atributo){ \
	try { return vetor##Membro.at(a_m1).getSizeVetor(a_m2, a_m3, a_m4, a_atributo); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeVetor(" + getString(a_m1) + "," + getString(a_m2) + "," + getString(a_m3) + "," + getString(a_m4) + "," + getString(a_atributo) + "): \n" + std::string(erro.what())); } \
};\
template<typename Id##M2, typename IdM3, typename IdM4, typename Atributo, typename Iterador> \
Iterador getIteradorInicial(const Id##Membro a_m1, const IdM2 a_m2, const IdM3 a_m3, const IdM4 a_m4, const Atributo a_atributo, const Iterador a_iterador) { \
	try { return vetor##Membro.at(a_m1).getIteradorInicial(a_m2, a_m3, a_m4, a_atributo, a_iterador); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIteradorInicial(" + getString(a_m1) + "," + getString(a_m2) + "," + getString(a_m3) + "," + getString(a_m4) + "," + getString(a_atributo) + "," + getString(a_iterador) + "): \n" + std::string(erro.what())); } \
}; \
template<typename Id##M2, typename IdM3, typename IdM4, typename Atributo, typename Iterador> \
Iterador getIteradorFinal(const Id##Membro a_m1, const IdM2 a_m2, const IdM3 a_m3, const IdM4 a_m4, const Atributo a_atributo, const Iterador a_iterador) { \
	try { return vetor##Membro.at(a_m1).getIteradorFinal(a_m2, a_m3, a_m4, a_atributo, a_iterador); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIteradorFinal(" + getString(a_m1) + "," + getString(a_m2) + "," + getString(a_m3) + "," + getString(a_m4) + "," + getString(a_atributo) + "," + getString(a_iterador) + "): \n" + std::string(erro.what())); } \
}; \
template<typename Id##M2, typename IdM3, typename IdM4, typename Atributo, typename Iterador, typename Valor> \
Valor getElementoVetor(const Id##Membro a_m1, const IdM2 a_m2, const IdM3 a_m3, const IdM4 a_m4, const Atributo a_atributo, const Iterador a_iterador, const Valor a_valor) { \
	try { return vetor##Membro.at(a_m1).getElementoVetor(a_m2, a_m3, a_m4, a_atributo, a_iterador, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getElementoVetor(" + getString(a_m1) + "," + getString(a_m2) + "," + getString(a_m3) + "," + getString(a_m4) + "," + getString(a_atributo) + "," + getString(a_iterador) + ",Valor): \n" + std::string(erro.what())); } \
}; \
bool addDadoAttMatriz##Membro(const std::string a_m1, const std::string a_idM1, const std::vector<std::string> &a_iter1, const std::vector<std::string> &a_iter2, const std::string &a_att, const std::vector<std::vector<std::string>> &a_vlr){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			if ((a_iter1.size() > 0) && (a_iter2.size() > 0) && (a_vlr.size() > 0)){ \
				const Id##Membro id##Membro = getId####Membro##FromChar(a_idM1.c_str()); \
				if (!vetor##Membro.isInstanciado(id##Membro)){ \
					Membro objeto##Membro; \
					objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idM1); \
					vetor##Membro.add(objeto##Membro);\
				} \
				vetor##Membro.at(id##Membro).addDadoAttMatriz(a_iter1, a_iter2, a_att, a_vlr); \
				vetor##Membro.at(id##Membro).validacaoDadosAttMatriz(std::vector<std::string>{a_att}); \
				return true; \
			} \
			else \
				throw std::invalid_argument("Faltando informacoes dos argumentos a_iter1, a_iter2 e a_vlr"); \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttMatriz" + std::string(#Membro) + "():\n" + std::string(erro.what())); } \
}; \
bool addDadoAttMatriz##Membro##_Membro(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::vector<std::string> &a_iter1, const std::vector<std::string> &a_iter2, const std::string &a_att, const std::vector<std::vector<std::string>> &a_vlr){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			const Id##Membro id##Membro = getId####Membro##FromChar(a_idM1.c_str()); \
			if (!vetor##Membro.isInstanciado(id##Membro)){ \
				Membro objeto##Membro; \
				objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idM1); \
				vetor##Membro.add(objeto##Membro);\
			} \
			vetor##Membro.at(id##Membro).addDadoAttMatrizM1(a_m2, a_idM2, a_iter1, a_iter2, a_att, a_vlr); \
			return true; \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadoAttMatriz" + std::string(#Membro) + "_Membro():\n" + std::string(erro.what())); } \
}; \
bool addDadoAttMatriz##Membro##_M2(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::vector<std::string> &a_iter1, const std::vector<std::string> &a_iter2, const std::string &a_att, const std::vector<std::vector<std::string>> &a_vlr){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			const Id##Membro id##Membro = getId####Membro##FromChar(a_idM1.c_str()); \
			if (!vetor##Membro.isInstanciado(id##Membro)){ \
				Membro objeto##Membro; \
				objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idM1); \
				vetor##Membro.add(objeto##Membro);\
			} \
			vetor##Membro.at(id##Membro).addDadoAttMatrizM2(a_m2, a_idM2, a_m3, a_idM3, a_iter1, a_iter2, a_att, a_vlr); \
			return true; \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadoAttMatriz" + std::string(#Membro) + "_M2():\n" + std::string(erro.what())); } \
}; \
bool addDadoAttMatriz##Membro##_M3(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_m4, const std::string a_idM4, const std::vector<std::string> &a_iter1, const std::vector<std::string> &a_iter2, const std::string &a_att, const std::vector<std::vector<std::string>> &a_vlr){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			const Id##Membro id##Membro = getId####Membro##FromChar(a_idM1.c_str()); \
			if (!vetor##Membro.isInstanciado(id##Membro)){ \
				Membro objeto##Membro; \
				objeto##Membro.setAtributoFromString(AttComum##Membro##_id##Membro, a_idM1); \
				vetor##Membro.add(objeto##Membro);\
			} \
			vetor##Membro.at(id##Membro).addDadoAttMatrizM3(a_m2, a_idM2, a_m3, a_idM3, a_m4, a_idM4, a_iter1, a_iter2, a_att, a_vlr); \
			return true; \
		} \
		else \
			return false; \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadoAttMatriz" + std::string(#Membro) + "_M3():\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttMatriz##Membro(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			std::vector<std::vector<std::string>> dados; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idM1.c_str()); \
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
						dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttMatriz(a_incluirCabecalho, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
						if (a_incluirCabecalho) \
							dados.at(0).insert(dados.at(0).begin(), std::string("id" + std::string(#Membro))); \
					} \
					else\
						dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttMatriz(false, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
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
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttMatriz" + std::string(#Membro) + "(" + getString(a_incluirCabecalho) + "," + a_m1 + "," + a_idM1 + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + "," + getString(a_incluirAtributo) + ",a_vetorAttMatriz):\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttMatriz##Membro##_Membro(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			bool primeiroDado = false; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idM1.c_str()); \
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
						std::vector<std::vector<std::string>> dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttMatrizM1(a_incluirCabecalho, a_m2, a_idM2, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
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
						std::vector<std::vector<std::string>> dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttMatrizM1(false, a_m2, a_idM2, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
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
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttMatriz" + std::string(#Membro) + "_Membro("+ getString(a_incluirCabecalho) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + "," + getString(a_incluirAtributo) + ",a_vetorAttMatriz):\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttMatriz##Membro##_M2(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			bool primeiroDado = false; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idM1.c_str()); \
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
						std::vector<std::vector<std::string>> dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttMatrizM2(a_incluirCabecalho, a_m2, a_idM2, a_m3, a_idM3, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
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
						std::vector<std::vector<std::string>> dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttMatrizM2(false, a_m2, a_idM2, a_m3, a_idM3, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
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
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttMatriz" + std::string(#Membro) + "_M2("+ getString(a_incluirCabecalho) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + "," + getString(a_incluirAtributo) + ",a_vetorAttMatriz):\n" + std::string(erro.what())); } \
}; \
std::vector<std::vector<std::string>> getDadosAttMatriz##Membro##_M3(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_m4, const std::string a_idM4, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz){ \
	try{ \
		if (strCompara(a_m1, std::string(#Membro))){ \
			bool primeiroDado = false; \
			std::vector<std::vector<std::string>> matrizRetorno; \
			const Id##Membro idMembro  = getId##Membro##FromChar(a_idM1.c_str()); \
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
						std::vector<std::vector<std::string>> dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttMatrizM3(a_incluirCabecalho, a_m2, a_idM2, a_m3, a_idM3, a_m4, a_idM4, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
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
						std::vector<std::vector<std::string>> dados = vetor##Membro.at(vetorIdMembro.at(i)).getDadosAttMatrizM3(false, a_m2, a_idM2, a_m3, a_idM3, a_m4, a_idM4, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
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
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttMatriz" + std::string(#Membro) + "_M3("+ getString(a_incluirCabecalho) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + "," + a_m4 + "," + a_idM4 + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + "," + getString(a_incluirAtributo) + ",a_vetorAttMatriz):\n" + std::string(erro.what())); } \
}; \
template<typename Atributo, typename Iterador1> \
Iterador1 getIterador1Inicial(const Id##Membro a_idM1, const Atributo a_atributo, const Iterador1 a_iterador1){ \
	try { return vetor##Membro.at(a_idM1).getIterador1Inicial(a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador1Inicial(" + getString(a_idM1) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1> \
Iterador1 getIterador1Final(const Id##Membro a_idM1, const Atributo a_atributo, const Iterador1 a_iterador1){ \
	try { return vetor##Membro.at(a_idM1).getIterador1Final(a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador1Final(" + getString(a_idM1) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename IdM2> \
Iterador1 getIterador1Inicial(const Id##Membro a_idM1, const IdM2 a_idM2, const Atributo a_atributo, const Iterador1 a_iterador1){ \
	try { return vetor##Membro.at(a_idM1).getIterador1Inicial(a_idM2, a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador1Inicial(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename IdM2> \
Iterador1 getIterador1Final(const Id##Membro a_idM1, const IdM2 a_idM2, const Atributo a_atributo, const Iterador1 a_iterador1){ \
	try { return vetor##Membro.at(a_idM1).getIterador1Final(a_idM2, a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador1Final(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename IdM2, typename IdM3> \
Iterador1 getIterador1Inicial(const Id##Membro a_idM1, const IdM2 a_idM2, const IdM3 a_idM3, const Atributo a_atributo, const Iterador1 a_iterador1){ \
	try { return vetor##Membro.at(a_idM1).getIterador1Inicial(a_idM2, a_idM3, a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador1Inicial(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_idM3) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename IdM2, typename IdM3> \
Iterador1 getIterador1Final(const Id##Membro a_idM1, const IdM2 a_idM2, const IdM3 a_idM3, const Atributo a_atributo, const Iterador1 a_iterador1){ \
	try { return vetor##Membro.at(a_idM1).getIterador1Final(a_idM2, a_idM3, a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador1Final(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_idM3) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename IdM2, typename IdM3, typename IdM4> \
Iterador1 getIterador1Inicial(const Id##Membro a_idM1, const IdM2 a_idM2, const IdM3 a_idM3, const IdM4 a_idM4, const Atributo a_atributo, const Iterador1 a_iterador1){ \
	try { return vetor##Membro.at(a_idM1).getIterador1Inicial(a_idM2, a_idM3, a_idM4, a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador1Inicial(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_idM3) + "," + getString(a_idM4) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename IdM2, typename IdM3, typename IdM4> \
Iterador1 getIterador1Final(const Id##Membro a_idM1, const IdM2 a_idM2, const IdM3 a_idM3, const IdM4 a_idM4, const Atributo a_atributo, const Iterador1 a_iterador1){ \
	try { return vetor##Membro.at(a_idM1).getIterador1Final(a_idM2, a_idM3, a_idM4, a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador1Final(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_idM3) + "," + getString(a_idM4) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2> \
Iterador2 getIterador2Inicial(const Id##Membro a_idM1, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2) { \
	try { return vetor##Membro.at(a_idM1).getIterador2Inicial(a_atributo, a_iterador1, a_iterador2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador2Inicial(" + getString(a_idM1) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2> \
Iterador2 getIterador2Final(const Id##Membro a_idM1, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2){ \
	try { return vetor##Membro.at(a_idM1).getIterador2Final(a_atributo, a_iterador1, a_iterador2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador2Final(" + getString(a_idM1) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2, typename IdM2> \
Iterador2 getIterador2Inicial(const Id##Membro a_idM1, const IdM2 a_idM2, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2){ \
	try { return vetor##Membro.at(a_idM1).getIterador2Inicial(a_idM2, a_atributo, a_iterador1, a_iterador2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador2Inicial(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2, typename IdM2> \
Iterador2 getIterador2Final(const Id##Membro a_idM1, const IdM2 a_idM2, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2){ \
	try { return vetor##Membro.at(a_idM1).getIterador2Final(a_idM2, a_atributo, a_iterador1, a_iterador2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador2Final(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2, typename IdM2, typename IdM3> \
Iterador2 getIterador2Inicial(const Id##Membro a_idM1, const IdM2 a_idM2, const IdM3 a_idM3, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2){ \
	try { return vetor##Membro.at(a_idM1).getIterador2Inicial(a_idM2, a_idM3, a_atributo, a_iterador1, a_iterador2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador2Inicial(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_idM3) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2, typename IdM2, typename IdM3> \
Iterador2 getIterador2Final(const Id##Membro a_idM1, const IdM2 a_idM2, const IdM3 a_idM3, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2){ \
	try { return vetor##Membro.at(a_idM1).getIterador2Final(a_idM2, a_idM3, a_atributo, a_iterador1, a_iterador2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador2Final(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_idM3) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2, typename IdM2, typename IdM3, typename IdM4> \
Iterador2 getIterador2Inicial(const Id##Membro a_idM1, const IdM2 a_idM2, const IdM3 a_idM3, const IdM4 a_idM4, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2){ \
	try { return vetor##Membro.at(a_idM1).getIterador2Inicial(a_idM2, a_idM3, a_idM4, a_atributo, a_iterador1, a_iterador2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador2Inicial(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_idM3) + "," + getString(a_idM4) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2, typename IdM2, typename IdM3, typename IdM4> \
Iterador2 getIterador2Final(const Id##Membro a_idM1, const IdM2 a_idM2, const IdM3 a_idM3, const IdM4 a_idM4, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2){ \
	try { return vetor##Membro.at(a_idM1).getIterador2Final(a_idM2, a_idM3, a_idM4, a_atributo, a_iterador1, a_iterador2); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getIterador2Final(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_idM3) + "," + getString(a_idM4) + "," + getString(a_atributo) + ",a_iterador1): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2, typename Valor> \
Valor getElementoMatriz(const Id##Membro a_m1, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2, const Valor a_valor){ \
	try { return vetor##Membro.at(a_m1).getElementoMatriz(a_atributo, a_iterador1, a_iterador2, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getElementoMatriz(" + getString(a_m1) + "," + getString(a_atributo) + "," + getString(a_iterador1) + "," + getString(a_iterador2) + ",Valor): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2, typename Valor, typename IdM2> \
Valor getElementoMatriz(const Id##Membro a_idM1, const IdM2 a_idM2, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2, const Valor a_valor){ \
	try { return vetor##Membro.at(a_idM1).getElementoMatriz(a_idM2, a_atributo, a_iterador1, a_iterador2, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getElementoMatriz(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_atributo) + "," + getString(a_iterador1) + "," + getString(a_iterador2) + ",Valor): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2, typename Valor, typename IdM2, typename IdM3> \
Valor getElementoMatriz(const Id##Membro a_idM1, const IdM2 a_idM2, const IdM3 a_idM3, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2, const Valor a_valor){ \
	try { return vetor##Membro.at(a_idM1).getElementoMatriz(a_idM2, a_idM3, a_atributo, a_iterador1, a_iterador2, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getElementoMatriz(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_idM3) + "," + getString(a_atributo) + "," + getString(a_iterador1) + "," + getString(a_iterador2) + ",Valor): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename Iterador2, typename Valor, typename IdM2, typename IdM3, typename IdM4> \
Valor getElementoMatriz(const Id##Membro a_idM1, const IdM2 a_idM2, const IdM3 a_idM3, const IdM4 a_idM4, const Atributo a_atributo, const Iterador1 a_iterador1, const Iterador2 a_iterador2, const Valor a_valor){ \
	try { return vetor##Membro.at(a_idM1).getElementoMatriz(a_idM2, a_idM3, a_idM4, a_atributo, a_iterador1, a_iterador2, a_valor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getElementoMatriz(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_idM3) + "," + getString(a_idM4) + "," + getString(a_atributo) + "," + getString(a_iterador1) + "," + getString(a_iterador2) + ",Valor): \n" + std::string(erro.what())); } \
};\
template<typename Atributo> \
int getSize1Matriz(const Id##Membro a_idM1, const Atributo a_atributo){ \
	try { return vetor##Membro.at(a_idM1).getSizeMatriz(a_atributo); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeMatriz(" + getString(a_idM1) + "," + getString(a_atributo) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename IdM2> \
int getSize1Matriz(const Id##Membro a_idM1, const IdM2 a_idM2, const Atributo a_atributo){ \
	try { return vetor##Membro.at(a_idM1).getSize1Matriz(a_idM2, a_atributo); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeMatriz(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_atributo) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename IdM2, typename IdM3> \
int getSize1Matriz(const Id##Membro a_idM1, const IdM2 a_idM2, const IdM3 a_idM3, const Atributo a_atributo){ \
	try { return vetor##Membro.at(a_idM1).getSize1Matriz(a_idM2, a_idM3, a_atributo); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeMatriz(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_idM3) + "," + getString(a_atributo) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename IdM2, typename IdM3, typename IdM4> \
int getSize1Matriz(const Id##Membro a_idM1, const IdM2 a_idM2, const IdM3 a_idM3, const IdM4 a_idM4, const Atributo a_atributo){ \
	try { return vetor##Membro.at(a_idM1).getSize1Matriz(a_idM2, a_idM3, a_idM4, a_atributo); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeMatriz(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_idM3) + "," + getString(a_idM4) + "," + getString(a_atributo) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1> \
int getSize2Matriz(const Id##Membro a_idM1, const Atributo a_atributo, Iterador1 a_iterador1){ \
	try { return vetor##Membro.at(a_idM1).getSizeMatriz(a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeMatriz(" + getString(a_idM1) + "," + getString(a_atributo) + "," + getString(a_iterador1) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename IdM2> \
int getSize2Matriz(const Id##Membro a_idM1, const IdM2 a_idM2, const Atributo a_atributo, Iterador1 a_iterador1){ \
	try { return vetor##Membro.at(a_idM1).getSize2Matriz(a_idM2, a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeMatriz(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_atributo) + "," + getString(a_iterador1) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename IdM2, typename IdM3> \
int getSize2Matriz(const Id##Membro a_idM1, const IdM2 a_idM2, const IdM3 a_idM3, const Atributo a_atributo, Iterador1 a_iterador1){ \
	try { return vetor##Membro.at(a_idM1).getSize2Matriz(a_idM2, a_idM3, a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeMatriz(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_idM3) + "," + getString(a_atributo) + "," + getString(a_iterador1) + "): \n" + std::string(erro.what())); } \
};\
template<typename Atributo, typename Iterador1, typename IdM2, typename IdM3, typename IdM4> \
int getSize2Matriz(const Id##Membro a_idM1, const IdM2 a_idM2, const IdM3 a_idM3, const IdM4 a_idM4, const Atributo a_atributo, Iterador1 a_iterador1){ \
	try { return vetor##Membro.at(a_idM1).getSize2Matriz(a_idM2, a_idM3, a_idM4, a_atributo, a_iterador1); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getSizeMatriz(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_idM3) + "," + getString(a_idM4) + "," + getString(a_atributo) + "," + getString(a_iterador1) + "): \n" + std::string(erro.what())); } \
};\
template<typename AttMatriz, typename TipoIterador1, typename TipoIterador2, typename TipoValor> \
SmartEnupla<TipoIterador2, TipoValor> getElementosMatriz(const Id##Membro a_idM1, const AttMatriz a_attMatriz, const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2, const TipoValor a_tipoValor){ \
	try { return vetor##Membro.at(a_idM1).getElementosMatriz(a_attMatriz, a_iterador1, a_iterador2, a_tipoValor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getElementosMatriz(" + getString(a_idM1) + "," + getString(a_attMatriz) + ",a_iterador1,a_iterador2,a_tipoValor): \n" + std::string(erro.what())); } \
};\
template<typename IdM2, typename AttMatriz, typename TipoIterador1, typename TipoIterador2, typename TipoValor> \
SmartEnupla<TipoIterador2, TipoValor> getElementosMatriz(const Id##Membro a_idM1, const IdM2 a_idM2, const AttMatriz a_attMatriz, const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2, const TipoValor a_tipoValor){ \
	try { return vetor##Membro.at(a_idM1).getElementosMatriz(a_idM2, a_attMatriz, a_iterador1, a_iterador2, a_tipoValor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getElementosMatriz(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_attMatriz) + ",a_iterador1,a_iterador2,a_tipoValor): \n" + std::string(erro.what())); } \
};\
template<typename IdM3, typename IdM2, typename AttMatriz, typename TipoIterador1, typename TipoIterador2, typename TipoValor> \
SmartEnupla<TipoIterador2, TipoValor> getElementosMatriz(const Id##Membro a_idM1, const IdM2 a_idM2, const IdM3 a_idM3, const AttMatriz a_attMatriz, const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2, const TipoValor a_tipoValor){ \
	try { return vetor##Membro.at(a_idM1).getElementosMatriz(a_idM2, a_idM3, a_attMatriz, a_iterador1, a_iterador2, a_tipoValor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getElementosMatriz(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_idM3) + "," + getString(a_attMatriz) + ",a_iterador1,a_iterador2,a_tipoValor): \n" + std::string(erro.what())); } \
};\
template<typename AttMatriz, typename TipoIterador1, typename TipoIterador2, typename TipoValor> \
SmartEnupla<TipoIterador1, SmartEnupla<TipoIterador2, TipoValor>> getMatriz(const Id##Membro a_idM1, const AttMatriz a_attMatriz, const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2, const TipoValor a_tipoValor){ \
	try { return vetor##Membro.at(a_idM1).getMatriz(a_attMatriz, a_iterador1, a_iterador2, a_tipoValor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getMatriz(" + getString(a_idM1) + "," + getString(a_attMatriz) + ",a_iterador1,a_iterador2,a_tipoValor): \n" + std::string(erro.what())); } \
};\
template<typename IdM2, typename AttMatriz, typename TipoIterador1, typename TipoIterador2, typename TipoValor> \
SmartEnupla<TipoIterador1, SmartEnupla<TipoIterador2, TipoValor>> getMatriz(const Id##Membro a_idM1, const IdM2 a_idM2, const AttMatriz a_attMatriz, const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2, const TipoValor a_tipoValor){ \
	try { return vetor##Membro.at(a_idM1).getMatriz(a_idM2, a_attMatriz, a_iterador1, a_iterador2, a_tipoValor); } \
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getMatriz(" + getString(a_idM1) + "," + getString(a_idM2) + "," + getString(a_attMatriz) + ",a_iterador1,a_iterador2,a_tipoValor): \n" + std::string(erro.what())); } \
};\

//
// AttComum
//

#define DEFINE_addDadosAttComumMembro(Classe, Membro)\
if (addDadosAttComum##Membro(a_m1, a_idM1, a_vetorAttComuns, a_vetorValores)) \
	return; 

#define DEFINE_addDadosAttComumM2(Classe, Membro)\
if (addDadosAttComum##Membro##_Membro(a_m1, a_idM1, a_m2, a_idM2, a_vetorAttComuns, a_vetorValores)) \
	return; 

#define DEFINE_addDadosAttComumM3(Classe, Membro)\
if (addDadosAttComum##Membro##_M2(a_m1, a_idM1, a_m2, a_idM2, a_m3, a_idM3, a_vetorAttComuns, a_vetorValores)) \
	return; 

#define DEFINE_addDadosAttComumM4(Classe, Membro)\
if (addDadosAttComum##Membro##_M3(a_m1, a_idM1, a_m2, a_idM2, a_m3, a_idM3, a_m4, a_idM4, a_vetorAttComuns, a_vetorValores)) \
	return; 

#define DEFINE_getDadosAttComumMembro(Classe, Membro)\
matrizRetorno = getDadosAttComum##Membro(a_incluirCabecalho, a_m1, a_idM1, a_vetorAttComum); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno; 

#define DEFINE_getDadosAttComumM2(Classe, Membro)\
matrizRetorno = getDadosAttComum##Membro##_Membro(a_incluirCabecalho, a_m1, a_idM1, a_m2, a_idM2, a_vetorAttComum); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno; 

#define DEFINE_getDadosAttComumM3(Classe, Membro)\
matrizRetorno = getDadosAttComum##Membro##_M2(a_incluirCabecalho, a_m1, a_idM1, a_m2, a_idM2, a_m3, a_idM3, a_vetorAttComum); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno; 

#define DEFINE_getDadosAttComumM4(Classe, Membro)\
matrizRetorno = getDadosAttComum##Membro##_M3(a_incluirCabecalho, a_m1, a_idM1, a_m2, a_idM2, a_m3, a_idM3, a_m4, a_idM4, a_vetorAttComum); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno; 

//
// AttVetor
//

#define DEFINE2_addDadoAttVetorM1(Classe, Membro)\
if (addDadoAttVetor##Membro(a_m1, a_idM1, a_iterador, a_attVetor, a_valor)) \
	return; 

#define DEFINE2_addDadoAttVetorM2(Classe, Membro)\
if (addDadoAttVetor##Membro##_Membro(a_m1, a_idM1, a_m2, a_idM2, a_iterador, a_attVetor, a_valor)) \
	return; 

#define DEFINE2_addDadoAttVetorM3(Classe, Membro)\
if (addDadoAttVetor##Membro##_M2(a_m1, a_idM1, a_m2, a_idM2, a_m3, a_idM3, a_iterador, a_attVetor, a_valor)) \
	return; 

#define DEFINE2_addDadoAttVetorM4(Classe, Membro)\
if (addDadoAttVetor##Membro##_M3(a_m1, a_idM1, a_m2, a_idM2, a_m3, a_idM3, a_m4, a_idM4, a_iterador, a_attVetor, a_valor)) \
	return; 

#define DEFINE_getDadosAttVetorM1(Classe, Membro)\
matrizRetorno = getDadosAttVetor##Membro(a_incluirCabecalho, a_m1, a_idM1, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno;

#define DEFINE_getDadosAttVetorM2(Classe, Membro)\
matrizRetorno = getDadosAttVetor##Membro##_Membro(a_incluirCabecalho, a_m1, a_idM1, a_m2, a_idM2, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno;

#define DEFINE_getDadosAttVetorM3(Classe, Membro)\
matrizRetorno = getDadosAttVetor##Membro##_M2(a_incluirCabecalho, a_m1, a_idM1, a_m2, a_idM2, a_m3, a_idM3, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno;

#define DEFINE_getDadosAttVetorM4(Classe, Membro)\
matrizRetorno = getDadosAttVetor##Membro##_M3(a_incluirCabecalho, a_m1, a_idM1, a_m2, a_idM2, a_m3, a_idM3, a_m4, a_idM4, a_iteradorInicial, a_iteradorFinal, a_incluirAtributo, a_vetorAttVetor); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno;

//
// AttMatriz
//

#define DEFINE2_addDadoAttMatrizM1(Classe, Membro)\
if (addDadoAttMatriz##Membro(a_m1, a_idM1, a_iter1, a_iter2, a_att, a_vlr)) \
	return; 

#define DEFINE2_addDadoAttMatrizM2(Classe, Membro)\
if (addDadoAttMatriz##Membro##_Membro(a_m1, a_idM1, a_m2, a_idM2, a_iter1, a_iter2, a_att, a_vlr)) \
	return; 

#define DEFINE2_addDadoAttMatrizM3(Classe, Membro)\
if (addDadoAttMatriz##Membro##_M2(a_m1, a_idM1, a_m2, a_idM2, a_m3, a_idM3, a_iter1, a_iter2, a_att, a_vlr)) \
	return; 

#define DEFINE2_addDadoAttMatrizM4(Classe, Membro)\
if (addDadoAttMatriz##Membro##_M3(a_m1, a_idM1, a_m2, a_idM2, a_m3, a_idM3, a_m4, a_idM4, a_iter1, a_iter2, a_att, a_vlr)) \
	return; 

#define DEFINE_getDadosAttMatrizM1(Classe, Membro)\
matrizRetorno = getDadosAttMatriz##Membro(a_incluirCabecalho, a_m1, a_idM1, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno;

#define DEFINE_getDadosAttMatrizM2(Classe, Membro)\
matrizRetorno = getDadosAttMatriz##Membro##_Membro(a_incluirCabecalho, a_m1, a_idM1, a_m2, a_idM2, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno;

#define DEFINE_getDadosAttMatrizM3(Classe, Membro)\
matrizRetorno = getDadosAttMatriz##Membro##_M2(a_incluirCabecalho, a_m1, a_idM1, a_m2, a_idM2, a_m3, a_idM3, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno;

#define DEFINE_getDadosAttMatrizM4(Classe, Membro)\
matrizRetorno = getDadosAttMatriz##Membro##_M3(a_incluirCabecalho, a_m1, a_idM1, a_m2, a_idM2, a_m3, a_idM3, a_m4, a_idM4, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, a_incluirAtributo, a_vetorAttMatriz); \
if (matrizRetorno.size() > 0) \
	return matrizRetorno;

#define DECLARA_METODOS_Membro(Classe, SmartElemento, valores)\
valores(DEFINE_METODOS_MEMBROS) \
virtual void addDadosAttComumM1(const std::string a_m1, const std::string a_idM1, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores){ \
	try{ \
		valores(DEFINE_addDadosAttComumMembro) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttComumM1:(" + a_m1 + "," + a_idM1 + ",a_vetorAttComuns,a_vetorValores):\n" + std::string(erro.what())); }\
}; \
virtual void addDadosAttComumM2(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores) {\
	try { \
		valores(DEFINE_addDadosAttComumM2) \
		throw std::invalid_argument("Membro ou Membro Filho nao encontrados."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttComumM2:(" + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + ",a_vetorAttComuns,a_vetorValores):\n" + std::string(erro.what())); }\
}; \
virtual void addDadosAttComumM3(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores) {\
	try { \
		valores(DEFINE_addDadosAttComumM3) \
		throw std::invalid_argument("Membro ou Membro Filho nao encontrados."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttComumM3:(" + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + ",a_vetorAttComuns,a_vetorValores):\n" + std::string(erro.what())); }\
}; \
virtual void addDadosAttComumM4(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_m4, const std::string a_idM4, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores) {\
	try { \
		valores(DEFINE_addDadosAttComumM4) \
		throw std::invalid_argument("Membro ou Membro Filho nao encontrados."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttComumM4:(" + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + "," + a_m4 + "," + a_idM4 + ",a_vetorAttComuns,a_vetorValores):\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttComumM1(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::vector<std::string> a_vetorAttComum){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttComumMembro) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttComumM1:(" + getString(a_incluirCabecalho) + "," + a_m1 + "," + a_idM1 + ",a_vetorAttComum):\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttComumM2(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::vector<std::string> a_vetorAttComum){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttComumM2) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttComumM2:(" + getString(a_incluirCabecalho) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + ",a_vetorAttComum):\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttComumM3(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::vector<std::string> a_vetorAttComum){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttComumM3) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttComumM3:(" + getString(a_incluirCabecalho) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + ",a_vetorAttComum):\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttComumM4(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_m4, const std::string a_idM4, const std::vector<std::string> a_vetorAttComum){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttComumM4) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttComumM4:(" + getString(a_incluirCabecalho) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + "," + a_m4 + "," + a_idM4 + ",a_vetorAttComum):\n" + std::string(erro.what())); }\
}; \
virtual void addDadoAttVetorM1(const std::string a_m1, const std::string a_idM1, const std::vector<std::string> &a_iterador, const std::string &a_attVetor, const std::vector<std::string> &a_valor){ \
	try{ \
		valores(DEFINE2_addDadoAttVetorM1) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttVetorM1:(" + a_m1 + "," + a_idM1 + "):\n" + std::string(erro.what())); }\
}; \
virtual void addDadoAttVetorM2(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::vector<std::string> &a_iterador, const std::string &a_attVetor, const std::vector<std::string> &a_valor) {\
	try { \
		valores(DEFINE2_addDadoAttVetorM2) \
		throw std::invalid_argument("Membro ou Membro Filho nao encontrados."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttVetorM2:(" + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "):\n" + std::string(erro.what())); }\
}; \
virtual void addDadoAttVetorM3(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::vector<std::string> &a_iterador, const std::string &a_attVetor, const std::vector<std::string> &a_valor) {\
	try { \
		valores(DEFINE2_addDadoAttVetorM3) \
		throw std::invalid_argument("Membro ou Membro Filho nao encontrados."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttVetorM3:(" + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + "):\n" + std::string(erro.what())); }\
}; \
virtual void addDadoAttVetorM4(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_m4, const std::string a_idM4, const std::vector<std::string> &a_iterador, const std::string &a_attVetor, const std::vector<std::string> &a_valor) {\
	try { \
		valores(DEFINE2_addDadoAttVetorM4) \
		throw std::invalid_argument("Membro ou Membro Filho nao encontrados."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadoAttVetorM4:(" + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + "," + a_m4 + "," + a_idM4 + "):\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttVetorM1(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttVetorM1) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttVetorM1:(" + getString(a_incluirCabecalho) + "," + a_m1 + "," + a_idM1 + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_vetorAttVetor):\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttVetorM2(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttVetorM2) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttVetorM2:(" + getString(a_incluirCabecalho) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_vetorAttVetor):\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttVetorM3(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttVetorM3) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttVetorM2:(" + getString(a_incluirCabecalho) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_vetorAttVetor):\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttVetorM4(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_m4, const std::string a_idM4, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttVetorM4) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttVetorM4:(" + getString(a_incluirCabecalho) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + "," + a_m4 + "," + a_idM4 + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_vetorAttVetor):\n" + std::string(erro.what())); }\
}; \
virtual void addDadoAttMatrizM1(const std::string a_m1, const std::string a_idM1, const std::vector<std::string>& a_iter1, const std::vector<std::string>& a_iter2, const std::string& a_att, const std::vector<std::vector<std::string>>& a_vlr){ \
	try{ \
		valores(DEFINE2_addDadoAttMatrizM1) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttMatrizM1:():\n" + std::string(erro.what())); }\
}; \
virtual void addDadoAttMatrizM2(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::vector<std::string>& a_iter1, const std::vector<std::string>& a_iter2, const std::string& a_att, const std::vector<std::vector<std::string>>& a_vlr) {\
	try { \
		valores(DEFINE2_addDadoAttMatrizM2) \
		throw std::invalid_argument("Membro ou Membro Filho nao encontrados."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttMatrizM2:():\n" + std::string(erro.what())); }\
}; \
virtual void addDadoAttMatrizM3(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::vector<std::string>& a_iter1, const std::vector<std::string>& a_iter2, const std::string& a_att, const std::vector<std::vector<std::string>>& a_vlr) {\
	try { \
		valores(DEFINE2_addDadoAttMatrizM3) \
		throw std::invalid_argument("Membro ou Membro Filho nao encontrados."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadosAttMatrizM3:():\n" + std::string(erro.what())); }\
}; \
virtual void addDadoAttMatrizM4(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_m4, const std::string a_idM4, const std::vector<std::string>& a_iter1, const std::vector<std::string>& a_iter2, const std::string& a_att, const std::vector<std::vector<std::string>>& a_vlr) {\
	try { \
		valores(DEFINE2_addDadoAttMatrizM4) \
		throw std::invalid_argument("Membro ou Membro Filho nao encontrados."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::addDadoAttMatrizM4:():\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttMatrizM1(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttMatrizM1) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttMatrizM1:(" + getString(a_incluirCabecalho) + "," + a_m1 + "," + a_idM1 + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + ",a_vetorAttMatriz):\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttMatrizM2(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttMatrizM2) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttMatrizM2:(" + getString(a_incluirCabecalho) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + ",a_vetorAttMatriz):\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttMatrizM3(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttMatrizM3) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttMatrizM2:(" + getString(a_incluirCabecalho) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + ",a_vetorAttMatriz):\n" + std::string(erro.what())); }\
}; \
virtual std::vector<std::vector<std::string>> getDadosAttMatrizM4(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_m4, const std::string a_idM4, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz){ \
	try{ \
		std::vector<std::vector<std::string>> matrizRetorno; \
		valores(DEFINE_getDadosAttMatrizM4) \
		throw std::invalid_argument("Membro nao encontrado."); \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument(std::string(#Classe) + "::getDadosAttMatrizM4:(" + getString(a_incluirCabecalho) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + "," + a_m4 + "," + a_idM4 + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + ",a_vetorAttMatriz):\n" + std::string(erro.what())); }\
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