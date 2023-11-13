#include "C_Periodo.h"

#include <sstream>

static const char* formatoPadrao = "DD/MM/AAAA-hh:mm:ss-TipoPeriodo";
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

	tipoPeriodo = TipoPeriodo_Nenhum;
	ano = IdAno_Nenhum;
	mes = IdMes_Nenhum;
	dia = IdDia_Nenhum;
	hora = IdHor_Nenhum;
	minuto = IdMin_Nenhum;

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
		if ((tipoPeriodo <= TipoPeriodo_Nenhum) || (tipoPeriodo >= TipoPeriodo_Excedente)) { throw std::invalid_argument("Periodo Invalido."); }

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




void Periodo::setPeriodo(const TipoPeriodo a_tipoPeriodo, const std::string a_periodo) {
	try {

		std::string periodoStr = a_periodo;

		std::vector<size_t> posSeparador;
		std::vector<char>   queSeparador;

		for (std::string::size_type pos = 0; pos < periodoStr.size(); ++pos) {
			for (int i = 0; i < numeroElementosArraySeparador; i++) {
				if (periodoStr.at(pos) == arraySeparador[i]) {
					posSeparador.push_back(pos);
					queSeparador.push_back(arraySeparador[i]);
					break;
				} // if (periodoStr.at(pos) == arraySeparador[i]) {
			} // for (int i = 0; i < numeroElementosArraySeparador; i++) {
		} // for (std::string::size_type pos = 0; pos < periodoStr.size(); ++pos) {

		TipoPeriodo tipoPeriodoLido = TipoPeriodo_Nenhum;

		if (posSeparador.size() == 0)
			tipoPeriodoLido = TipoPeriodo_anual;

		else if (posSeparador.size() == 1) {
			if (queSeparador.at(0) == '/')
				tipoPeriodoLido = TipoPeriodo_mensal;
			else
				throw std::invalid_argument("String com formato invalido de Periodo. Formato padrao completo " + std::string(formatoPadrao));
		} // if (posSeparador.size() == 1) {

		else if (posSeparador.size() == 2) {
			if ((queSeparador.at(0) == '/') && (queSeparador.at(1) == '/'))
				tipoPeriodoLido = TipoPeriodo_diario;
			else
				throw std::invalid_argument("String com formato invalido de Periodo. Formato padrao completo " + std::string(formatoPadrao));
		} // else if (posSeparador.size() == 2) {

		else if (posSeparador.size() == 3) {
			if ((queSeparador.at(0) == '/') && (queSeparador.at(1) == '/') && (queSeparador.at(2) == '-'))
				tipoPeriodoLido = TipoPeriodo_Excedente;
			else
				throw std::invalid_argument("String com formato invalido de Periodo. Formato padrao completo " + std::string(formatoPadrao));
		} // else if (posSeparador.size() == 3) {

		else if (posSeparador.size() == 4) {
			if ((queSeparador.at(0) == '/') && (queSeparador.at(1) == '/') && (queSeparador.at(2) == '-') && (queSeparador.at(3) == ':'))
				tipoPeriodoLido = TipoPeriodo_minuto;
			else
				throw std::invalid_argument("String com formato invalido de Periodo. Formato padrao completo " + std::string(formatoPadrao));
		} // else if (posSeparador.size() == 4) {

		else if (posSeparador.size() == 5) {
			if ((queSeparador.at(0) == '/') && (queSeparador.at(1) == '/') && (queSeparador.at(2) == '-') && (queSeparador.at(3) == ':') && (queSeparador.at(4) == ':'))
				tipoPeriodoLido = TipoPeriodo_minuto;
			else if ((queSeparador.at(0) == '/') && (queSeparador.at(1) == '/') && (queSeparador.at(2) == '-') && (queSeparador.at(3) == ':') && (queSeparador.at(4) == '-'))
				tipoPeriodoLido = TipoPeriodo_Excedente;
			else
				throw std::invalid_argument("String com formato invalido de Periodo. Formato padrao completo " + std::string(formatoPadrao));
		} // else if (posSeparador.size() == 5) {

		else if (posSeparador.size() == 6) {
			if ((queSeparador.at(0) == '/') && (queSeparador.at(1) == '/') && (queSeparador.at(2) == '-') && (queSeparador.at(3) == ':') && (queSeparador.at(4) == ':') && (queSeparador.at(5) == '-'))
				tipoPeriodoLido = TipoPeriodo_Excedente;
			else
				throw std::invalid_argument("String com formato invalido de Periodo. Formato padrao completo " + std::string(formatoPadrao));
		} // else if (posSeparador.size() == 6) {
		else
			throw std::invalid_argument("String com formato invalido de Periodo. Formato padrao completo " + std::string(formatoPadrao));

		TipoPeriodo tipoPeriodoAtribuido = tipoPeriodoLido;
		if ((a_tipoPeriodo < tipoPeriodoAtribuido) && (a_tipoPeriodo > TipoPeriodo_Nenhum))
			tipoPeriodoAtribuido = a_tipoPeriodo;
		else if (a_tipoPeriodo > tipoPeriodoAtribuido)
			throw std::invalid_argument("Tipo de Periodo nao compativel com o Periodo lido na String.");

		if (tipoPeriodoLido == TipoPeriodo_anual)
			setPeriodo(tipoPeriodoAtribuido, IdDia_1, IdMes_1, getIdAnoFromChar(periodoStr.c_str()), IdHor_0, IdMin_0);

		else if (tipoPeriodoLido == TipoPeriodo_mensal) 
			setPeriodo(tipoPeriodoAtribuido, IdDia_1, getIdMesFromChar(periodoStr.substr(0, posSeparador.at(0)).c_str()), getIdAnoFromChar(std::string(periodoStr.substr(posSeparador.at(0) + 1, std::string::npos)).c_str()), IdHor_0, IdMin_0);

		else if (tipoPeriodoLido == TipoPeriodo_diario) {
			const IdDia diaLido = getIdDiaFromChar(periodoStr.substr(0, posSeparador.at(0)).c_str());
			const IdMes mesLido = getIdMesFromChar(periodoStr.substr(posSeparador.at(0) + 1, posSeparador.at(1) - posSeparador.at(0) - 1).c_str());
			const IdAno anoLido = getIdAnoFromChar(std::string(periodoStr.substr(posSeparador.at(1) + 1, std::string::npos)).c_str());
			setPeriodo(tipoPeriodoAtribuido, diaLido, mesLido, anoLido, IdHor_0, IdMin_0);
		} // if (tipoPeriodoLido == TipoPeriodo_diario) {

		else if (tipoPeriodoLido == TipoPeriodo_horario) {
			const IdDia diaLido = getIdDiaFromChar(periodoStr.substr(0, posSeparador.at(0)).c_str());
			const IdMes mesLido = getIdMesFromChar(periodoStr.substr(posSeparador.at(0) + 1, posSeparador.at(1) - posSeparador.at(0) - 1).c_str());
			const IdAno anoLido = getIdAnoFromChar(periodoStr.substr(posSeparador.at(1) + 1, posSeparador.at(2) - posSeparador.at(1) - 1).c_str());
			const IdHor horLido = getIdHorFromChar(periodoStr.substr(posSeparador.at(2) + 1, std::string::npos).c_str());
			setPeriodo(tipoPeriodoAtribuido, diaLido, mesLido, anoLido, horLido, IdMin_0);
		} // else if (tipoPeriodoLido == TipoPeriodo_horario) {

		else if (tipoPeriodoLido == TipoPeriodo_minuto) {
			const IdDia diaLido = getIdDiaFromChar(periodoStr.substr(0, posSeparador.at(0)).c_str());
			const IdMes mesLido = getIdMesFromChar(periodoStr.substr(posSeparador.at(0) + 1, posSeparador.at(1) - posSeparador.at(0) - 1).c_str());
			const IdAno anoLido = getIdAnoFromChar(periodoStr.substr(posSeparador.at(1) + 1, posSeparador.at(2) - posSeparador.at(1) - 1).c_str());
			const IdHor horLido = getIdHorFromChar(periodoStr.substr(posSeparador.at(2) + 1, posSeparador.at(3) - posSeparador.at(2) - 1).c_str());
			IdMin minLido = IdMin_Nenhum;
			if (posSeparador.size() == 4)
				minLido = getIdMinFromChar(periodoStr.substr(posSeparador.at(3) + 1, std::string::npos).c_str());
			else
				minLido = getIdMinFromChar(periodoStr.substr(posSeparador.at(3) + 1, posSeparador.at(4) - posSeparador.at(3) - 1).c_str());
			setPeriodo(tipoPeriodoAtribuido, diaLido, mesLido, anoLido, horLido, minLido);
		} // else if (tipoPeriodoLido == TipoPeriodo_minuto) {

		else if (tipoPeriodoLido == TipoPeriodo_Excedente) {

			const IdDia diaLido = getIdDiaFromChar(periodoStr.substr(0, posSeparador.at(0)).c_str());
			const IdMes mesLido = getIdMesFromChar(periodoStr.substr(posSeparador.at(0) + 1, posSeparador.at(1) - posSeparador.at(0) - 1).c_str());
			const IdAno anoLido = getIdAnoFromChar(periodoStr.substr(posSeparador.at(1) + 1, posSeparador.at(2) - posSeparador.at(1) - 1).c_str());

			IdHor horLido = IdHor_0;
			IdMin minLido = IdMin_0;

			if (posSeparador.size() > 3) {
				horLido = getIdHorFromChar(periodoStr.substr(posSeparador.at(2) + 1, posSeparador.at(3) - posSeparador.at(2) - 1).c_str());
				minLido = getIdMinFromChar(periodoStr.substr(posSeparador.at(3) + 1, posSeparador.at(4) - posSeparador.at(3) - 1).c_str());
			} // if (posSeparador.size() > 3) {

			if (posSeparador.size() == 3)
				tipoPeriodoAtribuido = getTipoPeriodoFromChar(periodoStr.substr(posSeparador.at(2) + 1, std::string::npos).c_str());
			else if (posSeparador.size() == 5)
				tipoPeriodoAtribuido = getTipoPeriodoFromChar(periodoStr.substr(posSeparador.at(4) + 1, std::string::npos).c_str());
			else
				tipoPeriodoAtribuido = getTipoPeriodoFromChar(periodoStr.substr(posSeparador.at(5) + 1, std::string::npos).c_str());

			setPeriodo(tipoPeriodoAtribuido, diaLido, mesLido, anoLido, horLido, minLido);

		} // else if (tipoPeriodoLido == TipoPeriodo_Excedente) {

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::setPeriodo(" + a_periodo + "): \n" + std::string(erro.what())); }
} // void Periodo::setPeriodo(const TipoPeriodo a_tipoPeriodo, const std::string a_periodo){





void Periodo::setPeriodo(const TipoPeriodo a_tipoPeriodo, const IdAno a_ano) {
	try { setPeriodo(a_tipoPeriodo, IdDia_1, IdMes_1, a_ano, IdHor_0, IdMin_0); } // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::setPeriodo(" + getString(a_tipoPeriodo) + "," + getString(a_ano) + "): \n" + std::string(erro.what())); }
} // void Periodo::setPeriodo(const TipoPeriodo a_tipoPeriodo, const IdAno a_ano) {


void Periodo::setPeriodo(const TipoPeriodo a_tipoPeriodo, const IdMes a_mes, const IdAno a_ano) {
	try { setPeriodo(a_tipoPeriodo, IdDia_1, a_mes, a_ano, IdHor_0, IdMin_0); } // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::setPeriodo(" + getString(a_tipoPeriodo) + "," + getString(a_mes) + "," + getString(a_ano) + "): \n" + std::string(erro.what())); }
} // void Periodo::setPeriodo(const TipoPeriodo a_tipoPeriodo, const int IdMes, const IdAno a_ano){


void Periodo::setPeriodo(const TipoPeriodo a_tipoPeriodo, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano) {
	try { setPeriodo(a_tipoPeriodo, a_dia, a_mes, a_ano, IdHor_0, IdMin_0); } // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::setPeriodo(" + getString(a_tipoPeriodo) + "," + getString(a_dia) + "," + getString(a_mes) + "," + getString(a_ano) + "): \n" + std::string(erro.what())); }
} // void Periodo::setPeriodo(const TipoPeriodo a_tipoPeriodo, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano){


void Periodo::setPeriodo(const TipoPeriodo a_tipoPeriodo, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora) {
	try { setPeriodo(a_tipoPeriodo, a_dia, a_mes, a_ano, a_hora, IdMin_0); } // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::setPeriodo(" + getString(a_tipoPeriodo) + "," + getString(a_dia) + "," + getString(a_mes) + "," + getString(a_ano) + "," + getString(a_hora) + "): \n" + std::string(erro.what())); }
} // void Periodo::setPeriodo(const TipoPeriodo a_tipoPeriodo, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora){


void Periodo::setPeriodo(const TipoPeriodo a_tipoPeriodo, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora, const IdMin a_minuto) {
	try {
		if ((a_tipoPeriodo <= TipoPeriodo_Nenhum) || (a_tipoPeriodo > TipoPeriodo_minuto)) { throw std::invalid_argument("Tipo de Periodo Invalido."); }
		validaAno(a_ano);
		validaMes(a_mes);
		validaDia(a_dia);
		validaHora(a_hora);
		validaMinuto(a_minuto);
		tipoPeriodo = a_tipoPeriodo;
	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::setPeriodo(" + getString(a_tipoPeriodo) + "," + getString(a_dia) + "," + getString(a_mes) + "," + getString(a_ano) + "," + getString(a_hora) + "," + getString(a_minuto) + "): \n" + std::string(erro.what())); }
} // void Periodo::setPeriodo(const TipoPeriodo a_tipoPeriodo, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora, const IdMin a_minuto){


Periodo::Periodo() { inicializaAtributos(); }

Periodo::Periodo(const TipoPeriodo a_tipoPeriodo, const Periodo& a_periodo) {
	try {
		if (!a_periodo.isValido())
			throw std::invalid_argument("Periodo Invalido.");
		inicializaAtributos();
		setPeriodo(a_tipoPeriodo, a_periodo.getDia(), a_periodo.getMes(), a_periodo.getAno(), a_periodo.getHora(), a_periodo.getMinuto());
	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::Periodo(" + getFullString(a_tipoPeriodo) + "," + a_periodo.str() + ") \n" + std::string(erro.what())); }
}

Periodo::Periodo(const std::string a_periodo) {
	inicializaAtributos();
	setPeriodo(TipoPeriodo_Nenhum, a_periodo);
}

Periodo::Periodo(const TipoPeriodo a_tipoPeriodo, const std::string a_periodo) {
	if ((a_tipoPeriodo <= TipoPeriodo_Nenhum) || (a_tipoPeriodo >= TipoPeriodo_Excedente))
		throw std::invalid_argument("Periodo::Periodo(" + getString(a_tipoPeriodo) + "," + a_periodo + "): \nTipo de Periodo Invalido.");

	inicializaAtributos();
	setPeriodo(a_tipoPeriodo, a_periodo);
} // Periodo::Periodo(const TipoPeriodo a_tipoPeriodo, const std::string a_periodo) {


Periodo::Periodo(const int   a_ano) {
	std::stringstream anoStr;
	anoStr << a_ano;
	inicializaAtributos();
	setPeriodo(TipoPeriodo_anual, getIdAnoFromChar(anoStr.str().c_str()));
} // Periodo::Periodo(const int   a_ano) {

Periodo::Periodo(const IdAno a_ano) { inicializaAtributos(); setPeriodo(TipoPeriodo_anual, a_ano); }

Periodo::Periodo(const TipoPeriodo a_tipoPeriodo, const int a_ano) {
	std::stringstream anoStr;
	anoStr << a_ano;
	inicializaAtributos();
	setPeriodo(a_tipoPeriodo, getIdAnoFromChar(anoStr.str().c_str()));
} // Periodo::Periodo(const TipoPeriodo a_tipoPeriodo, const int   a_ano) {

Periodo::Periodo(const TipoPeriodo a_tipoPeriodo, const IdAno a_ano) { inicializaAtributos(); setPeriodo(a_tipoPeriodo, a_ano); }

Periodo::Periodo(const int a_mes, const int a_ano) {
	std::stringstream anoStr, mesStr;
	anoStr << a_ano;
	mesStr << a_mes;
	inicializaAtributos();
	setPeriodo(TipoPeriodo_mensal, getIdMesFromChar(mesStr.str().c_str()), getIdAnoFromChar(anoStr.str().c_str()));
} // Periodo::Periodo(const int   a_mes, const int   a_ano) {

Periodo::Periodo(const IdMes a_mes, const IdAno a_ano) { inicializaAtributos(); setPeriodo(TipoPeriodo_mensal, a_mes, a_ano); }

Periodo::Periodo(const TipoPeriodo a_tipoPeriodo, const int   a_mes, const int   a_ano) {
	std::stringstream anoStr, mesStr;
	anoStr << a_ano;
	mesStr << a_mes;
	inicializaAtributos();
	setPeriodo(a_tipoPeriodo, getIdMesFromChar(mesStr.str().c_str()), getIdAnoFromChar(anoStr.str().c_str()));
} // Periodo::Periodo(const TipoPeriodo a_tipoPeriodo, const int   a_mes, const int   a_ano) {

Periodo::Periodo(const TipoPeriodo a_tipoPeriodo, const IdMes a_mes, const IdAno a_ano) { inicializaAtributos(); setPeriodo(a_tipoPeriodo, a_mes, a_ano); }

Periodo::Periodo(const int   a_dia, const int   a_mes, const int   a_ano) {
	std::stringstream anoStr, mesStr, diaStr;
	anoStr << a_ano;
	mesStr << a_mes;
	diaStr << a_dia;
	inicializaAtributos();
	setPeriodo(TipoPeriodo_diario, getIdDiaFromChar(diaStr.str().c_str()), getIdMesFromChar(mesStr.str().c_str()), getIdAnoFromChar(anoStr.str().c_str()));
} // Periodo::Periodo(const int   a_dia, const int   a_mes, const int   a_ano) { 

Periodo::Periodo(const IdDia a_dia, const IdMes a_mes, const IdAno a_ano) { inicializaAtributos(); setPeriodo(TipoPeriodo_diario, a_dia, a_mes, a_ano); }

Periodo::Periodo(const TipoPeriodo a_tipoPeriodo, const int   a_dia, const int   a_mes, const int   a_ano) {
	std::stringstream anoStr, mesStr, diaStr;
	anoStr << a_ano;
	mesStr << a_mes;
	diaStr << a_dia;
	inicializaAtributos();
	setPeriodo(a_tipoPeriodo, getIdDiaFromChar(diaStr.str().c_str()), getIdMesFromChar(mesStr.str().c_str()), getIdAnoFromChar(anoStr.str().c_str()));
} // Periodo::Periodo(const TipoPeriodo a_tipoPeriodo, const int   a_dia, const int   a_mes, const int   a_ano) {

Periodo::Periodo(const TipoPeriodo a_tipoPeriodo, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano) { inicializaAtributos(); setPeriodo(a_tipoPeriodo, a_dia, a_mes, a_ano); }

Periodo::Periodo(const int   a_dia, const int   a_mes, const int   a_ano, const int   a_hora) {
	std::stringstream anoStr, mesStr, diaStr, horStr;
	anoStr << a_ano;
	mesStr << a_mes;
	diaStr << a_dia;
	horStr << a_hora;
	inicializaAtributos();
	setPeriodo(TipoPeriodo_horario, getIdDiaFromChar(diaStr.str().c_str()), getIdMesFromChar(mesStr.str().c_str()), getIdAnoFromChar(anoStr.str().c_str()), getIdHorFromChar(horStr.str().c_str()));
} // Periodo::Periodo(const int   a_dia, const int   a_mes, const int   a_ano, const int   a_hora) {

Periodo::Periodo(const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora) { inicializaAtributos(); setPeriodo(TipoPeriodo_horario, a_dia, a_mes, a_ano, a_hora); }

Periodo::Periodo(const TipoPeriodo a_tipoPeriodo, const int   a_dia, const int   a_mes, const int   a_ano, const int   a_hora) {
	std::stringstream anoStr, mesStr, diaStr, horStr;
	anoStr << a_ano;
	mesStr << a_mes;
	diaStr << a_dia;
	horStr << a_hora;
	inicializaAtributos();
	setPeriodo(a_tipoPeriodo, getIdDiaFromChar(diaStr.str().c_str()), getIdMesFromChar(mesStr.str().c_str()), getIdAnoFromChar(anoStr.str().c_str()), getIdHorFromChar(horStr.str().c_str()));
} // Periodo::Periodo(const TipoPeriodo a_tipoPeriodo, const int   a_dia, const int   a_mes, const int   a_ano, const int   a_hora) {

Periodo::Periodo(const TipoPeriodo a_tipoPeriodo, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora) { inicializaAtributos(); setPeriodo(a_tipoPeriodo, a_dia, a_mes, a_ano, a_hora); }

Periodo::Periodo(const int   a_dia, const int   a_mes, const int   a_ano, const int a_hora, const int a_minuto) {
	std::stringstream anoStr, mesStr, diaStr, horStr, minStr;
	anoStr << a_ano;
	mesStr << a_mes;
	diaStr << a_dia;
	horStr << a_hora;
	minStr << a_minuto;
	inicializaAtributos();
	setPeriodo(TipoPeriodo_minuto, getIdDiaFromChar(diaStr.str().c_str()), getIdMesFromChar(mesStr.str().c_str()), getIdAnoFromChar(anoStr.str().c_str()), getIdHorFromChar(horStr.str().c_str()), getIdMinFromChar(minStr.str().c_str()));
} // Periodo::Periodo(const int   a_dia, const int   a_mes, const int   a_ano, const int   a_hora, const int   a_minuto) {

Periodo::Periodo(const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora, const IdMin a_minuto) { inicializaAtributos(); setPeriodo(TipoPeriodo_minuto, a_dia, a_mes, a_ano, a_hora, a_minuto); }

Periodo::Periodo(const TipoPeriodo a_tipoPeriodo, const int   a_dia, const int   a_mes, const int   a_ano, const int   a_hora, const int   a_minuto) {
	std::stringstream anoStr, mesStr, diaStr, horStr, minStr;
	anoStr << a_ano;
	mesStr << a_mes;
	diaStr << a_dia;
	horStr << a_hora;
	minStr << a_minuto;
	inicializaAtributos();
	setPeriodo(a_tipoPeriodo, getIdDiaFromChar(diaStr.str().c_str()), getIdMesFromChar(mesStr.str().c_str()), getIdAnoFromChar(anoStr.str().c_str()), getIdHorFromChar(horStr.str().c_str()), getIdMinFromChar(minStr.str().c_str()));
} // Periodo::Periodo(const TipoPeriodo a_tipoPeriodo, const int   a_dia, const int   a_mes, const int   a_ano, const int   a_hora, const int   a_minuto) {

Periodo::Periodo(const TipoPeriodo a_tipoPeriodo, const IdDia a_dia, const IdMes a_mes, const IdAno a_ano, const IdHor a_hora, const IdMin a_minuto) { inicializaAtributos(); setPeriodo(a_tipoPeriodo, a_dia, a_mes, a_ano, a_hora, a_minuto); }


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

				periodo_ini = Periodo(TipoPeriodo(TipoPeriodo_Excedente - 1), periodo_base);
				periodo_fim = Periodo(TipoPeriodo(TipoPeriodo_Excedente - 1), periodo_overlap_seguinte);

			} // if ((periodo_base > a_periodo_overlap) && (periodo_base_seguinte > a_periodo_overlap)) {	 

			else if ((periodo_base <= a_periodo_overlap) && (periodo_base_seguinte <= periodo_overlap_seguinte)) {

				periodo_ini = Periodo(TipoPeriodo(TipoPeriodo_Excedente - 1), a_periodo_overlap);
				periodo_fim = Periodo(TipoPeriodo(TipoPeriodo_Excedente - 1), periodo_base_seguinte);

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

double Periodo::sobreposicao(const Periodo a_periodo_overlap_1, const Periodo a_periodo_overlap_2)const {

	try {

		const Periodo periodo_overlap_seguinte_1 = a_periodo_overlap_1 + 1;
		const Periodo periodo_overlap_seguinte_2 = a_periodo_overlap_2 + 1;

		const Periodo periodo_base = *this;

		const Periodo periodo_base_seguinte = periodo_base + 1;

		if ((periodo_overlap_seguinte_1 <= periodo_base) || (a_periodo_overlap_1 >= periodo_base_seguinte) || (periodo_overlap_seguinte_2 <= periodo_base) || (a_periodo_overlap_2 >= periodo_base_seguinte))
			return 0.0;

		else if (periodo_base == a_periodo_overlap_1 && periodo_base == a_periodo_overlap_2)
			return 1.0;

		else {

			//Determina o periodo_ini e periodo_fim dependendo da superposição do a_periodo_overlap_1 e a_periodo_overlap_2

			Periodo periodo_ini;
			Periodo periodo_fim;

			if (a_periodo_overlap_1 >= a_periodo_overlap_2) {

				periodo_ini = Periodo(TipoPeriodo(TipoPeriodo_Excedente - 1), a_periodo_overlap_1);
				periodo_fim = Periodo(TipoPeriodo(TipoPeriodo_Excedente - 1), a_periodo_overlap_2 + 1);

			}//if (a_periodo_overlap_1 >= a_periodo_overlap_2) {
			else {

				periodo_ini = Periodo(TipoPeriodo(TipoPeriodo_Excedente - 1), a_periodo_overlap_2);
				periodo_fim = Periodo(TipoPeriodo(TipoPeriodo_Excedente - 1), a_periodo_overlap_1 + 1);

			}//else {

			int minutos_sobreposicao = 0;

			for (Periodo periodo = periodo_ini; periodo < periodo_fim; periodo++) {
				if ((periodo >= periodo_base) && (periodo < periodo_base_seguinte))
					minutos_sobreposicao += periodo.getMinutos();
			} // for (Periodo periodo = periodo_ini; periodo < periodo_fim; periodo++) {

			return double(minutos_sobreposicao) / double(periodo_base.getMinutos());

		} // else {	 

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::sobreposicao(" + a_periodo_overlap_1.str() + a_periodo_overlap_2.str() + ") \n" + std::string(erro.what())); }

} // double Periodo::sobreposicao(const Periodo a_periodo_overlap){


double Periodo::atraso(const Periodo a_periodo) const {

	try {

		const Periodo periodo_base = *this;

		if (periodo_base >= a_periodo)
			return 0.0;

		const Periodo periodo_ini = Periodo(TipoPeriodo(TipoPeriodo_Excedente - 1), periodo_base);
		const Periodo periodo_fim = Periodo(TipoPeriodo(TipoPeriodo_Excedente - 1), a_periodo);

		int minutos_atraso = 0;

		for (Periodo periodo = periodo_ini; periodo < periodo_fim; periodo++)
			minutos_atraso += periodo.getMinutos();

		return double(minutos_atraso) / double(periodo_base.getMinutos());

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::sobreposicao(" + getString(a_periodo) + ") \n" + std::string(erro.what())); }

} // double Periodo::atraso(const Periodo a_periodo_atraso) const {



bool Periodo::sobreposicaoExcedente(const Periodo a_periodo_overlap)const {

	try {

		const Periodo periodo_overlap_seguinte = a_periodo_overlap + 1;

		const Periodo periodo_base = *this;

		const Periodo periodo_base_seguinte = periodo_base + 1;

		if ((a_periodo_overlap < periodo_base) && (periodo_overlap_seguinte >= periodo_base_seguinte))
			return true;

		if ((a_periodo_overlap <= periodo_base) && (periodo_overlap_seguinte > periodo_base_seguinte))
			return true;

		return false;

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::sobreposicao(" + a_periodo_overlap.str() + ") \n" + std::string(erro.what())); }

} // bool Periodo::sobreposicaoExcedente(const Periodo a_periodo_overlap){



int Periodo::getMeses() const {
	if (tipoPeriodo == TipoPeriodo_anual)
		return 12;
	else if (tipoPeriodo == TipoPeriodo_mensal)
		return 1;
	else
		return 0;
} // int Periodo::getMeses() const{


int Periodo::getDias() const {

	if (tipoPeriodo == TipoPeriodo_anual) {
		if (anoBissexto() && mes <= IdMes_2)
			return 366;
		else
			return 365;
	} // if (tipoPeriodo == TipoPeriodo_anual) {

	else if (tipoPeriodo == TipoPeriodo_mensal)
		return int(getMaiorDiaDoMes());

	else if (tipoPeriodo == TipoPeriodo_semanal)
		return 7;

	else if (tipoPeriodo == TipoPeriodo_60dias)
		return 60;

	else if (tipoPeriodo == TipoPeriodo_59dias)
		return 59;

	else if (tipoPeriodo == TipoPeriodo_58dias)
		return 58;

	else if (tipoPeriodo == TipoPeriodo_57dias)
		return 57;

	else if (tipoPeriodo == TipoPeriodo_56dias)
		return 56;

	else if (tipoPeriodo == TipoPeriodo_55dias)
		return 55;

	else if (tipoPeriodo == TipoPeriodo_54dias)
		return 54;

	else if (tipoPeriodo == TipoPeriodo_53dias)
		return 53;

	else if (tipoPeriodo == TipoPeriodo_52dias)
		return 52;

	else if (tipoPeriodo == TipoPeriodo_51dias)
		return 51;

	else if (tipoPeriodo == TipoPeriodo_50dias)
		return 50;

	else if (tipoPeriodo == TipoPeriodo_49dias)
		return 49;

	else if (tipoPeriodo == TipoPeriodo_48dias)
		return 48;

	else if (tipoPeriodo == TipoPeriodo_47dias)
		return 47;

	else if (tipoPeriodo == TipoPeriodo_46dias)
		return 46;

	else if (tipoPeriodo == TipoPeriodo_45dias)
		return 45;

	else if (tipoPeriodo == TipoPeriodo_44dias)
		return 44;

	else if (tipoPeriodo == TipoPeriodo_43dias)
		return 43;

	else if (tipoPeriodo == TipoPeriodo_42dias)
		return 42;

	else if (tipoPeriodo == TipoPeriodo_41dias)
		return 41;

	else if (tipoPeriodo == TipoPeriodo_40dias)
		return 40;

	else if (tipoPeriodo == TipoPeriodo_39dias)
		return 39;

	else if (tipoPeriodo == TipoPeriodo_38dias)
		return 38;

	else if (tipoPeriodo == TipoPeriodo_37dias)
		return 37;

	else if (tipoPeriodo == TipoPeriodo_36dias)
		return 36;

	else if (tipoPeriodo == TipoPeriodo_35dias)
		return 35;

	else if (tipoPeriodo == TipoPeriodo_34dias)
		return 34;

	else if (tipoPeriodo == TipoPeriodo_33dias)
		return 33;

	else if (tipoPeriodo == TipoPeriodo_32dias)
		return 32;

	else if (tipoPeriodo == TipoPeriodo_31dias)
		return 31;

	else if (tipoPeriodo == TipoPeriodo_30dias)
		return 30;

	else if (tipoPeriodo == TipoPeriodo_29dias)
		return 29;

	else if (tipoPeriodo == TipoPeriodo_28dias)
		return 28;

	else if (tipoPeriodo == TipoPeriodo_27dias)
		return 27;

	else if (tipoPeriodo == TipoPeriodo_26dias)
		return 26;

	else if (tipoPeriodo == TipoPeriodo_25dias)
		return 25;

	else if (tipoPeriodo == TipoPeriodo_24dias)
		return 24;

	else if (tipoPeriodo == TipoPeriodo_23dias)
		return 23;

	else if (tipoPeriodo == TipoPeriodo_22dias)
		return 22;

	else if (tipoPeriodo == TipoPeriodo_21dias)
		return 21;

	else if (tipoPeriodo == TipoPeriodo_20dias)
		return 20;

	else if (tipoPeriodo == TipoPeriodo_19dias)
		return 19;

	else if (tipoPeriodo == TipoPeriodo_18dias)
		return 18;

	else if (tipoPeriodo == TipoPeriodo_17dias)
		return 17;

	else if (tipoPeriodo == TipoPeriodo_16dias)
		return 16;

	else if (tipoPeriodo == TipoPeriodo_15dias)
		return 15;

	else if (tipoPeriodo == TipoPeriodo_14dias)
		return 14;

	else if (tipoPeriodo == TipoPeriodo_13dias)
		return 13;

	else if (tipoPeriodo == TipoPeriodo_12dias)
		return 12;

	else if (tipoPeriodo == TipoPeriodo_11dias)
		return 11;

	else if (tipoPeriodo == TipoPeriodo_10dias)
		return 10;

	else if (tipoPeriodo == TipoPeriodo_9dias)
		return 9;

	else if (tipoPeriodo == TipoPeriodo_8dias)
		return 8;

	else if (tipoPeriodo == TipoPeriodo_6dias)
		return 6;

	else if (tipoPeriodo == TipoPeriodo_5dias)
		return 5;

	else if (tipoPeriodo == TipoPeriodo_4dias)
		return 4;

	else if (tipoPeriodo == TipoPeriodo_3dias)
		return 3;

	else if (tipoPeriodo == TipoPeriodo_2dias)
		return 2;

	else if (tipoPeriodo == TipoPeriodo_diario)
		return 1;
	else
		return 0;

} // int Periodo::getDias() const{


int Periodo::getHoras() const {

	if (tipoPeriodo <= TipoPeriodo_diario)
		return getDias() * 24;
	else if (tipoPeriodo == TipoPeriodo_horario)
		return 1;
	else if (tipoPeriodo == TipoPeriodo_2horas)
		return 2;
	else if (tipoPeriodo == TipoPeriodo_3horas)
		return 3;
	else if (tipoPeriodo == TipoPeriodo_4horas)
		return 4;
	else if (tipoPeriodo == TipoPeriodo_5horas)
		return 5;
	else if (tipoPeriodo == TipoPeriodo_6horas)
		return 6;
	else if (tipoPeriodo == TipoPeriodo_7horas)
		return 7;
	else if (tipoPeriodo == TipoPeriodo_8horas)
		return 8;
	else if (tipoPeriodo == TipoPeriodo_9horas)
		return 9;
	else if (tipoPeriodo == TipoPeriodo_10horas)
		return 10;
	else if (tipoPeriodo == TipoPeriodo_11horas)
		return 11;
	else if (tipoPeriodo == TipoPeriodo_12horas)
		return 12;
	else if (tipoPeriodo == TipoPeriodo_13horas)
		return 13;
	else if (tipoPeriodo == TipoPeriodo_14horas)
		return 14;
	else if (tipoPeriodo == TipoPeriodo_15horas)
		return 15;
	else if (tipoPeriodo == TipoPeriodo_16horas)
		return 16;
	else if (tipoPeriodo == TipoPeriodo_17horas)
		return 17;
	else if (tipoPeriodo == TipoPeriodo_18horas)
		return 18;
	else if (tipoPeriodo == TipoPeriodo_19horas)
		return 19;
	else if (tipoPeriodo == TipoPeriodo_20horas)
		return 20;
	else if (tipoPeriodo == TipoPeriodo_21horas)
		return 21;
	else if (tipoPeriodo == TipoPeriodo_22horas)
		return 22;
	else if (tipoPeriodo == TipoPeriodo_23horas)
		return 23;
	else
		return 0;

} // int Periodo::getHoras() const{


int Periodo::getMinutos() const {

	if (tipoPeriodo <= TipoPeriodo_horario)
		return getHoras() * 60;
	else if (tipoPeriodo == TipoPeriodo_meia_hora)
		return 30;
	else if (tipoPeriodo == TipoPeriodo_minuto)
		return 1;
	else
		return 0;

} // int Periodo::getMinutos() const{


int Periodo::getSegundos() const {
	return getMinutos() * 60;
} // int Periodo::getSegundos() const{

TipoPeriodo Periodo::getTipoPeriodo() const { return tipoPeriodo; }

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

	if ((tipoPeriodo == TipoPeriodo_minuto) || (tipoPeriodo == TipoPeriodo_meia_hora))
		return std::string(normStringDia(dia) + "/" + normStringMes(mes) + "/" + getString(ano) + "-" + normStringHor(hora) + ":" + normStringMin(minuto) + ":00-" + getString(tipoPeriodo));

	else if ((hora == IdHor_0) && (minuto == IdMin_0)) {

		if (tipoPeriodo == TipoPeriodo_diario)
			return std::string(normStringDia(dia) + "/" + normStringMes(mes) + "/" + getString(ano));

		else if (((tipoPeriodo < TipoPeriodo_diario) && (tipoPeriodo > TipoPeriodo_mensal)) || ((tipoPeriodo == TipoPeriodo_mensal) && (dia != IdDia_1)))
			return std::string(normStringDia(dia) + "/" + normStringMes(mes) + "/" + getString(ano) + "-" + getString(tipoPeriodo));

		else if ((tipoPeriodo == TipoPeriodo_mensal) && (dia == IdDia_1))
			return std::string(normStringMes(mes) + "/" + getString(ano));

		else if ((tipoPeriodo == TipoPeriodo_anual) && (mes == IdMes_1) && (dia == IdDia_1))
			return std::string(getString(ano));
		else if (tipoPeriodo == TipoPeriodo_anual)
			return std::string(normStringDia(dia) + "/" + normStringMes(mes) + "/" + getString(ano) + "-" + getString(tipoPeriodo));

	} // else if ((hora == IdHor_0) && (minuto == IdMin_0)) {

	return std::string(normStringDia(dia) + "/" + normStringMes(mes) + "/" + getString(ano) + "-" + normStringHor(hora) + ":" + normStringMin(minuto) + ":00-" + getString(tipoPeriodo));

} // std::string Periodo::str() const{


bool Periodo::isValido() const { if (tipoPeriodo == TipoPeriodo_Nenhum) { return false; } else { return true; } }


Periodo operator+(const Periodo& a_periodo, const int a_iterador) {

	try {

		if (!a_periodo.isValido())
			throw std::invalid_argument("Periodo invalido.");

		const TipoPeriodo tipoPeriodoArg = a_periodo.getTipoPeriodo();

		IdAno anoIter;
		IdMes mesIter;
		IdDia diaIter;
		IdHor horIter;
		IdMin minIter;

		if (tipoPeriodoArg == TipoPeriodo_anual)
			Periodo::iteraAno(a_periodo, a_iterador, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_mensal)
			Periodo::iteraMes(a_periodo, a_iterador, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_60dias)
			Periodo::iteraDia(a_periodo, a_iterador * 60, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_59dias)
			Periodo::iteraDia(a_periodo, a_iterador * 59, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_58dias)
			Periodo::iteraDia(a_periodo, a_iterador * 58, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_57dias)
			Periodo::iteraDia(a_periodo, a_iterador * 57, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_56dias)
			Periodo::iteraDia(a_periodo, a_iterador * 56, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_55dias)
			Periodo::iteraDia(a_periodo, a_iterador * 55, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_54dias)
			Periodo::iteraDia(a_periodo, a_iterador * 54, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_53dias)
			Periodo::iteraDia(a_periodo, a_iterador * 53, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_52dias)
			Periodo::iteraDia(a_periodo, a_iterador * 52, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_51dias)
			Periodo::iteraDia(a_periodo, a_iterador * 51, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_50dias)
			Periodo::iteraDia(a_periodo, a_iterador * 50, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_49dias)
			Periodo::iteraDia(a_periodo, a_iterador * 49, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_48dias)
			Periodo::iteraDia(a_periodo, a_iterador * 48, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_47dias)
			Periodo::iteraDia(a_periodo, a_iterador * 47, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_46dias)
			Periodo::iteraDia(a_periodo, a_iterador * 46, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_45dias)
			Periodo::iteraDia(a_periodo, a_iterador * 45, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_44dias)
			Periodo::iteraDia(a_periodo, a_iterador * 44, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_43dias)
			Periodo::iteraDia(a_periodo, a_iterador * 43, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_42dias)
			Periodo::iteraDia(a_periodo, a_iterador * 42, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_41dias)
			Periodo::iteraDia(a_periodo, a_iterador * 41, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_40dias)
			Periodo::iteraDia(a_periodo, a_iterador * 40, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_39dias)
			Periodo::iteraDia(a_periodo, a_iterador * 39, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_38dias)
			Periodo::iteraDia(a_periodo, a_iterador * 38, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_37dias)
			Periodo::iteraDia(a_periodo, a_iterador * 37, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_36dias)
			Periodo::iteraDia(a_periodo, a_iterador * 36, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_35dias)
			Periodo::iteraDia(a_periodo, a_iterador * 35, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_34dias)
			Periodo::iteraDia(a_periodo, a_iterador * 34, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_33dias)
			Periodo::iteraDia(a_periodo, a_iterador * 33, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_32dias)
			Periodo::iteraDia(a_periodo, a_iterador * 32, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_31dias)
			Periodo::iteraDia(a_periodo, a_iterador * 31, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_30dias)
			Periodo::iteraDia(a_periodo, a_iterador * 30, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_29dias)
			Periodo::iteraDia(a_periodo, a_iterador * 29, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_28dias)
			Periodo::iteraDia(a_periodo, a_iterador * 28, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_27dias)
			Periodo::iteraDia(a_periodo, a_iterador * 27, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_26dias)
			Periodo::iteraDia(a_periodo, a_iterador * 26, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_25dias)
			Periodo::iteraDia(a_periodo, a_iterador * 25, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_24dias)
			Periodo::iteraDia(a_periodo, a_iterador * 24, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_23dias)
			Periodo::iteraDia(a_periodo, a_iterador * 23, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_22dias)
			Periodo::iteraDia(a_periodo, a_iterador * 22, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_21dias)
			Periodo::iteraDia(a_periodo, a_iterador * 21, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_20dias)
			Periodo::iteraDia(a_periodo, a_iterador * 20, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_19dias)
			Periodo::iteraDia(a_periodo, a_iterador * 19, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_18dias)
			Periodo::iteraDia(a_periodo, a_iterador * 18, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_17dias)
			Periodo::iteraDia(a_periodo, a_iterador * 17, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_16dias)
			Periodo::iteraDia(a_periodo, a_iterador * 16, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_15dias)
			Periodo::iteraDia(a_periodo, a_iterador * 15, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_14dias)
			Periodo::iteraDia(a_periodo, a_iterador * 14, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_13dias)
			Periodo::iteraDia(a_periodo, a_iterador * 13, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_12dias)
			Periodo::iteraDia(a_periodo, a_iterador * 12, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_11dias)
			Periodo::iteraDia(a_periodo, a_iterador * 11, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_10dias)
			Periodo::iteraDia(a_periodo, a_iterador * 10, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_9dias)
			Periodo::iteraDia(a_periodo, a_iterador * 9, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_8dias)
			Periodo::iteraDia(a_periodo, a_iterador * 8, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_semanal)
			Periodo::iteraDia(a_periodo, a_iterador * 7, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_6dias)
			Periodo::iteraDia(a_periodo, a_iterador * 6, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_5dias)
			Periodo::iteraDia(a_periodo, a_iterador * 5, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_4dias)
			Periodo::iteraDia(a_periodo, a_iterador * 4, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_3dias)
			Periodo::iteraDia(a_periodo, a_iterador * 3, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_2dias)
			Periodo::iteraDia(a_periodo, a_iterador * 2, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_diario)
			Periodo::iteraDia(a_periodo, a_iterador, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_23horas)
			Periodo::iteraHora(a_periodo, a_iterador * 23, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_22horas)
			Periodo::iteraHora(a_periodo, a_iterador * 22, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_21horas)
			Periodo::iteraHora(a_periodo, a_iterador * 21, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_20horas)
			Periodo::iteraHora(a_periodo, a_iterador * 20, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_19horas)
			Periodo::iteraHora(a_periodo, a_iterador * 19, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_18horas)
			Periodo::iteraHora(a_periodo, a_iterador * 18, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_17horas)
			Periodo::iteraHora(a_periodo, a_iterador * 17, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_16horas)
			Periodo::iteraHora(a_periodo, a_iterador * 16, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_15horas)
			Periodo::iteraHora(a_periodo, a_iterador * 15, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_14horas)
			Periodo::iteraHora(a_periodo, a_iterador * 14, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_13horas)
			Periodo::iteraHora(a_periodo, a_iterador * 13, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_12horas)
			Periodo::iteraHora(a_periodo, a_iterador * 12, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_11horas)
			Periodo::iteraHora(a_periodo, a_iterador * 11, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_10horas)
			Periodo::iteraHora(a_periodo, a_iterador * 10, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_9horas)
			Periodo::iteraHora(a_periodo, a_iterador * 9, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_8horas)
			Periodo::iteraHora(a_periodo, a_iterador * 8, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_7horas)
			Periodo::iteraHora(a_periodo, a_iterador * 7, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_6horas)
			Periodo::iteraHora(a_periodo, a_iterador * 6, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_5horas)
			Periodo::iteraHora(a_periodo, a_iterador * 5, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_4horas)
			Periodo::iteraHora(a_periodo, a_iterador * 4, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_3horas)
			Periodo::iteraHora(a_periodo, a_iterador * 3, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_2horas)
			Periodo::iteraHora(a_periodo, a_iterador * 2, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_horario)
			Periodo::iteraHora(a_periodo, a_iterador, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_meia_hora)
			Periodo::iteraMinuto(a_periodo, a_iterador * 30, anoIter, mesIter, diaIter, horIter, minIter);

		else if (tipoPeriodoArg == TipoPeriodo_minuto)
			Periodo::iteraMinuto(a_periodo, a_iterador, anoIter, mesIter, diaIter, horIter, minIter);
		else
			throw std::invalid_argument("Tipo de Periodo invalido.");

		return Periodo(tipoPeriodoArg, diaIter, mesIter, anoIter, horIter, minIter);

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::+(" + a_periodo.str() + "," + std::to_string(a_iterador) + "): \n" + std::string(erro.what())); }

} // Periodo operator+(const Periodo &a_periodo, const int a_iterador){


Periodo operator-(const Periodo& a_periodo, const int a_iterador) { return operator+(a_periodo, -a_iterador); }


int operator-(const Periodo& a_periodo1, const Periodo& a_periodo2) {

	try {

		if (!a_periodo1.isValido())
			throw std::invalid_argument("Argumento 1 com Periodo invalido.");

		else if (!a_periodo2.isValido())
			throw std::invalid_argument("Argumento 2 com Periodo invalido.");

		const TipoPeriodo tipoPeriodoArg1 = a_periodo1.getTipoPeriodo();
		const TipoPeriodo tipoPeriodoArg2 = a_periodo2.getTipoPeriodo();

		if (tipoPeriodoArg1 != tipoPeriodoArg2)
			throw std::invalid_argument("Tipos de Periodo diferentes inviabilizam a operacao.");

		if (tipoPeriodoArg1 == TipoPeriodo_anual)
			return a_periodo1.getAno() - a_periodo2.getAno();

		else if (tipoPeriodoArg1 == TipoPeriodo_mensal)
			return (a_periodo1.getAno() - a_periodo2.getAno()) * 12 + a_periodo1.getMes() - a_periodo2.getMes();

		else if (tipoPeriodoArg1 > TipoPeriodo_mensal && tipoPeriodoArg1 <= TipoPeriodo_diario) {

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

		}//else if (tipoPeriodoArg1 > TipoPeriodo_mensal && tipoPeriodoArg1 <= TipoPeriodo_diario) {

		else if (tipoPeriodoArg1 >= TipoPeriodo_diario) {

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

		} // else if (tipoPeriodoArg1 >= TipoPeriodo_diario) {

		else
			throw std::invalid_argument("Tipo de Periodo invalido.");

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

		const TipoPeriodo tipoPeriodoArg1 = a_periodo1.getTipoPeriodo();
		const TipoPeriodo tipoPeriodoArg2 = a_periodo2.getTipoPeriodo();

		if (tipoPeriodoArg1 != tipoPeriodoArg2)
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
		Periodo::iteraDia(Periodo(a_periodo.getTipoPeriodo(), a_diaIter, a_mesIter, a_anoIter, a_horaIter, a_minutoIter), iteradorDia, a_anoIter, a_mesIter, a_diaIter, a_horaIter, a_minutoIter);

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
		Periodo::iteraHora(Periodo(a_periodo.getTipoPeriodo(), a_diaIter, a_mesIter, a_anoIter, a_horaIter, a_minutoIter), iteradorHora, a_anoIter, a_mesIter, a_diaIter, a_horaIter, a_minutoIter);

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

Periodo Periodo::getPeriodoInicial(TipoPeriodo a_tipoPeriodo) {

	return Periodo(a_tipoPeriodo, 1, 1, ano_inicial, 0, 0);

} // Periodo Periodo::getPeriodoInicial(TipoPeriodo a_tipoPeriodo){


Periodo Periodo::getPeriodoFinal() {

	return Periodo(31, 12, ano_final, 23, 59);

} // Periodo Periodo::getPeriodoFinal(){


Periodo Periodo::getPeriodoFinal(TipoPeriodo a_tipoPeriodo) {

	return Periodo(a_tipoPeriodo, 31, 12, ano_final, 23, 59);

} // Periodo Periodo::getPeriodoFinal(TipoPeriodo a_tipoPeriodo){


IdEstacao Periodo::getEstacao() const {

	try {

		if ((tipoPeriodo <= TipoPeriodo_Nenhum) || (tipoPeriodo >= TipoPeriodo_Excedente))
			throw std::invalid_argument("Tipo de periodo invalido.");

		if (tipoPeriodo == TipoPeriodo_mensal)
			return IdEstacao(getMes());

		else
			return IdEstacao_1;

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::getEstacao(): \n" + std::string(erro.what())); }

}  // IdEstacao Periodo::getEstacao() const{


IdEstacao Periodo::getMaiorEstacao(TipoPeriodo a_tipoPeriodo) {

	try {

		if ((a_tipoPeriodo <= TipoPeriodo_Nenhum) || (a_tipoPeriodo >= TipoPeriodo_Excedente))
			throw std::invalid_argument("Tipo de periodo invalido.");

		if (a_tipoPeriodo == TipoPeriodo_anual)
			return IdEstacao_1;

		else if (a_tipoPeriodo == TipoPeriodo_mensal)
			return IdEstacao_12;

		else if (a_tipoPeriodo == TipoPeriodo_semanal)
			return IdEstacao_52;

		else if (a_tipoPeriodo == TipoPeriodo_diario)
			return IdEstacao_365;

		return IdEstacao_12;

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::getMaiorEstacao(" + getString(a_tipoPeriodo) + "): \n" + std::string(erro.what())); }

} // IdEstacao Periodo::getEstacao(TipoPeriodo a_tipoPeriodo){

Periodo Periodo::deslocarPeriodo(const Periodo& a_periodo, int a_numero_de_horas)
{
	try {

		IdAno anoIter;
		IdMes mesIter;
		IdDia diaIter;
		IdHor horIter;
		IdMin minIter;

		Periodo::iteraHora(a_periodo, a_numero_de_horas, anoIter, mesIter, diaIter, horIter, minIter);

		TipoPeriodo tipoPeriodo = a_periodo.getTipoPeriodo();

		if (tipoPeriodo == TipoPeriodo_mensal) {

			const int numeroDias = a_periodo.getDias();

			if (numeroDias == 28)
				tipoPeriodo = TipoPeriodo_28dias;
			else if (numeroDias == 29)
				tipoPeriodo = TipoPeriodo_29dias;
			else if (numeroDias == 30)
				tipoPeriodo = TipoPeriodo_30dias;
			else if (numeroDias == 31)
				tipoPeriodo = TipoPeriodo_31dias;
			else
				throw std::invalid_argument("Tipo de periodo mensal invalido.");

		}//if (tipoPeriodo == TipoPeriodo_mensal) {

		return Periodo(tipoPeriodo, diaIter, mesIter, anoIter, horIter, minIter);

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::deslocarPeriodo(" + getString(a_periodo) + getString(a_numero_de_horas) + "): \n" + std::string(erro.what())); }

}

Periodo Periodo::getPeriodoDiario_do_diaFinal(const Periodo& a_periodo)
{
	try {

		IdAno anoIter;
		IdMes mesIter;
		IdDia diaIter;
		IdHor horIter;
		IdMin minIter;

		const int numero_de_dias = a_periodo.getDias();

		Periodo::iteraDia(a_periodo, numero_de_dias, anoIter, mesIter, diaIter, horIter, minIter);

		return Periodo(TipoPeriodo_diario, diaIter, mesIter, anoIter, horIter, minIter);;

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Periodo::getPeriodoDiario_do_diaFinal(" + getString(a_periodo) + "): \n" + std::string(erro.what())); }

}

Periodo::~Periodo() {}


