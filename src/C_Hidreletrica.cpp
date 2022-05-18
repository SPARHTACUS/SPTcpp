#include "C_Hidreletrica.h"

#include "C_Dados.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Hidreletrica::Hidreletrica(){
	try { INICIALIZA_SMART_ELEMENTO(Hidreletrica, SMART_ELEMENTO_HIDRELETRICA) }
	catch (const std::exception&erro) { throw std::invalid_argument("Hidreletrica(" + getString(getIdObjeto()) + ")::Hidreletrica(): \n" + std::string(erro.what())); }
}

Hidreletrica::Hidreletrica(const Hidreletrica & instanciaCopiar){
	try {
		COPIA_SMART_ELEMENTO(Hidreletrica, SMART_ELEMENTO_HIDRELETRICA)
	}
	catch (const std::exception&erro) { throw std::invalid_argument("Hidreletrica(" + getString(getIdObjeto()) + ")::Hidreletrica(const Hidreletrica &instanciaCopiar): \n" + std::string(erro.what())); }
}

void Hidreletrica::esvaziar() {
	try {
		EXCLUI_SMART_ELEMENTO(Hidreletrica, SMART_ELEMENTO_HIDRELETRICA)
	}
	catch (const std::exception& erro) { throw std::invalid_argument("Hidreletrica(" + getString(getIdObjeto()) + ")::esvaziar(): \n" + std::string(erro.what())); }
}
Hidreletrica::~Hidreletrica(){
	EXCLUI_SMART_ELEMENTO(Hidreletrica, SMART_ELEMENTO_HIDRELETRICA)
}


void Hidreletrica::validacaoDadosAttComum(){
	try {

		if (getAtributo(AttComumHidreletrica_jusante, IdHidreletrica()) == getIdObjeto())
			throw std::invalid_argument("O id de AttComumHidreletrica_jusante é igual ao proprio id da Hidreletrica " + getString(getIdObjeto()) + ".");

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("Dados::validacaoDadosAttComum(): \n" + std::string(erro.what())); }

} // void Hidreletrica::validacaoDadosAttComum(){



double Hidreletrica::calcularProdutibilidade(const TipoPerdaHidraulica a_tipo_perda_hidraulica, const double a_perda_hidraulica, const double a_fator_producao, const double a_cota) {

	try{

		if (a_cota < 0.0)
			throw std::invalid_argument("Cota menor que 0.0");

		if (a_fator_producao < 0.0)
			throw std::invalid_argument("Fator de Producao menor que 0.0");

		if (a_perda_hidraulica < 0.0)
			throw std::invalid_argument("Perda Hidraulica menor que 0.0");

		if (a_tipo_perda_hidraulica == TipoPerdaHidraulica_percentual) {
			if (a_perda_hidraulica > 1.0)
				throw std::invalid_argument("Perda Hidraulica do tipo " + getFullString(a_tipo_perda_hidraulica) + " menor que 1.0");
		}

		double cota = a_cota;

		if (a_tipo_perda_hidraulica == TipoPerdaHidraulica_percentual)
			cota *= (1.0 - a_perda_hidraulica);

		else if (a_tipo_perda_hidraulica == TipoPerdaHidraulica_metro)
			cota -= a_perda_hidraulica;

		return a_fator_producao * cota;

	} // try{
	catch (const std::exception & erro) { throw std::invalid_argument("Hidreletrica(" + getString(getIdObjeto()) + ")::calcularProdutibilidade(" + getFullString(a_tipo_perda_hidraulica) + "," + getFullString(a_perda_hidraulica) + "," + getFullString(a_fator_producao) + "," + getFullString(a_cota) + "): \n" + std::string(erro.what())); }

} // double Hidreletrica::calcularProdutibilidade(const TipoPerdaHidraulica a_tipo_perda_hidraulica, const double a_perda_hidraulica, const double a_fator_producao, const double a_cota) const{

SmartEnupla<Periodo, SmartEnupla<int, double>> Hidreletrica::calcularProdutibilidade(const TipoPerdaHidraulica a_tipo_perda_hidraulica, const double a_perda_hidraulica, const double a_fator_producao, const SmartEnupla<Periodo, double>& a_horizonte_estudo) {

	try {

		SmartEnupla<Periodo, SmartEnupla<int, double>> coeficiente_QH(a_horizonte_estudo, SmartEnupla<int, double>(1, std::vector<double>(1, 0.0)));

		for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo)) {

			const double canal_fuga_medio = getElementoVetor(AttVetorHidreletrica_canal_fuga_medio, periodo, double());

			const double cota_referencia = getElementoVetor(IdReservatorio_1, AttVetorReservatorio_cota_referencia, periodo, double());

			const double cota = cota_referencia - canal_fuga_medio;

			coeficiente_QH.at(periodo).at(1) = calcularProdutibilidade(a_tipo_perda_hidraulica, a_perda_hidraulica, a_fator_producao, cota);

		} // for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo)){

		return coeficiente_QH;

	} // try{
	catch (const std::exception & erro) { throw std::invalid_argument("Hidreletrica(" + getString(getIdObjeto()) + ")::calcularProdutibilidade(" + getFullString(a_tipo_perda_hidraulica) + "," + getFullString(a_perda_hidraulica) + "," + getFullString(a_fator_producao) + ",a_horizonte_estudo): \n" + std::string(erro.what())); }

}; // SmartEnupla<Periodo, double> Hidreletrica::calcularProdutibilidadeEquivalenteCalc(const TipoPerdaHidraulica a_tipo_perda_hidraulica, const double a_perda_hidraulica, const double a_fator_producao, const SmartEnupla<Periodo, IdEstagio>& a_horizonte_estudo){


double Hidreletrica::getCotaJusante(const double a_vazao_defluente, const Periodo a_periodo) {

	try {

		double cota = 0;

		const IdPolinomioJusante maiorIdPolinomio = getMaiorId(IdPolinomioJusante());
		bool familiaPolinomios = false;
		if (int(maiorIdPolinomio) > 1) {
			for (IdPolinomioJusante idPolinomio = IdPolinomioJusante_1; idPolinomio <= maiorIdPolinomio; idPolinomio++) {
				if (getSizeVetor(idPolinomio, AttVetorPolinomioJusante_altura_jusante_ref) > 0) {
					familiaPolinomios = true;
					break;
				}
			}
		}//if (int(maiorIdPolinomio) > 1) {

		if (familiaPolinomios) {

			//const double cotaUsinaJusante = getElementoVetor(AttVetorHidreletrica_cota_montante_usina_jusante, a_periodo, double());
			const double cotaUsinaJusante = getElementoVetor(IdPolinomioJusante_1, AttVetorPolinomioJusante_altura_jusante_ref, a_periodo, double());
			
			double vazao_defluente_maxima = 0.0;
			for (IdPolinomioJusante idPolinomio = IdPolinomioJusante_1; idPolinomio <= maiorIdPolinomio; idPolinomio++) {
				if (vazao_defluente_maxima <= getAtributo(idPolinomio, AttComumPolinomioJusante_defluencia_maxima, double()))
					vazao_defluente_maxima = getAtributo(idPolinomio, AttComumPolinomioJusante_defluencia_maxima, double());
			}
			if (vazao_defluente_maxima > a_vazao_defluente)
				vazao_defluente_maxima = a_vazao_defluente;

			double h_ref_sup = 1000;
			double h_ref_inf = 0.0;

			for (IdPolinomioJusante idPolinomio = IdPolinomioJusante_1; idPolinomio <= maiorIdPolinomio; idPolinomio++) {
				if (getSizeVetor(idPolinomio, AttVetorPolinomioJusante_altura_jusante_ref) > 0) {
					if ((getElementoVetor(idPolinomio, AttVetorPolinomioJusante_altura_jusante_ref, a_periodo, double()) >= cotaUsinaJusante) && \
						(h_ref_sup >= getElementoVetor(idPolinomio, AttVetorPolinomioJusante_altura_jusante_ref, a_periodo, double())))
						h_ref_sup = getElementoVetor(idPolinomio, AttVetorPolinomioJusante_altura_jusante_ref, a_periodo, double());
					
					if ((getElementoVetor(idPolinomio, AttVetorPolinomioJusante_altura_jusante_ref, a_periodo, double()) >= 0) && \
						(getElementoVetor(idPolinomio, AttVetorPolinomioJusante_altura_jusante_ref, a_periodo, double()) <= cotaUsinaJusante) && \
						(h_ref_inf <= getElementoVetor(idPolinomio, AttVetorPolinomioJusante_altura_jusante_ref, a_periodo, double())))
						h_ref_inf = getElementoVetor(idPolinomio, AttVetorPolinomioJusante_altura_jusante_ref, a_periodo, double());
				}
			}//for (IdPolinomioJusante idPolinomio; idPolinomio <= maiorIdPolinomio; maiorIdPolinomio++) {

			if ((h_ref_sup == 1000) && (h_ref_inf == 0))
				throw std::invalid_argument("Familia da Polinomio jusante não encontrada.");

			double cotaJusante_1 = 0.0;
			double cotaJusante_2 = 0.0;


			for (IdPolinomioJusante idPolinomio = IdPolinomioJusante_1; idPolinomio <= maiorIdPolinomio; idPolinomio++) {
			
				if ((getElementoVetor(idPolinomio, AttVetorPolinomioJusante_altura_jusante_ref, a_periodo, double()) == h_ref_inf) && \
					(vazao_defluente_maxima >= getAtributo(idPolinomio, AttComumPolinomioJusante_defluencia_minima, double())) && \
					(vazao_defluente_maxima <= getAtributo(idPolinomio, AttComumPolinomioJusante_defluencia_maxima, double()))) {

					cotaJusante_1  = getElementoVetor(idPolinomio, AttVetorPolinomioJusante_coeficiente_0, a_periodo, double());
					cotaJusante_1 += getElementoVetor(idPolinomio, AttVetorPolinomioJusante_coeficiente_1, a_periodo, double()) * std::pow(a_vazao_defluente, 1.0);
					cotaJusante_1 += getElementoVetor(idPolinomio, AttVetorPolinomioJusante_coeficiente_2, a_periodo, double()) * std::pow(a_vazao_defluente, 2.0);
					cotaJusante_1 += getElementoVetor(idPolinomio, AttVetorPolinomioJusante_coeficiente_3, a_periodo, double()) * std::pow(a_vazao_defluente, 3.0);
					cotaJusante_1 += getElementoVetor(idPolinomio, AttVetorPolinomioJusante_coeficiente_4, a_periodo, double()) * std::pow(a_vazao_defluente, 4.0);
				}

				if ((getElementoVetor(idPolinomio, AttVetorPolinomioJusante_altura_jusante_ref, a_periodo, double()) == h_ref_sup) && \
					(vazao_defluente_maxima >= getAtributo(idPolinomio, AttComumPolinomioJusante_defluencia_minima, double())) && \
					(vazao_defluente_maxima <= getAtributo(idPolinomio, AttComumPolinomioJusante_defluencia_maxima, double()))) {

					cotaJusante_2 = getElementoVetor(idPolinomio, AttVetorPolinomioJusante_coeficiente_0, a_periodo, double());
					cotaJusante_2 += getElementoVetor(idPolinomio, AttVetorPolinomioJusante_coeficiente_1, a_periodo, double()) * std::pow(a_vazao_defluente, 1.0);
					cotaJusante_2 += getElementoVetor(idPolinomio, AttVetorPolinomioJusante_coeficiente_2, a_periodo, double()) * std::pow(a_vazao_defluente, 2.0);
					cotaJusante_2 += getElementoVetor(idPolinomio, AttVetorPolinomioJusante_coeficiente_3, a_periodo, double()) * std::pow(a_vazao_defluente, 3.0);
					cotaJusante_2 += getElementoVetor(idPolinomio, AttVetorPolinomioJusante_coeficiente_4, a_periodo, double()) * std::pow(a_vazao_defluente, 4.0);
				}

			}//for (IdPolinomioJusante idPolinomio; idPolinomio <= maiorIdPolinomio; maiorIdPolinomio++) {

			if ((cotaJusante_1 == 0.0) && (cotaJusante_2 == 0.0)) {
				cota = 0.1;
			}
				//throw std::invalid_argument("Cota jusante igual a 0.");

			else if (cotaJusante_1 == 0.0) { cota = cotaJusante_2; }
			else if (cotaJusante_2 == 0.0) { cota = cotaJusante_1; }
			else if (cotaJusante_2 == cotaJusante_1) { cota = cotaJusante_1; }
			else if ((cotaJusante_1 != 0.0) && (cotaJusante_2 != 0.0)) {
				cota = cotaJusante_1 + (cotaJusante_2 - cotaJusante_1) * \
					((cotaUsinaJusante - h_ref_inf) / (h_ref_sup - h_ref_inf));
			}

		}//if (familiaPolinomios) {

		else {

			cota = getElementoVetor(IdPolinomioJusante_1, AttVetorPolinomioJusante_coeficiente_0, a_periodo, double());
			cota += getElementoVetor(IdPolinomioJusante_1, AttVetorPolinomioJusante_coeficiente_1, a_periodo, double()) * std::pow(a_vazao_defluente, 1.0);
			cota += getElementoVetor(IdPolinomioJusante_1, AttVetorPolinomioJusante_coeficiente_2, a_periodo, double()) * std::pow(a_vazao_defluente, 2.0);
			cota += getElementoVetor(IdPolinomioJusante_1, AttVetorPolinomioJusante_coeficiente_3, a_periodo, double()) * std::pow(a_vazao_defluente, 3.0);
			cota += getElementoVetor(IdPolinomioJusante_1, AttVetorPolinomioJusante_coeficiente_4, a_periodo, double()) * std::pow(a_vazao_defluente, 4.0);
		}//else {
		if (std::isnan(cota))
			throw std::invalid_argument("Valor NaN.");
		else if (cota < 0.0)
			throw std::invalid_argument("Valor Negativo.");
		else if (cota == 0.0) {
			cota = 0.1;
			//throw std::invalid_argument("Cota jusante igual a 0.");
		}

		return cota;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Hidreletrica(" + getString(getIdObjeto()) + ")::getCotaJusante(" + getFullString(a_vazao_defluente) + "):\n" + std::string(erro.what())); }

}; //double getCotaJusante(const double a_vazao_defluente) {


double Hidreletrica::getCotaJusante_permite_valoresNegativos(const double a_vazao_defluente, const Periodo a_periodo) {

	try {

		double cota = 0;

		const IdPolinomioJusante maiorIdPolinomio = getMaiorId(IdPolinomioJusante());

		if(maiorIdPolinomio != IdPolinomioJusante_1)
			throw std::invalid_argument("O maiorIdPolinomio deve ser IDPolinioJusante_1");

		cota = getElementoVetor(IdPolinomioJusante_1, AttVetorPolinomioJusante_coeficiente_0, a_periodo, double());
		cota += getElementoVetor(IdPolinomioJusante_1, AttVetorPolinomioJusante_coeficiente_1, a_periodo, double()) * pow(a_vazao_defluente, 1.0);
		cota += getElementoVetor(IdPolinomioJusante_1, AttVetorPolinomioJusante_coeficiente_2, a_periodo, double()) * pow(a_vazao_defluente, 2.0);
		cota += getElementoVetor(IdPolinomioJusante_1, AttVetorPolinomioJusante_coeficiente_3, a_periodo, double()) * pow(a_vazao_defluente, 3.0);
		cota += getElementoVetor(IdPolinomioJusante_1, AttVetorPolinomioJusante_coeficiente_4, a_periodo, double()) * pow(a_vazao_defluente, 4.0);

		return cota;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Hidreletrica(" + getString(getIdObjeto()) + ")::getCotaJusante_permite_valoresNegativos(" + getFullString(a_vazao_defluente) + "):\n" + std::string(erro.what())); }

}; //double getCotaJusante(const double a_vazao_defluente) {