#include "C_LeituraCEPEL.h"
#include "C_EntradaSaidaDados.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <functional>

bool considera_perdas_transmissao = false;
bool considera_desvio_de_agua = false;
bool considera_energia_desvio_funcao_da_energia_armazenada = false;
bool considera_agrupamento_livre_de_intercambios = false;
bool considera_submotorizacao = false;
bool considera_cargas_adicionais = false;
bool considera_despacho_antecipado_gnl = false;
bool considera_geracao_hidraulica_minima = false;
bool considera_vazao_minima = true;//Está com lógica inversa no DECK
bool considera_restricoes_eletricas = false;//Está com lógica inversa no DECK
bool duracao_patamar_carga_variavel_por_ano = true;
bool considera_CVAR_variavel_no_tempo = false;
int tamanho_registro_arquivo_vazoes_historicas = 320;


void LeituraCEPEL::leitura_NEWAVE(Dados& a_dados, const std::string a_diretorio, const std::string nomeArquivo) {

	try {
		bool readPoliJusHidr_dat = true;
		std::ifstream poliJus(a_diretorio + "/polinjus.dat");
		if (poliJus.is_open()) { readPoliJusHidr_dat = false; }

		std::ifstream leituraArquivo(a_diretorio + "//" + nomeArquivo);
		std::string line;

		if (leituraArquivo.is_open()) {
			
			instancia_horizonte_preConfig(a_dados, a_dados.getAtributo(AttComumDados_diretorio_entrada_dados, std::string()) + "_PRECONFIG");

			instancia_dados_preConfig(a_dados, a_dados.getAtributo(AttComumDados_diretorio_entrada_dados, std::string()) + "_PRECONFIG");

			instancia_termeletricas_preConfig(a_dados, a_dados.getAtributo(AttComumDados_diretorio_entrada_dados, std::string()) + "_PRECONFIG");

			leitura_DGER_201908_NW25(a_dados, a_diretorio + "//DGER.DAT");

			instancia_hidreletricas_preConfig(a_dados, a_dados.getAtributo(AttComumDados_diretorio_entrada_dados, std::string()) + "_PRECONFIG");

			leitura_SISTEMA_201908_NW25(a_dados, a_diretorio + "//SISTEMA.DAT");

			leitura_PATAMAR_201908_NW25(a_dados, a_diretorio + "//PATAMAR.DAT");

			leitura_CONFHD_201908_NW25(a_dados, a_diretorio + "//CONFHD.DAT");

			inicializa_Submercados_Intercambios_Nao_Registrados(a_dados);

			leitura_CADUSIH_201904_NW25_DC29_DES16(a_dados, a_diretorio + "//HIDR.DAT", false, readPoliJusHidr_dat);

			leitura_MODIF_201908_NW25(a_dados, a_diretorio + "//MODIF.DAT");

			leitura_EXPH_201908_NW25(a_dados, a_diretorio + "//EXPH.DAT");

			leitura_CONFT_201908_NW25(a_dados, a_diretorio + "//CONFT.DAT");

			leitura_TERM_201908_NW25(a_dados, a_diretorio + "//TERM.DAT");

			leitura_EXPT_201908_NW25(a_dados, a_diretorio + "//EXPT.DAT");
			aplicarModificacoesUTE(a_dados);

			leitura_CLAST_201908_NW25(a_dados, a_diretorio + "//CLAST.DAT");

			leitura_MANUTT_201908_NW25(a_dados, a_diretorio + "//MANUTT.DAT");

			aplicarModificacoesUHE(a_dados);

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);

				const int line_size = int(line.length());

				std::string arquivo = line.substr(30, line_size);
				arquivo.erase(std::remove(arquivo.begin(), arquivo.end(), ' '), arquivo.end());

				if (arquivo == "DGER.DAT") {}

				if (arquivo == "SISTEMA.DAT") {}

				if (arquivo == "CONFHD.DAT") {}

				if (arquivo == "HIDR.DAT") {}

				if (arquivo == "MODIF.DAT") {}

				if (arquivo == "CONFT.DAT") {}

				if (arquivo == "TERM.DAT") {}

				if (arquivo == "CLAST.DAT") {}

				if (arquivo == "EXPH.DAT") {}

				if (arquivo == "EXPT.DAT") {}

				if (arquivo == "PATAMAR.DAT") {}

				if (arquivo == "MANUTT.DAT") {}

				if (arquivo == "VAZPAST.DAT") {}

				if (arquivo == "C_ADIC.DAT") { leitura_C_ADIC_201908_NW25(a_dados, a_diretorio + "//" + arquivo); }

				if (arquivo == "GTMINPAT.DAT") { leitura_GTMINPAT_201908_NW25(a_dados, a_diretorio + "//" + arquivo); }

				if (arquivo == "LOSS.DAT") { leitura_LOSS_201908_NW25(a_dados, a_diretorio + "//" + arquivo); }

				if (arquivo == "DSVAGUA.DAT") { leitura_DSVAGUA_201908_NW25(a_dados, a_diretorio + "//" + arquivo); }

				if (arquivo == "PENALID.DAT") { leitura_PENALID_201908_NW25(a_dados, a_diretorio + "//" + arquivo); }

				if (arquivo == "AGRINT.DAT") { leitura_AGRINT_201908_NW25(a_dados, a_diretorio + "//" + arquivo); }

				if (arquivo == "ADTERM.DAT") {
					EntradaSaidaDados entradaSaidaDados;
					entradaSaidaDados.setDiretorioEntrada(a_dados.getAtributo(AttComumDados_diretorio_entrada_dados, std::string()) + "_PRECONFIG");
					a_dados.carregarArquivosEntrada_TERMELETRICA_COMANDO(entradaSaidaDados, true, false);
					leitura_ADTERM_201908_NW25(a_dados, a_diretorio + "//" + arquivo);
				}

				if (arquivo == "GHMIN.DAT") { leitura_GHMIN_201908_NW25(a_dados, a_diretorio + "//" + arquivo); }

				if (arquivo == "CVAR.DAT") { leitura_CVAR_201908_NW25(a_dados, a_diretorio + "//" + arquivo); }

				if (arquivo == "RE.DAT") { leitura_RE_201908_NW25(a_dados, a_diretorio + "//" + arquivo); }
				
				if (arquivo == "CURVA.DAT") { leitura_CURVA_202001_NW27(a_dados, a_diretorio + "//" + arquivo); }

			}//while (std::getline(leituraArquivo, line)) {
			leituraArquivo.close();

			leitura_VAZPAST_201908_NW25(a_dados, a_diretorio + "//VAZPAST.DAT");

			leitura_VAZOES_201908_NW25(a_dados, a_diretorio + "//VAZOES.DAT");

			//Instancia intercâmbio hidráulicos das jusantes_desvio
			//adicionaIntercambiosHidraulicosApartirJusanteDesvio(a_dados);

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

		sequenciarRestricoesEletricas(a_dados);
		sequenciarRestricoesHidraulicas(a_dados);

		validacoes_NW(a_dados);

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_NEWAVE: \n" + std::string(erro.what())); }

} // void LeituraCEPEL::leitura_NEWAVE(Dados &a_dados, std::string nomeArquivo) {



void LeituraCEPEL::leitura_CURVA_202001_NW27(Dados& a_dados, std::string a_nomeArquivo)
{

	try {

		std::ifstream myfile(a_nomeArquivo);
		std::string line;
		
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		int bloco = 0;
		bool lido_bloco1 = false;
		bool lido_bloco2 = false;
		bool lido_bloco3 = false;
		IdMes  idMesPenalizacao;
		if (myfile.is_open()) {

			SmartEnupla<IdHidreletrica, SmartEnupla<Periodo, double>> lista_VOLMINOP(IdHidreletrica_1, std::vector<SmartEnupla<Periodo, double>>(a_dados.getMaiorId(IdHidreletrica()), SmartEnupla<Periodo, double>()));

			while (std::getline(myfile, line)) {
				std::string mnemonico = line.substr(0, 4);
				mnemonico.erase(std::remove(mnemonico.begin(), mnemonico.end(), ' '), mnemonico.end());

				

				if ((bloco == 1) && (!lido_bloco1)) {
					idMesPenalizacao = IdMes(std::stoi(line.substr(5, 3)));
					lido_bloco1 = true;
					a_dados.setAtributo(AttComumDados_mes_penalizacao_volume_util_minimo, idMesPenalizacao);
				}
				
				if ((bloco == 2) && (!lido_bloco2)) {

					while (std::getline(myfile, line)) {

						if (std::stoi(line.substr(1, 3)) == 999) { lido_bloco2 = true; break;  }

						//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica++)
							//	a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_penalidade_volume_util_minimo, std::stod(line.substr(11, 7)));

					}//while (std::getline(myfile, line)) {	

				}//if (bloco == 2) {
				

				if (bloco == 3) {

					int ree = 0;				

					while (std::getline(myfile, line)) {

						std::string mnemonico = line.substr(0, 4);
						mnemonico.erase(std::remove(mnemonico.begin(), mnemonico.end(), ' '), mnemonico.end());

						if (!mnemonico.empty()) {
							if (std::stoi(line.substr(0, 4)) == 9999) { lido_bloco3 = true;  break; }

							std::string line_erase = line.substr(0, 4);
							line_erase.erase(std::remove(line_erase.begin(), line_erase.end(), ' '), line_erase.end());

							if (line_erase.size() < 3) { ree = std::stoi(line.substr(1, 3)); }

							else if (ree != 0) {

								for (int mes = 1; mes <= 12; mes++) {

									const Periodo periodo_dado = Periodo(TipoPeriodo_mensal, IdMes(mes), getIdAnoFromChar(line.substr(0, 4).c_str()));
									double percentual_energia = std::stod(line.substr(mes * 6, 5)) * 0.01;

									for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica++) {

										if (lista_codigo_ONS_REE.getElemento(idHidreletrica) == ree) {

											if (a_dados.getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_percentual_volume_util_minimo) == 0)
												a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_percentual_volume_util_minimo, SmartEnupla<Periodo, double>(horizonte_estudo, double(0.0)));

											for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

												if (periodo.sobreposicao(periodo_dado) > 0 && (a_dados.getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_percentual_volume_util_minimo, periodo, double()) < percentual_energia))
													a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_percentual_volume_util_minimo, periodo, percentual_energia);

											}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

										}//if (lista_codigo_ONS_REE.getElemento(idHidreletrica) == ree) {

									}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica++) {								

								} // for (int mes = 1; mes <= 12; mes++) {

							}//else {
													   
						}//if (!mnemonico.empty()) {

					}//while (std::getline(myfile, line)) {

				}//if (bloco == 3) {
				if (mnemonico == "XXX") { bloco += 1; }

				if (lido_bloco3) { break; }		
				

			}//while (std::getline(myfile, line)) {

			myfile.clear();
			myfile.close();

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + a_nomeArquivo + ".");

	}//	try {
	catch (const std::exception & erro) { throw std::invalid_argument("LeituraCEPEL::leitura_CURVA_202001_NW27: \n" + std::string(erro.what())); }


}




void LeituraCEPEL::leitura_DGER_201908_NW25(Dados &a_dados, std::string nomeArquivo) {

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;

		////////////////////////////////////////////////////////

		if (leituraArquivo.is_open()) {

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 1 -  Nome do caso 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			atributo = line.substr(0, 80);

			a_dados.setAtributo(AttComumDados_nome_estudo, atributo);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 2 -  Tipo de execução
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			atributo = line.substr(21, 4);
			atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			if (!dadosPreConfig_instanciados) {

				if (atributo == "1")      //Manual: 1 = rodada completa 
					a_dados.setAtributo(AttComumDados_tipo_estudo, TipoEstudo_otimizacao_e_simulacao);
				else if (atributo == "0") //Manual: 0 = só executa simulação final 
					a_dados.setAtributo(AttComumDados_tipo_estudo, TipoEstudo_simulacao);
				else
					throw std::invalid_argument("Tipo de execucao nao definido no arquivo DGER.DAT ");

			}

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 3 -  Duração de cada estágio de operação, em meses (função desabilitada) 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 4 -  Número de anos de planejamento
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			const int anos_planejamento = std::stoi(line.substr(21, 4));

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 5 -  Mês inicial do período que antecede o período de planejamento.
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			atributo = line.substr(21, 4);
			atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 6 -  Mês inicial do período de planejamento. Se o período que antecede o período de planejamento 
			//for diferente de zero (registro 8), o Newave irá considerar esse valor unitário.
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			const int mes_inicial = std::stoi(line.substr(21, 4));

			/////////////////////
			//Set idEstagio_final_otimizacao
			/////////////////////

			const int meses_planejamento = 12 * (anos_planejamento - 1) + (12 - mes_inicial + 1); //O número de estágios é igual ao número de meses do primeiro ano mais o número de meses dos anos restantes

			const IdEstagio idEstagio_final_otimizacao = getIdEstagioFromChar(std::to_string(meses_planejamento).c_str());

			if ((!dadosPreConfig_instanciados) || (a_dados.getAtributo(AttComumDados_estagio_final, IdEstagio()) > idEstagio_final_otimizacao)) {

				a_dados.setAtributo(AttComumDados_estagio_final, idEstagio_final_otimizacao);

			} // if ((!dadosPreConfig_instanciados) || (a_dados.getAtributo(AttComumDados_estagio_final, IdEstagio()) > idEstagio_final_otimizacao)) {

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 7 -  Ano inicial do período de planejamento. 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			const int ano_inicial = std::stoi(line.substr(21, 4));

			/////////////////////
			//Set Periodo Inicial
			/////////////////////
			Periodo periodo_estudo_inicial(mes_inicial, ano_inicial);

			for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_estudo_inicial + meses_planejamento - 1; periodo++)
				horizonte_estudo_DECK.addElemento(periodo, true);

			if (!dadosPreConfig_instanciados)
				a_dados.setAtributo(AttComumDados_periodo_referencia, periodo_estudo_inicial);
			else
				periodo_estudo_inicial = a_dados.getAtributo(AttComumDados_periodo_referencia, Periodo());

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 8 -  Número de anos iniciais para fins de estabilização no cálculo da política 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			atributo = line.substr(21, 4);
			atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 9 -  Número de anos finais para fins de estabilização no cálculo da política
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			atributo = line.substr(21, 4);
			atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 10 -  Número de anos finais para fins de estabilização na simulação final
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			atributo = line.substr(21, 4);
			atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 11 -  Controle de impressão das características das usinas 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 12 -  Controle de impressão dos dados de mercado de energia
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 13 -  Controle de impressão as energias históricas afluentes
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 14 -  Controle de impressão dos parâmetros do modelo estocástico
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 15 -  Controle de impressão dos parâmetros dos reservatórios equivalentes de energia
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 16 -  Número máximo de iterações 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			if (!dadosPreConfig_instanciados) {
				const int numero_maximo_iteracoes = std::stoi(line.substr(21, 4));

				a_dados.setAtributo(AttComumDados_numero_maximo_iteracoes, numero_maximo_iteracoes);

				a_dados.setAtributo(AttComumDados_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico_hidrologico_bacia);

				a_dados.setAtributo(AttComumDados_tipo_modelo_geracao_cenario_hidrologico, TipoModeloGeracaoSinteticaCenario_lognormal_3p_sazonal);

				a_dados.setAtributo(AttComumDados_tipo_relaxacao_afluencia_incremental, TipoRelaxacaoAfluenciaIncremental_truncamento);

				a_dados.setAtributo(AttComumDados_imprimir_espaco_amostral_geracao_cenario_hidrologico, true);
				a_dados.setAtributo(AttComumDados_imprimir_geracao_cenario_hidrologico, true);

			} // if (!dadosPreConfig_instanciados) {

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 17 -  Número de simulações forward
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			if (!dadosPreConfig_instanciados) {

				const int numero_cenarios_por_iteracao_otimizacao = std::stoi(line.substr(21, 4));

				a_dados.setAtributo(AttComumDados_visitar_todos_cenarios_por_iteracao, false);

				a_dados.setAtributo(AttComumDados_numero_cenarios, numero_cenarios_por_iteracao_otimizacao * a_dados.getAtributo(AttComumDados_numero_maximo_iteracoes, int()));

				a_dados.setAtributo(AttComumDados_imprimir_solver, false);

				a_dados.setAtributo(AttComumDados_imprimir_resultado_por_estagio_por_cenario, false);

			} // if (!dadosPreConfig_instanciados) {

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 18 -  Número de aberturas para a simulação backward 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			if (!dadosPreConfig_instanciados)
				a_dados.setAtributo(AttComumDados_numero_aberturas, std::stoi(line.substr(21, 4)));

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 19 -  Número de séries sintéticas
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);


			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 20 -  Ordem máxima do modelo estocástico PAR(p) 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			if (!dadosPreConfig_instanciados) {
				a_dados.setAtributo(AttComumDados_ordem_maxima_auto_correlacao_geracao_cenario_hidrologico, std::stoi(line.substr(21, 4)));
			}

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 21 -  Ano inicial do arquivo de vazões históricas / Tamanho do registro do arquivo de vazões históricas
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);	

			const int ano_inicial_historico = std::stoi(line.substr(21, 4));

			atributo = line.substr(28, 1);
			atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			if (atributo == "0")//Manual: 0 = 320 palavras
				tamanho_registro_arquivo_vazoes_historicas = 320;
			else if (atributo == "1")//Manual: 1 = 600 palavras
				tamanho_registro_arquivo_vazoes_historicas = 600;
			else
				throw std::invalid_argument("Tamanho do registro do arquivo de vazoes historicas nao definido no arquivo DGER.DAT ");

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 22 -  Cálculo de energia armazenada inicial 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			atributo = line.substr(21, 4);
			atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			if (atributo == "0") {//0 = utiliza o valor do volume inicial informado no registro 23 deste arquivo

				try {
					std::ofstream escritaStream;
					escritaStream.open(SPT_avisos, std::ios_base::app);
					if (!escritaStream.is_open())
						throw std::invalid_argument("Nao foi possivel abrir o arquivo.");
					escritaStream << "Registro 22 - Modelo NEWAVE esta utilizando como volume inicial o registro por subsistema no arquivo DGER.DAT ao invés do HIDR.DAT" << std::endl;
					escritaStream.close();
					escritaStream.clear();
				} // try
				catch (const std::ofstream::failure& erro) { throw std::invalid_argument("Erro de integridade do arquivo " + SPT_avisos + ": \n" + std::string(erro.what())); }
				catch (const std::exception&         erro) { throw std::invalid_argument("Erro ao editar o arquivo " + SPT_avisos + ": \n" + std::string(erro.what())); }

			}//if (atributo == "0") {

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 23 -  Volume armazenado inicial (%) por REE
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Passa duas linhas porque a primeira é o cabeçalho
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 24 -  Probabilidade associada ao intervalo de confiança para convergência do algoritmo (%) 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 25 -  Taxa de desconto anual (%)
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			atributo = line.substr(21, 4);
			atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			if (!dadosPreConfig_instanciados) {
				const double taxa_desconto_anual = atof(atributo.c_str()) / 100;
				a_dados.setAtributo(AttComumDados_taxa_desconto_anual, taxa_desconto_anual);
			}

			a_dados.setAtributo(AttComumDados_multiplicador_tempo_limite_resolucao, 3.0);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 26 -  Simulação final após convergência PDDE
			//0 = não simula 1 = simulação com séries sintéticas 2 = simulação com a série histórica 3 = consistência de dados
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			if (!dadosPreConfig_instanciados) {

				strNormalizada(line);

				atributo = line.substr(21, 4);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				if (atributo == "1") {}
				else if (atributo == "2") {}
				else if (atributo == "3") {

					try {
						std::ofstream escritaStream;
						escritaStream.open(SPT_avisos, std::ios_base::app);
						if (!escritaStream.is_open())
							throw std::invalid_argument("Nao foi possivel abrir o arquivo.");
						escritaStream << "Registro 26 - Modelo NEWAVE utiliza simulação final com consistencia de dados" << std::endl;
						escritaStream.close();
						escritaStream.clear();
					} // try
					catch (const std::ofstream::failure& erro) { throw std::invalid_argument("Erro de integridade do arquivo " + SPT_avisos + ": \n" + std::string(erro.what())); }
					catch (const std::exception& erro) { throw std::invalid_argument("Erro ao editar o arquivo " + SPT_avisos + ": \n" + std::string(erro.what())); }

				}//else if (atributo == "3") {

			} // if (!dadosPreConfig_instanciados) {

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 27 -  Controle de impressão dos resultados da simulação final e do cálculo da política 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 28 -  Controle de impressão dos riscos de déficit e valor esperado da ENS
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 29 -  Este campo indica de quantas em quantas séries será gravado o relatório detalhado da simulação final.
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 30 -  1. Este campo contém o número mínimo de iterações para a convergência da política
			//               2. Este campo indica a iteração partir da qual será investigada a incerteza do parâmetro 
			//                  “valor esperado do custo total de operação obtido da função de custo futuro do 1° estágio - ZINF”
			//                   para proceder novo teste de convergência
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			const int line_size = int(line.length());

			if (line_size >= 29) {//Este campo indica a iteração partir da qual será investigada a incerteza do parâmetro -> O SPT set este valor como o numero_minimo_iteracoes

				atributo = line.substr(28, 1);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			}//if (line_size >= 29) {
			else {

				atributo = line.substr(21, 4);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			}//else {

			if (!dadosPreConfig_instanciados) {

				const int numero_minimo_iteracoes = atoi(atributo.c_str());

			}

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 31 -  Este campo indica a adoção ou não de corte de carga por otimização (CCO) na simulação final
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 32 -  Números de anos de informações de manutenção programada de usinas térmicas a serem considerados 
			//               no arquivo de dados de manutenção térmica 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			atributo = line.substr(21, 4);
			atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			numero_anos_manutencao_termica_programada = atoi(atributo.c_str());

			if (numero_anos_manutencao_termica_programada != 0 && numero_anos_manutencao_termica_programada != 1 && numero_anos_manutencao_termica_programada != 2)
				throw std::invalid_argument("Numero de anos de manuntencao nao especificado");

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 33 -  Consideração da tendência hidrológica no cálculo da política  
			//               Consideração da tendência hidrológica na Simulação Final
			//               0 = não será lido arquivo com a tendência hidrológica 1 = será lido arquivo com a tendência hidrológica por REE 2 = será lido arquivo com a tendência hidrológica por posto de medição
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			if (!dadosPreConfig_instanciados) {

				strNormalizada(line);

				atributo = line.substr(21, 4);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				TipoTendenciaEstocastica tipo_tendencia_hidrologica = TipoTendenciaEstocastica_serie_informada;

				if (atributo == "0") //não será lido arquivo com a tendência hidrológica  
					tipo_tendencia_hidrologica = TipoTendenciaEstocastica_sem_tendencia;
				else if (atributo == "1") {//será lido arquivo com a tendência hidrológica por REE

					try {
						std::ofstream escritaStream;
						escritaStream.open(SPT_avisos, std::ios_base::app);
						if (!escritaStream.is_open())
							throw std::invalid_argument("Nao foi possivel abrir o arquivo.");
						escritaStream << "Registro 33 - NEWAVE considera tendencia hidrologica dos Reservatorios Equivalentes" << std::endl;
						escritaStream.close();
						escritaStream.clear();
					} // try
					catch (const std::ofstream::failure& erro) { throw std::invalid_argument("Erro de integridade do arquivo " + SPT_avisos + ": \n" + std::string(erro.what())); }
					catch (const std::exception& erro) { throw std::invalid_argument("Erro ao editar o arquivo " + SPT_avisos + ": \n" + std::string(erro.what())); }

				}
				else if (atributo == "2")//será lido arquivo com a tendência hidrológica por posto de medição 
					tipo_tendencia_hidrologica = TipoTendenciaEstocastica_serie_informada;

				//Este valor é ignorado e considerado o mesmo tipo_tendencia_hidrologica para otimização e simulação
				atributo = line.substr(26, 4);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				a_dados.setAtributo(AttComumDados_tipo_tendencia_hidrologica, tipo_tendencia_hidrologica);

			}

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 34 -  Flag para consideração das restrições de Itaipu (flag desabilitado).
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 35 -  Flag para consideração do bid de demanda (função não implementada).
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 36 -  Flag para consideração das perdas de transmissão
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			atributo = line.substr(21, 4);
			atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			if (atributo == "1")
				considera_perdas_transmissao = true;

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 37 -  Flag para consideração do El Niño (função não implementada) 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 38 -  Índice de identificação ENSO (função não implementada). 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 39 -  Flag para tipo de duração do patamar.  
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			atributo = line.substr(21, 4);
			atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			if (atributo == "0")//Flag para tipo de duração do patamar. 0 = sazonal.
				duracao_patamar_carga_variavel_por_ano = false;

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 40 -  Flag para consideração de desvio de água 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			atributo = line.substr(21, 4);
			atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			if (atributo == "1")
				considera_desvio_de_agua = true;

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 41 -  Flag para consideração da energia de desvio de água como função da energia armazenada 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			atributo = line.substr(21, 4);
			atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			if (atributo == "1")
				considera_energia_desvio_funcao_da_energia_armazenada = true;

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 42 -  Flag para controle da curva de segurança 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 43 -  Flag para controle da geração de cenário de afluências para as simulações backward e forward 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 44 -   1. Profundidade para cálculo do risco de déficit (%) – primeiro valor
			//                2. Profundidade para cálculo do risco de déficit (%) – segundo valor
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 45 -   Funcionalidade pseudo-partida quente: número de iterações a ser considerada para a simulação final. 
			//                Se for zero, serão consideradas todas as iterações realizadas
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 46 -   Flag para consideração de agrupamento livre de  intercâmbios
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			atributo = line.substr(21, 4);
			atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			if (atributo == "1")
				considera_agrupamento_livre_de_intercambios = true;

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 47 -   Flag para consideração de equalização de penalidades de intercâmbio (flag desabilitado) 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 48 -   Flag para a consideração da representação da submotorização
			//Manual:
			// 0 – como função da potência instalada
			// 1 – como função da potência instalada e das energias afluentes médias históricas
			// 2 – como função da potência instalada, da energia afluente histórica da usina submotorizada e da regularização à montante da usina
			//DECK:
			// 0 - nao considera
			// 1 - subsistema
			// 2 - usina
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			//Segue a diretriz do DECK

			atributo = line.substr(21, 4);
			atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			if (atributo == "1") {//Subsitema

				try {
					std::ofstream escritaStream;
					escritaStream.open(SPT_avisos, std::ios_base::app);
					if (!escritaStream.is_open())
						throw std::invalid_argument("Nao foi possivel abrir o arquivo.");
					escritaStream << "Registro 48 - NEWAVE considera submotorização por subsistema" << std::endl;
					escritaStream.close();
					escritaStream.clear();
				} // try
				catch (const std::ofstream::failure& erro) { throw std::invalid_argument("Erro de integridade do arquivo " + SPT_avisos + ": \n" + std::string(erro.what())); }
				catch (const std::exception&         erro) { throw std::invalid_argument("Erro ao editar o arquivo " + SPT_avisos + ": \n" + std::string(erro.what())); }

			}//if (atributo == "1") {
			else if (atributo == "2")//Usina
				considera_submotorizacao = true;

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 49 -   Flag para a consideração da ordenação automática de subsistemas/submercados e classes térmicas
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 50 -   Flag para consideração do arquivo de cargas adicionais
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			atributo = line.substr(21, 4);
			atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			if (atributo == "1")
				considera_cargas_adicionais = true;

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 51 -   Valor percentual de ZSUP a ser subtraído de LINF para o critério de parada estatístico(%) 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 52 -   Valor máximo percentual para delta de ZINF no critério de parada não estatístico(%) 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			if (!dadosPreConfig_instanciados) {

				atributo = line.substr(21, 4);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				const double percentual_delta_zinf = atof(atributo.c_str()) / 100;

			}

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 53 -   Número de deltas de ZINF consecutivos a serem considerados no critério não estatístico
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			if (!dadosPreConfig_instanciados) {

				strNormalizada(line);

				atributo = line.substr(21, 4);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				const int numero_delta_zinf = atoi(atributo.c_str());

			}

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 54 -   Flag para consideração de despacho antecipado de usinas térmicas a gás natural liquefeito (GNL) 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			atributo = line.substr(21, 4);
			atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			if (atributo == "1")
				considera_despacho_antecipado_gnl = true;

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 55 -   Flag para modificação automática do montante de antecipação de despacho de uma usina GNL quando 
			//                a capacidade de geração máxima desta usina for inferior a este valor 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 56 -   Flag para consideração de restrições de geração hidráulica mínima 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			atributo = line.substr(21, 4);
			atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			if (atributo == "1")
				considera_geracao_hidraulica_minima = true;

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 57 -   Mês de início para o cálculo da simulação final
			//                Ano de início para o cálculo da simulação final
			//                Volume armazenado inicial (%) por REE para cálculo da simulação final.
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 58 -   Flag para utilização do gerenciador externo de processos 
			//                Flag para utilização da comunicação em dois níveis 
			//                Flag para utilização de armazenamento local de arquivos temporários
			//                Flag para utilização de alocação em memória da energia natural afluente
			//                Flag para utilização de alocação em memória dos cortes da função de custo futuro 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 59 -   Flag para utilização de mecanismo de aversão a risco: SAR 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 60 -   Flag para utilização de mecanismo de aversão a risco: CVaR
			//0 – não considera              1 – considera, constante no tempo              2 – considera, variável no tempo
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			if (!dadosPreConfig_instanciados) {

				atributo = line.substr(21, 4);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				TipoAversaoRisco tipo_aversao_a_risco = TipoAversaoRisco_CVAR;

				if (atributo == "0") {//Não considera

					tipo_aversao_a_risco = TipoAversaoRisco_valor_esperado;

					try {
						std::ofstream escritaStream;
						escritaStream.open(SPT_avisos, std::ios_base::app);
						if (!escritaStream.is_open())
							throw std::invalid_argument("Nao foi possivel abrir o arquivo.");
						escritaStream << "Registro 60 - NEWAVE nao considera o CVaR, neste caso o SPT considera o valor esperado como medida de risco" << std::endl;
						escritaStream.close();
						escritaStream.clear();
					} // try
					catch (const std::ofstream::failure& erro) { throw std::invalid_argument("Erro de integridade do arquivo " + SPT_avisos + ": \n" + std::string(erro.what())); }
					catch (const std::exception& erro) { throw std::invalid_argument("Erro ao editar o arquivo " + SPT_avisos + ": \n" + std::string(erro.what())); }


				}//if (atributo == "0") {
				else if (atributo == "1")// =1 CONSIDERA CTE TEMPO
					considera_CVAR_variavel_no_tempo = false;
				else if (atributo == "2")// =2 CONSIDERA VARIAVEL NO TEMP
					considera_CVAR_variavel_no_tempo = true;

				a_dados.setAtributo(AttComumDados_tipo_aversao_a_risco, tipo_aversao_a_risco);

			}

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 61 -   Flag para consideração do critério de mínimo ZSUP para convergência
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 62 -   Flag para não consideração do requisito de vazão mínima  / deck: DESCONSIDERA VAZMIN
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			atributo = line.substr(21, 4);
			atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			if (atributo == "1")
				considera_vazao_minima = false;

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 63 -   Flag para consideração de restrições elétricas internas aos REEs 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			atributo = line.substr(21, 4);
			atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			if (atributo == "1")
				considera_restricoes_eletricas = true;

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 64 -   Flag para consideração do procedimento de Seleção de Cortes de Benders 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			if (!dadosPreConfig_instanciados) {

				atributo = line.substr(21, 4);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				if (atributo == "0")//0-Não considera
					a_dados.setAtributo(AttComumDados_selecao_cortes_nivel_dominancia, 0);
				else if (atributo == "1")//1- considera
					a_dados.setAtributo(AttComumDados_selecao_cortes_nivel_dominancia, 1);

			}

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 65 -   Flag para consideração de Janela de Cortes de Benders
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 66 -   Flag para consideração Reamostragem de Cenários   0 – não considera                1 – considera
			//            -   Tipo de Reamostragem de Cenários                  0 – Recombinação               1 – Plena 
			//            -   Passo para Reamostragem de Cenários. Permitido valores entre 1 e número máximo de iterações
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			strNormalizada(line);

			atributo = line.substr(21, 4);
			atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			bool amostrar_cenarios_otimizacao = true;

			if (atributo == "0") {

				try {
					std::ofstream escritaStream;
					escritaStream.open(SPT_avisos, std::ios_base::app);
					if (!escritaStream.is_open())
						throw std::invalid_argument("Nao foi possivel abrir o arquivo.");
					escritaStream << "Registro 66 - NEWAVE nao considera reamostragem em cada etapa Forward. O SPT considera" << std::endl;
					escritaStream.close();
					escritaStream.clear();
				} // try
				catch (const std::ofstream::failure& erro) { throw std::invalid_argument("Erro de integridade do arquivo " + SPT_avisos + ": \n" + std::string(erro.what())); }
				catch (const std::exception&         erro) { throw std::invalid_argument("Erro ao editar o arquivo " + SPT_avisos + ": \n" + std::string(erro.what())); }

			}//if (atributo == "0") {


			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 67 -   Flag para consideração do Nó Zero no cálculo de ZINF 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Registro 68 -   Consulta à função de custo futuro ao longo das iterações da PDDE
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);

			//Fecha a leitura do arquivo
			leituraArquivo.close();

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Determina o vetor horizonte_otimização e horizonte_estudo
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			if (a_dados.getSizeVetor(AttVetorDados_horizonte_otimizacao) == 0) {

				SmartEnupla<IdEstagio, Periodo> horizonte_otimizacao;
				SmartEnupla<Periodo, IdEstagio> horizonte_estudo;

				Periodo periodo = periodo_estudo_inicial;

				for (IdEstagio idEstagio = IdEstagio_1; idEstagio <= idEstagio_final_otimizacao; idEstagio++) {

					horizonte_otimizacao.addElemento(periodo);

					horizonte_estudo.addElemento(periodo, idEstagio);

					if (periodo.getTipoPeriodo() != TipoPeriodo_mensal) {

						periodo = Periodo(TipoPeriodo_mensal, periodo + 1);

						if ((periodo.getDia() != IdDia_1) || (periodo.getHora() != IdHor_0) || (periodo.getMinuto() != IdMin_0))
							throw std::invalid_argument("A data inicial do segundo periodo de estudo deve corresponder ao inicio de um mes.");

					}
					else
						periodo++;

				}//for (IdEstagio idEstagio = IdEstagio_1; idEstagio <= idEstagio_final_otimizacao; idEstagio++) {

				a_dados.setVetor(AttVetorDados_horizonte_otimizacao, horizonte_otimizacao);
				a_dados.setVetor(AttVetorDados_horizonte_estudo, horizonte_estudo);

			} // if (a_dados.getSizeVetor(AttVetorDados_horizonte_otimizacao) == 0) {

			const Periodo periodo_pos_manutencao_programada = Periodo(IdMes_1, IdAno(horizonte_estudo_DECK.getIteradorInicial().getAno() + numero_anos_manutencao_termica_programada));

			const SmartEnupla<Periodo, IdEstagio>  horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
				const double sobreposicao = periodo.sobreposicao(periodo_pos_manutencao_programada);
				if ((periodo < periodo_pos_manutencao_programada) && (sobreposicao > 0.0))
					throw std::invalid_argument("O periodo de estudo " + getString(periodo) +  " nao deve incidir parcialmente sobre o periodo de pos manutencao programada " + getString(periodo_pos_manutencao_programada) + ".");
			}


		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_NEWAVE_26_DGER: \n" + std::string(erro.what())); }


}

void LeituraCEPEL::leitura_SISTEMA_201908_NW25(Dados &a_dados, std::string nomeArquivo) {

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;

		////////////////////////////////////////////////////////

		if (leituraArquivo.is_open()) {

			//Horizonte de estudo
			const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

			double sobreposicao_atraso_periodo_inicial = 0.0;

			if (desconsiderar_atraso_periodo_estudo_inicial)
				sobreposicao_atraso_periodo_inicial = horizonte_estudo.getIteradorInicial().atraso(horizonte_estudo_DECK.getIteradorInicial());
	

			//*******************************************************************************************************************
			//Bloco 1 -   Este bloco é composto por um registro especificando o total de patamares de déficit
			//*******************************************************************************************************************

			//Três registros destinados a comentários
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);

			std::getline(leituraArquivo, line);

			strNormalizada(line);

			//Número de patamares de déficit
			atributo = line.substr(1, 3);
			atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			const int numero_patamares_deficit = atoi(atributo.c_str());

			const IdPatamarDeficit maiorIdPatamarDeficit = getIdPatamarDeficitFromChar(atributo.c_str());


			//*******************************************************************************************************************
			//Bloco 2 - Este bloco é composto por tantos registros quantos forem os subsistemas/submercados considerados. 
			//        Cada registro contém informações sobre os patamares de déficit. O código 999 no campo 1 indica final do bloco
			//*******************************************************************************************************************

			//Três registros destinados a comentários
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);

			while (true) {

				std::getline(leituraArquivo, line);

				strNormalizada(line);

				//Número do subsistema/submercado ou critério de parada
				atributo = line.substr(1, 3);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				if (atributo == "999")
					break;

				const int codigo_usina = std::atoi(atributo.c_str());

				//Nome do subsistema/submercado
				atributo = line.substr(5, 10);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				const string nome_submercado = atributo;

				//Tipo do subsistema/submercado
				atributo = line.substr(17, 1);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				bool ficticio = false;

				if (atributo == "1")
					ficticio = true;

				//*******************************************************************************************************************
				//Set infromação na classe Submercado
				//*******************************************************************************************************************

				Submercado submercado;

				const IdSubmercado idSubmercado = IdSubmercado(a_dados.getMaiorId(IdSubmercado()) + 1);

				submercado.setAtributo(AttComumSubmercado_idSubmercado, idSubmercado);

				lista_codigo_ONS_submercado.addElemento(idSubmercado, codigo_usina);

				submercado.setAtributo(AttComumSubmercado_nome, nome_submercado);
				submercado.setAtributo(AttComumSubmercado_ficticio, ficticio);

				for (int patamar_deficit = 0; patamar_deficit < numero_patamares_deficit; patamar_deficit++) {

					if (ficticio == false) {

						//Custo do déficit do patamar ($/MWh) 
						atributo = line.substr(19 + 8 * patamar_deficit, 7);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						const double custo = atof(atributo.c_str());

						//Profundidade do patamar de déficit (p.u.) 
						atributo = line.substr(51 + 6 * patamar_deficit, 7);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						const double percentual_da_demanda = atof(atributo.c_str());

						PatamarDeficit patamarDeficit;

						const IdPatamarDeficit idPatamarDeficit = getIdPatamarDeficitFromChar(std::to_string(patamar_deficit + 1).c_str());

						patamarDeficit.setAtributo(AttComumPatamarDeficit_idPatamarDeficit, idPatamarDeficit);
						patamarDeficit.setAtributo(AttComumPatamarDeficit_custo, custo);
						patamarDeficit.setAtributo(AttComumPatamarDeficit_percentual, percentual_da_demanda);

						submercado.vetorPatamarDeficit.add(patamarDeficit);

					}//if (ficticio == false) {

				}//for (int patamar_deficit = 0; patamar_deficit < numero_patamares_deficit; patamar_deficit++) {

				//Adiciona um novo submercado no vetorSubmercado
				a_dados.vetorSubmercado.add(submercado);

			}//while (true) {

			//*******************************************************************************************************************
			//Bloco 3 - LIMITE DE INTERCÂMBIO OU INTERCÂMBIO MÍNIMO OBRIGATÓRIO
			//          Este bloco é composto por três tipos de registros. Para cada registro tipo 1, haverá tantos registros 
			//          tipos 2 e 3 quantos for o número de anos do período de planejamento. Os registros tipo 2 e 3 devem ser 
			//          fornecidos agrupadamente e os grupos serão separados por um registro em branco, de existência obrigatória. 
			//          Esta disposição dos registros tipo 1, 2 e 3 será repetida tantas quantas forem as interligações entre os 
			//          subsistemas/submercados. O código 999 no campo 1 indica final do bloco
			//*******************************************************************************************************************

			//Três registros destinados a comentários
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);

			////////////////

			IdSubmercado idSubmercado_origem;
			IdSubmercado idSubmercado_destino;

			while (true) {

				////////////////////////////////////////
				// Registro Tipo 1
				////////////////////////////////////////

				std::getline(leituraArquivo, line);

				strNormalizada(line);

				const int line_size = int(line.length());

				bool intercambio_A_para_B = false;

				bool limite_intercambio = true; //Considera-se por default, alguns registros no DECK não indicam este valor, ex, DECK201907;

				if (line_size >= 4) {//Significa que a linha tem pelo menos o primeiro registro (quando é registrado os limites do intercâmbio B -> A não são informados os subsistemas, são utilizados os previamente lidos

				//Número do subsistema/submercado A
					atributo = line.substr(1, 3);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					if (atributo != "") {//Nos Decks foram encontradas linhas com somente vazios lixo 

						intercambio_A_para_B = true;

						if (atributo == "999") //Parada da leitura do bloco 3
							break;

						const int submercado_origem_codigo_usina = atoi(atributo.c_str());

						idSubmercado_origem = getIdFromCodigoONS(lista_codigo_ONS_submercado, submercado_origem_codigo_usina);

						//Número do subsistema/submercado B

						if (line_size < 8)
							throw std::invalid_argument("SISTEMA.DAT registro somente 1 submercado para um intercambio, ver Bloco 3 Registro tipo 1 ");

						atributo = line.substr(5, 3);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						if (atributo == "")
							throw std::invalid_argument("SISTEMA.DAT registro somente 1 submercado para um intercambio, ver Bloco 3 Registro tipo 1 ");

						const int submercado_destino_codigo_usina = atoi(atributo.c_str());

						idSubmercado_destino = getIdFromCodigoONS(lista_codigo_ONS_submercado, submercado_destino_codigo_usina);

						//Flag 0 = limite de intercâmbio; 1 = intercâmbio mínimo obrigatório.

						if (line_size >= 24) {

							atributo = line.substr(23, 1);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							if (atributo == "1") //1 = intercâmbio mínimo obrigatório.
								limite_intercambio = false;

						}//if (line_size >= 24) {

						//Flag para considerar penalidade interna de intercâmbio	

						if (line_size >= 32) {

							atributo = line.substr(31, 1);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							if (atributo == "1") {//0 = considera penalidade - 1 = não considera penalidade

								try {
									std::ofstream escritaStream;
									escritaStream.open(SPT_avisos, std::ios_base::app);
									if (!escritaStream.is_open())
										throw std::invalid_argument("Nao foi possivel abrir o arquivo.");
									escritaStream << "SISTEMA.DAT: Bloco 3 Registro Tipo 1 : NEWAVE nao considera penalidade entre intercambios" << std::endl;
									escritaStream.close();
									escritaStream.clear();
								} // try
								catch (const std::ofstream::failure& erro) { throw std::invalid_argument("Erro de integridade do arquivo " + SPT_avisos + ": \n" + std::string(erro.what())); }
								catch (const std::exception&         erro) { throw std::invalid_argument("Erro ao editar o arquivo " + SPT_avisos + ": \n" + std::string(erro.what())); }

							}//if (atributo == "1") {

						}//if (line_size >= 32) {

					}//if (atributo != "") {

				}//if (line_size >= 4) {

				//Cria classe Intercambio

				Intercambio intercambio;

				const IdIntercambio idIntercambio = IdIntercambio(a_dados.getMaiorId(IdIntercambio()) + 1);

				intercambio.setAtributo(AttComumIntercambio_idIntercambio, idIntercambio);

				SmartEnupla<Periodo, double> lista_intercambio_potencia_minima(horizonte_estudo_DECK, 0.0);
				SmartEnupla<Periodo, double> lista_intercambio_potencia_maxima(horizonte_estudo_DECK, 0.0);

				if (intercambio_A_para_B == true) {

					intercambio.setAtributo(AttComumIntercambio_submercado_origem, idSubmercado_origem);
					intercambio.setAtributo(AttComumIntercambio_submercado_destino, idSubmercado_destino);

					const std::string nome_submercado_origem  = a_dados.getAtributo(idSubmercado_origem, AttComumSubmercado_nome, std::string());
					const std::string nome_submercado_destino = a_dados.getAtributo(idSubmercado_destino, AttComumSubmercado_nome, std::string());

					const std::string nome_intercambio = nome_submercado_origem + "->" + nome_submercado_destino;

					intercambio.setAtributo(AttComumIntercambio_nome, nome_intercambio);

				}//if (intercambio_A_para_B == true) {
				else if (intercambio_A_para_B == false) {

					//Neste caso, somente é necessário trocar a origem por destino
					intercambio.setAtributo(AttComumIntercambio_submercado_origem, idSubmercado_destino);
					intercambio.setAtributo(AttComumIntercambio_submercado_destino, idSubmercado_origem);

					const std::string nome_submercado_origem = a_dados.getAtributo(idSubmercado_destino, AttComumSubmercado_nome, std::string());
					const std::string nome_submercado_destino = a_dados.getAtributo(idSubmercado_origem, AttComumSubmercado_nome, std::string());

					const std::string nome_intercambio = nome_submercado_origem + "->" + nome_submercado_destino;

					intercambio.setAtributo(AttComumIntercambio_nome, nome_intercambio);

				}//else if (intercambio_A_para_B == false) {

				a_dados.vetorIntercambio.add(intercambio);

				a_dados.vetorIntercambio.att(idIntercambio).setVetor(AttVetorIntercambio_potencia_minima, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
				a_dados.vetorIntercambio.att(idIntercambio).setVetor(AttVetorIntercambio_potencia_maxima, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

				////////////////////////////////////////
				// Registro Tipo 2 e Tipo 3
				////////////////////////////////////////

				std::getline(leituraArquivo, line);

				strNormalizada(line);

				Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial();

				while (true) {

					atributo = line.substr(7 + 8 * (int(periodo_DECK.getMes()) - 1), 7);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const double valor_intercambio = atof(atributo.c_str());

					//Guarda informação no Smart Elemento

					if (limite_intercambio == true)
						lista_intercambio_potencia_maxima.at(periodo_DECK) = valor_intercambio;
					else if (limite_intercambio == false && intercambio_A_para_B == true) {//intercâmbio mínimo obrigatório. Manual: Adicionalmente, para cada mês do período de estudo que tenha a restrição de intercâmbio mínimo obrigatório, a capacidade de intercâmbio no sentido oposto da linha deve ser zero

						lista_intercambio_potencia_minima.at(periodo_DECK) = valor_intercambio;
						lista_intercambio_potencia_maxima.at(periodo_DECK) = valor_intercambio;

					}//else if (limite_intercambio == false) {

					//Critério de parada

					if (periodo_DECK == horizonte_estudo_DECK.getIteradorFinal())
						break;

					//Passo de linha e atualização do periodo

					if (periodo_DECK.getMes() == IdMes_12) {

						std::getline(leituraArquivo, line);

						strNormalizada(line);

					}//if (mes == 12) {

					horizonte_estudo_DECK.incrementarIterador(periodo_DECK);

				}//while (true) {

				for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo_DECK)) {

					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						double sobreposicao = periodo.sobreposicao(periodo_DECK);

						if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
							sobreposicao += sobreposicao_atraso_periodo_inicial;

						if (sobreposicao > 0.0) {

							const double valor_antigo_minimo = a_dados.getElementoVetor(idIntercambio, AttVetorIntercambio_potencia_minima, periodo, double());
							const double valor_antigo_maximo = a_dados.getElementoVetor(idIntercambio, AttVetorIntercambio_potencia_maxima, periodo, double());

							a_dados.vetorIntercambio.att(idIntercambio).setElemento(AttVetorIntercambio_potencia_minima, periodo, valor_antigo_minimo + sobreposicao * lista_intercambio_potencia_minima.at(periodo_DECK));
							a_dados.vetorIntercambio.att(idIntercambio).setElemento(AttVetorIntercambio_potencia_maxima, periodo, valor_antigo_maximo + sobreposicao * lista_intercambio_potencia_maxima.at(periodo_DECK));

						} // if ((periodo >= a_modificacaoUHE.periodo) || (sobreposicao > 0.0)) {

					} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				} // for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo_DECK)) {

			}//while (true) {

			//*******************************************************************************************************************
			//Bloco 4 - MERCADO DE ENERGIA TOTAL
			//          Este bloco é composto por tantos conjuntos de registros quantos forem os subsistemas/submercados. 
			//          Cada conjunto pode ser composto de dois a quatro tipos de registro. O primeiro registro (tipo 1) 
			//          identifica o subsistema/submercado. Este tipo será seguido por de um até três tipos de registro. 
			//          Se houver período inicial para fins de estabilização deverá haver um registro tipo 3, com o mercado 
			//          estático para este período inicial. A seguir, haverá tantos registros tipo 2 quantos forem os anos 
			//          de planejamento. Cada registro tipo 2 contém o mercado de energia do subsistema/submercado. E em seguida, 
			//          se houver período final para fins de estabilização deverá haver um registro tipo 4, com o mercado estático
			//          para este período final. Portanto, se não houver período inicial e/ou final para fins de estabilização, 
			//          os respectivos registros não deverão ser informados. O código 999 no campo 1 indica final do bloco
			//*******************************************************************************************************************

			//Três registros destinados a comentários
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);

			////////////////

			while (true) {

				////////////////////////////////////////
				// Registro Tipo 1
				////////////////////////////////////////

				while (true) {

					std::getline(leituraArquivo, line);

					strNormalizada(line);

					atributo = line.substr(1, 3);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					if (atoi(atributo.c_str()) > 0)//Evita POS ou PRE já que o atoi retorna 0 se for uma string
						break;

				}//while (true) {

				//Número do subsistema/submercado
				atributo = line.substr(1, 3);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				if (atributo == "999") //Parada da leitura do bloco 3
					break;

				const int submercado_codigo_usina = atoi(atributo.c_str());

				const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, submercado_codigo_usina);

				if (idSubmercado == IdSubmercado_Nenhum)
					throw std::invalid_argument("SISTEMA.DAT submercado nao instanciado, ver Bloco 4 Registro tipo 1 ");

				a_dados.vetorSubmercado.att(idSubmercado).setVetor_forced(AttVetorSubmercado_demanda, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

				////////////////////////////////////////
				// Registro Tipo 2
				////////////////////////////////////////

				std::getline(leituraArquivo, line);

				strNormalizada(line);

				Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial();

				SmartEnupla<Periodo, double> demanda(horizonte_estudo_DECK, 0.0);

				while (true) {

					atributo = line.substr(7 + 8 * (int(periodo_DECK.getMes()) - 1), 7);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const double valor_demanda = atof(atributo.c_str());

					//Guarda informação no Smart Elemento

					demanda.setElemento(periodo_DECK, valor_demanda);

					//Critério de parada

					if (periodo_DECK == horizonte_estudo_DECK.getIteradorFinal())
						break;

					//Passo de linha e atualização do periodo

					if (periodo_DECK.getMes() == IdMes_12) {

						std::getline(leituraArquivo, line);

						strNormalizada(line);

					}//if (mes == 12) {

					horizonte_estudo_DECK.incrementarIterador(periodo_DECK);

				}//while (true) {


				for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo_DECK)) {

					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						double sobreposicao = periodo.sobreposicao(periodo_DECK);

						if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
							sobreposicao += sobreposicao_atraso_periodo_inicial;

						if (sobreposicao > 0.0) {

							const double valor_antigo = a_dados.getElementoVetor(idSubmercado, AttVetorSubmercado_demanda, periodo, double());

							a_dados.vetorSubmercado.att(idSubmercado).setElemento(AttVetorSubmercado_demanda, periodo, valor_antigo + sobreposicao * demanda.at(periodo_DECK));

						} // if ((periodo >= a_modificacaoUHE.periodo) || (sobreposicao > 0.0)) {

					} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				} // for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo_DECK)) {


			}//while (true) {

			//*******************************************************************************************************************
			//Bloco 5 - GERACAO DE USINAS NAO SIMULADAS
			//          Este bloco é composto por tantos conjuntos de registros quanto o necessário. Poderá existir mais de um 
			//          conjunto de registro para o mesmo subsistema/submercado. Cada conjunto é composto por dois tipos de registro.
			//          O primeiro registro (tipo 1) identifica o subsistema/submercado e o bloco de usinas não simuladas,  
			//          sendo seguido por tantos registros tipo 2 quantos forem os anos de planejamento. Cada registro tipo 2 contém 
			//          a geração de pequenas usinas do subsistema/submercado. O código 999 no campo 1 indica final do bloco.
			//*******************************************************************************************************************


			//Três registros destinados a comentários
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);

			////////////////

			while (true) {

				////////////////////////////////////////
				// Registro Tipo 1
				////////////////////////////////////////

				std::getline(leituraArquivo, line);

				strNormalizada(line);

				const int line_size = int(line.length());

				//Número do subsistema/submercado
				atributo = line.substr(1, 3);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				if (atributo == "999") //Parada da leitura do bloco 3
					break;

				const int submercado_codigo_usina = atoi(atributo.c_str());

				const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, submercado_codigo_usina);

				if (idSubmercado == IdSubmercado_Nenhum)
					throw std::invalid_argument("SISTEMA.DAT submercado nao instanciado, ver Bloco 5 Registro tipo 1 ");

				
				IdUsinaNaoSimulada idUsinaNaoSimulada = IdUsinaNaoSimulada_1; //Por padrao IdUsinaNaoSimulada_1 é a total, se os valores estao desagregados instancia outros IdUsinaNaoSimulada

				if (line_size >= 9) {

					//Número do bloco de usinas não simuladas
					atributo = line.substr(5, 4); //Erro no manual, a posicao está entre a 6-9
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					if (atributo != "")
						idUsinaNaoSimulada = IdUsinaNaoSimulada(atoi(atributo.c_str()));

				}//if (line_size >= 9) {

				UsinaNaoSimulada usinaNaoSimulada;
				usinaNaoSimulada.setAtributo(AttComumUsinaNaoSimulada_idUsinaNaoSimulada, idUsinaNaoSimulada);

				a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.add(usinaNaoSimulada);

				//Descrição do bloco de usinas não simuladas
				if (line.size() >= 20) {
					atributo = line.substr(11, 20);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
				}
				else
					atributo = "";

				std::string nome = "Nao_informado";

				if (atributo != "")
					nome = atributo;

				a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).setAtributo(AttComumUsinaNaoSimulada_nome, nome);

				a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).setVetor(AttVetorUsinaNaoSimulada_potencia_minima, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
				a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).setVetor(AttVetorUsinaNaoSimulada_potencia_maxima, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

				//*******************************************************************************************************************
				//Set infromação na classe UsinaNaoSimulada
				//*******************************************************************************************************************

				////////////////////////////////////////
				// Registro Tipo 2
				////////////////////////////////////////

				std::getline(leituraArquivo, line);

				strNormalizada(line);

				Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial();

				SmartEnupla<Periodo, double> potencia_maxima(horizonte_estudo_DECK, 0.0);

				while (true) {

					atributo = line.substr(7 + 8 * (int(periodo_DECK.getMes()) - 1), 7);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const double valor_demanda = atof(atributo.c_str());

					//Guarda informação no Smart Elemento

					potencia_maxima.setElemento(periodo_DECK, valor_demanda);

					//Critério de parada

					if (periodo_DECK == horizonte_estudo_DECK.getIteradorFinal())
						break;

					//Passo de linha e atualização do periodo

					if (periodo_DECK.getMes() == IdMes_12) {

						std::getline(leituraArquivo, line);

						strNormalizada(line);

					}//if (mes == 12) {

					horizonte_estudo_DECK.incrementarIterador(periodo_DECK);

				}//while (true) {


				for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo_DECK)) {

					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						double sobreposicao = periodo.sobreposicao(periodo_DECK);

						if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
							sobreposicao += sobreposicao_atraso_periodo_inicial;

						if (sobreposicao > 0.0) {

							const double valor_antigo_minimo = a_dados.getElementoVetor(idSubmercado, idUsinaNaoSimulada, AttVetorUsinaNaoSimulada_potencia_minima, periodo, double());
							const double valor_antigo_maximo = a_dados.getElementoVetor(idSubmercado, idUsinaNaoSimulada, AttVetorUsinaNaoSimulada_potencia_maxima, periodo, double());

							a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).setElemento(AttVetorUsinaNaoSimulada_potencia_minima, periodo, valor_antigo_minimo + sobreposicao * potencia_maxima.at(periodo_DECK));
							a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).setElemento(AttVetorUsinaNaoSimulada_potencia_maxima, periodo, valor_antigo_maximo + sobreposicao * potencia_maxima.at(periodo_DECK));

						} // if ((periodo >= a_modificacaoUHE.periodo) || (sobreposicao > 0.0)) {

					} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				} // for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo_DECK)) {

			}//while (true) {

			leituraArquivo.close();

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_NEWAVE_26_SISTEMA: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_CONFHD_201908_NW25(Dados &a_dados, std::string nomeArquivo) {

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;

		////////////////////////////////////////////////////////

		int conteio_usinas_com_expansao_instanciadas = 0; //Atributo necessário no caso que hidreletricasPreConfig_instanciadas = true

		if (leituraArquivo.is_open()) {

			//Pula o cabeçalho
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);

				//Campo 1 -  Número da usina 
				const int codigo_usina = std::stoi(line.substr(1, 4));

				//Campo 2 -  Nome da usina 
				string  nome = line.substr(6, 12);
				nome.erase(std::find_if(nome.rbegin(), nome.rend(), std::bind1st(std::not_equal_to<char>(), ' ')).base(), nome.end());
							   
				//Campo 3 -  Número do posto de vazões da usina 
				const int codigo_posto_CEPEL = std::stoi(line.substr(19, 4));

				//Campo 4 -  Número da usina a jusante (código da usina no cadastro de usinas hidroelétricas)
					const int usina_jusante_codigo_usina = std::stoi(line.substr(25, 4));

				//Campo 5 -  Número do REE a que pertence a usina 
				const int codigo_REE_CEPEL = std::stoi(line.substr(30, 4));

				//Campo 6 -  Volume armazenado inicial em percentagem do volume útil 
				const double percentual_volume_inicial = std::stof(line.substr(35, 6)) / 100;
				
				//Campo 7 -  Indicador de usina existente e/ou em expansão
				atributo = line.substr(44, 2);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				bool considerar_usina     = true;
				bool usina_com_expansao   = false;
				bool usina_sem_capacidade = false;

				if (atributo == "EE"){
					usina_com_expansao   = true;
					usina_sem_capacidade = true;
				}
				else if (atributo == "NE") {
					usina_com_expansao   = true;
					usina_sem_capacidade = true;
				}

				else if (atributo == "NC")
					considerar_usina = false;


				// Hidrelétricas Fictícias não são instanciadass

				if (nome.length() >= 5)
					if (strCompara(nome.substr(0, 5), "FICT."))
						considerar_usina = false;

				//Campo 8 -  Índice de modificação de dados da usina 
				bool usina_com_modificacao = false;

				if (std::stoi(line.substr(49, 4)) == 1)
					usina_com_modificacao = true;

				//Campo 9 -  Primeiro ano do histórico de vazões, do posto correspondente à usina, considerado para ajuste do modelo de energias afluentes 
				//atributo = line.substr(58, 4);

				//Campo 10 -  Último ano do histórico de vazões, do posto correspondente à usina, considerado para ajuste do modelo de energias afluentes
				//atributo = line.substr(67, 4);

				//*******************************************************************************************************************
				//Set infromação na classe Hidreletrica
				//*******************************************************************************************************************

				if (lista_codigo_ONS_hidreletrica_original.size() == 0) {
					lista_codigo_ONS_hidreletrica_original.addElemento(1, codigo_usina);
					lista_codigo_ONS_hidreletrica_jusante_original.addElemento(1, usina_jusante_codigo_usina);
				}
				else {
					lista_codigo_ONS_hidreletrica_original.addElemento(codigo_usina);
					lista_codigo_ONS_hidreletrica_jusante_original.addElemento(usina_jusante_codigo_usina);
				}

				if (hidreletricasPreConfig_instanciadas) {

					const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

					int hidreletrica_agregada = 0;

					for (int pos = 1; pos <= int(lista_codigo_ONS_hidreletrica_agregada.size()); pos++) {

						if (lista_codigo_ONS_hidreletrica_agregada.getElemento(pos) == codigo_usina) {
							hidreletrica_agregada = pos;
							break;
						}//if (lista_codigo_ONS_hidreletrica_agregada.getElemento(pos) == codigo_usina) {

					}//for (int pos = 1; pos <= int(lista_codigo_ONS_hidreletrica_agregada.size()); pos++) {

					if (idHidreletrica != IdHidreletrica_Nenhum) {

						const IdBaciaHidrografica idBaciaHidrografica = a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_bacia, IdBaciaHidrografica());
					
						if (!a_dados.vetorBaciaHidrografica.isInstanciado(idBaciaHidrografica)) {

							BaciaHidrografica bacia;
							bacia.setAtributo(AttComumBaciaHidrografica_idBaciaHidrografica, idBaciaHidrografica);
							bacia.setAtributo(AttComumBaciaHidrografica_nome, getFullString(idBaciaHidrografica));
							a_dados.vetorBaciaHidrografica.add(bacia);

						} // if (!a_dados.vetorBaciaHidrografica.isInstanciado(idBaciaHidrografica)) {

						lista_codigo_ONS_REE.setElemento(idHidreletrica, codigo_REE_CEPEL);

						if (a_dados.getMaiorId(idHidreletrica, IdReservatorio()) == IdReservatorio_Nenhum) {
							Reservatorio reservatorio;
							reservatorio.setAtributo(AttComumReservatorio_idReservatorio, IdReservatorio_1); //Somente existe um reservatório por usina, portanto, todos tem IdReservatorio_1
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.add(reservatorio);
							a_dados.volume_inicial_carregado_from_premissa = true;
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_percentual_volume_util_inicial, percentual_volume_inicial);
						}

						a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_nome, nome);
						a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_considerar_usina, true);

						/////////////////////////////////////////////////////////
						//Caso especial: Usina Sobradinho -> codigo_usina = 169
						//Nota: Precisa mudar o posto devido a que o posto do curto prazo é o incremental (posto = 168) e o MP precisa para seus cálculos o posto natural (posto = 169)
						/////////////////////////////////////////////////////////
						if (codigo_usina == 169)
							a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_codigo_posto, 169);
						/////////////////////////////////////////////////////////

						lista_hidreletrica_com_expansao.setElemento(idHidreletrica, usina_com_expansao);
						lista_hidreletrica_com_modificacao.setElemento(idHidreletrica, usina_com_modificacao);
						lista_hidreletrica_sem_capacidade.setElemento(idHidreletrica, usina_sem_capacidade);

					} // if (idHidreletrica != IdHidreletrica_Nenhum)

					//Instancia as usinas com expansão (usinas não existentes na atualidade)
					else if ((idHidreletrica == IdHidreletrica_Nenhum) && (usina_com_expansao) && (considerar_usina) && (hidreletrica_agregada == 0)) {

						conteio_usinas_com_expansao_instanciadas++;

						const IdHidreletrica idHidreletrica_expansao = IdHidreletrica(a_dados.getMaiorId(IdHidreletrica()) + 1);

						if (true) {
							Hidreletrica hidreletrica;
							hidreletrica.setAtributo(AttComumHidreletrica_idHidreletrica, idHidreletrica_expansao);
							a_dados.vetorHidreletrica.add(hidreletrica);

						} // if (true) {				

						lista_hidreletrica_pre_configuracao.addElemento(idHidreletrica_expansao, false);

						lista_codigo_ONS_REE.addElemento(idHidreletrica_expansao, codigo_REE_CEPEL);

						lista_codigo_ONS_hidreletrica.addElemento(idHidreletrica_expansao, codigo_usina);

						if (true) {
							Reservatorio reservatorio;
							reservatorio.setAtributo(AttComumReservatorio_idReservatorio, IdReservatorio_1); //Somente existe um reservatório por usina, portanto, todos tem IdReservatorio_1
							a_dados.vetorHidreletrica.att(idHidreletrica_expansao).vetorReservatorio.add(reservatorio);
							a_dados.volume_inicial_carregado_from_premissa = true;
							a_dados.vetorHidreletrica.att(idHidreletrica_expansao).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_percentual_volume_util_inicial, percentual_volume_inicial);
						} // if (true) {

						a_dados.vetorHidreletrica.att(idHidreletrica_expansao).setAtributo(AttComumHidreletrica_nome, nome);
						a_dados.vetorHidreletrica.att(idHidreletrica_expansao).setAtributo(AttComumHidreletrica_codigo_usina, codigo_usina);
						a_dados.vetorHidreletrica.att(idHidreletrica_expansao).setAtributo(AttComumHidreletrica_considerar_usina, true);
						a_dados.vetorHidreletrica.att(idHidreletrica_expansao).setAtributo(AttComumHidreletrica_codigo_posto, codigo_posto_CEPEL);
						a_dados.vetorHidreletrica.att(idHidreletrica_expansao).setAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica_por_usina);

						lista_IdSubmercado_hidreletrica.addElemento(idHidreletrica_expansao, IdSubmercado_Nenhum);

						lista_hidreletrica_maiorIdConjuntoHidraulico.addElemento(idHidreletrica_expansao, IdConjuntoHidraulico_Nenhum);
						lista_hidreletrica_com_expansao.addElemento(idHidreletrica_expansao, usina_com_expansao);
						lista_hidreletrica_com_modificacao.addElemento(idHidreletrica_expansao, usina_com_modificacao);
						lista_hidreletrica_sem_capacidade.addElemento(idHidreletrica_expansao, usina_sem_capacidade);

					}//else if (idHidreletrica == IdHidreletrica_Nenhum && usina_com_expansao && considerar_usina) { 

					else if ((idHidreletrica == IdHidreletrica_Nenhum) && (considerar_usina) && (hidreletrica_agregada > 0)) {

						for (int pos = 1; pos <= lista_codigo_ONS_hidreletricas_desagregadas.at(hidreletrica_agregada).size(); pos++)
							lista_hidreletrica_com_modificacao.setElemento(getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, lista_codigo_ONS_hidreletricas_desagregadas.at(hidreletrica_agregada).at(pos)), usina_com_modificacao);

					}

				}//if (hidreletricasPreConfig_instanciadas) {

				else if (!hidreletricasPreConfig_instanciadas) {

					// Somente as usinas a serem consideradas no estudo serão instanciadas.
					if (considerar_usina) {

						//Identifica se alguma hidrelétrica tem sido inicializada com codigo_usina
						const IdHidreletrica idHidreletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

						if (idHidreletrica_inicializado == IdHidreletrica_Nenhum) { //Hidrelétrica não inicializada

							IdHidreletrica idHidreletrica_bacia = IdHidreletrica_Nenhum;
							for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica++) {
								if (lista_codigo_ONS_REE.at(idHidreletrica) == codigo_REE_CEPEL){
									idHidreletrica_bacia = idHidreletrica;
									break;
								} // if (getIdFromCodigoONS(lista_codigo_ONS_REE, codigo_REE_CEPEL) == idHidreletrica) {
							} // for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica++) {


							const IdHidreletrica idHidreletrica = IdHidreletrica(a_dados.getMaiorId(IdHidreletrica()) + 1);

							if (true) {
								Hidreletrica hidreletrica;
								hidreletrica.setAtributo(AttComumHidreletrica_idHidreletrica, idHidreletrica);
								a_dados.vetorHidreletrica.add(hidreletrica);

							} // if (true) {				

							IdBaciaHidrografica idBaciaHidrografica = IdBaciaHidrografica_Nenhum;

							if (idHidreletrica_bacia == IdHidreletrica_Nenhum) {

								idBaciaHidrografica = IdBaciaHidrografica(a_dados.getMaiorId(IdBaciaHidrografica()) + 1);
								BaciaHidrografica bacia;
								bacia.setAtributo(AttComumBaciaHidrografica_idBaciaHidrografica, idBaciaHidrografica);
								bacia.setAtributo(AttComumBaciaHidrografica_nome, std::string("REE_" + getString(codigo_REE_CEPEL)));
								a_dados.vetorBaciaHidrografica.add(bacia);

							} // if (idHidreletrica_bacia == IdHidreletrica_Nenhum) {

							else
								idBaciaHidrografica = a_dados.getAtributo(idHidreletrica_bacia, AttComumHidreletrica_bacia, IdBaciaHidrografica());

							a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_bacia, idBaciaHidrografica);

							a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_codigo_usina, codigo_usina);

							lista_hidreletrica_pre_configuracao.addElemento(idHidreletrica, false);

							lista_codigo_ONS_REE.addElemento(idHidreletrica, codigo_REE_CEPEL);

							lista_codigo_ONS_hidreletrica.addElemento(idHidreletrica, codigo_usina);

							if (true) {
								Reservatorio reservatorio;
								reservatorio.setAtributo(AttComumReservatorio_idReservatorio, IdReservatorio_1); //Somente existe um reservatório por usina, portanto, todos tem IdReservatorio_1
								a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.add(reservatorio);
								a_dados.volume_inicial_carregado_from_premissa = true;
								a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_percentual_volume_util_inicial, percentual_volume_inicial);
							} // if (true) {

							a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_nome, nome);
							a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_considerar_usina, true);
							a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_codigo_posto, codigo_posto_CEPEL);

							lista_IdSubmercado_hidreletrica.addElemento(idHidreletrica, IdSubmercado_Nenhum);

							lista_hidreletrica_maiorIdConjuntoHidraulico.addElemento(idHidreletrica, IdConjuntoHidraulico_Nenhum);
							lista_hidreletrica_com_expansao.addElemento(idHidreletrica, usina_com_expansao);
							lista_hidreletrica_com_modificacao.addElemento(idHidreletrica, usina_com_modificacao);
							lista_hidreletrica_sem_capacidade.addElemento(idHidreletrica, usina_sem_capacidade);

						} // if (idHidreletrica_inicializado == IdHidreletrica_Nenhum) { // Hidrelétrica não inicializada

						else if (idHidreletrica_inicializado != IdHidreletrica_Nenhum)
							throw std::invalid_argument("Hidreletrica ja instanciada! Codigo CEPEL " + getString(codigo_usina) + ".");

					} // if (considerar_usina){

				}//else if(!hidreletricasPreConfig_instanciadas) {

			} // while (std::getline(leituraArquivo, line)) {

			leituraArquivo.close();

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Identifica a usina a jusante: 
			// 1. hidreletricasPreConfig_instanciadas true: Identifica a montante e jusante das usinas em expansão
			// 2. hidreletricasPreConfig_instanciadas false: Cria a configuração hidráulica com base na informação do NW
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			if (hidreletricasPreConfig_instanciadas) {

				for (int usina_com_expansao = 0; usina_com_expansao < conteio_usinas_com_expansao_instanciadas; usina_com_expansao++) {

					const IdHidreletrica idHidreletrica = IdHidreletrica(a_dados.getMaiorId(IdHidreletrica()) - usina_com_expansao); //As últimas hidrelétricas instanciadas no modo de preconfig são as usinas em expansão

					const int codigo_usina_original = a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_codigo_usina, int());

					////////////////////////////////////////////////////
					//Set a usina jusante da usina em expansão
					////////////////////////////////////////////////////

					int pos_jusante = 0;

					for (int indice_original = 1; indice_original <= lista_codigo_ONS_hidreletrica_original.getIteradorFinal(); indice_original++) {

						if (lista_codigo_ONS_hidreletrica_original.getElemento(indice_original) == codigo_usina_original) {
							pos_jusante = indice_original;
							break;
						}//if (lista_codigo_ONS_hidreletrica_original.getElemento(indice_original) == codigo_usina_original) {

					}//for (int indice_original = 1; indice_original <= lista_codigo_ONS_hidreletrica_original.getIteradorFinal(); indice_original++) {

					int indice_jusante_original = pos_jusante;
					IdHidreletrica idHidreletrica_jusante = IdHidreletrica_Nenhum;

					while (true) {

						int codigo_usina_jusante_original = lista_codigo_ONS_hidreletrica_jusante_original.at(indice_jusante_original);

						for (int i = 1; i <= int(lista_codigo_ONS_hidreletrica_agregada.size()); i++) {

							if (lista_codigo_ONS_hidreletrica_agregada.getElemento(i) == codigo_usina_jusante_original) {
								codigo_usina_jusante_original = lista_codigo_ONS_hidreletricas_desagregadas.getElemento(i).getElemento(1);
								break;
							}//if (lista_codigo_ONS_hidreletrica_agregada.getElemento(pos) == codigo_usina) {

						} //for (int i = 1; i <= int(lista_codigo_ONS_hidreletrica_agregada.size()); i++) {

						if (codigo_usina_jusante_original == 0)
							break;

						idHidreletrica_jusante = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina_jusante_original);

						if (idHidreletrica_jusante != IdHidreletrica_Nenhum)
							break;

						indice_jusante_original = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica_original, codigo_usina_jusante_original);

					} // while (true) {

					a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_jusante, idHidreletrica_jusante);

					if (idHidreletrica_jusante != IdHidreletrica_Nenhum)
						a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_bacia, a_dados.getAtributo(idHidreletrica_jusante, AttComumHidreletrica_bacia, IdBaciaHidrografica()));

					////////////////////////////////////////////////////////////////////
					//Set como jusante a usina em expansão na usina existente a montante
					////////////////////////////////////////////////////////////////////

					std::vector<int> lista_codigo_usina_montante;
					std::vector<int> lista_posicao_codigo_usina_montante;

					for (int i = 1; i <= lista_codigo_ONS_hidreletrica_jusante_original.getIteradorFinal(); i++) {

						if (lista_codigo_ONS_hidreletrica_jusante_original.getElemento(i) == codigo_usina_original)
							lista_posicao_codigo_usina_montante.push_back(i);
					} // for (int i = 1; i <= lista_codigo_ONS_hidreletrica_jusante_original.getIteradorFinal(); i++) {

					if (lista_posicao_codigo_usina_montante.size() > 0) {

						for (int i = 1; i <= int(lista_posicao_codigo_usina_montante.size()); i++) {

							lista_codigo_usina_montante.push_back(lista_codigo_ONS_hidreletrica_original.getElemento(lista_posicao_codigo_usina_montante.at(i-1)));

							for (int pos_agregada = 1; pos_agregada <= int(lista_codigo_ONS_hidreletrica_agregada.size()); pos_agregada++) {

								if (lista_codigo_ONS_hidreletrica_agregada.getElemento(pos_agregada) == lista_codigo_usina_montante.at(i - 1)) {
									lista_codigo_usina_montante.at(i - 1) = lista_codigo_ONS_hidreletricas_desagregadas.getElemento(pos_agregada).getElemento(lista_codigo_ONS_hidreletricas_desagregadas.getElemento(pos_agregada).getIteradorFinal());
									break;
								}//if (lista_codigo_ONS_hidreletrica_agregada.getElemento(pos) == codigo_usina) {

							} // for (int pos_agregada = 1; pos_agregada <= int(lista_codigo_ONS_hidreletrica_agregada.size()); pos_agregada++) {

							const IdHidreletrica idHidreletrica_montante = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, lista_codigo_usina_montante.at(i - 1));

							if (idHidreletrica_montante != IdHidreletrica_Nenhum) {
								a_dados.vetorHidreletrica.att(idHidreletrica_montante).setAtributo(AttComumHidreletrica_jusante, idHidreletrica);

								if (a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_bacia, IdBaciaHidrografica()) == IdBaciaHidrografica_Nenhum)
									a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_bacia, a_dados.getAtributo(idHidreletrica_montante, AttComumHidreletrica_bacia, IdBaciaHidrografica()));

							}
							else
								throw std::invalid_argument("Modo carregar hidreletricas preconfig - Hidreletrica a montante nao instanciada! Codigo CEPEL " + getString(lista_codigo_usina_montante.at(i - 1)) + ".");

						} // for (int i = 1; i <= int(lista_posicao_codigo_usina_montante.size()); i++) {

					} // if (lista_posicao_codigo_usina_montante.size() > 0) {

					if (a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_bacia, IdBaciaHidrografica()) == IdBaciaHidrografica_Nenhum){

						const IdBaciaHidrografica idBaciaHidrografica = IdBaciaHidrografica(a_dados.getMaiorId(IdBaciaHidrografica()) + 1);
						BaciaHidrografica bacia;
						bacia.setAtributo(AttComumBaciaHidrografica_idBaciaHidrografica, idBaciaHidrografica);
						bacia.setAtributo(AttComumBaciaHidrografica_nome, a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_nome, std::string()));
						a_dados.vetorBaciaHidrografica.add(bacia);

						a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_bacia, idBaciaHidrografica);

					}

				}//for (int usina_com_expansao = 0; usina_com_expansao < conteio_usinas_com_expansao_instanciadas; usina_com_expansao++) {

			}//if (hidreletricasPreConfig_instanciadas) {

			else if (!hidreletricasPreConfig_instanciadas) {

				for (int indice_original = 1; indice_original <= lista_codigo_ONS_hidreletrica_original.getIteradorFinal(); indice_original++) {

					const int codigo_usina_original = lista_codigo_ONS_hidreletrica_original.at(indice_original);

					const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina_original);

					if (idHidreletrica != IdHidreletrica_Nenhum) {

						// Identifica Hidreletrica jusante instanciada

						int indice_jusante_original = indice_original;

						int codigo_usina_jusante_original = 0;

						IdHidreletrica idHidreletrica_jusante = IdHidreletrica_Nenhum;

						while (true) {

							codigo_usina_jusante_original = lista_codigo_ONS_hidreletrica_jusante_original.at(indice_jusante_original);

							if (codigo_usina_jusante_original == 0)
								break;

							idHidreletrica_jusante = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina_jusante_original);

							if (idHidreletrica_jusante != IdHidreletrica_Nenhum)
								break;

							indice_jusante_original = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica_original, codigo_usina_jusante_original);

						} // while (true) {

						//No NEWAVE está especificado somente usina a jusante, portanto, é considerado que está a jusante do turbinamento, vertimento e desvio
						a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_jusante, idHidreletrica_jusante);
						//a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_jusante_desvio,     idHidreletrica_jusante);

					} // if (idHidreletrica != IdHidreletrica_Nenhum) {

				} // for (int indice_original = 1; indice_original <= lista_codigo_ONS_hidreletrica_original.getIteradorFinal(); indice_original++) {

			}//else if (!hidreletricasPreConfig_instanciadas) {

		}//if (leituraArquivo.is_open()) {

		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_CONFHD_201908_NW25: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_VAZOES_201908_NW25(Dados &a_dados, std::string nomeArquivo)
{

	try {

		//ofstream     fp_out;

		//std::string arquivo = "testeVazoes.csv";
		//fp_out.open(arquivo.c_str(), ios_base::out); //Função para criar um arquivo

		int intLeitura_320[320];
		int intLeitura_600[600];

		///////////////////////////////////////////////////////////////////
		std::ifstream leituraArquivo;
		leituraArquivo.open(nomeArquivo, std::ios_base::in | std::ios::binary);

		int mes = 1;
		int ano = 1931; //Primeiro ano do histórico

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const IdHidreletrica maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

		if (leituraArquivo.is_open()) {

			while (!(leituraArquivo.eof())) {

				Periodo periodo(mes, ano);

				if (periodo >= horizonte_estudo_DECK.getIteradorInicial())
					break;

				mes++;

				if (mes > 12) {
					mes = 1;
					ano++;
				}//if (mes > 12) {

				if (tamanho_registro_arquivo_vazoes_historicas == 320)
					leituraArquivo.read((char*)intLeitura_320, sizeof(intLeitura_320));
				else if (tamanho_registro_arquivo_vazoes_historicas == 600)
					leituraArquivo.read((char*)intLeitura_600, sizeof(intLeitura_600));
				else
					throw std::invalid_argument("tamanho do registro de vazoes ivalido " + getFullString(tamanho_registro_arquivo_vazoes_historicas) + ".");

				for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

					if (!lista_hidreletrica_pre_configuracao.getElemento(idHidreletrica)) {

						const int codigo_usina_posto = a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_codigo_posto, int());

						double afluencia_natural_historico = 0.0;

						if (tamanho_registro_arquivo_vazoes_historicas == 320)
							afluencia_natural_historico = double(intLeitura_320[codigo_usina_posto - 1]);
						else if (tamanho_registro_arquivo_vazoes_historicas == 600)
							afluencia_natural_historico = double(intLeitura_600[codigo_usina_posto - 1]);

						if (!a_dados.vetorHidreletrica.att(idHidreletrica).vetorAfluencia.isInstanciado(IdAfluencia_vazao_afluente)) {

							Afluencia afluencia;
							afluencia.setAtributo(AttComumAfluencia_idAfluencia, IdAfluencia_vazao_afluente);
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorAfluencia.add(afluencia);

						} // if (!a_dados.vetorHidreletrica.att(idHidreletrica).vetorAfluencia.isInstanciado(IdAfluencia_vazao_afluente)){

						a_dados.vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).addElemento(AttVetorAfluencia_natural_historico, periodo, afluencia_natural_historico);

					} // if (!lista_hidreletrica_pre_configuracao.at(idHidreletrica)) {

				}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= idHidreletrica_MaiorId; idHidreletrica++) {

			}//while (!(leituraArquivo.eof())) {

			//fp_out.close();

			leituraArquivo.clear();
			leituraArquivo.close();

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

		a_dados.calcular_historico_afluencia_incremental_com_natural();

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_VAZOES_201908_NW25: \n" + std::string(erro.what())); }


}



void LeituraCEPEL::leitura_MODIF_201908_NW25(Dados &a_dados, std::string nomeArquivo) {

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;

		////////////////////////////////////////////////////////

		if (leituraArquivo.is_open()) {

			//Pula o cabeçalho
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);

			std::vector<IdHidreletrica> lista_idHidreletrica;

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Campo 1 -  USINA ou Palavra-chave que identifica o conteúdo do cadastro a ser modificado
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				atributo = line.substr(1, 8);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				int codigo_usina = 0;

				if (atributo == "USINA") {

					atributo = line.substr(10, 20);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					codigo_usina = atoi(atributo.c_str());

					//Identifica se alguma hidrelétrica tem sido inicializada com codigo_usina

					lista_idHidreletrica = std::vector<IdHidreletrica>{ getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina) };

					if (lista_idHidreletrica.at(0) == IdHidreletrica_161)
						int aa = 1;

					if (lista_idHidreletrica.at(0) == IdHidreletrica_Nenhum) {

						for (int i = 1; i <= lista_codigo_ONS_hidreletrica_agregada.size(); i++) {
							if (lista_codigo_ONS_hidreletrica_agregada.getElemento(i) == codigo_usina) {

								lista_idHidreletrica = std::vector<IdHidreletrica>();

								for (int j = 1; j <= lista_codigo_ONS_hidreletricas_desagregadas.at(i).getIteradorFinal(); j++)
									lista_idHidreletrica.push_back(getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, lista_codigo_ONS_hidreletricas_desagregadas.at(i).at(j)));

								break;

							}
						}

						if (lista_idHidreletrica.at(0) == IdHidreletrica_Nenhum)
							lista_idHidreletrica = std::vector<IdHidreletrica>();

					} // if (idHidreletrica == IdHidreletrica_Nenhum){



				}//if (atributo == "USINA") {

				else {

					if (lista_idHidreletrica.size() > 0) {

						for (int i = 0; i < int(lista_idHidreletrica.size()); i++) {

							if (lista_hidreletrica_com_modificacao.getElemento(lista_idHidreletrica.at(i))) {

								// Inicializa adicao de modificacao na hidreletrica
								if (lista_modificacaoUHE.size() - 1 < lista_idHidreletrica.at(i)) {

									IdHidreletrica menorIdHidreletrica = IdHidreletrica_1;

									if (lista_modificacaoUHE.size() > 2)
										menorIdHidreletrica = IdHidreletrica(lista_modificacaoUHE.size() - 1);

									for (IdHidreletrica idHidreletrica_aux = menorIdHidreletrica; idHidreletrica_aux <= lista_idHidreletrica.at(i); idHidreletrica_aux++)
										lista_modificacaoUHE.push_back(std::vector< ModificacaoUHE>());

								} // if (lista_modificacaoUHE.size() - 1 < lista_idHidreletrica.at(i)) {

							}

						}


						ModificacaoUHE modificacaoUHE;

						if ((atributo == "VOLMIN" || atributo == "volmin") && (lista_hidreletrica_com_modificacao.getElemento(lista_idHidreletrica.at(0)))) {

							if (lista_idHidreletrica.size() > 1)
								throw std::invalid_argument("Esta representacao deve ser reavaliada em termos das hidreletricas desagregadas da usina codigo " + getString(codigo_usina));

							line = line.substr(9, line.length());//Substrai da linha a parte do mnemônico
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios
							strNormalizada(line);

							//Novo valor

							const int pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double valor_1 = atof(atributo.c_str());

							//Unidade
							atributo = line.substr(pos, line.length());
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							TipoGrandezaModificacao tipo_de_grandeza;

							if (atributo == "'%'")
								tipo_de_grandeza = TipoGrandezaModificacao_percentual;
							else
								tipo_de_grandeza = TipoGrandezaModificacao_absoluta;

							//Set atributos modificação
							modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_VOLMIN;
							modificacaoUHE.valor_1 = valor_1;
							modificacaoUHE.tipo_de_grandeza = tipo_de_grandeza;

							//Set modificação da hidrelétrica
							lista_modificacaoUHE.at(lista_idHidreletrica.at(0)).push_back(modificacaoUHE);

						}//if (atributo == "VOLMIN" || atributo == "volmin") {

						if ((atributo == "VOLMAX" || atributo == "volmax") && (lista_hidreletrica_com_modificacao.getElemento(lista_idHidreletrica.at(0)))) {

							if (lista_idHidreletrica.size() > 1)
								throw std::invalid_argument("Esta representacao deve ser reavaliada em termos das hidreletricas desagregadas da usina codigo " + getString(codigo_usina));

							line = line.substr(9, line.length());//Substrai da linha a parte do mnemônico
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios
							strNormalizada(line);

							const int pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							double valor_1 = atof(atributo.c_str());

							//Unidade
							atributo = line.substr(pos, line.length());
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							TipoGrandezaModificacao tipo_de_grandeza;

							if (atributo == "'%'") {

								tipo_de_grandeza = TipoGrandezaModificacao_percentual;

								valor_1 *= 0.01;

							}//if (atributo == "'%'") {
							else
								tipo_de_grandeza = TipoGrandezaModificacao_absoluta;

							//Set atributos modificação
							modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_VOLMAX;
							modificacaoUHE.valor_1 = valor_1;
							modificacaoUHE.tipo_de_grandeza = tipo_de_grandeza;

							//Set modificação da hidrelétrica
							lista_modificacaoUHE.at(lista_idHidreletrica.at(0)).push_back(modificacaoUHE);

						}//if (atributo == "VOLMAX" || atributo == "volmax") {

						if ((atributo == "NUMCNJ" || atributo == "numcnj") && (lista_hidreletrica_com_modificacao.getElemento(lista_idHidreletrica.at(0)))) {

							if (lista_idHidreletrica.size() > 1)
								throw std::invalid_argument("Esta representacao deve ser reavaliada em termos das hidreletricas desagregadas da usina codigo " + getString(codigo_usina));

							line = line.substr(9, line.length());//Substrai da linha a parte do mnemônico
							strNormalizada(line);

							//Novo valor

							atributo = line.substr(0, line.length());
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							//Set atributos modificação
							modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_NUMCNJ;
							modificacaoUHE.valor_1 = atof(atributo.c_str());
							modificacaoUHE.tipo_de_grandeza = TipoGrandezaModificacao_absoluta;

							modificacaoUHE.periodo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio()).getIteradorInicial();//Para estar em concordancia do DC (no qual pode ser dada uma modificaçao a partir de um periodo específico)

							//Set modificação da hidrelétrica
							lista_modificacaoUHE.at(lista_idHidreletrica.at(0)).push_back(modificacaoUHE);

						}//if (atributo == "NUMCNJ" || atributo == "numcnj") {

						if ((atributo == "NUMMAQ" || atributo == "nummaq") && (lista_hidreletrica_com_modificacao.getElemento(lista_idHidreletrica.at(0)))) {

							if (lista_idHidreletrica.size() > 1)
								throw std::invalid_argument("Esta representacao deve ser reavaliada em termos das hidreletricas desagregadas da usina codigo " + getString(codigo_usina));

							line = line.substr(9, line.length());//Substrai da linha a parte do mnemônico
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios
							strNormalizada(line);

							//Número de máquinas 

							const int pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double valor_1 = atof(atributo.c_str());

							//Número do conjunto 
							atributo = line.substr(pos, line.length());
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int valor_2 = atoi(atributo.c_str());

							//Set atributos modificação
							modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_NUMMAQ;
							modificacaoUHE.valor_1 = valor_1;
							modificacaoUHE.valor_2 = valor_2;
							modificacaoUHE.tipo_de_grandeza = TipoGrandezaModificacao_absoluta;

							modificacaoUHE.periodo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio()).getIteradorInicial();//Para estar em concordancia do DC (no qual pode ser dada uma modificaçao a partir de um periodo específico)

							//Set modificação da hidrelétrica
							lista_modificacaoUHE.at(lista_idHidreletrica.at(0)).push_back(modificacaoUHE);

						}//if (atributo == "NUMMAQ" || atributo == "nummaq") {

						if ((atributo == "POTEFE" || atributo == "potefe") && (lista_hidreletrica_com_modificacao.getElemento(lista_idHidreletrica.at(0)))) {

							if (lista_idHidreletrica.size() > 1)
								throw std::invalid_argument("Esta representacao deve ser reavaliada em termos das hidreletricas desagregadas da usina codigo " + getString(codigo_usina));

							line = line.substr(9, line.length());//Substrai da linha a parte do mnemônico
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios
							strNormalizada(line);

							//Valor da potência efetiva (MW) 

							const int pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double valor_1 = atof(atributo.c_str());

							//Número do conjunto 
							atributo = line.substr(pos, line.length());
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double valor_2 = atof(atributo.c_str());

							//Set atributos modificação
							modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_POTEFE;
							modificacaoUHE.valor_1 = valor_1;
							modificacaoUHE.valor_2 = valor_2;
							modificacaoUHE.tipo_de_grandeza = TipoGrandezaModificacao_absoluta;

							//Set modificação da hidrelétrica
							lista_modificacaoUHE.at(lista_idHidreletrica.at(0)).push_back(modificacaoUHE);

						}//if (atributo == "POTEFE" || atributo == "potefe") {

						if ((atributo == "PRODESP" || atributo == "prodesp") && (lista_hidreletrica_com_modificacao.getElemento(lista_idHidreletrica.at(0)))) {

							if (lista_idHidreletrica.size() > 1)
								throw std::invalid_argument("Esta representacao deve ser reavaliada em termos das hidreletricas desagregadas da usina codigo " + getString(codigo_usina));

							line = line.substr(9, line.length());//Substrai da linha a parte do mnemônico
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios
							strNormalizada(line);

							//Produtibilidade específica

							atributo = line.substr(0, line.length());
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double valor_1 = atof(atributo.c_str());

							//Set atributos modificação
							modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_PRODESP;
							modificacaoUHE.valor_1 = valor_1;
							modificacaoUHE.tipo_de_grandeza = TipoGrandezaModificacao_absoluta;

							//Set modificação da hidrelétrica
							lista_modificacaoUHE.at(lista_idHidreletrica.at(0)).push_back(modificacaoUHE);

						}//if (atributo == "PRODESP" || atributo == "prodesp") {

						if ((atributo == "TEIF" || atributo == "teif") && (lista_hidreletrica_com_modificacao.getElemento(lista_idHidreletrica.at(0)))) {

							if (lista_idHidreletrica.size() > 1)
								throw std::invalid_argument("Esta representacao deve ser reavaliada em termos das hidreletricas desagregadas da usina codigo " + getString(codigo_usina));

							line = line.substr(9, line.length());//Substrai da linha a parte do mnemônico
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios
							strNormalizada(line);

							//Taxa esperada de indisponibilidade forçada (%) 

							atributo = line.substr(0, line.length());
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double valor_1 = atof(atributo.c_str());

							//Set atributos modificação
							modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_TEIF;
							modificacaoUHE.valor_1 = valor_1;
							modificacaoUHE.tipo_de_grandeza = TipoGrandezaModificacao_absoluta;

							//Set modificação da hidrelétrica
							lista_modificacaoUHE.at(lista_idHidreletrica.at(0)).push_back(modificacaoUHE);



						}//if (atributo == "TEIF" || atributo == "teif") {

						if ((atributo == "IP" || atributo == "ip") && (lista_hidreletrica_com_modificacao.getElemento(lista_idHidreletrica.at(0)))) {

							if (lista_idHidreletrica.size() > 1)
								throw std::invalid_argument("Esta representacao deve ser reavaliada em termos das hidreletricas desagregadas da usina codigo " + getString(codigo_usina));

							line = line.substr(9, line.length());//Substrai da linha a parte do mnemônico
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios
							strNormalizada(line);

							//Indisponibilidade programada (%) 

							atributo = line.substr(0, line.length());
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double valor_1 = atof(atributo.c_str());

							//Set atributos modificação
							modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_IP;
							modificacaoUHE.valor_1 = valor_1;
							modificacaoUHE.tipo_de_grandeza = TipoGrandezaModificacao_absoluta;

							//Set modificação da hidrelétrica
							lista_modificacaoUHE.at(lista_idHidreletrica.at(0)).push_back(modificacaoUHE);

						}//if (atributo == "IP" || atributo == "ip") {

						if ((atributo == "PERDHIDR" || atributo == "perdhidr") && (lista_hidreletrica_com_modificacao.getElemento(lista_idHidreletrica.at(0)))) {

							if (lista_idHidreletrica.size() > 1)
								throw std::invalid_argument("Esta representacao deve ser reavaliada em termos das hidreletricas desagregadas da usina codigo " + getString(codigo_usina));

							line = line.substr(9, line.length());//Substrai da linha a parte do mnemônico
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios
							strNormalizada(line);

							//Indisponibilidade programada (%) 

							atributo = line.substr(0, line.length());
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double valor_1 = atof(atributo.c_str());

							//Set atributos modificação
							modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_PERDHIDR;
							modificacaoUHE.valor_1 = valor_1;
							modificacaoUHE.tipo_de_grandeza = TipoGrandezaModificacao_absoluta;

							//Set modificação da hidrelétrica
							lista_modificacaoUHE.at(lista_idHidreletrica.at(0)).push_back(modificacaoUHE);

						}//if (atributo == "PERDHIDR" || atributo == "perdhidr") {

						if ((atributo == "CMONT" || atributo == "cmont") && (lista_hidreletrica_com_modificacao.getElemento(lista_idHidreletrica.at(0)))) {

							if (lista_idHidreletrica.size() > 1)
								throw std::invalid_argument("Esta representacao deve ser reavaliada em termos das hidreletricas desagregadas da usina codigo " + getString(codigo_usina));

							line = line.substr(9, line.length());//Substrai da linha a parte do mnemônico
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios
							strNormalizada(line);


							//Mês

							int pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int mes = atoi(atributo.c_str());

							line = line.substr(pos + 1, line.length());//Substrai da linha o último valor registrado
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor

							//Ano

							pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());


							const int ano = atoi(atributo.c_str());

							line = line.substr(pos + 1, line.length());//Substrai da linha o último valor registrado
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor


							atributo = line.substr(0, line.length());
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double valor_1 = atof(atributo.c_str());

							//Set atributos modificação
							modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_CMONT;
							modificacaoUHE.valor_1 = valor_1;
							modificacaoUHE.periodo = Periodo(mes, ano);
							modificacaoUHE.tipo_de_grandeza = TipoGrandezaModificacao_absoluta;

							//Set modificação da hidrelétrica
							lista_modificacaoUHE.at(lista_idHidreletrica.at(0)).push_back(modificacaoUHE);

						}// if (atributo == "CMONT" || atributo == "cmont") {

						if ((atributo == "COEFEVAP" || atributo == "coefevap") && (lista_hidreletrica_com_modificacao.getElemento(lista_idHidreletrica.at(0)))) {

							if (lista_idHidreletrica.size() > 1)
								throw std::invalid_argument("Esta representacao deve ser reavaliada em termos das hidreletricas desagregadas da usina codigo " + getString(codigo_usina));

							line = line.substr(9, line.length());//Substrai da linha a parte do mnemônico
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios
							strNormalizada(line);

							//Número de máquinas 

							const int pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double valor_1 = atof(atributo.c_str());

							//Número do conjunto 
							atributo = line.substr(pos, line.length());
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int valor_2 = atoi(atributo.c_str());

							//Set atributos modificação
							modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_COEFEVAP;
							modificacaoUHE.valor_1 = valor_1;
							modificacaoUHE.valor_2 = valor_2;
							modificacaoUHE.tipo_de_grandeza = TipoGrandezaModificacao_absoluta;

							//Set modificação da hidrelétrica
							lista_modificacaoUHE.at(lista_idHidreletrica.at(0)).push_back(modificacaoUHE);

						}//if (atributo == "COEFEVAP" || atributo == "coefevap") {

						if ((atributo == "COTAREA" || atributo == "cotarea") && (lista_hidreletrica_com_modificacao.getElemento(lista_idHidreletrica.at(0)))) {

							if (lista_idHidreletrica.size() > 1)
								throw std::invalid_argument("Esta representacao deve ser reavaliada em termos das hidreletricas desagregadas da usina codigo " + getString(codigo_usina));

							line = line.substr(9, line.length());//Substrai da linha a parte do mnemônico
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor
							strNormalizada(line);

							//Novos valores (do coef de ordem 0 até 4)

							//Coef 0

							int pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double valor_1 = atof(atributo.c_str());

							line = line.substr(pos + 1, line.length());//Substrai da linha o último valor registrado
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor

							//Coef 1

							pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double valor_2 = atof(atributo.c_str());

							line = line.substr(pos + 1, line.length());//Substrai da linha o último valor registrado
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor

							//Coef 2

							pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double valor_3 = atof(atributo.c_str());

							line = line.substr(pos + 1, line.length());//Substrai da linha o último valor registrado
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor

							//Coef 3

							pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double valor_4 = atof(atributo.c_str());

							line = line.substr(pos + 1, line.length());//Substrai da linha o último valor registrado
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor

							//Coef 4

							pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double valor_5 = atof(atributo.c_str());

							/////////////////////

							//Set atributos modificação
							modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_COTAREA;
							modificacaoUHE.valor_1 = valor_1;
							modificacaoUHE.valor_2 = valor_2;
							modificacaoUHE.valor_3 = valor_3;
							modificacaoUHE.valor_4 = valor_4;
							modificacaoUHE.valor_5 = valor_5;
							modificacaoUHE.tipo_de_grandeza = TipoGrandezaModificacao_absoluta;

							//Set modificação da hidrelétrica
							lista_modificacaoUHE.at(lista_idHidreletrica.at(0)).push_back(modificacaoUHE);

						}//if (atributo == "COTAREA" || atributo == "cotarea") {

						if ((atributo == "VOLCOTA" || atributo == "volcota") && (lista_hidreletrica_com_modificacao.getElemento(lista_idHidreletrica.at(0)))) {

							if (lista_idHidreletrica.size() > 1)
								throw std::invalid_argument("Esta representacao deve ser reavaliada em termos das hidreletricas desagregadas da usina codigo " + getString(codigo_usina));

							line = line.substr(9, line.length());//Substrai da linha a parte do mnemônico
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor
							strNormalizada(line);

							//Novos valores (do coef de ordem 0 até 4)

							//Coef 0

							int pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double valor_1 = atof(atributo.c_str());

							line = line.substr(pos + 1, line.length());//Substrai da linha o último valor registrado
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor

							//Coef 1

							pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());


							const double valor_2 = atof(atributo.c_str());

							line = line.substr(pos + 1, line.length());//Substrai da linha o último valor registrado
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor

							//Coef 2

							pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());


							const double valor_3 = atof(atributo.c_str());

							line = line.substr(pos + 1, line.length());//Substrai da linha o último valor registrado
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor

							//Coef 3

							pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());


							const double valor_4 = atof(atributo.c_str());

							line = line.substr(pos + 1, line.length());//Substrai da linha o último valor registrado
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor

							//Coef 4

							pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());


							const double valor_5 = atof(atributo.c_str());

							/////////////////////

							TipoGrandezaModificacao tipo_de_grandeza = TipoGrandezaModificacao_absoluta;

							//Set atributos modificação
							modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_VOLCOTA;
							modificacaoUHE.valor_1 = valor_1;
							modificacaoUHE.valor_2 = valor_2;
							modificacaoUHE.valor_3 = valor_3;
							modificacaoUHE.valor_4 = valor_4;
							modificacaoUHE.valor_5 = valor_5;
							modificacaoUHE.tipo_de_grandeza = tipo_de_grandeza;

							//Set modificação da hidrelétrica
							lista_modificacaoUHE.at(lista_idHidreletrica.at(0)).push_back(modificacaoUHE);

						}//if (atributo == "VOLCOTA" || atributo == "volcota") {

						if ((atributo == "CFUGA" || atributo == "cfuga") && (lista_hidreletrica_com_modificacao.getElemento(lista_idHidreletrica.at(0)))) {

							if (lista_idHidreletrica.size() > 1)
								throw std::invalid_argument("Esta representacao deve ser reavaliada em termos das hidreletricas desagregadas da usina codigo " + getString(codigo_usina));

							line = line.substr(9, line.length());//Substrai da linha a parte do mnemônico
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor
							strNormalizada(line);

							//Novos valores (do coef de ordem 0 até 4)

							//Mês

							int pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());


							const int mes = atoi(atributo.c_str());

							line = line.substr(pos + 1, line.length());//Substrai da linha o último valor registrado
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor

							//Ano

							pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());


							const int ano = atoi(atributo.c_str());

							line = line.substr(pos + 1, line.length());//Substrai da linha o último valor registrado
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor

							//Valor

							pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());


							double valor_1 = atof(atributo.c_str());

							/////////////////////

							const Periodo periodo(mes, ano);

							//Set atributos modificação
							modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_CFUGA;
							modificacaoUHE.valor_1 = valor_1;
							modificacaoUHE.periodo = periodo;
							modificacaoUHE.tipo_de_grandeza = TipoGrandezaModificacao_absoluta;

							//Set modificação da hidrelétrica
							lista_modificacaoUHE.at(lista_idHidreletrica.at(0)).push_back(modificacaoUHE);

						}//if (atributo == "CFUGA" || atributo == "cfuga") {

						if ((atributo == "VMAXT" || atributo == "vmaxt") && (lista_hidreletrica_com_modificacao.getElemento(lista_idHidreletrica.at(0)))) {

							if (lista_idHidreletrica.size() > 1)
								throw std::invalid_argument("Esta representacao deve ser reavaliada em termos das hidreletricas desagregadas da usina codigo " + getString(codigo_usina));

							line = line.substr(9, line.length());//Substrai da linha a parte do mnemônico
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor
							strNormalizada(line);

							//Novos valores (do coef de ordem 0 até 4)

							//Mês

							int pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());


							const int mes = atoi(atributo.c_str());

							line = line.substr(pos + 1, line.length());//Substrai da linha o último valor registrado
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor

							//Ano

							pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());


							const int ano = atoi(atributo.c_str());

							line = line.substr(pos + 1, line.length());//Substrai da linha o último valor registrado
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor

							//Valor

							pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());


							double valor_1 = atof(atributo.c_str());

							line = line.substr(pos + 1, line.length());//Substrai da linha o último valor registrado
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor

							//Unidade

							pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());


							TipoGrandezaModificacao tipo_de_grandeza;

							if (atributo == "'%'") {

								tipo_de_grandeza = TipoGrandezaModificacao_percentual;

								valor_1 *= 0.01;

							}//if (atributo == "'%'") {
							else
								tipo_de_grandeza = TipoGrandezaModificacao_absoluta;

							/////////////////////

							const Periodo periodo(mes, ano);

							//Set atributos modificação
							modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_VMAXT;
							modificacaoUHE.valor_1 = valor_1;
							modificacaoUHE.periodo = periodo;
							modificacaoUHE.tipo_de_grandeza = tipo_de_grandeza;

							//Set modificação da hidrelétrica
							lista_modificacaoUHE.at(lista_idHidreletrica.at(0)).push_back(modificacaoUHE);

						}//if (atributo == "VMAXT" || atributo == "vmaxt") {

						if ((atributo == "VMINT" || atributo == "vmint") && (lista_hidreletrica_com_modificacao.getElemento(lista_idHidreletrica.at(0)))) {

							if (lista_idHidreletrica.size() > 1)
								throw std::invalid_argument("Esta representacao deve ser reavaliada em termos das hidreletricas desagregadas da usina codigo " + getString(codigo_usina));

							line = line.substr(9, line.length());//Substrai da linha a parte do mnemônico
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor
							strNormalizada(line);

							//Novos valores (do coef de ordem 0 até 4)

							//Mês

							int pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());


							const int mes = atoi(atributo.c_str());

							line = line.substr(pos + 1, line.length());//Substrai da linha o último valor registrado
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor

							//Ano

							pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());


							const int ano = atoi(atributo.c_str());

							line = line.substr(pos + 1, line.length());//Substrai da linha o último valor registrado
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor

							//Valor

							pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());


							double valor_1 = atof(atributo.c_str());

							line = line.substr(pos + 1, line.length());//Substrai da linha o último valor registrado
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor

							//Unidade

							pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());


							TipoGrandezaModificacao tipo_de_grandeza;

							if (atributo == "'%'") {

								tipo_de_grandeza = TipoGrandezaModificacao_percentual;

								valor_1 *= 0.01;

							}//if (atributo == "'%'") {
							else
								tipo_de_grandeza = TipoGrandezaModificacao_absoluta;

							/////////////////////

							const Periodo periodo(mes, ano);

							//Set atributos modificação
							modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_VMINT;
							modificacaoUHE.valor_1 = valor_1;
							modificacaoUHE.periodo = periodo;
							modificacaoUHE.tipo_de_grandeza = tipo_de_grandeza;

							//Set modificação da hidrelétrica
							lista_modificacaoUHE.at(lista_idHidreletrica.at(0)).push_back(modificacaoUHE);

						}//if (atributo == "VMINT" || atributo == "vmint") {

						if ((atributo == "NUMBAS" || atributo == "numbas") && (lista_hidreletrica_com_modificacao.getElemento(lista_idHidreletrica.at(0)))) {

							if (lista_idHidreletrica.size() > 1)
								throw std::invalid_argument("Esta representacao deve ser reavaliada em termos das hidreletricas desagregadas da usina codigo " + getString(codigo_usina));

							line = line.substr(9, line.length());//Substrai da linha a parte do mnemônico
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios
							strNormalizada(line);

							//Número de unidades de base

							atributo = line.substr(0, line.length());
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double valor_1 = atof(atributo.c_str());

							//Set atributos modificação
							modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_NUMBAS;
							modificacaoUHE.valor_1 = valor_1;
							modificacaoUHE.tipo_de_grandeza = TipoGrandezaModificacao_absoluta;

							//Set modificação da hidrelétrica
							lista_modificacaoUHE.at(lista_idHidreletrica.at(0)).push_back(modificacaoUHE);

						}//if (atributo == "NUMBAS" || atributo == "numbas") {

						if ((atributo == "VMINP" || atributo == "vminp") && (lista_hidreletrica_com_modificacao.getElemento(lista_idHidreletrica.at(0)))) {

							if (lista_idHidreletrica.size() > 1)
								throw std::invalid_argument("Esta representacao deve ser reavaliada em termos das hidreletricas desagregadas da usina codigo " + getString(codigo_usina));

							line = line.substr(9, line.length());//Substrai da linha a parte do mnemônico
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor
							strNormalizada(line);

							//Novos valores (do coef de ordem 0 até 4)

							//Mês

							int pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());


							const int mes = atoi(atributo.c_str());

							line = line.substr(pos + 1, line.length());//Substrai da linha o último valor registrado
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor

							//Ano

							pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());


							const int ano = atoi(atributo.c_str());

							line = line.substr(pos + 1, line.length());//Substrai da linha o último valor registrado
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor

							//Valor

							pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());


							double valor_1 = atof(atributo.c_str());

							line = line.substr(pos + 1, line.length());//Substrai da linha o último valor registrado
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor

							//Unidade

							pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							TipoGrandezaModificacao tipo_de_grandeza;

							if (atributo == "'%'") {

								tipo_de_grandeza = TipoGrandezaModificacao_percentual;

								valor_1 *= 0.01;

							}//if (atributo == "'%'") {
							else
								tipo_de_grandeza = TipoGrandezaModificacao_absoluta;

							/////////////////////

							const Periodo periodo(mes, ano);

							//Set atributos modificação
							modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_VMINP;
							modificacaoUHE.valor_1 = valor_1;
							modificacaoUHE.periodo = periodo;
							modificacaoUHE.tipo_de_grandeza = tipo_de_grandeza;

							//Set modificação da hidrelétrica
							lista_modificacaoUHE.at(lista_idHidreletrica.at(0)).push_back(modificacaoUHE);

						}//if (atributo == "VMINP" || atributo == "vminp") {

						if (atributo == "VAZMIN" || atributo == "vazmin") {

							line = line.substr(9, line.length());//Substrai da linha a parte do mnemônico
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios
							strNormalizada(line);

							//Indisponibilidade programada (%) 

							atributo = line.substr(0, line.length());
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double valor_1 = atof(atributo.c_str());

							for (int i = 0; i < int(lista_idHidreletrica.size()); i++) {

								modificacaoUHE = ModificacaoUHE();

								//Set atributos modificação
								modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_VAZMIN;
								modificacaoUHE.valor_1 = valor_1;
								modificacaoUHE.tipo_de_grandeza = TipoGrandezaModificacao_absoluta;

								//Set modificação da hidrelétrica
								lista_modificacaoUHE.at(lista_idHidreletrica.at(i)).push_back(modificacaoUHE);

							}

						}//if (atributo == "VAZMIN" || atributo == "vazmin") {

						if (atributo == "VAZMINT" || atributo == "vazmint") {

							line = line.substr(9, line.length());//Substrai da linha a parte do mnemônico
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor
							strNormalizada(line);

							//Novos valores (do coef de ordem 0 até 4)

							//Mês

							int pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());


							const int mes = atoi(atributo.c_str());

							line = line.substr(pos + 1, line.length());//Substrai da linha o último valor registrado
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor

							//Ano

							pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());


							const int ano = atoi(atributo.c_str());

							line = line.substr(pos + 1, line.length());//Substrai da linha o último valor registrado
							line = line.substr(line.find_first_not_of(" "), line.length());//Substrai da linha os possíveis vazios até o primeiro valor

							//Valor

							pos = int(line.find(" "));

							atributo = line.substr(0, pos);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());


							double valor_1 = atof(atributo.c_str());

							/////////////////////

							const Periodo periodo(mes, ano);

							for (int i = 0; i < int(lista_idHidreletrica.size()); i++) {

								modificacaoUHE = ModificacaoUHE();

								//Set atributos modificação
								modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_VAZMINT;
								modificacaoUHE.valor_1 = valor_1;
								modificacaoUHE.periodo = periodo;
								modificacaoUHE.tipo_de_grandeza = TipoGrandezaModificacao_absoluta;

								//Set modificação da hidrelétrica
								lista_modificacaoUHE.at(lista_idHidreletrica.at(i)).push_back(modificacaoUHE);

							}

						}//if (atributo == "VAZMINT" || atributo == "vazmint") {

					}

				}//else {

			}//while (std::getline(leituraArquivo, line)) {

			leituraArquivo.close();

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_NEWAVE_26_MODIF: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_CONFT_201908_NW25(Dados &a_dados, std::string nomeArquivo) {

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;

		////////////////////////////////////////////////////////

		if (leituraArquivo.is_open()) {

			//Pula o cabeçalho
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);

				
				//Campo 1 -  Número da usina térmica		
				const int codigo_usina = std::stoi(line.substr(1, 4));

				//Campo 2 -  Nome da usina
				const string nome = line.substr(6, 12);

				
				//Campo 3 -  Número do submercado a que pertence a usina
				const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, std::stoi(line.substr(21, 4)));

				if (idSubmercado == IdSubmercado_Nenhum)
					throw std::invalid_argument("Submercado nao instanciado com codigo_usina_" + atributo);

				
				//Campo 4 -  Índice indicador de usina térmica existente
				atributo = line.substr(30, 2);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				bool considerar_usina   = true;
				bool usina_com_expansao = false;

				if ((atributo == "EE") || (atributo == "NE")) { usina_com_expansao = true; }
				else if (atributo == "NC")                    { considerar_usina   = false; }					

				//Campo 5 -  Número da classe térmica da usina / Esta informacao é igual ao id da usina, ver apra que sirve...
				const int classe_CEPEL = std::stoi(line.substr(35, 4));

				//Guarda informação nos SmartElementos	
				if ((considerar_usina) ) {

					//Identifica se alguma termelétrica tem sido inicializada com codigo_usina
					const IdTermeletrica idTermeletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, codigo_usina);

					if (idTermeletrica_inicializado == IdTermeletrica_Nenhum){					

						const IdTermeletrica idTermeletrica = IdTermeletrica(a_dados.getMaiorId(IdTermeletrica()) + 1);

						lista_codigo_ONS_termeletrica.addElemento(idTermeletrica, codigo_usina);
						lista_classe_ONS_termeletrica.addElemento(idTermeletrica, classe_CEPEL);
						lista_termeletrica_com_expansao.addElemento(idTermeletrica, usina_com_expansao);

						Termeletrica termeletrica;
						termeletrica.setAtributo(AttComumTermeletrica_idTermeletrica, idTermeletrica);
						termeletrica.setAtributo(AttComumTermeletrica_nome, nome);
						termeletrica.setAtributo(AttComumTermeletrica_submercado, idSubmercado);
						termeletrica.setAtributo(AttComumTermeletrica_considerar_usina, true);
						termeletrica.setAtributo(AttComumTermeletrica_codigo_usina, codigo_usina);
						termeletrica.setAtributo(AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica_por_usina);
						a_dados.vetorTermeletrica.add(termeletrica);

						UnidadeUTE unidadeUTE;
						unidadeUTE.setAtributo(AttComumUnidadeUTE_nome, nome);
						unidadeUTE.setAtributo(AttComumUnidadeUTE_submercado, idSubmercado);
						a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.add(unidadeUTE);

					} // if (idTermeletrica_inicializado == IdTermeletrica_Nenhum) {//Usina não inicializada

					else if ((idTermeletrica_inicializado != IdTermeletrica_Nenhum) && (termeletricasPreConfig_instanciadas)) {
						a_dados.vetorTermeletrica.att(idTermeletrica_inicializado).setAtributo(AttComumTermeletrica_nome, nome);
						a_dados.vetorTermeletrica.att(idTermeletrica_inicializado).setAtributo(AttComumTermeletrica_submercado, idSubmercado);
						a_dados.vetorTermeletrica.att(idTermeletrica_inicializado).setAtributo(AttComumTermeletrica_considerar_usina, true);

						lista_classe_ONS_termeletrica.setElemento(idTermeletrica_inicializado, classe_CEPEL);
						lista_termeletrica_com_expansao.setElemento(idTermeletrica_inicializado, usina_com_expansao);

						a_dados.vetorTermeletrica.att(idTermeletrica_inicializado).setAtributo(AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica_por_usina);

						UnidadeUTE unidadeUTE;
						unidadeUTE.setAtributo(AttComumUnidadeUTE_nome, nome);
						unidadeUTE.setAtributo(AttComumUnidadeUTE_submercado, idSubmercado);
						a_dados.vetorTermeletrica.att(idTermeletrica_inicializado).vetorUnidadeUTE.add(unidadeUTE);

					}//else if ((idTermeletrica_inicializado != IdTermeletrica_Nenhum) && (termeletricasPreConfig_instanciadas)) {

					else
						throw std::invalid_argument(getFullString(idTermeletrica_inicializado) + " ja instanciado.");

				} // if (considerar_usina){
	

			}//while (std::getline(leituraArquivo, line)) {

			leituraArquivo.close();

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_NEWAVE_26_CONFT: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_TERM_201908_NW25(Dados& a_dados, std::string nomeArquivo) {

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;

		//Horizonte de estudo
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		////////////////////////////////////////////////////////

		if (leituraArquivo.is_open()) {

			//Pula o cabeçalho
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);

				//Campo 1 -  Número da usina térmica
				//Identifica se alguma termelétrica tem sido inicializada com codigo_usina
				const IdTermeletrica idTermeletrica = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, std::stoi(line.substr(1, 4)));

				if (idTermeletrica != IdTermeletrica_Nenhum) {

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Campo 2 -  Nome da usina
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(5, 12);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const string nome = atributo;

					//Campo 3 -  Capacidade instalada (MW)
					double potencia_maxima = std::stod(line.substr(19, 5));

					//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Manual: Se a usina tiver expansão (existente ou não), suas capacidades máxima e mínima de geração, fornecidas  
					//no cadastro de usinas térmicas, recebem automaticamente o valor nulo. Se a usina tiver índice indicador NC, não será 
					//considerada nos cálculos do estudo
					//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					if (lista_termeletrica_com_expansao.getElemento(idTermeletrica))
						potencia_maxima = 0;
	

					//Campo 4 -  Fator de capacidade máximo (%)
					const double fator_de_capacidade = std::stod(line.substr(25, 4)) / 100;

					//Campo 5 -  TEIF da usina térmica (%) (Taxa Equivalente de Indisponibilidade Forçada)
					const double indisponibilidade_forcada = std::stod(line.substr(31, 6)) / 100;

					//Campo 6 -  Indisponibilidade programada (IP) da usina térmica para os demais anos do estudo(%)
					const double indisponibilidade_programada = std::stod(line.substr(38, 6)) / 100;

					//Campo 7-18 -  Geração térmica mínima (MW) - Mês 1 até Mês 12

					const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();
					const Periodo periodo_final   = horizonte_estudo.getIteradorFinal();

					double sobreposicao_atraso_periodo_inicial = horizonte_estudo.getIteradorInicial().atraso(horizonte_estudo_DECK.getIteradorInicial());

					Periodo periodo_deck = horizonte_estudo_DECK.getIteradorInicial();

					for (IdMes idMes = periodo_deck.getMes(); idMes <= IdMes_12; idMes++) {

						for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							double sobreposicao = periodo.sobreposicao(periodo_deck);

							if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_deck == horizonte_estudo_DECK.getIteradorInicial()))
								sobreposicao += sobreposicao_atraso_periodo_inicial;

							if (sobreposicao > 0.0) {

								atributo = line.substr(45 + 7 * (int(idMes) - 1), 6);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const double potencia_minima = atof(atributo.c_str());

								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {
									a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).addElemento(AttMatrizUnidadeUTE_potencia_minima, periodo, idPatamarCarga, sobreposicao * potencia_minima);
									a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).addElemento(AttMatrizUnidadeUTE_potencia_maxima, periodo, idPatamarCarga, sobreposicao * potencia_maxima);
								} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

							} // if (sobreposicao > 0.0) {

						} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
						//Criterio de parada

						periodo_deck++;

					} // for (IdMes idMes = periodo_deck.getMes(); idMes <= IdMes_12; idMes++){

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 19 -  Geração térmica mínima (Mwmês) para os demais anos
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(129, 6);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const double potencia_minima = atof(atributo.c_str());

					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						double sobreposicao = periodo.sobreposicao(periodo_deck);

						if ((sobreposicao > 0.0) || (periodo >= periodo_deck)) {

							if (sobreposicao == 0.0)
								sobreposicao = 1.0;

							if (a_dados.getIterador1Final(idTermeletrica, IdUnidadeUTE_1, AttMatrizUnidadeUTE_potencia_minima, Periodo()) >= periodo) {

								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {
									const double potencia_minima_old = a_dados.getElementoMatriz(idTermeletrica, IdUnidadeUTE_1, AttMatrizUnidadeUTE_potencia_minima, periodo, idPatamarCarga, double());
									a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).setElemento(AttMatrizUnidadeUTE_potencia_minima, periodo, idPatamarCarga, potencia_minima_old + sobreposicao * potencia_minima);
									const double potencia_maxima_old = a_dados.getElementoMatriz(idTermeletrica, IdUnidadeUTE_1, AttMatrizUnidadeUTE_potencia_maxima, periodo, idPatamarCarga, double());
									a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).setElemento(AttMatrizUnidadeUTE_potencia_maxima, periodo, idPatamarCarga, potencia_maxima_old + sobreposicao * potencia_maxima);
								} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

							} // if (a_dados.getIterador1Final(idTermeletrica, IdUnidadeUTE_1, AttMatrizUnidadeUTE_potencia_minima) >= periodo) {

							else {
								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {
									a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).addElemento(AttMatrizUnidadeUTE_potencia_minima, periodo, idPatamarCarga, sobreposicao * potencia_minima);
									a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).addElemento(AttMatrizUnidadeUTE_potencia_maxima, periodo, idPatamarCarga, sobreposicao * potencia_maxima);
								} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
							} // else {

						} // if ((sobreposicao > 0.0) && (periodo >= periodo_deck)) {

					} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {



					//******************************************************************************************************************
					//Guarda informação nos SmartElementos
					//******************************************************************************************************************

					a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).setAtributo(AttComumUnidadeUTE_potencia_maxima, potencia_maxima);

					a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_fator_de_capacidade, fator_de_capacidade);
					a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_indisponibilidade_forcada, indisponibilidade_forcada);
					a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_indisponibilidade_programada, indisponibilidade_programada);

					a_dados.vetorTermeletrica.att(idTermeletrica).setVetor(AttVetorTermeletrica_indisponibilidade_forcada, SmartEnupla<Periodo, double>(horizonte_estudo, indisponibilidade_forcada));

					a_dados.vetorTermeletrica.att(idTermeletrica).setVetor(AttVetorTermeletrica_fator_de_capacidade, SmartEnupla<Periodo, double>(horizonte_estudo, fator_de_capacidade));

					//Cria vetores com atributos

					///////////////////////////////////////////////

					SmartEnupla<Periodo, double> vetor_indisponibilidade_programada(horizonte_estudo, 0.0);

					const Periodo periodo_pos_manutt = Periodo(IdMes_1, IdAno(horizonte_estudo_DECK.getIteradorInicial().getAno() + numero_anos_manutencao_termica_programada));

					for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

						if (periodo >= periodo_pos_manutt)
							vetor_indisponibilidade_programada.setElemento(periodo, indisponibilidade_programada);

					} // for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

					a_dados.vetorTermeletrica.att(idTermeletrica).setVetor(AttVetorTermeletrica_indisponibilidade_programada, vetor_indisponibilidade_programada);

				} // if (idTermeletrica != IdTermeletrica_Nenhum){

			}//while (std::getline(leituraArquivo, line)) {

			leituraArquivo.close();

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_NEWAVE_26_TERM: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_CLAST_201908_NW25(Dados& a_dados, std::string nomeArquivo) {

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;

		//Horizonte de estudo
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		////////////////////////////////////////////////////////

		SmartEnupla<IdTermeletrica, SmartEnupla<Periodo, double>> lista_custo_operacao(IdTermeletrica_1, std::vector<SmartEnupla<Periodo, double>>(a_dados.getMaiorId(IdTermeletrica()), SmartEnupla<Periodo, double>(horizonte_estudo_DECK, 0.0)));

		if (leituraArquivo.is_open()) {

			//*******************************************************************************************************************
			//Registro Tipo 1
			//*******************************************************************************************************************

			//Dois registros destinados a comentários
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);

			while (true) {

				std::getline(leituraArquivo, line);

				strNormalizada(line);

				//Número da classe térmica ou critério de parada
				atributo = line.substr(1, 4);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				if (atributo == "9999")
					break;

				const int classe_CEPEL = std::atoi(atributo.c_str());

				//Nome da classe térmica
				atributo = line.substr(6, 12);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				const string nome_classe = atributo;

				//Tipo de combustível da classe térmica
				atributo = line.substr(19, 10);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				const string tipo_combustivel = atributo;

				//Identifica se alguma termelétrica tem sido inicializada com codigo_usina
				const IdTermeletrica idTermeletrica = getIdFromCodigoONS(lista_classe_ONS_termeletrica, classe_CEPEL);

				if (idTermeletrica != IdTermeletrica_Nenhum) {

					//Custo de operação para cada ano de operação

					const IdAno ano_inicial = horizonte_estudo_DECK.getIteradorInicial().getAno();

					for (Periodo periodo = horizonte_estudo_DECK.getIteradorInicial(); periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo)) {

						atributo = line.substr(30 + int(periodo.getAno() - ano_inicial) * 8, 7);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						lista_custo_operacao.at(idTermeletrica).at(periodo) = atof(atributo.c_str());

					} // for (Periodo periodo = horizonte_estudo_DECK.getIteradorInicial(); periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo)) {

					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
						for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++)
							a_dados.vetorTermeletrica.att(idTermeletrica).addElemento(AttMatrizTermeletrica_custo_de_operacao, periodo, idPatamarCarga, 0.0);
					} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

					//*******************************************************************************************************************
					// Guarda a informação nos SmartElementos
					//*******************************************************************************************************************

					a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_tipo_combustivel, getTipoCombustivelFromChar(tipo_combustivel.c_str()));

				} // if (idTermeletrica != IdTermeletrica_Nenhum){

			}//while (true) {

			//*******************************************************************************************************************
			//Registro Tipo 2
			//*******************************************************************************************************************

			//Dois registros destinados a comentários
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);

				///////////////////////////////////////////////////////////////////////
				//Campo 1 - Número da classe térmica ou critério de parada
				///////////////////////////////////////////////////////////////////////

				atributo = line.substr(1, 4);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				const int classe_CEPEL = std::atoi(atributo.c_str());

				const IdTermeletrica idTermeletrica = getIdFromCodigoONS(lista_classe_ONS_termeletrica, classe_CEPEL);

				if (idTermeletrica != IdTermeletrica_Nenhum) {

					///////////////////////////////////////////////////////////////////////
					//Campo 2 - Novo valor do Custo de operação da classe térmica($ / MWh)
					///////////////////////////////////////////////////////////////////////

					atributo = line.substr(8, 7);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const double custo_de_operacao = std::atof(atributo.c_str());

					Periodo periodo_inicio_modif;
					Periodo periodo_final_modif;

					///////////////////////////////////////////////////////////////////////
					//Campo 4 - Mês de início da modificação
					///////////////////////////////////////////////////////////////////////

					atributo = line.substr(17, 2);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());				

					if (atributo == "") {//Manual: Se os campos 3, 4, 5 e 6 não forem preenchidos, a modificação será válida apenas para o primeiro mês do primeiro ano de planejamento
						periodo_inicio_modif = horizonte_estudo_DECK.getIteradorInicial();
						periodo_final_modif  = horizonte_estudo_DECK.getIteradorInicial();
					}//if(mes_inicio_informado == false) {

					else {

						///////////////////////////////////////////////////////////////////////
						//Campo 5 - Ano de início da modificação
						///////////////////////////////////////////////////////////////////////

						const int mes_inicio = atoi(atributo.c_str());

						atributo = line.substr(20, 4);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						if (atributo == "")
							throw std::invalid_argument("Mes Inicial reportado sem Ano Inicial");

						const int ano_inicio = atoi(atributo.c_str());

						periodo_inicio_modif = Periodo(mes_inicio, ano_inicio);

						///////////////////////////////////////////////////////////////////////
						//Campo 6 - Mês de fim da modificação
						///////////////////////////////////////////////////////////////////////

						atributo = line.substr(26, 2);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						if (atributo == "")
							periodo_final_modif = horizonte_estudo_DECK.getIteradorFinal();

						else{

							const int mes_final = atoi(atributo.c_str());

							///////////////////////////////////////////////////////////////////////
							//Campo 7 - Ano de fim da modificação
							///////////////////////////////////////////////////////////////////////

							atributo = line.substr(29, 4);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							if (atributo == "")
								throw std::invalid_argument("Mes Final reportado sem Ano Final");

							const int ano_final = atoi(atributo.c_str());

							periodo_final_modif = Periodo(mes_final, ano_final);

						}//else {

					}//else {

					for (Periodo periodo = periodo_inicio_modif; periodo <= periodo_final_modif; periodo++)
							lista_custo_operacao.at(idTermeletrica).at(periodo) = custo_de_operacao;

				} // if (idTermeletrica != IdTermeletrica_Nenhum){

			}//while (std::getline(leituraArquivo, line)) {

			leituraArquivo.close();

			double sobreposicao_atraso_periodo_inicial = 0.0;

			if (desconsiderar_atraso_periodo_estudo_inicial)
				sobreposicao_atraso_periodo_inicial = horizonte_estudo.getIteradorInicial().atraso(horizonte_estudo_DECK.getIteradorInicial());

			for (IdTermeletrica idTermeletrica = IdTermeletrica_1; idTermeletrica <= lista_custo_operacao.getIteradorFinal(); idTermeletrica++) {

				if (lista_custo_operacao.at(idTermeletrica).size() > 0) {

					for (Periodo periodo_DECK = lista_custo_operacao.at(idTermeletrica).getIteradorInicial(); periodo_DECK <= lista_custo_operacao.at(idTermeletrica).getIteradorFinal(); periodo_DECK++) {

						for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							double sobreposicao = periodo.sobreposicao(periodo_DECK);

							if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
								sobreposicao += sobreposicao_atraso_periodo_inicial;

							if (sobreposicao > 0.0) {

								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++){
									if (a_dados.getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_custo_de_operacao) > 0) {
										const double valor_antigo = a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_custo_de_operacao, periodo, idPatamarCarga, double());

										a_dados.vetorTermeletrica.att(idTermeletrica).setElemento(AttMatrizTermeletrica_custo_de_operacao, periodo, idPatamarCarga, valor_antigo + sobreposicao * lista_custo_operacao.at(idTermeletrica).at(periodo_DECK));
									}
								} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= lista_custo_operacao.at(idTermeletrica).at(periodo_DECK).getIteradorFinal(); idPatamarCarga++) {

							} // if (sobreposicao > 0.0) {

						} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

					} // for (Periodo periodo_DECK = lista_custo_operacao.at(idTermeletrica).getIteradorInicial(); periodo_DECK <= lista_custo_operacao.at(idTermeletrica).getIteradorFinal(); periodo_DECK++) {

				} // if (lista_custo_operacao.at(idTermeletrica).size() > 0) {

			} // for (IdTermeletrica idTermeletrica = IdTermeletrica_1; idTermeletrica <= lista_custo_operacao.getIteradorFinal(); idTermeletrica++) {


		}//if (leituraArquivo.is_open()) {

		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_NEWAVE_26_CLAST: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_EXPH_201908_NW25(Dados& a_dados, std::string nomeArquivo) {

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;

		bool registro_tipo_1;
		bool registro_tipo_2;

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		////////////////////////////////////////////////////////

		if (leituraArquivo.is_open()) {

			//Pula o cabeçalho
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Campo 1 -  Código da usina no cadastro de usinas
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				atributo = line.substr(0, 4);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				const int codigo_usina = atoi(atributo.c_str());

				const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

				if (idHidreletrica == IdHidreletrica_Nenhum) {

					for (int i = 1; i <= lista_codigo_ONS_hidreletrica_agregada.size(); i++) {
						if (lista_codigo_ONS_hidreletrica_agregada.getElemento(i) == codigo_usina)
							throw std::invalid_argument("Esta representacao deve ser reavaliada em termos das hidreletricas desagregadas da usina codigo " + getString(codigo_usina));
					}

				} // if (idHidreletrica == IdHidreletrica_Nenhum){

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Campo 2 -  Nome da usina
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				atributo = line.substr(5, 12);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				while (true) {

					if (idHidreletrica != IdHidreletrica_Nenhum) {

						if (lista_hidreletrica_com_expansao.getElemento(idHidreletrica)) {

							//Mês de início do enchimento de volume morto
							atributo = line.substr(18, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							if (atributo == "") {

								registro_tipo_1 = false;
								registro_tipo_2 = true;

							}//if(atributo == "") {
							else {

								registro_tipo_1 = true;
								registro_tipo_2 = false;

							}//else {

							//////////////////////
							//Leitura do registros
							//////////////////////

							if (registro_tipo_1 == true) {

								//Manual:  O registro tipo 1 só pode ser informado uma única vez

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 3 -  Mês de início do enchimento de volume morto
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								atributo = line.substr(18, 2);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const int mes_inicio_enchimento = atoi(atributo.c_str());

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 4 -  Ano de início do enchimento de volume morto
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								atributo = line.substr(21, 4);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const int ano_inicio_enchimento = atoi(atributo.c_str());

								/////////////////////////////

								const Periodo periodo_inicio_enchimento(mes_inicio_enchimento, ano_inicio_enchimento);

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 5 -  Duração em meses do enchimento de volume morto
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								atributo = line.substr(31, 2);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const int duracao_em_meses_enchimento = atoi(atributo.c_str());


								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 6 -  % do volume morto já preenchido até a data de início informada.
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								atributo = line.substr(37, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const double porcentagem_volume_morto_preenchido = atof(atributo.c_str());

								//******************************************************************************
								//Guarda esta informação nos SmartElementos como uma modificação VMORTO
								//******************************************************************************

								// Inicializa adicao de modificacao na hidreletrica
								if (lista_modificacaoUHE.size() - 1 < idHidreletrica) {

									IdHidreletrica menorIdHidreletrica = IdHidreletrica_1;

									if (lista_modificacaoUHE.size() > 2)
										menorIdHidreletrica = IdHidreletrica(lista_modificacaoUHE.size() - 1);

									for (IdHidreletrica idHidreletrica_aux = menorIdHidreletrica; idHidreletrica_aux <= idHidreletrica; idHidreletrica_aux++)
										lista_modificacaoUHE.push_back(std::vector< ModificacaoUHE>());

								} // if (lista_modificacaoUHE.size() - 1 < idHidreletrica) {

								ModificacaoUHE modificacaoUHE;

								modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_VMORTO;
								modificacaoUHE.tipo_de_grandeza = TipoGrandezaModificacao_absoluta;
								modificacaoUHE.periodo = periodo_inicio_enchimento;
								modificacaoUHE.valor_1 = double(duracao_em_meses_enchimento);
								modificacaoUHE.valor_2 = porcentagem_volume_morto_preenchido;

								lista_modificacaoUHE.at(idHidreletrica).push_back(modificacaoUHE);


							}//if(registro_tipo_1 == true) {

							if (registro_tipo_2 == true) {

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 3 -  Mês de entrada em operação da unidade hidroelétrica	
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								atributo = line.substr(44, 2);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const int mes_entrada_operacao_unidade = atoi(atributo.c_str());

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 4 -  Ano de entrada em operação da unidade hidroelétrica	
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								atributo = line.substr(47, 4);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const int ano_entrada_operacao_unidade = atoi(atributo.c_str());

								/////////////////

								const Periodo periodo_entrada_operacao_unidade(mes_entrada_operacao_unidade, ano_entrada_operacao_unidade);

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 5 -  Número da unidade a ser adicionada	
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								atributo = line.substr(60, 2);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const int numero_unidade_adicionada = atoi(atributo.c_str());

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 6 -  Número do conjunto da unidade a ser adicionada	
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								atributo = line.substr(64, 1);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const int numero_conjunto_da_unidade_adicionada = atoi(atributo.c_str());

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo X -  Potência (Não está especificado no manual de referência porém aparece nos DECKs)	
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								atributo = line.substr(51, 8);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const double potencia = atof(atributo.c_str());

								//******************************************************************************
								//Guarda esta informação nos SmartElementos como uma modificação NOVAMAQ
								//******************************************************************************

								// Inicializa adicao de modificacao na hidreletrica
								if (lista_modificacaoUHE.size() - 1 < idHidreletrica) {

									IdHidreletrica menorIdHidreletrica = IdHidreletrica_1;

									if (lista_modificacaoUHE.size() > 2)
										menorIdHidreletrica = IdHidreletrica(lista_modificacaoUHE.size() - 1);

									for (IdHidreletrica idHidreletrica_aux = menorIdHidreletrica; idHidreletrica_aux <= idHidreletrica; idHidreletrica_aux++)
										lista_modificacaoUHE.push_back(std::vector< ModificacaoUHE>());

								} // if (lista_modificacaoUHE.size() - 1 < idHidreletrica) {

								ModificacaoUHE modificacaoUHE;

								modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_NOVAMAQ;
								modificacaoUHE.tipo_de_grandeza = TipoGrandezaModificacao_absoluta;
								modificacaoUHE.periodo = periodo_entrada_operacao_unidade;
								modificacaoUHE.valor_1 = potencia;
								modificacaoUHE.valor_2 = double(numero_unidade_adicionada);
								modificacaoUHE.valor_3 = double(numero_conjunto_da_unidade_adicionada);

								lista_modificacaoUHE.at(idHidreletrica).push_back(modificacaoUHE);

							}//if(registro_tipo_2 == true) {

						} // if (lista_hidreletrica_com_expansao.at(idHidreletrica)){

					} // if (idHidreletrica != IdHidreletrica_Nenhum){

					std::getline(leituraArquivo, line);

					strNormalizada(line);

					//Campo 1
					atributo = line.substr(0, 4);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					if (atributo == "9999")
						break;

				}//while (true) {

			}//while(std::getline(leituraArquivo, line)) {

			leituraArquivo.close();

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_NEWAVE_26_EXPH: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_EXPT_201908_NW25(Dados& a_dados, std::string nomeArquivo) {

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;

		//Horizonte de estudo
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		////////////////////////////////////////////////////////

		const Periodo periodo_pos_manutt = Periodo(1, atoi(getString(horizonte_estudo.getIteradorInicial().getAno()).c_str()) + numero_anos_manutencao_termica_programada);

		if (leituraArquivo.is_open()) {

			//Pula o cabeçalho
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);
				
				//Campo 1 -  Número da usina térmica
				const IdTermeletrica idTermeletrica = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, std::stoi(line.substr(0, 4)));

				if (idTermeletrica != IdTermeletrica_Nenhum) {

					if (lista_termeletrica_com_expansao.getElemento(idTermeletrica)) {
						
						//Campo 2 -  Tipo de modificação
						atributo = line.substr(5, 5);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
						const string tipo_modificacao = atributo;

						//Campo 3 -  Novo valor		
						double valor = std::stod(line.substr(11, 8));

						//Campo 4 -  Mês de início da modificação
						const int mes_inicio = std::stoi(line.substr(20, 2));

						//Campo 5 -  Ano de início da modificação			
						const int ano_inicio = std::stoi(line.substr(23, 4));

						//**************************************
						const int line_size = int(line.length());
						int mes_final, ano_final;

						if (line_size >= 35) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 6 -  Mês de fim da modificação
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							atributo = line.substr(28, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							if (atributo != "") {

								mes_final = atoi(atributo.c_str());

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 7 -  Ano de fim da modificação
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								atributo = line.substr(31, 4);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								if (atributo == "")
									throw std::invalid_argument("Mes_final reportado sem Ano_final associado");

								ano_final = atoi(atributo.c_str());

							}//if(atributo != "") {

							else {//Não é necessário o preenchimento dos campos 6 e 7 se a modificação for válida até o	fim do período de estudo

								mes_final = atoi(getString(horizonte_estudo.getIteradorFinal().getMes()).c_str());
								ano_final = atoi(getString(horizonte_estudo.getIteradorFinal().getAno()).c_str());

							}//else {

						}//if(line_size>=35) {
						else {//Não é necessário o preenchimento dos campos 6 e 7 se a modificação for válida até o	fim do período de estudo

							mes_final = atoi(getString(horizonte_estudo.getIteradorFinal().getMes()).c_str());
							ano_final = atoi(getString(horizonte_estudo.getIteradorFinal().getAno()).c_str());

						}//else {

						//**********************
						Periodo periodo_inicio;
						Periodo periodo_final(mes_final, ano_final);

						if ((mes_inicio == 0) || (ano_inicio == 0)) {
							periodo_inicio = horizonte_estudo.getIteradorInicial();
							periodo_final = horizonte_estudo.getIteradorFinal();
						}
						else
							periodo_inicio = Periodo(mes_inicio, ano_inicio);

						// Inicializa adicao de modificacao na Termeletrica
						if (lista_modificacaoUTE.size() - 1 < idTermeletrica) {

							IdTermeletrica menorIdTermeletrica = IdTermeletrica_1;

							if (lista_modificacaoUTE.size() > 2)
								menorIdTermeletrica = IdTermeletrica(lista_modificacaoUTE.size() - 1);

							for (IdTermeletrica idTermeletrica_aux = menorIdTermeletrica; idTermeletrica_aux <= idTermeletrica; idTermeletrica_aux++)
								lista_modificacaoUTE.push_back(std::vector< ModificacaoUTE>());

						} // if (lista_modificacaoUTE.size() - 1 < idTermeletrica) {

						ModificacaoUTE modificacaoUTE;

						modificacaoUTE.periodo_inicial = periodo_inicio;
						modificacaoUTE.periodo_final = periodo_final;

						if (tipo_modificacao == "GTMIN") {
							modificacaoUTE.tipo_de_modificacao = TipoModificacaoUTE_GTMIN;
							modificacaoUTE.valor = valor;
						}

						else if (tipo_modificacao == "POTEF") {
							modificacaoUTE.tipo_de_modificacao = TipoModificacaoUTE_POTEF;
							modificacaoUTE.valor = valor;
						}

						else if (tipo_modificacao == "FCMAX") {
							modificacaoUTE.tipo_de_modificacao = TipoModificacaoUTE_FCMAX;
							modificacaoUTE.valor = valor * 0.01;
						}

						else if (tipo_modificacao == "IPTER") {//IPTER (%)
							modificacaoUTE.tipo_de_modificacao = TipoModificacaoUTE_IPTER;
							modificacaoUTE.valor = valor * 0.01;

							if ((modificacaoUTE.periodo_inicial < periodo_pos_manutt) && (modificacaoUTE.periodo_final >= periodo_pos_manutt))
								modificacaoUTE.periodo_inicial = periodo_pos_manutt;
							else if ((modificacaoUTE.periodo_inicial < periodo_pos_manutt) && (modificacaoUTE.periodo_final < periodo_pos_manutt))
								modificacaoUTE.valor = 0.0;

						}

						else if (tipo_modificacao == "TEIFT") {//TEIFT (%)
							modificacaoUTE.tipo_de_modificacao = TipoModificacaoUTE_TEIFT;
							modificacaoUTE.valor = valor * 0.01;
						}
						else
							throw std::invalid_argument("Modificacao Termeletrica nao Implementada.");

						lista_modificacaoUTE.at(idTermeletrica).push_back(modificacaoUTE);

					} // if (lista_termeletrica_com_expansao.at(idTermeletrica)){

				} // if (idTermeletrica != IdTermeletrica_Nenhum){

			}//while (std::getline(leituraArquivo, line)) {

			leituraArquivo.close();

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_NEWAVE_26_EXPT: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_PATAMAR_201908_NW25(Dados &a_dados, std::string nomeArquivo) {

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;

		//Horizonte de estudo
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		////////////////////////////////////////////////////////

		if (leituraArquivo.is_open()) {

			if (duracao_patamar_carga_variavel_por_ano == true) {

				double sobreposicao_atraso_periodo_inicial = 0.0;

				if (desconsiderar_atraso_periodo_estudo_inicial)
					sobreposicao_atraso_periodo_inicial = horizonte_estudo.getIteradorInicial().atraso(horizonte_estudo_DECK.getIteradorInicial());

				//*******************************************************************************************************************
				//Bloco 1 -   Este bloco é composto por um registro especificando o total de patamares de mercado
				//*******************************************************************************************************************

				//Registros destinados a comentários
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);

				std::getline(leituraArquivo, line);

				strNormalizada(line);

				//Número de patamares de déficit
				atributo = line.substr(1, 2);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				const int numero_patamares = atoi(atributo.c_str());

				//*******************************************************************************************************************
				//Bloco 2 -  Este bloco pode ser de dois tipos, conforme definido pelo registro 39 do arquivo de dados gerais. 
				//           Se esse registro for preenchido com o valor zero, o bloco será do Tipo 1, e se for preenchido 
				//           com o valor um, será do Tipo 2                     
				//*******************************************************************************************************************

				//Três registros destinados a comentários
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);

				///////////////////////////////////////////////////////////////
				//Inicializa percentual_duracao_patamar_carga com valores 0
				//para todos os submercados inicializados
				///////////////////////////////////////////////////////////////
				std::vector<double> vetor_zero(numero_patamares, 0.0);

				const SmartEnupla<IdPatamarCarga, double> vetor_patamar_zero(IdPatamarCarga_1, vetor_zero);

				SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_periodo_patamar_zero(horizonte_estudo, vetor_patamar_zero);

				const IdSubmercado idSubmercado_Maior = a_dados.vetorSubmercado.getMaiorId();

				a_dados.setMatriz(AttMatrizDados_percentual_duracao_patamar_carga, matriz_periodo_patamar_zero);

				const Periodo periodo_estudo_inicial = horizonte_estudo.getIteradorInicial();

				const Periodo periodo_estudo_final = horizonte_estudo.getIteradorFinal();

				////////////////

				while (true) {

					////////////////////////////////////////
					// Registro Tipo 2
					////////////////////////////////////////

					std::getline(leituraArquivo, line);

					strNormalizada(line);

					//////////////////////////////////////////////////////////////////////////////////////
					//Campo 1 - Ano para o qual os fatores serão lidos (apenas para orientação do usuário)
					//////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(0, 4);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const IdAno ano = getIdAnoFromChar(atributo.c_str());

					int patamar = 1;

					IdMes mes_inicial = IdMes_1;

					if (ano == horizonte_estudo_DECK.getIteradorInicial().getAno())
						mes_inicial = horizonte_estudo_DECK.getIteradorInicial().getMes();

					IdMes mes = mes_inicial;

					while (true) {

						const IdPatamarCarga idPatamarCarga = IdPatamarCarga(patamar);

						//////////////////////////////////////////////////////////////////////////////////////
						//Campo 2-12 - Fator de duração do X patamar para o X mês do ano em questão
						//////////////////////////////////////////////////////////////////////////////////////

						atributo = line.substr(6 + 8 * (int(mes) - 1), 6);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						const double percentual_duracao_patamar_carga = atof(atributo.c_str());

						//Guarda informação no Smart Elemento

						const Periodo periodo_DECK(mes, ano);

						for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							double sobreposicao = periodo.sobreposicao(periodo_DECK);

							if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
								sobreposicao += sobreposicao_atraso_periodo_inicial;

							if (sobreposicao > 0.0) {

								const double valor_antigo = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());

								a_dados.setElemento(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, valor_antigo + sobreposicao * percentual_duracao_patamar_carga);

							} // if (sobreposicao > 0.0) {

						} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						//Passo de linha e atualização do periodo

						if (mes == IdMes_12) {

							patamar++;

							//Critério de parada

							if (patamar > numero_patamares)
								break;

							mes = mes_inicial;

							std::getline(leituraArquivo, line);

							strNormalizada(line);

						}//if (mes == 12) {

						else
							mes++;

					}//while (true) {

					if (ano == horizonte_estudo_DECK.getIteradorFinal().getAno())//Significa que todos os anos do horizonte foram lidos
						break;

				}//while (true) {

				//*******************************************************************************************************************
				//Bloco 3 - Este bloco é composto por tantos conjuntos de registros quantos forem os subsistemas/submercados 
				//          e é precedido de um conjunto de três registros de existência obrigatória, destinado a comentários. 
				//          O código 9999 no campo 1 indica final do bloco. Para cada subsistema/submercado, os conjuntos 
				//          de registros podem ser ou do tipo 1 ou do tipo 2                    
				//*******************************************************************************************************************

				//Registros destinados a comentários
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);

				///////////////////////////////////////////////////////////////
				//Inicializa percentual_duracao_patamar_carga com valores 0
				//para todos os submercados inicializados
				///////////////////////////////////////////////////////////////

				for (IdSubmercado idSubmercado = IdSubmercado_1; idSubmercado <= idSubmercado_Maior; idSubmercado++)
					a_dados.vetorSubmercado.att(idSubmercado).setMatriz(AttMatrizSubmercado_percentual_variacao_patamar_carga, matriz_periodo_patamar_zero);

				////////////////

				while (true) {

					////////////////////////////////////////
					// Registro 1
					////////////////////////////////////////

					std::getline(leituraArquivo, line);

					strNormalizada(line);

					//Critério de parada
					atributo = line.substr(0, 4);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					if (atributo == "9999")
						break;

					//////////////////////////////////////////////////////////////////////////////////////
					//Campo 1 - Número do subsistema/submercado 
					//////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(1, 3);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const int submercado_codigo_usina = atoi(atributo.c_str());

					const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, submercado_codigo_usina);

					if (idSubmercado == IdSubmercado_Nenhum)
						throw std::invalid_argument("SISTEMA.DAT submercado nao instanciado, ver Bloco 4 Registro tipo 1 ");

					while (true) {

						////////////////////////////////////////
						// Registro 2
						////////////////////////////////////////

						std::getline(leituraArquivo, line);

						strNormalizada(line);

						//////////////////////////////////////////////////////////////////////////////////////
						//Campo 1 - Ano referente à informação. 
						//////////////////////////////////////////////////////////////////////////////////////

						atributo = line.substr(3, 4);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						const IdAno ano = getIdAnoFromChar(atributo.c_str());

						int patamar = 1;

						IdMes mes_inicial = IdMes_1;

						if (ano == horizonte_estudo_DECK.getIteradorInicial().getAno())
							mes_inicial = horizonte_estudo_DECK.getIteradorInicial().getMes();

						IdMes mes = mes_inicial;

						while (true) {

							const Periodo periodo_DECK(mes, ano);

							///////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2-13 - Fator que deve ser aplicado à demanda média para compor o mercado do n-ésimo patamar do mês X
							///////////////////////////////////////////////////////////////////////////////////////////////////////////////

							const IdPatamarCarga idPatamarCarga = IdPatamarCarga(patamar);

							try {
								atributo = line.substr(8 + 7 * (int(mes) - 1), 6);
							}
							catch (const std::exception& erro) { throw std::invalid_argument(getString(periodo_DECK) + " " + getFullString(idPatamarCarga) + ": \n" + std::string(erro.what())); }

							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double percentual_variacao_patamar_carga = atof(atributo.c_str());

							//Guarda informação no Smart Elemento

							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								double sobreposicao = periodo.sobreposicao(periodo_DECK);

								if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
									sobreposicao += sobreposicao_atraso_periodo_inicial;

								if (sobreposicao > 0.0) {

									const double valor_antigo = a_dados.vetorSubmercado.att(idSubmercado).getElementoMatriz(AttMatrizSubmercado_percentual_variacao_patamar_carga, periodo, idPatamarCarga, double());

									a_dados.vetorSubmercado.att(idSubmercado).setElemento(AttMatrizSubmercado_percentual_variacao_patamar_carga, periodo, idPatamarCarga, valor_antigo + sobreposicao * percentual_variacao_patamar_carga);

								} // if (sobreposicao > 0.0) {

							} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							//Passo de linha e atualização do periodo

							if (mes == IdMes_12) {

								patamar++;

								//Critério de parada

								if (patamar > numero_patamares)
									break;

								mes = mes_inicial;

								std::getline(leituraArquivo, line);

								strNormalizada(line);

							}//if (mes == 12) {
							else
								mes++;

						}//while (true) {

						if (ano == horizonte_estudo_DECK.getIteradorFinal().getAno())//Significa que todos os anos do horizonte foram lidos
							break;

					}//while (true) {

				}//while (true) {

				//*******************************************************************************************************************
				//Bloco 4 - Este bloco é composto por tantos conjuntos de registros quantas forem as interligações entre os 
				//          subsistemas/submercados. É precedido de um conjunto de cinco registros de existência obrigatória, 
				//          destinado a orientar o usuário no preenchimento/alteração dos dados. . O código 9999 no campo 1 indica 
				//          final do bloco. Para cada interligação, os conjuntos de registros podem ser ou do tipo 1 ou do tipo 2.                  
				//*******************************************************************************************************************

				//Registros destinados a comentários
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);

				///////////////////////////////////////////////////////////////
				//Inicializa percentual_duracao_patamar_carga com valores 0
				//para todos os submercados inicializados
				///////////////////////////////////////////////////////////////

				const IdIntercambio idIntercambio_Maior = a_dados.vetorIntercambio.getMaiorId();

				for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= idIntercambio_Maior; idIntercambio++)
					a_dados.vetorIntercambio.att(idIntercambio).setMatriz(AttMatrizIntercambio_percentual_variacao_patamar_carga, matriz_periodo_patamar_zero);

				////////////////

				bool leitura_bloco_5 = true;

				while (true) {

					std::getline(leituraArquivo, line);

					strNormalizada(line);

					//Alguns Decks (p.ex. NW201907) não colocam o 9999 para significar o fim deste bloco

					if (int(line.length()) == 0) {//Significa que não existem mais linhas para ler 

						leitura_bloco_5 = false;

						break;

					}//if (int(line.length()) == 0) {

					atributo = line.substr(0, 4);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					if (atributo == "9999")
						break;

					//////////////////////////////////////////////////////////////////////////////////////
					//Campo 1 - Número do subsistema/submercado A 
					//////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(1, 3);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const int submercado_origem_codigo_usina = atoi(atributo.c_str());

					const IdSubmercado idSubmercado_origem = getIdFromCodigoONS(lista_codigo_ONS_submercado, submercado_origem_codigo_usina);

					if (idSubmercado_origem == IdSubmercado_Nenhum)
						throw std::invalid_argument("SISTEMA.DAT submercado nao instanciado, ver Bloco 4 Registro tipo 1 ");


					//////////////////////////////////////////////////////////////////////////////////////
					//Campo 2 - Número do subsistema/submercado B 
					//////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(5, 3);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const int submercado_destino_codigo_usina = atoi(atributo.c_str());

					const IdSubmercado idSubmercado_destino = getIdFromCodigoONS(lista_codigo_ONS_submercado, submercado_destino_codigo_usina);

					if (idSubmercado_destino == IdSubmercado_Nenhum)
						throw std::invalid_argument("SISTEMA.DAT submercado nao instanciado, ver Bloco 4 Registro tipo 1 ");


					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Determina o Intercambio
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					std::vector<IdIntercambio> idIntercambio_inicializado = a_dados.vetorIntercambio.getIdObjetos(AttComumIntercambio_submercado_origem, idSubmercado_origem);

					int idIntercambio_inicializado_size = int(idIntercambio_inicializado.size());

					IdIntercambio idIntercambio;

					for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

						idIntercambio = idIntercambio_inicializado.at(intercambio_inicializado);

						if (a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_destino)
							break;

						if (intercambio_inicializado + 1 == idIntercambio_inicializado_size)
							throw std::invalid_argument("Intercambio nao inicializado entre subsistema_" + getString(idSubmercado_origem) + "e subsistema_" + getString(idSubmercado_destino));

					}//for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

					/////////////////////////

					while (true) {

						////////////////////////////////////////
						// Registro 2
						////////////////////////////////////////

						std::getline(leituraArquivo, line);

						strNormalizada(line);

						//////////////////////////////////////////////////////////////////////////////////////
						//Campo 1 - Ano referente à informação. 
						//////////////////////////////////////////////////////////////////////////////////////

						atributo = line.substr(3, 4);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						const IdAno ano = getIdAnoFromChar(atributo.c_str());

						int patamar = 1;

						IdMes mes_inicial = IdMes_1;

						if (ano == horizonte_estudo_DECK.getIteradorInicial().getAno())
							mes_inicial = horizonte_estudo_DECK.getIteradorInicial().getMes();

						IdMes mes = mes_inicial;

						while (true) {

							const IdPatamarCarga idPatamarCarga = IdPatamarCarga(patamar);

							///////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 1-12 - Fator que deve ser aplicado ao intercâmbio médio para compor o intercâmbio do 
							//             subsistema/submercado A para o subsistema/submercado B do n-ésimo patamar de cada mês deste ano
							///////////////////////////////////////////////////////////////////////////////////////////////////////////////

							atributo = line.substr(8 + 7 * (int(mes) - 1), 6);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double percentual_variacao_patamar_carga = atof(atributo.c_str());

							//Guarda informação no Smart Elemento

							const Periodo periodo_DECK(mes, ano);

							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								double sobreposicao = periodo.sobreposicao(periodo_DECK);

								if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
									sobreposicao += sobreposicao_atraso_periodo_inicial;

								if (sobreposicao > 0.0) {

									const double valor_antigo = a_dados.vetorIntercambio.att(idIntercambio).getElementoMatriz(AttMatrizIntercambio_percentual_variacao_patamar_carga, periodo, idPatamarCarga, double());

									a_dados.vetorIntercambio.att(idIntercambio).setElemento(AttMatrizIntercambio_percentual_variacao_patamar_carga, periodo, idPatamarCarga, valor_antigo + sobreposicao * percentual_variacao_patamar_carga);

								} // if (sobreposicao > 0.0) {

							} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							//Passo de linha e atualização do periodo

							if (mes == IdMes_12) {

								patamar++;

								//Critério de parada

								if (patamar > numero_patamares)
									break;

								mes = mes_inicial;

								std::getline(leituraArquivo, line);

								strNormalizada(line);

							}//if (mes == 12) {
							else
								mes++;

						}//while (true) {

						if (ano == horizonte_estudo_DECK.getIteradorFinal().getAno())//Significa que todos os anos do horizonte foram lidos
							break;

					}//while (true) {

				}//while (true) {


				//*******************************************************************************************************************
				//Bloco 5 - Este bloco é composto no máximo por tantos conjuntos de registros quantas forem
				//os blocos de usinas não simuladas.É precedido de um conjunto de quatro registros
				//de existência obrigatória, destinado a orientar o usuário no preenchimento / alteração
				//dos dados..O código 9999 no campo 1 indica final do bloco.Para cada bloco de
				//usinas não simuladas, os conjuntos de registros podem ser ou do tipo 1 ou do tipo 2.
				//*******************************************************************************************************************

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Manual: Para os blocos de usinas não simuladas definidos no arquivo de dados dos subsistemas/submercados (item 3.6)
				//        e não informados no arquivo de patamares, serão considerados fatores unitários ao longo de todo horizonte 
				//        de estudo para todos os patamares de carga.
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				if (leitura_bloco_5 == false) {

					///////////////////////////////////////////////////////////////
					//Inicializa percentual_variacao_patamar_carga com valores 1
					//para todos os submercados inicializados
					///////////////////////////////////////////////////////////////
					std::vector<double> vetor_ones(numero_patamares, 1.0);

					const SmartEnupla<IdPatamarCarga, double> vetor_usinaNaoSimulada_percentual_variacao_patamar_carga(IdPatamarCarga_1, vetor_ones);

					SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_usinaNaoSimulada_percentual_variacao_patamar_carga(horizonte_estudo, vetor_usinaNaoSimulada_percentual_variacao_patamar_carga);

					const IdSubmercado idSubmercado_Maior = a_dados.vetorSubmercado.getMaiorId();

					for (IdSubmercado idSubmercado = IdSubmercado_1; idSubmercado <= idSubmercado_Maior; idSubmercado++) {

						if (a_dados.vetorSubmercado.att(idSubmercado).getAtributo(AttComumSubmercado_ficticio, bool()) == false) {

							const IdUsinaNaoSimulada maiorIdUsinaNaoSimulada = a_dados.vetorSubmercado.att(idSubmercado).getMaiorId(IdUsinaNaoSimulada());

							for (IdUsinaNaoSimulada idUsinaNaoSimulada = IdUsinaNaoSimulada_1; idUsinaNaoSimulada <= maiorIdUsinaNaoSimulada; idUsinaNaoSimulada++)
								a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).setMatriz(AttMatrizUsinaNaoSimulada_percentual_variacao_patamar_carga, matriz_usinaNaoSimulada_percentual_variacao_patamar_carga);

						}//if (a_dados.vetorSubmercado.att(idSubmercado).getAtributo(AttComumSubmercado_ficticio, bool()) == false) {

					}//for (IdSubmercado idSubmercado = IdSubmercado_1; idSubmercado <= idSubmercado_Maior; idSubmercado++) {

				}//if (leitura_bloco_5 == false) {
				else if (leitura_bloco_5 == true) {

					std::getline(leituraArquivo, line);

					strNormalizada(line);

					if (int(line.length()) == 0) {//Significa que não existem mais linhas para ler 

						///////////////////////////////////////////////////////////////
						//Inicializa percentual_variacao_patamar_carga com valores 1
						//para todos os submercados inicializados
						///////////////////////////////////////////////////////////////
						std::vector<double> vetor_ones(numero_patamares, 1.0);

						const SmartEnupla<IdPatamarCarga, double> vetor_usinaNaoSimulada_percentual_variacao_patamar_carga(IdPatamarCarga_1, vetor_ones);

						SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_usinaNaoSimulada_percentual_variacao_patamar_carga(horizonte_estudo, vetor_usinaNaoSimulada_percentual_variacao_patamar_carga);

						const IdSubmercado idSubmercado_Maior = a_dados.vetorSubmercado.getMaiorId();

						for (IdSubmercado idSubmercado = IdSubmercado_1; idSubmercado <= idSubmercado_Maior; idSubmercado++) {

							if (a_dados.vetorSubmercado.att(idSubmercado).getAtributo(AttComumSubmercado_ficticio, bool()) == false) {

								const IdUsinaNaoSimulada maiorIdUsinaNaoSimulada = a_dados.vetorSubmercado.att(idSubmercado).getMaiorId(IdUsinaNaoSimulada());

								for (IdUsinaNaoSimulada idUsinaNaoSimulada = IdUsinaNaoSimulada_1; idUsinaNaoSimulada <= maiorIdUsinaNaoSimulada; idUsinaNaoSimulada++)
									a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).setMatriz(AttMatrizUsinaNaoSimulada_percentual_variacao_patamar_carga, matriz_usinaNaoSimulada_percentual_variacao_patamar_carga);

							}//if (a_dados.vetorSubmercado.att(idSubmercado).getAtributo(AttComumSubmercado_ficticio, bool()) == false) {

						}//for (IdSubmercado idSubmercado = IdSubmercado_1; idSubmercado <= idSubmercado_Maior; idSubmercado++) {

					}//if (int(line.length()) == 0) {
					else {

						//Registros destinados a comentários
						std::getline(leituraArquivo, line);
						std::getline(leituraArquivo, line);
						std::getline(leituraArquivo, line);

						///////////////////////////////////////////////////////////////
						//Inicializa percentual_duracao_patamar_carga com valores 0
						//para todos os submercados inicializados
						///////////////////////////////////////////////////////////////

						for (IdSubmercado idSubmercado = IdSubmercado_1; idSubmercado <= idSubmercado_Maior; idSubmercado++) {

							const IdUsinaNaoSimulada maiorIdUsinaNaoSimulada = a_dados.vetorSubmercado.att(idSubmercado).getMaiorId(IdUsinaNaoSimulada());

							for (IdUsinaNaoSimulada idUsinaNaoSimulada = IdUsinaNaoSimulada_1; idUsinaNaoSimulada <= maiorIdUsinaNaoSimulada; idUsinaNaoSimulada++)
								a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).setMatriz(AttMatrizUsinaNaoSimulada_percentual_variacao_patamar_carga, matriz_periodo_patamar_zero);

						}//for (IdSubmercado idSubmercado = IdSubmercado_1; idSubmercado <= idSubmercado_Maior; idSubmercado++) {

						////////////////


						while (true) {

							std::getline(leituraArquivo, line);

							strNormalizada(line);

							//Alguns Decks (p.ex. NW201907) não colocam o 9999 para significar o fim deste bloco

							if (int(line.length()) == 0)//Significa que não existem mais linhas para ler 
								break;

							atributo = line.substr(0, 4);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							if ((atributo == "9999") || (leituraArquivo.eof()))
								break;

							//////////////////////////////////////////////////////////////////////////////////////
							//Campo 1 - Número do subsistema/submercado 
							//////////////////////////////////////////////////////////////////////////////////////

							atributo = line.substr(1, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int submercado_codigo_usina = atoi(atributo.c_str());

							const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, submercado_codigo_usina);

							if (idSubmercado == IdSubmercado_Nenhum)
								throw std::invalid_argument("SISTEMA.DAT submercado nao instanciado, ver Bloco 4 Registro tipo 1 ");

							//////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 - Número do bloco de usinas não simuladas
							//////////////////////////////////////////////////////////////////////////////////////

							IdUsinaNaoSimulada idUsinaNaoSimulada = IdUsinaNaoSimulada_1; //Padrao se a informaçao nao é dada desagregada

							if (line.size() > 4) {

								atributo = line.substr(5, (line.length()-4));
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								if (atributo != "")
									idUsinaNaoSimulada = IdUsinaNaoSimulada(atoi(atributo.c_str()));

							}//if (line.size() >= 8) {

							/////////////////////////

							while (true) {

								////////////////////////////////////////
								// Registro 2
								////////////////////////////////////////

								std::getline(leituraArquivo, line);

								strNormalizada(line);

								//////////////////////////////////////////////////////////////////////////////////////
								//Campo 1 - Ano referente à informação. 
								//////////////////////////////////////////////////////////////////////////////////////

								atributo = line.substr(3, 4);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const IdAno ano = getIdAnoFromChar(atributo.c_str());

								int patamar = 1;

								IdMes mes_inicial = IdMes_1;

								if (ano == horizonte_estudo_DECK.getIteradorInicial().getAno())
									mes_inicial = horizonte_estudo_DECK.getIteradorInicial().getMes();

								IdMes mes = mes_inicial;

								while (true) {

									///////////////////////////////////////////////////////////////////////////////////////////////////////////////
									//Campo 1-12 - Fator que deve ser aplicado ao intercâmbio médio para compor o intercâmbio do 
									//             subsistema/submercado A para o subsistema/submercado B do n-ésimo patamar de cada mês deste ano
									///////////////////////////////////////////////////////////////////////////////////////////////////////////////

									atributo = line.substr(8 + 7 * (int(mes) - 1), 6);
									atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

									const double percentual_variacao_patamar_carga = atof(atributo.c_str());

									//Guarda informação no Smart Elemento

									const IdPatamarCarga idPatamarCarga = IdPatamarCarga(patamar);

									const Periodo periodo_DECK(mes, ano);

									for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

										double sobreposicao = periodo.sobreposicao(periodo_DECK);

										if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
											sobreposicao += sobreposicao_atraso_periodo_inicial;

										if (sobreposicao > 0.0) {

											const double valor_antigo = a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).getElementoMatriz(AttMatrizUsinaNaoSimulada_percentual_variacao_patamar_carga, periodo, idPatamarCarga, double());

											a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).setElemento(AttMatrizUsinaNaoSimulada_percentual_variacao_patamar_carga, periodo, idPatamarCarga, valor_antigo + sobreposicao * percentual_variacao_patamar_carga);

										} // if (sobreposicao > 0.0) {

									} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									//Passo de linha e atualização do periodo

									if (mes == IdMes_12) {

										patamar++;

										//Critério de parada

										if (patamar > numero_patamares)
											break;

										mes = mes_inicial;

										std::getline(leituraArquivo, line);

										strNormalizada(line);

									}//if (mes == 12) {
									else
										mes++;

								}//while (true) {

								if (ano == horizonte_estudo_DECK.getIteradorFinal().getAno())//Significa que todos os anos do horizonte foram lidos
									break;

							}//while (true) {

						}//while (true) {

					}//else {

				}//else if (leitura_bloco_5 == true) {

				leituraArquivo.close();

			}//if(duracao_patamar_carga_variavel_por_ano == true){

			if (duracao_patamar_carga_variavel_por_ano == false) { //Patamar Sazonal

				if (horizonte_estudo.getIteradorInicial() != horizonte_estudo_DECK.getIteradorInicial())
					throw std::invalid_argument("Patamar Sazonal implementado somente para horizonte de estudo do DECK.");

				//*******************************************************************************************************************
				//Bloco 1 -   Este bloco é composto por um registro especificando o total de patamares de mercado
				//*******************************************************************************************************************

				//Registros destinados a comentários
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);

				std::getline(leituraArquivo, line);

				strNormalizada(line);

				//Número de patamares de déficit
				atributo = line.substr(1, 2);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				const int numero_patamares = atoi(atributo.c_str());

				//*******************************************************************************************************************
				//Bloco 2 -  Este bloco pode ser de dois tipos, conforme definido pelo registro 39 do arquivo de dados gerais. 
				//           Se esse registro for preenchido com o valor zero, o bloco será do Tipo 1, e se for preenchido 
				//           com o valor um, será do Tipo 2                     
				//*******************************************************************************************************************

				//Três registros destinados a comentários
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);

				///////////////////////////////////////////////////////////////
				//Inicializa percentual_duracao_patamar_carga com valores 0
				//para todos os submercados inicializados
				///////////////////////////////////////////////////////////////
				std::vector<double> vetor_zero(numero_patamares, 0.0);

				const SmartEnupla<IdPatamarCarga, double> vetor_patamar_zero(IdPatamarCarga_1, vetor_zero);

				SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_periodo_patamar_zero(horizonte_estudo, vetor_patamar_zero);

				const IdSubmercado idSubmercado_Maior = a_dados.vetorSubmercado.getMaiorId();

				a_dados.setMatriz(AttMatrizDados_percentual_duracao_patamar_carga, matriz_periodo_patamar_zero);

				////////////////

				////////////////////////////////////////
				// Registro Tipo 2
				////////////////////////////////////////

				//////////////////////////////////////////////////////////////////////////////////////
				//Campo 2-6 - Duração do X patamar em p.u. do mês 
				//////////////////////////////////////////////////////////////////////////////////////

				std::vector<std::vector<double>> valor_mes_patamar;
				std::vector<double> valor_patamar;

				for (int mes = 0; mes < 12; mes++) {

					std::getline(leituraArquivo, line);

					strNormalizada(line);

					for (int patamar = 0; patamar < numero_patamares; patamar++) {

						atributo = line.substr(6 + 8 * patamar, 6);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						valor_patamar.push_back(atof(atributo.c_str()));

					}//for (int patamar = 0; patamar < numero_patamares; patamar++) {

					valor_mes_patamar.push_back(valor_patamar);

					valor_patamar.clear();

				}//for (int mes = 0; mes < 12; mes++) {

				//////////////////////////
				//Inicializa parâmetros
				//////////////////////////

				const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();

				const Periodo periodo_final = horizonte_estudo.getIteradorFinal();

				for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

					int mes = atoi(getString(periodo.getMes()).c_str());

					for (int patamar = 0; patamar < numero_patamares; patamar++) {

						double percentual_duracao_patamar_carga = valor_mes_patamar.at(mes - 1).at(patamar);

						a_dados.setElemento(AttMatrizDados_percentual_duracao_patamar_carga, periodo, getIdPatamarCargaFromChar(std::to_string(patamar + 1).c_str()), percentual_duracao_patamar_carga);

					}//for (int patamar = 0; patamar < numero_patamares; patamar++) {

				}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {


				//*******************************************************************************************************************
				//Bloco 3 - Este bloco é composto por tantos conjuntos de registros quantos forem os subsistemas/submercados 
				//          e é precedido de um conjunto de três registros de existência obrigatória, destinado a comentários. 
				//          O código 9999 no campo 1 indica final do bloco. Para cada subsistema/submercado, os conjuntos 
				//          de registros podem ser ou do tipo 1 ou do tipo 2                    
				//*******************************************************************************************************************

				//Registros destinados a comentários
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);

				///////////////////////////////////////////////////////////////
				//Inicializa percentual_duracao_patamar_carga com valores 0
				//para todos os submercados inicializados
				///////////////////////////////////////////////////////////////

				for (IdSubmercado idSubmercado = IdSubmercado_1; idSubmercado <= idSubmercado_Maior; idSubmercado++)
					a_dados.vetorSubmercado.att(idSubmercado).setMatriz(AttMatrizSubmercado_percentual_variacao_patamar_carga, matriz_periodo_patamar_zero);

				////////////////

				while (true) {

					////////////////////////////////////////
					// Registro 1
					////////////////////////////////////////

					std::getline(leituraArquivo, line);

					strNormalizada(line);

					//Critério de parada
					atributo = line.substr(0, 4);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					if (atributo == "9999")
						break;

					//////////////////////////////////////////////////////////////////////////////////////
					//Campo 1 - Número do subsistema/submercado 
					//////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(1, 3);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const int submercado_codigo_usina = atoi(atributo.c_str());

					const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, submercado_codigo_usina);

					if (idSubmercado == IdSubmercado_Nenhum)
						throw std::invalid_argument("SISTEMA.DAT submercado nao instanciado, ver Bloco 4 Registro tipo 1 ");

					//////////////////////////////////////////////////////////////////////////////////////
					//Campo 1-12 - Fator que deve ser aplicado à demanda média para compor o mercado 
					//            do primeiro patamar do X mês do período de planejamento. 
					//////////////////////////////////////////////////////////////////////////////////////

					std::vector<std::vector<double>> valor_patamar_mes;
					std::vector<double> valor_mes;

					for (int patamar = 0; patamar < numero_patamares; patamar++) {

						std::getline(leituraArquivo, line);

						strNormalizada(line);

						for (int mes = 0; mes < 12; mes++) {

							atributo = line.substr(1 + 7 * mes, 6);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							valor_mes.push_back(atof(atributo.c_str()));

						}//for (int mes = 0; mes < 12; mes++) {

						valor_patamar_mes.push_back(valor_mes);

						valor_mes.clear();

					}//for (int patamar = 0; patamar < numero_patamares; patamar++) {

					const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();

					const Periodo periodo_final = horizonte_estudo.getIteradorFinal();

					for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

						int mes = atoi(getString(periodo.getMes()).c_str());

						for (int patamar = 0; patamar < numero_patamares; patamar++) {

							double percentual_variacao_patamar_carga = valor_patamar_mes.at(patamar).at(mes - 1);

							a_dados.vetorSubmercado.att(idSubmercado).setElemento(AttMatrizSubmercado_percentual_variacao_patamar_carga, periodo, getIdPatamarCargaFromChar(std::to_string(patamar + 1).c_str()), percentual_variacao_patamar_carga);

						}//for (int patamar = 0; patamar < numero_patamares; patamar++) {

					}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

				}//while (true) {

				//*******************************************************************************************************************
				//Bloco 4 - Este bloco é composto por tantos conjuntos de registros quantas forem as interligações entre os 
				//          subsistemas/submercados. É precedido de um conjunto de cinco registros de existência obrigatória, 
				//          destinado a orientar o usuário no preenchimento/alteração dos dados. . O código 9999 no campo 1 indica 
				//          final do bloco. Para cada interligação, os conjuntos de registros podem ser ou do tipo 1 ou do tipo 2.                  
				//*******************************************************************************************************************

				//Registros destinados a comentários
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);

				///////////////////////////////////////////////////////////////
				//Inicializa percentual_duracao_patamar_carga com valores 0
				//para todos os submercados inicializados
				///////////////////////////////////////////////////////////////

				const IdIntercambio idIntercambio_Maior = a_dados.vetorIntercambio.getMaiorId();

				for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= idIntercambio_Maior; idIntercambio++)
					a_dados.vetorIntercambio.att(idIntercambio).setMatriz(AttMatrizIntercambio_percentual_variacao_patamar_carga, matriz_periodo_patamar_zero);

				////////////////

				bool leitura_bloco_5 = true;

				while (true) {

					std::getline(leituraArquivo, line);

					strNormalizada(line);

					//Alguns Decks (p.ex. NW201907) não colocam o 9999 para significar o fim deste bloco

					if (int(line.length()) == 0) {//Significa que não existem mais linhas para ler 

						leitura_bloco_5 = false;

						break;

					}//if (int(line.length()) == 0) {

					atributo = line.substr(0, 4);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					if (atributo == "9999")
						break;

					//////////////////////////////////////////////////////////////////////////////////////
					//Campo 1 - Número do subsistema/submercado A 
					//////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(1, 3);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const int submercado_origem_codigo_usina = atoi(atributo.c_str());

					const IdSubmercado idSubmercado_origem = getIdFromCodigoONS(lista_codigo_ONS_submercado, submercado_origem_codigo_usina);

					if (idSubmercado_origem == IdSubmercado_Nenhum)
						throw std::invalid_argument("SISTEMA.DAT submercado nao instanciado, ver Bloco 4 Registro tipo 1 ");

					//////////////////////////////////////////////////////////////////////////////////////
					//Campo 2 - Número do subsistema/submercado B 
					//////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(5, 3);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const int submercado_destino_codigo_usina = atoi(atributo.c_str());

					const IdSubmercado idSubmercado_destino = getIdFromCodigoONS(lista_codigo_ONS_submercado, submercado_destino_codigo_usina);

					if (idSubmercado_destino == IdSubmercado_Nenhum)
						throw std::invalid_argument("SISTEMA.DAT submercado nao instanciado, ver Bloco 4 Registro tipo 1 ");


					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Determina o Intercambio
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					std::vector<IdIntercambio> idIntercambio_inicializado = a_dados.vetorIntercambio.getIdObjetos(AttComumIntercambio_submercado_origem, idSubmercado_origem);

					int idIntercambio_inicializado_size = int(idIntercambio_inicializado.size());

					IdIntercambio idIntercambio;

					for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

						idIntercambio = idIntercambio_inicializado.at(intercambio_inicializado);

						if (a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_destino)
							break;

						if (intercambio_inicializado + 1 == idIntercambio_inicializado_size)
							throw std::invalid_argument("Intercambio nao inicializado entre subsistema_" + getString(idSubmercado_origem) + "e subsistema_" + getString(idSubmercado_destino));

					}//for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

					/////////////////////////

					//////////////////////////////////////////////////////////////////////////////////////
					//Campo 1-12 - Fator que deve ser aplicado ao intercâmbio médio para compor o 
					//             intercâmbio do subsistema/submercado A para o subsistema/submercado B 
					//             do primeiro patamar do X mês do período de planejamento. 
					//////////////////////////////////////////////////////////////////////////////////////

					std::vector<std::vector<double>> valor_patamar_mes;
					std::vector<double> valor_mes;

					for (int patamar = 0; patamar < numero_patamares; patamar++) {

						std::getline(leituraArquivo, line);

						strNormalizada(line);

						for (int mes = 0; mes < 12; mes++) {

							atributo = line.substr(1 + 7 * mes, 6);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							valor_mes.push_back(atof(atributo.c_str()));

						}//for (int mes = 0; mes < 12; mes++) {

						valor_patamar_mes.push_back(valor_mes);

						valor_mes.clear();

					}//for (int patamar = 0; patamar < numero_patamares; patamar++) {

					const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();

					const Periodo periodo_final = horizonte_estudo.getIteradorFinal();

					for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

						int mes = atoi(getString(periodo.getMes()).c_str());

						for (int patamar = 0; patamar < numero_patamares; patamar++) {

							double percentual_variacao_patamar_carga = valor_patamar_mes.at(patamar).at(mes - 1);

							a_dados.vetorIntercambio.att(idIntercambio).setElemento(AttMatrizIntercambio_percentual_variacao_patamar_carga, periodo, getIdPatamarCargaFromChar(std::to_string(patamar + 1).c_str()), percentual_variacao_patamar_carga);

						}//for (int patamar = 0; patamar < numero_patamares; patamar++) {

					}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

				}//while (true) {


				//*******************************************************************************************************************
				//Bloco 5 - Este bloco é composto no máximo por tantos conjuntos de registros quantas forem
				//os blocos de usinas não simuladas.É precedido de um conjunto de quatro registros
				//de existência obrigatória, destinado a orientar o usuário no preenchimento / alteração
				//dos dados..O código 9999 no campo 1 indica final do bloco.Para cada bloco de
				//usinas não simuladas, os conjuntos de registros podem ser ou do tipo 1 ou do tipo 2.
				//*******************************************************************************************************************

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Manual: Para os blocos de usinas não simuladas definidos no arquivo de dados dos subsistemas/submercados (item 3.6)
				//        e não informados no arquivo de patamares, serão considerados fatores unitários ao longo de todo horizonte 
				//        de estudo para todos os patamares de carga.
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				if (leitura_bloco_5 == false) {

					///////////////////////////////////////////////////////////////
					//Inicializa percentual_variacao_patamar_carga com valores 1
					//para todos os submercados inicializados
					///////////////////////////////////////////////////////////////
					std::vector<double> vetor_ones(numero_patamares, 1.0);

					const SmartEnupla<IdPatamarCarga, double> vetor_usinaNaoSimulada_percentual_variacao_patamar_carga(IdPatamarCarga_1, vetor_ones);

					SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_usinaNaoSimulada_percentual_variacao_patamar_carga(horizonte_estudo, vetor_usinaNaoSimulada_percentual_variacao_patamar_carga);

					const IdSubmercado idSubmercado_Maior = a_dados.vetorSubmercado.getMaiorId();

					for (IdSubmercado idSubmercado = IdSubmercado_1; idSubmercado <= idSubmercado_Maior; idSubmercado++) {

						if (a_dados.vetorSubmercado.att(idSubmercado).getAtributo(AttComumSubmercado_ficticio, bool()) == false) {

							const IdUsinaNaoSimulada maiorIdUsinaNaoSimulada = a_dados.vetorSubmercado.att(idSubmercado).getMaiorId(IdUsinaNaoSimulada());

							for (IdUsinaNaoSimulada idUsinaNaoSimulada = IdUsinaNaoSimulada_1; idUsinaNaoSimulada <= maiorIdUsinaNaoSimulada; idUsinaNaoSimulada++)
								a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).setMatriz(AttMatrizUsinaNaoSimulada_percentual_variacao_patamar_carga, matriz_usinaNaoSimulada_percentual_variacao_patamar_carga);

						}//if (a_dados.vetorSubmercado.att(idSubmercado).getAtributo(AttComumSubmercado_ficticio, bool()) == false) {

					}//for (IdSubmercado idSubmercado = IdSubmercado_1; idSubmercado <= idSubmercado_Maior; idSubmercado++) {

				}//if (leitura_bloco_5 == false) {
				else if (leitura_bloco_5 == true) {

					std::getline(leituraArquivo, line);

					strNormalizada(line);

					if (int(line.length()) == 0) {//Significa que não existem mais linhas para ler 

						///////////////////////////////////////////////////////////////
						//Inicializa percentual_variacao_patamar_carga com valores 1
						//para todos os submercados inicializados
						///////////////////////////////////////////////////////////////
						std::vector<double> vetor_ones(numero_patamares, 1.0);

						const SmartEnupla<IdPatamarCarga, double> vetor_usinaNaoSimulada_percentual_variacao_patamar_carga(IdPatamarCarga_1, vetor_ones);

						SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_usinaNaoSimulada_percentual_variacao_patamar_carga(horizonte_estudo, vetor_usinaNaoSimulada_percentual_variacao_patamar_carga);

						const IdSubmercado idSubmercado_Maior = a_dados.vetorSubmercado.getMaiorId();

						for (IdSubmercado idSubmercado = IdSubmercado_1; idSubmercado <= idSubmercado_Maior; idSubmercado++) {

							if (a_dados.vetorSubmercado.att(idSubmercado).getAtributo(AttComumSubmercado_ficticio, bool()) == false) {

								const IdUsinaNaoSimulada maiorIdUsinaNaoSimulada = a_dados.vetorSubmercado.att(idSubmercado).getMaiorId(IdUsinaNaoSimulada());

								for (IdUsinaNaoSimulada idUsinaNaoSimulada = IdUsinaNaoSimulada_1; idUsinaNaoSimulada <= maiorIdUsinaNaoSimulada; idUsinaNaoSimulada++)
									a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).setMatriz(AttMatrizUsinaNaoSimulada_percentual_variacao_patamar_carga, matriz_usinaNaoSimulada_percentual_variacao_patamar_carga);

							}//if (a_dados.vetorSubmercado.att(idSubmercado).getAtributo(AttComumSubmercado_ficticio, bool()) == false) {

						}//for (IdSubmercado idSubmercado = IdSubmercado_1; idSubmercado <= idSubmercado_Maior; idSubmercado++) {

					}//if (int(line.length()) == 0) {
					else {

						//Registros destinados a comentários
						std::getline(leituraArquivo, line);
						std::getline(leituraArquivo, line);
						std::getline(leituraArquivo, line);

						///////////////////////////////////////////////////////////////
						//Inicializa percentual_duracao_patamar_carga com valores 0
						//para todos os submercados inicializados
						///////////////////////////////////////////////////////////////

						for (IdSubmercado idSubmercado = IdSubmercado_1; idSubmercado <= idSubmercado_Maior; idSubmercado++) {

							const IdUsinaNaoSimulada maiorIdUsinaNaoSimulada = a_dados.vetorSubmercado.att(idSubmercado).getMaiorId(IdUsinaNaoSimulada());

							for (IdUsinaNaoSimulada idUsinaNaoSimulada = IdUsinaNaoSimulada_1; idUsinaNaoSimulada <= maiorIdUsinaNaoSimulada; idUsinaNaoSimulada++)
								a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).setMatriz(AttMatrizUsinaNaoSimulada_percentual_variacao_patamar_carga, matriz_periodo_patamar_zero);

						}//for (IdSubmercado idSubmercado = IdSubmercado_1; idSubmercado <= idSubmercado_Maior; idSubmercado++) {
						////////////////

						while (true) {

							std::getline(leituraArquivo, line);

							strNormalizada(line);

							//Alguns Decks (p.ex. NW201907) não colocam o 9999 para significar o fim deste bloco

							if (int(line.length()) == 0)//Significa que não existem mais linhas para ler 
								break;

							atributo = line.substr(0, 4);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							if (atributo == "9999")
								break;

							//////////////////////////////////////////////////////////////////////////////////////
							//Campo 1 - Número do subsistema/submercado 
							//////////////////////////////////////////////////////////////////////////////////////

							atributo = line.substr(1, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int submercado_codigo_usina = atoi(atributo.c_str());

							const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, submercado_codigo_usina);

							if (idSubmercado == IdSubmercado_Nenhum)
								throw std::invalid_argument("SISTEMA.DAT submercado nao instanciado, ver Bloco 4 Registro tipo 1 ");

							//////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 - Número do bloco de usinas não simuladas
							//////////////////////////////////////////////////////////////////////////////////////

							IdUsinaNaoSimulada idUsinaNaoSimulada = IdUsinaNaoSimulada_1; //Padrao se a informaçao nao é dada desagregada

							if (line.size() > 4) {

								atributo = line.substr(5, (line.length() - 4));
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								if (atributo != "")
									idUsinaNaoSimulada = IdUsinaNaoSimulada(atoi(atributo.c_str()));

							}//if (line.size() >= 8) {


							/////////////////////////

							//////////////////////////////////////////////////////////////////////////////////////
							//Campo 1-12 - Fator que deve ser aplicado ao bloco de usinas não simuladas para 
							//   compor sua geração no X patamar do X mês do período de planejamento
							//////////////////////////////////////////////////////////////////////////////////////

							std::vector<std::vector<double>> valor_patamar_mes;
							std::vector<double> valor_mes;

							for (int patamar = 0; patamar < numero_patamares; patamar++) {

								std::getline(leituraArquivo, line);

								strNormalizada(line);

								for (int mes = 0; mes < 12; mes++) {

									atributo = line.substr(1 + 7 * mes, 6);
									atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

									valor_mes.push_back(atof(atributo.c_str()));

								}//for (int mes = 0; mes < 12; mes++) {

								valor_patamar_mes.push_back(valor_mes);

								valor_mes.clear();

							}//for (int patamar = 0; patamar < numero_patamares; patamar++) {

							const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();

							const Periodo periodo_final = horizonte_estudo.getIteradorFinal();

							for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

								int mes = atoi(getString(periodo.getMes()).c_str());

								for (int patamar = 0; patamar < numero_patamares; patamar++) {

									double percentual_variacao_patamar_carga = valor_patamar_mes.at(patamar).at(mes - 1);

									a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).setElemento(AttMatrizUsinaNaoSimulada_percentual_variacao_patamar_carga, periodo, getIdPatamarCargaFromChar(std::to_string(patamar + 1).c_str()), percentual_variacao_patamar_carga);

								}//for (int patamar = 0; patamar < numero_patamares; patamar++) {

							}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

						}//while (true) {

					}//else {

				}//else if (leitura_bloco_5 == true) {

				leituraArquivo.close();

			}//if(duracao_patamar_carga_variavel_por_ano == false){

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//try {
	catch (const std::exception & erro) { throw std::invalid_argument("LeituraCEPEL::leitura_NEWAVE_26_PATAMAR: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_MANUTT_201908_NW25(Dados& a_dados, std::string nomeArquivo) {

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;

		//Horizonte de estudo
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		////////////////////////////////////////////////////////

		if (leituraArquivo.is_open()) {

			double sobreposicao_atraso_periodo_inicial = 0.0;

			if (desconsiderar_atraso_periodo_estudo_inicial)
				sobreposicao_atraso_periodo_inicial = horizonte_estudo.getIteradorInicial().atraso(horizonte_estudo_DECK.getIteradorInicial());

			//Pula o cabeçalho
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Campo 1 -  Número da usina térmica
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				atributo = line.substr(17, 3);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				const int codigo_usina = atoi(atributo.c_str());

				const IdTermeletrica idTermeletrica = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, codigo_usina);

				if (idTermeletrica == IdTermeletrica_Nenhum)
					throw std::invalid_argument("Nao foi possivel encontrar a termeletrica de codigo " + getFullString(codigo_usina) + ".");

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Campo 2-3-4 -  Data de início da manutenção (ddmmaaaa). A data deve pertencer
				//               ao primeiro ou ao segundo ano do planejamento
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				//Dia

				atributo = line.substr(40, 2);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				const IdDia dia_inicial = getIdDiaFromChar(atributo.c_str());

				//Mês

				atributo = line.substr(42, 2);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				const IdMes mes_inicial = getIdMesFromChar(atributo.c_str());

				//Ano

				atributo = line.substr(44, 4);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				const IdAno ano_inicial = getIdAnoFromChar(atributo.c_str());

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Campo 5 -  Duração da manutenção, em dias
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				atributo = line.substr(49, 3);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				const int duracao_manutencao_em_dias = atoi(atributo.c_str());

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Campo 6 -  Potência da unidade em manutenção (MW). 
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				atributo = line.substr(55, 7);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				const double potencia_unidade_em_manutencao = atof(atributo.c_str());

				std::vector<Periodo> periodos_em_manutencao;

				if (true) {

					const int numero_periodos_60dias_cheios_manutencao = duracao_manutencao_em_dias / 60;
					const int duracao_manutencao_em_dias_restantes     = duracao_manutencao_em_dias % 60;

					for (int p = 0; p < numero_periodos_60dias_cheios_manutencao; p++) {
						if (p == 0)
							periodos_em_manutencao.push_back(Periodo(TipoPeriodo_60dias, dia_inicial, mes_inicial, ano_inicial));
						else
							periodos_em_manutencao.push_back(periodos_em_manutencao.at(p - 1) + 1);
					} // for (int p = 0; p < numero_periodos_60dias_cheios_manutencao; p++) {

					if (duracao_manutencao_em_dias_restantes > 0) {
						if (periodos_em_manutencao.size() == 0)
							periodos_em_manutencao.push_back(Periodo(TipoPeriodo(63 - duracao_manutencao_em_dias_restantes), dia_inicial, mes_inicial, ano_inicial));
						else
							periodos_em_manutencao.push_back(Periodo(TipoPeriodo(63 - duracao_manutencao_em_dias_restantes), periodos_em_manutencao.at(periodos_em_manutencao.size() - 1) + 1));
					} // if (duracao_manutencao_em_dias_restantes > 0) {

				} // if (true) {

				//*******************************************************************************************************************
				//Cálculo da indisponibilidade programada mensal em porcentagem
				//*******************************************************************************************************************

				//1. Determina o periodo ao qual pertece a data de início da manutenção informada


				const Periodo periodo_manutencao_inicial = periodos_em_manutencao.at(0);
				const Periodo periodo_manutencao_final   = periodos_em_manutencao.at(periodos_em_manutencao.size() - 1);

				const Periodo periodo_pos_manutencao = Periodo(IdMes_1, IdAno(horizonte_estudo_DECK.getIteradorInicial().getAno() + numero_anos_manutencao_termica_programada));

				for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

					if (periodo < periodo_pos_manutencao) {

						double sobreposicao_inicial = periodo.sobreposicao(periodo_manutencao_inicial);
						
						if ((periodo == horizonte_estudo.getIteradorInicial()) && (Periodo(horizonte_estudo_DECK.getIteradorInicial().getTipoPeriodo(), periodo_manutencao_inicial) == horizonte_estudo_DECK.getIteradorInicial()))
							sobreposicao_inicial += sobreposicao_atraso_periodo_inicial;

						const double sobreposicao_final = periodo.sobreposicao(periodo_manutencao_final);

						double sobreposicao = sobreposicao_inicial + sobreposicao_final;
						if (periodo_manutencao_inicial == periodo_manutencao_final)
							sobreposicao = sobreposicao_inicial;

						if (((periodo >= periodo_manutencao_inicial) && (periodo <= periodo_manutencao_final)) || (sobreposicao > 0.0)) {

							if (sobreposicao == 0.0)
								sobreposicao = 1.0;

							double potencia_maxima = 0.0;
							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++)
								potencia_maxima += a_dados.getElementoMatriz(idTermeletrica, IdUnidadeUTE_1, AttMatrizUnidadeUTE_potencia_maxima, periodo, idPatamarCarga, double()) * a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());

							if ((potencia_maxima == 0.0) && (potencia_unidade_em_manutencao > 0.0))
								throw std::invalid_argument("Erro no calculo de indisponibilidade programada de " + getFullString(idTermeletrica) + ".");

							const double valor_antigo = a_dados.getElementoVetor(idTermeletrica, AttVetorTermeletrica_indisponibilidade_programada, periodo, double());

							const double indisponibilidade_programada = valor_antigo + sobreposicao * (potencia_unidade_em_manutencao / potencia_maxima);

							a_dados.vetorTermeletrica.att(idTermeletrica).setElemento(AttVetorTermeletrica_indisponibilidade_programada, periodo, indisponibilidade_programada);

						} // if (((periodo >= periodo_manutencao_inicial) && (periodo <= periodo_manutencao_final)) || (sobreposicao > 0.0)) {

					} // if (periodo < periodo_pos_manutencao) {
					else
						break;

				} // for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

			} // while (std::getline(leituraArquivo, line)) {

			leituraArquivo.close();

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//try {
	catch (const std::exception & erro) { throw std::invalid_argument("LeituraCEPEL::leitura_NEWAVE_26_MANUTT: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_VAZPAST_201908_NW25(Dados& a_dados, std::string nomeArquivo) {

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;

		//Horizonte de estudo
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		////////////////////////////////////////////////////////


		if ((hidreletricasPreConfig_instanciadas) && (!tendenciaPreConfig_instanciadas))
			throw std::invalid_argument("Necessario informar tendencia hidrologica para as hidreletricas da pre configuracao.");

		else if ((hidreletricasPreConfig_instanciadas) && (tendenciaPreConfig_instanciadas))
			return;

		if (leituraArquivo.is_open()) {

			//Pula o cabeçalho
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Campo 1 -  Número do posto
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				atributo = line.substr(2, 3);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				const int posto = atoi(atributo.c_str());

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Campo 2 -  Nome da usina 
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				atributo = line.substr(6, 12);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				std::vector<IdHidreletrica> idIdHidreletrica_posto = a_dados.vetorHidreletrica.getIdObjetos(AttComumHidreletrica_codigo_posto, posto);

				IdHidreletrica idHidreletrica = IdHidreletrica_Nenhum;

				if ((int(idIdHidreletrica_posto.size()) > 1) && (!hidreletricasPreConfig_instanciadas))
					throw std::invalid_argument("Identificadas varias idHidreletricas com posto_ " + getString(posto));
				/*else if ((int(idIdHidreletrica_posto.size()) == 0) && (!hidreletricasPreConfig_instanciadas))
					throw std::invalid_argument("Nao existe idHidreletrica com posto_ " + getString(posto));
			*/	else if (int(idIdHidreletrica_posto.size()) == 1)
					idHidreletrica = idIdHidreletrica_posto.at(0);

				if (idHidreletrica != IdHidreletrica_Nenhum) { // No arquivo de vazões passadas podem existir usinas que não são consideradas no estudo (Arquivo CONFHD.DAT)

					const int codigo_usina = lista_codigo_ONS_hidreletrica.at(idHidreletrica);

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 2 -  Nome do posto (não lido pelo programa) 
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(6, 12);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 3-14 -  Vazão afluente no mês X, em m3/s 
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					SmartEnupla<IdMes, double> afluencia_tendencia_mensal(12);

					for (IdMes idMes = IdMes_1; idMes <= IdMes_12; idMes++) {

						atributo = line.substr(18 + int(idMes - 1) * 10, 9);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						const double afluencia = atof(atributo.c_str());

						afluencia_tendencia_mensal.addElemento(idMes, afluencia);

					} // for (IdMes idMes = IdMes_1; idMes <= IdMes_12; idMes++) {

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Guarda informação nos SmartElementos 
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					if (!a_dados.vetorHidreletrica.att(idHidreletrica).vetorAfluencia.isInstanciado(IdAfluencia_vazao_afluente)) {
						Afluencia afluencia;
						afluencia.setAtributo(AttComumAfluencia_idAfluencia, IdAfluencia_vazao_afluente);
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorAfluencia.add(afluencia);
					} // if (!a_dados.vetorHidreletrica.att(idHidreletrica).vetorAfluencia.isInstanciado(IdAfluencia_vazao_afluente)) {

					////////////////////////////////////////////////////////////////////////
					//Identifica o mês e ano do periodo onde começa a tendência
					////////////////////////////////////////////////////////////////////////

					const Periodo periodo_final_tendencia = horizonte_estudo.getIteradorInicial() - 1;
					const Periodo periodo_inicial_tendencia = periodo_final_tendencia - 11;

					try {

						for (Periodo periodo = periodo_inicial_tendencia; periodo <= periodo_final_tendencia; periodo++)
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).addElemento(AttMatrizAfluencia_natural_tendencia, IdCenario_1, periodo, afluencia_tendencia_mensal.at(periodo.getMes()));

					}
					catch (const std::exception & erro) { 
						throw std::invalid_argument(getFullString(idHidreletrica) + ": \n" + std::string(erro.what()));
					}

				} // if (idHidreletrica_inicializado != IdHidreletrica_Nenhum) {

			} // while(std::getline(leituraArquivo, line)) {

			/*
			for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica++) {

				if (a_dados.getSize1Matriz(idHidreletrica, IdAfluencia_vazao_afluente, AttMatrizAfluencia_natural_tendencia) == 0) {

					const Periodo periodo_final_tendencia = horizonte_estudo.getIteradorInicial() - 1;
					const Periodo periodo_inicial_tendencia = periodo_final_tendencia - 11;

					for (Periodo periodo = periodo_inicial_tendencia; periodo <= periodo_final_tendencia; periodo++)
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).addElemento(AttMatrizAfluencia_natural_tendencia, IdCenario_1, periodo, a_dados.getElementoVetor(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_natural_historico, periodo, double()));
					
				} // if (a_dados.getSize1Matriz(idHidreletrica, IdAfluencia_vazao_afluente, AttMatrizAfluencia_natural_tendencia) == 0) {

			} // for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica++) {
			*/

			leituraArquivo.close();

			a_dados.setAtributo(AttComumDados_tipo_tendencia_hidrologica, TipoTendenciaEstocastica_serie_informada);

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_NEWAVE_26_VAZPAST: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_C_ADIC_201908_NW25(Dados& a_dados, std::string nomeArquivo) {

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;

		//Horizonte de estudo
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		////////////////////////////////////////////////////////

		if (considera_cargas_adicionais == true) {//Parâmetro determinado no arquivo DGER.DAT

			double sobreposicao_atraso_periodo_inicial = 0.0;

			if (desconsiderar_atraso_periodo_estudo_inicial)
				sobreposicao_atraso_periodo_inicial = horizonte_estudo.getIteradorInicial().atraso(horizonte_estudo_DECK.getIteradorInicial());

			SmartEnupla<IdSubmercado, SmartEnupla<Periodo, double>> lista_C_ADIC(IdSubmercado_1, std::vector<SmartEnupla<Periodo, double>>(a_dados.getMaiorId(IdSubmercado()), SmartEnupla<Periodo, double>()));

			if (leituraArquivo.is_open()) {

				//Pula o cabeçalho
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);

				while (true) {

					////////////////////////////////////////
					// Registro Tipo 1
					////////////////////////////////////////

					while (true) {

						std::getline(leituraArquivo, line);

						strNormalizada(line);

						atributo = line.substr(1, 3);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						if (atoi(atributo.c_str()) > 0)//Evita POS ou PRE já que o atoi retorna 0 se for uma string
							break;

					}//while (true) {

					//Número do subsistema/submercado
					atributo = line.substr(1, 3);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					if (atributo == "999") //Parada da leitura do bloco 3
						break;

					const int submercado_codigo_usina = atoi(atributo.c_str());

					const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, submercado_codigo_usina);

					if (idSubmercado == IdSubmercado_Nenhum)
						throw std::invalid_argument("SISTEMA.DAT submercado nao instanciado, ver Bloco 4 Registro tipo 1 ");

					if (lista_C_ADIC.at(idSubmercado).size() == 0)
						lista_C_ADIC.at(idSubmercado) = SmartEnupla<Periodo, double>(horizonte_estudo, 0.0);

					////////////////////////////////////////
					// Registro Tipo 2
					////////////////////////////////////////

					std::getline(leituraArquivo, line);

					strNormalizada(line);

					Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial();

					while (true) {

						atributo = line.substr(7 + 8 * (int(periodo_DECK.getMes()) - 1), 7);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						const double carga_adicional = atof(atributo.c_str());

						for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							double sobreposicao = periodo.sobreposicao(periodo_DECK);

							if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
								sobreposicao += sobreposicao_atraso_periodo_inicial;

							if (sobreposicao > 0.0)
								lista_C_ADIC.at(idSubmercado).at(periodo) += sobreposicao * carga_adicional;

							if ((sobreposicao == 0.0) && (periodo > periodo_DECK))
								break;

						} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						//Critério de parada

						if (periodo_DECK == horizonte_estudo_DECK.getIteradorFinal())
							break;

						//Passo de linha e atualização do periodo

						if (periodo_DECK.getMes() == IdMes_12) {

							std::getline(leituraArquivo, line);

							strNormalizada(line);

						}// if (periodo_DECK.getMes() == IdMes_12) {

						periodo_DECK++;

					}//while (true) {

					//Registros tipo 3 e 4 (periodos estáticos) não são considerados

				}//while (true) {

				leituraArquivo.close();

				for (IdSubmercado idSubmercado = IdSubmercado_1; idSubmercado <= a_dados.getMaiorId(IdSubmercado()); idSubmercado++) {

					if (lista_C_ADIC.at(idSubmercado).size() > 0) {

						for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							double demanda_bruta = a_dados.getElementoVetor(idSubmercado, AttVetorSubmercado_demanda, periodo, double());

							demanda_bruta += lista_C_ADIC.at(idSubmercado).at(periodo);

							a_dados.vetorSubmercado.att(idSubmercado).setElemento(AttVetorSubmercado_demanda, periodo, demanda_bruta);

						} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

					} // if (lista_C_ADIC.at(idSubmercado).size() > 0) {
				} // for (IdSubmercado idSubmercado = IdSubmercado_1; idSubmercado <= a_dados.getMaiorId(IdSubmercado()); idSubmercado++) {

			}//if (leituraArquivo.is_open()) {
			else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

		}//if (considera_cargas_adicionais == true) {

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_NEWAVE_26_C_ADIC: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_GTMINPAT_201908_NW25(Dados &a_dados, std::string nomeArquivo) {

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;

		////////////////////////////////////////////////////////

		if (leituraArquivo.is_open()) {

			//Pula o cabeçalho
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);

				throw std::invalid_argument("O modelo SPT nao esta preparado para ler esta estrutura de arquivo - Atualizar metodo");

			}//while (std::getline(leituraArquivo, line)) {

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_NEWAVE_26_GTMINPAT: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_LOSS_201908_NW25(Dados &a_dados, std::string nomeArquivo) {

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;

		////////////////////////////////////////////////////////

		if (considera_perdas_transmissao == true) {

			if (leituraArquivo.is_open()) {

				//Pula o cabeçalho
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);

				while (std::getline(leituraArquivo, line)) {

					strNormalizada(line);

					throw std::invalid_argument("O modelo SPT nao esta preparado para ler esta estrutura de arquivo - Atualizar metodo");

				}//while (std::getline(leituraArquivo, line)) {

			}//if (leituraArquivo.is_open()) {
			else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

		}//if (considera_perdas_transmissao == true) {

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_NEWAVE_26_LOSS: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_DSVAGUA_201908_NW25(Dados& a_dados, std::string nomeArquivo) {

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;

		//Horizonte de estudo
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());


		//Inicializa vetor desvio

		if (leituraArquivo.is_open()) {

			SmartEnupla<IdHidreletrica, SmartEnupla<Periodo, double>> lista_DESVAGUA(IdHidreletrica_1, std::vector<SmartEnupla<Periodo, double>>(a_dados.getMaiorId(IdHidreletrica()), SmartEnupla<Periodo, double>()));

			//Pula o cabeçalho
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);

			while (true) {

				std::getline(leituraArquivo, line);

				strNormalizada(line);

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Campo 1 -  Ano do período de planejamento
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				atributo = line.substr(0, 4);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				if (atributo == "9999")
					break;

				const IdAno ano = getIdAnoFromChar(atributo.c_str());

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Campo 2 -  Número da usina hidroelétrica
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				atributo = line.substr(5, 4);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				const int codigo_usina = atoi(atributo.c_str());

				const IdHidreletrica idHidreletrica_inicializada = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

				IdHidreletrica idHidreletrica = IdHidreletrica_Nenhum;

				if (idHidreletrica_inicializada != IdHidreletrica_Nenhum)
					idHidreletrica = idHidreletrica_inicializada;

				// Identifica se hidrelétrica possui jusante instanciada para receber os valores de desvio d'agua.

				else if (idHidreletrica_inicializada == IdHidreletrica_Nenhum) {

					int codigo_usina_novo = codigo_usina;

					for (int i = 1; i <= lista_codigo_ONS_hidreletrica_agregada.size(); i++) {
						if (lista_codigo_ONS_hidreletrica_agregada.getElemento(i) == codigo_usina) {
							codigo_usina_novo = lista_codigo_ONS_hidreletricas_desagregadas.at(i).getElemento(1);
							break;
						}
					}

					if (codigo_usina_novo != codigo_usina)
						idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina_novo);

					else if (codigo_usina_novo == codigo_usina) {

						int indice_jusante_original = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica_original, codigo_usina_novo);

						int codigo_usina_jusante_original = 0;

						while (true) {

							codigo_usina_jusante_original = lista_codigo_ONS_hidreletrica_jusante_original.at(indice_jusante_original);

							if (codigo_usina_jusante_original == 0)
								break;

							idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina_jusante_original);

							if (idHidreletrica != IdHidreletrica_Nenhum)
								break;

							indice_jusante_original = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica_original, codigo_usina_jusante_original);

						} // while (true) {

					} // else if (codigo_usina_novo == codigo_usina) {

				} // else if (idHidreletrica_inicializada == IdHidreletrica_Nenhum) {


				if (idHidreletrica != IdHidreletrica_Nenhum) {

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					// Campo 15 -  Flag para a consideração do registro de desvio, caso a usina seja NC 
					//            0 = o registro é ignorado               
					//            1 = a informação contida no registro passa automaticamente para a usina de jusante
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(97, 4);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					if (lista_DESVAGUA.at(idHidreletrica).size() == 0)
						lista_DESVAGUA.at(idHidreletrica) = SmartEnupla<Periodo, double>(horizonte_estudo_DECK, 0.0);

					////////////////////////////////////////////////////////////////////

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					// Campo 3-14 -  Vazão adicionada (positivo) ou desviada (negativo) no mês X do ano correspondente
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					// Determina o mês inicial

					IdMes mes_inicial;

					if (ano == horizonte_estudo_DECK.getIteradorInicial().getAno())
						mes_inicial = horizonte_estudo_DECK.getIteradorInicial().getMes();
					else
						mes_inicial = IdMes_1;

					for (IdMes mes = mes_inicial; mes <= IdMes_12; mes++) {

						Periodo periodo_deck(mes, ano);

						atributo = line.substr(9 + 7 * (int(mes) - 1), 7);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						const double desvio_agua_novo = -atof(atributo.c_str());

						lista_DESVAGUA.at(idHidreletrica).at(periodo_deck) += desvio_agua_novo;

					} // for (int mes = 0; mes < 12; mes++) {

				} // if (idHidreletrica != IdHidreletrica_Nenhum) {

			} // while(true) {

			leituraArquivo.close();

			double sobreposicao_atraso_periodo_inicial = 0.0;

			if (desconsiderar_atraso_periodo_estudo_inicial)
				sobreposicao_atraso_periodo_inicial = horizonte_estudo.getIteradorInicial().atraso(horizonte_estudo_DECK.getIteradorInicial());

			for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= lista_DESVAGUA.getIteradorFinal(); idHidreletrica++) {

				if (lista_DESVAGUA.at(idHidreletrica).size() > 0) {

					if (a_dados.getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_retirada) == 0)
						a_dados.vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_vazao_retirada, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

					for (Periodo periodo_DECK = lista_DESVAGUA.at(idHidreletrica).getIteradorInicial(); periodo_DECK <= lista_DESVAGUA.at(idHidreletrica).getIteradorFinal(); periodo_DECK++) {

						for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							double sobreposicao = periodo.sobreposicao(periodo_DECK);

							if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
								sobreposicao += sobreposicao_atraso_periodo_inicial;

							if (sobreposicao > 0.0) {

								const double valor_antigo_retirada = a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_retirada, periodo, double());

								a_dados.vetorHidreletrica.att(idHidreletrica).setElemento(AttVetorHidreletrica_vazao_retirada, periodo, valor_antigo_retirada + sobreposicao * lista_DESVAGUA.at(idHidreletrica).at(periodo_DECK));

							} // if (sobreposicao > 0.0) {

						} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

					} // for (Periodo periodo_DECK = lista_DESVAGUA.at(idHidreletrica).getIteradorInicial(); periodo_DECK <= lista_DESVAGUA.at(idHidreletrica).getIteradorFinal(); periodo_DECK++) {

				} // if (lista_DESVAGUA.at(idHidreletrica).size() > 0) {

			} // for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= lista_DESVAGUA.getIteradorFinal(); idHidreletrica++) {

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_DSVAGUA_201908_NW25: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_PENALID_201908_NW25(Dados& a_dados, std::string nomeArquivo) {

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;

		////////////////////////////////////////////////////////

		if (leituraArquivo.is_open()) {

			//Pula o cabeçalho
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Campo 1 -  Palavra chave que define qual variável de folga estará sendo criada
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				atributo = line.substr(1, 6);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				const string menemonico = atributo;

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Campo 2 -  Penalidade aplicada à utilização da variável de folga
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				atributo = line.substr(14, 8);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				const double penalidade = atof(atributo.c_str());

				/////////////////////////////////////////////////

				const int line_size = int(line.length());


				if (line_size >= 32) {

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 3 -  Penalidade aplicada à utilização da variável de folga no segundo patamar 
					//           (aplicável somente para restrição de vazão mínima, quando essa é por patamares). 
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					//Nesta versão não vai ser considerada, se existir um valor, o SPT emite um aviso

					atributo = line.substr(24, 8);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					if (atributo != "") {

						try {
							std::ofstream escritaStream;
							escritaStream.open(SPT_avisos, std::ios_base::app);
							if (!escritaStream.is_open())
								throw std::invalid_argument("Nao foi possivel abrir o arquivo.");
							escritaStream << "Modelo NEWAVE utiliza penalidade no segundo patamar para a restricao de vazao minima" << std::endl;
							escritaStream.close();
							escritaStream.clear();
						} // try
						catch (const std::ofstream::failure& erro) { throw std::invalid_argument("Erro de integridade do arquivo " + SPT_avisos + ": \n" + std::string(erro.what())); }
						catch (const std::exception&         erro) { throw std::invalid_argument("Erro ao editar o arquivo " + SPT_avisos + ": \n" + std::string(erro.what())); }

					}//if (atributo != "") {

				}//if (line_size >= 32) {

				/////////////////////////////////////////////

				int subsistema;

				bool subsistema_reportado = false;

				if (line_size >= 39) {

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 4 -  Número do REE (ou subsistema/submercado, no caso de intercâmbio mínimo). 
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(36, 3);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					if (atributo != "") {

						subsistema_reportado = true;
						subsistema = atoi(atributo.c_str());

					}//if (atributo != "") {				

				}//if (line_size >= 39) {

				/////////////////////////////////////////////

				if (line_size >= 44) {

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 5 -  Número do patamar de carga  
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					//Nesta versão não vai ser considerada, se existir um valor, o SPT emite um aviso

					atributo = line.substr(42, 2);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					if (atributo != "") {

						try {
							std::ofstream escritaStream;
							escritaStream.open(SPT_avisos, std::ios_base::app);
							if (!escritaStream.is_open())
								throw std::invalid_argument("Nao foi possivel abrir o arquivo.");
							escritaStream << "Modelo NEWAVE utiliza penalidade diferente por patamar, o SPT considera o mesmo valor para todos os patamares de carga" << std::endl;
							escritaStream.close();
							escritaStream.clear();
						} // try
						catch (const std::ofstream::failure& erro) { throw std::invalid_argument("Erro de integridade do arquivo " + SPT_avisos + ": \n" + std::string(erro.what())); }
						catch (const std::exception&         erro) { throw std::invalid_argument("Erro ao editar o arquivo " + SPT_avisos + ": \n" + std::string(erro.what())); }

					}//if (atributo != "") {

				}//if (line_size >= 44) {

				//******************************************
				//Guarda informação nos SmartElementos
				//******************************************

				if (menemonico == "DESVIO" || menemonico == "desvio") {

					if (subsistema_reportado == true) {

						//O subsistema para as hidrelétricas refere-se ao REE associado (ver arquivo: CONFHD.DAT)

						for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica++) {
							if (lista_codigo_ONS_REE.at(idHidreletrica) == subsistema) {
								a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_penalidade_desvio_agua, penalidade);
								a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_penalidade_afluencia_incremental, penalidade);
							} // if (lista_codigo_ONS_REE.at(idHidreletrica) == subsistema) {
						} // for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica++) {	

					}//if (subsistema_reportado == true) {

					else if (subsistema_reportado == false) {

						//O valor de penalidade aplica para todas as usinas

						const IdHidreletrica idHidreletrica_maiorId = a_dados.vetorHidreletrica.getMaiorId();

						for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= idHidreletrica_maiorId; idHidreletrica++) {
							a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_penalidade_desvio_agua, penalidade);
							a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_penalidade_afluencia_incremental, penalidade);
						}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= idHidreletrica_maiorId; idHidreletrica++) {

					} //else if (subsistema_reportado == false) {

				}//if (menemonico == "DESVIO" || menemonico == "desvio") {

				if (menemonico == "INTMIN" || menemonico == "intmin") {

					if (subsistema_reportado == true) {

						//O subsistema para as hidrelétricas refere-se ao REE associado (ver arquivo: CONFHD.DAT)

						const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, subsistema);

						if (idSubmercado == IdSubmercado_Nenhum)
							throw std::invalid_argument("SISTEMA.DAT submercado nao instanciado, ver Bloco 4 Registro tipo 1 ");


						//Nota: Pega os intercambios que o submercado seja de origem ou de destino

						//////////////////////////////////////////////////
						// Submercado origem 
						//////////////////////////////////////////////////

						std::vector<IdIntercambio> idIntercambio_inicializado_origem = a_dados.vetorIntercambio.getIdObjetos(AttComumIntercambio_submercado_origem, idSubmercado);

						const int numero_intercambios_origem = int(idIntercambio_inicializado_origem.size());

						for (int intercambio = 0; intercambio < numero_intercambios_origem; intercambio++) {

							const IdIntercambio idIntercambio = idIntercambio_inicializado_origem.at(intercambio);

							a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_penalidade_intercambio, penalidade);

						}//for (int intercambio = 0; intercambio < numero_intercambios; intercambio++) {

						//////////////////////////////////////////////////
						// Submercado destino
						//////////////////////////////////////////////////

						std::vector<IdIntercambio> idIntercambio_inicializado_destino = a_dados.vetorIntercambio.getIdObjetos(AttComumIntercambio_submercado_origem, idSubmercado);

						const int numero_intercambios_destino = int(idIntercambio_inicializado_destino.size());

						for (int intercambio = 0; intercambio < numero_intercambios_destino; intercambio++) {

							const IdIntercambio idIntercambio = idIntercambio_inicializado_destino.at(intercambio);

							a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_penalidade_intercambio, penalidade);

						}//for (int intercambio = 0; intercambio < numero_intercambios; intercambio++) {

					}//if (subsistema_reportado == true) {
					else if (subsistema_reportado == false) {

						//O valor de penalidade aplica para todos os intercâmbios

						const IdIntercambio idIntercambio_maiorId = a_dados.vetorIntercambio.getMaiorId();

						for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= idIntercambio_maiorId; idIntercambio++) {

							a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_penalidade_intercambio, penalidade);

						}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= idHidreletrica_maiorId; idHidreletrica++) {

					}//else if (subsistema_reportado == false) {

				}//if (menemonico == "INTMIN" || menemonico == "intmin") {

				if (menemonico == "VAZMIN" || menemonico == "vazmin") {

					if (subsistema_reportado == true) {

						//O subsistema para as hidrelétricas refere-se ao REE associado (ver arquivo: CONFHD.DAT)

						for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica++) {
							if (lista_codigo_ONS_REE.at(idHidreletrica) == subsistema) {
								a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_penalidade_vazao_defluente_minima, penalidade);
								a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_penalidade_vazao_defluente_maxima, penalidade);
							} // if (lista_codigo_ONS_REE.at(idHidreletrica) == subsistema) {
						} // for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica++) {

					}//if (subsistema_reportado == true) {

					else if (subsistema_reportado == false) {

						//O valor de penalidade aplica para todas as usinas

						const IdHidreletrica idHidreletrica_maiorId = a_dados.vetorHidreletrica.getMaiorId();

						for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= idHidreletrica_maiorId; idHidreletrica++) {

							a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_penalidade_vazao_defluente_minima, penalidade);

						}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= idHidreletrica_maiorId; idHidreletrica++) {

					}//else if (subsistema_reportado == false) {

				}//if (menemonico == "VAZMIN" || menemonico == "vazmin") {

				if (menemonico == "VOLMIN" || menemonico == "volmin") {

					if (subsistema_reportado == true) {

						//O subsistema para as hidrelétricas refere-se ao REE associado (ver arquivo: CONFHD.DAT)

						for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica++) {
							if (lista_codigo_ONS_REE.at(idHidreletrica) == subsistema) 
								a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_penalidade_volume_minimo, penalidade);
						} // for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica++) {

					} // if (subsistema_reportado == true) {

					else if (subsistema_reportado == false) {

						//O valor de penalidade aplica para todas as usinas

						const IdHidreletrica idHidreletrica_maiorId = a_dados.vetorHidreletrica.getMaiorId();

						for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= idHidreletrica_maiorId; idHidreletrica++) {

							a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_penalidade_volume_minimo, penalidade);

						}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= idHidreletrica_maiorId; idHidreletrica++) {

					}//else if (subsistema_reportado == false) {

				}//if (menemonico == "VOLMIN" || menemonico == "volmin") {

				if (menemonico == "GHMIN" || menemonico == "ghmin") {

					if (subsistema_reportado == true) {

						// O subsistema para as hidrelétricas refere-se ao REE associado (ver arquivo: CONFHD.DAT)

						for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica++) {
							if (lista_codigo_ONS_REE.at(idHidreletrica) == subsistema)
								a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_penalidade_potencia_minima, penalidade);
						} // for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica++) {

					}//if (subsistema_reportado == true) {
					else if (subsistema_reportado == false) {

						//O valor de penalidade aplica para todas as usinas

						const IdHidreletrica idHidreletrica_maiorId = a_dados.vetorHidreletrica.getMaiorId();

						for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= idHidreletrica_maiorId; idHidreletrica++) {

							a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_penalidade_potencia_minima, penalidade);

						}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= idHidreletrica_maiorId; idHidreletrica++) {

					}//else if (subsistema_reportado == false) {

				}//if (menemonico == "GHMIN" || menemonico == "ghmin") {

			}//while (std::getline(leituraArquivo, line)) {

			leituraArquivo.close();

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_NEWAVE_26_PENALID: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_AGRINT_201908_NW25(Dados& a_dados, std::string nomeArquivo) {

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;

		//Horizonte de estudo
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		////////////////////////////////////////////////////////

		if (considera_agrupamento_livre_de_intercambios == true) {

			SmartEnupla<IdAgrupamentoIntercambio, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>> lista_AGRINT_PMAX;

			if (leituraArquivo.is_open()) {

				//Pula o cabeçalho
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);

				//******************************
				//Bloco 1
				//******************************

				while (true) {

					std::getline(leituraArquivo, line);

					strNormalizada(line);

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 1 -  Número do agrupamento ou critério de parada
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(1, 3);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					if (atributo == "999")
						break;

					const int codigo_usina = atoi(atributo.c_str());

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 2 -  Subsistema/submercado de origem da interligação que compõe o agrupamento
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(5, 3);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const int codigo_usina_submercado_origem = atoi(atributo.c_str());

					const IdSubmercado idSubmercado_origem = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_usina_submercado_origem);

					if (idSubmercado_origem == IdSubmercado_Nenhum)
						throw std::invalid_argument("SISTEMA.DAT submercado nao instanciado, ver Bloco 4 Registro tipo 1 ");

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 3 -  Subsistema/submercado de destino da interligação que compõe o agrupamento 
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(9, 3);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const int codigo_usina_submercado_destino = atoi(atributo.c_str());

					const IdSubmercado idSubmercado_destino = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_usina_submercado_destino);

					if (idSubmercado_destino == IdSubmercado_Nenhum)
						throw std::invalid_argument("SISTEMA.DAT submercado nao instanciado, ver Bloco 4 Registro tipo 1 ");

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Identifica o IdIntercambio
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					const std::vector<IdIntercambio> idIntercambio_inicializado = a_dados.vetorIntercambio.getIdObjetos(AttComumIntercambio_submercado_origem, idSubmercado_origem);

					const int idIntercambio_inicializado_size = int(idIntercambio_inicializado.size());

					IdIntercambio idIntercambio;

					for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

						idIntercambio = idIntercambio_inicializado.at(intercambio_inicializado);

						if (a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_destino)
							break;

						if (intercambio_inicializado + 1 == idIntercambio_inicializado_size)
							throw std::invalid_argument("Intercambio nao inicializado entre subsistema_" + getString(idSubmercado_origem) + "e subsistema_" + getString(idSubmercado_destino));

					}//for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 4 -  Coeficiente associado à interligação que compõe o agrupamento (ki) 
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(13, 7);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const double fator_ponderacao = atof(atributo.c_str());

					//*******************************************************************************************************************
					//Guarda informação nos SmartElementos
					//*******************************************************************************************************************

					const IdAgrupamentoIntercambio idAgrupamentoIntercambio_inicializado = getIdFromCodigoONS(lista_codigo_ONS_agrupamento_intercambio, codigo_usina);

					if (idAgrupamentoIntercambio_inicializado == IdAgrupamentoIntercambio_Nenhum) { // Novo AgrupamentoIntercambio

						AgrupamentoIntercambio agrupamentoIntercambio;

						const IdAgrupamentoIntercambio idAgrupamentoIntercambio = IdAgrupamentoIntercambio(a_dados.getMaiorId(IdAgrupamentoIntercambio()) + 1);

						agrupamentoIntercambio.setAtributo(AttComumAgrupamentoIntercambio_idAgrupamentoIntercambio, idAgrupamentoIntercambio);

						lista_codigo_ONS_agrupamento_intercambio.addElemento(idAgrupamentoIntercambio, codigo_usina);

						agrupamentoIntercambio.addElemento(AttVetorAgrupamentoIntercambio_intercambio, 1, idIntercambio); //Set o primeiro elemento do do vetor
						agrupamentoIntercambio.addElemento(AttVetorAgrupamentoIntercambio_fator_ponderacao, 1, fator_ponderacao); //Set o primeiro elemento do do vetor

						///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						//Inicializa a matriz potencia_maxima e logo no bloco 2 são atualizadas nos periodos indicados
						///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

						a_dados.vetorAgrupamentoIntercambio.add(agrupamentoIntercambio);

						for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, horizonte_estudo.getIteradorInicial(), IdPatamarCarga()); idPatamarCarga++)
								a_dados.vetorAgrupamentoIntercambio.att(idAgrupamentoIntercambio).addElemento(AttMatrizAgrupamentoIntercambio_potencia_maxima, periodo, idPatamarCarga, 0.0);
						} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); periodo++) {


						///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


					} // if (idAgrupamentoIntercambio_inicializado == IdAgrupamentoIntercambio_Nenhum)

					else {

						const IdAgrupamentoIntercambio idAgrupamentoIntercambio = idAgrupamentoIntercambio_inicializado;

						int iterador = a_dados.getIteradorFinal(idAgrupamentoIntercambio, AttVetorAgrupamentoIntercambio_intercambio, int());

						iterador++;

						a_dados.vetorAgrupamentoIntercambio.att(idAgrupamentoIntercambio).addElemento(AttVetorAgrupamentoIntercambio_intercambio, iterador, idIntercambio);
						a_dados.vetorAgrupamentoIntercambio.att(idAgrupamentoIntercambio).addElemento(AttVetorAgrupamentoIntercambio_fator_ponderacao, iterador, fator_ponderacao);

					}//else {

					//const IdAgrupamentoIntercambio idAgrupamentoIntercambio = idAgrupamentoIntercambio_inicializado.at(0);

				}//while (true) {

				lista_AGRINT_PMAX = SmartEnupla<IdAgrupamentoIntercambio, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>>(IdAgrupamentoIntercambio_1, std::vector<SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>>(a_dados.getMaiorId(IdAgrupamentoIntercambio()), SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>()));


				//******************************
				//Bloco 2
				//******************************

				//Pula o cabeçalho
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);

				while (true) {

					std::getline(leituraArquivo, line);

					strNormalizada(line);

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 1 -  Número do agrupamento
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(1, 3);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					if (atributo == "999")
						break;

					const int codigo_usina = atoi(atributo.c_str());

					const IdAgrupamentoIntercambio idAgrupamentoIntercambio = getIdFromCodigoONS(lista_codigo_ONS_agrupamento_intercambio, codigo_usina);

					if (idAgrupamentoIntercambio != IdAgrupamentoIntercambio_Nenhum) {

					/////////////////////////////////////////////////
					//Parâmetros
					/////////////////////////////////////////////////

						IdMes mes_inicial = IdMes_Nenhum;
						IdAno ano_inicial = IdAno_Nenhum;
						IdMes mes_final = IdMes_Nenhum;
						IdAno ano_final = IdAno_Nenhum;

						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						//Campo 2 -  Mês de início para o limite do agrupamento  
						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

						atributo = line.substr(6, 2);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						if (atributo != "")
							mes_inicial = getIdMesFromChar(atributo.c_str());

						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						//Campo 3 -  Ano de início para o limite do agrupamento
						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

						atributo = line.substr(9, 4);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						if (atributo != "")
							ano_inicial = getIdAnoFromChar(atributo.c_str());

						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						//Campo 4 -  Mês de fim para o limite do agrupamento  
						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

						atributo = line.substr(14, 2);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						if (atributo != "")
							mes_final = getIdMesFromChar(atributo.c_str());

						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						//Campo 5 -  Ano de fim para o limite do agrupamento
						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

						atributo = line.substr(17, 4);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						if (atributo != "")
							ano_final = getIdAnoFromChar(atributo.c_str());

						//*************************************************
						//Determina periodos inicial e final
						//*************************************************

						//Manual: 
						//Caso os campos relativos à data inicial estiverem em branco e aqueles relacionados à data final estiverem preenchidos, 
						//os dados relativos ao limite do agrupamento serão considerados a partir do início do período de planejamento. 
						//Se a data inicial for anterior ao primeiro período de planejamento, essa será deslocada para o início do período de planejamento. 
						//Caso os campos relativos à data final estiverem em branco e aqueles relacionados à data inicial estiverem preenchidos, os dados relativos 
						//ao limite do agrupamento serão considerados até o final do horizonte de planejamento. Se a data final for posterior ao fim do período de planejamento, 
						//essa será deslocada para o final do horizonte de planejamento

						Periodo periodo_agrupamento_inicio;
						Periodo periodo_agrupamento_final;

						if (((mes_inicial == IdMes_Nenhum) || (ano_inicial == IdAno_Nenhum)) && ((mes_final == IdMes_Nenhum) || (ano_final == IdAno_Nenhum)))//Manual: Se os campos relacionados à data inicial e à data final estiverem em branco, a execução do programa será interrompida com uma mensagem de erro
							throw std::invalid_argument("Data inicial e final nao reportada");


						if ((mes_inicial == IdMes_Nenhum) || (ano_inicial == IdAno_Nenhum))
							periodo_agrupamento_inicio = horizonte_estudo_DECK.getIteradorInicial();

						else
							periodo_agrupamento_inicio = Periodo(mes_inicial, ano_inicial);


						if ((mes_final == IdMes_Nenhum) || (ano_final == IdAno_Nenhum))
							periodo_agrupamento_final = horizonte_estudo_DECK.getIteradorFinal();

						else
							periodo_agrupamento_final = Periodo(mes_final, ano_final);


						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						//Campo 6-10 -  Limite do agrupamento (MWmédio) para o patamar de carga X
						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

						SmartEnupla<IdPatamarCarga, double> potencia_maxima_patamar(IdPatamarCarga_1, std::vector<double>(a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, horizonte_estudo.getIteradorInicial(), IdPatamarCarga()), 0.0));

						if (lista_AGRINT_PMAX.at(idAgrupamentoIntercambio).size() == 0)
							lista_AGRINT_PMAX.at(idAgrupamentoIntercambio) = SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo_DECK.getIteradorInicial(), std::vector<SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo_DECK.getIteradorFinal() - horizonte_estudo_DECK.getIteradorInicial() + 1, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, horizonte_estudo.getIteradorInicial(), IdPatamarCarga()), 0.0))));


						for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, horizonte_estudo.getIteradorInicial(), IdPatamarCarga()); idPatamarCarga++) {

							atributo = line.substr(22 + 8 * int(idPatamarCarga - 1), 7);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							if (atributo == "")
								throw std::invalid_argument("Limite do agrupamento nao reportado para " + getFullString(idPatamarCarga));

							double valor = atof(atributo.c_str());

							if (valor == -1) //-1 significa que deve ser desconsiderada a restrição
								valor = 0;

							if (valor < 0)
								throw std::invalid_argument("Valor do limite do agrupamento negativo");

							potencia_maxima_patamar.at(idPatamarCarga) = valor;

						} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga_global; idPatamarCarga++) {

						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						//Campo Não obrigatório -  Nome do agrupamento de intercâmbio
						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

						const int line_size = int(line.length());

						if (line_size > 29 + 8 * int(a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, horizonte_estudo.getIteradorInicial(), IdPatamarCarga()) - 1)) {//29 + 8 * int(maiorIdPatamarCarga_global - 1) refere-se ao tamanho da linha até o último camo obrigatório

							atributo = line.substr(29 + 8 * int(a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, horizonte_estudo.getIteradorInicial(), IdPatamarCarga()) - 1), line_size - (29 + 8 * int(a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, horizonte_estudo.getIteradorInicial(), IdPatamarCarga()) - 1)) + 1);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							if (atributo != "") {

								const std::string nome_agrupamento_intercambio = atributo;

								a_dados.vetorAgrupamentoIntercambio.att(idAgrupamentoIntercambio).setAtributo(AttComumAgrupamentoIntercambio_nome, nome_agrupamento_intercambio);

							}//if (atributo != "") {

						}//if (line_size > 37) {


						for (Periodo periodo = periodo_agrupamento_inicio; periodo <= periodo_agrupamento_final; periodo++) {

							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= potencia_maxima_patamar.getIteradorFinal(); idPatamarCarga++) {

								if (potencia_maxima_patamar.at(idPatamarCarga) > 0.0)
									lista_AGRINT_PMAX.at(idAgrupamentoIntercambio).at(periodo).at(idPatamarCarga) = potencia_maxima_patamar.at(idPatamarCarga);

							} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= potencia_maxima_patamar.getIteradorFinal(); idPatamarCarga++) {

						} // for (Periodo periodo = periodo_restricao; periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo)) {

					} // if (idAgrupamentoIntercambio != IdAgrupamentoIntercambio_Nenhum){

				}//while (true) {

				leituraArquivo.close();

				double sobreposicao_atraso_periodo_inicial = 0.0;

				if (desconsiderar_atraso_periodo_estudo_inicial)
					sobreposicao_atraso_periodo_inicial = horizonte_estudo.getIteradorInicial().atraso(horizonte_estudo_DECK.getIteradorInicial());

				for (IdAgrupamentoIntercambio idAgrupamentoIntercambio = IdAgrupamentoIntercambio_1; idAgrupamentoIntercambio <= lista_AGRINT_PMAX.getIteradorFinal(); idAgrupamentoIntercambio++) {

					if (lista_AGRINT_PMAX.at(idAgrupamentoIntercambio).size() > 0) {

						a_dados.vetorAgrupamentoIntercambio.att(idAgrupamentoIntercambio).setMatriz_forced(AttMatrizAgrupamentoIntercambio_potencia_maxima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, horizonte_estudo.getIteradorInicial(), IdPatamarCarga()), 0.0))));

						for (Periodo periodo_DECK = lista_AGRINT_PMAX.at(idAgrupamentoIntercambio).getIteradorInicial(); periodo_DECK <= lista_AGRINT_PMAX.at(idAgrupamentoIntercambio).getIteradorFinal(); periodo_DECK++) {

							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								double sobreposicao = periodo.sobreposicao(periodo_DECK);

								if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
									sobreposicao += sobreposicao_atraso_periodo_inicial;

								if (sobreposicao > 0.0) {

									for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= lista_AGRINT_PMAX.at(idAgrupamentoIntercambio).at(periodo_DECK).getIteradorFinal(); idPatamarCarga++) {

										const double valor_antigo = a_dados.getElementoMatriz(idAgrupamentoIntercambio, AttMatrizAgrupamentoIntercambio_potencia_maxima, periodo, idPatamarCarga, double());

										a_dados.vetorAgrupamentoIntercambio.att(idAgrupamentoIntercambio).setElemento(AttMatrizAgrupamentoIntercambio_potencia_maxima, periodo, idPatamarCarga, valor_antigo + sobreposicao * lista_AGRINT_PMAX.at(idAgrupamentoIntercambio).at(periodo_DECK).at(idPatamarCarga));

									} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= lista_AGRINT_PMAX.at(idAgrupamentoIntercambio).at(periodo_DECK).getIteradorFinal(); idPatamarCarga++) {

								} // if (sobreposicao > 0.0) {

							} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						} // for (Periodo periodo_DECK = lista_AGRINT_PMAX.at(idAgrupamentoIntercambio).getIteradorInicial(); periodo_DECK <= lista_AGRINT_PMAX.at(idAgrupamentoIntercambio).getIteradorFinal(); periodo_DECK++) {

					} // if (lista_AGRINT_PMAX.at(idAgrupamentoIntercambio).size() > 0) {

				} // for (IdAgrupamentoIntercambio idAgrupamentoIntercambio = IdAgrupamentoIntercambio_1; idAgrupamentoIntercambio <= lista_AGRINT_PMAX.getIteradorFinal(); idAgrupamentoIntercambio++) {

			}//if (leituraArquivo.is_open()) {
			else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

		}//if (considera_agrupamento_livre_de_intercambios == true) {

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_NEWAVE_26_AGRINT: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_ADTERM_201908_NW25(Dados& a_dados, std::string nomeArquivo) {

	try {

		if (considera_despacho_antecipado_gnl == true) {

			//Horizonte de estudo
			const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

			double sobreposicao_atraso_periodo_inicial = 0.0;

			if (desconsiderar_atraso_periodo_estudo_inicial)
				sobreposicao_atraso_periodo_inicial = horizonte_estudo.getIteradorInicial().atraso(horizonte_estudo_DECK.getIteradorInicial());

			std::ifstream leituraArquivo(nomeArquivo);
			std::string line;

			std::string atributo;

			////////////////////////////////////////////////////////

			if (leituraArquivo.is_open()) {

				//Pula o cabeçalho
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);

				int codigo_usina = 0;

				IdTermeletrica idTermeletrica = IdTermeletrica_Nenhum;

				int lag_de_antecipacao = -1;

				SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> potencia_disponivel_pre_comandada;

				while (true) {

					std::getline(leituraArquivo, line);

					strNormalizada(line);

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 1 -  Número da usina térmica GNL
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(1, 4);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					//*******************************************************************************************************************
					//	Registro tipo 1
					//*******************************************************************************************************************

					if (atributo != "") {

						if ((potencia_disponivel_pre_comandada.size() > 0) && (a_dados.getSizeVetor(idTermeletrica, AttVetorTermeletrica_potencia_disponivel_pre_comandada) == 0)) {

							for (Periodo periodo_DECK = potencia_disponivel_pre_comandada.getIteradorInicial(); periodo_DECK <= potencia_disponivel_pre_comandada.getIteradorFinal(); potencia_disponivel_pre_comandada.incrementarIterador(periodo_DECK)) {

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									double sobreposicao = periodo.sobreposicao(periodo_DECK);

									if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
										sobreposicao += sobreposicao_atraso_periodo_inicial;

									if (sobreposicao > 0.0) {

										double potencia = 0.0;

										for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++)
											potencia += potencia_disponivel_pre_comandada.at(periodo_DECK).at(idPatamarCarga) * a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());

										if (a_dados.getSizeVetor(idTermeletrica, AttVetorTermeletrica_potencia_disponivel_pre_comandada) == 0)
											a_dados.vetorTermeletrica.att(idTermeletrica).addElemento(AttVetorTermeletrica_potencia_disponivel_pre_comandada, periodo, 0.0);
										else if (a_dados.getIteradorFinal(idTermeletrica, AttVetorTermeletrica_potencia_disponivel_pre_comandada, Periodo()) < periodo)
											a_dados.vetorTermeletrica.att(idTermeletrica).addElemento(AttVetorTermeletrica_potencia_disponivel_pre_comandada, periodo, 0.0);

										const double valor_antigo = a_dados.getElementoVetor(idTermeletrica, AttVetorTermeletrica_potencia_disponivel_pre_comandada, periodo, double());

										a_dados.vetorTermeletrica.att(idTermeletrica).setElemento(AttVetorTermeletrica_potencia_disponivel_pre_comandada, periodo, valor_antigo + sobreposicao * potencia);

									} // if (sobreposicao > 0.0) {

								} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							} // for (Periodo periodo_DECK = potencia_disponivel_pre_comandada.getIteradorInicial(); periodo_DECK <= potencia_disponivel_pre_comandada.getIteradorFinal(); potencia_disponivel_pre_comandada.incrementarIterador(periodo_DECK)) {

						} // if ((potencia_disponivel_pre_comandada.size() > 0) && (a_dados.getSizeVetor(idTermeletrica, AttVetorTermeletrica_potencia_disponivel_pre_comandada) == 0)) {

						if (atributo == "9999")
							break;

						codigo_usina = atoi(atributo.c_str());

						//Identifica se alguma termelétrica tem sido inicializada com codigo_usina
						idTermeletrica = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, codigo_usina);

						if (idTermeletrica == IdTermeletrica_Nenhum)
							throw std::invalid_argument("Nao inicializada idTermeletrica com codigo_usina_" + atributo);

						//Campo 2 -  Nome da usina térmica GNL
						atributo = line.substr(7, 12);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());


						//Campo 3 -  Lag de antecipação de despacho da usina térmica GNL (nlag)
						const int lag_de_antecipacao = std::stoi(line.substr(21, 1));
						a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada, lag_de_antecipacao);

						potencia_disponivel_pre_comandada = SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>();

					}

					//*******************************************************************************************************************
					//Registro tipo 2
					//*******************************************************************************************************************

					else {

						const IdPatamarCarga maiorIdPatamarCarga = IdPatamarCarga_3;

						if (potencia_disponivel_pre_comandada.size() == 0)
							potencia_disponivel_pre_comandada.addElemento(horizonte_estudo_DECK.getIteradorInicial(), SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(maiorIdPatamarCarga, NAN)));
						else
							potencia_disponivel_pre_comandada.addElemento(potencia_disponivel_pre_comandada.getIteradorFinal() + 1, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(maiorIdPatamarCarga, NAN)));

						for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 1-5 -  Geração térmica antecipada lag i para X patamar de carga (MW) 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							atributo = line.substr(24 + 12 * int(idPatamarCarga - 1), 10);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							double potencia = atof(atributo.c_str());

							potencia_disponivel_pre_comandada.at(potencia_disponivel_pre_comandada.getIteradorFinal()).at(idPatamarCarga) = potencia;

						} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

					} // else {

				}//while (true) {

				leituraArquivo.close();

				for (IdTermeletrica idTermeletrica = IdTermeletrica_1; idTermeletrica <= a_dados.getMaiorId(IdTermeletrica()); idTermeletrica++) {

					//
					// Anula custo de operação em caso de potencia_disponivel_pre_comandada
					//

					if (a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada, int()) > 0) {

						const SmartEnupla<Periodo, double> potencia_disponivel_pre_comandada = a_dados.getVetor(idTermeletrica, AttVetorTermeletrica_potencia_disponivel_pre_comandada, Periodo(), double());

						if (potencia_disponivel_pre_comandada.size() > 0) {

							for (Periodo periodo_estudo = horizonte_estudo.getIteradorInicial(); periodo_estudo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo_estudo)) {

								double percentual_valor = 1.0;

								for (Periodo periodo_pre_comando = potencia_disponivel_pre_comandada.getIteradorInicial(); periodo_pre_comando <= potencia_disponivel_pre_comandada.getIteradorFinal(); potencia_disponivel_pre_comandada.incrementarIterador(periodo_pre_comando)) {

									const double sobreposicao = periodo_estudo.sobreposicao(periodo_pre_comando);
									if (sobreposicao > 0.0)
										percentual_valor -= sobreposicao;

								} // for (Periodo periodo_pre_comando = potencia_disponivel_pre_comandada.getIteradorInicial(); periodo_pre_comando <= potencia_disponivel_pre_comandada.getIteradorFinal(); potencia_disponivel_pre_comandada.incrementarIterador(periodo_pre_comando)) {

								if (percentual_valor < 0.0)
									percentual_valor = 0.0;

								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(idTermeletrica, AttMatrizTermeletrica_custo_de_operacao, periodo_estudo, IdPatamarCarga()); idPatamarCarga++) {

									const double custo_de_operacao = a_dados.getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_custo_de_operacao, periodo_estudo, idPatamarCarga, double()) * percentual_valor;

									a_dados.vetorTermeletrica.att(idTermeletrica).setElemento(AttMatrizTermeletrica_custo_de_operacao, periodo_estudo, idPatamarCarga, custo_de_operacao);

								} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(idTermeletrica, AttMatrizTermeletrica_custo_de_operacao, periodo_estudo, IdPatamarCarga()); idPatamarCarga++) {

							} // for (Periodo periodo_estudo = horizonte_estudo.getIteradorInicial(); periodo_estudo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo_estudo)) {

						} // if (potencia_disponivel_pre_comandada.size() > 0) {

					} // if (a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada, int()) > 0) {

				} // for (IdTermeletrica idTermeletrica = IdTermeletrica_1; idTermeletrica <= a_dados.getMaiorId(IdTermeletrica()); idTermeletrica++) {

			}//if (leituraArquivo.is_open()) {
			else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

		}//if (considera_despacho_antecipado_gnl == true) {

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_NEWAVE_26_ADTERM: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_GHMIN_201908_NW25(Dados& a_dados, std::string nomeArquivo) {

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;

		//Horizonte de estudo
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		SmartEnupla<IdHidreletrica, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>> lista_GHMIN(IdHidreletrica_1, std::vector<SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>>(a_dados.getMaiorId(IdHidreletrica()), SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>()));

		////////////////////////////////////////////////////////

		if (leituraArquivo.is_open()) {

			//Pula o cabeçalho
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);

			while (true) {

				std::getline(leituraArquivo, line);

				strNormalizada(line);

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Campo 1 -  Número da usina hidroelétrica. 
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				atributo = line.substr(0, 3);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				if (atributo == "999")
					break;

				const int codigo_usina = atoi(atributo.c_str());


				const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

				if (idHidreletrica == IdHidreletrica_Nenhum) {

					for (int i = 1; i <= lista_codigo_ONS_hidreletrica_agregada.size(); i++) {
						if (lista_codigo_ONS_hidreletrica_agregada.getElemento(i) == codigo_usina)
							throw std::invalid_argument("Esta representacao deve ser reavaliada em termos das hidreletricas desagregadas da usina codigo " + getString(codigo_usina));
					}

				} // if (idHidreletrica == IdHidreletrica_Nenhum){


				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Campo 2 -  Mês de início da restrição de geração hidráulica mínima. 
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				atributo = line.substr(5, 2);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				const IdMes mes = getIdMesFromChar(atributo.c_str());

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Campo 3 -  Ano de início da restrição de geração hidráulica mínima.
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				atributo = line.substr(8, 4);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				IdAno ano = IdAno_Nenhum;

				if (!strCompara(atributo, "PRE") && !strCompara(atributo, "POS"))
					ano = getIdAnoFromChar(atributo.c_str());

				if (ano != IdAno_Nenhum) {//Ignora valores para periodos estáticos PRE/POS

					const Periodo periodo_restricao(mes, ano);

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 4 -  Número do patamar de carga. 
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(14, 1);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const IdPatamarCarga idPatamarCarga_lido = getIdPatamarCargaFromChar(atributo.c_str());

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 5 -  Geração hidráulica mínima da usina (MWmédio).
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(17, 6);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const double potencia_minima = atoi(atributo.c_str());

					//*******************************************************************************************************************
					//Guarda informação nos SmartElementos
					//*******************************************************************************************************************

					////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Manual: 
					//Nota 1: A data de início da restrição de geração hidráulica mínima, declarada nos campos 2 e 3, deve pertencer ao 
					//        período de estudo. Uma restrição de geração hidráulica mínima é válida até a data de início de outra restrição
					////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					SmartEnupla<IdPatamarCarga, double> potencia_minima_patamar;

					if (idPatamarCarga_lido == IdPatamarCarga_Nenhum)
						potencia_minima_patamar = SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, horizonte_estudo.getIteradorInicial(), IdPatamarCarga()), potencia_minima));

					else {
						potencia_minima_patamar = SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, horizonte_estudo.getIteradorInicial(), IdPatamarCarga()), 0.0));
						potencia_minima_patamar.at(idPatamarCarga_lido) = potencia_minima;
					}

					if (lista_GHMIN.at(idHidreletrica).size() == 0)
						lista_GHMIN.at(idHidreletrica) = SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo_DECK.getIteradorInicial(), std::vector<SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo_DECK.getIteradorFinal() - horizonte_estudo_DECK.getIteradorInicial() + 1, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, horizonte_estudo.getIteradorInicial(), IdPatamarCarga()), 0.0))));

					for (Periodo periodo = periodo_restricao; periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo)) {

						for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= potencia_minima_patamar.getIteradorFinal(); idPatamarCarga++) {

							if (potencia_minima_patamar.at(idPatamarCarga) > 0.0)
								lista_GHMIN.at(idHidreletrica).at(periodo).at(idPatamarCarga) = potencia_minima_patamar.at(idPatamarCarga);

						} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= potencia_minima_patamar.getIteradorFinal(); idPatamarCarga++) {

					} // for (Periodo periodo = periodo_restricao; periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo)) {

				} // if (ano != IdAno_Nenhum) {//Ignora valores para periodos estáticos PRE/POS

			}//while (true) {

			leituraArquivo.close();


			double sobreposicao_atraso_periodo_inicial = 0.0;

			if (desconsiderar_atraso_periodo_estudo_inicial)
				sobreposicao_atraso_periodo_inicial = horizonte_estudo.getIteradorInicial().atraso(horizonte_estudo_DECK.getIteradorInicial());

			for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= lista_GHMIN.getIteradorFinal(); idHidreletrica++) {

				if (lista_GHMIN.at(idHidreletrica).size() > 0) {

					a_dados.vetorHidreletrica.att(idHidreletrica).setMatriz_forced(AttMatrizHidreletrica_potencia_disponivel_minima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, horizonte_estudo.getIteradorInicial(), IdPatamarCarga()), 0.0))));

					for (Periodo periodo_DECK = lista_GHMIN.at(idHidreletrica).getIteradorInicial(); periodo_DECK <= lista_GHMIN.at(idHidreletrica).getIteradorFinal(); periodo_DECK++) {

						for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							double sobreposicao = periodo.sobreposicao(periodo_DECK);

							if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
								sobreposicao += sobreposicao_atraso_periodo_inicial;

							if (sobreposicao > 0.0) {

								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= lista_GHMIN.at(idHidreletrica).at(periodo_DECK).getIteradorFinal(); idPatamarCarga++) {

									const double valor_antigo = a_dados.getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_minima, periodo, idPatamarCarga, double());

									a_dados.vetorHidreletrica.att(idHidreletrica).setElemento(AttMatrizHidreletrica_potencia_disponivel_minima, periodo, idPatamarCarga, valor_antigo + sobreposicao * lista_GHMIN.at(idHidreletrica).at(periodo_DECK).at(idPatamarCarga));

								} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= lista_GHMIN.at(idHidreletrica).at(periodo_DECK).getIteradorFinal(); idPatamarCarga++) {

							} // if (sobreposicao > 0.0) {

						} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

					} // for (Periodo periodo_DECK = lista_GHMIN.at(idHidreletrica).getIteradorInicial(); periodo_DECK <= lista_GHMIN.at(idHidreletrica).getIteradorFinal(); periodo_DECK++) {

				} // if (lista_GHMIN.at(idHidreletrica).size() > 0) {

			} // for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= lista_GHMIN.getIteradorFinal(); idHidreletrica++) {


		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_GHMIN_201908_NW25: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_CVAR_201908_NW25(Dados& a_dados, std::string nomeArquivo) {

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;

		//Horizonte de estudo
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		////////////////////////////////////////////////////////

		if (a_dados.getAtributo(AttComumDados_tipo_aversao_a_risco, TipoAversaoRisco()) == TipoAversaoRisco_CVAR) {

			if (leituraArquivo.is_open()) {

				//******************************
				//Bloco 1
				//******************************

				//Pula o cabeçalho
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);

				std::getline(leituraArquivo, line);

				strNormalizada(line);

				if (considera_CVAR_variavel_no_tempo == false) {//Valores constante no tempo

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 1 -  Parâmetro (alpha): Percentual do total dos cenários de um período, de custo mais elevado, 
					//           que será considerado com custo adicional na função objetivo  
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(8, 5); //Nota: erro no manual de referência
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const double alpha_CVAR = atof(atributo.c_str()) / 100;

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 2 -  Parâmetro (lambda): Peso (em percentual) da parcela adicional que será considerada na função objetivo 
					//           do problema de otimização, referente ao custo esperado dos (alpha) cenários mais críticos 
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(15, 5); //Nota: erro no manual de referência
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const double lambda_CVAR = atof(atributo.c_str()) / 100;

					a_dados.setAtributo(AttComumDados_alpha_CVAR, alpha_CVAR);
					a_dados.setAtributo(AttComumDados_lambda_CVAR, lambda_CVAR);

				}//if (considera_CVAR_variavel_no_tempo == false) {

				else if (considera_CVAR_variavel_no_tempo == true) {

					//******************************
					//Bloco 2
					//******************************

					//Pula o cabeçalho
					std::getline(leituraArquivo, line);
					std::getline(leituraArquivo, line);
					std::getline(leituraArquivo, line);

					strNormalizada(line);

					SmartEnupla<IdEstagio, double> alpha_CVAR;

					IdEstagio idEstagio = IdEstagio_1;

					for (Periodo periodo = horizonte_estudo_DECK.getIteradorInicial(); periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo)) {

						const int mes = atoi(getString(periodo.getMes()).c_str());

						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						//Campo 1 - 12 -  Percentual do total dos cenários de um período que será considerado com custo adicional 
						//                na função objetivo (  ) para o mês 1 (%) 
						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

						atributo = line.substr(8 + (mes - 1) * 7, 5); //Nota: erro no manual de referência
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						alpha_CVAR.addElemento(idEstagio, atof(atributo.c_str()) / 100);

						idEstagio++;

						if (idEstagio > a_dados.getAtributo(AttComumDados_estagio_final, IdEstagio()))
							break;

						if (mes == 12) {

							std::getline(leituraArquivo, line);

							strNormalizada(line);

						}//if (mes == 12) {

					}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

					if (a_dados.getSizeVetor(AttVetorDados_alpha_CVAR) == 0)
						a_dados.setVetor(AttVetorDados_alpha_CVAR, alpha_CVAR);

					//******************************
					//Bloco 3
					//******************************

					//Pula de linha até encontrar uma linha com ano (início da informação do bloco 3)
					while (true) {

						std::getline(leituraArquivo, line);

						strNormalizada(line);

						atributo = line.substr(0, 7);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						if (atoi(atributo.c_str()) > 0)
							break;

					}//while (true) {


					SmartEnupla<IdEstagio, double> lambda_CVAR;

					idEstagio = IdEstagio_1;

					for (Periodo periodo = horizonte_estudo_DECK.getIteradorInicial(); periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo)) {

						const int mes = atoi(getString(periodo.getMes()).c_str());

						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						//Campo 1 - 12 -  Percentual do total dos cenários de um período que será considerado com custo adicional 
						//                na função objetivo (  ) para o mês 1 (%) 
						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

						atributo = line.substr(8 + (mes - 1) * 7, 5); //Nota: erro no manual de referência
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						lambda_CVAR.addElemento(idEstagio, atof(atributo.c_str()) / 100);

						idEstagio++;

						if (idEstagio > a_dados.getAtributo(AttComumDados_estagio_final, IdEstagio()))
							break;

						if (mes == 12) {

							std::getline(leituraArquivo, line);

							strNormalizada(line);

						}//if (mes == 12) {

					}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

					if (a_dados.getSizeVetor(AttVetorDados_lambda_CVAR) == 0)
						a_dados.setVetor(AttVetorDados_lambda_CVAR, lambda_CVAR);

				}//else if(considera_CVAR_variavel_no_tempo == true) {

				leituraArquivo.close();

			}//if (leituraArquivo.is_open()) {
			else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

		}//if(a_dados.getAtributo(AttComumDados_tipo_aversao_a_risco, TipoAversaoRisco())== TipoAversaoRisco_CVAR){

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_CVAR_201908_NW25: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_RE_201908_NW25(Dados& a_dados, std::string nomeArquivo) {

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;

		//Horizonte de estudo
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		////////////////////////////////////////////////////////

		if (considera_restricoes_eletricas == true) {

			SmartEnupla<IdRestricaoEletrica, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>> lista_RE_PMAX;

			if (leituraArquivo.is_open()) {

				//******************************
				//Bloco 1
				//******************************

				//Pula o cabeçalho
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);

				while (true) {

					std::getline(leituraArquivo, line);

					strNormalizada(line);

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 1 -  Número da restrição elétrica
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(0, 3);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					if (atributo == "999")
						break;

					const int codigo_usina_restricao = atoi(atributo.c_str());

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 2-11 -  Número da X usina hidroelétrica que compõe a restrição 
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					std::vector<int> vetor_codigo_usina;

					const int line_size = int(line.length());

					for (int conteio = 0; conteio < 10; conteio++) {//Uma restrição elétrica deve ter um conjunto de no mínimo uma usina e no máximo dez usinas

						if (line_size >= 7 + 4 * conteio) {

							const int size_registro = line_size - (6 + 4 * conteio); //O NW pode ter registros somente na primeira coluna e o tamanho ser só 1 caracter

							atributo = line.substr(6 + 4 * conteio, size_registro);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							if (atributo == "")//Teste registros com linhas com vazios
								break;

							vetor_codigo_usina.push_back(atoi(atributo.c_str()));

						}//if (line_size >= 7 + 4 * conteio) {

					}//for (int conteio = 0; conteio < 10; conteio++) {

					//*************************************************
					//Carrega Restrição elétrica
					//*************************************************

					const IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica(a_dados.getMaiorId(IdRestricaoEletrica()) + 1);

					if (true) {

						RestricaoEletrica restricaoEletrica;

						restricaoEletrica.setAtributo(AttComumRestricaoEletrica_idRestricaoEletrica, idRestricaoEletrica);

						lista_codigo_ONS_restricao_eletrica.addElemento(idRestricaoEletrica, codigo_usina_restricao);

						a_dados.vetorRestricaoEletrica.add(restricaoEletrica);

					} // if (true) {

					///////////////////////////
					//Set vetor idHidreletrica
					///////////////////////////

					for (int iterador = 1; iterador <= int(vetor_codigo_usina.size()); iterador++) {

						const IdHidreletrica idHidreletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, vetor_codigo_usina.at(iterador - 1));

						if (idHidreletrica_inicializado == IdHidreletrica_Nenhum) {

							for (int i = 1; i <= lista_codigo_ONS_hidreletrica_agregada.size(); i++) {
								if (lista_codigo_ONS_hidreletrica_agregada.getElemento(i) == vetor_codigo_usina.at(iterador - 1)) {

									for (int j = 1; j <= lista_codigo_ONS_hidreletricas_desagregadas.at(i).getIteradorFinal(); j++)
										vetor_codigo_usina.push_back(lista_codigo_ONS_hidreletricas_desagregadas.at(i).at(j));

								}
							}

						} // if (idHidreletrica_inicializado == IdHidreletrica_Nenhum) {

					}


					for (int iterador = 1; iterador <= int(vetor_codigo_usina.size()); iterador++) {

						const IdHidreletrica idHidreletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, vetor_codigo_usina.at(iterador - 1));

						if (idHidreletrica_inicializado == IdHidreletrica_Nenhum) {

							// XXX Escrever arquivo de aviso que será desconsiderada a restrição elétrica de hidrelétrica não utilizada

						} // if (idHidreletrica_inicializado == IdHidreletrica_Nenhum) {

						else {

							const IdElementoSistema idElementoSistema = IdElementoSistema(a_dados.getMaiorId(idRestricaoEletrica, IdElementoSistema()) + 1);

							ElementoSistema elementoSistema;
							elementoSistema.setAtributo(AttComumElementoSistema_idElementoSistema, idElementoSistema);
							elementoSistema.setAtributo(AttComumElementoSistema_hidreletrica, idHidreletrica_inicializado);
							elementoSistema.setAtributo(AttComumElementoSistema_tipo_elemento, TipoElementoSistema_hidreletrica);
							elementoSistema.setAtributo(AttComumElementoSistema_fator_participacao, 1.0);

							a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.add(elementoSistema);

						}

					} // for (int iterador = 0; iterador < size_vetor_codigo_usina; iterador++) {

					///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Inicializa a matriz potencia_maxima e logo no bloco 2 são atualizadas nos periodos indicados
					///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {

							a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).addElemento(AttMatrizRestricaoEletrica_potencia_minima, periodo, idPatamarCarga, 0.0);
							//a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).addElemento(AttMatrizRestricaoEletrica_potencia_maxima, periodo, idPatamarCarga, getdoubleFromChar("max"));

						} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
					} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); periodo++) {


				}//while (true) {

				lista_RE_PMAX = SmartEnupla<IdRestricaoEletrica, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>>(IdRestricaoEletrica_1, std::vector<SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>>(a_dados.getMaiorId(IdRestricaoEletrica()), SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>()));


				//******************************
				//Bloco 2
				//******************************

				//Pula o cabeçalho
				std::getline(leituraArquivo, line);
				std::getline(leituraArquivo, line);

				while (true) {

					std::getline(leituraArquivo, line);

					strNormalizada(line);

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 1 -  Número da restrição elétrica
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(0, 3);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					if (atributo == "999")
						break;

					const int codigo_usina_restricao = atoi(atributo.c_str());

					const IdRestricaoEletrica idRestricaoEletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_restricao_eletrica, codigo_usina_restricao);

					if (idRestricaoEletrica_inicializado == IdRestricaoEletrica_Nenhum)
						throw std::invalid_argument("Nao inicializada idRestricaoEletrica  com codigo_usina_" + atributo);

					const IdRestricaoEletrica  idRestricaoEletrica = idRestricaoEletrica_inicializado;

					/////////////////////////////////////////////////
					//Parâmetros
					/////////////////////////////////////////////////

					IdMes mes_inicial = IdMes_Nenhum;
					IdAno ano_inicial = IdAno_Nenhum;
					IdMes mes_final = IdMes_Nenhum;
					IdAno ano_final = IdAno_Nenhum;

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 2 -  Mês de início para restrição elétrica 
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(4, 2);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					if (atributo != "")
						mes_inicial = getIdMesFromChar(atributo.c_str());

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 3 -  Ano de início para restrição elétrica
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(7, 4);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					if (atributo != "")
						ano_inicial = getIdAnoFromChar(atributo.c_str());

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 4 -  Mês de fim para restrição elétrica  
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(12, 2);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					if (atributo != "")
						mes_final = getIdMesFromChar(atributo.c_str());

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 5 -  Ano de fim para restrição elétrica 
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(15, 4);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					if (atributo != "")
						ano_final = getIdAnoFromChar(atributo.c_str());

					//*************************************************
					//Determina periodos inicial e final
					//*************************************************

					//Manual: 
					//Caso os campos relativos à data inicial estiverem em branco e aqueles relacionados à data final estiverem preenchidos, 
					//os dados relativos ao limite do agrupamento serão considerados a partir do início do período de planejamento. 
					//Se a data inicial for anterior ao primeiro período de planejamento, essa será deslocada para o início do período de planejamento. 
					//Caso os campos relativos à data final estiverem em branco e aqueles relacionados à data inicial estiverem preenchidos, os dados relativos 
					//ao limite do agrupamento serão considerados até o final do horizonte de planejamento. Se a data final for posterior ao fim do período de planejamento, 
					//essa será deslocada para o final do horizonte de planejamento


					Periodo periodo_re_inicio;
					Periodo periodo_re_final;

					if (((mes_inicial == IdMes_Nenhum) || (ano_inicial == IdAno_Nenhum)) && ((mes_final == IdMes_Nenhum) || (ano_final == IdAno_Nenhum)))//Manual: Se os campos relacionados à data inicial e à data final estiverem em branco, a execução do programa será interrompida com uma mensagem de erro
						throw std::invalid_argument("Data inicial e final nao reportada");


					if ((mes_inicial == IdMes_Nenhum) || (ano_inicial == IdAno_Nenhum))
						periodo_re_inicio = horizonte_estudo_DECK.getIteradorInicial();

					else
						periodo_re_inicio = Periodo(mes_inicial, ano_inicial);


					if ((mes_final == IdMes_Nenhum) || (ano_final == IdAno_Nenhum))
						periodo_re_final = horizonte_estudo_DECK.getIteradorFinal();

					else
						periodo_re_final = Periodo(mes_final, ano_final);

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 6 -  Número do patamar de carga.
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(20, 1);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const IdPatamarCarga idPatamarCarga_lido = getIdPatamarCargaFromChar(atributo.c_str());

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 7 -  Valor da restrição elétrica (MWmédio). 
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(22, 15);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const double potencia_maxima = atof(atributo.c_str()); //O valor fornecido no campo7 deve ser sempre maior do que zero, e indica a capacidade máxima de geração permitida para o conjunto

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo Não obrigatório -  Nome da restrição elétrica
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					const int line_size = int(line.length());

					if (line_size > 37) {//37 refere-se ao tamanho da linha até o último campo obrigatório

						atributo = line.substr(37, line_size - 36);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						if (atributo != "") {

							const std::string nome_restricao_eletrica = atributo;

							a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).setAtributo(AttComumRestricaoEletrica_nome, nome_restricao_eletrica);

						}//if (atributo != "") {

					}//if (line_size > 37) {

					//*********************************************
					//Atualiza vetores nos SmartElementos
					//*********************************************

					SmartEnupla<IdPatamarCarga, double> potencia_maxima_patamar;

					if (idPatamarCarga_lido == IdPatamarCarga_Nenhum)
						potencia_maxima_patamar = SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, horizonte_estudo.getIteradorInicial(), IdPatamarCarga()), potencia_maxima));

					else {
						potencia_maxima_patamar = SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, horizonte_estudo.getIteradorInicial(), IdPatamarCarga()), 0.0));
						potencia_maxima_patamar.at(idPatamarCarga_lido) = potencia_maxima;
					}

					if (lista_RE_PMAX.at(idRestricaoEletrica).size() == 0)
						lista_RE_PMAX.at(idRestricaoEletrica) = SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo_DECK.getIteradorInicial(), std::vector<SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo_DECK.getIteradorFinal() - horizonte_estudo_DECK.getIteradorInicial() + 1, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, horizonte_estudo.getIteradorInicial(), IdPatamarCarga()), getdoubleFromChar("max")))));

					for (Periodo periodo = periodo_re_inicio; periodo <= periodo_re_final; periodo++) {

						for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= potencia_maxima_patamar.getIteradorFinal(); idPatamarCarga++) {

							if (potencia_maxima_patamar.at(idPatamarCarga) > 0.0)
								lista_RE_PMAX.at(idRestricaoEletrica).at(periodo).at(idPatamarCarga) = potencia_maxima_patamar.at(idPatamarCarga);

						} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= potencia_maxima_patamar.getIteradorFinal(); idPatamarCarga++) {

					} // for (Periodo periodo = periodo_restricao; periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo)) {

				}//while (true) {

				leituraArquivo.close();

			}//if (leituraArquivo.is_open()) {
			else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

			double sobreposicao_atraso_periodo_inicial = 0.0;

			if (desconsiderar_atraso_periodo_estudo_inicial)
				sobreposicao_atraso_periodo_inicial = horizonte_estudo.getIteradorInicial().atraso(horizonte_estudo_DECK.getIteradorInicial());

			for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= lista_RE_PMAX.getIteradorFinal(); idRestricaoEletrica++) {

				if (lista_RE_PMAX.at(idRestricaoEletrica).size() > 0) {

					a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).setMatriz_forced(AttMatrizRestricaoEletrica_potencia_maxima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, horizonte_estudo.getIteradorInicial(), IdPatamarCarga()), getdoubleFromChar("max")))));

					for (Periodo periodo_DECK = lista_RE_PMAX.at(idRestricaoEletrica).getIteradorInicial(); periodo_DECK <= lista_RE_PMAX.at(idRestricaoEletrica).getIteradorFinal(); periodo_DECK++) {

						for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							double sobreposicao = periodo.sobreposicao(periodo_DECK);

							if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
								sobreposicao += sobreposicao_atraso_periodo_inicial;

							if (sobreposicao > 0.0) {

								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= lista_RE_PMAX.at(idRestricaoEletrica).at(periodo_DECK).getIteradorFinal(); idPatamarCarga++) {

									if (lista_RE_PMAX.at(idRestricaoEletrica).at(periodo_DECK).at(idPatamarCarga) != getdoubleFromChar("max")) {//Somente periodos com valores de registros de potencia_maxima sao atualizados 

										double valor_antigo = a_dados.getElementoMatriz(idRestricaoEletrica, AttMatrizRestricaoEletrica_potencia_maxima, periodo, idPatamarCarga, double());

										if (valor_antigo == getdoubleFromChar("max"))//O primeiro valor a modificar pela restriçao deve ser zero
											valor_antigo = 0;

										a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).setElemento(AttMatrizRestricaoEletrica_potencia_maxima, periodo, idPatamarCarga, valor_antigo + sobreposicao * lista_RE_PMAX.at(idRestricaoEletrica).at(periodo_DECK).at(idPatamarCarga));

									}//if (lista_RE_PMAX.at(idRestricaoEletrica).at(periodo_DECK).at(idPatamarCarga) != getdoubleFromChar("max")) {
									
								} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= lista_RE_PMAX.at(idRestricaoEletrica).at(periodo_DECK).getIteradorFinal(); idPatamarCarga++) {

							} // if (sobreposicao > 0.0) {

						} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

					} // for (Periodo periodo_DECK = lista_RE_PMAX.at(idRestricaoEletrica).getIteradorInicial(); periodo_DECK <= lista_RE_PMAX.at(idRestricaoEletrica).getIteradorFinal(); periodo_DECK++) {

				} // if (lista_RE_PMAX.at(idRestricaoEletrica).size() > 0) {

			} // for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= lista_RE_PMAX.getIteradorFinal(); idRestricaoEletrica++) {

		}//if (considera_restricoes_eletricas == true) {

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_RE_201908_NW25: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::validacoes_NW(Dados & a_dados) {

	try {

		EntradaSaidaDados entradaSaidaDados;

		entradaSaidaDados.setDiretorioEntrada(a_dados.getAtributo(AttComumDados_diretorio_entrada_dados, std::string()));
		entradaSaidaDados.setDiretorioSaida(a_dados.getAtributo(AttComumDados_diretorio_saida_dados, std::string()));

		const std::string diretorio_att_operacionais = entradaSaidaDados.getDiretorioEntrada() + "";

		std::string diretorio_att_premissas = "";
		std::string diretorio_exportacao_pos_estudo = "";

		const TipoEstudo tipo_estudo = a_dados.getAtributo(AttComumDados_tipo_estudo, TipoEstudo());

		if (tipo_estudo == TipoEstudo_simulacao) {
			diretorio_att_premissas = entradaSaidaDados.getDiretorioEntrada() + "//Simulacao//AtributosPremissasNEWAVE//";
			diretorio_exportacao_pos_estudo = entradaSaidaDados.getDiretorioSaida() + "//Simulacao//AcoplamentoPosEstudo";
		}
		else {
			diretorio_att_premissas = entradaSaidaDados.getDiretorioEntrada() + "//Otimizacao//AtributosPremissasNEWAVE";
			diretorio_exportacao_pos_estudo = entradaSaidaDados.getDiretorioSaida() + "//Otimizacao//AcoplamentoPosEstudo";
		}

		if (strCompara(a_dados.getAtributo(AttComumDados_diretorio_importacao_pos_estudo, std::string()), "nenhum"))
			diretorio_exportacao_pos_estudo = "nenhum";

		const bool imprimir_att_operacionais_sem_recarregar = true;

		a_dados.validacaoDadosAttComum();

		a_dados.setAtributo(AttComumDados_diretorio_entrada_dados, entradaSaidaDados.getDiretorioEntrada());
		a_dados.setAtributo(AttComumDados_diretorio_saida_dados,   entradaSaidaDados.getDiretorioSaida());

		a_dados.validacao_operacional_Dados(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		a_dados.validacao_operacional_Submercado(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		a_dados.validacao_operacional_Intercambio(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		a_dados.validacao_operacional_Termeletrica(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		aplicarModificacoesUHE(a_dados);

		a_dados.validacao_operacional_Hidreletrica(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		a_dados.validacao_operacional_Intercambio_Hidraulico(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		a_dados.validacao_operacional_BaciaHidrografica(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		a_dados.validacao_operacional_RestricaoEletrica(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		a_dados.validacao_operacional_AgrupamentoIntercambio(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		a_dados.validacao_operacional_ProcessoEstocasticoHidrologico(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, diretorio_exportacao_pos_estudo, imprimir_att_operacionais_sem_recarregar);

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::validacoes_NW(a_dados): \n" + std::string(erro.what())); }

} // void LeituraCEPEL::validacoes_NW(Dados & a_dados){




