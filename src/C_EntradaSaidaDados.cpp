#include "C_EntradaSaidaDados.h"

#include <iostream>
#include <fstream>

// Um número de caracteres muito grande pode estourar o buffer da string;

static int const numCaract = 300000;
static int const numLinhas = 5000000;

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
		else if (a_tipoAcesso == TipoAcessoInstancia_m1)
			numMinColunas = 2;
		else if (a_tipoAcesso == TipoAcessoInstancia_m2)
			numMinColunas = 3;
		else if (a_tipoAcesso == TipoAcessoInstancia_m3)
			numMinColunas = 4;
		else if (a_tipoAcesso == TipoAcessoInstancia_m4)
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

			cabecalho.push_back(remEspacosEntreStr(valor));

			linha = linha.substr(pos + 1, linha.length());

			if (linha == "")
				break;

		} // while (pos != string::npos) {

		if (cabecalho.size() < numMinColunas)
			throw std::invalid_argument("O cabecalho do arquivo deve possuir pelo menos " + std::to_string(numMinColunas) + " colunas.");

		std::string membro = "";
		std::string membroMembro = "";
		std::string membroM2 = "";
		std::string membroM3 = "";

		std::vector<std::string> vetorValores;
		std::vector<std::string> vetorAttComuns;

		std::vector<std::string> vetorIdMembro;
		std::vector<std::string> vetorIdM2;
		std::vector<std::string> vetorIdM3;
		std::vector<std::string> vetorIdM4;

		if (a_tipoAcesso == TipoAcessoInstancia_direto) {

			  vetorValores.push_back(cabecalho.at(1));
			vetorAttComuns.push_back(cabecalho.at(0));

		} // if (a_tipoAcesso == TipoAcessoInstancia_direto) {

		else if (a_tipoAcesso != TipoAcessoInstancia_direto) {

			if (a_tipoAcesso >= TipoAcessoInstancia_m1)
				membro = cabecalho.at(0);

			if (a_tipoAcesso >= TipoAcessoInstancia_m2)
				membroMembro = cabecalho.at(1);

			if (a_tipoAcesso >= TipoAcessoInstancia_m3)
				membroM2 = cabecalho.at(2);

			if (a_tipoAcesso >= TipoAcessoInstancia_m4)
				membroM3 = cabecalho.at(3);

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
					if (!strCompara(remEspacosEntreStr(valor), fimDeArquivo))
						std::cout << "Aviso! Arquivo " + caminhoArquivo + " nao finalizado com " << fimDeArquivo << std::endl;
				} // if (fimDeArquivo != "") {
				break;
			} // if (pos != string::npos) {

			valor = linha.substr(0, pos).c_str();

			if (fimDeArquivo != "") {
				if (strCompara(remEspacosEntreStr(valor), fimDeArquivo))
					break;
			} // if (fimDeArquivo != "") {

			std::string idMembro = "";

			if (a_tipoAcesso >= TipoAcessoInstancia_m1) {
				idMembro = remEspacosEntreStr(valor);

				if (!findStringNoVetor(idMembro, vetorIdMembro))
					vetorIdMembro.push_back(idMembro);

				pos = linha.find(separadorCSV);
				linha = linha.substr(pos + 1, linha.length());

			} // if (a_tipoAcesso >= TipoAcessoInstancia_m1) {

			std::string idM2 = "";

			if (a_tipoAcesso >= TipoAcessoInstancia_m2) {
				pos = linha.find(separadorCSV);
				valor = linha.substr(0, pos).c_str();

				idM2 = remEspacosEntreStr(valor);

				if (!findStringNoVetor(idM2, vetorIdM2))
					vetorIdM2.push_back(idM2);

				pos = linha.find(separadorCSV);
				linha = linha.substr(pos + 1, linha.length());

			} // if (a_tipoAcesso >= TipoAcessoInstancia_m2) {

			std::string idM3 = "";

			if (a_tipoAcesso >= TipoAcessoInstancia_m3) {
				pos = linha.find(separadorCSV);
				valor = linha.substr(0, pos).c_str();

				idM3 = remEspacosEntreStr(valor);

				if (!findStringNoVetor(idM3, vetorIdM3))
					vetorIdM3.push_back(idM3);

				pos = linha.find(separadorCSV);
				linha = linha.substr(pos + 1, linha.length());
			} // if (a_tipoAcesso >= TipoAcessoInstancia_m3) {

			std::string idM4 = "";

			if (a_tipoAcesso >= TipoAcessoInstancia_m4) {
				pos = linha.find(separadorCSV);
				valor = linha.substr(0, pos).c_str();

				idM4 = remEspacosEntreStr(valor);

				if (!findStringNoVetor(idM4, vetorIdM4))
					vetorIdM4.push_back(idM4);

				pos = linha.find(separadorCSV);
				linha = linha.substr(pos + 1, linha.length());
			} // if (a_tipoAcesso >= TipoAcessoInstancia_m4) {


			if (membro.size() > 2) {
				if (membro.substr(0, 2) == "id")
					membro = membro.substr(2, membro.size() - 2);
			} // if (membro.size() > 2) {

			if (membroMembro.size() > 2) {
				if (membroMembro.substr(0, 2) == "id")
					membroMembro = membroMembro.substr(2, membroMembro.size() - 2);
			} // if (membroMembro.size() > 2) {

			if (membroM2.size() > 2) {
				if (membroM2.substr(0, 2) == "id")
					membroM2 = membroM2.substr(2, membroM2.size() - 2);
			} // if (membroM2.size() > 2) {

			if (membroM3.size() > 2) {
				if (membroM3.substr(0, 2) == "id")
					membroM3 = membroM3.substr(2, membroM3.size() - 2);
			} // if (membroM3.size() > 2) {


			if (a_tipoAcesso == TipoAcessoInstancia_direto) {

				vetorAttComuns.push_back(valor);

				pos = linha.find(separadorCSV);
				linha = linha.substr(pos + 1, linha.length());

				pos = linha.find(separadorCSV);
				valor = linha.substr(0, pos).c_str();

				vetorValores.push_back(remEspacosEntreStr(valor));

			} // if (a_tipoAcesso == TipoAcessoInstancia_direto) {

			else if (a_tipoAcesso != TipoAcessoInstancia_direto) {

				pos = 0;
				while (pos != std::string::npos) {

					pos = linha.find(separadorCSV);

					valor = linha.substr(0, pos).c_str();

					vetorValores.push_back(remEspacosEntreStr(valor));

					linha = linha.substr(pos + 1, linha.length());

					if (linha == "")
						break;

				} // while (pos != string::npos) {

				if ((vetorAttComuns.size() != vetorValores.size()) && (a_tipoAcesso != TipoAcessoInstancia_direto))
					throw std::invalid_argument("Numero de colunas no cabecalho diferente do numero de valores na linha " + std::to_string(lin) + ".");

				if (a_tipoAcesso == TipoAcessoInstancia_m1)
					a_objetoDados.addDadosAttComumM1(membro, idMembro, vetorAttComuns, vetorValores);
				else if (a_tipoAcesso == TipoAcessoInstancia_m2)
					a_objetoDados.addDadosAttComumM2(membro, idMembro, membroMembro, idM2, vetorAttComuns, vetorValores);
				else if (a_tipoAcesso == TipoAcessoInstancia_m3)
					a_objetoDados.addDadosAttComumM3(membro, idMembro, membroMembro, idM2, membroM2, idM3, vetorAttComuns, vetorValores);
				else if (a_tipoAcesso == TipoAcessoInstancia_m4)
					a_objetoDados.addDadosAttComumM4(membro, idMembro, membroMembro, idM2, membroM2, idM3, membroM3, idM4, vetorAttComuns, vetorValores);

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
		else if (a_tipoAcesso == TipoAcessoInstancia_m1)
			a_objetoDados.validacaoDadosAttComumM1(membro, vetorIdMembro);
		else if (a_tipoAcesso == TipoAcessoInstancia_m2)
			a_objetoDados.validacaoDadosAttComumM2(membro, vetorIdMembro, membroMembro, vetorIdM2);
		else if (a_tipoAcesso == TipoAcessoInstancia_m3)
			a_objetoDados.validacaoDadosAttComumM3(membro, vetorIdMembro, membroMembro, vetorIdM2, membroM2, vetorIdM3);
		else if (a_tipoAcesso == TipoAcessoInstancia_m4)
			a_objetoDados.validacaoDadosAttComumM4(membro, vetorIdMembro, membroMembro, vetorIdM2, membroM2, vetorIdM3, membroM3, vetorIdM4);

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
		else if (a_tipoAcesso == TipoAcessoInstancia_m1)
			numMinColunas = 3;
		else if (a_tipoAcesso == TipoAcessoInstancia_m2)
			numMinColunas = 4;
		else if (a_tipoAcesso == TipoAcessoInstancia_m3)
			numMinColunas = 5;
		else if (a_tipoAcesso == TipoAcessoInstancia_m4)
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

			if (pos == 0)
				break;

			valor = linha.substr(0, pos).c_str();

			if (valor.size() == 0)
				break;

			cabecalho.push_back(remEspacosEntreStr(valor));

			linha = linha.substr(pos + 1, linha.length());

		} // while (pos != string::npos) {

		if (cabecalho.size() < numMinColunas)
			throw std::invalid_argument("O cabecalho do arquivo deve possuir pelo menos " + std::to_string(numMinColunas) + " colunas.");

		std::string m1 = "";
		std::string m2 = "";
		std::string m3 = "";
		std::string m4 = "";
		std::string attVetor = "";

		if (a_tipoAcesso >= TipoAcessoInstancia_m1)
			m1 = cabecalho.at(0);

		if (a_tipoAcesso >= TipoAcessoInstancia_m2) 
			m2 = cabecalho.at(1);

		if (a_tipoAcesso >= TipoAcessoInstancia_m3) 
			m3 = cabecalho.at(2);

		if (a_tipoAcesso >= TipoAcessoInstancia_m4)
			m4 = cabecalho.at(3);

		if (m1.size() > 2) {
			if (m1.substr(0, 2) == "id")
				m1 = m1.substr(2, m1.size() - 2);
		} // if (m1.size() > 2) {

		if (m2.size() > 2) {
			if (m2.substr(0, 2) == "id")
				m2 = m2.substr(2, m2.size() - 2);
		} // if (m2.size() > 2) {

		if (m3.size() > 2) {
			if (m3.substr(0, 2) == "id")
				m3 = m3.substr(2, m3.size() - 2);
		} // if (m3.size() > 2) {

		if (m4.size() > 2) {
			if (m4.substr(0, 2) == "id")
				m4 = m4.substr(2, m4.size() - 2);
		} // if (m4.size() > 2) {

		const std::vector<std::string> vetorIter(cabecalho.begin() + numMinColunas - 1, cabecalho.end());
		std::vector<std::string> vector_data;
		std::vector<std::string> vetorAttVetor;
		std::vector<std::string> vetorIdM1;
		std::vector<std::string> vetorIdM2;
		std::vector<std::string> vetorIdM3;
		std::vector<std::string> vetorIdM4;

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
					if (!strCompara(remEspacosEntreStr(valor), fimDeArquivo))
						std::cout << "Aviso! Arquivo " + caminhoArquivo + " nao finalizado com FIM" << std::endl;
				} // if (fimDeArquivo != "") {
				break;
			} // if (pos != string::npos) {

			valor = linha.substr(0, pos).c_str();

			if (fimDeArquivo != "") {
				if (strCompara(valor, fimDeArquivo))
					break;
			} // if (fimDeArquivo != "") {

			std::string idM1 = "";
			if (a_tipoAcesso >= TipoAcessoInstancia_m1) {
				idM1 = remEspacosEntreStr(valor);
				linha = linha.substr(pos + 1, linha.length());
			}

			std::string idM2 = "";
			if (a_tipoAcesso >= TipoAcessoInstancia_m2) {
				pos = linha.find(separadorCSV);
				valor = linha.substr(0, pos).c_str();
				idM2 = remEspacosEntreStr(valor);
				linha = linha.substr(pos + 1, linha.length());
			} // if (a_tipoAcesso >= TipoAcessoInstancia_m2) {

			std::string idM3 = "";
			if (a_tipoAcesso >= TipoAcessoInstancia_m3) {
				pos = linha.find(separadorCSV);
				valor = linha.substr(0, pos).c_str();
				idM3 = remEspacosEntreStr(valor);
				linha = linha.substr(pos + 1, linha.length());
			} // if (a_tipoAcesso >= TipoAcessoInstancia_m3) {

			std::string idM4 = "";
			if (a_tipoAcesso >= TipoAcessoInstancia_m4) {
				pos = linha.find(separadorCSV);
				valor = linha.substr(0, pos).c_str();
				idM4 = remEspacosEntreStr(valor);
				linha = linha.substr(pos + 1, linha.length());
			} // if (a_tipoAcesso >= TipoAcessoInstancia_m4) {

			if (a_tipoAcesso > TipoAcessoInstancia_direto) {
				pos = linha.find(separadorCSV);
				valor = linha.substr(0, pos).c_str();
			}

			attVetor = remEspacosEntreStr(valor);
			linha = linha.substr(pos + 1, linha.length());

			vector_data.reserve(vetorIter.size());

			pos = 0;
			while (pos != std::string::npos) {

				if (linha.size() == 0)
					break;

				pos = linha.find(separadorCSV);

				if (pos == 0)
					break;

				valor = linha.substr(0, pos).c_str();

				if (valor.size() == 0)
					break;

				vector_data.push_back(remEspacosEntreStr(valor));

				linha = linha.substr(pos + 1, linha.length());

			} // while (pos != string::npos) {

			if (vetorIter.size() < vector_data.size())
				throw std::invalid_argument("Numero de colunas no cabecalho menor que o numero de valores na linha " + std::to_string(lin) + ".");

			if (a_tipoAcesso == TipoAcessoInstancia_direto)
				a_objetoDados.addDadoAttVetor(vetorIter, attVetor, vector_data);
			else if (a_tipoAcesso == TipoAcessoInstancia_m1)
				a_objetoDados.addDadoAttVetorM1(m1, idM1, vetorIter, attVetor, vector_data);
			else if (a_tipoAcesso == TipoAcessoInstancia_m2)
				a_objetoDados.addDadoAttVetorM2(m1, idM1, m2, idM2, vetorIter, attVetor, vector_data);
			else if (a_tipoAcesso == TipoAcessoInstancia_m3)
				a_objetoDados.addDadoAttVetorM3(m1, idM1, m2, idM2, m3, idM3, vetorIter, attVetor, vector_data);
			else if (a_tipoAcesso == TipoAcessoInstancia_m4)
				a_objetoDados.addDadoAttVetorM4(m1, idM1, m2, idM2, m3, idM3, m4, idM4, vetorIter, attVetor, vector_data);

			if (idM1.size() > 0)
				if (!findStringNoVetor(idM1, vetorIdM1))
					vetorIdM1.push_back(idM1);

			if (idM2.size() > 0)
				if (!findStringNoVetor(idM2, vetorIdM2))
					vetorIdM2.push_back(idM2);

			if (idM3.size() > 0)
				if (!findStringNoVetor(idM3, vetorIdM3))
					vetorIdM3.push_back(idM3);

			if (idM4.size() > 0)
				if (!findStringNoVetor(idM4, vetorIdM4))
					vetorIdM4.push_back(idM4);

			if (attVetor.size() > 0)
				if (!findStringNoVetor(attVetor, vetorAttVetor))
					vetorAttVetor.push_back(attVetor);

			lin++;

			vector_data = std::vector<std::string>();

		} // while (lin < numLinhas) {

		// Metodos são chamados para validação pós-leitura.
		if (a_tipoAcesso == TipoAcessoInstancia_direto)
			a_objetoDados.validacaoDadosAttVetor(vetorAttVetor);
		else if (a_tipoAcesso == TipoAcessoInstancia_m1)
			a_objetoDados.validacaoDadosAttVetorM1(m1, vetorIdM1, vetorAttVetor);
		else if (a_tipoAcesso == TipoAcessoInstancia_m2)
			a_objetoDados.validacaoDadosAttVetorM2(m1, vetorIdM1, m2, vetorIdM2, vetorAttVetor);
		else if (a_tipoAcesso == TipoAcessoInstancia_m3)
			a_objetoDados.validacaoDadosAttVetorM3(m1, vetorIdM1, m2, vetorIdM2, m3, vetorIdM3, vetorAttVetor);
		else if (a_tipoAcesso == TipoAcessoInstancia_m4)
			a_objetoDados.validacaoDadosAttVetorM4(m1, vetorIdM1, m2, vetorIdM2, m3, vetorIdM3, m4, vetorIdM4, vetorAttVetor);

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


void EntradaSaidaDados::carregarArquivoCSV_AttMatriz(const std::string a_nomeArquivo, SmartDados& a_objetoDados, const TipoAcessoInstancia a_tipoAcesso)const {

	std::string caminhoArquivo = diretorioEntrada + "//" + a_nomeArquivo;
	if (diretorioEntrada == "")
		caminhoArquivo = a_nomeArquivo;;

	try {

		if (a_tipoAcesso == TipoAcessoInstancia_Nenhum)
			throw std::invalid_argument("Necessario informar o argumento Tipo de Acesso a Instancia.");

		int numMinColunas = 0;

		if (a_tipoAcesso == TipoAcessoInstancia_direto)
			numMinColunas = 3;
		else if (a_tipoAcesso == TipoAcessoInstancia_m1)
			numMinColunas = 4;
		else if (a_tipoAcesso == TipoAcessoInstancia_m2)
			numMinColunas = 5;
		else if (a_tipoAcesso == TipoAcessoInstancia_m3)
			numMinColunas = 6;
		else if (a_tipoAcesso == TipoAcessoInstancia_m4)
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

			if (pos == 0)
				break;

			valor = linha.substr(0, pos).c_str();

			if (valor.size() == 0)
				break;

			cabecalho.push_back(remEspacosEntreStr(valor));

			linha = linha.substr(pos + 1, linha.length());

		} // while (pos != string::npos) {

		if (cabecalho.size() < numMinColunas)
			throw std::invalid_argument("O cabecalho do arquivo deve possuir pelo menos " + std::to_string(numMinColunas) + " colunas.");

		std::string m1 = "";
		std::string m2 = "";
		std::string m3 = "";
		std::string m4 = "";

		if (a_tipoAcesso >= TipoAcessoInstancia_m1)
			m1 = cabecalho.at(0);

		if (a_tipoAcesso >= TipoAcessoInstancia_m2)
			m2 = cabecalho.at(1);

		if (a_tipoAcesso >= TipoAcessoInstancia_m3)
			m3 = cabecalho.at(2);

		if (a_tipoAcesso >= TipoAcessoInstancia_m4)
			m4 = cabecalho.at(3);

		if (m1.size() > 2) {
			if (m1.substr(0, 2) == "id")
				m1 = m1.substr(2, m1.size() - 2);
		} // if (m1.size() > 2) {

		if (m2.size() > 2) {
			if (m2.substr(0, 2) == "id")
				m2 = m2.substr(2, m2.size() - 2);
		} // if (m2.size() > 2) {

		if (m3.size() > 2) {
			if (m3.substr(0, 2) == "id")
				m3 = m3.substr(2, m3.size() - 2);
		} // if (m3.size() > 2) {

		if (m4.size() > 2) {
			if (m4.substr(0, 2) == "id")
				m4 = m4.substr(2, m4.size() - 2);
		} // if (m4.size() > 2) {

		std::vector<std::string> vetorAttMatriz;
		std::vector<std::string> vetorIdM1;
		std::vector<std::string> vetorIdM2;
		std::vector<std::string> vetorIdM3;
		std::vector<std::string> vetorIdM4;

		const int num_alloc_data = 1000;
		std::vector<std::vector<std::string>> vector_data;
		std::vector<std::string> vectorIter1;
		const std::vector<std::string> vectorIter2(cabecalho.begin() + numMinColunas - 1, cabecalho.end());

		std::string last_idM1 = "";
		std::string last_idM2 = "";
		std::string last_idM3 = "";
		std::string last_idM4 = "";
		std::string last_attMatriz = "";

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

			if (fimDeArquivo.size() > 0) {
				if (strCompara(remEspacosEntreStr(valor), fimDeArquivo))
					break;
			} // if (fimDeArquivo != "") {

			std::string idM1 = "";
			if (a_tipoAcesso >= TipoAcessoInstancia_m1) {
				idM1 = remEspacosEntreStr(valor);
				linha = linha.substr(pos + 1, linha.length());
			}

			std::string idM2 = "";
			if (a_tipoAcesso >= TipoAcessoInstancia_m2) {
				pos = linha.find(separadorCSV);
				valor = linha.substr(0, pos).c_str();
				idM2 = remEspacosEntreStr(valor);
				linha = linha.substr(pos + 1, linha.length());
			} // if (a_tipoAcesso >= TipoAcessoInstancia_m2) {

			std::string idM3 = "";
			if (a_tipoAcesso >= TipoAcessoInstancia_m3) {
				pos = linha.find(separadorCSV);
				valor = linha.substr(0, pos).c_str();
				idM3 = remEspacosEntreStr(valor);
				linha = linha.substr(pos + 1, linha.length());
			} // if (a_tipoAcesso >= TipoAcessoInstancia_m3) {

			std::string idM4 = "";
			if (a_tipoAcesso >= TipoAcessoInstancia_m4) {
				pos = linha.find(separadorCSV);
				valor = linha.substr(0, pos).c_str();
				idM4 = remEspacosEntreStr(valor);
				linha = linha.substr(pos + 1, linha.length());
			} // if (a_tipoAcesso >= TipoAcessoInstancia_m4) {

			if (a_tipoAcesso > TipoAcessoInstancia_direto) {
				pos = linha.find(separadorCSV);
				valor = linha.substr(0, pos).c_str();
			}

			const std::string attMatriz = remEspacosEntreStr(valor);
			linha = linha.substr(pos + 1, linha.length());

			if (!strCompara(idM1, last_idM1) || !strCompara(idM2, last_idM2) || !strCompara(idM3, last_idM3) || !strCompara(idM4, last_idM4) || !strCompara(attMatriz, last_attMatriz)) {

				if (last_attMatriz.size() > 0) {
					if (a_tipoAcesso == TipoAcessoInstancia_direto)
						a_objetoDados.addDadoAttMatriz(vectorIter1, vectorIter2, last_attMatriz, vector_data);
					else if (a_tipoAcesso == TipoAcessoInstancia_m1)
						a_objetoDados.addDadoAttMatrizM1(m1, last_idM1, vectorIter1, vectorIter2, last_attMatriz, vector_data);
					else if (a_tipoAcesso == TipoAcessoInstancia_m2)
						a_objetoDados.addDadoAttMatrizM2(m1, last_idM1, m2, last_idM2, vectorIter1, vectorIter2, last_attMatriz, vector_data);
					else if (a_tipoAcesso == TipoAcessoInstancia_m3)
						a_objetoDados.addDadoAttMatrizM3(m1, last_idM1, m2, last_idM2, m3, last_idM3, vectorIter1, vectorIter2, last_attMatriz, vector_data);
					else if (a_tipoAcesso == TipoAcessoInstancia_m4)
						a_objetoDados.addDadoAttMatrizM4(m1, last_idM1, m2, last_idM2, m3, last_idM3, m4, last_idM4, vectorIter1, vectorIter2, last_attMatriz, vector_data);
				} // if (last_attMatriz.size() > 0) {

				if (idM1.size() > 0)
					if (!findStringNoVetor(idM1, vetorIdM1))
						vetorIdM1.push_back(idM1);

				if (idM2.size() > 0)
					if (!findStringNoVetor(idM2, vetorIdM2))
						vetorIdM2.push_back(idM2);

				if (idM3.size() > 0)
					if (!findStringNoVetor(idM3, vetorIdM3))
						vetorIdM3.push_back(idM3);

				if (idM4.size() > 0)
					if (!findStringNoVetor(idM4, vetorIdM4))
						vetorIdM4.push_back(idM4);

				if (attMatriz.size() > 0)
					if (!findStringNoVetor(attMatriz, vetorAttMatriz))
						vetorAttMatriz.push_back(attMatriz);

				last_idM1 = idM1;
				last_idM2 = idM2;
				last_idM3 = idM3;
				last_idM4 = idM4;
				last_attMatriz = attMatriz;

				vector_data = std::vector<std::vector<std::string>>();
				vector_data.reserve(num_alloc_data);

				vectorIter1 = std::vector<std::string>();
				vectorIter1.reserve(num_alloc_data);

			} // if (!strCompara(idM1, last_idM1) || !strCompara(idM2, last_idM2) || !strCompara(idM3, last_idM3) || !strCompara(idM4, last_idM4) || !strCompara(attMatriz, last_attMatriz)) {

			vector_data.push_back(std::vector<std::string>());
			const int idx = int(vector_data.size() - 1);
			vector_data.at(idx).reserve(vectorIter2.size());

			pos = linha.find(separadorCSV);
			valor = linha.substr(0, pos).c_str();

			vectorIter1.push_back(remEspacosEntreStr(valor));

			linha = linha.substr(pos + 1, linha.length());

			while (pos != std::string::npos) {

				if (linha.size() == 0)
					break;

				pos = linha.find(separadorCSV);

				if (pos == 0)
					break;

				valor = linha.substr(0, pos).c_str();

				if (valor.size() == 0)
					break;

				vector_data.at(idx).push_back(remEspacosEntreStr(valor));

				linha = linha.substr(pos + 1, linha.length());

			} // while (pos != std::string::npos) {

			lin++;

		} // while (lin < numLinhas) {

		if (vector_data.size() > 0) {
			if (a_tipoAcesso == TipoAcessoInstancia_direto)
				a_objetoDados.addDadoAttMatriz(vectorIter1, vectorIter2, last_attMatriz, vector_data);
			else if (a_tipoAcesso == TipoAcessoInstancia_m1)
				a_objetoDados.addDadoAttMatrizM1(m1, last_idM1, vectorIter1, vectorIter2, last_attMatriz, vector_data);
			else if (a_tipoAcesso == TipoAcessoInstancia_m2)
				a_objetoDados.addDadoAttMatrizM2(m1, last_idM1, m2, last_idM2, vectorIter1, vectorIter2, last_attMatriz, vector_data);
			else if (a_tipoAcesso == TipoAcessoInstancia_m3)
				a_objetoDados.addDadoAttMatrizM3(m1, last_idM1, m2, last_idM2, m3, last_idM3, vectorIter1, vectorIter2, last_attMatriz, vector_data);
			else if (a_tipoAcesso == TipoAcessoInstancia_m4)
				a_objetoDados.addDadoAttMatrizM4(m1, last_idM1, m2, last_idM2, m3, last_idM3, m4, last_idM4, vectorIter1, vectorIter2, last_attMatriz, vector_data);
		} // if (vector_data.size() > 0) {

		// Metodos são chamados para validação pós-leitura.
		if (a_tipoAcesso == TipoAcessoInstancia_direto)
			a_objetoDados.validacaoDadosAttMatriz(vetorAttMatriz);
		else if (a_tipoAcesso == TipoAcessoInstancia_m1)
			a_objetoDados.validacaoDadosAttMatrizM1(m1, vetorIdM1, vetorAttMatriz);
		else if (a_tipoAcesso == TipoAcessoInstancia_m2)
			a_objetoDados.validacaoDadosAttMatrizM2(m1, vetorIdM1, m2, vetorIdM2, vetorAttMatriz);
		else if (a_tipoAcesso == TipoAcessoInstancia_m3)
			a_objetoDados.validacaoDadosAttMatrizM3(m1, vetorIdM1, m2, vetorIdM2, m3, vetorIdM3, vetorAttMatriz);
		else if (a_tipoAcesso == TipoAcessoInstancia_m4)
			a_objetoDados.validacaoDadosAttMatrizM4(m1, vetorIdM1, m2, vetorIdM2, m3, vetorIdM3, m4, vetorIdM4, vetorAttMatriz);

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


void EntradaSaidaDados::imprimirArquivoCSV_AttComum_(const std::string a_nomeArquivo, const std::string a_m1, const std::string a_m2, const std::string a_m3, const std::string a_m4, SmartDados & a_objetoDados, const std::vector<std::string> a_vetorAttComum)const {

	std::string caminhoArquivo = diretorioSaida + "//" + a_nomeArquivo;
	if (diretorioSaida == "")
		caminhoArquivo = a_nomeArquivo;

	try {

		criarDiretorio(diretorioSaida);

		std::string membro = "";
		std::string idMembro = "";

		getMembroIdMembro(a_m1, membro, idMembro);

		if ((a_m1 != "") && ((membro == "") || (idMembro == "")))
			throw std::invalid_argument("Membro ou IdMembro invalidos.");

		std::string membroMembro = "";
		std::string idM2 = "";

		getMembroIdMembro(a_m2, membroMembro, idM2);

		if ((a_m2 != "") && ((membroMembro == "") || (idM2 == "")))
			throw std::invalid_argument("M2 ou IdM2 invalidos.");

		std::string membroM2 = "";
		std::string idM3 = "";

		getMembroIdMembro(a_m3, membroM2, idM3);

		if ((a_m3 != "") && ((membroM2 == "") || (idM3 == "")))
			throw std::invalid_argument("M3 ou IdM3 invalidos.");

		std::string membroM3 = "";
		std::string idM4 = "";

		getMembroIdMembro(a_m4, membroM3, idM4);

		if ((a_m4 != "") && ((membroM3 == "") || (idM4 == "")))
			throw std::invalid_argument("M4 ou IdM4 invalidos.");

		TipoAcessoInstancia tipoAcessoInstancia = TipoAcessoInstancia_Nenhum;

		if      ((membro == "") && (membroMembro == "") && (membroM2 == "") && (membroM3 == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_direto;
		else if ((membro != "") && (membroMembro == "") && (membroM2 == "") && (membroM3 == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_m1;
		else if ((membro != "") && (membroMembro != "") && (membroM2 == "") && (membroM3 == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_m2;
		else if ((membro != "") && (membroMembro != "") && (membroM2 != "") && (membroM3 == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_m3;
		else if ((membro != "") && (membroMembro != "") && (membroM2 != "") && (membroM3 != ""))
			tipoAcessoInstancia = TipoAcessoInstancia_m4;
		else
			throw std::invalid_argument("Acesso a instancia invalido.");

		bool incluirCabecalho = true;
		if (appendArquivo)
			incluirCabecalho = false;

		std::vector<std::vector<std::string>> matrizDados;
		if (tipoAcessoInstancia == TipoAcessoInstancia_direto)
			matrizDados = a_objetoDados.getDadosAttComum(incluirCabecalho, a_vetorAttComum);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_m1)
			matrizDados = a_objetoDados.getDadosAttComumM1(incluirCabecalho, membro, idMembro, a_vetorAttComum);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_m2)
			matrizDados = a_objetoDados.getDadosAttComumM2(incluirCabecalho, membro, idMembro, membroMembro, idM2, a_vetorAttComum);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_m3)
			matrizDados = a_objetoDados.getDadosAttComumM3(incluirCabecalho, membro, idMembro, membroMembro, idM2, membroM2, idM3, a_vetorAttComum);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_m4)
			matrizDados = a_objetoDados.getDadosAttComumM4(incluirCabecalho, membro, idMembro, membroMembro, idM2, membroM2, idM3, membroM3, idM4, a_vetorAttComum);

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

	catch (const std::ifstream::failure& erro) { throw std::invalid_argument("EntradaSaidaDados::imprimirArquivoCSV_AttComum_(" + caminhoArquivo + "," + a_m1 + "," + a_m2 + "," + a_m3 + "," + a_m4 + ",a_objetoDados,a_vetorAttComum): \nErro de Integridade do Arquivo." + std::string(erro.what())); }
	catch (const std::exception& erro) { throw std::invalid_argument("EntradaSaidaDados::imprimirArquivoCSV_AttComum_(" + caminhoArquivo + ", " + a_m1 + ", " + a_m2 + ", " + a_m3 + "," + a_m4 + ", a_objetoDados,a_vetorAttComum) : \n" + std::string(erro.what())); }

} // void EntradaSaidaDados::imprimirArquivoCSV_AttComum_(const std::string a_nomeArquivo, std::string a_m1, std::string a_m2, std::string a_m3, SmartDados & a_objetoDados, const std::string a_attComum){


void EntradaSaidaDados::imprimirArquivoCSV_AttVetor_(const std::string a_nomeArquivo, const std::string a_m1, const std::string a_m2, const std::string a_m3, const std::string a_m4, SmartDados & a_objetoDados, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const std::vector<std::string> a_vetorAttVetor)const {

	std::string caminhoArquivo = diretorioSaida + "//" + a_nomeArquivo;
	if (diretorioSaida == "")
		caminhoArquivo = a_nomeArquivo;

	try {

		criarDiretorio(diretorioSaida);

		std::string membro   = "";
		std::string idMembro = "";
			
		getMembroIdMembro(a_m1, membro, idMembro);

		if ((a_m1 != "") && ((membro == "") || (idMembro == "")))
			throw std::invalid_argument("Membro ou IdMembro invalidos.");

		std::string membroMembro   = "";
		std::string idM2 = "";

		getMembroIdMembro(a_m2, membroMembro, idM2);

		if ((a_m2 != "") && ((membroMembro == "") || (idM2 == "")))
			throw std::invalid_argument("M2 ou IdM2 invalidos.");

		std::string membroM2   = "";
		std::string idM3 = "";

		getMembroIdMembro(a_m3, membroM2, idM3);

		if ((a_m3 != "") && ((membroM2 == "") || (idM3 == "")))
			throw std::invalid_argument("M3 ou IdM3 invalidos.");

		std::string membroM3 = "";
		std::string idM4 = "";

		getMembroIdMembro(a_m4, membroM3, idM4);

		if ((a_m4 != "") && ((membroM3 == "") || (idM4 == "")))
			throw std::invalid_argument("M4 ou IdM4 invalidos.");

		TipoAcessoInstancia tipoAcessoInstancia = TipoAcessoInstancia_Nenhum;

		if ((membro == "") && (membroMembro == "") && (membroM2 == "") && (membroM3 == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_direto;
		else if ((membro != "") && (membroMembro == "") && (membroM2 == "") && (membroM3 == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_m1;
		else if ((membro != "") && (membroMembro != "") && (membroM2 == "") && (membroM3 == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_m2;
		else if ((membro != "") && (membroMembro != "") && (membroM2 != "") && (membroM3 == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_m3;
		else if ((membro != "") && (membroMembro != "") && (membroM2 != "") && (membroM3 != ""))
			tipoAcessoInstancia = TipoAcessoInstancia_m4;
		else
			throw std::invalid_argument("Acesso a instancia invalido.");

		bool incluirCabecalho = true;
		if (appendArquivo)
			incluirCabecalho = false;

		std::vector<std::vector<std::string>> matrizDados;
		if (tipoAcessoInstancia == TipoAcessoInstancia_direto)
			matrizDados = a_objetoDados.getDadosAttVetor(incluirCabecalho, a_iteradorInicial, a_iteradorFinal, true, a_vetorAttVetor);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_m1)
			matrizDados = a_objetoDados.getDadosAttVetorM1(incluirCabecalho, membro, idMembro, a_iteradorInicial, a_iteradorFinal, true, a_vetorAttVetor);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_m2)
			matrizDados = a_objetoDados.getDadosAttVetorM2(incluirCabecalho, membro, idMembro, membroMembro, idM2, a_iteradorInicial, a_iteradorFinal, true, a_vetorAttVetor);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_m3)
			matrizDados = a_objetoDados.getDadosAttVetorM3(incluirCabecalho, membro, idMembro, membroMembro, idM2, membroM2, idM3, a_iteradorInicial, a_iteradorFinal, true, a_vetorAttVetor);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_m4)
			matrizDados = a_objetoDados.getDadosAttVetorM4(incluirCabecalho, membro, idMembro, membroMembro, idM2, membroM2, idM3, membroM3, idM4, a_iteradorInicial, a_iteradorFinal, true, a_vetorAttVetor);

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

	catch (const std::ifstream::failure& erro) { throw std::invalid_argument("EntradaSaidaDados::imprimirArquivoCSV_AttVetor_(" + caminhoArquivo + "," + a_m1 + "," + a_m2 + "," + a_m3 + "," + a_m4 + ",a_objetoDados," + a_iteradorInicial + "," + a_iteradorFinal + ",a_vetorAttVetor): \nErro de Integridade do Arquivo." + std::string(erro.what())); }
	catch (const std::exception& erro) { throw std::invalid_argument("EntradaSaidaDados::imprimirArquivoCSV_AttVetor_(" + caminhoArquivo + "," + a_m1 + "," + a_m2 + "," + a_m3 + "," + a_m4 + ",a_objetoDados," + a_iteradorInicial + "," + a_iteradorFinal + ",a_vetorAttVetor): \n" + std::string(erro.what())); }

} // void EntradaSaidaDados::imprimirArquivoCSV_AttVetor_(const std::string a_nomeArquivo, const std::string a_m1, const std::string a_m2, const std::string a_m3, SmartDados & a_objetoDados, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const std::string a_attVetor){


void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz_(const std::string a_nomeArquivo, const std::string a_m1, const std::string a_m2, const std::string a_m3, const std::string a_m4, SmartDados & a_objetoDados, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const std::vector<std::string> a_vetorAttMatriz)const {

	std::string caminhoArquivo = diretorioSaida + "//" + a_nomeArquivo;
	if (diretorioSaida == "")
		caminhoArquivo = a_nomeArquivo;

	try {

		criarDiretorio(diretorioSaida);

		std::string membro = "";
		std::string idMembro = "";

		getMembroIdMembro(a_m1, membro, idMembro);

		if ((a_m1 != "") && ((membro == "") || (idMembro == "")))
			throw std::invalid_argument("Membro ou IdMembro invalidos.");

		std::string membroMembro = "";
		std::string idM2 = "";

		getMembroIdMembro(a_m2, membroMembro, idM2);

		if ((a_m2 != "") && ((membroMembro == "") || (idM2 == "")))
			throw std::invalid_argument("M2 ou IdM2 invalidos.");

		std::string membroM2 = "";
		std::string idM3 = "";

		getMembroIdMembro(a_m3, membroM2, idM3);

		if ((a_m3 != "") && ((membroM2 == "") || (idM3 == "")))
			throw std::invalid_argument("M3 ou IdM3 invalidos.");

		std::string membroM3 = "";
		std::string idM4 = "";

		getMembroIdMembro(a_m4, membroM3, idM4);

		if ((a_m4 != "") && ((membroM3 == "") || (idM4 == "")))
			throw std::invalid_argument("M4 ou IdM4 invalidos.");

		TipoAcessoInstancia tipoAcessoInstancia = TipoAcessoInstancia_Nenhum;

		if ((membro == "") && (membroMembro == "") && (membroM2 == "") && (membroM3 == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_direto;
		else if ((membro != "") && (membroMembro == "") && (membroM2 == "") && (membroM3 == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_m1;
		else if ((membro != "") && (membroMembro != "") && (membroM2 == "") && (membroM3 == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_m2;
		else if ((membro != "") && (membroMembro != "") && (membroM2 != "") && (membroM3 == ""))
			tipoAcessoInstancia = TipoAcessoInstancia_m3;
		else if ((membro != "") && (membroMembro != "") && (membroM2 != "") && (membroM3 != ""))
			tipoAcessoInstancia = TipoAcessoInstancia_m4;
		else
			throw std::invalid_argument("Acesso a instancia invalido.");

		bool incluirCabecalho = true;
		if (appendArquivo)
			incluirCabecalho = false;

		std::vector<std::vector<std::string>> matrizDados;
		if (tipoAcessoInstancia == TipoAcessoInstancia_direto)
			matrizDados = a_objetoDados.getDadosAttMatriz(incluirCabecalho, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, true, a_vetorAttMatriz);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_m1)
			matrizDados = a_objetoDados.getDadosAttMatrizM1(incluirCabecalho, membro, idMembro, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, true, a_vetorAttMatriz);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_m2)
			matrizDados = a_objetoDados.getDadosAttMatrizM2(incluirCabecalho, membro, idMembro, membroMembro, idM2, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, true, a_vetorAttMatriz);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_m3)
			matrizDados = a_objetoDados.getDadosAttMatrizM3(incluirCabecalho, membro, idMembro, membroMembro, idM2, membroM2, idM3, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, true, a_vetorAttMatriz);
		else if (tipoAcessoInstancia == TipoAcessoInstancia_m4)
			matrizDados = a_objetoDados.getDadosAttMatrizM4(incluirCabecalho, membro, idMembro, membroMembro, idM2, membroM2, idM3, membroM3, idM4, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, true, a_vetorAttMatriz);

		const int numLin = int(matrizDados.size());
		if (numLin == 0)
			return;

		int numCol = 0;
		for (int l = 0; l < int(matrizDados.size()); l++) {
			int numCol_l = 0;
			for (numCol_l = 1; numCol_l <= int(matrizDados.at(l).size()); numCol_l++) {
				if (matrizDados.at(l).at(numCol_l - 1) == "") {
					numCol_l--;
					break;
				}
			}
			if (numCol_l > numCol)
				numCol = numCol_l;
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

	catch (const std::ifstream::failure& erro) { throw std::invalid_argument("EntradaSaidaDados::imprimirArquivoCSV_AttMatriz_(" + caminhoArquivo + "," + a_m1 + "," + a_m2 + "," + a_m3 + "," + a_m4 + ",a_objetoDados," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + ",a_vetorAttMatriz): \nErro de Integridade do Arquivo." + std::string(erro.what())); }
	catch (const std::exception& erro) { throw std::invalid_argument("EntradaSaidaDados::imprimirArquivoCSV_AttMatriz_(" + caminhoArquivo + "," + a_m1 + "," + a_m2 + "," + a_m3 + "," + a_m4 + ",a_objetoDados," + a_iterador1Inicial + "," + a_iterador1Final + "," + a_iterador2Inicial + "," + a_iterador2Final + ",a_vetorAttMatriz): \n" + std::string(erro.what())); }

} // void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz_(const std::string a_nomeArquivo, const std::string a_m1, const std::string a_m2, const std::string a_m3, SmartDados & a_objetoDados, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const std::string a_attMatriz){


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


void EntradaSaidaDados::imprimirConsolidacaoVerticalCSV(const std::string a_nome_arquivo_resultante, const std::vector<std::string>& a_lista_arquivos_originais, const bool a_apagar_arquivos_originais, const bool a_remover_cabecalho) const {


	std::string caminhoArquivo = diretorioSaida + "//" + a_nome_arquivo_resultante;
	if (diretorioSaida == "")
		caminhoArquivo = a_nome_arquivo_resultante;

	try {

		if (a_lista_arquivos_originais.size() == 0)
			throw std::invalid_argument("A lista de arquivos a serem consolidados esta vazia.");


		std::ofstream escritaArquivo;

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


		escritaArquivo.open(caminhoArquivo.c_str(), std::ios_base::out);

		if (!escritaArquivo.is_open())
			return;

		std::string linha_inner;
		for (int i = 0; i < int(lista_escritaArquivo.size()); i++) {

			bool no_write_line = a_remover_cabecalho;
			while (!lista_escritaArquivo.at(i).eof()) {

				std::getline(lista_escritaArquivo.at(i), linha_inner);

				if (no_write_line && (i > 0)) {
					no_write_line = false;
				}
				else {
					strNormalizada(linha_inner);
					if (linha_inner.size() > 0)
						escritaArquivo << linha_inner << std::endl;
				}
			}
		} // for (int i = 0; i < int(lista_escritaArquivo.size()); i++) {

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

	catch (const std::ifstream::failure& erro) { throw std::invalid_argument("EntradaSaidaDados::imprimirConsolidacaoVerticalCSV(" + caminhoArquivo + ",a_matriz_string," + getString(a_apagar_arquivos_originais) + "): \nErro de Integridade do Arquivo." + std::string(erro.what())); }
	catch (const std::exception& erro) { throw std::invalid_argument("EntradaSaidaDados::imprimirConsolidacaoVerticalCSV(" + caminhoArquivo + ",a_matriz_string," + getString(a_apagar_arquivos_originais) + "): \n" + std::string(erro.what())); }

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

void EntradaSaidaDados::imprimirArquivoCSV(const std::string a_nomeArquivo, const std::vector<std::string>& a_matriz_string, const bool a_incluir_cabecalho = true) const {

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

		for (int lin = 0; lin < numLin; lin++) {
			if ((incluirCabecalho) || (lin > 0))
				escritaArquivo << a_matriz_string.at(lin) << std::endl;
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


void EntradaSaidaDados::getMembroIdMembro(const std::string a_string, std::string & a_m1, std::string & a_idM1)const {

	if (a_string.empty()) {
		a_m1   = "";
		a_idM1 = "";
		return;
	} // if (a_string.empty()) {

	size_t posId, pos_;

	posId = a_string.find("Id");
	pos_  = a_string.find("_");

	if ((posId == std::string::npos) || (pos_ == std::string::npos)) {
		a_m1   = "";
		a_idM1 = "";
	}

	else {
		a_m1   = a_string.substr(posId + 2, pos_ - 2);
		a_idM1 = a_string.substr(pos_ + 1, a_string.length());
	}

} // void EntradaSaidaDados::getMembroIdMembro(const std::string a_string, std::string & a_m1, std::string & a_idM1){


