#ifndef PROCESSO_ESTOCASTICO
#define PROCESSO_ESTOCASTICO

#include "C_SmartElemento.h"

#include "C_VariavelAleatoria.h"

#include "C_Solver.h"

#define ATT_COMUM_PROCESSO_ESTOCASTICO(m)  \
	  m(ProcessoEstocastico,  AttComum,                   idProcessoEstocastico,               IdProcessoEstocastico,         min,         max,           min,      nao) \
	  m(ProcessoEstocastico,  AttComum,                    tipo_espaco_amostral,                  TipoEspacoAmostral,         min,         max,           min,      nao) \
	  m(ProcessoEstocastico,  AttComum,    tipo_correlacao_variaveis_aleatorias,   TipoCorrelacaoVariaveisAleatorias,         min,         max,           min,      nao) 
//                 c_classe,   smrtAtt,                            nomeAtributo,                                tipo,  lowerBound,  upperBound,  initialValue, mustRead?


#define ATT_VETOR_PROCESSO_ESTOCASTICO(m)  \
	  m(ProcessoEstocastico,  AttVetor,     mapeamento_tendencia_temporal,     IdCenario,        min,          max,           min,  IdCenario) 


#define ATT_MATRIZ_PROCESSO_ESTOCASTICO(m)  \
	  m(ProcessoEstocastico,  AttMatriz,      mapeamento_espaco_amostral, IdRealizacao,          min,          max,             1,  IdCenario,      Periodo)  \
	  m(ProcessoEstocastico,  AttMatriz,        probabilidade_realizacao,       double,          min,          max,             1,  Periodo,   IdRealizacao)  \
      m(ProcessoEstocastico,  AttMatriz,      mapeamento_arvore_cenarios,         IdNo,          min,          max,             1,  IdCenario,      Periodo)  \
	  m(ProcessoEstocastico,  AttMatriz,                   no_realizacao, IdRealizacao,          min,          max,             1,  Periodo,           IdNo)  \
      m(ProcessoEstocastico,  AttMatriz,                   no_antecessor,         IdNo,          min,          max,             1,  Periodo,           IdNo)  \
      m(ProcessoEstocastico,  AttMatriz,                no_probabilidade,       double,          min,          max,             1,  Periodo,           IdNo)  \
	  m(ProcessoEstocastico,  AttMatriz,    variavelAleatoria_realizacao,       double,          min,          max,             1,  IdRealizacao,  IdVariavelAleatoria)
//                 c_classe,    smrtAtt,                    nomeAtributo,         Tipo,   lowerBound,   upperBound,  initialValue,  TipoIterador

#define MEMBRO_PROCESSO_ESTOCASTICO(m)           \
	m(ProcessoEstocastico, VariavelAleatoria)

#define SMART_ELEMENTO_PROCESSO_ESTOCASTICO(m) \
	m(ProcessoEstocastico,  AttComum, ATT_COMUM_PROCESSO_ESTOCASTICO) \
	m(ProcessoEstocastico,  AttVetor, ATT_VETOR_PROCESSO_ESTOCASTICO) \
	m(ProcessoEstocastico, AttMatriz, ATT_MATRIZ_PROCESSO_ESTOCASTICO) \
	m(ProcessoEstocastico,   Membro,    MEMBRO_PROCESSO_ESTOCASTICO) 

DEFINE_SMART_ELEMENTO(ProcessoEstocastico, SMART_ELEMENTO_PROCESSO_ESTOCASTICO)

class EntradaSaidaDados;

class ProcessoEstocastico : public SmartDados {
public:

	ProcessoEstocastico();
	ProcessoEstocastico(const ProcessoEstocastico &instanciaCopiar);
	ProcessoEstocastico& operator=(const ProcessoEstocastico& instanciaCopiar);
	void esvaziar();
	virtual ~ProcessoEstocastico();


	DECLARA_SMART_ELEMENTO(ProcessoEstocastico, SMART_ELEMENTO_PROCESSO_ESTOCASTICO)

	template<typename TipoVariavelAleatoria, typename TipoVariavelAleatoriaInterna>
	void addSeriesTemporais(const TipoVariavelAleatoria a_tipo_variavel_aleatoria, const SmartEnupla<int, TipoVariavelAleatoriaInterna> a_lista_variavel_aleatoria_interna, const SmartEnupla<int, SmartEnupla<Periodo, double>>& a_series_temporais);

	template<typename TipoVariavelAleatoria, typename TipoVariavelAleatoriaInterna>
	void addTendenciasTemporais(const TipoVariavelAleatoria a_tipo_variavel_aleatoria, const SmartEnupla<int, TipoVariavelAleatoriaInterna> a_lista_tipo_variavel_aleatoria_interna, const SmartEnupla<int, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>>& a_tendencias_temporais);

	void gerarTendenciaTemporalMedia(const Periodo a_periodo_final);

	void parametrizarModelo(const EntradaSaidaDados &a_entradaSaidaDados, const bool a_imprimir_parametros, const TipoModeloGeracaoSinteticaCenario a_tipo_modelo_geracao_sintetica, const TipoValor a_tipo_coeficiente_auto_correlacao, const int a_ordem_coeficiente_auto_correlacao, const TipoCorrelacaoVariaveisAleatorias a_tipo_correlacao_variaveis_aleatorias);

	void avaliarModeloViaSerieSintetica(const EntradaSaidaDados &a_entradaSaidaDados, const SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>>& a_horizonte_espaco_amostral, const TipoSorteio a_tipo_sorteio, const int a_numero_periodos_avaliacao_sintetica);

	void gerarEspacoAmostralPorSorteio(const EntradaSaidaDados &a_entradaSaidaDados, const bool a_imprimir_amostra, const SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> &a_horizonte_espaco_amostral, const TipoSorteio a_tipo_sorteio, int &a_semente);
	
	void mapearCenariosTendencia(const int a_numero_cenarios, const int a_numero_cenarios_tendencia);

	void validar_probabilidade_realizacao();

	void gerarCenariosPorSorteio(const EntradaSaidaDados &a_entradaSaidaDados, const bool a_imprimir_cenarios, const bool a_gerar_cenarios_buffer, const bool a_gerar_cenarios_internos, const int a_numero_cenarios_tendencia, const int a_numero_cenarios_global, const IdCenario a_cenario_inicial, const IdCenario a_cenario_final, const TipoSorteio a_tipo_sorteio, int &a_semente);
	SmartEnupla<IdVariavelAleatoria, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>> gerarCenariosPorSorteioRetorno(const EntradaSaidaDados &a_entradaSaidaDados, const bool a_imprimir_cenarios, const bool a_gerar_cenarios_buffer, const bool a_gerar_cenarios_internos, const int a_numero_cenarios_tendencia, const int a_numero_cenarios_global, const IdCenario a_cenario_inicial, const IdCenario a_cenario_final, const TipoSorteio a_tipo_sorteio, int &a_semente);

	void setCenariosMapeamento(                                                 const AttMatrizProcessoEstocastico           a_attMatrizProcessoEstocastico,                                         const SmartEnupla<IdCenario, SmartEnupla<Periodo, IdRealizacao>>  &a_matriz);
	void setCenarios          (                                                 const AttMatrizVariavelAleatoria               a_attMatrizVariavelAleatoria, const SmartEnupla<IdVariavelAleatoria,        SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>> &a_matriz);
	void setCenariosInternos  (const IdVariavelAleatoria a_idVariavelAleatoria, const AttMatrizVariavelAleatoriaInterna a_attMatrizVariavelAleatoriaInterna, const SmartEnupla<IdVariavelAleatoriaInterna, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>> &a_matriz);

	                                        SmartEnupla<IdCenario, SmartEnupla<Periodo, IdRealizacao>>  getCenariosMapeamento(const AttMatrizProcessoEstocastico a_attMatrizProcessoEstocastico, const IdCenario a_cenarioInicial, const IdCenario a_cenarioFinal);
	SmartEnupla<IdVariavelAleatoria,        SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>> getCenarios          (const AttMatrizVariavelAleatoria a_attMatrizVariavelAleatoria);
	SmartEnupla<IdVariavelAleatoriaInterna, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>> getCenariosInternos  (const IdVariavelAleatoria a_idVariavelAleatoria, const AttMatrizVariavelAleatoriaInterna a_attMatrizVariavelAleatoriaInterna);
	SmartEnupla<IdVariavelAleatoriaInterna, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>> getCenariosInternos  (const IdVariavelAleatoria a_idVariavelAleatoria, const AttMatrizVariavelAleatoriaInterna a_attMatrizVariavelAleatoriaInterna, const IdCenario a_cenarioInicial, const IdCenario a_cenarioFinal);

	double calcularRealizacaoInterna(const IdVariavelAleatoria a_idVariavelAleatoria, const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const Periodo a_periodo, const double a_realizacao);
	double calcularRealizacaoInterna(const IdVariavelAleatoria a_idVariavelAleatoria, const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const IdCenario a_idCenario, const Periodo a_periodo);
	double calcularRealizacaoInterna(const IdVariavelAleatoria a_idVariavelAleatoria, const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const Periodo a_periodo);

	double calcularRealizacao(const IdVariavelAleatoria a_idVariavelAleatoria, const IdRealizacao a_idRealizacao, const Periodo a_periodo, const SmartEnupla<Periodo, double> &a_tendencia);
	double calcularRealizacao(const IdVariavelAleatoria a_idVariavelAleatoria, const IdCenario a_idCenario, const Periodo a_periodo);
	double calcularRealizacao(const IdVariavelAleatoria a_idVariavelAleatoria, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const Periodo a_periodo);

	double calcularRealizacaoParaValor(const IdVariavelAleatoria a_idVariavelAleatoria, const double a_valor, const Periodo a_periodo);

	void imprimirEspacoAmostral(EntradaSaidaDados a_entradaSaidaDados);

	void imprimirCenarios(EntradaSaidaDados a_entradaSaidaDados);

	void imprimirParametros(EntradaSaidaDados a_entradaSaidaDados);

	int getMaiorOrdemAutocorrelacaoLinear(const Periodo a_periodo);

	void reducao_espaco_amostral(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_reducao_cenarios, IdProcesso a_idProcesso, const SmartEnupla <IdEstagio, int> a_numero_aberturas, const SmartEnupla <IdEstagio, Periodo> a_horizonte_otimizacao, const IdEstagio a_estagio_acoplamento_pre_estudo, const IdEstagio a_estagio_final, TipoSolver& a_tipoSolver);
	
	void reducao_inicial(EntradaSaidaDados a_entradaSaidaDados, std::vector<std::vector<std::vector<double>>> &a_residuo_espaco_amostral_reduzido, const SmartEnupla <IdEstagio, int> a_numero_aberturas, const SmartEnupla <IdEstagio, Periodo> a_horizonte_otimizacao, const IdEstagio a_estagio_acoplamento_pre_estudo, const IdEstagio a_estagio_final);
	
	void reducao_adaptacao_nested_distance(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_reducao_cenarios, IdProcesso a_idProcesso, std::vector<std::vector<std::vector<double>>>&a_residuo_espaco_amostral_reduzido, std::vector<std::vector<double>>& a_probabilidade_reduzido, const SmartEnupla <IdEstagio, int> a_numero_aberturas, const SmartEnupla <IdEstagio, Periodo> a_horizonte_otimizacao, const IdEstagio a_estagio_acoplamento_pre_estudo, const IdEstagio a_estagio_final, TipoSolver& a_tipoSolver);

	void mapearTipoPeriodoEspacoAmostral();
	std::vector<TipoPeriodo> getTipoPeriodoEspacoAmostral();

private:

	std::vector<TipoPeriodo> tipo_periodo_espaco_amostral;

	void mapearCenariosEspacoAmostralPorSorteio  (const TipoSorteio a_tipo_sorteio, const int a_numero_cenarios_tendencia, const int a_numero_cenarios_global, const IdCenario a_menorIdcenario, const IdCenario a_maiorIdcenario, int &a_semente);
	bool mapearCenariosEspacoAmostralCompleto    (const int a_numero_cenarios_tendencia, const int a_numero_cenarios_global, const IdCenario a_menorIdcenario, const IdCenario a_maiorIdcenario);

	void calcularCorrelacaoSazonalVariaveisAleatorias();

	void calcularCorrelacaoSazonalResiduoVariaveisAleatorias();

	void calcularMatrizCargaSazonalResiduoVariaveisAleatorias();

	void calcularRuidoCorrelacionadoEspacoAmostral();

	double calcularNorma2(const std::vector<double>& a_vector_1, const std::vector<double>& a_vector_2);
	
	IdVariavelAleatoria getMaiorIdVariavelAleatoria();

	Solver* instanciarSolver(TipoSolver& a_tipoSolver);

};

GET_STRING_FROM_CLASS(ProcessoEstocastico)


template<typename TipoVariavelAleatoria, typename TipoVariavelAleatoriaInterna>
inline void ProcessoEstocastico::addSeriesTemporais(const TipoVariavelAleatoria a_tipo_variavel_aleatoria, const SmartEnupla<int, TipoVariavelAleatoriaInterna> a_lista_tipo_variavel_aleatoria_interna, const SmartEnupla<int, SmartEnupla<Periodo, double>>& a_series_temporais){

	try {

		if (a_series_temporais.size() == 0)
			throw std::invalid_argument("Nao ha valores de series temporais a serem adicionadas.");

		if (a_lista_tipo_variavel_aleatoria_interna.size() != a_series_temporais.size())
			throw std::invalid_argument("O tamanho da lista de variaveis aleatorias internas deve ser compativel com o numero de series temporais.");

		if (a_lista_tipo_variavel_aleatoria_interna.getIteradorInicial() != a_series_temporais.getIteradorInicial())
			throw std::invalid_argument("O iterador inicial da lista de variaveis aleatorias internas deve ser compativel com o iterador inicial das series temporais.");

		if (a_lista_tipo_variavel_aleatoria_interna.getIteradorInicial() != 1)
			throw std::invalid_argument("O iterador inicial da lista de variaveis aleatorias internas deve ser 1.");

		const IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria(a_tipo_variavel_aleatoria);

		if (vetorVariavelAleatoria.isInstanciado(idVariavelAleatoria))
			throw std::invalid_argument("Variavel aleatoria ja instanciada " + getFullString(idVariavelAleatoria) + " " + getAtributo(idVariavelAleatoria, AttComumVariavelAleatoria_nome, std::string()));

		else {
			VariavelAleatoria variavelAleatoria;
			variavelAleatoria.setAtributo(AttComumVariavelAleatoria_idVariavelAleatoria, idVariavelAleatoria);
			variavelAleatoria.setAtributo(AttComumVariavelAleatoria_nome, getFullString(a_tipo_variavel_aleatoria));
			vetorVariavelAleatoria.add(variavelAleatoria);
		}	

		for (int iter = 1; iter <= a_lista_tipo_variavel_aleatoria_interna.getIteradorFinal(); iter++) {
			const IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna(iter);

			if (vetorVariavelAleatoria.att(idVariavelAleatoria).vetorVariavelAleatoriaInterna.isInstanciado(idVariavelAleatoriaInterna))
				throw std::invalid_argument("Variavel aleatoria interna ja instanciada " + getFullString(idVariavelAleatoriaInterna) + " " + getAtributo(idVariavelAleatoria, idVariavelAleatoriaInterna, AttComumVariavelAleatoriaInterna_nome, std::string()));

			VariavelAleatoriaInterna variavelAleatoriaInterna;
			variavelAleatoriaInterna.setAtributo(AttComumVariavelAleatoriaInterna_idVariavelAleatoriaInterna, idVariavelAleatoriaInterna);
			variavelAleatoriaInterna.setAtributo(AttComumVariavelAleatoriaInterna_nome, getFullString(a_lista_tipo_variavel_aleatoria_interna.getElemento(iter)));

			vetorVariavelAleatoria.att(idVariavelAleatoria).vetorVariavelAleatoriaInterna.add(variavelAleatoriaInterna);

			vetorVariavelAleatoria.att(idVariavelAleatoria).addSerieTemporalVariavelAleatoriaInterna(idVariavelAleatoriaInterna, a_series_temporais.at(iter));

		} // for (int iter = 1; iter <= a_lista_variavel_aleatoria_interna.getIteradorFinal(); iter++) {

		vetorVariavelAleatoria.att(idVariavelAleatoria).calcularSerieTemporal();

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::addSeriesTemporais(" + getFullString(a_tipo_variavel_aleatoria) + ",a_lista_tipo_variavel_aleatoria_interna,a_series_temporais): \n" + std::string(erro.what())); }

} // inline void ProcessoEstocastico::addSeriesTemporais(const TipoVariavelAleatoria a_tipo_variavel_aleatoria, const SmartEnupla<int, TipoVariavelAleatoriaInterna> a_lista_tipo_variavel_aleatoria_interna, const SmartEnupla<int, SmartEnupla<Periodo, double>>& a_series_temporais){


template<typename TipoVariavelAleatoria, typename TipoVariavelAleatoriaInterna>
inline void ProcessoEstocastico::addTendenciasTemporais(const TipoVariavelAleatoria a_tipo_variavel_aleatoria, const SmartEnupla<int, TipoVariavelAleatoriaInterna> a_lista_tipo_variavel_aleatoria_interna, const SmartEnupla<int, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>>& a_tendencias_temporais){

	try {

		if (a_tendencias_temporais.size() == 0)
			throw std::invalid_argument("Nao ha valores de tendencias temporais a serem adicionadas.");


		if (a_lista_tipo_variavel_aleatoria_interna.size() != a_tendencias_temporais.size())
			throw std::invalid_argument("O tamanho da lista de variaveis aleatorias internas deve ser compativel com o numero de series temporais.");

		if (a_lista_tipo_variavel_aleatoria_interna.getIteradorInicial() != a_tendencias_temporais.getIteradorInicial())
			throw std::invalid_argument("O iterador inicial da lista de variaveis aleatorias internas deve ser compativel com o iterador inicial das series temporais.");

		if (a_lista_tipo_variavel_aleatoria_interna.getIteradorInicial() != 1)
			throw std::invalid_argument("O iterador inicial da lista de variaveis aleatorias internas deve ser 1.");

		const IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria(a_tipo_variavel_aleatoria);

		if (!vetorVariavelAleatoria.isInstanciado(idVariavelAleatoria))
			throw std::invalid_argument("Variavel aleatoria ainda nao instanciada " + getFullString(idVariavelAleatoria));


		for (int iter = 1; iter <= a_lista_tipo_variavel_aleatoria_interna.getIteradorFinal(); iter++) {
			const IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna(iter);

			if (!vetorVariavelAleatoria.att(idVariavelAleatoria).vetorVariavelAleatoriaInterna.isInstanciado(idVariavelAleatoriaInterna))
				throw std::invalid_argument("Variavel aleatoria interna nao instanciada " + getFullString(idVariavelAleatoriaInterna));

			vetorVariavelAleatoria.att(idVariavelAleatoria).addTendenciaTemporalVariavelAleatoriaInterna(idVariavelAleatoriaInterna, a_tendencias_temporais.at(iter));

		} // for (int iter = 1; iter <= a_lista_tipo_variavel_aleatoria_interna.getIteradorFinal(); iter++) {

		vetorVariavelAleatoria.att(idVariavelAleatoria).calcularTendenciaTemporal();

	} // try{
	catch (const std::exception&erro) { throw std::invalid_argument("ProcessoEstocastico(" + getString(getIdObjeto()) + ")::addTendenciasTemporais(" + getFullString(a_tipo_variavel_aleatoria) + ",a_lista_tipo_variavel_aleatoria_interna,a_tendencias_temporais): \n" + std::string(erro.what())); }

} // inline void ProcessoEstocastico::addTendenciasTemporais(const TipoVariavelAleatoria a_tipo_variavel_aleatoria, const SmartEnupla<int, TipoVariavelAleatoriaInterna> a_lista_tipo_variavel_aleatoria_interna, const SmartEnupla<int, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>>& a_tendencias_temporais){

#endif 