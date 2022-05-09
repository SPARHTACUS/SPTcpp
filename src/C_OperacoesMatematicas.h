#ifndef C_OPERACOES_MATEMATICAS
#define C_OPERACOES_MATEMATICAS

#include "C_Auxiliar.h"
#include "C_SmartEnupla.h"

#include <vector>
#include <string>

#include <iostream>

#include <sstream>

#include <random> 

#ifdef SPTWIN
#include "includeQMC.h"
#endif

template<typename Valor>
static int encontrar(const std::vector<Valor> &a_vetor, const Valor a_valor) {

	for (int i = 0; i < int(a_vetor.size()); i++) {

		if (a_vetor.at(i) == a_valor)
			return i;
	}

	return -1;

} // static int encontrar(const std::vector<Valor> &a_vetor) {


static std::vector<double> getMinimosQuadrados(const std::vector<double> a_x, const std::vector<double> a_y) {
	try {
		if (a_x.size() != a_y.size())
			throw std::invalid_argument("Os vetores não tem a mesma dimensao.");
		
		std::vector<double> coef;

		double somatorioX = 0;
		double somatorioY = 0;
		double somatorioXQuad = 0;
		double somatorioX_x_Y = 0;


		for (int i = 0; i < int(a_x.size()); i++) {
			somatorioX += a_x[i];
			somatorioY += a_y[i];
			somatorioXQuad += a_x[i] * a_x[i];
			somatorioX_x_Y += a_x[i] * a_y[i];
					   			 
		} // for (int i = 0; i < a_x.size(); i++) {

		//COEFICIENTE A0
		coef.push_back(((somatorioXQuad * somatorioY) - (somatorioX_x_Y * somatorioX)) / \
			(double(a_x.size()) * somatorioXQuad - (somatorioX * somatorioX)));

		//COEFICIENTE A1
		coef.push_back(((double(a_x.size()) * somatorioX_x_Y) - (somatorioX * somatorioY)) /\
			(double(a_x.size()) * somatorioXQuad - (somatorioX * somatorioX)));
		
		double erro = 0;
		for (int i = 0; i < int(a_x.size()); i++) {
			erro += std::abs(((a_x[i] * coef[1] + coef[0]) - a_y[i]) / a_y[i]);
		}
		
		if(coef[0] == 0 && coef[1] == 0)
			coef.push_back(0);
		
		else if (erro == 0)
			coef.push_back(0);
		
		else if (erro > 0)
			coef.push_back((erro / double(a_x.size())) * 100.0);
		
		return coef;

	} // try{
	
	catch (const std::exception & erro) { throw std::invalid_argument("getMinimosQuadrados ): \n" + std::string(erro.what())); }

} //static std::vector<double> minimoQuadrado(const std::vector<double> a_x, const std::vector<double> a_y) {


static double conversor_vazao_m3s_em_volume_hm3(const Periodo a_periodo) {
	try { return double(a_periodo.getMinutos()) * 0.00006; } // try{
	catch (const std::exception&erro) { throw std::invalid_argument("conversor_vazao_m3s_em_volume_hm3(" + getString(a_periodo) + "): \n" + std::string(erro.what())); }
} // static double conversao_vazao_m3s_em_volume_hm3(const Periodo a_periodo, const double a_vazao_m3s) {

static double conversao_vazao_m3s_em_volume_hm3(const Periodo a_periodo, const double a_vazao_m3s) {
	try { return conversor_vazao_m3s_em_volume_hm3(a_periodo) * a_vazao_m3s; } // try{
	catch (const std::exception&erro) { throw std::invalid_argument("conversao_vazao_m3s_em_volume_hm3(" + getString(a_periodo) + "," + getString(a_vazao_m3s) + "): \n" + std::string(erro.what())); }
} // static double conversao_vazao_m3s_em_volume_hm3(const Periodo a_periodo, const double a_vazao_m3s) {

//
// Sorteio Uniforme entre 0.0 e 1.0
//

static double getUniforme_01(int &a_semente) {
	//
	//  Purpose:
	//
	//    R8_UNIFORM_01 returns a unit pseudorandom R8.
	//
	//  Discussion:
	//
	//    This routine implements the recursion
	//
	//      a_semente = 16807 * a_semente mod ( 2**31 - 1 )
	//      r8_uniform_01 = a_semente / ( 2**31 - 1 )
	//
	//    The integer arithmetic never requires more than 32 bits,
	//    including a sign bit.
	//
	//    If the initial a_semente is 12345, then the first three computations are
	//
	//      Input     Output      R8_UNIFORM_01
	//      SEED      SEED
	//
	//         12345   207482415  0.096616
	//     207482415  1790989824  0.833995
	//    1790989824  2035175616  0.947702
	//
	//  Modified:
	//
	//    11 August 2004
	//
	//  Author:
	//
	//    John Burkardt
	//
	//  Reference:
	//
	//    Paul Bratley, Bennett Fox, Linus Schrage,
	//    A Guide to Simulation,
	//    Springer Verlag, pages 201-202, 1983.
	//
	//    Pierre L'Ecuyer,
	//    Random Number Generation,
	//    in Handbook of Simulation
	//    edited by Jerry Banks,
	//    Wiley Interscience, page 95, 1998.
	//
	//    Bennett Fox,
	//    Algorithm 647:
	//    Implementation and Relative Efficiency of Quasirandom
	//    Sequence Generators,
	//    ACM Transactions on Mathematical Software,
	//    Volume 12, Number 4, pages 362-376, 1986.
	//
	//    Peter Lewis, Allen Goodman, James Miller,
	//    A Pseudo-Random Number Generator for the System/360,
	//    IBM Systems Journal,
	//    Volume 8, pages 136-143, 1969.
	//
	//  Parameters:
	//
	//    Input/output, int a_semente, the "a_semente" value.  Normally, this
	//    value should not be 0.  On output, SEED has been updated.
	//
	//    Output, double R8_UNIFORM_01, a new pseudorandom variate, strictly between
	//    0 and 1.
	//
	const int k = a_semente / 127773;

	a_semente = 16807 * (a_semente - k * 127773) - k * 2836;

	if (a_semente < 0)
		a_semente += 2147483647;
	//
	//  Although SEED can be represented exactly as a 32 bit integer,
	//  it generally cannot be represented exactly as a 32 bit real number!
	//
	const double retorno = double(a_semente) * 4.656612875E-10;

	if (retorno > 1.0)
		return 1.0;
	else if (retorno < 0.0)
		return 0.0;
	else
		return retorno;

} // static double getUniforme_01(int &a_semente) {

template<typename Elemento>
static Elemento getUniforme_Elemento(int &a_semente, const Elemento a_maior_elemento_amostra) { return getUniforme_Elemento(a_semente, Elemento(1), a_maior_elemento_amostra); }

template<typename Elemento>
static Elemento getUniforme_Elemento(int &a_semente, const Elemento a_menor_elemento_amostra, const Elemento a_maior_elemento_amostra) {

	try {
		if (a_menor_elemento_amostra > a_maior_elemento_amostra)
			throw std::invalid_argument("Elementos da amostra invalidos.");

		else if (a_menor_elemento_amostra == a_maior_elemento_amostra)
			return a_menor_elemento_amostra;

		const double numero_elementos = double(a_maior_elemento_amostra) - double(a_menor_elemento_amostra) + 1.0;

		int sorteio_uniforme = 0;
		while (sorteio_uniforme <= 0)
			sorteio_uniforme = int(std::ceil(getUniforme_01(a_semente) * numero_elementos));

		return Elemento(sorteio_uniforme - 1 + a_menor_elemento_amostra);

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("getUniforme_Elemento(" + getString(a_semente) + ","  + getString(a_menor_elemento_amostra) + "," + getString(a_maior_elemento_amostra) + "): \n" + std::string(erro.what())); }

} // static int getUniforme_Inteiro(int &a_semente, const int a_tamanho_amostra) {


//
// Sorteio Normal entre 0.0 e 1.0
//
static double getNormal_01(int &a_semente){

	//
	//  Purpose:
	//
	//    R8_NORMAL_01 returns a unit pseudonormal R8.
	//
	//  Discussion:
	//
	//    The standard normal probability distribution function (PDF) has 
	//    mean 0 and standard deviation 1.
	//
	//    Because this routine uses the Box Muller method, it requires pairs
	//    of uniform random values to generate a std::pair of normal random values.
	//    This means that on every other call, the code can use the second
	//    value that it calculated.
	//
	//    However, if the user has changed the a_semente value between calls,
	//    the routine automatically resets itself and discards the saved data.
	//
	//  Modified:
	//
	//    08 January 2007
	//
	//  Author:
	//
	//    John Burkardt
	//
	//  Parameters:
	//
	//    Input/output, int &a_semente, a seed for the random number generator.
	//
	//    Output, double R8_NORMAL_01, a normally distributed random value.
	//


	double r1;
	double r2;
	int semente1 = 0;
	int semente2 = 0;
	int semente3 = 0;
	int used = 0;
	double v1;
	double v2 = 0.0;
	//
	//  If USED is odd, but the input SEED does not match
	//  the output SEED on the previous call, then the user has changed
	//  the a_semente.  Wipe out internal memory.
	//
	if ((used % 2) == 1){
		if (a_semente != semente2){
			used = 0;
			semente1 = 0;
			semente2 = 0;
			semente3 = 0;
			v2 = 0.0;
		}
	}
	//
	//  If USED is even, generate two uniforms, create two normals,
	//  return the first normal and its corresponding a_semente.
	//
	if ((used % 2) == 0){

		semente1 = a_semente;

		r1 = getUniforme_01(a_semente);

		if (r1 == 0.0)
		{
			std::cout << "\n";
			std::cout << "R8_NORMAL_01 - Fatal error!\n";
			std::cout << "  R8_UNIFORM_01 returned a value of 0.\n";
			exit(1);
		}

		semente2 = a_semente;

		r2 = getUniforme_01(semente2);

		semente3 = a_semente;

		v1 = std::sqrt(-2.0 * std::log(r1)) * std::cos(2.0 * PI * r2);
		v2 = std::sqrt(-2.0 * std::log(r1)) * std::sin(2.0 * PI * r2);

		a_semente = semente2;
	}
	//
	//  If USED is odd (and the input SEED matched the output value from
	//  the previous call), return the second normal and its corresponding a_semente.
	//
	else{

		v1 = v2;
		a_semente = semente3;
	}

	used = used + 1;

	return v1;

} // double getNormal_01(int &a_semente){



static std::vector<double> getRuidoBranco_MC(const int a_dimensao, int &a_semente) {
	try {
		if (a_dimensao < 1)
			throw std::invalid_argument("Dimensao invalida.");

		std::vector<double> vetor_retorno(a_dimensao, NAN);
		for (int i = 0; i < a_dimensao; i++)
			vetor_retorno.at(i) = getNormal_01(a_semente);

		return vetor_retorno;
	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("getRuidoBranco_MC(" + getString(a_dimensao) + "," + getString(a_semente) + "): \n" + std::string(erro.what())); }
}

template<typename Iterador>
static SmartEnupla<Iterador, double> getRuidoBranco_MC(const Iterador a_iterador_inicial, const Iterador a_iterador_final, int &a_semente) {
	try {
		const int dimensao = int(a_iterador_final) - int(a_iterador_inicial) + 1;
		return SmartEnupla<Iterador, double>(a_iterador_inicial, getRuidoBranco_MC(dimensao, a_semente));
	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("getRuidoBranco_MC(" + getString(a_iterador_inicial) + "," + getString(a_iterador_final) + "," + getString(a_semente) + "): \n" + std::string(erro.what())); }
} // static SmartEnupla<Iterador, double> getRuidoBranco_MC(const Iterador a_iterador_inicial, const Iterador a_iterador_final, int &a_semente) {



static std::vector<double> getUniforme_01_LHS(int dimensao, int discretizacao, int& a_semente) {

	std::vector<double> retorno(dimensao, NAN);
	std::vector<double> ruido(discretizacao, 1.0);

	std::default_random_engine semente(a_semente);

	for (int i = 0; i < dimensao; i++) {

		for (int j = 0; j < discretizacao; j++) {

			const double Umin = double(j) / double(discretizacao);
			const double Umax = double(j + 1) / double(discretizacao);
			ruido[j] = Umin + (Umax - Umin) * getUniforme_01(a_semente);

			if (ruido[j] > (1 - std::pow(10.0, (-20.0))))
				ruido[j] = 1 - (10 ^ (-19));
			else if (ruido[j] < std::pow(10.0, (-20.0)))
				ruido[j] = 10 ^ (-19);
		}

		std::shuffle(ruido.begin(), ruido.end(), semente);

		retorno.at(i) = ruido[0];

	}

	return retorno;

} // static std::vector<double> getUniforme_01_LHS(int dimensao, int discretizacao, int &a_semente){

static double getNormalFromUniforme(const double a_valor_uniforme){

	if (((std::pow(10.0, (-20.0))) <= a_valor_uniforme) && (a_valor_uniforme <= 0.5)){

		const double y = std::sqrt(-2.0 * std::log(a_valor_uniforme));

		double Ax = -0.322232431088;
		Ax = Ax - 1 * y;
		Ax = Ax - 0.342242088547     * std::pow(y, 2.0);
		Ax = Ax - 0.0204231210245    * std::pow(y, 3.0);
		Ax = Ax - 0.0000453642210148 * std::pow(y, 4.0);

		double Bx = 0.0993484626060;
		Bx = Bx + 0.588581570495  *  y;
		Bx = Bx + 0.531103462366  * std::pow(y, 2.0);
		Bx = Bx + 0.103537752850  * std::pow(y, 3.0);
		Bx = Bx + 0.0038560700634 * std::pow(y, 4.0);

		const double valor_normal = y + (Ax / Bx);
		return valor_normal;
	}

	else{

		const double y = std::sqrt(-2.0 * std::log(1 - a_valor_uniforme));

		double Ax = -0.322232431088;
		Ax = Ax - 1 * y;
		Ax = Ax - 0.342242088547     * std::pow(y, 2.0);
		Ax = Ax - 0.0204231210245    * std::pow(y, 3.0);
		Ax = Ax - 0.0000453642210148 * std::pow(y, 4.0);

		double Bx = 0.0993484626060;
		Bx = Bx + 0.588581570495  * y;
		Bx = Bx + 0.531103462366  * std::pow(y, 2.0);
		Bx = Bx + 0.103537752850  * std::pow(y, 3.0);
		Bx = Bx + 0.0038560700634 * std::pow(y, 4.0);

		const double valor_normal = -y - (Ax / Bx);
		return valor_normal;
	}

} // static double getNormalFromUniforme(const double a_valor_uniforme){


static std::vector<double> getRuidoBranco_LHS(const int a_dimensao, int& a_semente) {
	try {
		if (a_dimensao < 1)
			throw std::invalid_argument("Dimensao invalida.");
		
		std::vector<double> vetor_retorno = getUniforme_01_LHS(a_dimensao, int(std::ceil(std::sqrt(a_dimensao))), a_semente);
		for (int i = 0; i < a_dimensao; i++)
			vetor_retorno.at(i) = getNormalFromUniforme(vetor_retorno.at(i));

		return vetor_retorno;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("getRuidoBranco_LHS(" + getString(a_dimensao) + "," + getString(a_semente) + "): \n" + std::string(erro.what())); }
}

template<typename Iterador>
static SmartEnupla<Iterador, double> getRuidoBranco_LHS(const Iterador a_iterador_inicial, const Iterador a_iterador_final, int& a_semente) {
	try {
		const int dimensao = int(a_iterador_final) - int(a_iterador_inicial) + 1;
		return SmartEnupla<Iterador, double>(a_iterador_inicial, getRuidoBranco_LHS(dimensao, a_semente));
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("getRuidoBranco_LHS(" + getString(a_iterador_inicial) + "," + getString(a_iterador_final) + "," + getString(a_semente) + "): \n" + std::string(erro.what())); }
} // static SmartEnupla<Iterador, double> getRuidoBranco_LHS(const Iterador a_iterador_inicial, const Iterador a_iterador_final, int &a_semente) {


static std::vector<double> getRuidoBranco(const TipoSorteio a_tipo_sorteio, const int a_dimensao, int &a_semente) {
	try {
		if (a_tipo_sorteio == TipoSorteio_MC)
			return getRuidoBranco_MC(a_dimensao, a_semente);
		if (a_tipo_sorteio == TipoSorteio_LHS)
			return getRuidoBranco_LHS(a_dimensao, a_semente);
		else
			throw std::invalid_argument("Tipo de sorteio ainda nao implementado");
	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("getRuidoBranco(" + getString(a_tipo_sorteio) + "," + getString(a_dimensao) + "," + getString(a_semente) + "): \n" + std::string(erro.what())); }
} // static SmartEnupla<Iterador, double> getRuidoBranco(const TipoSorteio a_tipo_sorteio, const Iterador a_iterador_inicial, const Iterador a_iterador_final, int &a_semente) {


template<typename Iterador>
static SmartEnupla<Iterador, double> getRuidoBranco(const TipoSorteio a_tipo_sorteio, const Iterador a_iterador_inicial, const Iterador a_iterador_final, int &a_semente) {
	try {
		if (a_tipo_sorteio == TipoSorteio_MC)
			return getRuidoBranco_MC(a_iterador_inicial, a_iterador_final, a_semente);
		else
			throw std::invalid_argument("Tipo de sorteio ainda nao implementado");
	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("getRuidoBranco(" + getString(a_tipo_sorteio) + "," + getString(a_iterador_inicial) + "," + getString(a_iterador_final) + "," + getString(a_semente) + "): \n" + std::string(erro.what())); }
} // static SmartEnupla<Iterador, double> getRuidoBranco(const TipoSorteio a_tipo_sorteio, const Iterador a_iterador_inicial, const Iterador a_iterador_final, int &a_semente) {


static void getUniforme_01_QMC(double **x, int dimensao, int discretizacao, int &a_semente){

#ifdef SPTWIN

	int i, j;

	UL_int base;
	int length;

	length = discretizacao;
	base = 2;

	char *p = (char*)"Niederreiter";

	Ring R(base);
	C_matrix C(&R, p, dimensao);
	Sequence *QMCSeq;

	getUniforme_01(a_semente);
	srand(a_semente);

	if (base == 2)
		QMCSeq = new RandomizedTSSequence_base_2(&C, dimensao, length);
	else
		QMCSeq = new RandomizedTSSequence(&C, dimensao, length);

	for (i = 0; i < length; i++){
		for (j = 0; j < dimensao; j++)
			x[j][i] = (*QMCSeq)[j];
		++(*QMCSeq);
	}

#endif

} // static void getUniforme_01_QMC(double **x, int dimensao, int discretizacao, int *seed){



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
static Valor getSoma(const SmartEnupla<Iterador, Valor> &a_enupla) { return getSoma(a_enupla.getElementos()); } 

template<typename Valor>
static Valor getSoma_noNAN(const std::vector<Valor>& a_vetor) {

	const int numero_elementos = int(a_vetor.size());

	if (numero_elementos == 0)
		return Valor(0);

	Valor soma = a_vetor.at(0);
	for (int i = 1; i < numero_elementos; i++) {
		if (!std::isnan(a_vetor.at(i)))
			soma += a_vetor.at(i);
	}

	return soma;

} // static Valor getSoma(const std::vector<Valor> &a_vetor) {

template<typename Iterador, typename Valor>
static Valor getSoma_noNAN(const SmartEnupla<Iterador, Valor>& a_enupla) { return getSoma_noNAN(a_enupla.getElementos()); }

//
// Cálculo de Ordenação
//

template<typename Valor>
static std::vector<int> getOrdenacaoMaiorParaMenor(const std::vector<Valor>& a_vetor) {

	const int numero_elementos = int(a_vetor.size());

	if (numero_elementos == 0)
		return std::vector<int>();
	else if (numero_elementos == 1)
		return std::vector<int>(1, 0);

	std::vector<int> ordenacao(1, 0);

	for (int i = 1; i < numero_elementos; i++) {

		for (int j = 0; j < int(ordenacao.size()); j++) {

			if (a_vetor.at(i) > a_vetor.at(ordenacao.at(j))) {
				ordenacao.insert(ordenacao.begin() + j, i);
				break;
			}
			else if (j == int(ordenacao.size() - 1)) {
				ordenacao.push_back(i);
				break;
			}

		} // for (int j = 0; j < int(ordenacao.size()); j++) {

	} // for (int i = 0; i < numero_elementos; i++) {

	return ordenacao;

} // static std::vector<int> getOrdenacaoMaiorParaMenor(const std::vector<Valor>& a_vetor) {



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
static Valor getMedia(const SmartEnupla<Iterador, Valor> &a_enupla) { return getMedia(a_enupla.getElementos()); } 


static double getMedia_noNAN(const std::vector<double> &a_vetor) {

	const int numero_elementos = int(a_vetor.size());

	if (numero_elementos == 0)
		return NAN;

	int numero_elementos_noNAN = 0;
	double media = 0.0;
	for (int i = 0; i < numero_elementos; i++) {
		if (!std::isnan(a_vetor.at(i))) {
			media += a_vetor.at(i);
			numero_elementos_noNAN++;
		}
	}

	media /= double(numero_elementos_noNAN);

	return media;

} // static double getMedia(const std::vector<double> &a_vetor) {


template<typename Iterador>
static double getMedia_noNAN(const SmartEnupla<Iterador, double> &a_enupla) { return getMedia_noNAN(a_enupla.getElementos()); }


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

	desvio /= double(numero_elementos);

	return std::sqrt(desvio);

} // static Valor getDesvio(const Valor a_media, const std::vector<Valor> &a_vetor) {


template<typename Iterador, typename Valor>
static Valor getDesvio(const SmartEnupla<Iterador, Valor> &a_enupla) { return getDesvio(getMedia(a_enupla), a_enupla); }

template<typename Iterador, typename Valor>
static Valor getDesvio(const Valor a_media, const SmartEnupla<Iterador, Valor> &a_enupla) {	return getDesvio(a_media, a_enupla.getElementos()); } 


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

	if (a_desvio > 0.0)
		assimetria /= std::pow(a_desvio, 3) * numero_elementos;
	else
		assimetria = 0.0;

	if (assimetria < 0.0)
		assimetria = 0.0;

	return assimetria;

} // static Valor getAssimetria(const std::vector<Valor> &a_vetor) {


template<typename Iterador, typename Valor>
static Valor getAssimetria(const SmartEnupla<Iterador, Valor> &a_enupla) { return getAssimetria(getMedia(a_enupla), getDesvio(a_enupla), a_enupla); }

template<typename Iterador, typename Valor>
static Valor getAssimetria(const Valor a_media, const Valor a_desvio, const SmartEnupla<Iterador, Valor> &a_enupla) { return getAssimetria(a_media, a_desvio, a_enupla.getElementos()); }


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
				throw std::invalid_argument("O elemento " + getString(valor) + " inviabiliza um valor real de std::log().");

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
		
		return SmartEnupla<Iterador, Valor>(iteradorInicial, getLog(a_enupla.getElementos()));

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("getLog(a_enupla): \n" + std::string(erro.what())); }

} // static Valor getLog(const Valor a_media, const SmartEnupla<Iterador, Valor> &a_enupla) {


template<typename Valor>
std::vector<Valor> getAutoCovarianciaNaoSazonal(const std::vector<Valor> &a_serie_temporal) {

	try {

		const int numero_elementos = int(a_serie_temporal.size());

		if (numero_elementos == 0)
			return std::vector<Valor>();

		const int ordem_maxima = int(std::sqrt(numero_elementos));

		const Valor media = getMedia(a_serie_temporal);

		std::vector<Valor> auto_covariancia(ordem_maxima + 1, Valor(0));

		// A auto_covariancia no lag 0 é i própria variancia na estação
		for (int lag = 0; lag <= ordem_maxima; lag++) {

			Valor covariancia = Valor(0);

			for (int i = 0; i < numero_elementos; i++) {

				const int i_lag = i - lag;

				if (i_lag >= 0)
					covariancia += (a_serie_temporal.at(i) - media) * (a_serie_temporal.at(i_lag) - media);

			} // for (int i = 0; i < numero_elementos; i++) {

			covariancia /= numero_elementos;

			auto_covariancia.at(lag) = covariancia;

		} // for (int lag = 0; lag <= ordem_maxima; lag++) {

		return auto_covariancia;

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("getAutoCovarianciaNaoSazonal(a_serie_temporal): \n" + std::string(erro.what())); }

} // std::vector<Valor> getAutoCovarianciaNaoSazonal(const std::vector<Valor> &a_serie_temporal) {

template<typename Valor>
SmartEnupla<int, Valor> getAutoCovarianciaNaoSazonal(const SmartEnupla<Periodo, Valor> &a_serie_temporal) { return SmartEnupla<int, Valor>(0, getAutoCovarianciaNaoSazonal(a_serie_temporal.getElementos())); }

template<typename Valor>
SmartEnupla<IdEstacao, SmartEnupla<int, double>> getAutoCovarianciaSazonal(const SmartEnupla<Periodo, Valor> &a_serie_temporal) {

	try {

		if (a_serie_temporal.size() == 0)
			return SmartEnupla<IdEstacao, SmartEnupla<int, Valor>>();

		const Periodo periodo_inicial_serie_temporal = a_serie_temporal.getIteradorInicial();
		const Periodo periodo_final_serie_temporal   = a_serie_temporal.getIteradorFinal();

		const TipoPeriodo tipoPeriodo = a_serie_temporal.getIteradorInicial().getTipoPeriodo();

		const IdEstacao maiorEstacao = Periodo::getMaiorEstacao(tipoPeriodo);

		const int ordem_maxima = int(maiorEstacao) - 1;

		SmartEnupla<IdEstacao, SmartEnupla<int, Valor>> auto_covariancia;

		SmartEnupla <IdEstacao, Valor> medias(IdEstacao_1, std::vector<Valor>(int(maiorEstacao), Valor(0)));
		for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {
			const std::vector<Valor> elementos = a_serie_temporal.getElementos(idEstacao).at(tipoPeriodo);
			medias.setElemento(idEstacao, getMedia(elementos));
		} // for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

		for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

			SmartEnupla<int, Valor> auto_covariancia_estacao(0, std::vector<Valor>(ordem_maxima + 1, Valor(0)));

			// A auto_covariancia no lag 0 é i própria variancia na estação
			for (int lag = 0; lag <= ordem_maxima; lag++) {

				Valor covariancia = Valor(0);

				int numero_elementos = 0;
				for (Periodo periodo = periodo_inicial_serie_temporal; periodo <= periodo_final_serie_temporal; periodo++) {

					if (periodo.getEstacao() == idEstacao) {

						const Periodo periodo_lag = periodo - lag;

						if (periodo_lag >= periodo_inicial_serie_temporal) {

							covariancia += (a_serie_temporal.getElemento(periodo) - medias.getElemento(idEstacao)) * (a_serie_temporal.getElemento(periodo_lag) - medias.getElemento(periodo_lag.getEstacao()));

							numero_elementos++;

						} // if (periodo_lag >= periodo_inicial_serie_temporal) {

					} // if (periodo.getEstacao() == idEstacao) {

				} // for (Periodo periodo = periodo_inicial_serie_temporal; periodo <= periodo_final_serie_temporal; periodo++) {

				covariancia /= numero_elementos;

				auto_covariancia_estacao.at(lag) = covariancia;

			} // for (int lag = 0; lag <= ordem_maxima; lag++) {

			auto_covariancia.addElemento(idEstacao, auto_covariancia_estacao);

		} // for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

		return auto_covariancia;

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("getAutoCovarianciaSazonal(a_serie_temporal): \n" + std::string(erro.what())); }

} // SmartEnupla<IdEstacao, SmartEnupla<int, double>> getAutoCovarianciaSazonal(const SmartEnupla<Periodo, Valor> &a_enupla) {


template<typename Valor>
std::vector<Valor> getAutoCorrelacaoNaoSazonal(const std::vector<Valor> &a_auto_covariancia_sazonal) {

	try {

		const int ordem_maxima = int(a_auto_covariancia_sazonal.size()) - 1;

		if (ordem_maxima < 0)
			return std::vector<Valor>();

		std::vector<Valor> auto_correlacao(ordem_maxima + 1, Valor(0));

		// A auto_correlacao no lag 0 é a própria variancia na estação
		for (int lag = 0; lag <= ordem_maxima; lag++) {
			if (a_auto_covariancia_sazonal.at(0) > 0.0)
				auto_correlacao.at(lag) = a_auto_covariancia_sazonal.at(lag) / std::sqrt(a_auto_covariancia_sazonal.at(0) * a_auto_covariancia_sazonal.at(0));
			else
				auto_correlacao.at(lag) = 0.0;
		}

		return auto_correlacao;

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("getAutoCorrelacaoNaoSazonal(a_auto_covariancia_sazonal): \n" + std::string(erro.what())); }

} // std::vector<Valor> getAutoCorrelacaoNaoSazonal(const std::vector<Valor> &a_auto_covariancia_sazonal) {

template<typename Valor>
SmartEnupla<int, Valor> getAutoCorrelacaoNaoSazonal(const SmartEnupla<int, Valor> &a_auto_covariancia_sazonal) { return SmartEnupla<int, Valor>(0, getAutoCorrelacaoNaoSazonal(a_auto_covariancia_sazonal.getElementos())); }


template<typename Valor>
SmartEnupla<IdEstacao, SmartEnupla<int, Valor>> getAutoCorrelacaoSazonal(const SmartEnupla<IdEstacao, SmartEnupla<int, Valor>> &a_auto_covariancia_sazonal) {

	try {

		if (a_auto_covariancia_sazonal.size() == 0)
			return SmartEnupla<IdEstacao, SmartEnupla<int, Valor>>();

		const IdEstacao maiorEstacao = a_auto_covariancia_sazonal.getIteradorFinal();

		const int ordem_maxima = int(maiorEstacao) - 1;

		SmartEnupla<IdEstacao, SmartEnupla<int, Valor>> auto_correlacao;

		for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

			const Valor auto_covariancia_lag0_idEstacao = a_auto_covariancia_sazonal.at(idEstacao).getElemento(0);

			SmartEnupla<int, Valor> auto_correlacao_estacao(0, std::vector<Valor>(ordem_maxima + 1, Valor(0)));

			// A auto_correlacao no lag 0 é a própria variancia na estação
			for (int lag = 0; lag <= ordem_maxima; lag++) {

				const Valor auto_covariancia_lag_idEstacao = a_auto_covariancia_sazonal.at(idEstacao).getElemento(lag);

				IdEstacao idEstacaoLag = IdEstacao_Nenhum;

				if (IdEstacao(idEstacao - lag) < IdEstacao_1)
					idEstacaoLag = IdEstacao(idEstacao - lag + maiorEstacao);
				else
					idEstacaoLag = IdEstacao(idEstacao - lag);

				const Valor auto_covariancia_lag0_idEstacaoLag = a_auto_covariancia_sazonal.at(idEstacaoLag).getElemento(0);

				if ((auto_covariancia_lag0_idEstacao > 0.0) && (auto_covariancia_lag0_idEstacaoLag > 0.0))
					auto_correlacao_estacao.at(lag) = auto_covariancia_lag_idEstacao / std::sqrt(auto_covariancia_lag0_idEstacao * auto_covariancia_lag0_idEstacaoLag);
				else
					auto_correlacao_estacao.at(lag) = 0.0;

			} // for (int lag = 0; lag <= ordem_maxima; lag++) {

			auto_correlacao.addElemento(idEstacao, auto_correlacao_estacao);

		} // for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

		return auto_correlacao;

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("getAutoCorrelacaoSazonal(a_auto_covariancia_sazonal): \n" + std::string(erro.what())); }

} // SmartEnupla<IdEstacao, SmartEnupla<int, Valor>> getAutoCorrelacaoSazonal(const SmartEnupla<IdEstacao, SmartEnupla<int, Valor>> &a_auto_covariancia_sazonal) {


template<typename Valor>
SmartEnupla<IdEstacao, Valor> getCorrelacaoSazonal(const SmartEnupla<Periodo, Valor> &a_serie_temporal1, const SmartEnupla<Periodo, Valor> &a_serie_temporal2) {

	try {

		if ((a_serie_temporal1.size() == 0) || (a_serie_temporal2.size() == 0))
			return SmartEnupla<IdEstacao, Valor>();

		const Periodo periodo_inicial_serie1 = a_serie_temporal1.getIteradorInicial();
		const Periodo periodo_inicial_serie2 = a_serie_temporal2.getIteradorInicial();

		const Periodo periodo_final_serie1 = a_serie_temporal1.getIteradorFinal();
		const Periodo periodo_final_serie2 = a_serie_temporal2.getIteradorFinal();

		if (periodo_final_serie1 < periodo_inicial_serie2)
			return SmartEnupla<IdEstacao, Valor>();
		else if (periodo_final_serie2 < periodo_inicial_serie1)
			return SmartEnupla<IdEstacao, Valor>();

		Periodo periodo_inicial = periodo_inicial_serie1;
		Periodo periodo_final   = periodo_final_serie1;

		if (periodo_inicial_serie1 > periodo_inicial_serie2)
			periodo_inicial = periodo_inicial_serie1;
		else if (periodo_inicial_serie2 > periodo_inicial_serie1)
			periodo_inicial = periodo_inicial_serie2;

		if (periodo_final_serie1 < periodo_final_serie2)
			periodo_final = periodo_final_serie1;
		else if (periodo_final_serie2 < periodo_final_serie1)
			periodo_final = periodo_final_serie2;

		const TipoPeriodo tipoPeriodo = periodo_inicial.getTipoPeriodo();

		const IdEstacao maiorEstacao = Periodo::getMaiorEstacao(tipoPeriodo);

		SmartEnupla<IdEstacao, Valor> media_serie1(IdEstacao_1, std::vector<Valor>(int(maiorEstacao), Valor(0)));
		SmartEnupla<IdEstacao, Valor> media_serie2(IdEstacao_1, std::vector<Valor>(int(maiorEstacao), Valor(0)));

		SmartEnupla<IdEstacao, Valor> desvio_serie1(IdEstacao_1, std::vector<Valor>(int(maiorEstacao), Valor(0)));
		SmartEnupla<IdEstacao, Valor> desvio_serie2(IdEstacao_1, std::vector<Valor>(int(maiorEstacao), Valor(0)));

		for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {
			const std::vector<Valor> elementos_serie1 = a_serie_temporal1.getElementos(idEstacao).at(tipoPeriodo);
			const std::vector<Valor> elementos_serie2 = a_serie_temporal2.getElementos(idEstacao).at(tipoPeriodo);

			media_serie1.setElemento(idEstacao, getMedia(elementos_serie1));
			media_serie2.setElemento(idEstacao, getMedia(elementos_serie2));

			desvio_serie1.setElemento(idEstacao, getDesvio(media_serie1.getElemento(idEstacao), elementos_serie1));
			desvio_serie2.setElemento(idEstacao, getDesvio(media_serie2.getElemento(idEstacao), elementos_serie2));
		} // for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

		SmartEnupla<IdEstacao, Valor> correlacao(IdEstacao_1, std::vector<Valor>(maiorEstacao, Valor(0)));

		for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

			Valor valor_correlacao = Valor(0);

			int numero_elementos = 0;
			for (Periodo periodo = periodo_inicial; periodo <= periodo_final; periodo++) {

				if (periodo.getEstacao() == idEstacao) {

					valor_correlacao += (a_serie_temporal1.getElemento(periodo) - media_serie1.getElemento(idEstacao)) * (a_serie_temporal2.getElemento(periodo) - media_serie2.getElemento(idEstacao));

					numero_elementos++;

				} // if (periodo.getEstacao() == idEstacao) {

			} // for (Periodo periodo = periodo_inicial; periodo <= periodo_final; periodo++){

			if ((desvio_serie1.getElemento(idEstacao) > 0.0) && (desvio_serie2.getElemento(idEstacao) > 0.0))
				valor_correlacao /= (Valor(numero_elementos) * desvio_serie1.getElemento(idEstacao) * desvio_serie2.getElemento(idEstacao));
			else
				valor_correlacao = 0.0;

			correlacao.setElemento(idEstacao, valor_correlacao);

		} // for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

		return correlacao;

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("getCorrelacaoSazonal(a_serie_temporal1, a_serie_temporal2): \n" + std::string(erro.what())); }

} // SmartEnupla<IdEstacao, Valor> getCorrelacaoSazonal(const SmartEnupla<Periodo, Valor> &a_serie_temporal1, const SmartEnupla<Periodo, Valor> &a_serie_temporal2) {


#endif 