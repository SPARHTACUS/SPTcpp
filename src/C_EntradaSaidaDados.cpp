#include "C_EntradaSaidaDados.h"

#include <iostream>
#include <fstream>

// Um número de caracteres muito grande pode estourar o buffer da string;

static int const numCaract = 300000;
static int const numLinhas = 950000;

static int const tempo_maximo_tentativa_escrita_arquivo = 300000; // Milisegundos

EntradaSaidaDados::EntradaSaidaDados(){

	restauraImprimirNaTela();

	restauraAppendArquivo();

	restauraDiretorioEntrada();

	restauraDiretorioSaida();

	restauraFimDeArquivo();

	restauraSeparadorCSV();

} // EntradaSaidaDados::EntradaSaidaDados(){

EntradaSaidaDados::EntradaSaidaDados(const bool a_append, const bool a_imprimirNaTela, const std::string a_diretorioEntrada, const std::string a_diretorioSaida, const std::string a_fimDeArquivo, const std::string a_separadorCSV){
	
	appendArquivo_original = a_append;
	restauraAppendArquivo();

	imprimirNaTela_original = a_imprimirNaTela;
	restauraImprimirNaTela();

	diretorioEntrada_original = a_diretorioEntrada;
	restauraDiretorioEntrada();

	diretorioSaida_original = a_diretorioSaida;
	restauraDiretorioSaida();

	fimDeArquivo_original = a_fimDeArquivo;
	restauraFimDeArquivo();

	separadorCSV_original = a_separadorCSV;
	restauraSeparadorCSV();

}

EntradaSaidaDados::~EntradaSaidaDados(){}

// Sets

void EntradaSaidaDados::setAppendArquivo(const bool a_append) { appendArquivo = a_append; }

void EntradaSaidaDados::setImprimirNaTela(const bool a_imprimirNaTela) { imprimirNaTela = a_imprimirNaTela; }

void EntradaSaidaDados::setDiretorioEntrada(const std::string a_diretorioEntrada) { diretorioEntrada = a_diretorioEntrada; }

void EntradaSaidaDados::setDiretorioSaida(const std::string a_diretorioSaida) { diretorioSaida = a_diretorioSaida; }

void EntradaSaidaDados::setFimDeArquivo(const std::string a_fimDeArquivo) { fimDeArquivo = a_fimDeArquivo; }

void EntradaSaidaDados::setSeparadorCSV(const std::string a_separadorCSV) { separadorCSV = a_separadorCSV; }

// Gets

bool EntradaSaidaDados::getAppendArquivo()const {	return appendArquivo; }

bool EntradaSaidaDados::getImprimirNaTela()const { return imprimirNaTela; }

std::string EntradaSaidaDados::getDiretorioEntrada()const { return diretorioEntrada; }

std::string EntradaSaidaDados::getDiretorioSaida()const { return diretorioSaida; }

std::string EntradaSaidaDados::getFimDeArquivo()const { return fimDeArquivo; }

std::string EntradaSaidaDados::getSeparadorCSV()const { return separadorCSV; }

// Restaura

void EntradaSaidaDados::restauraImprimirNaTela() { setImprimirNaTela(imprimirNaTela_original); }

void EntradaSaidaDados::restauraAppendArquivo() { setAppendArquivo(appendArquivo_original); }

void EntradaSaidaDados::restauraDiretorioEntrada() { setDiretorioEntrada(diretorioEntrada_original); }

void EntradaSaidaDados::restauraDiretorioSaida() { setDiretorioSaida(diretorioSaida_original); }

void EntradaSaidaDados::restauraFimDeArquivo() { setFimDeArquivo(fimDeArquivo_original); }

void EntradaSaidaDados::restauraSeparadorCSV() { setSeparadorCSV(separadorCSV_original); }



void EntradaSaidaDados::carregarArquivoCSV_AttComum(const std::string a_nomeArquivo, SmartDados & a_objetoDados, const TipoAcessoInstancia a_tipoAcesso)const {

	std::string caminhoArquivo = diretorioEntrada + "//" + a_nomeArquivo;
	if (diretorioEntrada == "")
		caminhoArquivo = a_nomeArquivo;

	try {

		if (a_tipoAcesso == TipoAcessoInstancia_Nenhum)
			throw std::invalid_argument("Necessario informar o argumento Tipo de Acesso a Instancia.");

		int numMinColunas = 0;

		if (a_tipoAcesso == TipoAcessoInstancia_direto)
			numMinColunas = 2;
		else if (a_tipoAcesso == TipoAcessoInstancia_membro)
			numMinColunas = 2;
		else if (a_tipoAcesso == TipoAcessoInstancia_membroMembro)
			numMinColunas = 3;
		else if (a_tipoAcesso == TipoAcessoInstancia_membroMembroMembro)
			numMinColunas = 4;
		else if (a_tipoAcesso == TipoAcessoInstancia_membroMembroMembroMembro)
			numMinColunas = 5;
		else
			throw std::invalid_argument("Argumento Tipo de Acesso a Instancia invalido.");

		std::ifstream leituraArquivo;

		leituraArquivo.open(caminhoArquivo.c_str(), std::ios_base::in);

		if (!leituraArquivo.is_open())
			throw std::invalid_argument("Nao foi possivel abrir o arquivo.");

		char linhaChar[numCaract];

		std::string   linha; // Linha de dados lida do arquivo.
		std::string   valor; // Valor retirado da linha de dados do arquivo.

		int lin; // Posição da linha da informação ao longo do arquivo de dados.
		size_t pos; // Posição da coluna da informação ao longo da linha de dados.

		std::vector<std::string> cabecalho;

		// Leitura do Cabeçalho
		leituraArquivo.getline(linhaChar, numCaract);
		linha = linhaChar;

		strNormalizada(linha);

		pos = 0;
		while (pos != std::string::npos) {

			pos = linha.find(separadorCSV);

			valor = linha.substr(0, pos).c_str();

			cabecalho.push_back(valor);

			linha = linha.substr(pos + 1, linha.length());

			if (linha == "")
				break;

		} // while (pos != string::npos) {

		if (cabecalho.size() < numMinColunas)
			throw std::invalid_argument("O cabecalho do arquivo deve possuir pelo menos " + std::to_string(numMinColunas) + " colunas.");

		std::string membro = "";
		std::string membroMembro = "";
		std::string membroMembroMembro = "";
		std::string membroMembroMembroMembro = "";

		std::vector<std::string> vetorValores;
		std::vector<std::string> vetorAttComuns;

		std::vector<std::string> vetorIdMembro;
		std::vector<std::string> vetorIdMembroMembro;
		std::vector<std::string> vetorIdMembroMembroMembro;
		std::vector<std::string> vetorIdMembroMembroMembroMembro;

		if (a_tipoAcesso == TipoAcessoInstancia_direto) {

			  vetorValores.push_back(cabecalho.at(1));
			vetorAttComuns.push_back(cabecalho.at(0));

		} // if (a_tipoAcesso == TipoAcessoInstancia_direto) {

		else if (a_tipoAcesso != TipoAcessoInstancia_direto) {

			if (a_tipoAcesso >= TipoAcessoInstancia_membro)
				membro = cabecalho.at(0);

			if (a_tipoAcesso >= TipoAcessoInstancia_membroMembro)
				membroMembro = cabecalho.at(1);

			if (a_tipoAcesso >= TipoAcessoInstancia_membroMembroMembro)
				membroMembroMembro = cabecalho.at(2);

			if (a_tipoAcesso >= TipoAcessoInstancia_membroMembroMembroMembro)
				membroMembroMembroMembro = cabecalho.at(3);

			vetorAttComuns = std::vector<std::string>(cabecalho.begin() + numMinColunas - 1, cabecalho.end());

		} // else if (a_tipoAcesso != TipoAcessoInstancia_direto){

			// Leitura dados do arquivo
		lin = 2;
		while (lin < numLinhas) {

			leituraArquivo.getline(linhaChar, numCaract);
			linha = linhaChar;

			strNormalizada(linha);

			valor = linha;

			pos = valor.find(separadorCSV);

			if (pos == std::string::npos) {
				if (fimDeArquivo != "") {
					if (!strCompara(valor, fimDeArquivo))
						std::cout << "Aviso! Arquivo " + caminhoArquivo + " nao finalizado com " << fimDeArquivo << std::endl;
				} // if (fimDeArquivo != "") {
				break;
			} // if (pos != string::npos) {

			valor = linha.substr(0, pos).c_str();

			if (fimDeArquivo != "") {
				if (strCompara(valor, fimDeArquivo))
					break;
			} // if (fimDeArquivo != "") {

			std::string idMembro = "";

			if (a_tipoAcesso >= TipoAcessoInstancia_membro) {
				idMembro = valor;

				if (!findStringNoVetor(idMembro, vetorIdMembro))
					vetorIdMembro.push_back(idMembro);

				pos = linha.find(separadorCSV);
				linha = linha.substr(pos + 1, linha.length());

			} // if (a_tipoAcesso >= TipoAcessoInstancia_membro) {

			std::string idMembroMembro = "";

			if (a_tipoAcesso >= TipoAcessoInstancia_membroMembro) {
				pos = linha.find(separadorCSV);
				valor = linha.substr(0, pos).c_str();

				idMembroMembro = valor;

				if (!findStringNoVetor(idMembroMembro, vetorIdMembroMembro))
					vetorIdMembroMembro.push_back(idMembroMembro);

				pos = linha.find(separadorCSV);
				linha = linha.substr(pos + 1, linha.length());

			} // if (a_tipoAcesso >= TipoAcessoInstancia_membroMembro) {

			std::string idMembroMembroMembro = "";

			if (a_tipoAcesso >= TipoAcessoInstancia_membroMembroMembro) {
				pos = linha.find(separadorCSV);
				valor = linha.substr(0, pos).c_str();

				idMembroMembroMembro = valor;

				if (!findStringNoVetor(idMembroMembroMembro, vetorIdMembroMembroMembro))
					vetorIdMembroMembroMembro.push_back(idMembroMembroMembro);

				pos = linha.find(separadorCSV);
				linha = linha.substr(pos + 1, linha.length());
			} // if (a_tipoAcesso >= TipoAcessoInstancia_membroMembroMembro) {

			std::string idMembroMembroMembroMembro = "";

			if (a_tipoAcesso >= TipoAcessoInstancia_membroMembroMembroMembro) {
				pos = linha.find(separadorCSV);
				valor = linha.substr(0, pos).c_str();

				idMembroMembroMembroMembro = valor;

				if (!findStringNoVetor(idMembroMembroMembroMembro, vetorIdMembroMembroMembroMembro))
					vetorIdMembroMembroMembroMembro.push_back(idMembroMembroMembroMembro);

				pos = linha.find(separadorCSV);
				linha = linha.substr(pos + 1, linha.length());
			} // if (a_tipoAcesso >= TipoAcessoInstancia_membroMembroMembroMembro) {


			if (membro.size() > 2) {
				if (membro.substr(0, 2) == "id")
					membro = membro.substr(2, membro.size() - 2);
			} // if (membro.size() > 2) {

			if (membroMembro.size() > 2) {
				if (membroMembro.substr(0, 2) == "id")
					membroMembro = membroMembro.substr(2, membroMembro.size() - 2);
			} // if (membroMembro.size() > 2) {

			if (membroMembroMembro.size() > 2) {
				if (membroMembroMembro.substr(0, 2) == "id")
					membroMembroMembro = membroMembroMembro.substr(2, membroMembroMembro.size() - 2);
			} // if (membroMembroMembro.size() > 2) {

			if (membroMembroMembroMembro.size() > 2) {
				if (membroMembroMembroMembro.substr(0, 2) == "id")
					membroMembroMembroMembro = membroMembroMembroMembro.substr(2, membroMembroMembroMembro.size() - 2);
			} // if (membroMembroMembroMembro.size() > 2) {


			if (a_tipoAcesso == TipoAcessoInstancia_direto) {

				vetorAttComuns.push_back(valor);

				pos = linha.find(separadorCSV);
				linha = linha.substr(pos + 1, linha.length());

				pos = linha.find(separadorCSV);
				valor = linha.substr(0, pos).c_str();

				vetorValores.push_back(valor);

			} // if (a_tipoAcesso == TipoAcessoInstancia_direto) {

			else if (a_tipoAcesso != TipoAcessoInstancia_direto) {

				pos = 0;
				while (pos != std::string::npos) {

					pos = linha.find(separadorCSV);

					valor = linha.substr(0, pos).c_str();

					vetorValores.push_back(valor);

					linha = linha.substr(pos + 1, linha.length());

					if (linha == "")
						break;

				} // while (pos != string::npos) {

				if ((vetorAttComuns.size() != vetorValores.size()) && (a_tipoAcesso != TipoAcessoInstancia_direto))
					throw std::invalid_argument("Numero de colunas no cabecalho diferente do numero de valores na linha " + std::to_string(lin) + ".");

				if (a_tipoAcesso == TipoAcessoInstancia_membro)
					a_objetoDados.addDadosAttComumMembro(membro, idMembro, vetorAttComuns, vetorValores);
				else if (a_tipoAcesso == TipoAcessoInstancia_membroMembro)
					a_objetoDados.addDadosAttComumMembroMembro(membro, idMembro, membroMembro, idMembroMembro, vetorAttComuns, vetorValores);
				else if (a_tipoAcesso == TipoAcessoInstancia_membroMembroMembro)
					a_objetoDados.addDadosAttComumMembroMembroMembro(membro, idMembro, membroMembro, idMembroMembro, membroMembroMembro, idMembroMembroMembro, vetorAttComuns, vetorValores);
				else if (a_tipoAcesso == TipoAcessoInstancia_membroMembroMembroMembro)
					a_objetoDados.addDadosAttComumMembroMembroMembroMembro(membro, idMembro, membroMembro, idMembroMembro, membroMembroMembro, idMembroMembroMembro, membroMembroMembroMembro, idMembroMembroMembroMembro, vetorAttComuns, vetorValores);

				lin++;

				vetorValores.clear();

			} // else if (a_tipoAcesso != TipoAcessoInstancia_direto) {

		} // while (lin < numLinhas) {

		if (a_tipoAcesso == TipoAcessoInstancia_direto)
			a_objetoDados.addDadosAttComum(vetorAttComuns, vetorValores);


		leituraArquivo.close();
		leituraArquivo.clear();

		// Metodos são chamados para validação pós-leitura.
		if (a_tipoAcesso == TipoAcessoInstancia_direto)
			a_objetoDados.validacaoDadosAttComum();
		else if (a_tipoAcesso == TipoAcessoInstancia_membro)
			a_objetoDados.validacaoDadosAttComumMembro(membro, vetorIdMembro);
		else if (a_tipoAcesso == TipoAcessoInstancia_membroMembro)
			a_objetoDados.validacaoDadosAttComumMembroMembro(membro, vetorIdMembro, membroMembro, vetorIdMembroMembro);
		else if (a_tipoAcesso == TipoAcessoInstancia_membroMembroMembro)
			a_objetoDados.validacaoDadosAttComumMembroMembroMembro(membro, vetorIdMembro, membroMembro, vetorIdMembroMembro, membroMembroMembro, vetorIdMembroMembroMembro);
		else if (a_tipoAcesso == TipoAcessoInstancia_membroMembroMembroMembro)
			a_objetoDados.validacaoDadosAttComumMembroMembroMembroMembro(membro, vetorIdMembro, membroMembro, vetorIdMembroMembro, membroMembroMembro, vetorIdMembroMembroMembro, membroMembroMembroMembro, vetorIdMembroMembroMembroMembro);

		if (imprimirNaTela)
			std::cout << "Sucesso na leitura de " << caminhoArquivo << std::endl;

	} // try

	catch (const std::ifstream::failure& erro) { throw std::invalid_argument("EntradaSaidaDados::carregarArquivoCSV_AttComum(" + caminhoArquivo + ",a_objetoDados," + getString(a_tipoAcesso) + "): \nErro de Integridade do Arquivo." + std::string(erro.what())); }
	catch (const std::exception& erro) { throw std::invalid_argument("EntradaSaidaDados::carregarArquivoCSV_AttComum(" + caminhoArquivo + ",a_objetoDados," + getString(a_tipoAcesso) + "): \n" + std::string(erro.what())); }

} // void EntradaSaidaDados::carregarArquivoCSV_AttComum(std::string a_nomeArquivo, SmartDados & a_objetoDados){

bool EntradaSaidaDados::carregarArquivoCSV_AttComum_seExistir(const std::string a_nomeArquivo, SmartDados & a_objetoDados, const TipoAcessoInstancia a_tipoAcesso) const{

	std::string caminhoArquivo = diretorioEntrada + "//" + a_nomeArquivo;
	if (diretorioEntrada == "")
		caminhoArquivo = a_nomeArquivo;

	try {

		std::ifstream leituraArquivo;

		leituraArquivo.open(caminhoArquivo.c_str(), std::ios_base::in);

		if (!leituraArquivo.is_open())
			return false;

		leituraArquivo.close();
		leituraArquivo.clear();

		carregarArquivoCSV_AttComum(a_nomeArquivo, a_objetoDados, a_tipoAcesso);

		return true;

	} // try

	catch (const std::ifstream::failure& erro) { throw std::invalid_argument("EntradaSaidaDados::carregarArquivoCSV_AttComum_seExistir(" + caminhoArquivo + ",a_objetoDados," + getString(a_tipoAcesso) + "): \nErro de Integridade do Arquivo." + std::string(erro.what())); }
	catch (const std::exception& erro) { throw std::invalid_argument("EntradaSaidaDados::carregarArquivoCSV_AttComum_seExistir(" + caminhoArquivo + ",a_objetoDados," + getString(a_tipoAcesso) + "): \n" + std::string(erro.what())); }

} // void EntradaSaidaDados::carregarArquivoCSV_AttComum_seExistir(const std::string a_nomeArquivo, SmartDados & a_objetoDados, const TipoAcessoInstancia a_tipoAcesso) const{



void EntradaSaidaDados::carregarArquivoCSV_AttVetor(const std::string a_nomeArquivo, SmartDados &a_objetoDados, const TipoAcessoInstancia a_tipoAcesso)const {

	std::string caminhoArquivo = diretorioEntrada + "//" + a_nomeArquivo;
	if (diretorioEntrada == "")
		caminhoArquivo = a_nomeArquivo;;

	try {

		if (a_tipoAcesso == TipoAcessoInstancia_Nenhum)
			throw std::invalid_argument("Necessario informar o argumento Tipo de Acesso a Instancia.");

		int numMinColunas = 0;

		if (a_tipoAcesso == TipoAcessoInstancia_direto)
			numMinColunas = 2;
		else if (a_tipoAcesso == TipoAcessoInstancia_membro)
			numMinColunas = 3;
		else if (a_tipoAcesso == TipoAcessoInstancia_membroMembro)
			numMinColunas = 4;
		else if (a_tipoAcesso == TipoAcessoInstancia_membroMembroMembro)
			numMinColunas = 5;
		else if (a_tipoAcesso == TipoAcessoInstancia_membroMembroMembroMembro)
			numMinColunas = 6;
		else
			throw std::invalid_argument("Argumento Tipo de Acesso a Instancia invalido.");

		std::ifstream leituraArquivo;

		leituraArquivo.open(caminhoArquivo.c_str(), std::ios_base::in);

		if (!leituraArquivo.is_open())
			throw std::invalid_argument("Nao foi possivel abrir o arquivo.");

		char linhaChar[numCaract];

		std::string   linha; // Linha de dados lida do arquivo.
		std::string   valor; // Valor retirado da linha de dados do arquivo.

		int lin; // Posição da linha da informação ao longo do arquivo de dados.
		size_t pos; // Posição da coluna da informação ao longo da linha de dados.

		std::vector<std::string> cabecalho;

		// Leitura do Cabeçalho
		leituraArquivo.getline(linhaChar, numCaract);
		linha = linhaChar;

		strNormalizada(linha);

		pos = 0;
		while (pos != std::string::npos) {

			pos = linha.find(separadorCSV);

			valor = linha.substr(0, pos).c_str();

			cabecalho.push_back(valor);

			linha = linha.substr(pos + 1, linha.length());

			if (linha == "")
				break;

		} // while (pos != string::npos) {

		if (cabecalho.size() < numMinColunas)
			throw std::invalid_argument("O cabecalho do arquivo deve possuir pelo menos " + std::to_string(numMinColunas) + " colunas.");

		std::string membro                   = "";
		std::string membroMembro             = "";
		std::string membroMembroMembro       = "";
		std::string membroMembroMembroMembro = "";

		if (a_tipoAcesso >= TipoAcessoInstancia_membro)
			membro = cabecalho.at(0);

		if (a_tipoAcesso >= TipoAcessoInstancia_membroMembro) 
			membroMembro = cabecalho.at(1);

		if (a_tipoAcesso >= TipoAcessoInstancia_membroMembroMembro) 
			membroMembroMembro = cabecalho.at(2);

		if (a_tipoAcesso >= TipoAcessoInstancia_membroMembroMembroMembro)
			membroMembroMembroMembro = cabecalho.at(3);

		if (membro.size() > 2) {
			if (membro.substr(0, 2) == "id")
				membro = membro.substr(2, membro.size() - 2);
		} // if (membro.size() > 2) {

		if (membroMembro.size() > 2) {
			if (membroMembro.substr(0, 2) == "id")
				membroMembro = membroMembro.substr(2, membroMembro.size() - 2);
		} // if (membroMembro.size() > 2) {

		if (membroMembroMembro.size() > 2) {
			if (membroMembroMembro.substr(0, 2) == "id")
				membroMembroMembro = membroMembroMembro.substr(2, membroMembroMembro.size() - 2);
		} // if (membroMembroMembro.size() > 2) {

		if (membroMembroMembroMembro.size() > 2) {
			if (membroMembroMembroMembro.substr(0, 2) == "id")
				membroMembroMembroMembro = membroMembroMembroMembro.substr(2, membroMembroMembroMembro.size() - 2);
		} // if (membroMembroMembroMembro.size() > 2) {

		const std::vector<std::string> vetorIteradores(cabecalho.begin() + numMinColunas - 1, cabecalho.end());
		std::vector<std::string> vetorValores;
		std::vector<std::string> vetorAttVetor;
		std::vector<std::string> vetorIdMembro;
		std::vector<std::string> vetorIdMembroMembro;
		std::vector<std::string> vetorIdMembroMembroMembro;
		std::vector<std::string> vetorIdMembroMembroMembroMembro;

		std::string attVetor = "";

		// Leitura dados do arquivo
		lin = 2;
		while (lin < numLinhas) {

			leituraArquivo.getline(linhaChar, numCaract);
			linha = linhaChar;

			strNormalizada(linha);

			valor = linha;

			pos = valor.find(separadorCSV);

			if (pos == std::string::npos) {
				if (fimDeArquivo != "") {
					if (!strCompara(valor, fimDeArquivo))
						std::cout << "Aviso! Arquivo " + caminhoArquivo + " nao finalizado com FIM" << std::endl;
				} // if (fimDeArquivo != "") {
				break;
			} // if (pos != string::npos) {

			valor = linha.substr(0, pos).c_str();

			if (fimDeArquivo != "") {
				if (strCompara(valor, fimDeArquivo))
					break;
			} // if (fimDeArquivo != "") {

			std::string idMembro = "";

			if (a_tipoAcesso >= TipoAcessoInstancia_membro) {
				idMembro = valor;

				if (!findStringNoVetor(idMembro, vetorIdMembro))
					vetorIdMembro.push_back(idMembro);

				pos = linha.find(separadorCSV);
				linha = linha.substr(pos + 1, linha.length());
			}

			std::string idMembroMembro = "";

			if (a_tipoAcesso >= TipoAcessoInstancia_membroMembro) {
				pos = linha.find(separadorCSV);
				valor = linha.substr(0, pos).c_str();

				idMembroMembro = valor;

				if (!findStringNoVetor(idMembroMembro, vetorIdMembroMembro))
					vetorIdMembroMembro.push_back(idMembroMembro);

				pos = linha.find(separadorCSV);
				linha = linha.substr(pos + 1, linha.length());

			} // if (a_tipoAcesso >= TipoAcessoInstancia_membroMembro) {

			std::string idMembroMembroMembro = "";

			if (a_tipoAcesso >= TipoAcessoInstancia_membroMembroMembro) {
				pos = linha.find(separadorCSV);
				valor = linha.substr(0, pos).c_str();

				idMembroMembroMembro = valor;

				if (!findStringNoVetor(idMembroMembroMembro, vetorIdMembroMembroMembro))
					vetorIdMembroMembroMembro.push_back(idMembroMembroMembro);

				pos = linha.find(separadorCSV);
				linha = linha.substr(pos + 1, linha.length());
			} // if (a_tipoAcesso >= TipoAcessoInstancia_membroMembroMembro) {

			std::string idMembroMembroMembroMembro = "";

			if (a_tipoAcesso >= TipoAcessoInstancia_membroMembroMembroMembro) {
				pos = linha.find(separadorCSV);
				valor = linha.substr(0, pos).c_str();

				idMembroMembroMembroMembro = valor;

				if (!findStringNoVetor(idMembroMembroMembroMembro, vetorIdMembroMembroMembroMembro))
					vetorIdMembroMembroMembroMembro.push_back(idMembroMembroMembroMembro);

				pos = linha.find(separadorCSV);
				linha = linha.substr(pos + 1, linha.length());
			} // if (a_tipoAcesso >= TipoAcessoInstancia_membroMembroMembroMembro) {

			pos = linha.find(separadorCSV);
			valor = linha.substr(0, pos).c_str();

			attVetor = valor;

			if (!findStringNoVetor(attVetor, vetorAttVetor))
				vetorAttVetor.push_back(attVetor);

			pos = linha.find(separadorCSV);
			linha = linha.substr(pos + 1, linha.length());

			pos = 0;
			while (pos != std::string::npos) {

				pos = linha.find(separadorCSV);

				valor = linha.substr(0, pos).c_str();

				if (valor != ""	)
					vetorValores.push_back(valor);

				linha = linha.substr(pos + 1, linha.length());

				if (linha == "")
					break;

			} // while (pos != string::npos) {

			if (vetorIteradores.size() < vetorValores.size())
				throw std::invalid_argument("Numero de colunas no cabecalho menor que o numero de valores na linha " + std::to_string(lin) + ".");

			const int numero_elementos = int(vetorValores.size());

			for (int i = 0; i < numero_elementos; i++) {
				if (a_tipoAcesso == TipoAcessoInstancia_direto)
					a_objetoDados.addDadoAttVetor(vetorIteradores.at(i), attVetor, vetorValores.at(i), numero_elementos);
				else if (a_tipoAcesso == TipoAcessoInstancia_membro)
					a_objetoDados.addDadoAttVetorMembro(membro, idMembro, vetorIteradores.at(i), attVetor, vetorValores.at(i), numero_elementos);
				else if (a_tipoAcesso == TipoAcessoInstancia_membroMembro)
					a_objetoDados.addDadoAttVetorMembroMembro(membro, idMembro, membroMembro, idMembroMembro, vetorIteradores.at(i), attVetor, vetorValores.at(i), numero_elementos);
				else if (a_tipoAcesso == TipoAcessoInstancia_membroMembroMembro)
					a_objetoDados.addDadoAttVetorMembroMembroMembro(membro, idMembro, membroMembro, idMembroMembro, membroMembroMembro, idMembroMembroMembro, vetorIteradores.at(i), attVetor, vetorValores.at(i), numero_elementos);
				else if (a_tipoAcesso == TipoAcessoInstancia_membroMembroMembroMembro)
					a_objetoDados.addDadoAttVetorMembroMembroMembroMembro(membro, idMembro, membroMembro, idMembroMembro, membroMembroMembro, idMembroMembroMembro, membroMembroMembroMembro, idMembroMembroMembroMembro, vetorIteradores.at(i), attVetor, vetorValores.at(i), numero_elementos);
			} // for (int i = 0; i < int(vetorValores.size()); i++) {

			lin++;

			vetorValores.clear();

		} // while (lin < numLinhas) {

		// Metodos são chamados para validação pós-leitura.
		if (a_tipoAcesso == TipoAcessoInstancia_direto)
			a_objetoDados.validacaoDadosAttVetor(vetorAttVetor);
		else if (a_tipoAcesso == TipoAcessoInstancia_membro)
			a_objetoDados.validacaoDadosAttVetorMembro(membro, vetorIdMembro, vetorAttVetor);
		else if (a_tipoAcesso == TipoAcessoInstancia_membroMembro)
			a_objetoDados.validacaoDadosAttVetorMembroMembro(membro, vetorIdMembro, membroMembro, vetorIdMembroMembro, vetorAttVetor);
		else if (a_tipoAcesso == TipoAcessoInstancia_membroMembroMembro)
			a_objetoDados.validacaoDadosAttVetorMembroMembroMembro(membro, vetorIdMembro, membroMembro, vetorIdMembroMembro, membroMembroMembro, vetorIdMembroMembroMembro, vetorAttVetor);
		else if (a_tipoAcesso == TipoAcessoInstancia_membroMembroMembroMembro)
			a_objetoDados.validacaoDadosAttVetorMembroMembroMembroMembro(membro, vetorIdMembro, membroMembro, vetorIdMembroMembro, membroMembroMembro, vetorIdMembroMembroMembro, membroMembroMembroMembro, vetorIdMembroMembroMembroMembro, vetorAttVetor);

		leituraArquivo.close();
		leituraArquivo.clear();

		if (imprimirNaTela)
			std::cout << "Sucesso na leitura de " << caminhoArquivo << std::endl;

	} // try

	catch (const std::ifstream::failure& erro) { throw std::invalid_argument("EntradaSaidaDados::carregarArquivoCSV_AttVetor(" + caminhoArquivo + ",a_objetoDados): \nErro de Integridade do Arquivo." + std::string(erro.what())); }
	catch (const std::exception& erro) { throw std::invalid_argument("EntradaSaidaDados::carregarArquivoCSV_AttVetor(" + caminhoArquivo + ",a_objetoDados): \n" + std::string(erro.what())); }

} // void EntradaSaidaDados::carregarArquivoCSV_AttVetor(string a_nomeArquivo, SmartDados* a_objetoDados, const TipoAcessoInstancia a_tipoAcesso){

bool EntradaSaidaDados::carregarArquivoCSV_AttVetor_seExistir(const std::string a_nomeArquivo, SmartDados & a_objetoDados, const TipoAcessoInstancia a_tipoAcesso) const{

	std::string caminhoArquivo = diretorioEntrada + "//" + a_nomeArquivo;
	if (diretorioEntrada == "")
		caminhoArquivo = a_nomeArquivo;

	try {

		std::ifstream leituraArquivo;

		leituraArquivo.open(caminhoArquivo.c_str(), std::ios_base::in);

		if (!leituraArquivo.is_open())
			return false;

		leituraArquivo.close();
		leituraArquivo.clear();

		carregarArquivoCSV_AttVetor(a_nomeArquivo, a_objetoDados, a_tipoAcesso);

		return true;

	} // try

	catch (const std::ifstream::failure& erro) { throw std::invalid_argument("EntradaSaidaDados::carregarArquivoCSV_AttVetor_seExistir(" + caminhoArquivo + ",a_objetoDados," + getString(a_tipoAcesso) + "): \nErro de Integridade do Arquivo." + std::string(erro.what())); }
	catch (const std::exception& erro) { throw std::invalid_argument("EntradaSaidaDados::carregarArquivoCSV_AttVetor_seExistir(" + caminhoArquivo + ",a_objetoDados," + getString(a_tipoAcesso) + "): \n" + std::string(erro.what())); }

} // void EntradaSaidaDados::carregarArquivoCSV_AttVetor_seExistir(const std::string a_nomeArquivo, SmartDados & a_objetoDados, const TipoAcessoInstancia a_tipoAcesso) const{


void EntradaSaidaDados::carregarArquivoCSV_AttMatriz(const std::string a_nomeArquivo, SmartDados &a_objetoDados, const TipoAcessoInstancia a_tipoAcesso)const {

	std::string caminhoArquivo = diretorioEntrada + "//" + a_nomeArquivo;
	if (diretorioEntrada == "")
		caminhoArquivo = a_nomeArquivo;;

	try {

		if (a_tipoAcesso == TipoAcessoInstancia_Nenhum)
			throw std::invalid_argument("Necessario informar o argumento Tipo de Acesso a Instancia.");

		int numMinColunas = 0;

		if (a_tipoAcesso == TipoAcessoInstancia_direto)
			numMinColunas = 3;
		else if (a_tipoAcesso == TipoAcessoInstancia_membro)
			numMinColunas = 4;
		else if (a_tipoAcesso == TipoAcessoInstancia_membroMembro)
			numMinColunas = 5;
		else if (a_tipoAcesso == TipoAcessoInstancia_membroMembroMembro)
			numMinColunas = 6;
		else if (a_tipoAcesso == TipoAcessoInstancia_membroMembroMembroMembro)
			numMinColunas = 7;
		else
			throw std::invalid_argument("Argumento Tipo de Acesso a Instancia invalido.");

		std::ifstream leituraArquivo;

		leituraArquivo.open(caminhoArquivo.c_str(), std::ios_base::in);

		if (!leituraArquivo.is_open())
			throw std::invalid_argument("Nao foi possivel abrir o arquivo.");

		char linhaChar[numCaract];

		std::string   linha; // Linha de dados lida do arquivo.
		std::string   valor; // Valor retirado da linha de dados do arquivo.

		int lin; // Posição da linha da informação ao longo do arquivo de dados.
		size_t pos; // Posição da coluna da informação ao longo da linha de dados.

		std::vector<std::string> cabecalho;

		// Leitura do Cabeçalho
		leituraArquivo.getline(linhaChar, numCaract);
		linha = linhaChar;

		strNormalizada(linha);

		pos = 0;
		while (pos != std::string::npos) {

			pos = linha.find(separadorCSV);

			valor = linha.substr(0, pos).c_str();

			cabecalho.push_back(valor);

			linha = linha.substr(pos + 1, linha.length());

			if (linha == "")
				break;

		} // while (pos != string::npos) {

		if (cabecalho.size() < numMinColunas)
			throw std::invalid_argument("O cabecalho do arquivo deve possuir pelo menos " + std::to_string(numMinColunas) + " colunas.");

		std::string membro = "";
		std::string membroMembro = "";
		std::string membroMembroMembro = "";
		std::string membroMembroMembroMembro = "";

		if (a_tipoAcesso >= TipoAcessoInstancia_membro)
			membro = cabecalho.at(0);

		if (a_tipoAcesso >= TipoAcessoInstancia_membroMembro)
			membroMembro = cabecalho.at(1);

		if (a_tipoAcesso >= TipoAcessoInstancia_membroMembroMembro)
			membroMembroMembro = cabecalho.at(2);

		if (a_tipoAcesso >= TipoAcessoInstancia_membroMembroMembroMembro)
			membroMembroMembroMembro = cabecalho.at(3);

		if (membro.size() > 2) {
			if (membro.substr(0, 2) == "id")
				membro = membro.substr(2, membro.size() - 2);
		} // if (membro.size() > 2) {

		if (membroMembro.size() > 2) {
			if (membroMembro.substr(0, 2) == "id")
				membroMembro = membroMembro.substr(2, membroMembro.size() - 2);
		} // if (membroMembro.size() > 2) {

		if (membroMembroMembro.size() > 2) {
			if (membroMembroMembro.substr(0, 2) == "id")
				membroMembroMembro = membroMembroMembro.substr(2, membroMembroMembro.size() - 2);
		} // if (membroMembroMembro.size() > 2) {

		if (membroMembroMembroMembro.size() > 2) {
			if (membroMembroMembroMembro.substr(0, 2) == "id")
				membroMembroMembroMembro = membroMembroMembroMembro.substr(2, membroMembroMembroMembro.size() - 2);
		} // if (membroMembroMembroMembro.size() > 2) {

		const std::vector<std::string> vetorIteradores2(cabecalho.begin() + numMinColunas - 1, cabecalho.end());
		std::vector<std::string> vetorValores;
		std::vector<std::string> vetorAttMatriz;
		std::vector<std::string> vetorIdMembro;
		std::vector<std::string> vetorIdMembroMembro;
		std::vector<std::string> vetorIdMembroMembroMembro;
		std::vector<std::string> vetorIdMembroMembroMembroMembro;

		// Leitura dados do arquivo
		lin = 2;
		while (lin < numLinhas) {

			leituraArquivo.getline(linhaChar, numCaract);
			linha = linhaChar;

			strNormalizada(linha);

			valor = linha;

			pos = valor.find(separadorCSV);

			if (pos == std::string::npos) {
				if (fimDeArquivo != "") {
					if (!strCompara(valor, fimDeArquivo))
						std::cout << "Aviso! Arquivo " + caminhoArquivo + " nao finalizado com FIM" << std::endl;
				} // if (fimDeArquivo != "") {
				break;
			} // if (pos != string::npos) {

			valor = linha.substr(0, pos).c_str();

			if (fimDeArquivo != "") {
				if (strCompara(valor, fimDeArquivo))
					break;
			} // if (fimDeArquivo != "") {

			std::string idMembro = "";

			if (a_tipoAcesso >= TipoAcessoInstancia_membro) {
				idMembro = valor;

				if (!findStringNoVetor(idMembro, vetorIdMembro))
					vetorIdMembro.push_back(idMembro);

				pos = linha.find(separadorCSV);
				linha = linha.substr(pos + 1, linha.length());
			}

			std::string idMembroMembro = "";

			if (a_tipoAcesso >= TipoAcessoInstancia_membroMembro) {
				pos = linha.find(separadorCSV);
				valor = linha.substr(0, pos).c_str();

				idMembroMembro = valor;

				if (!findStringNoVetor(idMembroMembro, vetorIdMembroMembro))
					vetorIdMembroMembro.push_back(idMembroMembro);

				pos = linha.find(separadorCSV);
				linha = linha.substr(pos + 1, linha.length());

			} // if (a_tipoAcesso >= TipoAcessoInstancia_membroMembro) {

			std::string idMembroMembroMembro = "";

			if (a_tipoAcesso >= TipoAcessoInstancia_membroMembroMembro) {
				pos = linha.find(separadorCSV);
				valor = linha.substr(0, pos).c_str();

				idMembroMembroMembro = valor;

				if (!findStringNoVetor(idMembroMembroMembro, vetorIdMembroMembroMembro))
					vetorIdMembroMembroMembro.push_back(idMembroMembroMembro);

				pos = linha.find(separadorCSV);
				linha = linha.substr(pos + 1, linha.length());

			} // if (a_tipoAcesso >= TipoAcessoInstancia_membroMembroMembro) {

			std::string idMembroMembroMembroMembro = "";

			if (a_tipoAcesso >= TipoAcessoInstancia_membroMembroMembroMembro) {
				pos = linha.find(separadorCSV);
				valor = linha.substr(0, pos).c_str();

				idMembroMembroMembroMembro = valor;

				if (!findStringNoVetor(idMembroMembroMembroMembro, vetorIdMembroMembroMembroMembro))
					vetorIdMembroMembroMembroMembro.push_back(idMembroMembroMembroMembro);

				pos = linha.find(separadorCSV);
				linha = linha.substr(pos + 1, linha.length());
			} // if (a_tipoAcesso >= TipoAcessoInstancia_membroMembroMembroMembro) {

			pos = linha.find(separadorCSV);
			valor = linha.substr(0, pos).c_str();

			const std::string attMatriz = valor;

			if (!findStringNoVetor(attMatriz, vetorAttMatriz))
				vetorAttMatriz.push_back(attMatriz);

			pos = linha.find(separadorCSV);
			linha = linha.substr(pos + 1, linha.length());

			pos = linha.find(separadorCSV);
			valor = linha.substr(0, pos).c_str();

			const std::string iterador1 = valor;

			if (!findStringNoVetor(attMatriz, vetorAttMatriz))
				vetorAttMatriz.push_back(attMatriz);

			pos = linha.find(separadorCSV);
			linha = linha.substr(pos + 1, linha.length());

			int pos_inicial = 0;

			pos = 0;
			while (pos != std::string::npos) {

				pos = linha.find(separadorCSV);

				valor = linha.substr(0, pos).c_str();

				vetorValores.push_back(valor);

				linha = linha.substr(pos + 1, linha.length());

				if (linha == "")
					break;

			} // while (pos != string::npos) {

			if (vetorIteradores2.size() < vetorValores.size())
				throw std::invalid_argument("Numero de colunas no cabecalho menor que o numero de valores na linha " + std::to_string(lin) + ".");

			if (vetorValores.size() == 0) {
				if (a_tipoAcesso == TipoAcessoInstancia_direto)
					a_objetoDados.addDadoAttMatriz(iterador1, "", attMatriz, "", 1, 1);
				else if (a_tipoAcesso == TipoAcessoInstancia_membro)
					a_objetoDados.addDadoAttMatrizMembro(membro, idMembro, iterador1, "", attMatriz, "", 1, 1);
				else if (a_tipoAcesso == TipoAcessoInstancia_membroMembro)
					a_objetoDados.addDadoAttMatrizMembroMembro(membro, idMembro, membroMembro, idMembroMembro, iterador1, "", attMatriz, "", 1, 1);
				else if (a_tipoAcesso == TipoAcessoInstancia_membroMembroMembro)
					a_objetoDados.addDadoAttMatrizMembroMembroMembro(membro, idMembro, membroMembro, idMembroMembro, membroMembroMembro, idMembroMembroMembro, iterador1, "", attMatriz, "", 1, 1);
				else if (a_tipoAcesso == TipoAcessoInstancia_membroMembroMembroMembro)
					a_objetoDados.addDadoAttMatrizMembroMembroMembroMembro(membro, idMembro, membroMembro, idMembroMembro, membroMembroMembro, idMembroMembroMembro, membroMembroMembroMembro, idMembroMembroMembroMembro, iterador1, "", attMatriz, "", 1, 1);
			} // if (vetorValores.size() == 0) {

			else {

				for (int i = 0; i < int(vetorValores.size()); i++) {

					if (vetorValores.at(i) != "") {

						if (a_tipoAcesso == TipoAcessoInstancia_direto)
							a_objetoDados.addDadoAttMatriz(iterador1, vetorIteradores2.at(i), attMatriz, vetorValores.at(i), 1, int(vetorValores.size()));
						else if (a_tipoAcesso == TipoAcessoInstancia_membro)
							a_objetoDados.addDadoAttMatrizMembro(membro, idMembro, iterador1, vetorIteradores2.at(i), attMatriz, vetorValores.at(i), 1, int(vetorValores.size()));
						else if (a_tipoAcesso == TipoAcessoInstancia_membroMembro)
							a_objetoDados.addDadoAttMatrizMembroMembro(membro, idMembro, membroMembro, idMembroMembro, iterador1, vetorIteradores2.at(i), attMatriz, vetorValores.at(i), 1, int(vetorValores.size()));
						else if (a_tipoAcesso == TipoAcessoInstancia_membroMembroMembro)
							a_objetoDados.addDadoAttMatrizMembroMembroMembro(membro, idMembro, membroMembro, idMembroMembro, membroMembroMembro, idMembroMembroMembro, iterador1, vetorIteradores2.at(i), attMatriz, vetorValores.at(i), 1, int(vetorValores.size()));
						else if (a_tipoAcesso == TipoAcessoInstancia_membroMembroMembroMembro)
							a_objetoDados.addDadoAttMatrizMembroMembroMembroMembro(membro, idMembro, membroMembro, idMembroMembro, membroMembroMembro, idMembroMembroMembro, membroMembroMembroMembro, idMembroMembroMembroMembro, iterador1, vetorIteradores2.at(i), attMatriz, vetorValores.at(i), 1, int(vetorValores.size()));

					} // if (vetorValores.at(i) != "") {

				} // for (int i = 0; i < int(vetorValores.size()); i++) {

			}

			lin++;

			vetorValores.clear();

		} // while (lin < numLinhas) {

		// Metodos são chamados para validação pós-leitura.
		if (a_tipoAcesso == TipoAcessoInstancia_direto)
			a_objetoDados.validacaoDadosAttMatriz(vetorAttMatriz);
		else if (a_tipoAcesso == TipoAcessoInstancia_membro)
			a_objetoDados.validacaoDadosAttMatrizMembro(membro, vetorIdMembro, vetorAttMatriz);
		else if (a_tipoAcesso == TipoAcessoInstancia_membroMembro)
			a_objetoDados.validacaoDadosAttMatrizMembroMembro(membro, vetorIdMembro, membroMembro, vetorIdMembroMembro, vetorAttMatriz);
		else if (a_tipoAcesso == TipoAcessoInstancia_membroMembroMembro)
			a_objetoDados.validacaoDadosAttMatrizMembroMembroMembro(membro, vetorIdMembro, membroMembro, vetorIdMembroMembro, membroMembroMembro, vetorIdMembroMembroMembro, vetorAttMatriz);
		else if (a_tipoAcesso == TipoAcessoInstancia_membroMembroMembroMembro)
			a_objetoDados.validacaoDadosAttMatrizMembroMembroMembroMembro(membro, vetorIdMembro, membroMembro, vetorIdMembroMembro, membroMembroMembro, vetorIdMembroMembroMembro, membroMembroMembroMembro, vetorIdMembroMembroMembroMembro, vetorAttMatriz);

		leituraArquivo.close();
		leituraArquivo.clear();

		if (imprimirNaTela)
			std::cout << "Sucesso na leitura de " << caminhoArquivo << std::endl;

	} // try

	catch (const std::ifstream::failure& erro) { throw std::invalid_argument("EntradaSaidaDados::carregarArquivoCSV_AttMatriz(" + caminhoArquivo + ",a_objetoDados): \nErro de Integridade do Arquivo." + std::string(erro.what())); }
	catch (const std::exception& erro) { throw std::invalid_argument("EntradaSaidaDados::carregarArquivoCSV_AttMatriz(" + caminhoArquivo + ",a_objetoDados): \n" + std::string(erro.what())); }

} // void EntradaSaidaDados::carregarArquivoCSV_AttMatriz(string a_nomeArquivo, SmartDados* a_objetoDados, const TipoAcessoInstancia a_tipoAcesso){

bool EntradaSaidaDados::carregarArquivoCSV_AttMatriz_seExistir(const std::string a_nomeArquivo, SmartDados & a_objetoDados, const TipoAcessoInstancia a_tipoAcesso) const{

	std::string caminhoArquivo = diretorioEntrada + "//" + a_nomeArquivo;
	if (diretorioEntrada == "")
		caminhoArquivo = a_nomeArquivo;

	try {

		std::ifstream leituraArquivo;

		leituraArquivo.open(caminhoArquivo.c_str(), std::ios_base::in);

		if (!leituraArquivo.is_open())
			return false;

		leituraArquivo.close();
		leituraArquivo.clear();

		carregarArquivoCSV_AttMatriz(a_nomeArquivo, a_objetoDados, a_tipoAcesso);

		return true;

	} // try

	catch (const std::ifstream::failure& erro) { throw std::invalid_argument("EntradaSaidaDados::carregarArquivoCSV_AttMatriz_seExistir(" + caminhoArquivo + ",a_objetoDados," + getString(a_tipoAcesso) + "): \nErro de Integridade do Arquivo." + std::string(erro.what())); }
	catch (const std::exception& erro) { throw std::invalid_argument("EntradaSaidaDados::carregarArquivoCSV_AttMatriz_seExistir(" + caminhoArquivo + ",a_objetoDados," + getString(a_tipoAcesso) + "): \n" + std::string(erro.what())); }

} // void EntradaSaidaDados::carregarArquivoCSV_AttMatriz_seExistir(const std::string a_nomeArquivo, SmartDados & a_objetoDados, const TipoAcessoInstancia a_tipoAcesso) const{


void EntradaSaidaDados::renomearArquivo(const std::string a_nomeArquivo_antigo, const std::string a_nomeArquivo_novo){

	std::string caminhoArquivo_antigo = diretorioSaida + "//" + a_nomeArquivo_antigo;
	std::string caminhoArquivo_novo   = diretorioSaida + "//" + a_nomeArquivo_novo;

	if (diretorioSaida == "") {
		caminhoArquivo_antigo = a_nomeArquivo_antigo;
		caminhoArquivo_novo = a_nomeArquivo_novo;
	}

	try {

		std::ifstream leituraArquivo;

		int tempo_total = 0;
		bool arquivo_existente = false;
		int arquivo_renomeado = 1;
		while (true) {
			leituraArquivo.open(caminhoArquivo_antigo.c_str(), std::ios_base::in);
			arquivo_existente = leituraArquivo.is_open();

			if (arquivo_existente) {
				if (leituraArquivo.is_open())
					leituraArquivo.close();

				leituraArquivo.clear();

				arquivo_renomeado = rename(caminhoArquivo_antigo.c_str(), caminhoArquivo_novo.c_str());

				if (arquivo_renomeado == 0)
					return;

			} // if (arquivo_existente) {

			if (tempo_total >= tempo_maximo_tentativa_escrita_arquivo)
				throw std::invalid_argument("Nao foi possivel abrir o arquivo para renomeacao apos " + getString(int(tempo_maximo_tentativa_escrita_arquivo / 60000)) + " minutos.");

			aguardarTempo(5000);
			tempo_total += 5000;

		} // while (true) {

		throw std::invalid_argument("Nao foi possivel abrir o arquivo para renomeacao apos " + getString(int(tempo_maximo_tentativa_escrita_arquivo / 60000)) + " minutos.");

	} // try

	catch (const std::ifstream::failure& erro) { throw std::invalid_argument("EntradaSaidaDados::sequenciarNomeArquivoImpressao(" + a_nomeArquivo_antigo + "," + a_nomeArquivo_novo + "): \nErro de Integridade do Arquivo." + std::string(erro.what())); }
	catch (const std::exception& erro) { throw std::invalid_argument("EntradaSaidaDados::sequenciarNomeArquivoImpressao(" + a_nomeArquivo_antigo + "," + a_nomeArquivo_novo + "): \n" + std::string(erro.what())); }


}


std::string EntradaSaidaDados::sequenciarNomeArquivoImpressao(const std::string a_nomeArquivo, const std::string a_extensao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso) {

	std::string nomeArquivo_processador_atual = a_nomeArquivo + "_" + getFullString(a_idProcesso) + "." + a_extensao;
	if (a_idProcesso == a_maiorIdProcesso)
		nomeArquivo_processador_atual = a_nomeArquivo + "." + a_extensao;

	setAppendArquivo(true);

	if (a_idProcesso == IdProcesso_mestre) {
		setAppendArquivo(false);
		return nomeArquivo_processador_atual;
	}

	try {

		std::string caminhoArquivo_processador_anterior = diretorioSaida + "//" + a_nomeArquivo + "_" + getFullString(IdProcesso(a_idProcesso - 1)) + "." + a_extensao;
		std::string caminhoArquivo_processador_atual    = diretorioSaida + "//" + nomeArquivo_processador_atual;
		if (diretorioSaida == "") {
			caminhoArquivo_processador_anterior = a_nomeArquivo + "_" + getFullString(IdProcesso(a_idProcesso - 1)) + "." + a_extensao;
			caminhoArquivo_processador_atual    = nomeArquivo_processador_atual;
		}

		std::ifstream leituraArquivo;

		int tempo_total = 0;
		bool arquivo_existente = false;
		int arquivo_renomeado = 1;
		while (true) {
			leituraArquivo.open(caminhoArquivo_processador_anterior.c_str(), std::ios_base::in);
			arquivo_existente = leituraArquivo.is_open();

			if (arquivo_existente) {
				if (leituraArquivo.is_open())
					leituraArquivo.close();

				leituraArquivo.clear();

				arquivo_renomeado = rename(caminhoArquivo_processador_anterior.c_str(), std::string(caminhoArquivo_processador_atual + "_rw").c_str());

				if (arquivo_renomeado == 0)
					return nomeArquivo_processador_atual;

			} // if (arquivo_existente) {

			if (tempo_total >= 10000000)
				throw std::invalid_argument("Nao foi possivel abrir o arquivo para escrita apos " + getString(int(10000000 / 60000)) + " minutos.");

			aguardarTempo(5000);
			tempo_total += 5000;

		} // while (true) {

		throw std::invalid_argument("Nao foi possivel abrir o arquivo para escrita apos " + getString(int(10000000 / 60000)) + " minutos.");

	} // try

	catch (const std::ifstream::failure& erro) { throw std::invalid_argument("EntradaSaidaDados::sequenciarNomeArquivoImpressao(" + a_nomeArquivo + "," + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + "): \nErro de Integridade do Arquivo." + std::string(erro.what())); }
	catch (const std::exception& erro) { throw std::invalid_argument("EntradaSaidaDados::sequenciarNomeArquivoImpressao(" + a_nomeArquivo + "," + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + "): \n" + std::string(erro.what())); }

} // std::string EntradaSaidaDados::sequenciarNomeArquivoImpressao(const std::string a_nome_arquivo, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso) {


void EntradaSaidaDados::imprimirArquivoCSV_AttComum_(const std::string a_nomeArquivo, const std::string a_membro, const std::string a_membroMembro, const std::string a_membroMembroMembro, const std::string a_membroMembroMembroMembro, SmartDados & a_objetoDados, const std::vector<std::string> a_vetorAttComum)const {

	std::string caminhoArquivo = diretorioSaida + "//" + a_nomeArquivo;
	if (diretorioSaida == "")
		caminhoArquivo = a_nomeArquivo;

	try {

		criarDiretorio(diretorioSaida);

		std::string membro = "";
		std::string idMembro = "";

		getMembroIdMembro(a_membro, membro, idMembro);

		if ((a_membro != "") && ((membro == "") || (idMembro == "")))
			throw std::invalid_argument("Membro ou IdMembro invalidos.");

		std::string membroMembro = "";
		std::string idMembroMembro = "";

		getMembroIdMembro(a_membroMembro, membroMembro, idMembroMembro);

		if ((a_membroMembro != "") && ((membroMembro == "") || (idMembroMembro == "")))
			throw std::invalid_argument("MembroMembro ou IdMembroMembro invalidos.");

		std::string membroMembroMembro = "";
		std::string idMembroMembroMembro = "";

		getMembroIdMembro(a_membroMembroMembro, membroMembroMembro, idMembroMembroMembro);

		if ((a_membroMembroMembro != "") && ((membroMembroMembro == "") || (idMembroMembroMembro == "")))
			throw std::invalid_argument("MembroMembroMembro ou IdMembroMembroMembro invalidos.");

		std::string membroMembroMembroMembro = "";
		std::string idMembroMembroMembroMembro = "";

		getMembroIdMembro(a_membroMembroMembroMembro, membroMembroMembroMembro, idMembroMembroMembroMembro);

		if ((a_membroMembroMembroMembro != "") && ((membroMembroMembroMembro == "") || (idMembroMembroMembroMembro == "")))
			throw std::invalid_argument("MembroMembroMembroMembro ou IdMembroMembroMembroMembro invalidos.");

		TipoAcessoInstancia tipoAcessoInstancia = TipoAcessoInstancia_Nenhum;

		if      ((membro == "") && (membroMembro == "") && (membroMembroMembro == "") && (membroMembroMembroMembro == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_direto;
		else if ((membro != "") && (membroMembro == "") && (membroMembroMembro == "") && (membroMembroMembroMembro == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_membro;
		else if ((membro != "") && (membroMembro != "") && (membroMembroMembro == "") && (membroMembroMembroMembro == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_membroMembro;
		else if ((membro != "") && (membroMembro != "") && (membroMembroMembro != "") && (membroMembroMembroMembro == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_membroMembroMembro;
		else if ((membro != "") && (membroMembro != "") && (membroMembroMembro != "") && (membroMembroMembroMembro != ""))
			tipoAcessoInstancia = TipoAcessoInstancia_membroMembroMembroMembro;
		else
			throw std::invalid_argument("Acesso a instancia invalido.");

		bool incluirCabecalho = true;
		if (appendArquivo)
			incluirCabecalho = false;

		std::vector<std::vector<std::string>> matrizDados;
		if (tipoAcessoInstancia == TipoAcessoInstancia_direto)
			matrizDados = a_objetoDados.getDadosAttComum(incluirCabecalho, a_vetorAttComum);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_membro)
			matrizDados = a_objetoDados.getDadosAttComumMembro(incluirCabecalho, membro, idMembro, a_vetorAttComum);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_membroMembro)
			matrizDados = a_objetoDados.getDadosAttComumMembroMembro(incluirCabecalho, membro, idMembro, membroMembro, idMembroMembro, a_vetorAttComum);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_membroMembroMembro)
			matrizDados = a_objetoDados.getDadosAttComumMembroMembroMembro(incluirCabecalho, membro, idMembro, membroMembro, idMembroMembro, membroMembroMembro, idMembroMembroMembro, a_vetorAttComum);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_membroMembroMembroMembro)
			matrizDados = a_objetoDados.getDadosAttComumMembroMembroMembroMembro(incluirCabecalho, membro, idMembro, membroMembro, idMembroMembro, membroMembroMembro, idMembroMembroMembro, membroMembroMembroMembro, idMembroMembroMembroMembro, a_vetorAttComum);

		const int numLin = int(matrizDados.size());
		if (numLin == 0)
			throw std::invalid_argument("Numero insuficiente de linhas na matriz de dados.");

		const int numCol = int(matrizDados.at(0).size());
		if (numCol == 0)
			throw std::invalid_argument("Numero insuficiente de linhas na matriz de dados.");

		std::ofstream escritaArquivo;

		std::string caminhoArquivo_wr = caminhoArquivo + "_rw";

		int tempo_total = 0;
		bool arquivo_existente = false;
		int arquivo_renomeado = 1;
		while (true) {
			escritaArquivo.open(caminhoArquivo.c_str(), std::ios_base::app);
			arquivo_existente = escritaArquivo.is_open();

			if (arquivo_existente) {
				if (escritaArquivo.is_open())
					escritaArquivo.close();

				arquivo_renomeado = rename(caminhoArquivo.c_str(), caminhoArquivo_wr.c_str());

				if (arquivo_renomeado == 0)
					break;
			} // if (arquivo_existente) {

			if (tempo_total >= tempo_maximo_tentativa_escrita_arquivo)
				throw std::invalid_argument("Nao foi possivel abrir o arquivo para escrita apos " + getString(int(tempo_maximo_tentativa_escrita_arquivo / 60000)) + " minutos.");

			aguardarTempo(5000);
			tempo_total += 5000;

		} // while (true) {

		if (appendArquivo)
			escritaArquivo.open(caminhoArquivo_wr.c_str(), std::ios_base::app);
		else
			escritaArquivo.open(caminhoArquivo_wr.c_str(), std::ios_base::out);

		if (!escritaArquivo.is_open())
			throw std::invalid_argument("Nao foi possivel abrir o arquivo " + caminhoArquivo_wr + ".");

		if (tipoAcessoInstancia == TipoAcessoInstancia_direto) {
			for (int col = 0; col < numCol; col++){
				escritaArquivo << matrizDados.at(0).at(col) << separadorCSV;
				escritaArquivo << matrizDados.at(1).at(col) << separadorCSV;
				escritaArquivo << std::endl;
			} // for (int col = 0; col < numCol; col++){
		}

		else {
			for (int lin = 0; lin < numLin; lin++) {
				for (int col = 0; col < numCol; col++)
					escritaArquivo << matrizDados.at(lin).at(col) << separadorCSV;
				escritaArquivo << std::endl;
			} // for (int lin = 0; lin < numLin; lin++) {
		}

		if (fimDeArquivo != "")
			escritaArquivo << fimDeArquivo;

		escritaArquivo.close();
		escritaArquivo.clear();

		if (rename(caminhoArquivo_wr.c_str(), caminhoArquivo.c_str()) != 0)
			throw std::invalid_argument("Nao foi possivel renomear o arquivo " + caminhoArquivo_wr + " para " + caminhoArquivo + ".");

		if (imprimirNaTela)
			std::cout << "Sucesso na impressao de " << caminhoArquivo << std::endl;

	} // try

	catch (const std::ifstream::failure& erro) { throw std::invalid_argument("EntradaSaidaDados::imprimirArquivoCSV_AttComum_(" + caminhoArquivo + "," + a_membro + "," + a_membroMembro + "," + a_membroMembroMembro + "," + a_membroMembroMembroMembro + ",a_objetoDados,a_vetorAttComum): \nErro de Integridade do Arquivo." + std::string(erro.what())); }
	catch (const std::exception& erro) { throw std::invalid_argument("EntradaSaidaDados::imprimirArquivoCSV_AttComum_(" + caminhoArquivo + ", " + a_membro + ", " + a_membroMembro + ", " + a_membroMembroMembro + "," + a_membroMembroMembroMembro + ", a_objetoDados,a_vetorAttComum) : \n" + std::string(erro.what())); }

} // void EntradaSaidaDados::imprimirArquivoCSV_AttComum_(const std::string a_nomeArquivo, std::string a_membro, std::string a_membroMembro, std::string a_membroMembroMembro, SmartDados & a_objetoDados, const std::string a_attComum){


void EntradaSaidaDados::imprimirArquivoCSV_AttVetor_(const std::string a_nomeArquivo, const std::string a_membro, const std::string a_membroMembro, const std::string a_membroMembroMembro, const std::string a_membroMembroMembroMembro, SmartDados & a_objetoDados, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const std::vector<std::string> a_vetorAttVetor)const {

	std::string caminhoArquivo = diretorioSaida + "//" + a_nomeArquivo;
	if (diretorioSaida == "")
		caminhoArquivo = a_nomeArquivo;

	try {

		criarDiretorio(diretorioSaida);

		std::string membro   = "";
		std::string idMembro = "";
			
		getMembroIdMembro(a_membro, membro, idMembro);

		if ((a_membro != "") && ((membro == "") || (idMembro == "")))
			throw std::invalid_argument("Membro ou IdMembro invalidos.");

		std::string membroMembro   = "";
		std::string idMembroMembro = "";

		getMembroIdMembro(a_membroMembro, membroMembro, idMembroMembro);

		if ((a_membroMembro != "") && ((membroMembro == "") || (idMembroMembro == "")))
			throw std::invalid_argument("MembroMembro ou IdMembroMembro invalidos.");

		std::string membroMembroMembro   = "";
		std::string idMembroMembroMembro = "";

		getMembroIdMembro(a_membroMembroMembro, membroMembroMembro, idMembroMembroMembro);

		if ((a_membroMembroMembro != "") && ((membroMembroMembro == "") || (idMembroMembroMembro == "")))
			throw std::invalid_argument("MembroMembroMembro ou IdMembroMembroMembro invalidos.");

		std::string membroMembroMembroMembro = "";
		std::string idMembroMembroMembroMembro = "";

		getMembroIdMembro(a_membroMembroMembroMembro, membroMembroMembroMembro, idMembroMembroMembroMembro);

		if ((a_membroMembroMembroMembro != "") && ((membroMembroMembroMembro == "") || (idMembroMembroMembroMembro == "")))
			throw std::invalid_argument("MembroMembroMembroMembro ou IdMembroMembroMembroMembro invalidos.");

		TipoAcessoInstancia tipoAcessoInstancia = TipoAcessoInstancia_Nenhum;

		if ((membro == "") && (membroMembro == "") && (membroMembroMembro == "") && (membroMembroMembroMembro == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_direto;
		else if ((membro != "") && (membroMembro == "") && (membroMembroMembro == "") && (membroMembroMembroMembro == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_membro;
		else if ((membro != "") && (membroMembro != "") && (membroMembroMembro == "") && (membroMembroMembroMembro == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_membroMembro;
		else if ((membro != "") && (membroMembro != "") && (membroMembroMembro != "") && (membroMembroMembroMembro == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_membroMembroMembro;
		else if ((membro != "") && (membroMembro != "") && (membroMembroMembro != "") && (membroMembroMembroMembro != ""))
			tipoAcessoInstancia = TipoAcessoInstancia_membroMembroMembroMembro;
		else
			throw std::invalid_argument("Acesso a instancia invalido.");

		bool incluirCabecalho = true;
		if (appendArquivo)
			incluirCabecalho = false;

		std::vector<std::vector<std::string>> matrizDados;
		if (tipoAcessoInstancia == TipoAcessoInstancia_direto)
			matrizDados = a_objetoDados.getDadosAttVetor(incluirCabecalho, a_iteradorInicial, a_iteradorFinal, true, a_vetorAttVetor);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_membro)
			matrizDados = a_objetoDados.getDadosAttVetorMembro(incluirCabecalho, membro, idMembro, a_iteradorInicial, a_iteradorFinal, true, a_vetorAttVetor);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_membroMembro)
			matrizDados = a_objetoDados.getDadosAttVetorMembroMembro(incluirCabecalho, membro, idMembro, membroMembro, idMembroMembro, a_iteradorInicial, a_iteradorFinal, true, a_vetorAttVetor);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_membroMembroMembro)
			matrizDados = a_objetoDados.getDadosAttVetorMembroMembroMembro(incluirCabecalho, membro, idMembro, membroMembro, idMembroMembro, membroMembroMembro, idMembroMembroMembro, a_iteradorInicial, a_iteradorFinal, true, a_vetorAttVetor);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_membroMembroMembroMembro)
			matrizDados = a_objetoDados.getDadosAttVetorMembroMembroMembroMembro(incluirCabecalho, membro, idMembro, membroMembro, idMembroMembro, membroMembroMembro, idMembroMembroMembro, membroMembroMembroMembro, idMembroMembroMembroMembro, a_iteradorInicial, a_iteradorFinal, true, a_vetorAttVetor);

		const int numLin = int(matrizDados.size());
		if (numLin == 0)
			return;

		int numCol = int(matrizDados.at(0).size());
		for (int lin = 0; lin < numLin; lin++) {
			if (int(matrizDados.at(lin).size()) > numCol)
				numCol = int(matrizDados.at(lin).size());
		}

		if (numCol == 0)
			return;

		std::ofstream escritaArquivo;

		std::string caminhoArquivo_wr = caminhoArquivo + "_rw";

		int tempo_total = 0;
		bool arquivo_existente = false;
		int arquivo_renomeado = 1;
		while (true) {
			escritaArquivo.open(caminhoArquivo.c_str(), std::ios_base::app);
			arquivo_existente = escritaArquivo.is_open();

			if (arquivo_existente) {
				if (escritaArquivo.is_open())
					escritaArquivo.close();

				arquivo_renomeado = rename(caminhoArquivo.c_str(), caminhoArquivo_wr.c_str());

				if (arquivo_renomeado == 0)
					break;
			} // if (arquivo_existente) {

			if (tempo_total >= tempo_maximo_tentativa_escrita_arquivo)
				throw std::invalid_argument("Nao foi possivel abrir o arquivo para escrita apos " + getString(int(tempo_maximo_tentativa_escrita_arquivo / 60000)) + " minutos.");

			aguardarTempo(5000);
			tempo_total += 5000;

		} // while (true) {

		if (appendArquivo)
			escritaArquivo.open(caminhoArquivo_wr.c_str(), std::ios_base::app);
		else
			escritaArquivo.open(caminhoArquivo_wr.c_str(), std::ios_base::out);

		if (!escritaArquivo.is_open())
			throw std::invalid_argument("Nao foi possivel abrir o arquivo " + caminhoArquivo_wr + ".");


		for (int lin = 0; lin < numLin; lin++) {

			std::string escritaArquivo_linha;
			int contador_coluna_vazia = 0;

			for (int col = 0; col < numCol; col++) {
				if (int(matrizDados.at(lin).size()) > col)
					if (matrizDados.at(lin).at(col) != "")
						escritaArquivo_linha += matrizDados.at(lin).at(col) + separadorCSV;
					else {
						escritaArquivo_linha += separadorCSV;
						contador_coluna_vazia++;
					}
				else {
					escritaArquivo_linha += separadorCSV;
					contador_coluna_vazia++;
				}
			}

			if ((numCol - contador_coluna_vazia) > int(tipoAcessoInstancia))
				escritaArquivo << escritaArquivo_linha << std::endl;

		} // for (int lin = 0; lin < numLin; lin++) {

		if (fimDeArquivo != "")
			escritaArquivo << fimDeArquivo;

		escritaArquivo.close();
		escritaArquivo.clear();

		if (rename(caminhoArquivo_wr.c_str(), caminhoArquivo.c_str()) != 0)
			throw std::invalid_argument("Nao foi possivel renomear o arquivo " + caminhoArquivo_wr + " para " + caminhoArquivo + ".");

		if (imprimirNaTela)
			std::cout << "Sucesso na impressao de " << caminhoArquivo << std::endl;

	} // try

	catch (const std::ifstream::failure& erro) { throw std::invalid_argument("EntradaSaidaDados::imprimirArquivoCSV_AttVetor_(" + caminhoArquivo + "," + a_membro + "," + a_membroMembro + "," + a_membroMembroMembro + "," + a_membroMembroMembroMembro + ",a_objetoDados," + a_iteradorInicial + "," + a_iteradorFinal + ",a_vetorAttVetor): \nErro de Integridade do Arquivo." + std::string(erro.what())); }
	catch (const std::exception& erro) { throw std::invalid_argument("EntradaSaidaDados::imprimirArquivoCSV_AttVetor_(" + caminhoArquivo + "," + a_membro + "," + a_membroMembro + "," + a_membroMembroMembro + "," + a_membroMembroMembroMembro + ",a_objetoDados," + a_iteradorInicial + "," + a_iteradorFinal + ",a_vetorAttVetor): \n" + std::string(erro.what())); }

} // void EntradaSaidaDados::imprimirArquivoCSV_AttVetor_(const std::string a_nomeArquivo, const std::string a_membro, const std::string a_membroMembro, const std::string a_membroMembroMembro, SmartDados & a_objetoDados, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const std::string a_attVetor){


void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz_(const std::string a_nomeArquivo, const std::string a_membro, const std::string a_membroMembro, const std::string a_membroMembroMembro, const std::string a_membroMembroMembroMembro, SmartDados & a_objetoDados, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const std::vector<std::string> a_vetorAttMatriz)const {

	std::string caminhoArquivo = diretorioSaida + "//" + a_nomeArquivo;
	if (diretorioSaida == "")
		caminhoArquivo = a_nomeArquivo;

	try {

		criarDiretorio(diretorioSaida);

		std::string membro = "";
		std::string idMembro = "";

		getMembroIdMembro(a_membro, membro, idMembro);

		if ((a_membro != "") && ((membro == "") || (idMembro == "")))
			throw std::invalid_argument("Membro ou IdMembro invalidos.");

		std::string membroMembro = "";
		std::string idMembroMembro = "";

		getMembroIdMembro(a_membroMembro, membroMembro, idMembroMembro);

		if ((a_membroMembro != "") && ((membroMembro == "") || (idMembroMembro == "")))
			throw std::invalid_argument("MembroMembro ou IdMembroMembro invalidos.");

		std::string membroMembroMembro = "";
		std::string idMembroMembroMembro = "";

		getMembroIdMembro(a_membroMembroMembro, membroMembroMembro, idMembroMembroMembro);

		if ((a_membroMembroMembro != "") && ((membroMembroMembro == "") || (idMembroMembroMembro == "")))
			throw std::invalid_argument("MembroMembroMembro ou IdMembroMembroMembro invalidos.");

		std::string membroMembroMembroMembro = "";
		std::string idMembroMembroMembroMembro = "";

		getMembroIdMembro(a_membroMembroMembroMembro, membroMembroMembroMembro, idMembroMembroMembroMembro);

		if ((a_membroMembroMembroMembro != "") && ((membroMembroMembroMembro == "") || (idMembroMembroMembroMembro == "")))
			throw std::invalid_argument("MembroMembroMembroMembro ou IdMembroMembroMembroMembro invalidos.");

		TipoAcessoInstancia tipoAcessoInstancia = TipoAcessoInstancia_Nenhum;

		if ((membro == "") && (membroMembro == "") && (membroMembroMembro == "") && (membroMembroMembroMembro == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_direto;
		else if ((membro != "") && (membroMembro == "") && (membroMembroMembro == "") && (membroMembroMembroMembro == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_membro;
		else if ((membro != "") && (membroMembro != "") && (membroMembroMembro == "") && (membroMembroMembroMembro == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_membroMembro;
		else if ((membro != "") && (membroMembro != "") && (membroMembroMembro != "") && (membroMembroMembroMembro == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_membroMembroMembro;
		else if ((membro != "") && (membroMembro != "") && (membroMembroMembro != "") && (membroMembroMembroMembro != ""))
			tipoAcessoInstancia = TipoAcessoInstancia_membroMembroMembroMembro;
		else
			throw std::invalid_argument("Acesso a instancia invalido.");

		bool incluirCabecalho = true;
		if (appendArquivo)
			incluirCabecalho = false;

		std::vector<std::vector<std::string>> matrizDados;
		if (tipoAcessoInstancia == TipoAcessoInstancia_direto)
			matrizDados = a_objetoDados.getDadosAttMatriz(incluirCabecalho, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, true, a_vetorAttMatriz);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_membro)
			matrizDados = a_objetoDados.getDadosAttMatrizMembro(incluirCabecalho, membro, idMembro, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, true, a_vetorAttMatriz);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_membroMembro)
			matrizDados = a_objetoDados.getDadosAttMatrizMembroMembro(incluirCabecalho, membro, idMembro, membroMembro, idMembroMembro, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, true, a_vetorAttMatriz);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_membroMembroMembro)
			matrizDados = a_objetoDados.getDadosAttMatrizMembroMembroMembro(incluirCabecalho, membro, idMembro, membroMembro, idMembroMembro, membroMembroMembro, idMembroMembroMembro, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, true, a_vetorAttMatriz);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_membroMembroMembroMembro)
			matrizDados = a_objetoDados.getDadosAttMatrizMembroMembroMembroMembro(incluirCabecalho, membro, idMembro, membroMembro, idMembroMembro, membroMembroMembro, idMembroMembroMembro, membroMembroMembroMembro, idMembroMembroMembroMembro, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, true, a_vetorAttMatriz);

		const int numLin = int(matrizDados.size());
		if (numLin == 0)
			return;

		int numCol = 0;
		for (numCol = 1; numCol <= int(matrizDados.at(0).size()); numCol++) {
			if (matrizDados.at(0).at(numCol - 1) == "") {
				numCol--;
				break;
			}
		}
		
		if (numCol == 0)
			return;

		std::ofstream escritaArquivo;

		std::string caminhoArquivo_wr = caminhoArquivo + "_rw";

		int tempo_total = 0;
		bool arquivo_existente = false;
		int arquivo_renomeado = 1;
		while (true) {
			escritaArquivo.open(caminhoArquivo.c_str(), std::ios_base::app);
			arquivo_existente = escritaArquivo.is_open();

			if (arquivo_existente) {
				if (escritaArquivo.is_open())
					escritaArquivo.close();

				arquivo_renomeado = rename(caminhoArquivo.c_str(), caminhoArquivo_wr.c_str());

				if (arquivo_renomeado == 0)
					break;
			} // if (arquivo_existente) {

			if (tempo_total >= tempo_maximo_tentativa_escrita_arquivo)
				throw std::invalid_argument("Nao foi possivel abrir o arquivo para escrita apos " + getString(int(tempo_maximo_tentativa_escrita_arquivo / 60000)) + " minutos.");

			aguardarTempo(5000);
			tempo_total += 5000;

		} // while (true) {

		if (appendArquivo)
			escritaArquivo.open(caminhoArquivo_wr.c_str(), std::ios_base::app);
		else
			escritaArquivo.open(caminhoArquivo_wr.c_str(), std::ios_base::out);

		if (!escritaArquivo.is_open())
			throw std::invalid_argument("Nao foi possivel abrir o arquivo " + caminhoArquivo_wr + ".");

		for (int lin = 0; lin < numLin; lin++) {

			std::string escritaArquivo_linha;
			int contador_coluna_vazia = 0;

			for (int col = 0; col < numCol; col++) {
				if (lin < int(matrizDados.size()) && int(col < matrizDados.at(lin).size())) {
					if (matrizDados.at(lin).at(col) != "")
						escritaArquivo_linha += matrizDados.at(lin).at(col) + separadorCSV;
					else {
						escritaArquivo_linha += separadorCSV;
						contador_coluna_vazia++;
					}
				}
				else {
					escritaArquivo_linha += separadorCSV;
					contador_coluna_vazia++;
				}
			}

			if ((numCol - contador_coluna_vazia) > (int(tipoAcessoInstancia) + 1))
				escritaArquivo << escritaArquivo_linha << std::endl;

		} // for (int lin = 0; lin < numLin; lin++) {

		if (fimDeArquivo != "")
			escritaArquivo << fimDeArquivo;

		escritaArquivo.close();
		escritaArquivo.clear();

		if (rename(caminhoArquivo_wr.c_str(), caminhoArquivo.c_str()) != 0)
			throw std::invalid_argument("Nao foi possivel renomear o arquivo " + caminhoArquivo_wr + " para " + caminhoArquivo + ".");

		if (imprimirNaTela)
			std::cout << "Sucesso na impressao de " << caminhoArquivo << std::endl;

	} // try

	catch (const std::ifstream::failure& erro) { throw std::invalid_argument("EntradaSaidaDados::imprimirArquivoCSV_AttMatriz_(" + caminhoArquivo + "," + a_membro + "," + a_membroMembro + "," + a_membroMembroMembro + "," + a_membroMembroMembroMembro + ",a_objetoDados," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + ",a_vetorAttMatriz): \nErro de Integridade do Arquivo." + std::string(erro.what())); }
	catch (const std::exception& erro) { throw std::invalid_argument("EntradaSaidaDados::imprimirArquivoCSV_AttMatriz_(" + caminhoArquivo + "," + a_membro + "," + a_membroMembro + "," + a_membroMembroMembro + "," + a_membroMembroMembroMembro + ",a_objetoDados," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + ",a_vetorAttMatriz): \n" + std::string(erro.what())); }

} // void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz_(const std::string a_nomeArquivo, const std::string a_membro, const std::string a_membroMembro, const std::string a_membroMembroMembro, SmartDados & a_objetoDados, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const std::string a_attMatriz){


void EntradaSaidaDados::imprimirConsolidacaoHorizontalCSV(const std::string a_nome_arquivo_resultante, const std::vector<std::string>& a_lista_arquivos_originais, const int a_numero_colunas_suprimir, const bool a_apagar_arquivos_originais) const{


	std::string caminhoArquivo = diretorioSaida + "//" + a_nome_arquivo_resultante;
	if (diretorioSaida == "")
		caminhoArquivo = a_nome_arquivo_resultante;

	try {

		if (a_lista_arquivos_originais.size() == 0)
			throw std::invalid_argument("A lista de arquivos a serem consolidados esta vazia.");

		if (a_numero_colunas_suprimir < 0)
			throw std::invalid_argument("O numero de colunas a suprimir deve ser maior que 0.");

		std::ofstream escritaArquivo;

		escritaArquivo.open(caminhoArquivo.c_str(), std::ios_base::out);

		if (!escritaArquivo.is_open())
			return;

		std::vector<int> indice_arquivos_originais;
		std::vector<std::ifstream> lista_escritaArquivo;

		std::vector<int> arquivos_nao_abertos;
		for (int i = 0; i < int(a_lista_arquivos_originais.size()); i++) {

			const std::string caminhoArquivo_original = diretorioSaida + "//" + a_lista_arquivos_originais.at(i);

			std::ifstream escritaArquivo;
			escritaArquivo.open(caminhoArquivo_original.c_str(), std::ios_base::in);

			if (!escritaArquivo.is_open())
				arquivos_nao_abertos.push_back(i);
			else {
				escritaArquivo.close();
				escritaArquivo.clear();
				lista_escritaArquivo.push_back(std::ifstream());
				lista_escritaArquivo.at(lista_escritaArquivo.size() - 1).open(caminhoArquivo_original.c_str(), std::ios_base::in);
				indice_arquivos_originais.push_back(i);
			}
		
		}// for (int i = 0; i < int(a_lista_arquivos_originais.size()); i++) {

		if (arquivos_nao_abertos.size() == a_lista_arquivos_originais.size())
			return;
		//else if (arquivos_nao_abertos.size() > 0)
			//throw std::invalid_argument("Nao foi possivel abrir o arquivo " + a_lista_arquivos_originais.at(arquivos_nao_abertos.at(0)) + ".");

		std::string linha;

		while (std::getline(lista_escritaArquivo.at(0), linha)) {

			strNormalizada(linha);

			for (int i = 1; i < int(lista_escritaArquivo.size()); i++) {

				std::string linha_inner;

				std::getline(lista_escritaArquivo.at(i), linha_inner);

				strNormalizada(linha_inner);

				for (int c = 1; c <= a_numero_colunas_suprimir; c++) {
					const size_t pos = linha_inner.find(separadorCSV);
					if (pos == std::string::npos)
						break;
					linha_inner = linha_inner.substr(pos + 1, linha_inner.length());
				} // for (int c = 1; c <= a_numero_colunas_suprimir; c++) {
			
				if (linha.length() > 0) {
					if (linha.at(linha.length() - 1) != separadorCSV.c_str()[0])
						linha = linha + separadorCSV + linha_inner;
					else
						linha = linha + linha_inner;
				}
				else
					linha = linha_inner;

			} // for (int i = 1; i < int(lista_escritaArquivo.size()); i++) {

			escritaArquivo << linha << std::endl;

		} // while (std::getline(lista_escritaArquivo.at(0), linha)) {

		if (fimDeArquivo != "")
			escritaArquivo << fimDeArquivo;

		for (int i = 0; i < int(lista_escritaArquivo.size()); i++) {

			lista_escritaArquivo.at(i).close();
			lista_escritaArquivo.at(i).clear();

			if (a_apagar_arquivos_originais) {

				const std::string caminhoArquivo_original = diretorioSaida + "//" + a_lista_arquivos_originais.at(indice_arquivos_originais.at(i));

				std::ofstream escrita_zerar_arquivo;

				escrita_zerar_arquivo.open(caminhoArquivo_original.c_str(), std::ios_base::out);

				if (escrita_zerar_arquivo.is_open()) {
					escrita_zerar_arquivo << "";
					escrita_zerar_arquivo.close();
				} // if (escrita_zerar_arquivo.is_open()) {

				escrita_zerar_arquivo.clear();

				deletarArquivo(caminhoArquivo_original.c_str());

			} // if (a_apagar_arquivos_originais) {

		} // for (int i = 0; i < int(a_lista_arquivos_originais.size()); i++) {

		escritaArquivo.close();
		escritaArquivo.clear();


		if (imprimirNaTela)
			std::cout << "Sucesso na consolidacao de " << caminhoArquivo << std::endl;

	} // try

	catch (const std::ifstream::failure & erro) { throw std::invalid_argument("EntradaSaidaDados::imprimirConsolidacaoHorizontalCSV(" + caminhoArquivo + ",a_matriz_string," + getString(a_apagar_arquivos_originais) + "): \nErro de Integridade do Arquivo." + std::string(erro.what())); }
	catch (const std::exception & erro) { throw std::invalid_argument("EntradaSaidaDados::imprimirConsolidacaoHorizontalCSV(" + caminhoArquivo + ",a_matriz_string," + getString(a_apagar_arquivos_originais) + "): \n" + std::string(erro.what())); }

} // void EntradaSaidaDados::imprimirConsolidacaoHorizontalCSV(const std::string a_nome_arquivo_resultante, const std::vector<std::string>& a_lista_arquivos_originais, const bool a_apagar_arquivos_originais) const{


void EntradaSaidaDados::imprimirArquivoCSV_SmartEnupla(const std::string a_nomeArquivo, const std::vector<std::vector<std::string>>& a_matriz_string) const{ imprimirArquivoCSV_SmartEnupla(a_nomeArquivo, a_matriz_string, true); }

void EntradaSaidaDados::imprimirArquivoCSV_SmartEnupla(const std::string a_nomeArquivo, const std::vector<std::vector<std::string>>& a_matriz_string, const bool a_incluir_cabecalho) const {

	std::string caminhoArquivo = diretorioSaida + "//" + a_nomeArquivo;
	if (diretorioSaida == "")
		caminhoArquivo = a_nomeArquivo;

	try {

		bool incluirCabecalho = a_incluir_cabecalho;
		if (appendArquivo)
			incluirCabecalho = false;

		const int numLin = int(a_matriz_string.size());

		if (numLin == 0)
			return;

		criarDiretorio(diretorioSaida);

		std::ofstream escritaArquivo;

		std::string caminhoArquivo_wr = caminhoArquivo + "_rw";

		int tempo_total = 0;
		bool arquivo_existente = false;
		int arquivo_renomeado = 1;
		while (true) {
			escritaArquivo.open(caminhoArquivo.c_str(), std::ios_base::app);
			arquivo_existente = escritaArquivo.is_open();

			if (arquivo_existente) {
				if (escritaArquivo.is_open())
					escritaArquivo.close();

				arquivo_renomeado = rename(caminhoArquivo.c_str(), caminhoArquivo_wr.c_str());

				if (arquivo_renomeado == 0)
					break;
			} // if (arquivo_existente) {

			if (tempo_total >= tempo_maximo_tentativa_escrita_arquivo)
				throw std::invalid_argument("Nao foi possivel abrir o arquivo para escrita apos " + getString(int(tempo_maximo_tentativa_escrita_arquivo / 60000)) + " minutos.");

			aguardarTempo(5000);
			tempo_total += 5000;

		} // while (true) {

		if (appendArquivo)
			escritaArquivo.open(caminhoArquivo_wr.c_str(), std::ios_base::app);
		else
			escritaArquivo.open(caminhoArquivo_wr.c_str(), std::ios_base::out);

		if (!escritaArquivo.is_open())
			throw std::invalid_argument("Nao foi possivel abrir o arquivo " + caminhoArquivo_wr + ".");

		int primeira_coluna_IdCenario = -1;

		for (int c = 0; c < a_matriz_string.at(0).size(); c++) {
			if (a_matriz_string.at(0).at(c).find("IdCenario") != std::string::npos) {
				primeira_coluna_IdCenario = c;
				break;
			}
		} // for (int c = 0; c < a_matriz_string.at(0).size(); c++) {

		for (int lin = 0; lin < numLin; lin++) {

			if ((incluirCabecalho) || (lin > 0)) {

				const int numCol = int(a_matriz_string.at(lin).size());

				bool imprimir_linha = true;
				if ((primeira_coluna_IdCenario > -1) && (numCol > primeira_coluna_IdCenario)) {
					if (a_matriz_string.at(lin).at(primeira_coluna_IdCenario) == "") {
						imprimir_linha = false;
						for (int col = primeira_coluna_IdCenario + 1; col < numCol; col++) {
							if (a_matriz_string.at(lin).at(col) != "") {
								imprimir_linha = true;
								break;
							}
						}
					}
				}

				if (imprimir_linha) {

					for (int col = 0; col < numCol; col++)
						escritaArquivo << a_matriz_string.at(lin).at(col) << separadorCSV;

					escritaArquivo << std::endl;

				}// if (imprimir_linha) {

			} // if ((incluirCabecalho) || (lin > 0)) {
		} // for (int lin = 0; lin < numLin; lin++) {

		if (fimDeArquivo != "")
			escritaArquivo << fimDeArquivo;

		escritaArquivo.close();
		escritaArquivo.clear();

		if (rename(caminhoArquivo_wr.c_str(), caminhoArquivo.c_str()) != 0)
			throw std::invalid_argument("Nao foi possivel renomear o arquivo " + caminhoArquivo_wr + " para " + caminhoArquivo + ".");

		if (imprimirNaTela)
			std::cout << "Sucesso na impressao de " << caminhoArquivo << std::endl;

	} // try

	catch (const std::ifstream::failure& erro) { throw std::invalid_argument("EntradaSaidaDados::imprimirArquivoCSV_SmartEnupla(" + caminhoArquivo + ",a_matriz_string," + getString(a_incluir_cabecalho) + "): \nErro de Integridade do Arquivo." + std::string(erro.what())); }
	catch (const std::exception& erro) { throw std::invalid_argument("EntradaSaidaDados::imprimirArquivoCSV_SmartEnupla(" + caminhoArquivo + ",a_matriz_string," + getString(a_incluir_cabecalho) + "): \n" + std::string(erro.what())); }

} // void EntradaSaidaDados::imprimirArquivoCSV_SmartEnupla(const std::string a_nomeArquivo, const std::vector<std::vector<std::string>>& a_matriz_string, const bool a_incluir_cabecalho) const{


void EntradaSaidaDados::getMembroIdMembro(const std::string a_string, std::string & a_membro, std::string & a_idMembro)const {

	if (a_string.empty()) {
		a_membro   = "";
		a_idMembro = "";
		return;
	} // if (a_string.empty()) {

	size_t posId, pos_;

	posId = a_string.find("Id");
	pos_  = a_string.find("_");

	if ((posId == std::string::npos) || (pos_ == std::string::npos)) {
		a_membro   = "";
		a_idMembro = "";
	}

	else {
		a_membro   = a_string.substr(posId + 2, pos_ - 2);
		a_idMembro = a_string.substr(pos_ + 1, a_string.length());
	}

} // void EntradaSaidaDados::getMembroIdMembro(const std::string a_string, std::string & a_membro, std::string & a_idMembro){


