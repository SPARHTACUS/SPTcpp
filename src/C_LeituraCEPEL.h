#ifndef LEITURA_CEPEL
#define LEITURA_CEPEL

#include "C_Dados.h"


class LeituraCEPEL {

public:

	LeituraCEPEL();
	virtual ~LeituraCEPEL();

	void leitura_CEPEL(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const std::string a_deck, const std::string a_nick);

	void leitura_DESSEM(Dados& a_dados, const std::string nomeArquivo, const std::string nomePasta);
	void leitura_DECOMP(Dados& a_dados, const std::string a_diretorio);
	void leitura_NEWAVE(Dados& a_dados, const std::string a_diretorio, const std::string nomeArquivo);

	void inicializa_arquivo_avisos();

private:

	std::string deck_str;
	std::string nick_str;

	std::string SPT_avisos;

	IdBaciaHidrografica atribui_bacia_hidrografica(Dados& a_dados, const int codigo_ons_uhe);

	// CARREGA AttComum HIDRELETRICAS E INSTANCIA AS MESMAS
	void instancia_hidreletricas_preConfig(Dados& a_dados, const std::string a_diretorio);

	void instancia_termeletricas_preConfig(Dados& a_dados, const std::string a_diretorio);

	void instancia_horizonte_preConfig(Dados& a_dados, const std::string a_diretorio);

	void instancia_dados_preConfig(Dados& a_dados, const std::string a_diretorio);

	void sequenciarRestricoesEletricas(Dados& a_dados);
	void sequenciarRestricoesHidraulicas(Dados& a_dados);

	/////////////////////////////////////////
	//DESSEM
	/////////////////////////////////////////

	//CALCULO DO PERIODO INICIAL DA RESTRIÇÃO
	Periodo getPeriodoInicial(Dados& a_dados, std::string diaInicial, std::string horaInicial, std::string meiaHoraInicial);

	//GET PERIODO FINAL DA RESTRIÇÃO
	Periodo getPeriodoFinalResticao(Periodo a_periodoFinalRestricao, SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo);

	//GET PERIODO INICIAL DA RESTRIÇÃO
	Periodo getPeriodoInicialResticao(Periodo a_periodoInicialRestricao, SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo);

	//CALCULO DO PERIODO FINAL DA RESTRIÇÃO
	Periodo getPeriodoFinal(Dados& a_dados, std::string diaFinal, std::string horaFinal, std::string meiaHoraFinal);

	// VAZOES NATURAIS  
	void leitura_VAZOES_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo);

	//INSTANCIA PROCESSO ESTOCASTICO
	void InstanciaProcessoEstocastico(Dados& a_dados, const SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo);

	// DADGER    DADOS GERAIS DO PROBLEMA  
	void leitura_DADGER_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo);
	void leitura_DADGER_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo, const std::string a_registro, const SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo);

	// CADTERM   CADASTRO DAS USINAS TERMICAS 
	void leitura_CADTERM_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo);
	void leitura_CADTERM_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo, const std::string a_registro, const SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo);

	// OPERUT    OPERACAO DAS UNIDADES TERMICAS
	void leitura_OPERUT_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo);
	void leitura_OPERUT_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo, const std::string a_registro, const SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo);

	// RAMPAS    Arquivo de trajetorias 
	void leitura_RAMPAS_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo);

	// CADASTRO DAS USINAS HIDRELETRICAS  
	void leitura_CADUSIH_201904_NW25_DC29_DES16(Dados& a_dados, const std::string a_nomeArquivo, const bool a_hidreletricasPreConfig_instanciadas, const bool a_readPoliJus);

	//RESTRICOES DE OPERACAO HIDRAULICA
	void leitura_OPERUH_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo);
	void leitura_OPERUH_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo, const std::string a_registro, const SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo);

	//DEFLUENCIAS ANTERIORES   
	void leitura_DEFLANT_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo);

	//CANAL PEREIRA BARRETO   
	void leitura_ILSTRI_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo);

	//COTAS NA R11 ANTERIORES   
	void leitura_COTASR11_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo);

	//CADASTRO DE RESERVA DE POTENCIA 
	void leitura_AREACONT_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo);

	//ESTUDO DE  RESERVA DE POTENCIA
	void leitura_RESPOT_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo);

	//PONTO DE OPERAÇÃO DE USINAS GNL
	void leitura_PTOPER_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo);

	//RESERVATORIO EQUIVALENTES DE ENERGIA  
	void leitura_REE_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo);
	void leitura_REE_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo, const std::string a_registro, const SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo);

	//RENOVAVEIS  
	void leitura_EOLICA_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo);

	void validarGeracaoMediaRenovaveis(Dados& a_dados);

	//RENOVAVEIS  
	void calculaRegularizacaoUHE(Dados& a_dados);

	/////////////////////////////////////////
	//DECOMP
	/////////////////////////////////////////

	// CASO
	std::string leitura_CASO_201906_DC29(std::string nomeArquivo);

	//VAZOES
	void ler_vazao_probabilidade_estrutura_arvore_cenarios_from_VAZOES_201906_DC29(Dados& a_dados, std::string nomeArquivo);
	void definir_horizonte_otimizacao_DC_from_VAZOES_201906_DC29(Dados& a_dados, std::string nomeArquivo);

	void ler_afluencia_passada_from_VAZOES_201906_DC29  (Dados& a_dados, std::string nomeArquivo);
	void ler_historico_afluencia_from_VAZOES_201906_DC29(Dados& a_dados, std::string nomeArquivo);

	void imprime_VAZOES_201906_DC29(Dados& a_dados, std::string nomeArquivo);
	void imprime_VAZOES_DAT();
	void instanciar_hidreletricas_from_VAZOES_201906_DC29(Dados& a_dados, std::string nomeArquivo);
	void instanciar_membros_das_hidreletricas_instanciadas(Dados& a_dados);
	void instanciar_processo_estocastico_CP(Dados& a_dados);

	void instanciar_mapeamento_arvore_cenarios_simetrica(Dados& a_dados);
	void instanciar_no_antecessor_arvore_simetrica(Dados& a_dados);
	void instanciar_no_realizacao_arvore_simetrica(Dados& a_dados);
	void instanciar_no_probabilidade_arvore_simetrica(Dados& a_dados);
	void instanciar_variavelAleatoria_realizacao_from_vazoes_DC(Dados& a_dados);

	bool criar_tendencia_temporal_com_vazoes_observadas_CP(const std::string a_diretorio, const std::string a_revisao);

	void leitura_TENDENCIA_VAZOES_MENSAIS_GEVAZP(Dados& a_dados, const std::string nomeArquivo);
	void leitura_TENDENCIA_VAZOES_SEMANAIS_GEVAZP(Dados& a_dados, const std::string nomeArquivo, const std::string a_revisao);

	// DADGER    DADOS GERAIS DO PROBLEMA  
	void leitura_DADGER_201906_DC29(Dados& a_dados, std::string nomeArquivo);

	// POLIJUS   FAMILIA DE POLINOMIOS DE COTA JUSANTE  
	void leitura_POLIJUS(Dados& a_dados, std::string nomeArquivo);

	// SET PERCENTUAL DEFLAUT  DELTA DE VOLUME CALCULO FPH  
	void leitura_setPercentualVolumeCalculoFPH(Dados& a_dados);

	// GNL    DADOS DAS USINAS TÉRMICAS GNL 
	void leitura_DADGNL_201906_DC29_A(Dados& a_dados, std::string nomeArquivo, std::string nomeArquivo_pastaRaiz_relgnl, std::string nomeArquivo_pastaAdicionais_relgnl);
	void leitura_DADGNL_201906_DC29_B(Dados& a_dados, const std::string a_diretorio, const std::string nomeArquivo);

	void leitura_PATAMAR_201908_NW25_em_DADGNL(const std::string a_diretorio, const std::string a_nomeArquivo, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>& a_horizonte_comandado);

	// PERDAS    fatores de perda para centro de gravidade da carga  
	void leitura_PERDAS_201906_DC29(Dados& a_dados, std::string nomeArquivo);

	// MLT    médias mensais de longo termo   
	void leitura_MLT_201906_DC29(Dados& a_dados, std::string nomeArquivo);

	void atualiza_volume_util_maximo_com_percentual_volume_util_maximo(Dados& a_dados);

	// Info adicional da saida do DC   
	void leitura_volumes_meta_from_relatos_DC(Dados& a_dados, std::string a_nomeArquivo_1, std::string a_nomeArquivo_2);
	void leitura_volumes_meta_from_dec_oper_usih_DC(Dados& a_dados, std::string a_nomeArquivo, const bool a_somente_volume_meta_no_ultimo_estagio);
	bool leitura_turbinamento_maximo_from_relato_e_avl_turb_max_DC(Dados& a_dados, std::string a_nomeArquivo_1, std::string a_nomeArquivo_2);
	bool leitura_turbinamento_maximo_from_avl_turb_max_DC(Dados& a_dados, std::string a_nomeArquivo_1);
	void leitura_coeficientes_evaporacao_from_dec_cortes_evap_DC(Dados& a_dados, std::string a_nomeArquivo);
	void leitura_fph_from_avl_cortesfpha_dec_DC(Dados& a_dados, std::string a_nomeArquivo);
	void leitura_range_volume_from_eco_fpha_DC(Dados& a_dados, std::string a_nomeArquivo);
	void leitura_vazao_evaporada_meta_from_dec_oper_usih_DC(Dados& a_dados, std::string a_nomeArquivo);
	void set_hidreletrica_vazao_turbinada_disponivel_meta(Dados& a_dados);
	void set_hidreletrica_potencia_disponivel_meta_from_dec_oper_usih_DC(Dados& a_dados, std::string a_nomeArquivo);
	void set_termeletrica_potencia_disponivel_meta(Dados& a_dados);
	void leitura_potencia_comandada_from_relgnl(Dados& a_dados, const std::string a_nomeArquivo_pastaRaiz_relgnl, const std::string a_nomeArquivo_pastaAdicionais_relgnl, const std::vector<int> a_codigo_gnl, const std::vector<std::string> a_nome_gnl, SmartEnupla<IdTermeletrica, SmartEnupla<Periodo, double>>& a_lista_termeletrica_potencia_pre_comandada);
	void leitura_vRef_from_CadUsH_csv(Dados& a_dados, std::string a_nomeArquivo);

	//Validações
	void validacoes_DC(Dados& a_dados, const std::string a_diretorio, const std::string a_revisao);

	//Define horizontes do curto prazo
	void defineHorizontes_CP(Dados& a_dados);
	void define_horizonte_otimizacao_CP(Dados& a_dados);
	void define_horizonte_estudo_CP(Dados& a_dados);

	//Define mapeamento_espaco_amostral
	SmartEnupla<IdCenario, SmartEnupla<Periodo, IdRealizacao>> define_mapeamento_espaco_amostral_arvore_simetrica_CP(Dados& a_dados, const IdCenario a_cenario_inicial, const IdCenario a_cenario_final);
	void define_realizacao_transformada_espaco_amostral_arvore_completa_CP(Dados& a_dados);
	void define_variavel_aleatoria_interna_CP(Dados& a_dados, const IdCenario a_cenario_inicial, const IdCenario a_cenario_final);
	void define_numero_cenarios_CP(Dados& a_dados);
	void define_afluencia_arvore_de_cenarios_postos_CP(Dados& a_dados);

	void modifica_lista_jusante_hidreletrica_com_casos_validados_CP(Dados& a_dados);
	void inicializa_vazao_defluente_CP(Dados& a_dados);

	void valida_bacia_sao_francisco(Dados& a_dados);

	void imprime_na_tela_avisos_de_possiveis_inviabilidades_fph(Dados& a_dados);

	/////////////////////////////////////////
	//NEWAVE
	/////////////////////////////////////////

	void leitura_DGER_201908_NW25(Dados& a_dados, std::string nomeArquivo);

	void leitura_SISTEMA_201908_NW25(Dados& a_dados, std::string nomeArquivo);

	void leitura_CONFHD_201908_NW25(Dados& a_dados, std::string nomeArquivo);

	void leitura_MODIF_201908_NW25(Dados& a_dados, std::string nomeArquivo);

	void leitura_CONFT_201908_NW25(Dados& a_dados, std::string nomeArquivo);

	void leitura_TERM_201908_NW25(Dados& a_dados, std::string nomeArquivo);

	void leitura_CLAST_201908_NW25(Dados& a_dados, std::string nomeArquivo);

	void leitura_EXPH_201908_NW25(Dados& a_dados, std::string nomeArquivo);

	void leitura_EXPT_201908_NW25(Dados& a_dados, std::string nomeArquivo);

	void leitura_PATAMAR_201908_NW25(Dados& a_dados, std::string nomeArquivo);

	void leitura_MANUTT_201908_NW25(Dados& a_dados, std::string nomeArquivo);

	void leitura_VAZPAST_201908_NW25(Dados& a_dados, std::string nomeArquivo);

	void leitura_C_ADIC_201908_NW25(Dados& a_dados, std::string nomeArquivo);

	void leitura_GTMINPAT_201908_NW25(Dados& a_dados, std::string nomeArquivo);

	void leitura_LOSS_201908_NW25(Dados& a_dados, std::string nomeArquivo);

	void leitura_DSVAGUA_201908_NW25(Dados& a_dados, std::string nomeArquivo);

	void leitura_PENALID_201908_NW25(Dados& a_dados, std::string nomeArquivo);

	void leitura_AGRINT_201908_NW25(Dados& a_dados, std::string nomeArquivo);

	void leitura_ADTERM_201908_NW25(Dados& a_dados, std::string nomeArquivo);

	void leitura_GHMIN_201908_NW25(Dados& a_dados, std::string nomeArquivo);

	void leitura_CVAR_201908_NW25(Dados& a_dados, std::string nomeArquivo);

	void leitura_RE_201908_NW25(Dados& a_dados, std::string nomeArquivo);

	void leitura_VAZOES_201908_NW25(Dados& a_dados, std::string nomeArquivo);

	void leitura_CURVA_202001_NW27(Dados& a_dados, std::string nomeArquivo);

	void validacoes_NW(Dados& a_dados);


	void validacoes_DESSEM(Dados& a_dados, const std::string a_diretorio);


	void calculaEngolimentoMaximo(Dados& a_dados, const SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo, const Periodo a_periodo_final, const bool a_lido_turbinamento_maximo_from_relato_e_avl_turb_max_DC);

	double calculaDefluenciaMinima_para_EngolimentoMaximo(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const Periodo a_periodo, const IdEstagio idEstagio, const SmartEnupla<Periodo, bool> a_horizonte_processo_estocastico);

	double calculaRaizPolinomioJusante(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const Periodo a_periodo);

	void set_zero_vazao_defluente_minima_historica_usina_fio_sem_reservatorio_a_montante(Dados& a_dados, const IdHidreletrica a_idHidreletrica);

	////////////////////////////////////////////////////////////////
	//Definição de Submercados e Intercambios 
	//não declarados nos modelos oficiais (IV, ANDE)
	////////////////////////////////////////////////////////////////

	const int codigo_IV = 1111; //Código dado na Norus ao submercado IVAPORÃ
	const int codigo_ANDE = 2222; //Código dado na Norus ao submercado ANDE 
	const int codigo_submercado_ITAIPU = 3333; //Código dado na Norus ao submercado ANDE 

	void inicializa_Submercados_Intercambios_Nao_Registrados(Dados& a_dados);

	////////////////////////////////////////////////////////////////
	//PreConfig Hidrelétricas

	SmartEnupla<Periodo, bool> horizonte_estudo_DECK;

	const bool desconsiderar_atraso_periodo_estudo_inicial = true;

	bool dadosPreConfig_instanciados = false;
	bool hidreletricasPreConfig_instanciadas = false;
	bool termeletricasPreConfig_instanciadas = false;
	bool tendenciaPreConfig_instanciadas = false;

	bool itaipu_tipo_detalhamento_producao_por_conjunto = false;

	int numero_anos_manutencao_termica_programada = 0;

	////////////////////////////////////////////////////////////////

	SmartEnupla<IdHidreletrica, int> lista_codigo_ONS_REE;
	SmartEnupla<IdBaciaHidrografica, int> lista_codigo_ONS_bacia_REE;

	SmartEnupla<IdHidreletrica, int> lista_codigo_ONS_hidreletrica;

	SmartEnupla<IdHidreletrica, bool> lista_hidreletrica_pre_configuracao;

	SmartEnupla<IdHidreletrica, bool> lista_hidreletrica_com_expansao;
	SmartEnupla<IdHidreletrica, bool> lista_hidreletrica_com_modificacao;
	SmartEnupla<IdHidreletrica, bool> lista_hidreletrica_sem_capacidade;

	SmartEnupla<IdHidreletrica, IdConjuntoHidraulico> lista_hidreletrica_maiorIdConjuntoHidraulico;

	SmartEnupla<IdTermeletrica, int> lista_codigo_ONS_termeletrica;
	SmartEnupla<IdTermeletrica, int> lista_classe_ONS_termeletrica;

	SmartEnupla<IdTermeletrica, bool> lista_termeletrica_com_expansao;

	SmartEnupla<IdContrato, int> lista_codigo_ONS_contrato;

	SmartEnupla<IdUsinaEolica, int> lista_codigo_ONS_usina_eolica;

	SmartEnupla<IdSubmercado, SmartEnupla<IdPatamarDeficit, int>> matriz_codigo_ONS_patamar_deficit;

	SmartEnupla<IdRestricaoEletrica, int> lista_codigo_ONS_restricao_eletrica;

	SmartEnupla<Periodo, double> lista_demanda_ANDE;

	SmartEnupla<IdRestricaoEletrica, Periodo> lista_restricao_eletrica_periodo_inicial;
	SmartEnupla<IdRestricaoEletrica, Periodo> lista_restricao_eletrica_periodo_final;

	SmartEnupla<IdAgrupamentoIntercambio, int> lista_codigo_ONS_agrupamento_intercambio;

	SmartEnupla<IdRestricaoOperativaUHE, int> lista_codigo_ONS_restricao_operativa_UHE_vazao_afluente;
	SmartEnupla<IdRestricaoOperativaUHE, int> lista_codigo_ONS_restricao_operativa_UHE_volume;
	SmartEnupla<IdRestricaoOperativaUHE, int> lista_codigo_ONS_restricao_operativa_UHE_vazao_defluente;

	SmartEnupla<IdUsinaElevatoria, int> lista_codigo_ONS_usina_elevatoria;

	SmartEnupla<IdSubmercado, int> lista_codigo_ONS_submercado;

	SmartEnupla<IdHidreletrica, IdSubmercado> lista_IdSubmercado_hidreletrica;

	SmartEnupla<IdSubmercado, std::string> lista_submercado_mnemonico;

	SmartEnupla<int, int> lista_codigo_ONS_hidreletrica_original;
	SmartEnupla<int, int> lista_codigo_ONS_hidreletrica_jusante_original;

	SmartEnupla<int, int> lista_codigo_ONS_hidreletrica_agregada; //Código das usinas que somente existem no NW representando um equivalente de várias outras usinas
	SmartEnupla<int, SmartEnupla<int, int>> lista_codigo_ONS_hidreletricas_desagregadas; //Código das usinas desagregada correspondete à hidreletrica_agregada

	//Listas necessárias para validação das hidrelétricas PreConfig
	SmartEnupla<IdHidreletrica, IdHidreletrica> lista_jusante_hidreletrica;        //Lista da jusante de cada hidrelétrica -> Somente para validação entre a configuração PD e CP
	SmartEnupla<IdHidreletrica, IdHidreletrica> lista_jusante_desvio_hidreletrica; //Lista da jusante_desvio de cada hidrelétrica -> Somente para validação entre a configuração PD e CP

	SmartEnupla<IdHidreletrica, int> lista_hidreletrica_NPOSNW; //Lista que registra se uma usina tem diferença de posto entre DECOMP  e NEWAVE

	template<typename TipoIterador>
	TipoIterador getIdFromCodigoONS(const SmartEnupla<TipoIterador, int>& a_lista_codigo_ONS, const int a_codigo_ONS) {
		try {

			if (a_lista_codigo_ONS.size() == 0)
				return TipoIterador(0);

			const TipoIterador menorIdIterador = a_lista_codigo_ONS.getIteradorInicial();
			const TipoIterador maiorIdIterador = a_lista_codigo_ONS.getIteradorFinal();

			if (menorIdIterador < getFromChar(TipoIterador(), "min"))
				throw std::invalid_argument("O menor iterador deve ser maior ou igual a " + getString(getFromChar(TipoIterador(), "min")));
			else if (maiorIdIterador > getFromChar(TipoIterador(), "max"))
				throw std::invalid_argument("O iterador Inicial deve ser menor ou igual a " + getString(getFromChar(TipoIterador(), "max")));

			TipoIterador iterador_retorno = TipoIterador(0);
			bool isCodigoEncontrado = false;
			for (TipoIterador iterador = menorIdIterador; iterador <= maiorIdIterador; iterador++) {
				if ((a_lista_codigo_ONS.at(iterador) == a_codigo_ONS) && (!isCodigoEncontrado)) {
					isCodigoEncontrado = true;
					iterador_retorno = iterador;
				}
				else if ((a_lista_codigo_ONS.at(iterador) == a_codigo_ONS) && (isCodigoEncontrado))
					throw std::invalid_argument("Multiplos elementos com o mesmo codigo CEPEL.");
			}

			return iterador_retorno;

		}//try {
		catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::getIdFromCodigoONS(a_lista_codigo_ONS, " + getFullString(a_codigo_ONS) + "): \n" + std::string(erro.what())); }
	};

	template<typename TipoIterador>
	std::vector<TipoIterador> getIdsFromCodigoONS(const SmartEnupla<TipoIterador, int>& a_lista_codigo_ONS, const int a_codigo_ONS) {
		try {

			std::vector<TipoIterador> vetor_retorno;

			//if (a_lista_codigo_ONS.size() == 0)
				//return vetor_retorno.push_back(TipoIterador(0));

			const TipoIterador menorIdIterador = a_lista_codigo_ONS.getIteradorInicial();
			const TipoIterador maiorIdIterador = a_lista_codigo_ONS.getIteradorFinal();

			if (menorIdIterador < getFromChar(TipoIterador(), "min"))
				throw std::invalid_argument("O menor iterador deve ser maior ou igual a " + getString(getFromChar(TipoIterador(), "min")));
			else if (maiorIdIterador > getFromChar(TipoIterador(), "max"))
				throw std::invalid_argument("O iterador Inicial deve ser menor ou igual a " + getString(getFromChar(TipoIterador(), "max")));

			for (TipoIterador iterador = menorIdIterador; iterador <= maiorIdIterador; iterador++) {
				if ((a_lista_codigo_ONS.at(iterador) == a_codigo_ONS))
					vetor_retorno.push_back(iterador);
			}//for (TipoIterador iterador = menorIdIterador; iterador <= maiorIdIterador; iterador++) {

			return vetor_retorno;

		}//try {
		catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::getIdFromCodigoONS(a_lista_codigo_ONS, " + getFullString(a_codigo_ONS) + "): \n" + std::string(erro.what())); }
	};

	template<typename TipoIterador_1, typename TipoIterador_2>
	TipoIterador_2 getIdFromCodigoONS(const SmartEnupla<TipoIterador_1, SmartEnupla<TipoIterador_2, int>>& a_matriz_codigo_ONS, const TipoIterador_1 a_iterador_1, const int a_codigo_ONS) {
		try {

			if (a_matriz_codigo_ONS.size() == 0)
				return TipoIterador_2(0);

			const TipoIterador_2 menorIdIterador_2 = a_matriz_codigo_ONS.at(a_iterador_1).getIteradorInicial();
			const TipoIterador_2 maiorIdIterador_2 = a_matriz_codigo_ONS.at(a_iterador_1).getIteradorFinal();

			if (menorIdIterador_2 < getFromChar(TipoIterador_2(), "min"))
				throw std::invalid_argument("O menor iterador deve ser maior ou igual a " + getString(getFromChar(TipoIterador_2(), "min")));
			else if (maiorIdIterador_2 > getFromChar(TipoIterador_2(), "max"))
				throw std::invalid_argument("O iterador Inicial deve ser menor ou igual a " + getString(getFromChar(TipoIterador_2(), "max")));

			TipoIterador_2 iterador_retorno = TipoIterador_2(0);
			bool isCodigoEncontrado = false;
			for (TipoIterador_2 iterador = menorIdIterador_2; iterador <= maiorIdIterador_2; iterador++) {
				if ((a_matriz_codigo_ONS.at(a_iterador_1).at(iterador) == a_codigo_ONS) && (!isCodigoEncontrado)) {
					isCodigoEncontrado = true;
					iterador_retorno = iterador;
				}
				else if ((a_matriz_codigo_ONS.at(a_iterador_1).at(iterador) == a_codigo_ONS) && (isCodigoEncontrado))
					throw std::invalid_argument("Multiplos elementos com o mesmo codigo CEPEL.");
			}

			return iterador_retorno;

		}//try {
		catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::getIdFromCodigoONS(a_matriz_codigo_ONS, " + getFullString(a_codigo_ONS) + "): \n" + std::string(erro.what())); }
	};

	IdSubmercado getIdSubmercadoFromMnemonico(const std::string a_menemonico) {
		try {

			for (IdSubmercado idSubmercado = IdSubmercado_1; idSubmercado <= lista_submercado_mnemonico.getIteradorFinal(); idSubmercado++) {
				if (strCompara(a_menemonico, lista_submercado_mnemonico.getElemento(idSubmercado)))
					return idSubmercado;
			}

			return IdSubmercado_Nenhum;

		}//try {
		catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::getIdSubmercadoFromMnemonico(" + a_menemonico + "): \n" + std::string(erro.what())); }
	} // IdSubmercado getIdSubmercadoFromMnemonico(const std::string a_mnmonico) {

	////////////////////////////////////////////////////////////////////////////////////
	//No DECOMP, existe um campo que indica o estágio a partir do qual a hidrelétrica 
	//entra em operação e pode ser maior ao primeiro estágio. Na lógica do SPT, a usina 
	//vai ser considerada com potência maxima, turbinamento e número de máquinas zeradas. 
	//E logo é criada uma modificação a partir do periodo indicado
	////////////////////////////////////////////////////////////////////////////////////

	SmartEnupla<int, SmartEnupla<IdHidreletrica, Periodo>> matriz_hidreletrica_periodo_entrada_em_operacao;

	int getIteradorHidreletricaPeriodoEntradaOperacao(const SmartEnupla<int, SmartEnupla<IdHidreletrica, Periodo>>& a_matriz_hidreletrica_periodo_entrada_em_operacao, const IdHidreletrica a_idHidreletrica_teste) {
		try {

			int iterador_retornar = -1;

			if (matriz_hidreletrica_periodo_entrada_em_operacao.size() == 0)
				return iterador_retornar;

			const int menorIdIterador = a_matriz_hidreletrica_periodo_entrada_em_operacao.getIteradorInicial();
			const int maiorIdIterador = a_matriz_hidreletrica_periodo_entrada_em_operacao.getIteradorFinal();

			bool isIdHidreletricaEncontrado = false;
			for (int iterador = menorIdIterador; iterador <= maiorIdIterador; iterador++) {
				if ((a_matriz_hidreletrica_periodo_entrada_em_operacao.at(iterador).getIteradorInicial() == a_idHidreletrica_teste) && (!isIdHidreletricaEncontrado)) {
					isIdHidreletricaEncontrado = true;
					iterador_retornar = iterador;
				}
				else if ((a_matriz_hidreletrica_periodo_entrada_em_operacao.at(iterador).getIteradorInicial() == a_idHidreletrica_teste) && (isIdHidreletricaEncontrado))
					throw std::invalid_argument("Multiplos elementos com o mesmo IdHidreletrica.");
			}

			return iterador_retornar;

		}//try {
		catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::getIteradorHidreletricaPeriodoEntradaOperacao(a_matriz_hidreletrica_periodo_entrada_em_operacao, " + getFullString(a_idHidreletrica_teste) + "): \n" + std::string(erro.what())); }
	};

	Periodo getPeriodoHidreletricaPeriodoEntradaOperacao(const SmartEnupla<int, SmartEnupla<IdHidreletrica, Periodo>>& a_matriz_hidreletrica_periodo_entrada_em_operacao, const int a_iterador) {
		try {

			if (matriz_hidreletrica_periodo_entrada_em_operacao.size() == 0)
				throw std::invalid_argument("matriz_hidreletrica_periodo_entrada_em_operacao vazia");

			if (a_iterador < 1)
				throw std::invalid_argument("Iterador nao valido");

			const Periodo periodo = a_matriz_hidreletrica_periodo_entrada_em_operacao.at(a_iterador).at(a_matriz_hidreletrica_periodo_entrada_em_operacao.at(a_iterador).getIteradorInicial());

			return periodo;

		}//try {
		catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::getPeriodoHidreletricaPeriodoEntradaOperacao(a_matriz_hidreletrica_periodo_entrada_em_operacao, " + getFullString(a_iterador) + "): \n" + std::string(erro.what())); }
	};

	////////////////////////////////////////////////////////////////////////////////////

	// ------------------------
	//
	// Modificacao Termeletrica
	//
	// ------------------------

	enum TipoModificacaoUTE {
		TipoModificacaoUTE_Nenhum,
		TipoModificacaoUTE_TEIFT,
		TipoModificacaoUTE_IPTER,
		TipoModificacaoUTE_GTMIN,
		TipoModificacaoUTE_POTEF,
		TipoModificacaoUTE_FCMAX,
		TipoModificacaoUTE_Excedente,
	};

	struct ModificacaoUTE {
		Periodo periodo_inicial;
		Periodo periodo_final;
		TipoModificacaoUTE tipo_de_modificacao = TipoModificacaoUTE_Nenhum;
		double valor = NAN;
	};

	std::vector<std::vector<ModificacaoUTE>> lista_modificacaoUTE;

	// Método para aplicar modificações nas Hidreletricas.
	void aplicarModificacoesUTE(Dados& a_dados);

	// ------------------------
	//
	// Modificacao Hidreletrica
	//
	// ------------------------

	enum TipoModificacaoUHE {
		TipoModificacaoUHE_Nenhum,
		TipoModificacaoUHE_IP,
		TipoModificacaoUHE_TEIF,
		TipoModificacaoUHE_VAZMIN,
		TipoModificacaoUHE_VAZMINT,
		TipoModificacaoUHE_VAZMINHISTORICA,
		TipoModificacaoUHE_VMAXT,
		TipoModificacaoUHE_VMINT,
		TipoModificacaoUHE_VMINP,
		TipoModificacaoUHE_VMORTO,
		TipoModificacaoUHE_NOVAMAQ,
		TipoModificacaoUHE_NUMCNJ,
		TipoModificacaoUHE_NUMMAQ,
		TipoModificacaoUHE_POTEFE,
		TipoModificacaoUHE_NUMBAS,
		TipoModificacaoUHE_PERDHIDR,
		TipoModificacaoUHE_COEFEVAP,
		TipoModificacaoUHE_PRODESP,
		TipoModificacaoUHE_COTAREA,
		TipoModificacaoUHE_COTARE,
		TipoModificacaoUHE_VOLCOTA,
		TipoModificacaoUHE_COTVOL,
		TipoModificacaoUHE_CMONT,
		TipoModificacaoUHE_VOLMIN,
		TipoModificacaoUHE_VOLMAX,
		TipoModificacaoUHE_CFUGA,
		TipoModificacaoUHE_NOMEUH,
		TipoModificacaoUHE_NUMPOS,
		TipoModificacaoUHE_NUMJUS,
		TipoModificacaoUHE_DESVIO,
		TipoModificacaoUHE_ALTEFE,
		TipoModificacaoUHE_VAZEFE,
		TipoModificacaoUHE_NCHAVE,
		TipoModificacaoUHE_COTVAZ,
		TipoModificacaoUHE_VERTJU,
		TipoModificacaoUHE_TIPTUR,
		TipoModificacaoUHE_TIPERH,
		TipoModificacaoUHE_VAZCCF,
		TipoModificacaoUHE_JUSENA,
		TipoModificacaoUHE_VSVERT,
		TipoModificacaoUHE_VMDESV,
		TipoModificacaoUHE_TIPUSI,
		TipoModificacaoUHE_NPOSNW,
		TipoModificacaoUHE_Excedente
	};

	struct ModificacaoUHE {
		Periodo periodo;
		TipoModificacaoUHE      tipo_de_modificacao = TipoModificacaoUHE_Nenhum;
		TipoGrandezaModificacao    tipo_de_grandeza = TipoGrandezaModificacao_Nenhum;
		std::string valor_0 = "";
		double valor_1 = NAN;
		double valor_2 = NAN;
		double valor_3 = NAN;
		double valor_4 = NAN;
		double valor_5 = NAN;
	};

	std::vector<std::vector<ModificacaoUHE>> lista_modificacaoUHE;

	// Método para aplicar modificações nas Hidreletricas.
	void aplicarModificacoesUHE(Dados& a_dados);

	// Métodos de aplicação individual de modificação;
	bool aplicarModificacaoIP(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoTEIF(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoNUMCNJ(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoNUMMAQ(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoVOLMIN(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoVOLMAX(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoPOTEFE(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoNUMBAS(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoVAZMIN(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoVAZMINHISTORICA(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoCOTAREA(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoCOTARE(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const std::vector<Periodo>& a_lista_periodo, const std::vector<std::vector<double>>& a_lista_valor, const std::vector<TipoGrandezaModificacao>& a_lista_tipo);

	bool aplicarModificacaoCMONT(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const std::vector<Periodo>& a_lista_periodo, const std::vector<std::vector<double>>& a_lista_valor, const std::vector<TipoGrandezaModificacao>& a_lista_tipo);

	bool aplicarModificacaoVOLCOTA(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoCOTVOL(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const std::vector<Periodo>& a_lista_periodo, const std::vector<std::vector<double>>& a_lista_valor, const std::vector<TipoGrandezaModificacao>& a_lista_tipo);

	bool aplicarModificacaoPRODESP(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoCOEFEVAP(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoPERDHIDR(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoCFUGA(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const std::vector<Periodo>& a_lista_periodo, const std::vector<std::vector<double>>& a_lista_valor, const std::vector<TipoGrandezaModificacao>& a_lista_tipo);

	bool aplicarModificacaoVAZMINT(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const std::vector<Periodo>& a_lista_periodo, const std::vector<std::vector<double>>& a_lista_valor, const std::vector<TipoGrandezaModificacao>& a_lista_tipo);

	bool aplicarModificacaoVMAXT(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const std::vector<Periodo>& a_lista_periodo, const std::vector<std::vector<double>>& a_lista_valor, const std::vector<TipoGrandezaModificacao>& a_lista_tipo);

	bool aplicarModificacaoVMINT(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const std::vector<Periodo>& a_lista_periodo, const std::vector<std::vector<double>>& a_lista_valor, const std::vector<TipoGrandezaModificacao>& a_lista_tipo);

	bool aplicarModificacaoVMINP(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const std::vector<Periodo>& a_lista_periodo, const std::vector<std::vector<double>>& a_lista_valor, const std::vector<TipoGrandezaModificacao>& a_lista_tipo);

	bool aplicarModificacaoVMORTO(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoNOVAMAQ(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoNOMEUH(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoNUMJUS(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoDESVIO(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoVAZEFE(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoVMDESV(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoNUMPOS(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoNPOSNW(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoNCHAVE(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoCOTVAZ(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	bool aplicarModificacaoVERTJU(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	//Cria os intercâmbios hidráulicos correspondentes à jusante desvio 

	void adicionaIntercambiosHidraulicosApartirJusanteDesvio(Dados& a_dados);

	void adicionaLimitesDesvioApartirJusanteDesvio(Dados& a_dados);

};


#endif // !defined(C_TURBINA_H)
