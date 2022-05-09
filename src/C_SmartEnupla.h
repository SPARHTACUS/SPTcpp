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
	SmartEnupla(const TipoIterador a_iteradorInicial, const std::vector<TipoValor> &a_vetorInicial) {
		if (a_iteradorInicial < getFromChar(TipoIterador(), "min"))
			throw std::invalid_argument("O iterador Inicial deve ser maior ou igual a " + getString(getFromChar(TipoIterador(), "min")));
		else if (a_vetorInicial.size() == 0)
			throw std::invalid_argument("O vetor inicial deve possuir algum elemento.");
		iteradorInicial = a_iteradorInicial;
		smartEnupla      = a_vetorInicial;
		iteradorFinal   = TipoIterador(iteradorInicial + int(smartEnupla.size()) - 1);
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
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::addElemento(" +  getString(a_valor) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoValor a_valor) {

	void addElemento(TipoIterador a_iterador, TipoValor a_valor) {
		try {
			if (size() == 0) {
				if (a_iterador < getFromChar(TipoIterador(), "min"))
					throw std::invalid_argument("O iterador Inicial deve ser maior ou igual a " + getString(getFromChar(TipoIterador(), "min")));
				iteradorInicial = a_iterador;
			}
			else if (getIteradorFinal() != a_iterador - 1)
				throw std::invalid_argument("Argumento Iterador - " + getString(a_iterador) + " - nao sequencial ao Iterador final do SmartEnupla - " + getString(getIteradorFinal()) + ".");

			smartEnupla.push_back(a_valor);
			iteradorFinal = a_iterador;
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::addElemento(" + getString(a_iterador) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iterador, TipoValor a_valor) {

	void setElemento(TipoIterador a_iterador, TipoValor a_valor) {
		try {
			smartEnupla.at(getIndice(a_iterador)) = a_valor;
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::setElemento(" + getString(a_iterador) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iterador, TipoValor a_valor) {

	TipoValor getElemento(TipoIterador a_iterador)const {
		try {
			return smartEnupla.at(getIndice(a_iterador));
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getElemento(" + getString(a_iterador) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iterador, TipoValor a_valor) {

	TipoValor at(TipoIterador a_iterador)const {
		try {
			return smartEnupla.at(getIndice(a_iterador));
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::at(" + getFullString(a_iterador) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iterador, TipoValor a_valor) {

	TipoValor& at(TipoIterador a_iterador){
		try {
			return smartEnupla.at(getIndice(a_iterador));
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::at(" + getFullString(a_iterador) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(TipoIterador a_iterador, TipoValor a_valor) {

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

	void incrementarIterador(TipoIterador &a_iterador, const int a_inteiro) const {
		try {
			if (a_inteiro < 0)
				throw std::invalid_argument("Inteiro invalido.");
			for (int i = 1; i <= a_inteiro; i++)
				incrementarIterador(a_iterador);
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::incrementaIterador(" + getString(a_iterador) + "," + getString(a_inteiro) + "): \n" + std::string(erro.what())); }
	} // void incrementarIterador(Periodo &a_periodo, const int a_inteiro) const {

	void incrementarIterador(TipoIterador &a_iterador) const {
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

	void decrementarIterador(TipoIterador &a_iterador) const {
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

	bool isProximoIterador(const TipoIterador a_iterador) {
		try {
			if (TipoIterador(getIteradorFinal() + 1) == a_iterador)
				return true;
			return false;
		} // try{
		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::isProximoIterador(" + getString(a_iterador) + "): \n" + std::string(erro.what())); }
	}

	std::string str() { return "SmartEnupla<iterador,valor>"; }

	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iteradores, const std::string a_string) { return std::vector<std::vector<std::string>>(); };
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iteradores, const char * a_char) { return std::vector<std::vector<std::string>>(); };
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iteradores, const bool a_bool) { return std::vector<std::vector<std::string>>(); };
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iteradores, const double a_double) { return std::vector<std::vector<std::string>>(); };
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iteradores, const int a_int) { return std::vector<std::vector<std::string>>(); };
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iteradores, const std::vector<int> a_vetor) { return std::vector<std::vector<std::string>>(); };
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iteradores, const std::vector<double> a_vetor) { return std::vector<std::vector<std::string>>(); };
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iteradores, const std::vector<std::vector<int>> a_vetor) { return std::vector<std::vector<std::string>>(); };
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iteradores, const std::vector<std::vector<double>> a_vetor) { return std::vector<std::vector<std::string>>(); };

	template<typename Iterador, typename Valor>
	static std::vector<std::vector<std::string>> getDadosAsString(const bool a_incluir_iteradores, const SmartEnupla<Iterador, Valor> &a_enupla){

		try {

			if (a_enupla.size() == 0)
				return std::vector<std::vector<std::string>>();

			const Iterador iteradorInicial = a_enupla.getIteradorInicial();
			const Iterador iteradorFinal = a_enupla.getIteradorFinal();

			std::vector<std::vector<std::string>> matriz_retorno;

			// Ultima camada da SmarEnupla
			if (SmartEnupla<int, int>::getDadosAsString(a_incluir_iteradores, a_enupla.at(a_enupla.getIteradorInicial())).size() == 0) {

				if (a_incluir_iteradores)
					matriz_retorno = std::vector<std::vector<std::string>>(2, std::vector<std::string>(a_enupla.size(), ""));
				else
					matriz_retorno = std::vector<std::vector<std::string>>(1, std::vector<std::string>(a_enupla.size(), ""));

				int i = 0;
				for (Iterador iterador = iteradorInicial; iterador <= iteradorFinal; a_enupla.incrementarIterador(iterador)) {

					if (a_incluir_iteradores) {
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

				for (Iterador iterador = iteradorInicial; iterador <= iteradorFinal; a_enupla.incrementarIterador(iterador)) {

					const std::vector<std::vector<std::string>> matriz_retornada = SmartEnupla<int, int>::getDadosAsString(a_incluir_iteradores, a_enupla.at(iterador));

					if (matriz_retornada.size() > 0) {

						for (int l = 0; l < matriz_retornada.size(); l++) {
							if (matriz_retornada.at(l).size() > 0) {

								if (a_incluir_iteradores) {						

									if (matriz_retorno.size() == 0) {
										matriz_retorno.push_back(std::vector<std::string>(matriz_retornada.at(l).size() + 1, ""));
										matriz_retorno.at(0).at(0) = getStringTipo(iterador);
										for (int c = 0; c < matriz_retornada.at(l).size(); c++)
											matriz_retorno.at(matriz_retorno.size() - 1).at(c + 1) = matriz_retornada.at(l).at(c);
									}

									else if ((matriz_retorno.size() > 0) && (l == 0)) {
										if (matriz_retornada.at(0).size() > matriz_retorno.at(0).size() - 1) {
											matriz_retorno.at(0)  = std::vector<std::string>(matriz_retornada.at(0).size() + 1, "");
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

								} // if (a_incluir_iteradores) {

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

		catch (const std::exception& erro) { throw std::invalid_argument("SmartEnupla::getDadosAsString(" + getString(a_incluir_iteradores) + ",a_enupla): \n" + std::string(erro.what())); }

	} // std::vector<std::vector<std::string>> SmartEnupla::getDadosAsString(const bool a_incluir_cabecalho){

private:

	int getIndice(TipoIterador a_iterador) const{
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

			const Periodo periodo_externo_seguinte = a_periodo_externo + 1;

			std::vector<Periodo> iteradores;

			bool is_sobreposicao = false;

			for (Periodo periodo_interno = getIteradorInicial(); periodo_interno <= getIteradorFinal(); incrementarIterador(periodo_interno)) {

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

	void incrementarIterador(Periodo& a_periodo) const {
		try {
			if (size() == 0)
				throw std::invalid_argument("Nao ha elementos na SmartEnupla.");

			const TipoPeriodo tipo = a_periodo.getTipoPeriodo();

			if (true) {

				const Periodo periodo_final = getIteradorFinal();

				if ((periodo_final.getTipoPeriodo() == tipo) && (periodo_final <= a_periodo)) {
					a_periodo++;
					return;
				}

			} // if (true) {

			//////////////////////////////////////////////////
			//Identifica a estrutura a qual pertence o periodo
			//////////////////////////////////////////////////

			int indiceEstrutura = getIndiceEstrutura(a_periodo);

			//////////////////////////////////////////////////

			if (lista_estrutura.at(indiceEstrutura).iteradores_finais.at(tipo).size() == 0)
				throw std::invalid_argument("Iterador invalido.");
			else if ((a_periodo >= lista_estrutura.at(indiceEstrutura).iteradores_iniciais.at(tipo).at(0)) && (a_periodo < lista_estrutura.at(indiceEstrutura).iteradores_finais.at(tipo).at(0))) {
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

	void decrementarIterador(Periodo& a_periodo) const {
		try {
			if (size() == 0)
				throw std::invalid_argument("Nao ha elementos na SmartEnupla.");

			//////////////////////////////////////////////////
			//Identifica a estrutura a qual pertence o periodo
			//////////////////////////////////////////////////

			int indiceEstrutura = getIndiceEstrutura(a_periodo);

			//////////////////////////////////////////////////

			const TipoPeriodo tipo = a_periodo.getTipoPeriodo();
			if (lista_estrutura.at(indiceEstrutura).iteradores_finais.at(tipo).size() == 0)
				throw std::invalid_argument("Iterador invalido.");
			else if ((a_periodo > lista_estrutura.at(indiceEstrutura).iteradores_iniciais.at(tipo).at(0)) && (a_periodo <= lista_estrutura.at(indiceEstrutura).iteradores_finais.at(tipo).at(0))) {
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

	bool isProximoIterador(const Periodo a_iterador) {
		try {
			if (Periodo(a_iterador.getTipoPeriodo(), getIteradorFinal() + 1) == a_iterador)
				return true;
			return false;
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::isProximoIterador(" + getString(a_iterador) + "): \n" + std::string(erro.what())); }
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

			else if (Periodo(a_iterador.getTipoPeriodo(), getIteradorFinal() + 1) != a_iterador)
				throw std::invalid_argument("Argumento Iterador - " + getString(a_iterador) + " - nao sequencial ao Iterador final do SmartEnupla - " + getString(getIteradorFinal()) + ".");

			else {

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
						inicializacaoEstruturaPeriodo();//Cria uma nova estrutura
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
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::addElemento(" + getString(a_iterador) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }
	}; // void addElemento(Periodo a_iterador, TipoValor a_valor) {

	void setElemento(Periodo a_iterador, TipoValor a_valor) {
		try { 
			
			const int pos = getIndiceEstrutura(a_iterador);
			lista_estrutura.at(pos).smartEnupla.at(a_iterador.getTipoPeriodo()).at(getIndice(a_iterador, pos)) = a_valor;

		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::setElemento(" + getString(a_iterador) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }
	}; // void setElemento(Periodo a_iterador, TipoValor a_valor) {

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

			bool procurar_em_todas_as_estruturas = false;

			if (a_periodo.estrutura > 0) {

				if (lista_estrutura.size() > a_periodo.estrutura) {
					if (lista_estrutura.at(a_periodo.estrutura - 1).iteradores_iniciais.at(lista_estrutura.at(a_periodo.estrutura - 1).tipo_iterador_inicial).at(0) <= a_periodo \
						&& lista_estrutura.at(a_periodo.estrutura - 1).iteradores_finais.at(lista_estrutura.at(a_periodo.estrutura - 1).tipo_iterador_final).at(0) >= a_periodo) {
						return a_periodo.estrutura - 1;
					}
					else
						procurar_em_todas_as_estruturas = true;
				}

				else
					procurar_em_todas_as_estruturas = true;

			}//if (a_periodo.estrutura > 0) {

			if (a_periodo.estrutura == 0 || procurar_em_todas_as_estruturas) {//Procura em todas as estruturas até encontrar

				const int numero_estruturas = int(lista_estrutura.size());
				
				for (int pos = 0; pos < numero_estruturas; pos++) {

					if (lista_estrutura.at(pos).iteradores_iniciais.at(lista_estrutura.at(pos).tipo_iterador_inicial).at(0) <= a_periodo \
						&& lista_estrutura.at(pos).iteradores_finais.at(lista_estrutura.at(pos).tipo_iterador_final).at(0) >= a_periodo) {
						return pos;							
					}
				}//for (int estrutura = 0; estrutura < int(lista_estrutura.size()); estrutura++) {
				
				throw std::invalid_argument("Nao existem estruturas definidas para o periodo " + getFullString(a_periodo));

			}//if (a_periodo.estrutura == 0 || procurar_em_todas_as_estruturas) {

			return -1;
			
		} // try{
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::getIndiceEstrutura(" + getString(a_periodo) + "): \n" + std::string(erro.what())); }
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

			inicializacaoEstruturaPeriodo(a_alocacao);

		}
		catch (const std::exception & erro) { throw std::invalid_argument("SmartEnupla::inicializacao(" + getFullString(a_alocacao) + "): \n" + std::string(erro.what())); }
	}; // void inicializacao(const int a_alocacao) {

	void inicializacaoEstruturaPeriodo() { inicializacaoEstruturaPeriodo(0); };

	void inicializacaoEstruturaPeriodo(const int a_alocacao) {
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

			lista_estrutura.push_back(estruturaPeriodo);
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