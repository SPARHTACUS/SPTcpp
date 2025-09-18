# CMakeList.txt : CMake project for SPTLNX, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

project ("SPTLNX")

set(CMAKE_CXX_STANDARD 20)
  
find_package(MPI REQUIRED)
  
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

# Diretorio do projeto
set(SPT_DIR "/mnt/d/repos/SPARHTACUS")

#set(CMAKE_CXX_FLAGS "-DGRB -I${SPT_DIR}/ThirdParty/COIN/dist/include/coin-or -I${SPT_DIR}/ThirdParty/lsZollen -I/opt/gurobi912/linux64/include -Ofast")

set(CMAKE_CXX_FLAGS "-I${SPT_DIR}/ThirdParty/COIN/dist/include/coin-or -I${SPT_DIR}/ThirdParty/lsZollen -Ofast")

#set(LINK_FLAGS "-L/opt/gurobi912/linux64/lib -lgurobi_c++ -lgurobi91 -L${SPT_DIR}/ThirdParty/COIN/dist/lib -lCbcSolver -lCbc -lCgl -lOsiClp -lClpSolver -lClp -lOsi -lCoinUtils -lcoinlapack -lcoinblas -lbz2 -lz -lgfortran -lgomp -lquadmath -ldl -lpthread")

set(LINK_FLAGS "-L${SPT_DIR}/ThirdParty/COIN/dist/lib -lCbc -lCgl -lOsiClp -lClp -lOsi -lCoinUtils -lcoinlapack -lcoinblas -lbz2 -lz -lgfortran -lgomp -lquadmath -ldl -lpthread -lreadline")

set(SPT_SOURCES ${SPT_DIR}/src/SPT_main.cpp
                ${SPT_DIR}/src/C_Afluencia.cpp
                ${SPT_DIR}/src/C_AgrupamentoIntercambio.cpp
                ${SPT_DIR}/src/C_ArranjoResolucao.cpp
                ${SPT_DIR}/src/C_BaciaHidrografica.cpp
                ${SPT_DIR}/src/C_ConjuntoHidraulico.cpp 
                ${SPT_DIR}/src/C_Contrato.cpp
                ${SPT_DIR}/src/C_ControleCotaVazao.cpp
                ${SPT_DIR}/src/C_CorteBenders.cpp
                ${SPT_DIR}/src/C_Dados.cpp
                ${SPT_DIR}/src/C_Defluencia.cpp 
                ${SPT_DIR}/src/C_DemandaEspecial.cpp 
                ${SPT_DIR}/src/C_ElementoSistema.cpp 
                ${SPT_DIR}/src/C_EntradaSaidaDados.cpp 
                ${SPT_DIR}/src/C_EstagioOtimizacao.cpp 
                ${SPT_DIR}/src/C_FuncaoProducaoHidreletrica.cpp 
                ${SPT_DIR}/src/C_Hidreletrica.cpp 
                ${SPT_DIR}/src/C_Intercambio.cpp 
                ${SPT_DIR}/src/C_Iteracao.cpp 
                ${SPT_DIR}/src/C_LeituraCEPEL.cpp 
                ${SPT_DIR}/src/C_LeituraCEPEL_DECOMP.cpp 
                ${SPT_DIR}/src/C_LeituraCEPEL_NEWAVE.cpp 
                ${SPT_DIR}/src/C_MetodoSolucao.cpp 
                ${SPT_DIR}/src/C_MetodoSolucao_PDDE.cpp 
                ${SPT_DIR}/src/C_MetodoSolucao_PSD.cpp 
                ${SPT_DIR}/src/C_ModeloOtimizacao.cpp 
                ${SPT_DIR}/src/C_ModeloOtimizacao_multiestagio_estocastico.cpp 
                ${SPT_DIR}/src/C_PatamarDeficit.cpp 
                ${SPT_DIR}/src/C_Periodo.cpp 
                ${SPT_DIR}/src/C_PolinomioJusante.cpp 
                ${SPT_DIR}/src/C_Processo.cpp 
                ${SPT_DIR}/src/C_ProcessoEstocastico.cpp
                ${SPT_DIR}/src/C_Renovavel.cpp 
                ${SPT_DIR}/src/C_ReservaPotencia.cpp 
                ${SPT_DIR}/src/C_Reservatorio.cpp
                ${SPT_DIR}/src/C_ReservatorioEquivalente.cpp
                ${SPT_DIR}/src/C_RestricaoCenario.cpp
                ${SPT_DIR}/src/C_RestricaoEletrica.cpp 
                ${SPT_DIR}/src/C_RestricaoOperativaUHE.cpp 
                ${SPT_DIR}/src/C_Submercado.cpp 
                ${SPT_DIR}/src/C_Termeletrica.cpp 
                ${SPT_DIR}/src/C_UnidadeUHE.cpp 
                ${SPT_DIR}/src/C_UnidadeUTE.cpp 
                ${SPT_DIR}/src/C_UsinaElevatoria.cpp 
                ${SPT_DIR}/src/C_UsinaNaoSimulada.cpp 
                ${SPT_DIR}/src/C_VariavelAleatoria.cpp 
                ${SPT_DIR}/src/C_VariavelAleatoriaInterna.cpp 
                ${SPT_DIR}/src/C_VariavelEstado.cpp 
                ${SPT_DIR}/src/C_VariavelRealizacao.cpp 
                ${SPT_DIR}/src/C_VariavelRealizacaoInterna.cpp)

add_executable(SPTLNX ${SPT_SOURCES})
  
target_link_libraries(SPTLNX ${MPI_CXX_LIBRARIES} ${MPI_CXX_LINK_FLAGS} ${LINK_FLAGS})




