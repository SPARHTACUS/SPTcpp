#include "C_VariavelAleatoria.h"

#include "ls_zollen.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static const double grau_liberdade_minimo = 1.1;

VariavelAleatoria::VariavelAleatoria(){
	try { INICIALIZA_SMART_ELEMENTO(VariavelAleatoria, SMART_ELEMENTO_VARIAVEL_ALEATORIA) }
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::VariavelAleatoria(): \n" + std::string(erro.what())); }
}

VariavelAleatoria::VariavelAleatoria(const VariavelAleatoria &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(VariavelAleatoria, SMART_ELEMENTO_VARIAVEL_ALEATORIA) }
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::VariavelAleatoria(const VariavelAleatoria &instanciaCopiar): \n" + std::string(erro.what())); }
}

void VariavelAleatoria::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(VariavelAleatoria, SMART_ELEMENTO_VARIAVEL_ALEATORIA) }
	catch (const std::exception& erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::esvaziar(): \n" + std::string(erro.what())); }
}

VariavelAleatoria::~VariavelAleatoria(){ EXCLUI_SMART_ELEMENTO(VariavelAleatoria, SMART_ELEMENTO_VARIAVEL_ALEATORIA) }


void VariavelAleatoria::addSerieTemporalVariavelAleatoriaInterna(const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const SmartEnupla<Periodo, double>& a_serie_temporal){

	try {

		vetorVariavelAleatoriaInterna.att(a_idVariavelAleatoriaInterna).setAtributo(AttComumVariavelAleatoriaInterna_grau_liberdade, 0.0);

		vetorVariavelAleatoriaInterna.att(a_idVariavelAleatoriaInterna).setVetor_forced(AttVetorVariavelAleatoriaInterna_serie_temporal,              a_serie_temporal);
		vetorVariavelAleatoriaInterna.att(a_idVariavelAleatoriaInterna).setVetor_forced(AttVetorVariavelAleatoriaInterna_serie_temporal_transformada, a_serie_temporal);

		vetorVariavelAleatoriaInterna.att(a_idVariavelAleatoriaInterna).calcularEstatisticaSerieTemporal();

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::addSerieTemporalVariavelAleatoriaInterna(" + getFullString(a_idVariavelAleatoriaInterna) + ",a_serie_temporal): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::addSerieTemporalVariavelAleatoriaInterna(const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const SmartEnupla<Periodo, double>& a_serie_temporal){


void VariavelAleatoria::addTendenciaTemporalVariavelAleatoriaInterna(const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>& a_tendencia_temporal){

	try{

		vetorVariavelAleatoriaInterna.att(a_idVariavelAleatoriaInterna).setMatriz_forced(AttMatrizVariavelAleatoriaInterna_tendencia_temporal,              a_tendencia_temporal);
		vetorVariavelAleatoriaInterna.att(a_idVariavelAleatoriaInterna).setMatriz_forced(AttMatrizVariavelAleatoriaInterna_tendencia_temporal_transformada, a_tendencia_temporal);

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::addTendenciaTemporalVariavelAleatoriaInterna(" + getFullString(a_idVariavelAleatoriaInterna) + ",a_tendencia_temporal): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::addTendenciaTemporalVariavelAleatoriaInterna(const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>& a_tendencia_temporal){



void VariavelAleatoria::gerarTendenciaTemporalMediaVariavelAleatoriaInterna(const Periodo a_periodo_final){

	const IdVariavelAleatoriaInterna maiorIdVariavelAleatoriaInterna = getMaiorId(IdVariavelAleatoriaInterna());

	if (maiorIdVariavelAleatoriaInterna == IdVariavelAleatoriaInterna_Nenhum)
		throw std::invalid_argument("Nenhuma variavel aleatoria interna foi instanciada.");

	for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= maiorIdVariavelAleatoriaInterna; idVariavelAleatoriaInterna++)
		vetorVariavelAleatoriaInterna.att(idVariavelAleatoriaInterna).gerarTendenciaTemporalMedia(a_periodo_final);

} // void VariavelAleatoria::gerarTendenciaTemporalMediaVariavelAleatoriaInterna(const Periodo a_periodo_final){


void VariavelAleatoria::setCenariosInternos(const AttMatrizVariavelAleatoriaInterna a_attMatrizVariavelAleatoriaInterna, const SmartEnupla<IdVariavelAleatoriaInterna, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>>& a_matriz){
	
	try {

		if (a_attMatrizVariavelAleatoriaInterna != AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral)
			throw std::invalid_argument("Argumento invalido.");

		for (IdVariavelAleatoriaInterna idVarInterna = IdVariavelAleatoriaInterna_1; idVarInterna <= getMaiorId(IdVariavelAleatoriaInterna()); idVarInterna++)
			vetorVariavelAleatoriaInterna.att(idVarInterna).setMatriz(a_attMatrizVariavelAleatoriaInterna, a_matriz.at(idVarInterna));

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::setCenariosInternos(" + getFullString(a_attMatrizVariavelAleatoriaInterna) + "a_enupla): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::setCenariosInternos(const AttMatrizVariavelAleatoriaInterna a_attMatrizVariavelAleatoriaInterna, const SmartEnupla<IdVariavelAleatoriaInterna, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>>& a_matriz){
 


void VariavelAleatoria::calcularSerieTemporal(){

	try{

		const IdVariavelAleatoriaInterna maiorIdVariavelAleatoriaInterna = getMaiorId(IdVariavelAleatoriaInterna());

		if (maiorIdVariavelAleatoriaInterna == IdVariavelAleatoriaInterna_Nenhum)
			throw std::invalid_argument("Nenhuma variavel aleatoria interna foi instanciada.");

		const Periodo periodo_inicial = getIteradorInicial(IdVariavelAleatoriaInterna_1, AttVetorVariavelAleatoriaInterna_serie_temporal, Periodo());
		const Periodo periodo_final   = getIteradorFinal  (IdVariavelAleatoriaInterna_1, AttVetorVariavelAleatoriaInterna_serie_temporal, Periodo());

		if (periodo_inicial.getTipoPeriodo() != periodo_final.getTipoPeriodo())
			throw std::invalid_argument("O tipo de periodo inicial e final das series temporais de variaveis aleatorias internas deve ser o mesmo.");

		const SmartEnupla<Periodo, double> horizonte_serie_temporal = getVetor(IdVariavelAleatoriaInterna_1, AttVetorVariavelAleatoriaInterna_serie_temporal, Periodo(), double());

		for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= maiorIdVariavelAleatoriaInterna; idVariavelAleatoriaInterna++) {

			if (getIteradorInicial(idVariavelAleatoriaInterna, AttVetorVariavelAleatoriaInterna_serie_temporal, Periodo()) != periodo_inicial)
				throw std::invalid_argument("O periodo inicial das series temporais de variaveis aleatorias internas deve ser o mesmo.");
			else if (getIteradorFinal(idVariavelAleatoriaInterna, AttVetorVariavelAleatoriaInterna_serie_temporal, Periodo()) != periodo_final)
				throw std::invalid_argument("O periodo final das series temporais de variaveis aleatorias internas deve ser o mesmo.");

			vetorVariavelAleatoriaInterna.att(idVariavelAleatoriaInterna).deslocarSerieComGrauLiberdade();

			vetorVariavelAleatoriaInterna.att(idVariavelAleatoriaInterna).calcularEstatisticaSerieTransformada();

		} // for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= maiorIdVariavelAleatoriaInterna; idVariavelAleatoriaInterna++) {

		SmartEnupla<Periodo, double> serie_temporal(horizonte_serie_temporal, NAN);

		for (Periodo periodo = periodo_inicial; periodo <= periodo_final; periodo++) {

			double valor = 0.0;

			for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= maiorIdVariavelAleatoriaInterna; idVariavelAleatoriaInterna++)
				valor += getElementoVetor(idVariavelAleatoriaInterna, AttVetorVariavelAleatoriaInterna_serie_temporal_transformada, periodo, double());

			serie_temporal.setElemento(periodo, valor);

		} // for (Periodo periodo = periodo_inicial; periodo <= periodo_final; periodo++) {

		setVetor_forced(AttVetorVariavelAleatoria_serie_temporal,              serie_temporal);
		setVetor_forced(AttVetorVariavelAleatoria_serie_temporal_transformada, serie_temporal);

		calcularCoeficienteParticipacaoVariavelAleatoriaInterna();

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularSerieTemporal(): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::calcularSerieTemporal(){


void VariavelAleatoria::calcularTendenciaTemporal(){

	try {

		const IdVariavelAleatoriaInterna maiorIdVariavelAleatoriaInterna = getMaiorId(IdVariavelAleatoriaInterna());

		if (maiorIdVariavelAleatoriaInterna == IdVariavelAleatoriaInterna_Nenhum)
			throw std::invalid_argument("Nenhuma variavel aleatoria interna foi instanciada.");

		const IdCenario cenario_inicial = getIterador1Inicial(IdVariavelAleatoriaInterna_1, AttMatrizVariavelAleatoriaInterna_tendencia_temporal, IdCenario());
		const IdCenario cenario_final   = getIterador1Final  (IdVariavelAleatoriaInterna_1, AttMatrizVariavelAleatoriaInterna_tendencia_temporal, IdCenario());

		if (cenario_inicial != IdCenario_1)
			throw std::invalid_argument("O cenario inicial deve ser 1.");

		const Periodo periodo_inicial = getIterador2Inicial(IdVariavelAleatoriaInterna_1, AttMatrizVariavelAleatoriaInterna_tendencia_temporal, IdCenario_1, Periodo());
		const Periodo periodo_final   = getIterador2Final  (IdVariavelAleatoriaInterna_1, AttMatrizVariavelAleatoriaInterna_tendencia_temporal, IdCenario_1, Periodo());

		const SmartEnupla<Periodo, double> horizonte_tendencia = getElementosMatriz(IdVariavelAleatoriaInterna_1, AttMatrizVariavelAleatoriaInterna_tendencia_temporal, IdCenario_1, Periodo(), double());

		for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= maiorIdVariavelAleatoriaInterna; idVariavelAleatoriaInterna++) {

			if (getIterador1Inicial(idVariavelAleatoriaInterna, AttMatrizVariavelAleatoriaInterna_tendencia_temporal, IdCenario()) != cenario_inicial)
				throw std::invalid_argument("O cenario inicial das tendencias temporais de variaveis aleatorias internas deve ser o mesmo.");
			else if (getIterador1Final  (idVariavelAleatoriaInterna, AttMatrizVariavelAleatoriaInterna_tendencia_temporal, IdCenario()) != cenario_final)
				throw std::invalid_argument("O cenario final das tendencias temporais de variaveis aleatorias internas deve ser o mesmo.");

			for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

				if (getIterador2Inicial(idVariavelAleatoriaInterna, AttMatrizVariavelAleatoriaInterna_tendencia_temporal, idCenario, Periodo()) != periodo_inicial)
					throw std::invalid_argument("O periodo inicial das tendencias temporais de variaveis aleatorias internas deve ser o mesmo.");
				else if (getIterador2Final  (idVariavelAleatoriaInterna, AttMatrizVariavelAleatoriaInterna_tendencia_temporal, idCenario, Periodo()) != periodo_final)
					throw std::invalid_argument("O periodo final das tendencias temporais de variaveis aleatorias internas deve ser o mesmo.");

			} // for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

			vetorVariavelAleatoriaInterna.att(idVariavelAleatoriaInterna).deslocarTendenciaComGrauLiberdade();

		} // for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= maiorIdVariavelAleatoriaInterna; idVariavelAleatoriaInterna++) {

		SmartEnupla<IdCenario, SmartEnupla<Periodo, double>> tendencia_temporal(cenario_inicial, std::vector<SmartEnupla<Periodo, double>>(cenario_final, SmartEnupla<Periodo, double>(horizonte_tendencia, NAN)));

		for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

			for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_tendencia.incrementarIterador(periodo)) {

				double valor = 0.0;

				for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= maiorIdVariavelAleatoriaInterna; idVariavelAleatoriaInterna++)
					valor += getElementoMatriz(idVariavelAleatoriaInterna, AttMatrizVariavelAleatoriaInterna_tendencia_temporal_transformada, idCenario, periodo, double());

				tendencia_temporal.at(idCenario).setElemento(periodo, valor);

			} // for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_tendencia.incrementarIterador(periodo)) {

		} // for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

		setMatriz_forced(AttMatrizVariavelAleatoria_tendencia_temporal,              tendencia_temporal);
		setMatriz_forced(AttMatrizVariavelAleatoria_tendencia_temporal_transformada, tendencia_temporal);

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularTendenciaTemporal(): \n" + std::string(erro.what())); }


} // void VariavelAleatoria::calcularTendenciaTemporal(){


void VariavelAleatoria::calcularCoeficienteParticipacaoVariavelAleatoriaInterna(){

	try{

		const IdVariavelAleatoriaInterna maiorIdVariavelAleatoriaInterna = getMaiorId(IdVariavelAleatoriaInterna());

		if (maiorIdVariavelAleatoriaInterna == IdVariavelAleatoriaInterna_Nenhum)
			throw std::invalid_argument("Nenhuma variavel aleatoria interna foi instanciada.");

		const Periodo periodo_inicial = getIteradorInicial(IdVariavelAleatoriaInterna_1, AttVetorVariavelAleatoriaInterna_serie_temporal_transformada, Periodo());
		const Periodo periodo_final   = getIteradorFinal  (IdVariavelAleatoriaInterna_1, AttVetorVariavelAleatoriaInterna_serie_temporal_transformada, Periodo());

		if (periodo_inicial.getTipoPeriodo() != periodo_final.getTipoPeriodo())
			throw std::invalid_argument("O tipo de periodo inicial e final das series temporais de variaveis aleatorias internas deve ser o mesmo.");

		const IdEstacao maiorIdEstacao = Periodo::getMaiorEstacao(periodo_inicial.getTipoPeriodo());

		SmartEnupla<IdEstacao, double> participacao_total(IdEstacao_1, std::vector<double>(maiorIdEstacao, 0.0));

		for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= maiorIdVariavelAleatoriaInterna; idVariavelAleatoriaInterna++) {

			SmartEnupla<IdEstacao, SmartEnupla<int, double>> participacao(IdEstacao_1, std::vector<SmartEnupla<int, double>>(maiorIdEstacao, SmartEnupla<int, double>()));

			for (Periodo periodo = periodo_inicial; periodo <= periodo_final; periodo++) {

				const IdEstacao idEstacao = periodo.getEstacao();

				const double valor         = getElementoVetor(AttVetorVariavelAleatoria_serie_temporal, periodo, double());
				const double valor_interno = getElementoVetor(idVariavelAleatoriaInterna, AttVetorVariavelAleatoriaInterna_serie_temporal_transformada, periodo, double());

				participacao.at(idEstacao).addElemento(valor_interno / valor);

			} // for (Periodo periodo = periodo_inicial; periodo <= periodo_final; periodo++) {

			for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorIdEstacao; idEstacao++) {

				const double media_participacao = getMedia(participacao.at(idEstacao));

				vetorVariavelAleatoriaInterna.att(idVariavelAleatoriaInterna).addElemento(AttVetorVariavelAleatoriaInterna_coeficiente_participacao_sazonal, idEstacao, media_participacao);

				participacao_total.at(idEstacao) += media_participacao;

			} // for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorIdEstacao; idEstacao++) {

		} // for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= maiorIdVariavelAleatoriaInterna; idVariavelAleatoriaInterna++) {

		for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorIdEstacao; idEstacao++) {

			if (!doubleCompara(0.001, participacao_total.at(idEstacao), 1.0))
				throw std::invalid_argument("A participacao das series internas na serie da variavel aleatoria deve totalizar 1.0.");

		} // for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorIdEstacao; idEstacao++) {

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularCoeficienteParticipacaoVariavelAleatoriaInterna(): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::calcularCoeficienteParticipacaoVariavelAleatoriaInterna(){


void VariavelAleatoria::calcularEstatisticaSerieTemporal(){

	try {

		const TipoPeriodo tipoPeriodo = getIteradorInicial(AttVetorVariavelAleatoria_serie_temporal, Periodo()).getTipoPeriodo();

		const IdEstacao maiorEstacao = Periodo::getMaiorEstacao(tipoPeriodo);

		SmartEnupla<IdEstacao, double> media;
		SmartEnupla<IdEstacao, double> desvio;
		SmartEnupla<IdEstacao, double> assimetria;

		for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

			const std::vector<double> dados = getVetor(AttVetorVariavelAleatoria_serie_temporal, Periodo(), double()).getElementos(idEstacao).at(tipoPeriodo);

			media.addElemento(idEstacao, getMedia(dados));

			desvio.addElemento(idEstacao, getDesvio(media.getElemento(idEstacao), dados));

            assimetria.addElemento(idEstacao, getAssimetria(media.getElemento(idEstacao), desvio.getElemento(idEstacao), dados));

		} // for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

		setVetor_forced(AttVetorVariavelAleatoria_media_serie_temporal, media);
		setVetor_forced(AttVetorVariavelAleatoria_desvio_serie_temporal, desvio);
		setVetor_forced(AttVetorVariavelAleatoria_assimetria_serie_temporal, assimetria);

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularEstatisticaSerieTemporal(): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::calcularEstatisticaSerieTemporal(){



void VariavelAleatoria::calcularEstatisticaSerieTransformada(){

	try {

		const TipoPeriodo tipoPeriodo = getIteradorInicial(AttVetorVariavelAleatoria_serie_temporal_transformada, Periodo()).getTipoPeriodo();

		const IdEstacao maiorEstacao = Periodo::getMaiorEstacao(tipoPeriodo);

		SmartEnupla<IdEstacao, double> media;
		SmartEnupla<IdEstacao, double> desvio;
		SmartEnupla<IdEstacao, double> assimetria;
		SmartEnupla<IdEstacao, double> ic_auto_correlacao;

		for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

			const std::vector<double> dados = getVetor(AttVetorVariavelAleatoria_serie_temporal_transformada, Periodo(), double()).getElementos(idEstacao).at(tipoPeriodo);

			media.addElemento(idEstacao, getMedia(dados));

			desvio.addElemento(idEstacao, getDesvio(media.getElemento(idEstacao), dados));

			assimetria.addElemento(idEstacao, getAssimetria(media.getElemento(idEstacao), desvio.getElemento(idEstacao), dados));

			// Cálculo do Intervalo de Confiança
			// rk(95%) = +- 1.96*sqrt(N)
			ic_auto_correlacao.addElemento(idEstacao, 1.96 / std::sqrt(double(dados.size())));

		} // for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

		setVetor_forced(AttVetorVariavelAleatoria_media_serie_transformada, media);
		setVetor_forced(AttVetorVariavelAleatoria_desvio_serie_transformada, desvio);
		setVetor_forced(AttVetorVariavelAleatoria_assimetria_serie_transformada, assimetria);
		setVetor_forced(AttVetorVariavelAleatoria_ic_auto_correlacao, ic_auto_correlacao);

		calcularAutoCovarianciaSerieTransformada();

		calcularAutoCovarianciaNaoSerieTransformada();

		calcularAutoCorrelacaoSerieTransformada();

		calcularAutoCorrelacaoNaoSazonalSerieTransformada();

		calcularAutoCorrelacaoParcialSerieTransformada();

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularEstatisticaSerieTransformada(): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::calcularEstatisticaBasica(){


void VariavelAleatoria::calcularAutoCovarianciaSerieTransformada(){

	try{

		if (getSizeMatriz(AttMatrizVariavelAleatoria_auto_covariancia_sazonal) > 0)
			return;

		setMatriz_forced(AttMatrizVariavelAleatoria_auto_covariancia_sazonal, getAutoCovarianciaSazonal(getVetor(AttVetorVariavelAleatoria_serie_temporal_transformada, Periodo(), double())));

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularAutoCovarianciaSerieTransformada(): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::calcularAutoCovarianciaSerieTransformada(){


void VariavelAleatoria::calcularAutoCovarianciaNaoSerieTransformada(){

	try {

		if (getSizeVetor(AttVetorVariavelAleatoria_auto_covariancia) > 0)
			return;

		setVetor_forced(AttVetorVariavelAleatoria_auto_covariancia, getAutoCovarianciaNaoSazonal(getVetor(AttVetorVariavelAleatoria_serie_temporal_transformada, Periodo(), double())));

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularAutoCovarianciaSerieTransformada(): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::calcularAutoCovarianciaNaoSerieTransformada(){


void VariavelAleatoria::calcularAutoCorrelacaoSerieTransformada() {

	try {

		if (getSizeMatriz(AttMatrizVariavelAleatoria_auto_correlacao) > 0)
			return;

		setMatriz_forced(AttMatrizVariavelAleatoria_auto_correlacao, getAutoCorrelacaoSazonal(getMatriz(AttMatrizVariavelAleatoria_auto_covariancia_sazonal, IdEstacao(), int(), double())));

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularAutoCorrelacaoSerieTransformada(): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::calcularAutoCorrelacaoSerieTransformada(){


void VariavelAleatoria::calcularAutoCorrelacaoNaoSazonalSerieTransformada() {

	try {

		if (getSizeVetor(AttVetorVariavelAleatoria_auto_correlacao) > 0)
			return;

		setVetor_forced(AttVetorVariavelAleatoria_auto_correlacao, getAutoCorrelacaoNaoSazonal(getVetor(AttVetorVariavelAleatoria_auto_covariancia, int(), double())));

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularAutoCorrelacaoNaoSerieTransformada(): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::calcularAutoCorrelacaoNaoSerieTransformada(){


void VariavelAleatoria::calcularAutoCorrelacaoParcialSerieTransformada(){

	try {

		const TipoPeriodo tipoPeriodo = getIteradorInicial(AttVetorVariavelAleatoria_serie_temporal_transformada, Periodo()).getTipoPeriodo();

		const IdEstacao maiorEstacao = Periodo::getMaiorEstacao(tipoPeriodo);

		const int ordem_maxima = int(maiorEstacao) - 1;

		SmartEnupla<IdEstacao, SmartEnupla<int, double>> auto_correlacao_parcial;

		// ----------------------------------------------------------
		// Definindo a ordem da correlação temporal para cada estação
		// ----------------------------------------------------------
		// Calculo da Matriz Yule-Walker (YW) e a Função de Auto Correlação Parcial (PACF)
		for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

			SmartEnupla<int, double> auto_correlacao_parcial_estacao(1, std::vector<double>(ordem_maxima, 0.0));

			for (int p = 1; p <= ordem_maxima; p++) {

				SparseMatrix<double> matriz_YW;
				std::valarray<double>  vetor_b;
				std::valarray<double>  vetor_x;

				matriz_YW.Dim(p, p, 1);
				matriz_YW.Set_strategy(new LS_Zollen<double>);

				vetor_b.resize(p);
				vetor_x.resize(p);

				for (int i = 1; i <= p; i++) {  // Constroi a matrix matriz_YW

					vetor_b[i - 1] = getElementoMatriz(AttMatrizVariavelAleatoria_auto_correlacao, idEstacao, i, double());

					IdEstacao idEstacao_lag = IdEstacao_Nenhum;

					if (IdEstacao(idEstacao - i) < IdEstacao_1)
						idEstacao_lag = IdEstacao(idEstacao - i + maiorEstacao);
					else
						idEstacao_lag = IdEstacao(idEstacao - i);

					for (int j = 1; j <= p; j++) {
						if (i <= j) {
							const int lag = j - i;
							const double auto_correlacao = getElementoMatriz(AttMatrizVariavelAleatoria_auto_correlacao, idEstacao_lag, lag, double());
							matriz_YW[i - 1][j - 1] = auto_correlacao;
							matriz_YW[j - 1][i - 1] = auto_correlacao;
						} // if (i <= j) {
					} // for (int j = 1; j <= p; j++) {

				} // for (int i = 1; i <= p; i++){

				matriz_YW.Solve_LinSys(vetor_b, vetor_x);

				const double valor = vetor_x[p - 1];

				if (std::isnan(valor) || std::isinf(valor) || doubleCompara(valor, 0.0))
					auto_correlacao_parcial_estacao.setElemento(p, 0.0);
				else
					auto_correlacao_parcial_estacao.setElemento(p, vetor_x[p - 1]);

				matriz_YW.Reset();

			} // for (int p = 1; p <= ordem_maxima; p++) {

			auto_correlacao_parcial.addElemento(idEstacao, auto_correlacao_parcial_estacao);

		} // for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

		setMatriz_forced(AttMatrizVariavelAleatoria_auto_correlacao_parcial, auto_correlacao_parcial);

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularAutoCorrelacaoParcialSerieTransformada(): \n" + std::string(erro.what())); }


} // void VariavelAleatoria::calcularAutoCorrelacaoParcialSerieTransformada(){



void VariavelAleatoria::determinarOrdemAutoCorrelacaoSerieTransformada(const TipoValor a_tipo_coeficiente_auto_correlacao, const int a_ordem_fixa_correlacao_temporal){

	try {

		const TipoPeriodo tipoPeriodo = getIteradorInicial(AttVetorVariavelAleatoria_serie_temporal_transformada, Periodo()).getTipoPeriodo();

		const IdEstacao maiorEstacao = Periodo::getMaiorEstacao(tipoPeriodo);

		const int ordem_maxima = int(maiorEstacao) - 1;

		if (a_ordem_fixa_correlacao_temporal == 0) {

			for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

				if (getSizeVetor(AttVetorVariavelAleatoria_ordem_auto_correlacao) == int(maiorEstacao))
					setElemento(AttVetorVariavelAleatoria_ordem_auto_correlacao, idEstacao, 0);
				else
					addElemento(AttVetorVariavelAleatoria_ordem_auto_correlacao, idEstacao, 0);

			} // for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

			return;

		} // if (a_ordem_fixa_correlacao_temporal == 0) {

		else if (a_ordem_fixa_correlacao_temporal < 0)
			throw std::invalid_argument("Ordem fixa para correlacao temporal invalida.");
		else if (a_ordem_fixa_correlacao_temporal > ordem_maxima)
			throw std::invalid_argument("Ordem fixa para correlacao temporal: " + getString(a_ordem_fixa_correlacao_temporal) + " maior que ordem maxima: " + getString(ordem_maxima));


		// Define os Lags da PACF que ultrapassam os Limites
		for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

			const double ic_auto_correlacao = getElementoVetor(AttVetorVariavelAleatoria_ic_auto_correlacao, idEstacao, double());

			int ordem = 0;
			for (int p = 1; p <= ordem_maxima; p++) {
				const double valor_PACF = getElementoMatriz(AttMatrizVariavelAleatoria_auto_correlacao_parcial, idEstacao, p, double());

				// A ordem é determinada pelo numero consecutivo de valores significativos de PACF
				if (((a_tipo_coeficiente_auto_correlacao == TipoValor_positivo_e_negativo) || (a_tipo_coeficiente_auto_correlacao == TipoValor_negativo)) && (valor_PACF < -ic_auto_correlacao))
					ordem = p;
				else if (((a_tipo_coeficiente_auto_correlacao == TipoValor_positivo_e_negativo) || (a_tipo_coeficiente_auto_correlacao == TipoValor_positivo)) && (valor_PACF > ic_auto_correlacao))
					ordem = p;
				else
					break;

			} // for (int p = 1; p <= ordem_maxima; p++) {

			if (ordem > a_ordem_fixa_correlacao_temporal)
				ordem = a_ordem_fixa_correlacao_temporal;

			if (getSizeVetor(AttVetorVariavelAleatoria_ordem_auto_correlacao) == int(maiorEstacao))
				setElemento(AttVetorVariavelAleatoria_ordem_auto_correlacao, idEstacao, ordem);
			else
				addElemento(AttVetorVariavelAleatoria_ordem_auto_correlacao, idEstacao, ordem);

		} // for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

		// Corrige a progressão dos lags.

		int contador_correcao_progressao_ordem = 0;

		IdEstacao idEstacao = IdEstacao_1;
		IdEstacao idEstacao_anterior = IdEstacao_Nenhum;

		while (contador_correcao_progressao_ordem < int(maiorEstacao)) {

			if (idEstacao == IdEstacao_1)
				idEstacao_anterior = maiorEstacao;
			else
				idEstacao_anterior = IdEstacao(idEstacao - 1);

			const int ordem          = getElementoVetor(AttVetorVariavelAleatoria_ordem_auto_correlacao, idEstacao, int());
			const int ordem_anterior = getElementoVetor(AttVetorVariavelAleatoria_ordem_auto_correlacao, idEstacao_anterior, int());

			if (ordem > ordem_anterior + 1) {
				setElemento(AttVetorVariavelAleatoria_ordem_auto_correlacao, idEstacao, ordem_anterior + 1);
				contador_correcao_progressao_ordem = 1;
			}

			else
				contador_correcao_progressao_ordem++;

			if (idEstacao == maiorEstacao)
				idEstacao = IdEstacao_1;
			else
				idEstacao = IdEstacao(idEstacao + 1);

		} // while (contador_correcao_progressao_ordem < int(maiorEstacao)) {

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::determinarOrdemAutoCorrelacaoSerieTransformada(" + getString(a_tipo_coeficiente_auto_correlacao) + "," + getString(a_ordem_fixa_correlacao_temporal) + "): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::determinarOrdemAutoCorrelacaoSerieTransformada(const TipoValor a_tipo_coeficiente_auto_correlacao, const int a_ordem_fixa_correlacao_temporal){



void VariavelAleatoria::calcularCoeficientesAutoCorrelacaoSerieTransformada(const TipoValor a_tipo_coeficiente_auto_correlacao, const int a_ordem_fixa_correlacao_temporal){

	try{

		const TipoPeriodo tipoPeriodo = getIteradorInicial(AttVetorVariavelAleatoria_serie_temporal_transformada, Periodo()).getTipoPeriodo();

		const IdEstacao maiorEstacao = Periodo::getMaiorEstacao(tipoPeriodo);

		SmartEnupla<IdEstacao, SmartEnupla<int, double>> coeficiente_correlacao_temporal;
		SmartEnupla<IdEstacao, SmartEnupla<int, double>> coeficiente_linear_correlacao_temporal;

		if (a_ordem_fixa_correlacao_temporal == 0) {

			coeficiente_correlacao_temporal         = SmartEnupla<IdEstacao, SmartEnupla<int, double>>(IdEstacao_1, std::vector<SmartEnupla<int, double>>(maiorEstacao, SmartEnupla<int, double>(1, std::vector<double>(1, 0.0))));
			coeficiente_linear_correlacao_temporal = coeficiente_correlacao_temporal;

			setMatriz_forced(AttMatrizVariavelAleatoria_coeficiente_auto_correlacao, coeficiente_correlacao_temporal);
			setMatriz_forced(AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao_sazonal, coeficiente_linear_correlacao_temporal);

			return;

		}

		// -------------------------------------------------------------------
		// Estima coeficientes da correlação temporal para cada lag da estação
		// -------------------------------------------------------------------
		for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

			SmartEnupla<int, double> coeficiente_auto_correlacao(1, std::vector<double>(a_ordem_fixa_correlacao_temporal, 0.0));
			SmartEnupla<int, double> coeficiente_linear_auto_correlacao;

			const int p = getElementoVetor(AttVetorVariavelAleatoria_ordem_auto_correlacao, idEstacao, int());

			if (p > 0) {

				int p_novo = p;
				bool coeficiente_linear_invalido = true;
				while (coeficiente_linear_invalido) {

					coeficiente_linear_invalido = false;

					SparseMatrix<double> matriz_YW;
					std::valarray<double>  vetor_b;
					std::valarray<double>  vetor_x;

					matriz_YW.Dim(p_novo, p_novo, 1);
					matriz_YW.Set_strategy(new LS_Zollen<double>);

					vetor_b.resize(p_novo);
					vetor_x.resize(p_novo);

					for (int i = 1; i <= p_novo; i++) {  // Constroi a matrix matriz_YW

						vetor_b[i - 1] = getElementoMatriz(AttMatrizVariavelAleatoria_auto_correlacao, idEstacao, i, double());

						IdEstacao idEstacao_lag = IdEstacao_Nenhum;

						if (IdEstacao(idEstacao - i) < IdEstacao_1)
							idEstacao_lag = IdEstacao(idEstacao - i + maiorEstacao);
						else
							idEstacao_lag = IdEstacao(idEstacao - i);

						for (int j = 1; j <= p_novo; j++) {
							if (i <= j) {
								const int lag = j - i;
								const double auto_correlacao = getElementoMatriz(AttMatrizVariavelAleatoria_auto_correlacao, idEstacao_lag, lag, double());
								matriz_YW[i - 1][j - 1] = auto_correlacao;
								matriz_YW[j - 1][i - 1] = auto_correlacao;
							} // if (i <= j) {
						} // for (int j = 1; j <= p_novo; j++) {

					} // for (int i = 1; i <= p_novo; i++){

					matriz_YW.Solve_LinSys(vetor_b, vetor_x);

					const double desvio = getElementoVetor(AttVetorVariavelAleatoria_desvio_serie_transformada, idEstacao, double());

					for (int i = 1; i <= p_novo; i++) {

						double coeficiente = vetor_x[i - 1];

						if (std::isnan(coeficiente) || std::isinf(coeficiente) || doubleCompara(coeficiente, 0.0))
							coeficiente = 0.0;

						coeficiente_auto_correlacao.setElemento(i, coeficiente);

						IdEstacao idEstacao_lag = IdEstacao_Nenhum;

						if (IdEstacao(idEstacao - i) < IdEstacao_1)
							idEstacao_lag = IdEstacao(idEstacao - i + maiorEstacao);
						else
							idEstacao_lag = IdEstacao(idEstacao - i);

						const double desvio_lag = getElementoVetor(AttVetorVariavelAleatoria_desvio_serie_transformada, idEstacao_lag, double());

						const double coeficiente_linear = coeficiente * (desvio / desvio_lag);


						if (((a_tipo_coeficiente_auto_correlacao == TipoValor_positivo) && (coeficiente_linear < 0.0)) || ((a_tipo_coeficiente_auto_correlacao == TipoValor_negativo) && (coeficiente_linear > 0.0))) {
							coeficiente_linear_invalido = true;

							p_novo--;
							setElemento(AttVetorVariavelAleatoria_ordem_auto_correlacao, idEstacao, p_novo);

							break;

						} // if (((a_tipo_coeficiente_auto_correlacao == TipoValor_positivo) && (coeficiente_linear < 0.0)) || ((a_tipo_coeficiente_auto_correlacao == TipoValor_negativo) && (coeficiente_linear > 0.0))) {

						coeficiente_linear_auto_correlacao.addElemento(i, coeficiente_linear);

					} // for (int i = 1; i <= p_novo; i++) {

					matriz_YW.Reset();

					if (p_novo == 0)
						coeficiente_linear_invalido = false;

				} // while(coeficiente_linear_invalido){

			} // if (p > 0) {

			if (coeficiente_auto_correlacao.size() == 0)
				coeficiente_auto_correlacao.addElemento(1, 0.0);

			if (coeficiente_linear_auto_correlacao.size() == 0)
				coeficiente_linear_auto_correlacao.addElemento(1, 0.0);

			coeficiente_correlacao_temporal.addElemento(idEstacao, coeficiente_auto_correlacao);
			coeficiente_linear_correlacao_temporal.addElemento(idEstacao, coeficiente_linear_auto_correlacao);

			if (idEstacao == maiorEstacao) {

				setMatriz_forced(AttMatrizVariavelAleatoria_coeficiente_auto_correlacao, coeficiente_correlacao_temporal);
				setMatriz_forced(AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao_sazonal, coeficiente_linear_correlacao_temporal);

			} // if (idEstacao == maiorEstacao) {

		} // for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularCoeficientesAutoCorrelacaoSerieTransformada(" + getString(a_tipo_coeficiente_auto_correlacao) + "," + getString(a_ordem_fixa_correlacao_temporal) + "): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::calcularCoeficientesAutoCorrelacaoSerieTransformada(const TipoValor a_tipo_coeficiente_auto_correlacao, const int a_ordem_fixa_correlacao_temporal){



void VariavelAleatoria::parametrizarModelo(const TipoModeloGeracaoSinteticaCenario a_tipo_modelo_geracao_sintetica, const TipoValor a_tipo_coeficiente_auto_correlacao, const int a_ordem_maxima_coeficiente_auto_correlacao) {

	try {

		setAtributo(AttComumVariavelAleatoria_tipo_modelo_geracao_sintetica,            a_tipo_modelo_geracao_sintetica);
		setAtributo(AttComumVariavelAleatoria_tipo_coeficiente_auto_correlacao,         a_tipo_coeficiente_auto_correlacao);
		setAtributo(AttComumVariavelAleatoria_ordem_maxima_coeficiente_auto_correlacao, a_ordem_maxima_coeficiente_auto_correlacao);

		if (a_tipo_modelo_geracao_sintetica == TipoModeloGeracaoSinteticaCenario_lognormal_3p_sazonal)
			parametrizar_modelo_lognormal_3p_sazonal(a_tipo_coeficiente_auto_correlacao, a_ordem_maxima_coeficiente_auto_correlacao);

		else
			throw std::invalid_argument("Tipo de Modelo Estocastico nao utilizado");

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::parametrizarModelo(a_entradaSaidaDados," + "," + getFullString(a_tipo_modelo_geracao_sintetica) + "," + getFullString(a_tipo_coeficiente_auto_correlacao) + "," + getFullString(a_ordem_maxima_coeficiente_auto_correlacao) + "): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::parametrizarModelo(const TipoModeloGeracaoSinteticaCenario tipo_modelo_geracao_sintetica, const TipoValor a_tipo_coeficiente_auto_correlacao, const int a_ordem_coeficiente_auto_correlacao, const TipoSorteio a_tipo_sorteio){


void VariavelAleatoria::parametrizar_modelo_lognormal_3p_sazonal(const TipoValor a_tipo_coeficiente_auto_correlacao, const int a_ordem_fixa_correlacao_temporal) {

	try {

		calcularEstatisticaSerieTemporal();

		calcularEstatisticaSerieTransformada();

		determinarOrdemAutoCorrelacaoSerieTransformada(a_tipo_coeficiente_auto_correlacao, a_ordem_fixa_correlacao_temporal);

		calcularCoeficientesAutoCorrelacaoSerieTransformada(a_tipo_coeficiente_auto_correlacao, a_ordem_fixa_correlacao_temporal);

		calcularSerieResiduo();

		calcularEstatisticaSerieResiduo();

		calcularAutoCorrelacaoSerieResiduo();

		calcularAutoCorrelacaoNaoSazonalSerieResiduo();

		calcularSigmaMiDeltaResiduoNormal();

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::parametrizar_modelo_lognormal_3p_sazonal(" + getString(a_tipo_coeficiente_auto_correlacao) + "," + getString(a_ordem_fixa_correlacao_temporal) + "): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::parametrizar_modelo_lognormal_3p_sazonal(const int a_ordem_maxima_correlacao_temporal) {




void VariavelAleatoria::calcularSerieResiduo(){

	try {

		const TipoPeriodo tipoPeriodo = getIteradorInicial(AttVetorVariavelAleatoria_serie_temporal_transformada, Periodo()).getTipoPeriodo();

		const IdEstacao maiorEstacao = Periodo::getMaiorEstacao(tipoPeriodo);

		const int ordem_maxima = int(maiorEstacao) - 1;

		// O periodo inicial do histórico é considerado a partir do 2o ciclo de estações do historico.
		const Periodo periodo_inicial_serie_temporal = getIteradorInicial(AttVetorVariavelAleatoria_serie_temporal_transformada, Periodo()) + int(maiorEstacao);
		const Periodo periodo_final_serie_temporal   = getIteradorFinal(AttVetorVariavelAleatoria_serie_temporal_transformada, Periodo());

		const int numero_elementos_serie_temporal = periodo_final_serie_temporal - periodo_inicial_serie_temporal + 1;

		SmartEnupla<Periodo, double> residuo          (periodo_inicial_serie_temporal, std::vector<double>(numero_elementos_serie_temporal, 0.0));
		SmartEnupla<Periodo, double> residuo_lognormal(periodo_inicial_serie_temporal, std::vector<double>(numero_elementos_serie_temporal, 0.0));

		double grau_liberdade_residuo_lognormal = 0.0;

		for (Periodo periodo = periodo_inicial_serie_temporal; periodo <= periodo_final_serie_temporal; periodo++) {

			const IdEstacao idEstacao = periodo.getEstacao();

			const int ordem_maxima = getElementoVetor(AttVetorVariavelAleatoria_ordem_auto_correlacao, idEstacao, int());

			double parcela_nao_aleatoria_residuo_periodo = getElementoVetor(AttVetorVariavelAleatoria_media_serie_transformada, idEstacao, double());

			double valor_anterior = 0;

			for (int lag = 1; lag <= ordem_maxima; lag++) {

				const Periodo periodo_lag = periodo - lag;

				const double coeficiente_linear_lag = getElementoMatriz(AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao_sazonal, idEstacao, lag, double());

				const double valor_lag = getElementoVetor(AttVetorVariavelAleatoria_serie_temporal_transformada, periodo_lag, double());

				valor_anterior += coeficiente_linear_lag * valor_lag;

				const double media_lag = getElementoVetor(AttVetorVariavelAleatoria_media_serie_transformada, periodo_lag.getEstacao(), double());

				parcela_nao_aleatoria_residuo_periodo -= coeficiente_linear_lag * media_lag;

			} // for (int lag = 1; lag <= ordem_maxima; lag++) {

			const double valor = getElementoVetor(AttVetorVariavelAleatoria_serie_temporal_transformada, periodo, double());

			const double residuo_periodo = valor - valor_anterior;

			double desvio = getElementoVetor(AttVetorVariavelAleatoria_desvio_serie_transformada, idEstacao, double());

			if (desvio == 0)
				desvio = 1;

			const double residuo_lognormal_periodo = (residuo_periodo - parcela_nao_aleatoria_residuo_periodo) / desvio;

			residuo.setElemento          (periodo, residuo_periodo);
			residuo_lognormal.setElemento(periodo, residuo_lognormal_periodo);

			if (residuo_lognormal_periodo < grau_liberdade_residuo_lognormal)
				grau_liberdade_residuo_lognormal = residuo_lognormal_periodo;

		} // for (Periodo periodo = periodo_inicial_serie_temporal; periodo <= periodo_final_serie_temporal; periodo++) {

		grau_liberdade_residuo_lognormal = 1.1 - grau_liberdade_residuo_lognormal;

		setAtributo(AttComumVariavelAleatoria_grau_liberdade_residuo_lognormal, grau_liberdade_residuo_lognormal);

		for (Periodo periodo = periodo_inicial_serie_temporal; periodo <= periodo_final_serie_temporal; periodo++)
			residuo_lognormal.at(periodo) +=  grau_liberdade_residuo_lognormal;

		const SmartEnupla<Periodo, double> residuo_normal = getLog(residuo_lognormal);

		setVetor_forced(AttVetorVariavelAleatoria_serie_residuo, residuo);
		setVetor_forced(AttVetorVariavelAleatoria_serie_residuo_normal, residuo_normal);
		setVetor_forced(AttVetorVariavelAleatoria_serie_residuo_lognormal, residuo_lognormal);

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularSerieResiduo(): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::calcularSerieResiduo(){



void VariavelAleatoria::calcularEstatisticaSerieResiduo(){

	try {

		const TipoPeriodo tipoPeriodo = getIteradorInicial(AttVetorVariavelAleatoria_serie_residuo, Periodo()).getTipoPeriodo();

		const IdEstacao maiorEstacao = Periodo::getMaiorEstacao(tipoPeriodo);

		SmartEnupla<IdEstacao ,double> enupla_media_residuo_normal;
		SmartEnupla<IdEstacao, double> enupla_media_residuo_lognormal;
		SmartEnupla<IdEstacao, double> enupla_desvio_residuo_normal;
		SmartEnupla<IdEstacao, double> enupla_desvio_residuo_lognormal;
		SmartEnupla<IdEstacao, double> enupla_assimetria_residuo_lognormal;

		for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

			const std::vector<double> dados_residuo_normal     = getVetor(AttVetorVariavelAleatoria_serie_residuo_normal, Periodo(), double()).getElementos(idEstacao).at(tipoPeriodo);
			const std::vector<double> dados_residuo_lognormal = getVetor(AttVetorVariavelAleatoria_serie_residuo_lognormal, Periodo(), double()).getElementos(idEstacao).at(tipoPeriodo);

			const double media_residuo_normal    = getMedia(dados_residuo_normal);
			const double media_residuo_lognormal = getMedia(dados_residuo_lognormal);

			const double desvio_residuo_normal    = getDesvio(media_residuo_normal, dados_residuo_normal);
			const double desvio_residuo_lognormal = getDesvio(media_residuo_lognormal, dados_residuo_lognormal);

			const double assimetria_residuo_lognormal = getAssimetria(media_residuo_lognormal, desvio_residuo_lognormal, dados_residuo_lognormal);

			enupla_media_residuo_normal.addElemento(idEstacao, media_residuo_normal);
			enupla_media_residuo_lognormal.addElemento(idEstacao, media_residuo_lognormal);
			enupla_desvio_residuo_normal.addElemento(idEstacao, desvio_residuo_normal);
			enupla_desvio_residuo_lognormal.addElemento(idEstacao, desvio_residuo_lognormal);
			enupla_assimetria_residuo_lognormal.addElemento(idEstacao, assimetria_residuo_lognormal);

		} // for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

		setVetor_forced(AttVetorVariavelAleatoria_media_residuo_normal,         enupla_media_residuo_normal);
		setVetor_forced(AttVetorVariavelAleatoria_media_residuo_lognormal,      enupla_media_residuo_lognormal);
		setVetor_forced(AttVetorVariavelAleatoria_desvio_residuo_normal,        enupla_desvio_residuo_normal);
		setVetor_forced(AttVetorVariavelAleatoria_desvio_residuo_lognormal,     enupla_desvio_residuo_lognormal);
		setVetor_forced(AttVetorVariavelAleatoria_assimetria_residuo_lognormal, enupla_assimetria_residuo_lognormal);

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularEstatisticaSerieResiduo(): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::calcularEstatisticaSerieResiduo(){


void VariavelAleatoria::calcularAutoCorrelacaoSerieResiduo(){

	try{

		setMatriz_forced(AttMatrizVariavelAleatoria_auto_correlacao_residuo,           getAutoCorrelacaoSazonal(getAutoCovarianciaSazonal(getVetor(AttVetorVariavelAleatoria_serie_residuo, Periodo(), double()))));
		setMatriz_forced(AttMatrizVariavelAleatoria_auto_correlacao_residuo_normal,    getAutoCorrelacaoSazonal(getAutoCovarianciaSazonal(getVetor(AttVetorVariavelAleatoria_serie_residuo_normal, Periodo(), double()))));
		setMatriz_forced(AttMatrizVariavelAleatoria_auto_correlacao_residuo_lognormal, getAutoCorrelacaoSazonal(getAutoCovarianciaSazonal(getVetor(AttVetorVariavelAleatoria_serie_residuo_lognormal, Periodo(), double()))));

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularAutoCorrelacaoSerieResiduo(): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::calcularAutoCorrelacaoSerieResiduo(){

void VariavelAleatoria::calcularAutoCorrelacaoNaoSazonalSerieResiduo(){

	try {

		setVetor_forced(AttVetorVariavelAleatoria_auto_correlacao_residuo, getAutoCorrelacaoNaoSazonal(getAutoCovarianciaNaoSazonal(getVetor(AttVetorVariavelAleatoria_serie_residuo, Periodo(), double()))));
		setVetor_forced(AttVetorVariavelAleatoria_auto_correlacao_residuo_normal, getAutoCorrelacaoNaoSazonal(getAutoCovarianciaNaoSazonal(getVetor(AttVetorVariavelAleatoria_serie_residuo_normal, Periodo(), double()))));
		setVetor_forced(AttVetorVariavelAleatoria_auto_correlacao_residuo_lognormal, getAutoCorrelacaoNaoSazonal(getAutoCovarianciaNaoSazonal(getVetor(AttVetorVariavelAleatoria_serie_residuo_lognormal, Periodo(), double()))));

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularAutoCorrelacaoNaoSazonalSerieResiduo(): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::calcularAutoCorrelacaoNaoSerieResiduo(){



void VariavelAleatoria::calcularSigmaMiDeltaResiduoNormal(){

	try{

		const TipoPeriodo tipoPeriodo = getIteradorInicial(AttVetorVariavelAleatoria_serie_residuo_lognormal, Periodo()).getTipoPeriodo();

		const IdEstacao maiorEstacao = Periodo::getMaiorEstacao(tipoPeriodo);

		SmartEnupla<IdEstacao, double> enupla_sigma_residuo_normal;
		SmartEnupla<IdEstacao, double> enupla_mi_residuo_normal;
		SmartEnupla<IdEstacao, double> enupla_delta_residuo_normal;
		SmartEnupla<IdEstacao, double> enupla_phi_residuo_lognormal;

		for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

			const double  media_residuo_normal = getElementoVetor(AttVetorVariavelAleatoria_media_residuo_normal, idEstacao, double());
			const double desvio_residuo_normal = getElementoVetor(AttVetorVariavelAleatoria_desvio_residuo_normal, idEstacao, double());

			const double      media_residuo_lognormal = getElementoVetor(AttVetorVariavelAleatoria_media_residuo_lognormal,      idEstacao, double());
			const double     desvio_residuo_lognormal = getElementoVetor(AttVetorVariavelAleatoria_desvio_residuo_lognormal,     idEstacao, double());
			const double assimetria_residuo_lognormal = getElementoVetor(AttVetorVariavelAleatoria_assimetria_residuo_lognormal, idEstacao, double());

			const double grau_liberdade_residuo_lognormal = getAtributo(AttComumVariavelAleatoria_grau_liberdade_residuo_lognormal, double());

			double phi_residuo_lognormal = 1.0;

			if (assimetria_residuo_lognormal > 0.0) {

				phi_residuo_lognormal  = std::pow(((1 + std::pow(assimetria_residuo_lognormal, 2) / 2) + std::pow((std::pow(assimetria_residuo_lognormal, 2) + std::pow(assimetria_residuo_lognormal, 4) / 4), 0.5)), 0.33333);
				phi_residuo_lognormal += std::pow(((1 + std::pow(assimetria_residuo_lognormal, 2) / 2) - std::pow((std::pow(assimetria_residuo_lognormal, 2) + std::pow(assimetria_residuo_lognormal, 4) / 4), 0.5)), 0.33333) - 1.0;

				if (phi_residuo_lognormal < 1.0)
					phi_residuo_lognormal = 1.0;

			} // if (assimetria_residuo_lognormal > 0.0) {


			double sigma_residuo_normal = 0.0;
			double mi_residuo_normal    = 0.0;
			double delta_residuo_normal = 0.0;

			//
			// Em caso de desvio do resíduo nulo, resíduo é nulo.
			//
			if (desvio_residuo_lognormal == 0.0) {}

			//
			// Em caso de distribuição Lognormal inviável p/ resíduos, utiliza-se Normal.
			//
			else if (assimetria_residuo_lognormal == 0.0) {
				sigma_residuo_normal = desvio_residuo_lognormal;
				mi_residuo_normal    = media_residuo_lognormal;
				delta_residuo_normal = -grau_liberdade_residuo_lognormal;
			} // else if (assimetria_residuo_lognormal == 0.0) {

			//
			// Em caso de Inviabilidade no cálculo de phi_residuo_lognormal, utiliza-se uma distribuição Lognormal a 2 partâmetros.
			//
			else if (phi_residuo_lognormal <= 1.0){
				sigma_residuo_normal = desvio_residuo_normal;
				mi_residuo_normal    = media_residuo_normal;
				delta_residuo_normal = -grau_liberdade_residuo_lognormal;
			} // else if (phi_residuo_lognormal <= 1.0){

			else {
				sigma_residuo_normal = std::sqrt(std::log(phi_residuo_lognormal));
				mi_residuo_normal    = 0.5* std::log(std::pow(desvio_residuo_lognormal, 2) / (phi_residuo_lognormal * (phi_residuo_lognormal - 1.0)));
				delta_residuo_normal = media_residuo_lognormal - std::pow((std::pow(desvio_residuo_lognormal, 2) / (phi_residuo_lognormal - 1.0)), 0.5) - grau_liberdade_residuo_lognormal;
			}

			enupla_mi_residuo_normal.addElemento    (idEstacao, mi_residuo_normal);
			enupla_sigma_residuo_normal.addElemento (idEstacao, sigma_residuo_normal);
			enupla_delta_residuo_normal.addElemento (idEstacao, delta_residuo_normal);
			enupla_phi_residuo_lognormal.addElemento(idEstacao, phi_residuo_lognormal);

		} // for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

		setVetor_forced(AttVetorVariavelAleatoria_mi_residuo_normal,     enupla_mi_residuo_normal);
		setVetor_forced(AttVetorVariavelAleatoria_sigma_residuo_normal,  enupla_sigma_residuo_normal);
		setVetor_forced(AttVetorVariavelAleatoria_delta_residuo_normal,  enupla_delta_residuo_normal);
		setVetor_forced(AttVetorVariavelAleatoria_phi_residuo_lognormal, enupla_phi_residuo_lognormal);

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularSigmaMiDeltaResiduoNormal(): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::calcularSigmaMiDeltaResiduoNormal(){



double VariavelAleatoria::calcularRuidoCorrelacionado(const TipoCorrelacaoVariaveisAleatorias a_tipo_correlacao_variaveis_aleatorias, const IdEstacao a_idEstacao, const SmartEnupla<IdVariavelAleatoria, double>& a_ruido_branco) {

	try {

		if (a_tipo_correlacao_variaveis_aleatorias == TipoCorrelacaoVariaveisAleatorias_matriz_carga) {

			double ruido_correlacionado = 0.0;
			for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= getIdObjeto(); idVariavelAleatoria++) {

				const double carga = getElementoMatriz(AttMatrizVariavelAleatoria_matriz_carga_residuo_lognormal, a_idEstacao, idVariavelAleatoria, double());

				const double ruido = a_ruido_branco.getElemento(idVariavelAleatoria);

				ruido_correlacionado += carga * ruido;

			} // for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= getIdObjeto(); idVariavelAleatoria++) {

			return ruido_correlacionado;

		} // if (tipo_correlacao_variaveis_aleatorias == TipoCorrelacaoVariaveisAleatorias_matriz_carga) {

		else if (a_tipo_correlacao_variaveis_aleatorias == TipoCorrelacaoVariaveisAleatorias_sem_correlacao)
			return a_ruido_branco.getElemento(getIdObjeto());

		else
			throw std::invalid_argument("Tipo de correlacao nao utilizada no modelo.");

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularRuidoCorrelacionado(" + getString(a_idEstacao) + ",a_ruido_branco): \n" + std::string(erro.what())); }

} // double VariavelAleatoria::calcularRuidoCorrelacionado(const TipoCorrelacaoVariaveisAleatorias a_tipo_correlacao_variaveis_aleatorias, const IdEstacao a_idEstacao, const SmartEnupla<IdVariavelAleatoria, double>& a_ruido_branco) {



void VariavelAleatoria::gerarRuidoBrancoEspacoAmostral(const SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>>& a_horizonte_espaco_amostral, const TipoSorteio a_tipo_sorteio, int& a_semente){

	try {

		if (a_horizonte_espaco_amostral.size() == 0)
			throw std::invalid_argument("Horizonte vazio.");

		const Periodo periodo_inicial = a_horizonte_espaco_amostral.getIteradorInicial();
		const Periodo periodo_final = a_horizonte_espaco_amostral.getIteradorFinal();

		const TipoPeriodo tipoPeriodo_serie = getIteradorInicial(AttVetorVariavelAleatoria_serie_temporal_transformada, Periodo()).getTipoPeriodo();

		if ((periodo_inicial.getTipoPeriodo() != periodo_final.getTipoPeriodo()) || (periodo_inicial.getTipoPeriodo() != tipoPeriodo_serie))
			throw std::invalid_argument("O mesmo tipo de periodo deve constar no argumento a_horizonte_amostra e na serie temporal da variavel aleatoria.");

		setMatriz_forced(AttMatrizVariavelAleatoria_ruido_branco_espaco_amostral, a_horizonte_espaco_amostral);

		for (Periodo periodo = periodo_inicial; periodo <= periodo_final; periodo++) {
			for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= a_horizonte_espaco_amostral.at(periodo).getIteradorFinal(); idRealizacao++)
				setElemento(AttMatrizVariavelAleatoria_ruido_branco_espaco_amostral, periodo, idRealizacao, getRuidoBranco(a_tipo_sorteio, 1, a_semente).at(0));
		} // for (Periodo periodo = periodo_inicial; periodo <= periodo_final; periodo++) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::gerarRuidoBrancoEspacoAmostral(a_horizonte_amostra," + getString(a_tipo_sorteio) + "): \n" + std::string(erro.what())); }

}

void VariavelAleatoria::gerarRuidoBrancoEspacoAmostral(const SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> &a_horizonte_espaco_amostral, const TipoSorteio a_tipo_sorteio, const int a_numero_maximo_periodos, int &a_semente) {
	try {

		if (a_horizonte_espaco_amostral.size() == 0)
			throw std::invalid_argument("Horizonte vazio.");

		const Periodo periodo_inicial = a_horizonte_espaco_amostral.getIteradorInicial();
		const Periodo periodo_final   = a_horizonte_espaco_amostral.getIteradorFinal();

		const TipoPeriodo tipoPeriodo_serie = getIteradorInicial(AttVetorVariavelAleatoria_serie_temporal_transformada, Periodo()).getTipoPeriodo();

		if ((periodo_inicial.getTipoPeriodo() != periodo_final.getTipoPeriodo()) || (periodo_inicial.getTipoPeriodo() != tipoPeriodo_serie))
			throw std::invalid_argument("O mesmo tipo de periodo deve constar no argumento a_horizonte_amostra e na serie temporal da variavel aleatoria.");

		setMatriz_forced(AttMatrizVariavelAleatoria_ruido_branco_espaco_amostral, a_horizonte_espaco_amostral);

		Periodo periodo = periodo_inicial;
		for (int p = 1; p <= a_numero_maximo_periodos; p++){

			for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao < IdRealizacao_Excedente; idRealizacao++) {

				if (periodo.isValido()) {
					if (idRealizacao <= a_horizonte_espaco_amostral.getElemento(periodo).getIteradorFinal())
						setElemento(AttMatrizVariavelAleatoria_ruido_branco_espaco_amostral, periodo, idRealizacao, getRuidoBranco(a_tipo_sorteio, 1, a_semente).at(0));
					else
						getRuidoBranco(a_tipo_sorteio, 1, a_semente);
				}
				else
					getRuidoBranco(a_tipo_sorteio, 1, a_semente);

			} // for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao < IdRealizacao_Excedente; idRealizacao++) {

			if (periodo.isValido()) {
				if (periodo < periodo_final)
					a_horizonte_espaco_amostral.incrementarIterador(periodo);
				else if (periodo == periodo_final)
					periodo = Periodo();
			}

		} // for (int p = 1; p <= a_numero_maximo_periodos; p++){

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::gerarRuidoBrancoEspacoAmostral(a_horizonte_amostra," + getString(a_tipo_sorteio) + "): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::gerarRuidoBrancoEspacoAmostral(const SmartEnupla<Periodo, IdRealizacao> &a_horizonte_amostra, const TipoSorteio a_tipo_sorteio, const int a_semente) {


void VariavelAleatoria::gerarEspacoAmostralFromRuido(const SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>>& a_horizonte_espaco_amostral){
	
	try {

		const Periodo periodo_inicial = getIterador1Inicial(AttMatrizVariavelAleatoria_ruido_correlacionado_espaco_amostral, Periodo());
		const Periodo periodo_final   = getIterador1Final  (AttMatrizVariavelAleatoria_ruido_correlacionado_espaco_amostral, Periodo());

		const TipoPeriodo tipoPeriodo = getIteradorInicial(AttVetorVariavelAleatoria_serie_temporal_transformada, Periodo()).getTipoPeriodo();

		if (tipoPeriodo != periodo_inicial.getTipoPeriodo())
			throw std::invalid_argument("Tipo de periodo no horizonte nao compativel com tipo de periodo da serie temporal.");

		setMatriz_forced(AttMatrizVariavelAleatoria_residuo_espaco_amostral, a_horizonte_espaco_amostral);

		for (Periodo periodo = periodo_inicial; periodo <= periodo_final; a_horizonte_espaco_amostral.incrementarIterador(periodo)) {

			const IdRealizacao realizacao_final = getIterador2Final(AttMatrizVariavelAleatoria_ruido_correlacionado_espaco_amostral, periodo, IdRealizacao());

			for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= realizacao_final; idRealizacao++) {

				const double ruido_correlacionado = getElementoMatriz(AttMatrizVariavelAleatoria_ruido_correlacionado_espaco_amostral, periodo, idRealizacao, double());

				const double residuo = calcularResiduo(ruido_correlacionado, periodo);

				setElemento(AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodo, idRealizacao, residuo);

			} // for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= maiorVariavelAleatoria; idVar++) {

		} // for (Periodo periodo = periodo_inicial; periodo <= periodo_final; periodo++) {

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::gerarEspacoAmostralFromRuido(): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::gerarEspacoAmostralFromRuido(const SmartEnupla<Periodo, IdRealizacao>& a_horizonte_amostra_comum, const TipoSorteio a_tipo_sorteio, int & a_semente){


void VariavelAleatoria::gerarCenariosEspacoAmostral(const SmartEnupla<IdCenario, IdCenario> &a_mapeamento_cenarios_tendencia, const SmartEnupla <IdCenario, SmartEnupla<Periodo, IdRealizacao>> &a_mapeamento_amostra_comum, const SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>& a_horizonte_processo_estocastico, const bool a_gerar_cenarios_buffer, const bool a_gerar_cenarios_internos, const bool a_espaco_amostral_mesmo_tipo_periodo){

	try{

		if (a_mapeamento_cenarios_tendencia.size() == 0)
			throw std::invalid_argument("O Argumento a_mapeamento_cenarios_tendencia deve possuir elementos.");

		setMatriz_forced(AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral, a_horizonte_processo_estocastico);

		if (a_gerar_cenarios_internos) {
			for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= getMaiorId(IdVariavelAleatoriaInterna()); idVariavelAleatoriaInterna++) {
				if (vetorVariavelAleatoriaInterna.att(idVariavelAleatoriaInterna).getSizeMatriz(AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral) == 0)
					vetorVariavelAleatoriaInterna.att(idVariavelAleatoriaInterna).setMatriz_forced(AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral, a_horizonte_processo_estocastico);
			}
		}

		const IdCenario cenario_inicial = a_horizonte_processo_estocastico.getIteradorInicial();
		const IdCenario cenario_final   = a_horizonte_processo_estocastico.getIteradorFinal();

		const Periodo periodo_inicial = a_horizonte_processo_estocastico.at(cenario_inicial).getIteradorInicial();
		const Periodo periodo_final   = a_horizonte_processo_estocastico.at(cenario_inicial).getIteradorFinal();

		Periodo periodo_inicial_amostra = periodo_final;
		a_horizonte_processo_estocastico.at(cenario_inicial).incrementarIterador(periodo_inicial_amostra);

		if (a_mapeamento_amostra_comum.size() > 0)
			periodo_inicial_amostra = a_mapeamento_amostra_comum.at(cenario_inicial).getIteradorInicial();

		for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

			//
			// Horizonte Tendencia
			//

			for (Periodo periodo = periodo_inicial; periodo < periodo_inicial_amostra; a_horizonte_processo_estocastico.at(idCenario).incrementarIterador(periodo)) {

				// Realização Transformada é a própria Tendencia Transformada

				setElemento(AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral, idCenario, periodo, getTendenciaTemporalTransformada(a_mapeamento_cenarios_tendencia, idCenario, periodo));

				if (a_gerar_cenarios_internos)
					setRealizacaoInternaFromTendencia(idCenario, a_mapeamento_cenarios_tendencia.getElemento(idCenario), periodo);

			} // for (Periodo periodo = periodo_inicial_tendencia; periodo <= periodo_final_amostra; periodo++) {

			//
			// Horizonte Espaco Amostral
			//

			for (Periodo periodo = periodo_inicial_amostra; periodo <= periodo_final; a_mapeamento_amostra_comum.at(idCenario).incrementarIterador(periodo)) {

				// Realização Transformada é obtida a partir da realização mapeada e do cenário.

				const IdRealizacao idRealizacao = a_mapeamento_amostra_comum.at(idCenario).getElemento(periodo);

				if (a_gerar_cenarios_buffer)
					setElemento(AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral, idCenario, periodo, getRealizacaoTransformadaEspacoAmostral(idCenario, idRealizacao, periodo, a_espaco_amostral_mesmo_tipo_periodo));
				else
					setElemento(AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral, idCenario, periodo, getRealizacaoTransformadaEspacoAmostral_recursivo(idCenario, idRealizacao, a_mapeamento_amostra_comum.at(idCenario), periodo, periodo, a_horizonte_processo_estocastico.at(idCenario), a_espaco_amostral_mesmo_tipo_periodo));

				if (a_gerar_cenarios_internos)
					setRealizacaoInterna(idCenario, periodo);

			} // for (Periodo periodo = periodo_inicial_amostra; periodo <= periodo_final; a_mapeamento_amostra_comum.at(cenario_inicial).incrementarIterador(periodo)) {

		} // for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_Final; idCenario++) {

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::gerarCenariosEspacoAmostral(a_mapeamento_cenarios_tendencia,a_mapeamento_amostra_comum): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::gerarCenariosEspacoAmostral(const SmartEnupla<IdCenario, IdCenario> a_mapeamento_cenarios_tendencia){

void VariavelAleatoria::expandirParametrosEspacoAmostral(const SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>>& a_horizonte_espaco_amostral){

	try {

		SmartEnupla<Periodo, SmartEnupla<int, double>> coeficiente_linear_auto_correlacao(a_horizonte_espaco_amostral, SmartEnupla<int, double>());

		SmartEnupla<Periodo, double> coeficiente_participacao(a_horizonte_espaco_amostral, 0.0);

		for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= getMaiorId(IdVariavelAleatoriaInterna()); idVariavelAleatoriaInterna++) {

			for (Periodo periodo = a_horizonte_espaco_amostral.getIteradorInicial(); periodo <= a_horizonte_espaco_amostral.getIteradorFinal(); a_horizonte_espaco_amostral.incrementarIterador(periodo)) {

				const IdEstacao idEstacao = periodo.getEstacao();

				if (idVariavelAleatoriaInterna == IdVariavelAleatoriaInterna_1) {
					for (int lag = 1; lag <= getIterador2Final(AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao_sazonal, idEstacao, int()); lag++)
						coeficiente_linear_auto_correlacao.at(periodo).addElemento(lag, getElementoMatriz(AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao_sazonal, idEstacao, lag, double()));
				} // if (idVariavelAleatoriaInterna == IdVariavelAleatoriaInterna_1) {

				coeficiente_participacao.at(periodo) = getElementoVetor(idVariavelAleatoriaInterna, AttVetorVariavelAleatoriaInterna_coeficiente_participacao_sazonal, idEstacao, double());

			} // for (Periodo periodo = a_horizonte_espaco_amostral.getIteradorInicial(); periodo <= a_horizonte_espaco_amostral.getIteradorFinal(); a_horizonte_espaco_amostral.incrementarIterador(periodo)) {

			if (idVariavelAleatoriaInterna == IdVariavelAleatoriaInterna_1)
				setMatriz_forced(AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, coeficiente_linear_auto_correlacao);

			vetorVariavelAleatoriaInterna.att(idVariavelAleatoriaInterna).setVetor_forced(AttVetorVariavelAleatoriaInterna_coeficiente_participacao, coeficiente_participacao);

		} // for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= getMaiorId(IdVariavelAleatoriaInterna()); idVariavelAleatoriaInterna++) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::expandirParametrosEspacoAmostral(a_horizonte_espaco_amostral): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::expandirParametrosEspacoAmostral(const SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>>& a_horizonte_espaco_amostral){

void VariavelAleatoria::expandirParametrosHorizonteCompleto(const SmartEnupla<Periodo, double>& a_horizonte_completo) {
	try {

		for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= getMaiorId(IdVariavelAleatoriaInterna()); idVariavelAleatoriaInterna++) {
			if (getSizeVetor(idVariavelAleatoriaInterna, AttVetorVariavelAleatoriaInterna_grau_liberdade) == 0) {
				SmartEnupla<Periodo, double> grau_liberdade(a_horizonte_completo, getAtributo(idVariavelAleatoriaInterna, AttComumVariavelAleatoriaInterna_grau_liberdade, double()));
				vetorVariavelAleatoriaInterna.att(idVariavelAleatoriaInterna).setVetor_forced(AttVetorVariavelAleatoriaInterna_grau_liberdade, grau_liberdade);
			} // if (getSizeVetor(idVariavelAleatoriaInterna, AttVetorVariavelAleatoriaInterna_grau_liberdade) == 0) {
		} // for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= getMaiorId(IdVariavelAleatoriaInterna()); idVariavelAleatoriaInterna++) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::expandirParametrosHorizonteCompleto(a_horizonte_completo): \n" + std::string(erro.what())); }
} // void VariavelAleatoria::expandirParametrosHorizonteCompleto(const SmartEnupla<Periodo, double>& a_horizonte_completo){


double VariavelAleatoria::getRealizacaoEspacoAmostral(const IdCenario a_idCenario, const Periodo a_periodo) const{

	try { 
		
		return getRealizacaoTransformadaEspacoAmostral(a_idCenario, a_periodo);
	
	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::getRealizacaoEspacoAmostral(" + getString(a_idCenario) + "," + getString(a_periodo) + "): \n" + std::string(erro.what())); }

} // double VariavelAleatoria::getRealizacaoEspacoAmostral(const IdCenario a_idCenario, const Periodo a_periodo) const{


double VariavelAleatoria::getRealizacaoEspacoAmostral(const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const Periodo a_periodo, const bool a_espaco_amostral_mesmo_tipo_periodo) {

	try {

		return getRealizacaoTransformadaEspacoAmostral(a_idCenario, a_idRealizacao, a_periodo, a_espaco_amostral_mesmo_tipo_periodo);

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::getRealizacaoEspacoAmostral(" + getString(a_idCenario) + "," + getString(a_periodo) + "): \n" + std::string(erro.what())); }

} // double VariavelAleatoria::getRealizacaoEspacoAmostral(const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const Periodo a_periodo) const{


double VariavelAleatoria::getRealizacaoEspacoAmostral(const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const SmartEnupla<Periodo, IdRealizacao> a_idRealizacoes_cenario, const Periodo a_periodo, const SmartEnupla<Periodo, IdRealizacao> a_horizonte_mapeamento_espaco_amostral, const bool a_espaco_amostral_mesmo_tipo_periodo){

	try { 
		
		SmartEnupla<Periodo, double> horizonte_completo;

		const SmartEnupla<Periodo, double> horizonte_tendencia = vetorVariavelAleatoriaInterna.att(IdVariavelAleatoriaInterna_1).getElementosMatriz(AttMatrizVariavelAleatoriaInterna_tendencia_temporal, IdCenario_1, Periodo(), double());

		for(Periodo periodo_tendencia = horizonte_tendencia.getIteradorInicial(); periodo_tendencia <= horizonte_tendencia.getIteradorFinal(); horizonte_tendencia.incrementarIterador(periodo_tendencia))
			horizonte_completo.addElemento(periodo_tendencia, 0.0);

		for (Periodo periodo_mapeamento_espaco_amostral = a_horizonte_mapeamento_espaco_amostral.getIteradorInicial(); periodo_mapeamento_espaco_amostral <= a_horizonte_mapeamento_espaco_amostral.getIteradorFinal(); a_horizonte_mapeamento_espaco_amostral.incrementarIterador(periodo_mapeamento_espaco_amostral))
			horizonte_completo.addElemento(periodo_mapeamento_espaco_amostral, 0.0);

		return getRealizacaoTransformadaEspacoAmostral_recursivo(a_idCenario, a_idRealizacao, a_idRealizacoes_cenario, a_periodo, a_periodo, horizonte_completo, a_espaco_amostral_mesmo_tipo_periodo);
	
	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::getRealizacaoEspacoAmostral(" + getString(a_idCenario) + "," + getString(a_periodo) + "): \n" + std::string(erro.what())); }

} // double VariavelAleatoria::getRealizacaoEspacoAmostral(const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const Periodo a_periodo) const{

double VariavelAleatoria::getRealizacaoTransformadaEspacoAmostral(const IdCenario a_idCenario, const Periodo a_periodo) const {

	try { return getElementoMatriz(AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral, a_idCenario, a_periodo, double()); } // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::getRealizacaoTransformadaEspacoAmostral(" + getString(a_idCenario) + "," + getString(a_periodo) + "): \n" + std::string(erro.what())); }

} // double VariavelAleatoria::getRealizacaoTransformadaEspacoAmostral(const IdCenario a_idCenario, const Periodo a_periodo) const{


double VariavelAleatoria::getRealizacaoTransformadaEspacoAmostral(const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const Periodo a_periodo, const bool a_espaco_amostral_mesmo_tipo_periodo) const{

	try{
		// Adicionado 1 na ordem_auto_correlacao para que a SmartEnupla tendencia_por_periodo nao fique vazia em caso de AttVetorVariavelAleatoria_ordem_auto_correlacao = 0.
		const int ordem_auto_correlacao = getSizeMatriz(AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, a_periodo);

		SmartEnupla<Periodo, double> tendencia_por_periodo;

		const SmartEnupla<Periodo, double> horizonte = getElementosMatriz(AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral, a_idCenario, Periodo(), double());

		for (int lag = ordem_auto_correlacao; lag >= 1; lag--) {

			Periodo periodo_lag = a_periodo;

			if (a_espaco_amostral_mesmo_tipo_periodo)
				periodo_lag = a_periodo - lag;

			else {
				for (int i = 0; i < lag; i++)
					horizonte.decrementarIterador(periodo_lag);		
			}

			tendencia_por_periodo.addElemento(periodo_lag, 0.0);

			bool sobreposicao_encontrada = false;
			for (Periodo periodo = horizonte.getIteradorInicial(); periodo <= horizonte.getIteradorFinal(); horizonte.incrementarIterador(periodo)) {

				const double sobreposicao = periodo_lag.sobreposicao(periodo);

				if (sobreposicao > 0.0) {
					tendencia_por_periodo.at(periodo_lag) += sobreposicao * getElementoMatriz(AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral, a_idCenario, periodo, double());
					sobreposicao_encontrada = true;
				}
				else if ((sobreposicao == 0.0) && (sobreposicao_encontrada))
					break;

			} // for (Periodo periodo = horizonte.getIteradorInicial(); periodo <= horizonte.getIteradorFinal(); horizonte.incrementarIterador(periodo)) {

		} // for (int lag = ordem_auto_correlacao; lag >= 1; lag--) {

		return calcularRealizacao(a_periodo, tendencia_por_periodo, getElementoMatriz(AttMatrizVariavelAleatoria_residuo_espaco_amostral, a_periodo, a_idRealizacao, double()), a_espaco_amostral_mesmo_tipo_periodo);

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::getRealizacaoTransformadaEspacoAmostral(" + getString(a_idCenario) + "," + getString(a_idRealizacao) + "," + getString(a_periodo) + "): \n" + std::string(erro.what())); }

} // double VariavelAleatoria::getRealizacaoTransformadaEspacoAmostral(const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const Periodo a_periodo) const{


double VariavelAleatoria::getRealizacaoTransformadaEspacoAmostral_recursivo(const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const SmartEnupla<Periodo, IdRealizacao> a_idRealizacoes_cenario, const Periodo a_periodo, const Periodo a_periodo_realizacao, const SmartEnupla<Periodo, double> a_horizonte_completo, const bool a_espaco_amostral_mesmo_tipo_periodo) {

	try {
		
		IdRealizacao idRealizacao = IdRealizacao_Nenhum;

		if (a_periodo == a_periodo_realizacao)
			idRealizacao = a_idRealizacao;
		else
			idRealizacao = a_idRealizacoes_cenario.getElemento(a_periodo);

		// Adicionado 1 na ordem_auto_correlacao para que a SmartEnupla tendencia_por_periodo nao fique vazia em caso de AttVetorVariavelAleatoria_ordem_auto_correlacao = 0.
		const int ordem_auto_correlacao = getSizeMatriz(AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, a_periodo);

		SmartEnupla<Periodo, double> tendencia_por_periodo;

		const Periodo periodo_tendencia_final = vetorVariavelAleatoriaInterna.att(IdVariavelAleatoriaInterna_1).getIterador2Final(AttMatrizVariavelAleatoriaInterna_tendencia_temporal, IdCenario_1, Periodo());

		for (int lag = ordem_auto_correlacao; lag >= 1; lag--) {

			Periodo periodo_lag = a_periodo;

			if (a_espaco_amostral_mesmo_tipo_periodo)
				periodo_lag = a_periodo - lag;

			else {
				for (int i = 0; i < lag; i++)
					a_horizonte_completo.decrementarIterador(periodo_lag);
			}

			tendencia_por_periodo.addElemento(periodo_lag, 0.0);

			bool sobreposicao_encontrada = false;
			for (Periodo periodo = a_horizonte_completo.getIteradorInicial(); periodo <= a_horizonte_completo.getIteradorFinal(); a_horizonte_completo.incrementarIterador(periodo)) {

				const double sobreposicao = periodo_lag.sobreposicao(periodo);

				if (sobreposicao > 0.0) {
					
					sobreposicao_encontrada = true;

					if (periodo <= periodo_tendencia_final) {

						//Pega o valor da tendência, o qual está dentro da VariavelAleatoriaInterna

						double valor_realizacao = 0;

						const SmartEnupla<Periodo, double> horizonte_tendencia = vetorVariavelAleatoriaInterna.att(IdVariavelAleatoriaInterna_1).getElementosMatriz(AttMatrizVariavelAleatoriaInterna_tendencia_temporal, IdCenario_1, Periodo(), double());

						const Periodo periodo_tendencia_inicial = horizonte_tendencia.getIteradorInicial();

						const IdVariavelAleatoriaInterna maiorIdVariavelAleatoriaInterna = getMaiorId(IdVariavelAleatoriaInterna());

						bool sobreposicao_encontrada = false;
						for (Periodo periodo_tendencia = periodo_tendencia_inicial; periodo_tendencia <= periodo_tendencia_final; horizonte_tendencia.incrementarIterador(periodo_tendencia)) {

							const double sobreposicao = periodo_lag.sobreposicao(periodo_tendencia);

							if (sobreposicao > 0.0) {

								for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= maiorIdVariavelAleatoriaInterna; idVariavelAleatoriaInterna++)
									valor_realizacao += sobreposicao * (vetorVariavelAleatoriaInterna.att(idVariavelAleatoriaInterna).getElementoMatriz(AttMatrizVariavelAleatoriaInterna_tendencia_temporal, IdCenario_1, periodo_tendencia, double()) + vetorVariavelAleatoriaInterna.att(idVariavelAleatoriaInterna).getElementoVetor(AttVetorVariavelAleatoriaInterna_grau_liberdade, periodo_tendencia, double()));

								sobreposicao_encontrada = true;
							}
							else if ((sobreposicao == 0.0) && (sobreposicao_encontrada))
								break;

						}//for (Periodo periodo_tendencia = periodo_tendencia_inicial; periodo_tendencia <= periodo_tendencia_final; horizonte_tendencia.incrementarIterador(periodo_tendencia)) {

						tendencia_por_periodo.at(periodo_lag) = valor_realizacao;

					}//if (a_periodo <= vetorVariavelAleatoriaInterna.att(IdVariavelAleatoriaInterna_1).getIterador2Final(AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral, a_idCenario, Periodo())) {
					else
						tendencia_por_periodo.at(periodo_lag) += sobreposicao * getRealizacaoTransformadaEspacoAmostral_recursivo(a_idCenario, a_idRealizacao, a_idRealizacoes_cenario, periodo_lag, a_periodo_realizacao, a_horizonte_completo, a_espaco_amostral_mesmo_tipo_periodo);

				}
				else if ((sobreposicao == 0.0) && (sobreposicao_encontrada))
					break;

			} // for (Periodo periodo = a_horizonte_completo.getIteradorInicial(); periodo <= a_horizonte_completo.getIteradorFinal(); a_horizonte_completo.incrementarIterador(periodo)) {

		} // for (int lag = ordem_auto_correlacao; lag >= 1; lag--) {

		return calcularRealizacao(a_periodo, tendencia_por_periodo, getElementoMatriz(AttMatrizVariavelAleatoria_residuo_espaco_amostral, a_periodo, a_idRealizacao, double()), a_espaco_amostral_mesmo_tipo_periodo);

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::getRealizacaoTransformadaEspacoAmostral_recursivo(" + getString(a_idCenario)  + "," + getString(a_periodo) + "): \n" + std::string(erro.what())); }

} // double VariavelAleatoria::getRealizacaoTransformadaEspacoAmostral_recursivo(const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const Periodo a_periodo) const{


double VariavelAleatoria::getTendenciaTemporalTransformada(const SmartEnupla<IdCenario, IdCenario> a_mapeamento_cenarios_tendencia, const IdCenario a_idCenario, const Periodo a_periodo){

	try {

		const double tendencia_temporal_transformada = getTendenciaTemporalTransformadaFromVariavelAleatoriaInterna(a_mapeamento_cenarios_tendencia, a_idCenario, a_periodo);

		if (!std::isnan(tendencia_temporal_transformada))
			return tendencia_temporal_transformada;

		if (getSizeMatriz(AttMatrizVariavelAleatoria_tendencia_temporal_transformada) > 0) {

			if (a_mapeamento_cenarios_tendencia.size() > 0)
				return getElementoMatriz(AttMatrizVariavelAleatoria_tendencia_temporal_transformada, a_mapeamento_cenarios_tendencia.getElemento(a_idCenario), a_periodo, double());

		} // if (getSizeMatriz(AttMatrizVariavelAleatoria_tendencia_temporal_transformada) > 0) {

		if (getSizeMatriz(AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral) > 0) {

			if ((getIterador2Inicial(AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral, a_idCenario, Periodo()) <= a_periodo) && \
				(getIterador2Final(AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral, a_idCenario, Periodo()) >= a_periodo))
				return getElementoMatriz(AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral, a_idCenario, a_periodo, double());

		} // if (getSizeMatriz(AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral) > 0) {

		throw std::invalid_argument("Tendencia Temporal nao encontrada");

		return NAN;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::getTendenciaTemporalTransformada(" + getString(a_idCenario) + "," + getString(a_periodo) + "): \n" + std::string(erro.what())); }


} // double VariavelAleatoria::getTendenciaTemporalTransformada(const IdCenario a_idCenario, const Periodo a_periodo){


double VariavelAleatoria::getTendenciaTemporalTransformadaFromVariavelAleatoriaInterna(const SmartEnupla<IdCenario, IdCenario> a_mapeamento_cenarios_tendencia, const IdCenario a_idCenario, const Periodo a_periodo) {

	try {

		double tendencia_temporal_transformada = 0.0;

		bool tendencia_temporal_interna_encontrada = false;

		for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= getMaiorId(IdVariavelAleatoriaInterna()); idVariavelAleatoriaInterna++) {

			tendencia_temporal_interna_encontrada = false;

			if (getSize1Matriz(idVariavelAleatoriaInterna, AttMatrizVariavelAleatoriaInterna_tendencia_temporal_transformada) > 0) {
				tendencia_temporal_transformada += getElementoMatriz(idVariavelAleatoriaInterna, AttMatrizVariavelAleatoriaInterna_tendencia_temporal_transformada, a_mapeamento_cenarios_tendencia.getElemento(a_idCenario), a_periodo, double());
				tendencia_temporal_interna_encontrada = true;
			}

			if ((!tendencia_temporal_interna_encontrada) && (getSize1Matriz(idVariavelAleatoriaInterna, AttMatrizVariavelAleatoriaInterna_tendencia_temporal) > 0)) {
				tendencia_temporal_transformada += getElementoMatriz(idVariavelAleatoriaInterna, AttMatrizVariavelAleatoriaInterna_tendencia_temporal, a_mapeamento_cenarios_tendencia.getElemento(a_idCenario), a_periodo, double()) + getElementoVetor(idVariavelAleatoriaInterna, AttVetorVariavelAleatoriaInterna_grau_liberdade, a_periodo, double());
				tendencia_temporal_interna_encontrada = true;
			}

			if ((!tendencia_temporal_interna_encontrada) && (getSize1Matriz(idVariavelAleatoriaInterna, AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral) > 0)) {
				tendencia_temporal_transformada += getElementoMatriz(idVariavelAleatoriaInterna, AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral, a_idCenario, a_periodo, double()) + getElementoVetor(idVariavelAleatoriaInterna, AttVetorVariavelAleatoriaInterna_grau_liberdade, a_periodo, double());
				tendencia_temporal_interna_encontrada = true;
			}

			if (!tendencia_temporal_interna_encontrada)
				break;

		} // for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= getMaiorId(IdVariavelAleatoriaInterna()); idVariavelAleatoriaInterna++) {

		if (tendencia_temporal_interna_encontrada)
			return tendencia_temporal_transformada;
		else
			return NAN;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::getTendenciaTemporalTransformadaFromVariavelAleatoriaInterna(" + getString(a_idCenario) + "," + getString(a_periodo) + "): \n" + std::string(erro.what())); }

} // double VariavelAleatoria::getTendenciaTemporalTransformadaFromVariavelAleatoriaInterna(const IdCenario a_idCenario, const Periodo a_periodo) {


double VariavelAleatoria::calcularResiduo(double a_ruido_correlacionado, const Periodo a_periodo) const {

	try {

		const TipoModeloGeracaoSinteticaCenario tipo_modelo_geracao_sintetica = getAtributo(AttComumVariavelAleatoria_tipo_modelo_geracao_sintetica, TipoModeloGeracaoSinteticaCenario());

		if (tipo_modelo_geracao_sintetica == TipoModeloGeracaoSinteticaCenario_lognormal_3p_sazonal)
			return calcularResiduo_lognormal_3p (a_ruido_correlacionado, a_periodo);

		else
			throw std::invalid_argument("TipoModeloGeracaoSinteticaCenario nao utilizado.");

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularResiduo(" + getString(a_ruido_correlacionado) + "," + getString(a_periodo) + "): \n" + std::string(erro.what())); }

} // double VariavelAleatoria::calcularResiduo(const TipoModeloGeracaoSinteticaCenario a_tipo_modelo_geracao_sintetica, double a_ruido_correlacionado, const Periodo a_periodo){



double VariavelAleatoria::calcularResiduo(const Periodo a_periodo, const SmartEnupla<Periodo, double>& a_tendencia, const double a_realizacao, const bool a_espaco_amostral_mesmo_tipo_periodo) const {

	try {

		if (std::isnan(a_realizacao) || (std::isinf(a_realizacao)))
			throw std::invalid_argument("Realizacao invalida (NaN ou inf.");

		const TipoModeloGeracaoSinteticaCenario tipo_modelo_geracao_sintetica = getAtributo(AttComumVariavelAleatoria_tipo_modelo_geracao_sintetica, TipoModeloGeracaoSinteticaCenario());

		if (tipo_modelo_geracao_sintetica == TipoModeloGeracaoSinteticaCenario_lognormal_3p_sazonal)
			return a_realizacao - calcularRegressivo_lognormal_3p(a_periodo, a_tendencia, a_espaco_amostral_mesmo_tipo_periodo);

		else
			throw std::invalid_argument("TipoModeloGeracaoSinteticaCenario nao utilizado.");

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularResiduo(a_tendencia," + getString(a_realizacao) + "): \n" + std::string(erro.what())); }

} // double VariavelAleatoria::calcularResiduo(const TipoModeloGeracaoSinteticaCenario a_tipo_modelo_geracao_sintetica, const SmartEnupla<Periodo, double>& a_tendencia, const double a_realizacao) {


double VariavelAleatoria::calcularRealizacao(const Periodo a_periodo, const SmartEnupla<Periodo, double>& a_tendencia, const double a_residuo, const bool a_espaco_amostral_mesmo_tipo_periodo) const {

	try {

		if (std::isnan(a_residuo) || (std::isinf(a_residuo)))
			throw std::invalid_argument("Residuo invalido (NaN ou inf.");

		const TipoModeloGeracaoSinteticaCenario tipo_modelo_geracao_sintetica = getAtributo(AttComumVariavelAleatoria_tipo_modelo_geracao_sintetica, TipoModeloGeracaoSinteticaCenario());

		if (tipo_modelo_geracao_sintetica == TipoModeloGeracaoSinteticaCenario_lognormal_3p_sazonal)
			return a_residuo + calcularRegressivo_lognormal_3p(a_periodo, a_tendencia, a_espaco_amostral_mesmo_tipo_periodo);

		else
			throw std::invalid_argument("TipoModeloGeracaoSinteticaCenario nao utilizado.");

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularRealizacao(a_tendencia," + getString(a_residuo) + "): \n" + std::string(erro.what())); }

} // double VariavelAleatoria::calcularRealizacao(const TipoModeloGeracaoSinteticaCenario a_tipo_modelo_geracao_sintetica, const SmartEnupla<Periodo, double>& a_tendencia, const double a_residuo) {



void VariavelAleatoria::calcularRealizacaoAndResiduo(double a_ruido_correlacionado, const Periodo a_periodo, const SmartEnupla<Periodo, double>& a_tendencia, double & a_valor_realizacao, double & a_valor_residuo_realizacao, const bool a_espaco_amostral_mesmo_tipo_periodo) const {

	try {

		double residuo = 0.0;
		double regressivo = 0.0;

		const TipoModeloGeracaoSinteticaCenario tipo_modelo_geracao_sintetica = getAtributo(AttComumVariavelAleatoria_tipo_modelo_geracao_sintetica, TipoModeloGeracaoSinteticaCenario());

		if (tipo_modelo_geracao_sintetica == TipoModeloGeracaoSinteticaCenario_lognormal_3p_sazonal) {
			residuo    = calcularResiduo_lognormal_3p   (a_ruido_correlacionado, a_periodo);
			regressivo = calcularRegressivo_lognormal_3p(a_periodo, a_tendencia, a_espaco_amostral_mesmo_tipo_periodo);
		} // if (tipo_modelo_geracao_sintetica == TipoModeloGeracaoSinteticaCenario_lognormal_3p_sazonal) {

		else
			throw std::invalid_argument("TipoModeloGeracaoSinteticaCenario nao utilizado.");

		double realizacao = regressivo + residuo;

		a_valor_realizacao         = realizacao;
		a_valor_residuo_realizacao = residuo;

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularRealizacaoAndResiduo(" + getString(a_ruido_correlacionado) + ",a_tendencia,&a_valor_realizacao,&a_valor_residuo_realizacao): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::calcularRealizacaoAndResiduo(double a_ruido_correlacionado, const SmartEnupla<Periodo, double>& a_tendencia, double & a_valor_realizacao, double & a_valor_residuo_realizacao){


double VariavelAleatoria::calcularRealizacaoInterna(const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const Periodo a_periodo, const double a_realizacao){
	try {

		return vetorVariavelAleatoriaInterna.att(a_idVariavelAleatoriaInterna).calcularRealizacao(a_realizacao, a_periodo);

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularRealizacaoInterna(" + getFullString(a_idVariavelAleatoriaInterna) + "," + getFullString(a_periodo) + "," + getFullString(a_realizacao) + "): \n" + std::string(erro.what())); }
}

double VariavelAleatoria::calcularRealizacaoInterna(const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const IdCenario a_idCenario, const Periodo a_periodo) {

	try {

		const double realizacao = getRealizacaoEspacoAmostral(a_idCenario, a_periodo);

		return vetorVariavelAleatoriaInterna.att(a_idVariavelAleatoriaInterna).calcularRealizacao(realizacao, a_periodo);

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularRealizacaoInterna(" + getFullString(a_idVariavelAleatoriaInterna) + "," + getFullString(a_idCenario) + "," + getFullString(a_periodo) + "): \n" + std::string(erro.what())); }

} // double VariavelAleatoria::calcularRealizacaoInterna(const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const IdCenario a_idCenario, const Periodo a_periodo) {



double VariavelAleatoria::calcularRealizacaoInterna(const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const Periodo a_periodo, const bool a_espaco_amostral_mesmo_tipo_periodo) {

	try {

		const double realizacao = getRealizacaoEspacoAmostral(a_idCenario, a_idRealizacao, a_periodo, a_espaco_amostral_mesmo_tipo_periodo);

		return vetorVariavelAleatoriaInterna.att(a_idVariavelAleatoriaInterna).calcularRealizacao(realizacao, a_periodo);

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularRealizacaoInterna(" + getFullString(a_idVariavelAleatoriaInterna) + "," + getFullString(a_idCenario) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_periodo) + "): \n" + std::string(erro.what())); }

} // double VariavelAleatoria::calcularRealizacaoInterna(const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const Periodo a_periodo) {


double VariavelAleatoria::calcularRealizacaoInterna(const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const Periodo a_periodo, const SmartEnupla<Periodo, IdRealizacao> a_idRealizacoes_cenario, const SmartEnupla<Periodo, IdRealizacao> a_horizonte_mapeamento_espaco_amostral, const bool a_espaco_amostral_mesmo_tipo_periodo) {

	try {

		const double realizacao = getRealizacaoEspacoAmostral(a_idCenario, a_idRealizacao, a_idRealizacoes_cenario, a_periodo, a_horizonte_mapeamento_espaco_amostral, a_espaco_amostral_mesmo_tipo_periodo);

		return vetorVariavelAleatoriaInterna.att(a_idVariavelAleatoriaInterna).calcularRealizacao(realizacao, a_periodo);

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularRealizacaoInterna(" + getFullString(a_idVariavelAleatoriaInterna) + "," + getFullString(a_idCenario) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_periodo) + "): \n" + std::string(erro.what())); }

} // double VariavelAleatoria::calcularRealizacaoInterna(const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const Periodo a_periodo) {

double VariavelAleatoria::calcularRealizacaoParaValor(const double a_valor, const Periodo a_periodo){

	try {

		double maior_realizacao = vetorVariavelAleatoriaInterna.att(IdVariavelAleatoriaInterna_1).calcularRealizacaoVariavelAleatoriaParaValor(a_valor, a_periodo);

		for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_2; idVariavelAleatoriaInterna <= getMaiorId(IdVariavelAleatoriaInterna()); idVariavelAleatoriaInterna++) {
			double realizacao = vetorVariavelAleatoriaInterna.att(idVariavelAleatoriaInterna).calcularRealizacaoVariavelAleatoriaParaValor(a_valor, a_periodo);

			if (realizacao > maior_realizacao)
				maior_realizacao = realizacao;

		} // for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_2; idVariavelAleatoriaInterna <= getMaiorId(IdVariavelAleatoriaInterna()); idVariavelAleatoriaInterna++) {

		return maior_realizacao;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularRealizacaoParaValor(" + getFullString(a_periodo) + "): \n" + std::string(erro.what())); }

} // double VariavelAleatoria::calcularRealizacaoParaValor(const double a_valor, const Periodo a_periodo){


void VariavelAleatoria::addRealizacaoInterna(const IdCenario a_idCenario, const Periodo a_periodo){

	try{

		const double realizacao = getRealizacaoEspacoAmostral(a_idCenario, a_periodo);

		const IdVariavelAleatoriaInterna maiorIdVariavelAleatoria = getMaiorId(IdVariavelAleatoriaInterna());

		for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= maiorIdVariavelAleatoria; idVariavelAleatoriaInterna++)
			vetorVariavelAleatoriaInterna.att(idVariavelAleatoriaInterna).addRealizacao(realizacao, a_idCenario, a_periodo);

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::addRealizacaoInterna(" + getFullString(a_idCenario) + "," + getFullString(a_periodo) + "): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::addRealizacaoInterna(const IdCenario a_idCenario, const Periodo a_periodo){


void VariavelAleatoria::setRealizacaoInterna(const IdCenario a_idCenario, const Periodo a_periodo) {

	try {

		const double realizacao = getRealizacaoEspacoAmostral(a_idCenario, a_periodo);

		const IdVariavelAleatoriaInterna maiorIdVariavelAleatoria = getMaiorId(IdVariavelAleatoriaInterna());

		for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= maiorIdVariavelAleatoria; idVariavelAleatoriaInterna++)
			vetorVariavelAleatoriaInterna.att(idVariavelAleatoriaInterna).setRealizacao(realizacao, a_idCenario, a_periodo);

	} // try{
	catch (const std::exception & erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::setRealizacaoInterna(" + getFullString(a_idCenario) + "," + getFullString(a_periodo) + "): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::addRealizacaoInterna(const IdCenario a_idCenario, const Periodo a_periodo){


void VariavelAleatoria::setRealizacaoInternaFromTendencia(const IdCenario a_idCenario, const IdCenario a_idCenario_tendencia, const Periodo a_periodo) {

	try {

		const IdVariavelAleatoriaInterna maiorIdVariavelAleatoria = getMaiorId(IdVariavelAleatoriaInterna());

		for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= maiorIdVariavelAleatoria; idVariavelAleatoriaInterna++)
			vetorVariavelAleatoriaInterna.att(idVariavelAleatoriaInterna).setRealizacaoFromTendencia(a_idCenario, a_idCenario_tendencia, a_periodo);

	} // try{
	catch (const std::exception & erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::setRealizacaoInternaFromTendencia(" + getFullString(a_idCenario) + "," + getFullString(a_periodo) + "): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::addRealizacaoInterna(const IdCenario a_idCenario, const Periodo a_periodo){


double VariavelAleatoria::calcularRegressivo_lognormal_3p(const Periodo a_periodo, const SmartEnupla<Periodo, double>& a_tendencia, const bool a_espaco_amostral_mesmo_tipo_periodo)const {

	try {

		double parcela_regressiva_realizacao = 0.0;
		const int lag_final = getIterador2Final(AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, a_periodo, int());

		Periodo periodo_final_tendencia;
		for (Periodo periodo = a_tendencia.getIteradorFinal(); periodo >= a_tendencia.getIteradorInicial(); a_tendencia.decrementarIterador(periodo)) {
			if (Periodo(a_periodo.getTipoPeriodo(), periodo + 1) == a_periodo) {
				periodo_final_tendencia = periodo;
				break;
			}
		}

		if (!periodo_final_tendencia.isValido())
			throw std::invalid_argument("Nao foi possivel encontrar periodo final da tendencia.");

		int lag = 1;
		Periodo periodo_lag;
		
		if (a_espaco_amostral_mesmo_tipo_periodo)
			periodo_lag = a_periodo - lag;

		else
			periodo_lag = periodo_final_tendencia;

		for (Periodo periodo = periodo_final_tendencia; periodo >= a_tendencia.getIteradorInicial(); a_tendencia.decrementarIterador(periodo)) {

			const double sobreposicao = periodo_lag.sobreposicao(periodo);

			if (sobreposicao == 0.0)
				throw std::invalid_argument("Erro ao calcular parcela autoregressiva.");

			const double coeficiente_linear = getElementoMatriz(AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, a_periodo, lag, double());

			parcela_regressiva_realizacao += coeficiente_linear * a_tendencia.getElemento(periodo) * sobreposicao;

			Periodo periodo_anterior = periodo;
			a_tendencia.decrementarIterador(periodo_anterior);

			if (periodo_anterior < periodo_lag) {
				lag++;
				if (a_espaco_amostral_mesmo_tipo_periodo)
					periodo_lag = a_periodo - lag;
				else
					a_tendencia.decrementarIterador(periodo_lag);
			}

			if (lag_final < lag)
				break;

		} // for (Periodo periodo = a_tendencia.getIteradorInicial(); periodo <= a_tendencia.getIteradorFinal(); a_tendencia.incrementarIterador(periodo)) {

		if (std::isnan(parcela_regressiva_realizacao))
			throw std::invalid_argument("Regressivo invalido (NaN ou inf.");

		return parcela_regressiva_realizacao;

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularRegressivo_lognormal_3p(" + getFullString(a_periodo) + ",a_tendencia): \n" + std::string(erro.what())); }

} // void VariavelAleatoria::calcularRegressivo_lognormal_3p(const SmartEnupla<IdVariavelAleatoria, double>& a_ruido_branco, const SmartEnupla<Periodo, double>& a_tendencia){



double VariavelAleatoria::calcularResiduo_lognormal_3p(double a_ruido_correlacionado, const Periodo a_periodo) const {


	try {

		const IdEstacao idEstacao = a_periodo.getEstacao();

		const double media = getElementoVetor(AttVetorVariavelAleatoria_media_serie_transformada, idEstacao, double());
		const double desvio = getElementoVetor(AttVetorVariavelAleatoria_desvio_serie_transformada, idEstacao, double());

		const double desvio_residuo_lognormal = getElementoVetor(AttVetorVariavelAleatoria_desvio_residuo_lognormal, idEstacao, double());
		const double assimetria_residuo_lognormal = getElementoVetor(AttVetorVariavelAleatoria_assimetria_residuo_lognormal, idEstacao, double());

		const int ordem_maxima = getSizeMatriz(AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, a_periodo);

		const Periodo periodo_maior_lag = a_periodo - ordem_maxima;

		double parcela_regressiva_residuo = media;
		for (Periodo periodo = periodo_maior_lag; periodo < a_periodo; periodo++) {

			const double coeficiente_linear = getElementoMatriz(AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, a_periodo, a_periodo - periodo, double());

			const double media_lag = getElementoVetor(AttVetorVariavelAleatoria_media_serie_transformada, periodo.getEstacao(), double());

			parcela_regressiva_residuo -= coeficiente_linear * media_lag;

		} // for (Periodo periodo = periodo_maior_lag; periodo < a_periodo; periodo++) {

		const double sigma_residuo_normal = getElementoVetor(AttVetorVariavelAleatoria_sigma_residuo_normal, idEstacao, double());
		const double mi_residuo_normal    = getElementoVetor(AttVetorVariavelAleatoria_mi_residuo_normal,    idEstacao, double());
		const double delta_residuo_normal = getElementoVetor(AttVetorVariavelAleatoria_delta_residuo_normal, idEstacao, double());

		double residuo_aleatorio = 0.0;

		// Residuo Normal
		if ((desvio_residuo_lognormal == 0) || (assimetria_residuo_lognormal == 0))
			residuo_aleatorio = a_ruido_correlacionado * sigma_residuo_normal + mi_residuo_normal + delta_residuo_normal;

		// Residuo Lognormal
		else
			residuo_aleatorio = std::exp(a_ruido_correlacionado * sigma_residuo_normal + mi_residuo_normal) + delta_residuo_normal;

		const double residuo = residuo_aleatorio * desvio + parcela_regressiva_residuo;

		if (std::isnan(residuo))
			throw std::invalid_argument("Residuo invalido (NaN ou inf.");

		return residuo;
		
	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("VariavelAleatoria(" + getString(getIdObjeto()) + ")::calcularResiduo_lognormal_3p("  + getString(a_ruido_correlacionado) + "," + getString(a_periodo) + "): \n" + std::string(erro.what())); }

} // double VariavelAleatoria::calcularResiduo_lognormal_3p(double a_ruido_correlacionado, const Periodo a_periodo){
