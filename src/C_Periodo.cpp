#include "C_Periodo.h"

#include <sstream>

static const char* formatoPadrao = "DD/MM/AAAA-hh:mm:ss-Duration";
//Formas simplificadas de ingressar o Periodo:
//AAAA
//MM/AAAA
//DD/MM/AAAA
//DD/MM/AAAA-hh:mm
//DD/MM/AAAA-TipoPeriodo
//DD/MM/AAAA-hh:mm-TipoPeriodo


static const char* arraySeparador = "/:-";

static const int numeroElementosArraySeparador = 3;

static const int ano_inicial = 1900;
static const int ano_final = 2200;


void Periodo::inicializaAtributos() {

	ano = IdAno_Nenhum;
	mes = IdMes_Nenhum;
	dia = IdDia_Nenhum;
	hora = IdHor_Nenhum;
	minuto = IdMin_Nenhum;

	duration.first = 0;
	duration.second = '0';

} // void Periodo::inicializaAtributos(){


bool Periodo::anoBissexto() const {

	return anoBissexto(ano);

} // bool Periodo::anoBissexto() const{


bool Periodo::anoBissexto(const IdAno a_ano) {
	try {
		if ((a_ano <= IdAno_Nenhum) || (a_ano >= IdAno_Excedente)) { throw std::invalid_argument("Ano invalido. Consulte os limites minimo e maximo."); }
		int anoInt = atoi(getString(a_ano).c_str());
		return anoBissexto(anoInt);
	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::anoBissexto(" + getString(a_ano) + "): \n" + std::string(erro.what())); }
}

bool Periodo::anoBissexto(const int a_ano) { return bool((a_ano % 4 == 0 && a_ano % 100 != 0) || (a_ano % 400 == 0)); }


IdDia Periodo::getMaiorDiaDoMes() const {

	if (mes != IdMes_Nenhum) {

		IdDia maiorDia = IdDia_Nenhum;

		if (mes == IdMes_4 || mes == IdMes_6 || mes == IdMes_9 || mes == IdMes_11)
			maiorDia = IdDia_30;

		else if (mes == IdMes_2) {

			if (anoBissexto())
				maiorDia = IdDia_29;
			else
				maiorDia = IdDia_28;
		} // else if (mes == IdMes_2) {

		else
			maiorDia = IdDia_31;

		return maiorDia;

	} // if (mes != IdMes_Nenhum) {

	return IdDia_Nenhum;

} // void Periodo::setMaiorDiaDoMes() {

IdDiaSemana Periodo::getDiaSemana()const {
	try {
		if (!isValido()) { throw std::invalid_argument("Periodo Invalido."); }

		const int anoInt = int(ano) - 1 + ano_inicial;

		const int mesInt = int(mes);

		const int diaInt = int(dia);

		int diaSemanaInt = diaInt + ((153 * (mesInt + 12 * ((14 - mesInt) / 12) - 3) + 2) / 5) \
			+ (365 * (anoInt + 4800 - ((14 - mesInt) / 12)))                                  \
			+ ((anoInt + 4800 - ((14 - mesInt) / 12)) / 4)                              \
			- ((anoInt + 4800 - ((14 - mesInt) / 12)) / 100)                            \
			+ ((anoInt + 4800 - ((14 - mesInt) / 12)) / 400)                            \
			- 32045;

		const int restoDiaSemana = (diaSemanaInt + 1) % 7;
		if ((restoDiaSemana < 0) || (restoDiaSemana > 6)) { throw std::invalid_argument("Erro no calculo do dia da semana."); }
		return IdDiaSemana(restoDiaSemana + 1);
	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::setDiaSemana(): \n" + std::string(erro.what())); }
}


void Periodo::validaAno(const IdAno a_ano) {
	try {
		if ((a_ano < IdAno(1)) || (a_ano >= IdAno_Excedente)) { throw std::invalid_argument("Ano invalido. Consulte os limites minimo e maximo."); }
		ano = a_ano;
	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::validaAno(" + getString(a_ano) + "): \n" + std::string(erro.what())); }
} // void Periodo::validaAno(IdAno a_ano) {

void Periodo::validaMes(const IdMes a_mes) {
	try {
		if ((a_mes < IdMes_1) || (a_mes >= IdMes_Excedente)) { throw std::invalid_argument("Mes invalido."); }
		mes = a_mes;
	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::validaMes(" + getString(a_mes) + "): \n" + std::string(erro.what())); }
} // void Periodo::validaMes(const IdMes a_mes){

void Periodo::validaDia(const IdDia a_dia) {
	try {
		if ((IdDia_1 > a_dia) || (a_dia > getMaiorDiaDoMes())) { throw std::invalid_argument("Dia invalido."); }
		dia = a_dia;
	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::validaDia(" + getString(a_dia) + "): \n" + std::string(erro.what())); }
} // void Periodo::validaDia(const IdDia a_dia){

void Periodo::validaHora(const IdHor a_hora) {
	try {
		if ((a_hora < IdHor_0) || (a_hora >= IdHor_Excedente)) { throw std::invalid_argument("Hora invalida."); }
		hora = a_hora;
	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::validaHora(" + getString(a_hora) + "): \n" + std::string(erro.what())); }
} // void Periodo::validaHora(const IdHor a_hora){


void Periodo::validaMinuto(const IdMin a_minuto) {
	try {
		if ((a_minuto < IdMin_0) || (a_minuto >= IdMin_Excedente)) { throw std::invalid_argument("Minuto invalido."); }
		minuto = a_minuto;
	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::validaMinuto(" + getString(a_minuto) + "): \n" + std::string(erro.what())); }
} // void Periodo::validaMinuto(const IdMin a_minuto){




void Periodo::setPeriodo(const std::pair<unsigned int, char>& a_dur, const std::string a_perStr) {
	try {

		std::vector<size_t> posSeparador;
		std::vector<char>   queSeparador;

		for (std::string::size_type pos = 0; pos < a_perStr.size(); ++pos) {
			for (int i = 0; i < numeroElementosArraySeparador; i++) {
				if (a_perStr.at(pos) == arraySeparador[i]) {
					posSeparador.push_back(pos);
					queSeparador.push_back(arraySeparador[i]);
					break;
				} // if (a_perStr.at(pos) == arraySeparador[i]) {
			} // for (int i = 0; i < numeroElementosArraySeparador; i++) {
		} // for (std::string::size_type pos = 0; pos < a_perStr.size(); ++pos) {

		std::pair<unsigned int, char> duration_ = a_dur;

		const bool isDurValid = isValidDuration(a_dur);

		IdHor horLido = IdHor_0;
		IdMin minLido = IdMin_0;
		IdDia diaLido = IdDia_1;
		IdMes mesLido = IdMes_1;
		IdAno anoLido = IdAno_Nenhum;

		if (posSeparador.size() == 0) {
			if (!isDurValid)
				duration_ = getDurationFromStr("a");
			anoLido = getIdAnoFromChar(a_perStr.c_str());
		}

		else if (posSeparador.size() == 1) {
			if (queSeparador.at(0) == '/') {
				if (!isDurValid)
					duration_ = getDurationFromStr("M");
				anoLido = getIdAnoFromChar(std::string(a_perStr.substr(posSeparador.at(0) + 1, std::string::npos)).c_str());
				mesLido = getIdMesFromChar(a_perStr.substr(0, posSeparador.at(0)).c_str());
			}
			else
				throw std::invalid_argument("String com formato invalido de Periodo. Formato padrao completo " + std::string(formatoPadrao));
		} // if (posSeparador.size() == 1) {

		else if (posSeparador.size() == 2) {
			if ((queSeparador.at(0) == '/') && (queSeparador.at(1) == '/')) {
				if (!isDurValid)
					duration_ = getDurationFromStr("d");
				diaLido = getIdDiaFromChar(a_perStr.substr(0, posSeparador.at(0)).c_str());
				mesLido = getIdMesFromChar(a_perStr.substr(posSeparador.at(0) + 1, posSeparador.at(1) - posSeparador.at(0) - 1).c_str());
				anoLido = getIdAnoFromChar(std::string(a_perStr.substr(posSeparador.at(1) + 1, std::string::npos)).c_str());
			}
			else if ((queSeparador.at(0) == '/') && (queSeparador.at(1) == '-')) {
				if (!isDurValid)
					duration_ = getDurationFromStr("M");
				anoLido = getIdAnoFromChar(std::string(a_perStr.substr(posSeparador.at(0) + 1, posSeparador.at(1) - posSeparador.at(0) - 1)).c_str());
				mesLido = getIdMesFromChar(a_perStr.substr(0, posSeparador.at(0)).c_str());
			}
			else
				throw std::invalid_argument("String com formato invalido de Periodo. Formato padrao completo " + std::string(formatoPadrao));
		} // else if (posSeparador.size() == 2) {

		else if (posSeparador.size() == 3) {
			if ((queSeparador.at(0) == '/') && (queSeparador.at(1) == '/') && (queSeparador.at(2) == '-')) {}
			else
				throw std::invalid_argument("String com formato invalido de Periodo. Formato padrao completo " + std::string(formatoPadrao));
		} // else if (posSeparador.size() == 3) {

		else if (posSeparador.size() == 4) {
			if ((queSeparador.at(0) == '/') && (queSeparador.at(1) == '/') && (queSeparador.at(2) == '-') && (queSeparador.at(3) == ':')) {
				if (!isDurValid)
					duration_ = getDurationFromStr("m");
			}
			else
				throw std::invalid_argument("String com formato invalido de Periodo. Formato padrao completo " + std::string(formatoPadrao));
		} // else if (posSeparador.size() == 4) {

		else if (posSeparador.size() == 5) {
			if ((queSeparador.at(0) == '/') && (queSeparador.at(1) == '/') && (queSeparador.at(2) == '-') && (queSeparador.at(3) == ':') && (queSeparador.at(4) == ':')) {
				if (!isDurValid)
					duration_ = getDurationFromStr("m");
			}
			else if ((queSeparador.at(0) == '/') && (queSeparador.at(1) == '/') && (queSeparador.at(2) == '-') && (queSeparador.at(3) == ':') && (queSeparador.at(4) == '-')) {}
			else
				throw std::invalid_argument("String com formato invalido de Periodo. Formato padrao completo " + std::string(formatoPadrao));
		} // else if (posSeparador.size() == 5) {

		else if (posSeparador.size() == 6) {
			if ((queSeparador.at(0) == '/') && (queSeparador.at(1) == '/') && (queSeparador.at(2) == '-') && (queSeparador.at(3) == ':') && (queSeparador.at(4) == ':') && (queSeparador.at(5) == '-')) {}
			else
				throw std::invalid_argument("String com formato invalido de Periodo. Formato padrao completo " + std::string(formatoPadrao));
		} // else if (posSeparador.size() == 6) {
		else
			throw std::invalid_argument("String com formato invalido de Periodo. Formato padrao completo " + std::string(formatoPadrao));

		if (anoLido == IdAno_Nenhum) {

			diaLido = getIdDiaFromChar(a_perStr.substr(0, posSeparador.at(0)).c_str());
			mesLido = getIdMesFromChar(a_perStr.substr(posSeparador.at(0) + 1, posSeparador.at(1) - posSeparador.at(0) - 1).c_str());
			anoLido = getIdAnoFromChar(a_perStr.substr(posSeparador.at(1) + 1, posSeparador.at(2) - posSeparador.at(1) - 1).c_str());

			horLido = IdHor_0;
			minLido = IdMin_0;

			if (posSeparador.size() > 3) {
				horLido = getIdHorFromChar(a_perStr.substr(posSeparador.at(2) + 1, posSeparador.at(3) - posSeparador.at(2) - 1).c_str());
				minLido = getIdMinFromChar(a_perStr.substr(posSeparador.at(3) + 1, posSeparador.at(4) - posSeparador.at(3) - 1).c_str());
			} // if (posSeparador.size() > 3) {

			if (posSeparador.size() == 3)
				duration_ = getDurationFromStr(a_perStr.substr(posSeparador.at(2) + 1, std::string::npos));
			else if (posSeparador.size() == 5)
				duration_ = getDurationFromStr(a_perStr.substr(posSeparador.at(4) + 1, std::string::npos));
			else
				duration_ = getDurationFromStr(a_perStr.substr(posSeparador.at(5) + 1, std::string::npos));

		} // if (anoLido == IdAno_Nenhum) {

		setPeriodo(duration_, diaLido, mesLido, anoLido, horLido, minLido);

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::setPeriodo(" + getFullString(a_dur) + "," + a_perStr + "): \n" + std::string(erro.what())); }
} // void Periodo::setPeriodo(const std::pair<unsigned int, char>& a_dur, const std::string a_periodo){





void Periodo::setPeriodo(const std::pair<unsigned int, char>& a_dur, const IdAno a_ano) {
	try { setPeriodo(a_dur, IdDia_1, IdMes_1, a_ano, IdHor_0, IdMin_0); } // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::setPeriodo(" + getString(a_dur) + "," + getString(a_ano) + "): \n" + std::string(erro.what())); }
} // void Periodo::setPeriodo(const std::pair<unsigned int, char>& a_dur, const IdAno a_ano) {


void Periodo::setPeriodo(const std::pair<unsigned int, char>& a_dur, const IdMes a_mes, const IdAno a_ano) {
	try { setPeriodo(a_dur, IdDia_1, a_mes, a_ano, IdHor_0, IdMin_0); } // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::setPeriodo(" + getString(a_dur) + "," + getString(a_mes) + "," + getString(a_ano) + "): \n" + std::string(erro.what())); }
} // void Periodo::setPeriodo(const std::pair<unsigned int, char>& a_dur, const int IdMes, const IdAno a_ano){


void Periodo::setPeriodo(const std::pair<unsigned int, char>& a_dur, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano) {
	try { setPeriodo(a_dur, a_dia, a_mes, a_ano, IdHor_0, IdMin_0); } // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::setPeriodo(" + getString(a_dur) + "," + getString(a_dia) + "," + getString(a_mes) + "," + getString(a_ano) + "): \n" + std::string(erro.what())); }
} // void Periodo::setPeriodo(const std::pair<unsigned int, char>& a_dur, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano){


void Periodo::setPeriodo(const std::pair<unsigned int, char>& a_dur, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora) {
	try { setPeriodo(a_dur, a_dia, a_mes, a_ano, a_hora, IdMin_0); } // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::setPeriodo(" + getString(a_dur) + "," + getString(a_dia) + "," + getString(a_mes) + "," + getString(a_ano) + "," + getString(a_hora) + "): \n" + std::string(erro.what())); }
} // void Periodo::setPeriodo(const std::pair<unsigned int, char>& a_dur, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora){


void Periodo::setPeriodo(const std::pair<unsigned int, char>& a_dur, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora, const IdMin a_minuto) {
	try {
		if (!isValidDuration(a_dur)) { throw std::invalid_argument("Invalid a_dur."); }
		validaAno(a_ano);
		validaMes(a_mes);
		validaDia(a_dia);
		validaHora(a_hora);
		validaMinuto(a_minuto);
		duration = a_dur;
	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::setPeriodo(" + getString(a_dur) + "," + getString(a_dia) + "," + getString(a_mes) + "," + getString(a_ano) + "," + getString(a_hora) + "," + getString(a_minuto) + "): \n" + std::string(erro.what())); }
} // void Periodo::setPeriodo(const std::pair<unsigned int, char>& a_dur, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora, const IdMin a_minuto){


Periodo::Periodo() { inicializaAtributos(); }

Periodo::Periodo(const std::string a_duration, const Periodo& a_periodo) {
	try {
		if (!a_periodo.isValido())
			throw std::invalid_argument("Periodo Invalido.");
		inicializaAtributos();
		setPeriodo(getDurationFromStr(a_duration), a_periodo.getDia(), a_periodo.getMes(), a_periodo.getAno(), a_periodo.getHora(), a_periodo.getMinuto());
	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::Periodo(" + getFullString(a_duration) + "," + a_periodo.str() + ") \n" + std::string(erro.what())); }
}

Periodo::Periodo(const std::string a_periodo) {
	inicializaAtributos();
	setPeriodo(getDurationFromStr(""), a_periodo);
}

Periodo::Periodo(const std::string a_duration, const std::string a_periodo) {

	inicializaAtributos();
	setPeriodo(getDurationFromStr(a_duration), a_periodo);
} // Periodo::Periodo(const std::string a_duration, const std::string a_periodo) {


Periodo::Periodo(const int   a_ano) {
	std::stringstream anoStr;
	anoStr << a_ano;
	inicializaAtributos();
	setPeriodo(getDurationFromStr("1a"), getIdAnoFromChar(anoStr.str().c_str()));
} // Periodo::Periodo(const int   a_ano) {

Periodo::Periodo(const IdAno a_ano) { inicializaAtributos(); setPeriodo(getDurationFromStr("1a"), a_ano); }

Periodo::Periodo(const std::string a_duration, const int a_ano) {
	std::stringstream anoStr;
	anoStr << a_ano;
	inicializaAtributos();
	setPeriodo(getDurationFromStr(a_duration), getIdAnoFromChar(anoStr.str().c_str()));
} // Periodo::Periodo(const std::string a_duration, const int   a_ano) {

Periodo::Periodo(const std::string a_duration, const IdAno a_ano) { inicializaAtributos(); setPeriodo(getDurationFromStr(a_duration), a_ano); }

Periodo::Periodo(const int a_mes, const int a_ano) {
	std::stringstream anoStr, mesStr;
	anoStr << a_ano;
	mesStr << a_mes;
	inicializaAtributos();
	setPeriodo(getDurationFromStr("1M"), getIdMesFromChar(mesStr.str().c_str()), getIdAnoFromChar(anoStr.str().c_str()));
} // Periodo::Periodo(const int   a_mes, const int   a_ano) {

Periodo::Periodo(const IdMes a_mes, const IdAno a_ano) { inicializaAtributos(); setPeriodo(getDurationFromStr("1M"), a_mes, a_ano); }

Periodo::Periodo(const std::string a_duration, const int   a_mes, const int   a_ano) {
	std::stringstream anoStr, mesStr;
	anoStr << a_ano;
	mesStr << a_mes;
	inicializaAtributos();
	setPeriodo(getDurationFromStr(a_duration), getIdMesFromChar(mesStr.str().c_str()), getIdAnoFromChar(anoStr.str().c_str()));
} // Periodo::Periodo(const std::string a_duration, const int   a_mes, const int   a_ano) {

Periodo::Periodo(const std::string a_duration, const IdMes a_mes, const IdAno a_ano) { inicializaAtributos(); setPeriodo(getDurationFromStr(a_duration), a_mes, a_ano); }

Periodo::Periodo(const int   a_dia, const int   a_mes, const int   a_ano) {
	std::stringstream anoStr, mesStr, diaStr;
	anoStr << a_ano;
	mesStr << a_mes;
	diaStr << a_dia;
	inicializaAtributos();
	setPeriodo(getDurationFromStr("1d"), getIdDiaFromChar(diaStr.str().c_str()), getIdMesFromChar(mesStr.str().c_str()), getIdAnoFromChar(anoStr.str().c_str()));
} // Periodo::Periodo(const int   a_dia, const int   a_mes, const int   a_ano) { 

Periodo::Periodo(const IdDia a_dia, const IdMes a_mes, const IdAno a_ano) { inicializaAtributos(); setPeriodo(getDurationFromStr("1d"), a_dia, a_mes, a_ano); }

Periodo::Periodo(const std::string a_duration, const int   a_dia, const int   a_mes, const int   a_ano) {
	std::stringstream anoStr, mesStr, diaStr;
	anoStr << a_ano;
	mesStr << a_mes;
	diaStr << a_dia;
	inicializaAtributos();
	setPeriodo(getDurationFromStr(a_duration), getIdDiaFromChar(diaStr.str().c_str()), getIdMesFromChar(mesStr.str().c_str()), getIdAnoFromChar(anoStr.str().c_str()));
} // Periodo::Periodo(const std::string a_duration, const int   a_dia, const int   a_mes, const int   a_ano) {

Periodo::Periodo(const std::string a_duration, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano) { inicializaAtributos(); setPeriodo(getDurationFromStr(a_duration), a_dia, a_mes, a_ano); }

Periodo::Periodo(const int   a_dia, const int   a_mes, const int   a_ano, const int   a_hora) {
	std::stringstream anoStr, mesStr, diaStr, horStr;
	anoStr << a_ano;
	mesStr << a_mes;
	diaStr << a_dia;
	horStr << a_hora;
	inicializaAtributos();
	setPeriodo(getDurationFromStr("1h"), getIdDiaFromChar(diaStr.str().c_str()), getIdMesFromChar(mesStr.str().c_str()), getIdAnoFromChar(anoStr.str().c_str()), getIdHorFromChar(horStr.str().c_str()));
} // Periodo::Periodo(const int   a_dia, const int   a_mes, const int   a_ano, const int   a_hora) {

Periodo::Periodo(const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora) { inicializaAtributos(); setPeriodo(getDurationFromStr("1h"), a_dia, a_mes, a_ano, a_hora); }

Periodo::Periodo(const std::string a_duration, const int   a_dia, const int   a_mes, const int   a_ano, const int   a_hora) {
	std::stringstream anoStr, mesStr, diaStr, horStr;
	anoStr << a_ano;
	mesStr << a_mes;
	diaStr << a_dia;
	horStr << a_hora;
	inicializaAtributos();
	setPeriodo(getDurationFromStr(a_duration), getIdDiaFromChar(diaStr.str().c_str()), getIdMesFromChar(mesStr.str().c_str()), getIdAnoFromChar(anoStr.str().c_str()), getIdHorFromChar(horStr.str().c_str()));
} // Periodo::Periodo(const std::string a_duration, const int   a_dia, const int   a_mes, const int   a_ano, const int   a_hora) {

Periodo::Periodo(const std::string a_duration, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora) { inicializaAtributos(); setPeriodo(getDurationFromStr(a_duration), a_dia, a_mes, a_ano, a_hora); }

Periodo::Periodo(const int   a_dia, const int   a_mes, const int   a_ano, const int a_hora, const int a_minuto) {
	std::stringstream anoStr, mesStr, diaStr, horStr, minStr;
	anoStr << a_ano;
	mesStr << a_mes;
	diaStr << a_dia;
	horStr << a_hora;
	minStr << a_minuto;
	inicializaAtributos();
	setPeriodo(getDurationFromStr("1m"), getIdDiaFromChar(diaStr.str().c_str()), getIdMesFromChar(mesStr.str().c_str()), getIdAnoFromChar(anoStr.str().c_str()), getIdHorFromChar(horStr.str().c_str()), getIdMinFromChar(minStr.str().c_str()));
} // Periodo::Periodo(const int   a_dia, const int   a_mes, const int   a_ano, const int   a_hora, const int   a_minuto) {

Periodo::Periodo(const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora, const IdMin a_minuto) { inicializaAtributos(); setPeriodo(getDurationFromStr("1m"), a_dia, a_mes, a_ano, a_hora, a_minuto); }

Periodo::Periodo(const std::string a_duration, const int   a_dia, const int   a_mes, const int   a_ano, const int   a_hora, const int   a_minuto) {
	std::stringstream anoStr, mesStr, diaStr, horStr, minStr;
	anoStr << a_ano;
	mesStr << a_mes;
	diaStr << a_dia;
	horStr << a_hora;
	minStr << a_minuto;
	inicializaAtributos();
	setPeriodo(getDurationFromStr(a_duration), getIdDiaFromChar(diaStr.str().c_str()), getIdMesFromChar(mesStr.str().c_str()), getIdAnoFromChar(anoStr.str().c_str()), getIdHorFromChar(horStr.str().c_str()), getIdMinFromChar(minStr.str().c_str()));
} // Periodo::Periodo(const std::string a_duration, const int   a_dia, const int   a_mes, const int   a_ano, const int   a_hora, const int   a_minuto) {

Periodo::Periodo(const std::string a_duration, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora, const IdMin a_minuto) { inicializaAtributos(); setPeriodo(getDurationFromStr(a_duration), a_dia, a_mes, a_ano, a_hora, a_minuto); }


double Periodo::sobreposicao(const Periodo a_periodo_overlap)const {

	try {

		const Periodo periodo_overlap_seguinte = a_periodo_overlap + 1;

		const Periodo periodo_base = *this;

		const Periodo periodo_base_seguinte = periodo_base + 1;

		if (periodo_overlap_seguinte <= periodo_base)
			return 0.0;

		if (a_periodo_overlap >= periodo_base_seguinte)
			return 0.0;

		else if (periodo_base == a_periodo_overlap)
			return 1.0;

		else if ((periodo_base >= a_periodo_overlap) && (periodo_base_seguinte <= periodo_overlap_seguinte))
			return 1.0;

		else if ((periodo_base <= a_periodo_overlap) && (periodo_base_seguinte >= periodo_overlap_seguinte))
			return double(a_periodo_overlap.getMinutos()) / double(periodo_base.getMinutos());

		else {

			Periodo periodo_ini;
			Periodo periodo_fim;

			if ((periodo_base >= a_periodo_overlap) && (periodo_base_seguinte >= periodo_overlap_seguinte)) {

				periodo_ini = Periodo("m", periodo_base);
				periodo_fim = Periodo("m", periodo_overlap_seguinte);

			} // if ((periodo_base > a_periodo_overlap) && (periodo_base_seguinte > a_periodo_overlap)) {	 

			else if ((periodo_base <= a_periodo_overlap) && (periodo_base_seguinte <= periodo_overlap_seguinte)) {

				periodo_ini = Periodo("m", a_periodo_overlap);
				periodo_fim = Periodo("m", periodo_base_seguinte);

			} // else if ((periodo_base < a_periodo_overlap) && (periodo_base_seguinte < a_periodo_overlap)) {

			int minutos_sobreposicao = 0;

			for (Periodo periodo = periodo_ini; periodo < periodo_fim; periodo++) {
				if ((periodo >= periodo_base) && (periodo < periodo_base_seguinte))
					minutos_sobreposicao += periodo.getMinutos();
				if (periodo >= periodo_base_seguinte)
					break;
			} // for (Periodo periodo = periodo_ini; periodo < periodo_fim; periodo++) {

			return double(minutos_sobreposicao) / double(periodo_base.getMinutos());

		} // else {

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::sobreposicao(" + a_periodo_overlap.str() + ") \n" + std::string(erro.what())); }

} // double Periodo::sobreposicao(const Periodo a_periodo_overlap){


double Periodo::atraso(const Periodo a_periodo) const {

	try {

		const Periodo periodo_base = *this;

		if (periodo_base >= a_periodo)
			return 0.0;

		const Periodo periodo_ini = Periodo("m", periodo_base);
		const Periodo periodo_fim = Periodo("m", a_periodo);

		int minutos_atraso = 0;

		for (Periodo periodo = periodo_ini; periodo < periodo_fim; periodo++)
			minutos_atraso += periodo.getMinutos();

		return double(minutos_atraso) / double(periodo_base.getMinutos());

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::sobreposicao(" + getString(a_periodo) + ") \n" + std::string(erro.what())); }

} // double Periodo::atraso(const Periodo a_periodo_atraso) const {


unsigned int Periodo::getMeses() const {
	try{

		if (!isValido())
			throw std::invalid_argument("Invalid Period.");

		if (duration.second == 'a')
			return 12U * duration.first;
		else if (duration.second == 'M')
			return duration.first;

		const Periodo periodIni("M", *this);
		const Periodo periodEnd = *this + 1;

		unsigned int numMes = 0;
		for (Periodo period = periodIni; period + 1 <= periodEnd; period++)
			numMes++;

		return numMes;

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::getMeses() \n" + std::string(erro.what())); }

} // unsigned int Periodo::getMeses() const{


unsigned int Periodo::getDias() const {

	try {

		if (!isValido())
			throw std::invalid_argument("Invalid Period.");

		if (duration.second == 'd')
			return duration.first;

		if (duration.second == 'h')
			return unsigned int(duration.first / 24U);

		if (duration.second == 'm')
			return unsigned int(duration.first / 1440U);

		if (duration.first == 1U) {
			if ((dia == IdDia_1) && (hora == IdHor_1) && (minuto == IdMin_1)) {
				if (duration.second == 'a') {
					if (mes <= IdMes_2) {
						if (anoBissexto())
							return 366U;
					}
					return 365U;
				}
				else if (duration.second == 'M') {
					if (mes == IdMes_1) return 31U;
					else if (mes == IdMes_2) {
						if (anoBissexto())
							return 29U;
						return 28U;
					}
					else if (mes == IdMes_3) return 31U;
					else if (mes == IdMes_4) return 30U;
					else if (mes == IdMes_5) return 31U;
					else if (mes == IdMes_6) return 30U;
					else if (mes == IdMes_7) return 31U;
					else if (mes == IdMes_8) return 31U;
					else if (mes == IdMes_9) return 30U;
					else if (mes == IdMes_10) return 31U;
					else if (mes == IdMes_11) return 30U;
					else if (mes == IdMes_12) return 31U;
				}
			}
		}

		const Periodo periodIni("d", *this);
		const Periodo periodEnd = *this + 1;

		unsigned int numDia = 0;
		for (Periodo period = periodIni; period + 1 <= periodEnd; period++)
			numDia++;

		return numDia;

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::getDias() \n" + std::string(erro.what())); }

} // unsigned int Periodo::getDias() const{


unsigned int Periodo::getHoras() const {

	try {

		if (!isValido())
			throw std::invalid_argument("Invalid Period.");

		if (duration.second == 'd')
			return  unsigned int(duration.first * 24U);

		if (duration.second == 'h')
			return duration.first;

		if (duration.second == 'm')
			return unsigned int(duration.first / 60U);

		return getDias() * 24U;

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::getHoras() \n" + std::string(erro.what())); }

} // int Periodo::getHoras() const{


unsigned int Periodo::getMinutos() const {

	try {

		if (!isValido())
			throw std::invalid_argument("Invalid Period.");

		if (duration.second == 'd')
			return  unsigned int(duration.first * 1440U);

		if (duration.second == 'h')
			return unsigned int(duration.first * 60U);

		if (duration.second == 'm')
			return duration.first;

		return getDias() * 1440U;

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::getMinutos() \n" + std::string(erro.what())); }

} // int Periodo::getMinutos() const{


unsigned int Periodo::getSegundos() const {

	try {

		return getMinutos() * 60U;

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::getSegundos() \n" + std::string(erro.what())); }

} // int Periodo::getSegundos() const{

std::pair<unsigned int, char> Periodo::getDuration() const{

	return duration;
}


std::string normStringMes(const IdMes a_idMes) {
	if (a_idMes <= IdMes_9)
		return "0" + getString(a_idMes);
	else
		return getString(a_idMes);
}

std::string normStringDia(const IdDia a_idDia) {
	if (a_idDia <= IdDia_9)
		return "0" + getString(a_idDia);
	else
		return getString(a_idDia);
}

std::string normStringHor(const IdHor a_idHor) {
	if (a_idHor <= IdHor_9)
		return "0" + getString(a_idHor);
	else
		return getString(a_idHor);
}

std::string normStringMin(const IdMin a_idMin) {
	if (a_idMin <= IdMin_9)
		return "0" + getString(a_idMin);
	else
		return getString(a_idMin);
}

std::string Periodo::str() const {

	std::string durStr = getString(duration);
	if (duration.first == 1U)
		durStr = duration.second;

	if (duration.second == 'm')
		return std::string(normStringDia(dia) + "/" + normStringMes(mes) + "/" + getString(ano) + "-" + normStringHor(hora) + ":" + normStringMin(minuto) + "-" + durStr);

	else if ((hora == IdHor_0) && (minuto == IdMin_0)) {

		if (duration.second == 'd')
			return std::string(normStringDia(dia) + "/" + normStringMes(mes) + "/" + getString(ano) + "-" + durStr);

		else if ((duration.second == 'M') && (dia == IdDia_1))
			return std::string(normStringMes(mes) + "/" + getString(ano) + "-" + durStr);

		else if ((duration.second == 'a') && (mes == IdMes_1) && (dia == IdDia_1))
			return std::string(getString(ano) + "-" + durStr);

		return std::string(normStringDia(dia) + "/" + normStringMes(mes) + "/" + getString(ano) + "-" + durStr);

	} // else if ((hora == IdHor_0) && (minuto == IdMin_0)) {

	return std::string(normStringDia(dia) + "/" + normStringMes(mes) + "/" + getString(ano) + "-" + normStringHor(hora) + ":" + normStringMin(minuto) + ":-" + durStr);

} // std::string Periodo::str() const{


bool Periodo::isValido() const { 
	
	if (isValidDuration(getDuration()))
		return true;

	return false;
}


Periodo operator+(const Periodo& a_periodo, const int a_iterador) {

	try {

		if (!a_periodo.isValido())
			throw std::invalid_argument("Periodo invalido.");

		const std::pair<unsigned int, char> duration_ = a_periodo.getDuration();

		IdAno anoIter;
		IdMes mesIter;
		IdDia diaIter;
		IdHor horIter;
		IdMin minIter;

		if (duration_.second == 'a')
			Periodo::iteraAno(a_periodo, a_iterador * duration_.first, anoIter, mesIter, diaIter, horIter, minIter);

		else if (duration_.second == 'M')
			Periodo::iteraMes(a_periodo, a_iterador * duration_.first, anoIter, mesIter, diaIter, horIter, minIter);

		else if (duration_.second == 'd')
			Periodo::iteraDia(a_periodo, a_iterador * duration_.first, anoIter, mesIter, diaIter, horIter, minIter);

		else if (duration_.second == 'h')
			Periodo::iteraHora(a_periodo, a_iterador * duration_.first, anoIter, mesIter, diaIter, horIter, minIter);

		else if (duration_.second == 'm')
			Periodo::iteraMinuto(a_periodo, a_iterador * duration_.first, anoIter, mesIter, diaIter, horIter, minIter);

		else
			throw std::invalid_argument("Tipo de Periodo invalido.");

		return Periodo(getString(duration_), diaIter, mesIter, anoIter, horIter, minIter);

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::+(" + a_periodo.str() + "," + std::to_string(a_iterador) + "): \n" + std::string(erro.what())); }

} // Periodo operator+(const Periodo &a_periodo, const int a_iterador){


Periodo operator-(const Periodo& a_periodo, const int a_iterador) { return operator+(a_periodo, -a_iterador); }


unsigned int operator-(const Periodo& a_periodo1, const Periodo& a_periodo2) {

	try {

		if (!a_periodo1.isValido())
			throw std::invalid_argument("Argumento 1 com Periodo invalido.");

		else if (!a_periodo2.isValido())
			throw std::invalid_argument("Argumento 2 com Periodo invalido.");

		if (!Periodo::isSameDuration(a_periodo1, a_periodo2))
			throw std::invalid_argument("Tipos de Periodo diferentes inviabilizam a operacao.");

		const std::pair<unsigned int, char> dur1 = a_periodo1.getDuration();
		const std::pair<unsigned int, char> dur2 = a_periodo2.getDuration();

		if (dur1.first == 1U) {

			if (dur1.second == 'a')
				return a_periodo1.getAno() - a_periodo2.getAno();

			else if (dur1.second == 'M')
				return (a_periodo1.getAno() - a_periodo2.getAno()) * 12 + a_periodo1.getMes() - a_periodo2.getMes();

			else if (dur1.second == 'd') {

				int contador = 0;

				if (a_periodo1 >= a_periodo2) {

					Periodo periodo_teste = a_periodo2;

					while (true) {

						periodo_teste = periodo_teste + 1;

						if (periodo_teste <= a_periodo1)
							contador++;

						if (periodo_teste >= a_periodo1)
							break;

					}//while (true) {

				}//if (a_periodo1 >= a_periodo2) {
				else if (a_periodo1 < a_periodo2) {

					Periodo periodo_teste = a_periodo1;

					while (true) {

						periodo_teste = periodo_teste + 1;

						if (periodo_teste <= a_periodo2)
							contador--;

						if (periodo_teste >= a_periodo2)
							break;

					}//while (true) {


				}//else if (a_periodo1 < a_periodo2) {

				return contador;

			}// else if (dur1.second == 'd') {

			else if ((dur1.second == 'h') || (dur1.second == 'm')) {

				int iterador;
				if (a_periodo1 > a_periodo2)
					iterador = 1;
				else if (a_periodo1 < a_periodo2)
					iterador = -1;
				else
					return 0;

				int contador = 0;
				Periodo periodo = a_periodo1;
				while (true) {
					periodo = periodo - iterador;
					contador++;
					if (a_periodo1 > a_periodo2) {
						if (periodo < a_periodo2) { contador--; break; }
						else if (periodo == a_periodo2) { break; }
					}

					else if (a_periodo1 < a_periodo2) {
						if (periodo > a_periodo2) { contador--; break; }
						else if (periodo == a_periodo2) { break; }
					}
				} // while (true) {

				return 	contador * iterador;

			} // else if ((dur1.second == 'h') || (dur1.second == 'm')) {

			else
				throw std::invalid_argument("Tipo de Periodo invalido.");

		}

		return 0;

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::-(" + a_periodo1.str() + "," + a_periodo2.str() + "): \n" + std::string(erro.what())); }

} // int operator-(const Periodo &a_periodo1, const Periodo &a_periodo2){


void operator++(Periodo& a_periodo, int) {
	try {
		if (a_periodo.isValido())
			a_periodo = a_periodo + 1;
		else
			throw std::invalid_argument("Periodo invalido.");
	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::++(" + a_periodo.str() + "): \n" + std::string(erro.what())); }
} // void operator++(Periodo & a_periodo, int) {


void operator--(Periodo& a_periodo, int) {
	try {
		if (a_periodo.isValido())
			a_periodo = a_periodo - 1;
		else
			throw std::invalid_argument("Periodo invalido.");
	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::--(" + a_periodo.str() + "): \n" + std::string(erro.what())); }
} // void operator++(Periodo & a_periodo, int) {



bool operator==(const Periodo& a_periodo1, const Periodo& a_periodo2) {

	try {

		if (!a_periodo1.isValido())
			throw std::invalid_argument("Argumento 1 com Periodo invalido.");

		else if (!a_periodo2.isValido())
			throw std::invalid_argument("Argumento 2 com Periodo invalido.");

		if (!Periodo::isSameDuration(a_periodo1, a_periodo2))
			return false;

		else if (a_periodo1.getAno() != a_periodo2.getAno())
			return false;

		else if (a_periodo1.getMes() != a_periodo2.getMes())
			return false;

		else if (a_periodo1.getDia() != a_periodo2.getDia())
			return false;

		else if (a_periodo1.getHora() != a_periodo2.getHora())
			return false;

		else if (a_periodo1.getMinuto() != a_periodo2.getMinuto())
			return false;

		return true;

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::==(" + a_periodo1.str() + "," + a_periodo2.str() + "): \n" + std::string(erro.what())); }

} // bool operator==(const Periodo &a_periodo1, const Periodo &a_periodo2){


bool operator!=(const Periodo& a_periodo1, const Periodo& a_periodo2) {
	try {

		if (a_periodo1 == a_periodo2)
			return false;
		else
			return true;

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::!=(" + a_periodo1.str() + "," + a_periodo2.str() + "): \n" + std::string(erro.what())); }
} // bool operator!=(const Periodo & a_periodo1, const Periodo & a_periodo2){



bool operator<(const Periodo& a_periodo1, const Periodo& a_periodo2) {

	try {

		if (!a_periodo1.isValido())
			throw std::invalid_argument("Argumento 1 com Periodo invalido.");

		else if (!a_periodo2.isValido())
			throw std::invalid_argument("Argumento 2 com Periodo invalido.");

		if (a_periodo1.getAno() < a_periodo2.getAno())
			return true;

		else if ((a_periodo1.getAno() == a_periodo2.getAno())) {

			if (a_periodo1.getMes() < a_periodo2.getMes())
				return true;

			else if (a_periodo1.getMes() == a_periodo2.getMes()) {

				if ((a_periodo1.getDia() < a_periodo2.getDia()))
					return true;

				else if ((a_periodo1.getDia() == a_periodo2.getDia())) {

					if (a_periodo1.getHora() < a_periodo2.getHora())
						return true;

					else if (a_periodo1.getHora() == a_periodo2.getHora()) {

						if (a_periodo1.getMinuto() < a_periodo2.getMinuto())
							return true;
					} // else if (a_periodo1.getHora() == a_periodo2.getHora()) {
				} // else if ((a_periodo1.getDia() == a_periodo2.getDia())) {
			} // else if (a_periodo1.getMes() == a_periodo2.getMes()) {
		} // else if ((a_periodo1.getAno() == a_periodo2.getAno())) {

		return false;

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::<(" + a_periodo1.str() + "," + a_periodo2.str() + "): \n" + std::string(erro.what())); }

} // bool operator<(const Periodo & a_periodo1, const Periodo & a_periodo2){


bool operator<=(const Periodo& a_periodo1, const Periodo& a_periodo2) {

	try {

		if (!a_periodo1.isValido())
			throw std::invalid_argument("Argumento 1 com Periodo invalido.");

		else if (!a_periodo2.isValido())
			throw std::invalid_argument("Argumento 2 com Periodo invalido.");

		if (a_periodo1.getAno() < a_periodo2.getAno())
			return true;

		else if ((a_periodo1.getAno() == a_periodo2.getAno())) {

			if (a_periodo1.getMes() < a_periodo2.getMes())
				return true;

			else if (a_periodo1.getMes() == a_periodo2.getMes()) {

				if ((a_periodo1.getDia() < a_periodo2.getDia()))
					return true;

				else if ((a_periodo1.getDia() == a_periodo2.getDia())) {

					if (a_periodo1.getHora() < a_periodo2.getHora())
						return true;

					else if (a_periodo1.getHora() == a_periodo2.getHora()) {

						if (a_periodo1.getMinuto() <= a_periodo2.getMinuto())
							return true;
					} // else if (a_periodo1.getHora() == a_periodo2.getHora()) {
				} // else if ((a_periodo1.getDia() == a_periodo2.getDia())) {
			} // else if (a_periodo1.getMes() == a_periodo2.getMes()) {
		} // else if ((a_periodo1.getAno() == a_periodo2.getAno())) {

		return false;

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::<=(" + a_periodo1.str() + "," + a_periodo2.str() + "): \n" + std::string(erro.what())); }

} // bool operator<=(const Periodo & a_periodo1, const Periodo & a_periodo2){


bool operator>(const Periodo& a_periodo1, const Periodo& a_periodo2) { return operator<(a_periodo2, a_periodo1); }

bool operator>=(const Periodo& a_periodo1, const Periodo& a_periodo2) { return operator<=(a_periodo2, a_periodo1); }


void Periodo::iteraAno(const Periodo& a_periodo, const int a_iterador, IdAno& a_anoIter, IdMes& a_mesIter, IdDia& a_diaIter, IdHor& a_horaIter, IdMin& a_minutoIter) {

	if (a_iterador == 0) {
		a_anoIter = a_periodo.getAno();
		a_mesIter = a_periodo.getMes();
		a_diaIter = a_periodo.getDia();
		a_horaIter = a_periodo.getHora();
		a_minutoIter = a_periodo.getMinuto();
		return;
	} // if (a_iterador == 0) {

	const IdAno anoArg = a_periodo.getAno();
	const IdMes mesArg = a_periodo.getMes();
	const IdDia diaArg = a_periodo.getDia();
	const IdHor horArg = a_periodo.getHora();
	const IdMin minArg = a_periodo.getMinuto();

	a_anoIter = IdAno(anoArg + a_iterador);
	a_mesIter = mesArg;
	a_diaIter = diaArg;
	if ((mesArg == IdMes_2) && (diaArg == IdDia_29) && (Periodo::anoBissexto(a_anoIter)))
		a_diaIter = IdDia_29;
	else if ((mesArg == IdMes_2) && (diaArg == IdDia_29))
		a_diaIter = IdDia_28;

	a_horaIter = horArg;
	a_minutoIter = minArg;

} // void Periodo::iteraAno(const Periodo & a_periodo, const int a_iterador, IdAno & a_anoIter, IdMes & a_mesIter, IdDia & a_diaIter, IdHor & a_horaIter, IdMin & a_minutoIter) {


void Periodo::iteraMes(const Periodo& a_periodo, const int a_iterador, IdAno& a_anoIter, IdMes& a_mesIter, IdDia& a_diaIter, IdHor& a_horaIter, IdMin& a_minutoIter) {

	if (a_iterador == 0) {
		a_anoIter = a_periodo.getAno();
		a_mesIter = a_periodo.getMes();
		a_diaIter = a_periodo.getDia();
		a_horaIter = a_periodo.getHora();
		a_minutoIter = a_periodo.getMinuto();
		return;
	} // if (a_iterador == 0) {

	const IdAno anoArg = a_periodo.getAno();
	const IdMes mesArg = a_periodo.getMes();
	const IdDia diaArg = a_periodo.getDia();
	const IdHor horArg = a_periodo.getHora();
	const IdMin minArg = a_periodo.getMinuto();

	int mesIter = int(mesArg + a_iterador) % 12;
	int anoIter = 1900 + anoArg - 1 + int(int(mesArg + a_iterador) / 12);

	if (mesIter <= 0) {
		mesIter += 12;
		anoIter -= 1;
	} // if (mesIter <= 0) {

	a_anoIter = IdAno(anoIter - 1900 + 1);
	a_mesIter = IdMes(mesIter - 1 + 1);

	a_diaIter = diaArg;
	if (a_mesIter == IdMes_2) {
		if ((a_diaIter > IdDia_28) && (Periodo::anoBissexto(a_anoIter)))
			a_diaIter = IdDia_29;
		else if (a_diaIter > IdDia_28)
			a_diaIter = IdDia_28;
	} // if (a_mesIter == IdMes_2) {

	else if ((a_diaIter == IdDia_31) && ((a_mesIter == IdMes_4) || (a_mesIter == IdMes_6) || (a_mesIter == IdMes_9) || (a_mesIter == IdMes_11)))
		a_diaIter = IdDia_30;

	a_horaIter = horArg;
	a_minutoIter = minArg;

} // void Periodo::iteraMes(Periodo & a_periodo, int & a_iterador, IdAno & a_anoIter, IdMes & a_mesIter, IdDia & a_diaIter, IdHor & a_horaIter, IdMin & a_minutoIter){


void Periodo::iteraDia(const Periodo& a_periodo, const int a_iterador, IdAno& a_anoIter, IdMes& a_mesIter, IdDia& a_diaIter, IdHor& a_horaIter, IdMin& a_minutoIter) {

	if (a_iterador == 0) {
		a_anoIter = a_periodo.getAno();
		a_mesIter = a_periodo.getMes();
		a_diaIter = a_periodo.getDia();
		a_horaIter = a_periodo.getHora();
		a_minutoIter = a_periodo.getMinuto();
		return;
	} // if (a_iterador == 0) {

	const IdAno anoArg = a_periodo.getAno();
	const IdMes mesArg = a_periodo.getMes();
	const IdDia diaArg = a_periodo.getDia();
	const IdHor horArg = a_periodo.getHora();
	const IdMin minArg = a_periodo.getMinuto();

	a_anoIter = anoArg;
	a_mesIter = mesArg;
	a_diaIter = diaArg;

	if (a_iterador > 0) {
		for (int i = 1; i <= a_iterador; i++) {
			if (a_mesIter == IdMes_2) {
				if ((a_diaIter == IdDia_28) && (!Periodo::anoBissexto(a_anoIter))) { a_diaIter = IdDia_1; a_mesIter++; }
				else if ((a_diaIter == IdDia_29) && (Periodo::anoBissexto(a_anoIter))) { a_diaIter = IdDia_1; a_mesIter++; }
				else a_diaIter++;
			} // if (a_mesIter == IdMes_2) {

			else if ((a_diaIter == IdDia_30) && ((a_mesIter == IdMes_4) || (a_mesIter == IdMes_6) || (a_mesIter == IdMes_9) || (a_mesIter == IdMes_11))) { a_diaIter = IdDia_1; a_mesIter++; }

			else if ((a_diaIter == IdDia_31) && ((a_mesIter == IdMes_1) || (a_mesIter == IdMes_3) || (a_mesIter == IdMes_5) || (a_mesIter == IdMes_7) || (a_mesIter == IdMes_8) || (a_mesIter == IdMes_10) || (a_mesIter == IdMes_12))) {
				a_diaIter = IdDia_1;
				if (a_mesIter == IdMes_12) { a_mesIter = IdMes_1; a_anoIter++; }
				else a_mesIter++;
			} // else if ((a_diaIter == IdDia_31) && ((a_mesIter == IdMes_1) || (a_mesIter == IdMes_3) || (a_mesIter == IdMes_5) || (a_mesIter == IdMes_7) || (a_mesIter == IdMes_8) || (a_mesIter == IdMes_10) || (a_mesIter == IdMes_12))) {

			else a_diaIter++;

		} // for (int i = 1; i <= a_iterador; i++) {
	} // if (a_iterador > 0) {

	else if (a_iterador < 0) {
		for (int i = a_iterador; i <= -1; i++) {
			if (a_mesIter == IdMes_3) {
				if ((a_diaIter == IdDia_1) && (!Periodo::anoBissexto(a_anoIter))) { a_diaIter = IdDia_28; a_mesIter--; }
				else if ((a_diaIter == IdDia_1) && (Periodo::anoBissexto(a_anoIter))) { a_diaIter = IdDia_29; a_mesIter--; }
				else a_diaIter--;
			} // if (a_mesIter == IdMes_2) {

			else if ((a_diaIter == IdDia_1) && ((a_mesIter == IdMes_1) || (a_mesIter == IdMes_2) || (a_mesIter == IdMes_4) || (a_mesIter == IdMes_6) || (a_mesIter == IdMes_8) || (a_mesIter == IdMes_9) || (a_mesIter == IdMes_11))) {
				a_diaIter = IdDia_31;
				if (a_mesIter == IdMes_1) { a_mesIter = IdMes_12; a_anoIter--; }
				else a_mesIter--;
			} // else if ((a_diaIter == 1) && ((a_mesIter == 1) || (a_mesIter == 2) || (a_mesIter == 4) || (a_mesIter == 6) || (a_mesIter == 8) || (a_mesIter == 9) || (a_mesIter == 11))) {

			else if ((a_diaIter == IdDia_1) && ((a_mesIter == IdMes_5) || (a_mesIter == IdMes_7) || (a_mesIter == IdMes_10) || (a_mesIter == IdMes_12))) { a_diaIter = IdDia_30; a_mesIter--; }

			else a_diaIter--;

		} // for (int i = a_iterador; i <= -1; i++) {
	} // else if (a_iterador < 0) {

	a_horaIter = horArg;
	a_minutoIter = minArg;

} // void Periodo::iteraDia(const Periodo & a_periodo, const int a_iterador, IdAno & a_anoIter, IdMes & a_mesIter, IdDia & a_diaIter, IdHor & a_horaIter, IdMin & a_minutoIter){



void Periodo::iteraHora(const Periodo& a_periodo, const int a_iterador, IdAno& a_anoIter, IdMes& a_mesIter, IdDia& a_diaIter, IdHor& a_horaIter, IdMin& a_minutoIter) {

	if (a_iterador == 0) {
		a_anoIter = a_periodo.getAno();
		a_mesIter = a_periodo.getMes();
		a_diaIter = a_periodo.getDia();
		a_horaIter = a_periodo.getHora();
		a_minutoIter = a_periodo.getMinuto();
		return;
	} // if (a_iterador == 0) {

	const IdAno anoArg = a_periodo.getAno();
	const IdMes mesArg = a_periodo.getMes();
	const IdDia diaArg = a_periodo.getDia();
	const IdHor horArg = a_periodo.getHora();
	const IdMin minArg = a_periodo.getMinuto();

	a_anoIter = anoArg;
	a_mesIter = mesArg;
	a_diaIter = diaArg;
	a_minutoIter = minArg;

	int horaIter = int(horArg + a_iterador - 1) % 24;
	int iteradorDia = int(int(horArg + a_iterador - 1) / 24);

	if (horaIter < 0) {
		horaIter += 24;
		iteradorDia--;
	} // if (a_horaIter < 0) {

	a_horaIter = IdHor(horaIter + 1);

	if (iteradorDia != 0)
		Periodo::iteraDia(Periodo(getString(a_periodo.getDuration()), a_diaIter, a_mesIter, a_anoIter, a_horaIter, a_minutoIter), iteradorDia, a_anoIter, a_mesIter, a_diaIter, a_horaIter, a_minutoIter);

} // void Periodo::iteraHora(const Periodo & a_periodo, const int a_iterador, IdAno & a_anoIter, IdMes & a_mesIter, IdDia & a_diaIter, IdHor & a_horaIter, IdMin & a_minutoIter){



void Periodo::iteraMinuto(const Periodo& a_periodo, const int a_iterador, IdAno& a_anoIter, IdMes& a_mesIter, IdDia& a_diaIter, IdHor& a_horaIter, IdMin& a_minutoIter) {

	if (a_iterador == 0) {
		a_anoIter = a_periodo.getAno();
		a_mesIter = a_periodo.getMes();
		a_diaIter = a_periodo.getDia();
		a_horaIter = a_periodo.getHora();
		a_minutoIter = a_periodo.getMinuto();
		return;
	} // if (a_iterador == 0) {

	const IdAno anoArg = a_periodo.getAno();
	const IdMes mesArg = a_periodo.getMes();
	const IdDia diaArg = a_periodo.getDia();
	const IdHor horArg = a_periodo.getHora();
	const IdMin minArg = a_periodo.getMinuto();

	a_anoIter = anoArg;
	a_mesIter = mesArg;
	a_diaIter = diaArg;
	a_horaIter = horArg;

	int minutoIter = int(minArg - 1 + a_iterador) % 60;
	int iteradorHora = int(int(minArg - 1 + a_iterador) / 60);

	if (minutoIter < 0) {
		minutoIter += 60;
		iteradorHora--;
	} // if (minutoIter < 0) {

	a_minutoIter = IdMin(minutoIter + 1);

	if (iteradorHora != 0)
		Periodo::iteraHora(Periodo(getString(a_periodo.getDuration()), a_diaIter, a_mesIter, a_anoIter, a_horaIter, a_minutoIter), iteradorHora, a_anoIter, a_mesIter, a_diaIter, a_horaIter, a_minutoIter);

} // void Periodo::iteraMinuto(const Periodo & a_periodo, const int a_iterador, IdAno & a_anoIter, IdMes & a_mesIter, IdDia & a_diaIter, IdHor & a_horaIter, IdMin & a_minutoIter){

IdDia Periodo::getMaiorDiaDoMes(IdMes a_idMes) const {
	if (a_idMes != IdMes_Nenhum) {

		IdDia maiorDia = IdDia_Nenhum;

		if (a_idMes == IdMes_4 || a_idMes == IdMes_6 || a_idMes == IdMes_9 || a_idMes == IdMes_11)
			maiorDia = IdDia_30;

		else if (a_idMes == IdMes_2) {

			if (anoBissexto())
				maiorDia = IdDia_29;
			else
				maiorDia = IdDia_28;
		} // else if (a_idMes == IdMes_2) {

		else
			maiorDia = IdDia_31;

		return maiorDia;

	} // if (mes != IdMes_Nenhum) {
	else
		throw std::invalid_argument("Nao existe maiorDiaDoMes para IdMes_Nenhum ");

}

Periodo Periodo::getPeriodoInicial() {

	return Periodo(1, 1, ano_inicial, 0, 0);

} // Periodo Periodo::getPeriodoInicial(){


Periodo Periodo::getPeriodoFinal() {

	return Periodo(31, 12, ano_final, 23, 59);

} // Periodo Periodo::getPeriodoFinal(){


std::pair<unsigned int, char> Periodo::getDurationFromStr(const std::string &a_str){

	std::pair<unsigned int, char> duration_(0, '0');

	if (a_str.size() == 0)
		return duration_;

	const char durT = a_str.at(a_str.size() - 1);

	if (isValidDurationT(durT)) {

		if (a_str.size() == 1) {
			duration_.first = 1U;
			duration_.second = durT;
			return duration_;
		}

		// Teste para retrocompatibilizacao com antigos periodos
		if (a_str.at(a_str.size() - 2) != 'r') {

			bool allZero = true;
			const std::string durV = a_str.substr(0, a_str.size() - 1);
			for (size_t i = 0; i < durV.size(); i++) {
				if (!isCharNonZeroNumber(durV[i])) {
					if (durV[i] != '0')
						return duration_;
				}
				else
					allZero = false;
			}

			if (allZero)
				return duration_;

			duration_.first = std::atoi(durV.c_str());
			duration_.second = durT;

			if ((durT == 'M') || (durT == 'a'))
				return duration_;

			if ((durT == 'm') && (duration_.first >= 60U)) {
				if (duration_.first >= 1440U) {
					if (duration_.first % 1440U == 0) {
						duration_.first /= 1440U;
						duration_.second = 'd';
						return duration_;
					}
				}

				if (duration_.first % 60U == 0) {
					duration_.first /= 60U;
					duration_.second = 'h';
					return duration_;
				}
			} // if ((durT == 'm') && (duration_.first >= 60U)) {

			if ((durT == 'h') && (duration_.first >= 24U)) {
				if (duration_.first % 24U == 0) {
					duration_.first /= 24U;
					duration_.second = 'h';
					return duration_;
				}
			}

			return duration_;
		}
	}

	size_t pos = a_str.find("minuto");
	if (pos != std::string::npos)
		return getDurationFromStr("m");

	if (a_str.at(a_str.size() - 1) == 'm')
		return getDurationFromStr(a_str);

	pos = a_str.find("meia_hora");
	if (pos != std::string::npos)
		return getDurationFromStr("30m");

	pos = a_str.find("horario");
	if (pos != std::string::npos)
		return getDurationFromStr("h");

	pos = a_str.find("horas");
	if (pos != std::string::npos)
		return getDurationFromStr(std::string(a_str.substr(0, pos) + "h"));

	pos = a_str.find("diario");
	if (pos != std::string::npos)
		return getDurationFromStr("d");

	pos = a_str.find("dias");
	if (pos != std::string::npos)
		return getDurationFromStr(std::string(a_str.substr(0, pos) + "d"));

	pos = a_str.find("semanal");
	if (pos != std::string::npos)
		return getDurationFromStr("7d");

	pos = a_str.find("mensal");
	if (pos != std::string::npos)
		return getDurationFromStr("M");

	pos = a_str.find("anual");
	if (pos != std::string::npos)
		return getDurationFromStr("a");

	return duration_;

}


bool Periodo::isValidDuration(const std::pair<unsigned int, char>& a_dur1){

	if (a_dur1.first == 0U)
		return false;

	if (!isValidDurationT(a_dur1.second))
		return false;

	return true;

}

bool Periodo::isValidDurationT(const char a_durT){

	if ((a_durT == 'm') || (a_durT == 'h') || (a_durT == 'd') || (a_durT == 'M') || (a_durT == 'a'))
		return true;

	return false;
}

bool Periodo::isSameDuration(const Periodo& a_per1, const Periodo& a_per2){

	try {

		if (!a_per1.isValido())
			throw std::invalid_argument("Invalid a_per1");

		if (!a_per2.isValido())
			throw std::invalid_argument("Invalid a_per2");

		const std::pair<unsigned int, char> dur1 = a_per1.getDuration();
		const std::pair<unsigned int, char> dur2 = a_per2.getDuration();

		if (dur1.second == dur2.second) {
			if (dur1.first == dur2.first)
				return true;
			else
				return false;
		}

		if (a_per1.getMinutos() == a_per2.getMinutos())
			return true;

		return false;

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::isSameDuration(" + getFullString(a_per1) + "," + getFullString(a_per2) + "): \n" + std::string(erro.what())); }

}

Periodo Periodo::getPeriodBtwn(const Periodo& a_per1, const Periodo& a_per2){

	try{

		Periodo perMaior = a_per2;
		Periodo perMenor = a_per1;

		if (a_per1 > a_per2) {
			perMaior = a_per1;
			perMenor = a_per2;
		}

		const Periodo perMenorNxt_min = Periodo("m", perMenor + 1);

		if (perMenorNxt_min >= perMaior)
			throw std::invalid_argument("There is no gap between periods.");

		const Periodo perMaiorPrv_min = Periodo("m", perMaior) - 1;

		const unsigned int numMin = perMaiorPrv_min - perMenorNxt_min + 1;

		return Periodo(getString(numMin) + "m", perMenorNxt_min);

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::getPeriodBtwn(" + getFullString(a_per1) + "," + getFullString(a_per2) + "): \n" + std::string(erro.what())); }

}

std::vector<std::string> Periodo::getDurT(){

	return std::vector<std::string>{"m", "h", "d", "M", "a"};
}



Periodo::~Periodo() {}


