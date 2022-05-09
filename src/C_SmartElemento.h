#ifndef SMART_ELEMENTOS_H
#define SMART_ELEMENTOS_H

#include "C_Auxiliar.h"
#include "C_OperacoesMatematicas.h"
#include "C_SmartEnum.h"
#include "C_Periodo.h"

#include "C_SmartMembro.h"
#include "C_SmartAttComum.h"
#include "C_SmartAttVetor.h"
#include "C_SmartAttMatriz.h"

#include <vector>
#include <string>

enum SmartElementos {
	AttComum,
	AttVetor,
	AttMatriz,
	Membro,
};


class SmartDados {

public:

	SmartDados() {};

	virtual ~SmartDados() {};

	//
	// AttComum
	//

	virtual void addDadosAttComum(const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores) { 
			try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); } 
			catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadosAttComum(&a_vetorAttComuns,&a_vetorValores): \n" + std::string(erro.what())); }
	};
	virtual void addDadosAttComum_teste(const std::vector<std::string>& a_vetorAttComuns, const std::vector<std::string>& a_vetorValores) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception & erro) { throw std::invalid_argument("SmartDados::addDadosAttComum(&a_vetorAttComuns,&a_vetorValores): \n" + std::string(erro.what())); }
	};
	virtual void addDadosAttComum(const std::string a_idMembro, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores) { 
			try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); } 
			catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadosAttComum(" + a_idMembro + "&a_vetorAttComuns,&a_vetorValores): \n" + std::string(erro.what())); }
	};
	virtual void validacaoDadosAttComum() {};

	virtual std::vector<std::vector<std::string>> getDadosAttComum(const bool a_incluirCabecalho) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttComum(a_obterCabecalho): \n" + std::string(erro.what())); }
	};

	virtual std::vector<std::vector<std::string>> getDadosAttComum(const bool a_incluirCabecalho, const std::vector<std::string> &a_vetorAttComum) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttComum(a_vetorAttComum): \n" + std::string(erro.what())); }
	};

	virtual void addDadosAttComumMembro(const std::string a_membro, const std::string a_idMembro,  const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); } 
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadosAttComumMembros(" + a_membro + "," + a_idMembro + ",&a_vetorAttComuns,&a_vetorValores): \n" + std::string(erro.what())); }
	};
	virtual void validacaoDadosAttComumMembro(const std::string a_membro, const std::vector<std::string> &a_vetorIdMembro) {};

	virtual std::vector<std::vector<std::string>> getDadosAttComumMembro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::vector<std::string> a_vetorAttComum) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttComumMembro(" + a_membro + "," + a_idMembro + ",a_vetorAttComum): \n" + std::string(erro.what())); }
	};

	virtual void addDadosAttComumMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores) {
			try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); } 
			catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadosAttComumMembroMembro(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + ",&a_vetorAttComuns,&a_vetorValores): \n" + std::string(erro.what())); }
	};
	virtual void validacaoDadosAttComumMembroMembro(const std::string a_membro, const std::vector<std::string> &a_vetorIdMembro, const std::string a_membroMembro, const std::vector<std::string> &a_vetorIdMembroMembro) {};

	virtual std::vector<std::vector<std::string>> getDadosAttComumMembroMembro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::vector<std::string> a_vetorAttComum) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttComumMembroMembro(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + ",a_vetorAttComum): \n" + std::string(erro.what())); }
	};

	virtual void addDadosAttComumMembroMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores) { 
			try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); } 
			catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadosAttComumMembroMembroMembro(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + ",&a_vetorAttComuns,&a_vetorValores): \n" + std::string(erro.what())); }
	};
	virtual void validacaoDadosAttComumMembroMembroMembro(const std::string a_membro, const std::vector<std::string> &a_vetorIdMembro, const std::string a_membroMembro, const std::vector<std::string> &a_vetorIdMembroMembro, const std::string a_membroMembroMembro, const std::vector<std::string> &a_vetorIdMembroMembroMembro) {};

	virtual std::vector<std::vector<std::string>> getDadosAttComumMembroMembroMembro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::vector<std::string> a_vetorAttComum) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttComumMembroMembroMembro(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + ",a_vetorAttComum): \n" + std::string(erro.what())); }
	};

	virtual void addDadosAttComumMembroMembroMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_membroMembroMembroMembro, const std::string a_idMembroMembroMembroMembro, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadosAttComumMembroMembroMembroMembro(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_membroMembroMembroMembro + "," + a_idMembroMembroMembroMembro + ",&a_vetorAttComuns,&a_vetorValores): \n" + std::string(erro.what())); }
	};
	virtual void validacaoDadosAttComumMembroMembroMembroMembro(const std::string a_membro, const std::vector<std::string> &a_vetorIdMembro, const std::string a_membroMembro, const std::vector<std::string> &a_vetorIdMembroMembro, const std::string a_membroMembroMembro, const std::vector<std::string> &a_vetorIdMembroMembroMembro, const std::string a_membroMembroMembroMembro, const std::vector<std::string> &a_vetorIdMembroMembroMembroMembro) {};

	virtual std::vector<std::vector<std::string>> getDadosAttComumMembroMembroMembroMembro(const bool a_incluirCabecalho, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_membroMembroMembroMembro, const std::string a_idMembroMembroMembroMembro, const std::vector<std::string> a_vetorAttComum) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttComumMembroMembroMembroMembro(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_membroMembroMembroMembro + "," + a_idMembroMembroMembroMembro + ",a_vetorAttComum): \n" + std::string(erro.what())); }
	};

	//
	// AttVetor
	//

	virtual void addDadoAttVetor(const std::string a_iterador, const std::string a_attVetor, const std::string a_valor) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadoAttVetor(" + a_iterador + "," + a_attVetor + "," + a_valor + "): \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttVetor(const std::string a_iterador, const std::string a_attVetor, const std::string a_valor, const int a_alocacao) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception & erro) { throw std::invalid_argument("SmartDados::addDadoAttVetor(" + a_iterador + "," + a_attVetor + "," + a_valor + "," + getFullString(a_alocacao) + "): \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttVetor(const std::string a_idMembro, const std::string a_iterador, const std::string a_attVetor, const std::string a_valor) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadoAttVetor(" + a_idMembro + "," + a_iterador + "," + a_attVetor + "," + a_valor + "): \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttVetor(const std::string a_idMembro, const std::string a_iterador, const std::string a_attVetor, const std::string a_valor, const int a_alocacao) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception & erro) { throw std::invalid_argument("SmartDados::addDadoAttVetor(" + a_idMembro + "," + a_iterador + "," + a_attVetor + "," + a_valor + "," + getFullString(a_alocacao) + "): \n" + std::string(erro.what())); }
	};

	virtual void validacaoDadosAttVetor(const std::vector<std::string> &a_vetorAttVetor) {};

	virtual std::vector<std::vector<std::string>> getDadosAttVetor(const bool a_incluirIteradores, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttVetor(" + getString(a_incluirIteradores) + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_attVetor): \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttVetorMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_iterador, const std::string a_attVetor, const std::string a_valor) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadoAttVetorMembro(" + a_membro + "," + a_idMembro + "," + a_iterador + "," + a_attVetor + "," + a_valor + "): \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttVetorMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_iterador, const std::string a_attVetor, const std::string a_valor, const int a_alocacao) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception & erro) { throw std::invalid_argument("SmartDados::addDadoAttVetorMembro(" + a_membro + "," + a_idMembro + "," + a_iterador + "," + a_attVetor + "," + a_valor + "," + getFullString(a_alocacao) + "): \n" + std::string(erro.what())); }
	};

	virtual void validacaoDadosAttVetorMembro(const std::string a_membro, const std::vector<std::string> &a_vetorIdMembro, const std::vector<std::string> &a_vetorAttVetor) {};

	virtual std::vector<std::vector<std::string>> getDadosAttVetorMembro(const bool a_incluirIteradores, const std::string a_membro, const std::string a_idMembro, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttVetor(" + getString(a_incluirIteradores) + "," + a_membro + "," + a_idMembro + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_attVetor): \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttVetorMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_iterador, const std::string a_attVetor, const std::string a_valor) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadoAttVetorMembroMembro(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_iterador + "," + a_attVetor + "," + a_valor + "): \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttVetorMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_iterador, const std::string a_attVetor, const std::string a_valor, const int a_alocacao) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception & erro) { throw std::invalid_argument("SmartDados::addDadoAttVetorMembroMembro(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_iterador + "," + a_attVetor + "," + a_valor + "," + getFullString(a_alocacao) + "): \n" + std::string(erro.what())); }
	};

	virtual void validacaoDadosAttVetorMembroMembro(const std::string a_membro, const std::vector<std::string> &a_vetorIdMembro, const std::string a_membroMembro, const std::vector<std::string> &a_vetorIdMembroMembro, const std::vector<std::string> &a_vetorAttVetor) {};

	virtual std::vector<std::vector<std::string>> getDadosAttVetorMembroMembro(const bool a_incluirIteradores, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttVetor(" + getString(a_incluirIteradores) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_attVetor): \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttVetorMembroMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_iterador, const std::string a_attVetor, const std::string a_valor) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadoAttVetorMembroMembroMembro(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_iterador + "," + a_attVetor + "," + a_valor + "): \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttVetorMembroMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_iterador, const std::string a_attVetor, const std::string a_valor, const int a_alocacao) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception & erro) { throw std::invalid_argument("SmartDados::addDadoAttVetorMembroMembroMembro(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_iterador + "," + a_attVetor + "," + a_valor + "," + getFullString(a_alocacao) + "): \n" + std::string(erro.what())); }
	};

	virtual void validacaoDadosAttVetorMembroMembroMembro(const std::string a_membro, const std::vector<std::string> &a_vetorIdMembro, const std::string a_membroMembro, const std::vector<std::string> &a_vetorIdMembroMembro, const std::string a_membroMembroMembro, const std::vector<std::string> &a_vetorIdMembroMembroMembro, const std::vector<std::string> &a_vetorAttVetor) {};

	virtual std::vector<std::vector<std::string>> getDadosAttVetorMembroMembroMembro(const bool a_incluirIteradores, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttVetorMembroMembroMembro(" + getString(a_incluirIteradores) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_attVetor): \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttVetorMembroMembroMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_membroMembroMembroMembro, const std::string a_idMembroMembroMembroMembro, const std::string a_iterador, const std::string a_attVetor, const std::string a_valor) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadoAttVetorMembroMembroMembroMembro(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_membroMembroMembroMembro + "," + a_idMembroMembroMembroMembro + "," + a_iterador + "," + a_attVetor + "," + a_valor + "): \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttVetorMembroMembroMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_membroMembroMembroMembro, const std::string a_idMembroMembroMembroMembro, const std::string a_iterador, const std::string a_attVetor, const std::string a_valor, const int a_alocacao) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception & erro) { throw std::invalid_argument("SmartDados::addDadoAttVetorMembroMembroMembroMembro(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_membroMembroMembroMembro + "," + a_idMembroMembroMembroMembro + "," + a_iterador + "," + a_attVetor + "," + a_valor + "," + getFullString(a_alocacao) + "): \n" + std::string(erro.what())); }
	};

	virtual void validacaoDadosAttVetorMembroMembroMembroMembro(const std::string a_membro, const std::vector<std::string> &a_vetorIdMembro, const std::string a_membroMembro, const std::vector<std::string> &a_vetorIdMembroMembro, const std::string a_membroMembroMembro, const std::vector<std::string> &a_vetorIdMembroMembroMembro, const std::string a_membroMembroMembroMembro, const std::vector<std::string> &a_vetorIdMembroMembroMembroMembro, const std::vector<std::string> &a_vetorAttVetor) {};

	virtual std::vector<std::vector<std::string>> getDadosAttVetorMembroMembroMembroMembro(const bool a_incluirIteradores, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_membroMembroMembroMembro, const std::string a_idMembroMembroMembroMembro, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttVetorMembroMembroMembroMembro(" + getString(a_incluirIteradores) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_membroMembroMembroMembro + "," + a_idMembroMembroMembroMembro + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_attVetor): \n" + std::string(erro.what())); }
	};

	//
	// AttMatriz
	//

	virtual void addDadoAttMatriz(const std::string a_iterador1, const std::string a_iterador2, const std::string a_attMatriz, const std::string a_valor) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadoAttMatriz(" + a_iterador1 + "," + a_iterador2 + "," + a_attMatriz + "," + a_valor + "): \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttMatriz(const std::string a_iterador1, const std::string a_iterador2, const std::string a_attMatriz, const std::string a_valor, const int a_alocacao_1, const int a_alocacao_2) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception & erro) { throw std::invalid_argument("SmartDados::addDadoAttMatriz(" + a_iterador1 + "," + a_iterador2 + "," + a_attMatriz + "," + a_valor + "): \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttMatriz(const std::string a_idMembro, const std::string a_iterador1, const std::string a_iterador2, const std::string a_attMatriz, const std::string a_valor) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadoAttMatriz(" + a_idMembro + "," + a_iterador1 + "," + a_iterador2 + "," + a_attMatriz + "," + a_valor + "): \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttMatriz(const std::string a_idMembro, const std::string a_iterador1, const std::string a_iterador2, const std::string a_attMatriz, const std::string a_valor, const int a_alocacao_1, const int a_alocacao_2) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception & erro) { throw std::invalid_argument("SmartDados::addDadoAttMatriz(" + a_idMembro + "," + a_iterador1 + "," + a_iterador2 + "," + a_attMatriz + "," + a_valor + "): \n" + std::string(erro.what())); }
	};

	virtual void validacaoDadosAttMatriz(const std::vector<std::string> &a_vetorAttMatriz) {};

	virtual std::vector<std::vector<std::string>> getDadosAttMatriz(const bool a_incluirIteradores, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz) const{
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttMatriz(" + getString(a_incluirIteradores) + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + "," + getString(a_incluirAtributo) + ",a_attMatriz): \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttMatrizMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_iterador1, const std::string a_iterador2, const std::string a_attMatriz, const std::string a_valor) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadoAttMatrizMembro(" + a_membro + "," + a_idMembro + "," + a_iterador1 + "," + a_iterador2 + "," + a_attMatriz + "," + a_valor + "): \n" + std::string(erro.what())); }
	};
	virtual void addDadoAttMatrizMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_iterador1, const std::string a_iterador2, const std::string a_attMatriz, const std::string a_valor, const int a_alocacao_1, const int a_alocacao_2) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception & erro) { throw std::invalid_argument("SmartDados::addDadoAttMatrizMembro(" + a_membro + "," + a_idMembro + "," + a_iterador1 + "," + a_iterador2 + "," + a_attMatriz + "," + a_valor + "," + getString(a_alocacao_1) + "," + getString(a_alocacao_2) + "): \n" + std::string(erro.what())); }
	};
	virtual void validacaoDadosAttMatrizMembro(const std::string a_membro, const std::vector<std::string> &a_vetorIdMembro, const std::vector<std::string> &a_vetorAttMatriz) {};

	virtual std::vector<std::vector<std::string>> getDadosAttMatrizMembro(const bool a_incluirIteradores, const std::string a_membro, const std::string a_idMembro, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttMatriz(" + getString(a_incluirIteradores) + "," + a_membro + "," + a_idMembro + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + "," + getString(a_incluirAtributo) + ",a_attMatriz): \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttMatrizMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_iterador1, const std::string a_iterador2, const std::string a_attMatriz, const std::string a_valor) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadoAttMatrizMembroMembro(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_iterador1 + "," + a_iterador2 + "," + a_attMatriz + "," + a_valor + "): \n" + std::string(erro.what())); }
	};
	virtual void addDadoAttMatrizMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_iterador1, const std::string a_iterador2, const std::string a_attMatriz, const std::string a_valor, const int a_alocacao_1, const int a_alocacao_2) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception & erro) { throw std::invalid_argument("SmartDados::addDadoAttMatrizMembroMembro(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_iterador1 + "," + a_iterador2 + "," + a_attMatriz + "," + a_valor + "): \n" + std::string(erro.what())); }
	};
	virtual void validacaoDadosAttMatrizMembroMembro(const std::string a_membro, const std::vector<std::string> &a_vetorIdMembro, const std::string a_membroMembro, const std::vector<std::string> &a_vetorIdMembroMembro, const std::vector<std::string> &a_vetorAttMatriz) {};

	virtual std::vector<std::vector<std::string>> getDadosAttMatrizMembroMembro(const bool a_incluirIteradores, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttMatriz(" + getString(a_incluirIteradores) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + "," + getString(a_incluirAtributo) + ",a_attMatriz): \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttMatrizMembroMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_iterador1, const std::string a_iterador2, const std::string a_attMatriz, const std::string a_valor) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadoAttMatrizMembroMembroMembro(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_iterador1 + "," + a_iterador2 + "," + a_attMatriz + "," + a_valor + "): \n" + std::string(erro.what())); }
	};
	virtual void addDadoAttMatrizMembroMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_iterador1, const std::string a_iterador2, const std::string a_attMatriz, const std::string a_valor, const int a_alocacao_1, const int a_alocacao_2) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception & erro) { throw std::invalid_argument("SmartDados::addDadoAttMatrizMembroMembroMembro(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_iterador1 + "," + a_iterador2 + "," + a_attMatriz + "," + a_valor + "): \n" + std::string(erro.what())); }
	};
	virtual void validacaoDadosAttMatrizMembroMembroMembro(const std::string a_membro, const std::vector<std::string> &a_vetorIdMembro, const std::string a_membroMembro, const std::vector<std::string> &a_vetorIdMembroMembro, const std::string a_membroMembroMembro, const std::vector<std::string> &a_vetorIdMembroMembroMembro, const std::vector<std::string> &a_vetorAttMatriz) {};

	virtual std::vector<std::vector<std::string>> getDadosAttMatrizMembroMembroMembro(const bool a_incluirIteradores, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttMatrizMembroMembroMembro(" + getString(a_incluirIteradores) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + "," + getString(a_incluirAtributo) + ",a_attMatriz): \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttMatrizMembroMembroMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_membroMembroMembroMembro, const std::string a_idMembroMembroMembroMembro, const std::string a_iterador1, const std::string a_iterador2, const std::string a_attMatriz, const std::string a_valor) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadoAttMatrizMembroMembroMembroMembro(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_membroMembroMembroMembro + "," + a_idMembroMembroMembroMembro + "," + a_iterador1 + "," + a_iterador2 + "," + a_attMatriz + "," + a_valor + "): \n" + std::string(erro.what())); }
	};
	virtual void addDadoAttMatrizMembroMembroMembroMembro(const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_membroMembroMembroMembro, const std::string a_idMembroMembroMembroMembro, const std::string a_iterador1, const std::string a_iterador2, const std::string a_attMatriz, const std::string a_valor, const int a_alocacao_1, const int a_alocacao_2) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception & erro) { throw std::invalid_argument("SmartDados::addDadoAttMatrizMembroMembroMembroMembro(" + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_membroMembroMembroMembro + "," + a_idMembroMembroMembroMembro + "," + a_iterador1 + "," + a_iterador2 + "," + a_attMatriz + "," + a_valor + "): \n" + std::string(erro.what())); }
	};
	virtual void validacaoDadosAttMatrizMembroMembroMembroMembro(const std::string a_membro, const std::vector<std::string> &a_vetorIdMembro, const std::string a_membroMembro, const std::vector<std::string> &a_vetorIdMembroMembro, const std::string a_membroMembroMembro, const std::vector<std::string> &a_vetorIdMembroMembroMembro, const std::string a_membroMembroMembroMembro, const std::vector<std::string> &a_vetorIdMembroMembroMembroMembro, const std::vector<std::string> &a_vetorAttMatriz) {};

	virtual std::vector<std::vector<std::string>> getDadosAttMatrizMembroMembroMembroMembro(const bool a_incluirIteradores, const std::string a_membro, const std::string a_idMembro, const std::string a_membroMembro, const std::string a_idMembroMembro, const std::string a_membroMembroMembro, const std::string a_idMembroMembroMembro, const std::string a_membroMembroMembroMembro, const std::string a_idMembroMembroMembroMembro, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttMatrizMembroMembroMembroMembro(" + getString(a_incluirIteradores) + "," + a_membro + "," + a_idMembro + "," + a_membroMembro + "," + a_idMembroMembro + "," + a_membroMembroMembro + "," + a_idMembroMembroMembro + "," + a_membroMembroMembroMembro + "," + a_idMembroMembroMembroMembro + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + "," + getString(a_incluirAtributo) + ",a_attMatriz): \n" + std::string(erro.what())); }
	};

};

#define DEFINE_SMART_ELEMENTO_INDIVIDUALMENTE(Classe, SmartElemento, valores) \
DEFINE_SMART_##SmartElemento(Classe, SmartElemento, valores)

#define DEFINE_SMART_ELEMENTO(Classe, valores) \
valores(DEFINE_SMART_ELEMENTO_INDIVIDUALMENTE) 

#define DECLARA_SMART_ELEMENTO_INDIVIDUALMENTE(Classe, SmartElemento, valores) \
DECLARA_METODOS_##SmartElemento(Classe, SmartElemento, valores)

#define DECLARA_SMART_ELEMENTO(Classe, valores) \
valores(DECLARA_SMART_ELEMENTO_INDIVIDUALMENTE) \
\
template<typename IdM> \
IdM getMaiorId(const IdM a_m)const{ return IdM(); }; \
template<typename IdM, typename IdMM> \
IdMM getMaiorId(const Id##M a_m, const IdMM a_mM){ return IdMM(); };\
template<typename IdM, typename IdMM, typename IdMMM> \
IdMMM getMaiorId(const Id##M a_m, const IdMM a_mM, const IdMMM a_mMM){ return IdMMM(); };\
template<typename IdM, typename Id##MM, typename IdMMM, typename IdMMMM> \
IdMMMM getMaiorId(const Id##M a_m, const Id##MM a_mM, const IdMMM a_mMM, const IdMMMM a_mMMM){ return IdMMMM(); };\
\
template<typename IdM> \
std::vector<IdM> getIdObjetos(const IdM a_m)const{ return std::vector<IdM>(); }; \
template<typename IdM, typename IdMM> \
std::vector<IdMM> getIdObjetos(const Id##M a_m, const IdMM a_mM){ return std::vector<IdMM>(); };\
\
template<typename IdM, typename Att, typename Vlr> \
std::vector<IdM> getIdObjetos(const IdM a_m, const Att a_att, const Vlr a_vlr)const{ return std::vector<IdM>(); }; \
template<typename IdM, typename IdMM, typename Att, typename Vlr> \
std::vector<IdMM> getIdObjetos(const Id##M a_m, const IdMM a_mM, const Att a_att, const Vlr a_vlr){ return std::vector<IdMM>(); };\
\
template<typename IdM> \
int numObjetos(const IdM a_m){ return 0; }; \
template<typename IdM, typename IdMM> \
int numObjetos(const Id##M a_m, const IdMM a_mM){ return 0; };\
template<typename IdM, typename IdMM, typename IdMMM> \
int numObjetos(const Id##M a_m, const IdMM a_mM, const IdMMM a_mMM){ return 0; };\
template<typename IdM, typename Id##MM, typename IdMMM, typename IdMMMM> \
int numObjetos(const Id##M a_m, const Id##MM a_mM, const IdMMM a_mMM, const IdMMMM a_mMMM){ return 0; };\
\
\
template<typename Att> \
int getSizeVetor(const Att a_att)const{ return 0; }; \
\
template<typename IdM, typename Att> \
int getSizeVetor(const IdM a_m, const Att a_att){ return 0; }; \
template<typename IdM, typename IdMM, typename Att> \
int getSizeVetor(const Id##M a_m, const IdMM a_mM, const Att a_att){ return 0; };\
template<typename IdM, typename IdMM, typename IdMMM, typename Att> \
int getSizeVetor(const Id##M a_m, const IdMM a_mM, const IdMMM a_mMM, const Att a_att){ return 0; };\
template<typename IdM, typename Id##MM, typename IdMMM, typename IdMMMM, typename Att> \
int getSizeVetor(const Id##M a_m, const Id##MM a_mM, const IdMMM a_mMM, const IdMMMM a_mMMM, const Att a_att){ return 0; };\
\
template<typename IdM, typename Att, typename It1> \
It1 getIteradorInicial(const IdM a_m, const Att a_att, const It1 a_it1){ return It1(); }; \
template<typename IdM, typename IdMM, typename Att, typename It1> \
It1 getIteradorInicial(const Id##M a_m, const IdMM a_mM, const Att a_att, const It1 a_it1){ return It1(); };\
template<typename IdM, typename IdMM, typename IdMMM, typename Att, typename It1> \
It1 getIteradorInicial(const Id##M a_m, const IdMM a_mM, const IdMMM a_mMM, const Att a_att, const It1 a_it1){ return It1(); };\
template<typename IdM, typename Id##MM, typename IdMMM, typename IdMMMM, typename Att, typename It1> \
It1 getIteradorInicial(const Id##M a_m, const Id##MM a_mM, const IdMMM a_mMM, const IdMMMM a_mMMM, const Att a_att, const It1 a_it1){ return It1(); };\
\
template<typename IdM, typename Att, typename It1> \
It1 getIteradorFinal(const IdM a_m, const Att a_att, const It1 a_it1){ return It1(); }; \
template<typename IdM, typename IdMM, typename Att, typename It1> \
It1 getIteradorFinal(const Id##M a_m, const IdMM a_mM, const Att a_att, const It1 a_it1){ return It1(); };\
template<typename IdM, typename IdMM, typename IdMMM, typename Att, typename It1> \
It1 getIteradorFinal(const Id##M a_m, const IdMM a_mM, const IdMMM a_mMM, const Att a_att, const It1 a_it1){ return It1(); };\
template<typename IdM, typename Id##MM, typename IdMMM, typename IdMMMM, typename Att, typename It1> \
It1 getIteradorFinal(const Id##M a_m, const Id##MM a_mM, const IdMMM a_mMM, const IdMMMM a_mMMM, const Att a_att, const It1 a_it1){ return It1(); };\
\
template<typename IdM, typename Att, typename It1, typename Vlr> \
Vlr getElementoVetor(const IdM a_m, const Att a_att, const It1 a_it1, const Vlr a_vlr){ return Vlr(); }; \
template<typename IdM, typename IdMM, typename Att, typename It1, typename Vlr> \
Vlr getElementoVetor(const Id##M a_m, const IdMM a_mM, const Att a_att, const It1 a_it1, const Vlr a_vlr){ return Vlr(); };\
template<typename IdM, typename IdMM, typename IdMMM, typename Att, typename It1, typename Vlr> \
Vlr getElementoVetor(const Id##M a_m, const IdMM a_mM, const IdMMM a_mMM, const Att a_att, const It1 a_it1, const Vlr a_vlr){ return Vlr(); };\
template<typename IdM, typename Id##MM, typename IdMMM, typename IdMMMM, typename Att, typename It1, typename Vlr> \
Vlr getElementoVetor(const Id##M a_m, const Id##MM a_mM, const IdMMM a_mMM, const IdMMMM a_mMMM, const Att a_att, const It1 a_it1, const Vlr a_vlr){ return Vlr(); };\
\
template<typename IdM, typename Att, typename It1, typename Vlr> \
SmartEnupla<It1, Vlr> getVetor(const IdM a_m, const Att a_att, const It1 a_it1, const Vlr a_vlr){ return SmartEnupla<It1, Vlr>(); }; \
template<typename IdM, typename IdMM, typename Att, typename It1, typename Vlr> \
SmartEnupla<It1, Vlr> getVetor(const Id##M a_m, const IdMM a_mM, const Att a_att, const It1 a_it1, const Vlr a_vlr){ return SmartEnupla<It1, Vlr>(); };\
template<typename IdM, typename IdMM, typename IdMMM, typename Att, typename It1, typename Vlr> \
SmartEnupla<It1, Vlr> getVetor(const Id##M a_m, const IdMM a_mM, const IdMMM a_mMM, const Att a_att, const It1 a_it1, const Vlr a_vlr){ return SmartEnupla<It1, Vlr>(); };\
template<typename IdM, typename Id##MM, typename IdMMM, typename IdMMMM, typename Att, typename It1, typename Vlr> \
SmartEnupla<It1, Vlr> getVetor(const Id##M a_m, const Id##MM a_mM, const IdMMM a_mMM, const IdMMMM a_mMMM, const Att a_att, const It1 a_it1, const Vlr a_vlr){ return SmartEnupla<It1, Vlr>(); };\
\
\
\
template<typename Att> \
int getSizeMatriz(const Att a_att)const{ return 0; }; \
template<typename Att, typename It1> \
int getSizeMatriz(const Att a_att, const It1 a_it1)const{ return 0; }; \
\
template<typename IdM, typename Att> \
int getSize1Matriz(const IdM a_m, const Att a_att){ return 0; }; \
template<typename IdM, typename IdMM, typename Att> \
int getSize1Matriz(const Id##M a_m, const IdMM a_mM, const Att a_att){ return 0; };\
template<typename IdM, typename IdMM, typename IdMMM, typename Att> \
int getSize1Matriz(const Id##M a_m, const IdMM a_mM, const IdMMM a_mMM, const Att a_att){ return 0; };\
template<typename IdM, typename Id##MM, typename IdMMM, typename IdMMMM, typename Att> \
int getSize1Matriz(const Id##M a_m, const Id##MM a_mM, const IdMMM a_mMM, const IdMMMM a_mMMM, const Att a_att){ return 0; };\
\
template<typename IdM, typename Att, typename It1> \
int getSize2Matriz(const IdM a_m, const Att a_att, const It1 a_it1){ return 0; }; \
template<typename IdM, typename IdMM, typename Att, typename It1> \
int getSize2Matriz(const Id##M a_m, const IdMM a_mM, const Att a_att, const It1 a_it1){ return 0; };\
template<typename IdM, typename IdMM, typename IdMMM, typename Att, typename It1> \
int getSize2Matriz(const Id##M a_m, const IdMM a_mM, const IdMMM a_mMM, const Att a_att, const It1 a_it1){ return 0; };\
template<typename IdM, typename Id##MM, typename IdMMM, typename IdMMMM, typename Att, typename It1> \
int getSize2Matriz(const Id##M a_m, const Id##MM a_mM, const IdMMM a_mMM, const IdMMMM a_mMMM, const Att a_att, const It1 a_it1){ return 0; };\
\
template<typename IdM, typename Att, typename It1> \
It1 getIterador1Inicial(const IdM a_m, const Att a_att, const It1 a_it1){ return It1(); }; \
template<typename IdM, typename IdMM, typename Att, typename It1> \
It1 getIterador1Inicial(const Id##M a_m, const IdMM a_mM, const Att a_att, const It1 a_it1){ return It1(); };\
template<typename IdM, typename IdMM, typename IdMMM, typename Att, typename It1> \
It1 getIterador1Inicial(const Id##M a_m, const IdMM a_mM, const IdMMM a_mMM, const Att a_att, const It1 a_it1){ return It1(); };\
template<typename IdM, typename Id##MM, typename IdMMM, typename IdMMMM, typename Att, typename It1> \
It1 getIterador1Inicial(const Id##M a_m, const Id##MM a_mM, const IdMMM a_mMM, const IdMMMM a_mMMM, const Att a_att, const It1 a_it1){ return It1(); };\
\
template<typename IdM, typename Att, typename It1> \
It1 getIterador1Final(const IdM a_m, const Att a_att, const It1 a_it1){ return It1(); }; \
template<typename IdM, typename IdMM, typename Att, typename It1> \
It1 getIterador1Final(const Id##M a_m, const IdMM a_mM, const Att a_att, const It1 a_it1){ return It1(); };\
template<typename IdM, typename IdMM, typename IdMMM, typename Att, typename It1> \
It1 getIterador1Final(const Id##M a_m, const IdMM a_mM, const IdMMM a_mMM, const Att a_att, const It1 a_it1){ return It1(); };\
template<typename IdM, typename Id##MM, typename IdMMM, typename IdMMMM, typename Att, typename It1> \
It1 getIterador1Final(const Id##M a_m, const Id##MM a_mM, const IdMMM a_mMM, const IdMMMM a_mMMM, const Att a_att, const It1 a_it1){ return It1(); };\
\
template<typename IdM, typename Att, typename It1, typename It2> \
It2 getIterador2Inicial(const IdM a_m, const Att a_att, const It1 a_it1, const It2 a_it2){ return It2(); }; \
template<typename IdM, typename IdMM, typename Att, typename It1, typename It2> \
It2 getIterador2Inicial(const Id##M a_m, const IdMM a_mM, const Att a_att, const It1 a_it1, const It2 a_it2){ return It2(); };\
template<typename IdM, typename IdMM, typename IdMMM, typename Att, typename It1, typename It2> \
It2 getIterador2Inicial(const Id##M a_m, const IdMM a_mM, const IdMMM a_mMM, const Att a_att, const It1 a_it1, const It2 a_it2){ return It2(); };\
template<typename IdM, typename Id##MM, typename IdMMM, typename IdMMMM, typename Att, typename It1, typename It2> \
It2 getIterador2Inicial(const Id##M a_m, const Id##MM a_mM, const IdMMM a_mMM, const IdMMMM a_mMMM, const Att a_att, const It1 a_it1, const It2 a_it2){ return It2(); };\
\
template<typename IdM, typename Att, typename It1, typename It2> \
It2 getIterador2Final(const IdM a_m, const Att a_att, const It1 a_it1, const It2 a_it2){ return It2(); }; \
template<typename IdM, typename IdMM, typename Att, typename It1, typename It2> \
It2 getIterador2Final(const Id##M a_m, const IdMM a_mM, const Att a_att, const It1 a_it1, const It2 a_it2){ return It2(); };\
template<typename IdM, typename IdMM, typename IdMMM, typename Att, typename It1, typename It2> \
It2 getIterador2Final(const Id##M a_m, const IdMM a_mM, const IdMMM a_mMM, const Att a_att, const It1 a_it1, const It2 a_it2){ return It2(); };\
template<typename IdM, typename Id##MM, typename IdMMM, typename IdMMMM, typename Att, typename It1, typename It2> \
It2 getIterador2Final(const Id##M a_m, const Id##MM a_mM, const IdMMM a_mMM, const IdMMMM a_mMMM, const Att a_att, const It1 a_it1, const It2 a_it2){ return It2(); };\
\
template<typename IdM, typename Att, typename It1, typename It2, typename Vlr> \
Vlr getElementoMatriz(const IdM a_m, const Att a_att, const It1 a_it1, const It2 a_it2, const Vlr a_vlr){ return Vlr(); }; \
template<typename IdM, typename IdMM, typename Att, typename It1, typename It2, typename Vlr> \
Vlr getElementoMatriz(const Id##M a_m, const IdMM a_mM, const Att a_att, const It1 a_it1, const It2 a_it2, const Vlr a_vlr){ return Vlr(); };\
template<typename IdM, typename IdMM, typename IdMMM, typename Att, typename It1, typename It2, typename Vlr> \
Vlr getElementoMatriz(const Id##M a_m, const IdMM a_mM, const IdMMM a_mMM, const Att a_att, const It1 a_it1, const It2 a_it2, const Vlr a_vlr){ return Vlr(); };\
template<typename IdM, typename Id##MM, typename IdMMM, typename IdMMMM, typename Att, typename It1, typename It2, typename Vlr> \
Vlr getElementoMatriz(const Id##M a_m, const Id##MM a_mM, const IdMMM a_mMM, const IdMMMM a_mMMM, const Att a_att, const It1 a_it1, const It2 a_it2, const Vlr a_vlr){ return Vlr(); };\
\
template<typename IdM, typename Att, typename It1, typename It2, typename Vlr> \
SmartEnupla<It2, Vlr> getElementosMatriz(const IdM a_m, const Att a_att, const It1 a_it1, const It2 a_it2, const Vlr a_vlr){ return SmartEnupla<It2, Vlr>(); }; \
template<typename IdM, typename IdMM, typename Att, typename It1, typename It2, typename Vlr> \
SmartEnupla<It2, Vlr> getElementosMatriz(const Id##M a_m, const IdMM a_mM, const Att a_att, const It1 a_it1, const It2 a_it2, const Vlr a_vlr){ return SmartEnupla<It2, Vlr>(); };\
template<typename IdM, typename IdMM, typename IdMMM, typename Att, typename It1, typename It2, typename Vlr> \
SmartEnupla<It2, Vlr> getElementosMatriz(const Id##M a_m, const IdMM a_mM, const IdMMM a_mMM, const Att a_att, const It1 a_it1, const It2 a_it2, const Vlr a_vlr){ return SmartEnupla<It2, Vlr>(); };\
template<typename IdM, typename Id##MM, typename IdMMM, typename IdMMMM, typename Att, typename It1, typename It2, typename Vlr> \
SmartEnupla<It2, Vlr> getElementosMatriz(const Id##M a_m, const Id##MM a_mM, const IdMMM a_mMM, const IdMMMM a_mMMM, const Att a_att, const It1 a_it1, const It2 a_it2, const Vlr a_vlr){ return SmartEnupla<It2, Vlr>(); };\
\
template<typename IdM, typename Att, typename It1, typename It2, typename Vlr> \
SmartEnupla<It1, SmartEnupla<It2, Vlr>> getMatriz(const IdM a_m, const Att a_att, const It1 a_it1, const It2 a_it2, const Vlr a_vlr){ return SmartEnupla<It1, SmartEnupla<It2, Vlr>>(); }; \
template<typename IdM, typename IdMM, typename Att, typename It1, typename It2, typename Vlr> \
SmartEnupla<It1, SmartEnupla<It2, Vlr>> getMatriz(const Id##M a_m, const IdMM a_mM, const Att a_att, const It1 a_it1, const It2 a_it2, const Vlr a_vlr){ return SmartEnupla<It1, SmartEnupla<It2, Vlr>>(); };\
template<typename IdM, typename IdMM, typename IdMMM, typename Att, typename It1, typename It2, typename Vlr> \
SmartEnupla<It1, SmartEnupla<It2, Vlr>> getMatriz(const Id##M a_m, const IdMM a_mM, const IdMMM a_mMM, const Att a_att, const It1 a_it1, const It2 a_it2, const Vlr a_vlr){ return SmartEnupla<It1, SmartEnupla<It2, Vlr>>(); };\
template<typename IdM, typename Id##MM, typename IdMMM, typename IdMMMM, typename Att, typename It1, typename It2, typename Vlr> \
SmartEnupla<It1, SmartEnupla<It2, Vlr>> getMatriz(const Id##M a_m, const Id##MM a_mM, const IdMMM a_mMM, const IdMMMM a_mMMM, const Att a_att, const It1 a_it1, const It2 a_it2, const Vlr a_vlr){ return SmartEnupla<It1, SmartEnupla<It2, Vlr>>(); };\
\


#define INICIALIZA_SMART_ELEMENTO_INDIVIDUALMENTE(Classe, SmartElemento, valores) \
INICIALIZAR_##SmartElemento(Classe, SmartElemento, valores)

#define INICIALIZA_SMART_ELEMENTO(Classe, valores) \
valores(INICIALIZA_SMART_ELEMENTO_INDIVIDUALMENTE)


#define COPIA_SMART_ELEMENTO_INDIVIDUALMENTE(Classe, SmartElemento, valores) \
COPIAR_##SmartElemento(Classe, SmartElemento, valores)

#define COPIA_SMART_ELEMENTO(Classe, valores) \
valores(COPIA_SMART_ELEMENTO_INDIVIDUALMENTE)

#define EXCLUI_SMART_ELEMENTO_INDIVIDUALMENTE(Classe, SmartElemento, valores) \
EXCLUIR_##SmartElemento(Classe, SmartElemento, valores)

#define EXCLUI_SMART_ELEMENTO(Classe, valores) \
valores(EXCLUI_SMART_ELEMENTO_INDIVIDUALMENTE)

#define GET_STRING_FROM_CLASS(Classe) \
static std::string getString    (Classe a_classe) { return std::string(#Classe); }; \
static std::string getFullString(Classe a_classe) { return std::string(#Classe); }; \
static std::string getStringTipo(Classe a_classe) { return std::string(#Classe); }; 
#endif  // SMART_ELEMENTOS_H