#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <string>
#include <chrono>
#include <stdexcept>
#include <cassert>
#include "C_SmartEnum.h"

class Solver {
protected:
    enum class RowScaling { OFF = 0, ON = 1, DONE = 2 };

    /* parametros para escalonamento de restricoes */
    bool escalonarModelo_; // true, se o modelo original sera substituido por um modelo escalonado
    std::vector<RowScaling> escalonarRestricao_; //1, se a restricao deve ser escalonada; 0, cc.
    double menorValorCoef_; //ao realizar scaling, esse sera o menor valor possivel para um coeficiente
    double maiorValorCoef_; //ao realizar scaling, esse sera o maior valor possivel para um coeficiente

    /* dados solucao */
    TipoStatusSolver statusOtimizacao;
    double tempo_otimizacao;
    double objValue;
    double bestPossibleObjValue;
    double *dualPrice = nullptr;
    double *varX = nullptr; //solucao
    double *varRC = nullptr; //custo reduzido

public:
    //Solver() : escalonarModelo_(true), menorValorCoef_(1.0), maiorValorCoef_(1000.0) {}
    Solver() : escalonarModelo_(false), menorValorCoef_(1.0), maiorValorCoef_(1000.0) {}

    ~Solver() {
        if (varX) {
            delete[] varX;
        }

        if (varRC) {
            delete[] varRC;
        }

        if (dualPrice) {
            delete[] dualPrice;
        }
    }

    virtual bool isNomeSimplificado() = 0;

    virtual bool resetar() = 0;

    virtual int addVar(const double a_lb, const double a_ub, const double a_obj, const std::string a_nome) = 0;

    virtual bool setCofObjetivo(const int a_posicao, const double a_cofObjetivo) = 0;

    virtual bool setLimInferior(const int a_posicao, const double a_limInferior) = 0;

    virtual bool setLimSuperior(const int a_posicao, const double a_limSuperior) = 0;

    virtual bool setTipoVariavelContinua(const int a_posicao) = 0;

    virtual bool setTipoVariavelBinaria(const int a_posicao) = 0;

    virtual double getCofObjetivo(const int a_posicao) = 0;

    virtual double getLimInferior(const int a_posicao) = 0;

    virtual double getLimSuperior(const int a_posicao) = 0;

    double getValorPrimal(const int a_posicao) {
        return varX[a_posicao];
    }

    double getReducedCost(const int a_posicao) {
        return varRC[a_posicao];
    }

    virtual double getInfinito() = 0;

    virtual int addConstrIgual(const std::string a_nome, bool scale = false) = 0;

    virtual int addConstrMaior(const std::string a_nome, bool scale = false) = 0;

    virtual int addConstrMenor(const std::string a_nome, bool scale = false) = 0;

    virtual bool remRestricao(const int a_posicaoRestricao) = 0;

    virtual bool addVarDinamica(const int a_posicaoVariavel) = 0;

    virtual double getSolucaoInferiorVarDinamica() = 0;

    virtual std::vector<double> getLimiteInferiorVarDinamica() = 0;
    virtual std::vector<double> getLimiteSuperiorVarDinamica() = 0;
    virtual std::vector<double> getReducedCostVarDinamica() = 0;

    virtual void getLimiteInferiorVarDinamica(const int a_idx, double* a_array) = 0;
    virtual void  getLimiteSuperiorVarDinamica(const int a_idx, double* a_array) = 0;
    virtual void  getReducedCostVarDinamica(const int a_idx, double* a_array) = 0;

    virtual int getNumeroVarDinamica() = 0;

    virtual bool setNomeRestricao(const int a_posicaoRestricao, const std::string a_nome) = 0;

    virtual bool setCofRestricao(const int a_posicaoVariavel, const int a_posicaoRestricao, const double a_cofRestricao) = 0;

    virtual bool setRHSRestricao(const int a_posicaoRestricao, const double a_RHS) = 0;

    virtual double getCofRestricao(const int a_posicaoVariavel, const int a_posicaoRestricao) = 0;

    virtual double getRHSRestricao(const int a_posicaoRestricao) = 0;

    virtual double getMultiplicador(const int a_posicaoRestricao) = 0;

    virtual char getSinalRestricao(const int a_posicaoRestricao) = 0;

    TipoStatusSolver getStatusOtimizacao() {
        return statusOtimizacao;
    }

    double getTempoOtimizacao() {
        return tempo_otimizacao;
    }; // double getTempoOtimizacao() {

    virtual bool otimizar() = 0;

    virtual bool otimizarComScaling() = 0;

    virtual bool atualizar() = 0;

    virtual bool setMetodoPadrao(const TipoMetodoSolver a_tipoMetodoSolver) = 0;

    virtual bool setMetodoPadrao() = 0;

    virtual bool setMetodo(const TipoMetodoSolver a_tipoMetodoSolver) = 0;

    virtual bool setMetodoAuto() = 0;

    virtual bool setFocoNumerico(const int a_foco_numerico) = 0;

    virtual bool setToleranciaOtimalidade(const double a_tolerancia_otimalidade) = 0;

    virtual bool setToleranciaViabilidade(const double a_tolerancia_viabilidade) = 0;

    virtual bool setTempoLimite() = 0;

    virtual bool setTempoLimite(const double a_tempo_limite) = 0;

    virtual double getTempoLimite() = 0;

    virtual int calcularIIS() = 0;

    virtual bool isRestricaoIIS(const int a_posicaoRestricao) = 0;

    virtual bool isLimiteInferiorIIS(const int a_posicaoVariavel) = 0;

    virtual bool isLimiteSuperiorIIS(const int a_posicaoVariavel) = 0;

    virtual bool exibirNaTela(const bool exibirNaTela) = 0;

    double getValorObjetivo() {
        return objValue;
    }

    double getValorObjetivoBound() {
        return bestPossibleObjValue;
    }

    virtual bool imprimirLP(const std::string a_nomeArquivo) = 0;

    virtual bool imprimirMPS(const std::string a_nomeArquivo) = 0;

    virtual bool imprimirILP(const std::string a_nomeArquivo) = 0;

    virtual std::string str() = 0;

    bool escalonarModelo() const { return escalonarModelo_; }

    bool escalonarModelo(bool escalonar) {
        escalonarModelo_ = escalonar;
        return true;
    }

    bool escalonarRestricao(const int a_posicaoRestricao, bool escalonar) {
        try {
            escalonarRestricao_.at(a_posicaoRestricao) = escalonar ? RowScaling::ON : RowScaling::OFF;
            return true;
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("Solver::escalonarRestricao(" + std::to_string(a_posicaoRestricao) + "," +
                                        std::to_string(escalonar) + "): \n" + std::string(erro.what()));
        }
    }

    bool escalonarRestricao(const int a_posicaoRestricao) {
        try {
            RowScaling rs = escalonarRestricao_.at(a_posicaoRestricao);
            return rs == RowScaling::ON || rs == RowScaling::DONE;
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("Solver::escalonarRestricao(" + std::to_string(a_posicaoRestricao) + "): \n" +
                                        std::string(erro.what()));
        }
    }

    double getMenorValorCoef() const { return menorValorCoef_; }

    bool setMenorValorCoef(double menorValorCoef) {
        menorValorCoef_ = menorValorCoef;
        return true;
    }

    double getMaiorValorCoef() const { return maiorValorCoef_; }

    bool setMaiorValorCoef(double maiorValorCoef) {
        maiorValorCoef_ = maiorValorCoef;
        return true;
    }
}; // class Solver {


static std::string getString(Solver *a_ptrSolver) { return a_ptrSolver->str(); }


#ifdef GRB

#include "gurobi_c++.h"


class SolverGRB : public Solver {

private:

    GRBEnv *ptrAmbiente = nullptr;
    GRBModel *ptrModelo = nullptr;

    double infinitoSolver = GRB_INFINITY;

    TipoMetodoSolver tipoMetodoSolverPadrao = TipoMetodoSolver_dual_simplex;

    std::vector<int> lista_variavel_dinamica;

    std::vector<GRBVar> vetorGRBVar;
    std::vector<GRBConstr> vetorGRBConstr;

    bool inicializar() {
        try {
            if (ptrModelo != nullptr)
                delete ptrModelo;
            if (ptrAmbiente != nullptr)
                delete ptrAmbiente;

            if (dualPrice) {
                delete[] dualPrice;
                dualPrice = nullptr;
            }
            if (varX) {
                delete[] varX;
                varX = nullptr;
            }
            if (varRC) {
                delete[] varRC;
                varRC = nullptr;
            }

            ptrAmbiente = new GRBEnv();
            ptrModelo = new GRBModel(*ptrAmbiente);

            vetorGRBVar = std::vector<GRBVar>();
            vetorGRBConstr = std::vector<GRBConstr>();

            lista_variavel_dinamica = std::vector<int>();
            escalonarRestricao_.clear();

            statusOtimizacao = TipoStatusSolver_Nenhum;
            objValue = bestPossibleObjValue = GRB_INFINITY;

            tempo_otimizacao = 0.0;

            setMetodoPadrao();

            exibirNaTela(false);

            return true;
        }

        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::inicializar(): \n" + std::string(erro.getMessage()) + " " +
                                        std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::inicializar(): \n" + std::string(erro.what()));
        }


    }; // bool inicializar() {

public:

    SolverGRB() {
        try {
            inicializar();
        }
        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::SolverGRB(): \n" + std::string(erro.getMessage()) + " " +
                                        std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::SolverGRB(): \n" + std::string(erro.what()));
        }
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

    int addVar(const double a_lb, const double a_ub, const double a_obj, const std::string a_nome) {
        try {
            if (this == nullptr)
                throw std::invalid_argument("Solver nao instanciado");

            vetorGRBVar.push_back(ptrModelo->addVar(a_lb, a_ub, a_obj, GRB_CONTINUOUS, a_nome));
            return int(vetorGRBVar.size()) - 1;
        }

        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::addVar(" + std::to_string(a_lb) + "," + std::to_string(a_ub) + "," +
                                        std::to_string(a_obj) + "," + a_nome + "): \n" +
                                        std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::addVar(" + std::to_string(a_lb) + "," + std::to_string(a_ub) + "," +
                                        std::to_string(a_obj) + "," + a_nome + "): \n" + std::string(erro.what()));
        }

    }; // bool addVar(double a_lb, double a_ub, double a_obj, std::string a_nome) {


    bool setCofObjetivo(const int a_posicao, const double a_cofObjetivo) {
        try {
            vetorGRBVar.at(a_posicao).set(GRB_DoubleAttr_Obj, a_cofObjetivo);
            return true;
        }

        catch (const GRBException erro) {
            throw std::invalid_argument(
                    "SolverGRB::setLimInferior(" + std::to_string(a_posicao) + "," + std::to_string(a_cofObjetivo) +
                    "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode()));
        }

        catch (const std::exception &erro) {
            throw std::invalid_argument(
                    "SolverGRB::setLimInferior(" + std::to_string(a_posicao) + "," + std::to_string(a_cofObjetivo) +
                    "): \n" + std::string(erro.what()));
        }
    }; // bool setCofObjetivo(int a_posicao, double a_cofObjetivo) {


    bool setLimInferior(const int a_posicao, const double a_limInferior) {
        try {
            if (a_limInferior <= -infinitoSolver)
                vetorGRBVar.at(a_posicao).set(GRB_DoubleAttr_LB, -GRB_INFINITY);
            else
                vetorGRBVar.at(a_posicao).set(GRB_DoubleAttr_LB, a_limInferior);
            return true;
        }

        catch (const GRBException erro) {
            throw std::invalid_argument(
                    "SolverGRB::setLimInferior(" + std::to_string(a_posicao) + "," + std::to_string(a_limInferior) +
                    "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode()));
        }

        catch (const std::exception &erro) {
            throw std::invalid_argument(
                    "SolverGRB::setLimInferior(" + std::to_string(a_posicao) + "," + std::to_string(a_limInferior) +
                    "): \n" + std::string(erro.what()));
        }
    }; // bool setLimInferior(int a_posicao, double a_limInferior) {


    bool setLimSuperior(const int a_posicao, const double a_limSuperior) {
        try {
            if (a_limSuperior >= infinitoSolver)
                vetorGRBVar.at(a_posicao).set(GRB_DoubleAttr_UB, GRB_INFINITY);
            else
                vetorGRBVar.at(a_posicao).set(GRB_DoubleAttr_UB, a_limSuperior);
            return true;
        }

        catch (const GRBException erro) {
            throw std::invalid_argument(
                    "SolverGRB::setLimSuperior(" + std::to_string(a_posicao) + "," + std::to_string(a_limSuperior) +
                    "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode()));
        }

        catch (const std::exception &erro) {
            throw std::invalid_argument(
                    "SolverGRB::setLimSuperior(" + std::to_string(a_posicao) + "," + std::to_string(a_limSuperior) +
                    "): \n" + std::string(erro.what()));
        }
    }; // bool setLimSuperior(int a_posicao, double a_limSuperior) {


    bool setTipoVariavelContinua(const int a_posicao) {
        try {
            vetorGRBVar.at(a_posicao).set(GRB_CharAttr_VType, GRB_CONTINUOUS);
            return true;
        }

        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::setTipoVariavelContinua(" + std::to_string(a_posicao) + "): \n" +
                                        std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode()));
        }

        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::setTipoVariavelContinua(" + std::to_string(a_posicao) + "): \n" +
                                        std::string(erro.what()));
        }
    }; // bool setTipoVariavelContinua(int a_posicao) {


    bool setTipoVariavelBinaria(const int a_posicao) {
        try {
            vetorGRBVar.at(a_posicao).set(GRB_CharAttr_VType, GRB_BINARY);
            return true;
        }

        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::setTipoVariavelBinaria(" + std::to_string(a_posicao) + "): \n" +
                                        std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode()));
        }

        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::setTipoVariavelBinaria(" + std::to_string(a_posicao) + "): \n" +
                                        std::string(erro.what()));
        }
    }; // bool setTipoVariavelBinaria(int a_posicao) {


    double getInfinito() {
        try {
            if (this == nullptr)
                throw std::invalid_argument("Solver nao instanciado");

            return infinitoSolver;
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::getInfinito(): \n" + std::string(erro.what()));
        }
    };


    double getCofObjetivo(const int a_posicao) {
        try { return vetorGRBVar.at(a_posicao).get(GRB_DoubleAttr_Obj); }

        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::getCofObjetivo(" + std::to_string(a_posicao) + "): \n" +
                                        std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument(
                    "SolverGRB::getCofObjetivo(" + std::to_string(a_posicao) + "): \n" + std::string(erro.what()));
        }
    }; // double getCofObjetivo(int a_posicao) {


    double getLimInferior(const int a_posicao) {
        try { return vetorGRBVar.at(a_posicao).get(GRB_DoubleAttr_LB); }

        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::getLimInferior(" + std::to_string(a_posicao) + "): \n" +
                                        std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument(
                    "SolverGRB::getLimInferior(" + std::to_string(a_posicao) + "): \n" + std::string(erro.what()));
        }
    }; // double getLimInferior(int a_posicao) {


    double getLimSuperior(const int a_posicao) {
        try { return vetorGRBVar.at(a_posicao).get(GRB_DoubleAttr_UB); }

        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::getLimSuperior(" + std::to_string(a_posicao) + "): \n" +
                                        std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument(
                    "SolverGRB::getLimSuperior(" + std::to_string(a_posicao) + "): \n" + std::string(erro.what()));
        }
    }; // double getLimSuperior(int a_posicao) {

    int addConstrIgual(const std::string a_nome, bool scale = false) {
        try {
            vetorGRBConstr.push_back(ptrModelo->addConstr(GRBLinExpr(0), GRB_EQUAL, 0.0, a_nome));
            escalonarRestricao_.push_back(scale ? RowScaling::ON : RowScaling::OFF);
            return int(vetorGRBConstr.size()) - 1;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument(
                    "SolverGRB::addConstrIgual(" + a_nome + "): \n" + std::string(erro.getMessage()) + " " +
                    std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::addConstrIgual(" + a_nome + "): \n" + std::string(erro.what()));
        }
    }; // bool addConstrIgual(std::string a_nome) {


    int addConstrMaior(const std::string a_nome, bool scale = false) {
        try {
            vetorGRBConstr.push_back(ptrModelo->addConstr(GRBLinExpr(0), GRB_GREATER_EQUAL, 0.0, a_nome));
            escalonarRestricao_.push_back(scale ? RowScaling::ON : RowScaling::OFF);
            return int(vetorGRBConstr.size()) - 1;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument(
                    "SolverGRB::addConstrMaior(" + a_nome + "): \n" + std::string(erro.getMessage()) + " " +
                    std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::addConstrMaior(" + a_nome + "): \n" + std::string(erro.what()));
        }
    }; // bool addConstrMaior(std::string a_nome) {

    int addConstrMenor(const std::string a_nome, bool scale = false) {
        try {
            vetorGRBConstr.push_back(ptrModelo->addConstr(GRBLinExpr(0), GRB_LESS_EQUAL, 0.0, a_nome));
            escalonarRestricao_.push_back(scale ? RowScaling::ON : RowScaling::OFF);
            return int(vetorGRBConstr.size()) - 1;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument(
                    "SolverGRB::addConstrMenor(" + a_nome + "): \n" + std::string(erro.getMessage()) + " " +
                    std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::addConstrMenor(" + a_nome + "): \n" + std::string(erro.what()));
        }
    };

    bool setNomeRestricao(const int a_posicaoRestricao, const std::string a_nome) {
        try {
            ptrModelo->set(GRB_StringAttr_ConstrName, &vetorGRBConstr.at(a_posicaoRestricao), &a_nome, 1);
            return true;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument(
                    "SolverGRB::setNomeRestricao(" + std::to_string(a_posicaoRestricao) + "," + a_nome + "): \n" +
                    std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument(
                    "SolverGRB::setNomeRestricao(" + std::to_string(a_posicaoRestricao) + "," + a_nome + "): \n" +
                    std::string(erro.what()));
        }
    } // bool setNomeRestricao(const int a_posicaoRestricao, const std::string a_nome) {

    bool setCofRestricao(const int a_posicaoVariavel, const int a_posicaoRestricao, const double a_cofRestricao) {
        try {
            ptrModelo->chgCoeff(vetorGRBConstr.at(a_posicaoRestricao), vetorGRBVar.at(a_posicaoVariavel),
                                a_cofRestricao);
            return true;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::setCofRestricao(" + std::to_string(a_posicaoVariavel) + "," +
                                        std::to_string(a_posicaoRestricao) + "," + std::to_string(a_cofRestricao) +
                                        "): \n" + std::string(erro.getMessage()) + " " +
                                        std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::setCofRestricao(" + std::to_string(a_posicaoVariavel) + "," +
                                        std::to_string(a_posicaoRestricao) + "," + std::to_string(a_cofRestricao) +
                                        "): \n" + std::string(erro.what()));
        }
    }; // bool setCofRestricao(int a_posicaoVariavel, int a_posicaoRestricao, double a_cofRestricao) {


    bool setRHSRestricao(const int a_posicaoRestricao, const double a_RHS) {
        try {
            vetorGRBConstr.at(a_posicaoRestricao).set(GRB_DoubleAttr_RHS, a_RHS);
            return true;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument(
                    "SolverGRB::setRHSRestricao(" + std::to_string(a_posicaoRestricao) + "," + std::to_string(a_RHS) +
                    "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument(
                    "SolverGRB::setRHSRestricao(" + std::to_string(a_posicaoRestricao) + "," + std::to_string(a_RHS) +
                    "): \n" + std::string(erro.what()));
        }
    }; // bool setRHSRestricao(int a_posicaoRestricao, double a_RHS) {


    double getCofRestricao(const int a_posicaoVariavel, const int a_posicaoRestricao) {
        try { return ptrModelo->getCoeff(vetorGRBConstr.at(a_posicaoRestricao), vetorGRBVar.at(a_posicaoVariavel)); }
        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::getCofRestricao(" + std::to_string(a_posicaoVariavel) + "," +
                                        std::to_string(a_posicaoRestricao) + "): \n" + std::string(erro.getMessage()) +
                                        " " + std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::getCofRestricao(" + std::to_string(a_posicaoVariavel) + "," +
                                        std::to_string(a_posicaoRestricao) + "): \n" + std::string(erro.what()));
        }
    }; // double getCofRestricao(int a_posicaoVariavel, int a_posicaoRestricao) {


    double getRHSRestricao(const int a_posicaoRestricao) {
        try { return vetorGRBConstr.at(a_posicaoRestricao).get(GRB_DoubleAttr_RHS); }
        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::getRHSRestricao(" + std::to_string(a_posicaoRestricao) + "): \n" +
                                        std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::getRHSRestricao(" + std::to_string(a_posicaoRestricao) + "): \n" +
                                        std::string(erro.what()));
        }
    }; // double getRHSRestricao(int a_posicaoRestricao) {

    bool remRestricao(const int a_posicaoRestricao) {
        try {
            ptrModelo->remove(vetorGRBConstr.at(a_posicaoRestricao));
            escalonarRestricao_.at(a_posicaoRestricao) = RowScaling::OFF;
            return true;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::remRestricao(" + std::to_string(a_posicaoRestricao) + "): \n" +
                                        std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::remRestricao(" + std::to_string(a_posicaoRestricao) + "): \n" +
                                        std::string(erro.what()));
        }
    }; // double getRHSRestricao(int a_posicaoRestricao) {

    bool addVarDinamica(const int a_posicaoVariavel) {
        try {
            if (vetorGRBVar.size() > a_posicaoVariavel) {
                for (int i = 0; i < lista_variavel_dinamica.size(); i++) {
                    if (lista_variavel_dinamica.at(i) == a_posicaoVariavel)
                        return true;
                }
                lista_variavel_dinamica.push_back(a_posicaoVariavel);
            } else
                throw std::invalid_argument("Variavel inexistente.");
            return true;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::addVarDinamica(" + std::to_string(a_posicaoVariavel) + "): \n" +
                                        std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::addVarDinamica(" + std::to_string(a_posicaoVariavel) + "): \n" +
                                        std::string(erro.what()));
        }
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
                } else if (reduced_cost < 0.0) {
                    const double limite_superior = getLimInferior(lista_variavel_dinamica.at(i));
                    if (limite_superior > 0.0)
                        valor_integrado_negativo += reduced_cost * limite_superior;
                    else if (limite_superior < 0.0)
                        valor_integrado_positivo += reduced_cost * limite_superior;
                }
            }
            return valor_integrado_positivo + valor_integrado_negativo;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument(
                    "SolverGRB::getSolucaoInferiorVarDinamica(): \n" + std::string(erro.getMessage()) + " " +
                    std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::getSolucaoInferiorVarDinamica(): \n" + std::string(erro.what()));
        }
    }; // double getRHSRestricao(int a_posicaoRestricao) {

    std::vector<double> getLimiteInferiorVarDinamica() {
        try {
            std::vector<double> limite_inferior(lista_variavel_dinamica.size(), 0.0);
            for (int i = 0; i < lista_variavel_dinamica.size(); i++)
                limite_inferior.at(i) = getLimInferior(lista_variavel_dinamica.at(i));
            return limite_inferior;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument(
                    "SolverGRB::getLimiteInferiorVarDinamica(): \n" + std::string(erro.getMessage()) + " " +
                    std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::getLimiteInferiorVarDinamica(): \n" + std::string(erro.what()));
        }
    }; // std::vector<double> getLimiteInferiorVarDinamica() {

    std::vector<double> getLimiteSuperiorVarDinamica() {
        try {
            std::vector<double> limite_superior(lista_variavel_dinamica.size(), 0.0);
            for (int i = 0; i < lista_variavel_dinamica.size(); i++)
                limite_superior.at(i) = getLimSuperior(lista_variavel_dinamica.at(i));
            return limite_superior;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument(
                    "SolverGRB::getLimiteSuperiorVarDinamica(): \n" + std::string(erro.getMessage()) + " " +
                    std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::getLimiteSuperiorVarDinamica(): \n" + std::string(erro.what()));
        }
    }; // std::vector<double> getLimiteSuperiorVarDinamica() {

    std::vector<double> getReducedCostVarDinamica() {
        try {
            std::vector<double> reduced_cost(lista_variavel_dinamica.size(), 0.0);
            for (int i = 0; i < lista_variavel_dinamica.size(); i++)
                reduced_cost.at(i) = getReducedCost(lista_variavel_dinamica.at(i));
            return reduced_cost;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument(
                    "SolverGRB::getReducedCostVarDinamica(): \n" + std::string(erro.getMessage()) + " " +
                    std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::getReducedCostVarDinamica(): \n" + std::string(erro.what()));
        }
    }; // std::vector<double> getReducedCostVarDinamica() {

    void  getLimiteSuperiorVarDinamica(const int a_idx, double* a_array) {
        try {
            for (int i = 0; i < lista_variavel_dinamica.size(); i++)
                a_array[a_idx + i] = getLimSuperior(lista_variavel_dinamica.at(i));
            return;
        }
        catch (const GRBException erro) { throw std::invalid_argument( "SolverGRB::getLimiteSuperiorVarDinamica(): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode())); }
        catch (const std::exception& erro) { throw std::invalid_argument("SolverGRB::getLimiteSuperiorVarDinamica(): \n" + std::string(erro.what()));  }
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

    int getNumeroVarDinamica() { return int(lista_variavel_dinamica.size()); }

    double getMultiplicador(const int a_posicaoRestricao) {
        try {
            const int rowIdx = vetorGRBConstr.at(a_posicaoRestricao).index();

            if (rowIdx < 0) {
                throw std::invalid_argument(
                        "Restricao de indice " + std::to_string(a_posicaoRestricao) + " foi removida do modelo.");
            }

            assert(rowIdx < ptrModelo->get(GRB_IntAttr_NumConstrs));
            return dualPrice[rowIdx];
        }
        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::getMultiplicador(" + std::to_string(a_posicaoRestricao) + "): \n" +
                                        std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::getMultiplicador(" + std::to_string(a_posicaoRestricao) + "): \n" +
                                        std::string(erro.what()));
        }
    }; // double getMultiplicador(int a_posicaoRestricao) {


    char getSinalRestricao(const int a_posicaoRestricao) {
        try { return vetorGRBConstr.at(a_posicaoRestricao).get(GRB_CharAttr_Sense); }
        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::getSinalRestricao(" + std::to_string(a_posicaoRestricao) + "): \n" +
                                        std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::getSinalRestricao(" + std::to_string(a_posicaoRestricao) + "): \n" +
                                        std::string(erro.what()));
        }
    }; // char getSinalRestricao(int a_posicaoRestricao) {

    void fillSolution(GRBModel *model) {
        const int status = model->get(GRB_IntAttr_Status);
        const int numVars = model->get(GRB_IntAttr_NumVars);
        const int numRows = model->get(GRB_IntAttr_NumConstrs);

        if (status == GRB_OPTIMAL) {
            statusOtimizacao = TipoStatusSolver_otimalidade;
            objValue = model->get(GRB_DoubleAttr_ObjVal);
            bestPossibleObjValue = model->get(GRB_DoubleAttr_ObjBound);

            if (model->get(GRB_IntAttr_IsMIP) == 1) {
                for (int j = 0; j < numVars; j++) {
                    varX[j] = model->getVar(j).get(GRB_DoubleAttr_X);
                    varRC[j] = 0.0;
                }
                std::fill(dualPrice, dualPrice + numRows, 0.0);
            } else {
                for (int j = 0; j < numVars; j++) {
                    varX[j] = model->getVar(j).get(GRB_DoubleAttr_X);
                    varRC[j] = model->getVar(j).get(GRB_DoubleAttr_RC);
                }
                for (int i = 0; i < numRows; i++) {
                    dualPrice[i] = model->getConstr(i).get(GRB_DoubleAttr_Pi);
                }
            }
        } else if (status == GRB_TIME_LIMIT) {
            statusOtimizacao = TipoStatusSolver_tempo_excedido;
            objValue = model->get(GRB_DoubleAttr_ObjVal);
            bestPossibleObjValue = model->get(GRB_DoubleAttr_ObjBound);

            if (model->get(GRB_IntAttr_IsMIP) == 1) {
                for (int j = 0; j < numVars; j++) {
                    varX[j] = model->getVar(j).get(GRB_DoubleAttr_X);
                    varRC[j] = 0.0;
                }
                std::fill(dualPrice, dualPrice + numRows, 0.0);
            } else {
                for (int j = 0; j < numVars; j++) {
                    varX[j] = model->getVar(j).get(GRB_DoubleAttr_X);
                    varRC[j] = model->getVar(j).get(GRB_DoubleAttr_RC);
                }
                for (int i = 0; i < numRows; i++) {
                    dualPrice[i] = model->getConstr(i).get(GRB_DoubleAttr_Pi);
                }
            }
        } else {
            statusOtimizacao = TipoStatusSolver_nao_otimalidade;
            objValue = bestPossibleObjValue = GRB_INFINITY;
            std::fill(varX, varX + model->get(GRB_IntAttr_NumVars), NAN);
            std::fill(varRC, varRC + model->get(GRB_IntAttr_NumVars), NAN);
            std::fill(dualPrice, dualPrice + model->get(GRB_IntAttr_NumConstrs), NAN);
        }
    }

    bool otimizar(GRBModel *model) {
        try {
            auto start_clock = std::chrono::high_resolution_clock::now();
            model->update();

            if (varX) {
                delete[] varX;
            }
            if (varRC) {
                delete[] varRC;
            }
            if (dualPrice) {
                delete[] dualPrice;
            }

            varX = new double[model->get(GRB_IntAttr_NumVars)];
            varRC = new double[model->get(GRB_IntAttr_NumVars)];
            dualPrice = new double[model->get(GRB_IntAttr_NumConstrs)];
            model->optimize();
            fillSolution(model);
            std::chrono::duration<double> tempo = std::chrono::high_resolution_clock::now() - start_clock;
            tempo_otimizacao = tempo.count();

            return true;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument(
                    "SolverGRB::otimiza(GRBModel *model): \n" + std::string(erro.getMessage()) + " " +
                    std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::otimiza(GRBModel *model): \n" + std::string(erro.what()));
        }
    }; // bool otimizar() {

    void escalonarRestricoes() {
        ptrModelo->update();

        const int numRows = ptrModelo->get(GRB_IntAttr_NumConstrs);
        const int numVars = ptrModelo->get(GRB_IntAttr_NumVars);
        int *rowsToScale = new int[numRows];
        int nRowsToScale = 0;

        assert(escalonarRestricao_.size() == vetorGRBConstr.size());

        for (size_t i = 0; i < vetorGRBConstr.size(); i++) {
            const int rowIdx = vetorGRBConstr[i].index();

            if (rowIdx < 0) { //restricao removida
                continue;
            }

            assert(rowIdx < numRows);

            if (escalonarRestricao_.at(i) == RowScaling::ON) {
                rowsToScale[nRowsToScale++] = rowIdx;
                escalonarRestricao_.at(i) = RowScaling::DONE;
            }
        }

        if (!nRowsToScale) {
            delete[] rowsToScale;
            return;
        }

        const double newMinCoef = getMenorValorCoef();
        const double newMaxCoef = getMaiorValorCoef();
        GRBConstr *rows = ptrModelo->getConstrs();
        GRBVar *vars = ptrModelo->getVars();
        GRBVar *idxVarsRow = new GRBVar[numVars];
        double *coefsRow = new double[numVars];

        for (int i = 0; i < nRowsToScale; i++) {
            const int idxRow = rowsToScale[i];
            const double rhs = rows[idxRow].get(GRB_DoubleAttr_RHS);
            double minCoef = rhs;
            double maxCoef = rhs;
            int nz = 0;

            for (size_t j = 0; j < numVars; j++) {
                const double varCoef = ptrModelo->getCoeff(rows[idxRow], vars[j]);

                if (varCoef == 0.0) {
                    continue;
                }

                minCoef = (std::min)(minCoef, varCoef);
                maxCoef = (std::max)(maxCoef, varCoef);
                idxVarsRow[nz] = vars[j];
                coefsRow[nz] = varCoef;
                nz++;
            }

            if (minCoef >= newMinCoef && maxCoef <= newMaxCoef) { //nao eh necessario escalonar
                continue;
            } else if (fabs(maxCoef - minCoef) <= 1e-6) { //todos os valores de coefs e rhs sao iguais
                for (int j = 0; j < nz; j++) {
                    ptrModelo->chgCoeff(rows[idxRow], idxVarsRow[j], 1.0);
                }
                rows[idxRow].set(GRB_DoubleAttr_RHS, 1.0);
            } else { //faz scaling
                for (int j = 0; j < nz; j++) {
                    const double newCoef =
                            (((coefsRow[j] - minCoef) / (maxCoef - minCoef)) * (newMaxCoef - newMinCoef)) +
                            newMinCoef;
                    ptrModelo->chgCoeff(rows[idxRow], idxVarsRow[j], newCoef);
                }
                const double newRHS =
                        (((rhs - minCoef) / (maxCoef - minCoef)) * (newMaxCoef - newMinCoef)) + newMinCoef;
                rows[idxRow].set(GRB_DoubleAttr_RHS, newRHS);
            }
        }

        delete[] rowsToScale;
        delete[] idxVarsRow;
        delete[] coefsRow;
        delete[] rows;
        delete[] vars;
    }

    bool otimizar() {
        if (escalonarModelo_) {
            escalonarRestricoes();
        }

        return otimizar(ptrModelo);
    }

    bool resetar() {
        try {
            ptrModelo->reset(0);
            tempo_otimizacao = 0.0;
            statusOtimizacao = TipoStatusSolver_Nenhum;
            return true;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::resetar(): \n" + std::string(erro.getMessage()) + " " +
                                        std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::resetar(): \n" + std::string(erro.what()));
        }
    }; // bool otimiza() {

    bool atualizar() {
        try {
            ptrModelo->update();
            return true;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::atualizar(): \n" + std::string(erro.getMessage()) + " " +
                                        std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::atualizar(): \n" + std::string(erro.what()));
        }
    }; // bool otimiza() {

    bool setFocoNumerico(const int a_foco_numerico) {
        try {
            ptrModelo->set(GRB_IntParam_NumericFocus, a_foco_numerico);
            return true;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::setFocoNumerico(): \n" + std::string(erro.getMessage()) + " " +
                                        std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::setFocoNumerico(): \n" + std::string(erro.what()));
        }
    }; // bool otimiza() {

    bool setTempoLimite() {
        try {
            ptrModelo->set(GRB_DoubleParam_TimeLimit, infinitoSolver);
            return true;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::setTempoLimite(): \n" + std::string(erro.getMessage()) + " " +
                                        std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::setTempoLimite(): \n" + std::string(erro.what()));
        }
    }; // bool setTempoLimite() {

    bool setTempoLimite(const double a_tempo_limite) {
        try {
            ptrModelo->set(GRB_DoubleParam_TimeLimit, a_tempo_limite);
            return true;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::setTempoLimite(): \n" + std::string(erro.getMessage()) + " " +
                                        std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::setTempoLimite(): \n" + std::string(erro.what()));
        }
    }; // bool setTempoLimite() {

    double getTempoLimite() {
        try {
            return ptrModelo->get(GRB_DoubleParam_TimeLimit);
        }
        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::getTempoLimite(): \n" + std::string(erro.getMessage()) + " " +
                                        std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::getTempoLimite(): \n" + std::string(erro.what()));
        }
    }; // bool setTempoLimite() {

    bool setToleranciaOtimalidade(const double a_tolerancia_otimalidade) {
        try {
            ptrModelo->set(GRB_DoubleParam_OptimalityTol, a_tolerancia_otimalidade);
            return true;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument(
                    "SolverGRB::setToleranciaOtimalidade(): \n" + std::string(erro.getMessage()) + " " +
                    std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::setToleranciaOtimalidade(): \n" + std::string(erro.what()));
        }
    }; // bool otimiza() {

    bool setToleranciaViabilidade(const double a_tolerancia_viabilidade) {
        try {
            ptrModelo->set(GRB_DoubleParam_FeasibilityTol, a_tolerancia_viabilidade);
            return true;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument(
                    "SolverGRB::setToleranciaViabilidade(): \n" + std::string(erro.getMessage()) + " " +
                    std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::seteTolranciaViabilidade(): \n" + std::string(erro.what()));
        }
    }; // bool otimiza() {

    bool setMetodoPadrao() {
        try {
            setMetodo(tipoMetodoSolverPadrao);
            return true;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::setMetodoPadrao(): \n" + std::string(erro.getMessage()) + " " +
                                        std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::setMetodoPadrao(): \n" + std::string(erro.what()));
        }
    }; // bool setMetodoPadrao() {

    bool setMetodoAuto() {
        try { return setMetodo(TipoMetodoSolver_automatico); }
        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::setMetodoAuto(): \n" + std::string(erro.getMessage()) + " " +
                                        std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::setMetodoAuto(): \n" + std::string(erro.what()));
        }
    }; // bool setMetodoAuto() {

    bool setMetodoPadrao(const TipoMetodoSolver a_tipoMetodoSolverPadrao) {
        try {

            tipoMetodoSolverPadrao = a_tipoMetodoSolverPadrao;

            setMetodoPadrao();

            return true;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::setMetodoPadrao(): \n" + std::string(erro.getMessage()) + " " +
                                        std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::setMetodoPadrao(): \n" + std::string(erro.what()));
        }
    }; // bool otimiza() {

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
            } else if (a_tipoMetodoSolver == TipoMetodoSolver_concorrente) {
                ptrModelo->set(GRB_IntParam_Method, GRB_METHOD_CONCURRENT);
                ptrModelo->set(GRB_IntParam_Crossover, 0);
            } else
                throw std::invalid_argument("Metodo solver " + getFullString(a_tipoMetodoSolver) + " invalido");

            return true;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::setMetodoPadrao(): \n" + std::string(erro.getMessage()) + " " +
                                        std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::setMetodoPadrao(): \n" + std::string(erro.what()));
        }
    }; // bool otimiza() {

    bool isLimiteInferiorIIS(const int a_posicaoVariavel) {
        try {
            if (vetorGRBVar.at(a_posicaoVariavel).get(GRB_IntAttr_IISLB) > 0)
                return true;
            return false;
        } // try {
        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::isLimiteInferiorIIS(): \n" + std::string(erro.getMessage()) + " " +
                                        std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::isLimiteInferiorIIS(): \n" + std::string(erro.what()));
        }
    }; // bool isLimiteInferiorIIS(const int a_posicaoVariavel) {

    bool isLimiteSuperiorIIS(const int a_posicaoVariavel) {
        try {
            if (vetorGRBVar.at(a_posicaoVariavel).get(GRB_IntAttr_IISUB) > 0)
                return true;
            return false;
        } // try {
        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::isLimiteSuperiorIIS(): \n" + std::string(erro.getMessage()) + " " +
                                        std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::isLimiteSuperiorIIS(): \n" + std::string(erro.what()));
        }
    }; // bool isLimiteSuperiorIIS(const int a_posicaoVariavel) {

    bool isRestricaoIIS(const int a_posicaoRestricao) {
        try {
            if (vetorGRBConstr.at(a_posicaoRestricao).get(GRB_IntAttr_IISConstr) > 0)
                return true;
            return false;
        } // try {
        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::isRestricaoIIS(): \n" + std::string(erro.getMessage()) + " " +
                                        std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::isRestricaoIIS(): \n" + std::string(erro.what()));
        }
    }; // bool isRestricaoIIS(const int a_posicaoRestricao) {

    bool imprimirLP(const std::string a_nomeArquivo) {
        try {
            ptrModelo->write(std::string(a_nomeArquivo + ".lp"));
            return true;
        } // try {
        catch (const GRBException erro) {
            throw std::invalid_argument(
                    "SolverGRB::imprimirLP(" + a_nomeArquivo + "): \n" + std::string(erro.getMessage()) + " " +
                    std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::imprimirLP(" + a_nomeArquivo + "): \n" + std::string(erro.what()));
        }
    }; // bool imprimirLP(std::string a_nomeArquivo) {

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
        catch (const std::exception &erro) {
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
        catch (const GRBException erro) {
            throw std::invalid_argument(
                    "SolverGRB::imprimirILP(" + a_nomeArquivo + "): \n" + std::string(erro.getMessage()) + " " +
                    std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::imprimirILP(" + a_nomeArquivo + "): \n" + std::string(erro.what()));
        }
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
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::calcularIIS(): \n" + std::string(erro.what()));
        }
    }; // int calcularIIS() {

    bool exibirNaTela(const bool exibirNaTela) {
        try {
            if (exibirNaTela)
                ptrModelo->set(GRB_IntParam_OutputFlag, 1);
            else
                ptrModelo->set(GRB_IntParam_OutputFlag, 0);
            return true;
        } // try {
        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::exibirNaTela(" + std::to_string(exibirNaTela) + "): \n" +
                                        std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument(
                    "SolverGRB::exibirNaTela(" + std::to_string(exibirNaTela) + "): \n" + std::string(erro.what()));
        }
    }; // bool exibirNaTela(bool exibirNaTela) {

    bool otimizarComScaling() {
        try {
            if (escalonarModelo_) { //nao faz nada, modelo ja esta escalonado
                return false;
            }

            ptrModelo->update();

            GRBModel *modeloEscalonado = new GRBModel(*ptrModelo);
            int *rowsToScale = new int[modeloEscalonado->get(GRB_IntAttr_NumConstrs)];
            int nRowsToScale = 0;

            assert(escalonarRestricao_.size() == vetorGRBConstr.size());

            for (size_t i = 0; i < vetorGRBConstr.size(); i++) {
                const int rowIdx = vetorGRBConstr[i].index();

                if (rowIdx < 0) { //restricao removida
                    continue;
                }

                assert(rowIdx < modeloEscalonado->get(GRB_IntAttr_NumConstrs));
                assert(ptrModelo->getConstr(rowIdx).get(GRB_StringAttr_ConstrName) ==
                       modeloEscalonado->getConstr(rowIdx).get(GRB_StringAttr_ConstrName));
                assert(ptrModelo->getConstr(rowIdx).index() == modeloEscalonado->getConstr(rowIdx).index());

                if (escalonarRestricao_.at(i) == RowScaling::ON) {
                    rowsToScale[nRowsToScale++] = rowIdx;
                }
            }

            if (!nRowsToScale) {
                delete[] rowsToScale;
                return true;
            }

            const double newMinCoef = getMenorValorCoef();
            const double newMaxCoef = getMaiorValorCoef();
            const int numRows = modeloEscalonado->get(GRB_IntAttr_NumConstrs);
            const int numVars = modeloEscalonado->get(GRB_IntAttr_NumVars);
            GRBConstr *rows = modeloEscalonado->getConstrs();
            GRBVar *vars = modeloEscalonado->getVars();
            GRBVar *idxVarsRow = new GRBVar[numVars];
            double *coefsRow = new double[numVars];

            for (int i = 0; i < nRowsToScale; i++) {
                const int idxRow = rowsToScale[i];
                const double rhs = rows[idxRow].get(GRB_DoubleAttr_RHS);
                double minCoef = rhs;
                double maxCoef = rhs;
                int nz = 0;

                for (size_t j = 0; j < numVars; j++) {
                    const double varCoef = modeloEscalonado->getCoeff(rows[idxRow], vars[j]);

                    if (varCoef == 0.0) {
                        continue;
                    }

                    minCoef = (std::min)(minCoef, varCoef);
                    maxCoef = (std::max)(maxCoef, varCoef);
                    idxVarsRow[nz] = vars[j];
                    coefsRow[nz] = varCoef;
                    nz++;
                }

                if (minCoef >= newMinCoef && maxCoef <= newMaxCoef) { //nao eh necessario escalonar
                    continue;
                } else if (fabs(maxCoef - minCoef) <= 1e-6) { //todos os valores de coefs e rhs sao iguais
                    for (int j = 0; j < nz; j++) {
                        modeloEscalonado->chgCoeff(rows[idxRow], idxVarsRow[j], 1.0);
                    }
                    rows[idxRow].set(GRB_DoubleAttr_RHS, 1.0);
                } else { //faz scaling
                    for (int j = 0; j < nz; j++) {
                        const double newCoef =
                                (((coefsRow[j] - minCoef) / (maxCoef - minCoef)) * (newMaxCoef - newMinCoef)) +
                                newMinCoef;
                        modeloEscalonado->chgCoeff(rows[idxRow], idxVarsRow[j], newCoef);
                    }
                    const double newRHS =
                            (((rhs - minCoef) / (maxCoef - minCoef)) * (newMaxCoef - newMinCoef)) + newMinCoef;
                    rows[idxRow].set(GRB_DoubleAttr_RHS, newRHS);
                }
            }

            delete[] rowsToScale;
            delete[] idxVarsRow;
            delete[] coefsRow;
            delete[] rows;
            delete[] vars;

            otimizar(modeloEscalonado);

            delete modeloEscalonado;

            return true;
        }

        catch (const GRBException &erro) {
            throw std::invalid_argument("SolverGRB::otimizarComScaling(): \n" + std::string(erro.getMessage()) + " " +
                                        std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::otimizarComScaling(): \n" + std::string(erro.what()));
        }
    }

}; // class SolverGRB : public Solver {


#endif

#include "ClpSimplex.hpp"
#include "ClpSimplexOther.hpp"
#include "ClpDualRowSteepest.hpp"
#include "ClpDualRowDantzig.hpp"
#include "ClpPEDualRowSteepest.hpp"
#include "OsiClpSolverInterface.hpp"
#include "CbcModel.hpp"
#include "CbcSolver.hpp"
#include <algorithm>

#define CAP_INI_CONSTR 512     //capacidade inicial para inserir restricoes
#define CAP_INI_POR_CONSTR 256 //capacidade inicial para inserir idxs e coefs em cada restricao

enum DualPivot {
    DP_Auto = 0,
    DP_Dantzig = 1,
    DP_Steepest = 2,
    DP_Partial = 3,
    DP_PESteepest = 4
};

static void *xmalloc(const size_t size);

static void *xrealloc(void *ptr, const size_t size);

class SolverCLP : public Solver {
private:
    /* solver */
    ClpSimplex *solver = nullptr;
    bool isMILP = false;

    int fakeNumRows;
    int realNumRows;
    std::vector<int> origRowIdx;
    std::vector<char> origRowSense;

    /* buffer de restricoes */
    int **matrizIdxs = nullptr; //matriz de restricoes, contendo os indices das vars
    double **matrizCoefs = nullptr; //matriz de restricoes, contendo os coeficientes das vars
    char **nomeRow = nullptr; //nomes das restricoes
    int *capRow = nullptr; //capacidade de cada restricao
    int *sizeRow = nullptr; //qtde de entradas em cada restricao
    double *rhsRow = nullptr; //lado direito de cada restricao
    char *sinalRow = nullptr; // < (<=), = (=) ou > (>=)
    int *fakeIdxRow = nullptr; //indice da restricao (nao considera remocao)
    int capBufferRows = 0; //capcidade de restricoes
    int sizeBufferRows = 0; //numero atual de restricoes; nao considera remocao

    /* parametros do solver */
    double infinitoSolver = COIN_DBL_MAX;
    double tempoLimite = COIN_DBL_MAX;
    double toleranciaOtimalidade = 1e-6;
    double toleranciaViabilidade = 1e-6;
    TipoMetodoSolver tipoMetodoSolver;
    TipoMetodoSolver tipoMetodoSolverPadrao = TipoMetodoSolver_dual_simplex;
    int verbose;

    /* lista de variaveis dinamicas */
    std::vector<int> lista_variavel_dinamica;

    void inicializaMatriz() {
        sizeBufferRows = 0;
        capBufferRows = CAP_INI_CONSTR;
        matrizIdxs = (int **) xmalloc(sizeof(int *) * capBufferRows);
        matrizCoefs = (double **) xmalloc(sizeof(double *) * capBufferRows);
        nomeRow = (char **) xmalloc(sizeof(char *) * capBufferRows);
        capRow = (int *) xmalloc(sizeof(int) * capBufferRows);
        sizeRow = (int *) xmalloc(sizeof(int) * capBufferRows);
        rhsRow = (double *) xmalloc(sizeof(double) * capBufferRows);
        sinalRow = (char *) xmalloc(sizeof(char) * capBufferRows);
        fakeIdxRow = (int *) xmalloc(sizeof(int) * capBufferRows);

        for (int i = 0; i < capBufferRows; i++) {
            matrizIdxs[i] = nullptr;
            matrizCoefs[i] = nullptr;
            nomeRow[i] = nullptr;
            capRow[i] = 0;
            sizeRow[i] = 0;
        }
    }

    void realocaMemRestricao(int pos) {
        assert(pos >= 0 && pos < sizeBufferRows);
        const int capAntiga = capRow[pos];
        capRow[pos] = (std::max)(CAP_INI_POR_CONSTR, (int) ceil(capAntiga * 1.5));
        matrizIdxs[pos] = (int *) xrealloc(matrizIdxs[pos], sizeof(int) * capRow[pos]);
        matrizCoefs[pos] = (double *) xrealloc(matrizCoefs[pos], sizeof(double) * capRow[pos]);
    }

    void realocaMemMatriz() {
        const int capAntiga = capBufferRows;
        capBufferRows = (std::max)(CAP_INI_CONSTR, (int) ceil(capAntiga * 1.5));

        matrizIdxs = (int **) xrealloc(matrizIdxs, sizeof(int *) * capBufferRows);
        matrizCoefs = (double **) xrealloc(matrizCoefs, sizeof(double *) * capBufferRows);
        nomeRow = (char **) xrealloc(nomeRow, sizeof(char *) * capBufferRows);
        capRow = (int *) xrealloc(capRow, sizeof(int) * capBufferRows);
        sizeRow = (int *) xrealloc(sizeRow, sizeof(int) * capBufferRows);
        rhsRow = (double *) xrealloc(rhsRow, sizeof(double) * capBufferRows);
        sinalRow = (char *) xrealloc(sinalRow, sizeof(char) * capBufferRows);
        fakeIdxRow = (int *) xrealloc(fakeIdxRow, sizeof(int) * capBufferRows);

        for (int i = capAntiga; i < capBufferRows; i++) {
            matrizIdxs[i] = nullptr;
            matrizCoefs[i] = nullptr;
            nomeRow[i] = nullptr;
            capRow[i] = 0;
            sizeRow[i] = 0;
        }
    }

    void deletaMatriz() {
        if (capBufferRows > 0) {
            for (int i = 0; i < capBufferRows; i++) {
                if (capRow[i] > 0) {
                    free(matrizIdxs[i]);
                    matrizIdxs[i] = nullptr;
                    free(matrizCoefs[i]);
                    matrizCoefs[i] = nullptr;
                    free(nomeRow[i]);
                    nomeRow[i] = nullptr;
                }
            }
            sizeBufferRows = 0;
            capBufferRows = 0;
            free(matrizIdxs);
            matrizIdxs = nullptr;
            free(matrizCoefs);
            matrizCoefs = nullptr;
            free(nomeRow);
            nomeRow = nullptr;
            free(capRow);
            capRow = nullptr;
            free(sizeRow);
            sizeRow = nullptr;
            free(rhsRow);
            rhsRow = nullptr;
            free(sinalRow);
            sinalRow = nullptr;
            free(fakeIdxRow);
            fakeIdxRow = nullptr;
        }
    }

    void insereRestricoes() {
        if (sizeBufferRows == 0) {
            return;
        }

        int numEls = 0;
        for (int i = 0; i < sizeBufferRows; i++) {
            numEls += sizeRow[i];
        }

        int *rowStarts = (int *) xmalloc(sizeof(int) * (sizeBufferRows + 1));
        CoinBigIndex *rowIdxs = (CoinBigIndex *) xmalloc(sizeof(CoinBigIndex) * numEls);
        double *rowCoefs = (double *) xmalloc(sizeof(double) * numEls);
        double *rowLB = (double *) xmalloc(sizeof(double) * sizeBufferRows);
        double *rowUB = (double *) xmalloc(sizeof(double) * sizeBufferRows);
        std::vector<std::string> rowNames(sizeBufferRows);

        numEls = 0;
        for (int i = 0; i < sizeBufferRows; i++) {
            const int nz = sizeRow[i];

            if (escalonarModelo_ && escalonarRestricao_.at(fakeIdxRow[i]) == RowScaling::ON) {
                scaleCoefs(nz, matrizCoefs[i], &rhsRow[i]);
                escalonarRestricao_.at(fakeIdxRow[i]) = RowScaling::DONE;
            }

            rowStarts[i] = numEls;
            rowNames[i] = nomeRow[i];
            memcpy(rowIdxs + numEls, matrizIdxs[i], sizeof(int) * nz);
            memcpy(rowCoefs + numEls, matrizCoefs[i], sizeof(double) * nz);

            if (sinalRow[i] == '<') {
                rowLB[i] = -COIN_DBL_MAX;
                rowUB[i] = rhsRow[i];
            } else if (sinalRow[i] == '=') {
                rowLB[i] = rhsRow[i];
                rowUB[i] = rhsRow[i];
            } else if (sinalRow[i] == '>') {
                rowLB[i] = rhsRow[i];
                rowUB[i] = COIN_DBL_MAX;
            } else {
                throw std::invalid_argument(
                        "Sinal invalido [" + std::to_string(sinalRow[i]) + "] para a restricao de indice " +
                        std::to_string(i));
            }

            numEls += nz;
        }
        rowStarts[sizeBufferRows] = numEls;

        const int numRowsAnterior = solver->getNumRows();
        solver->addRows(sizeBufferRows, rowLB, rowUB, rowStarts, rowIdxs, rowCoefs);

        for (int i = 0; i < sizeBufferRows; i++) {
            solver->setRowName(i + numRowsAnterior, rowNames[i]);
        }

        sizeBufferRows = 0;

        if (capBufferRows > CAP_INI_CONSTR) {
            deletaMatriz();
            inicializaMatriz();
        }

        free(rowStarts);
        free(rowIdxs);
        free(rowCoefs);
        free(rowLB);
        free(rowUB);
    }

    static void remCharsInvalidos(std::string &str) {
        for (char &s : str) {
            if (s == '/' || s == '-') {
                s = '_';
            }
        }
    }

    int solveLinearProgram(ClpSimplex *clp) {
        try {
            assert(clp);

            TipoMetodoSolver metodoSolver = tipoMetodoSolver;
            int paramIdiot = -1, paramPertV = 50;
            double paramPsi = -1.0;
            DualPivot paramDualp = DP_Auto;

            /* verificando se o metodo utilizado eh o automatico */
            if (metodoSolver == TipoMetodoSolver_automatico) {
                ClpSimplexOther *clpo = static_cast<ClpSimplexOther *>(clp);
                assert(clpo);
                char *opts = clpo->guess(0);

                if (opts) {
                    if (strstr(opts, "-primals") != nullptr) {
                        metodoSolver = TipoMetodoSolver_primal_simplex;
                    } else if (strstr(opts, "-duals") != nullptr) {
                        metodoSolver = TipoMetodoSolver_dual_simplex;
                    } else if (strstr(opts, "-barrier") != nullptr) {
                        metodoSolver = TipoMetodoSolver_barrier;
                    }

                    char *s = nullptr;
                    char str[256] = "";
                    if ((s = strstr(opts, "-idiot"))) {
                        s = strstr(s + 1, " ");
                        if (s) {
                            strcpy(str, s + 1);
                            if ((s = strstr(str + 1, " "))) {
                                *s = '\0';
                            }
                            paramIdiot = atoi(str);
                        }
                    } // idiot
                    if ((s = strstr(opts, "-pertv"))) {
                        s = strstr(s + 1, " ");
                        if (s) {
                            strcpy(str, s + 1);
                            if ((s = strstr(str + 1, " "))) {
                                *s = '\0';
                            }
                            paramPertV = atoi(str);
                        }
                    } // perturbation value
                    if ((s = strstr(opts, "-psi"))) {
                        s = strstr(s + 1, " ");
                        if (s) {
                            strcpy(str, s + 1);
                            if ((s = strstr(str + 1, " "))) {
                                *s = '\0';
                            }
                            paramPsi = atof(str);
                        }
                    } // psi
                    if ((s = strstr(opts, "-dualpivot"))) {
                        strcpy(str, s + 1);
                        if ((s = strstr(str + 1, " "))) {
                            *s = '\0';
                        }
                        if (strstr(str, "pesteep")) {
                            paramDualp = DP_PESteepest;
                        }
                    } // dual pivot
                    delete[] opts;
                }
            } // auto

            switch (paramDualp) {
                case DP_Auto: {
                    ClpDualRowSteepest asteep(3);
                    clp->setDualRowPivotAlgorithm(asteep);
                    break;
                }
                case DP_Dantzig: {
                    ClpDualRowDantzig dantzig;
                    clp->setDualRowPivotAlgorithm(dantzig);
                    break;
                }
                case DP_Partial: {
                    ClpDualRowSteepest bsteep(2);
                    clp->setDualRowPivotAlgorithm(bsteep);
                    break;
                }
                case DP_Steepest: {
                    ClpDualRowSteepest csteep;
                    clp->setDualRowPivotAlgorithm(csteep);
                    break;
                }
                case DP_PESteepest: {
                    ClpPEDualRowSteepest p(paramPsi);
                    clp->setDualRowPivotAlgorithm(p);
                    break;
                }
            }

            clp->scaling(1); //equilibrium
            clp->setRandomSeed(1);
            clp->setPrimalTolerance(toleranciaViabilidade);
            clp->setDualTolerance(toleranciaOtimalidade);
            clp->setLogLevel(verbose);
            clp->setMaximumSeconds(tempoLimite);
            clp->setMaximumWallSeconds(tempoLimite);
            clp->setPerturbation(paramPertV);

            ClpSolve clpOptions;
            switch (metodoSolver) {
                case TipoMetodoSolver_automatico:
                    fprintf(stderr, "O metodo automatico ja foi configurado.\n");
                    break;
                case TipoMetodoSolver_primal_simplex:
                    if (paramIdiot > 0) {
                        clpOptions.setSpecialOption(1, 2, paramIdiot);
                    }
                    clpOptions.setSolveType(ClpSolve::usePrimal);
                    clp->setSolveType(1);
                    break;
                case TipoMetodoSolver_dual_simplex:
                    clpOptions.setSolveType(ClpSolve::useDual);
                    clp->setSolveType(1);
                    break;
                case TipoMetodoSolver_barrier:
                    clpOptions.setSolveType(ClpSolve::useBarrier);
                    clpOptions.setSpecialOption(4, 4);
                    clp->setSolveType(2);
                    break;
                default:
                    throw std::invalid_argument("Metodo invalido: " + std::to_string(metodoSolver));
            }

            clp->initialSolve();
            const int status = clp->status();
            const int secondaryStatus = clp->secondaryStatus();

            if (clp->isProvenOptimal()) {
                return 0;
            }
            if (clp->isIterationLimitReached() || (status == 3 && secondaryStatus == 9)) {
                return 1;
            }
            if (clp->isProvenDualInfeasible()) {
                return 3;
            }
            if (clp->isProvenPrimalInfeasible()) {
                return 2;
            }

            //clp->writeMps("erro_solveLinearProgram");
            //clp->writeLp("erro_solveLinearProgram");
            statusOtimizacao = TipoStatusSolver_nao_otimalidade;
            throw std::invalid_argument(
                    "10005 CLP retornou um status invalido: primary = " + std::to_string(status) + " secondary = " +
                    std::to_string(secondaryStatus));
        }

        catch (const CoinError &erro) {
            throw std::invalid_argument("SolverCLP::solveLinearProgram(): \n" + erro.message());
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::solveLinearProgram(): \n" + std::string(erro.what()));
        }
    }

    void solve(ClpSimplex *clp) {
        try {
            int res = solveLinearProgram(clp);

            if (varX) {
                delete[] varX;
            }
            if (varRC) {
                delete[] varRC;
            }
            if (dualPrice) {
                delete[] dualPrice;
            }

            varX = new double[clp->getNumCols()];
            varRC = new double[clp->getNumCols()];
            dualPrice = new double[clp->getNumRows()];

            if (res == -1) { //status invalido
                objValue = bestPossibleObjValue = COIN_DBL_MAX;
                std::fill(varX, varX + clp->getNumCols(), NAN);
                std::fill(varRC, varRC + clp->getNumCols(), NAN);
                std::fill(dualPrice, dualPrice + clp->getNumRows(), NAN);
                statusOtimizacao = TipoStatusSolver_nao_otimalidade;
                const int status = clp->status();
                const int secondaryStatus = clp->secondaryStatus();
               // clp->writeMps("erro_solve-1");
                //clp->writeLp("erro_solve-1");
                throw std::invalid_argument(
                        "10005 CLP retornou um status invalido: primary = " + std::to_string(status) + " secondary = " +
                        std::to_string(secondaryStatus));
            } else if (res == 2 || res == 3) { //problema infactivel
                objValue = bestPossibleObjValue = COIN_DBL_MAX;
                std::fill(varX, varX + clp->getNumCols(), NAN);
                std::fill(varRC, varRC + clp->getNumCols(), NAN);
                std::fill(dualPrice, dualPrice + clp->getNumRows(), NAN);
                statusOtimizacao = TipoStatusSolver_nao_otimalidade;
                //clp->writeMps("erro_infactivel");
               // clp->writeLp("erro_infactivel");
                return;
            } else if (res == 1) { // tempo limite excedido
                objValue = bestPossibleObjValue = clp->objectiveValue();
                const double *x = clp->primalColumnSolution();
                memcpy(varX, x, sizeof(double) * clp->getNumCols());
                const double *rc = clp->dualColumnSolution();
                memcpy(varRC, rc, sizeof(double) * clp->getNumCols());
                const double *dp = clp->dualRowSolution();
                memcpy(dualPrice, dp, sizeof(double) * clp->getNumRows());
                statusOtimizacao = TipoStatusSolver_tempo_excedido;
                return;
            }

            if (!isMILP) { //CLP resolveu na otimalidade e o problema contem somente vars continuas
                objValue = bestPossibleObjValue = clp->objectiveValue();
                const double *x = clp->primalColumnSolution();
                memcpy(varX, x, sizeof(double) * clp->getNumCols());
                const double *rc = clp->dualColumnSolution();
                memcpy(varRC, rc, sizeof(double) * clp->getNumCols());
                const double *dp = clp->dualRowSolution();
                memcpy(dualPrice, dp, sizeof(double) * clp->getNumRows());
                statusOtimizacao = TipoStatusSolver_otimalidade;
                return;
            }

            OsiClpSolverInterface linearProgram(clp);
            CbcModel cbcModel(linearProgram);
            CbcSolverUsefulData cbcData;
            double paramAllowableGap = 1e-10, paramAllowableFractionGap = 0.0001, paramIntegerTolerance = 1e-6;
            int paramRoundIntVars = 1, paramRandomSeed = 1, paramUseElapsedTime = 1;

            CbcMain0(cbcModel, cbcData);
            cbcData.printWelcome_ = false;
            cbcModel.setMaximumSeconds(tempoLimite);
            cbcModel.setAllowableGap(paramAllowableGap);
            cbcModel.setAllowableFractionGap(paramAllowableFractionGap);
            cbcModel.setIntegerTolerance(paramIntegerTolerance);
            cbcModel.setLogLevel(verbose);
            cbcModel.setRoundIntegerVariables(paramRoundIntVars);
            cbcModel.setRandomSeed(paramRandomSeed);
            cbcModel.setUseElapsedTime(paramUseElapsedTime == 1);

            const char *args[] = {"cbc", "-cgraph=on", "-clqstr=off", "-solve", "-quit"};
            const int nargs = 5;
            CbcMain1(nargs, args, cbcModel, cbcData);

            if (cbcModel.isAbandoned()) {
                objValue = bestPossibleObjValue = COIN_DBL_MAX;
                std::fill(varX, varX + clp->getNumCols(), NAN);
                std::fill(varRC, varRC + clp->getNumCols(), NAN);
                std::fill(dualPrice, dualPrice + clp->getNumRows(), NAN);
                statusOtimizacao = TipoStatusSolver_nao_otimalidade;
                //cbcModel.solver()->writeMps("erro_solve-abandoned");
                //cbcModel.solver()->writeLp("erro_solve-abandoned");
                throw std::invalid_argument("10005 solve-abandoned - Instabilidade numerica!");
            } else if (cbcModel.isProvenInfeasible() || cbcModel.isProvenDualInfeasible() ||
                       cbcModel.isContinuousUnbounded()) {
                objValue = bestPossibleObjValue = COIN_DBL_MAX;
                std::fill(varX, varX + clp->getNumCols(), NAN);
                std::fill(varRC, varRC + clp->getNumCols(), NAN);
                std::fill(dualPrice, dualPrice + clp->getNumRows(), NAN);
                statusOtimizacao = TipoStatusSolver_nao_otimalidade;
            } else if (cbcModel.isProvenOptimal() || cbcModel.isSecondsLimitReached()) {
                if (cbcModel.isProvenOptimal()) {
                    statusOtimizacao = TipoStatusSolver_otimalidade;
                } else {
                    statusOtimizacao = TipoStatusSolver_tempo_excedido;
                }

                bestPossibleObjValue = cbcModel.getBestPossibleObjValue();

                if (cbcModel.bestSolution()) {
                    if (paramRoundIntVars) {
                        cbcModel.roundIntVars();
                    }

                    objValue = cbcModel.getObjValue();
                    const double *x = cbcModel.bestSolution();
                    memcpy(varX, x, sizeof(double) * cbcModel.getNumCols());
                    const double *rc = cbcModel.getReducedCost();
                    memcpy(varRC, rc, sizeof(double) * cbcModel.getNumCols());
                    const double *dp = cbcModel.getRowPrice();
                    memcpy(dualPrice, dp, sizeof(double) * cbcModel.getNumRows());
                } else {
                    objValue = COIN_DBL_MAX;
                    std::fill(varX, varX + clp->getNumCols(), NAN);
                    std::fill(varRC, varRC + clp->getNumCols(), NAN);
                    std::fill(dualPrice, dualPrice + clp->getNumRows(), NAN);
                }
            } else {
                statusOtimizacao = TipoStatusSolver_nao_otimalidade;
                //cbcModel.solver()->writeMps("erro_solve-invalido");
               // cbcModel.solver()->writeLp("erro_solve-invalido");
                throw std::invalid_argument("10005 CBC retornou um status invalido!");
            }
        }

        catch (const CoinError &erro) { throw std::invalid_argument("SolverCLP::otimiza(): \n" + erro.message()); }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::otimiza(): \n" + std::string(erro.what()));
        }
    }

    bool inicializar() {
        try {
            if (solver != nullptr) {
                delete solver;
                solver = nullptr;
            }

            deletaMatriz();

            if (dualPrice) {
                delete[] dualPrice;
                dualPrice = nullptr;
            }
            if (varX) {
                delete[] varX;
                varX = nullptr;
            }
            if (varRC) {
                delete[] varRC;
                varRC = nullptr;
            }

            solver = new ClpSimplex();
            infinitoSolver = COIN_DBL_MAX;
            realNumRows = fakeNumRows = 0;
            toleranciaOtimalidade = 1e-6;
            toleranciaViabilidade = 1e-6;
            tempo_otimizacao = 0.0;
            isMILP = false;
            statusOtimizacao = TipoStatusSolver_Nenhum;
            lista_variavel_dinamica = std::vector<int>();
            escalonarRestricao_.clear();
            origRowIdx.clear();
            origRowSense.clear();
            tempoLimite = COIN_DBL_MAX;
            objValue = COIN_DBL_MAX;
            bestPossibleObjValue = COIN_DBL_MAX;
            tipoMetodoSolverPadrao = TipoMetodoSolver_dual_simplex;
            setMetodoPadrao();
            exibirNaTela(false);
            inicializaMatriz();

            return true;
        }

        catch (const CoinError &erro) {
            throw std::invalid_argument("SolverCLP::inicializar(): \n" + erro.message());
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::inicializar(): \n" + std::string(erro.what()));
        }
    }

    void scaleCoefs(int nz, double *coefs, double *rhs) {
        const double newMinCoef = getMenorValorCoef();
        const double newMaxCoef = getMaiorValorCoef();
        assert(newMaxCoef > newMinCoef + 1e-6);

        double minCoef = *rhs;
        double maxCoef = *rhs;
        for (int j = 0; j < nz; j++) {
            minCoef = (std::min)(minCoef, coefs[j]);
            maxCoef = (std::max)(maxCoef, coefs[j]);
        }

        if (minCoef >= newMinCoef && maxCoef <= newMaxCoef) {
            return;
        }

        if (fabs(maxCoef - minCoef) > 1e-6) {
            for (int j = 0; j < nz; j++) {
                coefs[j] = (((coefs[j] - minCoef) / (maxCoef - minCoef)) * (newMaxCoef - newMinCoef)) + newMinCoef;
            }
            *rhs = ((((*rhs) - minCoef) / (maxCoef - minCoef)) * (newMaxCoef - newMinCoef)) + newMinCoef;
        } else {//todos os valores de coefs e rhs sao iguais; todos serao escalonados para 1
            for (int j = 0; j < nz; j++) {
                coefs[j] = 1.0;
            }
            *rhs = 1.0;
        }
    }

public:
    SolverCLP() {
        try {
            inicializar();
        }
        catch (const CoinError &erro) {
            throw std::invalid_argument("SolverCLP::SolverCLP(): \n" + erro.message());
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::SolverCLP(): \n" + std::string(erro.what()));
        }
    }

    ~SolverCLP() {
        if (solver != nullptr) {
            delete solver;
            solver = nullptr;
        }

        deletaMatriz();
    }

    std::string str() { return "CLP"; }

    bool isNomeSimplificado() { return true; }

    int addVar(const double a_lb, const double a_ub, const double a_obj, const std::string a_nome) {
        try {
            std::string nomeVar = a_nome;
            remCharsInvalidos(nomeVar);
            const int varIdx = solver->getNumCols();
            solver->addColumn(0, nullptr, nullptr, a_lb, a_ub, a_obj);
            solver->setColumnName(varIdx, nomeVar);
            solver->setContinuous(varIdx); /* variavel inicialmente setada como continua */

            return varIdx;
        }

        catch (const CoinError &erro) {
            throw std::invalid_argument("SolverCLP::addVar(" + std::to_string(a_lb) + "," + std::to_string(a_ub) + "," +
                                        std::to_string(a_obj) + "," + a_nome + "): \n" + erro.message());
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::addVar(" + std::to_string(a_lb) + "," + std::to_string(a_ub) + "," +
                                        std::to_string(a_obj) + "," + a_nome + "): \n" + std::string(erro.what()));
        }
    }

    bool setCofObjetivo(const int a_posicao, const double a_cofObjetivo) {
        try {
            solver->setObjectiveCoefficient(a_posicao, a_cofObjetivo);
            return true;
        }

        catch (const CoinError &erro) {
            throw std::invalid_argument(
                    "SolverCLP::setCofObjetivo(" + std::to_string(a_posicao) + "," + std::to_string(a_cofObjetivo) +
                    "): \n" + erro.message());
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument(
                    "SolverCLP::setCofObjetivo(" + std::to_string(a_posicao) + "," + std::to_string(a_cofObjetivo) +
                    "): \n" + std::string(erro.what()));
        }
    }

    bool setLimInferior(const int a_posicao, const double a_limInferior) {
        try {
            solver->setColLower(a_posicao, a_limInferior);
            return true;
        }

        catch (const CoinError &erro) {
            throw std::invalid_argument(
                    "SolverCLP::setLimInferior(" + std::to_string(a_posicao) + "," + std::to_string(a_limInferior) +
                    "): \n" + erro.message());
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument(
                    "SolverCLP::setLimInferior(" + std::to_string(a_posicao) + "," + std::to_string(a_limInferior) +
                    "): \n" + std::string(erro.what()));
        }
    }

    bool setLimSuperior(const int a_posicao, const double a_limSuperior) {
        try {
            solver->setColUpper(a_posicao, a_limSuperior);
            return true;
        }

        catch (const CoinError &erro) {
            throw std::invalid_argument(
                    "SolverCLP::setLimSuperior(" + std::to_string(a_posicao) + "," + std::to_string(a_limSuperior) +
                    "): \n" + erro.message());
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument(
                    "SolverCLP::setLimSuperior(" + std::to_string(a_posicao) + "," + std::to_string(a_limSuperior) +
                    "): \n" + std::string(erro.what()));
        }
    }

    bool setTipoVariavelContinua(const int a_posicao) {
        try {
            solver->setContinuous(a_posicao);
            return true;
        }

        catch (const CoinError &erro) {
            throw std::invalid_argument(
                    "SolverCLP::setTipoVariavelContinua(" + std::to_string(a_posicao) + "): \n" + erro.message());
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::setTipoVariavelContinua(" + std::to_string(a_posicao) + "): \n" +
                                        std::string(erro.what()));
        }
    }

    bool setTipoVariavelBinaria(const int a_posicao) {
        try {
            solver->setInteger(a_posicao);
            solver->setColBounds(a_posicao, 0.0, 1.0);
            isMILP = true;

            return true;
        }

        catch (const CoinError &erro) {
            throw std::invalid_argument(
                    "SolverCLP::setTipoVariavelBinaria(" + std::to_string(a_posicao) + "): \n" + erro.message());
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::setTipoVariavelBinaria(" + std::to_string(a_posicao) + "): \n" +
                                        std::string(erro.what()));
        }
    }

    double getInfinito() {
        return infinitoSolver;
    }

    double getCofObjetivo(const int a_posicao) {
        try {
            return solver->getObjCoefficients()[a_posicao];
        }

        catch (const std::exception &erro) {
            throw std::invalid_argument(
                    "SolverCLP::getCofObjetivo(" + std::to_string(a_posicao) + "): \n" + std::string(erro.what()));
        }
    }

    double getLimInferior(const int a_posicao) {
        try {
            return solver->getColLower()[a_posicao];
        }

        catch (const std::exception &erro) {
            throw std::invalid_argument(
                    "SolverCLP::getLimInferior(" + std::to_string(a_posicao) + "): \n" + std::string(erro.what()));
        }
    }

    double getLimSuperior(const int a_posicao) {
        try {
            return solver->getColUpper()[a_posicao];
        }

        catch (const std::exception &erro) {
            throw std::invalid_argument(
                    "SolverCLP::getLimSuperior(" + std::to_string(a_posicao) + "): \n" + std::string(erro.what()));
        }
    }

    int addConstrIgual(const std::string a_nome, bool scale = false) {
        try {
            std::string nomeRestricao = a_nome;
            remCharsInvalidos(nomeRestricao);

            if (sizeBufferRows + 1 > capBufferRows) {
                realocaMemMatriz();
            }

            assert(sizeRow[sizeBufferRows] == 0);

            if (capRow[sizeBufferRows] == 0) {
                capRow[sizeBufferRows] = CAP_INI_POR_CONSTR;
                matrizIdxs[sizeBufferRows] = (int *) xmalloc(sizeof(int) * capRow[sizeBufferRows]);
                matrizCoefs[sizeBufferRows] = (double *) xmalloc(sizeof(double) * capRow[sizeBufferRows]);
            }

            if (nomeRow[sizeBufferRows] == nullptr) {
                nomeRow[sizeBufferRows] = (char *) xmalloc(sizeof(char) * (nomeRestricao.size() + 1));
            } else if (strlen(nomeRow[sizeBufferRows]) < nomeRestricao.size() + 1) {
                free(nomeRow[sizeBufferRows]);
                nomeRow[sizeBufferRows] = (char *) xmalloc(sizeof(char) * (nomeRestricao.size() + 1));
            }

            strncpy(nomeRow[sizeBufferRows], nomeRestricao.c_str(), nomeRestricao.size());
            nomeRow[sizeBufferRows][nomeRestricao.size()] = '\0';
            sinalRow[sizeBufferRows] = '=';
            fakeIdxRow[sizeBufferRows] = fakeNumRows;
            rhsRow[sizeBufferRows] = 0.0;
            sizeBufferRows++;
            origRowIdx.push_back(realNumRows);
            origRowSense.push_back('=');
            escalonarRestricao_.push_back(scale ? RowScaling::ON : RowScaling::OFF);
            fakeNumRows++;
            realNumRows++;

            return (fakeNumRows - 1);
        }

        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::addConstrIgual(" + a_nome + "): \n" + std::string(erro.what()));
        }
    }

    int addConstrMaior(const std::string a_nome, bool scale = false) {
        try {
            std::string nomeRestricao = a_nome;
            remCharsInvalidos(nomeRestricao);

            if (sizeBufferRows + 1 > capBufferRows) {
                realocaMemMatriz();
            }

            assert(sizeRow[sizeBufferRows] == 0);

            if (capRow[sizeBufferRows] == 0) {
                capRow[sizeBufferRows] = CAP_INI_POR_CONSTR;
                matrizIdxs[sizeBufferRows] = (int *) xmalloc(sizeof(int) * capRow[sizeBufferRows]);
                matrizCoefs[sizeBufferRows] = (double *) xmalloc(sizeof(double) * capRow[sizeBufferRows]);
            }

            if (nomeRow[sizeBufferRows] == nullptr) {
                nomeRow[sizeBufferRows] = (char *) xmalloc(sizeof(char) * (nomeRestricao.size() + 1));
            } else if (strlen(nomeRow[sizeBufferRows]) < nomeRestricao.size() + 1) {
                free(nomeRow[sizeBufferRows]);
                nomeRow[sizeBufferRows] = (char *) xmalloc(sizeof(char) * (nomeRestricao.size() + 1));
            }

            strncpy(nomeRow[sizeBufferRows], nomeRestricao.c_str(), nomeRestricao.size());
            nomeRow[sizeBufferRows][nomeRestricao.size()] = '\0';
            sinalRow[sizeBufferRows] = '>';
            fakeIdxRow[sizeBufferRows] = fakeNumRows;
            rhsRow[sizeBufferRows] = 0.0;
            sizeBufferRows++;
            origRowIdx.push_back(realNumRows);
            origRowSense.push_back('>');
            escalonarRestricao_.push_back(scale ? RowScaling::ON : RowScaling::OFF);
            fakeNumRows++;
            realNumRows++;

            return (fakeNumRows - 1);
        }

        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::addConstrMaior(" + a_nome + "): \n" + std::string(erro.what()));
        }
    }

    int addConstrMenor(const std::string a_nome, bool scale = false) {
        try {
            std::string nomeRestricao = a_nome;
            remCharsInvalidos(nomeRestricao);

            if (sizeBufferRows + 1 > capBufferRows) {
                realocaMemMatriz();
            }

            assert(sizeRow[sizeBufferRows] == 0);

            if (capRow[sizeBufferRows] == 0) {
                capRow[sizeBufferRows] = CAP_INI_POR_CONSTR;
                matrizIdxs[sizeBufferRows] = (int *) xmalloc(sizeof(int) * capRow[sizeBufferRows]);
                matrizCoefs[sizeBufferRows] = (double *) xmalloc(sizeof(double) * capRow[sizeBufferRows]);
            }

            if (nomeRow[sizeBufferRows] == nullptr) {
                nomeRow[sizeBufferRows] = (char *) xmalloc(sizeof(char) * (nomeRestricao.size() + 1));
            } else if (strlen(nomeRow[sizeBufferRows]) < nomeRestricao.size() + 1) {
                free(nomeRow[sizeBufferRows]);
                nomeRow[sizeBufferRows] = (char *) xmalloc(sizeof(char) * (nomeRestricao.size() + 1));
            }

            strncpy(nomeRow[sizeBufferRows], nomeRestricao.c_str(), nomeRestricao.size());
            nomeRow[sizeBufferRows][nomeRestricao.size()] = '\0';
            sinalRow[sizeBufferRows] = '<';
            fakeIdxRow[sizeBufferRows] = fakeNumRows;
            rhsRow[sizeBufferRows] = 0.0;
            sizeBufferRows++;
            origRowIdx.push_back(realNumRows);
            origRowSense.push_back('<');
            escalonarRestricao_.push_back(scale ? RowScaling::ON : RowScaling::OFF);
            fakeNumRows++;
            realNumRows++;

            return (fakeNumRows - 1);
        }

        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::addConstrMenor(" + a_nome + "): \n" + std::string(erro.what()));
        }
    }

    bool setNomeRestricao(const int a_posicaoRestricao, const std::string a_nome) {
        try {
            const int rowIdx = origRowIdx.at(a_posicaoRestricao);
            if (rowIdx == -1) { //restricao ja foi removida do modelo
                throw std::invalid_argument(
                        "Restricao de indice [" + std::to_string(a_posicaoRestricao) + "] foi removida do modelo.");
            }

            std::string nomeRestricao = a_nome;
            remCharsInvalidos(nomeRestricao);

            if (rowIdx >= solver->getNumRows()) { //restricao esta no buffer; ainda nao foi inserida no modelo
                const int idxBuffer = rowIdx - solver->getNumRows();

                if (strlen(nomeRow[idxBuffer]) < nomeRestricao.size() + 1) {
                    free(nomeRow[idxBuffer]);
                    nomeRow[idxBuffer] = (char *) xmalloc(sizeof(char) * (nomeRestricao.size() + 1));
                }
                strncpy(nomeRow[idxBuffer], nomeRestricao.c_str(), nomeRestricao.size());
                nomeRow[idxBuffer][nomeRestricao.size()] = '\0';
            } else { //restricao ja foi inserida no solver
                solver->setRowName(rowIdx, nomeRestricao);
            }

            return true;
        }

        catch (const CoinError &erro) {
            throw std::invalid_argument(
                    "SolverCLP::setNomeRestricao(" + std::to_string(a_posicaoRestricao) + "," + a_nome + "): \n" +
                    erro.message());
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument(
                    "SolverCLP::setNomeRestricao(" + std::to_string(a_posicaoRestricao) + "," + a_nome + "): \n" +
                    std::string(erro.what()));
        }
    }

    bool setCofRestricao(const int a_posicaoVariavel, const int a_posicaoRestricao, const double a_cofRestricao) {
        try {
            const int rowIdx = origRowIdx.at(a_posicaoRestricao);
            if (rowIdx == -1) {//restricao ja foi removida do modelo
                throw std::invalid_argument(
                        "Restricao de indice [" + std::to_string(a_posicaoRestricao) + "] foi removida do modelo.");
            }

            if (rowIdx >= solver->getNumRows()) { //restricao esta no buffer; ainda nao foi inserida no modelo
                const int idxBuffer = rowIdx - solver->getNumRows();

                for (int i = 0; i < sizeRow[idxBuffer]; i++) {
                    //coef da var na restricao eh diferente de zero, basta substituir
                    if (matrizIdxs[idxBuffer][i] == a_posicaoVariavel) {
                        matrizCoefs[idxBuffer][i] = a_cofRestricao;
                        return true;
                    }
                }

                const int tamAtual = sizeRow[idxBuffer];

                if (tamAtual + 1 > capRow[idxBuffer]) {
                    realocaMemRestricao(idxBuffer);
                }

                matrizIdxs[idxBuffer][tamAtual] = a_posicaoVariavel;
                matrizCoefs[idxBuffer][tamAtual] = a_cofRestricao;
                sizeRow[idxBuffer] = sizeRow[idxBuffer] + 1;
            } else { //restricao ja foi inserida no solver
                solver->modifyCoefficient(rowIdx, a_posicaoVariavel, a_cofRestricao);
            }

            return true;
        }

        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::setCofRestricao(" + std::to_string(a_posicaoVariavel) + "," +
                                        std::to_string(a_posicaoRestricao) + "," + std::to_string(a_cofRestricao) +
                                        "): \n" + std::string(erro.what()));
        }
    }

    bool setRHSRestricao(const int a_posicaoRestricao, const double a_RHS) {
        try {
            const int rowIdx = origRowIdx.at(a_posicaoRestricao);
            if (rowIdx == -1) {//restricao ja foi removida do modelo
                throw std::invalid_argument(
                        "Restricao de indice [" + std::to_string(a_posicaoRestricao) + "] foi removida do modelo.");
            }

            if (rowIdx >= solver->getNumRows()) { //restricao esta no buffer; ainda nao foi inserida no modelo
                const int idxBuffer = rowIdx - solver->getNumRows();
                rhsRow[idxBuffer] = a_RHS;
            } else { //restricao ja foi inserida no solver
                const char rowSense = origRowSense.at(a_posicaoRestricao);

                if (rowSense == '<') {
                    solver->setRowBounds(rowIdx, -COIN_DBL_MAX, a_RHS);
                } else if (rowSense == '=') {
                    solver->setRowBounds(rowIdx, a_RHS, a_RHS);
                } else if (rowSense == '>') {
                    solver->setRowBounds(rowIdx, a_RHS, COIN_DBL_MAX);
                } else {
                    throw std::invalid_argument(
                            "Sinal invalido [" + std::to_string(rowSense) + "] para a restricao de indice " +
                            std::to_string(a_posicaoRestricao));
                }
            }

            return true;
        }

        catch (const CoinError &erro) {
            throw std::invalid_argument(
                    "SolverCLP::setRHSRestricao(" + std::to_string(a_posicaoRestricao) + "," + std::to_string(a_RHS) +
                    "): \n" + erro.message());
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument(
                    "SolverCLP::setRHSRestricao(" + std::to_string(a_posicaoRestricao) + "," + std::to_string(a_RHS) +
                    "): \n" + std::string(erro.what()));
        }
    }

    double getCofRestricao(const int a_posicaoVariavel, const int a_posicaoRestricao) {
        try {
            const int rowIdx = origRowIdx.at(a_posicaoRestricao);
            if (rowIdx == -1) {//restricao ja foi removida do modelo
                throw std::invalid_argument(
                        "Restricao de indice [" + std::to_string(a_posicaoRestricao) + "] foi removida do modelo.");
            }

            if (rowIdx >= solver->getNumRows()) { //restricao esta no buffer; ainda nao foi inserida no modelo
                const int idxBuffer = rowIdx - solver->getNumRows();
                for (int i = 0; i < sizeRow[idxBuffer]; i++) {
                    if (matrizIdxs[idxBuffer][i] == a_posicaoVariavel) {
                        return matrizCoefs[idxBuffer][i];
                    }
                }
                return 0.0;
            } else { //restricao ja foi inserida no solver
                const CoinPackedMatrix *matrixByRow = solver->matrix();
                const int *idxs = matrixByRow->getIndices();
                const double *coefs = matrixByRow->getElements();
                const CoinBigIndex *start = matrixByRow->getVectorStarts();
                const int *length = matrixByRow->getVectorLengths();

                for (int j = start[rowIdx]; j < start[rowIdx] + length[rowIdx]; j++) {
                    if (idxs[j] == a_posicaoVariavel) {
                        return coefs[j];
                    }
                }

                return 0.0;
            }
        }

        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::getCofRestricao(" + std::to_string(a_posicaoVariavel) + "," +
                                        std::to_string(a_posicaoRestricao) + "): \n" + std::string(erro.what()));
        }
    }

    double getRHSRestricao(const int a_posicaoRestricao) {
        try {
            const int rowIdx = origRowIdx.at(a_posicaoRestricao);
            if (rowIdx == -1) {//restricao ja foi removida do modelo
                throw std::invalid_argument(
                        "Restricao de indice [" + std::to_string(a_posicaoRestricao) + "] foi removida do modelo.");
            }

            const char rowSense = origRowSense.at(a_posicaoRestricao);

            if (rowSense == '<' || rowSense == '=') {
                return solver->getRowUpper()[rowIdx];
            } else if (rowSense == '>') {
                return solver->getRowLower()[rowIdx];
            } else {
                throw std::invalid_argument(
                        "Sinal invalido [" + std::to_string(rowSense) + "] para a restricao de indice " +
                        std::to_string(a_posicaoRestricao));
            }
        }

        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::getRHSRestricao(" + std::to_string(a_posicaoRestricao) + "): \n" +
                                        std::string(erro.what()));
        }
    }

    bool remRestricao(const int a_posicaoRestricao) {
        try {
            const int rowIdx = origRowIdx.at(a_posicaoRestricao);
            if (rowIdx == -1) {//restricao ja foi removida do modelo
                throw std::invalid_argument(
                        "Restricao de indice [" + std::to_string(a_posicaoRestricao) + "] ja foi removida do modelo.");
            }

            if (rowIdx >= solver->getNumRows()) { //restricao esta no buffer; ainda nao foi inserida no modelo
                const int idxBuffer = rowIdx - solver->getNumRows();

                free(matrizIdxs[idxBuffer]);
                free(matrizCoefs[idxBuffer]);
                free(nomeRow[idxBuffer]);

                for (int i = idxBuffer + 1; i < sizeBufferRows; i++) {
                    matrizIdxs[i - 1] = matrizIdxs[i];
                    matrizCoefs[i - 1] = matrizCoefs[i];
                    nomeRow[i - 1] = nomeRow[i];
                    capRow[i - 1] = capRow[i];
                    sizeRow[i - 1] = sizeRow[i];
                    rhsRow[i - 1] = rhsRow[i];
                    sinalRow[i - 1] = sinalRow[i];
                    fakeIdxRow[i - 1] = fakeIdxRow[i];
                }
                matrizIdxs[sizeBufferRows - 1] = nullptr;
                matrizCoefs[sizeBufferRows - 1] = nullptr;
                nomeRow[sizeBufferRows - 1] = nullptr;
                capRow[sizeBufferRows - 1] = 0;
                sizeRow[sizeBufferRows - 1] = 0;
                rhsRow[sizeBufferRows - 1] = NAN;
                sinalRow[sizeBufferRows - 1] = ' ';
                fakeIdxRow[sizeBufferRows - 1] = -1;
                sizeBufferRows--;
            } else { //restricao ja foi inserida no solver
                int toDelete[1];
                toDelete[0] = rowIdx;
                solver->deleteRows(1, toDelete);
            }

            /* atualizando os indices das restricoes posteriores */
            origRowIdx.at(a_posicaoRestricao) = -1; /* restricoes com indices iguais a -1 sao restricoes removidas */
            origRowSense.at(a_posicaoRestricao) = ' ';
            escalonarRestricao_.at(a_posicaoRestricao) = RowScaling::OFF;
            for (int i = a_posicaoRestricao + 1; i < fakeNumRows; i++) {
                assert(origRowIdx[i] == -1 || (origRowIdx[i] > 0 && origRowIdx[i] < realNumRows));

                if (origRowIdx[i] != -1) {
                    origRowIdx[i] = origRowIdx[i] - 1;
                }
            }

            realNumRows--;

            return true;
        }

        catch (const CoinError &erro) {
            throw std::invalid_argument(
                    "SolverCLP::remRestricao(" + std::to_string(a_posicaoRestricao) + "): \n" + erro.message());
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::remRestricao(" + std::to_string(a_posicaoRestricao) + "): \n" +
                                        std::string(erro.what()));
        }
    }

    bool addVarDinamica(const int a_posicaoVariavel) {
        try {
            if (a_posicaoVariavel >= 0 && a_posicaoVariavel < solver->getNumCols()) {
                for (int vd : lista_variavel_dinamica) {
                    if (vd == a_posicaoVariavel) {
                        return true; /* variavel dinamica ja inserida anteriormente */
                    }
                }

                lista_variavel_dinamica.push_back(a_posicaoVariavel);
            } else {
                throw std::invalid_argument(
                        "Variavel com indice " + std::to_string(a_posicaoVariavel) + "nao existe no modelo.");
            }

            return true;
        }

        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::addVarDinamica(" + std::to_string(a_posicaoVariavel) + "): \n" +
                                        std::string(erro.what()));
        }
    }

    double getSolucaoInferiorVarDinamica() {
        try {
            double valor_integrado_positivo = 0.0;
            double valor_integrado_negativo = 0.0;

            for (int var : lista_variavel_dinamica) {
                const double reduced_cost = getReducedCost(var);

                if (reduced_cost > 0.0) {
                    const double limite_inferior = getLimInferior(var);

                    if (limite_inferior > 0.0) {
                        valor_integrado_positivo += reduced_cost * limite_inferior;
                    } else if (limite_inferior < 0.0) {
                        valor_integrado_negativo += reduced_cost * limite_inferior;
                    }
                } else if (reduced_cost < 0.0) {
                    const double limite_superior = getLimSuperior(var);

                    if (limite_superior > 0.0) {
                        valor_integrado_negativo += reduced_cost * limite_superior;
                    } else if (limite_superior < 0.0) {
                        valor_integrado_positivo += reduced_cost * limite_superior;
                    }
                }
            }

            return valor_integrado_positivo + valor_integrado_negativo;
        }

        catch (const CoinError &erro) {
            throw std::invalid_argument("SolverCLP::getSolucaoInferiorVarDinamica(): \n" + erro.message());
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::getSolucaoInferiorVarDinamica(): \n" + std::string(erro.what()));
        }
    }

    std::vector<double> getLimiteInferiorVarDinamica() {
        try {
            std::vector<double> limite_inferior(lista_variavel_dinamica.size(), 0.0);

            for (int i = 0; i < lista_variavel_dinamica.size(); i++) {
                limite_inferior[i] = getLimInferior(lista_variavel_dinamica[i]);
            }

            return limite_inferior;
        }

        catch (const CoinError &erro) {
            throw std::invalid_argument("SolverCLP::getLimiteInferiorVarDinamica(): \n" + erro.message());
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::getLimiteInferiorVarDinamica(): \n" + std::string(erro.what()));
        }
    }

    std::vector<double> getLimiteSuperiorVarDinamica() {
        try {
            std::vector<double> limite_superior(lista_variavel_dinamica.size(), 0.0);

            for (int i = 0; i < lista_variavel_dinamica.size(); i++) {
                limite_superior[i] = getLimSuperior(lista_variavel_dinamica[i]);
            }

            return limite_superior;
        }

        catch (const CoinError &erro) {
            throw std::invalid_argument("SolverCLP::getLimiteSuperiorVarDinamica(): \n" + erro.message());
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::getLimiteSuperiorVarDinamica(): \n" + std::string(erro.what()));
        }
    }

    std::vector<double> getReducedCostVarDinamica() {
        try {
            std::vector<double> reduced_cost(lista_variavel_dinamica.size(), 0.0);

            for (int i = 0; i < lista_variavel_dinamica.size(); i++) {
                reduced_cost[i] = getReducedCost(lista_variavel_dinamica[i]);
            }

            return reduced_cost;
        }

        catch (const CoinError &erro) {
            throw std::invalid_argument("SolverCLP::getReducedCostVarDinamica(): \n" + erro.message());
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::getReducedCostVarDinamica(): \n" + std::string(erro.what()));
        }
    }

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

    int getNumeroVarDinamica() { return int(lista_variavel_dinamica.size()); }

    double getMultiplicador(const int a_posicaoRestricao) {
        try {
            const int rowIdx = origRowIdx.at(a_posicaoRestricao);

            if (rowIdx == -1) {
                throw std::invalid_argument(
                        "Restricao de indice " + std::to_string(a_posicaoRestricao) + " foi removida do modelo.");
            }

            return dualPrice[rowIdx];
        }

        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::getMultiplicador(" + std::to_string(a_posicaoRestricao) + "): \n" +
                                        std::string(erro.what()));
        }
    }

    char getSinalRestricao(const int a_posicaoRestricao) {
        try {
            const int rowIdx = origRowIdx.at(a_posicaoRestricao);

            if (rowIdx == -1) {
                throw std::invalid_argument(
                        "Restricao de indice " + std::to_string(a_posicaoRestricao) + " foi removida do modelo.");
            }

            return origRowSense.at(a_posicaoRestricao);
        }

        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::getSinalRestricao(" + std::to_string(a_posicaoRestricao) + "): \n" +
                                        std::string(erro.what()));
        }
    }

    bool otimizar(ClpSimplex *clp) {
        auto start_clock = std::chrono::high_resolution_clock::now();
        solve(clp);
        std::chrono::duration<double> tempo = std::chrono::high_resolution_clock::now() - start_clock;
        tempo_otimizacao = tempo.count();

        return true;
    }

    bool otimizar() {
        insereRestricoes();
        return otimizar(solver);
    }

    bool resetar() {
        try {
            insereRestricoes();

            ClpSimplex *newSolver = new ClpSimplex();
            newSolver->loadProblem(*solver->matrix(), solver->getColLower(), solver->getColUpper(),
                                   solver->getObjCoefficients(), solver->getRowLower(), solver->getRowUpper());
            for (int i = 0; i < solver->getNumCols(); i++) {
                std::string colName = solver->columnName(i);
                newSolver->setColumnName(i, colName);

                if (solver->isInteger(i)) {
                    newSolver->setInteger(i);
                } else {
                    newSolver->setContinuous(i);
                }
            }
            for (int i = 0; i < solver->getNumRows(); i++) {
                std::string rowName = solver->rowName(i);
                newSolver->setRowName(i, rowName);
            }

            delete solver;
            solver = newSolver;

            infinitoSolver = COIN_DBL_MAX;
            tempo_otimizacao = 0.0;
            statusOtimizacao = TipoStatusSolver_Nenhum;

            return true;
        }

        catch (const CoinError &erro) { throw std::invalid_argument("SolverCLP::resetar(): \n" + erro.message()); }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::resetar(): \n" + std::string(erro.what()));
        }
    }

    //Nao eh necessario utilizar o metodo atualizar no CLP
    bool atualizar() {
        return true;
    }

    //Nao existe um metodo semelhante no CLP
    bool setFocoNumerico(const int a_foco_numerico) {
        return true;
    }

    bool setTempoLimite() {
        tempoLimite = COIN_DBL_MAX;

        return true;
    }

    bool setTempoLimite(const double a_tempo_limite) {
        tempoLimite = a_tempo_limite;

        return true;
    }

    double getTempoLimite() {
        return tempoLimite;
    }

    bool setToleranciaOtimalidade(const double a_tolerancia_otimalidade) {
        toleranciaOtimalidade = a_tolerancia_otimalidade;

        return true;
    }

    bool setToleranciaViabilidade(const double a_tolerancia_viabilidade) {
        toleranciaViabilidade = a_tolerancia_viabilidade;

        return true;
    }

    bool setMetodoPadrao(const TipoMetodoSolver a_tipoMetodoSolver) {
        tipoMetodoSolverPadrao = a_tipoMetodoSolver;
        setMetodoPadrao();
        return true;
    }

    bool setMetodoPadrao() {
        setMetodo(tipoMetodoSolverPadrao);
        return true;
    }

    bool setMetodoAuto() {
        setMetodo(TipoMetodoSolver_automatico);
        return true;
    }

    bool setMetodo(const TipoMetodoSolver a_tipoMetodoSolver) {
        try {

            if (a_tipoMetodoSolver == TipoMetodoSolver_concorrente)
                tipoMetodoSolver = TipoMetodoSolver_automatico;

            else
                tipoMetodoSolver = a_tipoMetodoSolver;

            return true;
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument(
                    "SolverCLP::setMetodo(" + getFullString(a_tipoMetodoSolver) + "): \n" + std::string(erro.what()));
        }

        return true;
    }

    //Nao existe um metodo semelhante no CLP
    bool isLimiteInferiorIIS(const int a_posicaoVariavel) {
        fprintf(stderr, "O metodo isLimiteInferiorIIS nao esta implementado para o CLP!\n");
        return false;
    }

    //Nao existe um metodo semelhante no CLP
    bool isLimiteSuperiorIIS(const int a_posicaoVariavel) {
        fprintf(stderr, "O metodo isLimiteSuperiorIIS nao esta implementado para o CLP!\n");
        return false;
    }

    //Nao existe um metodo semelhante no CLP
    bool isRestricaoIIS(const int a_posicaoRestricao) {
        fprintf(stderr, "O metodo isRestricaoIIS nao esta implementado para o CLP!\n");
        return false;
    }

    bool imprimirLP(const std::string a_nomeArquivo) {
        try {
            insereRestricoes();
            solver->writeLp(a_nomeArquivo.c_str());
            return true;
        }

        catch (const CoinError &erro) {
            throw std::invalid_argument("SolverCLP::imprimirLP(" + a_nomeArquivo + "): \n" + erro.message());
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::imprimirLP(" + a_nomeArquivo + "): \n" + std::string(erro.what()));
        }
    }

    bool imprimirMPS(const std::string a_nomeArquivo) {
        try {
            insereRestricoes();
            solver->writeMps(a_nomeArquivo.c_str());
            return true;
        }

        catch (const CoinError &erro) {
            throw std::invalid_argument("SolverCLP::imprimirMPS(" + a_nomeArquivo + "): \n" + erro.message());
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::imprimirMPS(" + a_nomeArquivo + "): \n" + std::string(erro.what()));
        }
    }

    //Nao existe um metodo semelhante no CLP
    bool imprimirILP(const std::string a_nomeArquivo) {
        fprintf(stderr, "O metodo imprimirILP nao esta implementado para o CLP!\n");
        return false;
    }

    //Nao existe um metodo semelhante no CLP
    int calcularIIS() {
        //fprintf(stderr, "O metodo calcularIIS nao esta implementado para o CLP!\n");
        return -1;
    }

    bool exibirNaTela(const bool exibirNaTela) {
        verbose = (int) exibirNaTela;
        return true;
    }

    bool otimizarComScaling() {
        try {
            insereRestricoes();

            if (escalonarModelo_) { //nao faz nada, modelo ja esta escalonado
                return false;
            }

            ClpSimplex *clp = new ClpSimplex();
            const double *varLB = solver->getColLower();
            const double *varUB = solver->getColUpper();
            const double *varObj = solver->getObjCoefficients();
            const CoinBigIndex nz = solver->getNumElements();
            const int numRows = solver->getNumRows();
            CoinPackedMatrix *matrix = new CoinPackedMatrix(*solver->matrix(), 0, 0, solver->matrix()->isColOrdered());
            const int *ridxs = matrix->getIndices();
            const double *rcoefs = matrix->getElements();
            const CoinBigIndex *rstart = matrix->getVectorStarts();
            const int *rlength = matrix->getVectorLengths();
            const double *rLB = solver->getRowLower();
            const double *rUB = solver->getRowUpper();
            const double newMinCoef = getMenorValorCoef();
            const double newMaxCoef = getMaiorValorCoef();

            for (int j = 0; j < solver->getNumCols(); j++) {
                clp->addColumn(0, nullptr, nullptr, varLB[j], varUB[j], varObj[j]);
                std::string colName = solver->getColumnName(j);
                clp->setColumnName(j, colName);

                if (solver->isInteger(j)) {
                    clp->setInteger(j);
                } else {
                    clp->setContinuous(j);
                }
            }

            int *rowStart = (int *) xmalloc(sizeof(int) * (numRows + 1));
            CoinBigIndex *rowIdxs = (CoinBigIndex *) xmalloc(sizeof(CoinBigIndex) * nz);
            double *rowCoefs = (double *) xmalloc(sizeof(double) * nz);
            double *rowLB = (double *) xmalloc(sizeof(double) * numRows);
            double *rowUB = (double *) xmalloc(sizeof(double) * numRows);
            char *scaleRow = (char *) xmalloc(sizeof(char) * numRows);

            for (size_t i = 0; i < origRowIdx.size(); i++) {
                const int rowIdx = origRowIdx[i];

                if (rowIdx == -1) { //restricao removida
                    continue;
                }

                assert(rowIdx >= 0 && rowIdx < solver->getNumRows());

                if (escalonarRestricao_.at(i) == RowScaling::ON) {
                    scaleRow[rowIdx] = 1;
                } else {
                    scaleRow[rowIdx] = 0;
                }
            }

            int numEls = 0;
            for (int i = 0; i < numRows; i++) {
                rowStart[i] = numEls;

                if (!scaleRow[i]) { //nao escalonar restricao
                    memcpy(rowIdxs + numEls, ridxs + rstart[i], sizeof(int) * rlength[i]);
                    memcpy(rowCoefs + numEls, rcoefs + rstart[i], sizeof(double) * rlength[i]);
                    rowLB[i] = rLB[i];
                    rowUB[i] = rUB[i];
                    numEls += rlength[i];
                } else { //escalonar restricao
                    const double rhs = (rLB[i] > -COIN_DBL_MAX) ? rLB[i] : rUB[i];
                    assert(rhs > -COIN_DBL_MAX && rhs < COIN_DBL_MAX);
                    double minCoef = rhs;
                    double maxCoef = rhs;
                    for (int j = rstart[i]; j < rstart[i] + rlength[i]; j++) {
                        minCoef = (std::min)(minCoef, rcoefs[j]);
                        maxCoef = (std::max)(maxCoef, rcoefs[j]);
                    }

                    if (minCoef >= newMinCoef && maxCoef <= newMaxCoef) { //nao eh necessario escalonar
                        memcpy(rowIdxs + numEls, ridxs + rstart[i], sizeof(int) * rlength[i]);
                        memcpy(rowCoefs + numEls, rcoefs + rstart[i], sizeof(double) * rlength[i]);
                        rowLB[i] = rLB[i];
                        rowUB[i] = rUB[i];
                        numEls += rlength[i];
                    } else if (fabs(maxCoef - minCoef) <= 1e-6) { //todos os valores de coefs e rhs sao iguais
                        for (int j = rstart[i]; j < rstart[i] + rlength[i]; j++) {
                            rowIdxs[numEls] = ridxs[j];
                            rowCoefs[numEls] = 1.0;
                            numEls++;
                        }
                        const double newRhs = 1.0;
                        rowLB[i] = (rLB[i] > -COIN_DBL_MAX) ? newRhs : -COIN_DBL_MAX;
                        rowUB[i] = (rUB[i] < COIN_DBL_MAX) ? newRhs : COIN_DBL_MAX;
                    } else { //faz scaling
                        for (int j = rstart[i]; j < rstart[i] + rlength[i]; j++) {
                            const double newCoef =
                                    (((rcoefs[j] - minCoef) / (maxCoef - minCoef)) * (newMaxCoef - newMinCoef)) +
                                    newMinCoef;
                            rowIdxs[numEls] = ridxs[j];
                            rowCoefs[numEls] = newCoef;
                            numEls++;
                        }
                        const double newRhs =
                                (((rhs - minCoef) / (maxCoef - minCoef)) * (newMaxCoef - newMinCoef)) + newMinCoef;
                        rowLB[i] = (rLB[i] > -COIN_DBL_MAX) ? newRhs : -COIN_DBL_MAX;
                        rowUB[i] = (rUB[i] < COIN_DBL_MAX) ? newRhs : COIN_DBL_MAX;
                    }
                }
            }
            rowStart[numRows] = numEls;

            clp->addRows(numRows, rowLB, rowUB, rowStart, rowIdxs, rowCoefs);

            for (int i = 0; i < numRows; i++) {
                std::string rName = solver->getRowName(i);
                clp->setRowName(i, rName);
            }

            free(rowStart);
            free(rowIdxs);
            free(rowCoefs);
            free(rowLB);
            free(rowUB);
            free(scaleRow);
            delete matrix;

            otimizar(clp);

            delete clp;

            return true;
        }

        catch (const CoinError &erro) {
            throw std::invalid_argument("SolverCLP::otimizarComScaling(): \n" + erro.message());
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::otimizarComScaling(): \n" + std::string(erro.what()));
        }
    }

}; // class SolverCLP : public Solver 

static void *xmalloc(const size_t size) {
    void *result = malloc(size);
    if (!result) {
        fprintf(stderr, "No more memory available. Trying to allocate %zu bytes.", size);
        abort();
    }

    return result;
}

static void *xrealloc(void *ptr, const size_t size) {
    void *res = realloc(ptr, size);
    if (!res) {
        fprintf(stderr, "No more memory available. Trying to allocate %zu bytes in CoinCliqueList", size);
        abort();
    }

    return res;
}

#endif