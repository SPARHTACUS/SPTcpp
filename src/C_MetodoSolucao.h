#ifndef METODO_SOLUCAO
#define METODO_SOLUCAO

#include "C_SmartElemento.h"

#include "C_Dados.h"

#include "C_ModeloOtimizacao.h"

#include "C_EstruturaResultados.h"

class EntradaSaidaDados;

#define ATT_COMUM_METODO_SOLUCAO(m)  \
	  m(MetodoSolucao,  AttComum,      idMetodoSolucao,  IdMetodoSolucao,        min,          max,        min,      sim) \
	  m(MetodoSolucao,  AttComum, tempo_execucao_total,           double,          0,          max,          0,      sim) 

	
#define ATT_VETOR_METODO_SOLUCAO(m)  \
	  m(MetodoSolucao,  AttVetor,              custo_inferior,    double,     0,      max,      0,  IdIteracao) \
	  m(MetodoSolucao,  AttVetor,              custo_superior,    double,     0,      max,      0,  IdIteracao) \
	  m(MetodoSolucao,  AttVetor,       tempo_medio_solver_fw,    double,     0,      max,      0,  IdIteracao) \
	  m(MetodoSolucao,  AttVetor,       tempo_medio_solver_bw,    double,     0,      max,      0,  IdIteracao) \
	  m(MetodoSolucao,  AttVetor,              tempo_execucao,    double,     0,      max,      0,  IdIteracao) \
	  m(MetodoSolucao,  AttVetor,    tempo_execucao_acumulado,    double,     0,      max,      0,  IdIteracao) 

#define ATT_MATRIZ_METODO_SOLUCAO(m)  \
	  m(MetodoSolucao,  AttMatriz,         custo_inferior,    double,     0,      max,      0,  IdIteracao,  IdCenario) \
	  m(MetodoSolucao,  AttMatriz,         custo_superior,    double,     0,      max,      0,  IdIteracao,  IdCenario) \
      m(MetodoSolucao,  AttMatriz,  probabilidade_cenario,    double,     0,      max,      0,  IdIteracao,  IdCenario) 

#define SMART_ELEMENTO_METODO_SOLUCAO(m) \
	m(MetodoSolucao, AttComum,  ATT_COMUM_METODO_SOLUCAO) \
	m(MetodoSolucao, AttVetor,  ATT_VETOR_METODO_SOLUCAO) \
	m(MetodoSolucao, AttMatriz, ATT_MATRIZ_METODO_SOLUCAO)

DEFINE_SMART_ELEMENTO(MetodoSolucao, SMART_ELEMENTO_METODO_SOLUCAO)

class MetodoSolucao : public SmartDados {
public:

	MetodoSolucao(EntradaSaidaDados a_entradaSaidaDados, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdMetodoSolucao a_idMetodoSolucao, ModeloOtimizacao &a_modeloOtimizacao);
	MetodoSolucao(const MetodoSolucao &instanciaCopiar);
	void esvaziar();
	virtual ~MetodoSolucao();

	DECLARA_SMART_ELEMENTO(MetodoSolucao, SMART_ELEMENTO_METODO_SOLUCAO)

private:

	void executarPDDE(EntradaSaidaDados a_entradaSaidaDados, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, ModeloOtimizacao &a_modeloOtimizacao);

	void executarPDDE_forward (EntradaSaidaDados a_entradaSaidaDados, const IdEstagio a_estagio_inicial, const IdEstagio a_estagio_final, const bool a_simulacao, const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, ModeloOtimizacao &a_modeloOtimizacao);
	void executarPDDE_backward_new(EntradaSaidaDados a_entradaSaidaDados, const IdEstagio a_estagio_inicial, const IdEstagio a_estagio_final, const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, ModeloOtimizacao &a_modeloOtimizacao);
	
	void executarPSD(EntradaSaidaDados a_entradaSaidaDados, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, ModeloOtimizacao& a_modeloOtimizacao);
		
	void executarPDDE_atualizarCustoSuperior(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const SmartEnupla<IdCenario, double>& a_custo_superior, const SmartEnupla<IdCenario, double>& a_probabilidade_cenario, ModeloOtimizacao &a_modeloOtimizacao);
	void executarPDDE_atualizarCustoInferior(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const SmartEnupla<IdCenario, double> &a_custo_inferior, ModeloOtimizacao &a_modeloOtimizacao);

	void executarPDDE_avaliarSolucao(EntradaSaidaDados a_entradaSaidaDados, const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, ModeloOtimizacao &a_modeloOtimizacao);

	void executarPDDE_calcularCorteBendersCVaR(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, const SmartEnupla <IdCenario, SmartEnupla<IdRealizacao, double>>& a_custo_total, const SmartEnupla <IdCenario, SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>>>& a_solucao_dual_variavel_estado, const SmartEnupla <IdCenario, SmartEnupla<IdVariavelEstado, double>>& a_valor_variavel_estado, ModeloOtimizacao& a_modeloOtimizacao);
	
	void executarPDDE_calcularCorteBendersMultiCut(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, const SmartEnupla <IdCenario, SmartEnupla<IdRealizacao, double>>& a_custo_total, const SmartEnupla <IdCenario, SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>>>& a_solucao_dual_variavel_estado, const SmartEnupla <IdCenario, SmartEnupla<IdVariavelEstado, double>>& a_valor_variavel_estado, ModeloOtimizacao& a_modeloOtimizacao);

	void executarPDDE_adicionarCorteBenders(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, double* a_vetor_estado, double* a_vetor_rhs, double* a_vetor_rhs_primal, double* a_vetor_rhs_estado, double* a_vetor_coeficiente, int* a_numero_cenario_por_processo, const int a_maior_numero_cenario, int* a_abertura_otimizada_por_cenario, const int a_multiplicidade_corte, const int a_numero_estado, ModeloOtimizacao& a_modeloOtimizacao);
	
	void executarPDDE_requestCorteBenders(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, ModeloOtimizacao &a_modeloOtimizacao, const std::string a_diretorio, EntradaSaidaDados a_entradaSaidaDados);

	void executarPDDE_distribuirEstadosEntreProcessos(const IdProcesso a_idProcesso, const IdProcesso a_maior_processo, const IdIteracao a_idIteracao, ModeloOtimizacao& a_modeloOtimizacao);
	void executarPDDE_distribuirRealizacoesEntreProcessos(const IdProcesso a_idProcesso, const IdProcesso a_maior_processo, const IdIteracao a_idIteracao, ModeloOtimizacao& a_modeloOtimizacao);

	void executarPDDE_operacao_final(EntradaSaidaDados a_entradaSaidaDados, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, ModeloOtimizacao& a_modeloOtimizacao);

	void executarPDDE_imprimirEstados(EntradaSaidaDados a_entradaSaidaDados, const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, ModeloOtimizacao& a_modeloOtimizacao);

	void incrementarTempoExecucao(const IdIteracao a_idIteracao, const double a_tempo_adicional);

	void executarPDDE_calcularCorteBenders_new(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, EstruturaResultados<int>& a_status_otimizacao, EstruturaResultados<double>& a_custo_total, EstruturaResultados<double>& a_sol_inf_var_dinamica, EstruturaResultados<double>& a_solucao_dual_var_dinamica, EstruturaResultados<double>& a_limite_inferior_var_dinamica, EstruturaResultados<double>& a_limite_superior_var_dinamica, EstruturaResultados<double>& a_sol_dual_var_estado, EstruturaResultados<double>& a_vlr_var_estado, ModeloOtimizacao& a_modeloOtimizacao);

	void executarPDDE_sincronizarResultadosParaCorteBenders(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, EstruturaResultados<int>& a_status_otimizacao, EstruturaResultados<double>& a_custo_total, EstruturaResultados<double>& a_sol_inf_var_dinamica, EstruturaResultados<double>& a_solucao_dual_var_dinamica, EstruturaResultados<double>& a_limite_inferior_var_dinamica, EstruturaResultados<double>& a_limite_superior_var_dinamica, EstruturaResultados<double>& a_sol_dual_var_estado, EstruturaResultados<double>& a_vlr_var_estado, ModeloOtimizacao& a_modeloOtimizacao, const bool a_busca_global);

	void executarPDDE_calcularCorteBendersValorEsperado_new(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, EstruturaResultados<double>& a_custo_total, EstruturaResultados<double>& a_sol_dual_var_estado, EstruturaResultados<int> & a_map_solucao_dual_proxy, EstruturaResultados<double>& a_vlr_var_estado, ModeloOtimizacao& a_modeloOtimizacao, const bool a_busca_global);

	void executarPDDE_calcularCorteBendersCVaR_new(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, EstruturaResultados<double>& a_custo_total, EstruturaResultados<double>& a_sol_dual_var_estado, EstruturaResultados<int>& a_map_solucao_dual_proxy, EstruturaResultados<double>& a_vlr_var_estado, ModeloOtimizacao& a_modeloOtimizacao, const bool a_busca_global);

	void executarPDDE_calcularCorteBendersMultiCut_new(const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, EstruturaResultados<double>& a_custo_total, EstruturaResultados<double>& a_sol_dual_var_estado, EstruturaResultados<int>& a_map_solucao_dual_proxy, EstruturaResultados<double>& a_vlr_var_estado, ModeloOtimizacao& a_modeloOtimizacao, const bool a_busca_global);

	void executarPDDE_mapearSolucaoProxy(EstruturaResultados<int>& a_status_otimizacao, EstruturaResultados<double>& a_custo_total, EstruturaResultados<double>& a_sol_inf_var_dinamica, EstruturaResultados<double>& a_solucao_dual_var_dinamica, EstruturaResultados<double>& a_limite_inferior_var_dinamica, EstruturaResultados<double>& a_limite_superior_var_dinamica, EstruturaResultados<double>& a_sol_dual_var_estado, EstruturaResultados<int> &a_map_solucao_dual_proxy, ModeloOtimizacao& a_modeloOtimizacao, const bool a_busca_global);


};

#endif 
