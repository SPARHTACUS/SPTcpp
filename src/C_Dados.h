#ifndef DADOS_H
#define DADOS_H

#include <iostream>
#include <fstream>
#include <stdio.h>

#include "C_SmartElemento.h"
#include "C_BaciaHidrografica.h"
#include "C_Submercado.h"
#include "C_Intercambio.h"
#include "C_Hidreletrica.h"
#include "C_Termeletrica.h"
#include "C_RestricaoEletrica.h"
#include "C_AgrupamentoIntercambio.h"
#include "C_ProcessoEstocastico.h"
#include "C_UsinaElevatoria.h"
#include "C_Contrato.h"
#include "C_ReservaPotencia.h"
#include "C_Eolica.h"
#include "C_DiscretizacaoTemporal.h"
#include "C_Regua11.h"
#include "C_IntercambioHidraulico.h"
#include "C_DemandaEspecial.h"
#include "C_EstruturaResultados.h"
#include "C_ArranjoResolucao.h"

#define ATT_COMUM_DADOS(m)  \
	  m(Dados,  AttComum,                                         idDados,                           IdDados,           1,           1,             1,      nao) \
	  m(Dados,  AttComum,                                     nome_estudo,                            string,         min,         max,        Nenhum,      nao) \
	  m(Dados,  AttComum,                                     tipo_estudo,                        TipoEstudo,         min,         max,           min,      sim) \
	  m(Dados,  AttComum,                             taxa_desconto_anual,                            double,           0,           1,             0,      sim) \
	  m(Dados,  AttComum,                              periodo_referencia,                           Periodo,         min,         max,           min,      sim) \
	  m(Dados,  AttComum,                                 estagio_inicial,                         IdEstagio,           1,         max,             1,      sim) \
	  m(Dados,  AttComum,                                   estagio_final,                         IdEstagio,           1,         max,             1,      sim) \
	  m(Dados,  AttComum,                  estagio_acoplamento_pre_estudo,                         IdEstagio,         min,         max,             1,      sim) \
	  m(Dados,  AttComum,                     diretorio_importacao_cortes,                            string,         min,         max,        Nenhum,      sim) \
	  m(Dados,  AttComum,                 diretorio_importacao_pos_estudo,                            string,         min,         max,        Nenhum,      sim) \
	  m(Dados,  AttComum,                 diretorio_importacao_pre_estudo,                            string,         min,         max,        Nenhum,      sim) \
	  m(Dados,  AttComum,                                iteracao_inicial,                        IdIteracao,         min,         max,           min,      nao) \
	  m(Dados,  AttComum,                         numero_maximo_iteracoes,                               int,           0,         max,             0,      sim) \
	  m(Dados,  AttComum,                                numero_aberturas,                               int,           1,         max,             1,      nao) \
	  m(Dados,  AttComum,             visitar_todos_cenarios_por_iteracao,                              bool,         min,         max,             0,      sim) \
	  m(Dados,  AttComum,                                 numero_cenarios,                               int,           1,         max,             1,      sim) \
	  m(Dados,  AttComum,                                    tipo_semente,                       TipoSemente,         min,         max,           min,      nao) \
	  m(Dados,  AttComum,                            tipo_aversao_a_risco,                  TipoAversaoRisco,         min,         max,           min,      nao) \
	  m(Dados,  AttComum,                                      alpha_CVAR,                            double,           0,           1,             0,      nao) \
	  m(Dados,  AttComum,                                     lambda_CVAR,                            double,           0,           1,             0,      nao) \
	  m(Dados,  AttComum,                      tipo_tendencia_hidrologica,          TipoTendenciaEstocastica,         min,         max,           min,      nao) \
	  m(Dados,  AttComum,         periodo_tendencia_hidrologica_historica,                           Periodo,         min,         max,           min,      nao) \
	  m(Dados,  AttComum,           tipo_processo_estocastico_hidrologico,             IdProcessoEstocastico,         min,         max,           min,      sim) \
	  m(Dados,  AttComum, relaxar_afluencia_incremental_com_viabilidade_hidraulica,                     bool,         min,         max,           nao,      nao) \
	  m(Dados,  AttComum,    calcular_custo_primal_via_subproblema_mestre,                              bool,          min,         max,           min,     nao) \
	  m(Dados,  AttComum,            tipo_espaco_amostral_geracao_cenario_hidrologico,                TipoEspacoAmostral,         min,         max,           min,      nao) \
	  m(Dados,  AttComum,                 tipo_correlacao_geracao_cenario_hidrologico, TipoCorrelacaoVariaveisAleatorias,         min,         max,                 min,      nao) \
	  m(Dados,  AttComum,                 correlacao_dominante_geracao_cenario_hidrologico,                double,        0.1,         1.0,         0.95,      nao) \
	  m(Dados,  AttComum,                     tipo_modelo_geracao_cenario_hidrologico, TipoModeloGeracaoSinteticaCenario,      nenhum,         max,                 min,      nao) \
	  m(Dados,  AttComum,tipo_coeficiente_auto_correlacao_geracao_cenario_hidrologico,                         TipoValor,         min,         max, positivo_e_negativo,      nao) \
	  m(Dados,  AttComum,       numero_periodos_avaliacao_geracao_cenario_hidrologico,                               int,           0,         max,                   0,      nao) \
	  m(Dados,  AttComum,         semente_espaco_amostral_geracao_cenario_hidrologico,                               int,           1,         max,                   1,      nao) \
	  m(Dados,  AttComum,                         semente_geracao_cenario_hidrologico,                               int,           1,         max,                   1,      nao) \
	  m(Dados,  AttComum,    tipo_sorteio_espaco_amostral_geracao_cenario_hidrologico,                       TipoSorteio,          min,         max,                  MC,      nao) \
	  m(Dados,  AttComum,                            tipo_geracao_cenario_hidrologico,                TipoGeracaoCenario,         min,         max,                 min,      nao) \
	  m(Dados,  AttComum,               periodo_historico_geracao_cenario_hidrologico,                           Periodo,         min,         max,                 min,      nao) \
	  m(Dados,  AttComum,          imprimir_espaco_amostral_geracao_cenario_hidrologico,                              bool,         min,         max,                 sim,      nao) \
	  m(Dados,  AttComum,                        imprimir_geracao_cenario_hidrologico,                              bool,         min,         max,                 nao,      nao) \
	  m(Dados,  AttComum,    ordem_maxima_auto_correlacao_geracao_cenario_hidrologico,                            int,           0,         365,             0,      nao) \
	  m(Dados,  AttComum,                 calcular_cenario_hidrologico_pre_otimizacao,                  bool,         min,         max,             1,      nao) \
	  m(Dados,  AttComum,                 imprimir_cenario_hidrologico_pre_otimizacao,                  bool,         min,         max,             0,      nao) \
	  m(Dados,  AttComum,                   imprimir_exportacao_pos_estudo,                  bool,         min,         max,             0,      nao) \
	  m(Dados,  AttComum,                             tipo_metodo_solucao,                   IdMetodoSolucao,         min,         max,           min,      nao) \
	  m(Dados,  AttComum,                           maior_corte_importado,                    IdCorteBenders,         min,         max,           max,      nao) \
	  m(Dados,  AttComum,                maior_corte_importado_pos_estudo,                    IdCorteBenders,         min,         max,           max,      nao) \
	  m(Dados,  AttComum,                 selecao_cortes_nivel_dominancia,                               int,           0,          10,             1,      nao) \
	  m(Dados,  AttComum,                       remover_cortes_dominados,                               bool,         min,         max,           sim,      nao) \
	  m(Dados,  AttComum,                         diretorio_entrada_dados,                            string,         min,         max,  DadosEntrada,      nao) \
	  m(Dados,  AttComum,                           diretorio_saida_dados,                            string,         min,         max,    DadosSaida,      nao) \
	  m(Dados,  AttComum,                                    tipo_solver,                         TipoSolver,         min,         max,           clp,      nao) \
      m(Dados,  AttComum,                                  metodo_solver,                   TipoMetodoSolver,         min,         max,  dual_simplex,      nao) \
	  m(Dados,  AttComum,                                 foco_numerico,                                 int,           0,           4,             2,      nao) \
	  m(Dados,  AttComum,                                  escalonamento,                                int,           0,           4,             1,      nao) \
	  m(Dados,  AttComum,                         tolerancia_otimalidade,                             double,        1e-12,        1e-2,          1e-6,      nao) \
	  m(Dados,  AttComum,                         tolerancia_viabilidade,                             double,        1e-12,        1e-2,          1e-6,      nao) \
	  m(Dados,  AttComum,           multiplicador_tempo_limite_resolucao,                             double,         min,         max,            -1,      nao) \
	  m(Dados,  AttComum,            numero_maximo_tentativas_resolucao,                                 int,           1,        1000,           100,      nao) \
	  m(Dados,  AttComum,                    tipo_selecao_solucao_proxy,             TipoSelecaoSolucaoProxy,         min,         max,           min,      nao) \
	  m(Dados,  AttComum,                            solucao_proxy_nula,                                bool,         min,         max,           min,      nao) \
	  m(Dados,  AttComum,                               imprimir_solver,                                bool,         min,         max,           min,      nao) \
	  m(Dados,  AttComum,        imprimir_solver_viabilidade_hidraulica,                                bool,         min,         max,           min,      nao) \
	  m(Dados,  AttComum,                        imprimir_solver_mestre,                                bool,         min,         max,           min,      nao) \
	  m(Dados,  AttComum,               exibir_na_tela_resultado_solver,                                bool,         min,         max,           min,      nao) \
	  m(Dados,  AttComum,                  imprimir_estado_por_iteracao,                                bool,         min,         max,           min,      nao) \
	  m(Dados,  AttComum,                   imprimir_corte_por_iteracao,                                bool,         min,         max,           min,      nao) \
	  m(Dados,  AttComum,           imprimir_selecao_corte_por_iteracao,                                bool,         min,         max,           min,      nao) \
	  m(Dados,  AttComum,             imprimir_tempos_construcao_modelo,                                bool,         min,         max,           min,      nao) \
	  m(Dados,  AttComum,                imprimir_tempos_metodo_solucao,                                bool,         min,         max,           min,      nao) \
	  m(Dados,  AttComum,    imprimir_resultado_por_estagio_por_cenario,                                bool,         min,         max,           min,      nao) \
	  m(Dados,  AttComum, imprimir_resultado_por_estagio_por_cenario_por_realizacao,                    bool,         min,         max,           min,      nao) \
	  m(Dados,  AttComum,      imprimir_resultado_viabilidade_hidraulica,                               bool,         min,         max,           min,      nao) \
	  m(Dados,  AttComum,                      imprimir_resultado_mestre,                               bool,         min,         max,           min,      nao) \
      m(Dados,  AttComum,  representar_producao_hidreletrica_com_turbinamento_disponivel,               bool,         min,         max,           nao,      nao) \
      m(Dados,  AttComum,  representar_todos_balancos_hidricos_por_volume,                              bool,         min,         max,           nao,      nao) \
      m(Dados,  AttComum,  representar_defluencia_disponivel_em_restricoes_hidraulicas,                 bool,         min,         max,           sim,      nao) \
      m(Dados,  AttComum,  representar_turbinamento_disponivel_em_restricoes_hidraulicas,               bool,         min,         max,           sim,      nao) \
      m(Dados,  AttComum,  representar_potencia_hidro_disponivel_em_restricoes_hidraulicas,             bool,         min,         max,           sim,      nao) \
      m(Dados,  AttComum,  representar_potencia_termo_disponivel_em_restricoes_hidraulicas,             bool,         min,         max,           sim,      nao) \
      m(Dados,  AttComum,           nao_utilizar_restricoes_simples_em_restricoes_eletricas,            bool,         min,         max,           nao,      nao) \
      m(Dados,  AttComum,           nao_utilizar_restricoes_simples_em_restricoes_hidraulicas,          bool,         min,         max,         nao,      nao) \
      m(Dados,  AttComum,               maior_estagio_impressao_versao_alternativa_cortes,         IdEstagio,      min,         max,            12,      nao) \
      m(Dados,  AttComum,                              mes_penalizacao_volume_util_minimo,             IdMes,      nenhum,         max,            11,      nao) \
      m(Dados,  AttComum,                  custo_acumulado_penalizacao_volume_util_minimo,            double,           0,         max,             0,      nao) \
      m(Dados,  AttComum,               taxa_considerar_tempo_viagem_agua,                            double,           0,           1,           0.2,      nao) \
      m(Dados,  AttComum,                     tipo_processamento_paralelo,         TipoProcessamentoParalelo,         min,         max,   por_cenario,      nao) \
      m(Dados,  AttComum,              imprimir_cortes_NW_com_reducao_estados,                          bool,         min,         max,           nao,      nao) \
      m(Dados,  AttComum,                          considerar_variaveis_folga,                          bool,         min,         max,           sim,      nao)


//   c_classe,   smrtAtt,                                    nomeAtributo,                              tipo,  lowerBound,  upperBound,         initialValue, mustRead?


#define ATT_VETOR_DADOS(m)  \
	  m(Dados,  AttVetor,                                          horizonte_otimizacao,       Periodo,        min,          max,           min,  IdEstagio) \
	  m(Dados,  AttVetor,                                              horizonte_estudo,     IdEstagio,        min,          max,           min,    Periodo) \
	  m(Dados,  AttVetor,                                        conversor_vazao_volume,        double,          0,          100,             0,    Periodo) \
	  m(Dados,  AttVetor,                                              numero_aberturas,           int,          1,          max,             0,  IdEstagio) \
	  m(Dados,  AttVetor,                                numero_aberturas_solucao_proxy,           int,          1,          max,             0,  IdEstagio) \
	  m(Dados,  AttVetor,                                                   lambda_CVAR,        double,          0,            1,             0,  IdEstagio) \
	  m(Dados,  AttVetor,                                                    alpha_CVAR,        double,          0,            1,             0,  IdEstagio) \
	  m(Dados,  AttVetor,                                              cortes_multiplos,           int,          0,          max,             0,  IdEstagio)  

//     c_classe,   smrtAtt,              nomeAtributo,      Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador


#define ATT_MATRIZ_DADOS(m)  \
	  m(Dados,  AttMatriz,     percentual_duracao_patamar_carga,                                       double,          0,            1,             0,    Periodo,   IdPatamarCarga) \
	  m(Dados,  AttMatriz,               conversor_vazao_volume,                                       double,          0,          100,             0,    Periodo,   IdPatamarCarga) \
	  m(Dados,  AttMatriz,  percentual_duracao_horizonte_estudo,                                       double,          0,            1,             0,  IdEstagio,   Periodo)        \
	  m(Dados,  AttMatriz,   desagio_acumulado_horizonte_estudo,                                       double,          0,            1,             0,  IdEstagio,   Periodo)                   

//   c_classe,    smrtAtt,                      nomeAtributo,       Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador

#define MEMBRO_DADOS(m)              \
	m(Dados, BaciaHidrografica)      \
	m(Dados, Submercado)             \
	m(Dados, Intercambio)            \
	m(Dados, Termeletrica)           \
	m(Dados, Hidreletrica)           \
	m(Dados, RestricaoEletrica)      \
    m(Dados, RestricaoOperativaUHE)  \
	m(Dados, AgrupamentoIntercambio) \
    m(Dados, UsinaElevatoria)        \
    m(Dados, Contrato)               \
    m(Dados, ReservaPotencia)        \
    m(Dados, UsinaEolica)            \
    m(Dados, Discretizacao)          \
    m(Dados, Regua11)                \
    m(Dados, DemandaEspecial)        \
    m(Dados, IntercambioHidraulico)


#define SMART_ELEMENTO_DADOS(m)           \
	m(Dados,   AttComum, ATT_COMUM_DADOS)  \
	m(Dados,   AttVetor, ATT_VETOR_DADOS)  \
	m(Dados,  AttMatriz, ATT_MATRIZ_DADOS)  \
	m(Dados,    Membro,    MEMBRO_DADOS)

DEFINE_SMART_ELEMENTO(Dados, SMART_ELEMENTO_DADOS)

class EntradaSaidaDados;

class Dados : public SmartDados {

	friend class Leitura_CEPEL;

public:

	//////////////////////////////////////////////////////////////////////
	// Construtor/Destrutor
	//////////////////////////////////////////////////////////////////////

	Dados();
	Dados(const Dados& instanciaCopiar);
	void esvaziar();
	virtual ~Dados();

	DECLARA_SMART_ELEMENTO(Dados, SMART_ELEMENTO_DADOS)


public:

	bool isDadosAttComumValidados()const;

	Periodo   getMenorPeriodoHorizonteEstudo() const;
	Periodo   getMaiorPeriodoHorizonteEstudo() const;

	enum PreencherAtributo {
		nao_sem_utilizacao,
		nao_operacional_informado,
		nao_premissa_informado,
		sim_operacional,
		sim_operacional_info,
		sim_premissa,
	};

	//
	// Validações
	//

	void validaIntercambio();

	void validaTermeletrica();

	void validaBaciaHidrografica();

	void validaIntercambioHidraulico();

	void validaUsinaElevatoria();

	void validaContrato();

	void validaUnidadeUTE();

	void validacao_operacional_Dados(EntradaSaidaDados a_entrada_saida_dados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_att_operacionais_sem_recarregar);

	void validacao_operacional_Termeletrica(EntradaSaidaDados a_entrada_saida_dados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_att_operacionais_sem_recarregar);

	void validacao_operacional_Intercambio_Hidraulico(EntradaSaidaDados a_entrada_saida_dados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_att_operacionais_sem_recarregar);

	void validacao_operacional_Hidreletrica(EntradaSaidaDados a_entrada_saida_dados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_att_operacionais_sem_recarregar);

	void validacao_operacional_Submercado(EntradaSaidaDados a_entrada_saida_dados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_att_operacionais_sem_recarregar);

	void validacao_operacional_Intercambio(EntradaSaidaDados a_entrada_saida_dados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_att_operacionais_sem_recarregar);

	void validacao_operacional_RestricaoEletrica(EntradaSaidaDados a_entrada_saida_dados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_att_operacionais_sem_recarregar);

	void validacao_operacional_Contrato(EntradaSaidaDados a_entrada_saida_dados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_att_operacionais_sem_recarregar);

	void validacao_operacional_DemandaEspecial(EntradaSaidaDados a_entrada_saida_dados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_att_operacionais_sem_recarregar);

	void validacao_operacional_Eolica(EntradaSaidaDados a_entrada_saida_dados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_att_operacionais_sem_recarregar);

	void validacao_operacional_BaciaHidrografica(EntradaSaidaDados a_entrada_saida_dados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_att_operacionais_sem_recarregar);

	void validacao_operacional_AgrupamentoIntercambio(EntradaSaidaDados a_entrada_saida_dados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_att_operacionais_sem_recarregar);

	void validacao_operacional_ProcessoEstocasticoHidrologico(EntradaSaidaDados& a_entrada_saida_dados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const std::string a_diretorio_exportacao_pos_estudo, const bool a_imprimir_att_operacionais_sem_recarregar);

	void validacao_operacional_RestricaoOperativaUHE(EntradaSaidaDados a_entrada_saida_dados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_att_operacionais_sem_recarregar);

	void validacao_operacional_UsinasElevatorias(EntradaSaidaDados a_entrada_saida_dados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_att_operacionais_sem_recarregar);

	void validacao_mapeamento_cenarios_aberturas(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar, bool &a_mapeamento_cenarios_e_aberturas_carregado);

	void instanciarProcessoEstocasticoHidrologicoComHistoricoAfluenciaIncremental(const IdProcessoEstocastico a_tipo_processo_estocastico, ProcessoEstocastico& a_processo_estocastico);

	void calcularDeterminacaoEspacialFromHistoricoAfluenciaNatural(EntradaSaidaDados a_entradaSaidaDados, const bool a_imprimir, const double a_valor_r2, ProcessoEstocastico& a_processo_estocastico);

	int isCalculoAttOperacionaisProcessoEstocasticoHidrologicoNecessario(ProcessoEstocastico& a_processo_estocastico, const IdProcessoEstocastico a_tipo_processo_estocastico);

	void validaHidreletrica();

	void validaPatamarDeficitEmSubmercado();

	void validaPatamarDeficitEmSubmercado(const IdSubmercado a_idSubmercado);

	void validaConjuntoHidraulicoEmHidreletrica();

	void validaUnidadeUHE();

	void validaReservatorioEmHidreletrica();

	bool validaFuncaoProducaoHidreletrica(FuncaoProducaoHidreletrica& a_funcaoProducaoHidreletrica, SmartEnupla<Periodo, double> a_horizonte_estudo, const IdHidreletrica a_idHidreletrica, const IdConjuntoHidraulico a_idConjuntoHidraulico, const IdUnidadeUHE a_idUnidadeUHE, SmartEnupla<AttVetorFuncaoProducaoHidreletrica, PreencherAtributo>& a_preencher_AttVetorFuncaoProducaoHidreletrica, SmartEnupla<AttMatrizFuncaoProducaoHidreletrica, PreencherAtributo>& a_preencher_AttMatrizFuncaoProducaoHidreletrica, SmartEnupla<AttVetorHidreletrica, PreencherAtributo>& a_preencher_AttVetorHidreletrica, SmartEnupla<AttVetorReservatorio, PreencherAtributo>& a_preencher_AttVetorReservatorio);

	void validaPatamarCarga();

	void validaRestricaoEletrica();

	void validaElementoSistemaRestricaoEletrica();

	void validaAgrupamentoIntercambio();

	void validaSubmercado();
	void validaSubmercado(const IdSubmercado a_idSubmercado);

	void validaUsinaNaoSimuladaEmSubmercado();
	void validaUsinaNaoSimuladaEmSubmercado(const IdSubmercado a_idSubmercado);

	void validaRestricaoOperativaUHE();

	void validaElementoSistemaRestricaoOperativaUHE();

	bool valida_historico_AfluenciaEmHidreletrica(const AttVetorAfluencia a_attVetorAfluencia);

	void calcular_historico_afluencia_incremental_com_natural();

	bool valida_tendencia_AfluenciaEmHidreletrica(const AttVetorAfluencia a_attVetorAfluencia);

	void calcular_tendencia_afluencia_incremental_com_natural();

	void adicionarTendenciaHidrologicaNula();
	void adicionarTendenciaHidrologicaHistorica();

	void valida_preconfig_hidraulica(const SmartEnupla<IdHidreletrica, IdHidreletrica> a_lista_jusante_hidreletrica, const SmartEnupla<IdHidreletrica, IdHidreletrica> a_lista_jusante_desvio_hidreletrica);

	void validaDefluencia();

	void valida_considerar_tempo_viagem_agua(const IdHidreletrica a_idHidreletrica);

	bool isRestricaoEletrica_simples(const IdRestricaoEletrica a_idRestricao_eletrica);
	bool isRestricaoOperativaUHE_simples(const IdRestricaoOperativaUHE a_idRestricaoOperacionalUHE);

	bool volume_inicial_carregado_from_premissa;
	bool volume_inicial_carregado_from_operacional;

	//
	// Demais Funções
	//

	SmartEnupla<IdHidreletrica, double> calculaAfluenciaIncremental(const SmartEnupla<IdHidreletrica, double>& a_afluenciaNatural);

	void definirCenariosPorProcessosEmArranjoResolucao();
	void mapearCenariosAberturasPorIteracaoEmArranjoResolucao();

	void adicionaHidreletricasMontante();

	void adicionaHidreletricasMontanteDesvio();

	void instanciaCotaJusanteUsinaJusante();

	void carregarArquivosEntrada(EntradaSaidaDados& a_entradaSaidaDados);

	SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> getHorizonteEspacoAmostralHidrologico(const IdEstagio a_idEstagioInicial, const IdEstagio a_idEstagioFinal)const;
	SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> getHorizonteEspacoAmostralHidrologico(const IdEstagio a_idEstagioInicial, const IdEstagio a_idEstagioFinal, const IdRealizacao a_maiorIdRealizacao)const;
	SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> getHorizonteEspacoAmostralHidrologico(const IdEstagio a_idEstagioInicial, const IdEstagio a_idEstagioFinal, const IdRealizacao a_maiorIdRealizacao, const bool a_manter_aberturas_estagio_inicial)const;

	IdHidreletrica getIdHidreletricaFromIdProcessoEstocasticoIdVariavelAleatoriaIdVariavelAleatoriaInterna(const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria, const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna);
	IdVariavelAleatoriaInterna getIdVariavelAleatoriaInternaFromIdVariavelAleatoriaIdHidreletrica(const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria, const IdHidreletrica a_hidreletrica);
	void getIdVariavelAleatoriaIdVariavelAleatoriaInternaFromIdHidreletrica(const IdProcessoEstocastico a_idProcessoEstocastico, IdVariavelAleatoria& a_idVariavelAleatoria, IdVariavelAleatoriaInterna& a_idVariavelAleatoriaInterna, const IdHidreletrica a_hidreletrica);


	ProcessoEstocastico processoEstocastico_hidrologico;

	ArranjoResolucao arranjoResolucao;

};

GET_STRING_FROM_CLASS(Dados)

#endif // 

