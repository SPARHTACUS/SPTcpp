#ifndef SMART_ENUPLA
#define SMART_ENUPLA

#include "C_Auxiliar.h"
#include "C_SmartEnum.h"
#include "C_Periodo.h"

#include <vector>
#include <string>
#include <algorithm>

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
	SmartEnupla(const int a_valor_alocar) { smartEnupla.reserve(a_valor_alocar); };
	SmartEnupla(const bool a_inicializar_from_0, const TipoIterador a_iteradorInicial, const std::vector<TipoValor>& a_vetorInicial) {
		if (a_inicializar_from_0) {
			if (a_iteradorInicial < TipoIterador(0))
				throw std::invalid_argument("O iterador Inicial deve ser maior ou igual a " + getString(TipoIterador(0)));
		}
		else {
			if (a_iteradorInicial < getFromChar(TipoIterador(), "min"))
				throw std::invalid_argument("O iterador Inicial deve ser maior ou igual a " + getString(getFromChar(TipoIterador(), "min")));
		}
		if (a_vetorInicial.size() == 0)
			throw std::invalid_argument("O vetor inicial deve possuir algum elemento.");
		iteradorInicial = a_iteradorInicial;
		smartEnupla = a_vetorInicial;
		iteradorFinal = TipoIterador(iteradorInicial + int(smartEnupla.size()) - 1);
	};
	SmartEnupla(const TipoIterador a_iteradorInicial, const std::vector<TipoValor>& a_vetorInicial) {
		if (a_iteradorInicial < getFromChar(TipoIterador(), "min"))
			throw std::invalid_argument("O iterador Inicial deve ser maior ou igual a " + getString(getFromChar(TipoIterador(), "min")));
		else if (a_vetorInicial.size() == 0)
			throw std::invalid_argument("O vetor inicial deve possuir algum elemento.");
		iteradorInicial = a_iteradorInicial;
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

	void addElemento(TipoValor a_valor) {
		try {
			if (size() == 0)
				iteradorInicial = getFromChar(TipoIterador(), "min");

			smartEnupla.push_back(a_valor);
			iteradorFinal = TipoIterador(int(smartEnupla.size()));
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::addElemento(" + getString(a_valor) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoValor a_valor) {

	void addElemento(TipoIterador a_iterador, TipoValor a_valor) {
		try {

			if (size() == 0) {
				if (a_iterador < getFromChar(TipoIterador(), "min"))
					throw std::invalid_argument("O iterador Inicial deve ser maior ou igual a " + getFullString(getFromChar(TipoIterador(), "min")));
				else if (a_iterador > getFromChar(TipoIterador(), "max"))
					throw std::invalid_argument("O iterador Inicial deve ser menor ou igual a " + getFullString(getFromChar(TipoIterador(), "max")));
				iteradorInicial = a_iterador;
				iteradorFinal = a_iterador;
				smartEnupla.push_back(a_valor);
			}
			else if (getIteradorFinal() == (a_iterador - 1)) {
				if (a_iterador > getFromChar(TipoIterador(), "max"))
					throw std::invalid_argument("O iterador Inicial deve ser menor ou igual a " + getFullString(getFromChar(TipoIterador(), "max")));
				iteradorFinal = a_iterador;
				smartEnupla.push_back(a_valor);
			}
			else if (getIteradorInicial() == (a_iterador + 1)) {
				if (a_iterador < getFromChar(TipoIterador(), "min"))
					throw std::invalid_argument("O iterador Inicial deve ser maior ou igual a " + getFullString(getFromChar(TipoIterador(), "min")));
				iteradorInicial = a_iterador;
				smartEnupla.insert(smartEnupla.begin(), a_valor);
			}
			else
				throw std::invalid_argument("Argumento Iterador - " + getFullString(a_iterador) + " - nao sequencial ao Iterador inicial: " + getFullString(getIteradorInicial()) +  " ou Iterador final: " + getFullString(getIteradorFinal()) + " da Smartenupla.");

		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::addElemento(" + getString(a_iterador) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iterador, TipoValor a_valor) {

	void setElemento(TipoIterador a_iterador, TipoValor a_valor) {
		try {
			smartEnupla.at(getIndice(a_iterador)) = a_valor;
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::setElemento(" + getString(a_iterador) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iterador, TipoValor a_valor) {

	void setElementoFromStr(TipoIterador a_iterador, std::string a_valor) {
		try {
			smartEnupla.at(getIndice(a_iterador)) = getFromString(TipoValor(), a_valor);
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::setElemento(" + getString(a_iterador) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iterador, TipoValor a_valor) {

	TipoValor getElemento(TipoIterador a_iterador)const {
		try {
			return smartEnupla.at(getIndice(a_iterador));
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getElemento(" + getString(a_iterador) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iterador, TipoValor a_valor) {

	TipoValor getTipoElemento()const {
		try {
			return TipoValor();
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getTipoElemento(): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iterador, TipoValor a_valor) {

	TipoValor at(TipoIterador a_iterador)const {
		try {
			return smartEnupla.at(getIndice(a_iterador));
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::at(" + getFullString(a_iterador) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iterador, TipoValor a_valor) {

	TipoValor& at(TipoIterador a_iterador) {
		try {
			return smartEnupla.at(getIndice(a_iterador));
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::at(" + getFullString(a_iterador) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iterador, TipoValor a_valor) {

	void alterarValorSeAlterarIterador(const TipoIterador a_iterador, const TipoValor a_valor) {
		try {

			if ((getIteradorInicial() <= a_iterador) && (a_iterador <= getIteradorFinal()))
				return;
			
			throw std::invalid_argument("Metodo nao valido para TipoIterador e TipoValor");


		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::alterarValorSeAlterarIterador(" + getFullString(a_iterador) + ",a_valor) : \n" + std::string(erro.what())); }
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

	void incrementarIterador(TipoIterador& a_iterador, const int a_inteiro) const {
		try {
			if (a_inteiro < 0)
				throw std::invalid_argument("Inteiro invalido.");
			for (int i = 1; i <= a_inteiro; i++)
				incrementarIterador(a_iterador);
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::incrementaIterador(" + getString(a_iterador) + "," + getString(a_inteiro) + "): \n" + std::string(erro.what())); }
	} // void incrementarIterador(Periodo &a_periodo, const int a_inteiro) const {

	void incrementarIteradorMinimo(TipoIterador& a_iterador) const { incrementarIterador(a_iterador); }

	void incrementarIterador(TipoIterador& a_iterador) const {
		try {
			if (size() == 0)
				throw std::invalid_argument("Nao ha elementos na SmartEnupla.");

			if ((getIteradorInicial() <= a_iterador) && (getIteradorFinal() >= a_iterador))
				a_iterador++;
			else
				throw std::invalid_argument("Iterador invalido.");
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::incrementaIterador(" + getString(a_iterador) + "): \n" + std::string(erro.what())); }
	} // void operator++(Periodo &a_periodo) const {

	void decrementarIteradorMinimo(TipoIterador& a_iterador) const { decrementarIterador(a_iterador); }

	void decrementarIterador(TipoIterador& a_iterador) const {
		try {
			if (size() == 0)
				throw std::invalid_argument("Nao ha elementos na SmartEnupla.");
			if ((getIteradorInicial() <= a_iterador) && (getIteradorFinal() >= a_iterador))
				a_iterador--;
			else
				throw std::invalid_argument("Iterador invalido.");
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::decrementarIterador(" + getString(a_iterador) + "): \n" + std::string(erro.what())); }
	} // void operator++(Periodo &a_periodo) const {

	bool isProximoIterador(const TipoIterador a_iterador) const{
		try {
			if (TipoIterador(getIteradorFinal() + 1) == a_iterador)
				return true;
			return false;
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::isProximoIterador(" + getString(a_iterador) + "): \n" + std::string(erro.what())); }
	}

	bool isIteradorValido(const TipoIterador a_iterador) const{
		try {
			if (size() == 0)
				return false;
			if ((a_iterador < getIteradorInicial()) || (getIteradorFinal() < a_iterador))
				return false;
			return true;
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::isIteradorValido(" + getString(a_iterador) + "): \n" + std::string(erro.what())); }
	}


	std::vector<TipoIterador> getIteradores(const TipoIterador a_iterador)const {
		try {

			return std::vector<TipoIterador>();

		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getIteradores(" + getFullString(a_iterador) + "): \n" + std::string(erro.what())); }
	} // SmartEnupla<Periodo, bool> getIteradores(const Periodo a_periodo)const {

	std::string str() { return "SmartEnupla<iterador,valor>"; }

	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iteradores, bool& a_cabecalho_incluir, const std::string a_string, bool a_chamada_inicial, int &a_linAdded, std::vector<std::vector<std::string>>& a_matriz_interna) {
		a_linAdded = -1;
		return std::vector<std::vector<std::string>>(); 
	};
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iteradores, bool& a_cabecalho_incluir, const char* a_char, bool a_chamada_inicial, int &a_linAdded, std::vector<std::vector<std::string>>& a_matriz_interna) {
		a_linAdded = -1;
		return std::vector<std::vector<std::string>>(); 
	};
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iteradores, bool& a_cabecalho_incluir, const bool a_bool, bool a_chamada_inicial, int &a_linAdded, std::vector<std::vector<std::string>>& a_matriz_interna) {
		a_linAdded = -1;
		return std::vector<std::vector<std::string>>(); 
	};
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iteradores, bool& a_cabecalho_incluir, const double a_double, bool a_chamada_inicial, int &a_linAdded, std::vector<std::vector<std::string>>& a_matriz_interna) {
		a_linAdded = -1;
		return std::vector<std::vector<std::string>>(); 
	};
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iteradores, bool& a_cabecalho_incluir, const int a_int, bool a_chamada_inicial, int &a_linAdded, std::vector<std::vector<std::string>>& a_matriz_interna) {
		a_linAdded = -1;
		return std::vector<std::vector<std::string>>(); 
	};
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iteradores, bool& a_cabecalho_incluir, const std::vector<int> a_vetor, bool a_chamada_inicial, int &a_linAdded, std::vector<std::vector<std::string>>& a_matriz_interna) {
		a_linAdded = -1;
		return std::vector<std::vector<std::string>>(); 
	};
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iteradores, bool& a_cabecalho_incluir, const std::vector<double> a_vetor, bool a_chamada_inicial, int &a_linAdded, std::vector<std::vector<std::string>>& a_matriz_interna) {
		a_linAdded = -1;
		return std::vector<std::vector<std::string>>(); 
	};
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iteradores, bool& a_cabecalho_incluir, const std::vector<std::vector<int>> a_vetor, bool a_chamada_inicial, int &a_linAdded, std::vector<std::vector<std::string>>& a_matriz_interna) {
		a_linAdded = -1;
		return std::vector<std::vector<std::string>>(); 
	};
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iteradores, bool& a_cabecalho_incluir, const std::vector<std::vector<double>> a_vetor, bool a_chamada_inicial, int &a_linAdded, std::vector<std::vector<std::string>>& a_matriz_interna) {
		a_linAdded = -1;
		return std::vector<std::vector<std::string>>(); 
	};

	template<typename Iterador, typename Valor>
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iteradores, const SmartEnupla<Iterador, Valor>& a_enupla) {
		std::vector<std::vector<std::string>> matriz_interna;
		bool isUltimoCabecalhoIncluso = false;
		int linAdded = 0;
		if (a_incluir_iteradores)
			return getDadosAsString(100, isUltimoCabecalhoIncluso, a_enupla, true, linAdded, matriz_interna);
		else
			return getDadosAsString(-1, isUltimoCabecalhoIncluso, a_enupla, true, linAdded, matriz_interna);
	}

	template<typename Iterador, typename Valor>
	static std::vector<std::vector<std::string>> getDadosAsString(int a_nro_iteradores_incluir, const SmartEnupla<Iterador, Valor>& a_enupla) {
		int linAdded = 0;
		std::vector<std::vector<std::string>> matriz_interna;
		bool isUltimoCabecalhoIncluso = false;
		return getDadosAsString(a_nro_iteradores_incluir, isUltimoCabecalhoIncluso, a_enupla, true, linAdded, matriz_interna);
	}

	template<typename Iterador, typename Valor>
	static std::vector<std::vector<std::string>> getDadosAsString(int a_nro_iteradores_incluir, bool &a_isUltimoCabecalhoIncluso, const SmartEnupla<Iterador, Valor>& a_enupla, bool a_chamada_inicial, int &a_linAdded, std::vector<std::vector<std::string>> &a_matriz_interna) {

		try {

			if (a_enupla.size() == 0) {
				a_linAdded = 0;
				return std::vector<std::vector<std::string>>();
			}

			const Iterador iteradorInicial = a_enupla.getIteradorInicial();
			const Iterador iteradorFinal = a_enupla.getIteradorFinal();

			int nro_iteradores_incluir = a_nro_iteradores_incluir;

			if (nro_iteradores_incluir > 0)
				nro_iteradores_incluir--;

			for (Iterador iterador = iteradorInicial; iterador <= iteradorFinal; a_enupla.incrementarIterador(iterador)) {

				SmartEnupla<int, int>::getDadosAsString(nro_iteradores_incluir, a_isUltimoCabecalhoIncluso, a_enupla.at(iterador), false, a_linAdded, a_matriz_interna);

				if ((a_linAdded > 0) && (a_nro_iteradores_incluir > 0)) {

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

				if ((a_nro_iteradores_incluir > -1) && (a_matriz_interna.size() == 0)) {
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

		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getDadosAsString(" + getFullString(a_nro_iteradores_incluir) + ",a_enupla," + getFullString(a_chamada_inicial) + "): \n" + std::string(erro.what())); }

	} // std::vector<std::vector<std::string>> SmartEnupla::getDadosAsString(const bool a_incluir_cabecalho){


	template<typename Iterador, typename Valor>
	static std::vector<std::vector<std::string>> getDadosAsString(int a_nro_iteradores_incluir, const SmartEnupla<Iterador, Valor>& a_enupla, bool a_chamada_inicial) {

		try {

			if (a_enupla.size() == 0)
				return std::vector<std::vector<std::string>>();

			const Iterador iteradorInicial = a_enupla.getIteradorInicial();
			const Iterador iteradorFinal = a_enupla.getIteradorFinal();

			std::vector<std::vector<std::string>> matriz_retorno;

			// Ultima camada da SmarEnupla
			if (SmartEnupla<int, int>::getDadosAsString(a_nro_iteradores_incluir, a_enupla.at(a_enupla.getIteradorInicial()), false).size() == 0) {

				if (a_nro_iteradores_incluir > -1)
					matriz_retorno = std::vector<std::vector<std::string>>(2, std::vector<std::string>(a_enupla.size(), ""));
				else
					matriz_retorno = std::vector<std::vector<std::string>>(1, std::vector<std::string>(a_enupla.size(), ""));

				int i = 0;
				for (Iterador iterador = iteradorInicial; iterador <= iteradorFinal; a_enupla.incrementarIterador(iterador)) {

					if (a_nro_iteradores_incluir > -1) {
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

				int nro_iteradores_incluir = a_nro_iteradores_incluir;

				if (nro_iteradores_incluir > 0)
					nro_iteradores_incluir--;

				for (Iterador iterador = iteradorInicial; iterador <= iteradorFinal; a_enupla.incrementarIterador(iterador)) {

					const std::vector<std::vector<std::string>> matriz_retornada = SmartEnupla<int, int>::getDadosAsString(nro_iteradores_incluir, a_enupla.at(iterador), false);

					if (matriz_retornada.size() > 0) {

						for (int l = 0; l < matriz_retornada.size(); l++) {
							if (matriz_retornada.at(l).size() > 0) {

								if (a_nro_iteradores_incluir > 0) {

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

								} // if (a_nro_iteradores_incluir > -1) {

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

		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getDadosAsString(" + getFullString(a_nro_iteradores_incluir) + ",a_enupla," + getFullString(a_chamada_inicial) + "): \n" + std::string(erro.what())); }

	} // std::vector<std::vector<std::string>> SmartEnupla::getDadosAsString(const bool a_incluir_cabecalho){

private:

	int getIndice(TipoIterador a_iterador) const {
		try {
			return int(a_iterador - getIteradorInicial());
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getIndice(" + getString(a_iterador) + "): \n" + std::string(erro.what())); }
	}; // int getIndice(TipoIterador a_iterador) {

	TipoIterador iteradorInicial;
	TipoIterador iteradorFinal;

	std::vector<TipoValor> smartEnupla;

}; // class SmartEnupla {


//
// Declaração Especializada para Periodo 
//
template<class TipoValor>
class SmartEnupla <Periodo, TipoValor> {
public:

	SmartEnupla() { inicializacao(0); };

	SmartEnupla(const int a_alocacao) { inicializacao(a_alocacao); };

	SmartEnupla(const Periodo a_periodoInicial, const std::vector<TipoValor> a_vetorInicial) {
		inicializacao(int(a_vetorInicial.size()));

		if (!a_periodoInicial.isValido())
			throw std::invalid_argument("O periodo inicial deve ser valido.");
		else if (a_vetorInicial.size() == 0)
			throw std::invalid_argument("O vetor inicial deve possuir algum elemento.");

		lista_estrutura.at(0).tipo_iterador_inicial = a_periodoInicial.getTipoPeriodo();
		lista_estrutura.at(0).tipo_iterador_final = lista_estrutura.at(0).tipo_iterador_inicial;

		numero_elementos = int(a_vetorInicial.size());

		lista_estrutura.at(0).iteradores_iniciais.at(lista_estrutura.at(0).tipo_iterador_inicial).push_back(a_periodoInicial);
		lista_estrutura.at(0).iteradores_finais.at(lista_estrutura.at(0).tipo_iterador_final).push_back(a_periodoInicial + numero_elementos - 1);

		lista_estrutura.at(0).smartEnupla.at(lista_estrutura.at(0).tipo_iterador_final) = a_vetorInicial;

	}; // SmartEnupla(const Periodo a_periodoInicial, const std::vector<TipoValor> a_vetorInicial) {

	template<typename TipoValor2>
	SmartEnupla(const SmartEnupla<Periodo, TipoValor2>& a_smartEnupla_inicializacao, TipoValor a_valor_inicializacao) {
		inicializacao(a_smartEnupla_inicializacao.size());
		for (Periodo periodo = a_smartEnupla_inicializacao.getIteradorInicial(); periodo <= a_smartEnupla_inicializacao.getIteradorFinal(); a_smartEnupla_inicializacao.incrementarIterador(periodo))
			addElemento(periodo, a_valor_inicializacao);
	}; // SmartEnupla(const SmartEnupla<Periodo, TipoValor2> &a_smartEnupla_inicializacao, TipoValor a_valor_inicializacao) {

	int size() const { return numero_elementos; };

	Periodo getIteradorInicial()const {
		try {
			if (size() == 0)
				throw std::invalid_argument("Iterador inicial nao definido.");
			return lista_estrutura.at(0).iteradores_iniciais.at(lista_estrutura.at(0).tipo_iterador_inicial).at(0);
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getIteradorInicial(): \n" + std::string(erro.what())); }
	}; // Periodo getIteradorInicial()const {

	std::vector<std::vector <std::vector<Periodo>>> getIteradoresIniciais()const {
		try {
			if (size() == 0)
				throw std::invalid_argument("Iterador inicial nao definido.");

			std::vector<std::vector <std::vector<Periodo>>> vetor_retorno;

			for (int estrutura = 0; estrutura < int(lista_estrutura.size()); estrutura++)
				vetor_retorno.push_back(lista_estrutura.at(estrutura).iteradores_iniciais);

			return vetor_retorno;
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getIteradoresIniciais(): \n" + std::string(erro.what())); }
	} // std::vector<Periodo> getIteradoresIniciais()const {

	Periodo getIteradorFinal()const {
		try {
			if (size() == 0)
				throw std::invalid_argument("Iterador final nao definido.");
			return lista_estrutura.at(int(lista_estrutura.size())-1).iteradores_finais.at(lista_estrutura.at(int(lista_estrutura.size()) - 1).tipo_iterador_final).at(0);
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getIteradorFinal(): \n" + std::string(erro.what())); }
	}; // Periodo getIteradorFinal()const {

	std::vector<std::vector<std::vector<Periodo>>> getIteradoresFinais()const {
		try {
			if (size() == 0)
				throw std::invalid_argument("Iterador final nao definido.");

			std::vector<std::vector <std::vector<Periodo>>> vetor_retorno;

			for (int estrutura = 0; estrutura < int(lista_estrutura.size()); estrutura++)
				vetor_retorno.push_back(lista_estrutura.at(estrutura).iteradores_finais);

			return vetor_retorno;
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getIteradoresFinais(): \n" + std::string(erro.what())); }
	} // std::vector<Periodo> getIteradoresIniciais()const {


	Periodo getIterador(const Periodo a_periodo_interno)const {
		try {

			for (Periodo periodo_externo = getIteradorInicial(); periodo_externo <= getIteradorFinal(); incrementarIterador(periodo_externo)) {

				Periodo periodo_externo_seguinte = periodo_externo;
				incrementarIterador(periodo_externo_seguinte);

				if ((periodo_externo <= a_periodo_interno) && (a_periodo_interno < periodo_externo_seguinte))
					return periodo_externo;
				else if (a_periodo_interno >= getIteradorFinal())
					return getIteradorFinal();

			} // for (Periodo periodo_externo = getIteradorInicial(); periodo_externo <= getIteradorFinal(); incrementarIterador(periodo_externo)) {

			throw std::invalid_argument("Iterador nao encontrado.");

		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getIteradores(" + getFullString(a_periodo_interno) + "): \n" + std::string(erro.what())); }
	} // Periodo getIterador(const Periodo a_periodo_interno)const {


	std::vector<Periodo> getIteradores(const Periodo a_periodo_externo)const {
		try {

			if (size() == 0)
				throw std::invalid_argument("Iteradores nao definidos.");

			Periodo periodo_inicial;

			const int indice_estrutura = getIndiceEstruturaSeExistir(a_periodo_externo);
			if (indice_estrutura > -1)
				periodo_inicial = lista_estrutura.at(indice_estrutura).iteradores_iniciais.at(lista_estrutura.at(indice_estrutura).tipo_iterador_inicial).at(0);
			else
				periodo_inicial = getIteradorInicial();

			const Periodo periodo_externo_seguinte = a_periodo_externo + 1;

			std::vector<Periodo> iteradores;

			bool is_sobreposicao = false;

			for (Periodo periodo_interno = periodo_inicial; periodo_interno <= getIteradorFinal(); incrementarIterador(periodo_interno)) {

				const double sobreposicao = a_periodo_externo.sobreposicao(periodo_interno);

				if ((sobreposicao > 0.0) && (!is_sobreposicao)) {
					iteradores.push_back(periodo_interno);
					is_sobreposicao = true;
				}
				else if ((sobreposicao > 0.0) && (is_sobreposicao))
					iteradores.push_back(periodo_interno);

				else if ((sobreposicao == 0.0) && (is_sobreposicao))
					return iteradores;

			} // for (Periodo periodo_interno = getIteradorInicial(); periodo_interno <= getIteradorFinal(); incrementarIterador(periodo_interno)) {

			return iteradores;

		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getIteradores(" + getFullString(a_periodo_externo) + "): \n" + std::string(erro.what())); }
	} // SmartEnupla<Periodo, bool> getIteradores(const Periodo a_periodo)const {
	
	std::vector<Periodo> getIteradores(const Periodo a_periodo_inicial, const Periodo a_periodo_final)const {
		try {

			if (size() == 0)
				throw std::invalid_argument("Iteradores nao definidos.");

			if(a_periodo_final < a_periodo_inicial)
				throw std::invalid_argument("Periodo inicial maior ao periodo final ");

			std::vector<Periodo> iteradores;

			for (Periodo periodo_interno = getIteradorInicial(); periodo_interno <= getIteradorFinal(); incrementarIterador(periodo_interno)) {

				if ((a_periodo_inicial <= periodo_interno) && (periodo_interno <= a_periodo_final))
					iteradores.push_back(periodo_interno);

			} // for (Periodo periodo_interno = getIteradorInicial(); periodo_interno <= getIteradorFinal(); incrementarIterador(periodo_interno)) {

			return iteradores;

		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getIteradores(" + getFullString(a_periodo_inicial) + getFullString(a_periodo_final) + "): \n" + std::string(erro.what())); }
	} 

	void incrementarIterador(Periodo &a_periodo, const int a_inteiro) const {
		try {
			if (a_inteiro < 0)
				throw std::invalid_argument("Inteiro invalido.");
			for (int i = 1; i <= a_inteiro; i++)
				incrementarIterador(a_periodo);
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::incrementaIterador(" + getString(a_periodo) + "," + getString(a_inteiro) + "): \n" + std::string(erro.what())); }
	} // void incrementarIterador(Periodo &a_periodo, const int a_inteiro) const {

	void incrementarIteradorMinimo(Periodo& a_periodo) const {
		try {
			
			a_periodo = Periodo(TipoPeriodo_meia_hora, a_periodo) + 1;

		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::incrementarIteradorMinimo(" + getString(a_periodo) + "): \n" + std::string(erro.what())); }

	}

	void incrementarIterador(Periodo& a_periodo) const {
		try {

			const TipoPeriodo tipo = a_periodo.getTipoPeriodo();


			//////////////////////////////////////////////////
			//Identifica a estrutura a qual pertence o periodo
			//////////////////////////////////////////////////

			int indiceEstrutura = getIndiceEstrutura(a_periodo);

			//////////////////////////////////////////////////

			if (indiceEstrutura == -1) {
				const Periodo periodo_final = getIteradorFinal();

				if ((periodo_final.getTipoPeriodo() == tipo) && (periodo_final <= a_periodo)) {
					a_periodo++;
					return;
				}
				throw std::invalid_argument("Iterador invalido.");
			}

			else if (a_periodo < lista_estrutura.at(indiceEstrutura).iteradores_finais.at(tipo).at(0)) {
				a_periodo++;
				a_periodo.estrutura = indiceEstrutura + 1;
			}
			else if (a_periodo == lista_estrutura.at(indiceEstrutura).iteradores_finais.at(tipo).at(0)) {

				//////////////////////////////////
				// Tipo_iterador_final
				//////////////////////////////////

				if (tipo == lista_estrutura.at(indiceEstrutura).tipo_iterador_final) {
					
					if (indiceEstrutura == int(lista_estrutura.size()) - 1) {//Se for a última estrutura, o iterador é incrementado naquele tipo mesmo fugindo do range da SmartEnupla.
						a_periodo++;
						return;
					}//if (indiceEstrutura == int(lista_estrutura.size()) - 1) {
					else { //Pega o iterador inicial da próxima estrutura						
						indiceEstrutura++;
						a_periodo = lista_estrutura.at(indiceEstrutura).iteradores_iniciais.at(lista_estrutura.at(indiceEstrutura).tipo_iterador_inicial).at(0);
						a_periodo.estrutura = indiceEstrutura + 1;
						return;
					}//else {
				}//if (tipo == lista_estrutura.at(indiceEstrutura).tipo_iterador_final) {

				//////////////////////////////////
				// Estrutura crescente: 
				//////////////////////////////////

				if (lista_estrutura.at(indiceEstrutura).tipoEstruturaPeriodo == TipoEstruturaPeriodo_crescente) {
					for (TipoPeriodo tipoPeriodo = TipoPeriodo(tipo - 1); tipoPeriodo >= lista_estrutura.at(indiceEstrutura).tipo_iterador_final; tipoPeriodo--) {
						if (lista_estrutura.at(indiceEstrutura).iteradores_iniciais.at(tipoPeriodo).size() > 0) {
							a_periodo = lista_estrutura.at(indiceEstrutura).iteradores_iniciais.at(tipoPeriodo).at(0);
							a_periodo.estrutura = indiceEstrutura + 1;
							return;
						}//if (iteradores_iniciais.at(tipoPeriodo).size() > 0) {
					}//for (TipoPeriodo tipoPeriodo = TipoPeriodo(tipo - 1); tipoPeriodo >= lista_estrutura.at(indiceEstrutura).tipo_iterador_final; tipoPeriodo--) {
				}//if (lista_estrutura.at(indiceEstrutura).tipoEstruturaPeriodo == TipoEstruturaPeriodo_crescente) {

				//////////////////////////////////
				// Estrutura decrescente: 
				//////////////////////////////////

				else if (lista_estrutura.at(indiceEstrutura).tipoEstruturaPeriodo == TipoEstruturaPeriodo_decrescente) {
					for (TipoPeriodo tipoPeriodo = TipoPeriodo(tipo + 1); tipoPeriodo <= lista_estrutura.at(indiceEstrutura).tipo_iterador_final; tipoPeriodo++) {
						if (lista_estrutura.at(indiceEstrutura).iteradores_iniciais.at(tipoPeriodo).size() > 0) {
							a_periodo = lista_estrutura.at(indiceEstrutura).iteradores_iniciais.at(tipoPeriodo).at(0);
							a_periodo.estrutura = indiceEstrutura + 1;
							return;
						}//if (iteradores_iniciais.at(tipoPeriodo).size() > 0) {
					}//for (TipoPeriodo tipoPeriodo = TipoPeriodo(tipo + 1); tipoPeriodo <= lista_estrutura.at(indiceEstrutura).tipo_iterador_final; tipoPeriodo++) {
				}//else if (lista_estrutura.at(indiceEstrutura).tipoEstruturaPeriodo == TipoEstruturaPeriodo_decrescente) {
			
			} // else if (a_periodo == iteradores_finais.at(tipo).at(0)) {
			else
				throw std::invalid_argument("Iterador invalido.");

		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::incrementaIterador(" + getString(a_periodo) + "): \n" + std::string(erro.what())); }
	} // void operator++(Periodo &a_periodo) const {

	void decrementarIteradorMinimo(Periodo& a_periodo) const {
		try {

			a_periodo = Periodo(TipoPeriodo_meia_hora, a_periodo) - 1;

		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::incrementarIteradorMinimo(" + getString(a_periodo) + "): \n" + std::string(erro.what())); }

	}

	void decrementarIterador(Periodo& a_periodo) const {
		try {


			const TipoPeriodo tipo = a_periodo.getTipoPeriodo();

			//////////////////////////////////////////////////
			//Identifica a estrutura a qual pertence o periodo
			//////////////////////////////////////////////////

			int indiceEstrutura = getIndiceEstrutura(a_periodo);

			//////////////////////////////////////////////////

			if (indiceEstrutura == -1) {
				const Periodo periodo_inicial = getIteradorInicial();

				if ((periodo_inicial.getTipoPeriodo() == tipo) && (periodo_inicial >= a_periodo)) {
					a_periodo--;
					return;
				}
				throw std::invalid_argument("Iterador invalido.");
			}
			else if (a_periodo > lista_estrutura.at(indiceEstrutura).iteradores_iniciais.at(tipo).at(0)){
				a_periodo--;
				a_periodo.estrutura = indiceEstrutura + 1;
			}
			else if (a_periodo == lista_estrutura.at(indiceEstrutura).iteradores_iniciais.at(tipo).at(0)) {

				//////////////////////////////////
				// Tipo_iterador_final
				//////////////////////////////////

				if (tipo == lista_estrutura.at(indiceEstrutura).tipo_iterador_inicial) {

					if (indiceEstrutura == 0) {//Se for a primeira estrutura, o iterador é decrementado naquele tipo mesmo fugindo do range da SmartEnupla.
						a_periodo--;
						return;
					}//if (indiceEstrutura == 0) {
					else { //Pega o iterador inicial da próxima estrutura						
						indiceEstrutura--;
						a_periodo = lista_estrutura.at(indiceEstrutura).iteradores_finais.at(lista_estrutura.at(indiceEstrutura).tipo_iterador_final).at(0);
						a_periodo.estrutura = indiceEstrutura + 1;
						return;
					}//else {
				}//if (tipo == lista_estrutura.at(indiceEstrutura).tipo_iterador_inicial) {


				//////////////////////////////////
				// Estrutura crescente: 
				//////////////////////////////////

				if (lista_estrutura.at(indiceEstrutura).tipoEstruturaPeriodo == TipoEstruturaPeriodo_crescente) {
					for (TipoPeriodo tipoPeriodo = TipoPeriodo(tipo + 1); tipoPeriodo <= lista_estrutura.at(indiceEstrutura).tipo_iterador_inicial; tipoPeriodo++) {
						if (lista_estrutura.at(indiceEstrutura).iteradores_finais.at(tipoPeriodo).size() > 0) {
							a_periodo = lista_estrutura.at(indiceEstrutura).iteradores_finais.at(tipoPeriodo).at(0);
							a_periodo.estrutura = indiceEstrutura + 1;
							return;
						}//if (iteradores_finais.at(tipoPeriodo).size() > 0) {
					}//for (TipoPeriodo tipoPeriodo = TipoPeriodo(tipo + 1); tipoPeriodo <= lista_estrutura.at(indiceEstrutura).tipo_iterador_inicial; tipoPeriodo++) {
				}//if (lista_estrutura.at(indiceEstrutura).tipoEstruturaPeriodo == TipoEstruturaPeriodo_crescente) {

				//////////////////////////////////
				// Estrutura decrescente: 
				//////////////////////////////////

				if (lista_estrutura.at(indiceEstrutura).tipoEstruturaPeriodo == TipoEstruturaPeriodo_decrescente) {
					for (TipoPeriodo tipoPeriodo = TipoPeriodo(tipo - 1); tipoPeriodo >= lista_estrutura.at(indiceEstrutura).tipo_iterador_inicial; tipoPeriodo--) {
						if (lista_estrutura.at(indiceEstrutura).iteradores_finais.at(tipoPeriodo).size() > 0) {
							a_periodo = lista_estrutura.at(indiceEstrutura).iteradores_finais.at(tipoPeriodo).at(0);
							a_periodo.estrutura = indiceEstrutura + 1;
							return;
						}//if (iteradores_finais.at(tipoPeriodo).size() > 0) {
					}//for (TipoPeriodo tipoPeriodo = TipoPeriodo(tipo - 1); tipoPeriodo >= lista_estrutura.at(indiceEstrutura).tipo_iterador_inicial; tipoPeriodo--) {
				}//if (lista_estrutura.at(indiceEstrutura).tipoEstruturaPeriodo == TipoEstruturaPeriodo_crescente) {

			} // else if (a_periodo == iteradores_finais.at(tipo).at(0)) {
			else
				throw std::invalid_argument("Iterador invalido.");
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::decrementarIterador(" + getString(a_periodo) + "): \n" + std::string(erro.what())); }
	} // void operator++(Periodo &a_periodo) const {

	void decrementarIterador(Periodo& a_periodo, const int a_inteiro) const {
		try {
			if (a_inteiro < 0)
				throw std::invalid_argument("Inteiro invalido.");
			for (int i = 1; i <= a_inteiro; i++)
				decrementarIterador(a_periodo);
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::decrementarIterador(" + getString(a_periodo) + "," + getString(a_inteiro) + "): \n" + std::string(erro.what())); }
	} // void incrementarIterador(Periodo &a_periodo, const int a_inteiro) const {

	bool isProximoIterador(const Periodo a_iterador) const {
		try {
			if (Periodo(a_iterador.getTipoPeriodo(), getIteradorFinal() + 1) == a_iterador)
				return true;
			return false;
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::isProximoIterador(" + getString(a_iterador) + "): \n" + std::string(erro.what())); }
	}

	bool isIteradorValido(const Periodo a_iterador) const {
		try {
			if (size() == 0)
				return false;
			else if ((a_iterador < getIteradorInicial()) || (getIteradorFinal() < a_iterador))
				return false;
			else if (getIndiceEstruturaSeExistir(a_iterador) == -1)
				return false;
			return true;
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::isIteradorValido(" + getString(a_iterador) + "): \n" + std::string(erro.what())); }
	}

	void addElemento(TipoValor a_valor) {
		try { throw std::invalid_argument("Para SmartEnupla com tipo de iterador - Periodo - o valor do iterador deve ser informado."); } // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::addElemento(" + getString(a_valor) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoValor a_valor) {

	void addElemento(Periodo a_iterador, TipoValor a_valor) {
		try {
			if (!a_iterador.isValido())
				throw std::invalid_argument("Periodo Invalido");

			else if (size() == 0) {
				lista_estrutura.at(0).tipo_iterador_inicial = a_iterador.getTipoPeriodo();
				lista_estrutura.at(0).tipo_iterador_final = lista_estrutura.at(0).tipo_iterador_inicial;

				lista_estrutura.at(0).iteradores_iniciais.at(lista_estrutura.at(0).tipo_iterador_final).push_back(a_iterador);
				lista_estrutura.at(0).iteradores_finais.at(lista_estrutura.at(0).tipo_iterador_final).push_back(a_iterador);

				lista_estrutura.at(0).smartEnupla.at(lista_estrutura.at(0).tipo_iterador_final).push_back(a_valor);
				numero_elementos++;

			} // else if (size() == 0) {
			
			else if (a_iterador + 1 == Periodo(a_iterador.getTipoPeriodo(), getIteradorInicial())) {

				////////////////////////////////////
				//Câmbio de Tipo de Periodo
				////////////////////////////////////

				if (a_iterador.getTipoPeriodo() != lista_estrutura.at(0).tipo_iterador_inicial) {

					//////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Estrutura_flat: Mantém a mesma estrutura e identifica se a lista é crescente ou decrescente
					//////////////////////////////////////////////////////////////////////////////////////////////////////////////

					if (lista_estrutura.at(0).tipoEstruturaPeriodo == TipoEstruturaPeriodo_flat && a_iterador.getTipoPeriodo() > lista_estrutura.at(0).tipo_iterador_inicial)
						lista_estrutura.at(0).tipoEstruturaPeriodo = TipoEstruturaPeriodo_crescente;

					else if (lista_estrutura.at(0).tipoEstruturaPeriodo == TipoEstruturaPeriodo_flat && a_iterador.getTipoPeriodo() < lista_estrutura.at(0).tipo_iterador_inicial)
						lista_estrutura.at(0).tipoEstruturaPeriodo = TipoEstruturaPeriodo_decrescente;

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Estrutura_crescente/decrescente : 
					//Caso crescente: Cria uma nova estrutura, se o tipo_periodo > tipo_iterador_inicial
					//Caso decrescente: Cria uma nova estrutura, se o tipo_periodo < tipo_iterador_inicial
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					else if ((lista_estrutura.at(0).tipoEstruturaPeriodo == TipoEstruturaPeriodo_crescente && a_iterador.getTipoPeriodo() < lista_estrutura.at(0).tipo_iterador_inicial) \
						|| (lista_estrutura.at(0).tipoEstruturaPeriodo == TipoEstruturaPeriodo_decrescente && a_iterador.getTipoPeriodo() > lista_estrutura.at(0).tipo_iterador_inicial)) {
						inicializacaoEstruturaPeriodo(0, 0);//Cria uma nova estrutura
						lista_estrutura.at(0).tipo_iterador_final = a_iterador.getTipoPeriodo();
					}

				}//if (a_iterador.getTipoPeriodo() != lista_estrutura.at(0).tipo_iterador_inicial) {

				////////////////////////////////////////////

				lista_estrutura.at(0).tipo_iterador_inicial = a_iterador.getTipoPeriodo();

				if (lista_estrutura.at(0).iteradores_iniciais.at(lista_estrutura.at(0).tipo_iterador_inicial).size() == 0)
					lista_estrutura.at(0).iteradores_iniciais.at(lista_estrutura.at(0).tipo_iterador_inicial).push_back(a_iterador);
				else
					lista_estrutura.at(0).iteradores_iniciais.at(lista_estrutura.at(0).tipo_iterador_inicial).at(0) = a_iterador;

				if (lista_estrutura.at(0).iteradores_finais.at(lista_estrutura.at(0).tipo_iterador_inicial).size() == 0)
					lista_estrutura.at(0).iteradores_finais.at(lista_estrutura.at(0).tipo_iterador_inicial).push_back(a_iterador);

				lista_estrutura.at(0).smartEnupla.at(lista_estrutura.at(0).tipo_iterador_inicial).insert(lista_estrutura.at(0).smartEnupla.at(lista_estrutura.at(0).tipo_iterador_inicial).begin(), a_valor);
				numero_elementos++;

			}

			else if (Periodo(a_iterador.getTipoPeriodo(), getIteradorFinal() + 1) == a_iterador) {

				////////////////////////////////////
				//Câmbio de Tipo de Periodo
				////////////////////////////////////

				if (a_iterador.getTipoPeriodo() != lista_estrutura.at(int(lista_estrutura.size()) - 1).tipo_iterador_final) {

					//////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Estrutura_flat: Mantém a mesma estrutura e identifica se a lista é crescente ou decrescente
					//////////////////////////////////////////////////////////////////////////////////////////////////////////////

					if (lista_estrutura.at(int(lista_estrutura.size()) - 1).tipoEstruturaPeriodo == TipoEstruturaPeriodo_flat && a_iterador.getTipoPeriodo() < lista_estrutura.at(int(lista_estrutura.size()) - 1).tipo_iterador_final)
						lista_estrutura.at(int(lista_estrutura.size()) - 1).tipoEstruturaPeriodo = TipoEstruturaPeriodo_crescente;

					else if (lista_estrutura.at(int(lista_estrutura.size()) - 1).tipoEstruturaPeriodo == TipoEstruturaPeriodo_flat && a_iterador.getTipoPeriodo() > lista_estrutura.at(int(lista_estrutura.size()) - 1).tipo_iterador_final)
						lista_estrutura.at(int(lista_estrutura.size()) - 1).tipoEstruturaPeriodo = TipoEstruturaPeriodo_decrescente;

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Estrutura_crescente/decrescente : 
					//Caso crescente: Cria uma nova estrutura, se o tipo_periodo < tipo_iterador_final
					//Caso decrescente: Cria uma nova estrutura, se o tipo_periodo > tipo_iterador_final
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					else if ((lista_estrutura.at(int(lista_estrutura.size()) - 1).tipoEstruturaPeriodo == TipoEstruturaPeriodo_crescente && a_iterador.getTipoPeriodo() > lista_estrutura.at(int(lista_estrutura.size()) - 1).tipo_iterador_final) \
						|| (lista_estrutura.at(int(lista_estrutura.size()) - 1).tipoEstruturaPeriodo == TipoEstruturaPeriodo_decrescente && a_iterador.getTipoPeriodo() < lista_estrutura.at(int(lista_estrutura.size()) - 1).tipo_iterador_final)) {
						inicializacaoEstruturaPeriodo(-1, 0);//Cria uma nova estrutura
						lista_estrutura.at(int(lista_estrutura.size()) - 1).tipo_iterador_inicial = a_iterador.getTipoPeriodo();
					}

				}//if (a_iterador.getTipoPeriodo() != lista_estrutura.at(int(lista_estrutura.size()) - 1).tipo_iterador_final) {

				////////////////////////////////////////////

				lista_estrutura.at(int(lista_estrutura.size()) - 1).tipo_iterador_final = a_iterador.getTipoPeriodo();

				if (lista_estrutura.at(int(lista_estrutura.size()) - 1).iteradores_iniciais.at(lista_estrutura.at(int(lista_estrutura.size()) - 1).tipo_iterador_final).size() == 0)
					lista_estrutura.at(int(lista_estrutura.size()) - 1).iteradores_iniciais.at(lista_estrutura.at(int(lista_estrutura.size()) - 1).tipo_iterador_final).push_back(a_iterador);

				if (lista_estrutura.at(int(lista_estrutura.size()) - 1).iteradores_finais.at(lista_estrutura.at(int(lista_estrutura.size()) - 1).tipo_iterador_final).size() == 0)
					lista_estrutura.at(int(lista_estrutura.size()) - 1).iteradores_finais.at(lista_estrutura.at(int(lista_estrutura.size()) - 1).tipo_iterador_final).push_back(a_iterador);
				else
					lista_estrutura.at(int(lista_estrutura.size()) - 1).iteradores_finais.at(lista_estrutura.at(int(lista_estrutura.size()) - 1).tipo_iterador_final).at(0) = a_iterador;

				lista_estrutura.at(int(lista_estrutura.size()) - 1).smartEnupla.at(lista_estrutura.at(int(lista_estrutura.size()) - 1).tipo_iterador_final).push_back(a_valor);
				numero_elementos++;
			}

			else
				throw std::invalid_argument("Argumento Iterador - " + getString(a_iterador) + " - nao sequencial ao Iterador inicial: " + getString(getIteradorInicial()) + " ou final: " + getString(getIteradorFinal()) + " do SmartEnupla.");

		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::addElemento(" + getString(a_iterador) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(Periodo a_iterador, TipoValor a_valor) {


	void alterarValorSeAlterarIterador(const Periodo a_iterador, TipoValor a_valor) {
		try {

			std::vector<Periodo> iteradores = getIteradores(a_iterador);

			if (iteradores.size() == 0)
				throw std::invalid_argument("Nao ha iteradores compativeis.");
			else if (iteradores.size() == 1) {
				if (iteradores.at(0) == a_iterador)
					return;
			}

			const Periodo periodo_inicial_orig = iteradores.at(0);
			const Periodo periodo_final_orig = iteradores.at(iteradores.size() - 1);

			if ((Periodo(periodo_inicial_orig.getTipoPeriodo(), a_iterador) != periodo_inicial_orig) || \
				(Periodo(periodo_final_orig.getTipoPeriodo(), a_iterador + 1) - 1 != periodo_final_orig))
					throw std::invalid_argument("Periodo nao se encaixa exatamente em range de iteradores.");


			TipoEstruturaPeriodo tipo_estrutura_anterior = TipoEstruturaPeriodo_Nenhum;
			TipoPeriodo tipo_periodo_anterior = TipoPeriodo_Nenhum;
			int indice_estrutura_anterior = -1;
			Periodo periodo_anterior = periodo_inicial_orig;
			if (periodo_inicial_orig > getIteradorInicial()) {
				decrementarIterador(periodo_anterior);
				indice_estrutura_anterior = getIndiceEstrutura(periodo_anterior);
				tipo_estrutura_anterior = lista_estrutura.at(indice_estrutura_anterior).tipoEstruturaPeriodo;
				tipo_periodo_anterior = periodo_anterior.getTipoPeriodo();
			}

			TipoEstruturaPeriodo tipo_estrutura_posterior = TipoEstruturaPeriodo_Nenhum;
			TipoPeriodo tipo_periodo_posterior = TipoPeriodo_Nenhum;
			int indice_estrutura_posterior = -1;
			Periodo periodo_posterior = periodo_final_orig; 
			if (periodo_final_orig < getIteradorFinal()){
				incrementarIterador(periodo_posterior);
				indice_estrutura_posterior = getIndiceEstrutura(periodo_posterior);
				tipo_estrutura_posterior = lista_estrutura.at(indice_estrutura_posterior).tipoEstruturaPeriodo;
				tipo_periodo_posterior = periodo_posterior.getTipoPeriodo();
			}

			std::vector<EstruturaPeriodo> lista_estrutura_copia = lista_estrutura;

			bool estruturas_anterior_posterior_duplicadas = false;
			// Verifica necessidade de duplicar estrutura atual
			if ((tipo_estrutura_anterior != TipoEstruturaPeriodo_Nenhum) && (tipo_estrutura_posterior != TipoEstruturaPeriodo_Nenhum)) {
				// Caso estruturas anterior e posterior sejam a mesma, duplica-se estrutura para inclusao entre ambas
				if (indice_estrutura_anterior == indice_estrutura_posterior) {
					lista_estrutura_copia.insert(lista_estrutura_copia.begin() + indice_estrutura_anterior, lista_estrutura_copia.at(indice_estrutura_anterior));
					indice_estrutura_posterior++;
					estruturas_anterior_posterior_duplicadas = true;
				}
			}// if ((tipo_estrutura_anterior != TipoEstruturaPeriodo_Nenhum) && (tipo_estrutura_posterior != TipoEstruturaPeriodo_Nenhum)) {

			const TipoPeriodo tipo_periodo_iterador = a_iterador.getTipoPeriodo();
			
			int status_inclusao_iterador = -1; // -1 Iterador novo em nova estrutura a ser criada | 0 iterador novo na estrutura de inicio | 1 iterador novo na estrutura do fim

			// Iterador novo em toda enupla.
			if ((tipo_estrutura_anterior == TipoEstruturaPeriodo_Nenhum) && (tipo_estrutura_posterior == TipoEstruturaPeriodo_Nenhum))
				lista_estrutura_copia = std::vector<EstruturaPeriodo>();

			// Iterador novo no inicio da enupla
			else if ((tipo_estrutura_anterior == TipoEstruturaPeriodo_Nenhum) && (tipo_estrutura_posterior != TipoEstruturaPeriodo_Nenhum)) {

				// Remove estruturas dos periodos que sairao
				const int nro_estruturas_a_remover = indice_estrutura_posterior;
				for (int i = 0; i < nro_estruturas_a_remover; i++) {
					lista_estrutura_copia.erase(lista_estrutura_copia.begin());
					indice_estrutura_posterior--;
				}

				// Realiza adequacoes na estrutura posterior
				if (tipo_periodo_posterior > lista_estrutura_copia.at(indice_estrutura_posterior).tipo_iterador_final) {
					tipo_estrutura_posterior = TipoEstruturaPeriodo_crescente;
					if (tipo_periodo_iterador >= tipo_periodo_posterior)
						status_inclusao_iterador = 1;
				}
				else if (tipo_periodo_posterior < lista_estrutura_copia.at(indice_estrutura_posterior).tipo_iterador_final) {
					tipo_estrutura_posterior = TipoEstruturaPeriodo_decrescente;
					if (tipo_periodo_iterador <= tipo_periodo_posterior)
						status_inclusao_iterador = 1;
				}
				else if (tipo_periodo_posterior == lista_estrutura_copia.at(indice_estrutura_posterior).tipo_iterador_final) {
					status_inclusao_iterador = 1;
					if (tipo_periodo_iterador >= tipo_periodo_posterior)
						tipo_estrutura_posterior = TipoEstruturaPeriodo_crescente;
					else if (tipo_periodo_iterador <= tipo_periodo_posterior)
						tipo_estrutura_posterior = TipoEstruturaPeriodo_decrescente;
					else
						tipo_estrutura_posterior = TipoEstruturaPeriodo_flat;
				}

				lista_estrutura_copia.at(indice_estrutura_posterior).tipoEstruturaPeriodo = tipo_estrutura_posterior;
					
			} // else if ((tipo_estrutura_anterior == TipoEstruturaPeriodo_Nenhum) && (tipo_estrutura_posterior != TipoEstruturaPeriodo_Nenhum)) {

			// Iterador novo no final da enupla
			else if ((tipo_estrutura_anterior != TipoEstruturaPeriodo_Nenhum) && (tipo_estrutura_posterior == TipoEstruturaPeriodo_Nenhum)) {

				// Remove estruturas dos periodos que sairao
				const int nro_estruturas_a_remover = int(lista_estrutura_copia.size()) - (indice_estrutura_anterior + 1);
				for (int i = 0; i < nro_estruturas_a_remover; i++)
					lista_estrutura_copia.pop_back();

				if (lista_estrutura_copia.at(indice_estrutura_anterior).tipo_iterador_inicial > tipo_periodo_anterior) {
					tipo_estrutura_anterior = TipoEstruturaPeriodo_crescente;
					if (tipo_periodo_anterior >= tipo_periodo_iterador)
						status_inclusao_iterador = 0;
				}
				else if (lista_estrutura_copia.at(indice_estrutura_anterior).tipo_iterador_inicial < tipo_periodo_anterior) {
					tipo_estrutura_anterior = TipoEstruturaPeriodo_decrescente;
					if (tipo_periodo_anterior <= tipo_periodo_iterador)
						status_inclusao_iterador = 0;
				}
				else if (lista_estrutura_copia.at(indice_estrutura_anterior).tipo_iterador_inicial == tipo_periodo_anterior) {
					status_inclusao_iterador = 0;
					if (tipo_periodo_anterior >= tipo_periodo_iterador)
						tipo_estrutura_anterior = TipoEstruturaPeriodo_crescente;
					else if (tipo_periodo_anterior <= tipo_periodo_iterador)
						tipo_estrutura_anterior = TipoEstruturaPeriodo_decrescente;
					else
						tipo_estrutura_anterior = TipoEstruturaPeriodo_flat;
				}

				lista_estrutura_copia.at(indice_estrutura_anterior).tipoEstruturaPeriodo = tipo_estrutura_anterior;

			} // else if ((tipo_estrutura_anterior != TipoEstruturaPeriodo_Nenhum) && (tipo_estrutura_posterior == TipoEstruturaPeriodo_Nenhum)) {

			// Iterador novo no meio da enupla
			else if ((tipo_estrutura_anterior != TipoEstruturaPeriodo_Nenhum) && (tipo_estrutura_posterior != TipoEstruturaPeriodo_Nenhum)) {

				// Remove estruturas dos periodos que sairao
				const int nro_estruturas_a_remover = indice_estrutura_posterior - indice_estrutura_anterior - 1;
				for (int i = 0; i < nro_estruturas_a_remover; i++) {
					lista_estrutura_copia.erase(lista_estrutura_copia.begin() + indice_estrutura_anterior + 1);
					indice_estrutura_posterior--;
				}

				// Realiza adequacoes na estrutura anterior
				if (lista_estrutura_copia.at(indice_estrutura_anterior).tipo_iterador_inicial > tipo_periodo_anterior) {
					tipo_estrutura_anterior = TipoEstruturaPeriodo_crescente;
					if (tipo_periodo_anterior >= tipo_periodo_iterador)
						status_inclusao_iterador = 0;
				}
				else if (lista_estrutura_copia.at(indice_estrutura_anterior).tipo_iterador_inicial < tipo_periodo_anterior) {
					tipo_estrutura_anterior = TipoEstruturaPeriodo_decrescente;
					if (tipo_periodo_anterior <= tipo_periodo_iterador)
						status_inclusao_iterador = 0;
				}
				else if (lista_estrutura_copia.at(indice_estrutura_anterior).tipo_iterador_inicial == tipo_periodo_anterior) {
					status_inclusao_iterador = 0;
					if (tipo_periodo_anterior >= tipo_periodo_iterador)
						tipo_estrutura_anterior = TipoEstruturaPeriodo_crescente;
					else if (tipo_periodo_anterior <= tipo_periodo_iterador)
						tipo_estrutura_anterior = TipoEstruturaPeriodo_decrescente;
					else
						tipo_estrutura_anterior = TipoEstruturaPeriodo_flat;
				}

				lista_estrutura_copia.at(indice_estrutura_anterior).tipoEstruturaPeriodo = tipo_estrutura_anterior;

				// Realiza adequacoes na estrutura posterior
				if (tipo_periodo_posterior > lista_estrutura_copia.at(indice_estrutura_posterior).tipo_iterador_final) {
					tipo_estrutura_posterior = TipoEstruturaPeriodo_crescente;
					if ((tipo_periodo_iterador >= tipo_periodo_posterior) && (status_inclusao_iterador == -1))
						status_inclusao_iterador = 1;
				}
				else if (tipo_periodo_posterior < lista_estrutura_copia.at(indice_estrutura_posterior).tipo_iterador_final) {
					tipo_estrutura_posterior = TipoEstruturaPeriodo_decrescente;
					if ((tipo_periodo_iterador <= tipo_periodo_posterior) && (status_inclusao_iterador == -1))
						status_inclusao_iterador = 1;
				}
				else if (tipo_periodo_posterior == lista_estrutura_copia.at(indice_estrutura_posterior).tipo_iterador_final) {
					tipo_estrutura_posterior = TipoEstruturaPeriodo_flat;
					if (status_inclusao_iterador == -1) {
						status_inclusao_iterador = 1;
						if (tipo_periodo_iterador >= tipo_periodo_posterior)
							tipo_estrutura_posterior = TipoEstruturaPeriodo_crescente;
						else if (tipo_periodo_iterador <= tipo_periodo_posterior)
							tipo_estrutura_posterior = TipoEstruturaPeriodo_decrescente;
					}
				}

				lista_estrutura_copia.at(indice_estrutura_posterior).tipoEstruturaPeriodo = tipo_estrutura_posterior;

			} // else if ((tipo_estrutura_anterior != TipoEstruturaPeriodo_Nenhum) && (tipo_estrutura_posterior != TipoEstruturaPeriodo_Nenhum)) {


			//
			// Criacao estrutura intermediaria
			//
			if (status_inclusao_iterador == -1) {

				const int indice_nova_estrutura = indice_estrutura_anterior + 1;

				if (true) {
					EstruturaPeriodo estruturaPeriodo;
					estruturaPeriodo.tipo_iterador_inicial = TipoPeriodo_Nenhum;
					estruturaPeriodo.tipo_iterador_final = TipoPeriodo_Nenhum;
					estruturaPeriodo.iteradores_iniciais = std::vector<std::vector<Periodo>>(TipoPeriodo_Excedente, std::vector<Periodo>());
					estruturaPeriodo.iteradores_finais = std::vector<std::vector<Periodo>>(TipoPeriodo_Excedente, std::vector<Periodo>());
					estruturaPeriodo.smartEnupla = std::vector<std::vector<TipoValor>>(TipoPeriodo_Excedente, std::vector<TipoValor>());
					for (TipoPeriodo tipo = TipoPeriodo(1); tipo < TipoPeriodo_Excedente; tipo++) {
						estruturaPeriodo.iteradores_iniciais.at(tipo).reserve(1);
						estruturaPeriodo.iteradores_finais.at(tipo).reserve(1);
						estruturaPeriodo.smartEnupla.at(tipo).reserve(1);
					}

					estruturaPeriodo.tipoEstruturaPeriodo = TipoEstruturaPeriodo_flat;
					lista_estrutura_copia.insert(lista_estrutura_copia.begin() + indice_nova_estrutura, estruturaPeriodo);
				}

				if (indice_estrutura_posterior > -1)
					indice_estrutura_posterior++;

				lista_estrutura_copia.at(indice_nova_estrutura).tipoEstruturaPeriodo = TipoEstruturaPeriodo_flat;

				lista_estrutura_copia.at(indice_nova_estrutura).tipo_iterador_inicial = tipo_periodo_iterador;
				lista_estrutura_copia.at(indice_nova_estrutura).tipo_iterador_final   = tipo_periodo_iterador;

				lista_estrutura_copia.at(indice_nova_estrutura).iteradores_iniciais.at(tipo_periodo_iterador).push_back(a_iterador);
				lista_estrutura_copia.at(indice_nova_estrutura).iteradores_finais.at(tipo_periodo_iterador).push_back(a_iterador);
				lista_estrutura_copia.at(indice_nova_estrutura).smartEnupla.at(tipo_periodo_iterador).push_back(a_valor);

			} // if (status_inclusao_iterador == -1) {

			//
			// Adequacao estrutura anterior
			//
			if (tipo_estrutura_anterior != TipoEstruturaPeriodo_Nenhum) {

				TipoPeriodo tipo_periodo_final_estrutura_anterior = lista_estrutura_copia.at(indice_estrutura_anterior).tipo_iterador_final;
				Periodo periodo_final_estrutura_anterior = lista_estrutura_copia.at(indice_estrutura_anterior).iteradores_finais.at(tipo_periodo_final_estrutura_anterior).at(0);

				while (periodo_final_estrutura_anterior > periodo_anterior) {

					Periodo novo_periodo_final_estrutura_anterior = periodo_final_estrutura_anterior;
					decrementarIterador(novo_periodo_final_estrutura_anterior);

					TipoPeriodo novo_tipo_periodo_final_estrutura_anterior = novo_periodo_final_estrutura_anterior.getTipoPeriodo();

					if (novo_tipo_periodo_final_estrutura_anterior == tipo_periodo_final_estrutura_anterior) {
						lista_estrutura_copia.at(indice_estrutura_anterior).iteradores_finais.at(tipo_periodo_final_estrutura_anterior).at(0) = novo_periodo_final_estrutura_anterior;
						lista_estrutura_copia.at(indice_estrutura_anterior).smartEnupla.at(tipo_periodo_final_estrutura_anterior).pop_back();
					}
					else{
						lista_estrutura_copia.at(indice_estrutura_anterior).iteradores_iniciais.at(tipo_periodo_final_estrutura_anterior) = std::vector<Periodo>();
						lista_estrutura_copia.at(indice_estrutura_anterior).iteradores_finais.at(tipo_periodo_final_estrutura_anterior) = std::vector<Periodo>();
						lista_estrutura_copia.at(indice_estrutura_anterior).smartEnupla.at(tipo_periodo_final_estrutura_anterior) = std::vector<TipoValor>();
					}

					periodo_final_estrutura_anterior = novo_periodo_final_estrutura_anterior;
					tipo_periodo_final_estrutura_anterior = novo_tipo_periodo_final_estrutura_anterior;

				} // while (periodo_final_estrutura_anterior > periodo_anterior) {

				lista_estrutura_copia.at(indice_estrutura_anterior).tipo_iterador_final = tipo_periodo_final_estrutura_anterior;

				if (status_inclusao_iterador == 0) {

					if (tipo_periodo_final_estrutura_anterior == tipo_periodo_iterador) {
						lista_estrutura_copia.at(indice_estrutura_anterior).iteradores_finais.at(tipo_periodo_final_estrutura_anterior).at(0) = a_iterador;
						lista_estrutura_copia.at(indice_estrutura_anterior).smartEnupla.at(tipo_periodo_final_estrutura_anterior).push_back(a_valor);
					}

					else {
						lista_estrutura_copia.at(indice_estrutura_anterior).iteradores_iniciais.at(tipo_periodo_iterador) = std::vector<Periodo>(1, a_iterador);
						lista_estrutura_copia.at(indice_estrutura_anterior).iteradores_finais.at(tipo_periodo_iterador) = std::vector<Periodo>(1, a_iterador);
						lista_estrutura_copia.at(indice_estrutura_anterior).smartEnupla.at(tipo_periodo_iterador) = std::vector<TipoValor>(1, a_valor);
					}

					lista_estrutura_copia.at(indice_estrutura_anterior).tipo_iterador_final = tipo_periodo_iterador;

				} // if (status_inclusao_iterador == 0) {
			} // if (tipo_estrutura_anterior != TipoEstruturaPeriodo_Nenhum) {
			
			//
			// Adequacao estrutura posterior
			//
			if (tipo_estrutura_posterior != TipoEstruturaPeriodo_Nenhum) {

				TipoPeriodo tipo_periodo_inicial_estrutura_posterior = lista_estrutura_copia.at(indice_estrutura_posterior).tipo_iterador_inicial;
				Periodo periodo_inicial_estrutura_posterior = lista_estrutura_copia.at(indice_estrutura_posterior).iteradores_iniciais.at(tipo_periodo_inicial_estrutura_posterior).at(0);

				while (periodo_inicial_estrutura_posterior < periodo_posterior) {

					Periodo novo_periodo_inicial_estrutura_posterior = periodo_inicial_estrutura_posterior;
					incrementarIterador(novo_periodo_inicial_estrutura_posterior);

					TipoPeriodo novo_tipo_periodo_inicial_estrutura_posterior = novo_periodo_inicial_estrutura_posterior.getTipoPeriodo();

					if (novo_tipo_periodo_inicial_estrutura_posterior == tipo_periodo_inicial_estrutura_posterior) {
						lista_estrutura_copia.at(indice_estrutura_posterior).iteradores_iniciais.at(tipo_periodo_inicial_estrutura_posterior).at(0) = novo_periodo_inicial_estrutura_posterior;
						lista_estrutura_copia.at(indice_estrutura_posterior).smartEnupla.at(tipo_periodo_inicial_estrutura_posterior).erase(lista_estrutura_copia.at(indice_estrutura_posterior).smartEnupla.at(tipo_periodo_inicial_estrutura_posterior).begin());
					}
					else {
						lista_estrutura_copia.at(indice_estrutura_posterior).iteradores_iniciais.at(tipo_periodo_inicial_estrutura_posterior) = std::vector<Periodo>();
						lista_estrutura_copia.at(indice_estrutura_posterior).iteradores_finais.at(tipo_periodo_inicial_estrutura_posterior) = std::vector<Periodo>();
						lista_estrutura_copia.at(indice_estrutura_posterior).smartEnupla.at(tipo_periodo_inicial_estrutura_posterior) = std::vector<TipoValor>();
					}
			
					periodo_inicial_estrutura_posterior = novo_periodo_inicial_estrutura_posterior;
					tipo_periodo_inicial_estrutura_posterior = novo_tipo_periodo_inicial_estrutura_posterior;

				} // while (periodo_inicial_estrutura_posterior > periodo_posterior) {

				lista_estrutura_copia.at(indice_estrutura_posterior).tipo_iterador_inicial = tipo_periodo_inicial_estrutura_posterior;

				if (status_inclusao_iterador == 1) {

					if (tipo_periodo_inicial_estrutura_posterior == tipo_periodo_iterador) {
						lista_estrutura_copia.at(indice_estrutura_posterior).iteradores_iniciais.at(tipo_periodo_inicial_estrutura_posterior).at(0) = a_iterador;
						lista_estrutura_copia.at(indice_estrutura_posterior).smartEnupla.at(tipo_periodo_inicial_estrutura_posterior).push_back(a_valor);
					}

					else {
						lista_estrutura_copia.at(indice_estrutura_posterior).iteradores_iniciais.at(tipo_periodo_iterador) = std::vector<Periodo>(1, a_iterador);
						lista_estrutura_copia.at(indice_estrutura_posterior).iteradores_finais.at(tipo_periodo_iterador) = std::vector<Periodo>(1, a_iterador);
						lista_estrutura_copia.at(indice_estrutura_posterior).smartEnupla.at(tipo_periodo_iterador) = std::vector<TipoValor>(1, a_valor);
					}

					lista_estrutura_copia.at(indice_estrutura_posterior).tipo_iterador_inicial = tipo_periodo_iterador;

				} // if (status_inclusao_iterador == 1) {
			} // if (tipo_estrutura_posterior != TipoEstruturaPeriodo_Nenhum) {

			lista_estrutura = lista_estrutura_copia;
			numero_elementos = numero_elementos - int(iteradores.size()) + 1;

		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::alterarValorSeAlterarIterador(" + getFullString(a_iterador) + ",a_tipo_valor): \n" + std::string(erro.what())); }
	}


	void setElemento(Periodo a_iterador, TipoValor a_valor) {
		try { 
			
			const int pos = getIndiceEstrutura(a_iterador);
			lista_estrutura.at(pos).smartEnupla.at(a_iterador.getTipoPeriodo()).at(getIndice(a_iterador, pos)) = a_valor;

		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::setElemento(" + getString(a_iterador) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }
	}; // void setElemento(Periodo a_iterador, TipoValor a_valor) {

	void setElementoFromStr(Periodo a_iterador, std::string a_valor) {
		try {

			const int pos = getIndiceEstrutura(a_iterador);
			lista_estrutura.at(pos).smartEnupla.at(a_iterador.getTipoPeriodo()).at(getIndice(a_iterador, pos)) = getFromString(TipoValor(), a_valor);

		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::setElemento(" + getString(a_iterador) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iterador, TipoValor a_valor) {

	TipoValor getTipoElemento()const {
		try {
			return TipoValor();
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getTipoElemento(): \n" + std::string(erro.what())); }
	}; // TipoValor getTipoElemento()const {

	TipoValor getElemento(Periodo a_iterador)const {
		try { 
			
			const int pos = getIndiceEstrutura(a_iterador);
			return lista_estrutura.at(pos).smartEnupla.at(a_iterador.getTipoPeriodo()).at(getIndice(a_iterador, pos));
		
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getElemento(" + getString(a_iterador) + "): \n" + std::string(erro.what())); }
	}; // TipoValor getElemento(Periodo a_iterador)const {

	TipoValor& at(Periodo a_iterador) {
		try { 
			
			const int pos = getIndiceEstrutura(a_iterador);
			return lista_estrutura.at(pos).smartEnupla.at(a_iterador.getTipoPeriodo()).at(getIndice(a_iterador, pos));
		
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::at(" + getString(a_iterador) + "): \n" + std::string(erro.what())); }
	}; // TipoValor getElemento(Periodo a_iterador)const {

	TipoValor at(Periodo a_iterador) const {
		try { 
			
			const int pos = getIndiceEstrutura(a_iterador);
			return lista_estrutura.at(pos).smartEnupla.at(a_iterador.getTipoPeriodo()).at(getIndice(a_iterador, pos));
		
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::at(" + getString(a_iterador) + "): \n" + std::string(erro.what())); }
	}; // TipoValor getElemento(Periodo a_iterador)const {


	void random_shuffle() {
		for (int estrutura = 0; estrutura < int(lista_estrutura.size()); estrutura++) {
			for (TipoPeriodo tipo = TipoPeriodo(1); tipo < TipoPeriodo_Excedente; tipo++) {
				if (lista_estrutura.at(estrutura).smartEnupla.at(tipo).size() >= 0)
					std::random_shuffle(lista_estrutura.at(estrutura).smartEnupla.at(tipo).begin(), lista_estrutura.at(estrutura).smartEnupla.at(tipo).end());
			} // for (TipoPeriodo tipo = TipoPeriodo_Nenhum; tipo < TipoPeriodo_Excedente; tipo++) {
		}//for (int estrutura = 0; estrutura < int(lista_estrutura.size()); estrutura++) {
	}; // void random_shuffle() {

	std::vector<TipoValor> getElementos() const {
		std::vector<TipoValor> vetor_retorno;
		for (int estrutura = 0; estrutura < int(lista_estrutura.size()); estrutura++) {
			for (TipoPeriodo tipo = TipoPeriodo(1); tipo < TipoPeriodo_Excedente; tipo++) {
				if (lista_estrutura.at(estrutura).smartEnupla.at(tipo).size() > 0)
					vetor_retorno.insert(vetor_retorno.end(), lista_estrutura.at(estrutura).smartEnupla.at(tipo).begin(), lista_estrutura.at(estrutura).smartEnupla.at(tipo).end());
			} // for (TipoPeriodo tipo = TipoPeriodo_Nenhum; tipo < TipoPeriodo_Excedente; tipo++) {
		}//for (int estrutura = 0; estrutura < int(lista_estrutura.size()); estrutura++) {
		return vetor_retorno;
	};

	SmartEnupla<Periodo, TipoValor> getElementos(const Periodo a_periodo_inicial, const Periodo a_periodo_final) const {
		try {
			const TipoPeriodo tipo = a_periodo_inicial.getTipoPeriodo();
			if ((tipo != a_periodo_final.getTipoPeriodo()) || (a_periodo_inicial > a_periodo_final))
				return SmartEnupla<Periodo, TipoValor>();

			////////////////////////////////

			int pos = -1;

			for (int estrutura = 0; estrutura < int(lista_estrutura.size()); estrutura++) {

				//Determina a estrutura e set o valor
				if (lista_estrutura.at(estrutura).iteradores_iniciais.at(lista_estrutura.at(estrutura).tipo_iterador_inicial).at(0) <= a_periodo_inicial \
					&& lista_estrutura.at(estrutura).iteradores_finais.at(lista_estrutura.at(estrutura).tipo_iterador_final).at(0) >= a_periodo_inicial \
					&& lista_estrutura.at(estrutura).iteradores_iniciais.at(lista_estrutura.at(estrutura).tipo_iterador_inicial).at(0) <= a_periodo_final \
					&& lista_estrutura.at(estrutura).iteradores_finais.at(lista_estrutura.at(estrutura).tipo_iterador_final).at(0) >= a_periodo_final){
					pos = estrutura;
					break;
				}

			}//for (int estrutura = 0; estrutura < int(lista_estrutura.size()); estrutura++) {

			if (pos < 0)
				throw std::invalid_argument("Periodo_inicial_" + getFullString(a_periodo_inicial) + " e periodo_final_" + getFullString(a_periodo_inicial) + "nao pertencem a mesma estrutura");

			/////////////////////////////////

			const int iterador_inicial = getIndice(a_periodo_inicial, pos);
			const int iterador_final = getIndice(a_periodo_final, pos);
			std::vector<TipoValor> vetor_retorno(iterador_final - iterador_inicial + 1, 0.0);
			for (int i = iterador_inicial; i <= iterador_final; i++)
				vetor_retorno.at(i - iterador_inicial) = lista_estrutura.at(pos).smartEnupla.at(tipo).at(i);
			return SmartEnupla<Periodo, TipoValor>(a_periodo_inicial, vetor_retorno);
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getElementos(" + getString(a_periodo_inicial) + "," + getString(a_periodo_final) + "): \n" + std::string(erro.what())); }
	};

	std::vector<TipoValor>& getElementos_referencia() {
		try {
			throw std::invalid_argument("Nao eh possivel retornar referencia em SmartEnupla com iterador Periodo, pois os elementos nao sao sequenciais.");
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getElementos(): \n" + std::string(erro.what())); }
	};

	SmartEnupla<Periodo, TipoValor> getElementos(const Periodo a_periodo_inicial, const int a_numero_elementos) const {
		try {
			if (a_numero_elementos <= 0)
				throw std::invalid_argument("Numero de elementos invalido.");
			return getElementos(a_periodo_inicial - a_numero_elementos + 1, a_periodo_inicial);
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getElementos(" + getString(a_periodo_inicial) + "," + getString(a_numero_elementos) + "): \n" + std::string(erro.what())); }
	};

	std::vector<std::vector<TipoValor>> getElementos(IdDia a_dia) const {
		try {
			if ((a_dia <= IdDia_Nenhum) || (a_dia >= IdDia_Excedente))
				throw std::invalid_argument("Dia Invalido.");

			std::vector<std::vector<TipoValor>> vetor_retorno(TipoPeriodo_Excedente, std::vector<TipoValor>());

			for (int estrutura = 0; estrutura < int(lista_estrutura.size()); estrutura++) {
				for (TipoPeriodo tipo = TipoPeriodo(1); tipo < TipoPeriodo_Excedente; tipo++) {
					if (lista_estrutura.at(estrutura).smartEnupla.at(tipo).size() > 0) {
						for (Periodo periodo = lista_estrutura.at(estrutura).iteradores_iniciais.at(tipo).at(0); periodo <= lista_estrutura.at(estrutura).iteradores_finais.at(tipo).at(0); periodo++) {
							if (periodo.getDia() == a_dia)
								vetor_retorno.at(tipo).push_back(lista_estrutura.at(estrutura).smartEnupla.at(getIndice(periodo, estrutura)));
						} // for (Periodo periodo = lista_estrutura.at(estrutura).iteradores_iniciais.at(tipo).at(0); periodo <= lista_estrutura.at(estrutura).iteradores_finais.at(tipo).at(0); periodo++) {
					} // if (smartEnupla.at(tipo).size() > 0) {
				} // for (TipoPeriodo tipo = TipoPeriodo(1); tipo < TipoPeriodo_Excedente; tipo++) {
			}//for (int estrutura = 0; estrutura < int(lista_estrutura.size()); estrutura++) {
			return vetor_retorno;
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getElementos(" + getString(a_dia) + "): \n" + std::string(erro.what())); }
	}; // std::vector<std::vector<TipoValor>> getElementos(IdDia a_dia) const {

	std::vector<std::vector<TipoValor>> getElementos(IdMes a_mes) const {
		try {
			if ((a_mes <= IdMes_Nenhum) || (a_mes >= IdMes_Excedente))
				throw std::invalid_argument("Mes Invalido.");

			std::vector<std::vector<TipoValor>> vetor_retorno(TipoPeriodo_Excedente, std::vector<TipoValor>());
			for (int estrutura = 0; estrutura < int(lista_estrutura.size()); estrutura++) {
				for (TipoPeriodo tipo = TipoPeriodo(1); tipo < TipoPeriodo_Excedente; tipo++) {
					if (lista_estrutura.at(estrutura).smartEnupla.at(tipo).size() > 0) {
						for (Periodo periodo = lista_estrutura.at(estrutura).iteradores_iniciais.at(tipo).at(0); periodo <= lista_estrutura.at(estrutura).iteradores_finais.at(tipo).at(0); periodo++) {
							if (periodo.getMes() == a_mes)
								vetor_retorno.at(tipo).push_back(lista_estrutura.at(estrutura).smartEnupla.at(tipo).at(getIndice(periodo, estrutura)));
						} // for (Periodo periodo = lista_estrutura.at(estrutura).iteradores_iniciais.at(tipo).at(0); periodo <= lista_estrutura.at(estrutura).iteradores_finais.at(tipo).at(0); periodo++) {
					} // if (smartEnupla.at(tipo).size() > 0) {
				} // for (TipoPeriodo tipo = TipoPeriodo(1); tipo < TipoPeriodo_Excedente; tipo++) {
			}//for (int estrutura = 0; estrutura < int(lista_estrutura.size()); estrutura++) {
			return vetor_retorno;
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getElementos(" + getString(a_mes) + "): \n" + std::string(erro.what())); }
	}; // std::vector<TipoValor> getElementos(int a_mes) {

	std::vector<std::vector<TipoValor>> getElementos(IdAno a_ano) const {
		try {
			if ((a_ano <= IdAno_Nenhum) || (a_ano >= IdAno_Excedente))
				throw std::invalid_argument("Ano Invalido.");

			std::vector<std::vector<TipoValor>> vetor_retorno(TipoPeriodo_Excedente, std::vector<TipoValor>());
			for (int estrutura = 0; estrutura < int(lista_estrutura.size()); estrutura++) {
				for (TipoPeriodo tipo = TipoPeriodo(1); tipo < TipoPeriodo_Excedente; tipo++) {
					if (lista_estrutura.at(estrutura).smartEnupla.at(tipo).size() > 0) {
						for (Periodo periodo = lista_estrutura.at(estrutura).iteradores_iniciais.at(tipo).at(0); periodo <= lista_estrutura.at(estrutura).iteradores_finais.at(tipo).at(0); periodo++) {
							if (periodo.getAno() == a_ano)
								vetor_retorno.at(tipo).push_back(lista_estrutura.at(estrutura).smartEnupla.at(tipo).at(getIndice(periodo, estrutura)));
						} // for (Periodo periodo = lista_estrutura.at(estrutura).iteradores_iniciais.at(tipo).at(0); periodo <= lista_estrutura.at(estrutura).iteradores_finais.at(tipo).at(0); periodo++) {
					} // if (smartEnupla.at(tipo).size() > 0) {
				} // for (TipoPeriodo tipo = TipoPeriodo(1); tipo < TipoPeriodo_Excedente; tipo++) {
			}//for (int estrutura = 0; estrutura < int(lista_estrutura.size()); estrutura++) {
			return vetor_retorno;
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getElementos(" + getString(a_ano) + "): \n" + std::string(erro.what())); }
	}; // SmartEnupla <Periodo, TipoValor> getElementos(IdAno a_ano) {

	std::vector<std::vector<TipoValor>> getElementos(IdEstacao a_estacao) const {
		try {
			if ((a_estacao <= IdEstacao_Nenhum) || (a_estacao >= IdEstacao_Excedente))
				throw std::invalid_argument("Estacao Invalida.");

			std::vector<std::vector<TipoValor>> vetor_retorno(TipoPeriodo_Excedente, std::vector<TipoValor>());
			for (int estrutura = 0; estrutura < int(lista_estrutura.size()); estrutura++) {
				for (TipoPeriodo tipo = TipoPeriodo(1); tipo < TipoPeriodo_Excedente; tipo++) {
					if (lista_estrutura.at(estrutura).smartEnupla.at(tipo).size() > 0) {
						for (Periodo periodo = lista_estrutura.at(estrutura).iteradores_iniciais.at(tipo).at(0); periodo <= lista_estrutura.at(estrutura).iteradores_finais.at(tipo).at(0); periodo++) {
							if (periodo.getEstacao() == a_estacao)
								vetor_retorno.at(tipo).push_back(lista_estrutura.at(estrutura).smartEnupla.at(tipo).at(getIndice(periodo, estrutura)));
						} // for (Periodo periodo = iteradores_iniciais.at(tipo).at(0); periodo <= iteradores_finais.at(tipo).at(0); periodo++) {
					} // if (smartEnupla.at(tipo).size() > 0) {
				} // for (TipoPeriodo tipo = TipoPeriodo(1); tipo < TipoPeriodo_Excedente; tipo++) {
			}//for (int estrutura = 0; estrutura < int(lista_estrutura.size()); estrutura++) {
			return vetor_retorno;
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getElementos(" + getString(a_estacao) + "): \n" + std::string(erro.what())); }
	}; // std::vector<TipoValor> getElementos(int a_estacao) {

	std::string str() { return "SmartEnupla<iterador,valor>"; }

private:

	int getIndiceEstrutura(Periodo a_periodo) const {
		try {
			const int indice = getIndiceEstruturaSeExistir(a_periodo);

			if (indice < 0)
				throw std::invalid_argument("Nao existem estruturas definidas para o periodo " + getFullString(a_periodo));

			return indice;
		}
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getIndiceEstrutura(" + getString(a_periodo) + "): \n" + std::string(erro.what())); }
	}

	int getIndiceEstruturaSeExistir(Periodo a_periodo) const {
		try {

			const TipoPeriodo tipo_periodo_iterador = a_periodo.getTipoPeriodo();

			int pos_min = 0;

			if ((a_periodo.estrutura > 0) && (lista_estrutura.size() >= a_periodo.estrutura)) {

				pos_min = a_periodo.estrutura - 1;
				if (lista_estrutura.at(pos_min).iteradores_iniciais.at(tipo_periodo_iterador).size() > 0) {
					if (lista_estrutura.at(pos_min).iteradores_iniciais.at(tipo_periodo_iterador).at(0) > a_periodo)
						pos_min = 0;
				}

			}// if ((a_periodo.estrutura > 0) && (lista_estrutura.size() > a_periodo.estrutura)) {

			const int pos_max = int(lista_estrutura.size()) - 1;

			for (int pos = pos_min; pos <= pos_max; pos++) {

				if (lista_estrutura.at(pos).iteradores_iniciais.at(tipo_periodo_iterador).size() > 0) {

					if ((lista_estrutura.at(pos).iteradores_iniciais.at(tipo_periodo_iterador).at(0) <= a_periodo) && \
						(a_periodo <= lista_estrutura.at(pos).iteradores_finais.at(tipo_periodo_iterador).at(0))) {
						return pos;
					}

				}

			}//for (int estrutura = 0; estrutura < int(lista_estrutura.size()); estrutura++) {

			return -1;

		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getIndiceEstruturaSeExistir(" + getString(a_periodo) + "): \n" + std::string(erro.what())); }
	}; // int getIndice(Periodo a_iterador, const int a_estrutura) const {

	int getIndice(Periodo a_iterador, const int a_estrutura) const {
		try {
			if (lista_estrutura.at(a_estrutura).iteradores_iniciais.at(a_iterador.getTipoPeriodo()).size() == 0)
				throw std::invalid_argument("Iterador nao utilizado no SmartEnupla.");
			return a_iterador - lista_estrutura.at(a_estrutura).iteradores_iniciais.at(a_iterador.getTipoPeriodo()).at(0);
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getIndice(" + getString(a_iterador) + "," + getString(a_estrutura) + "): \n" + std::string(erro.what())); }
	}; // int getIndice(Periodo a_iterador, const int a_estrutura) const {

	void inicializacao(const int a_alocacao) {
		try {
			if (a_alocacao < 0)
				throw std::invalid_argument("Alocacao deve ser maior que 0.");
			numero_elementos = 0;

			inicializacaoEstruturaPeriodo(-1, a_alocacao);

		}
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::inicializacao(" + getFullString(a_alocacao) + "): \n" + std::string(erro.what())); }
	}; // void inicializacao(const int a_alocacao) {


	void inicializacaoEstruturaPeriodo(const int a_insercao, const int a_alocacao) {
		try {
			if (a_alocacao < 0)
				throw std::invalid_argument("Alocacao deve ser maior que 0.");

			EstruturaPeriodo estruturaPeriodo;

			estruturaPeriodo.tipo_iterador_inicial = TipoPeriodo_Nenhum;
			estruturaPeriodo.tipo_iterador_final = TipoPeriodo_Nenhum;
			estruturaPeriodo.iteradores_iniciais = std::vector<std::vector<Periodo>>(TipoPeriodo_Excedente, std::vector<Periodo>());
			estruturaPeriodo.iteradores_finais = std::vector<std::vector<Periodo>>(TipoPeriodo_Excedente, std::vector<Periodo>());
			estruturaPeriodo.smartEnupla = std::vector<std::vector<TipoValor>>(TipoPeriodo_Excedente, std::vector<TipoValor>());
			for (TipoPeriodo tipo = TipoPeriodo(1); tipo < TipoPeriodo_Excedente; tipo++) {
				estruturaPeriodo.iteradores_iniciais.at(tipo).reserve(a_alocacao);
				estruturaPeriodo.iteradores_finais.at(tipo).reserve(a_alocacao);
				estruturaPeriodo.smartEnupla.at(tipo).reserve(a_alocacao);
			}

			estruturaPeriodo.tipoEstruturaPeriodo = TipoEstruturaPeriodo_flat;

			// Final lista
			if ((a_insercao < 0) || (lista_estrutura.size() == 0))
				lista_estrutura.push_back(estruturaPeriodo);
			// Decorrer lista
			else 
				lista_estrutura.insert(lista_estrutura.begin() + a_insercao, estruturaPeriodo);

		}
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::inicializacaoEstruturaPeriodo(" + getFullString(a_alocacao) + "): \n" + std::string(erro.what())); }
	}; // void inicializacaoEstruturaPeriodo(const int a_alocacao) {

	int numero_elementos;

	struct EstruturaPeriodo {

		TipoPeriodo tipo_iterador_inicial = TipoPeriodo_Nenhum;
		TipoPeriodo tipo_iterador_final = TipoPeriodo_Nenhum;

		TipoEstruturaPeriodo tipoEstruturaPeriodo = TipoEstruturaPeriodo_Nenhum;

		std::vector <std::vector<Periodo>> iteradores_iniciais;
		std::vector <std::vector<Periodo>> iteradores_finais;

		std::vector <std::vector<TipoValor>> smartEnupla;	

	};

	std::vector<EstruturaPeriodo> lista_estrutura;

}; // class SmartEnupla {


#endif