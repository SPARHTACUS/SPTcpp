#ifndef C_AUXILIAR
#define C_AUXILIAR

#include <utility> 

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>

#ifndef SPTWIN

#include <climits>
#include <limits.h>
#include <math.h>
#include <cmath>
#include <bits/stdc++.h>

#endif

#define DOUBLE_MAX 1.79e308

#define DOUBLE_MIN -DOUBLE_MAX

#define PRECISAO_COMPARACAO_DOUBLE 0.000001

#define PI 3.141592653589793

#define LISTA_TIPO_ELEMENTAR(m) m(double) m(int) m(bool) m(std::string) m(Periodo) 


static bool doubleCompara(const double a, const double b) {
	if ((b * (1.0 - PRECISAO_COMPARACAO_DOUBLE) <= a) && (b * (1.0 + PRECISAO_COMPARACAO_DOUBLE) >= a))
		return true;
	else
		return false;
}; // static bool doubleCompara(double a, double b) {

static bool doubleCompara(const double a_precisao, const double a, const double b) {
	if ((b * (1.0 - a_precisao) <= a) && (b * (1.0 + a_precisao) >= a))
		return true;
	else
		return false;
}; // static bool doubleCompara(double a, double b) {

//
// Funções GERAIS para comparar char e strings INCASESENSITIVE
//

static bool chrCompara(unsigned char a, unsigned char b) {
	if (tolower(a) == tolower(b))
		return true;
	else
		return false;
}; // static bool chrCompara(unsigned char a, unsigned char b) {

static bool strCompara(std::string const& a, std::string const& b) {
	if (a.length() == b.length()) {
		for (int i = 0; i < int(a.length()); i++) {
			if (!chrCompara(a[i], b[i]))
				return false;
		} // for (int i = 0; i < int(a.length()); i++) {
		return true;
	} // if (a.length() == b.length()) {
	else
		return false;

}; // static bool strCompara(std::string const& a, std::string const& b) {

template<typename TValue>
static bool vectorCompara(std::vector<TValue> a_lista1, std::vector<TValue> a_lista2) {

	if (a_lista1.size() != a_lista2.size())
		return false;

	for (int i = 0; i < a_lista1.size(); i++) {
		if (a_lista1.at(i) != a_lista2.at(i))
			return false;
	}
	return true;
}

static std::string alterar_CHARc_na_STRINGs_por_CHARn(const unsigned char c, std::string s, const unsigned char n) {

	for (int i = 0; i < int(s.length()); i++) {
		if (chrCompara(s[i], c))
			s[i] = n;
	} // for (int i = 0; i < int(s.length()); i++) {

	return s;
};

static int contaStringNoVetor(const std::string a_string, const std::vector<std::string>& a_vetor) {

	int contador = 0;

	for (int i = int(a_vetor.size() - 1); i >= 0; i--) {
		if (strCompara(a_string, a_vetor.at(i)))
			contador++;
	} // for (int i = int(a_vetor.size() - 1); i >= 0; i--) {

	return contador;

} // static bool findStringNoVetor(const std::string a_string, const std::vector<std::string> &a_vetor) {

static bool findStringNoVetor(const std::string a_string, const std::vector<std::string> &a_vetor) {

	for (int i = int(a_vetor.size() - 1); i >= 0; i--) {
		if (strCompara(a_string, a_vetor.at(i)))
			return true;
	} // for (int i = int(a_vetor.size() - 1); i >= 0; i--) {

	return false;

} // static bool findStringNoVetor(const std::string a_string, const std::vector<std::string> &a_vetor) {

static int findStringNoVetorReturnPos(const std::string a_string, const std::vector<std::string>& a_vetor) {

	for (int i = int(a_vetor.size() - 1); i >= 0; i--) {
		if (strCompara(a_string, a_vetor.at(i)))
			return i;
	} // for (int i = int(a_vetor.size() - 1); i >= 0; i--) {

	return -1;

} // static bool findStringNoVetorReturnPos(const std::string a_string, const std::vector<std::string> &a_vetor) {


static std::vector<bool> findStringsNoVetor(const std::vector<std::string> a_strings, const std::vector<std::string> &a_vetor) {

	std::vector<bool> comparacao(a_strings.size(), false);

	for (int i = 0; i < int(a_vetor.size()); i++) {
		for (int s = 0; s < int(a_strings.size()); s++) {
			if (!comparacao.at(s)) {
				if (strCompara(a_strings.at(s), a_vetor.at(i)))
					comparacao.at(s) = true;
			} // if (!comparacao.at(s)) {
		} // for (int s = 0; s < int(a_strings.size()); s++) {
	} // for (int i = 0; i < int(a_vetor.size()); i++) {

	return comparacao;

} // static bool findStringNoVetor(const std::string a_string, const std::vector<std::string> &a_vetor) {


//
// Funções GERAIS para obter Tipos e strings INCASESENSITIVE
//

static int getintFromChar(const char * a_char) {
	if (strCompara(std::string(a_char), std::string("max")))
		return INT_MAX;
	else if (strCompara(std::string(a_char), std::string("min")))
		return INT_MIN;
	else if (strCompara(std::string(a_char), std::string("inf")))
		return INT_MAX;
	else if (strCompara(std::string(a_char), std::string("-inf")))
		return INT_MIN;
	else
		return std::atoi(a_char);

}; // static int getintFromChar(const char * a_char) {


template<typename TValue>
static TValue getFromString(const TValue a_valor, const std::string a_str) { 
	throw std::invalid_argument("getFromChar(" + a_str + "): Nao implementado.");
};

static int getFromString(const int a_valor, const std::string a_str) {
	return getintFromChar(a_str.c_str());
};

static std::vector<int> getFromString(const std::vector<int> a_valor, const std::string a_str) {
	return std::vector<int>(1, getintFromChar(a_str.c_str()));
};


static int getFromChar(const int a_int, const char * a_char) { return getintFromChar(a_char); };

static std::string getString(int a_int) { 
	
	if (a_int == INT_MAX)
		return "inf";
	else if (a_int == INT_MIN)
		return "-inf";

	return std::to_string(a_int);
}

static std::string getStringTipo(int a_int) { return "int"; }
static std::string getFullString(int a_int) { return getStringTipo(a_int) + "_" + getString(a_int); }

static double getdoubleFromChar(const char * a_char) {
	if (strCompara(std::string(a_char), std::string("max")))
		return DOUBLE_MAX; 
	else if (strCompara(std::string(a_char), std::string("min")))
		return DOUBLE_MIN;
	else if (strCompara(std::string(a_char), std::string("inf")))
		return DOUBLE_MAX;
	else if (strCompara(std::string(a_char), std::string("-inf")))
		return DOUBLE_MIN;
	else
		return std::atof(a_char);
}; // static double getdoubleFromChar(const char * a_char) {

static double getFromChar(const double a_double, const char * a_char) { return getdoubleFromChar(a_char); };

static std::string getString(const double a_double) {
	if (isnan(a_double))
		return "NAN";
	if (a_double >= DOUBLE_MAX * 0.99)
		return "inf";
	else if (a_double <= DOUBLE_MIN * 0.99)
		return "-inf";
	std::ostringstream out;
	double intpart;
	const double fractpart = abs(std::modf(a_double, &intpart));
	if (doubleCompara(1e-12, fractpart, 0.0)) {
		out.precision(0);
		out << std::fixed << a_double;
	}
	else {
		intpart = double(abs(intpart));
		if (intpart > 0.0) {
			if (intpart < 1e9) {
				out.precision(12);
				out << std::fixed << a_double;
			}
			else if (1e9 <= intpart) {
				out.precision(15);
				out << std::scientific << a_double;
			}
		}
		else {
			if (fractpart > 1e-9) {
				out.precision(12);
				out << std::fixed << a_double;
			}
			else if (1e-9 >= fractpart) {
				out.precision(15);
				out << std::scientific << a_double;
			}
		}
	}
	return out.str();

} // static std::string getString(double a_double) {
static std::string getStringTipo(double a_double) { return "double"; }
static std::string getFullString(double a_double) { return getStringTipo(a_double) + "_" + getString(a_double); }

static bool getboolFromChar(const char * a_char) {
	if (strCompara(std::string(a_char), std::string("sim")))
		return true;
	else if (strCompara(std::string(a_char), std::string("nao")))
		return false;
	else if (strCompara(std::string(a_char), std::string("true")))
		return true;
	else if (strCompara(std::string(a_char), std::string("1")))
		return true;
	else if (strCompara(std::string(a_char), std::string("verdadeiro")))
		return true;
	else if (strCompara(std::string(a_char), std::string("max")))
		return true;
	else
		return false;
}; // static bool getboolFromChar(const char * a_char) {

static bool getFromChar(const bool a_bool, const char * a_char) { return getboolFromChar(a_char); };

static std::string getString(bool a_bool) {
	if (a_bool)
		return std::string("1");
	else
		return std::string("0");
}

static std::string getStringTipo(bool a_bool) { return "bool"; }
static std::string getFullString(bool a_bool) { return getStringTipo(a_bool) + "_" + getString(a_bool); }

static std::string getstringFromChar(const char * a_char) {
	if (strCompara(std::string(a_char), std::string("")))
		return "";
	else if (strCompara(std::string(a_char), std::string("min")))
		return std::string(100, char(0));
	else if (strCompara(std::string(a_char), std::string("max")))
		return std::string(100, char(255));

	return std::string(a_char);
};

static std::string getString(const char * a_char) { return std::string(a_char); }
static std::string getString(const std::string a_string) { return a_string; }

static std::string getString(const double* a_valor) { return std::string("double*"); }

static std::string getString(const std::vector<int> a_vetor) { return std::string("vector<int>"); }
static std::string getString(const std::vector<double> a_vetor) { return std::string("vector<double>"); }
static std::string getString(const std::vector<std::vector<double>> a_vetor) { return std::string("vector<vector<double>>"); }

template<typename TipoValor>
static std::string getString(TipoValor a_tipoValor) { return a_tipoValor.str(); }

template<typename TipoValor>
static std::string getStringFromLista(const std::vector<TipoValor> &a_lista, const std::string a_delimitador, bool a_full_string) {
	
	std::string retorno;

	for (int i = 0; i < int(a_lista.size()); i++) {

		std::string delimitador = a_delimitador;

		if (i == int(a_lista.size()) - 1)
			delimitador = "";

		if (a_full_string)
			retorno = retorno + getFullString(a_lista.at(i)) + delimitador;
		else
			retorno = retorno + getString(a_lista.at(i)) + delimitador;
	}

	return retorno;

};

static void strNormalizada(std::string &a_string){
	
	if (a_string.empty())
		return;

	if (a_string[a_string.size() - 1] == '\r')
		a_string.erase(a_string.size() - 1);
} // string EntradaSaidaDados::strNormalizada(){


static bool lerRegistro(std::string a_registro, std::string a_registroArquivo, std::string a_registroLer) {
	
	bool lerRegistro = false;

	if (a_registro == a_registroArquivo) {
		if (a_registroLer.empty())
			lerRegistro = true;
		if (a_registroLer == a_registro)
			lerRegistro = true;
	}
	return lerRegistro;

} // static bool lerRegistro(std::string a_registro, std::string a_registroArquivo, std::string a_registroLer) {

#ifdef SPTWIN

#include <windows.h>

static bool deletarArquivo(std::string a_caminhoDiretorio) {

	return DeleteFileA(a_caminhoDiretorio.c_str());

};

static bool criarDiretorio(std::string a_caminhoDiretorio) {

	std::string diretorio = "";

	int pos = 0;
	while (pos != std::string::npos) {

		pos = int(a_caminhoDiretorio.find("//"));

		diretorio += a_caminhoDiretorio.substr(0, pos) + "//";

		strNormalizada(diretorio);

		if (diretorio != "")
			CreateDirectoryA(diretorio.c_str(), nullptr);

		a_caminhoDiretorio = a_caminhoDiretorio.substr(pos + 1, a_caminhoDiretorio.length());

		strNormalizada(a_caminhoDiretorio);

		if (a_caminhoDiretorio == "")
			break;

	} // while (pos != string::npos) {

	return true;

}; // void criarDiretorio(std::string a_caminhoDiretorio) {

static void aguardarTempo(double a_tempo) { Sleep(DWORD(a_tempo)); };

static void aguardarTempo(int a_tempo) { Sleep(a_tempo); };

#endif

#ifndef SPTWIN

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <bits/stdc++.h>

static bool deletarArquivo(std::string a_caminhoDiretorio) {

	return remove(a_caminhoDiretorio.c_str());

};

static bool criarDiretorio(std::string a_caminhoDiretorio) {

	int status;
	status = system(std::string("mkdir -p " + a_caminhoDiretorio).c_str()); // Creating a directory
	if (status == -1)
		std::cout << "Nao foi possivel criar o diretorio " << " erro: " << strerror(errno) << std::endl;
	else
		return true;

	return false;

}; // void criarDiretorio(std::string a_caminhoDiretorio) {

static void aguardarTempo(double a_tempo) { aguardarTempo(int(a_tempo)); };

static void aguardarTempo(int a_tempo) { sleep(a_tempo); };

#endif

#endif // #define C_AUXILIAR