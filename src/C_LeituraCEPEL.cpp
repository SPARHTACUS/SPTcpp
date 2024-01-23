#include "C_LeituraCEPEL.h"
#include "C_EntradaSaidaDados.h"

const bool instanciarPreConfig = true;

LeituraCEPEL::LeituraCEPEL() {

	deck_str = "";

	SPT_avisos = "SPT_avisos.txt";

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


void LeituraCEPEL::calculaEngolimentoMaximo(Dados& a_dados, const SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo, const Periodo a_periodo_final, const bool a_lido_turbinamento_maximo_from_relato_e_avl_turb_max_DC) {
	IdHidreletrica idHidreletrica;
	try {
		const IdHidreletrica maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

		SmartEnupla<Periodo, bool>	horizonte_processo_estocastico;

		if (a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.att(IdVariavelAleatoria_1).vetorVariavelAleatoriaInterna.att(IdVariavelAleatoriaInterna_1).getSizeMatriz(AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral) > 0) {

			horizonte_processo_estocastico = SmartEnupla<Periodo, bool>(a_dados.processoEstocastico_hidrologico.getMatriz(IdVariavelAleatoria_1, IdVariavelAleatoriaInterna_1, AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral, Periodo(), IdCenario(), double()), true);


		}//	if (a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.att(IdVariavelAleatoria_1).vetorVariavelAleatoriaInterna.att(IdVariavelAleatoriaInterna_1).getSizeMatriz(AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral) > 0) {


		for (idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			if (a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) != TipoDetalhamentoProducaoHidreletrica_sem_producao) {
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Calcula a defluencia_maxima do polinomio_jusante para obter h_jus > 0 nas usinas com maiorPolinomiojusante = 1
				//Usinas reportadas no arquivo polijus.dat tem a informaçao da defluência_maxima

				if (a_dados.getMaiorId(idHidreletrica, IdPolinomioJusante()) == IdPolinomioJusante_1 && a_dados.getAtributo(idHidreletrica, IdPolinomioJusante_1, AttComumPolinomioJusante_defluencia_maxima, double()) == getdoubleFromChar("max")) {
					const double defluencia_maxima = calculaRaizPolinomioJusante(a_dados, idHidreletrica, a_horizonte_estudo.getIteradorInicial());
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorPolinomioJusante.att(IdPolinomioJusante_1).setAtributo(AttComumPolinomioJusante_defluencia_maxima, defluencia_maxima);
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
							a_dados.vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_vazao_turbinada_maxima, SmartEnupla<Periodo, double>(a_horizonte_estudo, q_efetiva));

						if (a_dados.getSizeVetor(idHidreletrica, AttVetorHidreletrica_cota_montante_usina_jusante) == 0)
							a_dados.vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_cota_montante_usina_jusante, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0));

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
								h_mont_jusante = a_dados.vetorHidreletrica.att(idHidreletrica_jusante).vetorReservatorio.att(IdReservatorio_1).getCota(periodo, volume);

							}
							else {
								for (IdPolinomioJusante idPolinomio = IdPolinomioJusante_1; idPolinomio <= maiorIdPolinomio; idPolinomio++)
									if (a_dados.getSizeVetor(idHidreletrica, idPolinomio, AttVetorPolinomioJusante_altura_jusante_ref) > 0)
										h_mont_jusante = a_dados.getElementoVetor(idHidreletrica, idPolinomio, AttVetorPolinomioJusante_altura_jusante_ref, periodo, double());
							}//else {

							a_dados.vetorHidreletrica.att(idHidreletrica).setElemento(AttVetorHidreletrica_cota_montante_usina_jusante, periodo, h_mont_jusante);

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

							const double h_mont = a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getCota(periodo, volume);

							const double cota_referencia = a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getCotaMedia(periodo, volume_minimo, volume_maximo);
							const double produtibilidade_media = a_dados.vetorHidreletrica.att(idHidreletrica).calcularProdutibilidade(tipo_perda_hidraulica, perda_hidraulica, fator_de_producao, cota_referencia - canal_fuga_medio);

							const double vazao_maxima_gerador = potencia_maxima / produtibilidade_media;

							double  q_max_inicial = 0;
							double q_max = vazao_maxima_gerador;

							for (int i = 0; i < 15; i++) {
								q_max_inicial = q_max;

								double h_jus = a_dados.vetorHidreletrica.att(idHidreletrica).getCotaJusante(q_max_inicial, periodo);
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
								const double vazao_defluencia_minima = calculaDefluenciaMinima_para_EngolimentoMaximo(a_dados, idHidreletrica, periodo, idEstagio_periodo, horizonte_processo_estocastico);
								const double defluencia_maxima_polinomio_jusante = a_dados.getAtributo(idHidreletrica, a_dados.getMaiorId(idHidreletrica, IdPolinomioJusante()), AttComumPolinomioJusante_defluencia_maxima, double());

								if (q_max < vazao_defluencia_minima && vazao_defluencia_minima < defluencia_maxima_polinomio_jusante) {

									//Recalcula o q_Max com h_jus(vazao_defluencia_minima)
									double h_jus = a_dados.vetorHidreletrica.att(idHidreletrica).getCotaJusante(vazao_defluencia_minima, periodo);
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

							a_dados.vetorHidreletrica.att(idHidreletrica).setElemento(AttVetorHidreletrica_vazao_turbinada_maxima, periodo, q_max);

						}//for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo)) {

					}//if (tipo_turbina != TipoTurbina_sem_turbina) {

				}//if (maiorIdConjunto != IdConjuntoHidraulico_Nenhum){
			}
		}//for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::calculaEngolimentoMaximo(" + getFullString(idHidreletrica) + "): \n" + std::string(erro.what())); }
}

double LeituraCEPEL::calculaDefluenciaMinima_para_EngolimentoMaximo(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const Periodo a_periodo, const IdEstagio a_idEstagio, const SmartEnupla<Periodo, bool> a_horizonte_processo_estocastico)
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
			if (a_dados.vetorHidreletrica.att(idHidreletricas_montante.at(0)).getElementoVetor(AttVetorHidreletrica_regularizacao, a_periodo, int()) != 1) {

				idHidreletricas_calculo_defluencia_minima.push_back(idHidreletricas_montante.at(0));

				//Inclui usinas a montante da usina idHidreletricas_montante.at(0) 

				numMontante = a_dados.getSizeVetor(idHidreletricas_montante.at(0), AttVetorHidreletrica_montante);

				for (int m = 1; m <= numMontante; m++)
					idHidreletricas_montante.push_back(a_dados.getElementoVetor(idHidreletricas_montante.at(0), AttVetorHidreletrica_montante, m, IdHidreletrica()));

			}//if (!a_dados.vetorHidreletrica.att(idHidreletricas_montante.at(0)).getElementoVetor(AttVetorHidreletrica_regularizacao, a_periodo, bool())) {

			//Elimina do vetor a usina já validada
			idHidreletricas_montante.erase(idHidreletricas_montante.begin());

		}//while (true) {

		//2. Cálculo das vazoes a montante

		double vazao_defluencia_minima = 0;

		for (Periodo periodo_processo_estocastico = a_horizonte_processo_estocastico.getIteradorInicial(); periodo_processo_estocastico <= a_horizonte_processo_estocastico.getIteradorFinal(); a_horizonte_processo_estocastico.incrementarIterador(periodo_processo_estocastico)) {

			if (a_periodo.sobreposicao(periodo_processo_estocastico) == 1) {

				for (int pos = 0; pos < int(idHidreletricas_calculo_defluencia_minima.size()); pos++) {

					const IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria(lista_hidreletrica_IdVariavelAleatoria.at(idHidreletricas_calculo_defluencia_minima.at(pos)));

					if (a_idEstagio <= a_dados.getAtributo(AttComumDados_estagio_acoplamento_pre_estudo, IdEstagio()))
						vazao_defluencia_minima += a_dados.processoEstocastico_hidrologico.getElementoMatriz(idVariavelAleatoria, IdVariavelAleatoriaInterna_1, AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral, periodo_processo_estocastico, IdCenario_1, double());
					else
						vazao_defluencia_minima += a_dados.processoEstocastico_hidrologico.getElementoMatriz(idVariavelAleatoria, AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodo_processo_estocastico, IdRealizacao_1, double());

				}//for (int pos = 0; pos < int(idHidreletricas_calculo_defluencia_minima.size()); pos++) {

				break;

			}//if (a_periodo.sobreposicao(periodo_processo_estocastico) == 1) {

		}//for (Periodo periodo_processo_estocastico = a_horizonte_processo_estocastico.getIteradorInicial(); periodo_processo_estocastico <= a_horizonte_processo_estocastico.getIteradorFinal(); a_horizonte_processo_estocastico.incrementarIterador(periodo_processo_estocastico)) {

		//3. Desconta a vazao que a usina consegue armazenar no periodo

		if (a_dados.vetorHidreletrica.att(a_idHidreletrica).getElementoVetor(AttVetorHidreletrica_regularizacao, a_periodo, int()) == 1) {

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

		}//if (a_dados.vetorHidreletrica.att(a_idHidreletrica).getElementoVetor(AttVetorHidreletrica_regularizacao, a_periodo, bool())){

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

		double h_jus_defluencia_maxima = a_dados.vetorHidreletrica.att(a_idHidreletrica).getCotaJusante_permite_valoresNegativos(defluencia_maxima, a_periodo);
		double h_jus_defluencia_minima = a_dados.vetorHidreletrica.att(a_idHidreletrica).getCotaJusante_permite_valoresNegativos(defluencia_minima, a_periodo);

		double defluencia_raiz_anterior = defluencia_raiz;

		if (h_jus_defluencia_minima * h_jus_defluencia_maxima < 0.0) {

			while (true) {

				//Aplica método de bissecçao para encontrar a raiz do polinômio
				defluencia_raiz = 0.5 * (defluencia_maxima + defluencia_minima);

				double h_jus_defluencia_raiz = a_dados.vetorHidreletrica.att(a_idHidreletrica).getCotaJusante_permite_valoresNegativos(defluencia_raiz, a_periodo);

				if (abs(defluencia_raiz - defluencia_raiz_anterior) <= tol || abs(h_jus_defluencia_raiz) <= tol) {
					defluencia_maxima = defluencia_raiz;
					break;
				}//if (abs(defluencia_raiz - defluencia_raiz_anterior) <= tol || abs(h_jus_defluencia_raiz) <= tol) {

				defluencia_raiz_anterior = defluencia_raiz;

				h_jus_defluencia_minima = a_dados.vetorHidreletrica.att(a_idHidreletrica).getCotaJusante_permite_valoresNegativos(defluencia_minima, a_periodo);

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
		if (!entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("TERMELETRICA_AttComumOperacional.csv", a_dados, TipoAcessoInstancia_membro))
			return;
		const IdTermeletrica  maiorIdTermeletrica = a_dados.getMaiorId(IdTermeletrica());

		for (IdTermeletrica idTermeletrica = a_dados.getMenorId(IdTermeletrica()); idTermeletrica <= maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idTermeletrica)) {
			a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_considerar_usina, false);
			a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_representacao_discreta_producao, false);
			a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica_sem_producao);

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
		entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("HIDRELETRICA_AttComumOperacional.csv", a_dados, TipoAcessoInstancia_membro);

		//Set listas necessárias na estrutura da leitura

		const IdHidreletrica  maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

		if (maiorIdHidreletrica > IdHidreletrica_Nenhum) {

			if (true) {

				a_dados.volume_inicial_carregado_from_operacional = entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("HIDRELETRICA_RESERVATORIO_AttComumOperacional.csv", a_dados, TipoAcessoInstancia_membroMembro);
				a_dados.volume_inicial_carregado_from_premissa    = entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("HIDRELETRICA_RESERVATORIO_AttComumPremissa.csv", a_dados, TipoAcessoInstancia_membroMembro);

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

			entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("BACIA_HIDROGRAFICA_AttComumOperacional.csv", a_dados, TipoAcessoInstancia_membro);

			entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("HIDRELETRICA_AttVetorOperacional_PorPeriodo.csv", a_dados, TipoAcessoInstancia_membro);

			if (true) {
				const bool leitura_att_vetor_defluencia = entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("HIDRELETRICA_DEFLUENCIA_AttVetorOperacional_PorPeriodo.csv", a_dados, TipoAcessoInstancia_membroMembro);
				if ((a_dados.getAtributo(AttComumDados_estagio_inicial, IdEstagio()) > IdEstagio_1) && (leitura_att_vetor_defluencia))
					throw std::invalid_argument("O arquivo HIDRELETRICA_DEFLUENCIA_AttVetorOperacional_PorPeriodo.csv nao deve ser lido caso " + getFullString(AttComumDados_estagio_inicial) + " seja maior que " + getFullString(IdEstagio_1) + ".");
			} // if (true) {

			bool tipo_detalhamento_producao_por_usina = false;
			bool tipo_detalhamento_producao_por_conjunto = false;
			bool tipo_detalhamento_producao_por_unidade = false;

			for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

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

					a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_bacia, idBaciaHidrografica);
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

			} // for(IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica))

			if (tipo_detalhamento_producao_por_usina)
				entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("HIDRELETRICA_FPH_AttMatrizOperacional_PorPeriodoPorInteiro.csv", a_dados, TipoAcessoInstancia_membroMembro);

			if (tipo_detalhamento_producao_por_conjunto)
				entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("HIDRELETRICA_CONJUNTO_FPH_AttMatrizOperacional_PorPeriodoPorInteiro.csv", a_dados, TipoAcessoInstancia_membroMembro);

			if (tipo_detalhamento_producao_por_unidade)
				entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("HIDRELETRICA_CONJUNTO_UNIDADE_FPH_AttMatrizOperacional_PorPeriodoPorInteiro.csv", a_dados, TipoAcessoInstancia_membroMembro);

			if ((a_dados.getAtributo(AttComumDados_tipo_geracao_cenario_hidrologico, TipoGeracaoCenario()) == TipoGeracaoCenario_sintetica_in_sample) ||
				(a_dados.getAtributo(AttComumDados_tipo_geracao_cenario_hidrologico, TipoGeracaoCenario()) == TipoGeracaoCenario_sintetica_out_of_sample)) {

				if (strCompara(a_dados.getAtributo(AttComumDados_diretorio_importacao_pre_estudo, std::string()), "nenhum")) {
					entradaSaidaDados.carregarArquivoCSV_AttVetor("HIDRELETRICA_AFLUENCIA_AttVetorPremissa_PorPeriodo.csv", a_dados, TipoAcessoInstancia_membroMembro);
					entradaSaidaDados.carregarArquivoCSV_AttMatriz("HIDRELETRICA_AFLUENCIA_AttMatrizPremissa_PorCenarioPorPeriodo.csv", a_dados, TipoAcessoInstancia_membroMembro);
				}
				else {
					const std::string diretorio_entrada = entradaSaidaDados.getDiretorioEntrada();
					entradaSaidaDados.setDiretorioEntrada(a_dados.getAtributo(AttComumDados_diretorio_importacao_pre_estudo, std::string()));
					entradaSaidaDados.carregarArquivoCSV_AttVetor("HIDRELETRICA_AFLUENCIA_AttVetorPremissa_PorPeriodo.csv", a_dados, TipoAcessoInstancia_membroMembro);
					entradaSaidaDados.carregarArquivoCSV_AttMatriz("HIDRELETRICA_AFLUENCIA_AttMatrizPremissa_PorCenarioPorPeriodo.csv", a_dados, TipoAcessoInstancia_membroMembro);
					entradaSaidaDados.setDiretorioEntrada(diretorio_entrada);
				}

				a_dados.setAtributo(AttComumDados_tipo_tendencia_hidrologica, TipoTendenciaEstocastica_serie_informada);

			}


			bool tendencia_natural = false;
			bool tendencia_incremental = false;
			if (a_dados.getVetor(AttVetorDados_horizonte_otimizacao, IdEstagio(), Periodo()).size() > 0) {

				Periodo periodo_final_tendencia = a_dados.getElementoVetor(AttVetorDados_horizonte_otimizacao, IdEstagio_1, Periodo());

				for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

					if (a_dados.vetorHidreletrica.att(idHidreletrica).vetorAfluencia.isInstanciado(IdAfluencia_vazao_afluente)) {

						if (a_dados.getSizeVetor(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_incremental_tendencia) > 0) {
							periodo_final_tendencia = a_dados.getIteradorFinal(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_incremental_tendencia, Periodo());
							tendencia_incremental = true;
						}

						else if (a_dados.getSizeVetor(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_natural_tendencia) > 0) {
							periodo_final_tendencia = a_dados.getIteradorFinal(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_natural_tendencia, Periodo());
							tendencia_natural = true;
						}
						

					} // if (a_dados.vetorHidreletrica.att(idHidreletrica).vetorAfluencia.isInstanciado(IdAfluencia_vazao_afluente)) {

				} // for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

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

		}//if (maiorIdHidreletrica > IdHidreletrica_Nenhum) {

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
			else if (!entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("VARIAVEL_ALEATORIA_" + getString(AttVetorVariavelAleatoria_tipo_relaxacao) + ".csv", processoEstocastico_hidrologico, TipoAcessoInstancia_membro))
				processoEstocastico_totalmente_carregado = false;
			else if (!entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("VARIAVEL_ALEATORIA_" + getString(AttMatrizVariavelAleatoria_residuo_espaco_amostral) + ".csv", processoEstocastico_hidrologico, TipoAcessoInstancia_membro))
				processoEstocastico_totalmente_carregado = false;
			else if (!entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("VARIAVEL_ALEATORIA_" + getString(AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao) + ".csv", processoEstocastico_hidrologico, TipoAcessoInstancia_membro))
				processoEstocastico_totalmente_carregado = false;
			else if (!entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("VARIAVEL_ALEATORIA_INTERNA_AttComumOperacional.csv", processoEstocastico_hidrologico, TipoAcessoInstancia_membroMembro))
				processoEstocastico_totalmente_carregado = false;
			else if (!entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("VARIAVEL_ALEATORIA_INTERNA_" + getString(AttVetorVariavelAleatoriaInterna_coeficiente_participacao) + ".csv", processoEstocastico_hidrologico, TipoAcessoInstancia_membroMembro))
				processoEstocastico_totalmente_carregado = false;
			else if (!entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("VARIAVEL_ALEATORIA_INTERNA_" + getString(AttVetorVariavelAleatoriaInterna_tendencia_temporal) + ".csv", processoEstocastico_hidrologico, TipoAcessoInstancia_membroMembro))
				processoEstocastico_totalmente_carregado = false;
		}

		if (!processoEstocastico_totalmente_carregado)
			return;

		entradaSaidaDados.carregarArquivoCSV_AttComum("PROCESSO_ESTOCASTICO_AttComumOperacional.csv", a_dados.processoEstocastico_hidrologico, TipoAcessoInstancia_direto);
		entradaSaidaDados.carregarArquivoCSV_AttMatriz("PROCESSO_ESTOCASTICO_" + getString(AttMatrizProcessoEstocastico_probabilidade_realizacao) + ".csv", a_dados.processoEstocastico_hidrologico, TipoAcessoInstancia_direto);
		entradaSaidaDados.carregarArquivoCSV_AttMatriz("PROCESSO_ESTOCASTICO_" + getString(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral) + ".csv", a_dados.processoEstocastico_hidrologico, TipoAcessoInstancia_direto);
		entradaSaidaDados.carregarArquivoCSV_AttVetor("VARIAVEL_ALEATORIA_" + getString(AttVetorVariavelAleatoria_tipo_relaxacao) + ".csv", a_dados.processoEstocastico_hidrologico, TipoAcessoInstancia_membro);
		entradaSaidaDados.carregarArquivoCSV_AttMatriz("VARIAVEL_ALEATORIA_" + getString(AttMatrizVariavelAleatoria_residuo_espaco_amostral) + ".csv", a_dados.processoEstocastico_hidrologico, TipoAcessoInstancia_membro);
		entradaSaidaDados.carregarArquivoCSV_AttMatriz("VARIAVEL_ALEATORIA_" + getString(AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao) + ".csv", a_dados.processoEstocastico_hidrologico, TipoAcessoInstancia_membro);
		entradaSaidaDados.carregarArquivoCSV_AttComum("VARIAVEL_ALEATORIA_INTERNA_AttComumOperacional.csv", a_dados.processoEstocastico_hidrologico, TipoAcessoInstancia_membroMembro);
		entradaSaidaDados.carregarArquivoCSV_AttVetor("VARIAVEL_ALEATORIA_INTERNA_" + getString(AttVetorVariavelAleatoriaInterna_coeficiente_participacao) + ".csv", a_dados.processoEstocastico_hidrologico, TipoAcessoInstancia_membroMembro);
		entradaSaidaDados.carregarArquivoCSV_AttVetor("VARIAVEL_ALEATORIA_INTERNA_" + getString(AttVetorVariavelAleatoriaInterna_tendencia_temporal) + ".csv", a_dados.processoEstocastico_hidrologico, TipoAcessoInstancia_membroMembro);

		processoEstocasticoHidrologicoPreConfig_instanciado = true;

		for (IdHidreletrica idUHE = a_dados.getMenorId(IdHidreletrica()); idUHE <= a_dados.getMaiorId(IdHidreletrica()); a_dados.vetorHidreletrica.incr(idUHE)) {

				IdVariavelAleatoria idVar = IdVariavelAleatoria_Nenhum;
				IdVariavelAleatoriaInterna idVarInt = IdVariavelAleatoriaInterna_Nenhum;
				a_dados.processoEstocastico_hidrologico.getIdVariavelAleatoriaIdVariavelAleatoriaInternaFromIdFisico(idVar, idVarInt, idUHE);
				lista_hidreletrica_IdVariavelAleatoria.at(idUHE) = idVar;
				a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.att(idVar).setAtributo(AttComumVariavelAleatoria_nome, getFullString(idUHE));

		} // for (IdHidreletrica idUHE = IdHidreletrica(1); idUHE <= a_dados.getMaiorId(IdHidreletrica()); idUHE++) {


		const int numero_cenarios = a_dados.processoEstocastico_hidrologico.getSizeMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral);

		if (numero_cenarios == 0)
			throw std::invalid_argument("Processo estocastico pre config sem cenarios em " + getFullString(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral) + ".");

		a_dados.setAtributo(AttComumDados_numero_cenarios, numero_cenarios);

		entradaSaidaDados.setDiretorioEntrada(diretorio_entrada);
		entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("HIDRELETRICA_AFLUENCIA_AttVetorPremissa_PorPeriodo.csv", a_dados, TipoAcessoInstancia_membroMembro);


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
		dadosPreConfig_instanciados = entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("DADOS_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", a_dados, TipoAcessoInstancia_direto);

		if (dadosPreConfig_instanciados) {
			
			if ((a_dados.getSizeVetor(AttVetorDados_horizonte_estudo) == 0))
				throw std::invalid_argument("Para validar " + getFullString(AttMatrizDados_percentual_duracao_patamar_carga) + " deve ser instanciado na pre-config o " + getFullString(AttVetorDados_horizonte_estudo));

			const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

			//Tenta dar um get varrendo o horizonte_estudo
			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
				const double percentual_duracao_patamar_carga = a_dados.getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga_1, double());

		} // if ((a_dados.getSizeVetor(AttVetorDados_horizonte_estudo) > 0) && (dadosPreConfig_instanciados)) {

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

		const IdHidreletrica maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

		for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

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

		} // for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)){

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacoesUHE(a_dados): \n" + std::string(erro.what())); }
} // void LeituraCEPEL::aplicarModificacoesUHE(Dados &a_dados){


bool LeituraCEPEL::aplicarModificacaoIP(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_IP)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const double indisponibilidade_programada = a_modificacaoUHE.valor_1;
		a_dados.vetorHidreletrica.att(a_idHidreletrica).setAtributo(AttComumHidreletrica_indisponibilidade_programada, indisponibilidade_programada);
		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoIP(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoIP(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE) {

bool LeituraCEPEL::aplicarModificacaoTEIF(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_TEIF)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const double indisponibilidade_forcada = a_modificacaoUHE.valor_1;
		a_dados.vetorHidreletrica.att(a_idHidreletrica).setAtributo(AttComumHidreletrica_indisponibilidade_forcada, indisponibilidade_forcada);
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
				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.add(conjuntohidraulico);
			}
		}//if (idConjunto > maiorIdConjuntoHidraulico) {

		if (idConjunto <= maiorIdConjuntoHidraulico) {
			for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico(idConjunto + 1); idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {

				const IdUnidadeUHE maiorIdUnidadeUHE = a_dados.getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE());

				for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= maiorIdUnidadeUHE; idUnidade++)
					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidade).setVetor(AttVetorUnidadeUHE_disponibilidade, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

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
				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.add(unidadeUHE);

				if (a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada) == 0)
					a_dados.vetorHidreletrica.att(a_idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_forcada, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_indisponibilidade_forcada, double())));

				if (a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada) == 0)
					a_dados.vetorHidreletrica.att(a_idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_programada, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_indisponibilidade_programada, double())));

				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidade).setVetor(AttVetorUnidadeUHE_disponibilidade, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

				for (Periodo periodo = a_modificacaoUHE.periodo; periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

					const double indisponibilidade_forcada = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada, periodo, double());
					const double indisponibilidade_programada = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada, periodo, double());
					const double disponibilidade = (1.0 - indisponibilidade_forcada) * (1.0 - indisponibilidade_programada);

					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidade).setElemento(AttVetorUnidadeUHE_disponibilidade, periodo, disponibilidade);

				} // for (Periodo periodo = disponibilidade_unidade.getIteradorInicial(); periodo <= disponibilidade_unidade.getIteradorFinal(); disponibilidade_unidade.incrementarIterador(periodo)) {

			}//for (IdUnidadeUHE idUnidade = maiorIdUnidadeUHE; idUnidade <= IdUnidadeUHE(numero_maquinas); idUnidade++) {

		} // if (numero_maquinas > int(maiorIdUnidadeUHE)) {

		else if (numero_maquinas > int(maiorIdUnidadeUHE)) {

			for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= IdUnidadeUHE(numero_maquinas); idUnidade++) {

				if (idUnidade >= IdUnidadeUHE(maiorIdUnidadeUHE + 1)) {
					UnidadeUHE unidadeUHE;
					unidadeUHE.setAtributo(AttComumUnidadeUHE_idUnidadeUHE, idUnidade);

					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.add(unidadeUHE);

				}//if (idUnidade >= IdUnidadeUHE(maiorIdUnidadeUHE + 1)) {

				if (a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidade).getSizeVetor(AttVetorUnidadeUHE_disponibilidade) == 0) {

					if (a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada) == 0)
						a_dados.vetorHidreletrica.att(a_idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_forcada, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_indisponibilidade_forcada, double())));

					if (a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada) == 0)
						a_dados.vetorHidreletrica.att(a_idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_programada, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_indisponibilidade_programada, double())));

					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidade).setVetor(AttVetorUnidadeUHE_disponibilidade, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

					for (Periodo periodo = a_modificacaoUHE.periodo; periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						const double indisponibilidade_forcada = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada, periodo, double());
						const double indisponibilidade_programada = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada, periodo, double());
						const double disponibilidade = (1.0 - indisponibilidade_forcada) * (1.0 - indisponibilidade_programada);

						a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidade).setElemento(AttVetorUnidadeUHE_disponibilidade, periodo, disponibilidade);

					} // for (Periodo periodo = disponibilidade_unidade.getIteradorInicial(); periodo <= disponibilidade_unidade.getIteradorFinal(); disponibilidade_unidade.incrementarIterador(periodo)) {

				}//if (a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidade).getSizeVetor(AttVetorUnidadeUHE_disponibilidade) == 0) {
				else {

					if (a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidade).getElementoVetor(AttVetorUnidadeUHE_disponibilidade, a_modificacaoUHE.periodo, double()) == 0) {

						if (a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada) == 0)
							a_dados.vetorHidreletrica.att(a_idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_forcada, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_indisponibilidade_forcada, double())));

						if (a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada) == 0)
							a_dados.vetorHidreletrica.att(a_idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_programada, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_indisponibilidade_programada, double())));

						for (Periodo periodo = a_modificacaoUHE.periodo; periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							const double indisponibilidade_forcada = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada, periodo, double());
							const double indisponibilidade_programada = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada, periodo, double());
							const double disponibilidade = (1.0 - indisponibilidade_forcada) * (1.0 - indisponibilidade_programada);

							a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidade).setElemento(AttVetorUnidadeUHE_disponibilidade, periodo, disponibilidade);

						} // for (Periodo periodo = disponibilidade_unidade.getIteradorInicial(); periodo <= disponibilidade_unidade.getIteradorFinal(); disponibilidade_unidade.incrementarIterador(periodo)) {

					}//if (a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidade).getElementoVetor(AttVetorUnidadeUHE_disponibilidade, a_modificacaoUHE.periodo, double()) == 0) {

				}//else {

			}//for (IdUnidadeUHE idUnidade = maiorIdUnidadeUHE; idUnidade <= IdUnidadeUHE(numero_maquinas); idUnidade++) {

		} // else if (numero_maquinas > int(maiorIdUnidadeUHE)) {

		else if (numero_maquinas <= int(maiorIdUnidadeUHE)) {

			for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= maiorIdUnidadeUHE; idUnidade++) {

				if (int(idUnidade) <= numero_maquinas) {

					if (a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidade).getSizeVetor(AttVetorUnidadeUHE_disponibilidade) == 0) {

						if (a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada) == 0)
							a_dados.vetorHidreletrica.att(a_idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_forcada, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_indisponibilidade_forcada, double())));

						if (a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada) == 0)
							a_dados.vetorHidreletrica.att(a_idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_programada, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_indisponibilidade_programada, double())));

						a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidade).setVetor(AttVetorUnidadeUHE_disponibilidade, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

						for (Periodo periodo = a_modificacaoUHE.periodo; periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							const double indisponibilidade_forcada = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada, periodo, double());
							const double indisponibilidade_programada = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada, periodo, double());
							const double disponibilidade = (1.0 - indisponibilidade_forcada) * (1.0 - indisponibilidade_programada);

							a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidade).setElemento(AttVetorUnidadeUHE_disponibilidade, periodo, disponibilidade);

						} // for (Periodo periodo = disponibilidade_unidade.getIteradorInicial(); periodo <= disponibilidade_unidade.getIteradorFinal(); disponibilidade_unidade.incrementarIterador(periodo)) {

					}//if (a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidade).getSizeVetor(AttVetorUnidadeUHE_disponibilidade) == 0) {
					else {

						if (a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidade).getElementoVetor(AttVetorUnidadeUHE_disponibilidade, a_modificacaoUHE.periodo, double()) == 0) {

							if (a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada) == 0)
								a_dados.vetorHidreletrica.att(a_idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_forcada, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_indisponibilidade_forcada, double())));

							if (a_dados.getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada) == 0)
								a_dados.vetorHidreletrica.att(a_idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_programada, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.getAtributo(a_idHidreletrica, AttComumHidreletrica_indisponibilidade_programada, double())));

							for (Periodo periodo = a_modificacaoUHE.periodo; periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								const double indisponibilidade_forcada = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada, periodo, double());
								const double indisponibilidade_programada = a_dados.getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada, periodo, double());
								const double disponibilidade = (1.0 - indisponibilidade_forcada) * (1.0 - indisponibilidade_programada);

								a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidade).setElemento(AttVetorUnidadeUHE_disponibilidade, periodo, disponibilidade);

							} // for (Periodo periodo = disponibilidade_unidade.getIteradorInicial(); periodo <= disponibilidade_unidade.getIteradorFinal(); disponibilidade_unidade.incrementarIterador(periodo)) {

						}//if (a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidade).getElementoVetor(AttVetorUnidadeUHE_disponibilidade, a_modificacaoUHE.periodo, double()) == 0) {

					}//else {

				}//if (int(idUnidade) <= numero_maquinas) {
				else
					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidade).setVetor(AttVetorUnidadeUHE_disponibilidade, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

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

		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_volume_minimo, volume_minimo_novo);

		if (a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getSizeVetor(AttVetorReservatorio_volume_minimo) > 0) {

			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_minimo, periodo, volume_minimo_novo);

		}//if (a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getSizeVetor(AttVetorReservatorio_volume_minimo) > 0) {

		//////////////////////////////////////////
		const double cota_referencia = a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getCotaMedia(a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_minimo, double()), a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_maximo, double()));

		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_cota_referencia, cota_referencia);

		if (a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getSizeVetor(AttVetorReservatorio_cota_referencia) > 0) {

			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_cota_referencia, periodo, cota_referencia);

		}//if (a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getSizeVetor(AttVetorReservatorio_volume_minimo) > 0) {

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

		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_volume_maximo, volume_maximo_novo);

		if (a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getSizeVetor(AttVetorReservatorio_volume_maximo) > 0) {

			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_maximo, periodo, volume_maximo_novo);

		}//if (a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getSizeVetor(AttVetorReservatorio_volume_minimo) > 0) {


		//////////////////////////////////////////
		const double cota_referencia = a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getCotaMedia(a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_minimo, double()), a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_maximo, double()));

		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_cota_referencia, cota_referencia);

		if (a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getSizeVetor(AttVetorReservatorio_cota_referencia) > 0) {

			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_cota_referencia, periodo, cota_referencia);

		}//if (a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getSizeVetor(AttVetorReservatorio_volume_minimo) > 0) {


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
			a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjunto).vetorUnidadeUHE.att(idUnidade).setAtributo(AttComumUnidadeUHE_potencia_maxima, potencia_maxima_maquina);

		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoPOTEFE(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoPOTEFE(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE){

bool LeituraCEPEL::aplicarModificacaoNUMBAS(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_NUMBAS)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const int numero_maquinas_base = int(a_modificacaoUHE.valor_1);
		a_dados.vetorHidreletrica.att(a_idHidreletrica).setAtributo(AttComumHidreletrica_numero_maquinas_base, numero_maquinas_base);
		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoNUMBAS(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoNUMBAS(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE){

bool LeituraCEPEL::aplicarModificacaoVAZMIN(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_VAZMIN)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const double vazao_defluente_minima = a_modificacaoUHE.valor_1;
		a_dados.vetorHidreletrica.att(a_idHidreletrica).setAtributo(AttComumHidreletrica_vazao_defluente_minima, vazao_defluente_minima);
		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoVAZMIN(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoVAZMIN(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE){


bool LeituraCEPEL::aplicarModificacaoVAZMINHISTORICA(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_VAZMINHISTORICA)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const double vazao_defluente_disponivel_minima_historica = a_modificacaoUHE.valor_1;

		a_dados.vetorHidreletrica.att(a_idHidreletrica).setAtributo(AttComumHidreletrica_vazao_defluente_minima_historica, vazao_defluente_disponivel_minima_historica);

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

		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_area_0, valor_0);
		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_area_1, valor_1);
		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_area_1, valor_2);
		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_area_2, valor_3);
		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_area_3, valor_4);

		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_cota_referencia, a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getCotaMedia(a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_minimo, double()), a_dados.getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_maximo, double())));

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

		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_area_0, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_area_1, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_area_2, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_area_3, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_area_4, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

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

					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_poli_cota_area_0, periodo, valor_antigo_0 + sobreposicao * cota_area_0_DECK.at(periodo_DECK));
					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_poli_cota_area_1, periodo, valor_antigo_1 + sobreposicao * cota_area_1_DECK.at(periodo_DECK));
					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_poli_cota_area_2, periodo, valor_antigo_2 + sobreposicao * cota_area_2_DECK.at(periodo_DECK));
					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_poli_cota_area_3, periodo, valor_antigo_3 + sobreposicao * cota_area_3_DECK.at(periodo_DECK));
					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_poli_cota_area_4, periodo, valor_antigo_4 + sobreposicao * cota_area_4_DECK.at(periodo_DECK));

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

		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_cota_referencia, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

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

					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_cota_referencia, periodo, valor_antigo + sobreposicao * cota_referencia_DECK.at(periodo_DECK));

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

		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_volume_0, valor_0);
		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_volume_1, valor_1);
		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_volume_1, valor_2);
		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_volume_2, valor_3);
		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_volume_3, valor_4);
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

		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_volume_0, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_volume_1, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_volume_2, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_volume_3, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_volume_4, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

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

					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_poli_cota_volume_0, periodo, valor_antigo_0 + sobreposicao * cota_volume_0_DECK.at(periodo_DECK));
					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_poli_cota_volume_1, periodo, valor_antigo_1 + sobreposicao * cota_volume_1_DECK.at(periodo_DECK));
					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_poli_cota_volume_2, periodo, valor_antigo_2 + sobreposicao * cota_volume_2_DECK.at(periodo_DECK));
					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_poli_cota_volume_3, periodo, valor_antigo_3 + sobreposicao * cota_volume_3_DECK.at(periodo_DECK));
					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_poli_cota_volume_4, periodo, valor_antigo_4 + sobreposicao * cota_volume_4_DECK.at(periodo_DECK));

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
				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).setAtributo(AttComumUnidadeUHE_fator_de_producao, fator_de_producao);
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

		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_evaporacao, mes, evaporacao);
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

					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).setAtributo(AttComumUnidadeUHE_tipo_de_perda_hidraulica, TipoPerdaHidraulica_percentual);

				} // if (tipo_de_grandeza == TipoGrandezaModificacao_percentual) {

				else if (tipo_de_grandeza == TipoGrandezaModificacao_absoluta)
					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).setAtributo(AttComumUnidadeUHE_tipo_de_perda_hidraulica, TipoPerdaHidraulica_metro);

				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).setAtributo(AttComumUnidadeUHE_perda_hidraulica, perda_hidraulica);

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

		a_dados.vetorHidreletrica.att(a_idHidreletrica).setVetor(AttVetorHidreletrica_canal_fuga_medio, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

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
					a_dados.vetorHidreletrica.att(a_idHidreletrica).setElemento(AttVetorHidreletrica_canal_fuga_medio, periodo, valor_antigo + sobreposicao * canal_fuga_medio_DECK.at(periodo_DECK));

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

		a_dados.vetorHidreletrica.att(a_idHidreletrica).setVetor(AttVetorHidreletrica_vazao_defluente_minima, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

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

					a_dados.vetorHidreletrica.att(a_idHidreletrica).setElemento(AttVetorHidreletrica_vazao_defluente_minima, periodo, valor_antigo + sobreposicao * vazao_defluente_minima_DECK.at(periodo_DECK));

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

		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_volume_maximo, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

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

					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_maximo, periodo, valor_antigo + sobreposicao * volume_maximo_DECK.at(periodo_DECK));

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

		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_volume_minimo, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

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

					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_minimo, periodo, valor_antigo + sobreposicao * volume_minimo_DECK.at(periodo_DECK));

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
					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_volume_util_minimo, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
				}

				for (Periodo periodo = horizonte_estudo_DECK.getIteradorInicial(); periodo <= horizonte_estudo_DECK.getIteradorFinal(); horizonte_estudo_DECK.incrementarIterador(periodo)) {
					if (periodo >= periodo_modificacao)
						volume_util_minimo_DECK.at(periodo) = valor_modificacao;
				}
					
			}

			else if (tipo_de_grandeza == TipoGrandezaModificacao_percentual) {

				if (percentual_volume_util_minimo_DECK.size() == 0) {
					percentual_volume_util_minimo_DECK = SmartEnupla<Periodo, double>(horizonte_estudo_DECK, percentual_volume_util_minimo);
					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_percentual_volume_util_minimo, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
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

						a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_util_minimo, periodo, valor_antigo + sobreposicao * volume_util_minimo_DECK.at(periodo_DECK));

					}

					if (percentual_volume_util_minimo_DECK.size() > 0) {

						const double valor_antigo = a_dados.getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_percentual_volume_util_minimo, periodo, double());

						a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_percentual_volume_util_minimo, periodo, valor_antigo + sobreposicao * percentual_volume_util_minimo_DECK.at(periodo_DECK));

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
				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_morto_completo, periodo, 0);
				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_enchendo_volume_morto, periodo, 1);
			}

			else if (periodo < periodo_enchendo_volume_morto) {
				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_morto_completo, periodo, 0);
				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_enchendo_volume_morto, periodo, 0);
			} // if (periodo < periodo_enchendo_volume_morto) {

			else if (periodo < periodo_volume_morto_completo) {
				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_morto_completo, periodo, 0);
				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_enchendo_volume_morto, periodo, 1);
			} // else if ((periodo_enchendo_volume_morto <= periodo) && (periodo < periodo_volume_morto_completo)) {

			else {
				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_morto_completo, periodo, 1);
				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_enchendo_volume_morto, periodo, 0);
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
				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).setVetor(AttVetorUnidadeUHE_disponibilidade, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

		}

		else {

			if (!doubleCompara(0.01, a_dados.getAtributo(a_idHidreletrica, idConjuntoHidraulico, maiorIdUnidadeUHE, AttComumUnidadeUHE_potencia_maxima, double()), potencia_maxima_maquina))
				throw std::invalid_argument("Potencia maxima da maquina de " + getString(potencia_maxima_maquina) + " MW nao compativel com " + getFullString(idConjuntoHidraulico) + ".");

			UnidadeUHE unidadeUHE(a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(maiorIdUnidadeUHE));
			unidadeUHE.setAtributo(AttComumUnidadeUHE_idUnidadeUHE, idUnidadeUHE);
			a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.add(unidadeUHE);
			a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).setVetor(AttVetorUnidadeUHE_disponibilidade, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

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

				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).setElemento(AttVetorUnidadeUHE_disponibilidade, periodo, sobreposicao * disponibilidade);

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
		a_dados.vetorHidreletrica.att(a_idHidreletrica).setAtributo(AttComumHidreletrica_nome, nome_hidreletrica);
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
			a_dados.vetorHidreletrica.att(a_idHidreletrica).setAtributo(AttComumHidreletrica_jusante, idHidreletrica_jusante);
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

		a_dados.vetorHidreletrica.att(a_idHidreletrica).setAtributo(AttComumHidreletrica_jusante_desvio, idHidreletrica_jusante_desvio);

		if (hidreletricasPreConfig_instanciadas)
			lista_jusante_desvio_hidreletrica.setElemento(a_idHidreletrica, idHidreletrica_jusante_desvio);

		a_dados.vetorHidreletrica.att(a_idHidreletrica).setVetor(AttVetorHidreletrica_vazao_desviada_minima, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
		a_dados.vetorHidreletrica.att(a_idHidreletrica).setVetor(AttVetorHidreletrica_vazao_desviada_maxima, SmartEnupla<Periodo, double>(horizonte_estudo, desvio_agua_maximo));

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

			a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico_novo).setAtributo(AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico_desvio);

			a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico_novo).setAtributo(AttComumIntercambioHidraulico_hidreletrica_origem, a_idHidreletrica);
			a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico_novo).setAtributo(AttComumIntercambioHidraulico_hidreletrica_destino, idHidreletrica_jusante_desvio);

			//Set desvio_agua_minimo e desvio_agua_maximo

			const SmartEnupla<Periodo, double> vetor_zero(horizonte_estudo, 0.0);
			const SmartEnupla<Periodo, double> desvio_agua_maximo(horizonte_estudo, desvio_agua_maximo);

			a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico_novo).setVetor(AttVetorIntercambioHidraulico_desvio_agua_minimo, vetor_zero);
			a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico_novo).setVetor(AttVetorIntercambioHidraulico_desvio_agua_maximo, desvio_agua_maximo);

		}//if (idIntercambioHidraulico == IdIntercambioHidraulico_Nenhum) {
		else {

			//Atualiza desvio_agua_maximo

			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
				a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setElemento(AttVetorIntercambioHidraulico_desvio_agua_maximo, periodo, desvio_agua_maximo);

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

		//if(a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.getIdObjetos(IdConjuntoHidraulico(numero_conjunto), IdConjuntoHidraulico()).size() == 0)
			//throw std::invalid_argument("Modificacao VAZEFE indica Conjunto Hidraulico nao inicializado com IdConjuntoHidraulico" + getString(numero_conjunto));

		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico(numero_conjunto)).setVetor(AttVetorConjuntoHidraulico_vazao_turbinada_maxima, SmartEnupla<Periodo, double>(horizonte_estudo, turbinamento_maximo));

		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoVAZEFE(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoIP(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE) {

bool LeituraCEPEL::aplicarModificacaoVMDESV(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_VMDESV)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const double volume_minimo_desvio = a_modificacaoUHE.valor_1;

		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_volume_minimo_desvio, volume_minimo_desvio);

		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoVMDESV(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoIP(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE) {

bool LeituraCEPEL::aplicarModificacaoNUMPOS(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_NUMPOS)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const int codigo_posto = int(a_modificacaoUHE.valor_1);

		a_dados.vetorHidreletrica.att(a_idHidreletrica).setAtributo(AttComumHidreletrica_codigo_posto, codigo_posto);

		return true;
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoNUMPOS(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoIP(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE) {

bool LeituraCEPEL::aplicarModificacaoNPOSNW(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {
		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_NPOSNW)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		lista_hidreletrica_NPOSNW.setElemento(a_idHidreletrica, int(a_modificacaoUHE.valor_1));

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

			if (altura_ref == a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorPolinomioJusante.att(idPolinomioJusante_teste).getElementoVetor(AttVetorPolinomioJusante_altura_ref, horizonte_estudo.getIteradorInicial(), double())) {

				idPolinomioJusante = idPolinomioJusante_teste;
				break;

			}//if (altura_ref == a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorPolinomioJusante.att(idPolinomioJusante_teste).getElementoVetor(AttVetorPolinomioJusante_altura_ref, horizonte_estudo.getIteradorInicial(), double())) {

		}//for (IdPolinomioJusante idPolinomioJusante_teste = IdPolinomioJusante_1; idPolinomioJusante_teste <= maiorIdPolinomioJusante; idPolinomioJusante_teste++) {

		//Cria um novo Polinomio Jusante se nao existir um com a altura_ref indicada

		if (idPolinomioJusante == IdPolinomioJusante_Nenhum) {

			PolinomioJusante polinomioJusante;

			idPolinomioJusante = IdPolinomioJusante(maiorIdPolinomioJusante + 1);

			polinomioJusante.setAtributo(AttComumPolinomioJusante_idPolinomioJusante, idPolinomioJusante);

			a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorPolinomioJusante.add(polinomioJusante);

			a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorPolinomioJusante.att(idPolinomioJusante).setAtributo(AttComumPolinomioJusante_nome, a_dados.vetorHidreletrica.att(a_idHidreletrica).getAtributo(AttComumHidreletrica_nome, std::string()));
			a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorPolinomioJusante.att(idPolinomioJusante).setVetor(AttVetorPolinomioJusante_altura_ref, SmartEnupla<Periodo, double>(horizonte_estudo, altura_ref));

		}//if (idPolinomioJusante == IdPolinomioJusante_Nenhum) {

		a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorPolinomioJusante.att(idPolinomioJusante).setAtributo(AttComumPolinomioJusante_curva_chave, curva_chave);

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
			if (curva_chave == a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorPolinomioJusante.att(idPolinomioJusante_teste).getAtributo(AttComumPolinomioJusante_curva_chave, int())) {
				idPolinomioJusante = idPolinomioJusante_teste;
				break;
			}//if (curva_chave == a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorPolinomioJusante.att(idPolinomioJusante_teste).getAtributo(AttComumPolinomioJusante_curva_chave, double())) {
		}//for (IdPolinomioJusante idPolinomioJusante_teste = IdPolinomioJusante_1; idPolinomioJusante_teste <= maiorIdPolinomioJusante; idPolinomioJusante_teste++) {

		//Cria um novo Polinomio Jusante se nao existir um com a altura_ref indicada

		if (idPolinomioJusante == IdPolinomioJusante_Nenhum)
			throw std::invalid_argument("Nao existe polinomio jusante inicializado com curva chave_" + getString(curva_chave));

		if (a_modificacaoUHE.periodo == horizonte_estudo.getIteradorInicial()) {

			if (coeficiente == 1)
				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorPolinomioJusante.att(idPolinomioJusante).setVetor(AttVetorPolinomioJusante_coeficiente_0, SmartEnupla<Periodo, double>(horizonte_estudo, valor_coeficiente));
			else if (coeficiente == 2)
				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorPolinomioJusante.att(idPolinomioJusante).setVetor(AttVetorPolinomioJusante_coeficiente_1, SmartEnupla<Periodo, double>(horizonte_estudo, valor_coeficiente));
			else if (coeficiente == 3)
				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorPolinomioJusante.att(idPolinomioJusante).setVetor(AttVetorPolinomioJusante_coeficiente_2, SmartEnupla<Periodo, double>(horizonte_estudo, valor_coeficiente));
			else if (coeficiente == 4)
				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorPolinomioJusante.att(idPolinomioJusante).setVetor(AttVetorPolinomioJusante_coeficiente_3, SmartEnupla<Periodo, double>(horizonte_estudo, valor_coeficiente));
			else if (coeficiente == 5)
				a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorPolinomioJusante.att(idPolinomioJusante).setVetor(AttVetorPolinomioJusante_coeficiente_4, SmartEnupla<Periodo, double>(horizonte_estudo, valor_coeficiente));
			else
				throw std::invalid_argument("Coeficiente nao identificado para o polinomio jusante inicializado com curva chave_" + getString(curva_chave));

		}//if (a_modificacaoUHE.periodo == horizonte_estudo.getIteradorInicial()) {
		else {

			for (Periodo periodo = a_modificacaoUHE.periodo; periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				if (coeficiente == 1)
					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorPolinomioJusante.att(idPolinomioJusante).setElemento(AttVetorPolinomioJusante_coeficiente_0, periodo, valor_coeficiente);
				else if (coeficiente == 2)
					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorPolinomioJusante.att(idPolinomioJusante).setElemento(AttVetorPolinomioJusante_coeficiente_1, periodo, valor_coeficiente);
				else if (coeficiente == 3)
					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorPolinomioJusante.att(idPolinomioJusante).setElemento(AttVetorPolinomioJusante_coeficiente_2, periodo, valor_coeficiente);
				else if (coeficiente == 4)
					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorPolinomioJusante.att(idPolinomioJusante).setElemento(AttVetorPolinomioJusante_coeficiente_3, periodo, valor_coeficiente);
				else if (coeficiente == 5)
					a_dados.vetorHidreletrica.att(a_idHidreletrica).vetorPolinomioJusante.att(idPolinomioJusante).setElemento(AttVetorPolinomioJusante_coeficiente_4, periodo, valor_coeficiente);
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
			a_dados.vetorHidreletrica.att(a_idHidreletrica).setAtributo(AttComumHidreletrica_vertimento_influencia_fcj, true);
		else if (vertimento_influencia_fcj == 0)
			a_dados.vetorHidreletrica.att(a_idHidreletrica).setAtributo(AttComumHidreletrica_vertimento_influencia_fcj, false);

		return true;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::aplicarModificacaoVERTJU(a_dados, " + getFullString(a_idHidreletrica) + ",a_modificacaoUHE,a_horizonte_estudo): \n" + std::string(erro.what())); }
} // bool LeituraCEPEL::aplicarModificacaoVERTJU(Dados &a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE &a_modificacaoUHE, const SmartEnupla<Periodo, IdEstagio>& a_horizonte_estudo){

bool LeituraCEPEL::aplicarModificacaoJUSENA(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE) {
	try {

		if (a_modificacaoUHE.tipo_de_modificacao != TipoModificacaoUHE_JUSENA)
			throw std::invalid_argument("Modificacao nao compativel com metodo.");

		const int codigo_usina_jusante_JUSENA = int(a_modificacaoUHE.valor_1);

		a_dados.vetorHidreletrica.att(a_idHidreletrica).setAtributo(AttComumHidreletrica_codigo_usina_jusante_JUSENA, codigo_usina_jusante_JUSENA);

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

		for (IdTermeletrica idTermeletrica = a_dados.getMenorId(IdTermeletrica()); idTermeletrica <= a_dados.getMaiorId(IdTermeletrica()); a_dados.vetorTermeletrica.incr(idTermeletrica)) {

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

						a_dados.vetorTermeletrica.att(idTermeletrica).setVetor(AttVetorTermeletrica_indisponibilidade_programada, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

					} // if (lista_modificacaoUTE.at(idTermeletrica).at(i).tipo_de_modificacao == TipoModificacaoUTE_IPTER) {

					else if ((lista_modificacaoUTE.at(idTermeletrica).at(i).tipo_de_modificacao == TipoModificacaoUTE_TEIFT) && (lista_TEIFT.size() == 0)) {
						lista_TEIFT = SmartEnupla<Periodo, double>(horizonte_estudo_DECK, a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_indisponibilidade_forcada, double()));

						a_dados.vetorTermeletrica.att(idTermeletrica).setVetor(AttVetorTermeletrica_indisponibilidade_forcada, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
					}

					else if ((lista_modificacaoUTE.at(idTermeletrica).at(i).tipo_de_modificacao == TipoModificacaoUTE_FCMAX) && (lista_FCMAX.size() == 0)) {
						lista_FCMAX = SmartEnupla<Periodo, double>(horizonte_estudo_DECK, a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_fator_de_capacidade, double()));

						a_dados.vetorTermeletrica.att(idTermeletrica).setVetor(AttVetorTermeletrica_fator_de_capacidade, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
					}

					else if ((lista_modificacaoUTE.at(idTermeletrica).at(i).tipo_de_modificacao == TipoModificacaoUTE_POTEF) && (lista_POTEF.size() == 0)) {
						lista_POTEF = SmartEnupla<Periodo, double>(horizonte_estudo_DECK, a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_potencia_maxima, double()));

						a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).setMatriz_forced(AttMatrizUnidadeUTE_potencia_maxima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, horizonte_estudo.getIteradorInicial(), IdPatamarCarga()), 0.0))));
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

						a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).setMatriz_forced(AttMatrizUnidadeUTE_potencia_minima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, horizonte_estudo.getIteradorInicial(), IdPatamarCarga()), 0.0))));

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
								const double valor_anterior = a_dados.vetorTermeletrica.att(idTermeletrica).getElementoVetor(AttVetorTermeletrica_indisponibilidade_programada, periodo, double());
								const double valor_novo = valor_anterior + sobreposicao * lista_IPTER.at(periodo_DECK);
								a_dados.vetorTermeletrica.att(idTermeletrica).setElemento(AttVetorTermeletrica_indisponibilidade_programada, periodo, valor_novo);
							}

							if (lista_TEIFT.size() > 0) {
								const double valor_anterior = a_dados.vetorTermeletrica.att(idTermeletrica).getElementoVetor(AttVetorTermeletrica_indisponibilidade_forcada, periodo, double());
								const double valor_novo = valor_anterior + sobreposicao * lista_TEIFT.at(periodo_DECK);
								a_dados.vetorTermeletrica.att(idTermeletrica).setElemento(AttVetorTermeletrica_indisponibilidade_forcada, periodo, valor_novo);
							}

							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {

								if (lista_GTMIN.size() > 0) {
									const double valor_anterior = a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).getElementoMatriz(AttMatrizUnidadeUTE_potencia_minima, periodo, idPatamarCarga, double());
									const double valor_novo = valor_anterior + sobreposicao * lista_GTMIN.at(periodo_DECK);

									a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).setElemento(AttMatrizUnidadeUTE_potencia_minima, periodo, idPatamarCarga, valor_novo);
								}
								if (lista_POTEF.size() > 0) {
									const double valor_anterior = a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).getElementoMatriz(AttMatrizUnidadeUTE_potencia_maxima, periodo, idPatamarCarga, double());
									const double valor_novo = valor_anterior + sobreposicao * lista_POTEF.at(periodo_DECK);
									a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).setElemento(AttMatrizUnidadeUTE_potencia_maxima, periodo, idPatamarCarga, valor_novo);
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
								const double valor_anterior = a_dados.vetorTermeletrica.att(idTermeletrica).getElementoVetor(AttVetorTermeletrica_fator_de_capacidade, periodo, double());
								const double valor_novo = valor_anterior + sobreposicao * lista_FCMAX.at(periodo_DECK);
								a_dados.vetorTermeletrica.att(idTermeletrica).setElemento(AttVetorTermeletrica_fator_de_capacidade, periodo, valor_novo);

								if (valor_novo == 0.0) {
									if (a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).getSizeMatriz(AttMatrizUnidadeUTE_potencia_minima) > 0) {
										if (a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).getSizeMatriz(AttMatrizUnidadeUTE_potencia_minima, periodo) > 0) {
											for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {
												a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).setElemento(AttMatrizUnidadeUTE_potencia_minima, periodo, idPatamarCarga, 0.0);
											}
										}
									}
									if (a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).getSizeMatriz(AttMatrizUnidadeUTE_potencia_util) > 0) {
										if (a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).getSizeMatriz(AttMatrizUnidadeUTE_potencia_util, periodo) > 0) {
											for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {
												a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(IdUnidadeUTE_1).setElemento(AttMatrizUnidadeUTE_potencia_util, periodo, idPatamarCarga, 0.0);
											}
										}
									}
								}

							}

						} // if (sobreposicao > 0.0) {

					} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				} // for (Periodo periodo_DECK = horizonte_estudo_DECK.getIteradorInicial(); periodo_DECK <= horizonte_estudo_DECK.getIteradorFinal(); periodo_DECK++) {


			} // if (lista_modificacaoUTE.at(idTermeletrica).size() > 0){

		} // for (IdTermeletrica idTermeletrica = a_dados.getMenorId(IdTermeletrica()); idTermeletrica <= maiorIdTermeletrica; a_dados.vetorTermeletrica.incr(idTermeletrica)) {

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

			a_dados.vetorSubmercado.att(idSubmercado).setAtributo(AttComumSubmercado_nome, nome);

			//////////////////////////////////////////////////////////////
			//Instancia demanda = 0 para todos os patamares
			a_dados.vetorSubmercado.att(idSubmercado).setMatriz(AttMatrizSubmercado_demanda, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>()));

			for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo)) {

				const IdPatamarCarga maiorIdPatamarCarga = get_maiorIdPatamarCarga_periodo_from_percentual_duracao_patamar_carga(a_dados, periodo);

				if (a_dados.vetorSubmercado.att(idSubmercado).getElementosMatriz(AttMatrizSubmercado_demanda, periodo, IdPatamarCarga(), double()).size() == 0)
					a_dados.vetorSubmercado.att(idSubmercado).setElementos(AttMatrizSubmercado_demanda, periodo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(maiorIdPatamarCarga, 0.0)));

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

				a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_submercado_origem, idSubmercado_origem);
				a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_submercado_destino, idSubmercado_destino);

				a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_nome, nome_submercado_origem + "->" + nome_submercado_destino);

				for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

					const IdPatamarCarga maiorIdPatamarCarga = get_maiorIdPatamarCarga_periodo_from_percentual_duracao_patamar_carga(a_dados, periodo);

					for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
						a_dados.vetorIntercambio.att(idIntercambio).addElemento(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, potencia_minima);
						a_dados.vetorIntercambio.att(idIntercambio).addElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, potencia_maxima);
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

				a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_submercado_origem, idSubmercado_origem);
				a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_submercado_destino, idSubmercado_destino);

				a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_nome, nome_submercado_origem + "->" + nome_submercado_destino);

				for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

					const IdPatamarCarga maiorIdPatamarCarga = get_maiorIdPatamarCarga_periodo_from_percentual_duracao_patamar_carga(a_dados, periodo);

					for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
						a_dados.vetorIntercambio.att(idIntercambio).addElemento(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, potencia_minima);
						a_dados.vetorIntercambio.att(idIntercambio).addElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, potencia_maxima);
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
					a_dados.vetorIntercambio.att(idIntercambio_S_SE).setAtributo(AttComumIntercambio_submercado_destino, idSubmercadoIV);
					a_dados.vetorIntercambio.att(idIntercambio_S_SE).setAtributo(AttComumIntercambio_nome, std::string("SUL->IV"));
					a_dados.vetorIntercambio.att(idIntercambio_SE_S).setAtributo(AttComumIntercambio_submercado_origem, idSubmercadoIV);
					a_dados.vetorIntercambio.att(idIntercambio_SE_S).setAtributo(AttComumIntercambio_nome, std::string("IV->SUL"));
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

					a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_submercado_origem, idSubmercado_origem);
					a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_submercado_destino, idSubmercado_destino);

					a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_nome, nome_submercado_origem + "->" + nome_submercado_destino);

					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						const IdPatamarCarga maiorIdPatamarCarga = get_maiorIdPatamarCarga_periodo_from_percentual_duracao_patamar_carga(a_dados, periodo);

						for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
							a_dados.vetorIntercambio.att(idIntercambio).addElemento(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, potencia_minima);
							a_dados.vetorIntercambio.att(idIntercambio).addElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, potencia_maxima);
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

					a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_submercado_origem, idSubmercado_origem);
					a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_submercado_destino, idSubmercado_destino);

					a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_nome, nome_submercado_origem + "->" + nome_submercado_destino);

					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						const IdPatamarCarga maiorIdPatamarCarga = get_maiorIdPatamarCarga_periodo_from_percentual_duracao_patamar_carga(a_dados, periodo);

						for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
							a_dados.vetorIntercambio.att(idIntercambio).addElemento(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, potencia_minima);
							a_dados.vetorIntercambio.att(idIntercambio).addElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, potencia_maxima);
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

			a_dados.vetorSubmercado.att(idSubmercado).setAtributo(AttComumSubmercado_nome, nome);

			//////////////////////////////////////////////////////////////
			//Instancia demanda = 0 para todos os patamares
			a_dados.vetorSubmercado.att(idSubmercado).setMatriz(AttMatrizSubmercado_demanda, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>()));

			for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo)) {

				const IdPatamarCarga maiorIdPatamarCarga = get_maiorIdPatamarCarga_periodo_from_percentual_duracao_patamar_carga(a_dados, periodo);

				if (a_dados.vetorSubmercado.att(idSubmercado).getElementosMatriz(AttMatrizSubmercado_demanda, periodo, IdPatamarCarga(), double()).size() == 0)
					a_dados.vetorSubmercado.att(idSubmercado).setElementos(AttMatrizSubmercado_demanda, periodo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(maiorIdPatamarCarga, 0.0)));

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

				a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_submercado_origem, idSubmercado_origem);
				a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_submercado_destino, idSubmercado_destino);

				a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_nome, nome_submercado_origem + "->" + nome_submercado_destino);

				for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

					const IdPatamarCarga maiorIdPatamarCarga = get_maiorIdPatamarCarga_periodo_from_percentual_duracao_patamar_carga(a_dados, periodo);

					for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
						a_dados.vetorIntercambio.att(idIntercambio).addElemento(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, potencia_minima);
						a_dados.vetorIntercambio.att(idIntercambio).addElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, potencia_maxima);
					}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

				}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {	

			}//if (idSubmercado_inicializado == IdSubmercado_Nenhum) {

			//////////////////////////////////////
			//PatamarDeficit ANDE igual ao Sudeste
			//////////////////////////////////////

			a_dados.vetorSubmercado.att(idSubmercado).vetorPatamarDeficit = a_dados.vetorSubmercado.att(idSubmercadoSE).vetorPatamarDeficit;

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

				a_dados.vetorSubmercado.att(idSubmercado).setAtributo(AttComumSubmercado_nome, nome);

				//////////////////////////////////////////////////////////////
				//Instancia demanda = 0 para todos os patamares
				a_dados.vetorSubmercado.att(idSubmercado).setMatriz(AttMatrizSubmercado_demanda, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>()));

				for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo)) {

					const IdPatamarCarga maiorIdPatamarCarga = get_maiorIdPatamarCarga_periodo_from_percentual_duracao_patamar_carga(a_dados, periodo);

					if (a_dados.vetorSubmercado.att(idSubmercado).getElementosMatriz(AttMatrizSubmercado_demanda, periodo, IdPatamarCarga(), double()).size() == 0)
						a_dados.vetorSubmercado.att(idSubmercado).setElementos(AttMatrizSubmercado_demanda, periodo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(maiorIdPatamarCarga, 0.0)));

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

					a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_submercado_origem, idSubmercado_origem);
					a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_submercado_destino, idSubmercado_destino);

					a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_nome, nome_submercado_origem + "->" + nome_submercado_destino);

					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						const IdPatamarCarga maiorIdPatamarCarga = get_maiorIdPatamarCarga_periodo_from_percentual_duracao_patamar_carga(a_dados, periodo);

						for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
							a_dados.vetorIntercambio.att(idIntercambio).addElemento(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, potencia_minima);
							a_dados.vetorIntercambio.att(idIntercambio).addElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, potencia_maxima);
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

					a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_submercado_origem, idSubmercado_origem);
					a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_submercado_destino, idSubmercado_destino);

					a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_nome, nome_submercado_origem + "->" + nome_submercado_destino);

					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						const IdPatamarCarga maiorIdPatamarCarga = get_maiorIdPatamarCarga_periodo_from_percentual_duracao_patamar_carga(a_dados, periodo);

						for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
							a_dados.vetorIntercambio.att(idIntercambio).addElemento(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, potencia_minima);
							a_dados.vetorIntercambio.att(idIntercambio).addElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, potencia_maxima);
						}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

					}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				}//if (true) {

			}//if (true) {

			const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, 66);

			a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_submercado, idSubmercado_inicializado);

		}//if (!itaipu_tipo_detalhamento_producao_por_conjunto) {


	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::inicializa_Submercados_Intercambios_Nao_Registrados(Dados& a_dados): \n" + std::string(erro.what())); }


}//void LeituraCEPEL::inicializa_Submercados_Intercambios_Nao_Registrados() {


void LeituraCEPEL::adicionaIntercambiosHidraulicosApartirJusanteDesvio(Dados& a_dados) {

	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const IdHidreletrica maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

		for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			const IdHidreletrica idHidreletrica_jusante_desvio = a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica());

			if (idHidreletrica_jusante_desvio != IdHidreletrica_Nenhum) {

				const SmartEnupla<Periodo, double> vetor_zero(horizonte_estudo, 0.0);
				const SmartEnupla<Periodo, double> desvio_agua_maximo(horizonte_estudo, 99999);//Valor não informado nos decks

				const IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico(a_dados.getMaiorId(IdIntercambioHidraulico()) + 1);

				IntercambioHidraulico intercambioHidraulico;
				intercambioHidraulico.setAtributo(AttComumIntercambioHidraulico_idIntercambioHidraulico, idIntercambioHidraulico);

				a_dados.vetorIntercambioHidraulico.add(intercambioHidraulico);

				a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setAtributo(AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico_desvio);

				a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setAtributo(AttComumIntercambioHidraulico_hidreletrica_origem, idHidreletrica);
				a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setAtributo(AttComumIntercambioHidraulico_hidreletrica_destino, idHidreletrica_jusante_desvio);

				a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setVetor(AttVetorIntercambioHidraulico_desvio_agua_minimo, vetor_zero);
				a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setVetor(AttVetorIntercambioHidraulico_desvio_agua_maximo, desvio_agua_maximo);

			}//if (idHidreletricajusante_desvio != IdHidreletrica_Nenhum) {

		}//for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::adicionaIntercambiosHidraulicosApartirJusanteDesvio(Dados &a_dados): \n" + std::string(erro.what())); }

} // void LeituraCEPEL::adicionaIntercambiosHidraulicosApartirJusanteDesvio() {


void LeituraCEPEL::adicionaLimitesDesvioApartirJusanteDesvio(Dados& a_dados) {

	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const IdHidreletrica maiorIdHidreletrica = a_dados.getMaiorId(IdHidreletrica());

		for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

			const IdHidreletrica idHidreletrica_jusante_desvio = a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica());

			if (idHidreletrica_jusante_desvio != IdHidreletrica_Nenhum) {

				a_dados.vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_vazao_desviada_minima, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
				a_dados.vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_vazao_desviada_maxima, SmartEnupla<Periodo, double>(horizonte_estudo, 99999));//Valor não informado nos decks

			}//if (idHidreletricajusante_desvio != IdHidreletrica_Nenhum) {

		}//for (IdHidreletrica idHidreletrica = a_dados.getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; a_dados.vetorHidreletrica.incr(idHidreletrica)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::adicionaLimitesDesvioApartirJusanteDesvio(Dados &a_dados): \n" + std::string(erro.what())); }

} // void LeituraCEPEL::adicionaLimitesDesvioApartirJusanteDesvio() {


void LeituraCEPEL::set_zero_vazao_defluente_minima_historica_usina_fio_sem_reservatorio_a_montante(Dados& a_dados, const IdHidreletrica a_idHidreletrica) {
	try {

		//Considera defluência mínima nula para as usinas a fio d`água até o primeiro reservatório da cascata

		const Periodo periodo_inicial = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio()).getIteradorInicial();
		const Periodo periodo_final = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio()).getIteradorFinal();

		for (Periodo periodo = periodo_inicial; periodo <= periodo_final; a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio()).incrementarIterador(periodo)) {

			if (a_dados.vetorHidreletrica.att(a_idHidreletrica).getElementoVetor(AttVetorHidreletrica_regularizacao, periodo, int()) != 1) {

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
					if (a_dados.vetorHidreletrica.att(idHidreletricas_montante.at(0)).getElementoVetor(AttVetorHidreletrica_regularizacao, periodo, int()) == 1) {
						usina_sem_regularizacao_sem_reservatorio_montante = false;
						break;
					}//if (vetorHidreletrica.att(idHidreletricas_montante.at(0)).getElementoVetor(AttVetorHidreletrica_regularizacao, periodo, bool())) {

					//Inclui usinas a montante da usina idHidreletricas_montante.at(0) 

					numMontante = a_dados.getSizeVetor(idHidreletricas_montante.at(0), AttVetorHidreletrica_montante);

					for (int m = 1; m <= numMontante; m++)
						idHidreletricas_montante.push_back(a_dados.getElementoVetor(idHidreletricas_montante.at(0), AttVetorHidreletrica_montante, m, IdHidreletrica()));

					//Elimina do vetor a usina já validada
					idHidreletricas_montante.erase(idHidreletricas_montante.begin());

				}//while (true) {

				//Atualiza vazao_defluente_minima com valor = 0 das usinas fio d'àgua sem usina com regularizaçao a montante
				if (usina_sem_regularizacao_sem_reservatorio_montante)
					a_dados.vetorHidreletrica.att(a_idHidreletrica).setAtributo(AttComumHidreletrica_vazao_defluente_minima_historica, 0.0);

			}//if (!vetorHidreletrica.att(a_idHidreletrica).getElementoVetor(AttVetorHidreletrica_regularizacao, periodo, bool())) {

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