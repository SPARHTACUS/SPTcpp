#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <string>
#include <chrono>
#include <stdexcept>
#include <cassert>
#include "C_SmartEnum.h"

class ColDebug {
private:
    std::vector<std::string> changes; //format: "name;bound type('lb' or 'ub');value
public:
    ColDebug() {
    }

    ~ColDebug() {
    }

    void changeLB(const std::string &varName, double prevLB, double newLB) {
        changes.emplace_back(varName + ";lb;" + std::to_string(prevLB) + ";" + std::to_string(newLB));
    }

    void changeUB(const std::string &varName, double prevUB, double newUB) {
        changes.emplace_back(varName + ";ub;" + std::to_string(prevUB) + ";" + std::to_string(newUB));
    }

    void writeLog(std::ofstream &output) {
        output << "varName;boundType;previousValue;newValue" << std::endl;

        for (const std::string &chg : changes) {
            output << chg << std::endl;
        }
    }

    void clear() {
        changes.clear();
    }
};

class Solver {
protected:
    /* dados solucao */
    TipoStatusSolver statusOtimizacao;
    double tempo_otimizacao;
    double objValue;
    double bestPossibleObjValue;
    double *dualPrice = nullptr;
    double *varX = nullptr; //solucao
    double *varRC = nullptr; //custo reduzido
    double dualObjValue;

    /* debug */
    std::string ultimaVar, ultimaConstr;
    ColDebug colDbg;

public:
    Solver() {
    }

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

    virtual int addConstrIgual(const std::string a_nome) = 0;

    virtual int addConstrMaior(const std::string a_nome) = 0;
    virtual int addConstrsMaior(const std::vector<std::string> &a_nomes) = 0;

    virtual int addConstrMenor(const std::string a_nome) = 0;

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
    virtual std::string getNomeRestricao(const int a_posicaoRestricao) = 0;

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

    virtual int otimizarComTratamentoInviabilidade() = 0;

    virtual bool atualizar() = 0;

    virtual bool setMetodoPadrao(const TipoMetodoSolver a_tipoMetodoSolver) = 0;

    virtual bool setMetodoPadrao() = 0;

    virtual bool setMetodo(const TipoMetodoSolver a_tipoMetodoSolver) = 0;

    virtual bool setMetodoAuto() = 0;

    virtual bool setFocoNumerico(const int a_foco_numerico) = 0;

    virtual bool setEscalonamento(const int a_escalonamento) = 0;
    virtual int getEscalonamento() const = 0;

    virtual bool setToleranciaOtimalidade(const double a_tolerancia_otimalidade) = 0;

    virtual bool setToleranciaViabilidade(const double a_tolerancia_viabilidade) = 0;

    virtual double getToleranciaOtimalidade() const = 0;

    virtual double getToleranciaViabilidade() const = 0;

    virtual bool setTempoLimite() = 0;

    virtual bool setTempoLimite(const double a_tempo_limite) = 0;

    virtual double getTempoLimite() = 0;

    virtual bool calcularIIS() = 0;

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

    std::string getEstadoUltimaViabilidade() const {
        return "ultima variavel adicionada: " + ultimaVar + "\tultima restricao adicionada: " + ultimaConstr;
    }

    double getDualObjValue() const {
        testeObjPrimalDual(getToleranciaOtimalidade());
        return dualObjValue;
    }

    void testeObjPrimalDual(double tolerance) const {
        const double gap = fabs(dualObjValue - objValue) / objValue;

        if (gap > tolerance) {
            char msg[512];
            sprintf(msg, "testeObjPrimalDual falhou!\nobj primal: %.12lf obj dual: %.12lf gap: %.12lf\n", objValue, dualObjValue, gap);
            throw std::invalid_argument(msg);
        }
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

            statusOtimizacao = TipoStatusSolver_Nenhum;
            objValue = bestPossibleObjValue = dualObjValue = GRB_INFINITY;

            tempo_otimizacao = 0.0;

            setMetodoPadrao();

            exibirNaTela(false);

            ultimaVar = ultimaConstr = "";

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

    double calculateDualObjValue(const GRBModel *model) {
        double dualObjPos = 0.0, dualObjNeg = 0.0;
        const int numVars = model->get(GRB_IntAttr_NumVars);
        const int numRows = model->get(GRB_IntAttr_NumConstrs);

        for (int j = 0; j < numVars; j++) {
            const double reducedCost = vetorGRBVar.at(j).get(GRB_DoubleAttr_RC);

            if (reducedCost > 0.0) {
                const double colLB = vetorGRBVar.at(j).get(GRB_DoubleAttr_LB);

                if (colLB > -infinitoSolver) {
                    const double result = reducedCost * colLB;

                    if (result < 0.0) {
                        dualObjNeg += result;
                    } else {
                        dualObjPos += result;
                    }
                }
            } else if (reducedCost < 0.0) {
                const double colUB = vetorGRBVar.at(j).get(GRB_DoubleAttr_UB);

                if (colUB < infinitoSolver) {
                    const double result = reducedCost * colUB;

                    if (result < 0.0) {
                        dualObjNeg += result;
                    } else {
                        dualObjPos += result;
                    }
                }
            }
        }

        for (int i = 0; i < numRows; i++) {
            const double rowPrice = vetorGRBConstr.at(i).get(GRB_DoubleAttr_Pi);
            const double rowRHS = vetorGRBConstr.at(i).get(GRB_DoubleAttr_RHS);
            const double result = rowPrice * rowRHS;

            if (result < 0.0) {
                dualObjNeg += result;
            } else {
                dualObjPos += result;
            }
        }

        return dualObjNeg + dualObjPos;
    }

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

    bool isNomeSimplificado() { return true; };

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
                    "SolverGRB::setCofObjetivo(" + std::to_string(a_posicao) + "," + std::to_string(a_cofObjetivo) +
                    "): \n" + std::string(erro.getMessage()) + " " + std::to_string(erro.getErrorCode()));
        }

        catch (const std::exception &erro) {
            throw std::invalid_argument(
                    "SolverGRB::setCofObjetivo(" + std::to_string(a_posicao) + "," + std::to_string(a_cofObjetivo) +
                    "): \n" + std::string(erro.what()));
        }
    }; // bool setCofObjetivo(int a_posicao, double a_cofObjetivo) {


    bool setLimInferior(const int a_posicao, const double a_limInferior) {
        try {
            ptrModelo->update();
            const std::string varName = vetorGRBVar.at(a_posicao).get(GRB_StringAttr_VarName);
            const double prevLB = vetorGRBVar.at(a_posicao).get(GRB_DoubleAttr_LB);

            if (a_limInferior <= -infinitoSolver) {
                colDbg.changeLB(varName, prevLB, -GRB_INFINITY);
                vetorGRBVar.at(a_posicao).set(GRB_DoubleAttr_LB, -GRB_INFINITY);
            } else {
                colDbg.changeLB(varName, prevLB, a_limInferior);
                vetorGRBVar.at(a_posicao).set(GRB_DoubleAttr_LB, a_limInferior);
            }

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
            ptrModelo->update();
            const std::string varName = vetorGRBVar.at(a_posicao).get(GRB_StringAttr_VarName);
            const double prevUB = vetorGRBVar.at(a_posicao).get(GRB_DoubleAttr_UB);

            if (a_limSuperior >= infinitoSolver) {
                colDbg.changeUB(varName, prevUB, GRB_INFINITY);
                vetorGRBVar.at(a_posicao).set(GRB_DoubleAttr_UB, GRB_INFINITY);
            } else {
                colDbg.changeUB(varName, prevUB, a_limSuperior);
                vetorGRBVar.at(a_posicao).set(GRB_DoubleAttr_UB, a_limSuperior);
            }
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

    int addConstrIgual(const std::string a_nome) {
        try {
            vetorGRBConstr.push_back(ptrModelo->addConstr(GRBLinExpr(0), GRB_EQUAL, 0.0, a_nome));
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


    int addConstrMaior(const std::string a_nome) {
        try {
            vetorGRBConstr.push_back(ptrModelo->addConstr(GRBLinExpr(0), GRB_GREATER_EQUAL, 0.0, a_nome));
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

    int addConstrsMaior(const std::vector<std::string> &a_nomes) {
        try {
            for (int i = 0; i < int(a_nomes.size()); i++)
                addConstrMaior(a_nomes.at(i));
            return int(vetorGRBConstr.size()) - 1;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument(
                "SolverGRB::addConstrsMaior(): \n" + std::string(erro.getMessage()) + " " +
                std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception& erro) {
            throw std::invalid_argument("SolverGRB::addConstrsMaior(): \n" + std::string(erro.what()));
        }
    }; // bool addConstrMaior(std::string a_nome) {


    int addConstrMenor(const std::string a_nome) {
        try {
            vetorGRBConstr.push_back(ptrModelo->addConstr(GRBLinExpr(0), GRB_LESS_EQUAL, 0.0, a_nome));
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

    std::string getNomeRestricao(const int a_posicaoRestricao) {
        try {
            return vetorGRBConstr.at(a_posicaoRestricao).get(GRB_StringAttr_ConstrName);
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument(
                    "SolverGRB::getNomeRestricao(" + std::to_string(a_posicaoRestricao) + "): \n" +
                    std::string(erro.what()));
        }
    }

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
                for (int i : lista_variavel_dinamica) {
                    if (i == a_posicaoVariavel)
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
            for (int i : lista_variavel_dinamica) {
                const double reduced_cost = getReducedCost(i);
                if (reduced_cost > 0.0) {
                    const double limite_inferior = getLimInferior(i);
                    if (limite_inferior > 0.0)
                        valor_integrado_positivo += reduced_cost * limite_inferior;
                    else if (limite_inferior < 0.0)
                        valor_integrado_negativo += reduced_cost * limite_inferior;
                } else if (reduced_cost < 0.0) {
                    const double limite_superior = getLimInferior(i);
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
                dualObjValue = 0.0;
                for (int j = 0; j < numVars; j++) {
                    varX[j] = model->getVar(j).get(GRB_DoubleAttr_X);
                    varRC[j] = 0.0;
                }
                std::fill(dualPrice, dualPrice + numRows, 0.0);
            } else {
                dualObjValue = calculateDualObjValue(model);
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
            dualObjValue = calculateDualObjValue(model);

            if (model->get(GRB_IntAttr_IsMIP) == 1) {
                dualObjValue = 0.0;
                for (int j = 0; j < numVars; j++) {
                    varX[j] = model->getVar(j).get(GRB_DoubleAttr_X);
                    varRC[j] = 0.0;
                }
                std::fill(dualPrice, dualPrice + numRows, 0.0);
            } else {
                dualObjValue = calculateDualObjValue(model);
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
            objValue = bestPossibleObjValue = dualObjValue = GRB_INFINITY;
            std::fill(varX, varX + model->get(GRB_IntAttr_NumVars), NAN);
            std::fill(varRC, varRC + model->get(GRB_IntAttr_NumVars), NAN);
            std::fill(dualPrice, dualPrice + model->get(GRB_IntAttr_NumConstrs), NAN);

            //DEBUG
            std::ofstream output("debug-status-invalido.txt", std::ios_base::out);
            if (!output.is_open()) {
                throw std::invalid_argument("Nao foi possivel abrir o arquivo debug-status-invalido.txt");
            }
            output << getEstadoUltimaViabilidade() << std::endl;
            colDbg.writeLog(output);
            output.close();
        }
    }

    bool otimizar(GRBModel *model) {
        try {
            auto start_clock = std::chrono::high_resolution_clock::now();
            model->update();

            //DEBUG
            ultimaVar = ptrModelo->getVar(model->get(GRB_IntAttr_NumVars) - 1).get(GRB_StringAttr_VarName);
            ultimaConstr = ptrModelo->getConstr(model->get(GRB_IntAttr_NumConstrs) - 1).get(GRB_StringAttr_ConstrName);

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

            colDbg.clear();

            return true;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument(
                    "SolverGRB::otimizar(GRBModel *model): \n" + std::string(erro.getMessage()) + " " +
                    std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::otimizar(GRBModel *model): \n" + std::string(erro.what()));
        }
    }; // bool otimizar() {

    bool otimizar() {
        return otimizar(ptrModelo);
    }

    int otimizarComTratamentoInviabilidade() {
        try {
            if (ptrModelo == nullptr) {
                throw std::invalid_argument("Solver invalido (NULO).");
            }

            int level = 3;
            while (level > 0) {
                try {
                    if (level == 3) {
                        resetar();
                        otimizar();

                        if (statusOtimizacao == TipoStatusSolver_otimalidade) {
                            return level;
                        }
                    }
                    else if (level == 2) {
                        const double toleranciaAntiga = getToleranciaViabilidade();
                        double novaTolerancia;

                        if (toleranciaAntiga >= 1e-8 || toleranciaAntiga <= 1e-10) {
                            novaTolerancia = 1e-9;
                        }
                        else {
                            novaTolerancia = 1e-6;
                        }

                        resetar();
                        setToleranciaOtimalidade(novaTolerancia);
                        setToleranciaViabilidade(novaTolerancia);
                        otimizar();
                        setToleranciaOtimalidade(toleranciaAntiga);
                        setToleranciaViabilidade(toleranciaAntiga);

                        if (statusOtimizacao == TipoStatusSolver_otimalidade) {
                            return level;
                        }
                    }
                    else {
                        assert(level == 1);
                        const TipoMetodoSolver metodoAntigo = tipoMetodoSolverPadrao;
                        const TipoMetodoSolver metodoNovo = (metodoAntigo == TipoMetodoSolver_dual_simplex) ? TipoMetodoSolver_primal_simplex : TipoMetodoSolver_dual_simplex;

                        resetar();
                        setMetodo(metodoNovo);
                        otimizar();
                        setMetodo(metodoAntigo);

                        if (statusOtimizacao == TipoStatusSolver_otimalidade) {
                            return level;
                        }
                    }

                    level--;
                }
                catch (const std::exception& erro) {
                    const std::string erro_str = std::string(erro.what());
                    if (erro_str.size() > 5) {
                        if (erro_str.find("10005") != std::string::npos)
                            return -1;
                    }
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

    bool setEscalonamento(const int a_escalonamento) {
        try {
            return false;
        }
        catch (const GRBException erro) {
            throw std::invalid_argument("SolverGRB::setEscalonamento(): \n" + std::string(erro.getMessage()) + " " +
                std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception& erro) {
            throw std::invalid_argument("SolverGRB::setEscalonamento(): \n" + std::string(erro.what()));
        }
    }; // bool setEscalonamento() {

    int getEscalonamento() const {
        return -1;
    }

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
    }; // bool setToleranciaOtimalidade() {

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
    }; // bool setToleranciaViabilidade() {

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
            if (calcularIIS()) {
                ptrModelo->write(std::string(a_nomeArquivo + ".ilp"));
                return true;
            }
            return false;
        } // try {
        catch (const GRBException erro) {
            return false;
            throw std::invalid_argument(
                    "SolverGRB::imprimirILP(" + a_nomeArquivo + "): \n" + std::string(erro.getMessage()) + " " +
                    std::to_string(erro.getErrorCode()));
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverGRB::imprimirILP(" + a_nomeArquivo + "): \n" + std::string(erro.what()));
        }
    }; // bool imprimirILP(std::string a_nomeArquivo) {

    bool calcularIIS() {
        try {
            ptrModelo->computeIIS();
            return true;
        } // try {
        catch (const GRBException erro) {
            return false;
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

#define CAP_INI_VAR 512     //capacidade inicial para inserir variaveis
#define CAP_INI_CONSTR 512     //capacidade inicial para inserir restricoes
#define CAP_INI_POR_CONSTR 256 //capacidade inicial para inserir idxs e coefs em cada restricao

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

    int fakeNumCols;
    int realNumCols;
    std::vector<int> origColIdx;

    /* buffer de variaveis */
    char** nomeCol = nullptr; //nomes das variaveis
    int*    binCol = nullptr; //binária
    double* lbCol = nullptr; //limite inferior
    double* ubCol = nullptr; //limite superior
    double* objCol = nullptr; //limite superior

    int* fakeIdxCol = nullptr; //indice da variavel (nao considera remocao)
    int capBufferCols = 0; //capcidade de variaveis
    int sizeBufferCols = 0; //numero atual de variaveis; nao considera remocao

    /* parametros do solver */
    double infinitoSolver = COIN_DBL_MAX;
    double tempoLimite = COIN_DBL_MAX;
    TipoMetodoSolver tipoMetodoSolver;
    TipoMetodoSolver tipoMetodoSolverPadrao = TipoMetodoSolver_dual_simplex;
    int verbose;
    int escalonamento = 1; //0 off, 1 equilibrium, 2 geometric, 3 auto, 4 auto-but-as-initialSolve-in-bab

    /* lista de variaveis dinamicas */
    std::vector<int> lista_variavel_dinamica;

    void inicializaMemRestricoes() {
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

    void inicializaMemVariaveis() {
        sizeBufferCols = 0;
        capBufferCols = CAP_INI_VAR;
        nomeCol = (char**)xmalloc(sizeof(char*) * capBufferCols);
        binCol = (int*)xmalloc(sizeof(double) * capBufferCols);
        lbCol = (double*)xmalloc(sizeof(double) * capBufferCols);
        ubCol = (double*)xmalloc(sizeof(double) * capBufferCols);
        objCol = (double*)xmalloc(sizeof(double) * capBufferCols);
        fakeIdxCol = (int*)xmalloc(sizeof(int) * capBufferCols);

        for (int i = 0; i < capBufferCols; i++) {
            binCol[i] = 0;
            nomeCol[i] = nullptr;
        }
       
    }

    void realocaMemDaRestricao(int pos) {
        assert(pos >= 0 && pos < sizeBufferRows);
        const int capAntiga = capRow[pos];
        capRow[pos] = (std::max)(CAP_INI_POR_CONSTR, (int) ceil(capAntiga * 1.2));
        matrizIdxs[pos] = (int *) xrealloc(matrizIdxs[pos], sizeof(int) * capRow[pos]);
        matrizCoefs[pos] = (double *) xrealloc(matrizCoefs[pos], sizeof(double) * capRow[pos]);
    }

    void realocaMemRestricoes() {
        realocaMemRestricoes(1);
    }

    void realocaMemRestricoes(const int a_numConstrs) {
        const int capAntiga = capBufferRows;

        if (a_numConstrs == 1)
            capBufferRows = (std::max)(CAP_INI_CONSTR, (int)ceil(capAntiga * 1.2));
        else
            capBufferRows += a_numConstrs;

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

    void realocaMemVariaveis() {
        realocaMemVariaveis(1);
    }

    void realocaMemVariaveis(const int a_numVars) {
        const int capAntiga = capBufferCols;

        if (a_numVars == 1)
            capBufferCols = (std::max)(CAP_INI_VAR, (int)ceil(capAntiga * 1.2));
        else
            capBufferCols += a_numVars;

        nomeCol = (char**)xrealloc(nomeCol, sizeof(char*) * capBufferCols);
        binCol = (int*)xrealloc(binCol, sizeof(int) * capBufferCols);
        lbCol = (double*)xrealloc(lbCol, sizeof(double) * capBufferCols);
        ubCol = (double*)xrealloc(ubCol, sizeof(double) * capBufferCols);
        objCol = (double*)xrealloc(objCol, sizeof(double) * capBufferCols);
        fakeIdxCol = (int*)xrealloc(fakeIdxCol, sizeof(int) * capBufferCols);

        for (int i = capAntiga; i < capBufferCols; i++) {
            binCol[i] = 0;
            nomeCol[i] = nullptr;
        }
        
    }

    void esvaziaMemRestricoes() {
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

    void esvaziaMemVariaveis() {
        if (capBufferCols > 0) {
            for (int i = 0; i < capBufferCols; i++){
                free(nomeCol[i]);
                nomeCol[i] = nullptr;
            }
            sizeBufferCols = 0;
            capBufferCols = 0;
            free(nomeCol);
            nomeCol = nullptr;
            free(lbCol);
            binCol = nullptr;
            free(binCol);
            lbCol = nullptr;
            free(ubCol);
            ubCol = nullptr;
            free(objCol);
            objCol = nullptr;
            free(fakeIdxCol);
            fakeIdxCol = nullptr;
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
            esvaziaMemRestricoes();
            inicializaMemRestricoes();
        }

        free(rowStarts);
        free(rowIdxs);
        free(rowCoefs);
        free(rowLB);
        free(rowUB);
    }

    void insereVariaveis() {
        if (sizeBufferCols == 0) {
            return;
        }

        int* colStarts = (int*)xmalloc(sizeof(int) * (sizeBufferCols + 1));
        std::vector<std::string> colNames(sizeBufferCols);

        int contVars = 0;
        for (int i = 0; i < sizeBufferCols; i++) {
            colStarts[i] = contVars;
            colNames[i] = nomeCol[i];

            contVars++;
        }
        colStarts[sizeBufferCols] = contVars;

        const int numColsAnterior = solver->getNumCols();
        solver->addColumns(sizeBufferCols, lbCol, ubCol, objCol, colStarts, nullptr, nullptr);

        for (int i = 0; i < sizeBufferCols; i++) {
            solver->setColumnName(i + numColsAnterior, colNames[i]);
            if (binCol[i] == 0) {
                solver->setContinuous(i + numColsAnterior);
            }
            else if (binCol[i] == 1) {
                solver->setInteger(i + numColsAnterior);
                solver->setColBounds(i + numColsAnterior, 0.0, 1.0);
            }
        }

        sizeBufferCols = 0;

        if (capBufferCols > CAP_INI_VAR) {
            esvaziaMemVariaveis();
            inicializaMemVariaveis();
        }

        free(colStarts);
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
            clp->setLogLevel(verbose);
            clp->scaling(escalonamento);
            clp->setPerturbation(50);
            clp->setMaximumSeconds(tempoLimite);
            clp->setMaximumWallSeconds(tempoLimite);

            if (tipoMetodoSolver == TipoMetodoSolver_automatico) {
                clp->initialSolve();
            } else if (tipoMetodoSolver == TipoMetodoSolver_primal_simplex) {
                clp->initialPrimalSolve();
            } else if (tipoMetodoSolver == TipoMetodoSolver_dual_simplex) {
                clp->initialDualSolve();
            } else if (tipoMetodoSolver == TipoMetodoSolver_barrier) {
                clp->initialBarrierNoCrossSolve();
            }

            const int status = clp->status();
            const int secondaryStatus = clp->secondaryStatus();

            if (clp->isProvenOptimal()) {
                return 0;
            }
            if (clp->isIterationLimitReached() || (status == 3 && secondaryStatus == 9)) {
                return 1;
            }
            if (clp->isProvenPrimalInfeasible()) {
                return 2;
            }

            //DEBUG
            std::ofstream output("debug-status-invalido.txt", std::ios_base::out);
            if (!output.is_open()) {
                throw std::invalid_argument("Nao foi possivel abrir o arquivo debug-status-invalido.txt");
            }
            output << getEstadoUltimaViabilidade() << std::endl;
            colDbg.writeLog(output);
            output.close();

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

            if (res == 2) { //problema infactivel
                objValue = bestPossibleObjValue = dualObjValue = COIN_DBL_MAX;
                std::fill(varX, varX + clp->getNumCols(), NAN);
                std::fill(varRC, varRC + clp->getNumCols(), NAN);
                std::fill(dualPrice, dualPrice + clp->getNumRows(), NAN);
                statusOtimizacao = TipoStatusSolver_nao_otimalidade;

                //DEBUG
                std::ofstream output("debug-status-infactivel.txt", std::ios_base::out);
                if (!output.is_open()) {
                    throw std::invalid_argument("Nao foi possivel abrir o arquivo debug-status-infactivel.txt");
                }
                output << getEstadoUltimaViabilidade() << std::endl;
                colDbg.writeLog(output);
                output.close();
                //throw std::invalid_argument("CLP status: modelo infactivel!");
                return;
            } else if (res == 1) { // tempo limite excedido
                objValue = bestPossibleObjValue = clp->getObjValue();
                dualObjValue = calculateDualObjValue(clp);
                const double *x = clp->getColSolution();
                memcpy(varX, x, sizeof(double) * clp->getNumCols());
                const double *rc = clp->getReducedCost();
                memcpy(varRC, rc, sizeof(double) * clp->getNumCols());
                const double *dp = clp->getRowPrice();
                memcpy(dualPrice, dp, sizeof(double) * clp->getNumRows());
                statusOtimizacao = TipoStatusSolver_tempo_excedido;
                return;
            }

            if (!isMILP) { //CLP resolveu na otimalidade e o problema contem somente vars continuas
                assert(res == 0);
                objValue = bestPossibleObjValue = clp->getObjValue();
                dualObjValue = calculateDualObjValue(clp);
                const double *x = clp->getColSolution();
                memcpy(varX, x, sizeof(double) * clp->getNumCols());
                const double *rc = clp->getReducedCost();
                memcpy(varRC, rc, sizeof(double) * clp->getNumCols());
                const double *dp = clp->getRowPrice();
                memcpy(dualPrice, dp, sizeof(double) * clp->getNumRows());
                statusOtimizacao = TipoStatusSolver_otimalidade;
                return;
            }

            OsiClpSolverInterface linearProgram(clp);
            CbcModel cbcModel(linearProgram);
            CbcSolverUsefulData cbcData;
            double paramAllowableGap = 1e-10, paramAllowableFractionGap = 0.0001, paramIntegerTolerance = 1e-6;
            int paramRoundIntVars = 1, paramRandomSeed = 1, paramUseElapsedTime = 1;

            cbcData.disableWelcomePrinting();
            CbcMain0(cbcModel, cbcData);
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
                objValue = bestPossibleObjValue = dualObjValue = COIN_DBL_MAX;
                std::fill(varX, varX + clp->getNumCols(), NAN);
                std::fill(varRC, varRC + clp->getNumCols(), NAN);
                std::fill(dualPrice, dualPrice + clp->getNumRows(), NAN);
                statusOtimizacao = TipoStatusSolver_nao_otimalidade;

                //DEBUG
                std::ofstream output("debug-status-abandoned.txt", std::ios_base::out);
                if (!output.is_open()) {
                    throw std::invalid_argument("Nao foi possivel abrir o arquivo debug-status-abandoned.txt");
                }
                output << getEstadoUltimaViabilidade() << std::endl;
                colDbg.writeLog(output);
                output.close();
                throw std::invalid_argument("10005 CBC status: abandoned - Instabilidade numerica!");
            } else if (cbcModel.isProvenInfeasible()) {
                objValue = bestPossibleObjValue = dualObjValue = COIN_DBL_MAX;
                std::fill(varX, varX + clp->getNumCols(), NAN);
                std::fill(varRC, varRC + clp->getNumCols(), NAN);
                std::fill(dualPrice, dualPrice + clp->getNumRows(), NAN);
                statusOtimizacao = TipoStatusSolver_nao_otimalidade;

                //DEBUG
                std::ofstream output("debug-status-infactivel.txt", std::ios_base::out);
                if (!output.is_open()) {
                    throw std::invalid_argument("Nao foi possivel abrir o arquivo debug-status-infactivel.txt");
                }
                output << getEstadoUltimaViabilidade() << std::endl;
                colDbg.writeLog(output);
                output.close();
//                throw std::invalid_argument("CBC status: infactivel!");
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
                    dualObjValue = calculateDualObjValue(clp);
                    const double *x = cbcModel.bestSolution();
                    memcpy(varX, x, sizeof(double) * cbcModel.getNumCols());
                    const double *rc = cbcModel.getReducedCost();
                    memcpy(varRC, rc, sizeof(double) * cbcModel.getNumCols());
                    const double *dp = cbcModel.getRowPrice();
                    memcpy(dualPrice, dp, sizeof(double) * cbcModel.getNumRows());
                } else {
                    objValue = dualObjValue = COIN_DBL_MAX;
                    std::fill(varX, varX + clp->getNumCols(), NAN);
                    std::fill(varRC, varRC + clp->getNumCols(), NAN);
                    std::fill(dualPrice, dualPrice + clp->getNumRows(), NAN);
                }
            } else {
                statusOtimizacao = TipoStatusSolver_nao_otimalidade;
                //cbcModel.solver()->writeMps("erro_solve-invalido");
               // cbcModel.solver()->writeLp("erro_solve-invalido");

                //DEBUG
                std::ofstream output("debug-status-invalido.txt", std::ios_base::out);
                if (!output.is_open()) {
                    throw std::invalid_argument("Nao foi possivel abrir o arquivo debug-status-invalido.txt");
                }
                output << getEstadoUltimaViabilidade() << std::endl;
                colDbg.writeLog(output);
                output.close();

                throw std::invalid_argument(
                    "10005 CBC retornou um status invalido: primary = " + std::to_string(cbcModel.status()) + " secondary = " +
                    std::to_string(cbcModel.secondaryStatus()));
            }
        }

        catch (const CoinError &erro) { throw std::invalid_argument("SolverCLP::otimiza(): \n" + erro.message()); }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::otimiza(): \n" + std::string(erro.what()));
        }
    }

    double calculateDualObjValue(const ClpSimplex *clp) {
        double dualObjPos = 0.0, dualObjNeg = 0.0;
        const double *rowPrice = clp->getRowPrice();
        const double *rowLB = clp->getRowLower();
        const double *rowUB = clp->getRowUpper();
        const double *reducedCost = clp->getReducedCost();
        const double *colLB = clp->getColLower();
        const double *colUB = clp->getColUpper();

        for (int j = 0; j < clp->getNumCols(); j++) {
            if (reducedCost[j] > 0.0) {
                if (colLB[j] > -infinitoSolver) {
                    const double result = reducedCost[j] * colLB[j];

                    if (result < 0.0) {
                        dualObjNeg += result;
                    } else {
                        dualObjPos += result;
                    }
                }
            }
            else if (reducedCost[j] < 0.0) {
                if (colUB[j] < infinitoSolver) {
                    const double result = reducedCost[j] * colUB[j];

                    if (result < 0.0) {
                        dualObjNeg += result;
                    } else {
                        dualObjPos += result;
                    }
                }
            }
        }

        for (int i = 0; i < clp->getNumRows(); i++) {
            if (rowPrice[i] > 0.0) {
                if (rowLB[i] > -infinitoSolver) {
                    const double result = rowPrice[i] * rowLB[i];

                    if (result < 0.0) {
                        dualObjNeg += result;
                    } else {
                        dualObjPos += result;
                    }
                }
            } else if (rowPrice[i] < 0.0) {
                if (rowUB[i] < infinitoSolver) {
                    const double result = rowPrice[i] * rowUB[i];

                    if (result < 0.0) {
                        dualObjNeg += result;
                    } else {
                        dualObjPos += result;
                    }
                }
            }
        }

        return dualObjNeg + dualObjPos;
    }

    bool inicializar() {
        try {
            if (solver != nullptr) {
                delete solver;
                solver = nullptr;
            }

            esvaziaMemVariaveis();
            esvaziaMemRestricoes();

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
            dualObjValue = COIN_DBL_MAX;

            realNumRows = fakeNumRows = 0;
            realNumCols = fakeNumCols = 0;
            
            tempo_otimizacao = 0.0;
            isMILP = false;
            escalonamento = 1;
            statusOtimizacao = TipoStatusSolver_Nenhum;
            lista_variavel_dinamica = std::vector<int>();
            
            origColIdx.clear();
            origRowIdx.clear();
            origRowSense.clear();
            
            tempoLimite = COIN_DBL_MAX;
            objValue = COIN_DBL_MAX;
            bestPossibleObjValue = COIN_DBL_MAX;
            tipoMetodoSolverPadrao = TipoMetodoSolver_dual_simplex;
            setMetodoPadrao();
            exibirNaTela(false);
            
            inicializaMemVariaveis();
            inicializaMemRestricoes();
            
            ultimaVar = ultimaConstr = "";

            return true;
        }

        catch (const CoinError &erro) {
            throw std::invalid_argument("SolverCLP::inicializar(): \n" + erro.message());
        }
        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::inicializar(): \n" + std::string(erro.what()));
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

        esvaziaMemVariaveis();
        esvaziaMemRestricoes();
    }

    std::string str() { return "CLP"; }

    bool isNomeSimplificado() { return true; }

    int addVar_old(const double a_lb, const double a_ub, const double a_obj, const std::string a_nome) {
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


    int addVar(const double a_lb, const double a_ub, const double a_obj, const std::string a_nome) {
        try {
            std::string nomeVariavel = a_nome;
            remCharsInvalidos(nomeVariavel);

            if (sizeBufferCols + 1 > capBufferCols) {
                realocaMemVariaveis();
            }

            if (nomeCol[sizeBufferCols] == nullptr) {
                nomeCol[sizeBufferCols] = (char*)xmalloc(sizeof(char) * (nomeVariavel.size() + 1));
            }
            else if (strlen(nomeCol[sizeBufferCols]) < nomeVariavel.size() + 1) {
                free(nomeCol[sizeBufferCols]);
                nomeCol[sizeBufferCols] = (char*)xmalloc(sizeof(char) * (nomeVariavel.size() + 1));
            }

            strncpy(nomeCol[sizeBufferCols], nomeVariavel.c_str(), nomeVariavel.size());
            nomeCol[sizeBufferCols][nomeVariavel.size()] = '\0';
            
            fakeIdxCol[sizeBufferCols] = fakeNumCols;

            lbCol[sizeBufferCols] = a_lb;
            ubCol[sizeBufferCols] = a_ub;
            objCol[sizeBufferCols] = a_obj;
            
            sizeBufferCols++;
            origColIdx.push_back(realNumCols);

            fakeNumCols++;
            realNumCols++;

            return (fakeNumCols - 1);
        }

        catch (const std::exception& erro) {
            throw std::invalid_argument("SolverCLP::addVar(" + std::to_string(a_lb) + "," + std::to_string(a_ub) + "," +
                std::to_string(a_obj) + "," + a_nome + "): \n" + std::string(erro.what()));
        }
    }


    bool setCofObjetivo_old(const int a_posicao, const double a_cofObjetivo) {
        try {
            solver->setObjCoeff(a_posicao, a_cofObjetivo);
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


    bool setCofObjetivo(const int a_posicao, const double a_cofObjetivo) {
        try {

            const int colIdx = origColIdx.at(a_posicao);
            if (colIdx == -1) {//restricao ja foi removida do modelo
                throw std::invalid_argument(
                    "Vaiavel de indice [" + std::to_string(a_posicao) + "] foi removida do modelo.");
            }

            if (colIdx >= solver->getNumCols()) { //restricao esta no buffer; ainda nao foi inserida no modelo
                const int idxBuffer = colIdx - solver->getNumCols();
                objCol[idxBuffer] = a_cofObjetivo;
            }
            else { //restricao ja foi inserida no solver

                solver->setObjCoeff(colIdx, a_cofObjetivo);
            }

            return true;
        }

        catch (const CoinError& erro) {
            throw std::invalid_argument(
                "SolverCLP::setCofObjetivo(" + std::to_string(a_posicao) + "," + std::to_string(a_cofObjetivo) +
                "): \n" + erro.message());
        }
        catch (const std::exception& erro) {
            throw std::invalid_argument(
                "SolverCLP::setCofObjetivo(" + std::to_string(a_posicao) + "," + std::to_string(a_cofObjetivo) +
                "): \n" + std::string(erro.what()));
        }
    }


    bool setLimInferior_old(const int a_posicao, const double a_limInferior) {
        try {
            const double newLB = (fabs(a_limInferior) <= 1e-6) ? 0.0 : a_limInferior;
            colDbg.changeLB(solver->getColumnName(a_posicao), solver->getColLower()[a_posicao], newLB);
            solver->setColLower(a_posicao, newLB);
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


    bool setLimInferior(const int a_posicao, const double a_limInferior) {
        try {

            const int colIdx = origColIdx.at(a_posicao);
            if (colIdx == -1) {//restricao ja foi removida do modelo
                throw std::invalid_argument(
                    "Vaiavel de indice [" + std::to_string(a_posicao) + "] foi removida do modelo.");
            }

            const double newLB = (fabs(a_limInferior) <= 1e-6) ? 0.0 : a_limInferior;

            if (colIdx >= solver->getNumCols()) { //restricao esta no buffer; ainda nao foi inserida no modelo
                const int idxBuffer = colIdx - solver->getNumCols();
                colDbg.changeLB(nomeCol[idxBuffer], lbCol[idxBuffer], newLB);
                lbCol[idxBuffer] = newLB;
            }
            else { //restricao ja foi inserida no solver

                solver->setColLower(colIdx, newLB);
                colDbg.changeLB(solver->getColumnName(a_posicao), solver->getColLower()[a_posicao], newLB);
            }

            return true;
        }

        catch (const CoinError& erro) {
            throw std::invalid_argument(
                "SolverCLP::setLimInferior(" + std::to_string(a_posicao) + "," + std::to_string(a_limInferior) +
                "): \n" + erro.message());
        }
        catch (const std::exception& erro) {
            throw std::invalid_argument(
                "SolverCLP::setLimInferior(" + std::to_string(a_posicao) + "," + std::to_string(a_limInferior) +
                "): \n" + std::string(erro.what()));
        }
    }

    bool setLimSuperior_old(const int a_posicao, const double a_limSuperior) {
        try {
            const double newUB = (fabs(a_limSuperior) <= 1e-6) ? 0.0 : a_limSuperior;
            colDbg.changeUB(solver->getColumnName(a_posicao), solver->getColUpper()[a_posicao], newUB);
            solver->setColUpper(a_posicao, newUB);
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

    bool setLimSuperior(const int a_posicao, const double a_limSuperior) {
        try {

            const int colIdx = origColIdx.at(a_posicao);
            if (colIdx == -1) {//restricao ja foi removida do modelo
                throw std::invalid_argument(
                    "Vaiavel de indice [" + std::to_string(a_posicao) + "] foi removida do modelo.");
            }

            const double newUB = (fabs(a_limSuperior) <= 1e-6) ? 0.0 : a_limSuperior;

            if (colIdx >= solver->getNumCols()) { //restricao esta no buffer; ainda nao foi inserida no modelo
                const int idxBuffer = colIdx - solver->getNumCols();
                colDbg.changeLB(nomeCol[idxBuffer], ubCol[idxBuffer], newUB);
                ubCol[idxBuffer] = newUB;
            }
            else { //restricao ja foi inserida no solver

                solver->setColUpper(colIdx, newUB);
                colDbg.changeLB(solver->getColumnName(a_posicao), solver->getColUpper()[a_posicao], newUB);
            }

            return true;
        }

        catch (const CoinError& erro) {
            throw std::invalid_argument(
                "SolverCLP::setLimSuperior(" + std::to_string(a_posicao) + "," + std::to_string(a_limSuperior) +
                "): \n" + erro.message());
        }
        catch (const std::exception& erro) {
            throw std::invalid_argument(
                "SolverCLP::setLimSuperior(" + std::to_string(a_posicao) + "," + std::to_string(a_limSuperior) +
                "): \n" + std::string(erro.what()));
        }
    }


    bool setTipoVariavelContinua(const int a_posicao) {
        try {

            const int colIdx = origColIdx.at(a_posicao);
            if (colIdx == -1) {//restricao ja foi removida do modelo
                throw std::invalid_argument(
                    "Vaiavel de indice [" + std::to_string(a_posicao) + "] foi removida do modelo.");
            }

            if (colIdx >= solver->getNumCols()) { //restricao esta no buffer; ainda nao foi inserida no modelo
                const int idxBuffer = colIdx - solver->getNumCols();
                binCol[idxBuffer] = 0;
            }
            else { //restricao ja foi inserida no solver
                solver->setContinuous(a_posicao);
            }
           
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


            const int colIdx = origColIdx.at(a_posicao);
            if (colIdx == -1) {//restricao ja foi removida do modelo
                throw std::invalid_argument(
                    "Vaiavel de indice [" + std::to_string(a_posicao) + "] foi removida do modelo.");
            }

            if (colIdx >= solver->getNumCols()) { //restricao esta no buffer; ainda nao foi inserida no modelo
                const int idxBuffer = colIdx - solver->getNumCols();
                binCol[idxBuffer] = 1;
                isMILP = true;
            }
            else { //restricao ja foi inserida no solver
                solver->setInteger(a_posicao);
                solver->setColBounds(a_posicao, 0.0, 1.0);
                isMILP = true;
            }

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

    int addConstrIgual(const std::string a_nome) {
        try {
            std::string nomeRestricao = a_nome;
            remCharsInvalidos(nomeRestricao);

            if (sizeBufferRows + 1 > capBufferRows) {
                realocaMemRestricoes();
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
            fakeNumRows++;
            realNumRows++;

            return (fakeNumRows - 1);
        }

        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::addConstrIgual(" + a_nome + "): \n" + std::string(erro.what()));
        }
    }

    int addConstrMaior(const std::string a_nome) {
        try {
            std::string nomeRestricao = a_nome;
            remCharsInvalidos(nomeRestricao);

            if (sizeBufferRows + 1 > capBufferRows) {
                realocaMemRestricoes();
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
            fakeNumRows++;
            realNumRows++;

            return (fakeNumRows - 1);
        }

        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::addConstrMaior(" + a_nome + "): \n" + std::string(erro.what()));
        }
    }

    int addConstrsMaior(const std::vector<std::string> &a_nomes) {
        try {

            const int numNewConstrs = int(a_nomes.size());

            if (sizeBufferRows + numNewConstrs > capBufferRows) {
                realocaMemRestricoes(numNewConstrs);
            }

            for (int i = 0; i < numNewConstrs; i++) {

                std::string nomeRestricao = a_nomes.at(i);
                remCharsInvalidos(nomeRestricao);

                assert(sizeRow[sizeBufferRows] == 0);

                if (capRow[sizeBufferRows] == 0) {
                    capRow[sizeBufferRows] = CAP_INI_POR_CONSTR;
                    matrizIdxs[sizeBufferRows] = (int*)xmalloc(sizeof(int) * capRow[sizeBufferRows]);
                    matrizCoefs[sizeBufferRows] = (double*)xmalloc(sizeof(double) * capRow[sizeBufferRows]);
                }

                if (nomeRow[sizeBufferRows] == nullptr) {
                    nomeRow[sizeBufferRows] = (char*)xmalloc(sizeof(char) * (nomeRestricao.size() + 1));
                }
                else if (strlen(nomeRow[sizeBufferRows]) < nomeRestricao.size() + 1) {
                    free(nomeRow[sizeBufferRows]);
                    nomeRow[sizeBufferRows] = (char*)xmalloc(sizeof(char) * (nomeRestricao.size() + 1));
                }

                strncpy(nomeRow[sizeBufferRows], nomeRestricao.c_str(), nomeRestricao.size());
                nomeRow[sizeBufferRows][nomeRestricao.size()] = '\0';
                sinalRow[sizeBufferRows] = '>';
                fakeIdxRow[sizeBufferRows] = fakeNumRows;
                rhsRow[sizeBufferRows] = 0.0;
                sizeBufferRows++;
                origRowIdx.push_back(realNumRows);
                origRowSense.push_back('>');
                fakeNumRows++;
                realNumRows++;

            }

            return (fakeNumRows - 1);
        }

        catch (const std::exception& erro) {
            throw std::invalid_argument("SolverCLP::addConstrsMaior(): \n" + std::string(erro.what()));
        }
    }

    int addConstrMenor(const std::string a_nome) {
        try {
            std::string nomeRestricao = a_nome;
            remCharsInvalidos(nomeRestricao);

            if (sizeBufferRows + 1 > capBufferRows) {
                realocaMemRestricoes();
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

    std::string getNomeRestricao(const int a_posicaoRestricao) {
        const int rowIdx = origRowIdx.at(a_posicaoRestricao);
        if (rowIdx == -1) { //restricao ja foi removida do modelo
            throw std::invalid_argument(
                    "Restricao de indice [" + std::to_string(a_posicaoRestricao) + "] foi removida do modelo.");
        }

        if (rowIdx >= solver->getNumRows()) { //restricao esta no buffer; ainda nao foi inserida no modelo
            const int idxBuffer = rowIdx - solver->getNumRows();
            return nomeRow[idxBuffer];
        } else { //restricao ja foi inserida no solver
            return solver->getRowName(rowIdx);
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
                    realocaMemDaRestricao(idxBuffer);
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
            const double newRHS = (fabs(a_RHS) <= 1e-6) ? 0.0 : a_RHS;
            const int rowIdx = origRowIdx.at(a_posicaoRestricao);
            if (rowIdx == -1) {//restricao ja foi removida do modelo
                throw std::invalid_argument(
                        "Restricao de indice [" + std::to_string(a_posicaoRestricao) + "] foi removida do modelo.");
            }

            if (rowIdx >= solver->getNumRows()) { //restricao esta no buffer; ainda nao foi inserida no modelo
                const int idxBuffer = rowIdx - solver->getNumRows();
                rhsRow[idxBuffer] = newRHS;
            } else { //restricao ja foi inserida no solver
                const char rowSense = origRowSense.at(a_posicaoRestricao);

                if (rowSense == '<') {
                    solver->setRowBounds(rowIdx, -COIN_DBL_MAX, newRHS);
                } else if (rowSense == '=') {
                    solver->setRowBounds(rowIdx, newRHS, newRHS);
                } else if (rowSense == '>') {
                    solver->setRowBounds(rowIdx, newRHS, COIN_DBL_MAX);
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

            if (rowIdx >= solver->getNumRows()) { //restricao esta no buffer; ainda nao foi inserida no modelo
                const int idxBuffer = rowIdx - solver->getNumRows();
                return rhsRow[idxBuffer];
            } else { //restricao ja foi inserida no solver
                const char rowSense = origRowSense.at(a_posicaoRestricao);

                if (rowSense == '<' || rowSense == '=') {
                    return solver->getRowUpper()[rowIdx];
                }
                else if (rowSense == '>') {
                    return solver->getRowLower()[rowIdx];
                }
                else {
                    throw std::invalid_argument(
                        "Sinal invalido [" + std::to_string(rowSense) + "] para a restricao de indice " +
                        std::to_string(a_posicaoRestricao));
                }
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

    bool addVarDinamica(const int a_posicao) {
        try {

            const int colIdx = origColIdx.at(a_posicao);
            if (colIdx == -1) {//restricao ja foi removida do modelo
                throw std::invalid_argument(
                    "Vaiavel de indice [" + std::to_string(a_posicao) + "] foi removida do modelo.");
            }

            if (colIdx >= solver->getNumCols()) { //restricao esta no buffer; ainda nao foi inserida no modelo
                const int idxBuffer = colIdx - solver->getNumCols();

                if (a_posicao >= 0 && a_posicao < sizeBufferCols) {
                    for (int vd : lista_variavel_dinamica) {
                        if (vd == a_posicao) {
                            return true; /* variavel dinamica ja inserida anteriormente */
                        }
                    }

                    lista_variavel_dinamica.push_back(a_posicao);
                }
                else {
                    throw std::invalid_argument(
                        "Variavel com indice " + std::to_string(a_posicao) + "nao existe no modelo.");
                }

            }
            else { //restricao ja foi inserida no solver

                if (a_posicao >= 0 && a_posicao < solver->getNumCols()) {
                    for (int vd : lista_variavel_dinamica) {
                        if (vd == a_posicao) {
                            return true; /* variavel dinamica ja inserida anteriormente */
                        }
                    }

                    lista_variavel_dinamica.push_back(a_posicao);
                }
                else {
                    throw std::invalid_argument(
                        "Variavel com indice " + std::to_string(a_posicao) + "nao existe no modelo.");
                }

            }





            return true;
        }

        catch (const std::exception &erro) {
            throw std::invalid_argument("SolverCLP::addVarDinamica(" + std::to_string(a_posicao) + "): \n" +
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
        //DEBUG
        ultimaVar = clp->getColumnName(clp->getNumCols() - 1);
        ultimaConstr = clp->getRowName(clp->getNumRows() - 1);

        auto start_clock = std::chrono::high_resolution_clock::now();
        solve(clp);
        std::chrono::duration<double> tempo = std::chrono::high_resolution_clock::now() - start_clock;
        tempo_otimizacao = tempo.count();

        colDbg.clear();

        return true;
    }

    bool otimizar() {
        insereVariaveis();
        insereRestricoes();
        return otimizar(solver);
    }

    int otimizarComTratamentoInviabilidade() {
        try {
            if (solver == nullptr) {
                throw std::invalid_argument("Solver invalido (NULO).");
            }

            int level = 4;
            while (level > 0) {
                try {
                    if (level == 4) {
                        const double toleranciaAntiga = getToleranciaViabilidade();
                        double novaTolerancia;

                        if (toleranciaAntiga >= 1e-8 || toleranciaAntiga <= 1e-10) {
                            novaTolerancia = 1e-9;
                        }
                        else {
                            novaTolerancia = 1e-6;
                        }

                        resetar();
                        setToleranciaOtimalidade(novaTolerancia);
                        setToleranciaViabilidade(novaTolerancia);
                        otimizar();
                        setToleranciaOtimalidade(toleranciaAntiga);
                        setToleranciaViabilidade(toleranciaAntiga);

                        if (statusOtimizacao == TipoStatusSolver_otimalidade) {
                            return level;
                        }
                    }
                    else if (level == 3) {
                        const int scalingAntigo = escalonamento;
                        const int novoScaling = (scalingAntigo > 0) ? 0 : 1;

                        resetar();
                        escalonamento = novoScaling;
                        otimizar();
                        escalonamento = scalingAntigo;

                        if (statusOtimizacao == TipoStatusSolver_otimalidade) {
                            return level;
                        }
                    }
                    else if (level == 2) {
                        const TipoMetodoSolver metodoAntigo = tipoMetodoSolver;
                        TipoMetodoSolver metodoNovo = (metodoAntigo == TipoMetodoSolver_dual_simplex) ? TipoMetodoSolver_primal_simplex : TipoMetodoSolver_dual_simplex;

                        resetar();
                        tipoMetodoSolver = metodoNovo;
                        otimizar();
                        tipoMetodoSolver = metodoAntigo;

                        if (statusOtimizacao == TipoStatusSolver_otimalidade) {
                            return level;
                        }
                    }
                    else {
                        assert(level == 1);
                        resetar();
                        otimizar();

                        if (statusOtimizacao == TipoStatusSolver_otimalidade) {
                            return level;
                        }
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

                level--;
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

    bool resetar() {
        try {
            insereVariaveis();
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

    bool setFocoNumerico(const int a_foco_numerico) {
        try {
            return true;
        }
        catch (const CoinError erro) {
            throw std::invalid_argument("SolverClp::setFocoNumerico(" + std::to_string(a_foco_numerico) + ": \n" + std::string(erro.message()));
        }
        catch (const std::exception& erro) {
            throw std::invalid_argument("SolverClp::setFocoNumerico(" + std::to_string(a_foco_numerico) + ": \n" + std::string(erro.what()));
        }
    }

    //Sets or unsets scaling: 0 off, 1 equilibrium, 2 geometric, 3 auto, 4 auto-but-as-initialSolve-in-bab
    bool setEscalonamento(const int a_escalonamento) {
        try {
            escalonamento = a_escalonamento;
            return true;
        }
        catch (const CoinError erro) {
            throw std::invalid_argument("SolverClp::setEscalonamento(" + std::to_string(a_escalonamento) + ": \n" + std::string(erro.message()));
        }
        catch (const std::exception& erro) {
            throw std::invalid_argument("SolverClp::setEscalonamento(" + std::to_string(a_escalonamento) + ": \n" + std::string(erro.what()));
        }
    }

    int getEscalonamento () const {
        return escalonamento;
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
        solver->setDualTolerance(a_tolerancia_otimalidade);

        return true;
    }

    bool setToleranciaViabilidade(const double a_tolerancia_viabilidade) {
        solver->setPrimalTolerance(a_tolerancia_viabilidade);

        return true;
    }

    double getToleranciaOtimalidade() const {
        return solver->dualTolerance();
    }

    double getToleranciaViabilidade() const {
        return solver->primalTolerance();
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
            if (a_tipoMetodoSolver == TipoMetodoSolver_concorrente) {
                throw std::invalid_argument(
                        "SolverCLP nao possui metodo " + getFullString(TipoMetodoSolver_concorrente));
            }

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
            insereVariaveis();
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
            insereVariaveis();
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
    bool calcularIIS() {
        fprintf(stderr, "O metodo calcularIIS nao esta implementado para o CLP!\n");
        return false;
    }

    bool exibirNaTela(const bool exibirNaTela) {
        verbose = (int) exibirNaTela;
        return true;
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