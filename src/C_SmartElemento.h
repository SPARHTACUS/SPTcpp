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
	virtual void addDadosAttComum(const std::string a_idM1, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores) { 
			try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); } 
			catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadosAttComum(" + a_idM1 + "&a_vetorAttComuns,&a_vetorValores): \n" + std::string(erro.what())); }
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

	virtual void addDadosAttComumM1(const std::string a_m1, const std::string a_idM1,  const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); } 
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadosAttComumMembros(" + a_m1 + "," + a_idM1 + ",&a_vetorAttComuns,&a_vetorValores): \n" + std::string(erro.what())); }
	};
	virtual void validacaoDadosAttComumM1(const std::string a_m1, const std::vector<std::string> &a_vetorIdMembro) {};

	virtual std::vector<std::vector<std::string>> getDadosAttComumM1(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::vector<std::string> a_vetorAttComum) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttComumM1(" + a_m1 + "," + a_idM1 + ",a_vetorAttComum): \n" + std::string(erro.what())); }
	};

	virtual void addDadosAttComumM2(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores) {
			try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); } 
			catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadosAttComumM2(" + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + ",&a_vetorAttComuns,&a_vetorValores): \n" + std::string(erro.what())); }
	};
	virtual void validacaoDadosAttComumM2(const std::string a_m1, const std::vector<std::string> &a_vetorIdMembro, const std::string a_m2, const std::vector<std::string> &a_vetorIdM2) {};

	virtual std::vector<std::vector<std::string>> getDadosAttComumM2(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::vector<std::string> a_vetorAttComum) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttComumM2(" + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + ",a_vetorAttComum): \n" + std::string(erro.what())); }
	};

	virtual void addDadosAttComumM3(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores) { 
			try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); } 
			catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadosAttComumM3(" + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + ",&a_vetorAttComuns,&a_vetorValores): \n" + std::string(erro.what())); }
	};
	virtual void validacaoDadosAttComumM3(const std::string a_m1, const std::vector<std::string> &a_vetorIdMembro, const std::string a_m2, const std::vector<std::string> &a_vetorIdM2, const std::string a_m3, const std::vector<std::string> &a_vetorIdM3) {};

	virtual std::vector<std::vector<std::string>> getDadosAttComumM3(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::vector<std::string> a_vetorAttComum) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttComumM3(" + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + ",a_vetorAttComum): \n" + std::string(erro.what())); }
	};

	virtual void addDadosAttComumM4(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_m4, const std::string a_idM4, const std::vector<std::string> &a_vetorAttComuns, const std::vector<std::string> &a_vetorValores) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadosAttComumM4(" + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + "," + a_m4 + "," + a_idM4 + ",&a_vetorAttComuns,&a_vetorValores): \n" + std::string(erro.what())); }
	};
	virtual void validacaoDadosAttComumM4(const std::string a_m1, const std::vector<std::string> &a_vetorIdMembro, const std::string a_m2, const std::vector<std::string> &a_vetorIdM2, const std::string a_m3, const std::vector<std::string> &a_vetorIdM3, const std::string a_m4, const std::vector<std::string> &a_vetorIdM4) {};

	virtual std::vector<std::vector<std::string>> getDadosAttComumM4(const bool a_incluirCabecalho, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_m4, const std::string a_idM4, const std::vector<std::string> a_vetorAttComum) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttComumM4(" + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + "," + a_m4 + "," + a_idM4 + ",a_vetorAttComum): \n" + std::string(erro.what())); }
	};

	//
	// AttVetor
	//

	virtual void addDadoAttVetor(const std::vector<std::string>& a_iterador, const std::string& a_attVetor, const std::vector<std::string>& a_valor) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadoAttVetor(): \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttVetorM1(const std::string a_m1, const std::string a_idM1, const std::vector<std::string>& a_iterador, const std::string& a_attVetor, const std::vector<std::string>& a_valor) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadoAttVetorM1(" + a_m1 + "," + a_idM1 + ") : \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttVetorM2(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::vector<std::string>& a_iterador, const std::string& a_attVetor, const std::vector<std::string>& a_valor) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadoAttVetorM2(" + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + ") : \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttVetorM3(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::vector<std::string>& a_iterador, const std::string& a_attVetor, const std::vector<std::string>& a_valor) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadoAttVetorM3(" + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + ") : \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttVetorM4(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_m4, const std::string a_idM4, const std::vector<std::string>& a_iterador, const std::string& a_attVetor, const std::vector<std::string>& a_valor) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadoAttVetorM4(" + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + "," + a_m4 + "," + a_idM4 + ") : \n" + std::string(erro.what())); }
	};

	virtual void validacaoDadosAttVetor(const std::vector<std::string> &a_vetorAttVetor) {};

	virtual std::vector<std::vector<std::string>> getDadosAttVetor(const bool a_incluirIteradores, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttVetor(" + getString(a_incluirIteradores) + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_attVetor): \n" + std::string(erro.what())); }
	};

	virtual void validacaoDadosAttVetorM1(const std::string a_m1, const std::vector<std::string> &a_vetorIdMembro, const std::vector<std::string> &a_vetorAttVetor) {};

	virtual std::vector<std::vector<std::string>> getDadosAttVetorM1(const bool a_incluirIteradores, const std::string a_m1, const std::string a_idM1, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttVetor(" + getString(a_incluirIteradores) + "," + a_m1 + "," + a_idM1 + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_attVetor): \n" + std::string(erro.what())); }
	};

	virtual void validacaoDadosAttVetorM2(const std::string a_m1, const std::vector<std::string> &a_vetorIdMembro, const std::string a_m2, const std::vector<std::string> &a_vetorIdM2, const std::vector<std::string> &a_vetorAttVetor) {};

	virtual std::vector<std::vector<std::string>> getDadosAttVetorM2(const bool a_incluirIteradores, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttVetor(" + getString(a_incluirIteradores) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_attVetor): \n" + std::string(erro.what())); }
	};

	virtual void validacaoDadosAttVetorM3(const std::string a_m1, const std::vector<std::string> &a_vetorIdMembro, const std::string a_m2, const std::vector<std::string> &a_vetorIdM2, const std::string a_m3, const std::vector<std::string> &a_vetorIdM3, const std::vector<std::string> &a_vetorAttVetor) {};

	virtual std::vector<std::vector<std::string>> getDadosAttVetorM3(const bool a_incluirIteradores, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttVetorM3(" + getString(a_incluirIteradores) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_attVetor): \n" + std::string(erro.what())); }
	};

	virtual void validacaoDadosAttVetorM4(const std::string a_m1, const std::vector<std::string> &a_vetorIdMembro, const std::string a_m2, const std::vector<std::string> &a_vetorIdM2, const std::string a_m3, const std::vector<std::string> &a_vetorIdM3, const std::string a_m4, const std::vector<std::string> &a_vetorIdM4, const std::vector<std::string> &a_vetorAttVetor) {};

	virtual std::vector<std::vector<std::string>> getDadosAttVetorM4(const bool a_incluirIteradores, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_m4, const std::string a_idM4, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttVetor) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttVetorM4(" + getString(a_incluirIteradores) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + "," + a_m4 + "," + a_idM4 + "," + a_iteradorInicial + "," + a_iteradorFinal + ",a_attVetor): \n" + std::string(erro.what())); }
	};

	//
	// AttMatriz
	//
	

	virtual void addDadoAttMatriz(const std::vector<std::string>& a_iter1, const std::vector<std::string>& a_iter2, const std::string& a_att, const std::vector<std::vector<std::string>>& a_vlr) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadoAttMatriz(): \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttMatrizM1(const std::string a_m1, const std::string a_idM1, const std::vector<std::string>& a_iter1, const std::vector<std::string>& a_iter2, const std::string& a_att, const std::vector<std::vector<std::string>>& a_vlr) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadoAttMatrizM1(): \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttMatrizM2(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::vector<std::string>& a_iter1, const std::vector<std::string>& a_iter2, const std::string& a_att, const std::vector<std::vector<std::string>>& a_vlr) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadoAttMatrizM2(): \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttMatrizM3(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::vector<std::string>& a_iter1, const std::vector<std::string>& a_iter2, const std::string& a_att, const std::vector<std::vector<std::string>>& a_vlr) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadoAttMatrizM3(): \n" + std::string(erro.what())); }
	};

	virtual void addDadoAttMatrizM4(const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_m4, const std::string a_idM4, const std::vector<std::string>& a_iter1, const std::vector<std::string>& a_iter2, const std::string& a_att, const std::vector<std::vector<std::string>>& a_vlr) {
		try { throw std::invalid_argument("Estrutura de adicao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::addDadoAttMatrizM4(): \n" + std::string(erro.what())); }
	};

	virtual void validacaoDadosAttMatriz(const std::vector<std::string> &a_vetorAttMatriz) {};

	virtual std::vector<std::vector<std::string>> getDadosAttMatriz(const bool a_incluirIteradores, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz) const{
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttMatriz(" + getString(a_incluirIteradores) + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + "," + getString(a_incluirAtributo) + ",a_attMatriz): \n" + std::string(erro.what())); }
	};

	virtual void validacaoDadosAttMatrizM1(const std::string a_m1, const std::vector<std::string> &a_vetorIdMembro, const std::vector<std::string> &a_vetorAttMatriz) {};

	virtual std::vector<std::vector<std::string>> getDadosAttMatrizM1(const bool a_incluirIteradores, const std::string a_m1, const std::string a_idM1, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttMatriz(" + getString(a_incluirIteradores) + "," + a_m1 + "," + a_idM1 + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + "," + getString(a_incluirAtributo) + ",a_attMatriz): \n" + std::string(erro.what())); }
	};

	virtual void validacaoDadosAttMatrizM2(const std::string a_m1, const std::vector<std::string> &a_vetorIdMembro, const std::string a_m2, const std::vector<std::string> &a_vetorIdM2, const std::vector<std::string> &a_vetorAttMatriz) {};

	virtual std::vector<std::vector<std::string>> getDadosAttMatrizM2(const bool a_incluirIteradores, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttMatriz(" + getString(a_incluirIteradores) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + "," + getString(a_incluirAtributo) + ",a_attMatriz): \n" + std::string(erro.what())); }
	};

	virtual void validacaoDadosAttMatrizM3(const std::string a_m1, const std::vector<std::string> &a_vetorIdMembro, const std::string a_m2, const std::vector<std::string> &a_vetorIdM2, const std::string a_m3, const std::vector<std::string> &a_vetorIdM3, const std::vector<std::string> &a_vetorAttMatriz) {};

	virtual std::vector<std::vector<std::string>> getDadosAttMatrizM3(const bool a_incluirIteradores, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttMatrizM3(" + getString(a_incluirIteradores) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + "," + getString(a_incluirAtributo) + ",a_attMatriz): \n" + std::string(erro.what())); }
	};

	virtual void validacaoDadosAttMatrizM4(const std::string a_m1, const std::vector<std::string> &a_vetorIdMembro, const std::string a_m2, const std::vector<std::string> &a_vetorIdM2, const std::string a_m3, const std::vector<std::string> &a_vetorIdM3, const std::string a_m4, const std::vector<std::string> &a_vetorIdM4, const std::vector<std::string> &a_vetorAttMatriz) {};

	virtual std::vector<std::vector<std::string>> getDadosAttMatrizM4(const bool a_incluirIteradores, const std::string a_m1, const std::string a_idM1, const std::string a_m2, const std::string a_idM2, const std::string a_m3, const std::string a_idM3, const std::string a_m4, const std::string a_idM4, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const bool a_incluirAtributo, const std::vector<std::string> a_vetorAttMatriz) {
		try { throw std::invalid_argument("Estrutura de obtencao de dados nao definida."); }
		catch (const std::exception& erro) { throw std::invalid_argument("SmartDados::getDadosAttMatrizM4(" + getString(a_incluirIteradores) + "," + a_m1 + "," + a_idM1 + "," + a_m2 + "," + a_idM2 + "," + a_m3 + "," + a_idM3 + "," + a_m4 + "," + a_idM4 + "," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + "," + getString(a_incluirAtributo) + ",a_attMatriz): \n" + std::string(erro.what())); }
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
IdM getIdOut(const IdM a_m)const{ return IdM(); }; \
template<typename IdM, typename IdMM> \
IdMM getIdOut(const Id##M a_m, const IdMM a_mM){ return IdMM(); };\
\
template<typename IdM> \
IdM getMenorId(const IdM a_m)const{ return IdM(); }; \
template<typename IdM, typename IdMM> \
IdMM getMenorId(const Id##M a_m, const IdMM a_mM){ return IdMM(); };\
template<typename IdM, typename IdMM, typename IdMMM> \
IdMMM getMenorId(const Id##M a_m, const IdMM a_mM, const IdMMM a_mMM){ return IdMMM(); };\
template<typename IdM, typename Id##MM, typename IdMMM, typename IdMMMM> \
IdMMMM getMenorId(const Id##M a_m, const Id##MM a_mM, const IdMMM a_mMM, const IdMMMM a_mMMM){ return IdMMMM(); };\
\
template<typename IdM> \
void incr(IdM &a_m)const{ return IdM(); }; \
template<typename IdM, typename IdMM> \
void incr(const Id##M a_m, IdMM &a_mM){ return IdMM(); };\
\
template<typename IdM> \
bool isInstanciado(const IdM a_m)const{ return IdM(); }; \
template<typename IdM, typename IdMM> \
bool isInstanciado(const Id##M a_m, const IdMM a_mM){ return IdMM(); };\
template<typename IdM, typename IdMM, typename IdMMM> \
bool isInstanciado(const Id##M a_m, const IdMM a_mM, const IdMMM a_mMM){ return IdMMM(); };\
template<typename IdM, typename Id##MM, typename IdMMM, typename IdMMMM> \
bool isInstanciado(const Id##M a_m, const Id##MM a_mM, const IdMMM a_mMM, const IdMMMM a_mMMM){ return IdMMMM(); };\
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