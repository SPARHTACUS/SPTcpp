#include "C_LeituraCEPEL.h"
#include "C_EntradaSaidaDados.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <cmath>
#include <vector>


void LeituraCEPEL::leitura_DESSEM(Dados& a_dados, const std::string a_nomeArquivo, const std::string a_nomePasta) {

	try {
		bool readPoliJusHidr_dat = true;
		std::ifstream poliJus(a_nomePasta + "/polinjus.dat");
		if (poliJus.is_open()) { readPoliJusHidr_dat = false; }

		std::ifstream myfile(a_nomeArquivo);
		std::string line;

		// LEITURAS DOS ARQUIVOS NA SEQUENCIA INFORMADA NO ARQUIVO dessem.arq
		if (myfile.is_open()) {


			while (std::getline(myfile, line)) {

				strNormalizada(line);

				std::string arquivo = line.substr(49, 80); arquivo.erase(std::remove(arquivo.begin(), arquivo.end(), ' '), arquivo.end());
				std::string mnemonico = line.substr(0, 9); mnemonico.erase(std::remove(mnemonico.begin(), mnemonico.end(), ' '), mnemonico.end());

				// LEITURA DOS DADOS DE VAZÕES NATURAIS (dadvaz.dat)
				if (mnemonico == "VAZOES") { leitura_VAZOES_201904_DES16(a_dados, a_nomePasta + "//" + arquivo); }

				// LEITURA DO ARQUIVO DE DADOS GERAIS DO PROBLEMA (entdados.dat)
				else if (mnemonico == "DADGER") { leitura_DADGER_201904_DES16(a_dados, a_nomePasta + "//" + arquivo); }

				//  LEITURA DO ARQUIVO DE DADOS  CADASTRO DAS USINAS TERMICAS (termdat.dat) 
				else if (mnemonico == "CADTERM") { leitura_CADTERM_201904_DES16(a_dados, a_nomePasta + "//" + arquivo); }

				//  LEITURA DO ARQUIVO DE DADOS OPERACAO DAS UNIDADES TERMICAS (operut.dat) 
				else if (mnemonico == "OPERUT") { leitura_OPERUT_201904_DES16(a_dados, a_nomePasta + "//" + arquivo); }

				//  LEITURA DO ARQUIVO DE DADOS DE RAMPAS (rampas.dat)
				else if (mnemonico == "RAMPAS") { leitura_RAMPAS_201904_DES16(a_dados, a_nomePasta + "//" + arquivo); }

				//  LEITURA DO ARQUIVO DE DADOS DE CADASTRO DAS USINAS HIDROELETRICAS (hidr.dat) 				
				else if (mnemonico == "CADUSIH") {
					inicializa_Submercados_Intercambios_Nao_Registrados(a_dados);
					leitura_CADUSIH_201904_NW25_DC29_DES16(a_dados, a_nomePasta + "//" + arquivo, false, readPoliJusHidr_dat);
				}

				//  LEITURA DO ARQUIVO DE DADOS DE RESTRICOES DE OPERACAO HIDRAULICA (operuh.dat) 
				//else if (mnemonico == "OPERUH") { leitura_OPERUH_201904_DES16(a_dados, a_nomePasta + "//" + arquivo); }

				//  LEITURA DO ARQUIVO DE DADOS DE DEFLUENCIAS ANTERIORES (deflant.dat)
				else if (mnemonico == "DEFLANT") { leitura_DEFLANT_201904_DES16(a_dados, a_nomePasta + "//" + arquivo); }

				//  LEITURA DO ARQUIVO DE DADOS DO CANAL PEREIRA BARRETO (ils_tri.dat) 
				else if (mnemonico == "ILSTRI") { leitura_ILSTRI_201904_DES16(a_dados, a_nomePasta + "//" + arquivo); }

				//  LEITURA DO ARQUIVO DE DADOS DE CADASTRO DE RESERVA DE POTENCIA (areacont.dat) 
				else if (mnemonico == "AREACONT") { leitura_AREACONT_201904_DES16(a_dados, a_nomePasta + "//" + arquivo); }

				//  LEITURA DO ARQUIVO DE DADOS DE ESTUDO DE  RESERVA DE POTENCIA (respot.dat)
				else if (mnemonico == "RESPOT") { leitura_RESPOT_201904_DES16(a_dados, a_nomePasta + "//" + arquivo); }

				//  LEITURA DO ARQUIVO DE DADOS DO PONTO DE OPERAÇÃO DE USINAS GNL (ptoper.dat)
				else if (mnemonico == "PTOPER") { leitura_PTOPER_201904_DES16(a_dados, a_nomePasta + "//" + arquivo); }

				//  LEITURA DO ARQUIVO DE DADOS DOS RESERVATORIO EQUIVALENTES DE ENERGIA (entdados.dat)
				else if (mnemonico == "REE") { leitura_REE_201904_DES16(a_dados, a_nomePasta + "//" + arquivo); }

				//  LEITURA DO ARQUIVO DE DADOS DE USINAS RENOVAVEIS (renovaveis.dat)
				else if (mnemonico == "EOLICA") { leitura_EOLICA_201904_DES16(a_dados, a_nomePasta + "//" + arquivo); }

				//  LEITURA DO ARQUIVO DE DADOS DAS COTAS NA R11 ANTERIORES
				else if (mnemonico == "COTASR11") { leitura_COTASR11_201904_DES16(a_dados, a_nomePasta + "//" + arquivo); }

				//  LEITURA DO ARQUIVO DE DADOS DAS COTAS NA R11 ANTERIORES
				else if (mnemonico == "TITULO") { a_dados.setAtributo(AttComumDados_nome_estudo, std::string(line.substr(52, 140))); }

			}//while (std::getline(myfile, line)) {

			myfile.close();
			myfile.clear();

		}//if (myfile.is_open()) {

		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo" + a_nomeArquivo + ".");

		// INSTANCIA PROCESSO ESTOCASTICO
		InstanciaProcessoEstocastico(a_dados, a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio()));

		// CALCULA REGULARIZAÇÃO HIDRELETRICAS
		calculaRegularizacaoUHE(a_dados);

		//Instancia intercâmbio hidráulicos das jusantes_desvio
		adicionaIntercambiosHidraulicosApartirJusanteDesvio(a_dados);

		validacoes_DESSEM(a_dados, a_nomePasta);

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_DESSEM: \n" + std::string(erro.what())); }

}


void LeituraCEPEL::calculaRegularizacaoUHE(Dados& a_dados) {

	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= a_dados.getMaiorId(IdHidreletrica()); idHidreletrica++) {
			a_dados.vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_regularizacao, SmartEnupla<Periodo, bool>(horizonte_estudo, false));

			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				if ((a_dados.getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo) > 0) && ((a_dados.getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_maximo) > 0))) {
					if ((a_dados.getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_maximo, periodo, double()) - a_dados.getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo, periodo, double())) > 0)
						a_dados.vetorHidreletrica.att(idHidreletrica).setElemento(AttVetorHidreletrica_regularizacao, periodo, true);
				}
				else if ((a_dados.getAtributo(idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_maximo, double()) - a_dados.getAtributo(idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_minimo, double())) > 0)
					a_dados.vetorHidreletrica.att(idHidreletrica).setElemento(AttVetorHidreletrica_regularizacao, periodo, true);

			}
		}
	}

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::calculaRegularizacaoUHE: \n" + std::string(erro.what())); }

}

Periodo LeituraCEPEL::getPeriodoInicial(Dados& a_dados, std::string diaInicial, std::string horaInicial, std::string meiaHoraInicial) {
	// METODO PARA CALCULAR O PERIODO INICIAL DAS RESTRIÇÕES
	try {
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());
		const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();
		IdHor idHoraInicial = periodo_inicial.getHora();
		IdMin idMinInicial = periodo_inicial.getMinuto();
		IdDia idDiaInicial = periodo_inicial.getDia();

		std::string diaInicialStr = diaInicial;
		diaInicialStr.erase(std::remove(diaInicialStr.begin(), diaInicialStr.end(), ' '), diaInicialStr.end());

		// QUANDO diaInicialStr == "I" A RESTRIÇÃO INICIA NO PERIODO INICIAL DO ESTUDO
		Periodo periodo_inicial_restricao = periodo_inicial;

		if (diaInicialStr != "I") {

			IdDia idDiaInicioEstudo = periodo_inicial.getDia();
			idMinInicial = IdMin_0;
			if (getIdDiaFromChar(diaInicial.c_str()) >= idDiaInicioEstudo) {

				idDiaInicial = getIdDiaFromChar(diaInicial.c_str());

				if (horaInicial != "  ") { idHoraInicial = getIdHorFromChar(horaInicial.c_str()); }

				if (meiaHoraInicial == "1") { idMinInicial = IdMin_30; }

				periodo_inicial_restricao = Periodo(TipoPeriodo_meia_hora, idDiaInicial, periodo_inicial.getMes(), periodo_inicial.getAno(), idHoraInicial, idMinInicial);

			}// if (getIdDiaFromChar(diaInicial.c_str()) >= idDiaInicioEstudo) {

		}//if (diaInicialStr != "I")

		return periodo_inicial_restricao;

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::periodo_inicio_restricao: \n" + std::string(erro.what())); }
}


Periodo LeituraCEPEL::getPeriodoFinal(Dados& a_dados, std::string diaFinal, std::string horaFinal, std::string meiaHoraFinal) {

	// METODO PARA CALCULAR O PERIODO FINAL DAS RESTRIÇÕES 
	try {
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());
		const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();

		IdHor   idHoraFinal = periodo_final_estudo.getHora();
		IdMin   idMinFinal = periodo_final_estudo.getMinuto();
		IdDia   idDiaFinal = periodo_final_estudo.getDia();
		IdMes   idMesInicio = periodo_inicial.getMes();
		IdMes   idMesFim = periodo_final_estudo.getMes();
		IdAno   idAnoInicio = periodo_inicial.getAno();
		IdAno   idAnoFim = periodo_final_estudo.getAno();

		// QUANDO diaInicialStr == "f" A RESTRIÇÃO VAI ATÉ O  PERIODO FINAL DO ESTUDO
		Periodo periodo_final_restricao = periodo_final_estudo;

		std::string diaFinalStr = diaFinal;
		diaFinalStr.erase(std::remove(diaFinalStr.begin(), diaFinalStr.end(), ' '), diaFinalStr.end());

		if (diaFinalStr != "F") {
			IdDia idDiaFinalEstudo = periodo_final_estudo.getDia();
			idMinFinal = IdMin_0;

			// FINAL DA RESTRIÇÃO NO MESMO MÊS DO INICIO DO ESTUDO 
			if (getIdDiaFromChar(diaFinal.c_str()) <= idDiaFinalEstudo && idMesInicio == idMesFim) {

				IdDia idDiaFinal = getIdDiaFromChar(diaFinal.c_str());

				if (horaFinal != "  ") { idHoraFinal = getIdHorFromChar(horaFinal.c_str()); }

				if (meiaHoraFinal == "1") { idMinFinal = IdMin_30; }

				periodo_final_restricao = Periodo(TipoPeriodo_meia_hora, idDiaFinal, periodo_final_estudo.getMes(), periodo_final_estudo.getAno(), idHoraFinal, idMinFinal);

			}// if (getIdDiaFromChar(diaFinal.c_str()) <= idDiaFinalEstudo) {

			// CONDIÇÃO PARA TRANSIÇÃO DE MÊS
			if (getIdDiaFromChar(diaFinal.c_str()) > idDiaFinalEstudo && idMesInicio < idMesFim) {

				IdDia idDiaFinal = getIdDiaFromChar(diaFinal.c_str());
				idMinFinal = IdMin_0;
				if (horaFinal != "  ") { idHoraFinal = getIdHorFromChar(horaFinal.c_str()); }

				if (meiaHoraFinal == "1") { idMinFinal = IdMin_30; }


				periodo_final_restricao = Periodo(TipoPeriodo_meia_hora, idDiaFinal, idMesInicio, periodo_inicial.getAno(), idHoraFinal, idMinFinal);

			}// if (getIdDiaFromChar(diaFinal.c_str()) <= idDiaFinalEstudo) {

		}//if (diaFinalStr != "F") {	 	  

		return periodo_final_restricao;
	}//try {

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::periodo_final_restricao: \n" + std::string(erro.what())); }
}


Periodo LeituraCEPEL::getPeriodoInicialResticao(Periodo a_periodoInicialRestricao, SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo) {
	try {
		if (a_periodoInicialRestricao <= a_horizonte_estudo.getIteradorInicial())
			return a_horizonte_estudo.getIteradorInicial();
		else {
			Periodo periodo_inicial_restricao;

			const Periodo periodoInicial = a_horizonte_estudo.getIteradorInicial();
			const Periodo periodoFinal = a_horizonte_estudo.getIteradorFinal();

			for (Periodo periodo = periodoInicial; periodo <= periodoFinal; a_horizonte_estudo.incrementarIterador(periodo))
				if (periodo <= a_periodoInicialRestricao)
					periodo_inicial_restricao = periodo;
			return periodo_inicial_restricao;
		}
	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::getPeriodoInicialResticao: \n" + std::string(erro.what())); }
}


Periodo LeituraCEPEL::getPeriodoFinalResticao(Periodo a_periodoFinalRestricao, SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo) {
	try {
		if (a_periodoFinalRestricao <= a_horizonte_estudo.getIteradorInicial())
			return a_periodoFinalRestricao;
		for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo)) {
			if (periodo == a_periodoFinalRestricao)
				return periodo;
			else if (periodo >= a_periodoFinalRestricao)
				return periodo;
		}
		return a_horizonte_estudo.getIteradorFinal();
	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::getPeriodoFinalResticao: \n" + std::string(erro.what())); }
}


void LeituraCEPEL::leitura_OPERUT_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo) {

	// QUANDO INFORMADO APENAS DOIS ARGUMENTOS PARA O METODO É FEITA A LEITURA DE TODOS OS RESGISTROS 
	// CASO SEJA INFORMADO UM REGISTRO É FEITA A LEITURA APENAS DESTE REGISTRO

	try { leitura_OPERUT_201904_DES16(a_dados, a_nomeArquivo, "", a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio())); }

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_OPERUT_201904_DES16: \n" + std::string(erro.what())); }
}


void LeituraCEPEL::leitura_OPERUH_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo) {

	// QUANDO INFORMADO APENAS DOIS ARGUMENTOS PARA O METODO É FEITA A LEITURA DE TODOS OS RESGISTROS 
	// CASO SEJA INFORMADO UM REGISTRO É FEITA A LEITURA APENAS DESTE REGISTRO

	try { leitura_OPERUH_201904_DES16(a_dados, a_nomeArquivo, "", a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio())); }

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_OPERUH_201904_DES16: \n" + std::string(erro.what())); }
}


void LeituraCEPEL::leitura_DADGER_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo) {

	// QUANDO INFORMADO APENAS DOIS ARGUMENTOS PARA O METODO É FEITA A LEITURA DE TODOS OS RESGISTROS 
	// CASO SEJA INFORMADO UM REGISTRO É FEITA A LEITURA APENAS DESTE REGISTRO

	try { leitura_DADGER_201904_DES16(a_dados, a_nomeArquivo, "", a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio())); }

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_DADGER_201904_DES16: \n" + std::string(erro.what())); }
}


void LeituraCEPEL::leitura_REE_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo) {

	// QUANDO INFORMADO APENAS DOIS ARGUMENTOS PARA O METODO É FEITA A LEITURA DE TODOS OS RESGISTROS 
	// CASO SEJA INFORMADO UM REGISTRO É FEITA A LEITURA APENAS DESTE REGISTRO

	try { leitura_REE_201904_DES16(a_dados, a_nomeArquivo, "", a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio())); }

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_REE_201904_DES16: \n" + std::string(erro.what())); }
}


void LeituraCEPEL::leitura_CADTERM_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo) {

	// QUANDO INFORMADO APENAS DOIS ARGUMENTOS PARA O METODO É FEITA A LEITURA DE TODOS OS RESGISTROS 
	// CASO SEJA INFORMADO UM REGISTRO É FEITA A LEITURA APENAS DESTE REGISTRO

	try { leitura_CADTERM_201904_DES16(a_dados, a_nomeArquivo, "", a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio())); }

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_CADTERM_201904_DES16: \n" + std::string(erro.what())); }
}


void LeituraCEPEL::InstanciaProcessoEstocastico(Dados& a_dados, SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo) {

	try {
		const SmartEnupla<IdEstagio, Periodo> horizonte_otimizacao = a_dados.getVetor(AttVetorDados_horizonte_otimizacao, IdEstagio(), Periodo());
		const IdEstagio estagio_inicial = horizonte_otimizacao.getIteradorInicial();
		const Periodo periodo_inicial = horizonte_otimizacao.getElemento(estagio_inicial);

		a_dados.setAtributo(AttComumDados_imprimir_solver, true);
		a_dados.setAtributo(AttComumDados_exibir_na_tela_resultado_solver, true);
		//a_dados.setAtributo(AttComumDados_metodo_solver, TipoMetodoSolver_barrier);
		a_dados.setAtributo(AttComumDados_tipo_solver, TipoSolver_gurobi);
		//a_dados.setAtributo(AttComumDados_diretorio_importacao_pos_estudo, std::string("DadosSaidaCP//Otimizacao//AcoplamentoPreEstudo") );
		a_dados.setAtributo(AttComumDados_estagio_acoplamento_pre_estudo, IdEstagio_2);
		a_dados.setAtributo(AttComumDados_imprimir_resultado_por_estagio_por_cenario, true);
		a_dados.setAtributo(AttComumDados_tipo_metodo_solucao, IdMetodoSolucao_MILP);
		a_dados.setAtributo(AttComumDados_tipo_espaco_amostral_geracao_cenario_hidrologico, TipoEspacoAmostral_arvore_cenarios);
		a_dados.setAtributo(AttComumDados_tipo_estudo, TipoEstudo_otimizacao);
		a_dados.setAtributo(AttComumDados_tipo_relaxacao_afluencia_incremental, TipoRelaxacaoAfluenciaIncremental_sem_relaxacao);
		a_dados.setAtributo(AttComumDados_tipo_correlacao_geracao_cenario_hidrologico, TipoCorrelacaoVariaveisAleatorias_sem_correlacao);

		a_dados.setVetor(AttVetorDados_numero_aberturas, SmartEnupla<IdEstagio, int>(IdEstagio_1, std::vector<int>(IdEstagio_1, 1)));

		a_dados.processoEstocastico_hidrologico = ProcessoEstocastico();

		a_dados.processoEstocastico_hidrologico.setAtributo(AttComumProcessoEstocastico_idProcessoEstocastico, IdProcessoEstocastico_hidrologico_hidreletrica);

		a_dados.processoEstocastico_hidrologico.setAtributo(AttComumProcessoEstocastico_tipo_espaco_amostral, TipoEspacoAmostral_arvore_cenarios);
		a_dados.processoEstocastico_hidrologico.setAtributo(AttComumProcessoEstocastico_tipo_correlacao_variaveis_aleatorias, TipoCorrelacaoVariaveisAleatorias_sem_correlacao);
		a_dados.processoEstocastico_hidrologico.setMatriz(AttMatrizProcessoEstocastico_probabilidade_realizacao, \
			SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>>(a_horizonte_estudo, SmartEnupla<IdRealizacao, double>(IdRealizacao_1, std::vector<double>(IdRealizacao_1, 1.0))));

		Periodo proximo_periodo = Periodo(a_horizonte_estudo.getIteradorFinal());

		a_dados.processoEstocastico_hidrologico.setMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, \
			SmartEnupla<IdCenario, SmartEnupla<Periodo, IdRealizacao>>(IdCenario_1, std::vector<SmartEnupla<Periodo, IdRealizacao>>(1, SmartEnupla<Periodo, IdRealizacao>(proximo_periodo, std::vector<IdRealizacao>(1, IdRealizacao_1)))));


		IdHidreletrica maiorIdHidreletrica = a_dados.vetorHidreletrica.getMaiorId();
		bool preencher_horizonte = true;
		for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

			VariavelAleatoria variavelAleatoria;
			const IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria(a_dados.processoEstocastico_hidrologico.getMaiorId(IdVariavelAleatoria()) + 1);

			variavelAleatoria.setAtributo(AttComumVariavelAleatoria_idVariavelAleatoria, idVariavelAleatoria);
			variavelAleatoria.setAtributo(AttComumVariavelAleatoria_tipo_modelo_geracao_sintetica, a_dados.getAtributo(AttComumDados_tipo_modelo_geracao_cenario_hidrologico, TipoModeloGeracaoSinteticaCenario()));
			variavelAleatoria.setAtributo(AttComumVariavelAleatoria_tipo_coeficiente_auto_correlacao, a_dados.getAtributo(AttComumDados_tipo_coeficiente_auto_correlacao_geracao_cenario_hidrologico, TipoValor()));
			variavelAleatoria.setAtributo(AttComumVariavelAleatoria_ordem_maxima_coeficiente_auto_correlacao, a_dados.getAtributo(AttComumDados_ordem_maxima_auto_correlacao_geracao_cenario_hidrologico, int()));
			a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.add(variavelAleatoria);


			VariavelAleatoriaInterna variavelAleatoriaInterna;
			variavelAleatoriaInterna.setAtributo(AttComumVariavelAleatoriaInterna_idVariavelAleatoriaInterna, IdVariavelAleatoriaInterna_1);
			variavelAleatoriaInterna.setAtributo(AttComumVariavelAleatoriaInterna_nome, getFullString(idHidreletrica));
			a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.att(idVariavelAleatoria).vetorVariavelAleatoriaInterna.add(variavelAleatoriaInterna);

			// XXX DESSEM

			Periodo periodo_inicial = a_horizonte_estudo.getIteradorInicial();

			a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.att(idVariavelAleatoria).vetorVariavelAleatoriaInterna.att(IdVariavelAleatoriaInterna_1).addElemento(AttVetorVariavelAleatoriaInterna_grau_liberdade, periodo_inicial, 0.0);

			if (a_dados.getSize1Matriz(idHidreletrica, IdAfluencia_vazao_afluente, AttMatrizAfluencia_incremental) > 0) {
				for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); (periodo = periodo + 48)) {
					//a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.att(idVariavelAleatoria).addElemento(AttMatrizVariavelAleatoria_residuo_espaco_amostral, Periodo(periodo.getDia(), periodo.getMes(), periodo.getAno()), IdRealizacao_1, a_dados.getElementoMatriz(idHidreletrica, IdAfluencia_vazao_afluente, AttMatrizAfluencia_incremental, IdCenario_1, getPeriodoInicialResticao(periodo, a_horizonte_estudo), double()));
					a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.att(idVariavelAleatoria).vetorVariavelAleatoriaInterna.att(IdVariavelAleatoriaInterna_1).addElemento(AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral, IdCenario_1,  Periodo(periodo.getDia(), periodo.getMes(), periodo.getAno()), a_dados.getElementoMatriz(idHidreletrica, IdAfluencia_vazao_afluente, AttMatrizAfluencia_incremental, IdCenario_1, getPeriodoInicialResticao(periodo, a_horizonte_estudo), double()));
					a_dados.processoEstocastico_hidrologico.vetorVariavelAleatoria.att(idVariavelAleatoria).vetorVariavelAleatoriaInterna.att(IdVariavelAleatoriaInterna_1).addElemento(AttMatrizVariavelAleatoriaInterna_tendencia_temporal, IdCenario_1, Periodo(periodo.getDia(), periodo.getMes(), periodo.getAno()), a_dados.getElementoMatriz(idHidreletrica, IdAfluencia_vazao_afluente, AttMatrizAfluencia_incremental, IdCenario_1, getPeriodoInicialResticao(periodo, a_horizonte_estudo), double()));

					//if (preencher_horizonte) {
					//	a_dados.processoEstocastico_hidrologico.addElemento(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario_1, Periodo(periodo.getDia(), periodo.getMes(), periodo.getAno()), IdRealizacao_1);
					//}
				}
			} // if (a_dados.getSize1Matriz(idHidreletrica, AttMatrizAfluencia_incremental) > 0) {
			else
				throw std::invalid_argument("O atributo " + getFullString(AttMatrizAfluencia_incremental) + " nao foi informado em " + getFullString(idHidreletrica));
			
			
			preencher_horizonte = false;

		} // for (IdHidreletrica idHidreletrica = IdHidreletrica_1; idHidreletrica <= maiorIdHidreletrica; idHidreletrica++) {

	} // try

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::InstanciaProcessoEstocasrico: \n" + std::string(erro.what())); }
}


void LeituraCEPEL::leitura_VAZOES_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo) {

	try {
		instancia_termeletricas_preConfig(a_dados, a_dados.getAtributo(AttComumDados_diretorio_entrada_dados, std::string()) + "_PRECONFIG");
		IdAfluencia idAfluencia = IdAfluencia_vazao_afluente;
		IdCenario idCenario = IdCenario_1;

		std::ifstream myfile(a_nomeArquivo);
		std::string line;
		int numeroLinha = 0;
		int usinaAnterior = 0;
		SmartEnupla<IdEstagio, Periodo> horizonte_otimizacao;
		SmartEnupla<Periodo, IdEstagio> horizonte_estudo;

		if (myfile.is_open()) {

			while (std::getline(myfile, line)) {

				try {

					strNormalizada(line);

					numeroLinha++;

					if (line.at(0) != '&') {

						if (line.substr(0, 3) == "FIM") { break; }

						// PERIODO INICIAL DO ESTUDO 
						if (numeroLinha == 10) {
							a_dados.setAtributo(AttComumDados_periodo_referencia, Periodo(TipoPeriodo_meia_hora, getIdDiaFromChar(line.substr(4, 2).c_str()), \
								getIdMesFromChar(line.substr(8, 2).c_str()), getIdAnoFromChar(line.substr(12, 4).c_str()), getIdHorFromChar(line.substr(0, 2).c_str()), IdMin_0));
						}//if (cont == 10) {

						// PERIODO FINAL DO ESTUDO
						if (numeroLinha == 13) {
							int numeroDias = (std::stoi(line.substr(4, 1)) * 7) - std::stoi(line.substr(0, 1)) + 1;
							Periodo periodo_estudo_inicial = a_dados.getAtributo(AttComumDados_periodo_referencia, Periodo());
							const Periodo periodo_final_estudo = periodo_estudo_inicial + ((numeroDias * 48) - (std::stoi(getString(periodo_estudo_inicial.getHora())) * 2));

							/*			for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo - 1; periodo++)
											horizonte_estudo.addElemento(periodo, IdEstagio_1);
										*/

							for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_estudo_inicial + 47; periodo++)
								horizonte_estudo.addElemento(periodo, IdEstagio_1);

							if (numeroDias > 1) {
								Periodo periodoRestante = Periodo(TipoPeriodo_horario, IdDia(periodo_estudo_inicial.getDia()), periodo_estudo_inicial.getMes(), periodo_estudo_inicial.getAno()) + 24;
								for (Periodo periodo = periodoRestante; periodo <= periodo_final_estudo - 1; periodo++)
									horizonte_estudo.addElemento(periodo, IdEstagio_1);
							}

							switch (numeroDias) {
							case 1: horizonte_otimizacao.addElemento(IdEstagio_1, Periodo(TipoPeriodo_diario, periodo_estudo_inicial));  break;
							case 2: horizonte_otimizacao.addElemento(IdEstagio_1, Periodo(TipoPeriodo_2dias, periodo_estudo_inicial));  break;
							case 3: horizonte_otimizacao.addElemento(IdEstagio_1, Periodo(TipoPeriodo_3dias, periodo_estudo_inicial));  break;
							case 4: horizonte_otimizacao.addElemento(IdEstagio_1, Periodo(TipoPeriodo_4dias, periodo_estudo_inicial));  break;
							case 5: horizonte_otimizacao.addElemento(IdEstagio_1, Periodo(TipoPeriodo_5dias, periodo_estudo_inicial));  break;
							case 6: horizonte_otimizacao.addElemento(IdEstagio_1, Periodo(TipoPeriodo_6dias, periodo_estudo_inicial));  break;
							case 7: horizonte_otimizacao.addElemento(IdEstagio_1, Periodo(TipoPeriodo_semanal, periodo_estudo_inicial));  break;
							} //switch (numeroDias) {

							a_dados.setVetor(AttVetorDados_horizonte_otimizacao, horizonte_otimizacao);
							a_dados.setVetor(AttVetorDados_horizonte_estudo, horizonte_estudo);

						}// if (cont == 13) {


						// LEITUA DOS DADOS DE AFLUÊNCIA E DAS UHEs A SER UTILIZADAS NO ESTUDO
						if (numeroLinha == 17)
							instancia_hidreletricas_preConfig(a_dados, a_dados.getAtributo(AttComumDados_diretorio_entrada_dados, std::string()) + "_PRECONFIG");

						if (numeroLinha > 16) {


							LeituraCEPEL leituraCEPEL;

							Periodo periodoInicialRestricao = getPeriodoInicial(a_dados, line.substr(24, 2), line.substr(27, 2), line.substr(30, 1));
							Periodo periodoIFinalRestricao = getPeriodoFinal(a_dados, line.substr(32, 2), line.substr(35, 2), line.substr(38, 1));

							const Periodo periodo_inicial = a_dados.getAtributo(AttComumDados_periodo_referencia, Periodo());

							//BaciaHidrografica baciaHidrografica;

							AttMatrizAfluencia atributo = AttMatrizAfluencia_natural;
							switch (std::stoi(line.substr(19, 1))) {
							case 1: atributo = AttMatrizAfluencia_incremental;   break;
							case 2: throw std::invalid_argument("Presença de afluencia natural no aquivo" + a_nomeArquivo + ".");       break;
							case 3: throw std::invalid_argument("Presença de afluencia regularizada no aquivo" + a_nomeArquivo + ".");  break;
							} //switch (tipo) {

							IdHidreletrica idHidreletricaInstanciada = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, std::stoi(line.substr(0, 3)));

							// INSTANCIA AS UHEs NÃO INSTANCIADAS 
							if ((idHidreletricaInstanciada == IdHidreletrica_Nenhum) && (!hidreletricasPreConfig_instanciadas)) {

								IdHidreletrica idHidreletrica = IdHidreletrica(a_dados.vetorHidreletrica.getMaiorId() + 1);
								//const IdBaciaHidrografica idBaciaHidrografica = IdBaciaHidrografica(int(idHidreletrica));

								lista_hidreletrica_sem_capacidade.addElemento(idHidreletrica, false);
								lista_hidreletrica_maiorIdConjuntoHidraulico.addElemento(idHidreletrica, IdConjuntoHidraulico_Nenhum);
								lista_codigo_ONS_hidreletrica.addElemento(idHidreletrica, std::stoi(line.substr(0, 3)));
								lista_IdSubmercado_hidreletrica.addElemento(idHidreletrica, IdSubmercado_Nenhum);

								//INSTANCIA A HIDRELÉTRICA
								Hidreletrica hidreletrica;
								hidreletrica.setAtributo(AttComumHidreletrica_idHidreletrica, idHidreletrica);
								hidreletrica.setAtributo(AttComumHidreletrica_codigo_usina, std::stoi(line.substr(0, 3)));
								hidreletrica.setAtributo(AttComumHidreletrica_nome, line.substr(4, 12));
								a_dados.vetorHidreletrica.add(hidreletrica);

								// TIPO DE DETALHAMENTO DA PRODUÇÃO
								a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica_por_usina);
								a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.add(FuncaoProducaoHidreletrica());
								a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).setAtributo(AttComumFuncaoProducaoHidreletrica_percentual_volume, 1.0);


								//INSTANCIA BACIA HIDROGRAFICA
								atribui_bacia_hidrografica(a_dados, std::stoi(line.substr(0, 3)));

								/*	baciaHidrografica.setAtributo(AttComumBaciaHidrografica_idBaciaHidrografica, idBaciaHidrografica);
									baciaHidrografica.setAtributo(AttComumBaciaHidrografica_nome, line.substr(4, 12));
									a_dados.vetorBaciaHidrografica.add(baciaHidrografica);
								*/
							}//if (idHidreletricaInstanciada == IdHidreletrica_Nenhum) {

							idHidreletricaInstanciada = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, std::stoi(line.substr(0, 3)));

							if ((!a_dados.vetorHidreletrica.att(idHidreletricaInstanciada).vetorAfluencia.isInstanciado(idAfluencia))) {

								//INSTANCIA A AFLUÊNCIA
								Afluencia afluencia;
								afluencia.setAtributo(AttComumAfluencia_idAfluencia, idAfluencia);
								a_dados.vetorHidreletrica.att(idHidreletricaInstanciada).vetorAfluencia.add(afluencia);

								// CONSIDERA INDISPONIBILIDADE FORÇADA E PROGRAMADA = 0 NA PD, POIS TEN-SE AS INFORMAÇÕES DA DISPONIBILIDADE DAS UGs
								a_dados.vetorHidreletrica.att(idHidreletricaInstanciada).setVetor(AttVetorHidreletrica_indisponibilidade_forcada, SmartEnupla<Periodo, double>(horizonte_estudo, 0));
								a_dados.vetorHidreletrica.att(idHidreletricaInstanciada).setVetor(AttVetorHidreletrica_indisponibilidade_programada, SmartEnupla<Periodo, double>(horizonte_estudo, 0));

								// PRÉ CARREGA  MATRIZ DE AFLUÊNCIA COM 0
								a_dados.vetorHidreletrica.att(idHidreletricaInstanciada).vetorAfluencia.att(idAfluencia).setMatriz(atributo, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>(IdCenario_1, std::vector<SmartEnupla<Periodo, double>>(IdCenario_1, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0))));

							}//if ((!a_dados.vetorHidreletrica.att(idHidreletricaInstanciada).vetorAfluencia.isInstanciado(idAfluencia))) {

							if ((!a_dados.vetorHidreletrica.att(idHidreletricaInstanciada).vetorDefluencia.isInstanciado(IdDefluencia_passada))) {

								//INSTANCIA A DEFLUÊNCIA
								Defluencia defluencia;
								defluencia.setAtributo(AttComumDefluencia_idDefluencia, IdDefluencia_passada);
								a_dados.vetorHidreletrica.att(idHidreletricaInstanciada).vetorDefluencia.add(defluencia);

								const SmartEnupla<Periodo, IdEstagio> horizonte_defluencia(Periodo(TipoPeriodo_diario, horizonte_estudo.getIteradorInicial()) - 15, std::vector<IdEstagio>(15, IdEstagio_1));

								// PRÉ CARREGA VETOR DEFLUÊNCIA PASSADAS COM 0.0
								a_dados.vetorHidreletrica.att(idHidreletricaInstanciada).vetorDefluencia.att(IdDefluencia_passada).setVetor(AttVetorDefluencia_vazao_defluencia, SmartEnupla<Periodo, double>(horizonte_defluencia, 0.0));

							}//if ((!a_dados.vetorHidreletrica.att(idHidreletricaInstanciada).vetorDefluencia.isInstanciado(IdDefluencia_passada))) {


							// SET DAS AFLUÊNCIAS 
							switch (std::stoi(line.substr(19, 1))) {
							case 1:	for (Periodo periodo = getPeriodoInicialResticao(periodoInicialRestricao, horizonte_estudo); periodo <= getPeriodoFinalResticao(periodoIFinalRestricao, horizonte_estudo); horizonte_estudo.incrementarIterador(periodo)) { a_dados.vetorHidreletrica.att(idHidreletricaInstanciada).vetorAfluencia.att(idAfluencia).setElemento(AttMatrizAfluencia_incremental, idCenario, periodo, double(std::stof(line.substr(44, 9)))); }	break;
							case 2:	for (Periodo periodo = getPeriodoInicialResticao(periodoInicialRestricao, horizonte_estudo); periodo <= getPeriodoFinalResticao(periodoIFinalRestricao, horizonte_estudo); horizonte_estudo.incrementarIterador(periodo)) { a_dados.vetorHidreletrica.att(idHidreletricaInstanciada).vetorAfluencia.att(idAfluencia).setElemento(AttMatrizAfluencia_natural, idCenario, periodo, double(std::stof(line.substr(44, 9)))); }	break;
							case 3:	for (Periodo periodo = getPeriodoInicialResticao(periodoInicialRestricao, horizonte_estudo); periodo <= getPeriodoFinalResticao(periodoIFinalRestricao, horizonte_estudo); horizonte_estudo.incrementarIterador(periodo)) { a_dados.vetorHidreletrica.att(idHidreletricaInstanciada).vetorAfluencia.att(idAfluencia).setElemento(AttMatrizAfluencia_regularizada, idCenario, periodo, double(std::stof(line.substr(44, 9)))); }	break;
							} // switch (tipo) {


						} // if (cont > 16) {

					} // if (line.at(0) != '&') {

				} // try {
				catch (const std::exception& erro) { throw std::invalid_argument(line + " \n" + std::string(erro.what())); }

			}// while (std::getline(myfile, line)) {

			myfile.close();

		} //if (myfile.is_open())

		else { throw std::invalid_argument("Nao foi possivel abrir o arquivo" + a_nomeArquivo + "."); }

	} // try

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_VAZOES_201904_DES16: \n" + std::string(erro.what())); }
}


void LeituraCEPEL::leitura_CADUSIH_201904_NW25_DC29_DES16(Dados& a_dados, const std::string a_nomeArquivo, const bool a_hidreletricasPreConfig_instanciadas, const bool a_readPoliJusHidr_dat) {

	try {

		// ESTRUTURA DO ARQUIVO BINÁRIO
		struct UHEs
		{
			char  nome[11];	         int   posto;		    char  postoDBH[8];	   int   subsistema;		  int   empresa;	          int   jusante;
			int   desvio;	         float volumeMin;	    float volumeMax;	   float volumeVertido;	      float volumeDesvio;	      float alturaMin;
			float alturaMax;         float coefVolume[5];   float coefArea[5];	   int   coefEvaporacao[12];  int   numConjuntos;	      int   numMaquinas[5];
			float potNominal[5];     float coefHBQT[5][5];  float coefHBQG[5][5];  float coefHBPT[5][5];      float alturaEfetiva[5];	  int   vazaoEfetiva[5];
			float produtibilidade;   float valorPerdas;	    int   numPoliJus;	   float coefJusante[6][5];	  float cotaJusanteRef[6];    float canalMed;
			int   vertInfCanalFuga;	 float fatorCargaMax;   float fatorCargaMin;   int   vazMinHistorica;     int   uniBase;	          int   tipoTurb;
			int   represConj;        float indispFor;       float indispProg;	   int   tipoPerda;		      char  sdata[8];	          char  obs[43];
			char  vRef[4];	         char  reg[1];
		};

		UHEs uhe;
		UHEs* uhe2;
		uhe2 = &uhe;

		int x = 0;
		std::ifstream leituraArquivo;
		leituraArquivo.open(a_nomeArquivo, std::ios_base::in | std::ios::binary);

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		while (!(leituraArquivo.eof())) {
			x++;
			leituraArquivo.read((char*)uhe2, 792);
			std::string nomeUHE(uhe.nome, 12);
			std::string nome = uhe.nome;
			nome.erase(std::remove(nome.begin(), nome.end(), ' '), nome.end());
			nome.erase(std::remove(nome.begin(), nome.end(), '\n'), nome.end());

			// ELIMINA OS ESPAÇOS NO FINAL DO NOME 
			nomeUHE.erase(std::find_if(nomeUHE.rbegin(), nomeUHE.rend(), std::bind1st(std::not_equal_to<char>(), ' ')).base(), nomeUHE.end());

			if (!nome.empty()) {

				const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, x);
				const std::string tipoReg(uhe.reg, 1);

				// CARREGA OS DADOS DA USINA CASO ELA VAI SER UTILIZADO NO ESTUDO, ESTAS JA DEVEM ESTAR INSTANCIADAS! 
				if (idHidreletrica != IdHidreletrica_Nenhum) {
					const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, uhe.subsistema);

					lista_IdSubmercado_hidreletrica.setElemento(idHidreletrica, idSubmercado);

					// USINA JUSANTE
					if ((deck_str == "DC") && (uhe.jusante != 0) && !a_hidreletricasPreConfig_instanciadas) {//Somente o DC não informa as usinas a jusante na configuração das hidrelétricas, então são carregadas como nenhum e logo no cadastro de modificações são atualizadas pelo Deck oficial
						const IdHidreletrica idHidreletricaJusante = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, uhe.jusante);
						if (idHidreletricaJusante != IdHidreletrica_Nenhum) { a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_jusante, idHidreletricaJusante); }
					}
					else if ((deck_str == "DC") && a_hidreletricasPreConfig_instanciadas) {//No modo carregar PreConfig, o CP precisa validar a configuração hidráulica respeito a jusantes/desvios, info do HIDR.dat + modificações NUMJUS (lidas posteriormente)  

						//Guarda informação em listas que servirão para a validação da configuração hidráulica entre a PD e o CP
						if (uhe.jusante != 0) {
							const IdHidreletrica idHidreletricaJusante = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, uhe.jusante);
							lista_jusante_hidreletrica.setElemento(idHidreletrica, idHidreletricaJusante);
						}//if (uhe.jusante != 0) {

						if (uhe.desvio != 0) {
							const IdHidreletrica idHidreletricaDesvio = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, uhe.desvio);
							lista_jusante_desvio_hidreletrica.setElemento(idHidreletrica, idHidreletricaDesvio);
						}//if (uhe.desvio != 0) {

					}//else if ((deck_str == "DC") && a_hidreletricasPreConfig_instanciadas) {

					if ((deck_str == "DS") && (uhe.jusante != 0) && (a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_jusante, IdHidreletrica()) == IdHidreletrica_Nenhum)) {
						const IdHidreletrica idHidreletricaJusante = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, uhe.jusante);
						if (idHidreletricaJusante != IdHidreletrica_Nenhum) { a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_jusante, idHidreletricaJusante); }
					}

					//CÓDIGO POSTO
					if ((deck_str == "DC") && (!a_hidreletricasPreConfig_instanciadas))//Somente o CP usa os postos lidos no HIDR.DAT, o MP usa o CONFHD.DAT ou a Preconfig e a PD usa o arquivo de tempo de viagem da água
						a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_codigo_posto, uhe.posto);

					// USINA DE DESVIO
					if (uhe.desvio != 0) {
						const IdHidreletrica idHidreletricaDesvio = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, uhe.desvio);
						if (idHidreletricaDesvio != IdHidreletrica_Nenhum) { a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_jusante_desvio, idHidreletricaDesvio); }
					}// if (uhe.desvio != 0) {

					// DADOS DAS USINA
					a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_numero_maquinas_base, int(uhe.uniBase));
					a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_nome, nomeUHE);
					a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_submercado, idSubmercado);
					a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_indisponibilidade_forcada, double(uhe.indispFor / 100));
					a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_indisponibilidade_programada, double(uhe.indispProg / 100));
					a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_canal_fuga_medio, double(uhe.canalMed));
					a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_vertimento_influencia_fcj, bool(uhe.vertInfCanalFuga));
					a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_vazao_defluente_minima_historica, double(uhe.vazMinHistorica));

					switch (int(uhe.tipoTurb)) {
					case 0: a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_tipo_turbina, TipoTurbina_sem_turbina);;  break;
					case 1: a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_tipo_turbina, TipoTurbina_francis);;  break;
					case 2: a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_tipo_turbina, TipoTurbina_kaplan);;  break;
					case 3: a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_tipo_turbina, TipoTurbina_pelton);;  break;
					}

					//INSTANCIA O RESERVATÓRIO, SE NÃO ESTIVER INSTANCIADO 
					if (a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.isInstanciado(IdReservatorio_1) == false) {
						Reservatorio reservatorio;
						reservatorio.setAtributo(AttComumReservatorio_idReservatorio, IdReservatorio_1);
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.add(reservatorio);
					}

					// CARREGA OS DADOS DOS RESERVATÓRIOS 
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_nome_usina, nomeUHE);
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_volume_minimo, double(uhe.volumeMin));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_volume_maximo, double(uhe.volumeMax));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_volume_minimo_vertimento, double(uhe.volumeVertido));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_volume_minimo_desvio, double(uhe.volumeDesvio));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_cota_minimo, double(uhe.alturaMin));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_cota_maximo, double(uhe.alturaMax));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_volume_0, double(uhe.coefVolume[0]));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_volume_1, double(uhe.coefVolume[1]));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_volume_2, double(uhe.coefVolume[2]));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_volume_3, double(uhe.coefVolume[3]));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_volume_4, double(uhe.coefVolume[4]));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_area_0, double(uhe.coefArea[0]));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_area_1, double(uhe.coefArea[1]));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_area_2, double(uhe.coefArea[2]));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_area_3, double(uhe.coefArea[3]));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_area_4, double(uhe.coefArea[4]));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_cota_referencia, a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getCotaMedia(double(uhe.volumeMin), double(uhe.volumeMax)));
					

					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Teste poli_cota_area: testa a cota_minima se der negativo zera o polinômio área
					// Nota: identificado áreas negativas inclusive com a cota_minima (polinômios mal condicionados)
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					if (true) {
						const double cota_minima = a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getAtributo(AttComumReservatorio_cota_minimo, double());

						const double A0 = a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getAtributo(AttComumReservatorio_poli_cota_area_0, double());
						const double A1 = a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getAtributo(AttComumReservatorio_poli_cota_area_1, double());
						const double A2 = a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getAtributo(AttComumReservatorio_poli_cota_area_2, double());
						const double A3 = a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getAtributo(AttComumReservatorio_poli_cota_area_3, double());
						const double A4 = a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getAtributo(AttComumReservatorio_poli_cota_area_4, double());

						double area_teste = A0;
						area_teste += A1 * cota_minima;
						area_teste += A2 * cota_minima * cota_minima;
						area_teste += A3 * cota_minima * cota_minima * cota_minima;
						area_teste += A4 * cota_minima * cota_minima * cota_minima * cota_minima;

						if (area_teste < 0) {
							std::cout << "Polinomio cota area mal condicionado no HIDR.dat da usina " << a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_nome, std::string()) << " -> coeficientes do polinomio zerados" << std::endl;

							a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_area_0, 0.0);
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_area_1, 0.0);
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_area_2, 0.0);
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_area_3, 0.0);
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_poli_cota_area_4, 0.0);

						}//if (area_teste < 0) {

					}//if (true) {
					
					
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_volume_0, SmartEnupla<Periodo, double>(horizonte_estudo, double(uhe.coefVolume[0])));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_volume_1, SmartEnupla<Periodo, double>(horizonte_estudo, double(uhe.coefVolume[1])));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_volume_2, SmartEnupla<Periodo, double>(horizonte_estudo, double(uhe.coefVolume[2])));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_volume_3, SmartEnupla<Periodo, double>(horizonte_estudo, double(uhe.coefVolume[3])));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_volume_4, SmartEnupla<Periodo, double>(horizonte_estudo, double(uhe.coefVolume[4])));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_area_0, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getAtributo(AttComumReservatorio_poli_cota_area_0, double())));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_area_1, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getAtributo(AttComumReservatorio_poli_cota_area_1, double())));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_area_2, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getAtributo(AttComumReservatorio_poli_cota_area_2, double())));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_area_3, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getAtributo(AttComumReservatorio_poli_cota_area_3, double())));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_poli_cota_area_4, SmartEnupla<Periodo, double>(horizonte_estudo, a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getAtributo(AttComumReservatorio_poli_cota_area_4, double())));
					
					
					//INICIA VETOR ENCHENDO VOLUME MORTO COM false
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_enchendo_volume_morto, SmartEnupla<Periodo, bool>(horizonte_estudo, false));

					// TIPO DE REGULARIZAÇÃO DA USINA
					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
						if ((tipoReg == "D") && (periodo.getTipoPeriodo() >= TipoPeriodo_diario)) { a_dados.vetorHidreletrica.att(idHidreletrica).addElemento(AttVetorHidreletrica_regularizacao, periodo, true); }
						else if ((tipoReg == "S") && (periodo.getTipoPeriodo() >= TipoPeriodo_semanal)) { a_dados.vetorHidreletrica.att(idHidreletrica).addElemento(AttVetorHidreletrica_regularizacao, periodo, true); }
						else if ((tipoReg == "M") && (periodo.getTipoPeriodo() >= TipoPeriodo_mensal)) { a_dados.vetorHidreletrica.att(idHidreletrica).addElemento(AttVetorHidreletrica_regularizacao, periodo, true); }
						else { a_dados.vetorHidreletrica.att(idHidreletrica).addElemento(AttVetorHidreletrica_regularizacao, periodo, false); }
					}


					// COEFICIENTES DE EVAPORAÇÃO MENSAL
					for (IdMes idMes = IdMes_1; idMes <= IdMes_12; idMes++)
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).addElemento(AttVetorReservatorio_evaporacao, idMes, double(uhe.coefEvaporacao[int(idMes) - 1]));

					if (lista_hidreletrica_sem_capacidade.getElemento(idHidreletrica)) {
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_volume_morto_completo, SmartEnupla<Periodo, bool>(horizonte_estudo, true));
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_enchendo_volume_morto, SmartEnupla<Periodo, bool>(horizonte_estudo, false));
						lista_hidreletrica_maiorIdConjuntoHidraulico.setElemento(idHidreletrica, IdConjuntoHidraulico_Nenhum);
					}
					else {
						lista_hidreletrica_maiorIdConjuntoHidraulico.setElemento(idHidreletrica, IdConjuntoHidraulico(uhe.numConjuntos));
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_volume_morto_completo, SmartEnupla<Periodo, bool>(horizonte_estudo, true));
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_enchendo_volume_morto, SmartEnupla<Periodo, bool>(horizonte_estudo, false));
					}


					if (int(uhe.numConjuntos) == 0)
						a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica_sem_producao);


					// CONJUNTOS HIDRAULICOS
					double queda_referencia_usina = 0;
					int numero_unidades_usina = 0;
					for (int conjunto = 0; conjunto < uhe.numConjuntos; conjunto++) {

						// VERIFICA SE O CONJUNTO HIDRAULICO JÁ FOI INSTANCIADO
						if (a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.isInstanciado(IdConjuntoHidraulico(conjunto + 1))) {

							//Esta condição é para o conjuntoHidraulico Itaipu 50 Hz que pode ser instanciado previamente
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico(conjunto + 1)).setAtributo(AttComumConjuntoHidraulico_queda_referencia, double(uhe.alturaEfetiva[conjunto]));
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico(conjunto + 1)).setAtributo(AttComumConjuntoHidraulico_nome, nomeUHE);
						}//if (a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.isInstanciado(IdConjuntoHidraulico(conjunto + 1))) {

						else {
							// INSTANCIA CONJUNTO HIDRAULICO
							ConjuntoHidraulico conjuntohidraulico;
							conjuntohidraulico.setAtributo(AttComumConjuntoHidraulico_idConjuntoHidraulico, IdConjuntoHidraulico(conjunto + 1));
							conjuntohidraulico.setAtributo(AttComumConjuntoHidraulico_submercado, idSubmercado);
							conjuntohidraulico.setAtributo(AttComumConjuntoHidraulico_queda_referencia, double(uhe.alturaEfetiva[conjunto]));
							conjuntohidraulico.setAtributo(AttComumConjuntoHidraulico_nome, nomeUHE);
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.add(conjuntohidraulico);
						}//else {

						// INSTANCIA AS UNIDADES DOS CONJUNTO HIDRAULICOS 
						for (int unidade = 1; unidade <= uhe.numMaquinas[conjunto]; unidade++) {
							UnidadeUHE unidadeUHE;
							unidadeUHE.setAtributo(AttComumUnidadeUHE_idUnidadeUHE, IdUnidadeUHE(unidade));
							unidadeUHE.setAtributo(AttComumUnidadeUHE_submercado, idSubmercado);
							unidadeUHE.setAtributo(AttComumUnidadeUHE_potencia_minima, 0.0);
							unidadeUHE.setAtributo(AttComumUnidadeUHE_potencia_maxima, double(uhe.potNominal[conjunto]));
							unidadeUHE.setAtributo(AttComumUnidadeUHE_vazao_turbinada_minima, 0.0);
							unidadeUHE.setAtributo(AttComumUnidadeUHE_vazao_turbinada_maxima, double(uhe.vazaoEfetiva[conjunto]));
							unidadeUHE.setAtributo(AttComumUnidadeUHE_fator_de_producao, double(uhe.produtibilidade));

							// TIPO DE PERDA HIDRAULICA
							if (uhe.tipoPerda == 1) {
								a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_tipo_de_perda_hidraulica, TipoPerdaHidraulica_percentual);
								a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_perda_hidraulica, double(uhe.valorPerdas / 100));
								unidadeUHE.setAtributo(AttComumUnidadeUHE_tipo_de_perda_hidraulica, TipoPerdaHidraulica_percentual);
								unidadeUHE.setAtributo(AttComumUnidadeUHE_perda_hidraulica, double(uhe.valorPerdas / 100));
							}
							else if (uhe.tipoPerda == 2) {
								a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_tipo_de_perda_hidraulica, TipoPerdaHidraulica_metro);
								a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_perda_hidraulica, double(uhe.valorPerdas));
								unidadeUHE.setAtributo(AttComumUnidadeUHE_tipo_de_perda_hidraulica, TipoPerdaHidraulica_metro);
								unidadeUHE.setAtributo(AttComumUnidadeUHE_perda_hidraulica, double(uhe.valorPerdas));
							}

							a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico(conjunto + 1)).vetorUnidadeUHE.add(unidadeUHE);

						}//for (int unidade = 1; unidade <= uhe.numMaquinas[conjunto]; unidade++) 

						queda_referencia_usina += uhe.numMaquinas[conjunto] * double(uhe.alturaEfetiva[conjunto]);
						numero_unidades_usina += uhe.numMaquinas[conjunto];

					}//for (int conjunto = 0; conjunto < uhe.numConjuntos; conjunto++) {

					//Calcula a queda_referencia_usina como a ponderaçao da queda_referencia por conjunto de acordo ao número de máquinas de cada conjunto dividido pelo numero total de máquinas
					queda_referencia_usina /= numero_unidades_usina;
					a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_queda_referencia, queda_referencia_usina);

					// TRATAMENTO CONJUNTO HIDRAULICO 60 HZ ITAIPU QUE ESTÁ CONECTADO NO NÓ IVAIPORÃ 
					if (x == 66) {

						if (itaipu_tipo_detalhamento_producao_por_conjunto) {

							a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica_por_conjunto);

							for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

								if (idConjuntoHidraulico == a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico())) {
									const IdSubmercado idSubmercado_IV = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_IV);
									if (idSubmercado_IV == IdSubmercado_Nenhum)
										throw std::invalid_argument("Submercado Ivaipora nao encontrado");
									a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).setAtributo(AttComumConjuntoHidraulico_submercado, idSubmercado_IV);
								}//if (idConjuntoHidraulico == a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico())) {

								else if (idConjuntoHidraulico == IdConjuntoHidraulico_1) {
									const IdSubmercado idSubmercado_ANDE = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_ANDE);
									if (idSubmercado_ANDE == IdSubmercado_Nenhum)
										throw std::invalid_argument("Submercado ANDE nao encontrado");
									a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).setAtributo(AttComumConjuntoHidraulico_submercado, idSubmercado_ANDE);
								}// else if (idConjuntoHidraulico == IdConjuntoHidraulico_1) {

								for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {
									const IdSubmercado idSubmercado_unidade = a_dados.getAtributo(idHidreletrica, idConjuntoHidraulico, AttComumConjuntoHidraulico_submercado, IdSubmercado());
									a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).setAtributo(AttComumUnidadeUHE_submercado, idSubmercado_unidade);
								}
							} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {							
						}//if (itaipu_tipo_detalhamento_producao_por_conjunto) {
						else {

							a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica_por_usina);
							const IdSubmercado idSubmercado_ITAIPU = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_submercado_ITAIPU);

							if (idSubmercado_ITAIPU == IdSubmercado_Nenhum)
								throw std::invalid_argument("Submercado ITAIPU nao encontrado");

							a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_submercado, idSubmercado_ITAIPU);

							for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {
								a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).setAtributo(AttComumConjuntoHidraulico_submercado, idSubmercado_ITAIPU);
								for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {
									const IdSubmercado idSubmercado_unidade = a_dados.getAtributo(idHidreletrica, idConjuntoHidraulico, AttComumConjuntoHidraulico_submercado, IdSubmercado());
									a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).setAtributo(AttComumUnidadeUHE_submercado, idSubmercado_unidade);
								}//for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= a_dados.getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {

							} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {

						}//else {

					} // if (x == 66) {

					//DADOS DOS POLINOMIOS DE JUSANTE
					if (a_readPoliJusHidr_dat) {
						for (int polinomio = 0; polinomio < uhe.numPoliJus; polinomio++) {
							const IdPolinomioJusante idPolinomioJusante = IdPolinomioJusante(polinomio + 1);
							PolinomioJusante polinomioJusante;
							polinomioJusante.setAtributo(AttComumPolinomioJusante_idPolinomioJusante, idPolinomioJusante);
							polinomioJusante.setAtributo(AttComumPolinomioJusante_nome, nomeUHE);
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorPolinomioJusante.add(polinomioJusante);
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorPolinomioJusante.att(idPolinomioJusante).setVetor(AttVetorPolinomioJusante_altura_ref, SmartEnupla<Periodo, double>(horizonte_estudo, double(uhe.cotaJusanteRef[polinomio])));
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorPolinomioJusante.att(idPolinomioJusante).setVetor(AttVetorPolinomioJusante_coeficiente_0, SmartEnupla<Periodo, double>(horizonte_estudo, double(uhe.coefJusante[polinomio][0])));
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorPolinomioJusante.att(idPolinomioJusante).setVetor(AttVetorPolinomioJusante_coeficiente_1, SmartEnupla<Periodo, double>(horizonte_estudo, double(uhe.coefJusante[polinomio][1])));
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorPolinomioJusante.att(idPolinomioJusante).setVetor(AttVetorPolinomioJusante_coeficiente_2, SmartEnupla<Periodo, double>(horizonte_estudo, double(uhe.coefJusante[polinomio][2])));
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorPolinomioJusante.att(idPolinomioJusante).setVetor(AttVetorPolinomioJusante_coeficiente_3, SmartEnupla<Periodo, double>(horizonte_estudo, double(uhe.coefJusante[polinomio][3])));
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorPolinomioJusante.att(idPolinomioJusante).setVetor(AttVetorPolinomioJusante_coeficiente_4, SmartEnupla<Periodo, double>(horizonte_estudo, double(uhe.coefJusante[polinomio][4])));
						}//for (int polinomio = 0; polinomio < uhe.numPoliJus; polinomio++) {
					}
				}// if(idHidreletrica.size() > 0){

			}// if (!nome.empty()) {

		} //while (!(leituraArquivo.eof())) {

		leituraArquivo.clear();
		leituraArquivo.close();

	}//	try {

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_CADUSIH_201904_NW25_DC29_DES16: \n" + std::string(erro.what())); }

}


void LeituraCEPEL::leitura_DEFLANT_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo) {

	try {
		LeituraCEPEL leituraCEPEL;
		std::ifstream myfile(a_nomeArquivo);
		std::string line;
		IdDefluencia idDefluencia = IdDefluencia_passada;

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());
		const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();

		if (myfile.is_open()) {

			while (std::getline(myfile, line)) {

				strNormalizada(line);

				if (line.substr(0, 6) == "DEFANT") {

					std::string diaFinal = line.substr(32, 2);
					diaFinal.erase(std::remove(diaFinal.begin(), diaFinal.end(), ' '), diaFinal.end());


					IdDia idDiaInicial = IdDia(std::stoi(line.substr(24, 2)));
					IdMes idMesInicial = periodo_inicial.getMes();
					IdAno idAnoInicial = periodo_inicial.getAno();
					IdDia idDiaInicio_estudo = periodo_inicial.getDia();

					if (idDiaInicial > idDiaInicio_estudo) {
						if (idMesInicial == IdMes_1) { idAnoInicial--; idMesInicial = IdMes_12; }
						else idMesInicial--;
					}

					Periodo periodo_inicial_defluencia = Periodo(TipoPeriodo_diario, idDiaInicial, idMesInicial, idAnoInicial);
					Periodo periodo_final_defluencia = a_dados.getAtributo(AttComumDados_periodo_referencia, Periodo());

					if (diaFinal != "F") {

						IdDia idDiaFinal = IdDia(std::stoi(diaFinal) + 1);
						IdMes idMesFinal = periodo_inicial.getMes();
						IdAno idAnoFinal = periodo_inicial.getAno();
						IdDia idDiaInicio_estudo = periodo_inicial.getDia();

						if (idDiaInicial > idDiaInicio_estudo) {
							if (idMesInicial == IdMes_1) { idAnoInicial--; idMesInicial = IdMes_12; }
							else idMesInicial--;
						}
						periodo_final_defluencia = Periodo(TipoPeriodo_meia_hora, idDiaFinal, idMesFinal, idAnoFinal);
					}

					const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, std::stoi(line.substr(9, 3)));
					const IdHidreletrica idHidreletricaJusante = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, std::stoi(line.substr(14, 3)));

					for (Periodo periodo = periodo_inicial_defluencia; periodo < periodo_final_defluencia; periodo++)
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorDefluencia.att(IdDefluencia_passada).setElemento(AttVetorDefluencia_vazao_defluencia, periodo, std::stod(line.substr(44, 10)));

					if (line.substr(19, 1) == "H") { a_dados.vetorHidreletrica.att(idHidreletrica).vetorDefluencia.att(IdDefluencia_passada).setAtributo(AttComumDefluencia_tipo_elemento_jusante, TipoElementoJusante_usina); }
					if (line.substr(19, 1) == "S") { a_dados.vetorHidreletrica.att(idHidreletrica).vetorDefluencia.att(IdDefluencia_passada).setAtributo(AttComumDefluencia_tipo_elemento_jusante, TipoElementoJusante_secao_rio); }



				}// if (line.substr(0, 6) == "DEFANT") {

			}// while (std::getline(myfile, line)) {

			myfile.close();

		}// if (myfile.is_open()) {

		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo" + a_nomeArquivo + ".");

	}// try {

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_DEFLANT_201904_DES16: \n" + std::string(erro.what())); }

}


void LeituraCEPEL::leitura_ILSTRI_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo) {

	try {

		std::ifstream myfile(a_nomeArquivo);
		std::string line;
		std::vector<std::string> nivelReservatorio(12);

		const int codigo_ONS_ilha_solteira = 34; //Código ilha solteira do HIDR.dat
		const int codigo_ONS_tres_irmaos = 43;   //Código três irmãos do HIDR.dat

		const IdHidreletrica idHidreletrica_ilha_solteira = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_ONS_ilha_solteira);
		const IdHidreletrica idHidreletrica_tres_irmaos = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, codigo_ONS_tres_irmaos);


		if (myfile.is_open()) {

			const IdIntercambioHidraulico idIntercambioHidraulico_ilha_solteira = IdIntercambioHidraulico(a_dados.getMaiorId(IdIntercambioHidraulico()) + 1);
			IntercambioHidraulico intercambioHidraulico_ilha_solteira;
			intercambioHidraulico_ilha_solteira.setAtributo(AttComumIntercambioHidraulico_idIntercambioHidraulico, idIntercambioHidraulico_ilha_solteira);
			a_dados.vetorIntercambioHidraulico.add(intercambioHidraulico_ilha_solteira);
			a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico_ilha_solteira).setAtributo(AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico_pereira_barreto);
			a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico_ilha_solteira).setAtributo(AttComumIntercambioHidraulico_hidreletrica_origem, idHidreletrica_ilha_solteira);
			a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico_ilha_solteira).setAtributo(AttComumIntercambioHidraulico_hidreletrica_destino, idHidreletrica_tres_irmaos);


			const IdIntercambioHidraulico idIntercambioHidraulico_tres_irmaos = IdIntercambioHidraulico(a_dados.getMaiorId(IdIntercambioHidraulico()) + 1);
			IntercambioHidraulico intercambioHidraulico_tres_irmaos;
			intercambioHidraulico_tres_irmaos.setAtributo(AttComumIntercambioHidraulico_idIntercambioHidraulico, idIntercambioHidraulico_tres_irmaos);
			a_dados.vetorIntercambioHidraulico.add(intercambioHidraulico_tres_irmaos);
			a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico_tres_irmaos).setAtributo(AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico_pereira_barreto);
			a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico_tres_irmaos).setAtributo(AttComumIntercambioHidraulico_hidreletrica_origem, idHidreletrica_tres_irmaos);
			a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico_tres_irmaos).setAtributo(AttComumIntercambioHidraulico_hidreletrica_destino, idHidreletrica_ilha_solteira);

			int contLinha = 0;

			while (std::getline(myfile, line)) {

				strNormalizada(line);

				std::string lineStr = line;
				lineStr.erase(std::remove(lineStr.begin(), lineStr.end(), ' '), lineStr.end());

				if (line.substr(0, 3) == "NIV") {

					for (int i = 0; i < 12; i++) {
						if (i == 0) {
							a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico_ilha_solteira).addElemento(AttVetorIntercambioHidraulico_nivel_reservatorio, i + 1, std::stod(line.substr(4, 6)));
							a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico_tres_irmaos).addElemento(AttVetorIntercambioHidraulico_nivel_reservatorio, i + 1, std::stod(line.substr(4, 6)));
						}
						else if (i != 0) {
							a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico_ilha_solteira).addElemento(AttVetorIntercambioHidraulico_nivel_reservatorio, i + 1, std::stod(line.substr((i * 7) + 4, 6)));
							a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico_tres_irmaos).addElemento(AttVetorIntercambioHidraulico_nivel_reservatorio, i + 1, std::stod(line.substr((i * 7) + 4, 6)));
						}
					}//for (int i = 0; i < 12; i++) {

				}//if (line.substr(0, 3) == "NIV" ) {

				if (lineStr != "" && line.substr(0, 1) != "&") {

					if (line.substr(0, 3) != "NIV" && line.substr(0, 3) != "NOR" && line.substr(0, 3) != "MAX") {
						contLinha++;

						a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico_ilha_solteira).addElemento(AttVetorIntercambioHidraulico_desnivel, contLinha, std::stod(line.substr(0, 3)));
						a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico_tres_irmaos).addElemento(AttVetorIntercambioHidraulico_desnivel, contLinha, std::stod(line.substr(0, 3)));


						for (int i = 1; i <= 12; i++) {
							std::string vazao;
							if (i == 1) { vazao = line.substr(4, 6); }
							if (i != 1) { vazao = line.substr((((i - 1) * 7) + 4), 6); }

							vazao.erase(std::remove(vazao.begin(), vazao.end(), ' '), vazao.end());

							if (vazao.empty()) { vazao = "0"; }

							a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico_ilha_solteira).addElemento(AttMatrizIntercambioHidraulico_vazao, contLinha, i, std::stod(vazao));
							a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico_tres_irmaos).addElemento(AttMatrizIntercambioHidraulico_vazao, contLinha, i, std::stod(vazao));

						}//for (int i = 0; i < 12; i++) {



					}//if (line.substr(0, 3) != "NIV") {

				}//if (lineStr != "") {

			}// while (std::getline(myfile, line)) {

			myfile.close();

		}// if (myfile.is_open()) {

		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo" + a_nomeArquivo + ".");

	} // 	try {

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_DESSEM_16_ILSTRI: \n" + std::string(erro.what())); }

}


void LeituraCEPEL::leitura_COTASR11_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo) {

	try {

		std::ifstream myfile(a_nomeArquivo);
		std::string line;
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());
		const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();
		if (myfile.is_open()) {

			while (std::getline(myfile, line)) {

				strNormalizada(line);

				std::string teste_comentario = line.substr(0, 1);

				if (teste_comentario != "&") {

					IdHor idHoraInicial = getIdHorFromChar(line.substr(3, 2).c_str());
					IdMin idMinInicial = IdMin_0;
					IdDia idDiaInicial = getIdDiaFromChar(line.substr(0, 2).c_str());
					IdMes idMesInicial = periodo_inicial.getMes();
					IdAno idAnoInicial = periodo_inicial.getAno();
					IdDia idDiaInicio_estudo = periodo_inicial.getDia();

					if (line.substr(6, 1) == "1") { idMinInicial = IdMin_30; }
					if (idDiaInicial > idDiaInicio_estudo) { 
						
						if (idMesInicial == IdMes_1) {
							idAnoInicial--;
							idMesInicial = IdMes_12;
						}
						else							
							idMesInicial--;

					}

					Periodo periodo = Periodo(TipoPeriodo_meia_hora, idDiaInicial, idMesInicial, idAnoInicial, idHoraInicial, idMinInicial);

					IdRegua11 idRegua11 = a_dados.vetorRegua11.getMaiorId();

					if (idRegua11 == IdRegua11_Nenhum) {
						idRegua11 = IdRegua11_1;
						Regua11 regua11;
						regua11.setAtributo(AttComumRegua11_idRegua11, idRegua11);
						a_dados.vetorRegua11.add(regua11);
					}//if (idRegua11 == IdRegua11_Nenhum) {

					a_dados.vetorRegua11.att(idRegua11).addElemento(AttVetorRegua11_nivel_dia_anterior, periodo, std::stod(line.substr(16, 10)));

				}//if (teste_comentario != "&") {

			}//while (std::getline(myfile, line)) {

			myfile.close();

		}//if (myfile.is_open()) {

		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo" + a_nomeArquivo + ".");

	}//try {

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_COTASR11_201904_DES16: \n" + std::string(erro.what())); }

}


void LeituraCEPEL::leitura_AREACONT_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo) {
	try {

		std::ifstream myfile(a_nomeArquivo);
		std::string line;


		if (myfile.is_open()) {

			while (std::getline(myfile, line)) {

				strNormalizada(line);
				std::string lineStr = line; lineStr.erase(std::remove(lineStr.begin(), lineStr.end(), ' '), lineStr.end());

				if (lineStr == "AREA") {

					IdReservaPotencia idReservaPotencia = IdReservaPotencia_1;

					while (std::getline(myfile, line)) {

						strNormalizada(line);

						std::string lineStr = line;	lineStr.erase(std::remove(lineStr.begin(), lineStr.end(), ' '), lineStr.end());

						if (lineStr == "FIM") { break; }

						IdArea idArea = IdArea(std::stoi(line.substr(0, 3)));
						std::string nome = line.substr(9, 40);

						ReservaPotencia reservaPotencia;
						reservaPotencia.setAtributo(AttComumReservaPotencia_idReservaPotencia, idReservaPotencia);
						reservaPotencia.setAtributo(AttComumReservaPotencia_nome, nome);
						reservaPotencia.setAtributo(AttComumReservaPotencia_area, idArea);
						a_dados.vetorReservaPotencia.add(reservaPotencia);

						idReservaPotencia++;

					}//while (std::getline(myfile, line))

				}//if (lineStr == "AREA") {

				if (lineStr == "USINA") {
					int i = 1;
					while (std::getline(myfile, line)) {

						strNormalizada(line);

						std::string lineStr = line;	lineStr.erase(std::remove(lineStr.begin(), lineStr.end(), ' '), lineStr.end());

						if (lineStr == "&") { i = 1; }

						if ((lineStr != "&") && (line.substr(0, 1) != "&")) {

							if (lineStr == "FIM") { break; }

							IdArea idArea = IdArea(std::stoi(line.substr(0, 3)));
							std::vector<IdReservaPotencia> idReservaPotencia = a_dados.vetorReservaPotencia.getIdObjetos(AttComumReservaPotencia_area, idArea);

							if (idReservaPotencia.size() > 0) {

								if (line.substr(7, 1) == "H") {

									std::string usina = line.substr(9, 3); usina.erase(std::remove(usina.begin(), usina.end(), ' '), usina.end());
									const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, std::stoi(usina));

									if (idHidreletrica == IdHidreletrica_Nenhum)
										throw std::invalid_argument("Nao inicializada idHidreletrica com codigo_ONS_" + usina);

									a_dados.vetorReservaPotencia.att(idReservaPotencia.at(0)).addElemento(AttVetorReservaPotencia_hidreletrica, i, idHidreletrica);

								}//if (line.substr(7, 1) == "H") {

								if (line.substr(7, 1) == "T") {
									std::string usina = line.substr(9, 3); usina.erase(std::remove(usina.begin(), usina.end(), ' '), usina.end());

									const IdTermeletrica idTermeletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, std::stoi(usina));

									if (idTermeletrica_inicializado == IdTermeletrica_Nenhum)
										throw std::invalid_argument("Nao inicializada idTermeletrica com codigo_ONS_" + usina);

									const IdTermeletrica idTermeletrica = idTermeletrica_inicializado;

									a_dados.vetorReservaPotencia.att(idReservaPotencia.at(0)).addElemento(AttVetorReservaPotencia_termeletrica, i, idTermeletrica);
								}//if (line.substr(7, 1) == "T") {

								if (line.substr(7, 1) == "S") {
									std::string submercado = line.substr(9, 3); submercado.erase(std::remove(submercado.begin(), submercado.end(), ' '), submercado.end());

									IdSubmercado idSubmercado = IdSubmercado_Nenhum;

									if (submercado == "SE") { idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, 1); }
									if (submercado == "S") { idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, 2); }
									if (submercado == "NE") { idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, 3); }
									if (submercado == "N") { idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, 4); }

									a_dados.vetorReservaPotencia.att(idReservaPotencia.at(0)).addElemento(AttVetorReservaPotencia_recebimento_submercado, i, idSubmercado);

								}//if (line.substr(7, 1) == "T") {

								i++;
							} // if (idReservaPotencia.size() > 0) {

							else
								throw std::invalid_argument("Nao instanciado idReservaPotencia ");

						}//if (lineStr != "&") {

					}//while (std::getline(myfile, line))

				}//if (lineStr == "AREA") {

			}//while (std::getline(myfile, line)) {

			myfile.close();

		}//if (myfile.is_open()) {

		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo" + a_nomeArquivo + ".");

	}//try {

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_DESSEM_16_AREACONT: \n" + std::string(erro.what())); }

}


void LeituraCEPEL::leitura_RESPOT_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo) {
	try {

		std::ifstream myfile(a_nomeArquivo);
		std::string line;

		if (myfile.is_open()) {

			LeituraCEPEL leituraCEPEL;
			const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());
			const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();
			const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();

			while (std::getline(myfile, line)) {

				strNormalizada(line);

				if (line.substr(0, 2) == "LM") {

					IdArea idArea = IdArea(std::stoi(line.substr(4, 3)));
					std::vector<IdReservaPotencia> idReservaPotencia = a_dados.vetorReservaPotencia.getIdObjetos(AttComumReservaPotencia_area, idArea);

					Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(17, 2), line.substr(20, 2), line.substr(13, 1));
					Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(9, 2), line.substr(12, 2), line.substr(15, 1));

					std::string reserva = line.substr(25, 10); reserva.erase(std::remove(reserva.begin(), reserva.end(), ' '), reserva.end());

					int sizeReservaMinima = a_dados.vetorReservaPotencia.att(idReservaPotencia.at(0)).getSizeVetor(AttVetorReservaPotencia_reserva_minima);

					if (sizeReservaMinima == 0) {
						for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial, horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_estudo, horizonte_estudo); horizonte_estudo.incrementarIterador(periodo)) {
							a_dados.addDadoAttVetorMembro(getFullString(ReservaPotencia()), getString(idReservaPotencia.at(0)), periodo.str(), "reserva_minima", "0", 1);
						}
					}

					//for (Periodo periodo = periodo_inicial_restricao; periodo <= periodo_final_restricao; horizonte_estudo.incrementarIterador(periodo)) {
					//	a_dados.vetorReservaPotencia.att(idReservaPotencia.at(0)).setElemento(AttVetorReservaPotencia_reserva_minima, periodo.str(), atof(reserva.c_str()));
					//}

				}//if (line.substr(0, 2) == "LM") {

			}//while (std::getline(myfile, line)) {

			myfile.close();

		}//if (myfile.is_open()) {

		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo" + a_nomeArquivo + ".");

	}//try {

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::LeituraCEPEL_DESSEM: \n" + std::string(erro.what())); }

}


void LeituraCEPEL::leitura_PTOPER_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo) {

	try {

		std::ifstream myfile(a_nomeArquivo);
		std::string line;
		LeituraCEPEL leituraCEPEL;
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		if (myfile.is_open()) {

			while (std::getline(myfile, line)) {

				strNormalizada(line);

				std::string registro = line.substr(0, 6); registro.erase(std::remove(registro.begin(), registro.end(), ' '), registro.end());

				if (registro == "PTOPER") {

					std::string mnemonico = line.substr(7, 6);   mnemonico.erase(std::remove(mnemonico.begin(), mnemonico.end(), ' '), mnemonico.end());
					std::string tipo = line.substr(18, 6);  tipo.erase(std::remove(tipo.begin(), tipo.end(), ' '), tipo.end());

					if (mnemonico == "USIT" && tipo == "GERA") {

						const IdTermeletrica idTermeletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, std::stoi(line.substr(14, 3)));

						if (idTermeletrica_inicializado == IdTermeletrica_Nenhum)
							throw std::invalid_argument("Nao inicializada idTermeletrica com codigo_ONS_" + std::stoi(line.substr(14, 3)));

						const IdTermeletrica idTermeletrica = idTermeletrica_inicializado;
						//a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada, 2);
						//if (a_dados.getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_comandada) == 0) {
						//	const SmartEnupla<IdPatamarCarga, double> geracao(IdPatamarCarga_1, std::vector<double>(1, 0));
						//	a_dados.vetorTermeletrica.att(idTermeletrica).setMatriz(AttMatrizTermeletrica_potencia_disponivel_comandada, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo, geracao));
						//}

						Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(25, 2), line.substr(28, 2), line.substr(31, 1));
						Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(33, 2), line.substr(36, 2), line.substr(39, 1));
						//for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, horizonte_estudo); horizonte_estudo.incrementarIterador(periodo)) {
						//	a_dados.vetorTermeletrica.att(idTermeletrica).setElemento(AttMatrizTermeletrica_potencia_disponivel_comandada, periodo, IdPatamarCarga_1, std::stod(line.substr(41, 10)));
						//}

						//throw std::invalid_argument("Atualizar leitura COMANDO Termeletrico");

						//for (Periodo periodo = periodo_inicial_restricao; periodo <= periodo_final_restricao; horizonte_estudo.incrementarIterador(periodo))
							//a_dados.vetorTermeletrica.att(idTermeletrica).addElemento(AttVetorTermeletrica_potencia_disponivel_pre_comandada, periodo, std::stod(line.substr(41, 10)));

					}//if (mnemonico == "USIT" && tipo == "GERA") {

				}//if (registro == "PTOPER") {

			}//while (std::getline(myfile, line)) {

			myfile.close();

		}//if (myfile.is_open()) {

		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo" + a_nomeArquivo + ".");

	}//try {

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_DESSEM_16_PTOPER: \n" + std::string(erro.what())); }

}


void LeituraCEPEL::leitura_RAMPAS_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo) {

	try {

		std::ifstream myfile(a_nomeArquivo);
		std::string line;

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());
		const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();

		if (myfile.is_open()) {

			while (std::getline(myfile, line)) {

				strNormalizada(line);

				std::string lineStr = line;	lineStr.erase(std::remove(lineStr.begin(), lineStr.end(), ' '), lineStr.end());

				if (lineStr == "RAMP") {

					while (std::getline(myfile, line)) {

						strNormalizada(line);

						lineStr = line;	lineStr.erase(std::remove(lineStr.begin(), lineStr.end(), ' '), lineStr.end());

						if (lineStr == "FIM") { break; }

						if (line.substr(0, 1) != "&") {

							const IdTermeletrica idTermeletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, std::stoi(line.substr(0, 3)));

							if (idTermeletrica_inicializado == IdTermeletrica_Nenhum)
								throw std::invalid_argument("Nao inicializada idTermeletrica com codigo_CEPEL_" + std::stoi(line.substr(0, 3)));

							const IdTermeletrica idTermeletrica = idTermeletrica_inicializado;

							std::string nomeUTE = a_dados.vetorTermeletrica.att(idTermeletrica).getAtributo(AttComumTermeletrica_nome, string());

							std::string numeroUnidade = line.substr(4, 3);
							numeroUnidade.erase(std::remove(numeroUnidade.begin(), numeroUnidade.end(), ' '), numeroUnidade.end());

							std::string nomeUnidade = nomeUTE + "_UNIDADE_" + numeroUnidade;

							IdUnidadeUTE idUnidadeUTE = a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.getIdObjetos(AttComumUnidadeUTE_nome, nomeUnidade).at(0);

							int meiaHora = 0;
							if (line.size() > 36) {
								std::string meiaHoraStr = line.substr(37, 1);
								meiaHoraStr.erase(std::remove(meiaHoraStr.begin(), meiaHoraStr.end(), ' '), meiaHoraStr.end());
								if (!meiaHoraStr.empty()) { meiaHora = std::stoi(meiaHoraStr); }
							}//if (line.size() > 36) { 

							int numeroPeriodos = std::stoi(line.substr(31, 5)) * 2 + meiaHora;

							AttVetorUnidadeUTE attVetorUnidadeUTE;

							if (line.substr(17, 1) == "A") { attVetorUnidadeUTE = AttVetorUnidadeUTE_rampa_acionamento; }
							if (line.substr(17, 1) == "D") { attVetorUnidadeUTE = AttVetorUnidadeUTE_rampa_desligamento; }

							int sizeVetor = a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).getSizeVetor(attVetorUnidadeUTE);
							double valorPotencia = std::stod(line.substr(21, 10));

							if (sizeVetor == 0) {

								if (numeroPeriodos == 1) { a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).addElemento(attVetorUnidadeUTE, numeroPeriodos, valorPotencia); }

								if (attVetorUnidadeUTE == AttVetorUnidadeUTE_rampa_acionamento) {
									if (numeroPeriodos > 1) { for (int i = 1; i <= numeroPeriodos; i++) { a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).addElemento(attVetorUnidadeUTE, i, (valorPotencia / numeroPeriodos) * (i + 1)); } }
								}
								else {
									if (numeroPeriodos > 1) { for (int i = 1; i <= numeroPeriodos; i++) { a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).addElemento(attVetorUnidadeUTE, i, valorPotencia - (valorPotencia / numeroPeriodos) * (i)); } }
								}
							}//if (sizeVetor == 0){

							if (sizeVetor > 0) {
								int iteradorFinal = a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).getIteradorFinal(attVetorUnidadeUTE, int());
								if (numeroPeriodos > 1) { for (int i = iteradorFinal + 1; i <= numeroPeriodos; i++) { a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).addElemento(attVetorUnidadeUTE, i, valorPotencia); } }
							}//if (sizeVetor > 0) {	

						}//if (line.substr(0, 1) != "&") {


					}//while (std::getline(myfile, line)) {


				}//if (lineStr == "RAMP") {

			}//while (std::getline(myfile, line)) {

			myfile.close();

		}//if (myfile.is_open()) {

		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo" + a_nomeArquivo + ".");

	}//try {

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_DESSEM_16_RAMPAS: \n" + std::string(erro.what())); }

}


void LeituraCEPEL::leitura_EOLICA_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo) {

	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());
		const Periodo periodo_inicial_estudo = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();

		const Periodo periodo_inicial_estudo_meia = Periodo(TipoPeriodo_meia_hora, periodo_inicial_estudo);
		const Periodo periodo_final_estudo_meia = Periodo(TipoPeriodo_meia_hora, periodo_final_estudo + 1) - 1;

		std::ifstream myfile(a_nomeArquivo);
		std::string line;
		IdUsinaEolica idUsinaEolica;

		if (myfile.is_open()) {

			while (std::getline(myfile, line)) {

				strNormalizada(line);

				try {

					std::istringstream lineStr(line);
					std::string valor;
					std::vector <std::string> dado;

					while (std::getline(lineStr, valor, ';')) {
						valor.erase(std::remove(valor.begin(), valor.end(), ' '), valor.end());
						dado.push_back(valor);
					}//while (std::getline(lineStr, valor, ';')) {

					if (dado.at(0) == "EOLICA") {

						const IdUsinaEolica idUsinaEolica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_usina_eolica, std::stoi(dado.at(1)));

						if (idUsinaEolica_inicializado != IdUsinaEolica_Nenhum) {
							idUsinaEolica = idUsinaEolica_inicializado;
							a_dados.vetorUsinaEolica.att(idUsinaEolica).setAtributoFromString(AttComumUsinaEolica_nome, dado.at(2));
							a_dados.vetorUsinaEolica.att(idUsinaEolica).setAtributoFromString(AttComumUsinaEolica_geracao_maxima, dado.at(3));
							a_dados.vetorUsinaEolica.att(idUsinaEolica).setAtributoFromString(AttComumUsinaEolica_fator_capacidade, dado.at(4));
							a_dados.vetorUsinaEolica.att(idUsinaEolica).setAtributoFromString(AttComumUsinaEolica_constrained_off, dado.at(5));
						}//if (idUsinaEolicaInstanciada.size() != 0) { 

						else {
							idUsinaEolica = a_dados.vetorUsinaEolica.getMaiorId();
							if (idUsinaEolica == IdUsinaEolica_Nenhum) { idUsinaEolica = IdUsinaEolica_1; }
							else { idUsinaEolica++; }

							UsinaEolica usinaEolica;
							usinaEolica.setAtributo(AttComumUsinaEolica_idUsinaEolica, idUsinaEolica);
							usinaEolica.setAtributoFromString(AttComumUsinaEolica_nome, dado.at(2));
							usinaEolica.setAtributoFromString(AttComumUsinaEolica_geracao_maxima, dado.at(3));
							usinaEolica.setAtributoFromString(AttComumUsinaEolica_fator_capacidade, dado.at(4));
							usinaEolica.setAtributoFromString(AttComumUsinaEolica_constrained_off, dado.at(5));
							a_dados.vetorUsinaEolica.add(usinaEolica);

							lista_codigo_ONS_usina_eolica.addElemento(idUsinaEolica, std::atoi(dado.at(1).c_str()));

						}//else {

						//a_dados.vetorUsinaEolica.att(idUsinaEolica).setMatriz(AttMatrizUsinaEolica_geracao, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(IdPatamarCarga_1, 0.0))));

					}//if (dado[0] == "EOLICA") {

					if (dado[0] == "EOLICASUBM") {
						const IdSubmercado idSubmercado = getIdSubmercadoFromMnemonico(dado.at(2));

						const IdUsinaEolica idUsinaEolica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_usina_eolica, std::stoi(dado.at(1)));

						if (idUsinaEolica_inicializado == IdUsinaEolica_Nenhum)
							throw std::invalid_argument("Nao inicializada idUsinaEolica com codigo_CEPEL_" + dado.at(1));

						const IdUsinaEolica  idUsinaEolica = idUsinaEolica_inicializado;

						a_dados.vetorUsinaEolica.att(idUsinaEolica).setAtributo(AttComumUsinaEolica_submercado, idSubmercado);
					}//if (dado[0] == "EOLICASUBM") {


					if (dado[0] == "EOLICABARRA") {

						const IdUsinaEolica idUsinaEolica = getIdFromCodigoONS(lista_codigo_ONS_usina_eolica, std::stoi(dado.at(1)));

						if (idUsinaEolica == IdUsinaEolica_Nenhum)
							throw std::invalid_argument("Nao inicializada idUsinaEolica com codigo_CEPEL_" + dado.at(1));

						a_dados.vetorUsinaEolica.att(idUsinaEolica).setAtributo(AttComumUsinaEolica_barra, std::stoi(dado.at(2)));
					}//if (dado[0] == "EOLICASUBM") {


					if (dado[0] == "EOLICA-GERACAO") {

						Periodo periodo_inicial_geracao;
						if (strCompara(dado.at(2), "i"))
							periodo_inicial_geracao = periodo_inicial_estudo_meia;
						else {

							const IdDia idDiaInicial = getIdDiaFromChar(dado.at(2).c_str());
							IdMes idMesInicial = periodo_inicial_estudo.getMes();
							IdAno idAnoInicial = periodo_inicial_estudo.getAno();
							IdHor idHorInicial = getIdHorFromChar(dado.at(3).c_str());

							if (idDiaInicial < periodo_inicial_estudo.getDia()) {
								if (idMesInicial == IdMes_12) {
									idMesInicial = IdMes_1;
									idAnoInicial++;
								}
								else
									idMesInicial++;
							}

							if (dado.at(4) == "1")
								periodo_inicial_geracao = Periodo(TipoPeriodo_meia_hora, idDiaInicial, idMesInicial, idAnoInicial, idHorInicial, IdMin_30);
							else
								periodo_inicial_geracao = Periodo(TipoPeriodo_meia_hora, idDiaInicial, idMesInicial, idAnoInicial, idHorInicial, IdMin_0);
						}

						Periodo periodo_final_geracao;
						if (strCompara(dado.at(5), "f"))
							periodo_final_geracao = periodo_final_estudo_meia;
						else {

							const IdDia idDiaFinal = getIdDiaFromChar(dado.at(5).c_str());
							IdMes idMesFinal = periodo_inicial_estudo.getMes();
							IdAno idAnoFinal = periodo_inicial_estudo.getAno();
							IdHor idHorFinal = getIdHorFromChar(dado.at(6).c_str());

							if (idDiaFinal < periodo_inicial_estudo.getDia()) {
								if (idMesFinal == IdMes_12) {
									idMesFinal = IdMes_1;
									idAnoFinal++;
								}
								else
									idMesFinal++;
							}

							if (dado.at(7) == "1")
								periodo_final_geracao = Periodo(TipoPeriodo_meia_hora, idDiaFinal, idMesFinal, idAnoFinal, idHorFinal, IdMin_30);
							else
								periodo_final_geracao = Periodo(TipoPeriodo_meia_hora, idDiaFinal, idMesFinal, idAnoFinal, idHorFinal, IdMin_0);
						}

						const IdUsinaEolica idUsinaEolica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_usina_eolica, std::stoi(dado.at(1)));

						if (idUsinaEolica_inicializado == IdUsinaEolica_Nenhum)
							throw std::invalid_argument("Nao inicializada idUsinaEolica com codigo_CEPEL_" + dado.at(1));

						const IdUsinaEolica  idUsinaEolica = idUsinaEolica_inicializado;

						if (a_dados.getSize1Matriz(idUsinaEolica, AttMatrizUsinaEolica_geracao) == 0)
							for (Periodo periodo = periodo_inicial_estudo_meia; periodo < periodo_inicial_geracao; periodo++)
								a_dados.vetorUsinaEolica.att(idUsinaEolica).addElemento(AttMatrizUsinaEolica_geracao, periodo, IdPatamarCarga_1, 0.0);

						const double valor = std::stod(dado.at(8));

						if (idUsinaEolica == IdUsinaEolica_9)
							int aa = 1;

						if (valor == 2)
							int aa = 1;

						for (Periodo periodo = periodo_inicial_geracao; periodo < periodo_final_geracao; periodo++)
							if (periodo <= periodo_final_estudo_meia)
								a_dados.vetorUsinaEolica.att(idUsinaEolica).addElemento(AttMatrizUsinaEolica_geracao, periodo, IdPatamarCarga_1, valor);


					}//if (dado[0] == "EOLICA-GERACAO") {

				}

				catch (const std::exception& erro) { throw std::invalid_argument(line + " \n" + std::string(erro.what())); }

			}//while (std::getline(myfile, line)) {

			myfile.close();

			validarGeracaoMediaRenovaveis(a_dados);

		}//if (myfile.is_open()) {

		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo " + a_nomeArquivo + ".");

	}//try {

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_DESSEM_16_EOLICA: \n" + std::string(erro.what())); }
}

void LeituraCEPEL::validarGeracaoMediaRenovaveis(Dados& a_dados){

	try{

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());
		const Periodo periodo_inicial_estudo = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();

		const Periodo periodo_inicial_estudo_meia = Periodo(TipoPeriodo_meia_hora, periodo_inicial_estudo);
		const Periodo periodo_final_estudo_meia   = Periodo(TipoPeriodo_meia_hora, periodo_final_estudo + 1) - 1;

		const IdUsinaEolica  maiorIdUsinaEolica = a_dados.getMaiorId(IdUsinaEolica());

		for (IdUsinaEolica idUsinaEolica = IdUsinaEolica_1; idUsinaEolica <= maiorIdUsinaEolica; idUsinaEolica++) {

			SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> geracao(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(1, 0.0)));

			if (a_dados.getSize1Matriz(idUsinaEolica, AttMatrizUsinaEolica_geracao) == 0)
				a_dados.vetorUsinaEolica.att(idUsinaEolica).setMatriz_forced(AttMatrizUsinaEolica_geracao, geracao);

			else {
				const Periodo periodo_final_registro = a_dados.getIterador1Final(idUsinaEolica, AttMatrizUsinaEolica_geracao, Periodo());

				if (periodo_final_registro < periodo_final_estudo_meia) {
					for (Periodo periodo = periodo_final_registro + 1; periodo <= periodo_final_estudo_meia; periodo++)
						a_dados.vetorUsinaEolica.att(idUsinaEolica).addElemento(AttMatrizUsinaEolica_geracao, periodo, IdPatamarCarga_1, 0.0);
				}

				for (Periodo periodo_estudo = horizonte_estudo.getIteradorInicial(); periodo_estudo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo_estudo)) {

					const Periodo periodo_estudo_inicial_meia = Periodo(TipoPeriodo_meia_hora, periodo_estudo);
					const Periodo periodo_estudo_final_meia = Periodo(TipoPeriodo_meia_hora, periodo_estudo + 1) - 1;

					double valor = 0.0;

					int divisor = 0;

					for (Periodo periodo_estudo_meia = periodo_estudo_inicial_meia; periodo_estudo_meia <= periodo_estudo_final_meia; periodo_estudo_meia++) {
						valor += a_dados.getElementoMatriz(idUsinaEolica, AttMatrizUsinaEolica_geracao, periodo_estudo_meia, IdPatamarCarga_1, double());
						divisor++;
					}

					valor /= double(divisor);

					geracao.at(periodo_estudo).setElemento(IdPatamarCarga_1, valor);

				}

				a_dados.vetorUsinaEolica.att(idUsinaEolica).setMatriz_forced(AttMatrizUsinaEolica_geracao, geracao);

			}

		} // for (IdUsinaEolica idUsinaEolica = IdUsinaEolica_1; idUsinaEolica <= maiorIdUsinaEolica; idUsinaEolica++) {

	}//try {

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::validarGeracaoMediaRenovaveis: \n" + std::string(erro.what())); }

} // void LeituraCEPEL::calcularGeracaoMediaRenovaveis(){


void LeituraCEPEL::leitura_DADGER_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo, const std::string a_registro, const SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo) {

	LeituraCEPEL leituraCEPEL;
	try {
		std::ifstream myfile(a_nomeArquivo);
		std::string line;

		const Periodo periodo_inicial = a_horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = a_horizonte_estudo.getIteradorFinal();
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		if (myfile.is_open()) {

			while (std::getline(myfile, line)) {

				strNormalizada(line);

				//DISCRETIZACAO DO ESTUDO
				if (lerRegistro("TM", line.substr(0, 2), a_registro)) {

					Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(4, 2), line.substr(9, 2), line.substr(14, 1));

					std::string patamar = line.substr(33, 6); patamar.erase(std::remove(patamar.begin(), patamar.end(), ' '), patamar.end());

					IdDiscretizacao idDiscretizacao = a_dados.vetorDiscretizacao.getMaiorId();

					if (idDiscretizacao == IdDiscretizacao_Nenhum) { idDiscretizacao = IdDiscretizacao_1; }
					else { idDiscretizacao++; }

					Discretizacao discretizacao;
					discretizacao.setAtributo(AttComumDiscretizacao_idDiscretizacao, idDiscretizacao);
					discretizacao.setAtributo(AttComumDiscretizacao_inicio_periodo, periodo_inicial_restricao);
					discretizacao.setAtributo(AttComumDiscretizacao_duracao_periodo, std::stod(line.substr(19, 5)));
					discretizacao.setAtributo(AttComumDiscretizacao_rede, bool(std::stoi(line.substr(29, 1))));

					if (patamar == "LEVE" || patamar == "leve") { discretizacao.setAtributo(AttComumDiscretizacao_patamar, TipoPatamar_leve); }
					if (patamar == "MEDIA" || patamar == "media") { discretizacao.setAtributo(AttComumDiscretizacao_patamar, TipoPatamar_media); }
					if (patamar == "PESADA" || patamar == "pesada") { discretizacao.setAtributo(AttComumDiscretizacao_patamar, TipoPatamar_pesada); }

					a_dados.vetorDiscretizacao.add(discretizacao);

				}//if (line.substr(0, 2) == "TM") {

				//LEITURA DA CONFIGURAÇÃO DAS USINAS TÉRMICAS, BLOCO UT		
				if (lerRegistro("UT", line.substr(0, 2), a_registro)) {

					const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, std::stoi(line.substr(22, 2)));
					const IdTermeletrica idTermeletricaInstanciado = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, std::stoi(line.substr(4, 3)));
					IdTermeletrica idTermeletrica = idTermeletricaInstanciado;

					if (idTermeletricaInstanciado == IdTermeletrica_Nenhum) {

						idTermeletrica = IdTermeletrica(a_dados.vetorTermeletrica.getMaiorId() + 1);

						Termeletrica termeletrica;
						termeletrica.setAtributo(AttComumTermeletrica_idTermeletrica, idTermeletrica);
						termeletrica.setAtributo(AttComumTermeletrica_codigo_usina, std::stoi(line.substr(4, 3)));
						termeletrica.setAtributo(AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica_por_unidade);
						termeletrica.setAtributo(AttComumTermeletrica_representacao_discreta_producao, true);
						termeletrica.setAtributo(AttComumTermeletrica_considerar_usina, true);;
						termeletrica.setAtributo(AttComumTermeletrica_submercado, idSubmercado);
						lista_codigo_ONS_termeletrica.addElemento(idTermeletrica, std::stoi(line.substr(4, 3)));
						a_dados.vetorTermeletrica.add(termeletrica);

						const SmartEnupla<IdPatamarCarga, double> geracao(IdPatamarCarga_1, std::vector<double>(1, 0));
						a_dados.vetorTermeletrica.att(idTermeletrica).setMatriz(AttMatrizTermeletrica_potencia_minima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, geracao));
						a_dados.vetorTermeletrica.att(idTermeletrica).setMatriz(AttMatrizTermeletrica_potencia_maxima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, geracao));
					} // if (idTermeletricaInstanciado == IdTermeletrica_Nenhum) {

					else if (termeletricasPreConfig_instanciadas) {
						a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica_por_unidade);
						a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_representacao_discreta_producao, true);
						a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_considerar_usina, true);
						a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_submercado, idSubmercado);
					}



					const Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(27, 2), line.substr(30, 2), line.substr(33, 1));
					const Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(35, 2), line.substr(38, 2), line.substr(41, 1));

					if (int(std::stoi(line.substr(25, 1))) == 2) {

						if (a_dados.getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_minima) == 0) {
							const SmartEnupla<IdPatamarCarga, double> geracao(IdPatamarCarga_1, std::vector<double>(1, 0));
							a_dados.vetorTermeletrica.att(idTermeletrica).setMatriz(AttMatrizTermeletrica_potencia_minima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, geracao));
						}

						if (a_dados.getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_maxima) == 0) {
							const SmartEnupla<IdPatamarCarga, double> geracao(IdPatamarCarga_1, std::vector<double>(1, 0));
							a_dados.vetorTermeletrica.att(idTermeletrica).setMatriz(AttMatrizTermeletrica_potencia_maxima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, geracao));
						}

						std::string geracaoMaxima = std::string(line.substr(57, 10));
						std::string geracaoMinima = std::string(line.substr(47, 10));
						geracaoMaxima.erase(std::remove(geracaoMaxima.begin(), geracaoMaxima.end(), ' '), geracaoMaxima.end());
						geracaoMinima.erase(std::remove(geracaoMinima.begin(), geracaoMinima.end(), ' '), geracaoMinima.end());

						if (!geracaoMinima.empty())
							for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, horizonte_estudo); horizonte_estudo.incrementarIterador(periodo))
								a_dados.vetorTermeletrica.att(idTermeletrica).setElemento(AttMatrizTermeletrica_potencia_minima, periodo, IdPatamarCarga_1, atof(geracaoMinima.c_str()));

						if (!geracaoMaxima.empty())
							for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, horizonte_estudo); horizonte_estudo.incrementarIterador(periodo))
								a_dados.vetorTermeletrica.att(idTermeletrica).setElemento(AttMatrizTermeletrica_potencia_maxima, periodo, IdPatamarCarga_1, atof(geracaoMaxima.c_str()));

					}// if (int(std::stoi(line.substr(25, 1))) == 2) {

				} //if (line.substr(0, 2) == REGISTRO_USINAS_TERMICAS) {

				//LEITURA DO BLOCO DAS USINAS HIDRELETRICAS, BLOCO UH
				if (lerRegistro("UH", line.substr(0, 2), a_registro)) {

					IdReservatorio idReservatorio = IdReservatorio_1;
					Reservatorio reservatorio;

					a_dados.volume_inicial_carregado_from_premissa = true;
					reservatorio.setAtributo(AttComumReservatorio_idReservatorio, idReservatorio);
					reservatorio.setAtributo(AttComumReservatorio_percentual_volume_util_inicial, atof(line.substr(29, 10).c_str()) / 100);
					Periodo entradaOperacao;
					const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, std::stoi(line.substr(4, 3)));

					if (line.size() > 46) {
						entradaOperacao = getPeriodoInicial(a_dados, line.substr(41, 2), line.substr(44, 2), line.substr(47, 1));
						for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial, horizonte_estudo); periodo < getPeriodoFinalResticao(entradaOperacao, horizonte_estudo); horizonte_estudo.incrementarIterador(periodo))
							a_dados.vetorHidreletrica.att(idHidreletrica).setElemento(AttVetorHidreletrica_disponibilidade, periodo, 0.0);
					} // if (line.size() > 46) {

					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.add(reservatorio);
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(idReservatorio).setVetor(AttVetorReservatorio_taxa_enchimento_volume_morto, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0));
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(idReservatorio).setVetor(AttVetorReservatorio_enchendo_volume_morto, SmartEnupla<Periodo, bool>(a_horizonte_estudo, false));
					a_dados.vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_vazao_defluente_minima, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0));

					if (line.size() > 48) {
						std::string volumeMorto = line.substr(49, 10);	volumeMorto.erase(std::remove(volumeMorto.begin(), volumeMorto.end(), ' '), volumeMorto.end());
						if (volumeMorto != "") {
							for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial, horizonte_estudo); periodo < getPeriodoFinalResticao(entradaOperacao, horizonte_estudo); horizonte_estudo.incrementarIterador(periodo))
								a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(idReservatorio).setElemento(AttVetorReservatorio_enchendo_volume_morto, periodo, true);
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(idReservatorio).setAtributo(AttComumReservatorio_enchendo_volume_morto, true);
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(idReservatorio).setAtributo(AttComumReservatorio_volume_morto, std::stoi(volumeMorto));
						}
					}// if (line.size() > 49) {	

					//if (line.size() > 63) { if (line.substr(64, 1) == "1") { a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_tipo_fph, TipoFuncaoProducaoHidreletrica_produtibilidade); } }

				}// if (line.substr(0, 2) == "UH") {

				 //LEITURA DO TEMPO DE VIAGEM DA ÁGUA
				if (lerRegistro("TVIAG", line.substr(0, 5), a_registro)) {

					const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, std::stoi(line.substr(6, 3)));
					const IdHidreletrica idHidreletricaJusante = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, std::stoi(line.substr(10, 3)));

					a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_jusante, idHidreletricaJusante);
					a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_tempo_viagem_agua, std::stoi(line.substr(19, 3)));

					if (line.substr(14, 1) == "H") { a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_tipo_elemento_jusante, TipoElementoJusante_usina); }
					if (line.substr(14, 1) == "S") { a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_tipo_elemento_jusante, TipoElementoJusante_secao_rio); }
					if (line.size() > 23) { 
						
						if (line.substr(24, 1) == "2") { 
						//throw std::invalid_argument("Tempo de viagem da água por propagação, necessario tratar o arquivo CURVTVIAG.DAT"); 
						} 
					}
				}//if (lerRegistro("TVIAG", line.substr(0, 2), a_registro)) {

				// LEITURA DOS SUBSISTEMAS 
				if (lerRegistro("SIST", line.substr(0, 4), a_registro)) {
					std::string mnemonico = line.substr(10, 2);	mnemonico.erase(std::remove(mnemonico.begin(), mnemonico.end(), ' '), mnemonico.end());

					IdSubmercado idSubmercado = a_dados.vetorSubmercado.getMaiorId();
					if (idSubmercado == IdSubmercado_Nenhum) { idSubmercado = IdSubmercado_1; }
					else { idSubmercado++; }

					lista_codigo_ONS_submercado.addElemento(idSubmercado, std::stoi(line.substr(7, 2)));
					lista_submercado_mnemonico.addElemento(idSubmercado, mnemonico);

					Submercado submercado;
					submercado.setAtributo(AttComumSubmercado_idSubmercado, idSubmercado);
					submercado.setAtributo(AttComumSubmercado_nome, line.substr(16, 10));
					submercado.setAtributo(AttComumSubmercado_ficticio, bool(std::stoi(line.substr(14, 1))));
					a_dados.vetorSubmercado.add(submercado);

					UsinaNaoSimulada usinaNaoSimulada;
					usinaNaoSimulada.setAtributo(AttComumUsinaNaoSimulada_idUsinaNaoSimulada, IdUsinaNaoSimulada_1);
					a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.add(usinaNaoSimulada);

					a_dados.vetorSubmercado.att(idSubmercado).setVetor(AttVetorSubmercado_demanda, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0));

					const SmartEnupla<IdPatamarCarga, double> valor(IdPatamarCarga_1, std::vector<double>(1, 1));

					a_dados.setMatriz(AttMatrizDados_percentual_duracao_patamar_carga, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, valor));

					const SmartEnupla<IdPatamarCarga, double> geracao(IdPatamarCarga_1, std::vector<double>(1, 0));

					a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(IdUsinaNaoSimulada_1).setMatriz(AttMatrizUsinaNaoSimulada_potencia_maxima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, geracao));
					a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(IdUsinaNaoSimulada_1).setMatriz(AttMatrizUsinaNaoSimulada_potencia_minima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, geracao));

					if (idSubmercado == IdSubmercado_5) {

						// CRIA NÓ IVAIPORÃ
						if (true) {
							idSubmercado++;
							Submercado submercado;
							submercado.setAtributo(AttComumSubmercado_idSubmercado, idSubmercado);
							submercado.setAtributo(AttComumSubmercado_nome, std::string("IVAIPORA"));
							submercado.setAtributo(AttComumSubmercado_ficticio, true);
							a_dados.vetorSubmercado.add(submercado);

							UsinaNaoSimulada usinaNaoSimulada;
							usinaNaoSimulada.setAtributo(AttComumUsinaNaoSimulada_idUsinaNaoSimulada, IdUsinaNaoSimulada_1);
							a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.add(usinaNaoSimulada);

							a_dados.vetorSubmercado.att(idSubmercado).setVetor(AttVetorSubmercado_demanda, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0));

							const SmartEnupla<IdPatamarCarga, double> valor(IdPatamarCarga_1, std::vector<double>(1, 1));

							a_dados.setMatriz(AttMatrizDados_percentual_duracao_patamar_carga, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, valor));

							const SmartEnupla<IdPatamarCarga, double> geracao(IdPatamarCarga_1, std::vector<double>(1, 0));

							a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(IdUsinaNaoSimulada_1).setMatriz(AttMatrizUsinaNaoSimulada_potencia_maxima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, geracao));
							a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(IdUsinaNaoSimulada_1).setMatriz(AttMatrizUsinaNaoSimulada_potencia_minima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, geracao));

							lista_codigo_ONS_submercado.addElemento(idSubmercado, codigo_IV);
							lista_submercado_mnemonico.addElemento(idSubmercado, std::string("IV"));
						}



						// CRIA SUBMERCADO ANDE (ITAIPU 50 HZ CARGA PARAGUAI)
						if (true) {
							idSubmercado++;
							Submercado submercado;
							submercado.setAtributo(AttComumSubmercado_idSubmercado, idSubmercado);
							submercado.setAtributo(AttComumSubmercado_nome, std::string("ANDE"));
							submercado.setAtributo(AttComumSubmercado_ficticio, true);
							a_dados.vetorSubmercado.add(submercado);

							UsinaNaoSimulada usinaNaoSimulada;
							usinaNaoSimulada.setAtributo(AttComumUsinaNaoSimulada_idUsinaNaoSimulada, IdUsinaNaoSimulada_1);
							a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.add(usinaNaoSimulada);

							a_dados.vetorSubmercado.att(idSubmercado).setVetor(AttVetorSubmercado_demanda, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0));

							const SmartEnupla<IdPatamarCarga, double> geracao(IdPatamarCarga_1, std::vector<double>(1, 0));

							a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(IdUsinaNaoSimulada_1).setMatriz(AttMatrizUsinaNaoSimulada_potencia_maxima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, geracao));
							a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(IdUsinaNaoSimulada_1).setMatriz(AttMatrizUsinaNaoSimulada_potencia_minima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, geracao));

							lista_codigo_ONS_submercado.addElemento(idSubmercado, codigo_ANDE);
							lista_submercado_mnemonico.addElemento(idSubmercado, std::string("ANDE"));

							// CRIA INTERCAMBIO ENTRE ANDE E SE
							const IdIntercambio idIntercambio = IdIntercambio(a_dados.getMaiorId(IdIntercambio()) + 1);

							Intercambio intercambio;

							intercambio.setAtributo(AttComumIntercambio_idIntercambio, idIntercambio);

							a_dados.vetorIntercambio.add(intercambio);

							const IdSubmercado idSubmercado_origem = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_ANDE);
							const IdSubmercado idSubmercado_destino = getIdSubmercadoFromMnemonico("SE");

							const std::string nome_submercado_origem = a_dados.getAtributo(idSubmercado_origem, AttComumSubmercado_nome, std::string());
							const std::string nome_submercado_destino = a_dados.getAtributo(idSubmercado_destino, AttComumSubmercado_nome, std::string());

							a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_submercado_origem, idSubmercado_origem);
							a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_submercado_destino, idSubmercado_destino);

							a_dados.vetorIntercambio.att(idIntercambio).setAtributo(AttComumIntercambio_nome, nome_submercado_origem + "->" + nome_submercado_destino);

							for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo)) {

								const IdPatamarCarga maiorIdPatamarCarga = a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
									a_dados.vetorIntercambio.att(idIntercambio).addElemento(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, 0.0);
									a_dados.vetorIntercambio.att(idIntercambio).addElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, 99999);
								}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

							}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {	

							a_dados.vetorSubmercado.att(idSubmercado).vetorPatamarDeficit = a_dados.vetorSubmercado.att(idSubmercado_destino).vetorPatamarDeficit;
						}
					}
				}//if (lerRegistro("SIST", line.substr(0, 2), a_registro)) {

				// LEITURA DA DEMANDA
				if (lerRegistro("DP", line.substr(0, 2), a_registro)) {

					LeituraCEPEL leituraCEPEL;
					const Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(8, 2), line.substr(11, 2), line.substr(14, 1));
					const Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(16, 2), line.substr(19, 2), line.substr(22, 1));

					const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, std::stoi(line.substr(4, 2)));
					for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, horizonte_estudo); horizonte_estudo.incrementarIterador(periodo)) {
						a_dados.vetorSubmercado.att(idSubmercado).setElemento(AttVetorSubmercado_demanda, periodo, std::stod(line.substr(24, 10)));
					}

				}//if (lerRegistro("DP", line.substr(0, 2), a_registro)) {

				// LEITURA DA GERAÇÃO DAS PEQUENAS USINAS (NÃO SIMULADAS) E ABATIDO DA DEMANDA BRUTA
				if (lerRegistro("PQ", line.substr(0, 2), a_registro)) {

					LeituraCEPEL leituraCEPEL;
					Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(24, 2), line.substr(27, 2), line.substr(30, 1));
					Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(32, 2), line.substr(35, 2), line.substr(38, 1));

					const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, std::stoi(line.substr(19, 5)));

					for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, horizonte_estudo); horizonte_estudo.incrementarIterador(periodo)) {
						double geracao = a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(IdUsinaNaoSimulada_1).getElementoMatriz(AttMatrizUsinaNaoSimulada_potencia_maxima, periodo, IdPatamarCarga_1, double());;
						a_dados.vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(IdUsinaNaoSimulada_1).setElemento(AttMatrizUsinaNaoSimulada_potencia_maxima, periodo, IdPatamarCarga_1, (geracao + std::stod(line.substr(40, 10))));
						//a_dados.vetorSubmercado.att(idSubmercado).setElemento(AttVetorSubmercado_demanda_liquida, periodo, (demanda - std::stod(line.substr(40, 10))) );
					}

				}//if (lerRegistro("PQ", line.substr(0, 2), a_registro))

				//INTERCAMBIOS DE ENERGIA ENTRE SUBSISTEMAS 	
				if (lerRegistro("IA", line.substr(0, 2), a_registro)) {

					IdIntercambio idIntercambio = a_dados.vetorIntercambio.getMaiorId();
					const Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(13, 2), line.substr(16, 2), line.substr(19, 1));
					const Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(21, 2), line.substr(24, 2), line.substr(27, 1));

					std::string ss1 = line.substr(4, 2);	ss1.erase(std::remove(ss1.begin(), ss1.end(), ' '), ss1.end());
					std::string ss2 = line.substr(9, 2);	ss2.erase(std::remove(ss2.begin(), ss2.end(), ' '), ss2.end());
					std::string nomeOrigemDestino = ss1 + " -> " + ss2;
					std::string nomeDestinoOrigem = ss2 + " -> " + ss1;

					std::vector<IdIntercambio> idIntercambioOrigemDestino = a_dados.vetorIntercambio.getIdObjetos(AttComumIntercambio_nome, nomeOrigemDestino);
					std::vector<IdIntercambio> idIntercambioDestinoOrigem = a_dados.vetorIntercambio.getIdObjetos(AttComumIntercambio_nome, nomeDestinoOrigem);
					const IdSubmercado IdSubmercadoOrigem = getIdSubmercadoFromMnemonico(ss1);
					const IdSubmercado IdSubmercadoDestino = getIdSubmercadoFromMnemonico(ss2);

					if (idIntercambioOrigemDestino.size() == 0) {

						if (idIntercambio == IdIntercambio_Nenhum) { idIntercambio = IdIntercambio_1; }
						else idIntercambio++;

						Intercambio intercambio;
						intercambio.setAtributo(AttComumIntercambio_idIntercambio, idIntercambio);
						intercambio.setAtributo(AttComumIntercambio_nome, nomeOrigemDestino);
						intercambio.setAtributo(AttComumIntercambio_submercado_origem, IdSubmercadoOrigem);
						intercambio.setAtributo(AttComumIntercambio_submercado_destino, IdSubmercadoDestino);
						a_dados.vetorIntercambio.add(intercambio);
						a_dados.vetorIntercambio.att(idIntercambio).setVetor(AttVetorIntercambio_potencia_maxima, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0));

						idIntercambioOrigemDestino = a_dados.vetorIntercambio.getIdObjetos(AttComumIntercambio_nome, nomeOrigemDestino);
					}

					if (idIntercambioDestinoOrigem.size() == 0) {

						if (idIntercambio == IdIntercambio_Nenhum) { idIntercambio = IdIntercambio_1; }
						else idIntercambio++;

						Intercambio intercambio;
						intercambio.setAtributo(AttComumIntercambio_idIntercambio, idIntercambio);
						intercambio.setAtributo(AttComumIntercambio_nome, nomeDestinoOrigem);
						intercambio.setAtributo(AttComumIntercambio_submercado_origem, IdSubmercadoDestino);
						intercambio.setAtributo(AttComumIntercambio_submercado_destino, IdSubmercadoOrigem);
						a_dados.vetorIntercambio.add(intercambio);
						a_dados.vetorIntercambio.att(idIntercambio).setVetor(AttVetorIntercambio_potencia_maxima, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0));

						idIntercambioDestinoOrigem = a_dados.vetorIntercambio.getIdObjetos(AttComumIntercambio_nome, nomeDestinoOrigem);
					}

					for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, horizonte_estudo); horizonte_estudo.incrementarIterador(periodo)) {
						a_dados.vetorIntercambio.att(idIntercambioOrigemDestino.at(0)).setElemento(AttVetorIntercambio_potencia_maxima, periodo, std::stod(line.substr(29, 10)));
						a_dados.vetorIntercambio.att(idIntercambioDestinoOrigem.at(0)).setElemento(AttVetorIntercambio_potencia_maxima, periodo, std::stod(line.substr(39, 10)));
					}

				}//if (line.substr(0, 2) == "IA") {	

				//USINAS ELEVATORIAS 
				if (lerRegistro("USIE", line.substr(0, 4), a_registro)) {

					IdHidreletrica idHidreletricaOrigem = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, std::stoi(line.substr(34, 3)));
					IdHidreletrica idHidreletricaDestino = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, std::stoi(line.substr(29, 3)));

					if (idHidreletricaOrigem == IdHidreletrica_Nenhum) {
						IdHidreletrica idHidreletrica = a_dados.vetorHidreletrica.getMaiorId();
						idHidreletrica++;
						Hidreletrica hidreletrica;
						hidreletrica.setAtributo(AttComumHidreletrica_idHidreletrica, idHidreletrica);
						lista_codigo_ONS_hidreletrica.addElemento(idHidreletrica, std::stoi(line.substr(34, 3)));
						a_dados.vetorHidreletrica.add(hidreletrica);
						idHidreletricaOrigem = idHidreletrica;
					} // if (idHidreletricaOrigem == IdHidreletrica_Nenhum) {

					if (idHidreletricaDestino == IdHidreletrica_Nenhum) {
						IdHidreletrica idHidreletrica = a_dados.vetorHidreletrica.getMaiorId();
						idHidreletrica++;
						Hidreletrica hidreletrica;
						hidreletrica.setAtributo(AttComumHidreletrica_idHidreletrica, idHidreletrica);
						lista_codigo_ONS_hidreletrica.addElemento(idHidreletrica, std::stoi(line.substr(29, 3)));
						a_dados.vetorHidreletrica.add(hidreletrica);
						idHidreletricaDestino = idHidreletrica;
					} // if (idHidreletricaDestino == IdHidreletrica_Nenhum) {

					const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, std::stoi(line.substr(9, 2)));

					IdUsinaElevatoria idUsinaElevatoria = a_dados.vetorUsinaElevatoria.getMaiorId();

					if (idUsinaElevatoria == IdUsinaElevatoria_Nenhum) { idUsinaElevatoria = IdUsinaElevatoria_1; }
					else { idUsinaElevatoria++; }

					lista_codigo_ONS_usina_elevatoria.addElemento(idUsinaElevatoria, std::stoi(line.substr(9, 2)));

					UsinaElevatoria usinaElevatoria;
					usinaElevatoria.setAtributo(AttComumUsinaElevatoria_idUsinaElevatoria, idUsinaElevatoria);
					usinaElevatoria.setAtributo(AttComumUsinaElevatoria_submercado, idSubmercado);
					usinaElevatoria.setAtributo(AttComumUsinaElevatoria_nome, line.substr(14, 12));
					usinaElevatoria.setAtributo(AttComumUsinaElevatoria_usina_destino_bombeamento, idHidreletricaDestino);
					usinaElevatoria.setAtributo(AttComumUsinaElevatoria_usina_origem_bombeamento, idHidreletricaOrigem);
					usinaElevatoria.setAtributo(AttComumUsinaElevatoria_vazao_bombeada_minima, std::stod(line.substr(39, 10)));
					usinaElevatoria.setAtributo(AttComumUsinaElevatoria_vazao_bombeada_maxima, std::stod(line.substr(49, 10)));
					usinaElevatoria.setAtributo(AttComumUsinaElevatoria_taxa_de_consumo, std::stod(line.substr(59, 10)));
					a_dados.vetorUsinaElevatoria.add(usinaElevatoria);
					a_dados.vetorUsinaElevatoria.att(idUsinaElevatoria).setVetor(AttVetorUsinaElevatoria_fator_disponibilidade, SmartEnupla<Periodo, double>(a_horizonte_estudo, 1));

				}//if (lerRegistro("USIE", line.substr(0, 2), a_registro)){

				//CUSTO DE DEFICIT   
				if (lerRegistro("CD", line.substr(0, 2), a_registro)) {

					const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, std::stoi(line.substr(3, 2)));

					const Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(9, 2), line.substr(12, 2), line.substr(15, 1));
					const Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(17, 2), line.substr(20, 2), line.substr(23, 1));
					std::vector<IdPatamarDeficit> idPatamarDeficit = a_dados.vetorSubmercado.att(idSubmercado).vetorPatamarDeficit.getIdObjetos(AttComumPatamarDeficit_percentual, std::stod(line.substr(35, 10)) / 100);

					if (idPatamarDeficit.size() == 0) {
						IdPatamarDeficit maiorIdPatamarDeficit = a_dados.vetorSubmercado.att(idSubmercado).vetorPatamarDeficit.getMaiorId();

						if (maiorIdPatamarDeficit == IdPatamarDeficit_Nenhum) { maiorIdPatamarDeficit = IdPatamarDeficit_1; }
						else { maiorIdPatamarDeficit++; }

						PatamarDeficit patamarDeficit;
						patamarDeficit.setAtributo(AttComumPatamarDeficit_idPatamarDeficit, maiorIdPatamarDeficit);
						patamarDeficit.setAtributo(AttComumPatamarDeficit_percentual, std::stod(line.substr(35, 10)) / 100);
						a_dados.vetorSubmercado.att(idSubmercado).vetorPatamarDeficit.add(patamarDeficit);
						a_dados.vetorSubmercado.att(idSubmercado).vetorPatamarDeficit.att(maiorIdPatamarDeficit).setVetor(AttVetorPatamarDeficit_custo, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0));

					}//if (idPatamarDeficit.size() == 0) {

					idPatamarDeficit = a_dados.vetorSubmercado.att(idSubmercado).vetorPatamarDeficit.getIdObjetos(AttComumPatamarDeficit_percentual, std::stod(line.substr(35, 10)) / 100);

					for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, horizonte_estudo); horizonte_estudo.incrementarIterador(periodo)) {
						a_dados.vetorSubmercado.att(idSubmercado).vetorPatamarDeficit.att(idPatamarDeficit.at(0)).setElemento(AttVetorPatamarDeficit_custo, periodo, std::stod(line.substr(25, 10)));
					}

				}//if (lerRegistro("CD", line.substr(0, 2), a_registro)){

				 // RESTRICOES DE VARIACAO DO NIVEL DA REGUA 11
				if (lerRegistro("R11", line.substr(0, 2), a_registro)) {

					const Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(4, 2), line.substr(7, 2), line.substr(10, 1));
					const Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(12, 2), line.substr(15, 2), line.substr(18, 2));

					IdRegua11 idRegua11 = a_dados.vetorRegua11.getMaiorId();

					if (idRegua11 == IdRegua11_Nenhum) {
						idRegua11 = IdRegua11_1;
						Regua11 regua11;
						regua11.setAtributo(AttComumRegua11_idRegua11, idRegua11);

						a_dados.vetorRegua11.add(regua11);

					}//if (idRegua11 == IdRegua11_Nenhum) {

					a_dados.vetorRegua11.att(idRegua11).setAtributo(AttComumRegua11_coeficiente_0, std::stod(line.substr(59, 15)));
					a_dados.vetorRegua11.att(idRegua11).setAtributo(AttComumRegua11_coeficiente_1, std::stod(line.substr(74, 15)));
					a_dados.vetorRegua11.att(idRegua11).setAtributo(AttComumRegua11_coeficiente_2, std::stod(line.substr(89, 15)));
					a_dados.vetorRegua11.att(idRegua11).setAtributo(AttComumRegua11_coeficiente_3, std::stod(line.substr(104, 15)));
					a_dados.vetorRegua11.att(idRegua11).setAtributo(AttComumRegua11_coeficiente_4, std::stod(line.substr(119, 15)));
					a_dados.vetorRegua11.att(idRegua11).setAtributo(AttComumRegua11_coeficiente_5, std::stod(line.substr(134, 15)));
					a_dados.vetorRegua11.att(idRegua11).setAtributo(AttComumRegua11_coeficiente_6, std::stod(line.substr(149, 15)));

					if(a_dados.getSizeVetor(idRegua11, AttVetorRegua11_variacao_maxima_horaria) == 0)
						a_dados.vetorRegua11.att(idRegua11).setVetor(AttVetorRegua11_variacao_maxima_horaria, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0));
					
					if (a_dados.getSizeVetor(idRegua11, AttVetorRegua11_variacao_maxima_diaria) == 0)
						a_dados.vetorRegua11.att(idRegua11).setVetor(AttVetorRegua11_variacao_maxima_diaria, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0));

					std::string variacaoMaximaHoraria = line.substr(30, 10); variacaoMaximaHoraria.erase(std::remove(variacaoMaximaHoraria.begin(), variacaoMaximaHoraria.end(), ' '), variacaoMaximaHoraria.end());
					std::string variacaoMaximaDiaria = line.substr(40, 10); variacaoMaximaDiaria.erase(std::remove(variacaoMaximaDiaria.begin(), variacaoMaximaDiaria.end(), ' '), variacaoMaximaDiaria.end());

					if (!variacaoMaximaHoraria.empty()) {
						for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, horizonte_estudo); horizonte_estudo.incrementarIterador(periodo)) {
							a_dados.vetorRegua11.att(idRegua11).setElemento(AttVetorRegua11_variacao_maxima_horaria, periodo, std::stod(variacaoMaximaDiaria));
						}
					}//if (!variacaoMaximaHoraria.empty()) {

					if (!variacaoMaximaDiaria.empty()) {
						for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, horizonte_estudo); horizonte_estudo.incrementarIterador(periodo)) {
							a_dados.vetorRegua11.att(idRegua11).setElemento(AttVetorRegua11_variacao_maxima_diaria, periodo, std::stod(variacaoMaximaDiaria));
						}
					}//if (!variacaoMaximaDiaria.empty()) {

				}//if (lerRegistro("R11", line.substr(0, 2), a_registro)){


			}// while (std::getline(myfile, line))

			myfile.close();

		} // if (myfile.is_open()) 

		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo" + a_nomeArquivo + ".");

	} //try 

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_DADGER_201904_DES16: \n" + std::string(erro.what())); }
}


void LeituraCEPEL::leitura_OPERUH_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo, const std::string a_registro, const SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo) {

	try {

		std::ifstream myfile(a_nomeArquivo);
		std::string line;
		IdRestricaoOperativaUHE idRestricaoOperativaUHE;
		std::vector<IdRestricaoOperativaUHE> idRestricao;
		LeituraCEPEL leituraCEPEL;

		if (myfile.is_open()) {

			while (std::getline(myfile, line)) {

				strNormalizada(line);

				if (line.substr(0, 1) != "&") {

					std::string mnemonico = line.substr(0, 6);
					std::string mnemonicoTipo = line.substr(7, 6);
					mnemonicoTipo.erase(std::remove(mnemonicoTipo.begin(), mnemonicoTipo.end(), ' '), mnemonicoTipo.end());
					mnemonico.erase(std::remove(mnemonico.begin(), mnemonico.end(), ' '), mnemonico.end());

					if (mnemonico == "OPERUH" && mnemonicoTipo == "REST" && line.substr(21, 1) == "V") {

						IdRestricaoOperativaUHE maiorIdRestricaoOperativaUHE = a_dados.vetorRestricaoOperativaUHE.getMaiorId();
						if (maiorIdRestricaoOperativaUHE == IdRestricaoOperativaUHE_Nenhum) { idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; }
						else { idRestricaoOperativaUHE = IdRestricaoOperativaUHE(int(maiorIdRestricaoOperativaUHE) + 1); }

						RestricaoOperativaUHE restricaoOperativaUHE;
						restricaoOperativaUHE.setAtributo(AttComumRestricaoOperativaUHE_idRestricaoOperativaUHE, idRestricaoOperativaUHE);
						//restricaoOperativaUHE.setAtributo(AttComumRestricaoOperativaUHE_valor_inicial, std::stod(line.substr(40, 10)));
						restricaoOperativaUHE.setAtributo(AttComumRestricaoOperativaUHE_num_restricao, std::stoi(line.substr(14, 5)));
						a_dados.vetorRestricaoOperativaUHE.add(restricaoOperativaUHE);

					}//	if (mnemonico == "OPERUH" && mnemonicoTipo == "REST" && line.substr(7, 6) == "V") {

					if (mnemonico == "OPERUH" && mnemonicoTipo == "ELEM") {

						std::vector<IdRestricaoOperativaUHE> idRestricao = a_dados.vetorRestricaoOperativaUHE.getIdObjetos(AttComumRestricaoOperativaUHE_num_restricao, std::stoi(line.substr(14, 5)));

						if (idRestricao.size() == 0) {

							IdRestricaoOperativaUHE maiorIdRestricaoOperativaUHE = a_dados.vetorRestricaoOperativaUHE.getMaiorId();
							if (maiorIdRestricaoOperativaUHE == IdRestricaoOperativaUHE_Nenhum) { idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; }
							else { idRestricaoOperativaUHE = IdRestricaoOperativaUHE(int(maiorIdRestricaoOperativaUHE) + 1); }

							RestricaoOperativaUHE restricaoOperativaUHE;
							restricaoOperativaUHE.setAtributo(AttComumRestricaoOperativaUHE_idRestricaoOperativaUHE, idRestricaoOperativaUHE);
							restricaoOperativaUHE.setAtributo(AttComumRestricaoOperativaUHE_num_restricao, std::stoi(line.substr(14, 5)));
							a_dados.vetorRestricaoOperativaUHE.add(restricaoOperativaUHE);
						}

						IdRestricaoOperativaUHE idRestricaoInstanciada = a_dados.vetorRestricaoOperativaUHE.getIdObjetos(AttComumRestricaoOperativaUHE_num_restricao, std::stoi(line.substr(14, 5))).at(0);

						const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, std::stoi(line.substr(20, 3)));

						///////////////////////////////////////////////////////////////////////////////////////////////////

						IdElementoSistema idElementoSistema = a_dados.getMaiorId(idRestricaoInstanciada, IdElementoSistema());

						if (idElementoSistema == IdElementoSistema_Nenhum) { idElementoSistema = IdElementoSistema_1; }
						else { idElementoSistema++; }

						ElementoSistema elementoSistema;
						elementoSistema.setAtributo(AttComumElementoSistema_idElementoSistema, idElementoSistema);
						elementoSistema.setAtributo(AttComumElementoSistema_tipo_elemento, TipoElementoSistema_hidreletrica);
						elementoSistema.setAtributo(AttComumElementoSistema_hidreletrica, idHidreletrica);

						std::string tipo = line.substr(40, 2);
						tipo.erase(std::remove(tipo.begin(), tipo.end(), ' '), tipo.end());

						switch (int(std::stoi(tipo))) {
						case 1: { elementoSistema.setAtributo(AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa_volume_reservatorio);   elementoSistema.setAtributo(AttComumElementoSistema_fator_participacao, std::stod(line.substr(43, 5))); break; }
						case 2: { elementoSistema.setAtributo(AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa_volume_util);           elementoSistema.setAtributo(AttComumElementoSistema_fator_participacao, std::stod(line.substr(43, 5))); break; }
						case 3: { elementoSistema.setAtributo(AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa_vazao_turbinada);       elementoSistema.setAtributo(AttComumElementoSistema_fator_participacao, std::stod(line.substr(43, 5))); break; }
						case 4: { elementoSistema.setAtributo(AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa_vazao_vertida);         elementoSistema.setAtributo(AttComumElementoSistema_fator_participacao, std::stod(line.substr(43, 5))); break; }
						case 5: { elementoSistema.setAtributo(AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa_vazao_desviada);        elementoSistema.setAtributo(AttComumElementoSistema_fator_participacao, std::stod(line.substr(43, 5))); break; }
						case 6: { elementoSistema.setAtributo(AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa_vazao_defluente);       elementoSistema.setAtributo(AttComumElementoSistema_fator_participacao, std::stod(line.substr(43, 5)));	break; }
						case 7: { elementoSistema.setAtributo(AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa_producao_hidreletrica); elementoSistema.setAtributo(AttComumElementoSistema_fator_participacao, std::stod(line.substr(43, 5))); break; }
						case 8: { elementoSistema.setAtributo(AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa_vazao_bombeada);        elementoSistema.setAtributo(AttComumElementoSistema_fator_participacao, std::stod(line.substr(43, 5))); break; }
						case 9: { elementoSistema.setAtributo(AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa_vazao_afluente);        elementoSistema.setAtributo(AttComumElementoSistema_fator_participacao, std::stod(line.substr(43, 5))); break; }
						}//switch (std::stoi(line.substr(40, 2))) {

						a_dados.vetorRestricaoOperativaUHE.att(idRestricaoInstanciada).vetorElementoSistema.add(elementoSistema);

					}//if (mnemonico == "OPERUH" && mnemonicoTipo == "ELEM") {

					if (mnemonico == "OPERUH" && mnemonicoTipo == "LIM") {

						idRestricao = a_dados.vetorRestricaoOperativaUHE.getIdObjetos(AttComumRestricaoOperativaUHE_num_restricao, std::stoi(line.substr(14, 5)));

						//Valores default
						double limiteInferior = 0.0;
						double limiteSuperior = getdoubleFromChar("max");

						std::string limiteSuperior_str = "";
						if (int(line.size()) > 47) { limiteSuperior_str = line.substr(48, 10); }

						std::string limiteInferior_str = line.substr(38, 10);
						
						limiteInferior_str.erase(std::remove(limiteInferior_str.begin(), limiteInferior_str.end(), ' '), limiteInferior_str.end());
						limiteSuperior_str.erase(std::remove(limiteSuperior_str.begin(), limiteSuperior_str.end(), ' '), limiteSuperior_str.end());

						if (limiteInferior_str != "") { limiteInferior = std::stod(limiteInferior_str); }
						if (limiteSuperior_str != "") { limiteSuperior = std::stod(limiteSuperior_str); }

						if (std::stoi(line.substr(14, 5)) == 2 && limiteSuperior > 100) { limiteSuperior = 100; }

						a_dados.vetorRestricaoOperativaUHE.att(idRestricao.at(0)).setAtributo(AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica_limite);
						
						if(a_dados.getSizeMatriz(idRestricao.at(0), AttMatrizRestricaoOperativaUHE_limite_inferior)==0)
							a_dados.vetorRestricaoOperativaUHE.att(idRestricao.at(0)).setMatriz(AttMatrizRestricaoOperativaUHE_limite_inferior, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(IdPatamarCarga_1, 0.0))));

						if (a_dados.getSizeMatriz(idRestricao.at(0), AttMatrizRestricaoOperativaUHE_limite_superior) == 0)
							a_dados.vetorRestricaoOperativaUHE.att(idRestricao.at(0)).setMatriz(AttMatrizRestricaoOperativaUHE_limite_superior, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(IdPatamarCarga_1, getdoubleFromChar("max")))));

						const Periodo periodo_inicial = getPeriodoInicial(a_dados, line.substr(19, 2), line.substr(23, 2), line.substr(27, 1));
						const Periodo periodo_final = getPeriodoFinal(a_dados, line.substr(28, 2), line.substr(31, 2), line.substr(35, 1));
						
						for (Periodo periodo = periodo_inicial; periodo <= periodo_final; a_horizonte_estudo.incrementarIterador(periodo)) {
							a_dados.vetorRestricaoOperativaUHE.att(idRestricao.at(0)).setElemento(AttMatrizRestricaoOperativaUHE_limite_inferior, periodo, IdPatamarCarga_1, limiteInferior);
							a_dados.vetorRestricaoOperativaUHE.att(idRestricao.at(0)).setElemento(AttMatrizRestricaoOperativaUHE_limite_superior, periodo, IdPatamarCarga_1, limiteSuperior);
						}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; a_horizonte_estudo.incrementarIterador(periodo)) {

					}// if (mnemonico == "OPERUH" && mnemonicoTipo == "LIM" && numeroRest == std::stoi(line.substr(14, 5))) {

					if (mnemonico == "OPERUH" && mnemonicoTipo == "VAR") {

						LeituraCEPEL leituraCEPEL;
						idRestricao = a_dados.vetorRestricaoOperativaUHE.getIdObjetos(AttComumRestricaoOperativaUHE_num_restricao, std::stoi(line.substr(14, 5)));

						std::string rampa_min_percentual = line.substr(37, 10);
						std::string rampa_max_percentual = line.substr(47, 10);
						std::string rampa_min_absoluta = line.substr(57, 10);
						std::string rampa_max_absoluta = "";

						if (int(line.size()) > 66) { rampa_max_absoluta = line.substr(67, 10); }
						rampa_min_percentual.erase(std::remove(rampa_min_percentual.begin(), rampa_min_percentual.end(), ' '), rampa_min_percentual.end());
						rampa_max_percentual.erase(std::remove(rampa_max_percentual.begin(), rampa_max_percentual.end(), ' '), rampa_max_percentual.end());
						rampa_min_absoluta.erase(std::remove(rampa_min_absoluta.begin(), rampa_min_absoluta.end(), ' '), rampa_min_absoluta.end());
						rampa_max_absoluta.erase(std::remove(rampa_max_absoluta.begin(), rampa_max_absoluta.end(), ' '), rampa_max_absoluta.end());

						
						if((rampa_min_percentual.size() > 0 && rampa_min_absoluta.size() > 0) || (rampa_min_percentual.size() > 0 && rampa_max_absoluta.size() > 0) || (rampa_max_percentual.size() > 0 && rampa_min_absoluta.size() > 0) || (rampa_max_percentual.size() > 0 && rampa_max_absoluta.size() > 0))
							throw std::invalid_argument("Registros OPERUH-VAR com variacao absoluta e relativa simultaneamente");

						double limiteInferior = 0.0;
						double limiteSuperior = getdoubleFromChar("max");

						TipoRestricaoHidraulica tipoRestricaoHidraulica = TipoRestricaoHidraulica_Nenhum;

						if (rampa_min_percentual.size() > 0) { limiteInferior = std::stod(rampa_min_percentual); tipoRestricaoHidraulica = TipoRestricaoHidraulica_variacao_percentual; }
						if (rampa_max_percentual.size() > 0) { limiteSuperior = std::stod(rampa_max_percentual); tipoRestricaoHidraulica = TipoRestricaoHidraulica_variacao_percentual; }
						if (rampa_min_absoluta.size() > 0) { limiteInferior = std::stod(rampa_min_absoluta); tipoRestricaoHidraulica = TipoRestricaoHidraulica_variacao_absoluta; }
						if (rampa_max_absoluta.size() > 0) { limiteSuperior = std::stod(rampa_max_absoluta); tipoRestricaoHidraulica = TipoRestricaoHidraulica_variacao_absoluta; }


						if (a_dados.getSizeMatriz(idRestricao.at(0), AttMatrizRestricaoOperativaUHE_limite_inferior) == 0)
							a_dados.vetorRestricaoOperativaUHE.att(idRestricao.at(0)).setMatriz(AttMatrizRestricaoOperativaUHE_limite_inferior, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(IdPatamarCarga_1, 0.0))));
						else {

							if(tipoRestricaoHidraulica != a_dados.vetorRestricaoOperativaUHE.att(idRestricao.at(0)).getAtributo(AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()))
								throw std::invalid_argument("Registros OPERUH-VAR com variacao absoluta e relativa simultaneamente");
						}

						if (a_dados.getSizeMatriz(idRestricao.at(0), AttMatrizRestricaoOperativaUHE_limite_superior) == 0)
							a_dados.vetorRestricaoOperativaUHE.att(idRestricao.at(0)).setMatriz(AttMatrizRestricaoOperativaUHE_limite_superior, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(IdPatamarCarga_1, getdoubleFromChar("max")))));
						else {

							if (tipoRestricaoHidraulica != a_dados.vetorRestricaoOperativaUHE.att(idRestricao.at(0)).getAtributo(AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica()))
								throw std::invalid_argument("Registros OPERUH-VAR com variacao absoluta e relativa simultaneamente");
						}

						a_dados.vetorRestricaoOperativaUHE.att(idRestricao.at(0)).setAtributo(AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, tipoRestricaoHidraulica);
						
						const Periodo periodo_inicial = getPeriodoInicial(a_dados, line.substr(19, 2), line.substr(22, 2), line.substr(25, 1));
						const Periodo periodo_final = getPeriodoFinal(a_dados, line.substr(27, 2), line.substr(30, 2), line.substr(34, 1));

						for (Periodo periodo = periodo_inicial; periodo <= periodo_final; a_horizonte_estudo.incrementarIterador(periodo)) {
							a_dados.vetorRestricaoOperativaUHE.att(idRestricao.at(0)).setElemento(AttMatrizRestricaoOperativaUHE_limite_inferior, periodo, IdPatamarCarga_1, limiteInferior);
							a_dados.vetorRestricaoOperativaUHE.att(idRestricao.at(0)).setElemento(AttMatrizRestricaoOperativaUHE_limite_superior, periodo, IdPatamarCarga_1, limiteSuperior);
						}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; a_horizonte_estudo.incrementarIterador(periodo)) {

						
					}// if (mnemonico == "OPERUH" && mnemonicoTipo == "LIM" && numeroRest == std::stoi(line.substr(14, 5))) {

				}//if (line != "&")

			}// while (std::getline(myfile, line)) {

			myfile.close();

		}// if (myfile.is_open()) {

		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo" + a_nomeArquivo + ".");

	}// try

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_OPERUH_201904_DES16: \n" + std::string(erro.what())); }

}


void LeituraCEPEL::leitura_CADTERM_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo, const std::string a_registro, const SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo) {

	try {



		std::ifstream myfile(a_nomeArquivo);
		std::string line;

		if (myfile.is_open()) {

			while (std::getline(myfile, line)) {

				strNormalizada(line);

				if (lerRegistro("CADUSIT", line.substr(0, 7), a_registro)) {
					try {
						const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, std::stoi(line.substr(25, 2)));
						const IdTermeletrica idTermeletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, std::stoi(line.substr(8, 3)));
						IdTermeletrica idTermeletrica = idTermeletrica_inicializado;

						if (idTermeletrica_inicializado == IdTermeletrica_Nenhum) {
							idTermeletrica = IdTermeletrica(a_dados.vetorTermeletrica.getMaiorId() + 1);

							Termeletrica termeletrica;
							termeletrica.setAtributo(AttComumTermeletrica_idTermeletrica, idTermeletrica);
							termeletrica.setAtributo(AttComumTermeletrica_codigo_usina, std::stoi(line.substr(8, 3)));
							termeletrica.setAtributo(AttComumTermeletrica_representacao_discreta_producao, true);
							termeletrica.setAtributo(AttComumTermeletrica_considerar_usina, true);
							termeletrica.setAtributo(AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica_por_unidade);
							a_dados.vetorTermeletrica.add(termeletrica);

							lista_codigo_ONS_termeletrica.addElemento(idTermeletrica, std::stoi(line.substr(8, 3)));

							idTermeletrica = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, std::stoi(line.substr(8, 3)));

						}// if (idTermeletrica_inicializado == IdTermeletrica_Nenhum) {
						else if (termeletricasPreConfig_instanciadas) {
							a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica_por_unidade);
							a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_representacao_discreta_producao, true);
							a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_considerar_usina, true);
							a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_submercado, idSubmercado);
						}

						std::string nomeUTE = line.substr(12, 12);
						nomeUTE.erase(std::find_if(nomeUTE.rbegin(), nomeUTE.rend(), std::bind1st(std::not_equal_to<char>(), ' ')).base(), nomeUTE.end());

						a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_nome, nomeUTE);
						a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_submercado, idSubmercado);
						a_dados.vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_entrada_operacao, Periodo(std::stoi(line.substr(36, 2)), std::stoi(line.substr(33, 2)), std::stoi(line.substr(28, 4)), std::stoi(line.substr(39, 2)), std::stoi(line.substr(42, 1)) * 30));

						if (a_dados.getSizeVetor(idTermeletrica, AttVetorTermeletrica_indisponibilidade_forcada) == 0)
							a_dados.vetorTermeletrica.att(idTermeletrica).setVetor(AttVetorTermeletrica_indisponibilidade_forcada, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0));

						if (a_dados.getSizeVetor(idTermeletrica, AttVetorTermeletrica_indisponibilidade_programada) == 0)
							a_dados.vetorTermeletrica.att(idTermeletrica).setVetor(AttVetorTermeletrica_indisponibilidade_programada, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0));

						if (a_dados.getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_minima) == 0) {
							const SmartEnupla<IdPatamarCarga, double> geracao(IdPatamarCarga_1, std::vector<double>(1, 0));
							a_dados.vetorTermeletrica.att(idTermeletrica).setMatriz(AttMatrizTermeletrica_potencia_minima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, geracao));
							a_dados.vetorTermeletrica.att(idTermeletrica).setMatriz(AttMatrizTermeletrica_potencia_maxima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, geracao));
						}
					}
					catch (const std::exception& erro) { throw std::invalid_argument("lerRegistro(CADUSIT linha ( " + line + ") ): \n" + std::string(erro.what())); }

				} // if (line.substr(0, 1) != "&" && line.substr(0, 7) == "CADUSIT") {


				if (lerRegistro("CADUNIDT", line.substr(0, 8), a_registro)) {
					try {
						const Periodo entradaOperacao = Periodo(std::stoi(line.substr(24, 2)), std::stoi(line.substr(21, 2)), std::stoi(line.substr(16, 4)), std::stoi(line.substr(27, 2)), std::stoi(line.substr(30, 1)) * 30).str();

						const IdTermeletrica idTermeletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, std::stoi(line.substr(9, 3)));

						if (idTermeletrica_inicializado == IdTermeletrica_Nenhum) { throw std::invalid_argument("Nao inicializada idTermeletrica com codigo_ONS_" + std::stoi(line.substr(9, 3))); }

						const IdTermeletrica idTermeletrica = idTermeletrica_inicializado;

						const IdUnidadeUTE  maiorIdUnidadeUTE = a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.getMaiorId();

						std::string nomeUTE = a_dados.vetorTermeletrica.att(idTermeletrica).getAtributo(AttComumTermeletrica_nome, string());

						std::string unidadeEquivalente = "0";

						std::string numeroUnidade = line.substr(12, 3);
						numeroUnidade.erase(std::remove(numeroUnidade.begin(), numeroUnidade.end(), ' '), numeroUnidade.end());

						std::string nomeUnidade = nomeUTE + "_UNIDADE_" + numeroUnidade;

						if (line.size() > 126) {
							unidadeEquivalente = line.substr(127, 3);
							unidadeEquivalente.erase(std::remove(unidadeEquivalente.begin(), unidadeEquivalente.end(), ' '), unidadeEquivalente.end());

							if (unidadeEquivalente == "") { unidadeEquivalente = "0"; }
						} // if (line.size() > 126) {

						IdUnidadeUTE  idUnidadeUTE = IdUnidadeUTE(int(maiorIdUnidadeUTE) + 1);

						const IdSubmercado idSubmercado = a_dados.vetorTermeletrica.att(idTermeletrica).getAtributo(AttComumTermeletrica_submercado, IdSubmercado());

						UnidadeUTE unidadeUTE;
						unidadeUTE.setAtributo(AttComumUnidadeUTE_idUnidadeUTE, idUnidadeUTE);
						unidadeUTE.setAtributo(AttComumUnidadeUTE_submercado, idSubmercado);
						unidadeUTE.setAtributo(AttComumUnidadeUTE_nome, nomeUnidade);
						unidadeUTE.setAtributo(AttComumUnidadeUTE_t_on, std::stoi(line.substr(55, 5)));
						unidadeUTE.setAtributo(AttComumUnidadeUTE_t_of, std::stoi(line.substr(61, 5)));
						unidadeUTE.setAtributo(AttComumUnidadeUTE_potencia_minima, std::stod(line.substr(44, 10)));
						unidadeUTE.setAtributo(AttComumUnidadeUTE_potencia_maxima, std::stod(line.substr(33, 10)));
						unidadeUTE.setAtributo(AttComumUnidadeUTE_unidade_equivalente, bool(std::stoi(unidadeEquivalente)));

						a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.add(unidadeUTE);

						const SmartEnupla<IdPatamarCarga, double> potenciaZero(IdPatamarCarga_1, std::vector<double>(1, 0.0));
						const SmartEnupla<IdPatamarCarga, double> potenciaMinima(IdPatamarCarga_1, std::vector<double>(1, std::stod(line.substr(44, 10))));
						const SmartEnupla<IdPatamarCarga, double> potenciaMaxima(IdPatamarCarga_1, std::vector<double>(1, std::stod(line.substr(33, 10))));

						a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).setVetor(AttVetorUnidadeUTE_disponibilidade, SmartEnupla<Periodo, double>(a_horizonte_estudo, 1));

						a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).setMatriz(AttMatrizUnidadeUTE_potencia_minima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, potenciaMinima));
						a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).setMatriz(AttMatrizUnidadeUTE_potencia_maxima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, potenciaMaxima));
						/*	a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).setMatriz(AttMatrizUnidadeUTE_potencia_disponivel_minima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> (a_horizonte_estudo, potenciaMinima));
							a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).setMatriz(AttMatrizUnidadeUTE_potencia_disponivel_maxima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> (a_horizonte_estudo, potenciaMaxima));
						*/
						if ((entradaOperacao >= a_horizonte_estudo.getIteradorInicial()) && (entradaOperacao <= a_horizonte_estudo.getIteradorFinal())) {

							a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).setVetor(AttVetorUnidadeUTE_disponibilidade, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0));

							for (Periodo periodo = getPeriodoInicialResticao(entradaOperacao, a_horizonte_estudo); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo)) {
								a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).setElemento(AttVetorUnidadeUTE_disponibilidade, periodo, 1.0);
							}

						} // if (entradaOperacao > a_horizonte_estudo.getIteradorInicial()) {
					}
					catch (const std::exception& erro) { throw std::invalid_argument("lerRegistro(CADUNIDT, linha ( " + line + ") ): \n" + std::string(erro.what())); }

				} //if (line.at(0) != "&" && line.substr(0, 8) == "CADUNIDT")

				if (lerRegistro("CADCONF", line.substr(0, 7), a_registro)) {
					try {
						const IdTermeletrica idTermeletrica = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, std::stoi(line.substr(8, 3)));

						std::string nomeUTE = a_dados.vetorTermeletrica.att(idTermeletrica).getAtributo(AttComumTermeletrica_nome, string());

						std::string numeroUnidade = line.substr(12, 3);

						numeroUnidade.erase(std::remove(numeroUnidade.begin(), numeroUnidade.end(), ' '), numeroUnidade.end());

						std::string nomeUnidade = nomeUTE + "_UNIDADE_" + numeroUnidade;

						const IdUnidadeUTE idUnidadeUTE = a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.getIdObjetos(AttComumUnidadeUTE_nome, nomeUnidade).at(0);

						const IdUnidadeRealUTE idUnidadeRealUTE = IdUnidadeRealUTE(std::stoi(line.substr(16, 3)));

						const int size_unidades_reais = a_dados.getSizeVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_unidades_reais);

						a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).addElemento(AttVetorUnidadeUTE_unidades_reais, size_unidades_reais, idUnidadeRealUTE);
					}
					catch (const std::exception& erro) { throw std::invalid_argument("lerRegistro(CADCONF, linha ( " + line + ") ): \n" + std::string(erro.what())); }

				} // if (line.substr(0, 1) != "&" && line.substr(0, 7) == "CADCONF") {


				if (lerRegistro("CADMIN", line.substr(0, 7), a_registro)) {
					try {
						const IdTermeletrica idTermeletrica = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, std::stoi(line.substr(8, 3)));

						std::string nomeUTE = a_dados.vetorTermeletrica.att(idTermeletrica).getAtributo(AttComumTermeletrica_nome, string());

						std::string numeroUnidade = line.substr(12, 3);

						numeroUnidade.erase(std::remove(numeroUnidade.begin(), numeroUnidade.end(), ' '), numeroUnidade.end());

						std::string nomeUnidade = nomeUTE + "_UNIDADE_" + numeroUnidade;

						const IdUnidadeUTE idUnidadeUTE = a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.getIdObjetos(AttComumUnidadeUTE_nome, nomeUnidade).at(0);

						const int numero_minimo_unidades = std::stoi(line.substr(13, 3));

						a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).setAtributo(AttComumUnidadeUTE_numero_minimo_unidades_reais, numero_minimo_unidades);
					}
					catch (const std::exception& erro) { throw std::invalid_argument("lerRegistro(CADMIN, linha ( " + line + ") ): \n" + std::string(erro.what())); }

				} // if (line.substr(0, 1) != "&" && line.substr(0, 7) == "CADCONF") {


			}// while (std::getline(myfile, line))

			myfile.close();

		} // if (myfile.is_open()) 

		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo" + a_nomeArquivo + ".");

	} //try 

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_DESSEM_16_CADTERM: \n" + std::string(erro.what())); }

} //void LeituraCEPEL::leitura_DESSEM_16_termdat_dat(Dados & a_dados)


void LeituraCEPEL::leitura_OPERUT_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo, const std::string a_registro, const SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo) {

	try {
		std::ifstream myfile(a_nomeArquivo);
		std::string line;

		if (myfile.is_open()) {
			while (std::getline(myfile, line)) {
				strNormalizada(line);

				try {

					// LEITURA DO BLOCO INIT
					if (lerRegistro("INIT", line.substr(0, 4), a_registro)) {

						while (std::getline(myfile, line)) {
							strNormalizada(line);

							if (line.substr(0, 3) == "FIM") { break; }

							if (line.substr(0, 1) != "&") {

								if (std::stoi(line.substr(0, 3)) == 170)
									int aa = 1;

								const IdTermeletrica idTermeletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, std::stoi(line.substr(0, 3)));

								if (idTermeletrica_inicializado == IdTermeletrica_Nenhum)
									throw std::invalid_argument("Nao inicializada idTermeletrica com codigo_ONS_" + std::stoi(line.substr(0, 3)));

								const IdTermeletrica idTermeletrica = idTermeletrica_inicializado;

								std::string nomeUTE = a_dados.vetorTermeletrica.att(idTermeletrica).getAtributo(AttComumTermeletrica_nome, string());

								std::string numeroUnidade = line.substr(18, 3);
								numeroUnidade.erase(std::remove(numeroUnidade.begin(), numeroUnidade.end(), ' '), numeroUnidade.end());

								std::string nomeUnidade = nomeUTE + "_UNIDADE_" + numeroUnidade;

								IdUnidadeUTE idUnidadeUTE = a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.getIdObjetos(AttComumUnidadeUTE_nome, nomeUnidade).at(0);

								a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).setAtributo(AttComumUnidadeUTE_geracao_inicial, std::stod(line.substr(29, 10)));
								a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).setAtributo(AttComumUnidadeUTE_horas_neste_status, std::stoi(line.substr(41, 5)));
								a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).setAtributo(AttComumUnidadeUTE_min_neste_status, std::stoi(line.substr(48, 1)) * 30);

								switch (std::stoi(line.substr(51, 1))) {
								case 1:	a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).setAtributo(AttComumUnidadeUTE_status, TipoStatusUnidadeUTE_ligando); break;
								case 2:	a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).setAtributo(AttComumUnidadeUTE_status, TipoStatusUnidadeUTE_desligando); break;
								}

								switch (std::stoi(line.substr(24, 2))) {
								case 0:	a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).setAtributo(AttComumUnidadeUTE_status, TipoStatusUnidadeUTE_desligada); break;
								case 1:	a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).setAtributo(AttComumUnidadeUTE_status, TipoStatusUnidadeUTE_ligada); break;
								}

							} //if (line.at(0) != COMENTARIO)

						} //while (std::getline(myfile, line) || line.substr(0, 3)!= "FIM")

					} //if (lerRegistro("INIT", line.substr(0, 4), a_registro)) {

					// LEITURA DO BLOCO OPER
					if (lerRegistro("OPER", line.substr(0, 4), a_registro)) {

						while (std::getline(myfile, line)) {

							strNormalizada(line);

							if (line.substr(0, 3) == "FIM") { break; }

							if (line.substr(0, 1) != "&") {

								LeituraCEPEL leituraCEPEL;
								Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(20, 2), line.substr(23, 2), line.substr(26, 1));
								Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(28, 2), line.substr(31, 2), line.substr(34, 1));

								std::string geracaoMaxima = std::string(line.substr(46, 10));
								std::string geracaoMinima = std::string(line.substr(36, 10));

								geracaoMaxima.erase(std::remove(geracaoMaxima.begin(), geracaoMaxima.end(), ' '), geracaoMaxima.end());
								geracaoMinima.erase(std::remove(geracaoMinima.begin(), geracaoMinima.end(), ' '), geracaoMinima.end());

								const IdTermeletrica idTermeletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, std::stoi(line.substr(0, 3)));

								if (idTermeletrica_inicializado == IdTermeletrica_Nenhum)
									throw std::invalid_argument("Nao inicializada idTermeletrica com codigo_ONS_" + line.substr(0, 3));

								const IdTermeletrica idTermeletrica = idTermeletrica_inicializado;

								std::string nomeUTE = a_dados.vetorTermeletrica.att(idTermeletrica).getAtributo(AttComumTermeletrica_nome, string());

								std::string numeroUnidade = line.substr(16, 3);
								numeroUnidade.erase(std::remove(numeroUnidade.begin(), numeroUnidade.end(), ' '), numeroUnidade.end());

								std::string nomeUnidade = nomeUTE + "_UNIDADE_" + numeroUnidade;

								IdUnidadeUTE idUnidadeUTE = a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.getIdObjetos(AttComumUnidadeUTE_nome, nomeUnidade).at(0);

								if (!geracaoMinima.empty()) {
									for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo)) {
										if (a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_minima, periodo, IdPatamarCarga_1, double()) < std::stod(geracaoMinima))
											a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).setElemento(AttMatrizUnidadeUTE_potencia_minima, periodo, IdPatamarCarga_1, atof(geracaoMinima.c_str()));
									}
								} // (!geracaoMinima.empty()) 

								if (!geracaoMaxima.empty()) {
									for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo)) {
										if (a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima, periodo, IdPatamarCarga_1, double()) > std::stod(geracaoMaxima))
											a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).setElemento(AttMatrizUnidadeUTE_potencia_maxima, periodo, IdPatamarCarga_1, atof(geracaoMaxima.c_str()));
										if (a_dados.getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_minima, periodo, IdPatamarCarga_1, double()) > std::stod(geracaoMaxima))
											a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).setElemento(AttMatrizUnidadeUTE_potencia_minima, periodo, IdPatamarCarga_1, atof(geracaoMaxima.c_str()));

									}
								}// if (!geracaoMaxima.empty())

								if (a_dados.getSize1Matriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_custo_de_operacao) == 0)
									a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).setMatriz(AttMatrizUnidadeUTE_custo_de_operacao, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(IdPatamarCarga_1, 0.0))));

								for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo)) {
									a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).setElemento(AttMatrizUnidadeUTE_custo_de_operacao, periodo, IdPatamarCarga_1, std::stod(line.substr(56, 10)));
								}

							} //if (line.at(0) != COMENTARIO)

						} //while (std::getline(myfile, line) || line.substr(0, 3)!= "FIM")

					} //if (lerRegistro("OPER", line.substr(0, 4), a_registro)) {

				}

				catch (const std::exception& erro) { throw std::invalid_argument(line + " \n" + std::string(erro.what())); }

			} //while (std::getline(myfile, line))

			myfile.close();

		} //if (myfile.is_open())

		else { throw std::invalid_argument("Nao foi possivel abrir o arquivo" + a_nomeArquivo + "."); }

	} // try

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_DESSEM_16_OPERUT: \n" + std::string(erro.what())); }

} // void LeituraCEPEL::leitura_OPERUT_201904_DES16(Dados & a_dados, const std::string a_nomeArquivo, const std::string a_registro, const SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo) {


void LeituraCEPEL::leitura_REE_201904_DES16(Dados& a_dados, const std::string a_nomeArquivo, const std::string a_registro, const SmartEnupla<Periodo, IdEstagio> a_horizonte_estudo) {

	try {
		std::ifstream myfile(a_nomeArquivo);
		std::string line;

		const Periodo periodo_inicial = a_horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = a_horizonte_estudo.getIteradorFinal();
		LeituraCEPEL        leituraCEPEL;
		IdContrato          idContrato = IdContrato_1;
		IdElementoSistema   idElementoSistema = IdElementoSistema_1;

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		if (myfile.is_open()) {

			while (std::getline(myfile, line)) {
				//std::cout << line << std::endl;
				strNormalizada(line);

				if (line.substr(0, 2) == "FP") {
					const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, std::stoi(line.substr(3, 3)));
					if (a_dados.getMaiorId(idHidreletrica, IdFuncaoProducaoHidreletrica()) == IdFuncaoProducaoHidreletrica_Nenhum)
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.add(FuncaoProducaoHidreletrica());
					a_dados.vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).setAtributo(AttComumFuncaoProducaoHidreletrica_percentual_volume, double(std::stod(line.substr(29, 10)) / 100.0));
				}//if (line.substr(9, 6) == "NUMJUS") {

				//Coeficientes do Canal de Fuga de Itaipu - Regua 11
				if (lerRegistro("IT", line.substr(0, 3), a_registro)) {

					const IdHidreletrica idHidreletrica_itaipu = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, 66);
					const IdHidreletrica idHidreletrica_SaltoCaxias = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, 82);

					IdRegua11 idRegua11 = a_dados.vetorRegua11.getMaiorId();

					if (idRegua11 == IdRegua11_Nenhum) {
						idRegua11 = IdRegua11_1;
						Regua11 regua11;
						regua11.setAtributo(AttComumRegua11_idRegua11, idRegua11);
						
						a_dados.vetorRegua11.add(regua11);

					}//if (idRegua11 == IdRegua11_Nenhum) {

					a_dados.vetorRegua11.att(idRegua11).setAtributo(AttComumRegua11_coeficiente_0, std::stod(line.substr(9, 15)));
					a_dados.vetorRegua11.att(idRegua11).setAtributo(AttComumRegua11_coeficiente_1, std::stod(line.substr(24, 15)));
					a_dados.vetorRegua11.att(idRegua11).setAtributo(AttComumRegua11_coeficiente_2, std::stod(line.substr(39, 15)));
					a_dados.vetorRegua11.att(idRegua11).setAtributo(AttComumRegua11_coeficiente_3, std::stod(line.substr(54, 15)));
					a_dados.vetorRegua11.att(idRegua11).setAtributo(AttComumRegua11_coeficiente_4, std::stod(line.substr(69, 15)));
					a_dados.vetorRegua11.att(idRegua11).addElemento(AttVetorRegua11_hidreletrica, 1, idHidreletrica_itaipu);
					a_dados.vetorRegua11.att(idRegua11).addElemento(AttVetorRegua11_hidreletrica, 2, idHidreletrica_SaltoCaxias);

				}//if (line.substr(0, 2) == "IT") {

				//RESTRICAO DE ITAIPU 50HZ E 60HZ E PARCELA DA ANDE  
				if (lerRegistro("RI", line.substr(0, 2), a_registro) && (line.substr(0, 5) != "RIVAR")) {

					const Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(8, 2), line.substr(11, 2), line.substr(14, 1));
					const Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(16, 2), line.substr(19, 2), line.substr(22, 1));

					const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, 66);

					if (a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

						const IdSubmercado idSubmercado_ande = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_ANDE);

						SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_zero(a_horizonte_estudo, SmartEnupla<IdPatamarCarga, double>());

						for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo))
							matriz_zero.setElemento(periodo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()), 0.0)));

						if (a_dados.getSize1Matriz(idHidreletrica, IdConjuntoHidraulico_1, AttMatrizConjuntoHidraulico_potencia_minima) == 0)
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico_1).setMatriz(AttMatrizConjuntoHidraulico_potencia_minima, matriz_zero);

						if (a_dados.getSize1Matriz(idHidreletrica, IdConjuntoHidraulico_1, AttMatrizConjuntoHidraulico_potencia_maxima) == 0)
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico_1).setMatriz(AttMatrizConjuntoHidraulico_potencia_maxima, matriz_zero);

						if (a_dados.getSize1Matriz(idHidreletrica, IdConjuntoHidraulico_2, AttMatrizConjuntoHidraulico_potencia_minima) == 0)
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico_2).setMatriz(AttMatrizConjuntoHidraulico_potencia_minima, matriz_zero);

						if (a_dados.getSize1Matriz(idHidreletrica, IdConjuntoHidraulico_2, AttMatrizConjuntoHidraulico_potencia_maxima) == 0)
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico_2).setMatriz(AttMatrizConjuntoHidraulico_potencia_maxima, matriz_zero);

						if (a_dados.getSizeVetor(idSubmercado_ande, AttVetorSubmercado_demanda) == 0)
							a_dados.vetorSubmercado.att(idSubmercado_ande).setVetor(AttVetorSubmercado_demanda, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0.0));

						for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo)) {

							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {
								a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico_1).setElemento(AttMatrizConjuntoHidraulico_potencia_minima, periodo, idPatamarCarga, std::stod(line.substr(26, 10)));
								a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico_1).setElemento(AttMatrizConjuntoHidraulico_potencia_maxima, periodo, idPatamarCarga, std::stod(line.substr(36, 10)));
								a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico_2).setElemento(AttMatrizConjuntoHidraulico_potencia_minima, periodo, idPatamarCarga, std::stod(line.substr(46, 10)));
								a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico_2).setElemento(AttMatrizConjuntoHidraulico_potencia_maxima, periodo, idPatamarCarga, std::stod(line.substr(56, 10)));
							}

							a_dados.vetorSubmercado.att(idSubmercado_ande).setElemento(AttVetorSubmercado_demanda, periodo, std::stod(line.substr(66, 10)));
						} // for (Periodo periodo = periodo_inicial_restricao; periodo <= periodo_final_restricao; horizonte_estudo.incrementarIterador(periodo)) {


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

						SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_zero(a_horizonte_estudo, SmartEnupla<IdPatamarCarga, double>());

						for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo))
							matriz_zero.setElemento(periodo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()), 0.0)));

						if (a_dados.getSize1Matriz(idIntercambio_itaipu_ande, AttMatrizIntercambio_potencia_minima) == 0)
							a_dados.vetorIntercambio.att(idIntercambio_itaipu_ande).setMatriz(AttMatrizIntercambio_potencia_minima, matriz_zero);

						if (a_dados.getSize1Matriz(idIntercambio_itaipu_ande, AttMatrizIntercambio_potencia_maxima) == 0)
							a_dados.vetorIntercambio.att(idIntercambio_itaipu_ande).setMatriz(AttMatrizIntercambio_potencia_maxima, matriz_zero);

						if (a_dados.getSize1Matriz(idIntercambio_itaipu_iv, AttMatrizIntercambio_potencia_minima) == 0)
							a_dados.vetorIntercambio.att(idIntercambio_itaipu_iv).setMatriz(AttMatrizIntercambio_potencia_minima, matriz_zero);

						if (a_dados.getSize1Matriz(idIntercambio_itaipu_iv, AttMatrizIntercambio_potencia_maxima) == 0)
							a_dados.vetorIntercambio.att(idIntercambio_itaipu_iv).setMatriz(AttMatrizIntercambio_potencia_maxima, matriz_zero);

						if (a_dados.getSizeVetor(idSubmercado_ande, AttVetorSubmercado_demanda) == 0)
							a_dados.vetorSubmercado.att(idSubmercado_ande).setVetor(AttVetorSubmercado_demanda, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0.0));

						for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo)) {

							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {
								a_dados.vetorIntercambio.att(idIntercambio_itaipu_ande).setElemento(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, std::stod(line.substr(26, 10)));
								a_dados.vetorIntercambio.att(idIntercambio_itaipu_ande).setElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, std::stod(line.substr(36, 10)));
								a_dados.vetorIntercambio.att(idIntercambio_itaipu_iv).setElemento(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, std::stod(line.substr(46, 10)));
								a_dados.vetorIntercambio.att(idIntercambio_itaipu_iv).setElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, std::stod(line.substr(56, 10)));

							}

							a_dados.vetorSubmercado.att(idSubmercado_ande).setElemento(AttVetorSubmercado_demanda, periodo, std::stod(line.substr(66, 10)));
						} // for (Periodo periodo = periodo_inicial_restricao; periodo <= periodo_final_restricao; horizonte_estudo.incrementarIterador(periodo)) {

					}//if (a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) == TipoDetalhamentoProducaoHidreletrica_por_usina) {

				}//if (line.substr(0, 2) == "RI") {

				// ALTERAÇÃO CADASTRAL DAS UHE
				if (lerRegistro("AC", line.substr(0, 2), a_registro)) {

					const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, std::stoi(line.substr(4, 3)));

					if (line.substr(9, 6) == "COFEVA") {
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_evaporacao, IdMes(std::stoi(line.substr(19, 5))), std::stod(line.substr(24, 5)));
					}//if (line.substr(9, 6) == "COFEVA") {

					if (line.substr(9, 6) == "COTVAZ") {
						std::string polinomio = line.substr(19, 5); polinomio.erase(std::remove(polinomio.begin(), polinomio.end(), ' '), polinomio.end());
						const AttComumPolinomioJusante attComumPolinomioJusante = getAttComumPolinomioJusanteFromChar(("coeficiente_" + polinomio).c_str());
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorPolinomioJusante.att(IdPolinomioJusante_1).setAtributo(attComumPolinomioJusante, std::stod(line.substr(24, 15)));
					}//if (line.substr(9, 6) == "COTVAZ"){ 

					if (line.substr(9, 6) == "COTTAR") {
						std::string polinomio = line.substr(19, 5); polinomio.erase(std::remove(polinomio.begin(), polinomio.end(), ' '), polinomio.end());
						const AttComumReservatorio attComumReservatorio = getAttComumReservatorioFromChar(("poli_cota_area_" + polinomio).c_str());
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(attComumReservatorio, std::stod(line.substr(24, 15)));
					}//if (line.substr(9, 6) == "COTTAR"){

					if (line.substr(9, 6) == "COTVOL") {
						std::string polinomio = line.substr(19, 5); polinomio.erase(std::remove(polinomio.begin(), polinomio.end(), ' '), polinomio.end());
						const AttComumReservatorio attComumReservatorio = getAttComumReservatorioFromChar(("poli_cota_volume_" + polinomio).c_str());
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(attComumReservatorio, std::stod(line.substr(24, 15)));
					}//if (line.substr(9, 6) == "COTVOL") {

					if (line.substr(9, 6) == "DESVIO") {
						const IdHidreletrica idHidreletricaJusante = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, std::stoi(line.substr(19, 5)));
						a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_jusante_desvio, idHidreletricaJusante);
					}//if (line.substr(9, 6) == "DESVIO"){

					if (line.substr(9, 6) == "JUSMED") {
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorPolinomioJusante.att(IdPolinomioJusante_1).setAtributo(AttComumPolinomioJusante_altura_ref, std::stod(line.substr(19, 10)));
					}//if (line.substr(9, 6) == "JUSMED") { 

					if (line.substr(9, 6) == "NUMCON") {

						const IdConjuntoHidraulico maiorIdConjuntoHidraulico = a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico());
						const IdSubmercado idSubmercado = lista_IdSubmercado_hidreletrica.getElemento(idHidreletrica);
						const std::string nome = a_dados.vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_nome, string());

						SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz_zero(a_horizonte_estudo, SmartEnupla<IdPatamarCarga, double>());

						for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo))
							matriz_zero.setElemento(periodo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(a_dados.getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()), 0.0)));

						if (std::stoi(line.substr(19, 5)) > int(maiorIdConjuntoHidraulico)) {
							for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico(int(maiorIdConjuntoHidraulico) + 1); idConjuntoHidraulico <= IdConjuntoHidraulico(std::stoi(line.substr(19, 5))); idConjuntoHidraulico++) {
								ConjuntoHidraulico conjuntohidraulico;
								conjuntohidraulico.setAtributo(AttComumConjuntoHidraulico_idConjuntoHidraulico, idConjuntoHidraulico);
								conjuntohidraulico.setAtributo(AttComumConjuntoHidraulico_submercado, idSubmercado);
								conjuntohidraulico.setAtributo(AttComumConjuntoHidraulico_nome, nome);
								conjuntohidraulico.setAtributo(AttComumConjuntoHidraulico_queda_referencia, 0.0);
								a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.add(conjuntohidraulico);
							}
						}//if (std::stoi(line.substr(19, 5)) > int(maiorIdConjuntoHidraulico)) {

						if (std::stoi(line.substr(19, 5)) < int(maiorIdConjuntoHidraulico)) {
							for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico(std::stoi(line.substr(19, 5)) + 1); idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {

								const IdUnidadeUHE maiorIdUnidadeUHE = a_dados.getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE());

								for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= maiorIdUnidadeUHE; idUnidade++) {
									a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidade).setVetor(AttVetorUnidadeUHE_disponibilidade, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0));
								}//for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= maiorIdUnidadeUHE; idUnidade++) {
							}
						}//if (std::stoi(line.substr(19, 5)) > int(maiorIdConjuntoHidraulico)) {

					}//if (line.substr(9, 6) == "NUMCON") {

					if (line.substr(9, 6) == "NUMJUS") {
						const IdHidreletrica idHidreletricaJusante = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, std::stoi(line.substr(19, 5)));
						a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_jusante, idHidreletricaJusante);
					}//if (line.substr(9, 6) == "NUMJUS") {	


					if (line.substr(9, 6) == "NUMMAQ") {
						try {
							const IdUnidadeUHE maiorIdUnidadeUHE = a_dados.getMaiorId(idHidreletrica, IdConjuntoHidraulico(std::stoi(line.substr(19, 5))), IdUnidadeUHE());

							if (maiorIdUnidadeUHE == IdUnidadeUHE_Nenhum) {

								for (int unidade = 1; unidade <= std::stoi(line.substr(24, 5)); unidade++) {

									UnidadeUHE unidadeUHE;
									unidadeUHE.setAtributo(AttComumUnidadeUHE_idUnidadeUHE, IdUnidadeUHE(unidade));
									unidadeUHE.setAtributo(AttComumUnidadeUHE_submercado, a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico(std::stoi(line.substr(19, 5)))).getAtributo(AttComumConjuntoHidraulico_submercado, IdSubmercado()));

									a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico(std::stoi(line.substr(19, 5)))).vetorUnidadeUHE.add(unidadeUHE);
								}

							}// else if (maiorIdUnidadeUHE == IdUnidadeUHE_Nenhum) {

							else if (maiorIdUnidadeUHE < IdUnidadeUHE(std::stoi(line.substr(24, 5)))) {

								for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE(int(maiorIdUnidadeUHE + 1)); idUnidadeUHE <= IdUnidadeUHE(std::stoi(line.substr(24, 5))); idUnidadeUHE++) {

									UnidadeUHE unidadeUHE;
									unidadeUHE.setAtributo(AttComumUnidadeUHE_idUnidadeUHE, idUnidadeUHE);
									unidadeUHE.setAtributo(AttComumUnidadeUHE_submercado, a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico(std::stoi(line.substr(19, 5)))).getAtributo(AttComumConjuntoHidraulico_submercado, IdSubmercado()));

									unidadeUHE.setAtributo(AttComumUnidadeUHE_potencia_minima, \
										a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico(std::stoi(line.substr(19, 5)))).vetorUnidadeUHE.att(maiorIdUnidadeUHE).getAtributo(AttComumUnidadeUHE_potencia_minima, double()));
									unidadeUHE.setAtributo(AttComumUnidadeUHE_potencia_maxima, \
										a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico(std::stoi(line.substr(19, 5)))).vetorUnidadeUHE.att(maiorIdUnidadeUHE).getAtributo(AttComumUnidadeUHE_potencia_maxima, double()));

									unidadeUHE.setAtributo(AttComumUnidadeUHE_vazao_turbinada_minima, \
										a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico(std::stoi(line.substr(19, 5)))).vetorUnidadeUHE.att(maiorIdUnidadeUHE).getAtributo(AttComumUnidadeUHE_vazao_turbinada_minima, double()));
									unidadeUHE.setAtributo(AttComumUnidadeUHE_vazao_turbinada_maxima, \
										a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico(std::stoi(line.substr(19, 5)))).vetorUnidadeUHE.att(maiorIdUnidadeUHE).getAtributo(AttComumUnidadeUHE_vazao_turbinada_maxima, double()));

									a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico(std::stoi(line.substr(19, 5)))).vetorUnidadeUHE.add(unidadeUHE);

								}

							}//if (maiorIdUnidadeUHE != IdUnidadeUHE_Nenhum && maiorIdUnidadeUHE > IdUnidadeUHE(std::stoi(line.substr(24, 5)))) {

							else if (maiorIdUnidadeUHE > IdUnidadeUHE(std::stoi(line.substr(24, 5)))) {

								for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE(std::stoi(line.substr(24, 5)) + 1); idUnidadeUHE <= maiorIdUnidadeUHE; idUnidadeUHE++) {
								}

							}// else if (maiorIdUnidadeUHE != IdUnidadeUHE_Nenhum && maiorIdUnidadeUHE <= IdUnidadeUHE(std::stoi(line.substr(24, 5)))) {
						}
						catch (const std::exception& erro) { throw std::invalid_argument("NUMMAQ, linha ( " + line + ") ): \n" + std::string(erro.what())); }
					}//if (line.substr(9, 6) == "NUMMAQ") {

					//if (line.substr(9, 6) == "NUMPOS") {
					//	lista_codigo_ONS_posto.setElemento(idHidreletrica, std::stoi(line.substr(19, 5)));
					//}//if (line.substr(9, 6) == "NUMPOS") { 

					if (line.substr(9, 6) == "PERHID") {
						const IdUnidadeUHE maiorIdUnidadeUHE = a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico(std::stoi(line.substr(19, 5)))).vetorUnidadeUHE.getMaiorId();
						for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= maiorIdUnidadeUHE; idUnidadeUHE++)
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico(std::stoi(line.substr(19, 5)))).vetorUnidadeUHE.att(idUnidadeUHE).setAtributo(AttComumUnidadeUHE_perda_hidraulica, std::stod(line.substr(19, 10)));
					}//if (line.substr(9, 6) == "PERHID") {

					if (line.substr(9, 6) == "POTEFE") {
						const IdUnidadeUHE maiorIdUnidadeUHE = a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico(std::stoi(line.substr(19, 5)))).vetorUnidadeUHE.getMaiorId();
						for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= maiorIdUnidadeUHE; idUnidadeUHE++) {
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico(std::stoi(line.substr(19, 5)))).vetorUnidadeUHE.att(\
								idUnidadeUHE).setAtributo(AttComumUnidadeUHE_potencia_maxima, std::stod(line.substr(24, 10)));
						}
					}//if (line.substr(9, 6) == "POTEFE") {	

					if (line.substr(9, 6) == "VAZEFE") {
						const IdUnidadeUHE maiorIdUnidadeUHE = a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico(std::stoi(line.substr(19, 5)))).vetorUnidadeUHE.getMaiorId();
						for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= maiorIdUnidadeUHE; idUnidadeUHE++) {
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico(std::stoi(line.substr(19, 5)))).vetorUnidadeUHE.att(\
								idUnidadeUHE).setAtributo(AttComumUnidadeUHE_vazao_turbinada_maxima, std::stod(line.substr(24, 10)));
						}
					}//if (line.substr(9, 6) == "VAZEFE") {

					if (line.substr(9, 6) == "PROESP") {
						const IdUnidadeUHE maiorIdUnidadeUHE = a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico(std::stoi(line.substr(19, 5)))).vetorUnidadeUHE.getMaiorId();
						for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= maiorIdUnidadeUHE; idUnidadeUHE++)
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico(std::stoi(line.substr(19, 5)))).vetorUnidadeUHE.att(idUnidadeUHE).setAtributo(AttComumUnidadeUHE_fator_de_producao, std::stod(line.substr(19, 10)));
					}//if (line.substr(9, 6) == "PROESP") { 

					if (line.substr(9, 6) == "TAXFOR") {
						a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_indisponibilidade_forcada, std::stod(line.substr(19, 10)));
					}//if (line.substr(9, 6) == "TAXFOR") { 

					if (line.substr(9, 6) == "TAXMAN") {
						a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_indisponibilidade_programada, std::stod(line.substr(19, 10)));
					}//if (line.substr(9, 6) == "TAXMAN") { 

					if (line.substr(9, 6) == "VOLMAX") {
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_volume_maximo, std::stod(line.substr(19, 10)));
						//for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_estudo, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo))
							//a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_maximo, periodo, std::stod(line.substr(19, 10)));
						
					}//if (line.substr(9, 6) == "VOLMAX") { 

					if (line.substr(9, 6) == "VOLMIN") {
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_volume_minimo, std::stod(line.substr(19, 10)));
						//for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_estudo, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo)) 
							//a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_minimo, periodo, std::stod(line.substr(19, 10)));
						
					}//if (line.substr(9, 6) == "VOLMIN") {

					if (line.substr(9, 6) == "VSVERT") {
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_volume_minimo_vertimento, std::stod(line.substr(19, 10)));
					}//if (line.substr(9, 6) == "VSVERT") { 

					if (line.substr(9, 6) == "VMDESV") {
						a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_volume_minimo_desvio, std::stod(line.substr(19, 10)));
					}//if (line.substr(9, 6) == "VMDESV") {

					if (line.substr(9, 6) == "JUSENA") {
						const IdHidreletrica idHidreletricaENA = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, std::stoi(line.substr(19, 5)));
						//						a_dados.vetorHidreletrica.att(idHidreletrica).setAtributo(AttComumHidreletrica_Jusante_calculo_ENA, idHidreletricaENA);
					}//if (line.substr(9, 6) == "JUSENA") {

				}//if (lerRegistro("AC", line.substr(0, 2), a_registro)) {

				// DESVIO DE AGUA 
				if (lerRegistro("DA", line.substr(0, 2), a_registro)) {
					try {
						LeituraCEPEL leituraCEPEL;
						const Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(8, 2), line.substr(11, 2), line.substr(14, 1));
						const Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(16, 2), line.substr(19, 2), line.substr(22, 1));

						const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, std::stoi(line.substr(4, 3)));

						const IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico(a_dados.getMaiorId(IdIntercambioHidraulico()) + 1);
						IntercambioHidraulico intercambioHidraulico;
						intercambioHidraulico.setAtributo(AttComumIntercambioHidraulico_idIntercambioHidraulico, idIntercambioHidraulico);
						intercambioHidraulico.setAtributo(AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico_retirada);

						a_dados.vetorIntercambioHidraulico.add(intercambioHidraulico);

						a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setAtributo(AttComumIntercambioHidraulico_hidreletrica_origem, idHidreletrica);
						a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setAtributo(AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica_Nenhum);

						a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setVetor(AttVetorIntercambioHidraulico_desvio_agua_minimo, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
						a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setVetor(AttVetorIntercambioHidraulico_desvio_agua_maximo, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

						for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo)) {
							a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setElemento(AttVetorIntercambioHidraulico_desvio_agua_minimo, periodo, std::stod(line.substr(24, 10)));
							a_dados.vetorIntercambioHidraulico.att(idIntercambioHidraulico).setElemento(AttVetorIntercambioHidraulico_desvio_agua_maximo, periodo, std::stod(line.substr(24, 10)));
						}//for (Periodo periodo = periodo_inicial_restricao; periodo <= periodo_final_restricao; horizonte_estudo.incrementarIterador(periodo)) {
					}
					catch (const std::exception& erro) { throw std::invalid_argument("lerRegistro(DA, linha ( " + line + ") ): \n" + std::string(erro.what())); }

				}//if (lerRegistro("DA", line.substr(0, 2), a_registro)) {

				// VOLUME DE ESPERA MAXIMO 
				if (lerRegistro("VE", line.substr(0, 2), a_registro)) {
					try {
						LeituraCEPEL leituraCEPEL;
						const Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(8, 2), line.substr(11, 2), line.substr(14, 1));
						const Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(16, 2), line.substr(19, 2), line.substr(22, 1));

						const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, std::stoi(line.substr(4, 3)));

						if (a_dados.getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_percentual_volume_util_maximo) == 0)
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_percentual_volume_util_maximo, SmartEnupla<Periodo, double>(horizonte_estudo, 1.0));

						for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo)) {
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_percentual_volume_util_maximo, periodo, double(std::stod(line.substr(24, 10)) / 100.00));
						}
					}
					catch (const std::exception& erro) { throw std::invalid_argument("lerRegistro(VE, linha ( " + line + ") ): \n" + std::string(erro.what())); }
				}//if (lerRegistro("VE", line.substr(0, 2), a_registro)) {

					// MANUTENCAO HIDRAULICA
				if (lerRegistro("MH", line.substr(0, 2), a_registro)) {
					try {
						LeituraCEPEL leituraCEPEL;
						const Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(14, 2), line.substr(17, 2), line.substr(20, 1));
						const Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(22, 2), line.substr(25, 2), line.substr(28, 1));

						const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, std::stoi(line.substr(4, 3)));

						std::string arg_30_1 = line.substr(30, 1);

						if (std::stoi(arg_30_1.c_str()) == 0) {

							std::string arg_09_2 = line.substr(9, 2);
							std::string arg_12_2 = line.substr(12, 2);

							if ((arg_09_2 != std::string("  ")) && (arg_12_2 != std::string("  "))) {

								if (a_dados.getSizeVetor(idHidreletrica, IdConjuntoHidraulico(std::stoi(line.substr(9, 2))), IdUnidadeUHE(std::stoi(line.substr(12, 2))), AttVetorUnidadeUHE_disponibilidade) == 0)
									a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico(std::stoi(line.substr(9, 2)))).\
									vetorUnidadeUHE.att(IdUnidadeUHE(std::stoi(line.substr(12, 2)))).setVetor(AttVetorUnidadeUHE_disponibilidade, SmartEnupla<Periodo, double >(a_horizonte_estudo, 1));

								for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo)) {
									a_dados.vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(IdConjuntoHidraulico(std::stoi(line.substr(9, 2)))).\
										vetorUnidadeUHE.att(IdUnidadeUHE(std::stoi(line.substr(12, 2)))).setElemento(AttVetorUnidadeUHE_disponibilidade, periodo, 0.0);
								}

							}

						}// if (std::stoi(arg_30_1.c_str()) == 0) {

						if (std::stoi(line.substr(30, 1)) == 1) { throw std::invalid_argument("Tratar o regitro MH (Manutenção Hidraulica), unidades entrando em operação."); }
					}
					catch (const std::exception& erro) { throw std::invalid_argument("lerRegistro(MH, linha ( " + line + ") ): \n" + std::string(erro.what())); }

				}// if (lerRegistro("MH", line.substr(0, 2), a_registro)) {

				// MANUTENCAO TERMELETRICA
				if (lerRegistro("MT", line.substr(0, 2), a_registro)) {
					try {
						LeituraCEPEL leituraCEPEL;
						const Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(13, 2), line.substr(16, 2), line.substr(19, 1));
						const Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(21, 2), line.substr(24, 2), line.substr(27, 1));
						const IdTermeletrica idTermeletrica = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, std::stoi(line.substr(4, 3)));

						if (std::stoi(line.substr(4, 3)) == 205)
							int aa = 1;

						if (idTermeletrica == IdTermeletrica_Nenhum)
							throw std::invalid_argument("Nao inicializada idTermeletrica com codigo_ONS_" + std::stoi(line.substr(4, 3)));

						if (a_dados.getAtributo(idTermeletrica, IdUnidadeUTE_1, AttComumUnidadeUTE_unidade_equivalente, bool())) {

							IdUnidadeRealUTE idUnidadeRealUTE = IdUnidadeRealUTE(std::stoi(line.substr(8, 3)));
							IdUnidadeUTE maiorIdUnidadeUTE = a_dados.getMaiorId(idTermeletrica, IdUnidadeUTE());

							for (IdUnidadeUTE idUnidade = IdUnidadeUTE_1; idUnidade <= maiorIdUnidadeUTE; idUnidade++) {
								int size_numero_unidades = a_dados.getSizeVetor(idTermeletrica, idUnidade, AttVetorUnidadeUTE_unidades_reais);
								for (int unidade = 0; unidade < size_numero_unidades; unidade++) {
									if (idUnidadeRealUTE == a_dados.getElementoVetor(idTermeletrica, idUnidade, AttVetorUnidadeUTE_unidades_reais, unidade, IdUnidadeRealUTE())) {

										const int numero_minimo_unidades_disp = a_dados.getAtributo(idTermeletrica, idUnidade, AttComumUnidadeUTE_numero_minimo_unidades_reais, int());

										const double disp_minima = double(numero_minimo_unidades_disp) / double(size_numero_unidades);

										for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo)) {

											double disponibilidade = a_dados.getElementoVetor(idTermeletrica, idUnidade, AttVetorUnidadeUTE_disponibilidade, periodo, double());

											if (std::stoi(line.substr(29, 1)) == 0)
												disponibilidade = (disponibilidade / double(size_numero_unidades)) * double(size_numero_unidades - 1);

											if (std::stoi(line.substr(29, 1)) == 1)
												disponibilidade = (disponibilidade / double(size_numero_unidades)) * double(size_numero_unidades - 1);

											if (disp_minima > disponibilidade)
												disponibilidade = 0.0;

											a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidade).setElemento(AttVetorUnidadeUTE_disponibilidade, periodo, disponibilidade);

										}//	for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo)){

									}//if (idUnidadeRealUTE == a_dados.getElementoVetor(idTermeletrica, idUnidade, AttVetorUnidadeUTE_unidades_reais, unidade, IdUnidadeRealUTE())) {

								}//for (int unidade = 0; unidade < size_numero_unidades; unidade++) {

							}//for (IdUnidadeUTE idUnidade = IdUnidadeUTE_1; idUnidade <= maiorIdUnidadeUTE; idUnidade++) {

						}//if (a_dados.getAtributo(idTermeletrica, IdUnidadeUTE_1, AttComumUnidadeUTE_unidade_equivalente, bool())) {
						else {

							std::string nomeUTE = a_dados.vetorTermeletrica.att(idTermeletrica).getAtributo(AttComumTermeletrica_nome, string());

							if (line.size() > 8) {

								std::string nomeUnidade = nomeUTE + "_UNIDADE_" + std::to_string(std::stoi(line.substr(8, 3)));

								if (a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.getIdObjetos(AttComumUnidadeUTE_nome, nomeUnidade).size() > 0) {

									IdUnidadeUTE idUnidadeUTE = a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.getIdObjetos(AttComumUnidadeUTE_nome, nomeUnidade).at(0);

									double disponibilidade = 0;
									if (std::stoi(line.substr(29, 1)) == 1) {
										disponibilidade = 1;
										a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).setVetor(AttVetorUnidadeUTE_disponibilidade, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0.0));
									}

									for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo))
										a_dados.vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).setElemento(AttVetorUnidadeUTE_disponibilidade, periodo, 0.0);

								}
							}
						}//else {
					}
					catch (const std::exception& erro) { throw std::invalid_argument("lerRegistro(MT, linha ( " + line + ") ): \n" + std::string(erro.what())); }

				}//if (lerRegistro("MT", line.substr(0, 2), a_registro)) {

				// TAXA DE ENCHIMENTO DO VOLUME MORTO
				if (lerRegistro("VM", line.substr(0, 2), a_registro)) {
					try {
						const Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(9, 2), line.substr(12, 2), line.substr(15, 1));
						const Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(17, 2), line.substr(19, 2), line.substr(22, 1));
						const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, std::stoi(line.substr(4, 3)));
						for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo)) {
							a_dados.vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_taxa_enchimento_volume_morto, periodo, std::stod(line.substr(24, 10)));
						}
					}
					catch (const std::exception& erro) { throw std::invalid_argument("lerRegistro(VM, linha ( " + line + ") ): \n" + std::string(erro.what())); }
				}

				// TAXA DE DESCARGA DE FUNDO (DEFLUENCIA MINIMA DURANTE ENCHIMENTO DO VOLUME MORTO)
				if (lerRegistro("DF", line.substr(0, 2), a_registro)) {
					try {
						const Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(9, 2), line.substr(12, 2), line.substr(15, 1));
						const Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(17, 2), line.substr(19, 2), line.substr(22, 1));
						const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, std::stoi(line.substr(4, 3)));
						for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo)) {
							a_dados.vetorHidreletrica.att(idHidreletrica).setElemento(AttVetorHidreletrica_vazao_defluente_minima, periodo, std::stod(line.substr(24, 10)));
						}
					}
					catch (const std::exception& erro) { throw std::invalid_argument("lerRegistro(DF, linha ( " + line + ") ): \n" + std::string(erro.what())); }
				}

				// MANUTENCAO DAS USINAS ELEVATORIAS
				if (lerRegistro("ME", line.substr(0, 2), a_registro)) {
					try {
						const Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(8, 2), line.substr(11, 2), line.substr(14, 1));
						const Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(16, 2), line.substr(19, 2), line.substr(22, 1));
						const std::vector<IdUsinaElevatoria> idUsinaElevatoria = a_dados.vetorUsinaElevatoria.getIdObjetos(AttComumUsinaElevatoria_nome, std::stoi(line.substr(4, 3)));
						for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo))
							a_dados.vetorUsinaElevatoria.att(idUsinaElevatoria.at(0)).setElemento(AttVetorUsinaElevatoria_fator_disponibilidade, periodo, std::stod(line.substr(24, 10)));
					}
					catch (const std::exception& erro) { throw std::invalid_argument("lerRegistro(ME, linha ( " + line + ") ): \n" + std::string(erro.what())); }
				}

				// CONTRATO IMPORTACAO/EXPORTACAO
				if (lerRegistro("CI", line.substr(0, 2), a_registro) || lerRegistro("CE", line.substr(0, 2), a_registro)) {
					try {
						const Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(25, 2), line.substr(28, 2), line.substr(31, 1));
						const Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(33, 2), line.substr(36, 2), line.substr(39, 1));

						const IdSubmercado idSubmercado = getIdFromCodigoONS(lista_codigo_ONS_submercado, std::stoi(line.substr(18, 5)));

						//VERIFICA SE O CONTRATO JÁ ESTA INSTANCIADO
						IdContrato idContrato = getIdFromCodigoONS(lista_codigo_ONS_contrato, std::stoi(line.substr(3, 3)));

						if (idContrato == IdContrato_Nenhum) {
							idContrato = IdContrato(a_dados.vetorContrato.getMaiorId() + 1);
							Contrato contrato;
							contrato.setAtributo(AttComumContrato_idContrato, idContrato);
							contrato.setAtributo(AttComumContrato_tipo_contrato, TipoContrato_importacao);
							contrato.setAtributo(AttComumContrato_tipo_unidade, TipoUnidadeRestricaoContrato_MW);
							contrato.setAtributo(AttComumContrato_nome, line.substr(7, 10));
							contrato.setAtributo(AttComumContrato_submercado, idSubmercado);
							a_dados.vetorContrato.add(contrato);
							a_dados.vetorContrato.att(idContrato).setVetor(AttVetorContrato_variacao_acrescimo_maxima, SmartEnupla<Periodo, double >(a_horizonte_estudo, 10000000));
							a_dados.vetorContrato.att(idContrato).setVetor(AttVetorContrato_variacao_decrescimo_maxima, SmartEnupla<Periodo, double >(a_horizonte_estudo, 0));
							a_dados.vetorContrato.att(idContrato).setMatriz(AttMatrizContrato_energia_maxima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(IdPatamarCarga_1, 100000.0))));
							a_dados.vetorContrato.att(idContrato).setMatriz(AttMatrizContrato_energia_minima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(IdPatamarCarga_1, 0.0))));
							a_dados.vetorContrato.att(idContrato).setMatriz(AttMatrizContrato_preco_energia_imp_exp, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(IdPatamarCarga_1, 0.0))));


							lista_codigo_ONS_contrato.addElemento(idContrato, std::stoi(line.substr(3, 3)));

							if (line.substr(0, 2) == "CE") { a_dados.vetorContrato.att(idContrato).setAtributo(AttComumContrato_tipo_contrato, TipoContrato_exportacao); }
						}

						if (std::stoi(line.substr(23, 1)) == 1) {
							if (std::stoi(line.substr(41, 1)) == 1) { a_dados.vetorContrato.att(idContrato).setAtributo(AttComumContrato_tipo_unidade, TipoUnidadeRestricaoContrato_percentual); }
							a_dados.vetorContrato.att(idContrato).setAtributo(AttComumContrato_tipo_restricao, TipoRestricaoContrato_rampa);
							a_dados.vetorContrato.att(idContrato).setAtributo(AttComumContrato_energia_imp_exp_periodo_anterior, std::stod(line.substr(73, 10)));
							for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo)) {
								a_dados.vetorContrato.att(idContrato).setElemento(AttVetorContrato_variacao_acrescimo_maxima, periodo, std::stod(line.substr(43, 10)));
								a_dados.vetorContrato.att(idContrato).setElemento(AttVetorContrato_variacao_decrescimo_maxima, periodo, std::stod(line.substr(53, 10)));
							}
						}//if(std::stoi(line.substr(23, 1)) == 1 ){ 

						if (std::stoi(line.substr(23, 1)) == 2) {
							a_dados.vetorContrato.att(idContrato).setAtributo(AttComumContrato_tipo_restricao, TipoRestricaoContrato_limite);
							for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo)) {
								a_dados.vetorContrato.att(idContrato).setElemento(AttMatrizContrato_energia_maxima, periodo, IdPatamarCarga_1, std::stod(line.substr(53, 10)));
								a_dados.vetorContrato.att(idContrato).setElemento(AttMatrizContrato_energia_minima, periodo, IdPatamarCarga_1, std::stod(line.substr(43, 10)));
								a_dados.vetorContrato.att(idContrato).setElemento(AttMatrizContrato_preco_energia_imp_exp, periodo, IdPatamarCarga_1, std::stod(line.substr(63, 10)));
							}
						}//if(std::stoi(line.substr(23, 1)) == 2 ){ 
					}

					catch (const std::exception& erro) { throw std::invalid_argument("lerRegistro(CI/CE, linha ( " + line + ") ): \n" + std::string(erro.what())); }

				}//if (lerRegistro("CI", line.substr(0, 2), a_registro) || lerRegistro("CE", line.substr(0, 2), a_registro)) {

				 // RESTRICOES ELETRICAS ESPECIAIS - DADOS DA RESTRICAO  
				std::string registro = line.substr(0, 3);
				registro.erase(std::remove(registro.begin(), registro.end(), ' '), registro.end());
				if (lerRegistro("RE", registro, a_registro)) {
					try {
						std::string horaFinal = "0";
						std::string minFinal = "0";

						if (line.size() > 19) { horaFinal = line.substr(20, 2); }
						if (line.size() > 22) { minFinal = line.substr(23, 1); }

						const Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(9, 2), line.substr(12, 2), line.substr(15, 1));
						const Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(17, 2), horaFinal, minFinal);

						IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica(a_dados.vetorRestricaoEletrica.getMaiorId() + 1);

						RestricaoEletrica restricaoEletrica;
						restricaoEletrica.setAtributo(AttComumRestricaoEletrica_idRestricaoEletrica, idRestricaoEletrica);
						restricaoEletrica.setAtributo(AttComumRestricaoEletrica_nome, "RE_" + (line.substr(4, 3)));
						a_dados.vetorRestricaoEletrica.add(restricaoEletrica);

						lista_codigo_ONS_restricao_eletrica.addElemento(idRestricaoEletrica, std::stoi(line.substr(4, 3)));

						a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).setMatriz(AttMatrizRestricaoEletrica_potencia_minima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(IdPatamarCarga_1, 0.0))));
						a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).setMatriz(AttMatrizRestricaoEletrica_potencia_maxima, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(IdPatamarCarga_1, getdoubleFromChar("max")))));
					}
					catch (const std::exception& erro) { throw std::invalid_argument("lerRegistro(RE, linha ( " + line + ") ): \n" + std::string(erro.what())); }

				}//if (lerRegistro("RE", line.substr(0, 3), a_registro)) {

				 // RESTRICOES ELETRICAS ESPECIAIS - LIMITES  
				if (lerRegistro("LU", line.substr(0, 2), a_registro)) {

					std::string horaFinal = "0";
					std::string minFinal = "0";
					std::string limiteInferior = "0";
					std::string limiteSuperior = "1000000";

					if (line.size() > 18) { horaFinal = line.substr(19, 2); }
					if (line.size() > 21) { minFinal = line.substr(22, 1); }
					if (line.size() > 23) { limiteInferior = line.substr(24, 10); }
					if (line.size() > 33) { limiteSuperior = line.substr(34, 10); }

					limiteInferior.erase(std::remove(limiteInferior.begin(), limiteInferior.end(), ' '), limiteInferior.end());
					limiteSuperior.erase(std::remove(limiteSuperior.begin(), limiteSuperior.end(), ' '), limiteSuperior.end());

					const Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(8, 2), line.substr(11, 2), line.substr(14, 1));
					const Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(16, 2), horaFinal, minFinal);

					const IdRestricaoEletrica idRestricaoEletrica = getIdFromCodigoONS(lista_codigo_ONS_restricao_eletrica, std::stoi(line.substr(4, 3)));

					if (idRestricaoEletrica == IdRestricaoEletrica_Nenhum)
						throw std::invalid_argument("Nao inicializada idRestricaoEletrica  com codigo_ONS_" + line.substr(4, 3));

					for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo)) {
						if (limiteInferior != "") { a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).setElemento(AttMatrizRestricaoEletrica_potencia_minima, periodo, IdPatamarCarga_1, std::stod(limiteInferior)); }
						if (limiteSuperior != "") { a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).setElemento(AttMatrizRestricaoEletrica_potencia_maxima, periodo, IdPatamarCarga_1, std::stod(limiteSuperior)); }
					}

				}//if (lerRegistro("LU", line.substr(0, 2), a_registro)) {

				 // RESTRICOES ELETRICAS ESPECIAIS - HIDRELETRICAS 
				if (lerRegistro("FH", line.substr(0, 2), a_registro)) {
					try {

						std::string horaFinal = "0";
						std::string minFinal = "0";
						std::string conjuntoHidraulico = line.substr(27, 2);
						conjuntoHidraulico.erase(std::remove(conjuntoHidraulico.begin(), conjuntoHidraulico.end(), ' '), conjuntoHidraulico.end());

						if (line.size() > 18) { horaFinal = line.substr(19, 2); }
						if (line.size() > 21) { minFinal = line.substr(22, 1); }

						const Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(8, 2), line.substr(11, 2), line.substr(14, 1));
						const Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(16, 2), horaFinal, minFinal);

						const IdRestricaoEletrica idRestricaoEletrica = getIdFromCodigoONS(lista_codigo_ONS_restricao_eletrica, std::stoi(line.substr(4, 3)));

						if (idRestricaoEletrica == IdRestricaoEletrica_Nenhum)
							throw std::invalid_argument("Nao inicializada idRestricaoEletrica  com codigo_ONS_" + line.substr(4, 3));


						const IdHidreletrica idHidreletrica = getIdFromCodigoONS(lista_codigo_ONS_hidreletrica, std::stoi(line.substr(24, 3)));

						idElementoSistema = IdElementoSistema(a_dados.getMaiorId(idRestricaoEletrica, IdElementoSistema()) + 1);

						ElementoSistema elementoSistema;
						elementoSistema.setAtributo(AttComumElementoSistema_idElementoSistema, idElementoSistema);
						elementoSistema.setAtributo(AttComumElementoSistema_tipo_elemento, TipoElementoSistema_hidreletrica);
						elementoSistema.setAtributo(AttComumElementoSistema_hidreletrica, idHidreletrica);

						if (conjuntoHidraulico != "") {
							const IdConjuntoHidraulico idConjuntoHidraulico = getIdConjuntoHidraulicoFromChar(conjuntoHidraulico.c_str());

							if ((a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_codigo_usina, int()) == 66) && \
								(a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica()) == TipoDetalhamentoProducaoHidreletrica_por_usina)) {

								const IdSubmercado idSubmercado_itaipu = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_submercado_ITAIPU);
								const IdSubmercado idSubmercado_ande = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_ANDE);
								const IdSubmercado idSubmercado_iv = getIdFromCodigoONS(lista_codigo_ONS_submercado, codigo_IV);

								std::vector<IdIntercambio> idIntercambio_inicializado = a_dados.vetorIntercambio.getIdObjetos(AttComumIntercambio_submercado_origem, idSubmercado_itaipu);

								int idIntercambio_inicializado_size = int(idIntercambio_inicializado.size());

								ElementoSistema elementoSistema;
								elementoSistema.setAtributo(AttComumElementoSistema_idElementoSistema, idElementoSistema);
								elementoSistema.setAtributo(AttComumElementoSistema_tipo_elemento, TipoElementoSistema_intercambio);

								if (idConjuntoHidraulico == IdConjuntoHidraulico_1) {
									IdIntercambio idIntercambio_itaipu_iv = IdIntercambio_Nenhum;
									for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++)
										if (a_dados.getAtributo(idIntercambio_inicializado.at(intercambio_inicializado), AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_iv) {
											idIntercambio_itaipu_iv = idIntercambio_inicializado.at(intercambio_inicializado);
											break;
										}

									if (idIntercambio_itaipu_iv == IdIntercambio_Nenhum) { throw std::invalid_argument("Registro RI - Intercambio nao encontrado"); }

									elementoSistema.setAtributo(AttComumElementoSistema_intercambio, idIntercambio_itaipu_iv);

								}//if (idConjuntoHidraulico == IdConjuntoHidraulico_1) {

								else if (idConjuntoHidraulico == IdConjuntoHidraulico_2) {
									IdIntercambio idIntercambio_itaipu_ande = IdIntercambio_Nenhum;
									for (int intercambio_inicializado = 0; intercambio_inicializado < idIntercambio_inicializado_size; intercambio_inicializado++)
										if (a_dados.getAtributo(idIntercambio_inicializado.at(intercambio_inicializado), AttComumIntercambio_submercado_destino, IdSubmercado()) == idSubmercado_ande) {
											idIntercambio_itaipu_ande = idIntercambio_inicializado.at(intercambio_inicializado);
											break;
										}

									if (idIntercambio_itaipu_ande == IdIntercambio_Nenhum) { throw std::invalid_argument("Registro RI - Intercambio nao encontrado"); }

									elementoSistema.setAtributo(AttComumElementoSistema_intercambio, idIntercambio_itaipu_ande);
								}//if (idConjuntoHidraulico == IdConjuntoHidraulico_1) {


							}//if ((a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_codigo_usina, int()) == 66) &&

							else {
								const IdConjuntoHidraulico idConjuntoHidraulico = getIdConjuntoHidraulicoFromChar(conjuntoHidraulico.c_str());
								elementoSistema.setAtributo(AttComumElementoSistema_conjuntohidraulico, idConjuntoHidraulico);
								elementoSistema.setAtributo(AttComumElementoSistema_tipo_elemento, TipoElementoSistema_conjunto_hidraulico);
							}//else {

						}//if (conjuntoHidraulico != "") {

						a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.add(elementoSistema);

						a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.att(idElementoSistema).setVetor(AttVetorElementoSistema_fator_participacao, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0.0));

						for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo))
							a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.att(idElementoSistema).setElemento(AttVetorElementoSistema_fator_participacao, periodo, std::stod(line.substr(34, 10)));
					}
					catch (const std::exception& erro) { throw std::invalid_argument("lerRegistro(FH, linha ( " + line + ") ): \n" + std::string(erro.what())); }

				}//if (lerRegistro("FH", line.substr(0, 2), a_registro)) {

				// RESTRICOES ELETRICAS ESPECIAIS - TERMELETRICAS 
				if (lerRegistro("FT", line.substr(0, 2), a_registro)) {
					try {
						std::string horaFinal = "0";
						std::string minFinal = "0";

						if (line.size() > 18) { horaFinal = line.substr(19, 2); }
						if (line.size() > 21) { minFinal = line.substr(22, 1); }

						const Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(8, 2), line.substr(11, 2), line.substr(14, 1));
						const Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(16, 2), horaFinal, minFinal);

						const IdRestricaoEletrica idRestricaoEletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_restricao_eletrica, std::stoi(line.substr(4, 3)));

						if (idRestricaoEletrica_inicializado == IdRestricaoEletrica_Nenhum)
							throw std::invalid_argument("Nao inicializada idRestricaoEletrica  com codigo_ONS_" + line.substr(4, 3));

						const IdRestricaoEletrica  idRestricaoEletrica = idRestricaoEletrica_inicializado;

						const IdTermeletrica idTermeletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_termeletrica, std::stoi(line.substr(24, 3)));

						if (idTermeletrica_inicializado == IdTermeletrica_Nenhum)
							throw std::invalid_argument("Nao inicializada idTermeletrica com codigo_ONS_" + std::stoi(line.substr(24, 3)));

						const IdTermeletrica idTermeletrica = idTermeletrica_inicializado;

						idElementoSistema = IdElementoSistema(a_dados.getMaiorId(idRestricaoEletrica, IdElementoSistema()) + 1);

						const double fator_participacao = std::stod(line.substr(34, 10));

						ElementoSistema elementoSistema;
						elementoSistema.setAtributo(AttComumElementoSistema_idElementoSistema, idElementoSistema);
						elementoSistema.setAtributo(AttComumElementoSistema_tipo_elemento, TipoElementoSistema_termeletrica);
						elementoSistema.setAtributo(AttComumElementoSistema_termeletrica, idTermeletrica);
						elementoSistema.setAtributo(AttComumElementoSistema_fator_participacao, std::stod(line.substr(34, 10)));
						a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.add(elementoSistema);

						a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.att(idElementoSistema).setVetor(AttVetorElementoSistema_fator_participacao, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0.0));

						for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo))
							a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.att(idElementoSistema).setElemento(AttVetorElementoSistema_fator_participacao, periodo, fator_participacao);
					}
					catch (const std::exception& erro) { throw std::invalid_argument("lerRegistro(FT, linha ( " + line + ") ): \n" + std::string(erro.what())); }

				}//	if (lerRegistro("FT", line.substr(0, 2), a_registro)) {

				// RESTRICOES ELETRICAS ESPECIAIS - INTERCAMBIO
				if (lerRegistro("FI", line.substr(0, 2), a_registro)) {
					try {
						std::string ss1 = line.substr(24, 2);	ss1.erase(std::remove(ss1.begin(), ss1.end(), ' '), ss1.end());
						std::string ss2 = line.substr(29, 2);	ss2.erase(std::remove(ss2.begin(), ss2.end(), ' '), ss2.end());
						std::string nome = ss1 + " -> " + ss2;

						const Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(8, 2), line.substr(11, 2), line.substr(14, 1));
						const Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(16, 2), line.substr(19, 2), line.substr(22, 2));

						const IdRestricaoEletrica idRestricaoEletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_restricao_eletrica, std::stoi(line.substr(4, 3)));

						if (idRestricaoEletrica_inicializado == IdRestricaoEletrica_Nenhum)
							throw std::invalid_argument("Nao inicializada idRestricaoEletrica  com codigo_ONS_" + line.substr(4, 3));

						const IdRestricaoEletrica  idRestricaoEletrica = idRestricaoEletrica_inicializado;

						const std::vector<IdIntercambio>       idIntercambio = a_dados.vetorIntercambio.getIdObjetos(AttComumIntercambio_nome, nome);

						idElementoSistema = IdElementoSistema(a_dados.getMaiorId(idRestricaoEletrica, IdElementoSistema()) + 1);

						const double fator_participacao = std::stod(line.substr(34, 10));

						ElementoSistema elementoSistema;
						elementoSistema.setAtributo(AttComumElementoSistema_idElementoSistema, idElementoSistema);
						elementoSistema.setAtributo(AttComumElementoSistema_tipo_elemento, TipoElementoSistema_intercambio);
						elementoSistema.setAtributo(AttComumElementoSistema_intercambio, idIntercambio.at(0));
						elementoSistema.setAtributo(AttComumElementoSistema_fator_participacao, fator_participacao);
						elementoSistema.setAtributo(AttComumElementoSistema_fator_participacao, std::stod(line.substr(34, 10)));
						a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.add(elementoSistema);

						a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.att(idElementoSistema).setVetor(AttVetorElementoSistema_fator_participacao, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0.0));

						for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo))
							a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.att(idElementoSistema).setElemento(AttVetorElementoSistema_fator_participacao, periodo, fator_participacao);
					}
					catch (const std::exception& erro) { throw std::invalid_argument("lerRegistro(FI, linha ( " + line + ") ): \n" + std::string(erro.what())); }

				}//if (lerRegistro("FI", line.substr(0, 2), a_registro)) {

				// RESTRICOES ELETRICAS ESPECIAIS - CONTRATOS
				if (lerRegistro("FE", line.substr(0, 2), a_registro)) {
					try {
						const Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(8, 2), line.substr(11, 2), line.substr(14, 1));
						const Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(16, 2), line.substr(19, 2), line.substr(22, 2));

						const IdRestricaoEletrica idRestricaoEletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_restricao_eletrica, std::stoi(line.substr(4, 3)));

						if (idRestricaoEletrica_inicializado == IdRestricaoEletrica_Nenhum)
							throw std::invalid_argument("Nao inicializada idRestricaoEletrica  com codigo_ONS_" + line.substr(4, 3));

						const IdRestricaoEletrica  idRestricaoEletrica = idRestricaoEletrica_inicializado;

						const IdContrato idContrato_inicializado = getIdFromCodigoONS(lista_codigo_ONS_contrato, std::stoi(line.substr(24, 3)));

						if (idContrato_inicializado == IdContrato_Nenhum)
							throw std::invalid_argument("Nao inicializada idContrato com codigo_ONS_" + std::stoi(line.substr(24, 3)));

						const IdContrato  idContrato = idContrato_inicializado;

						idElementoSistema = IdElementoSistema(a_dados.getMaiorId(idRestricaoEletrica, IdElementoSistema()) + 1);

						const double fator_participacao = std::stod(line.substr(34, 10));

						ElementoSistema elementoSistema;
						elementoSistema.setAtributo(AttComumElementoSistema_idElementoSistema, idElementoSistema);
						elementoSistema.setAtributo(AttComumElementoSistema_tipo_elemento, TipoElementoSistema_contrato);
						elementoSistema.setAtributo(AttComumElementoSistema_contrato, idContrato);
						elementoSistema.setAtributo(AttComumElementoSistema_fator_participacao, fator_participacao);
						a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.add(elementoSistema);

						if (a_dados.getSizeVetor(idRestricaoEletrica, idElementoSistema, AttVetorElementoSistema_fator_participacao) == 0)
							a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.att(idElementoSistema).setVetor(AttVetorElementoSistema_fator_participacao, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0.0));

						for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo))
							a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.att(idElementoSistema).setElemento(AttVetorElementoSistema_fator_participacao, periodo, fator_participacao);
					}
					catch (const std::exception& erro) { throw std::invalid_argument("lerRegistro(FE" + line + "): \n" + std::string(erro.what())); }

				}//if (lerRegistro("FE", line.substr(0, 2), a_registro)) {

				// RESTRICOES ELETRICAS ESPECIAIS - RENOVAVEIS
				if (lerRegistro("FR", line.substr(0, 2), a_registro)) {
					try {
						std::string horaFinal = "0";
						std::string minFinal = "0";

						if (line.size() > 20) { horaFinal = line.substr(21, 2); }
						if (line.size() > 23) { minFinal = line.substr(24, 1); }

						const Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(10, 2), line.substr(13, 2), line.substr(16, 1));
						const Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(18, 2), horaFinal, minFinal);

						const IdRestricaoEletrica idRestricaoEletrica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_restricao_eletrica, std::stoi(line.substr(4, 5)));

						if (idRestricaoEletrica_inicializado == IdRestricaoEletrica_Nenhum)
							throw std::invalid_argument("Nao inicializada idRestricaoEletrica  com codigo_ONS_" + line.substr(4, 5));

						const IdRestricaoEletrica  idRestricaoEletricaFE = idRestricaoEletrica_inicializado;

						idElementoSistema = IdElementoSistema(a_dados.getMaiorId(idRestricaoEletricaFE, IdElementoSistema()) + 1);

						const IdUsinaEolica idUsinaEolica_inicializado = getIdFromCodigoONS(lista_codigo_ONS_usina_eolica, std::stoi(line.substr(26, 5)));

						IdUsinaEolica idUsinaEolica;

						if (idUsinaEolica_inicializado != IdUsinaEolica_Nenhum) {
							idUsinaEolica = idUsinaEolica_inicializado;
						}//if (idUsinaEolicaInstanciada.size() != 0) { 

						else {
							idUsinaEolica = a_dados.vetorUsinaEolica.getMaiorId();
							if (idUsinaEolica == IdUsinaEolica_Nenhum) { idUsinaEolica = IdUsinaEolica_1; }
							else { idUsinaEolica++; }

							UsinaEolica usinaEolica;
							usinaEolica.setAtributo(AttComumUsinaEolica_idUsinaEolica, idUsinaEolica);

							lista_codigo_ONS_usina_eolica.addElemento(idUsinaEolica, std::stoi(line.substr(26, 5)));

							a_dados.vetorUsinaEolica.add(usinaEolica);

						}// else

						const double fator_participacao = std::stod(line.substr(34, 10));

						ElementoSistema elementoSistema;
						elementoSistema.setAtributo(AttComumElementoSistema_idElementoSistema, idElementoSistema);
						elementoSistema.setAtributo(AttComumElementoSistema_tipo_elemento, TipoElementoSistema_eolica);
						elementoSistema.setAtributo(AttComumElementoSistema_eolica, idUsinaEolica);
						elementoSistema.setAtributo(AttComumElementoSistema_fator_participacao, fator_participacao);
						a_dados.vetorRestricaoEletrica.att(idRestricaoEletricaFE).vetorElementoSistema.add(elementoSistema);
						a_dados.vetorRestricaoEletrica.att(idRestricaoEletricaFE).vetorElementoSistema.att(idElementoSistema).setVetor(AttVetorElementoSistema_fator_participacao, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0.0));

						for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo))
							a_dados.vetorRestricaoEletrica.att(idRestricaoEletricaFE).vetorElementoSistema.att(idElementoSistema).setElemento(AttVetorElementoSistema_fator_participacao, periodo, fator_participacao);
					}
					catch (const std::exception& erro) { throw std::invalid_argument("lerRegistro(FR, linha ( " + line + ") ): \n" + std::string(erro.what())); }

				}//if (lerRegistro("FR", line.substr(0, 2), a_registro)) {

				// DEMANDA ESPECIAL
				if (line.substr(0, 2) == "DE") {
					try {
						strNormalizada(line);

						if (line.size() > 44) {
							if (line.substr(36, 9) == std::string("CARGA.IPU"))
								int aa = 1;
						}

						const Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(8, 2), line.substr(11, 2), line.substr(14, 1));
						const Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(16, 2), line.substr(19, 2), line.substr(22, 1));
						std::vector<IdDemandaEspecial>vectorIdDemanda = a_dados.vetorDemandaEspecial.getIdObjetos(AttComumDemandaEspecial_numero_demanda, int(std::stoi(line.substr(4, 3))));

						IdDemandaEspecial idDemanda;
						if (!vectorIdDemanda.empty()) { idDemanda = vectorIdDemanda.at(0); }
						else {
							idDemanda = IdDemandaEspecial(a_dados.getMaiorId(IdDemandaEspecial()) + 1);
							DemandaEspecial demandaEspecial;
							demandaEspecial.setAtributo(AttComumDemandaEspecial_idDemandaEspecial, idDemanda);
							demandaEspecial.setAtributo(AttComumDemandaEspecial_numero_demanda, int(std::stoi(line.substr(4, 3))));
							demandaEspecial.setMatriz(AttMatrizDemandaEspecial_demanda, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(a_horizonte_estudo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(IdPatamarCarga_1, 0.0))));
							a_dados.vetorDemandaEspecial.add(demandaEspecial);
						}

						for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo))
							a_dados.vetorDemandaEspecial.att(idDemanda).setElemento(AttMatrizDemandaEspecial_demanda, periodo, IdPatamarCarga_1, double(std::stod(line.substr(24, 10))));
					}
					catch (const std::exception& erro) { throw std::invalid_argument("lerRegistro(DE , linha ( " + line + ") ): \n" + std::string(erro.what())); }
				}

				// RESTRICOES ELETRICAS ESPECIAIS - DEMANDA/CARGA
				if (lerRegistro("FC", line.substr(0, 2), a_registro)) {
					try {

						std::string horaFinal = "0";
						std::string minFinal = "0";

						if (line.size() > 20) { horaFinal = line.substr(21, 2); }
						if (line.size() > 23) { minFinal = line.substr(24, 1); }

						const Periodo periodo_inicial_restricao = getPeriodoInicial(a_dados, line.substr(10, 2), line.substr(13, 2), line.substr(16, 1));
						const Periodo periodo_final_restricao = getPeriodoFinal(a_dados, line.substr(18, 2), horaFinal, minFinal);

						const IdRestricaoEletrica idRestricaoEletrica = getIdFromCodigoONS(lista_codigo_ONS_restricao_eletrica, std::stoi(line.substr(4, 3)));

						if (idRestricaoEletrica == IdRestricaoEletrica_Nenhum)
							throw std::invalid_argument("Nao inicializada idRestricaoEletrica  com codigo_ONS_" + line.substr(4, 3));

						const IdDemandaEspecial idDemanda = a_dados.vetorDemandaEspecial.getIdObjetos(AttComumDemandaEspecial_numero_demanda, int(std::stoi(line.substr(26, 3)))).at(0);

						if (idDemanda == IdDemandaEspecial_Nenhum)
							throw std::invalid_argument("Nao inicializada IdDemandaEspecial_" + std::stoi(line.substr(26, 3)));

						idElementoSistema = IdElementoSistema(a_dados.getMaiorId(idRestricaoEletrica, IdElementoSistema()) + 1);

						const double fator_participacao = std::stod(line.substr(36, 10));

						ElementoSistema elementoSistema;
						elementoSistema.setAtributo(AttComumElementoSistema_idElementoSistema, idElementoSistema);
						elementoSistema.setAtributo(AttComumElementoSistema_tipo_elemento, TipoElementoSistema_hidreletrica);
						elementoSistema.setAtributo(AttComumElementoSistema_demandaEspecial, idDemanda);
						elementoSistema.setAtributo(AttComumElementoSistema_fator_participacao, fator_participacao);
						a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.add(elementoSistema);

						a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.att(idElementoSistema).setVetor(AttVetorElementoSistema_fator_participacao, SmartEnupla<Periodo, double>(a_horizonte_estudo, 0.0));

						for (Periodo periodo = getPeriodoInicialResticao(periodo_inicial_restricao, a_horizonte_estudo); periodo <= getPeriodoFinalResticao(periodo_final_restricao, a_horizonte_estudo); a_horizonte_estudo.incrementarIterador(periodo))
							a_dados.vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.att(idElementoSistema).setElemento(AttVetorElementoSistema_fator_participacao, periodo, fator_participacao);
					}
					catch (const std::exception& erro) { throw std::invalid_argument("lerRegistro(FC " + line + "): \n" + std::string(erro.what())); }

				}//	if (lerRegistro("FC", line.substr(0, 2), a_registro)) {


			}// while (std::getline(myfile, line))

			myfile.close();


		} // if (myfile.is_open()) 


		else  throw std::invalid_argument("Nao foi possivel abrir o arquivo" + a_nomeArquivo + ".");

	} //try 

	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::leitura_REE_201904_DES16: \n" + std::string(erro.what())); }
}


void LeituraCEPEL::validacoes_DESSEM(Dados& a_dados, const std::string a_diretorio) {

	try {
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		EntradaSaidaDados entradaSaidaDados;

		entradaSaidaDados.setDiretorioEntrada(a_dados.getAtributo(AttComumDados_diretorio_entrada_dados, std::string()));
		entradaSaidaDados.setDiretorioSaida(a_dados.getAtributo(AttComumDados_diretorio_saida_dados, std::string()));

		const TipoEstudo tipo_estudo = a_dados.getAtributo(AttComumDados_tipo_estudo, TipoEstudo());

		const std::string diretorio_att_operacionais = entradaSaidaDados.getDiretorioEntrada() + "";

		std::string diretorio_att_premissas = "";
		std::string diretorio_exportacao_pos_estudo = "";

		if (tipo_estudo == TipoEstudo_simulacao) {
			diretorio_att_premissas = entradaSaidaDados.getDiretorioEntrada() + "//Simulacao//AtributosPremissasDESSEM//";
			diretorio_exportacao_pos_estudo = entradaSaidaDados.getDiretorioSaida() + "//Simulacao//AcoplamentoPosEstudo";
		}
		else {
			diretorio_att_premissas = entradaSaidaDados.getDiretorioEntrada() + "//Otimizacao//AtributosPremissasDESSEM";
			diretorio_exportacao_pos_estudo = entradaSaidaDados.getDiretorioSaida() + "//Otimizacao//AcoplamentoPosEstudo";
		}

		if (strCompara(a_dados.getAtributo(AttComumDados_diretorio_importacao_pos_estudo, std::string()), "nenhum"))
			diretorio_exportacao_pos_estudo = "nenhum";

		const bool imprimir_att_operacionais_sem_recarregar = true;

		a_dados.validacaoDadosAttComum();

		a_dados.setAtributo(AttComumDados_diretorio_entrada_dados, entradaSaidaDados.getDiretorioEntrada());

		a_dados.setAtributo(AttComumDados_diretorio_saida_dados, entradaSaidaDados.getDiretorioSaida());

		a_dados.validacao_operacional_Dados(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		leitura_vRef_from_CadUsH_csv(a_dados, a_diretorio + "//CadUsH.csv");
		calculaEngolimentoMaximo(a_dados, horizonte_estudo, Periodo(horizonte_estudo.getIteradorFinal() + 1), false);

		a_dados.validacao_operacional_Submercado(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		a_dados.validacao_operacional_Intercambio(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		//a_dados.validacao_operacional_Termeletrica(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		//a_dados.validacao_operacional_Hidreletrica(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		//a_dados.validacao_operacional_RestricaoEletrica(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		a_dados.validacao_operacional_Contrato(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		a_dados.validacao_operacional_DemandaEspecial(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		a_dados.validacao_operacional_Eolica(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		//a_dados.validacao_operacional_ProcessoEstocasticoHidrologico(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, diretorio_exportacao_pos_estudo, imprimir_att_operacionais_sem_recarregar);

		//a_dados.validacao_operacional_Intercambio_Hidraulico(entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

	}//try {
	catch (const std::exception& erro) { throw std::invalid_argument("LeituraCEPEL::validacoes_DESSSEM: \n" + std::string(erro.what())); }

} // void LeituraCEPEL::validacoes_DESSEM(Dados & a_dados){

