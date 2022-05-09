#include "C_LeituraCEPEL.h"
#include "C_EntradaSaidaDados.h"
#include <iostream>
#include <fstream>

//////////////////////////////////////////////////////////////////////////
//Parâmetros para simular o CP em modo DC

const bool somente_volume_meta_no_ultimo_estagio = false;
const bool leitura_vazao_evaporada_meta = false;
const bool teste_hidreletrica_volume_meta = false;
const bool teste_hidreletrica_vazao_turbinada_disponivel_meta = false;
const bool teste_hidreletrica_potencia_disponivel_meta = false;
const bool teste_termeletrica_potencia_disponivel_meta = false;
/////////////////////////////////////////////////////////////////////////


IdEstagio idEstagioMaximo = IdEstagio_3; //Decomposição adoptada para o CP

const IdEstagio estagio_acoplamento_pre_estudo = IdEstagio_2; //Acoplamento com o PD

SmartEnupla<IdEstagio, Periodo> horizonte_otimizacao_DC; //Somente é usado na leitura de dados do DECOMP para mapear a data do estágio DC e carregar a info no horizonte_estudo do SPT
														 //Os iteradores horizonte_otimizacao_DC são o horizonte_estocastico (forma como realiza a variável aleatória)

SmartEnupla<Periodo, bool> horizonte_processo_estocastico;
SmartEnupla<Periodo, int> numero_realizacoes_por_periodo;

std::vector<std::vector<double>> vazao_no_posto; //Linha: nó / Coluna: posto
SmartEnupla<IdVariavelAleatoria, SmartEnupla <Periodo, double>>  valor_afluencia_estagios_DC_anteriores_estagio_acoplamento_pre_estudo; //Valores de afluência dos estágios DC com t < estagio_acoplamento_pre_estudo

Periodo data_execucao; //Periodo no qual o DC é executado (dia anterior ao periodo_inicial do horizonte de estudo)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Afluências passadas 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Notas:
//1. Se uma hidrelétrica tem mudança de posto entre o DECOMP  e NEWAVE -> As afluências observadas vão ser obtidas dos arquivos VAZOES.DAT e prevs.RVX do GEVAZP com os postos do DECOMP
//2. Se uma hidrelétrica NÃO tem mudança de posto entre o DECOMP  e NEWAVE -> As afluências observadas vão ser obtidas do arquivo VAZOES.RVX do DECOMP com os postos do DECOMP

SmartEnupla<int, SmartEnupla <Periodo, double>>  valor_afluencia_passadas_GEVAZP; //Valores de afluência dos meses e semanas passadas
SmartEnupla<int, SmartEnupla <Periodo, double>>  valor_afluencia_passadas_DECOMP; //Valores de afluência dos meses e semanas passadas
SmartEnupla<int, SmartEnupla <Periodo, double>>  valor_afluencia_historica; //Valores de afluência dos meses e semanas passadas

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Vazao defluente historica minima
SmartEnupla<int, SmartEnupla<Periodo, double>> porcentagem_vazao_minima_historica_REE;
int maior_ONS_REE = 0;
bool inicializa_vetor_porcentagem_vazao_minima_historica_REE = true;
////////////////////////////////////////////////////////////////////////////////////////

void LeituraCEPEL::leitura_DECOMP(Dados& a_dados, const std::string a_diretorio) {

	try {
		bool readPoliJusHidr_dat = true;
		const std::string revisao = leitura_CASO_201906_DC29(a_diretorio + "//CASO.DAT");

		//////////////////////////
		//Dados
		///////////////////////
		instancia_dados_preConfig(a_dados, a_dados.getAtributo(AttComumDados_diretorio_entrada_dados, std::string()) + "_PRECONFIG");

		a_dados.setAtributo(AttComumDados_tipo_geracao_cenario_hidrologico, TipoGeracaoCenario_serie_informada);
		a_dados.setAtributo(AttComumDados_mes_penalizacao_volume_util_minimo, IdMes_Nenhum);

		////////////////////////////////////////////////////////////////////
		//Hidrelétricas
		////////////////////////////////////////////////////////////////////

		instancia_hidreletricas_preConfig(a_dados, a_dados.getAtributo(AttComumDados_diretorio_entrada_dados, std::string()) + "_PRECONFIG");
		std::ifstream myfile(a_diretorio + "/polinjus.dat");
		if (myfile.is_open()) { readPoliJusHidr_dat = false; }

		if (a_dados.getMaiorId(IdHidreletrica()) > IdHidreletrica_Nenhum)
			hidreletricasPreConfig_instanciadas = true;

		if (!hidreletricasPreConfig_instanciadas)
			instanciar_hidreletricas_from_VAZOES_201906_DC29(a_dados, a_diretorio + "//VAZOES." + revisao);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Filosofia: 
		//1. Ler VAZOES para definir os horizontes de estudo e otimização, armazenando os valores de afluência de todos os postos em vetor local
		//2. Ler DADGER e ver as modificações "NUMPOS" para complementar o registro de postos do HIDR.DAT
		//3. Atualizar os valores de VARIAVEL_ALEATORIA_INTERNA_cenarios_realizacao_espaco_amostral e VARIAVEL_ALEATORIA_residuo_espaco_amostral
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		const bool imprimeArquivoVazoesRVX = false;

		if (imprimeArquivoVazoesRVX)//Somente para teste
			imprime_VAZOES_201906_DC29(a_dados, a_diretorio + "//VAZOES." + revisao);

		definir_horizonte_otimizacao_DC_from_VAZOES_201906_DC29(a_dados, a_diretorio + "//VAZOES." + revisao);

		defineHorizontes_CP(a_dados);

		instanciar_processo_estocastico_CP(a_dados);

		ler_afluencia_passada_from_VAZOES_201906_DC29(a_dados, a_diretorio + "//VAZOES." + revisao);

		//Leitura da árvore de cenários

		if (a_dados.processoEstocastico_hidrologico.getSizeMatriz(AttMatrizProcessoEstocastico_variavelAleatoria_realizacao) == 0 && a_dados.processoEstocastico_hidrologico.getSizeMatriz(AttMatrizProcessoEstocastico_no_probabilidade) == 0 && int(numero_realizacoes_por_periodo.size()) == 0)
			ler_vazao_probabilidade_estrutura_arvore_cenarios_from_VAZOES_201906_DC29(a_dados, a_diretorio + "//VAZOES." + revisao);//Lê árvore DC

		//////////////////////////////////////////////////////////////////////////
		//Set numero_aberturas (vai mudar com a estrutura árvore)
		//////////////////////////////////////////////////////////////////////////

		SmartEnupla<IdEstagio, int> vetor_numero_realizacoes(IdEstagio_1, std::vector<int>(int(a_dados.getVetor(AttVetorDados_horizonte_otimizacao, IdEstagio(), Periodo()).getIteradorFinal()), 1));
		vetor_numero_realizacoes.setElemento(a_dados.getVetor(AttVetorDados_horizonte_otimizacao, IdEstagio(), Periodo()).getIteradorFinal(), numero_realizacoes_por_periodo.getElemento(numero_realizacoes_por_periodo.getIteradorFinal()));
		a_dados.setVetor(AttVetorDados_numero_aberturas, vetor_numero_realizacoes);

		////////

		//Cria o horizonte_processo_estocastico

		for (IdEstagio idEstagio = IdEstagio_1; idEstagio <= a_dados.getVetor(AttVetorDados_horizonte_otimizacao, IdEstagio(), Periodo()).getIteradorFinal(); idEstagio++) {

			std::vector<Periodo> periodos_processo_estocastico = numero_realizacoes_por_periodo.getIteradores(a_dados.getVetor(AttVetorDados_horizonte_otimizacao, IdEstagio(), Periodo()).getElemento(idEstagio));

			for (int pos = 0; pos < int(periodos_processo_estocastico.size()); pos++)
				horizonte_processo_estocastico.addElemento(periodos_processo_estocastico.at(pos), true);

		}//for (IdEstagio idEstagio = IdEstagio_1; idEstagio <= horizonte_otimizacao.getIteradorFinal(); idEstagio++) {

		define_numero_cenarios_CP(a_dados);

		if (a_dados.processoEstocastico_hidrologico.getSizeMatriz(AttMatrizProcessoEstocastico_mapeamento_arvore_cenarios) == 0)
			instanciar_mapeamento_arvore_cenarios_simetrica(a_dados);

		if (a_dados.processoEstocastico_hidrologico.getSizeMatriz(AttMatrizProcessoEstocastico_no_antecessor) == 0)
			instanciar_no_antecessor_arvore_simetrica(a_dados);

		if (a_dados.processoEstocastico_hidrologico.getSizeMatriz(AttMatrizProcessoEstocastico_no_realizacao) == 0)
			instanciar_no_realizacao_arvore_simetrica(a_dados);

		if (a_dados.processoEstocastico_hidrologico.getSizeMatriz(AttMatrizProcessoEstocastico_no_probabilidade) == 0)
			instanciar_no_probabilidade_arvore_simetrica(a_dados);

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		instanciar_membros_das_hidreletricas_instanciadas(a_dados);

		leitura_DADGER_201906_DC29(a_dados, a_diretorio + "//DADGER." + revisao);

		sequenciarRestricoesEletricas(a_dados);
		sequenciarRestricoesHidraulicas(a_dados);

		leitura_DADGNL_201906_DC29_A(a_dados, a_diretorio + "//DADGNL." + revisao, a_diretorio + "//relgnl." + revisao, a_diretorio + "//DadosAdicionais//relgnl." + revisao);// Ajustar que a geração comandada pode estar fora do horizonte_estudo
		leitura_DADGNL_201906_DC29_B(a_dados, a_diretorio, "DADGNL." + revisao);// Ajustar que a geração comandada pode estar fora do horizonte_estudo

		leitura_CADUSIH_201904_NW25_DC29_DES16(a_dados, a_diretorio + "//HIDR.DAT", hidreletricasPreConfig_instanciadas, readPoliJusHidr_dat);

		leitura_PERDAS_201906_DC29(a_dados, a_diretorio + "//PERDAS.DAT");

		leitura_MLT_201906_DC29(a_dados, a_diretorio + "//MLT.DAT");

		leitura_POLIJUS(a_dados, a_diretorio + "//polinjus.dat");

		leitura_setPercentualVolumeCalculoFPH(a_dados);

		//Instancia intercâmbio hidráulicos das jusantes_desvio
		//adicionaIntercambiosHidraulicosApartirJusanteDesvio(a_dados);

		adicionaLimitesDesvioApartirJusanteDesvio(a_dados);

		////////////////////////////////////////////////////////////////////////////////
		//Determina se o arquivo de tendencia imprime os registros das vazões observadas
		////////////////////////////////////////////////////////////////////////////////
		const bool criar_tendencia_temporal_com_vazoes_observadas = criar_tendencia_temporal_com_vazoes_observadas_CP(a_diretorio, revisao);

		if (criar_tendencia_temporal_com_vazoes_observadas) {

			std::string arquivo_vazoes_dat = a_diretorio + "//VAZOES.DAT";

			std::ifstream leituraArquivo_vazoes_dat(arquivo_vazoes_dat);

			if (!leituraArquivo_vazoes_dat.is_open())
				arquivo_vazoes_dat = a_diretorio + "//DadosAdicionais//VAZOES.DAT";
			else {
				leituraArquivo_vazoes_dat.clear();
				leituraArquivo_vazoes_dat.close();
			}//else {

			leitura_TENDENCIA_VAZOES_MENSAIS_GEVAZP(a_dados, arquivo_vazoes_dat);

			ler_historico_afluencia_from_VAZOES_201906_DC29(a_dados, arquivo_vazoes_dat);

			const bool imprimeArquivoVazoesDAT = false;

			if (imprimeArquivoVazoesDAT)//Somente para teste
				imprime_VAZOES_DAT();

			if (revisao != "rv0") {

				std::string arquivo_prevs = a_diretorio + "//prevs";

				std::ifstream leituraArquivo_prevs(arquivo_prevs);

				if (!leituraArquivo_prevs.is_open())
					arquivo_prevs = a_diretorio + "//DadosAdicionais//prevs";
				else {
					leituraArquivo_prevs.clear();
					leituraArquivo_prevs.close();
				}//else {

				leitura_TENDENCIA_VAZOES_SEMANAIS_GEVAZP(a_dados, arquivo_prevs, revisao);

			}

		}//if (criar_tendencia_temporal_com_vazoes_observadas) {

		////////////////////////////////////////////////////////////////////////////////

		validacoes_DC(a_dados, a_diretorio, revisao);

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_DECOMP: \n" + std::string(erro.what())); }

}

std::string LeituraCEPEL::leitura_CASO_201906_DC29(std::string nomeArquivo)
{

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;
		std::string revisao;

		////////////////////////////////////////////////////////

		if (leituraArquivo.is_open()) {

			std::getline(leituraArquivo, line);

			strNormalizada(line);

			atributo = line.substr(0, 80);
			atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

			revisao = atributo;

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

		return revisao;

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_CASO_201906_DC29: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_POLIJUS(Dados& a_dados, std::string a_nomeArquivo) {

	try {
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());
		std::vector<int>lista_codigo_usina;
		std::vector<int>lista_indice_polinomio;
		std::vector<int>lista_numero_polinomios;
		std::vector<double>lista_h_jus_ref;


		std::ifstream myfile(a_nomeArquivo);
		std::string line;
		std::string atributo;
		if (myfile.is_open()) {

			while (std::getline(myfile, line)) {
				strNormalizada(line);

				if (line.substr(0, 8) == "CURVAJUS") {

					atributo = line.substr(11, 4);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					lista_codigo_usina.push_back(std::stoi(atributo));

					atributo = line.substr(20, 3);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					lista_indice_polinomio.push_back(std::stoi(atributo));

					atributo = line.substr(27, 10);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					lista_h_jus_ref.push_back(std::stod(atributo));

					atributo = line.substr(39, 3);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					lista_numero_polinomios.push_back(std::stoi(atributo));
				}

				if (line.substr(0, 6) == "PPPJUS") {

					atributo = line.substr(11, 4);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const int codigo_usina = std::stoi(atributo);

					atributo = line.substr(20, 3);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const int indici_poli = std::stoi(atributo);
					const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

					if (idHidreletrica != IdHidreletrica_Nenhum) {
						for (int i = 0; i < int(lista_indice_polinomio.size()); i++) {
							if ((lista_codigo_usina.at(i) == codigo_usina) && (lista_indice_polinomio.at(i) == indici_poli)) {
								IdPolinomioJusante idPolinomio = IdPolinomioJusante(a_dados.getMaiorId(idHidreletrica, IdPolinomioJusante()) + 1);
								PolinomioJusante polinomioJusante;
								polinomioJusante.setAtributo(AttComumPolinomioJusante_idPolinomioJusante, idPolinomio);
								polinomioJusante.setAtributo(AttComumPolinomioJusante_altura_jusante_ref, lista_h_jus_ref.at(i));
								polinomioJusante.setAtributo(AttComumPolinomioJusante_altura_ref, 0.0);
								polinomioJusante.setAtributo(AttComumPolinomioJusante_defluencia_minima, std::stod(line.substr(28, 20)));

								double defluencia_maxima = std::stod(line.substr(49, 20));

								if (defluencia_maxima >= 1e10)
									defluencia_maxima = getdoubleFromChar("max");

								polinomioJusante.setAtributo(AttComumPolinomioJusante_defluencia_maxima, defluencia_maxima);
								polinomioJusante.setVetor(AttVetorPolinomioJusante_altura_ref, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
								polinomioJusante.setVetor(AttVetorPolinomioJusante_altura_jusante_ref, SmartEnupla<Periodo, double>(horizonte_estudo, lista_h_jus_ref.at(i)));

								//////////////////////////////////////

								atributo = line.substr(70, 20);
								polinomioJusante.setVetor(AttVetorPolinomioJusante_coeficiente_0, SmartEnupla<Periodo, double>(horizonte_estudo, std::stod(atributo.substr(0, int(atributo.find("D")))) * std::pow(10, std::stod(atributo.substr(int(atributo.find("D")) + 1, 19 - int(atributo.find("D")))))));

								//////////////////////////////////////

								atributo = line.substr(91, 20);
								polinomioJusante.setVetor(AttVetorPolinomioJusante_coeficiente_1, SmartEnupla<Periodo, double>(horizonte_estudo, std::stod(atributo.substr(0, int(atributo.find("D")))) * std::pow(10, std::stod(atributo.substr(int(atributo.find("D")) + 1, 19 - int(atributo.find("D")))))));

								//////////////////////////////////////

								atributo = line.substr(112, 20);
								polinomioJusante.setVetor(AttVetorPolinomioJusante_coeficiente_2, SmartEnupla<Periodo, double>(horizonte_estudo, std::stod(atributo.substr(0, int(atributo.find("D")))) * std::pow(10, std::stod(atributo.substr(int(atributo.find("D")) + 1, 19 - int(atributo.find("D")))))));

								//////////////////////////////////////

								atributo = line.substr(133, 20);
								polinomioJusante.setVetor(AttVetorPolinomioJusante_coeficiente_3, SmartEnupla<Periodo, double>(horizonte_estudo, std::stod(atributo.substr(0, int(atributo.find("D")))) * std::pow(10, std::stod(atributo.substr(int(atributo.find("D")) + 1, 19 - int(atributo.find("D")))))));

								//////////////////////////////////////

								atributo = line.substr(154, 20);
								polinomioJusante.setVetor(AttVetorPolinomioJusante_coeficiente_4, SmartEnupla<Periodo, double>(horizonte_estudo, std::stod(atributo.substr(0, int(atributo.find("D")))) * std::pow(10, std::stod(atributo.substr(int(atributo.find("D")) + 1, 19 - int(atributo.find("D")))))));

								//////////////////////////////////////
								a_dados.vetorHidreletrica.att(idHidreletrica).vetorPolinomioJusante.add(polinomioJusante);
								break;
							}

						}//for (int i = 0; i <= size(lista_indice_polinomio); i++) {	
					}//if (idHidreletrica != IdHidreletrica_Nenhum) {
				}//if (line.substr(0, 8) == "PPPJUS") {

			}//while (std::getline(myfile, line)) {
			myfile.close();
		}//if (myfile.is_open()) {
	}
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_POLIJUS: \n" + std::string(erro.what())); }
}

void LeituraCEPEL::leitura_setPercentualVolumeCalculoFPH(Dados& a_dados) {

	try {
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const IdHidreletrica maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

		for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

			double percentual = 0.2;
			Periodo periodoFinalDC = horizonte_otimizacao_DC.at(horizonte_otimizacao_DC.getIteradorFinal());
			if (periodo.sobreposicao(periodoFinalDC) > 0)
				percentual = 0.5;

			for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {
				if (a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.isInstanciado(IdFuncaoProducaoHidreletrica_1) == false) {
					FuncaoProducaoHidreletrica Fph;
					Fph.setAtributo(AttComumFuncaoProducaoHidreletrica_idFuncaoProducaoHidreletrica, IdFuncaoProducaoHidreletrica_1);
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.add(Fph);
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).setVetor(AttVetorFuncaoProducaoHidreletrica_percentual_volume_minimo, SmartEnupla<Periodo, double>(horizonte_estudo, 0.2));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).setVetor(AttVetorFuncaoProducaoHidreletrica_percentual_volume_maximo, SmartEnupla<Periodo, double>(horizonte_estudo, 0.2));
				}
				a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).setElemento(AttVetorFuncaoProducaoHidreletrica_percentual_volume_minimo, periodo, percentual);
				a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).setElemento(AttVetorFuncaoProducaoHidreletrica_percentual_volume_maximo, periodo, percentual);
			}
		}

	}
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_setPercentualVolumeCalculoFPH: \n" + std::string(erro.what())); }
}

void LeituraCEPEL::instanciar_membros_das_hidreletricas_instanciadas(Dados& a_dados) {

	try {

		const IdHidreletrica  maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

		for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

			//Set Atributo
			a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica_por_usina);

			/////////////////////////////////////////////////
			//inicializa afluência

			if (!a_dados.vetorHidreletrica.att(idHidreletrica).vetorAfluencia.isInstanciado(IdAfluencia_vazao_afluente)) {
				Afluencia afluencia;
				afluencia.setAtributo(AttComumAfluencia_idAfluencia, IdAfluencia_vazao_afluente);
				a_dados.vetorHidreletrica.att(idHidreletrica).vetorAfluencia.add(afluencia);
			}

			/////////////////////////////////////////////////

			//INSTANCIA O RESERVATÓRIO, SE NÃO ESTIVER INSTANCIADO 
			if (a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.isInstanciado(IdReservatorio_1) == false) {
				Reservatorio reservatorio;
				reservatorio.setAtributo(AttComumReservatorio_idReservatorio, IdReservatorio_1);
				a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.add(reservatorio);

			}//if (a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.isInstanciado(IdReservatorio_1) == false) {

			/////////////////////////////////////////////////

			VariavelAleatoria variavelAleatoria;

			const IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria(idHidreletrica);

			variavelAleatoria.setAtributo(AttComumVariavelAleatoria_idVariavelAleatoria, idVariavelAleatoria);
			variavelAleatoria.setAtributo(AttComumVariavelAleatoria_ordem_maxima_coeficiente_auto_correlacao, 0);
			variavelAleatoria.setAtributo(AttComumVariavelAleatoria_tipo_coeficiente_auto_correlacao, TipoValor_positivo_e_negativo);

			a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.add(variavelAleatoria);

			/////////////////////////////////////////////////

			valor_afluencia_estagios_DC_anteriores_estagio_acoplamento_pre_estudo.addElemento(idVariavelAleatoria);

			/////////////////////////////////////////////////

			if (hidreletricasPreConfig_instanciadas) {

				//Modo carrega hidreletricasPreConfig:
				//Inicializa listas de jusante e jusante_desvio, logo são atualizadas no HIDR.dat + modificações NUMJUS para validar com a configuração do CP

				lista_jusante_hidreletrica.addElemento(idHidreletrica, IdHidreletrica_Nenhum);
				lista_jusante_desvio_hidreletrica.addElemento(idHidreletrica, IdHidreletrica_Nenhum);

			}//if (hidreletricasPreConfig_instanciadas) {

			//////////////////////////////////////////////

			lista_hidreletrica_NPOSNW.addElemento(idHidreletrica, -1);//Este valor é atualizado logo de aplicar as modificações NPOSNW

			/////////////////////////////////////////////
			if (!hidreletricasPreConfig_instanciadas)
				lista_codigo_ONS_REE.addElemento(idHidreletrica, 0);

		}//for(IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++)

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::instanciar_membros_das_hidreletricas_instanciadas: \n" + std::string(erro.what())); }

}

bool LeituraCEPEL::criar_tendencia_temporal_com_vazoes_observadas_CP(const std::string a_diretorio, const std::string a_revisao)
{
	try {
		//A tendência temporal para acoplar com o MP, os valores mensais/semanais observados são obtidos dos arquivos do GEVAZP: VAZOES.DAT e prevs.RVX

		const std::string vazoes_dat_raiz = a_diretorio + "//VAZOES.DAT";
		const std::string prevs_raiz = a_diretorio + "//prevs." + a_revisao;

		const std::string vazoes_dat_adicional = a_diretorio + "//DadosAdicionais//VAZOES.DAT";
		const std::string prevs_adicional = a_diretorio + "//DadosAdicionais//prevs." + a_revisao;

		std::ifstream leituraArquivo_vazoes_raiz(vazoes_dat_raiz);
		std::ifstream leituraArquivo_prevs_raiz(prevs_raiz);

		std::ifstream leituraArquivo_vazoes_adicional(vazoes_dat_adicional);
		std::ifstream leituraArquivo_prevs_adicional(prevs_adicional);

		if (!leituraArquivo_vazoes_raiz.is_open() && !leituraArquivo_vazoes_adicional.is_open())
			return false;

		if (a_revisao == "rv0")
			return true;
		else {

			leituraArquivo_vazoes_raiz.clear();
			leituraArquivo_vazoes_raiz.close();

			leituraArquivo_vazoes_adicional.clear();
			leituraArquivo_vazoes_adicional.close();

			if (!leituraArquivo_prevs_raiz.is_open() && !leituraArquivo_prevs_adicional.is_open())
				return false;

		}//else {

		leituraArquivo_prevs_raiz.clear();
		leituraArquivo_prevs_raiz.close();

		leituraArquivo_prevs_adicional.clear();
		leituraArquivo_prevs_adicional.close();

		return true;

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("criar_tendencia_temporal_com_vazoes_observadas_CP: \n" + std::string(erro.what())); }

}//bool LeituraCEPEL::criar_tendencia_temporal_com_vazoes_observadas_CP(const std::string a_revisao)

void LeituraCEPEL::imprime_VAZOES_201906_DC29(Dados& a_dados, std::string nomeArquivo) {

	try {

		//Nota registro 2 é ignorado porque as usinas hidréletricas neste ponto já foram instanciadas

		std::ofstream     fp_out;

		std::string arquivo = "VAZOES.csv";
		fp_out.open(arquivo.c_str(), std::ios_base::out); //Função para criar um arquivo


		const int tamanho = 320;
		int intLeitura[tamanho];
		float floatLeitura[tamanho];

		int registro = 0;
		int numNos = 0;
		int NPROB = 0;

		int  registroProbabilidade_controle = 0;
		bool registroProbabilidade = true;

		///////////////////////////////////////////////////////////////////
		std::ifstream leituraArquivo;
		leituraArquivo.open(nomeArquivo, std::ios_base::in | std::ios::binary);

		if (leituraArquivo.is_open()) {

			while (!(leituraArquivo.eof())) {

				registro++;

				///////////////////////////////////////
				//Identifica o tipo do registro
				///////////////////////////////////////

				if (registro >= 4 && registroProbabilidade == true)
					leituraArquivo.read((char*)floatLeitura, sizeof(floatLeitura));
				else
					leituraArquivo.read((char*)intLeitura, sizeof(intLeitura));

				if (registro == 1) {

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Registro 1 -  total de postos considerados , número de estágios considerados  e número de aberturas em cada estágio
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					const int numEstagios_DC = intLeitura[1]; // Número de estágios do problema

					//Número de aberturas por estágio do DECOMP

					//numero_realizacoes_DC(numEstagios_DC);//Número de realizações por estágio da árvore de cenários

					//Determina o número de nós do problema
					for (int pos = 2; pos < 2 + numEstagios_DC; pos++)
						numNos += intLeitura[pos]; //Número de nós por estágio


					//Define o número de registros para a probabilidade
					NPROB = numNos / 320;
					NPROB += 1;

				}//if (registro == 1) {

				//Imprime os valores
				for (int pos = 0; pos < tamanho; pos++) {

					if (registro >= 4 && registroProbabilidade == true)
						fp_out << floatLeitura[pos] << ";";
					else
						fp_out << intLeitura[pos] << ";";

				}//for (int pos = 0; pos < 320; pos++) {

				fp_out << std::endl; //Passa de linha

				if (registro >= 4 && registroProbabilidade == true) {

					registroProbabilidade_controle++;

					if (NPROB == registroProbabilidade_controle)
						registroProbabilidade = false;

				}//if (registro >= 4 && registroProbabilidade == true) {

			}//while (!(leituraArquivo.eof())) {

			fp_out.close();

			leituraArquivo.clear();
			leituraArquivo.close();

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::imprime_VAZOES_201906_DC29: \n" + std::string(erro.what())); }

}


void LeituraCEPEL::imprime_VAZOES_DAT() {

	try {

		std::ofstream     fp_out;

		std::string arquivo = "VAZOES_DAT.csv";
		fp_out.open(arquivo.c_str(), std::ios_base::out); //Função para criar um arquivo

		const int numero_postos = int(valor_afluencia_historica.size());

		const Periodo periodo_inicial = valor_afluencia_historica.at(0).getIteradorInicial();
		const Periodo periodo_final = valor_afluencia_historica.at(0).getIteradorFinal();

		for (Periodo periodo = periodo_inicial; periodo <= periodo_final; valor_afluencia_historica.at(0).incrementarIterador(periodo)) {

			for (int posto = 0; posto < numero_postos; posto++)
				fp_out << valor_afluencia_historica.at(posto).getElemento(periodo) << ";";
					
			fp_out << std::endl; //Passa de linha

		}//for (int posto = 0; posto < numero_postos; posto++) {

		fp_out.close();

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::imprime_VAZOES_DAT: \n" + std::string(erro.what())); }

}


void LeituraCEPEL::ler_vazao_probabilidade_estrutura_arvore_cenarios_from_VAZOES_201906_DC29(Dados& a_dados, std::string nomeArquivo) {

	try {

		int mesInicial_PMO;
		int anoInicial_PMO;
		int diasNoSeguinteMes;

		Periodo periodo_semana_PMO; //Começa na primeira semana do PMO

		int revisao; //Dado para controlar a leitura das afluências históricas

		const int tamanho = 320;
		int intLeitura[tamanho];
		float floatLeitura[tamanho];

		int registro = 0;
		int numNos = 0;
		int NPROB; //Número de registros de probabilidade

		int numUHE;
		int numPostos;

		int  registroProbabilidade_controle = 0;
		bool registroProbabilidade = true;

		SmartEnupla <IdRealizacao, double> probabilidade_abertura; //Probabilidade das aberturas do último estágio no DC //Apagar em algum momento

		int conteio_registro_vazoes = 0;

		int conteio_registro_afluencias_passadas = 0;

		int dia_afluencia_historica = 1; //O dia acopla todos os periodos semanais para a leitura das afluências_incremental_tendencia mensais e semanais

		int conteio_realizacoes = 0;

		int conteio_estagio = 0;

		bool registroAfluenciasPassadas = false;

		///////////////////////////////////////////////////////////////////
		std::ifstream leituraArquivo;
		leituraArquivo.open(nomeArquivo, std::ios_base::in | std::ios::binary);

		if (leituraArquivo.is_open()) {

			while (!(leituraArquivo.eof())) {

				registro++;

				///////////////////////////////////////
				//Identifica o tipo do registro
				///////////////////////////////////////

				if (registro >= 4 && registroProbabilidade == true)
					leituraArquivo.read((char*)floatLeitura, sizeof(floatLeitura));
				else
					leituraArquivo.read((char*)intLeitura, sizeof(intLeitura));


				if (registro == 1) {

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Registro 1 -  total de postos considerados , número de estágios considerados  e número de aberturas em cada estágio
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					numUHE = intLeitura[0]; // Total de postos considerados
					const int numEstagios_DC = intLeitura[1]; // Número de estágios do problema

					//Número de aberturas por estágio do DECOMP

					//numero_realizacoes_DC(numEstagios_DC);//Número de realizações por estágio da árvore de cenários

					//Determina o número de nós do problema
					for (int pos = 2; pos < 2 + numEstagios_DC; pos++) {
						numNos += intLeitura[pos]; //Número de nós por estágio

						const IdEstagio idEstagio = getIdEstagioFromChar(getString(pos - 1).c_str());

						numero_realizacoes_por_periodo.addElemento(horizonte_otimizacao_DC.getElemento(idEstagio), intLeitura[pos]);

					}//for (int pos = 2; pos < 2 + numEstagios_DC; pos++) {

					//Inicializa probabilidade_abertura

					const int numero_realizacoes = numero_realizacoes_por_periodo.getElemento(numero_realizacoes_por_periodo.getIteradorFinal());

					for (int realizacao = 1; realizacao <= numero_realizacoes; realizacao++) {

						const IdRealizacao idRealizacao = IdRealizacao(realizacao);
						probabilidade_abertura.addElemento(idRealizacao, 0.0);

					}//for (int realizacao = 1; realizacao <= numero_realizacoes; realizacao++) {

					//Define o número de registros para a probabilidade
					NPROB = int(numNos / 320);
					NPROB += 1;

				}//if (registro == 1) {
				else if (registro == 3) {

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Registro 3 - número de semanas completas, número de dias que devem ser excluídos do estágio seguinte 
					//             ao mês inicial decomposto em semanas, índice do mês inicial do estudo  e ano do mês inicial do estudo
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					const int numSemanasEstudo = intLeitura[0];
					diasNoSeguinteMes = intLeitura[1];
					mesInicial_PMO = intLeitura[2];
					anoInicial_PMO = intLeitura[3];
					numPostos = intLeitura[4];
					revisao = intLeitura[5];

				}//else if (registro == 3) {
				else if (registro >= 4 && registroProbabilidade == true) {

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Registros 4 e subsequentes : probabilidades associadas a cada nó
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					int pos = 0;

					while (true) {

						if (pos == 320 || floatLeitura[pos] == 0)
							break;

						char buffer[100];
						std::snprintf(buffer, sizeof(buffer), "%f", floatLeitura[pos]);

						const double prob = std::atof(buffer);

						conteio_estagio++;

						if (registro == 4) {

							if (pos + 1 >= int(horizonte_otimizacao_DC.size())) {//Somente armazena as probabilidades referentes às aberturas (último mês)

								conteio_realizacoes++;

								const IdRealizacao idRealizacao = IdRealizacao(conteio_realizacoes);
								probabilidade_abertura.setElemento(idRealizacao, prob);

								a_dados.processoEstocastico_hidrologico.addElemento(AttMatrizProcessoEstocastico_no_probabilidade, horizonte_otimizacao_DC.getElemento(horizonte_otimizacao_DC.getIteradorFinal()), IdNo(conteio_realizacoes + 1), prob);

							}//if (pos + 1 >= int(horizonte_otimizacao_DC.size())) {
							else
								a_dados.processoEstocastico_hidrologico.addElemento(AttMatrizProcessoEstocastico_no_probabilidade, horizonte_otimizacao_DC.getElemento(IdEstagio(conteio_estagio)), IdNo_1, prob);

						}//if (registro == 4) {
						else {

							conteio_realizacoes++;

							const IdRealizacao idRealizacao = IdRealizacao(conteio_realizacoes);
							probabilidade_abertura.setElemento(idRealizacao, prob);

							a_dados.processoEstocastico_hidrologico.addElemento(AttMatrizProcessoEstocastico_no_probabilidade, horizonte_otimizacao_DC.getElemento(horizonte_otimizacao_DC.getIteradorFinal()), IdNo(conteio_realizacoes + 1), prob);

						}//else {

						pos++;

					}//while (true) 

					bool arvoreEquiprovavel = false;

					if (pos == 0) {

						arvoreEquiprovavel = true;

						//Manual: Se estes valores forem nulos as vazões serão consideradas equiprováveis
						//Árvore equiprovável

						//Somente registra as probabilidades associadas às aberturas (último mes)

						const int numero_realizacoes = numero_realizacoes_por_periodo.getElemento(numero_realizacoes_por_periodo.getIteradorFinal());

						for (int realizacao = 0; realizacao < numero_realizacoes; realizacao++)
							probabilidade_abertura.setElemento(IdRealizacao(realizacao + 1), std::pow(numero_realizacoes, -1));

						////////////////////////////////////////

						const IdEstagio idEstagio_inicial = horizonte_otimizacao_DC.getIteradorInicial();
						const IdEstagio idEstagio_final = horizonte_otimizacao_DC.getIteradorFinal();

						int numero_nos = 1;

						for (IdEstagio idEstagio = idEstagio_inicial; idEstagio <= idEstagio_final; idEstagio++) {

							numero_nos *= numero_realizacoes_por_periodo.getElemento(horizonte_otimizacao_DC.getElemento(idEstagio));

							const double prob = 1 / numero_realizacoes_por_periodo.getElemento(horizonte_otimizacao_DC.getElemento(idEstagio));

							for (int no = 0; no < numero_nos; no++)
								a_dados.processoEstocastico_hidrologico.addElemento(AttMatrizProcessoEstocastico_no_probabilidade, horizonte_otimizacao_DC.getElemento(idEstagio), IdNo(no + 2), prob);

						}//for (IdEstagio idEstagio = idEstagio_inicial; idEstagio <= idEstagio_final; idEstagio++) {

					}//if (pos == 0) {

					/////////////////////////////////////

					//Controle do número de registros referentes às probabilidades dos nós da árvore
					registroProbabilidade_controle++;

					if (NPROB == registroProbabilidade_controle || arvoreEquiprovavel) {

						registroProbabilidade = false;

						//////////////////////////////////////////////////////////////////////////
						//Set probabilidade_abertura no processo estocástico
						//////////////////////////////////////////////////////////////////////////

						SmartEnupla <Periodo, SmartEnupla <IdRealizacao, double>> matriz_probabilidade_abertura;
						matriz_probabilidade_abertura.addElemento(a_dados.getElementoVetor(AttVetorDados_horizonte_otimizacao, a_dados.getVetor(AttVetorDados_horizonte_otimizacao, IdEstagio(), Periodo()).getIteradorFinal(), Periodo()), probabilidade_abertura);

						a_dados.processoEstocastico_hidrologico.setMatriz_forced(AttMatrizProcessoEstocastico_probabilidade_realizacao, matriz_probabilidade_abertura);
						a_dados.processoEstocastico_hidrologico.validar_probabilidade_realizacao();

					}//if (NPROB == registroProbabilidade_controle || arvoreEquiprovavel) {

				}//if (registro >= 4 && registroProbabilidade == true) {
				else if (registro >= 4 && registroProbabilidade == false && conteio_registro_vazoes < numNos) {

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Registros (4 + NPROB) e subsequentes : vazões incrementais
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					std::vector<double> vazao_no_posto_auxiliar(numPostos, 0.0);

					for (int posto = 0; posto < numPostos; posto++)
						vazao_no_posto_auxiliar.at(posto) = intLeitura[posto];

					vazao_no_posto.push_back(vazao_no_posto_auxiliar);

					conteio_registro_vazoes++;

				}//else if (registro >= 4 && registroProbabilidade == false && conteio_registro_vazoes < numNos) {

			}//while (!(leituraArquivo.eof())) {

			leituraArquivo.clear();
			leituraArquivo.close();

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::ler_vazao_probabilidade_estrutura_arvore_cenarios_from_VAZOES_201906_DC29: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::ler_afluencia_passada_from_VAZOES_201906_DC29(Dados& a_dados, std::string nomeArquivo) {

	try {

		int mesInicial_PMO;
		int anoInicial_PMO;
		int diasNoSeguinteMes;

		Periodo periodo_semana_PMO; //Começa na primeira semana do PMO

		int revisao; //Dado para controlar a leitura das afluências históricas

		const int tamanho = 320;
		int intLeitura[tamanho];
		float floatLeitura[tamanho];

		int registro = 0;
		int numNos = 0;
		int NPROB; //Número de registros de probabilidade

		int numUHE;
		int numPostos;

		int  registroProbabilidade_controle = 0;
		bool registroProbabilidade = true;

		int conteio_registro_vazoes = 0;

		int conteio_registro_afluencias_passadas = 0;

		int dia_afluencia_historica = 1; //O dia acopla todos os periodos semanais para a leitura das afluências_incremental_tendencia mensais e semanais

		int conteio_realizacoes = 0;

		bool registroAfluenciasPassadas = false;

		///////////////////////////////////////////////////////////////////
		std::ifstream leituraArquivo;
		leituraArquivo.open(nomeArquivo, std::ios_base::in | std::ios::binary);

		if (leituraArquivo.is_open()) {

			while (!(leituraArquivo.eof())) {

				registro++;

				///////////////////////////////////////
				//Identifica o tipo do registro
				///////////////////////////////////////

				if (registro >= 4 && registroProbabilidade == true)
					leituraArquivo.read((char*)floatLeitura, sizeof(floatLeitura));
				else
					leituraArquivo.read((char*)intLeitura, sizeof(intLeitura));


				if (registro == 1) {

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Registro 1 -  total de postos considerados , número de estágios considerados  e número de aberturas em cada estágio
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					numUHE = intLeitura[0]; // Total de postos considerados
					const int numEstagios_DC = intLeitura[1]; // Número de estágios do problema

					//Número de aberturas por estágio do DECOMP

					//numero_realizacoes_DC(numEstagios_DC);//Número de realizações por estágio da árvore de cenários

					//Determina o número de nós do problema
					for (int pos = 2; pos < 2 + numEstagios_DC; pos++)
						numNos += intLeitura[pos]; //Número de nós por estágio

					//Define o número de registros para a probabilidade
					NPROB = int(numNos / 320);
					NPROB += 1;

				}//if (registro == 1) {
				else if (registro == 3) {

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Registro 3 - número de semanas completas, número de dias que devem ser excluídos do estágio seguinte 
					//             ao mês inicial decomposto em semanas, índice do mês inicial do estudo  e ano do mês inicial do estudo
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					const int numSemanasEstudo = intLeitura[0];
					diasNoSeguinteMes = intLeitura[1];
					mesInicial_PMO = intLeitura[2];
					anoInicial_PMO = intLeitura[3];
					numPostos = intLeitura[4];
					revisao = intLeitura[5];

				}//else if (registro == 3) {
				else if (registro >= 4 && registroProbabilidade == true) {

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Registros 4 e subsequentes : probabilidades associadas a cada nó
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					bool arvoreEquiprovavel = false;

					if (floatLeitura[0] == 0)
						arvoreEquiprovavel = true;

					/////////////////////////////////////

					//Controle do número de registros referentes às probabilidades dos nós da árvore
					registroProbabilidade_controle++;

					if (NPROB == registroProbabilidade_controle || arvoreEquiprovavel) {

						registroProbabilidade = false;

					}//if (NPROB == registroProbabilidade_controle || arvoreEquiprovavel) {

				}//if (registro >= 4 && registroProbabilidade == true) {
				else if (registroAfluenciasPassadas == true && conteio_registro_afluencias_passadas < 11 + revisao) {//11 + revisao : significam 11 valores de afluência mensal e um número de afluências semanais igual a revisao. A última linha é repetida nos arquivos e nesta leitura é desconsiderada

					//Notas:
					//O dia inicial dos periodos semanais criados somente é importante para o acoplamento entre periodos. No nosso caso, o primeiro Periodo semanal começa no dia 1 do primeiro mês da tendência
					//O valor mensal ou semanal médio vai ser obtido por meio de um getElementos(IdMes), por isso, o dia não é relevante para determinar o periodo semanal

					const IdHidreletrica idHidreletrica_MaiorId = a_dados.getMaiorId(IdHidreletrica());

					if (conteio_registro_afluencias_passadas < 11) {//Registros de afluências passadas MENSAIS

						///////////////////////////////////
						//Inicializa valor_afluencia_passadas
						///////////////////////////////////

						if (conteio_registro_afluencias_passadas == 0) {

							for (int posto = 0; posto < numPostos; posto++)
								valor_afluencia_passadas_DECOMP.addElemento(posto, posto);

						}//if (conteio_registro_afluencias_passadas == 0) {

						///////////////////////////////////
						//Identifica mes e ano
						///////////////////////////////////

						int mes_afluencia_historica = mesInicial_PMO;
						int ano_afluencia_historica = anoInicial_PMO;

						const int conteio_meses_anteriores = 11 - conteio_registro_afluencias_passadas;

						for (int conteio = 0; conteio < conteio_meses_anteriores; conteio++) {

							mes_afluencia_historica--;

							if (mes_afluencia_historica == 0) {

								mes_afluencia_historica = 12;
								ano_afluencia_historica--;

							}//if (mes_afluencia_historica < 0) {

						}//for (int conteio = 0; conteio < conteio_meses_anteriores; conteio++) {

						/////////////////////////////////////////////////
						//Guarda a informação mensal em periodos semanais
						/////////////////////////////////////////////////

						if (conteio_meses_anteriores == 1) {

							//No mês anterior ao mês inicial do PMO deve encontrar o Tipo_Periodo dependendo dos dias
							//da primeira semana (RV0) do PMO dentro do mês anterior

							////////////////////////////////////////////////////////////////////////
							//1. Determina o dia da primeira semana operativa do PMO no mês anterior 
							////////////////////////////////////////////////////////////////////////

							IdDia idDia = data_execucao.getDia();
							IdMes idMes = data_execucao.getMes();
							IdAno idAno = data_execucao.getAno();

							IdDia maiordiadomes = data_execucao.getMaiorDiaDoMes(idMes);

							//O periodo começa no dia seguinte da data de execução
							idDia++;

							if (idDia > maiordiadomes) {
								idDia = IdDia_1;
								idMes++;
								if (idMes == IdMes_Excedente) {
									idMes = IdMes_1;
									idAno++;
								}//if (idMes == IdMes_Excedente) {

							}//if(idDia == maiordiadomes) {

							//Cria periodo semanal logo do dia_execução

							const Periodo periodo_diario(idDia, idMes, idAno);

							Periodo periodo_semanal(TipoPeriodo_semanal, periodo_diario);

							const Periodo periodo_diario_limite(1, mesInicial_PMO, anoInicial_PMO); //Corresponde ao primeiro dia do mês do PMO

							while (periodo_semanal > periodo_diario_limite)
								periodo_semanal--;

							periodo_semana_PMO = periodo_semanal;

							///////////////////////////////////////////////////////////////////////////////////////
							//2. Realiza conteio do número de dias do mês anterior até o periodo_semana_PMO do passo 1.
							///////////////////////////////////////////////////////////////////////////////////////

							Periodo periodo_diario_mes_anterior(1, mes_afluencia_historica, ano_afluencia_historica);

							int conteioDias = 0;

							while (periodo_diario_mes_anterior < periodo_semana_PMO) {

								conteioDias++;
								periodo_diario_mes_anterior++;

							}//while (periodo_diario_mes_anterior < periodo_semana_PMO) {

							std::string string_periodo = getString(1) + "/" + getString(mes_afluencia_historica) + "/" + getString(ano_afluencia_historica) + "-" + getString(conteioDias) + "dias";;

							const Periodo periodo_mensal(string_periodo);

							for (int posto = 0; posto < numPostos; posto++) {

								const int afluencia = intLeitura[posto];

								//Adiciona o valor de afluencia na matriz da classe Afluencia
								valor_afluencia_passadas_DECOMP.at(posto).addElemento(periodo_mensal, afluencia);

							}//for (int posto = 0; posto < numPostos; posto++) {

						}//if (conteio_meses_anteriores == 1) {
						else {

							const Periodo periodo_mensal(mes_afluencia_historica, ano_afluencia_historica);

							for (int posto = 0; posto < numPostos; posto++) {

								const int afluencia = intLeitura[posto];

								//Adiciona o valor de afluencia na matriz da classe Afluencia
								valor_afluencia_passadas_DECOMP.at(posto).addElemento(periodo_mensal, afluencia);

							}//for (int posto = 0; posto < numPostos; posto++) {

						}//else {

					}//if (conteio_registro_afluencias_passadas < 11) {
					else {//Registros de afluências passadas SEMANAIS

						//periodo_semana_PMO: A primeira semana PMO é calculada no último mês registrado no registroAfluenciasPassadas

						/////////////////////////////////////////////////
						//Guarda a informação semanal
						/////////////////////////////////////////////////

						for (int posto = 0; posto < numPostos; posto++) {

							const int afluencia = intLeitura[posto];

							//Adiciona o valor de afluencia na matriz da classe Afluencia
							valor_afluencia_passadas_DECOMP.at(posto).addElemento(periodo_semana_PMO, afluencia);

						}//for (int posto = 0; posto < numPostos; posto++) {

						/////////////////////////////////////////////////
						//Aumenta o dia para o próximo periodo semanal
						/////////////////////////////////////////////////

						periodo_semana_PMO++;

					}//else {

					conteio_registro_afluencias_passadas++;

				}//else if (registroAfluenciasPassadas == true && conteio_registro_afluencias_passadas < 11 + revisao) {
				else if (conteio_registro_vazoes >= numNos && registroAfluenciasPassadas == false) {// Descarta a informação das vazões para o acoplamento com o Newave

					conteio_registro_vazoes++;

					if (conteio_registro_vazoes == 2 * numNos)
						registroAfluenciasPassadas = true;

				}//else if (conteio_registro_vazoes >= numNos) {
				else if (registro >= 4 && registroProbabilidade == false && conteio_registro_vazoes < numNos) {

					conteio_registro_vazoes++;

				}//else if (registro >= 4 && registroProbabilidade == false) {


			}//while (!(leituraArquivo.eof())) {

			leituraArquivo.clear();
			leituraArquivo.close();


		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::ler_afluencia_passada_from_VAZOES_201906_DC29: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::ler_historico_afluencia_from_VAZOES_201906_DC29(Dados& a_dados, std::string nomeArquivo){

	try {

		int intLeitura_320[320];
		int intLeitura_600[600];

		///////////////////////////////////////////////////////////////////
		std::ifstream leituraArquivo;
		leituraArquivo.open(nomeArquivo, std::ios_base::in | std::ios::binary);

		if (leituraArquivo.is_open()) {

			const int tamanho_registro_arquivo_vazoes_historicas = 320;

			const int numero_postos = valor_afluencia_passadas_GEVAZP.size();

			valor_afluencia_historica = SmartEnupla<int, SmartEnupla<Periodo, double>>(0, std::vector<SmartEnupla<Periodo, double>>(numero_postos, SmartEnupla<Periodo, double>()));

			int mes = 1;
			int ano = 1931; //Primeiro ano do histórico

			const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

			const IdHidreletrica maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

			while (!(leituraArquivo.eof())) {

				Periodo periodo(mes, ano);

				if (periodo > horizonte_estudo.getIteradorInicial())
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

				for (int posto = 0; posto < numero_postos; posto++){

					double afluencia_historico = 0.0;

					if (tamanho_registro_arquivo_vazoes_historicas == 320)
						afluencia_historico = double(intLeitura_320[posto]);
					else if (tamanho_registro_arquivo_vazoes_historicas == 600)
						afluencia_historico = double(intLeitura_600[posto]);

					valor_afluencia_historica.at(posto).addElemento(periodo, afluencia_historico);

				}// for (int posto = 0; posto < numero_postos; posto++){

			}//while (!(leituraArquivo.eof())) {

			//fp_out.close();

			leituraArquivo.clear();
			leituraArquivo.close();

		}//if (leituraArquivo.is_open()) {

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::ler_historico_afluencia_from_VAZOES_201906_DC29: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::definir_horizonte_otimizacao_DC_from_VAZOES_201906_DC29(Dados& a_dados, std::string nomeArquivo) {

	try {

		const int tamanho = 320;
		int intLeitura[tamanho];

		int registro = 0;

		///////////////////////////////////////////////////////////////////
		std::ifstream leituraArquivo;
		leituraArquivo.open(nomeArquivo, std::ios_base::in | std::ios::binary);

		if (leituraArquivo.is_open()) {

			while (true) {

				registro++;

				leituraArquivo.read((char*)intLeitura, sizeof(intLeitura));

				if (registro == 3) {

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Registro 3 - número de semanas completas, número de dias que devem ser excluídos do estágio seguinte 
					//             ao mês inicial decomposto em semanas, índice do mês inicial do estudo  e ano do mês inicial do estudo
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					const int numSemanasEstudo = intLeitura[0];
					const int diasNoSeguinteMes = intLeitura[1];
					const int mesInicial_PMO = intLeitura[2];
					const int anoInicial_PMO = intLeitura[3];

					int mesAnterior;
					int anoAnterior;

					mesAnterior = mesInicial_PMO - 1;

					if (mesAnterior < 1) {
						mesAnterior = 12;
						anoAnterior = anoInicial_PMO - 1;
					}//if (mesAnterior > 1) {
					else
						anoAnterior = anoInicial_PMO;

					Periodo periodo(mesInicial_PMO, anoInicial_PMO);
					Periodo periodoAnterior(mesAnterior, anoAnterior);

					IdMes idMes = getIdMesFromInt(mesInicial_PMO);
					IdAno idAno = getIdAnoFromChar(std::to_string(anoInicial_PMO).c_str());
					IdDia idDia = periodo.getMaiorDiaDoMes(idMes);

					IdMes idMesAnterior = getIdMesFromInt(mesAnterior);

					int diasDesconto;

					for (int semana = 0; semana < numSemanasEstudo; semana++) {

						if (semana == 0)
							diasDesconto = 7 - diasNoSeguinteMes;
						else
							diasDesconto = 7;

						for (int dia = 0; dia < diasDesconto; dia++) {

							if (idDia == IdDia_1) {

								idDia = periodo.getMaiorDiaDoMes(idMesAnterior);

								if (idMes == IdMes_1) {

									idMes = IdMes_12;
									idAno--;
								}//if (idMes == IdMes_1) {
								else
									idMes--;

							}//if (idDia == IdDia_1) {
							else
								idDia--;

						}//for (int dia = 0; dia < diasDesconto; dia++) {

					}//for (int semana = 0; semana < numSemanasEstudo; semana++) {

					//Set Data execução
					Periodo data_execucao_auxiliar(idDia, idMes, idAno);

					data_execucao = data_execucao_auxiliar;

					IdDiaSemana diaSemana = data_execucao.getDiaSemana();

					if (diaSemana != IdDiaSemana_SEX)
						throw std::invalid_argument("Data de execucao diferente a uma sexta-feira");

					//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Determina o vetor horizonte_otimizacao_DC (somente serve para a mapear na leitura de dados a data para o horizonte_estudo do SPT
					//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					std::string string_periodo;
					IdDia maiordiadomes;
					maiordiadomes = data_execucao.getMaiorDiaDoMes(idMes);

					IdEstagio idEstagio = IdEstagio_1;

					//O periodo começa no dia seguinte da data de execução
					idDia++;

					if (idDia > maiordiadomes) {
						idDia = IdDia_1;
						idMes++;
						if (idMes == IdMes_Excedente) {
							idMes = IdMes_1;
							idAno++;
						}//if (idMes == IdMes_Excedente) {

						maiordiadomes = data_execucao.getMaiorDiaDoMes(idMes);
					}//if(idDia == maiordiadomes) {

					//Estabelece todos os periodos do horizonte de planejamento

					//Periodos semanais do DECOMP

					for (int semana = 0; semana < numSemanasEstudo; semana++) {

						string_periodo = getString(idDia) + "/" + getString(idMes) + "/" + getString(idAno) + "-semanal";

						for (int dia = 0; dia < 7; dia++) {
							idDia++;

							if (idDia > maiordiadomes) {
								idDia = IdDia_1;
								idMes++;
								if (idMes == IdMes_Excedente) {
									idMes = IdMes_1;
									idAno++;
								}//if (idMes == IdMes_Excedente) {

								maiordiadomes = Periodo(idMes, idAno).getMaiorDiaDoMes();
							}//if(idDia == maiordiadomes) {
						}//for (int dia = 0; dia < 7; dia++) {

						Periodo periodoSemanal(string_periodo);
						horizonte_otimizacao_DC.addElemento(periodoSemanal);

						idEstagio++;

					}//for (int semana = 0; semana < numSemanasEstudo; semana++) {

					//Periodo "mensal" do DECOMP

					string_periodo = getString(idDia) + "/" + getString(idMes) + "/" + getString(idAno);

					int conteioDias = 1; //O primeiro dia conta na formação do periodo
					while (true) {

						if (idDia == maiordiadomes)
							break;

						idDia++;
						conteioDias++;

					}//while (true) {

					if (conteioDias == int(maiordiadomes))
						string_periodo += "-mensal";
					else if (conteioDias < int(maiordiadomes))
						string_periodo += "-" + getString(conteioDias) + "dias";

					const Periodo periodoMensal(string_periodo);

					horizonte_otimizacao_DC.addElemento(periodoMensal);

					break;

				}//else if (registro == 3) {

			}//while (true) {

			leituraArquivo.clear();
			leituraArquivo.close();

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::definir_horizonte_otimizacao_DC_from_VAZOES_201906_DC29: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::instanciar_hidreletricas_from_VAZOES_201906_DC29(Dados& a_dados, std::string nomeArquivo) {

	try {

		//********************************************************************************************************************************

		const int tamanho = 320;
		int intLeitura[tamanho];

		int registro = 0;

		int numUHE;

		///////////////////////////////////////////////////////////////////
		std::ifstream leituraArquivo;
		leituraArquivo.open(nomeArquivo, std::ios_base::in | std::ios::binary);

		if (leituraArquivo.is_open()) {

			while (true) {

				registro++;

				///////////////////////////////////////
				//Identifica o tipo do registro
				///////////////////////////////////////

				leituraArquivo.read((char*)intLeitura, sizeof(intLeitura));

				if (registro == 1) {

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Registro 1 -  total de postos considerados , número de estágios considerados  e número de aberturas em cada estágio
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					numUHE = intLeitura[0]; // Total de postos considerados

				}//if (registro == 1) {
				else if (registro == 2) {

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Registro 2 -  código das usinas hidráulicas associadas aos postos de vazões considerados
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					//Inicializa as hidrelétricas desde o decomp

					for (int h = 0; h < numUHE; h++) {

						const int codigo_usina = intLeitura[h];

						//Identifica se alguma hidrelétrica tem sido inicializada com codigo_usina
						const IdHidreletrica idHidreletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

						if (idHidreletrica_inicializado == IdHidreletrica_Nenhum) {

							Hidreletrica   hidreletrica;
							IdHidreletrica idHidreletrica;

							idHidreletrica = a_dados.getMaiorId(IdHidreletrica());

							if (idHidreletrica == IdHidreletrica_Nenhum)
								idHidreletrica = IdHidreletrica_1;
							else
								idHidreletrica++;

							//Set idHidreletrica
							hidreletrica.setAtributo(AttComumHidreletrica_idHidreletrica, idHidreletrica);
							hidreletrica.setAtributo(AttComumHidreletrica_codigo_usina, codigo_usina);
							hidreletrica.setAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica_por_usina);
							//Set codigo_usina
							lista_codigo_ONS_hidreletrica.addElemento(idHidreletrica, codigo_usina);

							a_dados.vetorHidreletrica.add(hidreletrica);

							lista_hidreletrica_sem_capacidade.addElemento(idHidreletrica, false);
							lista_hidreletrica_maiorIdConjuntoHidraulico.addElemento(idHidreletrica, IdConjuntoHidraulico_Nenhum); //Logo lendo o HIDR.DAT este parâmetro é atualizado

							lista_IdSubmercado_hidreletrica.addElemento(idHidreletrica, IdSubmercado_Nenhum); //Logo no HIDR.DAT esta informação é atualizada

						}//if (idHidreletrica_inicializado == IdHidreletrica_Nenhum) {

					}//for (int h = 0; h < numUHE; h++) {

					break;

				}//else if (registro == 2) {

			}//while (true) {

			leituraArquivo.clear();
			leituraArquivo.close();

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::instanciar_hidreletricas_from_VAZOES_201906_DC29: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::instanciar_processo_estocastico_CP(Dados& a_dados) {

	try {

		a_dados.processoEstocastico_hidrologico = ProcessoEstocastico();
		a_dados.processoEstocastico_hidrologico.setAtributo(AttComumProcessoEstocastico_idProcessoEstocastico, IdProcessoEstocastico_hidrologico_hidreletrica);
		a_dados.processoEstocastico_hidrologico.setAtributo(AttComumProcessoEstocastico_tipo_espaco_amostral, TipoEspacoAmostral_arvore_cenarios);
		a_dados.processoEstocastico_hidrologico.setAtributo(AttComumProcessoEstocastico_tipo_correlacao_variaveis_aleatorias, TipoCorrelacaoVariaveisAleatorias_sem_correlacao);
		a_dados.setAtributo(AttComumDados_tipo_espaco_amostral_geracao_cenario_hidrologico, TipoEspacoAmostral_arvore_cenarios);

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::instanciar_processo_estocastico_CP: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::instanciar_mapeamento_arvore_cenarios_simetrica(Dados& a_dados)
{

	try {

		//Filosofia: Somente serve se a árvore de cenários for simêtrica

		if (int(numero_realizacoes_por_periodo.size()) == 0)
			throw std::invalid_argument("Informar na pre-configuracao o numero_realizacoes_por_periodo_processo_estocastico");

		const int numero_cenarios = a_dados.getAtributo(AttComumDados_numero_cenarios, int());

		const IdCenario maiorIdCenario = IdCenario(numero_cenarios);

		SmartEnupla<IdCenario, SmartEnupla<Periodo, IdNo>> mapeamento_arvore_cenario_simetrica;

		////////////////////////////////////////////////////////////////
		//Cria vetor com o número de "sub-cenários" em cada nó da árvore
		////////////////////////////////////////////////////////////////

		SmartEnupla<Periodo, int> numero_sub_cenarios;

		int sub_cenarios = numero_cenarios;

		for (Periodo periodo = horizonte_processo_estocastico.getIteradorInicial(); periodo <= horizonte_processo_estocastico.getIteradorFinal(); horizonte_processo_estocastico.incrementarIterador(periodo)) {
			sub_cenarios /= numero_realizacoes_por_periodo.getElemento(periodo);
			numero_sub_cenarios.addElemento(periodo, sub_cenarios);

		}//for (Periodo periodo = horizonte_processo_estocastico.getIteradorInicial(); periodo <= horizonte_processo_estocastico.getIteradorFinal(); horizonte_processo_estocastico.incrementarIterador(periodo)) {

		////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////
		//Realiza mapeamento espaço amostral
		/////////////////////////////////////////////////

		SmartEnupla<Periodo, int> conteio_sub_cenarios(numero_sub_cenarios, 0);
		SmartEnupla<Periodo, int> no(numero_sub_cenarios, 1);

		for (IdCenario idCenario = IdCenario_1; idCenario <= maiorIdCenario; idCenario++) {

			SmartEnupla<Periodo, IdNo> mapeamento_arvore_cenario_simetrica_cenario;

			//for (IdEstagio idEstagio = estagio_acoplamento_pre_estudo; idEstagio <= idEstagio_final; idEstagio++) {
			for (Periodo periodo = horizonte_processo_estocastico.getIteradorInicial(); periodo <= horizonte_processo_estocastico.getIteradorFinal(); horizonte_processo_estocastico.incrementarIterador(periodo)) {

				const int sub_cenarios = numero_sub_cenarios.getElemento(periodo);

				if (conteio_sub_cenarios.getElemento(periodo) >= sub_cenarios) {

					const int aumento_no = no.getElemento(periodo) + 1;
					no.setElemento(periodo, aumento_no);

					conteio_sub_cenarios.setElemento(periodo, 0);

				}//if (conteio_sub_cenarios.getElemento(periodo) > sub_cenarios) {

				const IdNo idNo = IdNo(no.getElemento(periodo) + 1);

				mapeamento_arvore_cenario_simetrica_cenario.addElemento(periodo, idNo);

				///////////

				const int aumento_sub_cenarios = conteio_sub_cenarios.getElemento(periodo) + 1;
				conteio_sub_cenarios.setElemento(periodo, aumento_sub_cenarios);

			}//for (Periodo periodo = horizonte_processo_estocastico.getIteradorInicial(); periodo <= horizonte_processo_estocastico.getIteradorFinal(); horizonte_processo_estocastico.incrementarIterador(periodo)) {

			/////////////////////////////////////////////////////////////////

			mapeamento_arvore_cenario_simetrica.addElemento(idCenario, mapeamento_arvore_cenario_simetrica_cenario);

		}//for (IdCenario idCenario = a_cenario_inicial; idCenario <= a_cenario_final; idCenario++) {

		a_dados.processoEstocastico_hidrologico.setMatriz(AttMatrizProcessoEstocastico_mapeamento_arvore_cenarios, mapeamento_arvore_cenario_simetrica);

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::instanciar_mapeamento_arvore_cenarios_simetrica: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::instanciar_no_antecessor_arvore_simetrica(Dados& a_dados)
{

	try {

		if (int(numero_realizacoes_por_periodo.size()) == 0)
			throw std::invalid_argument("Informar na pre-configuracao o numero_realizacoes_por_periodo_processo_estocastico");

		/////////////////////////////////////////////
		//Inicializa no_antecessor
		/////////////////////////////////////////////

		int numero_nos_por_periodo = 1;

		for (Periodo periodo = horizonte_processo_estocastico.getIteradorInicial(); periodo <= horizonte_processo_estocastico.getIteradorFinal(); horizonte_processo_estocastico.incrementarIterador(periodo)) {

			numero_nos_por_periodo *= numero_realizacoes_por_periodo.getElemento(periodo);
			const IdNo maiorIdNo_periodo = IdNo(numero_nos_por_periodo + 1); //+1 porque existe o IdNo_0

			for (IdNo idNo = IdNo_1; idNo <= maiorIdNo_periodo; idNo++)
				a_dados.processoEstocastico_hidrologico.addElemento(AttMatrizProcessoEstocastico_no_antecessor, periodo, idNo, IdNo_0);

		}//for (Periodo periodo = horizonte_processo_estocastico.getIteradorInicial(); periodo <= horizonte_processo_estocastico.getIteradorFinal(); horizonte_processo_estocastico.incrementarIterador(periodo)) {

		/////////////////////////////////////////////
		//Atualiza no_antecessor
		/////////////////////////////////////////////

		const IdCenario maiorIdCenario = a_dados.processoEstocastico_hidrologico.getIterador1Final(AttMatrizProcessoEstocastico_mapeamento_arvore_cenarios, IdCenario());

		for (IdCenario idCenario = IdCenario_1; idCenario <= maiorIdCenario; idCenario++) {

			for (Periodo periodo = horizonte_processo_estocastico.getIteradorInicial(); periodo <= horizonte_processo_estocastico.getIteradorFinal(); horizonte_processo_estocastico.incrementarIterador(periodo)) {

				if (periodo > horizonte_processo_estocastico.getIteradorInicial()) {

					Periodo periodo_anterior = periodo;
					horizonte_processo_estocastico.decrementarIterador(periodo_anterior);

					const IdNo idNo = a_dados.processoEstocastico_hidrologico.getElementoMatriz(AttMatrizProcessoEstocastico_mapeamento_arvore_cenarios, idCenario, periodo, IdNo());
					const IdNo idNo_antecessor = a_dados.processoEstocastico_hidrologico.getElementoMatriz(AttMatrizProcessoEstocastico_mapeamento_arvore_cenarios, idCenario, periodo_anterior, IdNo());

					a_dados.processoEstocastico_hidrologico.setElemento(AttMatrizProcessoEstocastico_no_antecessor, periodo, idNo, idNo_antecessor);

				}//if (periodo > horizonte_processo_estocastico.getIteradorInicial()) {

			}//for (Periodo periodo = horizonte_processo_estocastico.getIteradorInicial(); periodo <= horizonte_processo_estocastico.getIteradorFinal(); horizonte_processo_estocastico.incrementarIterador(periodo)) {

		}//for (IdCenario idCenario = IdCenario_1; idCenario <= maiorIdCenario; idCenario++) {

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::instanciar_no_antecessor_arvore_simetrica: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::instanciar_no_realizacao_arvore_simetrica(Dados& a_dados)
{

	try {

		if (int(numero_realizacoes_por_periodo.size()) == 0)
			throw std::invalid_argument("Informar na pre-configuracao o numero_realizacoes_por_periodo_processo_estocastico");

		//Este vetor pode ser alterado para gerar para um determinado periodo com amostras comuns
		SmartEnupla<Periodo, bool> periodo_amostra_comum(horizonte_processo_estocastico, false);

		/////////////////////////////////////////////
		//Atualiza no_antecessor
		/////////////////////////////////////////////

		IdRealizacao idRealizacao = IdRealizacao_1;

		for (Periodo periodo = horizonte_processo_estocastico.getIteradorInicial(); periodo <= horizonte_processo_estocastico.getIteradorFinal(); horizonte_processo_estocastico.incrementarIterador(periodo)) {

			const SmartEnupla<IdNo, IdNo> no_antecessor_periodo = a_dados.processoEstocastico_hidrologico.getElementosMatriz(AttMatrizProcessoEstocastico_no_antecessor, periodo, IdNo(), IdNo());

			const IdNo maiorIdNo = no_antecessor_periodo.getIteradorFinal();

			int conteio_aberturas = 0;

			const IdRealizacao idRealizacao_referencia = idRealizacao;

			for (IdNo idNo = IdNo_1; idNo <= maiorIdNo; idNo++) {

				if (periodo_amostra_comum.getElemento(periodo) && conteio_aberturas == numero_realizacoes_por_periodo.getElemento(periodo)) {
					idRealizacao = idRealizacao_referencia;
					conteio_aberturas = 0;
				}//if (periodo_amostra_comum.getElemento(periodo) && conteio_aberturas == numero_realizacoes_por_periodo.getElemento(periodo)) {

				conteio_aberturas++;

				a_dados.processoEstocastico_hidrologico.addElemento(AttMatrizProcessoEstocastico_no_realizacao, periodo, idNo, idRealizacao);

				idRealizacao++;

			}//for (IdNo idNo = IdNo_1; idNo <= maiorIdNo; idNo++) {

		}//for (Periodo periodo = horizonte_processo_estocastico.getIteradorInicial(); periodo <= horizonte_processo_estocastico.getIteradorFinal(); horizonte_processo_estocastico.incrementarIterador(periodo)) {

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::instanciar_no_realizacao_arvore_simetrica: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::instanciar_no_probabilidade_arvore_simetrica(Dados& a_dados)
{

	try {

		if (int(numero_realizacoes_por_periodo.size()) == 0)
			throw std::invalid_argument("Informar na pre-configuracao o numero_realizacoes_por_periodo_processo_estocastico");

		int numero_nos = 1;

		for (Periodo periodo = horizonte_processo_estocastico.getIteradorInicial(); periodo <= horizonte_processo_estocastico.getIteradorFinal(); horizonte_processo_estocastico.incrementarIterador(periodo)) {

			numero_nos *= numero_realizacoes_por_periodo.getElemento(periodo);

			const double prob = 1 / numero_realizacoes_por_periodo.getElemento(periodo);

			for (int no = 0; no < numero_nos; no++)
				a_dados.processoEstocastico_hidrologico.addElemento(AttMatrizProcessoEstocastico_no_probabilidade, periodo, IdNo(no + 2), prob);

		}//for (Periodo periodo = horizonte_processo_estocastico.getIteradorInicial(); periodo <= horizonte_processo_estocastico.getIteradorFinal(); horizonte_processo_estocastico.incrementarIterador(periodo)) {

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::instanciar_no_probabilidade_arvore_simetrica: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_DADGER_201906_DC29(Dados& a_dados, std::string nomeArquivo)
{

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;

		//Horizonte de estudo
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		//Parâmetros para registros BE (Bacias especiais) e PQ (pequenas usinas)
		bool inicializar_conteio_numero_usina_nao_simulado_por_submercado = true;

		SmartEnupla<IdSubmercado, SmartEnupla <Periodo, int>> conteio_numero_usina_nao_simulado_por_submercado;

		SmartEnupla<IdHidreletrica, SmartEnupla <Periodo, double>> desvio_registro_DA(IdHidreletrica_1, std::vector<SmartEnupla<Periodo, double>>(a_dados.getMaiorId(IdHidreletrica()), SmartEnupla<Periodo, double>(horizonte_estudo, 0.0)));

		//Se todos os registros SB (submercados) e DP (carga subsistemas) são lidos, é ativado um método para a inicialização dos submercados IVAPORÃ e ANDE
		bool leitura_registro_SB = false;
		bool leitura_registro_DP = false;
		bool leitura_registro_CD = false;

		if (leituraArquivo.is_open()) {

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);

				std::string teste_comentario = line.substr(0, 1);

				if (teste_comentario != "&") {

					std::string menemonico = line.substr(0, 2);

					if (menemonico == "TE") {

						try {
							//Título do estudo
							atributo = line.substr(4, 80);
							if (!dadosPreConfig_instanciados)
								a_dados.setAtributo(AttComumDados_nome_estudo, atributo);
						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro TE: \n" + std::string(erro.what())); }

					}//if(menemonico == "TE"){
					if (menemonico == "PT") {

						try {

							//Patamares de carga

							atributo = line.substr(5, 3);
							const IdEstagio idEstagio = getIdEstagioFromChar(atributo.c_str());

							atributo = line.substr(11, 2);
							const int valor = std::atoi(atributo.c_str());

							/*
							if (idEstagio == IdEstagio_1) {

								const SmartEnupla<Periodo, int> numero_patamares_por_periodo(horizonte_estudo, valor);

								a_dados.setVetor(AttVetorDados_numero_patamares_por_periodo, numero_patamares_por_periodo);

							}//if (idEstagio_inicial == IdEstagio_1) {
							else {

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									if (periodo >= horizonte_otimizacao_DC.at(idEstagio))
										a_dados.setElemento(AttVetorDados_numero_patamares_por_periodo, periodo, valor);

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							}//else {
							*/

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro PT: \n" + std::string(erro.what())); }

					}//if(menemonico == "PT"){

					else if (menemonico == "FP") {
						const IdHidreletrica maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());
						if (std::stoi(line.substr(4, 3)) == 999) {
							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {
									if (a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.isInstanciado(IdFuncaoProducaoHidreletrica_1) == false) {
										FuncaoProducaoHidreletrica Fph;
										Fph.setAtributo(AttComumFuncaoProducaoHidreletrica_idFuncaoProducaoHidreletrica, IdFuncaoProducaoHidreletrica_1);
										a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.add(Fph);
									}
									a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).setVetor(AttVetorFuncaoProducaoHidreletrica_percentual_volume_minimo, SmartEnupla<Periodo, double>(horizonte_estudo, double(std::stod(line.substr(41, 5)))));
									a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).setVetor(AttVetorFuncaoProducaoHidreletrica_percentual_volume_maximo, SmartEnupla<Periodo, double>(horizonte_estudo, double(std::stod(line.substr(47, 5)))));
								}
							}
						}
					}//else if (menemonico == "FP") {

					if (menemonico == "SB") {

						try {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Índice do subsistema  
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_usina = std::atoi(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Mnemônico de identificação para o subsistema.   
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const string mnemonico = atributo;

							//*******************************************************************************************************************
							//Set infromação na classe Submercado
							//*******************************************************************************************************************

							//Testa se algum subsistema foi inicializado com o id_Cepel

							const IdSubmercado idSubmercado_inicializado = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_usina);

							if (idSubmercado_inicializado == IdSubmercado_Nenhum) {

								const IdSubmercado idSubmercado = IdSubmercado(a_dados.getMaiorId(IdSubmercado()) + 1);

								Submercado submercado;

								submercado.setAtributo(AttComumSubmercado_idSubmercado, idSubmercado);
								submercado.setAtributo(AttComumSubmercado_nome, mnemonico);

								lista_codigo_ONS_submercado.addElemento(idSubmercado, codigo_usina);

								lista_submercado_mnemonico.addElemento(idSubmercado, mnemonico);

								//Adiciona um novo submercado no vetorSubmercado
								a_dados.vetorSubmercado.add(submercado);

							} // if (idSubmercado_inicializado == IdSubmercado_Nenhum) {

							else if (idSubmercado_inicializado != IdSubmercado_Nenhum)
								throw std::invalid_argument("Submercado ja instanciado, mnemonico: " + mnemonico);

							leitura_registro_SB = true;

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro SB: \n" + std::string(erro.what())); }

					}//if (menemonico == "SB") {

					if (menemonico == "UH") {

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Número da usina hidráulica  conforme registro 303 do arquivo de vazões 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//codigo_usina
							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_usina = atoi(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Índice do reservatório equivalente de energia (REE) ao qual pertence a usina
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//codigo_REE_CEPEL
							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							int codigo_REE_CEPEL = 0; //default

							if (atributo != "")
								codigo_REE_CEPEL = atoi(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 4 -  Volume armazenado inicial em percentagem do  volume útil (default = 0.0 %). 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(14, 10);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							double percentual_volume_inicial = 0.0; //default

							if (atributo != "")
								percentual_volume_inicial = atof(atributo.c_str()) / 100;

							//******************************************************************************************************************
							//Nota: Até o campo 4 é obrigatório no DECK ter um valor assignado, portanto, a partir deste ponto é necessário 
							//      verificar o tamanho do número de caracteres da string line
							//******************************************************************************************************************

							const int line_size = int(line.length());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 5 -  Vazão defluente mínima , em m3/s. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							double vazao_defluente_minima;

							if (line_size >= 34) {

								//vazao_defluente_minima
								atributo = line.substr(24, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								vazao_defluente_minima = 0.0; //default

								if (atributo != "")
									vazao_defluente_minima = atof(atributo.c_str());


							}//if (line_size >= 34) {
							else {
								//Set valor default
								vazao_defluente_minima = 0.0; //default
							}//else {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 6 -  Chave para considerar  evaporação: = 0 - não considera (default).  = 1 – considera. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							bool considera_evaporacao;

							if (line_size >= 40) {
								atributo = line.substr(39, 1);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								considera_evaporacao = false; //default

								if (atributo != "") {

									if (atoi(atributo.c_str()) == 1)
										considera_evaporacao = true;

								}//if (atributo != "") {

							}//if (line_size >= 40) {
							else {
								//Set valor default
								considera_evaporacao = false; //default
							}//else {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 7 - Identificação do estágio (inclusive) a partir do qual a usina passa a produzir energia elétrica (default = 1)
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							IdEstagio idEstagio_entrada_operacao;

							if (line_size >= 46) {
								atributo = line.substr(44, 2);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								idEstagio_entrada_operacao = IdEstagio_1; //default

								if (atributo != "")
									idEstagio_entrada_operacao = getIdEstagioFromChar(atributo.c_str());

							}//if (line_size >= 40) {
							else {
								//Set valor default
								idEstagio_entrada_operacao = IdEstagio_1; //default
							}//else {

							//////////////////////////////////////
							//Determina o periodo_entrada_operacao
							//////////////////////////////////////

							Periodo periodo_entrada_operacao;

							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								if (periodo >= horizonte_otimizacao_DC.at(idEstagio_entrada_operacao)) {

									periodo_entrada_operacao = periodo;
									break;

								}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_entrada_operacao)) {

							}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 8 - Volume morto inicial em hm3, (default = 0.0). Caso este valor seja fornecido o campo 4 será desconsiderado
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							double volume_morto;

							if (line_size >= 59) {
								atributo = line.substr(49, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								volume_morto = 0.0; //default

								if (atributo != "") {

									volume_morto = atof(atributo.c_str());

									//Caso este valor seja fornecido o campo 4 (Volume armazenado inicial) será desconsiderado
									percentual_volume_inicial = 0.0; //default

								}//if (atributo != "") {

							}//if (line_size >= 59) {
							else {
								//Set valor default
								volume_morto = 0.0; //default
							}//else {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 9 - Limite superior para vertimento, em  m3/s (default = 1.0e+21). 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							double vertimento_maximo;

							if (line_size >= 69) {
								atributo = line.substr(59, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								vertimento_maximo = 1e21; //default

								if (atributo != "")
									vertimento_maximo = atof(atributo.c_str());

							}//if (line_size >= 69) {
							else {
								//Set valor default
								vertimento_maximo = 1e21; //default
							}//else {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 10 - Chave para comsiderar o balanço hídrico em cada patamar, para usinas a fio d’água  
							//            = 0 - não considera (default).  = 1 – considera. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							//A informação do campo 10 não é relevante já que o SPT tem sua própia lógica para considerar o balanço hídrico para as usinas fio d'água


							//*******************************************************************************************************************
							//Set infromação na classe Hidreletrica
							//*******************************************************************************************************************

							//Neste ponto as hidrelétricas já estão inicializadas pela preconfiguração ou na leitura do VAZOES.DAT

							//Identifica se alguma hidrelétrica tem sido inicializada com codigo_ONS
							const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

							if (idHidreletrica == IdHidreletrica_Nenhum)
								throw std::invalid_argument("Resgistro UH - hidreletrica nao instanciada com codigo_usina_" + getString(codigo_usina));

							//const IdBaciaHidrografica idBaciaHidrografica = a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_bacia, IdBaciaHidrografica());

							if (maior_ONS_REE < codigo_REE_CEPEL)
								maior_ONS_REE = codigo_REE_CEPEL;

							///////////////////////
							a_dados.volume_inicial_carregado_from_premissa = true;
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_percentual_volume_util_inicial, percentual_volume_inicial);
							a_dados.vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_vazao_defluente_minima, SmartEnupla<Periodo, double>(horizonte_estudo, vazao_defluente_minima)); //Tem que inicializar o vetor porque o registro "DF" (registro taxa de defluencia) informa os valores por periodo
							//a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_considera_evaporacao, considera_evaporacao);
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_volume_morto, volume_morto);
							a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_vertimento_maximo, vertimento_maximo);

							a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_bacia, atribui_bacia_hidrografica(a_dados, codigo_usina));

							lista_codigo_ONS_REE.setElemento(idHidreletrica, codigo_REE_CEPEL); //Precisa para setar a defluencia_disponivel_minima

							//Determina se a usina está em operação

							if (idEstagio_entrada_operacao != IdEstagio_1) {

								SmartEnupla<IdHidreletrica, Periodo> lista_hidreletrica_periodo(1);

								lista_hidreletrica_periodo.addElemento(idHidreletrica, periodo_entrada_operacao);

								int iterador = int(matriz_hidreletrica_periodo_entrada_em_operacao.size());
								iterador++;

								matriz_hidreletrica_periodo_entrada_em_operacao.addElemento(iterador, lista_hidreletrica_periodo);

							}//if (idEstagio_entrada_operacao != IdEstagio_1) {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro UH: \n" + std::string(erro.what())); }

					}//if (menemonico == "UH") {
					if (menemonico == "CT") {//Usinas térmicas

						try {


							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Número da usina térmica
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//codigo_usina
							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_usina = atoi(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -   Índice do subsistema ao qual pertence a usina. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							//Identifica se alguma termelétrica tem sido inicializada com codigo_usina

							const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, atoi(atributo.c_str()));

							if (idSubmercado == IdSubmercado_Nenhum)
								throw std::invalid_argument("Submercado nao instanciado com codigo_usina_" + atributo);


							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 4 -   Nome da usina térmica. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							atributo = line.substr(14, 10);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const string nome = atributo;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 5 -   Índice do estágio. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							atributo = line.substr(24, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_termeletrica = getIdEstagioFromChar(atributo.c_str());

							//******************************************************************************************************************
							//Nota: Até o campo 5 estamos considerando obrigatório no DECK ter um valor assignado, portanto, a partir deste ponto é necessário 
							//      verificar o tamanho do número de caracteres da string line
							//******************************************************************************************************************

							const int line_size = int(line.length());

							int numero_patamares = 0;

							//Vetores com informação por patamar
							std::vector<double> potencia_minima;
							std::vector<double> potencia_maxima;
							std::vector<double> custo_operacao;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campos 6 - 7 - 8  
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							if (line_size >= 49) {//Por cada patamar são feito 3 registros: potencia_minima, potencia_maxima, custo_operacao

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 6 - Geração mínima fixa da usina,em MWmed, no patamar 1.
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(29, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								potencia_minima.push_back(atof(atributo.c_str()));

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 7 - Capacidade de geração da usina,em MWmed, no patamar 1. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(34, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								potencia_maxima.push_back(atof(atributo.c_str()));

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 8 - Custo de geração da usina térmica, em $/MWh, no patamar 1. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(39, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								custo_operacao.push_back(atof(atributo.c_str()));

							}//if (line_size >= 49) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campos 9 - 10 - 11  
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							if (line_size >= 69) {//Por cada patamar são feito 3 registros: potencia_minima, potencia_maxima, custo_operacao

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 9 - Geração mínima fixa da usina,em MWmed, no patamar 2.
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(49, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								potencia_minima.push_back(atof(atributo.c_str()));

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 10 - Capacidade de geração da usina,em MWmed, no patamar 2. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(54, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								potencia_maxima.push_back(atof(atributo.c_str()));

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 11 - Custo de geração da usina térmica, em $/MWh, no patamar 2. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(59, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								custo_operacao.push_back(atof(atributo.c_str()));

							}//if (line_size >= 69) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campos 12 - 13 - 14  
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							if (line_size >= 89) {//Por cada patamar são feito 3 registros: potencia_minima, potencia_maxima, custo_operacao

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 12 - Geração mínima fixa da usina,em MWmed, no patamar 3.
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(69, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								potencia_minima.push_back(atof(atributo.c_str()));

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 13 - Capacidade de geração da usina,em MWmed, no patamar 3. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(74, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								potencia_maxima.push_back(atof(atributo.c_str()));

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 14 - Custo de geração da usina térmica, em $/MWh, no patamar 3. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(79, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								custo_operacao.push_back(atof(atributo.c_str()));

							}//if (line_size >= 89) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campos 15 - 16 - 17  
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							if (line_size >= 109) {//Por cada patamar são feito 3 registros: potencia_minima, potencia_maxima, custo_operacao

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 12 - Geração mínima fixa da usina,em MWmed, no patamar 3.
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(89, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								potencia_minima.push_back(atof(atributo.c_str()));

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 13 - Capacidade de geração da usina,em MWmed, no patamar 3. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(94, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								potencia_maxima.push_back(atof(atributo.c_str()));

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 14 - Custo de geração da usina térmica, em $/MWh, no patamar 3. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(99, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								custo_operacao.push_back(atof(atributo.c_str()));

							}//if (line_size >= 109) {


							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campos 18 - 19 - 20 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							if (line_size >= 129) {//Por cada patamar são feito 3 registros: potencia_minima, potencia_maxima, custo_operacao

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 12 - Geração mínima fixa da usina,em MWmed, no patamar 3.
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(109, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								potencia_minima.push_back(atof(atributo.c_str()));

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 13 - Capacidade de geração da usina,em MWmed, no patamar 3. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(114, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								potencia_maxima.push_back(atof(atributo.c_str()));

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 14 - Custo de geração da usina térmica, em $/MWh, no patamar 3. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(119, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								custo_operacao.push_back(atof(atributo.c_str()));

							}//if (line_size >= 129) {

							/////////////////////////////////////////
							//Guarda a informação nos SmartElementos
							/////////////////////////////////////////

							//Filosofia: A informação registrada é válida para o idEstagio >= idEstagio_leitura: No primeiro estágio preenche todos 
							//           os valores dos estágios restantes e logo é sobreescrita a informação para idEstágio > 1


							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Inicializa termelétrica
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							if (getIdFromCodigoONS(lista_codigo_ONS_termeletrica, codigo_usina) == IdTermeletrica_Nenhum) {

								Termeletrica termeletrica;

								const IdTermeletrica idTermeletrica = IdTermeletrica(a_dados.getMaiorId(IdTermeletrica()) + 1);

								termeletrica.setAtributo(AttComumTermeletrica_idTermeletrica, idTermeletrica);

								a_dados.vetorTermeletrica.add(termeletrica);

								a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_submercado, idSubmercado);
								a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_nome, nome);
								//a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_tipo_termeletrica, TipoTermeletrica_convencional);
								a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_considerar_usina, true);
								a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica_por_usina);
								a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_codigo_usina, codigo_usina);

								lista_codigo_ONS_termeletrica.addElemento(idTermeletrica, codigo_usina);

								//Cria vetores potencia_minima, potencia_maxima, custo_de_operacao

								SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> enupla_zero(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(numero_patamares, 0.0)));

								a_dados.vetorTermeletrica.att(idTermeletrica).setMatriz(AttMatrizTermeletrica_custo_de_operacao, enupla_zero);

								//Inicializa uma unidade termelétrica (necessário para a validação das termelétricas)

								UnidadeUTE unidadeUTE;
								unidadeUTE.setAtributo(AttComumUnidadeUTE_idUnidadeUTE, IdUnidadeUTE_1);

								unidadeUTE.setMatriz(AttMatrizUnidadeUTE_potencia_minima, enupla_zero);
								unidadeUTE.setMatriz(AttMatrizUnidadeUTE_potencia_maxima, enupla_zero);

								a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.add(unidadeUTE);

							}//if (getIdFromCodigoONS(lista_codigo_ONS_termeletrica, codigo_usina) == IdTermeletrica_Nenhum) {


							//Atualiza informaçao

							const IdTermeletrica idTermeletrica = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, codigo_usina);

							if (idTermeletrica == IdTermeletrica_Nenhum)
								throw std::invalid_argument("Nao inicializada idTermeletrica com codigo_usina_" + getString(codigo_usina) + " nomeUTE_" + nome);

							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								if (periodo >= horizonte_otimizacao_DC.at(idEstagio_termeletrica)) {

									for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

										a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).setElemento(AttMatrizUnidadeUTE_potencia_minima, periodo, idPatamarCarga, potencia_minima.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));
										a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).setElemento(AttMatrizUnidadeUTE_potencia_maxima, periodo, idPatamarCarga, potencia_maxima.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));
										a_dados.vetorTermeletrica.att(idTermeletrica).setElemento(AttMatrizTermeletrica_custo_de_operacao, periodo, idPatamarCarga, custo_operacao.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));

									}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

								}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_termeletrica)) {

							}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro CT: \n" + std::string(erro.what())); }

					}//if (menemonico == "CT") {
					if (menemonico == "UE") {//Estações de bombeamento 

						try {
							IdUsinaElevatoria idUsinaElevatoria;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Número da estação de bombeamento. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//codigo_usina
							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_usina = atoi(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -   Índice do subsistema da estação de bombeamento, conforme campo 2 do registro SB.  
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, atoi(atributo.c_str()));

							if (idSubmercado == IdSubmercado_Nenhum)
								throw std::invalid_argument("Submercado nao instanciado com codigo_usina_" + atributo);


							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 4 -   Nome da estação de bombeamento.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							atributo = line.substr(14, 12);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const string nome = atributo;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 5 -   Número da usina hidrelétrica a montante, conforme campo 2 do registro UH. (MONTANTE = DESTINO)
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							atributo = line.substr(29, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdHidreletrica idHidreletrica_destino = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, atoi(atributo.c_str()));

							if (idHidreletrica_destino == IdHidreletrica_Nenhum)
								throw std::invalid_argument("Hidreletrica nao instanciada com codigo_usina_" + atributo);

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 6 -   Número da usina hidrelétrica a jusante, conforme campo 2 do registro UH. (JUSANTE = ORIGEM)
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							atributo = line.substr(34, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdHidreletrica idHidreletrica_origem = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, atoi(atributo.c_str()));

							if (idHidreletrica_origem == IdHidreletrica_Nenhum)
								throw std::invalid_argument("Hidreletrica nao instanciada com codigo_usina_" + atributo);

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 7 -   Vazão mínima bombeável, (m3/s). 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							atributo = line.substr(39, 10);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double bombeamento_minimo = atof(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 8 -   Vazão máxima bombeável, (m3/s).  
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							atributo = line.substr(49, 10);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double bombeamento_maximo = atof(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 9 -   Taxa de consumo (MWmed/m3/s) da estação de bombeamento.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							atributo = line.substr(59, 10);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double taxa_de_consumo = atof(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Armazena a informação nos SmartElementos
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							//Identifica se alguma usina elevatoria tem sido inicializada com codigo_usina

							const IdUsinaElevatoria idUsinaElevatoria_inicializado = getIdFromCodigoONS(lista_codigo_ONS_usina_elevatoria, codigo_usina);

							if (idUsinaElevatoria_inicializado == IdUsinaElevatoria_Nenhum) { //não inicializada

								UsinaElevatoria usinaElevatoria;

								idUsinaElevatoria = a_dados.getMaiorId(IdUsinaElevatoria());

								if (idUsinaElevatoria == IdUsinaElevatoria_Nenhum)
									idUsinaElevatoria = IdUsinaElevatoria_1;
								else
									idUsinaElevatoria++;

								usinaElevatoria.setAtributo(AttComumUsinaElevatoria_idUsinaElevatoria, idUsinaElevatoria);
								usinaElevatoria.setAtributo(AttComumUsinaElevatoria_submercado, idSubmercado);
								usinaElevatoria.setAtributo(AttComumUsinaElevatoria_nome, nome);
								usinaElevatoria.setAtributo(AttComumUsinaElevatoria_usina_origem_bombeamento, idHidreletrica_origem);
								usinaElevatoria.setAtributo(AttComumUsinaElevatoria_usina_destino_bombeamento, idHidreletrica_destino);
								usinaElevatoria.setAtributo(AttComumUsinaElevatoria_vazao_bombeada_minima, bombeamento_minimo);
								usinaElevatoria.setAtributo(AttComumUsinaElevatoria_vazao_bombeada_maxima, bombeamento_maximo);
								usinaElevatoria.setAtributo(AttComumUsinaElevatoria_taxa_de_consumo, taxa_de_consumo);

								lista_codigo_ONS_usina_elevatoria.addElemento(idUsinaElevatoria, codigo_usina);

								//Cria vetor fator_disponibilidade com valores default 1.0 p.u
								const SmartEnupla<Periodo, double> vetor_fator_disponibilidade(horizonte_estudo, 1.0); //Valor default
								usinaElevatoria.setVetor(AttVetorUsinaElevatoria_fator_disponibilidade, vetor_fator_disponibilidade);

								usinaElevatoria.setVetor(AttVetorUsinaElevatoria_vazao_bombeada_minima, SmartEnupla<Periodo, double>(horizonte_estudo, bombeamento_minimo));
								usinaElevatoria.setVetor(AttVetorUsinaElevatoria_vazao_bombeada_maxima, SmartEnupla<Periodo, double>(horizonte_estudo, bombeamento_maximo));


								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Inicializa Smart elemento
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								a_dados.vetorUsinaElevatoria.add(usinaElevatoria);

							}//if (int(idUsinaElevatoria_inicializado.size()) == 0) {
							else { //já inicializada

								idUsinaElevatoria = idUsinaElevatoria_inicializado;

								a_dados.vetorUsinaElevatoria.att(idUsinaElevatoria).setAtributo(AttComumUsinaElevatoria_submercado, idSubmercado);
								a_dados.vetorUsinaElevatoria.att(idUsinaElevatoria).setAtributo(AttComumUsinaElevatoria_nome, nome);
								a_dados.vetorUsinaElevatoria.att(idUsinaElevatoria).setAtributo(AttComumUsinaElevatoria_usina_origem_bombeamento, idHidreletrica_origem);
								a_dados.vetorUsinaElevatoria.att(idUsinaElevatoria).setAtributo(AttComumUsinaElevatoria_usina_destino_bombeamento, idHidreletrica_destino);
								a_dados.vetorUsinaElevatoria.att(idUsinaElevatoria).setAtributo(AttComumUsinaElevatoria_vazao_bombeada_minima, bombeamento_minimo);
								a_dados.vetorUsinaElevatoria.att(idUsinaElevatoria).setAtributo(AttComumUsinaElevatoria_vazao_bombeada_minima, bombeamento_maximo);
								a_dados.vetorUsinaElevatoria.att(idUsinaElevatoria).setAtributo(AttComumUsinaElevatoria_taxa_de_consumo, taxa_de_consumo);

								//Cria vetor fator_disponibilidade com valores default 1.0 p.u
								const SmartEnupla<Periodo, double> vetor_fator_disponibilidade(horizonte_estudo, 1.0); //Valor default
								a_dados.vetorUsinaElevatoria.att(idUsinaElevatoria).setVetor(AttVetorUsinaElevatoria_fator_disponibilidade, vetor_fator_disponibilidade);

								a_dados.vetorUsinaElevatoria.att(idUsinaElevatoria).setVetor(AttVetorUsinaElevatoria_vazao_bombeada_minima, SmartEnupla<Periodo, double>(horizonte_estudo, bombeamento_minimo));
								a_dados.vetorUsinaElevatoria.att(idUsinaElevatoria).setVetor(AttVetorUsinaElevatoria_vazao_bombeada_maxima, SmartEnupla<Periodo, double>(horizonte_estudo, bombeamento_maximo));

							}//else if (int(idTermeletrica_inicializado.size()) == 1) {

							//Nota: Criar o vetor fator_disponibilidade com valores default 1, logo se encontra o menemonico ME atualiza estes valores

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro UE: \n" + std::string(erro.what())); }

					}//if (menemonico == "UE") {
					if (menemonico == "ME") {//Dados de manutenção das estações de bombeamento

						try {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Campo 2 -  Número da estação de bombeamento. 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//codigo_usina
							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_usina = atoi(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -   Índice do subsistema ao qual pertence a estação de bombeamento.  
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, atoi(atributo.c_str()));

							if (idSubmercado == IdSubmercado_Nenhum)
								throw std::invalid_argument("Submercado nao instanciado com codigo_usina_" + atributo);


							//Identifica se alguma usina elevatoria tem sido inicializada com codigo_usina

							const IdUsinaElevatoria idUsinaElevatoria_inicializado = getIdFromCodigoONS(lista_codigo_ONS_usina_elevatoria, codigo_usina);

							if (idUsinaElevatoria_inicializado != IdUsinaElevatoria_Nenhum) {

								const IdUsinaElevatoria  idUsinaElevatoria = idUsinaElevatoria_inicializado;

								if (a_dados.getAtributo(idUsinaElevatoria, AttComumUsinaElevatoria_submercado, IdSubmercado()) != idSubmercado)
									throw std::invalid_argument("IdSubmercado do registro ME diferente do registro UE para a usina elevatoria com codigo_usina_" + codigo_usina);

								IdEstagio idEstagio_usinaElevatoria = IdEstagio_Nenhum;

								//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Nota: Campo não encontrado nos Decks pesquisados, geralmente, esta informação vem como default no campo UE
								//      Segundo o manual do usuário, são 17 campos com 85 colunas, portanto, a lógica de leitura considera 5 colunas/campo
								//      Cada campo corresponderia a um estágio
								//      Verifica o tamanho da linha a ser lida
								//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								const int line_size = int(line.length());

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 4 a 20 -   Fatores de disponibilidade para os N estágios em p.u..  
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								int conteio = 0;

								while (line_size >= 19 + conteio * 5) {

									atributo = line.substr(14 + conteio * 5, 5);
									atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

									const double fator_disponibilidade = atof(atributo.c_str());

									idEstagio_usinaElevatoria++;

									for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

										//Atualiza a informação unicamente para o estágio DC indicado

										if (horizonte_otimizacao_DC.getIteradorFinal() == idEstagio_usinaElevatoria) {

											//Se é último estágio DC, o periodo deve ser maior ou igual ao estágio DC

											if (periodo >= horizonte_otimizacao_DC.at(idEstagio_usinaElevatoria))
												a_dados.vetorUsinaElevatoria.att(idUsinaElevatoria).setElemento(AttVetorUsinaElevatoria_fator_disponibilidade, periodo, fator_disponibilidade);

										}//if (horizonte_otimizacao_DC.getIteradorFinal() == idEstagio_usinaElevatoria) {
										else {

											//Se não é último estágio DC, o periodo deve ser entre dois estágios DC consecutivos

											const IdEstagio idEstagio_usinaElevatoria_seguinte = IdEstagio(idEstagio_usinaElevatoria + 1);

											if (periodo >= horizonte_otimizacao_DC.at(idEstagio_usinaElevatoria) && periodo < horizonte_otimizacao_DC.at(idEstagio_usinaElevatoria_seguinte))
												a_dados.vetorUsinaElevatoria.att(idUsinaElevatoria).setElemento(AttVetorUsinaElevatoria_fator_disponibilidade, periodo, fator_disponibilidade);

										}//else {

									}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									conteio++;

								}//while (line_size >= 19) {

							}//if (idUsinaElevatoria_inicializado != IdUsinaElevatoria_Nenhum) {
							else
								throw std::invalid_argument("Leitura do registro ME com usina elevatoria nao instanciada com codigo_usina_" + codigo_usina);

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro ME: \n" + std::string(erro.what())); }

					}//if (menemonico == "ME") {

					if (menemonico == "DP") {//Carga dos subsistemas					

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Campo 2 -  Número de identificação do estágio (em ordem crescente, máximo igual a 24).
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							atributo = line.substr(4, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_demanda = getIdEstagioFromChar(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Índice do subsistema .
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_usina = atoi(atributo.c_str());

							const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_usina);

							if (idSubmercado == IdSubmercado_Nenhum)
								throw std::invalid_argument("Submercado nao instanciado com codigo_usina_" + codigo_usina);


							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Filosofia: Verifica o tamanho da linha se existem dados de demanda e duração para determinar um novo patamar de carga
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							int numero_patamares = 0;

							double valor;

							std::vector<double> demanda_patamar;
							std::vector<double> duracao_horas_patamar;

							const int line_size = int(line.length());

							if (line_size >= 39) {

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 4 - Carga do patamar 1,em MWmed. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(19, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								if (atributo == "")
									valor = 0;
								else
									valor = atof(atributo.c_str());

								demanda_patamar.push_back(valor);

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 5 - Duração do patamar 1, em horas.  
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(29, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								duracao_horas_patamar.push_back(atof(atributo.c_str()));

							}//if (line_size >= 39) {

							if (line_size >= 59) {

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 6 - Carga do patamar 2,em MWmed. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(39, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								if (atributo == "")
									valor = 0;
								else
									valor = atof(atributo.c_str());

								demanda_patamar.push_back(valor);

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 7 - Duração do patamar 2, em horas.  
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(49, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								duracao_horas_patamar.push_back(atof(atributo.c_str()));

							}//if (line_size >= 59) {

							if (line_size >= 79) {

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 8 - Carga do patamar 3,em MWmed. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(59, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								if (atributo == "")
									valor = 0;
								else
									valor = atof(atributo.c_str());

								demanda_patamar.push_back(valor);

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 9 - Duração do patamar 3, em horas.  
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(69, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								duracao_horas_patamar.push_back(atof(atributo.c_str()));

							}//if (line_size >= 79) {

							if (line_size >= 99) {

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 10 - Carga do patamar 4,em MWmed. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(79, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								if (atributo == "")
									valor = 0;
								else
									valor = atof(atributo.c_str());

								demanda_patamar.push_back(valor);

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 11 - Duração do patamar 4, em horas.  
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(89, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								duracao_horas_patamar.push_back(atof(atributo.c_str()));

							}//if (line_size >= 99) {

							if (line_size >= 119) {

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 10 - Carga do patamar 5,em MWmed. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(99, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								if (atributo == "")
									valor = 0;
								else
									valor = atof(atributo.c_str());

								demanda_patamar.push_back(valor);

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 11 - Duração do patamar 5, em horas.  
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(109, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								duracao_horas_patamar.push_back(atof(atributo.c_str()));

							}//if (line_size >= 119) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Guarda informação nos Smart Elementos
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							///////////////////////////////////////////////////////////////////////////
							//Para o estágio 1 - Inicializa matrizes com valor 0 para todos os estágios
							///////////////////////////////////////////////////////////////////////////

							if (idEstagio_demanda == IdEstagio_1) {

								std::vector<double> vetor_zero(numero_patamares, 0.0);

								const SmartEnupla<IdPatamarCarga, double> vetor_demanda_patamar(IdPatamarCarga_1, vetor_zero);
								const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_demanda_patamar(horizonte_estudo, vetor_demanda_patamar);

								a_dados.vetorSubmercado.att(idSubmercado).setMatriz(AttMatrizSubmercado_demanda, matriz_demanda_patamar);

								if (idSubmercado == IdSubmercado_1) {

									const SmartEnupla<IdPatamarCarga, double> vetor_percentual_duracao_patamar(IdPatamarCarga_1, vetor_zero);
									const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_percentual_duracao_patamar(horizonte_estudo, vetor_percentual_duracao_patamar);

									a_dados.setMatriz(AttMatrizDados_percentual_duracao_patamar_carga, matriz_percentual_duracao_patamar);

								}//if (idSubmercado == IdSubmercado_1) {

							}//if (idEstagio_demanda == IdEstagio_1) {

							///////////////////////////////////////////////////////////////////////////
							//Atualiza informação de cada estágio 
							///////////////////////////////////////////////////////////////////////////	

							const int size_duracao_horas_patamar = int(duracao_horas_patamar.size());

							std::vector<double> percentual_duracao_patamar;

							double horasTotais = 0;

							for (int pos = 0; pos < size_duracao_horas_patamar; pos++)
								horasTotais += duracao_horas_patamar.at(pos);

							for (int pos = 0; pos < size_duracao_horas_patamar; pos++)
								percentual_duracao_patamar.push_back(duracao_horas_patamar.at(pos) * std::pow(horasTotais, -1));

							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								//Atualiza a informação unicamente para o estágio DC indicado

								if (horizonte_otimizacao_DC.getIteradorFinal() == idEstagio_demanda) {

									//Se é último estágio DC, o periodo deve ser maior ou igual ao estágio DC

									if (periodo >= horizonte_otimizacao_DC.at(idEstagio_demanda)) {

										for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

											a_dados.vetorSubmercado.att(idSubmercado).setElemento(AttMatrizSubmercado_demanda, periodo, idPatamarCarga, demanda_patamar.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));

											a_dados.setElemento(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, percentual_duracao_patamar.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));

										}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

									}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_demanda)) {

								}//if (horizonte_otimizacao_DC.getIteradorFinal() == idEstagio_demanda) {
								else {

									//Se não é último estágio DC, o periodo deve ser entre dois estágios DC consecutivos

									const IdEstagio idEstagio_demanda_seguinte = IdEstagio(idEstagio_demanda + 1);

									if (periodo >= horizonte_otimizacao_DC.at(idEstagio_demanda) && periodo < horizonte_otimizacao_DC.at(idEstagio_demanda_seguinte)) {

										for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

											a_dados.vetorSubmercado.att(idSubmercado).setElemento(AttMatrizSubmercado_demanda, periodo, idPatamarCarga, demanda_patamar.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));

											a_dados.setElemento(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, percentual_duracao_patamar.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));

										}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

									}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_demanda) && periodo < horizonte_otimizacao_DC.at(idEstagio_demanda_seguinte)) {

									else if (periodo >= horizonte_otimizacao_DC.at(idEstagio_demanda_seguinte))
										break; //Evita percorrer todo o horizonte_estudo

								}//else {

							}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {					

							leitura_registro_DP = true;

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro DP: \n" + std::string(erro.what())); }

					}//if (menemonico == "DP") {

					if (menemonico == "CD") {//Custo de déficit

						try {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Campo 2 -  Número da curva de deficit 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_usina = atoi(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Índice do subsistema ao qual pertence a curva. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////					
							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, atoi(atributo.c_str()));

							if (idSubmercado == IdSubmercado_Nenhum)
								throw std::invalid_argument("Submercado nao instanciado com codigo_usina_" + atributo);

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 4 -  Nome da curva de deficit. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(14, 10);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const string nome = atributo;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 5 -  Índice do estágio.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(24, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_patamar_deficit = getIdEstagioFromChar(atributo.c_str());

							/////////////////////////////////

							std::vector<double> patamarDeficit_percentual_da_demanda;
							std::vector<double> patamarDeficit_custo;

							int numero_patamares = 0;
							const int line_size = int(line.length());

							if (line_size >= 44) {

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 6 - Intervalo da curva de custo de deficit em percentual da carga, associado ao 1 o patamar de carga. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(29, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								patamarDeficit_percentual_da_demanda.push_back(atof(atributo.c_str()) / 100);

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 7 - Custo de deficit associado ao 1o patamar de carga em $/MWh.  
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(34, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								patamarDeficit_custo.push_back(atof(atributo.c_str()));

							}//if (line_size >= 44) {

							if (line_size >= 59) {

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 8 - Intervalo da curva de custo de deficit em percentual da carga, associado ao 2 o patamar de carga
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(44, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								patamarDeficit_percentual_da_demanda.push_back(atof(atributo.c_str()) / 100);

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 9 - Custo de deficit associado ao 2o patamar de carga em $/MWh.
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(49, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								patamarDeficit_custo.push_back(atof(atributo.c_str()));

							}//if (line_size >= 59) {

							if (line_size >= 74) {

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 10 - Intervalo da curva de custo de deficit em percentual da carga, associado ao 3 o patamar de carga
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(59, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								patamarDeficit_percentual_da_demanda.push_back(atof(atributo.c_str()) / 100);

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 11 - Custo de deficit associado ao 3o patamar de carga em $/MWh.
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(64, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								patamarDeficit_custo.push_back(atof(atributo.c_str()));

							}//if (line_size >= 74) {

							if (line_size >= 89) {

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 12 - Intervalo da curva de custo de deficit em percentual da carga, associado ao 4 o patamar de carga
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(74, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								patamarDeficit_percentual_da_demanda.push_back(atof(atributo.c_str()) / 100);

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 13 - Custo de deficit associado ao 4o patamar de carga em $/MWh.
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(79, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								patamarDeficit_custo.push_back(atof(atributo.c_str()));

							}//if (line_size >= 89) {

							if (line_size >= 104) {

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 14 - Intervalo da curva de custo de deficit em percentual da carga, associado ao 5 o patamar de carga
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(89, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								patamarDeficit_percentual_da_demanda.push_back(atof(atributo.c_str()) / 100);

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 15 - Custo de deficit associado ao 5o patamar de carga em $/MWh.
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(94, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								patamarDeficit_custo.push_back(atof(atributo.c_str()));

							}//if (line_size >= 104) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Filosofia, sobreescreve a informação para t >= idEstagio
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							if (idEstagio_patamar_deficit == IdEstagio_1) { //Inicializa PatamarDeficit

								PatamarDeficit patamarDeficit;

								//IdPatamarDeficit idPatamarDeficit = a_dados.vetorSubmercado.att(idSubmercado).vetorPatamarDeficit.getMaiorId();

								IdPatamarDeficit idPatamarDeficit = a_dados.getMaiorId(idSubmercado, IdPatamarDeficit());

								if (idPatamarDeficit == IdPatamarDeficit_Nenhum)
									idPatamarDeficit = IdPatamarDeficit_1;
								else
									idPatamarDeficit++;

								patamarDeficit.setAtributo(AttComumPatamarDeficit_idPatamarDeficit, idPatamarDeficit);
								patamarDeficit.setAtributo(AttComumPatamarDeficit_nome, nome);

								if (idPatamarDeficit == IdPatamarDeficit_1) {

									SmartEnupla<IdPatamarDeficit, int> lista_codigo_usina_patamar_deficit(1);

									lista_codigo_usina_patamar_deficit.addElemento(idPatamarDeficit, codigo_usina);

									matriz_codigo_ONS_patamar_deficit.addElemento(idSubmercado, lista_codigo_usina_patamar_deficit);

								}//if (idPatamarDeficit == IdPatamarDeficit_1) {
								else
									matriz_codigo_ONS_patamar_deficit.at(idSubmercado).addElemento(idPatamarDeficit, codigo_usina);


								const SmartEnupla<IdPatamarCarga, double> vetor_patamarDeficit_percentual_da_demanda(IdPatamarCarga_1, patamarDeficit_percentual_da_demanda);
								const SmartEnupla<IdPatamarCarga, double> vetor_patamarDeficit_custo(IdPatamarCarga_1, patamarDeficit_custo);

								const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_patamarDeficit_percentual_da_demanda(horizonte_estudo, vetor_patamarDeficit_percentual_da_demanda);
								const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_patamarDeficit_custo(horizonte_estudo, vetor_patamarDeficit_custo);

								patamarDeficit.setMatriz(AttMatrizPatamarDeficit_percentual, matriz_patamarDeficit_percentual_da_demanda);
								patamarDeficit.setMatriz(AttMatrizPatamarDeficit_custo, matriz_patamarDeficit_custo);

								a_dados.vetorSubmercado.att(idSubmercado).vetorPatamarDeficit.add(patamarDeficit);

							}//if (int(idPatamarDeficit_inicializado.size()) == 1) {
							else {

								//Identifica o PatamarDeficit inicializado com codigo_usina
								const IdPatamarDeficit idPatamarDeficit_inicializado = getIdFromCodigoONS(matriz_codigo_ONS_patamar_deficit, idSubmercado, codigo_usina);

								if (idPatamarDeficit_inicializado == IdPatamarDeficit_Nenhum)
									throw std::invalid_argument("Nao inicializada idContrato com codigo_usina_" + getString(codigo_usina));

								const IdPatamarDeficit  idPatamarDeficit = idPatamarDeficit_inicializado;

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									if (periodo >= horizonte_otimizacao_DC.at(idEstagio_patamar_deficit)) {

										for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

											a_dados.vetorSubmercado.att(idSubmercado).vetorPatamarDeficit.att(idPatamarDeficit).setElemento(AttMatrizPatamarDeficit_percentual, periodo, idPatamarCarga, patamarDeficit_percentual_da_demanda.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));
											a_dados.vetorSubmercado.att(idSubmercado).vetorPatamarDeficit.att(idPatamarDeficit).setElemento(AttMatrizPatamarDeficit_custo, periodo, idPatamarCarga, patamarDeficit_custo.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));

										}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

									}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_patamar_deficit)) {

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							}//else {

							leitura_registro_CD = true;

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro CD: \n" + std::string(erro.what())); }

					}//if (menemonico == "CD") {

					//Se a leitura dos submercados/ cargas dos submercados /custo déficit é finalizada (Registro SB/DP/CD) então inicializa submercados IV e ANDE
					if (leitura_registro_SB == true && menemonico != "SB" && leitura_registro_DP == true && menemonico != "DP" && leitura_registro_CD == true && menemonico != "CD") {

						inicializa_Submercados_Intercambios_Nao_Registrados(a_dados);

						leitura_registro_SB = false;
						leitura_registro_DP = false;
						leitura_registro_CD = false;

					}//if (leitura_registro_SB == true && menemonico != "SB") {

					if (menemonico == "BE") {//Geração em bacias especiais/pequenas bacias

						try {
							const int numero_submercados = getintFromChar(getString(a_dados.getMaiorId(IdSubmercado())).c_str());

							if (inicializar_conteio_numero_usina_nao_simulado_por_submercado == true) {

								SmartEnupla <Periodo, int> vetor_zero(horizonte_estudo, 0);

								for (int submercado = 0; submercado < numero_submercados; submercado++)
									conteio_numero_usina_nao_simulado_por_submercado.addElemento(IdSubmercado(submercado + 1), vetor_zero);

								inicializar_conteio_numero_usina_nao_simulado_por_submercado = false;

							}//if (inicializar_conteio_numero_usina_nao_simulado_por_submercado == true) {


							//Esta geração vai ser considerada dentro das UsinaNaoSimulada

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Nome da bacia. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 10);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const string nome = atributo;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Índice do submercado ao qual pertence a bacia.  
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////					
							atributo = line.substr(14, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, atoi(atributo.c_str()));

							if (idSubmercado == IdSubmercado_Nenhum)
								throw std::invalid_argument("Submercado nao instanciado com codigo_usina_" + atributo);


							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 4 -  Identificação do estágio correspondente às gerações.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(19, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_bacia_especial = getIdEstagioFromChar(atributo.c_str());

							////////////////////////////////////////////////////////////////////////////////////////////
							// Campos 5-9 - Geração no patamar 1 em MWmed e Fator de perdas para o centro de gravidade da carga (%) 
							// Manual: O campo “fator de perdas” é o campo seguinte ao campo de geração do último patamar. 
							////////////////////////////////////////////////////////////////////////////////////////////

							const int line_size = int(line.length());

							int numero_patamares;

							if (line_size >= 49 && line.substr(48, 1) != " ")
								numero_patamares = 5;
							else if (line_size >= 44 && line.substr(43, 1) != " ")
								numero_patamares = 4;
							else if (line_size >= 39 && line.substr(38, 1) != " ")
								numero_patamares = 3;
							else if (line_size >= 34 && line.substr(33, 1) != " ")
								numero_patamares = 2;
							else if (line_size >= 29 && line.substr(28, 1) != " ")
								numero_patamares = 1;


							std::vector<double> usinaNaoSimulada_potencia_gerada;

							for (int pat = 0; pat < numero_patamares; pat++) {

								atributo = line.substr(24 + 5 * pat, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								usinaNaoSimulada_potencia_gerada.push_back(atof(atributo.c_str()));

							}//for (int pat = 0; pat < numero_patamares; pat++) {


							//No Deck não aparece esta informação apesar das diretrizes do manual de referência
							double fator_de_perdas;

							if (line_size >= 29 + 5 * numero_patamares) {

								atributo = line.substr(24 + 5 * numero_patamares, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
								fator_de_perdas = atof(atributo.c_str());

							}//if (line_size >= 29 + 5 * numero_patamares) {
							else
								fator_de_perdas = 1; //Default: nos Decks não especificam este valor

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Filosofia, sobreescreve a informação para t >= idEstagio e soma todas as bacias especias guardando um valor total
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							//Identifica o PatamarDeficit inicializado com codigo_usina
							const IdUsinaNaoSimulada idUsinaNaoSimulada = IdUsinaNaoSimulada_1; //No DC o padrao é IdUsinaNaoSimulada agregada

							if (idEstagio_bacia_especial == IdEstagio_1 && !a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.isInstanciado(idUsinaNaoSimulada)) { //UsinaNaoSimulada não instanciada

								UsinaNaoSimulada usinaNaoSimulada;

								usinaNaoSimulada.setAtributo(AttComumUsinaNaoSimulada_idUsinaNaoSimulada, idUsinaNaoSimulada);

								const SmartEnupla<IdPatamarCarga, double> vetor_usinaNaoSimulada_potencia_gerada(IdPatamarCarga_1, usinaNaoSimulada_potencia_gerada);

								const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_usinaNaoSimulada_potencia_gerada(horizonte_estudo, vetor_usinaNaoSimulada_potencia_gerada);

								usinaNaoSimulada.setMatriz(AttMatrizUsinaNaoSimulada_potencia_minima, matriz_usinaNaoSimulada_potencia_gerada);
								usinaNaoSimulada.setMatriz(AttMatrizUsinaNaoSimulada_potencia_maxima, matriz_usinaNaoSimulada_potencia_gerada);

								a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.add(usinaNaoSimulada);

								bool sobreposicao_encontrada = false;

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									double sobreposicao = periodo.sobreposicao(horizonte_otimizacao_DC.at(idEstagio_bacia_especial));

									if (sobreposicao > 0) {
										conteio_numero_usina_nao_simulado_por_submercado.at(idSubmercado).setElemento(periodo, conteio_numero_usina_nao_simulado_por_submercado.at(idSubmercado).getElemento(periodo) + 1);
										sobreposicao_encontrada = true;
									}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_bacia_especial)) {

									if (sobreposicao_encontrada && sobreposicao == 0)
										break;

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							}//if (int(idPatamarDeficit_inicializado.size()) == 1) {
							else if (a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.isInstanciado(idUsinaNaoSimulada)) { //UsinaNaoSimulada instanciada e idEstagio > 1 -> Substrai o valor do estágio anterior (somado previamente) e soma a nova bacia com os valores já registrados

								double valor;

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									if (periodo >= horizonte_otimizacao_DC.at(idEstagio_bacia_especial)) {

										for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

											if (conteio_numero_usina_nao_simulado_por_submercado.at(idSubmercado).getElemento(periodo) == 0 && periodo.sobreposicao(horizonte_otimizacao_DC.at(idEstagio_bacia_especial)) > 0) {
												valor = usinaNaoSimulada_potencia_gerada.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1);
											}//if (conteio == 0) {
											else {
												valor = a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).getElementoMatriz(AttMatrizUsinaNaoSimulada_potencia_maxima, periodo, idPatamarCarga, double());
												valor += usinaNaoSimulada_potencia_gerada.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1);
											}//else {

											a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).setElemento(AttMatrizUsinaNaoSimulada_potencia_minima, periodo, idPatamarCarga, valor);
											a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).setElemento(AttMatrizUsinaNaoSimulada_potencia_maxima, periodo, idPatamarCarga, valor);

										}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

									}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_bacia_especial)) {

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								bool sobreposicao_encontrada = false;

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									double sobreposicao = periodo.sobreposicao(horizonte_otimizacao_DC.at(idEstagio_bacia_especial));

									if (sobreposicao > 0) {
										conteio_numero_usina_nao_simulado_por_submercado.at(idSubmercado).setElemento(periodo, conteio_numero_usina_nao_simulado_por_submercado.at(idSubmercado).getElemento(periodo) + 1);
										sobreposicao_encontrada = true;
									}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_bacia_especial)) {

									if (sobreposicao_encontrada && sobreposicao == 0)
										break;

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							}//else if (idEstagio_bacia_especial == IdEstagio_1 && int(idUsinaNaoSimulada_inicializado.size() == 1))
							else
								throw std::invalid_argument("IdEstagio_1 do registro BE nao instanciado");

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro BE: \n" + std::string(erro.what())); }

					}//if (menemonico == "BE") {


					if (menemonico == "PQ") {//Geração em pequenas usinas

						try {
							const int numero_submercados = getintFromChar(getString(a_dados.getMaiorId(IdSubmercado())).c_str());

							if (inicializar_conteio_numero_usina_nao_simulado_por_submercado == true) {

								SmartEnupla <Periodo, int> vetor_zero(horizonte_estudo, 0);

								for (int submercado = 0; submercado < numero_submercados; submercado++)
									conteio_numero_usina_nao_simulado_por_submercado.addElemento(IdSubmercado(submercado + 1), vetor_zero);

								inicializar_conteio_numero_usina_nao_simulado_por_submercado = false;

							}//if (inicializar_conteio_numero_usina_nao_simulado_por_submercado == true) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Nome da bacia. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 10);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const string nome = atributo;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Índice do submercado ao qual pertence a bacia.  
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////					
							atributo = line.substr(14, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, atoi(atributo.c_str()));

							if (idSubmercado == IdSubmercado_Nenhum)
								throw std::invalid_argument("Submercado nao instanciado com codigo_usina_" + atributo);

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 4 -  Identificação do estágio correspondente às gerações.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(19, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_pequena_usina = getIdEstagioFromChar(atributo.c_str());

							////////////////////////////////////////////////////////////////////////////////////////////
							// Campos 5-10 - Geração no patamar 1 em MWmed e Fator de perdas para o centro de gravidade da carga (%) 
							// Manual: O campo “fator de perdas” é o campo seguinte ao campo de geração do último patamar. 
							////////////////////////////////////////////////////////////////////////////////////////////

							const int line_size = int(line.length());

							int numero_patamares;


							if (line_size >= 49 && line.substr(48, 1) != " ")
								numero_patamares = 5;
							else if (line_size >= 44 && line.substr(43, 1) != " ")
								numero_patamares = 4;
							else if (line_size >= 39 && line.substr(38, 1) != " ")
								numero_patamares = 3;
							else if (line_size >= 34 && line.substr(33, 1) != " ")
								numero_patamares = 2;
							else if (line_size >= 29 && line.substr(28, 1) != " ")
								numero_patamares = 1;

							std::vector<double> usinaNaoSimulada_potencia_gerada;

							for (int pat = 0; pat < numero_patamares; pat++) {

								atributo = line.substr(24 + 5 * pat, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								usinaNaoSimulada_potencia_gerada.push_back(atof(atributo.c_str()));

							}//for (int pat = 0; pat < numero_patamares; pat++) {


							//No Deck não aparece esta informação apesar das diretrizes do manual de referência
							double fator_de_perdas;

							if (line_size >= 29 + 5 * numero_patamares) {

								atributo = line.substr(24 + 5 * numero_patamares, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
								fator_de_perdas = atof(atributo.c_str());

							}//if (line_size >= 29 + 5 * numero_patamares) {
							else
								fator_de_perdas = 1; //Default: nos Decks não especificam este valor


							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Filosofia, sobreescreve a informação para t >= idEstagio e soma todas as bacias especias guardando um valor total
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							const IdUsinaNaoSimulada idUsinaNaoSimulada = IdUsinaNaoSimulada_1;

							if (idEstagio_pequena_usina == IdEstagio_1 && !a_dados.vetorSubmercado.at(idSubmercado).vetorUsinaNaoSimulada.isInstanciado(idUsinaNaoSimulada)) { //UsinaNaoSimulada não instanciada

								UsinaNaoSimulada usinaNaoSimulada;

								usinaNaoSimulada.setAtributo(AttComumUsinaNaoSimulada_idUsinaNaoSimulada, idUsinaNaoSimulada);

								const SmartEnupla<IdPatamarCarga, double> vetor_usinaNaoSimulada_potencia_gerada(IdPatamarCarga_1, usinaNaoSimulada_potencia_gerada);

								const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_usinaNaoSimulada_potencia_gerada(horizonte_estudo, vetor_usinaNaoSimulada_potencia_gerada);

								usinaNaoSimulada.setMatriz(AttMatrizUsinaNaoSimulada_potencia_minima, matriz_usinaNaoSimulada_potencia_gerada);
								usinaNaoSimulada.setMatriz(AttMatrizUsinaNaoSimulada_potencia_maxima, matriz_usinaNaoSimulada_potencia_gerada);

								a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.add(usinaNaoSimulada);

								bool sobreposicao_encontrada = false;

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									double sobreposicao = periodo.sobreposicao(horizonte_otimizacao_DC.at(idEstagio_pequena_usina));

									if (sobreposicao > 0) {
										conteio_numero_usina_nao_simulado_por_submercado.at(idSubmercado).setElemento(periodo, conteio_numero_usina_nao_simulado_por_submercado.at(idSubmercado).getElemento(periodo) + 1);
										sobreposicao_encontrada = true;
									}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_bacia_especial)) {

									if (sobreposicao_encontrada && sobreposicao == 0)
										break;

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							}//
							else if (a_dados.vetorSubmercado.at(idSubmercado).vetorUsinaNaoSimulada.isInstanciado(idUsinaNaoSimulada)) { //UsinaNaoSimulada instanciada e idEstagio > 1 -> Substrai o valor do estágio anterior (somado previamente) e soma a nova bacia com os valores já registrados

								//Identifica o PatamarDeficit inicializado com codigo_usina
								double valor;

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									if (periodo >= horizonte_otimizacao_DC.at(idEstagio_pequena_usina)) {

										for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

											if (conteio_numero_usina_nao_simulado_por_submercado.at(idSubmercado).getElemento(periodo) == 0 && periodo.sobreposicao(horizonte_otimizacao_DC.at(idEstagio_pequena_usina))) {
												valor = usinaNaoSimulada_potencia_gerada.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1);
											}//if (conteio == 0) {
											else {
												valor = a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).getElementoMatriz(AttMatrizUsinaNaoSimulada_potencia_maxima, periodo, idPatamarCarga, double());
												valor += usinaNaoSimulada_potencia_gerada.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1);
											}//else {

											a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).setElemento(AttMatrizUsinaNaoSimulada_potencia_minima, periodo, idPatamarCarga, valor);
											a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).setElemento(AttMatrizUsinaNaoSimulada_potencia_maxima, periodo, idPatamarCarga, valor);

										}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

									}//if (horizonte_estudo.at(periodo) >= idEstagio_bacia_especial) {

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								bool sobreposicao_encontrada = false;

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									double sobreposicao = periodo.sobreposicao(horizonte_otimizacao_DC.at(idEstagio_pequena_usina));

									if (sobreposicao > 0) {
										conteio_numero_usina_nao_simulado_por_submercado.at(idSubmercado).setElemento(periodo, conteio_numero_usina_nao_simulado_por_submercado.at(idSubmercado).getElemento(periodo) + 1);
										sobreposicao_encontrada = true;
									}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_bacia_especial)) {

									if (sobreposicao_encontrada && sobreposicao == 0)
										break;

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							}//
							else
								throw std::invalid_argument("IdEstagio_1 do registro PQ nao instanciado");

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro PQ: \n" + std::string(erro.what())); }

					}//if (menemonico == "PQ") {


					// RESTRIÇÃO DE GERAÇÃO ITAIPU (50/60 HZ)
					if (menemonico == "RI") {

						try {

							const int codigo_usina = std::stoi(line.substr(4, 3));

							const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

							if (idHidreletrica == IdHidreletrica_Nenhum)
								throw std::invalid_argument("Registro RI - hidreletrica nao instanciada com codigo_usina_" + getString(codigo_usina));


							const IdEstagio idEstagio_inicial = IdEstagio(std::stoi(line.substr(8, 3)));

							int num_patamar_carga = 0;

							if (line.size() >= 190) { num_patamar_carga = 5; }
							else if (line.size() >= 155) { num_patamar_carga = 4; }
							else if (line.size() >= 120) { num_patamar_carga = 3; }
							else if (line.size() >= 85) { num_patamar_carga = 2; }
							else if (line.size() >= 50) { num_patamar_carga = 1; }

							IdPatamarCarga maiorIdPatamar = IdPatamarCarga(num_patamar_carga);

							const SmartEnupla<IdPatamarCarga, double> vetor_zero(IdPatamarCarga_1, std::vector<double>(int(maiorIdPatamar), 0.0));
							const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_zero(horizonte_estudo, vetor_zero);

							if (a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

								const IdSubmercado idSubmercado_ande = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_ANDE);

								// VERIFICA SE O CONJUNTO HIDRAULICO JÁ FOI INSTANCIADO
								if (!a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.isInstanciado(IdConjuntoHidraulico_1)) {
									ConjuntoHidraulico conjuntohidraulico;
									conjuntohidraulico.setAtributo(AttComumConjuntoHidraulico_idConjuntoHidraulico, IdConjuntoHidraulico_1);
									a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.add(conjuntohidraulico);
								}
								if (!a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.isInstanciado(IdConjuntoHidraulico_2)) {
									ConjuntoHidraulico conjuntohidraulico;
									conjuntohidraulico.setAtributo(AttComumConjuntoHidraulico_idConjuntoHidraulico, IdConjuntoHidraulico_2);
									a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.add(conjuntohidraulico);
								}


								if (a_dados.getSize1Matriz(idHidreletrica, IdConjuntoHidraulico_1, AttMatrizConjuntoHidraulico_potencia_minima) == 0)
									a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico_1).setMatriz(AttMatrizConjuntoHidraulico_potencia_minima, matriz_zero);

								if (a_dados.getSize1Matriz(idHidreletrica, IdConjuntoHidraulico_2, AttMatrizConjuntoHidraulico_potencia_minima) == 0)
									a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico_2).setMatriz(AttMatrizConjuntoHidraulico_potencia_minima, matriz_zero);

								if (a_dados.getSize1Matriz(idHidreletrica, IdConjuntoHidraulico_1, AttMatrizConjuntoHidraulico_potencia_maxima) == 0)
									a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico_1).setMatriz(AttMatrizConjuntoHidraulico_potencia_maxima, matriz_zero);

								if (a_dados.getSize1Matriz(idHidreletrica, IdConjuntoHidraulico_2, AttMatrizConjuntoHidraulico_potencia_maxima) == 0)
									a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico_2).setMatriz(AttMatrizConjuntoHidraulico_potencia_maxima, matriz_zero);

								if (a_dados.getSize1Matriz(idSubmercado_ande, AttMatrizSubmercado_demanda) == 0)
									a_dados.vetorSubmercado.att(idSubmercado_ande).setMatriz(AttMatrizSubmercado_demanda, matriz_zero);

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									if (periodo >= horizonte_otimizacao_DC.getElemento(idEstagio_inicial)) {
										for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamar; idPatamarCarga++) {

											int passo = (int(idPatamarCarga) - 1) * 35;
											// GERAÇÃO ITAIPU 60 HZ
											a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico_2).setElemento(AttMatrizConjuntoHidraulico_potencia_minima, periodo, idPatamarCarga, std::stod(line.substr(passo + 16, 7)));
											a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico_2).setElemento(AttMatrizConjuntoHidraulico_potencia_maxima, periodo, idPatamarCarga, std::stod(line.substr(passo + 24, 7)));

											// GERAÇÃO ITAIPU 50 HZ
											a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico_1).setElemento(AttMatrizConjuntoHidraulico_potencia_minima, periodo, idPatamarCarga, std::stod(line.substr(passo + 30, 7)));
											a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico_1).setElemento(AttMatrizConjuntoHidraulico_potencia_maxima, periodo, idPatamarCarga, std::stod(line.substr(passo + 37, 7)));

											// CARGA ANDE
											a_dados.vetorSubmercado.att(idSubmercado_ande).setElemento(AttMatrizSubmercado_demanda, periodo, idPatamarCarga, std::stod(line.substr(passo + 44, 7)));

										}
									}//if (periodo >= horizonte_otimizacao_DC.getElemento(idEstagio_inicial)) {
								}

							}//if (a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

							else if (a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) == TipoDetalhamentoProducaoHidreletrica_por_usina) {

								//Na representaçao por usina de Itaipú os limites do intercambio Itaipú->Ande e Itaipú->IV sao atualizados

								const IdSubmercado idSubmercado_itaipu = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_submercado_ITAIPU);
								const IdSubmercado idSubmercado_ande = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_ANDE);
								const IdSubmercado idSubmercado_iv = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_IV);

								std::vector<IdIntercambio> idIntercambio_inicializado = a_dados.vetorIntercambio.getIdObjetos(AttComumIntercambio_submercado_origem, idSubmercado_itaipu);

								int idIntercambio_inicializado_size = int(idIntercambio_inicializado.size());

								IdIntercambio idIntercambio_itaipu_ande = IdIntercambio_Nenhum;

								for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

									if (a_dados.getAtributo(idIntercambio_inicializado.at(intercambio_inicializado), AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_ande) {
										idIntercambio_itaipu_ande = idIntercambio_inicializado.at(intercambio_inicializado);
										break;
									}//if (a_dados.getAtributo(idIntercambio_inicializado.at(intercambio_inicializado), AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_destino) {

								}//for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

								if (idIntercambio_itaipu_ande == IdIntercambio_Nenhum)
									throw std::invalid_argument("Registro RI - Intercambio nao encontrado");

								////////////////

								IdIntercambio idIntercambio_itaipu_iv = IdIntercambio_Nenhum;

								for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

									if (a_dados.getAtributo(idIntercambio_inicializado.at(intercambio_inicializado), AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_iv) {
										idIntercambio_itaipu_iv = idIntercambio_inicializado.at(intercambio_inicializado);
										break;
									}//if (a_dados.getAtributo(idIntercambio_inicializado.at(intercambio_inicializado), AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_destino) {

								}//for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

								if (idIntercambio_itaipu_iv == IdIntercambio_Nenhum)
									throw std::invalid_argument("Registro RI - Intercambio nao encontrado");


								/////////////////////////////

								if (a_dados.getSize1Matriz(idIntercambio_itaipu_ande, AttMatrizIntercambio_potencia_minima) == 0)
									a_dados.vetorIntercambio.att(idIntercambio_itaipu_ande).setMatriz(AttMatrizIntercambio_potencia_minima, matriz_zero);

								if (a_dados.getSize1Matriz(idIntercambio_itaipu_ande, AttMatrizIntercambio_potencia_maxima) == 0)
									a_dados.vetorIntercambio.att(idIntercambio_itaipu_ande).setMatriz(AttMatrizIntercambio_potencia_maxima, matriz_zero);

								if (a_dados.getSize1Matriz(idIntercambio_itaipu_iv, AttMatrizIntercambio_potencia_minima) == 0)
									a_dados.vetorIntercambio.att(idIntercambio_itaipu_iv).setMatriz(AttMatrizIntercambio_potencia_minima, matriz_zero);

								if (a_dados.getSize1Matriz(idIntercambio_itaipu_iv, AttMatrizIntercambio_potencia_maxima) == 0)
									a_dados.vetorIntercambio.att(idIntercambio_itaipu_iv).setMatriz(AttMatrizIntercambio_potencia_maxima, matriz_zero);

								if (a_dados.getSize1Matriz(idSubmercado_ande, AttMatrizSubmercado_demanda) == 0)
									a_dados.vetorSubmercado.att(idSubmercado_ande).setMatriz(AttMatrizSubmercado_demanda, matriz_zero);

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									if (periodo >= horizonte_otimizacao_DC.getElemento(idEstagio_inicial)) {
										for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamar; idPatamarCarga++) {

											int passo = (int(idPatamarCarga) - 1) * 35;
											// GERAÇÃO ITAIPU 60 HZ
											a_dados.vetorIntercambio.att(idIntercambio_itaipu_iv).setElemento(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, std::stod(line.substr(passo + 16, 7)));
											a_dados.vetorIntercambio.att(idIntercambio_itaipu_iv).setElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, std::stod(line.substr(passo + 24, 7)));

											// GERAÇÃO ITAIPU 50 HZ
											a_dados.vetorIntercambio.att(idIntercambio_itaipu_ande).setElemento(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, std::stod(line.substr(passo + 30, 7)));
											a_dados.vetorIntercambio.att(idIntercambio_itaipu_ande).setElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, std::stod(line.substr(passo + 37, 7)));

											// CARGA ANDE
											a_dados.vetorSubmercado.att(idSubmercado_ande).setElemento(AttMatrizSubmercado_demanda, periodo, idPatamarCarga, std::stod(line.substr(passo + 44, 7)));

										}
									}//if (periodo >= horizonte_otimizacao_DC.getElemento(idEstagio_inicial)) {
								}

							}//else if (a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) == TipoDetalhamentoProducaoHidreletrica_por_usina) {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro RI: \n" + std::string(erro.what())); }

					}//if (menemonico == "RI") 


					if (menemonico == "IT") {// Restrição de geração de Itaipu_50 Hz e carga da ANDE

						try {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Campo 2 -  Identificação do estágio. 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_Itaipu = getIdEstagioFromChar(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Índice da usina de Itaipu ( conforme campo 2 dos registros UH).
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(9, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int hidreletrica_codigo_usina = atoi(atributo.c_str());

							const IdHidreletrica idHidreletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, hidreletrica_codigo_usina);

							IdHidreletrica idHidreletrica = idHidreletrica_inicializado;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 4 -  Índice do subsistema que representa o Sudeste.   
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(14, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_usina_conexo_ANDE = atoi(atributo.c_str()); //codigo_usina_conexo_ANDE -> Sudeste

							////////////////////////////////////////

							int numero_patamares = 0;

							//Vetores com informação por patamar
							std::vector<double> itaipu_50Hz_potencia;
							std::vector<double> itaipu_carga_ANDE;

							const int line_size = int(line.length());

							if (line_size >= 29) {

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 5 - Geração de Itaipu_50 Hz, em MWmed, no patamar 1.  
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(19, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								itaipu_50Hz_potencia.push_back(atof(atributo.c_str()));

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 6 - Carga da Ande, em MWmed, no patamar 1.
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(24, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								itaipu_carga_ANDE.push_back(atof(atributo.c_str()));

							}//if (line_size >= 29) {

							if (line_size >= 39) {

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 6 - Geração de Itaipu_50 Hz, em MWmed, no patamar 2.  
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(29, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								itaipu_50Hz_potencia.push_back(atof(atributo.c_str()));

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 7 - Carga da Ande, em MWmed, no patamar 2.
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(34, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								itaipu_carga_ANDE.push_back(atof(atributo.c_str()));

							}//if (line_size >= 39) {

							if (line_size >= 49) {

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 8 - Geração de Itaipu_50 Hz, em MWmed, no patamar 3.  
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(39, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								itaipu_50Hz_potencia.push_back(atof(atributo.c_str()));

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 9 - Carga da Ande, em MWmed, no patamar 3.
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(44, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								itaipu_carga_ANDE.push_back(atof(atributo.c_str()));

							}//if (line_size >= 49) {

							if (line_size >= 59) {

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 10 - Geração de Itaipu_50 Hz, em MWmed, no patamar 4.  
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(49, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								itaipu_50Hz_potencia.push_back(atof(atributo.c_str()));

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 11 - Carga da Ande, em MWmed, no patamar 4.
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(54, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								itaipu_carga_ANDE.push_back(atof(atributo.c_str()));

							}//if (line_size >= 59) {

							if (line_size >= 69) {

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 12 - Geração de Itaipu_50 Hz, em MWmed, no patamar 5.  
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(59, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								itaipu_50Hz_potencia.push_back(atof(atributo.c_str()));

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 13 - Carga da Ande, em MWmed, no patamar 5.
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(64, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								itaipu_carga_ANDE.push_back(atof(atributo.c_str()));

							}//if (line_size >= 69) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Guarda informação nos Smart Elementos
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							if (a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

								IdConjuntoHidraulico idConjuntoHidraulico;

								//*********************************
								//Submercado ANDE
								//*********************************

								const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_ANDE);

								if (idEstagio_Itaipu == IdEstagio_1) {

									///////////////////////////////////
									//Itaipu_50Hz_potencia
									///////////////////////////////////

									if (!a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.isInstanciado(IdConjuntoHidraulico_1)) {

										ConjuntoHidraulico conjuntoHidraulico;
										conjuntoHidraulico.setAtributo(AttComumConjuntoHidraulico_idConjuntoHidraulico, IdConjuntoHidraulico_1);//Itaipu_50Hz é definido como IdConjuntoHidraulico_1 da usina Itaipu

										a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.add(conjuntoHidraulico);

									}//if (!a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.isInstanciado(IdConjuntoHidraulico_1)) {

									///////////////////////////////////

									const string nome = "itaipu_50";

									a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico_1).setAtributo(AttComumConjuntoHidraulico_nome, nome);
									a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico_1).setAtributo(AttComumConjuntoHidraulico_submercado, idSubmercado); //O conjunto hidráulico Itaipú 50 Hz é associado ao submercado ANDE

									const SmartEnupla<IdPatamarCarga, double> vetor_itaipu_50Hz_potencia(IdPatamarCarga_1, itaipu_50Hz_potencia);
									const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_itaipu_50Hz_potencia(horizonte_estudo, vetor_itaipu_50Hz_potencia);

									a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico_1).setMatriz(AttMatrizConjuntoHidraulico_potencia_minima, matriz_itaipu_50Hz_potencia);
									a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico_1).setMatriz(AttMatrizConjuntoHidraulico_potencia_maxima, matriz_itaipu_50Hz_potencia);



									///////////////////////////////////
									//Carga_ANDE
									//////////////////////////////////

									const SmartEnupla<IdPatamarCarga, double> vetor_itaipu_carga_ANDE(IdPatamarCarga_1, itaipu_carga_ANDE);
									const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_itaipu_carga_ANDE(horizonte_estudo, vetor_itaipu_carga_ANDE);

									a_dados.vetorSubmercado.att(idSubmercado).setMatriz(AttMatrizSubmercado_demanda, matriz_itaipu_carga_ANDE);

								}//if (idEstagio_Itaipu == IdEstagio_1) { 
								else {

									idConjuntoHidraulico = IdConjuntoHidraulico_1; //Itaipu_50Hz é definido como IdConjuntoHidraulico_1 da usina Itaipu

									for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

										if (periodo >= horizonte_otimizacao_DC.at(idEstagio_Itaipu)) {

											for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

												//Itaipu_50Hz_potencia

												a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).setElemento(AttMatrizConjuntoHidraulico_potencia_minima, periodo, idPatamarCarga, itaipu_50Hz_potencia.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));
												a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).setElemento(AttMatrizConjuntoHidraulico_potencia_maxima, periodo, idPatamarCarga, itaipu_50Hz_potencia.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));

												//Carga_ANDE

												a_dados.vetorSubmercado.att(idSubmercado).setElemento(AttMatrizSubmercado_demanda, periodo, idPatamarCarga, itaipu_carga_ANDE.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));

											}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

										}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_Itaipu)) {

									}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								}//else {

							}//if (a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {
							else if (a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) == TipoDetalhamentoProducaoHidreletrica_por_usina) {

								//Na representaçao por usina de Itaipú os limites do intercambio Itaipú->Ande sao atualizados

								const IdSubmercado idSubmercado_origem = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_submercado_ITAIPU);
								const IdSubmercado idSubmercado_destino = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_ANDE);

								std::vector<IdIntercambio> idIntercambio_inicializado = a_dados.vetorIntercambio.getIdObjetos(AttComumIntercambio_submercado_origem, idSubmercado_origem);

								int idIntercambio_inicializado_size = int(idIntercambio_inicializado.size());

								IdIntercambio idIntercambio = IdIntercambio_Nenhum;

								for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

									if (a_dados.getAtributo(idIntercambio_inicializado.at(intercambio_inicializado), AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_destino) {
										idIntercambio = idIntercambio_inicializado.at(intercambio_inicializado);
										break;
									}//if (a_dados.getAtributo(idIntercambio_inicializado.at(intercambio_inicializado), AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_destino) {

								}//for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

								if (idIntercambio == IdIntercambio_Nenhum)
									throw std::invalid_argument("Registro IT - Intercambio nao encontrado");

								if (idEstagio_Itaipu == IdEstagio_1) {

									///////////////////////////////////
									//Itaipu_50Hz_potencia
									///////////////////////////////////

									const SmartEnupla<IdPatamarCarga, double> vetor_itaipu_50Hz_potencia(IdPatamarCarga_1, itaipu_50Hz_potencia);
									const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_itaipu_50Hz_potencia(horizonte_estudo, vetor_itaipu_50Hz_potencia);

									a_dados.vetorIntercambio.att(idIntercambio).setMatriz(AttMatrizIntercambio_potencia_minima, matriz_itaipu_50Hz_potencia);
									a_dados.vetorIntercambio.att(idIntercambio).setMatriz(AttMatrizIntercambio_potencia_maxima, matriz_itaipu_50Hz_potencia);


									///////////////////////////////////
									//Carga_ANDE
									//////////////////////////////////

									const SmartEnupla<IdPatamarCarga, double> vetor_itaipu_carga_ANDE(IdPatamarCarga_1, itaipu_carga_ANDE);
									const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_itaipu_carga_ANDE(horizonte_estudo, vetor_itaipu_carga_ANDE);

									a_dados.vetorSubmercado.att(idSubmercado_destino).setMatriz(AttMatrizSubmercado_demanda, matriz_itaipu_carga_ANDE);

								}//if (idEstagio_Itaipu == IdEstagio_1) { 
								else {

									for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

										if (periodo >= horizonte_otimizacao_DC.at(idEstagio_Itaipu)) {

											for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

												//Itaipu_50Hz_potencia

												a_dados.vetorIntercambio.att(idIntercambio).setElemento(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, itaipu_50Hz_potencia.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));
												a_dados.vetorIntercambio.att(idIntercambio).setElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, itaipu_50Hz_potencia.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));

												//Carga_ANDE

												a_dados.vetorSubmercado.att(idSubmercado_destino).setElemento(AttMatrizSubmercado_demanda, periodo, idPatamarCarga, itaipu_carga_ANDE.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));

											}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

										}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_Itaipu)) {

									}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								}//else {

							}//else if (a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) == TipoDetalhamentoProducaoHidreletrica_por_usina) {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro UH: \n" + std::string(erro.what())); }

					}//if (menemonico == "IT") {

					if (menemonico == "IA") {//Limite de fluxo entre subsistemas 

						try {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Campo 2 -  Identificação do estágio.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_intercambio = getIdEstagioFromChar(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Mnemônico de identificação do subsistema I (conforme campo do 3 do registro SB).  
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const string mnemonico_submercado_origem = atributo;

							//Identifica se algum intercambio tem sido inicializada com submercado_origem
							const IdSubmercado idSubmercado_origem_inicializado = getIdSubmercadoFromMnemonico(mnemonico_submercado_origem);

							IdSubmercado idSubmercado_origem;

							if (idSubmercado_origem_inicializado != IdSubmercado_Nenhum)
								idSubmercado_origem = idSubmercado_origem_inicializado;
							else
								throw std::invalid_argument("Registro IA - Submercado nao encontrado com menemonico_" + mnemonico_submercado_origem);


							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 4 -  Mnemônico de identificação do subsistema J (conforme campo do 3 do registro SB).  
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(14, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const string mnemonico_submercado_destino = atributo;

							//Identifica se algum intercambio tem sido inicializada com submercado_origem
							const IdSubmercado idSubmercado_destino_inicializado = getIdSubmercadoFromMnemonico(mnemonico_submercado_destino);

							IdSubmercado idSubmercado_destino;

							if (idSubmercado_destino_inicializado != IdSubmercado_Nenhum)
								idSubmercado_destino = idSubmercado_destino_inicializado;
							else
								throw std::invalid_argument("Registro IA - Submercado nao encontrado com menemonico_" + mnemonico_submercado_destino);

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 5 -  Flag indicador de penalidade: flag=0 (default) indica uso de penalidade;  (flag=1) indica penalidade nula 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(17, 1);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							int flag_penalidade = 0; //Default - Uso de penalidade

							if (atributo == "1")
								flag_penalidade = 1;

							////////////////////////////////////////

							int numero_patamares = 0;

							//Vetores com informação por patamar
							std::vector<double> intercambio_capacidade_maxima_submercado_origem;
							std::vector<double> intercambio_capacidade_maxima_submercado_destino;

							const int line_size = int(line.length());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campos 6 - 7
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							if (line_size >= 39) {

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 6 - Capacidade máxima de transporte do subsistema  I para o subsistema J em MWmed no patamar 1. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(19, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								intercambio_capacidade_maxima_submercado_origem.push_back(atof(atributo.c_str()));

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 7 - Capacidade máxima de transporte do subsistema  J para o subsistema I em MWmed no patamar 1.  
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(29, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								intercambio_capacidade_maxima_submercado_destino.push_back(atof(atributo.c_str()));

							}//if (line_size >= 49) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campos 8 - 9
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							if (line_size >= 59) {

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 8 - Capacidade máxima de transporte do subsistema  I para o subsistema J em MWmed no patamar 2. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(39, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								intercambio_capacidade_maxima_submercado_origem.push_back(atof(atributo.c_str()));

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 9 - Capacidade máxima de transporte do subsistema  J para o subsistema I em MWmed no patamar 2.  
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(49, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								intercambio_capacidade_maxima_submercado_destino.push_back(atof(atributo.c_str()));

							}//if (line_size >= 59) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campos 10 - 11
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							if (line_size >= 79) {

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 10 - Capacidade máxima de transporte do subsistema  I para o subsistema J em MWmed no patamar 3. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(59, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								intercambio_capacidade_maxima_submercado_origem.push_back(atof(atributo.c_str()));

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 11 - Capacidade máxima de transporte do subsistema  J para o subsistema I em MWmed no patamar 3.  
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(69, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								intercambio_capacidade_maxima_submercado_destino.push_back(atof(atributo.c_str()));

							}//if (line_size >= 79) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campos 12 - 13
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							if (line_size >= 99) {

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 12 - Capacidade máxima de transporte do subsistema  I para o subsistema J em MWmed no patamar 4. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(79, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								intercambio_capacidade_maxima_submercado_origem.push_back(atof(atributo.c_str()));

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 13 - Capacidade máxima de transporte do subsistema  J para o subsistema I em MWmed no patamar 4.  
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(89, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								intercambio_capacidade_maxima_submercado_destino.push_back(atof(atributo.c_str()));

							}//if (line_size >= 99) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campos 14 - 15
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							if (line_size >= 119) {

								numero_patamares++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 14 - Capacidade máxima de transporte do subsistema  I para o subsistema J em MWmed no patamar 5. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(99, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								intercambio_capacidade_maxima_submercado_origem.push_back(atof(atributo.c_str()));

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 15 - Capacidade máxima de transporte do subsistema  J para o subsistema I em MWmed no patamar 5.  
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(109, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								intercambio_capacidade_maxima_submercado_destino.push_back(atof(atributo.c_str()));

							}//if (line_size >= 119) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Guarda informação nos Smart Elementos
							//Nota: //Cada linha lida tem dois intercâmbios: (i) DE->PARA (ii) PARA->DE
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							IdIntercambio idIntercambio;

							if (idEstagio_intercambio == IdEstagio_1) {

								//////////////////////////////////////////////////////////////////////////
								//Identifica se o Intercambio está inicializado
								//////////////////////////////////////////////////////////////////////////

								std::vector<IdIntercambio> idIntercambio_inicializado = a_dados.vetorIntercambio.getIdObjetos(AttComumIntercambio_submercado_origem, idSubmercado_origem);

								int idIntercambio_inicializado_size = int(idIntercambio_inicializado.size());

								idIntercambio = IdIntercambio_Nenhum;

								for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

									if (a_dados.getAtributo(idIntercambio_inicializado.at(intercambio_inicializado), AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_destino) {
										idIntercambio = idIntercambio_inicializado.at(intercambio_inicializado);
										break;
									}//if (a_dados.getAtributo(idIntercambio_inicializado.at(intercambio_inicializado), AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_destino) {

								}//for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

								//////////////////////////////////////////////////////////////////////////

								if (idIntercambio == IdIntercambio_Nenhum) {//Inicializa um novo intercambio

									Intercambio intercambio;

									//////////////////////////////
									//Intercâmbio 1
									//////////////////////////////

									idIntercambio = a_dados.getMaiorId(IdIntercambio());

									if (idIntercambio == IdIntercambio_Nenhum)
										idIntercambio = IdIntercambio_1;
									else
										idIntercambio++;

									intercambio.setAtributo(AttComumIntercambio_idIntercambio, idIntercambio);
									intercambio.setAtributo(AttComumIntercambio_submercado_origem, idSubmercado_origem);
									intercambio.setAtributo(AttComumIntercambio_submercado_destino, idSubmercado_destino);
									intercambio.setAtributo(AttComumIntercambio_nome, mnemonico_submercado_origem + "->" + mnemonico_submercado_destino);

									const SmartEnupla<IdPatamarCarga, double> vetor_intercambio_capacidade_maxima_submercado_origem(IdPatamarCarga_1, intercambio_capacidade_maxima_submercado_origem);
									const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_intercambio_capacidade_maxima_submercado_origem(horizonte_estudo, vetor_intercambio_capacidade_maxima_submercado_origem);

									intercambio.setMatriz(AttMatrizIntercambio_potencia_maxima, matriz_intercambio_capacidade_maxima_submercado_origem);

									a_dados.vetorIntercambio.add(intercambio);

									//////////////////////////////
									//Intercâmbio 2
									//////////////////////////////

									idIntercambio++;
									intercambio.setAtributo(AttComumIntercambio_idIntercambio, idIntercambio);
									intercambio.setAtributo(AttComumIntercambio_submercado_origem, idSubmercado_destino); //No intercâmbio 2, o destino é o origen
									intercambio.setAtributo(AttComumIntercambio_submercado_destino, idSubmercado_origem); //No intercâmbio 2, o origem é o destino
									intercambio.setAtributo(AttComumIntercambio_nome, mnemonico_submercado_destino + "->" + mnemonico_submercado_origem);

									const SmartEnupla<IdPatamarCarga, double> vetor_intercambio_capacidade_maxima_submercado_destino(IdPatamarCarga_1, intercambio_capacidade_maxima_submercado_destino);
									const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_intercambio_capacidade_maxima_submercado_destino(horizonte_estudo, vetor_intercambio_capacidade_maxima_submercado_destino);

									intercambio.setMatriz(AttMatrizIntercambio_potencia_maxima, matriz_intercambio_capacidade_maxima_submercado_destino);

									a_dados.vetorIntercambio.add(intercambio);

								}//if (idIntercambio == IdIntercambio_Nenhum) {
								else {//Alguns intercambios referentes a IVAPORÃ e ANDE são inicializados de antemão

									const SmartEnupla<IdPatamarCarga, double> vetor_intercambio_capacidade_maxima_submercado_origem(IdPatamarCarga_1, intercambio_capacidade_maxima_submercado_origem);
									const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_intercambio_capacidade_maxima_submercado_origem(horizonte_estudo, vetor_intercambio_capacidade_maxima_submercado_origem);

									a_dados.vetorIntercambio.att(idIntercambio).setMatriz(AttMatrizIntercambio_potencia_maxima, matriz_intercambio_capacidade_maxima_submercado_origem);

									////////////////////////////////////////////////
									//Determina o idIntercambio do Intercâmbio 2
									////////////////////////////////////////////////

									std::vector<IdIntercambio> idIntercambio_inicializado = a_dados.vetorIntercambio.getIdObjetos(AttComumIntercambio_submercado_origem, idSubmercado_destino);

									int idIntercambio_inicializado_size = int(idIntercambio_inicializado.size());

									idIntercambio = IdIntercambio_Nenhum;

									for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

										if (a_dados.getAtributo(idIntercambio_inicializado.at(intercambio_inicializado), AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_origem) {
											idIntercambio = idIntercambio_inicializado.at(intercambio_inicializado);
											break;
										}//if (a_dados.getAtributo(idIntercambio_inicializado.at(intercambio_inicializado), AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_destino) {

									}//for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

									if (idIntercambio == IdIntercambio_Nenhum)
										throw std::invalid_argument("Registro IA - Intercambio nao encontrado com idSubmercado_origem_" + getString(idSubmercado_destino) + " e idSubmercado_destino_" + getString(idSubmercado_origem));

									const SmartEnupla<IdPatamarCarga, double> vetor_intercambio_capacidade_maxima_submercado_destino(IdPatamarCarga_1, intercambio_capacidade_maxima_submercado_destino);
									const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_intercambio_capacidade_maxima_submercado_destino(horizonte_estudo, vetor_intercambio_capacidade_maxima_submercado_destino);

									a_dados.vetorIntercambio.att(idIntercambio).setMatriz(AttMatrizIntercambio_potencia_maxima, matriz_intercambio_capacidade_maxima_submercado_destino);

								}//else {

							}//if (idEstagio_intercambio == IdEstagio_1) {
							else {

								//////////////////////////////
								//Intercâmbio 1
								//////////////////////////////

								std::vector<IdIntercambio> idIntercambio_inicializado = a_dados.vetorIntercambio.getIdObjetos(AttComumIntercambio_submercado_origem, idSubmercado_origem);

								int idIntercambio_inicializado_size = int(idIntercambio_inicializado.size());

								for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

									idIntercambio = idIntercambio_inicializado.at(intercambio_inicializado);

									if (a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_destino)
										break;

								}//for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									if (periodo >= horizonte_otimizacao_DC.at(idEstagio_intercambio)) {

										for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

											a_dados.vetorIntercambio.att(idIntercambio).setElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, intercambio_capacidade_maxima_submercado_origem.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));

										}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

									}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_intercambio)) {

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								//////////////////////////////
								//Intercâmbio 2
								//////////////////////////////

								idIntercambio_inicializado = a_dados.vetorIntercambio.getIdObjetos(AttComumIntercambio_submercado_origem, idSubmercado_destino); //No intercâmbio 2, o destino é o origen

								idIntercambio_inicializado_size = int(idIntercambio_inicializado.size());

								for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

									idIntercambio = idIntercambio_inicializado.at(intercambio_inicializado);

									if (a_dados.vetorIntercambio.at(idIntercambio).getAtributo(AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_origem)
										break;

								}//for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									if (periodo >= horizonte_otimizacao_DC.at(idEstagio_intercambio)) {

										for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

											a_dados.vetorIntercambio.att(idIntercambio).setElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, intercambio_capacidade_maxima_submercado_destino.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));

										}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

									}//if (horizonte_estudo.at(periodo) >= idEstagio_termeletrica) {

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							}//else {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro IA: \n" + std::string(erro.what())); }

					}//if (menemonico == "IA") {

					if (menemonico == "TX") {

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Taxa nominal de desconto anual em percentagem (default = 10.0) 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 5);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							double taxa_desconto_anual;

							if (atributo == "")
								taxa_desconto_anual = 10; //Default
							else
								taxa_desconto_anual = atof(atributo.c_str()) / 100;

							if (!dadosPreConfig_instanciados)
								a_dados.setAtributo(AttComumDados_taxa_desconto_anual, taxa_desconto_anual);

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro TX: \n" + std::string(erro.what())); }

					}//if (menemonico == "TX") {

					if (menemonico == "PE") {//Penalidades

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Campo 2 -  Índice de identificação do subsistema.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdSubmercado idSubmercado = getIdSubmercadoFromChar(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(8, 1);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							if (atributo == "1") {//Se o valor informado for igual a 0, a penalidade informada no campo 4 será para vertimento. 
								//Vertimento

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 4
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(9, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const double valor = atof(atributo.c_str());

								for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica++) {
									if (a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.numObjetos() > 0) {
										if (a_dados.getAtributo(idHidreletrica, IdConjuntoHidraulico_1, AttComumConjuntoHidraulico_submercado, IdSubmercado()) == idSubmercado)
											a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_penalidade_vertimento, valor);
									}
								}

							}//if (atributo == "1") {
							else if (atributo == "2") {//Se o valor informado for igual a 1, a penalidade informada no campo 4 será o valor base para todos os intercâmbios e a informação descrita do campo 2 não será considerada
								//Intercambios

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 4
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(9, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const double valor = atof(atributo.c_str());

								for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= a_dados.getMaiorId(IdIntercambio()); idIntercambio++)
									a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_penalidade_intercambio, valor);

							}//else if (atributo == "2") {
							else if (atributo == "3") {//Se o valor informado for igual a 2, a penalidade informada no campo 4 será o valor base para todos os desvios de água e a informação descrita do campo 2 não será considerada. 
								//Desvio água

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 4
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(9, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const double valor = atof(atributo.c_str());

								for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica++) {
									if (a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.numObjetos() > 0) {
										if (a_dados.getAtributo(idHidreletrica, IdConjuntoHidraulico_1, AttComumConjuntoHidraulico_submercado, IdSubmercado()) == idSubmercado)
											a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_penalidade_desvio_agua, valor);
									}
								}

							}//else if (atributo == "3") {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro PE: \n" + std::string(erro.what())); }

					}//if (menemonico == "PE") {

					if (menemonico == "GP") {//TOLERANCIA PARA CONVERGENCIA

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Taxa nominal de desconto anual em percentagem (default = 10.0) 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 10);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							double tolerancia_convergencia;

							if (atributo == "")
								tolerancia_convergencia = 0.00001; //Default
							else
								tolerancia_convergencia = atof(atributo.c_str()) / 100;

							//if (!dadosPreConfig_instanciados)
							//a_dados.setAtributo(AttComumDados_tolerancia_convergencia, tolerancia_convergencia);

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro GP: \n" + std::string(erro.what())); }

					}//if (menemonico == "GP") {

					if (menemonico == "NI") {//TOTAL DE ITERACOES

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Campo 2 -  Número de iterações.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							int numero_iteracoes;

							if (atributo == "")
								numero_iteracoes = 500; //Default
							else
								numero_iteracoes = atoi(atributo.c_str());

							const int line_size = int(line.length());

							bool numero_iteracoes_maxima = true;

							if (line_size >= 9) {

								atributo = line.substr(9, 1);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								if (atoi(atributo.c_str()) == 1)
									numero_iteracoes_maxima = false;

							}//if (line_size >= 9) {

							if (!dadosPreConfig_instanciados) {

								if (numero_iteracoes_maxima == true)
									a_dados.setAtributo(AttComumDados_numero_maximo_iteracoes, numero_iteracoes);
								else if (numero_iteracoes_maxima == false) {
									a_dados.setAtributo(AttComumDados_numero_maximo_iteracoes, 500); //Default
								}//else if (numero_iteracoes_maxima == false) {

							}//if (!dadosPreConfig_instanciados) {


						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro NI: \n" + std::string(erro.what())); }

					}//if (menemonico == "NI") {

					if (menemonico == "DT") {//DATA DE REFERENCIA Do ESTUDO

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Campo 2 -  Dia (default: dia do sistema). 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int dia = atoi(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Mês (default: mês do sistema). 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int mes = atoi(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 4 -  Ano (default: ano do sistema).
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(14, 4);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int ano = atoi(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Verifica respeito à data encontrada no arquivo VAZOES
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							Periodo data_reportada_DADGER(dia, mes, ano);

							if (data_reportada_DADGER != Periodo(TipoPeriodo_diario, horizonte_estudo.getIteradorInicial()))
								throw std::invalid_argument("Registro DT - Data nao coincidente com os estagios gerados a partir do arquivo VAZOES.RVX");

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro DT: \n" + std::string(erro.what())); }

					}//if (menemonico == "DT") {

					if (menemonico == "MP") {//MANUTENCOES PROGRAMADAS HIDRAULICAS

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Número da usina conforme campo 2 dos registros UH. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_usina = atoi(atributo.c_str());

							const IdHidreletrica idHidreletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

							const IdHidreletrica idHidreletrica = idHidreletrica_inicializado;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3-19 -  Fatores de manutenção para os N estágios em p.u. //i.e., 5 colunas/campo
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							//Nota: O deck registra os valores de disponiblidade associados à manutenção. No SPT é ingressado como uma 
							//indisponibilidade (1- disponibilidadeManutenção) e logo nas validações é calculada a disponiblidade em conjunto com a indisponibilidade forçada

							std::vector<double> indisponibilidade_programada;

							const int line_size = int(line.length());

							const int numeroEstagios = horizonte_otimizacao_DC.size();

							for (int estagio = 0; estagio < numeroEstagios; estagio++) {

								if (line_size >= 14 + 5 * estagio) {

									atributo = line.substr(9 + 5 * estagio, 5);
									atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

									if (atributo == "")
										indisponibilidade_programada.push_back(0);//Default
									else
										indisponibilidade_programada.push_back(1 - atof(atributo.c_str()));
								}//if (line_size >= 14 + 5 * estagio) {
								else //Condição que prevee se o último estágio é deixado vazio no DECK
									indisponibilidade_programada.push_back(0);//Default

							}//for (int estagio = 0; estagio < numeroEstagios; estagio++) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Guarda informação nos Smart Elemtentos
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							SmartEnupla<Periodo, double> vetor_indisponibilidade_programada(horizonte_estudo, 1.0); //Valor default

							for (int estagio = 0; estagio < numeroEstagios; estagio++) {

								const IdEstagio idEstagio_manutencao = IdEstagio(estagio + 1);

								const double valor = indisponibilidade_programada.at(estagio);

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									//Atualiza a informação unicamente para o estágio DC indicado

									if (horizonte_otimizacao_DC.getIteradorFinal() == idEstagio_manutencao) {

										//Se é último estágio DC, o periodo deve ser maior ou igual ao estágio DC

										if (periodo >= horizonte_otimizacao_DC.at(idEstagio_manutencao))
											vetor_indisponibilidade_programada.setElemento(periodo, valor);

									}//if (horizonte_otimizacao_DC.getIteradorFinal() == idEstagio_manutencao) {
									else {

										//Se não é último estágio DC, o periodo deve ser entre dois estágios DC consecutivos

										const IdEstagio idEstagio_manutencao_seguinte = IdEstagio(idEstagio_manutencao + 1);

										if (periodo >= horizonte_otimizacao_DC.at(idEstagio_manutencao) && periodo < horizonte_otimizacao_DC.at(idEstagio_manutencao_seguinte))
											vetor_indisponibilidade_programada.setElemento(periodo, valor);

										else if (periodo >= horizonte_otimizacao_DC.at(idEstagio_manutencao_seguinte))
											break; //Evita percorrer todo o horizonte_estudo

									}//else {

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							}//for (int estagio = 0; estagio < numeroEstagios; estagio++) {

							/////////////////////////////////////////////////////////////////////////////
							//Procura se é um registro de um conjunto hidráulico de IT, codigo_usina = 66

							atributo = line.substr(7, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							if (codigo_usina == 66 && atributo != "" && a_dados.vetorHidreletrica.att(idHidreletrica).getSizeVetor(AttVetorHidreletrica_indisponibilidade_programada) > 0) {

								const SmartEnupla<Periodo, double> vetor_indisponibilidade_programada_anterior = a_dados.vetorHidreletrica.att(idHidreletrica).getVetor(AttVetorHidreletrica_indisponibilidade_programada, Periodo(), double());

								const Periodo periodo_inicial = vetor_indisponibilidade_programada_anterior.getIteradorInicial();
								const Periodo periodo_final = vetor_indisponibilidade_programada_anterior.getIteradorFinal();

								for (Periodo periodo = periodo_inicial; periodo <= periodo_final; vetor_indisponibilidade_programada_anterior.incrementarIterador(periodo)) {

									const double indisponibilidade_programada_nova = 0.5 * (vetor_indisponibilidade_programada_anterior.getElemento(periodo) + vetor_indisponibilidade_programada.getElemento(periodo));

									a_dados.vetorHidreletrica.att(idHidreletrica).setElemento(AttVetorHidreletrica_indisponibilidade_programada, periodo, indisponibilidade_programada_nova);

								}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; vetor_indisponibilidade_programada_anterior.incrementarIterador(periodo)) {

							}//if (codigo_usina == 66 && atributo != "" && a_dados.vetorHidreletrica.att(idHidreletrica).getSizeVetor(AttVetorHidreletrica_indisponibilidade_programada) > 0) {
							else
								a_dados.vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_programada, vetor_indisponibilidade_programada);

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro MP: \n" + std::string(erro.what())); }

					}//if (menemonico == "MP") {

					if (menemonico == "MT") {//MANUTENCAO PROGRAMADA DE USINAS TERMICAS

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Campo 2 -  Número da usina conforme campo 2 dos registros CT. 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_usina = atoi(atributo.c_str());

							const IdTermeletrica idTermeletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, codigo_usina);

							if (idTermeletrica_inicializado == IdTermeletrica_Nenhum)
								throw std::invalid_argument("Nao inicializada idTermeletrica com codigo_usina_" + atributo);

							const IdTermeletrica idTermeletrica = idTermeletrica_inicializado;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Índice do subsistema ao qual pertence a usina térmica.  
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int submercado_codigo_usina = atoi(atributo.c_str()); //dado que não se precisa, já foi carregado no reistro CT

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 4-20 -  Fatores de manutenção para os N estágios em p.u. //i.e., 5 colunas/campo
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							//Nota: O deck registra os valores de disponiblidade associados à manutenção. No SPT é ingressado como uma 
							//indisponibilidade (1- disponibilidadeManutenção) e logo nas validações é calculada a disponiblidade em conjunto com a indisponibilidade forçada

							std::vector<double> indisponibilidade_programada;

							const int line_size = int(line.length());

							const int numeroEstagios = horizonte_otimizacao_DC.size();

							for (int estagio = 0; estagio < numeroEstagios; estagio++) {

								if (line_size >= 19 + 5 * estagio) {

									atributo = line.substr(14 + 5 * estagio, 5);
									atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

									if (atributo == "")
										indisponibilidade_programada.push_back(0);//Default
									else
										indisponibilidade_programada.push_back(1 - atof(atributo.c_str()));
								}//if (line_size >= 19 + 5 * estagio) {
								else //Condição que prevee se o último estágio é deixado vazio no DECK
									indisponibilidade_programada.push_back(0);//Default

							}//for (int estagio = 0; estagio < numeroEstagios; estagio++) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Guarda informação nos Smart Elemtentos
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							SmartEnupla<Periodo, double> vetor_indisponibilidade_programada(horizonte_estudo, 1.0); //Valor default

							for (int estagio = 0; estagio < numeroEstagios; estagio++) {

								const IdEstagio idEstagio_manutencao = IdEstagio(estagio + 1);

								const double valor = indisponibilidade_programada.at(estagio);

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									//Atualiza a informação unicamente para o estágio DC indicado

									if (horizonte_otimizacao_DC.getIteradorFinal() == idEstagio_manutencao) {

										//Se é último estágio DC, o periodo deve ser maior ou igual ao estágio DC

										if (periodo >= horizonte_otimizacao_DC.at(idEstagio_manutencao))
											vetor_indisponibilidade_programada.setElemento(periodo, valor);

									}//if (horizonte_otimizacao_DC.getIteradorFinal() == idEstagio_manutencao) {
									else {

										//Se não é último estágio DC, o periodo deve ser entre dois estágios DC consecutivos

										const IdEstagio idEstagio_manutencao_seguinte = IdEstagio(idEstagio_manutencao + 1);

										if (periodo >= horizonte_otimizacao_DC.at(idEstagio_manutencao) && periodo < horizonte_otimizacao_DC.at(idEstagio_manutencao_seguinte))
											vetor_indisponibilidade_programada.setElemento(periodo, valor);

										else if (periodo >= horizonte_otimizacao_DC.at(idEstagio_manutencao_seguinte))
											break; //Evita percorrer todo o horizonte_estudo

									}//else {

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							}//for (int estagio = 0; estagio < numeroEstagios; estagio++) {

							a_dados.vetorTermeletrica.att(idTermeletrica).setVetor(AttVetorTermeletrica_indisponibilidade_programada, vetor_indisponibilidade_programada);

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro MT: \n" + std::string(erro.what())); }

					}//if (menemonico == "MT") {

					if (menemonico == "FD") {//FATOR DE DISPONIBILIDADE DE USINAS HIDRAULICAS

						try {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Número da usina conforme campo 2 dos registros UH. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_usina = atoi(atributo.c_str());

							const IdHidreletrica idHidreletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

							const IdHidreletrica idHidreletrica = idHidreletrica_inicializado;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3-19 -  Fatores de manutenção para os N estágios em p.u. //i.e., 5 colunas/campo
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							std::vector<double> indisponibilidade_forcada;

							const int line_size = int(line.length());

							const int numeroEstagios = horizonte_otimizacao_DC.size();

							for (int estagio = 0; estagio < numeroEstagios; estagio++) {

								if (line_size >= 14 + 5 * estagio) {

									atributo = line.substr(9 + 5 * estagio, 5);
									atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

									if (atributo == "")
										indisponibilidade_forcada.push_back(0);//Default
									else
										indisponibilidade_forcada.push_back(1 - atof(atributo.c_str()));
								}//if (line_size >= 14 + 5 * estagio) {
								else //Condição que prevee se o último estágio é deixado vazio no DECK
									indisponibilidade_forcada.push_back(0);//Default

							}//for (int estagio = 0; estagio < numeroEstagios; estagio++) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Guarda informação nos Smart Elemtentos
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							SmartEnupla<Periodo, double> vetor_indisponibilidade_forcada(horizonte_estudo, 1.0); //Valor default

							for (int estagio = 0; estagio < numeroEstagios; estagio++) {

								const IdEstagio idEstagio_manutencao = IdEstagio(estagio + 1);

								const double valor = indisponibilidade_forcada.at(estagio);

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									//Atualiza a informação unicamente para o estágio DC indicado

									if (horizonte_otimizacao_DC.getIteradorFinal() == idEstagio_manutencao) {

										//Se é último estágio DC, o periodo deve ser maior ou igual ao estágio DC

										if (periodo >= horizonte_otimizacao_DC.at(idEstagio_manutencao))
											vetor_indisponibilidade_forcada.setElemento(periodo, valor);

									}//if (horizonte_otimizacao_DC.getIteradorFinal() == idEstagio_manutencao) {
									else {

										//Se não é último estágio DC, o periodo deve ser entre dois estágios DC consecutivos

										const IdEstagio idEstagio_manutencao_seguinte = IdEstagio(idEstagio_manutencao + 1);

										if (periodo >= horizonte_otimizacao_DC.at(idEstagio_manutencao) && periodo < horizonte_otimizacao_DC.at(idEstagio_manutencao_seguinte))
											vetor_indisponibilidade_forcada.setElemento(periodo, valor);

										else if (periodo >= horizonte_otimizacao_DC.at(idEstagio_manutencao_seguinte))
											break; //Evita percorrer todo o horizonte_estudo

									}//else {

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							}//for (int estagio = 0; estagio < numeroEstagios; estagio++) {


							/////////////////////////////////////////////////////////////////////////////
							//Procura se é um registro de um conjunto hidráulico de IT, codigo_usina = 66

							atributo = line.substr(7, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							if (codigo_usina == 66 && atributo != "" && a_dados.vetorHidreletrica.att(idHidreletrica).getSizeVetor(AttVetorHidreletrica_indisponibilidade_forcada) > 0) {

								const SmartEnupla<Periodo, double> vetor_indisponibilidade_forcada_anterior = a_dados.vetorHidreletrica.att(idHidreletrica).getVetor(AttVetorHidreletrica_indisponibilidade_forcada, Periodo(), double());

								const Periodo periodo_inicial = vetor_indisponibilidade_forcada_anterior.getIteradorInicial();
								const Periodo periodo_final = vetor_indisponibilidade_forcada_anterior.getIteradorFinal();

								for (Periodo periodo = periodo_inicial; periodo <= periodo_final; vetor_indisponibilidade_forcada_anterior.incrementarIterador(periodo)) {

									const double indisponibilidade_forcada_nova = 0.5 * (vetor_indisponibilidade_forcada_anterior.getElemento(periodo) + vetor_indisponibilidade_forcada.getElemento(periodo));

									a_dados.vetorHidreletrica.att(idHidreletrica).setElemento(AttVetorHidreletrica_indisponibilidade_forcada, periodo, indisponibilidade_forcada_nova);

								}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; vetor_indisponibilidade_forcada_anterior.incrementarIterador(periodo)) {

							}//if (codigo_usina == 66 && atributo != "" && a_dados.vetorHidreletrica.att(idHidreletrica).getSizeVetor(AttVetorHidreletrica_indisponibilidade_forcada) > 0) {
							else
								a_dados.vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_forcada, vetor_indisponibilidade_forcada);

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro FD: \n" + std::string(erro.what())); }

					}//if (menemonico == "FD") {

					if (menemonico == "VE") {//VOLUME DE ESPERA

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Campo 2 -  Número da usina hidráulica com reservatório conforme campo 2 dos registros UH
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_usina = atoi(atributo.c_str());

							const IdHidreletrica idHidreletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

							const IdHidreletrica idHidreletrica = idHidreletrica_inicializado;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3-19 -  Volume de espera em percentagem do volume útil para os N estágios.//i.e., 5 colunas/campo
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							std::vector<double> percentual_volume_util_maximo;

							const int line_size = int(line.length());

							const int numeroEstagios = horizonte_otimizacao_DC.size();

							for (int estagio = 0; estagio < numeroEstagios; estagio++) {

								if (line_size >= 14 + 5 * estagio) {

									atributo = line.substr(9 + 5 * estagio, 5);
									atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

									percentual_volume_util_maximo.push_back(atof(atributo.c_str()) / 100);

								}//if (line_size >= 14 + 5 * estagio) {

							}//for (int estagio = 0; estagio < numeroEstagios; estagio++) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Guarda informação nos Smart Elemtentos
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							SmartEnupla<Periodo, double> vetor_percentual_volume_util_maximo(horizonte_estudo, 1.0); //Valor default

							for (int estagio = 0; estagio < numeroEstagios; estagio++) {

								const IdEstagio idEstagio_volume_espera = IdEstagio(estagio + 1);

								const double valor = percentual_volume_util_maximo.at(estagio);

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									//Atualiza a informação unicamente para o estágio DC indicado

									if (horizonte_otimizacao_DC.getIteradorFinal() == idEstagio_volume_espera) {

										//Se é último estágio DC, o periodo deve ser maior ou igual ao estágio DC

										if (periodo >= horizonte_otimizacao_DC.at(idEstagio_volume_espera))
											vetor_percentual_volume_util_maximo.setElemento(periodo, valor);

									}//if (horizonte_otimizacao_DC.getIteradorFinal() == idEstagio_volume_espera) {
									else {

										//Se não é último estágio DC, o periodo deve ser entre dois estágios DC consecutivos

										const IdEstagio idEstagio_volume_espera_seguinte = IdEstagio(idEstagio_volume_espera + 1);

										if (periodo >= horizonte_otimizacao_DC.at(idEstagio_volume_espera) && periodo < horizonte_otimizacao_DC.at(idEstagio_volume_espera_seguinte))
											vetor_percentual_volume_util_maximo.setElemento(periodo, valor);

										else if (periodo >= horizonte_otimizacao_DC.at(idEstagio_volume_espera_seguinte))
											break; //Evita percorrer todo o horizonte_estudo

									}//else {

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							}//for (int estagio = 0; estagio < numeroEstagios; estagio++) {

							a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_percentual_volume_util_maximo, vetor_percentual_volume_util_maximo);

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro VE: \n" + std::string(erro.what())); }

					}//if (menemonico == "VE") {

					if (menemonico == "VM") {//DADOS DE ENCHIMENTO DE VOLUME MORTO - TAXA DE ENCHIMENTO

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Campo 2 -  Número da usina hidráulica com reservatório conforme campo 2 dos registros UH
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_usina = atoi(atributo.c_str());

							const IdHidreletrica idHidreletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

							const IdHidreletrica idHidreletrica = idHidreletrica_inicializado;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3-19 -  Taxa de enchimento de volume morto em m3/s para cada estágio do estudo até a entrada em operação ou até o estágio final.//i.e., 5 colunas/campo
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////						

							const int line_size = int(line.length());

							const int numeroEstagios_DC = horizonte_otimizacao_DC.size();

							SmartEnupla<IdEstagio, double> enchimento_minimo_volume_morto;

							for (int estagio_DC = 0; estagio_DC < numeroEstagios_DC; estagio_DC++) {

								if (line_size >= 14 + 5 * estagio_DC) {

									atributo = line.substr(9 + 5 * estagio_DC, 5);
									atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

									enchimento_minimo_volume_morto.addElemento(IdEstagio(estagio_DC + 1), atof(atributo.c_str()) / 100);

								}//if (line_size >= 14 + 5 * estagio_DC) {

							}//for (int estagio_DC = 0; estagio_DC < numeroEstagios_DC; estagio_DC++) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Guarda informação nos Smart Elemtentos
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							SmartEnupla<Periodo, double> vetor_enchimento_minimo_volume_morto(horizonte_estudo, 0);

							for (int estagio_DC = 0; estagio_DC < numeroEstagios_DC; estagio_DC++) {

								const IdEstagio idEstagio_enchimento_minimo_volume_morto = IdEstagio(estagio_DC + 1);

								const double valor = enchimento_minimo_volume_morto.getElemento(idEstagio_enchimento_minimo_volume_morto);

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									//Atualiza a informação unicamente para o estágio DC indicado

									if (horizonte_otimizacao_DC.getIteradorFinal() == idEstagio_enchimento_minimo_volume_morto) {

										//Se é último estágio DC, o periodo deve ser maior ou igual ao estágio DC

										if (periodo >= horizonte_otimizacao_DC.at(idEstagio_enchimento_minimo_volume_morto))
											vetor_enchimento_minimo_volume_morto.setElemento(periodo, valor);

									}//if (horizonte_otimizacao_DC.getIteradorFinal() == idEstagio_enchimento_minimo_volume_morto) {
									else {

										//Se não é último estágio DC, o periodo deve ser entre dois estágios DC consecutivos

										const IdEstagio idEstagio_enchimento_minimo_volume_morto_seguinte = IdEstagio(idEstagio_enchimento_minimo_volume_morto + 1);

										if (periodo >= horizonte_otimizacao_DC.at(idEstagio_enchimento_minimo_volume_morto) && periodo < horizonte_otimizacao_DC.at(idEstagio_enchimento_minimo_volume_morto_seguinte))
											vetor_enchimento_minimo_volume_morto.setElemento(periodo, valor);

										else if (periodo >= horizonte_otimizacao_DC.at(idEstagio_enchimento_minimo_volume_morto_seguinte))
											break; //Evita percorrer todo o horizonte_estudo

									}//else {

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							}//for (int estagio_DC = 0; estagio_DC < numeroEstagios_DC; estagio_DC++) {

							a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_taxa_enchimento_volume_morto, vetor_enchimento_minimo_volume_morto);

							lista_hidreletrica_sem_capacidade.setElemento(idHidreletrica, true);

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro VM: \n" + std::string(erro.what())); }

					}//if (menemonico == "VM") {

					if (menemonico == "DF") {// Registro com taxa de defluência 

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Número da usina hidráulica conforme campo 2 dos registros UH
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_usina = atoi(atributo.c_str());

							const IdHidreletrica idHidreletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

							const IdHidreletrica idHidreletrica = idHidreletrica_inicializado;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3-19 -  Taxa de enchimento de volume morto em m3/s para cada estágio do estudo até a entrada em operação ou até o estágio final.//i.e., 5 colunas/campo
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							const int line_size = int(line.length());

							const int numeroEstagios_DC = horizonte_otimizacao_DC.size();
							SmartEnupla<IdEstagio, double> defluencia_volume_morto;

							for (int estagio_DC = 0; estagio_DC < numeroEstagios_DC; estagio_DC++) {

								if (line_size >= 14 + 5 * estagio_DC) {

									atributo = line.substr(9 + 5 * estagio_DC, 5);
									atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

									defluencia_volume_morto.addElemento(IdEstagio(estagio_DC + 1), atof(atributo.c_str()) / 100);

								}//if (line_size >= 14 + 5 * estagio_DC) {

							}//for (int estagio_DC = 0; estagio_DC < numeroEstagios_DC; estagio_DC++) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Guarda informação nos Smart Elementos
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							//Filosofia: Se o valor de defluência enchendo o volume morto é maior do que a defluência indicada no campo "UH", a defluência mínima é atualizada 

							for (int estagio_DC = 0; estagio_DC < numeroEstagios_DC; estagio_DC++) {

								const IdEstagio idEstagio_defluencia_volume_morto = IdEstagio(estagio_DC + 1);

								const double valor = defluencia_volume_morto.getElemento(idEstagio_defluencia_volume_morto);

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									//Atualiza a informação unicamente para o estágio DC indicado

									if (horizonte_otimizacao_DC.getIteradorFinal() == idEstagio_defluencia_volume_morto) {

										//Se é último estágio DC, o periodo deve ser maior ou igual ao estágio DC

										if (periodo >= horizonte_otimizacao_DC.at(idEstagio_defluencia_volume_morto)) {

											if (a_dados.vetorHidreletrica.att(idHidreletrica).getElementoVetor(AttVetorHidreletrica_vazao_defluente_minima, periodo, double()) < valor)
												a_dados.vetorHidreletrica.att(idHidreletrica).setElemento(AttVetorHidreletrica_vazao_defluente_minima, periodo, valor);

										}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_defluencia_volume_morto)) {

									}//if (horizonte_otimizacao_DC.getIteradorFinal() == idEstagio_defluencia_volume_morto) {
									else {

										//Se não é último estágio DC, o periodo deve ser entre dois estágios DC consecutivos

										const IdEstagio idEstagio_defluencia_volume_morto_seguinte = IdEstagio(idEstagio_defluencia_volume_morto + 1);

										if (periodo >= horizonte_otimizacao_DC.at(idEstagio_defluencia_volume_morto) && periodo < horizonte_otimizacao_DC.at(idEstagio_defluencia_volume_morto_seguinte)) {

											if (a_dados.vetorHidreletrica.att(idHidreletrica).getElementoVetor(AttVetorHidreletrica_vazao_defluente_minima, periodo, double()) < valor)
												a_dados.vetorHidreletrica.att(idHidreletrica).setElemento(AttVetorHidreletrica_vazao_defluente_minima, periodo, valor);

										}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_defluencia_volume_morto) && periodo < horizonte_otimizacao_DC.at(idEstagio_defluencia_volume_morto_seguinte)) {

										else if (periodo >= horizonte_otimizacao_DC.at(idEstagio_defluencia_volume_morto_seguinte))
											break; //Evita percorrer todo o horizonte_estudo

									}//else {

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							}//for (int estagio_DC = 0; estagio_DC < numeroEstagios_DC; estagio_DC++) {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro DF: \n" + std::string(erro.what())); }

					}//if (menemonico == "DF") {

					if (menemonico == "RE") {//Identificação da restrição

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Número de identificação da restrição elétrica
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 4);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const string nome_restricao_eletrica = atributo;
							const int codigo_restricao_eletrica = atoi(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Estágio inicial da restrição. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_inicial = getIdEstagioFromChar(atributo.c_str());

							Periodo periodo_inicio;

							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {
									periodo_inicio = periodo;
									break;
								}//if (idEstagio_inicial == horizonte_estudo.at(periodo)) {

							}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 4 -  Estágio final da restrição.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(14, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_final = getIdEstagioFromChar(atributo.c_str());

							const IdEstagio idEstagio_final_horizonte_otimizacao_DC = horizonte_otimizacao_DC.getIteradorFinal();

							Periodo periodo_final;

							if (idEstagio_final == idEstagio_final_horizonte_otimizacao_DC) {

								periodo_final = horizonte_estudo.getIteradorFinal();

							}//if (idEstagio_final == idEstagio_final_horizonte_otimizacao_DC) {
							else {//Se não é o último estágio, precisa encontrar o último periodo referente ao idEstagio_final

								Periodo periodo = horizonte_estudo.getIteradorInicial();

								const IdEstagio idEstagio_final_seguinte = IdEstagio(idEstagio_final + 1);

								while (horizonte_otimizacao_DC.at(idEstagio_final_seguinte) > periodo) {

									horizonte_estudo.incrementarIterador(periodo);

								}//while (horizonte_otimizacao_DC.at(idEstagio_final) > periodo) {

								horizonte_estudo.decrementarIterador(periodo);

								periodo_final = periodo;

							}//else {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Guarda informação nos Smart Elementos
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							RestricaoEletrica restricaoEletrica;

							const IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica(a_dados.getMaiorId(IdRestricaoEletrica()) + 1);

							restricaoEletrica.setAtributo(AttComumRestricaoEletrica_idRestricaoEletrica, idRestricaoEletrica);
							restricaoEletrica.setAtributo(AttComumRestricaoEletrica_nome, nome_restricao_eletrica);

							lista_codigo_ONS_restricao_eletrica.addElemento(idRestricaoEletrica, codigo_restricao_eletrica);

							lista_restricao_eletrica_periodo_inicial.addElemento(idRestricaoEletrica, periodo_inicio);
							lista_restricao_eletrica_periodo_final.addElemento(idRestricaoEletrica, periodo_final);

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Carrega restrição
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							a_dados.vetorRestricaoEletrica.add(restricaoEletrica);

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Inicializa matrizes minimo e maximo
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//O numero_patamares não é informado no registro especificado, porém, da leitura de outros registros é possível obtê-lo

							//std::vector<double> potencia_minima(atoi(getString(maiorIdPatamarCarga).c_str()), 0.0);
							//std::vector<double> potencia_maxima(atoi(getString(maiorIdPatamarCarga).c_str()), 1e21);

							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

									a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).addElemento(AttMatrizRestricaoEletrica_potencia_minima, periodo, idPatamarCarga, 0.0);
									a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).addElemento(AttMatrizRestricaoEletrica_potencia_maxima, periodo, idPatamarCarga, getdoubleFromChar("max"));

								} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
							} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro RE: \n" + std::string(erro.what())); }

					}//if (menemonico == "RE") {

					if (menemonico == "LU") {//Limites da restrição 

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Campo 2 -  Número da restrição elétrica conforme campo 2 do registro RE.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 4);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_usina = atoi(atributo.c_str());

							const IdRestricaoEletrica idRestricaoEletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_restricao_eletrica, codigo_usina);

							if (idRestricaoEletrica_inicializado == IdRestricaoEletrica_Nenhum)
								throw std::invalid_argument("Nao inicializada idRestricaoEletrica  com codigo_usina_" + atributo);

							const IdRestricaoEletrica  idRestricaoEletrica = idRestricaoEletrica_inicializado;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Número do estágio, em ordem crescente.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_inicial = getIdEstagioFromChar(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Determina o periodo inicial e periodo final
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							Periodo periodo_inicio;

							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {
									periodo_inicio = periodo;
									break;
								}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {

							}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							lista_restricao_eletrica_periodo_inicial.setElemento(idRestricaoEletrica, periodo_inicio);

							const Periodo periodo_final = lista_restricao_eletrica_periodo_final.getElemento(idRestricaoEletrica);

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Filosofia: Atualiza se encontrar um valor 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							int numero_patamar = 0;

							IdPatamarCarga idPatamarCarga;

							std::string limiteInferior;
							std::string limiteSuperior;

							const int line_size = int(line.length());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Patamar 1
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							limiteInferior = "";
							limiteSuperior = "";

							if (line_size >= 24) {

								numero_patamar++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 4 - Limite inferior em MWmed para o patamar 1. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								limiteInferior = line.substr(14, 10);
								limiteInferior.erase(std::remove(limiteInferior.begin(), limiteInferior.end(), ' '), limiteInferior.end());

							}//if (line_size >= 24) {

							if (line_size >= 34) {

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 5 - Limite superior em MWmed para o patamar 1. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								limiteSuperior = line.substr(24, 10);
								limiteSuperior.erase(std::remove(limiteSuperior.begin(), limiteSuperior.end(), ' '), limiteSuperior.end());

							}//if (line_size >= 34) {


							if (limiteInferior != "" || limiteSuperior != "") {

								idPatamarCarga = getIdPatamarCargaFromChar(std::to_string(numero_patamar).c_str());

								for (Periodo periodo = periodo_inicio; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {
									if (limiteInferior != "") { a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).setElemento(AttMatrizRestricaoEletrica_potencia_minima, periodo, idPatamarCarga, std::atof(limiteInferior.c_str())); }
									if (limiteSuperior != "") { a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).setElemento(AttMatrizRestricaoEletrica_potencia_maxima, periodo, idPatamarCarga, std::atof(limiteSuperior.c_str())); }
								}//for (Periodo periodo = periodo_inicio; periodo <= periodo_final; horizonte_estudo.at(periodo)) {

							}//if (limiteInferior != "" || limiteSuperior != "") {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Patamar 2
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							limiteInferior = "";
							limiteSuperior = "";

							if (line_size >= 44) {

								numero_patamar++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 6 - Limite inferior em MWmed para o patamar 2. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								limiteInferior = line.substr(34, 10);
								limiteInferior.erase(std::remove(limiteInferior.begin(), limiteInferior.end(), ' '), limiteInferior.end());

							}//if (line_size >= 44) {

							if (line_size >= 54) {

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 7 - Limite superior em MWmed para o patamar 2. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								limiteSuperior = line.substr(44, 10);
								limiteSuperior.erase(std::remove(limiteSuperior.begin(), limiteSuperior.end(), ' '), limiteSuperior.end());

							}//if (line_size >= 54) {


							if (limiteInferior != "" || limiteSuperior != "") {

								idPatamarCarga = getIdPatamarCargaFromChar(std::to_string(numero_patamar).c_str());

								for (Periodo periodo = periodo_inicio; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {
									if (limiteInferior != "") { a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).setElemento(AttMatrizRestricaoEletrica_potencia_minima, periodo, idPatamarCarga, std::atof(limiteInferior.c_str())); }
									if (limiteSuperior != "") { a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).setElemento(AttMatrizRestricaoEletrica_potencia_maxima, periodo, idPatamarCarga, std::atof(limiteSuperior.c_str())); }
								}//for (Periodo periodo = periodo_inicio; periodo <= periodo_final; horizonte_estudo.at(periodo)) {

							}//if (limiteInferior != "" || limiteSuperior != "") {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Patamar 3
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							limiteInferior = "";
							limiteSuperior = "";

							if (line_size >= 64) {

								numero_patamar++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 8 - Limite inferior em MWmed para o patamar 3. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								limiteInferior = line.substr(54, 10);
								limiteInferior.erase(std::remove(limiteInferior.begin(), limiteInferior.end(), ' '), limiteInferior.end());

							}//if (line_size >= 64) {

							if (line_size >= 74) {

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 9 - Limite superior em MWmed para o patamar 3. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								limiteSuperior = line.substr(64, 10);
								limiteSuperior.erase(std::remove(limiteSuperior.begin(), limiteSuperior.end(), ' '), limiteSuperior.end());

							}//if (line_size >= 74) {


							if (limiteInferior != "" || limiteSuperior != "") {

								idPatamarCarga = getIdPatamarCargaFromChar(std::to_string(numero_patamar).c_str());

								for (Periodo periodo = periodo_inicio; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {
									if (limiteInferior != "") { a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).setElemento(AttMatrizRestricaoEletrica_potencia_minima, periodo, idPatamarCarga, std::atof(limiteInferior.c_str())); }
									if (limiteSuperior != "") { a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).setElemento(AttMatrizRestricaoEletrica_potencia_maxima, periodo, idPatamarCarga, std::atof(limiteSuperior.c_str())); }
								}//for (Periodo periodo = periodo_inicio; periodo <= periodo_final; horizonte_estudo.at(periodo)) {

							}//if (limiteInferior != "" || limiteSuperior != "") {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Patamar 4
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							limiteInferior = "";
							limiteSuperior = "";

							if (line_size >= 84) {

								numero_patamar++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 10 - Limite inferior em MWmed para o patamar 4. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								limiteInferior = line.substr(74, 10);
								limiteInferior.erase(std::remove(limiteInferior.begin(), limiteInferior.end(), ' '), limiteInferior.end());

							}//if (line_size >= 84) {

							if (line_size >= 94) {

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 11 - Limite superior em MWmed para o patamar 4. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								limiteSuperior = line.substr(84, 10);
								limiteSuperior.erase(std::remove(limiteSuperior.begin(), limiteSuperior.end(), ' '), limiteSuperior.end());

							}//if (line_size >= 94) {

							if (limiteInferior != "" || limiteSuperior != "") {

								idPatamarCarga = getIdPatamarCargaFromChar(std::to_string(numero_patamar).c_str());

								for (Periodo periodo = periodo_inicio; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {
									if (limiteInferior != "") { a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).setElemento(AttMatrizRestricaoEletrica_potencia_minima, periodo, idPatamarCarga, std::atof(limiteInferior.c_str())); }
									if (limiteSuperior != "") { a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).setElemento(AttMatrizRestricaoEletrica_potencia_maxima, periodo, idPatamarCarga, std::atof(limiteSuperior.c_str())); }
								}//for (Periodo periodo = periodo_inicio; periodo <= periodo_final; horizonte_estudo.at(periodo)) {

							}//if (limiteInferior != "" || limiteSuperior != "") {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Patamar 5
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							limiteInferior = "";
							limiteSuperior = "";

							if (line_size >= 104) {

								numero_patamar++;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 12 - Limite inferior em MWmed para o patamar 5. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								limiteInferior = line.substr(94, 10);
								limiteInferior.erase(std::remove(limiteInferior.begin(), limiteInferior.end(), ' '), limiteInferior.end());

							}//if (line_size >= 104) {

							if (line_size >= 114) {

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 13 - Limite superior em MWmed para o patamar 5. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								limiteSuperior = line.substr(104, 10);
								limiteSuperior.erase(std::remove(limiteSuperior.begin(), limiteSuperior.end(), ' '), limiteSuperior.end());

							}//if (line_size >= 114) {

							if (limiteInferior != "" || limiteSuperior != "") {

								idPatamarCarga = getIdPatamarCargaFromChar(std::to_string(numero_patamar).c_str());

								for (Periodo periodo = periodo_inicio; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {
									if (limiteInferior != "") { a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).setElemento(AttMatrizRestricaoEletrica_potencia_minima, periodo, idPatamarCarga, std::atof(limiteInferior.c_str())); }
									if (limiteSuperior != "") { a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).setElemento(AttMatrizRestricaoEletrica_potencia_maxima, periodo, idPatamarCarga, std::atof(limiteSuperior.c_str())); }
								}//for (Periodo periodo = periodo_inicio; periodo <= periodo_final; horizonte_estudo.at(periodo)) {

							}//if (limiteInferior != "" || limiteSuperior != "") {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro LU: \n" + std::string(erro.what())); }

					}//if (menemonico == "LU") {

					if (menemonico == "FU") {//Coeficientes das usinas hidráulicas na restrição

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Número da restrição elétrica conforme campo 2 do registro RE.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 4);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int restricao_codigo_usina = atoi(atributo.c_str());

							const IdRestricaoEletrica idRestricaoEletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_restricao_eletrica, restricao_codigo_usina);

							if (idRestricaoEletrica_inicializado == IdRestricaoEletrica_Nenhum)
								throw std::invalid_argument("Nao inicializada idRestricaoEletrica  com codigo_usina_" + atributo);

							const IdRestricaoEletrica  idRestricaoEletrica = idRestricaoEletrica_inicializado;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Número do estágio, em ordem crescente.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_inicial = getIdEstagioFromChar(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 4 -  Número da usina hidráulica conforme campo 2 dos registros UH.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(14, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int hidreletrica_codigo_usina = atoi(atributo.c_str());

							const IdHidreletrica idHidreletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, hidreletrica_codigo_usina);

							const IdHidreletrica idHidreletrica = idHidreletrica_inicializado;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 5 -  Fator de participação da usina.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(19, 10);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double fator_participacao = atof(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo Adicional -  Identificador do conjunto Itaipu (codigo_usina = 66)
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							IdIntercambio idIntercambio = IdIntercambio_Nenhum;

							if (line.size() >= 32 && hidreletrica_codigo_usina == 66) {

								atributo = line.substr(30, 2);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								//Na representaçao por usina de Itaipú os limites do intercambio Itaipú->Ande e Itaipú->IV sao atualizados

								const IdSubmercado idSubmercado_itaipu = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_submercado_ITAIPU);
								const IdSubmercado idSubmercado_ande = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_ANDE);
								const IdSubmercado idSubmercado_iv = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_IV);

								std::vector<IdIntercambio> idIntercambio_inicializado = a_dados.vetorIntercambio.getIdObjetos(AttComumIntercambio_submercado_origem, idSubmercado_itaipu);
								const int idIntercambio_inicializado_size = int(idIntercambio_inicializado.size());

								if (atributo == "50") {//Carrega restriçao para os limites de intercambio ITAIPU->ANDE

									for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

										if (a_dados.getAtributo(idIntercambio_inicializado.at(intercambio_inicializado), AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_ande) {
											idIntercambio = idIntercambio_inicializado.at(intercambio_inicializado);
											break;
										}//if (a_dados.getAtributo(idIntercambio_inicializado.at(intercambio_inicializado), AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_destino) {

									}//for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

									if (idIntercambio == IdIntercambio_Nenhum)
										throw std::invalid_argument("Registro FU - Intercambio nao encontrado");

								}//if (atributo == "50") {
								else if (atributo == "60") {//Carrega restriçao para os limites de intercambio ITAIPU->IV

									for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

										if (a_dados.getAtributo(idIntercambio_inicializado.at(intercambio_inicializado), AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_iv) {
											idIntercambio = idIntercambio_inicializado.at(intercambio_inicializado);
											break;
										}//if (a_dados.getAtributo(idIntercambio_inicializado.at(intercambio_inicializado), AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_destino) {

									}//for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

									if (idIntercambio == IdIntercambio_Nenhum)
										throw std::invalid_argument("Registro RI - Intercambio nao encontrado");

								}//else if (atributo == "60") {

							}//if (line.size() >= 32 && hidreletrica_codigo_usina == 66) {


							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Guarda informação nos Smart Elementos
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							if (idIntercambio == IdIntercambio_Nenhum) {//Significa que a restriçao elétrica é referente à idHidreletrica

								//Verifica se o elemento da restrição já foi inicializado

								const std::vector<IdElementoSistema> idElementoSistema_inicializada = a_dados.vetorRestricaoEletrica.at(idRestricaoEletrica).vetorElementoSistema.getIdObjetos(AttComumElementoSistema_hidreletrica, idHidreletrica);

								if (int(idElementoSistema_inicializada.size()) == 0) {//Ainda não foi inicializado

									IdElementoSistema idElementoSistema = a_dados.getMaiorId(idRestricaoEletrica, IdElementoSistema());

									if (idElementoSistema == IdElementoSistema_Nenhum) { idElementoSistema = IdElementoSistema_1; }
									else { idElementoSistema++; }

									ElementoSistema elementoSistema;
									elementoSistema.setAtributo(AttComumElementoSistema_idElementoSistema, idElementoSistema);
									elementoSistema.setAtributo(AttComumElementoSistema_tipo_elemento, TipoElementoSistema_hidreletrica);
									elementoSistema.setAtributo(AttComumElementoSistema_hidreletrica, idHidreletrica);

									SmartEnupla<Periodo, double> vetor_fator_participacao(horizonte_estudo, 0);

									const Periodo periodo_inicial = lista_restricao_eletrica_periodo_inicial.getElemento(idRestricaoEletrica);
									const Periodo periodo_final = lista_restricao_eletrica_periodo_final.getElemento(idRestricaoEletrica);

									for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

										if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial))
											vetor_fator_participacao.setElemento(periodo, fator_participacao);

									}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									elementoSistema.setVetor(AttVetorElementoSistema_fator_participacao, vetor_fator_participacao);

									a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.add(elementoSistema);
								}//if (int(idElementoSistema_inicializada.size()) == 0) {
								else {

									const Periodo periodo_inicial = lista_restricao_eletrica_periodo_inicial.getElemento(idRestricaoEletrica);
									const Periodo periodo_final = lista_restricao_eletrica_periodo_final.getElemento(idRestricaoEletrica);

									for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

										if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial))
											a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.att(idElementoSistema_inicializada.at(0)).setElemento(AttVetorElementoSistema_fator_participacao, periodo, fator_participacao);

									}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								}//else {

							}//if (idIntercambio == IdIntercambio_Nenhum) {
							else {

								//As restriçoes elétricas referentes aos conjuntos 50/60 Hz Itaipu sao incluidos nos
								//intercambios ITAIPU->ANDE e ITAIPU->IV respectivamente

								//Verifica se o elemento da restrição já foi inicializado

								const std::vector<IdElementoSistema> idElementoSistema_inicializada = a_dados.vetorRestricaoEletrica.at(idRestricaoEletrica).vetorElementoSistema.getIdObjetos(AttComumElementoSistema_intercambio, idIntercambio);

								if (int(idElementoSistema_inicializada.size()) == 0) {//Ainda não foi inicializado

									IdElementoSistema idElementoSistema = a_dados.getMaiorId(idRestricaoEletrica, IdElementoSistema());

									if (idElementoSistema == IdElementoSistema_Nenhum) { idElementoSistema = IdElementoSistema_1; }
									else { idElementoSistema++; }

									ElementoSistema elementoSistema;
									elementoSistema.setAtributo(AttComumElementoSistema_idElementoSistema, idElementoSistema);
									elementoSistema.setAtributo(AttComumElementoSistema_tipo_elemento, TipoElementoSistema_intercambio);
									elementoSistema.setAtributo(AttComumElementoSistema_intercambio, idIntercambio);

									SmartEnupla<Periodo, double> vetor_fator_participacao(horizonte_estudo, 0);

									const Periodo periodo_inicial = lista_restricao_eletrica_periodo_inicial.getElemento(idRestricaoEletrica);
									const Periodo periodo_final = lista_restricao_eletrica_periodo_final.getElemento(idRestricaoEletrica);

									for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

										if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial))
											vetor_fator_participacao.setElemento(periodo, fator_participacao);

									}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									elementoSistema.setVetor(AttVetorElementoSistema_fator_participacao, vetor_fator_participacao);

									a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.add(elementoSistema);
								}//if (int(idElementoSistema_inicializada.size()) == 0) {
								else {

									const Periodo periodo_inicial = lista_restricao_eletrica_periodo_inicial.getElemento(idRestricaoEletrica);
									const Periodo periodo_final = lista_restricao_eletrica_periodo_final.getElemento(idRestricaoEletrica);

									for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

										if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial))
											a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.att(idElementoSistema_inicializada.at(0)).setElemento(AttVetorElementoSistema_fator_participacao, periodo, fator_participacao);

									}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								}//else {

							}//else {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro FU: \n" + std::string(erro.what())); }

					}//if (menemonico == "FU") {

					if (menemonico == "FT") {

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Campo 2 -  Número de identificação da restrição elétrica conforme campo 2 do registro RE. 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 4);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int restricao_codigo_usina = atoi(atributo.c_str());

							const IdRestricaoEletrica idRestricaoEletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_restricao_eletrica, restricao_codigo_usina);

							if (idRestricaoEletrica_inicializado == IdRestricaoEletrica_Nenhum)
								throw std::invalid_argument("Nao inicializada idRestricaoEletrica  com codigo_usina_" + atributo);

							const IdRestricaoEletrica  idRestricaoEletrica = idRestricaoEletrica_inicializado;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Número do estágio, em ordem crescente.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_inicial = getIdEstagioFromChar(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 4 -  Número da usina térmica conforme campo 2 dos registros CT ou TG do arquivo DADGNL. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(14, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int termeletrica_codigo_usina = atoi(atributo.c_str());

							const IdTermeletrica idTermeletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, termeletrica_codigo_usina);

							if (idTermeletrica_inicializado == IdTermeletrica_Nenhum)
								throw std::invalid_argument("Nao inicializada idTermeletrica com codigo_usina_" + atributo);

							const IdTermeletrica idTermeletrica = idTermeletrica_inicializado;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 5 -  Número do subsistema conforme campo 3 dos registros CT ou TG do arquivo DADGNL.  
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(19, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int submercado_codigo_usina = atoi(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 6 -  Fator de participação da usina.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(24, 10);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double fator_participacao = atof(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Guarda informação nos Smart Elementos
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							//Verifica se o elemento da restrição já foi inicializado

							const std::vector<IdElementoSistema> idElementoSistema_inicializada = a_dados.vetorRestricaoEletrica.at(idRestricaoEletrica).vetorElementoSistema.getIdObjetos(AttComumElementoSistema_termeletrica, idTermeletrica);

							if (int(idElementoSistema_inicializada.size()) == 0) {//Ainda não foi inicializado

								IdElementoSistema idElementoSistema = a_dados.getMaiorId(idRestricaoEletrica, IdElementoSistema());

								if (idElementoSistema == IdElementoSistema_Nenhum) { idElementoSistema = IdElementoSistema_1; }
								else { idElementoSistema++; }

								ElementoSistema elementoSistema;
								elementoSistema.setAtributo(AttComumElementoSistema_idElementoSistema, idElementoSistema);
								elementoSistema.setAtributo(AttComumElementoSistema_tipo_elemento, TipoElementoSistema_termeletrica);
								elementoSistema.setAtributo(AttComumElementoSistema_termeletrica, idTermeletrica);

								SmartEnupla<Periodo, double> vetor_fator_participacao(horizonte_estudo, 0);

								const Periodo periodo_inicial = lista_restricao_eletrica_periodo_inicial.getElemento(idRestricaoEletrica);
								const Periodo periodo_final = lista_restricao_eletrica_periodo_final.getElemento(idRestricaoEletrica);

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial))
										vetor_fator_participacao.setElemento(periodo, fator_participacao);

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								elementoSistema.setVetor(AttVetorElementoSistema_fator_participacao, vetor_fator_participacao);

								a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.add(elementoSistema);
							}//if (int(idElementoSistema_inicializada.size()) == 0) {
							else {

								const Periodo periodo_inicial = lista_restricao_eletrica_periodo_inicial.getElemento(idRestricaoEletrica);
								const Periodo periodo_final = lista_restricao_eletrica_periodo_final.getElemento(idRestricaoEletrica);

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial))
										a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.att(idElementoSistema_inicializada.at(0)).setElemento(AttVetorElementoSistema_fator_participacao, periodo, fator_participacao);

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							}//else {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro FT: \n" + std::string(erro.what())); }

					}//if (menemonico == "FT") {

					if (menemonico == "FI") {//Coeficientes das interligações na restrição

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Campo 2 -  Número de identificação da restrição elétrica conforme campo 2 do registro RE. 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 4);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int restricao_codigo_usina = atoi(atributo.c_str());

							const IdRestricaoEletrica idRestricaoEletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_restricao_eletrica, restricao_codigo_usina);

							if (idRestricaoEletrica_inicializado == IdRestricaoEletrica_Nenhum)
								throw std::invalid_argument("Nao inicializada idRestricaoEletrica  com codigo_usina_" + atributo);

							const IdRestricaoEletrica  idRestricaoEletrica = idRestricaoEletrica_inicializado;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Número do estágio, em ordem crescente.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_inicial = getIdEstagioFromChar(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 4 -  Mnemônico do subsistema “DE” conforme campo 2 dos registros SB 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(14, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const string submercado_origem_mnemonico = atributo;

							const IdSubmercado idSubmercado_origem_inicializada = getIdSubmercadoFromMnemonico(submercado_origem_mnemonico);

							const IdSubmercado idSubmercado_origem = idSubmercado_origem_inicializada;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 5 -  Mnemônico do subsistema “PARA” conforme campo 2 dos registros SB. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(19, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const string submercado_destino_mnemonico = atributo;

							const IdSubmercado idSubmercado_destino_inicializada = getIdSubmercadoFromMnemonico(submercado_destino_mnemonico);

							const IdSubmercado idSubmercado_destino = idSubmercado_destino_inicializada;

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
							//Campo 6 -  Fator de participação da usina.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(24, 10);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double fator_participacao = atof(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Guarda informação nos Smart Elementos
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							//Verifica se o elemento da restrição já foi inicializado

							const std::vector<IdElementoSistema> idElementoSistema_inicializada = a_dados.vetorRestricaoEletrica.at(idRestricaoEletrica).vetorElementoSistema.getIdObjetos(AttComumElementoSistema_intercambio, idIntercambio);

							if (int(idElementoSistema_inicializada.size()) == 0) {//Ainda não foi inicializado

								IdElementoSistema idElementoSistema = a_dados.getMaiorId(idRestricaoEletrica, IdElementoSistema());

								if (idElementoSistema == IdElementoSistema_Nenhum) { idElementoSistema = IdElementoSistema_1; }
								else { idElementoSistema++; }

								ElementoSistema elementoSistema;
								elementoSistema.setAtributo(AttComumElementoSistema_idElementoSistema, idElementoSistema);
								elementoSistema.setAtributo(AttComumElementoSistema_tipo_elemento, TipoElementoSistema_intercambio);
								elementoSistema.setAtributo(AttComumElementoSistema_intercambio, idIntercambio);

								SmartEnupla<Periodo, double> vetor_fator_participacao(horizonte_estudo, 0);

								const Periodo periodo_inicial = lista_restricao_eletrica_periodo_inicial.getElemento(idRestricaoEletrica);
								const Periodo periodo_final = lista_restricao_eletrica_periodo_final.getElemento(idRestricaoEletrica);

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial))
										vetor_fator_participacao.setElemento(periodo, fator_participacao);

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								elementoSistema.setVetor(AttVetorElementoSistema_fator_participacao, vetor_fator_participacao);

								a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.add(elementoSistema);

							}//if (int(idElementoSistema_inicializada.size()) == 0) {
							else {

								const Periodo periodo_inicial = lista_restricao_eletrica_periodo_inicial.getElemento(idRestricaoEletrica);
								const Periodo periodo_final = lista_restricao_eletrica_periodo_final.getElemento(idRestricaoEletrica);

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial))
										a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.att(idElementoSistema_inicializada.at(0)).setElemento(AttVetorElementoSistema_fator_participacao, periodo, fator_participacao);

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							}//else {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro FI: \n" + std::string(erro.what())); }

					}//if (menemonico == "FI") {

					if (menemonico == "FE") {//Coeficientes dos contratos de exportação e importação na restrição 

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Campo 2 -  Número de identificação da restrição elétrica conforme campo 2 do registro RE. 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 4);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int restricao_codigo_usina = atoi(atributo.c_str());

							const IdRestricaoEletrica idRestricaoEletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_restricao_eletrica, restricao_codigo_usina);

							if (idRestricaoEletrica_inicializado == IdRestricaoEletrica_Nenhum)
								throw std::invalid_argument("Nao inicializada idRestricaoEletrica  com codigo_usina_" + atributo);

							const IdRestricaoEletrica  idRestricaoEletrica = idRestricaoEletrica_inicializado;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Número do estágio, em ordem crescente.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_inicial = getIdEstagioFromChar(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 4 -  Número do contrato de importação/exportação conforme o campo 2 do registro CI/CE.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(14, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int contrato_codigo_usina = atoi(atributo.c_str());

							const IdContrato idContrato_inicializado = getIdFromCodigoONS(lista_codigo_ONS_contrato, contrato_codigo_usina);

							if (idContrato_inicializado == IdContrato_Nenhum)
								throw std::invalid_argument("Nao inicializada idContrato com codigo_usina_" + atributo);

							const IdContrato  idContrato = idContrato_inicializado;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 5 -  Número do submercado conforme o campo 3 do registro SB 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(19, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int submercado_codigo_usina = atoi(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 6 -  Fator de participação da usina.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(24, 10);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double fator_participacao = atof(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Guarda informação nos Smart Elementos
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							//Verifica se o elemento da restrição já foi inicializado

							const std::vector<IdElementoSistema> idElementoSistema_inicializada = a_dados.vetorRestricaoEletrica.at(idRestricaoEletrica).vetorElementoSistema.getIdObjetos(AttComumElementoSistema_contrato, idContrato);

							if (int(idElementoSistema_inicializada.size()) == 0) {//Ainda não foi inicializado

								IdElementoSistema idElementoSistema = a_dados.getMaiorId(idRestricaoEletrica, IdElementoSistema());

								if (idElementoSistema == IdElementoSistema_Nenhum) { idElementoSistema = IdElementoSistema_1; }
								else { idElementoSistema++; }

								ElementoSistema elementoSistema;
								elementoSistema.setAtributo(AttComumElementoSistema_idElementoSistema, idElementoSistema);
								elementoSistema.setAtributo(AttComumElementoSistema_tipo_elemento, TipoElementoSistema_contrato);
								elementoSistema.setAtributo(AttComumElementoSistema_contrato, idContrato);

								SmartEnupla<Periodo, double> vetor_fator_participacao(horizonte_estudo, 0);

								const Periodo periodo_inicial = lista_restricao_eletrica_periodo_inicial.getElemento(idRestricaoEletrica);
								const Periodo periodo_final = lista_restricao_eletrica_periodo_final.getElemento(idRestricaoEletrica);

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial))
										vetor_fator_participacao.setElemento(periodo, fator_participacao);

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								elementoSistema.setVetor(AttVetorElementoSistema_fator_participacao, vetor_fator_participacao);

								a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.add(elementoSistema);

							}//if (int(idElementoSistema_inicializada.size()) == 0) {
							else {

								const Periodo periodo_inicial = lista_restricao_eletrica_periodo_inicial.getElemento(idRestricaoEletrica);
								const Periodo periodo_final = lista_restricao_eletrica_periodo_final.getElemento(idRestricaoEletrica);

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial))
										a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.att(idElementoSistema_inicializada.at(0)).setElemento(AttVetorElementoSistema_fator_participacao, periodo, fator_participacao);

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							}//else {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro FE: \n" + std::string(erro.what())); }

					}//if (menemonico == "FE") {

					if (menemonico == "VI") {//Tempo de viagem

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Campo 2 -  Número da usina hidraúlica (conforme campo 2 dos registros UH) cuja vazão defluente atrase mais que 24 horas para o aproveitamento de jusante 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int hidreletrica_codigo_usina = atoi(atributo.c_str());

							const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, hidreletrica_codigo_usina);

							if (idHidreletrica == IdHidreletrica_Nenhum)
								throw std::invalid_argument("Registro VI - idHidreletrica nao instanciada com codigo_" + getString(hidreletrica_codigo_usina));

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Tempo de viagem, em horas
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(9, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int tempo_viagem_agua = atoi(atributo.c_str());

							a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_tempo_viagem_agua, tempo_viagem_agua);

							if (!a_dados.vetorHidreletrica.att(idHidreletrica).vetorDefluencia.isInstanciado(IdDefluencia_passada)) {//As defluências podem ser inicializadas desde a pre-configuração

								//Nota: A defluencia passada vai considerar periodos_diarios de 15 dias anteriores ao horizonte de estudo (15 dias máximo tempo de viagem d'água registrado)
								const SmartEnupla<Periodo, bool> horizonte_defluencia_passada(Periodo(TipoPeriodo_diario, horizonte_estudo.getIteradorInicial()) - 15, std::vector<bool>(15, true));

								Defluencia defluencia;
								defluencia.setAtributo(AttComumDefluencia_idDefluencia, IdDefluencia_passada);
								defluencia.setAtributo(AttComumDefluencia_tipo_elemento_jusante, TipoElementoJusante_usina);

								a_dados.vetorHidreletrica.att(idHidreletrica).vetorDefluencia.add(defluencia);

								a_dados.vetorHidreletrica.att(idHidreletrica).vetorDefluencia.att(IdDefluencia_passada).setVetor(AttVetorDefluencia_vazao_defluencia, SmartEnupla<Periodo, double>(horizonte_defluencia_passada, 0.0));

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 4-8 -  Vazão defluente nas semanas do mês anterior ao início do estudo, em m3/s. Estes valores devem ser 
								//             ordenados da última para a primeira semana do mês anterior. Para casos onde o primeiro período é mensal, 
								//             o valor informado deve ser a vazão do mês anterior ao inicio do estudo, em m3/s. 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								const int line_size = int(line.length());

								int registro = 0;

								std::vector<double> vazao_defluencia;

								//////////////////////////////////////////////////////

								while (true) {

									if (line_size >= 19 + 5 * registro) {

										atributo = line.substr(14 + 5 * registro, 5);
										atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

										if (atributo == "")
											break;

										vazao_defluencia.push_back(atof(atributo.c_str()));

									}//if (line_size >= 19 + 5 * registro) {
									else
										break;

									registro++;

								}//while (true) {

								//////////////////////////////////////////////////////
								//Identifica o periodo_semanal inicial do mês anterior
								//////////////////////////////////////////////////////

								Periodo periodo_semanal = horizonte_otimizacao_DC.getElemento(IdEstagio_1);

								const int numero_semanas_anteriores = int(vazao_defluencia.size());

								for (int semana_anterior = 0; semana_anterior < numero_semanas_anteriores; semana_anterior++)
									periodo_semanal--;


								const Periodo periodo_defluencia_passada_inicial = a_dados.vetorHidreletrica.att(idHidreletrica).vetorDefluencia.att(IdDefluencia_passada).getIteradorInicial(AttVetorDefluencia_vazao_defluencia, Periodo());

								Periodo periodo_defluencia_passada = periodo_defluencia_passada_inicial;

								for (int semana_anterior = 0; semana_anterior < numero_semanas_anteriores; semana_anterior++) {

									while (periodo_defluencia_passada.sobreposicao(periodo_semanal)) {

										a_dados.vetorHidreletrica.att(idHidreletrica).vetorDefluencia.att(IdDefluencia_passada).setElemento(AttVetorDefluencia_vazao_defluencia, periodo_defluencia_passada, vazao_defluencia.at(numero_semanas_anteriores - semana_anterior - 1));
										periodo_defluencia_passada++;

									}//while (periodo_defluencia_passada.sobreposicao(periodo_semanal)) {

									periodo_semanal++;

								}//for (int semana_anterior = 0; semana_anterior < numero_semanas_anteriores; semana_anterior) {

							}//if (!a_dados.vetorHidreletrica.att(idHidreletrica).vetorDefluencia.isInstanciado(IdDefluencia_passada)) {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro VI: \n" + std::string(erro.what())); }

					}//if (menemonico == "VI") {

					if (menemonico == "AC") {//Modificação de cadastro 

						try {
							const int line_size = int(line.length());

							if (line_size > 15) {//Foram detectados registros sem nenhum valor de modificação (Erro do DECK), Até a coluna 15 é o mnemônico da modificação

								//Atributo para tstar que tem algum valor logo do mnemônico
								atributo = line.substr(15, line_size - 15);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								if (atributo != "") {

									/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
									//Campo 2 -  Número da usina hidráulica conforme campo 2 dos registros UH
									/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
									const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, std::stoi(line.substr(4, 3)));

									if (idHidreletrica != IdHidreletrica_Nenhum) {

										// Inicializa adicao de modificacao na hidreletrica
										if (lista_modificacaoUHE.size() - 1 < idHidreletrica) {

											IdHidreletrica menorIdHidreletrica = IdHidreletrica_1;

											if (lista_modificacaoUHE.size() > 2)
												menorIdHidreletrica = IdHidreletrica(lista_modificacaoUHE.size() - 1);

											for (IdHidreletrica idHidreletrica_aux = menorIdHidreletrica; idHidreletrica_aux <= idHidreletrica; idHidreletrica_aux++)
												lista_modificacaoUHE.push_back(std::vector< ModificacaoUHE>());

										} // if (lista_modificacaoUHE.size() - 1 < idHidreletrica) {


										/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
										//Campo 3 -  Mnemônico para identificação do parâmetro a ser modificado.
										/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

										atributo = line.substr(9, 6);
										atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

										const string mnemonico = atributo;

										ModificacaoUHE modificacaoUHE;

										if (strCompara(mnemonico, "NOMEUH")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_NOMEUH;
											modificacaoUHE.valor_0 = line.substr(19, 12);
										}

										else if (strCompara(mnemonico, "NUMPOS")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_NUMPOS;
											modificacaoUHE.valor_1 = std::stoi(line.substr(19, 5));
										}

										else if (strCompara(mnemonico, "NUMJUS")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_NUMJUS;
											modificacaoUHE.valor_1 = std::stoi(line.substr(19, 5));
										}

										else if (strCompara(mnemonico, "DESVIO")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_DESVIO;
											modificacaoUHE.valor_1 = std::stod(line.substr(19, 5));

											if (line.substr(33, 1) != " ")
												modificacaoUHE.valor_2 = std::stod(line.substr(24, 10));
											else
												modificacaoUHE.valor_2 = 99999;
										}

										else if (strCompara(mnemonico, "VOLMIN")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_VOLMIN;
											modificacaoUHE.valor_1 = std::stod(line.substr(19, 10));
										}

										else if (strCompara(mnemonico, "VOLMAX")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_VOLMAX;
											modificacaoUHE.valor_1 = std::stod(line.substr(19, 10));
										}

										else if (strCompara(mnemonico, "COTVOL")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_COTVOL;
											modificacaoUHE.valor_1 = std::stod(line.substr(19, 5)); //Índice da cota_volume
											modificacaoUHE.valor_2 = std::stod(line.substr(33, 6)); //Valor do parâmetro da polinômio cota_volume

											modificacaoUHE.tipo_de_grandeza = TipoGrandezaModificacao_absoluta;
										}

										else if (strCompara(mnemonico, "COTARE")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_COTARE;
											modificacaoUHE.valor_1 = std::stod(line.substr(19, 5)); //Índice da cota_area
											modificacaoUHE.valor_2 = std::stod(line.substr(33, 6)); //Valor do parâmetro da polinômio cota_area

											modificacaoUHE.tipo_de_grandeza = TipoGrandezaModificacao_absoluta;
										}

										else if (strCompara(mnemonico, "COFEVA")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_COEFEVAP;
											modificacaoUHE.valor_2 = std::stod(line.substr(19, 5));
											modificacaoUHE.valor_1 = std::stod(line.substr(24, 5));
										}

										else if (strCompara(mnemonico, "NUMCON")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_NUMCNJ;
											modificacaoUHE.valor_1 = std::stod(line.substr(19, 5));

										}//else if (strCompara(mnemonico, "NUMCON")) {

										else if (strCompara(mnemonico, "NUMMAQ")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_NUMMAQ;
											modificacaoUHE.valor_2 = std::stod(line.substr(19, 5));
											modificacaoUHE.valor_1 = std::stoi(line.substr(24, 5));

										}//else if (strCompara(mnemonico, "NUMMAQ")) {

										else if (strCompara(mnemonico, "POTEFE")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_POTEFE;
											modificacaoUHE.valor_2 = std::stod(line.substr(19, 5));//No DC o valor_1 e valor_2 desta modificação é diferente ao NW
											modificacaoUHE.valor_1 = std::stod(line.substr(24, 10));
										}

										else if (strCompara(mnemonico, "ALTEFE")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_ALTEFE;
											modificacaoUHE.valor_1 = std::stod(line.substr(19, 5));
											modificacaoUHE.valor_2 = std::stod(line.substr(24, 10));
										}

										else if (strCompara(mnemonico, "VAZEFE")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_VAZEFE;
											modificacaoUHE.valor_1 = std::stod(line.substr(19, 5));
											modificacaoUHE.valor_2 = std::stod(line.substr(24, 5));
										}

										else if (strCompara(mnemonico, "PROESP")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_PRODESP;
											modificacaoUHE.valor_1 = std::stod(line.substr(19, 10));
										}

										else if (strCompara(mnemonico, "PERHID")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_PERDHIDR;
											modificacaoUHE.valor_1 = std::stod(line.substr(19, 10));
										}

										else if (strCompara(mnemonico, "NCHAVE")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_NCHAVE;
											modificacaoUHE.valor_1 = std::stod(line.substr(19, 5));
											modificacaoUHE.valor_2 = std::stod(line.substr(24, 10));
										}

										else if (strCompara(mnemonico, "COTVAZ")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_COTVAZ;
											modificacaoUHE.valor_1 = std::stod(line.substr(19, 5));
											modificacaoUHE.valor_2 = std::stod(line.substr(24, 5));
											modificacaoUHE.valor_3 = std::stod(line.substr(29, 15));



										}

										else if (strCompara(mnemonico, "JUSMED")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_CFUGA;
											modificacaoUHE.tipo_de_grandeza = TipoGrandezaModificacao_absoluta;
											modificacaoUHE.valor_1 = std::stod(line.substr(19, 10));
										}

										else if (strCompara(mnemonico, "VERTJU")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_VERTJU;
											modificacaoUHE.valor_1 = std::stod(line.substr(19, 5));
										}

										else if (strCompara(mnemonico, "VAZMIN")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_VAZMINHISTORICA;
											modificacaoUHE.valor_1 = std::stod(line.substr(19, 5));
										}

										else if (strCompara(mnemonico, "NUMBAS")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_NUMBAS;
											modificacaoUHE.valor_1 = std::stod(line.substr(19, 5));
										}

										else if (strCompara(mnemonico, "TIPTUR")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_TIPTUR;
											modificacaoUHE.valor_1 = std::stod(line.substr(19, 5));
										}

										else if (strCompara(mnemonico, "TIPERH")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_TIPERH;
											modificacaoUHE.valor_1 = std::stod(line.substr(19, 5));
										}

										else if (strCompara(mnemonico, "VAZCCF")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_VAZCCF;
											modificacaoUHE.valor_1 = std::stod(line.substr(19, 10));
											modificacaoUHE.valor_2 = std::stod(line.substr(29, 5));
											modificacaoUHE.valor_3 = std::stod(line.substr(34, 10));
										}

										else if (strCompara(mnemonico, "JUSENA")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_JUSENA;
											modificacaoUHE.valor_1 = std::stod(line.substr(19, 5));
										}

										else if (strCompara(mnemonico, "VSVERT")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_VSVERT;
											modificacaoUHE.valor_1 = std::stod(line.substr(19, 10));
										}

										else if (strCompara(mnemonico, "VMDESV")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_VMDESV;
											modificacaoUHE.valor_1 = std::stod(line.substr(19, 10));
										}

										else if (strCompara(mnemonico, "TIPUSI")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_TIPUSI;
											atributo = line.substr(19, 12);
											atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
											strNormalizada(atributo);
											modificacaoUHE.valor_0 = atributo;
										}

										else if (strCompara(mnemonico, "NPOSNW")) {
											modificacaoUHE.tipo_de_modificacao = TipoModificacaoUHE_NPOSNW;
											modificacaoUHE.valor_1 = std::stod(line.substr(19, 5));
										}//else if (strCompara(mnemonico, "NPOSNW")) {						

										/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
										//Campo 5-6-7 - Mês/número da semana/ano
										/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

										if (line_size >= 72) {

											IdEstagio idEstagio_DC = IdEstagio_Nenhum;

											if (line.substr(69, 3) == "  ")
												idEstagio_DC = horizonte_otimizacao_DC.getIteradorInicial();
											else {

												IdMes idMes = IdMes_Nenhum;

												if (line.substr(69, 3) == "JAN")
													idMes = IdMes_1;
												else if (line.substr(69, 3) == "FEV")
													idMes = IdMes_2;
												else if (line.substr(69, 3) == "MAR")
													idMes = IdMes_3;
												else if (line.substr(69, 3) == "ABR")
													idMes = IdMes_4;
												else if (line.substr(69, 3) == "MAI")
													idMes = IdMes_5;
												else if (line.substr(69, 3) == "JUN")
													idMes = IdMes_6;
												else if (line.substr(69, 3) == "JUL")
													idMes = IdMes_7;
												else if (line.substr(69, 3) == "AGO")
													idMes = IdMes_8;
												else if (line.substr(69, 3) == "SET")
													idMes = IdMes_9;
												else if (line.substr(69, 3) == "OUT")
													idMes = IdMes_10;
												else if (line.substr(69, 3) == "NOV")
													idMes = IdMes_11;
												else if (line.substr(69, 3) == "DEZ")
													idMes = IdMes_12;
												else
													throw std::invalid_argument("Mes nao identificado no registro AC");

												if (idMes == horizonte_otimizacao_DC.getElemento(horizonte_otimizacao_DC.getIteradorFinal()).getMes())
													idEstagio_DC = horizonte_otimizacao_DC.getIteradorFinal();
												else {

													atributo = line.substr(74, 1);
													atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

													if (atributo == "")
														throw std::invalid_argument("Semana nao identificada no registro AC");

													const IdEstagio idEstagio_DC_lido = IdEstagio(atoi(atributo.c_str()));

													idEstagio_DC = idEstagio_DC_lido;

												}//else {

											}//else {


											const Periodo periodo_DC = horizonte_otimizacao_DC.getElemento(idEstagio_DC);

											const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();
											const Periodo periodo_final = horizonte_estudo.getIteradorFinal();

											for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

												if (periodo >= periodo_DC) {
													modificacaoUHE.periodo = periodo;
													break;
												}//if (periodo >= periodo_DC) {

											}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

										}//if (line_size >= 72) {
										else
											modificacaoUHE.periodo = horizonte_estudo.getIteradorInicial();//Default

										lista_modificacaoUHE.at(idHidreletrica).push_back(modificacaoUHE);

									} // if (idHidreletrica != IdHidreletrica_Nenhum) {

								} // if (atributo != "") {

							}//if (line_size > 15) {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro AC: \n" + std::string(erro.what())); }

					}//if (menemonico == "AC") {

					if (menemonico == "CI" || menemonico == "CE") {// Contratos de importação/exportação de energia 

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 1 -  Identificação do registro: CI = importação ou CE = exportação
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							TipoContrato tipo_contrato;

							if (menemonico == "CI")
								tipo_contrato = TipoContrato_importacao;
							else if (menemonico == "CE")
								tipo_contrato = TipoContrato_exportacao;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Número do contrato. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_usina = atoi(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Índice do subsistema ao qual pertence o contrato.  
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////					
							atributo = line.substr(8, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, atoi(atributo.c_str()));

							if (idSubmercado == IdSubmercado_Nenhum)
								throw std::invalid_argument("Submercado nao instanciado com codigo_usina_" + atributo);


							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 4 -  Nome do contrato 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////					
							atributo = line.substr(11, 10);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							//const string nome = atributo;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 5 -  Índice do estágio.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(24, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_contrato = getIdEstagioFromChar(atributo.c_str());

							////////////////////////////////////////////////////////////////////////////////////////////
							// Campos 6-15 - Limite Inferior / Limite Superior / Custo / ...Fator Perdas
							////////////////////////////////////////////////////////////////////////////////////////////

							const int line_size = int(line.length());

							int numero_patamares;

							if (line_size >= 129 && line.substr(128, 1) != " ")
								numero_patamares = 5;
							else if (line_size >= 109 && line.substr(108, 1) != " ")
								numero_patamares = 4;
							else if (line_size >= 89 && line.substr(88, 1) != " ")
								numero_patamares = 3;
							else if (line_size >= 69 && line.substr(68, 1) != " ")
								numero_patamares = 2;
							else if (line_size >= 49 && line.substr(48, 1) != " ")
								numero_patamares = 1;


							std::vector<double> limite_inferior;
							std::vector<double> limite_superior;
							std::vector<double> custo;

							for (int pat = 0; pat < numero_patamares; pat++) {

								//Limite inferior de impor./exportação de energia, em MWmed
								atributo = line.substr(29 + 20 * pat, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								limite_inferior.push_back(atof(atributo.c_str()));

								//Limite superior de impor./exportação de energia, em MWmed
								atributo = line.substr(34 + 20 * pat, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								limite_superior.push_back(atof(atributo.c_str()));

								//Custo da energia impor./exportada, em $/MWh
								atributo = line.substr(39 + 20 * pat, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								custo.push_back(atof(atributo.c_str()));

							}//for (int pat = 0; pat < numero_patamares; pat++) {

							//No Deck não aparece esta informação apesar das diretrizes do manual de referência
							double fator_de_perdas;

							if (line_size >= 29 + 20 * numero_patamares) {

								atributo = line.substr(24 + 20 * numero_patamares, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
								fator_de_perdas = atof(atributo.c_str());

							}//if (line_size >= 29 + 5 * numero_patamares) {
							else
								fator_de_perdas = 1; //Default: nos Decks não especificam este valor

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Filosofia, sobreescreve a informação para t >= idEstagio e soma todas as bacias especias guardando um valor total
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Filosofia, sobreescreve a informação para t >= idEstagio
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							if (idEstagio_contrato == IdEstagio_1) { //Inicializa Contrato

								Contrato contrato;

								IdContrato idContrato = a_dados.getMaiorId(IdContrato());

								if (idContrato == IdContrato_Nenhum)
									idContrato = IdContrato_1;
								else
									idContrato++;

								contrato.setAtributo(AttComumContrato_idContrato, idContrato);
								contrato.setAtributo(AttComumContrato_tipo_contrato, tipo_contrato);
								contrato.setAtributo(AttComumContrato_submercado, idSubmercado);
								contrato.setAtributo(AttComumContrato_tipo_restricao, TipoRestricaoContrato_limite);
								contrato.setAtributo(AttComumContrato_tipo_unidade, TipoUnidadeRestricaoContrato_MW);

								lista_codigo_ONS_contrato.addElemento(idContrato, codigo_usina);

								const SmartEnupla<IdPatamarCarga, double> vetor_limite_inferior(IdPatamarCarga_1, limite_inferior);
								const SmartEnupla<IdPatamarCarga, double> vetor_limite_superior(IdPatamarCarga_1, limite_superior);
								const SmartEnupla<IdPatamarCarga, double>           vetor_custo(IdPatamarCarga_1, custo);

								const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_limite_inferior(horizonte_estudo, vetor_limite_inferior);
								const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_limite_superior(horizonte_estudo, vetor_limite_superior);
								const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>           matriz_custo(horizonte_estudo, vetor_custo);

								contrato.setMatriz(AttMatrizContrato_energia_minima, matriz_limite_inferior);
								contrato.setMatriz(AttMatrizContrato_energia_maxima, matriz_limite_superior);
								contrato.setMatriz(AttMatrizContrato_preco_energia_imp_exp, matriz_custo);

								a_dados.vetorContrato.add(contrato);

							}//if (int(idPatamarDeficit_inicializado.size()) == 1) {
							else {

								//Identifica o Contrato inicializado com codigo_usina
								const IdContrato idContrato_inicializado = getIdFromCodigoONS(lista_codigo_ONS_contrato, codigo_usina);

								if (idContrato_inicializado == IdContrato_Nenhum)
									throw std::invalid_argument("Nao inicializada idContrato com codigo_usina_" + atributo);

								const IdContrato  idContrato = idContrato_inicializado;

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									if (periodo >= horizonte_otimizacao_DC.at(idEstagio_contrato)) {

										for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

											a_dados.vetorContrato.att(idContrato).setElemento(AttMatrizContrato_energia_minima, periodo, idPatamarCarga, limite_inferior.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));
											a_dados.vetorContrato.att(idContrato).setElemento(AttMatrizContrato_energia_maxima, periodo, idPatamarCarga, limite_superior.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));
											a_dados.vetorContrato.att(idContrato).setElemento(AttMatrizContrato_preco_energia_imp_exp, periodo, idPatamarCarga, custo.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));

										}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

									}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_contrato)) {

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							}//else {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro CI/CE: \n" + std::string(erro.what())); }

					}//if (menemonico == "CI" || menemonico == "CE") {

					if (menemonico == "TI") {//Taxa de irrigação 

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Número da usina hidráulica (conforme campo 2 dos registros UH). 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int hidreletrica_codigo_usina = atoi(atributo.c_str());

							const IdHidreletrica idHidreletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, hidreletrica_codigo_usina);

							const IdHidreletrica idHidreletrica = idHidreletrica_inicializado;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3-19 -  Vazão desviada em cada estágio do estudo (m3/s).//i.e., 5 colunas/campo
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							std::vector<double> vazao_desviada;

							const int line_size = int(line.length());

							const int numeroEstagios = horizonte_otimizacao_DC.size();

							for (int estagio = 0; estagio < numeroEstagios; estagio++) {

								if (line_size >= 14 + 5 * estagio) {

									atributo = line.substr(9 + 5 * estagio, 5);
									atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

									vazao_desviada.push_back(atof(atributo.c_str()));

								}//if (line_size >= 14 + 5 * estagio) {

							}//for (int estagio = 0; estagio < numeroEstagios; estagio++) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Guarda informação nos Smart Elemtentos
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							SmartEnupla<Periodo, double> vetor_vazao_desviada(horizonte_estudo, 0.0);

							for (int estagio = 0; estagio < numeroEstagios; estagio++) {

								const IdEstagio idEstagio_vazao_desviada = IdEstagio(estagio + 1);

								const double valor = vazao_desviada.at(estagio);

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									//Atualiza a informação unicamente para o estágio DC indicado

									if (horizonte_otimizacao_DC.getIteradorFinal() == idEstagio_vazao_desviada) {

										//Se é último estágio DC, o periodo deve ser maior ou igual ao estágio DC

										if (periodo >= horizonte_otimizacao_DC.at(idEstagio_vazao_desviada))
											vetor_vazao_desviada.setElemento(periodo, valor);

									}//if (horizonte_otimizacao_DC.getIteradorFinal() == idEstagio_vazao_desviada) {
									else {

										//Se não é último estágio DC, o periodo deve ser entre dois estágios DC consecutivos

										const IdEstagio idEstagio_vazao_desviada_seguinte = IdEstagio(idEstagio_vazao_desviada + 1);

										if (periodo >= horizonte_otimizacao_DC.at(idEstagio_vazao_desviada) && periodo < horizonte_otimizacao_DC.at(idEstagio_vazao_desviada_seguinte))
											vetor_vazao_desviada.setElemento(periodo, valor);

										else if (periodo >= horizonte_otimizacao_DC.at(idEstagio_vazao_desviada_seguinte))
											break; //Evita percorrer todo o horizonte_estudo

									}//else {

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							}//for (int estagio = 0; estagio < numeroEstagios; estagio++) {

							////////////////////////////////////////////////////
							//Guarda informação nos Smart Elementos
							////////////////////////////////////////////////////

							if (a_dados.vetorHidreletrica.att(idHidreletrica).getSizeVetor(AttVetorHidreletrica_vazao_retirada) == 0)
								a_dados.vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_vazao_retirada, vetor_vazao_desviada);
							else {

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									double vazao_retirada = a_dados.vetorHidreletrica.att(idHidreletrica).getElementoVetor(AttVetorHidreletrica_vazao_retirada, periodo, double());
									vazao_retirada += vetor_vazao_desviada.getElemento(periodo);

									a_dados.vetorHidreletrica.att(idHidreletrica).setElemento(AttVetorHidreletrica_vazao_retirada, periodo, vazao_retirada);

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							}//else {

							/*

							const IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico(a_dados.getMaiorId(IdIntercambioHidraulico()) + 1);

							IntercambioHidraulico intercambioHidraulico;
							intercambioHidraulico.setAtributo(AttComumIntercambioHidraulico_idIntercambioHidraulico, idIntercambioHidraulico);

							a_dados.vetorIntercambioHidraulico.add(intercambioHidraulico);

							a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setAtributo(AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico_retirada);

							a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setAtributo(AttComumIntercambioHidraulico_hidreletrica_origem, idHidreletrica);
							a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setAtributo(AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica_Nenhum); //Manual: irrigação, abastecimento ou retirada de água para outros usos -> Portanto, não se acopla com outras usinas (IdHidreletrica_Nenhum)

							a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setVetor(AttVetorIntercambioHidraulico_desvio_agua_minimo, vetor_vazao_desviada);
							a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setVetor(AttVetorIntercambioHidraulico_desvio_agua_maximo, vetor_vazao_desviada);

							*/

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro TI: \n" + std::string(erro.what())); }

					}//if (menemonico == "TI") {


					if (menemonico == "HA") {//Identificação das restrições RHA					

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Número de identificação da restrição de afluência (entre 01 e 120).
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							
							const string nome = atributo;

							const int codigo_restricao_operativa_UHE_vazao_afluente = atoi(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Estágio inicial da restrição. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_inicial = getIdEstagioFromChar(atributo.c_str());

							Periodo periodo_inicio;

							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {
									periodo_inicio = periodo;
									break;
								}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {

							}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {


							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 4 -  Estágio final da restrição.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(14, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_final = getIdEstagioFromChar(atributo.c_str());

							const IdEstagio idEstagio_final_horizonte_otimizacao_DC = horizonte_otimizacao_DC.getIteradorFinal();

							Periodo periodo_final;

							if (idEstagio_final == idEstagio_final_horizonte_otimizacao_DC) {

								periodo_final = horizonte_estudo.getIteradorFinal();

							}//if (idEstagio_final == idEstagio_final_horizonte_otimizacao_DC) {
							else {//Se não é o último estágio, precisa encontrar o último periodo referente ao idEstagio_final

								Periodo periodo = horizonte_estudo.getIteradorInicial();

								const IdEstagio idEstagio_final_seguinte = IdEstagio(idEstagio_final + 1);

								while (horizonte_otimizacao_DC.at(idEstagio_final_seguinte) > periodo) {

									horizonte_estudo.incrementarIterador(periodo);

								}//while (horizonte_otimizacao_DC.at(idEstagio_final) > periodo) {

								horizonte_estudo.decrementarIterador(periodo);

								periodo_final = periodo;

							}//else {

							//Nota: Alguns decks repetem por erro o mesmo registro, portanto, é verificado que a restriçao nao tenha sido inicializada.
							//Caso contrário, verifica que o periodo_inicial e periodo_final do registro duplicado seja igual à restriçao instanciada
							const IdRestricaoOperativaUHE idRestricaoOperativaUHE_inicializado = getIdFromCodigoONS(lista_codigo_ONS_restricao_operativa_UHE_vazao_afluente, codigo_restricao_operativa_UHE_vazao_afluente);

							if (idRestricaoOperativaUHE_inicializado == IdRestricaoOperativaUHE_Nenhum) {

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Guarda informação nos Smart Elementos
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								RestricaoOperativaUHE restricaoOperativaUHE;

								IdRestricaoOperativaUHE idRestricaoOperativaUHE = a_dados.getMaiorId(IdRestricaoOperativaUHE());

								if (idRestricaoOperativaUHE == IdRestricaoOperativaUHE_Nenhum)
									idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1;
								else
									idRestricaoOperativaUHE++;

								restricaoOperativaUHE.setAtributo(AttComumRestricaoOperativaUHE_idRestricaoOperativaUHE, idRestricaoOperativaUHE);
								restricaoOperativaUHE.setAtributo(AttComumRestricaoOperativaUHE_nome, nome);
								restricaoOperativaUHE.setAtributo(AttComumRestricaoOperativaUHE_periodo_inicio, periodo_inicio);
								restricaoOperativaUHE.setAtributo(AttComumRestricaoOperativaUHE_periodo_final, periodo_final);
								//restricaoOperativaUHE.setAtributo(AttComumRestricaoOperativaUHE_tipoRestricaoOperativaUHE, tipoRestricaoOperativaUHE);

								restricaoOperativaUHE.setAtributo(AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica_vazao_afluente);

								lista_codigo_ONS_restricao_operativa_UHE_vazao_afluente.addElemento(idRestricaoOperativaUHE, codigo_restricao_operativa_UHE_vazao_afluente);

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Inicializa vetores minimo e maximo
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								//São instanciados todos os periodos do horizonte_estudo e logo no resgistro LA são atualizados os limites entre periodo_inicio e periodo_final

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									restricaoOperativaUHE.addElemento(AttVetorRestricaoOperativaUHE_limite_inferior, periodo, getdoubleFromChar("min"));
									restricaoOperativaUHE.addElemento(AttVetorRestricaoOperativaUHE_limite_superior, periodo, getdoubleFromChar("max"));

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Carrega restrição
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								a_dados.vetorRestricaoOperativaUHE.add(restricaoOperativaUHE);

							}
							else {

								if (periodo_inicio != a_dados.getAtributo(idRestricaoOperativaUHE_inicializado, AttComumRestricaoOperativaUHE_periodo_inicio, Periodo()))
									throw std::invalid_argument("Diversos registros com o mesmo codigo de restricao com diferente periodo_inicial");

								if (periodo_final != a_dados.getAtributo(idRestricaoOperativaUHE_inicializado, AttComumRestricaoOperativaUHE_periodo_final, Periodo()))
									throw std::invalid_argument("Diversos registros com o mesmo codigo de restricao com diferente periodo_final");

							}//else {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro HA: \n" + std::string(erro.what())); }

					}//if (menemonico == "HA") {

					if (menemonico == "LA") {//Limites das restrições RH

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Número da restrição de afluência, conforme campo 2 do registro HA
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Pode ocorrer que diferentes tipos de Restrição operativa das UHE tenham o mesmo código CEPEL; portanto, primeiro é
							// realizada uma seleção do tipo da restrição e logo uma procura pelo código CEPEL  asignado no registro HA

							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_restricao_operativa_UHE_vazao_afluente = atoi(atributo.c_str());

							const IdRestricaoOperativaUHE idRestricaoOperativaUHE_inicializado = getIdFromCodigoONS(lista_codigo_ONS_restricao_operativa_UHE_vazao_afluente, codigo_restricao_operativa_UHE_vazao_afluente);

							if (idRestricaoOperativaUHE_inicializado == IdRestricaoOperativaUHE_Nenhum)
								throw std::invalid_argument("Nao inicializada idRestricaoOperativaUHE  com codigo_restricao_operativa_UHE_vazao_afluente_" + atributo);

							const IdRestricaoOperativaUHE  idRestricaoOperativaUHE = idRestricaoOperativaUHE_inicializado;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Número do estágio, em ordem crescente.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_inicial = getIdEstagioFromChar(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Determina o periodo inicial e periodo final
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							Periodo periodo_inicio;

							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {
									periodo_inicio = periodo;
									break;
								}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {

							}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							const Periodo periodo_final = a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_periodo_final, Periodo());

							if (periodo_inicio < a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_periodo_inicio, Periodo()) || periodo_inicio > periodo_final)
								throw std::invalid_argument("Registro LA - Periodo inicio nao correspondente aos limites da restricao");

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Filosofia: Atualiza se encontrar um valor 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							std::string limiteInferior;
							std::string limiteSuperior;

							const int line_size = int(line.length());

							limiteInferior = "";
							limiteSuperior = "";

							if (line_size >= 24) {

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 4 - Limite inferior, (m3/s). 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								limiteInferior = line.substr(14, 10);
								limiteInferior.erase(std::remove(limiteInferior.begin(), limiteInferior.end(), ' '), limiteInferior.end());

							}//if (line_size >= 24) {

							if (line_size >= 34) {

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 5 - Limite superior (m3/s).
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								limiteSuperior = line.substr(24, 10);
								limiteSuperior.erase(std::remove(limiteSuperior.begin(), limiteSuperior.end(), ' '), limiteSuperior.end());

							}//if (line_size >= 34) {

							if (limiteInferior != "" || limiteSuperior != "") {

								//Filosofia: Este tipo de restrição é carregada para todos os periodos

								for (Periodo periodo = periodo_inicio; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {
									if (limiteInferior != "") { a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).setElemento(AttVetorRestricaoOperativaUHE_limite_inferior, periodo, std::atof(limiteInferior.c_str())); }
									if (limiteSuperior != "") { a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).setElemento(AttVetorRestricaoOperativaUHE_limite_superior, periodo, std::atof(limiteSuperior.c_str())); }
								}//for (Periodo periodo = periodo_inicio; periodo <= periodo_final; horizonte_estudo.at(periodo)) {

							}//if (limiteInferior != "" || limiteSuperior != "") {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro LA: \n" + std::string(erro.what())); }

					}//if (menemonico == "LA") {

					if (menemonico == "CA") {//Usinas hidrelétricas na restrição RHA 

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Número da restrição de afluência, conforme campo 2 do registro HA
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Pode ocorrer que diferentes tipos de Restrição operativa das UHE tenham o mesmo código CEPEL; portanto, primeiro é
							// realizada uma seleção do tipo da restrição e logo uma procura pelo código CEPEL  asignado no registro HA

							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_restricao_operativa_UHE_vazao_afluente = atoi(atributo.c_str());

							const IdRestricaoOperativaUHE idRestricaoOperativaUHE_inicializado = getIdFromCodigoONS(lista_codigo_ONS_restricao_operativa_UHE_vazao_afluente, codigo_restricao_operativa_UHE_vazao_afluente);

							if (idRestricaoOperativaUHE_inicializado == IdRestricaoOperativaUHE_Nenhum)
								throw std::invalid_argument("Nao inicializada idRestricaoOperativaUHE  com codigo_restricao_operativa_UHE_vazao_afluente_" + atributo);

							const IdRestricaoOperativaUHE  idRestricaoOperativaUHE = idRestricaoOperativaUHE_inicializado;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Número do estágio, em ordem crescente.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_inicial = getIdEstagioFromChar(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Determina o periodo inicial e periodo final
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							Periodo periodo_inicio;

							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {
									periodo_inicio = periodo;
									break;
								}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {

							}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							const Periodo periodo_final = a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_periodo_final, Periodo());

							if (periodo_inicio < a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_periodo_inicio, Periodo()) || periodo_inicio > periodo_final)
								throw std::invalid_argument("Registro CA - Periodo inicio nao correspondente aos limites da restricao");

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 4 -  Número da usina hidráulica conforme campo 2 dos registros UH.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(14, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int hidreletrica_codigo_usina = atoi(atributo.c_str());

							const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, hidreletrica_codigo_usina);

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Guarda informação nos Smart Elementos
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							//Manual Referência: 5.2.5.1 Restrição Hidráulica de Vazão Afluente (RHA) A restrição hidráulica de vazão afluente tem por objetivo especificar limites mínimos e máximos para 
							//a quantidade de água afluente a uma dada usina do sistema, para determinados estágios do estudo

							//Manual: Deve ser fornecido um registro para a usina hidrelétrica a que se refere a restrição RHA

							//Filosofia: Cada restrição RHA somente pode ter uma única usina associada

							if (periodo_inicio == a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_periodo_inicio, Periodo())) {//Ainda não foi inicializado

								if (a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).vetorElementoSistema.getMaiorId() > IdElementoSistema_Nenhum)
									throw std::invalid_argument("A restricao hidraulica RHA de afluencia so pode ter 1 usina associada");

								IdElementoSistema idElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());

								if (idElementoSistema == IdElementoSistema_Nenhum) { idElementoSistema = IdElementoSistema_1; }
								else { idElementoSistema++; }

								ElementoSistema elementoSistema;
								elementoSistema.setAtributo(AttComumElementoSistema_idElementoSistema, idElementoSistema);
								elementoSistema.setAtributo(AttComumElementoSistema_tipo_elemento, TipoElementoSistema_hidreletrica);
								elementoSistema.setAtributo(AttComumElementoSistema_hidreletrica, idHidreletrica);
								elementoSistema.setAtributo(AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa_vazao_afluente);

								//Inicializa o fator_participacao para o horizonte todo, porém no modelo são considerados somentes periodos com limites máximos e mínimos diferentes a "max" e "min"
								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
									elementoSistema.addElemento(AttVetorElementoSistema_fator_participacao, periodo, 1); //O manual não especifica nenhum fator de participação, foi adotado 1

								a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).vetorElementoSistema.add(elementoSistema);

							}//if (periodo_inicio == a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_periodo_inicio, Periodo())) {
							else {

								const std::vector<IdElementoSistema> idElementoSistema_inicializada = a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).vetorElementoSistema.getIdObjetos(AttComumElementoSistema_hidreletrica, idHidreletrica);

								if (int(idElementoSistema_inicializada.size()) == 0)
									throw std::invalid_argument("Elemento do sistema na restrição RHA nao instanciado");

								for (Periodo periodo = periodo_inicio; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo))
									a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).vetorElementoSistema.att(idElementoSistema_inicializada.at(0)).setElemento(AttVetorElementoSistema_fator_participacao, periodo, 1);

							}//else {
						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro CA: \n" + std::string(erro.what())); }

					}//if (menemonico == "CA") {

					if (menemonico == "HV") {//Identificação das restrições RHV: Restrições de volume armazenado/volume defluente

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Número de identificação da restrição de afluência (entre 01 e 120).
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const string nome = atributo;

							const int codigo_restricao_operativa_UHE_volume = atoi(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Estágio inicial da restrição. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_inicial = getIdEstagioFromChar(atributo.c_str());

							Periodo periodo_inicio;

							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {
									periodo_inicio = periodo;
									break;
								}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {

							}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {


							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 4 -  Estágio final da restrição.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(14, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_final = getIdEstagioFromChar(atributo.c_str());

							const IdEstagio idEstagio_final_horizonte_otimizacao_DC = horizonte_otimizacao_DC.getIteradorFinal();

							Periodo periodo_final;

							if (idEstagio_final == idEstagio_final_horizonte_otimizacao_DC) {

								periodo_final = horizonte_estudo.getIteradorFinal();

							}//if (idEstagio_final == idEstagio_final_horizonte_otimizacao_DC) {
							else {//Se não é o último estágio, precisa encontrar o último periodo referente ao idEstagio_final

								Periodo periodo = horizonte_estudo.getIteradorInicial();

								const IdEstagio idEstagio_final_seguinte = IdEstagio(idEstagio_final + 1);

								while (horizonte_otimizacao_DC.at(idEstagio_final_seguinte) > periodo) {

									horizonte_estudo.incrementarIterador(periodo);

								}//while (horizonte_otimizacao_DC.at(idEstagio_final) > periodo) {

								horizonte_estudo.decrementarIterador(periodo);

								periodo_final = periodo;

							}//else {

							//Nota: Alguns decks repetem por erro o mesmo registro, portanto, é verificado que a restriçao nao tenha sido inicializada.
							//Caso contrário, verifica que o periodo_inicial e periodo_final do registro duplicado seja igual à restriçao instanciada
							const IdRestricaoOperativaUHE idRestricaoOperativaUHE_inicializado = getIdFromCodigoONS(lista_codigo_ONS_restricao_operativa_UHE_volume, codigo_restricao_operativa_UHE_volume);

							if (idRestricaoOperativaUHE_inicializado == IdRestricaoOperativaUHE_Nenhum) {

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Guarda informação nos Smart Elementos
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								RestricaoOperativaUHE restricaoOperativaUHE;

								IdRestricaoOperativaUHE idRestricaoOperativaUHE = a_dados.getMaiorId(IdRestricaoOperativaUHE());

								if (idRestricaoOperativaUHE == IdRestricaoOperativaUHE_Nenhum)
									idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1;
								else
									idRestricaoOperativaUHE++;

								restricaoOperativaUHE.setAtributo(AttComumRestricaoOperativaUHE_idRestricaoOperativaUHE, idRestricaoOperativaUHE);
								restricaoOperativaUHE.setAtributo(AttComumRestricaoOperativaUHE_nome, nome);
								restricaoOperativaUHE.setAtributo(AttComumRestricaoOperativaUHE_periodo_inicio, periodo_inicio);
								restricaoOperativaUHE.setAtributo(AttComumRestricaoOperativaUHE_periodo_final, periodo_final);

								restricaoOperativaUHE.setAtributo(AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica_volume_armazenado);

								lista_codigo_ONS_restricao_operativa_UHE_volume.addElemento(idRestricaoOperativaUHE, codigo_restricao_operativa_UHE_volume);

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Inicializa matrizes minimo e maximo
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								//Nota: limite_inferior/limite_superior com valor min/max não é considerado dentro do modelo
								//São instanciados todos os periodos do horizonte_estudo e logo no resgistro LV são atualizados os limites entre periodo_inicio e periodo_final

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

									for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

										restricaoOperativaUHE.addElemento(AttMatrizRestricaoOperativaUHE_limite_inferior, periodo, idPatamarCarga, getdoubleFromChar("min"));
										restricaoOperativaUHE.addElemento(AttMatrizRestricaoOperativaUHE_limite_superior, periodo, idPatamarCarga, getdoubleFromChar("max"));

									}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								//const SmartEnupla<Periodo, double> vetor_fator_participacao(horizonte_estudo, 0);

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Carrega restrição
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								a_dados.vetorRestricaoOperativaUHE.add(restricaoOperativaUHE);

							}//if (idRestricaoOperativaUHE_inicializado == IdRestricaoOperativaUHE_Nenhum)
							else {

								if (periodo_inicio != a_dados.getAtributo(idRestricaoOperativaUHE_inicializado, AttComumRestricaoOperativaUHE_periodo_inicio, Periodo()))
									throw std::invalid_argument("Diversos registros com o mesmo codigo de restricao com diferente periodo_inicial");

								if (periodo_final != a_dados.getAtributo(idRestricaoOperativaUHE_inicializado, AttComumRestricaoOperativaUHE_periodo_final, Periodo()))
									throw std::invalid_argument("Diversos registros com o mesmo codigo de restricao com diferente periodo_inicial");


							}//else {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro HV: \n" + std::string(erro.what())); }

					}//if (menemonico == "HV") {

					if (menemonico == "LV") {//Limites das restrições RHV 

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Número da restrição de afluência, conforme campo 2 do registro HA
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Pode ocorrer que diferentes tipos de Restrição operativa das UHE tenham o mesmo código CEPEL; portanto, primeiro é
							// realizada uma seleção do tipo da restrição e logo uma procura pelo código CEPEL  asignado no registro HA

							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_restricao_operativa_UHE_volume = atoi(atributo.c_str());

							const IdRestricaoOperativaUHE idRestricaoOperativaUHE_inicializado = getIdFromCodigoONS(lista_codigo_ONS_restricao_operativa_UHE_volume, codigo_restricao_operativa_UHE_volume);

							if (idRestricaoOperativaUHE_inicializado == IdRestricaoOperativaUHE_Nenhum)
								throw std::invalid_argument("Nao inicializada idRestricaoOperativaUHE  com codigo_restricao_operativa_UHE_volume_" + atributo);

							const IdRestricaoOperativaUHE  idRestricaoOperativaUHE = idRestricaoOperativaUHE_inicializado;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Número do estágio, em ordem crescente.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_inicial = getIdEstagioFromChar(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Determina o periodo inicial e periodo final
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							Periodo periodo_inicio;

							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {
									periodo_inicio = periodo;
									break;
								}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {

							}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							const Periodo periodo_final = a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_periodo_final, Periodo());

							if (periodo_inicio < a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_periodo_inicio, Periodo()) || periodo_inicio > periodo_final)
								throw std::invalid_argument("Registro LV - Periodo inicio nao correspondente aos limites da restricao");

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Filosofia: Atualiza se encontrar um valor 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							std::string limiteInferior;
							std::string limiteSuperior;

							const int line_size = int(line.length());

							limiteInferior = "";
							limiteSuperior = "";

							if (line_size >= 24) {

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 4 - Limite inferior (hm3). 
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								limiteInferior = line.substr(14, 10);
								limiteInferior.erase(std::remove(limiteInferior.begin(), limiteInferior.end(), ' '), limiteInferior.end());

							}//if (line_size >= 24) {

							if (line_size >= 34) {

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 5 - Limite superior (hm3).
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								limiteSuperior = line.substr(24, 10);
								limiteSuperior.erase(std::remove(limiteSuperior.begin(), limiteSuperior.end(), ' '), limiteSuperior.end());

							}//if (line_size >= 34) {

							if (limiteInferior != "" || limiteSuperior != "") {

								//Filosofia: As restrições são fornecidas para um estágio_DC, portanto, o limite da restrição SOMENTE vai ser atualizado no periodo_SPT onde termina o estágio_DC
								//com o fim da construção da restrição RHV, onde valores "MAX" dos limites são ignorados

								//////////////////////////////////////////////////////////////////////////////
								//Determina o idEstagio_final_DC
								//////////////////////////////////////////////////////////////////////////////

								//const IdEstagio idEstagio_final_DC = horizonte_otimizacao_DC.getIteradorFinal();

								IdEstagio idEstagio_final_DC = IdEstagio_Nenhum;

								if (periodo_final == horizonte_estudo.getIteradorFinal())
									idEstagio_final_DC = horizonte_otimizacao_DC.getIteradorFinal();
								else {

									for (IdEstagio idEstagio_aux = idEstagio_inicial; idEstagio_aux <= horizonte_otimizacao_DC.getIteradorFinal(); idEstagio_aux++) {

										if (horizonte_otimizacao_DC.at(IdEstagio(idEstagio_aux + 1)) > periodo_final) {//Se o estágio seguinte é maior do que o periodo_final

											idEstagio_final_DC = idEstagio_aux;
											break;

										}//if (horizonte_otimizacao_DC.at(idEstagio_aux) >= periodo_final) {

									}//for (IdEstagio idEstagio = IdEstagio_1; idEstagio <= horizonte_otimizacao_DC.getIteradorFinal(); idEstagio++) {

								}//else {

								if (idEstagio_final_DC == IdEstagio_Nenhum)
									throw std::invalid_argument("Registro LV - IdEstagio_DC incompativel com o horizonte de estudo");


								//////////////////////////////////////////////////////////////////////////////

								for (IdEstagio idEstagio_DC = idEstagio_inicial; idEstagio_DC <= idEstagio_final_DC; idEstagio_DC++) {

									Periodo periodo_seguinte = periodo_inicio;
									Periodo periodo_atualizar_limites = periodo_inicio;

									if (idEstagio_DC < horizonte_otimizacao_DC.getIteradorFinal()) {

										IdEstagio idEstagio_DC_seguinte = IdEstagio(idEstagio_DC + 1);
										Periodo periodo_DC_seguinte = horizonte_otimizacao_DC.at(idEstagio_DC_seguinte);

										while (true) {

											//Pega o periodo anterior a quando o tem uma mudança de estágio no DC (o qual correponde ao último periodo paralelo ao DC)

											horizonte_estudo.incrementarIterador(periodo_seguinte);

											if (periodo_seguinte >= periodo_DC_seguinte)
												break;

											horizonte_estudo.incrementarIterador(periodo_atualizar_limites);

										}//while (true) {

									}//if (idEstagio_DC < idEstagio_final_DC) {
									else //Para o último estágio o periodo_atualizar corresponde com último periodo do horizonte_estudo
										periodo_atualizar_limites = horizonte_estudo.getIteradorFinal();

									if (limiteInferior != "") {

										const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_atualizar_limites, IdPatamarCarga());

										for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++)
											a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).setElemento(AttMatrizRestricaoOperativaUHE_limite_inferior, periodo_atualizar_limites, idPatamarCarga, std::atof(limiteInferior.c_str()));

									}//if (limiteInferior != "") {
									if (limiteSuperior != "") {

										const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo_atualizar_limites, IdPatamarCarga());

										for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++)
											a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).setElemento(AttMatrizRestricaoOperativaUHE_limite_superior, periodo_atualizar_limites, idPatamarCarga, std::atof(limiteSuperior.c_str()));

									}//if (limiteSuperior != "") { 

								}//for (IdEstagio idEstagio_DC = idEstagio_inicial; idEstagio_DC <= horizonte_otimizacao_DC.getIteradorFinal(); idEstagio_DC++) {

							}//if (limiteInferior != "" || limiteSuperior != "") {
						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro LV: \n" + std::string(erro.what())); }

					}//if (menemonico == "LV") {

					if (menemonico == "CV") {//Coeficientes das restrições RHV

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Número da restrição de Volume, conforme campo 2 do registro HV
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Pode ocorrer que diferentes tipos de Restrição operativa das UHE tenham o mesmo código CEPEL; portanto, primeiro é
							// realizada uma seleção do tipo da restrição e logo uma procura pelo código CEPEL  asignado no registro HA

							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_restricao_operativa_UHE_volume = atoi(atributo.c_str());

							const IdRestricaoOperativaUHE idRestricaoOperativaUHE_inicializado = getIdFromCodigoONS(lista_codigo_ONS_restricao_operativa_UHE_volume, codigo_restricao_operativa_UHE_volume);

							if (idRestricaoOperativaUHE_inicializado == IdRestricaoOperativaUHE_Nenhum)
								throw std::invalid_argument("Nao inicializada idRestricaoOperativaUHE  com codigo_restricao_operativa_UHE_volume_" + atributo);

							const IdRestricaoOperativaUHE  idRestricaoOperativaUHE = idRestricaoOperativaUHE_inicializado;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Número do estágio, em ordem crescente.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_inicial = getIdEstagioFromChar(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Determina o periodo inicial e periodo final
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							Periodo periodo_inicio;

							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {
									periodo_inicio = periodo;
									break;
								}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {

							}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							const Periodo periodo_final = a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_periodo_final, Periodo());

							if (periodo_inicio < a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_periodo_inicio, Periodo()) || periodo_inicio > periodo_final)
								throw std::invalid_argument("Registro CV - Periodo inicio nao correspondente aos limites da restricao");

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 5 -  Coeficiente de cada variável na restrição HV. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(19, 10);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							double fator_participacao = atof(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 6 -  Tipos válidos para a variável na restrição RHV:
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							const int line_size = int(line.length());

							atributo = line.substr(34, 4);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							TipoVariavelRestricaoOperativa tipoVariavelRestricaoOperativa;

							if (line_size >= 38) {

								if (atributo == "VARM")
									tipoVariavelRestricaoOperativa = TipoVariavelRestricaoOperativa_volume_util;
								else if (atributo == "VDEF")
									tipoVariavelRestricaoOperativa = TipoVariavelRestricaoOperativa_volume_defluente;
								else if (atributo == "VDES")
									tipoVariavelRestricaoOperativa = TipoVariavelRestricaoOperativa_volume_desviado;
								else if (atributo == "VBOM")
									tipoVariavelRestricaoOperativa = TipoVariavelRestricaoOperativa_volume_bombeado;

							}//if (line_size >= 38) {
							else
								tipoVariavelRestricaoOperativa = TipoVariavelRestricaoOperativa_volume_util; //Valor default

							//Se for um dado de vazão convertida em volume então divide pelo conversor_vazao_m3s_em_volume_hm3 para obter o coeficiente de participação na restrição e logo na construção das restrições é multiplicado pelo conversor do periodo SPT
							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_desviado || tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_defluente)
								fator_participacao *= std::pow(conversor_vazao_m3s_em_volume_hm3(horizonte_otimizacao_DC.at(idEstagio_inicial)), -1);

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 4 -  Número da usina hidrelétrica, conforme campo 2 do registro UH 
							//           ou estação de bombeamento, conforme campo 2 do registro UE
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(14, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							//Manual: 
							//-Deve ser fornecido um registro para cada variável (volume armazenado no reservatório, vazão defluente de uma usina hidrelétrica ou vazão bombeada de uma estação de bombeamento) com coeficiente não nulo na restrição RHV
							//-Podem ser fornecidos mais de um registro CV para uma mesma restrição de VARM. Neste caso a restrição passa a envolver os volumes armazenados de mais de uma usina

							//Filosofia: Pode ter uma mesma restrição com variáveis VARM, VDEF, VDES, VBOM da mesma usina ou de usinas diferentes, ver Manual Referência seção 5.2.5.3

							if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_bombeado) {//Volume bombeada nas estações de bombeamento. 

								const int usina_elevatoria_codigo_usina = atoi(atributo.c_str());

								const IdUsinaElevatoria idUsinaElevatoria = getIdFromCodigoONS(lista_codigo_ONS_usina_elevatoria, usina_elevatoria_codigo_usina);

								if (idUsinaElevatoria == IdUsinaElevatoria_Nenhum)
									throw std::invalid_argument("Nao inicializada idUsinaElevatoria com codigo_usina_" + atributo);

								if (periodo_inicio == a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_periodo_inicio, Periodo())) {//Ainda não foi inicializado

									IdElementoSistema idElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());

									if (idElementoSistema == IdElementoSistema_Nenhum) { idElementoSistema = IdElementoSistema_1; }
									else { idElementoSistema++; }

									ElementoSistema elementoSistema;
									elementoSistema.setAtributo(AttComumElementoSistema_idElementoSistema, idElementoSistema);
									elementoSistema.setAtributo(AttComumElementoSistema_tipo_elemento, TipoElementoSistema_usina_elevatoria);
									elementoSistema.setAtributo(AttComumElementoSistema_usina_elevatoria, idUsinaElevatoria);
									elementoSistema.setAtributo(AttComumElementoSistema_tipoVariavelRestricaoOperativa, tipoVariavelRestricaoOperativa);

									//Inicializa o fator_participacao para o horizonte todo, porém no modelo são considerados somentes periodos com limites máximos e mínimos diferentes a "max" e "min"
									for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
										elementoSistema.addElemento(AttVetorElementoSistema_fator_participacao, periodo, fator_participacao);

									a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).vetorElementoSistema.add(elementoSistema);

								}//if (periodo_inicio == a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_periodo_inicio, Periodo())) {
								else {

									const std::vector<IdElementoSistema> idElementoSistema_inicializada = a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).vetorElementoSistema.getIdObjetos(AttComumElementoSistema_usina_elevatoria, idUsinaElevatoria);

									if (int(idElementoSistema_inicializada.size()) == 0)
										throw std::invalid_argument("Elemento do sistema na restrição RHV nao instanciado");

									for (Periodo periodo = periodo_inicio; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo))
										a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).vetorElementoSistema.att(idElementoSistema_inicializada.at(0)).setElemento(AttVetorElementoSistema_fator_participacao, periodo, fator_participacao);

								}//else {				

							}//if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_bombeado) {
							else if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_util || tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_desviado || tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_defluente) {

								const int hidreletrica_codigo_usina = atoi(atributo.c_str());

								const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, hidreletrica_codigo_usina);

								if (idHidreletrica == IdHidreletrica_Nenhum)
									throw std::invalid_argument("Nao inicializada idHidreletrica com codigo_usina_" + atributo);

								if (periodo_inicio == a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_periodo_inicio, Periodo())) {//Ainda não foi inicializado

									IdElementoSistema idElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());

									if (idElementoSistema == IdElementoSistema_Nenhum) { idElementoSistema = IdElementoSistema_1; }
									else { idElementoSistema++; }

									ElementoSistema elementoSistema;
									elementoSistema.setAtributo(AttComumElementoSistema_idElementoSistema, idElementoSistema);
									elementoSistema.setAtributo(AttComumElementoSistema_tipo_elemento, TipoElementoSistema_hidreletrica);
									elementoSistema.setAtributo(AttComumElementoSistema_hidreletrica, idHidreletrica);
									elementoSistema.setAtributo(AttComumElementoSistema_tipoVariavelRestricaoOperativa, tipoVariavelRestricaoOperativa);

									for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
										elementoSistema.addElemento(AttVetorElementoSistema_fator_participacao, periodo, fator_participacao);

									a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).vetorElementoSistema.add(elementoSistema);

								}//if (periodo_inicio == a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_periodo_inicio, Periodo())) {
								else {

									const std::vector<IdElementoSistema> idElementoSistema_inicializada = a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).vetorElementoSistema.getIdObjetos(AttComumElementoSistema_hidreletrica, idHidreletrica);

									IdElementoSistema idElementoSistema = IdElementoSistema_Nenhum;

									for (int pos = 0; pos < int(idElementoSistema_inicializada.size()); pos++) {

										if (tipoVariavelRestricaoOperativa == a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).vetorElementoSistema.att(idElementoSistema_inicializada.at(pos)).getAtributo(AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa())) {
											idElementoSistema = idElementoSistema_inicializada.at(pos);
											break;
										}//if (tipoVariavelRestricaoOperativa == a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).vetorElementoSistema.att(idElementoSistema_inicializada.at(pos)).getAtributo(AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa())) {

									}//for (int pos = 0; pos < int(idElementoSistema_inicializada.size()); pos++) {

									if (idElementoSistema == IdElementoSistema_Nenhum)
										throw std::invalid_argument("Elemento do sistema na restrição RHV nao instanciado");

									for (Periodo periodo = periodo_inicio; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo))
										a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).vetorElementoSistema.att(idElementoSistema).setElemento(AttVetorElementoSistema_fator_participacao, periodo, fator_participacao);

								}//else {

							}//else if(tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_armazenado || tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_desviado || tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_volume_defluente) {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro CV: \n" + std::string(erro.what())); }

					}//if (menemonico == "CV") {

					if (menemonico == "HQ") {//Identificação das restrições RHQ: Restrição Hidráulica de Vazão Defluente 

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Número de identificação da restrição de afluência (entre 01 e 120).
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const string nome = atributo;

							const int codigo_restricao_operativa_UHE_vazao_defluente = atoi(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Estágio inicial da restrição. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_inicial = getIdEstagioFromChar(atributo.c_str());

							Periodo periodo_inicio;

							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {
									periodo_inicio = periodo;
									break;
								}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {

							}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {


							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 4 -  Estágio final da restrição.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(14, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_final = getIdEstagioFromChar(atributo.c_str());

							const IdEstagio idEstagio_final_horizonte_otimizacao_DC = horizonte_otimizacao_DC.getIteradorFinal();

							Periodo periodo_final;

							if (idEstagio_final == idEstagio_final_horizonte_otimizacao_DC) {

								periodo_final = horizonte_estudo.getIteradorFinal();

							}//if (idEstagio_final == idEstagio_final_horizonte_otimizacao_DC) {
							else {//Se não é o último estágio, precisa encontrar o último periodo referente ao idEstagio_final

								Periodo periodo = horizonte_estudo.getIteradorInicial();

								const IdEstagio idEstagio_final_seguinte = IdEstagio(idEstagio_final + 1);

								while (horizonte_otimizacao_DC.at(idEstagio_final_seguinte) > periodo) {

									horizonte_estudo.incrementarIterador(periodo);

								}//while (horizonte_otimizacao_DC.at(idEstagio_final) > periodo) {

								horizonte_estudo.decrementarIterador(periodo);

								periodo_final = periodo;

							}//else {

							//Nota: Alguns decks repetem por erro o mesmo registro, portanto, é verificado que a restriçao nao tenha sido inicializada.
							//Caso contrário, verifica que o periodo_inicial e periodo_final do registro duplicado seja igual à restriçao instanciada
							const IdRestricaoOperativaUHE idRestricaoOperativaUHE_inicializado = getIdFromCodigoONS(lista_codigo_ONS_restricao_operativa_UHE_vazao_defluente, codigo_restricao_operativa_UHE_vazao_defluente);

							if (idRestricaoOperativaUHE_inicializado == IdRestricaoOperativaUHE_Nenhum) {

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Guarda informação nos Smart Elementos
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								RestricaoOperativaUHE restricaoOperativaUHE;

								IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE(a_dados.getMaiorId(IdRestricaoOperativaUHE()) + 1);

								restricaoOperativaUHE.setAtributo(AttComumRestricaoOperativaUHE_idRestricaoOperativaUHE, idRestricaoOperativaUHE);
								restricaoOperativaUHE.setAtributo(AttComumRestricaoOperativaUHE_nome, nome);
								restricaoOperativaUHE.setAtributo(AttComumRestricaoOperativaUHE_periodo_inicio, periodo_inicio);
								restricaoOperativaUHE.setAtributo(AttComumRestricaoOperativaUHE_periodo_final, periodo_final);

								restricaoOperativaUHE.setAtributo(AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica_vazao_defluente);

								lista_codigo_ONS_restricao_operativa_UHE_vazao_defluente.addElemento(idRestricaoOperativaUHE, codigo_restricao_operativa_UHE_vazao_defluente);

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Inicializa vetor fator participação
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								//restricaoOperativaUHE.setVetor(AttVetorRestricaoOperativaUHE_fator_participacao, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Carrega Restrição Operativa
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								a_dados.vetorRestricaoOperativaUHE.add(restricaoOperativaUHE);

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Inicializa matrizes minimo e maximo
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								//O numero_patamares não é informado no registro especificado, porém, da leitura de outros registros é possível obtê-lo
								//São instanciados todos os periodos do horizonte_estudo e logo no resgistro LQ são atualizados os limites entre periodo_inicio e periodo_final

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

									for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

										// Nota: limite_inferior / limite_superior com valor max não é considerado dentro do modelo

										a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).addElemento(AttMatrizRestricaoOperativaUHE_limite_inferior, periodo, idPatamarCarga, getdoubleFromChar("min"));
										a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).addElemento(AttMatrizRestricaoOperativaUHE_limite_superior, periodo, idPatamarCarga, getdoubleFromChar("max"));

									} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
								} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							}
							else {

								if (periodo_inicio != a_dados.getAtributo(idRestricaoOperativaUHE_inicializado, AttComumRestricaoOperativaUHE_periodo_inicio, Periodo()))
									throw std::invalid_argument("Diversos registros com o mesmo codigo de restricao com diferente periodo_inicial");

								if (periodo_final != a_dados.getAtributo(idRestricaoOperativaUHE_inicializado, AttComumRestricaoOperativaUHE_periodo_final, Periodo()))
									throw std::invalid_argument("Diversos registros com o mesmo codigo de restricao com diferente periodo_inicial");

							}//else {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro HQ: \n" + std::string(erro.what())); }

					}//if (menemonico == "HQ") {

					if (menemonico == "LQ") {// Limites das restrições RHQ 

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Número da restrição de afluência, conforme campo 2 do registro HA
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Pode ocorrer que diferentes tipos de Restrição operativa das UHE tenham o mesmo código CEPEL; portanto, primeiro é
							// realizada uma seleção do tipo da restrição e logo uma procura pelo código CEPEL  asignado no registro HA

							const int codigo_restricao_operativa_UHE_vazao_defluente = std::stoi(line.substr(4, 3));

							const IdRestricaoOperativaUHE idRestricaoOperativaUHE_inicializado = getIdFromCodigoONS(lista_codigo_ONS_restricao_operativa_UHE_vazao_defluente, codigo_restricao_operativa_UHE_vazao_defluente);

							if (idRestricaoOperativaUHE_inicializado == IdRestricaoOperativaUHE_Nenhum)
								throw std::invalid_argument("Nao inicializada idRestricaoOperativaUHE  com codigo_restricao_operativa_UHE_vazao_defluente_" + atributo);

							const IdRestricaoOperativaUHE  idRestricaoOperativaUHE = idRestricaoOperativaUHE_inicializado;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Número do estágio, em ordem crescente.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_inicial = getIdEstagioFromChar(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Determina o periodo inicial e periodo final
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							Periodo periodo_inicio;

							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {
									periodo_inicio = periodo;
									break;
								}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {

							}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {


							const Periodo periodo_final = a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_periodo_final, Periodo());

							if (periodo_inicio < a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_periodo_inicio, Periodo()) || periodo_inicio > periodo_final)
								throw std::invalid_argument("Registro LQ - Periodo inicio nao correspondente aos limites da restricao");

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Filosofia: Atualiza se encontrar um valor 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							std::string limiteInferior;
							std::string limiteSuperior;

							const int line_size = int(line.length());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Patamar 1-5
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							int numero_patamar = 0;

							for (int patamar = 0; patamar < 5; patamar++) {

								limiteInferior = "";
								limiteSuperior = "";

								if (line_size >= 24 + 20 * patamar) {

									/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
									//Campo 4 - Limite inferior (hm3). 
									/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
									limiteInferior = line.substr(14 + 20 * patamar, 10);
									limiteInferior.erase(std::remove(limiteInferior.begin(), limiteInferior.end(), ' '), limiteInferior.end());

								}//if (line_size >= 24 + 20 * patamar) {

								if (line_size >= 34 + 20 * patamar) {

									/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
									//Campo 5 - Limite superior (hm3).
									/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
									limiteSuperior = line.substr(24 + 20 * patamar, 10);
									limiteSuperior.erase(std::remove(limiteSuperior.begin(), limiteSuperior.end(), ' '), limiteSuperior.end());

								}//if (line_size >= 34 + 20 * patamar) {

								if (limiteInferior != "" || limiteSuperior != "") {

									//Filosofia: Este tipo de restrição é carregada para todos os periodos

									IdPatamarCarga idPatamarCarga = IdPatamarCarga(patamar + 1);

									for (Periodo periodo = periodo_inicio; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {
										if (limiteInferior != "") { a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).setElemento(AttMatrizRestricaoOperativaUHE_limite_inferior, periodo, idPatamarCarga, std::atof(limiteInferior.c_str())); }
										if (limiteSuperior != "") { a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).setElemento(AttMatrizRestricaoOperativaUHE_limite_superior, periodo, idPatamarCarga, std::atof(limiteSuperior.c_str())); }
									}//for (Periodo periodo = periodo_inicio; periodo <= periodo_final; horizonte_estudo.at(periodo)) {

								}//if (limiteInferior != "" || limiteSuperior != "") {

							}//for (int patamar = 0; patamar < 5; patamar++) {
						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro LQ: \n" + std::string(erro.what())); }

					}//if (menemonico == "LQ") {

					if (menemonico == "CQ") {// Coeficientes das restrições RHQ 

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Número da restrição de Volume, conforme campo 2 do registro HV
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Pode ocorrer que diferentes tipos de Restrição operativa das UHE tenham o mesmo código CEPEL; portanto, primeiro é
							// realizada uma seleção do tipo da restrição e logo uma procura pelo código CEPEL  asignado no registro HQ

							const int codigo_restricao_operativa_UHE_vazao_defluente = std::stoi(line.substr(4, 3));

							const IdRestricaoOperativaUHE idRestricaoOperativaUHE_inicializado = getIdFromCodigoONS(lista_codigo_ONS_restricao_operativa_UHE_vazao_defluente, codigo_restricao_operativa_UHE_vazao_defluente);

							if (idRestricaoOperativaUHE_inicializado == IdRestricaoOperativaUHE_Nenhum)
								throw std::invalid_argument("Nao inicializada idRestricaoOperativaUHE  com codigo_restricao_operativa_UHE_vazao_defluente_" + atributo);

							const IdRestricaoOperativaUHE  idRestricaoOperativaUHE = idRestricaoOperativaUHE_inicializado;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Número do estágio, em ordem crescente.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							const IdEstagio idEstagio_inicial = IdEstagio(std::stoi(line.substr(9, 2)));

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Determina o periodo inicial e periodo final
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							Periodo periodo_inicio;

							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {
									periodo_inicio = periodo;
									break;
								}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {

							}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							const Periodo periodo_final = a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_periodo_final, Periodo());

							if (periodo_inicio < a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_periodo_inicio, Periodo()) || periodo_inicio > periodo_final)
								throw std::invalid_argument("Registro CQ - Periodo inicio nao correspondente aos limites da restricao");

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 5 -  Coeficiente de cada variável na restrição RHQ. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							const double fator_participacao = std::stoi(line.substr(19, 10));

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 6 -  Tipos válidos para a variável na restrição RHQ:
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							string tipo_restricao_RHQ = line.substr(34, 4);
							tipo_restricao_RHQ.erase(std::remove(tipo_restricao_RHQ.begin(), tipo_restricao_RHQ.end(), ' '), tipo_restricao_RHQ.end());

							//Manual Referência: A restrição hidráulica de vazão defluente consiste em uma restrição que possibilita determinar faixas de operação para algumas variáveis do problema, 
							//como vazão defluente, desvios de água e bombeamentos. Além disso, este tipo de restrição permite que o usuário combine faixas de operação para essas variáveis

							if ((tipo_restricao_RHQ == "QDEF") || (tipo_restricao_RHQ == "QDES") || (tipo_restricao_RHQ == "QTUR") || (tipo_restricao_RHQ == "QVER")) { //Vazão defluênte ou vazão desviada de uma hidrelétrica

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 4 -  Número da usina hidrelétrica, conforme campo 2 do registro UH
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(14, 3);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const int hidreletrica_codigo_usina = atoi(atributo.c_str());

								const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, hidreletrica_codigo_usina);

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 6 -  Tipos válidos para a variável na restrição RHQ:
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								TipoVariavelRestricaoOperativa tipoVariavelRestricaoOperativa = TipoVariavelRestricaoOperativa_Nenhum;

								if (tipo_restricao_RHQ == "QDEF")
									tipoVariavelRestricaoOperativa = TipoVariavelRestricaoOperativa_vazao_defluente;
								else if (tipo_restricao_RHQ == "QDES")
									tipoVariavelRestricaoOperativa = TipoVariavelRestricaoOperativa_vazao_desviada;
								else if (tipo_restricao_RHQ == "QTUR")
									tipoVariavelRestricaoOperativa = TipoVariavelRestricaoOperativa_vazao_turbinada;
								else if (tipo_restricao_RHQ == "QVER")
									tipoVariavelRestricaoOperativa = TipoVariavelRestricaoOperativa_vazao_vertida;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Guarda informação nos Smart Elementos
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								if (periodo_inicio == a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_periodo_inicio, Periodo())) {//Ainda não foi inicializado

									IdElementoSistema idElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());

									if (idElementoSistema == IdElementoSistema_Nenhum) { idElementoSistema = IdElementoSistema_1; }
									else { idElementoSistema++; }

									ElementoSistema elementoSistema;
									elementoSistema.setAtributo(AttComumElementoSistema_idElementoSistema, idElementoSistema);
									elementoSistema.setAtributo(AttComumElementoSistema_tipo_elemento, TipoElementoSistema_hidreletrica);
									elementoSistema.setAtributo(AttComumElementoSistema_hidreletrica, idHidreletrica);
									elementoSistema.setAtributo(AttComumElementoSistema_tipoVariavelRestricaoOperativa, tipoVariavelRestricaoOperativa);

									//Inicializa o fator_participacao para o horizonte todo, porém no modelo são considerados somentes periodos com limites máximos e mínimos diferentes a "max" e "min"
									for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
										elementoSistema.addElemento(AttVetorElementoSistema_fator_participacao, periodo, fator_participacao);

									a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).vetorElementoSistema.add(elementoSistema);

								}//if (periodo_inicio == a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_periodo_inicio, Periodo())) {						
								else {

									const std::vector<IdElementoSistema> idElementoSistema_inicializada = a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).vetorElementoSistema.getIdObjetos(AttComumElementoSistema_hidreletrica, idHidreletrica);

									IdElementoSistema idElementoSistema = IdElementoSistema_Nenhum;

									//Uma mesma Restrição pode ter dois elementos associados à mesma hidrelétrica mas uma QDEF e outra QDES 

									for (int pos = 0; pos < int(idElementoSistema_inicializada.size()); pos++)
										if (a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).vetorElementoSistema.att(idElementoSistema_inicializada.at(pos)).getAtributo(AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa()) == tipoVariavelRestricaoOperativa)
											idElementoSistema = idElementoSistema_inicializada.at(pos);

									if (idElementoSistema == IdElementoSistema_Nenhum)
										throw std::invalid_argument("Restricao RHQ com idElementoSistema nao definido");

									for (Periodo periodo = periodo_inicio; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo))
										a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).vetorElementoSistema.att(idElementoSistema).setElemento(AttVetorElementoSistema_fator_participacao, periodo, fator_participacao);

								}//else {		

							}//if (tipo_restricao_RHQ == "QDEF" || tipo_restricao_RHQ == "QDES") {
							else if (tipo_restricao_RHQ == "QBOM") {

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 4 -  Número da usina hidrelétrica, conforme campo 2 do registro UH
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(14, 3);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const int usina_elevatoria_codigo_usina = atoi(atributo.c_str());

								const IdUsinaElevatoria idUsinaElevatoria_inicializado = getIdFromCodigoONS(lista_codigo_ONS_usina_elevatoria, usina_elevatoria_codigo_usina);

								if (idUsinaElevatoria_inicializado == IdUsinaElevatoria_Nenhum)
									throw std::invalid_argument("Nao inicializada idUsinaElevatoria  com codigo_usina_" + atributo);

								const IdUsinaElevatoria  idUsinaElevatoria = idUsinaElevatoria_inicializado;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 6 -  Tipos válidos para a variável na restrição RHQ:
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								const TipoVariavelRestricaoOperativa tipoVariavelRestricaoOperativa = TipoVariavelRestricaoOperativa_vazao_bombeada;

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Guarda informação nos Smart Elementos
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////						

								if (periodo_inicio == a_dados.getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_periodo_inicio, Periodo())) {//Ainda não foi inicializado

									IdElementoSistema idElementoSistema = a_dados.getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());

									if (idElementoSistema == IdElementoSistema_Nenhum) { idElementoSistema = IdElementoSistema_1; }
									else { idElementoSistema++; }

									ElementoSistema elementoSistema;
									elementoSistema.setAtributo(AttComumElementoSistema_idElementoSistema, idElementoSistema);
									elementoSistema.setAtributo(AttComumElementoSistema_tipo_elemento, TipoElementoSistema_usina_elevatoria);
									elementoSistema.setAtributo(AttComumElementoSistema_usina_elevatoria, idUsinaElevatoria);
									elementoSistema.setAtributo(AttComumElementoSistema_tipoVariavelRestricaoOperativa, tipoVariavelRestricaoOperativa);

									for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
										elementoSistema.addElemento(AttVetorElementoSistema_fator_participacao, periodo, fator_participacao);

									a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).vetorElementoSistema.add(elementoSistema);

								}//if (idEstagio_inicial == IdEstagio_1) {
								else {

									const std::vector<IdElementoSistema> idElementoSistema_inicializada = a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).vetorElementoSistema.getIdObjetos(AttComumElementoSistema_usina_elevatoria, idUsinaElevatoria);

									if (int(idElementoSistema_inicializada.size()) == 0)
										throw std::invalid_argument("Elemento do sistema na restrição RHQ nao instanciado");

									for (Periodo periodo = periodo_inicio; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo))
										a_dados.vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).vetorElementoSistema.att(idElementoSistema_inicializada.at(0)).setElemento(AttVetorElementoSistema_fator_participacao, periodo, fator_participacao);

								}//else {		

							}//else if (tipo_restricao_RHQ == "QBOM") {
						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro CQ: \n" + std::string(erro.what())); }

					}//if (menemonico == "CQ") {

					if (menemonico == "DA") {//Desvios de água cujo não atendimento é associado a um custo 

						try {
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Número da usina hidráulica a montante da qual será feita a retirada, conforme campo 2 dos registros UH 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_usina = atoi(atributo.c_str());

							const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Número da usina hidráulica a montante da qual se dará o retorno da água, conforme campo 2 dos registros UH 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(9, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							IdHidreletrica idHidreletrica_jusante_desvio = IdHidreletrica_Nenhum;

							if (atributo != "") {

								const int jusante_desvio_codigo_usina = atoi(atributo.c_str());
								idHidreletrica_jusante_desvio = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, jusante_desvio_codigo_usina);

							}//if (atributo != "") {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 4 -  Número do estágio, em ordem crescente 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(13, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdEstagio idEstagio_inicial = getIdEstagioFromChar(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 5 -  Vazão a ser desviada, em m3/s 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(16, 6);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							double vazao_desviada = atof(atributo.c_str());

							/////////////////////////////
							const int line_size = int(line.length());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 6 -  Retorno em % da vazão desviada
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							double percentual_retorno_do_desvio = 0.0; //default

							if (line_size >= 30) {

								atributo = line.substr(24, 4);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								if (atributo != "")
									percentual_retorno_do_desvio = atof(atributo.c_str()) / 100;

							}//if (line_size >= 30) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 7 -  Custo de não atendimento ao desvio, em $/( hm3).
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							double penalidade_desvio_agua = 0.1; //Default -Ver 3.3.15 Penalidades Observação 1.

							if (line_size >= 45) {

								atributo = line.substr(34, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								if (atributo != "")
									penalidade_desvio_agua = atof(atributo.c_str());

							}//if (line_size >= 45) {

							////////////////////////
							if (idHidreletrica == idHidreletrica_jusante_desvio) //O retorno da água é para a própria usina
								vazao_desviada *= (1 - percentual_retorno_do_desvio);

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Guarda informação nos Smart Elementos
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							/*
							if (idEstagio_inicial == IdEstagio_1) {


								const IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico(a_dados.getMaiorId(IdIntercambioHidraulico()) + 1);

								IntercambioHidraulico intercambioHidraulico;
								intercambioHidraulico.setAtributo(AttComumIntercambioHidraulico_idIntercambioHidraulico, idIntercambioHidraulico);

								a_dados.vetorIntercambioHidraulico.add(intercambioHidraulico);

								a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setAtributo(AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico_retirada);

								a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setAtributo(AttComumIntercambioHidraulico_hidreletrica_origem, idHidreletrica);
								a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setAtributo(AttComumIntercambioHidraulico_hidreletrica_destino, idHidreletrica_jusante_desvio);
								//a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setAtributo(AttComumIntercambioHidraulico_penalidade_desvio_agua, penalidade_desvio_agua);
								a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setAtributo(AttComumIntercambioHidraulico_percentual_retorno_do_desvio, percentual_retorno_do_desvio);

								const SmartEnupla<Periodo, double> desvio_agua(horizonte_estudo, vazao_desviada);

								a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setVetor(AttVetorIntercambioHidraulico_desvio_agua_minimo, desvio_agua);
								a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setVetor(AttVetorIntercambioHidraulico_desvio_agua_maximo, desvio_agua);

							}//if (idEstagio_inicial == IdEstagio_1) {
							else {

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {


										//const IdIntercambioHidraulico idIntercambioHidraulico = a_dados.getMaiorId(IdIntercambioHidraulico()); //O último IdIntercambioHidraulico corresponde com o Id do desvio a modificar

										////////////////////////////////////////////////////////////////////
										//Identifica o idIntercambioHidraulico correspondente
										////////////////////////////////////////////////////////////////////

										IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico_Nenhum;

										const std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_inicializado = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_origem, idHidreletrica);

										for (int pos = 0; pos < int(idIntercambioHidraulico_inicializado.size()); pos++) {

											if (a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico_inicializado.at(pos)).getAtributo(AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica()) == idHidreletrica_jusante_desvio) {

												idIntercambioHidraulico = idIntercambioHidraulico_inicializado.at(pos);
												break;

											}//if (a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico_inicializado.at(pos)).getAtributo(AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica()) == idHidreletrica_jusante_desvio) {

										}//for (int pos = 0; pos < int(idIntercambioHidraulico_inicializado.size()); pos++) {

										if (idIntercambioHidraulico == IdIntercambioHidraulico_Nenhum)
											throw std::invalid_argument("Registro DA - IntercambioHidraulico nao identificado");

										////////////////////////////////////////////////////////////////////

										a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setElemento(AttVetorIntercambioHidraulico_desvio_agua_minimo, periodo, vazao_desviada);
										a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setElemento(AttVetorIntercambioHidraulico_desvio_agua_maximo, periodo, vazao_desviada);

									}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							}//else {
							*/

							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial))
									desvio_registro_DA.at(idHidreletrica).setElemento(periodo, vazao_desviada);

							}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro DA: \n" + std::string(erro.what())); }

					}//if (menemonico == "DA") {

					if (menemonico == "AR") {//Aplicação do Mecanismo de Aversão ao Risco CVaR
						/*
						try {
						//Se este registro existe, significa que o problema considera CVaR

						a_dados.setAtributo(AttComumDados_tipo_aversao_a_risco, TipoAversaoRisco_CVAR);

						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						//Campo 2 -  Número do estágio, em ordem crescente
						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						atributo = line.substr(5, 3);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						const IdEstagio idEstagio_inicial = getIdEstagioFromChar(atributo.c_str());

						const int line_size = int(line.length());

						double lambda;
						double alpha;

						if (line_size >= 22) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Valor do lambda (caso este campo seja deixado em branco será utilizado o valor empregado pelo Newave)
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(11, 5);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							if (atributo != "") {
								lambda = atof(atributo.c_str())/100;
							}//if (atributo != "") {
							else
								throw std::invalid_argument("Registro AR-Aversao ao Risco- sem valores do alpha - colocar os mesmos valores do NEWAVE do arquivo CVAR.DAT");

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 4 -  Valor do alpha (caso este campo seja deixado em branco será utilizado o valor empregado pelo Newave)
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(17, 5);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							if (atributo != "") {
								alpha = atof(atributo.c_str()) / 100;
							}//if (atributo != "") {
							else
								throw std::invalid_argument("Registro AR-Aversao ao Risco- sem valores do lambda - colocar os mesmos valores do NEWAVE do arquivo CVAR.DAT");

						}//if (line_size >= 22) {
						else
							throw std::invalid_argument("Registro AR-Aversao ao Risco- sem valores do alpha e lambda - colocar os mesmos valores do NEWAVE do arquivo CVAR.DAT");

						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						//Guarda informação nos Smart Elementos
						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

						const SmartEnupla<IdEstagio, Periodo> horizonte_otimizacao = a_dados.getVetor(AttVetorDados_horizonte_otimizacao, IdEstagio(), Periodo());

						if (idEstagio_inicial == IdEstagio_1) {

							const IdEstagio idEstagio_Maior = horizonte_otimizacao.getIteradorFinal();

							std::vector<double> lambda_vetor(int(idEstagio_Maior), lambda);
							std::vector<double> alpha_vetor(int(idEstagio_Maior), alpha);

							const SmartEnupla<IdEstagio, double> lambda_CVAR(IdEstagio_1, lambda_vetor);
							const SmartEnupla<IdEstagio, double> alpha_CVAR (IdEstagio_1, alpha_vetor);

							a_dados.setVetor(AttVetorDados_lambda_CVAR, lambda_CVAR);
							a_dados.setVetor(AttVetorDados_alpha_CVAR, alpha_CVAR);

						}//if (idEstagio_inicial == IdEstagio_1) {
						else {

							for (IdEstagio idEstagio = horizonte_otimizacao.getIteradorInicial(); idEstagio <= horizonte_otimizacao.getIteradorFinal(); idEstagio++) {

								//Inicialmente, identifica o IdEstagio do SPT com base no periodo do horizonte_otimizacao_DC do IdEstagio DECOMP

								IdEstagio IdEstagio_teste;

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {
										IdEstagio_teste = horizonte_estudo.at(periodo);
										break;
									}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								//Atualiza os dados do lambda e alpha

								if (idEstagio >= IdEstagio_teste) {
									a_dados.setElemento(AttVetorDados_lambda_CVAR, idEstagio, lambda);
									a_dados.setElemento(AttVetorDados_alpha_CVAR, idEstagio, alpha);

								}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_inicial)) {

							}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						}//else {


						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro AR: \n" + std::string(erro.what())); }
						*/
					}//if (menemonico == "AR") {

					if (menemonico == "EV") {//Evaporação

						try {
							TipoModeloEvaporacao tipo_modelo_evaporacao = TipoModeloEvaporacao_tradicional_fora_do_PL;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Modelo para cálculo da vazão evaporada:   0-modelo tradicional por fora do PL;   1-modelo linear. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(5, 1);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int modelo = atoi(atributo.c_str());

							/////////////////////////////////////////

							const int line_size = int(line.length());

							if (modelo == 1 && line_size >= 12) {

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Campo 3 -  Para o caso do campo 2 ser igual a “1”, deve-se informar o volume de referência a ser utilizado:   INI – Utiliza o volume inicial como volume de referência;  MED – Utiliza o volume útil médio como referência
								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								atributo = line.substr(9, 3);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								/*
								if (atributo == "INI")
									tipo_modelo_evaporacao = TipoModeloEvaporacao_lineal_volume_inicial;
								else if (atributo == "MED")
									tipo_modelo_evaporacao = TipoModeloEvaporacao_lineal_volume_medio;

															*/

							}//if (modelo == 1 && line_size >= 12) {

							//a_dados.setAtributo(AttComumDados_tipo_modelo_evaporacao, tipo_modelo_evaporacao);
						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro EV: \n" + std::string(erro.what())); }

					}//if (menemonico == "EV") {

					if (menemonico == "RQ") {//Vazão defluente mínima histórica

						try {

							if (inicializa_vetor_porcentagem_vazao_minima_historica_REE) {

								for (int ree = 1; ree <= maior_ONS_REE; ree++)
									porcentagem_vazao_minima_historica_REE.addElemento(ree, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

								inicializa_vetor_porcentagem_vazao_minima_historica_REE = false;
							}//if (inicializa_vetor_porcentagem_vazao_minima_historica_REE) {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 - Número do Reservatório equivalente. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_ONS_REE = atoi(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 - Percentual da vazão mínima histórica para cada estágio, em %. Valor default = 80 %
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							const int line_size = int(line.length());

							const int maiorEstagio_DC = int(horizonte_otimizacao_DC.size());

							for (int estagio_DC = 0; estagio_DC < maiorEstagio_DC; estagio_DC++) {

								double percentual_vazao_defluente_minima_historica = 0.8; //default

								if (line_size >= (14 + 5 * estagio_DC)) {

									atributo = line.substr(9 + 5 * estagio_DC, 5);
									atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

									if (atributo != "")
										percentual_vazao_defluente_minima_historica = atof(atributo.c_str()) / 100;

								}//if (line_size >= (14 + 5 * estagio_DC)) {

								bool sobreposicao_encontrada = false;

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									double sobreposicao = periodo.sobreposicao(horizonte_otimizacao_DC.getElemento(IdEstagio(estagio_DC + 1)));

									if (sobreposicao == 1) {
										porcentagem_vazao_minima_historica_REE.at(codigo_ONS_REE).setElemento(periodo, percentual_vazao_defluente_minima_historica);
										sobreposicao_encontrada = true;
									}

									if (sobreposicao == 0 && sobreposicao_encontrada)
										break;

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							}//for (int estagio_DC = 1; estagio_DC <= maiorEstagio_DC; estagio_DC++) {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro RQ: \n" + std::string(erro.what())); }

					}//if (menemonico == "RQ") {


				}//if (teste_comentario != COMENTARIO) {

			}// while (std::getline(leituraArquivo, line))

			leituraArquivo.clear();
			leituraArquivo.close();

			//Faz a soma dos desvios dos registros TI + DA
			for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica++) {

				bool vetor_vazo_retirado_inicializado = false;
				if (a_dados.vetorHidreletrica.att(idHidreletrica).getSizeVetor(AttVetorHidreletrica_vazao_retirada) > 0)
					vetor_vazo_retirado_inicializado = true;

				for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

					if (vetor_vazo_retirado_inicializado) {
						double vazao_retirada = a_dados.vetorHidreletrica.att(idHidreletrica).getElementoVetor(AttVetorHidreletrica_vazao_retirada, periodo, double());
						vazao_retirada += desvio_registro_DA.at(idHidreletrica).getElemento(periodo);

						a_dados.vetorHidreletrica.att(idHidreletrica).setElemento(AttVetorHidreletrica_vazao_retirada, periodo, vazao_retirada);

					}//if (vetor_vazo_retirado_inicializado) {
					else {

						double vazao_retirada = desvio_registro_DA.at(idHidreletrica).getElemento(periodo);
						a_dados.vetorHidreletrica.att(idHidreletrica).addElemento(AttVetorHidreletrica_vazao_retirada, periodo, vazao_retirada);

					}//else {

				}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

			}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica++) {

		} // if (leituraArquivo.is_open()) 

		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	} //try 
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_DECOMP_29_DADGER: \n" + std::string(erro.what())); }

}


void LeituraCEPEL::leitura_DADGNL_201906_DC29_A(Dados& a_dados, std::string nomeArquivo, std::string nomeArquivo_pastaRaiz_relgnl, std::string nomeArquivo_pastaAdicionais_relgnl) {
	try {

		std::ifstream leituraArquivo(nomeArquivo);

		if (leituraArquivo.is_open()) {

			std::string line;
			std::string atributo;

			SmartEnupla<IdTermeletrica, SmartEnupla<Periodo, double>> lista_termeletrica_potencia_pre_comandada(IdTermeletrica_1, std::vector<SmartEnupla<Periodo, double>>(IdTermeletrica_300, SmartEnupla<Periodo, double>()));

			//Informaçao necessária para ler arquivo relgnl.rvX (caso seja necessário)
			std::vector<int> codigo_gnl;
			std::vector<std::string> nome_gnl;

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);

				if (line.substr(0, 1) != "&") {

					std::string menemonico = line.substr(0, 2);

					if (menemonico == "TG") {//

						try {

							IdTermeletrica idTermeletrica;

							const int codigo_usina = std::stoi(line.substr(4, 3));

							const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, std::stoi(line.substr(9, 2)));

							if (idSubmercado == IdSubmercado_Nenhum)
								throw std::invalid_argument("Submercado nao instanciado com codigo_usina_" + atributo);

							const string nome = line.substr(14, 10);

							//Campo 5 -   Índice do estágio. 	
							const IdEstagio idEstagio_termeletrica = IdEstagio(std::stoi(line.substr(24, 2)));

							if (idEstagio_termeletrica == IdEstagio_1) { //termelétrica não inicializada

								idTermeletrica = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, codigo_usina);

								if (idTermeletrica == IdTermeletrica_Nenhum) {
									idTermeletrica = IdTermeletrica(a_dados.getMaiorId(IdTermeletrica()) + 1);
									Termeletrica termeletrica;
									termeletrica.setAtributo(AttComumTermeletrica_idTermeletrica, idTermeletrica);
									termeletrica.setAtributo(AttComumTermeletrica_codigo_usina, codigo_usina);
									termeletrica.setAtributo(AttComumTermeletrica_nome, nome);
									a_dados.vetorTermeletrica.add(termeletrica);
									lista_codigo_ONS_termeletrica.addElemento(idTermeletrica, codigo_usina);
								}

								a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_submercado, idSubmercado);
								a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_nome, nome);
								a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_considerar_usina, true);
								a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica_por_usina);

								codigo_gnl.push_back(codigo_usina);
								nome_gnl.push_back(nome);

							} // if (idEstagio_termeletrica == IdEstagio_1) {

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro TG: \n" + std::string(erro.what())); }

					}//if (menemonico == "TG") {

					if (menemonico == "GS") {

						try {

							//Filosofia: Carregar o número de semanas de cada mês do estudo (até um mês na frente) e ao finalizar a leitura de todos os registros criar um SmartEnupla

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Índice do mês. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(4, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							int mes = atoi(atributo.c_str());

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -  Número de intervalos de tempo do referido mês.
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							atributo = line.substr(9, 1);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int numero_semanas_no_mes = atoi(atributo.c_str());

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro GS: \n" + std::string(erro.what())); }

					}//if (menemonico == "GS") {

					if (menemonico == "NL") {//Lag de antecipação de despacho das usinas térmicas GNL

						try {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Número da usina térmica
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//codigo_usina
							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_usina = atoi(atributo.c_str());

							//Identifica se alguma termelétrica tem sido inicializada com codigo_usina
							const IdTermeletrica idTermeletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, codigo_usina);

							if (idTermeletrica_inicializado == IdTermeletrica_Nenhum)
								throw std::invalid_argument("Nao inicializada idTermeletrica com codigo_usina_" + atributo);

							const IdTermeletrica idTermeletrica = idTermeletrica_inicializado;

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 3 -   Índice do subsistema ao qual pertence a usina. 
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							atributo = line.substr(9, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							//Identifica se alguma termelétrica tem sido inicializada com codigo_usina

							const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, atoi(atributo.c_str()));

							if (idSubmercado == IdSubmercado_Nenhum)
								throw std::invalid_argument("Submercado nao instanciado com codigo_usina_" + atributo);


							//Campo 4 -  Lag de antecipação de despacho
							a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada, std::stoi(line.substr(14, 1)));

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro NL: \n" + std::string(erro.what())); }

					}//if (menemonico == "NL") {

					if (menemonico == "GL") {//Gerações já comandadas de usinas térmicas GNL 

						try {

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Campo 2 -  Número da usina térmica
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//codigo_usina
							atributo = line.substr(4, 3);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_usina = atoi(atributo.c_str());

							//Identifica se alguma termelétrica tem sido inicializada com codigo_usina
							const IdTermeletrica idTermeletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, codigo_usina);

							if (idTermeletrica_inicializado == IdTermeletrica_Nenhum)
								throw std::invalid_argument("Nao inicializada idTermeletrica com codigo_usina_" + atributo);

							const IdTermeletrica idTermeletrica = idTermeletrica_inicializado;

							const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, std::stoi(line.substr(9, 2)));

							if (idSubmercado == IdSubmercado_Nenhum)
								throw std::invalid_argument("Submercado nao instanciado com codigo_usina_" + atributo);

							const int semana_comandada = std::stoi(line.substr(14, 2));

							////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							// Campos 5-X - Geração comandada, em MWmed, por patamar e Dia de início da semana operativa / Mês de início da semana operativa / Ano de início da semana operativa
							// Manual: Os campos "dia", "mês" e "ano de início da semana operativa" será o campo imediatamente após o campo de duração do último patamar
							////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_dados.getIterador1Inicial(AttMatrizDados_percentual_duracao_patamar_carga, Periodo()), IdPatamarCarga());

							const int numero_patamares = int(maiorIdPatamarCarga);

							///////////////////////////////////////
							// Campos "dia", "mês" e "ano 
							///////////////////////////////////////					

							Periodo periodo_comandado(TipoPeriodo_semanal, std::stoi(line.substr(20 + 15 * numero_patamares, 2)), std::stoi(line.substr(22 + 15 * numero_patamares, 2)), \
								std::stoi(line.substr(24 + 15 * numero_patamares, 4)));

							double potencia_pre_comandada = 0.0;

							double numero_horas_total = 0.0;

							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

								atributo = line.substr(19 + 15 * (int(idPatamarCarga) - 1), 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const double potencia = atof(atributo.c_str());

								atributo = line.substr(29 + 15 * (int(idPatamarCarga) - 1), 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								double horas_patamar = atof(atributo.c_str());

								if (horas_patamar == 0.0)
									horas_patamar = 168.0 / double(numero_patamares);

								potencia_pre_comandada += potencia * horas_patamar / 168.0;

								numero_horas_total += horas_patamar;

							} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++)

							if (numero_horas_total < 168.0)
								potencia_pre_comandada /= (numero_horas_total / 168.0);

							lista_termeletrica_potencia_pre_comandada.at(idTermeletrica).addElemento(periodo_comandado, potencia_pre_comandada);

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro GL: \n" + std::string(erro.what())); }

					}//if (menemonico == "GL") {

				}//if (teste_comentario != "&") {

			}//while (std::getline(leituraArquivo, line)) {

			leituraArquivo.clear();
			leituraArquivo.close();

			//Carrega potência comandada do 3º mês se existir o arquivo saída DC "relgnl.rvX"
			leitura_potencia_comandada_from_relgnl(a_dados, nomeArquivo_pastaRaiz_relgnl, nomeArquivo_pastaAdicionais_relgnl, codigo_gnl, nome_gnl, lista_termeletrica_potencia_pre_comandada);

			for (IdTermeletrica idTermeletrica = IdTermeletrica_1; idTermeletrica <= lista_termeletrica_potencia_pre_comandada.getIteradorFinal(); idTermeletrica++) {

				if (lista_termeletrica_potencia_pre_comandada.at(idTermeletrica).size() > 0) {

					if (a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada, int()) == 0)
						throw std::invalid_argument("O atributo " + getFullString(AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada) + " deve ser maior que 0 para a leitura de valores de potencia disponivel pre programada.");

					a_dados.vetorTermeletrica.att(idTermeletrica).setVetor(AttVetorTermeletrica_potencia_disponivel_pre_comandada, lista_termeletrica_potencia_pre_comandada.at(idTermeletrica));

				} // if (lista_termeletrica_potencia_pre_comandada.at(idTermeletrica).size() > 0) {

			} // for (IdTermeletrica idTermeletrica = IdTermeletrica_1; idTermeletrica <= lista_termeletrica_potencia_comandada.getIteradorFinal(); idTermeletrica++) {

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_DADGNL_201906_DC29_A: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_DADGNL_201906_DC29_B(Dados& a_dados, const std::string a_diretorio, const std::string nomeArquivo) {
	try {

		std::ifstream leituraArquivo(a_diretorio + "\\" + nomeArquivo);

		if (leituraArquivo.is_open()) {

			std::string line;
			std::string atributo;

			SmartEnupla<IdTermeletrica, SmartEnupla<Periodo, double>> lista_termeletrica_potencia_comandada_minima(IdTermeletrica_1, std::vector<SmartEnupla<Periodo, double>>(a_dados.getMaiorId(IdTermeletrica()), SmartEnupla<Periodo, double>()));
			SmartEnupla<IdTermeletrica, SmartEnupla<Periodo, double>> lista_termeletrica_potencia_comandada_maxima(IdTermeletrica_1, std::vector<SmartEnupla<Periodo, double>>(a_dados.getMaiorId(IdTermeletrica()), SmartEnupla<Periodo, double>()));

			const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

			for (IdTermeletrica idTermeletrica = IdTermeletrica_1; idTermeletrica <= a_dados.getMaiorId(IdTermeletrica()); idTermeletrica++) {

				const int lag_mensal_potencia_disponivel_comandada = a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada, int());

				if (lag_mensal_potencia_disponivel_comandada > 0) {

					const Periodo periodo_final_pre_comandado = a_dados.getIteradorFinal(idTermeletrica, AttVetorTermeletrica_potencia_disponivel_pre_comandada, Periodo());

					if ((periodo_final_pre_comandado.getHora() != IdHor_0) && (periodo_final_pre_comandado.getMinuto() != IdMin_0))
						throw std::invalid_argument("Periodo final de " + getFullString(AttVetorTermeletrica_potencia_disponivel_pre_comandada) + " deve iniciar em 0Horas e 0Minutos.");

					const Periodo periodo_final_DC = horizonte_otimizacao_DC.at(horizonte_otimizacao_DC.getIteradorFinal());

					const Periodo periodo_mensal_inicial_comandado = Periodo(Periodo(periodo_final_pre_comandado + 1).getMes(), Periodo(periodo_final_pre_comandado + 1).getAno());

					const Periodo periodo_mensal_final_comandado = Periodo(TipoPeriodo_mensal, periodo_final_DC + 1) + lag_mensal_potencia_disponivel_comandada - 1;

					if (periodo_mensal_inicial_comandado <= periodo_mensal_final_comandado) {

						double sobreposicao = 0.0;

						Periodo periodo_interface;

						for (periodo_interface = periodo_mensal_inicial_comandado; periodo_interface <= periodo_mensal_final_comandado; periodo_interface++) {

							sobreposicao = periodo_interface.sobreposicao(periodo_final_pre_comandado);

							if (sobreposicao > 0.0)
								break;

						} // for (Periodo periodo = periodo_mensal_inicial_comandado; periodo <= periodo_mensal_final_comandado; periodo++) {

						if ((sobreposicao == 0.0) && (Periodo(TipoPeriodo_mensal, periodo_final_pre_comandado + 1) != periodo_mensal_inicial_comandado) && (periodo_final_pre_comandado < periodo_mensal_final_comandado))
							throw std::invalid_argument("Periodo final de " + getFullString(AttVetorTermeletrica_potencia_disponivel_pre_comandada) + " nao compativel com periodo a ser comandada.");

						Periodo periodo_inicial_comandado;
						Periodo periodo_final_comandado;

						if ((sobreposicao == 0.0) && (Periodo(TipoPeriodo_mensal, periodo_final_pre_comandado + 1) == periodo_mensal_inicial_comandado)) {
							periodo_inicial_comandado = periodo_mensal_inicial_comandado;
							periodo_final_comandado = periodo_mensal_final_comandado;
						}

						else if (sobreposicao > 0.0) {

							periodo_inicial_comandado = Periodo(TipoPeriodo_diario, periodo_final_pre_comandado + 1);

							Periodo periodo_diario = periodo_inicial_comandado;

							int cont = 0;
							while (true) {
								if (periodo_diario.getMes() != periodo_inicial_comandado.getMes())
									break;
								periodo_diario++;
								cont++;
							} // while (true) {

							periodo_inicial_comandado = Periodo(TipoPeriodo(TipoPeriodo_diario - cont + 1), periodo_inicial_comandado);

							if (periodo_mensal_inicial_comandado == periodo_mensal_final_comandado)
								periodo_final_comandado = periodo_inicial_comandado;
							else
								periodo_final_comandado = periodo_mensal_final_comandado;

						} // else if (sobreposicao > 0.0) {

						if (periodo_inicial_comandado.isValido()) {

							lista_termeletrica_potencia_comandada_minima.at(idTermeletrica).addElemento(periodo_inicial_comandado, NAN);
							lista_termeletrica_potencia_comandada_maxima.at(idTermeletrica).addElemento(periodo_inicial_comandado, NAN);

							if (periodo_inicial_comandado != periodo_final_comandado) {
								for (Periodo periodo = Periodo(TipoPeriodo_mensal, periodo_inicial_comandado + 1); periodo <= periodo_final_comandado; periodo++) {
									lista_termeletrica_potencia_comandada_minima.at(idTermeletrica).addElemento(periodo, NAN);
									lista_termeletrica_potencia_comandada_maxima.at(idTermeletrica).addElemento(periodo, NAN);
								} // for (Periodo periodo = Periodo(TipoPeriodo_mensal, periodo_inicial_comandado + 1); periodo <= periodo_final_comandado; periodo++) {
							} // if (periodo_inicial_comandado != periodo_final_comandado) {

						} // if (periodo_inicial_comandado.isValido()) {

					} // if (periodo_mensal_inicial_comandado <= periodo_mensal_final_comandado) {

				} // if (lag_mensal_potencia_disponivel_comandada > 0) {

			} // for (IdTermeletrica idTermeletrica = IdTermeletrica_1; idTermeletrica <= a_dados.getMaiorId(IdTermeletrica()); idTermeletrica++) {

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);

				if (line.substr(0, 1) != "&") {

					std::string menemonico = line.substr(0, 2);

					if (menemonico == "TG") {//Usinas térmicas

						try {

							IdTermeletrica idTermeletrica;

							const int codigo_usina = std::stoi(line.substr(4, 3));

							const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, std::stoi(line.substr(9, 2)));

							if (idSubmercado == IdSubmercado_Nenhum)
								throw std::invalid_argument("Submercado nao instanciado com codigo_usina_" + atributo);

							const string nome = line.substr(14, 10);

							//Campo 5 -   Índice do estágio. 	
							const IdEstagio idEstagio_termeletrica = IdEstagio(std::stoi(line.substr(24, 2)));

							const int line_size = int(line.length());
							int numero_patamares = 0;

							//Vetores com informação por patamar
							std::vector<double> potencia_minima;
							std::vector<double> potencia_maxima;
							std::vector<double> custo_operacao;

							//Campos 6 - 7 - 8  
							if (line_size >= 49) {//Por cada patamar são feito 3 registros: potencia_minima, potencia_maxima, custo_operacao
								numero_patamares++;
								potencia_minima.push_back(std::stod(line.substr(29, 5)));
								potencia_maxima.push_back(std::stod(line.substr(34, 5)));
								custo_operacao.push_back(std::stod(line.substr(39, 10)));
							}//if (line_size >= 49) {

							//Campos 9 - 10 - 11 
							if (line_size >= 69) {
								numero_patamares++;
								potencia_minima.push_back(std::stod(line.substr(49, 5)));
								potencia_maxima.push_back(std::stod(line.substr(54, 5)));
								custo_operacao.push_back(std::stod(line.substr(59, 10)));
							}//if (line_size >= 69) {

							//Campos 12 - 13 - 14
							if (line_size >= 89) {
								numero_patamares++;
								potencia_minima.push_back(std::stod(line.substr(69, 5)));
								potencia_maxima.push_back(std::stod(line.substr(74, 5)));
								custo_operacao.push_back(std::stod(line.substr(79, 10)));
							}//if (line_size >= 89) {

							//Campos 15 - 16 - 17 
							if (line_size >= 109) {
								numero_patamares++;
								potencia_minima.push_back(std::stod(line.substr(89, 5)));
								potencia_maxima.push_back(std::stod(line.substr(94, 5)));
								custo_operacao.push_back(std::stod(line.substr(99, 10)));
							}//if (line_size >= 109) {

							//Campos 18 - 19 - 20
							if (line_size >= 129) {
								numero_patamares++;
								potencia_minima.push_back(std::stod(line.substr(109, 5)));
								potencia_maxima.push_back(std::stod(line.substr(114, 5)));
								custo_operacao.push_back(std::stod(line.substr(119, 10)));
							}//if (line_size >= 129) {

							/////////////////////////////////////////
							//Guarda a informação nos SmartElementos
							/////////////////////////////////////////

							//Filosofia: A informação registrada é válida para o idEstagio >= idEstagio_leitura: No primeiro estágio preenche todos 
							//           os valores dos estágios restantes e logo é sobreescrita a informação para idEstágio > 1

							if (idEstagio_termeletrica == IdEstagio_1) { //termelétrica não inicializada

								idTermeletrica = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, codigo_usina);

								//Cria vetores potencia_minima, potencia_maxima, custo_de_operacao

								const SmartEnupla<IdPatamarCarga, double> vetor_termeletrica_potencia_minima(IdPatamarCarga_1, potencia_minima);
								const SmartEnupla<IdPatamarCarga, double> vetor_termeletrica_potencia_maxima(IdPatamarCarga_1, potencia_maxima);
								const SmartEnupla<IdPatamarCarga, double> vetor_termeletrica_custo_de_operacao(IdPatamarCarga_1, custo_operacao);

								SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_termeletrica_potencia_minima(horizonte_estudo, vetor_termeletrica_potencia_minima);
								SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_termeletrica_potencia_maxima(horizonte_estudo, vetor_termeletrica_potencia_maxima);
								SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_termeletrica_custo_de_operacao(horizonte_estudo, vetor_termeletrica_custo_de_operacao);

								a_dados.vetorTermeletrica.att(idTermeletrica).setMatriz(AttMatrizTermeletrica_custo_de_operacao, matriz_termeletrica_custo_de_operacao);

								//Inicializa uma unidade termelétrica (necessário para a validação das termelétricas)
								UnidadeUTE unidadeUTE;
								unidadeUTE.setAtributo(AttComumUnidadeUTE_idUnidadeUTE, IdUnidadeUTE_1);
								unidadeUTE.setMatriz(AttMatrizUnidadeUTE_potencia_minima, matriz_termeletrica_potencia_minima);
								unidadeUTE.setMatriz(AttMatrizUnidadeUTE_potencia_maxima, matriz_termeletrica_potencia_maxima);
								a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.add(unidadeUTE);

								a_dados.vetorTermeletrica.att(idTermeletrica).setMatriz(AttMatrizTermeletrica_potencia_disponivel_minima, matriz_termeletrica_potencia_minima);
								a_dados.vetorTermeletrica.att(idTermeletrica).setMatriz(AttMatrizTermeletrica_potencia_disponivel_maxima, matriz_termeletrica_potencia_maxima);

							}//if (idEstagio_termeletrica == IdEstagio_1) {
							else { //termelétrica já inicializada

								//Identifica se alguma termelétrica tem sido inicializada com codigo_usina
								const IdTermeletrica idTermeletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, codigo_usina);

								if (idTermeletrica_inicializado == IdTermeletrica_Nenhum)
									throw std::invalid_argument("Nao inicializada idTermeletrica com codigo_usina_" + getString(codigo_usina));

								idTermeletrica = idTermeletrica_inicializado;

								//***********************************************************************************************************************
								//No arquivo DADGNL, estágio refere-se a o número da semana dentro do horizonte de planejamento, por tanto, a atualização 
								//dos dados das termelétricas é realizado por meio da comparação da data correspondente à semana reportada
								//***********************************************************************************************************************

								Periodo periodo_teste = data_execucao + 1;

								for (IdEstagio idEstagio = IdEstagio_1; idEstagio < idEstagio_termeletrica; idEstagio++) {

									//Aumenta a data até contar (idEstagio-1) semanas = data do inicio da semana ao qual corresponde a info
									for (int conteio = 0; conteio < 7; conteio++)
										periodo_teste = periodo_teste + 1; //periodo_teste é um periodo diário e o aumento é por dia

								}//for (IdEstagio idEstagio = IdEstagio_1; idEstagio < idEstagio_termeletrica; idEstagio++) {


								if (a_dados.getSize1Matriz(idTermeletrica, IdUnidadeUTE_1, AttMatrizUnidadeUTE_potencia_minima) == 0) {
									const SmartEnupla<IdPatamarCarga, double> potenciaZero(IdPatamarCarga_1, std::vector<double>(numero_patamares, a_dados.getAtributo(idTermeletrica, IdUnidadeUTE_1, AttComumUnidadeUTE_potencia_minima, double())));
									a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).setMatriz(AttMatrizUnidadeUTE_potencia_minima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo, potenciaZero));
								}

								if (a_dados.getSize1Matriz(idTermeletrica, IdUnidadeUTE_1, AttMatrizUnidadeUTE_potencia_maxima) == 0) {
									const SmartEnupla<IdPatamarCarga, double> potenciaZero(IdPatamarCarga_1, std::vector<double>(numero_patamares, a_dados.getAtributo(idTermeletrica, IdUnidadeUTE_1, AttComumUnidadeUTE_potencia_maxima, double())));
									a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).setMatriz(AttMatrizUnidadeUTE_potencia_maxima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo, potenciaZero));
								}

								if (a_dados.getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_minima) == 0) {
									const SmartEnupla<IdPatamarCarga, double> potenciaZero(IdPatamarCarga_1, std::vector<double>(numero_patamares, a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_potencia_minima, double())));
									a_dados.vetorTermeletrica.att(idTermeletrica).setMatriz(AttMatrizTermeletrica_potencia_disponivel_minima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo, potenciaZero));
								}

								if (a_dados.getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_maxima) == 0) {
									const SmartEnupla<IdPatamarCarga, double> potenciaZero(IdPatamarCarga_1, std::vector<double>(numero_patamares, a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_potencia_maxima, double())));
									a_dados.vetorTermeletrica.att(idTermeletrica).setMatriz(AttMatrizTermeletrica_potencia_disponivel_maxima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo, potenciaZero));
								}

								for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

									if (periodo >= periodo_teste) {

										for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

											a_dados.vetorTermeletrica.att(idTermeletrica).setElemento(AttMatrizTermeletrica_potencia_disponivel_minima, periodo, idPatamarCarga, potencia_minima.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));
											a_dados.vetorTermeletrica.att(idTermeletrica).setElemento(AttMatrizTermeletrica_potencia_disponivel_maxima, periodo, idPatamarCarga, potencia_maxima.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));

											a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).setElemento(AttMatrizUnidadeUTE_potencia_minima, periodo, idPatamarCarga, potencia_minima.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));
											a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).setElemento(AttMatrizUnidadeUTE_potencia_maxima, periodo, idPatamarCarga, potencia_maxima.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));
											a_dados.vetorTermeletrica.att(idTermeletrica).setElemento(AttMatrizTermeletrica_custo_de_operacao, periodo, idPatamarCarga, custo_operacao.at(getintFromChar(getString(idPatamarCarga).c_str()) - 1));

										}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIdPatamarCargaFromChar(getString(numero_patamares).c_str()); idPatamarCarga++) {

									}//if (periodo >= horizonte_otimizacao_DC.at(idEstagio_termeletrica)) {

								}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							}//else {

							if (lista_termeletrica_potencia_comandada_minima.at(idTermeletrica).size() > 0) {

								SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> horizonte_comandado(lista_termeletrica_potencia_comandada_minima.at(idTermeletrica), SmartEnupla<IdPatamarCarga, double>());

								leitura_PATAMAR_201908_NW25_em_DADGNL(a_diretorio, "PATAMAR.DAT", horizonte_comandado);

								Periodo periodo_inicio_alteracao_comandado;

								if (idEstagio_termeletrica <= horizonte_otimizacao_DC.getIteradorFinal())
									periodo_inicio_alteracao_comandado = horizonte_comandado.getIteradorInicial();

								else {
									Periodo periodo_inicio_comando_mensal = Periodo(TipoPeriodo_mensal, horizonte_otimizacao_DC.at(horizonte_otimizacao_DC.getIteradorFinal()) + 1) + int(idEstagio_termeletrica - horizonte_otimizacao_DC.getIteradorFinal() - 1);

									for (Periodo periodo = horizonte_comandado.getIteradorInicial(); periodo <= horizonte_comandado.getIteradorFinal(); horizonte_comandado.incrementarIterador(periodo)) {
										if (periodo.sobreposicao(periodo_inicio_comando_mensal) > 0.0) {
											periodo_inicio_alteracao_comandado = periodo;
											break;
										}
									} // for (Periodo periodo = horizonte_comandado.getIteradorInicial(); periodo <= horizonte_comandado.getIteradorFinal(); horizonte_comandado.incrementarIterador(periodo)) {

									if (!periodo_inicio_alteracao_comandado.isValido())
										periodo_inicio_alteracao_comandado = horizonte_comandado.getIteradorFinal();

								} // else {

								for (Periodo periodo = periodo_inicio_alteracao_comandado; periodo <= horizonte_comandado.getIteradorFinal(); horizonte_comandado.incrementarIterador(periodo)) {

									double potencia_disponivel_minima = 0.0;
									double potencia_disponivel_maxima = 0.0;

									for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= horizonte_comandado.at(periodo).getIteradorFinal(); idPatamarCarga++) {
										potencia_disponivel_minima += potencia_minima.at(int(idPatamarCarga) - 1) * horizonte_comandado.at(periodo).at(idPatamarCarga) * double(periodo.getHoras()) / double(Periodo(periodo.getMes(), periodo.getAno()).getHoras());
										potencia_disponivel_maxima += potencia_maxima.at(int(idPatamarCarga) - 1) * horizonte_comandado.at(periodo).at(idPatamarCarga) * double(periodo.getHoras()) / double(Periodo(periodo.getMes(), periodo.getAno()).getHoras());
									} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= horizonte_maximo_comandado.at(periodo).getIteradorFinal(); idPatamarCarga++) {

									lista_termeletrica_potencia_comandada_minima.at(idTermeletrica).setElemento(periodo, potencia_disponivel_minima);
									lista_termeletrica_potencia_comandada_maxima.at(idTermeletrica).setElemento(periodo, potencia_disponivel_maxima);

								} // for (Periodo periodo = periodo_inicio_alteracao_comandado; periodo <= horizonte_comandado.getIteradorFinal(); horizonte_comandado.incrementarIterador(periodo)) {

							} // if (lista_termeletrica_potencia_comandada_minima.at(idTermeletrica).size() > 0){

						}//try {
						catch (const std::exception& erro) { throw std::invalid_argument("Erro Registro TG: \n" + std::string(erro.what())); }

					}//if (menemonico == "TG") {

					if (menemonico == "GS") {

					}//if (menemonico == "GS") {

					if (menemonico == "NL") {//Lag de antecipação de despacho das usinas térmicas GNL

					}//if (menemonico == "NL") {

					if (menemonico == "GL") {//Gerações já comandadas de usinas térmicas GNL 

					}//if (menemonico == "GL") {

				}//if (teste_comentario != "&") {

			}//while (std::getline(leituraArquivo, line)) {

			leituraArquivo.clear();
			leituraArquivo.close();

			for (IdTermeletrica idTermeletrica = IdTermeletrica_1; idTermeletrica <= lista_termeletrica_potencia_comandada_minima.getIteradorFinal(); idTermeletrica++) {

				if (lista_termeletrica_potencia_comandada_minima.at(idTermeletrica).size() > 0) {
					a_dados.vetorTermeletrica.att(idTermeletrica).setVetor(AttVetorTermeletrica_potencia_disponivel_comandada_minima, lista_termeletrica_potencia_comandada_minima.at(idTermeletrica));
					a_dados.vetorTermeletrica.att(idTermeletrica).setVetor(AttVetorTermeletrica_potencia_disponivel_comandada_maxima, lista_termeletrica_potencia_comandada_maxima.at(idTermeletrica));
				} // if (lista_termeletrica_potencia_comandada_minima.at(idTermeletrica).size() > 0) {

				//
				// Anula custo de operação em caso de potencia_disponivel_pre_comandada
				//
				/*
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
				*/
			} // for (IdTermeletrica idTermeletrica = IdTermeletrica_1; idTermeletrica <= lista_termeletrica_potencia_comandada.getIteradorFinal(); idTermeletrica++) {

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_DADGNL_201906_DC29_B: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_PATAMAR_201908_NW25_em_DADGNL(const std::string a_diretorio, const std::string nomeArquivo, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>& a_horizonte_comandado) {

	try {

		std::ifstream leituraArquivo(a_diretorio + "\\" + nomeArquivo);

		////////////////////////////////////////////////////////

		if (leituraArquivo.is_open()) {

			if (a_horizonte_comandado.size() == 0)
				throw std::invalid_argument("O argumento a_horizonte_comandado deve conter elementos.");

			std::string line;

			std::string atributo;

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

			for (Periodo periodo = a_horizonte_comandado.getIteradorInicial(); periodo <= a_horizonte_comandado.getIteradorFinal(); a_horizonte_comandado.incrementarIterador(periodo))
				a_horizonte_comandado.at(periodo) = SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(numero_patamares, 0.0));

			//*******************************************************************************************************************
			//Bloco 2 -  Este bloco pode ser de dois tipos, conforme definido pelo registro 39 do arquivo de dados gerais. 
			//           Se esse registro for preenchido com o valor zero, o bloco será do Tipo 1, e se for preenchido 
			//           com o valor um, será do Tipo 2                     
			//*******************************************************************************************************************

			//Três registros destinados a comentários
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);

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

					for (Periodo periodo = a_horizonte_comandado.getIteradorInicial(); periodo <= a_horizonte_comandado.getIteradorFinal(); a_horizonte_comandado.incrementarIterador(periodo)) {

						double sobreposicao = periodo.sobreposicao(periodo_DECK);

						if (sobreposicao > 0.0)
							a_horizonte_comandado.at(periodo).at(idPatamarCarga) = percentual_duracao_patamar_carga;

					} // for (Periodo periodo = a_horizonte_comandado.getIteradorInicial(); periodo <= a_horizonte_comandado.getIteradorFinal(); a_horizonte_comandado.incrementarIterador(periodo)) {

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

			leituraArquivo.close();

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_PATAMAR_201908_NW25_em_DADGNL: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_potencia_comandada_from_relgnl(Dados& a_dados, std::string a_nomeArquivo_pastaRaiz_relgnl, std::string a_nomeArquivo_pastaAdicionais_relgnl, const std::vector<int> a_codigo_gnl, const std::vector<std::string> a_nome_gnl, SmartEnupla<IdTermeletrica, SmartEnupla<Periodo, double>>& a_lista_termeletrica_potencia_pre_comandada) {
	try {


		std::string line;
		std::string atributo;

		std::ifstream leituraArquivo_1(a_nomeArquivo_pastaRaiz_relgnl);
		std::ifstream leituraArquivo_2(a_nomeArquivo_pastaAdicionais_relgnl);

		if (leituraArquivo_1.is_open() || leituraArquivo_2.is_open()) {

			std::string nomeArquivo_relgnl = a_nomeArquivo_pastaRaiz_relgnl;

			if (leituraArquivo_2.is_open())
				nomeArquivo_relgnl = a_nomeArquivo_pastaAdicionais_relgnl;

			leituraArquivo_1.clear();
			leituraArquivo_1.close();

			leituraArquivo_2.clear();
			leituraArquivo_2.close();

			std::ifstream leituraArquivo(nomeArquivo_relgnl);

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);

				if (line.size() >= 109) {

					if (line.substr(4, 105) == "Sis    Usina     Lag(k)               pat_1            pat_2            pat_3        Custo    Inic semana") {

						while (true) {

							std::getline(leituraArquivo, line);

							strNormalizada(line);

							if (line.size() < 108) //Registro "MENSAL"
								break;

							//Dia
							atributo = line.substr(98, 2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							if (atributo == "") //Registro "MENSAL" pode ter espaços na linha
								break;

							const int dia_data = atoi(atributo.c_str());

							if (dia_data > 0) {//Evita o cabeçalho

								//nome usina (pela estrutura do arquivo o idTermeletrica_registro deve se mapear do nome)
								atributo = line.substr(8, 11);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								IdTermeletrica idTermeletrica_registro = IdTermeletrica_Nenhum;

								for (int pos = 0; pos < int(a_nome_gnl.size()); pos++) {

									string nome = a_nome_gnl.at(pos);
									nome.erase(std::remove(nome.begin(), nome.end(), ' '), nome.end());

									if (nome == atributo) {

										idTermeletrica_registro = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, a_codigo_gnl.at(pos));
										break;
									}// if (nome == atributo) {

								}//for (int pos = 0; pos < int(nome_gnl.size()); pos++) {										

								if (idTermeletrica_registro == IdTermeletrica_Nenhum)
									throw std::invalid_argument("Nao identificada idTermeletrica com nome_ " + atributo);

								////////////////////

								//Mês
								atributo = line.substr(101, 2);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const int mes_data = atoi(atributo.c_str());

								//Ano
								atributo = line.substr(104, 4);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const int ano_data = atoi(atributo.c_str());

								const Periodo periodo_registro = Periodo(TipoPeriodo_semanal, Periodo(dia_data, mes_data, ano_data));

								////////////////////

								const IdPatamarCarga maiorIdPatamarCarga = IdPatamarCarga_3; //Estrutura do arquivo

								double potencia_pre_comandada = 0.0;

								double numero_horas_total = 0.0;

								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

									const double potencia = std::stof(line.substr(36 + (int(idPatamarCarga) - 1) * 17, 8));

									const double numero_horas = std::stof(line.substr(45 + (int(idPatamarCarga) - 1) * 17, 7));

									numero_horas_total += numero_horas;

									potencia_pre_comandada += potencia * numero_horas / 168.0;

								}

								if (numero_horas_total < 168.0)
									potencia_pre_comandada /= (numero_horas_total / 168.0);

								if (a_lista_termeletrica_potencia_pre_comandada.at(idTermeletrica_registro).getIteradorFinal() < periodo_registro)
									a_lista_termeletrica_potencia_pre_comandada.at(idTermeletrica_registro).addElemento(periodo_registro, potencia_pre_comandada);

							}//if (dia_data > 0) {

						}//while (true) {

					}//if (line.substr(4, 105) == "Sis    Usina     Lag(k)               pat_1            pat_2            pat_3        Custo    Inic semana") {

				}//if (line.size() >= 109) {

			}//while (std::getline(leituraArquivo, line)) {

		}//if (leituraArquivo_1.is_open() || leituraArquivo_2.is_open()) {

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_potencia_comandada_from_relgnl: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_PERDAS_201906_DC29(Dados& a_dados, std::string nomeArquivo)
{

	//Filosofia: se todos os campos são 9999 então não carrega nenhum matriz no programa, logo se a matriz tem o iterador_nenhum, significa que tudo é zero

	try {

		std::ifstream leituraArquivo(nomeArquivo);
		std::string line;

		std::string atributo;

		////////////////////////////////////////////////////////

		if (leituraArquivo.is_open()) {

			//******************************
			//Bloco 1 – Usinas hidrelétricas
			//******************************

			///////////////////////////////////////////////////////////////////////////////////////
			//Manual: Cada bloco de dados inicia com dois registros que são ignorados pelo programa
			///////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);
			///////////////////////////////////////////////////////////////////////////////////////

			while (true) {

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Campo 1 -  Número da usina hidrelétrica (conforme campo 2 do registro UH). 
				//Manual:    9999 no campo 1 indica fim do bloco. Este registro é obrigatório.
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				std::getline(leituraArquivo, line);

				strNormalizada(line);

				atributo = line.substr(1, 4);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				if (atributo == "9999")
					break;
				else {
					const int hidreletrica_codigo_usina = atoi(atributo.c_str());

					const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, hidreletrica_codigo_usina);

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 2 -  Número do primeiro patamar de carga
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(8, 1);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					IdPatamarCarga idPatamarCarga = getIdPatamarCargaFromChar(atributo.c_str());

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 3-14 -  Fator de perda (p.u)
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					//Nota: o programa DECOMP considera atualmente somente fatores de perda mensais.No primeiro mês do caso em estudo, 
					//em que os estágios são semanais, o programa atribui para cada semana fatores de perdas constantes e iguais ao do mês em questão.

					const int line_size = int(line.length());

					for (int mes = 0; mes < 12; mes++) {

						if (line_size >= 16 + 6 * mes) {

							atributo = line.substr(11 + 6 * mes, 5);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double fator_de_perda = atof(atributo.c_str());

							//a_dados.vetorHidreletrica.att(idHidreletrica).addElemento(AttMatrizHidreletrica_fator_perda_centro_gravidade_da_carga, getIdMesFromInt(mes + 1), idPatamarCarga, fator_de_perda);

						}//if (line_size >= 16 + 6*mes) {

					}//for (int mes = 0; mes < 12; mes++) {

				}//else {

			}//while(true){

			//******************************
			//Bloco 2 – Usinas  térmicas
			//******************************

			///////////////////////////////////////////////////////////////////////////////////////
			//Manual: Cada bloco de dados inicia com dois registros que são ignorados pelo programa
			///////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);
			///////////////////////////////////////////////////////////////////////////////////////

			while (true) {

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Campo 1 -  Número da usina hidrelétrica (conforme campo 2 do registro UH). 
				//Manual:    9999 no campo 1 indica fim do bloco. Este registro é obrigatório.
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				std::getline(leituraArquivo, line);

				strNormalizada(line);

				atributo = line.substr(1, 4);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				if (atributo == "9999")
					break;
				else {
					const int termeletrica_codigo_usina = atoi(atributo.c_str());

					const IdTermeletrica idTermeletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, termeletrica_codigo_usina);

					if (idTermeletrica_inicializado == IdTermeletrica_Nenhum)
						throw std::invalid_argument("Nao inicializada idTermeletrica com codigo_usina_" + atributo);

					const IdTermeletrica idTermeletrica = idTermeletrica_inicializado;

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 2 -  Número do primeiro patamar de carga
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(8, 1);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					IdPatamarCarga idPatamarCarga = getIdPatamarCargaFromChar(atributo.c_str());

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 3-14 -  Fator de perda (p.u)
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					//Nota: o programa DECOMP considera atualmente somente fatores de perda mensais.No primeiro mês do caso em estudo, 
					//em que os estágios são semanais, o programa atribui para cada semana fatores de perdas constantes e iguais ao do mês em questão.

					const int line_size = int(line.length());

					for (int mes = 0; mes < 12; mes++) {

						if (line_size >= 16 + 6 * mes) {

							atributo = line.substr(11 + 6 * mes, 5);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double fator_de_perda = atof(atributo.c_str());

							//a_dados.vetorTermeletrica.att(idTermeletrica).addElemento(AttMatrizTermeletrica_fator_perda_centro_gravidade_da_carga, getIdMesFromInt(mes + 1), idPatamarCarga, fator_de_perda);

						}//if (line_size >= 16 + 6*mes) {

					}//for (int mes = 0; mes < 12; mes++) {

				}//else {

			}//while(true){

			//*********************************
			//Bloco 3 – Demanda dos Subsistemas
			//*********************************

			///////////////////////////////////////////////////////////////////////////////////////
			//Manual: Cada bloco de dados inicia com dois registros que são ignorados pelo programa
			///////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);
			///////////////////////////////////////////////////////////////////////////////////////

			while (true) {

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Campo 1 -  Número da usina hidrelétrica (conforme campo 2 do registro UH). 
				//Manual:    9999 no campo 1 indica fim do bloco. Este registro é obrigatório.
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				std::getline(leituraArquivo, line);

				strNormalizada(line);

				atributo = line.substr(1, 4);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				if (atributo == "9999")
					break;
				else {
					const int submercado_codigo_usina = atoi(atributo.c_str());

					const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, submercado_codigo_usina);

					if (idSubmercado == IdSubmercado_Nenhum)
						throw std::invalid_argument("Submercado nao instanciado com codigo_usina_" + atributo);

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 2 -  Número do primeiro patamar de carga
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(8, 1);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					IdPatamarCarga idPatamarCarga = getIdPatamarCargaFromChar(atributo.c_str());

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 3-14 -  Fator de perda (p.u)
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					//Nota: o programa DECOMP considera atualmente somente fatores de perda mensais.No primeiro mês do caso em estudo, 
					//em que os estágios são semanais, o programa atribui para cada semana fatores de perdas constantes e iguais ao do mês em questão.

					const int line_size = int(line.length());

					for (int mes = 0; mes < 12; mes++) {

						if (line_size >= 16 + 6 * mes) {

							atributo = line.substr(11 + 6 * mes, 5);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double fator_de_perda = atof(atributo.c_str());

							//a_dados.vetorSubmercado.att(idSubmercado).addElemento(AttMatrizSubmercado_fator_perda_centro_gravidade_da_carga, getIdMesFromInt(mes + 1), idPatamarCarga, fator_de_perda);

						}//if (line_size >= 16 + 6*mes) {

					}//for (int mes = 0; mes < 12; mes++) {

				}//else {

			}//while(true){

			//***************************************
			//Bloco 4 – Intercâmbio entre Subsistemas 
			//***************************************

			///////////////////////////////////////////////////////////////////////////////////////
			//Manual: Cada bloco de dados inicia com dois registros que são ignorados pelo programa
			///////////////////////////////////////////////////////////////////////////////////////
			std::getline(leituraArquivo, line);
			std::getline(leituraArquivo, line);
			///////////////////////////////////////////////////////////////////////////////////////

			while (true) {

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Campo 1 -  Número do subsistema exportador  (conforme campo 1 do registro SB). 
				//Manual:    9999 no campo 1 indica fim do bloco. Este registro é obrigatório.
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				std::getline(leituraArquivo, line);

				strNormalizada(line);

				atributo = line.substr(1, 4);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				if (atributo == "9999")
					break;
				else {
					const int submercado_origem_codigo_usina = atoi(atributo.c_str());

					const IdSubmercado idSubmercado_origem = getIdFromCodigoONS(lista_codigo_ONS_submercado, submercado_origem_codigo_usina);

					if (idSubmercado_origem == IdSubmercado_Nenhum)
						throw std::invalid_argument("Submercado nao instanciado com codigo_usina_" + atributo);


					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 2 -  Número do subsistema importador  (conforme campo 1 do registro SB).
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(5, 4);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const int submercado_destino_codigo_usina = atoi(atributo.c_str());

					const IdSubmercado idSubmercado_destino = getIdFromCodigoONS(lista_codigo_ONS_submercado, submercado_destino_codigo_usina);

					if (idSubmercado_destino == IdSubmercado_Nenhum)
						throw std::invalid_argument("Submercado nao instanciado com codigo_usina_" + atributo);


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

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 2 -  Número do primeiro patamar de carga
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					atributo = line.substr(13, 1);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					IdPatamarCarga idPatamarCarga = getIdPatamarCargaFromChar(atributo.c_str());

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Campo 3-14 -  Fator de perda (p.u)
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					//Nota: o programa DECOMP considera atualmente somente fatores de perda mensais.No primeiro mês do caso em estudo, 
					//em que os estágios são semanais, o programa atribui para cada semana fatores de perdas constantes e iguais ao do mês em questão.

					const int line_size = int(line.length());

					for (int mes = 0; mes < 12; mes++) {

						if (line_size >= 21 + 6 * mes) {

							atributo = line.substr(16 + 6 * mes, 5);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double fator_de_perda = atof(atributo.c_str());

							a_dados.vetorIntercambio.att(idIntercambio).addElemento(AttMatrizIntercambio_fator_perda_centro_gravidade_da_carga, getIdMesFromInt(mes + 1), idPatamarCarga, fator_de_perda);

						}//if (line_size >= 16 + 6*mes) {

					}//for (int mes = 0; mes < 12; mes++) {

				}//else {

			}//while(true){

			leituraArquivo.clear();
			leituraArquivo.close();

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_DECOMP_29_PERDAS: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_MLT_201906_DC29(Dados& a_dados, std::string nomeArquivo) {

	try {

		//ofstream     fp_out;

		//std::string arquivo = "MLT.csv";
		//fp_out.open(arquivo.c_str(), ios_base::out); //Função para criar um arquivo

		const int tamanho = 320;
		int intLeitura[tamanho];

		///////////////////////////////////////////////////////////////////
		std::ifstream leituraArquivo;
		leituraArquivo.open(nomeArquivo, std::ios_base::in | std::ios::binary);

		int mes = 1;

		if (leituraArquivo.is_open()) {

			while (!(leituraArquivo.eof())) {

				leituraArquivo.read((char*)intLeitura, sizeof(intLeitura));

				if (mes <= 12) {//Os arquivos da MLTdos DECKs tem uma linha a mais, por isso esta verificação

					const IdHidreletrica idHidreletrica_MaiorId = a_dados.getMaiorId(IdHidreletrica());

					for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= idHidreletrica_MaiorId; idHidreletrica++) {

						//Leitura do valor de afluência
						const int codigo_usina = lista_codigo_ONS_hidreletrica.at(idHidreletrica);

						const int mlt = intLeitura[codigo_usina - 1];

						//Adiciona o valor de afluencia na matriz da classe Afluencia
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).addElemento(AttVetorAfluencia_media_mensal_longo_termo, getIdMesFromInt(mes), mlt);

					}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= idHidreletrica_MaiorId; idHidreletrica++) {

				}//if (mes <= 12) {

				mes++;


				//Imprime os valores
				//for (int pos = 0; pos < tamanho; pos++) {

					//fp_out << intLeitura[pos] << ";";

				//}//for (int pos = 0; pos < 320; pos++) {

				//fp_out << endl; //Passa de linha


			}//while (!(leituraArquivo.eof())) {

			//fp_out.close();

			leituraArquivo.clear();
			leituraArquivo.close();

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_DECOMP_29_MLT: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_TENDENCIA_VAZOES_MENSAIS_GEVAZP(Dados& a_dados, const std::string nomeArquivo)
{

	try {

		const int numPostos = 320;

		int intLeitura_320[numPostos];

		const int numero_meses_passados_considerados = 11;

		///////////////////////////////////////////////////////////////////
		std::ifstream leituraArquivo;
		leituraArquivo.open(nomeArquivo, std::ios_base::in | std::ios::binary);

		IdMes idMes_historico = IdMes_1;
		IdAno idAno_historico = IdAno_1931;

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		/////////////////////////////////////////////////
		//Inicializa valor_afluencia_passadas	
		/////////////////////////////////////////////////

		for (int posto = 0; posto < numPostos; posto++)
			valor_afluencia_passadas_GEVAZP.addElemento(posto, posto);

		/////////////////////////////////////////////////////////////////////////////////
		//Estabelece o mês inicial a apartir do qual a tendência é considerada (11 meses)
		/////////////////////////////////////////////////////////////////////////////////

		//Nota: O mês do PMO corresponde com o mês do último dia da primeria semana

		Periodo periodo_semana_inicial_PMO = horizonte_otimizacao_DC.getElemento(IdEstagio_1);

		IdDia idDia = periodo_semana_inicial_PMO.getDia();
		IdMes idMes = periodo_semana_inicial_PMO.getMes();
		IdAno idAno = periodo_semana_inicial_PMO.getAno();

		const int numero_dias_periodo_semanal = periodo_semana_inicial_PMO.getDias();

		for (int dia = 1; dia < numero_dias_periodo_semanal; dia++) {//O primeiro dia da semana também conta

			idDia++;

			IdDia maiordiadomes = periodo_semana_inicial_PMO.getMaiorDiaDoMes(idMes);

			if (idDia > maiordiadomes) {
				idDia = IdDia_1;
				idMes++;
				if (idMes == IdMes_Excedente) {
					idMes = IdMes_1;
					idAno++;
				}//if (idMes == IdMes_Excedente) {

			}//if(idDia == maiordiadomes) {

		}//for (int dia = 0; dia < numero_dias_periodo_semanal; dia++) {

		const IdMes idMes_Inicial_PMO = idMes;
		const IdAno idAno_Inicial_PMO = idAno;

		/////////////////////////////////////////////////////////////////////////////////////

		for (int conteio = 0; conteio < numero_meses_passados_considerados; conteio++) {

			idMes--;

			if (idMes == IdMes_Nenhum) {

				idMes = IdMes_12;
				idAno--;

			}//if (idMes == IdMes_Nenhum) {

		}//for (int conteio = 0; conteio < conteio_meses_anteriores; conteio++) {

		const Periodo periodo_limite_para_registrar(idMes, idAno);

		/////////////////////////////////////////////////////////////////////////////////

		if (leituraArquivo.is_open()) {

			int meses_registrados = 0;

			while (!(leituraArquivo.eof())) {

				Periodo periodo_mes_historico(idMes_historico, idAno_historico);

				if (meses_registrados == numero_meses_passados_considerados)
					break;

				idMes_historico++;

				if (idMes_historico > IdMes_12) {
					idMes_historico = IdMes_1;
					idAno_historico++;
				}//if (idMes_historico > IdMes_12) {

				leituraArquivo.read((char*)intLeitura_320, sizeof(intLeitura_320));

				if (periodo_mes_historico >= periodo_limite_para_registrar) {//Somente registra o último ano do histórico

					meses_registrados++;

					if (meses_registrados == numero_meses_passados_considerados) {

						//No mês anterior ao mês inicial do PMO deve encontrar o Tipo_Periodo dependendo dos dias
						//da primeira semana (RV0) do PMO dentro do mês anterior

						////////////////////////////////////////////////////////////////////////
						//1. Determina o dia da primeira semana operativa do PMO no mês anterior 
						////////////////////////////////////////////////////////////////////////

						Periodo periodo_semanal = horizonte_otimizacao_DC.getElemento(IdEstagio_1);

						const Periodo periodo_diario_limite(IdDia_1, idMes_Inicial_PMO, idAno_Inicial_PMO); //Corresponde ao primeiro dia do mês do PMO

						while (periodo_semanal > periodo_diario_limite)
							periodo_semanal--;


						///////////////////////////////////////////////////////////////////////////////////////
						//2. Realiza conteio do número de dias do mês anterior até o periodo_semana_PMO do passo 1.
						///////////////////////////////////////////////////////////////////////////////////////

						IdMes idMes_anterior_PMO = periodo_mes_historico.getMes();
						IdAno idAno_anterior_PMO = periodo_mes_historico.getAno();

						Periodo periodo_diario_mes_anterior(IdDia_1, idMes_anterior_PMO, idAno_anterior_PMO);

						int conteioDias = 0;

						while (periodo_diario_mes_anterior < periodo_semanal) {

							conteioDias++;
							periodo_diario_mes_anterior++;

						}//while (periodo_diario_mes_anterior < periodo_semanal) {

						std::string string_periodo = getString(1) + "/" + getString(idMes_anterior_PMO) + "/" + getString(idAno_anterior_PMO) + "-" + getString(conteioDias) + "dias";;

						const Periodo periodo_mensal(string_periodo);

						for (int posto = 0; posto < numPostos; posto++) {

							const int afluencia = intLeitura_320[posto];

							//Adiciona o valor de afluencia na matriz da classe Afluencia
							valor_afluencia_passadas_GEVAZP.at(posto).addElemento(periodo_mensal, afluencia);

						}//for (int posto = 0; posto < numPostos; posto++) {

					}//if (meses_registrados = numero_meses_passados_considerados) {
					else {

						for (int posto = 0; posto < numPostos; posto++) {

							const int afluencia = intLeitura_320[posto];

							//Adiciona o valor de afluencia na matriz da classe Afluencia
							valor_afluencia_passadas_GEVAZP.at(posto).addElemento(periodo_mes_historico, afluencia);

						}//for (int posto = 0; posto < numPostos; posto++) {

					}//else {

				}//if (periodo_historico >= periodo_limite_para_registrar) {

			}//while (!(leituraArquivo.eof())) {

			leituraArquivo.clear();
			leituraArquivo.close();

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_TENDENCIA_VAZOES_MENSAIS_GEVAZP: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_TENDENCIA_VAZOES_SEMANAIS_GEVAZP(Dados& a_dados, const std::string nomeArquivo, const std::string a_revisao)
{

	try {

		const int numPostos = 320;

		///////////////////////////////////////////////////////////////////
		std::ifstream leituraArquivo(nomeArquivo + "." + a_revisao);
		std::string line;
		std::string atributo;

		///////////////////////////////////////////////////////////////////
		//Estabelece o número de semanas observadas de acordo à revisão
		///////////////////////////////////////////////////////////////////

		int numero_semanas_observadas = 0;

		if (a_revisao == "rv1")
			numero_semanas_observadas = 1;
		else if (a_revisao == "rv2")
			numero_semanas_observadas = 2;
		else if (a_revisao == "rv3")
			numero_semanas_observadas = 3;
		else if (a_revisao == "rv4")
			numero_semanas_observadas = 4;
		else if (a_revisao == "rv5")
			numero_semanas_observadas = 5;

		////////////////////////////////////////////////////////////////////////
		//1. Determina a primeira semana operativa do PMO  
		////////////////////////////////////////////////////////////////////////

		//Nota: O mês do PMO corresponde com o mês do último dia da primeria semana

		Periodo periodo_semana_inicial_PMO = horizonte_otimizacao_DC.getElemento(IdEstagio_1);

		IdDia idDia = periodo_semana_inicial_PMO.getDia();
		IdMes idMes = periodo_semana_inicial_PMO.getMes();
		IdAno idAno = periodo_semana_inicial_PMO.getAno();

		const int numero_dias_periodo_semanal = periodo_semana_inicial_PMO.getDias();

		for (int dia = 1; dia < numero_dias_periodo_semanal; dia++) {//o primeiro dia também conta

			idDia++;

			IdDia maiordiadomes = periodo_semana_inicial_PMO.getMaiorDiaDoMes(idMes);

			if (idDia > maiordiadomes) {
				idDia = IdDia_1;
				idMes++;
				if (idMes == IdMes_Excedente) {
					idMes = IdMes_1;
					idAno++;
				}//if (idMes == IdMes_Excedente) {

			}//if(idDia == maiordiadomes) {

		}//for (int dia = 0; dia < numero_dias_periodo_semanal; dia++) {

		const IdMes idMes_Inicial_PMO = idMes;
		const IdAno idAno_Inicial_PMO = idAno;

		//////////////////////////////////////

		Periodo periodo_semanal_Inicial_PMO = horizonte_otimizacao_DC.getElemento(IdEstagio_1);

		const Periodo periodo_diario_limite(IdDia_1, idMes_Inicial_PMO, idAno_Inicial_PMO); //Corresponde ao primeiro dia do mês do PMO

		while (periodo_semanal_Inicial_PMO > periodo_diario_limite)
			periodo_semanal_Inicial_PMO--;


		///////////////////////////////////////////////////////////////////
		//Inicializa os postos com valor 0 já que no prevs não são 
		//informados todos eles
		///////////////////////////////////////////////////////////////////

		Periodo periodo_semanal = periodo_semanal_Inicial_PMO;

		for (int semanas_observadas = 0; semanas_observadas < numero_semanas_observadas; semanas_observadas++) {

			for (int posto = 0; posto < numPostos; posto++)
				valor_afluencia_passadas_GEVAZP.at(posto).addElemento(periodo_semanal, 0.0);

			periodo_semanal++;

		}//for (int semanas_observadas = 0; semanas_observadas < numero_semanas_observadas; semanas_observadas++) {

		///////////////////////////////////////////////////////////////////

		if (leituraArquivo.is_open()) {

			while (std::getline(leituraArquivo, line)) {

				//Código do posto
				atributo = line.substr(8, 3);
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				const int posto = std::atoi(atributo.c_str());

				periodo_semanal = periodo_semanal_Inicial_PMO;

				for (int semanas_observadas = 0; semanas_observadas < numero_semanas_observadas; semanas_observadas++) {

					//Afluência
					atributo = line.substr(12 + 10 * semanas_observadas, 9);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const double afluencia = std::atof(atributo.c_str());

					valor_afluencia_passadas_GEVAZP.at(posto - 1).setElemento(periodo_semanal, afluencia);

					periodo_semanal++;

				}//for (int semanas_observadas = 0; semanas_observadas < numero_semanas_observadas; semanas_observadas++) {

			}//while (std::getline(leituraArquivo, line)) {

		}//if (leituraArquivo.is_open()) {
		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + nomeArquivo + ".");

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_TENDENCIA_VAZOES_SEMANAIS_GEVAZP: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::set_termeletrica_potencia_disponivel_meta(Dados& a_dados)
{
	try {

		//Horizonte de estudo
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const IdEstagio maiorIdEstagioDC = horizonte_otimizacao_DC.getIteradorFinal();
		const IdCenario maiorIdCenarioDC = IdCenario(numero_realizacoes_por_periodo.getElemento(horizonte_otimizacao_DC.getElemento(maiorIdEstagioDC)));

		//Inicializa todas as termelétricas
		const IdTermeletrica	maiorIdTermeletrica = a_dados.getMaiorId(IdTermeletrica());
		for (IdTermeletrica idTermeletrica = IdTermeletrica_1; idTermeletrica <= maiorIdTermeletrica; idTermeletrica++)
			a_dados.vetorTermeletrica.att(idTermeletrica).setMatriz(AttMatrizTermeletrica_potencia_disponivel_meta, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>(IdCenario_1, std::vector<SmartEnupla<Periodo, double>>(maiorIdCenarioDC, SmartEnupla<Periodo, double>(horizonte_estudo, getdoubleFromChar("max")))));

		//Set valor na Termeletrica

		std::vector<int> codigos_usinas_avaliadas;

		codigos_usinas_avaliadas.push_back(140); //UTE Maua 3
		codigos_usinas_avaliadas.push_back(201); //Aparecida

		std::vector<double> potencia_disponivel_meta;

		potencia_disponivel_meta.push_back(408.16); //UTE Maua 3
		potencia_disponivel_meta.push_back(121.71); //UTE Maua 3

		std::vector<IdTermeletrica> vetor_idTermeletricas;

		for (int pos = 0; pos < int(codigos_usinas_avaliadas.size()); pos++)
			vetor_idTermeletricas.push_back(getIdFromCodigoONS(lista_codigo_ONS_termeletrica, codigos_usinas_avaliadas.at(pos)));


		for (IdCenario idCenario = IdCenario_1; idCenario <= maiorIdCenarioDC; idCenario++) {

			for (int pos = 0; pos < int(vetor_idTermeletricas.size()); pos++)
				a_dados.vetorTermeletrica.att(vetor_idTermeletricas.at(pos)).setElemento(AttMatrizTermeletrica_potencia_disponivel_meta, idCenario, horizonte_estudo.getIteradorFinal(), potencia_disponivel_meta.at(pos));

		}//for (IdCenario idCenario = IdCenario_1; idCenario <= maiorIdCenarioDC; idCenario++) {

		////////////////////////////////////////////


	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::set_termeletrica_potencia_disponivel_meta: \n" + std::string(erro.what())); }

}


void LeituraCEPEL::set_hidreletrica_vazao_turbinada_disponivel_meta(Dados& a_dados)
{
	try {

		//Horizonte de estudo
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const IdEstagio maiorIdEstagioDC = horizonte_otimizacao_DC.getIteradorFinal();
		const IdCenario maiorIdCenarioDC = IdCenario(numero_realizacoes_por_periodo.getElemento(horizonte_otimizacao_DC.getElemento(maiorIdEstagioDC)));

		//Inicializa todas as hidrelétricas
		const IdHidreletrica	maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());
		for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++)
			a_dados.vetorHidreletrica.att(idHidreletrica).setMatriz(AttMatrizHidreletrica_vazao_turbinada_disponivel_meta, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>(IdCenario_1, std::vector<SmartEnupla<Periodo, double>>(maiorIdCenarioDC, SmartEnupla<Periodo, double>(horizonte_estudo, getdoubleFromChar("max")))));

		//Set valor na hidrelétrica

		std::vector<int> codigos_usinas_avaliadas;

		codigos_usinas_avaliadas.push_back(156); //Três Marias
		codigos_usinas_avaliadas.push_back(162); //Queimado

		std::vector<SmartEnupla<Periodo, double>> vazao_turbinada_disponivel_meta;


		/////////////////////////
		//Três Marias vazao turbinada
		SmartEnupla<Periodo, double> vazao_turbinada_disponivel_meta_enupla_1;

		vazao_turbinada_disponivel_meta_enupla_1.addElemento(Periodo("16/05/2020-semanal"), 367.63);
		vazao_turbinada_disponivel_meta_enupla_1.addElemento(Periodo("23/05/2020-semanal"), 538.97);
		vazao_turbinada_disponivel_meta_enupla_1.addElemento(Periodo("30/05/2020-semanal"), 679.18);

		vazao_turbinada_disponivel_meta.push_back(vazao_turbinada_disponivel_meta_enupla_1);

		///////////////////////
		//Queimado vazao turbinada
		SmartEnupla<Periodo, double> vazao_turbinada_disponivel_meta_enupla_2;

		vazao_turbinada_disponivel_meta_enupla_2.addElemento(Periodo("16/05/2020-semanal"), 19.19);
		vazao_turbinada_disponivel_meta_enupla_2.addElemento(Periodo("23/05/2020-semanal"), 19.16);
		vazao_turbinada_disponivel_meta_enupla_2.addElemento(Periodo("30/05/2020-semanal"), 47.88);

		vazao_turbinada_disponivel_meta.push_back(vazao_turbinada_disponivel_meta_enupla_2);


		///////////////////////


		if (int(vazao_turbinada_disponivel_meta.size()) != int(codigos_usinas_avaliadas.size()))
			throw std::invalid_argument("Dimensoes de dados das usinas e potencia_meta devem ser iguais");

		std::vector<IdHidreletrica> vetor_idHidreletricas;

		for (int pos = 0; pos < int(codigos_usinas_avaliadas.size()); pos++)
			vetor_idHidreletricas.push_back(getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigos_usinas_avaliadas.at(pos)));

		//////////////////////////////////////////////////

		for (IdCenario idCenario = IdCenario_1; idCenario <= maiorIdCenarioDC; idCenario++) {

			for (int pos = 0; pos < int(vetor_idHidreletricas.size()); pos++) {

				const Periodo periodo_inicial = vazao_turbinada_disponivel_meta.at(pos).getIteradorInicial();
				const Periodo periodo_final = vazao_turbinada_disponivel_meta.at(pos).getIteradorFinal();

				for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo))
					a_dados.vetorHidreletrica.att(vetor_idHidreletricas.at(pos)).setElemento(AttMatrizHidreletrica_vazao_turbinada_disponivel_meta, idCenario, periodo, vazao_turbinada_disponivel_meta.at(pos).getElemento(periodo));
			}
		}//for (IdCenario idCenario = IdCenario_1; idCenario <= maiorIdCenarioDC; idCenario++) {

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::set_hidreletrica_vazao_turbinada_disponivel_meta: \n" + std::string(erro.what())); }

}


void LeituraCEPEL::set_hidreletrica_potencia_disponivel_meta_from_dec_oper_usih_DC(Dados& a_dados, std::string a_nomeArquivo)
{
	try {

		std::string line;
		std::string atributo;

		std::ifstream leituraArquivo(a_nomeArquivo);

		if (leituraArquivo.is_open()) {

			//Horizonte de estudo
			const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

			const IdHidreletrica  maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

			const IdCenario cenario_finalDC = IdCenario(numero_realizacoes_por_periodo.getElemento(horizonte_otimizacao_DC.getElemento(horizonte_otimizacao_DC.getIteradorFinal())));

			//////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Inicializa a potencia_disponivel_meta com valor getdoubleFromChar("max") -> Nesta lógica, valor "max" nao entra no PL
			//////////////////////////////////////////////////////////////////////////////////////////////////////////

			for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

				for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
					a_dados.vetorHidreletrica.att(idHidreletrica).setMatriz(AttMatrizHidreletrica_potencia_disponivel_meta, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>(IdCenario_1, std::vector <SmartEnupla<Periodo, double>>(cenario_finalDC, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0))));

			}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

			//////////////////////////////////////////////////////////////////

			SmartEnupla<IdHidreletrica, SmartEnupla<Periodo, double>> potencia_maxima_restricao_eletrica(IdHidreletrica_1, std::vector <SmartEnupla<Periodo, double>>(maiorIdHidreletrica, SmartEnupla<Periodo, double>(horizonte_estudo, getdoubleFromChar("max"))));
			SmartEnupla<IdHidreletrica, SmartEnupla<Periodo, double>> potencia_minima_restricao_eletrica(IdHidreletrica_1, std::vector <SmartEnupla<Periodo, double>>(maiorIdHidreletrica, SmartEnupla<Periodo, double>(horizonte_estudo, getdoubleFromChar("min"))));

			const IdRestricaoEletrica maiorIdRestricaoEletrica = a_dados.getMaiorId(IdRestricaoEletrica());

			for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= maiorIdRestricaoEletrica; idRestricaoEletrica++) {

				const IdHidreletrica idHidreletrica = a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.att(IdElementoSistema_1).getAtributo(AttComumElementoSistema_hidreletrica, IdHidreletrica());

				if (a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).getMaiorId(IdElementoSistema()) == IdElementoSistema_1 && idHidreletrica != IdHidreletrica_Nenhum) {//Significa que a restricao só tem 1 elemento hidreletrica

					//Cálculo da potência_media por periodo
					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						double potencia_maxima_media = 0;
						double potencia_minima_media = 0;

						const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

						for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

							potencia_maxima_media += a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).getElementoMatriz(AttMatrizRestricaoEletrica_potencia_maxima, periodo, idPatamarCarga, double()) * a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
							potencia_minima_media += a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).getElementoMatriz(AttMatrizRestricaoEletrica_potencia_minima, periodo, idPatamarCarga, double()) * a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());

						}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						if (potencia_maxima_restricao_eletrica.at(idHidreletrica).getElemento(periodo) > potencia_maxima_media)
							potencia_maxima_restricao_eletrica.at(idHidreletrica).setElemento(periodo, potencia_maxima_media);

						if (potencia_minima_restricao_eletrica.at(idHidreletrica).getElemento(periodo) < potencia_minima_media)
							potencia_minima_restricao_eletrica.at(idHidreletrica).setElemento(periodo, potencia_minima_media);

					}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				}//if (a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).getMaiorId(IdElementoSistema()) == IdElementoSistema_1 && idHidreletrica != IdHidreletrica_Nenhum) {

			}//for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= maiorIdRestricaoEletrica; idRestricaoEletrica++) {

			//Itaipú tem limites nos intercambios IT->ANDE IT->IV: O somatório é a geraçao total da usina IT

			const IdSubmercado idSubmercado_itaipu = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_submercado_ITAIPU);
			const IdSubmercado idSubmercado_ande = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_ANDE);
			const IdSubmercado idSubmercado_iv = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_IV);

			std::vector<IdIntercambio> idIntercambio_inicializado = a_dados.vetorIntercambio.getIdObjetos(AttComumIntercambio_submercado_origem, idSubmercado_itaipu);

			int idIntercambio_inicializado_size = int(idIntercambio_inicializado.size());

			IdIntercambio idIntercambio_itaipu_ande = IdIntercambio_Nenhum;

			for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

				if (a_dados.getAtributo(idIntercambio_inicializado.at(intercambio_inicializado), AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_ande) {
					idIntercambio_itaipu_ande = idIntercambio_inicializado.at(intercambio_inicializado);
					break;
				}//if (a_dados.getAtributo(idIntercambio_inicializado.at(intercambio_inicializado), AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_destino) {

			}//for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

			if (idIntercambio_itaipu_ande == IdIntercambio_Nenhum)
				throw std::invalid_argument("Intercambio nao encontrado");

			////////////////

			IdIntercambio idIntercambio_itaipu_iv = IdIntercambio_Nenhum;

			for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

				if (a_dados.getAtributo(idIntercambio_inicializado.at(intercambio_inicializado), AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_iv) {
					idIntercambio_itaipu_iv = idIntercambio_inicializado.at(intercambio_inicializado);
					break;
				}//if (a_dados.getAtributo(idIntercambio_inicializado.at(intercambio_inicializado), AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_destino) {

			}//for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++) {

			if (idIntercambio_itaipu_iv == IdIntercambio_Nenhum)
				throw std::invalid_argument("Intercambio nao encontrado");

			if (true) {

				const int codigo_usina = 66; //código para Itaipu

				const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

				if (idHidreletrica == IdHidreletrica_Nenhum)
					throw std::invalid_argument("idHidreletrica nao instanciada com codigo_" + getString(codigo_usina));

				//Cálculo da potência_media por periodo
				for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

					double potencia_maxima_media = 0;
					double potencia_minima_media = 0;

					const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

					for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						potencia_maxima_media += a_dados.vetorIntercambio.att(idIntercambio_itaipu_ande).getElementoMatriz(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, double()) * a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
						potencia_maxima_media += a_dados.vetorIntercambio.att(idIntercambio_itaipu_iv).getElementoMatriz(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, double()) * a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());

						potencia_minima_media += a_dados.vetorIntercambio.att(idIntercambio_itaipu_ande).getElementoMatriz(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, double()) * a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
						potencia_minima_media += a_dados.vetorIntercambio.att(idIntercambio_itaipu_iv).getElementoMatriz(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, double()) * a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());

					}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

					if (potencia_maxima_restricao_eletrica.at(idHidreletrica).getElemento(periodo) > potencia_maxima_media)
						potencia_maxima_restricao_eletrica.at(idHidreletrica).setElemento(periodo, potencia_maxima_media);

					if (potencia_minima_restricao_eletrica.at(idHidreletrica).getElemento(periodo) < potencia_minima_media)
						potencia_minima_restricao_eletrica.at(idHidreletrica).setElemento(periodo, potencia_minima_media);

				}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

			}//if (true) {


			/////////////////

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);

				if (line.size() >= 27) {

					//Patamar de carga: O campo "-" indica a média ponderada x patamar no periodo completo
					atributo = line.substr(13, 5);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const int patamarCarga = atoi(atributo.c_str());

					if (patamarCarga > 0) {

						//idPatamarCarga
						const IdPatamarCarga idPatamarCarga = IdPatamarCarga(patamarCarga);

						//idEstagio_DC

						atributo = line.substr(0, 5);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						const int estagio_DC = atoi(atributo.c_str());

						const IdEstagio idEstagio_DC = IdEstagio(estagio_DC);

						//idCenario

						atributo = line.substr(6, 6);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						const int cenario = atoi(atributo.c_str());

						const IdCenario idCenario_DC = IdCenario(cenario - estagio_DC + 1);//Utilizado para o último estágio DC. Ver arquivo saida DC para entender a lógica

						//código usina

						atributo = line.substr(28, 5);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						const int codigo_usina = atoi(atributo.c_str());

						const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

						if (idHidreletrica == IdHidreletrica_Nenhum)
							throw std::invalid_argument("idHidreletrica nao instanciada com codigo_" + atributo);

						//Potência meta

						atributo = line.substr(120, 9);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						const double potencia_disponivel_meta_patamar = atof(atributo.c_str());

						/////////////////

						const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();
						const Periodo periodo_final = horizonte_estudo.getIteradorFinal();

						bool sobreposicao_encontrada = false;

						for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

							double sobreposicao = periodo.sobreposicao(horizonte_otimizacao_DC.getElemento(idEstagio_DC));

							if (sobreposicao == 1) {
								sobreposicao_encontrada = true;

								if (idEstagio_DC < horizonte_otimizacao_DC.getIteradorFinal()) {

									for (IdCenario idCenario = IdCenario_1; idCenario <= cenario_finalDC; idCenario++) {

										double potencia_disponivel_meta = getdoubleFromChar("max");

										//if (a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_submercado, IdSubmercado()) == IdSubmercado_4 || a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_submercado, IdSubmercado()) == IdSubmercado_1 || a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_submercado, IdSubmercado()) == IdSubmercado_2 || a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_submercado, IdSubmercado()) == IdSubmercado_8)
											potencia_disponivel_meta = potencia_disponivel_meta_patamar * a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double()) + a_dados.vetorHidreletrica.att(idHidreletrica).getElementoMatriz(AttMatrizHidreletrica_potencia_disponivel_meta, idCenario, periodo, double());

										a_dados.vetorHidreletrica.att(idHidreletrica).setElemento(AttMatrizHidreletrica_potencia_disponivel_meta, idCenario, periodo, potencia_disponivel_meta);
									}//for (IdCenario idCenario = IdCenario_1; idCenario <= cenario_finalDC; idCenario++) {

								}//if (idEstagio_DC < horizonte_otimizacao_DC.getIteradorFinal()) {
								else if (idEstagio_DC == horizonte_otimizacao_DC.getIteradorFinal()) {

									double potencia_disponivel_meta = getdoubleFromChar("max");

									//if (a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_submercado, IdSubmercado()) == IdSubmercado_4 || a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_submercado, IdSubmercado()) == IdSubmercado_1 || a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_submercado, IdSubmercado()) == IdSubmercado_2 || a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_submercado, IdSubmercado()) == IdSubmercado_8)
										potencia_disponivel_meta = potencia_disponivel_meta_patamar * a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double()) + a_dados.vetorHidreletrica.att(idHidreletrica).getElementoMatriz(AttMatrizHidreletrica_potencia_disponivel_meta, idCenario_DC, periodo, double());

									a_dados.vetorHidreletrica.att(idHidreletrica).setElemento(AttMatrizHidreletrica_potencia_disponivel_meta, idCenario_DC, periodo, potencia_disponivel_meta);

								}//else if (idEstagio_DC == horizonte_otimizacao_DC.getIteradorFinal()) {


							}//if (sobreposicao == 1) {

							if (sobreposicao_encontrada && sobreposicao == 0)
								break;

						}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

					}//if (atributo != "-") {

				}//if (line.size() >= 27) {

			}//while (std::getline(leituraArquivo, line)) {

			/////////////////////////////////////////
			//Valida que a potencia_meta nao seja menor que a potencia_minima ou maior que a potencia_maxima por RE
			/////////////////////////////////////////

			for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

				for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

					for (IdCenario idCenario = IdCenario_1; idCenario <= cenario_finalDC; idCenario++) {

						double potencia_disponivel_meta = a_dados.vetorHidreletrica.att(idHidreletrica).getElementoMatriz(AttMatrizHidreletrica_potencia_disponivel_meta, idCenario, periodo, double());

						if (potencia_maxima_restricao_eletrica.at(idHidreletrica).getElemento(periodo) < potencia_disponivel_meta)
							potencia_disponivel_meta = potencia_maxima_restricao_eletrica.at(idHidreletrica).getElemento(periodo);

						if (potencia_minima_restricao_eletrica.at(idHidreletrica).getElemento(periodo) > potencia_disponivel_meta)
							potencia_disponivel_meta = potencia_minima_restricao_eletrica.at(idHidreletrica).getElemento(periodo);

						a_dados.vetorHidreletrica.att(idHidreletrica).setElemento(AttMatrizHidreletrica_potencia_disponivel_meta, idCenario, periodo, potencia_disponivel_meta);

					}//for (IdCenario idCenario = IdCenario_1; idCenario <= cenario_finalDC; idCenario++) {

				}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

			}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

		}//if (leituraArquivo.is_open()) {

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::set_hidreletrica_potencia_disponivel_meta_from_dec_oper_usih_DC: \n" + std::string(erro.what())); }

}


void LeituraCEPEL::leitura_range_volume_from_eco_fpha_DC(Dados& a_dados, std::string a_nomeArquivo)
{
	try {

		//std::string line;
		//std::string atributo;

		//std::ifstream leituraArquivo(a_nomeArquivo);

		//if (leituraArquivo.is_open()) {

		//	//Horizonte de estudo
		//	const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		//	const IdHidreletrica  maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

		//	//////////////////////////////////////////////////////////////////////////////////////////////////////////
		//	//Inicializa construcao_fph_volume_minimo e construcao_fph_volume_maximo com o range min-max da usina
		//	//////////////////////////////////////////////////////////////////////////////////////////////////////////

		//	for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

		//		for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

		//			a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).addElemento(AttVetorReservatorio_construcao_fph_volume_minimo, periodo, a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getElementoVetor(AttVetorReservatorio_volume_minimo, periodo, double()));
		//			a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).addElemento(AttVetorReservatorio_construcao_fph_volume_maximo, periodo, a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getElementoVetor(AttVetorReservatorio_volume_maximo, periodo, double()));

		//		}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

		//	}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

		//	//////////////////////////////////////////////////////////////////

		//	while (std::getline(leituraArquivo, line)) {

		//		strNormalizada(line);

		//		atributo = line.substr(0, 4);
		//		atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

		//		const int codigo_usina = atoi(atributo.c_str());

		//		if (codigo_usina > 0) {

		//			const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

		//			if (idHidreletrica != IdHidreletrica_Nenhum) {

		//				//idEstagio_DC

		//				atributo = line.substr(5, 4);
		//				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

		//				const int estagio_DC = atoi(atributo.c_str());

		//				const IdEstagio idEstagio_DC = IdEstagio(estagio_DC);

		//				//construcao_fph_volume_minimo

		//				atributo = line.substr(74, 8);
		//				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

		//				const double construcao_fph_volume_minimo = atof(atributo.c_str());

		//				//construcao_fph_volume_maximo

		//				atributo = line.substr(83, 8);
		//				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

		//				const double construcao_fph_volume_maximo = atof(atributo.c_str());

		//				/////////////////

		//				const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();
		//				const Periodo periodo_final = horizonte_estudo.getIteradorFinal();

		//				bool sobreposicao_encontrada = false;

		//				for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

		//					double sobreposicao = periodo.sobreposicao(horizonte_otimizacao_DC.getElemento(idEstagio_DC));

		//					if (sobreposicao == 1) {
		//						sobreposicao_encontrada = true;

		//						a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_construcao_fph_volume_minimo, periodo, construcao_fph_volume_minimo);
		//						a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_construcao_fph_volume_maximo, periodo, construcao_fph_volume_maximo);

		//					}//if (sobreposicao == 1) {

		//					if (sobreposicao_encontrada && sobreposicao == 0)
		//						break;

		//				}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

		//			}//if (idHidreletrica != IdHidreletrica_Nenhum) {
		//			else
		//				throw std::invalid_argument("idHidreletrica nao instanciada com codigo_" + atributo);

		//		}//if (codigo_usina > 0) {

		//	}//while (std::getline(leituraArquivo, line)) {

		//}//if (leituraArquivo.is_open()) {

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_range_volume_from_eco_fpha_DC: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_volumes_meta_from_dec_oper_usih_DC(Dados& a_dados, std::string a_nomeArquivo, const bool a_somente_volume_meta_no_ultimo_estagio)
{
	try {

		std::string line;
		std::string atributo;

		std::ifstream leituraArquivo(a_nomeArquivo);

		if (leituraArquivo.is_open()) {

			//Horizonte de estudo
			const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

			const IdHidreletrica  maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());
			const IdCenario maiorIdCenarioDC = IdCenario(numero_realizacoes_por_periodo.getElemento(horizonte_otimizacao_DC.getElemento(horizonte_otimizacao_DC.getIteradorFinal())));

			//////////////////////////////////////////////////////////////////
			//Inicializa volume_meta

			for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

				//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
				a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setMatriz(AttMatrizReservatorio_volume_meta, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>(IdCenario_1, std::vector <SmartEnupla<Periodo, double>>(maiorIdCenarioDC, SmartEnupla<Periodo, double>(horizonte_estudo, getdoubleFromChar("max")))));

			}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {


			//////////////////////////////////////////////////////////////////

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);

				if (line.size() >= 27) {

					//Patamar de carga: O campo "-" indica a média ponderada x patamar no periodo completo
					atributo = line.substr(19, 8);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					if (atributo == "-") {

						//idEstagio_DC

						atributo = line.substr(0, 5);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						const int estagio_DC = atoi(atributo.c_str());

						const IdEstagio idEstagio_DC = IdEstagio(estagio_DC);

						if (!a_somente_volume_meta_no_ultimo_estagio || (a_somente_volume_meta_no_ultimo_estagio && idEstagio_DC == horizonte_otimizacao_DC.getIteradorFinal())) {

							//idCenario

							atributo = line.substr(6, 6);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int cenario = atoi(atributo.c_str());

							const IdCenario idCenario_DC = IdCenario(cenario - estagio_DC + 1);//Utilizado para o último estágio DC. Ver arquivo saida DC para entender a lógica

							//código usina

							atributo = line.substr(28, 5);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const int codigo_usina = atoi(atributo.c_str());

							const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

							if (idHidreletrica == IdHidreletrica_Nenhum)
								throw std::invalid_argument("idHidreletrica nao instanciada com codigo_" + atributo);

							//Volume_util_meta

							atributo = line.substr(98, 10);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							if (atributo != "-") {//Teste para só pegar as usinas com reservatório

								const double volume_util_meta_lido = atof(atributo.c_str());

								const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();
								const Periodo periodo_final = horizonte_estudo.getIteradorFinal();

								bool sobreposicao_encontrada = false;

								for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

									double sobreposicao = periodo.sobreposicao(horizonte_otimizacao_DC.getElemento(idEstagio_DC));

									if (sobreposicao == 1) {
										sobreposicao_encontrada = true;

										//Identifica se o o dia final do periodo corresponde com o dia final do periodo_DC
										const Periodo periodo_dia_final_SPT = periodo.getPeriodoDiario_do_diaFinal(periodo);
										const Periodo periodo_dia_final_DC = horizonte_otimizacao_DC.getElemento(idEstagio_DC).getPeriodoDiario_do_diaFinal(horizonte_otimizacao_DC.getElemento(idEstagio_DC));

										double volume_util_meta = getdoubleFromChar("max");

										if (periodo_dia_final_SPT == periodo_dia_final_DC) {
											volume_util_meta = volume_util_meta_lido;

										}//if (periodo_dia_final_SPT == periodo_dia_final_DC) {


										if (idEstagio_DC < horizonte_otimizacao_DC.getIteradorFinal()) {

											for (IdCenario idCenario = IdCenario_1; idCenario <= maiorIdCenarioDC; idCenario++)
												a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttMatrizReservatorio_volume_meta, idCenario, periodo, volume_util_meta);

										}//if (idEstagio_DC < horizonte_otimizacao_DC.getIteradorFinal()) {
										else if (idEstagio_DC == horizonte_otimizacao_DC.getIteradorFinal())
											a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttMatrizReservatorio_volume_meta, idCenario_DC, periodo, volume_util_meta);

									}//if (sobreposicao == 1) {

									if (sobreposicao_encontrada && sobreposicao == 0)
										break;

								}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

							}//if (atributo != "-") {

						}//if (!a_somente_volume_meta_no_ultimo_estagio || a_somente_volume_meta_no_ultimo_estagio && idEstagio_DC == horizonte_otimizacao_DC.getIteradorFinal()) {

					}//if (atributo == "-") {

				}//if (line.size() >= 27) {

			}//while (std::getline(leituraArquivo, line)) {

		}//if (leituraArquivo.is_open()) {

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_volumes_meta_from_dec_oper_usih_DC: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_vazao_evaporada_meta_from_dec_oper_usih_DC(Dados& a_dados, std::string a_nomeArquivo)
{
	try {

		std::string line;
		std::string atributo;

		std::ifstream leituraArquivo(a_nomeArquivo);

		if (leituraArquivo.is_open()) {

			//Horizonte de estudo
			const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

			const IdHidreletrica  maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

			const IdCenario cenario_finalDC = IdCenario(numero_realizacoes_por_periodo.getElemento(horizonte_otimizacao_DC.getElemento(horizonte_otimizacao_DC.getIteradorFinal())));

			//////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Inicializa a evaporaçao_meta com valor 0.0 -> Nesta lógica, todas as usinas tem um requerimento de vazao_evaporada
			//////////////////////////////////////////////////////////////////////////////////////////////////////////

			for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

				for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
					a_dados.vetorHidreletrica.att(idHidreletrica).setMatriz(AttMatrizHidreletrica_vazao_evaporada_meta, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>(IdCenario_1, std::vector <SmartEnupla<Periodo, double>>(cenario_finalDC, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0))));

			}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

			//////////////////////////////////////////////////////////////////

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);

				if (line.size() >= 27) {

					//Patamar de carga: O campo "-" indica a média ponderada x patamar no periodo completo
					atributo = line.substr(19, 8);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					if (atributo == "-") {

						//idEstagio_DC

						atributo = line.substr(0, 5);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						const int estagio_DC = atoi(atributo.c_str());

						const IdEstagio idEstagio_DC = IdEstagio(estagio_DC);

						//idCenario

						atributo = line.substr(6, 6);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						const int cenario = atoi(atributo.c_str());

						const IdCenario idCenario_DC = IdCenario(cenario - estagio_DC + 1);//Utilizado para o último estágio DC. Ver arquivo saida DC para entender a lógica

						//código usina

						atributo = line.substr(28, 5);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						const int codigo_usina = atoi(atributo.c_str());

						const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

						if (idHidreletrica == IdHidreletrica_Nenhum)
							throw std::invalid_argument("idHidreletrica nao instanciada com codigo_" + atributo);

						//Vazao evaporada meta

						atributo = line.substr(296, 10);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						double vazao_evaporada_meta = atof(atributo.c_str());

						/////////////////

						const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();
						const Periodo periodo_final = horizonte_estudo.getIteradorFinal();

						bool sobreposicao_encontrada = false;

						for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

							double sobreposicao = periodo.sobreposicao(horizonte_otimizacao_DC.getElemento(idEstagio_DC));

							if (sobreposicao == 1) {
								sobreposicao_encontrada = true;

								if (idEstagio_DC < horizonte_otimizacao_DC.getIteradorFinal()) {

									for (IdCenario idCenario = IdCenario_1; idCenario <= cenario_finalDC; idCenario++)
										a_dados.vetorHidreletrica.att(idHidreletrica).setElemento(AttMatrizHidreletrica_vazao_evaporada_meta, idCenario, periodo, vazao_evaporada_meta);

								}//if (idEstagio_DC < horizonte_otimizacao_DC.getIteradorFinal()) {
								else if (idEstagio_DC == horizonte_otimizacao_DC.getIteradorFinal())
									a_dados.vetorHidreletrica.att(idHidreletrica).setElemento(AttMatrizHidreletrica_vazao_evaporada_meta, idCenario_DC, periodo, vazao_evaporada_meta);


							}//if (sobreposicao == 1) {

							if (sobreposicao_encontrada && sobreposicao == 0)
								break;

						}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

					}//if (atributo == "-") {

				}//if (line.size() >= 27) {

			}//while (std::getline(leituraArquivo, line)) {

		}//if (leituraArquivo.is_open()) {

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_vazao_evaporada_meta_from_dec_oper_usih_DC: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_fph_from_avl_cortesfpha_dec_DC(Dados& a_dados, std::string a_nomeArquivo)
{
	try {

		std::string line;
		std::string atributo;

		std::ifstream leituraArquivo(a_nomeArquivo);

		if (leituraArquivo.is_open()) {

			//Horizonte de estudo
			const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

			const IdHidreletrica  maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

			bool secao_fph = false;

			SmartEnupla <IdHidreletrica, bool> ler_fph_hidreletrica(IdHidreletrica_1, std::vector<bool>(maiorIdHidreletrica, true));

			//////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Identifica se as FPH foram carregadas na pre-config e inicializa para todos os periodos 1 corte = 0
			//////////////////////////////////////////////////////////////////////////////////////////////////////////

			for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

				if (!a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.isInstanciado(IdFuncaoProducaoHidreletrica_1)) {

					FuncaoProducaoHidreletrica funcaoProducaoHidreletrica;
					funcaoProducaoHidreletrica.setAtributo(AttComumFuncaoProducaoHidreletrica_idFuncaoProducaoHidreletrica, IdFuncaoProducaoHidreletrica_1);

					a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.add(funcaoProducaoHidreletrica);

				}//if (!a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.isInstanciado(IdFuncaoProducaoHidreletrica_1)) {

				if (a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).getSizeMatriz(AttMatrizFuncaoProducaoHidreletrica_RHS) > 0)
					ler_fph_hidreletrica.setElemento(idHidreletrica, false); //Se a fph de uma usina foi carregada via pré-config, nao sao carregadas as fph-DC
				else {

					//Instancia idcorte_1 com valores 0 para todos os periodos do horizonte_estudo

					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).addElemento(AttMatrizFuncaoProducaoHidreletrica_RHS, periodo, 1, 0.0);
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).addElemento(AttMatrizFuncaoProducaoHidreletrica_FC, periodo, 1, 0.0);
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).addElemento(AttMatrizFuncaoProducaoHidreletrica_VH, periodo, 1, 0.0);
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).addElemento(AttMatrizFuncaoProducaoHidreletrica_QH, periodo, 1, 0.0);
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).addElemento(AttMatrizFuncaoProducaoHidreletrica_SH, periodo, 1, 0.0);

					}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				}//else {

			}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

			//////////////////////////////////////////////////////////////////

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);

				if (line.size() >= 7 && !secao_fph) {

					if (line.substr(0, 7) == "@Tabela")
						secao_fph = true;

				}//if (line.size() >= 7) {

				if (secao_fph) {

					atributo = line.substr(0, 5);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const int codigo_usina = atoi(atributo.c_str());

					if (codigo_usina > 0) {

						const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

						if (idHidreletrica != IdHidreletrica_Nenhum) {

							if (ler_fph_hidreletrica.getElemento(idHidreletrica)) {

								//idEstagio_DC

								atributo = line.substr(6, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const int estagio_DC = atoi(atributo.c_str());

								const IdEstagio idEstagio_DC = IdEstagio(estagio_DC);

								//idCorte

								atributo = line.substr(27, 7);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const int idCorte = atoi(atributo.c_str());

								//FC

								atributo = line.substr(35, 10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const double fc = atof(atributo.c_str());

								//RHS

								atributo = line.substr(46, 16);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const double rhs = -atof(atributo.c_str()); // O sinal menos deixa o rhs padrao do SPT

								//VH

								atributo = line.substr(63, 16);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const double coef_vh = atof(atributo.c_str());

								//QTUR

								atributo = line.substr(80, 16);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const double coef_qtur = atof(atributo.c_str());

								//QVER

								atributo = line.substr(97, 16);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const double coef_qver = atof(atributo.c_str());

								/////////////////

								const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();
								const Periodo periodo_final = horizonte_estudo.getIteradorFinal();


								bool sobreposicao_encontrada = false;

								for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

									double sobreposicao = periodo.sobreposicao(horizonte_otimizacao_DC.getElemento(idEstagio_DC));

									if (sobreposicao == 1) {
										sobreposicao_encontrada = true;

										//Existem dados repetidos no arquivo DECOMP

										if (a_dados.getSize2Matriz(idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, periodo) == idCorte) {

											a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).setElemento(AttMatrizFuncaoProducaoHidreletrica_RHS, periodo, idCorte, rhs);
											a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).setElemento(AttMatrizFuncaoProducaoHidreletrica_FC, periodo, idCorte, fc);
											a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).setElemento(AttMatrizFuncaoProducaoHidreletrica_VH, periodo, idCorte, coef_vh);
											a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).setElemento(AttMatrizFuncaoProducaoHidreletrica_QH, periodo, idCorte, coef_qtur);
											a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).setElemento(AttMatrizFuncaoProducaoHidreletrica_SH, periodo, idCorte, coef_qver);

										}//if (a_dados.getSize2Matriz(idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, periodo) == idCorte) {
										else if (a_dados.getSize2Matriz(idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, periodo) < idCorte) {

											a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).addElemento(AttMatrizFuncaoProducaoHidreletrica_RHS, periodo, idCorte, rhs);
											a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).addElemento(AttMatrizFuncaoProducaoHidreletrica_FC, periodo, idCorte, fc);
											a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).addElemento(AttMatrizFuncaoProducaoHidreletrica_VH, periodo, idCorte, coef_vh);
											a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).addElemento(AttMatrizFuncaoProducaoHidreletrica_QH, periodo, idCorte, coef_qtur);
											a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).addElemento(AttMatrizFuncaoProducaoHidreletrica_SH, periodo, idCorte, coef_qver);

										}//else if (a_dados.getSize2Matriz(idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, periodo) < idCorte) {

									}//if (sobreposicao == 1) {

									if (sobreposicao_encontrada && sobreposicao == 0)
										break;

								}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

							}//if (ler_fph_hidreletrica.getElemento(idHidreletrica)) {

						}//if (idHidreletrica != IdHidreletrica_Nenhum) {
						else
							throw std::invalid_argument("idHidreletrica nao instanciada com codigo_" + atributo);

					}//if (codigo_usina > 0) {

				}//if (secao_fph) {

			}//while (std::getline(leituraArquivo, line)) {

		}//if (leituraArquivo.is_open()) {

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_fph_from_avl_cortesfpha_dec_DC: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_coeficientes_evaporacao_from_dec_cortes_evap_DC(Dados& a_dados, std::string a_nomeArquivo)
{
	try {

		std::string line;
		std::string atributo;

		std::ifstream leituraArquivo(a_nomeArquivo);

		if (leituraArquivo.is_open()) {

			//Horizonte de estudo
			const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

			const IdHidreletrica  maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

			bool secao_evaporacao = false;

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);

				if (line.size() >= 7) {

					if (line.substr(0, 7) == "@Tabela" && !secao_evaporacao)
						secao_evaporacao = true;

				}//if (line.size() >= 7) {

				if (secao_evaporacao) {

					atributo = line.substr(0, 4);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const int estagio_DC = atoi(atributo.c_str());

					if (estagio_DC > 0) {

						const IdEstagio idEstagio_DC = IdEstagio(estagio_DC);

						atributo = line.substr(7, 3);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						const int codigo_usina = atoi(atributo.c_str());

						const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

						if (idHidreletrica != IdHidreletrica_Nenhum) {

							/////////////////
							//Volume Referencia
							atributo = line.substr(84, 11);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double volume_referencia = atof(atributo.c_str());

							//Coef_1

							atributo = line.substr(111, 13);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							double coef_linear_evaporacao_1 = atof(atributo.c_str());

							//Coef_0: RHS

							atributo = line.substr(130, 7);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							double coef_linear_evaporacao_0 = atof(atributo.c_str());

							//Ajustes necessários
							//coef_linear_evaporacao_0 -= volume_referencia * coef_linear_evaporacao_1;
							//coef_linear_evaporacao_0 *= (-1);
							//coef_linear_evaporacao_1 *= 2;

							/////////////////

							if (idEstagio_DC == IdEstagio_1) {
								a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_coef_linear_evaporacao_0, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
								a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_coef_linear_evaporacao_1, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
							}//if (idEstagio_DC == IdEstagio_1) {

							const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();
							const Periodo periodo_final = horizonte_estudo.getIteradorFinal();

							bool sobreposicao_encontrada = false;

							for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

								double sobreposicao = periodo.sobreposicao(horizonte_otimizacao_DC.getElemento(idEstagio_DC));

								if (sobreposicao == 1) {
									sobreposicao_encontrada = true;

									const double conversor_vazao_volume_periodo = std::pow(a_dados.getElementoVetor(AttVetorDados_conversor_vazao_volume, periodo, double()), -1);

									a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_coef_linear_evaporacao_0, periodo, coef_linear_evaporacao_0 * conversor_vazao_volume_periodo);
									a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_coef_linear_evaporacao_1, periodo, coef_linear_evaporacao_1 * conversor_vazao_volume_periodo);
								}//if (sobreposicao == 1) {

								if (sobreposicao_encontrada && sobreposicao == 0)
									break;

							}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

						}//if (idHidreletrica != IdHidreletrica_Nenhum) {
						else
							throw std::invalid_argument("idHidreletrica nao instanciada com codigo_" + atributo);

					}//if (estagio_DC > 0) {

				}//if (secao_evaporacao) {

			}//while (std::getline(leituraArquivo, line)) {

		}//if (leituraArquivo.is_open()) {

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_coeficientes_evaporacao_from_dec_cortes_evap_DC: \n" + std::string(erro.what())); }

}


bool LeituraCEPEL::leitura_turbinamento_maximo_from_avl_turb_max_DC(Dados& a_dados, std::string a_nomeArquivo_1)
{
	try {

		bool lido_turbinamento_maximo_from_relato_e_avl_turb_max_DC = false;

		std::string line;
		std::string atributo;

		std::ifstream leituraArquivo_1(a_nomeArquivo_1);

		//Filosofia: do arquivo avl_turb_max.csv carrega o QTUR_MAX correspondente ao primeiro mês
		//Os dados do segundo mês sao carregados no cálculo de qMax

		if (leituraArquivo_1.is_open()) {

			lido_turbinamento_maximo_from_relato_e_avl_turb_max_DC = true;

			//Horizonte de estudo
			const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

			const IdHidreletrica  maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

			///////////////////////////////////////////
			//0. Inicializa usinas com turbinamento = 0
			///////////////////////////////////////////

			for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

				if (a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) != TipoDetalhamentoProducaoHidreletrica_sem_producao)
					a_dados.vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_vazao_turbinada_maxima, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

			}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

			////////////////////////////////////////////////////////
			//1. Leitura do avl_turb_max (Atualiza estagios DC t < T)
			////////////////////////////////////////////////////////

			while (std::getline(leituraArquivo_1, line)) {

				strNormalizada(line);

				if (line.size() >= 210) {

					////////////////////////////

					atributo = line.substr(0, 12);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const int estagio_DC = atoi(atributo.c_str());

					if (estagio_DC > 0) {

						const IdEstagio idEstagio_DC = IdEstagio(estagio_DC);

						////////////////////////////

						atributo = line.substr(26, 12);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						const int codigo_usina = atoi(atributo.c_str());

						const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

						if (idHidreletrica == IdHidreletrica_Nenhum)
							throw std::invalid_argument("Usina nao instanciada com codigo_" + getString(codigo_usina));

						////////////////////////////

						if (a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) != TipoDetalhamentoProducaoHidreletrica_sem_producao) {

							atributo = line.substr(93, 12);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double turbinamento_maximo_disponivel = atof(atributo.c_str());

							bool sobreposicao_encontrada = false;

							const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();
							const Periodo periodo_final = horizonte_estudo.getIteradorFinal();

							for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

								double sobreposicao = periodo.sobreposicao(horizonte_otimizacao_DC.getElemento(idEstagio_DC));

								if (sobreposicao == 1) {
									sobreposicao_encontrada = true;

									const double indisponibilidade_forcada = a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada, periodo, double());
									const double indisponibilidade_programada = a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada, periodo, double());
									const double disponibilidade = (1.0 - indisponibilidade_forcada) * (1.0 - indisponibilidade_programada);

									double turbinamento_maximo = 0;

									if (disponibilidade > 0)
										turbinamento_maximo = turbinamento_maximo_disponivel / disponibilidade;

									a_dados.vetorHidreletrica.att(idHidreletrica).setElemento(AttVetorHidreletrica_vazao_turbinada_maxima, periodo, turbinamento_maximo);
								}//if (sobreposicao == 1) {

								if (sobreposicao_encontrada && sobreposicao == 0)
									break;

							}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

						}//if (a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) != TipoDetalhamentoProducaoHidreletrica_sem_producao) {

					}//if (estagio_DC > 0) {

				}//if (line.size() >= 210) {

			}//while (std::getline(leituraArquivo, line)) {

			leituraArquivo_1.clear();
			leituraArquivo_1.close();

		}//if (leituraArquivo_1.is_open()) {

		return lido_turbinamento_maximo_from_relato_e_avl_turb_max_DC;

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_turbinamento_maximo_from_avl_turb_max_DC: \n" + std::string(erro.what())); }

}


bool LeituraCEPEL::leitura_turbinamento_maximo_from_relato_e_avl_turb_max_DC(Dados& a_dados, std::string a_nomeArquivo_1, std::string a_nomeArquivo_2)
{
	try {

		bool lido_turbinamento_maximo_from_relato_e_avl_turb_max_DC = false;

		std::string line;
		std::string atributo;

		std::ifstream leituraArquivo_1(a_nomeArquivo_1);
		std::ifstream leituraArquivo_2(a_nomeArquivo_2);

		//Filosofia: do arquivo avl_turb_max.csv carrega o QTUR_MAX correspondente ao primeiro mês
		//Os dados do segundo mês sao carregados do relatos.rvX (supondo que para o periodo mensal nao é limitado o turbinamento por engolimento)

		if (leituraArquivo_1.is_open() && leituraArquivo_2.is_open()) {

			lido_turbinamento_maximo_from_relato_e_avl_turb_max_DC = true;

			//Horizonte de estudo
			const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

			const IdHidreletrica  maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

			///////////////////////////////////////////
			//0. Inicializa usinas com turbinamento = 0
			///////////////////////////////////////////

			for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

				if (a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) != TipoDetalhamentoProducaoHidreletrica_sem_producao)
					a_dados.vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_vazao_turbinada_maxima, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

			}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

			/////////////////////////////////////////////////////////
			//1. Leitura do relato.rvX (Atualiza último estágio DC
			/////////////////////////////////////////////////////////

			while (std::getline(leituraArquivo_1, line)) {

				strNormalizada(line);

				if (line.size() >= 106) {

					if (line.substr(3, 99) == "Relatorio  dos  Dados  do  Cadastro  das  Usinas  Hidraulicas na Configuracao - a partir do estagio") {

						atributo = line.substr(104, 2);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						const IdEstagio idEstagio_DC = IdEstagio(atoi(atributo.c_str()));

						if (idEstagio_DC == horizonte_otimizacao_DC.getIteradorFinal()) {

							int conteio_usina = 0;

							while (std::getline(leituraArquivo_1, line)) {

								strNormalizada(line);

								atributo = line.substr(4, 3);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								const int codigo_usina = atoi(atributo.c_str());

								if (codigo_usina > 0) {

									const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

									if (idHidreletrica != IdHidreletrica_Nenhum) {//O relato tem usinas que nao entram no estudo de acordo com DADGER

										conteio_usina++;

										if (a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) != TipoDetalhamentoProducaoHidreletrica_sem_producao) {

											atributo = line.substr(84, 6);
											atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

											const double turbinamento_maximo = atof(atributo.c_str());

											const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();
											const Periodo periodo_final = horizonte_estudo.getIteradorFinal();

											bool sobreposicao_encontrada = false;

											for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

												double sobreposicao = periodo.sobreposicao(horizonte_otimizacao_DC.getElemento(idEstagio_DC));

												if (sobreposicao == 1) {
													sobreposicao_encontrada = true;
													a_dados.vetorHidreletrica.att(idHidreletrica).setElemento(AttVetorHidreletrica_vazao_turbinada_maxima, periodo, turbinamento_maximo);
												}//if (sobreposicao == 1) {

												if (sobreposicao_encontrada && sobreposicao == 0)
													break;

											}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

										}//if (a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) != TipoDetalhamentoProducaoHidreletrica_sem_producao) {

									}//if (idHidreletrica != IdHidreletrica_Nenhum) {

								}//if (codigo_usina > 0) {

								if (IdHidreletrica(conteio_usina) == maiorIdHidreletrica)
									break;

							}//while (std::getline(leituraArquivo, line)) {

							if (IdHidreletrica(conteio_usina) < maiorIdHidreletrica)
								throw std::invalid_argument("Numero de hidreletricas no arquivo relato.rvX menor as instanciadas no DADGER");

							if (idEstagio_DC == horizonte_otimizacao_DC.getIteradorFinal())
								break;

						}//if (idEstagio_DC == horizonte_otimizacao_DC.getIteradorFinal()) {

					}//if (line.substr(3, 99) == "Relatorio  dos  Dados  do  Cadastro  das  Usinas  Hidraulicas na Configuracao - a partir do estagio") {

				}//if (line.size() >= 106) {

			}//while (std::getline(leituraArquivo, line)) {

			leituraArquivo_1.clear();
			leituraArquivo_1.close();

			////////////////////////////////////////////////////////
			//2. Leitura do avl_turb_max (Atualiza estagios DC t < T
			////////////////////////////////////////////////////////

			while (std::getline(leituraArquivo_2, line)) {

				strNormalizada(line);

				if (line.size() >= 210) {

					////////////////////////////

					atributo = line.substr(0, 12);
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const int estagio_DC = atoi(atributo.c_str());

					if (estagio_DC > 0) {

						const IdEstagio idEstagio_DC = IdEstagio(estagio_DC);

						////////////////////////////

						atributo = line.substr(26, 12);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						const int codigo_usina = atoi(atributo.c_str());

						const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

						if (idHidreletrica == IdHidreletrica_Nenhum)
							throw std::invalid_argument("Usina nao instanciada com codigo_" + getString(codigo_usina));

						////////////////////////////

						if (a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) != TipoDetalhamentoProducaoHidreletrica_sem_producao) {

							atributo = line.substr(93, 12);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double turbinamento_maximo_disponivel = atof(atributo.c_str());

							bool sobreposicao_encontrada = false;

							const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();
							const Periodo periodo_final = horizonte_estudo.getIteradorFinal();

							for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

								double sobreposicao = periodo.sobreposicao(horizonte_otimizacao_DC.getElemento(idEstagio_DC));

								if (sobreposicao == 1) {
									sobreposicao_encontrada = true;

									const double indisponibilidade_forcada = a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada, periodo, double());
									const double indisponibilidade_programada = a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada, periodo, double());
									const double disponibilidade = (1.0 - indisponibilidade_forcada) * (1.0 - indisponibilidade_programada);

									double turbinamento_maximo = 0;

									if (disponibilidade > 0)
										turbinamento_maximo = turbinamento_maximo_disponivel / disponibilidade;

									a_dados.vetorHidreletrica.att(idHidreletrica).setElemento(AttVetorHidreletrica_vazao_turbinada_maxima, periodo, turbinamento_maximo);
								}//if (sobreposicao == 1) {

								if (sobreposicao_encontrada && sobreposicao == 0)
									break;

							}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

						}//if (a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) != TipoDetalhamentoProducaoHidreletrica_sem_producao) {

					}//if (estagio_DC > 0) {

				}//if (line.size() >= 210) {

			}//while (std::getline(leituraArquivo, line)) {

			leituraArquivo_2.clear();
			leituraArquivo_2.close();

		}//if (leituraArquivo_1.is_open() && leituraArquivo_2.is_open()) {

		return lido_turbinamento_maximo_from_relato_e_avl_turb_max_DC;

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_turbinamento_maximo_from_relato_e_avl_turb_max_DC: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_volumes_meta_from_relatos_DC(Dados& a_dados, std::string a_nomeArquivo_1, std::string a_nomeArquivo_2)
{
	try {

		std::string line;
		std::string atributo;

		std::ifstream leituraArquivo_1(a_nomeArquivo_1);
		std::ifstream leituraArquivo_2(a_nomeArquivo_2);

		//arquivo_1: relato.rvX  -> Volume final das semanas do primeiro mês
		//arquivo_2: relato2.rvX -> Volume final das aberturas do segundo mês

		if (leituraArquivo_1.is_open() && leituraArquivo_2.is_open()) {

			//Horizonte de estudo
			const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

			const IdHidreletrica  maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

			const IdEstagio maiorIdEstagioDC = horizonte_otimizacao_DC.getIteradorFinal();
			const IdCenario maiorIdCenarioDC = IdCenario(numero_realizacoes_por_periodo.getElemento(horizonte_otimizacao_DC.getElemento(maiorIdEstagioDC)));

			/////////////////////////////////////////////
			//arquivo_1: relato.rvX
			/////////////////////////////////////////////

			int estagio_DC = 0;

			while (std::getline(leituraArquivo_1, line)) {

				strNormalizada(line);

				if (line.substr(0, 41) == "    No.       Usina       Volume (% V.U.)") {

					estagio_DC++;

					int conteio_usina = 0;

					while (std::getline(leituraArquivo_1, line)) {

						atributo = line.substr(0, 7);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						const int codigo_usina = atoi(atributo.c_str());

						if (codigo_usina > 0) {

							const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

							if (idHidreletrica != IdHidreletrica_Nenhum) {

								conteio_usina++;

								atributo = line.substr(33, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								if (line.substr(33, 5) != "") {//Teste para só pegar as usinas com reservatório

									const double porcentagem_volume_meta = atof(atributo.c_str()) / 100;

									const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();
									const Periodo periodo_final = horizonte_estudo.getIteradorFinal();

									const IdEstagio idEstagio_DC = IdEstagio(estagio_DC);

									bool sobreposicao_encontrada = false;

									for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

										double sobreposicao = periodo.sobreposicao(horizonte_otimizacao_DC.getElemento(idEstagio_DC));

										if (sobreposicao == 1) {
											sobreposicao_encontrada = true;

											double volume_util_meta = porcentagem_volume_meta * (a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getElementoVetor(AttVetorReservatorio_volume_maximo, periodo, double()) - a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getElementoVetor(AttVetorReservatorio_volume_minimo, periodo, double()));

											//Identifica se o o dia final do periodo corresponde com o dia final do periodo_DC
											const Periodo periodo_dia_final_SPT = periodo.getPeriodoDiario_do_diaFinal(periodo);
											const Periodo periodo_dia_final_DC = horizonte_otimizacao_DC.getElemento(idEstagio_DC).getPeriodoDiario_do_diaFinal(horizonte_otimizacao_DC.getElemento(idEstagio_DC));

											if (periodo_dia_final_SPT != periodo_dia_final_DC)
												volume_util_meta = getdoubleFromChar("max");

											for (IdCenario idCenario = IdCenario_1; idCenario <= maiorIdCenarioDC; idCenario++)
												a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).addElemento(AttMatrizReservatorio_volume_meta, idCenario, periodo, volume_util_meta);

										}//if (sobreposicao == 1) {

										if (sobreposicao_encontrada && sobreposicao == 0)
											break;

									}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

								}//if (secao_volumes && codigo_usina > 0 && line.substr(33, 5) != "") {

							}//if (idHidreletrica != IdHidreletrica_Nenhum) {

						}//if (codigo_usina > 0) {

						if (IdHidreletrica(conteio_usina) == maiorIdHidreletrica)
							break;

					}//while (std::getline(leituraArquivo_1, line)) {

				}//if (line.substr(0, 41) == "    No.       Usina       Volume (% V.U.)") {

				if (IdEstagio(estagio_DC) == IdEstagio(horizonte_otimizacao_DC.getIteradorFinal() - 1))
					break;

			}//while (std::getline(leituraArquivo_1, line)) {

			leituraArquivo_1.clear();
			leituraArquivo_1.close();

			/////////////////////////////////////////////
			//arquivo_2: relato2.rvX
			/////////////////////////////////////////////

			int numero_cenarios = 0;

			while (std::getline(leituraArquivo_2, line)) {

				strNormalizada(line);

				if (line.substr(0, 41) == "    No.       Usina       Volume (% V.U.)") {

					numero_cenarios++;

					int conteio_usina = 0;

					while (std::getline(leituraArquivo_2, line)) {

						atributo = line.substr(0, 7);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						const int codigo_usina = atoi(atributo.c_str());

						if (codigo_usina > 0) {

							const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

							if (idHidreletrica != IdHidreletrica_Nenhum) {

								conteio_usina++;

								atributo = line.substr(33, 5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

								if (line.substr(33, 5) != "") {//Teste para só pegar as usinas com reservatório

									const IdCenario idCenario = IdCenario(numero_cenarios);

									const double porcentagem_volume_meta = atof(atributo.c_str()) / 100;

									const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();
									const Periodo periodo_final = horizonte_estudo.getIteradorFinal();

									bool sobreposicao_encontrada = false;

									for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

										double sobreposicao = periodo.sobreposicao(horizonte_otimizacao_DC.getElemento(horizonte_otimizacao_DC.getIteradorFinal()));

										if (sobreposicao == 1) {
											sobreposicao_encontrada = true;

											double volume_util_meta = porcentagem_volume_meta * (a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getElementoVetor(AttVetorReservatorio_volume_maximo, periodo, double()) - a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getElementoVetor(AttVetorReservatorio_volume_minimo, periodo, double()));

											//Identifica se o o dia final do periodo corresponde com o dia final do periodo_DC
											const Periodo periodo_dia_final_SPT = periodo.getPeriodoDiario_do_diaFinal(periodo);
											const Periodo periodo_dia_final_DC = horizonte_otimizacao_DC.getElemento(horizonte_otimizacao_DC.getIteradorFinal()).getPeriodoDiario_do_diaFinal(horizonte_otimizacao_DC.getElemento(horizonte_otimizacao_DC.getIteradorFinal()));

											if (periodo_dia_final_SPT != periodo_dia_final_DC)
												volume_util_meta = getdoubleFromChar("max");

											a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).addElemento(AttMatrizReservatorio_volume_meta, idCenario, periodo, volume_util_meta);

										}//if (sobreposicao == 1) {

										if (sobreposicao_encontrada && sobreposicao == 0)
											break;

									}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

								}//if (secao_volumes && codigo_usina > 0 && line.substr(33, 5) != "") {

							}//if (idHidreletrica != IdHidreletrica_Nenhum) {

						}//if (codigo_usina > 0) {

						if (IdHidreletrica(conteio_usina) == maiorIdHidreletrica)
							break;

					}//while (std::getline(leituraArquivo_2, line)) {

				}//if (line.substr(0, 41) == "    No.       Usina       Volume (% V.U.)") {

				if (IdCenario(numero_cenarios) == maiorIdCenarioDC)
					break;

			}//while (std::getline(leituraArquivo_2, line)) {


			leituraArquivo_2.clear();
			leituraArquivo_2.close();

		}//if (leituraArquivo_2.is_open() && leituraArquivo_1.is_open()) {

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_volumes_meta_from_relatos_DC: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_vRef_from_CadUsH_csv(Dados& a_dados, std::string a_nomeArquivo)
{
	try {

		int pos;
		std::string line;
		std::string atributo;

		std::ifstream leituraArquivo(a_nomeArquivo);

		if (leituraArquivo.is_open()) {

			//Descarta o cabeçalho
			std::getline(leituraArquivo, line);

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);

				pos = int(line.find(';'));
				atributo = line.substr(0, pos).c_str();
				atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

				line = line.substr(pos + 1, int(line.length()));

				const int codigo_usina = std::atoi(atributo.c_str());
				const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina);

				if (idHidreletrica != IdHidreletrica_Nenhum) {

					//Descarta info nao necessária
					for (int registro = 0; registro < 177; registro++) {
						pos = int(line.find(';'));
						line = line.substr(pos + 1, int(line.length()));
					}

					//Volume referência para o cálculo do turbinamento_máximo
					pos = int(line.find(';'));
					atributo = line.substr(0, pos).c_str();
					atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

					const double volume_referencia_calculo_turbinamento_maximo = atof(atributo.c_str());

					a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_volume_referencia_calculo_turbinamento_maximo, volume_referencia_calculo_turbinamento_maximo);


				}//if (idHidreletrica != IdHidreletrica_Nenhum) {

			}//while (std::getline(leituraArquivo, line)) {

		}//if (leituraArquivo.is_open()) {
		else
			throw std::invalid_argument("Nao foi possivel abrir o arquivo " + a_nomeArquivo);

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_vRef_from_CadUsH_csv: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::defineHorizontes_CP(Dados& a_dados)
{
	try {

		define_horizonte_otimizacao_CP(a_dados);
		define_horizonte_estudo_CP(a_dados);

	}//try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::defineHorizontes_CP(): \n" + std::string(erro.what())); }
}

void LeituraCEPEL::define_horizonte_otimizacao_CP(Dados& a_dados) {

	try {

		int conteioDias;

		std::string string_periodo;

		IdDia idDia;
		IdMes idMes;
		IdAno idAno;

		//*****************************************
		// Define horizonte otimização
		//*****************************************

		SmartEnupla<IdEstagio, Periodo> horizonte_otimizacao;

		//O DC no mínimo tem dois estágios (pode ocorrer em alguma RV4)
		if (idEstagioMaximo > horizonte_otimizacao_DC.getIteradorFinal())
			idEstagioMaximo = horizonte_otimizacao_DC.getIteradorFinal();

		if (idEstagioMaximo == IdEstagio_2) {

			//*****************************************
			// 1°estágio
			//*****************************************

			const Periodo periodo_inicial = data_execucao + 1;

			Periodo periodo_1(TipoPeriodo_semanal, periodo_inicial);
			horizonte_otimizacao.addElemento(periodo_1);

			//*****************************************
			// 2°estágio
			//*****************************************

			const Periodo periodo_ultimo_estagio_DC = horizonte_otimizacao_DC.getElemento(horizonte_otimizacao_DC.getIteradorFinal());

			const IdDia idDia_maiordiadomes = periodo_ultimo_estagio_DC.getMaiorDiaDoMes();
			const IdMes idMes_maiordiadomes = horizonte_otimizacao_DC.getElemento(horizonte_otimizacao_DC.getIteradorFinal()).getMes();
			const IdAno idAno_maiordiadomes = horizonte_otimizacao_DC.getElemento(horizonte_otimizacao_DC.getIteradorFinal()).getAno();

			const Periodo maiordiadomes(idDia_maiordiadomes, idMes_maiordiadomes, idAno_maiordiadomes);

			periodo_1++; //Aumenta 1 semana

			idDia = periodo_1.getDia();
			idMes = periodo_1.getMes();
			idAno = periodo_1.getAno();

			string_periodo = getString(idDia) + "/" + getString(idMes) + "/" + getString(idAno);

			Periodo periodo_diario(idDia, idMes, idAno);

			conteioDias = 1; //O primeiro dia conta na formação do periodo
			while (true) {

				periodo_diario++;

				if (periodo_diario > maiordiadomes)
					break;

				conteioDias++;

			}//while (true) {

			if (conteioDias == 1) { string_periodo += "-diario"; }
			else if (conteioDias == 7) { string_periodo += "-semanal"; }
			else if (conteioDias == int(idDia_maiordiadomes)) { string_periodo += "-mensal"; }
			else { string_periodo += "-" + getString(conteioDias) + "dias"; }

			const Periodo periodoMensal(string_periodo);


			const Periodo periodo_2(string_periodo);

			horizonte_otimizacao.addElemento(periodo_2);

			//*****************************************
			// Atualiza vetor horizonte_otimizacao
			//*****************************************

			if (horizonte_otimizacao.getIteradorFinal() != idEstagioMaximo)
				throw std::invalid_argument("idEstagioMaximo utilizado no vetor numero_aberturas nao corresponde ao iteradorFinal do horizonte_otimizacao");

		}//if (idEstagioMaximo == IdEstagio_2) {
		else if (idEstagioMaximo == IdEstagio_3) {

			//*****************************************
			// 1°estágio
			//*****************************************

			const Periodo periodo_inicial = data_execucao + 1;

			Periodo periodo_1(TipoPeriodo_semanal, periodo_inicial);
			horizonte_otimizacao.addElemento(periodo_1);

			//*****************************************
			// 2°estágio
			//*****************************************

			const Periodo periodo_ultimo_estagio_DC = horizonte_otimizacao_DC.getElemento(horizonte_otimizacao_DC.getIteradorFinal());

			periodo_1++; //Aumenta 1 semana

			idDia = periodo_1.getDia();
			idMes = periodo_1.getMes();
			idAno = periodo_1.getAno();

			string_periodo = getString(idDia) + "/" + getString(idMes) + "/" + getString(idAno);

			Periodo periodo_diario(idDia, idMes, idAno);

			conteioDias = 1; //O primeiro dia conta na formação do periodo
			while (true) {

				periodo_diario++;

				if (periodo_diario >= periodo_ultimo_estagio_DC)
					break;

				conteioDias++;

			}//while (true) {

			if (conteioDias == 1) { string_periodo += "-diario"; }
			else if (conteioDias == 7) { string_periodo += "-semanal"; }
			else { string_periodo += "-" + getString(conteioDias) + "dias"; }

			const Periodo periodo_2(string_periodo);

			horizonte_otimizacao.addElemento(periodo_2);

			//*****************************************
			// 3°estágio
			//*****************************************

			idDia = periodo_diario.getDia();
			idMes = periodo_diario.getMes();
			idAno = periodo_diario.getAno();

			string_periodo = getString(idDia) + "/" + getString(idMes) + "/" + getString(idAno);

			const IdDia idDia_maiordiadomes = periodo_diario.getMaiorDiaDoMes(horizonte_otimizacao_DC.getElemento(horizonte_otimizacao_DC.getIteradorFinal()).getMes());
			const IdMes idMes_maiordiadomes = horizonte_otimizacao_DC.getElemento(horizonte_otimizacao_DC.getIteradorFinal()).getMes();
			const IdAno idAno_maiordiadomes = horizonte_otimizacao_DC.getElemento(horizonte_otimizacao_DC.getIteradorFinal()).getAno();

			const Periodo maiordiadomes(idDia_maiordiadomes, idMes_maiordiadomes, idAno_maiordiadomes);

			conteioDias = 1; //O primeiro dia conta na formação do periodo
			while (true) {

				if (periodo_diario == maiordiadomes)
					break;

				periodo_diario++;
				conteioDias++;

			}//while (true) {


			if (conteioDias == 1) { string_periodo += "-diario"; }
			else if (conteioDias == 7) { string_periodo += "-semanal"; }
			else if (conteioDias == int(idDia_maiordiadomes)) { string_periodo += "-mensal"; }
			else { string_periodo += "-" + getString(conteioDias) + "dias"; }

			const Periodo periodo_3(string_periodo);

			horizonte_otimizacao.addElemento(periodo_3);

			//*****************************************
			// Atualiza vetor horizonte_otimizacao
			//*****************************************

			if (horizonte_otimizacao.getIteradorFinal() != idEstagioMaximo)
				throw std::invalid_argument("idEstagioMaximo utilizado no vetor numero_aberturas nao corresponde ao iteradorFinal do horizonte_otimizacao");

		}//else if (idEstagioMaximo == IdEstagio_3) {
		else
			throw std::invalid_argument("Numero de estagios para o horizonte_otimizacao nao implementado");


		a_dados.setVetor(AttVetorDados_horizonte_otimizacao, horizonte_otimizacao);

		if (!dadosPreConfig_instanciados) {

			a_dados.setAtributo(AttComumDados_estagio_final, horizonte_otimizacao.getIteradorFinal());
			a_dados.setAtributo(AttComumDados_estagio_acoplamento_pre_estudo, estagio_acoplamento_pre_estudo);

		}//if (!dadosPreConfig_instanciados) {

	}//try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::define_horizonte_otimizacao_CP(Dados &a_dados): \n" + std::string(erro.what())); }
}

void LeituraCEPEL::define_horizonte_estudo_CP(Dados& a_dados) {
	try {

		SmartEnupla<Periodo, IdEstagio>  horizonte_estudo;

		SmartEnupla<IdEstagio, Periodo> horizonte_otimizacao = a_dados.getVetor(AttVetorDados_horizonte_otimizacao, IdEstagio(), Periodo());

		//*****************************************
		// Define horizonte estudo
		//*****************************************

		const bool horizonte_estudo_semanal = true;
		const bool ultimo_estagio_mensal = true;

		if (idEstagioMaximo == IdEstagio_2) {

			if (horizonte_estudo_semanal) {

				const IdEstagio idEstagio_IteradorInicial = horizonte_otimizacao.getIteradorInicial();
				const IdEstagio idEstagio_IteradorFinal = horizonte_otimizacao.getIteradorFinal();

				for (IdEstagio idEstagio = idEstagio_IteradorInicial; idEstagio <= idEstagio_IteradorFinal; idEstagio++) {

					if (idEstagio == IdEstagio_1) {

						horizonte_estudo.addElemento(horizonte_otimizacao_DC.getElemento(IdEstagio_1), idEstagio);

					}//if (idEstagio == IdEstagio_1 {

					else if (idEstagio == IdEstagio_2) {

						if (ultimo_estagio_mensal) {

							horizonte_estudo.addElemento(horizonte_otimizacao_DC.getElemento(horizonte_otimizacao_DC.getIteradorFinal()), idEstagio);

						}//if (ultimo_estagio_mensal) {
						else {

							const IdEstagio maioridEstagio_DC = horizonte_otimizacao_DC.getIteradorFinal();

							for (IdEstagio idEstagio_DC = IdEstagio_2; idEstagio_DC <= maioridEstagio_DC; idEstagio_DC++) {

								if (idEstagio_DC < maioridEstagio_DC)
									horizonte_estudo.addElemento(horizonte_otimizacao_DC.getElemento(idEstagio_DC), idEstagio);
								else {

									Periodo periodo_final_DC = horizonte_otimizacao_DC.getElemento(horizonte_otimizacao_DC.getIteradorFinal());

									const IdDia idDia_maiordiadomes = periodo_final_DC.getMaiorDiaDoMes(horizonte_otimizacao_DC.getElemento(horizonte_otimizacao_DC.getIteradorFinal()).getMes());
									const IdMes idMes_maiordiadomes = periodo_final_DC.getMes();
									const IdAno idAno_maiordiadomes = periodo_final_DC.getAno();

									const Periodo maiordiadomes(idDia_maiordiadomes, idMes_maiordiadomes, idAno_maiordiadomes);

									int conteioDias = 1;

									Periodo periodo_diario_inicial = Periodo(TipoPeriodo_diario, periodo_final_DC);
									Periodo periodo_diario = periodo_diario_inicial;

									while (true) {

										if (conteioDias > 7) {

											Periodo periodo_semanal(TipoPeriodo_semanal, periodo_diario_inicial);
											horizonte_estudo.addElemento(periodo_semanal, idEstagio);

											conteioDias = 1;
											periodo_diario_inicial = periodo_diario;

										}//if (conteioDias == 7) {

										if (periodo_diario >= maiordiadomes) {

											std::string string_periodo = getString(periodo_diario_inicial.getDia()) + "/" + getString(periodo_diario_inicial.getMes()) + "/" + getString(periodo_diario_inicial.getAno());

											if (conteioDias == 7)
												string_periodo += "-semanal";
											else if (conteioDias == 1)
												string_periodo += "-diario";
											else if (conteioDias == int(idDia_maiordiadomes)) { string_periodo += "-mensal"; }
											else
												string_periodo += "-" + getString(conteioDias) + "dias";

											Periodo periodo_final(string_periodo);
											horizonte_estudo.addElemento(periodo_final, idEstagio);

											break;

										}//else if (periodo_diario >= maiordiadomes) {

										periodo_diario++;
										conteioDias++;

									}//while (true) {

								}//else {

							}//for (IdEstagio idEstagio_DC = IdEstagio_2; idEstagio_DC < maioridEstagio_DC; idEstagio_DC++) {

						}// else {

					}//else if (idEstagio == IdEstagio_2) {

				}//for (IdEstagio idEstagio = idEstagio_IteradorInicial; idEstagio <= idEstagio_IteradorFinal; idEstagio++) {

			}//if (horizonte_estudo_semanal) {
			else {
				throw std::invalid_argument("Metodo nao implementado");

			}//else {

		}//if (idEstagioMaximo == IdEstagio_2) {
		else if (idEstagioMaximo == IdEstagio_3) {

			if (horizonte_estudo_semanal) {

				for (IdEstagio idEstagio = horizonte_otimizacao.getIteradorInicial(); idEstagio <= horizonte_otimizacao.getIteradorFinal(); idEstagio++) {

					if (idEstagio == IdEstagio_1)
						horizonte_estudo.addElemento(horizonte_otimizacao_DC.getElemento(IdEstagio_1), idEstagio);

					else if (idEstagio == IdEstagio_2)
						for (IdEstagio idEstagio_DC = IdEstagio_2; idEstagio_DC < horizonte_otimizacao_DC.getIteradorFinal(); idEstagio_DC++)
							horizonte_estudo.addElemento(horizonte_otimizacao_DC.getElemento(idEstagio_DC), idEstagio);

					else if (idEstagio == IdEstagio_3) {

						if (ultimo_estagio_mensal) {

							horizonte_estudo.addElemento(horizonte_otimizacao_DC.getElemento(horizonte_otimizacao_DC.getIteradorFinal()), idEstagio);

						}//if (ultimo_estagio_mensal) {
						else {
							//Realiza uma discretizado em dias e semanas

							Periodo periodo_final_DC = horizonte_otimizacao_DC.getElemento(horizonte_otimizacao_DC.getIteradorFinal());

							const IdDia idDia_maiordiadomes = periodo_final_DC.getMaiorDiaDoMes(horizonte_otimizacao_DC.getElemento(horizonte_otimizacao_DC.getIteradorFinal()).getMes());
							const IdMes idMes_maiordiadomes = periodo_final_DC.getMes();
							const IdAno idAno_maiordiadomes = periodo_final_DC.getAno();

							const Periodo maiordiadomes(idDia_maiordiadomes, idMes_maiordiadomes, idAno_maiordiadomes);

							int conteioDias = 1;

							Periodo periodo_diario_inicial = Periodo(TipoPeriodo_diario, periodo_final_DC);
							Periodo periodo_diario = periodo_diario_inicial;

							while (true) {

								if (conteioDias > 7) {

									Periodo periodo_semanal(TipoPeriodo_semanal, periodo_diario_inicial);
									horizonte_estudo.addElemento(periodo_semanal, idEstagio);

									conteioDias = 1;
									periodo_diario_inicial = periodo_diario;

								}//if (conteioDias == 7) {

								if (periodo_diario >= maiordiadomes) {

									std::string string_periodo = getString(periodo_diario_inicial.getDia()) + "/" + getString(periodo_diario_inicial.getMes()) + "/" + getString(periodo_diario_inicial.getAno());

									if (conteioDias == 7)
										string_periodo += "-semanal";
									else if (conteioDias == 1)
										string_periodo += "-diario";
									else if (conteioDias == int(idDia_maiordiadomes)) { string_periodo += "-mensal"; }
									else
										string_periodo += "-" + getString(conteioDias) + "dias";

									Periodo periodo_final(string_periodo);
									horizonte_estudo.addElemento(periodo_final, idEstagio);

									break;

								}//else if (periodo_diario >= maiordiadomes) {

								periodo_diario++;
								conteioDias++;

							}//while (true) {

						}

					}//else if (idEstagio == IdEstagio_3) {

				}//for (IdEstagio idEstagio = idEstagio_IteradorInicial; idEstagio <= idEstagio_IteradorFinal; idEstagio++) {

			}//if (horizonte_estudo_semanal) {

		}//else if (idEstagioMaximo == IdEstagio_3) {

		else
			throw std::invalid_argument("Numero de estagios para o horizonte_otimizacao nao implementado");

		//*****************************************
		// Atualiza vetor horizonte_estudo
		//*****************************************

		a_dados.setVetor(AttVetorDados_horizonte_estudo, horizonte_estudo);

		for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
			horizonte_estudo_DECK.addElemento(periodo, true);

		if (!dadosPreConfig_instanciados)
			a_dados.setAtributo(AttComumDados_periodo_referencia, horizonte_estudo.getIteradorInicial());

	}//try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::define_horizonte_estudo_CP(Dados &a_dados): \n" + std::string(erro.what())); }
}

SmartEnupla<IdCenario, SmartEnupla<Periodo, IdRealizacao>> LeituraCEPEL::define_mapeamento_espaco_amostral_arvore_simetrica_CP(Dados& a_dados, const IdCenario a_cenario_inicial, const IdCenario a_cenario_final)
{
	try {

		//Filosofia: Somente serve se a árvore de cenários for simêtrica
		//Assume-se que o processo estocástico realiza como o DECOMP, i.e., semanal/semanal/.../mensal


		const int numero_cenarios = a_dados.getAtributo(AttComumDados_numero_cenarios, int());

		const IdCenario cenario_final = IdCenario(numero_cenarios);

		SmartEnupla<IdCenario, SmartEnupla<Periodo, IdRealizacao>> mapeamento_espaco_amostral_arvore_simetrica;
		SmartEnupla<IdCenario, SmartEnupla<Periodo, IdRealizacao>> mapeamento_espaco_amostral_arvore_simetrica_reduzida;

		////////////////////////////////////////////////////////////////
		//Cria vetor com o número de "sub-cenários" em cada nó da árvore
		////////////////////////////////////////////////////////////////

		//const IdEstagio idEstagio_inicial = horizonte_otimizacao_DC.getIteradorInicial();
		const IdEstagio idEstagio_final = horizonte_otimizacao_DC.getIteradorFinal();

		SmartEnupla<Periodo, int> numero_sub_cenarios;

		int sub_cenarios = numero_cenarios;

		for (IdEstagio idEstagio = estagio_acoplamento_pre_estudo; idEstagio <= idEstagio_final; idEstagio++) {

			sub_cenarios /= numero_realizacoes_por_periodo.getElemento(horizonte_otimizacao_DC.getElemento(idEstagio));
			numero_sub_cenarios.addElemento(horizonte_otimizacao_DC.at(idEstagio), sub_cenarios);

		}//for (IdEstagio idEstagio = idEstagio_inicial; idEstagio <= idEstagio_final; idEstagio++) {

		////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////
		//Realiza mapeamento espaço amostral
		/////////////////////////////////////////////////

		SmartEnupla<Periodo, int> conteio_sub_cenarios(numero_sub_cenarios, 0);
		SmartEnupla<Periodo, int> realizacao(numero_sub_cenarios, 1);


		for (IdCenario idCenario = IdCenario_1; idCenario <= cenario_final; idCenario++) {

			SmartEnupla<Periodo, IdRealizacao> mapeamento_espaco_amostral_arvore_simetrica_cenario;

			for (IdEstagio idEstagio = estagio_acoplamento_pre_estudo; idEstagio <= idEstagio_final; idEstagio++) {

				const Periodo periodo = horizonte_otimizacao_DC.at(idEstagio);

				const int sub_cenarios = numero_sub_cenarios.getElemento(periodo);

				if (conteio_sub_cenarios.getElemento(periodo) >= sub_cenarios) {

					const int aumento_realizacao = realizacao.getElemento(periodo) + 1;
					realizacao.setElemento(periodo, aumento_realizacao);

					conteio_sub_cenarios.setElemento(periodo, 0);

				}//if (conteio_sub_cenarios.getElemento(periodo) > sub_cenarios) {

				const IdRealizacao idRealizacao = IdRealizacao(realizacao.getElemento(periodo));

				mapeamento_espaco_amostral_arvore_simetrica_cenario.addElemento(periodo, idRealizacao);

				///////////

				const int aumento_sub_cenarios = conteio_sub_cenarios.getElemento(periodo) + 1;
				conteio_sub_cenarios.setElemento(periodo, aumento_sub_cenarios);

			}//for (IdEstagio idEstagio = estagio_acoplamento_pre_estudo; idEstagio <= idEstagio_final; idEstagio++) {

			/////////////////////////////////////////////////////////////////

			mapeamento_espaco_amostral_arvore_simetrica.addElemento(idCenario, mapeamento_espaco_amostral_arvore_simetrica_cenario);

		}//for (IdCenario idCenario = a_cenario_inicial; idCenario <= a_cenario_final; idCenario++) {

		///////////////////////////////////////////////////////////////////////
		//Reduzir mapeamento_espaco_amostral_arvore_simetrica 
		//de acordo aos IdCenario a_cenario_inicial, IdCenario a_cenario_final
		///////////////////////////////////////////////////////////////////////

		if (a_cenario_inicial == IdCenario_1 && a_cenario_final == cenario_final)
			mapeamento_espaco_amostral_arvore_simetrica_reduzida = mapeamento_espaco_amostral_arvore_simetrica;
		else {

			for (IdCenario idCenario = a_cenario_inicial; idCenario <= a_cenario_final; idCenario++)
				mapeamento_espaco_amostral_arvore_simetrica_reduzida.addElemento(idCenario, mapeamento_espaco_amostral_arvore_simetrica.getElemento(idCenario));

		}//else {

		return mapeamento_espaco_amostral_arvore_simetrica_reduzida;

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::define_mapeamento_espaco_amostral_arvore_simetrica_CP: \n" + std::string(erro.what())); }

}

/*
void LeituraCEPEL::define_realizacao_transformada_espaco_amostral_arvore_completa_CP(Dados& a_dados)
{
	try {

		//Horizonte do processo estocástico
		//const SmartEnupla<Periodo, IdEstagio> horizonte_processo_estocastico = a_dados.processoEstocastico_hidrologico.getVetor(AttVetorProcessoEstocastico_horizonte_processo_estocastico, Periodo(), IdEstagio());

		/////////////////////////////////////////////////
		//Determina o número de cenários da árvore
		/////////////////////////////////////////////////

		//const IdEstagio idEstagio_inicial = a_numero_realizacoes_por_estagio.getIteradorInicial();
		//const IdEstagio idEstagio_final = a_numero_realizacoes_por_estagio.getIteradorFinal();

		const int numero_cenarios = a_dados.getAtributo(AttComumDados_numero_cenarios, int());
		const IdCenario cenario_final = IdCenario(numero_cenarios);
		const IdCenario cenario_inicial = IdCenario_1;

		const IdVariavelAleatoria maiorIdVariavelAleatoria = a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.getMaiorId();

		const SmartEnupla <IdCenario, SmartEnupla<Periodo, IdRealizacao>> mapeamento_espaco_amostral = define_mapeamento_espaco_amostral_arvore_simetrica_CP(a_dados, cenario_inicial, cenario_final);

		/////////////////////////////////////////////////
		//Realiza mapeamento espaço amostral
		/////////////////////////////////////////////////

		for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

			for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= maiorIdVariavelAleatoria; idVariavelAleatoria++) {

				for (Periodo periodo = horizonte_processo_estocastico.getIteradorInicial(); periodo <= horizonte_processo_estocastico.getIteradorFinal(); horizonte_processo_estocastico.incrementarIterador(periodo)) {

					//const IdRealizacao idRealizacao = a_dados.processoEstocastico_hidrologico.getElementoMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, idCenario, periodo, IdRealizacao());

					const IdRealizacao idRealizacao = mapeamento_espaco_amostral.getElemento(idCenario).getElemento(periodo);

					const double afluencia = a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.att(idVariavelAleatoria).getElementoMatriz(AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodo, idRealizacao, double());

					a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.att(idVariavelAleatoria).addElemento(AttMatrizVariavelAleatoria_cenarios_realizacao_transformada_espaco_amostral, idCenario, periodo, afluencia);

				}//for (Periodo periodo = horizonte_processo_estocastico.getIteradorInicial(); periodo <= horizonte_processo_estocastico.getIteradorFinal(); horizonte_processo_estocastico.incrementarIterador(periodo)) {

			}//for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= maiorIdVariavelAleatoria; idVariavelAleatoria++) {

		}//for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

	}//try {
	catch (const std::exception & erro) { throw std::invalid_argument("LeituraCEPEL::define_realizacao_transformada_espaco_amostral_arvore_completa_CP(Dados& a_dados): \n" + std::string(erro.what())); }

}//void LeituraCEPEL::define_realizacao_transformada_espaco_amostral_DC(Dados& a_dados)
*/

void LeituraCEPEL::define_numero_cenarios_CP(Dados& a_dados) {

	try {

		/////////////////////////////////////////////////
		//Determina o número de cenários da árvore
		/////////////////////////////////////////////////

		if (a_dados.processoEstocastico_hidrologico.getSizeMatriz(AttMatrizProcessoEstocastico_mapeamento_arvore_cenarios) > 0)
			a_dados.setAtributo(AttComumDados_numero_cenarios, a_dados.processoEstocastico_hidrologico.getSizeMatriz(AttMatrizProcessoEstocastico_mapeamento_arvore_cenarios));
		else if (int(numero_realizacoes_por_periodo.size())) {

			//Assume-se que a árvore é simêtrica

			int numero_cenarios = 1;

			for (Periodo periodo = horizonte_processo_estocastico.getIteradorInicial(); periodo <= horizonte_processo_estocastico.getIteradorFinal(); horizonte_processo_estocastico.incrementarIterador(periodo))
				numero_cenarios *= numero_realizacoes_por_periodo.getElemento(periodo);

			a_dados.setAtributo(AttComumDados_numero_cenarios, numero_cenarios);

		}//else if (int(numero_realizacoes_por_periodo.size())) {
		else
			throw std::invalid_argument("Deve ser informado o mapeamento_arvore_cenarios ou numero_realizacoes_por_periodo");

		a_dados.setAtributo(AttComumDados_visitar_todos_cenarios_por_iteracao, true);

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::define_numero_cenarios_CP(Dados& a_dados): \n" + std::string(erro.what())); }

}


void LeituraCEPEL::instanciar_variavelAleatoria_realizacao_from_vazoes_DC(Dados& a_dados) {

	try {

		const IdHidreletrica  maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

		const int numero_nos = int(vazao_no_posto.size());

		for (int no = 0; no < numero_nos; no++) {

			/////////////////////////////////////////////////////////////////////////////////////
			//Identifica o estágio do registro
			//Filosofia: Devido à estrutura da árvore DECOMP  o arquivo de VAZOES.DAT
			//o idRealizacao = idNo
			/////////////////////////////////////////////////////////////////////////////////////

			for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

				const IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria(idHidreletrica);

				const int posto = a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_codigo_posto, int());

				const double afluencia = vazao_no_posto.at(no).at(posto - 1);

				a_dados.processoEstocastico_hidrologico.addElemento(AttMatrizProcessoEstocastico_variavelAleatoria_realizacao, IdRealizacao(no + 1), idVariavelAleatoria, afluencia);

			}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

		}//for (int no = 0; no < numero_nos; no++) {

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::instanciar_variavelAleatoria_realizacao_from_vazoes_DC: \n" + std::string(erro.what())); }

} // void LeituraCEPEL::define_afluencia_postos_CP(Dados& a_dados) {


void LeituraCEPEL::define_afluencia_arvore_de_cenarios_postos_CP(Dados& a_dados) {

	try {

		const IdHidreletrica  maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

		const int numero_nos = int(vazao_no_posto.size());

		for (int no = 0; no < numero_nos; no++) {

			/////////////////////////////////////////////////////////////////////////////////////
			//Identifica o estágio do registro
			//Filosofia: Devido à estrutura da árvore DECOMP  o arquivo de VAZOES.DAT
			//o idNo = idEstagio para t < T
			/////////////////////////////////////////////////////////////////////////////////////

			IdEstagio idEstagio_DC = IdEstagio_Nenhum;

			const int numero_estagios_DC = int(horizonte_otimizacao_DC.size());

			if (no + 1 < numero_estagios_DC)
				idEstagio_DC = IdEstagio(no + 1);
			else
				idEstagio_DC = horizonte_otimizacao_DC.getIteradorFinal();


			for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

				const IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria(idHidreletrica);

				const int posto = a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_codigo_posto, int());

				const double afluencia = vazao_no_posto.at(no).at(posto - 1);

				if (estagio_acoplamento_pre_estudo <= idEstagio_DC) {

					IdRealizacao idRealizacao;

					if (idEstagio_DC == horizonte_otimizacao_DC.getIteradorFinal())
						idRealizacao = IdRealizacao(no - numero_estagios_DC + 2); //Cada registro do último estágio é uma realização
					else
						idRealizacao = IdRealizacao_1;

					a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.att(idVariavelAleatoria).addElemento(AttMatrizVariavelAleatoria_residuo_espaco_amostral, horizonte_otimizacao_DC.at(idEstagio_DC), idRealizacao, afluencia);

				}//if (estagio_acoplamento_pre_estudo <= idEstagio_DC) {
				else {

					//Para t<estagio_acoplamento_pre_estudo, esta informação é guardada em valor_afluencia_estagios_DC_anteriores_estagio_acoplamento_pre_estudo e logo em VARIAVEL_ALEATORIA_INTERNA_cenarios_realizacao_espaco_amostral
					valor_afluencia_estagios_DC_anteriores_estagio_acoplamento_pre_estudo.at(idVariavelAleatoria).addElemento(horizonte_otimizacao_DC.at(idEstagio_DC), afluencia);

				}//else {

			}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

		}//for (int no = 0; no < numero_nos; no++) {

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::define_afluencia_arvore_de_cenarios_postos_CP: \n" + std::string(erro.what())); }

} // void LeituraCEPEL::define_afluencia_postos_CP(Dados& a_dados) {

void LeituraCEPEL::define_variavel_aleatoria_interna_CP(Dados& a_dados, const IdCenario a_cenario_inicial, const IdCenario a_cenario_final)
{
	try {

		//Nota: A VariavelAleatoriaInterna armazena os valores de afluência passadas + tendência (afluências menores ao periodo de acoplamento) + residuo_espaco_amostral (processo estocástico)
		//Logo na validação imprime na afluência_tendencia para o problema de MP, e limita a VariavelAleatoriaInterna somente à tendência

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());
		const SmartEnupla<IdEstagio, Periodo> horizonte_otimizacao = a_dados.getVetor(AttVetorDados_horizonte_otimizacao, IdEstagio(), Periodo());

		const IdHidreletrica  maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

		SmartEnupla<Periodo, SmartEnupla<IdHidreletrica, double>> afluencia_incremental_periodo_hidreletrica_GEVAZP;

		if (int(valor_afluencia_passadas_GEVAZP.size()) > 0) {//Somente reporta a tendência das afluências passadas se são incluídos os arquivos GEVAZP: VAZOES.DAT e PREVS.RVX

			//Inicializa vetores de afluência natural das hidréletricas por periodo

			const Periodo periodo_inicial_historico = valor_afluencia_passadas_GEVAZP.at(0).getIteradorInicial();
			const Periodo periodo_final_historico = valor_afluencia_passadas_GEVAZP.at(0).getIteradorFinal();

			for (Periodo periodo = periodo_inicial_historico; periodo <= periodo_final_historico; valor_afluencia_passadas_GEVAZP.at(0).incrementarIterador(periodo)) {

				SmartEnupla<IdHidreletrica, double> afluencia_natural_hidreletrica_GEVAZP;

				for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

					const int posto = a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_codigo_posto, int());
					
					if (posto == 168) { //Sobradinho (Posto incremental 168 + as afluências naturais dos postos a montante para obter a natural de sobradinho)

						double vazao_afluencia_natural = valor_afluencia_passadas_GEVAZP.at(posto - 1).getElemento(periodo);

						const int numero_hidreletricas_montante = a_dados.getSizeVetor(idHidreletrica, AttVetorHidreletrica_montante);
						for (int m = 1; m <= numero_hidreletricas_montante; m++) {
							const IdHidreletrica idHidreletrica_montante = a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_montante, m, IdHidreletrica());

							vazao_afluencia_natural += valor_afluencia_passadas_GEVAZP.at(a_dados.vetorHidreletrica.att(idHidreletrica_montante).getAtributo(AttComumHidreletrica_codigo_posto, int()) - 1).getElemento(periodo);

						}//for (int m = 1; m <= numero_hidreletricas_montante; m++) {

						afluencia_natural_hidreletrica_GEVAZP.addElemento(idHidreletrica, vazao_afluencia_natural);

					}//if (posto == 168) {
					else
						afluencia_natural_hidreletrica_GEVAZP.addElemento(idHidreletrica, valor_afluencia_passadas_GEVAZP.at(posto - 1).getElemento(periodo));
					

				}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

				//Calcula a afluência incremental
				const SmartEnupla<IdHidreletrica, double> afluencia_incremental_hidreletrica_GEVAZP = a_dados.calculaAfluenciaIncremental(afluencia_natural_hidreletrica_GEVAZP);

				afluencia_incremental_periodo_hidreletrica_GEVAZP.addElemento(periodo, afluencia_incremental_hidreletrica_GEVAZP);

			}//for (Periodo periodo = periodo_inicial_historico; periodo <= periodo_final_historico; valor_afluencia_passadas.at(0).incrementarIterador(periodo)) {

		}//if (int(valor_afluencia_passadas_GEVAZP.size()) > 0) {

		SmartEnupla<Periodo, SmartEnupla<IdHidreletrica, double>> afluencia_incremental_periodo_hidreletrica_historico;
		SmartEnupla<Periodo, SmartEnupla<IdHidreletrica, double>> afluencia_natural_periodo_hidreletrica_historico;

		if (int(valor_afluencia_historica.size()) > 0) {//Somente reporta a tendência das afluências passadas se são incluídos os arquivos GEVAZP: VAZOES.DAT e PREVS.RVX

			//Inicializa vetores de afluência natural das hidréletricas por periodo

			const Periodo periodo_inicial_historico = valor_afluencia_historica.at(0).getIteradorInicial();
			const Periodo periodo_final_historico = valor_afluencia_historica.at(0).getIteradorFinal();

			for (Periodo periodo = periodo_inicial_historico; periodo <= periodo_final_historico; valor_afluencia_historica.at(0).incrementarIterador(periodo)) {

				SmartEnupla<IdHidreletrica, double> afluencia_natural_hidreletrica_historica;

				for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

					int posto = a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_codigo_posto, int());

					//if (lista_hidreletrica_NPOSNW.getElemento(idHidreletrica) > -1)
						//posto = lista_hidreletrica_NPOSNW.getElemento(idHidreletrica);
					
					if (posto == 168) { //Sobradinho (Posto incremental 168 + as afluências naturais dos postos a montante para obter a natural de sobradinho)
						
						double vazao_afluencia_natural = valor_afluencia_historica.at(posto - 1).getElemento(periodo);

						const int numero_hidreletricas_montante = a_dados.getSizeVetor(idHidreletrica, AttVetorHidreletrica_montante);
						for (int m = 1; m <= numero_hidreletricas_montante; m++) {
							const IdHidreletrica idHidreletrica_montante = a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_montante, m, IdHidreletrica());

							vazao_afluencia_natural += valor_afluencia_historica.at(a_dados.vetorHidreletrica.att(idHidreletrica_montante).getAtributo(AttComumHidreletrica_codigo_posto, int()) - 1).getElemento(periodo);

						}//for (int m = 1; m <= numero_hidreletricas_montante; m++) {

						afluencia_natural_hidreletrica_historica.addElemento(idHidreletrica, vazao_afluencia_natural);

					}//if (posto == 168) {
					else
						afluencia_natural_hidreletrica_historica.addElemento(idHidreletrica, valor_afluencia_historica.at(posto - 1).getElemento(periodo));

				}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

				//Calcula a afluência incremental
				const SmartEnupla<IdHidreletrica, double> afluencia_incremental_hidreletrica_historico = a_dados.calculaAfluenciaIncremental(afluencia_natural_hidreletrica_historica);

				afluencia_incremental_periodo_hidreletrica_historico.addElemento(periodo, afluencia_incremental_hidreletrica_historico);
				afluencia_natural_periodo_hidreletrica_historico.addElemento(periodo, afluencia_natural_hidreletrica_historica);

			}//for (Periodo periodo = periodo_inicial_historico; periodo <= periodo_final_historico; valor_afluencia_passadas.at(0).incrementarIterador(periodo)) {

		}//if (int(valor_afluencia_historica.size()) > 0) {

		for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

			const int posto = a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_codigo_posto, int());

			const IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria(idHidreletrica);

			VariavelAleatoriaInterna variavelAleatoriaInterna;
			variavelAleatoriaInterna.setAtributo(AttComumVariavelAleatoriaInterna_idVariavelAleatoriaInterna, IdVariavelAleatoriaInterna_1);
			variavelAleatoriaInterna.setAtributo(AttComumVariavelAleatoriaInterna_nome, getFullString(idHidreletrica));

			a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.att(idVariavelAleatoria).vetorVariavelAleatoriaInterna.add(variavelAleatoriaInterna);

			////////////////////////

			const Periodo periodo_acoplamento = horizonte_otimizacao.getElemento(a_dados.getAtributo(AttComumDados_estagio_acoplamento_pre_estudo, IdEstagio()));

			// XXX DECOMP

			for (Periodo periodo = horizonte_processo_estocastico.getIteradorInicial(); periodo <= horizonte_processo_estocastico.getIteradorFinal(); horizonte_processo_estocastico.incrementarIterador(periodo)) {

				a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.att(idVariavelAleatoria).vetorVariavelAleatoriaInterna.att(IdVariavelAleatoriaInterna_1).addElemento(AttVetorVariavelAleatoriaInterna_grau_liberdade, periodo, 0.0);

				if (periodo >= periodo_acoplamento) {
					a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.att(idVariavelAleatoria).addElemento(AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, periodo, 1, 0.0);
					a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.att(idVariavelAleatoria).vetorVariavelAleatoriaInterna.att(IdVariavelAleatoriaInterna_1).addElemento(AttVetorVariavelAleatoriaInterna_coeficiente_participacao, periodo, 1.0);
				} // if (periodo >= periodo_acoplamento) {

			}//for (Periodo periodo = horizonte_processo_estocastico.getIteradorInicial(); periodo <= horizonte_processo_estocastico.getIteradorFinal(); horizonte_processo_estocastico.incrementarIterador(periodo)) {

			for (IdCenario idCenario = a_cenario_inicial; idCenario <= a_cenario_final; idCenario++) {

				///////////////////////////////////////////////
				//1. Afluências passadas
				///////////////////////////////////////////////

				if (int(valor_afluencia_passadas_GEVAZP.size()) > 0) { //Somente reporta a tendência das afluências passadas se são incluídos os arquivos GEVAZP: VAZOES.DAT e PREVS.RVX

					const Periodo periodo_inicial_historico = valor_afluencia_passadas_GEVAZP.at(posto - 1).getIteradorInicial();
					const Periodo periodo_final_historico = valor_afluencia_passadas_GEVAZP.at(posto - 1).getIteradorFinal();

					for (Periodo periodo = periodo_inicial_historico; periodo <= periodo_final_historico; valor_afluencia_passadas_GEVAZP.at(posto - 1).incrementarIterador(periodo)) {

						double afluencia_incremental;

						if (posto == 300)//posto com incremental 0
							afluencia_incremental = 0;
						else if (lista_hidreletrica_NPOSNW.getElemento(idHidreletrica) > -1)
							afluencia_incremental = afluencia_incremental_periodo_hidreletrica_GEVAZP.at(periodo).getElemento(idHidreletrica);
						else if (lista_hidreletrica_NPOSNW.getElemento(idHidreletrica) == -1)
							afluencia_incremental = valor_afluencia_passadas_DECOMP.at(posto - 1).getElemento(periodo);

						a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.att(idVariavelAleatoria).vetorVariavelAleatoriaInterna.att(IdVariavelAleatoriaInterna_1).addElemento(AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral, idCenario, periodo, afluencia_incremental);

						a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.att(idVariavelAleatoria).vetorVariavelAleatoriaInterna.att(IdVariavelAleatoriaInterna_1).addElemento(AttMatrizVariavelAleatoriaInterna_tendencia_temporal, idCenario, periodo, afluencia_incremental);

					}//for (Periodo periodo = periodo_inicial_historico; periodo <= periodo_final_historico; valor_afluencia_passadas.at(posto-1).incrementarIterador(periodo)) {

				}//if (int(valor_afluencia_passadas.size()) > 0) {

				///////////////////////////////////////////////
				//2. Afluências histórico
				///////////////////////////////////////////////

				if ((int(valor_afluencia_historica.size()) > 0) && (idCenario == IdCenario_1) ) { //Somente reporta a o histórico das afluências se são incluídos os arquivos GEVAZP: VAZOES.DAT

					const Periodo periodo_inicial_historico = valor_afluencia_historica.at(posto - 1).getIteradorInicial();
					const Periodo periodo_final_historico = valor_afluencia_historica.at(posto - 1).getIteradorFinal();

					for (Periodo periodo = periodo_inicial_historico; periodo <= periodo_final_historico; valor_afluencia_historica.at(posto - 1).incrementarIterador(periodo)) {

						double afluencia_incremental;

						if (posto == 300)//posto com incremental 0
							afluencia_incremental = 0;
						else 
							afluencia_incremental = afluencia_incremental_periodo_hidreletrica_historico.at(periodo).getElemento(idHidreletrica);

						if (!a_dados.vetorHidreletrica.att(idHidreletrica).vetorAfluencia.isInstanciado(IdAfluencia_vazao_afluente)) {
							Afluencia afluencia;
							afluencia.setAtributo(AttComumAfluencia_idAfluencia, IdAfluencia_vazao_afluente);
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorAfluencia.add(afluencia);
						}

						a_dados.vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).addElemento(AttVetorAfluencia_incremental_historico, periodo, afluencia_incremental);
						//a_dados.vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).addElemento(AttVetorAfluencia_natural_historico, periodo, afluencia_natural_periodo_hidreletrica_historico.at(periodo).getElemento(idHidreletrica));

					}//for (Periodo periodo = periodo_inicial_historico; periodo <= periodo_final_historico; valor_afluencia_passadas.at(posto-1).incrementarIterador(periodo)) {

				}//if (int(valor_afluencia_passadas.size()) > 0) {

				////////////////////////////////////////////////////////
				//3. Afluências da árvore de cenários
				////////////////////////////////////////////////////////

				for (Periodo periodo = horizonte_processo_estocastico.getIteradorInicial(); periodo <= horizonte_processo_estocastico.getIteradorFinal(); horizonte_processo_estocastico.incrementarIterador(periodo)) {

					const IdNo idNo = a_dados.processoEstocastico_hidrologico.getElementoMatriz(AttMatrizProcessoEstocastico_mapeamento_arvore_cenarios, idCenario, periodo, IdNo());
					const IdRealizacao idRealizacao = a_dados.processoEstocastico_hidrologico.getElementoMatriz(AttMatrizProcessoEstocastico_no_realizacao, periodo, idNo, IdRealizacao());

					const double afluencia = a_dados.processoEstocastico_hidrologico.getElementoMatriz(AttMatrizProcessoEstocastico_variavelAleatoria_realizacao, idRealizacao, idVariavelAleatoria, double());

					a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.att(idVariavelAleatoria).vetorVariavelAleatoriaInterna.att(IdVariavelAleatoriaInterna_1).addElemento(AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral, idCenario, periodo, afluencia);

					if (periodo < periodo_acoplamento)
						a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.att(idVariavelAleatoria).vetorVariavelAleatoriaInterna.att(IdVariavelAleatoriaInterna_1).addElemento(AttMatrizVariavelAleatoriaInterna_tendencia_temporal, idCenario, periodo, afluencia);

				}//for (Periodo periodo = horizonte_processo_estocastico.getIteradorInicial(); periodo <= horizonte_processo_estocastico.getIteradorFinal(); horizonte_processo_estocastico.incrementarIterador(periodo)) {

			}//for (IdCenario idCenario = a_cenario_inicial; idCenario <= a_cenario_final; idCenario++) {

		}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::define_variavel_aleatoria_interna_CP(Dados& a_dados): \n" + std::string(erro.what())); }

}//void LeituraCEPEL::define_variavel_aleatoria_interna_CP(Dados& a_dados, const IdCenario a_cenario_inicial, const IdCenario a_cenario_final)

/*
void LeituraCEPEL::inicializarPenalidades_DC(Dados& a_dados)
{

	try {

		//Horizonte de estudo
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const IdHidreletrica maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

		for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

			const IdSubmercado idSubmercado = a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_submercado, IdSubmercado());
			const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, horizonte_estudo.getIteradorInicial(), IdPatamarCarga());

			const double penalidade = a_dados.getElementoMatriz(idSubmercado, IdPatamarDeficit_1, AttMatrizPatamarDeficit_custo, horizonte_estudo.getIteradorInicial(), maiorIdPatamarCarga, double());

			if(a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_penalidade_desvio_agua, double()) == 0)//Atributo que pode ser informado no DECK DC
				a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_penalidade_desvio_agua, penalidade);

			a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_penalidade_turbinamento_minimo, penalidade);
			a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_penalidade_vazao_defluente_minima, penalidade);
			a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_penalidade_vazao_defluente_maxima, penalidade);
			a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_penalidade_volume_minimo, penalidade);
			a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_penalidade_potencia_minima, penalidade);

		}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

	}//try {
	catch (const std::exception & erro) { throw std::invalid_argument("LeituraCEPEL::inicializarPenalidades_DC(): \n" + std::string(erro.what())); }

}//void LeituraCEPEL::inicializarPenalidades_DC()
*/

void LeituraCEPEL::modifica_lista_jusante_hidreletrica_com_casos_validados_CP(Dados& a_dados) {

	try {

		IdHidreletrica idHidreletrica;
		IdHidreletrica idHidreletrica_jusante;

		//Conecta Irape->Itapebi

		idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, 148); //codigo_ONS = 148 -> IRAPE
		idHidreletrica_jusante = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, 154); //codigo_ONS = 154 -> ITAPEBI

		if (!hidreletricasPreConfig_instanciadas)
			a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_jusante, idHidreletrica_jusante);
		else if (hidreletricasPreConfig_instanciadas)
			lista_jusante_hidreletrica.setElemento(idHidreletrica, idHidreletrica_jusante);

		//Conecta Maua->Capivara

		idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, 57); //codigo_ONS = 57 -> MAUA
		idHidreletrica_jusante = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, 61); //codigo_ONS = 61 -> CAPIVARA

		if (!hidreletricasPreConfig_instanciadas)
			a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_jusante, idHidreletrica_jusante);
		else if (hidreletricasPreConfig_instanciadas)
			lista_jusante_hidreletrica.setElemento(idHidreletrica, idHidreletrica_jusante);

		//Conecta Estreito Toc->Tucurui

		idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, 267); //codigo_ONS = 267 -> ESTREITO TOC
		idHidreletrica_jusante = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, 275); //codigo_ONS = 275 -> TUCURUI

		if (!hidreletricasPreConfig_instanciadas)
			a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_jusante, idHidreletrica_jusante);
		else if (hidreletricasPreConfig_instanciadas)
			lista_jusante_hidreletrica.setElemento(idHidreletrica, idHidreletrica_jusante);

		//Conecta B.Coqueiros->Foz R.Claro

		idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, 312); //codigo_ONS = 312 -> B. COQUEIROS
		idHidreletrica_jusante = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, 315); //codigo_ONS = 315 -> FOZ R. CLARO

		if (!hidreletricasPreConfig_instanciadas)
			a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_jusante, idHidreletrica_jusante);
		else if (hidreletricasPreConfig_instanciadas)
			lista_jusante_hidreletrica.setElemento(idHidreletrica, idHidreletrica_jusante);

		//Conecta Espora->I.Solteira

		idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, 290); //codigo_ONS = 290 -> ESPORA 
		idHidreletrica_jusante = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, 34); //codigo_ONS = 34 -> I. SOLTEIRA 

		if (!hidreletricasPreConfig_instanciadas)
			a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_jusante, idHidreletrica_jusante);
		else if (hidreletricasPreConfig_instanciadas)
			lista_jusante_hidreletrica.setElemento(idHidreletrica, idHidreletrica_jusante);

		//Conecta Sta Cecilia->Simplicio

		idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, 125); //codigo_ONS = 125 -> STA CECILIA 
		idHidreletrica_jusante = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, 129); //codigo_ONS = 129 -> SIMPLICIO 

		if (!hidreletricasPreConfig_instanciadas)
			a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_jusante, idHidreletrica_jusante);
		else if (hidreletricasPreConfig_instanciadas)
			lista_jusante_hidreletrica.setElemento(idHidreletrica, idHidreletrica_jusante);

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::modifica_lista_jusante_hidreletrica_com_casos_validados_CP: \n" + std::string(erro.what())); }

} // void LeituraCEPEL::modifica_lista_jusante_hidreletrica_com_casos_validados_CP(Dados& a_dados) {



void LeituraCEPEL::inicializa_vazao_defluente_CP(Dados& a_dados) {

	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());
		const IdHidreletrica maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

		//Inicializa matriz com a vazao_defluente_minima
		for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

			if (a_dados.getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_defluente_minima) == 0)
				a_dados.vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_vazao_defluente_minima, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_vazao_defluente_minima, double())));

		}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

		//Se a vazao_defluente_minima_historica for maior, atualiza matriz

		if (int(porcentagem_vazao_minima_historica_REE.size()) > 0) {

			for (int codigo_ONS_REE = 1; codigo_ONS_REE <= maior_ONS_REE; codigo_ONS_REE++) {

				const std::vector<IdHidreletrica> idHidreletricas_REE = getIdsFromCodigoONS(lista_codigo_ONS_REE, codigo_ONS_REE);

				const int numero_usinas_no_REE = int(idHidreletricas_REE.size());

				for (int usina_REE = 0; usina_REE < numero_usinas_no_REE; usina_REE++) {

					const IdHidreletrica idHidreletrica = idHidreletricas_REE.at(usina_REE);

					set_zero_vazao_defluente_minima_historica_usina_fio_sem_reservatorio_a_montante(a_dados, idHidreletrica);

					const double vazao_defluente_minima_historica = a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_vazao_defluente_minima_historica, double());

					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							if (a_dados.vetorHidreletrica.att(idHidreletrica).getElementoVetor(AttVetorHidreletrica_vazao_defluente_minima, periodo, double()) < vazao_defluente_minima_historica * porcentagem_vazao_minima_historica_REE.at(codigo_ONS_REE).getElemento(periodo))
								a_dados.vetorHidreletrica.att(idHidreletrica).setElemento(AttVetorHidreletrica_vazao_defluente_minima, periodo, vazao_defluente_minima_historica * porcentagem_vazao_minima_historica_REE.at(codigo_ONS_REE).getElemento(periodo));

					}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				}//for (int usina_REE = 0; usina_REE < numero_usinas_no_REE; usina_REE++)

			}//for (int codigo_ONS_REE = 1; codigo_ONS_REE <= maior_ONS_REE; codigo_ONS_REE++) {

		}//if (int(porcentagem_vazao_minima_historica_REE.size()) > 0) {

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::inicializa_vazao_defluente_CP_CP: \n" + std::string(erro.what())); }

} // void LeituraCEPEL::inicializa_vazao_defluente_CP(Dados& a_dados) {


void LeituraCEPEL::valida_bacia_sao_francisco(Dados& a_dados){

	try{

		for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica++) {

			if (a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_codigo_posto, int()) == 168) {

				for (IdHidreletrica idHidreletrica_jus = IdHidreletrica_1; idHidreletrica_jus <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica_jus++) {

					const int posto = a_dados.getAtributo(idHidreletrica_jus, AttComumHidreletrica_codigo_posto, int());

					if ((posto == 172) || (posto == 173) || (posto == 175) || (posto == 178))
						a_dados.vetorHidreletrica.att(idHidreletrica_jus).setAtributo(AttComumHidreletrica_codigo_posto, 300);

				} // for (IdHidreletrica idHidreletrica_jus = IdHidreletrica_1; idHidreletrica_jus <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica_jus++) {

			} // if (a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_codigo_posto, int()) == 168) {

		} // for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica++) {


	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::valida_bacia_sao_francisco: \n" + std::string(erro.what())); }

} // void LeituraCEPEL::valida_bacia_sao_francisco(Dados& a_dados){



void LeituraCEPEL::imprime_na_tela_avisos_de_possiveis_inviabilidades_fph(Dados& a_dados) {

	try {

		if (idEstagioMaximo > IdEstagio_2) {

			////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//1. Identifica as usinas sem montante nem jusante/jusante_desvio
			////////////////////////////////////////////////////////////////////////////////////////////////////////////

			const IdHidreletrica maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

			std::vector<IdHidreletrica> vetor_hidreletrica_aislada; //usinas sem montante nem jusante

			for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

				if (a_dados.getSizeVetor(idHidreletrica, AttVetorHidreletrica_montante) == 0 && a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_jusante, IdHidreletrica()) == IdHidreletrica_Nenhum && a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_jusante_desvio, IdHidreletrica()) == IdHidreletrica_Nenhum)
					vetor_hidreletrica_aislada.push_back(idHidreletrica);

			}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

			////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//2. Calcula a afluencia_minima para tornar a FPH viável na pior condiçao de volume (v = 0)
			//Nota: Este procedimento é realizado somente para o último periodo
			////////////////////////////////////////////////////////////////////////////////////////////////////////////

			const Periodo periodo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio()).getIteradorFinal();

			const int size_vetor_hidreletrica_aislada = int(vetor_hidreletrica_aislada.size());

			for (int pos = 0; pos < size_vetor_hidreletrica_aislada; pos++) {

				const IdHidreletrica idHidreletrica = vetor_hidreletrica_aislada.at(pos);

				const int numero_planos = a_dados.getSize2Matriz(idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, periodo);

				double afluencia_minima_para_viabilidade = 0;

				for (int i = 1; i <= numero_planos; i++) {

					const double rhs = a_dados.getElementoMatriz(idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, periodo, i, double());
					const double coef_qh = a_dados.getElementoMatriz(idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_QH, periodo, i, double());

					if (afluencia_minima_para_viabilidade < rhs * std::pow(coef_qh, -1))
						afluencia_minima_para_viabilidade = rhs * std::pow(coef_qh, -1);

				}//for (int i = 1; i <= numero_planos; i++) {

				if (a_dados.getAtributo(AttComumDados_representar_producao_hidreletrica_com_turbinamento_disponivel, bool()))
					afluencia_minima_para_viabilidade /= a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_disponibilidade, periodo, double());

				//Compara a afluencia_minima_para_viabilidade com as afluencias da árvore de cenários

				const IdCenario cenario_inicial = a_dados.getAtributo(AttComumDados_menor_cenario_do_processo, IdCenario());
				const IdCenario cenario_final = a_dados.getAtributo(AttComumDados_maior_cenario_do_processo, IdCenario());

				const IdProcesso idProcesso = a_dados.getAtributo(AttComumDados_idProcesso, IdProcesso());

				for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

					if (afluencia_minima_para_viabilidade > a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.att(IdVariavelAleatoria(idHidreletrica)).getElementoMatriz(AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodo, IdRealizacao(idCenario), double())) {
						std::cout << getFullString(idProcesso) << " -Possivel inviabilidade na FPH da usina_ " << a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_nome, std::string()) << " " << getFullString(idHidreletrica) << " -Valor de afluencia no periodo_mensal inferior a " << afluencia_minima_para_viabilidade << std::endl;
						break;
					}//if (afluencia_minima_para_viabilidade > a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.att(IdVariavelAleatoria(idHidreletrica)).getElementoMatriz(AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodo, IdRealizacao(idCenario), double())) {

				}//for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

			}//for (int pos = 0; pos < size_vetor_hidreletrica_aislada; pos++) {

		}//if (idEstagioMaximo > IdEstagio_2) {

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::imprime_na_tela_avisos_de_possiveis_inviabilidades_fph: \n" + std::string(erro.what())); }

} // void LeituraCEPEL::imprime_na_tela_avisos_de_possiveis_inviabilidades_fph(Dados& a_dados) {


void LeituraCEPEL::atualiza_volume_util_maximo_com_percentual_volume_util_maximo(Dados& a_dados) {

	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const IdHidreletrica maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

		for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

			if (a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getSizeVetor(AttVetorReservatorio_volume_util_maximo) > 0) {

				for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

					const double volume_minimo = a_dados.getAtributo(idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_minimo, double());
					const double volume_maximo = a_dados.getAtributo(idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_maximo, double());

					if (volume_minimo > volume_maximo)
						throw std::invalid_argument("Volume Minimo = " + getString(volume_minimo) + " maior que Volume Maximo = " + getString(volume_maximo) + " em " + getFullString(idHidreletrica) + " em " + getString(periodo));

					double percentual_volume_util_maximo = 1.0; //É possível que no deck nao seja informado valores para o percentual do volume_espera para uma usina específica, nesse caso, considera-se 1.0 como default

					if(a_dados.getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_percentual_volume_util_maximo) > 0)
						percentual_volume_util_maximo = a_dados.getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_percentual_volume_util_maximo, periodo, double());

					double volume_util_maximo = (volume_maximo - volume_minimo) * percentual_volume_util_maximo;

					if (a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getElementoVetor(AttVetorReservatorio_volume_util_maximo, periodo, double()) > volume_util_maximo)
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_util_maximo, periodo, volume_util_maximo);

				}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

			}//if (a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getSizeVetor(AttVetorReservatorio_volume_util_maximo) > 0) {

		}//for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::atualiza_volume_util_maximo_com_percentual_volume_util_maximo: \n" + std::string(erro.what())); }

}


void LeituraCEPEL::validacoes_DC(Dados& a_dados, const std::string a_diretorio, const std::string a_revisao) {

	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////

		aplicarModificacoesUHE(a_dados);

		if (!hidreletricasPreConfig_instanciadas)
			valida_bacia_sao_francisco(a_dados);

		modifica_lista_jusante_hidreletrica_com_casos_validados_CP(a_dados);
		
			
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Lê volumes metas (se existir relato.rvX e relato2.rvX) e turbinamento_maximo (se existir relato.rvX)

		const bool lido_turbinamento_maximo_from_relato_e_avl_turb_max_DC = leitura_turbinamento_maximo_from_avl_turb_max_DC(a_dados, a_diretorio + "//DadosAdicionais" + "//avl_turb_max.csv");

		if (teste_hidreletrica_volume_meta)
			leitura_volumes_meta_from_dec_oper_usih_DC(a_dados, a_diretorio + "//DadosAdicionais" + "//dec_oper_usih.csv", somente_volume_meta_no_ultimo_estagio);

		leitura_fph_from_avl_cortesfpha_dec_DC(a_dados, a_diretorio + "//DadosAdicionais" + "//avl_cortesfpha_dec." + a_revisao);

		leitura_range_volume_from_eco_fpha_DC(a_dados, a_diretorio + "//DadosAdicionais" + "//eco_fpha_." + a_revisao);

		if (leitura_vazao_evaporada_meta)
			leitura_vazao_evaporada_meta_from_dec_oper_usih_DC(a_dados, a_diretorio + "//DadosAdicionais" + "//dec_oper_usih.csv");

		if (teste_hidreletrica_potencia_disponivel_meta)
			set_hidreletrica_potencia_disponivel_meta_from_dec_oper_usih_DC(a_dados, a_diretorio + "//DadosAdicionais" + "//dec_oper_usih.csv");

		if (teste_termeletrica_potencia_disponivel_meta)
			set_termeletrica_potencia_disponivel_meta(a_dados);

		if (teste_hidreletrica_vazao_turbinada_disponivel_meta)
			set_hidreletrica_vazao_turbinada_disponivel_meta(a_dados);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if (!dadosPreConfig_instanciados) {

			a_dados.setAtributo(AttComumDados_tipo_estudo, TipoEstudo_otimizacao_e_simulacao);

			//////////////////////////////////
			//Dados padrao do CP modo DC
			a_dados.setAtributo(AttComumDados_representar_producao_hidreletrica_com_turbinamento_disponivel, true);
			a_dados.setAtributo(AttComumDados_representar_todos_balancos_hidricos_por_volume, true);
			a_dados.setAtributo(AttComumDados_representar_defluencia_disponivel_em_restricoes_hidraulicas, false);
			a_dados.setAtributo(AttComumDados_representar_turbinamento_disponivel_em_restricoes_hidraulicas, false);
			a_dados.setAtributo(AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas, false);
			a_dados.setAtributo(AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas, false);
			//////////////////////////////////

			a_dados.setAtributo(AttComumDados_ordem_maxima_auto_correlacao_geracao_cenario_hidrologico, 0);
			a_dados.setAtributo(AttComumDados_tipo_coeficiente_auto_correlacao_geracao_cenario_hidrologico, TipoValor_positivo_e_negativo);
			a_dados.setAtributo(AttComumDados_tipo_relaxacao_afluencia_incremental, TipoRelaxacaoAfluenciaIncremental_sem_relaxacao);
			a_dados.setAtributo(AttComumDados_tipo_correlacao_geracao_cenario_hidrologico, TipoCorrelacaoVariaveisAleatorias_sem_correlacao);

			a_dados.setAtributo(AttComumDados_tipo_processamento_paralelo, TipoProcessamentoParalelo_por_abertura);
			a_dados.setAtributo(AttComumDados_mapear_processos_com_um_unico_cenario, true);
			a_dados.setAtributo(AttComumDados_numero_maximo_iteracoes, 20);

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////

		}//if (!dadosPreConfig_instanciados) {
		

		if (a_dados.getSize1Matriz(IdHidreletrica_1, IdReservatorio_1, AttMatrizReservatorio_volume_meta) > 0) {

			if (a_dados.getElementoMatriz(IdHidreletrica_1, IdReservatorio_1, AttMatrizReservatorio_volume_meta, IdCenario_1, horizonte_estudo.getIteradorInicial(), double()) != getdoubleFromChar("max"))
				a_dados.setAtributo(AttComumDados_tipo_estudo, TipoEstudo_simulacao);

		}//if ((a_dados.getSize1Matriz(IdHidreletrica_1, IdReservatorio_1, AttMatrizReservatorio_volume_meta) > 0)) {

		if (a_dados.getSize1Matriz(IdHidreletrica_1, AttMatrizHidreletrica_potencia_disponivel_meta) > 0)
			a_dados.setAtributo(AttComumDados_tipo_estudo, TipoEstudo_simulacao);



		EntradaSaidaDados entradaSaidaDados;

		entradaSaidaDados.setDiretorioEntrada(a_dados.getAtributo(AttComumDados_diretorio_entrada_dados, std::string()));
		entradaSaidaDados.setDiretorioSaida(a_dados.getAtributo(AttComumDados_diretorio_saida_dados, std::string()));

		entradaSaidaDados.setImprimirNaTela(false);

		const std::string diretorio_att_operacionais = entradaSaidaDados.getDiretorioEntrada() + "";

		std::string diretorio_att_premissas = "";
		std::string diretorio_exportacao_pos_estudo = "";

		if (!dadosPreConfig_instanciados)
			a_dados.setAtributo(AttComumDados_diretorio_importacao_pos_estudo, std::string("DadosSaidaMP//Otimizacao//AcoplamentoPreEstudo"));

		if (a_dados.getAtributo(AttComumDados_tipo_estudo, TipoEstudo()) == TipoEstudo_simulacao) {
			diretorio_att_premissas = entradaSaidaDados.getDiretorioEntrada() + "//Simulacao//AtributosPremissasDECOMP//";
			diretorio_exportacao_pos_estudo = entradaSaidaDados.getDiretorioSaida() + "//Simulacao//AcoplamentoPosEstudo";
		}
		else {
			diretorio_att_premissas = entradaSaidaDados.getDiretorioEntrada() + "//Otimizacao//AtributosPremissasDECOMP";
			diretorio_exportacao_pos_estudo = entradaSaidaDados.getDiretorioSaida() + "//Otimizacao//AcoplamentoPosEstudo";
		}

		if (strCompara(a_dados.getAtributo(AttComumDados_diretorio_importacao_pos_estudo, std::string()), "Nenhum"))
			diretorio_exportacao_pos_estudo = "nenhum";

		const bool imprimir_att_operacionais_sem_recarregar = true;

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////


		a_dados.validacaoDadosAttComum();

		a_dados.setAtributo(AttComumDados_diretorio_entrada_dados, entradaSaidaDados.getDiretorioEntrada());
		a_dados.setAtributo(AttComumDados_diretorio_saida_dados, entradaSaidaDados.getDiretorioSaida());

		a_dados.validacao_operacional_Dados(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		/////////////////

		if (a_dados.processoEstocastico_hidrologico.getSizeMatriz(AttMatrizProcessoEstocastico_variavelAleatoria_realizacao) == 0)//Utiliza as realizaçoes do DC
			instanciar_variavelAleatoria_realizacao_from_vazoes_DC(a_dados);

		//////////////////////////////////////////////////////////////
		//Métodos que logo vai ser apagados -> nao necessários na estrutura árvore:

		//Define mapeamento_espaco_amostral para cada Processo (é realizado logo de estabelecer os cenários resolvidos por cada processo)
		//a_dados.processoEstocastico_hidrologico.setMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, define_mapeamento_espaco_amostral_arvore_simetrica_CP(a_dados, a_dados.getAtributo(AttComumDados_menor_cenario_do_processo, IdCenario()), a_dados.getAtributo(AttComumDados_maior_cenario_do_processo, IdCenario())));
		a_dados.processoEstocastico_hidrologico.setMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, define_mapeamento_espaco_amostral_arvore_simetrica_CP(a_dados, IdCenario_1, IdCenario(a_dados.getAtributo(AttComumDados_numero_cenarios, int()))));

		define_afluencia_arvore_de_cenarios_postos_CP(a_dados);

		a_dados.adicionaHidreletricasMontante();
		inicializa_vazao_defluente_CP(a_dados);
		//Define variavel_aleatoria_interna para cada Processo (é realizado logo de estabelecer os cenários resolvidos por cada processo) e das modificaçõesUHE dos postos
		define_variavel_aleatoria_interna_CP(a_dados, a_dados.getAtributo(AttComumDados_menor_cenario_do_processo, IdCenario()), a_dados.getAtributo(AttComumDados_maior_cenario_do_processo, IdCenario()));

		const int numero_cenarios = int(a_dados.processoEstocastico_hidrologico.getIterador1Final(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario()));
		const int numero_cenarios_tendencia = 1;

		a_dados.processoEstocastico_hidrologico.mapearCenariosTendencia(numero_cenarios, numero_cenarios_tendencia);

		////////////////////////

		leitura_vRef_from_CadUsH_csv(a_dados, a_diretorio + "//CadUsH.csv");
		calculaEngolimentoMaximo(a_dados, horizonte_estudo, horizonte_otimizacao_DC.at(horizonte_otimizacao_DC.getIteradorFinal()), lido_turbinamento_maximo_from_relato_e_avl_turb_max_DC);

		//Algumas usinas podem ter instanciado o volume_util_maximo no método determina_restricoes_hidraulicas_especiais() e precisa verificar com o percentual_volume_util_maximo
		atualiza_volume_util_maximo_com_percentual_volume_util_maximo(a_dados); 

		//////////////////////////////////////////////////////////////

		leitura_coeficientes_evaporacao_from_dec_cortes_evap_DC(a_dados, a_diretorio + "//DadosAdicionais" + "//dec_cortes_evap.csv");

		a_dados.validacao_operacional_Submercado(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		a_dados.validacao_operacional_Intercambio(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		a_dados.validacao_operacional_Termeletrica(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		a_dados.validacao_operacional_Hidreletrica(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		a_dados.validacao_operacional_UsinasElevatorias(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		a_dados.validacao_operacional_Intercambio_Hidraulico(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		a_dados.validacao_operacional_BaciaHidrografica(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		a_dados.validacao_operacional_RestricaoEletrica(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		a_dados.validacao_operacional_RestricaoOperativaUHE(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		if (hidreletricasPreConfig_instanciadas)
			a_dados.valida_preconfig_hidraulica(lista_jusante_hidreletrica, lista_jusante_desvio_hidreletrica);

		imprime_na_tela_avisos_de_possiveis_inviabilidades_fph(a_dados);

		a_dados.validacao_operacional_ProcessoEstocasticoHidrologico(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, diretorio_exportacao_pos_estudo, imprimir_att_operacionais_sem_recarregar);


	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::validacoes_DC: \n" + std::string(erro.what())); }

} // void LeituraCEPEL::validacoes_DC(Dados & a_dados){