#ifndef LEITURA_CEPEL
#define LEITURA_CEPEL

#include "C_Dados.h"
#include "C_EstagioOtimizacao.h"


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
	void instancia_processoEstocasticoHidrologico_preConfig(Dados& a_dados, const std::string a_diretorio);

	void instancia_termeletricas_preConfig(Dados& a_dados, const std::string a_diretorio);

	void instancia_horizonte_preConfig(Dados& a_dados, const std::string a_diretorio);

	void instancia_dados_preConfig(Dados& a_dados, const std::string a_diretorio);
	
	void instancia_dados_matriz_preConfig(Dados& a_dados, const std::string a_diretorio);

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
	void leitura_volume_referencia_e_regularizacao_from_CadUsH_csv(Dados& a_dados, std::string a_nomeArquivo);
	
	void leitura_cortes_NEWAVE(Dados& a_dados, const SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo, std::string a_diretorio, std::string a_diretorio_cortes, std::string a_revisao);
	void leitura_cortes_NEWAVE_para_dimensionamento(Dados& a_dados, const SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo, SmartEnupla<IdReservatorioEquivalente, bool>& a_coeficientes_EAR, SmartEnupla<IdReservatorioEquivalente, SmartEnupla<int, bool>>& a_coeficiente_ENA, std::string a_nomeArquivo, const bool a_is_arquivo_fcfnwn, const int a_periodo_acoplamento);


	double get_cota_para_conversao_cortes_NEWAVE(const SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo, Hidreletrica& a_hidreletrica, const Periodo a_periodo, const Periodo a_periodo_inicial_horizonte_estudo, const double a_percentual_volume_util, const bool a_is_calculo_para_ENA);
	double get_produtibilidade_para_conversao_cortes_NEWAVE(Hidreletrica& a_hidreletrica, const double a_cota);
	void instanciar_codigo_usina_jusante_JUSENA(Dados& a_dados);
	void instancia_lista_hidreletrica_out_estudo_from_codigo_usina_jusante_JUSENA(Dados& a_dados, std::string a_nomeArquivo);
	void instancia_atributos_hidreletrica_out_from_CadUsH_csv(Dados& a_dados, Hidreletrica& a_hidreletrica, std::string a_nomeArquivo);
	void calcular_produtibilidade_ENA_regras_especiais(Dados& a_dados, const SmartEnupla<Periodo, bool> a_horizonte_tendencia_mais_estudo);
	void calcular_produtibilidade_ENA_por_usina_por_periodo(Dados& a_dados, const SmartEnupla<Periodo, bool> a_horizonte_tendencia_mais_estudo);
	void calcular_produtibilidade_EAR_acumulada_por_usina(Dados& a_dados);
	void calcular_ENA_x_REE_x_cenario_x_periodo(Dados& a_dados);
	void calcular_ENA_x_REE_x_cenario_x_periodo_com_equacionamento_REE(Dados& a_dados);
	void calcular_equacionamento_afluencia_natural_x_REE(Dados& a_dados, const SmartEnupla<Periodo, bool> &a_horizonte_tendencia_mais_estudo, const SmartEnupla<Periodo, bool> &a_horizonte_tendencia_mais_estudo_MENSAL, const Periodo a_periodo_inicial_horizonte_estudo);
	void calcular_equacionamento_afluencia_natural_x_hidreletrica(Dados& a_dados, const SmartEnupla<int, IdHidreletrica> &lista_codUsina, const SmartEnupla<int, IdHidreletrica>& lista_codPosto, const SmartEnupla<IdHidreletrica, IdVariavelAleatoria>& a_mapIdVar, const SmartEnupla<IdHidreletrica, IdVariavelAleatoriaInterna>& a_mapIdVarInterna, const SmartEnupla<Periodo, bool> &a_horizonte_tendencia_mais_estudo, const Periodo a_periodo_inicial_horizonte_estudo);
	void calcular_equacionamento_afluencia_natural_x_hidreletrica_out_estudo(Dados& a_dados, const SmartEnupla<int, IdHidreletrica>& lista_codUsina, const SmartEnupla<int, IdHidreletrica>& lista_codPosto, const SmartEnupla<IdHidreletrica, IdVariavelAleatoria>& a_mapIdVar, const SmartEnupla<IdHidreletrica, IdVariavelAleatoriaInterna>& a_mapIdVarInterna, const SmartEnupla<Periodo, bool> &a_horizonte_tendencia_mais_estudo, const Periodo a_periodo_inicial_horizonte_estudo);
	void retorna_equacionamento_regras_afluencia_natural_x_idHidreletrica(Dados& a_dados, const Periodo a_periodoPE, const SmartEnupla<int, IdHidreletrica>& lista_codUsina, const SmartEnupla<int, IdHidreletrica>& lista_codPosto, const SmartEnupla<IdHidreletrica, IdVariavelAleatoria> & a_mapIdVar, const SmartEnupla<IdHidreletrica, IdVariavelAleatoriaInterna>& a_mapIdVarInterna, const int a_codigo_posto_acoplamento_NW, const IdHidreletrica a_idHidreletrica, const IdCenario a_idCenario, const Periodo a_periodo_inicial, const Periodo a_periodo, std::vector<SmartEnupla<IdHidreletrica, double>>& a_coeficiente_idHidreletricas_calculo_ENA, double& a_termo_independente_calculo_ENA);
	void retorna_equacionamento_afluencia_natural_x_posto(Dados& a_dados, const SmartEnupla<int, IdHidreletrica>& a_lista_codUsina, const SmartEnupla<int, IdHidreletrica>& a_lista_codPosto, const IdHidreletrica a_idHidreletrica, const int a_codigo_posto, const double a_coeficiente, std::vector<SmartEnupla<IdHidreletrica, double>>& a_coeficiente_idHidreletricas_calculo_ENA);
	bool retorna_is_idHidreletrica_in_calculo_ENA(const int a_codigo_usina);
	IdMes get_IdMes_operativo(const Periodo a_periodo, const bool is_periodo_inicial);
	double get_afluencia_natural_posto(Dados& a_dados, const Periodo a_periodoPE, const SmartEnupla<int, IdHidreletrica>& a_lista_codUsina, const SmartEnupla<int, IdHidreletrica>& a_lista_codPosto, const SmartEnupla<IdHidreletrica, IdVariavelAleatoria>& a_mapIdVar, const SmartEnupla<IdHidreletrica, IdVariavelAleatoriaInterna>& a_mapIdVarInterna, const int a_codigo_posto, const IdCenario a_idCenario, const Periodo a_periodo);//Regras do hidrograma de Belo Monte

	Periodo get_periodo_ultimo_sobreposicao_com_horizonte_DC(Dados& a_dados);
	void atualizar_valores_periodos_horizonte_expandido(Dados& a_dados);
	void atualizar_valores_periodos_horizonte_expandido_hidreletrica(Dados& a_dados);
	void atualizar_valores_periodos_horizonte_expandido_termeletrica(Dados& a_dados);
	void atualizar_valores_periodos_horizonte_expandido_submercado(Dados& a_dados);
	void atualizar_valores_periodos_horizonte_expandido_intercambio(Dados& a_dados);
	void atualizar_valores_periodos_horizonte_expandido_usina_elevatoria(Dados& a_dados);
	void atualizar_valores_periodos_horizonte_expandido_restricao_eletrica(Dados& a_dados);
	void atualizar_valores_periodos_horizonte_expandido_restricao_operativa_UHE(Dados& a_dados);

	void atualizar_valores_periodos_horizonte_expandido_com_DadosEntradaMP_PRECONFIG(Dados& a_dados, std::string a_diretorio);
	
	void atualiza_lista_hidreletrica_NPOSNW_regras_especiais(Dados& a_dados);
	void defineHidreletricasMontanteNaCascataENA(Dados& a_dados);

	void imprime_produtibilidade_EAR_acumulada(Dados& a_dados, std::string nomeArquivo);
	void imprime_produtibilidade_EAR(Dados& a_dados, std::string nomeArquivo);
	void imprime_produtibilidade_ENA(Dados& a_dados, std::string nomeArquivo, const SmartEnupla<Periodo, bool> a_horizonte_tendencia_mais_estudo);
	void imprime_afluencia_natural_x_idHidreletrica_x_cenario_x_periodo(Dados& a_dados, std::string nomeArquivo, const SmartEnupla<Periodo, bool> a_horizonte_tendencia_mais_estudo);
	void imprime_ENA_x_REE_x_cenario_x_periodo(Dados& a_dados, std::string nomeArquivo);

	void instanciar_hidreletricas_ficticias_sem_producao(Dados& a_dados);//Para o acoplamento com os cortes NW com horizonte CP espandido é necessário o sorteio de postos fictícios para o cálculo das ENAs de acoplamento

	//Validações
	void validacoes_DC(Dados& a_dados, const std::string a_diretorio, const std::string a_revisao);

	//Define horizontes do curto prazo
	void defineHorizontes_CP(Dados& a_dados);
	void define_horizonte_otimizacao_CP(Dados& a_dados);
	void define_horizonte_estudo_CP(Dados& a_dados);
	void valida_horizonte_estudo_CP_respeito_horizonte_otimizacao_DC(Dados& a_dados);

	//Define mapeamento_espaco_amostral
	SmartEnupla<IdCenario, SmartEnupla<Periodo, IdRealizacao>> define_mapeamento_espaco_amostral_arvore_simetrica_CP(Dados& a_dados, const IdCenario a_cenario_inicial, const IdCenario a_cenario_final);
	void define_realizacao_transformada_espaco_amostral_arvore_completa_CP(Dados& a_dados);
	void define_variavel_aleatoria_interna_CP(Dados& a_dados);
	void define_numero_cenarios_CP(Dados& a_dados);
	void define_afluencia_arvore_de_cenarios_postos_CP(Dados& a_dados);

	void modifica_lista_jusante_hidreletrica_com_casos_validados_CP(Dados& a_dados);
	void inicializa_vazao_defluente_CP(Dados& a_dados);

	void valida_bacia_sao_francisco(Dados& a_dados);

	void imprime_na_tela_avisos_de_possiveis_inviabilidades_fph(Dados& a_dados);

	void atualiza_restricao_operativa_UHE_tipoRestricaoHidraulica_energia_armazenada(Dados& a_dados, std::string a_diretorio);

	IdPatamarCarga get_maiorIdPatamarCarga_periodo_from_percentual_duracao_patamar_carga(Dados& a_dados, const Periodo a_periodo);

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

	void inicializa_Submercados_Intercambios_Nao_Registrados(Dados& a_dados, const SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo);

	////////////////////////////////////////////////////////////////
	//PreConfig Hidrelétricas

	SmartEnupla<Periodo, bool> horizonte_estudo_DECK;

	const bool desconsiderar_atraso_periodo_estudo_inicial = true;

	bool dadosPreConfig_instanciados = false;
	bool hidreletricasPreConfig_instanciadas = false;
	bool termeletricasPreConfig_instanciadas = false;
	bool processoEstocasticoHidrologicoPreConfig_instanciado = false;
	bool tendenciaPreConfig_instanciadas = false;

	bool itaipu_tipo_detalhamento_producao_por_conjunto = false;

	int numero_anos_manutencao_termica_programada = 0;

	////////////////////////////////////////////////////////////////

	SmartEnupla<IdHidreletrica, int> lista_codigo_ONS_REE = SmartEnupla<IdHidreletrica, int>(IdHidreletrica(1), std::vector<int>(IdHidreletrica(int(IdHidreletrica_Excedente) - 1), -1));
	SmartEnupla<IdBaciaHidrografica, int> lista_codigo_ONS_bacia_REE = SmartEnupla<IdBaciaHidrografica, int>(IdBaciaHidrografica_1, std::vector<int>(IdBaciaHidrografica(int(IdBaciaHidrografica_Excedente) - 1), -1));

	SmartEnupla<IdHidreletrica, int> lista_codigo_ONS_hidreletrica = SmartEnupla<IdHidreletrica, int>(IdHidreletrica(1), std::vector<int>(IdHidreletrica(int(IdHidreletrica_Excedente) - 1), -1));

	SmartEnupla<IdHidreletrica, bool> lista_hidreletrica_pre_configuracao = SmartEnupla<IdHidreletrica, bool>(IdHidreletrica(1), std::vector<bool>(IdHidreletrica(int(IdHidreletrica_Excedente) - 1), false));

	SmartEnupla<IdHidreletrica, bool> lista_hidreletrica_com_expansao = SmartEnupla<IdHidreletrica, bool>(IdHidreletrica(1), std::vector<bool>(IdHidreletrica(int(IdHidreletrica_Excedente) - 1), false));
	SmartEnupla<IdHidreletrica, bool> lista_hidreletrica_com_modificacao = SmartEnupla<IdHidreletrica, bool>(IdHidreletrica(1), std::vector<bool>(IdHidreletrica(int(IdHidreletrica_Excedente) - 1), false));
	SmartEnupla<IdHidreletrica, bool> lista_hidreletrica_sem_capacidade = SmartEnupla<IdHidreletrica, bool>(IdHidreletrica(1), std::vector<bool>(IdHidreletrica(int(IdHidreletrica_Excedente) - 1), false));

	SmartEnupla<IdHidreletrica, IdVariavelAleatoria> lista_hidreletrica_IdVariavelAleatoria = SmartEnupla<IdHidreletrica, IdVariavelAleatoria>(IdHidreletrica(1), std::vector<IdVariavelAleatoria>(IdHidreletrica(int(IdHidreletrica_Excedente) - 1), IdVariavelAleatoria_Nenhum));
	SmartEnupla<IdHidreletrica, IdConjuntoHidraulico> lista_hidreletrica_maiorIdConjuntoHidraulico = SmartEnupla<IdHidreletrica, IdConjuntoHidraulico>(IdHidreletrica(1), std::vector<IdConjuntoHidraulico>(IdHidreletrica(int(IdHidreletrica_Excedente) - 1), IdConjuntoHidraulico_Nenhum));

	SmartEnupla<IdTermeletrica, int> lista_codigo_ONS_termeletrica = SmartEnupla<IdTermeletrica, int>(IdTermeletrica(1), std::vector<int>(IdTermeletrica(int(IdTermeletrica_Excedente) - 1), -1));
	SmartEnupla<IdTermeletrica, int> lista_classe_ONS_termeletrica = SmartEnupla<IdTermeletrica, int>(IdTermeletrica(1), std::vector<int>(IdTermeletrica(int(IdTermeletrica_Excedente) - 1), -1));

	SmartEnupla<IdTermeletrica, bool> lista_termeletrica_com_expansao = SmartEnupla<IdTermeletrica, bool>(IdTermeletrica(1), std::vector<bool>(IdTermeletrica(int(IdTermeletrica_Excedente) - 1), false));

	SmartEnupla<IdContrato, int> lista_codigo_ONS_contrato = SmartEnupla<IdContrato, int>(IdContrato_1, std::vector<int>(IdContrato(int(IdContrato_Excedente) - 1), -1));

	SmartEnupla<IdUsinaEolica, int> lista_codigo_ONS_usina_eolica = SmartEnupla<IdUsinaEolica, int>(IdUsinaEolica_1, std::vector<int>(IdUsinaEolica(int(IdUsinaEolica_Excedente) - 1), -1));

	SmartEnupla<IdRestricaoEletrica, int> lista_codigo_ONS_restricao_eletrica = SmartEnupla<IdRestricaoEletrica, int>(IdRestricaoEletrica_1, std::vector<int>(IdRestricaoEletrica(int(IdRestricaoEletrica_Excedente) - 1), -1));

	SmartEnupla<Periodo, double> lista_demanda_ANDE;

	SmartEnupla<IdRestricaoEletrica, Periodo> lista_restricao_eletrica_periodo_inicial = SmartEnupla<IdRestricaoEletrica, Periodo>(IdRestricaoEletrica_1, std::vector<Periodo>(IdRestricaoEletrica(int(IdRestricaoEletrica_Excedente) - 1), Periodo()));
	SmartEnupla<IdRestricaoEletrica, Periodo> lista_restricao_eletrica_periodo_final = SmartEnupla<IdRestricaoEletrica, Periodo>(IdRestricaoEletrica_1, std::vector<Periodo>(IdRestricaoEletrica(int(IdRestricaoEletrica_Excedente) - 1), Periodo()));

	SmartEnupla<IdAgrupamentoIntercambio, int> lista_codigo_ONS_agrupamento_intercambio = SmartEnupla<IdAgrupamentoIntercambio, int>(IdAgrupamentoIntercambio_1, std::vector<int>(IdAgrupamentoIntercambio(int(IdAgrupamentoIntercambio_Excedente) - 1), -1));

	SmartEnupla<IdRestricaoOperativaUHE, int> lista_codigo_ONS_restricao_operativa_UHE_vazao_afluente = SmartEnupla<IdRestricaoOperativaUHE, int>(IdRestricaoOperativaUHE_1, std::vector<int>(IdRestricaoOperativaUHE(int(IdRestricaoOperativaUHE_Excedente) - 1), -1));
	SmartEnupla<IdRestricaoOperativaUHE, int> lista_codigo_ONS_restricao_operativa_UHE_volume = SmartEnupla<IdRestricaoOperativaUHE, int>(IdRestricaoOperativaUHE_1, std::vector<int>(IdRestricaoOperativaUHE(int(IdRestricaoOperativaUHE_Excedente) - 1), -1));
	SmartEnupla<IdRestricaoOperativaUHE, int> lista_codigo_ONS_restricao_operativa_UHE_vazao_defluente = SmartEnupla<IdRestricaoOperativaUHE, int>(IdRestricaoOperativaUHE_1, std::vector<int>(IdRestricaoOperativaUHE(int(IdRestricaoOperativaUHE_Excedente) - 1), -1));
	SmartEnupla<IdRestricaoOperativaUHE, int> lista_codigo_ONS_restricao_operativa_UHE_energia_armazenada = SmartEnupla<IdRestricaoOperativaUHE, int>(IdRestricaoOperativaUHE_1, std::vector<int>(IdRestricaoOperativaUHE(int(IdRestricaoOperativaUHE_Excedente) - 1), -1));

	SmartEnupla<IdUsinaElevatoria, int> lista_codigo_ONS_usina_elevatoria = SmartEnupla<IdUsinaElevatoria, int>(IdUsinaElevatoria_1, std::vector<int>(IdUsinaElevatoria(int(IdUsinaElevatoria_Excedente) - 1), -1));

	SmartEnupla<IdSubmercado, int> lista_codigo_ONS_submercado = SmartEnupla<IdSubmercado, int>(IdSubmercado(1), std::vector<int>(IdSubmercado(int(IdSubmercado_Excedente) - 1), -1));

	SmartEnupla<IdSubmercado, SmartEnupla<IdPatamarDeficit, int>> matriz_codigo_ONS_patamar_deficit = SmartEnupla<IdSubmercado, SmartEnupla<IdPatamarDeficit, int>>(IdSubmercado(1), std::vector<SmartEnupla<IdPatamarDeficit, int>>(IdSubmercado(int(IdSubmercado_Excedente) - 1), SmartEnupla<IdPatamarDeficit, int>()));

	SmartEnupla<IdHidreletrica, IdSubmercado> lista_IdSubmercado_hidreletrica = SmartEnupla<IdHidreletrica, IdSubmercado>(IdHidreletrica(1), std::vector<IdSubmercado>(IdHidreletrica(int(IdHidreletrica_Excedente) - 1), IdSubmercado_Nenhum));

	SmartEnupla<IdSubmercado, std::string> lista_submercado_mnemonico = SmartEnupla<IdSubmercado, std::string>(IdSubmercado(1), std::vector<std::string>(IdSubmercado(int(IdSubmercado_Excedente) - 1), ""));

	SmartEnupla<int, int> lista_codigo_ONS_hidreletrica_original;
	SmartEnupla<int, int> lista_codigo_ONS_hidreletrica_jusante_original;

	SmartEnupla<int, int> lista_codigo_ONS_hidreletrica_agregada; //Código das usinas que somente existem no NW representando um equivalente de várias outras usinas
	SmartEnupla<int, SmartEnupla<int, int>> lista_codigo_ONS_hidreletricas_desagregadas; //Código das usinas desagregada correspondete à hidreletrica_agregada

	//Listas necessárias para validação das hidrelétricas PreConfig
	SmartEnupla<IdHidreletrica, IdHidreletrica> lista_jusante_hidreletrica = SmartEnupla<IdHidreletrica, IdHidreletrica>(IdHidreletrica(1), std::vector<IdHidreletrica>(IdHidreletrica(int(IdHidreletrica_Excedente) - 1), IdHidreletrica_Nenhum));        //Lista da jusante de cada hidrelétrica -> Somente para validação entre a configuração PD e CP
	SmartEnupla<IdHidreletrica, IdHidreletrica> lista_jusante_desvio_hidreletrica = SmartEnupla<IdHidreletrica, IdHidreletrica>(IdHidreletrica(1), std::vector<IdHidreletrica>(IdHidreletrica(int(IdHidreletrica_Excedente) - 1), IdHidreletrica_Nenhum)); //Lista da jusante_desvio de cada hidrelétrica -> Somente para validação entre a configuração PD e CP

	SmartEnupla<IdHidreletrica, int> lista_hidreletrica_NPOSNW = SmartEnupla<IdHidreletrica, int>(IdHidreletrica(1), std::vector<int>(IdHidreletrica(int(IdHidreletrica_Excedente) - 1), -1));; //Lista que registra se uma usina tem diferença de posto entre DECOMP  e NEWAVE

	SmartEnupla<int, Hidreletrica> lista_hidreletrica_out_estudo; //Lista que registra usinas que não fazem parte do estudo mas são necessárias para desacoplar o corte NEWAVE (p.ex COMP PAF-MOX indicado no registro JUSENA para calcular a EAR do REE NE)

	SmartEnupla<IdReservatorioEquivalente, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>> lista_ENA_calculada_x_REE_x_cenario_x_periodo;

	/////////////////////////////////
	//Informação com o equacionamento do cálculo das ENAs x usina em termos das afluências incrementais x cenário x período (Regras.dat não lineares, precisa de um equacionamento por realização de afluência)

	//Para usinas instanciadas no estudo CP
	SmartEnupla<Periodo, SmartEnupla<IdHidreletrica, SmartEnupla<IdCenario, std::vector<SmartEnupla<IdHidreletrica, double>>>>>	lista_coeficiente_idHidreletricas_calculo_ENA_x_periodo_x_hidreletrica_x_cenario;
	SmartEnupla<Periodo, SmartEnupla<IdHidreletrica, SmartEnupla<IdCenario, double>>>							              	lista_termo_independente_calculo_ENA_x_periodo_x_hidreletrica_x_cenario;

	//Para usinas NÃO instanciadas no estudo CP (e.g. COMP-MOX-PAFONSO): a chave vai ser um int com o código da usina
	SmartEnupla<Periodo, SmartEnupla<           int, SmartEnupla<IdCenario, std::vector<SmartEnupla<IdHidreletrica, double>>>>>	lista_coeficiente_idHidreletricas_calculo_ENA_x_periodo_x_codigo_usina_x_cenario;
	SmartEnupla<Periodo, SmartEnupla<           int, SmartEnupla<IdCenario, double>>>								            lista_termo_independente_calculo_ENA_x_periodo_x_codigo_usina_x_cenario;

	//Para os REE
	SmartEnupla<Periodo, SmartEnupla<IdHidreletrica, SmartEnupla<IdReservatorioEquivalente, SmartEnupla<IdCenario, double>>>>	 lista_coeficiente_idHidreletricas_calculo_ENA_x_periodo_x_REE_x_cenario;
	SmartEnupla<Periodo, SmartEnupla<IdHidreletrica, SmartEnupla<IdReservatorioEquivalente, SmartEnupla<IdCenario, double>>>>	 lista_termo_independente_calculo_ENA_x_periodo_x_REE_x_cenario;

	/////////////////////////////////

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

			std::string menemonico_baixo = a_menemonico;
			for (int i = 0; i < a_menemonico.size(); i++)
				menemonico_baixo.at(i) = tolower(menemonico_baixo.at(i));

			if ((menemonico_baixo.find("sud") != std::string::npos) || strCompara(menemonico_baixo, "se") || strCompara(menemonico_baixo, "se/co"))
				return IdSubmercado_SUDESTE;

			else if ((menemonico_baixo.find("sul") != std::string::npos) || strCompara(menemonico_baixo, "s"))
				return IdSubmercado_SUL;

			else if ((menemonico_baixo.find("nord") != std::string::npos) || strCompara(menemonico_baixo, "ne"))
				return IdSubmercado_NORDESTE;

			else if ((menemonico_baixo.find("nort") != std::string::npos) || strCompara(menemonico_baixo, "n"))
				return IdSubmercado_NORTE;

			else if ((menemonico_baixo.find("fc") != std::string::npos) || (menemonico_baixo.find("imp") != std::string::npos) || (menemonico_baixo.find("fic") != std::string::npos) || strCompara(menemonico_baixo, "f"))
				return IdSubmercado_IMPERATRIZ;

			else if ((menemonico_baixo.find("an") != std::string::npos))
				return IdSubmercado_ANDE;

			else if ((menemonico_baixo.find("iv") != std::string::npos))
				return IdSubmercado_IVAIPORA;

			else if ((menemonico_baixo.find("it") != std::string::npos))
				return IdSubmercado_ITAIPU;

			for (IdSubmercado idSubmercado = lista_submercado_mnemonico.getIteradorInicial(); idSubmercado <= lista_submercado_mnemonico.getIteradorFinal(); idSubmercado++) {
				if (strCompara(menemonico_baixo, lista_submercado_mnemonico.getElemento(idSubmercado)))
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

	SmartEnupla<IdHidreletrica, std::vector<ModificacaoUHE>> lista_modificacaoUHE = SmartEnupla<IdHidreletrica, std::vector<ModificacaoUHE>>(IdHidreletrica(1), std::vector<std::vector<ModificacaoUHE>>(IdHidreletrica(int(IdHidreletrica_Excedente)-1), std::vector<ModificacaoUHE>()));

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
	
	bool aplicarModificacaoJUSENA(Dados& a_dados, const IdHidreletrica a_idHidreletrica, const ModificacaoUHE& a_modificacaoUHE);

	//Cria os intercâmbios hidráulicos correspondentes à jusante desvio 

	void adicionaIntercambiosHidraulicosApartirJusanteDesvio(Dados& a_dados);

	void adicionaLimitesDesvioApartirJusanteDesvio(Dados& a_dados);

};


#endif // !defined(C_TURBINA_H)
