[![Sparhtacus](https://sparhtacus.com/wp-content/uploads/2020/12/spt-hzG.png)](https://sparhtacus.com/sobre/)

Modelo computacional open-source para programação hidrotérmica em C++, mais detalhes no [WIKI](https://github.com/SPARHTACUS/SPTcpp/wiki). 

Programa open-source distribuído sob [licença MIT](https://github.com/SPARHTACUS/SPTcpp/blob/main/LICENSE.md). 

Versão mais recente [SPTv2.0.](https://github.com/SPARHTACUS/SPTcpp/releases/tag/SPTv2.0).

# Baixar e executar

Instruções básicas para execução do modelo podem ser encontradas AQUI.

Decks disponibilizados em [sparhtacus.com](https://sparhtacus.com).

## Windows 10 e 11 (x64):

* Requisitos:
  * [Microsoft MPI](https://docs.microsoft.com/en-us/message-passing-interface/microsoft-mpi)
  * [Gurobi](https://www.gurobi.com/) (Opcional)
  * [SPTWIN](https://github.com/SPARHTACUS/SPTcpp/releases)

## Ubuntu 20.04 (x64):

* Requisitos:
  * OpenMPI
  ```
    $ apt-get install openmpi-bin
  ```  
  * [Gurobi](https://www.gurobi.com/) (Opcional)
  * [SPTLNX](https://github.com/SPARHTACUS/SPTcpp/releases)

# Programar e compilar

O repositório foi automatizado para utilização do *Microsoft Visual Studio Community 2019 (v16)* em ambiente *Windows* com *Windows Subsistem for Linux (WSL)*.

* Requisitos:
  * [libseq_beta_04.21.01](http://www.multires.caltech.edu/software/libseq/) (Incluso no diretório */ThirdParty/* com ajustes para compiladores recentes).
  * *ls_zollen* (Incluso no diretório */ThirdParty/*).
  * Resolvedores [COIN-OR](https://github.com/coin-or). Scripts instaladores `winCoinInstaller` e `lnxCoinInstaller` disponibilizados no diretório */ThirdParty/*. Ambos os scrips promovem as seguintes alterações do código original *COIN-OR* para compatibilização com biblioteca *<windows.h>*:
   ```
   "&small," -> "&small_," (ClpSimplexOther.hpp, l244)
   "std::max" -> "(std::max)" (CglGMI.hpp - l87, l101)
   ```
   Em caso de compilação *Windows* e *Linux* no mesmo diretório utilizando *WSL*, apenas o instalador `lnxCoinInstaller` é necessário.
