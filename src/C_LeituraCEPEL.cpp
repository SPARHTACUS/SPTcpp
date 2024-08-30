#include "C_LeituraCEPEL.h"
#include "C_EntradaSaidaDados.h"

const bool instanciarPreConfig = true;

//////////////////////////////////////////////////////////////////////////////
// Leitura cortes NEWAVE
//Nota: A estrutura dos arquivos fcfnwn.rvX e nwlistcf.rel é diferente.
//      Definem-se como default as posições e tamanho no arquivo fcfnwn.rvX. 
//      Caso exista o nwlistcf.rel são atualizados estes parâmetros
//////////////////////////////////////////////////////////////////////////////

//Estrutura arquivo fcfnwn.rvX:
//Posição do começo dos parâmetros 
int pos_RHS = 13;
int pos_idREE = 26;
int pos_Coef_Earm = 33;
int pos_Eafl_lag_1 = 53;
int pos_Eafl_lag_2 = 73;
int pos_Eafl_lag_3 = 93;
int pos_Eafl_lag_4 = 113;
int pos_Eafl_lag_5 = 133;
int pos_Eafl_lag_6 = 153;
int pos_Eafl_lag_7 = 173;
int pos_Eafl_lag_8 = 193;
int pos_Eafl_lag_9 = 213;
int pos_Eafl_lag_10 = 233;
int pos_Eafl_lag_11 = 253;
int pos_Eafl_lag_12 = 273;
int pos_GNL_pat_1_lag_1 = 294;
int pos_GNL_pat_1_lag_2 = 314;
int pos_GNL_pat_2_lag_1 = 334;
int pos_GNL_pat_2_lag_2 = 354;
int pos_GNL_pat_3_lag_1 = 374;
int pos_GNL_pat_3_lag_2 = 394;
int pos_Vminop = 414;

//Tamanho dos parâmetros
int tam_RHS = 12;
int tam_idREE = 6;
int tam_Coef_Earm = 20;
int tam_Eafl_lag_1 = 20;
int tam_Eafl_lag_2 = 20;
int tam_Eafl_lag_3 = 20;
int tam_Eafl_lag_4 = 20;
int tam_Eafl_lag_5 = 20;
int tam_Eafl_lag_6 = 20;
int tam_Eafl_lag_7 = 20;
int tam_Eafl_lag_8 = 20;
int tam_Eafl_lag_9 = 20;
int tam_Eafl_lag_10 = 20;
int tam_Eafl_lag_11 = 20;
int tam_Eafl_lag_12 = 20;
int tam_GNL_pat_1_lag_1 = 20;
int tam_GNL_pat_1_lag_2 = 20;
int tam_GNL_pat_2_lag_1 = 20;
int tam_GNL_pat_2_lag_2 = 20;
int tam_GNL_pat_3_lag_1 = 20;
int tam_GNL_pat_3_lag_2 = 20;
int tam_Vminop = 23;

//////////////////////////////////////////////////////////////////////////////


LeituraCEPEL::LeituraCEPEL() {

	deck_str = "";

	SPT_avisos = "SPT_avisos.txt";


	//////////////////////////////////////////////////////////////////////
	//Se existir arquivo de cortes instancia as hidrelétricas necessárias
	//  para o acoplamento ENA (caso não estiverem instanciadas)
	//////////////////////////////////////////////////////////////////////

	idHidreletricas_sem_producao.push_back(IdHidreletrica_168_ENA_SOBRADINHO); //Usina Posto acoplamento Sobradinho
	idHidreletricas_sem_producao.push_back(IdHidreletrica_172_ENA_ITAPARICA); //Usina Posto acoplamento Itaparica
	idHidreletricas_sem_producao.push_back(IdHidreletrica_176_COMPPAFMOX); //Usina Comp. Moxotó
	idHidreletricas_sem_producao.push_back(IdHidreletrica_178_ENA_XINGO); //Usina Posto acoplamento Xingo

	////////////////////////////////////////////////////////////////////////////////////////////

	codigo_usina_idHidreletricas_sem_producao.push_back(168);   //Usina Posto acoplamento Sobradinho
	codigo_usina_idHidreletricas_sem_producao.push_back(972);   //Usina Posto acoplamento Itaparica
	codigo_usina_idHidreletricas_sem_producao.push_back(176); //Usina Comp. Moxotó
	codigo_usina_idHidreletricas_sem_producao.push_back(978);   //Usina Posto acoplamento Xingo

	////////////////////////////////////////////////////////////////////////////////////////////

	codigo_posto_idHidreletricas_sem_producao.push_back(169); //Usina Posto acoplamento Sobradinho
	codigo_posto_idHidreletricas_sem_producao.push_back(172); //Usina Posto acoplamento Itaparica
	codigo_posto_idHidreletricas_sem_producao.push_back(176); //Usina Comp. Moxotó
	codigo_posto_idHidreletricas_sem_producao.push_back(178); //Usina Posto acoplamento Xingo

	////////////////////////////////////////////////////////////////////////////////////////////
	//Nas regras de cálculo da ENA, 0 significa que não entra no cálculo do corte (estas usinas estão dentro das regras de outras usinas) 

	codigo_posto_acoplamento_ENA_idHidreletricas_sem_producao.push_back(0);   //Usina Posto acoplamento Sobradinho
	codigo_posto_acoplamento_ENA_idHidreletricas_sem_producao.push_back(0);   //Usina Posto acoplamento Itaparica
	codigo_posto_acoplamento_ENA_idHidreletricas_sem_producao.push_back(976); //Usina Comp. Moxotó (Valor atribuido compatível com a lógica do cálculo das ENAs)
	codigo_posto_acoplamento_ENA_idHidreletricas_sem_producao.push_back(0);   //Usina Posto acoplamento Xingo

	////////////////////////////////////////////////////////////////////////////////////////////

	// codigo_ONS_REE = 3 -> NORDESTE

	codigo_ONS_REE_idHidreletricas_sem_producao.push_back(3);   //Usina Posto acoplamento Sobradinho
	codigo_ONS_REE_idHidreletricas_sem_producao.push_back(3);   //Usina Posto acoplamento Itaparica
	codigo_ONS_REE_idHidreletricas_sem_producao.push_back(3);   //Usina Comp. Moxotó (Valor atribuido compatível com a lógica do cálculo das ENAs)
	codigo_ONS_REE_idHidreletricas_sem_producao.push_back(3);   //Usina Posto acoplamento Xingo

	////////////////////////////////////////////////////////////////////////////////////////////


	/*
	lista_modificacaoUHE.reserve(500);
	lista_modificacaoUHE.push_back(std::vector<ModificacaoUHE>());

	lista_modificacaoUTE.reserve(500);
	lista_modificacaoUTE.push_back(std::vector<ModificacaoUTE>());
	*/

} // LeituraCEPEL::LeituraCEPEL() { 

LeituraCEPEL::~LeituraCEPEL() {}

void LeituraCEPEL::leitura_CEPEL(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const std::string a_deck, const std::string a_nick) {

	try {

		Dados dados;

		dados.arranjoResolucao.instanciarProcessos(a_idProcesso, a_maiorIdProcesso);

		EntradaSaidaDados entradaSaidaDados;
		entradaSaidaDados.setDiretorioEntrada(entradaSaidaDados.getDiretorioEntrada() + a_nick);
		entradaSaidaDados.setDiretorioSaida(entradaSaidaDados.getDiretorioSaida() + a_nick);

		dados.setAtributo(AttComumDados_diretorio_entrada_dados, entradaSaidaDados.getDiretorioEntrada());
		dados.setAtributo(AttComumDados_diretorio_saida_dados, entradaSaidaDados.getDiretorioSaida());

		nick_str = a_nick;

		if ((strCompara(a_deck, "NW")) || (strCompara(a_deck, "NEWAVE"))) {
			deck_str = "NW";
			leitura_NEWAVE(dados, "DadosEntradaNEWAVE", "ARQUIVOS.dat");
		}

		else if ((strCompara(a_deck, "DC")) || (strCompara(a_deck, "DECOMP"))) {
			deck_str = "DC";
			leitura_DECOMP(dados, "DadosEntradaDECOMP");

		}

		else if (((strCompara(a_deck, "DS")) || (strCompara(a_deck, "DESSEM"))) && (a_idProcesso == IdProcesso_mestre)) {
			deck_str = "DS";
			std::string nomePasta;
			//nomePasta = "DS_CCEE_022020_SEMREDE_RV2D15";
			//nomePasta = "DS_CCEE_012020_SEMREDE_RV3D23";
			//nomePasta = "DS_CCEE_052020_SEMREDE_RV0D28";
			nomePasta = "DadosEntradaDESSEM";
			leitura_DESSEM(dados, nomePasta + "//dessem.arq", nomePasta);
		}

		else
			throw std::invalid_argument("Deck desconhecido " + a_deck + ", valor deve ser NW, DC ou DS.");

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_CEPEL(" + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + "," + a_deck + "," + a_nick + "): \n" + std::string(erro.what())); }
}


IdBaciaHidrografica LeituraCEPEL::atribui_bacia_hidrografica(Dados& a_dados, const int a_codigo_ons_uhe) {

	try {

		const std::vector<int> paranaiba{ 24, 27, 28, 25, 33, 31, 30, 20, 32, 262, 26, 241, 203, 29, 311, 312, 315, 21, 290 };
		const std::vector<int> do_grande{ 1, 2, 4, 10, 14, 18, 15, 16, 6, 7, 8, 12, 17, 11, 9 };
		const std::vector<int> tiete{ 37, 40, 42, 38, 39, 43, 107, 108, 109, 117, 118, 119 };
		const std::vector<int> ilha_solteira{ 45, 34, 153, 46 };
		const std::vector<int> paranapanema{ 62, 63, 52, 51, 47, 49, 61, 50, 57, 48, 249 };
		const std::vector<int> paraguai{ 278, 281, 304, 305, 195 };
		const std::vector<int> doce_piracicaba{ 134, 143, 217, 193, 141, 135, 139, 144, 192 };
		const std::vector<int> parana{ 66 };
		const std::vector<int> paraiba_do_sul{ 120, 121, 123, 129, 122, 124, 130, 131, 133, 125, 180, 181, 182, 183, 184, 127, 126 };
		const std::vector<int> iguacu{ 74, 76, 82, 83, 73, 71, 72, 77, 78 };
		const std::vector<int> chapeco_uruguai{ 89, 93, 91, 92, 86, 90, 103, 95, 94, 101, 102 };
		const std::vector<int> jacui{ 110, 111, 112, 113, 114, 97, 98, 99 };
		const std::vector<int> outras_S{ 115, 215 };
		const std::vector<int> sao_francisco{ 156, 162, 169, 172, 173, 174, 175, 178, 155 };
		const std::vector<int> parnaiba{ 190 };
		const std::vector<int> tocantins{ 251, 257, 252, 253, 267, 275, 261 };
		const std::vector<int> outras_N{ 272, 279, 280, 204, 310, 276, 277, 286, 284 };
		const std::vector<int> madeira{ 287, 196, 285 };
		const std::vector<int> xingu{ 288, 314 };
		const std::vector<int> teles_pires{ 227, 228, 229, 230 };
		const std::vector<int> atlantico_leste{ 148, 189, 283, 154 };

		std::string nome_bacia = "Nenhum";
		if (encontrar(paranaiba, a_codigo_ons_uhe) > -1)
			nome_bacia = "paranaiba";
		else if (encontrar(do_grande, a_codigo_ons_uhe) > -1)
			nome_bacia = "do_grande";
		else if (encontrar(tiete, a_codigo_ons_uhe) > -1)
			nome_bacia = "tiete";
		else if (encontrar(ilha_solteira, a_codigo_ons_uhe) > -1)
			nome_bacia = "ilha_solteira";
		else if (encontrar(paranapanema, a_codigo_ons_uhe) > -1)
			nome_bacia = "paranapanema";
		else if (encontrar(paraguai, a_codigo_ons_uhe) > -1)
			nome_bacia = "paraguai";
		else if (encontrar(doce_piracicaba, a_codigo_ons_uhe) > -1)
			nome_bacia = "doce_piracicaba";
		else if (encontrar(parana, a_codigo_ons_uhe) > -1)
			nome_bacia = "parana";
		else if (encontrar(paraiba_do_sul, a_codigo_ons_uhe) > -1)
			nome_bacia = "paraiba_do_sul";
		else if (encontrar(iguacu, a_codigo_ons_uhe) > -1)
			nome_bacia = "iguacu";
		else if (encontrar(chapeco_uruguai, a_codigo_ons_uhe) > -1)
			nome_bacia = "chapeco_uruguai";
		else if (encontrar(jacui, a_codigo_ons_uhe) > -1)
			nome_bacia = "jacui";
		else if (encontrar(outras_S, a_codigo_ons_uhe) > -1)
			nome_bacia = "outras_S";
		else if (encontrar(sao_francisco, a_codigo_ons_uhe) > -1)
			nome_bacia = "sao_francisco";
		else if (encontrar(parnaiba, a_codigo_ons_uhe) > -1)
			nome_bacia = "parnaiba";
		else if (encontrar(tocantins, a_codigo_ons_uhe) > -1)
			nome_bacia = "tocantins";
		else if (encontrar(outras_N, a_codigo_ons_uhe) > -1)
			nome_bacia = "outras_N";
		else if (encontrar(madeira, a_codigo_ons_uhe) > -1)
			nome_bacia = "madeira";
		else if (encontrar(xingu, a_codigo_ons_uhe) > -1)
			nome_bacia = "xingu";
		else if (encontrar(teles_pires, a_codigo_ons_uhe) > -1)
			nome_bacia = "teles_pires";
		else if (encontrar(atlantico_leste, a_codigo_ons_uhe) > -1)
			nome_bacia = "atlantico_leste";

		const std::vector<IdBaciaHidrografica> lista_idBaciaHidrografica = a_dados.vetorBaciaHidrografica.getIdObjetos(AttComumBaciaHidrografica_nome, nome_bacia);

		if (lista_idBaciaHidrografica.size() == 0) {
			BaciaHidrografica baciaHidrografica;
			const IdBaciaHidrografica idBaciaHidrografica = IdBaciaHidrografica(a_dados.getMaiorId(IdBaciaHidrografica()) + 1);
			baciaHidrografica.setAtributo(AttComumBaciaHidrografica_idBaciaHidrografica, idBaciaHidrografica);
			baciaHidrografica.setAtributo(AttComumBaciaHidrografica_nome, nome_bacia);
			a_dados.vetorBaciaHidrografica.add(baciaHidrografica);
			return idBaciaHidrografica;
		}

		if (lista_idBaciaHidrografica.size() > 1)
			throw std::invalid_argument("Multiplas bacias instanciadas com o nome " + nome_bacia + ".");

		else if (lista_idBaciaHidrografica.size() == 1)
			return lista_idBaciaHidrografica.at(0);

		return IdBaciaHidrografica_Nenhum;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::atribui_bacia_hidrografica(a_dados," + getFullString(a_codigo_ons_uhe) + "): \n" + std::string(erro.what())); }

} // IdBaciaHidrografica LeituraCEPEL::atribui_bacia_hidrografica(Dados& a_dados, const int codigo_ons_uhe)


void LeituraCEPEL::calculaEngolimentoMaximo(Dados& a_dados, const SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo, const Periodo a_periodo_final, const bool a_lido_turbinamento_maximo_from_relato_e_avl_turb_max_DC, const IdCenario a_menor_cenario, const IdCenario a_maior_cenario) {
	IdHidreletrica idHidreletrica;
	try {
		
		SmartEnupla<Periodo, bool>	horizonte_processo_estocastico;

		if (a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.at(IdVariavelAleatoria_1).vetorVariavelAleatoriaInterna.at(IdVariavelAleatoriaInterna_1).getSizeMatriz(AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral) > 0) {

			horizonte_processo_estocastico = SmartEnupla<Periodo, bool>(a_dados.processoEstocastico_hidrologico.getMatriz(IdVariavelAleatoria_1, IdVariavelAleatoriaInterna_1, AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral, Periodo(), IdCenario(), double()), true);


		}//	if (a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.at(IdVariavelAleatoria_1).vetorVariavelAleatoriaInterna.at(IdVariavelAleatoriaInterna_1).getSizeMatriz(AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral) > 0) {


		const IdHidreletrica idHidreletricaIni = a_dados.getMenorId(IdHidreletrica());
		const IdHidreletrica idHidreletricaOut = a_dados.getIdOut(IdHidreletrica());

		for (idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			if (a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) != TipoDetalhamentoProducaoHidreletrica_sem_producao) {
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Calcula a defluencia_maxima do polinomio_jusante para obter h_jus > 0 nas usinas com maiorPolinomiojusante = 1
				//Usinas reportadas no arquivo polijus.dat tem a informaçao da defluência_maxima

				if (a_dados.getMaiorId(idHidreletrica, IdPolinomioJusante()) == IdPolinomioJusante_1 && a_dados.getAtributo(idHidreletrica, IdPolinomioJusante_1, AttComumPolinomioJusante_defluencia_maxima, double()) == getdoubleFromChar("max")) {
					const double defluencia_maxima = calculaRaizPolinomioJusante(a_dados, idHidreletrica, a_horizonte_estudo.getIteradorInicial());
					a_dados.vetorHidreletrica.at(idHidreletrica).vetorPolinomioJusante.at(IdPolinomioJusante_1).setAtributo(AttComumPolinomioJusante_defluencia_maxima, defluencia_maxima);
				}//if (a_dados.getMaiorId(idHidreletrica, IdPolinomioJusante()) == IdPolinomioJusante_1 && a_dados.getAtributo(idHidreletrica, IdPolinomioJusante_1, AttComumPolinomioJusante_defluencia_maxima, double()) == getdoubleFromChar("max")) {

				/////////////////////////////////////////////////

				const IdConjuntoHidraulico maiorIdConjunto = a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico());
				if (maiorIdConjunto != IdConjuntoHidraulico_Nenhum) {
					TipoTurbina tipo_turbina = a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_tipo_turbina, TipoTurbina());

					if (tipo_turbina != TipoTurbina_sem_turbina) {

						double fator = 0.5;
						if (tipo_turbina == TipoTurbina_kaplan) { fator = 0.2; }

						double Q_max_usina = 0;
						double P_max_usina = 0;
						for (IdConjuntoHidraulico idConjunto = IdConjuntoHidraulico_1; idConjunto <= maiorIdConjunto; idConjunto++) {
							const IdUnidadeUHE maiorIdunidade = a_dados.getMaiorId(idHidreletrica, idConjunto, IdUnidadeUHE());
							if (maiorIdunidade != IdUnidadeUHE_Nenhum) {
								Q_max_usina += double(int(maiorIdunidade)) * a_dados.getAtributo(idHidreletrica, idConjunto, IdUnidadeUHE_1, AttComumUnidadeUHE_vazao_turbinada_maxima, double());
								P_max_usina += double(int(maiorIdunidade)) * a_dados.getAtributo(idHidreletrica, idConjunto, IdUnidadeUHE_1, AttComumUnidadeUHE_potencia_maxima, double());
							}
						}

						const TipoPerdaHidraulica tipo_perda_hidraulica = a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_tipo_de_perda_hidraulica, TipoPerdaHidraulica());
						const IdPolinomioJusante maiorIdPolinomio = a_dados.getMaiorId(idHidreletrica, IdPolinomioJusante());
						const double perda_hidraulica = a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_perda_hidraulica, double());
						const double h_efetiva = a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_queda_referencia, double());
						const double percentual_volume_inicial = a_dados.getAtributo(idHidreletrica, IdReservatorio_1, AttComumReservatorio_percentual_volume_util_inicial, double());
						const double canal_fuga_medio = a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_canal_fuga_medio, double());
						const double potencia_maxima = P_max_usina;
						const double q_efetiva = Q_max_usina;

						//////////////////////////////
						//Normalmente o fator_de_producao é único para todas as unidades da usina, porém, no DESSEM pode ser modificado o valor de uma unidade
						//por isso é calculada a média para generalizar

						double fator_de_producao = 0.0;
						int numero_unidades = 0;

						for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= maiorIdConjunto; idConjuntoHidraulico++) {
							for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {
								fator_de_producao += a_dados.getAtributo(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttComumUnidadeUHE_fator_de_producao, double());
								numero_unidades++;
							}//for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {
						}//for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= maiorIdConjunto; idConjuntoHidraulico++) {

						fator_de_producao /= numero_unidades;

						//////////////////////////////

						if (a_dados.getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_maxima) == 0)
							a_dados.vetorHidreletrica.at(idHidreletrica).setVetor(AttVetorHidreletrica_vazao_turbinada_maxima, SmartEnupla<Periodo, double>(a_horizonte_estudo, q_efetiva));

						if (a_dados.getSizeVetor(idHidreletrica, AttVetorHidreletrica_cota_montante_usina_jusante) == 0)
							a_dados.vetorHidreletrica.at(idHidreletrica).setVetor(AttVetorHidreletrica_cota_montante_usina_jusante, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0));

						Periodo periodo_inicial = a_horizonte_estudo.getIteradorInicial();
						const Periodo periodo_final = a_horizonte_estudo.getIteradorFinal();

						if (a_lido_turbinamento_maximo_from_relato_e_avl_turb_max_DC)//Se sao carregados os qMax do avl_turb_max.csv somente atualiza o último periodo do DC
							periodo_inicial = a_horizonte_estudo.getIteradorFinal();

						for (Periodo periodo = periodo_inicial; periodo <= periodo_final; a_horizonte_estudo.incrementarIterador(periodo)) {
							double canal_fuga_medio = a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_canal_fuga_medio, double());

							//CALCULO DA COTA DE MONTANTE DA USINA DE JUSANTE 
							double h_mont_jusante;
							IdHidreletrica idHidreletrica_jusante = a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_jusante, IdHidreletrica());
							if (idHidreletrica_jusante != IdHidreletrica_Nenhum) {
								const double percentual_volume_inicial_jusante = a_dados.getAtributo(idHidreletrica_jusante, IdReservatorio_1, AttComumReservatorio_percentual_volume_util_inicial, double());

								double volume_minimo = a_dados.getAtributo(idHidreletrica_jusante, IdReservatorio_1, AttComumReservatorio_volume_minimo, double());
								if (a_dados.getSizeVetor(idHidreletrica_jusante, IdReservatorio_1, AttVetorReservatorio_volume_minimo) > 0)
									volume_minimo = a_dados.getElementoVetor(idHidreletrica_jusante, IdReservatorio_1, AttVetorReservatorio_volume_minimo, periodo, double());

								double volume_maximo = a_dados.getAtributo(idHidreletrica_jusante, IdReservatorio_1, AttComumReservatorio_volume_maximo, double());
								if (a_dados.getSizeVetor(idHidreletrica_jusante, IdReservatorio_1, AttVetorReservatorio_volume_maximo) > 0)
									volume_maximo = a_dados.getElementoVetor(idHidreletrica_jusante, IdReservatorio_1, AttVetorReservatorio_volume_maximo, periodo, double());

								double percentual_volume_util_maximo = a_dados.getAtributo(idHidreletrica_jusante, IdReservatorio_1, AttComumReservatorio_percentual_volume_util_maximo, double());
								if (a_dados.getSizeVetor(idHidreletrica_jusante, IdReservatorio_1, AttVetorReservatorio_percentual_volume_util_maximo) > 0)
									percentual_volume_util_maximo = a_dados.getElementoVetor(idHidreletrica_jusante, IdReservatorio_1, AttVetorReservatorio_percentual_volume_util_maximo, periodo, double());

								if (a_dados.getElementoVetor(idHidreletrica_jusante, AttVetorHidreletrica_regularizacao, periodo, int()) != 1)
									volume_minimo = volume_maximo;

								double volume = volume_minimo + percentual_volume_inicial_jusante * (volume_maximo - volume_minimo) * percentual_volume_util_maximo;
								h_mont_jusante = a_dados.vetorHidreletrica.at(idHidreletrica_jusante).vetorReservatorio.at(IdReservatorio_1).getCota(periodo, volume);

							}
							else {
								for (IdPolinomioJusante idPolinomio = IdPolinomioJusante_1; idPolinomio <= maiorIdPolinomio; idPolinomio++)
									if (a_dados.getSizeVetor(idHidreletrica, idPolinomio, AttVetorPolinomioJusante_altura_jusante_ref) > 0)
										h_mont_jusante = a_dados.getElementoVetor(idHidreletrica, idPolinomio, AttVetorPolinomioJusante_altura_jusante_ref, periodo, double());
							}//else {

							a_dados.vetorHidreletrica.at(idHidreletrica).setElemento(AttVetorHidreletrica_cota_montante_usina_jusante, periodo, h_mont_jusante);

							if (a_dados.getSizeVetor(idHidreletrica, AttVetorHidreletrica_canal_fuga_medio) > 0)
								canal_fuga_medio = a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_canal_fuga_medio, periodo, double());

							////////////////////////////

							double volume_minimo = a_dados.getAtributo(idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_minimo, double());
							if (a_dados.getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo) > 0)
								volume_minimo = a_dados.getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo, periodo, double());

							double volume_maximo = a_dados.getAtributo(idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_maximo, double());
							if (a_dados.getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_maximo) > 0)
								volume_maximo = a_dados.getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_maximo, periodo, double());

							double volume = volume_minimo + percentual_volume_inicial * (volume_maximo - volume_minimo);

							//Se a usina for fio d'àgua utiliza o volume_referencia do Hidr.dat (pode ser maior do que o volume_máximo: verificado em consulta ao Cepel)
							if (a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_regularizacao, periodo, int()) != 1) {
								volume_minimo = volume_maximo;
								volume = a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_volume_referencia, double());
							}//if (!bool(a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_regularizacao, periodo, bool()))) {

							const double h_mont = a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getCota(periodo, volume);

							const double cota_referencia = a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getCotaMedia(periodo, volume_minimo, volume_maximo);
							const double produtibilidade_media = a_dados.vetorHidreletrica.at(idHidreletrica).calcularProdutibilidade(tipo_perda_hidraulica, perda_hidraulica, fator_de_producao, cota_referencia - canal_fuga_medio);

							const double vazao_maxima_gerador = potencia_maxima / produtibilidade_media;

							double  q_max_inicial = 0;
							double q_max = vazao_maxima_gerador;

							for (int i = 0; i < 15; i++) {
								q_max_inicial = q_max;

								double h_jus = a_dados.vetorHidreletrica.at(idHidreletrica).getCotaJusante(q_max_inicial, periodo);
								double cota = h_mont - h_jus;

								if (tipo_perda_hidraulica == TipoPerdaHidraulica_percentual)
									cota *= (1.0 - perda_hidraulica);
								else if (tipo_perda_hidraulica == TipoPerdaHidraulica_metro)
									cota -= perda_hidraulica;

								q_max = std::pow(double(cota / h_efetiva), fator) * q_efetiva;
								if (abs(q_max_inicial - q_max) < 1)
									break;
							}//for (int i = 0; i < 15; i++) {

							////////////////////////////
							//Identifica o número de aberturas do periodo

							const SmartEnupla <IdEstagio, Periodo> horizonte_otimizacao = a_dados.getVetor(AttVetorDados_horizonte_otimizacao, IdEstagio(), Periodo());

							IdEstagio idEstagio_periodo = IdEstagio_Nenhum;

							for (IdEstagio idEstagio = IdEstagio_1; idEstagio <= horizonte_otimizacao.getIteradorFinal(); idEstagio++) {

								if (periodo.sobreposicao(horizonte_otimizacao.getElemento(idEstagio)) == 1.0) {
									idEstagio_periodo = idEstagio;
									break;
								}//if (periodo.sobreposicao(horizonte_otimizacao.getElemento(idEstagio)) == 1.0) {

							}//for (IdEstagio idEstagio = IdEstagio_1; idEstagio <= horizonte_otimizacao.getIteradorFinal(); idEstagio++) {

							int numero_aberturas = 1;

							for (IdEstagio idEstagio = IdEstagio_1; idEstagio <= idEstagio_periodo; idEstagio++)
								numero_aberturas *= a_dados.getElementoVetor(AttVetorDados_numero_aberturas, idEstagio, int());

							bool cota_negativa = false;

							if (periodo.sobreposicao(a_periodo_final) == 0 && numero_aberturas == 1) {
								//Realiza o cálculo da vazao_defluencia_minima = vazoes_montante (até a próxima usina com regularizaçao) - vazao_armazenavel
								const double vazao_defluencia_minima = calculaDefluenciaMinima_para_EngolimentoMaximo(a_dados, idHidreletrica, periodo, idEstagio_periodo, horizonte_processo_estocastico, a_menor_cenario, a_maior_cenario);
								const double defluencia_maxima_polinomio_jusante = a_dados.getAtributo(idHidreletrica, a_dados.getMaiorId(idHidreletrica, IdPolinomioJusante()), AttComumPolinomioJusante_defluencia_maxima, double());

								if (q_max < vazao_defluencia_minima && vazao_defluencia_minima < defluencia_maxima_polinomio_jusante) {

									//Recalcula o q_Max com h_jus(vazao_defluencia_minima)
									double h_jus = a_dados.vetorHidreletrica.at(idHidreletrica).getCotaJusante(vazao_defluencia_minima, periodo);
									double cota = h_mont - h_jus;

									if (tipo_perda_hidraulica == TipoPerdaHidraulica_percentual)
										cota *= (1.0 - perda_hidraulica);
									else if (tipo_perda_hidraulica == TipoPerdaHidraulica_metro)
										cota -= perda_hidraulica;

									if (cota < 0)
										cota_negativa = true;

									q_max = std::pow(double(cota / h_efetiva), fator) * q_efetiva;

								}//if (q_max < vazao_defluencia_minima && vazao_defluencia_minima < defluencia_maxima_polinomio_jusante) {

							}//if (periodo.sobreposicao(a_periodo_final) == 0) {

							if (q_max > vazao_maxima_gerador || cota_negativa) { q_max = vazao_maxima_gerador; }

							a_dados.vetorHidreletrica.at(idHidreletrica).setElemento(AttVetorHidreletrica_vazao_turbinada_maxima, periodo, q_max);

						}//for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo)) {

					}//if (tipo_turbina != TipoTurbina_sem_turbina) {

				}//if (maiorIdConjunto != IdConjuntoHidraulico_Nenhum){
			}
		}//for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::calculaEngolimentoMaximo(" + getFullString(idHidreletrica) + "): \n" + std::string(erro.what())); }
}

double LeituraCEPEL::calculaDefluenciaMinima_para_EngolimentoMaximo(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const Periodo a_periodo, const IdEstagio a_idEstagio, const SmartEnupla<Periodo, bool> a_horizonte_processo_estocastico, const IdCenario a_menor_cenario, const IdCenario a_maior_cenario)
{
	try {

		//1. As idHidreletricas_calculo_defluencia_minima = usina + usinas sem regularizaçao a montante

		std::vector<IdHidreletrica> idHidreletricas_calculo_defluencia_minima(1, a_idHidreletrica);
		std::vector<IdHidreletrica> idHidreletricas_montante;

		int numMontante = a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_montante);

		for (int m = 1; m <= numMontante; m++)
			idHidreletricas_montante.push_back(a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_montante, m, IdHidreletrica()));

		while (true) {

			//Teste se nao existem mais usinas a montante
			if (int(idHidreletricas_montante.size()) == 0)
				break;

			//Teste usina com regularizacao a montante encontrada
			if (a_dados.vetorHidreletrica.at(idHidreletricas_montante.at(0)).getElementoVetor(AttVetorHidreletrica_regularizacao, a_periodo, int()) != 1) {

				idHidreletricas_calculo_defluencia_minima.push_back(idHidreletricas_montante.at(0));

				//Inclui usinas a montante da usina idHidreletricas_montante.at(0) 

				numMontante = a_dados.getSizeVetor(idHidreletricas_montante.at(0), AttVetorHidreletrica_montante);

				for (int m = 1; m <= numMontante; m++)
					idHidreletricas_montante.push_back(a_dados.getElementoVetor(idHidreletricas_montante.at(0), AttVetorHidreletrica_montante, m, IdHidreletrica()));

			}//if (!a_dados.vetorHidreletrica.at(idHidreletricas_montante.at(0)).getElementoVetor(AttVetorHidreletrica_regularizacao, a_periodo, bool())) {

			//Elimina do vetor a usina já validada
			idHidreletricas_montante.erase(idHidreletricas_montante.begin());

		}//while (true) {

		//2. Cálculo das vazoes a montante

		double vazao_defluencia_minima = 0;

		for (Periodo periodo_processo_estocastico = a_horizonte_processo_estocastico.getIteradorInicial(); periodo_processo_estocastico <= a_horizonte_processo_estocastico.getIteradorFinal(); a_horizonte_processo_estocastico.incrementarIterador(periodo_processo_estocastico)) {

			if (a_periodo.sobreposicao(periodo_processo_estocastico) == 1) {

				for (int pos = 0; pos < int(idHidreletricas_calculo_defluencia_minima.size()); pos++) {

					const IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria(lista_hidreletrica_IdVariavelAleatoria.at(idHidreletricas_calculo_defluencia_minima.at(pos)));

					//validar
					vazao_defluencia_minima += a_dados.processoEstocastico_hidrologico.getElementoMatriz(idVariavelAleatoria, IdVariavelAleatoriaInterna_1, AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral, periodo_processo_estocastico, a_menor_cenario, double());

					/*
					if (a_idEstagio <= a_dados.getAtributo(AttComumDados_estagio_acoplamento_pre_estudo, IdEstagio()))
						vazao_defluencia_minima += a_dados.processoEstocastico_hidrologico.getElementoMatriz(idVariavelAleatoria, IdVariavelAleatoriaInterna_1, AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral, periodo_processo_estocastico, a_menor_cenario, double());
					else {
						
						const IdRealizacao idRealizacao = a_dados.processoEstocastico_hidrologico.getElementoMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_menor_cenario, periodo_processo_estocastico, IdRealizacao());
						vazao_defluencia_minima += a_dados.processoEstocastico_hidrologico.getElementoMatriz(idVariavelAleatoria, AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodo_processo_estocastico, idRealizacao, double());

					}//else {
					*/
				}//for (int pos = 0; pos < int(idHidreletricas_calculo_defluencia_minima.size()); pos++) {

				break;

			}//if (a_periodo.sobreposicao(periodo_processo_estocastico) == 1) {

		}//for (Periodo periodo_processo_estocastico = a_horizonte_processo_estocastico.getIteradorInicial(); periodo_processo_estocastico <= a_horizonte_processo_estocastico.getIteradorFinal(); a_horizonte_processo_estocastico.incrementarIterador(periodo_processo_estocastico)) {

		//3. Desconta a vazao que a usina consegue armazenar no periodo

		if (a_dados.vetorHidreletrica.at(a_idHidreletrica).getElementoVetor(AttVetorHidreletrica_regularizacao, a_periodo, int()) == 1) {

			const double percentual_volume_inicial = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_percentual_volume_util_inicial, double());
			const double conversor_vazao_volume = a_dados.getElementoVetor(AttVetorDados_conversor_vazao_volume, a_periodo, double());

			double volume_minimo = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_minimo, double());
			if (a_dados.getSizeVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo) > 0)
				volume_minimo = a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo, a_periodo, double());

			double volume_maximo = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_maximo, double());
			if (a_dados.getSizeVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_maximo) > 0)
				volume_maximo = a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_maximo, a_periodo, double());

			//Ver se precisa o volume_espera!
			const double volume_util = volume_maximo - volume_minimo;

			vazao_defluencia_minima -= (1 - percentual_volume_inicial) * volume_util / conversor_vazao_volume;

		}//if (a_dados.vetorHidreletrica.at(a_idHidreletrica).getElementoVetor(AttVetorHidreletrica_regularizacao, a_periodo, bool())){

		return vazao_defluencia_minima;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::calculaDefluenciaMinima_para_EngolimentoMaximo(" + getFullString(a_idHidreletrica) + ", " + getFullString(a_periodo) + "): \n" + std::string(erro.what())); }

}


double LeituraCEPEL::calculaRaizPolinomioJusante(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const Periodo a_periodo)
{
	try {

		const double tol = 1e-5;

		double defluencia_raiz = 0.0;

		double defluencia_minima = 0.0;
		double defluencia_maxima = 200000; //Valor maior ao máximo registrado para todas as usinas

		double h_jus_defluencia_maxima = a_dados.vetorHidreletrica.at(a_idHidreletrica).getCotaJusante_permite_valoresNegativos(defluencia_maxima, a_periodo);
		double h_jus_defluencia_minima = a_dados.vetorHidreletrica.at(a_idHidreletrica).getCotaJusante_permite_valoresNegativos(defluencia_minima, a_periodo);

		double defluencia_raiz_anterior = defluencia_raiz;

		if (h_jus_defluencia_minima * h_jus_defluencia_maxima < 0.0) {

			while (true) {

				//Aplica método de bissecçao para encontrar a raiz do polinômio
				defluencia_raiz = 0.5 * (defluencia_maxima + defluencia_minima);

				double h_jus_defluencia_raiz = a_dados.vetorHidreletrica.at(a_idHidreletrica).getCotaJusante_permite_valoresNegativos(defluencia_raiz, a_periodo);

				if (abs(defluencia_raiz - defluencia_raiz_anterior) <= tol || abs(h_jus_defluencia_raiz) <= tol) {
					defluencia_maxima = defluencia_raiz;
					break;
				}//if (abs(defluencia_raiz - defluencia_raiz_anterior) <= tol || abs(h_jus_defluencia_raiz) <= tol) {

				defluencia_raiz_anterior = defluencia_raiz;

				h_jus_defluencia_minima = a_dados.vetorHidreletrica.at(a_idHidreletrica).getCotaJusante_permite_valoresNegativos(defluencia_minima, a_periodo);

				if (h_jus_defluencia_raiz * h_jus_defluencia_minima < 0)
					defluencia_maxima = defluencia_raiz;
				else
					defluencia_minima = defluencia_raiz;

			}//while (true) {

		}//if (h_jus_defluencia_minima* h_jus_defluencia_maxima < 0.0) {

		return defluencia_maxima;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::calculaRaizPolinomioJusante(" + getFullString(a_idHidreletrica) + "): \n" + std::string(erro.what())); }

}


void LeituraCEPEL::instancia_termeletricas_preConfig(Dados& a_dados, const std::string a_diretorio) {

	try {

		EntradaSaidaDados entradaSaidaDados;

		entradaSaidaDados.setDiretorioEntrada(a_diretorio);
		if (!entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("TERMELETRICA_AttComumOperacional.csv", a_dados, TipoAcessoInstancia_m1))
			return;
		const IdTermeletrica	idTermeletricaIni = a_dados.getMenorId(IdTermeletrica());
		const IdTermeletrica	idTermeletricaOut = a_dados.getIdOut(IdTermeletrica());
		for (IdTermeletrica idTermeletrica = idTermeletricaIni; idTermeletrica < idTermeletricaOut; a_dados.vetorTermeletrica.incr(idTermeletrica)) {
			a_dados.vetorTermeletrica.at(idTermeletrica).setAtributo(AttComumTermeletrica_considerar_usina, false);
			a_dados.vetorTermeletrica.at(idTermeletrica).setAtributo(AttComumTermeletrica_representacao_discreta_producao, false);
			a_dados.vetorTermeletrica.at(idTermeletrica).setAtributo(AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica_sem_producao);

			termeletricasPreConfig_instanciadas = true;
			lista_codigo_ONS_termeletrica.setElemento(idTermeletrica, a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_codigo_usina, int()));
			lista_classe_ONS_termeletrica.setElemento(idTermeletrica, 0);
			lista_termeletrica_com_expansao.setElemento(idTermeletrica, false);


		}

	}
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::instancia_termeletricas_preConfig(): \n" + std::string(erro.what())); }

}

void LeituraCEPEL::instancia_hidreletricas_preConfig(Dados& a_dados, const std::string a_diretorio) {

	try {

		EntradaSaidaDados entradaSaidaDados;

		entradaSaidaDados.setDiretorioEntrada(a_diretorio);
		entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("HIDRELETRICA_AttComumOperacional.csv", a_dados, TipoAcessoInstancia_m1);

		//Set listas necessárias na estrutura da leitura

		const IdHidreletrica  idHidreletricaEnd = a_dados.getMaiorId(IdHidreletrica());

		if (idHidreletricaEnd > IdHidreletrica_Nenhum) {

			if (true) {

				a_dados.volume_inicial_carregado_from_operacional = entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("HIDRELETRICA_RESERVATORIO_AttComumOperacional.csv", a_dados, TipoAcessoInstancia_m2);
				a_dados.volume_inicial_carregado_from_premissa    = entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("HIDRELETRICA_RESERVATORIO_AttComumPremissa.csv", a_dados, TipoAcessoInstancia_m2);

				if (a_dados.getAtributo(AttComumDados_estagio_inicial, IdEstagio()) == IdEstagio_1) {
					if (a_dados.volume_inicial_carregado_from_operacional && a_dados.volume_inicial_carregado_from_premissa)
						throw std::invalid_argument("Apenas um dos arquivos HIDRELETRICA_RESERVATORIO_AttComumOperacional.csv, HIDRELETRICA_RESERVATORIO_AttComumPremissa.csv deve ser carregado.");
				}
				else {
					if (a_dados.volume_inicial_carregado_from_operacional || a_dados.volume_inicial_carregado_from_premissa)
						throw std::invalid_argument("Nenhum dos arquivos HIDRELETRICA_RESERVATORIO_AttComumOperacional.csv, HIDRELETRICA_RESERVATORIO_AttComumPremissa.csv deve ser carregado quando " + getFullString(AttComumDados_estagio_inicial) + " for maior que " + getFullString(IdEstagio_1));
				}

			} // if (true) {

			hidreletricasPreConfig_instanciadas = true;

			a_dados.adicionaHidreletricasMontante();

			entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("BACIA_HIDROGRAFICA_AttComumOperacional.csv", a_dados, TipoAcessoInstancia_m1);

			entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("HIDRELETRICA_AttVetorOperacional_PorPeriodo.csv", a_dados, TipoAcessoInstancia_m1);

			if (true) {
				const bool leitura_att_vetor_defluencia = entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("HIDRELETRICA_DEFLUENCIA_AttVetorOperacional_PorPeriodo.csv", a_dados, TipoAcessoInstancia_m2);
				if ((a_dados.getAtributo(AttComumDados_estagio_inicial, IdEstagio()) > IdEstagio_1) && (leitura_att_vetor_defluencia))
					throw std::invalid_argument("O arquivo HIDRELETRICA_DEFLUENCIA_AttVetorOperacional_PorPeriodo.csv nao deve ser lido caso " + getFullString(AttComumDados_estagio_inicial) + " seja maior que " + getFullString(IdEstagio_1) + ".");
			} // if (true) {

			bool tipo_detalhamento_producao_por_usina = false;
			bool tipo_detalhamento_producao_por_conjunto = false;
			bool tipo_detalhamento_producao_por_unidade = false;

			const IdHidreletrica idHidreletricaIni = a_dados.getMenorId(IdHidreletrica());
			const IdHidreletrica idHidreletricaOut = a_dados.getIdOut(IdHidreletrica());

			for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

				lista_hidreletrica_pre_configuracao.setElemento(idHidreletrica, true);

				lista_codigo_ONS_REE.setElemento(idHidreletrica, 0);
				lista_codigo_ONS_hidreletrica.setElemento(idHidreletrica, a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_codigo_usina, int()));
				lista_hidreletrica_com_expansao.setElemento(idHidreletrica, false);
				lista_hidreletrica_com_modificacao.setElemento(idHidreletrica, false);
				lista_hidreletrica_sem_capacidade.setElemento(idHidreletrica, false);
				lista_hidreletrica_maiorIdConjuntoHidraulico.setElemento(idHidreletrica, IdConjuntoHidraulico_Nenhum); //Logo lendo o HIDR.DAT este parâmetro é atualizado
				lista_IdSubmercado_hidreletrica.setElemento(idHidreletrica, IdSubmercado_Nenhum); //Logo no HIDR.DAT esta informação é atualizada

				if (a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_bacia, IdBaciaHidrografica()) == IdBaciaHidrografica_Nenhum) {
					const IdBaciaHidrografica idBaciaHidrografica = atribui_bacia_hidrografica(a_dados, a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_codigo_usina, int()));

					if (idBaciaHidrografica == IdBaciaHidrografica_Nenhum)
						throw std::invalid_argument("Nao foi possivel atribuir bacia hidrografica para " + getFullString(idHidreletrica) + ".");

					a_dados.vetorHidreletrica.at(idHidreletrica).setAtributo(AttComumHidreletrica_bacia, idBaciaHidrografica);
				}

				if (a_dados.getSizeVetor(idHidreletrica, AttVetorHidreletrica_tipo_detalhamento_producao) > 0) {

					const SmartEnupla<Periodo, TipoDetalhamentoProducaoHidreletrica> tipo_detalhamento_producao = a_dados.getVetor(idHidreletrica, AttVetorHidreletrica_tipo_detalhamento_producao, Periodo(), TipoDetalhamentoProducaoHidreletrica());

					for (Periodo periodo = tipo_detalhamento_producao.getIteradorInicial(); periodo <= tipo_detalhamento_producao.getIteradorFinal(); tipo_detalhamento_producao.incrementarIterador(periodo)) {
						if (tipo_detalhamento_producao.at(periodo) == TipoDetalhamentoProducaoHidreletrica_por_usina)
							tipo_detalhamento_producao_por_usina = true;
						else if (tipo_detalhamento_producao.at(periodo) == TipoDetalhamentoProducaoHidreletrica_por_conjunto)
							tipo_detalhamento_producao_por_conjunto = true;
						else if (tipo_detalhamento_producao.at(periodo) == TipoDetalhamentoProducaoHidreletrica_por_unidade)
							tipo_detalhamento_producao_por_unidade = true;
					}

				} // if (a_dados.getSizeVetor(idHidreletrica, AttVetorHidreletrica_tipo_detalhamento_producao) > 0) {

				else {

					const TipoDetalhamentoProducaoHidreletrica tipo_detalhamento_producao = a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica());

					if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_usina)
						tipo_detalhamento_producao_por_usina = true;
					else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto)
						tipo_detalhamento_producao_por_conjunto = true;
					else if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade)
						tipo_detalhamento_producao_por_unidade = true;

				}

			} // for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			if (tipo_detalhamento_producao_por_usina)
				entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("HIDRELETRICA_FPH_AttMatrizOperacional_PorPeriodoPorInteiro.csv", a_dados, TipoAcessoInstancia_m2);

			if (tipo_detalhamento_producao_por_conjunto)
				entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("HIDRELETRICA_CONJUNTO_FPH_AttMatrizOperacional_PorPeriodoPorInteiro.csv", a_dados, TipoAcessoInstancia_m2);

			if (tipo_detalhamento_producao_por_unidade)
				entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("HIDRELETRICA_CONJUNTO_UNIDADE_FPH_AttMatrizOperacional_PorPeriodoPorInteiro.csv", a_dados, TipoAcessoInstancia_m2);

			if ((a_dados.getAtributo(AttComumDados_tipo_geracao_cenario_hidrologico, TipoGeracaoCenario()) == TipoGeracaoCenario_sintetica_in_sample) ||
				(a_dados.getAtributo(AttComumDados_tipo_geracao_cenario_hidrologico, TipoGeracaoCenario()) == TipoGeracaoCenario_sintetica_out_of_sample)) {

				if (strCompara(a_dados.getAtributo(AttComumDados_diretorio_importacao_pre_estudo, std::string()), "nenhum")) {
					entradaSaidaDados.carregarArquivoCSV_AttVetor("HIDRELETRICA_AFLUENCIA_AttVetor_natural_historico.csv", a_dados, TipoAcessoInstancia_m2);
					entradaSaidaDados.carregarArquivoCSV_AttVetor("HIDRELETRICA_AFLUENCIA_AttVetor_natural_tendencia.csv", a_dados, TipoAcessoInstancia_m2);
				}
				else {
					const std::string diretorio_entrada = entradaSaidaDados.getDiretorioEntrada();
					entradaSaidaDados.setDiretorioEntrada(a_dados.getAtributo(AttComumDados_diretorio_importacao_pre_estudo, std::string()));
					entradaSaidaDados.carregarArquivoCSV_AttVetor("HIDRELETRICA_AFLUENCIA_AttVetor_natural_historico.csv", a_dados, TipoAcessoInstancia_m2);
					entradaSaidaDados.carregarArquivoCSV_AttVetor("HIDRELETRICA_AFLUENCIA_AttVetor_natural_tendencia.csv", a_dados, TipoAcessoInstancia_m2);
					entradaSaidaDados.setDiretorioEntrada(diretorio_entrada);
				}

				a_dados.setAtributo(AttComumDados_tipo_tendencia_hidrologica, TipoTendenciaEstocastica_serie_informada);

			}


			bool tendencia_natural = false;
			bool tendencia_incremental = false;
			if (a_dados.getVetor(AttVetorDados_horizonte_otimizacao, IdEstagio(), Periodo()).size() > 0) {

				Periodo periodo_final_tendencia = a_dados.getElementoVetor(AttVetorDados_horizonte_otimizacao, IdEstagio_1, Periodo());

				const IdHidreletrica idHidreletricaIni = a_dados.getMenorId(IdHidreletrica());
				const IdHidreletrica idHidreletricaOut = a_dados.getIdOut(IdHidreletrica());

				for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

					if (a_dados.vetorHidreletrica.at(idHidreletrica).vetorAfluencia.isInstanciado(IdAfluencia_vazao_afluente)) {

						if (a_dados.getSizeVetor(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_incremental_tendencia) > 0) {
							periodo_final_tendencia = a_dados.getIteradorFinal(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_incremental_tendencia, Periodo());
							tendencia_incremental = true;
						}

						else if (a_dados.getSizeVetor(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_natural_tendencia) > 0) {
							periodo_final_tendencia = a_dados.getIteradorFinal(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_natural_tendencia, Periodo());
							tendencia_natural = true;
						}
						

					} // if (a_dados.vetorHidreletrica.at(idHidreletrica).vetorAfluencia.isInstanciado(IdAfluencia_vazao_afluente)) {

				} // for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

				if (tendencia_natural && tendencia_incremental)
					throw std::invalid_argument("Apenas um tipo de tendencia deve ser instnciado. Natural ou incremental.");
				else if (tendencia_natural || tendencia_incremental){

					tendenciaPreConfig_instanciadas = true;

					a_dados.setAtributo(AttComumDados_tipo_tendencia_hidrologica, TipoTendenciaEstocastica_serie_informada);

					bool sobreposicao = false;
					IdEstagio estagio_acoplamento_pre_estudo = IdEstagio_1;
					for (IdEstagio idEstagio = IdEstagio_1; idEstagio <= a_dados.getIteradorFinal(AttVetorDados_horizonte_otimizacao, IdEstagio()); idEstagio++) {

						const Periodo periodo = a_dados.getElementoVetor(AttVetorDados_horizonte_otimizacao, idEstagio, Periodo());

						if ((periodo.sobreposicao(periodo_final_tendencia) > 0.0) && (!sobreposicao))
							sobreposicao = true;
						else if ((periodo.sobreposicao(periodo_final_tendencia) == 0.0) && (sobreposicao)) {
							estagio_acoplamento_pre_estudo = idEstagio;
							break;
						}
					}

					a_dados.setAtributo(AttComumDados_estagio_acoplamento_pre_estudo, estagio_acoplamento_pre_estudo);

				} // else if (tendencia_natural || tendencia_incremental){

			}////if (a_dados.getVetor(AttVetorDados_horizonte_otimizacao, IdEstagio(), Periodo()).size() > 0) {

			/////////////////////////////////////////////////////////////////////////////////////
			//Inicializa listas com os códigos das usinas agregadas e seu equivalente desagregado
			/////////////////////////////////////////////////////////////////////////////////////

			lista_codigo_ONS_hidreletrica_agregada.addElemento(1, 176);    // 176 COMP PAF-MOX: Complexo P.Afonso - Moxotó 
			lista_codigo_ONS_hidreletricas_desagregadas.addElemento(1, SmartEnupla<int, int>(1, std::vector<int>{173, 174, 175})); // 173 MOXOTO, 174 P. Afonso 123, 175 P. Afonso 4, 

			lista_codigo_ONS_hidreletrica_agregada.addElemento(2, 143);    // 143 FONTES
			lista_codigo_ONS_hidreletricas_desagregadas.addElemento(2, SmartEnupla<int, int>(1, std::vector<int>{183, 184})); // 183 FONTES A, 184 Fontes BC

		}//if (idHidreletricaEnd > IdHidreletrica_Nenhum) {

	}// try
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::instancia_hidreletricas_preConfig(): \n" + std::string(erro.what())); }

}

void LeituraCEPEL::instancia_processoEstocasticoHidrologico_preConfig(Dados& a_dados, const std::string a_diretorio){

	try {

		EntradaSaidaDados entradaSaidaDados;

		entradaSaidaDados.setDiretorioEntrada(a_diretorio);

		const std::string diretorio_entrada = entradaSaidaDados.getDiretorioEntrada();

		entradaSaidaDados.setDiretorioEntrada(diretorio_entrada + "//ProcessoEstocasticoHidrologico");

		bool processoEstocastico_totalmente_carregado = true;

		if (true) {
			ProcessoEstocastico processoEstocastico_hidrologico;
			if (!entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("PROCESSO_ESTOCASTICO_AttComumOperacional.csv", processoEstocastico_hidrologico, TipoAcessoInstancia_direto))
				processoEstocastico_totalmente_carregado = false;
			else if (!entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("PROCESSO_ESTOCASTICO_" + getString(AttMatrizProcessoEstocastico_probabilidade_realizacao) + ".csv", processoEstocastico_hidrologico, TipoAcessoInstancia_direto))
				processoEstocastico_totalmente_carregado = false;
			else if (!entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("PROCESSO_ESTOCASTICO_" + getString(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral) + ".csv", processoEstocastico_hidrologico, TipoAcessoInstancia_direto))
				processoEstocastico_totalmente_carregado = false;
			else if (!entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("VARIAVEL_ALEATORIA_" + getString(AttVetorVariavelAleatoria_tipo_relaxacao) + ".csv", processoEstocastico_hidrologico, TipoAcessoInstancia_m1))
				processoEstocastico_totalmente_carregado = false;
			else if (!entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("VARIAVEL_ALEATORIA_" + getString(AttMatrizVariavelAleatoria_residuo_espaco_amostral) + ".csv", processoEstocastico_hidrologico, TipoAcessoInstancia_m1))
				processoEstocastico_totalmente_carregado = false;
			else if (!entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("VARIAVEL_ALEATORIA_" + getString(AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao) + ".csv", processoEstocastico_hidrologico, TipoAcessoInstancia_m1))
				processoEstocastico_totalmente_carregado = false;
			else if (!entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("VARIAVEL_ALEATORIA_INTERNA_AttComumOperacional.csv", processoEstocastico_hidrologico, TipoAcessoInstancia_m2))
				processoEstocastico_totalmente_carregado = false;
			else if (!entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("VARIAVEL_ALEATORIA_INTERNA_" + getString(AttVetorVariavelAleatoriaInterna_coeficiente_participacao) + ".csv", processoEstocastico_hidrologico, TipoAcessoInstancia_m2))
				processoEstocastico_totalmente_carregado = false;
			else if (!entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("VARIAVEL_ALEATORIA_INTERNA_" + getString(AttVetorVariavelAleatoriaInterna_tendencia_temporal) + ".csv", processoEstocastico_hidrologico, TipoAcessoInstancia_m2))
				processoEstocastico_totalmente_carregado = false;
		}

		if (!processoEstocastico_totalmente_carregado)
			return;

		entradaSaidaDados.carregarArquivoCSV_AttComum("PROCESSO_ESTOCASTICO_AttComumOperacional.csv", a_dados.processoEstocastico_hidrologico, TipoAcessoInstancia_direto);
		entradaSaidaDados.carregarArquivoCSV_AttMatriz("PROCESSO_ESTOCASTICO_" + getString(AttMatrizProcessoEstocastico_probabilidade_realizacao) + ".csv", a_dados.processoEstocastico_hidrologico, TipoAcessoInstancia_direto);
		entradaSaidaDados.carregarArquivoCSV_AttMatriz("PROCESSO_ESTOCASTICO_" + getString(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral) + ".csv", a_dados.processoEstocastico_hidrologico, TipoAcessoInstancia_direto);
		entradaSaidaDados.carregarArquivoCSV_AttVetor("VARIAVEL_ALEATORIA_" + getString(AttVetorVariavelAleatoria_tipo_relaxacao) + ".csv", a_dados.processoEstocastico_hidrologico, TipoAcessoInstancia_m1);
		entradaSaidaDados.carregarArquivoCSV_AttMatriz("VARIAVEL_ALEATORIA_" + getString(AttMatrizVariavelAleatoria_residuo_espaco_amostral) + ".csv", a_dados.processoEstocastico_hidrologico, TipoAcessoInstancia_m1);
		entradaSaidaDados.carregarArquivoCSV_AttMatriz("VARIAVEL_ALEATORIA_" + getString(AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao) + ".csv", a_dados.processoEstocastico_hidrologico, TipoAcessoInstancia_m1);
		entradaSaidaDados.carregarArquivoCSV_AttComum("VARIAVEL_ALEATORIA_INTERNA_AttComumOperacional.csv", a_dados.processoEstocastico_hidrologico, TipoAcessoInstancia_m2);
		entradaSaidaDados.carregarArquivoCSV_AttVetor("VARIAVEL_ALEATORIA_INTERNA_" + getString(AttVetorVariavelAleatoriaInterna_coeficiente_participacao) + ".csv", a_dados.processoEstocastico_hidrologico, TipoAcessoInstancia_m2);
		entradaSaidaDados.carregarArquivoCSV_AttVetor("VARIAVEL_ALEATORIA_INTERNA_" + getString(AttVetorVariavelAleatoriaInterna_tendencia_temporal) + ".csv", a_dados.processoEstocastico_hidrologico, TipoAcessoInstancia_m2);

		processoEstocasticoHidrologicoPreConfig_instanciado = true;

		const IdHidreletrica idHidreletricaIni = a_dados.getMenorId(IdHidreletrica());
		const IdHidreletrica idHidreletricaOut = a_dados.getIdOut(IdHidreletrica());

		for (IdHidreletrica idUHE = idHidreletricaIni; idUHE < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idUHE)) {

				IdVariavelAleatoria idVar = IdVariavelAleatoria_Nenhum;
				IdVariavelAleatoriaInterna idVarInt = IdVariavelAleatoriaInterna_Nenhum;
				a_dados.processoEstocastico_hidrologico.getIdVariavelAleatoriaIdVariavelAleatoriaInternaFromIdFisico(idVar, idVarInt, idUHE);
				lista_hidreletrica_IdVariavelAleatoria.at(idUHE) = idVar;
				a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.at(idVar).setAtributo(AttComumVariavelAleatoria_nome, getFullString(idUHE));

		} // for (IdHidreletrica idUHE = idHidreletricaIni; idUHE < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idUHE)) {


		const int numero_cenarios = a_dados.processoEstocastico_hidrologico.getSizeMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral);

		if (numero_cenarios == 0)
			throw std::invalid_argument("Processo estocastico pre config sem cenarios em " + getFullString(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral) + ".");

		a_dados.setAtributo(AttComumDados_numero_cenarios, numero_cenarios);

		entradaSaidaDados.setDiretorioEntrada(diretorio_entrada);
		entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("HIDRELETRICA_AFLUENCIA_AttVetorPremissa_PorPeriodo.csv", a_dados, TipoAcessoInstancia_m2);


	}// try
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::instancia_processoEstocasticoHidrologico_preConfig(): \n" + std::string(erro.what())); }


}

void LeituraCEPEL::instancia_horizonte_preConfig(Dados& a_dados, const std::string a_diretorio) {

	try {

		EntradaSaidaDados entradaSaidaDados;

		entradaSaidaDados.setDiretorioEntrada(a_diretorio);
		entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("DADOS_AttVetorOperacional_PorIdEstagio.csv", a_dados, TipoAcessoInstancia_direto);

		entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("DADOS_AttVetorOperacional_PorPeriodo.csv", a_dados, TipoAcessoInstancia_direto);

		if ((a_dados.getSizeVetor(AttVetorDados_horizonte_estudo) == 0) && (a_dados.getSizeVetor(AttVetorDados_horizonte_otimizacao) != 0))
			throw std::invalid_argument("Os elementos de ambos " + getFullString(AttVetorDados_horizonte_estudo) + " e " + getFullString(AttVetorDados_horizonte_otimizacao) + " dever ser carregados na pre configuracao.");

		if ((a_dados.getSizeVetor(AttVetorDados_horizonte_estudo) != 0) && (a_dados.getSizeVetor(AttVetorDados_horizonte_otimizacao) == 0))
			throw std::invalid_argument("Os elementos de ambos " + getFullString(AttVetorDados_horizonte_estudo) + " e " + getFullString(AttVetorDados_horizonte_otimizacao) + " dever ser carregados na pre configuracao.");

	}// try
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::instancia_horizonte_preConfig(): \n" + std::string(erro.what())); }

} // void LeituraCEPEL::instancia_dados_preConfig(Dados& a_dados, const std::string a_diretorio){


void LeituraCEPEL::instancia_dados_preConfig(Dados& a_dados, const std::string a_diretorio) {

	try {

		EntradaSaidaDados entradaSaidaDados;

		entradaSaidaDados.setDiretorioEntrada(a_diretorio);
		dadosPreConfig_instanciados = entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("DADOS_AttComumOperacional.csv", a_dados, TipoAcessoInstancia_direto);

		if ((a_dados.getSizeVetor(AttVetorDados_horizonte_estudo) > 0) && (dadosPreConfig_instanciados)) {

			const Periodo periodo_referencia = a_dados.getAtributo(AttComumDados_periodo_referencia, Periodo());

			if (periodo_referencia != Periodo(periodo_referencia.getTipoPeriodo(), a_dados.getIteradorInicial(AttVetorDados_horizonte_estudo, Periodo())))
				throw std::invalid_argument("Atributo " + getFullString(AttComumDados_periodo_referencia) + " nao compativel com " + getFullString(AttVetorDados_horizonte_estudo));

		} // if ((a_dados.getSizeVetor(AttVetorDados_horizonte_estudo) > 0) && (dadosPreConfig_instanciados)) {


	}// try
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::instancia_dados_preConfig(): \n" + std::string(erro.what())); }

} // void LeituraCEPEL::instancia_dados_preConfig(Dados& a_dados, const std::string a_diretorio){

void LeituraCEPEL::instancia_dados_matriz_preConfig(Dados& a_dados, const std::string a_diretorio) {

	try {

		EntradaSaidaDados entradaSaidaDados;

		entradaSaidaDados.setDiretorioEntrada(a_diretorio);

		if (entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("DADOS_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", a_dados, TipoAcessoInstancia_direto)) {
			
			if ((a_dados.getSizeVetor(AttVetorDados_horizonte_estudo) == 0))
				throw std::invalid_argument("Para validar " + getFullString(AttMatrizDados_percentual_duracao_patamar_carga) + " deve ser instanciado na pre-config o " + getFullString(AttVetorDados_horizonte_estudo));

			const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

			//Tenta dar um get varrendo o horizonte_estudo
			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
				const double percentual_duracao_patamar_carga = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga_1, double());

		} // 

	}// try
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::instancia_dados_matriz_preConfig(): \n" + std::string(erro.what())); }

} // void LeituraCEPEL::instancia_dados_preConfig(Dados& a_dados, const std::string a_diretorio){


void LeituraCEPEL::inicializa_arquivo_avisos() {

	//
	// Inicialização de arquivo de avisos.
	//

	try {
		std::ofstream escritaStream;
		escritaStream.open(SPT_avisos, std::ios_base::out);
		if (!escritaStream.is_open())
			throw std::invalid_argument("Nao foi possivel abrir o arquivo.");
		escritaStream << "2;EM_EXECUCAO" << std::endl;
		escritaStream.close();
		escritaStream.clear();
	} // try
	catch (const std::ofstream::failure& erro) { throw std::invalid_argument("Erro de integridade do arquivo " + SPT_avisos + ": \n" + std::string(erro.what())); }
	catch (const std::exception& erro) { throw std::invalid_argument("Erro ao editar o arquivo " + SPT_avisos + ": \n" + std::string(erro.what())); }

}


void LeituraCEPEL::aplicarModificacoesUHE(Dados& a_dados) {
	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const IdHidreletrica idHidreletricaIni = a_dados.getMenorId(IdHidreletrica());
		const IdHidreletrica idHidreletricaOut = a_dados.getIdOut(IdHidreletrica());

		for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			if (lista_modificacaoUHE.size() <= int(idHidreletrica))
				return;

			if (lista_modificacaoUHE.at(idHidreletrica).size() > 0) {

				SmartEnupla<int, std::vector<Periodo>>                 lista_periodo_modificacao_dinamica(int(TipoModificacaoUHE_Nenhum + 1), std::vector<std::vector<Periodo>>(int(TipoModificacaoUHE_Excedente - 1), std::vector<Periodo>()));
				SmartEnupla<int, std::vector<std::vector<double>>>     lista_valor_modificacao_dinamica(int(TipoModificacaoUHE_Nenhum + 1), std::vector<std::vector<std::vector<double>>>(int(TipoModificacaoUHE_Excedente - 1), std::vector<std::vector<double>>(6, std::vector<double>())));
				SmartEnupla<int, std::vector<TipoGrandezaModificacao>> lista_grandeza_modificacao_dinamica(int(TipoModificacaoUHE_Nenhum + 1), std::vector<std::vector<TipoGrandezaModificacao>>(int(TipoModificacaoUHE_Excedente - 1), std::vector<TipoGrandezaModificacao>()));

				// 
				// Aplica Modificações Estáticas
				//
				for (int idModificacaoUHE = 0; idModificacaoUHE < lista_modificacaoUHE.at(idHidreletrica).size(); idModificacaoUHE++) {

					const ModificacaoUHE modificacaoUHE = lista_modificacaoUHE.at(idHidreletrica).at(idModificacaoUHE);

					const TipoModificacaoUHE tipoModificacao = modificacaoUHE.tipo_de_modificacao;

					if (tipoModificacao == TipoModificacaoUHE_IP)
						aplicarModificacaoIP(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_TEIF)
						aplicarModificacaoTEIF(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_NUMCNJ)
						aplicarModificacaoNUMCNJ(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_NUMMAQ)
						aplicarModificacaoNUMMAQ(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_VOLMIN)
						aplicarModificacaoVOLMIN(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_VOLMAX)
						aplicarModificacaoVOLMAX(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_POTEFE)
						aplicarModificacaoPOTEFE(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_NUMBAS)
						aplicarModificacaoNUMBAS(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_VAZMIN)
						aplicarModificacaoVAZMIN(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_VAZMINHISTORICA)
						aplicarModificacaoVAZMINHISTORICA(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_COTAREA)
						aplicarModificacaoCOTAREA(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_VOLCOTA)
						aplicarModificacaoVOLCOTA(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_COTARE) {
						const int coeficiente = int(modificacaoUHE.valor_1);
						bool modificacao_encontrada = false;

						if (int(lista_periodo_modificacao_dinamica.at(tipoModificacao).size()) == 0)
							modificacao_encontrada = true;
						else if (lista_periodo_modificacao_dinamica.at(tipoModificacao).at(int(lista_periodo_modificacao_dinamica.at(tipoModificacao).size()) - 1) < modificacaoUHE.periodo)
							modificacao_encontrada = true;

						if (modificacao_encontrada) {

							lista_periodo_modificacao_dinamica.at(tipoModificacao).push_back(modificacaoUHE.periodo);
							lista_valor_modificacao_dinamica.at(tipoModificacao).at(1).push_back(NAN);
							lista_valor_modificacao_dinamica.at(tipoModificacao).at(2).push_back(NAN);
							lista_valor_modificacao_dinamica.at(tipoModificacao).at(3).push_back(NAN);
							lista_valor_modificacao_dinamica.at(tipoModificacao).at(4).push_back(NAN);
							lista_valor_modificacao_dinamica.at(tipoModificacao).at(5).push_back(NAN);
							lista_valor_modificacao_dinamica.at(tipoModificacao).at(coeficiente).at(lista_periodo_modificacao_dinamica.at(tipoModificacao).size() - 1) = modificacaoUHE.valor_2;
							lista_grandeza_modificacao_dinamica.at(tipoModificacao).push_back(modificacaoUHE.tipo_de_grandeza);

						} // if (modificacao_encontrada) {


						for (int i = 0; i < int(lista_periodo_modificacao_dinamica.at(tipoModificacao).size()); i++) {
							if (lista_periodo_modificacao_dinamica.at(tipoModificacao).at(i) == modificacaoUHE.periodo) {
								modificacao_encontrada = true;
								lista_valor_modificacao_dinamica.at(tipoModificacao).at(coeficiente).at(i) = modificacaoUHE.valor_2;
								lista_grandeza_modificacao_dinamica.at(tipoModificacao).at(i) = modificacaoUHE.tipo_de_grandeza;
								break;
							} // if (lista_periodo_modificacao_dinamica.at(tipoModificacao).at(i) == modificacaoUHE.periodo) {
						} // for (int i = 0; i < int(lista_periodo_modificacao_dinamica.at(tipoModificacao).size()); i++) {
					} // else if (tipoModificacao == TipoModificacaoUHE_COTARE) {

					else if (tipoModificacao == TipoModificacaoUHE_COTVOL) {
						const int coeficiente = int(modificacaoUHE.valor_1);
						bool modificacao_encontrada = false;

						if (int(lista_periodo_modificacao_dinamica.at(tipoModificacao).size()) == 0)
							modificacao_encontrada = true;
						else if (lista_periodo_modificacao_dinamica.at(tipoModificacao).at(int(lista_periodo_modificacao_dinamica.at(tipoModificacao).size()) - 1) < modificacaoUHE.periodo)
							modificacao_encontrada = true;

						if (modificacao_encontrada) {

							lista_periodo_modificacao_dinamica.at(tipoModificacao).push_back(modificacaoUHE.periodo);
							lista_valor_modificacao_dinamica.at(tipoModificacao).at(1).push_back(NAN);
							lista_valor_modificacao_dinamica.at(tipoModificacao).at(2).push_back(NAN);
							lista_valor_modificacao_dinamica.at(tipoModificacao).at(3).push_back(NAN);
							lista_valor_modificacao_dinamica.at(tipoModificacao).at(4).push_back(NAN);
							lista_valor_modificacao_dinamica.at(tipoModificacao).at(5).push_back(NAN);
							lista_valor_modificacao_dinamica.at(tipoModificacao).at(coeficiente).at(lista_periodo_modificacao_dinamica.at(tipoModificacao).size() - 1) = modificacaoUHE.valor_2;
							lista_grandeza_modificacao_dinamica.at(tipoModificacao).push_back(modificacaoUHE.tipo_de_grandeza);

						} // if (modificacao_encontrada) {

						for (int i = 0; i < int(lista_periodo_modificacao_dinamica.at(tipoModificacao).size()); i++) {
							if (lista_periodo_modificacao_dinamica.at(tipoModificacao).at(i) == modificacaoUHE.periodo) {
								modificacao_encontrada = true;
								lista_valor_modificacao_dinamica.at(tipoModificacao).at(coeficiente).at(i) = modificacaoUHE.valor_2;
								lista_grandeza_modificacao_dinamica.at(tipoModificacao).at(i) = modificacaoUHE.tipo_de_grandeza;
								break;
							} // if (lista_periodo_modificacao_dinamica.at(tipoModificacao).at(i) == modificacaoUHE.periodo) {
						} // for (int i = 0; i < int(lista_periodo_modificacao_dinamica.at(tipoModificacao).size()); i++) {
					} // else if (tipoModificacao == TipoModificacaoUHE_COTVOL) {

					else if (tipoModificacao == TipoModificacaoUHE_CMONT) {
						lista_periodo_modificacao_dinamica.at(tipoModificacao).push_back(modificacaoUHE.periodo);
						lista_valor_modificacao_dinamica.at(tipoModificacao).at(1).push_back(modificacaoUHE.valor_1);
						lista_grandeza_modificacao_dinamica.at(tipoModificacao).push_back(modificacaoUHE.tipo_de_grandeza);
					}

					else if (tipoModificacao == TipoModificacaoUHE_PRODESP)
						aplicarModificacaoPRODESP(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_COEFEVAP)
						aplicarModificacaoCOEFEVAP(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_PERDHIDR)
						aplicarModificacaoPERDHIDR(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_CFUGA) {
						lista_periodo_modificacao_dinamica.at(tipoModificacao).push_back(modificacaoUHE.periodo);
						lista_valor_modificacao_dinamica.at(tipoModificacao).at(1).push_back(modificacaoUHE.valor_1);
						lista_grandeza_modificacao_dinamica.at(tipoModificacao).push_back(modificacaoUHE.tipo_de_grandeza);
					}

					else if (tipoModificacao == TipoModificacaoUHE_VMAXT) {
						lista_periodo_modificacao_dinamica.at(tipoModificacao).push_back(modificacaoUHE.periodo);
						lista_valor_modificacao_dinamica.at(tipoModificacao).at(1).push_back(modificacaoUHE.valor_1);
						lista_grandeza_modificacao_dinamica.at(tipoModificacao).push_back(modificacaoUHE.tipo_de_grandeza);
					}

					else if (tipoModificacao == TipoModificacaoUHE_VMINT) {
						lista_periodo_modificacao_dinamica.at(tipoModificacao).push_back(modificacaoUHE.periodo);
						lista_valor_modificacao_dinamica.at(tipoModificacao).at(1).push_back(modificacaoUHE.valor_1);
						lista_grandeza_modificacao_dinamica.at(tipoModificacao).push_back(modificacaoUHE.tipo_de_grandeza);
					}

					else if (tipoModificacao == TipoModificacaoUHE_VMINP) {
						lista_periodo_modificacao_dinamica.at(tipoModificacao).push_back(modificacaoUHE.periodo);
						lista_valor_modificacao_dinamica.at(tipoModificacao).at(1).push_back(modificacaoUHE.valor_1);
						lista_grandeza_modificacao_dinamica.at(tipoModificacao).push_back(modificacaoUHE.tipo_de_grandeza);
					}

					else if (tipoModificacao == TipoModificacaoUHE_VMORTO)
						aplicarModificacaoVMORTO(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_NOVAMAQ)
						aplicarModificacaoNOVAMAQ(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_VAZMINT) {
						lista_periodo_modificacao_dinamica.at(tipoModificacao).push_back(modificacaoUHE.periodo);
						lista_valor_modificacao_dinamica.at(tipoModificacao).at(1).push_back(modificacaoUHE.valor_1);
						lista_grandeza_modificacao_dinamica.at(tipoModificacao).push_back(modificacaoUHE.tipo_de_grandeza);
					}

					else if (tipoModificacao == TipoModificacaoUHE_NOMEUH)
						aplicarModificacaoNOMEUH(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_NUMJUS)
						aplicarModificacaoNUMJUS(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_DESVIO)
						aplicarModificacaoDESVIO(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_VAZEFE)
						aplicarModificacaoVAZEFE(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_VMDESV)
						aplicarModificacaoVMDESV(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_NUMPOS)
						aplicarModificacaoNUMPOS(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_NPOSNW)
						aplicarModificacaoNPOSNW(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_NCHAVE)
						aplicarModificacaoNCHAVE(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_COTVAZ)
						aplicarModificacaoCOTVAZ(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_VERTJU)
						aplicarModificacaoVERTJU(a_dados, idHidreletrica, modificacaoUHE);

					else if (tipoModificacao == TipoModificacaoUHE_JUSENA)
						aplicarModificacaoJUSENA(a_dados, idHidreletrica, modificacaoUHE);

				} // for (int idModificacaoUHE = 0; idModificacaoUHE < lista_modificacaoUHE.at(idHidreletrica).size(); idModificacaoUHE++) {

				// 
				// Aplica Modificações Dinâmicas
				//
				for (int i = 1; i <= lista_periodo_modificacao_dinamica.size(); i++) {

					const TipoModificacaoUHE tipoModificacao = TipoModificacaoUHE(i);

					if (lista_periodo_modificacao_dinamica.at(tipoModificacao).size() > 0) {

						if (tipoModificacao == TipoModificacaoUHE_COTARE)
							aplicarModificacaoCOTARE(a_dados, idHidreletrica, lista_periodo_modificacao_dinamica.at(tipoModificacao), lista_valor_modificacao_dinamica.at(tipoModificacao), lista_grandeza_modificacao_dinamica.at(tipoModificacao));

						else if (tipoModificacao == TipoModificacaoUHE_COTVOL)
							aplicarModificacaoCOTVOL(a_dados, idHidreletrica, lista_periodo_modificacao_dinamica.at(tipoModificacao), lista_valor_modificacao_dinamica.at(tipoModificacao), lista_grandeza_modificacao_dinamica.at(tipoModificacao));

						else if (tipoModificacao == TipoModificacaoUHE_CMONT)
							aplicarModificacaoCMONT(a_dados, idHidreletrica, lista_periodo_modificacao_dinamica.at(tipoModificacao), lista_valor_modificacao_dinamica.at(tipoModificacao), lista_grandeza_modificacao_dinamica.at(tipoModificacao));

						else if (tipoModificacao == TipoModificacaoUHE_CFUGA)
							aplicarModificacaoCFUGA(a_dados, idHidreletrica, lista_periodo_modificacao_dinamica.at(tipoModificacao), lista_valor_modificacao_dinamica.at(tipoModificacao), lista_grandeza_modificacao_dinamica.at(tipoModificacao));

						else if (tipoModificacao == TipoModificacaoUHE_VMAXT)
							aplicarModificacaoVMAXT(a_dados, idHidreletrica, lista_periodo_modificacao_dinamica.at(tipoModificacao), lista_valor_modificacao_dinamica.at(tipoModificacao), lista_grandeza_modificacao_dinamica.at(tipoModificacao));

						else if (tipoModificacao == TipoModificacaoUHE_VMINT)
							aplicarModificacaoVMINT(a_dados, idHidreletrica, lista_periodo_modificacao_dinamica.at(tipoModificacao), lista_valor_modificacao_dinamica.at(tipoModificacao), lista_grandeza_modificacao_dinamica.at(tipoModificacao));

						else if (tipoModificacao == TipoModificacaoUHE_VMINP)
							aplicarModificacaoVMINP(a_dados, idHidreletrica, lista_periodo_modificacao_dinamica.at(tipoModificacao), lista_valor_modificacao_dinamica.at(tipoModificacao), lista_grandeza_modificacao_dinamica.at(tipoModificacao));

						else if (tipoModificacao == TipoModificacaoUHE_VAZMINT)
							aplicarModificacaoVAZMINT(a_dados, idHidreletrica, lista_periodo_modificacao_dinamica.at(tipoModificacao), lista_valor_modificacao_dinamica.at(tipoModificacao), lista_grandeza_modificacao_dinamica.at(tipoModificacao));

					} // if (lista_periodo_modificacao_dinamica.at(tipoModificacao).size() > 0){

				} // for (int i = 0; i < lista_periodo_modificacao_dinamica.size(); i++) {

			} // if (lista_modificacaoUHE.at(idHidreletrica).size() > 0){

		} // for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacoesUHE(a_dados): \n" + std::string(erro.what())); }
} // void LeituraCEPEL::aplicarModificacoesUHE(Dados &a_dados){


bool LeituraCEPEL::aplicarModificacaoIP(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_IP)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const double indisponibilidade_programada = a_modificacaoUHE.valor_1;
		a_dados.vetorHidreletrica.at(a_idHidreletrica).setAtributo(AttComumHidreletrica_indisponibilidade_programada, indisponibilidade_programada);
		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoIP(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoIP(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE) {

bool LeituraCEPEL::aplicarModificacaoTEIF(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_TEIF)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const double indisponibilidade_forcada = a_modificacaoUHE.valor_1;
		a_dados.vetorHidreletrica.at(a_idHidreletrica).setAtributo(AttComumHidreletrica_indisponibilidade_forcada, indisponibilidade_forcada);
		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoTEIF(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoTEIF(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE){

bool LeituraCEPEL::aplicarModificacaoNUMCNJ(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_NUMCNJ)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const IdConjuntoHidraulico idConjunto = IdConjuntoHidraulico(int(a_modificacaoUHE.valor_1));

		lista_hidreletrica_maiorIdConjuntoHidraulico.setElemento(a_idHidreletrica, idConjunto);

		const IdSubmercado idSubmercado = lista_IdSubmercado_hidreletrica.getElemento(a_idHidreletrica);

		const IdConjuntoHidraulico maiorIdConjuntoHidraulico = a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico());

		if (idConjunto > maiorIdConjuntoHidraulico) {
			for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico(int(maiorIdConjuntoHidraulico) + 1); idConjuntoHidraulico <= idConjunto; idConjuntoHidraulico++) {
				ConjuntoHidraulico conjuntohidraulico;
				conjuntohidraulico.setAtributo(AttComumConjuntoHidraulico_idConjuntoHidraulico, idConjuntoHidraulico);
				conjuntohidraulico.setAtributo(AttComumConjuntoHidraulico_submercado, idSubmercado);
				conjuntohidraulico.setAtributo(AttComumConjuntoHidraulico_queda_referencia, 0.0);
				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.add(conjuntohidraulico);
			}
		}//if (idConjunto > maiorIdConjuntoHidraulico) {

		if (idConjunto <= maiorIdConjuntoHidraulico) {
			for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico(idConjunto + 1); idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {

				const IdUnidadeUHE maiorIdUnidadeUHE = a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE());

				for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= maiorIdUnidadeUHE; idUnidade++)
					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidade).setVetor(AttVetorUnidadeUHE_disponibilidade, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

			}
		}// if (idConjunto < int(maiorIdConjuntoHidraulico)) {

		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoNUMCNJ(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoNUMCNJ(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE){

bool LeituraCEPEL::aplicarModificacaoNUMMAQ(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_NUMMAQ)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const int numero_maquinas = int(a_modificacaoUHE.valor_1);
		const IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico(int(a_modificacaoUHE.valor_2));
		const IdUnidadeUHE maiorIdUnidadeUHE = (a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()));

		if (maiorIdUnidadeUHE == IdUnidadeUHE_Nenhum) {

			for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= IdUnidadeUHE(numero_maquinas); idUnidade++) {
				UnidadeUHE unidadeUHE;
				unidadeUHE.setAtributo(AttComumUnidadeUHE_idUnidadeUHE, idUnidade);
				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.add(unidadeUHE);

				if (a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada) == 0)
					a_dados.vetorHidreletrica.at(a_idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_forcada, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_indisponibilidade_forcada, double())));

				if (a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada) == 0)
					a_dados.vetorHidreletrica.at(a_idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_programada, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_indisponibilidade_programada, double())));

				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidade).setVetor(AttVetorUnidadeUHE_disponibilidade, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

				for (Periodo periodo = a_modificacaoUHE.periodo; periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

					const double indisponibilidade_forcada = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada, periodo, double());
					const double indisponibilidade_programada = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada, periodo, double());
					const double disponibilidade = (1.0 - indisponibilidade_forcada) * (1.0 - indisponibilidade_programada);

					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidade).setElemento(AttVetorUnidadeUHE_disponibilidade, periodo, disponibilidade);

				} // for (Periodo periodo = disponibilidade_unidade.getIteradorInicial(); periodo <= disponibilidade_unidade.getIteradorFinal(); disponibilidade_unidade.incrementarIterador(periodo)) {

			}//for (IdUnidadeUHE idUnidade = maiorIdUnidadeUHE; idUnidade <= IdUnidadeUHE(numero_maquinas); idUnidade++) {

		} // if (numero_maquinas > int(maiorIdUnidadeUHE)) {

		else if (numero_maquinas > int(maiorIdUnidadeUHE)) {

			for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= IdUnidadeUHE(numero_maquinas); idUnidade++) {

				if (idUnidade >= IdUnidadeUHE(maiorIdUnidadeUHE + 1)) {
					UnidadeUHE unidadeUHE;
					unidadeUHE.setAtributo(AttComumUnidadeUHE_idUnidadeUHE, idUnidade);

					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.add(unidadeUHE);

				}//if (idUnidade >= IdUnidadeUHE(maiorIdUnidadeUHE + 1)) {

				if (a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidade).getSizeVetor(AttVetorUnidadeUHE_disponibilidade) == 0) {

					if (a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada) == 0)
						a_dados.vetorHidreletrica.at(a_idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_forcada, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_indisponibilidade_forcada, double())));

					if (a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada) == 0)
						a_dados.vetorHidreletrica.at(a_idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_programada, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_indisponibilidade_programada, double())));

					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidade).setVetor(AttVetorUnidadeUHE_disponibilidade, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

					for (Periodo periodo = a_modificacaoUHE.periodo; periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						const double indisponibilidade_forcada = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada, periodo, double());
						const double indisponibilidade_programada = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada, periodo, double());
						const double disponibilidade = (1.0 - indisponibilidade_forcada) * (1.0 - indisponibilidade_programada);

						a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidade).setElemento(AttVetorUnidadeUHE_disponibilidade, periodo, disponibilidade);

					} // for (Periodo periodo = disponibilidade_unidade.getIteradorInicial(); periodo <= disponibilidade_unidade.getIteradorFinal(); disponibilidade_unidade.incrementarIterador(periodo)) {

				}//if (a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidade).getSizeVetor(AttVetorUnidadeUHE_disponibilidade) == 0) {
				else {

					if (a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidade).getElementoVetor(AttVetorUnidadeUHE_disponibilidade, a_modificacaoUHE.periodo, double()) == 0) {

						if (a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada) == 0)
							a_dados.vetorHidreletrica.at(a_idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_forcada, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_indisponibilidade_forcada, double())));

						if (a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada) == 0)
							a_dados.vetorHidreletrica.at(a_idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_programada, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_indisponibilidade_programada, double())));

						for (Periodo periodo = a_modificacaoUHE.periodo; periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							const double indisponibilidade_forcada = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada, periodo, double());
							const double indisponibilidade_programada = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada, periodo, double());
							const double disponibilidade = (1.0 - indisponibilidade_forcada) * (1.0 - indisponibilidade_programada);

							a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidade).setElemento(AttVetorUnidadeUHE_disponibilidade, periodo, disponibilidade);

						} // for (Periodo periodo = disponibilidade_unidade.getIteradorInicial(); periodo <= disponibilidade_unidade.getIteradorFinal(); disponibilidade_unidade.incrementarIterador(periodo)) {

					}//if (a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidade).getElementoVetor(AttVetorUnidadeUHE_disponibilidade, a_modificacaoUHE.periodo, double()) == 0) {

				}//else {

			}//for (IdUnidadeUHE idUnidade = maiorIdUnidadeUHE; idUnidade <= IdUnidadeUHE(numero_maquinas); idUnidade++) {

		} // else if (numero_maquinas > int(maiorIdUnidadeUHE)) {

		else if (numero_maquinas <= int(maiorIdUnidadeUHE)) {

			for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= maiorIdUnidadeUHE; idUnidade++) {

				if (int(idUnidade) <= numero_maquinas) {

					if (a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidade).getSizeVetor(AttVetorUnidadeUHE_disponibilidade) == 0) {

						if (a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada) == 0)
							a_dados.vetorHidreletrica.at(a_idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_forcada, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_indisponibilidade_forcada, double())));

						if (a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada) == 0)
							a_dados.vetorHidreletrica.at(a_idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_programada, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_indisponibilidade_programada, double())));

						a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidade).setVetor(AttVetorUnidadeUHE_disponibilidade, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

						for (Periodo periodo = a_modificacaoUHE.periodo; periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							const double indisponibilidade_forcada = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada, periodo, double());
							const double indisponibilidade_programada = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada, periodo, double());
							const double disponibilidade = (1.0 - indisponibilidade_forcada) * (1.0 - indisponibilidade_programada);

							a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidade).setElemento(AttVetorUnidadeUHE_disponibilidade, periodo, disponibilidade);

						} // for (Periodo periodo = disponibilidade_unidade.getIteradorInicial(); periodo <= disponibilidade_unidade.getIteradorFinal(); disponibilidade_unidade.incrementarIterador(periodo)) {

					}//if (a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidade).getSizeVetor(AttVetorUnidadeUHE_disponibilidade) == 0) {
					else {

						if (a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidade).getElementoVetor(AttVetorUnidadeUHE_disponibilidade, a_modificacaoUHE.periodo, double()) == 0) {

							if (a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada) == 0)
								a_dados.vetorHidreletrica.at(a_idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_forcada, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_indisponibilidade_forcada, double())));

							if (a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada) == 0)
								a_dados.vetorHidreletrica.at(a_idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_programada, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_indisponibilidade_programada, double())));

							for (Periodo periodo = a_modificacaoUHE.periodo; periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								const double indisponibilidade_forcada = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada, periodo, double());
								const double indisponibilidade_programada = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada, periodo, double());
								const double disponibilidade = (1.0 - indisponibilidade_forcada) * (1.0 - indisponibilidade_programada);

								a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidade).setElemento(AttVetorUnidadeUHE_disponibilidade, periodo, disponibilidade);

							} // for (Periodo periodo = disponibilidade_unidade.getIteradorInicial(); periodo <= disponibilidade_unidade.getIteradorFinal(); disponibilidade_unidade.incrementarIterador(periodo)) {

						}//if (a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidade).getElementoVetor(AttVetorUnidadeUHE_disponibilidade, a_modificacaoUHE.periodo, double()) == 0) {

					}//else {

				}//if (int(idUnidade) <= numero_maquinas) {
				else
					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidade).setVetor(AttVetorUnidadeUHE_disponibilidade, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

			}//for (IdUnidadeUHE idUnidade = maiorIdUnidadeUHE; idUnidade > IdUnidadeUHE(numero_maquinas); idUnidade--) {

		} // else if (numero_maquinas <= int(maiorIdUnidadeUHE)) {

		return true;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoNUMMAQ(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoNUMMAQ(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE, const SmartEnupla<Periodo, IdEstagio>& a_horizonte_estudo){

bool LeituraCEPEL::aplicarModificacaoVOLMIN(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_VOLMIN)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const TipoGrandezaModificacao tipo_de_grandeza = a_modificacaoUHE.tipo_de_grandeza;

		const double valor_modificacao = a_modificacaoUHE.valor_1;

		if (valor_modificacao < 0)
			throw std::invalid_argument("Valor Negativo Modificacao.");

		const double volume_minimo = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_minimo, double());

		const double volume_maximo = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_maximo, double());

		double volume_minimo_novo = valor_modificacao;

		if (tipo_de_grandeza == TipoGrandezaModificacao_percentual) {
			if (valor_modificacao > 1.0)
				throw std::invalid_argument("Modificacao de volume maior que 1.0 (100%).");
			volume_minimo_novo = volume_minimo + ((volume_maximo - volume_minimo) * valor_modificacao);
		}

		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setAtributo(AttComumReservatorio_volume_minimo, volume_minimo_novo);

		if (a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getSizeVetor(AttVetorReservatorio_volume_minimo) > 0) {

			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_minimo, periodo, volume_minimo_novo);

		}//if (a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getSizeVetor(AttVetorReservatorio_volume_minimo) > 0) {

		//////////////////////////////////////////
		const double cota_referencia = a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getCotaMedia(a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_minimo, double()), a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_maximo, double()));

		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setAtributo(AttComumReservatorio_cota_referencia, cota_referencia);

		if (a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getSizeVetor(AttVetorReservatorio_cota_referencia) > 0) {

			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_cota_referencia, periodo, cota_referencia);

		}//if (a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getSizeVetor(AttVetorReservatorio_volume_minimo) > 0) {

		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoVOLMIN(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoVOLMIN(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE){

bool LeituraCEPEL::aplicarModificacaoVOLMAX(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_VOLMAX)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const TipoGrandezaModificacao tipo_de_grandeza = a_modificacaoUHE.tipo_de_grandeza;

		const double valor_modificacao = a_modificacaoUHE.valor_1;

		if (valor_modificacao < 0)
			throw std::invalid_argument("Valor Negativo Modificacao.");

		const double volume_minimo = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_minimo, double());

		const double volume_maximo = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_maximo, double());

		double volume_maximo_novo = valor_modificacao;

		if (tipo_de_grandeza == TipoGrandezaModificacao_percentual) {
			if (valor_modificacao > 1.0)
				throw std::invalid_argument("Modificacao de volume maior que 1.0 (100%).");
			volume_maximo_novo += volume_minimo + ((volume_maximo - volume_minimo) * valor_modificacao);
		}

		//////////////////////////////////////////

		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setAtributo(AttComumReservatorio_volume_maximo, volume_maximo_novo);

		if (a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getSizeVetor(AttVetorReservatorio_volume_maximo) > 0) {

			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_maximo, periodo, volume_maximo_novo);

		}//if (a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getSizeVetor(AttVetorReservatorio_volume_minimo) > 0) {


		//////////////////////////////////////////
		const double cota_referencia = a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getCotaMedia(a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_minimo, double()), a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_maximo, double()));

		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setAtributo(AttComumReservatorio_cota_referencia, cota_referencia);

		if (a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getSizeVetor(AttVetorReservatorio_cota_referencia) > 0) {

			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_cota_referencia, periodo, cota_referencia);

		}//if (a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getSizeVetor(AttVetorReservatorio_volume_minimo) > 0) {


		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoVOLMAX(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoVOLMAX(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE, const SmartEnupla<Periodo, IdEstagio>& a_horizonte_estudo){

bool LeituraCEPEL::aplicarModificacaoPOTEFE(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {

		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_POTEFE)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());
		const IdConjuntoHidraulico idConjunto = IdConjuntoHidraulico(int(a_modificacaoUHE.valor_2));

		const double potencia_maxima_maquina = a_modificacaoUHE.valor_1;

		const IdUnidadeUHE maiorIdUnidadeUHE = (a_dados.getMaiorId(a_idHidreletrica, idConjunto, IdUnidadeUHE()));

		for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= maiorIdUnidadeUHE; idUnidade++)
			a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjunto).vetorUnidadeUHE.at(idUnidade).setAtributo(AttComumUnidadeUHE_potencia_maxima, potencia_maxima_maquina);

		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoPOTEFE(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoPOTEFE(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE){

bool LeituraCEPEL::aplicarModificacaoNUMBAS(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_NUMBAS)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const int numero_maquinas_base = int(a_modificacaoUHE.valor_1);
		a_dados.vetorHidreletrica.at(a_idHidreletrica).setAtributo(AttComumHidreletrica_numero_maquinas_base, numero_maquinas_base);
		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoNUMBAS(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoNUMBAS(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE){

bool LeituraCEPEL::aplicarModificacaoVAZMIN(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_VAZMIN)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const double vazao_defluente_minima = a_modificacaoUHE.valor_1;
		a_dados.vetorHidreletrica.at(a_idHidreletrica).setAtributo(AttComumHidreletrica_vazao_defluente_minima, vazao_defluente_minima);
		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoVAZMIN(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoVAZMIN(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE){


bool LeituraCEPEL::aplicarModificacaoVAZMINHISTORICA(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_VAZMINHISTORICA)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const double vazao_defluente_disponivel_minima_historica = a_modificacaoUHE.valor_1;

		a_dados.vetorHidreletrica.at(a_idHidreletrica).setAtributo(AttComumHidreletrica_vazao_defluente_minima_historica, vazao_defluente_disponivel_minima_historica);

		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoVAZMIN(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoVAZMIN(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE){


bool LeituraCEPEL::aplicarModificacaoCOTAREA(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_COTAREA)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const double valor_0 = a_modificacaoUHE.valor_1;
		const double valor_1 = a_modificacaoUHE.valor_2;
		const double valor_2 = a_modificacaoUHE.valor_3;
		const double valor_3 = a_modificacaoUHE.valor_4;
		const double valor_4 = a_modificacaoUHE.valor_5;

		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_area_0, valor_0);
		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_area_1, valor_1);
		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_area_1, valor_2);
		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_area_2, valor_3);
		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_area_3, valor_4);

		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setAtributo(AttComumReservatorio_cota_referencia, a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getCotaMedia(a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_minimo, double()), a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_maximo, double())));

		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoCOTAREA(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoCOTAREA(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE){

bool LeituraCEPEL::aplicarModificacaoCOTARE(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const std::vector<Periodo>& a_lista_periodo, const std::vector<std::vector<double>>& a_lista_valor, const std::vector<TipoGrandezaModificacao>& a_lista_tipo) {
	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const double poli_cota_area_0 = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_poli_cota_area_0, double());
		const double poli_cota_area_1 = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_poli_cota_area_1, double());
		const double poli_cota_area_2 = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_poli_cota_area_2, double());
		const double poli_cota_area_3 = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_poli_cota_area_3, double());
		const double poli_cota_area_4 = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_poli_cota_area_4, double());

		SmartEnupla<Periodo, double> cota_area_0_DECK(horizonte_estudo_DECK, poli_cota_area_0);
		SmartEnupla<Periodo, double> cota_area_1_DECK(horizonte_estudo_DECK, poli_cota_area_1);
		SmartEnupla<Periodo, double> cota_area_2_DECK(horizonte_estudo_DECK, poli_cota_area_2);
		SmartEnupla<Periodo, double> cota_area_3_DECK(horizonte_estudo_DECK, poli_cota_area_3);
		SmartEnupla<Periodo, double> cota_area_4_DECK(horizonte_estudo_DECK, poli_cota_area_4);

		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_area_0, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_area_1, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_area_2, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_area_3, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_area_4, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

		for (int i = 0; i < int(a_lista_periodo.size()); i++) {

			const Periodo periodo_modificacao = a_lista_periodo.at(i);

			const TipoGrandezaModificacao tipo_de_grandeza = a_lista_tipo.at(i);

			if (tipo_de_grandeza == TipoGrandezaModificacao_absoluta) {
				for (Periodo periodo = horizonte_estudo_DECK.getIteradorInicial(); periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo)) {
					
					if (periodo >= periodo_modificacao) {

						cota_area_0_DECK.at(periodo) = a_lista_valor.at(1).at(i);
						cota_area_1_DECK.at(periodo) = a_lista_valor.at(2).at(i);
						cota_area_2_DECK.at(periodo) = a_lista_valor.at(3).at(i);
						cota_area_3_DECK.at(periodo) = a_lista_valor.at(4).at(i);
						cota_area_4_DECK.at(periodo) = a_lista_valor.at(5).at(i);

					}//if (periodo >= periodo_modificacao) {

				} // for (Periodo periodo = horizonte_estudo_DECK.getIteradorInicial(); periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo)) {
			} // if (tipo_de_grandeza == TipoGrandezaModificacao_absoluta) {

			else if (tipo_de_grandeza == TipoGrandezaModificacao_percentual)
				throw std::invalid_argument("TipoGrandezaModificacao_percentual nao compativel com modificacao.");

		} // for (int i = 0; i < int(a_lista_periodo.size()); i++) {

		double sobreposicao_atraso_periodo_inicial = 0.0;

		if (desconsiderar_atraso_periodo_estudo_inicial)
			sobreposicao_atraso_periodo_inicial = horizonte_estudo.getIteradorInicial().atraso(horizonte_estudo_DECK.getIteradorInicial());

		for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo_DECK)) {

			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				double sobreposicao = periodo.sobreposicao(periodo_DECK);

				if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
					sobreposicao += sobreposicao_atraso_periodo_inicial;

				if (sobreposicao > 0.0) {

					const double valor_antigo_0 = a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_area_0, periodo, double());
					const double valor_antigo_1 = a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_area_1, periodo, double());
					const double valor_antigo_2 = a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_area_2, periodo, double());
					const double valor_antigo_3 = a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_area_3, periodo, double());
					const double valor_antigo_4 = a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_area_4, periodo, double());

					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_poli_cota_area_0, periodo, valor_antigo_0 + sobreposicao * cota_area_0_DECK.at(periodo_DECK));
					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_poli_cota_area_1, periodo, valor_antigo_1 + sobreposicao * cota_area_1_DECK.at(periodo_DECK));
					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_poli_cota_area_2, periodo, valor_antigo_2 + sobreposicao * cota_area_2_DECK.at(periodo_DECK));
					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_poli_cota_area_3, periodo, valor_antigo_3 + sobreposicao * cota_area_3_DECK.at(periodo_DECK));
					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_poli_cota_area_4, periodo, valor_antigo_4 + sobreposicao * cota_area_4_DECK.at(periodo_DECK));

				} // if ((periodo >= a_modificacaoUHE.periodo) || (sobreposicao > 0.0)) {

			} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

		} // for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo_DECK)) {

		return true;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoCOTARE(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoCOTARE(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE){


bool LeituraCEPEL::aplicarModificacaoCMONT(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const std::vector<Periodo>& a_lista_periodo, const std::vector<std::vector<double>>& a_lista_valor, const std::vector<TipoGrandezaModificacao>& a_lista_tipo) {

	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const double cota_referencia = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_cota_referencia, double());

		SmartEnupla<Periodo, double> cota_referencia_DECK(horizonte_estudo_DECK, cota_referencia);

		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_cota_referencia, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

		for (int i = 0; i < int(a_lista_periodo.size()); i++) {

			const Periodo periodo_modificacao = a_lista_periodo.at(i);

			const TipoGrandezaModificacao tipo_de_grandeza = a_lista_tipo.at(i);

			const double valor_modificacao = a_lista_valor.at(1).at(i);

			if (valor_modificacao < 0)
				throw std::invalid_argument("Valor Negativo Modificacao.");

			if (tipo_de_grandeza == TipoGrandezaModificacao_absoluta) {
				for (Periodo periodo = horizonte_estudo_DECK.getIteradorInicial(); periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo)) {
					if (periodo >= periodo_modificacao)
						cota_referencia_DECK.at(periodo) = valor_modificacao;
				}
					
			}

			else if (tipo_de_grandeza == TipoGrandezaModificacao_percentual)
				throw std::invalid_argument("TipoGrandezaModificacao_percentual nao compativel com modificacao.");

		} // for (int i = 0; i < int(a_lista_periodo.size()); i++) {

		double sobreposicao_atraso_periodo_inicial = 0.0;

		if (desconsiderar_atraso_periodo_estudo_inicial)
			sobreposicao_atraso_periodo_inicial = horizonte_estudo.getIteradorInicial().atraso(horizonte_estudo_DECK.getIteradorInicial());

		for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo_DECK)) {

			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				double sobreposicao = periodo.sobreposicao(periodo_DECK);

				if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
					sobreposicao += sobreposicao_atraso_periodo_inicial;

				if (sobreposicao > 0.0) {

					const double valor_antigo = a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_cota_referencia, periodo, double());

					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_cota_referencia, periodo, valor_antigo + sobreposicao * cota_referencia_DECK.at(periodo_DECK));

				} // if ((periodo >= a_modificacaoUHE.periodo) || (sobreposicao > 0.0)) {

			} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

		} // for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo_DECK)) {

		return true;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoCMONT(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }

} // bool LeituraCEPEL::aplicarModificacaoCMONT(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE){


bool LeituraCEPEL::aplicarModificacaoVOLCOTA(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_VOLCOTA)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const double valor_0 = a_modificacaoUHE.valor_1;
		const double valor_1 = a_modificacaoUHE.valor_2;
		const double valor_2 = a_modificacaoUHE.valor_3;
		const double valor_3 = a_modificacaoUHE.valor_4;
		const double valor_4 = a_modificacaoUHE.valor_5;

		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_volume_0, valor_0);
		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_volume_1, valor_1);
		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_volume_1, valor_2);
		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_volume_2, valor_3);
		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_volume_3, valor_4);
		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoVOLCOTA(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoVOLCOTA(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE){


bool LeituraCEPEL::aplicarModificacaoCOTVOL(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const std::vector<Periodo>& a_lista_periodo, const std::vector<std::vector<double>>& a_lista_valor, const std::vector<TipoGrandezaModificacao>& a_lista_tipo) {
	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const double poli_cota_volume_0 = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_poli_cota_volume_0, double());
		const double poli_cota_volume_1 = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_poli_cota_volume_1, double());
		const double poli_cota_volume_2 = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_poli_cota_volume_2, double());
		const double poli_cota_volume_3 = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_poli_cota_volume_3, double());
		const double poli_cota_volume_4 = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_poli_cota_volume_4, double());

		SmartEnupla<Periodo, double> cota_volume_0_DECK(horizonte_estudo_DECK, poli_cota_volume_0);
		SmartEnupla<Periodo, double> cota_volume_1_DECK(horizonte_estudo_DECK, poli_cota_volume_1);
		SmartEnupla<Periodo, double> cota_volume_2_DECK(horizonte_estudo_DECK, poli_cota_volume_2);
		SmartEnupla<Periodo, double> cota_volume_3_DECK(horizonte_estudo_DECK, poli_cota_volume_3);
		SmartEnupla<Periodo, double> cota_volume_4_DECK(horizonte_estudo_DECK, poli_cota_volume_4);

		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_volume_0, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_volume_1, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_volume_2, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_volume_3, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_volume_4, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

		for (int i = 0; i < int(a_lista_periodo.size()); i++) {

			const Periodo periodo_modificacao = a_lista_periodo.at(i);

			const TipoGrandezaModificacao tipo_de_grandeza = a_lista_tipo.at(i);

			if (tipo_de_grandeza == TipoGrandezaModificacao_absoluta) {
				for (Periodo periodo = horizonte_estudo_DECK.getIteradorInicial(); periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo)) {
					if (periodo >= periodo_modificacao) {
						cota_volume_0_DECK.at(periodo) = a_lista_valor.at(1).at(i);
						cota_volume_1_DECK.at(periodo) = a_lista_valor.at(2).at(i);
						cota_volume_2_DECK.at(periodo) = a_lista_valor.at(3).at(i);
						cota_volume_3_DECK.at(periodo) = a_lista_valor.at(4).at(i);
						cota_volume_4_DECK.at(periodo) = a_lista_valor.at(5).at(i);
					}
				} // for (Periodo periodo = horizonte_estudo_DECK.getIteradorInicial(); periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo))
			} // if (tipo_de_grandeza == TipoGrandezaModificacao_absoluta) {

			else if (tipo_de_grandeza == TipoGrandezaModificacao_percentual)
				throw std::invalid_argument("TipoGrandezaModificacao_percentual nao compativel com modificacao.");

		} // for (int i = 0; i < int(a_lista_periodo.size()); i++) {

		double sobreposicao_atraso_periodo_inicial = 0.0;

		if (desconsiderar_atraso_periodo_estudo_inicial)
			sobreposicao_atraso_periodo_inicial = horizonte_estudo.getIteradorInicial().atraso(horizonte_estudo_DECK.getIteradorInicial());

		for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo_DECK)) {

			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				double sobreposicao = periodo.sobreposicao(periodo_DECK);

				if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
					sobreposicao += sobreposicao_atraso_periodo_inicial;

				if (sobreposicao > 0.0) {

					const double valor_antigo_0 = a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_volume_0, periodo, double());
					const double valor_antigo_1 = a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_volume_1, periodo, double());
					const double valor_antigo_2 = a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_volume_2, periodo, double());
					const double valor_antigo_3 = a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_volume_3, periodo, double());
					const double valor_antigo_4 = a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_volume_4, periodo, double());

					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_poli_cota_volume_0, periodo, valor_antigo_0 + sobreposicao * cota_volume_0_DECK.at(periodo_DECK));
					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_poli_cota_volume_1, periodo, valor_antigo_1 + sobreposicao * cota_volume_1_DECK.at(periodo_DECK));
					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_poli_cota_volume_2, periodo, valor_antigo_2 + sobreposicao * cota_volume_2_DECK.at(periodo_DECK));
					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_poli_cota_volume_3, periodo, valor_antigo_3 + sobreposicao * cota_volume_3_DECK.at(periodo_DECK));
					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_poli_cota_volume_4, periodo, valor_antigo_4 + sobreposicao * cota_volume_4_DECK.at(periodo_DECK));

				} // if ((periodo >= a_modificacaoUHE.periodo) || (sobreposicao > 0.0)) {

			} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

		} // for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo_DECK)) {

		return true;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoCOTVOL(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoCOTVOL(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE){


bool LeituraCEPEL::aplicarModificacaoPRODESP(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_PRODESP)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const double fator_de_producao = a_modificacaoUHE.valor_1;

		for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {
			for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++)
				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidadeUHE).setAtributo(AttComumUnidadeUHE_fator_de_producao, fator_de_producao);
		}

		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoPRODESP(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoPRODESP(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE){

bool LeituraCEPEL::aplicarModificacaoCOEFEVAP(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_COEFEVAP)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const double evaporacao = a_modificacaoUHE.valor_1;
		const IdMes mes = getIdMesFromInt(int(a_modificacaoUHE.valor_2));

		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_evaporacao, mes, evaporacao);
		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoCOEFEVAP(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoCOEFEVAP(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE){

bool LeituraCEPEL::aplicarModificacaoPERDHIDR(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_PERDHIDR)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const double perda_hidraulica = a_modificacaoUHE.valor_1;
		const TipoGrandezaModificacao tipo_de_grandeza = a_modificacaoUHE.tipo_de_grandeza;

		for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {
			for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {

				if (tipo_de_grandeza == TipoGrandezaModificacao_percentual) {
					if (perda_hidraulica > 1.0)
						throw std::invalid_argument("Perda hidraulica maior que 1.0 (100%).");

					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidadeUHE).setAtributo(AttComumUnidadeUHE_tipo_de_perda_hidraulica, TipoPerdaHidraulica_percentual);

				} // if (tipo_de_grandeza == TipoGrandezaModificacao_percentual) {

				else if (tipo_de_grandeza == TipoGrandezaModificacao_absoluta)
					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidadeUHE).setAtributo(AttComumUnidadeUHE_tipo_de_perda_hidraulica, TipoPerdaHidraulica_metro);

				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidadeUHE).setAtributo(AttComumUnidadeUHE_perda_hidraulica, perda_hidraulica);

			} // for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {
		} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoPERDHIDR(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoPERDHIDR(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE){

bool LeituraCEPEL::aplicarModificacaoCFUGA(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const std::vector<Periodo>& a_lista_periodo, const std::vector<std::vector<double>>& a_lista_valor, const std::vector<TipoGrandezaModificacao>& a_lista_tipo) {
	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const double canal_fuga_medio = a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_canal_fuga_medio, double());

		SmartEnupla<Periodo, double> canal_fuga_medio_DECK(horizonte_estudo_DECK, canal_fuga_medio);

		a_dados.vetorHidreletrica.at(a_idHidreletrica).setVetor(AttVetorHidreletrica_canal_fuga_medio, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

		for (int i = 0; i < int(a_lista_periodo.size()); i++) {

			Periodo periodo_modificacao = a_lista_periodo.at(i);

			if (periodo_modificacao < horizonte_estudo_DECK.getIteradorInicial())
				periodo_modificacao = horizonte_estudo_DECK.getIteradorInicial();


			const TipoGrandezaModificacao tipo_de_grandeza = a_lista_tipo.at(i);

			const double valor_modificacao = a_lista_valor.at(1).at(i);

			if (valor_modificacao < 0)
				throw std::invalid_argument("Valor Negativo Modificacao.");

			if (tipo_de_grandeza == TipoGrandezaModificacao_absoluta) {

				for (Periodo periodo = horizonte_estudo_DECK.getIteradorInicial(); periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo)) {

					if (periodo >= periodo_modificacao)
						canal_fuga_medio_DECK.at(periodo) = valor_modificacao;

				}//for (Periodo periodo = horizonte_estudo_DECK.getIteradorInicial(); periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo)) {

			}//if (tipo_de_grandeza == TipoGrandezaModificacao_absoluta) {

			else if (tipo_de_grandeza == TipoGrandezaModificacao_percentual)
				throw std::invalid_argument("TipoGrandezaModificacao_percentual nao compativel com modificacao.");

		} // for (int i = 0; i < int(a_lista_periodo.size()); i++) {

		double sobreposicao_atraso_periodo_inicial = 0.0;

		if (desconsiderar_atraso_periodo_estudo_inicial)
			sobreposicao_atraso_periodo_inicial = horizonte_estudo.getIteradorInicial().atraso(horizonte_estudo_DECK.getIteradorInicial());

		for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo_DECK)) {

			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				double sobreposicao = periodo.sobreposicao(periodo_DECK);

				if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
					sobreposicao += sobreposicao_atraso_periodo_inicial;

				if (sobreposicao > 0.0) {

					const double valor_antigo = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_canal_fuga_medio, periodo, double());
					a_dados.vetorHidreletrica.at(a_idHidreletrica).setElemento(AttVetorHidreletrica_canal_fuga_medio, periodo, valor_antigo + sobreposicao * canal_fuga_medio_DECK.at(periodo_DECK));

				} // if ((periodo >= a_modificacaoUHE.periodo) || (sobreposicao > 0.0)) {

			} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

		} // for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo_DECK)) {

		return true;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoCFUGA(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoCFUGA(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE, const IdEstagio a_estagio_final){

bool LeituraCEPEL::aplicarModificacaoVAZMINT(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const std::vector<Periodo>& a_lista_periodo, const std::vector<std::vector<double>>& a_lista_valor, const std::vector<TipoGrandezaModificacao>& a_lista_tipo) {
	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const double vazao_defluente_minima = a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_vazao_defluente_minima, double());

		SmartEnupla<Periodo, double> vazao_defluente_minima_DECK(horizonte_estudo_DECK, vazao_defluente_minima);

		a_dados.vetorHidreletrica.at(a_idHidreletrica).setVetor(AttVetorHidreletrica_vazao_defluente_minima, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

		for (int i = 0; i < int(a_lista_periodo.size()); i++) {

			const Periodo periodo_modificacao = a_lista_periodo.at(i);

			const TipoGrandezaModificacao tipo_de_grandeza = a_lista_tipo.at(i);

			const double valor_modificacao = a_lista_valor.at(1).at(i);

			if (valor_modificacao < 0)
				throw std::invalid_argument("Valor Negativo Modificacao.");

			if (tipo_de_grandeza == TipoGrandezaModificacao_absoluta) {
				for (Periodo periodo = horizonte_estudo_DECK.getIteradorInicial(); periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo)) {
					if (periodo >= periodo_modificacao)
						vazao_defluente_minima_DECK.at(periodo) = valor_modificacao;
				}
					
			}

			else if (tipo_de_grandeza == TipoGrandezaModificacao_percentual)
				throw std::invalid_argument("TipoGrandezaModificacao_percentual nao compativel com modificacao.");

		} // for (int i = 0; i < int(a_lista_periodo.size()); i++) {

		double sobreposicao_atraso_periodo_inicial = 0.0;

		if (desconsiderar_atraso_periodo_estudo_inicial)
			sobreposicao_atraso_periodo_inicial = horizonte_estudo.getIteradorInicial().atraso(horizonte_estudo_DECK.getIteradorInicial());

		for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo_DECK)) {

			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				double sobreposicao = periodo.sobreposicao(periodo_DECK);

				if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
					sobreposicao += sobreposicao_atraso_periodo_inicial;

				if (sobreposicao > 0.0) {

					const double valor_antigo = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_vazao_defluente_minima, periodo, double());

					a_dados.vetorHidreletrica.at(a_idHidreletrica).setElemento(AttVetorHidreletrica_vazao_defluente_minima, periodo, valor_antigo + sobreposicao * vazao_defluente_minima_DECK.at(periodo_DECK));

				} // if ((periodo >= a_modificacaoUHE.periodo) || (sobreposicao > 0.0)) {

			} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

		} // for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo_DECK)) {

		return true;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoVAZMINT(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoVAZMINT(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE, const SmartEnupla<Periodo, IdEstagio>& a_horizonte_estudo){

bool LeituraCEPEL::aplicarModificacaoVMAXT(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const std::vector<Periodo>& a_lista_periodo, const std::vector<std::vector<double>>& a_lista_valor, const std::vector<TipoGrandezaModificacao>& a_lista_tipo) {
	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const double volume_maximo = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_maximo, double());

		SmartEnupla<Periodo, double> volume_maximo_DECK(horizonte_estudo_DECK, volume_maximo);

		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_volume_maximo, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

		for (int i = 0; i < int(a_lista_periodo.size()); i++) {

			const Periodo periodo_modificacao = a_lista_periodo.at(i);

			const TipoGrandezaModificacao tipo_de_grandeza = a_lista_tipo.at(i);

			const double valor_modificacao = a_lista_valor.at(1).at(i);

			if (valor_modificacao < 0)
				throw std::invalid_argument("Valor Negativo Modificacao.");

			if (tipo_de_grandeza == TipoGrandezaModificacao_absoluta) {
				for (Periodo periodo = horizonte_estudo_DECK.getIteradorInicial(); periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo)) {
					if (periodo >= periodo_modificacao)
						volume_maximo_DECK.at(periodo) = valor_modificacao;
				}
					
			}

			else if (tipo_de_grandeza == TipoGrandezaModificacao_percentual) {

				if (valor_modificacao > 1.0)
					throw std::invalid_argument("Modificacao de volume maior que 1.0 (100%).");

				const double volume_minimo = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_minimo, double());

				const double volume_util = volume_maximo - volume_minimo;

				for (Periodo periodo = horizonte_estudo_DECK.getIteradorInicial(); periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo)) {
					if (periodo >= periodo_modificacao)
						volume_maximo_DECK.at(periodo) = volume_minimo + valor_modificacao * volume_util;
				}
					

			} // else if (tipo_de_grandeza == TipoGrandezaModificacao_percentual) {

		} // for (int i = 0; i < int(a_lista_periodo.size()); i++) {

		double sobreposicao_atraso_periodo_inicial = 0.0;

		if (desconsiderar_atraso_periodo_estudo_inicial)
			sobreposicao_atraso_periodo_inicial = horizonte_estudo.getIteradorInicial().atraso(horizonte_estudo_DECK.getIteradorInicial());

		for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo_DECK)) {

			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				double sobreposicao = periodo.sobreposicao(periodo_DECK);

				if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
					sobreposicao += sobreposicao_atraso_periodo_inicial;

				if (sobreposicao > 0.0) {

					const double valor_antigo = a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_maximo, periodo, double());

					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_maximo, periodo, valor_antigo + sobreposicao * volume_maximo_DECK.at(periodo_DECK));

				} // if ((periodo >= a_modificacaoUHE.periodo) || (sobreposicao > 0.0)) {

			} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

		} // for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo_DECK)) {

		return true;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoVMAXT(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoVMAXT(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE, const SmartEnupla<Periodo, IdEstagio>& a_horizonte_estudo){

bool LeituraCEPEL::aplicarModificacaoVMINT(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const std::vector<Periodo>& a_lista_periodo, const std::vector<std::vector<double>>& a_lista_valor, const std::vector<TipoGrandezaModificacao>& a_lista_tipo) {
	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const double volume_minimo = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_minimo, double());

		SmartEnupla<Periodo, double> volume_minimo_DECK(horizonte_estudo_DECK, volume_minimo);

		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_volume_minimo, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

		for (int i = 0; i < int(a_lista_periodo.size()); i++) {

			Periodo periodo_modificacao = a_lista_periodo.at(i);
			if (periodo_modificacao < horizonte_estudo_DECK.getIteradorInicial())
				periodo_modificacao = horizonte_estudo_DECK.getIteradorInicial();

			const TipoGrandezaModificacao tipo_de_grandeza = a_lista_tipo.at(i);

			const double valor_modificacao = a_lista_valor.at(1).at(i);

			if (valor_modificacao < 0)
				throw std::invalid_argument("Valor Negativo Modificacao.");

			if (tipo_de_grandeza == TipoGrandezaModificacao_absoluta) {
				for (Periodo periodo = horizonte_estudo_DECK.getIteradorInicial(); periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo)) {
					if (periodo >= periodo_modificacao)
						volume_minimo_DECK.at(periodo) = valor_modificacao;
				}
					
			}

			else if (tipo_de_grandeza == TipoGrandezaModificacao_percentual) {

				if (valor_modificacao > 1.0)
					throw std::invalid_argument("Modificacao de volume maior que 1.0 (100%).");

				const double volume_maximo = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_maximo, double());

				const double volume_util = volume_maximo - volume_minimo;

				for (Periodo periodo = horizonte_estudo_DECK.getIteradorInicial(); periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo)) {
					if (periodo >= periodo_modificacao)
						volume_minimo_DECK.at(periodo) = volume_minimo + valor_modificacao * volume_util;
				}
					

			} // else if (tipo_de_grandeza == TipoGrandezaModificacao_percentual) {

		} // for (int i = 0; i < int(a_lista_periodo.size()); i++) {

		double sobreposicao_atraso_periodo_inicial = 0.0;

		if (desconsiderar_atraso_periodo_estudo_inicial)
			sobreposicao_atraso_periodo_inicial = horizonte_estudo.getIteradorInicial().atraso(horizonte_estudo_DECK.getIteradorInicial());

		for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo_DECK)) {

			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				double sobreposicao = periodo.sobreposicao(periodo_DECK);

				if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
					sobreposicao += sobreposicao_atraso_periodo_inicial;

				if (sobreposicao > 0.0) {

					const double valor_antigo = a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo, periodo, double());

					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_minimo, periodo, valor_antigo + sobreposicao * volume_minimo_DECK.at(periodo_DECK));

				} // if ((periodo >= a_modificacaoUHE.periodo) || (sobreposicao > 0.0)) {

			} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

		} // for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo_DECK)) {

		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoVMINT(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoVMINT(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE, const SmartEnupla<Periodo, IdEstagio>& a_horizonte_estudo){


bool LeituraCEPEL::aplicarModificacaoVMINP(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const std::vector<Periodo>& a_lista_periodo, const std::vector<std::vector<double>>& a_lista_valor, const std::vector<TipoGrandezaModificacao>& a_lista_tipo) {
	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const double volume_util_minimo = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_util_minimo, double());
		const double percentual_volume_util_minimo = a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_percentual_volume_util_minimo, double());

		SmartEnupla<Periodo, double> volume_util_minimo_DECK;
		SmartEnupla<Periodo, double> percentual_volume_util_minimo_DECK;

		for (int i = 0; i < int(a_lista_periodo.size()); i++) {

			Periodo periodo_modificacao = a_lista_periodo.at(i);
			if (periodo_modificacao < horizonte_estudo_DECK.getIteradorInicial())
				periodo_modificacao = horizonte_estudo_DECK.getIteradorInicial();

			const TipoGrandezaModificacao tipo_de_grandeza = a_lista_tipo.at(i);

			const double valor_modificacao = a_lista_valor.at(1).at(i);

			if (valor_modificacao < 0)
				throw std::invalid_argument("Valor Negativo Modificacao.");

			if (tipo_de_grandeza == TipoGrandezaModificacao_absoluta) {

				if (volume_util_minimo_DECK.size() == 0) {
					volume_util_minimo_DECK = SmartEnupla<Periodo, double>(horizonte_estudo_DECK, volume_util_minimo);
					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_volume_util_minimo, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
				}

				for (Periodo periodo = horizonte_estudo_DECK.getIteradorInicial(); periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo)) {
					if (periodo >= periodo_modificacao)
						volume_util_minimo_DECK.at(periodo) = valor_modificacao;
				}
					
			}

			else if (tipo_de_grandeza == TipoGrandezaModificacao_percentual) {

				if (percentual_volume_util_minimo_DECK.size() == 0) {
					percentual_volume_util_minimo_DECK = SmartEnupla<Periodo, double>(horizonte_estudo_DECK, percentual_volume_util_minimo);
					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_percentual_volume_util_minimo, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
				}

				if (valor_modificacao > 1.0)
					throw std::invalid_argument("Modificacao de volume maior que 1.0 (100%).");

				for (Periodo periodo = horizonte_estudo_DECK.getIteradorInicial(); periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo)) {
					if (periodo >= periodo_modificacao)
						percentual_volume_util_minimo_DECK.at(periodo) = valor_modificacao;
				}
					

			} // else if (tipo_de_grandeza == TipoGrandezaModificacao_percentual) {

		} // for (int i = 0; i < int(a_lista_periodo.size()); i++) {

		double sobreposicao_atraso_periodo_inicial = 0.0;

		if (desconsiderar_atraso_periodo_estudo_inicial)
			sobreposicao_atraso_periodo_inicial = horizonte_estudo.getIteradorInicial().atraso(horizonte_estudo_DECK.getIteradorInicial());

		for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo_DECK)) {

			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				double sobreposicao = periodo.sobreposicao(periodo_DECK);

				if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
					sobreposicao += sobreposicao_atraso_periodo_inicial;

				if (sobreposicao > 0.0) {

					if (volume_util_minimo_DECK.size() > 0) {

						const double valor_antigo = a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_minimo, periodo, double());

						a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_util_minimo, periodo, valor_antigo + sobreposicao * volume_util_minimo_DECK.at(periodo_DECK));

					}

					if (percentual_volume_util_minimo_DECK.size() > 0) {

						const double valor_antigo = a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_percentual_volume_util_minimo, periodo, double());

						a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_percentual_volume_util_minimo, periodo, valor_antigo + sobreposicao * percentual_volume_util_minimo_DECK.at(periodo_DECK));

					}

				} // if ((periodo >= a_modificacaoUHE.periodo) || (sobreposicao > 0.0)) {

			} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

		} // for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo_DECK)) {

		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoVMINP(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoVMINT(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE, const SmartEnupla<Periodo, IdEstagio>& a_horizonte_estudo){

bool LeituraCEPEL::aplicarModificacaoVMORTO(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_VMORTO)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_enchendo_volume_morto = a_modificacaoUHE.periodo;

		const Periodo periodo_volume_morto_completo = periodo_enchendo_volume_morto + int(a_modificacaoUHE.valor_1);

		for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

			if (periodo.sobreposicao(periodo_enchendo_volume_morto)) {
				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_morto_completo, periodo, 0);
				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_enchendo_volume_morto, periodo, 1);
			}

			else if (periodo < periodo_enchendo_volume_morto) {
				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_morto_completo, periodo, 0);
				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_enchendo_volume_morto, periodo, 0);
			} // if (periodo < periodo_enchendo_volume_morto) {

			else if (periodo < periodo_volume_morto_completo) {
				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_morto_completo, periodo, 0);
				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_enchendo_volume_morto, periodo, 1);
			} // else if ((periodo_enchendo_volume_morto <= periodo) && (periodo < periodo_volume_morto_completo)) {

			else {
				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_morto_completo, periodo, 1);
				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setElemento(AttVetorReservatorio_enchendo_volume_morto, periodo, 0);
			} // else {

		} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoVMORTO(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoVMORTO(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE, const SmartEnupla<Periodo, IdEstagio>& a_horizonte_estudo){

bool LeituraCEPEL::aplicarModificacaoNOVAMAQ(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {

		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_NOVAMAQ)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const double potencia_maxima_maquina = a_modificacaoUHE.valor_1;

		const IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico(int(a_modificacaoUHE.valor_3));

		const IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE(int(a_modificacaoUHE.valor_2));

		IdUnidadeUHE maiorIdUnidadeUHE = a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE());

		if (maiorIdUnidadeUHE == IdUnidadeUHE_Nenhum)
			throw std::invalid_argument("Nao foi possivel adicionar nova maquina em " + getFullString(idConjuntoHidraulico) + " pois nao ha maquinas.");

		if (idConjuntoHidraulico > a_dados.getMaiorId(a_idHidreletrica, IdConjuntoHidraulico()))
			throw std::invalid_argument("Conjunto hidraulico " + getFullString(idConjuntoHidraulico) + " nao considerado no estudo.");

		if (idUnidadeUHE > IdUnidadeUHE(maiorIdUnidadeUHE + 1))
			throw std::invalid_argument("Unidade " + getFullString(idUnidadeUHE) + " nao sequencial a maior unidade " + getFullString(maiorIdUnidadeUHE) + "do conjunto " + getFullString(idConjuntoHidraulico));

		double potencia_maxima_instanciada = a_dados.getAtributo(a_idHidreletrica, idConjuntoHidraulico, maiorIdUnidadeUHE, AttComumUnidadeUHE_potencia_maxima, double());

		if (idUnidadeUHE <= maiorIdUnidadeUHE) {
			if (!doubleCompara(0.01, a_dados.getAtributo(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttComumUnidadeUHE_potencia_maxima, double()), potencia_maxima_maquina))
				throw std::invalid_argument("Potencia maxima da maquina de " + getString(potencia_maxima_maquina) + " MW nao compativel com " + getFullString(idConjuntoHidraulico) + ".");

			if (a_dados.getSizeVetor(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_disponibilidade) == 0)
				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidadeUHE).setVetor(AttVetorUnidadeUHE_disponibilidade, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

		}

		else {

			if (!doubleCompara(0.01, a_dados.getAtributo(a_idHidreletrica, idConjuntoHidraulico, maiorIdUnidadeUHE, AttComumUnidadeUHE_potencia_maxima, double()), potencia_maxima_maquina))
				throw std::invalid_argument("Potencia maxima da maquina de " + getString(potencia_maxima_maquina) + " MW nao compativel com " + getFullString(idConjuntoHidraulico) + ".");

			UnidadeUHE unidadeUHE(a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(maiorIdUnidadeUHE));
			unidadeUHE.setAtributo(AttComumUnidadeUHE_idUnidadeUHE, idUnidadeUHE);
			a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.add(unidadeUHE);
			a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidadeUHE).setVetor(AttVetorUnidadeUHE_disponibilidade, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

		}

		double sobreposicao_atraso_periodo_inicial = 0.0;

		if (desconsiderar_atraso_periodo_estudo_inicial)
			sobreposicao_atraso_periodo_inicial = horizonte_estudo.getIteradorInicial().atraso(horizonte_estudo_DECK.getIteradorInicial());

		for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

			double sobreposicao = periodo.sobreposicao(a_modificacaoUHE.periodo);

			if ((periodo == horizonte_estudo.getIteradorInicial()) && (Periodo(horizonte_estudo_DECK.getIteradorInicial().getTipoPeriodo(), a_modificacaoUHE.periodo) == horizonte_estudo_DECK.getIteradorInicial()))
				sobreposicao += sobreposicao_atraso_periodo_inicial;

			if ((periodo >= a_modificacaoUHE.periodo) || (sobreposicao > 0.0)) {

				if (sobreposicao == 0.0)
					sobreposicao = 1.0;

				const double indisponibilidade_forcada = a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_indisponibilidade_forcada, double());
				const double indisponibilidade_programada = a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_indisponibilidade_programada, double());
				const double disponibilidade = (1.0 - indisponibilidade_forcada) * (1.0 - indisponibilidade_programada);

				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE.at(idUnidadeUHE).setElemento(AttVetorUnidadeUHE_disponibilidade, periodo, sobreposicao * disponibilidade);

			} // if ((periodo >= a_modificacaoUHE.periodo) || (sobreposicao > 0.0)) {

		} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

		return true;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoNOVAMAQ(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE,a_horizonte_estudo): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoNOVAMAQ(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE, const SmartEnupla<Periodo, IdEstagio>& a_horizonte_estudo){

bool LeituraCEPEL::aplicarModificacaoNOMEUH(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_NOMEUH)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const std::string nome_hidreletrica = a_modificacaoUHE.valor_0;
		a_dados.vetorHidreletrica.at(a_idHidreletrica).setAtributo(AttComumHidreletrica_nome, nome_hidreletrica);
		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoNOMEUH(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoIP(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE) {

bool LeituraCEPEL::aplicarModificacaoNUMJUS(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {

		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_NUMJUS)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const int codigo_ONS_jusante = int(a_modificacaoUHE.valor_1);

		const IdHidreletrica idHidreletrica_jusante = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_ONS_jusante);

		if (idHidreletrica_jusante == IdHidreletrica_Nenhum)
			throw std::invalid_argument("Modificacao NUMJUS indica usina hidreletrica nao inicializada com codigo ONS_" + getString(codigo_ONS_jusante));

		if (!hidreletricasPreConfig_instanciadas)
			a_dados.vetorHidreletrica.at(a_idHidreletrica).setAtributo(AttComumHidreletrica_jusante, idHidreletrica_jusante);
		else if (hidreletricasPreConfig_instanciadas)
			lista_jusante_hidreletrica.setElemento(a_idHidreletrica, idHidreletrica_jusante);

		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoNUMJUS(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoIP(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE) {

bool LeituraCEPEL::aplicarModificacaoDESVIO(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_DESVIO)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		//Manual: DESVIO: Índice da usina de jusante para canal de desvio e limite da vazão no canal (m3/s) 
		//Filosofia: O modelo vai criar uma restrição desvio <= desvio_limite

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const int codigo_ONS_jusante_desvio = int(a_modificacaoUHE.valor_1);
		const double desvio_agua_maximo = a_modificacaoUHE.valor_2;

		const IdHidreletrica idHidreletrica_jusante_desvio = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_ONS_jusante_desvio);

		if (idHidreletrica_jusante_desvio == IdHidreletrica_Nenhum)
			throw std::invalid_argument("Modificacao DESVIO indica usina hidreletrica nao inicializada com codigo ONS_" + getString(codigo_ONS_jusante_desvio));

		a_dados.vetorHidreletrica.at(a_idHidreletrica).setAtributo(AttComumHidreletrica_jusante_desvio, idHidreletrica_jusante_desvio);

		if (hidreletricasPreConfig_instanciadas)
			lista_jusante_desvio_hidreletrica.setElemento(a_idHidreletrica, idHidreletrica_jusante_desvio);

		a_dados.vetorHidreletrica.at(a_idHidreletrica).setVetor(AttVetorHidreletrica_vazao_desviada_minima, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
		a_dados.vetorHidreletrica.at(a_idHidreletrica).setVetor(AttVetorHidreletrica_vazao_desviada_maxima, SmartEnupla<Periodo, double>(horizonte_estudo, desvio_agua_maximo));

		//////////////////////////////////////////////////////////////
		//Identifica o Intercambio_Hidraulico correspondente ao desvio
		//////////////////////////////////////////////////////////////
		/*
		std::vector<IdIntercambioHidraulico> idIntercambioHidraulico_inicializado = a_dados.vetorIntercambioHidraulico.getIdObjetos(AttComumIntercambioHidraulico_hidreletrica_origem, a_idHidreletrica);

		int idIntercambioHidraulico_inicializado_size = int(idIntercambioHidraulico_inicializado.size());

		IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico_Nenhum;

		for (int intercambioHidraulico_inicializado = 0; intercambioHidraulico_inicializado < idIntercambioHidraulico_inicializado_size; intercambioHidraulico_inicializado++) {

			if (a_dados.getAtributo(idIntercambioHidraulico_inicializado.at(intercambioHidraulico_inicializado), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica()) == idHidreletrica_jusante_desvio) {
				idIntercambioHidraulico = idIntercambioHidraulico_inicializado.at(intercambioHidraulico_inicializado);
				break;
			}//if (a_dados.getAtributo(idIntercambioHidraulico_inicializado.at(intercambioHidraulico_inicializado), AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica()) == idHidreletrica_jusante_desvio) {

		}//for (int intercambioHidraulico_inicializado = 0; intercambioHidraulico_inicializado < idIntercambioHidraulico_inicializado_size; intercambioHidraulico_inicializado++) {

		if (idIntercambioHidraulico == IdIntercambioHidraulico_Nenhum) {

			const IdIntercambioHidraulico idIntercambioHidraulico_novo = IdIntercambioHidraulico(a_dados.getMaiorId(IdIntercambioHidraulico()) + 1);

			IntercambioHidraulico intercambioHidraulico;
			intercambioHidraulico.setAtributo(AttComumIntercambioHidraulico_idIntercambioHidraulico, idIntercambioHidraulico_novo);

			a_dados.vetorIntercambioHidraulico.add(intercambioHidraulico);

			a_dados.vetorIntercambioHidraulico.at(idIntercambioHidraulico_novo).setAtributo(AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico_desvio);

			a_dados.vetorIntercambioHidraulico.at(idIntercambioHidraulico_novo).setAtributo(AttComumIntercambioHidraulico_hidreletrica_origem, a_idHidreletrica);
			a_dados.vetorIntercambioHidraulico.at(idIntercambioHidraulico_novo).setAtributo(AttComumIntercambioHidraulico_hidreletrica_destino, idHidreletrica_jusante_desvio);

			//Set desvio_agua_minimo e desvio_agua_maximo

			const SmartEnupla<Periodo, double> vetor_zero(horizonte_estudo, 0.0);
			const SmartEnupla<Periodo, double> desvio_agua_maximo(horizonte_estudo, desvio_agua_maximo);

			a_dados.vetorIntercambioHidraulico.at(idIntercambioHidraulico_novo).setVetor(AttVetorIntercambioHidraulico_desvio_agua_minimo, vetor_zero);
			a_dados.vetorIntercambioHidraulico.at(idIntercambioHidraulico_novo).setVetor(AttVetorIntercambioHidraulico_desvio_agua_maximo, desvio_agua_maximo);

		}//if (idIntercambioHidraulico == IdIntercambioHidraulico_Nenhum) {
		else {

			//Atualiza desvio_agua_maximo

			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
				a_dados.vetorIntercambioHidraulico.at(idIntercambioHidraulico).setElemento(AttVetorIntercambioHidraulico_desvio_agua_maximo, periodo, desvio_agua_maximo);

		}//else {
		*/

		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoDESVIO(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoIP(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE) {

bool LeituraCEPEL::aplicarModificacaoVAZEFE(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_VAZEFE)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const int numero_conjunto = int(a_modificacaoUHE.valor_1);
		const double turbinamento_maximo = a_modificacaoUHE.valor_2;

		//if(a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.getIdObjetos(IdConjuntoHidraulico(numero_conjunto), IdConjuntoHidraulico()).size() == 0)
			//throw std::invalid_argument("Modificacao VAZEFE indica Conjunto Hidraulico nao inicializado com IdConjuntoHidraulico" + getString(numero_conjunto));

		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorConjuntoHidraulico.at(IdConjuntoHidraulico(numero_conjunto)).setVetor(AttVetorConjuntoHidraulico_vazao_turbinada_maxima, SmartEnupla<Periodo, double>(horizonte_estudo, turbinamento_maximo));

		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoVAZEFE(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoIP(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE) {

bool LeituraCEPEL::aplicarModificacaoVMDESV(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_VMDESV)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const double volume_minimo_desvio = a_modificacaoUHE.valor_1;

		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setAtributo(AttComumReservatorio_volume_minimo_desvio, volume_minimo_desvio);

		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoVMDESV(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoIP(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE) {

bool LeituraCEPEL::aplicarModificacaoNUMPOS(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_NUMPOS)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const int codigo_posto = int(a_modificacaoUHE.valor_1);

		a_dados.vetorHidreletrica.at(a_idHidreletrica).setAtributo(AttComumHidreletrica_codigo_posto, codigo_posto);

		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoNUMPOS(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoIP(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE) {

bool LeituraCEPEL::aplicarModificacaoNPOSNW(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_NPOSNW)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		a_dados.vetorHidreletrica.at(a_idHidreletrica).setAtributo(AttComumHidreletrica_codigo_posto_acoplamento_ENA, int(a_modificacaoUHE.valor_1));

		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoNPOSNW(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoIP(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE) {

bool LeituraCEPEL::aplicarModificacaoNCHAVE(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {

		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_NCHAVE)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const int curva_chave = int(a_modificacaoUHE.valor_1);
		const double altura_ref = a_modificacaoUHE.valor_2;

		IdPolinomioJusante maiorIdPolinomioJusante = a_dados.getMaiorId(a_idHidreletrica, IdPolinomioJusante());

		//Testa se existe um polinomio com a altura_ref indicada

		IdPolinomioJusante idPolinomioJusante = IdPolinomioJusante_Nenhum;

		for (IdPolinomioJusante idPolinomioJusante_teste = IdPolinomioJusante_1; idPolinomioJusante_teste <= maiorIdPolinomioJusante; idPolinomioJusante_teste++) {

			if (altura_ref == a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorPolinomioJusante.at(idPolinomioJusante_teste).getElementoVetor(AttVetorPolinomioJusante_altura_ref, horizonte_estudo.getIteradorInicial(), double())) {

				idPolinomioJusante = idPolinomioJusante_teste;
				break;

			}//if (altura_ref == a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorPolinomioJusante.at(idPolinomioJusante_teste).getElementoVetor(AttVetorPolinomioJusante_altura_ref, horizonte_estudo.getIteradorInicial(), double())) {

		}//for (IdPolinomioJusante idPolinomioJusante_teste = IdPolinomioJusante_1; idPolinomioJusante_teste <= maiorIdPolinomioJusante; idPolinomioJusante_teste++) {

		//Cria um novo Polinomio Jusante se nao existir um com a altura_ref indicada

		if (idPolinomioJusante == IdPolinomioJusante_Nenhum) {

			PolinomioJusante polinomioJusante;

			idPolinomioJusante = IdPolinomioJusante(maiorIdPolinomioJusante + 1);

			polinomioJusante.setAtributo(AttComumPolinomioJusante_idPolinomioJusante, idPolinomioJusante);

			a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorPolinomioJusante.add(polinomioJusante);

			a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorPolinomioJusante.at(idPolinomioJusante).setAtributo(AttComumPolinomioJusante_nome, a_dados.vetorHidreletrica.at(a_idHidreletrica).getAtributo(AttComumHidreletrica_nome, std::string()));
			a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorPolinomioJusante.at(idPolinomioJusante).setVetor(AttVetorPolinomioJusante_altura_ref, SmartEnupla<Periodo, double>(horizonte_estudo, altura_ref));

		}//if (idPolinomioJusante == IdPolinomioJusante_Nenhum) {

		a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorPolinomioJusante.at(idPolinomioJusante).setAtributo(AttComumPolinomioJusante_curva_chave, curva_chave);

		return true;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoNCHAVE(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE,a_horizonte_estudo): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoNCHAVE(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE, const SmartEnupla<Periodo, IdEstagio>& a_horizonte_estudo){

bool LeituraCEPEL::aplicarModificacaoCOTVAZ(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {

		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_COTVAZ)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const int curva_chave = int(a_modificacaoUHE.valor_1);
		const int coeficiente = int(a_modificacaoUHE.valor_2);
		const double valor_coeficiente = a_modificacaoUHE.valor_3;

		IdPolinomioJusante maiorIdPolinomioJusante = a_dados.getMaiorId(a_idHidreletrica, IdPolinomioJusante());

		//Testa se existe um polinomio com a curva_chave indicada

		IdPolinomioJusante idPolinomioJusante = IdPolinomioJusante_Nenhum;

		for (IdPolinomioJusante idPolinomioJusante_teste = IdPolinomioJusante_1; idPolinomioJusante_teste <= maiorIdPolinomioJusante; idPolinomioJusante_teste++) {
			if (curva_chave == a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorPolinomioJusante.at(idPolinomioJusante_teste).getAtributo(AttComumPolinomioJusante_curva_chave, int())) {
				idPolinomioJusante = idPolinomioJusante_teste;
				break;
			}//if (curva_chave == a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorPolinomioJusante.at(idPolinomioJusante_teste).getAtributo(AttComumPolinomioJusante_curva_chave, double())) {
		}//for (IdPolinomioJusante idPolinomioJusante_teste = IdPolinomioJusante_1; idPolinomioJusante_teste <= maiorIdPolinomioJusante; idPolinomioJusante_teste++) {

		//Cria um novo Polinomio Jusante se nao existir um com a altura_ref indicada

		if (idPolinomioJusante == IdPolinomioJusante_Nenhum)
			throw std::invalid_argument("Nao existe polinomio jusante inicializado com curva chave_" + getString(curva_chave));

		if (a_modificacaoUHE.periodo == horizonte_estudo.getIteradorInicial()) {

			if (coeficiente == 1)
				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorPolinomioJusante.at(idPolinomioJusante).setVetor(AttVetorPolinomioJusante_coeficiente_0, SmartEnupla<Periodo, double>(horizonte_estudo, valor_coeficiente));
			else if (coeficiente == 2)
				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorPolinomioJusante.at(idPolinomioJusante).setVetor(AttVetorPolinomioJusante_coeficiente_1, SmartEnupla<Periodo, double>(horizonte_estudo, valor_coeficiente));
			else if (coeficiente == 3)
				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorPolinomioJusante.at(idPolinomioJusante).setVetor(AttVetorPolinomioJusante_coeficiente_2, SmartEnupla<Periodo, double>(horizonte_estudo, valor_coeficiente));
			else if (coeficiente == 4)
				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorPolinomioJusante.at(idPolinomioJusante).setVetor(AttVetorPolinomioJusante_coeficiente_3, SmartEnupla<Periodo, double>(horizonte_estudo, valor_coeficiente));
			else if (coeficiente == 5)
				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorPolinomioJusante.at(idPolinomioJusante).setVetor(AttVetorPolinomioJusante_coeficiente_4, SmartEnupla<Periodo, double>(horizonte_estudo, valor_coeficiente));
			else
				throw std::invalid_argument("Coeficiente nao identificado para o polinomio jusante inicializado com curva chave_" + getString(curva_chave));

		}//if (a_modificacaoUHE.periodo == horizonte_estudo.getIteradorInicial()) {
		else {

			for (Periodo periodo = a_modificacaoUHE.periodo; periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				if (coeficiente == 1)
					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorPolinomioJusante.at(idPolinomioJusante).setElemento(AttVetorPolinomioJusante_coeficiente_0, periodo, valor_coeficiente);
				else if (coeficiente == 2)
					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorPolinomioJusante.at(idPolinomioJusante).setElemento(AttVetorPolinomioJusante_coeficiente_1, periodo, valor_coeficiente);
				else if (coeficiente == 3)
					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorPolinomioJusante.at(idPolinomioJusante).setElemento(AttVetorPolinomioJusante_coeficiente_2, periodo, valor_coeficiente);
				else if (coeficiente == 4)
					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorPolinomioJusante.at(idPolinomioJusante).setElemento(AttVetorPolinomioJusante_coeficiente_3, periodo, valor_coeficiente);
				else if (coeficiente == 5)
					a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorPolinomioJusante.at(idPolinomioJusante).setElemento(AttVetorPolinomioJusante_coeficiente_4, periodo, valor_coeficiente);
				else
					throw std::invalid_argument("Coeficiente nao identificado para o polinomio jusante inicializado com curva chave_" + getString(curva_chave));

			}//for (Periodo periodo = a_modificacaoUHE.periodo; periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

		}//else {

		return true;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoCOTVAZ(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE,a_horizonte_estudo): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoCOTVAZ(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE, const SmartEnupla<Periodo, IdEstagio>& a_horizonte_estudo){


bool LeituraCEPEL::aplicarModificacaoVERTJU(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {

		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_VERTJU)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const int vertimento_influencia_fcj = int(a_modificacaoUHE.valor_1);

		if (vertimento_influencia_fcj == 1)
			a_dados.vetorHidreletrica.at(a_idHidreletrica).setAtributo(AttComumHidreletrica_vertimento_influencia_fcj, true);
		else if (vertimento_influencia_fcj == 0)
			a_dados.vetorHidreletrica.at(a_idHidreletrica).setAtributo(AttComumHidreletrica_vertimento_influencia_fcj, false);

		return true;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoVERTJU(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE,a_horizonte_estudo): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoVERTJU(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE, const SmartEnupla<Periodo, IdEstagio>& a_horizonte_estudo){

bool LeituraCEPEL::aplicarModificacaoJUSENA(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {

		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_JUSENA)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const int codigo_usina_JUSENA = int(a_modificacaoUHE.valor_1);
		const IdHidreletrica idHidreletricaJusante_JUSENA = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_usina_JUSENA);

		a_dados.vetorHidreletrica.at(a_idHidreletrica).setAtributo(AttComumHidreletrica_jusante_JUSENA, idHidreletricaJusante_JUSENA);

		return true;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoJUSENA(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE,a_horizonte_estudo): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoJUSENA(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE, const SmartEnupla<Periodo, IdEstagio>& a_horizonte_estudo){


void LeituraCEPEL::aplicarModificacoesUTE(Dados& a_dados) {
	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		double sobreposicao_atraso_periodo_inicial = 0.0;

		if (desconsiderar_atraso_periodo_estudo_inicial)
			sobreposicao_atraso_periodo_inicial = horizonte_estudo.getIteradorInicial().atraso(horizonte_estudo_DECK.getIteradorInicial());

		const IdTermeletrica	idTermeletricaIni = a_dados.getMenorId(IdTermeletrica());
		const IdTermeletrica	idTermeletricaOut = a_dados.getIdOut(IdTermeletrica());
		for (IdTermeletrica idTermeletrica = idTermeletricaIni; idTermeletrica < idTermeletricaOut; a_dados.vetorTermeletrica.incr(idTermeletrica)) {

			//
			// Seta elementos em AttVetores de Termeletrica com valores de AttComum ModificacaoUTE
			//

			if (lista_modificacaoUTE.at(idTermeletrica).size() > 0) {

				SmartEnupla<Periodo, double> lista_IPTER;
				SmartEnupla<Periodo, double> lista_TEIFT;
				SmartEnupla<Periodo, double> lista_FCMAX;
				SmartEnupla<Periodo, double> lista_POTEF;
				SmartEnupla<Periodo, double> lista_GTMIN;

				for (int i = 0; i < lista_modificacaoUTE.at(idTermeletrica).size(); i++) {

					// Inicializa listas em base de periodo do DECK					

					if ((lista_modificacaoUTE.at(idTermeletrica).at(i).tipo_de_modificacao == TipoModificacaoUTE_IPTER) && (lista_IPTER.size() == 0)) {
						lista_IPTER = SmartEnupla<Periodo, double>(horizonte_estudo_DECK, 0.0);

						const Periodo periodo_pos_manutencao_programada = Periodo(IdMes_1, IdAno(horizonte_estudo_DECK.getIteradorInicial().getAno() + numero_anos_manutencao_termica_programada));

						for (Periodo periodo = periodo_pos_manutencao_programada; periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo))
							lista_IPTER.at(periodo) = a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_indisponibilidade_programada, double());

						a_dados.vetorTermeletrica.at(idTermeletrica).setVetor(AttVetorTermeletrica_indisponibilidade_programada, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

					} // if (lista_modificacaoUTE.at(idTermeletrica).at(i).tipo_de_modificacao == TipoModificacaoUTE_IPTER) {

					else if ((lista_modificacaoUTE.at(idTermeletrica).at(i).tipo_de_modificacao == TipoModificacaoUTE_TEIFT) && (lista_TEIFT.size() == 0)) {
						lista_TEIFT = SmartEnupla<Periodo, double>(horizonte_estudo_DECK, a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_indisponibilidade_forcada, double()));

						a_dados.vetorTermeletrica.at(idTermeletrica).setVetor(AttVetorTermeletrica_indisponibilidade_forcada, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
					}

					else if ((lista_modificacaoUTE.at(idTermeletrica).at(i).tipo_de_modificacao == TipoModificacaoUTE_FCMAX) && (lista_FCMAX.size() == 0)) {
						lista_FCMAX = SmartEnupla<Periodo, double>(horizonte_estudo_DECK, a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_fator_de_capacidade, double()));

						a_dados.vetorTermeletrica.at(idTermeletrica).setVetor(AttVetorTermeletrica_fator_de_capacidade, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
					}

					else if ((lista_modificacaoUTE.at(idTermeletrica).at(i).tipo_de_modificacao == TipoModificacaoUTE_POTEF) && (lista_POTEF.size() == 0)) {
						lista_POTEF = SmartEnupla<Periodo, double>(horizonte_estudo_DECK, a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_potencia_maxima, double()));

						a_dados.vetorTermeletrica.at(idTermeletrica).vetorUnidadeUTE.at(IdUnidadeUTE_1).setMatriz_forced(AttMatrizUnidadeUTE_potencia_maxima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, horizonte_estudo.getIteradorInicial(), IdPatamarCarga()), 0.0))));
					}

					else if ((lista_modificacaoUTE.at(idTermeletrica).at(i).tipo_de_modificacao == TipoModificacaoUTE_GTMIN) && (lista_GTMIN.size() == 0)) {
						lista_GTMIN = SmartEnupla<Periodo, double>(horizonte_estudo_DECK, 0.0);

						for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); periodo_DECK++) {

								const double sobreposicao = periodo_DECK.sobreposicao(periodo);

								if (sobreposicao > 0.0)
									lista_GTMIN.at(periodo_DECK) += sobreposicao * a_dados.getElementoMatriz(idTermeletrica, IdUnidadeUTE_1, AttMatrizUnidadeUTE_potencia_minima, periodo, IdPatamarCarga_1, double());

								if ((periodo_DECK > periodo) && (sobreposicao == 0.0))
									break;

							} // for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); periodo_DECK++) {

						} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						a_dados.vetorTermeletrica.at(idTermeletrica).vetorUnidadeUTE.at(IdUnidadeUTE_1).setMatriz_forced(AttMatrizUnidadeUTE_potencia_minima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, horizonte_estudo.getIteradorInicial(), IdPatamarCarga()), 0.0))));

					} // else if (lista_modificacaoUTE.at(idTermeletrica).at(i).tipo_de_modificacao == TipoModificacaoUTE_GTMIN) {

					//
					// Atualiza listas em base de periodo do DECK com valores das modificações
					//

					for (Periodo periodo_DECK = lista_modificacaoUTE.at(idTermeletrica).at(i).periodo_inicial; periodo_DECK <= lista_modificacaoUTE.at(idTermeletrica).at(i).periodo_final; periodo_DECK++) {

						if (lista_modificacaoUTE.at(idTermeletrica).at(i).tipo_de_modificacao == TipoModificacaoUTE_IPTER)
							lista_IPTER.at(periodo_DECK) = lista_modificacaoUTE.at(idTermeletrica).at(i).valor;

						else if (lista_modificacaoUTE.at(idTermeletrica).at(i).tipo_de_modificacao == TipoModificacaoUTE_TEIFT)
							lista_TEIFT.at(periodo_DECK) = lista_modificacaoUTE.at(idTermeletrica).at(i).valor;

						else if (lista_modificacaoUTE.at(idTermeletrica).at(i).tipo_de_modificacao == TipoModificacaoUTE_FCMAX)
							lista_FCMAX.at(periodo_DECK) = lista_modificacaoUTE.at(idTermeletrica).at(i).valor;

						else if (lista_modificacaoUTE.at(idTermeletrica).at(i).tipo_de_modificacao == TipoModificacaoUTE_GTMIN)
							lista_GTMIN.at(periodo_DECK) = lista_modificacaoUTE.at(idTermeletrica).at(i).valor;

						else if (lista_modificacaoUTE.at(idTermeletrica).at(i).tipo_de_modificacao == TipoModificacaoUTE_POTEF)
							lista_POTEF.at(periodo_DECK) = lista_modificacaoUTE.at(idTermeletrica).at(i).valor;

						else
							throw std::invalid_argument("Modificacao Termeletrica Invalida.");

					} // for (Periodo periodo = lista_modificacaoUTE.at(idTermeletrica).at(i).periodo_inicial; periodo <= lista_modificacaoUTE.at(idTermeletrica).at(i).periodo_final; periodo++) {

				} // for (int i = 0; i < lista_modificacaoUTE.at(idTermeletrica).size(); i++) {


				//
				// Atualiza atributos do horizonte de estudo com listas em base de periodo do DECK
				//

				for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); periodo_DECK++) {

					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						double sobreposicao = periodo.sobreposicao(periodo_DECK);

						if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
							sobreposicao += sobreposicao_atraso_periodo_inicial;

						if (sobreposicao > 0.0) {

							if (lista_IPTER.size() > 0) {
								const double valor_anterior = a_dados.vetorTermeletrica.at(idTermeletrica).getElementoVetor(AttVetorTermeletrica_indisponibilidade_programada, periodo, double());
								const double valor_novo = valor_anterior + sobreposicao * lista_IPTER.at(periodo_DECK);
								a_dados.vetorTermeletrica.at(idTermeletrica).setElemento(AttVetorTermeletrica_indisponibilidade_programada, periodo, valor_novo);
							}

							if (lista_TEIFT.size() > 0) {
								const double valor_anterior = a_dados.vetorTermeletrica.at(idTermeletrica).getElementoVetor(AttVetorTermeletrica_indisponibilidade_forcada, periodo, double());
								const double valor_novo = valor_anterior + sobreposicao * lista_TEIFT.at(periodo_DECK);
								a_dados.vetorTermeletrica.at(idTermeletrica).setElemento(AttVetorTermeletrica_indisponibilidade_forcada, periodo, valor_novo);
							}

							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {

								if (lista_GTMIN.size() > 0) {
									const double valor_anterior = a_dados.vetorTermeletrica.at(idTermeletrica).vetorUnidadeUTE.at(IdUnidadeUTE_1).getElementoMatriz(AttMatrizUnidadeUTE_potencia_minima, periodo, idPatamarCarga, double());
									const double valor_novo = valor_anterior + sobreposicao * lista_GTMIN.at(periodo_DECK);

									a_dados.vetorTermeletrica.at(idTermeletrica).vetorUnidadeUTE.at(IdUnidadeUTE_1).setElemento(AttMatrizUnidadeUTE_potencia_minima, periodo, idPatamarCarga, valor_novo);
								}
								if (lista_POTEF.size() > 0) {
									const double valor_anterior = a_dados.vetorTermeletrica.at(idTermeletrica).vetorUnidadeUTE.at(IdUnidadeUTE_1).getElementoMatriz(AttMatrizUnidadeUTE_potencia_maxima, periodo, idPatamarCarga, double());
									const double valor_novo = valor_anterior + sobreposicao * lista_POTEF.at(periodo_DECK);
									a_dados.vetorTermeletrica.at(idTermeletrica).vetorUnidadeUTE.at(IdUnidadeUTE_1).setElemento(AttMatrizUnidadeUTE_potencia_maxima, periodo, idPatamarCarga, valor_novo);
								}

							} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {

						} // if (sobreposicao > 0.0) {

					} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				} // for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); periodo_DECK++) {

				// (Específico para FCMAX, quando FCMAX = 0, GTMIN = 0)
				// Atualiza atributos do horizonte de estudo com listas em base de periodo do DECK
				//

				for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); periodo_DECK++) {

					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						double sobreposicao = periodo.sobreposicao(periodo_DECK);

						if ((periodo == horizonte_estudo.getIteradorInicial()) && (periodo_DECK == horizonte_estudo_DECK.getIteradorInicial()))
							sobreposicao += sobreposicao_atraso_periodo_inicial;

						if (sobreposicao > 0.0) {

							if (lista_FCMAX.size() > 0) {
								const double valor_anterior = a_dados.vetorTermeletrica.at(idTermeletrica).getElementoVetor(AttVetorTermeletrica_fator_de_capacidade, periodo, double());
								const double valor_novo = valor_anterior + sobreposicao * lista_FCMAX.at(periodo_DECK);
								a_dados.vetorTermeletrica.at(idTermeletrica).setElemento(AttVetorTermeletrica_fator_de_capacidade, periodo, valor_novo);

								if (valor_novo == 0.0) {
									if (a_dados.vetorTermeletrica.at(idTermeletrica).vetorUnidadeUTE.at(IdUnidadeUTE_1).getSizeMatriz(AttMatrizUnidadeUTE_potencia_minima) > 0) {
										if (a_dados.vetorTermeletrica.at(idTermeletrica).vetorUnidadeUTE.at(IdUnidadeUTE_1).getSizeMatriz(AttMatrizUnidadeUTE_potencia_minima, periodo) > 0) {
											for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {
												a_dados.vetorTermeletrica.at(idTermeletrica).vetorUnidadeUTE.at(IdUnidadeUTE_1).setElemento(AttMatrizUnidadeUTE_potencia_minima, periodo, idPatamarCarga, 0.0);
											}
										}
									}
									if (a_dados.vetorTermeletrica.at(idTermeletrica).vetorUnidadeUTE.at(IdUnidadeUTE_1).getSizeMatriz(AttMatrizUnidadeUTE_potencia_util) > 0) {
										if (a_dados.vetorTermeletrica.at(idTermeletrica).vetorUnidadeUTE.at(IdUnidadeUTE_1).getSizeMatriz(AttMatrizUnidadeUTE_potencia_util, periodo) > 0) {
											for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {
												a_dados.vetorTermeletrica.at(idTermeletrica).vetorUnidadeUTE.at(IdUnidadeUTE_1).setElemento(AttMatrizUnidadeUTE_potencia_util, periodo, idPatamarCarga, 0.0);
											}
										}
									}
								}

							}

						} // if (sobreposicao > 0.0) {

					} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				} // for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); periodo_DECK++) {


			} // if (lista_modificacaoUTE.at(idTermeletrica).size() > 0){

		} // for (IdTermeletrica idTermeletrica = idTermeletricaIni; idTermeletrica < idTermeletricaOut; a_dados.vetorTermeletrica.incr(idTermeletrica)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacoesUTE(a_dados): \n" + std::string(erro.what())); }

} // void LeituraCEPEL::aplicarModificacoesUTE() {


void LeituraCEPEL::inicializa_Submercados_Intercambios_Nao_Registrados(Dados& a_dados, const SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo) {

	try {

		const IdSubmercado idSubmercadoSE = getIdFromCodigoONS(lista_codigo_ONS_submercado, 1);

		if (idSubmercadoSE == IdSubmercado_Nenhum)
			throw std::invalid_argument("IdSubmercado invalido para SUDESTE");

		const IdSubmercado idSubmercadoS = getIdFromCodigoONS(lista_codigo_ONS_submercado, 2);

		if (idSubmercadoS == IdSubmercado_Nenhum)
			throw std::invalid_argument("IdSubmercado invalido para SUL");

		//Horizonte de estudo
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const double potencia_minima = 0;
		const double potencia_maxima = 99999;

		//*********************************
		//Cria Submercado IVAIPORÃ
		//*********************************

		IdSubmercado idSubmercado_inicializado = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_IV);

		if (idSubmercado_inicializado == IdSubmercado_Nenhum) {//O subsistema IVAIPORÃ deve ser inicializado

			//************************************************************
			//Inicializa Submercado
			//************************************************************

			const IdSubmercado idSubmercado = IdSubmercado_IVAIPORA;

			Submercado submercado;

			submercado.setAtributo(AttComumSubmercado_idSubmercado, idSubmercado);

			a_dados.vetorSubmercado.add(submercado);

			const std::string nome = "IV";

			a_dados.vetorSubmercado.at(idSubmercado).setAtributo(AttComumSubmercado_nome, nome);

			//////////////////////////////////////////////////////////////
			//Instancia demanda = 0 para todos os patamares
			a_dados.vetorSubmercado.at(idSubmercado).setMatriz(AttMatrizSubmercado_demanda, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>()));

			for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo)) {

				const IdPatamarCarga maiorIdPatamarCarga = get_maiorIdPatamarCarga_periodo_from_percentual_duracao_patamar_carga(a_dados, periodo);

				if (a_dados.vetorSubmercado.at(idSubmercado).getElementosMatriz(AttMatrizSubmercado_demanda, periodo, IdPatamarCarga(), double()).size() == 0)
					a_dados.vetorSubmercado.at(idSubmercado).setElementos(AttMatrizSubmercado_demanda, periodo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(maiorIdPatamarCarga, 0.0)));

			}//for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo)) {

			//////////////////////////////////////////////////////////////


			lista_codigo_ONS_submercado.setElemento(idSubmercado, codigo_IV);
			lista_submercado_mnemonico.setElemento(idSubmercado, nome);


			//*******************************************************************
			//Cria Intercambio Ivaiporã -> Sudeste
			//*******************************************************************

			if (true) {

				const IdIntercambio idIntercambio = IdIntercambio(a_dados.getMaiorId(IdIntercambio()) + 1);

				Intercambio intercambio;

				intercambio.setAtributo(AttComumIntercambio_idIntercambio, idIntercambio);

				a_dados.vetorIntercambio.add(intercambio);

				const IdSubmercado idSubmercado_origem = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_IV);
				const IdSubmercado idSubmercado_destino = idSubmercadoSE;

				const std::string nome_submercado_origem = a_dados.getAtributo(idSubmercado_origem, AttComumSubmercado_nome, std::string());
				const std::string nome_submercado_destino = a_dados.getAtributo(idSubmercado_destino, AttComumSubmercado_nome, std::string());

				a_dados.vetorIntercambio.at(idIntercambio).setAtributo(AttComumIntercambio_submercado_origem, idSubmercado_origem);
				a_dados.vetorIntercambio.at(idIntercambio).setAtributo(AttComumIntercambio_submercado_destino, idSubmercado_destino);

				a_dados.vetorIntercambio.at(idIntercambio).setAtributo(AttComumIntercambio_nome, nome_submercado_origem + "->" + nome_submercado_destino);

				for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

					const IdPatamarCarga maiorIdPatamarCarga = get_maiorIdPatamarCarga_periodo_from_percentual_duracao_patamar_carga(a_dados, periodo);

					for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
						a_dados.vetorIntercambio.at(idIntercambio).addElemento(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, potencia_minima);
						a_dados.vetorIntercambio.at(idIntercambio).addElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, potencia_maxima);
					}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

				}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {	

			}//if (true) {

			//*******************************************************************
			//Cria Intercambio Sudeste -> Ivaporã
			//*******************************************************************

			if (true) {

				const IdIntercambio idIntercambio = IdIntercambio(a_dados.getMaiorId(IdIntercambio()) + 1);

				Intercambio intercambio;

				intercambio.setAtributo(AttComumIntercambio_idIntercambio, idIntercambio);

				a_dados.vetorIntercambio.add(intercambio);

				const IdSubmercado idSubmercado_origem = getIdFromCodigoONS(lista_codigo_ONS_submercado, idSubmercadoSE);
				const IdSubmercado idSubmercado_destino = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_IV);

				const std::string nome_submercado_origem = a_dados.getAtributo(idSubmercado_origem, AttComumSubmercado_nome, std::string());
				const std::string nome_submercado_destino = a_dados.getAtributo(idSubmercado_destino, AttComumSubmercado_nome, std::string());

				a_dados.vetorIntercambio.at(idIntercambio).setAtributo(AttComumIntercambio_submercado_origem, idSubmercado_origem);
				a_dados.vetorIntercambio.at(idIntercambio).setAtributo(AttComumIntercambio_submercado_destino, idSubmercado_destino);

				a_dados.vetorIntercambio.at(idIntercambio).setAtributo(AttComumIntercambio_nome, nome_submercado_origem + "->" + nome_submercado_destino);

				for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

					const IdPatamarCarga maiorIdPatamarCarga = get_maiorIdPatamarCarga_periodo_from_percentual_duracao_patamar_carga(a_dados, periodo);

					for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
						a_dados.vetorIntercambio.at(idIntercambio).addElemento(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, potencia_minima);
						a_dados.vetorIntercambio.at(idIntercambio).addElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, potencia_maxima);
					}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

				}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

			}//if (true) {

			//*******************************************************************
			// Altera intercâmbio Sul-Sudeste para Sul-Ivaiporã (Caso não exista)
			//*******************************************************************

			IdIntercambio idIntercambio_S_IV = IdIntercambio_Nenhum;
			IdIntercambio idIntercambio_IV_S = IdIntercambio_Nenhum;

			IdIntercambio idIntercambio_S_SE = IdIntercambio_Nenhum;
			IdIntercambio idIntercambio_SE_S = IdIntercambio_Nenhum;

			const IdSubmercado idSubmercadoIV = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_IV);

			for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= a_dados.getMaiorId(IdIntercambio()); idIntercambio++) {

				const IdSubmercado idSubmercado_origem  = a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_origem, IdSubmercado());
				const IdSubmercado idSubmercado_destino = a_dados.getAtributo(idIntercambio, AttComumIntercambio_submercado_destino, IdSubmercado());

				if ((idSubmercado_origem == idSubmercadoS) && (idSubmercado_destino == idSubmercadoIV)) {
					if (idIntercambio_S_IV == IdIntercambio_Nenhum)
						idIntercambio_S_IV = idIntercambio;
					else
						throw std::invalid_argument("Erro ao indentificar o intercambio S-IV");
				}

				if ((idSubmercado_origem == idSubmercadoIV) && (idSubmercado_destino == idSubmercadoS)) {
					if (idIntercambio_IV_S == IdIntercambio_Nenhum)
						idIntercambio_IV_S = idIntercambio;
					else
						throw std::invalid_argument("Erro ao indentificar o intercambio IV-S");
				}

				if ((idSubmercado_origem == idSubmercadoS) && (idSubmercado_destino == idSubmercadoSE)) {
					if (idIntercambio_S_SE == IdIntercambio_Nenhum)
						idIntercambio_S_SE = idIntercambio;
					else
						throw std::invalid_argument("Erro ao indentificar o intercambio S-SE");
				}

				if ((idSubmercado_origem == idSubmercadoSE) && (idSubmercado_destino == idSubmercadoS)) {
					if (idIntercambio_SE_S == IdIntercambio_Nenhum)
						idIntercambio_SE_S = idIntercambio;
					else
						throw std::invalid_argument("Erro ao indentificar o intercambio SE-S");
				}

			} // for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= a_dados.getMaiorId(IdIntercambio()); idIntercambio++) {

			
			if ((idIntercambio_S_SE != IdIntercambio_Nenhum) && (idIntercambio_SE_S != IdIntercambio_Nenhum)) {

				if ((idIntercambio_S_IV == IdIntercambio_Nenhum) && (idIntercambio_IV_S == IdIntercambio_Nenhum)) {
					a_dados.vetorIntercambio.at(idIntercambio_S_SE).setAtributo(AttComumIntercambio_submercado_destino, idSubmercadoIV);
					a_dados.vetorIntercambio.at(idIntercambio_S_SE).setAtributo(AttComumIntercambio_nome, std::string("SUL->IV"));
					a_dados.vetorIntercambio.at(idIntercambio_SE_S).setAtributo(AttComumIntercambio_submercado_origem, idSubmercadoIV);
					a_dados.vetorIntercambio.at(idIntercambio_SE_S).setAtributo(AttComumIntercambio_nome, std::string("IV->SUL"));
				} // if ((idIntercambio_S_SE != IdIntercambio_Nenhum) && (idIntercambio_SE_S != IdIntercambio_Nenhum)) {

				else
					throw std::invalid_argument("Erro ao indentificar os intercambios S-SE e SE-S na criacao dos intercambios S-IV e IV-S");

			} // if ((idIntercambio_S_IV == IdIntercambio_Nenhum) && (idIntercambio_IV_S == IdIntercambio_Nenhum)) {

			else if ((idIntercambio_S_IV == IdIntercambio_Nenhum) && (idIntercambio_IV_S == IdIntercambio_Nenhum)) {

				//Se não está inicializado o S_SE / SE_S (caso do médio prazo) e não existe S_IV / IV_S -> Cria S_IV / IV_S (caso curto-prazo)

				//Cria IV_S

				if (true) {

					const IdIntercambio idIntercambio = IdIntercambio(a_dados.getMaiorId(IdIntercambio()) + 1);

					Intercambio intercambio;

					intercambio.setAtributo(AttComumIntercambio_idIntercambio, idIntercambio);

					a_dados.vetorIntercambio.add(intercambio);

					const IdSubmercado idSubmercado_origem = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_IV);
					const IdSubmercado idSubmercado_destino = idSubmercadoS;

					const std::string nome_submercado_origem = a_dados.getAtributo(idSubmercado_origem, AttComumSubmercado_nome, std::string());
					const std::string nome_submercado_destino = a_dados.getAtributo(idSubmercado_destino, AttComumSubmercado_nome, std::string());

					a_dados.vetorIntercambio.at(idIntercambio).setAtributo(AttComumIntercambio_submercado_origem, idSubmercado_origem);
					a_dados.vetorIntercambio.at(idIntercambio).setAtributo(AttComumIntercambio_submercado_destino, idSubmercado_destino);

					a_dados.vetorIntercambio.at(idIntercambio).setAtributo(AttComumIntercambio_nome, nome_submercado_origem + "->" + nome_submercado_destino);

					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						const IdPatamarCarga maiorIdPatamarCarga = get_maiorIdPatamarCarga_periodo_from_percentual_duracao_patamar_carga(a_dados, periodo);

						for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
							a_dados.vetorIntercambio.at(idIntercambio).addElemento(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, potencia_minima);
							a_dados.vetorIntercambio.at(idIntercambio).addElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, potencia_maxima);
						}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

					}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {	

				}//if (true) {

				//Cria S_IV

				if (true) {

					const IdIntercambio idIntercambio = IdIntercambio(a_dados.getMaiorId(IdIntercambio()) + 1);

					Intercambio intercambio;

					intercambio.setAtributo(AttComumIntercambio_idIntercambio, idIntercambio);

					a_dados.vetorIntercambio.add(intercambio);

					const IdSubmercado idSubmercado_origem = idSubmercadoS;
					const IdSubmercado idSubmercado_destino = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_IV);

					const std::string nome_submercado_origem = a_dados.getAtributo(idSubmercado_origem, AttComumSubmercado_nome, std::string());
					const std::string nome_submercado_destino = a_dados.getAtributo(idSubmercado_destino, AttComumSubmercado_nome, std::string());

					a_dados.vetorIntercambio.at(idIntercambio).setAtributo(AttComumIntercambio_submercado_origem, idSubmercado_origem);
					a_dados.vetorIntercambio.at(idIntercambio).setAtributo(AttComumIntercambio_submercado_destino, idSubmercado_destino);

					a_dados.vetorIntercambio.at(idIntercambio).setAtributo(AttComumIntercambio_nome, nome_submercado_origem + "->" + nome_submercado_destino);

					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						const IdPatamarCarga maiorIdPatamarCarga = get_maiorIdPatamarCarga_periodo_from_percentual_duracao_patamar_carga(a_dados, periodo);

						for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
							a_dados.vetorIntercambio.at(idIntercambio).addElemento(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, potencia_minima);
							a_dados.vetorIntercambio.at(idIntercambio).addElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, potencia_maxima);
						}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

					}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {	

				}//if (true) {
				
			}//else if ((idIntercambio_S_IV == IdIntercambio_Nenhum) && (idIntercambio_IV_S == IdIntercambio_Nenhum)) {
	
		}//if (true) {

		//*********************************
		//Cria Submercado ANDE
		//*********************************

		idSubmercado_inicializado = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_ANDE);

		if (idSubmercado_inicializado == IdSubmercado_Nenhum) {//O subsistema ANDE deve ser inicializado

			//************************************************************
			//Inicializa Submercado
			//************************************************************

			const IdSubmercado idSubmercado = IdSubmercado_ANDE;

			Submercado submercado;

			submercado.setAtributo(AttComumSubmercado_idSubmercado, idSubmercado);

			a_dados.vetorSubmercado.add(submercado);

			const std::string nome = "ande";

			a_dados.vetorSubmercado.at(idSubmercado).setAtributo(AttComumSubmercado_nome, nome);

			//////////////////////////////////////////////////////////////
			//Instancia demanda = 0 para todos os patamares
			a_dados.vetorSubmercado.at(idSubmercado).setMatriz(AttMatrizSubmercado_demanda, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>()));

			for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo)) {

				const IdPatamarCarga maiorIdPatamarCarga = get_maiorIdPatamarCarga_periodo_from_percentual_duracao_patamar_carga(a_dados, periodo);

				if (a_dados.vetorSubmercado.at(idSubmercado).getElementosMatriz(AttMatrizSubmercado_demanda, periodo, IdPatamarCarga(), double()).size() == 0)
					a_dados.vetorSubmercado.at(idSubmercado).setElementos(AttMatrizSubmercado_demanda, periodo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(maiorIdPatamarCarga, 0.0)));

			}//for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo)) {

			//////////////////////////////////////////////////////////////

			lista_codigo_ONS_submercado.setElemento(idSubmercado, codigo_ANDE);
			lista_submercado_mnemonico.setElemento(idSubmercado, nome);


			//************************************************************
			//Cria Intercambios entre submercado ANDE e submercado Sudeste
			//************************************************************

			if (true) {

				const IdIntercambio idIntercambio = IdIntercambio(a_dados.getMaiorId(IdIntercambio()) + 1);

				Intercambio intercambio;

				intercambio.setAtributo(AttComumIntercambio_idIntercambio, idIntercambio);

				a_dados.vetorIntercambio.add(intercambio);

				const IdSubmercado idSubmercado_origem = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_ANDE);
				const IdSubmercado idSubmercado_destino = getIdFromCodigoONS(lista_codigo_ONS_submercado, idSubmercadoSE);

				const std::string nome_submercado_origem = a_dados.getAtributo(idSubmercado_origem, AttComumSubmercado_nome, std::string());
				const std::string nome_submercado_destino = a_dados.getAtributo(idSubmercado_destino, AttComumSubmercado_nome, std::string());

				a_dados.vetorIntercambio.at(idIntercambio).setAtributo(AttComumIntercambio_submercado_origem, idSubmercado_origem);
				a_dados.vetorIntercambio.at(idIntercambio).setAtributo(AttComumIntercambio_submercado_destino, idSubmercado_destino);

				a_dados.vetorIntercambio.at(idIntercambio).setAtributo(AttComumIntercambio_nome, nome_submercado_origem + "->" + nome_submercado_destino);

				for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

					const IdPatamarCarga maiorIdPatamarCarga = get_maiorIdPatamarCarga_periodo_from_percentual_duracao_patamar_carga(a_dados, periodo);

					for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
						a_dados.vetorIntercambio.at(idIntercambio).addElemento(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, potencia_minima);
						a_dados.vetorIntercambio.at(idIntercambio).addElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, potencia_maxima);
					}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

				}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {	

			}//if (idSubmercado_inicializado == IdSubmercado_Nenhum) {

			//////////////////////////////////////
			//PatamarDeficit ANDE igual ao Sudeste
			//////////////////////////////////////

			a_dados.vetorSubmercado.at(idSubmercado).vetorPatamarDeficit = a_dados.vetorSubmercado.at(idSubmercadoSE).vetorPatamarDeficit;

		}//if (true) {

		//*********************************
		//Cria Submercado ITAIPU
		//*********************************

		if (!itaipu_tipo_detalhamento_producao_por_conjunto) { //Se a representaçao de Itaipú é por usina cria submercado Itaipú

			idSubmercado_inicializado = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_submercado_ITAIPU);

			if (idSubmercado_inicializado == IdSubmercado_Nenhum) {

				//************************************************************
				//Inicializa Submercado
				//************************************************************

				const IdSubmercado idSubmercado = IdSubmercado_ITAIPU;

				idSubmercado_inicializado = idSubmercado;

				Submercado submercado;

				submercado.setAtributo(AttComumSubmercado_idSubmercado, idSubmercado);

				a_dados.vetorSubmercado.add(submercado);

				const std::string nome = "ITAIPU";

				a_dados.vetorSubmercado.at(idSubmercado).setAtributo(AttComumSubmercado_nome, nome);

				//////////////////////////////////////////////////////////////
				//Instancia demanda = 0 para todos os patamares
				a_dados.vetorSubmercado.at(idSubmercado).setMatriz(AttMatrizSubmercado_demanda, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>()));

				for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo)) {

					const IdPatamarCarga maiorIdPatamarCarga = get_maiorIdPatamarCarga_periodo_from_percentual_duracao_patamar_carga(a_dados, periodo);

					if (a_dados.vetorSubmercado.at(idSubmercado).getElementosMatriz(AttMatrizSubmercado_demanda, periodo, IdPatamarCarga(), double()).size() == 0)
						a_dados.vetorSubmercado.at(idSubmercado).setElementos(AttMatrizSubmercado_demanda, periodo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(maiorIdPatamarCarga, 0.0)));

				}//for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo)) {

				//////////////////////////////////////////////////////////////

				lista_codigo_ONS_submercado.setElemento(idSubmercado, codigo_submercado_ITAIPU);
				lista_submercado_mnemonico.setElemento(idSubmercado, nome);


				//*******************************************************************
				//Cria Intercambio ITAIPU -> ANDE
				//*******************************************************************

				if (true) {

					const IdIntercambio idIntercambio = IdIntercambio(a_dados.getMaiorId(IdIntercambio()) + 1);

					Intercambio intercambio;

					intercambio.setAtributo(AttComumIntercambio_idIntercambio, idIntercambio);

					a_dados.vetorIntercambio.add(intercambio);

					const IdSubmercado idSubmercado_origem = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_submercado_ITAIPU);
					const IdSubmercado idSubmercado_destino = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_ANDE);

					const std::string nome_submercado_origem = a_dados.getAtributo(idSubmercado_origem, AttComumSubmercado_nome, std::string());
					const std::string nome_submercado_destino = a_dados.getAtributo(idSubmercado_destino, AttComumSubmercado_nome, std::string());

					a_dados.vetorIntercambio.at(idIntercambio).setAtributo(AttComumIntercambio_submercado_origem, idSubmercado_origem);
					a_dados.vetorIntercambio.at(idIntercambio).setAtributo(AttComumIntercambio_submercado_destino, idSubmercado_destino);

					a_dados.vetorIntercambio.at(idIntercambio).setAtributo(AttComumIntercambio_nome, nome_submercado_origem + "->" + nome_submercado_destino);

					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						const IdPatamarCarga maiorIdPatamarCarga = get_maiorIdPatamarCarga_periodo_from_percentual_duracao_patamar_carga(a_dados, periodo);

						for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
							a_dados.vetorIntercambio.at(idIntercambio).addElemento(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, potencia_minima);
							a_dados.vetorIntercambio.at(idIntercambio).addElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, potencia_maxima);
						}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

					}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {	

				}//if (true) {

				//*******************************************************************
				//Cria Intercambio ITAIPU -> Iviaporã
				//*******************************************************************

				if (true) {

					const IdIntercambio idIntercambio = IdIntercambio(a_dados.getMaiorId(IdIntercambio()) + 1);

					Intercambio intercambio;

					intercambio.setAtributo(AttComumIntercambio_idIntercambio, idIntercambio);

					a_dados.vetorIntercambio.add(intercambio);

					const IdSubmercado idSubmercado_origem = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_submercado_ITAIPU);
					const IdSubmercado idSubmercado_destino = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_IV);

					const std::string nome_submercado_origem = a_dados.getAtributo(idSubmercado_origem, AttComumSubmercado_nome, std::string());
					const std::string nome_submercado_destino = a_dados.getAtributo(idSubmercado_destino, AttComumSubmercado_nome, std::string());

					a_dados.vetorIntercambio.at(idIntercambio).setAtributo(AttComumIntercambio_submercado_origem, idSubmercado_origem);
					a_dados.vetorIntercambio.at(idIntercambio).setAtributo(AttComumIntercambio_submercado_destino, idSubmercado_destino);

					a_dados.vetorIntercambio.at(idIntercambio).setAtributo(AttComumIntercambio_nome, nome_submercado_origem + "->" + nome_submercado_destino);

					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						const IdPatamarCarga maiorIdPatamarCarga = get_maiorIdPatamarCarga_periodo_from_percentual_duracao_patamar_carga(a_dados, periodo);

						for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
							a_dados.vetorIntercambio.at(idIntercambio).addElemento(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, potencia_minima);
							a_dados.vetorIntercambio.at(idIntercambio).addElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, potencia_maxima);
						}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

					}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				}//if (true) {

			}//if (true) {

			const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, 66);

			a_dados.vetorHidreletrica.at(idHidreletrica).setAtributo(AttComumHidreletrica_submercado, idSubmercado_inicializado);

		}//if (!itaipu_tipo_detalhamento_producao_por_conjunto) {


	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::inicializa_Submercados_Intercambios_Nao_Registrados(Dados& a_dados): \n" + std::string(erro.what())); }


}//void LeituraCEPEL::inicializa_Submercados_Intercambios_Nao_Registrados() {


void LeituraCEPEL::adicionaIntercambiosHidraulicosApartirJusanteDesvio(Dados& a_dados) {

	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const IdHidreletrica idHidreletricaIni = a_dados.getMenorId(IdHidreletrica());
		const IdHidreletrica idHidreletricaOut = a_dados.getIdOut(IdHidreletrica());

		for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			const IdHidreletrica idHidreletrica_jusante_desvio = a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica());

			if (idHidreletrica_jusante_desvio != IdHidreletrica_Nenhum) {

				const SmartEnupla<Periodo, double> vetor_zero(horizonte_estudo, 0.0);
				const SmartEnupla<Periodo, double> desvio_agua_maximo(horizonte_estudo, 99999);//Valor não informado nos decks

				const IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico(a_dados.getMaiorId(IdIntercambioHidraulico()) + 1);

				IntercambioHidraulico intercambioHidraulico;
				intercambioHidraulico.setAtributo(AttComumIntercambioHidraulico_idIntercambioHidraulico, idIntercambioHidraulico);

				a_dados.vetorIntercambioHidraulico.add(intercambioHidraulico);

				a_dados.vetorIntercambioHidraulico.at(idIntercambioHidraulico).setAtributo(AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico_desvio);

				a_dados.vetorIntercambioHidraulico.at(idIntercambioHidraulico).setAtributo(AttComumIntercambioHidraulico_hidreletrica_origem, idHidreletrica);
				a_dados.vetorIntercambioHidraulico.at(idIntercambioHidraulico).setAtributo(AttComumIntercambioHidraulico_hidreletrica_destino, idHidreletrica_jusante_desvio);

				a_dados.vetorIntercambioHidraulico.at(idIntercambioHidraulico).setVetor(AttVetorIntercambioHidraulico_desvio_agua_minimo, vetor_zero);
				a_dados.vetorIntercambioHidraulico.at(idIntercambioHidraulico).setVetor(AttVetorIntercambioHidraulico_desvio_agua_maximo, desvio_agua_maximo);

			}//if (idHidreletricajusante_desvio != IdHidreletrica_Nenhum) {

		}//for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::adicionaIntercambiosHidraulicosApartirJusanteDesvio(Dados &a_dados): \n" + std::string(erro.what())); }

} // void LeituraCEPEL::adicionaIntercambiosHidraulicosApartirJusanteDesvio() {


void LeituraCEPEL::adicionaLimitesDesvioApartirJusanteDesvio(Dados& a_dados) {

	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const IdHidreletrica idHidreletricaIni = a_dados.getMenorId(IdHidreletrica());
		const IdHidreletrica idHidreletricaOut = a_dados.getIdOut(IdHidreletrica());

		for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			const IdHidreletrica idHidreletrica_jusante_desvio = a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica());

			if (idHidreletrica_jusante_desvio != IdHidreletrica_Nenhum) {

				a_dados.vetorHidreletrica.at(idHidreletrica).setVetor(AttVetorHidreletrica_vazao_desviada_minima, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
				a_dados.vetorHidreletrica.at(idHidreletrica).setVetor(AttVetorHidreletrica_vazao_desviada_maxima, SmartEnupla<Periodo, double>(horizonte_estudo, 99999));//Valor não informado nos decks

			}//if (idHidreletricajusante_desvio != IdHidreletrica_Nenhum) {

		}//for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::adicionaLimitesDesvioApartirJusanteDesvio(Dados &a_dados): \n" + std::string(erro.what())); }

} // void LeituraCEPEL::adicionaLimitesDesvioApartirJusanteDesvio() {


void LeituraCEPEL::set_zero_vazao_defluente_minima_historica_usina_fio_sem_reservatorio_a_montante(Dados& a_dados, const IdHidreletrica a_idHidreletrica) {
	try {

		//Considera defluência mínima nula para as usinas a fio d`água até o primeiro reservatório da cascata

		const Periodo periodo_inicial = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio()).getIteradorInicial();
		const Periodo periodo_final = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio()).getIteradorFinal();

		for (Periodo periodo = periodo_inicial; periodo <= periodo_final; a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio()).incrementarIterador(periodo)) {

			if (a_dados.vetorHidreletrica.at(a_idHidreletrica).getElementoVetor(AttVetorHidreletrica_regularizacao, periodo, int()) != 1) {

				bool usina_sem_regularizacao_sem_reservatorio_montante = true;

				std::vector<IdHidreletrica> idHidreletricas_montante;

				int numMontante = a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_montante);

				for (int m = 1; m <= numMontante; m++)
					idHidreletricas_montante.push_back(a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_montante, m, IdHidreletrica()));

				while (true) {

					//Teste se nao existem mais usinas a montante
					if (int(idHidreletricas_montante.size()) == 0)
						break;

					//Teste usina com regularizacao a montante encontrada
					if (a_dados.vetorHidreletrica.at(idHidreletricas_montante.at(0)).getElementoVetor(AttVetorHidreletrica_regularizacao, periodo, int()) == 1) {
						usina_sem_regularizacao_sem_reservatorio_montante = false;
						break;
					}//if (vetorHidreletrica.at(idHidreletricas_montante.at(0)).getElementoVetor(AttVetorHidreletrica_regularizacao, periodo, bool())) {

					//Inclui usinas a montante da usina idHidreletricas_montante.at(0) 

					numMontante = a_dados.getSizeVetor(idHidreletricas_montante.at(0), AttVetorHidreletrica_montante);

					for (int m = 1; m <= numMontante; m++)
						idHidreletricas_montante.push_back(a_dados.getElementoVetor(idHidreletricas_montante.at(0), AttVetorHidreletrica_montante, m, IdHidreletrica()));

					//Elimina do vetor a usina já validada
					idHidreletricas_montante.erase(idHidreletricas_montante.begin());

				}//while (true) {

				//Atualiza vazao_defluente_minima com valor = 0 das usinas fio d'àgua sem usina com regularizaçao a montante
				if (usina_sem_regularizacao_sem_reservatorio_montante)
					a_dados.vetorHidreletrica.at(a_idHidreletrica).setAtributo(AttComumHidreletrica_vazao_defluente_minima_historica, 0.0);

			}//if (!vetorHidreletrica.at(a_idHidreletrica).getElementoVetor(AttVetorHidreletrica_regularizacao, periodo, bool())) {

		} // for (Periodo periodo = periodo_inicial; periodo <= periodo_final; getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio()).incrementarIterador(periodo)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::set_zero_vazao_defluente_minima_historica_usina_fio_sem_reservatorio_a_montante(): " + getFullString(a_idHidreletrica) + std::string(erro.what())); }
} // void LeituraCEPEL::set_zero_vazao_defluente_minima_historica_usina_fio_sem_reservatorio_a_montante{

void LeituraCEPEL::sequenciarRestricoesEletricas(Dados& a_dados) {

	try {

		const IdRestricaoEletrica maiorIdRestricaoEletrica = a_dados.getMaiorId(IdRestricaoEletrica());

		if (maiorIdRestricaoEletrica == IdRestricaoEletrica_Nenhum)
			return;

		Dados dados;

		IdRestricaoEletrica idSequencial = IdRestricaoEletrica_1;

		// Lista restrições complexas
		for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= maiorIdRestricaoEletrica; idRestricaoEletrica++) {

			if (!a_dados.isRestricaoEletrica_simples(idRestricaoEletrica)) {
				RestricaoEletrica restricaoEletrica;
				restricaoEletrica = a_dados.vetorRestricaoEletrica.at(idRestricaoEletrica);
				restricaoEletrica.setAtributo(AttComumRestricaoEletrica_idRestricaoEletrica, idSequencial);
				dados.vetorRestricaoEletrica.add(restricaoEletrica);
				idSequencial++;
			}

		}

		// Lista restrições simples
		for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= maiorIdRestricaoEletrica; idRestricaoEletrica++) {

			if (a_dados.isRestricaoEletrica_simples(idRestricaoEletrica)) {
				RestricaoEletrica restricaoEletrica;
				restricaoEletrica = a_dados.vetorRestricaoEletrica.at(idRestricaoEletrica);
				restricaoEletrica.setAtributo(AttComumRestricaoEletrica_idRestricaoEletrica, idSequencial);
				dados.vetorRestricaoEletrica.add(restricaoEletrica);
				idSequencial++;
			}

		}

		a_dados.vetorRestricaoEletrica = VetorRestricaoEletricaEmDados();


		a_dados.vetorRestricaoEletrica = dados.vetorRestricaoEletrica;

	} //try 
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::sequenciarRestricoesEletricas: \n" + std::string(erro.what())); }

}


void LeituraCEPEL::sequenciarRestricoesHidraulicas(Dados& a_dados) {

	try {

		const IdRestricaoOperativaUHE maiorIdRestricaoOperativaUHE = a_dados.getMaiorId(IdRestricaoOperativaUHE());

		if (maiorIdRestricaoOperativaUHE == IdRestricaoOperativaUHE_Nenhum)
			return;

		Dados dados;

		IdRestricaoOperativaUHE idSequencial = IdRestricaoOperativaUHE_1;

		// Lista restrições com complexas
		for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= maiorIdRestricaoOperativaUHE; idRestricaoOperativaUHE++) {

			if (!a_dados.isRestricaoOperativaUHE_simples(idRestricaoOperativaUHE)) {
				RestricaoOperativaUHE RestricaoOperativaUHE;
				RestricaoOperativaUHE = a_dados.vetorRestricaoOperativaUHE.at(idRestricaoOperativaUHE);
				RestricaoOperativaUHE.setAtributo(AttComumRestricaoOperativaUHE_idRestricaoOperativaUHE, idSequencial);
				dados.vetorRestricaoOperativaUHE.add(RestricaoOperativaUHE);
				idSequencial++;
			}

		}

		// Lista restrições com simples
		for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= maiorIdRestricaoOperativaUHE; idRestricaoOperativaUHE++) {

			if (a_dados.isRestricaoOperativaUHE_simples(idRestricaoOperativaUHE)) {
				RestricaoOperativaUHE RestricaoOperativaUHE;
				RestricaoOperativaUHE = a_dados.vetorRestricaoOperativaUHE.at(idRestricaoOperativaUHE);
				RestricaoOperativaUHE.setAtributo(AttComumRestricaoOperativaUHE_idRestricaoOperativaUHE, idSequencial);
				dados.vetorRestricaoOperativaUHE.add(RestricaoOperativaUHE);
				idSequencial++;
			}

		}

		a_dados.vetorRestricaoOperativaUHE = VetorRestricaoOperativaUHEEmDados();


		a_dados.vetorRestricaoOperativaUHE = dados.vetorRestricaoOperativaUHE;

	} //try 
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::sequenciarRestricoesHidraulicas: \n" + std::string(erro.what())); }

}

IdPatamarCarga LeituraCEPEL::get_maiorIdPatamarCarga_periodo_from_percentual_duracao_patamar_carga(Dados& a_dados, const Periodo a_periodo) {

	try {

		int numero_patamares_carga = 0;

		//Premissa: um patamar tem percentual_duracao_patamar > 0 
		const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo, IdPatamarCarga());

		for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

			if (a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_periodo, idPatamarCarga, double()) > 0.0)
				numero_patamares_carga++;

		}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {


		return IdPatamarCarga(numero_patamares_carga);

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::get_maiorIdPatamarCarga_periodo_from_percentual_duracao_patamar_carga: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::instanciar_hidreletricas_sem_producao_para_acoplamento_cortes_NW(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const int a_codigo_usina, const int a_codigo_posto, const int a_codigo_posto_acoplamento_ENA, const int a_codigo_ONS_REE) {

	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		////////////////////////////////////////////////////////////////////////////////////////////

		if (!a_dados.vetorHidreletrica.isInstanciado(a_idHidreletrica)) {

			Hidreletrica   hidreletrica;

			//Set idHidreletrica
			hidreletrica.setAtributo(AttComumHidreletrica_idHidreletrica, a_idHidreletrica);
			a_dados.vetorHidreletrica.add(hidreletrica);

		}//if (!a_dados.vetorHidreletrica.isInstanciado(a_idHidreletrica)) {


		a_dados.vetorHidreletrica.at(a_idHidreletrica).setAtributo(AttComumHidreletrica_codigo_usina, a_codigo_usina);
		a_dados.vetorHidreletrica.at(a_idHidreletrica).setAtributo(AttComumHidreletrica_codigo_posto, a_codigo_posto);
		a_dados.vetorHidreletrica.at(a_idHidreletrica).setAtributo(AttComumHidreletrica_codigo_posto_acoplamento_ENA, a_codigo_posto_acoplamento_ENA);
		a_dados.vetorHidreletrica.at(a_idHidreletrica).setAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica_sem_producao);
		lista_codigo_ONS_REE.setElemento(a_idHidreletrica, a_codigo_ONS_REE);
		lista_codigo_ONS_hidreletrica.setElemento(a_idHidreletrica, a_codigo_usina);


		/////////////////////////////////////////////////
		//inicializa afluência
		
		if (!a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorAfluencia.isInstanciado(IdAfluencia_vazao_afluente)) {
			Afluencia afluencia;
			afluencia.setAtributo(AttComumAfluencia_idAfluencia, IdAfluencia_vazao_afluente);
			a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorAfluencia.add(afluencia);
		}
		

		if (!processoEstocasticoHidrologicoPreConfig_instanciado)
			instanciar_variavelAleatoria_x_idHidreletrica(a_dados, a_idHidreletrica);

		///////////////////////////////////////////////////////////////////////
		//Instancia atributos necessários para a validação das hidrelétricas
		///////////////////////////////////////////////////////////////////////

		if (!a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.isInstanciado(IdReservatorio_1)) {
			Reservatorio reservatorio;
			reservatorio.setAtributo(AttComumReservatorio_idReservatorio, IdReservatorio_1);

			a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.add(reservatorio);

		}//if (!a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.isInstanciado(IdReservatorio_1)) {

		
		if (a_dados.getSizeVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_evaporacao) == 0) {
			for (IdMes idMes = IdMes_1; idMes <= IdMes_12; idMes++)
				a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).addElemento(AttVetorReservatorio_evaporacao, idMes, 0.0);
		}//if (a_dados.getSizeVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_evaporacao) == 0) {

		if (a_dados.getSizeVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_volume_0) == 0)
			a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_volume_0, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getAtributo(AttComumReservatorio_poli_cota_volume_0, double())));

		if (a_dados.getSizeVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_volume_1) == 0)
			a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_volume_1, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getAtributo(AttComumReservatorio_poli_cota_volume_1, double())));

		if (a_dados.getSizeVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_volume_2) == 0)
			a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_volume_2, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getAtributo(AttComumReservatorio_poli_cota_volume_2, double())));

		if (a_dados.getSizeVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_volume_3) == 0)
			a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_volume_3, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getAtributo(AttComumReservatorio_poli_cota_volume_3, double())));

		if (a_dados.getSizeVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_volume_4) == 0)
			a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_volume_4, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getAtributo(AttComumReservatorio_poli_cota_volume_4, double())));

		if (a_dados.getSizeVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_area_0) == 0)
			a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_area_0, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getAtributo(AttComumReservatorio_poli_cota_area_0, double())));

		if (a_dados.getSizeVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_area_1) == 0)
			a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_area_1, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getAtributo(AttComumReservatorio_poli_cota_area_1, double())));

		if (a_dados.getSizeVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_area_2) == 0)
			a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_area_2, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getAtributo(AttComumReservatorio_poli_cota_area_2, double())));

		if (a_dados.getSizeVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_area_3) == 0)
			a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_area_3, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getAtributo(AttComumReservatorio_poli_cota_area_3, double())));

		if (a_dados.getSizeVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_area_4) == 0)
			a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_area_4, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getAtributo(AttComumReservatorio_poli_cota_area_4, double())));

		if (a_dados.getSizeVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_enchendo_volume_morto) == 0)
			a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(AttVetorReservatorio_enchendo_volume_morto, SmartEnupla<Periodo, int>(horizonte_estudo, a_dados.vetorHidreletrica.at(a_idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getAtributo(AttComumReservatorio_enchendo_volume_morto, int())));

		if (a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_regularizacao) == 0)
			a_dados.vetorHidreletrica.at(a_idHidreletrica).setVetor(AttVetorHidreletrica_regularizacao, SmartEnupla<Periodo, int>(horizonte_estudo, 0));
		

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::instanciar_hidreletricas_sem_producao_para_acoplamento_cortes_NW: \n" + std::string(erro.what())); }

}

IdUsinaNaoSimulada LeituraCEPEL::getIdUsinaNaoSimulada_from_nome_or_bloco(const std::string a_nome, const std::string a_bloco) {

	try {

		IdUsinaNaoSimulada idUsinaNaoSimulada = IdUsinaNaoSimulada_1; //Por padrao IdUsinaNaoSimulada_1 é a total

		if (a_nome.find("UFVMMGD") != std::string::npos || a_nome.find("UFVgd") != std::string::npos)
			idUsinaNaoSimulada = IdUsinaNaoSimulada_UFV_MMGD;
		else if (a_nome.find("PCHMMGD") != std::string::npos || a_nome.find("PCHgd") != std::string::npos)
			idUsinaNaoSimulada = IdUsinaNaoSimulada_PCH_MMGD;
		else if (a_nome.find("PCTMMGD") != std::string::npos || a_nome.find("PCTgd") != std::string::npos)
			idUsinaNaoSimulada = IdUsinaNaoSimulada_PCT_MMGD;
		else if (a_nome.find("EOLMMGD") != std::string::npos || a_nome.find("EOLgd") != std::string::npos)
			idUsinaNaoSimulada = IdUsinaNaoSimulada_EOL_MMGD;
		else if (a_nome.find("UFV") != std::string::npos)
			idUsinaNaoSimulada = IdUsinaNaoSimulada_UFV;
		else if (a_nome.find("PCH") != std::string::npos)
			idUsinaNaoSimulada = IdUsinaNaoSimulada_PCH;
		else if (a_nome.find("PCT") != std::string::npos)
			idUsinaNaoSimulada = IdUsinaNaoSimulada_PCT;
		else if (a_nome.find("EOL") != std::string::npos)
			idUsinaNaoSimulada = IdUsinaNaoSimulada_EOL;
		else if (a_bloco == "1")
			idUsinaNaoSimulada = IdUsinaNaoSimulada_1;
		else if (a_bloco == "2")
			idUsinaNaoSimulada = IdUsinaNaoSimulada_2;
		else if (a_bloco == "3")
			idUsinaNaoSimulada = IdUsinaNaoSimulada_3;
		else if (a_bloco == "4")
			idUsinaNaoSimulada = IdUsinaNaoSimulada_4;
		else if (a_bloco == "5")
			idUsinaNaoSimulada = IdUsinaNaoSimulada_5;
		else if (a_bloco == "6")
			idUsinaNaoSimulada = IdUsinaNaoSimulada_6;
		else if (a_bloco == "7")
			idUsinaNaoSimulada = IdUsinaNaoSimulada_7;
		else if (a_bloco == "8")
			idUsinaNaoSimulada = IdUsinaNaoSimulada_8;

		return idUsinaNaoSimulada;

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::getIdUsinaNaoSimulada_from_nome_or_bloco: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_cortes_NEWAVE(Dados& a_dados, const SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo, const std::string a_nomeArquivo_cortes, const std::string a_diretorio_att_premissas, const int a_maior_ONS_REE, const SmartEnupla<Periodo, bool> a_horizonte_processo_estocastico, const SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> a_percentual_duracao_patamar_carga_original)
{
	try {

		std::string line;
		std::string atributo;

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Podem ser informados os cortes do NEWAVE via duas opções: 
		// (i) fcfnwn.rvX: arquivo impresso pelo modelo DECOMP, com a impressão dos cortes de acoplamento entre o DC/NW 
		//                 (i.e. somente é impresso os cortes do período de acoplamento: terceiro mês do horizonte do NW)
		// (ii) nwlistcf.rel: impressão dos cortes de TODO o horizonte do NW
		// Nota: caso o usuário informe (i) e (ii) simultaneamente, (ii) vai ser mandatório
		//       (ii) serve para também para estudos com expansão do horizonte do CP
		//////////////////////////////////////////////////////////////////////////////////////////////////////////

		const Periodo periodo_ultimo_sobreposicao = get_periodo_ultimo_sobreposicao_com_horizonte_DC(a_dados);
		const Periodo periodo_inicial = a_horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final = a_horizonte_estudo.getIteradorFinal();

		if ((periodo_ultimo_sobreposicao < periodo_final) && (a_nomeArquivo_cortes.find("fcfnwn") != std::string::npos))//Existe expansão do horizonte
			throw std::invalid_argument("Cortes do NEWAVE do arquivo fcfnwn.rvX nao copativeis com acomplamento de horizonte estendido");

		std::cout << "Lendo cortes do modelo NEWAVE do arquivo: " << a_nomeArquivo_cortes << " ..." << std::endl;

		const IdHidreletrica idHidreletricaIni = a_dados.getMenorId(IdHidreletrica());
		const IdHidreletrica idHidreletricaEnd = a_dados.getMaiorId(IdHidreletrica());
		const IdHidreletrica idHidreletricaOut = a_dados.getIdOut(IdHidreletrica());

		/////////////////////////////////////////////////////
		//Define horizontes de interesse

		SmartEnupla <Periodo, bool> horizonte_tendencia_mais_processo_estocastico; //Vai conter nos iteradores os periodos da tendencia_temporal + processo estocástico
		//SmartEnupla <Periodo, bool> horizonte_tendencia_mais_estudo; //Vai conter nos iteradores os periodos da tendencia_temporal + horizonte de estudo (para cálculo das premissas necessárias do cálculo das produtibilidades EAR e ENA)

		//Tendência temporal
		const SmartEnupla <Periodo, double> tendencia_temporal = a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.at(IdVariavelAleatoria_1).vetorVariavelAleatoriaInterna.at(IdVariavelAleatoriaInterna_1).getVetor(AttVetorVariavelAleatoriaInterna_tendencia_temporal, Periodo(), double());

		for (Periodo periodo = tendencia_temporal.getIteradorInicial(); periodo <= tendencia_temporal.getIteradorFinal(); tendencia_temporal.incrementarIterador(periodo)) {
			horizonte_tendencia_mais_processo_estocastico.addElemento(periodo, true);
			//horizonte_tendencia_mais_estudo.addElemento(periodo, true);

		}

		//Adiciona os periodos do processo estocástico (incluídos os períodos da extensão do horizonte)
		for (Periodo periodo = a_horizonte_processo_estocastico.getIteradorInicial(); periodo <= a_horizonte_processo_estocastico.getIteradorFinal(); a_horizonte_processo_estocastico.incrementarIterador(periodo))
			horizonte_tendencia_mais_processo_estocastico.addElemento(periodo, true);


		///////////////////////////////////////////////////////////////////////
		//Cria horizonte_MENSAL da tendência + estudo 
		///////////////////////////////////////////////////////////////////////

		SmartEnupla<Periodo, bool> horizonte_tendencia_mais_processo_estocastico_MENSAL;

		const Periodo periodo_inicial_MENSAL = Periodo(TipoPeriodo_mensal, horizonte_tendencia_mais_processo_estocastico.getIteradorInicial().getMes(), horizonte_tendencia_mais_processo_estocastico.getIteradorInicial().getAno());
		const Periodo periodo_final_MENSAL = Periodo(TipoPeriodo_mensal, horizonte_tendencia_mais_processo_estocastico.getIteradorFinal().getMes(), horizonte_tendencia_mais_processo_estocastico.getIteradorFinal().getAno());

		for (Periodo periodo_aux = periodo_inicial_MENSAL; periodo_aux <= periodo_final_MENSAL; periodo_aux++)
			horizonte_tendencia_mais_processo_estocastico_MENSAL.addElemento(periodo_aux, true);

		//////////////////////////////////////////////////////////////////////////////////
		//2 Cálculo da produtibilidade_EAR acumulada por usina (aporte em cada REE)
		//////////////////////////////////////////////////////////////////////////////////

		atualizar_vetores_premissas_calculo_produtibilidades(a_dados, horizonte_tendencia_mais_processo_estocastico);

		if (a_dados.vetorHidreletrica.at(a_dados.getMenorId(IdHidreletrica())).getSizeVetor(AttVetorHidreletrica_produtibilidade_acumulada_EAR) == 0)//Pode ter sido instanciado para o cálculo das produtibilidade_acumulada_EAR em restrições de energia armazenada RHV
			calcular_produtibilidade_EAR_acumulada_por_usina(a_dados, SmartEnupla<Periodo, bool>(a_horizonte_estudo, true), a_maior_ONS_REE); //produtibilidade_EAR é estática e calculada com as premissas do 1° período do estudo

		calcular_produtibilidade_ENA_por_usina_por_periodo(a_dados, horizonte_tendencia_mais_processo_estocastico);

		//////////////////////////////////////////////////////////////////////////////////
		//3 Cálculo da ENA x REE x cenário x período
		//////////////////////////////////////////////////////////////////////////////////
		defineHidreletricasMontanteNaCascataENA(a_dados);//Define para cada idHidreletrica todas as usinas que estão a montante na cascata


		SmartEnupla<IdHidreletrica, IdVariavelAleatoria> mapIdVar(idHidreletricaIni, std::vector<IdVariavelAleatoria>(int(idHidreletricaEnd - idHidreletricaIni) + 1, IdVariavelAleatoria_Nenhum));
		SmartEnupla<IdHidreletrica, IdVariavelAleatoriaInterna> mapIdVarInterna(idHidreletricaIni, std::vector<IdVariavelAleatoriaInterna>(int(idHidreletricaEnd - idHidreletricaIni) + 1, IdVariavelAleatoriaInterna_Nenhum));
		SmartEnupla<int, IdHidreletrica> lista_codPosto_idHidreletrica(1, std::vector<IdHidreletrica>(999, IdHidreletrica_Nenhum));
		SmartEnupla<int, IdHidreletrica> lista_codUsina_idHidreletrica(1, std::vector<IdHidreletrica>(999, IdHidreletrica_Nenhum));

		for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {
			lista_codUsina_idHidreletrica.at(a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_codigo_usina, int())) = idHidreletrica;
			lista_codPosto_idHidreletrica.at(a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_codigo_posto, int())) = idHidreletrica;
			IdVariavelAleatoria        idVariavelAleatoria = IdVariavelAleatoria_Nenhum;
			IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_Nenhum;
			a_dados.getIdVariavelAleatoriaIdVariavelAleatoriaInternaFromIdHidreletrica(IdProcessoEstocastico_hidrologico_hidreletrica, idVariavelAleatoria, idVariavelAleatoriaInterna, idHidreletrica);
			mapIdVar.at(idHidreletrica) = idVariavelAleatoria;
			mapIdVarInterna.at(idHidreletrica) = idVariavelAleatoriaInterna;
		}

		//////////////////////////////////////////////////////////////////////////////////
		//4 Leitura dos cortes NW e conversão de formato REE em usina individualizada
		//////////////////////////////////////////////////////////////////////////////////

		if (true) {

			///////////////////////////////////////////////////////////////////////////////////////
			// Atualiza parâmetros de leitura do arquivo de cortes caso seja o arquivo nwlistcf.rel:
			///////////////////////////////////////////////////////////////////////////////////////

			bool is_arquivo_fcfnwn = true;

			int periodo_acoplamento = 0; //Inicializa com este valor por ser parte da lógica

			if (a_nomeArquivo_cortes.find("nwlistcf") != std::string::npos) {
				//No nwlistcf.rel o período se refer ao estágio onde os cortes são colocados no modelo de otimização 
				// (p.ex, Periodo: 10, significa a FCF que deve ser acoplada no mês outubro significando o custo de novembro em diante)
				//Informação validada com o arquivo fcfnwn.rvX (saída do modelo DC)

				is_arquivo_fcfnwn = false;

				/////////////////////////////////////////
				//Atualiza periodo_acoplamento
				/////////////////////////////////////////

				Periodo periodo_inicial_mensal = Periodo(TipoPeriodo_mensal, horizonte_estudo_DECK.getIteradorFinal().getMes(), horizonte_estudo_DECK.getIteradorFinal().getAno());
				periodo_inicial_mensal--;

				Periodo periodo_final_mensal = horizonte_tendencia_mais_processo_estocastico_MENSAL.getIteradorFinal();

				//////////////////////
				Periodo periodo_inicial_aux = Periodo(TipoPeriodo_mensal, IdMes_1, periodo_inicial_mensal.getAno()); //A impressão dos cortes no arquivo nwlistcf.rel é desde o mês 1 até o mês 12 (mesmo que o estudo comece p.ex. no mês 6)

				for (Periodo periodo = periodo_inicial_aux; periodo <= periodo_final_mensal; periodo++)//Todos os períodos são em base mensal
					periodo_acoplamento++;

			}//if (a_nomeArquivo_cortes.find("nwlistcf") != std::string::npos) {


			////////////////////////////////////////////////////////////////////////////////////

			const IdCenario idCenario_final = a_dados.processoEstocastico_hidrologico.getIterador1Final(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario());

			int ordem_maxima_PAR = 12;
			const int numero_patamares = 3;
			const int lag_GNL = 2;

			const double numero_horas_estagio_NEWAVE = 730.5;

			const double conversao_MWporVazao_em_MWhporVolume = 1e6 / 3600.0;

			const IdEstagio idEstagio_pos_estudo = IdEstagio(a_dados.getVetor(AttVetorDados_horizonte_otimizacao, IdEstagio(), Periodo()).getIteradorFinal() + 1);

			Estagio estagio_pos_estudo;

			estagio_pos_estudo.setAtributo(AttComumEstagio_idEstagio, idEstagio_pos_estudo);
			estagio_pos_estudo.setAtributo(AttComumEstagio_selecao_cortes_nivel_dominancia, 0);
			estagio_pos_estudo.setAtributo(AttComumEstagio_cortes_multiplos, 0);

			const Periodo periodo_pos_estudo = Periodo(TipoPeriodo_mensal, horizonte_tendencia_mais_processo_estocastico_MENSAL.getIteradorFinal() + 1);
			estagio_pos_estudo.setAtributo(AttComumEstagio_periodo_otimizacao, periodo_pos_estudo);

			estagio_pos_estudo.alocarCorteBenders(8000);

			const std::string strVarDecisaoVIIdEstagioPeriodo = std::string("VarDecisaoVI," + getString(estagio_pos_estudo.getAtributo(AttComumEstagio_idEstagio, IdEstagio())) + "," + getString(periodo_pos_estudo));
			const std::string strVarDecisaoENAIdEstagioPeriodo = std::string("VarDecisaoENA," + getString(estagio_pos_estudo.getAtributo(AttComumEstagio_idEstagio, IdEstagio())) + "," + getString(periodo_pos_estudo));
			const std::string strVarDecisaoPTDISPCOMIdEstagio = std::string("VarDecisaoPTDISPCOM," + getString(estagio_pos_estudo.getAtributo(AttComumEstagio_idEstagio, IdEstagio())));

			//Processo estocástico
			const IdTermeletrica	idTermeletricaIni = a_dados.getMenorId(IdTermeletrica());
			const IdTermeletrica	idTermeletricaEnd = a_dados.getMaiorId(IdTermeletrica());
			const IdTermeletrica	idTermeletricaOut = a_dados.getIdOut(IdTermeletrica());

			SmartEnupla<IdVariavelEstado, double> estados;
			SmartEnupla <IdHidreletrica, IdVariavelEstado> estados_VI(idHidreletricaIni, std::vector<IdVariavelEstado>(int(idHidreletricaEnd - idHidreletricaIni) + 1, IdVariavelEstado_Nenhum));
			SmartEnupla<IdReservatorioEquivalente, SmartEnupla<int, IdVariavelEstado>> estados_ENA(IdReservatorioEquivalente(1), std::vector<SmartEnupla<int, IdVariavelEstado>>(a_maior_ONS_REE, SmartEnupla < int, IdVariavelEstado>(1, std::vector<IdVariavelEstado>(ordem_maxima_PAR, IdVariavelEstado_Nenhum))));
			SmartEnupla <IdTermeletrica, SmartEnupla<IdSubmercado, SmartEnupla<int, IdVariavelEstado>>> estados_GNL(idTermeletricaIni, std::vector<SmartEnupla<IdSubmercado, SmartEnupla<int, IdVariavelEstado>>>(int(idTermeletricaEnd - idTermeletricaIni) + 1, SmartEnupla<IdSubmercado, SmartEnupla<int, IdVariavelEstado>>()));

			SmartEnupla<IdSubmercado, IdReservatorioEquivalente> mapeamentoSubmercadoxREE(IdSubmercado(1), std::vector<IdReservatorioEquivalente>(int(IdSubmercado_Excedente - 1), IdReservatorioEquivalente_Nenhum));

			mapeamentoSubmercadoxREE.at(IdSubmercado_SUDESTE) = IdReservatorioEquivalente_1_SUDESTE;
			mapeamentoSubmercadoxREE.at(IdSubmercado_SUL) = IdReservatorioEquivalente_2_SUL;
			mapeamentoSubmercadoxREE.at(IdSubmercado_NORDESTE) = IdReservatorioEquivalente_3_NORDESTE;
			mapeamentoSubmercadoxREE.at(IdSubmercado_NORTE) = IdReservatorioEquivalente_4_NORTE;

			std::cout << "Mapeando variaveis de estado..." << std::endl;

			//const SmartEnupla<Periodo, SmartEnupla<IdCenario, double>> inicializacao_conversao_ENA_acoplamento(lista_coeficiente_idHidreletricas_calculo_ENA_x_periodo_x_REE_x_cenario, SmartEnupla<IdCenario, double>(IdCenario_1, std::vector<double>(idCenario_final, 0.0)));

			if (true) {
				SmartEnupla<IdReservatorioEquivalente, bool> coeficientes_EAR;
				SmartEnupla<IdReservatorioEquivalente, SmartEnupla<int, bool>> coeficiente_ENA;

				leitura_cortes_NEWAVE_para_dimensionamento(a_dados, a_horizonte_estudo, coeficientes_EAR, coeficiente_ENA, a_nomeArquivo_cortes, is_arquivo_fcfnwn, periodo_acoplamento, a_maior_ONS_REE);

				int ordem_maxima_cortes = 0;
				for (int lag = 1; lag <= ordem_maxima_PAR; lag++) {
					for (IdReservatorioEquivalente idREE = coeficiente_ENA.getIteradorInicial(); idREE <= coeficiente_ENA.getIteradorFinal(); idREE++) {
						if (coeficiente_ENA.at(idREE).getElemento(lag)) {
							ordem_maxima_cortes = lag;
							break;
						}
					}
				}

				ordem_maxima_PAR = ordem_maxima_cortes;

				// Variaveis Estado
				for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

					// Estados VI
					if ((a_dados.getSizeVetor(idHidreletrica, AttVetorHidreletrica_produtibilidade_acumulada_EAR) > 0) && (a_dados.getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, a_horizonte_estudo.getIteradorFinal(), double()) > 0.0) && (a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_idHidreletrica, IdHidreletrica()) != IdHidreletrica_176_COMPPAFMOX)) {
						for (IdReservatorioEquivalente idREE = coeficientes_EAR.getIteradorInicial(); idREE <= coeficientes_EAR.getIteradorFinal(); idREE++) {
							if ((coeficientes_EAR.getElemento(idREE)) && (a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_produtibilidade_acumulada_EAR, idREE, double()) != 0.0)) {
								estados_VI.at(idHidreletrica) = estagio_pos_estudo.addVariavelEstado(false, TipoSubproblemaSolver_geral, std::string(strVarDecisaoVIIdEstagioPeriodo + "," + getString(idHidreletrica) + ",0.0,inf"), -1, -1);
								estados.addElemento(estados_VI.at(idHidreletrica), 0.0);
								break;
							}
						}
					}

					//mapeamentoSubmercadoxREE.at(a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_submercado, IdSubmercado())) = IdReservatorioEquivalente(lista_codigo_ONS_REE.at(idHidreletrica));

				} // for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

				// Estados ENA

				for (int lag = 1; lag <= ordem_maxima_PAR; lag++) {

					const Periodo periodo_lag = periodo_pos_estudo - lag;

					for (IdReservatorioEquivalente idREE = coeficiente_ENA.getIteradorInicial(); idREE <= coeficiente_ENA.getIteradorFinal(); idREE++) {

						estados_ENA.at(idREE).at(lag) = estagio_pos_estudo.addVariavelEstado(false, TipoSubproblemaSolver_geral, std::string(strVarDecisaoENAIdEstagioPeriodo + "," + getString(periodo_lag) + "," + getString(idREE)), -1, -1);
						estados.addElemento(estados_ENA.at(idREE).at(lag), 0.0);

					}//for (IdReservatorioEquivalente idREE = coeficiente_ENA.getIteradorInicial(); idREE <= coeficiente_ENA.getIteradorFinal(); idREE++) {

				}//for (int lag = 1; lag <= ordem_maxima_PAR; lag++) {


				/*
				for (int lag = 1; lag <= ordem_maxima_PAR; lag++) {

					const Periodo periodo_lag = periodo_pos_estudo - lag;

					for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

						for (IdReservatorioEquivalente idREE = coeficiente_ENA.getIteradorInicial(); idREE <= coeficiente_ENA.getIteradorFinal(); idREE++) {

							bool termo_0 = false; bool termo_1 = false;
							bool vazio_0 = false; bool vazio_1 = false;
							for (IdCenario idCen = IdCenario_1; idCen <= idCenario_final; idCen++) {
								if (lista_termo_independente_calculo_ENA_x_periodo_x_REE_x_cenario.at(periodo_lag).at(idHidreletrica).size() > 0) {
									if (lista_termo_independente_calculo_ENA_x_periodo_x_REE_x_cenario.at(periodo_lag).at(idHidreletrica).at(idREE).size() > 0) {
										if (lista_termo_independente_calculo_ENA_x_periodo_x_REE_x_cenario.at(periodo_lag).at(idHidreletrica).at(idREE).at(idCen) != 0.0) {

											if (!a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorioEquivalente.isInstanciado(idREE)) {
												ReservatorioEquivalente ree;
												ree.setAtributo(AttComumReservatorioEquivalente_idReservatorioEquivalente, idREE);
												a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorioEquivalente.add(ree);
											}

											if (a_dados.getSize1Matriz(idHidreletrica, idREE, AttMatrizReservatorioEquivalente_conversao_ENA_acoplamento_0) == 0)
												a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorioEquivalente.at(idREE).setMatriz_forced(AttMatrizReservatorioEquivalente_conversao_ENA_acoplamento_0, inicializacao_conversao_ENA_acoplamento);

											const double conversao_ENA_acoplamento_0 = lista_termo_independente_calculo_ENA_x_periodo_x_REE_x_cenario.at(periodo_lag).at(idHidreletrica).at(idREE).at(idCen);

											a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorioEquivalente.at(idREE).setElemento(AttMatrizReservatorioEquivalente_conversao_ENA_acoplamento_0, periodo_lag, idCen, conversao_ENA_acoplamento_0);

											termo_0 = true;

										}
									}
									else vazio_0 = true;
								}
								else vazio_0 = true;

								if (lista_coeficiente_idHidreletricas_calculo_ENA_x_periodo_x_REE_x_cenario.at(periodo_lag).at(idHidreletrica).size() > 0) {
									if (lista_coeficiente_idHidreletricas_calculo_ENA_x_periodo_x_REE_x_cenario.at(periodo_lag).at(idHidreletrica).at(idREE).size() > 0) {
										if (lista_coeficiente_idHidreletricas_calculo_ENA_x_periodo_x_REE_x_cenario.at(periodo_lag).at(idHidreletrica).at(idREE).at(idCen) != 0.0) {

											if (!a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorioEquivalente.isInstanciado(idREE)) {
												ReservatorioEquivalente ree;
												ree.setAtributo(AttComumReservatorioEquivalente_idReservatorioEquivalente, idREE);
												a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorioEquivalente.add(ree);
											}

											if (a_dados.getSize1Matriz(idHidreletrica, idREE, AttMatrizReservatorioEquivalente_conversao_ENA_acoplamento_1) == 0)
												a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorioEquivalente.at(idREE).setMatriz_forced(AttMatrizReservatorioEquivalente_conversao_ENA_acoplamento_1, inicializacao_conversao_ENA_acoplamento);

											const double conversao_ENA_acoplamento_1 = lista_coeficiente_idHidreletricas_calculo_ENA_x_periodo_x_REE_x_cenario.at(periodo_lag).at(idHidreletrica).at(idREE).at(idCen);

											a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorioEquivalente.at(idREE).setElemento(AttMatrizReservatorioEquivalente_conversao_ENA_acoplamento_1, periodo_lag, idCen, conversao_ENA_acoplamento_1);

											termo_1 = true;

										}
									}
									else vazio_1 = true;
								}
								else vazio_1 = true;

								if (vazio_0 && vazio_1)
									break;

								if (((termo_0) || (termo_1)) && (estados_ENA.at(idREE).at(lag) == IdVariavelEstado_Nenhum)) {
									estados_ENA.at(idREE).at(lag) = estagio_pos_estudo.addVariavelEstado(TipoSubproblemaSolver_geral, std::string(strVarDecisaoENAIdEstagioPeriodo + "," + getString(periodo_lag) + "," + getString(idREE)), -1, -1);
									estados.addElemento(estados_ENA.at(idREE).at(lag), 0.0);
								}

							} // for (IdCenario idCen = IdCenario_1; idCen <= idCenario_final; idCen++) {
						} // for (IdReservatorioEquivalente idREE = coeficiente_ENA.getIteradorInicial(); idREE <= coeficiente_ENA.getIteradorFinal(); idREE++) {

					} // for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {
				} // for (int lag = 1; lag <= ordem_maxima_PAR; lag++) {

				lista_coeficiente_idHidreletricas_calculo_ENA_x_periodo_x_REE_x_cenario = SmartEnupla<Periodo, SmartEnupla<IdHidreletrica, SmartEnupla<IdReservatorioEquivalente, SmartEnupla<IdCenario, double>>>>();
				lista_termo_independente_calculo_ENA_x_periodo_x_REE_x_cenario = SmartEnupla<Periodo, SmartEnupla<IdHidreletrica, SmartEnupla<IdReservatorioEquivalente, SmartEnupla<IdCenario, double>>>>();

				*/

			} // if (true) {

			// Estados PTDISPCOM
			for (IdTermeletrica idUTE = idTermeletricaIni; idUTE < idTermeletricaOut; a_dados.vetorTermeletrica.incr(idUTE)) {

				if (a_dados.getAtributo(idUTE, AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada, int()) > 0) {

					const IdSubmercado idSubmercado = a_dados.getAtributo(idUTE, AttComumTermeletrica_submercado, IdSubmercado());

					estados_GNL.at(idUTE) = SmartEnupla<IdSubmercado, SmartEnupla<int, IdVariavelEstado>>(idSubmercado, std::vector<SmartEnupla<int, IdVariavelEstado>>(1, SmartEnupla<int, IdVariavelEstado>(1, std::vector<IdVariavelEstado>(lag_GNL, IdVariavelEstado_Nenhum))));

					double potencia_disponivel_maxima = 0.0;
					double potencia_disponivel_minima = 0.0;
					for (IdPatamarCarga idPat = IdPatamarCarga_1; idPat <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, a_horizonte_estudo.getIteradorFinal(), IdPatamarCarga()); idPat++) {
						const double percentual_duracao = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_horizonte_estudo.getIteradorFinal(), idPat, double());
						potencia_disponivel_minima = a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_minima, a_horizonte_estudo.getIteradorFinal(), idPat, double()) * percentual_duracao;
						potencia_disponivel_maxima = a_dados.getElementoMatriz(idUTE, AttMatrizTermeletrica_potencia_disponivel_maxima, a_horizonte_estudo.getIteradorFinal(), idPat, double()) * percentual_duracao;
					}

					estados_GNL.at(idUTE).at(idSubmercado).at(1) = estagio_pos_estudo.addVariavelEstado(false, TipoSubproblemaSolver_geral, std::string(strVarDecisaoPTDISPCOMIdEstagio + "," + getString(periodo_pos_estudo + 1) + "," + getString(idUTE) + "," + getString(potencia_disponivel_minima) + "," + getString(potencia_disponivel_maxima)), -1, -1);
					estados.addElemento(estados_GNL.at(idUTE).at(idSubmercado).at(1), 0.0);

					estados_GNL.at(idUTE).at(idSubmercado).at(2) = estagio_pos_estudo.addVariavelEstado(false, TipoSubproblemaSolver_geral, std::string(strVarDecisaoPTDISPCOMIdEstagio + "," + getString(periodo_pos_estudo) + "," + getString(idUTE) + "," + getString(potencia_disponivel_minima) + "," + getString(potencia_disponivel_maxima)), -1, -1);
					estados.addElemento(estados_GNL.at(idUTE).at(idSubmercado).at(2), 0.0);

				}
			} // for (IdTermeletrica idUTE = idTermeletricaIni; idUTE < idTermeletricaOut; a_dados.vetorTermeletrica.incr(idUTE)) {

			// Estados VMINOP
			const IdMes mes_referencia_penalizacao_VMINOP = IdMes_11;
			Periodo periodo_penalizacao_VMINOP = Periodo(TipoPeriodo_minuto, Periodo(mes_referencia_penalizacao_VMINOP, periodo_pos_estudo.getAno()) + 1) - 1;
			if (periodo_pos_estudo.getMes() > mes_referencia_penalizacao_VMINOP)
				periodo_penalizacao_VMINOP = Periodo(TipoPeriodo_minuto, Periodo(mes_referencia_penalizacao_VMINOP, IdAno(int(periodo_pos_estudo.getAno()) + 1)) + 1) - 1;
			const std::string strVarDecisaoZP0_VF_FINFIdEstagio = std::string("VarDecisaoZP0_VF_FINF," + getString(estagio_pos_estudo.getAtributo(AttComumEstagio_idEstagio, IdEstagio())) + "," + getString(periodo_penalizacao_VMINOP));
			estados.addElemento(estagio_pos_estudo.addVariavelEstado(false, TipoSubproblemaSolver_geral, strVarDecisaoZP0_VF_FINFIdEstagio, -1, -1), 0.0);


			const double perc_pat1 = a_percentual_duracao_patamar_carga_original.at(a_horizonte_estudo.getIteradorFinal()).at(IdPatamarCarga_1);
			const double perc_pat2 = a_percentual_duracao_patamar_carga_original.at(a_horizonte_estudo.getIteradorFinal()).at(IdPatamarCarga_2);
			const double perc_pat3 = a_percentual_duracao_patamar_carga_original.at(a_horizonte_estudo.getIteradorFinal()).at(IdPatamarCarga_3);


			//const double perc_pat1 = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_horizonte_estudo.getIteradorFinal(), IdPatamarCarga_1, double());
			//const double perc_pat2 = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_horizonte_estudo.getIteradorFinal(), IdPatamarCarga_2, double());
			//const double perc_pat3 = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, a_horizonte_estudo.getIteradorFinal(), IdPatamarCarga_3, double());

			SmartEnupla<IdRealizacao, double> rhs_corte(IdRealizacao_1, std::vector<double>(1, 0.0));
			SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> coeficientes_corte(IdRealizacao_1, std::vector<SmartEnupla<IdVariavelEstado, double>>(1, SmartEnupla<IdVariavelEstado, double>(IdVariavelEstado_1, std::vector<double>(int(estados.getIteradorFinal()), 0.0))));

			///////////////////////////////////////////////////////////////////////////////////////

			std::ifstream leituraArquivo(a_nomeArquivo_cortes);

			////////////////////////////////////////////////////////////////////////////////////
			//Parâmetros para identificar o bloco de informação quando for o arquivo fcfnwn.rvX
			int numero_simbolo_cabecalho = 0;
			std::string simbolo_cabecalho = "X---------";

			////////////////////////////////////////////////////////////////////////////////////
			//Parâmetro para identificar o bloco de informação quando for o arquivo nwlistcf.rel
			std::string str_periodo_acoplamento = "PERIODO:" + getString(periodo_acoplamento);

			////////////////////////////////////////////////////////////////////////////////////

			//*********************************
			//Leitura dos cortes
			//*********************************

			std::cout << "Instanciando cortes..." << std::endl;

			bool is_bloco_informacao = false;

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);

				if (line.size() >= 23) {

					double rhs_valor = 0.0;

					if (is_bloco_informacao) {

						////////////////////////////////////////////
						//Critério de parada arquivo nwlistcf.rel
						////////////////////////////////////////////

						atributo = line.substr(0, 9);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						if (atributo == "PERIODO")//Significa que passou para um novo bloco de cortes de um outro período
							break;
						////////////////////////////////////////////

						//Leitura RHS
						atributo = line.substr(pos_RHS, tam_RHS);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						rhs_valor = std::atof(atributo.c_str());

						//Leitura Coeficientes

						SmartEnupla<IdReservatorioEquivalente, double> coeficientes_EAR(IdReservatorioEquivalente(1), std::vector<double>(IdReservatorioEquivalente(a_maior_ONS_REE), 0.0));
						SmartEnupla<IdReservatorioEquivalente, SmartEnupla<int, double>> coeficiente_ENA(IdReservatorioEquivalente(1), std::vector<SmartEnupla<int, double>>(IdReservatorioEquivalente(a_maior_ONS_REE), SmartEnupla<int, double>(1, std::vector<double>(ordem_maxima_PAR, 0.0))));
						SmartEnupla<IdReservatorioEquivalente, SmartEnupla<int, SmartEnupla<int, double>>> coeficiente_GNL(IdReservatorioEquivalente(1), std::vector<SmartEnupla<int, SmartEnupla<int, double>>>(IdReservatorioEquivalente(a_maior_ONS_REE), SmartEnupla<int, SmartEnupla<int, double>>(1, std::vector<SmartEnupla<int, double>>(numero_patamares, SmartEnupla<int, double>(1, std::vector<double>(lag_GNL, 0.0))))));
						SmartEnupla<IdReservatorioEquivalente, double> coeficientes_Vminop(IdReservatorioEquivalente(1), std::vector<double>(IdReservatorioEquivalente(a_maior_ONS_REE), 0.0));

						for (int pos = 0; pos < a_maior_ONS_REE; pos++) {

							//idREE
							atributo = line.substr(pos_idREE, tam_idREE);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdReservatorioEquivalente idReservatorioEquivalente = IdReservatorioEquivalente(std::atoi(atributo.c_str()));

							///////////////////////////////
							//Coef.Earm ($/MWh)
							///////////////////////////////
							atributo = line.substr(pos_Coef_Earm, tam_Coef_Earm);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double coeficiente_energia_armazenada = std::atof(atributo.c_str());

							///////////////////////////////
							//Coeficientes para Eafl ($/MWh)
							///////////////////////////////

							//lag_1

							atributo = "";

							if (pos_Eafl_lag_1 > 0) {
								atributo = line.substr(pos_Eafl_lag_1, tam_Eafl_lag_1);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}

							const double coeficiente_ENA_lag_1 = std::atof(atributo.c_str());

							//lag_2

							atributo = "";

							if (pos_Eafl_lag_2 > 0) {
								atributo = line.substr(pos_Eafl_lag_2, tam_Eafl_lag_2);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}

							const double coeficiente_ENA_lag_2 = std::atof(atributo.c_str());

							//lag_3

							atributo = "";

							if (pos_Eafl_lag_3 > 0) {
								atributo = line.substr(pos_Eafl_lag_3, tam_Eafl_lag_3);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}//if (tam_Eafl_lag_3 > 0) {

							const double coeficiente_ENA_lag_3 = std::atof(atributo.c_str());

							//lag_4

							atributo = "";

							if (pos_Eafl_lag_4 > 0) {
								atributo = line.substr(pos_Eafl_lag_4, tam_Eafl_lag_4);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}

							const double coeficiente_ENA_lag_4 = std::atof(atributo.c_str());

							//lag_5

							atributo = "";

							if (pos_Eafl_lag_5 > 0) {
								atributo = line.substr(pos_Eafl_lag_5, tam_Eafl_lag_5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}

							const double coeficiente_ENA_lag_5 = std::atof(atributo.c_str());

							//lag_6

							atributo = "";

							if (pos_Eafl_lag_6 > 0) {
								atributo = line.substr(pos_Eafl_lag_6, tam_Eafl_lag_6);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}//if (pos_Eafl_lag_6 > 0) {

							const double coeficiente_ENA_lag_6 = std::atof(atributo.c_str());

							//lag_7

							atributo = "";

							if (pos_Eafl_lag_7 > 0) {
								atributo = line.substr(pos_Eafl_lag_7, tam_Eafl_lag_7);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}

							const double coeficiente_ENA_lag_7 = std::atof(atributo.c_str());

							//lag_8

							atributo = "";

							if (pos_Eafl_lag_8 > 0) {
								atributo = line.substr(pos_Eafl_lag_8, tam_Eafl_lag_8);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}

							const double coeficiente_ENA_lag_8 = std::atof(atributo.c_str());

							//lag_9

							atributo = "";

							if (pos_Eafl_lag_9 > 0) {
								atributo = line.substr(pos_Eafl_lag_9, tam_Eafl_lag_9);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}

							const double coeficiente_ENA_lag_9 = std::atof(atributo.c_str());

							//lag_10

							atributo = "";

							if (pos_Eafl_lag_10 > 0) {
								atributo = line.substr(pos_Eafl_lag_10, tam_Eafl_lag_10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}

							const double coeficiente_ENA_lag_10 = std::atof(atributo.c_str());

							//lag_11

							atributo = "";

							if (pos_Eafl_lag_11 > 0) {
								atributo = line.substr(pos_Eafl_lag_11, tam_Eafl_lag_11);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}

							const double coeficiente_ENA_lag_11 = std::atof(atributo.c_str());

							//lag_12

							atributo = "";

							if (pos_Eafl_lag_12 > 0) {
								atributo = line.substr(pos_Eafl_lag_12, tam_Eafl_lag_12);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}

							const double coeficiente_ENA_lag_12 = std::atof(atributo.c_str());

							///////////////////////////////
							//Coeficientes para GNL ($/MWh)
							///////////////////////////////

							//pat_1_lag_1
							atributo = line.substr(pos_GNL_pat_1_lag_1, tam_GNL_pat_1_lag_1);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double coeficiente_GNL_pat_1_lag_1 = std::atof(atributo.c_str());


							//pat_1_lag_2
							atributo = line.substr(pos_GNL_pat_1_lag_2, tam_GNL_pat_1_lag_2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double coeficiente_GNL_pat_1_lag_2 = std::atof(atributo.c_str());

							//pat_2_lag_1
							atributo = line.substr(pos_GNL_pat_2_lag_1, tam_GNL_pat_2_lag_1);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double coeficiente_GNL_pat_2_lag_1 = std::atof(atributo.c_str());

							//pat_2_lag_2
							atributo = line.substr(pos_GNL_pat_2_lag_2, tam_GNL_pat_2_lag_2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double coeficiente_GNL_pat_2_lag_2 = std::atof(atributo.c_str());

							//pat_3_lag_1
							atributo = line.substr(pos_GNL_pat_3_lag_1, tam_GNL_pat_3_lag_1);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double coeficiente_GNL_pat_3_lag_1 = std::atof(atributo.c_str());

							//pat_3_lag_2
							atributo = line.substr(pos_GNL_pat_3_lag_2, tam_GNL_pat_3_lag_2);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double coeficiente_GNL_pat_3_lag_2 = std::atof(atributo.c_str());

							///////////////////////////////
							//Coef.Vminop-Max ($/MWh)
							///////////////////////////////
							atributo = line.substr(pos_Vminop, tam_Vminop);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double coeficiente_Vminop = std::atof(atributo.c_str());

							/////////////////////////////////
							//Armazena info em SmartEnuplas
							/////////////////////////////////

							coeficientes_EAR.setElemento(idReservatorioEquivalente, coeficiente_energia_armazenada);
							if (coeficiente_ENA_lag_1 != 0.0)
								coeficiente_ENA.at(idReservatorioEquivalente).setElemento(1, coeficiente_ENA_lag_1);
							if (coeficiente_ENA_lag_2 != 0.0)
								coeficiente_ENA.at(idReservatorioEquivalente).setElemento(2, coeficiente_ENA_lag_2);
							if (coeficiente_ENA_lag_3 != 0.0)
								coeficiente_ENA.at(idReservatorioEquivalente).setElemento(3, coeficiente_ENA_lag_3);
							if (coeficiente_ENA_lag_4 != 0.0)
								coeficiente_ENA.at(idReservatorioEquivalente).setElemento(4, coeficiente_ENA_lag_4);
							if (coeficiente_ENA_lag_5 != 0.0)
								coeficiente_ENA.at(idReservatorioEquivalente).setElemento(5, coeficiente_ENA_lag_5);
							if (coeficiente_ENA_lag_6 != 0.0)
								coeficiente_ENA.at(idReservatorioEquivalente).setElemento(6, coeficiente_ENA_lag_6);
							if (coeficiente_ENA_lag_7 != 0.0)
								coeficiente_ENA.at(idReservatorioEquivalente).setElemento(7, coeficiente_ENA_lag_7);
							if (coeficiente_ENA_lag_8 != 0.0)
								coeficiente_ENA.at(idReservatorioEquivalente).setElemento(8, coeficiente_ENA_lag_8);
							if (coeficiente_ENA_lag_9 != 0.0)
								coeficiente_ENA.at(idReservatorioEquivalente).setElemento(9, coeficiente_ENA_lag_9);
							if (coeficiente_ENA_lag_10 != 0.0)
								coeficiente_ENA.at(idReservatorioEquivalente).setElemento(10, coeficiente_ENA_lag_10);
							if (coeficiente_ENA_lag_11 != 0.0)
								coeficiente_ENA.at(idReservatorioEquivalente).setElemento(11, coeficiente_ENA_lag_11);
							if (coeficiente_ENA_lag_12 != 0.0)
								coeficiente_ENA.at(idReservatorioEquivalente).setElemento(12, coeficiente_ENA_lag_12);
							coeficiente_GNL.at(idReservatorioEquivalente).at(1).setElemento(1, coeficiente_GNL_pat_1_lag_1);
							coeficiente_GNL.at(idReservatorioEquivalente).at(1).setElemento(2, coeficiente_GNL_pat_1_lag_2);
							coeficiente_GNL.at(idReservatorioEquivalente).at(2).setElemento(1, coeficiente_GNL_pat_2_lag_1);
							coeficiente_GNL.at(idReservatorioEquivalente).at(2).setElemento(2, coeficiente_GNL_pat_2_lag_2);
							coeficiente_GNL.at(idReservatorioEquivalente).at(3).setElemento(1, coeficiente_GNL_pat_3_lag_1);
							coeficiente_GNL.at(idReservatorioEquivalente).at(3).setElemento(2, coeficiente_GNL_pat_3_lag_2);
							coeficientes_Vminop.setElemento(idReservatorioEquivalente, coeficiente_Vminop);

							///////

							if (pos + 1 < a_maior_ONS_REE)//Evita passar a linha quando é o último REE lido (depois no while vai pegar uma nova linha)
								std::getline(leituraArquivo, line);//Passa de linha
						}//for (int pos = 0; pos < 11; pos++) {

						//************************************
						//Construção dos cortes
						//************************************

						//
						// Computa Coeficientes Individualizados
						//

						// Coeficientes VI
						for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {
							const IdVariavelEstado idVariavelEstado = estados_VI.at(idHidreletrica);
							if (idVariavelEstado > IdVariavelEstado_Nenhum) {
								coeficientes_corte.at(IdRealizacao_1).at(idVariavelEstado) = 0.0;
								for (IdReservatorioEquivalente idREE = coeficientes_EAR.getIteradorInicial(); idREE <= coeficientes_EAR.getIteradorFinal(); idREE++)
									coeficientes_corte.at(IdRealizacao_1).at(idVariavelEstado) += coeficientes_EAR.at(idREE) * a_dados.getElementoVetor(idHidreletrica, AttVetorHidreletrica_produtibilidade_acumulada_EAR, idREE, double());
								coeficientes_corte.at(IdRealizacao_1).at(idVariavelEstado) *= conversao_MWporVazao_em_MWhporVolume;

							} // if (idVariavelEstado > IdVariavelEstado_Nenhum) {
						} // for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

						// Variaveis ENA REE
						for (int lag = 1; lag <= ordem_maxima_PAR; lag++) {

							for (IdReservatorioEquivalente idREE = coeficiente_ENA.getIteradorInicial(); idREE <= coeficiente_ENA.getIteradorFinal(); idREE++) {

								const IdVariavelEstado idVariavelEstado = estados_ENA.at(idREE).at(lag);

								if (idVariavelEstado > IdVariavelEstado_Nenhum)
									coeficientes_corte.at(IdRealizacao_1).at(idVariavelEstado) = coeficiente_ENA.at(idREE).at(lag) * numero_horas_estagio_NEWAVE;
							}

						} // for (int lag = 1; lag <= ordem_maxima_PAR; lag++) {

						// Variaveis PTDISPCOM 
						for (IdTermeletrica idUTE = idTermeletricaIni; idUTE < idTermeletricaOut; a_dados.vetorTermeletrica.incr(idUTE)) {

							if (estados_GNL.at(idUTE).size() > 0) {

								const IdSubmercado idSubmercado = a_dados.getAtributo(idUTE, AttComumTermeletrica_submercado, IdSubmercado());

								const IdVariavelEstado idVariavelEstado1 = estados_GNL.at(idUTE).at(idSubmercado).at(1);
								const IdVariavelEstado idVariavelEstado2 = estados_GNL.at(idUTE).at(idSubmercado).at(2);

								coeficientes_corte.at(IdRealizacao_1).at(idVariavelEstado1) = numero_horas_estagio_NEWAVE * (perc_pat1 * coeficiente_GNL.at(mapeamentoSubmercadoxREE.at(idSubmercado)).at(1).at(1) + perc_pat2 * coeficiente_GNL.at(mapeamentoSubmercadoxREE.at(idSubmercado)).at(2).at(1) + perc_pat3 * coeficiente_GNL.at(mapeamentoSubmercadoxREE.at(idSubmercado)).at(3).at(1));
								coeficientes_corte.at(IdRealizacao_1).at(idVariavelEstado2) = numero_horas_estagio_NEWAVE * (perc_pat1 * coeficiente_GNL.at(mapeamentoSubmercadoxREE.at(idSubmercado)).at(1).at(2) + perc_pat2 * coeficiente_GNL.at(mapeamentoSubmercadoxREE.at(idSubmercado)).at(2).at(2) + perc_pat3 * coeficiente_GNL.at(mapeamentoSubmercadoxREE.at(idSubmercado)).at(3).at(2));

							} // if (estados_GNL.at(idUTE).size() > 0) {
						} // for (IdTermeletrica idUTE = idTermeletricaIni; idUTE < idTermeletricaOut; a_dados.vetorTermeletrica.incr(idUTE)) {

						// Variavel ZP0_VF_FINF					
						if (true) {
							const IdVariavelEstado idVariavelEstado_VMINOP = estados.getIteradorFinal();
							coeficientes_corte.at(IdRealizacao_1).at(idVariavelEstado_VMINOP) = 0.0;
							for (IdReservatorioEquivalente idREE = coeficientes_Vminop.getIteradorInicial(); idREE <= coeficientes_Vminop.getIteradorFinal(); idREE++)
								coeficientes_corte.at(IdRealizacao_1).at(idVariavelEstado_VMINOP) += coeficientes_Vminop.at(idREE);
							coeficientes_corte.at(IdRealizacao_1).at(idVariavelEstado_VMINOP) *= numero_horas_estagio_NEWAVE;

						}

						// RHS
						if (true) {

							rhs_corte.at(IdRealizacao_1) = rhs_valor * numero_horas_estagio_NEWAVE;

						}

						SmartEnupla<IdVariavelEstado, double> estados_vazios;

						estagio_pos_estudo.instanciarCorteBenders(rhs_corte, coeficientes_corte, estados_vazios);

					}//if (is_bloco_informacao) {

					//////////////////////////////////////////////////////////////////////////////////
					//Chave para saber que está no bloco de informação
					// Nota: Lembrar que a estrutura do arquivo fcfnwn.rvX e nwlistcf.rel é diferente
					//////////////////////////////////////////////////////////////////////////////////
					if (is_arquivo_fcfnwn) { //Leitura arquivo fcfnwn.rvX
						if (line.substr(3, 10) == simbolo_cabecalho) {
							numero_simbolo_cabecalho += 1;

							if (numero_simbolo_cabecalho == 2)
								is_bloco_informacao = true;

						}//if (line.substr(3, 10) == simbolo_cabecalho) {

					}//if (is_arquivo_fcfnwn) {
					else if (!is_arquivo_fcfnwn) {//Leitura arquivo nwlistcf.rel

						atributo = line.substr(0, 23);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						if (atributo == str_periodo_acoplamento) {
							std::cout << "Encontrado bloco de cortes de acoplamento do " << str_periodo_acoplamento << std::endl;
							is_bloco_informacao = true;

							//Passa duas linhas de cabecalho (ver arquivo nwlistcf.rel)

							////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Arquivo sem formatação fixa: 
							// 1. Nota: vai ser obtida a posição e tamanho do atributo dependendo do tamanho dos campos X-----------------
							////////////////////////////////////////////////////////////////////////////////////////////////////////////

							std::vector<int> pos_estrutura_atributos;

							if (true) {

								std::getline(leituraArquivo, line);

								int contador = 0;
								int numero_atributos = 0;

								while (line.size() > 0) {
									int pos = int(line.find('X'));
									pos_estrutura_atributos.push_back(pos + contador + numero_atributos);

									contador += pos;
									numero_atributos++;

									line = line.substr(pos + 1, int(line.length()));
								}//while (line.size() > 0) {

							}

							////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Arquivo sem formatação fixa: 
							// 2. Nota: identifica os registros com o mnemônico do arquivo
							////////////////////////////////////////////////////////////////////////////////////////////////////////////

							if (true) {

								std::getline(leituraArquivo, line);

								//Instancia posições =-1 
								if (true) {

									pos_RHS = -1;
									pos_idREE = -1;
									pos_Coef_Earm = -1;
									pos_Eafl_lag_1 = -1;
									pos_Eafl_lag_2 = -1;
									pos_Eafl_lag_3 = -1;
									pos_Eafl_lag_4 = -1;
									pos_Eafl_lag_5 = -1;
									pos_Eafl_lag_6 = -1;
									pos_Eafl_lag_7 = -1;
									pos_Eafl_lag_8 = -1;
									pos_Eafl_lag_9 = -1;
									pos_Eafl_lag_10 = -1;
									pos_Eafl_lag_11 = -1;
									pos_Eafl_lag_12 = -1;
									pos_GNL_pat_1_lag_1 = -1;
									pos_GNL_pat_1_lag_2 = -1;
									pos_GNL_pat_2_lag_1 = -1;
									pos_GNL_pat_2_lag_2 = -1;
									pos_GNL_pat_3_lag_1 = -1;
									pos_GNL_pat_3_lag_2 = -1;
									pos_Vminop = -1;

								}//if (true) {

								//Atualiza posições dependendo do estrutura do arquivo
								for (int aux = 0; aux < int(pos_estrutura_atributos.size()) - 1; aux++) {

									//pos e tam para não pegar as posições dos 'X' no arquivo
									int pos = pos_estrutura_atributos.at(aux) + 1;
									int tam = pos_estrutura_atributos.at(aux + 1) - pos_estrutura_atributos.at(aux) - 1;

									atributo = line.substr(pos, tam);
									atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

									if (atributo == "REE") {
										pos_idREE = pos;
										tam_idREE = tam;
									}//if (atributo == "REE") {
									else if (atributo == "RHS") {
										pos_RHS = pos;
										tam_RHS = tam;
									}//else if (atributo == "RHS") {
									else if (atributo == "PIV") {
										pos_Coef_Earm = pos;
										tam_Coef_Earm = tam;
									}//else if (atributo == "PIV") {
									else if (atributo == "PIH(1)") {
										pos_Eafl_lag_1 = pos;
										tam_Eafl_lag_1 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIH(2)") {
										pos_Eafl_lag_2 = pos;
										tam_Eafl_lag_2 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIH(3)") {
										pos_Eafl_lag_3 = pos;
										tam_Eafl_lag_3 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIH(4)") {
										pos_Eafl_lag_4 = pos;
										tam_Eafl_lag_4 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIH(5)") {
										pos_Eafl_lag_5 = pos;
										tam_Eafl_lag_5 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIH(6)") {
										pos_Eafl_lag_6 = pos;
										tam_Eafl_lag_6 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIH(7)") {
										pos_Eafl_lag_7 = pos;
										tam_Eafl_lag_7 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIH(8)") {
										pos_Eafl_lag_8 = pos;
										tam_Eafl_lag_8 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIH(9)") {
										pos_Eafl_lag_9 = pos;
										tam_Eafl_lag_9 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIH(10)") {
										pos_Eafl_lag_10 = pos;
										tam_Eafl_lag_10 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIH(11)") {
										pos_Eafl_lag_11 = pos;
										tam_Eafl_lag_11 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIH(12)") {
										pos_Eafl_lag_12 = pos;
										tam_Eafl_lag_12 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIGTAD(P1L1)") {
										pos_GNL_pat_1_lag_1 = pos;
										tam_GNL_pat_1_lag_1 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIGTAD(P1L2)") {
										pos_GNL_pat_1_lag_2 = pos;
										tam_GNL_pat_1_lag_2 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIGTAD(P2L1)") {
										pos_GNL_pat_2_lag_1 = pos;
										tam_GNL_pat_2_lag_1 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIGTAD(P2L2)") {
										pos_GNL_pat_2_lag_2 = pos;
										tam_GNL_pat_2_lag_2 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIGTAD(P3L1)") {
										pos_GNL_pat_3_lag_1 = pos;
										tam_GNL_pat_3_lag_1 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIGTAD(P3L2)") {
										pos_GNL_pat_3_lag_2 = pos;
										tam_GNL_pat_3_lag_2 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIMX_VMN") {
										pos_Vminop = pos;
										tam_Vminop = tam;
									}//else if (atributo == "PIH(1)") {
									else {

										if (atributo != "IREG" && atributo != "PIMX_SAR")
											throw std::invalid_argument("Nao identificado na leitura de cortes o atributo: " + atributo);

									}//else {

								}//for (int aux = 0; aux < int(pos_estrutura_atributos.size()) - 1; aux++) {

							}//if (true) {

						}//if (atributo == str_periodo_acoplamento) {

					}//else if (!is_arquivo_fcfnwn) {

					//////////////////////////////////////////////////////////////////////////////////

				}//if (line.size() >= 23) {

			}//while (std::getline(leituraArquivo, line)) {

			if ((!is_bloco_informacao) && (!is_arquivo_fcfnwn))
				throw std::invalid_argument("Nao encontrado o bloco de cortes no arquivo nwlistcf.rel para o periodo: " + str_periodo_acoplamento);
			else if ((!is_bloco_informacao) && (is_arquivo_fcfnwn))
				throw std::invalid_argument("Nao encontrado o bloco de cortes no arquivo fcfnwn.rvX");

			////////////////////////////////////////////////////////////////////////

			EntradaSaidaDados entradaSaidaDados;

			entradaSaidaDados.setSeparadorCSV(";");
			entradaSaidaDados.setDiretorioSaida(a_dados.getAtributo(AttComumDados_diretorio_importacao_pos_estudo, std::string()));

			/*
			for (IdHidreletrica idUHE = idHidreletricaIni; idUHE < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idUHE)) {

				if (a_dados.vetorHidreletrica.at(idUHE).vetorReservatorioEquivalente.numObjetos() > 0) {

					for (IdReservatorioEquivalente idREE = a_dados.getMenorId(idUHE, IdReservatorioEquivalente()); idREE <= a_dados.getMaiorId(idUHE, IdReservatorioEquivalente()); a_dados.vetorHidreletrica.at(idUHE).vetorReservatorioEquivalente.incr(idREE)) {

						if (a_dados.getSize1Matriz(idUHE, idREE, AttMatrizReservatorioEquivalente_conversao_ENA_acoplamento_0) > 0) {
							entradaSaidaDados.imprimirArquivoCSV_AttMatriz("HIDRELETRICA_REE_conversao_ENA_acoplamento.csv", idUHE, idREE, a_dados, AttMatrizReservatorioEquivalente_conversao_ENA_acoplamento_0);
							entradaSaidaDados.setAppendArquivo(true);
						}

						if (a_dados.getSize1Matriz(idUHE, idREE, AttMatrizReservatorioEquivalente_conversao_ENA_acoplamento_1) > 0) {
							entradaSaidaDados.imprimirArquivoCSV_AttMatriz("HIDRELETRICA_REE_conversao_ENA_acoplamento.csv", idUHE, idREE, a_dados, AttMatrizReservatorioEquivalente_conversao_ENA_acoplamento_1);
							entradaSaidaDados.setAppendArquivo(true);
						}

					}
				}

			}
			*/

			entradaSaidaDados.setAppendArquivo(false);
			entradaSaidaDados.imprimirArquivoCSV_AttComum("estagio.csv", estagio_pos_estudo, std::vector<AttComumEstagio>{ AttComumEstagio_idEstagio, AttComumEstagio_periodo_otimizacao, AttComumEstagio_selecao_cortes_nivel_dominancia, AttComumEstagio_cortes_multiplos, AttComumEstagio_alpha_CVAR, AttComumEstagio_lambda_CVAR});
			entradaSaidaDados.imprimirArquivoCSV_AttComum("estado.csv", IdVariavelEstado_Nenhum, estagio_pos_estudo);
			entradaSaidaDados.imprimirArquivoCSV_AttVetor("corteBenders_rhs.csv", IdCorteBenders_Nenhum, estagio_pos_estudo, AttVetorCorteBenders_rhs);
			entradaSaidaDados.imprimirArquivoCSV_AttMatriz("corteBenders_coeficientes.csv", IdCorteBenders_Nenhum, estagio_pos_estudo, AttMatrizCorteBenders_coeficiente);

			///////////////////
			entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissas);
			entradaSaidaDados.setAppendArquivo(false);
			for (IdHidreletrica idUHE = idHidreletricaIni; idUHE < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idUHE)) {

				entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_HIDRELETRICA_RESERVATORIO_ENA_AttVetorPremissa.csv", idUHE, IdReservatorio_1, a_dados, AttVetorReservatorio_volume_minimo);
				entradaSaidaDados.setAppendArquivo(true);
				entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_HIDRELETRICA_RESERVATORIO_ENA_AttVetorPremissa.csv", idUHE, IdReservatorio_1, a_dados, AttVetorReservatorio_volume_maximo);
				entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_HIDRELETRICA_RESERVATORIO_ENA_AttVetorPremissa.csv", idUHE, IdReservatorio_1, a_dados, AttVetorReservatorio_poli_cota_volume_0);
				entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_HIDRELETRICA_RESERVATORIO_ENA_AttVetorPremissa.csv", idUHE, IdReservatorio_1, a_dados, AttVetorReservatorio_poli_cota_volume_1);
				entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_HIDRELETRICA_RESERVATORIO_ENA_AttVetorPremissa.csv", idUHE, IdReservatorio_1, a_dados, AttVetorReservatorio_poli_cota_volume_2);
				entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_HIDRELETRICA_RESERVATORIO_ENA_AttVetorPremissa.csv", idUHE, IdReservatorio_1, a_dados, AttVetorReservatorio_poli_cota_volume_3);
				entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_HIDRELETRICA_RESERVATORIO_ENA_AttVetorPremissa.csv", idUHE, IdReservatorio_1, a_dados, AttVetorReservatorio_poli_cota_volume_4);

			}//for (IdHidreletrica idUHE = idHidreletricaIni; idUHE < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idUHE)) {

			entradaSaidaDados.setAppendArquivo(false);
			for (IdHidreletrica idUHE = idHidreletricaIni; idUHE < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idUHE)) {

				entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_HIDRELETRICA_ENA_AttVetorPremissa.csv", idUHE, a_dados, AttVetorHidreletrica_canal_fuga_medio);
				entradaSaidaDados.setAppendArquivo(true);

			}//for (IdHidreletrica idUHE = idHidreletricaIni; idUHE < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idUHE)) {



			//////////////////

			entradaSaidaDados.setDiretorioSaida(a_dados.getAtributo(AttComumDados_diretorio_entrada_dados, std::string()));

			entradaSaidaDados.setAppendArquivo(false);
			for (IdHidreletrica idUHE = idHidreletricaIni; idUHE < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idUHE)) {

				if (a_dados.vetorHidreletrica.at(idUHE).vetorReservatorioEquivalente.numObjetos() > 0) {

					for (IdReservatorioEquivalente idREE = a_dados.getMenorId(idUHE, IdReservatorioEquivalente()); idREE <= a_dados.getMaiorId(idUHE, IdReservatorioEquivalente()); a_dados.vetorHidreletrica.at(idUHE).vetorReservatorioEquivalente.incr(idREE)) {

						if (a_dados.getSizeVetor(idUHE, idREE, AttVetorReservatorioEquivalente_produtibilidade_ENA) > 0) {
							entradaSaidaDados.imprimirArquivoCSV_AttVetor("HIDRELETRICA_REE_AttVetorPremissa_produtibilidade_ENA.csv", idUHE, idREE, a_dados, AttVetorReservatorioEquivalente_produtibilidade_ENA);
							entradaSaidaDados.setAppendArquivo(true);
						}

					}
				}

			}//for (IdHidreletrica idUHE = idHidreletricaIni; idUHE < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idUHE)) {


		}//if (true) {



	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_cortes_NEWAVE(): \n" + std::string(erro.what())); }

}

void LeituraCEPEL::leitura_cortes_NEWAVE_para_dimensionamento(Dados& a_dados, const SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo, SmartEnupla<IdReservatorioEquivalente, bool>& a_coeficientes_EAR, SmartEnupla<IdReservatorioEquivalente, SmartEnupla<int, bool>>& a_coeficiente_ENA, std::string a_nomeArquivo, const bool a_is_arquivo_fcfnwn, const int a_periodo_acoplamento, const int a_maior_ONS_REE) {

	try {

		std::string line;
		std::string atributo;

		std::ifstream leituraArquivo(a_nomeArquivo);

		if (leituraArquivo.is_open()) {

			a_coeficientes_EAR = SmartEnupla<IdReservatorioEquivalente, bool>(IdReservatorioEquivalente(1), std::vector<bool>(IdReservatorioEquivalente(a_maior_ONS_REE), false));
			a_coeficiente_ENA = SmartEnupla<IdReservatorioEquivalente, SmartEnupla<int, bool>>(IdReservatorioEquivalente(1), std::vector<SmartEnupla<int, bool>>(IdReservatorioEquivalente(a_maior_ONS_REE), SmartEnupla<int, bool>(1, std::vector<bool>(12, false))));

			/////////////////////////////////////////
				///////////////////////////////////////////////////////////////////////////////////////
				// Atualiza estrutura de leitura do arquivo de cortes caso seja o arquivo nwlistcf.rel:

			std::ifstream leituraArquivo(a_nomeArquivo);

			////////////////////////////////////////////////////////////////////////////////////
			//Parâmetros para identificar o bloco de informação quando for o arquivo fcfnwn.rvX
			int numero_simbolo_cabecalho = 0;
			std::string simbolo_cabecalho = "X---------";

			////////////////////////////////////////////////////////////////////////////////////
			//Parâmetro para identificar o bloco de informação quando for o arquivo nwlistcf.rel
			std::string str_periodo_acoplamento = "PERIODO:" + getString(a_periodo_acoplamento);

			////////////////////////////////////////////////////////////////////////////////////

			bool is_bloco_informacao = false;

			while (std::getline(leituraArquivo, line)) {

				strNormalizada(line);

				//Leitura dos cortes
				if (line.size() >= 23) {

					if (is_bloco_informacao) {

						////////////////////////////////////////////
						//Critério de parada arquivo nwlistcf.rel
						////////////////////////////////////////////

						atributo = line.substr(0, 9);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						if (atributo == "PERIODO")//Significa que passou para um novo bloco de cortes de um outro período
							break;
						////////////////////////////////////////////

						for (int pos = 0; pos < a_maior_ONS_REE; pos++) {

							//idREE
							atributo = line.substr(pos_idREE, tam_idREE);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const IdReservatorioEquivalente idReservatorioEquivalente = IdReservatorioEquivalente(std::atoi(atributo.c_str()));

							///////////////////////////////
							//Coef.Earm ($/MWh)
							///////////////////////////////
							atributo = line.substr(pos_Coef_Earm, tam_Coef_Earm);
							atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

							const double coeficiente_energia_armazenada = std::atof(atributo.c_str());

							///////////////////////////////
							//Coeficientes para Eafl ($/MWh)
							///////////////////////////////

							//lag_1

							atributo = "";

							if (pos_Eafl_lag_1 > 0) {
								atributo = line.substr(pos_Eafl_lag_1, tam_Eafl_lag_1);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}

							const double coeficiente_ENA_lag_1 = std::atof(atributo.c_str());

							//lag_2

							atributo = "";

							if (pos_Eafl_lag_2 > 0) {
								atributo = line.substr(pos_Eafl_lag_2, tam_Eafl_lag_2);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}

							const double coeficiente_ENA_lag_2 = std::atof(atributo.c_str());

							//lag_3

							atributo = "";

							if (pos_Eafl_lag_3 > 0) {
								atributo = line.substr(pos_Eafl_lag_3, tam_Eafl_lag_3);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}//if (tam_Eafl_lag_3 > 0) {

							const double coeficiente_ENA_lag_3 = std::atof(atributo.c_str());

							//lag_4

							atributo = "";

							if (pos_Eafl_lag_4 > 0) {
								atributo = line.substr(pos_Eafl_lag_4, tam_Eafl_lag_4);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}

							const double coeficiente_ENA_lag_4 = std::atof(atributo.c_str());

							//lag_5

							atributo = "";

							if (pos_Eafl_lag_5 > 0) {
								atributo = line.substr(pos_Eafl_lag_5, tam_Eafl_lag_5);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}

							const double coeficiente_ENA_lag_5 = std::atof(atributo.c_str());

							//lag_6

							atributo = "";

							if (pos_Eafl_lag_6 > 0) {
								atributo = line.substr(pos_Eafl_lag_6, tam_Eafl_lag_6);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}//if (pos_Eafl_lag_6 > 0) {

							const double coeficiente_ENA_lag_6 = std::atof(atributo.c_str());

							//lag_7

							atributo = "";

							if (pos_Eafl_lag_7 > 0) {
								atributo = line.substr(pos_Eafl_lag_7, tam_Eafl_lag_7);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}

							const double coeficiente_ENA_lag_7 = std::atof(atributo.c_str());

							//lag_8

							atributo = "";

							if (pos_Eafl_lag_8 > 0) {
								atributo = line.substr(pos_Eafl_lag_8, tam_Eafl_lag_8);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}

							const double coeficiente_ENA_lag_8 = std::atof(atributo.c_str());

							//lag_9

							atributo = "";

							if (pos_Eafl_lag_9 > 0) {
								atributo = line.substr(pos_Eafl_lag_9, tam_Eafl_lag_9);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}

							const double coeficiente_ENA_lag_9 = std::atof(atributo.c_str());

							//lag_10

							atributo = "";

							if (pos_Eafl_lag_10 > 0) {
								atributo = line.substr(pos_Eafl_lag_10, tam_Eafl_lag_10);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}

							const double coeficiente_ENA_lag_10 = std::atof(atributo.c_str());

							//lag_11

							atributo = "";

							if (pos_Eafl_lag_11 > 0) {
								atributo = line.substr(pos_Eafl_lag_11, tam_Eafl_lag_11);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}

							const double coeficiente_ENA_lag_11 = std::atof(atributo.c_str());

							//lag_12

							atributo = "";

							if (pos_Eafl_lag_12 > 0) {
								atributo = line.substr(pos_Eafl_lag_12, tam_Eafl_lag_12);
								atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());
							}

							const double coeficiente_ENA_lag_12 = std::atof(atributo.c_str());

							/////////////////////////////////
							//Armazena info em SmartEnuplas
							/////////////////////////////////

							if (coeficiente_energia_armazenada != 0.0)
								a_coeficientes_EAR.setElemento(idReservatorioEquivalente, true);

							if (coeficiente_ENA_lag_1 != 0.0)
								a_coeficiente_ENA.at(idReservatorioEquivalente).setElemento(1, true);

							if (coeficiente_ENA_lag_2 != 0.0)
								a_coeficiente_ENA.at(idReservatorioEquivalente).setElemento(2, true);

							if (coeficiente_ENA_lag_3 != 0.0)
								a_coeficiente_ENA.at(idReservatorioEquivalente).setElemento(3, true);

							if (coeficiente_ENA_lag_4 != 0.0)
								a_coeficiente_ENA.at(idReservatorioEquivalente).setElemento(4, true);

							if (coeficiente_ENA_lag_5 != 0.0)
								a_coeficiente_ENA.at(idReservatorioEquivalente).setElemento(5, true);

							if (coeficiente_ENA_lag_6 != 0.0)
								a_coeficiente_ENA.at(idReservatorioEquivalente).setElemento(6, true);

							if (coeficiente_ENA_lag_7 != 0.0)
								a_coeficiente_ENA.at(idReservatorioEquivalente).setElemento(7, true);

							if (coeficiente_ENA_lag_8 != 0.0)
								a_coeficiente_ENA.at(idReservatorioEquivalente).setElemento(8, true);

							if (coeficiente_ENA_lag_9 != 0.0)
								a_coeficiente_ENA.at(idReservatorioEquivalente).setElemento(9, true);

							if (coeficiente_ENA_lag_10 != 0.0)
								a_coeficiente_ENA.at(idReservatorioEquivalente).setElemento(10, true);

							if (coeficiente_ENA_lag_11 != 0.0)
								a_coeficiente_ENA.at(idReservatorioEquivalente).setElemento(11, true);

							if (coeficiente_ENA_lag_12 != 0.0)
								a_coeficiente_ENA.at(idReservatorioEquivalente).setElemento(12, true);

							///////

							if (pos + 1 < a_maior_ONS_REE)//Evita passar a linha quando é o último REE lido (depois no while vai pegar uma nova linha)
								std::getline(leituraArquivo, line);//Passa de linha
						}//for (int pos = 0; pos < 11; pos++) {

					}//if (is_bloco_informacao) {

					//////////////////////////////////////////////////////////////////////////////////
					//Chave para saber que está no bloco de informação
					// Nota: Lembrar que a estrutura do arquivo fcfnwn.rvX e nwlistcf.rel é diferente
					//////////////////////////////////////////////////////////////////////////////////
					if (a_is_arquivo_fcfnwn) { //Leitura arquivo fcfnwn.rvX
						if (line.substr(3, 10) == simbolo_cabecalho) {
							numero_simbolo_cabecalho += 1;

							if (numero_simbolo_cabecalho == 2)
								is_bloco_informacao = true;

						}//if (line.substr(3, 10) == simbolo_cabecalho) {

					}//if (is_arquivo_fcfnwn) {
					else if (!a_is_arquivo_fcfnwn) {//Leitura arquivo nwlistcf.rel

						atributo = line.substr(0, 23);
						atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

						if (atributo == str_periodo_acoplamento) {
							is_bloco_informacao = true;

							//Passa duas linhas de cabecalho (ver arquivo nwlistcf.rel)

							////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Arquivo sem formatação fixa: 
							// 1. Nota: vai ser obtida a posição e tamanho do atributo dependendo do tamanho dos campos X-----------------
							////////////////////////////////////////////////////////////////////////////////////////////////////////////

							std::vector<int> pos_estrutura_atributos;

							if (true) {

								std::getline(leituraArquivo, line);

								int contador = 0;
								int numero_atributos = 0;

								while (line.size() > 0) {
									int pos = int(line.find('X'));
									pos_estrutura_atributos.push_back(pos + contador + numero_atributos);

									contador += pos;
									numero_atributos++;

									line = line.substr(pos + 1, int(line.length()));
								}//while (line.size() > 0) {

							}

							////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//Arquivo sem formatação fixa: 
							// 2. Nota: identifica os registros com o mnemônico do arquivo
							////////////////////////////////////////////////////////////////////////////////////////////////////////////

							if (true) {

								std::getline(leituraArquivo, line);

								//Instancia posições =-1 
								if (true) {

									pos_RHS = -1;
									pos_idREE = -1;
									pos_Coef_Earm = -1;
									pos_Eafl_lag_1 = -1;
									pos_Eafl_lag_2 = -1;
									pos_Eafl_lag_3 = -1;
									pos_Eafl_lag_4 = -1;
									pos_Eafl_lag_5 = -1;
									pos_Eafl_lag_6 = -1;
									pos_Eafl_lag_7 = -1;
									pos_Eafl_lag_8 = -1;
									pos_Eafl_lag_9 = -1;
									pos_Eafl_lag_10 = -1;
									pos_Eafl_lag_11 = -1;
									pos_Eafl_lag_12 = -1;
									pos_GNL_pat_1_lag_1 = -1;
									pos_GNL_pat_1_lag_2 = -1;
									pos_GNL_pat_2_lag_1 = -1;
									pos_GNL_pat_2_lag_2 = -1;
									pos_GNL_pat_3_lag_1 = -1;
									pos_GNL_pat_3_lag_2 = -1;
									pos_Vminop = -1;

								}//if (true) {

								//Atualiza posições dependendo do estrutura do arquivo
								for (int aux = 0; aux < int(pos_estrutura_atributos.size()) - 1; aux++) {

									//pos e tam para não pegar as posições dos 'X' no arquivo
									int pos = pos_estrutura_atributos.at(aux) + 1;
									int tam = pos_estrutura_atributos.at(aux + 1) - pos_estrutura_atributos.at(aux) - 1;

									atributo = line.substr(pos, tam);
									atributo.erase(std::remove(atributo.begin(), atributo.end(), ' '), atributo.end());

									if (atributo == "REE") {
										pos_idREE = pos;
										tam_idREE = tam;
									}//if (atributo == "REE") {
									else if (atributo == "RHS") {
										pos_RHS = pos;
										tam_RHS = tam;
									}//else if (atributo == "RHS") {
									else if (atributo == "PIV") {
										pos_Coef_Earm = pos;
										tam_Coef_Earm = tam;
									}//else if (atributo == "PIV") {
									else if (atributo == "PIH(1)") {
										pos_Eafl_lag_1 = pos;
										tam_Eafl_lag_1 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIH(2)") {
										pos_Eafl_lag_2 = pos;
										tam_Eafl_lag_2 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIH(3)") {
										pos_Eafl_lag_3 = pos;
										tam_Eafl_lag_3 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIH(4)") {
										pos_Eafl_lag_4 = pos;
										tam_Eafl_lag_4 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIH(5)") {
										pos_Eafl_lag_5 = pos;
										tam_Eafl_lag_5 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIH(6)") {
										pos_Eafl_lag_6 = pos;
										tam_Eafl_lag_6 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIH(7)") {
										pos_Eafl_lag_7 = pos;
										tam_Eafl_lag_7 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIH(8)") {
										pos_Eafl_lag_8 = pos;
										tam_Eafl_lag_8 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIH(9)") {
										pos_Eafl_lag_9 = pos;
										tam_Eafl_lag_9 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIH(10)") {
										pos_Eafl_lag_10 = pos;
										tam_Eafl_lag_10 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIH(11)") {
										pos_Eafl_lag_11 = pos;
										tam_Eafl_lag_11 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIH(12)") {
										pos_Eafl_lag_12 = pos;
										tam_Eafl_lag_12 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIGTAD(P1L1)") {
										pos_GNL_pat_1_lag_1 = pos;
										tam_GNL_pat_1_lag_1 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIGTAD(P1L2)") {
										pos_GNL_pat_1_lag_2 = pos;
										tam_GNL_pat_1_lag_2 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIGTAD(P2L1)") {
										pos_GNL_pat_2_lag_1 = pos;
										tam_GNL_pat_2_lag_1 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIGTAD(P2L2)") {
										pos_GNL_pat_2_lag_2 = pos;
										tam_GNL_pat_2_lag_2 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIGTAD(P3L1)") {
										pos_GNL_pat_3_lag_1 = pos;
										tam_GNL_pat_3_lag_1 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIGTAD(P3L2)") {
										pos_GNL_pat_3_lag_2 = pos;
										tam_GNL_pat_3_lag_2 = tam;
									}//else if (atributo == "PIH(1)") {
									else if (atributo == "PIMX_VMN") {
										pos_Vminop = pos;
										tam_Vminop = tam;
									}//else if (atributo == "PIH(1)") {
									else {

										if (atributo != "IREG" && atributo != "PIMX_SAR")
											throw std::invalid_argument("Nao identificado na leitura de cortes o atributo: " + atributo);

									}//else {


								}//for (int aux = 0; aux < int(pos_estrutura_atributos.size()) - 1; aux++) {

							}//if (true) {

						}//if (atributo == str_periodo_acoplamento) {

					}//else if (!is_arquivo_fcfnwn) {

					//////////////////////////////////////////////////////////////////////////////////

				}//if (line.size() >= 23) {

			}//while (std::getline(leituraArquivo, line)) {



		}//if (leituraArquivo.is_open()) {

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_cortes_NEWAVE_para_dimensionamento: \n" + std::string(erro.what())); }

}

double LeituraCEPEL::get_cota_para_conversao_cortes_NEWAVE(Hidreletrica& a_hidreletrica, const SmartEnupla<Periodo, bool> a_horizonte_alvo, const Periodo a_periodo, const double a_percentual_volume_util, const bool a_is_calculo_para_ENA)
{
	try {

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//Premissa 1: Vmax e Vmin não mudam por período para desacoplamento cortes NEWAVE
		// Usinas com regularização mensal utiliza o Vmax, Vmin (hidr.dat)
		// Usinas com regularização semanal/diária utiliza VRef do hidr.dat
		//Quando o volume_maximo = volume_minimo é calculada a cota referencia, ver documento ONS - Submódulo 23.5 - Critérios para estudos hidrológicos 
		//Quando o volume_maximo > volume_minimo é calculada a cota geométrica, ver documento ONS - Submódulo 23.5 - Critérios para estudos hidrológicos
		//////////////////////////////////////////////////////////////////////////////////////////////////////

		double volume_minimo = a_hidreletrica.vetorReservatorio.at(IdReservatorio_1).getElementoVetor(AttVetorReservatorio_volume_minimo, a_periodo, double());
		double volume_maximo = a_hidreletrica.vetorReservatorio.at(IdReservatorio_1).getElementoVetor(AttVetorReservatorio_volume_maximo, a_periodo, double());

		volume_maximo = a_percentual_volume_util * (volume_maximo - volume_minimo) + volume_minimo;

		if (a_is_calculo_para_ENA) //Para o cálculo da produtibilidade para valorar a ENA, utiliza-se a cota referência
			volume_minimo = volume_maximo;

		const TipoRegularizacao tipo_regularizacao = a_hidreletrica.getAtributo(AttComumHidreletrica_tipo_regularizacao, TipoRegularizacao());

		if (tipo_regularizacao == TipoRegularizacao_semanal || tipo_regularizacao == TipoRegularizacao_diaria) {
			volume_minimo = a_hidreletrica.getAtributo(AttComumHidreletrica_volume_referencia, double());
			volume_maximo = a_hidreletrica.getAtributo(AttComumHidreletrica_volume_referencia, double());

		}//if getAtributo(idHidreletrica, AttComumHidreletrica_tipo_regularizacao, TipoRegularizacao()) == TipoRegularizacao_semanal || getAtributo(idHidreletrica, AttComumHidreletrica_tipo_regularizacao, TipoRegularizacao()) == TipoRegularizacao_diaria {


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Premissa 2: Para calcular a produtibilidade do cálculo das ENAs da tendência (períodos anteriores ao horizonte_estudo
		//            são consideradas as modificações de cotas e canal de fuga médio do periodo_inicial_horizonte_estudo
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		const double canal_fuga_medio = a_hidreletrica.getElementoVetor(AttVetorHidreletrica_canal_fuga_medio, a_periodo, double());
		const double cotaMedia = a_hidreletrica.vetorReservatorio.at(IdReservatorio_1).getCotaMedia(a_periodo, volume_minimo, volume_maximo);

		const double cota = cotaMedia - canal_fuga_medio;

		return cota;


	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::get_cota_para_conversao_cortes_NEWAVE: \n" + std::string(erro.what())); }

}

double LeituraCEPEL::get_produtibilidade_para_conversao_cortes_NEWAVE(Hidreletrica& a_hidreletrica, const double a_cota)
{
	try {

		const TipoPerdaHidraulica tipo_perda_hidraulica = a_hidreletrica.getAtributo(AttComumHidreletrica_tipo_de_perda_hidraulica, TipoPerdaHidraulica());
		const double perda_hidraulica = a_hidreletrica.getAtributo(AttComumHidreletrica_perda_hidraulica, double());
		const double fator_de_producao = a_hidreletrica.getAtributo(AttComumHidreletrica_fator_de_producao, double());

		const double produtibilidade = a_hidreletrica.calcularProdutibilidade(tipo_perda_hidraulica, perda_hidraulica, fator_de_producao, a_cota);

		return produtibilidade;

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::get_produtibilidade_para_conversao_cortes_NEWAVE: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::atualizar_vetores_premissas_calculo_produtibilidades(Dados& a_dados, const SmartEnupla<Periodo, bool> a_horizonte_alvo)
{
	try {
		//**************************************************************************************************************
		//Instancia vetores para o cálculo das produtibilidades no escopo temporal do a_horizonte_alvo
		// Nota: Para periodos < periodo_inicial_horizonte_estudo utiliza a info do periodo_inicial_horizonte_estudo
		//       Os atributos setados são os necessários para o cálculo das produtibilidades ENA e EAR
		//**************************************************************************************************************
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());
		const Periodo periodo_inicial_horizonte_estudo = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_horizonte_estudo = horizonte_estudo.getIteradorFinal();

		////////////////////////////
		//AttVetorReservatorio
		////////////////////////////

		SmartEnupla<int, AttVetorReservatorio> attVetorReservatorio;
		SmartEnupla<int, AttComumReservatorio> attComumReservatorio;

		if (true) {
			int pos_elemento = 1;
			attVetorReservatorio.addElemento(pos_elemento, AttVetorReservatorio_volume_minimo);
			attComumReservatorio.addElemento(pos_elemento, AttComumReservatorio_volume_minimo);
			pos_elemento++;
			attVetorReservatorio.addElemento(pos_elemento, AttVetorReservatorio_volume_maximo);
			attComumReservatorio.addElemento(pos_elemento, AttComumReservatorio_volume_maximo);
			pos_elemento++;
			attVetorReservatorio.addElemento(pos_elemento, AttVetorReservatorio_poli_cota_volume_0);
			attComumReservatorio.addElemento(pos_elemento, AttComumReservatorio_poli_cota_volume_0);
			pos_elemento++;
			attVetorReservatorio.addElemento(pos_elemento, AttVetorReservatorio_poli_cota_volume_1);
			attComumReservatorio.addElemento(pos_elemento, AttComumReservatorio_poli_cota_volume_1);
			pos_elemento++;
			attVetorReservatorio.addElemento(pos_elemento, AttVetorReservatorio_poli_cota_volume_2);
			attComumReservatorio.addElemento(pos_elemento, AttComumReservatorio_poli_cota_volume_2);
			pos_elemento++;
			attVetorReservatorio.addElemento(pos_elemento, AttVetorReservatorio_poli_cota_volume_3);
			attComumReservatorio.addElemento(pos_elemento, AttComumReservatorio_poli_cota_volume_3);
			pos_elemento++;
			attVetorReservatorio.addElemento(pos_elemento, AttVetorReservatorio_poli_cota_volume_4);
			attComumReservatorio.addElemento(pos_elemento, AttComumReservatorio_poli_cota_volume_4);
			pos_elemento++;

		}

		////////////////////////////
		//AttVetorHidreletrica
		////////////////////////////

		SmartEnupla<int, AttVetorHidreletrica> attVetorHidreletrica;
		SmartEnupla<int, AttComumHidreletrica> attComumHidreletrica;

		if (true) {
			int pos_elemento = 1;
			attVetorHidreletrica.addElemento(pos_elemento, AttVetorHidreletrica_canal_fuga_medio);
			attComumHidreletrica.addElemento(pos_elemento, AttComumHidreletrica_canal_fuga_medio);
			pos_elemento++;
		}

		////////////////////////////////////////
		//Instancia vetores
		////////////////////////////////////////

		const IdHidreletrica idHidreletricaIni = a_dados.getMenorId(IdHidreletrica());
		const IdHidreletrica idHidreletricaOut = a_dados.getIdOut(IdHidreletrica());

		for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			////////////////////////////
			//AttVetorReservatorio
			////////////////////////////
			if (true) {

				for (int pos = 1; pos <= int(attVetorReservatorio.size()); pos++) {

					if (a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getSizeVetor(attVetorReservatorio.at(pos)) == 0)
						a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(attVetorReservatorio.at(pos), SmartEnupla<Periodo, double>(a_horizonte_alvo, a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getAtributo(attComumReservatorio.at(pos), double())));
					else {

						SmartEnupla<Periodo, double> nova_enupla(a_horizonte_alvo, 0.0);

						for (Periodo periodo_alvo = a_horizonte_alvo.getIteradorInicial(); periodo_alvo <= a_horizonte_alvo.getIteradorFinal(); a_horizonte_alvo.incrementarIterador(periodo_alvo)) {

							//Premissa do setor: períodos anteriores ao periodo_inicial do estudo adotam o valor do periodo_inicial

							if (periodo_alvo >= periodo_inicial_horizonte_estudo) {

								bool is_sobreposicao_encontrada = false;
								double soma_sobreposicao = 0.0;
								double valor_aux = 0.0;
								for (Periodo periodo = periodo_inicial_horizonte_estudo; periodo <= periodo_final_horizonte_estudo; horizonte_estudo.incrementarIterador(periodo)) {

									const double sobreposicao = periodo_alvo.sobreposicao(periodo);

									if (sobreposicao > 0.0) {

										const double valor = a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getElementoVetor(attVetorReservatorio.at(pos), periodo, double());

										if (soma_sobreposicao == 0.0)
											valor_aux = valor;

										if (valor_aux != valor) { throw std::invalid_argument("Os valores no horizonte de estudo com sobreposicao devem ser iguais para o periodo : " + getString(periodo_alvo)); }

										is_sobreposicao_encontrada = true;
										soma_sobreposicao += sobreposicao;
										nova_enupla.at(periodo_alvo) += valor * sobreposicao;
									}//if (sobreposicao > 0.0) {

									if (is_sobreposicao_encontrada && sobreposicao == 0.0) {//Evita percorrer todo o horizonte							
										if (!doubleCompara(1e-6, soma_sobreposicao, 1.0)) { throw std::invalid_argument("Soma sobreposicao deve ser 1.0 para o periodo : " + getString(periodo_alvo)); }
										break;
									}//if (is_sobreposicao_encontrada && sobreposicao == 0.0) {

								}//for (Periodo periodo = periodo_inicial_horizonte_estudo; periodo <= periodo_final_horizonte_estudo; horizonte_estudo.incrementarIterador(periodo)) {

								if (!is_sobreposicao_encontrada)
									throw std::invalid_argument("Nao encontrado no horizonte de estudo o periodo_alvo: " + getString(periodo_alvo));

							}
							else
								nova_enupla.at(periodo_alvo) = a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorio.at(IdReservatorio_1).getElementoVetor(attVetorReservatorio.at(pos), periodo_inicial_horizonte_estudo, double());

						}//for (Periodo periodo = a_horizonte_alvo.getIteradorInicial(); periodo <= a_horizonte_alvo.getIteradorFinal(); a_horizonte_alvo.incrementarIterador(periodo)) {

						a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorio.at(IdReservatorio_1).setVetor(attVetorReservatorio.at(pos), nova_enupla);

					}//else {

				}//for (int pos = 1; pos <= int(attVetorReservatorio.size()); pos++) {

			}//if (true) {

			////////////////////////////
			//AttVetorHidreletrica
			////////////////////////////
			if (true) {

				for (int pos = 1; pos <= int(attVetorHidreletrica.size()); pos++) {

					if (a_dados.vetorHidreletrica.at(idHidreletrica).getSizeVetor(attVetorHidreletrica.at(pos)) == 0)
						a_dados.vetorHidreletrica.at(idHidreletrica).setVetor(attVetorHidreletrica.at(pos), SmartEnupla<Periodo, double>(a_horizonte_alvo, a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(attComumHidreletrica.at(pos), double())));
					else {

						SmartEnupla<Periodo, double> nova_enupla(a_horizonte_alvo, 0.0);

						for (Periodo periodo_alvo = a_horizonte_alvo.getIteradorInicial(); periodo_alvo <= a_horizonte_alvo.getIteradorFinal(); a_horizonte_alvo.incrementarIterador(periodo_alvo)) {

							//Premissa do setor: períodos anteriores ao periodo_inicial do estudo adotam o valor do periodo_inicial

							if (periodo_alvo >= periodo_inicial_horizonte_estudo) {

								bool is_sobreposicao_encontrada = false;
								double soma_sobreposicao = 0.0;
								double valor_aux = 0.0;
								for (Periodo periodo = periodo_inicial_horizonte_estudo; periodo <= periodo_final_horizonte_estudo; horizonte_estudo.incrementarIterador(periodo)) {

									const double sobreposicao = periodo_alvo.sobreposicao(periodo);

									if (sobreposicao > 0.0) {

										const double valor = a_dados.vetorHidreletrica.at(idHidreletrica).getElementoVetor(attVetorHidreletrica.at(pos), periodo, double());

										if (soma_sobreposicao == 0.0)
											valor_aux = valor;

										if (valor_aux != valor) { throw std::invalid_argument("Os valores no horizonte de estudo com sobreposicao devem ser iguais para o periodo : " + getString(periodo_alvo)); }

										is_sobreposicao_encontrada = true;
										soma_sobreposicao += sobreposicao;
										nova_enupla.at(periodo_alvo) += valor * sobreposicao;
									}//if (sobreposicao > 0.0) {

									if (is_sobreposicao_encontrada && sobreposicao == 0.0) {//Evita percorrer todo o horizonte							
										if (!doubleCompara(1e-6, soma_sobreposicao, 1.0)) { throw std::invalid_argument("Soma sobreposicao deve ser 1.0 para o periodo : " + getString(periodo_alvo)); }
										break;
									}//if (is_sobreposicao_encontrada && sobreposicao == 0.0) {

								}//for (Periodo periodo = periodo_inicial_horizonte_estudo; periodo <= periodo_final_horizonte_estudo; horizonte_estudo.incrementarIterador(periodo)) {

								if (!is_sobreposicao_encontrada)
									throw std::invalid_argument("Nao encontrado no horizonte de estudo o periodo_alvo: " + getString(periodo_alvo));

							}
							else
								nova_enupla.at(periodo_alvo) = a_dados.vetorHidreletrica.at(idHidreletrica).getElementoVetor(attVetorHidreletrica.at(pos), periodo_inicial_horizonte_estudo, double());

						}//for (Periodo periodo = a_horizonte_alvo.getIteradorInicial(); periodo <= a_horizonte_alvo.getIteradorFinal(); a_horizonte_alvo.incrementarIterador(periodo)) {

						a_dados.vetorHidreletrica.at(idHidreletrica).setVetor(attVetorHidreletrica.at(pos), nova_enupla);


					}//else {

				}//for (int pos = 1; pos <= int(attVetorReservatorio.size()); pos++) {

			}//if (true) {

		}//for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {


	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::atualizar_vetores_premissas_calculo_produtibilidades: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::calcular_produtibilidade_EAR_acumulada_por_usina(Dados& a_dados, const SmartEnupla<Periodo, bool> a_horizonte_alvo, const int a_maior_ONS_REE)
{
	try {


		SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Calcula produtibilidade_equivalente (cota_media(Vmax, Vmin), canal_fuga_medio) 
		//Premissa: Usinas com regularização mensal utiliza o Vmax, Vmin (hidr.dat + modificações dadger)
		//          Usinas com regularização semanal/diária utiliza VRef do hidr.dat
		// 
		//Nota: A usina 176-COMP PAF-MOX é considerada como TipoDetalhamentoProducaoHidreletrica_sem_producao (não faz parte do estudo CP)
		//porém, é necessário o cálculo da sua produtibilidade para valorar a EAR e ENA do NORDESTE
		///////////////////////////////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////
		//1.1 Cálculo da produtibilidade média para as usinas instanciadas no CP, 
		//seguindo a metodologia ONS - Submódulo 23.5: Critérios para estudos hidrológicos
		// Premissa: Utiliza os atributos do hidr.dat (sem modificações do deck!) -> i.e. a produtibilidade_EAR é estática (AttComum)
		// 
		//1.2 Cálculo da produtibilidade média para as usinas NÃO instanciadas no CP e indicadas no registro JUSENA (p.ex. COMP PAF-MOX)
		// Precisa-se ler o hidr.dat/CadUsH.csv para obter info de vMax, vMin, polinômios, canal_fuga_medio, perdas etc
		//////////////////////////////////////////////////////////////////////////////////

		const IdHidreletrica idHidreletricaIni = a_dados.getMenorId(IdHidreletrica());
		const IdHidreletrica idHidreletricaOut = a_dados.getIdOut(IdHidreletrica());

		//Instancia vetores (caso precisar) para o cálculo das produtibilidades

		//Para as usinas com registro JUSMED no DADGER.dat (p.ex Tucurui, Sto Antonio, Jirau), o cálculo da produtibilidade é realizado com o canal de fuga médio 
		//Ajustar:indicado no JUSMED mas do primeiro período semanal (deveria ser o JUSMED reportado para o período de acoplamento?). Premissa validada com o arquivo de saída energia_acopla.csv do deck DC 01/2023

		for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			if (a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) == TipoDetalhamentoProducaoHidreletrica_sem_producao \
				&& a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(AttComumHidreletrica_codigo_usina, int()) != 176) { //176-COMP PAF-MOX (calcula a produtibilidade_EAR)
				a_dados.vetorHidreletrica.at(idHidreletrica).setAtributo(AttComumHidreletrica_produtibilidade_EAR, 0.0);
			}//if (a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) == TipoDetalhamentoProducaoHidreletrica_sem_producao) {
			else {

				//////////////////////
				//produtibilidade_EAR
				//////////////////////
				if (true) {

					////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Encontra o primeiro periododo horizonte_alvo dentro do horizonte_DECK //Premissa validada com dados do DC
					////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					Periodo periodo_inicial_in_horizonte_estudo = a_horizonte_alvo.getIteradorInicial();
					bool is_sobreposicao_encontrada = false;

					for (Periodo periodo_alvo = a_horizonte_alvo.getIteradorInicial(); periodo_alvo <= a_horizonte_alvo.getIteradorFinal(); a_horizonte_alvo.incrementarIterador(periodo_alvo)) {

						const double sobreposicao = periodo_alvo.sobreposicao(horizonte_estudo.getIteradorInicial());

						if (sobreposicao == 1.0) {
							is_sobreposicao_encontrada = true;
							periodo_inicial_in_horizonte_estudo = periodo_alvo;
						}//if (sobreposicao == 1.0) {
						else if (sobreposicao > 0.0)
							throw std::invalid_argument("Periodo_alvo: " + getString(periodo_alvo) + "deve ser contido em sua totalidade pelo periodo do horizonte de estudo : " + getString(horizonte_estudo.getIteradorInicial()));

					}//for (Periodo periodo_alvo = a_horizonte_alvo.getIteradorInicial(); periodo_alvo <= a_horizonte_alvo.getIteradorFinal(); a_horizonte_alvo.incrementarIterador(periodo_alvo)) {

					if (!is_sobreposicao_encontrada)
						throw std::invalid_argument("Nao encontrado no horizonte_alvo o periodo_inicial do horizonte de estudo");

					////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					const double percentual_volume_util = 1.0; //Para cálculo produtibilidade_EAR

					a_dados.vetorHidreletrica.at(idHidreletrica).setAtributo(AttComumHidreletrica_produtibilidade_EAR, get_produtibilidade_para_conversao_cortes_NEWAVE(a_dados.vetorHidreletrica.at(idHidreletrica), get_cota_para_conversao_cortes_NEWAVE(a_dados.vetorHidreletrica.at(idHidreletrica), a_horizonte_alvo, periodo_inicial_in_horizonte_estudo, percentual_volume_util, false)));
				}//if (true) {

			}//else {

		}//for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Informação das hidrelétricas onde é valorado o volume armazenado em diferentes REEs
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		std::vector<int> codigo_usina_aporte_varios_reservatorios;
		std::vector<std::vector<int>> codigo_ONS_REE_aporte_varios_reservatorios;

		//////

		//155 - Retiro Baixo (aporta em REE 1 - SUDESTE / REE 3 - NORDESTE)
		codigo_usina_aporte_varios_reservatorios.push_back(155);
		codigo_ONS_REE_aporte_varios_reservatorios.push_back(std::vector<int> {1, 3});

		//156 - Três Marias (aporta em REE 1 - SUDESTE / REE 3 - NORDESTE)
		codigo_usina_aporte_varios_reservatorios.push_back(156);
		codigo_ONS_REE_aporte_varios_reservatorios.push_back(std::vector<int> {1, 3});

		//162 - Queimado (aporta em REE 1 - SUDESTE / REE 3 - NORDESTE)
		codigo_usina_aporte_varios_reservatorios.push_back(162);
		codigo_ONS_REE_aporte_varios_reservatorios.push_back(std::vector<int> {1, 3});

		//////

		//57 - Maua (aporta em REE 11 - IGUAÇU / REE 12 - PARANAPANEMA)
		codigo_usina_aporte_varios_reservatorios.push_back(57);
		codigo_ONS_REE_aporte_varios_reservatorios.push_back(std::vector<int> {11, 12});

		//////

		//148 - Irape (aporta em REE 1 - SUDESTE / REE 3 - NORDESTE)
		codigo_usina_aporte_varios_reservatorios.push_back(148);
		codigo_ONS_REE_aporte_varios_reservatorios.push_back(std::vector<int> {1, 3});

		//////

		//251 - Serra Mesa (aporta em REE 1 - SUDESTE / REE 4 - NORTE)
		codigo_usina_aporte_varios_reservatorios.push_back(251);
		codigo_ONS_REE_aporte_varios_reservatorios.push_back(std::vector<int> {1, 4});

		//257 - Peixe Angical (aporta em REE 1 - SUDESTE / REE 4 - NORTE)
		codigo_usina_aporte_varios_reservatorios.push_back(257);
		codigo_ONS_REE_aporte_varios_reservatorios.push_back(std::vector<int> {1, 4});

		////////////////////////////////////////
		//Vetores com as premissas incorporadas
		////////////////////////////////////////

		std::vector<int> codigo_usina_a_montante_desconsidera_Itaipu; //Na valoração da EAR existem usinas a montante de Itaipu que não consideram no cálculo da produtibilidade acumulada a produtibilidade de Itaipu

		codigo_usina_a_montante_desconsidera_Itaipu.push_back(153); //São Domingos - REE 10
		codigo_usina_a_montante_desconsidera_Itaipu.push_back(45);  //Jupiá        - REE 10
		codigo_usina_a_montante_desconsidera_Itaipu.push_back(46);  //P.Primavera  - REE 10
		codigo_usina_a_montante_desconsidera_Itaipu.push_back(63);  //Rosana       - REE 12
		codigo_usina_a_montante_desconsidera_Itaipu.push_back(62);  //Taquaruçu       - REE 12

		std::vector<int> codigo_ONS_REE_considera_Itaipu;
		codigo_ONS_REE_considera_Itaipu.push_back(10); // REE 10 - SUDESTE
		codigo_ONS_REE_considera_Itaipu.push_back(12);  // REE 12 - PARANAPANEMA

		//////////////////////////////
		//1.Usinas dentro do estudo
		//////////////////////////////

		for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			//Instancia produtibilidade_acumulada_EAR x idHidreletrica para todos os REEs
			a_dados.vetorHidreletrica.at(idHidreletrica).setVetor(AttVetorHidreletrica_produtibilidade_acumulada_EAR, SmartEnupla<IdReservatorioEquivalente, double>(IdReservatorioEquivalente(1), std::vector<double>(IdReservatorioEquivalente(a_maior_ONS_REE), 0.0)));

			const int codigo_ONS_REE_alvo = lista_codigo_ONS_REE.getElemento(idHidreletrica);

			if (codigo_ONS_REE_alvo != -1) {

				const int codigo_usina_alvo = a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(AttComumHidreletrica_codigo_usina, int());

				//Determina vetor_REE onde vai ser realizado o cálculo da produtibilidade_acumulada

				std::vector<int> codigos_ONS_REE_aporte_energia{ codigo_ONS_REE_alvo }; //Vetor indicando os REEs onde vai ser calculada a produtibilidade_acumulada: para EAR a usina pode aportar em vários REEs, para ENA a usina só aporta no próprio REE

				for (int pos = 0; pos < int(codigo_usina_aporte_varios_reservatorios.size()); pos++) {

					if (codigo_usina_aporte_varios_reservatorios.at(pos) == codigo_usina_alvo) {//Existem usinas valoradas em diferentes REEs

						////////////////////////
						//Valida que o REE onde pertence a usina esteja registrado em vetor codigo_ONS_REE_aporte_varios_reservatorios

						bool is_codigo_ONS_REE_alvo_in_codigo_ONS_REE_aporte_varios_reservatorios = false;

						for (int aux = 0; aux < int(codigo_ONS_REE_aporte_varios_reservatorios.at(pos).size()); aux++) {

							if (codigo_ONS_REE_aporte_varios_reservatorios.at(pos).at(aux) == codigo_ONS_REE_alvo) {
								is_codigo_ONS_REE_alvo_in_codigo_ONS_REE_aporte_varios_reservatorios = true;
								break;
							}//if (codigo_ONS_REE_aporte_varios_reservatorios.at(pos).at(aux) == codigo_ONS_REE_alvo) {

						}//for (int aux = 0; aux < int(codigo_ONS_REE_aporte_varios_reservatorios.at(pos).size()); aux++) {

						////////////////////////

						if (is_codigo_ONS_REE_alvo_in_codigo_ONS_REE_aporte_varios_reservatorios)
							codigos_ONS_REE_aporte_energia = codigo_ONS_REE_aporte_varios_reservatorios.at(pos);
						else
							throw std::invalid_argument("Nao encontrada codigo_ONS_REE_alvo em vetor codigo_ONS_REE_aporte_varios_reservatorios para a idHidreletrica_" + getString(idHidreletrica));

						break;

					}//if (codigo_usina_aporte_varios_reservatorios.at(pos) == codigo_usina_alvo) {

				}//for (int pos = 0; pos < int(codigo_usina_aporte_varios_reservatorios.size()); pos++) {

				//Determina se deve considerar Itaipu (caso precise na lógica)

				bool is_desconsiderar_Itaipu_no_calculo_produtibilidade_acumulada = false;

				for (int pos = 0; pos < int(codigo_usina_a_montante_desconsidera_Itaipu.size()); pos++) {
					if (codigo_usina_a_montante_desconsidera_Itaipu.at(pos) == codigo_usina_alvo) {
						is_desconsiderar_Itaipu_no_calculo_produtibilidade_acumulada = true;
						break;
					}//if (codigo_usina_a_montante_desconsidera_Itaipu.at(pos) ==  codigo_usina_alvo) {
				}//for (int pos = 0; int(codigo_usina_a_montante_desconsidera_Itaipu.size()); pos++) {

				//////////////////////////////////////////
				//Cálculo produtibilidade_acumulada_EAR
				//Realizado para REE onde a usina tem aporte
				//////////////////////////////////////////

				for (int pos = 0; pos < int(codigos_ONS_REE_aporte_energia.size()); pos++) {

					bool is_REE_encontrado = false; //Identifica desde qual usina começa a soma da produtibilidade_acumulada

					const IdReservatorioEquivalente idReservatorioEquivalente = IdReservatorioEquivalente(codigos_ONS_REE_aporte_energia.at(pos));

					//Determina se deve considerar Itaipu (caso precise na lógica)

					bool is_REE_considerando_Itaipu = false;

					for (int aux = 0; aux < int(codigo_ONS_REE_considera_Itaipu.size()); aux++) {
						if (codigo_ONS_REE_considera_Itaipu.at(aux) == codigos_ONS_REE_aporte_energia.at(pos)) {
							is_REE_considerando_Itaipu = true;
							break;
						}//if (codigo_ONS_REE_considera_Itaipu.at(aux) == codigos_ONS_REE_aporte_energia.at(pos)) {
					}//for (int aux = 0; aux < int(codigo_ONS_REE_considera_Itaipu.size()); aux++) {

					////////////////

					double produtibilidade_acumulada_EAR = 0.0;

					if (codigo_ONS_REE_alvo == codigos_ONS_REE_aporte_energia.at(pos)) {
						produtibilidade_acumulada_EAR += a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(AttComumHidreletrica_produtibilidade_EAR, double());
						is_REE_encontrado = true;
					}//if (codigo_ONS_REE_alvo == codigos_ONS_REE_aporte_energia.at(pos)) {

					IdHidreletrica idHidreletrica_jusante_JUSENA = a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(AttComumHidreletrica_jusante_JUSENA, IdHidreletrica());

					//////////////////////////////////
					//Soma quem estiver a jusante_EAR
					//Soma se estiver no mesmo REE (exceção PARANA -> soma ITAIPU)

					int aux_pos_lista_hidreletrica_out_estudo = -1;

					while (idHidreletrica_jusante_JUSENA != IdHidreletrica_Nenhum) {

						if (lista_codigo_ONS_REE.getElemento(idHidreletrica_jusante_JUSENA) == codigos_ONS_REE_aporte_energia.at(pos))
							is_REE_encontrado = true;

						///////////////////////////////////////////////////////////////////////////////////////////
						//Verifica se está no mesmo REE
						// Premissa: REE PARANÁ (codigo REE: 10) e REE PARANAPANEMA (REE:12) conta a produtibildiade de Itaipu (codigo REE: 5)
						//           a exceção das usinas em vetor codigo_usina_a_montante_desconsidera_Itaipu
						///////////////////////////////////////////////////////////////////////////////////////////

						if (codigos_ONS_REE_aporte_energia.at(pos) != lista_codigo_ONS_REE.getElemento(idHidreletrica_jusante_JUSENA) and is_REE_encontrado and (!is_REE_considerando_Itaipu))
							break;
						else if (codigos_ONS_REE_aporte_energia.at(pos) != lista_codigo_ONS_REE.getElemento(idHidreletrica_jusante_JUSENA) and is_REE_encontrado and is_desconsiderar_Itaipu_no_calculo_produtibilidade_acumulada)
							break;
						else {

							if (is_REE_encontrado) {
								produtibilidade_acumulada_EAR += a_dados.vetorHidreletrica.at(idHidreletrica_jusante_JUSENA).getAtributo(AttComumHidreletrica_produtibilidade_EAR, double());
							}//if (is_REE_encontrado) {

							idHidreletrica_jusante_JUSENA = a_dados.vetorHidreletrica.at(idHidreletrica_jusante_JUSENA).getAtributo(AttComumHidreletrica_jusante_JUSENA, IdHidreletrica());
						}//else {

					}//while (idHidreletrica_jusante_JUSENA != IdHidreletrica_Nenhum) {

					a_dados.vetorHidreletrica.at(idHidreletrica).setElemento(AttVetorHidreletrica_produtibilidade_acumulada_EAR, idReservatorioEquivalente, produtibilidade_acumulada_EAR);

				}//for (int pos = 0; pos < int(codigos_ONS_REE_aporte_energia.size()); pos++) {

			}//if (codigo_ONS_REE_alvo != -1) {

		}//for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::calcular_produtibilidade_EAR_acumulada_por_usina: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::calcular_produtibilidade_ENA_por_usina_por_periodo(Dados& a_dados, const SmartEnupla<Periodo, bool> a_horizonte_alvo)
{
	try {

		const IdHidreletrica idHidreletricaIni = a_dados.getMenorId(IdHidreletrica());
		const IdHidreletrica idHidreletricaOut = a_dados.getIdOut(IdHidreletrica());

		//Para as usinas com registro JUSMED no DADGER.dat (p.ex Tucurui, Sto Antonio, Jirau), o cálculo da produtibilidade é realizado com o canal de fuga médio 
		//Ajustar:indicado no JUSMED mas do primeiro período semanal (deveria ser o JUSMED reportado para o período de acoplamento?). Premissa validada com o arquivo de saída energia_acopla.csv do deck DC 01/2023

		for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			IdReservatorioEquivalente idREE = IdReservatorioEquivalente(lista_codigo_ONS_REE.getElemento(idHidreletrica));

			if (!a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorioEquivalente.isInstanciado(idREE)) {
				ReservatorioEquivalente ree;
				ree.setAtributo(AttComumReservatorioEquivalente_idReservatorioEquivalente, idREE);
				a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorioEquivalente.add(ree);
			}//if (!a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorioEquivalente.isInstanciado(idREE)) {

			///////////////////
			//Nota: Para acoplamento com os cortes NW, o DC estima a ENA com a produtibilidade_ENA do 176 - COMP PAF - MOX (agregação de P.Afonso 123 e 4 e Moxotó). Por tanto:
			// IdHidreletrica_176_COMPPAFMOX -> calcula a produtibilidade_ENA
			// IdHidreletrica_173_MOXOTO -> produtibilidade_ENA = 0
			// IdHidreletrica_174_PAFONSO123 -> produtibilidade_ENA = 0
			// IdHidreletrica_175_PAFONSO4 -> produtibilidade_ENA = 0


			if ((a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) == TipoDetalhamentoProducaoHidreletrica_sem_producao \
				&& a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(AttComumHidreletrica_idHidreletrica, IdHidreletrica()) != IdHidreletrica_176_COMPPAFMOX) \
				|| (a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(AttComumHidreletrica_idHidreletrica, IdHidreletrica()) == IdHidreletrica_173_MOXOTO) \
				|| (a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(AttComumHidreletrica_idHidreletrica, IdHidreletrica()) == IdHidreletrica_174_PAFONSO123)
				|| (a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(AttComumHidreletrica_idHidreletrica, IdHidreletrica()) == IdHidreletrica_175_PAFONSO4)) {
				a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorioEquivalente.at(idREE).setVetor(AttVetorReservatorioEquivalente_produtibilidade_ENA, SmartEnupla<Periodo, double>(a_horizonte_alvo, 0.0));

			}//if (a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) == TipoDetalhamentoProducaoHidreletrica_sem_producao) {
			else {

				//////////////////////
				//produtibilidade_ENA
				//////////////////////
				const double percentual_volume_util = 0.65; //Para cálculo produtibilidade_ENA
				for (Periodo periodo = a_horizonte_alvo.getIteradorInicial(); periodo <= a_horizonte_alvo.getIteradorFinal(); a_horizonte_alvo.incrementarIterador(periodo)) {
					a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorioEquivalente.at(idREE).addElemento(AttVetorReservatorioEquivalente_produtibilidade_ENA, periodo, get_produtibilidade_para_conversao_cortes_NEWAVE(a_dados.vetorHidreletrica.at(idHidreletrica), get_cota_para_conversao_cortes_NEWAVE(a_dados.vetorHidreletrica.at(idHidreletrica), a_horizonte_alvo, periodo, percentual_volume_util, true)));
				}

			}//else {

		}//for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

		//////////////////////////////////////////////////////////////////////////////////
		//Para a valoração das ENAs para as seguintes usinas deve ser somada a produtibilidade_ENA de Itaipu:
		//Ilha Solteira / Três Marias / Capivara
		calcular_produtibilidade_ENA_regras_especiais(a_dados, a_horizonte_alvo);

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::calcular_produtibilidade_ENA_por_usina_por_periodo: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::calcular_produtibilidade_ENA_regras_especiais(Dados& a_dados, const SmartEnupla<Periodo, bool> a_horizonte_tendencia_mais_estudo)
{
	try {

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//1. Para a valoração das ENAs: Soma a produtibilidade_ENA de Itaipu em Ilha_Solteira, Tres_Irmaos e Capivara
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		const int codigo_Itaipu = 66;
		const int codigo_Ilha_Solteira = 34;
		const int codigo_Tres_Irmaos = 43;
		const int codigo_Capivara = 61;

		//////////////////////////////

		const IdHidreletrica idHidreletricaIni = a_dados.getMenorId(IdHidreletrica());
		const IdHidreletrica idHidreletricaOut = a_dados.getIdOut(IdHidreletrica());

		for (Periodo periodo = a_horizonte_tendencia_mais_estudo.getIteradorInicial(); periodo <= a_horizonte_tendencia_mais_estudo.getIteradorFinal(); a_horizonte_tendencia_mais_estudo.incrementarIterador(periodo)) {

			double produtibilidade_ENA_Itaipu = -1.0;

			for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

				if (codigo_Itaipu == a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(AttComumHidreletrica_codigo_usina, int())) {

					IdReservatorioEquivalente idREE = IdReservatorioEquivalente(lista_codigo_ONS_REE.getElemento(idHidreletrica));

					produtibilidade_ENA_Itaipu = a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorioEquivalente.at(idREE).getElementoVetor(AttVetorReservatorioEquivalente_produtibilidade_ENA, periodo, double());
					break;
				}//if (codigo_Itaipu == a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(AttComumHidreletrica_codigo_usina, int())) {

			}//for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			if (produtibilidade_ENA_Itaipu == -1.0) { throw std::invalid_argument("Nao encontrada usina com codigo: " + getString(codigo_Itaipu) + "\n"); }

			/////

			bool is_codigo_Ilha_Solteira_encontrado = false;
			bool is_codigo_Tres_Irmaos_encontrado = false;
			bool is_codigo_Capivara_encontrado = false;

			for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

				IdReservatorioEquivalente idREE = IdReservatorioEquivalente(lista_codigo_ONS_REE.getElemento(idHidreletrica));

				if (codigo_Ilha_Solteira == a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(AttComumHidreletrica_codigo_usina, int())) {
					const double produtibilidade_ENA_antigo = a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorioEquivalente.at(idREE).getElementoVetor(AttVetorReservatorioEquivalente_produtibilidade_ENA, periodo, double());
					const double produtibilidade_ENA_novo = produtibilidade_ENA_antigo + produtibilidade_ENA_Itaipu;
					a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorioEquivalente.at(idREE).setElemento(AttVetorReservatorioEquivalente_produtibilidade_ENA, periodo, produtibilidade_ENA_novo);

					is_codigo_Ilha_Solteira_encontrado = true;
				}//if (codigo_Ilha_Solteira == a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(AttComumHidreletrica_codigo_usina, int())) {

				if (codigo_Tres_Irmaos == a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(AttComumHidreletrica_codigo_usina, int())) {
					const double produtibilidade_ENA_antigo = a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorioEquivalente.at(idREE).getElementoVetor(AttVetorReservatorioEquivalente_produtibilidade_ENA, periodo, double());
					const double produtibilidade_ENA_novo = produtibilidade_ENA_antigo + produtibilidade_ENA_Itaipu;
					a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorioEquivalente.at(idREE).setElemento(AttVetorReservatorioEquivalente_produtibilidade_ENA, periodo, produtibilidade_ENA_novo);

					is_codigo_Tres_Irmaos_encontrado = true;
				}//if (codigo_Tres_Irmaos == a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(AttComumHidreletrica_codigo_usina, int())) {

				if (codigo_Capivara == a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(AttComumHidreletrica_codigo_usina, int())) {
					const double produtibilidade_ENA_antigo = a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorioEquivalente.at(idREE).getElementoVetor(AttVetorReservatorioEquivalente_produtibilidade_ENA, periodo, double());
					const double produtibilidade_ENA_novo = produtibilidade_ENA_antigo + produtibilidade_ENA_Itaipu;
					a_dados.vetorHidreletrica.at(idHidreletrica).vetorReservatorioEquivalente.at(idREE).setElemento(AttVetorReservatorioEquivalente_produtibilidade_ENA, periodo, produtibilidade_ENA_novo);

					is_codigo_Capivara_encontrado = true;
				}//if (codigo_Capivara == a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(AttComumHidreletrica_codigo_usina, int())) {

				if (is_codigo_Ilha_Solteira_encontrado && is_codigo_Tres_Irmaos_encontrado && is_codigo_Capivara_encontrado)
					break;

			}//for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			if (!is_codigo_Ilha_Solteira_encontrado) { throw std::invalid_argument("Nao encontrada usina com codigo: " + getString(codigo_Ilha_Solteira) + "\n"); }
			if (!is_codigo_Tres_Irmaos_encontrado) { throw std::invalid_argument("Nao encontrada usina com codigo: " + getString(codigo_Tres_Irmaos) + "\n"); }
			if (!is_codigo_Capivara_encontrado) { throw std::invalid_argument("Nao encontrada usina com codigo: " + getString(codigo_Capivara) + "\n"); }

		}//for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo)) {

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::calcular_produtibilidade_ENA_regras_especiais: \n" + std::string(erro.what())); }

}

void LeituraCEPEL::defineHidreletricasMontanteNaCascataENA(Dados& a_dados) {

	try {

		const IdHidreletrica idHidreletricaIni = a_dados.getMenorId(IdHidreletrica());
		const IdHidreletrica idHidreletricaOut = a_dados.getIdOut(IdHidreletrica());

		for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			/////////////////////////////////////////////////////////////
			//1. Inicializa vetor usinas_calculo_ENA com o próprio idHidreletrica
			/////////////////////////////////////////////////////////////
			a_dados.vetorHidreletrica.at(idHidreletrica).addElemento(AttVetorHidreletrica_codigo_usinas_calculo_ENA, a_dados.vetorHidreletrica.at(idHidreletrica).getSizeVetor(AttVetorHidreletrica_codigo_usinas_calculo_ENA) + 1, a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(AttComumHidreletrica_codigo_usina, int()));

			/////////////////////////////////////////////////////////////
			//2. Completa vetor usinas_calculo_ENA com todas o idHidreletrica 
			// de TODAS as usinas a montante na cascata
			/////////////////////////////////////////////////////////////
			std::vector<int> vetor_dinamico_controle_codigo_usinas_montante;

			//Inicializa vetor_dinamico_controle_codigo_usinas_montante com as usinas a montante de idHidreletrica

			for (IdHidreletrica idUHE_montante = idHidreletricaIni; idUHE_montante < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idUHE_montante)) {

				const IdHidreletrica idUHE_jusante = a_dados.vetorHidreletrica.at(idUHE_montante).getAtributo(AttComumHidreletrica_jusante, IdHidreletrica());

				if (idUHE_jusante != IdHidreletrica_Nenhum) {

					if (a_dados.vetorHidreletrica.at(idUHE_jusante).getAtributo(AttComumHidreletrica_codigo_usina, int()) == a_dados.vetorHidreletrica.at(idHidreletrica).getAtributo(AttComumHidreletrica_codigo_usina, int()))
						vetor_dinamico_controle_codigo_usinas_montante.push_back(a_dados.vetorHidreletrica.at(idUHE_montante).getAtributo(AttComumHidreletrica_codigo_usina, int()));

				}//if (idUHE_jusante != IdHidreletrica_Nenhum) {

			} // for (IdHidreletrica idUHE_montante = idHidreletricaIni; idUHE_montante < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idUHE_montante)) {

			////////
			//Dinamicamente procura todas as usinas a montante da cascata

			while (int(vetor_dinamico_controle_codigo_usinas_montante.size()) > 0) {

				//Premissa: vai alocando o idHidreletrica da primeira posição do vetor_dinamico_controle_usinas_montante
				a_dados.vetorHidreletrica.at(idHidreletrica).addElemento(AttVetorHidreletrica_codigo_usinas_calculo_ENA, a_dados.vetorHidreletrica.at(idHidreletrica).getSizeVetor(AttVetorHidreletrica_codigo_usinas_calculo_ENA) + 1, vetor_dinamico_controle_codigo_usinas_montante.at(0));

				//Atualiza vetor_dinamico_controle_usinas_montante com as usinas a montante de vetor_dinamico_controle_usinas_montante.at(0)				
				for (IdHidreletrica idUHE_montante = idHidreletricaIni; idUHE_montante < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idUHE_montante)) {

					const IdHidreletrica idUHE_jusante = a_dados.vetorHidreletrica.at(idUHE_montante).getAtributo(AttComumHidreletrica_jusante, IdHidreletrica());

					if (idUHE_jusante != IdHidreletrica_Nenhum) {

						if (a_dados.vetorHidreletrica.at(idUHE_jusante).getAtributo(AttComumHidreletrica_codigo_usina, int()) == vetor_dinamico_controle_codigo_usinas_montante.at(0))
							vetor_dinamico_controle_codigo_usinas_montante.push_back(a_dados.vetorHidreletrica.at(idUHE_montante).getAtributo(AttComumHidreletrica_codigo_usina, int()));

					}//if (idUHE_jusante != IdHidreletrica_Nenhum) {

				} // for (IdHidreletrica idUHE_montante = idHidreletricaIni; idUHE_montante < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idUHE_montante)) {

				//Apaga vetor_dinamico_controle_usinas_montante.at(0)
				vetor_dinamico_controle_codigo_usinas_montante.erase(vetor_dinamico_controle_codigo_usinas_montante.begin());

			}//while (int(vetor_dinamico_controle_codigo_usinas_montante.size()) > 0) {

		} // for (IdHidreletrica idHidreletrica = idHidreletricaIni; idHidreletrica < idHidreletricaOut; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::defineHidreletricasMontanteNaCascataENA: \n" + std::string(erro.what())); }

} // void Dados::defineHidreletricasMontanteNaCascataENA() {

void LeituraCEPEL::retorna_equacionamento_regras_afluencia_natural_x_idHidreletrica(Dados& a_dados, const Periodo a_periodoPE, const SmartEnupla<int, IdHidreletrica>& a_lista_codUsina, const SmartEnupla<int, IdHidreletrica>& a_lista_codPosto, const SmartEnupla<IdHidreletrica, IdVariavelAleatoria>& a_mapIdVar, const SmartEnupla<IdHidreletrica, IdVariavelAleatoriaInterna>& a_mapIdVarInterna, const int a_codigo_posto, const int a_codigo_posto_acoplamento_ENA, const IdHidreletrica a_idHidreletrica, const IdCenario a_idCenario, const Periodo a_periodo_inicial_horizonte_estudo, const Periodo a_periodo, std::vector<SmartEnupla<IdHidreletrica, double>>& a_coeficiente_idHidreletricas_calculo_ENA, double& a_termo_independente_calculo_ENA) {

	try {

		if (a_codigo_posto_acoplamento_ENA == 0) //Usinas que não entram no equcionamento da ENA de forma direta (as afluências destas usinas são utilizadas dentro de regras de outras usinas)
			return;

		//Retorna a composição da afluência natural (como é valorada no NW) da usina com a_idHidreletrica
		//Estrutura em afluência incremental: termo_independiente + coef_a * afluencia_usina_a + coef_b * afluencia_usina_b + ... + coef_n * afluencia_usina_n
		//Notas:
		//(i) Precisa de um termo independente porque existem regras operativas que dada uma condição o valor de afluência a valorar é uma constante (e.g. Belo Monte = 13900 se aflu_posto_288 >= Lim)
		//(ii) os cálculos dos postos para cálculo de ENA de acoplamento podem variar ligeramente com as premissas do arquivos regras.dat, p.ex, Itaipu, Simplicio, Ilha Pombos, Nilo Peçanha, P.Passos
		//     IMPORTANTE no REE-Sudeste: a vazão natural de Funil é contabilizada em Nilo Peçanha -> P.Passos e não em Simplicio -> Ilha Pombos (registro Jusena), i.e., muda regra.dat

		if (a_codigo_posto == a_codigo_posto_acoplamento_ENA) {//Significa que é um posto natural sem regra específica (i.e. a natural valora no NW é composta por todas as incrementais das usinas a montante + a incremental da própria usina)


			const int iterador_inicial = a_dados.vetorHidreletrica.at(a_idHidreletrica).getIteradorInicial(AttVetorHidreletrica_codigo_usinas_calculo_ENA, int());
			const int iterador_final = a_dados.vetorHidreletrica.at(a_idHidreletrica).getIteradorFinal(AttVetorHidreletrica_codigo_usinas_calculo_ENA, int());

			for (int pos = iterador_inicial; pos <= iterador_final; pos++) {
				const IdHidreletrica idHidreletrica_aux = a_lista_codUsina.at(a_dados.vetorHidreletrica.at(a_idHidreletrica).getElementoVetor(AttVetorHidreletrica_codigo_usinas_calculo_ENA, pos, int()));
				a_coeficiente_idHidreletricas_calculo_ENA.push_back(SmartEnupla<IdHidreletrica, double>(idHidreletrica_aux, std::vector<double>(1, 1.0)));
			}

		}//if (a_codigo_posto == a_codigo_posto_acoplamento_ENA) {
		else {//Posto com regra específica para acoplamento com o NW

			const Periodo periodo_ultimo_sobreposicao = get_periodo_ultimo_sobreposicao_com_horizonte_DC(a_dados); //Necessário para as premissas da cascata do Rio São Francisco

			//Ver arquivo REGRAS.DAT do modelo GEVAZP
			//Posto 169 (Sobradinho natural) é uma propagação das vazões de Três Marias e Queimado + Incremental de Sobradinho
			//codigo_posto_acoplamento_ENA == 976 -> Regra para 176-COMP PAF-MOX

			if (a_codigo_posto_acoplamento_ENA == 37) {//Barra Bonita

				//Regra de acoplamento NW (REGRAS.DAT)
				//afluencia = afluencia_237 - 0.1 * (afluencia_161 - afluencia_117 - afluencia_301) - afluencia_117 - afluencia_301;
				//afluencia = afluencia_237 - 0.1 * afluencia_161 - 0.9 * afluencia_117 - 0.9 * afluencia_301;

				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 237, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 161, -0.1, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 117, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 301, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);

			}//else if (codigo_posto_acoplamento_NW == 37) {//Barra Bonita
			else if (a_codigo_posto_acoplamento_ENA == 38) {//BARIRI (A. S. LIMA)

				//Regra de acoplamento NW (REGRAS.DAT)
				//afluencia = afluencia_238 - 0.1 * (afluencia_161 - afluencia_117 - afluencia_301) - afluencia_117 - afluencia_301;
				//afluencia = afluencia_238 - 0.1 * afluencia_161 - 0.9 * afluencia_117 - 0.9 * afluencia_301;

				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 238, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 161, -0.1, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 117, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 301, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);


			}//else if (codigo_posto_acoplamento_NW == 38) {//BARIRI (A. S. LIMA)
			else if (a_codigo_posto_acoplamento_ENA == 39) {//IBITINGA

				//Regra de acoplamento NW (REGRAS.DAT)
				//afluencia = afluencia_239 - 0.1 * (afluencia_161 - afluencia_117 - afluencia_301) - afluencia_117 - afluencia_301;
				//afluencia = afluencia_239 - 0.1 * afluencia_161 - 0.9 * afluencia_117 - 0.9 * afluencia_301;

				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 239, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 161, -0.1, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 117, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 301, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);


			}//else if (codigo_posto_acoplamento_NW == 39) {//IBITINGA
			else if (a_codigo_posto_acoplamento_ENA == 40) {//PROMISSAO

				//Regra de acoplamento NW (REGRAS.DAT)
				//afluencia = afluencia_240 - 0.1 * (afluencia_161 - afluencia_117 - afluencia_301) - afluencia_117 - afluencia_301;
				//afluencia = afluencia_240 - 0.1 * afluencia_161 - 0.9 * afluencia_117 - 0.9 * afluencia_301;

				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 240, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 161, -0.1, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 117, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 301, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);


			}//else if (codigo_posto_acoplamento_NW == 40) {//PROMISSAO
			else if (a_codigo_posto_acoplamento_ENA == 42) {//NAVANHANDAVA

				//Regra de acoplamento NW (REGRAS.DAT)
				//afluencia = afluencia_242 - 0.1 * (afluencia_161 - afluencia_117 - afluencia_301) - afluencia_117 - afluencia_301;
				//afluencia = afluencia_242 - 0.1 * afluencia_161 - 0.9 * afluencia_117 - 0.9 * afluencia_301;

				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 242, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 161, -0.1, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 117, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 301, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);


			}//else if (codigo_posto_acoplamento_NW == 42) {//NAVANHANDAVA
			else if (a_codigo_posto_acoplamento_ENA == 43) {//Três Irmaos

				//Regra de acoplamento NW (REGRAS.DAT)
				//afluencia = afluencia_243 - 0.1 * (afluencia_161 - afluencia_117 - afluencia_301) - afluencia_117 - afluencia_301;
				//afluencia = afluencia_243 - 0.1 * afluencia_161 - 0.9 * afluencia_117 - 0.9 * afluencia_301;

				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 243, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 161, -0.1, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 117, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 301, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);

			}//if (codigo_posto_acoplamento_NW == 43) {//Três Irmaos	
			else if (a_codigo_posto_acoplamento_ENA == 45) {//JUPIA

				//Regra de acoplamento NW (REGRAS.DAT)
				//afluencia = afluencia_245 - 0.1 * (afluencia_161 - afluencia_117 - afluencia_301) - afluencia_117 - afluencia_301;
				//afluencia = afluencia_245 - 0.1 * afluencia_161 - 0.9 * afluencia_117 - 0.9 * afluencia_301;

				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 245, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 161, -0.1, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 117, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 301, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);

			}//else if (codigo_posto_acoplamento_NW == 45) {//JUPIA
			else if (a_codigo_posto_acoplamento_ENA == 46) {//P. PRIMAVERA

				//Regra de acoplamento NW (REGRAS.DAT)
				//afluencia = afluencia_246 - 0.1 * (afluencia_161 - afluencia_117 - afluencia_301) - afluencia_117 - afluencia_301;
				//afluencia = afluencia_246 - 0.1 * afluencia_161 - 0.9 * afluencia_117 - 0.9 * afluencia_301;

				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 246, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 161, -0.1, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 117, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 301, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);

			}//else if (codigo_posto_acoplamento_NW == 46) {//P. PRIMAVERA
			else if (a_codigo_posto_acoplamento_ENA == 66) {//ITAIPU

				/*
				//Regra de acoplamento NW (REGRAS.DAT)
				//afluencia = afluencia_266 - 0.1 * (afluencia_161 - afluencia_117 - afluencia_301) - afluencia_117 - afluencia_301;
				//afluencia = afluencia_266 - 0.1 * afluencia_161 - 0.9 * afluencia_117 - 0.9 * afluencia_301;

				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, 266, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, 161, -0.1, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, 117, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, 301, -0.9, a_coeficiente_idHidreletricas_calculo_ENA);
				*/

				//Regra de acoplamento NW : Atenção: diferente a Regras.dat
				//Aflu_natural_itaipu = Aflu_natural_itaipu - Aflu_natural_I_Solteira - Aflu_natural_Tres_Irmaos - Aflu_natural_Capivara
				//Equivalente a:
				//Aflu_incremental_itaipu = Aflu_incremental_itaipu + Aflu_incremental_jupia + Aflu_incremental_p_primavera + Aflu_incremental_domingos + Aflu_incremental_rosana + Aflu_incremental_taquaruçu

				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 266, 1.0, a_coeficiente_idHidreletricas_calculo_ENA); //Itaipu
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 34, -1.0, a_coeficiente_idHidreletricas_calculo_ENA); //I.Solteira
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 243, -1.0, a_coeficiente_idHidreletricas_calculo_ENA); //Tres Irmaos
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 61, -1.0, a_coeficiente_idHidreletricas_calculo_ENA); //Capivara


			}//else if (codigo_posto_acoplamento_NW == 66) {//ITAIPU
			else if (a_codigo_posto_acoplamento_ENA == 75) {//SEGREDO + DESVIO

				//Regra de acoplamento NW (REGRAS.DAT): VAZ(75) = VAZ(76)+MIN(VAZ(73)-10;173.5)

				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 76, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				const double afluencia_73 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 73, a_idCenario, a_periodo);

				if ((afluencia_73 - 10) < 173.5) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 73, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 10;
				}//if ((afluencia_73 - 10) < 173.5) {
				else
					a_termo_independente_calculo_ENA += 173.5;



			}//else if (codigo_posto_acoplamento_NW == 75) {//SEGREDO + DESVIO
			else if (a_codigo_posto_acoplamento_ENA == 126) {//SIMPLICIO

				//Regra de acoplamento NW (REGRAS.DAT):
				// VAZ(126) = SE(VAZ(127)<=430;MAX(0;VAZ(127)-90);340)+VAZ(123)
				// VAZ(127) = VAZ(129)-vaz(298)-vaz(203)+vaz(304)
				// VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
				// VAZ(304) =vaz(315)-vaz(316)
				// VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				// VAZ(317) = MAX(0;VAZ(201)-25)
				// VAZ(316) = MIN(VAZ(315);190)

				/////////////////////////
				//Postos "reais"
				/////////////////////////
				const double afluencia_129 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 129, a_idCenario, a_periodo);
				const double afluencia_125 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 125, a_idCenario, a_periodo);
				const double afluencia_203 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 203, a_idCenario, a_periodo);
				const double afluencia_201 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 201, a_idCenario, a_periodo);
				const double afluencia_123 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 123, a_idCenario, a_periodo);

				/////////////////////////
				//Postos "fictícios"
				/////////////////////////

				double afluencia_298 = -1.0;

				if (afluencia_125 <= 190)
					afluencia_298 = afluencia_125 * double(119.0 / 190.0);
				else if (afluencia_125 <= 209)
					afluencia_298 = 119;
				else if (afluencia_125 <= 250)
					afluencia_298 = afluencia_125 - 90;
				else
					afluencia_298 = 160;

				///////
				double afluencia_317 = 0;

				if ((afluencia_201 - 25) > afluencia_317)
					afluencia_317 = (afluencia_201 - 25);

				///////
				double afluencia_315 = afluencia_203 - afluencia_201 + afluencia_317 + afluencia_298;

				///////
				double afluencia_316 = 190;

				if (afluencia_315 < afluencia_316)
					afluencia_316 = afluencia_315;

				///////

				double afluencia_304 = afluencia_315 - afluencia_316;

				///////

				double afluencia_127 = afluencia_129 - afluencia_298 - afluencia_203 + afluencia_304;

				///////

				//**********************************************************************
				//VAZ(126) = SE(VAZ(127) <= 430; MAX(0; VAZ(127) - 90); 340) + VAZ(123)
				//**********************************************************************

				// ***Atenção: diferença com regra.dat
				// Substrai VAZ(123): Posto de Funil
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 123, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				//////////
				//VAZ(123)
				//////////
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 123, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				if (afluencia_127 <= 430) {

					if ((afluencia_127 - 90) > 0) {

						a_termo_independente_calculo_ENA -= 90;

						//   VAZ(127) = VAZ(129)-vaz(298)-vaz(203)+vaz(304)
						//   VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
						//   VAZ(304) = vaz(315)-vaz(316)
						//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
						//   VAZ(317) = MAX(0;VAZ(201)-25)
						//   VAZ(316) = MIN(VAZ(315);190)

						////////////
						// +VAZ(129)
						////////////
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 129, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						////////////
						// -VAZ(298)
						////////////

						if (afluencia_125 <= 190)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, -double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
						else if (afluencia_125 <= 209)
							a_termo_independente_calculo_ENA -= 119;
						else if (afluencia_125 <= 250) {
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
							a_termo_independente_calculo_ENA += 90;
						}//else if (afluencia_125 <= 250) {						
						else
							a_termo_independente_calculo_ENA -= 160;



						////////////
						// -VAZ(203)
						////////////
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 203, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						////////////
						// +VAZ(304)
						////////////

						//   VAZ(304) = vaz(315)-vaz(316)
						//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
						//   VAZ(317) = MAX(0;VAZ(201)-25)
						//   VAZ(316) = MIN(VAZ(315);190)

						//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
						// +VAZ(203)

						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 203, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						// -VAZ(201)
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						// +VAZ(317)
						if ((afluencia_201 - 25) > 0) {
							// +VAZ(201)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
							a_termo_independente_calculo_ENA -= 25;

						}//if ((afluencia_201 - 25) > 0) {

						// +VAZ(298)
						if (afluencia_125 <= 190)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
						else if (afluencia_125 <= 209)
							a_termo_independente_calculo_ENA += 119;
						else if (afluencia_125 <= 250) {
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
							a_termo_independente_calculo_ENA -= 90;
						}//else if (afluencia_125 <= 250) {						
						else
							a_termo_independente_calculo_ENA += 160;



						//////////
						//-VAZ(316)
						//VAZ(316) = MIN(VAZ(315); 190)
						//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
						if (afluencia_315 < 190) {

							// -VAZ(203)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 203, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

							// +VAZ(201)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

							// -VAZ(317)
							//   VAZ(317) = MAX(0;VAZ(201)-25)
							if ((afluencia_201 - 25) > 0) {
								// +VAZ(201)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
								a_termo_independente_calculo_ENA += 25;

							}//if ((afluencia_201 - 25) > 0) {

							// -VAZ(298)
							if (afluencia_125 <= 190)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, -double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
							else if (afluencia_125 <= 209)
								a_termo_independente_calculo_ENA -= 119;
							else if (afluencia_125 <= 250) {
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
								a_termo_independente_calculo_ENA += 90;
							}//else if (afluencia_125 <= 250) {						
							else
								a_termo_independente_calculo_ENA -= 160;


						}//if (afluencia_315 < 190) {							
						else
							a_termo_independente_calculo_ENA -= 190;

					}//if ((afluencia_127 - 90) > 0) {

				}//if (afluencia_127 <= 430) {							
				else
					a_termo_independente_calculo_ENA += 340;

			}//else if (codigo_posto_acoplamento_NW == 126) {//SIMPLICIO
			else if (a_codigo_posto_acoplamento_ENA == 131) {//NILO PECANHA

				//Regra de acoplamento NW (REGRAS.DAT): 
				// VAZ(131) = MIN(VAZ(316);144)-VAZ(123)
				// VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
				// VAZ(304) = vaz(315)-vaz(316)
				// VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				// VAZ(317) = MAX(0;VAZ(201)-25)
				// VAZ(316) = MIN(VAZ(315);190)

				/////////////////////////
				//Postos "reais"
				/////////////////////////

				const double afluencia_203 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 203, a_idCenario, a_periodo);
				const double afluencia_201 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 201, a_idCenario, a_periodo);
				const double afluencia_125 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 125, a_idCenario, a_periodo);
				const double afluencia_123 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 123, a_idCenario, a_periodo);

				/////////////////////////
				//Postos "fictícios"
				/////////////////////////

				double afluencia_298 = -1;

				if (afluencia_125 <= 190)
					afluencia_298 = afluencia_125 * double(119.0 / 190.0);
				else if (afluencia_125 <= 209)
					afluencia_298 = 119;
				else if (afluencia_125 <= 250)
					afluencia_298 = afluencia_125 - 90;
				else
					afluencia_298 = 160;

				///////
				double afluencia_317 = 0;

				if ((afluencia_201 - 25) > afluencia_317)
					afluencia_317 = (afluencia_201 - 25);

				///////
				double afluencia_315 = afluencia_203 - afluencia_201 + afluencia_317 + afluencia_298;

				///////
				double afluencia_316 = 190;

				if (afluencia_315 < afluencia_316)
					afluencia_316 = afluencia_315;


				//******************************************
				//VAZ(131) = MIN(VAZ(316); 144) - VAZ(123)
				//******************************************

				// ***Atenção: diferença com regra.dat
				// Soma VAZ(123): Posto de Funil
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 123, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				/////////////
				//   -VAZ(123)
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 123, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				if (afluencia_316 < 144) {

					/////////////
					//   VAZ(316) = MIN(VAZ(315);190)

					if (afluencia_315 < 190) {

						//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
						// +VAZ(203)
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 203, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						// -VAZ(201)
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						// +VAZ(317) 
						//   VAZ(317) = MAX(0;VAZ(201)-25)
						if ((afluencia_201 - 25) > 0) {
							// +VAZ(201)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
							a_termo_independente_calculo_ENA -= 25;

						}//if ((afluencia_201 - 25) > 0) {

						// +VAZ(298)
						//   VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
						if (afluencia_125 <= 190)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
						else if (afluencia_125 <= 209)
							a_termo_independente_calculo_ENA += 119;
						else if (afluencia_125 <= 250) {
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
							a_termo_independente_calculo_ENA -= 90;
						}//else if (afluencia_125 <= 250) {						
						else
							a_termo_independente_calculo_ENA += 160;

					}
					else
						a_termo_independente_calculo_ENA += 190;

				}//if (afluencia_316 < 144) {
				else
					a_termo_independente_calculo_ENA += 144;



			}//else if (codigo_posto_acoplamento_NW == 131) {//NILO PECANHA
			else if (a_codigo_posto_acoplamento_ENA == 299) {//ILHA POMBOS

				//Regra de acoplamento NW (REGRAS.DAT): 
				// VAZ(299) = VAZ(130)-VAZ(298)-VAZ(203)+VAZ(304)+VAZ(123)
				// VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
				// VAZ(304) =vaz(315)-vaz(316)
				// VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				// VAZ(317) = MAX(0;VAZ(201)-25)
				// VAZ(316) = MIN(VAZ(315);190)


				/////////////////////////
				//Postos "reais"
				/////////////////////////

				const double afluencia_130 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 130, a_idCenario, a_periodo);
				const double afluencia_125 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 125, a_idCenario, a_periodo);
				const double afluencia_203 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 203, a_idCenario, a_periodo);
				const double afluencia_201 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 201, a_idCenario, a_periodo);
				const double afluencia_123 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 123, a_idCenario, a_periodo);

				/////////////////////////
				//Postos "fictícios"
				/////////////////////////

				double afluencia_298 = -1;

				if (afluencia_125 <= 190)
					afluencia_298 = afluencia_125 * double(119.0 / 190.0);
				else if (afluencia_125 <= 209)
					afluencia_298 = 119;
				else if (afluencia_125 <= 250)
					afluencia_298 = afluencia_125 - 90;
				else
					afluencia_298 = 160;

				///////
				double afluencia_317 = 0;

				if ((afluencia_201 - 25) > afluencia_317)
					afluencia_317 = (afluencia_201 - 25);

				///////
				double afluencia_315 = afluencia_203 - afluencia_201 + afluencia_317 + afluencia_298;

				///////
				double afluencia_316 = 190;

				if (afluencia_315 < afluencia_316)
					afluencia_316 = afluencia_315;

				///////

				double afluencia_304 = afluencia_315 - afluencia_316;

				//*********************************************************
				// VAZ(299) = VAZ(130)-VAZ(298)-VAZ(203)+VAZ(304)+VAZ(123)
				//*********************************************************

				// ***Atenção: diferença com regra.dat
				// Substrai VAZ(123): Posto de Funil
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 123, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				/////////////
				// +VAZ(130)
				/////////////
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 130, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				/////////////
				// +VAZ(123)
				/////////////
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 123, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				/////////////
				// -VAZ(298)
				/////////////
				if (afluencia_125 <= 190)
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, double(-119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
				else if (afluencia_125 <= 209)
					a_termo_independente_calculo_ENA -= 119;
				else if (afluencia_125 <= 250) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA += 90;
				}//else if (afluencia_125 <= 250) {						
				else
					a_termo_independente_calculo_ENA -= 160;

				/////////////
				// -VAZ(203)
				/////////////
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 203, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				/////////////
				// +VAZ(304)
				/////////////

				//   VAZ(304) = vaz(315)-vaz(316)
				//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				//   VAZ(317) = MAX(0;VAZ(201)-25)
				//   VAZ(316) = MIN(VAZ(315);190)

				//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				// +VAZ(203)
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 203, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				// -VAZ(201)
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				// +VAZ(317)
				if ((afluencia_201 - 25) > 0) {
					// +VAZ(201)
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 25;

				}//if ((afluencia_201 - 25) > 0) {

				// +VAZ(298)
				if (afluencia_125 <= 190)
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
				else if (afluencia_125 <= 209)
					a_termo_independente_calculo_ENA += 119;
				else if (afluencia_125 <= 250) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 90;
				}//else if (afluencia_125 <= 250) {						
				else
					a_termo_independente_calculo_ENA += 160;

				//////////
				//-VAZ(316)
				//VAZ(316) = MIN(VAZ(315); 190)
				//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				if (afluencia_315 < 190) {

					// -VAZ(203)
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 203, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					// +VAZ(201)
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					// -VAZ(317)
					//   VAZ(317) = MAX(0;VAZ(201)-25)
					if ((afluencia_201 - 25) > 0) {
						// +VAZ(201)
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 25;

					}//if ((afluencia_201 - 25) > 0) {

					// -VAZ(298)
					if (afluencia_125 <= 190)
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, double(-119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
					else if (afluencia_125 <= 209)
						a_termo_independente_calculo_ENA -= 119;
					else if (afluencia_125 <= 250) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 90;
					}//else if (afluencia_125 <= 250) {						
					else
						a_termo_independente_calculo_ENA -= 160;
				}//if (afluencia_315 < 190) {
				else
					a_termo_independente_calculo_ENA -= 190;

			}//else if (codigo_posto_acoplamento_NW == 299) {//ILHA POMBOS
			else if (a_codigo_posto_acoplamento_ENA == 303) {//FONTES C

				//Regra de acoplamento NW (REGRAS.DAT): 
				// VAZ(303) = SE(VAZ(132)<17;VAZ(132)+MIN(VAZ(316)-(VAZ(131)+VAZ(123));34);17+MIN(VAZ(316)-(VAZ(131)+VAZ(123));34))
				// VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
				// VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				// VAZ(317) = MAX(0;VAZ(201)-25)
				// VAZ(316) = MIN(VAZ(315);190)
				// VAZ(132) = VAZ(202)+MIN(VAZ(201);25)
				// VAZ(131) = MIN(VAZ(316);144)-VAZ(123)

				/////////////////////////
				//Postos "reais"
				/////////////////////////

				const double afluencia_201 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 201, a_idCenario, a_periodo);
				const double afluencia_202 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 202, a_idCenario, a_periodo);
				const double afluencia_203 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 203, a_idCenario, a_periodo);
				const double afluencia_125 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 125, a_idCenario, a_periodo);
				const double afluencia_123 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 123, a_idCenario, a_periodo);

				/////////////////////////
				//Postos "fictícios"
				/////////////////////////

				double afluencia_298 = -1;

				if (afluencia_125 <= 190)
					afluencia_298 = afluencia_125 * double(119.0 / 190.0);
				else if (afluencia_125 <= 209)
					afluencia_298 = 119;
				else if (afluencia_125 <= 250)
					afluencia_298 = afluencia_125 - 90;
				else
					afluencia_298 = 160;

				///////
				double afluencia_317 = 0;

				if ((afluencia_201 - 25) > afluencia_317)
					afluencia_317 = (afluencia_201 - 25);

				///////
				double afluencia_315 = afluencia_203 - afluencia_201 + afluencia_317 + afluencia_298;

				///////
				double afluencia_316 = 190;

				if (afluencia_315 < afluencia_316)
					afluencia_316 = afluencia_315;


				//////
				double afluencia_132 = 25;

				if (afluencia_201 < afluencia_132)
					afluencia_132 = afluencia_201;

				afluencia_132 += afluencia_202;

				//////
				double afluencia_131 = 144;

				if (afluencia_316 < afluencia_131)
					afluencia_131 = afluencia_316;

				afluencia_131 -= afluencia_123;


				//******************************************************************************************************************
				// VAZ(303) = SE(VAZ(132)<17;VAZ(132)+MIN(VAZ(316)-(VAZ(131)+VAZ(123));34);17+MIN(VAZ(316)-(VAZ(131)+VAZ(123));34))
				//******************************************************************************************************************

				if (afluencia_132 < 17) {

					/////////////
					// +VAZ(132) = VAZ(202)+MIN(VAZ(201);25)
					/////////////

					/////////////
					// +VAZ(202)
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 202, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					if (afluencia_201 < 25) {
						/////////////
						// +VAZ(201)
						/////////////
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
					}//if (afluencia_201 < 25) {
					else
						a_termo_independente_calculo_ENA += 25;

					////////////////////////////////////////
					//MIN(VAZ(316)-(VAZ(131)+VAZ(123));34)
					////////////////////////////////////////
					if ((afluencia_316 - (afluencia_131 + afluencia_123)) < 34) {

						/////////////
						// VAZ(316) = MIN(VAZ(315);190)
						/////////////

						if (afluencia_315 < 190) {

							//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
							// +VAZ(203)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 203, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

							// -VAZ(201)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

							// +VAZ(317) 
							//   VAZ(317) = MAX(0;VAZ(201)-25)
							if ((afluencia_201 - 25) > 0) {
								// +VAZ(201)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
								a_termo_independente_calculo_ENA -= 25;

							}//if ((afluencia_201 - 25) > 0) {

							// +VAZ(298)
							//   VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
							if (afluencia_125 <= 190)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
							else if (afluencia_125 <= 209)
								a_termo_independente_calculo_ENA += 119;
							else if (afluencia_125 <= 250) {
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
								a_termo_independente_calculo_ENA -= 90;
							}//else if (afluencia_125 <= 250) {						
							else
								a_termo_independente_calculo_ENA += 160;

						}
						else
							a_termo_independente_calculo_ENA += 190;

						/////////////
						// -VAZ(131)
						/////////////

						//VAZ(131) = MIN(VAZ(316); 144) - VAZ(123)

						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 123, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						if (afluencia_316 < 144) {

							/////////////
							//   -VAZ(316) = -MIN(VAZ(315);190)

							if (afluencia_315 < 190) {

								//   -VAZ(315) = (-VAZ(203)+VAZ(201))-VAZ(317)-VAZ(298)
								// -VAZ(203)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 203, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

								// +VAZ(201)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

								// -VAZ(317) 
								//   VAZ(317) = MAX(0;VAZ(201)-25)
								if ((afluencia_201 - 25) > 0) {
									// -VAZ(201)
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
									a_termo_independente_calculo_ENA += 25;

								}//if ((afluencia_201 - 25) > 0) {

								// -VAZ(298)
								//   VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
								if (afluencia_125 <= 190)
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, -double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
								else if (afluencia_125 <= 209)
									a_termo_independente_calculo_ENA -= 119;
								else if (afluencia_125 <= 250) {
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
									a_termo_independente_calculo_ENA += 90;
								}//else if (afluencia_125 <= 250) {						
								else
									a_termo_independente_calculo_ENA -= 160;

							}
							else
								a_termo_independente_calculo_ENA -= 190;

						}//if (afluencia_316 < 144) {
						else
							a_termo_independente_calculo_ENA -= 144;

						/////////////
						// -VAZ(123)
						/////////////
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 123, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					}
					else
						a_termo_independente_calculo_ENA += 34;


				}//if (afluencia_132 < 17) {
				else {

					a_termo_independente_calculo_ENA += 17;

					////////////////////////////////////////
					//MIN(VAZ(316)-(VAZ(131)+VAZ(123));34)
					////////////////////////////////////////
					if ((afluencia_316 - (afluencia_131 + afluencia_123)) < 34) {

						/////////////
						// VAZ(316) = MIN(VAZ(315);190)
						/////////////

						if (afluencia_315 < 190) {

							//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
							// +VAZ(203)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 203, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

							// -VAZ(201)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

							// +VAZ(317) 
							//   VAZ(317) = MAX(0;VAZ(201)-25)
							if ((afluencia_201 - 25) > 0) {
								// +VAZ(201)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
								a_termo_independente_calculo_ENA -= 25;

							}//if ((afluencia_201 - 25) > 0) {

							// +VAZ(298)
							//   VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
							if (afluencia_125 <= 190)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
							else if (afluencia_125 <= 209)
								a_termo_independente_calculo_ENA += 119;
							else if (afluencia_125 <= 250) {
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
								a_termo_independente_calculo_ENA -= 90;
							}//else if (afluencia_125 <= 250) {						
							else
								a_termo_independente_calculo_ENA += 160;

						}
						else
							a_termo_independente_calculo_ENA += 190;

						/////////////
						// -VAZ(131)
						/////////////

						//VAZ(131) = MIN(VAZ(316); 144) - VAZ(123)

						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 123, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						if (afluencia_316 < 144) {

							/////////////
							//   -VAZ(316) = -MIN(VAZ(315);190)

							if (afluencia_315 < 190) {

								//   -VAZ(315) = (-VAZ(203)+VAZ(201))-VAZ(317)-VAZ(298)
								// -VAZ(203)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 203, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

								// +VAZ(201)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

								// -VAZ(317) 
								//   VAZ(317) = MAX(0;VAZ(201)-25)
								if ((afluencia_201 - 25) > 0) {
									// -VAZ(201)
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
									a_termo_independente_calculo_ENA += 25;

								}//if ((afluencia_201 - 25) > 0) {

								// -VAZ(298)
								//   VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
								if (afluencia_125 <= 190)
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, -double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
								else if (afluencia_125 <= 209)
									a_termo_independente_calculo_ENA -= 119;
								else if (afluencia_125 <= 250) {
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
									a_termo_independente_calculo_ENA += 90;
								}//else if (afluencia_125 <= 250) {						
								else
									a_termo_independente_calculo_ENA -= 160;

							}
							else
								a_termo_independente_calculo_ENA -= 190;

						}//if (afluencia_316 < 144) {
						else
							a_termo_independente_calculo_ENA -= 144;

						/////////////
						// -VAZ(123)
						/////////////
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 123, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					}
					else
						a_termo_independente_calculo_ENA += 34;

				}//else {

			}//else if (codigo_posto_acoplamento_NW == 303) {//FONTES C
			else if (a_codigo_posto_acoplamento_ENA == 306) {//P.PASSOS

				//Regra de acoplamento NW (REGRAS.DAT): 
				// VAZ(306) = VAZ(303)+VAZ(131)
				// VAZ(303) = SE(VAZ(132)<17;VAZ(132)+MIN(VAZ(316)-(VAZ(131)+VAZ(123));34);17+MIN(VAZ(316)-(VAZ(131)+VAZ(123));34))
				// VAZ(132) = VAZ(202)+MIN(VAZ(201);25)
				// VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
				// VAZ(304) =vaz(315)-vaz(316)
				// VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				// VAZ(317) = MAX(0;VAZ(201)-25)
				// VAZ(316) = MIN(VAZ(315);190)
				// VAZ(131) = MIN(VAZ(316);144)-VAZ(123)

				/////////////////////////
				//Postos "reais"
				/////////////////////////

				const double afluencia_201 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 201, a_idCenario, a_periodo);
				const double afluencia_202 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 202, a_idCenario, a_periodo);
				const double afluencia_203 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 203, a_idCenario, a_periodo);
				const double afluencia_123 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 123, a_idCenario, a_periodo);
				const double afluencia_125 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 125, a_idCenario, a_periodo);

				/////////////////////////
				//Postos "fictícios"
				/////////////////////////

				double afluencia_298 = -1;

				if (afluencia_125 <= 190)
					afluencia_298 = afluencia_125 * double(119.0 / 190.0);
				else if (afluencia_125 <= 209)
					afluencia_298 = 119;
				else if (afluencia_125 <= 250)
					afluencia_298 = afluencia_125 - 90;
				else
					afluencia_298 = 160;

				///////
				double afluencia_317 = 0;

				if ((afluencia_201 - 25) > afluencia_317)
					afluencia_317 = (afluencia_201 - 25);

				///////
				double afluencia_315 = afluencia_203 - afluencia_201 + afluencia_317 + afluencia_298;

				///////
				double afluencia_316 = 190;

				if (afluencia_315 < afluencia_316)
					afluencia_316 = afluencia_315;

				///////

				double afluencia_132 = 25;

				if (afluencia_201 < afluencia_132)
					afluencia_132 = afluencia_201;

				afluencia_132 += afluencia_202;

				//////
				double afluencia_131 = 144;

				if (afluencia_316 < afluencia_131)
					afluencia_131 = afluencia_316;

				afluencia_131 -= afluencia_123;

				//*******************************
				// VAZ(306) = VAZ(303)+VAZ(131)
				//*******************************

				// ***Atenção: diferença com regra.dat
				// Soma VAZ(123): Posto de Funil
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 123, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);


				/////////////
				//VAZ(303) = SE(VAZ(132)<17;VAZ(132)+MIN(VAZ(316)-(VAZ(131)+VAZ(123));34);17+MIN(VAZ(316)-(VAZ(131)+VAZ(123));34))
				/////////////
				if (afluencia_132 < 17) {

					/////////////
					// +VAZ(132) = VAZ(202)+MIN(VAZ(201);25)
					/////////////

					/////////////
					// +VAZ(202)
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 202, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					if (afluencia_201 < 25) {
						/////////////
						// +VAZ(201)
						/////////////
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
					}//if (afluencia_201 < 25) {
					else
						a_termo_independente_calculo_ENA += 25;

					////////////////////////////////////////
					//MIN(VAZ(316)-(VAZ(131)+VAZ(123));34)
					////////////////////////////////////////
					if ((afluencia_316 - (afluencia_131 + afluencia_123)) < 34) {

						/////////////
						// VAZ(316) = MIN(VAZ(315);190)
						/////////////

						if (afluencia_315 < 190) {

							//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
							// +VAZ(203)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 203, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

							// -VAZ(201)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

							// +VAZ(317) 
							//   VAZ(317) = MAX(0;VAZ(201)-25)
							if ((afluencia_201 - 25) > 0) {
								// +VAZ(201)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
								a_termo_independente_calculo_ENA -= 25;

							}//if ((afluencia_201 - 25) > 0) {

							// +VAZ(298)
							//   VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
							if (afluencia_125 <= 190)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
							else if (afluencia_125 <= 209)
								a_termo_independente_calculo_ENA += 119;
							else if (afluencia_125 <= 250) {
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
								a_termo_independente_calculo_ENA -= 90;
							}//else if (afluencia_125 <= 250) {						
							else
								a_termo_independente_calculo_ENA += 160;

						}
						else
							a_termo_independente_calculo_ENA += 190;

						/////////////
						// -VAZ(131)
						/////////////

						//VAZ(131) = MIN(VAZ(316); 144) - VAZ(123)

						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 123, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						if (afluencia_316 < 144) {

							/////////////
							//   -VAZ(316) = -MIN(VAZ(315);190)

							if (afluencia_315 < 190) {

								//   -VAZ(315) = (-VAZ(203)+VAZ(201))-VAZ(317)-VAZ(298)
								// -VAZ(203)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 203, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

								// +VAZ(201)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

								// -VAZ(317) 
								//   VAZ(317) = MAX(0;VAZ(201)-25)
								if ((afluencia_201 - 25) > 0) {
									// -VAZ(201)
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
									a_termo_independente_calculo_ENA += 25;

								}//if ((afluencia_201 - 25) > 0) {

								// -VAZ(298)
								//   VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
								if (afluencia_125 <= 190)
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, -double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
								else if (afluencia_125 <= 209)
									a_termo_independente_calculo_ENA -= 119;
								else if (afluencia_125 <= 250) {
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
									a_termo_independente_calculo_ENA += 90;
								}//else if (afluencia_125 <= 250) {						
								else
									a_termo_independente_calculo_ENA -= 160;

							}
							else
								a_termo_independente_calculo_ENA -= 190;

						}//if (afluencia_316 < 144) {
						else
							a_termo_independente_calculo_ENA -= 144;

						/////////////
						// -VAZ(123)
						/////////////
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 123, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					}
					else
						a_termo_independente_calculo_ENA += 34;


				}//if (afluencia_132 < 17) {
				else {

					a_termo_independente_calculo_ENA += 17;

					////////////////////////////////////////
					//MIN(VAZ(316)-(VAZ(131)+VAZ(123));34)
					////////////////////////////////////////
					if ((afluencia_316 - (afluencia_131 + afluencia_123)) < 34) {

						/////////////
						// VAZ(316) = MIN(VAZ(315);190)
						/////////////

						if (afluencia_315 < 190) {

							//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
							// +VAZ(203)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 203, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

							// -VAZ(201)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

							// +VAZ(317) 
							//   VAZ(317) = MAX(0;VAZ(201)-25)
							if ((afluencia_201 - 25) > 0) {
								// +VAZ(201)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
								a_termo_independente_calculo_ENA -= 25;

							}//if ((afluencia_201 - 25) > 0) {

							// +VAZ(298)
							//   VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
							if (afluencia_125 <= 190)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
							else if (afluencia_125 <= 209)
								a_termo_independente_calculo_ENA += 119;
							else if (afluencia_125 <= 250) {
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
								a_termo_independente_calculo_ENA -= 90;
							}//else if (afluencia_125 <= 250) {						
							else
								a_termo_independente_calculo_ENA += 160;

						}
						else
							a_termo_independente_calculo_ENA += 190;

						/////////////
						// -VAZ(131)
						/////////////

						//VAZ(131) = MIN(VAZ(316); 144) - VAZ(123)

						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 123, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						if (afluencia_316 < 144) {

							/////////////
							//   -VAZ(316) = -MIN(VAZ(315);190)

							if (afluencia_315 < 190) {

								//   -VAZ(315) = (-VAZ(203)+VAZ(201))-VAZ(317)-VAZ(298)
								// -VAZ(203)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 203, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

								// +VAZ(201)
								retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

								// -VAZ(317) 
								//   VAZ(317) = MAX(0;VAZ(201)-25)
								if ((afluencia_201 - 25) > 0) {
									// -VAZ(201)
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
									a_termo_independente_calculo_ENA += 25;

								}//if ((afluencia_201 - 25) > 0) {

								// -VAZ(298)
								//   VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
								if (afluencia_125 <= 190)
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, -double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
								else if (afluencia_125 <= 209)
									a_termo_independente_calculo_ENA -= 119;
								else if (afluencia_125 <= 250) {
									retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
									a_termo_independente_calculo_ENA += 90;
								}//else if (afluencia_125 <= 250) {						
								else
									a_termo_independente_calculo_ENA -= 160;

							}
							else
								a_termo_independente_calculo_ENA -= 190;

						}//if (afluencia_316 < 144) {
						else
							a_termo_independente_calculo_ENA -= 144;

						/////////////
						// -VAZ(123)
						/////////////
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 123, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					}
					else
						a_termo_independente_calculo_ENA += 34;

				}//else {

				/////////////
				//VAZ(131) = MIN(VAZ(316);144)-VAZ(123)
				/////////////

				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 123, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				if (afluencia_316 < 144) {

					/////////////
					//   VAZ(316) = MIN(VAZ(315);190)

					if (afluencia_315 < 190) {

						//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
						// +VAZ(203)
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 203, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						// -VAZ(201)
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

						// +VAZ(317) 
						//   VAZ(317) = MAX(0;VAZ(201)-25)
						if ((afluencia_201 - 25) > 0) {
							// +VAZ(201)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
							a_termo_independente_calculo_ENA -= 25;

						}//if ((afluencia_201 - 25) > 0) {

						// +VAZ(298)
						//   VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
						if (afluencia_125 <= 190)
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
						else if (afluencia_125 <= 209)
							a_termo_independente_calculo_ENA += 119;
						else if (afluencia_125 <= 250) {
							retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
							a_termo_independente_calculo_ENA -= 90;
						}//else if (afluencia_125 <= 250) {						
						else
							a_termo_independente_calculo_ENA += 160;

					}
					else
						a_termo_independente_calculo_ENA += 190;

				}//if (afluencia_316 < 144) {
				else
					a_termo_independente_calculo_ENA += 144;



			}//else if (codigo_posto_acoplamento_NW == 306) {//P.PASSOS
			else if (a_codigo_posto_acoplamento_ENA == 318) {//HENRY BORDEN

				//Regra de acoplamento NW (REGRAS.DAT): 
				// VAZ(318) = VAZ(116)+0.1*(VAZ(161)-VAZ(117)-VAZ(301))+VAZ(117)+VAZ(301)
				// VAZ(116) = VAZ(119)-VAZ(301)
				//POST    MES  CONF    FORMULA
				//	XXX    XX     X    XXXXXXXXXXXXXXXXXXXXXXXXXXX
				//	119     1          VAZ(301) * 1.217 + 0.608
				//	119     2          VAZ(301) * 1.232 + 0.123
				//	119     3          VAZ(301) * 1.311 - 2.359
				//	119     4          VAZ(301) * 1.241 - 0.496
				//	119     5          VAZ(301) * 1.167 + 0.467
				//	119     6          VAZ(301) * 1.333 - 0.533
				//	119     7          VAZ(301) * 1.247 - 0.374
				//	119     8          VAZ(301) * 1.2 + 0.36
				//	119     9          VAZ(301) * 1.292 - 1.292
				//	119    10          VAZ(301) * 1.25 - 0.25
				//	119    11          VAZ(301) * 1.294 - 1.682
				//	119    12          VAZ(301) * 1.215 + 0.729

				/////////////////////////
				//Postos "reais"
				/////////////////////////

				const double afluencia_161 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 161, a_idCenario, a_periodo);
				const double afluencia_117 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 117, a_idCenario, a_periodo);
				const double afluencia_301 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 301, a_idCenario, a_periodo);


				bool is_periodo_inicial = false;
				if (a_periodo == a_periodo_inicial_horizonte_estudo)
					is_periodo_inicial = true;

				const IdMes idMes_alvo = get_IdMes_operativo(a_periodo, is_periodo_inicial);

				//*************************************************************************
				// VAZ(318) = VAZ(116)+0.1*(VAZ(161)-VAZ(117)-VAZ(301))+VAZ(117)+VAZ(301)
				// VAZ(318) = VAZ(116)+0.1*VAZ(161)+0.9*VAZ(117)+0.9*VAZ(301)
				//*************************************************************************

				///////////
				// VAZ(116) = VAZ(119)-VAZ(301)
				///////////

				//////////
				//VAZ(119)
				//////////

				if (idMes_alvo == IdMes_1) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 301, 1.217, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA += 0.608;
				}//if (idMes_alvo == IdMes_1) {
				else if (idMes_alvo == IdMes_2) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 301, 1.232, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA += 0.123;
				}//else if (idMes_alvo == IdMes_2) {
				else if (idMes_alvo == IdMes_3) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 301, 1.311, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 2.359;
				}//else if (idMes_alvo == IdMes_3) {					
				else if (idMes_alvo == IdMes_4) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 301, 1.241, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 0.496;
				}//else if (idMes_alvo == IdMes_4) {					
				else if (idMes_alvo == IdMes_5) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 301, 1.167, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA += 0.467;
				}//else if (idMes_alvo == IdMes_5) {					
				else if (idMes_alvo == IdMes_6) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 301, 1.333, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 0.533;
				}//else if (idMes_alvo == IdMes_6) {					
				else if (idMes_alvo == IdMes_7) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 301, 1.247, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 0.374;
				}//else if (idMes_alvo == IdMes_7) {					
				else if (idMes_alvo == IdMes_8) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 301, 1.200, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA += 0.360;
				}//else if (idMes_alvo == IdMes_8) {					
				else if (idMes_alvo == IdMes_9) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 301, 1.292, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 1.292;
				}//else if (idMes_alvo == IdMes_9) {					
				else if (idMes_alvo == IdMes_10) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 301, 1.250, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 0.250;
				}//else if (idMes_alvo == IdMes_10) {				
				else if (idMes_alvo == IdMes_11) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 301, 1.294, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 1.682;
				}//else if (idMes_alvo == IdMes_11) {					
				else if (idMes_alvo == IdMes_12) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 301, 1.215, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA += 0.729;
				}//else if (idMes_alvo == IdMes_12) {					
				else { throw std::invalid_argument("Nao identificado idMes: " + getString(idMes_alvo) + " \n"); }

				//////////
				//-VAZ(301)
				//////////
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 301, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				//////////////////////////

				///////////
				// 0.1 * VAZ(161)
				///////////
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 161, 0.1, a_coeficiente_idHidreletricas_calculo_ENA);

				///////////
				// 0.9 * VAZ(117)
				///////////
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 117, 0.9, a_coeficiente_idHidreletricas_calculo_ENA);

				///////////
				// 0.9 * VAZ(301)
				///////////
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 301, 0.9, a_coeficiente_idHidreletricas_calculo_ENA);


			}//else if (codigo_posto_acoplamento_NW == 318) {//HENRY BORDEN
			else if (a_codigo_posto_acoplamento_ENA == 304) {//SANTANA


				//Regra de acoplamento NW (REGRAS.DAT): 
				// VAZ(304) = vaz(315)-vaz(316)
				// VAZ(298) = SE(VAZ(125)<=190;(VAZ(125)*119)/190;SE(VAZ(125)<=209;119;SE(VAZ(125)<=250;VAZ(125)-90;160)))
				// VAZ(304) =vaz(315)-vaz(316)
				// VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				// VAZ(317) = MAX(0;VAZ(201)-25)
				// VAZ(316) = MIN(VAZ(315);190)

				/////////////////////////
				//Postos "reais"
				/////////////////////////

				const double afluencia_203 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 203, a_idCenario, a_periodo);
				const double afluencia_201 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 201, a_idCenario, a_periodo);
				const double afluencia_125 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 125, a_idCenario, a_periodo);

				/////////////////////////
				//Postos "fictícios"
				/////////////////////////

				double afluencia_298 = -1;

				if (afluencia_125 <= 190)
					afluencia_298 = afluencia_125 * double(119.0 / 190.0);
				else if (afluencia_125 <= 209)
					afluencia_298 = 119;
				else if (afluencia_125 <= 250)
					afluencia_298 = afluencia_125 - 90;
				else
					afluencia_298 = 160;

				///////
				double afluencia_317 = 0;

				if ((afluencia_201 - 25) > afluencia_317)
					afluencia_317 = (afluencia_201 - 25);

				///////
				double afluencia_315 = afluencia_203 - afluencia_201 + afluencia_317 + afluencia_298;

				//***************************************
				//VAZ(304) = vaz(315) - vaz(316)
				//***************************************

				//   VAZ(304) = vaz(315)-vaz(316)
				//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				//   VAZ(317) = MAX(0;VAZ(201)-25)
				//   VAZ(316) = MIN(VAZ(315);190)

				//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				// +VAZ(203)
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 203, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				// -VAZ(201)
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				// +VAZ(317)
				if ((afluencia_201 - 25) > 0) {
					// +VAZ(201)
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 25;

				}//if ((afluencia_201 - 25) > 0) {

				// +VAZ(298)
				if (afluencia_125 <= 190)
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
				else if (afluencia_125 <= 209)
					a_termo_independente_calculo_ENA += 119;
				else if (afluencia_125 <= 250) {
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
					a_termo_independente_calculo_ENA -= 90;
				}//else if (afluencia_125 <= 250) {						
				else
					a_termo_independente_calculo_ENA += 160;

				//////////
				//-VAZ(316)
				//VAZ(316) = MIN(VAZ(315); 190)
				//   VAZ(315) = (VAZ(203)-VAZ(201))+VAZ(317)+VAZ(298)
				if (afluencia_315 < 190) {

					// -VAZ(203)
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 203, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					// +VAZ(201)
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					// -VAZ(317)
					//   VAZ(317) = MAX(0;VAZ(201)-25)
					if ((afluencia_201 - 25) > 0) {
						// +VAZ(201)
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 25;

					}//if ((afluencia_201 - 25) > 0) {

					// -VAZ(298)
					if (afluencia_125 <= 190)
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, -double(119.0 / 190.0), a_coeficiente_idHidreletricas_calculo_ENA);
					else if (afluencia_125 <= 209)
						a_termo_independente_calculo_ENA -= 119;
					else if (afluencia_125 <= 250) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 125, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 90;
					}//else if (afluencia_125 <= 250) {						
					else
						a_termo_independente_calculo_ENA -= 160;


				}//if (afluencia_315 < 190) {							
				else
					a_termo_independente_calculo_ENA -= 190;


			}//else if (codigo_posto_acoplamento_NW == 304) {//SANTANA
			else if (a_codigo_posto_acoplamento_ENA == 132) {//LAJES

				//Regra de acoplamento NW (REGRAS.DAT): 
				//VAZ(132) = VAZ(202)+MIN(VAZ(201);25)

				/////////////////////////
				//Postos "reais"
				/////////////////////////

				const double afluencia_202 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 202, a_idCenario, a_periodo);
				const double afluencia_201 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 201, a_idCenario, a_periodo);

				////////////
				// +VAZ(202)
				////////////
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 202, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				if (afluencia_201 < 25) {

					////////////
					// +VAZ(201)
					////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 201, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				}//if (afluencia_201 < 25) {
				else
					a_termo_independente_calculo_ENA += 25;


			}//else if (codigo_posto_acoplamento_NW == 132) {//LAJES
			else if (a_codigo_posto_acoplamento_ENA == 3) {//GUARAPIRANGA

				//Regra de acoplamento NW (REGRAS.DAT): VAZ(3) = 0
				a_termo_independente_calculo_ENA += 0.0;

			}//else if (codigo_posto_acoplamento_NW == 3) {//GUARAPIRANGA
			else if (a_codigo_posto_acoplamento_ENA == 4) {//BILLINGS

				//Regra de acoplamento NW (REGRAS.DAT): VAZ(4) = 0
				a_termo_independente_calculo_ENA += 0.0;

			}//else if (codigo_posto_acoplamento_NW == 4) {//BILLINGS
			else if (a_codigo_posto_acoplamento_ENA == 5) {//PEDREIRA

				//Regra de acoplamento NW (REGRAS.DAT): VAZ(5) = 0
				a_termo_independente_calculo_ENA += 0.0;

			}//else if (codigo_posto_acoplamento_NW == 5) {//PEDREIRA
			else if (a_codigo_posto_acoplamento_ENA == 13) {//TRAICAO

				//Regra de acoplamento NW (REGRAS.DAT): VAZ(13) = 0
				a_termo_independente_calculo_ENA += 0.0;

			}//else if (codigo_posto_acoplamento_NW == 13) {//TRAICAO
			else if (a_codigo_posto_acoplamento_ENA == 21) {//SANTA CECILIA (RETIRADA DE SANTA CECILIA DA CONFIGURACAO PARA O CALCULO DA ENA)

				//Regra de acoplamento NW (REGRAS.DAT): VAZ(21) = VAZ(123)
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 123, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

			}//else if (codigo_posto_acoplamento_NW == 21) {//SANTA CECILIA (RETIRADA DE SANTA CECILIA DA CONFIGURACAO PARA O CALCULO DA ENA)
			else if (a_codigo_posto_acoplamento_ENA == 19) {//TOCOS (RETIRADA DE TOCOS DA CONFIGURACAO PARA O CALCULO DA ENA)

				//Regra de acoplamento NW (REGRAS.DAT): VAZ(19) = 0
				a_termo_independente_calculo_ENA += 0.0;

			}//else if (codigo_posto_acoplamento_NW == 19) {//TOCOS (RETIRADA DE TOCOS DA CONFIGURACAO PARA O CALCULO DA ENA)
			else if (a_codigo_posto_acoplamento_ENA == 292) {//BELO MONTE

				//////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//288 - Belo Monte
				// Nota: Segue o hidrograma para Belo Monte
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////

				//Regra de acoplamento NW (REGRAS.DAT):
				//POST    MES  CONF    FORMULA
				//	XXX    XX     X    XXXXXXXXXXXXXXXXXXXXXXXXXXX
				//  292     1          SE(VAZ(288) <= 1100; 0; SE(VAZ(288) <= (1100 + 13900); VAZ(288) - 1100; 13900))
				//  292     2          SE(VAZ(288) <= 1600; 0; SE(VAZ(288) <= (1600 + 13900); VAZ(288) - 1600; 13900))
				//	292     3          SE(VAZ(288) <= 3250; 0; SE(VAZ(288) <= (3250 + 13900); VAZ(288) - 3250; 13900))
				//	292     4          SE(VAZ(288) <= 6000; 0; SE(VAZ(288) <= (6000 + 13900); VAZ(288) - 6000; 13900))
				//	292     5          SE(VAZ(288) <= 2900; 0; SE(VAZ(288) <= (2900 + 13900); VAZ(288) - 2900; 13900))
				//	292     6          SE(VAZ(288) <= 1600; 0; SE(VAZ(288) <= (1600 + 13900); VAZ(288) - 1600; 13900))
				//	292     7          SE(VAZ(288) <= 1100; 0; SE(VAZ(288) <= (1100 + 13900); VAZ(288) - 1100; 13900))
				//	292     8          SE(VAZ(288) <= 900; 0; SE(VAZ(288) <= (900 + 13900); VAZ(288) - 900; 13900))
				//	292     9          SE(VAZ(288) <= 750; 0; SE(VAZ(288) <= (750 + 13900); VAZ(288) - 750; 13900))
				//	292    10          SE(VAZ(288) <= 700; 0; SE(VAZ(288) <= (700 + 13900); VAZ(288) - 700; 13900))
				//	292    11          SE(VAZ(288) <= 800; 0; SE(VAZ(288) <= (800 + 13900); VAZ(288) - 800; 13900))
				//	292    12          SE(VAZ(288) <= 900; 0; SE(VAZ(288) <= (900 + 13900); VAZ(288) - 900; 13900))


				/////////////////////////
				//Postos "reais"
				/////////////////////////
				const double afluencia_288 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 288, a_idCenario, a_periodo);

				///////

				bool is_periodo_inicial = false;
				if (a_periodo == a_periodo_inicial_horizonte_estudo)
					is_periodo_inicial = true;

				const IdMes idMes_alvo = get_IdMes_operativo(a_periodo, is_periodo_inicial);

				if (idMes_alvo == IdMes_1) {

					if (afluencia_288 <= 1100)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (1100 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 1100;
					}//else if (afluencia_288 <= (1100 + 13900)) {						
					else
						a_termo_independente_calculo_ENA += 13900;

				}//if (idMes_alvo == IdMes_1) {
				else if (idMes_alvo == IdMes_2) {

					if (afluencia_288 <= 1600)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (1600 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 1600;
					}
					else
						a_termo_independente_calculo_ENA += 13900;

				}//else if (idMes_alvo == IdMes_2) {
				else if (idMes_alvo == IdMes_3) {

					if (afluencia_288 <= 3250)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (3250 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 3250;
					}
					else
						a_termo_independente_calculo_ENA += 13900;

				}//else if (idMes_alvo == IdMes_3) {
				else if (idMes_alvo == IdMes_4) {

					if (afluencia_288 <= 6000)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (6000 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 6000;
					}
					else
						a_termo_independente_calculo_ENA += 13900;

				}//else if (idMes_alvo == IdMes_4) {
				else if (idMes_alvo == IdMes_5) {

					if (afluencia_288 <= 2900)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (2900 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 2900;
					}
					else
						a_termo_independente_calculo_ENA += 13900;

				}//else if (idMes_alvo == IdMes_5) {
				else if (idMes_alvo == IdMes_6) {

					if (afluencia_288 <= 1600)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (1600 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 1600;
					}
					else
						a_termo_independente_calculo_ENA += 13900;

				}//else if (idMes_alvo == IdMes_6) {
				else if (idMes_alvo == IdMes_7) {

					if (afluencia_288 <= 1100)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (1100 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 1100;
					}
					else
						a_termo_independente_calculo_ENA += 13900;

				}//else if (idMes_alvo == IdMes_7) {
				else if (idMes_alvo == IdMes_8) {

					if (afluencia_288 <= 900)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (900 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 900;
					}
					else
						a_termo_independente_calculo_ENA += 13900;

				}//else if (idMes_alvo == IdMes_8) {
				else if (idMes_alvo == IdMes_9) {

					if (afluencia_288 <= 750)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (750 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 750;
					}
					else
						a_termo_independente_calculo_ENA += 13900;

				}//else if (idMes_alvo == IdMes_9) {
				else if (idMes_alvo == IdMes_10) {

					if (afluencia_288 <= 700)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (700 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 700;
					}
					else
						a_termo_independente_calculo_ENA += 13900;

				}//else if (idMes_alvo == IdMes_10) {
				else if (idMes_alvo == IdMes_11) {

					if (afluencia_288 <= 800)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (800 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 800;
					}
					else
						a_termo_independente_calculo_ENA += 13900;

				}//else if (idMes_alvo == IdMes_11) {
				else if (idMes_alvo == IdMes_12) {

					if (afluencia_288 <= 900)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (900 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA -= 900;
					}
					else
						a_termo_independente_calculo_ENA += 13900;

				}//else if (idMes_alvo == IdMes_12) {
				else { throw std::invalid_argument("Nao identificado idMes: " + getString(idMes_alvo) + " \n"); }

			}//else if (codigo_posto_acoplamento_NW == 292) {//BELO MONTE
			else if (a_codigo_posto_acoplamento_ENA == 302) {//PIMENTAL

				//////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//314 - Pimental
				// Nota: Segue o hidrograma para Belo Monte
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Regra:
				//VAZ(302) = VAZ(288) - VAZ(292)

				/////////////////////////
				//Postos "reais"
				/////////////////////////
				const double afluencia_288 = get_afluencia_natural_posto(a_dados, a_periodoPE, a_lista_codUsina, a_lista_codPosto, a_mapIdVar, a_mapIdVarInterna, 288, a_idCenario, a_periodo);

				//**********************************
				//VAZ(302) = VAZ(288) - VAZ(292)
				//**********************************

				//////////
				//VAZ(288)
				//////////
				retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				//////////
				//-VAZ(292)
				//////////
				bool is_periodo_inicial = false;
				if (a_periodo == a_periodo_inicial_horizonte_estudo)
					is_periodo_inicial = true;

				const IdMes idMes_alvo = get_IdMes_operativo(a_periodo, is_periodo_inicial);

				if (idMes_alvo == IdMes_1) {

					if (afluencia_288 <= 1100)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (1100 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 1100;
					}//else if (afluencia_288 <= (1100 + 13900)) {						
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//if (idMes_alvo == IdMes_1) {
				else if (idMes_alvo == IdMes_2) {

					if (afluencia_288 <= 1600)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (1600 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 1600;
					}
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//else if (idMes_alvo == IdMes_2) {
				else if (idMes_alvo == IdMes_3) {

					if (afluencia_288 <= 3250)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (3250 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 3250;
					}
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//else if (idMes_alvo == IdMes_3) {
				else if (idMes_alvo == IdMes_4) {

					if (afluencia_288 <= 6000)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (6000 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 6000;
					}
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//else if (idMes_alvo == IdMes_4) {
				else if (idMes_alvo == IdMes_5) {

					if (afluencia_288 <= 2900)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (2900 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 2900;
					}
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//else if (idMes_alvo == IdMes_5) {
				else if (idMes_alvo == IdMes_6) {

					if (afluencia_288 <= 1600)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (1600 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 1600;
					}
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//else if (idMes_alvo == IdMes_6) {
				else if (idMes_alvo == IdMes_7) {

					if (afluencia_288 <= 1100)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (1100 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 1100;
					}
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//else if (idMes_alvo == IdMes_7) {
				else if (idMes_alvo == IdMes_8) {

					if (afluencia_288 <= 900)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (900 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 900;
					}
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//else if (idMes_alvo == IdMes_8) {
				else if (idMes_alvo == IdMes_9) {

					if (afluencia_288 <= 750)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (750 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 750;
					}
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//else if (idMes_alvo == IdMes_9) {
				else if (idMes_alvo == IdMes_10) {

					if (afluencia_288 <= 700)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (700 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 700;
					}
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//else if (idMes_alvo == IdMes_10) {
				else if (idMes_alvo == IdMes_11) {

					if (afluencia_288 <= 800)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (800 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 800;
					}
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//else if (idMes_alvo == IdMes_11) {
				else if (idMes_alvo == IdMes_12) {

					if (afluencia_288 <= 900)
						a_termo_independente_calculo_ENA += 0;
					else if (afluencia_288 <= (900 + 13900)) {
						retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 288, -1.0, a_coeficiente_idHidreletricas_calculo_ENA);
						a_termo_independente_calculo_ENA += 900;
					}
					else
						a_termo_independente_calculo_ENA -= 13900;

				}//else if (idMes_alvo == IdMes_12) {
				else { throw std::invalid_argument("Nao identificado idMes: " + getString(idMes_alvo) + " \n"); }

			}//else if (codigo_posto_acoplamento_NW == 292) {//BELO MONTE						
			else if (a_codigo_posto_acoplamento_ENA == 169) {//SOBRADINHO - Posto para acoplamento com a FCF

				if (a_periodo >= a_periodo_inicial_horizonte_estudo) {

					//Nota: Como sobradinho fictício (posto = 169) está isolado do sistema, devem ser somadas as naturais de Três Marias e Queimado

					/////////////
					// VAZ(156) //Três Marias
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 156, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					/////////////
					// VAZ(158) //Queimado
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 158, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					/////////////
					// VAZ(169) //Sobradinho Fictício
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 169, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				}//if (a_periodo >= a_periodo_inicial_horizonte_estudo) {
				else if (a_periodo < a_periodo_inicial_horizonte_estudo) {

					//******************************
					//Tendência
					//******************************
					//Pega a informação da tendência do arquivo VAZOES.DAT (do modelo GEVAZP)-> Por construção esta informação já está em afluência natural
					const int posto = 169;
					a_termo_independente_calculo_ENA += a_dados.processoEstocastico_hidrologico.getElementoVetor(a_mapIdVar.at(a_lista_codPosto.at(posto)), a_mapIdVarInterna.at(a_lista_codPosto.at(posto)), AttVetorVariavelAleatoriaInterna_tendencia_temporal, a_periodo, double());

				}//else if (a_periodo < a_periodo_inicial_horizonte_estudo) {

			}//else if (codigo_posto_acoplamento_NW == 169) {//SOBRADINHO - Posto para acoplamento com a FCF
			else if (a_codigo_posto_acoplamento_ENA == 172) {//ITAPARICA

				//Usina com codigo_posto para otimização setado como 300 (afluência incremental = 0) mas com posto de acoplamento para cálculo das ENAs = posto do hidr.dat

				if (a_periodo >= a_periodo_inicial_horizonte_estudo) {

					//Nota: Como sobradinho fictício (posto = 169) está isolado do sistema, devem ser somadas as naturais de Três Marias e Queimado

					/////////////
					// VAZ(156) //Três Marias
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 156, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					/////////////
					// VAZ(158) //Queimado
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 158, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					/////////////
					// VAZ(169) //Sobradinho Fictício
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 169, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				}//if (a_periodo >= a_periodo_inicial_horizonte_estudo) {
				else if (a_periodo < a_periodo_inicial_horizonte_estudo) {

					//******************************
					//Tendência
					//******************************
					//Pega a informação da tendência do arquivo VAZOES.DAT (do modelo GEVAZP)-> Por construção esta informação já está em afluência natural
					const int posto = 172;
					a_termo_independente_calculo_ENA += a_dados.processoEstocastico_hidrologico.getElementoVetor(a_mapIdVar.at(a_lista_codPosto.at(posto)), a_mapIdVarInterna.at(a_lista_codPosto.at(posto)), AttVetorVariavelAleatoriaInterna_tendencia_temporal, a_periodo, double());

				}//else if (a_periodo < a_periodo_inicial_horizonte_estudo) {

			}//else if (codigo_posto_acoplamento_NW == 172) {//ITAPARICA
			else if (a_codigo_posto_acoplamento_ENA == 178) {//XINGO

				//Usina com codigo_posto para otimização setado como 300 (afluência incremental = 0) mas com posto de acoplamento para cálculo das ENAs = posto do hidr.dat

				if (a_periodo >= a_periodo_inicial_horizonte_estudo) {

					//Nota: Como sobradinho fictício (posto = 169) está isolado do sistema, devem ser somadas as naturais de Três Marias e Queimado

					/////////////
					// VAZ(156) //Três Marias
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 156, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					/////////////
					// VAZ(158) //Queimado
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 158, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					/////////////
					// VAZ(169) //Sobradinho Fictício
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 169, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				}//if (a_periodo >= a_periodo_inicial_horizonte_estudo) {
				else if (a_periodo < a_periodo_inicial_horizonte_estudo) {

					//******************************
					//Tendência
					//******************************
					//Pega a informação da tendência do arquivo VAZOES.DAT (do modelo GEVAZP)-> Por construção esta informação já está em afluência natural
					const int posto = 178;
					a_termo_independente_calculo_ENA += a_dados.processoEstocastico_hidrologico.getElementoVetor(a_mapIdVar.at(a_lista_codPosto.at(posto)), a_mapIdVarInterna.at(a_lista_codPosto.at(posto)), AttVetorVariavelAleatoriaInterna_tendencia_temporal, a_periodo, double());

				}//else if (a_periodo < a_periodo_inicial_horizonte_estudo) {
			}//else if (codigo_posto_acoplamento_NW == 178) {//XINGO
			else if (a_codigo_posto_acoplamento_ENA == 976) {//176-COMP-MOX-PAFONSO (atribuido o codigo_posto_acoplamento_ENA = 976 para ser compatível com a lógica das regras)

				if (a_periodo >= a_periodo_inicial_horizonte_estudo) {


					//Nota: Como sobradinho fictício (posto = 169) está isolado do sistema, devem ser somadas as naturais de Três Marias e Queimado

					/////////////
					// VAZ(156) //Três Marias
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 156, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					/////////////
					// VAZ(158) //Queimado
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 158, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

					/////////////
					// VAZ(169) //Sobradinho Fictício
					/////////////
					retorna_equacionamento_afluencia_natural_x_posto(a_dados, a_lista_codUsina, a_lista_codPosto, IdHidreletrica_Nenhum, 169, 1.0, a_coeficiente_idHidreletricas_calculo_ENA);

				}//if (a_periodo >= a_periodo_inicial_horizonte_estudo) {
				else if (a_periodo < a_periodo_inicial_horizonte_estudo) {

					//******************************
					//Tendência
					//******************************
					//Pega a informação da tendência do arquivo VAZOES.DAT (do modelo GEVAZP)-> Por construção esta informação já está em afluência natural
					const int posto = 176;
					a_termo_independente_calculo_ENA += a_dados.processoEstocastico_hidrologico.getElementoVetor(a_mapIdVar.at(a_lista_codPosto.at(posto)), a_mapIdVarInterna.at(a_lista_codPosto.at(posto)), AttVetorVariavelAleatoriaInterna_tendencia_temporal, a_periodo, double());

				}//else if (a_periodo < a_periodo_inicial_horizonte_estudo) {

			}//else if (codigo_posto_acoplamento_NW == 176) {
			else if (a_codigo_posto_acoplamento_ENA == 300) {//MOXOTÓ / P.AFONSO 123 / P.AFONSO 4

				//Nota: estas usinas são contabilizadas no posto do 176-COMP.MOX.PAFON
				//******************************
				//posto 300
				//******************************
				a_termo_independente_calculo_ENA += 0.0;

			}//else if (codigo_posto_acoplamento_NW == 300) {//MOXOTÓ / P.AFONSO 123 / P.AFONSO 4
			else { throw std::invalid_argument("Nao implementada regra de calculo de vazao para posto com codigo: " + getString(a_codigo_posto_acoplamento_ENA) + "\n"); }

		}//else {

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::retorna_equacionamento_regras_afluencia_natural_x_idHidreletrica(" + getFullString(a_idHidreletrica) + " periodo: " + getFullString(a_periodo) + ") : \n" + std::string(erro.what())); }

}

void LeituraCEPEL::retorna_equacionamento_afluencia_natural_x_posto(Dados& a_dados, const SmartEnupla<int, IdHidreletrica>& a_lista_codUsina, const SmartEnupla<int, IdHidreletrica>& a_lista_codPosto, const IdHidreletrica a_idHidreletrica, const int a_codigo_posto, const double a_coeficiente, std::vector<SmartEnupla<IdHidreletrica, double>>& a_coeficiente_idHidreletricas_calculo_ENA) {

	try {

		if (a_codigo_posto != 300) {//Existem várias idHidrelétricas com o posto 300 (incremental = 0)

			const IdHidreletrica idHidreletrica = a_lista_codPosto.at(a_codigo_posto);

			const int iterador_inicial = a_dados.vetorHidreletrica.at(idHidreletrica).getIteradorInicial(AttVetorHidreletrica_codigo_usinas_calculo_ENA, int());
			const int iterador_final = a_dados.vetorHidreletrica.at(idHidreletrica).getIteradorFinal(AttVetorHidreletrica_codigo_usinas_calculo_ENA, int());

			for (int pos = iterador_inicial; pos <= iterador_final; pos++) {
				const IdHidreletrica idHidreletrica_aux = a_lista_codUsina.at(a_dados.vetorHidreletrica.at(idHidreletrica).getElementoVetor(AttVetorHidreletrica_codigo_usinas_calculo_ENA, pos, int()));
				a_coeficiente_idHidreletricas_calculo_ENA.push_back(SmartEnupla<IdHidreletrica, double>(idHidreletrica_aux, std::vector<double>(1, a_coeficiente)));
			}//for (pos = iterador_inicial; pos <= iterador_final; pos++) {

		}//if (a_codigo_posto != 300) {
		else {

			if (a_idHidreletrica == IdHidreletrica_Nenhum) { throw std::invalid_argument("Deve ser informado um idHidreletrica\n"); }

			const int iterador_inicial = a_dados.vetorHidreletrica.at(a_idHidreletrica).getIteradorInicial(AttVetorHidreletrica_codigo_usinas_calculo_ENA, int());
			const int iterador_final = a_dados.vetorHidreletrica.at(a_idHidreletrica).getIteradorFinal(AttVetorHidreletrica_codigo_usinas_calculo_ENA, int());

			for (int pos = iterador_inicial; pos <= iterador_final; pos++) {
				const IdHidreletrica idHidreletrica_aux = a_lista_codUsina.at(a_dados.vetorHidreletrica.at(a_idHidreletrica).getElementoVetor(AttVetorHidreletrica_codigo_usinas_calculo_ENA, pos, int()));
				a_coeficiente_idHidreletricas_calculo_ENA.push_back(SmartEnupla<IdHidreletrica, double>(idHidreletrica_aux, std::vector<double>(1, a_coeficiente)));
			}//for (pos = iterador_inicial; pos <= iterador_final; pos++) {

		}//else {

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::retorna_equacionamento_afluencia_natural_x_posto: \n" + std::string(erro.what())); }

}

double LeituraCEPEL::get_afluencia_natural_posto(Dados& a_dados, const Periodo a_periodoPE, const SmartEnupla<int, IdHidreletrica>& a_lista_codUsina, const SmartEnupla<int, IdHidreletrica>& a_lista_codPosto, const SmartEnupla<IdHidreletrica, IdVariavelAleatoria>& a_mapIdVar, const SmartEnupla<IdHidreletrica, IdVariavelAleatoriaInterna>& a_mapIdVarInterna, const int a_codigo_posto, const IdCenario a_idCenario, const Periodo a_periodo) {

	try {

		//Nota: soma as incrementais de todas as usinas que estejam a montante da usina com codigo_posto = a_codigo_posto

		double afluencia_natural_posto = 0.0;
		bool is_encontrado_posto = false;

		const IdHidreletrica idHidreletrica = a_lista_codPosto.at(a_codigo_posto);

		const int iterador_inicial = a_dados.vetorHidreletrica.at(idHidreletrica).getIteradorInicial(AttVetorHidreletrica_codigo_usinas_calculo_ENA, int());
		const int iterador_final = a_dados.vetorHidreletrica.at(idHidreletrica).getIteradorFinal(AttVetorHidreletrica_codigo_usinas_calculo_ENA, int());

		for (int pos = iterador_inicial; pos <= iterador_final; pos++) {

			const IdHidreletrica idHidreletrica_aux = a_lista_codUsina.at(a_dados.vetorHidreletrica.at(idHidreletrica).getElementoVetor(AttVetorHidreletrica_codigo_usinas_calculo_ENA, pos, int()));

			if (a_periodo < a_dados.processoEstocastico_hidrologico.getIterador2Inicial(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario_1, Periodo())) //Valores da tendência
				afluencia_natural_posto += a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.at(a_mapIdVar.at(idHidreletrica_aux)).vetorVariavelAleatoriaInterna.at(a_mapIdVarInterna.at(idHidreletrica_aux)).getElementoVetor(AttVetorVariavelAleatoriaInterna_tendencia_temporal, a_periodo, double());
			else {

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//O processo estocástico pode ter sido "quebrado" em mais períodos. 
				// Portanto é procurado o primeiro período com sobreposição com o período do processo estocástico (o qual tem os valores das realizações)
				// a_periodo -> realiza como o DC
				// periodo_horizonte_processo_estocastico -> pode realizar com uma discretização menor ao DC
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				afluencia_natural_posto += a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.at(a_mapIdVar.at(idHidreletrica_aux)).vetorVariavelAleatoriaInterna.at(a_mapIdVarInterna.at(idHidreletrica_aux)).getElementoMatriz(AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral, a_periodoPE, a_idCenario, double());

			}//else {

		}//for (pos = iterador_inicial; pos <= iterador_final; pos++) {

		return afluencia_natural_posto;

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::get_afluencia_natural_posto: \n" + std::string(erro.what())); }

}

IdMes LeituraCEPEL::get_IdMes_operativo(const Periodo a_periodo, const bool is_periodo_inicial)
{
	try {

		if (a_periodo.getTipoPeriodo() > TipoPeriodo_semanal)
			throw std::invalid_argument("Nao implementada regra para periodos com duracao menor a TipoPeriodo_semanal \n");

		Periodo periodo_teste = a_periodo;

		if (is_periodo_inicial)//Para garantir que a primeira semana operativa corresponda ao mês operativo
			periodo_teste++;

		return periodo_teste.getMes();

	}//	try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::get_IdMes_operativo: \n" + std::string(erro.what())); }

}
