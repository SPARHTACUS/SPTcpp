#include "mpi.h"
#include "C_Dados.h"
#include "C_MetodoSolucao.h"
#include "C_EntradaSaidaDados.h"
#include "C_LeituraCEPEL.h"

#include <string>
#include <fstream>
#include <chrono>


// #define LARGE_MODEL

void getInfoProduto(const std::string a_arquivoLicenca) {
	std::cout << "##########################################################" << std::endl << std::endl;
	std::cout << "                   Norus POWER FUSION " << std::endl;
	std::cout << std::endl;
	std::cout << "                      Version: 1.0 " << std::endl;
	std::cout << "            Build: " << __TIMESTAMP__ << std::endl;

	std::cout << std::endl;
	std::cout << "           Copyright (c) Norus Tecnologia LTDA" << std::endl << std::endl;
	std::cout << "           2024 - All Rights Reserverd" << std::endl;
	std::cout << "           More information " << a_arquivoLicenca << " " << std::endl << std::endl;
	std::cout << "##########################################################" << std::endl << std::endl;
};

int getIndiceArgumento(const std::string a_arg, int argc, char* argv[]) {

	if (argc <= 1)
		return 0;

	int indice = 0;
	for (int i = 1; i < argc; i++) {
		if (strCompara(argv[i], a_arg)) {
			if (indice == 0) {
				if (i + 1 == argc) {
					std::cout << "Argumento deve ser acompanhado de valor, ex: -a MP" << std::endl;
					return -1;
				}
				indice = i;
			} else {
				std::cout << "Multiplos argumentos " << std::string(argv[indice]) << std::endl;
				return -1;
			}
		} // if (strCompara(argv[i], a_arg)) {
	} // for (int i = 1; i < argc; i++) {
	return indice;
} // int getIndiceArgumento(const std::string a_codigo, int argc, char* argv[]) {


void imprimirArquivoLicenca(const std::string a_arquivoLicenca) {

	try {
		std::ofstream escritaStream;
		escritaStream.open(a_arquivoLicenca, std::ios_base::out);
		if (!escritaStream.is_open())
			throw std::invalid_argument("Nao foi possivel abrir o arquivo.");

		escritaStream << "Property of Norus Tecnologia LTDA." << std::endl << std::endl;
		escritaStream << "2024 - All Rights Reserverd" << std::endl;
		  
		escritaStream.close();
		escritaStream.clear();
	} // try
	catch (const std::ofstream::failure& erro) { throw std::invalid_argument("Erro de integridade do arquivo " + a_arquivoLicenca + ": \n" + std::string(erro.what())); }
	catch (const std::exception& erro) { throw std::invalid_argument("Erro ao editar o arquivo " + a_arquivoLicenca + ": \n" + std::string(erro.what())); }

} // void imprimirArquivoLicenca() {

//////////////////////////////////////////////////////////////////////
// Programa Principal
//////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

	double tempoTotal = 0.0;
	auto start_clock_total = std::chrono::high_resolution_clock::now();

	std::string nick_estudo = "";
	std::string deck_cepel  = "";

	bool pause_initiation = false;
	bool encerrar_apos_deck_cepel = false;

	if (argc > 1) {
		 
		bool encerrar_sessao = false;
		int indice = getIndiceArgumento("-nick", argc, argv);
		if (indice > 0)
			nick_estudo = std::string(argv[indice + 1]);
		else if (indice < 0)
			encerrar_sessao = true;

		indice = getIndiceArgumento("-deck", argc, argv);
		if (indice > 0)
			deck_cepel = std::string(argv[indice + 1]);
		else if (indice < 0)
			encerrar_sessao = true;

		indice = getIndiceArgumento("-stop", argc, argv);
		if (indice > 0) {
			if (std::atoi(argv[indice + 1]) > 0)
				encerrar_apos_deck_cepel = true;
		}
		indice = getIndiceArgumento("-pause", argc, argv);
		if (indice > 0) {
			if (std::atoi(argv[indice + 1]) > 0)
				pause_initiation = true;
		}
		else if (indice < 0)
			encerrar_sessao = true;

		if (encerrar_sessao)
			return EXIT_FAILURE;

	} // if (argc > 1) {

	// --------------------------------------
	// Processamento paralelo
	// --------------------------------------
	int rank, numProcs;
	MPI_Init(&argc, &argv);						// Inicializa processamento paralelo
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);		// Define qual é o identificador do processo
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);	// Verifica quantos processos são rodados em paralelo

	const IdProcesso idProcesso = IdProcesso(rank + 1);
	const IdProcesso maiorIdProcesso = IdProcesso(numProcs);

	const std::string PWF_status = "PWF" + nick_estudo + "_status.txt";

	try {

		if ((idProcesso == IdProcesso_mestre) && pause_initiation)
			std::system("pause");
		MPI_Barrier(MPI_COMM_WORLD);


		//
		// Inicialização de arquivo de Status.
		//
		if (idProcesso == IdProcesso_mestre) {

			const std::string arquivoLicense = "LICENSE.lic";

			imprimirArquivoLicenca(arquivoLicense); 

			getInfoProduto(arquivoLicense);

			try {
				std::ofstream escritaStream;
				escritaStream.open(PWF_status, std::ios_base::out);
				if (!escritaStream.is_open())
					throw std::invalid_argument("Nao foi possivel abrir o arquivo.");
				escritaStream << "2;EM_EXECUCAO" << std::endl;
				escritaStream.close();
				escritaStream.clear();
			} // try
			catch (const std::ofstream::failure& erro) { throw std::invalid_argument("Erro de integridade do arquivo " + PWF_status + ": \n" + std::string(erro.what())); }
			catch (const std::exception&         erro) { throw std::invalid_argument("Erro ao editar o arquivo " + PWF_status + ": \n" + std::string(erro.what())); }
		} // if (idProcesso == IdProcesso_mestre) {

		if (deck_cepel != "") {
			LeituraCEPEL leituraCEPEL;
			leituraCEPEL.leitura_CEPEL(idProcesso, maiorIdProcesso, deck_cepel, nick_estudo);
		} // if (deck_cepel != "") {

		if (!encerrar_apos_deck_cepel) {

			MPI_Barrier(MPI_COMM_WORLD);

			Dados  dados;

			dados.arranjoResolucao.instanciarProcessos(idProcesso, maiorIdProcesso);

			EntradaSaidaDados entradaSaidaDados;

			entradaSaidaDados.setDiretorioEntrada(entradaSaidaDados.getDiretorioEntrada() + nick_estudo);
			entradaSaidaDados.setDiretorioSaida(entradaSaidaDados.getDiretorioSaida() + nick_estudo);

			dados.setAtributo(AttComumDados_diretorio_entrada_dados, entradaSaidaDados.getDiretorioEntrada());
			dados.setAtributo(AttComumDados_diretorio_saida_dados, entradaSaidaDados.getDiretorioSaida());

			// O diretorio de saida pode ser alterado via arquivo de configuração.
			dados.carregarArquivosEntrada(entradaSaidaDados);

			const TipoEstudo tipo_estudo = dados.getAtributo(AttComumDados_tipo_estudo, TipoEstudo());
			const IdMetodoSolucao idMetodoSolucao = dados.getAtributo(AttComumDados_tipo_metodo_solucao, IdMetodoSolucao());


			//
			// OTIMIZACAO
			//
			if ((tipo_estudo == TipoEstudo_otimizacao_e_simulacao) || (tipo_estudo == TipoEstudo_otimizacao)) {

				entradaSaidaDados.setDiretorioSaida(dados.getAtributo(AttComumDados_diretorio_saida_dados, string()) + "//Otimizacao");

				auto start_clock_modelo = std::chrono::high_resolution_clock::now();

				ModeloOtimizacao modeloOtimizacao(IdModeloOtimizacao_multiestagio_estocastico_otimizacao, dados, entradaSaidaDados);

				if (idProcesso == IdProcesso_mestre)
					std::cout << std::endl << std::endl << "TEMPO TOTAL PARA CRIAR O MODELO DE OTIMIZACAO                            = " << std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock_modelo).count() / 60 << std::endl;

				MetodoSolucao(entradaSaidaDados, idProcesso, maiorIdProcesso, idMetodoSolucao, modeloOtimizacao, dados);

				if (idProcesso == IdProcesso_mestre)
					std::cout << std::endl << std::endl << "TEMPO TOTAL DE OTIMIZACAO = " << std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock_modelo).count() / 60 << std::endl;
			}


			//
			// SIMULACAO
			//       
			else if (tipo_estudo == TipoEstudo_simulacao) {

				if (tipo_estudo == TipoEstudo_otimizacao_e_simulacao)
					dados.setAtributo(AttComumDados_diretorio_importacao_cortes, std::string(entradaSaidaDados.getDiretorioSaida() + "//Cortes"));

				entradaSaidaDados.setDiretorioSaida(dados.getAtributo(AttComumDados_diretorio_saida_dados, string()) + "//Simulacao");

				auto start_clock_modelo = std::chrono::high_resolution_clock::now();

				ModeloOtimizacao modeloOtimizacao(IdModeloOtimizacao_multiestagio_estocastico_simulacao, dados, entradaSaidaDados);
				MetodoSolucao(entradaSaidaDados, idProcesso, maiorIdProcesso, idMetodoSolucao, modeloOtimizacao, dados);

				if (idProcesso == IdProcesso_mestre)
					std::cout << std::endl << std::endl << "Tempo Total Simulacao = " << std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_clock_modelo).count() / 60 << std::endl;

			} // else if (tipo_estudo == TipoEstudo_simulacao) {

			else
				throw std::invalid_argument("Tipo de estudo " + getFullString(tipo_estudo) + " nao utilizado.");

		} // if (!encerrar_apos_deck_cepel) {

		MPI_Finalize();

		if (idProcesso == IdProcesso_mestre) {

			std::chrono::duration<double> tempo = std::chrono::high_resolution_clock::now() - start_clock_total;
			std::cout << std::endl << "Tempo Total de Execucao = " << tempo.count() / 60 << std::endl;

			std::cout << std::endl << "O PowerFusion foi finalizado com sucesso." << std::endl;

			try {
				std::ofstream escritaStream;
				escritaStream.open(PWF_status, std::ios_base::out);
				if (!escritaStream.is_open())
					throw std::invalid_argument("Nao foi possivel abrir o arquivo.");
				escritaStream << "1;FINALIZADO_COM_SUCESSO";
				escritaStream.close();
				escritaStream.clear();
			} // try
			catch (const std::ofstream::failure& erro) { throw std::invalid_argument("Erro de integridade do arquivo " + PWF_status + ": \n" + std::string(erro.what())); }
			catch (const std::exception&         erro) { throw std::invalid_argument("Erro ao editar o arquivo " + PWF_status + ": \n" + std::string(erro.what())); }

		} // if (idProcesso == IdProcesso_mestre) {
		
		return EXIT_SUCCESS;
	} // try

	catch (const std::exception& erro) {

		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << ":~(  Excecao encontrada na execucao do modelo!" << std::endl << std::endl;
		std::cout << erro.what() << std::endl;

		try {
			std::ofstream escritaStream;
			escritaStream.open(PWF_status, std::ios_base::out);
			if (!escritaStream.is_open()) {
				std::cout << erro.what() << std::endl;
				throw std::invalid_argument("Nao foi possivel abrir o arquivo.");
				
			}
			escritaStream << "0;FINALIZADO_COM_EXCECAO" << std::endl;
			escritaStream << erro.what();
			escritaStream.close();
			escritaStream.clear();
		} // try
		catch (const std::ofstream::failure& erro) { std::cout << "Erro de integridade do arquivo " << PWF_status.c_str() << "." << std::endl << erro.what() << std::endl << std::endl; }
		catch (const std::exception&         erro) { std::cout << "Erro ao editar o arquivo " + PWF_status << "." << std::endl << erro.what() << std::endl << std::endl; }

		MPI_Barrier(MPI_COMM_WORLD);

		MPI_Abort(MPI_COMM_WORLD, MPI_SUCCESS);

		return EXIT_FAILURE;

	} // catch (const std::exception& erro) {

} // int main(int argc, char *argv[]) {

