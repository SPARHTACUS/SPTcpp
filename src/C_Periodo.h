#ifndef C_PERIODO_H
#define C_PERIODO_H

//#define _GLIBCXX_USE_CXX11_ABI 0

#include <vector>
#include <string>

#include "C_SmartEnum.h"


#define TIPO_PERIODO(m)       \
	m(TipoPeriodo, anual)     \
	m(TipoPeriodo, mensal)    \
    m(TipoPeriodo, 60dias)    \
    m(TipoPeriodo, 59dias)    \
    m(TipoPeriodo, 58dias)    \
    m(TipoPeriodo, 57dias)    \
    m(TipoPeriodo, 56dias)    \
    m(TipoPeriodo, 55dias)    \
    m(TipoPeriodo, 54dias)    \
    m(TipoPeriodo, 53dias)    \
    m(TipoPeriodo, 52dias)    \
    m(TipoPeriodo, 51dias)    \
    m(TipoPeriodo, 50dias)    \
    m(TipoPeriodo, 49dias)    \
    m(TipoPeriodo, 48dias)    \
    m(TipoPeriodo, 47dias)    \
    m(TipoPeriodo, 46dias)    \
    m(TipoPeriodo, 45dias)    \
    m(TipoPeriodo, 44dias)    \
    m(TipoPeriodo, 43dias)    \
    m(TipoPeriodo, 42dias)    \
    m(TipoPeriodo, 41dias)    \
    m(TipoPeriodo, 40dias)    \
    m(TipoPeriodo, 39dias)    \
    m(TipoPeriodo, 38dias)    \
    m(TipoPeriodo, 37dias)    \
    m(TipoPeriodo, 36dias)    \
    m(TipoPeriodo, 35dias)    \
    m(TipoPeriodo, 34dias)    \
    m(TipoPeriodo, 33dias)    \
    m(TipoPeriodo, 32dias)    \
    m(TipoPeriodo, 31dias)    \
    m(TipoPeriodo, 30dias)    \
    m(TipoPeriodo, 29dias)    \
    m(TipoPeriodo, 28dias)    \
    m(TipoPeriodo, 27dias)    \
    m(TipoPeriodo, 26dias)    \
    m(TipoPeriodo, 25dias)    \
    m(TipoPeriodo, 24dias)    \
    m(TipoPeriodo, 23dias)    \
    m(TipoPeriodo, 22dias)    \
    m(TipoPeriodo, 21dias)    \
    m(TipoPeriodo, 20dias)    \
    m(TipoPeriodo, 19dias)    \
    m(TipoPeriodo, 18dias)    \
    m(TipoPeriodo, 17dias)    \
    m(TipoPeriodo, 16dias)    \
    m(TipoPeriodo, 15dias)    \
    m(TipoPeriodo, 14dias)    \
    m(TipoPeriodo, 13dias)    \
    m(TipoPeriodo, 12dias)    \
    m(TipoPeriodo, 11dias)    \
    m(TipoPeriodo, 10dias)    \
    m(TipoPeriodo, 9dias)     \
    m(TipoPeriodo, 8dias)     \
	m(TipoPeriodo, semanal)   \
	m(TipoPeriodo, 6dias)     \
	m(TipoPeriodo, 5dias)     \
	m(TipoPeriodo, 4dias)     \
	m(TipoPeriodo, 3dias)     \
	m(TipoPeriodo, 2dias)     \
	m(TipoPeriodo, diario)    \
	m(TipoPeriodo, 23horas)   \
	m(TipoPeriodo, 22horas)   \
	m(TipoPeriodo, 21horas)   \
	m(TipoPeriodo, 20horas)   \
	m(TipoPeriodo, 19horas)   \
	m(TipoPeriodo, 18horas)   \
	m(TipoPeriodo, 17horas)   \
	m(TipoPeriodo, 16horas)   \
	m(TipoPeriodo, 15horas)   \
	m(TipoPeriodo, 14horas)   \
	m(TipoPeriodo, 13horas)   \
	m(TipoPeriodo, 12horas)   \
	m(TipoPeriodo, 11horas)   \
	m(TipoPeriodo, 10horas)   \
	m(TipoPeriodo, 9horas)   \
	m(TipoPeriodo, 8horas)   \
	m(TipoPeriodo, 7horas)   \
	m(TipoPeriodo, 6horas)   \
	m(TipoPeriodo, 5horas)   \
	m(TipoPeriodo, 4horas)   \
	m(TipoPeriodo, 3horas)   \
	m(TipoPeriodo, 2horas)   \
	m(TipoPeriodo, horario)   \
	m(TipoPeriodo, meia_hora) \
	m(TipoPeriodo, minuto) 
DEFINE_SMART_ENUM(TipoPeriodo, TIPO_PERIODO)


class Periodo {

private:

	TipoPeriodo tipoPeriodo;

	IdAno ano;
	IdMes mes;
	IdDia dia;
	IdHor hora;
	IdMin minuto;

	void inicializaAtributos();


	void validaAno   (const IdAno a_ano);
	void validaMes   (const IdMes a_mes);
	void validaDia   (const IdDia a_hor);
	void validaHora  (const IdHor a_hora);
	void validaMinuto(const IdMin a_minuto);

	void setPeriodo(const TipoPeriodo a_tipoPeriodo, const std::string a_periodo);
	void setPeriodo(const TipoPeriodo a_tipoPeriodo, const IdAno a_ano);
	void setPeriodo(const TipoPeriodo a_tipoPeriodo, const IdMes a_mes, const IdAno a_ano);
	void setPeriodo(const TipoPeriodo a_tipoPeriodo, const IdDia a_hor, const IdMes a_mes, const IdAno a_ano);
	void setPeriodo(const TipoPeriodo a_tipoPeriodo, const IdDia a_hor, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora);
	void setPeriodo(const TipoPeriodo a_tipoPeriodo, const IdDia a_hor, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora, const IdMin a_minuto);

public:

	Periodo();

	Periodo(const TipoPeriodo a_tipoPeriodo, const Periodo &a_periodo);

	Periodo(const std::string a_periodo);
	Periodo(const TipoPeriodo a_tipoPeriodo, const std::string a_periodo);

	Periodo(const int a_ano);
	Periodo(const TipoPeriodo a_tipoPeriodo, const int a_ano);

	Periodo(const IdAno a_ano);
	Periodo(const TipoPeriodo a_tipoPeriodo, const IdAno a_ano);

	Periodo(const int a_mes, const int a_ano);
	Periodo(const TipoPeriodo a_tipoPeriodo, const int a_mes, const int a_ano);

	Periodo(const IdMes a_mes, const IdAno a_ano);
	Periodo(const TipoPeriodo a_tipoPeriodo, const IdMes a_mes, const IdAno a_ano);

	Periodo(const int a_dia, const int a_mes, const int a_ano);
	Periodo(const TipoPeriodo a_tipoPeriodo, const int a_dia, const int a_mes, const int a_ano);

	Periodo(const IdDia a_dia, const IdMes a_mes, const IdAno a_ano);
	Periodo(const TipoPeriodo a_tipoPeriodo, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano);

	Periodo(const int a_dia, const int a_mes, const int a_ano, const int a_hora);
	Periodo(const TipoPeriodo a_tipoPeriodo, const int a_dia, const int a_mes, const int a_ano, const int a_hora);
	
	Periodo(const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora);
	Periodo(const TipoPeriodo a_tipoPeriodo, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora);

	Periodo(const int a_dia, const int a_mes, const int a_ano, const int a_hora, const int a_minuto);
	Periodo(const TipoPeriodo a_tipoPeriodo, const int a_dia, const int a_mes, const int a_ano, const int a_hora, const int a_minuto);

	Periodo(const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora, const IdMin a_minuto);
	Periodo(const TipoPeriodo a_tipoPeriodo, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora, const IdMin a_minuto);

	double sobreposicao(const Periodo a_periodo_overlap)const;
	
	double sobreposicao(const Periodo a_periodo_overlap_1, const Periodo a_periodo_overlap_2)const;

	double atraso(const Periodo a_periodo)const;

	bool sobreposicaoExcedente(const Periodo a_periodo_overlap) const;

	IdAno getAno()    const { return ano;    };
	IdMes getMes()    const { return mes;    };
	IdDia getDia()    const { return dia;    };
	IdHor getHora()   const { return hora;   };
	IdMin getMinuto() const { return minuto; };
	IdDiaSemana getDiaSemana() const;

	IdDia getMaiorDiaDoMes() const;
	IdDia getMaiorDiaDoMes(IdMes a_idMes) const;

	int getMeses()   const;
	int getDias()    const;
	int getHoras()   const;
	int getMinutos() const;
	int getSegundos() const;

	int estrutura = 0; //Atributo que mapea em qual estrutura dentro de da SmartEnupla <Periodo> se encontra o periodo. Valor = 0 significa que devem ser identificada a estrutura

	TipoPeriodo getTipoPeriodo() const;

	bool anoBissexto() const;

	std::string str() const;

	bool isValido() const ;

	// Operadores

	friend Periodo operator+(const Periodo &a_periodo, const int a_iterador);

	friend Periodo operator-(const Periodo &a_periodo, const int a_iterador);

	friend int operator-(const Periodo &a_periodo1, const Periodo &a_periodo2);

	friend void operator++(Periodo &a_periodo, int);
	friend void operator--(Periodo &a_periodo, int);

	friend bool operator==(const Periodo &a_periodo1, const Periodo &a_periodo2);

	friend bool operator!=(const Periodo &a_periodo1, const Periodo &a_periodo2);

	friend bool operator< (const Periodo &a_periodo1, const Periodo &a_periodo2);
	friend bool operator<=(const Periodo &a_periodo1, const Periodo &a_periodo2);

	friend bool operator> (const Periodo &a_periodo1, const Periodo &a_periodo2);
	friend bool operator>=(const Periodo &a_periodo1, const Periodo &a_periodo2);

	// Métodos Estáticos

	static bool anoBissexto(const int   a_ano);
	static bool anoBissexto(const IdAno a_ano);

	static void iteraAno   (const Periodo &a_periodo, const int a_iterador, IdAno &a_anoIter, IdMes &a_mesIter, IdDia &a_diaIter, IdHor &a_horaIter, IdMin &a_minutoIter);
	static void iteraMes   (const Periodo &a_periodo, const int a_iterador, IdAno &a_anoIter, IdMes &a_mesIter, IdDia &a_diaIter, IdHor &a_horaIter, IdMin &a_minutoIter);
	static void iteraDia   (const Periodo &a_periodo, const int a_iterador, IdAno &a_anoIter, IdMes &a_mesIter, IdDia &a_diaIter, IdHor &a_horaIter, IdMin &a_minutoIter);
	static void iteraHora  (const Periodo &a_periodo, const int a_iterador, IdAno &a_anoIter, IdMes &a_mesIter, IdDia &a_diaIter, IdHor &a_horaIter, IdMin &a_minutoIter);
	static void iteraMinuto(const Periodo &a_periodo, const int a_iterador, IdAno &a_anoIter, IdMes &a_mesIter, IdDia &a_diaIter, IdHor &a_horaIter, IdMin &a_minutoIter);
	
	static Periodo getPeriodoInicial();
	static Periodo getPeriodoInicial(TipoPeriodo a_tipoPeriodo);

	static Periodo getPeriodoFinal();
	static Periodo getPeriodoFinal(TipoPeriodo a_tipoPeriodo);

	IdEstacao getEstacao() const;

	static IdEstacao getMaiorEstacao(TipoPeriodo a_tipoPeriodo);

	Periodo deslocarPeriodo(const Periodo& a_periodo, int a_numero_de_horas);
	Periodo getPeriodoDiario_do_diaFinal(const Periodo& a_periodo);

	~Periodo();

};

static std::string getString(const Periodo &a_periodo) { return a_periodo.str(); }
static std::string getStringTipo(const Periodo &a_periodo) { return "Periodo"; }
static std::string getFullString(const Periodo &a_periodo) { return a_periodo.str(); }

static Periodo getPeriodoFromChar(const char * a_char) {
	if (strCompara(std::string(a_char), "min")) { return Periodo::getPeriodoInicial(); }
	else if (strCompara(std::string(a_char), "max")) { return Periodo::getPeriodoFinal(); }
	return Periodo(std::string(a_char));
};

static Periodo getFromChar(const Periodo, const char* a_char) {
	return getPeriodoFromChar(a_char);
};

#endif