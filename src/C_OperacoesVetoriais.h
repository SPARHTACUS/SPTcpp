#ifndef C_OPERACOES_VETORIAIS
#define C_OPERACOES_VETORIAIS

#include "C_Auxiliar.h"
#include "C_SmartEnupla.h"

#include <vector>
#include <string>
#include <iostream>

#include <sstream>


//
// Cálculo de Soma
//

template<typename Valor>
static Valor getSoma(const std::vector<Valor> &a_vetor) {

	const int numero_elementos = int(a_vetor.size());

	if (numero_elementos == 0)
		return Valor(0);

	Valor soma = a_vetor.at(0);
	for (int i = 1; i < numero_elementos; i++)
		soma += a_vetor.at(i);

	return soma;

} // static Valor getSoma(const std::vector<Valor> &a_vetor) {


template<typename Iterador, typename Valor>
static Valor getSoma(const SmartEnupla<Iterador, Valor> &a_enupla) {

	const int numero_elementos = int(a_enupla.size());

	if (numero_elementos == 0)
		return Valor(0);

	const Iterador iteradorInicial = a_enupla.getIteradorInicial();
	const Iterador iteradorFinal = a_enupla.getIteradorFinal();

	Valor soma = a_enupla.getElemento(iteradorInicial);
	for (Iterador iter = Iterador(iteradorInicial + 1); iter <= iteradorFinal; iter++)
		soma += a_enupla.getElemento(iter);

	return soma;

} // static Valor getSoma(const std::vector<Valor> &a_vetor) {


//
// Cálculo de Média
//

template<typename Valor>
static Valor getMedia(const std::vector<Valor> &a_vetor) {

	const int numero_elementos = int(a_vetor.size());

	if (numero_elementos == 0)
		return Valor(0);

	Valor media = a_vetor.at(0);
	for (int i = 1; i < numero_elementos; i++)
		media += a_vetor.at(i);
		
	media /= Valor(numero_elementos);

	return media;

} // static Valor getMedia(const std::vector<Valor> &a_vetor) {


template<typename Iterador, typename Valor>
static Valor getMedia(const SmartEnupla<Iterador, Valor> &a_enupla) {

	const int numero_elementos = int(a_enupla.size());

	if (numero_elementos == 0)
		return Valor(0);

	const Iterador iteradorInicial = a_enupla.getIteradorInicial();
	const Iterador iteradorFinal   = a_enupla.getIteradorFinal  ();

	Valor media = a_enupla.getElemento(iteradorInicial);
	for (Iterador iter = Iterador(iteradorInicial + 1); iter <= iteradorFinal; iter++)
		media += a_enupla.getElemento(iter);

	media /= Valor(numero_elementos);

	return media;

} // static Valor getMedia(const std::vector<Valor> &a_vetor) {

template<typename Valor>
static Valor getDesvio(const std::vector<Valor> &a_vetor) { return getDesvio(getMedia(a_vetor), a_vetor); }

template<typename Valor>
static Valor getDesvio(const Valor a_media, const std::vector<Valor> &a_vetor) {

	const int numero_elementos = int(a_vetor.size());

	if ((numero_elementos == 0) || (numero_elementos == 1))
		return Valor(0);

	Valor desvio = std::pow(a_vetor.at(0) - a_media, 2);
	for (int i = 1; i < numero_elementos; i++)
		desvio += std::pow(a_vetor.at(i) - a_media, 2);

	desvio /= double(numero_elementos - 1);

	return std::sqrt(desvio);

} // static Valor getDesvio(const Valor a_media, const std::vector<Valor> &a_vetor) {


template<typename Iterador, typename Valor>
static Valor getDesvio(const SmartEnupla<Iterador, Valor> &a_enupla) { return getDesvio(getMedia(a_enupla), a_enupla); }

template<typename Iterador, typename Valor>
static Valor getDesvio(const Valor a_media, const SmartEnupla<Iterador, Valor> &a_enupla) {

	const int numero_elementos = int(a_enupla.size());

	if ((numero_elementos == 0) || (numero_elementos == 1))
		return Valor(0);

	const Iterador iteradorInicial = a_enupla.getIteradorInicial();
	const Iterador iteradorFinal = a_enupla.getIteradorFinal();

	Valor desvio = std::pow(a_enupla.getElemento(iteradorInicial) - a_media, 2);
	for (Iterador iter = Iterador(iteradorInicial + 1); iter <= iteradorFinal; iter++)
		desvio += std::pow(a_enupla.getElemento(iter) - a_media, 2);

	desvio /= double(numero_elementos - 1);

	return std::sqrt(desvio);

} // static Valor getDesvio(const Valor a_media, const SmartEnupla<Iterador, Valor> &a_enupla) {

template<typename Valor>
static Valor getAssimetria(const std::vector<Valor> &a_vetor) { return getAssimetria(getMedia(a_vetor), getDesvio(a_vetor), a_vetor); }

template<typename Valor>
static Valor getAssimetria(const Valor a_media, const Valor a_desvio, const std::vector<Valor> &a_vetor) {

	const int numero_elementos = int(a_vetor.size());

	if ((numero_elementos == 0) || (numero_elementos == 1))
		return Valor(0);

	Valor assimetria = std::pow(a_vetor.at(0) - a_media, 3);
	for (int i = 1; i < numero_elementos; i++)
		assimetria += std::pow(a_vetor.at(i) - a_media, 3);

	assimetria /= std::pow(a_desvio, 3);

	if (assimetria < 0.0)
		assimetria = 0.0;

	return assimetria;

} // static Valor getAssimetria(const std::vector<Valor> &a_vetor) {


template<typename Iterador, typename Valor>
static Valor getAssimetria(const SmartEnupla<Iterador, Valor> &a_enupla) { return getAssimetria(getMedia(a_enupla), getDesvio(a_enupla), a_enupla); }

template<typename Iterador, typename Valor>
static Valor getAssimetria(const Valor a_media, const Valor a_desvio, const SmartEnupla<Iterador, Valor> &a_enupla) {

	const int numero_elementos = int(a_enupla.size());

	if ((numero_elementos == 0) || (numero_elementos == 1))
		return Valor(0);

	const Iterador iteradorInicial = a_enupla.getIteradorInicial();
	const Iterador iteradorFinal = a_enupla.getIteradorFinal();

	Valor assimetria = std::pow(a_enupla.getElemento(iteradorInicial) - a_media, 3);
	for (Iterador iter = Iterador(iteradorInicial + 1); iter <= iteradorFinal; iter++)
		assimetria += std::pow(a_enupla.getElemento(iter) - a_media, 3);

	assimetria /= std::pow(a_desvio, 3);

	if (assimetria < 0.0)
		assimetria = 0.0;

	return assimetria;

} // static Valor getAssimetria(const Valor a_media, const SmartEnupla<Iterador, Valor> &a_enupla) {


template<typename Valor>
static std::vector<Valor> getLog(const std::vector<Valor> &a_vetor) {
	try {

		const int numero_elementos = int(a_vetor.size());

		if (numero_elementos == 0)
			return std::vector<Valor>();

		std::vector<Valor> vetor_log(numero_elementos, Valor(0));

		for (int i = 0; i < numero_elementos; i++) {
			const Valor valor = a_vetor.at(i);

			if (valor <= 0)
				throw std::invalid_argument("O elemento " + getString(valor) + " inviabiliza um valor real de log().");

			vetor_log.at(i) = std::log(valor);
		} // for (int i = 0; i < numero_elementos; i++) {

		return vetor_log;

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("getLog(a_vetor): \n" + std::string(erro.what())); }

} // static Valor getLog(const std::vector<Valor> &a_vetor) {


template<typename Iterador, typename Valor>
static SmartEnupla<Iterador, Valor> getLog(const SmartEnupla<Iterador, Valor> &a_enupla) {
	try {

		const int numero_elementos = int(a_enupla.size());

		if (numero_elementos == 1)
			return SmartEnupla<Iterador, Valor>();

		const Iterador iteradorInicial = a_enupla.getIteradorInicial();
		const Iterador iteradorFinal = a_enupla.getIteradorFinal();

		SmartEnupla<Iterador, Valor> vetor_log(iteradorInicial, std::vector<Valor>(numero_elementos, Valor(0)));

		for (Iterador iter = iteradorInicial; iter <= iteradorFinal; iter++) {
			const Valor valor = a_enupla.getElemento(iter);

			if (valor <= 0)
				throw std::invalid_argument("O elemento " + getString(valor) + " inviabiliza um valor real de log().");

			vetor_log.setElemento(iter,std::log(valor));
		} // for (Iterador iter = Iterador(iteradorInicial + 1); iter <= iteradorFinal; iter++){

		return vetor_log;

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("getLog(a_enupla): \n" + std::string(erro.what())); }

} // static Valor getLog(const Valor a_media, const SmartEnupla<Iterador, Valor> &a_enupla) {


#endif 