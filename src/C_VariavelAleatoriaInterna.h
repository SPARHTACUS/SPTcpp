#ifndef VARIAVEL_ALEATORIA_INTERNA
#define VARIAVEL_ALEATORIA_INTERNA


#include "C_SmartElemento.h"

#define ATT_COMUM_VARIAVEL_ALEATORIA_INTERNA(m)  \
	  m(VariavelAleatoriaInterna,  AttComum,                 idVariavelAleatoriaInterna,            IdVariavelAleatoriaInterna,         min,         max,                 min,      sim) \
	  m(VariavelAleatoriaInterna,  AttComum,                                       nome,                                string,         min,         max,              Nenhum,      sim) \
	  m(VariavelAleatoriaInterna,  AttComum,                             grau_liberdade,                                double,           0,         max,                   0,      sim) 
//                      c_classe,   smrtAtt,                               nomeAtributo,                                  tipo,  lowerBound,  upperBound,     initialValue, mustRead?

#define ATT_VETOR_VARIAVEL_ALEATORIA_INTERNA(m)  \
	  m(VariavelAleatoriaInterna,  AttVetor,                                serie_temporal,   double,        min,          max,             1,  Periodo)   \
	  m(VariavelAleatoriaInterna,  AttVetor,                   serie_temporal_transformada,   double,        min,          max,             1,  Periodo)   \
	  m(VariavelAleatoriaInterna,  AttVetor,                      coeficiente_participacao,   double,        min,          max,             1,  Periodo)   \
	  m(VariavelAleatoriaInterna,  AttVetor,                            tendencia_temporal,   double,        min,          max,             1,  Periodo)   \
	  m(VariavelAleatoriaInterna,  AttVetor,               tendencia_temporal_transformada,   double,        min,          max,             1,  Periodo)   \
	  m(VariavelAleatoriaInterna,  AttVetor,                          media_serie_temporal,   double,        min,          max,             1,  IdEstacao) \
	  m(VariavelAleatoriaInterna,  AttVetor,                      media_serie_transformada,   double,          1,          max,             1,  IdEstacao) \
	  m(VariavelAleatoriaInterna,  AttVetor,                         desvio_serie_temporal,   double,          0,          max,             1,  IdEstacao) \
	  m(VariavelAleatoriaInterna,  AttVetor,                     desvio_serie_transformada,   double,          0,          max,             1,  IdEstacao) \
	  m(VariavelAleatoriaInterna,  AttVetor,                     assimetria_serie_temporal,   double,          0,          max,             0,  IdEstacao) \
	  m(VariavelAleatoriaInterna,  AttVetor,                 assimetria_serie_transformada,   double,          0,          max,             0,  IdEstacao) \
	  m(VariavelAleatoriaInterna,  AttVetor,              coeficiente_participacao_sazonal,   double,          0,            1,             0,  IdEstacao) \
	  m(VariavelAleatoriaInterna,  AttVetor,                              auto_covariancia,   double,        min,          max,             0,  int) \
	  m(VariavelAleatoriaInterna,  AttVetor,                               auto_correlacao,   double,        min,          max,             0,  int) 
//                         c_classe,   smrtAtt,                                  nomeAtributo,     Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador

#define ATT_MATRIZ_VARIAVEL_ALEATORIA_INTERNA(m)  \
	  m(VariavelAleatoriaInterna,  AttMatriz,              cenarios_realizacao_espaco_amostral,       double,          min,          max,             1,  Periodo, IdCenario)   \
	  m(VariavelAleatoriaInterna,  AttMatriz,                                 auto_covariancia,       double,          min,          max,             0,  IdEstacao, int)       \
	  m(VariavelAleatoriaInterna,  AttMatriz,                                  auto_correlacao,       double,          min,          max,             0,  IdEstacao, int)       \
	  m(VariavelAleatoriaInterna,  AttMatriz,                                       correlacao,       double,          min,          max,             0,  IdVariavelAleatoriaInterna, IdEstacao)
//                         c_classe,    smrtAtt,                                     nomeAtributo,         Tipo,   lowerBound,   upperBound,  initialValue,  TipoIterador


#define SMART_ELEMENTO_VARIAVEL_ALEATORIA_INTERNA(m) \
	m(VariavelAleatoriaInterna,  AttComum,  ATT_COMUM_VARIAVEL_ALEATORIA_INTERNA) \
	m(VariavelAleatoriaInterna,  AttVetor,  ATT_VETOR_VARIAVEL_ALEATORIA_INTERNA) \
	m(VariavelAleatoriaInterna, AttMatriz, ATT_MATRIZ_VARIAVEL_ALEATORIA_INTERNA) \
	//m(VariavelAleatoriaInterna,    Membro,     MEMBRO_VARIAVEL_ALEATORIA_INTERNA)


DEFINE_SMART_ELEMENTO(VariavelAleatoriaInterna, SMART_ELEMENTO_VARIAVEL_ALEATORIA_INTERNA)

class VariavelAleatoriaInterna : public SmartDados {

	friend class ProcessoEstocastico;

public:

	VariavelAleatoriaInterna();
	VariavelAleatoriaInterna(const VariavelAleatoriaInterna &instanciaCopiar);
	void esvaziar();
	virtual ~VariavelAleatoriaInterna();

	DECLARA_SMART_ELEMENTO(VariavelAleatoriaInterna, SMART_ELEMENTO_VARIAVEL_ALEATORIA_INTERNA)

	void gerarTendenciaTemporalMedia(const Periodo a_periodo_final);

	double deslocarComGrauLiberdade(SmartEnupla<Periodo, double> &a_serie, const double a_grau_liberdade);

	void deslocarSerieComGrauLiberdade(const double a_grau_liberdade);

	void deslocarTendenciaComGrauLiberdade(const double a_grau_liberdade);

	void calcularEstatisticaSerieTemporal();

	void calcularEstatisticaSerieTransformada();

	void calcularAutoCovarianciaSerieTransformada();

	void calcularAutoCovarianciaNaoSazonalSerieTransformada();

	void calcularAutoCorrelacaoSerieTransformada();

	void calcularAutoCorrelacaoNaoSazonalSerieTransformada();

	double calcularRealizacao(const double a_realizacao_variavel_aleatoria, const Periodo a_periodo);

	double calcularRealizacaoVariavelAleatoriaParaValor(const double a_realizacao_variavel_aleatoria_interna, const Periodo a_periodo);

	void addRealizacao(const double a_realizacao_variavel_aleatoria, const IdCenario a_idCenario, const Periodo a_periodo);

	void setRealizacao(const double a_realizacao_variavel_aleatoria, const IdCenario a_idCenario, const Periodo a_periodo);
	void setRealizacaoFromTendencia(const IdCenario a_idCenario, const Periodo a_periodo);

};

GET_STRING_FROM_CLASS(VariavelAleatoriaInterna)

#endif 
