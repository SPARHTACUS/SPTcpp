#include "C_VariavelAleatoriaInterna.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static const double grau_liberdade_minimo = 1.1;

VariavelAleatoriaInterna::VariavelAleatoriaInterna(){
	try { INICIALIZA_SMART_ELEMENTO(VariavelAleatoriaInterna, SMART_ELEMENTO_VARIAVEL_ALEATORIA_INTERNA) }
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoriaInterna(" + getString(getIdObjeto()) + ")::VariavelAleatoriaInterna(): \n" + std::string(erro.what())); }
}

VariavelAleatoriaInterna::VariavelAleatoriaInterna(const VariavelAleatoriaInterna &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(VariavelAleatoriaInterna, SMART_ELEMENTO_VARIAVEL_ALEATORIA_INTERNA) }
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoriaInterna(" + getString(getIdObjeto()) + ")::VariavelAleatoriaInterna(const VariavelAleatoriaInterna &instanciaCopiar): \n" + std::string(erro.what())); }
}

void VariavelAleatoriaInterna::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(VariavelAleatoriaInterna, SMART_ELEMENTO_VARIAVEL_ALEATORIA_INTERNA) }
	catch (const std::exception& erro) { throw std::invalid_argument("VariavelAleatoriaInterna(" + getString(getIdObjeto()) + ")::esvaziar(): \n" + std::string(erro.what())); }
}

VariavelAleatoriaInterna::~VariavelAleatoriaInterna(){ EXCLUI_SMART_ELEMENTO(VariavelAleatoriaInterna, SMART_ELEMENTO_VARIAVEL_ALEATORIA_INTERNA) }



void VariavelAleatoriaInterna::gerarTendenciaTemporalMedia(const Periodo a_periodo_final) {

	try {

		const Periodo periodo_inicial = a_periodo_final - int(Periodo::getMaiorEstacao(a_periodo_final.getTipoPeriodo())) + 1;

		SmartEnupla<IdCenario, SmartEnupla<Periodo, double>> tendencia_media;

		tendencia_media.addElemento(IdCenario_1, SmartEnupla<Periodo, double>(periodo_inicial, std::vector<double>(a_periodo_final - periodo_inicial + 1, 0.0)));

		for (Periodo periodo = periodo_inicial; periodo <= a_periodo_final; periodo++) {
			const IdEstacao idEstacao = periodo.getEstacao();
			const double media = getElementoVetor(AttVetorVariavelAleatoriaInterna_media_serie_temporal, idEstacao, double());
			tendencia_media.at(IdCenario_1).setElemento(periodo, media);
		} // for (Periodo periodo = a_periodo_inicial; periodo <= a_periodo_final; periodo++) {

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoriaInterna(" + getString(getIdObjeto()) + ")::gerarTendenciaTemporalMedia(" + getString(a_periodo_final) + "): \n" + std::string(erro.what())); }

} // void VariavelAleatoriaInterna::gerarTendenciaTemporalMedia(const Periodo a_periodo_inicial, const Periodo a_periodo_final){


double VariavelAleatoriaInterna::deslocarComGrauLiberdade(SmartEnupla<Periodo, double>& a_serie, const double a_grau_liberdade){
	try{

		// O grau de liberdade é 0.0 quando não há necessidade de deslocamento (valor > grau_liberdade_minimo)
		// O grau de liberdade é maior ou igual a 0.0 quando há necessidade de deslocamento (valor + grau_liberdade < grau_liberdade_minimo)

		if (a_serie.size() == 0)
			throw std::invalid_argument("O tamanho da serie deve ser maior que 0.");

		const Periodo periodo_inicial = a_serie.getIteradorInicial();
		const Periodo periodo_final   = a_serie.getIteradorFinal();

		if (a_grau_liberdade < 0.0)
			throw std::invalid_argument("O grau de liberdade deve ser maior que 0.0.");

		double grau_liberdade = a_grau_liberdade;

		// Calcula Grau de Liberdade
		for (Periodo periodo = periodo_inicial; periodo <= periodo_final; a_serie.incrementarIterador(periodo)) {

			const double valor = a_serie.getElemento(periodo);

			if (valor + grau_liberdade < grau_liberdade_minimo)
				grau_liberdade = grau_liberdade_minimo - valor;

		} // for (Periodo periodo = periodo_inicial; periodo <= periodo_final; a_serie.incrementarIterador(periodo)) {

		for (Periodo periodo = periodo_inicial; periodo <= periodo_final; a_serie.incrementarIterador(periodo))
			a_serie.setElemento(periodo, a_serie.getElemento(periodo) + grau_liberdade);

		return grau_liberdade;

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoriaInterna(" + getString(getIdObjeto()) + ")::deslocarComGrauLiberdade(a_serie, " + getString(a_grau_liberdade) + "): \n" + std::string(erro.what())); }

} // double VariavelAleatoriaInterna::transformarComGrauLiberdade(const SmartEnupla<Periodo, double>& a_serie, const double a_grau_liberdade){


void VariavelAleatoriaInterna::deslocarSerieComGrauLiberdade(const double a_grau_liberdade) {

	try {

		SmartEnupla<Periodo, double> serie = getVetor(AttVetorVariavelAleatoriaInterna_serie_temporal_transformada, Periodo(), double());

		double grau_liberdade = getAtributo(AttComumVariavelAleatoriaInterna_grau_liberdade, double());
		if (!isnan(a_grau_liberdade))
			grau_liberdade = a_grau_liberdade;

		const double grau_liberdade_transformacao = deslocarComGrauLiberdade(serie, grau_liberdade);

		const double grau_liberdade_ajuste = grau_liberdade_transformacao - grau_liberdade;

		setVetor_forced(AttVetorVariavelAleatoriaInterna_serie_temporal_transformada, serie);

		if (grau_liberdade_ajuste != 0.0) {
			setAtributo(AttComumVariavelAleatoriaInterna_grau_liberdade, grau_liberdade_transformacao);
			if (getSizeVetor(AttVetorVariavelAleatoriaInterna_tendencia_temporal_transformada) > 0)
				deslocarTendenciaComGrauLiberdade(grau_liberdade_ajuste);
		} // if (grau_liberdade_ajuste != 0.0) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("VariavelAleatoriaInterna(" + getString(getIdObjeto()) + ")::deslocarSerieComGrauLiberdade(): \n" + std::string(erro.what())); }

} // void VariavelAleatoriaInterna::transformarSerieComGrauLiberdade(){



void VariavelAleatoriaInterna::deslocarTendenciaComGrauLiberdade(const double a_grau_liberdade) {

	try {

		SmartEnupla<Periodo, double> tendencias = getVetor(AttVetorVariavelAleatoriaInterna_tendencia_temporal_transformada, Periodo(), double());

		double grau_liberdade = getAtributo(AttComumVariavelAleatoriaInterna_grau_liberdade, double());
		if (!isnan(a_grau_liberdade))
			grau_liberdade = a_grau_liberdade;

		const double grau_liberdade_transformacao = deslocarComGrauLiberdade(tendencias, grau_liberdade);

		const double grau_liberdade_ajuste = grau_liberdade_transformacao - grau_liberdade;

		setVetor_forced(AttVetorVariavelAleatoriaInterna_tendencia_temporal_transformada, tendencias);

		// Em caso de diferença, uma nova transformação das tendencias é procedida de maneira a garantir 
		// que todos os cenarios sejam deslocados com o mesmo grau de liberdade: grau_liberdade_final.
		if (grau_liberdade_ajuste != 0.0) {
			setAtributo(AttComumVariavelAleatoriaInterna_grau_liberdade, grau_liberdade_transformacao);
			if (getSizeVetor(AttVetorVariavelAleatoriaInterna_serie_temporal_transformada) > 0)
				deslocarSerieComGrauLiberdade(grau_liberdade_ajuste);
		} // if (grau_liberdade_ajuste != 0.0) {	

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoriaInterna(" + getString(getIdObjeto()) + ")::deslocarTendenciaComGrauLiberdade(): \n" + std::string(erro.what())); }

} // void VariavelAleatoriaInterna::transformarTendenciaComGrauLiberdade(){



void VariavelAleatoriaInterna::calcularEstatisticaSerieTemporal(){

	try {

		const TipoPeriodo tipoPeriodo = getIteradorInicial(AttVetorVariavelAleatoriaInterna_serie_temporal, Periodo()).getTipoPeriodo();

		const IdEstacao maiorEstacao = Periodo::getMaiorEstacao(tipoPeriodo);

		SmartEnupla<IdEstacao, double> media;
		SmartEnupla<IdEstacao, double> desvio;
		SmartEnupla<IdEstacao, double> assimetria;

		for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

			const std::vector<double> dados = getVetor(AttVetorVariavelAleatoriaInterna_serie_temporal, Periodo(), double()).getElementos(idEstacao).at(tipoPeriodo);

			media.addElemento(idEstacao, getMedia(dados));

			desvio.addElemento(idEstacao, getDesvio(media.getElemento(idEstacao), dados));

            assimetria.addElemento(idEstacao, getAssimetria(media.getElemento(idEstacao), desvio.getElemento(idEstacao), dados));

		} // for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

		setVetor(AttVetorVariavelAleatoriaInterna_media_serie_temporal, media);
		setVetor(AttVetorVariavelAleatoriaInterna_desvio_serie_temporal, desvio);
		setVetor(AttVetorVariavelAleatoriaInterna_assimetria_serie_temporal, assimetria);



	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoriaInterna(" + getString(getIdObjeto()) + ")::calcularEstatisticaSerieTemporal(): \n" + std::string(erro.what())); }

} // void VariavelAleatoriaInterna::calcularEstatisticaSerieTemporal(){



void VariavelAleatoriaInterna::calcularEstatisticaSerieTransformada(){

	try {

		const TipoPeriodo tipoPeriodo = getIteradorInicial(AttVetorVariavelAleatoriaInterna_serie_temporal_transformada, Periodo()).getTipoPeriodo();

		const IdEstacao maiorEstacao = Periodo::getMaiorEstacao(tipoPeriodo);

		SmartEnupla<IdEstacao, double> media;
		SmartEnupla<IdEstacao, double> desvio;
		SmartEnupla<IdEstacao, double> assimetria;

		for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

			const std::vector<double> dados = getVetor(AttVetorVariavelAleatoriaInterna_serie_temporal_transformada, Periodo(), double()).getElementos(idEstacao).at(tipoPeriodo);

			media.addElemento(idEstacao, getMedia(dados));

			desvio.addElemento(idEstacao, getDesvio(media.getElemento(idEstacao), dados));

			assimetria.addElemento(idEstacao, getAssimetria(media.getElemento(idEstacao), desvio.getElemento(idEstacao), dados));

		} // for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

		setVetor(AttVetorVariavelAleatoriaInterna_media_serie_transformada, media);
		setVetor(AttVetorVariavelAleatoriaInterna_desvio_serie_transformada, desvio);
		setVetor(AttVetorVariavelAleatoriaInterna_assimetria_serie_transformada, assimetria);

		calcularAutoCovarianciaSerieTransformada();

		calcularAutoCovarianciaNaoSazonalSerieTransformada();

		calcularAutoCorrelacaoSerieTransformada();

		calcularAutoCorrelacaoNaoSazonalSerieTransformada();

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoriaInterna(" + getString(getIdObjeto()) + ")::calcularEstatisticaSerieTransformada(): \n" + std::string(erro.what())); }

} // void VariavelAleatoriaInterna::calcularEstatisticaBasica(){


void VariavelAleatoriaInterna::calcularAutoCovarianciaSerieTransformada(){

	try{

		if (getSizeMatriz(AttMatrizVariavelAleatoriaInterna_auto_covariancia) > 0)
			return;

		setMatriz(AttMatrizVariavelAleatoriaInterna_auto_covariancia, getAutoCovarianciaSazonal(getVetor(AttVetorVariavelAleatoriaInterna_serie_temporal_transformada, Periodo(), double())));

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoriaInterna(" + getString(getIdObjeto()) + ")::calcularAutoCovarianciaSerieTransformada(): \n" + std::string(erro.what())); }

} // void VariavelAleatoriaInterna::calcularAutoCovarianciaSerieTransformada(){


void VariavelAleatoriaInterna::calcularAutoCovarianciaNaoSazonalSerieTransformada(){

	try {

		if (getSizeVetor(AttVetorVariavelAleatoriaInterna_auto_covariancia) > 0)
			return;

		setVetor(AttVetorVariavelAleatoriaInterna_auto_covariancia, getAutoCovarianciaNaoSazonal(getVetor(AttVetorVariavelAleatoriaInterna_serie_temporal_transformada, Periodo(), double())));

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoriaInterna(" + getString(getIdObjeto()) + ")::calcularAutoCovarianciaSerieTransformada(): \n" + std::string(erro.what())); }

} // void VariavelAleatoriaInterna::calcularAutoCovarianciaNaoSerieTransformada(){


void VariavelAleatoriaInterna::calcularAutoCorrelacaoSerieTransformada() {

	try {

		if (getSizeMatriz(AttMatrizVariavelAleatoriaInterna_auto_correlacao) > 0)
			return;

		setMatriz(AttMatrizVariavelAleatoriaInterna_auto_correlacao, getAutoCorrelacaoSazonal(getMatriz(AttMatrizVariavelAleatoriaInterna_auto_covariancia, IdEstacao(), int(), double())));

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoriaInterna(" + getString(getIdObjeto()) + ")::calcularAutoCorrelacaoSerieTransformada(): \n" + std::string(erro.what())); }

} // void VariavelAleatoriaInterna::calcularAutoCorrelacaoSerieTransformada(){


void VariavelAleatoriaInterna::calcularAutoCorrelacaoNaoSazonalSerieTransformada() {

	try {

		if (getSizeVetor(AttVetorVariavelAleatoriaInterna_auto_correlacao) > 0)
			return;

		setVetor(AttVetorVariavelAleatoriaInterna_auto_correlacao, getAutoCorrelacaoNaoSazonal(getVetor(AttVetorVariavelAleatoriaInterna_auto_covariancia, int(), double())));

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoriaInterna(" + getString(getIdObjeto()) + ")::calcularAutoCorrelacaoNaoSazonalSerieTransformada(): \n" + std::string(erro.what())); }

} // void VariavelAleatoriaInterna::calcularAutoCorrelacaoNaoSerieTransformada(){



double VariavelAleatoriaInterna::calcularRealizacao(const double a_realizacao_variavel_aleatoria, const Periodo a_periodo){

	try{

		const double realizacao_interna = a_realizacao_variavel_aleatoria * getElementoVetor(AttVetorVariavelAleatoriaInterna_coeficiente_participacao, a_periodo, double()) - getAtributo(AttComumVariavelAleatoriaInterna_grau_liberdade, double());

		return realizacao_interna;

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoriaInterna(" + getString(getIdObjeto()) + ")::calcularRealizacao(" + getFullString(a_realizacao_variavel_aleatoria) + "," + getFullString(a_periodo) + "): \n" + std::string(erro.what())); }

} // double VariavelAleatoriaInterna::calcularRealizacao(const double a_realizacao_variavel_aleatoria){


double VariavelAleatoriaInterna::calcularRealizacaoVariavelAleatoriaParaValor(const double a_realizacao_variavel_aleatoria_interna, const Periodo a_periodo){

	try {

		const double realizacao_variavel_aleatoria = (a_realizacao_variavel_aleatoria_interna + getAtributo(AttComumVariavelAleatoriaInterna_grau_liberdade, double())) / getElementoVetor(AttVetorVariavelAleatoriaInterna_coeficiente_participacao, a_periodo, double());

		return realizacao_variavel_aleatoria;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("calcularRealizacaoVariavelAleatoriaParaValor(" + getString(getIdObjeto()) + ")::calcularRealizacaoVariavelAleatoriaParaValor(" + getFullString(a_realizacao_variavel_aleatoria_interna) + "," + getFullString(a_periodo) + "): \n" + std::string(erro.what())); }

} // double VariavelAleatoriaInterna::calcularRealizacaoVariavelAleatoriaParaValor(const double a_valor, const Periodo a_periodo){


void VariavelAleatoriaInterna::addRealizacao(const double a_realizacao_variavel_aleatoria, const IdCenario a_idCenario, const Periodo a_periodo){

	try {

		addElemento(AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral, a_periodo, a_idCenario, calcularRealizacao(a_realizacao_variavel_aleatoria, a_periodo));

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoriaInterna(" + getString(getIdObjeto()) + ")::addRealizacao(" + getFullString(a_realizacao_variavel_aleatoria) + "," + getFullString(a_idCenario) + "," + getFullString(a_periodo) + "): \n" + std::string(erro.what())); }

} // void VariavelAleatoriaInterna::addRealizacao(const double a_realizacao_variavel_aleatoria, const IdCenario a_idCenario, const Periodo a_periodo){


void VariavelAleatoriaInterna::setRealizacao(const double a_realizacao_variavel_aleatoria, const IdCenario a_idCenario, const Periodo a_periodo) {

	try {

		setElemento(AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral, a_periodo, a_idCenario, calcularRealizacao(a_realizacao_variavel_aleatoria, a_periodo));

	} // try{
	catch (const std::exception & erro) { throw std::invalid_argument("VariavelAleatoriaInterna(" + getString(getIdObjeto()) + ")::setRealizacao(" + getFullString(a_realizacao_variavel_aleatoria) + "," + getFullString(a_idCenario) + "," + getFullString(a_periodo) + "): \n" + std::string(erro.what())); }

} // void VariavelAleatoriaInterna::addRealizacao(const double a_realizacao_variavel_aleatoria, const IdCenario a_idCenario, const Periodo a_periodo){


void VariavelAleatoriaInterna::setRealizacaoFromTendencia(const IdCenario a_idCenario, const Periodo a_periodo) {

	try {

		if (getSizeVetor(AttVetorVariavelAleatoriaInterna_tendencia_temporal) > 0)
			setElemento(AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral, a_periodo, a_idCenario, getElementoVetor(AttVetorVariavelAleatoriaInterna_tendencia_temporal, a_periodo, double()));

	} // try{
	catch (const std::exception & erro) { throw std::invalid_argument("VariavelAleatoriaInterna(" + getString(getIdObjeto()) + ")::setRealizacaoFromTendencia(" + getFullString(a_idCenario) + "," + getFullString(a_periodo) + "): \n" + std::string(erro.what())); }

} // void VariavelAleatoriaInterna::addRealizacao(const double a_realizacao_variavel_aleatoria, const IdCenario a_idCenario, const Periodo a_periodo){