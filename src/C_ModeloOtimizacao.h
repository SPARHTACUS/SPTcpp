#ifndef MODELO_OTIMIZACAO
#define MODELO_OTIMIZACAO

#include "C_SmartElemento.h"

#include "C_Dados.h"

#include "C_ProcessoEstocastico.h"

#include "C_EstagioOtimizacao.h"
#include "C_Iteracao.h"

#include "C_VariavelDecisao.h"
#include "C_RestricaoEquacaoLinear.h"
#include "C_RestricaoInequacaoLinear.h"

#include "C_EstruturaResultados.h"

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
	  m(ModeloOtimizacao,  AttComum,                            numero_cenarios_tendencia_hidrologica,                                int,          1,          max,          1,      sim) \
	  m(ModeloOtimizacao,  AttComum,                               numero_maximo_tentativas_resolucao,                                int,          1,         1000,        100,      sim) \
	  m(ModeloOtimizacao,  AttComum,                             multiplicador_tempo_limite_resolucao,                             double,        min,         max,         100,      nao) \
	  m(ModeloOtimizacao,  AttComum,                                                    foco_numerico,                                int,          1,            4,          2,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                           tolerancia_otimalidade,                             double,       1e-12,         1e-2,       1e-6,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                           tolerancia_viabilidade,                             double,       1e-12,         1e-2,       1e-6,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                       tipo_selecao_solucao_proxy,            TipoSelecaoSolucaoProxy,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                         realizar_simulacao_final,                               bool,        min,          max,        sim,      nao) \
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
	  m(ModeloOtimizacao,  AttComum,                                                  cenario_inicial,                          IdCenario,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                                    cenario_final,                          IdCenario,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                                 iteracao_inicial,                         IdIteracao,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                                   iteracao_final,                         IdIteracao,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                       periodo_otimizacao_inicial,                            Periodo,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                         periodo_otimizacao_final,                            Periodo,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                           periodo_estudo_inicial,                            Periodo,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                             periodo_estudo_final,                            Periodo,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                                             tipo_aversao_a_risco,                   TipoAversaoRisco,        min,          max,        min,      sim) \
	  m(ModeloOtimizacao,  AttComum,                            tipo_processo_estocastico_hidrologico,              IdProcessoEstocastico,        Nenhum,       max,        Nenhum,   sim) \
	  m(ModeloOtimizacao,  AttComum,                             tipo_relaxacao_afluencia_incremental,  TipoRelaxacaoAfluenciaIncremental,        Nenhum,       max,        Nenhum,   sim) \
	  m(ModeloOtimizacao,  AttComum,                                tipo_funcao_producao_hidreletrica,     TipoFuncaoProducaoHidreletrica,        Nenhum,       max,        Nenhum,   sim) 

#define ATT_VETOR_MODELO_OTIMIZACAO(m)  \
	  m(ModeloOtimizacao,  AttVetor,           custo_total,         double,        0,          max,           0,  IdEstagio) \
	  m(ModeloOtimizacao,  AttVetor,        custo_imediato,         double,        0,          max,           0,  IdEstagio) \
	  m(ModeloOtimizacao,  AttVetor,          custo_futuro,         double,        0,          max,           0,  IdEstagio) \
	  m(ModeloOtimizacao,  AttVetor,      horizonte_estudo,      IdEstagio,        0,          max,           0,  Periodo) \
	  m(ModeloOtimizacao,  AttVetor, tratamento_inviabilidade,         int,       -1,          max,           0,  IdEstagio) \
	  m(ModeloOtimizacao,  AttVetor,      cortes_multiplos,            int,        0,          max,           0,  IdEstagio) \
	  m(ModeloOtimizacao,  AttVetor,           lambda_CVAR,         double,        0,            1,           0,  IdEstagio) \
	  m(ModeloOtimizacao,  AttVetor,            alpha_CVAR,         double,        0,            1,           0,  IdEstagio) 

#define ATT_MATRIZ_MODELO_OTIMIZACAO(m)  \
	  m(ModeloOtimizacao,  AttMatriz,  percentual_duracao_patamar_carga,                               double,          0,            1,             0,    Periodo,   IdPatamarCarga) \
	  m(ModeloOtimizacao,  AttMatriz,  horizonte_processo_estocastico_hidrologico,                     double,        min,          max,           min,  IdEstagio,  Periodo) \
	  m(ModeloOtimizacao,  AttMatriz,  horizonte_estudo,                                               double,        min,          max,           min,  IdEstagio,  Periodo) \
	  m(ModeloOtimizacao,  AttMatriz,  custo_medio,                                                    double,        min,          max,           min,  IdEstagio,  IdRealizacao) \
	  m(ModeloOtimizacao,  AttMatriz,  realizacao_solucao_proxy,                                          int,        min,          max,           min,  IdEstagio,  IdRealizacao)  

#define MEMBRO_MODELO_OTIMIZACAO(m)         \
	m(ModeloOtimizacao, Estagio)            \
	m(ModeloOtimizacao, Iteracao)            \
	m(ModeloOtimizacao, ProcessoEstocastico) 

#define SMART_ELEMENTO_MODELO_OTIMIZACAO(m) \
	m(ModeloOtimizacao, AttComum,  ATT_COMUM_MODELO_OTIMIZACAO) \
	m(ModeloOtimizacao, AttVetor,  ATT_VETOR_MODELO_OTIMIZACAO) \
	m(ModeloOtimizacao, AttMatriz, ATT_MATRIZ_MODELO_OTIMIZACAO) \
	m(ModeloOtimizacao, Membro, MEMBRO_MODELO_OTIMIZACAO)

DEFINE_SMART_ELEMENTO(ModeloOtimizacao, SMART_ELEMENTO_MODELO_OTIMIZACAO)


#define VARIAVEL_DECISAO_2(m)                                            \
	m(  ZP0,                      IdEstagio,               Periodo,            sim,          nao)  \
	m(  ZP0p,                     IdEstagio,               Periodo,            sim,          nao)  \
	m(  ZPp,                      IdEstagio,               Periodo,            sim,          nao)  \
	m(  ZO0,                      IdEstagio,               Periodo,            sim,          nao)  \
	m(  ZO0p,                     IdEstagio,               Periodo,            sim,          nao)  \
	m(  ZOp,                      IdEstagio,               Periodo,            sim,          nao)  \
	m(  ZO0_UTE,                  IdEstagio,               Periodo,            sim,          nao)  \
	m(  ZO0_DEFICIT,              IdEstagio,               Periodo,            sim,          nao)  \
	m(  ZO0_UTE_COMANDO,          IdEstagio,               Periodo,            sim,          nao)  \
	m(  ZT,                       IdEstagio,               Periodo,            sim,          nao)  \
	m(  ZI,                       IdEstagio,               Periodo,            sim,          nao)  \
	m(  ZPp_VF_FINF,            IdEstagio,               Periodo,            sim,          nao)  \
	m(  ZP0p_VF_FINF,           IdEstagio,               Periodo,            sim,          nao)  \
	m(  ZP0_VF_FINF,            IdEstagio,               Periodo,            sim,          nao)  \
	m(  ZP0_VF_FINF_ACUMULADO,  IdEstagio,               Periodo,            sim,          nao)  \
	m(  ZP0_VF_FINF_ACUMULANDO, IdEstagio,               Periodo,            sim,          nao)  \
	m(  ZPp_QDEF_FINF,            IdEstagio,               Periodo,            sim,          nao)  \
	m(  ZP0p_QDEF_FINF,           IdEstagio,               Periodo,            sim,          nao)  \
	m(  ZP0_QDEF_FINF,            IdEstagio,               Periodo,            sim,          nao)  \
	m(  ZPp_QDEF_FSUP,            IdEstagio,               Periodo,            sim,          nao)  \
	m(  ZP0p_QDEF_FSUP,           IdEstagio,               Periodo,            sim,          nao)  \
	m(  ZP0_QDEF_FSUP,            IdEstagio,               Periodo,            sim,          nao)  \
	m(  ZP,                       IdEstagio,               Periodo,            sim,          nao)  \
	m(  ESTADO,                   IdEstagio,      IdVariavelEstado,            sim,          nao)  \
	m(  ZF,                       IdEstagio,               Periodo,            sim,          nao)  
// Nome,  TipoIterador1,         TipoIterador2, ImprimirPrimal, ImprimirDual

#define VARIAVEL_DECISAO_3(m)                                                                     \
	m(  ZP,               IdEstagio,          Periodo,            IdPatamarCarga,        sim,          nao)  \
	m(  ZO,               IdEstagio,          Periodo,            IdPatamarCarga,        sim,          nao)  \
	m(  ZF,               IdEstagio,          Periodo,            IdRealizacao,          sim,          nao)  \
	m(  ZT,               IdEstagio,          Periodo,            IdRealizacao,          sim,          nao)  \
	m(  ZO0_UTE,          IdEstagio,          Periodo,            IdTermeletrica,        sim,          nao)  \
	m(  ZOp_UTE,          IdEstagio,          Periodo,            IdTermeletrica,        sim,          nao)  \
	m(  ZO0p_UTE,         IdEstagio,          Periodo,            IdTermeletrica,        sim,          nao)  \
	m(  ZO0_DEFICIT,      IdEstagio,          Periodo,            IdTermeletrica,        sim,          nao)  \
	m(  ZO0p_DEFICIT,     IdEstagio,          Periodo,            IdTermeletrica,        sim,          nao)  \
	m(  ZOp_DEFICIT,      IdEstagio,          Periodo,            IdTermeletrica,        sim,          nao)  \
	m(  ZOp_UTE_COMANDO,  IdEstagio,          Periodo,            IdTermeletrica,        sim,          nao)  \
	m(  ZO0p_UTE_COMANDO, IdEstagio,          Periodo,            IdTermeletrica,        sim,          nao)  \
	m(  ZO0_UTE_COMANDO,  IdEstagio,          Periodo,            IdTermeletrica,        sim,          nao)  \
	m(  YH,       IdEstagio,               Periodo,        IdHidreletrica,              sim,          nao)  \
	m(  YHF,      IdEstagio,               Periodo,        IdHidreletrica,              sim,          nao)  \
	m(  QEV,      IdEstagio,               Periodo,        IdHidreletrica,              sim,          nao)  \
	m(  QEV_REL,  IdEstagio,               Periodo,        IdHidreletrica,              sim,          nao)  \
	m(  QEV_FINF, IdEstagio,               Periodo,        IdHidreletrica,              sim,          nao)  \
    m(  QDEF,          IdEstagio,               Periodo,        IdHidreletrica,              sim,          nao)  \
    m(  QDEF_FINF,     IdEstagio,               Periodo,        IdHidreletrica,              sim,          nao)  \
    m(  QDEF_FSUP,     IdEstagio,               Periodo,        IdHidreletrica,              sim,          nao)  \
    m(  VDEF_ACUMULADO,   IdEstagio,      IdHidreletrica,               Periodo,              sim,          nao)  \
    m(  VDEF,             IdEstagio,      IdHidreletrica,               Periodo,              sim,          nao)  \
    m(  VDEF_ACOPLAMENTO, IdEstagio,      IdHidreletrica,               Periodo,              sim,          nao)  \
    m(  YP_ACUMULADO,     IdEstagio,             Periodo,   IdVariavelAleatoria,         sim,          nao)  \
    m(  YP_ACUMULANDO,        IdEstagio,         Periodo,   IdVariavelAleatoria,              sim,          nao)  \
	m(  VI,      IdEstagio,               Periodo,        IdHidreletrica,              sim,          sim)  \
	m(  VF,      IdEstagio,               Periodo,        IdHidreletrica,              sim,          nao)  \
	m(  VMED,      IdEstagio,               Periodo,        IdHidreletrica,              sim,          nao)  \
    m(  VF_FINF, IdEstagio,               Periodo,        IdHidreletrica,              sim,          nao)  \
	m(  VMORTO_FINF,IdEstagio,               Periodo,        IdHidreletrica,              sim,          nao)  \
	m(  PL,        IdEstagio,               Periodo,          IdSubmercado,              sim,          nao)  \
	m(  PBOMDISP,  IdEstagio,               Periodo,     IdUsinaElevatoria,              sim,          nao)  \
	m(  PD,        IdEstagio,               Periodo,          IdSubmercado,              sim,          nao)  \
	m(  PHDISP,    IdEstagio,               Periodo,        IdHidreletrica,              sim,          nao)  \
    m(  PHDISPMETA,    IdEstagio,               Periodo,        IdHidreletrica,              sim,          nao)  \
    m(  PHDISPMETA_FINF,    IdEstagio,               Periodo,        IdHidreletrica,              sim,          nao)  \
    m(  PHDISPMETA_FSUP,    IdEstagio,               Periodo,        IdHidreletrica,              sim,          nao)  \
	m(  QVER,      IdEstagio,               Periodo,        IdHidreletrica,              sim,          nao)  \
	m(  QTURDISP,  IdEstagio,               Periodo,        IdHidreletrica,              sim,          nao)  \
	m(  PTDISP,    IdEstagio,               Periodo,        IdTermeletrica,              sim,          nao)  \
	m(  PTDISPPRECOM,     IdEstagio,     IdTermeletrica,        Periodo,              sim,          nao)  \
	m(  PTDISPCOM_FINF,   IdEstagio,     IdTermeletrica,        Periodo,              sim,          nao)  \
	m(  PTDISPCOM_FSUP,   IdEstagio,     IdTermeletrica,        Periodo,              sim,          nao)  \
	m(  PI,        IdEstagio,               Periodo,          IdSubmercado,              sim,          nao)  \
	m(  PN,        IdEstagio,               Periodo,          IdSubmercado,              sim,          nao)  \
    m(  RHA,       IdEstagio,               Periodo,    IdRestricaoOperativaUHE,         sim,          nao)  \
    m(  RHA_FINF,  IdEstagio,               Periodo,    IdRestricaoOperativaUHE,         sim,          nao)  \
    m(  RHA_FSUP,  IdEstagio,               Periodo,    IdRestricaoOperativaUHE,         sim,          nao)  \
    m(  RHV,       IdEstagio,               Periodo,    IdRestricaoOperativaUHE,         sim,          nao)  \
    m(  RHV_FINF,  IdEstagio,               Periodo,    IdRestricaoOperativaUHE,         sim,          nao)  \
    m(  RHV_FSUP,  IdEstagio,               Periodo,    IdRestricaoOperativaUHE,         sim,          nao)
   // Nome,  TipoIterador1,         TipoIterador2,         TipoIterador3,   ImprimirPrimal, ImprimirDual

#define VARIAVEL_DECISAO_4(m)                                                                                             \
	m(  ZO_COMANDO,      IdEstagio,               Periodo,        IdPatamarCarga,      IdTermeletrica,             sim,          nao)  \
	m(  ZO_UTE_COMANDO,  IdEstagio,               Periodo,        IdPatamarCarga,      IdTermeletrica,             sim,          nao)  \
	m(  PH,          IdEstagio,               Periodo,        IdPatamarCarga,      IdHidreletrica,             sim,          nao)  \
	m(  PH_REL,      IdEstagio,               Periodo,        IdPatamarCarga,      IdHidreletrica,             sim,          nao)  \
	m(  PH_FINF,     IdEstagio,               Periodo,        IdPatamarCarga,      IdHidreletrica,             sim,          nao)  \
	m(  PHDISP,      IdEstagio,               Periodo,        IdPatamarCarga,      IdHidreletrica,             sim,          nao)  \
	m(  PHDISP_REL,  IdEstagio,               Periodo,        IdPatamarCarga,      IdHidreletrica,             sim,          nao)  \
	m(  PHDISP_FINF, IdEstagio,               Periodo,        IdPatamarCarga,      IdHidreletrica,             sim,          nao)  \
    m(  QBOM,        IdEstagio,               Periodo,        IdPatamarCarga,   IdUsinaElevatoria,             sim,          nao)  \
    m(  QBOM_FINF,   IdEstagio,               Periodo,        IdPatamarCarga,   IdUsinaElevatoria,             sim,          nao)  \
    m(  QBOMDISP,        IdEstagio,               Periodo,        IdPatamarCarga,   IdUsinaElevatoria,             sim,          nao)  \
    m(  QBOMDISP_FINF,   IdEstagio,               Periodo,        IdPatamarCarga,   IdUsinaElevatoria,             sim,          nao)  \
    m(  QTURDISP,    IdEstagio,               Periodo,        IdPatamarCarga,      IdHidreletrica,             sim,          nao)  \
    m(  QTURDISP_FINF,    IdEstagio,               Periodo,        IdPatamarCarga,      IdHidreletrica,             sim,          nao)  \
    m(  QTUR,        IdEstagio,               Periodo,        IdPatamarCarga,      IdHidreletrica,             sim,          nao)  \
    m(  QTUR_FINF,   IdEstagio,               Periodo,        IdPatamarCarga,      IdHidreletrica,             sim,          nao)  \
	m(  QVER,        IdEstagio,               Periodo,        IdPatamarCarga,      IdHidreletrica,             sim,          nao)  \
	m(  QMON,        IdEstagio,               Periodo,        IdPatamarCarga,      IdHidreletrica,             sim,          nao)  \
    m(  QDEF,        IdEstagio,               Periodo,        IdPatamarCarga,      IdHidreletrica,             sim,          nao)  \
    m(  QDEF_FINF,    IdEstagio,               Periodo,        IdPatamarCarga,      IdHidreletrica,             sim,          nao)  \
    m(  QDEF_FSUP,    IdEstagio,               Periodo,        IdPatamarCarga,      IdHidreletrica,             sim,          nao)  \
	m(  PL,          IdEstagio,               Periodo,        IdPatamarCarga,        IdSubmercado,             sim,          sim)  \
	m(  DE,			 IdEstagio,               Periodo,        IdPatamarCarga,			 IdDemandaEspecial,             sim,          sim)  \
	m(  CIMP,		 IdEstagio,               Periodo,        IdPatamarCarga,		            IdContrato,             sim,          sim)  \
	m(  CEXP,		 IdEstagio,               Periodo,        IdPatamarCarga,			        IdContrato,             sim,          sim)  \
	m(  PBOMDISP,    IdEstagio,               Periodo,        IdPatamarCarga,   IdUsinaElevatoria,             sim,          sim)  \
	m(  RE_FINF,     IdEstagio,               Periodo,        IdPatamarCarga, IdRestricaoEletrica,             sim,          nao)  \
    m(  RE_FSUP,     IdEstagio,               Periodo,        IdPatamarCarga, IdRestricaoEletrica,             sim,          nao)  \
	m(  PI,          IdEstagio,               Periodo,        IdPatamarCarga,       IdIntercambio,             sim,          sim)  \
    m(  PI_FINF,     IdEstagio,               Periodo,        IdPatamarCarga,       IdIntercambio,             sim,          nao)  \
	m(  PT,          IdEstagio,               Periodo,        IdPatamarCarga,      IdTermeletrica,             sim,          nao)  \
	m(  PE,			 IdEstagio,               Periodo,        IdPatamarCarga,		         IdUsinaEolica,             sim,          nao)  \
	m(  PT_UTIL,     IdEstagio,               Periodo,        IdPatamarCarga,      IdTermeletrica,             sim,          nao)  \
	m(  PT_MIN,      IdEstagio,               Periodo,        IdPatamarCarga,      IdTermeletrica,             sim,          nao)  \
	m(  LIG_T,       IdEstagio,               Periodo,        IdPatamarCarga,      IdTermeletrica,             sim,          nao)  \
	m(  DESL_T,      IdEstagio,               Periodo,        IdPatamarCarga,      IdTermeletrica,             sim,          nao)  \
	m(  ACIO_T,      IdEstagio,               Periodo,        IdPatamarCarga,      IdTermeletrica,             sim,          nao)  \
	m(  PT_REL,      IdEstagio,               Periodo,        IdPatamarCarga,      IdTermeletrica,             sim,          nao)  \
	m(  PT_FINF,     IdEstagio,               Periodo,        IdPatamarCarga,      IdTermeletrica,             sim,          nao)  \
	m(  PT_FSUP,     IdEstagio,               Periodo,        IdPatamarCarga,      IdTermeletrica,             sim,          nao)  \
	m(  PTDISP,      IdEstagio,               Periodo,        IdPatamarCarga,      IdTermeletrica,             sim,          nao)  \
	m(  PTDISP_REL,  IdEstagio,               Periodo,        IdPatamarCarga,      IdTermeletrica,             sim,          nao)  \
	m(  PTDISP_FINF, IdEstagio,               Periodo,        IdPatamarCarga,      IdTermeletrica,             sim,          nao)  \
	m(  PTDISP_FSUP, IdEstagio,               Periodo,        IdPatamarCarga,      IdTermeletrica,             sim,          nao)  \
	m(  PTDISPCOM,           IdEstagio,     IdTermeletrica,          int,        Periodo,              sim,          nao)  \
	m(  PTDISPCOM_ACUMULADO, IdEstagio,     IdTermeletrica,          int,        Periodo,              sim,          nao)  \
	m(  RP,          IdEstagio,               Periodo, IdProcessoEstocastico, IdVariavelAleatoria,             sim,          nao)  \
	m(  YP_REL,     IdEstagio,               Periodo, IdProcessoEstocastico, IdVariavelAleatoria,             sim,          nao)  \
	m(  YPF,        IdEstagio,               Periodo, IdProcessoEstocastico, IdVariavelAleatoria,             sim,          nao)  \
    m(  RHQ,         IdEstagio,               Periodo,        IdPatamarCarga, IdRestricaoOperativaUHE,         sim,          nao)  \
    m(  RHQ_FINF,    IdEstagio,               Periodo,        IdPatamarCarga, IdRestricaoOperativaUHE,         sim,          nao)  \
    m(  RHQ_FSUP,    IdEstagio,               Periodo,        IdPatamarCarga, IdRestricaoOperativaUHE,         sim,          nao)  \
	m(  QILS_TRI,        IdEstagio,               Periodo,        IdPatamarCarga, IdIntercambioHidraulico,         sim,          nao)  \
    m(  QILS_TRI_FINF,   IdEstagio,               Periodo,        IdPatamarCarga, IdIntercambioHidraulico,         sim,          nao)  \
    m(  QILS_TRI_REL,    IdEstagio,               Periodo,        IdPatamarCarga, IdIntercambioHidraulico,         sim,          nao)  \
    m(  QOUT,        IdEstagio,               Periodo,        IdPatamarCarga,          IdHidreletrica,             sim,          nao)  \
    m(  QOUT_FINF,   IdEstagio,               Periodo,        IdPatamarCarga,          IdHidreletrica,             sim,          nao)

   // Nome,  TipoIterador1,         TipoIterador2,         TipoIterador3,       TipoIterador4,  ImprimirPrimal, ImprimirDual

#define VARIAVEL_DECISAO_5(m)                                                                                                                             \
	m(  PH,          IdEstagio,               Periodo,        IdPatamarCarga,          IdHidreletrica,       IdConjuntoHidraulico,             sim,          nao)  \
	m(  PH_REL,      IdEstagio,               Periodo,        IdPatamarCarga,          IdHidreletrica,       IdConjuntoHidraulico,             sim,          nao)  \
	m(  PH_FINF,     IdEstagio,               Periodo,        IdPatamarCarga,          IdHidreletrica,       IdConjuntoHidraulico,             sim,          nao)  \
	m(  PHDISP,      IdEstagio,               Periodo,        IdPatamarCarga,          IdHidreletrica,       IdConjuntoHidraulico,             sim,          nao)  \
	m(  PHDISP_REL,  IdEstagio,               Periodo,        IdPatamarCarga,          IdHidreletrica,       IdConjuntoHidraulico,             sim,          nao)  \
	m(  PHDISP_FINF, IdEstagio,               Periodo,        IdPatamarCarga,          IdHidreletrica,       IdConjuntoHidraulico,             sim,          nao)  \
    m(  QTURDISP,    IdEstagio,               Periodo,        IdPatamarCarga,          IdHidreletrica,       IdConjuntoHidraulico,             sim,          nao)  \
    m(  QTUR,        IdEstagio,               Periodo,        IdPatamarCarga,          IdHidreletrica,       IdConjuntoHidraulico,             sim,          nao)  \
    m(  QTUR_FINF,   IdEstagio,               Periodo,        IdPatamarCarga,          IdHidreletrica,       IdConjuntoHidraulico,             sim,          nao)  \
	m(  PN,          IdEstagio,               Periodo,        IdPatamarCarga,            IdSubmercado,         IdUsinaNaoSimulada,             sim,          nao)  \
	m(  PT,          IdEstagio,               Periodo,        IdPatamarCarga,          IdTermeletrica,               IdUnidadeUTE,             sim,          nao)  \
	m(  PT_UTIL,     IdEstagio,               Periodo,        IdPatamarCarga,          IdTermeletrica,               IdUnidadeUTE,             sim,          nao)  \
	m(  PT_TRAJ_ACIO, IdEstagio,               Periodo,        IdPatamarCarga,          IdTermeletrica,               IdUnidadeUTE,             sim,          nao)  \
	m(  PT_TRAJ_DESL,IdEstagio,               Periodo,        IdPatamarCarga,          IdTermeletrica,               IdUnidadeUTE,             sim,          nao)  \
	m(  PT_MIN,      IdEstagio,               Periodo,        IdPatamarCarga,          IdTermeletrica,               IdUnidadeUTE,             sim,          nao)  \
	m(  LIG_T,       IdEstagio,               Periodo,        IdPatamarCarga,          IdTermeletrica,               IdUnidadeUTE,             sim,          nao)  \
	m(  DESL_T,      IdEstagio,               Periodo,        IdPatamarCarga,          IdTermeletrica,               IdUnidadeUTE,             sim,          nao)  \
	m(  ACIO_T,      IdEstagio,               Periodo,        IdPatamarCarga,          IdTermeletrica,               IdUnidadeUTE,             sim,          nao)  \
	m(  PT_REL,      IdEstagio,               Periodo,        IdPatamarCarga,          IdTermeletrica,               IdUnidadeUTE,             sim,          nao)  \
	m(  PT_FINF,     IdEstagio,               Periodo,        IdPatamarCarga,          IdTermeletrica,               IdUnidadeUTE,             sim,          nao)  \
	m(  PTDISP,      IdEstagio,               Periodo,        IdPatamarCarga,          IdTermeletrica,               IdUnidadeUTE,             sim,          nao)  \
	m(  YP,          IdEstagio,               Periodo, IdProcessoEstocastico,     IdVariavelAleatoria,                        int,             sim,          nao)  \
	m(  PD,          IdEstagio,               Periodo,        IdPatamarCarga,            IdSubmercado,           IdPatamarDeficit,             sim,          nao)  \
    m(  QOUT,        IdEstagio,               Periodo,        IdPatamarCarga, IdIntercambioHidraulico,             IdHidreletrica,             sim,          nao)  \
    m(  QOUT_FINF,   IdEstagio,               Periodo,        IdPatamarCarga, IdIntercambioHidraulico,             IdHidreletrica,             sim,          nao)  \
    m(  QOUT_REL,    IdEstagio,               Periodo,        IdPatamarCarga, IdIntercambioHidraulico,             IdHidreletrica,             sim,          nao)  \
    m(  QDES,        IdEstagio,               Periodo,        IdPatamarCarga,          IdHidreletrica,             IdHidreletrica,             sim,          nao)  \
    m(  QDES_FINF,   IdEstagio,               Periodo,        IdPatamarCarga,          IdHidreletrica,             IdHidreletrica,             sim,          nao)
   // Nome,      TipoIterador1,         TipoIterador2,         TipoIterador3,           TipoIterador4,              TipoIterador5,  ImprimirPrimal, ImprimirDual

#define VARIAVEL_DECISAO_6(m)                                                                                                                                               \
	m(  PH,          IdEstagio,               Periodo,        IdPatamarCarga,          IdHidreletrica,       IdConjuntoHidraulico,     IdUnidadeUHE,             sim,          nao)  \
	m(  PH_REL,      IdEstagio,               Periodo,        IdPatamarCarga,          IdHidreletrica,       IdConjuntoHidraulico,     IdUnidadeUHE,             sim,          nao)  \
	m(  PH_FINF,     IdEstagio,               Periodo,        IdPatamarCarga,          IdHidreletrica,       IdConjuntoHidraulico,     IdUnidadeUHE,             sim,          nao)  \
	m(  PH_FSUP,     IdEstagio,               Periodo,        IdPatamarCarga,          IdHidreletrica,       IdConjuntoHidraulico,     IdUnidadeUHE,             sim,          nao)  \
	m(  PHDISP,      IdEstagio,               Periodo,        IdPatamarCarga,          IdHidreletrica,       IdConjuntoHidraulico,     IdUnidadeUHE,             sim,          nao)  \
	m(  PHDISP_REL,  IdEstagio,               Periodo,        IdPatamarCarga,          IdHidreletrica,       IdConjuntoHidraulico,     IdUnidadeUHE,             sim,          nao)  \
	m(  PHDISP_FINF, IdEstagio,               Periodo,        IdPatamarCarga,          IdHidreletrica,       IdConjuntoHidraulico,     IdUnidadeUHE,             sim,          nao)  \
    m(  QTURDISP,    IdEstagio,               Periodo,        IdPatamarCarga,          IdHidreletrica,       IdConjuntoHidraulico,     IdUnidadeUHE,             sim,          nao)  \
    m(  QTUR,        IdEstagio,               Periodo,        IdPatamarCarga,          IdHidreletrica,       IdConjuntoHidraulico,     IdUnidadeUHE,             sim,          nao)  \
    m(  QTUR_FINF,   IdEstagio,               Periodo,        IdPatamarCarga,          IdHidreletrica,       IdConjuntoHidraulico,     IdUnidadeUHE,             sim,          nao)  \
    m(  QDES,        IdEstagio,               Periodo,        IdPatamarCarga, IdIntercambioHidraulico,             IdHidreletrica,   IdHidreletrica,             sim,          nao)
   // Nome,    TipoIterador1,         TipoIterador2,         TipoIterador3,           TipoIterador4,              TipoIterador5,    TipoIterador6,  ImprimirPrimal, ImprimirDual


#define RESTRICAO_EQUACAO_LINEAR_2(m)                                                            \
	m(  CUSTO_PENALIDADE_0,                  IdEstagio,        Periodo,                    nao)  \
	m(  CUSTO_PENALIDADE_0p,                 IdEstagio,        Periodo,                    nao)  \
	m(  CUSTO_PENALIDADE_p,                  IdEstagio,        Periodo,                    nao)  \
	m(  CUSTO_OPERACAO_0,                    IdEstagio,        Periodo,                    nao)  \
	m(  CUSTO_OPERACAO_0p,                   IdEstagio,        Periodo,                    nao)  \
	m(  CUSTO_OPERACAO_p,                    IdEstagio,        Periodo,                    nao)  \
	m(  CUSTO_OPERACAO_UTE_COMANDO_0,        IdEstagio,        Periodo,                    nao)  \
	m(  CUSTO_TOTAL,                         IdEstagio,        Periodo,                    nao)  \
	m(  CUSTO_TOTAL_MESTRE,                  IdEstagio,        Periodo,                    nao)  \
	m(  CUSTO_IMEDIATO,                      IdEstagio,        Periodo,                    nao)  \
	m(  CUSTO_PENALIDADE_VOLUME_0,           IdEstagio,        Periodo,                    nao)  \
	m(  CUSTO_PENALIDADE_VOLUME_0_ACUMULANDO,IdEstagio,        Periodo,                    nao)  \
	m(  CUSTO_PENALIDADE_VOLUME_p,           IdEstagio,        Periodo,                    nao)  \
	m(  CUSTO_PENALIDADE_VOLUME_0p,          IdEstagio,        Periodo,                    nao)  \
	m(  CUSTO_PENALIDADE,                    IdEstagio,        Periodo,                    nao)  \
	m(  CUSTO_FUTURO,                        IdEstagio,        Periodo,                    nao)  
   // Nome,                              TipoIterador1,  TipoIterador2,  ImprimirMultiplicador
 
#define RESTRICAO_EQUACAO_LINEAR_3(m)                                                                                      \
	m(  CUSTO_PENALIDADE,                     IdEstagio,        Periodo,         IdPatamarCarga,                      nao)  \
	m(  CUSTO_OPERACAO,                       IdEstagio,        Periodo,         IdPatamarCarga,                      nao)  \
	m(  CUSTO_OPERACAO_UTE_COMANDO_0,         IdEstagio,        Periodo,         IdTermeletrica,                      nao)  \
	m(  CUSTO_OPERACAO_UTE_COMANDO_p,         IdEstagio,        Periodo,         IdTermeletrica,                      nao)  \
	m(  CUSTO_OPERACAO_UTE_COMANDO_0p,        IdEstagio,        Periodo,         IdTermeletrica,                      nao)  \
	m(  VOLUME_MEDIO,                         IdEstagio,        Periodo,         IdHidreletrica,                      nao)  \
	m(  AFLUENCIA_INCREMENTAL,                IdEstagio,        Periodo,         IdHidreletrica,                      nao)  \
	m(  DEMANDA,                              IdEstagio,        Periodo,           IdSubmercado,                      nao)  \
	m(  DEFICIT,                              IdEstagio,        Periodo,           IdSubmercado,                      nao)  \
	m(  INTERCAMBIO,                          IdEstagio,        Periodo,           IdSubmercado,                      nao)  \
	m(  POTENCIA_BOMBEAMENTO_DISPONIVEL,      IdEstagio,        Periodo,      IdUsinaElevatoria,                      nao)  \
	m(  POTENCIA_HIDRAULICA_DISPONIVEL,       IdEstagio,        Periodo,         IdHidreletrica,                      nao)  \
	m(  POTENCIA_TERMICA_DISPONIVEL,          IdEstagio,        Periodo,         IdTermeletrica,                      nao)  \
	m(  POTENCIA_TERMICA_DISPONIVEL_COMANDADA,          IdEstagio,        Periodo,         IdTermeletrica,                      nao)  \
	m(  POTENCIA_TERMICA_DISPONIVEL_COMANDADA_ACUMULADA,IdEstagio,        Periodo,         IdTermeletrica,                      nao)  \
	m(  PRODUCAO_USINA_NAO_SIMULADA,          IdEstagio,        Periodo,           IdSubmercado,                      nao)  \
	m(  VAZAO_EVAPORADA,                      IdEstagio,        Periodo,           IdHidreletrica,                    nao)  \
	m(  VAZAO_EVAPORADA_RELAXADA,             IdEstagio,        Periodo,           IdHidreletrica,                    nao)  \
	m(  VAZAO_EVAPORADA_DISPONIVEL,           IdEstagio,        Periodo,           IdHidreletrica,                    nao)  \
	m(  VAZAO_TURBINADA_DISPONIVEL,           IdEstagio,        Periodo,         IdHidreletrica,                      nao)  \
	m(  VAZAO_DEFLUENTE,                      IdEstagio,        Periodo,         IdHidreletrica,                      nao)  \
	m(  VAZAO_VERTIDA,                        IdEstagio,        Periodo,         IdHidreletrica,                      nao)  \
    m(  DEFLUENCIA_VIAJANTE_BALANCO_HIDRICO,  IdEstagio,        Periodo,           IdHidreletrica,                    nao)  \
    m(  DEFLUENCIA_VIAJANTE_ACOPLAMENTO,      IdEstagio,        IdHidreletrica,           Periodo,                    nao)  \
    m(  POTENCIA_DISPONIVEL_META,             IdEstagio,        Periodo,           IdHidreletrica,                    nao)  \
    m(  AFLUENCIA_PROCESSO_ESTOCASTICO_ACUMULADA,  IdEstagio,   Periodo,           IdVariavelAleatoria,               nao)

   // Nome,                              TipoIterador1,  TipoIterador2,          TipoIterador3,    ImprimirMultiplicador

#define RESTRICAO_EQUACAO_LINEAR_4(m)                                                                                                                        \
	m(  AFLUENCIA_PROCESSO_ESTOCASTICO,           IdEstagio,        Periodo,         IdProcessoEstocastico,    IdVariavelAleatoria,                    nao)  \
	m(  ATENDIMENTO_DEMANDA,                      IdEstagio,        Periodo,                IdPatamarCarga,           IdSubmercado,                    nao)  \
	m(  BALANCO_HIDRAULICO_VOL,                   IdEstagio,        Periodo,                IdPatamarCarga,         IdHidreletrica,                    nao)   \
	m(  CUSTO_OPERACAO_UTE_COMANDO,               IdEstagio,        Periodo,                IdPatamarCarga,         IdTermeletrica,                    nao)  \
	m(  BALANCO_HIDRAULICO_VAZ,                   IdEstagio,        Periodo,                IdPatamarCarga,         IdHidreletrica,                    nao)  \
	m(  VAZAO_MONTANTE,                           IdEstagio,        Periodo,                IdPatamarCarga,         IdHidreletrica,                    nao)  \
    m(  VAZAO_DEFLUENTE,                          IdEstagio,        Periodo,                IdPatamarCarga,         IdHidreletrica,                    nao)  \
	m(  VAZAO_TURBINADA,                          IdEstagio,        Periodo,                IdPatamarCarga,         IdHidreletrica,                    nao)  \
	m(  VAZAO_TURBINADA_DISPONIVEL,               IdEstagio,        Periodo,                IdPatamarCarga,         IdHidreletrica,                    nao)  \
	m(  POTENCIA_TERMICA_RELAXADA,                IdEstagio,        Periodo,                IdPatamarCarga,         IdTermeletrica,                    nao)  \
	m(  POTENCIA_TERMICA_DISPONIVEL,              IdEstagio,        Periodo,                IdPatamarCarga,         IdTermeletrica,                    nao)  \
	m(  POTENCIA_TERMICA,                         IdEstagio,        Periodo,                IdPatamarCarga,         IdTermeletrica,                    nao)  \
	m(  RELACAO_VARIAVEIS_BINARIAS_TERMICA,       IdEstagio,        Periodo,                IdPatamarCarga,         IdTermeletrica,                    nao)  \
    m(  POTENCIA_TERMICA_DISPONIVEL_RELAXADA,     IdEstagio,        Periodo,                IdPatamarCarga,         IdTermeletrica,                    nao)  \
	m(  POTENCIA_BOMBEAMENTO_DISPONIVEL,          IdEstagio,        Periodo,                IdPatamarCarga,      IdUsinaElevatoria,                    nao)  \
	m(  POTENCIA_HIDRAULICA,                      IdEstagio,        Periodo,                IdPatamarCarga,         IdHidreletrica,                    nao)  \
	m(  POTENCIA_HIDRAULICA_RELAXADA,             IdEstagio,        Periodo,                IdPatamarCarga,         IdHidreletrica,                    nao)  \
	m(  POTENCIA_HIDRAULICA_DISPONIVEL,           IdEstagio,        Periodo,                IdPatamarCarga,         IdHidreletrica,                    nao)  \
	m(  POTENCIA_HIDRAULICA_DISPONIVEL_RELAXADA,  IdEstagio,        Periodo,                IdPatamarCarga,         IdHidreletrica,                    nao)  \
    m(  INTERCAMBIO_HIDRAULICO_RETIRADA_RELAXADA, IdEstagio,        Periodo,                IdPatamarCarga,   IdIntercambioHidraulico,                 nao)  \
    m(  VAZAO_RETIRADA_FOLGA_INFERIOR,            IdEstagio,        Periodo,                IdPatamarCarga,         IdHidreletrica,                    nao)  \
    m(  VAZAO_BOMBEADA_DISPONIVEL,                IdEstagio,        Periodo,                IdPatamarCarga,      IdUsinaElevatoria,                    nao)

   // Nome,                                   TipoIterador1,  TipoIterador2,                 TipoIterador3,          TipoIterador4,   ImprimirMultiplicador

#define RESTRICAO_EQUACAO_LINEAR_5(m)                                                                                                                                       \
	m(  AFLUENCIA_PROCESSO_ESTOCASTICO,              IdEstagio,        Periodo,  IdProcessoEstocastico,  IdVariavelAleatoria,              int,                   nao)      \
	m(  VAZAO_TURBINADA,                             IdEstagio,        Periodo,      IdPatamarCarga,     IdHidreletrica,  IdConjuntoHidraulico,                   nao)      \
	m(  VAZAO_TURBINADA_RELAXADA,                    IdEstagio,        Periodo,      IdPatamarCarga,     IdHidreletrica,  IdConjuntoHidraulico,                   nao)      \
	m(  VAZAO_TURBINADA_DISPONIVEL,                  IdEstagio,        Periodo,      IdPatamarCarga,     IdHidreletrica,  IdConjuntoHidraulico,                   nao)      \
	m(  POTENCIA_TERMICA,                            IdEstagio,        Periodo,      IdPatamarCarga,     IdTermeletrica,          IdUnidadeUTE,                   nao)      \
	m(  POTENCIA_TERMICA_RELAXADA,                   IdEstagio,        Periodo,      IdPatamarCarga,     IdTermeletrica,          IdUnidadeUTE,                   nao)      \
	m(  POTENCIA_TERMICA_DISPONIVEL,                 IdEstagio,        Periodo,      IdPatamarCarga,     IdTermeletrica,          IdUnidadeUTE,                   nao)      \
    m(  POTENCIA_TERMICA_DISPONIVEL_RELAXADA,        IdEstagio,        Periodo,      IdPatamarCarga,     IdTermeletrica,          IdUnidadeUTE,                   nao)      \
	m(  RELACAO_VARIAVEIS_BINARIAS_TERMICA,          IdEstagio,        Periodo,      IdPatamarCarga,     IdTermeletrica,          IdUnidadeUTE,                   nao)      \
	m(  POTENCIA_HIDRAULICA,                         IdEstagio,        Periodo,      IdPatamarCarga,     IdHidreletrica,  IdConjuntoHidraulico,                   nao)      \
	m(  POTENCIA_HIDRAULICA_RELAXADA,                IdEstagio,        Periodo,      IdPatamarCarga,     IdHidreletrica,  IdConjuntoHidraulico,                   nao)      \
	m(  POTENCIA_HIDRAULICA_DISPONIVEL,              IdEstagio,        Periodo,      IdPatamarCarga,     IdHidreletrica,  IdConjuntoHidraulico,                   nao)      \
	m(  POTENCIA_HIDRAULICA_DISPONIVEL_RELAXADA,     IdEstagio,        Periodo,      IdPatamarCarga,     IdHidreletrica,  IdConjuntoHidraulico,                   nao)      
   // Nome,                                      TipoIterador1,  TipoIterador2,       TipoIterador3,      TipoIterador4,         TipoIterador5,  ImprimirMultiplicador

#define RESTRICAO_EQUACAO_LINEAR_6(m)                                                                                                                                                       \
	m(  VAZAO_TURBINADA_RELAXADA,                    IdEstagio,        Periodo,      IdPatamarCarga,     IdHidreletrica,  IdConjuntoHidraulico,   IdUnidadeUHE,                   nao)      \
	m(  VAZAO_TURBINADA_DISPONIVEL,                  IdEstagio,        Periodo,      IdPatamarCarga,     IdHidreletrica,  IdConjuntoHidraulico,   IdUnidadeUHE,                   nao)      \
	m(  POTENCIA_HIDRAULICA,                         IdEstagio,        Periodo,      IdPatamarCarga,     IdHidreletrica,  IdConjuntoHidraulico,   IdUnidadeUHE,                   nao)      \
	m(  POTENCIA_HIDRAULICA_RELAXADA,                IdEstagio,        Periodo,      IdPatamarCarga,     IdHidreletrica,  IdConjuntoHidraulico,   IdUnidadeUHE,                   nao)      \
	m(  POTENCIA_HIDRAULICA_DISPONIVEL,              IdEstagio,        Periodo,      IdPatamarCarga,     IdHidreletrica,  IdConjuntoHidraulico,   IdUnidadeUHE,                   nao)      \
	m(  POTENCIA_HIDRAULICA_DISPONIVEL_RELAXADA,     IdEstagio,        Periodo,      IdPatamarCarga,     IdHidreletrica,  IdConjuntoHidraulico,   IdUnidadeUHE,                   nao)      
   // Nome,                                      TipoIterador1,  TipoIterador2,       TipoIterador3,      TipoIterador4,         TipoIterador5,  TipoIterador6,  ImprimirMultiplicador



#define RESTRICAO_INEQUACAO_LINEAR_3(m)                                                                                                      \
    m(  RESTRICAO_HIDRAULICA_VOLUME,                    IdEstagio,        Periodo,         IdRestricaoOperativaUHE,                    nao)  \
    m(  RESTRICAO_HIDRAULICA_VOLUME_LIMITE_INFERIOR,    IdEstagio,        Periodo,         IdRestricaoOperativaUHE,                    nao)  \
    m(  RESTRICAO_HIDRAULICA_VOLUME_LIMITE_SUPERIOR,    IdEstagio,        Periodo,         IdRestricaoOperativaUHE,                    nao)  \
    m(  RESTRICAO_HIDRAULICA_AFLUENTE_LIMITE_INFERIOR,  IdEstagio,        Periodo,         IdRestricaoOperativaUHE,                    nao)  \
    m(  RESTRICAO_HIDRAULICA_AFLUENTE_LIMITE_SUPERIOR,  IdEstagio,        Periodo,         IdRestricaoOperativaUHE,                    nao)  \
    m(  RESTRICAO_HIDRAULICA_AFLUENTE,                  IdEstagio,        Periodo,         IdRestricaoOperativaUHE,                    nao)  \
	m(  VAZAO_DEFLUENTE_FOLGA_INFERIOR,                 IdEstagio,        Periodo,                  IdHidreletrica,                    nao)  \
    m(  VAZAO_DEFLUENTE_FOLGA_SUPERIOR,                 IdEstagio,        Periodo,                  IdHidreletrica,                    nao)  \
    m(  POTENCIA_TERMICA_COMANDADA_LIMITE_INFERIOR,     IdEstagio,        Periodo,                  IdTermeletrica,                    nao)  \
    m(  POTENCIA_TERMICA_COMANDADA_LIMITE_SUPERIOR,     IdEstagio,        Periodo,                  IdTermeletrica,                    nao)  \
    m(  VOLUME_UTIL_FOLGA_INFERIOR,                     IdEstagio,        Periodo,                  IdHidreletrica,                    nao)  
      // Nome,                       TipoIterador1,  TipoIterador2,          TipoIterador3,           ImprimirMultiplicador

#define RESTRICAO_INEQUACAO_LINEAR_4(m)                                                                                                                                    \
	m(  CB_ZF,                                               IdEstagio,        Periodo,           IdRealizacao,                   IdCorteBenders,                    sim)  \
	m(  CB_ZT,                                               IdEstagio,        Periodo,           IdRealizacao,                   IdCorteBenders,                    sim)  \
	m(  RESTRICAO_ELETRICA_LIMITE_INFERIOR,                  IdEstagio,        Periodo,         IdPatamarCarga,              IdRestricaoEletrica,                    nao)  \
    m(  RESTRICAO_ELETRICA_LIMITE_SUPERIOR,                  IdEstagio,        Periodo,         IdPatamarCarga,              IdRestricaoEletrica,                    nao)  \
	m(  AGRUPAMENTO_INTERCAMBIO,                             IdEstagio,        Periodo,         IdPatamarCarga,         IdAgrupamentoIntercambio,                    nao)  \
    m(  VOLUME_MINIMO_PENALIZADO,                            IdEstagio,        Periodo,         IdPatamarCarga,                   IdHidreletrica,                    nao)  \
	m(  META_ENCHENDO_VOLUME_MORTO,                          IdEstagio,        Periodo,         IdPatamarCarga,                   IdHidreletrica,                    nao)  \
	m(  VAZAO_DEFLUENTE_FOLGA_INFERIOR,                      IdEstagio,        Periodo,         IdPatamarCarga,                   IdHidreletrica,                    nao)  \
	m(  VAZAO_DEFLUENTE_FOLGA_SUPERIOR,                      IdEstagio,        Periodo,         IdPatamarCarga,                   IdHidreletrica,                    nao)  \
	m(  RELACAO_UNIDADES_EQUIVALENTE_TERMICA,                IdEstagio,        Periodo,         IdPatamarCarga,                   IdTermeletrica,                    nao)  \
    m(  RESTRICAO_HIDRAULICA_DEFLUENCIA_LIMITE_INFERIOR,     IdEstagio,        Periodo,         IdPatamarCarga,          IdRestricaoOperativaUHE,                    nao)  \
    m(  RESTRICAO_HIDRAULICA_DEFLUENCIA_LIMITE_SUPERIOR,     IdEstagio,        Periodo,         IdPatamarCarga,          IdRestricaoOperativaUHE,                    nao)  \
	m(  POTENCIA_TERMELETRICA_UTIL,                          IdEstagio,        Periodo,         IdPatamarCarga,                   IdTermeletrica,                    nao)  \
	m(  POTENCIA_TERMELETRICA_PARTIDA,                       IdEstagio,        Periodo,         IdPatamarCarga,                   IdTermeletrica,                    nao)  \
	m(  POTENCIA_TERMELETRICA_DESLIGAMENTO,                  IdEstagio,        Periodo,         IdPatamarCarga,                   IdTermeletrica,                    nao)  \
	m(  POTENCIA_TERMICA_DISPONIVEL_FINF,                    IdEstagio,        Periodo,         IdPatamarCarga,                   IdTermeletrica,                    nao)  \
	m(  POTENCIA_TERMICA_DISPONIVEL_FSUP,                    IdEstagio,        Periodo,         IdPatamarCarga,                   IdTermeletrica,                    nao)  \
    m(  RESTRICAO_HIDRAULICA_DEFLUENCIA,                     IdEstagio,        Periodo,         IdPatamarCarga,          IdRestricaoOperativaUHE,                    nao)  \
    m(  VAZAO_BOMBEADA_FOLGA_INFERIOR,                       IdEstagio,        Periodo,         IdPatamarCarga,                IdUsinaElevatoria,                    nao)  \
    m(  VAZAO_BOMBEADA_DISPONIVEL_FOLGA_INFERIOR,            IdEstagio,        Periodo,         IdPatamarCarga,                IdUsinaElevatoria,                    nao)  \
    m(  VAZAO_TURBINADA_DISPONIVEL_RELAXADA,                 IdEstagio,        Periodo,         IdPatamarCarga,                   IdHidreletrica,                    nao)  \
    m(  VAZAO_TURBINADA_RELAXADA,                            IdEstagio,        Periodo,         IdPatamarCarga,                   IdHidreletrica,                    nao)  \
    m(  INTERCAMBIO_RELAXADO,                                IdEstagio,        Periodo,         IdPatamarCarga,                    IdIntercambio,                    nao)
	   // Nome,                       TipoIterador1,  TipoIterador2,          TipoIterador3,                    TipoIterador4,   ImprimirMultiplicador

#define RESTRICAO_INEQUACAO_LINEAR_5(m)                                                                                                                                             \
	m(  POTENCIA_HIDRAULICA,                IdEstagio,        Periodo,         IdPatamarCarga,         IdHidreletrica,                              int,                      sim)  \
	m(  POTENCIA_TERMELETRICA_UTIL,         IdEstagio,        Periodo,         IdPatamarCarga,         IdTermeletrica,                     IdUnidadeUTE,                      nao)  \
	m(  POTENCIA_TERMELETRICA_PARTIDA,      IdEstagio,        Periodo,         IdPatamarCarga,         IdTermeletrica,                     IdUnidadeUTE,                      nao)  \
	m(  POTENCIA_TERMELETRICA_DESLIGAMENTO, IdEstagio,        Periodo,         IdPatamarCarga,         IdTermeletrica,                     IdUnidadeUTE,                      nao)  \
	m(  TEMPO_MINIMO_LIGADA,                IdEstagio,        Periodo,         IdPatamarCarga,         IdTermeletrica,                     IdUnidadeUTE,                      nao)  \
	m(  TEMPO_MINIMO_DESLIGADA,             IdEstagio,        Periodo,         IdPatamarCarga,         IdTermeletrica,                     IdUnidadeUTE,                      nao)  \
    m(  VAZAO_DESVIADA_FOLGA_INFERIOR,      IdEstagio,        Periodo,         IdPatamarCarga,         IdHidreletrica,                     IdHidreletrica,                    nao)  
   // Nome,                            TipoIterador1,  TipoIterador2,          TipoIterador3,          TipoIterador4,                    TipoIterador5,     ImprimirMultiplicador

#define RESTRICAO_INEQUACAO_LINEAR_6(m)                                                                                                                                                                 \
	m(  PT_TRAJETORIA_ACIONAMENTO_INF,          IdEstagio,        Periodo,         IdPatamarCarga,         IdTermeletrica,         IdUnidadeUTE,                        Periodo,                    nao)  \
	m(  PT_TRAJETORIA_ACIONAMENTO_SUP,          IdEstagio,        Periodo,         IdPatamarCarga,         IdTermeletrica,         IdUnidadeUTE,                        Periodo,                    nao)  \
	m(  PT_TRAJETORIA_ACIONAMENTO,              IdEstagio,        Periodo,         IdPatamarCarga,         IdTermeletrica,         IdUnidadeUTE,                        Periodo,                    nao)  \
	m(  POTENCIA_HIDRAULICA,                    IdEstagio,        Periodo,         IdPatamarCarga,         IdHidreletrica, IdConjuntoHidraulico,                            int,                    nao)  
   // Nome,                            TipoIterador1,  TipoIterador2,          TipoIterador3,          TipoIterador4,         TipoIterador5,                   TipoIterador6,   ImprimirMultiplicador

#define RESTRICAO_INEQUACAO_LINEAR_7(m)  \
	m(  PT_TRAJETORIA_DESLIGAMENTO_INF,    IdEstagio,        Periodo,         IdPatamarCarga,         IdTermeletrica,         IdUnidadeUTE,		        Periodo,                  int,  nao)  \
	m(  PT_TRAJETORIA_DESLIGAMENTO_SUP,    IdEstagio,        Periodo,         IdPatamarCarga,         IdTermeletrica,         IdUnidadeUTE,				Periodo,                  int,  nao)  \
	m(  PT_TRAJETORIA_DESLIGAMENTO,        IdEstagio,        Periodo,         IdPatamarCarga,         IdTermeletrica,         IdUnidadeUTE,			    Periodo,                  int,  nao)  \
	m(  POTENCIA_HIDRAULICA,               IdEstagio,        Periodo,         IdPatamarCarga,         IdHidreletrica,  IdConjuntoHidraulico,       IdUnidadeUHE,                  int,  nao)  
   // Nome,                            TipoIterador1,  TipoIterador2,          TipoIterador3,          TipoIterador4,         TipoIterador5,      TipoIterador6,                      TipoIterador6,   ImprimirMultiplicador


	class ModeloOtimizacao : public SmartDados {

	public:

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

		void addCorteBendersToZF(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdEstagio a_idEstagio_anterior, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders, const double a_rhs, const SmartEnupla<IdVariavelEstado, double>& a_coeficiente);
		void setCorteBendersToZF(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdEstagio a_idEstagio_anterior, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders, const double a_rhs, const SmartEnupla<IdVariavelEstado, double>& a_coeficiente);
		void remCorteBendersFromZF(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdEstagio a_idEstagio_anterior, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders);

		void addCorteBendersToZT(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders, const double a_rhs, const SmartEnupla<IdVariavelEstado, double>& a_coeficiente);
		void setCorteBendersToZT(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders, const double a_rhs, const SmartEnupla<IdVariavelEstado, double>& a_coeficiente);
		void remCorteBendersFromZT(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders);

		bool isNecessarioInstanciarSolver(const IdEstagio a_idEstagio, const TipoSubproblemaSolver a_tipoSubproblemaSolver);

		void imprimirEstagios(EntradaSaidaDados a_entradaSaidaDados);

		void imprimirSolver(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const std::string a_nome_arquivo);

		void imprimirVariaveisEstado(EntradaSaidaDados a_entradaSaidaDados);

		void imprimirVariaveisRealizacao(EntradaSaidaDados a_entradaSaidaDados);

		void imprimirVariaveisRealizacaoInterna(EntradaSaidaDados a_entradaSaidaDados);

		void exportarCorteBenders(const IdProcesso a_idProcesso, const IdEstagio a_estagio_inicial, const IdEstagio a_estagio_final, EntradaSaidaDados a_entradaSaidaDados);

		void exportarCorteBenders(const IdProcesso a_idProcesso, const IdEstagio a_estagio, EntradaSaidaDados a_entradaSaidaDados);
		void exportarVersaoAlternativaCorteBenders(const IdProcesso a_idProcesso, const IdEstagio a_estagio, EntradaSaidaDados a_entradaSaidaDados);

		void removerCorteBenders(const IdEstagio a_estagio_inicial, const IdEstagio a_estagio_final);
		void removerCorteBenders(const IdEstagio a_estagio);

		void exportarCorteBenders_AcoplamentoPreEstudo(const IdIteracao a_idIteracao, EntradaSaidaDados a_entradaSaidaDados);

		void importarCorteBenders(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdProcesso a_idProcesso, const std::string a_diretorio_impressao_selecao_cortes, EntradaSaidaDados a_entradaSaidaDados, const SmartEnupla <Periodo, bool> a_horizonte_tendencia_mais_processo_estocastico);
		void importarCorteBenders_AcoplamentoPosEstudo(const TipoSubproblemaSolver a_TSS, const IdProcesso a_idProcesso, const IdIteracao a_idIteracao, const std::string a_diretorio_impressao_selecao_cortes, EntradaSaidaDados a_entradaSaidaDados);

		void importarVariaveisEstado_AcoplamentoPosEstudo(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdProcesso a_idProcesso, EntradaSaidaDados a_entradaSaidaDados, const SmartEnupla <Periodo, bool> a_horizonte_defluencia_passada, const SmartEnupla <Periodo, bool> a_horizonte_tendencia_mais_processo_estocastico);

		void importarVariaveisEstado_AcoplamentoPreEstudo(const IdProcesso a_idProcesso, const IdProcesso a_maior_processo, const IdIteracao a_idIteracao, const IdCenario a_cenario_inicial, const IdCenario a_cenario_final, EntradaSaidaDados a_entradaSaidaDados);

		void exportarVariaveisEstado_AcoplamentoPreEstudo(EntradaSaidaDados a_entradaSaidaDados);

		void exportarVariaveisEstado_AcoplamentoPosEstudo(const IdProcesso a_idProcesso, const IdProcesso a_maior_processo, const IdIteracao a_idIteracao, EntradaSaidaDados a_entradaSaidaDados);

		bool isVariavelEstadoInstanciada(const IdEstagio a_idEstagio, const IdVariavelEstado a_idVariavelEstado);

		void esvaziarVetorCorteBenders();
		void esvaziarVetorCorteBenders(const IdEstagio a_idEstagio);

		SmartEnupla<IdProcessoEstocastico, SmartEnupla<IdProcesso, SmartEnupla<IdVariavelAleatoria, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>>>> realizacoes;

		void gerarRealizacoes(const IdProcesso a_idProcesso, const IdProcesso a_maior_processo, const IdCenario a_cenario_inicial, const IdCenario a_cenario_final, EntradaSaidaDados a_entradaSaidaDados);

		void atualizarModeloOtimizacaoComVariavelEstado(const IdEstagio a_idEstagio, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdCenario a_idCenario);
		void atualizarModeloOtimizacaoComVariavelEstado(const IdEstagio a_idEstagio, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdCenario a_idCenario, EstruturaResultados<double> &a_retorno_estados);

		void atualizarModeloOtimizacaoComVariavelRealizacao(const IdEstagio a_idEstagio, const IdCenario    a_idCenario);
		void atualizarModeloOtimizacaoComVariavelRealizacao(const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao);

		bool atualizarModeloOtimizacaoComVariavelRealizacaoInterna(const TipoSubproblemaSolver a_TSS, const TipoSubproblemaSolver a_TSS_origem, const IdProcesso a_idProcesso, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const std::string a_diretorio);

		bool atualizarModeloOtimizacaoComVariavelRealizacaoInterna(const TipoSubproblemaSolver a_TSS, const IdProcesso a_idProcesso, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao);

		void resetarVariavelRealizacaoInterna(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio);

		void atualizarVariavelEstadoComSolucaoPrimal(const bool a_resetar, const IdIteracao a_idIteracao, IdProcesso a_idProcesso, const IdProcesso a_maior_processo, const IdEstagio a_idEstagio, const IdCenario a_idCenario);

		void addValorVariavelEstado(const IdEstagio a_idEstagio, const IdVariavelEstado idVariavelEstado, const IdProcesso a_idProcesso, const IdProcesso a_maior_processo, const IdCenario a_idCenario, const double valor);

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

		void calcularCustoPrimalViaSubproblemaMestre(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const std::string a_diretorio, double& a_custo_geral, double& a_custo_individual);

		double getProbabilidadeAbertura(const IdEstagio a_idEstagio, const IdCenario a_idCenario);

		double getProbabilidadeAbertura(const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao);

		void getSolucaoDualVariavelEstado(const IdEstagio a_idEstagio, int &a_idx_sol_dual_var_estado, double* a_sol_dual_var_estado);

		double* getReferenciaValoresEstado(const IdEstagio a_idEstagio, const IdVariavelEstado a_idVariavelEstado, const IdProcesso a_idProcesso, const IdCenario a_idCenario_inicial, const IdCenario a_idCenario_final);

		double getCustoTotal(const IdEstagio a_idEstagio);
		double getCustoImediato(const IdEstagio a_idEstagio);
		double getCustoFuturo(const IdEstagio a_idEstagio);

		void consolidarResultados(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);
		void consolidarVariaveis(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);
		void consolidarEquacoes(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);
		void consolidarInequacoes(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);

		int getMenorNumeroAberturasEstagio(const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, const IdIteracao a_idIteracao);
		int getMaiorNumeroAberturasEstagio(const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, const IdIteracao a_idIteracao);

		int getMenorNumeroAberturasProcessoEstagio(const IdProcesso a_idProcesso, const IdEstagio a_idEstagio, const IdIteracao a_idIteracao);
		int getMaiorNumeroAberturasProcessoEstagio(const IdProcesso a_idProcesso, const IdEstagio a_idEstagio, const IdIteracao a_idIteracao);

		int getNumeroTotalCenariosEmEstados(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, const IdIteracao a_idIteracao);
		int getNumeroVariavelDinamica(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio);

		IdCenario getCenarioInicial(const IdProcesso a_idProcesso, const IdIteracao a_idIteracao);
		IdCenario getCenarioFinal(const IdProcesso a_idProcesso, const IdIteracao a_idIteracao);
		int getNumeroCenarios(const IdCenario a_cenario_inicial, const IdCenario a_cenario_final);

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

		std::vector<std::vector<std::string>> lista_variaveis_instanciadas;
		std::vector<std::vector<std::string>> lista_equacoes_instanciadas;
		std::vector<std::vector<std::string>> lista_inequacoes_instanciadas;

		IdCorteBenders maiorIdCorteExportadoPreEstudo = IdCorteBenders_Nenhum;

		VARIAVEL_DECISAO_2(DECLARAR_VARIAVEL_DECISAO_2)
			VARIAVEL_DECISAO_3(DECLARAR_VARIAVEL_DECISAO_3)
			VARIAVEL_DECISAO_4(DECLARAR_VARIAVEL_DECISAO_4)
			VARIAVEL_DECISAO_5(DECLARAR_VARIAVEL_DECISAO_5)
			VARIAVEL_DECISAO_6(DECLARAR_VARIAVEL_DECISAO_6)


			RESTRICAO_EQUACAO_LINEAR_2(DECLARAR_RESTRICAO_EQUACAO_LINEAR_2)
			RESTRICAO_EQUACAO_LINEAR_3(DECLARAR_RESTRICAO_EQUACAO_LINEAR_3)
			RESTRICAO_EQUACAO_LINEAR_4(DECLARAR_RESTRICAO_EQUACAO_LINEAR_4)
			RESTRICAO_EQUACAO_LINEAR_5(DECLARAR_RESTRICAO_EQUACAO_LINEAR_5)
			RESTRICAO_EQUACAO_LINEAR_6(DECLARAR_RESTRICAO_EQUACAO_LINEAR_6)


			RESTRICAO_INEQUACAO_LINEAR_3(DECLARAR_RESTRICAO_INEQUACAO_LINEAR_3)
			RESTRICAO_INEQUACAO_LINEAR_4(DECLARAR_RESTRICAO_INEQUACAO_LINEAR_4)
			RESTRICAO_INEQUACAO_LINEAR_5(DECLARAR_RESTRICAO_INEQUACAO_LINEAR_5)
			RESTRICAO_INEQUACAO_LINEAR_6(DECLARAR_RESTRICAO_INEQUACAO_LINEAR_6)
			RESTRICAO_INEQUACAO_LINEAR_7(DECLARAR_RESTRICAO_INEQUACAO_LINEAR_7)


			void instanciarProcessoEstocastico(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados, const IdProcesso a_idProcesso);

		void criarVariaveis_e_Restricoes_ComandoTermeletrico(const TipoSubproblemaSolver a_TSS, Dados& a_dados);

		int criarVariaveis_Decisao_e_Estado_PTDISPCOM(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_comandado, const IdTermeletrica a_idTermeletrica, const int a_codigo_usina);
		void criarRestricoesLimites_PTDISPCOM(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_comandado, const IdTermeletrica a_idTermeletrica, const int a_codigo_usina);

		void criarVariaveisDecisaoPTDISPPRECOM(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdTermeletrica a_idTermeletrica);

		void criarRestricoes_PTDISPPRECOM_e_PTDISPCOM_horizonte_estudo(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdTermeletrica a_idTermeletrica);


		int criarVariaveis_Decisao_e_Estado_PTDISPCOM_ACUMULADO(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_comandado, const IdTermeletrica a_idTermeletrica, const int a_codigo_usina);

		void criarVariaveis_Decisao_e_Estado_e_Restricoes_ZPVOLUME_ACUMULADO_e_ACUMULANDO(const TipoSubproblemaSolver a_TSS, Dados& a_dados);

		int criarVariaveis_Decisao_e_Estado_YP(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const Periodo a_periodo_processo_estocastico, const int a_lag, const IdProcessoEstocastico a_tipo_processo_estocastico_hidrologico, const IdVariavelAleatoria a_idVariavelAleatoria, const bool a_limite_nulo_lag0 = false);
		
		int criarVariaveis_Decisao_e_Estado_YP_ACUMULADO_From_CorteBenders(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo_inicial, const Periodo a_periodo_processo_estocastico_PAR, const int a_lag, const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria, const SmartEnupla <Periodo, bool> a_horizonte_tendencia_mais_processo_estocastico, IdVariavelEstado& a_idVariavelEstado_criada);

		void criarVariaveis_Decisao_e_Estado_YH(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_processo_estocastico, const IdProcessoEstocastico a_tipo_processo_estocastico_hidrologico);

		int criarVariaveis_Decisao_e_Estado_YH_From_CorteBenders(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const Periodo a_periodo_processo_estocastico, const IdHidreletrica a_idHidreletrica);

		int retornarVariaveis_Decisao_QDEF(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_defluencia_passada, const Periodo a_periodo_estudo_inicial, const IdHidreletrica a_idHidreletrica);

		int criarVariaveis_Decisao_QDEF_From_CorteBenders(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_defluencia_passada, const Periodo a_periodo_estudo_inicial, const IdHidreletrica a_idHidreletrica);

		void somar_VDEF_estagio(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_deslocado, const Periodo a_periodo_estudo, const Periodo a_periodo_estudo_inicial, const Periodo a_menor_periodo_estagio_acoplamento_pre_estudo, const IdHidreletrica a_idHidreletrica, const SmartEnupla <Periodo, bool> a_horizonte_defluencia_passada, const int a_posEquDEF_VIAJ_BH);
		
		void somar_YP_parcela(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_processo_estocastico_PAR, const Periodo a_periodo_estudo_inicial, const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria, const SmartEnupla <Periodo, bool> a_horizonte_tendencia_mais_processo_estocastico, const int a_posEqu_YP_ACUMULADO);

		int criarVariaveis_Decisao_e_Estado_VDEF_ACUMULADO_e_VDEF(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_deslocado, const Periodo a_periodo_estudo, const Periodo a_periodo_estudo_inicial, const Periodo a_menor_periodo_estagio_acoplamento_pre_estudo, const IdHidreletrica a_idHidreletrica, const IdParcelaDefluenciaViajante a_maiorParcelaDefluenciaViajante, const SmartEnupla <Periodo, bool> a_horizonte_defluencia_passada, const SmartEnupla <IdParcelaDefluenciaViajante, IdEstagio> a_enupla_idEstagio_parcela);
		
		int criarVariaveis_Decisao_e_Estado_YP_ACUMULADO(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_processo_estocastico_PAR, const Periodo a_periodo_estudo_inicial, const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria, const int a_numero_parcelas_processo_estocastico_PAR, const SmartEnupla <Periodo, bool> a_horizonte_tendencia_mais_processo_estocastico, const SmartEnupla <int, IdEstagio> a_enupla_idEstagio_parcela, IdVariavelEstado &a_idVariavelEstado_criada);

		void criarVariaveis_Decisao_YHF(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_processo_estocastico, const IdProcessoEstocastico a_tipo_processo_estocastico_hidrologico, const TipoRelaxacaoAfluenciaIncremental a_tipo_relaxacao_afluencia_incremental);

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

		void criarVariaveisAssociadasHorizonteProcessoEstocastico(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const TipoRelaxacaoAfluenciaIncremental a_tipo_relaxacao_afluencia_incremental, const IdProcessoEstocastico a_tipo_processo_estocastico_hidrologico, const IdEstagio a_estagio_inicial, const IdCenario a_cenario_final, const IdCenario a_cenario_inicial);

		void criarRestricoesPotenciaHidraulicaDisponivel(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdPatamarCarga a_idPatamarCarga, const Periodo a_periodo_estudo, const IdHidreletrica a_idHidreletrica, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio);

		void criarRestricoesVolumeUtil(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdHidreletrica a_idHidreletrica, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio);

		void criarRestricoesFuncaoProducaoHidreletrica(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga, const SmartEnupla<Periodo, double>a_horizonte_estudo_estagio, const Periodo a_periodo_estudo);

		void criarRestricoesProducaoTermeletrica_porProdutibilidade(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdPatamarCarga a_idPatamarCarga, const Periodo a_periodo_estudo, const IdTermeletrica a_maiorIdTermeletrica, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio);

		void criarVariaveisVolume(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const SmartEnupla<Periodo, double> &a_horizonte_estudo_estagio, const SmartEnupla<Periodo, IdEstagio> &a_horizonte_estudo_vetor);

		void criarRestricoesCustoPenalidade_periodoEstudo_patamarCarga(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdSubmercado a_maiorIdSubmercado, \
			const IdIntercambio a_maiorIdIntercambio, const IdRestricaoEletrica a_maiorIdRestricaoEletrica, const IdTermeletrica a_maiorIdTermeletrica, const IdRestricaoOperativaUHE a_maiorIdRestricaoOperativaUHE, \
			const IdHidreletrica a_maiorIdHidreletrica, const IdIntercambioHidraulico a_maiorIdIntercambioHidraulico, const IdUsinaElevatoria a_maiorIdUsinaElevatoria, const TipoRelaxacaoAfluenciaIncremental a_tipo_relaxacao_afluencia_incremental, const Periodo a_periodo_otimizacao, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesCustoPenalidade_periodoEstudo(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdRestricaoOperativaUHE a_maiorIdRestricaoOperativaUHE, const IdHidreletrica a_maiorIdHidreletrica);


		void criarRestricoesBombeamentoHidraulico(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdPatamarCarga a_idPatamarCarga, const IdUsinaElevatoria a_idUsinaElevatoria);

		void criarRestricoesTurbinamentoHidraulico(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesVazaoDefluente(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesVazaoDesviada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesVazaoRetirada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesVazaoBombeada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdUsinaElevatoria a_idUsinaElevatoria, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesDefluenciaPorSuperposicaoPeriodosTempoViagemAgua(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdEstagio a_idEstagio_acoplamento, const Periodo a_periodo_acoplamento, const Periodo a_periodo_estudo, const Periodo a_periodo_estudo_inicial, const Periodo a_menor_periodo_estagio_acoplamento_pre_estudo, const Periodo a_periodo_otimizacao_final, const IdHidreletrica a_idHidreletrica, const SmartEnupla <Periodo, bool> a_horizonte_defluencia_passada);

		void criarRestricoesEletricas(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdRestricaoEletrica a_maiorIdRestricaoEletrica, const Periodo a_periodo_estudo, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesAgrupamentoIntercambio(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdAgrupamentoIntercambio a_maiorIdAgrupamentoIntercambio, const Periodo a_periodo_estudo, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesCorteBendersEmCustoFuturo(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio);
		void criarRestricoesCorteBendersEmCustoTotal(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio);

		void criarRestricoesCusto_Total_Imediato_Futuro(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdEstagio a_estagio_final, const IdHidreletrica a_maiorIdHidreletrica, const Periodo a_periodo_otimizacao, const SmartEnupla<Periodo, double> a_horizonte_estudo_estagio, const  IdProcessoEstocastico a_tipo_processo_estocastico_hidrologico);

		void criarRestricoesCustoValorPresente_periodo(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo);

		void criarRestricoesCusto_periodo(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo);

		void criarRestricoesCustoOperacao_periodoEstudo_patamarCarga(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdSubmercado a_maiorIdSubmercado, const IdTermeletrica a_maiorIdTermeletrica, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesProducaoMedia(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdSubmercado a_idSubmercado, const IdIntercambio a_maiorIdIntercambio, const IdTermeletrica a_maiorIdTermeletrica, const IdHidreletrica a_maiorIdHidreletrica, const IdPatamarCarga a_idPatamarCarga);

		void criarRestricoesBalancoHidraulicoUsinaByVolume(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdEstagio a_idEstagio_acoplamento, const Periodo a_periodo_acoplamento, const Periodo a_periodo_estudo, const Periodo a_periodo_otimizacao_final, const Periodo a_periodo_estudo_inicial, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga, const TipoRelaxacaoAfluenciaIncremental a_tipo_relaxacao_afluencia_incremental, const SmartEnupla<Periodo, double>& a_horizonte_processo_estocastico, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio);

		void criarRestricoesBalancoHidraulicoUsinaByVazao(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdEstagio a_idEstagio_acoplamento, const Periodo a_periodo_acoplamento, const Periodo a_periodo_estudo, const Periodo a_periodo_otimizacao_final, const Periodo a_periodo_estudo_inicial, const IdHidreletrica a_idHidreletrica, const IdPatamarCarga a_idPatamarCarga, const TipoRelaxacaoAfluenciaIncremental a_tipo_relaxacao_afluencia_incremental, const SmartEnupla<Periodo, double>& a_horizonte_processo_estocastico, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio);

		void criarRestricoesAtendimentoDemanda(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdUsinaEolica a_maiorIdUsinaEolica, const IdContrato a_maiorIdContrato, const IdSubmercado a_idSubmercado, const IdIntercambio a_maiorIdIntercambio, const IdTermeletrica a_maiorIdTermeletrica, const IdHidreletrica a_maiorIdHidreletrica, const IdUsinaElevatoria a_maiorIdUsinaElevatoria, const IdPatamarCarga a_idPatamarCarga);

		void formularModeloOtimizacao(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados);

		void criarRestricoesHidraulicaEspecial_vazao_afluente(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const SmartEnupla<Periodo, double>& a_horizonte_processo_estocastico, const SmartEnupla<Periodo, double>& a_horizonte_estudo_estagio);

		void criarRestricoesHidraulicaEspecial_vazao_defluente(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const IdPatamarCarga a_idPatamarCarga, const Periodo a_periodo_estudo);

		void criarRestricoesHidraulicaEspecial_volume_armazenado(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const SmartEnupla<Periodo, double>a_horizonte_estudo);

		void criarRestricoesAfluenciaIncremental_porPeriodo_horizonteProcessoEstocastico(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_processo_estocastico, const IdHidreletrica a_maiorIdHidreletrica, const IdProcessoEstocastico a_tipo_processo_estocastico_hidrologico);

		void criarRestricoesProcessoEstocasticoHidrologico_porPeriodo_horizonteProcessoEstocastico(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_inicial_estagio_inicial_processo_estocastico, const Periodo a_periodo_processo_estocastico, const IdProcessoEstocastico a_tipo_processo_estocastico_hidrologico);

		void criarRestricoesIntercambioHidraulicooRetiradaRelaxada(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdPatamarCarga a_idPatamarCarga, const IdIntercambioHidraulico a_idIntercambioHidraulico);

		void setVolumeMeta(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const SmartEnupla<Periodo, double> &a_horizonte_estudo_estagio);

		void criarRestricoesIntercambioRelaxado(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdEstagio a_idEstagio, const Periodo a_periodo_estudo, const IdIntercambio a_idIntercambio, const IdPatamarCarga a_idPatamarCarga);


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
