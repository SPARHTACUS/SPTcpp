#ifndef C_HIDRELETRICA_H
#define C_HIDRELETRICA_H

#include "C_FuncaoProducaoHidreletrica.h"
#include "C_ConjuntoHidraulico.h"
#include "C_PolinomioJusante.h"
#include "C_Reservatorio.h"
#include "C_Afluencia.h"
#include "C_RestricaoOperativaUHE.h"
#include "C_Defluencia.h"
#include "C_ReservatorioEquivalente.h"
#include "C_SmartElemento.h"

#define ATT_COMUM_HIDRELETRICA(m)  \
	  m(Hidreletrica,  AttComum,                    idHidreletrica,                       IdHidreletrica,        min,          max,                 1,         sim) \
	  m(Hidreletrica,  AttComum,                              nome,                               string,        min,          max,            Nenhum,         sim) \
	  m(Hidreletrica,  AttComum,                        submercado,                         IdSubmercado,     Nenhum,          max,            Nenhum,         sim) \
	  m(Hidreletrica,  AttComum,                             bacia,                  IdBaciaHidrografica,     Nenhum,          max,            Nenhum,         sim) \
	  m(Hidreletrica,  AttComum,                           jusante,                       IdHidreletrica,     Nenhum,          max,            Nenhum,         sim) \
 	  m(Hidreletrica,  AttComum,                    jusante_desvio,                       IdHidreletrica,     Nenhum,          max,            Nenhum,        CPPD) \
	  m(Hidreletrica,  AttComum,        tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica,        min,          max,         por_usina,         nao) \
	  m(Hidreletrica,  AttComum,   representacao_discreta_producao,                                 bool,      false,         true,             false,         nao) \
	  m(Hidreletrica,  AttComum,                  considerar_usina,                                 bool,      false,         true,              true,         sim) \
	  m(Hidreletrica,  AttComum,              numero_maquinas_base,                                  int,          0,          100,                 1,         nao) \
	  m(Hidreletrica,  AttComum,                      codigo_usina,                                  int,          0,          500,                 1,         nao) \
      m(Hidreletrica,  AttComum,                      codigo_posto,                                  int,          0,          500,                 0,         nao) \
	  m(Hidreletrica,  AttComum,          tipo_de_perda_hidraulica,                  TipoPerdaHidraulica,        min,          max,               min,         nao) \
	  m(Hidreletrica,  AttComum,                  perda_hidraulica,                               double,          0,          100,                 0,         nao) \
	  m(Hidreletrica,  AttComum,                 fator_de_producao,                               double,          0,         0.01,                 0,         nao) \
      m(Hidreletrica,  AttComum,  vazao_defluente_minima_historica,                               double,          0,          max,                 0,         nao) \
      m(Hidreletrica,  AttComum, vazao_defluente_disponivel_minima,                               double,          0,        50000,                 0,         nao) \
	  m(Hidreletrica,  AttComum, vazao_defluente_disponivel_maxima,                               double,          0,          max,               max,         nao) \
	  m(Hidreletrica,  AttComum,            vazao_defluente_minima,                               double,          0,          max,                 0,         nao) \
	  m(Hidreletrica,  AttComum,            vazao_defluente_maxima,                               double,          0,          max,               max,         nao) \
	  m(Hidreletrica,  AttComum,                    vazao_retirada,                               double,          0,          max,                 0,         nao) \
	  m(Hidreletrica,  AttComum, vazao_turbinada_disponivel_minima,                               double,          0,        50000,                 0,         nao) \
	  m(Hidreletrica,  AttComum, vazao_turbinada_disponivel_maxima,                               double,          0,          max,               max,         nao) \
	  m(Hidreletrica,  AttComum,             vazao_desviada_minima,                               double,          0,        50000,                 0,         nao) \
	  m(Hidreletrica,  AttComum,             vazao_desviada_maxima,                               double,          0,          max,             99999,         nao) \
	  m(Hidreletrica,  AttComum,         indisponibilidade_forcada,                               double,          0,            1,                 0,         nao) \
	  m(Hidreletrica,  AttComum,      indisponibilidade_programada,                               double,          0,            1,                 0,         nao) \
	  m(Hidreletrica,  AttComum,                  canal_fuga_medio,                               double,          0,        10000,                 0,         nao) \
      m(Hidreletrica,  AttComum,                 tempo_viagem_agua,                                  int,          0,          360,                 0,         nao) \
 	  m(Hidreletrica,  AttComum,         vertimento_influencia_fcj,                                 bool,      false,         true,             false,         nao) \
 	  m(Hidreletrica,  AttComum,             tipo_elemento_jusante,                  TipoElementoJusante,     Nenhum,          max,            Nenhum,         nao) \
 	  m(Hidreletrica,  AttComum,                      tipo_turbina,                          TipoTurbina,     Nenhum,          max,            Nenhum,         nao) \
	  m(Hidreletrica,  AttComum,                 vertimento_maximo,                               double,        min,          max,               max,         nao) \
      m(Hidreletrica,  AttComum, percentual_retorno_vazao_desviada,                               double,          0,            1,                 0,         nao) \
      m(Hidreletrica,  AttComum,           penalidade_turbinamento,                               double,          0,          max,                 0,         sim) \
      m(Hidreletrica,  AttComum,             penalidade_vertimento,                               double,          0,          max,             0.001,         sim) \
      m(Hidreletrica,  AttComum,            penalidade_desvio_agua,                               double,          0,          max,               5000,         sim) \
	  m(Hidreletrica,  AttComum,    penalidade_turbinamento_minimo,                               double,          0,          max,               5000,         sim) \
	  m(Hidreletrica,  AttComum, penalidade_vazao_defluente_minima,                               double,          0,          max,               5000,         sim) \
	  m(Hidreletrica,  AttComum, penalidade_vazao_defluente_maxima,                               double,          0,          max,               5000,         sim) \
      m(Hidreletrica,  AttComum,  penalidade_vazao_desviada_minima,                               double,          0,          max,               5000,         sim) \
	  m(Hidreletrica,  AttComum,  penalidade_afluencia_incremental,                               double,          0,          max,               5000,         sim) \
      m(Hidreletrica,  AttComum,         penalidade_vazao_retirada,                               double,          0,          max,              10000,         sim) \
	  m(Hidreletrica,  AttComum,          penalidade_volume_minimo,                               double,          0,          max,              10000,         sim) \
	  m(Hidreletrica,  AttComum,     penalidade_volume_util_minimo,                               double,          0,          max,                600,         não) \
	  m(Hidreletrica,  AttComum,             penalidade_evaporacao,                               double,          0,          max,              10000,         sim) \
      m(Hidreletrica,  AttComum,        penalidade_potencia_minima,                               double,          0,          max,               5000,         sim) \
      m(Hidreletrica,  AttComum,      considerar_tempo_viagem_agua,                               bool,        false,         true,              false,         nao) \
      m(Hidreletrica,  AttComum,                  queda_referencia,                               double,          0,         1000,                  0,         nao) \
      m(Hidreletrica,  AttComum,                 volume_referencia,                               double,          0,       100000,                  0,         nao) \
      m(Hidreletrica,  AttComum,                tipo_regularizacao,                    TipoRegularizacao,     Nenhum,          max,             Nenhum,         nao) \
      m(Hidreletrica,  AttComum,          codigo_usina_jusante_JUSENA,                                  int,          0,          500,                  0,         nao) \
      m(Hidreletrica,  AttComum,               produtibilidade_EAR,                               double,          0,       100000,                  0,         nao)
//          c_classe,   smrtAtt,                      nomeAtributo,                                 tipo, lowerBound,   upperBound,      initialValue, mustRead?


#define ATT_VETOR_HIDRELETRICA(m)  \
	  m(Hidreletrica,  AttVetor,                                 montante,                        IdHidreletrica,        min,          max,           min,        int) \
      m(Hidreletrica,  AttVetor,                          montante_desvio,                        IdHidreletrica,        min,          max,           min,        int) \
	  m(Hidreletrica,  AttVetor,               tipo_detalhamento_producao,  TipoDetalhamentoProducaoHidreletrica,        min,          max,           min,    Periodo) \
	  m(Hidreletrica,  AttVetor,                            regularizacao,                                   int,          0,            1,             0,    Periodo) \
	  m(Hidreletrica,  AttVetor,              cota_montante_usina_jusante,                                double,          0,         1000,             0,    Periodo) \
	  m(Hidreletrica,  AttVetor,                   vazao_turbinada_minima,                                double,          0,        50000,             0,    Periodo) \
	  m(Hidreletrica,  AttVetor,                   vazao_turbinada_maxima,                                double,          0,        50000,             0,    Periodo) \
      m(Hidreletrica,  AttVetor,  vazao_turbinada_disponivel_minima_media,                                double,          0,        50000,             0,    Periodo) \
	  m(Hidreletrica,  AttVetor,  vazao_turbinada_disponivel_maxima_media,                                double,          0,          max,             0,    Periodo) \
	  m(Hidreletrica,  AttVetor,                         canal_fuga_medio,                                double,          0,        10000,             0,    Periodo) \
	  m(Hidreletrica,  AttVetor,                   vazao_defluente_minima,                                double,          0,        50000,             0,    Periodo) \
	  m(Hidreletrica,  AttVetor,                   vazao_defluente_maxima,                                double,          0,          max,           max,    Periodo) \
      m(Hidreletrica,  AttVetor,             vazao_defluente_minima_media,                                double,          0,        50000,             0,    Periodo) \
	  m(Hidreletrica,  AttVetor,             vazao_defluente_maxima_media,                                double,          0,          max,           max,    Periodo) \
      m(Hidreletrica,  AttVetor,                    vazao_desviada_minima,                                double,          0,        50000,             0,    Periodo) \
	  m(Hidreletrica,  AttVetor,                    vazao_desviada_maxima,                                double,          0,          max,           max,    Periodo) \
      m(Hidreletrica,  AttVetor,              vazao_desviada_minima_media,                                double,          0,        50000,             0,    Periodo) \
	  m(Hidreletrica,  AttVetor,              vazao_desviada_maxima_media,                                double,          0,          max,           max,    Periodo) \
      m(Hidreletrica,  AttVetor,                    potencia_minima_media,                                double,          0,        50000,             0,    Periodo) \
	  m(Hidreletrica,  AttVetor,                    potencia_maxima_media,                                double,          0,          max,           max,    Periodo) \
      m(Hidreletrica,  AttVetor,         potencia_disponivel_minima_media,                                double,          0,        50000,             0,    Periodo) \
	  m(Hidreletrica,  AttVetor,         potencia_disponivel_maxima_media,                                double,          0,          max,           max,    Periodo) \
      m(Hidreletrica,  AttVetor,                           vazao_retirada,                                double,     -50000,        50000,             0,    Periodo) \
	  m(Hidreletrica,  AttVetor,                indisponibilidade_forcada,                                double,          0,            1,             0,    Periodo) \
      m(Hidreletrica,  AttVetor,             indisponibilidade_programada,                                double,          0,            1,             0,    Periodo) \
      m(Hidreletrica,  AttVetor,                          disponibilidade,                                double,          0,            1,             0,    Periodo) \
      m(Hidreletrica,  AttVetor,            produtibilidade_acumulada_EAR,                                double,          0,        50000,             0,    IdReservatorioEquivalente) \
      m(Hidreletrica,  AttVetor,                      produtibilidade_ENA,                                double,          0,        50000,             0,    Periodo) \
      m(Hidreletrica,  AttVetor,                codigo_usinas_calculo_ENA,                                   int,        min,          max,           min,        int)


//          c_classe,   smrtAtt,                             nomeAtributo,                                  Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador


#define ATT_MATRIZ_HIDRELETRICA(m)  \
      m(Hidreletrica,  AttMatriz,                          potencia_minima,     double,          0,      50000,              0,  Periodo, IdPatamarCarga) \
      m(Hidreletrica,  AttMatriz,                          potencia_maxima,     double,          0,      50000,              0,  Periodo, IdPatamarCarga) \
      m(Hidreletrica,  AttMatriz,               potencia_disponivel_minima,     double,          0,      50000,              0,  Periodo, IdPatamarCarga) \
      m(Hidreletrica,  AttMatriz,               potencia_disponivel_maxima,     double,          0,        max,            max,  Periodo, IdPatamarCarga) \
      m(Hidreletrica,  AttMatriz,        vazao_turbinada_disponivel_minima,     double,          0,      50000,              0,  Periodo, IdPatamarCarga) \
      m(Hidreletrica,  AttMatriz,        vazao_turbinada_disponivel_maxima,     double,          0,        max,            max,  Periodo, IdPatamarCarga) \
      m(Hidreletrica,  AttMatriz,                   vazao_defluente_minima,     double,          0,      50000,              0,  Periodo, IdPatamarCarga) \
      m(Hidreletrica,  AttMatriz,                   vazao_defluente_maxima,     double,          0,        max,            max,  Periodo, IdPatamarCarga) \
      m(Hidreletrica,  AttMatriz,                    vazao_desviada_minima,     double,          0,      50000,              0,  Periodo, IdPatamarCarga) \
      m(Hidreletrica,  AttMatriz,                    vazao_desviada_maxima,     double,          0,        max,            max,  Periodo, IdPatamarCarga) \
      m(Hidreletrica,  AttMatriz,                 potencia_disponivel_meta,     double,          0,        max,              0,  IdCenario, Periodo) \
      m(Hidreletrica,  AttMatriz,          vazao_turbinada_disponivel_meta,     double,          0,        max,              0,  IdCenario, Periodo) \
      m(Hidreletrica,  AttMatriz,                       vazao_vertida_meta,     double,          0,        max,              0,  IdCenario, Periodo) \
      m(Hidreletrica,  AttMatriz,                     vazao_evaporada_meta,     double,        min,        max,              0,  IdCenario, Periodo)
//          c_classe,    smrtAtt,                             nomeAtributo,       Tipo, lowerBound,   upperBound,  initialValue, TipoIterador

#define MEMBRO_HIDRELETRICA(m)                  \
	m(Hidreletrica, FuncaoProducaoHidreletrica) \
	m(Hidreletrica, ConjuntoHidraulico)         \
	m(Hidreletrica, PolinomioJusante)           \
	m(Hidreletrica, Defluencia)                 \
	m(Hidreletrica, Reservatorio)               \
	m(Hidreletrica, ReservatorioEquivalente)    \
	m(Hidreletrica, Afluencia)                  

#define SMART_ELEMENTO_HIDRELETRICA(m) \
	m(Hidreletrica, AttComum,  ATT_COMUM_HIDRELETRICA) \
	m(Hidreletrica, AttVetor,  ATT_VETOR_HIDRELETRICA) \
    m(Hidreletrica, AttMatriz,  ATT_MATRIZ_HIDRELETRICA) \
	m(Hidreletrica,   Membro,    MEMBRO_HIDRELETRICA)


DEFINE_SMART_ELEMENTO(Hidreletrica, SMART_ELEMENTO_HIDRELETRICA)


class Hidreletrica : public SmartDados {

	friend class Dados;
	friend class LeituraCEPEL;

public:

	Hidreletrica();			// Construtor
	Hidreletrica(const Hidreletrica& instanciaCopiar);
	void esvaziar();
	virtual ~Hidreletrica();	// Destrutor

	DECLARA_SMART_ELEMENTO(Hidreletrica, SMART_ELEMENTO_HIDRELETRICA)

		virtual void validacaoDadosAttComum();

private:

	SmartEnupla<Periodo, SmartEnupla<int, double>> calcularProdutibilidade(const TipoPerdaHidraulica a_tipo_perda_hidraulica, const double a_perda_hidraulica, const double a_fator_producao, const SmartEnupla<Periodo, double>& a_horizonte_estudo);

	double calcularProdutibilidade(const TipoPerdaHidraulica a_tipo_perda_hidraulica, const double a_perda_hidraulica, const double a_fator_producao, const double a_cota);
	double getCotaJusante(const double a_vazao_defluente, const Periodo a_periodo);
	double getCotaJusante_permite_valoresNegativos(const double a_vazao_defluente, const Periodo a_periodo); //Usado para encontrar a raiz do polinomio jusante

};


GET_STRING_FROM_CLASS(Hidreletrica)

#endif // !defined(C_UnidadesHidraulicas_H)
