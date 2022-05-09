#ifndef VARIAVEL_ALEATORIA
#define VARIAVEL_ALEATORIA


#include "C_SmartElemento.h"

#include "C_VariavelAleatoriaInterna.h"

#define ATT_COMUM_VARIAVEL_ALEATORIA(m)  \
	  m(VariavelAleatoria,  AttComum,                        idVariavelAleatoria,               IdVariavelAleatoria,         min,         max,           min,      nao) \
	  m(VariavelAleatoria,  AttComum,                                       nome,                            string,         min,         max,        Nenhum,      nao) \
	  m(VariavelAleatoria,  AttComum,           grau_liberdade_residuo_lognormal,                            double,           0,         max,             0,      nao) \
	  m(VariavelAleatoria,  AttComum,              tipo_modelo_geracao_sintetica, TipoModeloGeracaoSinteticaCenario,      nenhum,         max,           min,      nao) \
	  m(VariavelAleatoria,  AttComum,           tipo_coeficiente_auto_correlacao,                         TipoValor,         min,         max,           min,      nao) \
	  m(VariavelAleatoria,  AttComum,   ordem_maxima_coeficiente_auto_correlacao,                               int,           0,         365,             0,      nao) 
//               c_classe,   smrtAtt,                               nomeAtributo,                              tipo,  lowerBound,  upperBound,  initialValue, mustRead?

#define ATT_VETOR_VARIAVEL_ALEATORIA(m)  \
	  m(VariavelAleatoria,  AttVetor,                                serie_temporal,   double,        min,          max,             1,  Periodo)   \
	  m(VariavelAleatoria,  AttVetor,                   serie_temporal_transformada,   double,        min,          max,             1,  Periodo)   \
	  m(VariavelAleatoria,  AttVetor,                                 serie_residuo,   double,        min,          max,             1,  Periodo)   \
	  m(VariavelAleatoria,  AttVetor,                          serie_residuo_normal,   double,          0,          max,             0,  Periodo)   \
	  m(VariavelAleatoria,  AttVetor,                       serie_residuo_lognormal,   double,          1,          max,             1,  Periodo)   \
	  m(VariavelAleatoria,  AttVetor,                          media_serie_temporal,   double,        min,          max,             1,  IdEstacao) \
	  m(VariavelAleatoria,  AttVetor,                      media_serie_transformada,   double,          1,          max,             1,  IdEstacao) \
	  m(VariavelAleatoria,  AttVetor,                          media_residuo_normal,   double,          0,          max,             1,  IdEstacao) \
	  m(VariavelAleatoria,  AttVetor,                       media_residuo_lognormal,   double,          1,          max,             1,  IdEstacao) \
	  m(VariavelAleatoria,  AttVetor,                         desvio_serie_temporal,   double,          0,          max,             1,  IdEstacao) \
	  m(VariavelAleatoria,  AttVetor,                     desvio_serie_transformada,   double,          0,          max,             1,  IdEstacao) \
	  m(VariavelAleatoria,  AttVetor,                         desvio_residuo_normal,   double,          0,          max,             1,  IdEstacao) \
	  m(VariavelAleatoria,  AttVetor,                      desvio_residuo_lognormal,   double,          0,          max,             1,  IdEstacao) \
	  m(VariavelAleatoria,  AttVetor,                     assimetria_serie_temporal,   double,          0,          max,             0,  IdEstacao) \
	  m(VariavelAleatoria,  AttVetor,                 assimetria_serie_transformada,   double,          0,          max,             0,  IdEstacao) \
	  m(VariavelAleatoria,  AttVetor,                  assimetria_residuo_lognormal,   double,          0,          max,             0,  IdEstacao) \
	  m(VariavelAleatoria,  AttVetor,                         ordem_auto_correlacao,      int,          0,          365,             0,  IdEstacao) \
	  m(VariavelAleatoria,  AttVetor,                            ic_auto_correlacao,   double,          0,          max,             0,  IdEstacao) \
	  m(VariavelAleatoria,  AttVetor,                          sigma_residuo_normal,   double,          0,          max,             0,  IdEstacao) \
	  m(VariavelAleatoria,  AttVetor,                             mi_residuo_normal,   double,        min,          max,             0,  IdEstacao) \
	  m(VariavelAleatoria,  AttVetor,                          delta_residuo_normal,   double,        min,          max,             0,  IdEstacao) \
	  m(VariavelAleatoria,  AttVetor,                         phi_residuo_lognormal,   double,        min,          max,             0,  IdEstacao) \
	  m(VariavelAleatoria,  AttVetor,                              auto_covariancia,   double,        min,          max,             0,  int) \
	  m(VariavelAleatoria,  AttVetor,                               auto_correlacao,   double,        min,          max,             0,  int) \
	  m(VariavelAleatoria,  AttVetor,                       auto_correlacao_residuo,   double,        min,          max,             0,  int) \
	  m(VariavelAleatoria,  AttVetor,                auto_correlacao_residuo_normal,   double,        min,          max,             0,  int) \
	  m(VariavelAleatoria,  AttVetor,             auto_correlacao_residuo_lognormal,   double,        min,          max,             0,  int) \
	  m(VariavelAleatoria,  AttVetor,                  ruido_branco_serie_sintetica,   double,        min,          max,             0,  Periodo) \
	  m(VariavelAleatoria,  AttVetor,          ruido_correlacionado_serie_sintetica,   double,        min,          max,             0,  Periodo)
//               c_classe,   smrtAtt,                                  nomeAtributo,     Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador

#define ATT_MATRIZ_VARIAVEL_ALEATORIA(m)  \
	  m(VariavelAleatoria,  AttMatriz,                               tendencia_temporal,       double,          min,          max,             1,  IdCenario,      Periodo)   \
	  m(VariavelAleatoria,  AttMatriz,                  tendencia_temporal_transformada,       double,          min,          max,             1,  IdCenario,      Periodo)   \
	  m(VariavelAleatoria,  AttMatriz, cenarios_realizacao_transformada_espaco_amostral,       double,          min,          max,             1,  IdCenario,      Periodo)   \
	  m(VariavelAleatoria,  AttMatriz,                     ruido_branco_espaco_amostral,       double,          min,          max,             0,    Periodo, IdRealizacao)   \
	  m(VariavelAleatoria,  AttMatriz,             ruido_correlacionado_espaco_amostral,       double,          min,          max,             0,    Periodo, IdRealizacao)   \
	  m(VariavelAleatoria,  AttMatriz,                          residuo_espaco_amostral,       double,          min,          max,             0,    Periodo, IdRealizacao)   \
	  m(VariavelAleatoria,  AttMatriz,               coeficiente_linear_auto_correlacao,       double,          min,          max,             0,    Periodo, int)       \
	  m(VariavelAleatoria,  AttMatriz,                         auto_covariancia_sazonal,       double,          min,          max,             0,  IdEstacao, int)       \
	  m(VariavelAleatoria,  AttMatriz,                                  auto_correlacao,       double,          min,          max,             0,  IdEstacao, int)       \
	  m(VariavelAleatoria,  AttMatriz,                          auto_correlacao_residuo,       double,          min,          max,             0,  IdEstacao, int)       \
	  m(VariavelAleatoria,  AttMatriz,                   auto_correlacao_residuo_normal,       double,          min,          max,             0,  IdEstacao, int)       \
	  m(VariavelAleatoria,  AttMatriz,                auto_correlacao_residuo_lognormal,       double,          min,          max,             0,  IdEstacao, int)       \
	  m(VariavelAleatoria,  AttMatriz,                          auto_correlacao_parcial,       double,          min,          max,             0,  IdEstacao, int)       \
	  m(VariavelAleatoria,  AttMatriz,                      coeficiente_auto_correlacao,       double,          -10,           10,             0,  IdEstacao, int)       \
	  m(VariavelAleatoria,  AttMatriz,       coeficiente_linear_auto_correlacao_sazonal,       double,          -10,           10,             0,  IdEstacao, int)       \
	  m(VariavelAleatoria,  AttMatriz,                                       correlacao,       double,          min,          max,             0,  IdVariavelAleatoria, IdEstacao) \
	  m(VariavelAleatoria,  AttMatriz,                     correlacao_residuo_lognormal,       double,          min,          max,             0,  IdVariavelAleatoria, IdEstacao) \
	  m(VariavelAleatoria,  AttMatriz,                   matriz_carga_residuo_lognormal,       double,          min,          max,             0,  IdEstacao, IdVariavelAleatoria)
//               c_classe,    smrtAtt,                                     nomeAtributo,         Tipo,   lowerBound,   upperBound,  initialValue,  TipoIterador

#define MEMBRO_VARIAVEL_ALEATORIA(m)\
	m(VariavelAleatoria, VariavelAleatoriaInterna)

#define SMART_ELEMENTO_VARIAVEL_ALEATORIA(m) \
	m(VariavelAleatoria,  AttComum,  ATT_COMUM_VARIAVEL_ALEATORIA) \
	m(VariavelAleatoria,  AttVetor,  ATT_VETOR_VARIAVEL_ALEATORIA) \
	m(VariavelAleatoria, AttMatriz, ATT_MATRIZ_VARIAVEL_ALEATORIA) \
	m(VariavelAleatoria,    Membro,     MEMBRO_VARIAVEL_ALEATORIA)


DEFINE_SMART_ELEMENTO(VariavelAleatoria, SMART_ELEMENTO_VARIAVEL_ALEATORIA)

class VariavelAleatoria : public SmartDados {
public:

	VariavelAleatoria();
	VariavelAleatoria(const VariavelAleatoria &instanciaCopiar);
	void esvaziar();
	virtual ~VariavelAleatoria();

	DECLARA_SMART_ELEMENTO(VariavelAleatoria, SMART_ELEMENTO_VARIAVEL_ALEATORIA)

	void addSerieTemporalVariavelAleatoriaInterna(const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const SmartEnupla<Periodo, double> &a_serie_temporal);

	void addTendenciaTemporalVariavelAleatoriaInterna(const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const SmartEnupla <IdCenario, SmartEnupla<Periodo, double>> &a_tendencia_temporal);

	void gerarTendenciaTemporalMediaVariavelAleatoriaInterna(const Periodo a_periodo_final);

	void setCenariosInternos(const AttMatrizVariavelAleatoriaInterna a_attMatrizVariavelAleatoriaInterna, const SmartEnupla<IdVariavelAleatoriaInterna, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>>& a_matriz);

	void calcularSerieTemporal();

	void calcularTendenciaTemporal();

	void calcularCoeficienteParticipacaoVariavelAleatoriaInterna();

	void calcularEstatisticaSerieTemporal();

	void calcularEstatisticaSerieTransformada();

	void calcularAutoCovarianciaSerieTransformada();

	void calcularAutoCovarianciaNaoSerieTransformada();

	void calcularAutoCorrelacaoSerieTransformada();

	void calcularAutoCorrelacaoNaoSazonalSerieTransformada();

	void calcularAutoCorrelacaoParcialSerieTransformada();

	void determinarOrdemAutoCorrelacaoSerieTransformada(const TipoValor a_tipo_coeficiente_auto_correlacao, const int a_ordem_fixa_correlacao_temporal);

	void calcularCoeficientesAutoCorrelacaoSerieTransformada(const TipoValor a_tipo_coeficiente_auto_correlacao, const int a_ordem_fixa_correlacao_temporal);

	void parametrizarModelo(const TipoModeloGeracaoSinteticaCenario a_tipo_modelo_geracao_sintetica, const TipoValor a_tipo_coeficiente_auto_correlacao, const int a_ordem_coeficiente_auto_correlacao);

	void parametrizar_modelo_lognormal_3p_sazonal(const TipoValor a_tipo_coeficiente_auto_correlacao, const int a_ordem_fixa_correlacao_temporal);

	void calcularSerieResiduo();

	void calcularEstatisticaSerieResiduo();

	void calcularAutoCorrelacaoSerieResiduo();

	void calcularAutoCorrelacaoNaoSazonalSerieResiduo();

	void calcularSigmaMiDeltaResiduoNormal();

	double calcularRuidoCorrelacionado(const TipoCorrelacaoVariaveisAleatorias a_tipo_correlacao_variaveis_aleatorias, const IdEstacao a_idEstacao, const SmartEnupla<IdVariavelAleatoria, double>& a_ruido_branco);

	void gerarRuidoBrancoEspacoAmostral(const SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> &a_horizonte_espaco_amostral, const TipoSorteio a_tipo_sorteio, int &a_semente);
	void gerarRuidoBrancoEspacoAmostral(const SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> &a_horizonte_espaco_amostral, const TipoSorteio a_tipo_sorteio, const int a_numero_maximo_periodos, int &a_semente);

	void gerarEspacoAmostralFromRuido(const SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>>& a_horizonte_espaco_amostral);

	void gerarCenariosEspacoAmostral(const SmartEnupla<IdCenario, IdCenario> &a_mapeamento_cenarios_tendencia, const SmartEnupla <IdCenario, SmartEnupla<Periodo, IdRealizacao>> &a_mapeamento_amostra_comum, const SmartEnupla<IdCenario, SmartEnupla<Periodo, double>> &a_horizonte_processo_estocastico, const bool a_gerar_cenarios_buffer, const bool a_gerar_cenarios_internos, const bool a_espaco_amostral_mesmo_tipo_periodo);

	void expandirParametrosEspacoAmostral(const SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>>& a_horizonte_espaco_amostral);

	void expandirParametrosHorizonteCompleto(const SmartEnupla<Periodo, double>& a_horizonte_completo);

	//double getResiduoEspacoAmostral(const IdRealizacao a_idRealizacao, const Periodo a_periodo) const;
	//double getResiduoEspacoAmostral(const IdCenario a_idCenario, const Periodo a_periodo) const;

	double getRealizacaoEspacoAmostral(const IdCenario a_idCenario, const Periodo a_periodo) const;
	double getRealizacaoEspacoAmostral(const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const Periodo a_periodo, const bool a_espaco_amostral_mesmo_tipo_periodo);
	double getRealizacaoEspacoAmostral(const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const SmartEnupla<Periodo, IdRealizacao> a_idRealizacoes_cenario, const Periodo a_periodo, const SmartEnupla<Periodo, IdRealizacao> a_horizonte_mapeamento_espaco_amostral, const bool a_espaco_amostral_mesmo_tipo_periodo);

	double getRealizacaoTransformadaEspacoAmostral(const IdCenario a_idCenario, const Periodo a_periodo) const;
	double getRealizacaoTransformadaEspacoAmostral(const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const Periodo a_periodo, const bool a_espaco_amostral_mesmo_tipo_periodo) const;
	double getRealizacaoTransformadaEspacoAmostral_recursivo(const IdCenario a_idCenario, const IdRealizacao a_idRealizacao,  const SmartEnupla<Periodo, IdRealizacao> a_idRealizacoes_cenario, const Periodo a_periodo, const Periodo a_periodo_realizacao, const SmartEnupla<Periodo, double> a_horizonte_completo, const bool a_espaco_amostral_mesmo_tipo_periodo);

	double getTendenciaTemporalTransformada(const SmartEnupla<IdCenario, IdCenario> a_mapeamento_cenarios_tendencia, const IdCenario a_idCenario, const Periodo a_periodo);
	double getTendenciaTemporalTransformadaFromVariavelAleatoriaInterna(const SmartEnupla<IdCenario, IdCenario> a_mapeamento_cenarios_tendencia, const IdCenario a_idCenario, const Periodo a_periodo);

	double calcularResiduo             (const double a_ruido_correlacionado, const Periodo a_periodo) const;
	double calcularResiduo             (const Periodo a_periodo, const SmartEnupla<Periodo, double> &a_tendencia, const double a_realizacao, const bool a_espaco_amostral_mesmo_tipo_periodo) const;
	double calcularRealizacao          (const Periodo a_periodo, const SmartEnupla<Periodo, double> &a_tendencia, const double a_residuo, const bool a_espaco_amostral_mesmo_tipo_periodo) const;
	void   calcularRealizacaoAndResiduo(const double a_ruido_correlacionado, const Periodo a_periodo, const SmartEnupla<Periodo, double> &a_tendencia, double &a_valor_realizacao, double &a_valor_residuo_realizacao, const bool a_espaco_amostral_mesmo_tipo_periodo) const;

	double calcularRealizacaoInterna(const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const Periodo a_periodo, const double a_realizacao);
	double calcularRealizacaoInterna(const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const IdCenario a_idCenario, const Periodo a_periodo);
	double calcularRealizacaoInterna(const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const Periodo a_periodo, const bool a_espaco_amostral_mesmo_tipo_periodo);
	double calcularRealizacaoInterna(const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const Periodo a_periodo, const SmartEnupla<Periodo, IdRealizacao> a_idRealizacoes_cenario, const SmartEnupla<Periodo, IdRealizacao> a_horizonte_mapeamento_espaco_amostral, const bool a_espaco_amostral_mesmo_tipo_periodo) ;

	double calcularRealizacaoParaValor(const double a_valor, const Periodo a_periodo);

	void addRealizacaoInterna(const IdCenario a_idCenario, const Periodo a_periodo);

	void setRealizacaoInterna(const IdCenario a_idCenario, const Periodo a_periodo);
	void setRealizacaoInternaFromTendencia(const IdCenario a_idCenario, const IdCenario a_idCenario_tendencia, const Periodo a_periodo);

	double calcularRegressivo_lognormal_3p(const Periodo a_periodo, const SmartEnupla<Periodo, double> &a_tendencia, const bool a_espaco_amostral_mesmo_tipo_periodo) const;

	double calcularResiduo_lognormal_3p(double a_ruido_correlacionado, const Periodo a_periodo) const;

};

GET_STRING_FROM_CLASS(VariavelAleatoria)

#endif 
