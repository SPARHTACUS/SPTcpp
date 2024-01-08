#include "C_ProcessoEstocastico.h"

#include "C_EntradaSaidaDados.h"

#include <chrono>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static const int numero_maximo_periodos_mapeamento_completo_espaco_amostral =   12;
static const int numero_maximo_cenarios_mapeamento_completo_espaco_amostral = 2048;

static const int numero_maximo_periodos_espaco_amostral = 240;

ProcessoEstocastico::ProcessoEstocastico(){
	try { INICIALIZA_SMART_ELEMENTO(ProcessoEstocastico, SMART_ELEMENTO_PROCESSO_ESTOCASTICO)	}
	catch (const std::exception&erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::ProcessoEstocastico(): \n" + std::string(erro.what())); }
}

ProcessoEstocastico::ProcessoEstocastico(const ProcessoEstocastico &instanciaCopiar){
	try { COPIA_SMART_ELEMENTO(ProcessoEstocastico, SMART_ELEMENTO_PROCESSO_ESTOCASTICO)	}
	catch (const std::exception&erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::ProcessoEstocastico(const ProcessoEstocastico &instanciaCopiar): \n" + std::string(erro.what())); }
}

ProcessoEstocastico & ProcessoEstocastico::operator=(const ProcessoEstocastico & instanciaCopiar){
	try {
		COPIA_SMART_ELEMENTO(ProcessoEstocastico, SMART_ELEMENTO_PROCESSO_ESTOCASTICO)
		return *this;
	}
	catch (const std::exception&erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::operator=(const ProcessoEstocastico &instanciaCopiar): \n" + std::string(erro.what())); }
}

void ProcessoEstocastico::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(ProcessoEstocastico, SMART_ELEMENTO_PROCESSO_ESTOCASTICO) }
	catch (const std::exception& erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::esvaziar(): \n" + std::string(erro.what())); }
}

ProcessoEstocastico::~ProcessoEstocastico(){ 
	EXCLUI_SMART_ELEMENTO(ProcessoEstocastico, SMART_ELEMENTO_PROCESSO_ESTOCASTICO)
}



void ProcessoEstocastico::gerarTendenciaTemporalMedia(const Periodo a_periodo_final){

	try{

		const IdVariavelAleatoria maiorVariavelAleatoria = getMaiorIdVariavelAleatoria();

		for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= maiorVariavelAleatoria; idVar++)
			vetorVariavelAleatoria.att(idVar).gerarTendenciaTemporalMediaVariavelAleatoriaInterna(a_periodo_final);

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::gerarTendenciaTemporalMedia(" + getString(a_periodo_final) + "): \n" + std::string(erro.what())); }

} // void ProcessoEstocastico::gerarTendenciaTemporalMedia(const Periodo a_periodo_inicial, const Periodo a_periodo_final){



void ProcessoEstocastico::parametrizarModelo(const EntradaSaidaDados &a_entradaSaidaDados, const bool a_imprimir_parametros, const TipoModeloGeracaoSinteticaCenario a_tipo_modelo_geracao_sintetica, const TipoValor a_tipo_coeficiente_auto_correlacao, const int a_ordem_maxima_coeficiente_auto_correlacao, const TipoCorrelacaoVariaveisAleatorias a_tipo_correlacao_variaveis_aleatorias, const double a_valor_determinacao_correlacao){

	try {

		const IdVariavelAleatoria maiorVariavelAleatoria = getMaiorIdVariavelAleatoria();

		for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= maiorVariavelAleatoria; idVar++)
			vetorVariavelAleatoria.att(idVar).parametrizarModelo(a_tipo_modelo_geracao_sintetica, a_tipo_coeficiente_auto_correlacao, a_ordem_maxima_coeficiente_auto_correlacao);

		
		setAtributo(AttComumProcessoEstocastico_tipo_correlacao_variaveis_aleatorias, a_tipo_correlacao_variaveis_aleatorias);
		setAtributo(AttComumProcessoEstocastico_correlacao_determinante, a_valor_determinacao_correlacao);



		calcularCorrelacaoSazonalVariaveisAleatorias(a_valor_determinacao_correlacao);

		calcularCorrelacaoSazonalResiduoVariaveisAleatorias(a_valor_determinacao_correlacao);

		calcularMatrizCargaSazonalResiduoVariaveisAleatorias();

		if (a_imprimir_parametros)
			imprimirParametros(a_entradaSaidaDados);

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::parametrizarModelo(a_entradaSaidaDados," + getFullString(a_imprimir_parametros) + "," + getFullString(a_tipo_modelo_geracao_sintetica) + "," + getFullString(a_tipo_coeficiente_auto_correlacao) + "," + getFullString(a_ordem_maxima_coeficiente_auto_correlacao) + "," +  getFullString(a_tipo_correlacao_variaveis_aleatorias) + "): \n" + std::string(erro.what())); }

} // void ProcessoEstocastico::parametrizarModelo(const EntradaSaidaDados &a_entradaSaidaDados, const bool a_imprimir_parametros, const TipoModeloGeracaoSinteticaCenario a_tipo_modelo_geracao_sintetica, const TipoValor a_tipo_coeficiente_auto_correlacao, const int a_ordem_maxima_coeficiente_auto_correlacao, const TipoCorrelacaoVariaveisAleatorias a_tipo_correlacao_variaveis_aleatorias){



void ProcessoEstocastico::avaliarModeloViaSerieSintetica(const EntradaSaidaDados &a_entradaSaidaDados, const SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>>& a_horizonte_espaco_amostral, const TipoSorteio a_tipo_sorteio, const int a_numero_periodos_avaliacao_sintetica){

	try{

		if (a_numero_periodos_avaliacao_sintetica == 0)
			return;

		if (a_horizonte_espaco_amostral.size() == 0)
			return;

		Periodo periodo_inicial_serie = a_horizonte_espaco_amostral.getIteradorInicial();

		for (int i = 0; i < a_horizonte_espaco_amostral.size(); i++) {

			if (periodo_inicial_serie.getTipoPeriodo() == getIteradorFinal(IdVariavelAleatoria_1, IdVariavelAleatoriaInterna_1, AttVetorVariavelAleatoriaInterna_serie_temporal, Periodo()).getTipoPeriodo())
				break;

			a_horizonte_espaco_amostral.incrementarIterador(periodo_inicial_serie);

		}

		SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> horizonte_amostra_comum(periodo_inicial_serie, std::vector<SmartEnupla<IdRealizacao, double>>(a_numero_periodos_avaliacao_sintetica, SmartEnupla<IdRealizacao, double>(IdRealizacao_1, std::vector<double>(1, NAN))));

		int semente = 1;

		gerarEspacoAmostralPorSorteio(a_entradaSaidaDados, true, TipoRelaxacaoVariavelAleatoria_sem_relaxacao, horizonte_amostra_comum, a_tipo_sorteio, semente);

		gerarCenariosPorSorteio(a_entradaSaidaDados, true, true, true, 1, IdCenario_1, IdCenario_1, TipoSorteio_uniforme, semente);

		//SmartEnupla<IdVariavelAleatoria, SmartEnupla<Periodo, double>> serie_sintetica;
		
		//const IdVariavelAleatoria maiorVariavelAleatoria = getMaiorIdVariavelAleatoria();
		//for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= maiorVariavelAleatoria; idVar++)
			//serie_sintetica.addElemento(idVar, vetorVariavelAleatoria.att(idVar).getMatriz(AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral, IdCenario(), Periodo(), double()).at(IdCenario_1));

		ProcessoEstocastico processoEstocastico;

		const IdProcessoEstocastico idProcessoEstocastico = getIdProcessoEstocasticoFromChar(std::string(getString(getIdObjeto()) + "_sintetico").c_str());

		processoEstocastico.setAtributo(AttComumProcessoEstocastico_idProcessoEstocastico, idProcessoEstocastico);

		const TipoModeloGeracaoSinteticaCenario            tipo_modelo_geracao_sintetica = getAtributo(IdVariavelAleatoria_1, AttComumVariavelAleatoria_tipo_modelo_geracao_sintetica,            TipoModeloGeracaoSinteticaCenario());
		const TipoValor     tipo_coeficiente_auto_correlacao         = getAtributo(IdVariavelAleatoria_1, AttComumVariavelAleatoria_tipo_coeficiente_auto_correlacao,         TipoValor());
		const int                               ordem_maxima_coeficiente_auto_correlacao = getAtributo(IdVariavelAleatoria_1, AttComumVariavelAleatoria_ordem_maxima_coeficiente_auto_correlacao, int());

		const TipoCorrelacaoVariaveisAleatorias tipo_correlacao_variaveis_aleatorias     = getAtributo(AttComumProcessoEstocastico_tipo_correlacao_variaveis_aleatorias,                          TipoCorrelacaoVariaveisAleatorias());
		const double correlacao_determinante     = getAtributo(AttComumProcessoEstocastico_correlacao_determinante,                          double());

		//processoEstocastico.addSeriesTemporais(serie_sintetica);

		processoEstocastico.parametrizarModelo(a_entradaSaidaDados, true, tipo_modelo_geracao_sintetica, tipo_coeficiente_auto_correlacao, ordem_maxima_coeficiente_auto_correlacao, tipo_correlacao_variaveis_aleatorias, correlacao_determinante);

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::avaliarModeloViaSerieSintetica(a_entradaSaidaDados," + getString(a_tipo_sorteio) + "," + getString(a_numero_periodos_avaliacao_sintetica) + "): \n" + std::string(erro.what())); }

} // void ProcessoEstocastico::avaliarModeloViaSerieSintetica(const EntradaSaidaDados &a_entradaSaidaDados, const TipoSorteio a_tipo_sorteio, const int a_numero_periodos_avaliacao_sintetica){


void ProcessoEstocastico::gerarEspacoAmostralPorSorteio(const EntradaSaidaDados & a_entradaSaidaDados, const bool imprimir_amostra, const TipoRelaxacaoVariavelAleatoria a_tipo_relaxacao, const SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> &a_horizonte_espaco_amostral, const TipoSorteio a_tipo_sorteio, int &a_semente){

	try{

		if (a_horizonte_espaco_amostral.size() == 0)
			return;


		SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> horizonte_espaco_amostral;

		if (getSize1Matriz(IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_residuo_espaco_amostral) > 0) {

			Periodo periodo_inicial = getIterador1Final(IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_residuo_espaco_amostral, Periodo());

			a_horizonte_espaco_amostral.incrementarIterador(periodo_inicial);

			for (Periodo periodo = periodo_inicial; periodo <= a_horizonte_espaco_amostral.getIteradorFinal(); a_horizonte_espaco_amostral.incrementarIterador(periodo))
				horizonte_espaco_amostral.addElemento(periodo, a_horizonte_espaco_amostral.at(periodo));

		}
		else
			horizonte_espaco_amostral = a_horizonte_espaco_amostral;

		const IdVariavelAleatoria maiorVariavelAleatoria = getMaiorIdVariavelAleatoria();

		for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= maiorVariavelAleatoria; idVar++) {
			if (numero_maximo_periodos_espaco_amostral < horizonte_espaco_amostral.size())
				vetorVariavelAleatoria.att(idVar).gerarRuidoBrancoEspacoAmostral(horizonte_espaco_amostral, a_tipo_sorteio, numero_maximo_periodos_espaco_amostral, a_semente);
			else
				vetorVariavelAleatoria.att(idVar).gerarRuidoBrancoEspacoAmostral(horizonte_espaco_amostral, a_tipo_sorteio, a_semente);
		}

		calcularRuidoCorrelacionadoEspacoAmostral();

		for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= maiorVariavelAleatoria; idVar++) {
			vetorVariavelAleatoria.att(idVar).expandirParametrosEspacoAmostral(horizonte_espaco_amostral);
			vetorVariavelAleatoria.att(idVar).gerarEspacoAmostralFromRuido(horizonte_espaco_amostral);
		}

		for (Periodo periodo = horizonte_espaco_amostral.getIteradorInicial(); periodo <= horizonte_espaco_amostral.getIteradorFinal(); horizonte_espaco_amostral.incrementarIterador(periodo)) {
			for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= maiorVariavelAleatoria; idVar++)
				vetorVariavelAleatoria.att(idVar).addElemento(AttVetorVariavelAleatoria_tipo_relaxacao, periodo, a_tipo_relaxacao);
			const IdRealizacao maiorIdRealizacao = horizonte_espaco_amostral.at(periodo).getIteradorFinal();
			for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao; idRealizacao++)
				addElemento(AttMatrizProcessoEstocastico_probabilidade_realizacao, periodo, idRealizacao, 1.0 / double(maiorIdRealizacao));
			validar_probabilidade_realizacao(periodo);
		} // for (Periodo periodo = a_horizonte_espaco_amostral.getIteradorInicial(); periodo <= a_horizonte_espaco_amostral.getIteradorFinal(); a_horizonte_espaco_amostral.incrementarIterador(periodo)) {

		if (imprimir_amostra)
			imprimirEspacoAmostral(a_entradaSaidaDados);

		// Exclui matrizes basicas de ruido.
		for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= maiorVariavelAleatoria; idVar++) {
			vetorVariavelAleatoria.att(idVar).setMatriz(AttMatrizVariavelAleatoria_ruido_branco_espaco_amostral, SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>>());
			vetorVariavelAleatoria.att(idVar).setMatriz(AttMatrizVariavelAleatoria_ruido_correlacionado_espaco_amostral, SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>>());
		} // for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= maiorVariavelAleatoria; idVar++) {

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::gerarEspacoAmostralComSorteio(a_entradaSaidaDados,a_horizonte_amostra," + getString(a_tipo_sorteio) + "," + getString(a_semente) + "): \n" + std::string(erro.what())); }

} // void ProcessoEstocastico::gerarEspacoAmostralComSorteio(const SmartEnupla<Periodo, IdRealizacao> &a_horizonte_amostra, const TipoSorteio a_tipo_sorteio, int &a_semente){

void ProcessoEstocastico::validar_probabilidade_realizacao(const Periodo a_periodo)const {
	try {

		if (getSizeMatriz(AttMatrizProcessoEstocastico_probabilidade_realizacao) > 0) {

				double probabilidade_acumulada = 0.0;

				for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= getIterador2Final(AttMatrizProcessoEstocastico_probabilidade_realizacao, a_periodo, IdRealizacao()); idRealizacao++)
					probabilidade_acumulada += getElementoMatriz(AttMatrizProcessoEstocastico_probabilidade_realizacao, a_periodo, idRealizacao, double());

				if (probabilidade_acumulada > 1.00001 || probabilidade_acumulada < 0.99995)
					throw std::invalid_argument("Atributo " + getFullString(AttMatrizProcessoEstocastico_probabilidade_realizacao) + " com somatoria de valores diferente de 1.0 em " + getFullString(a_periodo));

		} // if (getSizeMatriz(AttMatrizProcessoEstocastico_probabilidade_realizacao) > 0) {

	} // try{
	catch (const std::exception & erro) { throw std::invalid_argument("ProcessoEstocastico::validar_probabilidade_realizacao(): \n" + std::string(erro.what())); }
} // void ProcessoEstocastico::validar_probabilidade_realizacao() {

void ProcessoEstocastico::validar_probabilidade_realizacao() const{
	try {

		if (getSizeMatriz(AttMatrizProcessoEstocastico_probabilidade_realizacao) > 0) {

			SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> probabilidade_realizacao = getMatriz(AttMatrizProcessoEstocastico_probabilidade_realizacao, Periodo(), IdRealizacao(), double());

			for (Periodo periodo = probabilidade_realizacao.getIteradorInicial(); periodo <= probabilidade_realizacao.getIteradorFinal(); probabilidade_realizacao.incrementarIterador(periodo)) {

				validar_probabilidade_realizacao(periodo);

			} // for (Periodo periodo = probabilidade_realizacao.getIteradorInicial(); periodo <= probabilidade_realizacao.getIteradorFinal(); probabilidade_realizacao.incrementarIterador(periodo)) {

		} // if (getSizeMatriz(AttMatrizProcessoEstocastico_probabilidade_realizacao) > 0) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("ProcessoEstocastico::validar_probabilidade_realizacao(): \n" + std::string(erro.what())); }
} // void ProcessoEstocastico::validar_probabilidade_realizacao() {

void ProcessoEstocastico::gerarCenariosPorSorteio(const EntradaSaidaDados &a_entradaSaidaDados, const bool a_imprimir_cenarios, const bool a_gerar_cenarios_buffer, const bool a_gerar_cenarios_internos, const int a_numero_cenarios_global, const IdCenario a_cenario_inicial, const IdCenario a_cenario_final, const TipoSorteio a_tipo_sorteio, int &a_semente){

	try {

		mapearCenariosEspacoAmostralPorSorteio(a_tipo_sorteio, a_numero_cenarios_global, a_cenario_inicial, a_cenario_final, a_semente);

		SmartEnupla<Periodo, double> horizonte_tendencia;
		
		if (getSizeVetor(IdVariavelAleatoria_1, IdVariavelAleatoriaInterna_1, AttVetorVariavelAleatoriaInterna_tendencia_temporal) > 0)
			horizonte_tendencia = getVetor(IdVariavelAleatoria_1, IdVariavelAleatoriaInterna_1, AttVetorVariavelAleatoriaInterna_tendencia_temporal, Periodo(), double());
		else if (getSizeVetor(IdVariavelAleatoria_1, IdVariavelAleatoriaInterna_1, AttVetorVariavelAleatoriaInterna_tendencia_temporal_transformada) > 0)
			horizonte_tendencia = getVetor(IdVariavelAleatoria_1, IdVariavelAleatoriaInterna_1, AttVetorVariavelAleatoriaInterna_tendencia_temporal_transformada, Periodo(), double());
		else
			horizonte_tendencia = getElementosMatriz(IdVariavelAleatoria_1, IdVariavelAleatoriaInterna_1, AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral, a_cenario_inicial, Periodo(), double());

		const SmartEnupla <IdCenario, SmartEnupla<Periodo, IdRealizacao>> mapeamento_espaco_amostral = getMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario(), Periodo(), IdRealizacao());

		SmartEnupla<Periodo, IdRealizacao> horizonte_amostra;
		Periodo periodo_inicial_amostra;

		if (mapeamento_espaco_amostral.size() > 0) {
			horizonte_amostra = mapeamento_espaco_amostral.getElemento(a_cenario_inicial);
			periodo_inicial_amostra = horizonte_amostra.getIteradorInicial();
		}
		else
			periodo_inicial_amostra = horizonte_tendencia.getIteradorFinal() + 1;

		SmartEnupla<Periodo, double> horizonte_processo_estocastico(horizonte_tendencia.size() + horizonte_amostra.size());
		for (Periodo periodo = horizonte_tendencia.getIteradorInicial(); periodo < periodo_inicial_amostra; horizonte_tendencia.incrementarIterador(periodo)) {

			if (periodo >= periodo_inicial_amostra)
				break;

			horizonte_processo_estocastico.addElemento(periodo, NAN);

		} // for (Periodo periodo = horizonte_tendencia.getIteradorInicial(); periodo < periodo_inicial_amostra; horizonte_tendencia.incrementarIterador(periodo)) {

		if (horizonte_amostra.size() > 0) {
			for (Periodo periodo = horizonte_amostra.getIteradorInicial(); periodo <= horizonte_amostra.getIteradorFinal(); horizonte_amostra.incrementarIterador(periodo))
				horizonte_processo_estocastico.addElemento(periodo, NAN);
		}

		const SmartEnupla<Periodo, SmartEnupla<IdCenario, double>> cenarios_horizonte_processo_estocastico(horizonte_processo_estocastico, SmartEnupla<IdCenario, double>(a_cenario_inicial, std::vector<double>(int(a_cenario_final - a_cenario_inicial) + 1, NAN)));

		for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= getMaiorIdVariavelAleatoria(); idVar++) {
			vetorVariavelAleatoria.att(idVar).gerarCenariosEspacoAmostral(mapeamento_espaco_amostral, cenarios_horizonte_processo_estocastico, a_gerar_cenarios_buffer, a_gerar_cenarios_internos);
		}

		if (a_imprimir_cenarios)
			imprimirCenarios(a_entradaSaidaDados);

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::gerarCenariosPorSorteio(a_entradaSaidaDados," + getString(a_imprimir_cenarios) + ",a_mapeamento_cenarios_tendencia," + getString(a_tipo_sorteio) + "," + getString(a_semente) + "): \n" + std::string(erro.what())); }

} // void ProcessoEstocastico::gerarCenariosPorSorteio(const SmartEnupla<IdCenario, IdCenario> a_mapeamento_cenarios_tendencia, const int a_semente){

SmartEnupla<IdVariavelAleatoria, SmartEnupla<Periodo, SmartEnupla<IdCenario, double>>> ProcessoEstocastico::gerarCenariosPorSorteioRetorno(const EntradaSaidaDados& a_entradaSaidaDados, const bool a_imprimir_cenarios, const bool a_gerar_cenarios_buffer, const bool a_gerar_cenarios_internos, const int a_numero_cenarios_global, const IdCenario a_cenario_inicial, const IdCenario a_cenario_final, const TipoSorteio a_tipo_sorteio, int& a_semente){
	try {

		gerarCenariosPorSorteio(a_entradaSaidaDados, a_imprimir_cenarios, a_gerar_cenarios_buffer, a_gerar_cenarios_internos, a_numero_cenarios_global, a_cenario_inicial, a_cenario_final, a_tipo_sorteio, a_semente);

		const IdVariavelAleatoria maiorIdVariavelAleatoria = getMaiorIdVariavelAleatoria();

		SmartEnupla<IdVariavelAleatoria, SmartEnupla<Periodo, SmartEnupla<IdCenario, double>>> enupla_retorno(IdVariavelAleatoria_1, std::vector<SmartEnupla<Periodo, SmartEnupla<IdCenario, double>>>(maiorIdVariavelAleatoria, SmartEnupla<Periodo, SmartEnupla<IdCenario, double>>()));

		for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= maiorIdVariavelAleatoria; idVar++) {
			enupla_retorno.at(idVar) = getMatriz(idVar, AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral, Periodo(), IdCenario(), double());
			vetorVariavelAleatoria.att(idVar).setMatriz_forced(AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral, SmartEnupla<Periodo, SmartEnupla<IdCenario, double>>());
		}

		return enupla_retorno;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::gerarCenariosPorSorteioRetorno(a_entradaSaidaDados," + getString(a_imprimir_cenarios) + ",a_mapeamento_cenarios_tendencia," + getString(a_tipo_sorteio) + "," + getString(a_semente) + "): \n" + std::string(erro.what())); }
}


SmartEnupla<IdCenario, SmartEnupla<Periodo, IdRealizacao>> ProcessoEstocastico::getCenariosMapeamento(const AttMatrizProcessoEstocastico a_attMatrizProcessoEstocastico, const IdCenario a_cenarioInicial, const IdCenario a_cenarioFinal){

	try {

		if (a_attMatrizProcessoEstocastico != AttMatrizProcessoEstocastico_mapeamento_espaco_amostral)
			throw std::invalid_argument("Argumento invalido.");
			//(a_attMatrizProcessoEstocastico != AttMatrizProcessoEstocastico_mapeamento_arvore_cenarios))

		SmartEnupla<IdCenario, SmartEnupla<Periodo, IdRealizacao>> cenarios(a_cenarioInicial, std::vector<SmartEnupla<Periodo, IdRealizacao>>(int(a_cenarioFinal - a_cenarioInicial) + 1, SmartEnupla<Periodo, IdRealizacao>()));

		for (IdCenario idCenario = a_cenarioInicial; idCenario <= a_cenarioFinal; idCenario++)
			cenarios.setElemento(idCenario, getElementosMatriz(a_attMatrizProcessoEstocastico, idCenario, Periodo(), IdRealizacao()));

		return cenarios;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::getCenariosMapeamento(" + getFullString(a_attMatrizProcessoEstocastico) + "," + getFullString(a_cenarioInicial) + "," + getFullString(a_cenarioFinal) + "): \n" + std::string(erro.what())); }

}

SmartEnupla<IdVariavelAleatoria, SmartEnupla<Periodo, SmartEnupla<IdCenario, double>>> ProcessoEstocastico::getCenarios(const AttMatrizVariavelAleatoria a_attMatrizVariavelAleatoria){

	try {

		if (a_attMatrizVariavelAleatoria != AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral)
			throw std::invalid_argument("Argumento invalido.");
			
		const IdVariavelAleatoria maiorVariavelAleatoria = getMaiorIdVariavelAleatoria();


		SmartEnupla<IdVariavelAleatoria, SmartEnupla<Periodo, SmartEnupla<IdCenario, double>>> cenarios(IdVariavelAleatoria_1, std::vector<SmartEnupla<Periodo, SmartEnupla<IdCenario, double>>>(int(maiorVariavelAleatoria), SmartEnupla<Periodo, SmartEnupla<IdCenario, double>>()));

		for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= maiorVariavelAleatoria; idVar++)
			cenarios.at(idVar) = vetorVariavelAleatoria.att(idVar).getMatriz(a_attMatrizVariavelAleatoria, Periodo(), IdCenario(), double());

		return cenarios;

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::getCenarios(" + getFullString(a_attMatrizVariavelAleatoria) + "): \n" + std::string(erro.what())); }

} // SmartEnupla<IdVariavelAleatoria, SmartEnupla<Periodo, double>> ProcessoEstocastico::getCenarios(const AttMatrizVariavelAleatoria a_attMatrizVariavelAleatoria){


SmartEnupla<IdVariavelAleatoriaInterna, SmartEnupla<Periodo, SmartEnupla<IdCenario, double>>> ProcessoEstocastico::getCenariosInternos(const IdVariavelAleatoria a_idVariavelAleatoria, const AttMatrizVariavelAleatoriaInterna a_attMatrizVariavelAleatoriaInterna){

	try {

		if (a_attMatrizVariavelAleatoriaInterna != AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral)
			throw std::invalid_argument("Argumento invalido.");

		SmartEnupla<IdVariavelAleatoriaInterna, SmartEnupla<Periodo, SmartEnupla<IdCenario, double>>> cenarios;

		for (IdVariavelAleatoriaInterna idVarInterna = IdVariavelAleatoriaInterna_1; idVarInterna <= getMaiorId(a_idVariavelAleatoria, IdVariavelAleatoriaInterna()); idVarInterna++)
			cenarios.addElemento(idVarInterna, vetorVariavelAleatoria.att(a_idVariavelAleatoria).getMatriz(idVarInterna, a_attMatrizVariavelAleatoriaInterna, Periodo(), IdCenario(), double()));

		return cenarios;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::getCenariosInternos(" + getFullString(a_idVariavelAleatoria) + "," + getFullString(a_attMatrizVariavelAleatoriaInterna) + "): \n" + std::string(erro.what())); }

} // SmartEnupla<IdVariavelAleatoriaInterna, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>> ProcessoEstocastico::getCenariosInternos(const IdVariavelAleatoria a_idVariavelAleatoria, const AttMatrizVariavelAleatoriaInterna a_attMatrizVariavelAleatoriaInterna){


SmartEnupla<IdVariavelAleatoriaInterna, SmartEnupla<Periodo, SmartEnupla<IdCenario, double>>> ProcessoEstocastico::getCenariosInternos(const IdVariavelAleatoria a_idVariavelAleatoria, const AttMatrizVariavelAleatoriaInterna a_attMatrizVariavelAleatoriaInterna, const IdCenario a_cenarioInicial, const IdCenario a_cenarioFinal){
	
	try {

		if (a_attMatrizVariavelAleatoriaInterna != AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral)
			throw std::invalid_argument("Argumento invalido.");

		const IdVariavelAleatoriaInterna maiorIdVariavelAleatoriaInterna = getMaiorId(a_idVariavelAleatoria, IdVariavelAleatoriaInterna());

		SmartEnupla<IdVariavelAleatoriaInterna, SmartEnupla<Periodo, SmartEnupla<IdCenario, double>>> cenarios(IdVariavelAleatoriaInterna_1, std::vector<SmartEnupla<Periodo, SmartEnupla<IdCenario, double>>>(maiorIdVariavelAleatoriaInterna, SmartEnupla<Periodo, SmartEnupla<IdCenario, double>>(vetorVariavelAleatoria.att(a_idVariavelAleatoria).getMatriz(IdVariavelAleatoriaInterna_1, a_attMatrizVariavelAleatoriaInterna, Periodo(), IdCenario(), double()), SmartEnupla<IdCenario, double>())));

		const SmartEnupla<IdCenario, double> inicializacao(a_cenarioInicial, std::vector<double>(int(a_cenarioFinal - a_cenarioInicial) + 1, NAN));
		for (IdVariavelAleatoriaInterna idVarInterna = IdVariavelAleatoriaInterna_1; idVarInterna <= maiorIdVariavelAleatoriaInterna; idVarInterna++) {
			for (Periodo periodo = cenarios.at(idVarInterna).getIteradorInicial(); periodo <= cenarios.at(idVarInterna).getIteradorFinal(); cenarios.at(idVarInterna).incrementarIterador(periodo)) {
				cenarios.at(idVarInterna).at(periodo) = inicializacao;
				for (IdCenario idCenario = a_cenarioInicial; idCenario <= a_cenarioFinal; idCenario++)
					cenarios.at(idVarInterna).at(periodo).at(idCenario) = vetorVariavelAleatoria.att(a_idVariavelAleatoria).getElementoMatriz(IdVariavelAleatoriaInterna_1, a_attMatrizVariavelAleatoriaInterna, periodo, idCenario, double());
			}
		}

		return cenarios;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::getCenariosInternos(" + getFullString(a_idVariavelAleatoria) + "," + getFullString(a_attMatrizVariavelAleatoriaInterna) + "," + getFullString(a_cenarioInicial) + "," + getFullString(a_cenarioFinal) + "): \n" + std::string(erro.what())); }

}


double ProcessoEstocastico::calcularRealizacaoInterna(const IdVariavelAleatoria a_idVariavelAleatoria, const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const Periodo a_periodo, const double a_realizacao){
	try {

		return vetorVariavelAleatoria.att(a_idVariavelAleatoria).calcularRealizacaoInterna(a_idVariavelAleatoriaInterna, a_periodo, a_realizacao);

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::calcularRealizacaoInterna(" + getFullString(a_idVariavelAleatoria) + "," + getFullString(a_idVariavelAleatoriaInterna) + "," + getFullString(a_periodo) + "," + getFullString(a_realizacao) + "): \n" + std::string(erro.what())); }
}

double ProcessoEstocastico::calcularRealizacaoInterna(const IdVariavelAleatoria a_idVariavelAleatoria, const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const IdCenario a_idCenario, const Periodo a_periodo) {

	try {

		/**/
		if (vetorVariavelAleatoria.att(a_idVariavelAleatoria).getSizeMatriz(AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral) > 0)
			return vetorVariavelAleatoria.att(a_idVariavelAleatoria).calcularRealizacaoInterna(a_idVariavelAleatoriaInterna, a_idCenario, a_periodo);
		else {

			const SmartEnupla<Periodo, IdRealizacao> idRealizacoes_cenario = getElementosMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, Periodo(), IdRealizacao());
			const SmartEnupla<Periodo, IdRealizacao> horizonte_mapeamento_espaco_amostral = getElementosMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, Periodo(), IdRealizacao());
			return vetorVariavelAleatoria.att(a_idVariavelAleatoria).calcularRealizacaoInterna(a_idVariavelAleatoriaInterna, a_idCenario, idRealizacoes_cenario.at(a_periodo), a_periodo, idRealizacoes_cenario, horizonte_mapeamento_espaco_amostral);
		}//else{

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::calcularRealizacaoInterna(" + getFullString(a_idVariavelAleatoria) + "," + getFullString(a_idVariavelAleatoriaInterna) + "," + getFullString(a_idCenario) + "," + getFullString(a_periodo) + "): \n" + std::string(erro.what())); }

} // double ProcessoEstocastico::calcularRealizacao(const SmartEnupla<Periodo, double>& a_tendencia, const double a_residuo) const{


double ProcessoEstocastico::calcularRealizacaoInterna(const IdVariavelAleatoria a_idVariavelAleatoria, const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const Periodo a_periodo){

	try{ 
		
		/**/
		if (vetorVariavelAleatoria.att(a_idVariavelAleatoria).getSizeMatriz(AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral) > 0)
			return vetorVariavelAleatoria.att(a_idVariavelAleatoria).calcularRealizacaoInterna(a_idVariavelAleatoriaInterna, a_idCenario, a_idRealizacao, a_periodo);
		else{
			const SmartEnupla<Periodo, IdRealizacao> idRealizacoes_cenario = getElementosMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, Periodo(), IdRealizacao());
			const SmartEnupla<Periodo, IdRealizacao> horizonte_mapeamento_espaco_amostral = getElementosMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, Periodo(), IdRealizacao());
			return vetorVariavelAleatoria.att(a_idVariavelAleatoria).calcularRealizacaoInterna(a_idVariavelAleatoriaInterna, a_idCenario, a_idRealizacao, a_periodo, idRealizacoes_cenario, horizonte_mapeamento_espaco_amostral);
		}//else{

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::calcularRealizacaoInterna(" + getFullString(a_idVariavelAleatoria) + "," + getFullString(a_idVariavelAleatoriaInterna) + "," + getFullString(a_idCenario)  + "," + getFullString(a_periodo) + "): \n" + std::string(erro.what())); }

} // double ProcessoEstocastico::calcularRealizacao(const SmartEnupla<Periodo, double>& a_tendencia, const double a_residuo) const{


double ProcessoEstocastico::calcularRealizacao(const IdVariavelAleatoria a_idVariavelAleatoria, const IdCenario a_idCenario, const Periodo a_periodo){

	try {

		/**/
		if (vetorVariavelAleatoria.att(a_idVariavelAleatoria).getSizeMatriz(AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral) > 0)
			return vetorVariavelAleatoria.att(a_idVariavelAleatoria).getRealizacaoEspacoAmostral(a_idCenario, a_periodo);
		else {

			const SmartEnupla<Periodo, IdRealizacao> idRealizacoes_cenario = getElementosMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, Periodo(), IdRealizacao());
			const SmartEnupla<Periodo, IdRealizacao> horizonte_mapeamento_espaco_amostral = getElementosMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, Periodo(), IdRealizacao());
			return vetorVariavelAleatoria.att(a_idVariavelAleatoria).getRealizacaoEspacoAmostral(a_idCenario, idRealizacoes_cenario.at(a_periodo), idRealizacoes_cenario, a_periodo, horizonte_mapeamento_espaco_amostral);
		}//else{

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::calcularRealizacao(" + getFullString(a_idVariavelAleatoria) + "," + getFullString(a_idCenario) + "," + getFullString(a_periodo) + "): \n" + std::string(erro.what())); }
}


double ProcessoEstocastico::calcularRealizacao(const IdVariavelAleatoria a_idVariavelAleatoria, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const Periodo a_periodo){

	try {

		/**/
		if (vetorVariavelAleatoria.att(a_idVariavelAleatoria).getSizeMatriz(AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral) > 0)
			return vetorVariavelAleatoria.att(a_idVariavelAleatoria).calcularRealizacao(a_periodo, a_idCenario, SmartEnupla<Periodo, double>(), getElementoMatriz(a_idVariavelAleatoria, AttMatrizVariavelAleatoria_residuo_espaco_amostral, a_periodo, a_idRealizacao, double()));
		else {
			const SmartEnupla<Periodo, IdRealizacao> idRealizacoes_cenario = getElementosMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, Periodo(), IdRealizacao());
			const SmartEnupla<Periodo, IdRealizacao> horizonte_mapeamento_espaco_amostral = getElementosMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, Periodo(), IdRealizacao());
			return vetorVariavelAleatoria.att(a_idVariavelAleatoria).getRealizacaoEspacoAmostral(a_idCenario, a_idRealizacao, idRealizacoes_cenario, a_periodo, horizonte_mapeamento_espaco_amostral);
		}//else{

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::calcularRealizacao(" + getFullString(a_idVariavelAleatoria) + "," + getFullString(a_idCenario) + "," + getFullString(a_periodo) + "): \n" + std::string(erro.what())); }

}


double ProcessoEstocastico::calcularRealizacaoParaValor(const IdVariavelAleatoria a_idVariavelAleatoria, const double a_valor, const Periodo a_periodo){
	try{

		return vetorVariavelAleatoria.att(a_idVariavelAleatoria).calcularRealizacaoParaValor(a_valor, a_periodo);

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::calcularRealizacaoParaValor(" + getFullString(a_idVariavelAleatoria) + "," + getFullString(a_valor) + "," + getFullString(a_periodo) + "): \n" + std::string(erro.what())); }
} // double ProcessoEstocastico::calcularRealizacaoParaValor(const IdVariavelAleatoria a_idVariavelAleatoria, const double a_valor, const Periodo a_periodo){




void ProcessoEstocastico::imprimirEspacoAmostral(EntradaSaidaDados a_entradaSaidaDados){

	try {

		const std::string diretorio_base = a_entradaSaidaDados.getDiretorioSaida();

		for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= getMaiorIdVariavelAleatoria(); idVar++) {

			const std::string diretorio_va = diretorio_base + "//" + getAtributo(idVar, AttComumVariavelAleatoria_nome, std::string());

			a_entradaSaidaDados.setDiretorioSaida(diretorio_va);

			a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("_info_" + getString(AttMatrizVariavelAleatoria_residuo_espaco_amostral) + ".csv", idVar, *this, AttMatrizVariavelAleatoria_residuo_espaco_amostral);
			a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("_info_" + getString(AttMatrizVariavelAleatoria_ruido_branco_espaco_amostral) + ".csv", idVar, *this, AttMatrizVariavelAleatoria_ruido_branco_espaco_amostral);
			a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("_info_" + getString(AttMatrizVariavelAleatoria_ruido_correlacionado_espaco_amostral) + ".csv", idVar, *this, AttMatrizVariavelAleatoria_ruido_correlacionado_espaco_amostral);

		} // for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= getMaiorIdVariavelAleatoria(); idVar++) {

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::imprimirArvoreEspacoAmostral(a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ProcessoEstocastico::imprimirEspacoAmostral(const EntradaSaidaDados & a_entradaSaidaDados){


void ProcessoEstocastico::imprimirCenarios(EntradaSaidaDados a_entradaSaidaDados){

	try {

		const std::string diretorio_base = a_entradaSaidaDados.getDiretorioSaida();

		IdCenario menor_cenario = IdCenario_Nenhum;
		IdCenario maior_cenario = IdCenario_Nenhum;


		for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= getMaiorIdVariavelAleatoria(); idVar++) {

			const std::string diretorio_va = diretorio_base + "//" + getAtributo(idVar, AttComumVariavelAleatoria_nome, std::string());

			a_entradaSaidaDados.setDiretorioSaida(diretorio_va);

			if (getSize1Matriz(idVar, AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral) > 0) {
				const Periodo menor_periodo = getIterador1Inicial(idVar, AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral, Periodo());
				menor_cenario = getIterador2Inicial(idVar, AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral, menor_periodo, IdCenario());
				maior_cenario = getIterador2Final(idVar, AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral, menor_periodo, IdCenario());
			}
			else {
				menor_cenario = IdCenario_Nenhum;
				maior_cenario = IdCenario_Nenhum;
			}

			a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("_info_" + getString(AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral) + "_" + getString(menor_cenario) + "_" + getString(maior_cenario) + ".csv", idVar, *this, AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral);

			for (IdVariavelAleatoriaInterna idVarInterna = IdVariavelAleatoriaInterna_1; idVarInterna <= getMaiorId(idVar, IdVariavelAleatoriaInterna()); idVarInterna++){

				const std::string diretorio_var_interna = diretorio_va + "//" + getAtributo(idVar, idVarInterna, AttComumVariavelAleatoriaInterna_nome, std::string());

				a_entradaSaidaDados.setDiretorioSaida(diretorio_var_interna);

				if (getSize1Matriz(idVar, idVarInterna, AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral) > 0) {
					const Periodo menor_periodo = getIterador1Inicial(idVar, idVarInterna, AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral, Periodo());
					menor_cenario = getIterador2Inicial(idVar, idVarInterna, AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral, menor_periodo, IdCenario());
					maior_cenario = getIterador2Final(idVar, idVarInterna, AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral, menor_periodo, IdCenario());
				}
				else {
					menor_cenario = IdCenario_Nenhum;
					maior_cenario = IdCenario_Nenhum;
				}

				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("_info_" + getString(AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral) + "_" + getString(menor_cenario) + "_" + getString(maior_cenario) + ".csv", idVar, idVarInterna, *this, AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral);

			} // for (IdVariavelAleatoriaInterna idVarInterna = IdVariavelAleatoriaInterna_1; idVarInterna <= getMaiorId(idVar, IdVariavelAleatoriaInterna()); idVarInterna++){

		} // for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= maiorVariavelAleatoria; idVar++) {

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::imprimirCenarios(a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ProcessoEstocastico::imprimirCenarios(const EntradaSaidaDados & a_entradaSaidaDados){


void ProcessoEstocastico::imprimirParametros(EntradaSaidaDados a_entradaSaidaDados){

	try {

		const std::string diretorio_base = a_entradaSaidaDados.getDiretorioSaida();

		a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_MLT.csv", IdVariavelAleatoria_Nenhum, IdVariavelAleatoriaInterna_Nenhum, *this, AttVetorVariavelAleatoriaInterna_media_serie_temporal);

		for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= getMaiorIdVariavelAleatoria(); idVar++) {

			const std::string diretorio_va = diretorio_base + "//" + getAtributo(idVar, AttComumVariavelAleatoria_nome, std::string());

			a_entradaSaidaDados.setDiretorioSaida(diretorio_va);

			a_entradaSaidaDados.imprimirArquivoCSV_AttComum("_info_AttComumOperacional.csv", idVar, *this);

			for (AttVetorVariavelAleatoria attVetor = AttVetorVariavelAleatoria(1); attVetor < AttVetorVariavelAleatoria_Excedente; attVetor++) {
				if ((attVetor != AttVetorVariavelAleatoria_ruido_branco_serie_sintetica) && (attVetor != AttVetorVariavelAleatoria_ruido_correlacionado_serie_sintetica))
				a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_" + getString(attVetor) + ".csv", idVar, *this, attVetor);
			}

			for (AttMatrizVariavelAleatoria attMatriz = AttMatrizVariavelAleatoria(1); attMatriz < AttMatrizVariavelAleatoria_Excedente; attMatriz++) {
				if ((attMatriz != AttMatrizVariavelAleatoria_residuo_espaco_amostral) && (attMatriz != AttMatrizVariavelAleatoria_ruido_branco_espaco_amostral) && (attMatriz != AttMatrizVariavelAleatoria_ruido_correlacionado_espaco_amostral) && (attMatriz != AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral))
					a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("_info_" + getString(attMatriz) + ".csv", idVar, *this, attMatriz);
			}

			for (IdVariavelAleatoriaInterna idVarInterna = IdVariavelAleatoriaInterna_1; idVarInterna <= getMaiorId(idVar, IdVariavelAleatoriaInterna()); idVarInterna++) {

				const std::string diretorio_var_interna = diretorio_va + "//" + getAtributo(idVar, idVarInterna, AttComumVariavelAleatoriaInterna_nome, std::string());

				a_entradaSaidaDados.setDiretorioSaida(diretorio_var_interna);

				a_entradaSaidaDados.imprimirArquivoCSV_AttComum("_info_AttComumOperacional.csv", idVar, idVarInterna, *this);

				for (AttVetorVariavelAleatoriaInterna attVetor = AttVetorVariavelAleatoriaInterna(1); attVetor < AttVetorVariavelAleatoriaInterna_Excedente; attVetor++)
					a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_" + getString(attVetor) + ".csv", idVar, idVarInterna, *this, attVetor);

				for (AttMatrizVariavelAleatoriaInterna attMatriz = AttMatrizVariavelAleatoriaInterna(1); attMatriz < AttMatrizVariavelAleatoriaInterna_Excedente; attMatriz++) {
					if (attMatriz != AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral)
						a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("_info_" + getString(attMatriz) + ".csv", idVar, idVarInterna, *this, attMatriz);
				}

			} // for (IdVariavelAleatoriaInterna idVarInterna = IdVariavelAleatoriaInterna_1; idVarInterna <= getMaiorId(idVar, IdVariavelAleatoriaInterna()); idVarInterna++){

		} // for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= maiorVariavelAleatoria; idVar++) {

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::imprimirParametros(a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ProcessoEstocastico::imprimirParametros(const EntradaSaidaDados &a_entradaSaidaDados){

int ProcessoEstocastico::getMaiorOrdemAutocorrelacaoLinear(const Periodo a_periodo){

	try {

		int maior_ordem = 0;

		for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= getMaiorIdVariavelAleatoria(); idVar++) {

			if (getSize1Matriz(idVar, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao) > 0) {

				int ordem = getIterador2Final(idVar, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, a_periodo, int());

				if ((ordem == 1) && (getElementoMatriz(idVar, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, a_periodo, ordem, double()) == 0.0))
					ordem = 0;

				if (ordem > maior_ordem)
					maior_ordem = ordem;

			} // if (getSize1Matriz(idVar, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao_sazonal) > 0) {

		} // for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= maiorVariavelAleatoria; idVar++) {

		return maior_ordem;

	} // try{
	catch (const std::exception & erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::imprimirParametros(a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // int ProcessoEstocastico::getMaiorOrdemAutocorrelacaoLinear(){

double ProcessoEstocastico::getGrauLiberdade(const IdVariavelAleatoria a_idVariavelAleatoria){
	try {
		return vetorVariavelAleatoria.att(a_idVariavelAleatoria).getGrauLiberdade();
	}
	catch (const std::exception& erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::getGrauLiberdade(" + getFullString(a_idVariavelAleatoria) + "): \n" + std::string(erro.what())); }
}


void ProcessoEstocastico::calcularCorrelacaoSazonalVariaveisAleatorias(const double a_valor_determinacao_correlacao) {

	try {

		const IdVariavelAleatoria maiorVariavelAleatoria = getMaiorIdVariavelAleatoria();

		for (IdVariavelAleatoria idVar_A = IdVariavelAleatoria_1; idVar_A <= maiorVariavelAleatoria; idVar_A++) {

			const IdVariavelAleatoria idVar_A_determinacao = getAtributo(idVar_A, AttComumVariavelAleatoria_idVariavelAleatoria_determinacao, IdVariavelAleatoria());

			if (idVar_A_determinacao != IdVariavelAleatoria_Nenhum) {

				SmartEnupla<Periodo, double> serie_temporal_idVar_A;

				SmartEnupla<IdVariavelAleatoria, SmartEnupla<IdEstacao, double>> correlacao_idVar_A(IdVariavelAleatoria_1, std::vector<SmartEnupla<IdEstacao, double>>(int(maiorVariavelAleatoria), SmartEnupla<IdEstacao, double>()));

				for (IdVariavelAleatoria idVar_B = IdVariavelAleatoria_1; idVar_B <= maiorVariavelAleatoria; idVar_B++) {

					const IdVariavelAleatoria idVar_B_determinacao = getAtributo(idVar_B, AttComumVariavelAleatoria_idVariavelAleatoria_determinacao, IdVariavelAleatoria());

					if (idVar_B_determinacao != IdVariavelAleatoria_Nenhum) {

						if (getSize1Matriz(idVar_B, AttMatrizVariavelAleatoria_correlacao) > 0) {
							const IdEstacao maiorEstacao = getIterador2Final(idVar_B, AttMatrizVariavelAleatoria_correlacao, idVar_A, IdEstacao());
							correlacao_idVar_A.at(idVar_B) = SmartEnupla<IdEstacao, double>(IdEstacao_1, std::vector<double>(int(maiorEstacao), 0.0));
							for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++)
								correlacao_idVar_A.at(idVar_B).setElemento(idEstacao, getElementoMatriz(idVar_B, AttMatrizVariavelAleatoria_correlacao, idVar_A, idEstacao, double()));
						}
						else {

							if (serie_temporal_idVar_A.size() == 0)
								serie_temporal_idVar_A = vetorVariavelAleatoria.att(idVar_A).getVetor(AttVetorVariavelAleatoria_serie_temporal_transformada, Periodo(), double());

							const SmartEnupla<Periodo, double> serie_temporal_idVar_B = vetorVariavelAleatoria.att(idVar_B).getVetor(AttVetorVariavelAleatoria_serie_temporal_transformada, Periodo(), double());
							correlacao_idVar_A.setElemento(idVar_B, getCorrelacaoSazonal(serie_temporal_idVar_A, serie_temporal_idVar_B));

							if (idVar_A_determinacao == idVar_A) {
								bool determinarAemB = true;
								for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= correlacao_idVar_A.at(idVar_B).getIteradorFinal(); idEstacao++) {
									if (correlacao_idVar_A.at(idVar_B).at(idEstacao) < a_valor_determinacao_correlacao) {
										determinarAemB = false;
										break;
									}
									if (idVar_B_determinacao != idVar_B) {
										if (getElementoMatriz(idVar_B_determinacao, AttMatrizVariavelAleatoria_correlacao, idVar_B, idEstacao, double()) > correlacao_idVar_A.at(idVar_B).at(idEstacao)) {
											determinarAemB = false;
											break;
										}
									}
								}

								if (determinarAemB) {
									for (IdVariavelAleatoria idVar_C = IdVariavelAleatoria_1; idVar_C <= maiorVariavelAleatoria; idVar_C++) {
										if (getAtributo(idVar_C, AttComumVariavelAleatoria_idVariavelAleatoria_determinacao, IdVariavelAleatoria()) == idVar_B)
											vetorVariavelAleatoria.att(idVar_C).setAtributo(AttComumVariavelAleatoria_idVariavelAleatoria_determinacao, idVar_A);
									}
								}
							}

						}
					} // if (getAtributo(idVar_B, AttComumVariavelAleatoria_idVariavelAleatoria_determinacao, IdVariavelAleatoria()) == idVar_B) {
				}

				vetorVariavelAleatoria.att(idVar_A).setMatriz_forced(AttMatrizVariavelAleatoria_correlacao, correlacao_idVar_A);

			} // if (getAtributo(idVar_A, AttComumVariavelAleatoria_idVariavelAleatoria_determinacao, IdVariavelAleatoria()) == idVar_A) {

		} // for (IdVariavelAleatoria idVar_A = IdVariavelAleatoria_1; idVar_A <= maiorVariavelAleatoria; idVar_A++) {

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::calcularCorrelacaoSazonalVariaveisAleatorias(): \n" + std::string(erro.what())); }

} // void ProcessoEstocastico::calcularCorrelacaoSazonalVariaveisAleatorias(){



void ProcessoEstocastico::calcularCorrelacaoSazonalResiduoVariaveisAleatorias(const double a_valor_determinacao_correlacao){

	try {

		const IdVariavelAleatoria maiorVariavelAleatoria = getMaiorIdVariavelAleatoria();

		for (IdVariavelAleatoria idVar_A = IdVariavelAleatoria_1; idVar_A <= maiorVariavelAleatoria; idVar_A++) {

			const IdVariavelAleatoria idVar_A_determinacao = getAtributo(idVar_A, AttComumVariavelAleatoria_idVariavelAleatoria_determinacao, IdVariavelAleatoria());

			if (idVar_A_determinacao != IdVariavelAleatoria_Nenhum) {

				SmartEnupla<Periodo, double> serie_temporal_idVar_A;

				SmartEnupla<IdVariavelAleatoria, SmartEnupla<IdEstacao, double>> correlacao_idVar_A(IdVariavelAleatoria_1, std::vector<SmartEnupla<IdEstacao, double>>(int(maiorVariavelAleatoria), SmartEnupla<IdEstacao, double>()));

				for (IdVariavelAleatoria idVar_B = IdVariavelAleatoria_1; idVar_B <= maiorVariavelAleatoria; idVar_B++) {

					const IdVariavelAleatoria idVar_B_determinacao = getAtributo(idVar_B, AttComumVariavelAleatoria_idVariavelAleatoria_determinacao, IdVariavelAleatoria());

					if (idVar_B_determinacao != IdVariavelAleatoria_Nenhum) {

						if (getSize1Matriz(idVar_B, AttMatrizVariavelAleatoria_correlacao_residuo_lognormal) > 0) {
							const IdEstacao maiorEstacao = getIterador2Final(idVar_B, AttMatrizVariavelAleatoria_correlacao_residuo_lognormal, idVar_A, IdEstacao());
							correlacao_idVar_A.at(idVar_B) = SmartEnupla<IdEstacao, double>(IdEstacao_1, std::vector<double>(int(maiorEstacao), 0.0));
							for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++)
								correlacao_idVar_A.at(idVar_B).setElemento(idEstacao, getElementoMatriz(idVar_B, AttMatrizVariavelAleatoria_correlacao_residuo_lognormal, idVar_A, idEstacao, double()));
						}
						else {
							if (serie_temporal_idVar_A.size() == 0)
								serie_temporal_idVar_A = vetorVariavelAleatoria.att(idVar_A).getVetor(AttVetorVariavelAleatoria_serie_residuo_lognormal, Periodo(), double());

							const SmartEnupla<Periodo, double> serie_temporal_idVar_B = vetorVariavelAleatoria.att(idVar_B).getVetor(AttVetorVariavelAleatoria_serie_residuo_lognormal, Periodo(), double());
							correlacao_idVar_A.setElemento(idVar_B, getCorrelacaoSazonal(serie_temporal_idVar_A, serie_temporal_idVar_B));

							if (idVar_A_determinacao == idVar_A) {
								bool determinarAemB = true;
								for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= correlacao_idVar_A.at(idVar_B).getIteradorFinal(); idEstacao++) {
									if (correlacao_idVar_A.at(idVar_B).at(idEstacao) < a_valor_determinacao_correlacao) {
										determinarAemB = false;
										break;
									}
									if (idVar_B_determinacao != idVar_B) {
										if (getSize1Matriz(idVar_B_determinacao, AttMatrizVariavelAleatoria_correlacao_residuo_lognormal) > 0) {
											if (getElementoMatriz(idVar_B_determinacao, AttMatrizVariavelAleatoria_correlacao_residuo_lognormal, idVar_B, idEstacao, double()) > correlacao_idVar_A.at(idVar_B).at(idEstacao)) {
												determinarAemB = false;
												break;
											}
										}
									}
								}

								if (determinarAemB) {
									for (IdVariavelAleatoria idVar_C = IdVariavelAleatoria_1; idVar_C <= maiorVariavelAleatoria; idVar_C++) {
										if (getAtributo(idVar_C, AttComumVariavelAleatoria_idVariavelAleatoria_determinacao, IdVariavelAleatoria()) == idVar_B)
											vetorVariavelAleatoria.att(idVar_C).setAtributo(AttComumVariavelAleatoria_idVariavelAleatoria_determinacao, idVar_A);
									}
								}
							}

						} // if (idVar_B_determinacao != IdVariavelAleatoria_Nenhum) {
					} // if (getAtributo(idVar_B, AttComumVariavelAleatoria_idVariavelAleatoria_determinacao, IdVariavelAleatoria()) == idVar_B) {
				}

				vetorVariavelAleatoria.att(idVar_A).setMatriz_forced(AttMatrizVariavelAleatoria_correlacao_residuo_lognormal, correlacao_idVar_A);

			} // if (idVar_A_determinacao != IdVariavelAleatoria_Nenhum) {

		} // for (IdVariavelAleatoria idVar_A = IdVariavelAleatoria_1; idVar_A <= maiorVariavelAleatoria; idVar_A++) {

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::calcularCorrelacaoSazonalResiduoVariaveisAleatorias(): \n" + std::string(erro.what())); }

} // void ProcessoEstocastico::calcularCorrelacaoSazonalResiduoVariaveisAleatorias(){


void ProcessoEstocastico::calcularMatrizCargaSazonalResiduoVariaveisAleatorias() {

	try {

		const TipoCorrelacaoVariaveisAleatorias tipo_correlacao = getAtributo(AttComumProcessoEstocastico_tipo_correlacao_variaveis_aleatorias, TipoCorrelacaoVariaveisAleatorias());

		const IdVariavelAleatoria maiorVariavelAleatoria = getMaiorIdVariavelAleatoria();

		for (IdVariavelAleatoria idVar_A = IdVariavelAleatoria_1; idVar_A <= maiorVariavelAleatoria; idVar_A++) {

			if (getAtributo(idVar_A, AttComumVariavelAleatoria_idVariavelAleatoria_determinacao, IdVariavelAleatoria()) == idVar_A) {

				const IdEstacao maiorEstacao = getIterador2Final(idVar_A, AttMatrizVariavelAleatoria_correlacao_residuo_lognormal, idVar_A, IdEstacao());

				SmartEnupla<IdEstacao, SmartEnupla<IdVariavelAleatoria, double>> matriz_carga(IdEstacao_1, std::vector<SmartEnupla<IdVariavelAleatoria, double>>(int(maiorEstacao), SmartEnupla<IdVariavelAleatoria, double>()));

				for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

					SmartEnupla<IdVariavelAleatoria, double> matriz_carga_estacao(IdVariavelAleatoria_1, std::vector<double>(int(idVar_A), 0.0));

					for (IdVariavelAleatoria idVar_B = IdVariavelAleatoria_1; idVar_B <= idVar_A; idVar_B++) {

						if (getAtributo(idVar_B, AttComumVariavelAleatoria_idVariavelAleatoria_determinacao, IdVariavelAleatoria()) == idVar_B) {

							double carga = getElementoMatriz(idVar_A, AttMatrizVariavelAleatoria_correlacao_residuo_lognormal, idVar_B, idEstacao, double());

							if (idVar_B == idVar_A) {

								carga *= (1.0 + 1e-5);

								for (IdVariavelAleatoria idVar_C = IdVariavelAleatoria_1; idVar_C < idVar_B; idVar_C++) {
									if (getAtributo(idVar_C, AttComumVariavelAleatoria_idVariavelAleatoria_determinacao, IdVariavelAleatoria()) == idVar_C) {
										//const double elemento_ = getElementoMatriz(idVar_A, AttMatrizVariavelAleatoria_matriz_carga_residuo_lognormal, idEstacao, idVar_C, double());
										const double elemento = matriz_carga_estacao.getElemento(idVar_C);
										carga -= elemento * elemento;
									} // if (getAtributo(idVar_C, AttComumVariavelAleatoria_idVariavelAleatoria_determinacao, IdVariavelAleatoria()) == idVar_C) {
								} // for (IdVariavelAleatoria idVar_C = IdVariavelAleatoria_1; idVar_C < idVar_B; idVar_C++) {

								if (carga > 0)
									carga = std::sqrt(carga);
								else
									carga = 0;

							} // if (idVar_B == idVar_A) {

							else if (idVar_B < idVar_A) {

								for (IdVariavelAleatoria idVar_C = IdVariavelAleatoria_1; idVar_C < idVar_B; idVar_C++) {
									if (getAtributo(idVar_C, AttComumVariavelAleatoria_idVariavelAleatoria_determinacao, IdVariavelAleatoria()) == idVar_C) {
										//const double elementoA_ = getElementoMatriz(idVar_A, AttMatrizVariavelAleatoria_matriz_carga_residuo_lognormal, idEstacao, idVar_C, double());
										const double elementoA = matriz_carga_estacao.getElemento(idVar_C);
										const double elementoB = getElementoMatriz(idVar_B, AttMatrizVariavelAleatoria_matriz_carga_residuo_lognormal, idEstacao, idVar_C, double());
										carga -= elementoA * elementoB;
									} // if (getAtributo(idVar_C, AttComumVariavelAleatoria_idVariavelAleatoria_determinacao, IdVariavelAleatoria()) == idVar_C) {
								} // for (IdVariavelAleatoria idVar_C = IdVariavelAleatoria_1; idVar_C < idVar_B; idVar_C++) {

								const double denominador = getElementoMatriz(idVar_B, AttMatrizVariavelAleatoria_matriz_carga_residuo_lognormal, idEstacao, idVar_B, double());

								if (denominador > 0)
									carga /= denominador;
								else
									carga = 0;

							} // else if (idVar_B < idVar_A) {

							//vetorVariavelAleatoria.att(idVar_A).addElemento(AttMatrizVariavelAleatoria_matriz_carga_residuo_lognormal, idVar_B, idEstacao, carga);

							matriz_carga_estacao.setElemento(idVar_B, carga);

						} // if (getAtributo(idVar_B, AttComumVariavelAleatoria_idVariavelAleatoria_determinacao, IdVariavelAleatoria()) == idVar_B) {

					} // for (IdVariavelAleatoria idVar_B = IdVariavelAleatoria_1; idVar_B <= idVar_A; idVar_B++) {

					matriz_carga.setElemento(idEstacao, matriz_carga_estacao);

				} // for (IdEstacao idEstacao = IdEstacao_1; idEstacao <= maiorEstacao; idEstacao++) {

				vetorVariavelAleatoria.att(idVar_A).setMatriz_forced(AttMatrizVariavelAleatoria_matriz_carga_residuo_lognormal, matriz_carga);

			} // if (getAtributo(idVar_A, AttComumVariavelAleatoria_idVariavelAleatoria_determinacao, IdVariavelAleatoria()) == idVar_A) {

		} // for (IdVariavelAleatoria idVar_A = IdVariavelAleatoria_1; idVar_A <= maiorVariavelAleatoria; idVar_A++) {

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::calcularMatrizCargaSazonalResiduoVariaveisAleatorias(): \n" + std::string(erro.what())); }

} // void ProcessoEstocastico::calcularMatrizCargaSazonalResiduoVariaveisAleatorias(){




void ProcessoEstocastico::calcularRuidoCorrelacionadoEspacoAmostral(){

	try {

		const IdVariavelAleatoria maiorVariavelAleatoria = getMaiorIdVariavelAleatoria();

		const TipoCorrelacaoVariaveisAleatorias tipo_correlacao_variaveis_aleatorias = getAtributo(AttComumProcessoEstocastico_tipo_correlacao_variaveis_aleatorias, TipoCorrelacaoVariaveisAleatorias());


		IdVariavelAleatoria idVar_ref;
		for (idVar_ref = IdVariavelAleatoria_1; idVar_ref <= maiorVariavelAleatoria; idVar_ref++) {
			if (getSize1Matriz(idVar_ref, AttMatrizVariavelAleatoria_ruido_branco_espaco_amostral) > 0)
				break;
		}

		SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> ruido_branco_ref = getMatriz(idVar_ref, AttMatrizVariavelAleatoria_ruido_branco_espaco_amostral, Periodo(), IdRealizacao(), double());

		for (Periodo periodo = ruido_branco_ref.getIteradorInicial(); periodo <= ruido_branco_ref.getIteradorFinal(); ruido_branco_ref.incrementarIterador(periodo)) {

			for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= ruido_branco_ref.at(periodo).getIteradorFinal(); idRealizacao++) {

				for (IdVariavelAleatoria idVar_A = IdVariavelAleatoria_1; idVar_A <= maiorVariavelAleatoria; idVar_A++) {

					SmartEnupla<IdVariavelAleatoria, double> ruido_branco(IdVariavelAleatoria_1, std::vector<double>(int(maiorVariavelAleatoria), NAN));

					for (IdVariavelAleatoria idVar_B = IdVariavelAleatoria_1; idVar_B <= maiorVariavelAleatoria; idVar_B++) {
						if (getAtributo(idVar_B, AttComumVariavelAleatoria_idVariavelAleatoria_determinacao, IdVariavelAleatoria()) == idVar_B)
							ruido_branco.setElemento(idVar_B, getElementoMatriz(idVar_B, AttMatrizVariavelAleatoria_ruido_branco_espaco_amostral, periodo, idRealizacao, double()));
						else
							ruido_branco.setElemento(idVar_B, 0.0);
					}

					const double ruido_correlacionado = vetorVariavelAleatoria.att(idVar_A).calcularRuidoCorrelacionado(tipo_correlacao_variaveis_aleatorias, periodo.getEstacao(), ruido_branco);

					vetorVariavelAleatoria.att(idVar_A).addElemento(AttMatrizVariavelAleatoria_ruido_correlacionado_espaco_amostral, periodo, idRealizacao, ruido_correlacionado);

				} // for (IdVariavelAleatoria idVar_A = IdVariavelAleatoria_1; idVar_A <= maiorVariavelAleatoria; idVar_A++) {

			} // for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= realizacao_final; idRealizacao++) {

		} // for (Periodo periodo = periodo_inicial; periodo <= periodo_final; periodo++) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::calcularRuidoCorrelacionadoEspacoAmostral(): \n" + std::string(erro.what())); }

} // void ProcessoEstocastico::gerarRuidoCorrelacionadoAmostra(){





IdVariavelAleatoria ProcessoEstocastico::getMaiorIdVariavelAleatoria(){

	try{

		const IdVariavelAleatoria maiorVariavelAleatoria = vetorVariavelAleatoria.getMaiorId();

		if ((maiorVariavelAleatoria <= IdVariavelAleatoria_Nenhum) || (maiorVariavelAleatoria >= IdVariavelAleatoria_Excedente))
			throw std::invalid_argument("Nao ha variaveis aleatorias no ProcessoEstocastico.");

		return maiorVariavelAleatoria;

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::getMaiorIdVariavelAleatoria(): \n" + std::string(erro.what())); }

} // IdVariavelAleatoria ProcessoEstocastico::getMaiorIdVariavelAleatoria(){



void ProcessoEstocastico::setCenariosMapeamento(const AttMatrizProcessoEstocastico a_attMatrizProcessoEstocastico, const SmartEnupla<IdCenario, SmartEnupla<Periodo, IdRealizacao>>& a_matriz){

	try {

		if (a_attMatrizProcessoEstocastico != AttMatrizProcessoEstocastico_mapeamento_espaco_amostral) 
			throw std::invalid_argument("Argumento invalido.");
			//(a_attMatrizProcessoEstocastico != AttMatrizProcessoEstocastico_mapeamento_arvore_cenarios))

		setMatriz(a_attMatrizProcessoEstocastico, a_matriz);

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::setCenariosMapeamento(" + getFullString(a_attMatrizProcessoEstocastico) + "): \n" + std::string(erro.what())); }


}


void ProcessoEstocastico::setCenarios(const AttMatrizVariavelAleatoria a_attMatrizVariavelAleatoria, const SmartEnupla<IdVariavelAleatoria, SmartEnupla<Periodo, SmartEnupla<IdCenario, double>>> &a_enupla){
	try {


		if (a_attMatrizVariavelAleatoria != AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral)
			throw std::invalid_argument("Argumento invalido.");

		const IdVariavelAleatoria maiorVariavelAleatoria = getMaiorIdVariavelAleatoria();

		for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= maiorVariavelAleatoria; idVar++) 
			vetorVariavelAleatoria.att(idVar).setMatriz(a_attMatrizVariavelAleatoria, a_enupla.at(idVar));

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::setCenarios(" + getFullString(a_attMatrizVariavelAleatoria) + "a_enupla): \n" + std::string(erro.what())); }

} // void ProcessoEstocastico::setCenarios(const AttMatrizVariavelAleatoria a_attMatrizVariavelAleatoria, const SmartEnupla<IdVariavelAleatoria, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>> &a_enupla){


void ProcessoEstocastico::setCenariosInternos(const IdVariavelAleatoria a_idVariavelAleatoria, const AttMatrizVariavelAleatoriaInterna a_attMatrizVariavelAleatoriaInterna, const SmartEnupla<IdVariavelAleatoriaInterna, SmartEnupla<Periodo, SmartEnupla<IdCenario, double>>>& a_matriz){

	try{

		vetorVariavelAleatoria.att(a_idVariavelAleatoria).setCenariosInternos(a_attMatrizVariavelAleatoriaInterna, a_matriz);

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::setCenariosInternos(" + getFullString(a_idVariavelAleatoria) + "," + getFullString(a_attMatrizVariavelAleatoriaInterna) + "a_enupla): \n" + std::string(erro.what())); }

} // void ProcessoEstocastico::setCenariosInternos(const IdVariavelAleatoria a_idVariavelAleatoria, const AttMatrizVariavelAleatoriaInterna a_attMatrizVariavelAleatoriaInterna, const SmartEnupla<IdVariavelAleatoriaInterna, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>>& a_matriz){


void ProcessoEstocastico::mapearCenariosEspacoAmostralPorSorteio(const TipoSorteio a_tipo_sorteio, const int a_numero_cenarios_global, const IdCenario a_menorIdcenario, const IdCenario a_maiorIdcenario, int &a_semente) {

	try {

		if (getSize1Matriz(IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_residuo_espaco_amostral) == 0)
			return;

		const SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> horizonte_espaco_amostral = getMatriz(IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_residuo_espaco_amostral, Periodo(), IdRealizacao(), double());

		const Periodo periodo_final = horizonte_espaco_amostral.getIteradorFinal();


		Periodo periodo_seguinte_mapeamento_existente = horizonte_espaco_amostral.getIteradorInicial();
		if (getSizeMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral) > 0) {
			if (getIterador1Inicial(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario()) > a_menorIdcenario)
				throw std::invalid_argument("Matriz inicializada com cenario inicial maior que " + getFullString(a_menorIdcenario) + ".");

			if (getIterador1Final(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario()) < a_maiorIdcenario)
				throw std::invalid_argument("Matriz inicializada com cenario final menor que " + getFullString(a_maiorIdcenario) + ".");

			if (getIterador2Final(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_menorIdcenario, Periodo()) == periodo_final)
				return;

			periodo_seguinte_mapeamento_existente = getIterador2Final(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_menorIdcenario, Periodo());
			horizonte_espaco_amostral.incrementarIterador(periodo_seguinte_mapeamento_existente);
		}


		Periodo periodo_inicial = periodo_seguinte_mapeamento_existente;
		for (Periodo periodo = periodo_seguinte_mapeamento_existente; periodo <= periodo_final; horizonte_espaco_amostral.incrementarIterador(periodo)) {
			if (!mapearCenariosEspacoAmostralCompletoPorPeriodo(periodo_inicial, a_numero_cenarios_global, a_menorIdcenario, a_maiorIdcenario))
				break;
			else
				horizonte_espaco_amostral.incrementarIterador(periodo_inicial);
		}

		if (periodo_inicial > periodo_final)
			return;

		SmartEnupla<Periodo, int> horizonte_mapeamento_cenarios_por_sorteio;
		for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_espaco_amostral.incrementarIterador(periodo))
			horizonte_mapeamento_cenarios_por_sorteio.addElemento(periodo, 0);

		if (a_tipo_sorteio != TipoSorteio_uniforme)
			throw std::invalid_argument("Tipo de sorteio nao utilizado no mapeamento de cenarios.");

		SmartEnupla<IdCenario, SmartEnupla<Periodo, IdRealizacao>> cenarios;

		for (IdCenario idCenario = IdCenario_1; idCenario < IdCenario_Excedente; idCenario++) {

			if ((a_menorIdcenario <= idCenario) && (idCenario <= a_maiorIdcenario)) {

				SmartEnupla<Periodo, IdRealizacao> realizacoes_por_periodo(horizonte_mapeamento_cenarios_por_sorteio, IdRealizacao_Nenhum);

				Periodo periodo = periodo_inicial;
				for (int p = 1; p <= numero_maximo_periodos_espaco_amostral; p++) {

					if (periodo.isValido()) {
						realizacoes_por_periodo.setElemento(periodo, getUniforme_Elemento(a_semente, getIterador2Final(IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodo, IdRealizacao())));
						if (periodo == periodo_final)
							periodo = Periodo();
						else if (periodo < periodo_final)
							horizonte_mapeamento_cenarios_por_sorteio.incrementarIterador(periodo);
					}
					else
						getUniforme_Elemento(a_semente, getIterador2Final(IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodo_final, IdRealizacao()));

				} // for (int p = 1; p <= numero_maximo_periodos_espaco_amostral; p++){

				cenarios.addElemento(idCenario, realizacoes_por_periodo);

			} // if ((a_menorIdcenario <= idCenario) && (idCenario <= a_maiorIdcenario)) {

			else {
				for (int p = 1; p <= numero_maximo_periodos_espaco_amostral; p++)
					getUniforme_Elemento(a_semente, getIterador2Final(IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodo_final, IdRealizacao()));
			}

		} // for (IdCenario idCenario = IdCenario_1; idCenario < IdCenario_Excedente; idCenario++) {

		if (periodo_seguinte_mapeamento_existente == horizonte_espaco_amostral.getIteradorInicial())
			setMatriz_forced(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, cenarios);
		else {
			for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_mapeamento_cenarios_por_sorteio.incrementarIterador(periodo)) {
				for (IdCenario idCenario = a_menorIdcenario; idCenario <= a_maiorIdcenario; idCenario++)
					addElemento(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, idCenario, periodo, cenarios.at(idCenario).at(periodo));
			}
		}

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::mapearCenariosEspacoAmostralPorSorteio(" + getFullString(a_tipo_sorteio) + "," + getFullString(a_numero_cenarios_global) + "," + getFullString(a_menorIdcenario) + "," + getFullString(a_maiorIdcenario) + "," + getFullString(a_semente) + "): \n" + std::string(erro.what())); }

} // void ProcessoEstocastico::mapearCenariosEspacoAmostralPorSorteio(const TipoSorteio a_tipo_sorteio, const IdCenario a_cenario_inicial, const IdCenario a_cenario_final){


bool ProcessoEstocastico::mapearCenariosEspacoAmostralCompleto(const int a_numero_cenarios_global, const IdCenario a_menorIdcenario, const IdCenario a_maiorIdcenario){

	try {

		if (getSize1Matriz(IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_residuo_espaco_amostral) == 0)
			return false;

		const int numero_cenarios_local = int(a_maiorIdcenario - a_menorIdcenario) + 1;
		if (numero_cenarios_local <= 0)
			throw std::invalid_argument("Cenarios Invalidos.");

		const int numero_cenarios_global = int(a_numero_cenarios_global);

		if (numero_cenarios_global > numero_maximo_cenarios_mapeamento_completo_espaco_amostral)
			return false;

		const Periodo periodo_inicial = getIterador1Inicial(IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_residuo_espaco_amostral, Periodo());
		const Periodo periodo_final = getIterador1Final  (IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_residuo_espaco_amostral, Periodo());

		const int numero_periodos = periodo_final - periodo_inicial + 1;

		// Verifica se pode ser realizado o mapeamento completo das amostras
		if (numero_periodos > numero_maximo_periodos_mapeamento_completo_espaco_amostral)
			return false;

		int numero_maximo_cenarios_amostra_comum = 1;

		SmartEnupla<Periodo, int> numero_repeticoes_acumulado_bw(periodo_inicial, std::vector<int>(numero_periodos, 1));
		SmartEnupla<Periodo, int> numero_repeticoes_acumulado_fw(periodo_inicial, std::vector<int>(numero_periodos, 1));

		int p = 0;
		for (Periodo periodo = periodo_inicial; periodo <= periodo_final ; periodo++) {
			const int numero_realizacoes = int(getIterador2Final(IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodo, IdRealizacao()));

			numero_maximo_cenarios_amostra_comum *= numero_realizacoes;
			if (numero_maximo_cenarios_amostra_comum > numero_maximo_cenarios_mapeamento_completo_espaco_amostral)
				return false;

			else if (periodo > periodo_inicial){
				const int numero_realizacoes_bw = int(getIterador2Final(IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodo_final - p + 1, IdRealizacao()));
				const int numero_realizacoes_fw = int(getIterador2Final(IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodo - 1, IdRealizacao()));

				numero_repeticoes_acumulado_fw.at(periodo)           = numero_repeticoes_acumulado_fw.at(periodo - 1) * numero_realizacoes_fw;
				numero_repeticoes_acumulado_bw.at(periodo_final - p) = numero_repeticoes_acumulado_bw.at(periodo_final - p + 1) * numero_realizacoes_bw;
			}

			p++;

		} // for (Periodo periodo = periodo_inicial; periodo <= periodo_final ; periodo++) {

		if (numero_cenarios_global != numero_maximo_cenarios_amostra_comum)
			return false;

		SmartEnupla<IdCenario, SmartEnupla<Periodo, IdRealizacao>> cenarios(a_menorIdcenario, std::vector<SmartEnupla<Periodo, IdRealizacao>>(numero_cenarios_local, SmartEnupla<Periodo, IdRealizacao>(periodo_inicial, std::vector<IdRealizacao>(numero_periodos, IdRealizacao_Nenhum))));


		for (int c = 0; c < 1; c++) {

			for (Periodo periodo = periodo_inicial; periodo <= periodo_final; periodo++) {

				const IdRealizacao maiorIdRealizacao = getIterador2Final(IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodo, IdRealizacao());

				IdCenario idCenario = IdCenario((c * numero_cenarios_global) + 1);

				for (int f = 0; f < numero_repeticoes_acumulado_fw.at(periodo); f++) {

					for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao; idRealizacao++) {

						for (int b = 0; b < numero_repeticoes_acumulado_bw.at(periodo); b++) {

							if ((idCenario >= a_menorIdcenario) && (idCenario <= a_maiorIdcenario))
								cenarios.at(idCenario).at(periodo) = idRealizacao;

							idCenario++;

						} // for (int b = 0; b < numero_repeticoes_acumulado_bw.at(periodo); b++) {

					} // for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao; idRealizacao++) {

				} // for (int f = 0; f < numero_repeticoes_acumulado_fw.at(periodo); f++) {

			} // for (Periodo periodo = periodo_inicial; periodo <= periodo_final; periodo++) {

		} // for (int c = 0; c < a_numero_cenarios_tendencia; c++) {

		setMatriz_forced(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, cenarios);

		return true;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::mapearCenariosEspacoAmostralCompleto(" + "," + getFullString(a_numero_cenarios_global) + "," + getFullString(a_menorIdcenario) + "," + getFullString(a_maiorIdcenario) + "," + "): \n" + std::string(erro.what())); }


} // bool ProcessoEstocastico::mapearCenariosEspacoAmostralCompleta(const SmartEnupla<IdCenario, IdCenario>& a_mapeamento_cenarios_tendencia){

bool ProcessoEstocastico::mapearCenariosEspacoAmostralCompletoPorPeriodo(const Periodo a_periodo_final, const int a_numero_cenarios_global, const IdCenario a_menorIdcenario, const IdCenario a_maiorIdcenario) {

	try {

		if (getSize1Matriz(IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_residuo_espaco_amostral) == 0)
			return false;

		const int numero_cenarios_local = int(a_maiorIdcenario - a_menorIdcenario) + 1;
		if (numero_cenarios_local <= 0)
			throw std::invalid_argument("Cenarios Invalidos.");

		const int numero_cenarios_global = int(a_numero_cenarios_global);

		const SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> horizonte_espaco_amostral = getMatriz(IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_residuo_espaco_amostral, Periodo(), IdRealizacao(), double());

		const Periodo periodo_inicial = horizonte_espaco_amostral.getIteradorInicial();

		int numero_maximo_cenarios_amostra_comum = 1;

		SmartEnupla<Periodo, int> numero_repeticoes_acumulado_bw(horizonte_espaco_amostral, 1);
		SmartEnupla<Periodo, int> numero_repeticoes_acumulado_fw = numero_repeticoes_acumulado_bw;

		int p = 0;
		for (Periodo periodo = periodo_inicial; periodo <= a_periodo_final; horizonte_espaco_amostral.incrementarIterador(periodo)) {
			const int numero_realizacoes = int(getIterador2Final(IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodo, IdRealizacao()));

			numero_maximo_cenarios_amostra_comum *= numero_realizacoes;
			if (numero_maximo_cenarios_amostra_comum > numero_cenarios_global)
				return false;

			else if (periodo > periodo_inicial) {

				Periodo periodo_menos_1 = periodo;
				horizonte_espaco_amostral.decrementarIterador(periodo_menos_1);

				Periodo periodo_final_menos_p_mais_1 = a_periodo_final;
				horizonte_espaco_amostral.decrementarIterador(periodo_final_menos_p_mais_1, p - 1);

				Periodo periodo_final_menos_p = a_periodo_final;
				horizonte_espaco_amostral.decrementarIterador(periodo_final_menos_p, p);

				const int numero_realizacoes_bw = int(getIterador2Final(IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodo_final_menos_p_mais_1, IdRealizacao()));
				const int numero_realizacoes_fw = int(getIterador2Final(IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodo_menos_1, IdRealizacao()));

				numero_repeticoes_acumulado_fw.at(periodo) = numero_repeticoes_acumulado_fw.at(periodo_menos_1) * numero_realizacoes_fw;
				numero_repeticoes_acumulado_bw.at(periodo_final_menos_p) = numero_repeticoes_acumulado_bw.at(periodo_final_menos_p_mais_1) * numero_realizacoes_bw;
			}

			p++;

		} // for (Periodo periodo = periodo_inicial; periodo <= periodo_final ; periodo++) {

		if (numero_cenarios_global < numero_maximo_cenarios_amostra_comum)
			return false;

		if ((numero_cenarios_global % numero_maximo_cenarios_amostra_comum) != 0)
			return false;

		const int num_rep = (numero_cenarios_global / numero_maximo_cenarios_amostra_comum);

		SmartEnupla<IdCenario, SmartEnupla<Periodo, IdRealizacao>> cenarios(a_menorIdcenario, std::vector<SmartEnupla<Periodo, IdRealizacao>>(numero_cenarios_local, SmartEnupla<Periodo, IdRealizacao>(horizonte_espaco_amostral, IdRealizacao_Nenhum)));

		for (int c = 0; c < num_rep; c++) {

			for (Periodo periodo = periodo_inicial; periodo <= a_periodo_final; horizonte_espaco_amostral.incrementarIterador(periodo)) {

				const IdRealizacao maiorIdRealizacao = getIterador2Final(IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodo, IdRealizacao());

				IdCenario idCenario = IdCenario((c * numero_maximo_cenarios_amostra_comum) + 1);

				for (int f = 0; f < numero_repeticoes_acumulado_fw.at(periodo); f++) {

					for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao; idRealizacao++) {

						for (int b = 0; b < numero_repeticoes_acumulado_bw.at(periodo); b++) {

							if ((idCenario >= a_menorIdcenario) && (idCenario <= a_maiorIdcenario))
								cenarios.at(idCenario).at(periodo) = idRealizacao;

							idCenario++;

						} // for (int b = 0; b < numero_repeticoes_acumulado_bw.at(periodo); b++) {

					} // for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao; idRealizacao++) {

				} // for (int f = 0; f < numero_repeticoes_acumulado_fw.at(periodo); f++) {

			} // for (Periodo periodo = periodo_inicial; periodo <= periodo_final; periodo++) {

		} // for (int c = 0; c < a_numero_cenarios_tendencia; c++) {

		setMatriz_forced(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, cenarios);

		return true;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::mapearCenariosEspacoAmostralCompletoPorPeriodo(" + "," + getFullString(a_numero_cenarios_global) + "," + getFullString(a_menorIdcenario) + "," + getFullString(a_maiorIdcenario) + "," + "): \n" + std::string(erro.what())); }


} // bool ProcessoEstocastico::mapearCenariosEspacoAmostralCompletoPorPeriodo(const SmartEnupla<IdCenario, IdCenario>& a_mapeamento_cenarios_tendencia){


void ProcessoEstocastico::reducao_espaco_amostral(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_reducao_cenarios, IdProcesso a_idProcesso, const SmartEnupla <IdEstagio, int> a_numero_aberturas, const SmartEnupla <IdEstagio, Periodo> a_horizonte_otimizacao, const IdEstagio a_estagio_acoplamento_pre_estudo, const IdEstagio a_estagio_final, TipoSolver& a_tipoSolver)
{

	try {

		std::vector<std::vector<std::string>> valores_tempo(2, std::vector<std::string>());

		if (a_idProcesso == IdProcesso_mestre) {
			a_entradaSaidaDados.setDiretorioSaida(a_diretorio_reducao_cenarios);
			std::cout << "Iniciado algoritmo de reducao de cenarios" << std::endl;
		}
			
		//Inicializa enupla que tem a info do espaço amostral reduzido

		std::vector<std::vector<std::vector<double>>> residuo_espaco_amostral_reduzido; //ite_1: estágio //ite_2: Realização / ite_3: Variável Aleatória
		std::vector<std::vector<double>>			  probabilidade_reduzido; //ite_1: estágio //ite_2: Realização

		for (IdEstagio idEstagio = a_estagio_acoplamento_pre_estudo; idEstagio <= a_estagio_final; idEstagio++)
			probabilidade_reduzido.push_back(std::vector<double>(a_numero_aberturas.getElemento(idEstagio), std::pow(a_numero_aberturas.getElemento(idEstagio), -1)));

		//1. Redução Inicial (realiza um merge das realizações com menor norma_2 até obter a estrutura reduzida)

		auto start_clock = std::chrono::high_resolution_clock::now();
		reducao_inicial(a_entradaSaidaDados, residuo_espaco_amostral_reduzido, a_numero_aberturas, a_horizonte_otimizacao, a_estagio_acoplamento_pre_estudo, a_estagio_final);
		auto stop_clock = std::chrono::high_resolution_clock::now(); 

		if (a_idProcesso == IdProcesso_mestre) {
			std::cout << "Reducao - Etapa 1 - Tempo [min] = " << std::chrono::duration<double>(stop_clock - start_clock).count() / 60 << std::endl;

			valores_tempo.at(0).push_back(std::string("Tempo_etapa_1 [min]") + ";" + getString(std::chrono::duration<double>(stop_clock - start_clock).count() / 60));

		}

		//2. Adaptação ótima com Nested Distance
		start_clock = std::chrono::high_resolution_clock::now();
		reducao_adaptacao_nested_distance(a_entradaSaidaDados, a_diretorio_reducao_cenarios, a_idProcesso, residuo_espaco_amostral_reduzido, probabilidade_reduzido, a_numero_aberturas, a_horizonte_otimizacao, a_estagio_acoplamento_pre_estudo, a_estagio_final, a_tipoSolver);
		stop_clock = std::chrono::high_resolution_clock::now();

		if (a_idProcesso == IdProcesso_mestre) {
			std::cout << "Reducao - Etapa 2 - Tempo [min] = " << std::chrono::duration<double>(stop_clock - start_clock).count() / 60 << std::endl;

			valores_tempo.at(1).push_back(std::string("Tempo_etapa_2 [min]") + ";" + getString(std::chrono::duration<double>(stop_clock - start_clock).count() / 60));

		}

		//3.Atualiza o processo estocástico com a amostra reduzida

		const IdVariavelAleatoria maiorIdVariavelAleatoria = getMaiorId(IdVariavelAleatoria());

		SmartEnupla<IdVariavelAleatoria, SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>>> enupla_residuo_espaco_amostral_reduzido(IdVariavelAleatoria_1, std::vector<SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>>>(maiorIdVariavelAleatoria, SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>>()));
		SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> enupla_probabilidade_reduzido;

		for (IdEstagio idEstagio = a_estagio_acoplamento_pre_estudo; idEstagio <= a_estagio_final; idEstagio++) {

			const Periodo periodo = a_horizonte_otimizacao.getElemento(idEstagio);

			for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= maiorIdVariavelAleatoria; idVariavelAleatoria++)
				enupla_residuo_espaco_amostral_reduzido.at(idVariavelAleatoria).addElemento(periodo, SmartEnupla<IdRealizacao, double>(IdRealizacao_1, std::vector<double>(a_numero_aberturas.getElemento(idEstagio), getdoubleFromChar("max"))));

			enupla_probabilidade_reduzido.addElemento(periodo, SmartEnupla<IdRealizacao, double>(IdRealizacao_1, std::vector<double>(a_numero_aberturas.getElemento(idEstagio), std::pow(a_numero_aberturas.getElemento(idEstagio), -1))));

			for (int num = 0; num < a_numero_aberturas.getElemento(idEstagio); num++) {

				for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= maiorIdVariavelAleatoria; idVariavelAleatoria++)
					enupla_residuo_espaco_amostral_reduzido.at(idVariavelAleatoria).at(periodo).setElemento(IdRealizacao(num + 1), residuo_espaco_amostral_reduzido.at(int(idEstagio - a_estagio_acoplamento_pre_estudo)).at(num).at(int(idVariavelAleatoria - 1)));

				enupla_probabilidade_reduzido.at(periodo).setElemento(IdRealizacao(num + 1), probabilidade_reduzido.at(int(idEstagio - a_estagio_acoplamento_pre_estudo)).at(num));

			}//for (int pos = 0; pos < a_numero_aberturas.getElemento(idEstagio); pos++) {

		}//for (IdEstagio idEstagio = a_estagio_acoplamento_pre_estudo; idEstagio <= a_estagio_final; idEstagio++) {

		setMatriz_forced(AttMatrizProcessoEstocastico_probabilidade_realizacao, enupla_probabilidade_reduzido);

		for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= maiorIdVariavelAleatoria; idVariavelAleatoria++)
			vetorVariavelAleatoria.att(idVariavelAleatoria).setMatriz_forced(AttMatrizVariavelAleatoria_residuo_espaco_amostral, enupla_residuo_espaco_amostral_reduzido.at(idVariavelAleatoria));


		if (a_idProcesso == IdProcesso_mestre)
			a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla("tempos_reducao_cenarios.csv", valores_tempo);



	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::reducao_espaco_amostral(" + "): \n" + std::string(erro.what())); }

}


void ProcessoEstocastico::reducao_inicial(EntradaSaidaDados a_entradaSaidaDados, std::vector<std::vector<std::vector<double>>>& a_residuo_espaco_amostral_reduzido, const SmartEnupla <IdEstagio, int> a_numero_aberturas, const SmartEnupla <IdEstagio, Periodo> a_horizonte_otimizacao, const IdEstagio a_estagio_acoplamento_pre_estudo, const IdEstagio a_estagio_final)
{

	try {

		for (IdEstagio idEstagio = a_estagio_acoplamento_pre_estudo; idEstagio <= a_estagio_final; idEstagio++) {

			const Periodo periodo = a_horizonte_otimizacao.getElemento(idEstagio);

			const IdRealizacao maiorIdRealizacao_original = getIterador2Final(AttMatrizProcessoEstocastico_probabilidade_realizacao, periodo, IdRealizacao());
			const IdVariavelAleatoria maiorIdVariavelAleatoria = getMaiorId(IdVariavelAleatoria());

			std::vector<std::vector<double>> realizacoes_amostra(int(maiorIdRealizacao_original), std::vector<double>(int(maiorIdVariavelAleatoria), getdoubleFromChar("max"))); //Linhas: Realização / Colunas: Variável Aleatória

			//
			//1. Preenche realizacoes_amostra com a árvore original
			//

			for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao_original; idRealizacao++) {

				for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= maiorIdVariavelAleatoria; idVariavelAleatoria++)
					realizacoes_amostra.at(int(idRealizacao - 1)).at(int(idVariavelAleatoria - 1)) = vetorVariavelAleatoria.att(idVariavelAleatoria).getElementoMatriz(AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodo, idRealizacao, double());

			}//for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao_original; idRealizacao++) {

			//
			//2. Realiza merge de realizações com base na menor norma euclidiana 
			//

			//2.1 Inicializa vetores controle

			int numero_combinacoes_distancias = 0;

			for (int num = 1; num < int(realizacoes_amostra.size()); num++)
				numero_combinacoes_distancias += (int(realizacoes_amostra.size()) - num);

			std::vector<double> norma_2(numero_combinacoes_distancias, getdoubleFromChar("max"));
			std::vector<int> index_1(numero_combinacoes_distancias, -1);
			std::vector<int> index_2(numero_combinacoes_distancias, -1);
			std::vector<bool> considerar_combinacao(numero_combinacoes_distancias, true);
			std::vector<int> index_realizacoes(int(maiorIdRealizacao_original), -1);
			std::vector<int> considerar_index_realizacoes(int(maiorIdRealizacao_original), true);

			int pos = 0;
			int index_realizacao = 0;

			for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao_original; idRealizacao++) {

				index_realizacao++;
				index_realizacoes.at(int(idRealizacao - 1)) = index_realizacao;

				if (idRealizacao < maiorIdRealizacao_original) {

					for (IdRealizacao idRealizacao_aux = IdRealizacao(idRealizacao + 1); idRealizacao_aux <= maiorIdRealizacao_original; idRealizacao_aux++) {
						norma_2.at(pos) = calcularNorma2(realizacoes_amostra.at(int(idRealizacao - 1)), realizacoes_amostra.at(int(idRealizacao_aux - 1)));
						index_1.at(pos) = int(idRealizacao);
						index_2.at(pos) = int(idRealizacao_aux);

						pos++;
					}//for (IdRealizacao idRealizacao_aux = IdRealizacao(idRealizacao + 1); idRealizacao_aux <= maiorIdRealizacao_original; idRealizacao_aux++) {

				}//if (idRealizacao < maiorIdRealizacao_original) {

			}//for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao < maiorIdRealizacao_original; idRealizacao++) {


			//2.2 Merge

			const int numero_merges = int(maiorIdRealizacao_original) - a_numero_aberturas.getElemento(idEstagio);

			for (int merge = 0; merge < numero_merges; merge++) {

				//Determina o index da menor distância

				int index_min_distancia = -1;
				double norma_2_aux = getdoubleFromChar("max");

				const int size_norma_2 = int(norma_2.size());

				for (pos = 0; pos < size_norma_2; pos++) {

					if (norma_2.at(pos) < norma_2_aux && considerar_combinacao.at(pos)) {
						norma_2_aux = norma_2.at(pos);
						index_min_distancia = pos;
					}//if (norma_2.at(pos) < norma_2_aux && considerar_combinacao.at(pos)) {

				}//for (pos = 0; pos < size_norma_2; pos++) {


				//Atualiza vetores
				//Nota: ordem importante para apagar elementos do realizacoes_amostra: primeiro o maior index

				std::vector<int> index_merge(2, -1);

				if (index_2.at(index_min_distancia) > index_1.at(index_min_distancia)) {
					index_merge.at(0) = index_2.at(index_min_distancia);
					index_merge.at(1) = index_1.at(index_min_distancia);
				}//if (index_2.at(index_min_distancia) > index_1.at(index_min_distancia)) {
				else {
					index_merge.at(0) = index_1.at(index_min_distancia);
					index_merge.at(1) = index_2.at(index_min_distancia);
				}//else {

				std::vector<double> realizacoes_amostra_merge(int(maiorIdVariavelAleatoria), 0.0);

				for (int aux = 0; aux < int(index_merge.size()); aux++) {

					//Mapea a realizacao

					bool realizacao_encontrada = false;

					for (pos = 0; pos < int(index_realizacoes.size()); pos++) {

						if (index_realizacoes.at(pos) == index_merge.at(aux)) {

							//Merge realizações
							for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= maiorIdVariavelAleatoria; idVariavelAleatoria++)
								realizacoes_amostra_merge.at(int(idVariavelAleatoria - 1)) += 0.5 * realizacoes_amostra.at(pos).at(int(idVariavelAleatoria - 1));

							considerar_index_realizacoes.at(pos) = false;

							realizacao_encontrada = true;
							break;

						}//if (index_realizacoes.at(pos) == index_merge.at(aux)) {

					}//for (pos = 0; pos < int(index_realizacoes.size()); pos++) {

					if (!realizacao_encontrada)
						throw std::invalid_argument("Index nao encontrado na reducao inicial da amostra comum");


					//Elimina elementos com a posição do index_1
					const int size_index_1 = int(index_1.size());

					for (pos = size_index_1 - 1; pos >= 0; pos--) {

						if (index_1.at(pos) == index_merge.at(aux))
							considerar_combinacao.at(pos) = false;

					}//for (pos = size_index_1-1; pos >= 0; pos--) {

					//Elimina elementos com a posição do index_2
					const int size_index_2 = int(index_2.size());

					for (pos = size_index_2 - 1; pos >= 0; pos--) {

						if (index_2.at(pos) == index_merge.at(aux))
							considerar_combinacao.at(pos) = false;

					}//for (pos = size_index_1-1; pos >= 0; pos--) {

				}//for (int aux = 0; aux < int(index_merge.size()); aux++) {

				//Atualiza árvore
				realizacoes_amostra.push_back(realizacoes_amostra_merge);

				index_realizacao++;
				index_realizacoes.push_back(index_realizacao);
				considerar_index_realizacoes.push_back(true);

				//Calcula novas distâncias com a nova realização obtida do merge
				for (int aux = 0; aux < int(index_realizacoes.size()) - 1; aux++) {

					if (considerar_index_realizacoes.at(aux)) {

						index_1.push_back(index_realizacoes.at(aux));
						index_2.push_back(index_realizacoes.at(int(index_realizacoes.size()) - 1));
						norma_2.push_back(calcularNorma2(realizacoes_amostra.at(aux), realizacoes_amostra.at(int(realizacoes_amostra.size()) - 1)));
						considerar_combinacao.push_back(true);

					}//if (considerar_index_realizacoes.at(aux)) {

				}//for (int aux = 0; aux < int(index_realizacoes.size()) - 1; aux++) {

			}//for (int merge = 0; merge < numero_merges; merge++) {

			//2.3 Armazena valores da árvore reduzida (as realizaçoes com considerar_index_realizacoes = true são o resultado do merge)

			std::vector<std::vector<double>> realizacoes_amostra_reduzido(a_numero_aberturas.getElemento(idEstagio), std::vector<double>()); //Linhas: Realização / Colunas: Variável Aleatória

			pos = 0;

			for (int aux = 0; aux < int(index_realizacoes.size()); aux++) {

				if (considerar_index_realizacoes.at(aux)) {
					realizacoes_amostra_reduzido.at(pos) = realizacoes_amostra.at(aux);
					pos++;
				}//if (considerar_index_realizacoes.at(aux)) {
					
			}//for (int aux = 0; aux < int(index_realizacoes.size()) - 1; aux++) {

			a_residuo_espaco_amostral_reduzido.push_back(realizacoes_amostra_reduzido);

		}//for (IdEstagio idEstagio = a_estagio_acoplamento_pre_estudo; idEstagio <= a_estagio_final; idEstagio++) {

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::reducao_inicial(" + "): \n" + std::string(erro.what())); }

}

void ProcessoEstocastico::reducao_adaptacao_nested_distance(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_reducao_cenarios, IdProcesso a_idProcesso, std::vector<std::vector<std::vector<double>>>& a_residuo_espaco_amostral_reduzido, std::vector<std::vector<double>>& a_probabilidade_reduzido, const SmartEnupla <IdEstagio, int> a_numero_aberturas, const SmartEnupla <IdEstagio, Periodo> a_horizonte_otimizacao, const IdEstagio a_estagio_acoplamento_pre_estudo, const IdEstagio a_estagio_final, TipoSolver& a_tipoSolver)
{

	try {

		std::vector<std::vector<std::string>> valores_nested_distance;	

		//*****************************
		//1. Cria modelo de otimização
		//*****************************

		const IdVariavelAleatoria maiorIdVariavelAleatoria = getMaiorId(IdVariavelAleatoria());

		SmartEnupla <IdEstagio, Solver*> vetorPtrSolver;
		SmartEnupla <IdEstagio, SmartEnupla<IdRealizacao, int>> var_matriz_transporte_estagio;//Armazena a posição da primeira variável associada com um IdRealizacao da amostra original

		SmartEnupla < IdEstagio, int> var_probabilidades_arvore_reduzida_estagio;

		for (IdEstagio idEstagio = a_estagio_acoplamento_pre_estudo; idEstagio <= a_estagio_final; idEstagio++) {

			const Periodo periodo = a_horizonte_otimizacao.getElemento(idEstagio);

			///////////////////////////////////////////////////////
			//1.1 Instancia solver para cada estágio
			///////////////////////////////////////////////////////
			vetorPtrSolver.addElemento(idEstagio, instanciarSolver(a_tipoSolver));

			vetorPtrSolver.at(idEstagio)->setMetodo(TipoMetodoSolver_dual_simplex);

			///////////////////////////////////////////////////////
			//1.2. Cria variáveis
			///////////////////////////////////////////////////////

			const IdRealizacao maiorIdRealizacao_original = getIterador2Final(AttMatrizProcessoEstocastico_probabilidade_realizacao, periodo, IdRealizacao());

			SmartEnupla<IdRealizacao, int> var_matriz_transporte(IdRealizacao_1, std::vector<int>(maiorIdRealizacao_original, 0));

			int var = 0;

			//Variáveis da matriz de transporte
			for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao_original; idRealizacao++) {

				var_matriz_transporte.setElemento(idRealizacao, var);

				for (int num = 0; num < a_numero_aberturas.getElemento(idEstagio); num++) {

					vetorPtrSolver.at(idEstagio)->addVar(0.0, 1.0, 0.0, "Transporte_" + getString(idRealizacao) + "_" + getString(num + 1));
					var++;
				}//for (int num = 0; num < a_numero_aberturas.getElemento(idEstagio); num++) {

			}//for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao_original; idRealizacao++) {

			var_matriz_transporte_estagio.addElemento(idEstagio, var_matriz_transporte);

			//Variáveis da probabilidade da árvore reduzida
			var_probabilidades_arvore_reduzida_estagio.addElemento(idEstagio, var);

			for (int num = 0; num < a_numero_aberturas.getElemento(idEstagio); num++) {
				vetorPtrSolver.at(idEstagio)->addVar(0.0, 1.0, 0.0, "p_" + getString(num + 1));
				var++;
			}//for (int num = 0; num < a_numero_aberturas.getElemento(idEstagio); num++) {

			///////////////////////////////////////////////////////
			//1.3 Cria restrições
			///////////////////////////////////////////////////////

			//Restrições associadas à amostra_original
			//Somatorio(PIij) = probabilidade_amostra_original (i fixo e j barrendo as aberturas da amostra_reduzida)

			for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao_original; idRealizacao++) {

				const int posEqu = vetorPtrSolver.at(idEstagio)->addConstrIgual("Transporte_amostra_original_" + getFullString(idRealizacao));
				vetorPtrSolver.at(idEstagio)->setRHSRestricao(posEqu, getElementoMatriz(AttMatrizProcessoEstocastico_probabilidade_realizacao, periodo, idRealizacao, double()));

				for (int num = 0; num < a_numero_aberturas.getElemento(idEstagio); num++) {

					const int posVar = var_matriz_transporte_estagio.at(idEstagio).getElemento(idRealizacao) + num;
					vetorPtrSolver.at(idEstagio)->setCofRestricao(posVar, posEqu, 1.0);

				}//for (int num = 0; num < a_numero_aberturas.getElemento(idEstagio); num++) {

			}//for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao_original; idRealizacao++) {

			//Restrições associadas à amostra_reduzida
			//Somatorio(PIij) - probabilidade_amostra_reduzida = 0 (j fixo e i barrendo as aberturas da amostra_original)

			for (int num = 0; num < a_numero_aberturas.getElemento(idEstagio); num++) {

				const int posEqu = vetorPtrSolver.at(idEstagio)->addConstrIgual("Transporte_amostra_reduzida_" + getString(num + 1));
				vetorPtrSolver.at(idEstagio)->setRHSRestricao(posEqu, 0.0);

				for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao_original; idRealizacao++) {

					const int posVar = var_matriz_transporte_estagio.at(idEstagio).getElemento(idRealizacao) + num;
					vetorPtrSolver.at(idEstagio)->setCofRestricao(posVar, posEqu, 1.0);

				}//for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao_original; idRealizacao++) {

				const int posVar = var_probabilidades_arvore_reduzida_estagio.getElemento(idEstagio) + num;
				vetorPtrSolver.at(idEstagio)->setCofRestricao(posVar, posEqu, -1.0);

			}//for (int num = 0; num < a_numero_aberturas.getElemento(idEstagio); num++) {

			//Restrições associadas às probabilidades da amostra_reduzida
			//Somatorio(pj) = 1

			const int posEqu = vetorPtrSolver.at(idEstagio)->addConstrIgual("Probabilidade_amostra_reduzida");
			vetorPtrSolver.at(idEstagio)->setRHSRestricao(posEqu, 1.0);

			for (int num = 0; num < a_numero_aberturas.getElemento(idEstagio); num++) {

				const int posVar = var_probabilidades_arvore_reduzida_estagio.getElemento(idEstagio) + num;
				vetorPtrSolver.at(idEstagio)->setCofRestricao(posVar, posEqu, 1.0);

			}//for (int num = 0; num < a_numero_aberturas.getElemento(idEstagio); num++) {

		}//for (IdEstagio idEstagio = a_estagio_acoplamento_pre_estudo; idEstagio <= a_estagio_final; idEstagio++) {


		//*****************************
		//2. Algoritmo Nested Distance
		//*****************************

		int iteracao = 0;
		double nested_distance_criterio_parada = getdoubleFromChar("max");

		//O algoritmo para quando a ND aumenta, portanto o a_residuo_espaco_amostral_reduzido é obtido na iteração anterior (ite-1)
		std::vector<std::vector<std::vector<double>>> residuo_espaco_amostral_original;
		std::vector<std::vector<std::vector<double>>> residuo_espaco_amostral_reduzido_algoritmo = a_residuo_espaco_amostral_reduzido;			
		std::vector<std::vector<double>> probabilidade_reduzido_algoritmo = a_probabilidade_reduzido;

		SmartEnupla<IdEstagio, double> nested_distance(IdEstagio(a_estagio_acoplamento_pre_estudo), std::vector<double>(a_estagio_final, 0.0));


		for (IdEstagio idEstagio = a_estagio_acoplamento_pre_estudo; idEstagio <= a_estagio_final; idEstagio++) {

			const IdRealizacao maiorIdRealizacao_original = getIterador2Final(AttMatrizProcessoEstocastico_probabilidade_realizacao, a_horizonte_otimizacao.getElemento(idEstagio), IdRealizacao());

			std::vector<std::vector<double>> residuo_espaco_amostral_original_auxiliar(int(maiorIdRealizacao_original), std::vector<double>(int(maiorIdVariavelAleatoria), getdoubleFromChar("max")));

			for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao_original; idRealizacao++) {

				for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= maiorIdVariavelAleatoria; idVariavelAleatoria++)
					residuo_espaco_amostral_original_auxiliar.at(int(idRealizacao-1)).at(int(idVariavelAleatoria-1)) = getElementoMatriz(idVariavelAleatoria, AttMatrizVariavelAleatoria_residuo_espaco_amostral, a_horizonte_otimizacao.getElemento(idEstagio), idRealizacao, double());

			}//for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao_original; idRealizacao++) {

			residuo_espaco_amostral_original.push_back(residuo_espaco_amostral_original_auxiliar);

		}//for (IdEstagio idEstagio = a_estagio_acoplamento_pre_estudo; idEstagio <= a_estagio_final; idEstagio++) {


		while (true) {

			for (IdEstagio idEstagio = a_estagio_final; idEstagio >= a_estagio_acoplamento_pre_estudo; idEstagio--) {

				const Periodo periodo = a_horizonte_otimizacao.getElemento(idEstagio);
				const IdRealizacao maiorIdRealizacao_original = getIterador2Final(AttMatrizProcessoEstocastico_probabilidade_realizacao, periodo, IdRealizacao());

				//Atualização dos limites das probabilidades da árvore reduzida

				if (iteracao == 0) {

					//Na iteração inicial calcula a nested distance da árvore reduzida inicial

					for (int num = 0; num < a_numero_aberturas.getElemento(idEstagio); num++) {

						const int posVar = var_probabilidades_arvore_reduzida_estagio.getElemento(idEstagio) + num;
						vetorPtrSolver.at(idEstagio)->setLimInferior(posVar, std::pow(a_numero_aberturas.getElemento(idEstagio), -1));
						vetorPtrSolver.at(idEstagio)->setLimSuperior(posVar, std::pow(a_numero_aberturas.getElemento(idEstagio), -1));

					}//for (int num = 0; num < a_numero_aberturas.getElemento(idEstagio); num++) {

				}//if (iteracao == 0) {
				else if(iteracao == 1){

					for (int num = 0; num < a_numero_aberturas.getElemento(idEstagio); num++) {

						const int posVar = var_probabilidades_arvore_reduzida_estagio.getElemento(idEstagio) + num;
						vetorPtrSolver.at(idEstagio)->setLimInferior(posVar, 0.0);
						vetorPtrSolver.at(idEstagio)->setLimSuperior(posVar, 1.0);

					}//for (int num = 0; num < a_numero_aberturas.getElemento(idEstagio); num++) {

				}//else if(iteracao == 1){

				//Atualização dos coeficientes da função objetivo (com base na norma_2)

				//Nota: por questões númericas é conveniente realizar uma normalização das distâncias entre realizações na função objetivo
				//O fator normalizador vai ser cálculado respeito às idRealizacao_1 da amostra_original e da amostra_reduzida

				double fator_normalizador = 0.0;

				for (IdRealizacao idRealizacao_original = IdRealizacao_1; idRealizacao_original <= maiorIdRealizacao_original; idRealizacao_original++) {

					bool realizacoes_diferentes = false;

					for (int num = 0; num < a_numero_aberturas.getElemento(idEstagio); num++) {

						fator_normalizador = calcularNorma2(residuo_espaco_amostral_original.at(int(idEstagio - a_estagio_acoplamento_pre_estudo)).at(int(idRealizacao_original - 1)), residuo_espaco_amostral_reduzido_algoritmo.at(int(idEstagio - a_estagio_acoplamento_pre_estudo)).at(num));

						if (fator_normalizador != 0.0) {
							realizacoes_diferentes = true;
							break;
						}//if (fator_normalizador != 0.0) {

					}//for (int num = 0; num < a_numero_aberturas.getElemento(idEstagio); num++) {

					if (realizacoes_diferentes)
						break;

				}//for (IdRealizacao idRealizacao_original = IdRealizacao_1; idRealizacao_original <= maiorIdRealizacao_original; idRealizacao_original++) {

				if (idEstagio < a_estagio_final)
					fator_normalizador += nested_distance.getElemento(IdEstagio(idEstagio + 1));

				if (fator_normalizador == 0.0)
					throw std::invalid_argument("Fator normalizador igual a zero");

				for (IdRealizacao idRealizacao_original = IdRealizacao_1; idRealizacao_original <= maiorIdRealizacao_original; idRealizacao_original++) {

					for (int num = 0; num < a_numero_aberturas.getElemento(idEstagio); num++) {

						const int posVar = var_matriz_transporte_estagio.at(idEstagio).getElemento(idRealizacao_original) + num;
						double norma_2 = calcularNorma2(residuo_espaco_amostral_original.at(int(idEstagio-a_estagio_acoplamento_pre_estudo)).at(int(idRealizacao_original-1)), residuo_espaco_amostral_reduzido_algoritmo.at(int(idEstagio - a_estagio_acoplamento_pre_estudo)).at(num));
												
						if (idEstagio < a_estagio_final)
							norma_2 += nested_distance.getElemento(IdEstagio(idEstagio + 1));

						vetorPtrSolver.at(idEstagio)->setCofObjetivo(posVar, norma_2*std::pow(fator_normalizador, -1));

					}//for (int num = 0; num < a_numero_aberturas.getElemento(idEstagio); num++) {

				}//for (IdRealizacao idRealizacao_original = IdRealizacao_1; idRealizacao_original <= maiorIdRealizacao_original; idRealizacao_original++) {

				//Soluciona o problema

				//vetorPtrSolver.at(idEstagio)->imprimirLP(getString(idEstagio) + "_" + getString(iteracao));
				vetorPtrSolver.at(idEstagio)->otimizar();

				nested_distance.setElemento(idEstagio, vetorPtrSolver.at(idEstagio)->getValorObjetivo()* fator_normalizador);
		
			}//for (IdEstagio idEstagio = a_estagio_final; idEstagio >= a_estagio_acoplamento_pre_estudo; idEstagio--) {


			///////////////////////////////////////
			//Critério de parada
			///////////////////////////////////////
			
			if (a_idProcesso == IdProcesso_mestre) {

				if(iteracao == 0)
					valores_nested_distance.push_back(std::vector<string>(1, std::string("Iteracao") + ";" + std::string("Nested_distance")));

				valores_nested_distance.push_back(std::vector<string>(1, getString(iteracao+1) + ";" + getString(nested_distance.getElemento(a_estagio_acoplamento_pre_estudo))));

			}//if (a_idProcesso == IdProcesso_mestre) {

			if (nested_distance_criterio_parada < nested_distance.getElemento(a_estagio_acoplamento_pre_estudo) + 0.01)
				break;

			nested_distance_criterio_parada = nested_distance.getElemento(a_estagio_acoplamento_pre_estudo);
			iteracao++;

			//Atualiza amostra reduzida
			a_residuo_espaco_amostral_reduzido = residuo_espaco_amostral_reduzido_algoritmo;
			a_probabilidade_reduzido           = probabilidade_reduzido_algoritmo;

			///////////////////////////////////////////////////////////////////////
			//Atualiza nova amostra reduzida para o cálculo da nested distance
			///////////////////////////////////////////////////////////////////////

			for (IdEstagio idEstagio = a_estagio_acoplamento_pre_estudo; idEstagio <= a_estagio_final; idEstagio++) {

				const Periodo periodo = a_horizonte_otimizacao.getElemento(idEstagio);
				const IdRealizacao maiorIdRealizacao_original = getIterador2Final(AttMatrizProcessoEstocastico_probabilidade_realizacao, periodo, IdRealizacao());

				//Atualiza as probabilidades

				for (int num = 0; num < a_numero_aberturas.getElemento(idEstagio); num++) {

					const int posVar = var_probabilidades_arvore_reduzida_estagio.at(idEstagio) + num;
					probabilidade_reduzido_algoritmo.at(int(idEstagio- a_estagio_acoplamento_pre_estudo)).at(num) = vetorPtrSolver.at(idEstagio)->getValorPrimal(posVar);

				}//for (int num = 0; num < a_numero_aberturas.getElemento(idEstagio); num++) {

				//Atualiza as realizações: ver (Beltrán et al, 2017) equação (9)

				residuo_espaco_amostral_reduzido_algoritmo.at(int(idEstagio- a_estagio_acoplamento_pre_estudo)) = std::vector<std::vector<double>> (a_numero_aberturas.getElemento(idEstagio), std::vector<double>(int(maiorIdVariavelAleatoria), 0.0));

				for (int num = 0; num < a_numero_aberturas.getElemento(idEstagio); num++) {

					double somatorio_denominador = 0;

					for (IdRealizacao idRealizacao_original = IdRealizacao_1; idRealizacao_original <= maiorIdRealizacao_original; idRealizacao_original++) {
						const int posVar = var_matriz_transporte_estagio.at(idEstagio).getElemento(idRealizacao_original) + num;
						somatorio_denominador += vetorPtrSolver.at(idEstagio)->getValorPrimal(posVar);
					}//for (IdRealizacao idRealizacao_original = IdRealizacao_1; idRealizacao_original <= maiorIdRealizacao_original; idRealizacao_original++) {
						
					for (IdRealizacao idRealizacao_original = IdRealizacao_1; idRealizacao_original <= maiorIdRealizacao_original; idRealizacao_original++) {
						
						const int posVar = var_matriz_transporte_estagio.at(idEstagio).getElemento(idRealizacao_original) + num;
						const double fator_parcela = vetorPtrSolver.at(idEstagio)->getValorPrimal(posVar) / somatorio_denominador;

						for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= maiorIdVariavelAleatoria; idVariavelAleatoria++)
							residuo_espaco_amostral_reduzido_algoritmo.at(int(idEstagio - a_estagio_acoplamento_pre_estudo)).at(num).at(int(idVariavelAleatoria - 1)) += fator_parcela * residuo_espaco_amostral_original.at(int(idEstagio - a_estagio_acoplamento_pre_estudo)).at(int(idRealizacao_original - 1)).at(int(idVariavelAleatoria - 1));

					}//for (IdRealizacao idRealizacao_original = IdRealizacao_1; idRealizacao_original <= maiorIdRealizacao_original; idRealizacao_original++) {

				}//for (int num = 0; num < a_numero_aberturas.getElemento(idEstagio); num++) {

			}//for (IdEstagio idEstagio = a_estagio_acoplamento_pre_estudo; idEstagio <= a_estagio_final; idEstagio++) {
			
		}//while (true) {

		if (a_idProcesso == IdProcesso_mestre)
			a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla("algoritmo_nested_distance.csv", valores_nested_distance);

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::reducao_adaptacao_nested_distance(" + "): \n" + std::string(erro.what())); }

}


Solver* ProcessoEstocastico::instanciarSolver(TipoSolver& a_tipoSolver)
{

	try {

#ifdef GRB

		if (a_tipoSolver == TipoSolver_gurobi) {
			try {
				return new SolverGRB();

			}
			catch (...) {
				std::cout << "Nao foi possivel instanciar modelo com " + getString(TipoSolver_gurobi) << ", alterando para " + getString(TipoSolver_clp) << std::endl;
				a_tipoSolver = TipoSolver_clp;
			}
		}//if (a_tipoSolver == TipoSolver_gurobi) {

#endif

		if (a_tipoSolver == TipoSolver_clp)
			return new SolverCLP();
			
		throw std::invalid_argument("Tipo de Solver Invalido");

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::instanciarSolver(" + "): \n" + std::string(erro.what())); }

}

double ProcessoEstocastico::calcularNorma2(const std::vector<double> &a_vector_1, const std::vector<double> &a_vector_2)
{

	try {

		if(int(a_vector_1.size()) != int(a_vector_1.size()))
			throw std::invalid_argument("Tamanho dos vetores deve ser igual");

		double norma2 = 0;

		for (int pos = 0; pos < int(a_vector_1.size()); pos++)
			norma2 += std::pow(a_vector_1.at(pos) - a_vector_2.at(pos),2);

		return norma2;

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::calcularNorma2(" + "): \n" + std::string(erro.what())); }

}

