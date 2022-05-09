#ifndef SOLVER_H
#define SOLVER_H

#include <vector>

#include <string>

#include <chrono>

class Solver {

public:

	TipoStatusSolver statusOtimizacao;

	virtual bool isNomeSimplificado() { throw std::invalid_argument("Solver - Classe derivada nao instanciada."); };

	virtual bool inicializar() { throw std::invalid_argument("Solver - Classe derivada nao instanciada."); };
	virtual bool resetar() { throw std::invalid_argument("Solver - Classe derivada nao instanciada."); };

	virtual int addVar(const double a_lb, const double a_ub, const double a_obj, const std::string a_nome) { throw std::invalid_argument("Solver::addVar() - Classe derivada nao instanciada."); };

	virtual bool setCofObjetivo(const int a_posicao, const double a_cofObjetivo) { throw std::invalid_argument("Solver::setCofObjetivo() - Classe derivada nao instanciada."); };
	virtual bool setLimInferior(const int a_posicao, const double a_limInferior) { throw std::invalid_argument("Solver::setLimInferior() - Classe derivada nao instanciada."); };
	virtual bool setLimSuperior(const int a_posicao, const double a_limSuperior) { throw std::invalid_argument("Solver::setLimSuperior() - Classe derivada nao instanciada."); };

	virtual bool setTipoVariavelContinua(const int a_posicao) { throw std::invalid_argument("Solver::setTipoVariavelContinua() - Classe derivada nao instanciada."); };
	virtual bool setTipoVariavelBinaria(const int a_posicao) { throw std::invalid_argument("Solver::setTipoVariavelBinaria() - Classe derivada nao instanciada."); };

	virtual double getCofObjetivo(const int a_posicao) { throw std::invalid_argument("Solver::getCofObjetivo() - Classe derivada nao instanciada."); };
	virtual double getLimInferior(const int a_posicao) { throw std::invalid_argument("Solver::getLimInferior() - Classe derivada nao instanciada."); };
	virtual double getLimSuperior(const int a_posicao) { throw std::invalid_argument("Solver::getLimSuperior() - Classe derivada nao instanciada."); };
	virtual double getValorPrimal(const int a_posicao) { throw std::invalid_argument("Solver::getValorPrimal() - Classe derivada nao instanciada."); };
	virtual double getReducedCost(const int a_posicao) { throw std::invalid_argument("Solver::getReducedCost() - Classe derivada nao instanciada."); };

	virtual double getInfinito() { throw std::invalid_argument("Solver::getInfinito() - Classe derivada nao instanciada."); };

	virtual int addConstrIgual(const std::string a_nome) { throw std::invalid_argument("Solver::addConstrIgual() - Classe derivada nao instanciada."); };
	virtual int addConstrMaior(const std::string a_nome) { throw std::invalid_argument("Solver::addConstrMaior() - Classe derivada nao instanciada."); };

	virtual bool remRestricao(const int a_posicaoRestricao) { throw std::invalid_argument("Solver::remRestricao() - Classe derivada nao instanciada."); };

	virtual bool addVarDinamica(const int a_posicaoVariavel) { throw std::invalid_argument("Solver::addVarDinamica() - Classe derivada nao instanciada."); };

	virtual double getSolucaoInferiorVarDinamica() { throw std::invalid_argument("Solver::getSolucaoIntegradaVarDinamica() - Classe derivada nao instanciada."); };

	virtual std::vector<double> getLimiteInferiorVarDinamica() { throw std::invalid_argument("Solver::getSolucaoIntegradaVarDinamica() - Classe derivada nao instanciada."); };
	virtual std::vector<double> getLimiteSuperiorVarDinamica() { throw std::invalid_argument("Solver::getSolucaoIntegradaVarDinamica() - Classe derivada nao instanciada."); };
	virtual std::vector<double> getReducedCostVarDinamica()    { throw std::invalid_argument("Solver::getSolucaoIntegradaVarDinamica() - Classe derivada nao instanciada."); };

	virtual void getLimiteInferiorVarDinamica(const int a_idx, double* a_array) = 0;
	virtual void  getLimiteSuperiorVarDinamica(const int a_idx, double* a_array) = 0;
	virtual void  getReducedCostVarDinamica(const int a_idx, double* a_array) = 0;

	virtual bool setNomeRestricao(const int a_posicaoRestricao, const std::string a_nome) { throw std::invalid_argument("Solver::setNomeRestricao() - Classe derivada nao instanciada."); };

	virtual bool setCofRestricao(const int a_posicaoVariavel, const int a_posicaoRestricao, const double a_cofRestricao) { throw std::invalid_argument("Solver::setCofRestricao() - Classe derivada nao instanciada."); };
	virtual bool setRHSRestricao(const int a_posicaoRestricao, const double a_RHS) { throw std::invalid_argument("Solver::setRHSRestricao() - Classe derivada nao instanciada."); };

	virtual double getCofRestricao(const int a_posicaoVariavel, const int a_posicaoRestricao) { throw std::invalid_argument("Solver::getCofRestricao() - Classe derivada nao instanciada."); };
	virtual double getRHSRestricao(const int a_posicaoRestricao) { throw std::invalid_argument("Solver::getRHSRestricao() - Classe derivada nao instanciada."); };
	virtual double getMultiplicador(const int a_posicaoRestricao) { throw std::invalid_argument("Solver::getMultiplicador() - Classe derivada nao instanciada."); };

	virtual char getSinalRestricao(const int a_posicaoRestricao) { throw std::invalid_argument("Solver::getSinalRestricao() - Classe derivada nao instanciada."); };

	virtual TipoStatusSolver getStatusOtimizacao() { throw std::invalid_argument("Solver::getStatusOtimizacao() - Classe derivada nao instanciada."); };
	virtual double getTempoOtimizacao() { throw std::invalid_argument("Solver::getTempoOtimizacao() - Classe derivada nao instanciada."); };

	virtual bool otimizar() { throw std::invalid_argument("Solver::otimiza() - Classe derivada nao instanciada."); };

	virtual bool atualizar() { throw std::invalid_argument("Solver::atualizar() - Classe derivada nao instanciada."); };

	virtual bool setMetodoPadrao(const TipoMetodoSolver a_tipoMetodoSolver) = 0;
	virtual bool setMetodoPadrao() { throw std::invalid_argument("Solver::setMetodoPadrao() - Classe derivada nao instanciada."); };
	virtual bool setMetodo(const TipoMetodoSolver a_tipoMetodoSolver) { throw std::invalid_argument("Solver::setMetodo() - Classe derivada nao instanciada."); };
	virtual bool setMetodoAuto() { throw std::invalid_argument("Solver::setMetodoAuto() - Classe derivada nao instanciada."); };

	virtual bool setFocoNumerico(const int a_foco_numerico) { throw std::invalid_argument("Solver::setFocoNumerico() - Classe derivada nao instanciada."); };
	virtual bool setToleranciaOtimalidade(const double a_tolerancia_otimalidade) { throw std::invalid_argument("Solver::setToleranciaOtimalidade() - Classe derivada nao instanciada."); };
	virtual bool setToleranciaViabilidade(const double a_tolerancia_viabilidade) { throw std::invalid_argument("Solver::setToleranciaViabilidade() - Classe derivada nao instanciada."); };

	virtual bool setTempoLimite() { throw std::invalid_argument("Solver::setTempoLimite() - Classe derivada nao instanciada."); };
	virtual bool setTempoLimite(const double a_tempo_limite) { throw std::invalid_argument("Solver::setTempoLimite() - Classe derivada nao instanciada."); };
	virtual double getTempoLimite() { throw std::invalid_argument("Solver::setTempoLimite() - Classe derivada nao instanciada."); };

	virtual double getToleranciaOtimalidade() const = 0;

	virtual double getToleranciaViabilidade() const = 0;

	virtual int getNumeroVarDinamica() = 0;

	virtual int calcularIIS() { throw std::invalid_argument("Solver::calcularIIS() - Classe derivada nao instanciada."); };

	virtual bool isRestricaoIIS(const int a_posicaoRestricao) { throw std::invalid_argument("Solver::isRestricaoIIS() - Classe derivada nao instanciada."); };

	virtual bool isLimiteInferiorIIS(const int a_posicaoVariavel) { throw std::invalid_argument("Solver::isLimiteInferiorIIS() - Classe derivada nao instanciada."); };
	virtual bool isLimiteSuperiorIIS(const int a_posicaoVariavel) { throw std::invalid_argument("Solver::isLimiteSuperiorIIS() - Classe derivada nao instanciada."); };

	virtual bool exibirNaTela(const bool exibirNaTela) { throw std::invalid_argument("Solver::exibirNaTela() - Classe derivada nao instanciada."); };

	virtual double getValorObjetivo() { throw std::invalid_argument("Solver::getValorObjetivo() - Classe derivada nao instanciada."); };
	virtual double getValorObjetivoBound() { throw std::invalid_argument("Solver::getValorObjetivo() - Classe derivada nao instanciada."); };

	virtual bool imprimirMPS(const std::string a_nomeArquivo) = 0;
	virtual bool imprimirLP(const std::string a_nomeArquivo) { throw std::invalid_argument("Solver::imprimirLP() - Classe derivada nao instanciada."); };
	virtual bool imprimirILP(const std::string a_nomeArquivo) { throw std::invalid_argument("Solver::imprimirILP() - Classe derivada nao instanciada."); };

	virtual std::string str() { throw std::invalid_argument("Solver::str() - Classe derivada nao instanciada."); };

	virtual int otimizarComTratamentoInviabilidade() = 0;

}; // class Solver {


static std::string getString(Solver* a_ptrSolver) { return a_ptrSolver->str(); }


#ifdef GRB

#include "gurobi_c++.h"


class SolverGRB : public Solver {

private:

	GRBEnv*    ptrAmbiente = nullptr;
	GRBModel*  ptrModelo   = nullptr;

	double infinitoSolver = GRB_INFINITY;

	double tempo_otimizacao = 0.0;

	TipoMetodoSolver tipoMetodoSolver = TipoMetodoSolver_dual_simplex;

	std::vector<int> lista_variavel_dinamica;

	std::vector<GRBVar>    vetorGRBVar;
	std::vector<GRBConstr> vetorGRBConstr;

public:

	SolverGRB() {
		try{
			inicializar(); 
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::SolverGRB(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::SolverGRB(): \n" + std::string(erro.what())); }
	}; // SolverGRB() {


	~SolverGRB() {
		if (ptrModelo != nullptr) {
			delete ptrModelo;
			ptrModelo = nullptr;
		} // if (ptrModelo != nullptr) {

		if (ptrAmbiente != nullptr) {
			delete ptrAmbiente;
			ptrAmbiente = nullptr;
		} // if (ptrAmbiente != nullptr) {
	}; // ~SolverGRB() {


	std::string str() { return "GUROBI"; };

	bool isNomeSimplificado() { return false; };

	bool inicializar() {
		try {
			if (ptrModelo != nullptr)
				delete ptrModelo;
			if (ptrAmbiente != nullptr)
				delete ptrAmbiente;

			ptrAmbiente = new GRBEnv();
			ptrModelo   = new GRBModel(*ptrAmbiente);

			vetorGRBVar    = std::vector<GRBVar>();
			vetorGRBConstr = std::vector<GRBConstr>();

			lista_variavel_dinamica = std::vector<int>();

			setMetodoPadrao();

			exibirNaTela(false);

			return true;
		}

		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::inicializar(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::inicializar(): \n" + std::string(erro.what())); }


	}; // bool inicializar() {


	int addVar(const double a_lb, const double a_ub, const double a_obj, const std::string a_nome) {
		try {
			if (this == nullptr)
				throw std::invalid_argument("Solver nao instanciado");

			vetorGRBVar.push_back(ptrModelo->addVar(a_lb, a_ub, a_obj, GRB_CONTINUOUS, a_nome));
			return int(vetorGRBVar.size()) - 1;
		}

		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::addVar(" + std::to_string(a_lb) + "," + std::to_string(a_ub) + "," + std::to_string(a_obj) + "," + a_nome + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::addVar(" + std::to_string(a_lb) + "," + std::to_string(a_ub) + "," + std::to_string(a_obj) + "," + a_nome + "): \n" + std::string(erro.what())); }

	}; // bool addVar(double a_lb, double a_ub, double a_obj, std::string a_nome) {


	bool setCofObjetivo(const int a_posicao, const double a_cofObjetivo) {
		try {
			vetorGRBVar.at(a_posicao).set(GRB_DoubleAttr_Obj, a_cofObjetivo);
			return true;
		}

		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::setLimInferior(" + std::to_string(a_posicao) + "," + std::to_string(a_cofObjetivo) + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }

		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::setLimInferior(" + std::to_string(a_posicao) + "," + std::to_string(a_cofObjetivo) + "): \n" + std::string(erro.what())); }
	}; // bool setCofObjetivo(int a_posicao, double a_cofObjetivo) {


	bool setLimInferior(const int a_posicao, const double a_limInferior) {
		try {
			if (a_limInferior <= -infinitoSolver)
				vetorGRBVar.at(a_posicao).set(GRB_DoubleAttr_LB, -GRB_INFINITY);
			else
				vetorGRBVar.at(a_posicao).set(GRB_DoubleAttr_LB, a_limInferior);
			return true;
		}

		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::setLimInferior(" + std::to_string(a_posicao) + "," + std::to_string(a_limInferior) + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }

		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::setLimInferior(" + std::to_string(a_posicao) + "," + std::to_string(a_limInferior) + "): \n" + std::string(erro.what())); }
	}; // bool setLimInferior(int a_posicao, double a_limInferior) {


	bool setLimSuperior(const int a_posicao, const double a_limSuperior) {
		try {
			if (a_limSuperior >= infinitoSolver)
				vetorGRBVar.at(a_posicao).set(GRB_DoubleAttr_UB, GRB_INFINITY);
			else
				vetorGRBVar.at(a_posicao).set(GRB_DoubleAttr_UB, a_limSuperior);
			return true;
		}

		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::setLimSuperior(" + std::to_string(a_posicao) + "," + std::to_string(a_limSuperior) + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }

		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::setLimSuperior(" + std::to_string(a_posicao) + "," + std::to_string(a_limSuperior) + "): \n" + std::string(erro.what())); }
	}; // bool setLimSuperior(int a_posicao, double a_limSuperior) {	
	

	bool setTipoVariavelContinua(const int a_posicao) {
		try {
			vetorGRBVar.at(a_posicao).set(GRB_CharAttr_VType, GRB_CONTINUOUS);
			return true;
		}

		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::setTipoVariavelContinua(" + std::to_string(a_posicao) + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }

		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::setTipoVariavelContinua(" + std::to_string(a_posicao) + "): \n" + std::string(erro.what())); }
	}; // bool setTipoVariavelContinua(int a_posicao) {
			

	bool setTipoVariavelBinaria(const int a_posicao) {
		try {
			vetorGRBVar.at(a_posicao).set(GRB_CharAttr_VType, GRB_BINARY);
			return true;
		}

		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::setTipoVariavelBinaria(" + std::to_string(a_posicao) + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }

		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::setTipoVariavelBinaria(" + std::to_string(a_posicao) + "): \n" + std::string(erro.what())); }
	}; // bool setTipoVariavelBinaria(int a_posicao) {


	double getInfinito() {
		try {
			if (this == nullptr)
				throw std::invalid_argument("Solver nao instanciado");

			return infinitoSolver;
		}
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::getInfinito(): \n" + std::string(erro.what())); }
	};


	double getCofObjetivo(const int a_posicao) {
		try { return vetorGRBVar.at(a_posicao).get(GRB_DoubleAttr_Obj); }

		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::getCofObjetivo(" + std::to_string(a_posicao) + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::getCofObjetivo(" + std::to_string(a_posicao) + "): \n" + std::string(erro.what())); }
	}; // double getCofObjetivo(int a_posicao) {


	double getLimInferior(const int a_posicao) {
		try { return vetorGRBVar.at(a_posicao).get(GRB_DoubleAttr_LB); }

		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::getLimInferior(" + std::to_string(a_posicao) + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::getLimInferior(" + std::to_string(a_posicao) + "): \n" + std::string(erro.what())); }
	}; // double getLimInferior(int a_posicao) {


	double getLimSuperior(const int a_posicao) {
		try { return vetorGRBVar.at(a_posicao).get(GRB_DoubleAttr_UB); }

		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::getLimSuperior(" + std::to_string(a_posicao) + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::getLimSuperior(" + std::to_string(a_posicao) + "): \n" + std::string(erro.what())); }
	}; // double getLimSuperior(int a_posicao) {


	double getValorPrimal(const int a_posicao) {
		try { return vetorGRBVar.at(a_posicao).get(GRB_DoubleAttr_X); }
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::getValorPrimal(" + std::to_string(a_posicao) + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::getValorPrimal(" + std::to_string(a_posicao) + "): \n" + std::string(erro.what())); }
	}; // double getValorPrimal(int a_posicao) {


	double getReducedCost(const int a_posicao) {
		try { 
			if (ptrModelo->get(GRB_IntAttr_IsMIP) == 1)
				return 0.0;
			else
				return vetorGRBVar.at(a_posicao).get(GRB_DoubleAttr_RC);
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::getReducedCost(" + std::to_string(a_posicao) + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::getReducedCost(" + std::to_string(a_posicao) + "): \n" + std::string(erro.what())); }
	}; // double getReducedCost(int a_posicao) {


	int addConstrIgual(const std::string a_nome) {
		try {
			vetorGRBConstr.push_back(ptrModelo->addConstr(GRBLinExpr(0), GRB_EQUAL, 0.0, a_nome));
			return int(vetorGRBConstr.size()) - 1;
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::addConstrIgual(" + a_nome + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::addConstrIgual(" + a_nome + "): \n" + std::string(erro.what())); }
	}; // bool addConstrIgual(std::string a_nome) {


	int addConstrMaior(const std::string a_nome) {
		try {
			vetorGRBConstr.push_back(ptrModelo->addConstr(GRBLinExpr(0), GRB_GREATER_EQUAL, 0.0, a_nome));
			return int(vetorGRBConstr.size()) - 1;
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::addConstrMaior(" + a_nome + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::addConstrMaior(" + a_nome + "): \n" + std::string(erro.what())); }
	}; // bool addConstrMaior(std::string a_nome) {


	bool setNomeRestricao(const int a_posicaoRestricao, const std::string a_nome) {
		try {
			ptrModelo->set(GRB_StringAttr_ConstrName, &vetorGRBConstr.at(a_posicaoRestricao), &a_nome, 1);
			return true;
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::setNomeRestricao(" + std::to_string(a_posicaoRestricao) + "," + a_nome + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::setNomeRestricao(" + std::to_string(a_posicaoRestricao) + "," + a_nome + "): \n" + std::string(erro.what())); }
	} // bool setNomeRestricao(const int a_posicaoRestricao, const std::string a_nome) {

	bool setCofRestricao(const int a_posicaoVariavel, const int a_posicaoRestricao, const double a_cofRestricao) {
		try {
			ptrModelo->chgCoeff(vetorGRBConstr.at(a_posicaoRestricao), vetorGRBVar.at(a_posicaoVariavel), a_cofRestricao);
			return true;
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::setCofRestricao(" + std::to_string(a_posicaoVariavel) + "," + std::to_string(a_posicaoRestricao) + "," + std::to_string(a_cofRestricao) + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::setCofRestricao(" + std::to_string(a_posicaoVariavel) + "," + std::to_string(a_posicaoRestricao) + "," + std::to_string(a_cofRestricao) + "): \n" + std::string(erro.what())); }
	}; // bool setCofRestricao(int a_posicaoVariavel, int a_posicaoRestricao, double a_cofRestricao) {


	bool setRHSRestricao(const int a_posicaoRestricao, const double a_RHS) {
		try {
			vetorGRBConstr.at(a_posicaoRestricao).set(GRB_DoubleAttr_RHS, a_RHS);
			return true;
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::setRHSRestricao(" + std::to_string(a_posicaoRestricao) + "," + std::to_string(a_RHS) + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::setRHSRestricao(" + std::to_string(a_posicaoRestricao) + "," + std::to_string(a_RHS) + "): \n" + std::string(erro.what())); }
	}; // bool setRHSRestricao(int a_posicaoRestricao, double a_RHS) {


	double getCofRestricao(const int a_posicaoVariavel, const int a_posicaoRestricao) {
		try { return ptrModelo->getCoeff(vetorGRBConstr.at(a_posicaoRestricao), vetorGRBVar.at(a_posicaoVariavel)); }
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::getCofRestricao(" + std::to_string(a_posicaoVariavel) + "," + std::to_string(a_posicaoRestricao) + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::getCofRestricao(" + std::to_string(a_posicaoVariavel) + "," + std::to_string(a_posicaoRestricao) + "): \n" + std::string(erro.what())); }
	}; // double getCofRestricao(int a_posicaoVariavel, int a_posicaoRestricao) {


	double getRHSRestricao(const int a_posicaoRestricao) {
		try { return vetorGRBConstr.at(a_posicaoRestricao).get(GRB_DoubleAttr_RHS); }
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::getRHSRestricao(" + std::to_string(a_posicaoRestricao) + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::getRHSRestricao(" + std::to_string(a_posicaoRestricao) + "): \n" + std::string(erro.what())); }
	}; // double getRHSRestricao(int a_posicaoRestricao) {

	bool remRestricao(const int a_posicaoRestricao) {
		try { 
			ptrModelo->remove(vetorGRBConstr.at(a_posicaoRestricao));
			return true;
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::remRestricao(" + std::to_string(a_posicaoRestricao) + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::remRestricao(" + std::to_string(a_posicaoRestricao) + "): \n" + std::string(erro.what())); }
	}; // double getRHSRestricao(int a_posicaoRestricao) {

	bool addVarDinamica(const int a_posicaoVariavel) {
		try {
			if (vetorGRBVar.size() > a_posicaoVariavel) {
				for (int i = 0; i < lista_variavel_dinamica.size(); i++) {
					if (lista_variavel_dinamica.at(i) == a_posicaoVariavel)
						return true;
				}
				lista_variavel_dinamica.push_back(a_posicaoVariavel);
			}
			else
				throw std::invalid_argument("Variavel inexistente.");
			return true;
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::addVarDinamica(" + std::to_string(a_posicaoVariavel) + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::addVarDinamica(" + std::to_string(a_posicaoVariavel) + "): \n" + std::string(erro.what())); }
	}; // double getRHSRestricao(int a_posicaoRestricao) {

	double getSolucaoInferiorVarDinamica() {
		try {

			double valor_integrado_positivo = 0.0;
			double valor_integrado_negativo = 0.0;
			for (int i = 0; i < lista_variavel_dinamica.size(); i++) {
				const double reduced_cost = getReducedCost(lista_variavel_dinamica.at(i));
				if (reduced_cost > 0.0) {
					const double limite_inferior = getLimInferior(lista_variavel_dinamica.at(i));
					if (limite_inferior > 0.0)
						valor_integrado_positivo += reduced_cost * limite_inferior;
					else if (limite_inferior < 0.0)
						valor_integrado_negativo += reduced_cost * limite_inferior;
				}
				else if (reduced_cost < 0.0) {
					const double limite_superior = getLimInferior(lista_variavel_dinamica.at(i));
					if (limite_superior > 0.0)
						valor_integrado_negativo += reduced_cost * limite_superior;
					else if (limite_superior < 0.0)
						valor_integrado_positivo += reduced_cost * limite_superior;
				}
			}
			return valor_integrado_positivo + valor_integrado_negativo;
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::getSolucaoInferiorVarDinamica(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::getSolucaoInferiorVarDinamica(): \n" + std::string(erro.what())); }
	}; // double getRHSRestricao(int a_posicaoRestricao) {

	std::vector<double> getLimiteInferiorVarDinamica() {
		try {
			std::vector<double> limite_inferior(lista_variavel_dinamica.size(), 0.0);
			for (int i = 0; i < lista_variavel_dinamica.size(); i++)
				limite_inferior.at(i) = getLimInferior(lista_variavel_dinamica.at(i));
			return limite_inferior;
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::getLimiteInferiorVarDinamica(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::getLimiteInferiorVarDinamica(): \n" + std::string(erro.what())); }
	}; // std::vector<double> getLimiteInferiorVarDinamica() {

	void  getLimiteSuperiorVarDinamica(const int a_idx, double* a_array) {
		try {
			for (int i = 0; i < lista_variavel_dinamica.size(); i++)
				a_array[a_idx + i] = getLimSuperior(lista_variavel_dinamica.at(i));
			return;
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::getLimiteSuperiorVarDinamica(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::getLimiteSuperiorVarDinamica(): \n" + std::string(erro.what())); }
	}; // void  getLimiteSuperiorVarDinamica(const int a_idx, double* a_array) {

	void getLimiteInferiorVarDinamica(const int a_idx, double* a_array) {
		try {
			for (int i = 0; i < lista_variavel_dinamica.size(); i++)
				a_array[a_idx + i] = getLimInferior(lista_variavel_dinamica.at(i));
			return;
		}
		catch (const GRBException erro) {
			throw std::invalid_argument(
				"SolverGRB::getLimiteInferiorVarDinamica(): \n" + std::string(erro.getMessage()) + " " +
				std::to_string(erro.getErrorCode()));
		}
		catch (const std::exception& erro) {
			throw std::invalid_argument("SolverGRB::getLimiteInferiorVarDinamica(): \n" + std::string(erro.what()));
		}
	}; // std::vector<double> getLimiteInferiorVarDinamica() {

	void getReducedCostVarDinamica(const int a_idx, double* a_array) {
		try {
			for (int i = 0; i < lista_variavel_dinamica.size(); i++)
				a_array[a_idx + i] = getReducedCost(lista_variavel_dinamica.at(i));
		}
		catch (const GRBException erro) {
			throw std::invalid_argument(
				"SolverGRB::getReducedCostVarDinamica(): \n" + std::string(erro.getMessage()) + " " +
				std::to_string(erro.getErrorCode()));
		}
		catch (const std::exception& erro) {
			throw std::invalid_argument("SolverGRB::getReducedCostVarDinamica(): \n" + std::string(erro.what()));
		}
	}; // std::vector<double> getReducedCostVarDinamica() {

	std::vector<double> getLimiteSuperiorVarDinamica() {
		try {
			std::vector<double> limite_superior(lista_variavel_dinamica.size(), 0.0);
			for (int i = 0; i < lista_variavel_dinamica.size(); i++)
				limite_superior.at(i) = getLimSuperior(lista_variavel_dinamica.at(i));
			return limite_superior;
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::getLimiteSuperiorVarDinamica(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::getLimiteSuperiorVarDinamica(): \n" + std::string(erro.what())); }
	}; // std::vector<double> getLimiteSuperiorVarDinamica() {

	std::vector<double> getReducedCostVarDinamica() {
		try {
			std::vector<double> reduced_cost(lista_variavel_dinamica.size(), 0.0);
			for (int i = 0; i < lista_variavel_dinamica.size(); i++)
				reduced_cost.at(i) = getReducedCost(lista_variavel_dinamica.at(i));
			return reduced_cost;
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::getReducedCostVarDinamica(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::getReducedCostVarDinamica(): \n" + std::string(erro.what())); }
	}; // std::vector<double> getReducedCostVarDinamica() {

	int getNumeroVarDinamica() { return int(lista_variavel_dinamica.size()); }

	double getMultiplicador(const int a_posicaoRestricao) {

		try {
			if (ptrModelo->get(GRB_IntAttr_IsMIP) == 1)
				return 0.0;
			else
				return vetorGRBConstr.at(a_posicaoRestricao).get(GRB_DoubleAttr_Pi);	
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::getMultiplicador(" + std::to_string(a_posicaoRestricao) + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::getMultiplicador(" + std::to_string(a_posicaoRestricao) + "): \n" + std::string(erro.what())); }
	}; // double getMultiplicador(int a_posicaoRestricao) {


	char getSinalRestricao(const int a_posicaoRestricao) {
		try { return vetorGRBConstr.at(a_posicaoRestricao).get(GRB_CharAttr_Sense); }
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::getSinalRestricao(" + std::to_string(a_posicaoRestricao) + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::getSinalRestricao(" + std::to_string(a_posicaoRestricao) + "): \n" + std::string(erro.what())); }
	}; // char getSinalRestricao(int a_posicaoRestricao) {

	double getTempoOtimizacao() {

		try {
			if (this == nullptr)
				throw std::invalid_argument("Solver nao instanciado");

			return tempo_otimizacao;
		}
		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::getTempoOtimizacao(): \n" + std::string(erro.what())); }
	}; // double getMultiplicador(int a_posicaoRestricao) {

	bool otimizar() {
		try {
			auto start_clock = std::chrono::high_resolution_clock::now();
			ptrModelo->optimize();
			std::chrono::duration<double> tempo = std::chrono::high_resolution_clock::now() - start_clock;
			tempo_otimizacao = tempo.count();
			return true;
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::otimiza(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::otimiza(): \n" + std::string(erro.what())); }
	}; // bool otimiza() {

	int otimizarComTratamentoInviabilidade() {
		try {
			if (ptrModelo == nullptr) {
				throw std::invalid_argument("Solver invalido (NULO).");
			}

			try {
				resetar();
				otimizar();

				if (statusOtimizacao == TipoStatusSolver_otimalidade) {
					return 1;
				}
			}
			catch (const std::exception& erro) {
				const std::string erro_str = std::string(erro.what());
				if (erro_str.size() > 5) {
					if (erro_str.find("10005") != std::string::npos)
						return -1;
				}
			}

			return 0;
		}
		catch (const GRBException erro) {
			throw std::invalid_argument(
				"SolverGRB::otimizarComTratamentoInviabilidade(): \n" + std::string(erro.getMessage()) + " " +
				std::to_string(erro.getErrorCode()));
		}
		catch (const std::exception& erro) {
			throw std::invalid_argument("SolverGRB::otimizarComTratamentoInviabilidade(): \n" + std::string(erro.what()));
		}
	}

	bool resetar() {
		try {
			ptrModelo->reset(0);
			return true;
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::resetar(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::resetar(): \n" + std::string(erro.what())); }
	}; // bool otimiza() {

	bool atualizar() {
		try {
			ptrModelo->update();
			return true;
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::atualizar(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::atualizar(): \n" + std::string(erro.what())); }
	}; // bool otimiza() {

	bool setFocoNumerico(const int a_foco_numerico) {
		try {
			ptrModelo->set(GRB_IntParam_NumericFocus, a_foco_numerico);
			return true;
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::setFocoNumerico(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::setFocoNumerico(): \n" + std::string(erro.what())); }
	}; // bool otimiza() {

	bool setTempoLimite() {
		try {
			ptrModelo->set(GRB_DoubleParam_TimeLimit, infinitoSolver);
			return true;
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::setTempoLimite(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::setTempoLimite(): \n" + std::string(erro.what())); }
	}; // bool setTempoLimite() {

	bool setTempoLimite(const double a_tempo_limite) {
		try {
			ptrModelo->set(GRB_DoubleParam_TimeLimit, a_tempo_limite);
			return true;
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::setTempoLimite(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::setTempoLimite(): \n" + std::string(erro.what())); }
	}; // bool setTempoLimite() {

	double getTempoLimite() {
		try {
			return ptrModelo->get(GRB_DoubleParam_TimeLimit);
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::getTempoLimite(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::getTempoLimite(): \n" + std::string(erro.what())); }
	}; // bool setTempoLimite() {

	bool setToleranciaOtimalidade(const double a_tolerancia_otimalidade) {
		try {
			ptrModelo->set(GRB_DoubleParam_OptimalityTol, a_tolerancia_otimalidade);
			return true;
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::setToleranciaOtimalidade(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::setToleranciaOtimalidade(): \n" + std::string(erro.what())); }
	}; // bool otimiza() {

	bool setToleranciaViabilidade(const double a_tolerancia_viabilidade) {
		try {
			ptrModelo->set(GRB_DoubleParam_FeasibilityTol, a_tolerancia_viabilidade);
			return true;
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::setToleranciaViabilidade(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::seteTolranciaViabilidade(): \n" + std::string(erro.what())); }
	}; // bool otimiza() {

	double getToleranciaOtimalidade() const {
		try {
			return ptrModelo->get(GRB_DoubleParam_OptimalityTol);
		}
		catch (const GRBException erro) {
			throw std::invalid_argument(
				"SolverGRB::getToleranciaOtimalidade(): \n" + std::string(erro.getMessage()) + " " +
				std::to_string(erro.getErrorCode()));
		}
	}

	double getToleranciaViabilidade() const {
		try {
			return ptrModelo->get(GRB_DoubleParam_FeasibilityTol);
		}
		catch (const GRBException erro) {
			throw std::invalid_argument(
				"SolverGRB::getToleranciaViabilidade(): \n" + std::string(erro.getMessage()) + " " +
				std::to_string(erro.getErrorCode()));
		}
	}

	bool setMetodoPadrao(const TipoMetodoSolver a_tipoMetodoSolverPadrao) {
		try {

			tipoMetodoSolver = a_tipoMetodoSolverPadrao;

			setMetodoPadrao();

			return true;
		}
		catch (const GRBException erro) {
			throw std::invalid_argument("SolverGRB::setMetodoPadrao(): \n" + std::string(erro.getMessage()) + " " +
				std::to_string(erro.getErrorCode()));
		}
		catch (const std::exception& erro) {
			throw std::invalid_argument("SolverGRB::setMetodoPadrao(): \n" + std::string(erro.what()));
		}
	}; // bool otimiza() {

	bool setMetodoPadrao() {
		try {

			if (tipoMetodoSolver == TipoMetodoSolver_automatico)
				ptrModelo->set(GRB_IntParam_Method, GRB_METHOD_AUTO);

			else if (tipoMetodoSolver == TipoMetodoSolver_primal_simplex)
				ptrModelo->set(GRB_IntParam_Method, GRB_METHOD_PRIMAL);

			else if (tipoMetodoSolver == TipoMetodoSolver_dual_simplex)
				ptrModelo->set(GRB_IntParam_Method, GRB_METHOD_DUAL);

			else if (tipoMetodoSolver == TipoMetodoSolver_barrier) {
				ptrModelo->set(GRB_IntParam_Crossover, 0);
				ptrModelo->set(GRB_IntParam_Method, GRB_METHOD_BARRIER);
			}

			else if (tipoMetodoSolver == TipoMetodoSolver_concorrente) {
				ptrModelo->set(GRB_IntParam_Crossover, 0);
				ptrModelo->set(GRB_IntParam_Method, GRB_METHOD_CONCURRENT);
			}

			return true;
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::setMetodoPadrao(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::setMetodoPadrao(): \n" + std::string(erro.what())); }
	}; // bool otimiza() {

	bool setMetodoAuto() {
		try { return setMetodo(TipoMetodoSolver_automatico); }
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::setMetodoAuto(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::setMetodoAuto(): \n" + std::string(erro.what())); }
	}; // bool setMetodoAuto() {

	bool setMetodo(const TipoMetodoSolver a_tipoMetodoSolver) {
		try {

			if (a_tipoMetodoSolver == TipoMetodoSolver_automatico)
				ptrModelo->set(GRB_IntParam_Method, GRB_METHOD_AUTO);
			
			else if (a_tipoMetodoSolver == TipoMetodoSolver_primal_simplex)
				ptrModelo->set(GRB_IntParam_Method, GRB_METHOD_PRIMAL);
			
			else if (a_tipoMetodoSolver == TipoMetodoSolver_dual_simplex)
				ptrModelo->set(GRB_IntParam_Method, GRB_METHOD_DUAL);
				
			else if (a_tipoMetodoSolver == TipoMetodoSolver_barrier) {
				ptrModelo->set(GRB_IntParam_Method, GRB_METHOD_BARRIER);
				ptrModelo->set(GRB_IntParam_Crossover, 0);
				//ptrModelo->set(GRB_DoubleParam_BarConvTol, 1e-10);
			}
				
			else if (a_tipoMetodoSolver == TipoMetodoSolver_concorrente) {
				ptrModelo->set(GRB_IntParam_Method, GRB_METHOD_CONCURRENT);
				ptrModelo->set(GRB_IntParam_Crossover, 0);
			}

			return true;
		}
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::setMetodoPadrao(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::setMetodoPadrao(): \n" + std::string(erro.what())); }
	}; // bool otimiza() {

	double getValorObjetivo() {
		try { return ptrModelo->get(GRB_DoubleAttr_ObjVal); }
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::getValorObjetivo(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::getValorObjetivo(): \n" + std::string(erro.what())); }
	}; // double getValorObjetivo() {

	double getValorObjetivoBound() {
		try { return ptrModelo->get(GRB_DoubleAttr_ObjBound); }
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::getValorObjetivoBound(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::getValorObjetivoBound(): \n" + std::string(erro.what())); }
	}; // double getValorObjetivo() {


	TipoStatusSolver getStatusOtimizacao() {
		try {
			const int status = ptrModelo->get(GRB_IntAttr_Status);
			if (status == GRB_OPTIMAL)
				return TipoStatusSolver_otimalidade;
			else if (status == GRB_TIME_LIMIT)
				return TipoStatusSolver_tempo_excedido;
			else
				return TipoStatusSolver_nao_otimalidade;
		} // try {
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::getStatusOtimizacao(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::getStatusOtimizacao(): \n" + std::string(erro.what())); }
	}; // bool getStatusOtimizacao() {

	bool isLimiteInferiorIIS(const int a_posicaoVariavel) {
		try {
			if (vetorGRBVar.at(a_posicaoVariavel).get(GRB_IntAttr_IISLB) > 0)
				return true;
			return false;
		} // try {
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::isLimiteInferiorIIS(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::isLimiteInferiorIIS(): \n" + std::string(erro.what())); }
	}; // bool isLimiteInferiorIIS(const int a_posicaoVariavel) {

	bool isLimiteSuperiorIIS(const int a_posicaoVariavel) {
		try {
			if (vetorGRBVar.at(a_posicaoVariavel).get(GRB_IntAttr_IISUB) > 0)
				return true;
			return false;
		} // try {
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::isLimiteSuperiorIIS(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::isLimiteSuperiorIIS(): \n" + std::string(erro.what())); }
	}; // bool isLimiteSuperiorIIS(const int a_posicaoVariavel) {

	bool isRestricaoIIS(const int a_posicaoRestricao) {
		try {
			if (vetorGRBConstr.at(a_posicaoRestricao).get(GRB_IntAttr_IISConstr) > 0)
				return true;
			return false;
		} // try {
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::isRestricaoIIS(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::isRestricaoIIS(): \n" + std::string(erro.what())); }
	}; // bool isRestricaoIIS(const int a_posicaoRestricao) {

	bool imprimirLP(const std::string a_nomeArquivo) {
		try {
			 ptrModelo->write(std::string(a_nomeArquivo + ".lp"));
			 return true;
		} // try {
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::imprimirLP(" + a_nomeArquivo + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::imprimirLP(" + a_nomeArquivo + "): \n" + std::string(erro.what())); }
	}; // bool imprimePL(std::string a_nomeArquivo) {

	bool imprimirMPS(const std::string a_nomeArquivo) {
		try {
			ptrModelo->write(std::string(a_nomeArquivo + ".mps"));
			return true;
		} // try {
		catch (const GRBException erro) {
			throw std::invalid_argument(
				"SolverGRB::imprimirMPS(" + a_nomeArquivo + "): \n" + std::string(erro.getMessage()) + " " +
				std::to_string(erro.getErrorCode()));
		}
		catch (const std::exception& erro) {
			throw std::invalid_argument("SolverGRB::imprimirMPS(" + a_nomeArquivo + "): \n" + std::string(erro.what()));
		}
	}; // bool imprimirMPS(std::string a_nomeArquivo) {

	bool imprimirILP(const std::string a_nomeArquivo) {
		try {
			if (calcularIIS() == 1) {
				ptrModelo->write(std::string(a_nomeArquivo + ".ilp"));
				return true;
			}
			return false;
		} // try {
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::imprimirILP(" + a_nomeArquivo + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::imprimirILP(" + a_nomeArquivo + "): \n" + std::string(erro.what())); }
	}; // bool imprimirILP(std::string a_nomeArquivo) {

	int calcularIIS() {
		try {
			ptrModelo->computeIIS();
			return 1;
		} // try {
		catch (const GRBException erro) {
			return 0;
			//throw std::invalid_argument("SolverGRB::calcularIIS(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode()));
		}
		catch (const std::exception & erro) { throw std::invalid_argument("SolverGRB::calcularIIS(): \n" + std::string(erro.what())); }
	}; // bool getStatusOtimizacao() {

	bool exibirNaTela(const bool exibirNaTela) {
		try {
			if (exibirNaTela)
				ptrModelo->set(GRB_IntParam_OutputFlag, 1);
			else
				ptrModelo->set(GRB_IntParam_OutputFlag, 0);
			return true;
		} // try {
		catch (const GRBException erro) { throw std::invalid_argument("SolverGRB::exibirNaTela(" + std::to_string(exibirNaTela) + "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::exibirNaTela(" + std::to_string(exibirNaTela) + "): \n" + std::string(erro.what())); }
	}; // bool exibirNaTela(bool exibirNaTela) {

}; // class SolverGRB : public Solver {


#endif

#include "ClpSimplex.hpp"
#include "CoinHelperFunctions.hpp"
#include "OsiClpSolverInterface.hpp"


class SolverCLP : public Solver {

private:

	double** matrizCoeffRestrPorVar  = nullptr;
	int**    matrizIndiceRestrPorVar = nullptr;
	int**    matrizIndiceVarPorRestr  = nullptr;


	ClpSimplex*  ptrModelo = nullptr;

	int numVar = 0;
	// Arrays de Variáveis
	double* cofObjetivo = nullptr;
	double* limInferior = nullptr;
	double* limSuperior = nullptr;
	double* valorPrimal = nullptr;
	double* reducedCost = nullptr;

	std::vector<std::string> nomeVariavel;

	int numRes = 0;
	int numCof = 0;
	// Arrays de Restrições
	char*         sinais = nullptr;
	int*          vetorNumCoefPorVar   = nullptr;
	int*          vetorNumCoefPorRestr = nullptr;

	bool isModeloRedimensionado = false;

	std::vector<std::string> nomeRestricao;

	int* maiorResAdd = nullptr;

	double* RHSRestricaoSup = nullptr;
	double* RHSRestricaoInf = nullptr;

	double* multiplicador = nullptr;

	int status = 1;
	double objetivo = NAN;

	double infinitoSolver = COIN_DBL_MAX;

	double precisao = 1e-6;

	std::vector<int> lista_variavel_dinamica;

	double tempo_otimizacao = 0.0;

	TipoMetodoSolver tipoMetodoSolver        = TipoMetodoSolver_Nenhum;
	TipoMetodoSolver tipoMetodoSolver_padrao = TipoMetodoSolver_dual_simplex;

public:

	std::string str() { return "CLP"; };

	SolverCLP() {
		try { inicializar(); } // try {
		catch (const std::exception& erro) { throw std::invalid_argument("SolverCLP::SolverCLP(): \n" + std::string(erro.what())); }
	}; // SolverCLP() {

	~SolverCLP() {

		inicializar();

		if (ptrModelo != nullptr)
			delete ptrModelo;

	}; // ~SolverCLP() {

	bool isNomeSimplificado() { return true; };

	bool inicializar() {

		try {

			if (ptrModelo != nullptr)
				delete ptrModelo;

			ptrModelo = new ClpSimplex;

			ptrModelo->setPrimalTolerance(precisao);
			ptrModelo->setDualTolerance(precisao);

			valorPrimal = nullptr;
			reducedCost = nullptr;
			multiplicador = nullptr;

			if (cofObjetivo != nullptr)
				delete cofObjetivo;
			if (limInferior != nullptr)
				delete limInferior;
			if (limSuperior != nullptr)
				delete limSuperior;

			nomeVariavel = std::vector<std::string>();

			cofObjetivo = nullptr;
			limInferior = nullptr;
			limSuperior = nullptr;

			if (sinais != nullptr)
				delete sinais;
			if (vetorNumCoefPorVar != nullptr)
				delete vetorNumCoefPorVar;
			if (vetorNumCoefPorRestr != nullptr)
				delete vetorNumCoefPorRestr;
			if (maiorResAdd != nullptr)
				delete maiorResAdd;

			sinais = nullptr;
			vetorNumCoefPorVar = nullptr;
			vetorNumCoefPorRestr = nullptr;
			maiorResAdd = nullptr;

			nomeRestricao = std::vector<std::string>();

			if (RHSRestricaoSup != nullptr)
				delete RHSRestricaoSup;
			if (RHSRestricaoInf != nullptr)
				delete RHSRestricaoInf;

			RHSRestricaoSup = nullptr;
			RHSRestricaoInf = nullptr;

			for (int v = 0; v < numVar; v++) {
				if (matrizCoeffRestrPorVar[v] != nullptr)
					delete matrizCoeffRestrPorVar[v];
				if (matrizIndiceRestrPorVar[v] != nullptr)
					delete matrizIndiceRestrPorVar[v];
			} // for (int v = 0; v < numVar; v++) {

			if (matrizCoeffRestrPorVar != nullptr)
				delete matrizCoeffRestrPorVar;

			matrizCoeffRestrPorVar = nullptr;

			if (matrizIndiceRestrPorVar != nullptr)
				delete matrizIndiceRestrPorVar;

			matrizIndiceRestrPorVar = nullptr;

			for (int r = 0; r < numRes; r++) {
				if (matrizIndiceVarPorRestr[r] != nullptr)
					delete matrizIndiceVarPorRestr[r];
			}
			if (matrizIndiceVarPorRestr != nullptr)
				delete matrizIndiceVarPorRestr;

			matrizIndiceVarPorRestr = nullptr;

			numVar = 0;
			numRes = 0;
			numCof = 0;
			status = 1;
			objetivo = NAN;

			lista_variavel_dinamica = std::vector<int>();

			setMetodoPadrao();

			exibirNaTela(false);

			isModeloRedimensionado = true;

			return true;

		} // try {

		catch (const CoinError erro) { throw std::invalid_argument("SolverCLP::inicializar(): \n" + erro.className() + "::" + erro.methodName() + "(): \n" + erro.message()); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverCLP::inicializar(): \n" + std::string(erro.what())); }

	}; // bool inicializar() {

	void alocarVariavel() {

		try {

			if (numVar == 0) {

				const int allocVar = 1000;

				vetorNumCoefPorVar = new int[allocVar];
				maiorResAdd = new int[allocVar];

				cofObjetivo = new double[allocVar];
				limInferior = new double[allocVar];
				limSuperior = new double[allocVar];

				matrizCoeffRestrPorVar = new double* [allocVar];
				matrizIndiceRestrPorVar = new    int* [allocVar];

				nomeVariavel = std::vector<std::string>(allocVar, std::string());

			} // if (numVar == 0) {

			else {

				int allocVar = 0;

				if (numVar == 1000)
					allocVar = 10000;
				else if (numVar == 10000)
					allocVar = 100000;
				else if (numVar == 100000)
					allocVar = 5000000;
				else if (numVar == 500000)
					allocVar = 1000000;
				else if (numVar == 1000000)
					allocVar = 2000000;
				else if (numVar == 2000000)
					allocVar = 5000000;
				else if (numVar == 5000000)
					allocVar = 10000000;
				else if (numVar == 10000000)
					throw invalid_argument("Numero maximo de variaveis atingido! 10.000.000");

				if (allocVar > 0) {

					int* vetorNumCoefPorVarAux = new int[allocVar];
					int* maiorResAddAux = new int[allocVar];

					double* cofObjetivoAux = new double[allocVar];
					double* limInferiorAux = new double[allocVar];
					double* limSuperiorAux = new double[allocVar];

					std::vector<std::string> nomeVariavelAux = std::vector<std::string>(allocVar, std::string());

					double** matrizCoeffRestrPorVarAux = new double* [allocVar];
					int** matrizIndiceRestrPorVarAux = new    int* [allocVar];

					for (int v = 0; v < numVar; v++) {
						vetorNumCoefPorVarAux[v] = vetorNumCoefPorVar[v];
						maiorResAddAux[v] = maiorResAdd[v];

						cofObjetivoAux[v] = cofObjetivo[v];
						limInferiorAux[v] = limInferior[v];
						limSuperiorAux[v] = limSuperior[v];
						nomeVariavelAux.at(v) = nomeVariavel.at(v);

						matrizCoeffRestrPorVarAux[v] = matrizCoeffRestrPorVar[v];
						matrizIndiceRestrPorVarAux[v] = matrizIndiceRestrPorVar[v];
					} // for (int v = 0; v < numVar; v++) {

					delete vetorNumCoefPorVar;
					vetorNumCoefPorVar = vetorNumCoefPorVarAux;

					delete maiorResAdd;
					maiorResAdd = maiorResAddAux;

					delete cofObjetivo;
					cofObjetivo = cofObjetivoAux;

					delete limInferior;
					limInferior = limInferiorAux;

					delete limSuperior;
					limSuperior = limSuperiorAux;

					nomeVariavel = nomeVariavelAux;

					delete matrizCoeffRestrPorVar;
					matrizCoeffRestrPorVar = matrizCoeffRestrPorVarAux;

					delete matrizIndiceRestrPorVar;
					matrizIndiceRestrPorVar = matrizIndiceRestrPorVarAux;

				} // if (allocVar > 0) {

			} // else

		} // try {

		catch (const CoinError erro) { throw std::invalid_argument("SolverCLP::alocarVariavel(): \n" + erro.className() + "::" + erro.methodName() + "(): \n" + erro.message()); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::alocarVariavel(): \n" + std::string(erro.what())); }

	} // 	void alocarVariavel() {


	int addVar(double a_lb, double a_ub, double a_obj, std::string a_nome) {

		try {

			alocarVariavel();

			matrizCoeffRestrPorVar[numVar] = nullptr;
			matrizIndiceRestrPorVar[numVar] = nullptr;

			vetorNumCoefPorVar[numVar] = 0;
			maiorResAdd[numVar] = 0;

			if (a_lb <= -infinitoSolver)
				limInferior[numVar] = -COIN_DBL_MAX;
			else
				limInferior[numVar] = a_lb;

			if (a_ub >= infinitoSolver)
				limSuperior[numVar] = COIN_DBL_MAX;
			else
				limSuperior[numVar] = a_ub;

			nomeVariavel.at(numVar) = a_nome;

			cofObjetivo[numVar] = a_obj;

			numVar++;

			isModeloRedimensionado = true;

			return numVar - 1;

		} // try {

		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::addVar(" + std::to_string(a_lb) + "," + std::to_string(a_ub) + "," + std::to_string(a_obj) + "," + a_nome + "): \n" + std::string(erro.what())); }

	}; // bool addVar(double a_lb, double a_ub, double a_obj, std::string a_nome) {


	bool setCofObjetivo(int a_posicao, double a_cofObjetivo) {

		try {

			if (a_posicao >= numVar)
				throw invalid_argument("Variavel nao definida no solver.");

			cofObjetivo[a_posicao] = a_cofObjetivo;

			if (!isModeloRedimensionado)
				ptrModelo->setObjectiveCoefficient(a_posicao, a_cofObjetivo);

			return true;

		} // try {

		catch (const CoinError erro) { throw std::invalid_argument("SolverCLP::setCofObjetivo(" + std::to_string(a_posicao) + "," + std::to_string(a_cofObjetivo) + "): \n" + erro.className() + "::" + erro.methodName() + "(): \n" + erro.message()); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverCLP::setCofObjetivo(" + std::to_string(a_posicao) + "," + std::to_string(a_cofObjetivo) + "): \n" + std::string(erro.what())); }

	}; // bool setCofObjetivo(int a_posicao, double a_cofObjetivo) {


	bool setLimInferior(int a_posicao, double a_limInferior) {

		try {

			if (a_posicao >= numVar)
				throw invalid_argument("Variavel nao definida no solver.");

			limInferior[a_posicao] = a_limInferior;

			if (!isModeloRedimensionado)
				ptrModelo->setColLower(a_posicao, a_limInferior);

			return true;

		} // try {

		catch (const CoinError erro) { throw std::invalid_argument("SolverCLP::setLimSuperior(" + std::to_string(a_posicao) + "," + std::to_string(a_limInferior) + "): \n" + erro.className() + "::" + erro.methodName() + "(): \n" + erro.message()); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverCLP::setLimSuperior(" + std::to_string(a_posicao) + "," + std::to_string(a_limInferior) + "): \n" + std::string(erro.what())); }

	}; // bool setLimInferior(int a_posicao, double a_limInferior) {
	 

	bool setLimSuperior(int a_posicao, double a_limSuperior) {

		try {

			if (a_posicao >= numVar)
				throw invalid_argument("Variavel nao definida no solver.");

			limSuperior[a_posicao] = a_limSuperior;

			if (!isModeloRedimensionado)
				ptrModelo->setColUpper(a_posicao, a_limSuperior);

			return true;

		} // try {

		catch (const CoinError erro) { throw std::invalid_argument("SolverCLP::setLimSuperior(" + std::to_string(a_posicao) + "," + std::to_string(a_limSuperior) + "): \n" + erro.className() + "::" + erro.methodName() + "(): \n" + erro.message()); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverCLP::setLimSuperior(" + std::to_string(a_posicao) + "," + std::to_string(a_limSuperior) + "): \n" + std::string(erro.what())); }

	}; // bool setLimSuperior(int a_posicao, double a_limSuperior) {


	bool setTipoVariavelContinua(const int a_posicao) {	return true; }; // bool setTipoVariavelContinua(int a_posicao) {


	bool setTipoVariavelBinaria(const int a_posicao) {
		try { throw std::invalid_argument("O Solver CLP nao comporta variaveis binarias."); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::setTipoVariavelBinaria(" + std::to_string(a_posicao) + "): \n" + std::string(erro.what())); }
	}; // bool setTipoVariavelBinaria(int a_posicao) {


	double getInfinito() {
		try {
			if (this == nullptr)
				throw std::invalid_argument("Solver nao instanciado");

			return infinitoSolver;
		}
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::getInfinito(): \n" + std::string(erro.what())); }
	}; //double getInfinito() {


	double getCofObjetivo(int a_posicao) {

		try {

			if (a_posicao >= numVar)
				throw invalid_argument("Variavel nao definida no solver.");

			return cofObjetivo[a_posicao];

		} // try {

		catch (const std::exception& erro) { throw std::invalid_argument("SolverCLP::getCofObjetivo(" + std::to_string(a_posicao) + "): \n" + std::string(erro.what())); }

	}; // double getCofObjetivo(int a_posicao) {


	double getLimInferior(int a_posicao) {

		try {

			if (a_posicao >= numVar)
				throw invalid_argument("Variavel nao definida no solver.");

			return limInferior[a_posicao];

		} // try {

		catch (const std::exception& erro) { throw std::invalid_argument("SolverCLP::getLimInferior(" + std::to_string(a_posicao) + "): \n" + std::string(erro.what())); }

	}; // double getLimInferior(int a_posicao) {


	double getLimSuperior(int a_posicao) {

		try {

			if (a_posicao >= numVar)
				throw invalid_argument("Variavel nao definida no solver.");

			return limSuperior[a_posicao];

		} // try {

		catch (const std::exception& erro) { throw std::invalid_argument("SolverCLP::getLimSuperior(" + std::to_string(a_posicao) + "): \n" + std::string(erro.what())); }

	}; // double getLimSuperior(int a_posicao) {


	double getValorPrimal(int a_posicao) {

		try {

			if (a_posicao >= numVar)
				throw invalid_argument("Variavel nao definida no solver.");

			if (status != 0)
				return NAN;
			else
				return valorPrimal[a_posicao];

		} // try {

		catch (const std::exception& erro) { throw std::invalid_argument("SolverCLP::getValorPrimal(" + std::to_string(a_posicao) + "): \n" + std::string(erro.what())); }

	}; // double getValorPrimal(int a_posicao) {


	double getReducedCost(int a_posicao) {

		try {

			if (a_posicao >= numVar)
				throw invalid_argument("Variavel nao definida no solver.");

			if (status != 0)
				return NAN;
			else
				return reducedCost[a_posicao];

		} // try {

		catch (const std::exception& erro) { throw std::invalid_argument("SolverCLP::getReducedCost(" + std::to_string(a_posicao) + "): \n" + std::string(erro.what())); }

	}; // double getReducedCost(int a_posicao) {


	void alocarRestricao() {

		try {

			if (numRes == 0) {

				const int allocRes = 1000;

				sinais = new char[allocRes];

				nomeRestricao = std::vector<std::string>(allocRes, std::string());

				RHSRestricaoSup = new double[allocRes];
				RHSRestricaoInf = new double[allocRes];

				vetorNumCoefPorRestr = new int [allocRes];

				matrizIndiceVarPorRestr = new int* [allocRes];

			} // if (numRes == 0) {

			else {

				int allocRes = 0;

				if (numRes == 1000)
					allocRes = 10000;
				else if (numRes == 10000)
					allocRes = 100000;
				else if (numRes == 100000)
					allocRes = 500000;
				else if (numRes == 500000)
					allocRes = 1000000;
				else if (numRes == 1000000)
					allocRes = 2000000;
				else if (numRes == 2000000)
					allocRes = 5000000;
				else if (numRes == 5000000)
					allocRes = 10000000;
				else if (numRes == 10000000)
					throw invalid_argument("Numero maximo de coeficientes de restricoes atingido! 10.000.000");

				if (allocRes > 0) {

					char* sinaisAux = new   char[allocRes];

					std::vector<std::string> nomeRestricaoAux = std::vector<std::string>(allocRes, std::string());

					double* RHSRestricaoSupAux = new double[allocRes];
					double* RHSRestricaoInfAux = new double[allocRes];

					int *vetorNumCoefPorRestrAux = new int[allocRes];

					int** matrizIndiceVarPorRestrAux = new int* [allocRes];

					for (int r = 0; r < numRes; r++) {
						sinaisAux[r] = sinais[r];
						nomeRestricaoAux.at(r) = nomeRestricao.at(r);
						RHSRestricaoSupAux[r] = RHSRestricaoSup[r];
						RHSRestricaoInfAux[r] = RHSRestricaoInf[r];

						vetorNumCoefPorRestrAux[r] = vetorNumCoefPorRestr[r];

						matrizIndiceVarPorRestrAux[r] = matrizIndiceVarPorRestr[r];

					} // for (int r = 0; r < numRes; r++) {

					delete matrizIndiceVarPorRestr;
					matrizIndiceVarPorRestr = matrizIndiceVarPorRestrAux;

					delete vetorNumCoefPorRestr;
					vetorNumCoefPorRestr = vetorNumCoefPorRestrAux;

					delete sinais;
					sinais = sinaisAux;

					nomeRestricao = nomeRestricaoAux;

					delete RHSRestricaoSup;
					RHSRestricaoSup = RHSRestricaoSupAux;
					delete RHSRestricaoInf;
					RHSRestricaoInf = RHSRestricaoInfAux;

				} // if (allocCof > 0) {
			} // else

		} // try {

		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::alocarRestricao(): \n" + std::string(erro.what())); }


	} // void alocarRestricao() {

	int addConstrIgual(std::string a_nome) {

		try {

			alocarRestricao();

			matrizIndiceVarPorRestr[numRes] = nullptr;

			vetorNumCoefPorRestr[numRes] = 0;

			RHSRestricaoSup[numRes] = 0;
			RHSRestricaoInf[numRes] = 0;

			sinais[numRes] = '=';

			nomeRestricao.at(numRes) = a_nome;

			numRes++;

			isModeloRedimensionado = true;

			return numRes - 1;

		} // try {

		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::addConstrIgual(" + a_nome + "): \n" + std::string(erro.what())); }

	}; // bool addConstrIgual(std::string a_nome) {


	int addConstrMaior(std::string a_nome) {

		try {

			alocarRestricao();

			matrizIndiceVarPorRestr[numRes] = nullptr;

			vetorNumCoefPorRestr[numRes] = 0;

			RHSRestricaoSup[numRes] = COIN_DBL_MAX;
			RHSRestricaoInf[numRes] = 0;

			sinais[numRes] = '>';

			nomeRestricao.at(numRes) = a_nome;

			numRes++;

			isModeloRedimensionado = true;

			return numRes - 1;

		} // try {

		catch (const std::exception& erro) { throw std::invalid_argument("SolverCLP::addConstrMaior(" + a_nome + "): \n" + std::string(erro.what())); }

	}; // bool addConstrMaior(std::string a_nome) {

	bool setNomeRestricao(const int a_posicaoRestricao, const std::string a_nome) {
		try {

			nomeRestricao.at(a_posicaoRestricao) = a_nome;

			if (!isModeloRedimensionado) {
				std::string nome = a_nome;
				ptrModelo->setRowName(a_posicaoRestricao, nome);
			} // if (!isModeloRedimensionado) {

			return true;
		}
		catch (const CoinError erro) { throw std::invalid_argument("SolverCLP::setNomeRestricao(" + std::to_string(a_posicaoRestricao) + "," + a_nome + "): \n" + erro.className() + "::" + erro.methodName() + "(): \n" + erro.message()); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::setNomeRestricao(" + std::to_string(a_posicaoRestricao) + "," + a_nome + "): \n" + std::string(erro.what())); }
	} // bool setNomeRestricao(const int a_posicaoRestricao, const std::string a_nome) {


	bool alocarCoeficientes(int a_posicaoVariavel, int a_posicaoRestricao, double a_cofRestricao) {
		try {

			if (a_posicaoVariavel >= numVar)
				throw invalid_argument("Variavel nao definida no solver.");

			if (a_posicaoRestricao >= numRes)
				throw invalid_argument("Restricao nao definida no solver.");

			if (std::isnan(a_cofRestricao))
				throw invalid_argument("Coeficiente da restricao " + to_string(a_posicaoRestricao) + "e variavel " + to_string(a_posicaoVariavel) + " nao numerico.");

			const int numCofPorVar = vetorNumCoefPorVar[a_posicaoVariavel];
			const int numCofPorRes = vetorNumCoefPorRestr[a_posicaoRestricao];

			if (numCofPorVar == 0) {

				if (a_cofRestricao == 0.0)
					return false;

				const int allocCof = 2;

				matrizIndiceRestrPorVar[a_posicaoVariavel] = new    int[allocCof];
				matrizCoeffRestrPorVar[a_posicaoVariavel] = new double[allocCof];

			} // if (numCofPorVar == 0) {


			else {

				if (maiorResAdd[a_posicaoVariavel] >= a_posicaoRestricao) {

					// Caso coeficiente já foi adicionado ao solver.
					for (int c = 0; c < numCofPorVar; c++) {
						if (matrizIndiceRestrPorVar[a_posicaoVariavel][c] == a_posicaoRestricao) {
							matrizCoeffRestrPorVar[a_posicaoVariavel][c] = a_cofRestricao;
							return false;
						} // if (matrizIndiceRestrPorVar[a_posicaoVariavel][c] == a_posicaoRestricao) {
					} // for (int c = 0; c < numCofPorVar; c++) {

				} // if (maiorResAdd[a_posicaoVariavel] > a_posicaoRestricao) {

				if (a_cofRestricao == 0.0)
					return false;

				int allocCof = 0;

				if (numCofPorVar == 2)
					allocCof = 5;
				else if (numCofPorVar == 5)
					allocCof = 10;
				else if (numCofPorVar == 10)
					allocCof = 100;
				else if (numCofPorVar == 100)
					allocCof = 1000;
				else if (numCofPorVar == 1000)
					allocCof = 100000;
				else if (numCofPorVar == 100000)
					allocCof = 1000000;
				else if (numCofPorVar == 1000000)
					allocCof = 10000000;
				else if (numCofPorVar == 10000000)
					throw invalid_argument("Numero maximo de coeficientes de restricoes atingido! 10.000.000");

				if (allocCof > 0) {

					// Caso coeficiente é novo.
					int* linhasNovo = new    int[allocCof];
					double* coeficientesNovo = new double[allocCof];

					for (int c = 0; c < numCofPorVar; c++) {
						linhasNovo[c] = matrizIndiceRestrPorVar[a_posicaoVariavel][c];
						coeficientesNovo[c] = matrizCoeffRestrPorVar[a_posicaoVariavel][c];
					} // for (int c = 0; c < numCofPorVar; c++) {

					delete matrizIndiceRestrPorVar[a_posicaoVariavel];
					matrizIndiceRestrPorVar[a_posicaoVariavel] = linhasNovo;

					delete matrizCoeffRestrPorVar[a_posicaoVariavel];
					matrizCoeffRestrPorVar[a_posicaoVariavel] = coeficientesNovo;

				} // if (allocCof > 0) {

			} // else


			if (numCofPorRes == 0) {

				const int allocCofPorRestr = 2;

				matrizIndiceVarPorRestr[a_posicaoRestricao] = new int[allocCofPorRestr];

			} // if (numCofPorRes == 0) {


			else {

				int allocCofPorRestr = 0;

				if (numCofPorRes == 2)
					allocCofPorRestr = 5;
				else if (numCofPorRes == 5)
					allocCofPorRestr = 10;
				else if (numCofPorRes == 10)
					allocCofPorRestr = 100;
				else if (numCofPorRes == 100)
					allocCofPorRestr = 1000;
				else if (numCofPorRes == 1000)
					allocCofPorRestr = 100000;
				else if (numCofPorRes == 100000)
					allocCofPorRestr = 1000000;
				else if (numCofPorRes == 1000000)
					allocCofPorRestr = 10000000;
				else if (numCofPorRes == 10000000)
					throw invalid_argument("Numero maximo de coeficientes de restricoes atingido! 10.000.000");

				if (allocCofPorRestr > 0) {

					// Caso coeficiente é novo.
					int* matrizIndiceVarPorRestrAux = new    int[allocCofPorRestr];

					for (int c = 0; c < numCofPorRes; c++)
						matrizIndiceVarPorRestrAux[c] = matrizIndiceVarPorRestr[a_posicaoRestricao][c];

					delete matrizIndiceVarPorRestr[a_posicaoRestricao];
					matrizIndiceVarPorRestr[a_posicaoRestricao] = matrizIndiceVarPorRestrAux;

				} // if (allocCofPorRestr > 0) {

			} // else

			return true;
		}
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::alocarCoeficientes(): \n" + std::string(erro.what())); }
	} // bool alocarCoeficientes() {


	bool setCofRestricao(int a_posicaoVariavel, int a_posicaoRestricao, double a_cofRestricao) {

		try {

			if (!alocarCoeficientes(a_posicaoVariavel, a_posicaoRestricao, a_cofRestricao)) {
				isModeloRedimensionado = true;
				return true;
			}

			const int numCofPorVar = vetorNumCoefPorVar[a_posicaoVariavel];
			const int numCofPorRes = vetorNumCoefPorRestr[a_posicaoRestricao];

			matrizIndiceRestrPorVar[a_posicaoVariavel][numCofPorVar] = a_posicaoRestricao;
			matrizCoeffRestrPorVar[a_posicaoVariavel][numCofPorVar] = a_cofRestricao;

			vetorNumCoefPorVar[a_posicaoVariavel]++;

			matrizIndiceVarPorRestr[a_posicaoRestricao][numCofPorRes] = a_posicaoVariavel;

			vetorNumCoefPorRestr[a_posicaoRestricao]++;

			numCof++;

			if (a_posicaoRestricao > maiorResAdd[a_posicaoVariavel])
				maiorResAdd[a_posicaoVariavel] = a_posicaoRestricao;

			isModeloRedimensionado = true;

			return true;

		} // try {

		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::setCofRestricao(" + std::to_string(a_posicaoVariavel) + "," + std::to_string(a_posicaoRestricao) + "," + std::to_string(a_cofRestricao) + "): \n" + std::string(erro.what())); }

	}; // bool setCofRestricao(int a_posicaoVariavel, int a_posicaoRestricao, double a_cofRestricao) {


	bool setRHSRestricao(int a_posicaoRestricao, double a_RHS) {

		try {

			if (a_posicaoRestricao >= numRes)
				throw invalid_argument("Restricao nao definida no solver.");

			if (std::isnan(a_RHS))
				throw invalid_argument("RHS da restricao " + to_string(a_posicaoRestricao) + " nao numerico.");

			RHSRestricaoInf[a_posicaoRestricao] = a_RHS;

			if (sinais[a_posicaoRestricao] == '=')
				RHSRestricaoSup[a_posicaoRestricao] = a_RHS;

			else if (sinais[a_posicaoRestricao] == '>')
				RHSRestricaoSup[a_posicaoRestricao] = COIN_DBL_MAX;

			if (!isModeloRedimensionado) {
				ptrModelo->setColumnLower(a_posicaoRestricao, a_RHS);

				if (sinais[a_posicaoRestricao] == '=')
					ptrModelo->setColumnUpper(a_posicaoRestricao, a_RHS);

				else if (sinais[a_posicaoRestricao] == '>')
					ptrModelo->setColumnUpper(a_posicaoRestricao, COIN_DBL_MAX);
			}

			return true;

		} // try {

		catch (const CoinError erro) { throw std::invalid_argument("SolverCLP::setRHSRestricao(" + std::to_string(a_posicaoRestricao) + "," + std::to_string(a_RHS) + "): \n" + erro.className() + "::" + erro.methodName() + "(): \n" + erro.message()); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverCLP::setRHSRestricao(" + std::to_string(a_posicaoRestricao) + "," + std::to_string(a_RHS) + "): \n" + std::string(erro.what())); }

	}; // bool setRHSRestricao(int a_posicaoRestricao, double a_RHS) {


	double getCofRestricao(int a_posicaoVariavel, int a_posicaoRestricao) {

		try {

			if (a_posicaoVariavel >= numVar)
				throw invalid_argument("Variavel nao definida no solver.");

			if (a_posicaoRestricao >= numRes)
				throw invalid_argument("Restricao nao definida no solver.");


			const int numCofPorVar = vetorNumCoefPorVar[a_posicaoVariavel];

			if (numCofPorVar == 0)
				return 0;

			// Caso coeficiente já foi adicionado ao solver.
			for (int c = 0; c < numCofPorVar; c++) {
				if (matrizIndiceRestrPorVar[a_posicaoVariavel][c] == a_posicaoRestricao)
					return matrizCoeffRestrPorVar[a_posicaoVariavel][c];
			} // for (int c = 0; c < numCofPorVar; c++) {

			return 0.0;


		} // try {

		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::getCofRestricao(" + std::to_string(a_posicaoVariavel) + "," + std::to_string(a_posicaoRestricao) + "): \n" + std::string(erro.what())); }

	}; // double getCofRestricao(int a_posicaoVariavel, int a_posicaoRestricao) {


	double getRHSRestricao(int a_posicaoRestricao) {

		try {

			if (a_posicaoRestricao >= numRes)
				throw invalid_argument("Restricao nao definida no solver.");

			if (isModeloRedimensionado)
				return RHSRestricaoInf[a_posicaoRestricao];
			else
				return ptrModelo->columnLower()[a_posicaoRestricao];

		} // try {

		catch (const CoinError erro) { throw std::invalid_argument("SolverCLP::getRHSRestricao(" + std::to_string(a_posicaoRestricao) + "): \n" + erro.className() + "::" + erro.methodName() + "(): \n" + erro.message()); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverCLP::getRHSRestricao(" + std::to_string(a_posicaoRestricao) + "): \n" + std::string(erro.what())); }

	}; // double getRHSRestricao(int a_posicaoRestricao) {


	bool remRestricao(const int a_posicaoRestricao) {
		try {

			if (a_posicaoRestricao >= numRes)
				throw std::invalid_argument("Restricao nao instanciada.");

			if (vetorNumCoefPorRestr[a_posicaoRestricao] == 0)
				return false;

			RHSRestricaoInf[a_posicaoRestricao] = 0.0;
			RHSRestricaoSup[a_posicaoRestricao] = 0.0;

			for (int i = 0; i < vetorNumCoefPorRestr[a_posicaoRestricao]; i++) {

				const int v = matrizIndiceVarPorRestr[a_posicaoRestricao][i];

				if (maiorResAdd[v] >= a_posicaoRestricao) {

					const int numCofPorVar = vetorNumCoefPorVar[v];

					for (int c = 0; c < numCofPorVar; c++) {

						if (matrizIndiceRestrPorVar[v][c] == a_posicaoRestricao) {

							for (int c_ = c; c_ < numCofPorVar - 1; c_++) {
								matrizIndiceRestrPorVar[v][c_] = matrizIndiceRestrPorVar[v][c_ + 1];
								matrizCoeffRestrPorVar[v][c_] = matrizCoeffRestrPorVar[v][c_ + 1];
							} // for (int c_ = c; c_ < numCofPorVar - 1; c_++) {

							if (c == numCofPorVar - 1) {
								if (c == 0)
									maiorResAdd[v] = -1;
								else
									maiorResAdd[v] = matrizIndiceRestrPorVar[v][c];
							} // if (c == numCofPorVar - 1) {

							vetorNumCoefPorVar[v]--;

							numCof--;

							break;

						} // if (matrizIndiceRestrPorVar[v][c] == a_posicaoRestricao) {

					} // for (int c = 0; c < numCofPorVar; c++) {

				} // if (maiorResAdd[v] >= a_posicaoRestricao) {

			} // for (int v = 0; v < numVar; v++) {

			isModeloRedimensionado = true;

			return true;

		}
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::remRestricao(" + std::to_string(a_posicaoRestricao) + "): \n" + std::string(erro.what())); }
	}; // double getRHSRestricao(int a_posicaoRestricao) {

	bool addVarDinamica(const int a_posicaoVariavel) {
		try {
			if (numVar > a_posicaoVariavel) {
				for (int i = 0; i < lista_variavel_dinamica.size(); i++) {
					if (lista_variavel_dinamica.at(i) == a_posicaoVariavel)
						return true;
				}
				lista_variavel_dinamica.push_back(a_posicaoVariavel);
			}
			else
				throw std::invalid_argument("Variavel inexistente.");
			return true;
		}
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::addVarDinamica(" + std::to_string(a_posicaoVariavel) + "): \n" + std::string(erro.what())); }
	}; //

	double getSolucaoInferiorVarDinamica() {
		try {

			double valor_integrado_positivo = 0.0;
			double valor_integrado_negativo = 0.0;
			for (int i = 0; i < lista_variavel_dinamica.size(); i++) {
				const double reduced_cost = getReducedCost(lista_variavel_dinamica.at(i));
				if (reduced_cost > 0.0) {
					const double limite_inferior = getLimInferior(lista_variavel_dinamica.at(i));
					if (limite_inferior > 0.0)
						valor_integrado_positivo += reduced_cost * limite_inferior;
					else if (limite_inferior < 0.0)
						valor_integrado_negativo += reduced_cost * limite_inferior;
				}
				else if (reduced_cost < 0.0) {
					const double limite_superior = getLimInferior(lista_variavel_dinamica.at(i));
					if (limite_superior > 0.0)
						valor_integrado_negativo += reduced_cost * limite_superior;
					else if (limite_superior < 0.0)
						valor_integrado_positivo += reduced_cost * limite_superior;
				}
			}
			return valor_integrado_positivo + valor_integrado_negativo;
		}
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::getSolucaoInferiorVarDinamica(): \n" + std::string(erro.what())); }
	}; // double getRHSRestricao(int a_posicaoRestricao) {

	std::vector<double> getLimiteInferiorVarDinamica() {
		try {
			std::vector<double> limite_inferior(lista_variavel_dinamica.size(), 0.0);
			for (int i = 0; i < lista_variavel_dinamica.size(); i++)
				limite_inferior.at(i) = getLimInferior(lista_variavel_dinamica.at(i));
			return limite_inferior;
		}
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::getLimiteInferiorVarDinamica(): \n" + std::string(erro.what())); }
	}; // std::vector<double> getLimiteInferiorVarDinamica() {

	virtual void getLimiteInferiorVarDinamica(const int a_idx, double* a_array) {
		try {
			for (int i = 0; i < lista_variavel_dinamica.size(); i++) {
				a_array[a_idx + i] = getLimInferior(lista_variavel_dinamica[i]);
			}
			return;
		}

		catch (const CoinError& erro) {
			throw std::invalid_argument("SolverCLP::getLimiteInferiorVarDinamica(): \n" + erro.message());
		}
		catch (const std::exception& erro) {
			throw std::invalid_argument("SolverCLP::getLimiteInferiorVarDinamica(): \n" + std::string(erro.what()));
		}
	}

	virtual void  getLimiteSuperiorVarDinamica(const int a_idx, double* a_array) {
		try {
			for (int i = 0; i < lista_variavel_dinamica.size(); i++) {
				a_array[a_idx + i] = getLimSuperior(lista_variavel_dinamica[i]);
			}

			return;
		}

		catch (const CoinError& erro) {
			throw std::invalid_argument("SolverCLP::getLimiteSuperiorVarDinamica(): \n" + erro.message());
		}
		catch (const std::exception& erro) {
			throw std::invalid_argument("SolverCLP::getLimiteSuperiorVarDinamica(): \n" + std::string(erro.what()));
		}
	}

	virtual void  getReducedCostVarDinamica(const int a_idx, double* a_array) {
		try {
			for (int i = 0; i < lista_variavel_dinamica.size(); i++) {
				a_array[a_idx + i] = getReducedCost(lista_variavel_dinamica[i]);
			}

			return;
		}

		catch (const CoinError& erro) {
			throw std::invalid_argument("SolverCLP::getReducedCostVarDinamica(): \n" + erro.message());
		}
		catch (const std::exception& erro) {
			throw std::invalid_argument("SolverCLP::getReducedCostVarDinamica(): \n" + std::string(erro.what()));
		}
	}

	std::vector<double> getLimiteSuperiorVarDinamica() {
		try {
			std::vector<double> limite_superior(lista_variavel_dinamica.size(), 0.0);
			for (int i = 0; i < lista_variavel_dinamica.size(); i++)
				limite_superior.at(i) = getLimSuperior(lista_variavel_dinamica.at(i));
			return limite_superior;
		}
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::getLimiteSuperiorVarDinamica(): \n" + std::string(erro.what())); }
	}; // std::vector<double> getLimiteSuperiorVarDinamica() {

	std::vector<double> getReducedCostVarDinamica() {
		try {
			std::vector<double> reduced_cost(lista_variavel_dinamica.size(), 0.0);
			for (int i = 0; i < lista_variavel_dinamica.size(); i++)
				reduced_cost.at(i) = getReducedCost(lista_variavel_dinamica.at(i));
			return reduced_cost;
		}
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::getReducedCostVarDinamica(): \n" + std::string(erro.what())); }
	}; // std::vector<double> getReducedCostVarDinamica() {

	double getMultiplicador(int a_posicaoRestricao) {

		try {

			if (a_posicaoRestricao >= numRes)
				throw invalid_argument("Restricao nao definida no solver.");

			if (status != 0)
				return NAN;
			else
				return multiplicador[a_posicaoRestricao];

		} // try {

		catch (const std::exception& erro) { throw std::invalid_argument("SolverCLP::getMultiplicador(" + std::to_string(a_posicaoRestricao) + "): \n" + std::string(erro.what())); }

	}; // double getMultiplicador(int a_posicaoRestricao) {

	int getNumeroVarDinamica() { return int(lista_variavel_dinamica.size()); }


	char getSinalRestricao(int a_posicaoRestricao) {

		try {

			if (a_posicaoRestricao >= numRes)
				throw invalid_argument("Restricao nao definida no solver.");

			return sinais[a_posicaoRestricao];

		} // try {

		catch (const std::exception& erro) { throw std::invalid_argument("SolverCLP::getSinalRestricao(" + std::to_string(a_posicaoRestricao) + "): \n" + std::string(erro.what())); }

	}; // char getSinalRestricao(int a_posicaoRestricao) {


	double getTempoOtimizacao() {

		try {
			if (this == nullptr)
				throw std::invalid_argument("Solver nao instanciado");

			return tempo_otimizacao;
		}
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::getTempoOtimizacao(): \n" + std::string(erro.what())); }
	}; // double getMultiplicador(int a_posicaoRestricao) {

	bool otimizar() {

		try {

			resetar(isModeloRedimensionado);

			auto start_clock = std::chrono::high_resolution_clock::now();

			if (tipoMetodoSolver == TipoMetodoSolver_automatico)
				status = ptrModelo->initialSolve();
			else if (tipoMetodoSolver == TipoMetodoSolver_barrier)
				status = ptrModelo->initialBarrierNoCrossSolve();
			else if (tipoMetodoSolver == TipoMetodoSolver_primal_simplex)
				status = ptrModelo->initialPrimalSolve();
			else if (tipoMetodoSolver == TipoMetodoSolver_dual_simplex)
				status = ptrModelo->initialDualSolve();

			std::chrono::duration<double> tempo = std::chrono::high_resolution_clock::now() - start_clock;
			tempo_otimizacao = tempo.count();

			valorPrimal = ptrModelo->primalColumnSolution();

			reducedCost = ptrModelo->dualColumnSolution();

			multiplicador = ptrModelo->dualRowSolution();

			objetivo = ptrModelo->objectiveValue();

			return true;

		} // try {

		catch (const CoinError erro) { throw std::invalid_argument("SolverCLP::otimizar(): \n" + erro.className() + "::" + erro.methodName() + "(): \n" + erro.message()); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverCLP::otimizar(): \n" + std::string(erro.what())); }

	}; // bool otimizar() {

	int otimizarComTratamentoInviabilidade() {
		try {
			if (!ptrModelo) {
				throw std::invalid_argument("Solver invalido (NULO).");
			}

			try {
				resetar();
				otimizar();

				if (statusOtimizacao == TipoStatusSolver_otimalidade) {
					return 1;
				}
			}
			catch (const std::exception& erro) {
				const std::string erro_str = std::string(erro.what());
				if (erro_str.size() > 5) {
					if (erro_str.find("10005") != std::string::npos) {
						return -1;
					}
				}
			}


			return 0;
		}
		catch (const CoinError erro) {
			throw std::invalid_argument("SolverCLP::otimizarComTratamentoInviabilidade(): \n" + std::string(erro.message()));
		}
		catch (const std::exception& erro) {
			throw std::invalid_argument("SolverCLP::otimizarComTratamentoInviabilidade(): \n" + std::string(erro.what()));
		}
	}
	 
	bool resetar(bool a_resetar) {
		try {

			if (!a_resetar)
				return false;

			return resetar();

		}
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::resetar(" + getFullString(a_resetar) + "): \n" + std::string(erro.what())); }
	} // bool resetar(bool a_resetar) {


	bool resetar() {
		try {

			CoinBigIndex* indices = new CoinBigIndex[numVar + 1];

			indices[0] = 0;

			int cont = indices[0];
			for (int v = 0; v < numVar; v++) {

				cont += vetorNumCoefPorVar[v];

				indices[v + 1] = cont;

			} // for (int v = 1; v < numVar + 1; v++) {

			int* linhas = new int[numCof];
			double* coeficientes = new double[numCof];

			cont = 0;
			for (int v = 0; v < numVar; v++) {

				for (int c = 0; c < vetorNumCoefPorVar[v]; c++) {
					linhas[cont] = matrizIndiceRestrPorVar[v][c];
					coeficientes[cont] = matrizCoeffRestrPorVar[v][c];
					cont++;
				} // for (int c = 0; c < vetorNumCoefPorVar[v]; c++) {

			} // for (int v = 0; v < numVar; v++) {

			CoinPackedMatrix restricoes(true, numRes, numVar, numCof, coeficientes, linhas, indices, vetorNumCoefPorVar);

			ptrModelo->loadProblem(restricoes, limInferior, limSuperior, cofObjetivo, RHSRestricaoInf, RHSRestricaoSup);

			ptrModelo->scaling(1);

			delete indices;
			delete linhas;
			delete coeficientes;

			isModeloRedimensionado = false;

			return true;

		}
		catch (const CoinError erro) { throw std::invalid_argument("SolverCLP::resetar(): \n" + erro.className() + "::" + erro.methodName() + "(): \n" + erro.message()); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::resetar(): \n" + std::string(erro.what())); }
	}; // bool otimiza() {

	bool atualizar() {
		try {
			return true;
		}
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::atualizar(): \n" + std::string(erro.what())); }
	}; // bool otimiza() {

	bool setFocoNumerico(const int a_foco_numerico) {
		try {
			return true;
		}
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::setFocoNumerico(): \n" + std::string(erro.what())); }
	}; // bool otimiza() {

	bool setTempoLimite() {
		try {
			ptrModelo->setMaximumWallSeconds(infinitoSolver);
			return true;
		}
		catch (const CoinError erro) { throw std::invalid_argument("SolverCLP::setTempoLimite(): \n" + erro.className() + "::" + erro.methodName() + "(): \n" + erro.message()); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::setTempoLimite(): \n" + std::string(erro.what())); }
	}; // bool setTempoLimite() {

	bool setTempoLimite(const double a_tempo_limite) {
		try {
			ptrModelo->setMaximumWallSeconds(a_tempo_limite);
			return true;
		}
		catch (const CoinError erro) { throw std::invalid_argument("SolverCLP::setTempoLimite(" + getFullString(a_tempo_limite) + "): \n" + erro.className() + "::" + erro.methodName() + "(): \n" + erro.message()); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::setTempoLimite(): \n" + std::string(erro.what())); }
	}; // bool setTempoLimite() {

	double getTempoLimite() {
		try {
			return ptrModelo->maximumSeconds();
		}
		catch (const CoinError erro) { throw std::invalid_argument("SolverCLP::getTempoLimite(): \n" + erro.className() + "::" + erro.methodName() + "(): \n" + erro.message()); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::getTempoLimite(): \n" + std::string(erro.what())); }
	}; // bool setTempoLimite() {

	bool setToleranciaOtimalidade(const double a_tolerancia_otimalidade) {
		try {
			ptrModelo->setDualTolerance(a_tolerancia_otimalidade);
			return true;
		}
		catch (const CoinError erro) { throw std::invalid_argument("SolverCLP::setTempoLimite(" + getFullString(a_tolerancia_otimalidade) + "): \n" + erro.className() + "::" + erro.methodName() + "(): \n" + erro.message()); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::setToleranciaOtimalidade(): \n" + std::string(erro.what())); }
	}

	bool setToleranciaViabilidade(const double a_tolerancia_viabilidade) {
		try {
			ptrModelo->setPrimalTolerance(a_tolerancia_viabilidade);
			return true;
		}
		catch (const CoinError erro) { throw std::invalid_argument("SolverCLP::seteTolranciaViabilidade(" + getFullString(a_tolerancia_viabilidade) + "): \n" + erro.className() + "::" + erro.methodName() + "(): \n" + erro.message()); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::seteTolranciaViabilidade(): \n" + std::string(erro.what())); }
	}; // bool otimiza() {

	double getToleranciaOtimalidade() const {
		return ptrModelo->dualTolerance();
	}

	double getToleranciaViabilidade() const {
		return ptrModelo->primalTolerance();
	}

	bool setMetodoPadrao(const TipoMetodoSolver a_tipoMetodoSolver) {
		tipoMetodoSolver_padrao = a_tipoMetodoSolver;
		setMetodoPadrao();
		return true;
	}

	bool setMetodoPadrao() {
		try { return setMetodo(tipoMetodoSolver_padrao); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::setMetodoPadrao(): \n" + std::string(erro.what())); }
	}; // bool otimiza() {

	bool setMetodoAuto() {
		try { return setMetodo(TipoMetodoSolver_automatico); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::setMetodoAuto(): \n" + std::string(erro.what())); }
	}; // bool setMetodoAuto() {

	bool setMetodo(const TipoMetodoSolver a_tipoMetodoSolver) {
		try {

			if (a_tipoMetodoSolver == TipoMetodoSolver_automatico)
				setMetodoPadrao();

			else if (a_tipoMetodoSolver == TipoMetodoSolver_primal_simplex)
				ptrModelo->setSolveType(1);

			else if (a_tipoMetodoSolver == TipoMetodoSolver_dual_simplex)
				ptrModelo->setSolveType(1);

			else if (a_tipoMetodoSolver == TipoMetodoSolver_barrier) {
				ptrModelo->setSolveType(2);
			}

			else if (a_tipoMetodoSolver == TipoMetodoSolver_concorrente) 
				throw std::invalid_argument("SolverCLP nao possui metodo " + getFullString(TipoMetodoSolver_concorrente));	

			tipoMetodoSolver = a_tipoMetodoSolver;

			return true;
		}
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::setMetodoPadrao(" + getFullString(a_tipoMetodoSolver) + "): \n" + std::string(erro.what())); }
	}; // bool otimiza() {

	bool exibirNaTela(bool exibirNaTela) {

		try {

			if (exibirNaTela)
				ptrModelo->setLogLevel(1);
			else
				ptrModelo->setLogLevel(0);

			return true;

		} // try {

		catch (const CoinError erro) { throw std::invalid_argument("SolverCLP::exibirNaTela(" + getFullString(exibirNaTela) + "): \n" + erro.className() + "::" + erro.methodName() + "(): \n" + erro.message()); }
		catch (const std::exception& erro) { throw std::invalid_argument("SolverCLP::exibirNaTela(" + std::to_string(exibirNaTela) + "): \n" + std::string(erro.what())); }

	}; // bool exibirNaTela(bool exibirNaTela) {

	double getValorObjetivo() {	return objetivo; };

	TipoStatusSolver getStatusOtimizacao() {

		if (status == 0)
			return TipoStatusSolver_otimalidade;
		else if (status == 3)
			return TipoStatusSolver_tempo_excedido;

		return TipoStatusSolver_nao_otimalidade;

	}; // bool getStatusOtimizacao() {

	bool isLimiteInferiorIIS(const int a_posicaoVariavel) {
		try {
			throw std::invalid_argument("Sem calculo IIS");
			return false;
		} // try {
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::isLimiteInferiorIIS(): \n" + std::string(erro.what())); }
	}; // bool isLimiteInferiorIIS(const int a_posicaoVariavel) {

	bool isLimiteSuperiorIIS(const int a_posicaoVariavel) {
		try {
			throw std::invalid_argument("Sem calculo IIS");
			return false;
		} // try {
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::isLimiteSuperiorIIS(): \n" + std::string(erro.what())); }
	}; // bool isLimiteSuperiorIIS(const int a_posicaoVariavel) {

	bool isRestricaoIIS(const int a_posicaoRestricao) {
		try {
			throw std::invalid_argument("Sem calculo IIS");
			return false;
		} // try {
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::isRestricaoIIS(): \n" + std::string(erro.what())); }
	}; // bool isRestricaoIIS(const int a_posicaoRestricao) {


	bool imprimirLP(std::string a_nomeArquivo) {

		try {

			ptrModelo->writeMps(std::string(a_nomeArquivo + "_SN.mps").c_str(), 1);
			
			if (!isModeloRedimensionado) {
				for (int v = 0; v < numVar; v++)
					ptrModelo->setColumnName(v, nomeVariavel.at(v));

				for (int r = 0; r < numRes; r++)
					ptrModelo->setRowName(r, nomeRestricao.at(r));

				ptrModelo->writeMps(std::string(a_nomeArquivo + ".mps").c_str(), 0);
			}
			
			return true;

		} // try {

		catch (const CoinError erro) { throw std::invalid_argument("SolverCLP::imprimirLP(" + a_nomeArquivo + "): \n" + erro.className() + "::" + erro.methodName() + "(): \n" + erro.message()); }
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::imprimirLP(" + a_nomeArquivo + "): \n" + std::string(erro.what())); }

	}; // bool imprimePL(std::string a_nomeArquivo) {

	bool imprimirMPS(const std::string a_nomeArquivo) {
		try {
			ptrModelo->writeMps(a_nomeArquivo.c_str());
			return true;
		}

		catch (const CoinError& erro) {
			throw std::invalid_argument("SolverCLP::imprimirMPS(" + a_nomeArquivo + "): \n" + erro.message());
		}
		catch (const std::exception& erro) {
			throw std::invalid_argument("SolverCLP::imprimirMPS(" + a_nomeArquivo + "): \n" + std::string(erro.what()));
		}
	}

	bool imprimirILP(const std::string a_nomeArquivo) {
		try {
			if (calcularIIS() == 1) {
			}
			return false;
		} // try {
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::imprimirILP(" + a_nomeArquivo + "): \n" + std::string(erro.what())); }
	}; // bool imprimirILP(std::string a_nomeArquivo) {

	int calcularIIS() {
		try {
			return -1;
		}
		catch (const std::exception & erro) { throw std::invalid_argument("SolverCLP::calcularIIS(): \n" + std::string(erro.what())); }
	}; // bool getStatusOtimizacao() {

}; // class SolverCLP : public Solver 



#endif