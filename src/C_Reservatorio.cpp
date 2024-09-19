#include "C_Reservatorio.h"
#include "C_Dados.h"
#include "C_OperacoesMatematicas.h"

#include <cmath>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Reservatorio::Reservatorio() {
	try { INICIALIZA_SMART_ELEMENTO(Reservatorio, SMART_ELEMENTO_RESERVATORIO) }
	catch (const std::exception&erro) { throw std::invalid_argument("Reservatorio::Reservatorio(): \n" + std::string(erro.what())); }
}

Reservatorio::Reservatorio(const Reservatorio & instanciaCopiar) {
	try { COPIA_SMART_ELEMENTO(Reservatorio, SMART_ELEMENTO_RESERVATORIO) }
	catch (const std::exception&erro) { throw std::invalid_argument("Reservatorio::Reservatorio(const Reservatorio &instanciaCopiar): \n" + std::string(erro.what())); }
}

void Reservatorio::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(Reservatorio, SMART_ELEMENTO_RESERVATORIO) }
	catch (const std::exception& erro) { throw std::invalid_argument("Reservatorio::esvaziar(): \n" + std::string(erro.what())); }
}

Reservatorio::~Reservatorio() {
	EXCLUI_SMART_ELEMENTO(Reservatorio, SMART_ELEMENTO_RESERVATORIO)
}


void Reservatorio::calculaAproximacaoLinearEvaporacao( const Periodo a_periodo) {
	try {

		const double volumeMinimo = getElementoVetor(AttVetorReservatorio_volume_minimo, a_periodo, double());
		const double volumeMaximo = getElementoVetor(AttVetorReservatorio_volume_maximo, a_periodo, double());
	
		calculaAproximacaoLinearEvaporacao(volumeMinimo, volumeMaximo, a_periodo);
	
	} // try{
	catch (const std::exception & erro) { throw std::invalid_argument("Reservatorio::calculaAproximacaoLinearEvaporacao(): \n" + std::string(erro.what())); }
}


void Reservatorio::calculaAproximacaoLinearEvaporacao(const double a_volumeUtil, const Periodo a_periodo) {
	try {

		const double volumeMinimo = getElementoVetor(AttVetorReservatorio_volume_minimo, a_periodo, double());

		calculaAproximacaoLinearEvaporacao(volumeMinimo, double(volumeMinimo + a_volumeUtil), a_periodo);

	} // try{
	catch (const std::exception & erro) { throw std::invalid_argument("Reservatorio::calculaAproximacaoLinearEvaporacao(): \n" + std::string(erro.what())); }
}

void Reservatorio::calculaAproximacaoLinearEvaporacao(const double a_volumeMinimo, const double a_volumeMaximo, const Periodo a_periodo, bool a_regra_esp) {
	try {

		const int numeroPontos = 100;
		std::vector<double> evaporacaoMensal;
		std::vector<double> gradeVolume;
		std::vector<double> coef;		

		double coefEvaporacao = 0.0;


		
		if (a_regra_esp) {
			if (getSizeVetor(AttVetorReservatorio_poli_cota_area_0) > 0) {			
				if (a_periodo == getIteradorInicial(AttVetorReservatorio_poli_cota_area_0, Periodo())) {
					const Periodo period_min_end = Periodo(TipoPeriodo_minuto, a_periodo + 1) - 1;
					const Periodo period_month_end = Periodo(TipoPeriodo_mensal, period_min_end.getMes(), period_min_end.getAno());
					coefEvaporacao = (getElementoVetor(AttVetorReservatorio_evaporacao, period_month_end.getMes(), double()) / (3.6 * 730));
				}
				else {
					coefEvaporacao = (getElementoVetor(AttVetorReservatorio_evaporacao, a_periodo.getMes(), double()) / (3.6 * 730));
				}
			}
			else
				a_regra_esp = false;
		}
		if (!a_regra_esp) {
			const Periodo period_min_end = Periodo(TipoPeriodo_minuto, a_periodo + 1) - 1;
			const Periodo period_month_end = Periodo(TipoPeriodo_mensal, period_min_end.getMes(), period_min_end.getAno());
			SmartEnupla<Periodo, double> periods_month;

			Periodo period_month = Periodo(TipoPeriodo_mensal, a_periodo.getMes(), a_periodo.getAno());
			periods_month.addElemento(period_month, a_periodo.sobreposicao(period_month));

			if (period_month < period_month_end) {
				for (Periodo period = period_month + 1; period <= period_month_end; period++)
					periods_month.addElemento(period, a_periodo.sobreposicao(period));
			}

			for (Periodo period = periods_month.getIteradorInicial(); period <= periods_month.getIteradorFinal(); periods_month.incrementarIterador(period))
				coefEvaporacao += (getElementoVetor(AttVetorReservatorio_evaporacao, period.getMes(), double()) / (3.6 * double(period.getHoras()))) * periods_month.at(period);
		}


		// EVAPORAÇÃO = 0 QUANDO O COEFICIENTE DE EVAPORAÇÃO É NULO  
		if (coefEvaporacao == 0) {
			coef.push_back(0);
			coef.push_back(0);
			coef.push_back(0);
		} // else if (a_volumeMaximo == 0) {


		// CALCULA OS COEFICIENTES QUANDO VOLUME UTIL MAIOR QUE 0
		else if (a_volumeMaximo > a_volumeMinimo) {

			for (int i = 0; i < numeroPontos; i++) {

				gradeVolume.push_back(  (a_volumeMaximo - a_volumeMinimo) * double(double(i) / double(numeroPontos)));

				const double area = getAreaFromVolume(a_periodo, (gradeVolume.at(i)) + a_volumeMinimo);

				const double evap = area * coefEvaporacao;

				evaporacaoMensal.push_back(evap);

			} // for (int i = 0; i < numeroPontos; i++) {

			coef = getMinimosQuadrados(gradeVolume, evaporacaoMensal);

			if  (coef.at(1) < 1E-7)                      { coef.at(1) = 0; }
			if ((coef.at(1) <    0) && (coef.at(0) > 0)) { coef.at(0) = 0; }
			if ((coef.at(1) >    0) && (coef.at(0) < 0)) { coef.at(0) = 0; }

		}// else if (a_volumeMaximo > a_volumeMinimo) {		

		// EVAPORAÇÃO COSNTANTE PARA VOLUME UTIL = 0  
		else if (a_volumeMinimo == a_volumeMaximo) {			
			coef.push_back((getAreaFromVolume(a_periodo, a_volumeMinimo) * coefEvaporacao));
			coef.push_back(0);
			coef.push_back(0);
		}//else if (a_volumeMinimo == a_volumeMaximo) {


		// EVAPORAÇÃO = 0 PARA VOLUME MAXIMO = 0 
		else if (a_volumeMaximo == 0) {
			coef.push_back(0);
			coef.push_back(0);
			coef.push_back(0);
		} // else if (a_volumeMaximo == 0) {


		// COEFICIENTE A0      => coef[0]
		// COEFICIENTE A1      => coef[1]
		// ERRO DE APROXIMAÇÃO => coef[2]
		addElemento(AttVetorReservatorio_coef_linear_evaporacao_0, a_periodo, coef.at(0));
		addElemento(AttVetorReservatorio_coef_linear_evaporacao_1, a_periodo, coef.at(1));

	} // try{
	catch (const std::exception & erro) { throw std::invalid_argument("Reservatorio::calculaAproximacaoLinearEvaporacao(" + getString(a_volumeMinimo) + "," + getString(a_volumeMaximo) + "," + getString(a_periodo) + "): \n" + std::string(erro.what())); }
}


void Reservatorio::calculaAproximacaoLinearCotaMontante(const Periodo a_periodo) {
	try {

		const double volumeMinimo = getElementoVetor(AttVetorReservatorio_volume_minimo, a_periodo, double());
		const double volumeMaximo = getElementoVetor(AttVetorReservatorio_volume_maximo, a_periodo, double());

		calculaAproximacaoLinearCotaMontante(volumeMinimo, volumeMaximo, a_periodo);

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Reservatorio::calculaAproximacaoLinearCotaMontante(): \n" + std::string(erro.what())); }
}


void Reservatorio::calculaAproximacaoLinearCotaMontante(const double a_volumeUtil, const Periodo a_periodo) {
	try {

		const double volumeMinimo = getElementoVetor(AttVetorReservatorio_volume_minimo, a_periodo, double());

		calculaAproximacaoLinearCotaMontante(volumeMinimo, double(volumeMinimo + a_volumeUtil), a_periodo);

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Reservatorio::calculaAproximacaoLinearCotaMontante(): \n" + std::string(erro.what())); }
}

void Reservatorio::calculaAproximacaoLinearCotaMontante(const double a_volumeMinimo, const double a_volumeMaximo, const Periodo a_periodo) {
	try {

		const int numeroPontos = 100;
		std::vector<double> gradeCota;
		std::vector<double> gradeVolume;
		std::vector<double> coef;


		// CALCULA OS COEFICIENTES QUANDO VOLUME UTIL MAIOR QUE 0
		if (a_volumeMaximo > a_volumeMinimo) {

			for (int i = 0; i < numeroPontos; i++) {

				gradeVolume.push_back((a_volumeMaximo - a_volumeMinimo) * double(double(i) / double(numeroPontos)));

				const double cota = getCota(a_periodo, (gradeVolume.at(i)) + a_volumeMinimo);

				gradeCota.push_back(cota);

			} // for (int i = 0; i < numeroPontos; i++) {

			coef = getMinimosQuadrados(gradeVolume, gradeCota);

			if (coef.at(1) < 1E-7) { coef.at(1) = 0; }
			if ((coef.at(1) < 0) && (coef.at(0) > 0)) { coef.at(0) = 0; }
			if ((coef.at(1) > 0) && (coef.at(0) < 0)) { coef.at(0) = 0; }

		}// else if (a_volumeMaximo > a_volumeMinimo) {		

		// EVAPORAÇÃO COSNTANTE PARA VOLUME UTIL = 0  
		else if (a_volumeMinimo == a_volumeMaximo) {
			coef.push_back((getCota(a_periodo, a_volumeMinimo)));
			coef.push_back(0);
			coef.push_back(0);
		}//else if (a_volumeMinimo == a_volumeMaximo) {


		// EVAPORAÇÃO = 0 PARA VOLUME MAXIMO = 0 
		else if (a_volumeMaximo == 0) {
			coef.push_back(0);
			coef.push_back(0);
			coef.push_back(0);
		} // else if (a_volumeMaximo == 0) {


		// COEFICIENTE A0      => coef[0]
		// COEFICIENTE A1      => coef[1]
		// ERRO DE APROXIMAÇÃO => coef[2]
		addElemento(AttVetorReservatorio_coef_linear_cota_montante_0, a_periodo, coef.at(0));
		addElemento(AttVetorReservatorio_coef_linear_cota_montante_1, a_periodo, coef.at(1));

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Reservatorio::calculaAproximacaoLinearCotaMontante(" + getString(a_volumeMinimo) + "," + getString(a_volumeMaximo) + "," + getString(a_periodo) + "): \n" + std::string(erro.what())); }
}




void Reservatorio::validacaoDadosAttComum() {
	try {

		const double volume_minimo = getAtributo(AttComumReservatorio_volume_minimo, double());
		const double volume_maximo = getAtributo(AttComumReservatorio_volume_maximo, double());

		if (volume_maximo < volume_minimo)
			throw std::invalid_argument(getString(AttComumReservatorio_volume_minimo) + " maior que " + getString(AttComumReservatorio_volume_maximo) + ".");

		//const double volume_util = volume_maximo - volume_minimo;

		//const double percentual_volume_inicial = getAtributo(AttComumReservatorio_percentual_volume_util_inicial, double());

		//const double volume_util_inicial = volume_util * percentual_volume_inicial;

		//setAtributo(AttComumReservatorio_volume_util_inicial, volume_util_inicial);

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("Reservatorio::validaReservatorioEmHidreletrica(): \n" + std::string(erro.what())); }
} // void Reservatorio::validacaoDadosAttComum(){



double Reservatorio::getCota(const Periodo a_periodo, const double a_volume) const{
	try {
		double cota = getElementoVetor(AttVetorReservatorio_poli_cota_volume_0, a_periodo, double());

		cota += getElementoVetor(AttVetorReservatorio_poli_cota_volume_1, a_periodo, double()) * std::pow(a_volume, 1.0);
		cota += getElementoVetor(AttVetorReservatorio_poli_cota_volume_2, a_periodo, double()) * std::pow(a_volume, 2.0);
		cota += getElementoVetor(AttVetorReservatorio_poli_cota_volume_3, a_periodo, double()) * std::pow(a_volume, 3.0);
		cota += getElementoVetor(AttVetorReservatorio_poli_cota_volume_4, a_periodo, double()) * std::pow(a_volume, 4.0);

		if (std::isnan(cota))
			throw std::invalid_argument("Valor NaN.");
		else if (cota < 0.0)
			throw std::invalid_argument("Valor Negativo.");

		return cota;
	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("Reservatorio::getCota(" + getString(a_volume) + "): \n" + std::string(erro.what())); }
} // double Reservatorio::getCota(const Periodo a_periodo, const double a_volume) const{

double Reservatorio::getCota(const double a_volume) const {
	try {
		double cota = getAtributo(AttComumReservatorio_poli_cota_volume_0, double());

		cota += getAtributo(AttComumReservatorio_poli_cota_volume_1, double()) * std::pow(a_volume, 1.0);
		cota += getAtributo(AttComumReservatorio_poli_cota_volume_2, double()) * std::pow(a_volume, 2.0);
		cota += getAtributo(AttComumReservatorio_poli_cota_volume_3, double()) * std::pow(a_volume, 3.0);
		cota += getAtributo(AttComumReservatorio_poli_cota_volume_4, double()) * std::pow(a_volume, 4.0);

		if (std::isnan(cota))
			throw std::invalid_argument("Valor NaN.");
		else if (cota < 0.0)
			throw std::invalid_argument("Valor Negativo.");

		return cota;
	} // try{
	catch (const std::exception & erro) { throw std::invalid_argument("Reservatorio::getCota(" + getString(a_volume) + "): \n" + std::string(erro.what())); }
} // double Reservatorio::getCota(const Periodo a_periodo, const double a_volume) const{

double Reservatorio::getCotaMedia(const Periodo a_periodo, const double a_volume_minimo, double a_volume_maximo) const{
	try {

		// Se os volumes são iguais, a Cota média é igual a própria Cota
		if (a_volume_maximo == a_volume_minimo)
			return getCota(a_periodo, a_volume_maximo);

		double cota = 0.0;

		cota += getElementoVetor(AttVetorReservatorio_poli_cota_volume_0, a_periodo, double()) * std::pow(a_volume_maximo, 1.0) / 1.0;
		cota -= getElementoVetor(AttVetorReservatorio_poli_cota_volume_0, a_periodo, double()) * std::pow(a_volume_minimo, 1.0) / 1.0;

		cota += getElementoVetor(AttVetorReservatorio_poli_cota_volume_1, a_periodo, double()) * std::pow(a_volume_maximo, 2.0) / 2.0;
		cota -= getElementoVetor(AttVetorReservatorio_poli_cota_volume_1, a_periodo, double()) * std::pow(a_volume_minimo, 2.0) / 2.0;

		cota += getElementoVetor(AttVetorReservatorio_poli_cota_volume_2, a_periodo, double()) * std::pow(a_volume_maximo, 3.0) / 3.0;
		cota -= getElementoVetor(AttVetorReservatorio_poli_cota_volume_2, a_periodo, double()) * std::pow(a_volume_minimo, 3.0) / 3.0;

		cota += getElementoVetor(AttVetorReservatorio_poli_cota_volume_3, a_periodo, double()) * std::pow(a_volume_maximo, 4.0) / 4.0;
		cota -= getElementoVetor(AttVetorReservatorio_poli_cota_volume_3, a_periodo, double()) * std::pow(a_volume_minimo, 4.0) / 4.0;

		cota += getElementoVetor(AttVetorReservatorio_poli_cota_volume_4, a_periodo, double()) * std::pow(a_volume_maximo, 5.0) / 5.0;
		cota -= getElementoVetor(AttVetorReservatorio_poli_cota_volume_4, a_periodo, double()) * std::pow(a_volume_minimo, 5.0) / 5.0;

		cota /= (a_volume_maximo - a_volume_minimo);

		if (std::isnan(cota))
			throw std::invalid_argument("Valor NaN.");
		else if (cota < 0.0)
			throw std::invalid_argument("Valor Negativo.");

		return cota;

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("Reservatorio::getCotaMedia(" + getString(a_volume_minimo) + "," + getString(a_volume_maximo) + "): \n" + std::string(erro.what())); }
} // double Reservatorio::getCotaMedia(const Periodo a_periodo, const double a_volume_minimo, double a_volume_maximo) const{


double Reservatorio::getCotaMedia(const double a_volume_minimo, double a_volume_maximo) const {
	try {

		// Se os volumes são iguais, a Cota média é igual a própria Cota
		if (a_volume_maximo == a_volume_minimo)
			return getCota(a_volume_maximo);

		double cota = 0.0;

		cota += getAtributo(AttComumReservatorio_poli_cota_volume_0, double()) * std::pow(a_volume_maximo, 1.0) / 1.0;
		cota -= getAtributo(AttComumReservatorio_poli_cota_volume_0, double()) * std::pow(a_volume_minimo, 1.0) / 1.0;

		cota += getAtributo(AttComumReservatorio_poli_cota_volume_1, double()) * std::pow(a_volume_maximo, 2.0) / 2.0;
		cota -= getAtributo(AttComumReservatorio_poli_cota_volume_1, double()) * std::pow(a_volume_minimo, 2.0) / 2.0;

		cota += getAtributo(AttComumReservatorio_poli_cota_volume_2, double()) * std::pow(a_volume_maximo, 3.0) / 3.0;
		cota -= getAtributo(AttComumReservatorio_poli_cota_volume_2, double()) * std::pow(a_volume_minimo, 3.0) / 3.0;

		cota += getAtributo(AttComumReservatorio_poli_cota_volume_3, double()) * std::pow(a_volume_maximo, 4.0) / 4.0;
		cota -= getAtributo(AttComumReservatorio_poli_cota_volume_3, double()) * std::pow(a_volume_minimo, 4.0) / 4.0;

		cota += getAtributo(AttComumReservatorio_poli_cota_volume_4, double()) * std::pow(a_volume_maximo, 5.0) / 5.0;
		cota -= getAtributo(AttComumReservatorio_poli_cota_volume_4, double()) * std::pow(a_volume_minimo, 5.0) / 5.0;

		cota /= (a_volume_maximo - a_volume_minimo);

		if (std::isnan(cota))
			throw std::invalid_argument("Valor NaN.");
		else if (cota < 0.0)
			throw std::invalid_argument("Valor Negativo.");

		return cota;

	} // try{
	catch (const std::exception & erro) { throw std::invalid_argument("Reservatorio::getCotaMedia(" + getString(a_volume_minimo) + "," + getString(a_volume_maximo) + "): \n" + std::string(erro.what())); }
} // double Reservatorio::getCotaMedia(const Periodo a_periodo, const double a_volume_minimo, double a_volume_maximo) const{

double Reservatorio::getAreaFromCota(const Periodo a_periodo, const double a_cota) const{
	try {

		double area = getElementoVetor(AttVetorReservatorio_poli_cota_area_0, a_periodo, double());

		area += getElementoVetor(AttVetorReservatorio_poli_cota_area_1, a_periodo, double()) * a_cota;
		area += getElementoVetor(AttVetorReservatorio_poli_cota_area_2, a_periodo, double()) * a_cota * a_cota;
		area += getElementoVetor(AttVetorReservatorio_poli_cota_area_3, a_periodo, double()) * a_cota * a_cota * a_cota;
		area += getElementoVetor(AttVetorReservatorio_poli_cota_area_4, a_periodo, double()) * a_cota * a_cota * a_cota * a_cota;

		if (std::isnan(area))
			throw std::invalid_argument("Valor NaN.");
		else if (area < 0.0)
			throw std::invalid_argument("Valor Negativo.");

		return area;

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("Reservatorio::getAreaFromCota(" + getString(a_cota) + "): \n" + std::string(erro.what())); }
} // double Reservatorio::getAreaFromCota(const Periodo a_periodo, const double a_cota) const{



double Reservatorio::getAreaFromVolume(const Periodo a_periodo, const double a_volume) const{
	try {

		const double cota = getCota(a_periodo, a_volume);
		return getAreaFromCota(a_periodo, cota);

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("Reservatorio::getAreaFromVolume(" + getString(a_volume) + "): \n" + std::string(erro.what())); }
} // double Reservatorio::getAreaFromVolume(const Periodo a_periodo, const double a_volume) const{




bool Reservatorio::isReservatorioDeRegularizacao(const Periodo a_periodo, const double a_volume_util_operacional, const double a_turbinamento_maximo){

	try {

		if (a_volume_util_operacional < 0.0)
			throw std::invalid_argument("Volume util operacional invalido");
		else if (a_turbinamento_maximo < 0.0)
			throw std::invalid_argument("Turbinamento maximo invalido");
		else if (a_volume_util_operacional == 0.0)
			return false;
		else if (a_turbinamento_maximo == 0.0)
			return true;

		return true;

		const double volume_turbinamento_maximo = conversao_vazao_m3s_em_volume_hm3(a_periodo, a_turbinamento_maximo);

		if (volume_turbinamento_maximo > a_volume_util_operacional * 1.1)
			return false;
		else
			return true;

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("Reservatorio::isReservatorioDeRegularizacao(" + getString(a_periodo) + "," + getString(a_volume_util_operacional) + "," + getString(a_turbinamento_maximo) + "): \n" + std::string(erro.what())); }

} // bool Reservatorio::isReservatorioDeRegularizacao(const Periodo a_periodo, const double a_volume_util_operacional){

