#ifndef SMART_ENUPLA
#define SMART_ENUPLA

#include "C_Auxiliar.h"
#include "C_SmartEnum.h"
#include "C_Periodo.h"

#include <vector>
#include <string>
#include <algorithm>

#include <cstdlib>

typedef std::string string;


// ----------------------------------------------------
// 
// Declaração Classe SmartEnupla (utilizada em AttVetor)
//
// ----------------------------------------------------


//
// Declaração Geral
//
template<class TipoIterador, class TipoValor>
class SmartEnupla {
public:

	SmartEnupla() {};
	SmartEnupla(const int a_vlr_alocar) { smartEnupla.reserve(a_vlr_alocar); };
	SmartEnupla(const bool a_inicializar_from_0, const TipoIterador a_iterInicial, const std::vector<TipoValor>& a_vetorInicial) {
		if (a_inicializar_from_0) {
			if (a_iterInicial < TipoIterador(0))
				throw std::invalid_argument("O iterador Inicial deve ser maior ou igual a " + getString(TipoIterador(0)));
		}
		else {
			if (a_iterInicial < getFromChar(TipoIterador(), "min"))
				throw std::invalid_argument("O iterador Inicial deve ser maior ou igual a " + getString(getFromChar(TipoIterador(), "min")));
		}
		if (a_vetorInicial.size() == 0)
			throw std::invalid_argument("O vetor inicial deve possuir algum elemento.");
		iteradorInicial = a_iterInicial;
		smartEnupla = a_vetorInicial;
		iteradorFinal = TipoIterador(iteradorInicial + int(smartEnupla.size()) - 1);
	};
	SmartEnupla(const TipoIterador a_iterInicial, const std::vector<TipoValor>& a_vetorInicial) {
		if (a_iterInicial < getFromChar(TipoIterador(), "min"))
			throw std::invalid_argument("O iterador Inicial deve ser maior ou igual a " + getString(getFromChar(TipoIterador(), "min")));
		else if (a_vetorInicial.size() == 0)
			throw std::invalid_argument("O vetor inicial deve possuir algum elemento.");
		iteradorInicial = a_iterInicial;
		smartEnupla = a_vetorInicial;
		iteradorFinal = TipoIterador(iteradorInicial + int(smartEnupla.size()) - 1);
	};

	int size() const { return int(smartEnupla.size()); };

	TipoIterador getIteradorInicial()const {
		try {
			if (size() == 0)
				throw std::invalid_argument("Iterador inicial nao definido.");
			return iteradorInicial;
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getIteradorInicial(): \n" + std::string(erro.what())); }
	}; // TipoIterador getIteradorInicial()const {

	TipoIterador getIteradorFinal()const {
		try {
			if (size() == 0)
				throw std::invalid_argument("Iterador final nao definido.");
			return iteradorFinal;
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getIteradorFinal(): \n" + std::string(erro.what())); }
	}; // TipoIterador getIteradorFinal()const {

	void addElemento(TipoValor a_vlr) {
		try {
			if (size() == 0)
				iteradorInicial = getFromChar(TipoIterador(), "min");

			smartEnupla.push_back(a_vlr);
			iteradorFinal = TipoIterador(int(smartEnupla.size()));
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::addElemento(" + getString(a_vlr) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoValor a_vlr) {

	void addElemento(TipoIterador a_iter, TipoValor a_vlr) {
		try {

			if (size() == 0) {
				if (a_iter < getFromChar(TipoIterador(), "min"))
					throw std::invalid_argument("O iterador Inicial deve ser maior ou igual a " + getFullString(getFromChar(TipoIterador(), "min")));
				else if (a_iter > getFromChar(TipoIterador(), "max"))
					throw std::invalid_argument("O iterador Inicial deve ser menor ou igual a " + getFullString(getFromChar(TipoIterador(), "max")));
				iteradorInicial = a_iter;
				iteradorFinal = a_iter;
				smartEnupla.push_back(a_vlr);
			}
			else if (getIteradorFinal() == (a_iter - 1)) {
				if (a_iter > getFromChar(TipoIterador(), "max"))
					throw std::invalid_argument("O iterador Inicial deve ser menor ou igual a " + getFullString(getFromChar(TipoIterador(), "max")));
				iteradorFinal = a_iter;
				smartEnupla.push_back(a_vlr);
			}
			else if (getIteradorInicial() == (a_iter + 1)) {
				if (a_iter < getFromChar(TipoIterador(), "min"))
					throw std::invalid_argument("O iterador Inicial deve ser maior ou igual a " + getFullString(getFromChar(TipoIterador(), "min")));
				iteradorInicial = a_iter;
				smartEnupla.insert(smartEnupla.begin(), a_vlr);
			}
			else
				throw std::invalid_argument("Argumento Iterador - " + getFullString(a_iter) + " - nao sequencial ao Iterador inicial: " + getFullString(getIteradorInicial()) +  " ou Iterador final: " + getFullString(getIteradorFinal()) + " da Smartenupla.");

		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::addElemento(" + getString(a_iter) + "," + getString(a_vlr) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iter, TipoValor a_vlr) {

	void adddElemento(TipoIterador a_iter, TipoValor a_vlr) {
		try {

			return addElemento(a_iter, a_vlr);

		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::adddElemento(" + getString(a_iter) + "," + getString(a_vlr) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iter, TipoValor a_vlr) {

	void adddElementoVoid(TipoIterador a_iter) {
		try {

			return adddElemento(a_iter, TipoValor());

		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::adddElementoVoid(" + getString(a_iter) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iter, TipoValor a_vlr) {

	void setElemento(TipoIterador a_iter, TipoValor a_vlr) {
		try {
			smartEnupla.at(getIndice(a_iter)) = a_vlr;
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::setElemento(" + getString(a_iter) + "," + getString(a_vlr) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iter, TipoValor a_vlr) {

	void settElemento(TipoIterador a_iter, TipoValor a_vlr) {
		try {
			smartEnupla.at(getIndice(a_iter)) = a_vlr;
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::settElemento(" + getString(a_iter) + "," + getString(a_vlr) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iter, TipoValor a_vlr) {

	void setElementoFromStr(TipoIterador a_iter, std::string a_vlr) {
		try {
			smartEnupla.at(getIndice(a_iter)) = getFromString(TipoValor(), a_vlr);
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::setElemento(" + getString(a_iter) + "," + getString(a_vlr) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iter, TipoValor a_vlr) {

	TipoValor getElemento(TipoIterador a_iter)const {
		try {
			return smartEnupla.at(getIndice(a_iter));
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getElemento(" + getString(a_iter) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iter, TipoValor a_vlr) {

	TipoValor gettElemento(TipoIterador a_iter)const {
		try {
			return smartEnupla.at(getIndice(a_iter));
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::gettElemento(" + getString(a_iter) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iter, TipoValor a_vlr) {

	TipoValor getTipoElemento()const {
		try {
			return TipoValor();
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getTipoElemento(): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iter, TipoValor a_vlr) {

	TipoValor at(TipoIterador a_iter)const {
		try {
			return smartEnupla.at(getIndice(a_iter));
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::at(" + getFullString(a_iter) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iter, TipoValor a_vlr) {

	TipoValor att(TipoIterador &a_iter)const {
		try {
			return smartEnupla.at(getIndice(a_iter));
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::att(" + getFullString(a_iter) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iter, TipoValor a_vlr) {

	TipoValor& at(TipoIterador a_iter) {
		try {
			return smartEnupla.at(getIndice(a_iter));
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::at(" + getFullString(a_iter) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iter, TipoValor a_vlr) {

	TipoValor& att(TipoIterador &a_iter) {
		try {
			return smartEnupla.at(getIndice(a_iter));
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::att(" + getFullString(a_iter) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iter, TipoValor a_vlr) {

	void alterarValorSeAlterarIterador(const TipoIterador a_iter, const TipoValor a_vlr) {
		try {

			if ((getIteradorInicial() <= a_iter) && (a_iter <= getIteradorFinal()))
				return;
			
			throw std::invalid_argument("Metodo nao valido para TipoIterador e TipoValor");


		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::alterarValorSeAlterarIterador(" + getFullString(a_iter) + ",a_vlr) : \n" + std::string(erro.what())); }
	}

	void random_shuffle() { std::random_shuffle(smartEnupla.begin(), smartEnupla.end()); };


	std::vector<TipoValor> getElementos() const {
		try {
			if (size() == 0)
				throw std::invalid_argument("Nao ha elementos na SmartEnupla");
			return smartEnupla;
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getElementos(): \n" + std::string(erro.what())); }
	};

	std::vector<TipoValor>& getElementos_referencia() {
		try {
			if (size() == 0)
				throw std::invalid_argument("Nao ha elementos na SmartEnupla");
			return smartEnupla;
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getElementos(): \n" + std::string(erro.what())); }
	};

	void incrementarIterador(TipoIterador& a_iter, const int a_inteiro) const {
		try {
			if (a_inteiro < 0)
				throw std::invalid_argument("Inteiro invalido.");
			for (int i = 1; i <= a_inteiro; i++)
				incrementarIterador(a_iter);
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::incrementaIterador(" + getString(a_iter) + "," + getString(a_inteiro) + "): \n" + std::string(erro.what())); }
	} // void incrementarIterador(Periodo &a_period, const int a_inteiro) const {

	void incrementarIteradorMinimo(TipoIterador& a_iter) const { incrementarIterador(a_iter); }

	void incrementarIterador(TipoIterador& a_iter) const {
		try {
			if (size() == 0)
				throw std::invalid_argument("Nao ha elementos na SmartEnupla.");

			if ((getIteradorInicial() <= a_iter) && (getIteradorFinal() >= a_iter))
				a_iter++;
			else
				throw std::invalid_argument("Iterador invalido.");
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::incrementaIterador(" + getString(a_iter) + "): \n" + std::string(erro.what())); }
	} // void operator++(Periodo &a_period) const {

	void decrementarIteradorMinimo(TipoIterador& a_iter) const { decrementarIterador(a_iter); }

	void decrementarIterador(TipoIterador& a_iter) const {
		try {
			if (size() == 0)
				throw std::invalid_argument("Nao ha elementos na SmartEnupla.");
			if ((getIteradorInicial() <= a_iter) && (getIteradorFinal() >= a_iter))
				a_iter--;
			else
				throw std::invalid_argument("Iterador invalido.");
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::decrementarIterador(" + getString(a_iter) + "): \n" + std::string(erro.what())); }
	} // void operator++(Periodo &a_period) const {

	bool isProximoIterador(const TipoIterador a_iter) const{
		try {
			if (TipoIterador(getIteradorFinal() + 1) == a_iter)
				return true;
			return false;
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::isProximoIterador(" + getString(a_iter) + "): \n" + std::string(erro.what())); }
	}

	bool isIteradorValido(const TipoIterador a_iter) const{
		try {
			if (size() == 0)
				return false;
			if ((a_iter < getIteradorInicial()) || (getIteradorFinal() < a_iter))
				return false;
			return true;
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::isIteradorValido(" + getString(a_iter) + "): \n" + std::string(erro.what())); }
	}


	std::vector<TipoIterador> getIteradores(const TipoIterador a_iter)const {
		try {

			return std::vector<TipoIterador>();

		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getIteradores(" + getFullString(a_iter) + "): \n" + std::string(erro.what())); }
	} // SmartEnupla<Periodo, bool> getIteradores(const Periodo a_period)const {

	std::string str() { return "SmartEnupla<iterador,valor>"; }

	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iters, bool& a_cabecalho_incluir, const std::string a_string, bool a_chamada_inicial, int &a_linAdded, std::vector<std::vector<std::string>>& a_matriz_interna) {
		a_linAdded = -1;
		return std::vector<std::vector<std::string>>(); 
	};
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iters, bool& a_cabecalho_incluir, const char* a_char, bool a_chamada_inicial, int &a_linAdded, std::vector<std::vector<std::string>>& a_matriz_interna) {
		a_linAdded = -1;
		return std::vector<std::vector<std::string>>(); 
	};
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iters, bool& a_cabecalho_incluir, const bool a_bool, bool a_chamada_inicial, int &a_linAdded, std::vector<std::vector<std::string>>& a_matriz_interna) {
		a_linAdded = -1;
		return std::vector<std::vector<std::string>>(); 
	};
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iters, bool& a_cabecalho_incluir, const double a_double, bool a_chamada_inicial, int &a_linAdded, std::vector<std::vector<std::string>>& a_matriz_interna) {
		a_linAdded = -1;
		return std::vector<std::vector<std::string>>(); 
	};
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iters, bool& a_cabecalho_incluir, const int a_int, bool a_chamada_inicial, int &a_linAdded, std::vector<std::vector<std::string>>& a_matriz_interna) {
		a_linAdded = -1;
		return std::vector<std::vector<std::string>>(); 
	};
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iters, bool& a_cabecalho_incluir, const std::vector<int> a_vetor, bool a_chamada_inicial, int &a_linAdded, std::vector<std::vector<std::string>>& a_matriz_interna) {
		a_linAdded = -1;
		return std::vector<std::vector<std::string>>(); 
	};
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iters, bool& a_cabecalho_incluir, const std::vector<double> a_vetor, bool a_chamada_inicial, int &a_linAdded, std::vector<std::vector<std::string>>& a_matriz_interna) {
		a_linAdded = -1;
		return std::vector<std::vector<std::string>>(); 
	};
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iters, bool& a_cabecalho_incluir, const std::vector<std::vector<int>> a_vetor, bool a_chamada_inicial, int &a_linAdded, std::vector<std::vector<std::string>>& a_matriz_interna) {
		a_linAdded = -1;
		return std::vector<std::vector<std::string>>(); 
	};
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iters, bool& a_cabecalho_incluir, const std::vector<std::vector<double>> a_vetor, bool a_chamada_inicial, int &a_linAdded, std::vector<std::vector<std::string>>& a_matriz_interna) {
		a_linAdded = -1;
		return std::vector<std::vector<std::string>>(); 
	};

	template<typename Iterador, typename Valor>
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iters, const SmartEnupla<Iterador, Valor>& a_enupla) {
		std::vector<std::vector<std::string>> matriz_interna;
		bool isUltimoCabecalhoIncluso = false;
		int linAdded = 0;
		if (a_incluir_iters)
			return getDadosAsString(100, isUltimoCabecalhoIncluso, a_enupla, true, linAdded, matriz_interna);
		else
			return getDadosAsString(-1, isUltimoCabecalhoIncluso, a_enupla, true, linAdded, matriz_interna);
	}

	template<typename Iterador, typename Valor>
	static std::vector<std::vector<std::string>> getDadosAsString(int a_nro_iters_incluir, const SmartEnupla<Iterador, Valor>& a_enupla) {
		int linAdded = 0;
		std::vector<std::vector<std::string>> matriz_interna;
		bool isUltimoCabecalhoIncluso = false;
		return getDadosAsString(a_nro_iters_incluir, isUltimoCabecalhoIncluso, a_enupla, true, linAdded, matriz_interna);
	}

	template<typename Iterador, typename Valor>
	static std::vector<std::vector<std::string>> getDadosAsString(int a_nro_iters_incluir, bool &a_isUltimoCabecalhoIncluso, const SmartEnupla<Iterador, Valor>& a_enupla, bool a_chamada_inicial, int &a_linAdded, std::vector<std::vector<std::string>> &a_matriz_interna) {

		try {

			if (a_enupla.size() == 0) {
				a_linAdded = 0;
				return std::vector<std::vector<std::string>>();
			}

			const Iterador iteradorInicial = a_enupla.getIteradorInicial();
			const Iterador iteradorFinal = a_enupla.getIteradorFinal();

			int nro_iters_incluir = a_nro_iters_incluir;

			if (nro_iters_incluir > 0)
				nro_iters_incluir--;

			for (Iterador iterador = iteradorInicial; iterador <= iteradorFinal; a_enupla.incrementarIterador(iterador)) {

				SmartEnupla<int, int>::getDadosAsString(nro_iters_incluir, a_isUltimoCabecalhoIncluso, a_enupla.at(iterador), false, a_linAdded, a_matriz_interna);

				if ((a_linAdded > 0) && (a_nro_iters_incluir > 0)) {

					bool preencher_todas_linhas = true;

					// Verifica se matriz interna não é quadrada
					if (a_matriz_interna.at(0).size() != a_matriz_interna.at(a_matriz_interna.size() - 1).size())
						preencher_todas_linhas = false;

					for (int l = int(a_matriz_interna.size()) - 1; l >= 0; l--) {
						if (preencher_todas_linhas)
							a_matriz_interna.at(l).insert(a_matriz_interna.at(l).begin(), getString(iterador));
						else {
							if (l == (a_matriz_interna.size() - 1))
								a_matriz_interna.at(l).insert(a_matriz_interna.at(l).begin(), getString(iterador));
							else if ((a_matriz_interna.at(l).size() + 1) == a_matriz_interna.at(l + 1).size())
								a_matriz_interna.at(l).insert(a_matriz_interna.at(l).begin(), getString(iterador));
							else
								break;
						}
					}

					if (a_isUltimoCabecalhoIncluso)
						a_matriz_interna.at(0).at(0) = getStringTipo(iterador);

				} // if (a_linAdded > 0) {

				if (a_linAdded == -1)
					break;

			} // for (Iterador iterador = iteradorInicial; iterador <= iteradorFinal; incrementarIterador(iterador)) {

			if (a_linAdded == -1) {

				a_linAdded = 1;
				bool is_cabecalho_adicionado = false;

				if ((a_nro_iters_incluir > -1) && (a_matriz_interna.size() == 0)) {
					is_cabecalho_adicionado = true;
					a_isUltimoCabecalhoIncluso = true;
					a_linAdded = 2;
					a_matriz_interna = std::vector<std::vector<std::string>>(2, std::vector<std::string>(a_enupla.size(), ""));
				}
				else
					a_matriz_interna.push_back(std::vector<std::string>(a_enupla.size(), ""));

				const int l = int(a_matriz_interna.size()) - 1;

				int i = 0;
				for (Iterador iterador = iteradorInicial; iterador <= iteradorFinal; a_enupla.incrementarIterador(iterador)) {

					if (is_cabecalho_adicionado)
						a_matriz_interna.at(l - 1).at(i) = getFullString(iterador);

					a_matriz_interna.at(l).at(i) = getString(a_enupla.at(iterador));

					i++;

				} // for (Iterador iterador = iteradorInicial; iterador <= iteradorFinal; incrementarIterador(iterador)) {

				if (a_chamada_inicial)
					return a_matriz_interna;
				else
					return std::vector<std::vector<std::string>>();

			} // if (a_linAdded == -1) {


			if (a_chamada_inicial)
				return a_matriz_interna;
			else
				return std::vector<std::vector<std::string>>();



		} // try

		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getDadosAsString(" + getFullString(a_nro_iters_incluir) + ",a_enupla," + getFullString(a_chamada_inicial) + "): \n" + std::string(erro.what())); }

	} // std::vector<std::vector<std::string>> SmartEnupla::getDadosAsString(const bool a_incluir_cabecalho){


	template<typename Iterador, typename Valor>
	static std::vector<std::vector<std::string>> getDadosAsString(int a_nro_iters_incluir, const SmartEnupla<Iterador, Valor>& a_enupla, bool a_chamada_inicial) {

		try {

			if (a_enupla.size() == 0)
				return std::vector<std::vector<std::string>>();

			const Iterador iteradorInicial = a_enupla.getIteradorInicial();
			const Iterador iteradorFinal = a_enupla.getIteradorFinal();

			std::vector<std::vector<std::string>> matriz_retorno;

			// Ultima camada da SmarEnupla
			if (SmartEnupla<int, int>::getDadosAsString(a_nro_iters_incluir, a_enupla.at(a_enupla.getIteradorInicial()), false).size() == 0) {

				if (a_nro_iters_incluir > -1)
					matriz_retorno = std::vector<std::vector<std::string>>(2, std::vector<std::string>(a_enupla.size(), ""));
				else
					matriz_retorno = std::vector<std::vector<std::string>>(1, std::vector<std::string>(a_enupla.size(), ""));

				int i = 0;
				for (Iterador iterador = iteradorInicial; iterador <= iteradorFinal; a_enupla.incrementarIterador(iterador)) {

					if (a_nro_iters_incluir > -1) {
						matriz_retorno.at(0).at(i) = getFullString(iterador);
						matriz_retorno.at(1).at(i) = getString(a_enupla.at(iterador));
					} // if (a_incluir_cabecalho) {

					else
						matriz_retorno.at(0).at(i) = getString(a_enupla.at(iterador));

					i++;

				} // for (Iterador iterador = iteradorInicial; iterador <= iteradorFinal; incrementarIterador(iterador)) {

				return matriz_retorno;

			} // if (!isSmartEnupla(TipoValor()){

			else {

				int nro_iters_incluir = a_nro_iters_incluir;

				if (nro_iters_incluir > 0)
					nro_iters_incluir--;

				for (Iterador iterador = iteradorInicial; iterador <= iteradorFinal; a_enupla.incrementarIterador(iterador)) {

					const std::vector<std::vector<std::string>> matriz_retornada = SmartEnupla<int, int>::getDadosAsString(nro_iters_incluir, a_enupla.at(iterador), false);

					if (matriz_retornada.size() > 0) {

						for (int l = 0; l < matriz_retornada.size(); l++) {
							if (matriz_retornada.at(l).size() > 0) {

								if (a_nro_iters_incluir > 0) {

									if (matriz_retorno.size() == 0) {
										matriz_retorno.push_back(std::vector<std::string>(matriz_retornada.at(l).size() + 1, ""));
										matriz_retorno.at(0).at(0) = getStringTipo(iterador);
										for (int c = 0; c < matriz_retornada.at(l).size(); c++)
											matriz_retorno.at(matriz_retorno.size() - 1).at(c + 1) = matriz_retornada.at(l).at(c);
									}

									else if ((matriz_retorno.size() > 0) && (l == 0)) {
										if (matriz_retornada.at(0).size() > matriz_retorno.at(0).size() - 1) {
											matriz_retorno.at(0) = std::vector<std::string>(matriz_retornada.at(0).size() + 1, "");
											matriz_retorno.at(0).at(0) = getStringTipo(iterador);
											for (int c = 0; c < matriz_retornada.at(0).size(); c++)
												matriz_retorno.at(0).at(c + 1) = matriz_retornada.at(0).at(c);
										}
									}

									else if ((matriz_retorno.size() > 0) && (l > 0)) {
										matriz_retorno.push_back(std::vector<std::string>(matriz_retornada.at(l).size() + 1, ""));
										matriz_retorno.at(matriz_retorno.size() - 1).at(0) = getString(iterador);
										for (int c = 0; c < matriz_retornada.at(l).size(); c++)
											matriz_retorno.at(matriz_retorno.size() - 1).at(c + 1) = matriz_retornada.at(l).at(c);
									}

								} // if (a_nro_iters_incluir > -1) {

								else {
									matriz_retorno.push_back(std::vector<std::string>(matriz_retornada.at(l).size(), ""));

									for (int c = 0; c < matriz_retornada.at(l).size(); c++)
										matriz_retorno.at(matriz_retorno.size() - 1).at(c) = matriz_retornada.at(l).at(c);

								}

							} // if (matriz_retornada.at(l).size() > 0) {
						} // for (int l = 0; l < matriz_retornada.size(); l++) {

					} // if (matriz_retornada.size() > 0){

					else {

						if (matriz_retorno.size() > 0) {
							matriz_retorno.push_back(std::vector<std::string>(matriz_retorno.at(matriz_retorno.size() - 1).size(), "NAN"));
							matriz_retorno.at(matriz_retorno.size() - 1).at(0) = getString(iterador);
						}
					}

				} // for (Iterador iterador = iteradorInicial; iterador <= iteradorFinal; incrementarIterador(iterador)) {

				return matriz_retorno;

			} // else {

			return matriz_retorno;

		} // try

		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getDadosAsString(" + getFullString(a_nro_iters_incluir) + ",a_enupla," + getFullString(a_chamada_inicial) + "): \n" + std::string(erro.what())); }

	} // std::vector<std::vector<std::string>> SmartEnupla::getDadosAsString(const bool a_incluir_cabecalho){

private:

	int getIndice(TipoIterador a_iter) const {
		try {
			return int(a_iter - getIteradorInicial());
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getIndice(" + getString(a_iter) + "): \n" + std::string(erro.what())); }
	}; // int getIndice(TipoIterador a_iter) {

	TipoIterador iteradorInicial;
	TipoIterador iteradorFinal;

	std::vector<TipoValor> smartEnupla;

}; // class SmartEnupla {


struct StructPeriod {
public:

private:

	bool empty_struct = true;
	int idx_prev = -2;
	int num_per = 0;
	Periodo per_ini;


public:

	StructPeriod() {};

	StructPeriod(const Periodo a_period, const int a_num_per, const int a_idx_prev, const bool a_empty_struct = false) {
		try {
			if (!a_period.isValido())
				throw std::invalid_argument("Invalid period.");
			else if (a_idx_prev < -1)
				throw std::invalid_argument("Invalid idx prev.");
			else if (a_num_per < 1)
				throw std::invalid_argument("Invalid num per.");
			else if (a_empty_struct) {
				if (a_period.getTipoPeriodo() != TipoPeriodo_minuto)
					throw std::invalid_argument("Only " + getFullString(TipoPeriodo_minuto) + " must be accepted for empty structure.");
			}
			per_ini = a_period;
			idx_prev = a_idx_prev;
			num_per = a_num_per;
			empty_struct = a_empty_struct;
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("StructPeriod(" + getFullString(a_period) + "," + getFullString(a_period) + "," + getFullString(a_num_per) + "," + getFullString(a_idx_prev) + "," + getFullString(a_empty_struct) + "): \n" + std::string(erro.what())); }
	}

	int addNext(const Periodo a_period) {

		// Period at the end
		// return >= 0 if the current structure is started by the period or a sequential period is placed at the end of the current structure
		// return -1 if a new struct is needed for a sequential period at the end
		// return -2 if a new struct is needed for a sparse period at the end
		// return -10 if the new period is infeaseble

		// No initiate structure
		if (!per_ini.isValido()) {
			per_ini = a_period;
			num_per = 1;
			idx_prev = -1;
			empty_struct = false;
			return getIdxEnd();
		}

		const Periodo per_next = Periodo(a_period.getTipoPeriodo(), getPeriodEnd() + 1);
		if (per_next == a_period) {
			if ((empty_struct) || getType() != a_period.getTipoPeriodo())
				return -1;
			else {
				num_per++;
				return getIdxEnd();
			}

		}
		else if (per_next < a_period)
			return -2;

		return -30;

	}; // int addNext(const Periodo a_period) {

	int addPrev(const Periodo a_period) {

		// Period at the begining
		// return >= 0 if the current structure is started by the period or the period is placed at the begining of the current structure
		// return -1 if a new struct is needed for a sequential period at the begining
		// return -2 if a new struct is needed for a sparse period at the begining
		// return -10 if the new period is infeaseble

		// No initiate structure
		if (!per_ini.isValido()) {
			per_ini = a_period;
			num_per = 1;
			idx_prev = -1;
			empty_struct = false;
			return getIdxIni();
		}

		if (empty_struct)
			return -20;

		const Periodo per_prev = Periodo(a_period.getTipoPeriodo(), getPeriodIni()) - 1;
		if (per_prev == a_period) {
			if ((empty_struct) || getType() != a_period.getTipoPeriodo())
				return -1;
			else {
				num_per++;
				per_ini = a_period;
				return getIdxIni();
			}
		}
		else if (a_period + 1 <= getPeriodIni())
			return -2;

		return -10;

	}; // int addPrev(const Periodo a_period) {

	bool isEmpty()const { return empty_struct; };

	TipoPeriodo getType()const { return per_ini.getTipoPeriodo(); };
	
	void setIdxPrev(const int a_idx_prev) {
		if (a_idx_prev < -1)
			throw std::invalid_argument("Invalid idx prev.");
		idx_prev = a_idx_prev;
	};

	void incrIdxPrev(const int a_incr_idx_prev = 1) {
		if (a_incr_idx_prev < 1)
			throw std::invalid_argument("Invalid incr idx prev.");
		idx_prev = idx_prev + a_incr_idx_prev;
	};


	int getIdxIni()const { if (empty_struct) return idx_prev; else return idx_prev + 1; };
	int getIdxEnd()const { if (empty_struct) return idx_prev; else return idx_prev + num_per; };

	int getIdx(Periodo &a_periodo)const {
		if (!a_periodo.isValido()) { throw std::invalid_argument("invalid period"); }
		else if (empty_struct) return -1;
		else if (getType() != a_periodo.getTipoPeriodo()) return -2;
		else if (a_periodo > getPeriodEnd()) return -2;
		for (int num = 1; num <= num_per; num++) {
			if (a_periodo == (getPeriodIni() + num - 1))
				return idx_prev + num;
		}
		return -2;
	};

	bool isIdxValid(const int a_idx)const {
		if (empty_struct) return false;
		else if ((getIdxIni() <= a_idx) && (a_idx <= getIdxEnd()))
			return true;
		return false;
	};

	int getNumPer()const { return num_per; };

	Periodo getPeriodIni()const { return per_ini; };
	Periodo getPeriodEnd()const { return per_ini + num_per - 1; };

	Periodo getPeriod(const int a_idx)const {
		if (a_idx < 0) { throw std::invalid_argument("invalid index"); }
		else if ((!empty_struct) && (isIdxValid(a_idx)))
			return getPeriodIni() + a_idx - getIdxIni();
		return Periodo();
	};

	Periodo getPeriodNext(const int a_idx)const {
		if (a_idx < 0) { throw std::invalid_argument("invalid index"); }
		else if ((!empty_struct) && ((getIdxIni() <= a_idx) && (a_idx < getIdxEnd())))
			return getPeriodIni() + a_idx - getIdxIni() + 1;
		return Periodo();
	};

	Periodo getPeriodPrev(const int a_idx)const {
		if (a_idx < 0) { throw std::invalid_argument("invalid index"); }
		else if ((!empty_struct) && ((getIdxIni() < a_idx) && (a_idx <= getIdxEnd())))
			return getPeriodIni() + a_idx - getIdxIni() - 1;
		return Periodo();
	};

};


//
// Declaração Especializada para Periodo 
//
template<class TipoValor>
class SmartEnupla <Periodo, TipoValor> {
private:

	bool any_empty_struct = false;
	int code = 0;
	std::vector<StructPeriod> list_structPeriod;
	std::vector<TipoValor> vlr;

	int genNewCode() {
		code = std::rand();
		return code;
	}

	int getPosStructIni() const {
		try {
			
			if (any_empty_struct){
				for (int pos = 0; pos < int(list_structPeriod.size()); pos++) {
					if (!list_structPeriod.at(pos).isEmpty())
						return pos;
				}
			}

			return 0;
		}
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getPosStructIni(): \n" + std::string(erro.what())); }
	}

	int getPosStructEnd() const {
		try {

			if (any_empty_struct) {
				for (int pos = int(list_structPeriod.size() - 1); pos >= 0; pos--) {
					if (!list_structPeriod.at(pos).isEmpty())
						return pos;
				}
			}

			return int(list_structPeriod.size() - 1);
		}
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getPosStructEnd(): \n" + std::string(erro.what())); }
	}

	std::vector<int> getPosIdxStruct(Periodo &a_period) const {
		try {
			const std::vector<int> pos_idx = getPosIdxStructIfAny(a_period);

			if (pos_idx.size() == 0)
				throw std::invalid_argument("Nao existem structPeriods definidas para o periodo " + getFullString(a_period));

			return pos_idx;
		}
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getPosIdxStruct(" + getString(a_period) + "): \n" + std::string(erro.what())); }
	}

	std::vector<int> getPosIdxStructIfAny(Periodo &a_period) const {
		try {


			std::vector<int> pos_idx_inPeriod = getPosIdxStructInPeriod(a_period);
			if (pos_idx_inPeriod.size() > 0)
				return pos_idx_inPeriod;

			const int pos_end = int(list_structPeriod.size() - 1);
			for (int pos = 0; pos <= pos_end; pos++) {

				try {
					if (pos < pos_end) {
						if ((list_structPeriod.at(pos).getPeriodIni() <= a_period) && (a_period < list_structPeriod.at(pos + 1).getPeriodIni())) {
							const int idx = list_structPeriod.at(pos).getIdx(a_period);
							if (idx >= -1) {
								a_period.codStructPeriod = code;
								a_period.posStructPeriod = pos;
								a_period.idxStructPeriod = idx;
								return std::vector<int>{pos, idx};
							}
							else
								return std::vector<int>();
						}
					}
					else {
						const int idx = list_structPeriod.at(pos).getIdx(a_period);
						if (idx >= -1) {
							a_period.codStructPeriod = code;
							a_period.posStructPeriod = pos;
							a_period.idxStructPeriod = idx;
							return std::vector<int>{pos, idx};
						}
						else
							return std::vector<int>();
					}
				}
				catch (const std::exception& erro) { throw std::invalid_argument("Except: \n" + std::string(erro.what())); }

			} // for (int pos = 0; pos <= pos_end; pos++) {

			return std::vector<int>();

		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getPosIdxStructIfAny(" + getString(a_period) + "): \n" + std::string(erro.what())); }
	}; // int getIndice(Periodo a_iter, const int a_structPeriod) const {

	std::vector<int> getPosIdxStructInPeriod(Periodo &a_period) const {
		try {

			// return { pos , idx } where: pos >= 0 and idx >=-1 (-1 if structPeriod is empty);

			if (code == a_period.codStructPeriod) {
				if ((a_period.posStructPeriod >= 0) && (a_period.idxStructPeriod >= 0)) {
					if (list_structPeriod.at(a_period.posStructPeriod).isIdxValid(a_period.idxStructPeriod))
						return std::vector<int>{a_period.posStructPeriod, a_period.idxStructPeriod};
				}
				else if ((a_period.posStructPeriod >= 0) && (a_period.idxStructPeriod == -1)) {
					if (list_structPeriod.at(a_period.posStructPeriod).isEmpty())
						return std::vector<int>{a_period.posStructPeriod, a_period.idxStructPeriod};
				}
			} // if (code == a_period.codStructPeriod) {

			return std::vector<int>();

		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getPosIdxStructInPeriod(" + getString(a_period) + "): \n" + std::string(erro.what())); }
	}; // int getIndice(Periodo a_iter, const int a_structPeriod) const {


	void initialization(const int a_alocacao = 0) {
		try {

			if (a_alocacao > 0)
				vlr.reserve(a_alocacao);

			list_structPeriod.reserve(5);

		}
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::initialization(" + getFullString(a_alocacao) + "): \n" + std::string(erro.what())); }
	}; // void initialization(const int a_alocacao) {


	void updateStructIdx() {
		try {

			bool any_empty = false;
			list_structPeriod.at(0).setIdxPrev(-1);
			int num_elem = 0;
			for (int pos = 0; pos < int(list_structPeriod.size()); pos++) {

				if (pos > 0)
					list_structPeriod.at(pos).setIdxPrev(list_structPeriod.at(pos - 1).getIdxEnd());

				if (list_structPeriod.at(pos).isEmpty())
					any_empty = true;
				else
					num_elem += list_structPeriod.at(pos).getNumPer();

			}

			any_empty_struct = any_empty;

			genNewCode();

			if ((list_structPeriod.at(list_structPeriod.size() - 1).getIdxEnd() + 1 != vlr.size()) || (vlr.size() != num_elem))
				throw std::invalid_argument("Error");

		}
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::updateStructIdx(): \n" + std::string(erro.what())); }
	} // void updateStructIdx() {

public:

	SmartEnupla() { initialization(); };

	SmartEnupla(const int a_alocacao) { initialization(a_alocacao); };

	SmartEnupla(const Periodo a_periodInicial, const std::vector<TipoValor> &a_vetorInicial) {

		if (!a_periodInicial.isValido())
			throw std::invalid_argument("O periodo inicial deve ser valido.");
		else if (a_vetorInicial.size() == 0)
			throw std::invalid_argument("O vetor inicial deve possuir algum elemento.");

		initialization();

		genNewCode();

		list_structPeriod.push_back(StructPeriod(a_periodInicial, int(a_vetorInicial.size()), -1));

		vlr = a_vetorInicial;

	}; // SmartEnupla(const Periodo a_periodInicial, const std::vector<TipoValor> a_vetorInicial) {

	template<typename TipoValor2>
	SmartEnupla(const SmartEnupla<Periodo, TipoValor2>& a_smartEnupla_initialization, TipoValor a_vlr_initialization) {

		list_structPeriod = a_smartEnupla_initialization.getListaStructPeriod();
		vlr = std::vector<TipoValor>(a_smartEnupla_initialization.size(), a_vlr_initialization);
		code = a_smartEnupla_initialization.getIteradorInicial().codStructPeriod;

	}; // SmartEnupla(const SmartEnupla<Periodo, TipoValor2> &a_smartEnupla_initialization, TipoValor a_vlr_initialization) {

	int size() const { return int(vlr.size()); };

	Periodo getIteradorInicial()const {
		try {
			if (size() == 0)
				throw std::invalid_argument("Iterador inicial nao definido.");
			const int pos_ini = getPosStructIni();
			Periodo period = list_structPeriod.at(pos_ini).getPeriodIni();
			period.codStructPeriod = code;
			period.posStructPeriod = pos_ini;
			period.idxStructPeriod = list_structPeriod.at(pos_ini).getIdxIni();
			return period;
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getIteradorInicial(): \n" + std::string(erro.what())); }
	}; // Periodo getIteradorInicial()const {

	/*
	std::vector<std::vector <std::vector<Periodo>>> getIteradoresIniciais()const {
		try {
			if (size() == 0)
				throw std::invalid_argument("Iterador inicial nao definido.");

			std::vector<std::vector <std::vector<Periodo>>> vetor_retorno;

			for (int structPeriod = 0; structPeriod < int(list_structPeriod.size()); structPeriod++)
				vetor_retorno.push_back(list_structPeriod.at(structPeriod).iters_iniciais);

			return vetor_retorno;
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getIteradoresIniciais(): \n" + std::string(erro.what())); }
	} // std::vector<Periodo> getIteradoresIniciais()const {
	*/

	Periodo getIteradorFinal()const {
		try {
			if (size() == 0)
				throw std::invalid_argument("Iterador final nao definido.");
			const int pos_end = getPosStructEnd();
			Periodo period = list_structPeriod.at(pos_end).getPeriodEnd();
			period.codStructPeriod = code;
			period.posStructPeriod = pos_end;
			period.idxStructPeriod = list_structPeriod.at(pos_end).getIdxEnd();
			return period;
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getIteradorFinal(): \n" + std::string(erro.what())); }
	}; // Periodo getIteradorFinal()const {

	/*
	std::vector<std::vector<std::vector<Periodo>>> getIteradoresFinais()const {
		try {
			if (size() == 0)
				throw std::invalid_argument("Iterador final nao definido.");

			std::vector<std::vector <std::vector<Periodo>>> vetor_retorno;

			for (int structPeriod = 0; structPeriod < int(list_structPeriod.size()); structPeriod++)
				vetor_retorno.push_back(list_structPeriod.at(structPeriod).iters_finais);

			return vetor_retorno;
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getIteradoresFinais(): \n" + std::string(erro.what())); }
	} // std::vector<Periodo> getIteradoresIniciais()const {
	*/

	Periodo getIterador(const Periodo a_period_interno)const {
		try {

			for (Periodo periodo_externo = getIteradorInicial(); periodo_externo <= getIteradorFinal(); incrementarIterador(periodo_externo)) {

				Periodo periodo_externo_seguinte = periodo_externo;
				incrementarIterador(periodo_externo_seguinte);

				if ((periodo_externo <= a_period_interno) && (a_period_interno < periodo_externo_seguinte))
					return periodo_externo;
				else if (a_period_interno >= getIteradorFinal())
					return getIteradorFinal();

			} // for (Periodo periodo_externo = getIteradorInicial(); periodo_externo <= getIteradorFinal(); incrementarIterador(periodo_externo)) {

			throw std::invalid_argument("Iterador nao encontrado.");

		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getIteradores(" + getFullString(a_period_interno) + "): \n" + std::string(erro.what())); }
	} // Periodo getIterador(const Periodo a_period_interno)const {


	std::vector<Periodo> getIteradores(Periodo a_period_externo)const {
		try {

			if (size() == 0)
				throw std::invalid_argument("Iteradores nao definidos.");

			Periodo perIni;

			const std::vector<int> pos_idx = getPosIdxStructIfAny(a_period_externo);
			if (pos_idx.at(1) > -1)
				perIni = list_structPeriod.at(pos_idx.at(0)).getPeriod(pos_idx.at(1));
			else
				perIni = getIteradorInicial();

			const Periodo periodo_externo_seguinte = a_period_externo + 1;

			std::vector<Periodo> iters;

			bool is_sobreposicao = false;

			for (Periodo perInner = perIni; perInner <= getIteradorFinal(); incrementarIterador(perInner)) {

				const double sobreposicao = a_period_externo.sobreposicao(perInner);

				if ((sobreposicao > 0.0) && (!is_sobreposicao)) {
					iters.push_back(perInner);
					is_sobreposicao = true;
				}
				else if ((sobreposicao > 0.0) && (is_sobreposicao))
					iters.push_back(perInner);

				else if ((sobreposicao == 0.0) && (is_sobreposicao))
					return iters;

			} // for (Periodo perInner = getIteradorInicial(); perInner <= getIteradorFinal(); incrementarIterador(perInner)) {

			return iters;

		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getIteradores(" + getFullString(a_period_externo) + "): \n" + std::string(erro.what())); }
	} // SmartEnupla<Periodo, bool> getIteradores(const Periodo a_period)const {
	
	std::vector<Periodo> getIteradores(const Periodo a_perIni, const Periodo a_perEnd)const {
		try {

			if (size() == 0)
				throw std::invalid_argument("Iteradores nao definidos.");

			if(a_perEnd < a_perIni)
				throw std::invalid_argument("Periodo inicial maior ao periodo final ");

			std::vector<Periodo> iters;

			for (Periodo perInner = getIteradorInicial(); perInner <= getIteradorFinal(); incrementarIterador(perInner)) {

				if ((a_perIni <= perInner) && (perInner <= a_perEnd))
					iters.push_back(perInner);

			} // for (Periodo perInner = getIteradorInicial(); perInner <= getIteradorFinal(); incrementarIterador(perInner)) {

			return iters;

		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getIteradores(" + getFullString(a_perIni) + getFullString(a_perEnd) + "): \n" + std::string(erro.what())); }
	} 

	void incrementarIterador(Periodo &a_period, const int a_inteiro) const {
		try {
			if (a_inteiro < 0)
				throw std::invalid_argument("Inteiro invalido.");
			for (int i = 1; i <= a_inteiro; i++)
				incrementarIterador(a_period);
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::incrementaIterador(" + getString(a_period) + "," + getString(a_inteiro) + "): \n" + std::string(erro.what())); }
	} // void incrementarIterador(Periodo &a_period, const int a_inteiro) const {

	void incrementarIteradorMinimo(Periodo& a_period) const {
		try {
			
			a_period = Periodo(TipoPeriodo_meia_hora, a_period) + 1;

		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::incrementarIteradorMinimo(" + getString(a_period) + "): \n" + std::string(erro.what())); }

	}

	void incrementarIterador(Periodo& a_period) const {
		try {

			//////////////////////////////////////////////////
			//Identifica a structPeriod a qual pertence o periodo
			//////////////////////////////////////////////////

			std::vector<int> pos_idx = getPosIdxStruct(a_period);

			if (pos_idx.at(1) < -1)
				throw std::invalid_argument("Period mismatch.");
			else if (pos_idx.at(1) > -1) {
				Periodo period_next = list_structPeriod.at(pos_idx.at(0)).getPeriodNext(pos_idx.at(1));
				if (period_next.isValido()) {
					a_period = period_next;
					a_period.codStructPeriod = code;
					a_period.posStructPeriod = pos_idx.at(0);
					a_period.idxStructPeriod = pos_idx.at(1) + 1;
					return;
				}
			}

			const int pos_end = int(list_structPeriod.size() - 1);
			// find next period in non empty structure
			for (int pos = pos_idx.at(0) + 1; pos <= pos_end; pos++) {
				if (!list_structPeriod.at(pos).isEmpty()) {
					a_period = list_structPeriod.at(pos).getPeriodIni();
					a_period.codStructPeriod = code;
					a_period.posStructPeriod = pos;
					a_period.idxStructPeriod = list_structPeriod.at(pos).getIdxIni();
					return;
				}
			}

			if ((pos_idx.at(0) == pos_end) && (pos_idx.at(1) == list_structPeriod.at(pos_idx.at(0)).getIdxEnd())) {
				a_period++;
				return;
			}

			else if (list_structPeriod.at(pos_end).getPeriodEnd() + 1 <= a_period) {
				a_period++;
				return;
			}

			throw std::invalid_argument("next period in empty structure at the end of the list.");


		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::incrementaIterador(" + getString(a_period) + "): \n" + std::string(erro.what())); }
	} // void operator++(Periodo &a_period) const {

	void decrementarIteradorMinimo(Periodo& a_period) const {
		try {

			a_period = Periodo(TipoPeriodo_meia_hora, a_period) - 1;

		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::incrementarIteradorMinimo(" + getString(a_period) + "): \n" + std::string(erro.what())); }

	}

	void decrementarIterador(Periodo& a_period) const {
		try {

			//////////////////////////////////////////////////
			//Identifica a structPeriod a qual pertence o periodo
			//////////////////////////////////////////////////

			std::vector<int> pos_idx = getPosIdxStruct(a_period);

			if (pos_idx.at(1) < -1)
				throw std::invalid_argument("Period mismatch.");
			else if (pos_idx.at(1) > -1) {
				Periodo period_prev = list_structPeriod.at(pos_idx.at(0)).getPeriodPrev(pos_idx.at(1));
				if (period_prev.isValido()) {
					a_period = period_prev;
					a_period.codStructPeriod = code;
					a_period.posStructPeriod = pos_idx.at(0);
					a_period.idxStructPeriod = pos_idx.at(1) - 1;
					return;
				}
			}
			// find next period in non empty structure
			const int pos_ini = 0;
			for (int pos = pos_idx.at(0) - 1; pos >= pos_ini; pos--) {
				if (!list_structPeriod.at(pos).isEmpty()) {
					a_period = list_structPeriod.at(pos).getPeriodEnd();
					a_period.codStructPeriod = code;
					a_period.posStructPeriod = pos;
					a_period.idxStructPeriod = list_structPeriod.at(pos).getIdxEnd();
					return;
				}
			}

			if ((pos_idx.at(0) == pos_ini) && (pos_idx.at(1) == list_structPeriod.at(pos_idx.at(0)).getIdxIni())) {
				a_period--;
				return;
			}

			else if (a_period + 1 <= list_structPeriod.at(pos_ini).getPeriodIni()) {
				a_period--;
				return;
			}

			throw std::invalid_argument("prev period in empty structure at the begining of the list.");


		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::decrementarIterador(" + getString(a_period) + "): \n" + std::string(erro.what())); }
	} // void operator++(Periodo &a_period) const {

	void decrementarIterador(Periodo& a_period, const int a_inteiro) const {
		try {
			if (a_inteiro < 0)
				throw std::invalid_argument("Inteiro invalido.");
			for (int i = 1; i <= a_inteiro; i++)
				decrementarIterador(a_period);
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::decrementarIterador(" + getString(a_period) + "," + getString(a_inteiro) + "): \n" + std::string(erro.what())); }
	} // void incrementarIterador(Periodo &a_period, const int a_inteiro) const {

	bool isProximoIterador(const Periodo a_iter) const {
		try {
			if (Periodo(a_iter.getTipoPeriodo(), getIteradorFinal() + 1) == a_iter)
				return true;
			return false;
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::isProximoIterador(" + getString(a_iter) + "): \n" + std::string(erro.what())); }
	}

	bool isIteradorValido(Periodo a_iter) const {
		try {
			if (size() == 0)
				return false;
			
			std::vector<int> pos_idx = getPosIdxStructIfAny(a_iter);
			if (pos_idx.size() > 0) {
				if (pos_idx.at(1) > -1)
					return true;
			}
			
		
			return false;
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::isIteradorValido(" + getString(a_iter) + "): \n" + std::string(erro.what())); }
	}

	void addElemento(const TipoValor a_vlr) {
		try { throw std::invalid_argument("Para SmartEnupla com tipo de iterador - Periodo - o valor do iterador deve ser informado."); } // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::addElemento(" + getString(a_vlr) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoValor a_vlr) {

	void addElemento(Periodo a_itr, const TipoValor a_vlr) {

		try {

			return adddElemento(a_itr, a_vlr);

		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::addElemento(" + getString(a_itr) + "," + getString(a_vlr) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(Periodo a_iter, TipoValor a_vlr) {

	void adddElemento(Periodo &a_itr, const TipoValor a_vlr) {

		try {

			if (list_structPeriod.size() == 0) {
				list_structPeriod.push_back(StructPeriod(a_itr, 1, -1));
				vlr.push_back(a_vlr);
				any_empty_struct = false;
				genNewCode();
				return;
			}

			//
			// Period at the end
			//
			try {
				const Periodo perEnd = list_structPeriod.at(list_structPeriod.size() - 1).getPeriodEnd();
				if (a_itr > perEnd) {

					int pos = int(list_structPeriod.size() - 1);
					int idx = list_structPeriod.at(pos).addNext(a_itr);

					// period added in the pos structure
					if (idx > 0) {
						vlr.push_back(a_vlr);
						return;
					}

					else if ((idx == -1) || (idx == -2)) {
						// adding new empty structure after the end
						if (idx == -2) {
							const Periodo perIni_empty(TipoPeriodo_minuto, perEnd + 1);
							const Periodo perEnd_empty = Periodo(TipoPeriodo_minuto, a_itr) - 1;
							list_structPeriod.push_back(StructPeriod(perIni_empty, perEnd_empty - perIni_empty + 1, list_structPeriod.at(pos).getIdxEnd(), true));
							any_empty_struct = true;
						}
						// period must be added in new structure after the end
						list_structPeriod.push_back(StructPeriod(a_itr, 1, list_structPeriod.at(pos).getIdxEnd()));
						vlr.push_back(a_vlr);
						return;
					} // else if ((idx == -1) || (idx == -2)) {
					else
						throw std::invalid_argument("Invalid period at the end.");

				} // if (a_itr > perEnd) {
			}
			catch (const std::exception& erro) { throw std::invalid_argument("Period at the end: \n" + std::string(erro.what())); }


			//
			// Period at the begining
			//
			try {
				const Periodo perIni = list_structPeriod.at(0).getPeriodIni();
				if (a_itr < perIni) {

					int pos = 0;
					int idx = list_structPeriod.at(pos).addPrev(a_itr);

					// period added in the begining of the pos structure
					if (idx == 0) {
						vlr.insert(vlr.begin(), a_vlr);
						updateStructIdx();
						return;
					}

					else if ((idx == -1) || (idx == -2)) {
						// adding new empty structure in the begining
						if (idx == -2) {
							const Periodo perIni_empty(TipoPeriodo_minuto, a_itr + 1);
							const Periodo perEnd_empty = Periodo(TipoPeriodo_minuto, perIni) - 1;
							list_structPeriod.insert(list_structPeriod.begin(), StructPeriod(perIni_empty, perEnd_empty - perIni_empty + 1, -1, true));
							any_empty_struct = true;
						}
						// period must be added in new structure in the begining
						list_structPeriod.insert(list_structPeriod.begin(), StructPeriod(a_itr, 1, -1));
						vlr.insert(vlr.begin(), a_vlr);
						updateStructIdx();
						return;
					} // else if ((idx == -1) || (idx == -2)) {
					else
						throw std::invalid_argument("Invalid period at the begining.");

				} // if (a_itr < perIni) {
			}
			catch (const std::exception& erro) { throw std::invalid_argument("Period at the begining: \n" + std::string(erro.what())); }

			//
			// period in some empty structure
			//
			try {
				const Periodo itr_next_minute = Periodo(TipoPeriodo_minuto, a_itr + 1);
				const int pos_end = int(list_structPeriod.size() - 1);
				for (int pos = 0; pos <= pos_end; pos++) {
					if (list_structPeriod.at(pos).isEmpty()) {
						int insert_flag = -100; // -1: end prev struct, 0: current, 1: begining next.
						if (list_structPeriod.at(pos).getPeriodIni() <= a_itr) {
							if (pos < pos_end) {
								if (itr_next_minute <= list_structPeriod.at(pos + 1).getPeriodIni()) {
									insert_flag = -10;
									if (pos > 0) {
										if (list_structPeriod.at(pos - 1).getPeriodEnd() + 1 == a_itr)
											insert_flag = -1;
									}
									else if (list_structPeriod.at(pos + 1).getPeriodIni() - 1 == a_itr)
										insert_flag = 1;
									else
										insert_flag = 0;
								}
							} // if (pos < pos_end) {
							else if (itr_next_minute <= list_structPeriod.at(pos).getPeriodEnd() + 1) {
								insert_flag = -10;
								if (list_structPeriod.at(pos - 1).getPeriodEnd() + 1 == a_itr)
									insert_flag = -1;
								else
									insert_flag = 0;
							}
						} // if (list_structPeriod.at(pos).getPeriodIni() <= a_itr) {

						if (insert_flag == -10)
							throw std::invalid_argument("Invalid period.");

						// -1: end prev struct
						else if (insert_flag == -1) {
							if (list_structPeriod.at(pos - 1).addNext(a_itr) < 0)
								throw std::invalid_argument("Error");
							vlr.insert(vlr.begin() + list_structPeriod.at(pos - 1).getIdxEnd(), a_vlr);
							const int num_per = list_structPeriod.at(pos).getNumPer() - a_itr.getMinutos();
							if (num_per > 0)
								list_structPeriod.at(pos) = StructPeriod(itr_next_minute, num_per, list_structPeriod.at(pos - 1).getIdxEnd(), true);
							else
								list_structPeriod.erase(list_structPeriod.begin() + pos);
							updateStructIdx();
							return;
						} // else if (insert_flag == -1) {

						// 1: begining next struct
						else if (insert_flag == 1) {
							if (list_structPeriod.at(pos + 1).addPrev(a_itr) < 0)
								throw std::invalid_argument("Error");
							vlr.insert(vlr.begin() + list_structPeriod.at(pos + 1).getIdxIni(), a_vlr);
							const int num_per = list_structPeriod.at(pos).getNumPer() - a_itr.getMinutos();
							if (num_per > 0)
								list_structPeriod.at(pos) = StructPeriod(list_structPeriod.at(pos).getPeriodIni(), num_per, list_structPeriod.at(pos - 1).getIdxEnd(), true);
							else
								list_structPeriod.erase(list_structPeriod.begin() + pos);
							updateStructIdx();
							return;
						} // else if (insert_flag == 1) {

						// 0: current struct will be replaced by the new ones
						else if (insert_flag == 0) {
							std::vector<StructPeriod> new_structs(3);
							const int num_per = a_itr.getMinutos();
							if (list_structPeriod.at(pos).getPeriodIni() == Periodo(TipoPeriodo_minuto, a_itr)) {
								if (pos == 0)
									new_structs.push_back(StructPeriod(a_itr, 1, -1));
								else
									new_structs.push_back(StructPeriod(a_itr, 1, list_structPeriod.at(pos - 1).getIdxEnd()));
								vlr.insert(vlr.begin() + new_structs.at(0).getIdxEnd(), a_vlr);
								if (num_per < list_structPeriod.at(pos).getNumPer())
									new_structs.push_back(StructPeriod(itr_next_minute, list_structPeriod.at(pos).getNumPer() - num_per, new_structs.at(0).getIdxEnd(), true));
							}
							else if (list_structPeriod.at(pos).getPeriodEnd() + 1 == itr_next_minute) {
								if (num_per < list_structPeriod.at(pos).getNumPer())
									new_structs.push_back(StructPeriod(list_structPeriod.at(pos).getPeriodIni(), list_structPeriod.at(pos).getNumPer() - num_per, list_structPeriod.at(pos).getIdxIni(), true));
								if (pos == 0)
									new_structs.push_back(StructPeriod(a_itr, 1, -1));
								else
									new_structs.push_back(StructPeriod(a_itr, 1, list_structPeriod.at(pos - 1).getIdxEnd()));
								vlr.insert(vlr.begin() + new_structs.at(new_structs.size() - 1).getIdxEnd(), a_vlr);
							}
							else {
								new_structs.push_back(StructPeriod(list_structPeriod.at(pos).getPeriodIni(), Periodo(TipoPeriodo_minuto, a_itr) - list_structPeriod.at(pos).getPeriodIni(), list_structPeriod.at(pos).getIdxIni(), true));
								if (pos == 0)
									new_structs.push_back(StructPeriod(a_itr, 1, -1));
								else
									new_structs.push_back(StructPeriod(a_itr, 1, list_structPeriod.at(pos - 1).getIdxEnd()));
								vlr.insert(vlr.begin() + new_structs.at(new_structs.size() - 1).getIdxEnd(), a_vlr);
								new_structs.push_back(StructPeriod(itr_next_minute, list_structPeriod.at(pos).getNumPer() - new_structs.at(0).getNumPer() - num_per, new_structs.at(new_structs.size() - 1).getIdxEnd(), true));
							}
							list_structPeriod.erase(list_structPeriod.begin() + pos);
							list_structPeriod.insert(list_structPeriod.begin() + pos, new_structs.begin(), new_structs.end());
							updateStructIdx();
							return;
						} // else if (insert_flag == 0) {

					} // if (list_structPeriod.at(pos).isEmpty()) {
				} // for (int pos = 0; pos <= pos_end; pos++) {
			}
			catch (const std::exception& erro) { throw std::invalid_argument("Period in empty struct: \n" + std::string(erro.what())); }

			throw std::invalid_argument("Error adding element.");

		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::adddElemento(" + getString(a_itr) + "," + getString(a_vlr) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(Periodo a_iter, TipoValor a_vlr) {


	void adddElementoVoid(Periodo& a_itr) {

	}; // void addElemento(Periodo a_iter, TipoValor a_vlr) {


	void alterarValorSeAlterarIterador(const Periodo a_iter, const TipoValor a_vlr) {
		try {

			/*
			std::vector<Periodo> iters = getIteradores(a_iter);

			if (iters.size() == 0)
				throw std::invalid_argument("Nao ha iters compativeis.");
			else if (iters.size() == 1) {
				if (iters.at(0) == a_iter)
					return;
			}

			const Periodo perIni_orig = iters.at(0);
			const Periodo periodo_final_orig = iters.at(iters.size() - 1);

			if ((Periodo(perIni_orig.getTipoPeriodo(), a_iter) != perIni_orig) || \
				(Periodo(periodo_final_orig.getTipoPeriodo(), a_iter + 1) - 1 != periodo_final_orig))
					throw std::invalid_argument("Periodo nao se encaixa exatamente em range de iters.");


			TipoStructPeriodPeriodo tipo_structPeriod_anterior = TipoStructPeriodPeriodo_Nenhum;
			TipoPeriodo tipo_periodo_anterior = TipoPeriodo_Nenhum;
			int indice_structPeriod_anterior = -1;
			Periodo periodo_anterior = perIni_orig;
			if (perIni_orig > getIteradorInicial()) {
				decrementarIterador(periodo_anterior);
				indice_structPeriod_anterior = getPosIdxStruct(periodo_anterior);
				tipo_structPeriod_anterior = list_structPeriod.at(indice_structPeriod_anterior).tipoStructPeriodPeriodo;
				tipo_periodo_anterior = periodo_anterior.getTipoPeriodo();
			}

			TipoStructPeriodPeriodo tipo_structPeriod_posterior = TipoStructPeriodPeriodo_Nenhum;
			TipoPeriodo tipo_periodo_posterior = TipoPeriodo_Nenhum;
			int indice_structPeriod_posterior = -1;
			Periodo periodo_posterior = periodo_final_orig; 
			if (periodo_final_orig < getIteradorFinal()){
				incrementarIterador(periodo_posterior);
				indice_structPeriod_posterior = getPosIdxStruct(periodo_posterior);
				tipo_structPeriod_posterior = list_structPeriod.at(indice_structPeriod_posterior).tipoStructPeriodPeriodo;
				tipo_periodo_posterior = periodo_posterior.getTipoPeriodo();
			}

			std::vector<StructPeriod> list_structPeriod_copia = list_structPeriod;

			bool structPeriods_anterior_posterior_duplicadas = false;
			// Verifica necessidade de duplicar structPeriod atual
			if ((tipo_structPeriod_anterior != TipoStructPeriodPeriodo_Nenhum) && (tipo_structPeriod_posterior != TipoStructPeriodPeriodo_Nenhum)) {
				// Caso structPeriods anterior e posterior sejam a mesma, duplica-se structPeriod para inclusao entre ambas
				if (indice_structPeriod_anterior == indice_structPeriod_posterior) {
					list_structPeriod_copia.insert(list_structPeriod_copia.begin() + indice_structPeriod_anterior, list_structPeriod_copia.at(indice_structPeriod_anterior));
					indice_structPeriod_posterior++;
					structPeriods_anterior_posterior_duplicadas = true;
				}
			}// if ((tipo_structPeriod_anterior != TipoStructPeriodPeriodo_Nenhum) && (tipo_structPeriod_posterior != TipoStructPeriodPeriodo_Nenhum)) {

			const TipoPeriodo type_per = a_iter.getTipoPeriodo();
			
			int status_inclusao_iterador = -1; // -1 Iterador novo em nova structPeriod a ser criada | 0 iterador novo na structPeriod de inicio | 1 iterador novo na structPeriod do fim

			// Iterador novo em toda enupla.
			if ((tipo_structPeriod_anterior == TipoStructPeriodPeriodo_Nenhum) && (tipo_structPeriod_posterior == TipoStructPeriodPeriodo_Nenhum))
				list_structPeriod_copia = std::vector<StructPeriod>();

			// Iterador novo no inicio da enupla
			else if ((tipo_structPeriod_anterior == TipoStructPeriodPeriodo_Nenhum) && (tipo_structPeriod_posterior != TipoStructPeriodPeriodo_Nenhum)) {

				// Remove structPeriods dos periodos que sairao
				const int nro_structPeriods_a_remover = indice_structPeriod_posterior;
				for (int i = 0; i < nro_structPeriods_a_remover; i++) {
					list_structPeriod_copia.erase(list_structPeriod_copia.begin());
					indice_structPeriod_posterior--;
				}

				// Realiza adequacoes na structPeriod posterior
				if (tipo_periodo_posterior > list_structPeriod_copia.at(indice_structPeriod_posterior).tipo_iterador_final) {
					tipo_structPeriod_posterior = TipoStructPeriodPeriodo_crescente;
					if (type_per >= tipo_periodo_posterior)
						status_inclusao_iterador = 1;
				}
				else if (tipo_periodo_posterior < list_structPeriod_copia.at(indice_structPeriod_posterior).tipo_iterador_final) {
					tipo_structPeriod_posterior = TipoStructPeriodPeriodo_decrescente;
					if (type_per <= tipo_periodo_posterior)
						status_inclusao_iterador = 1;
				}
				else if (tipo_periodo_posterior == list_structPeriod_copia.at(indice_structPeriod_posterior).tipo_iterador_final) {
					status_inclusao_iterador = 1;
					if (type_per >= tipo_periodo_posterior)
						tipo_structPeriod_posterior = TipoStructPeriodPeriodo_crescente;
					else if (type_per <= tipo_periodo_posterior)
						tipo_structPeriod_posterior = TipoStructPeriodPeriodo_decrescente;
					else
						tipo_structPeriod_posterior = TipoStructPeriodPeriodo_flat;
				}

				list_structPeriod_copia.at(indice_structPeriod_posterior).tipoStructPeriodPeriodo = tipo_structPeriod_posterior;
					
			} // else if ((tipo_structPeriod_anterior == TipoStructPeriodPeriodo_Nenhum) && (tipo_structPeriod_posterior != TipoStructPeriodPeriodo_Nenhum)) {

			// Iterador novo no final da enupla
			else if ((tipo_structPeriod_anterior != TipoStructPeriodPeriodo_Nenhum) && (tipo_structPeriod_posterior == TipoStructPeriodPeriodo_Nenhum)) {

				// Remove structPeriods dos periodos que sairao
				const int nro_structPeriods_a_remover = int(list_structPeriod_copia.size()) - (indice_structPeriod_anterior + 1);
				for (int i = 0; i < nro_structPeriods_a_remover; i++)
					list_structPeriod_copia.pop_back();

				if (list_structPeriod_copia.at(indice_structPeriod_anterior).tipo_iterador_inicial > tipo_periodo_anterior) {
					tipo_structPeriod_anterior = TipoStructPeriodPeriodo_crescente;
					if (tipo_periodo_anterior >= type_per)
						status_inclusao_iterador = 0;
				}
				else if (list_structPeriod_copia.at(indice_structPeriod_anterior).tipo_iterador_inicial < tipo_periodo_anterior) {
					tipo_structPeriod_anterior = TipoStructPeriodPeriodo_decrescente;
					if (tipo_periodo_anterior <= type_per)
						status_inclusao_iterador = 0;
				}
				else if (list_structPeriod_copia.at(indice_structPeriod_anterior).tipo_iterador_inicial == tipo_periodo_anterior) {
					status_inclusao_iterador = 0;
					if (tipo_periodo_anterior >= type_per)
						tipo_structPeriod_anterior = TipoStructPeriodPeriodo_crescente;
					else if (tipo_periodo_anterior <= type_per)
						tipo_structPeriod_anterior = TipoStructPeriodPeriodo_decrescente;
					else
						tipo_structPeriod_anterior = TipoStructPeriodPeriodo_flat;
				}

				list_structPeriod_copia.at(indice_structPeriod_anterior).tipoStructPeriodPeriodo = tipo_structPeriod_anterior;

			} // else if ((tipo_structPeriod_anterior != TipoStructPeriodPeriodo_Nenhum) && (tipo_structPeriod_posterior == TipoStructPeriodPeriodo_Nenhum)) {

			// Iterador novo no meio da enupla
			else if ((tipo_structPeriod_anterior != TipoStructPeriodPeriodo_Nenhum) && (tipo_structPeriod_posterior != TipoStructPeriodPeriodo_Nenhum)) {

				// Remove structPeriods dos periodos que sairao
				const int nro_structPeriods_a_remover = indice_structPeriod_posterior - indice_structPeriod_anterior - 1;
				for (int i = 0; i < nro_structPeriods_a_remover; i++) {
					list_structPeriod_copia.erase(list_structPeriod_copia.begin() + indice_structPeriod_anterior + 1);
					indice_structPeriod_posterior--;
				}

				// Realiza adequacoes na structPeriod anterior
				if (list_structPeriod_copia.at(indice_structPeriod_anterior).tipo_iterador_inicial > tipo_periodo_anterior) {
					tipo_structPeriod_anterior = TipoStructPeriodPeriodo_crescente;
					if (tipo_periodo_anterior >= type_per)
						status_inclusao_iterador = 0;
				}
				else if (list_structPeriod_copia.at(indice_structPeriod_anterior).tipo_iterador_inicial < tipo_periodo_anterior) {
					tipo_structPeriod_anterior = TipoStructPeriodPeriodo_decrescente;
					if (tipo_periodo_anterior <= type_per)
						status_inclusao_iterador = 0;
				}
				else if (list_structPeriod_copia.at(indice_structPeriod_anterior).tipo_iterador_inicial == tipo_periodo_anterior) {
					status_inclusao_iterador = 0;
					if (tipo_periodo_anterior >= type_per)
						tipo_structPeriod_anterior = TipoStructPeriodPeriodo_crescente;
					else if (tipo_periodo_anterior <= type_per)
						tipo_structPeriod_anterior = TipoStructPeriodPeriodo_decrescente;
					else
						tipo_structPeriod_anterior = TipoStructPeriodPeriodo_flat;
				}

				list_structPeriod_copia.at(indice_structPeriod_anterior).tipoStructPeriodPeriodo = tipo_structPeriod_anterior;

				// Realiza adequacoes na structPeriod posterior
				if (tipo_periodo_posterior > list_structPeriod_copia.at(indice_structPeriod_posterior).tipo_iterador_final) {
					tipo_structPeriod_posterior = TipoStructPeriodPeriodo_crescente;
					if ((type_per >= tipo_periodo_posterior) && (status_inclusao_iterador == -1))
						status_inclusao_iterador = 1;
				}
				else if (tipo_periodo_posterior < list_structPeriod_copia.at(indice_structPeriod_posterior).tipo_iterador_final) {
					tipo_structPeriod_posterior = TipoStructPeriodPeriodo_decrescente;
					if ((type_per <= tipo_periodo_posterior) && (status_inclusao_iterador == -1))
						status_inclusao_iterador = 1;
				}
				else if (tipo_periodo_posterior == list_structPeriod_copia.at(indice_structPeriod_posterior).tipo_iterador_final) {
					tipo_structPeriod_posterior = TipoStructPeriodPeriodo_flat;
					if (status_inclusao_iterador == -1) {
						status_inclusao_iterador = 1;
						if (type_per >= tipo_periodo_posterior)
							tipo_structPeriod_posterior = TipoStructPeriodPeriodo_crescente;
						else if (type_per <= tipo_periodo_posterior)
							tipo_structPeriod_posterior = TipoStructPeriodPeriodo_decrescente;
					}
				}

				list_structPeriod_copia.at(indice_structPeriod_posterior).tipoStructPeriodPeriodo = tipo_structPeriod_posterior;

			} // else if ((tipo_structPeriod_anterior != TipoStructPeriodPeriodo_Nenhum) && (tipo_structPeriod_posterior != TipoStructPeriodPeriodo_Nenhum)) {


			//
			// Criacao structPeriod intermediaria
			//
			if (status_inclusao_iterador == -1) {

				const int indice_nova_structPeriod = indice_structPeriod_anterior + 1;

				if (true) {
					StructPeriod structPeriod;
					structPeriod.tipo_iterador_inicial = TipoPeriodo_Nenhum;
					structPeriod.tipo_iterador_final = TipoPeriodo_Nenhum;
					structPeriod.iters_iniciais = std::vector<std::vector<Periodo>>(TipoPeriodo_Excedente, std::vector<Periodo>());
					structPeriod.iters_finais = std::vector<std::vector<Periodo>>(TipoPeriodo_Excedente, std::vector<Periodo>());
					structPeriod.smartEnupla = std::vector<std::vector<TipoValor>>(TipoPeriodo_Excedente, std::vector<TipoValor>());
					for (TipoPeriodo tipo = TipoPeriodo(1); tipo < TipoPeriodo_Excedente; tipo++) {
						structPeriod.iters_iniciais.at(tipo).reserve(1);
						structPeriod.iters_finais.at(tipo).reserve(1);
						structPeriod.smartEnupla.at(tipo).reserve(1);
					}

					structPeriod.tipoStructPeriodPeriodo = TipoStructPeriodPeriodo_flat;
					list_structPeriod_copia.insert(list_structPeriod_copia.begin() + indice_nova_structPeriod, structPeriod);
				}

				if (indice_structPeriod_posterior > -1)
					indice_structPeriod_posterior++;

				list_structPeriod_copia.at(indice_nova_structPeriod).tipoStructPeriodPeriodo = TipoStructPeriodPeriodo_flat;

				list_structPeriod_copia.at(indice_nova_structPeriod).tipo_iterador_inicial = type_per;
				list_structPeriod_copia.at(indice_nova_structPeriod).tipo_iterador_final   = type_per;

				list_structPeriod_copia.at(indice_nova_structPeriod).iters_iniciais.at(type_per).push_back(a_iter);
				list_structPeriod_copia.at(indice_nova_structPeriod).iters_finais.at(type_per).push_back(a_iter);
				list_structPeriod_copia.at(indice_nova_structPeriod).smartEnupla.at(type_per).push_back(a_vlr);

			} // if (status_inclusao_iterador == -1) {

			//
			// Adequacao structPeriod anterior
			//
			if (tipo_structPeriod_anterior != TipoStructPeriodPeriodo_Nenhum) {

				TipoPeriodo tipo_periodo_final_structPeriod_anterior = list_structPeriod_copia.at(indice_structPeriod_anterior).tipo_iterador_final;
				Periodo periodo_final_structPeriod_anterior = list_structPeriod_copia.at(indice_structPeriod_anterior).iters_finais.at(tipo_periodo_final_structPeriod_anterior).at(0);

				while (periodo_final_structPeriod_anterior > periodo_anterior) {

					Periodo novo_periodo_final_structPeriod_anterior = periodo_final_structPeriod_anterior;
					decrementarIterador(novo_periodo_final_structPeriod_anterior);

					TipoPeriodo novo_tipo_periodo_final_structPeriod_anterior = novo_periodo_final_structPeriod_anterior.getTipoPeriodo();

					if (novo_tipo_periodo_final_structPeriod_anterior == tipo_periodo_final_structPeriod_anterior) {
						list_structPeriod_copia.at(indice_structPeriod_anterior).iters_finais.at(tipo_periodo_final_structPeriod_anterior).at(0) = novo_periodo_final_structPeriod_anterior;
						list_structPeriod_copia.at(indice_structPeriod_anterior).smartEnupla.at(tipo_periodo_final_structPeriod_anterior).pop_back();
					}
					else{
						list_structPeriod_copia.at(indice_structPeriod_anterior).iters_iniciais.at(tipo_periodo_final_structPeriod_anterior) = std::vector<Periodo>();
						list_structPeriod_copia.at(indice_structPeriod_anterior).iters_finais.at(tipo_periodo_final_structPeriod_anterior) = std::vector<Periodo>();
						list_structPeriod_copia.at(indice_structPeriod_anterior).smartEnupla.at(tipo_periodo_final_structPeriod_anterior) = std::vector<TipoValor>();
					}

					periodo_final_structPeriod_anterior = novo_periodo_final_structPeriod_anterior;
					tipo_periodo_final_structPeriod_anterior = novo_tipo_periodo_final_structPeriod_anterior;

				} // while (periodo_final_structPeriod_anterior > periodo_anterior) {

				list_structPeriod_copia.at(indice_structPeriod_anterior).tipo_iterador_final = tipo_periodo_final_structPeriod_anterior;

				if (status_inclusao_iterador == 0) {

					if (tipo_periodo_final_structPeriod_anterior == type_per) {
						list_structPeriod_copia.at(indice_structPeriod_anterior).iters_finais.at(tipo_periodo_final_structPeriod_anterior).at(0) = a_iter;
						list_structPeriod_copia.at(indice_structPeriod_anterior).smartEnupla.at(tipo_periodo_final_structPeriod_anterior).push_back(a_vlr);
					}

					else {
						list_structPeriod_copia.at(indice_structPeriod_anterior).iters_iniciais.at(type_per) = std::vector<Periodo>(1, a_iter);
						list_structPeriod_copia.at(indice_structPeriod_anterior).iters_finais.at(type_per) = std::vector<Periodo>(1, a_iter);
						list_structPeriod_copia.at(indice_structPeriod_anterior).smartEnupla.at(type_per) = std::vector<TipoValor>(1, a_vlr);
					}

					list_structPeriod_copia.at(indice_structPeriod_anterior).tipo_iterador_final = type_per;

				} // if (status_inclusao_iterador == 0) {
			} // if (tipo_structPeriod_anterior != TipoStructPeriodPeriodo_Nenhum) {
			
			//
			// Adequacao structPeriod posterior
			//
			if (tipo_structPeriod_posterior != TipoStructPeriodPeriodo_Nenhum) {

				TipoPeriodo tipo_perIni_structPeriod_posterior = list_structPeriod_copia.at(indice_structPeriod_posterior).tipo_iterador_inicial;
				Periodo perIni_structPeriod_posterior = list_structPeriod_copia.at(indice_structPeriod_posterior).iters_iniciais.at(tipo_perIni_structPeriod_posterior).at(0);

				while (perIni_structPeriod_posterior < periodo_posterior) {

					Periodo novo_perIni_structPeriod_posterior = perIni_structPeriod_posterior;
					incrementarIterador(novo_perIni_structPeriod_posterior);

					TipoPeriodo novo_tipo_perIni_structPeriod_posterior = novo_perIni_structPeriod_posterior.getTipoPeriodo();

					if (novo_tipo_perIni_structPeriod_posterior == tipo_perIni_structPeriod_posterior) {
						list_structPeriod_copia.at(indice_structPeriod_posterior).iters_iniciais.at(tipo_perIni_structPeriod_posterior).at(0) = novo_perIni_structPeriod_posterior;
						list_structPeriod_copia.at(indice_structPeriod_posterior).smartEnupla.at(tipo_perIni_structPeriod_posterior).erase(list_structPeriod_copia.at(indice_structPeriod_posterior).smartEnupla.at(tipo_perIni_structPeriod_posterior).begin());
					}
					else {
						list_structPeriod_copia.at(indice_structPeriod_posterior).iters_iniciais.at(tipo_perIni_structPeriod_posterior) = std::vector<Periodo>();
						list_structPeriod_copia.at(indice_structPeriod_posterior).iters_finais.at(tipo_perIni_structPeriod_posterior) = std::vector<Periodo>();
						list_structPeriod_copia.at(indice_structPeriod_posterior).smartEnupla.at(tipo_perIni_structPeriod_posterior) = std::vector<TipoValor>();
					}
			
					perIni_structPeriod_posterior = novo_perIni_structPeriod_posterior;
					tipo_perIni_structPeriod_posterior = novo_tipo_perIni_structPeriod_posterior;

				} // while (perIni_structPeriod_posterior > periodo_posterior) {

				list_structPeriod_copia.at(indice_structPeriod_posterior).tipo_iterador_inicial = tipo_perIni_structPeriod_posterior;

				if (status_inclusao_iterador == 1) {

					if (tipo_perIni_structPeriod_posterior == type_per) {
						list_structPeriod_copia.at(indice_structPeriod_posterior).iters_iniciais.at(tipo_perIni_structPeriod_posterior).at(0) = a_iter;
						list_structPeriod_copia.at(indice_structPeriod_posterior).smartEnupla.at(tipo_perIni_structPeriod_posterior).push_back(a_vlr);
					}

					else {
						list_structPeriod_copia.at(indice_structPeriod_posterior).iters_iniciais.at(type_per) = std::vector<Periodo>(1, a_iter);
						list_structPeriod_copia.at(indice_structPeriod_posterior).iters_finais.at(type_per) = std::vector<Periodo>(1, a_iter);
						list_structPeriod_copia.at(indice_structPeriod_posterior).smartEnupla.at(type_per) = std::vector<TipoValor>(1, a_vlr);
					}

					list_structPeriod_copia.at(indice_structPeriod_posterior).tipo_iterador_inicial = type_per;

				} // if (status_inclusao_iterador == 1) {
			} // if (tipo_structPeriod_posterior != TipoStructPeriodPeriodo_Nenhum) {

			list_structPeriod = list_structPeriod_copia;
			num_elem = num_elem - int(iters.size()) + 1;
			*/

		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::alterarValorSeAlterarIterador(" + getFullString(a_iter) + ",a_tipo_valor): \n" + std::string(erro.what())); }
	}


	void setElemento(Periodo a_iter, const TipoValor a_vlr) {
		try { 
			
			settElemento(a_iter, a_vlr);

		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::setElemento(" + getString(a_iter) + "," + getString(a_vlr) + "): \n" + std::string(erro.what())); }
	}; // void setElemento(Periodo a_iter, TipoValor a_vlr) {

	void settElemento(Periodo &a_iter, const TipoValor a_vlr) {
		try {

			//////////////////////////////////////////////////
			//Identifica a structPeriod a qual pertence o periodo
			//////////////////////////////////////////////////

			std::vector<int> pos_idx = getPosIdxStruct(a_iter);

			if (pos_idx.at(1) < -1)
				throw std::invalid_argument("Period mismatch.");
			else if (pos_idx.at(1) == -1)
				throw std::invalid_argument("Period in empty structure.");
			else if (pos_idx.at(1) > -1)
				vlr.at(pos_idx.at(1)) = a_vlr;

		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::settElemento(" + getString(a_iter) + "," + getString(a_vlr) + "): \n" + std::string(erro.what())); }
	}; // void setElemento(Periodo a_iter, TipoValor a_vlr) {

	void setElementoFromStr(Periodo a_iter, const std::string a_vlr) {
		try {

			settElementoFromStr(a_iter,a_vlr);

		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::setElemento(" + getString(a_iter) + "," + getString(a_vlr) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iter, TipoValor a_vlr) {

	void settElementoFromStr(Periodo &a_iter, const std::string a_vlr) {
		try {

			settElemento(a_iter, getFromString(TipoValor(), a_vlr));

		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::settElemento(" + getString(a_iter) + "," + getString(a_vlr) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iter, TipoValor a_vlr) {


	TipoValor getTipoElemento()const {
		try {
			return TipoValor();
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getTipoElemento(): \n" + std::string(erro.what())); }
	}; // TipoValor getTipoElemento()const {

	TipoValor getElemento(Periodo a_iter)const {
		try { 
			
			return gettElemento(a_iter);
		
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getElemento(" + getString(a_iter) + "): \n" + std::string(erro.what())); }
	}; // TipoValor getElemento(Periodo a_iter)const {

	TipoValor gettElemento(Periodo &a_iter)const {
		try {

			return att(a_iter);

		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::gettElemento(" + getString(a_iter) + "): \n" + std::string(erro.what())); }
	}; // TipoValor getElemento(Periodo a_iter)const {

	TipoValor& at(Periodo a_iter) {
		try { 
			
			std::vector<int> pos_idx = getPosIdxStruct(a_iter);

			if (pos_idx.at(1) < -1)
				throw std::invalid_argument("Period mismatch.");
			else if (pos_idx.at(1) == -1)
				throw std::invalid_argument("Period in empty structure.");
			else
				return vlr.at(pos_idx.at(1));
		
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::at(" + getString(a_iter) + "): \n" + std::string(erro.what())); }
	}; // TipoValor at(Periodo a_iter)const {

	TipoValor& att(Periodo &a_iter) {
		try {

			std::vector<int> pos_idx = getPosIdxStruct(a_iter);

			if (pos_idx.at(1) < -1)
				throw std::invalid_argument("Period mismatch.");
			else if (pos_idx.at(1) == -1)
				throw std::invalid_argument("Period in empty structure.");
			else
				return vlr.at(pos_idx.at(1));

		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::att(" + getString(a_iter) + "): \n" + std::string(erro.what())); }
	}; // TipoValor at(Periodo a_iter)const {

	TipoValor at(Periodo a_iter) const {
		try { 
			
			std::vector<int> pos_idx = getPosIdxStruct(a_iter);

			if (pos_idx.at(1) < -1)
				throw std::invalid_argument("Period mismatch.");
			else if (pos_idx.at(1) == -1)
				throw std::invalid_argument("Period in empty structure.");
			else
				return vlr.at(pos_idx.at(1));
		
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::at(" + getString(a_iter) + "): \n" + std::string(erro.what())); }
	}; // TipoValor at(Periodo a_iter)const {

	TipoValor att(Periodo &a_iter) const {
		try {

			std::vector<int> pos_idx = getPosIdxStruct(a_iter);

			if (pos_idx.at(1) < -1)
				throw std::invalid_argument("Period mismatch.");
			else if (pos_idx.at(1) == -1)
				throw std::invalid_argument("Period in empty structure.");
			else
				return vlr.at(pos_idx.at(1));

		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::att(" + getString(a_iter) + "): \n" + std::string(erro.what())); }
	}; // TipoValor at(Periodo a_iter)const {

	void random_shuffle() {
			std::random_shuffle(vlr.begin(), vlr.end());
	}; // void random_shuffle() {

	std::vector<TipoValor> getElementos() const {
		return vlr;
	};

	SmartEnupla<Periodo, TipoValor> getElementos(Periodo &a_perIni, Periodo &a_perEnd) const {
		try {

			std::vector<int> pos_idx1 = getPosIdxStruct(a_perIni);
			std::vector<int> pos_idx2 = getPosIdxStruct(a_perEnd);

			SmartEnupla<Periodo, TipoValor> enupla_return(a_perEnd - a_perIni + 1);
			for (Periodo period = a_perIni; period <= a_perEnd; incrementarIterador(period))
				enupla_return.addElemento(period, getElemento(period));

			return enupla_return;

		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getElementos(" + getString(a_perIni) + "," + getString(a_perEnd) + "): \n" + std::string(erro.what())); }
	};

	std::vector<TipoValor>& getElementos_referencia() {
		try {
			throw std::invalid_argument("Nao eh possivel retornar referencia em SmartEnupla com iterador Periodo, pois os elementos nao sao sequenciais.");
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getElementos(): \n" + std::string(erro.what())); }
	};

	SmartEnupla<Periodo, TipoValor> getElementos(Periodo &a_perIni, const int a_num_elem) const {
		try {
			if (a_num_elem <= 0)
				throw std::invalid_argument("Numero de elementos invalido.");

			std::vector<int> pos_idx1 = getPosIdxStruct(a_perIni);

			SmartEnupla<Periodo, TipoValor> enupla_return(a_num_elem);
			int cont = 0;
			for (Periodo period = a_perIni; period <= getIteradorFinal(); incrementarIterador(period)) {
				cont++;
				enupla_return.addElemento(period, getElemento(period));
				if (cont == a_num_elem)
					break;
			}

			return enupla_return;
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getElementos(" + getString(a_perIni) + "," + getString(a_num_elem) + "): \n" + std::string(erro.what())); }
	};


	std::vector<std::vector<TipoValor>> getElementos(const IdEstacao a_estacao) const {
		try {
			if ((a_estacao <= IdEstacao_Nenhum) || (a_estacao >= IdEstacao_Excedente))
				throw std::invalid_argument("Estacao Invalida.");

			std::vector<std::vector<TipoValor>> vetor_retorno(TipoPeriodo_Excedente, std::vector<TipoValor>());
			for (Periodo period = getIteradorInicial(); period <= getIteradorFinal(); incrementarIterador(period)) {
				if (period.getEstacao() == a_estacao)
					vetor_retorno.at(period.getTipoPeriodo()).push_back(getElemento(period));
			}
			return vetor_retorno;
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getElementos(" + getString(a_estacao) + "): \n" + std::string(erro.what())); }
	}; // std::vector<TipoValor> getElementos(int a_estacao) {

	std::string str() { return "SmartEnupla<iterador,valor>"; }

	std::vector<StructPeriod> getListaStructPeriod()const { return list_structPeriod; }

}; // class SmartEnupla {




#endif