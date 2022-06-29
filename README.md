[![Sparhtacus](https://sparhtacus.com/wp-content/uploads/2020/12/spt-hzG.png)](https://sparhtacus.com/sobre/)


Modelo computacional open-source para programação hidrotérmica em C++, mais detalhes no [WIKI](https://github.com/SPARHTACUS/SPTcpp/wiki). 

[![Versao](https://img.shields.io/github/license/SPARHTACUS/SPTcpp)](https://github.com/SPARHTACUS/SPTcpp/blob/main/LICENSE.md)


Versão mais recente:

[![SPTv2.0.](https://img.shields.io/github/v/release/sparhtacus/SPTcpp)](https://github.com/SPARHTACUS/SPTcpp/releases/tag/SPTv2.0)


**github@sparhtacus.com** para entrar para a comunidade e colaborar tecnicamente com o projeto! 

---------------------------------------------------------------------------------------------------------------------------------------------------------

# Baixar e executar

Instruções básicas para execução do modelo podem ser encontradas [AQUI](https://github.com/SPARHTACUS/SPTcpp/wiki/Execucao-Basica).

Decks disponibilizados em [sparhtacus.com/decks](https://sparhtacus.com/decks).

## Windows 10 e 11 (x64):

* Requisitos:
  * [Microsoft Visual C++ Redistributable](https://docs.microsoft.com/en-US/cpp/windows/latest-supported-vc-redist?view=msvc-170) (Caso necessário) 
  * [Microsoft MPI (bin)](https://docs.microsoft.com/en-us/message-passing-interface/microsoft-mpi)
  * Executável [SPTWIN](https://github.com/SPARHTACUS/SPTcpp/releases)

## Ubuntu 20.04 (x64):

* Requisitos:
  * OpenMPI
  ```
    $ sudo apt-get install openmpi-bin
  ```  
  * Executável [SPTLNX](https://github.com/SPARHTACUS/SPTcpp/releases)

---------------------------------------------------------------------------------------------------------------------------------------------------------

# Programar e compilar

O repositório foi automatizado para utilização do *Microsoft Visual Studio Community 2019 (v16)* em ambiente *Windows* com *Windows Subsistem for Linux (WSL)*.

## SPTWIN (x64)

* Requisitos:
  * [Microsoft MPI (sdk)](https://docs.microsoft.com/en-us/message-passing-interface/microsoft-mpi).
    * Criar variável ambiente `MSMPI_INC` apontando o diretório `/Microsoft SDKs/MPI/Include`.
    * Criar variável ambiente `MSMPI_LIB64` apontando o diretório `/Microsoft SDKs/MPI/Lib/x64`.
  * [libseq_beta_04.21.01](http://www.multires.caltech.edu/software/libseq/) (Incluso no diretório */ThirdParty/* com ajustes para compiladores recentes).
  * *ls_zollen* (Incluso no diretório */ThirdParty/*).
  * Resolvedores [COIN-OR](https://github.com/coin-or).
    * Executar script instalador `winCoinInstaller` ou `lnxCoinInstaller` disponibilizados no diretório */ThirdParty/*.
      *  Apenas o instalador `lnxCoinInstaller` é necessário para compilação *Windows* e *Linux* no mesmo diretório utilizando *WSL*. Nesse caso também é necessário instalar via `$ sudo apt-install` as bibliotecas: `libopenmpi-dev`, `gfortran`, `pkgconf`, `libbz2-dev` e `zlib1g-dev`.
    * Ambos os scrips promovem as seguintes alterações do código original *COIN-OR* para compatibilização com biblioteca *<windows.h>*:
        ```
        "&small," -> "&small_," (ClpSimplexOther.hpp, l244)
        "std::max" -> "(std::max)" (CglGMI.hpp - l87, l101)
        ```
  * Resolvedor [Gurobi](https://www.gurobi.com/) (Opcional)
    * Incluir `GRB` nas instruções de precompilação *preprocessor definitions*
    * Criar variável ambiente `GUROBI_INC64` apontando o diretório `/gurobiXXX/win64/include`
    * Criar variável ambiente `GUROBI_LIB64` apontando o diretório `/gurobiXXX/win64/lib`

## SPTLNX (x64)

* Requisitos:
  * [libseq_beta_04.21.01](http://www.multires.caltech.edu/software/libseq/) (Incluso no diretório */ThirdParty/* com ajustes para compiladores recentes).
  * *ls_zollen* (Incluso no diretório */ThirdParty/*).
  * Resolvedores [COIN-OR](https://github.com/coin-or).
    * Instalar via `$ sudo apt-install` as bibliotecas: `libopenmpi-dev`, `gfortran`, `pkgconf`, `libbz2-dev` e `zlib1g-dev`.
    * Executar script instalador `lnxCoinInstaller` disponibilizado no diretório */ThirdParty/* (em caso de compilação *Windows* e *Linux* no mesmo diretório utilizando *WSL*, apenas o instalador `lnxCoinInstaller` é necessário).
    * Ambos os scrips promovem as seguintes alterações do código original *COIN-OR* para compatibilização com biblioteca *<windows.h>*:
        ```
        "&small," -> "&small_," (ClpSimplexOther.hpp, l244)
        "std::max" -> "(std::max)" (CglGMI.hpp - l87, l101)
        ```
  * Resolvedor [Gurobi](https://www.gurobi.com/) (Opcional)
    * Descomentar com as linhas 11 e 15 e comentar as linhas 13 e 17 do arquivo CMakeLists.txt
