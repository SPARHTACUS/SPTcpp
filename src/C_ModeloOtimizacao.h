#ifndef MODELO_OTIMIZACAO
#define MODELO_OTIMIZACAO

#include "C_SmartElemento.h"

#include "C_Dados.h"

#include "C_ProcessoEstocastico.h"

#include "C_EstagioOtimizacao.h"
#include "C_Iteracao.h"

#include "C_ModeloOtimizacaoElementos.h"

#include "C_EstruturaResultados.h"

#include "C_ArranjoResolucao.h"

class EntradaSaidaDados;

#define ATT_COMUM_MODELO_OTIMIZACAO(m)  \
	  m(ModeloOtimizacao,  AttComum,                                               idModeloOtimizacao,                 IdModeloOtimizacao,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                                    valor_semente,                                int,          1,          max,          1,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                  selecao_cortes_nivel_dominancia,                                int,          0,           10,          1,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                         remover_cortes_dominados,                               bool,         min,         max,        nao,      nao) \
	  m(ModeloOtimizacao,  AttComum,                                            maior_corte_importado,                     IdCorteBenders,        min,          max,        max,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                 maior_corte_importado_pos_estudo,                     IdCorteBenders,        min,          max,        max,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                      diretorio_importacao_cortes,                             string,        min,          max,     Nenhum,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                  diretorio_importacao_pre_estudo,                             string,        min,          max,     Nenhum,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                  diretorio_importacao_pos_estudo,                             string,        min,          max,     Nenhum,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                                  numero_cenarios,                                int,          1,          max,          1,      sim) \
	  m(ModeloOtimizacao,  AttComum,                               numero_maximo_tentativas_resolucao,                                int,          1,         1000,        100,      sim) \
	  m(ModeloOtimizacao,  AttComum,                             multiplicador_tempo_limite_resolucao,                             double,        min,         max,         100,      nao) \
	  m(ModeloOtimizacao,  AttComum,                                                    foco_numerico,                                int,          1,            4,          2,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                           tolerancia_otimalidade,                             double,       1e-12,         1e-2,       1e-6,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                           tolerancia_viabilidade,                             double,       1e-12,         1e-2,       1e-6,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                       tipo_selecao_solucao_proxy,            TipoSelecaoSolucaoProxy,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                               solucao_proxy_nula,                               bool,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                                  imprimir_solver,                               bool,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                           imprimir_solver_viabilidade_hidraulica,                               bool,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                           imprimir_solver_mestre,                               bool,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                   imprimir_tempos_metodo_solucao,                               bool,        min,          max,        min,      nao) \
	  m(ModeloOtimizacao,  AttComum,                                     imprimir_estado_por_iteracao,                               bool,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                      imprimir_corte_por_iteracao,                               bool,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                              imprimir_selecao_corte_por_iteracao,                               bool,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                imprimir_variavel_decisao_por_estagio_por_cenario,                               bool,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                       imprimir_restricao_por_estagio_por_cenario,                               bool,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum, imprimir_variavel_decisao_por_estagio_por_cenario_por_realizacao,                               bool,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,        imprimir_restricao_por_estagio_por_cenario_por_realizacao,                               bool,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                        imprimir_resultado_viabilidade_hidraulica,                               bool,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                        imprimir_resultado_mestre,                               bool,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                      imprimir_cenario_hidrologico_pre_otimizacao,                               bool,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                     calcular_custo_primal_via_subproblema_mestre,                               bool,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                      calcular_cenario_hidrologico_pre_otimizacao,                               bool,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                maior_estagio_impressao_versao_alternativa_cortes,                          IdEstagio,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                   estagio_acoplamento_pre_estudo,                          IdEstagio,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                                  estagio_inicial,                          IdEstagio,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                                    estagio_final,                          IdEstagio,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                       periodo_otimizacao_inicial,                            Periodo,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                         periodo_otimizacao_final,                            Periodo,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                           periodo_estudo_inicial,                            Periodo,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                             periodo_estudo_final,                            Periodo,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                            tipo_processo_estocastico_hidrologico,              IdProcessoEstocastico,        Nenhum,       max,        Nenhum,   sim) \
	  m(ModeloOtimizacao,  AttComum,         relaxar_afluencia_incremental_com_viabilidade_hidraulica,                               bool,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                tipo_funcao_producao_hidreletrica,     TipoFuncaoProducaoHidreletrica,        Nenhum,       max,        Nenhum,   sim) 

#define ATT_VETOR_MODELO_OTIMIZACAO(m)  \
	  m(ModeloOtimizacao,  AttVetor,  alguma_variavel_aleatoria_hidrologica_com_truncamento,         int,        0,          1,           0,  IdEstagio) \
	  m(ModeloOtimizacao,  AttVetor,           custo_total,         double,        0,          max,           0,  IdEstagio) \
	  m(ModeloOtimizacao,  AttVetor,        custo_imediato,         double,        0,          max,           0,  IdEstagio) \
	  m(ModeloOtimizacao,  AttVetor,          custo_futuro,         double,        0,          max,           0,  IdEstagio) \
	  m(ModeloOtimizacao,  AttVetor,      horizonte_estudo,      IdEstagio,        0,          max,           0,  Periodo) \
	  m(ModeloOtimizacao,  AttVetor, tratamento_inviabilidade,         int,       -1,          max,           0,  IdEstagio)

#define ATT_MATRIZ_MODELO_OTIMIZACAO(m)  \
	  m(ModeloOtimizacao,  AttMatriz,  percentual_duracao_patamar_carga,                               double,          0,            1,             0,    Periodo,   IdPatamarCarga) \
	  m(ModeloOtimizacao,  AttMatriz,  horizonte_espaco_amostral_hidrologico,                             int,          1,          max,             1,  IdEstagio,  Periodo) \
	  m(ModeloOtimizacao,  AttMatriz,  horizonte_estudo,                                               double,        min,          max,           min,  IdEstagio,  Periodo) \
	  m(ModeloOtimizacao,  AttMatriz,  custo_medio,                                                    double,        min,          max,           min,  IdEstagio,  IdRealizacao) \
	  m(ModeloOtimizacao,  AttMatriz,  realizacao_solucao_proxy,                                          int,        min,          max,           min,  IdEstagio,  IdRealizacao)  

#define MEMBRO_MODELO_OTIMIZACAO(m)         \
	m(ModeloOtimizacao, Estagio)            \
	m(ModeloOtimizacao, ArranjoResolucao)    \
	m(ModeloOtimizacao, ProcessoEstocastico) 

#define SMART_ELEMENTO_MODELO_OTIMIZACAO(m) \
	m(ModeloOtimizacao, AttComum,  ATT_COMUM_MODELO_OTIMIZACAO) \
	m(ModeloOtimizacao, AttVetor,  ATT_VETOR_MODELO_OTIMIZACAO) \
	m(ModeloOtimizacao, AttMatriz, ATT_MATRIZ_MODELO_OTIMIZACAO) \
	m(ModeloOtimizacao, Membro, MEMBRO_MODELO_OTIMIZACAO)

DEFINE_SMART_ELEMENTO(ModeloOtimizacao, SMART_ELEMENTO_MODELO_OTIMIZACAO)

//
// ITERADORES 2
//

#define ITERADORES_2_ESTAGIO_PERIODO(m)         m(1, IdEstagio) m(2, Periodo)
#define ITERADORES_2_ESTAGIO_VARIAVELESTADO(m)  m(1, IdEstagio) m(2, IdVariavelEstado)

#define VARIAVEL_DECISAO_2(m)\
	m(  VarDecisao,     ZP0,                      2,    ITERADORES_2_ESTAGIO_PERIODO,            sim,          nao,                nao)  \
	m(  VarDecisao,     ZP0p,                     2,    ITERADORES_2_ESTAGIO_PERIODO,            sim,          nao,                nao)  \
	m(  VarDecisao,     ZPp,                      2,    ITERADORES_2_ESTAGIO_PERIODO,            sim,          nao,                nao)  \
	m(  VarDecisao,     ZO0,                      2,    ITERADORES_2_ESTAGIO_PERIODO,            sim,          nao,                nao)  \
	m(  VarDecisao,     ZO0p,                     2,    ITERADORES_2_ESTAGIO_PERIODO,            sim,          nao,                nao)  \
	m(  VarDecisao,     ZOp,                      2,    ITERADORES_2_ESTAGIO_PERIODO,            sim,          nao,                nao)  \
	m(  VarDecisao,     ZO0_UTE,                  2,    ITERADORES_2_ESTAGIO_PERIODO,            sim,          nao,                nao)  \
	m(  VarDecisao,     ZO0_DEFICIT,              2,    ITERADORES_2_ESTAGIO_PERIODO,            sim,          nao,                nao)  \
	m(  VarDecisao,     ZT,                       2,    ITERADORES_2_ESTAGIO_PERIODO,            sim,          nao,                nao)  \
	m(  VarDecisao,     ZI,                       2,    ITERADORES_2_ESTAGIO_PERIODO,            sim,          nao,                nao)  \
	m(  VarDecisao,     ZP0_VF_FINF,              2,    ITERADORES_2_ESTAGIO_PERIODO,            sim,          nao,                nao)  \
	m(  VarDecisao,     ZP0_VF_FINF_ADD,          2,    ITERADORES_2_ESTAGIO_PERIODO,            sim,          nao,                nao)  \
	m(  VarDecisao,     ZP,                       2,    ITERADORES_2_ESTAGIO_PERIODO,            sim,          nao,                nao)  \
	m(  VarDecisao,     ESTADO,                   2,    ITERADORES_2_ESTAGIO_VARIAVELESTADO,     sim,          nao,                nao)  \
	m(  VarDecisao,     ZF,                       2,    ITERADORES_2_ESTAGIO_PERIODO,            sim,          nao,                nao)  
   //   Elemento,       Nome,               NroIter,    Iteradores,                      Impr.Primal,     Impr.Dual              NormDual

#define EQUACAO_LINEAR_2(m)\
	m(  EquLinear,     CUSTO_PENALIDADE_0,                  2,    ITERADORES_2_ESTAGIO_PERIODO,       nao,         nao,                nao)  \
	m(  EquLinear,     CUSTO_PENALIDADE_0p,                 2,    ITERADORES_2_ESTAGIO_PERIODO,       nao,         nao,                nao)  \
	m(  EquLinear,     CUSTO_PENALIDADE_p,                  2,    ITERADORES_2_ESTAGIO_PERIODO,       nao,         nao,                nao)  \
	m(  EquLinear,     CUSTO_OPERACAO_0,                    2,    ITERADORES_2_ESTAGIO_PERIODO,       nao,         nao,                nao)  \
	m(  EquLinear,     CUSTO_OPERACAO_0p,                   2,    ITERADORES_2_ESTAGIO_PERIODO,       nao,         nao,                nao)  \
	m(  EquLinear,     CUSTO_OPERACAO_p,                    2,    ITERADORES_2_ESTAGIO_PERIODO,       nao,         nao,                nao)  \
	m(  EquLinear,     CUSTO_TOTAL,                         2,    ITERADORES_2_ESTAGIO_PERIODO,       nao,         nao,                nao)  \
	m(  EquLinear,     CUSTO_TOTAL_MESTRE,                  2,    ITERADORES_2_ESTAGIO_PERIODO,       nao,         nao,                nao)  \
	m(  EquLinear,     CUSTO_IMEDIATO,                      2,    ITERADORES_2_ESTAGIO_PERIODO,       nao,         nao,                nao)  \
	m(  EquLinear,     CUSTO_PENALIDADE_VOLUME_INFERIOR_0,  2,    ITERADORES_2_ESTAGIO_PERIODO,       nao,         nao,                nao)  \
	m(  EquLinear,     CUSTO_PENALIDADE,                    2,    ITERADORES_2_ESTAGIO_PERIODO,       nao,         nao,                nao)  \
	m(  EquLinear,     CUSTO_FUTURO,                        2,    ITERADORES_2_ESTAGIO_PERIODO,       nao,         nao,                nao)  
   //   Elemento,     Nome,                           NroIter,    Iteradores,                 Impr.Primal,    Impr.Dual              NormDual

//
// ITERADORES 3
//

#define ITERADORES_3_ESTAGIO_PERIODO_PERIODO(m)               m(1, IdEstagio) m(2, Periodo)        m(3, Periodo)
#define ITERADORES_3_ESTAGIO_PERIODO_PATAMARCARGA(m)          m(1, IdEstagio) m(2, Periodo)        m(3, IdPatamarCarga)
#define ITERADORES_3_ESTAGIO_PERIODO_REALIZACAO(m)            m(1, IdEstagio) m(2, Periodo)        m(3, IdRealizacao)
#define ITERADORES_3_ESTAGIO_PERIODO_TERMELETRICA(m)          m(1, IdEstagio) m(2, Periodo)        m(3, IdTermeletrica)
#define ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA(m)          m(1, IdEstagio) m(2, Periodo)        m(3, IdHidreletrica)
#define ITERADORES_3_ESTAGIO_HIDRELETRICA_PERIODO(m)          m(1, IdEstagio) m(2, IdHidreletrica) m(3, Periodo)
#define ITERADORES_3_ESTAGIO_TERMELETRICA_PERIODO(m)          m(1, IdEstagio) m(2, IdTermeletrica) m(3, Periodo)
#define ITERADORES_3_ESTAGIO_PERIODO_SUBMERCADO(m)            m(1, IdEstagio) m(2, Periodo)        m(3, IdSubmercado)
#define ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE(m) m(1, IdEstagio) m(2, Periodo)        m(3, IdRestricaoOperativaUHE)
#define ITERADORES_3_ESTAGIO_PERIODO_USINAELEVATORIA(m)       m(1, IdEstagio) m(2, Periodo)        m(3, IdUsinaElevatoria)
#define ITERADORES_3_ESTAGIO_PERIODO_VARIAVELALEATORIA(m)     m(1, IdEstagio) m(2, Periodo)        m(3, IdVariavelAleatoria)

#define VARIAVEL_DECISAO_3(m)\
	m(  VarDecisao,   ZP,               3,         ITERADORES_3_ESTAGIO_PERIODO_PATAMARCARGA,              sim,          nao,                nao)  \
	m(  VarDecisao,   ZO,               3,         ITERADORES_3_ESTAGIO_PERIODO_PATAMARCARGA,              sim,          nao,                nao)  \
	m(  VarDecisao,   ZF,               3,         ITERADORES_3_ESTAGIO_PERIODO_REALIZACAO,                sim,          nao,                nao)  \
	m(  VarDecisao,   ZT,               3,         ITERADORES_3_ESTAGIO_PERIODO_REALIZACAO,                sim,          nao,                nao)  \
	m(  VarDecisao,   ZO0_UTE,          3,         ITERADORES_3_ESTAGIO_PERIODO_TERMELETRICA,              sim,          nao,                nao)  \
	m(  VarDecisao,   ZOp_UTE,          3,         ITERADORES_3_ESTAGIO_PERIODO_TERMELETRICA,              sim,          nao,                nao)  \
	m(  VarDecisao,   ZO0p_UTE,         3,         ITERADORES_3_ESTAGIO_PERIODO_TERMELETRICA,              sim,          nao,                nao)  \
	m(  VarDecisao,   ZO0_DEFICIT,      3,         ITERADORES_3_ESTAGIO_PERIODO_TERMELETRICA,              sim,          nao,                nao)  \
	m(  VarDecisao,   ZO0p_DEFICIT,     3,         ITERADORES_3_ESTAGIO_PERIODO_TERMELETRICA,              sim,          nao,                nao)  \
	m(  VarDecisao,   ZOp_DEFICIT,      3,         ITERADORES_3_ESTAGIO_PERIODO_TERMELETRICA,              sim,          nao,                nao)  \
	m(  VarDecisao,   ZP0_VF_FINF,      3,         ITERADORES_3_ESTAGIO_PERIODO_PERIODO,                   sim,          nao,                nao)  \
	m(  VarDecisao,   YH,               3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,              sim,          nao,                nao)  \
	m(  VarDecisao,   YHF,              3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,              sim,          nao,                nao)  \
	m(  VarDecisao,   ENA,              3,         ITERADORES_3_ESTAGIO_PERIODO_PERIODO,                   sim,          nao,                nao)  \
	m(  VarDecisao,   QEV,              3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,              sim,          nao,                nao)  \
	m(  VarDecisao,   QEV_REL,          3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,              sim,          nao,                nao)  \
	m(  VarDecisao,   QEV_FINF,         3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,              sim,          nao,                nao)  \
    m(  VarDecisao,   QDEF,             3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,              sim,          nao,                nao)  \
    m(  VarDecisao,   QDEF_FINF,        3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,              sim,          nao,                nao)  \
    m(  VarDecisao,   QDEF_FSUP,        3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,              sim,          nao,                nao)  \
	m(  VarDecisao,   VI,               3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,              sim,          sim,                nao)  \
	m(  VarDecisao,   VF,               3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,              sim,          nao,                nao)  \
	m(  VarDecisao,   VMED,             3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,              sim,          nao,                nao)  \
    m(  VarDecisao,   VF_FINF,          3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,              sim,          nao,                nao)  \
	m(  VarDecisao,   VMORTO_FINF,      3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,              sim,          nao,                nao)  \
	m(  VarDecisao,   PL,               3,         ITERADORES_3_ESTAGIO_PERIODO_SUBMERCADO,                sim,          nao,                nao)  \
	m(  VarDecisao,   PBOMDISP,         3,         ITERADORES_3_ESTAGIO_PERIODO_USINAELEVATORIA,           sim,          nao,                nao)  \
	m(  VarDecisao,   PD,               3,         ITERADORES_3_ESTAGIO_PERIODO_SUBMERCADO,                sim,          nao,                nao)  \
	m(  VarDecisao,   PHDISP,           3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,              sim,          nao,                nao)  \
    m(  VarDecisao,   PHDISPMETA,       3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,              sim,          nao,                nao)  \
    m(  VarDecisao,   PHDISPMETA_FINF,  3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,              sim,          nao,                nao)  \
    m(  VarDecisao,   PHDISPMETA_FSUP,  3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,              sim,          nao,                nao)  \
	m(  VarDecisao,   QVER,             3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,              sim,          nao,                nao)  \
	m(  VarDecisao,   QTURDISP,         3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,              sim,          nao,                nao)  \
	m(  VarDecisao,   PTDISP,           3,         ITERADORES_3_ESTAGIO_PERIODO_TERMELETRICA,              sim,          nao,                nao)  \
	m(  VarDecisao,   PTDISPCOM,        3,         ITERADORES_3_ESTAGIO_PERIODO_TERMELETRICA,              sim,          nao,                nao)  \
	m(  VarDecisao,   PI,               3,         ITERADORES_3_ESTAGIO_PERIODO_SUBMERCADO,                sim,          nao,                nao)  \
	m(  VarDecisao,   PN,               3,         ITERADORES_3_ESTAGIO_PERIODO_SUBMERCADO,                sim,          nao,                nao)  \
    m(  VarDecisao,   RHA,              3,         ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE,     sim,          nao,                nao)  \
    m(  VarDecisao,   RHA_FINF,         3,         ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE,     sim,          nao,                nao)  \
    m(  VarDecisao,   RHA_FSUP,         3,         ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE,     sim,          nao,                nao)  \
    m(  VarDecisao,   RHV,              3,         ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE,     sim,          nao,                nao)  \
    m(  VarDecisao,   RHV_FINF,         3,         ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE,     sim,          nao,                nao)  \
    m(  VarDecisao,   RHV_FSUP,         3,         ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE,     sim,          nao,                nao)  \
    m(  VarDecisao,   RHE_FINF,         3,         ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE,     sim,          nao,                nao)  \
    m(  VarDecisao,   RHE_FSUP,         3,         ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE,     sim,          nao,                nao)
   //   Elemento,     Nome,       NroIter,         Iteradores,                                     Impr.Primal,     Impr.Dual              NormDual

#define EQUACAO_LINEAR_3(m)\
	m(  EquLinear,   ENA,                                            3,   ITERADORES_3_ESTAGIO_PERIODO_PERIODO,            nao,  nao,                nao)  \
	m(  EquLinear,   CUSTO_PENALIDADE,                               3,   ITERADORES_3_ESTAGIO_PERIODO_PATAMARCARGA,       nao,  nao,                nao)  \
	m(  EquLinear,   CUSTO_OPERACAO,                                 3,   ITERADORES_3_ESTAGIO_PERIODO_PATAMARCARGA,       nao,  nao,                nao)  \
	m(  EquLinear,   CUSTO_PENALIDADE_VOLUME_INFERIOR_0,             3,   ITERADORES_3_ESTAGIO_PERIODO_PERIODO,            nao,  nao,                nao)  \
	m(  EquLinear,   VOLUME_MEDIO,                                   3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,       nao,  nao,                nao)  \
	m(  EquLinear,   AFLUENCIA_INCREMENTAL,                          3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,       nao,  nao,                nao)  \
	m(  EquLinear,   DEMANDA,                                        3,   ITERADORES_3_ESTAGIO_PERIODO_SUBMERCADO,         nao,  nao,                nao)  \
	m(  EquLinear,   DEFICIT,                                        3,   ITERADORES_3_ESTAGIO_PERIODO_SUBMERCADO,         nao,  nao,                nao)  \
	m(  EquLinear,   INTERCAMBIO,                                    3,   ITERADORES_3_ESTAGIO_PERIODO_SUBMERCADO,         nao,  nao,                nao)  \
	m(  EquLinear,   POTENCIA_BOMBEAMENTO_DISPONIVEL,                3,   ITERADORES_3_ESTAGIO_PERIODO_USINAELEVATORIA,    nao,  nao,                nao)  \
	m(  EquLinear,   POTENCIA_HIDRAULICA_DISPONIVEL,                 3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,       nao,  nao,                nao)  \
	m(  EquLinear,   POTENCIA_TERMICA_DISPONIVEL,                    3,   ITERADORES_3_ESTAGIO_PERIODO_TERMELETRICA,       nao,  nao,                nao)  \
	m(  EquLinear,   POTENCIA_TERMICA_DISPONIVEL_COMANDADA,          3,   ITERADORES_3_ESTAGIO_PERIODO_TERMELETRICA,       nao,  nao,                nao)  \
	m(  EquLinear,   PRODUCAO_USINA_NAO_SIMULADA,                    3,   ITERADORES_3_ESTAGIO_PERIODO_SUBMERCADO,         nao,  nao,                nao)  \
	m(  EquLinear,   VAZAO_EVAPORADA,                                3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,       nao,  nao,                nao)  \
	m(  EquLinear,   VAZAO_EVAPORADA_RELAXADA,                       3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,       nao,  nao,                nao)  \
	m(  EquLinear,   VAZAO_EVAPORADA_DISPONIVEL,                     3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,       nao,  nao,                nao)  \
	m(  EquLinear,   VAZAO_TURBINADA_DISPONIVEL,                     3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,       nao,  nao,                nao)  \
	m(  EquLinear,   VAZAO_DEFLUENTE,                                3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,       nao,  nao,                nao)  \
	m(  EquLinear,   VAZAO_VERTIDA,                                  3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,       nao,  nao,                nao)  \
    m(  EquLinear,   POTENCIA_DISPONIVEL_META,                       3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,       nao,  nao,                nao)  \
    m(  EquLinear,   AFLUENCIA_PROCESSO_ESTOCASTICO_ACUMULADA,       3,   ITERADORES_3_ESTAGIO_PERIODO_VARIAVELALEATORIA,  nao,  nao,                nao)
	//  Elemento,    Nome,                                     NroIter,   Iteradores,                              Impr.Primal,  Impr.Dual         NormDual

#define INEQUACAO_LINEAR_3(m)\
    m(  IneLinear,   RESTRICAO_HIDRAULICA_VOLUME,                    3,   ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE,     nao,  nao,                nao)  \
    m(  IneLinear,   RESTRICAO_HIDRAULICA_VOLUME_LIMITE_INFERIOR,    3,   ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE,     nao,  nao,                nao)  \
    m(  IneLinear,   RESTRICAO_HIDRAULICA_VOLUME_LIMITE_SUPERIOR,    3,   ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE,     nao,  nao,                nao)  \
    m(  IneLinear,   RESTRICAO_HIDRAULICA_ENERGIA_LIMITE_INFERIOR,   3,   ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE,     nao,  nao,                nao)  \
    m(  IneLinear,   RESTRICAO_HIDRAULICA_ENERGIA_LIMITE_SUPERIOR,   3,   ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE,     nao,  nao,                nao)  \
    m(  IneLinear,   RESTRICAO_HIDRAULICA_AFLUENTE_LIMITE_INFERIOR,  3,   ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE,     nao,  nao,                nao)  \
    m(  IneLinear,   RESTRICAO_HIDRAULICA_AFLUENTE_LIMITE_SUPERIOR,  3,   ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE,     nao,  nao,                nao)  \
    m(  IneLinear,   RESTRICAO_HIDRAULICA_AFLUENTE,                  3,   ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE,     nao,  nao,                nao)  \
	m(  IneLinear,   VAZAO_DEFLUENTE_FOLGA_INFERIOR,                 3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,              nao,  nao,                nao)  \
    m(  IneLinear,   VAZAO_DEFLUENTE_FOLGA_SUPERIOR,                 3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,              nao,  nao,                nao)  \
    m(  IneLinear,   VOLUME_UTIL_FOLGA_INFERIOR,                     3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA,              nao,  nao,                nao)  
   //   Elemento,    Nome,                                     NroIter,   Iteradores,                                     Impr.Primal,  Impr.Dual          NormDual

//
// ITERADORES 4
//

#define ITERADORES_4_ESTAGIO_PERIODO_PERIODO_REE(m)                           m(1, IdEstagio) m(2, Periodo) m(3, Periodo) m(4, IdReservatorioEquivalente)
#define ITERADORES_4_ESTAGIO_PERIODO_PERIODO_HIDRELETRICA(m)                  m(1, IdEstagio) m(2, Periodo) m(3, Periodo) m(4, IdHidreletrica)
#define ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_SUBMERCADO(m)               m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdSubmercado)
#define ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA(m)             m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdHidreletrica)
#define ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA(m)             m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdTermeletrica)
#define ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_USINAELEVATORIA(m)          m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdUsinaElevatoria)
#define ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_USINAEOLICA(m)              m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdUsinaEolica)
#define ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_CONTRATO(m)                 m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdContrato)
#define ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_RESTRICAOELETRICA(m)        m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdRestricaoEletrica)
#define ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_AGRUPAMENTOINTERCAMBIO(m)   m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdAgrupamentoIntercambio)
#define ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_INTERCAMBIO(m)              m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdIntercambio)
#define ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_DEMANDAESPECIAL(m)          m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdDemandaEspecial)
#define ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_RESTRICAOOPERATIVAUHE(m)    m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdRestricaoOperativaUHE)
#define ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_INTERCAMBIOHIDRAULICO(m)    m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdIntercambioHidraulico)
#define ITERADORES_4_ESTAGIO_PERIODO_REALIZACAO_CORTEBENDERS(m)               m(1, IdEstagio) m(2, Periodo) m(3, IdRealizacao)   m(4, IdCorteBenders)
#define ITERADORES_4_ESTAGIO_PERIODO_PROCESSOESTOCASTICO_VARIAVELALEATORIA(m) m(1, IdEstagio) m(2, Periodo) m(3, IdProcessoEstocastico) m(4, IdVariavelAleatoria)
#define ITERADORES_4_ESTAGIO_PROCESSOESTOCASTICO_VARIAVELALEATORIA_PERIODO(m) m(1, IdEstagio) m(2, IdProcessoEstocastico) m(3, IdVariavelAleatoria) m(4, Periodo)
#define ITERADORES_4_ESTAGIO_TERMELETRICA_INT_PERIODO(m)                      m(1, IdEstagio) m(2, IdTermeletrica) m(3, int) m(4, Periodo)


#define VARIAVEL_DECISAO_4(m)\
	m(  VarDecisao,    ENA,                    4,    ITERADORES_4_ESTAGIO_PERIODO_PERIODO_REE,                           sim,          nao,                nao)  \
	m(  VarDecisao,    PH,                     4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    PH_REL,                 4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    PH_FINF,                4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    PHDISP,                 4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    PHDISP_REL,             4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    PHDISP_FINF,            4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             sim,          nao,                nao)  \
    m(  VarDecisao,    QBOM,                   4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_USINAELEVATORIA,          sim,          nao,                nao)  \
    m(  VarDecisao,    QBOM_FINF,              4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_USINAELEVATORIA,          sim,          nao,                nao)  \
    m(  VarDecisao,    QBOMDISP,               4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_USINAELEVATORIA,          sim,          nao,                nao)  \
    m(  VarDecisao,    QBOMDISP_FINF,          4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_USINAELEVATORIA,          sim,          nao,                nao)  \
    m(  VarDecisao,    QTURDISP,               4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             sim,          nao,                nao)  \
    m(  VarDecisao,    QTURDISP_FINF,          4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             sim,          nao,                nao)  \
    m(  VarDecisao,    QTUR,                   4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             sim,          nao,                nao)  \
    m(  VarDecisao,    QTUR_FINF,              4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    QVER,                   4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    QMON,                   4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             sim,          nao,                nao)  \
    m(  VarDecisao,    QDEFLAG,                4,    ITERADORES_4_ESTAGIO_PERIODO_PERIODO_HIDRELETRICA,                  sim,          nao,                nao)  \
    m(  VarDecisao,    QDEFLAG_ADD,            4,    ITERADORES_4_ESTAGIO_PERIODO_PERIODO_HIDRELETRICA,                  sim,          nao,                nao)  \
    m(  VarDecisao,    QDEF,                   4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             sim,          nao,                nao)  \
    m(  VarDecisao,    QDEF_FINF,              4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             sim,          nao,                nao)  \
    m(  VarDecisao,    QDEF_FSUP,              4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    PL,                     4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_SUBMERCADO,               sim,          sim,                sim)  \
	m(  VarDecisao,    DE,			           4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_DEMANDAESPECIAL,          sim,          sim,                nao)  \
	m(  VarDecisao,    CIMP,		           4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_CONTRATO,                 sim,          sim,                nao)  \
	m(  VarDecisao,    CEXP,		           4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_CONTRATO,                 sim,          sim,                nao)  \
	m(  VarDecisao,    PBOMDISP,               4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_USINAELEVATORIA,          sim,          sim,                nao)  \
	m(  VarDecisao,    RE_FINF,                4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_RESTRICAOELETRICA,        sim,          nao,                nao)  \
    m(  VarDecisao,    RE_FSUP,                4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_RESTRICAOELETRICA,        sim,          nao,                nao)  \
	m(  VarDecisao,    PI,                     4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_INTERCAMBIO,              sim,          sim,                nao)  \
    m(  VarDecisao,    PI_FINF,                4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_INTERCAMBIO,              sim,          nao,                nao)  \
	m(  VarDecisao,    PT,                     4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    PE,			           4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_USINAEOLICA,              sim,          nao,                nao)  \
	m(  VarDecisao,    PT_UTIL,                4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    PT_MIN,                 4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    LIG_T,                  4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    DESL_T,                 4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    ACIO_T,                 4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    PT_REL,                 4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    PT_FINF,                4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    PT_FSUP,                4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    PTDISP,                 4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    PTDISP_REL,             4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    PTDISP_FINF,            4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    PTDISP_FSUP,            4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    PTDISPCOM,              4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    PTDISPPRECOM,           4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    PTDISPCOM_FINF,         4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             sim,          nao,                nao)  \
	m(  VarDecisao,    PTDISPCOM_FSUP,         4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             sim,          nao,                nao)  \
    m(  VarDecisao,    RHQ,                    4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_RESTRICAOOPERATIVAUHE,    sim,          nao,                nao)  \
    m(  VarDecisao,    RHQ_FINF,               4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_RESTRICAOOPERATIVAUHE,    sim,          nao,                nao)  \
    m(  VarDecisao,    RHQ_FSUP,               4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_RESTRICAOOPERATIVAUHE,    sim,          nao,                nao)  \
	m(  VarDecisao,    QILS_TRI,               4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_INTERCAMBIOHIDRAULICO,    sim,          nao,                nao)  \
    m(  VarDecisao,    QILS_TRI_FINF,          4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_INTERCAMBIOHIDRAULICO,    sim,          nao,                nao)  \
    m(  VarDecisao,    QILS_TRI_REL,           4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_INTERCAMBIOHIDRAULICO,    sim,          nao,                nao)  \
    m(  VarDecisao,    QOUT,                   4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             sim,          nao,                nao)  \
    m(  VarDecisao,    QOUT_FINF,              4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             sim,          nao,                nao)
   //   Elemento,      Nome,             NroIter,    Iteradores,                                                 Impr.Primal,     Impr.Dual              NormDual

#define EQUACAO_LINEAR_4(m)\
	m(  EquLinear,    ENA,                                               4,    ITERADORES_4_ESTAGIO_PERIODO_PERIODO_REE,                           nao,         nao,                nao)  \
	m(  EquLinear,    ATENDIMENTO_DEMANDA,                               4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_SUBMERCADO,               nao,         nao,                nao)  \
	m(  EquLinear,    BALANCO_HIDRAULICO_VOL,                            4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             nao,         nao,                nao)  \
	m(  EquLinear,    BALANCO_HIDRAULICO_VAZ,                            4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             nao,         nao,                nao)  \
	m(  EquLinear,    VAZAO_MONTANTE,                                    4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             nao,         nao,                nao)  \
    m(  EquLinear,    VAZAO_DEFLUENTE,                                   4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             nao,         nao,                nao)  \
    m(  EquLinear,    VAZAO_DEFLUENTE_LAG,                               4,    ITERADORES_4_ESTAGIO_PERIODO_PERIODO_HIDRELETRICA,                  nao,         nao,                nao)  \
	m(  EquLinear,    VAZAO_TURBINADA,                                   4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             nao,         nao,                nao)  \
	m(  EquLinear,    VAZAO_TURBINADA_DISPONIVEL,                        4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             nao,         nao,                nao)  \
	m(  EquLinear,    POTENCIA_TERMICA_RELAXADA,                         4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             nao,         nao,                nao)  \
	m(  EquLinear,    POTENCIA_TERMICA_DISPONIVEL,                       4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             nao,         nao,                nao)  \
	m(  EquLinear,    POTENCIA_TERMICA_DISPONIVEL_COMANDADA,             4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             nao,         nao,                nao)  \
	m(  EquLinear,    POTENCIA_TERMICA_DISPONIVEL_COMANDADA_RELAXADA,    4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             nao,         nao,                nao)  \
	m(  EquLinear,    POTENCIA_TERMICA,                                  4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             nao,         nao,                nao)  \
	m(  EquLinear,    RELACAO_VARIAVEIS_BINARIAS_TERMICA,                4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             nao,         nao,                nao)  \
    m(  EquLinear,    POTENCIA_TERMICA_DISPONIVEL_RELAXADA,              4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,             nao,         nao,                nao)  \
	m(  EquLinear,    POTENCIA_BOMBEAMENTO_DISPONIVEL,                   4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_USINAELEVATORIA,          nao,         nao,                nao)  \
	m(  EquLinear,    POTENCIA_HIDRAULICA,                               4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             nao,         nao,                nao)  \
	m(  EquLinear,    POTENCIA_HIDRAULICA_RELAXADA,                      4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             nao,         nao,                nao)  \
	m(  EquLinear,    POTENCIA_HIDRAULICA_DISPONIVEL,                    4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             nao,         nao,                nao)  \
	m(  EquLinear,    POTENCIA_HIDRAULICA_DISPONIVEL_RELAXADA,           4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             nao,         nao,                nao)  \
    m(  EquLinear,    INTERCAMBIO_HIDRAULICO_RETIRADA_RELAXADA,          4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_INTERCAMBIOHIDRAULICO,    nao,         nao,                nao)  \
    m(  EquLinear,    VAZAO_RETIRADA_FOLGA_INFERIOR,                     4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,             nao,         nao,                nao)  \
    m(  EquLinear,    VAZAO_BOMBEADA_DISPONIVEL,                         4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_USINAELEVATORIA,          nao,         nao,                nao)
   //   Elemento,     Nome,                                        NroIter,    Iteradores,                                                 Impr.Primal,    Impr.Dual              NormDual

#define INEQUACAO_LINEAR_4(m)\
	m(  IneLinear,    CB_ZF,                                               4,    ITERADORES_4_ESTAGIO_PERIODO_REALIZACAO_CORTEBENDERS,             nao,         nao,                nao)  \
	m(  IneLinear,    CB_ZT,                                               4,    ITERADORES_4_ESTAGIO_PERIODO_REALIZACAO_CORTEBENDERS,             nao,         nao,                nao)  \
	m(  IneLinear,    RESTRICAO_ELETRICA_LIMITE_INFERIOR,                  4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_RESTRICAOELETRICA,      nao,         nao,                nao)  \
    m(  IneLinear,    RESTRICAO_ELETRICA_LIMITE_SUPERIOR,                  4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_RESTRICAOELETRICA,      nao,         nao,                nao)  \
	m(  IneLinear,    AGRUPAMENTO_INTERCAMBIO,                             4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_AGRUPAMENTOINTERCAMBIO, nao,         nao,                nao)  \
    m(  IneLinear,    VOLUME_MINIMO_PENALIZADO,                            4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,           nao,         nao,                nao)  \
	m(  IneLinear,    META_ENCHENDO_VOLUME_MORTO,                          4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,           nao,         nao,                nao)  \
	m(  IneLinear,    VAZAO_DEFLUENTE_FOLGA_INFERIOR,                      4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,           nao,         nao,                nao)  \
	m(  IneLinear,    VAZAO_DEFLUENTE_FOLGA_SUPERIOR,                      4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,           nao,         nao,                nao)  \
	m(  IneLinear,    RELACAO_UNIDADES_EQUIVALENTE_TERMICA,                4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,           nao,         nao,                nao)  \
    m(  IneLinear,    RESTRICAO_HIDRAULICA_DEFLUENCIA_LIMITE_INFERIOR,     4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_RESTRICAOOPERATIVAUHE,  nao,         nao,                nao)  \
    m(  IneLinear,    RESTRICAO_HIDRAULICA_DEFLUENCIA_LIMITE_SUPERIOR,     4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_RESTRICAOOPERATIVAUHE,  nao,         nao,                nao)  \
	m(  IneLinear,    POTENCIA_TERMELETRICA_UTIL,                          4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,           nao,         nao,                nao)  \
	m(  IneLinear,    POTENCIA_TERMELETRICA_PARTIDA,                       4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,           nao,         nao,                nao)  \
	m(  IneLinear,    POTENCIA_TERMELETRICA_DESLIGAMENTO,                  4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,           nao,         nao,                nao)  \
	m(  IneLinear,    POTENCIA_TERMICA_DISPONIVEL_FINF,                    4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,           nao,         nao,                nao)  \
	m(  IneLinear,    POTENCIA_TERMICA_DISPONIVEL_FSUP,                    4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA,           nao,         nao,                nao)  \
    m(  IneLinear,    RESTRICAO_HIDRAULICA_DEFLUENCIA,                     4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_RESTRICAOOPERATIVAUHE,  nao,         nao,                nao)  \
    m(  IneLinear,    VAZAO_BOMBEADA_FOLGA_INFERIOR,                       4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_USINAELEVATORIA,        nao,         nao,                nao)  \
    m(  IneLinear,    VAZAO_BOMBEADA_DISPONIVEL_FOLGA_INFERIOR,            4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_USINAELEVATORIA,        nao,         nao,                nao)  \
    m(  IneLinear,    VAZAO_TURBINADA_DISPONIVEL_RELAXADA,                 4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,           nao,         nao,                nao)  \
    m(  IneLinear,    VAZAO_TURBINADA_RELAXADA,                            4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA,           nao,         nao,                nao)  \
    m(  IneLinear,    INTERCAMBIO_RELAXADO,                                4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_INTERCAMBIO,            nao,         nao,                nao)
   //   Elemento,     Nome,                                          NroIter,    Iteradores,                                               Impr.Primal,    Impr.Dual              NormDual


//
// ITERADORES 5
//

#define ITERADORES_5_ESTAGIO_PERIODO_PERIODO_HIDRELETRICA_REE(m)                        m(1, IdEstagio) m(2, Periodo) m(3, Periodo) m(4, IdHidreletrica) m(5, IdReservatorioEquivalente) 
#define ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_SUBMERCADO_USINANAOSIMULADA(m)        m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdSubmercado) m(5, IdUsinaNaoSimulada)
#define ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_SUBMERCADO_PATAMARDEFICIT(m)          m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdSubmercado) m(5, IdPatamarDeficit)
#define ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_INT(m)                   m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdHidreletrica) m(5, int)
#define ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_HIDRELETRICA(m)          m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdHidreletrica) m(5, IdHidreletrica)
#define ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO(m)    m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdHidreletrica) m(5, IdConjuntoHidraulico)
#define ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE(m)            m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdTermeletrica) m(5, IdUnidadeUTE)
#define ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_INTERCAMBIOHIDRAULICO_HIDRELETRICA(m) m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdIntercambioHidraulico) m(5, IdHidreletrica)
#define ITERADORES_5_ESTAGIO_PERIODO_PERIODO_PROCESSOESTOCASTICO_VARIAVELALEATORIA(m)   m(1, IdEstagio) m(2, Periodo) m(3, Periodo) m(4, IdProcessoEstocastico) m(5, IdVariavelAleatoria)

#define VARIAVEL_DECISAO_5(m)\
	m(  VarDecisao,    ENA,         5,     ITERADORES_5_ESTAGIO_PERIODO_PERIODO_HIDRELETRICA_REE,                         sim,          nao,                nao)  \
	m(  VarDecisao,    PH,          5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO,     sim,          nao,                nao)  \
	m(  VarDecisao,    PH_REL,      5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO,     sim,          nao,                nao)  \
	m(  VarDecisao,    PH_FINF,     5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO,     sim,          nao,                nao)  \
	m(  VarDecisao,    PHDISP,      5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO,     sim,          nao,                nao)  \
	m(  VarDecisao,    PHDISP_REL,  5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO,     sim,          nao,                nao)  \
	m(  VarDecisao,    PHDISP_FINF, 5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO,     sim,          nao,                nao)  \
    m(  VarDecisao,    QTURDISP,    5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO,     sim,          nao,                nao)  \
    m(  VarDecisao,    QTUR,        5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO,     sim,          nao,                nao)  \
    m(  VarDecisao,    QTUR_FINF,   5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO,     sim,          nao,                nao)  \
	m(  VarDecisao,    PN,          5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_SUBMERCADO_USINANAOSIMULADA,         sim,          nao,                nao)  \
	m(  VarDecisao,    PT,          5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE,             sim,          nao,                nao)  \
	m(  VarDecisao,    PT_UTIL,     5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE,             sim,          nao,                nao)  \
	m(  VarDecisao,    PT_TRAJ_ACIO,5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE,             sim,          nao,                nao)  \
	m(  VarDecisao,    PT_TRAJ_DESL,5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE,             sim,          nao,                nao)  \
	m(  VarDecisao,    PT_MIN,      5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE,             sim,          nao,                nao)  \
	m(  VarDecisao,    LIG_T,       5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE,             sim,          nao,                nao)  \
	m(  VarDecisao,    DESL_T,      5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE,             sim,          nao,                nao)  \
	m(  VarDecisao,    ACIO_T,      5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE,             sim,          nao,                nao)  \
	m(  VarDecisao,    PT_REL,      5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE,             sim,          nao,                nao)  \
	m(  VarDecisao,    PT_FINF,     5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE,             sim,          nao,                nao)  \
	m(  VarDecisao,    PTDISP,      5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE,             sim,          nao,                nao)  \
	m(  VarDecisao,    RP,          5,     ITERADORES_5_ESTAGIO_PERIODO_PERIODO_PROCESSOESTOCASTICO_VARIAVELALEATORIA,    sim,          nao,                nao)  \
	m(  VarDecisao,    YP,          5,     ITERADORES_5_ESTAGIO_PERIODO_PERIODO_PROCESSOESTOCASTICO_VARIAVELALEATORIA,    sim,          nao,                nao)  \
	m(  VarDecisao,    YPF,         5,     ITERADORES_5_ESTAGIO_PERIODO_PERIODO_PROCESSOESTOCASTICO_VARIAVELALEATORIA,    sim,          nao,                nao)  \
	m(  VarDecisao,    YP_ADD,      5,     ITERADORES_5_ESTAGIO_PERIODO_PERIODO_PROCESSOESTOCASTICO_VARIAVELALEATORIA,    sim,          nao,                nao)  \
	m(  VarDecisao,    PD,          5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_SUBMERCADO_PATAMARDEFICIT,           sim,          nao,                nao)  \
    m(  VarDecisao,    QOUT,        5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_INTERCAMBIOHIDRAULICO_HIDRELETRICA,  sim,          nao,                nao)  \
    m(  VarDecisao,    QOUT_FINF,   5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_INTERCAMBIOHIDRAULICO_HIDRELETRICA,  sim,          nao,                nao)  \
    m(  VarDecisao,    QOUT_REL,    5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_INTERCAMBIOHIDRAULICO_HIDRELETRICA,  sim,          nao,                nao)  \
    m(  VarDecisao,    QDES,        5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_HIDRELETRICA,           sim,          nao,                nao)  \
    m(  VarDecisao,    QDES_FINF,   5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_HIDRELETRICA,           sim,          nao,                nao)   
   //   Elemento,      Nome,  NroIter,     Iteradores,                                                            Impr.Primal,     Impr.Dual              NormDual


#define EQUACAO_LINEAR_5(m)\
	m(  EquLinear,    ENA,                                         5,     ITERADORES_5_ESTAGIO_PERIODO_PERIODO_HIDRELETRICA_REE,                      nao,        nao,                nao)      \
	m(  EquLinear,    YP,                                          5,     ITERADORES_5_ESTAGIO_PERIODO_PERIODO_PROCESSOESTOCASTICO_VARIAVELALEATORIA, nao,        nao,                nao)      \
	m(  EquLinear,    VAZAO_TURBINADA,                             5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO,  nao,        nao,                nao)      \
	m(  EquLinear,    VAZAO_TURBINADA_RELAXADA,                    5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO,  nao,        nao,                nao)      \
	m(  EquLinear,    VAZAO_TURBINADA_DISPONIVEL,                  5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO,  nao,        nao,                nao)      \
	m(  EquLinear,    POTENCIA_TERMICA,                            5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE,          nao,        nao,                nao)      \
	m(  EquLinear,    POTENCIA_TERMICA_RELAXADA,                   5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE,          nao,        nao,                nao)      \
	m(  EquLinear,    POTENCIA_TERMICA_DISPONIVEL,                 5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE,          nao,        nao,                nao)      \
    m(  EquLinear,    POTENCIA_TERMICA_DISPONIVEL_RELAXADA,        5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE,          nao,        nao,                nao)      \
	m(  EquLinear,    RELACAO_VARIAVEIS_BINARIAS_TERMICA,          5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE,          nao,        nao,                nao)      \
	m(  EquLinear,    POTENCIA_HIDRAULICA,                         5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO,  nao,        nao,                nao)      \
	m(  EquLinear,    POTENCIA_HIDRAULICA_RELAXADA,                5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO,  nao,        nao,                nao)      \
	m(  EquLinear,    POTENCIA_HIDRAULICA_DISPONIVEL,              5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO,  nao,        nao,                nao)      \
	m(  EquLinear,    POTENCIA_HIDRAULICA_DISPONIVEL_RELAXADA,     5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO,  nao,        nao,                nao)      
   //   Elemento,      Nome,                                  NroIter,     Iteradores,                                                         Impr.Primal,   Impr.Dual             NormDual

#define INEQUACAO_LINEAR_5(m)\
	m(  IneLinear,    POTENCIA_HIDRAULICA,                5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_INT,          nao,        sim,                nao)  \
	m(  IneLinear,    POTENCIA_TERMELETRICA_UTIL,         5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE,   nao,        nao,                nao)  \
	m(  IneLinear,    POTENCIA_TERMELETRICA_PARTIDA,      5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE,   nao,        nao,                nao)  \
	m(  IneLinear,    POTENCIA_TERMELETRICA_DESLIGAMENTO, 5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE,   nao,        nao,                nao)  \
	m(  IneLinear,    TEMPO_MINIMO_LIGADA,                5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE,   nao,        nao,                nao)  \
	m(  IneLinear,    TEMPO_MINIMO_DESLIGADA,             5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE,   nao,        nao,                nao)  \
    m(  IneLinear,    VAZAO_DESVIADA_FOLGA_INFERIOR,      5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_HIDRELETRICA, nao,        nao,                nao)  
   //   Elemento,      Nome,                         NroIter,     Iteradores,                                                  Impr.Primal,   Impr.Dual              NormDual

//
// ITERADORES 6
//

#define ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_INT(m)             m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdHidreletrica) m(5, IdConjuntoHidraulico) m(6, int)
#define ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE(m)      m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdHidreletrica) m(5, IdConjuntoHidraulico) m(6, IdUnidadeUHE)
#define ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE_PERIODO(m)                 m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdTermeletrica) m(5, IdUnidadeUTE) m(6, Periodo)
#define ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_INTERCAMBIOHIDRAULICO_HIDRELETRICA_HIDRELETRICA(m) m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdIntercambioHidraulico) m(5, IdHidreletrica) m(6, IdHidreletrica)

#define VARIAVEL_DECISAO_6(m)\
	m(  VarDecisao,     PH,          6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE,             sim,          nao,                nao)  \
	m(  VarDecisao,     PH_REL,      6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE,             sim,          nao,                nao)  \
	m(  VarDecisao,     PH_FINF,     6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE,             sim,          nao,                nao)  \
	m(  VarDecisao,     PH_FSUP,     6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE,             sim,          nao,                nao)  \
	m(  VarDecisao,     PHDISP,      6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE,             sim,          nao,                nao)  \
	m(  VarDecisao,     PHDISP_REL,  6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE,             sim,          nao,                nao)  \
	m(  VarDecisao,     PHDISP_FINF, 6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE,             sim,          nao,                nao)  \
    m(  VarDecisao,     QTURDISP,    6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE,             sim,          nao,                nao)  \
    m(  VarDecisao,     QTUR,        6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE,             sim,          nao,                nao)  \
    m(  VarDecisao,     QTUR_FINF,   6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE,             sim,          nao,                nao)  \
    m(  VarDecisao,     QDES,        6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_INTERCAMBIOHIDRAULICO_HIDRELETRICA_HIDRELETRICA,        sim,          nao,                nao)
   //   Elemento,       Nome,  NroIter,    Iteradores,                                                                               Impr.Primal,     Impr.Dual            NormDual


#define EQUACAO_LINEAR_6(m)\
	m(  EquLinear,     VAZAO_TURBINADA_RELAXADA,                    6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE,        nao,               nao,                nao)      \
	m(  EquLinear,     VAZAO_TURBINADA_DISPONIVEL,                  6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE,        nao,               nao,                nao)      \
	m(  EquLinear,     POTENCIA_HIDRAULICA,                         6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE,        nao,               nao,                nao)      \
	m(  EquLinear,     POTENCIA_HIDRAULICA_RELAXADA,                6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE,        nao,               nao,                nao)      \
	m(  EquLinear,     POTENCIA_HIDRAULICA_DISPONIVEL,              6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE,        nao,               nao,                nao)      \
	m(  EquLinear,     POTENCIA_HIDRAULICA_DISPONIVEL_RELAXADA,     6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE,        nao,               nao,                nao)      
   //   Elemento,       Nome,                                  NroIter,    Iteradores,                                                                          Impr.Primal,          Impr.Dual              NormDual  

#define INEQUACAO_LINEAR_6(m)\
	m(  IneLinear,     PT_TRAJETORIA_ACIONAMENTO_INF,          6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE_PERIODO,         nao,               nao,                nao)  \
	m(  IneLinear,     PT_TRAJETORIA_ACIONAMENTO_SUP,          6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE_PERIODO,         nao,               nao,                nao)  \
	m(  IneLinear,     PT_TRAJETORIA_ACIONAMENTO,              6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE_PERIODO,         nao,               nao,                nao)  \
	m(  IneLinear,     POTENCIA_HIDRAULICA,                    6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_INT,     nao,               nao,                nao)  
   //   Elemento,       Nome,                             NroIter,    Iteradores,                                                                Impr.Primal,          Impr.Dual             NormDual  


//
// ITERADORES 7
//

#define ITERADORES_7_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE_INT(m)   m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdHidreletrica) m(5, IdConjuntoHidraulico) m(6, IdUnidadeUHE) m(7, int)
#define ITERADORES_7_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE_PERIODO_INT(m)              m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdTermeletrica) m(5, IdUnidadeUTE) m(6, Periodo) m(7, int)


#define INEQUACAO_LINEAR_7(m)\
	m(  IneLinear,     PT_TRAJETORIA_DESLIGAMENTO_INF,    7,    ITERADORES_7_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE_PERIODO_INT,            nao,    nao,                nao)  \
	m(  IneLinear,     PT_TRAJETORIA_DESLIGAMENTO_SUP,    7,    ITERADORES_7_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE_PERIODO_INT,            nao,    nao,                nao)  \
	m(  IneLinear,     PT_TRAJETORIA_DESLIGAMENTO,        7,    ITERADORES_7_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE_PERIODO_INT,            nao,    nao,                nao)  \
	m(  IneLinear,     POTENCIA_HIDRAULICA,               7,    ITERADORES_7_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE_INT, nao,    nao,                nao)  
   //   Elemento,      Nome,                        NroIter,    Iteradores,                                                                       Impr.Primal,    Impr.Dual         NormDual  






	class ModeloOtimizacao : public SmartDados {

	public:

		ArranjoResolucao arranjoResolucao;

		ModeloOtimizacao(const IdModeloOtimizacao a_idModeloOtimizacao, Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados);
		ModeloOtimizacao(const ModeloOtimizacao& instanciaCopiar);
		void esvaziar();
		virtual ~ModeloOtimizacao();

		DECLARA_SMART_ELEMENTO(ModeloOtimizacao, SMART_ELEMENTO_MODELO_OTIMIZACAO)

		void instanciarCorteBenders(const IdEstagio a_idEstagio, const SmartEnupla<IdRealizacao, double>& a_rhs, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>>& a_coeficiente, const SmartEnupla<IdVariavelEstado, double>& a_estado);
		void instanciarCorteBenders(const IdEstagio a_idEstagio, const SmartEnupla<IdRealizacao, double>& a_rhs, const SmartEnupla<IdRealizacao, double>& a_rhs_primal, const SmartEnupla<IdRealizacao, double>& a_rhs_estado, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>>& a_coeficiente, const SmartEnupla<IdVariavelEstado, double>& a_estado);

		void removerCortesBendersDominados(const IdEstagio a_idEstagio, const IdProcesso a_idProcesso, EntradaSaidaDados a_entradaSaidaDados);

		void requestCorteBenders(const IdProcesso a_idProcesso, const IdEstagio a_idEstagio, const std::string a_diretorio, EntradaSaidaDados a_entradaSaidaDados);

		void requestCorteBenders(const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders, double& a_rhs, SmartEnupla<IdVariavelEstado, double>& a_coeficiente);

		void requestCorteBendersFinalizado(const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao);

		void selecionarCorteBenders(const IdEstagio a_idEstagio);

		void addCorteBendersToZF(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdEstagio a_idEstagio_anterior, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders);
		void setCorteBendersToZF(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdEstagio a_idEstagio_anterior, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders);
		void remCorteBendersFromZF(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdEstagio a_idEstagio_anterior, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders);

		void addCorteBendersToZT(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders);
		void setCorteBendersToZT(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders);
		void remCorteBendersFromZT(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders);

		bool isNecessarioInstanciarSolver(const IdEstagio a_idEstagio, const TipoSubproblemaSolver a_tipoSubproblemaSolver);

		void imprimirEstagios(EntradaSaidaDados a_entradaSaidaDados);

		void imprimirSolver(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const std::string a_nome_arquivo);

		void imprimirVariaveisEstado(EntradaSaidaDados a_entradaSaidaDados);

		void imprimirVariaveisRealizacao(EntradaSaidaDados a_entradaSaidaDados);
		void imprimirRestricoesCenario(EntradaSaidaDados a_entradaSaidaDados);

		void imprimirVariaveisRealizacaoInterna(EntradaSaidaDados a_entradaSaidaDados);

		void exportarCorteBenders(const IdProcesso a_idProcesso, const IdEstagio a_estagio_inicial, const IdEstagio a_estagio_final, EntradaSaidaDados a_entradaSaidaDados);

		void exportarCorteBenders(const IdProcesso a_idProcesso, const IdEstagio a_estagio, EntradaSaidaDados a_entradaSaidaDados);
		void exportarVersaoAlternativaCorteBenders(const IdProcesso a_idProcesso, const IdEstagio a_estagio, EntradaSaidaDados a_entradaSaidaDados);

		void removerCorteBenders(const IdEstagio a_estagio_inicial, const IdEstagio a_estagio_final);
		void removerCorteBenders(const IdEstagio a_estagio);

		void exportarCorteBenders_AcoplamentoPreEstudo(const IdIteracao a_idIteracao, EntradaSaidaDados a_entradaSaidaDados);

		void importarCorteBenders(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdProcesso a_idProcesso, const std::string a_diretorio_impressao_selecao_cortes, EntradaSaidaDados a_entradaSaidaDados);
		void importarCorteBenders_AcoplamentoPosEstudo(const TipoSubproblemaSolver a_TSS, const IdProcesso a_idProcesso, const IdIteracao a_idIteracao, const std::string a_diretorio_impressao_selecao_cortes, EntradaSaidaDados a_entradaSaidaDados);

		void importarVariaveisEstado_AcoplamentoPosEstudo(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdProcesso a_idProcesso, EntradaSaidaDados a_entradaSaidaDados);

		void importarVariaveisEstado_AcoplamentoPreEstudo(const IdProcesso a_idProcesso, const IdProcesso a_maior_processo, const IdIteracao a_idIteracao, const IdCenario a_cenario_inicial, const IdCenario a_cenario_final, EntradaSaidaDados a_entradaSaidaDados);

		void exportarVariaveisEstado_AcoplamentoPreEstudo(EntradaSaidaDados a_entradaSaidaDados);

		void exportarVariaveisEstado_AcoplamentoPosEstudo(const IdIteracao a_idIteracao, EntradaSaidaDados a_entradaSaidaDados);

		bool isVariavelEstadoInstanciada(const IdEstagio a_idEstagio, const IdVariavelEstado a_idVariavelEstado);

		void esvaziarVetorCorteBenders();
		void esvaziarVetorCorteBenders(const IdEstagio a_idEstagio);

		void gerarRealizacoes(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, EntradaSaidaDados a_entradaSaidaDados);

		void atualizarModeloOtimizacaoComVariavelEstado(const IdEstagio a_idEstagio, const IdCenario a_idCenario);
		void atualizarModeloOtimizacaoComVariavelEstado(const IdEstagio a_idEstagio, const IdCenario a_idCenario, EstruturaResultados<double> &a_retorno_estados);

		void atualizarModeloOtimizacaoComVariavelRealizacao(const IdEstagio a_idEstagio, const IdCenario    a_idCenario);
		void atualizarModeloOtimizacaoComVariavelRealizacao(const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao);

		void atualizarModeloOtimizacaoComRestricaoCenario(const IdEstagio a_idEstagio, const IdCenario    a_idCenario);
		void atualizarModeloOtimizacaoComRestricaoCenario(const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao);

		bool atualizarModeloOtimizacaoComVariavelRealizacaoInterna(const TipoSubproblemaSolver a_TSS, const TipoSubproblemaSolver a_TSS_origem, const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const std::string a_diretorio);

		bool atualizarModeloOtimizacaoComVariavelRealizacaoInterna(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao);

		void resetarVariavelRealizacaoInterna(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio);

		void atualizarVariavelEstadoComSolucaoPrimal(const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario);

		void addValorVariavelEstado(const IdEstagio a_idEstagio, const IdVariavelEstado idVariavelEstado, const IdCenario a_idCenario, const double valor);
		void alocarVariaveisEstado(const IdIteracao a_idIteracao, const IdEstagio a_idEstagio);

		void imprimirSolucaoPorEstagioPorCenario_porEstagio(const IdProcesso a_idProcesso, const std::string a_subdiretorio, EntradaSaidaDados a_entradaSaidaDados);

		void imprimirSolucaoPorEstagioPorCenarioPorRealizacao_porEstagio(const IdProcesso a_idProcesso, const std::string a_subdiretorio, EntradaSaidaDados a_entradaSaidaDados);

		bool otimizarProblemaComTratamentoNumericoInviabilidade(const TipoSubproblemaSolver a_TSS, const int a_level, const IdEstagio a_idEstagio);
		bool otimizarProblemaComTratamentoNumericoInviabilidade(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio);

		void setTempoLimiteOtimizacao(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const double a_tempo_limite);

		double getTempoOtimizacao(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio);

		double otimizarProblema(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const std::string a_diretorio);

		double otimizarProblema(const TipoSubproblemaSolver a_TSS, const IdProcesso a_idProcesso, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const std::string a_diretorio);

		bool otimizarProblema(const TipoSubproblemaSolver a_TSS, const IdProcesso a_idProcesso, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, EstruturaResultados<double>& a_sol_inf_var_dinamica, EstruturaResultados<double>& a_solucao_dual_var_dinamica, EstruturaResultados<double>& a_limite_inferior_var_dinamica, EstruturaResultados<double>& a_limite_superior_var_dinamica, EstruturaResultados<double>& a_sol_dual_var_estado, const std::string a_diretorio);

		int contadorLog10005;
		void escreverLog10005(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const std::string a_string);

		double posOtimizacaoProblema(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const std::string a_diretorio);

		double posOtimizacaoProblema(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const std::string a_diretorio);

		bool posOtimizacaoProblema(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, bool a_retornar_proxy, EstruturaResultados<double>& a_sol_inf_var_dinamica, EstruturaResultados<double>& a_solucao_dual_var_dinamica, EstruturaResultados<double>& a_limite_inferior_var_dinamica, EstruturaResultados<double>& a_limite_superior_var_dinamica, EstruturaResultados<double>& a_sol_dual_var_estado, const std::string a_diretorio);

		void armazenarValoresSolver(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao);

		void calcularCustoPrimalViaSubproblemaMestre(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const std::string a_diretorio, double& a_custo_geral, double& a_custo_individual);

		double getProbabilidadeAbertura(const IdEstagio a_idEstagio, const IdCenario a_idCenario);

		double getProbabilidadeAbertura(const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao);

		void getSolucaoDualVariavelEstado(const IdEstagio a_idEstagio, int &a_idx_sol_dual_var_estado, double* a_sol_dual_var_estado);

		double getCustoTotal(const IdEstagio a_idEstagio);
		double getCustoImediato(const IdEstagio a_idEstagio);
		double getCustoFuturo(const IdEstagio a_idEstagio);

		void consolidarResultados(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);
		void consolidarVariaveis(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);
		void consolidarEquacoes(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);
		void consolidarInequacoes(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);

		int getNumeroVariavelDinamica(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio);

	private:

		struct TempoVariaveis {

			double variaveis = 0.0;
			double variaveisVolume = 0.0;
			double variaveisHidreletrica = 0.0;
			double variaveisTermeletrica = 0.0;
			double variaveisDeficit = 0.0;
			double variaveisDemanda = 0.0;
			double variaveisUsinaNSimulada = 0.0;
			double variaveisIntercambio = 0.0;
			double variaveisRestOperativaUHE = 0.0;
			double variaveisRestEleticaFolga = 0.0;
			double restricaoCustoValorPresente_periodo = 0;
			double restricaoCusto_periodo = 0;
			double restricaoCusto_periodoEstudo_patamarCarga = 0;
			double restricaoCustoPenalidade_periodoEstudo_patamarCarga = 0;
			double restricaoProducaoMedia = 0;
			double restricaoAtendimentoDemanda = 0;
			double restricaoBalancoHidraulicoUsinaRegularizacao = 0;
			double restricaoBalancoHidraulicoUsinaFioAgua = 0;
			double restricaoProducaoHidreletrica_porProdutibilidade = 0;
			double restricaoProducaoHidreletrica_porFPH = 0;
			double restricaoProducaoTermeletrica_porProdutibilidade = 0;
			double restricaoVazaoDefluente = 0;
			double restricaoVazaoDesviada = 0;
			double restricaoVazaoRetirada = 0;
			double restricaoVolumeUtil = 0;
			double restricoesEletricas = 0;
			double restricoesAgrupamentoIntercambio = 0;
			double restricaoAfluenciaIncremental_porPeriodo_horizonteProcessoEstocastico = 0;
			double restricaoProcessoEstocasticoHidrologico_porPeriodo_horizonteProcessoEstocastico = 0;
			double restricaoCortebendersEmCustoFuturo = 0;
			double variaveisAssociadasHorizonteOtimizacao = 0;
			double variaveisAssociadasHorizonteEstudo = 0;
			double variaveisAssociadasHorizonteProcessoEstocastico = 0;
			double restricaoIgualdadeHorizonteOtimizacao = 0;
			double restricaoTurbinamentoHidraulico = 0;
			double restricaoHidraulicaEspecial_vazao_afluente = 0;
			double restricaoHidraulicaEspecial_vazao_defluente = 0;
			double restricaoHidraulicaEspecial_volume_armazenado = 0;
			double variaveisIntercambioHidraulico = 0;
			double restricaoIntercambioHidraulicoRetiradas = 0;
			double restricaoSuperposicaoPeriodosTempoViagemAgua = 0;

		};

		TempoVariaveis tempoCriarVariaveis;

		std::vector<std::vector<std::string>> lista_VarDecisao_instanciadas;
		std::vector<std::vector<std::string>> lista_EquLinear_instanciadas;
		std::vector<std::vector<std::string>> lista_IneLinear_instanciadas;

		IdCorteBenders maiorIdCorteExportadoPreEstudo = IdCorteBenders_Nenhum;


		template<typename TListasIdxElem, typename TIt>
		std::vector<TIt> alocConteudoIter(TListasIdxElem& a_listasIdxElem, const TIt a_it) {
			try {

				// Inicializa Lista
				if (a_listasIdxElem.size() == 0)
					a_listasIdxElem.addElemento(a_it, a_listasIdxElem.getTipoElemento());

				// Alocação do final da lista até iterador informado
				else if (a_listasIdxElem.getIteradorFinal() < a_it) {
					for (TIt it = a_listasIdxElem.getIteradorFinal(); it < a_it; a_listasIdxElem.incrementarIteradorMinimo(it))
						if (it >= a_listasIdxElem.getIteradorFinal() + 1)
							a_listasIdxElem.addElemento(it, a_listasIdxElem.getTipoElemento());
					a_listasIdxElem.addElemento(a_it, a_listasIdxElem.getTipoElemento());
				}

				// Alocação do início da lista até o iterador informado
				else if (a_it < a_listasIdxElem.getIteradorInicial()) {
					for (TIt it = a_listasIdxElem.getIteradorInicial(); it >= a_it + 1; a_listasIdxElem.decrementarIteradorMinimo(it))
						if (it < a_listasIdxElem.getIteradorInicial())
							a_listasIdxElem.addElemento(it, a_listasIdxElem.getTipoElemento());
					a_listasIdxElem.addElemento(a_it, a_listasIdxElem.getTipoElemento());
				}

				if (!a_listasIdxElem.isIteradorValido(a_it))
					return a_listasIdxElem.getIteradores(a_it);

				return std::vector<TIt>();

			}
			catch (const std::exception& erro) { throw std::invalid_argument("alocConteudoIter(a_listasIdxElem," + getFullString(a_it) + "): \n" + std::string(erro.what())); }
		};

		template<typename TListasIdxElem, typename TIt>
		void tratConteudoIter(TListasIdxElem& a_listasIdxElem, const TIt a_it) {
			try {

		        const std::vector<TIt> iteradores_multiplos = alocConteudoIter(a_listasIdxElem, a_it);

				if (iteradores_multiplos.size() <= 1)
					return;

				for (int i = 0; i < int(iteradores_multiplos.size()); i++) {
					if (a_listasIdxElem.at(iteradores_multiplos.at(i)).size() > 0)
						throw std::invalid_argument("Nao foi possivel alocar conteudo devido a existencia de multiplos iteradores com elementos em " + getString(iteradores_multiplos.at(i)));
				}

				a_listasIdxElem.alterarValorSeAlterarIterador(a_it, a_listasIdxElem.getTipoElemento());

			}
			catch (const std::exception& erro) { throw std::invalid_argument("tratConteudoIter(a_listasIdxElem," + getFullString(a_it) + "): \n" + std::string(erro.what())); }
		};
		
		template<typename TListasIdxElem, typename TConteudo, typename TIt1, typename TIt2, typename TIt3, typename TIt4, typename TIt5, typename TIt6, typename TIt7, typename TIt8, typename TIt9, typename TIt10, typename TIt11, typename TIt12>
		void addConteudoIters_12(TListasIdxElem& a_listasIdxElem, TConteudo& a_conteudo, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6, const TIt7 a_it7, const TIt8 a_it8, const TIt9 a_it9, const TIt10 a_it10, const TIt11 a_it11, const TIt12 a_it12) {

			try {

				tratConteudoIter(a_listasIdxElem, a_it1);
				tratConteudoIter(a_listasIdxElem.at(a_it1), a_it2);
				tratConteudoIter(a_listasIdxElem.at(a_it1).at(a_it2), a_it3);
				tratConteudoIter(a_listasIdxElem.at(a_it1).at(a_it2).at(a_it3), a_it4);
				tratConteudoIter(a_listasIdxElem.at(a_it1).at(a_it2).at(a_it3).at(a_it4), a_it5);
				tratConteudoIter(a_listasIdxElem.at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5), a_it6);
				tratConteudoIter(a_listasIdxElem.at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6), a_it7);
				tratConteudoIter(a_listasIdxElem.at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).at(a_it7), a_it8);
				tratConteudoIter(a_listasIdxElem.at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).at(a_it7).at(a_it8), a_it9);
				tratConteudoIter(a_listasIdxElem.at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).at(a_it7).at(a_it8).at(a_it9), a_it10);
				tratConteudoIter(a_listasIdxElem.at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).at(a_it7).at(a_it8).at(a_it9).at(a_it10), a_it11);
				if (alocConteudoIter(a_listasIdxElem.at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).at(a_it7).at(a_it8).at(a_it9).at(a_it10).at(a_it11), a_it12).size() > 1)
					throw std::invalid_argument("Multiplos iteradores na ultima camada.");

				a_listasIdxElem.at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).at(a_it7).at(a_it8).at(a_it9).at(a_it10).at(a_it11).setElemento(a_it12, a_conteudo);

			}
			catch (const std::exception& erro) { throw std::invalid_argument("addConteudoIters_12(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "," + getFullString(a_it6) + "," + getFullString(a_it7) + "," + getFullString(a_it8) + "," + getFullString(a_it9) + "," + getFullString(a_it10) + "," + getFullString(a_it11) + "," + getFullString(a_it12) + "): \n" + std::string(erro.what())); }
		};

		template<typename TListasArmzElem, typename TListasIdxElem, typename TListasNormElem, typename TIt1, typename TIt2, typename TIt3, typename TIt4, typename TIt5, typename TIt6, typename TIt7, typename TIt8, typename TIt9, typename TIt10>
		bool varredurasIters_1(TListasArmzElem& a_lArmz, const TListasIdxElem& a_lIdx, const TListasNormElem& a_lNorm, const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdIteracao a_idIteracao, const bool a_isVar, const bool a_isPrimal, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6, const TIt7 a_it7, const TIt8 a_it8, const TIt9 a_it9, const TIt10 a_it10, const IdRealizacao a_idRealizacao, const IdCenario a_idCenario) {

			try {

				if ((!a_isVar) && (a_isPrimal))
					return false;

				if (a_lIdx.size() == 0)
					return false;

				const TIt1 it1 = a_idEstagio;

				if ((a_lIdx.getIteradorInicial() <= it1) && (it1 <= a_lIdx.getIteradorFinal())) {

					if (a_lArmz.size() == 0)
						a_lArmz.addElemento(it1, a_lIdx.at(it1));

					if (a_lArmz.at(it1).size() > 0) {

						double vlrNorm = 1.0;

						if ((!a_isPrimal) && (a_lNorm.size() > 0))
							vlrNorm = a_lNorm.at(it1).at(1).at(1).at(1).at(1).at(1).at(1).at(1).at(1).at(1).at(IdRealizacao_1).at(IdCenario_1);

						varredurasIters(a_lArmz.at(it1).at(1).at(1).at(1).at(1).at(1).at(1).at(1).at(1).at(1), a_lIdx.at(it1).at(1).at(1).at(1).at(1).at(1).at(1).at(1).at(1).at(1), vlrNorm, a_TSS, a_idEstagio, a_idIteracao, a_isVar, a_isPrimal, a_idRealizacao, a_idCenario);
					}
				}


				return true;

			}
			catch (const std::exception& erro) { throw std::invalid_argument("varredurasIters_1(" + getFullString(a_idEstagio) + "," + getFullString(a_isVar) + "," + getFullString(a_isPrimal) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }

		} // bool varredurasIters_1(TListasIdxElem& a_listasIdxElem, const bool a_isVar, const bool a_isPrimal, const IdRealizacao a_idRealizacao, const IdCenario a_idCenario) {

		template<typename TListasArmzElem, typename TListasIdxElem, typename TListasNormElem, typename TIt1, typename TIt2, typename TIt3, typename TIt4, typename TIt5, typename TIt6, typename TIt7, typename TIt8, typename TIt9, typename TIt10>
		bool varredurasIters_2(TListasArmzElem& a_lArmz, const TListasIdxElem& a_lIdx, const TListasNormElem& a_lNorm, const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdIteracao a_idIteracao, const bool a_isVar, const bool a_isPrimal, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6, const TIt7 a_it7, const TIt8 a_it8, const TIt9 a_it9, const TIt10 a_it10, const IdRealizacao a_idRealizacao, const IdCenario a_idCenario) {

			try {

				if ((!a_isVar) && (a_isPrimal))
					return false;

				if (a_lIdx.size() == 0)
					return false;

				const TIt1 it1 = a_idEstagio;

				if ((a_lIdx.getIteradorInicial() <= it1) && (it1 <= a_lIdx.getIteradorFinal())) {

					if (a_lArmz.size() == 0)
						a_lArmz.addElemento(it1, a_lIdx.at(it1));

					if (a_lArmz.at(it1).size() > 0) {
						for (TIt2 it2 = a_lArmz.at(it1).getIteradorInicial(); it2 <= a_lArmz.at(it1).getIteradorFinal(); a_lArmz.at(it1).incrementarIterador(it2)) {
							if (a_lArmz.at(it1).at(it2).size() > 0) {

								double vlrNorm = 1.0;

								if ((!a_isPrimal) && (a_lNorm.size() > 0))
									vlrNorm = a_lNorm.at(it1).at(it2).at(1).at(1).at(1).at(1).at(1).at(1).at(1).at(1).at(IdRealizacao_1).at(IdCenario_1);

								varredurasIters(a_lArmz.at(it1).at(it2).at(1).at(1).at(1).at(1).at(1).at(1).at(1).at(1), a_lIdx.at(it1).at(it2).at(1).at(1).at(1).at(1).at(1).at(1).at(1).at(1), vlrNorm, a_TSS, a_idEstagio, a_idIteracao, a_isVar, a_isPrimal, a_idRealizacao, a_idCenario);
							}
						}
					}
				}


				return true;

			}
			catch (const std::exception& erro) { throw std::invalid_argument("varredurasIters_2(" + getFullString(a_idEstagio) + "," + getFullString(a_isVar) + "," + getFullString(a_isPrimal) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }

		} // bool varredurasIters_3(TListasIdxElem& a_listasIdxElem, const bool a_isVar, const bool a_isPrimal, const IdRealizacao a_idRealizacao, const IdCenario a_idCenario) {

		template<typename TListasArmzElem, typename TListasIdxElem, typename TListasNormElem, typename TIt1, typename TIt2, typename TIt3, typename TIt4, typename TIt5, typename TIt6, typename TIt7, typename TIt8, typename TIt9, typename TIt10>
		bool varredurasIters_3(TListasArmzElem& a_lArmz, const TListasIdxElem& a_lIdx, const TListasNormElem& a_lNorm, const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdIteracao a_idIteracao, const bool a_isVar, const bool a_isPrimal, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6, const TIt7 a_it7, const TIt8 a_it8, const TIt9 a_it9, const TIt10 a_it10, const IdRealizacao a_idRealizacao, const IdCenario a_idCenario) {

			try {

				if ((!a_isVar) && (a_isPrimal))
					return false;

				if (a_lIdx.size() == 0)
					return false;

				const TIt1 it1 = a_idEstagio;

				if ((a_lIdx.getIteradorInicial() <= it1) && (it1 <= a_lIdx.getIteradorFinal())) {

					if (a_lArmz.size() == 0)
						a_lArmz.addElemento(it1, a_lIdx.at(it1));

					if (a_lArmz.at(it1).size() > 0) {
						for (TIt2 it2 = a_lArmz.at(it1).getIteradorInicial(); it2 <= a_lArmz.at(it1).getIteradorFinal(); a_lArmz.at(it1).incrementarIterador(it2)) {
							if (a_lArmz.at(it1).at(it2).size() > 0) {
								for (TIt3 it3 = a_lArmz.at(it1).at(it2).getIteradorInicial(); it3 <= a_lArmz.at(it1).at(it2).getIteradorFinal(); a_lArmz.at(it1).at(it2).incrementarIterador(it3)) {
									if (a_lArmz.at(it1).at(it2).at(it3).size() > 0) {

										double vlrNorm = 1.0;

										if ((!a_isPrimal) && (a_lNorm.size() > 0))
											vlrNorm = a_lNorm.at(it1).at(it2).at(it3).at(1).at(1).at(1).at(1).at(1).at(1).at(1).at(IdRealizacao_1).at(IdCenario_1);

										varredurasIters(a_lArmz.at(it1).at(it2).at(it3).at(1).at(1).at(1).at(1).at(1).at(1).at(1), a_lIdx.at(it1).at(it2).at(it3).at(1).at(1).at(1).at(1).at(1).at(1).at(1), vlrNorm, a_TSS, a_idEstagio, a_idIteracao, a_isVar, a_isPrimal, a_idRealizacao, a_idCenario);
									}
								}
							}
						}
					}
				}


				return true;

			}
			catch (const std::exception& erro) { throw std::invalid_argument("varredurasIters_3(" + getFullString(a_idEstagio) + "," + getFullString(a_isVar) + "," + getFullString(a_isPrimal) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }

		} // bool varredurasIters_3(TListasIdxElem& a_listasIdxElem, const bool a_isVar, const bool a_isPrimal, const IdRealizacao a_idRealizacao, const IdCenario a_idCenario) {

		template<typename TListasArmzElem, typename TListasIdxElem, typename TListasNormElem, typename TIt1, typename TIt2, typename TIt3, typename TIt4, typename TIt5, typename TIt6, typename TIt7, typename TIt8, typename TIt9, typename TIt10>
		bool varredurasIters_4(TListasArmzElem& a_lArmz, const TListasIdxElem& a_lIdx, const TListasNormElem& a_lNorm, const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdIteracao a_idIteracao, const bool a_isVar, const bool a_isPrimal, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6, const TIt7 a_it7, const TIt8 a_it8, const TIt9 a_it9, const TIt10 a_it10, const IdRealizacao a_idRealizacao, const IdCenario a_idCenario) {

			try {

				if ((!a_isVar) && (a_isPrimal))
					return false;

				if (a_lIdx.size() == 0)
					return false;

				const TIt1 it1 = a_idEstagio;

				if ((a_lIdx.getIteradorInicial() <= it1) && (it1 <= a_lIdx.getIteradorFinal())) {

					if (a_lArmz.size() == 0)
						a_lArmz.addElemento(it1, a_lIdx.at(it1));

					if (a_lArmz.at(it1).size() > 0) {
						for (TIt2 it2 = a_lArmz.at(it1).getIteradorInicial(); it2 <= a_lArmz.at(it1).getIteradorFinal(); a_lArmz.at(it1).incrementarIterador(it2)) {
							if (a_lArmz.at(it1).at(it2).size() > 0) {
								for (TIt3 it3 = a_lArmz.at(it1).at(it2).getIteradorInicial(); it3 <= a_lArmz.at(it1).at(it2).getIteradorFinal(); a_lArmz.at(it1).at(it2).incrementarIterador(it3)) {
									if (a_lArmz.at(it1).at(it2).at(it3).size() > 0) {
										for (TIt4 it4 = a_lArmz.at(it1).at(it2).at(it3).getIteradorInicial(); it4 <= a_lArmz.at(it1).at(it2).at(it3).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).incrementarIterador(it4)) {
											if (a_lArmz.at(it1).at(it2).at(it3).at(it4).size() > 0) {

												double vlrNorm = 1.0;

												if ((!a_isPrimal) && (a_lNorm.size() > 0))
													vlrNorm = a_lNorm.at(it1).at(it2).at(it3).at(it4).at(1).at(1).at(1).at(1).at(1).at(1).at(IdRealizacao_1).at(IdCenario_1);

												varredurasIters(a_lArmz.at(it1).at(it2).at(it3).at(it4).at(1).at(1).at(1).at(1).at(1).at(1), a_lIdx.at(it1).at(it2).at(it3).at(it4).at(1).at(1).at(1).at(1).at(1).at(1), vlrNorm, a_TSS, a_idEstagio, a_idIteracao, a_isVar, a_isPrimal, a_idRealizacao, a_idCenario);
											}
										}
									}
								}
							}
						}
					}
				}


				return true;

			}
			catch (const std::exception& erro) { throw std::invalid_argument("varredurasIters_4(" + getFullString(a_idEstagio) + "," + getFullString(a_isVar) + "," + getFullString(a_isPrimal) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }

		} // bool varredurasIters_4(TListasIdxElem& a_listasIdxElem, const bool a_isVar, const bool a_isPrimal, const IdRealizacao a_idRealizacao, const IdCenario a_idCenario) {

		template<typename TListasArmzElem, typename TListasIdxElem, typename TListasNormElem, typename TIt1, typename TIt2, typename TIt3, typename TIt4, typename TIt5, typename TIt6, typename TIt7, typename TIt8, typename TIt9, typename TIt10>
		bool varredurasIters_5(TListasArmzElem& a_lArmz, const TListasIdxElem& a_lIdx, const TListasNormElem& a_lNorm, const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdIteracao a_idIteracao, const bool a_isVar, const bool a_isPrimal, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6, const TIt7 a_it7, const TIt8 a_it8, const TIt9 a_it9, const TIt10 a_it10, const IdRealizacao a_idRealizacao, const IdCenario a_idCenario) {

			try {

				if ((!a_isVar) && (a_isPrimal))
					return false;

				if (a_lIdx.size() == 0)
					return false;

				const TIt1 it1 = a_idEstagio;

				if ((a_lIdx.getIteradorInicial() <= it1) && (it1 <= a_lIdx.getIteradorFinal())) {

					if (a_lArmz.size() == 0)
						a_lArmz.addElemento(it1, a_lIdx.at(it1));

					if (a_lArmz.at(it1).size() > 0) {
						for (TIt2 it2 = a_lArmz.at(it1).getIteradorInicial(); it2 <= a_lArmz.at(it1).getIteradorFinal(); a_lArmz.at(it1).incrementarIterador(it2)) {
							if (a_lArmz.at(it1).at(it2).size() > 0) {
								for (TIt3 it3 = a_lArmz.at(it1).at(it2).getIteradorInicial(); it3 <= a_lArmz.at(it1).at(it2).getIteradorFinal(); a_lArmz.at(it1).at(it2).incrementarIterador(it3)) {
									if (a_lArmz.at(it1).at(it2).at(it3).size() > 0) {
										for (TIt4 it4 = a_lArmz.at(it1).at(it2).at(it3).getIteradorInicial(); it4 <= a_lArmz.at(it1).at(it2).at(it3).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).incrementarIterador(it4)) {
											if (a_lArmz.at(it1).at(it2).at(it3).at(it4).size() > 0) {
												for (TIt5 it5 = a_lArmz.at(it1).at(it2).at(it3).at(it4).getIteradorInicial(); it5 <= a_lArmz.at(it1).at(it2).at(it3).at(it4).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).at(it4).incrementarIterador(it5)) {
													if (a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).size() > 0) {

														double vlrNorm = 1.0;

														if ((!a_isPrimal) && (a_lNorm.size() > 0))
															vlrNorm = a_lNorm.at(it1).at(it2).at(it3).at(it4).at(it5).at(1).at(1).at(1).at(1).at(1).at(IdRealizacao_1).at(IdCenario_1);

														varredurasIters(a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(1).at(1).at(1).at(1).at(1), a_lIdx.at(it1).at(it2).at(it3).at(it4).at(it5).at(1).at(1).at(1).at(1).at(1), vlrNorm, a_TSS, a_idEstagio, a_idIteracao, a_isVar, a_isPrimal, a_idRealizacao, a_idCenario);
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}


				return true;

			}
			catch (const std::exception& erro) { throw std::invalid_argument("varredurasIters_5(" + getFullString(a_idEstagio) + "," + getFullString(a_isVar) + "," + getFullString(a_isPrimal) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }

		} // bool varredurasIters_5(TListasIdxElem& a_listasIdxElem, const bool a_isVar, const bool a_isPrimal, const IdRealizacao a_idRealizacao, const IdCenario a_idCenario) {

		template<typename TListasArmzElem, typename TListasIdxElem, typename TListasNormElem, typename TIt1, typename TIt2, typename TIt3, typename TIt4, typename TIt5, typename TIt6, typename TIt7, typename TIt8, typename TIt9, typename TIt10>
		bool varredurasIters_6(TListasArmzElem& a_lArmz, const TListasIdxElem& a_lIdx, const TListasNormElem& a_lNorm, const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdIteracao a_idIteracao, const bool a_isVar, const bool a_isPrimal, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6, const TIt7 a_it7, const TIt8 a_it8, const TIt9 a_it9, const TIt10 a_it10, const IdRealizacao a_idRealizacao, const IdCenario a_idCenario) {

			try {

				if ((!a_isVar) && (a_isPrimal))
					return false;

				if (a_lIdx.size() == 0)
					return false;

				const TIt1 it1 = a_idEstagio;

				if ((a_lIdx.getIteradorInicial() <= it1) && (it1 <= a_lIdx.getIteradorFinal())) {

					if (a_lArmz.size() == 0)
						a_lArmz.addElemento(it1, a_lIdx.at(it1));

					if (a_lArmz.at(it1).size() > 0) {
						for (TIt2 it2 = a_lArmz.at(it1).getIteradorInicial(); it2 <= a_lArmz.at(it1).getIteradorFinal(); a_lArmz.at(it1).incrementarIterador(it2)) {
							if (a_lArmz.at(it1).at(it2).size() > 0) {
								for (TIt3 it3 = a_lArmz.at(it1).at(it2).getIteradorInicial(); it3 <= a_lArmz.at(it1).at(it2).getIteradorFinal(); a_lArmz.at(it1).at(it2).incrementarIterador(it3)) {
									if (a_lArmz.at(it1).at(it2).at(it3).size() > 0) {
										for (TIt4 it4 = a_lArmz.at(it1).at(it2).at(it3).getIteradorInicial(); it4 <= a_lArmz.at(it1).at(it2).at(it3).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).incrementarIterador(it4)) {
											if (a_lArmz.at(it1).at(it2).at(it3).at(it4).size() > 0) {
												for (TIt5 it5 = a_lArmz.at(it1).at(it2).at(it3).at(it4).getIteradorInicial(); it5 <= a_lArmz.at(it1).at(it2).at(it3).at(it4).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).at(it4).incrementarIterador(it5)) {
													if (a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).size() > 0) {
														for (TIt6 it6 = a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).getIteradorInicial(); it6 <= a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).incrementarIterador(it6)) {
															if (a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).size() > 0) {

																double vlrNorm = 1.0;

																if ((!a_isPrimal) && (a_lNorm.size() > 0))
																	vlrNorm = a_lNorm.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(1).at(1).at(1).at(1).at(IdRealizacao_1).at(IdCenario_1);

																varredurasIters(a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(1).at(1).at(1).at(1), a_lIdx.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(1).at(1).at(1).at(1), vlrNorm, a_TSS, a_idEstagio, a_idIteracao, a_isVar, a_isPrimal, a_idRealizacao, a_idCenario);
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}


				return true;

			}
			catch (const std::exception& erro) { throw std::invalid_argument("varredurasIters_6(" + getFullString(a_idEstagio) + "," + getFullString(a_isVar) + "," + getFullString(a_isPrimal) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }

		} // bool varredurasIters_6(TListasIdxElem& a_listasIdxElem, const bool a_isVar, const bool a_isPrimal, const IdRealizacao a_idRealizacao, const IdCenario a_idCenario) {

		template<typename TListasArmzElem, typename TListasIdxElem, typename TListasNormElem, typename TIt1, typename TIt2, typename TIt3, typename TIt4, typename TIt5, typename TIt6, typename TIt7, typename TIt8, typename TIt9, typename TIt10>
		bool varredurasIters_7(TListasArmzElem& a_lArmz, const TListasIdxElem& a_lIdx, const TListasNormElem& a_lNorm, const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdIteracao a_idIteracao, const bool a_isVar, const bool a_isPrimal, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6, const TIt7 a_it7, const TIt8 a_it8, const TIt9 a_it9, const TIt10 a_it10, const IdRealizacao a_idRealizacao, const IdCenario a_idCenario) {

			try {

				if ((!a_isVar) && (a_isPrimal))
					return false;

				if (a_lIdx.size() == 0)
					return false;

				const TIt1 it1 = a_idEstagio;

				if ((a_lIdx.getIteradorInicial() <= it1) && (it1 <= a_lIdx.getIteradorFinal())) {

					if (a_lArmz.size() == 0)
						a_lArmz.addElemento(it1, a_lIdx.at(it1));

					if (a_lArmz.at(it1).size() > 0) {
						for (TIt2 it2 = a_lArmz.at(it1).getIteradorInicial(); it2 <= a_lArmz.at(it1).getIteradorFinal(); a_lArmz.at(it1).incrementarIterador(it2)) {
							if (a_lArmz.at(it1).at(it2).size() > 0) {
								for (TIt3 it3 = a_lArmz.at(it1).at(it2).getIteradorInicial(); it3 <= a_lArmz.at(it1).at(it2).getIteradorFinal(); a_lArmz.at(it1).at(it2).incrementarIterador(it3)) {
									if (a_lArmz.at(it1).at(it2).at(it3).size() > 0) {
										for (TIt4 it4 = a_lArmz.at(it1).at(it2).at(it3).getIteradorInicial(); it4 <= a_lArmz.at(it1).at(it2).at(it3).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).incrementarIterador(it4)) {
											if (a_lArmz.at(it1).at(it2).at(it3).at(it4).size() > 0) {
												for (TIt5 it5 = a_lArmz.at(it1).at(it2).at(it3).at(it4).getIteradorInicial(); it5 <= a_lArmz.at(it1).at(it2).at(it3).at(it4).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).at(it4).incrementarIterador(it5)) {
													if (a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).size() > 0) {
														for (TIt6 it6 = a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).getIteradorInicial(); it6 <= a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).incrementarIterador(it6)) {
															if (a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).size() > 0) {
																for (TIt7 it7 = a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).getIteradorInicial(); it7 <= a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).incrementarIterador(it7)) {
																	if (a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).size() > 0) {

																		double vlrNorm = 1.0;

																		if ((!a_isPrimal) && (a_lNorm.size() > 0))
																			vlrNorm = a_lNorm.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(1).at(1).at(1).at(IdRealizacao_1).at(IdCenario_1);

																		varredurasIters(a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(1).at(1).at(1), a_lIdx.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(1).at(1).at(1), vlrNorm, a_TSS, a_idEstagio, a_idIteracao, a_isVar, a_isPrimal, a_idRealizacao, a_idCenario);
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}


				return true;

			}
			catch (const std::exception& erro) { throw std::invalid_argument("varredurasIters_7(" + getFullString(a_idEstagio) + "," + getFullString(a_isVar) + "," + getFullString(a_isPrimal) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }

		} // bool varredurasIters_7(TListasIdxElem& a_listasIdxElem, const bool a_isVar, const bool a_isPrimal, const IdRealizacao a_idRealizacao, const IdCenario a_idCenario) {

		template<typename TListasArmzElem, typename TListasIdxElem, typename TListasNormElem, typename TIt1, typename TIt2, typename TIt3, typename TIt4, typename TIt5, typename TIt6, typename TIt7, typename TIt8, typename TIt9, typename TIt10>
		bool varredurasIters_8(TListasArmzElem& a_lArmz, const TListasIdxElem& a_lIdx, const TListasNormElem& a_lNorm, const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdIteracao a_idIteracao, const bool a_isVar, const bool a_isPrimal, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6, const TIt7 a_it7, const TIt8 a_it8, const TIt9 a_it9, const TIt10 a_it10, const IdRealizacao a_idRealizacao, const IdCenario a_idCenario) {

			try {

				if ((!a_isVar) && (a_isPrimal))
					return false;

				if (a_lIdx.size() == 0)
					return false;

				const TIt1 it1 = a_idEstagio;

				if ((a_lIdx.getIteradorInicial() <= it1) && (it1 <= a_lIdx.getIteradorFinal())) {

					if (a_lArmz.size() == 0)
						a_lArmz.addElemento(it1, a_lIdx.at(it1));

					if (a_lArmz.at(it1).size() > 0) {
						for (TIt2 it2 = a_lArmz.at(it1).getIteradorInicial(); it2 <= a_lArmz.at(it1).getIteradorFinal(); a_lArmz.at(it1).incrementarIterador(it2)) {
							if (a_lArmz.at(it1).at(it2).size() > 0) {
								for (TIt3 it3 = a_lArmz.at(it1).at(it2).getIteradorInicial(); it3 <= a_lArmz.at(it1).at(it2).getIteradorFinal(); a_lArmz.at(it1).at(it2).incrementarIterador(it3)) {
									if (a_lArmz.at(it1).at(it2).at(it3).size() > 0) {
										for (TIt4 it4 = a_lArmz.at(it1).at(it2).at(it3).getIteradorInicial(); it4 <= a_lArmz.at(it1).at(it2).at(it3).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).incrementarIterador(it4)) {
											if (a_lArmz.at(it1).at(it2).at(it3).at(it4).size() > 0) {
												for (TIt5 it5 = a_lArmz.at(it1).at(it2).at(it3).at(it4).getIteradorInicial(); it5 <= a_lArmz.at(it1).at(it2).at(it3).at(it4).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).at(it4).incrementarIterador(it5)) {
													if (a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).size() > 0) {
														for (TIt6 it6 = a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).getIteradorInicial(); it6 <= a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).incrementarIterador(it6)) {
															if (a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).size() > 0) {
																for (TIt7 it7 = a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).getIteradorInicial(); it7 <= a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).incrementarIterador(it7)) {
																	if (a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).size() > 0) {
																		for (TIt8 it8 = a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).getIteradorInicial(); it8 <= a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).incrementarIterador(it8)) {
																			if (a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).size() > 0) {

																				double vlrNorm = 1.0;

																				if ((!a_isPrimal) && (a_lNorm.size() > 0))
																					vlrNorm = a_lNorm.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).at(1).at(1).at(IdRealizacao_1).at(IdCenario_1);

																				varredurasIters(a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).at(1).at(1), a_lIdx.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).at(1).at(1), vlrNorm, a_TSS, a_idEstagio, a_idIteracao, a_isVar, a_isPrimal, a_idRealizacao, a_idCenario);
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}


				return true;

			}
			catch (const std::exception& erro) { throw std::invalid_argument("varredurasIters_8(" + getFullString(a_idEstagio) + "," + getFullString(a_isVar) + "," + getFullString(a_isPrimal) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }

		} // bool varredurasIters_8(TListasIdxElem& a_listasIdxElem, const bool a_isVar, const bool a_isPrimal, const IdRealizacao a_idRealizacao, const IdCenario a_idCenario) {

		template<typename TListasArmzElem, typename TListasIdxElem, typename TListasNormElem, typename TIt1, typename TIt2, typename TIt3, typename TIt4, typename TIt5, typename TIt6, typename TIt7, typename TIt8, typename TIt9, typename TIt10>
		bool varredurasIters_9(TListasArmzElem& a_lArmz, const TListasIdxElem& a_lIdx, const TListasNormElem& a_lNorm, const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdIteracao a_idIteracao, const bool a_isVar, const bool a_isPrimal, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6, const TIt7 a_it7, const TIt8 a_it8, const TIt9 a_it9, const TIt10 a_it10, const IdRealizacao a_idRealizacao, const IdCenario a_idCenario) {

			try {

				if ((!a_isVar) && (a_isPrimal))
					return false;

				if (a_lIdx.size() == 0)
					return false;

				const TIt1 it1 = a_idEstagio;

				if ((a_lIdx.getIteradorInicial() <= it1) && (it1 <= a_lIdx.getIteradorFinal())) {

					if (a_lArmz.size() == 0)
						a_lArmz.addElemento(it1, a_lIdx.at(it1));

					if (a_lArmz.at(it1).size() > 0) {
						for (TIt2 it2 = a_lArmz.at(it1).getIteradorInicial(); it2 <= a_lArmz.at(it1).getIteradorFinal(); a_lArmz.at(it1).incrementarIterador(it2)) {
							if (a_lArmz.at(it1).at(it2).size() > 0) {
								for (TIt3 it3 = a_lArmz.at(it1).at(it2).getIteradorInicial(); it3 <= a_lArmz.at(it1).at(it2).getIteradorFinal(); a_lArmz.at(it1).at(it2).incrementarIterador(it3)) {
									if (a_lArmz.at(it1).at(it2).at(it3).size() > 0) {
										for (TIt4 it4 = a_lArmz.at(it1).at(it2).at(it3).getIteradorInicial(); it4 <= a_lArmz.at(it1).at(it2).at(it3).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).incrementarIterador(it4)) {
											if (a_lArmz.at(it1).at(it2).at(it3).at(it4).size() > 0) {
												for (TIt5 it5 = a_lArmz.at(it1).at(it2).at(it3).at(it4).getIteradorInicial(); it5 <= a_lArmz.at(it1).at(it2).at(it3).at(it4).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).at(it4).incrementarIterador(it5)) {
													if (a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).size() > 0) {
														for (TIt6 it6 = a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).getIteradorInicial(); it6 <= a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).incrementarIterador(it6)) {
															if (a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).size() > 0) {
																for (TIt7 it7 = a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).getIteradorInicial(); it7 <= a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).incrementarIterador(it7)) {
																	if (a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).size() > 0) {
																		for (TIt8 it8 = a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).getIteradorInicial(); it8 <= a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).incrementarIterador(it8)) {
																			if (a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).size() > 0) {
																				for (TIt9 it9 = a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).getIteradorInicial(); it9 <= a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).incrementarIterador(it9)) {
																					if (a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).at(it9).size() > 0) {

																						double vlrNorm = 1.0;

																						if ((!a_isPrimal) && (a_lNorm.size() > 0))
																							vlrNorm = a_lNorm.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).at(it9).at(1).at(IdRealizacao_1).at(IdCenario_1);

																						varredurasIters(a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).at(it9).at(1), a_lIdx.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).at(it9).at(1), vlrNorm, a_TSS, a_idEstagio, a_idIteracao, a_isVar, a_isPrimal, a_idRealizacao, a_idCenario);

																					}
																				}
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}


				return true;

			}
			catch (const std::exception& erro) { throw std::invalid_argument("varredurasIters_9(" + getFullString(a_idEstagio) + "," + getFullString(a_isVar) + "," + getFullString(a_isPrimal) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }

		} // bool varredurasIters_9(TListasIdxElem& a_listasIdxElem, const bool a_isVar, const bool a_isPrimal, const IdRealizacao a_idRealizacao, const IdCenario a_idCenario) {

		template<typename TListasArmzElem, typename TListasIdxElem, typename TListasNormElem, typename TIt1, typename TIt2, typename TIt3, typename TIt4, typename TIt5, typename TIt6, typename TIt7, typename TIt8, typename TIt9, typename TIt10>
		bool varredurasIters_10(TListasArmzElem& a_lArmz, const TListasIdxElem& a_lIdx, const TListasNormElem& a_lNorm, const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdIteracao a_idIteracao, const bool a_isVar, const bool a_isPrimal, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6, const TIt7 a_it7, const TIt8 a_it8, const TIt9 a_it9, const TIt10 a_it10, const IdRealizacao a_idRealizacao, const IdCenario a_idCenario) {

			try {

				if ((!a_isVar) && (a_isPrimal))
					return false;

				if (a_lIdx.size() == 0)
					return false;

				const TIt1 it1 = a_idEstagio;

				if ((a_lIdx.getIteradorInicial() <= it1) && (it1 <= a_lIdx.getIteradorFinal())) {

					if (a_lArmz.size() == 0)
						a_lArmz.addElemento(it1, a_lIdx.at(it1));

					if (a_lArmz.at(it1).size() > 0) {
						for (TIt2 it2 = a_lArmz.at(it1).getIteradorInicial(); it2 <= a_lArmz.at(it1).getIteradorFinal(); a_lArmz.at(it1).incrementarIterador(it2)) {
							if (a_lArmz.at(it1).at(it2).size() > 0) {
								for (TIt3 it3 = a_lArmz.at(it1).at(it2).getIteradorInicial(); it3 <= a_lArmz.at(it1).at(it2).getIteradorFinal(); a_lArmz.at(it1).at(it2).incrementarIterador(it3)) {
									if (a_lArmz.at(it1).at(it2).at(it3).size() > 0) {
										for (TIt4 it4 = a_lArmz.at(it1).at(it2).at(it3).getIteradorInicial(); it4 <= a_lArmz.at(it1).at(it2).at(it3).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).incrementarIterador(it4)) {
											if (a_lArmz.at(it1).at(it2).at(it3).at(it4).size() > 0) {
												for (TIt5 it5 = a_lArmz.at(it1).at(it2).at(it3).at(it4).getIteradorInicial(); it5 <= a_lArmz.at(it1).at(it2).at(it3).at(it4).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).at(it4).incrementarIterador(it5)) {
													if (a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).size() > 0) {
														for (TIt6 it6 = a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).getIteradorInicial(); it6 <= a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).incrementarIterador(it6)) {
															if (a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).size() > 0) {
																for (TIt7 it7 = a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).getIteradorInicial(); it7 <= a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).incrementarIterador(it7)) {
																	if (a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).size() > 0) {
																		for (TIt8 it8 = a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).getIteradorInicial(); it8 <= a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).incrementarIterador(it8)) {
																			if (a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).size() > 0) {
																				for (TIt9 it9 = a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).getIteradorInicial(); it9 <= a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).incrementarIterador(it9)) {
																					if (a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).at(it9).size() > 0) {
																						for (TIt10 it10 = a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).at(it9).getIteradorInicial(); it10 <= a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).at(it9).getIteradorFinal(); a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).at(it9).incrementarIterador(it10)) {
																							if (a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).at(it9).at(it10).size() > 0) {

																								double vlrNorm = 1.0;

																								if ((!a_isPrimal) && (a_lNorm.size() > 0))
																									vlrNorm = a_lNorm.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).at(it9).at(it10).at(IdRealizacao_1).at(IdCenario_1);
																								
																								varredurasIters(a_lArmz.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).at(it9).at(it10), a_lIdx.at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).at(it9).at(it10), vlrNorm, a_TSS, a_idEstagio, a_idIteracao, a_isVar, a_isPrimal, a_idRealizacao, a_idCenario);

																							}
																						}
																					}
																				}
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}


				return true;

			}
			catch (const std::exception& erro) { throw std::invalid_argument("varredurasIters_10(" + getFullString(a_idEstagio) + "," + getFullString(a_isVar) + "," + getFullString(a_isPrimal) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }

		} // bool varredurasIters_10(TListasIdxElem& a_listasIdxElem, const bool a_isVar, const bool a_isPrimal, const IdRealizacao a_idRealizacao, const IdCenario a_idCenario) {

		template<typename TListasArmzElem, typename TListasIdxElem>
		void varredurasIters(TListasArmzElem& a_lArmz, const TListasIdxElem& a_lIdx, const double a_VlrNorm, const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdIteracao a_idIteracao, const bool a_isVar, const bool a_isPrimal, const IdRealizacao a_idRealizacao, const IdCenario a_idCenario) {

			try {

				double conteudo = NAN;

				if (a_idCenario != IdCenario_Nenhum) {
					if ((a_isVar) && (a_isPrimal))
						conteudo = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorPrimal(int(a_lIdx.at(IdRealizacao_1).at(IdCenario_1)));

					else if ((a_isVar) && (!a_isPrimal))
						conteudo = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getReducedCost(int(a_lIdx.at(IdRealizacao_1).at(IdCenario_1))) * a_VlrNorm;

					else if ((!a_isVar) && (!a_isPrimal))
						conteudo = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getMultiplicador(int(a_lIdx.at(IdRealizacao_1).at(IdCenario_1))) * a_VlrNorm;
				}
			
				if (a_idRealizacao == IdRealizacao_Nenhum) {
					if (a_lArmz.at(IdRealizacao_1).size() == 1) {
						const IdProcesso idProcesso = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());
						const IdCenario cIni = arranjoResolucao.getAtributo(a_idIteracao, idProcesso, AttComumProcesso_menor_cenario, IdCenario());
						const int numero_cenarios = int(arranjoResolucao.getAtributo(a_idIteracao, idProcesso, AttComumProcesso_maior_cenario, IdCenario()) - cIni) + 1;
						a_lArmz.at(IdRealizacao_1) = SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN));
					}

					if (a_idCenario != IdCenario_Nenhum)
						a_lArmz.at(IdRealizacao_1).at(a_idCenario) = conteudo;

				} // if (a_idRealizacao == IdRealizacao_Nenhum) {

				else {

					if (a_lArmz.size() == 1) {
						IdRealizacao rBase = a_lArmz.getIteradorInicial();
						if (a_lArmz.at(rBase).size() == 1) {
							const IdRealizacao maiorIdRealizacao = getAtributo(a_idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao());
							a_lArmz = SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(int(rBase - IdRealizacao_1) + 1, SmartEnupla<IdCenario, double>()));
						}
					} // if (a_lArmz.size() == 1) {

					if (a_lArmz.at(a_idRealizacao).size() == 0) {

						const IdProcesso idProcesso = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());
						const IdCenario cIni = arranjoResolucao.getAtributo(a_idIteracao, idProcesso, AttComumProcesso_menor_cenario, IdCenario());
						const int numero_cenarios = int(arranjoResolucao.getAtributo(a_idIteracao, idProcesso, AttComumProcesso_maior_cenario, IdCenario()) - cIni) + 1;

						a_lArmz.at(a_idRealizacao) = SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN));

					} // if (a_lArmz.at(a_idRealizacao).size() == 1) {

					a_lArmz.at(a_idRealizacao).at(a_idCenario) = conteudo;

				} // else {

			}
			catch (const std::exception& erro) { throw std::invalid_argument("varredurasIters(" + getFullString(a_idEstagio) + "," + getFullString(a_isVar) + "," + getFullString(a_isPrimal) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }

		}

		template<typename TListasIdxElem, typename TConteudo, typename TIt1, typename TIt2, typename TIt3, typename TIt4, typename TIt5, typename TIt6, typename TIt7, typename TIt8, typename TIt9, typename TIt10, typename TIt11, typename TIt12>
		bool getConteudoIters_12(TListasIdxElem& a_listasIdxElem, TConteudo &a_conteudo, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6, const TIt7 a_it7, const TIt8 a_it8, const TIt9 a_it9, const TIt10 a_it10, const TIt11 a_it11, const TIt12 a_it12) {

			try {

				if (!a_listasIdxElem.isIteradorValido(a_it1)) return false;
				else if (!a_listasIdxElem.at(a_it1).isIteradorValido(a_it2)) return false;
				else if (!a_listasIdxElem.at(a_it1).at(a_it2).isIteradorValido(a_it3)) return false;
				else if (!a_listasIdxElem.at(a_it1).at(a_it2).at(a_it3).isIteradorValido(a_it4)) return false;
				else if (!a_listasIdxElem.at(a_it1).at(a_it2).at(a_it3).at(a_it4).isIteradorValido(a_it5)) return false;
				else if (!a_listasIdxElem.at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).isIteradorValido(a_it6)) return false;
				else if (!a_listasIdxElem.at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).isIteradorValido(a_it7)) return false;
				else if (!a_listasIdxElem.at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).at(a_it7).isIteradorValido(a_it8)) return false;
				else if (!a_listasIdxElem.at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).at(a_it7).at(a_it8).isIteradorValido(a_it9)) return false;
				else if (!a_listasIdxElem.at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).at(a_it7).at(a_it8).at(a_it9).isIteradorValido(a_it10)) return false;
				else if (!a_listasIdxElem.at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).at(a_it7).at(a_it8).at(a_it9).at(a_it10).isIteradorValido(a_it11)) return false;
				else if (!a_listasIdxElem.at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).at(a_it7).at(a_it8).at(a_it9).at(a_it10).at(a_it11).isIteradorValido(a_it12)) return false;

				a_conteudo = a_listasIdxElem.at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).at(a_it7).at(a_it8).at(a_it9).at(a_it10).at(a_it11).at(a_it12);
				return true;

			}
			catch (const std::exception& erro) { throw std::invalid_argument("getConteudoIters12(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "," + getFullString(a_it6) + "," + getFullString(a_it7) + "," + getFullString(a_it8) + "," + getFullString(a_it9) + "," + getFullString(a_it10) + "," + getFullString(a_it11) + "," + getFullString(a_it12) + "): \n" + std::string(erro.what())); }
		};

			VARIAVEL_DECISAO_2(DECLARAR_METODOS_ELEMENTO)

			VARIAVEL_DECISAO_3(DECLARAR_METODOS_ELEMENTO)

			VARIAVEL_DECISAO_4(DECLARAR_METODOS_ELEMENTO)
			VARIAVEL_DECISAO_5(DECLARAR_METODOS_ELEMENTO)
			VARIAVEL_DECISAO_6(DECLARAR_METODOS_ELEMENTO)

			EQUACAO_LINEAR_2(DECLARAR_METODOS_ELEMENTO)
			EQUACAO_LINEAR_3(DECLARAR_METODOS_ELEMENTO)
			EQUACAO_LINEAR_4(DECLARAR_METODOS_ELEMENTO)
			EQUACAO_LINEAR_5(DECLARAR_METODOS_ELEMENTO)
			EQUACAO_LINEAR_6(DECLARAR_METODOS_ELEMENTO)


			INEQUACAO_LINEAR_3(DECLARAR_METODOS_ELEMENTO)
			INEQUACAO_LINEAR_4(DECLARAR_METODOS_ELEMENTO)
			INEQUACAO_LINEAR_5(DECLARAR_METODOS_ELEMENTO)
			INEQUACAO_LINEAR_6(DECLARAR_METODOS_ELEMENTO)
			INEQUACAO_LINEAR_7(DECLARAR_METODOS_ELEMENTO)


		void instanciarProcessoEstocastico(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados);

		int criarVariaveisDecisao_VariaveisEstado_Restricoes_ZP0_VF_FINF(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_penalizacao);
		int criarVariaveisDecisao_VariaveisEstado_Restricoes_QDEFLAG(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo, const IdHidreletrica a_idHidreletrica, const Periodo a_periodo_lag);
		int criarVariaveisDecisao_VariaveisEstado_Restricoes_YP(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo, const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria, const Periodo a_periodo_lag, const double a_grau_liberdade, std::vector<IdHidreletrica> a_idHidreletrica = std::vector<IdHidreletrica>());
		int criarVariaveisDecisao_VariaveisEstado_Restricoes_PTDISPCOM(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo, const IdPatamarCarga a_idPatamarCarga, const IdTermeletrica a_idTermeletrica, const double a_potencia_disponivel_minima, const double a_potencia_disponivel_maxima);

		void criarModeloOtimizacao(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados);

		void criarVariaveisTermeletricas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdPatamarCarga a_idPatamarCarga, const Periodo a_periodo_estudo, const IdTermeletrica a_maiorIdTermeletrica, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio);

		void criarVariaveisContrato(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdPatamarCarga a_idPatamarCarga, const Periodo a_periodo_estudo, const IdContrato a_maiorIdContrato, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio);

		void criarVariaveisEolicas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdPatamarCarga a_idPatamarCarga, const Periodo a_periodo_estudo, const IdUsinaEolica a_maiorIdUsinaEolica, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio);

		void criarVariaveisDemandaEspecial(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdPatamarCarga a_idPatamarCarga, const Periodo a_periodo_estudo, const IdDemandaEspecial a_maiorIdDemandaEspecial, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio);

		void criarVariaveisHidraulicas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio);
		void criarVariaveisHidraulicas_porPatamar(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdPatamarCarga a_idPatamarCarga, const Periodo a_periodo_estudo);

		void criarVariaveisAssociadasHorizonteEstudo(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdDemandaEspecial a_maiorIdDemandaEspecial, const IdUsinaEolica a_maiorIdUsinaEolica, const IdContrato a_maiorIdContrato, const IdSubmercado a_maiorIdSubmercado, const IdIntercambio a_maiorIdIntercambio, \
			const IdRestricaoEletrica a_maiorIdRestricaoEletrica, const IdTermeletrica a_maiorIdTermeletrica, const IdRestricaoOperativaUHE a_maiorIdRestricaoOperativaUHE, \
			const IdIntercambioHidraulico a_maiorIdIntercambioHidraulico, const IdUsinaElevatoria a_maiorIdUsinaElevatoria, const SmartEnupla<Periodo, double>a_horizonte_estudo_estagio, TempoVariaveis& a_tempoCriarVariaveis);

		void criarRestricoesEvaporacao(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdHidreletrica a_idHidreletrica, const Periodo a_proximo_periodo_estudo);

		void criarVariaveisAssociadasHorizonteOtimizacao(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdEstagio a_estagio_final, const IdEstagio a_estagio_inicial, const Periodo a_periodo_otimizacao);

		void criarVariaveisDecisao_Restricoes_ProcessoEstocasticoHidrologico(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio);

		void criarRestricoesPotenciaHidraulicaDisponivel(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdPatamarCarga a_idPatamarCarga, const Periodo a_periodo_estudo, const IdHidreletrica a_idHidreletrica, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio);

		void criarRestricoesVolumeUtil_e_Penalidade(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdHidreletrica a_idHidreletrica);

		void criarRestricoesFuncaoProducaoHidreletrica(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga, const SmartEnupla<Periodo, double>a_horizonte_estudo_estagio, const Periodo a_periodo_estudo);

		void criarRestricoesProducaoTermeletrica_porProdutibilidade(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdPatamarCarga a_idPatamarCarga, const Periodo a_periodo_estudo, const IdTermeletrica a_maiorIdTermeletrica, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio);

		void criarVariaveisVolume(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const SmartEnupla<Periodo, double> &a_horizonte_estudo_estagio, const SmartEnupla<Periodo, IdEstagio> &a_horizonte_estudo_vetor);

		void criarRestricoesCustoPenalidade_periodoEstudo_patamarCarga(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdSubmercado a_maiorIdSubmercado, \
			const IdIntercambio a_maiorIdIntercambio, const IdRestricaoEletrica a_maiorIdRestricaoEletrica, const IdTermeletrica a_maiorIdTermeletrica, const IdRestricaoOperativaUHE a_maiorIdRestricaoOperativaUHE, \
			const IdHidreletrica a_maiorIdHidreletrica, const IdIntercambioHidraulico a_maiorIdIntercambioHidraulico, const IdUsinaElevatoria a_maiorIdUsinaElevatoria, const Periodo a_periodo_otimizacao, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesCustoPenalidade_periodoEstudo(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdRestricaoOperativaUHE a_maiorIdRestricaoOperativaUHE, const IdHidreletrica a_maiorIdHidreletrica);

		void criarRestricoesBombeamentoHidraulico(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdPatamarCarga a_idPatamarCarga, const IdUsinaElevatoria a_idUsinaElevatoria);

		void criarRestricoesTurbinamentoHidraulico(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesVazaoDefluente(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesVazaoDesviada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesVazaoRetirada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesVazaoBombeada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdUsinaElevatoria a_idUsinaElevatoria, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesEletricas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdRestricaoEletrica a_maiorIdRestricaoEletrica, const Periodo a_periodo_estudo, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesAgrupamentoIntercambio(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdAgrupamentoIntercambio a_maiorIdAgrupamentoIntercambio, const Periodo a_periodo_estudo, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesCorteBendersEmCustoFuturo(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio);
		void criarRestricoesCorteBendersEmCustoTotal(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio);

		void criarRestricoesCusto_Total_Imediato_Futuro(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdEstagio a_estagio_final, const IdHidreletrica a_maiorIdHidreletrica, const Periodo a_periodo_otimizacao, const SmartEnupla<Periodo, double> a_horizonte_estudo_estagio, const  IdProcessoEstocastico a_tipo_processo_estocastico_hidrologico);

		void criarRestricoesCustoValorPresente_periodo(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo);

		void criarRestricoesCusto_periodo(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo);

		void criarRestricoesCustoOperacao_periodoEstudo_patamarCarga(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdSubmercado a_maiorIdSubmercado, const IdTermeletrica a_maiorIdTermeletrica, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesProducaoMedia(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdSubmercado a_idSubmercado, const IdIntercambio a_maiorIdIntercambio, const IdTermeletrica a_maiorIdTermeletrica, const IdHidreletrica a_maiorIdHidreletrica, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesBalancoHidraulicoUsinaByVolume(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdEstagio a_idEstagio_acoplamento, const Periodo a_periodo_acoplamento, const Periodo a_periodo_estudo, const Periodo a_periodo_otimizacao_final, const Periodo a_periodo_estudo_inicial, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga, const SmartEnupla<Periodo, int>& a_horizonte_processo_estocastico, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio);

		void criarRestricoesBalancoHidraulicoUsinaByVazao(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdEstagio a_idEstagio_acoplamento, const Periodo a_periodo_acoplamento, const Periodo a_periodo_estudo, const Periodo a_periodo_otimizacao_final, const Periodo a_periodo_estudo_inicial, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga, const SmartEnupla<Periodo, int>& a_horizonte_processo_estocastico, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio);

		void criarRestricoesAtendimentoDemanda(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdUsinaEolica a_maiorIdUsinaEolica, const IdContrato a_maiorIdContrato, const IdSubmercado a_idSubmercado, const IdIntercambio a_maiorIdIntercambio, const IdTermeletrica a_maiorIdTermeletrica, const IdHidreletrica a_maiorIdHidreletrica, const IdUsinaElevatoria a_maiorIdUsinaElevatoria, const IdPatamarCarga a_idPatamarCarga);

		void formularModeloOtimizacao(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados);

		void criarRestricoesHidraulicaEspecial_vazao_afluente(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const SmartEnupla<Periodo, int>& a_horizonte_processo_estocastico, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio);

		void criarRestricoesHidraulicaEspecial_vazao_defluente(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdPatamarCarga a_idPatamarCarga, const Periodo a_periodo_estudo);

		void criarRestricoesHidraulicaEspecial_volume_armazenado(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const SmartEnupla<Periodo, double>a_horizonte_estudo);

		void criarRestricoesHidraulicaEspecial_energia_armazenada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const SmartEnupla<Periodo, double>a_horizonte_estudo);

		void criarRestricoesIntercambioHidraulicooRetiradaRelaxada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdPatamarCarga a_idPatamarCarga, const IdIntercambioHidraulico a_idIntercambioHidraulico);

		void setVolumeMeta(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const SmartEnupla<Periodo, double> &a_horizonte_estudo_estagio);

		void criarRestricoesIntercambioRelaxado(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdIntercambio a_idIntercambio, const IdPatamarCarga a_idPatamarCarga);

		void zerarVariaveisFolga(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdIntercambio a_maiorIdIntercambio, const IdRestricaoEletrica a_maiorIdRestricaoEletrica, const IdTermeletrica a_maiorIdTermeletrica, const IdRestricaoOperativaUHE a_maiorIdRestricaoOperativaUHE, const IdIntercambioHidraulico a_maiorIdIntercambioHidraulico, const IdUsinaElevatoria a_maiorIdUsinaElevatoria, const SmartEnupla<Periodo, double>a_horizonte_estudo_estagio);

		IdHidreletrica getIdHidreletricaFromIdProcessoEstocasticoIdVariavelAleatoriaIdVariavelAleatoriaInterna(const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria, const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna);
		std::vector<IdHidreletrica> getIdHidreletricaFromIdProcessoEstocasticoIdVariavelAleatoria(const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria);

		void getIdVariavelAleatoriaIdVariavelAleatoriaInternaFromIdHidreletrica(const IdProcessoEstocastico a_idProcessoEstocastico, IdVariavelAleatoria &a_idVariavelAleatoria, IdVariavelAleatoriaInterna& a_idVariavelAleatoriaInterna, const IdHidreletrica a_hidreletrica);

		IdVariavelAleatoriaInterna getIdVariavelAleatoriaInternaFromIdVariavelAleatoriaIdHidreletrica(const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria, const IdHidreletrica a_hidreletrica);

		template<typename TipoIterador, typename Valor>
		struct GetNumeroHorasFromIterador {
			static constexpr Valor getNumeroHoras(const TipoIterador a_iterador) {
				return Valor(1.0);
			}
		};

		template<typename TipoIterador1, typename TipoIterador2, typename Valor>
		struct GetNumeroHorasFromIteradores {
			static constexpr Valor getNumeroHoras(const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2, const ModeloOtimizacao* a_modeloOtimizacao) {
				return Valor(1.0);
			}
		};

		template<typename Valor>
		struct GetNumeroHorasFromIterador <Periodo, Valor> {
			static constexpr Valor getNumeroHoras(const Periodo a_periodo) {
				try {
					const int numero_horas = a_periodo.getHoras();
					if (numero_horas == 0)
						return Valor(Valor(a_periodo.getMinutos()) / 60.0);
					else
						return Valor(numero_horas);
				}
				catch (const std::exception& erro) { throw std::invalid_argument("getNumeroHoras(" + getString(a_periodo) + "): \n" + std::string(erro.what())); }
			}
		};

		template<typename Valor>
		struct GetNumeroHorasFromIteradores <Periodo, IdPatamarCarga, Valor> {
			static constexpr Valor getNumeroHoras(const Periodo a_periodo, const IdPatamarCarga a_idPatamarCarga, const ModeloOtimizacao* a_modeloOtimizacao) {
				try {
					Valor numero_horas = Valor(a_periodo.getHoras());
					if (numero_horas == 0.0)
						numero_horas = Valor(Valor(a_periodo.getMinutos()) / 60.0);
					return numero_horas * Valor(a_modeloOtimizacao->getElementoMatriz(AttMatrizModeloOtimizacao_percentual_duracao_patamar_carga, a_periodo, a_idPatamarCarga, double()));
				}
				catch (const std::exception& erro) { throw std::invalid_argument("getNumeroHoras(" + getString(a_periodo) + "," + getString(a_idPatamarCarga) + "): \n" + std::string(erro.what())); }
			}
		};

		template<typename TipoIterador>
		constexpr double getNumeroHorasFromIterador(const TipoIterador a_iterador) {
			return GetNumeroHorasFromIterador<TipoIterador, double>::getNumeroHoras(a_iterador);
		};

		template<typename TipoIterador1, typename TipoIterador2>
		constexpr double getNumeroHorasFromIteradores(const TipoIterador1 a_iterador1, const TipoIterador2 a_iterador2) {
			return GetNumeroHorasFromIteradores<TipoIterador1, TipoIterador2, double>::getNumeroHoras(a_iterador1, a_iterador2, this);
		};

};

GET_STRING_FROM_CLASS(ModeloOtimizacao)

#endif 
