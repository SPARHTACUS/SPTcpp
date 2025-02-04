#ifndef C_PERIODO_H
#define C_PERIODO_H

#include <vector>
#include <string>

#include "C_SmartEnum.h"



class Periodo {

private:

	IdAno ano;
	IdMes mes;
	IdDia dia;
	IdHor hora;
	IdMin minuto;

	std::pair<unsigned int, char> duration;

	void inicializaAtributos();

	void validaAno   (const IdAno a_ano);
	void validaMes   (const IdMes a_mes);
	void validaDia   (const IdDia a_hor);
	void validaHora  (const IdHor a_hora);
	void validaMinuto(const IdMin a_minuto);

	void setPeriodo(const std::pair<unsigned int, char>& a_dur, const std::string a_periodo);
	void setPeriodo(const std::pair<unsigned int, char>& a_dur, const IdAno a_ano);
	void setPeriodo(const std::pair<unsigned int, char>& a_dur, const IdMes a_mes, const IdAno a_ano);
	void setPeriodo(const std::pair<unsigned int, char>& a_dur, const IdDia a_hor, const IdMes a_mes, const IdAno a_ano);
	void setPeriodo(const std::pair<unsigned int, char>& a_dur, const IdDia a_hor, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora);
	void setPeriodo(const std::pair<unsigned int, char>& a_dur, const IdDia a_hor, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora, const IdMin a_minuto);

public:

	Periodo();

	Periodo(const std::string a_duration, const Periodo &a_periodo);

	Periodo(const std::string a_periodo);
	Periodo(const std::string a_duration, const std::string a_periodo);

	Periodo(const int a_ano);
	Periodo(const std::string a_duration, const int a_ano);

	Periodo(const IdAno a_ano);
	Periodo(const std::string a_duration, const IdAno a_ano);

	Periodo(const int a_mes, const int a_ano);
	Periodo(const std::string a_duration, const int a_mes, const int a_ano);

	Periodo(const IdMes a_mes, const IdAno a_ano);
	Periodo(const std::string a_duration, const IdMes a_mes, const IdAno a_ano);

	Periodo(const int a_dia, const int a_mes, const int a_ano);
	Periodo(const std::string a_duration, const int a_dia, const int a_mes, const int a_ano);

	Periodo(const IdDia a_dia, const IdMes a_mes, const IdAno a_ano);
	Periodo(const std::string a_duration, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano);

	Periodo(const int a_dia, const int a_mes, const int a_ano, const int a_hora);
	Periodo(const std::string a_duration, const int a_dia, const int a_mes, const int a_ano, const int a_hora);
	
	Periodo(const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora);
	Periodo(const std::string a_duration, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora);

	Periodo(const int a_dia, const int a_mes, const int a_ano, const int a_hora, const int a_minuto);
	Periodo(const std::string a_duration, const int a_dia, const int a_mes, const int a_ano, const int a_hora, const int a_minuto);

	Periodo(const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora, const IdMin a_minuto);
	Periodo(const std::string a_duration, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora, const IdMin a_minuto);

	double sobreposicao(const Periodo a_periodo_overlap)const;
	
	double atraso(const Periodo a_periodo)const;

	IdAno getAno()    const { return ano;    };
	IdMes getMes()    const { return mes;    };
	IdDia getDia()    const { return dia;    };
	IdHor getHora()   const { return hora;   };
	IdMin getMinuto() const { return minuto; };
	IdDiaSemana getDiaSemana() const;

	IdDia getMaiorDiaDoMes() const;
	IdDia getMaiorDiaDoMes(IdMes a_idMes) const;

	unsigned int getMeses()   const;
	unsigned int getDias()    const;
	unsigned int getHoras()   const;
	unsigned int getMinutos() const;
	unsigned int getSegundos() const;

	std::pair<unsigned int, char> getDuration()const;

	int posStructPeriod = -1;
	int idxStructPeriod = -1;
	std::string codStructPeriod;

	bool anoBissexto() const;

	std::string str() const;

	bool isValido() const ;

	// Operadores

	friend Periodo operator+(const Periodo &a_periodo, const int a_iterador);

	friend Periodo operator-(const Periodo &a_periodo, const int a_iterador);

	friend unsigned int operator-(const Periodo &a_periodo1, const Periodo &a_periodo2);

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

	static Periodo getPeriodoFinal();

	static std::pair<unsigned int, char> getDurationFromStr(const std::string &a_str);

	static bool isValidDuration(const std::pair<unsigned int, char>& a_dur1);
	static bool isValidDurationT(const char a_durT);

	static bool isSameDuration(const Periodo &a_per1, const Periodo& a_per2);

	static Periodo getPeriodBtwn(const Periodo& a_per1, const Periodo& a_per2);

	static std::vector<std::string> getDurT();

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