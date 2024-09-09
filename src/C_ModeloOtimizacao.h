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
	  m(ModeloOtimizacao,  AttVetor, tratamento_inviabilidade,         int,       -1,          max,           0,  IdEstagio)

#define ATT_MATRIZ_MODELO_OTIMIZACAO(m)  \
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
// ITERADORES 1
//

#define ITERADORES_1_ESTAGIO(m)         m(1, IdEstagio)

#define VARIAVEL_DECISAO_1(m)\
	m(  VarDecisao,     ZP0,                      1,    ITERADORES_1_ESTAGIO)  \
	m(  VarDecisao,     ZO0,                      1,    ITERADORES_1_ESTAGIO)  \
	m(  VarDecisao,     ZT,                       1,    ITERADORES_1_ESTAGIO)  \
	m(  VarDecisao,     ZI,                       1,    ITERADORES_1_ESTAGIO)  \
	m(  VarDecisao,     ZP,                       1,    ITERADORES_1_ESTAGIO)  \
	m(  VarDecisao,     ZF,                       1,    ITERADORES_1_ESTAGIO)  
   //   Elemento,       Nome,               NroIter,    Iteradores,                                         

#define EQUACAO_LINEAR_1(m)\
	m(  EquLinear,     ZP0,                   1,    ITERADORES_1_ESTAGIO)  \
	m(  EquLinear,     ZO0,                   1,    ITERADORES_1_ESTAGIO)  \
	m(  EquLinear,     ZT,                    1,    ITERADORES_1_ESTAGIO)  \
	m(  EquLinear,     ZT_MESTRE,             1,    ITERADORES_1_ESTAGIO)  \
	m(  EquLinear,     ZI,                    1,    ITERADORES_1_ESTAGIO)  \
	m(  EquLinear,     ZP,                    1,    ITERADORES_1_ESTAGIO)  \
	m(  EquLinear,     ZF,                    1,    ITERADORES_1_ESTAGIO)  
   //   Elemento,     Nome,                           NroIter,    Iteradores,                                   


//
// ITERADORES 2
//

#define ITERADORES_2_ESTAGIO_PERIODO(m)            m(1, IdEstagio) m(2, Periodo)
#define ITERADORES_2_ESTAGIO_VARIAVELESTADO(m)     m(1, IdEstagio) m(2, IdVariavelEstado)
#define ITERADORES_3_ESTAGIO_REALIZACAO(m)         m(1, IdEstagio) m(2, IdRealizacao)

#define VARIAVEL_DECISAO_2(m)\
	m(  VarDecisao,     ZF,                      2,   ITERADORES_3_ESTAGIO_REALIZACAO)  \
	m(  VarDecisao,     ZT,                      2,   ITERADORES_3_ESTAGIO_REALIZACAO)  \
	m(  VarDecisao,     ZP0_VF_FINF,             2,   ITERADORES_2_ESTAGIO_PERIODO)  \
	m(  VarDecisao,     ZP0_VF_FINF_ADD,         2,   ITERADORES_2_ESTAGIO_PERIODO)  \
	m(  VarDecisao,     ZO0,                     2,    ITERADORES_2_ESTAGIO_PERIODO)  \
	m(  VarDecisao,     ZO,                      2,    ITERADORES_2_ESTAGIO_PERIODO)  \
	m(  VarDecisao,     ZP0,                     2,    ITERADORES_2_ESTAGIO_PERIODO)  \
	m(  VarDecisao,     ZP,                      2,    ITERADORES_2_ESTAGIO_PERIODO)  \
	m(  VarDecisao,     ESTADO,                  2,    ITERADORES_2_ESTAGIO_VARIAVELESTADO)  
   //   Elemento,       Nome,               NroIter,    Iteradores,                                         


#define EQUACAO_LINEAR_2(m)\
	m(  EquLinear,     ZP0_VF_FINF,         2,    ITERADORES_2_ESTAGIO_PERIODO)  \
	m(  EquLinear,     ZP0,                 2,    ITERADORES_2_ESTAGIO_PERIODO)  \
	m(  EquLinear,     ZP,                  2,    ITERADORES_2_ESTAGIO_PERIODO)  \
	m(  EquLinear,     ZO0,                 2,    ITERADORES_2_ESTAGIO_PERIODO)  \
	m(  EquLinear,     ZO,                  2,    ITERADORES_2_ESTAGIO_PERIODO)  

   //   Elemento,     Nome,                           NroIter,    Iteradores,                                   

//
// ITERADORES 3
//

#define ITERADORES_3_ESTAGIO_PERIODO_PERIODO(m)               m(1, IdEstagio) m(2, Periodo)        m(3, Periodo)
#define ITERADORES_3_ESTAGIO_PERIODO_PATAMARCARGA(m)          m(1, IdEstagio) m(2, Periodo)        m(3, IdPatamarCarga)
#define ITERADORES_3_ESTAGIO_PERIODO_TERMELETRICA(m)          m(1, IdEstagio) m(2, Periodo)        m(3, IdTermeletrica)
#define ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA(m)          m(1, IdEstagio) m(2, Periodo)        m(3, IdHidreletrica)
#define ITERADORES_3_ESTAGIO_PERIODO_SUBMERCADO(m)            m(1, IdEstagio) m(2, Periodo)        m(3, IdSubmercado)
#define ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE(m) m(1, IdEstagio) m(2, Periodo)        m(3, IdRestricaoOperativaUHE)
#define ITERADORES_3_ESTAGIO_PERIODO_USINAELEVATORIA(m)       m(1, IdEstagio) m(2, Periodo)        m(3, IdUsinaElevatoria)
#define ITERADORES_3_ESTAGIO_PERIODO_VARIAVELALEATORIA(m)     m(1, IdEstagio) m(2, Periodo)        m(3, IdVariavelAleatoria)
#define ITERADORES_3_ESTAGIO_REALIZACAO_CORTEBENDERS(m)       m(1, IdEstagio) m(2, IdRealizacao)   m(3, IdCorteBenders)

#define VARIAVEL_DECISAO_3(m)\
	m(  VarDecisao,   ZP,               3,         ITERADORES_3_ESTAGIO_PERIODO_PATAMARCARGA)  \
	m(  VarDecisao,   ZO,               3,         ITERADORES_3_ESTAGIO_PERIODO_PATAMARCARGA)  \
	m(  VarDecisao,   ZP0_VF_FINF,      3,         ITERADORES_3_ESTAGIO_PERIODO_PERIODO)  \
	m(  VarDecisao,   YH,               3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
	m(  VarDecisao,   YHF,              3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
	m(  VarDecisao,   ENA,              3,         ITERADORES_3_ESTAGIO_PERIODO_PERIODO)  \
	m(  VarDecisao,   QE,               3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
	m(  VarDecisao,   QE_REL,           3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
	m(  VarDecisao,   QE_FINF,          3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
    m(  VarDecisao,   QD,               3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
    m(  VarDecisao,   QD_FINF,          3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
    m(  VarDecisao,   QD_FSUP,          3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
	m(  VarDecisao,   VI,               3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
	m(  VarDecisao,   VF,               3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
	m(  VarDecisao,   VMED,             3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
    m(  VarDecisao,   VF_FINF,          3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
	m(  VarDecisao,   VMORTO_FINF,      3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
	m(  VarDecisao,   PL,               3,         ITERADORES_3_ESTAGIO_PERIODO_SUBMERCADO)  \
	m(  VarDecisao,   PBDISP,           3,         ITERADORES_3_ESTAGIO_PERIODO_USINAELEVATORIA)  \
	m(  VarDecisao,   PD,               3,         ITERADORES_3_ESTAGIO_PERIODO_SUBMERCADO)  \
	m(  VarDecisao,   PHDISP,           3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
    m(  VarDecisao,   PHDISPMETA,       3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
    m(  VarDecisao,   PHDISPMETA_FINF,  3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
    m(  VarDecisao,   PHDISPMETA_FSUP,  3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
	m(  VarDecisao,   QV,               3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
	m(  VarDecisao,   QTDISP,           3,         ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
	m(  VarDecisao,   PTDISP,           3,         ITERADORES_3_ESTAGIO_PERIODO_TERMELETRICA)  \
	m(  VarDecisao,   PTDISPCOM,        3,         ITERADORES_3_ESTAGIO_PERIODO_TERMELETRICA)  \
	m(  VarDecisao,   PI,               3,         ITERADORES_3_ESTAGIO_PERIODO_SUBMERCADO)  \
	m(  VarDecisao,   PN,               3,         ITERADORES_3_ESTAGIO_PERIODO_SUBMERCADO)  \
    m(  VarDecisao,   RHA,              3,         ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE)  \
    m(  VarDecisao,   RHA_FINF,         3,         ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE)  \
    m(  VarDecisao,   RHA_FSUP,         3,         ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE)  \
    m(  VarDecisao,   RHV,              3,         ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE)  \
    m(  VarDecisao,   RHV_FINF,         3,         ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE)  \
    m(  VarDecisao,   RHV_FSUP,         3,         ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE)  \
    m(  VarDecisao,   RHE_FINF,         3,         ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE)  \
    m(  VarDecisao,   RHE_FSUP,         3,         ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE)
   //   Elemento,     Nome,       NroIter,         Iteradores,                                                        

#define EQUACAO_LINEAR_3(m)\
	m(  EquLinear,   ZP,                3,   ITERADORES_3_ESTAGIO_PERIODO_PATAMARCARGA)  \
	m(  EquLinear,   ZO,                3,   ITERADORES_3_ESTAGIO_PERIODO_PATAMARCARGA)  \
	m(  EquLinear,   ZP0_VF_FINF,       3,   ITERADORES_3_ESTAGIO_PERIODO_PERIODO)  \
	m(  EquLinear,   BH_VOL,            3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
	m(  EquLinear,   VMED,              3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
	m(  EquLinear,   YH,                3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
	m(  EquLinear,   PL,                3,   ITERADORES_3_ESTAGIO_PERIODO_SUBMERCADO)  \
	m(  EquLinear,   PD,                3,   ITERADORES_3_ESTAGIO_PERIODO_SUBMERCADO)  \
	m(  EquLinear,   PI,                3,   ITERADORES_3_ESTAGIO_PERIODO_SUBMERCADO)  \
	m(  EquLinear,   PBDISP,            3,   ITERADORES_3_ESTAGIO_PERIODO_USINAELEVATORIA)  \
	m(  EquLinear,   PHDISP,            3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
	m(  EquLinear,   PTDISP,            3,   ITERADORES_3_ESTAGIO_PERIODO_TERMELETRICA)  \
	m(  EquLinear,   PTDISPCOM,         3,   ITERADORES_3_ESTAGIO_PERIODO_TERMELETRICA)  \
	m(  EquLinear,   PN,                3,   ITERADORES_3_ESTAGIO_PERIODO_SUBMERCADO)  \
	m(  EquLinear,   QE,                3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
	m(  EquLinear,   QE_REL,            3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
	m(  EquLinear,   QTDISP,            3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
	m(  EquLinear,   QD,                3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
	m(  EquLinear,   QV,                3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
    m(  EquLinear,   PHDISPMETA,        3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  
	//  Elemento,    Nome,                                     NroIter,   Iteradores,                                         

#define INEQUACAO_LINEAR_3(m)\
	m(  IneLinear,   CB_ZF,     3,   ITERADORES_3_ESTAGIO_REALIZACAO_CORTEBENDERS)  \
	m(  IneLinear,   CB_ZT,     3,   ITERADORES_3_ESTAGIO_REALIZACAO_CORTEBENDERS)  \
    m(  IneLinear,   RHV,       3,   ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE)  \
    m(  IneLinear,   RHV_FINF,  3,   ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE)  \
    m(  IneLinear,   RHV_FSUP,  3,   ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE)  \
    m(  IneLinear,   RHE_FINF,  3,   ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE)  \
    m(  IneLinear,   RHE_FSUP,  3,   ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE)  \
    m(  IneLinear,   RHA_FINF,  3,   ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE)  \
    m(  IneLinear,   RHA_FSUP,  3,   ITERADORES_3_ESTAGIO_PERIODO_RESTRICAOOPERATIVAUHE)  \
	m(  IneLinear,   QD_FINF,   3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
    m(  IneLinear,   QD_FSUP,   3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  \
    m(  IneLinear,   VF_FINF,   3,   ITERADORES_3_ESTAGIO_PERIODO_HIDRELETRICA)  
   //   Elemento,    Nome,                                     NroIter,   Iteradores,                                                 

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
#define ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_AGRUPAMENTOPI(m)            m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdAgrupamentoIntercambio)
#define ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_PI(m)                       m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdIntercambio)
#define ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_PLESPECIAL(m)               m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdDemandaEspecial)
#define ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_RESTRICAOOPERATIVAUHE(m)    m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdRestricaoOperativaUHE)
#define ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_PIHIDRAULICO(m)             m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdIntercambioHidraulico)




#define VARIAVEL_DECISAO_4(m)\
	m(  VarDecisao,    ENA,                  4,    ITERADORES_4_ESTAGIO_PERIODO_PERIODO_REE)  \
	m(  VarDecisao,    PH,                   4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
	m(  VarDecisao,    PH_REL,               4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
	m(  VarDecisao,    PH_FINF,              4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
	m(  VarDecisao,    PHDISP,               4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
	m(  VarDecisao,    PHDISP_REL,           4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
	m(  VarDecisao,    PHDISP_FINF,          4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
    m(  VarDecisao,    QB,                   4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_USINAELEVATORIA)  \
    m(  VarDecisao,    QB_FINF,              4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_USINAELEVATORIA)  \
    m(  VarDecisao,    QBDISP,               4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_USINAELEVATORIA)  \
    m(  VarDecisao,    QBDISP_FINF,          4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_USINAELEVATORIA)  \
    m(  VarDecisao,    QTDISP,               4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
    m(  VarDecisao,    QTDISP_FINF,          4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
    m(  VarDecisao,    QT,                   4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
    m(  VarDecisao,    QT_FINF,              4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
	m(  VarDecisao,    QV,                   4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
	m(  VarDecisao,    QM,                   4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
    m(  VarDecisao,    QDLAG,                4,    ITERADORES_4_ESTAGIO_PERIODO_PERIODO_HIDRELETRICA)  \
    m(  VarDecisao,    QDLAG_ADD,            4,    ITERADORES_4_ESTAGIO_PERIODO_PERIODO_HIDRELETRICA)  \
    m(  VarDecisao,    QD,                   4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
    m(  VarDecisao,    QD_FINF,              4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
    m(  VarDecisao,    QD_FSUP,              4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
	m(  VarDecisao,    PL,                   4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_SUBMERCADO)  \
	m(  VarDecisao,    PLE,			         4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_PLESPECIAL)  \
	m(  VarDecisao,    CIMP,		         4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_CONTRATO)  \
	m(  VarDecisao,    CEXP,		         4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_CONTRATO)  \
	m(  VarDecisao,    PBDISP,               4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_USINAELEVATORIA)  \
	m(  VarDecisao,    RE_FINF,              4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_RESTRICAOELETRICA)  \
    m(  VarDecisao,    RE_FSUP,              4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_RESTRICAOELETRICA)  \
	m(  VarDecisao,    PI,                   4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_PI)  \
    m(  VarDecisao,    PI_FINF,              4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_PI)  \
	m(  VarDecisao,    PT,                   4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  VarDecisao,    PE,			         4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_USINAEOLICA)  \
	m(  VarDecisao,    PTUTIL,               4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  VarDecisao,    UTE_ON_T,                4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  VarDecisao,    UTE_OFF_T,               4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  VarDecisao,    ACIO_T,               4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  VarDecisao,    PT_REL,               4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  VarDecisao,    PT_FINF,              4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  VarDecisao,    PT_FSUP,              4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  VarDecisao,    PTDISP,               4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  VarDecisao,    PTDISP_REL,           4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  VarDecisao,    PTDISP_FINF,          4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  VarDecisao,    PTDISP_FSUP,          4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  VarDecisao,    PTDISPCOM,            4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  VarDecisao,    PTDISPPRECOM,         4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  VarDecisao,    PTDISPCOM_FINF,       4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  VarDecisao,    PTDISPCOM_FSUP,       4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
    m(  VarDecisao,    RHQ,                  4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_RESTRICAOOPERATIVAUHE)  \
    m(  VarDecisao,    RHQ_FINF,             4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_RESTRICAOOPERATIVAUHE)  \
    m(  VarDecisao,    RHQ_FSUP,             4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_RESTRICAOOPERATIVAUHE)  \
	m(  VarDecisao,    QILS_TRI,             4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_PIHIDRAULICO)  \
    m(  VarDecisao,    QILS_TRI_FINF,        4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_PIHIDRAULICO)  \
    m(  VarDecisao,    QILS_TRI_REL,         4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_PIHIDRAULICO)  \
    m(  VarDecisao,    QI,                   4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
    m(  VarDecisao,    QI_FINF,              4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)
   //   Elemento,      Nome,             NroIter,    Iteradores,                                                                    

#define EQUACAO_LINEAR_4(m)\
	m(  EquLinear,    PL,                4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_SUBMERCADO)  \
	m(  EquLinear,    BH_VAZ,            4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
	m(  EquLinear,    QM,                4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
    m(  EquLinear,    QD,                4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
    m(  EquLinear,    QD_LAG,            4,    ITERADORES_4_ESTAGIO_PERIODO_PERIODO_HIDRELETRICA)  \
	m(  EquLinear,    QT,                4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
	m(  EquLinear,    QTDISP,            4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
	m(  EquLinear,    PT_REL,            4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  EquLinear,    PTDISP,            4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  EquLinear,    PTDISPCOM,         4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  EquLinear,    PTDISPCOM_REL,     4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  EquLinear,    PT,                4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  EquLinear,    UTE_BIN,           4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
    m(  EquLinear,    PTDISP_REL,        4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  EquLinear,    PBDISP,            4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_USINAELEVATORIA)  \
	m(  EquLinear,    PH,                4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
	m(  EquLinear,    PH_REL,            4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
	m(  EquLinear,    PHDISP,            4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
	m(  EquLinear,    PHDISP_REL,        4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
    m(  EquLinear,    QI_REL,            4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_PIHIDRAULICO)  \
    m(  EquLinear,    QI_FINF,           4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
    m(  EquLinear,    QBDISP,           4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_USINAELEVATORIA)
   //   Elemento,     Nome,                                        NroIter,    Iteradores,                                                                   

#define INEQUACAO_LINEAR_4(m)\
	m(  IneLinear,    RE_FINF,                  4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_RESTRICAOELETRICA)  \
    m(  IneLinear,    RE_FSUP,                  4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_RESTRICAOELETRICA)  \
	m(  IneLinear,    PI_AGRUP,                 4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_AGRUPAMENTOPI)  \
	m(  IneLinear,    VMORTO_ENCH,              4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
	m(  IneLinear,    QD_FINF,                  4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
	m(  IneLinear,    QD_FSUP,                  4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
	m(  IneLinear,    UTE_EQV,                  4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
    m(  IneLinear,    RHQ_FINF,                 4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_RESTRICAOOPERATIVAUHE)  \
    m(  IneLinear,    RHQ_FSUP,                 4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_RESTRICAOOPERATIVAUHE)  \
	m(  IneLinear,    PTUTIL,                   4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  IneLinear,    PT_ON,                    4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  IneLinear,    PT_OFF,                   4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  IneLinear,    PTDISP_FINF,              4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
	m(  IneLinear,    PTDISP_FSUP,              4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA)  \
    m(  IneLinear,    RHQ,                      4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_RESTRICAOOPERATIVAUHE)  \
    m(  IneLinear,    QB_FINF,                  4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_USINAELEVATORIA)  \
    m(  IneLinear,    QBDISP_FINF,              4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_USINAELEVATORIA)  \
    m(  IneLinear,    QTDISP_REL,               4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
    m(  IneLinear,    QT_REL,                   4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA)  \
    m(  IneLinear,    PI_REL,                   4,    ITERADORES_4_ESTAGIO_PERIODO_PATAMARCARGA_PI)
   //   Elemento,     Nome,                                          NroIter,    Iteradores,                                                                 


//
// ITERADORES 5
//

#define ITERADORES_5_ESTAGIO_PERIODO_PERIODO_HIDRELETRICA_REE(m)                        m(1, IdEstagio) m(2, Periodo) m(3, Periodo) m(4, IdHidreletrica) m(5, IdReservatorioEquivalente) 
#define ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_SUBMERCADO_USINANAOSIMULADA(m)        m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdSubmercado) m(5, IdUsinaNaoSimulada)
#define ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_SUBMERCADO_PATAMARPD(m)               m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdSubmercado) m(5, IdPatamarDeficit)
#define ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_INT(m)                   m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdHidreletrica) m(5, int)
#define ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_HIDRELETRICA(m)          m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdHidreletrica) m(5, IdHidreletrica)
#define ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO(m)    m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdHidreletrica) m(5, IdConjuntoHidraulico)
#define ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE(m)            m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdTermeletrica) m(5, IdUnidadeUTE)
#define ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_PIHIDRAULICO_HIDRELETRICA(m)          m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdIntercambioHidraulico) m(5, IdHidreletrica)
#define ITERADORES_5_ESTAGIO_PERIODO_PERIODO_PROCESSOESTOCASTICO_VARIAVELALEATORIA(m)   m(1, IdEstagio) m(2, Periodo) m(3, Periodo) m(4, IdProcessoEstocastico) m(5, IdVariavelAleatoria)

#define VARIAVEL_DECISAO_5(m)\
	m(  VarDecisao,    ENA,         5,     ITERADORES_5_ESTAGIO_PERIODO_PERIODO_HIDRELETRICA_REE)  \
	m(  VarDecisao,    PH,          5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO)  \
	m(  VarDecisao,    PH_REL,      5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO)  \
	m(  VarDecisao,    PH_FINF,     5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO)  \
	m(  VarDecisao,    PHDISP,      5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO)  \
	m(  VarDecisao,    PHDISP_REL,  5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO)  \
	m(  VarDecisao,    PHDISP_FINF, 5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO)  \
    m(  VarDecisao,    QTDISP,      5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO)  \
    m(  VarDecisao,    QT,          5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO)  \
    m(  VarDecisao,    QT_FINF,     5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO)  \
	m(  VarDecisao,    PN,          5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_SUBMERCADO_USINANAOSIMULADA)  \
	m(  VarDecisao,    PT,          5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE)  \
	m(  VarDecisao,    PTUTIL,      5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE)  \
	m(  VarDecisao,    PT_TRAJ_ACIO,5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE)  \
	m(  VarDecisao,    PT_TRAJ_DESL,5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE)  \
	m(  VarDecisao,    UTE_ON_T,    5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE)  \
	m(  VarDecisao,    UTE_OFF_T,   5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE)  \
	m(  VarDecisao,    ACIO_T,      5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE)  \
	m(  VarDecisao,    PT_REL,      5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE)  \
	m(  VarDecisao,    PT_FINF,     5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE)  \
	m(  VarDecisao,    PTDISP,      5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE)  \
	m(  VarDecisao,    RP,          5,     ITERADORES_5_ESTAGIO_PERIODO_PERIODO_PROCESSOESTOCASTICO_VARIAVELALEATORIA)  \
	m(  VarDecisao,    YP,          5,     ITERADORES_5_ESTAGIO_PERIODO_PERIODO_PROCESSOESTOCASTICO_VARIAVELALEATORIA)  \
	m(  VarDecisao,    YPF,         5,     ITERADORES_5_ESTAGIO_PERIODO_PERIODO_PROCESSOESTOCASTICO_VARIAVELALEATORIA)  \
	m(  VarDecisao,    YP_ADD,      5,     ITERADORES_5_ESTAGIO_PERIODO_PERIODO_PROCESSOESTOCASTICO_VARIAVELALEATORIA)  \
	m(  VarDecisao,    PD,          5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_SUBMERCADO_PATAMARPD)  \
    m(  VarDecisao,    QI,          5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_PIHIDRAULICO_HIDRELETRICA)  \
    m(  VarDecisao,    QI_FINF,     5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_PIHIDRAULICO_HIDRELETRICA)  \
    m(  VarDecisao,    QI_REL,      5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_PIHIDRAULICO_HIDRELETRICA)  \
    m(  VarDecisao,    QR,          5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_HIDRELETRICA)  \
    m(  VarDecisao,    QR_FINF,     5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_HIDRELETRICA)   
   //   Elemento,      Nome,  NroIter,     Iteradores,                                                                               


#define EQUACAO_LINEAR_5(m)\
	m(  EquLinear,    YP,             5,     ITERADORES_5_ESTAGIO_PERIODO_PERIODO_PROCESSOESTOCASTICO_VARIAVELALEATORIA)      \
	m(  EquLinear,    QT,             5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO)      \
	m(  EquLinear,    QT_REL,         5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO)      \
	m(  EquLinear,    QTDISP,         5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO)      \
	m(  EquLinear,    PT,             5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE)      \
	m(  EquLinear,    PT_REL,         5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE)      \
	m(  EquLinear,    PTDISP,         5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE)      \
    m(  EquLinear,    PTDISP_REL,     5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE)      \
	m(  EquLinear,    UTE_BIN,        5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE)      \
	m(  EquLinear,    PH,             5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO)      \
	m(  EquLinear,    PH_REL,         5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO)      \
	m(  EquLinear,    PHDISP,         5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO)      \
	m(  EquLinear,    PHDISP_REL,     5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO)      
   //   Elemento,      Nome,                                  NroIter,     Iteradores,                                                                         

#define INEQUACAO_LINEAR_5(m)\
	m(  IneLinear,    PH,            5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_INT)  \
	m(  IneLinear,    PTUTIL,        5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE)  \
	m(  IneLinear,    PT_ON,         5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE)  \
	m(  IneLinear,    PT_OFF,        5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE)  \
	m(  IneLinear,    UTE_ON_T,      5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE)  \
	m(  IneLinear,    UTE_OFF_T,     5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE)  \
    m(  IneLinear,    QR_FINF,       5,     ITERADORES_5_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_HIDRELETRICA)  
   //   Elemento,      Nome,                         NroIter,     Iteradores,                                                                   

//
// ITERADORES 6
//

#define ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_INT(m)             m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdHidreletrica) m(5, IdConjuntoHidraulico) m(6, int)
#define ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE(m)      m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdHidreletrica) m(5, IdConjuntoHidraulico) m(6, IdUnidadeUHE)
#define ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE_PERIODO(m)                 m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdTermeletrica) m(5, IdUnidadeUTE) m(6, Periodo)
#define ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_PIHIDRAULICO_HIDRELETRICA_HIDRELETRICA(m) m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdIntercambioHidraulico) m(5, IdHidreletrica) m(6, IdHidreletrica)

#define VARIAVEL_DECISAO_6(m)\
	m(  VarDecisao,     PH,          6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE)  \
	m(  VarDecisao,     PH_REL,      6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE)  \
	m(  VarDecisao,     PH_FINF,     6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE)  \
	m(  VarDecisao,     PH_FSUP,     6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE)  \
	m(  VarDecisao,     PHDISP,      6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE)  \
	m(  VarDecisao,     PHDISP_REL,  6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE)  \
	m(  VarDecisao,     PHDISP_FINF, 6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE)  \
    m(  VarDecisao,     QTDISP,      6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE)  \
    m(  VarDecisao,     QT,          6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE)  \
    m(  VarDecisao,     QT_FINF,     6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE)  \
    m(  VarDecisao,     QI,          6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_PIHIDRAULICO_HIDRELETRICA_HIDRELETRICA)
   //   Elemento,       Nome,  NroIter,    Iteradores,                                                                                                


#define EQUACAO_LINEAR_6(m)\
	m(  EquLinear,     QT_REL,         6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE)      \
	m(  EquLinear,     QTDISP,         6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE)      \
	m(  EquLinear,     PH,             6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE)      \
	m(  EquLinear,     PH_REL,         6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE)      \
	m(  EquLinear,     PHDISP,         6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE)      \
	m(  EquLinear,     PHDISP_REL,     6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE)      
   //   Elemento,       Nome,                                  NroIter,    Iteradores,                                                                                                 

#define INEQUACAO_LINEAR_6(m)\
	m(  IneLinear,     UTE_ON_T_FINF,         6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE_PERIODO)  \
	m(  IneLinear,     UTE_ON_T_FSUP,         6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE_PERIODO)  \
	m(  IneLinear,     UTE_ON_T,              6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE_PERIODO)  \
	m(  IneLinear,     PH,                    6,    ITERADORES_6_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_INT)  
   //   Elemento,       Nome,                             NroIter,    Iteradores,                                                                                      


//
// ITERADORES 7
//

#define ITERADORES_7_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE_INT(m)   m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdHidreletrica) m(5, IdConjuntoHidraulico) m(6, IdUnidadeUHE) m(7, int)
#define ITERADORES_7_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE_PERIODO_INT(m)              m(1, IdEstagio) m(2, Periodo) m(3, IdPatamarCarga) m(4, IdTermeletrica) m(5, IdUnidadeUTE) m(6, Periodo) m(7, int)


#define INEQUACAO_LINEAR_7(m)\
	m(  IneLinear,     UTE_OFF_T_FINF,    7,    ITERADORES_7_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE_PERIODO_INT)  \
	m(  IneLinear,     UTE_OFF_T_FSUP,    7,    ITERADORES_7_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE_PERIODO_INT)  \
	m(  IneLinear,     UTE_OFF_T,         7,    ITERADORES_7_ESTAGIO_PERIODO_PATAMARCARGA_TERMELETRICA_UNIDADEUTE_PERIODO_INT)  \
	m(  IneLinear,     PH,                7,    ITERADORES_7_ESTAGIO_PERIODO_PATAMARCARGA_HIDRELETRICA_CONJUNTOHIDRAULICO_UNIDADEUHE_INT)  
   //   Elemento,      Nome,                        NroIter,    Iteradores,                                                                                   






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

		void gerarRealizacoes(const IdEstagio a_estagioIni, const IdEstagio a_estagioEnd, const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, EntradaSaidaDados a_entradaSaidaDados);

		void atualizarModeloOtimizacaoComVariavelEstado(const IdEstagio a_idEstagio, const IdCenario a_idCenario);
		void atualizarModeloOtimizacaoComVariavelEstado(const IdEstagio a_idEstagio, const IdCenario a_idCenario, EstruturaResultados<double> &a_retorno_estados);
		
		void atualizarModeloOtimizacaoComVariavelEstado_posEstudo(const IdEstagio a_idEstagio, const IdEstagio a_idEstagio_final, const IdCenario a_idCenario_estado, const IdCenario a_idCenario, IdRealizacao a_idRealizacao, Dados& a_dados);

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

		void imprimirSolucaoPorEstagio(const IdProcesso a_idProcesso, const IdEstagio a_idEstagio, const std::string a_subdiretorio, EntradaSaidaDados a_entradaSaidaDados);

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

		int getNumeroVariavelDinamica(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio);

		double atualizar_ENA_acoplamento(Dados& a_dados, const IdReservatorioEquivalente a_idReservatorioEquivalente, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const Periodo a_periodo_lag);
		void defineHidreletricasMontanteNaCascataENA(Dados& a_dados);
		void retorna_equacionamento_regras_afluencia_natural_x_idHidreletrica(Dados& a_dados, const Periodo a_periodoPE, const int a_codigo_posto, const int a_codigo_posto_acoplamento_ENA, const IdHidreletrica a_idHidreletrica, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const IdProcessoEstocastico a_idProcessoEstocastico, std::vector<SmartEnupla<IdHidreletrica, double>>& a_coeficiente_idHidreletricas_calculo_ENA, double& a_termo_independente_calculo_ENA);
		void retorna_equacionamento_afluencia_natural_x_posto(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const int a_codigo_posto, const double a_coeficiente, std::vector<SmartEnupla<IdHidreletrica, double>>& a_coeficiente_idHidreletricas_calculo_ENA);
		double get_afluencia_natural_posto(Dados& a_dados, const Periodo a_periodoPE, const int a_codigo_posto, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const IdProcessoEstocastico a_idProcessoEstocastico);
		double get_afluencia_incremental_from_idVariavelAleatoria(const IdVariavelAleatoria a_idVariavelAleatoria, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const Periodo a_periodoPE);
		IdMes get_IdMes_operativo(const Periodo a_periodoPE, const Periodo a_periodo_inicial_semanal);
		bool get_is_variavelEstado_ENA();

	private:

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//SmartEnuplas necesárias no cálculo dinâmico das ENAs
		SmartEnupla<IdReservatorioEquivalente, SmartEnupla<int, IdHidreletrica>> lista_idREE_idHidreletricas; //aqui
		SmartEnupla<IdHidreletrica, SmartEnupla<int, IdHidreletrica>> idHidreletricas_x_usina_calculo_ENA;
		SmartEnupla<int, IdHidreletrica> lista_codPosto_idHidreletrica;
		SmartEnupla<IdHidreletrica, IdVariavelAleatoria> mapIdVar;
		SmartEnupla<IdHidreletrica, IdVariavelAleatoriaInterna> mapIdVarInterna;

		//////////////////////////////////////////////////////////////////////////////////////////////////

		int isPrintElemSync = 0;

		std::vector<std::vector<std::string>> lista_elemento_impressao;

		IdCorteBenders maiorIdCorteExportadoPreEstudo = IdCorteBenders_Nenhum;


			VARIAVEL_DECISAO_1(DECLARAR_METODOS_ELEMENTO)
			VARIAVEL_DECISAO_2(DECLARAR_METODOS_ELEMENTO)
			VARIAVEL_DECISAO_3(DECLARAR_METODOS_ELEMENTO)
			VARIAVEL_DECISAO_4(DECLARAR_METODOS_ELEMENTO)
			VARIAVEL_DECISAO_5(DECLARAR_METODOS_ELEMENTO)
			VARIAVEL_DECISAO_6(DECLARAR_METODOS_ELEMENTO)

			EQUACAO_LINEAR_1(DECLARAR_METODOS_ELEMENTO)
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

		int criarVariaveisDecisao_VariaveisEstado_Restricoes_ZP0_VF_FINF(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo_penalizacao);
		int criarVariaveisDecisao_VariaveisEstado_Restricoes_QDLAG(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo, const IdHidreletrica a_idHidreletrica, Periodo &a_periodo_lag);
		int criarVariaveisDecisao_VariaveisEstado_Restricoes_YP(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo, const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria, Periodo &a_periodo_lag, const double a_grau_liberdade, std::vector<IdHidreletrica> a_idHidreletrica = std::vector<IdHidreletrica>());
		int criarVariaveisDecisao_VariaveisEstado_Restricoes_PTDISPCOM(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo, const IdPatamarCarga a_idPatamarCarga, const IdTermeletrica a_idTermeletrica, const double a_potencia_disponivel_minima, const double a_potencia_disponivel_maxima);

		void criarModeloOtimizacao(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados);

		void criarVariaveisTermeletricas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodIni_stage, Periodo& a_periodPrev, Periodo& a_period, const IdPatamarCarga a_idPat);

		void criarVariaveisUsinaNaoSimulada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat);
		
		void criarVariaveisIntercambio(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat);
		
		void criarVariaveisContrato(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat);

		void criarVariaveisEolicas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat);

		void criarVariaveisDemandaEspecial(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat);
		
		void criarVariaveisDemanda(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat);
		
		void criarVariaveisDeficit(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat);
		
		void criarVariaveisRestricaoEletrica(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat);

		void criarVariaveisHidraulicas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo_estudo, const IdPatamarCarga a_idPat);


		void criarVariaveisCusto(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio);
		void criarVariaveisCusto(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period);
		void criarVariaveisCusto(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat);

		void criarRestricoesEvaporacao(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo_estudo, const IdHidreletrica a_idHidreletrica, Periodo &a_proximo_periodo_estudo);

		void criarVariaveisDecisao_Restricoes_ProcessoEstocasticoHidrologico(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodIni_stage, Periodo& a_period_stage, Periodo& a_periodEnd_stage, const SmartEnupla<Periodo, int>& a_horizonSP, const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>& a_horizon);

		void criarRestricoesPotenciaHidraulicaDisponivel(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdPatamarCarga a_idPatamarCarga, Periodo& a_period, const IdHidreletrica a_idHidreletrica);

		void criarRestricoesVolumeUtil_e_Penalidade(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo_estudo, const IdHidreletrica a_idHidreletrica);

		void criarRestricoesFuncaoProducaoHidreletrica(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga, Periodo& a_period, Periodo& a_periodNext, Periodo& a_periodEnd_stage);

		void criarRestricoesTermeletrica(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdPatamarCarga a_idPat, Periodo& a_periodIni_stage, Periodo& a_periodEnd_stage, Periodo& a_periodPrev, Periodo& a_period, const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>& a_horizon);

		void criarVariaveisVolume(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodIni_stage, Periodo& a_periodEnd_stage, Periodo& a_periodPrev, Periodo& a_period, Periodo& a_periodNext);

		void criarRestricoesCustoPenalidade(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo_estudo, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesCustoPenalidade(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo_estudo);

		void criarRestricoesTurbinamentoHidraulico(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo_estudo, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesVazaoDefluente(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo_estudo, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesVazaoDesviada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo_estudo, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesVazaoRetirada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo_estudo, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesBombeamentoHidraulico(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodo_estudo, const IdPatamarCarga a_idPatamarCarga, const IdUsinaElevatoria a_idUsinaElevatoria);

		void criarRestricoesVazaoBombeada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo_estudo, const IdPatamarCarga a_idPatamarCarga, const IdUsinaElevatoria a_idUsinaElevatoria);

		void criarRestricoesUsinaElevatoria(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat);

		void criarRestricoesEletricas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo_estudo, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesAgrupamentoIntercambio(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo_estudo, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesCorteBendersEmCustoFuturo(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio);
		void criarRestricoesCorteBendersEmCustoTotal(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio);

		void criarRestricoesCusto(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period);

		void criarRestricoesCusto(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo_estudo, const IdPatamarCarga a_idPat);

		void criarRestricoesCustoOperacao(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo_estudo, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesInformacaoMedia(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat);

		void criarRestricoesHidraulicas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodIni_stage, Periodo& a_periodEnd_stage, Periodo& a_periodPrev, Periodo& a_period, Periodo& a_periodNext, const IdPatamarCarga a_idPatamarCarga);
		
		void criarRestricoesBalancoHidraulicoUsinaByVolume(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodIni_stage, Periodo& a_periodEnd_stage, Periodo& a_periodPrev, Periodo& a_period, Periodo& a_periodNext, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesBalancoHidraulicoUsinaByVazao(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodIni_stage, Periodo& a_periodEnd_stage, Periodo& a_periodPrev, Periodo& a_period, Periodo& a_periodNext, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesAtendimentoDemanda(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period, const IdPatamarCarga a_idPat);

		void formularModeloOtimizacao(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados);

		void formularModeloOtimizacao(const SmartEnupla<IdEstagio, std::vector<TipoSubproblemaSolver>>& a_listaTSS, Dados& a_dados, const IdEstagio a_idEstagioIni, const IdEstagio a_idEstagioEnd, const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>& a_horizonte_estudo);

		void criarRestricoesHidraulicaEspecial_vazao_afluente(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo_estudo);

		void criarRestricoesHidraulicaEspecial_vazao_defluente(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo_estudo, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesHidraulicaEspecial_volume_armazenado(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodIni_stage, Periodo& a_periodEnd_stage, Periodo& a_period, Periodo& a_periodNext, const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>& a_horizonte_estudo);

		void criarRestricoesHidraulicaEspecial_energia_armazenada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_periodEnd_stage, Periodo& a_period, Periodo& a_periodNext);

		void criarRestricoesIntercambioHidraulico(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo_estudo, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesIntercambio(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo &a_periodo_estudo, const IdPatamarCarga a_idPatamarCarga);

		void tratarVariaveisFolga(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, Periodo& a_period);

		IdHidreletrica getIdHidreletricaFromIdProcessoEstocasticoIdVariavelAleatoriaIdVariavelAleatoriaInterna(const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria, const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna);
		std::vector<IdHidreletrica> getIdHidreletricaFromIdProcessoEstocasticoIdVariavelAleatoria(const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria);

		void getIdVariavelAleatoriaIdVariavelAleatoriaInternaFromIdHidreletrica(const IdProcessoEstocastico a_idProcessoEstocastico, IdVariavelAleatoria &a_idVariavelAleatoria, IdVariavelAleatoriaInterna& a_idVariavelAleatoriaInterna, const IdHidreletrica a_hidreletrica);

		IdVariavelAleatoriaInterna getIdVariavelAleatoriaInternaFromIdVariavelAleatoriaIdHidreletrica(const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria, const IdHidreletrica a_hidreletrica);

};

GET_STRING_FROM_CLASS(ModeloOtimizacao)

#endif 
