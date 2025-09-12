cmake_minimum_required(VERSION 3.10)

project(SPTLNX)

set(CMAKE_CXX_STANDARD 20)
   
find_package(MPI REQUIRED)
  
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

#set(CMAKE_CXX_FLAGS "-DGRB -I${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/COIN/dist/include/coin-or -I${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/lsZollen -I/opt/gurobi912/linux64/include -Ofast")

set(CMAKE_CXX_FLAGS "-I${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/COIN/dist/include/coin-or -I${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/lsZollen -Ofast")

#set(LINK_FLAGS "-L/opt/gurobi912/linux64/lib -lgurobi_c++ -lgurobi91 -L${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/COIN/dist/lib -lCbcSolver -lCbc -lCgl -lOsiClp -lClpSolver -lClp -lOsi -lCoinUtils -lcoinlapack -lcoinblas -lbz2 -lz -lgfortran -lgomp -lquadmath -ldl -lpthread")

set(LINK_FLAGS "-L${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/COIN/dist/lib -lCbc -lCgl -lOsiClp -lClp -lOsi -lCoinUtils -lcoinlapack -lcoinblas -lbz2 -lz -lgfortran -lgomp -lquadmath -ldl -lpthread -lreadline")

set(SPT_SOURCES ../src/SPT_main.cpp
                ../src/C_Afluencia.cpp
                ../src/C_AgrupamentoIntercambio.cpp
                ../src/C_BaciaHidrografica.cpp
                ../src/C_ConjuntoHidraulico.cpp 
                ../src/C_Contrato.cpp 
                ../src/C_CorteBenders.cpp 
                ../src/C_Dados_CP.cpp 
                ../src/C_Dados.cpp
                ../src/C_Dados_MP.cpp 
                ../src/C_Defluencia.cpp 
                ../src/C_DemandaEspecial.cpp 
                ../src/C_DiscretizacaoTemporal.cpp 
                ../src/C_ElementoSistema.cpp 
                ../src/C_EntradaSaidaDados.cpp 
                ../src/C_Eolica.cpp 
                ../src/C_EstagioOtimizacao.cpp 
                ../src/C_FuncaoProducaoHidreletrica.cpp 
                ../src/C_Hidreletrica.cpp 
                ../src/C_Intercambio.cpp 
                ../src/C_IntercambioHidraulico.cpp
                ../src/C_Iteracao.cpp 
                ../src/C_LeituraCEPEL.cpp 
                ../src/C_LeituraCEPEL_DECOMP.cpp 
                ../src/C_LeituraCEPEL_DESSEM.cpp 
                ../src/C_LeituraCEPEL_NEWAVE.cpp 
                ../src/C_MetodoSolucao.cpp 
                ../src/C_MetodoSolucao_PDDE.cpp 
                ../src/C_MetodoSolucao_PSD.cpp 
                ../src/C_ModeloOtimizacao.cpp 
                ../src/C_ModeloOtimizacao_multiestagio_estocastico.cpp 
                ../src/C_PatamarDeficit.cpp 
                ../src/C_Periodo.cpp 
                ../src/C_PolinomioJusante.cpp 
                ../src/C_ProcessoEstocastico.cpp
                ../src/C_Processo.cpp 
                ../src/C_Regua11.cpp 
                ../src/C_ReservaPotencia.cpp 
                ../src/C_Reservatorio.cpp
                ../src/C_ReservatorioEquivalente.cpp
                ../src/C_RestricaoEletrica.cpp 
                ../src/C_RestricaoOperativaUHE.cpp 
                ../src/C_RestricaoRealizacao.cpp
                ../src/C_Submercado.cpp 
                ../src/C_Termeletrica.cpp 
                ../src/C_UnidadeUHE.cpp 
                ../src/C_UnidadeUTE.cpp 
                ../src/C_UsinaElevatoria.cpp 
                ../src/C_UsinaNaoSimulada.cpp 
                ../src/C_VariavelAleatoria.cpp 
                ../src/C_VariavelAleatoriaInterna.cpp 
                ../src/C_VariavelEstado.cpp 
                ../src/C_VariavelRealizacao.cpp 
                ../src/C_VariavelRealizacaoInterna.cpp)

add_executable(SPTLNX ${SPT_SOURCES})
  
target_link_libraries(SPTLNX ${MPI_CXX_LIBRARIES} ${MPI_CXX_LINK_FLAGS} ${LINK_FLAGS})
 
