#ifndef ESTAGIO_OTIMIZACAO
#define ESTAGIO_OTIMIZACAO

#include "C_SmartElemento.h"

#include "C_Solver.h"

#include "C_VariavelEstado.h"
#include "C_VariavelRealizacao.h"
#include "C_RestricaoCenario.h"
#include "C_VariavelRealizacaoInterna.h"
#include "C_CorteBenders.h"

#define ATT_COMUM_ESTAGIO_OTIMIZACAO(m)  \
	  m(Estagio,  AttComum,                       idEstagio,    IdEstagio,         min,          max,           min,      sim) \
	  m(Estagio,  AttComum,                     tipo_solver,   TipoSolver,         min,          max,           min,      nao) \
	  m(Estagio,  AttComum,              periodo_otimizacao,      Periodo,         min,          max,           min,      sim) \
	  m(Estagio,  AttComum,                     lambda_CVAR,       double,           0,            1,             0,      sim) \
	  m(Estagio,  AttComum,                      alpha_CVAR,       double,           0,            1,             0,      sim) \
	  m(Estagio,  AttComum,               maiorIdRealizacao, IdRealizacao,         min,          max,           min,      nao) \
	  m(Estagio,  AttComum,                cortes_multiplos,          int,           0,          max,             0,      sim) \
	  m(Estagio,  AttComum, selecao_cortes_nivel_dominancia,          int,           0,           10,             1,      sim) 

//     c_classe,   smrtAtt,                           nomeAtributo,                      tipo,  lowerBound,   upperBound,  initialValue, mustRead?

#define ATT_VETOR_ESTAGIO_OTIMIZACAO(m)  \
	  m(Estagio,  AttVetor, request_cortes_selecionados,                int,       0,         1,        0,  IdRealizacao)   \
	  m(Estagio,  AttVetor,       selecao_solucao_proxy,               int,       0,         1,        0,  int) 

#define ATT_MATRIZ_ESTAGIO_OTIMIZACAO(m)  \
	  m(Estagio,  AttMatriz,   acao_cortes_selecionados,               int,      -1,      max,        0,  IdRealizacao, int) \
	  m(Estagio,  AttMatriz,        cortes_selecionados,    IdCorteBenders,     min,      max,      min,  IdRealizacao, int) \
	  m(Estagio,  AttMatriz,      cortes_deselecionados,    IdCorteBenders,     min,      max,      min,  IdRealizacao, int) 

#define MEMBRO_ESTAGIO_OTIMIZACAO(m)  \
	m(Estagio, CorteBenders) \
	m(Estagio, VariavelEstado) \
	m(Estagio, RestricaoCenario) \
	m(Estagio, VariavelRealizacao) \
	m(Estagio, VariavelRealizacaoInterna) 

#define SMART_ELEMENTO_ESTAGIO_OTIMIZACAO(m) \
	m(Estagio, AttComum,  ATT_COMUM_ESTAGIO_OTIMIZACAO)  \
	m(Estagio, AttVetor,  ATT_VETOR_ESTAGIO_OTIMIZACAO)  \
	m(Estagio, AttMatriz, ATT_MATRIZ_ESTAGIO_OTIMIZACAO) \
	m(Estagio,   Membro, MEMBRO_ESTAGIO_OTIMIZACAO) 
 

DEFINE_SMART_ELEMENTO(Estagio, SMART_ELEMENTO_ESTAGIO_OTIMIZACAO)

class Estagio : public SmartDados {



public:

	Estagio();
	Estagio(const Estagio &instanciaCopiar);
	void esvaziar();
	virtual ~Estagio();

	DECLARA_SMART_ELEMENTO(Estagio, SMART_ELEMENTO_ESTAGIO_OTIMIZACAO)

	IdVariavelEstado            addVariavelEstado           (const TipoSubproblemaSolver a_TSS, const string a_nome, const int a_idVariavelDecisao, const int a_idVariavelDecisaoEstagioAnterior);
	IdVariavelRealizacao        addVariavelRealizacao       (const TipoSubproblemaSolver a_TSS, const string a_nome, const int a_idVariavelDecisao, const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria, const Periodo a_periodo, const double a_fator);
	IdRestricaoCenario         addRestricaoCenario        (const TipoSubproblemaSolver a_TSS, const string a_nome, const int a_idRestricao, const SmartEnupla<IdCenario, double>& a_rhs, const SmartEnupla<int, SmartEnupla<IdCenario, double>> &a_coeficiente);
	IdVariavelRealizacaoInterna addVariavelRealizacaoInterna(const TipoSubproblemaSolver a_TSS, const string a_nome, const int a_idVariavelDecisao, const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria, const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const Periodo a_periodo, const double a_fator, const TipoValor a_tipo_valor, const double percentual_inicial, const double percentual_passo);

	void addValorVariavelEstado(const IdVariavelEstado a_idVariavelEstado, const IdCenario a_idCenario, const IdCenario a_menorIdCenario, const IdCenario a_maiorIdCenario, const double a_valor);
	void alocarVariaveisEstado(const IdCenario a_menorIdCenario, const IdCenario a_maiorIdCenario);

	std::vector<std::string> getNomeVariavelEstado(const IdVariavelEstado a_idVariavelEstado);

	void setVariavelDecisaoEmVariavelEstado        (const IdVariavelEstado a_idVariavelEstado, const TipoSubproblemaSolver a_TSS, const int a_idVariavelDecisao);
	void setVariavelDecisaoAnteriorEmVariavelEstado(const IdVariavelEstado a_idVariavelEstado, const TipoSubproblemaSolver a_TSS, const int a_idVariavelDecisao_anterior);

	double calcularValorVariavelRealizacaoInterna(const IdVariavelRealizacaoInterna idVariavelRealizacaoInterna, const double a_valor);
	void resetarValorVariavelRealizacaoInterna(const TipoSubproblemaSolver a_tipoSubproblemaSolver, const IdVariavelRealizacaoInterna idVariavelRealizacaoInterna);

	void selecaoSolucaoProxy(const int a_numero_aberturas_solucao_proxy);

	void instanciarCorteBenders(const SmartEnupla<IdRealizacao, double> &a_rhs, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> &a_coeficiente, const SmartEnupla<IdVariavelEstado, double> &a_estado);
	void instanciarCorteBenders(const SmartEnupla<IdRealizacao, double> &a_rhs, const SmartEnupla<IdRealizacao, double> &a_rhs_primal, const SmartEnupla<IdRealizacao, double> &a_rhs_estado, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> &a_coeficiente, const SmartEnupla<IdVariavelEstado, double> &a_estado);

	SmartEnupla<IdCorteBenders, IdCorteBenders> sequenciarCorteBenders();

	void requestCorteBenders(const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders, double &a_rhs, SmartEnupla<IdVariavelEstado, double> &a_coeficiente);

	void requestCorteBendersFinalizado(const IdRealizacao a_idRealizacao);

	void anularVariavelEstadoCorteBenders(const IdVariavelEstado a_idVariavelEstado);

	void anularVariavelEstadoCorteBenders(const IdVariavelEstado a_idVariavelEstado, const IdCorteBenders a_idCorteBenders);

	void alocarCorteBenders(const int a_numero_objetos);

	void instanciarSolver(const TipoSubproblemaSolver a_tipoSubproblemaSolver, TipoSolver &a_tipoSolver);

	Solver* getSolver(const TipoSubproblemaSolver a_tipoSubproblemaSolver);

	bool isSolverInstanciado(const TipoSubproblemaSolver a_tipoSubproblemaSolver);

	void excluirInstanciasSolver();
	void excluirInstanciasSolver(const TipoSubproblemaSolver a_tipoSubproblemaSolver);

	void selecionarCorteBenders();

	void avaliarDominanciaCorteBenders(); 
	void avaliarDominanciaCorteBenders(const IdCorteBenders a_idCorteBenders);

	void setElementosCorteBenders(const IdCorteBenders a_idCorteBenders, const SmartEnupla<IdVariavelEstado, double> &a_estado, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>>& a_coeficiente);

	void avaliarDominanciaCorteBendersParaEstado(const IdCorteBenders a_idCorteBenders_a_ser_avaliado, const IdCorteBenders a_idCorteBenders_com_estado);

	std::vector<TipoSubproblemaSolver> getListaTipoSubproblemaSolver() { return lista_tipoSubproblemaSolver; };

	void exportarVersaoAlternativaCorteBenders(Estagio &a_estagio, const std::string a_idProcessoEstocasticoNovo, const std::string a_idProcessoEstocasticoNovo_compacto, const SmartEnupla<IdVariavelAleatoria, std::vector<std::string>> &a_lista_nome_compacto_idVariavelAleatoriaNova, const SmartEnupla<IdVariavelAleatoria, std::vector<std::string>>& a_lista_nome_idVariavelAleatoriaNova, const SmartEnupla<IdVariavelAleatoria, std::vector<double>> &a_lista_participacao_idVariavelAleatoriaNova);

	bool carregarRHSCortesBenders(const std::string a_nome_arquivo);
	bool carregarCoeficientesCortesBenders(const std::string a_nome_arquivo);
	bool carregarEstadosCortesBenders(const std::string a_nome_arquivo);

	void removerTodosCorteBenders();

private:

	void selecionarCorteBenders(const IdCorteBenders a_idCorteBenders, SmartEnupla<int, IdCorteBenders> &a_cortes_selecionados);

	int isCorteBendersSelecionado(const IdCorteBenders a_idCorteBenders, const SmartEnupla<int, IdCorteBenders> &a_cortes_selecionados);

	double getCustoCorteBendersFromEstado(const IdCorteBenders a_idCorteBenders_a_ser_avaliado, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders_com_estado);

	std::vector<Solver*> vetorPtrSolver;

	std::vector<TipoSubproblemaSolver> lista_tipoSubproblemaSolver;

};

GET_STRING_FROM_CLASS(Estagio)

#endif 
