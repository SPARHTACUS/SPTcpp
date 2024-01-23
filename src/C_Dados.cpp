#include "C_Dados.h"

#include "C_EntradaSaidaDados.h"
#include <chrono>
#include "mpi.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Dados::Dados() {
	try {
		
		volume_inicial_carregado_from_premissa = false;
		volume_inicial_carregado_from_operacional = false;
		
		INICIALIZA_SMART_ELEMENTO(Dados, SMART_ELEMENTO_DADOS) 
	
	}
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::Dados(): \n" + std::string(erro.what())); }
}

Dados::Dados(const Dados& instanciaCopiar) {
	try { COPIA_SMART_ELEMENTO(Dados, SMART_ELEMENTO_DADOS) }
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::Dados(const Dados &instanciaCopiar): \n" + std::string(erro.what())); }
}

void Dados::esvaziar() {
	try { EXCLUI_SMART_ELEMENTO(Dados, SMART_ELEMENTO_DADOS) }
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::esvaziar(): \n" + std::string(erro.what())); }
}

Dados::~Dados() {
	EXCLUI_SMART_ELEMENTO(Dados, SMART_ELEMENTO_DADOS)
}



Periodo Dados::getMenorPeriodoHorizonteEstudo() const {

	try {

		const IdEstagio estagio_inicial = getAtributo(AttComumDados_estagio_inicial, IdEstagio());
		const IdEstagio estagio_final = getAtributo(AttComumDados_estagio_final, IdEstagio());

		if (estagio_inicial > estagio_final)
			throw std::invalid_argument("O valor de " + getFullString(AttComumDados_estagio_inicial) + " nao pode ser maior que " + getFullString(AttComumDados_estagio_final));

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_inicial_horizonte_estudo = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_horizonte_estudo = horizonte_estudo.getIteradorFinal();

		Periodo periodo_inicial = periodo_final_horizonte_estudo;
		const IdEstagio estagio_anterior = IdEstagio(estagio_inicial - 1);
		for (Periodo periodo = periodo_final_horizonte_estudo; periodo >= periodo_inicial_horizonte_estudo; horizonte_estudo.decrementarIterador(periodo)) {
			if (horizonte_estudo.getElemento(periodo) == estagio_anterior)
				return periodo_inicial;
			periodo_inicial = periodo;
		}

		return periodo_inicial_horizonte_estudo;

	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("Dados::getMenorPeriodoHorizonteEstudo(): \n" + std::string(erro.what())); }

} // Periodo Dados::getMenorPeriodoHorizonteEstudo() const{

Periodo Dados::getMaiorPeriodoHorizonteEstudo() const {

	try {

		const IdEstagio estagio_inicial = getAtributo(AttComumDados_estagio_inicial, IdEstagio());
		const IdEstagio estagio_final = getAtributo(AttComumDados_estagio_final, IdEstagio());

		if (estagio_inicial > estagio_final)
			throw std::invalid_argument("O valor de " + getFullString(AttComumDados_estagio_inicial) + " nao pode ser maior que " + getFullString(AttComumDados_estagio_final));

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_inicial_horizonte_estudo = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_horizonte_estudo = horizonte_estudo.getIteradorFinal();

		Periodo periodo_final = periodo_inicial_horizonte_estudo;
		const IdEstagio estagio_posterior = IdEstagio(estagio_final + 1);
		for (Periodo periodo = periodo_inicial_horizonte_estudo; periodo <= periodo_final_horizonte_estudo; horizonte_estudo.incrementarIterador(periodo)) {
			if (horizonte_estudo.getElemento(periodo) == estagio_posterior)
				return periodo_final;
			periodo_final = periodo;
		}

		return periodo_final_horizonte_estudo;

	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("Dados::getMaiorPeriodoHorizonteEstudo(): \n" + std::string(erro.what())); }

} // Periodo Dados::getMaiorPeriodoHorizonteEstudo() const{


void Dados::carregarArquivosEntrada(EntradaSaidaDados& a_entradaSaidaDados) {
	try {

		const IdProcesso idProcesso = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());
		const IdProcesso maior_processo = arranjoResolucao.getMaiorId(IdProcesso());
		const int numero_processos_paralelos = int(arranjoResolucao.getMaiorId(IdProcesso()));


		if (true) {
			int barreira = 0;

			if (arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso()) == IdProcesso_mestre) {
				for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++)
					MPI_Send(&barreira, 1, MPI_INT, getRank(idProcesso), 0, MPI_COMM_WORLD);
			}
			else
				MPI_Recv(&barreira, 1, MPI_INT, getRank(IdProcesso_mestre), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}


		const bool imprimir_att_operacionais_sem_recarregar = false;

		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//                                                                        Carregar Arquivos de Dados
		//
		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		a_entradaSaidaDados.carregarArquivoCSV_AttComum("DADOS_AttComumOperacional.csv", *this, TipoAcessoInstancia_direto);

		if (true) {
			std::string diretorioSaida = a_entradaSaidaDados.getDiretorioSaida();
			const std::string diretorioEntrada = a_entradaSaidaDados.getDiretorioEntrada();
			if (!strCompara(diretorioEntrada, getAtributo(AttComumDados_diretorio_entrada_dados, std::string())))
				throw std::invalid_argument("O diretorio de entrada nao pode ser alterado via arquivo de configuracao.");
			else if (!strCompara(diretorioSaida, getAtributo(AttComumDados_diretorio_saida_dados, std::string()))) {
				diretorioSaida = getAtributo(AttComumDados_diretorio_saida_dados, std::string());
				a_entradaSaidaDados = EntradaSaidaDados(false, false, diretorioEntrada, diretorioSaida, a_entradaSaidaDados.getFimDeArquivo(), a_entradaSaidaDados.getSeparadorCSV());
			}
		} // if (true) {


		const TipoEstudo tipo_estudo = getAtributo(AttComumDados_tipo_estudo, TipoEstudo());

		std::string diretorio_att_premissas = "";
		std::string diretorio_att_operacionais = "";

		std::string diretorio_exportacao_pos_estudo = "";

		if (tipo_estudo == TipoEstudo_simulacao) {
			diretorio_att_premissas = a_entradaSaidaDados.getDiretorioEntrada() + "//Simulacao//AtributosPremissasSPT//";
			diretorio_att_operacionais = a_entradaSaidaDados.getDiretorioEntrada() + "//Simulacao//AtributosOperacionaisSPT";
			diretorio_exportacao_pos_estudo = a_entradaSaidaDados.getDiretorioSaida() + "//Simulacao//AcoplamentoPosEstudo";
		}
		else {
			diretorio_att_premissas = a_entradaSaidaDados.getDiretorioEntrada() + "//Otimizacao//AtributosPremissasSPT";
			diretorio_att_operacionais = a_entradaSaidaDados.getDiretorioEntrada() + "//Otimizacao//AtributosOperacionaisSPT";
			diretorio_exportacao_pos_estudo = a_entradaSaidaDados.getDiretorioSaida() + "//Otimizacao//AcoplamentoPosEstudo";
		}

		if (strCompara(getAtributo(AttComumDados_diretorio_importacao_pos_estudo, std::string()), "nenhum"))
			diretorio_exportacao_pos_estudo = "nenhum";


		a_entradaSaidaDados.carregarArquivoCSV_AttVetor("DADOS_AttVetorOperacional_PorPeriodo.csv", *this, TipoAcessoInstancia_direto);
		a_entradaSaidaDados.carregarArquivoCSV_AttVetor("DADOS_AttVetorOperacional_PorIdEstagio.csv", *this, TipoAcessoInstancia_direto);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz("DADOS_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_direto);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("DADOS_" + getString(AttMatrizDados_desagio_acumulado_horizonte_estudo) + ".csv", *this, TipoAcessoInstancia_direto);

		validacao_operacional_Dados(a_entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);


		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//                                                                        Carregar Arquivos de Submercado
		//
		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		a_entradaSaidaDados.carregarArquivoCSV_AttComum("SUBMERCADO_AttComumOperacional.csv", *this, TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("SUBMERCADO_AttVetorPremissa_PorPeriodo.csv", *this, TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("SUBMERCADO_AttMatrizPremissa_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);
		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("SUBMERCADO_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);

		// Patamar Deficit

		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("SUBMERCADO_PATAMAR_DEFICIT_AttComumOperacional.csv", *this, TipoAcessoInstancia_membroMembro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("SUBMERCADO_PATAMAR_DEFICIT_AttVetorPremissa_PorPeriodo.csv", *this, TipoAcessoInstancia_membroMembro);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("SUBMERCADO_PATAMAR_DEFICIT_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membroMembro);

		// Usina Nao Simulada

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("SUBMERCADO_USINA_NAO_SIMULADA_AttVetorPremissa_PorPeriodo.csv", *this, TipoAcessoInstancia_membroMembro);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("SUBMERCADO_USINA_NAO_SIMULADA_AttMatrizPremissa_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membroMembro);
		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("SUBMERCADO_USINA_NAO_SIMULADA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membroMembro);

		validacao_operacional_Submercado(a_entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//                                                                        Carregar Arquivos de Intercambio
		//
		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		a_entradaSaidaDados.carregarArquivoCSV_AttComum("INTERCAMBIO_AttComumOperacional.csv", *this, TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("INTERCAMBIO_AttVetorPremissa_PorPeriodo.csv", *this, TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("INTERCAMBIO_AttMatrizPremissa_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);
		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("INTERCAMBIO_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);

		validacao_operacional_Intercambio(a_entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);


		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//                                                                        Carregar Arquivos de Termeletrica
		//
		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		a_entradaSaidaDados.carregarArquivoCSV_AttComum("TERMELETRICA_AttComumOperacional.csv", *this, TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("TERMELETRICA_AttVetorPremissa_PorPeriodo.csv", *this, TipoAcessoInstancia_membro);
		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("TERMELETRICA_AttVetorOperacional_PorPeriodo.csv", *this, TipoAcessoInstancia_membro);
		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("TERMELETRICA_UNIDADE_AttVetorPremissa_PorInteiro.csv", *this, TipoAcessoInstancia_membroMembro);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("TERMELETRICA_AttMatrizPremissa_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);
		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("TERMELETRICA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("TERMELETRICA_COMANDO_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);

		if (tipo_estudo == TipoEstudo_simulacao)
			a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("TERMELETRICA_AttMatrizOperacional_PorCenarioPorPeriodo.csv", *this, TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("TERMELETRICA_UNIDADE_AttComumOperacional.csv", *this, TipoAcessoInstancia_membroMembro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("TERMELETRICA_UNIDADE_AttVetorOperacional_PorPeriodo.csv", *this, TipoAcessoInstancia_membroMembro);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("TERMELETRICA_UNIDADE_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membroMembro);


		validacao_operacional_Termeletrica(a_entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);


		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//                                                                        Carregar Arquivos de Hidreletrica
		//
		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


		a_entradaSaidaDados.carregarArquivoCSV_AttComum("HIDRELETRICA_AttComumOperacional.csv", *this, TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("HIDRELETRICA_AttVetorPremissa_PorPeriodo.csv", *this, TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("HIDRELETRICA_AttVetorOperacional_PorPeriodo.csv", *this, TipoAcessoInstancia_membro);
		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("HIDRELETRICA_AttVetorOperacional_PorEstacao.csv", *this, TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("HIDRELETRICA_AttMatrizPremissa_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);
		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("HIDRELETRICA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);

		if (tipo_estudo == TipoEstudo_simulacao)
			a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("HIDRELETRICA_AttMatrizOperacional_PorCenarioPorPeriodo.csv", *this, TipoAcessoInstancia_membro);


		// Afluencia
		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("HIDRELETRICA_AFLUENCIA_AttVetorPremissa_PorPeriodo.csv", *this, TipoAcessoInstancia_membroMembro);

		std::string diretorio_importacao_pre_estudo = getAtributo(AttComumDados_diretorio_importacao_pre_estudo, std::string());
		if (!strCompara(diretorio_importacao_pre_estudo, "nenhum"))
			a_entradaSaidaDados.setDiretorioEntrada(diretorio_importacao_pre_estudo);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("HIDRELETRICA_AFLUENCIA_AttMatrizPremissa_PorCenarioPorPeriodo.csv", *this, TipoAcessoInstancia_membroMembro);

		a_entradaSaidaDados.setDiretorioEntrada(getAtributo(AttComumDados_diretorio_entrada_dados, std::string()));

		// Reservatorio

		volume_inicial_carregado_from_operacional = a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("HIDRELETRICA_RESERVATORIO_AttComumOperacional.csv", *this, TipoAcessoInstancia_membroMembro);
		volume_inicial_carregado_from_premissa    = a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("HIDRELETRICA_RESERVATORIO_AttComumPremissa.csv", *this, TipoAcessoInstancia_membroMembro);

		if (getAtributo(AttComumDados_estagio_inicial, IdEstagio()) == IdEstagio_1) {
			if (volume_inicial_carregado_from_operacional && volume_inicial_carregado_from_premissa)
				throw std::invalid_argument("Apenas um dos arquivos HIDRELETRICA_RESERVATORIO_AttComumOperacional.csv, HIDRELETRICA_RESERVATORIO_AttComumPremissa.csv deve ser carregado.");
			else if (!volume_inicial_carregado_from_operacional && !volume_inicial_carregado_from_premissa)
				throw std::invalid_argument("Ao menos um dos arquivos HIDRELETRICA_RESERVATORIO_AttComumOperacional.csv, HIDRELETRICA_RESERVATORIO_AttComumPremissa.csv deve ser carregado.");
		}
		else {
			if (volume_inicial_carregado_from_operacional || volume_inicial_carregado_from_premissa)
				throw std::invalid_argument("Nenhum dos arquivos HIDRELETRICA_RESERVATORIO_AttComumOperacional.csv, HIDRELETRICA_RESERVATORIO_AttComumPremissa.csv deve ser carregado quando " + getFullString(AttComumDados_estagio_inicial) + " for maior que " + getFullString(IdEstagio_1));
		}

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("HIDRELETRICA_RESERVATORIO_AttVetorPremissa_PorPeriodo.csv", *this, TipoAcessoInstancia_membroMembro);
		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("HIDRELETRICA_RESERVATORIO_AttVetorPremissa_PorIdMes.csv", *this, TipoAcessoInstancia_membroMembro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("HIDRELETRICA_RESERVATORIO_AttVetorOperacional_PorPeriodo.csv", *this, TipoAcessoInstancia_membroMembro);

		if (tipo_estudo == TipoEstudo_simulacao)
			a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("HIDRELETRICA_RESERVATORIO_AttMatrizOperacional_PorCenarioPorPeriodo.csv", *this, TipoAcessoInstancia_membroMembro);

		// Conjunto Hidraulico

		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("HIDRELETRICA_CONJUNTO_AttComumOperacional.csv", *this, TipoAcessoInstancia_membroMembro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("HIDRELETRICA_CONJUNTO_AttVetorPremissa_PorPeriodo.csv", *this, TipoAcessoInstancia_membroMembro);
		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("HIDRELETRICA_CONJUNTO_AttVetorOperacional_PorPeriodo.csv", *this, TipoAcessoInstancia_membroMembro);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("HIDRELETRICA_CONJUNTO_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membroMembro);

		// Unidade Hidraulica

		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("HIDRELETRICA_CONJUNTO_UNIDADE_AttComumOperacional.csv", *this, TipoAcessoInstancia_membroMembroMembro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("HIDRELETRICA_CONJUNTO_UNIDADE_AttVetorPremissa_PorPeriodo.csv", *this, TipoAcessoInstancia_membroMembroMembro);
		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("HIDRELETRICA_CONJUNTO_UNIDADE_AttVetorOperacional_PorPeriodo.csv", *this, TipoAcessoInstancia_membroMembroMembro);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("HIDRELETRICA_CONJUNTO_UNIDADE_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membroMembroMembro);


		//FPH
		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("HIDRELETRICA_FPH_AttVetorPremissa_PorInteiro.csv", *this, TipoAcessoInstancia_membroMembro);
		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("HIDRELETRICA_FPH_AttMatrizOperacional_PorPeriodoPorInteiro.csv", *this, TipoAcessoInstancia_membroMembro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("HIDRELETRICA_CONJUNTO_FPH_AttVetorPremissa_PorInteiro.csv", *this, TipoAcessoInstancia_membroMembroMembro);
		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("HIDRELETRICA_CONJUNTO_FPH_AttMatrizOperacional_PorPeriodoPorInteiro.csv", *this, TipoAcessoInstancia_membroMembroMembro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("HIDRELETRICA_CONJUNTO_UNIDADE_FPH_AttVetorPremissa_PorInteiro.csv", *this, TipoAcessoInstancia_membroMembroMembroMembro);
		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("HIDRELETRICA_CONJUNTO_UNIDADE_FPH_AttMatrizOperacional_PorPeriodoPorInteiro.csv", *this, TipoAcessoInstancia_membroMembroMembroMembro);

		//Deflu�ncia passada
		if (getAtributo(AttComumDados_estagio_inicial, IdEstagio()) == IdEstagio_1)
			a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("HIDRELETRICA_DEFLUENCIA_AttVetorOperacional_PorPeriodo.csv", *this, TipoAcessoInstancia_membroMembro);
		else {
			if (a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("HIDRELETRICA_DEFLUENCIA_AttVetorOperacional_PorPeriodo.csv", *this, TipoAcessoInstancia_membroMembro))
				throw std::invalid_argument("O arquivo HIDRELETRICA_DEFLUENCIA_AttVetorOperacional_PorPeriodo.csv nao deve ser carregado quando " + getFullString(AttComumDados_estagio_inicial) + " for maior que " + getFullString(IdEstagio_1));
		}

		validacao_operacional_Hidreletrica(a_entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);


		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//                                                                        Carregar Arquivos de Bacia Hidrografica
		//
		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("BACIA_HIDROGRAFICA_AttComumOperacional.csv", *this, TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("BACIA_HIDROGRAFICA_AttVetorOperacional_PorInteiro.csv", *this, TipoAcessoInstancia_membro);

		validacao_operacional_BaciaHidrografica(a_entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//                                                                        Carregar Arquivos de Restricao Eletrica
		//
		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("RESTRICAO_ELETRICA_AttComumOperacional.csv", *this, TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("RESTRICAO_ELETRICA_AttVetorPremissa_PorPeriodo.csv", *this, TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("RESTRICAO_ELETRICA_AttMatrizPremissa_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);
		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("RESTRICAO_ELETRICA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);

		// ElementoSistema

		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("RESTRICAO_ELETRICA_ELEMENTO_SISTEMA_AttComumOperacional.csv", *this, TipoAcessoInstancia_membroMembro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("RESTRICAO_ELETRICA_ELEMENTO_SISTEMA_AttVetorPremissa_PorPeriodo.csv", *this, TipoAcessoInstancia_membroMembro);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("RESTRICAO_ELETRICA_ELEMENTO_SISTEMA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membroMembro);

		validacao_operacional_RestricaoEletrica(a_entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//                                                                        Carregar Arquivos de Intercambio Hidraulico
		//
		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("INTERCAMBIO_HIDRAULICO_AttComumOperacional.csv", *this, TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("INTERCAMBIO_HIDRAULICO_AttVetorOperacional_PorInteiro.csv", *this, TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("INTERCAMBIO_HIDRAULICO_AttVetorOperacional_PorPeriodo.csv", *this, TipoAcessoInstancia_membro);

		validacao_operacional_Intercambio_Hidraulico(a_entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);



		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//                                                                        Carregar Arquivos de Restricao Hidraulica Especial
		//
		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("RESTRICAO_OPERATIVA_UHE_AttComumOperacional.csv", *this, TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("RESTRICAO_OPERATIVA_UHE_AttVetorOperacional_PorPeriodo.csv", *this, TipoAcessoInstancia_membro);
		
		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("RESTRICAO_OPERATIVA_UHE_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);

		// ElementoSistema

		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("RESTRICAO_OPERATIVA_UHE_ELEMENTO_SISTEMA_AttComumOperacional.csv", *this, TipoAcessoInstancia_membroMembro);
		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("RESTRICAO_OPERATIVA_UHE_ELEMENTO_SISTEMA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membroMembro);

		validacao_operacional_RestricaoOperativaUHE(a_entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);



		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//                                                                        Carregar Arquivos de Agrupamento Intercambio
		//
		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("AGRUPAMENTO_INTERCAMBIO_AttVetorOperacional_PorInteiro.csv", *this, TipoAcessoInstancia_membro);
		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("AGRUPAMENTO_INTERCAMBIO_AttVetorPremissa_PorPeriodo.csv", *this, TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("AGRUPAMENTO_INTERCAMBIO_AttMatrizPremissa_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);
		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("AGRUPAMENTO_INTERCAMBIO_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);

		validacao_operacional_AgrupamentoIntercambio(a_entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//                                                                        Carregar Arquivos de Usinas Elevat�rias
		//
		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("USINA_ELEVATORIA_AttComumOperacional.csv", *this, TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("USINA_ELEVATORIA_AttVetorOperacional_PorPeriodo.csv", *this, TipoAcessoInstancia_membro);

		validacao_operacional_UsinasElevatorias(a_entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);

		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//		//
		//                                                                        Carregar Arquivos de Contratos
		//
		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("CONTRATO_AttComumOperacional.csv", *this, TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("CONTRATO_AttVetorOperacional_PorPeriodo.csv", *this, TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("CONTRATO_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);

		validacao_operacional_Contrato(a_entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);


		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//		//
		//                                                                        Carregar Arquivos de Demanda Especial
		//
		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("DEMANDA_ESPECIAL_AttComumOperacional.csv", *this, TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("DEMANDA_ESPECIAL_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);

		validacao_operacional_DemandaEspecial(a_entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);


		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//		//
		//                                                                        Carregar Arquivos de Usinas Eolicas
		//
		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("USINA_EOLICA_AttComumOperacional.csv", *this, TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("USINA_EOLICA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);

		validacao_operacional_Eolica(a_entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar);


		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//                                                                        Carregar Arquivo Mapeamento Cenario e Abertura por Processo Por Iteracao
		//
		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


		bool mapeamento_cenarios_aberturas = a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("ESTRUTURA_RESOLUCAO_AttMatrizOperacional_PorCenarioPorEstagio.csv", *this, TipoAcessoInstancia_membroMembroMembro);

		if (!mapeamento_cenarios_aberturas)
			definirCenariosPorProcessosEmArranjoResolucao();

		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//                                                                        Carregar Arquivos de Processo Estocastico Otimizacao
		//
		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		const std::string diretorio_entrada = a_entradaSaidaDados.getDiretorioEntrada();

		a_entradaSaidaDados.setDiretorioEntrada(diretorio_entrada + "//ProcessoEstocasticoHidrologico");

		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("PROCESSO_ESTOCASTICO_AttComumOperacional.csv", processoEstocastico_hidrologico, TipoAcessoInstancia_direto);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("PROCESSO_ESTOCASTICO_" + getString(AttMatrizProcessoEstocastico_probabilidade_realizacao) + ".csv", processoEstocastico_hidrologico, TipoAcessoInstancia_direto);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("PROCESSO_ESTOCASTICO_" + getString(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral) + ".csv", processoEstocastico_hidrologico, TipoAcessoInstancia_direto);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("VARIAVEL_ALEATORIA_" + getString(AttVetorVariavelAleatoria_tipo_relaxacao) + ".csv", processoEstocastico_hidrologico, TipoAcessoInstancia_membro);
		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("VARIAVEL_ALEATORIA_" + getString(AttMatrizVariavelAleatoria_residuo_espaco_amostral) + ".csv", processoEstocastico_hidrologico, TipoAcessoInstancia_membro);
		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("VARIAVEL_ALEATORIA_" + getString(AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao) + ".csv", processoEstocastico_hidrologico, TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("VARIAVEL_ALEATORIA_INTERNA_AttComumOperacional.csv", processoEstocastico_hidrologico, TipoAcessoInstancia_membroMembro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("VARIAVEL_ALEATORIA_INTERNA_" + getString(AttVetorVariavelAleatoriaInterna_coeficiente_participacao) + ".csv", processoEstocastico_hidrologico, TipoAcessoInstancia_membroMembro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("VARIAVEL_ALEATORIA_INTERNA_" + getString(AttVetorVariavelAleatoriaInterna_tendencia_temporal) + ".csv", processoEstocastico_hidrologico, TipoAcessoInstancia_membroMembro);

		a_entradaSaidaDados.setDiretorioEntrada(diretorio_entrada);

		validacao_operacional_ProcessoEstocasticoHidrologico(a_entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, diretorio_exportacao_pos_estudo, imprimir_att_operacionais_sem_recarregar);

		//validacao_mapeamento_aberturas(a_entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar, mapeamento_cenarios_aberturas);

		a_entradaSaidaDados.setDiretorioSaida(getAtributo(AttComumDados_diretorio_saida_dados, std::string()));

		if (!mapeamento_cenarios_aberturas)
			mapearCenariosAberturasPorIteracaoEmArranjoResolucao();

		//
		// Esvazia Atributos Processo Estocástico Hidrológico
		//

		processoEstocastico_hidrologico = ProcessoEstocastico();

		validacao_mapeamento_cenarios_aberturas(a_entradaSaidaDados, diretorio_att_operacionais, diretorio_att_premissas, imprimir_att_operacionais_sem_recarregar, mapeamento_cenarios_aberturas);

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::carregarArquivosEntrada(a_entradaSaidaDados): \n" + std::string(erro.what())); }
} // void Dados::carregarArquivosEntrada(){




SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> Dados::getHorizonteEspacoAmostralHidrologico(const IdEstagio a_idEstagioInicial, const IdEstagio a_idEstagioFinal)const {

	try {

		if (a_idEstagioInicial > a_idEstagioFinal)
			throw std::invalid_argument("Estagio inicial maior que final.");

		SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> horizonte_amostra;
		for (IdEstagio idEstagio = a_idEstagioInicial; idEstagio <= a_idEstagioFinal; idEstagio++) {
			const Periodo periodo = getElementoVetor(AttVetorDados_horizonte_otimizacao, idEstagio, Periodo());
			horizonte_amostra.addElemento(periodo, SmartEnupla<IdRealizacao, double>(IdRealizacao_1, std::vector<double>(IdRealizacao(getElementoVetor(AttVetorDados_numero_aberturas, idEstagio, int())), NAN)));
		}

		return horizonte_amostra;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::getHorizonteEspacoAmostralHidrologico(" + getString(a_idEstagioInicial) + "," + getString(a_idEstagioFinal) + "): \n" + std::string(erro.what())); }

} // SmartEnupla<Periodo, IdRealizacao> Dados::getHorizonteEspacoAmostralHidrologico(const IdEstagio a_idEstagioInicial, const IdEstagio a_idEstagioFinal){


SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> Dados::getHorizonteEspacoAmostralHidrologico(const IdEstagio a_idEstagioInicial, const IdEstagio a_idEstagioFinal, const IdRealizacao a_maiorIdRealizacao)const {

	try {

		if (a_idEstagioInicial > a_idEstagioFinal)
			throw std::invalid_argument("Estagio inicial maior que final.");

		SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> horizonte_amostra;
		for (IdEstagio idEstagio = a_idEstagioInicial; idEstagio <= a_idEstagioFinal; idEstagio++) {
			const Periodo periodo = getElementoVetor(AttVetorDados_horizonte_otimizacao, idEstagio, Periodo());
			horizonte_amostra.addElemento(periodo, SmartEnupla<IdRealizacao, double>(IdRealizacao_1, std::vector<double>(a_maiorIdRealizacao, NAN)));
		}

		return horizonte_amostra;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::getHorizonteEspacoAmostralHidrologico(" + getString(a_idEstagioInicial) + "," + getString(a_idEstagioFinal) + "): \n" + std::string(erro.what())); }

} // SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> Dados::getHorizonteEspacoAmostralHidrologico(const IdEstagio a_idEstagioInicial, const IdEstagio a_idEstagioFinal, const IdRealizacao a_maiorIdRealizacao)const {

SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> Dados::getHorizonteEspacoAmostralHidrologico(const IdEstagio a_idEstagioInicial, const IdEstagio a_idEstagioFinal, const IdRealizacao a_maiorIdRealizacao, const bool a_manter_aberturas_estagio_inicial)const {

	try {

		if (a_idEstagioInicial > a_idEstagioFinal)
			throw std::invalid_argument("Estagio inicial maior que final.");

		SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> horizonte_amostra;
		for (IdEstagio idEstagio = a_idEstagioInicial; idEstagio <= a_idEstagioFinal; idEstagio++) {
			const Periodo periodo = getElementoVetor(AttVetorDados_horizonte_otimizacao, idEstagio, Periodo());

			if(a_manter_aberturas_estagio_inicial && idEstagio == a_idEstagioInicial && idEstagio >= getAtributo(AttComumDados_estagio_acoplamento_pre_estudo, IdEstagio()))
				horizonte_amostra.addElemento(periodo, SmartEnupla<IdRealizacao, double>(IdRealizacao_1, std::vector<double>(IdRealizacao(getElementoVetor(AttVetorDados_numero_aberturas, idEstagio, int())), NAN)));
			else if (idEstagio >= getAtributo(AttComumDados_estagio_acoplamento_pre_estudo, IdEstagio()))
				horizonte_amostra.addElemento(periodo, SmartEnupla<IdRealizacao, double>(IdRealizacao_1, std::vector<double>(a_maiorIdRealizacao, NAN)));
		}

		return horizonte_amostra;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::getHorizonteEspacoAmostralHidrologico(" + getString(a_idEstagioInicial) + "," + getString(a_idEstagioFinal) + "): \n" + std::string(erro.what())); }

} // SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> Dados::getHorizonteEspacoAmostralHidrologico(const IdEstagio a_idEstagioInicial, const IdEstagio a_idEstagioFinal, const IdRealizacao a_maiorIdRealizacao)const {


void Dados::validaIntercambioHidraulico() {
	try {

		const IdIntercambioHidraulico maiorIdIntercambioHidraulico = getMaiorId(IdIntercambioHidraulico());

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		for (IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico_1; idIntercambioHidraulico <= maiorIdIntercambioHidraulico; idIntercambioHidraulico++) {

			const TipoIntercambioHidraulico tipoIntercambioHidraulico = getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

			const IdHidreletrica idHidreletricaOrigem = getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica());

			const IdHidreletrica idHidreletricaDestino = getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica());

			if (idHidreletricaOrigem == IdHidreletrica_Nenhum)
				throw std::invalid_argument("Necessario informar a Hidreletrica origem no Intercambio hidraulico " + getString(idIntercambioHidraulico));

			if (tipoIntercambioHidraulico == TipoIntercambioHidraulico_Nenhum)
				throw std::invalid_argument("Necessario informar o tipo de intercambio hidraulico no Intercambio hidraulico " + getString(idIntercambioHidraulico));

			if ((tipoIntercambioHidraulico == TipoIntercambioHidraulico_desvio) && (idHidreletricaDestino == IdHidreletrica_Nenhum))
				throw std::invalid_argument(" Para o tipo de intercambio hidraulico " + getString(tipoIntercambioHidraulico) + " e necessario informar a Hidreletrica destino no Intercambio hidraulico " + getString(idIntercambioHidraulico));

			if (getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_desvio_agua_minimo, double()) > getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_desvio_agua_maximo, double()))
				throw std::invalid_argument(" Desvio minimo maior que o devio maximo no Intercambio hidraulico " + getString(idIntercambioHidraulico));

			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
				if ((getSizeVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo) > 0) && (getSizeVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo) > 0))
					if (getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo, periodo, double()) > getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo, periodo, double()))
						throw std::invalid_argument(" Desvio minimo maior que o devio maximo no Intercambio hidraulico" + getString(idIntercambioHidraulico) + " no periodo" + getString(periodo));

			//if (getSizeVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_vazao) != 0 \
			//	&& getSizeVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_vazao) != getSizeVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_nivel_reservatorio))
			//	throw std::invalid_argument(" Numero de registros de vazao e nivel_reservatorio devem ser iguais no Intercambio hidraulico" + getString(idIntercambioHidraulico));

		} // for (IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico_1; idIntercambioHidraulico <= maiorIdIntercambioHidraulico; idIntercambioHidraulico++) {

	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaIntercambioHidraulico(): \n" + std::string(erro.what())); }
}

void Dados::validaUsinaElevatoria() {
	try {

		const IdUsinaElevatoria maiorIdUsinaElevatoria = getMaiorId(IdUsinaElevatoria());

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria <= maiorIdUsinaElevatoria; idUsinaElevatoria++) {

			const IdSubmercado idSubmercado = getAtributo(idUsinaElevatoria, AttComumUsinaElevatoria_submercado, IdSubmercado());

			const IdHidreletrica idHidreletricaOrigem = getAtributo(idUsinaElevatoria, AttComumUsinaElevatoria_usina_origem_bombeamento, IdHidreletrica());

			const IdHidreletrica idHidreletricaDestino = getAtributo(idUsinaElevatoria, AttComumUsinaElevatoria_usina_destino_bombeamento, IdHidreletrica());

			if (idSubmercado == IdSubmercado_Nenhum)
				throw std::invalid_argument("Necessario informar o Submercado na UsinaElevatoria " + getString(idUsinaElevatoria));

			if (idHidreletricaOrigem == IdHidreletrica_Nenhum)
				throw std::invalid_argument("Necessario informar a Hidreletrica origem na UsinaElevatoria " + getString(idUsinaElevatoria));

			if (idHidreletricaDestino == IdHidreletrica_Nenhum)
				throw std::invalid_argument("Necessario informar a Hidreletrica destino na UsinaElevatoria " + getString(idUsinaElevatoria));

			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				if (getSizeVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_vazao_bombeada_minima) > 0 && getSizeVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_vazao_bombeada_maxima) > 0) {

					if (getElementoVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_vazao_bombeada_minima, periodo, double()) > getElementoVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_vazao_bombeada_maxima, periodo, double()))
						throw std::invalid_argument("Vazao_bombeada minima da  " + getFullString(idUsinaElevatoria) + " no " + getFullString(periodo) + "  maior que o vazao_bombeada maxima");

				}//if (getSizeVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_vazao_bombeada_minima) > 0 && getSizeVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_vazao_bombeada_maxima) > 0) {

				const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

				for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

					// VALIDACAO DOS ATRIBUTOS MATRIZ DA USINA ELEVATORIA
					if (getSize1Matriz(idUsinaElevatoria, AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_minima) != 0 && getSize1Matriz(idUsinaElevatoria, AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_maxima) != 0) {
						if (getElementoMatriz(idUsinaElevatoria, AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_minima, periodo, idPatamarCarga, double()) > getElementoMatriz(idUsinaElevatoria, AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_minima, periodo, idPatamarCarga, double()))
							throw std::invalid_argument("Vazao_bombeada_disponivel_minima da  " + getFullString(idUsinaElevatoria) + " no " + getFullString(periodo) + " no " + getFullString(idPatamarCarga) + "  maior que a vazao_bombeada_disponivel_maxima");
					}// if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_minima) != 0 && getSize1Matriz(idHidreletrica, AttMatrizConjuntoHidraulico_potencia_disponivel_maxima) != 0) {

				}

			}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

		} // for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria <= maiorIdUsinaElevatoria; idUsinaElevatoria++) {

	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaUsinaElevatoria(): \n" + std::string(erro.what())); }
}


void Dados::validaIntercambio() {
	try {

		const IdSubmercado maiorIdSubmercado = getMaiorId(IdSubmercado());
		if (maiorIdSubmercado == IdSubmercado_Nenhum)
			throw std::invalid_argument("Necessaria a adicao dos dados de " + getString(Submercado()) + " em Dados para a validacao de AttComum de " + getString(Intercambio()) + ".");

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const IdIntercambio maiorIdIntercambio = getMaiorId(IdIntercambio());
		for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= maiorIdIntercambio; idIntercambio++) {
			const IdSubmercado idSubmercadoOrigem = getAtributo(idIntercambio, AttComumIntercambio_submercado_origem, IdSubmercado());
			if (idSubmercadoOrigem > maiorIdSubmercado)
				throw std::invalid_argument("O Submercado Origem " + getString(idSubmercadoOrigem) + " cadastrado no Intercambio " + getString(idIntercambio) + " nao consta no cadastro do Dados.");

			const IdSubmercado idSubmercadoDestino = getAtributo(idIntercambio, AttComumIntercambio_submercado_destino, IdSubmercado());
			if (idSubmercadoDestino > maiorIdSubmercado)
				throw std::invalid_argument("O Submercado Destino " + getString(idSubmercadoDestino) + " cadastrado no Intercambio " + getString(idIntercambio) + " nao consta no cadastro do Dados.");


			if (getSize1Matriz(idIntercambio, AttMatrizIntercambio_percentual_variacao_patamar_carga) > 0) {

				for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

					const IdPatamarCarga maiorIdPatamarCargaReferencia = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

					const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(idIntercambio, AttMatrizIntercambio_percentual_variacao_patamar_carga, periodo, IdPatamarCarga());

					if (maiorIdPatamarCargaReferencia != maiorIdPatamarCarga)
						throw std::invalid_argument("O maior PatamarCarga em " + getFullString(idIntercambio) + " em " + getString(periodo) + " deve ser igual ao cadastrado em Dados.");

					double percentualVariacaoPonderado = 0.0;

					for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
						const double percentual_duracao = getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
						const double percentual_variacao = getElementoMatriz(idIntercambio, AttMatrizIntercambio_percentual_variacao_patamar_carga, periodo, idPatamarCarga, double());
						percentualVariacaoPonderado += percentual_duracao * percentual_variacao;
					} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

					if (!doubleCompara(0.02, percentualVariacaoPonderado, 1.0))
						throw std::invalid_argument("A ponderacao da Duracao pela Variacao dos PatamarCarga em " + getString(periodo) + " no Intercambio " + getString(idIntercambio) + " difere de 1.0 dada a precisao de 0.02.");

				} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

			} // if (getSize1Matriz(idIntercambio, AttMatrizIntercambio_percentual_variacao_patamar_carga) > 0) {

		} // for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= maiorIdIntercambio; idIntercambio++) {
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaIntercambio(): \n" + std::string(erro.what())); }
} // void Dados::validaIntercambio(){



void Dados::validaTermeletrica() {
	try {

		// Valida se AttComuns "submercado e combustivel" correspondem ao cadastrado em Dados.

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_estudo_inicial = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();


		const IdSubmercado maiorIdSubmercado = getMaiorId(IdSubmercado());
		if (maiorIdSubmercado == IdSubmercado_Nenhum)
			throw std::invalid_argument("Necessaria a adicao dos dados de " + getString(Submercado()) + " em Dados para a validacao de AttComum de " + getString(Termeletrica()) + ".");

		const IdTermeletrica maiorIdTermeletrica = getMaiorId(IdTermeletrica());

		for (IdTermeletrica idTermeletrica = getMenorId(IdTermeletrica()); idTermeletrica <= maiorIdTermeletrica; vetorTermeletrica.incr(idTermeletrica)) {

			const bool considerar_usina = getAtributo(idTermeletrica, AttComumTermeletrica_considerar_usina, bool());

			if (considerar_usina) {

				const IdSubmercado idSubmercado = getAtributo(idTermeletrica, AttComumTermeletrica_submercado, IdSubmercado());
				if (idSubmercado > maiorIdSubmercado)
					throw std::invalid_argument("O Submercado " + getString(idSubmercado) + " cadastrado na Termeletrica " + getString(idTermeletrica) + " nao consta no cadastro do Dados.");

				if (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_minima) != 0 && getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_maxima) != 0) {

					TipoDetalhamentoProducaoTermeletrica tipo_detalhamento_producao_termeletrica = getAtributo(idTermeletrica, AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica());

					const bool representacao_discreta_producao = getAtributo(idTermeletrica, AttComumTermeletrica_representacao_discreta_producao, bool());

					for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

						if (getSizeVetor(idTermeletrica, AttVetorTermeletrica_tipo_detalhamento_producao) != 0)
							tipo_detalhamento_producao_termeletrica = getElementoVetor(idTermeletrica, AttVetorTermeletrica_tipo_detalhamento_producao, periodo, TipoDetalhamentoProducaoTermeletrica());

						const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

						for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

							double potencia_disponivel_minima = -1.0;
							double potencia_disponivel_maxima = -1.0;

							if (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_minima) != 0)
								if ((getIterador1Inicial(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_minima, Periodo()) <= periodo) && (periodo <= getIterador1Final(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_minima, Periodo())))
									potencia_disponivel_minima = getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_minima, periodo, idPatamarCarga, double());

							if (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_maxima) != 0)
								if ((getIterador1Inicial(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_maxima, Periodo()) <= periodo) && (periodo <= getIterador1Final(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_maxima, Periodo())))
									potencia_disponivel_maxima = getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_maxima, periodo, idPatamarCarga, double());

							if ((potencia_disponivel_minima > -1.0) && (potencia_disponivel_maxima > -1.0) && (potencia_disponivel_minima > potencia_disponivel_maxima))
								throw std::invalid_argument("Potencia minima da  " + getFullString(idTermeletrica) + " no " + getFullString(periodo) + " no " + getFullString(idPatamarCarga) + "  maior que a potencia maxima");

							// VALIDA��O DOS ATRIBUTOS MATRIZ DA TERMELETRICA
							if (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_minima) != 0 && getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_maxima) != 0) {
								if (getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_minima, periodo, idPatamarCarga, double()) > getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_maxima, periodo, idPatamarCarga, double()))
									throw std::invalid_argument("Potencia minima da  " + getFullString(idTermeletrica) + " no " + getFullString(periodo) + " no " + getFullString(idPatamarCarga) + "  maior que a potencia maxima");

								if ((getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_util) != 0)) {
									if (!doubleCompara(0.01, getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_util, periodo, idPatamarCarga, double()), (getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_maxima, periodo, idPatamarCarga, double()) - getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_minima, periodo, idPatamarCarga, double()))))
										throw std::invalid_argument(getFullString(AttMatrizTermeletrica_potencia_util) + " de  " + getFullString(idTermeletrica) + " no " + getFullString(periodo) + " no " + getFullString(idPatamarCarga) + "  nao coincide com a diferenca entre a potencia  maxima e a minima ");
								}
							}

							if ((tipo_detalhamento_producao_termeletrica == TipoDetalhamentoProducaoTermeletrica_por_unidade) && (!representacao_discreta_producao)) {

								double potencia_disponivel_minima_total = 0.0;

								for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= getMaiorId(idTermeletrica, IdUnidadeUTE()); idUnidadeUTE++) {

									double potencia_disponivel_minima_unidade = -1.0;
									double potencia_disponivel_maxima_unidade = -1.0;

									if (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_minima) != 0)
										if ((getIterador1Inicial(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_minima, Periodo()) <= periodo) && (periodo <= getIterador1Final(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_minima, Periodo())))
											potencia_disponivel_minima_unidade = getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_minima, periodo, idPatamarCarga, double());

									if (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_maxima) != 0)
										if ((getIterador1Inicial(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_maxima, Periodo()) <= periodo) && (periodo <= getIterador1Final(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_maxima, Periodo())))
											potencia_disponivel_maxima_unidade = getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_maxima, periodo, idPatamarCarga, double());

									if ((potencia_disponivel_minima_unidade > -1.0) && (potencia_disponivel_maxima_unidade > -1.0) && (potencia_disponivel_minima_unidade > potencia_disponivel_maxima_unidade))
										throw std::invalid_argument("Potencia minima da  " + getFullString(idTermeletrica) + " no " + getFullString(periodo) + " no " + getFullString(idPatamarCarga) + "  maior que a potencia maxima");

									if (potencia_disponivel_minima_unidade > -1.0)
										potencia_disponivel_minima_total += potencia_disponivel_minima_unidade;

								} // for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= maiorIdUnidadeUTE; idUnidadeUTE++) {

								if (potencia_disponivel_minima > -1.0)
									if (potencia_disponivel_minima_total > potencia_disponivel_minima)
										throw std::invalid_argument("Soma de " + getFullString(AttMatrizUnidadeUTE_potencia_disponivel_minima) + " em " + getFullString(idTermeletrica) + " no " + getFullString(periodo) + " no " + getFullString(idPatamarCarga) + "  maior que " + getFullString(AttMatrizTermeletrica_potencia_disponivel_minima));

							} //if ((tipo_detalhamento_producao_termeletrica == TipoDetalhamentoProducaoTermeletrica_por_unidade) && (!representacao_discreta_producao)) { 

						}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

					} // for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

				}

			} // if (considerar_usina){

		} // for (IdTermeletrica idTermeletrica = getMenorId(IdTermeletrica()); idTermeletrica <= maiorIdTermeletrica; vetorTermeletrica.incr(idTermeletrica)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaTermeletrica(): \n" + std::string(erro.what())); }

} // void Dados::validaTermeletrica() {


void Dados::validaBaciaHidrografica() {

	for (IdBaciaHidrografica idBaciaHidrografica = IdBaciaHidrografica_1; idBaciaHidrografica <= getMaiorId(IdBaciaHidrografica()); idBaciaHidrografica++) {

		if (!vetorBaciaHidrografica.isInstanciado(idBaciaHidrografica))
			throw std::invalid_argument(getFullString(idBaciaHidrografica) + " nao instanciada.");

	} // for (IdBaciaHidrografica idBaciaHidrografica = IdBaciaHidrografica_1; idBaciaHidrografica <= getMaiorId(IdBaciaHidrografica()); idBaciaHidrografica++) {


} // void Dados::validaBaciaHidrografica(){


void Dados::validaUnidadeUTE() {
	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_estudo_inicial = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();

		const IdEstagio idEstagioFinal = getIteradorFinal(AttVetorDados_horizonte_otimizacao, IdEstagio());;

		const IdTermeletrica maiorIdTermeletrica = getMaiorId(IdTermeletrica());

		const int numeroPeriodos = getSizeVetor(AttVetorDados_horizonte_estudo);

		for (IdTermeletrica idUTE = getMenorId(IdTermeletrica()); idUTE <= maiorIdTermeletrica; vetorTermeletrica.incr(idUTE)) {

			const bool  considerar_usina = getAtributo(idUTE, AttComumTermeletrica_considerar_usina, bool());

			if (considerar_usina) {

				const IdUnidadeUTE maiorIdUnidadeUTE = getMaiorId(idUTE, IdUnidadeUTE());

				for (IdUnidadeUTE idUnidade = IdUnidadeUTE_1; idUnidade <= maiorIdUnidadeUTE; idUnidade++) {


					//if (((getAtributo(idUTE, idUnidade, AttComumUnidadeUTE_status) != TipoStatusUnidadeUTE_desligada) && ((getAtributo(idUTE, idUnidade, AttComumUnidadeUTE_horas_neste_status) != 0)  || (getAtributo(idUTE, idUnidade, AttComumUnidadeUTE_min_neste_status) != 0)) && (getAtributo(idUTE, idUnidade, AttComumUnidadeUTE_geracao_inicial) != 0) )
					//throw std::invalid_argument("Geracao inicial de  " + getFullString(idUnidade) + " de " + getFullString(idUTE) + " no " + getFullString(periodo) + " no " + getFullString(idPatamarCarga) + "  nao compativel com o estado anterior da unidade" + );



					if ((getSize1Matriz(idUTE, idUnidade, AttMatrizUnidadeUTE_potencia_minima) != 0) && (getSize1Matriz(idUTE, idUnidade, AttMatrizUnidadeUTE_potencia_maxima) != 0)) {

						for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {
							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {

								if (getElementoMatriz(idUTE, idUnidade, AttMatrizUnidadeUTE_potencia_minima, periodo, idPatamarCarga, double()) > getElementoMatriz(idUTE, idUnidade, AttMatrizUnidadeUTE_potencia_maxima, periodo, idPatamarCarga, double()))
									throw std::invalid_argument(getFullString(AttMatrizUnidadeUTE_potencia_minima) + " de  " + getFullString(idUnidade) + " de " + getFullString(idUTE) + " com " + getString(AttComumTermeletrica_codigo_usina) + "_" + getFullString(getAtributo(idUTE, AttComumTermeletrica_codigo_usina, int())) + " no " + getFullString(periodo) + " no " + getFullString(idPatamarCarga) + "  maior que " + getFullString(AttMatrizUnidadeUTE_potencia_maxima));

								if ((getSize1Matriz(idUTE, idUnidade, AttMatrizUnidadeUTE_potencia_util) != 0)) {
									if (!doubleCompara(0.01, getElementoMatriz(idUTE, idUnidade, AttMatrizUnidadeUTE_potencia_util, periodo, idPatamarCarga, double()), (getElementoMatriz(idUTE, idUnidade, AttMatrizUnidadeUTE_potencia_maxima, periodo, idPatamarCarga, double()) - getElementoMatriz(idUTE, idUnidade, AttMatrizUnidadeUTE_potencia_minima, periodo, idPatamarCarga, double()))))
										throw std::invalid_argument(getFullString(AttMatrizUnidadeUTE_potencia_util) + " de  " + getFullString(idUnidade) + " de " + getFullString(idUTE) + " no " + getFullString(periodo) + " no " + getFullString(idPatamarCarga) + "  nao coincide com a diferenca entre a potencia  maxima e a minima ");
								}
							}
						} // for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

					} // if ((getSize1Matriz(idUTE, idUnidade, AttMatrizUnidadeUTE_potencia_minima) != 0) && (getSize1Matriz(idUTE, idUnidade, AttMatrizUnidadeUTE_potencia_maxima) != 0)) {

					if ((getSize1Matriz(idUTE, idUnidade, AttMatrizUnidadeUTE_potencia_disponivel_minima) != 0) && (getSize1Matriz(idUTE, idUnidade, AttMatrizUnidadeUTE_potencia_disponivel_maxima) != 0)) {

						for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {
							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {
								if (getElementoMatriz(idUTE, idUnidade, AttMatrizUnidadeUTE_potencia_disponivel_minima, periodo, idPatamarCarga, double()) > getElementoMatriz(idUTE, idUnidade, AttMatrizUnidadeUTE_potencia_disponivel_maxima, periodo, idPatamarCarga, double()))
									throw std::invalid_argument(getFullString(AttMatrizUnidadeUTE_potencia_disponivel_minima) + " de  " + getFullString(idUnidade) + " de " + getFullString(idUTE) + " no " + getFullString(periodo) + " no " + getFullString(idPatamarCarga) + "  maior que " + getFullString(AttMatrizUnidadeUTE_potencia_disponivel_maxima));

							}
						} // for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

					} // if ((getSize1Matriz(idUTE, idUnidade, AttMatrizUnidadeUTE_potencia_disponivel_minima) != 0) && (getSize1Matriz(idUTE, idUnidade, AttMatrizUnidadeUTE_potencia_disponivel_maxima) != 0)) {

				} // for (IdUnidadeUTE idUnidade = IdUnidadeUTE_1; idUnidade <= maiorIdUnidadeUTE; idUnidade++) {

			} // if (considerar_usina) {

		} //for (IdTermeletrica idUTE = getMenorId(IdTermeletrica()); idUTE <= maiorIdTermeletrica; vetorTermeletrica.incr(idUTE)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaUnidadeUTE(): \n" + std::string(erro.what())); }
} // void Dados::validaUnidadeUTE(){


void Dados::validacao_operacional_Dados(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar) {

	try {

		const IdProcesso idProcesso = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());

		bool calculo_att_operacionais = false;

		if (getSizeVetor(AttVetorDados_horizonte_estudo) == 0)
			throw std::invalid_argument("Os elementos de " + getFullString(AttVetorDados_horizonte_estudo) + " devem ser informados.");
		else if (getSizeVetor(AttVetorDados_horizonte_otimizacao) == 0)
			throw std::invalid_argument("Os elementos de " + getFullString(AttVetorDados_horizonte_otimizacao) + " devem ser informados.");

		const IdEstagio estagio_inicial = getAtributo(AttComumDados_estagio_inicial, IdEstagio());

		if (estagio_inicial < getIteradorInicial(AttVetorDados_horizonte_otimizacao, IdEstagio()))
			throw std::invalid_argument("O valor de " + getFullString(AttComumDados_estagio_inicial) + " nao pode ser MENOR que o iterador inicial de " + getFullString(AttVetorDados_horizonte_otimizacao) + ".");

		const IdEstagio estagio_final = getAtributo(AttComumDados_estagio_final, IdEstagio());

		if (estagio_final > getIteradorFinal(AttVetorDados_horizonte_otimizacao, IdEstagio()))
			throw std::invalid_argument("O valor de " + getFullString(AttComumDados_estagio_final) + " nao pode ser MAIOR que o iterador inicial de " + getFullString(AttVetorDados_horizonte_otimizacao) + ".");

		const IdEstagio estagio_acoplamento_pre_estudo = getAtributo(AttComumDados_estagio_acoplamento_pre_estudo, IdEstagio());

		if (estagio_acoplamento_pre_estudo < estagio_inicial)
			throw std::invalid_argument("O valor de " + getFullString(AttComumDados_estagio_acoplamento_pre_estudo) + " nao pode ser MENOR que " + getFullString(AttComumDados_estagio_inicial) + ".");

		if ((estagio_inicial != IdEstagio_1) && (estagio_acoplamento_pre_estudo != estagio_inicial))
			throw std::invalid_argument("Caso o valor de " + getFullString(AttComumDados_estagio_inicial) + " seja diferente de " + getFullString(IdEstagio_1) + ", " + getFullString(AttComumDados_estagio_acoplamento_pre_estudo) + " deve possuir o mesmo valor.");

		if (true) {
			const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());
			SmartEnupla<Periodo, IdEstagio> horizonte_estudo_limitado;
			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
				if (horizonte_estudo.at(periodo) > estagio_final)
					break;
				horizonte_estudo_limitado.addElemento(periodo, horizonte_estudo.at(periodo));
			}
			setVetor(AttVetorDados_horizonte_estudo, horizonte_estudo_limitado);
		}

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_inicial_horizonte_estudo = getMenorPeriodoHorizonteEstudo();
		const Periodo periodo_final_horizonte_estudo = getMaiorPeriodoHorizonteEstudo();

		const double taxa_desconto_anual = getAtributo(AttComumDados_taxa_desconto_anual, double());

		if (getSizeMatriz(AttMatrizDados_percentual_duracao_horizonte_estudo) > 0)
			throw std::invalid_argument("O atributo " + getFullString(AttMatrizDados_percentual_duracao_horizonte_estudo) + " nao foi validado.");

		Periodo periodo_horizonte_estudo = horizonte_estudo.getIteradorInicial();
		for (IdEstagio idEstagio = IdEstagio_1; idEstagio <= estagio_final; idEstagio++) {

			const Periodo periodo_horizonte_otimizacao = getElementoVetor(AttVetorDados_horizonte_otimizacao, idEstagio, Periodo());

			const TipoPeriodo tipo_periodo_horizonte_otimizacao = periodo_horizonte_otimizacao.getTipoPeriodo();

			if (Periodo(tipo_periodo_horizonte_otimizacao, periodo_horizonte_estudo) != periodo_horizonte_otimizacao)
				throw std::invalid_argument("Data inicial do horizonte de estudo incompativel com horizonte de otimizacao no " + getFullString(idEstagio));

			const double segundos_periodo_horizonte_otimizacao = double(periodo_horizonte_otimizacao.getSegundos());

			IdEstagio idEstagio_horizonte_estudo = horizonte_estudo.getElemento(periodo_horizonte_estudo);

			while (idEstagio_horizonte_estudo == idEstagio) {

				const double segundos_periodo_horizonte_estudo = double(periodo_horizonte_estudo.getSegundos());

				const double percentual_duracao_periodo_estudo = segundos_periodo_horizonte_estudo / segundos_periodo_horizonte_otimizacao;

				addElemento(AttMatrizDados_percentual_duracao_horizonte_estudo, idEstagio, periodo_horizonte_estudo, percentual_duracao_periodo_estudo);

				horizonte_estudo.incrementarIterador(periodo_horizonte_estudo);

				if (periodo_horizonte_estudo > periodo_final_horizonte_estudo)
					break;

				idEstagio_horizonte_estudo = horizonte_estudo.getElemento(periodo_horizonte_estudo);

			} // while (idEstagio_horizonte_estudo == idEstagio) {

			if (getSizeMatriz(AttMatrizDados_percentual_duracao_horizonte_estudo, idEstagio) == 0)
				throw std::invalid_argument("Nao foram encontrados periodos de estudo no horizonte de otimizacao em " + getFullString(idEstagio));

			if (Periodo(tipo_periodo_horizonte_otimizacao, periodo_horizonte_estudo) != periodo_horizonte_otimizacao + 1)
				throw std::invalid_argument("Data final do horizonte de estudo incompativel com horizonte de otimizacao no " + getFullString(idEstagio));

		} // for (IdEstagio idEstagio = IdEstagio_1; idEstagio <= estagio_final; idEstagio++) {

		//
		//
		// Valida��o Percentual Dura��o Patamar Carga
		//
		//

		if (getSizeMatriz(AttMatrizDados_percentual_duracao_patamar_carga) == 0) {
			calculo_att_operacionais = true;
			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= periodo_final_horizonte_estudo; horizonte_estudo.incrementarIterador(periodo))
				addElemento(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga_1, 1.0);
		} // if (getSizeMatriz(AttMatrizDados_percentual_duracao_patamar_carga) == 0) {
		else
			validaPatamarCarga();

		//
		//
		// Valida��o Conversor Vaz�o Volume 
		//
		//

		if (getSizeVetor(AttVetorDados_conversor_vazao_volume) > 0)
			throw std::invalid_argument("O atributo " + getFullString(AttVetorDados_conversor_vazao_volume) + " nao foi validado.");

		if (getSizeMatriz(AttMatrizDados_conversor_vazao_volume) > 0)
			throw std::invalid_argument("O atributo " + getFullString(AttMatrizDados_conversor_vazao_volume) + " nao foi validado.");

		//Cria horizonte_defluencia_passada (defluencia_passada + horizonte_estudo)


		//Nota: A defluencia passada vai considerar periodos_diarios de 15 dias anteriores ao horizonte de estudo (15 dias m�ximo tempo de viagem d'�gua registrado)
		SmartEnupla<Periodo, bool> horizonte_defluencia_passada_mais_estudo(Periodo(TipoPeriodo_diario, horizonte_estudo.getIteradorInicial()) - 15, std::vector<bool>(15, true));

		//Adiciona periodos do horizonte_estudo
		for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= periodo_final_horizonte_estudo; horizonte_estudo.incrementarIterador(periodo))
			horizonte_defluencia_passada_mais_estudo.addElemento(periodo, true);

		for (Periodo periodo = horizonte_defluencia_passada_mais_estudo.getIteradorInicial(); periodo <= horizonte_defluencia_passada_mais_estudo.getIteradorFinal(); horizonte_defluencia_passada_mais_estudo.incrementarIterador(periodo)) {

			const double conversor_periodo = conversor_vazao_m3s_em_volume_hm3(periodo);
			addElemento(AttVetorDados_conversor_vazao_volume, periodo, conversor_periodo);

		} // for (Periodo periodo = horizonte_defluencia_passada_mais_estudo.getIteradorInicial(); periodo <= horizonte_defluencia_passada_mais_estudo.getIteradorFinal(); horizonte_defluencia_passada_mais_estudo.incrementarIterador(periodo)) {

		IdPatamarCarga maiorIdPatamarCarga_horizonte_estudo = IdPatamarCarga_Nenhum;

		for (Periodo periodo = periodo_inicial_horizonte_estudo; periodo <= periodo_final_horizonte_estudo; horizonte_estudo.incrementarIterador(periodo)) {
			double duracaoTotalPatamar = 0.0;

			const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

			if (maiorIdPatamarCarga > maiorIdPatamarCarga_horizonte_estudo)
				maiorIdPatamarCarga_horizonte_estudo = maiorIdPatamarCarga;

			const double conversor_periodo = getElementoVetor(AttVetorDados_conversor_vazao_volume, periodo, double());

			for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
				const double duracao_patamar = getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
				const double conversor_patamar = duracao_patamar * conversor_periodo;
				addElemento(AttMatrizDados_conversor_vazao_volume, periodo, idPatamarCarga, conversor_patamar);
			} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

		} // for (Periodo periodo = periodo_inicial_horizonte_estudo; periodo <= periodo_final_horizonte_estudo; horizonte_estudo.incrementarIterador(periodo)) {


		//
		//
		// Valida��o Desagio Acumulado
		//
		//

		bool desagio_carregado = false;

		if (getSizeMatriz(AttMatrizDados_desagio_acumulado_horizonte_estudo) > 0) {

			desagio_carregado = true;

			SmartEnupla<IdEstagio, SmartEnupla<Periodo, double>> percentual_duracao_horizonte_estudo = getMatriz(AttMatrizDados_percentual_duracao_horizonte_estudo, IdEstagio(), Periodo(), double());

			for (IdEstagio idEstagio = percentual_duracao_horizonte_estudo.getIteradorInicial(); idEstagio <= percentual_duracao_horizonte_estudo.getIteradorFinal(); idEstagio++) {
				if (percentual_duracao_horizonte_estudo.at(idEstagio).getIteradorInicial() != getIteradorInicial(AttMatrizDados_desagio_acumulado_horizonte_estudo, idEstagio, Periodo()))
					throw std::invalid_argument("Periodo inicial em " + getFullString(idEstagio) + " deve ser " + getFullString(percentual_duracao_horizonte_estudo.at(idEstagio).getIteradorInicial()) + " em " + getFullString(AttMatrizDados_desagio_acumulado_horizonte_estudo));
				if (percentual_duracao_horizonte_estudo.at(idEstagio).getIteradorFinal() != getIteradorFinal(AttMatrizDados_desagio_acumulado_horizonte_estudo, idEstagio, Periodo()))
					throw std::invalid_argument("Periodo final em " + getFullString(idEstagio) + " deve ser " + getFullString(percentual_duracao_horizonte_estudo.at(idEstagio).getIteradorFinal()) + " em " + getFullString(AttMatrizDados_desagio_acumulado_horizonte_estudo));
			}

		} // if (getSizeMatriz(AttMatrizDados_desagio_acumulado_horizonte_estudo) > 0) {

		else {

			SmartEnupla<IdEstagio, SmartEnupla<Periodo, double>> desagio_acumulado_horizonte_estudo;
			for (IdEstagio idEstagio = IdEstagio_1; idEstagio <= estagio_final; idEstagio++)
				desagio_acumulado_horizonte_estudo.addElemento(idEstagio, SmartEnupla<Periodo, double>(getElementosMatriz(AttMatrizDados_percentual_duracao_horizonte_estudo, idEstagio, Periodo(), double()), 1.0));

			Periodo periodo_referencia = getAtributo(AttComumDados_periodo_referencia, Periodo());
			Periodo periodo_referencia_anual = Periodo(TipoPeriodo_anual, periodo_referencia.getAno());
			Periodo periodo_referencia_minuto = Periodo(TipoPeriodo(TipoPeriodo_Excedente - 1), periodo_referencia);

			double desagio_acumulado = 1.0;

			Periodo periodo_estudo = horizonte_estudo.getIteradorInicial();

			for (Periodo periodo = periodo_referencia_minuto; periodo < periodo_referencia + 1; periodo++) {

				if (periodo > periodo_referencia_anual + 1)
					periodo_referencia_anual++;

				const double taxa_desconto_minuto = std::pow((1.0 + taxa_desconto_anual), 1.0 / double(periodo_referencia_anual.getMinutos())) - 1.0;

				desagio_acumulado *= 1.0 / (1.0 + taxa_desconto_minuto);

				if ((periodo_estudo.sobreposicao(periodo) > 0.0) && (periodo_estudo.sobreposicao(periodo_referencia) == 1.0))
					horizonte_estudo.incrementarIterador(periodo_estudo);

			} // for (Periodo periodo = periodo_referencia_minuto; periodo < periodo_referencia + 1; periodo++) {

			while (true) {

				periodo_referencia_minuto = Periodo(TipoPeriodo(TipoPeriodo_Excedente - 1), periodo_referencia_anual);

				const double taxa_desconto_minuto = std::pow((1.0 + taxa_desconto_anual), 1.0 / double(periodo_referencia_anual.getMinutos())) - 1.0;

				for (Periodo periodo = periodo_referencia_minuto; periodo < periodo_referencia_anual + 1; periodo++) {

					if (periodo_estudo.sobreposicao(periodo)) {

						desagio_acumulado_horizonte_estudo.at(horizonte_estudo.at(periodo_estudo)).at(periodo_estudo) = desagio_acumulado;

						if (periodo_estudo == periodo_final_horizonte_estudo)
							break;

						horizonte_estudo.incrementarIterador(periodo_estudo);

					} // if (periodo_estudo.sobreposicao(periodo)) {

					desagio_acumulado *= 1.0 / (1.0 + taxa_desconto_minuto);

				} // for (Periodo periodo = periodo_estudo_minuto; periodo < periodo_estudo_anual + 1; periodo++) {

				if (periodo_referencia_anual.getAno() > (periodo_final_horizonte_estudo + 1).getAno())
					break;

				periodo_referencia_anual++;

			} // while (true) {

			setMatriz(AttMatrizDados_desagio_acumulado_horizonte_estudo, desagio_acumulado_horizonte_estudo);

		} // if (true) {

		//
		//
		// Valida��o Numero Aberturas
		//
		//

		if (getSizeVetor(AttVetorDados_numero_aberturas) == 0) {
			calculo_att_operacionais = true;

			for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

				if (idEstagio < estagio_acoplamento_pre_estudo)
					addElemento(AttVetorDados_numero_aberturas, idEstagio, 1);
				else
					addElemento(AttVetorDados_numero_aberturas, idEstagio, getAtributo(AttComumDados_numero_aberturas, int()));

			} // for (IdEstagio idEstagio = estagio_inicial; idEstagio < estagio_final; idEstagio++) {

		} // if (getSizeVetor(AttVetorDados_numero_aberturas) == 0) {

		else if (getSizeVetor(AttVetorDados_numero_aberturas) > 0) {

			for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

				const int numero_abertura = getElementoVetor(AttVetorDados_numero_aberturas, idEstagio, int());

				if ((numero_abertura > 1) && (estagio_acoplamento_pre_estudo > idEstagio))
					throw std::invalid_argument("O numero de aberturas do horizonte de otimizacao deve ser 1 no estagio " + getFullString(idEstagio) + " dado o estagio de acoplamento " + getFullString(estagio_acoplamento_pre_estudo) + ".");

			} // for (IdEstagio idEstagio = estagio_inicial; idEstagio < estagio_final; idEstagio++) {

		} // else if (getSizeVetor(AttVetorDados_numero_aberturas) > 0) {

		//
		//
		// Valida��o Multiplicidade Cortes
		//
		//

		if (getSizeVetor(AttVetorDados_cortes_multiplos) > 0) {
			for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {
				const int multiplicidade_corte = getElementoVetor(AttVetorDados_cortes_multiplos, idEstagio, int());

				if ((multiplicidade_corte > 1) && (getAtributo(AttComumDados_tipo_aversao_a_risco, TipoAversaoRisco()) != TipoAversaoRisco_valor_esperado))
					throw std::invalid_argument("Devido ao atributo " + getFullString(AttComumDados_tipo_aversao_a_risco) + " a multiplicidade dos cortes em " + getFullString(idEstagio) + " deve ser 0.");

				if ((multiplicidade_corte > 1) && (multiplicidade_corte > getElementoVetor(AttVetorDados_numero_aberturas, idEstagio, int())))
					throw std::invalid_argument("A multiplicidade dos cortes em " + getFullString(idEstagio) + " deve ser menor ou igual ao atributo " + getFullString(AttVetorDados_numero_aberturas) + ".");
			}
		} // if (getSizeVetor(AttVetorDados_multiplicidade_corte) > 0) {

		else {
			calculo_att_operacionais = true;
			for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {
				if (getAtributo(AttComumDados_calcular_custo_primal_via_subproblema_mestre, bool()))
					addElemento(AttVetorDados_cortes_multiplos, idEstagio, getElementoVetor(AttVetorDados_numero_aberturas, idEstagio, int()));
				else
					addElemento(AttVetorDados_cortes_multiplos, idEstagio, 0);
			}
		}


		//
		//
		// Numero aberturas Solução Proxy
		//
		//

		if (getSizeVetor(AttVetorDados_numero_aberturas_solucao_proxy) == 0) {
			calculo_att_operacionais = true;

			for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

				const int numero_aberturas = getElementoVetor(AttVetorDados_numero_aberturas, idEstagio, int());

				addElemento(AttVetorDados_numero_aberturas_solucao_proxy, idEstagio, numero_aberturas);

			} // for (IdEstagio idEstagio = estagio_inicial; idEstagio < estagio_final; idEstagio++) {

		} // if (getSizeVetor(AttVetorDados_numero_aberturas_solucao_proxy) == 0) {

		else if (getSizeVetor(AttVetorDados_numero_aberturas_solucao_proxy) > 0) {

			for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

				const int numero_abertura = getElementoVetor(AttVetorDados_numero_aberturas, idEstagio, int());
				const int numero_abertura_solucao_proxy = getElementoVetor(AttVetorDados_numero_aberturas_solucao_proxy, idEstagio, int());

				if (numero_abertura_solucao_proxy > numero_abertura)
					throw std::invalid_argument("O atributo " + getFullString(AttVetorDados_numero_aberturas_solucao_proxy) + " em " + getFullString(idEstagio) + " deve ser menor que o atributo " + getFullString(AttVetorDados_numero_aberturas) + ".");

			} // for (IdEstagio idEstagio = estagio_inicial; idEstagio < estagio_final; idEstagio++) {

		} // else if (getSizeVetor(AttVetorDados_numero_aberturas_solucao_proxy) > 0) {

		//
		//
		// Valida��o Par�metros CVaR
		//
		//

		bool alpha_cvar_redefinido = false;
		bool lambda_cvar_redefinido = false;

		if (getAtributo(AttComumDados_tipo_aversao_a_risco, TipoAversaoRisco()) == TipoAversaoRisco_CVAR) {

			if (getSizeVetor(AttVetorDados_alpha_CVAR) == 0) {
				setVetor(AttVetorDados_alpha_CVAR, SmartEnupla<IdEstagio, double>(estagio_inicial, std::vector<double>(estagio_final, getAtributo(AttComumDados_alpha_CVAR, double()))));
				calculo_att_operacionais = true;
			}

			if (getSizeVetor(AttVetorDados_lambda_CVAR) == 0) {
				setVetor(AttVetorDados_lambda_CVAR, SmartEnupla<IdEstagio, double>(estagio_inicial, std::vector<double>(estagio_final, getAtributo(AttComumDados_lambda_CVAR, double()))));
				calculo_att_operacionais = true;
			}

		} // if (getAtributo(AttComumDados_tipo_aversao_a_risco, TipoAversaoRisco()) == TipoAversaoRisco_CVAR){

		else {

			if (getSizeVetor(AttVetorDados_lambda_CVAR) > 0) {

				for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

					const double lambda = getElementoVetor(AttVetorDados_lambda_CVAR, idEstagio, double());

					if (lambda > 0.0) {
						lambda_cvar_redefinido = true;
						break;
					}

				} // for (IdEstagio idEstagio = estagio_inicial; idEstagio < estagio_final; idEstagio++) {
			}

			if ((getSizeVetor(AttVetorDados_lambda_CVAR) == 0) || (lambda_cvar_redefinido)) {
				setVetor_forced(AttVetorDados_lambda_CVAR, SmartEnupla<IdEstagio, double>(estagio_inicial, std::vector<double>(estagio_final, 0.0)));
				lambda_cvar_redefinido = true;
			}

			if (getSizeVetor(AttVetorDados_alpha_CVAR) > 0) {

				for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

					const double alpha = getElementoVetor(AttVetorDados_alpha_CVAR, idEstagio, double());

					if (alpha > 0.0) {
						alpha_cvar_redefinido = true;
						break;
					}

				} // for (IdEstagio idEstagio = estagio_inicial; idEstagio < estagio_final; idEstagio++) {
			}

			if ((getSizeVetor(AttVetorDados_alpha_CVAR) == 0) || (alpha_cvar_redefinido)) {
				setVetor_forced(AttVetorDados_alpha_CVAR, SmartEnupla<IdEstagio, double>(estagio_inicial, std::vector<double>(estagio_final, 0.0)));
				alpha_cvar_redefinido = true;
			}
		}


		if (idProcesso == IdProcesso_mestre) {

			a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

			a_entradaSaidaDados.setAppendArquivo(false);
			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_DADOS_AttVetorOperacional_PorPeriodo.csv", *this, horizonte_defluencia_passada_mais_estudo.getIteradorInicial(), periodo_final_horizonte_estudo, AttVetorDados_conversor_vazao_volume);

			a_entradaSaidaDados.setAppendArquivo(false);
			a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("_info_DADOS_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, periodo_inicial_horizonte_estudo, periodo_final_horizonte_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte_estudo, AttMatrizDados_conversor_vazao_volume);

			a_entradaSaidaDados.setAppendArquivo(false);
			a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("_info_DADOS_AttMatrizOperacional_PorIdEstagioPorPeriodo.csv", *this, estagio_inicial, estagio_final, periodo_inicial_horizonte_estudo, periodo_final_horizonte_estudo, AttMatrizDados_percentual_duracao_horizonte_estudo);

			if (!desagio_carregado)
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("_info_DADOS_" + getString(AttMatrizDados_desagio_acumulado_horizonte_estudo) + ".csv", *this, estagio_inicial, estagio_final, periodo_inicial_horizonte_estudo, periodo_final_horizonte_estudo, AttMatrizDados_desagio_acumulado_horizonte_estudo);

		}

		a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

		if ((!calculo_att_operacionais) && (!a_imprimir_atributos_sem_recarregar)) {
			if (idProcesso == IdProcesso_mestre) {

				a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

				a_entradaSaidaDados.setAppendArquivo(false);

				if (lambda_cvar_redefinido) {
					a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("DADOS_AttVetorOperacional_PorIdEstagio.csv", *this, estagio_inicial, estagio_final, AttVetorDados_lambda_CVAR);
					a_entradaSaidaDados.setAppendArquivo(true);
				}
				if (alpha_cvar_redefinido)
					a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("DADOS_AttVetorOperacional_PorIdEstagio.csv", *this, estagio_inicial, estagio_final, AttVetorDados_alpha_CVAR);
			}
		}

		if ((calculo_att_operacionais) || (a_imprimir_atributos_sem_recarregar)) {

			if (idProcesso == IdProcesso_mestre) {

				a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

				a_entradaSaidaDados.setAppendArquivo(false);
				a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("DADOS_AttVetorOperacional_PorIdEstagio.csv", *this, estagio_inicial, estagio_final, AttVetorDados_numero_aberturas);

				a_entradaSaidaDados.setAppendArquivo(true);
				a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("DADOS_AttVetorOperacional_PorIdEstagio.csv", *this, estagio_inicial, estagio_final, AttVetorDados_numero_aberturas_solucao_proxy);
				a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("DADOS_AttVetorOperacional_PorIdEstagio.csv", *this, estagio_inicial, estagio_final, AttVetorDados_cortes_multiplos);
				a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("DADOS_AttVetorOperacional_PorIdEstagio.csv", *this, estagio_inicial, estagio_final, AttVetorDados_lambda_CVAR);
				a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("DADOS_AttVetorOperacional_PorIdEstagio.csv", *this, estagio_inicial, estagio_final, AttVetorDados_alpha_CVAR);

				a_entradaSaidaDados.setAppendArquivo(false);
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("DADOS_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, periodo_inicial_horizonte_estudo, periodo_final_horizonte_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte_estudo, AttMatrizDados_percentual_duracao_patamar_carga);

				if (a_imprimir_atributos_sem_recarregar) {

					a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

					a_entradaSaidaDados.setAppendArquivo(false);

					std::vector<AttComumDados> attComumDados_impressao;
					for (AttComumDados attComumDados = AttComumDados(AttComumDados_Nenhum + 1); attComumDados < AttComumDados_Excedente; attComumDados++) {
						if (attComumDados == AttComumDados_idDados) {}
						else if (attComumDados == AttComumDados_numero_aberturas) {}
						else if (attComumDados == AttComumDados_alpha_CVAR) {}
						else if (attComumDados == AttComumDados_lambda_CVAR) {}
						else if (attComumDados == AttComumDados_calcular_custo_primal_via_subproblema_mestre) {}
						else if (attComumDados == AttComumDados_tipo_modelo_geracao_cenario_hidrologico) {}
						else if (attComumDados == AttComumDados_numero_periodos_avaliacao_geracao_cenario_hidrologico) {}
						else if (attComumDados == AttComumDados_periodo_tendencia_hidrologica_historica) {}
						else if (attComumDados == AttComumDados_periodo_historico_geracao_cenario_hidrologico) {}
						else if (attComumDados == AttComumDados_numero_maximo_tentativas_resolucao) {}
						else if (attComumDados == AttComumDados_diretorio_entrada_dados) {}
						else if (attComumDados == AttComumDados_exibir_na_tela_resultado_solver) {}
						else if (attComumDados == AttComumDados_calcular_cenario_hidrologico_pre_otimizacao) {}
						else if (attComumDados == AttComumDados_imprimir_cenario_hidrologico_pre_otimizacao) {}
						else if (attComumDados == AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_eletricas) {}
						else if (attComumDados == AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_hidraulicas) {}
						else if (attComumDados == AttComumDados_representar_defluencia_disponivel_em_restricoes_hidraulicas) {}
						else if (attComumDados == AttComumDados_representar_turbinamento_disponivel_em_restricoes_hidraulicas) {}
						else if (attComumDados == AttComumDados_representar_potencia_hidro_disponivel_em_restricoes_hidraulicas) {}
						else if (attComumDados == AttComumDados_representar_potencia_termo_disponivel_em_restricoes_hidraulicas) {}
						else if (attComumDados == AttComumDados_imprimir_exportacao_pos_estudo) {}
						else if (attComumDados == AttComumDados_imprimir_geracao_cenario_hidrologico) {}
						else if (attComumDados == AttComumDados_imprimir_espaco_amostral_geracao_cenario_hidrologico) {}
						else if (attComumDados == AttComumDados_maior_estagio_impressao_versao_alternativa_cortes) {}
						else
							attComumDados_impressao.push_back(attComumDados);
					}

					a_entradaSaidaDados.imprimirArquivoCSV_AttComum("DADOS_AttComumOperacional.csv", *this, attComumDados_impressao);

					a_entradaSaidaDados.setAppendArquivo(false);
					a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("DADOS_AttVetorOperacional_PorPeriodo.csv", *this, periodo_inicial_horizonte_estudo, periodo_final_horizonte_estudo, AttVetorDados_horizonte_estudo);

					a_entradaSaidaDados.setAppendArquivo(true);
					a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("DADOS_AttVetorOperacional_PorIdEstagio.csv", *this, estagio_inicial, estagio_final, AttVetorDados_horizonte_otimizacao);

				} // if (a_imprimir_atributos_sem_recarregar) {

			} // if (idProcesso == IdProcesso_mestre){

			if (!a_imprimir_atributos_sem_recarregar) {

				if (true) {
					int barreira = 0;

					if (arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso()) == IdProcesso_mestre) {
						for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++)
							MPI_Send(&barreira, 1, MPI_INT, getRank(idProcesso), 0, MPI_COMM_WORLD);
					}
					else
						MPI_Recv(&barreira, 1, MPI_INT, getRank(IdProcesso_mestre), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}

				setVetor(AttVetorDados_numero_aberturas, SmartEnupla<IdEstagio, int>());
				setVetor(AttVetorDados_numero_aberturas_solucao_proxy, SmartEnupla<IdEstagio, int>());
				setVetor(AttVetorDados_cortes_multiplos, SmartEnupla<IdEstagio, int>());
				setVetor(AttVetorDados_lambda_CVAR, SmartEnupla<IdEstagio, double>());
				setVetor(AttVetorDados_alpha_CVAR, SmartEnupla<IdEstagio, double>());

				setMatriz(AttMatrizDados_percentual_duracao_patamar_carga, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>());

				a_entradaSaidaDados.setDiretorioEntrada(a_diretorio_att_operacional);
				a_entradaSaidaDados.carregarArquivoCSV_AttVetor("DADOS_AttVetorOperacional_PorIdEstagio.csv", *this, TipoAcessoInstancia_direto);

				a_entradaSaidaDados.carregarArquivoCSV_AttMatriz("DADOS_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_direto);

			} // if (!a_imprimir_atributos_sem_recarregar) {

		} // if ((calculo_att_operacionais) || (a_imprimir_atributos_sem_recarregar)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validacao_operacional_Dados(a_entradaSaidaDados, " + a_diretorio_att_operacional + "," + a_diretorio_att_premissa + "," + getString(a_imprimir_atributos_sem_recarregar) + "): \n" + std::string(erro.what())); }

} // void Dados::validacao_operacional_horizonte(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar){


void Dados::validacao_operacional_Intercambio_Hidraulico(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar) {

	try {

		validaIntercambioHidraulico();

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_estudo_inicial = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();

		bool recarregar_atributo = false;

		// ID PROCESSO
		const IdProcesso idProcesso = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());

		const IdIntercambioHidraulico maiorIdIntercambioHidraulico = getMaiorId(IdIntercambioHidraulico());


		for (IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico_1; idIntercambioHidraulico <= maiorIdIntercambioHidraulico; idIntercambioHidraulico++) {

			const TipoIntercambioHidraulico tipoIntercambioHidraulico = getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico());

			const IdHidreletrica idHidreletricaOrigem = getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_origem, IdHidreletrica());

			const IdHidreletrica idHidreletricaDestino = getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_hidreletrica_destino, IdHidreletrica());


			if ((getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico()) == TipoIntercambioHidraulico_pereira_barreto) && ((getSizeVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo) == 0) || (getSizeVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo) == 0))) {

				const double cotaOrigem = vetorHidreletrica.att(idHidreletricaOrigem).vetorReservatorio.att(IdReservatorio_1).getCota(periodo_estudo_inicial, double(getElementoVetor(idHidreletricaOrigem, IdReservatorio_1, AttVetorReservatorio_volume_minimo, periodo_estudo_inicial, double()) + getElementoVetor(idHidreletricaOrigem, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, periodo_estudo_inicial, double())));
				const double cotaDestino = vetorHidreletrica.att(idHidreletricaDestino).vetorReservatorio.att(IdReservatorio_1).getCota(periodo_estudo_inicial, double(getElementoVetor(idHidreletricaDestino, IdReservatorio_1, AttVetorReservatorio_volume_minimo, periodo_estudo_inicial, double()) + getElementoVetor(idHidreletricaDestino, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, periodo_estudo_inicial, double())));

				double vazao = 0;
				int desnivel = int((cotaOrigem - cotaDestino) * 100);

				int posicaoDesnivel = 0;
				int posicaoNivel = 0;

				if ((cotaOrigem > cotaDestino) && (desnivel >= 1)) {

					for (int i = 1; i <= getSizeVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desnivel); i++)
						if (getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desnivel, i, double()) <= desnivel)
							posicaoDesnivel = i;

					for (int i = 1; i <= getSizeVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_nivel_reservatorio); i++)
						if (getElementoVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_nivel_reservatorio, i, double()) <= cotaOrigem)
							posicaoNivel = i;

					vazao = getElementoMatriz(idIntercambioHidraulico, AttMatrizIntercambioHidraulico_vazao, posicaoNivel, posicaoDesnivel, double());
				}

				vetorIntercambioHidraulico.att(idIntercambioHidraulico).setVetor(AttVetorIntercambioHidraulico_desvio_agua_minimo, SmartEnupla<Periodo, double>(horizonte_estudo, vazao));
				vetorIntercambioHidraulico.att(idIntercambioHidraulico).setVetor(AttVetorIntercambioHidraulico_desvio_agua_maximo, SmartEnupla<Periodo, double>(horizonte_estudo, vazao));
				recarregar_atributo = true;

			}//	if ((getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_tipo_intercambio_hidraulico, TipoIntercambioHidraulico()) == TipoIntercambioHidraulico_pereira_barreto) && ((getSizeVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo) == 0) || (getSizeVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo) == 0))) {


			if ((getSizeVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo) == 0)) {
				vetorIntercambioHidraulico.att(idIntercambioHidraulico).setVetor(AttVetorIntercambioHidraulico_desvio_agua_minimo, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_desvio_agua_minimo, double())));
				recarregar_atributo = true;
			}

			if ((getSizeVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo) == 0)) {
				vetorIntercambioHidraulico.att(idIntercambioHidraulico).setVetor(AttVetorIntercambioHidraulico_desvio_agua_maximo, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idIntercambioHidraulico, AttComumIntercambioHidraulico_desvio_agua_maximo, double())));
				recarregar_atributo = true;
			}

		}//for (IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico_1; idIntercambioHidraulico <= maiorIdIntercambioHidraulico; idIntercambioHidraulico++) {


		validaIntercambioHidraulico();


		if (idProcesso == IdProcesso_mestre) {


			if (a_imprimir_atributos_sem_recarregar) {

				if (getMaiorId(IdIntercambioHidraulico()) > IdIntercambioHidraulico_Nenhum) {

					a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

					a_entradaSaidaDados.setAppendArquivo(false);
					a_entradaSaidaDados.imprimirArquivoCSV_AttComum("INTERCAMBIO_HIDRAULICO_AttComumOperacional.csv", IdIntercambioHidraulico_Nenhum, *this);

					bool impresso = false;
					bool impresso_por_inteiro = false;

					for (IdIntercambioHidraulico idIntercambioHidraulico = IdIntercambioHidraulico_1; idIntercambioHidraulico <= maiorIdIntercambioHidraulico; idIntercambioHidraulico++) {
						if (getSizeVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_minimo) != 0) {
							a_entradaSaidaDados.setAppendArquivo(impresso);
							a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("INTERCAMBIO_HIDRAULICO_AttVetorOperacional_PorPeriodo.csv", idIntercambioHidraulico, *this, periodo_estudo_inicial, periodo_final_estudo, AttVetorIntercambioHidraulico_desvio_agua_minimo);
							impresso = true;
						}

						if (getSizeVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_desvio_agua_maximo) != 0) {
							a_entradaSaidaDados.setAppendArquivo(impresso);
							a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("INTERCAMBIO_HIDRAULICO_AttVetorOperacional_PorPeriodo.csv", idIntercambioHidraulico, *this, periodo_estudo_inicial, periodo_final_estudo, AttVetorIntercambioHidraulico_desvio_agua_maximo);
							impresso = true;
						}

						if (getSize1Matriz(idIntercambioHidraulico, AttMatrizIntercambioHidraulico_vazao) > 0) {

							a_entradaSaidaDados.setAppendArquivo(impresso_por_inteiro);
							a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("INTERCAMBIO_HIDRAULICO_AttVetorOperacional_PorInteiro.csv", idIntercambioHidraulico, *this, 1, 15, AttVetorIntercambioHidraulico_desnivel);

							a_entradaSaidaDados.setAppendArquivo(impresso_por_inteiro);
							a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("INTERCAMBIO_HIDRAULICO_AttMatrizOperacional_PorInteiro.csv", idIntercambioHidraulico, *this, 1, getSize1Matriz(idIntercambioHidraulico, AttMatrizIntercambioHidraulico_vazao), 1, getSize2Matriz(idIntercambioHidraulico, AttMatrizIntercambioHidraulico_vazao, 1), AttMatrizIntercambioHidraulico_vazao);

							impresso_por_inteiro = true;

							a_entradaSaidaDados.setAppendArquivo(impresso_por_inteiro);
							a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("INTERCAMBIO_HIDRAULICO_AttVetorOperacional_PorInteiro.csv", idIntercambioHidraulico, *this, 1, 40, AttVetorIntercambioHidraulico_nivel_reservatorio);

						}//if (getSizeVetor(idIntercambioHidraulico, AttVetorIntercambioHidraulico_vazao) > 0) {

					}//for (IdIntercambioHidraulico idIntercambio = IdIntercambioHidraulico_1; idIntercambio <= maiorIdIntercambioHidraulico; idIntercambio++) {

				}

			}

		}//if (idProcesso == IdProcesso_mestre) {

		if (!a_imprimir_atributos_sem_recarregar) {

			if (true) {
				int barreira = 0;

				if (arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso()) == IdProcesso_mestre) {
					for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++)
						MPI_Send(&barreira, 1, MPI_INT, getRank(idProcesso), 0, MPI_COMM_WORLD);
				}
				else
					MPI_Recv(&barreira, 1, MPI_INT, getRank(IdProcesso_mestre), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}

			if (recarregar_atributo) {
				a_entradaSaidaDados.setDiretorioEntrada(a_diretorio_att_operacional);

				a_entradaSaidaDados.carregarArquivoCSV_AttComum("INTERCAMBIO_HIDRAULICO_AttComumOperacional.csv", *this, TipoAcessoInstancia_membro);

				a_entradaSaidaDados.carregarArquivoCSV_AttVetor("INTERCAMBIO_HIDRAULICO_AttVetorOperacional_PorPeriodo.csv", *this, TipoAcessoInstancia_membro);

				a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("INTERCAMBIO_HIDRAULICO_AttVetorOperacional_PorInteiro.csv", *this, TipoAcessoInstancia_membro);

			}//if (recarregar_atributo) {

		}//if (!a_imprimir_atributos_sem_recarregar) {

	}
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validacao_operacional_Intercambio_Hidraulico(a_entradaSaidaDados, " + a_diretorio_att_operacional + "," + a_diretorio_att_premissa + "," + getString(a_imprimir_atributos_sem_recarregar) + "): \n" + std::string(erro.what())); }

}

void Dados::validacao_operacional_UsinasElevatorias(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar) {

	try {

		validaUsinaElevatoria();

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_estudo_inicial = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();

		// MAIOR PATAMAR DE CARGA DO HORIZONTE DE ESTUDO 
		IdPatamarCarga maiorIdPatamarCarga_horizonte = IdPatamarCarga_Nenhum;
		for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {
			const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
			if (maiorIdPatamarCarga > maiorIdPatamarCarga_horizonte)
				maiorIdPatamarCarga_horizonte = maiorIdPatamarCarga;
		} // for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

		bool recarregar_atributo = false;

		// ID PROCESSO
		const IdProcesso idProcesso = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());

		const IdUsinaElevatoria maiorIdUsinaElevatoria = getMaiorId(IdUsinaElevatoria());

		for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria <= maiorIdUsinaElevatoria; idUsinaElevatoria++) {

			if ((getSizeVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_fator_disponibilidade) == 0)) {
				vetorUsinaElevatoria.att(idUsinaElevatoria).setVetor(AttVetorUsinaElevatoria_fator_disponibilidade, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idUsinaElevatoria, AttComumUsinaElevatoria_fator_disponibilidade, double())));
				recarregar_atributo = true;
			}

			if ((getSizeVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_vazao_bombeada_minima) == 0)) {
				vetorUsinaElevatoria.att(idUsinaElevatoria).setVetor(AttVetorUsinaElevatoria_vazao_bombeada_minima, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idUsinaElevatoria, AttComumUsinaElevatoria_vazao_bombeada_minima, double())));
				recarregar_atributo = true;
			}

			if ((getSizeVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_vazao_bombeada_maxima) == 0)) {
				vetorUsinaElevatoria.att(idUsinaElevatoria).setVetor(AttVetorUsinaElevatoria_vazao_bombeada_maxima, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idUsinaElevatoria, AttComumUsinaElevatoria_vazao_bombeada_maxima, double())));
				recarregar_atributo = true;
			}

			if (getSize1Matriz(idUsinaElevatoria, AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_minima) == 0) {

				if (getSizeVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_vazao_bombeada_disponivel_minima) == 0) {


					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						for (IdPatamarCarga idPatamarCarga_aux = IdPatamarCarga_1; idPatamarCarga_aux <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga_aux++)
							vetorUsinaElevatoria.att(idUsinaElevatoria).addElemento(AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_minima, periodo, idPatamarCarga_aux, getAtributo(idUsinaElevatoria, AttComumUsinaElevatoria_vazao_bombeada_disponivel_minima, double()));

					}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				}
				else {

					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						for (IdPatamarCarga idPatamarCarga_aux = IdPatamarCarga_1; idPatamarCarga_aux <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga_aux++)
							vetorUsinaElevatoria.att(idUsinaElevatoria).addElemento(AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_minima, periodo, idPatamarCarga_aux, getElementoVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_vazao_bombeada_disponivel_minima, periodo, double()));

					}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				}//else {

				recarregar_atributo = true;
				
			}//if (getSize1Matriz(idUsinaElevatoria, AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_minima) == 0) {
			
			if (getSize1Matriz(idUsinaElevatoria, AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_maxima) == 0) {

				if (getSizeVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_vazao_bombeada_disponivel_maxima) == 0) {


					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						for (IdPatamarCarga idPatamarCarga_aux = IdPatamarCarga_1; idPatamarCarga_aux <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga_aux++)
							vetorUsinaElevatoria.att(idUsinaElevatoria).addElemento(AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_maxima, periodo, idPatamarCarga_aux, getAtributo(idUsinaElevatoria, AttComumUsinaElevatoria_vazao_bombeada_disponivel_maxima, double()));

					}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				}
				else {

					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						for (IdPatamarCarga idPatamarCarga_aux = IdPatamarCarga_1; idPatamarCarga_aux <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga_aux++)
							vetorUsinaElevatoria.att(idUsinaElevatoria).addElemento(AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_maxima, periodo, idPatamarCarga_aux, getElementoVetor(idUsinaElevatoria, AttVetorUsinaElevatoria_vazao_bombeada_disponivel_maxima, periodo, double()));

					}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				}//else {

				recarregar_atributo = true;

			}//if (getSize1Matriz(idUsinaElevatoria, AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_maxima) == 0) {

			for (AttMatrizUsinaElevatoria attMatrizUsinaElevatoria = AttMatrizUsinaElevatoria(1); attMatrizUsinaElevatoria < AttMatrizUsinaElevatoria_Excedente; attMatrizUsinaElevatoria++) {

				const AttVetorUsinaElevatoria attVetorUsinaElevatoria_media = getAttVetorUsinaElevatoriaFromChar(std::string(getString(attMatrizUsinaElevatoria) + "_media").c_str());

				if (attVetorUsinaElevatoria_media != AttVetorUsinaElevatoria_Nenhum) {

					SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz = getMatriz(idUsinaElevatoria, attMatrizUsinaElevatoria, Periodo(), IdPatamarCarga(), double());

					if (matriz.size() > 0) {

						SmartEnupla<Periodo, double> vetor(matriz, 0.0);

						for (Periodo periodo = matriz.getIteradorInicial(); periodo <= periodo_final_estudo; matriz.incrementarIterador(periodo)) {
							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= matriz.at(periodo).getIteradorFinal(); idPatamarCarga++)
								vetor.at(periodo) += matriz.at(periodo).at(idPatamarCarga) * getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
						}

						vetorUsinaElevatoria.att(idUsinaElevatoria).setVetor_forced(attVetorUsinaElevatoria_media, vetor);

					} // if (matriz.size() > 0) {

				} // if (attVetorUsinaElevatoria_media != AttVetorUsinaElevatoria_Nenhum) {

			} // for (AttMatrizUsinaElevatoria attMatrizUsinaElevatoria = AttMatrizUsinaElevatoria(1); attMatrizUsinaElevatoria < AttMatrizUsinaElevatoria_Excedente; attMatrizUsinaElevatoria++) {

			std::vector<IdRestricaoOperativaUHE> vetor_idRestricaoOperativaUHE_comUsinaBombeada;

			if ((getAtributo(AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_hidraulicas, bool())) && (a_imprimir_atributos_sem_recarregar)) {
				for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= getMaiorId(IdRestricaoOperativaUHE()); idRestricaoOperativaUHE++) {
					if ((getAtributo(idRestricaoOperativaUHE, IdElementoSistema_1, AttComumElementoSistema_usina_elevatoria, IdUsinaElevatoria()) == idUsinaElevatoria) && (getAtributo(idRestricaoOperativaUHE, IdElementoSistema_1, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa()) == TipoVariavelRestricaoOperativa_vazao_bombeada) && (isRestricaoOperativaUHE_simples(idRestricaoOperativaUHE)))
						vetor_idRestricaoOperativaUHE_comUsinaBombeada.push_back(idRestricaoOperativaUHE);
				}
			} // if (getAtributo(AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_hidraulicas, bool())) {

			if (vetor_idRestricaoOperativaUHE_comUsinaBombeada.size() > 0) {
				for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
					for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {

						for (int pos = 0; pos < int(vetor_idRestricaoOperativaUHE_comUsinaBombeada.size()); pos++) {

							if (getElementoMatriz(vetor_idRestricaoOperativaUHE_comUsinaBombeada.at(pos), AttMatrizRestricaoOperativaUHE_limite_inferior, periodo, idPatamarCarga, double()) > getElementoMatriz(idUsinaElevatoria, AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_minima, periodo, idPatamarCarga, double()))
								vetorUsinaElevatoria.att(idUsinaElevatoria).setElemento(AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_minima, periodo, idPatamarCarga, getElementoMatriz(vetor_idRestricaoOperativaUHE_comUsinaBombeada.at(pos), AttMatrizRestricaoOperativaUHE_limite_inferior, periodo, idPatamarCarga, double()));

							if (getElementoMatriz(vetor_idRestricaoOperativaUHE_comUsinaBombeada.at(pos), AttMatrizRestricaoOperativaUHE_limite_superior, periodo, idPatamarCarga, double()) < getElementoMatriz(idUsinaElevatoria, AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_maxima, periodo, idPatamarCarga, double()))
								vetorUsinaElevatoria.att(idUsinaElevatoria).setElemento(AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_maxima, periodo, idPatamarCarga, getElementoMatriz(vetor_idRestricaoOperativaUHE_comUsinaBombeada.at(pos), AttMatrizRestricaoOperativaUHE_limite_superior, periodo, idPatamarCarga, double()));

						}//for (int pos = 0; pos < int(vetor_idRestricaoOperativaUHE_comUsinaBombeada.size()); pos++) {
					} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {
				} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
			} // if (vetor_idRestricaoOperativaUHE_comUHE_volume_util.size() > 0) {

		}//for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria <= maiorIdUsinaElevatoria; idUsinaElevatoria++) {

		validaUsinaElevatoria();


		if (idProcesso == IdProcesso_mestre) {

			if (true) {
				bool impresso = false;
				for (AttMatrizUsinaElevatoria attMatrizUsinaElevatoria = AttMatrizUsinaElevatoria(1); attMatrizUsinaElevatoria < AttMatrizUsinaElevatoria_Excedente; attMatrizUsinaElevatoria++) {

					const AttVetorUsinaElevatoria attVetorUsinaElevatoria_media = getAttVetorUsinaElevatoriaFromChar(std::string(getString(attMatrizUsinaElevatoria) + "_media").c_str());

					if (attVetorUsinaElevatoria_media != AttVetorUsinaElevatoria_Nenhum) {

						a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

						a_entradaSaidaDados.setAppendArquivo(impresso);
						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_USINA_ELEVATORIA_AttVetorPremissa_PorPeriodo.csv", IdUsinaElevatoria_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, attVetorUsinaElevatoria_media);
						impresso = true;

						for (IdUsinaElevatoria idUsinaElevatoria = IdUsinaElevatoria_1; idUsinaElevatoria <= maiorIdUsinaElevatoria; idUsinaElevatoria++)
							vetorUsinaElevatoria.att(idUsinaElevatoria).clear(attVetorUsinaElevatoria_media);

					}

				} // for (AttMatrizUsinaElevatoria attMatrizUsinaElevatoria = AttMatrizUsinaElevatoria(1); attMatrizUsinaElevatoria < AttMatrizUsinaElevatoria_Nenhum; attMatrizUsinaElevatoria++) {
			} // if (true) {

			if (a_imprimir_atributos_sem_recarregar) {

				a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

				a_entradaSaidaDados.setAppendArquivo(false);

				const std::vector<AttComumUsinaElevatoria> lista_USINA_ELEVATORIA_AttComumOperacional{ AttComumUsinaElevatoria_idUsinaElevatoria, AttComumUsinaElevatoria_nome, AttComumUsinaElevatoria_submercado, \
					AttComumUsinaElevatoria_taxa_de_consumo, AttComumUsinaElevatoria_usina_origem_bombeamento, AttComumUsinaElevatoria_usina_destino_bombeamento, AttComumUsinaElevatoria_penalidade_vazao_bombeada_minima };

				a_entradaSaidaDados.imprimirArquivoCSV_AttComum("USINA_ELEVATORIA_AttComumOperacional.csv", IdUsinaElevatoria_Nenhum, *this, lista_USINA_ELEVATORIA_AttComumOperacional);

				a_entradaSaidaDados.setAppendArquivo(false);
				a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("USINA_ELEVATORIA_AttVetorOperacional_PorPeriodo.csv", IdUsinaElevatoria_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, AttVetorUsinaElevatoria_fator_disponibilidade);

				a_entradaSaidaDados.setAppendArquivo(true);
				a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("USINA_ELEVATORIA_AttVetorOperacional_PorPeriodo.csv", IdUsinaElevatoria_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, AttVetorUsinaElevatoria_vazao_bombeada_minima);
				a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("USINA_ELEVATORIA_AttVetorOperacional_PorPeriodo.csv", IdUsinaElevatoria_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, AttVetorUsinaElevatoria_vazao_bombeada_maxima);

				a_entradaSaidaDados.setAppendArquivo(false);
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("USINA_ELEVATORIA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", IdUsinaElevatoria_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_maxima);
				a_entradaSaidaDados.setAppendArquivo(true);
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("USINA_ELEVATORIA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", IdUsinaElevatoria_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizUsinaElevatoria_vazao_bombeada_disponivel_minima);

			}

		}//if (idProcesso == IdProcesso_mestre) {

		if (!a_imprimir_atributos_sem_recarregar) {

			if (true) {
				int barreira = 0;

				if (arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso()) == IdProcesso_mestre) {
					for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++)
						MPI_Send(&barreira, 1, MPI_INT, getRank(idProcesso), 0, MPI_COMM_WORLD);
				}
				else
					MPI_Recv(&barreira, 1, MPI_INT, getRank(IdProcesso_mestre), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}

			if (recarregar_atributo) {

				a_entradaSaidaDados.setDiretorioEntrada(a_diretorio_att_operacional);

				a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("USINA_ELEVATORIA_AttComumOperacional.csv", *this, TipoAcessoInstancia_membro);

				a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("USINA_ELEVATORIA_AttVetorOperacional_PorPeriodo.csv", *this, TipoAcessoInstancia_membro);

				a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("USINA_ELEVATORIA_AttMatrizOperacional_PorPeriodoPorIdPatamar.csv", *this, TipoAcessoInstancia_membro);

			}//if (recarregar_atributo) {

		}//if (!a_imprimir_atributos_sem_recarregar) {

	}
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validacao_operacional_UsinasElevatorias(a_entradaSaidaDados, " + a_diretorio_att_operacional + "," + a_diretorio_att_premissa + "," + getString(a_imprimir_atributos_sem_recarregar) + "): \n" + std::string(erro.what())); }

}

void Dados::validacao_operacional_Termeletrica(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar) {

	try {

		//  VALIDA AS UNIDADES TERMEL�TRICAS
		validaUnidadeUTE();

		// VALIDA AS TERMEL�TRICAS 
		validaTermeletrica();

		//MAIOR ID TERMELETRICA
		const IdTermeletrica maiorIdTermeletrica = getMaiorId(IdTermeletrica());
		const IdTermeletrica menorIdTermeletrica = getMenorId(IdTermeletrica());

		const int numIdTermeletrica = int(maiorIdTermeletrica - menorIdTermeletrica) + 1;

		bool recarregar_AttVetorReservatorio_PorIdMes = false;
		bool recarregar_AttVetorReservatorio_PorPeriodo = false;
		bool recarregar_AttVetorTermeletrica_PorPeriodo = false;
		bool recarregar_AttVetorTermeletrica_PorInteiro = false;
		bool recarregar_AttMatrizTermeletrica_COMANDO_PorPeriodoPorIdPatamarCarga = false;
		bool recarregar_AttMatrizTermeletrica_PorPeriodoPorIdPatamarCarga = false;
		bool recarregar_AttVetorUnidadeUTE_PorPeriodo = false;
		bool recarregar_AttMatrizUnidadeUTE_PorPeriodoPorIdPatamarCarga = false;
		bool impresso_AttComumUnidadeUTE = false;
		bool impresso_AttComumTermeletrica = false;

		std::vector<bool> impresso_AttVetorTermeletrica_PorPeriodo(3, false);
		std::vector<bool> impresso_AttVetorTermeletrica_PorInteiro(2, false);
		std::vector<bool> impresso_AttMatrizTermeletrica_COMANDO_PorPeriodoPorIdPatamarCarga(2, false);
		std::vector<bool> impresso_AttMatrizTermeletrica_PorPeriodoPorIdPatamarCarga(2, false);
		std::vector<bool> impresso_AttMatrizTermeletrica_PorIdCenarioPorPeriodo(2, false);
		std::vector<bool> impresso_AttVetorUnidadeUTE_PorInteiro(2, false);
		std::vector<bool> impresso_AttVetorUnidadeUTE_PorPeriodo(2, false);
		std::vector<bool> impresso_AttMatrizUnidadeUTE_PorPeriodoPorIdPatamarCarga(2, false);

		// SMART ENUPLA AUXILIARES
		SmartEnupla<IdTermeletrica, SmartEnupla<AttVetorTermeletrica, PreencherAtributo>> preencher_AttVetorTermeletrica(menorIdTermeletrica, std::vector<SmartEnupla<AttVetorTermeletrica, PreencherAtributo>>(numIdTermeletrica, SmartEnupla<AttVetorTermeletrica, PreencherAtributo>()));
		SmartEnupla<IdTermeletrica, SmartEnupla<AttMatrizTermeletrica, PreencherAtributo>> preencher_AttMatrizTermeletrica(menorIdTermeletrica, std::vector<SmartEnupla<AttMatrizTermeletrica, PreencherAtributo>>(numIdTermeletrica, SmartEnupla<AttMatrizTermeletrica, PreencherAtributo>()));

		SmartEnupla<IdTermeletrica, SmartEnupla<IdUnidadeUTE, SmartEnupla<AttVetorUnidadeUTE, PreencherAtributo>>> preencher_AttVetorUnidadeUTE(menorIdTermeletrica, std::vector<SmartEnupla<IdUnidadeUTE, SmartEnupla<AttVetorUnidadeUTE, PreencherAtributo>>>(numIdTermeletrica, SmartEnupla<IdUnidadeUTE, SmartEnupla<AttVetorUnidadeUTE, PreencherAtributo>>()));
		SmartEnupla<IdTermeletrica, SmartEnupla<IdUnidadeUTE, SmartEnupla<AttMatrizUnidadeUTE, PreencherAtributo>>> preencher_AttMatrizUnidadeUTE(menorIdTermeletrica, std::vector< SmartEnupla<IdUnidadeUTE, SmartEnupla<AttMatrizUnidadeUTE, PreencherAtributo>>>(numIdTermeletrica, SmartEnupla<IdUnidadeUTE, SmartEnupla<AttMatrizUnidadeUTE, PreencherAtributo>>()));

		// ID PROCESSO
		const IdProcesso idProcesso = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());

		// HORIZONTE DE ESTUDO 
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const SmartEnupla<Periodo, double> vetor_zero(horizonte_estudo, 0.0);

		const int numeroPeriodos = getSizeVetor(AttVetorDados_horizonte_estudo);

		Periodo periodo_estudo_inicial = horizonte_estudo.getIteradorInicial();

		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();

		if (getAtributo(AttComumDados_estagio_inicial, IdEstagio()) > IdEstagio_1) {
			for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {
				if (horizonte_estudo.at(periodo) == getAtributo(AttComumDados_estagio_inicial, IdEstagio())) {
					periodo_estudo_inicial = periodo;
					break;
				}
			}
		}

		// MAIOR PATAMAR DE CARGA DO HORIZONTE DE ESTUDO 
		IdPatamarCarga maiorIdPatamarCarga_horizonte = IdPatamarCarga_Nenhum;
		for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {
			const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
			if (maiorIdPatamarCarga > maiorIdPatamarCarga_horizonte)
				maiorIdPatamarCarga_horizonte = maiorIdPatamarCarga;
		} // for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {


		for (IdTermeletrica idTermeletrica = menorIdTermeletrica; idTermeletrica <= maiorIdTermeletrica; vetorTermeletrica.incr(idTermeletrica)) {

			if (true) {
				//if (getAtributo(idTermeletrica, AttComumTermeletrica_considerar_usina, bool())) {

				const IdUnidadeUTE maiorIdUnidadeUTE = getMaiorId(idTermeletrica, IdUnidadeUTE());

				bool termeletrica_com_unidade = false;
				if (maiorIdUnidadeUTE != IdUnidadeUTE_Nenhum)
					termeletrica_com_unidade = true;


				// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				//
				//                                                                                   TERMEL�TRICA
				//
				// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

				preencher_AttVetorTermeletrica.at(idTermeletrica) = SmartEnupla<AttVetorTermeletrica, PreencherAtributo>(AttVetorTermeletrica(AttVetorTermeletrica_Nenhum + 1), std::vector<PreencherAtributo>(AttVetorTermeletrica(AttVetorTermeletrica_Excedente - 1), nao_sem_utilizacao));
				preencher_AttMatrizTermeletrica.at(idTermeletrica) = SmartEnupla<AttMatrizTermeletrica, PreencherAtributo>(AttMatrizTermeletrica(AttMatrizTermeletrica_Nenhum + 1), std::vector<PreencherAtributo>(AttMatrizTermeletrica(AttMatrizTermeletrica_Excedente - 1), nao_sem_utilizacao));




				// ------------------------------------
				//
				// AttVetorTermeletrica_tipo_detalhamento_producao
				//
				// ------------------------------------
				// 


				if (getSizeVetor(idTermeletrica, AttVetorTermeletrica_tipo_detalhamento_producao) == 0) {

					const TipoDetalhamentoProducaoTermeletrica tipo_detalhamento_producao = getAtributo(idTermeletrica, AttComumTermeletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoTermeletrica());

					if ((tipo_detalhamento_producao != TipoDetalhamentoProducaoTermeletrica_sem_producao) && (!termeletrica_com_unidade))
						throw std::invalid_argument("O atributo " + getFullString(AttComumTermeletrica_tipo_detalhamento_producao) + " nao deve ser " + getFullString(tipo_detalhamento_producao) + " caso nao sejam informadas unidades em " + getFullString(idTermeletrica) + ".");

					preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_tipo_detalhamento_producao) = sim_operacional;
					vetorTermeletrica.att(idTermeletrica).setVetor(AttVetorTermeletrica_tipo_detalhamento_producao, SmartEnupla<Periodo, TipoDetalhamentoProducaoTermeletrica>(horizonte_estudo, tipo_detalhamento_producao));

				} // if (getSizeVetor(idTermeletrica, AttVetorTermeletrica_tipo_detalhamento_producao) == 0) {

				else if (getSizeVetor(idTermeletrica, AttVetorTermeletrica_tipo_detalhamento_producao) > 0)
					preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_tipo_detalhamento_producao) = nao_operacional_informado;


				SmartEnupla<TipoDetalhamentoProducaoTermeletrica, SmartEnupla<Periodo, double>>  vetor_zero(TipoDetalhamentoProducaoTermeletrica(1), std::vector<SmartEnupla<Periodo, double>>(TipoDetalhamentoProducaoTermeletrica(TipoDetalhamentoProducaoTermeletrica_Excedente - 1), SmartEnupla<Periodo, double>()));
				SmartEnupla<TipoDetalhamentoProducaoTermeletrica, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>> matriz_zero(TipoDetalhamentoProducaoTermeletrica(1), std::vector<SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>>(TipoDetalhamentoProducaoTermeletrica(TipoDetalhamentoProducaoTermeletrica_Excedente - 1), SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>()));

				for (TipoDetalhamentoProducaoTermeletrica tipo_detalhamento_producao = TipoDetalhamentoProducaoTermeletrica(1); tipo_detalhamento_producao < TipoDetalhamentoProducaoTermeletrica_Excedente; tipo_detalhamento_producao++) {

					int contador_encontro = 0;
					bool tipo_encontrado = false;

					for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

						const TipoDetalhamentoProducaoTermeletrica tipo_detalhamento_producao_carregado = getElementoVetor(idTermeletrica, AttVetorTermeletrica_tipo_detalhamento_producao, periodo, TipoDetalhamentoProducaoTermeletrica());

						const SmartEnupla<IdPatamarCarga, double> vetor_zero_patamar(IdPatamarCarga_1, std::vector<double>(getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()), 0.0));

						if ((tipo_detalhamento_producao == tipo_detalhamento_producao_carregado) && (!tipo_encontrado)) {
							tipo_encontrado = true;
							contador_encontro++;
							if (contador_encontro > 1)
								throw std::invalid_argument("Os elementos de " + getFullString(AttVetorTermeletrica_tipo_detalhamento_producao) + " devem ser informados em blocos, sem repeticao de blocos ao longo do horizonte.");
							vetor_zero.at(tipo_detalhamento_producao).addElemento(periodo, 0.0);
							matriz_zero.at(tipo_detalhamento_producao).addElemento(periodo, vetor_zero_patamar);
						}

						else if ((tipo_detalhamento_producao == tipo_detalhamento_producao_carregado) && (tipo_encontrado)) {
							vetor_zero.at(tipo_detalhamento_producao).addElemento(periodo, 0.0);
							matriz_zero.at(tipo_detalhamento_producao).addElemento(periodo, vetor_zero_patamar);
						}

						else if (tipo_detalhamento_producao != tipo_detalhamento_producao_carregado)
							tipo_encontrado = false;

					} // for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

					if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_sem_producao).size() > 0) {
						if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoTermeletrica_sem_producao) && (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_sem_producao).size() != horizonte_estudo.size()))
							throw std::invalid_argument("Os elementos " + getFullString(TipoDetalhamentoProducaoTermeletrica_sem_producao) + " de " + getFullString(AttVetorTermeletrica_tipo_detalhamento_producao) + " quando informados devem constar em todo o horizonte.");
					}

				} // for (TipoDetalhamentoProducaoTermeletrica tipo_detalhamento_producao = TipoDetalhamentoProducaoTermeletrica(1); tipo_detalhamento_producao < TipoDetalhamentoProducaoTermeletrica_Excedente; tipo_detalhamento_producao++) {


				// -------------------------------------
				//
				// AttMatrizTermeletrica_potencia_util
				//
				// -------------------------------------
				SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> potencia_util_termeletrica;

				if (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_util) == 0) {
					if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_sem_producao).size() < horizonte_estudo.size()) {
						preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_util) = sim_operacional;
						potencia_util_termeletrica = matriz_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina);
					}
				}
				else if (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_util) > 0) {

					if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_sem_producao).size() >= horizonte_estudo.size())
						throw std::invalid_argument("O atributo " + getFullString(AttMatrizTermeletrica_potencia_util) + " nao deve ser informado em " + getFullString(idTermeletrica));
					else
						preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_util) = nao_operacional_informado;

				} // else if (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_util) > 0) {



				// -------------------------------------
				//
				// AttMatrizTermeletrica_potencia_disponivel_minima
				//
				// -------------------------------------

				if ((getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_minima) == 0) && (termeletrica_com_unidade)) {

					if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).size() > 0)
						preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_disponivel_minima) = sim_operacional;

				} // if ((getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_minima) == 0) && (termeletrica_com_unidade)) {

				else if (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_minima) > 0) {

					if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_sem_producao).size() >= horizonte_estudo.size())
						throw std::invalid_argument("O atributo " + getFullString(AttMatrizTermeletrica_potencia_disponivel_minima) + " nao deve ser informado em " + getFullString(idTermeletrica));
					else
						preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_disponivel_minima) = nao_operacional_informado;

				} // else if (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_minima) > 0) {


				// -------------------------------------
				//
				// AttMatrizTermeletrica_potencia_disponivel_maxima
				//
				// -------------------------------------

				if ((getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_maxima) == 0) && (termeletrica_com_unidade)) {

					if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).size() > 0) {
						preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_disponivel_maxima) = sim_operacional;

						if (getSizeVetor(idTermeletrica, AttVetorTermeletrica_fator_de_capacidade) == 0) {
							preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_fator_de_capacidade) = sim_premissa;
							vetorTermeletrica.att(idTermeletrica).setVetor(AttVetorTermeletrica_fator_de_capacidade, SmartEnupla<Periodo, double>(vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina), getAtributo(idTermeletrica, AttComumTermeletrica_fator_de_capacidade, double())));
						}
						else
							preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_fator_de_capacidade) = nao_premissa_informado;

					} // if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).size() > 0) {

				} // if ((getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_maxima) == 0) && (termeletrica_com_unidade)) {

				else if (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_maxima) > 0) {

					if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_sem_producao).size() >= horizonte_estudo.size())
						throw std::invalid_argument("O atributo " + getFullString(AttMatrizTermeletrica_potencia_disponivel_maxima) + " nao deve ser informado em " + getFullString(idTermeletrica));
					else
						preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_disponivel_maxima) = nao_operacional_informado;

				} // else if (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_maxima) > 0) {


				// -------------------------------------
				//
				// AttMatrizTermeletrica_potencia_disponivel_comandada
				//
				// -------------------------------------


				if (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_comandada) > 0) {

					if (getAtributo(idTermeletrica, AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada, int()) == 0)
						throw std::invalid_argument("O atributo " + getFullString(AttMatrizTermeletrica_potencia_disponivel_comandada) + " nao deve ser informado em " + getFullString(idTermeletrica) + " de " + getFullString(AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada) + " zero.");

					if (getAtributo(AttComumDados_estagio_inicial, IdEstagio()) > IdEstagio_1)
						throw std::invalid_argument("O atributo " + getFullString(AttMatrizTermeletrica_potencia_disponivel_comandada) + " nao deve ser informado em " + getFullString(idTermeletrica) + " pois estudo inicia em " + getFullString(getAtributo(AttComumDados_estagio_inicial, IdEstagio())) + ".");

					const Periodo periodo_inicial_potencia_disponivel_comandada = getIterador1Inicial(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_comandada, Periodo());
					const Periodo periodo_final_potencia_disponivel_comandada = getIterador1Final(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_comandada, Periodo());

					if (((periodo_inicial_potencia_disponivel_comandada <= periodo_estudo_inicial) || (periodo_inicial_potencia_disponivel_comandada.sobreposicao(periodo_estudo_inicial) > 0.0)) && \
						((periodo_final_potencia_disponivel_comandada >= periodo_estudo_inicial) || (periodo_final_potencia_disponivel_comandada.sobreposicao(periodo_estudo_inicial) > 0.0))) {
					}
					else
						throw std::invalid_argument("O periodo inicial do atributo " + getFullString(AttMatrizTermeletrica_potencia_disponivel_comandada) + " em " + getFullString(idTermeletrica) + " deve iniciar com horizonte de estudo.");

					preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_disponivel_comandada) = nao_operacional_informado;

				} // if (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_comandada) > 0) {


				// -----------------------------------------------
				//
				// AttMatrizTermeletrica_potencia_disponivel_meta
				//
				// ----------------------------------------------
				// 

				if ((getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_meta) > 0) && (getAtributo(AttComumDados_tipo_estudo, TipoEstudo()) == TipoEstudo_simulacao)) {
					if (getAtributo(AttComumDados_numero_cenarios, int()) != getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_meta))
						throw std::invalid_argument("Numero de cenarios em " + getFullString(AttMatrizTermeletrica_potencia_disponivel_meta) + " diferente de " + getFullString(AttComumDados_numero_cenarios));
					//preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_disponivel_meta) = nao_operacional_informado;
				}
				else if (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_meta) > 0)
					throw std::invalid_argument("Atributo " + getFullString(AttMatrizTermeletrica_potencia_disponivel_meta) + " em " + getFullString(idTermeletrica) + " nao compativel com " + getFullString(getAtributo(AttComumDados_tipo_estudo, TipoEstudo())));


				// -------------------------------------
				//
				// AttMatrizTermeletrica_potencia_minima
				//
				// -------------------------------------

				SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> potencia_minima_termeletrica;

				if ((getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_minima) == 0) && (termeletrica_com_unidade)) {

					if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).size() > 0) {
						preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_minima) = sim_operacional;
						potencia_minima_termeletrica = matriz_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina);
					} // if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).size() > 0) {

				} // if ((getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_minima) == 0) && (termeletrica_com_unidade)) {

				else if (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_minima) > 0) {

					if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_sem_producao).size() >= horizonte_estudo.size())
						throw std::invalid_argument("O atributo " + getFullString(AttMatrizTermeletrica_potencia_minima) + " nao deve ser informado em " + getFullString(idTermeletrica));
					else
						preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_minima) = nao_operacional_informado;

				} // else if (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_minima) > 0) {

				// -------------------------------------
				//
				// AttMatrizTermeletrica_potencia_maxima
				//
				// -------------------------------------

				SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> potencia_maxima_termeletrica;

				if ((getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_maxima) == 0) && (termeletrica_com_unidade)) {

					if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).size() > 0) {
						preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_maxima) = sim_premissa;
						potencia_maxima_termeletrica = matriz_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina);
					} // if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).size() > 0) {

					if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_unidade).size() > 0) {
						preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_maxima) = sim_operacional;
						potencia_maxima_termeletrica = matriz_zero.at(TipoDetalhamentoProducaoTermeletrica_por_unidade);
					}

				} // if ((getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_maxima) == 0) && (termeletrica_com_unidade)) {

				else if (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_maxima) > 0) {

					if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_sem_producao).size() >= horizonte_estudo.size())
						throw std::invalid_argument("O atributo " + getFullString(AttMatrizTermeletrica_potencia_maxima) + " nao deve ser informado em " + getFullString(idTermeletrica));
					else if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_unidade).size() > 0)
						preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_maxima) = nao_operacional_informado;
					else
						preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_maxima) = nao_premissa_informado;

				} // else if (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_maxima) > 0) {


				// -------------------------------------
				//
				// AttMatrizTermeletrica_custo_de_operacao
				//
				// -------------------------------------

				SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> custo_de_operacao_termeletrica;

				if ((getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_custo_de_operacao) == 0) && (termeletrica_com_unidade)) {

					if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).size() > 0) {

						preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_custo_de_operacao) = sim_operacional;

						custo_de_operacao_termeletrica = matriz_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina);

					} // if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).size() > 0) {

				} // if ((getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_custo_de_operacao) == 0) && (termeletrica_com_unidade)) {

				else if (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_custo_de_operacao) > 0) {

					if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).size() > 0)
						preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_custo_de_operacao) = nao_operacional_informado;

					else
						throw std::invalid_argument("O atributo " + getFullString(AttMatrizTermeletrica_custo_de_operacao) + " nao deve ser informado em " + getFullString(idTermeletrica));

				} // else if (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_custo_de_operacao) > 0) {


				// ------------------------------------
				//
				// AttVetorTermeletrica_disponibilidade
				//
				// ------------------------------------
				// 

				SmartEnupla<Periodo, double> disponibilidade_termeletrica;

				if ((getSizeVetor(idTermeletrica, AttVetorTermeletrica_disponibilidade) == 0) && (termeletrica_com_unidade)) {

					if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).size() > 0) {

						preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_disponibilidade) = sim_operacional;

						disponibilidade_termeletrica = vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina);

					} // if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).size() > 0) {

				} // if ((getSizeVetor(idTermeletrica, AttVetorTermeletrica_disponibilidade) == 0) && (termeletrica_com_unidade)) {

				else if (getSizeVetor(idTermeletrica, AttVetorTermeletrica_disponibilidade) > 0) {

					if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).size() > 0)
						preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_disponibilidade) = nao_operacional_informado;

					else
						throw std::invalid_argument("O atributo " + getFullString(AttVetorTermeletrica_disponibilidade) + " nao deve ser informado em " + getFullString(idTermeletrica));

				} // else if (getSizeVetor(idTermeletrica, AttVetorTermeletrica_disponibilidade) > 0) {

				// ------------------------------------------------
				//
				// AttComumTermeletrica_penalidade_violacao_potencia
				//
				// ------------------------------------------------

				if (getAtributo(idTermeletrica, AttComumTermeletrica_penalidade_violacao_potencia, double()) == 0.0) {

					const IdSubmercado idSubmercado = getAtributo(idTermeletrica, AttComumTermeletrica_submercado, IdSubmercado());

					const double custo_deficit = getElementoMatriz(idSubmercado, IdPatamarDeficit_1, AttMatrizPatamarDeficit_custo, horizonte_estudo.getIteradorInicial(), IdPatamarCarga_1, double());

					vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_penalidade_violacao_potencia, 1.5 * custo_deficit);

				}//if (getAtributo(idTermeletrica, AttComumTermeletrica_penalidade_potencia_minima, double()) == 0.0) {


				// ------------------------------------------------
				//
				// AttComumTermeletrica_penalidade_violacao_comando
				//
				// ------------------------------------------------

				if (getAtributo(idTermeletrica, AttComumTermeletrica_penalidade_violacao_comando, double()) == 0.0) {

					const IdSubmercado idSubmercado = getAtributo(idTermeletrica, AttComumTermeletrica_submercado, IdSubmercado());

					const double custo_deficit = getElementoMatriz(idSubmercado, IdPatamarDeficit_1, AttMatrizPatamarDeficit_custo, horizonte_estudo.getIteradorInicial(), IdPatamarCarga_1, double());

					vetorTermeletrica.att(idTermeletrica).setAtributo(AttComumTermeletrica_penalidade_violacao_comando, custo_deficit);

				}//if (getAtributo(idTermeletrica, AttComumTermeletrica_penalidade_potencia_minima, double()) == 0.0) {

				// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				//
				//                                                                                            UnidadeUTE
				//
				// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

				preencher_AttVetorUnidadeUTE.at(idTermeletrica) = SmartEnupla<IdUnidadeUTE, SmartEnupla<AttVetorUnidadeUTE, PreencherAtributo>>();
				preencher_AttMatrizUnidadeUTE.at(idTermeletrica) = SmartEnupla<IdUnidadeUTE, SmartEnupla<AttMatrizUnidadeUTE, PreencherAtributo>>();

				if (maiorIdUnidadeUTE == IdUnidadeUTE_Nenhum) {

					if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_unidade).size() > 0)
						throw std::invalid_argument("Para " + getFullString(TipoDetalhamentoProducaoTermeletrica_por_unidade) + " deve haver ao menos uma unidade termeletrica.");

				} // if (maiorIdUnidadeUTE == IdUnidadeUTE_Nenhum) {



				for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= maiorIdUnidadeUTE; idUnidadeUTE++) {

					preencher_AttVetorUnidadeUTE.at(idTermeletrica).addElemento(idUnidadeUTE, SmartEnupla<AttVetorUnidadeUTE, PreencherAtributo>(AttVetorUnidadeUTE(AttVetorUnidadeUTE_Nenhum + 1), std::vector<PreencherAtributo>(AttVetorUnidadeUTE(AttVetorUnidadeUTE_Excedente - 1), nao_sem_utilizacao)));
					preencher_AttMatrizUnidadeUTE.at(idTermeletrica).addElemento(idUnidadeUTE, SmartEnupla<AttMatrizUnidadeUTE, PreencherAtributo>(AttMatrizUnidadeUTE(AttMatrizUnidadeUTE_Nenhum + 1), std::vector<PreencherAtributo>(AttMatrizUnidadeUTE(AttMatrizUnidadeUTE_Excedente - 1), nao_sem_utilizacao)));


					// ----------------------------------
					//
					// AttVetorUnidadeUTE_disponibilidade
					//
					// ----------------------------------

					if (getSizeVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_disponibilidade) == 0) {

						SmartEnupla<Periodo, double> disponibilidade_unidade;

						if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_unidade).size() > 0) {
							preencher_AttVetorUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttVetorUnidadeUTE_disponibilidade) = sim_operacional;
							disponibilidade_unidade = vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_unidade);
						}

						else if ((preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_disponibilidade) == sim_premissa) || (preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_disponibilidade) == sim_operacional)) {
							preencher_AttVetorUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttVetorUnidadeUTE_disponibilidade) = sim_premissa;
							disponibilidade_unidade = SmartEnupla<Periodo, double>(disponibilidade_termeletrica, 0.0);
						}

						else if ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_minima) == sim_premissa) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_minima) == sim_operacional)) {
							preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_minima) = sim_premissa;
							for (Periodo periodo = potencia_minima_termeletrica.getIteradorInicial(); periodo <= potencia_minima_termeletrica.getIteradorFinal(); potencia_minima_termeletrica.incrementarIterador(periodo))
								disponibilidade_unidade.addElemento(periodo, 0.0);
						}

						else if ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_maxima) == sim_premissa) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_maxima) == sim_operacional)) {
							preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_maxima) = sim_premissa;
							for (Periodo periodo = potencia_maxima_termeletrica.getIteradorInicial(); periodo <= potencia_maxima_termeletrica.getIteradorFinal(); potencia_maxima_termeletrica.incrementarIterador(periodo))
								disponibilidade_unidade.addElemento(periodo, 0.0);
						}

						if ((preencher_AttVetorUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttVetorUnidadeUTE_disponibilidade) == sim_premissa) || (preencher_AttVetorUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttVetorUnidadeUTE_disponibilidade) == sim_operacional)) {

							// AttVetorTermeletrica_indisponibilidade_forcada

							if (getSizeVetor(idTermeletrica, AttVetorTermeletrica_indisponibilidade_forcada) == 0) {
								vetorTermeletrica.att(idTermeletrica).setVetor(AttVetorTermeletrica_indisponibilidade_forcada, SmartEnupla<Periodo, double>(disponibilidade_unidade, getAtributo(idTermeletrica, AttComumTermeletrica_indisponibilidade_forcada, double())));
								preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_indisponibilidade_forcada) = sim_premissa;
							}
							else
								preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_indisponibilidade_forcada) = nao_premissa_informado;

							// AttVetorTermeletrica_indisponibilidade_programada

							if (getSizeVetor(idTermeletrica, AttVetorTermeletrica_indisponibilidade_programada) == 0) {
								vetorTermeletrica.att(idTermeletrica).setVetor(AttVetorTermeletrica_indisponibilidade_programada, SmartEnupla<Periodo, double>(disponibilidade_unidade, getAtributo(idTermeletrica, AttComumTermeletrica_indisponibilidade_programada, double())));
								preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_indisponibilidade_programada) = sim_premissa;
							}
							else
								preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_indisponibilidade_programada) = nao_premissa_informado;

							for (Periodo periodo = disponibilidade_unidade.getIteradorInicial(); periodo <= disponibilidade_unidade.getIteradorFinal(); disponibilidade_unidade.incrementarIterador(periodo)) {

								const double indisponibilidade_forcada = getElementoVetor(idTermeletrica, AttVetorTermeletrica_indisponibilidade_forcada, periodo, double());
								const double indisponibilidade_programada = getElementoVetor(idTermeletrica, AttVetorTermeletrica_indisponibilidade_programada, periodo, double());
								double disponibilidade = (1.0 - indisponibilidade_forcada) * (1.0 - indisponibilidade_programada);

								if (disponibilidade < 0)
									disponibilidade = -disponibilidade;

								if (doubleCompara(0.0, disponibilidade))
									disponibilidade = 0.0;
								else if (doubleCompara(1.0, disponibilidade))
									disponibilidade = 1.0;

								vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).addElemento(AttVetorUnidadeUTE_disponibilidade, periodo, disponibilidade);

							} // for (Periodo periodo = disponibilidade_unidade.getIteradorInicial(); periodo <= disponibilidade_unidade.getIteradorFinal(); disponibilidade_unidade.incrementarIterador(periodo)) {

						} // if ((preencher_AttVetorUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttVetorUnidadeUTE_disponibilidade) == sim_premissa) || (preencher_AttVetorUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttVetorUnidadeUTE_disponibilidade) == sim_operacional)) {

					} // if (getSizeVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_disponibilidade) == 0) {

					else if (getSizeVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_disponibilidade) > 0) {

						if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_unidade).size() > 0)
							preencher_AttVetorUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttVetorUnidadeUTE_disponibilidade) = nao_operacional_informado;

						else if ((preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_disponibilidade) == sim_premissa) || (preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_disponibilidade) == sim_operacional))
							preencher_AttVetorUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttVetorUnidadeUTE_disponibilidade) = nao_premissa_informado;

						else if ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_minima) == sim_premissa) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_minima) == sim_operacional))
							preencher_AttVetorUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttVetorUnidadeUTE_disponibilidade) = nao_premissa_informado;

						else if ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_maxima) == sim_premissa) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_maxima) == sim_operacional))
							preencher_AttVetorUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttVetorUnidadeUTE_disponibilidade) = nao_premissa_informado;

						else
							throw std::invalid_argument("O atributo " + getFullString(AttVetorUnidadeUTE_disponibilidade) + " nao deve ser informado em " + getFullString(idTermeletrica) + " " + getFullString(idUnidadeUTE));

					} // else if (getSizeVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_disponibilidade) > 0) {



					// -------------------------------------
					//
					// AttMatrizUnidadeUTE_potencia_util
					//
					// -------------------------------------	
					SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> potencia_util_unidade;

					if ((vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_unidade).size() != 0) && (getSize1Matriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_util) == 0)) {
						preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_util) = sim_operacional;
						potencia_util_unidade = matriz_zero.at(TipoDetalhamentoProducaoTermeletrica_por_unidade);
					}

					// -----------------------------------
					//
					// AttMatrizUnidadeUTE_potencia_disponivel_minima
					//
					// -----------------------------------

					if (getSize1Matriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_minima) > 0) {

						if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_unidade).size() > 0)
							preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_disponivel_minima) = nao_operacional_informado;

						/*	else
								throw std::invalid_argument("O atributo " + getFullString(AttMatrizUnidadeUTE_potencia_disponivel_minima) + " nao deve ser informado em " + getFullString(idTermeletrica)  + " " + getFullString(idUnidadeUTE));*/

					} // else if (getSize1Matriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_minima) > 0) {


					// -----------------------------------
					//
					// AttMatrizUnidadeUTE_potencia_disponivel_maxima
					//
					// -----------------------------------

					if (getSize1Matriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_maxima) > 0) {

						if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_unidade).size() > 0)
							preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_disponivel_maxima) = nao_operacional_informado;

						/*	else
								throw std::invalid_argument("O atributo " + getFullString(AttMatrizUnidadeUTE_potencia_disponivel_maxima) + " nao deve ser informado em " + getFullString(idTermeletrica) +  " " + getFullString(idUnidadeUTE));*/

					} // else if (getSize1Matriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_disponivel_maxima) > 0) {


					// -----------------------------------
					//
					// AttMatrizUnidadeUTE_potencia_minima
					//
					// -----------------------------------
					SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> potencia_minima_unidade;

					if (getSize1Matriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_minima) == 0) {

						if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_unidade).size() > 0) {
							preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_minima) = sim_operacional;
							potencia_minima_unidade = matriz_zero.at(TipoDetalhamentoProducaoTermeletrica_por_unidade);
						}

						else if ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_minima) == sim_premissa) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_minima) == sim_operacional)) {
							preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_minima) = sim_premissa;
							potencia_minima_unidade = SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(potencia_minima_termeletrica, SmartEnupla<IdPatamarCarga, double>());
							for (Periodo periodo = potencia_minima_unidade.getIteradorInicial(); periodo <= potencia_minima_unidade.getIteradorFinal(); potencia_minima_unidade.incrementarIterador(periodo))
								potencia_minima_unidade.at(periodo) = SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()), 0.0));
						}

						if ((preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_minima) == sim_premissa) || (preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_minima) == sim_operacional)) {

							for (Periodo periodo = potencia_minima_unidade.getIteradorInicial(); periodo <= potencia_minima_unidade.getIteradorFinal(); potencia_minima_unidade.incrementarIterador(periodo)) {

								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {

									if (getElementoVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_disponibilidade, periodo, double()) > 0.0)
										vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).addElemento(AttMatrizUnidadeUTE_potencia_minima, periodo, idPatamarCarga, getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_potencia_minima, double()));
									else
										vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).addElemento(AttMatrizUnidadeUTE_potencia_minima, periodo, idPatamarCarga, 0.0);

								} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {

							} // for (Periodo periodo = potencia_minima_unidade.getIteradorInicial(); periodo <= potencia_minima_unidade.getIteradorFinal(); potencia_minima_unidade.incrementarIterador(periodo)) {

						} // if ((preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_minima) == sim_premissa) || (preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_minima) == sim_operacional)) {

					} // if (getSize1Matriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_minima) == 0) {

					else if (getSize1Matriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_minima) > 0) {

						if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_unidade).size() > 0)
							preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_minima) = nao_operacional_informado;

						else if ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_minima) == sim_premissa) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_minima) == sim_operacional))
							preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_minima) = nao_premissa_informado;

						else
							throw std::invalid_argument("O atributo " + getFullString(AttMatrizUnidadeUTE_potencia_minima) + " nao deve ser informado em " + getFullString(idTermeletrica) + " " + getFullString(idUnidadeUTE));

					} // else if (getSize1Matriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_minima) > 0) {

					if ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_minima) == sim_premissa) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_minima) == sim_operacional)) {
						for (Periodo periodo = potencia_minima_termeletrica.getIteradorInicial(); periodo <= potencia_minima_termeletrica.getIteradorFinal(); potencia_minima_termeletrica.incrementarIterador(periodo)) {
							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++)
								potencia_minima_termeletrica.at(periodo).at(idPatamarCarga) += getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_minima, periodo, idPatamarCarga, double());
						}
					} // if ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_minima) == sim_premissa) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_minima) == sim_operacional)) {


					// -----------------------------------
					//
					// AttMatrizUnidadeUTE_potencia_maxima
					//
					// -----------------------------------
					SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> potencia_maxima_unidade;

					if (getSize1Matriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima) == 0) {

						if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_unidade).size() > 0) {
							if (getAtributo(idTermeletrica, AttComumTermeletrica_representacao_discreta_producao, bool()))
								preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_maxima) = sim_operacional;
							else
								preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_maxima) = sim_premissa;
							potencia_maxima_unidade = matriz_zero.at(TipoDetalhamentoProducaoTermeletrica_por_unidade);
						}

						else if ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_maxima) == sim_premissa) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_maxima) == sim_operacional)) {
							preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_maxima) = sim_premissa;
							potencia_maxima_unidade = SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(potencia_maxima_termeletrica, SmartEnupla<IdPatamarCarga, double>());
							for (Periodo periodo = potencia_maxima_unidade.getIteradorInicial(); periodo <= potencia_maxima_unidade.getIteradorFinal(); potencia_maxima_unidade.incrementarIterador(periodo))
								potencia_maxima_unidade.at(periodo) = SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()), 0.0));

						}

						else if ((preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_disponibilidade) == sim_premissa) || (preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_disponibilidade) == sim_operacional)) {
							preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_maxima) = sim_premissa;
							for (Periodo periodo = disponibilidade_termeletrica.getIteradorInicial(); periodo <= disponibilidade_termeletrica.getIteradorFinal(); disponibilidade_termeletrica.incrementarIterador(periodo))
								potencia_maxima_unidade.addElemento(periodo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(1, 0.0)));
						}

						if ((preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_maxima) == sim_premissa) || (preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_maxima) == sim_operacional)) {

							for (Periodo periodo = potencia_maxima_unidade.getIteradorInicial(); periodo <= potencia_maxima_unidade.getIteradorFinal(); potencia_maxima_unidade.incrementarIterador(periodo)) {

								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {

									if (getElementoVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_disponibilidade, periodo, double()) > 0.0)
										vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).addElemento(AttMatrizUnidadeUTE_potencia_maxima, periodo, idPatamarCarga, getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_potencia_maxima, double()));
									else
										vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).addElemento(AttMatrizUnidadeUTE_potencia_maxima, periodo, idPatamarCarga, 0.0);

								} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {

							} // for (Periodo periodo = potencia_maxima_unidade.getIteradorInicial(); periodo <= potencia_maxima_unidade.getIteradorFinal(); potencia_maxima_unidade.incrementarIterador(periodo)) {

						} // if ((preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_maxima) == sim_premissa) || (preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_maxima) == sim_operacional)) {

					} // if (getSize1Matriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima) == 0) {

					else if (getSize1Matriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima) > 0) {

						if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_unidade).size() > 0) {
							if (getAtributo(idTermeletrica, AttComumTermeletrica_representacao_discreta_producao, bool()))
								preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_maxima) = nao_operacional_informado;
							else
								preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_maxima) = nao_premissa_informado;
						}

						else if ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_maxima) == sim_premissa) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_maxima) == sim_operacional))
							preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_maxima) = nao_premissa_informado;

						else if ((preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_disponibilidade) == sim_premissa) || (preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_disponibilidade) == sim_operacional))
							preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_maxima) = nao_premissa_informado;

						else
							throw std::invalid_argument("O atributo " + getFullString(AttMatrizUnidadeUTE_potencia_maxima) + " nao deve ser informado em " + getFullString(idTermeletrica) + " " + getFullString(idUnidadeUTE));

					} // else if (getSize1Matriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima) > 0) {

					if ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_maxima) == sim_premissa) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_maxima) == sim_operacional)) {
						for (Periodo periodo = potencia_maxima_termeletrica.getIteradorInicial(); periodo <= potencia_maxima_termeletrica.getIteradorFinal(); potencia_maxima_termeletrica.incrementarIterador(periodo)) {
							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++)
								potencia_maxima_termeletrica.at(periodo).at(idPatamarCarga) += getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima, periodo, idPatamarCarga, double());
						}
					} // if ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_maxima) == sim_premissa) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_maxima) == sim_operacional)) {



					// --------------------------------------
					//
					// AttVetorUnidadeUTE_custo_de_operacao
					//
					// --------------------------------------

					if (getSize1Matriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_custo_de_operacao) == 0) {

						SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> custo_de_operacao_unidade;

						if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_unidade).size() > 0) {
							preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_custo_de_operacao) = sim_operacional;
							custo_de_operacao_unidade = matriz_zero.at(TipoDetalhamentoProducaoTermeletrica_por_unidade);
						}

						else if ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_custo_de_operacao) == sim_premissa) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_custo_de_operacao) == sim_operacional)) {
							preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_custo_de_operacao) = sim_premissa;
							custo_de_operacao_unidade = matriz_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina);
						}

						if ((preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_custo_de_operacao) == sim_operacional) || (preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_custo_de_operacao) == sim_premissa)) {

							for (Periodo periodo = custo_de_operacao_unidade.getIteradorInicial(); periodo <= custo_de_operacao_unidade.getIteradorFinal(); custo_de_operacao_unidade.incrementarIterador(periodo)) {
								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++)
									vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).addElemento(AttMatrizUnidadeUTE_custo_de_operacao, periodo, idPatamarCarga, getAtributo(idTermeletrica, idUnidadeUTE, AttComumUnidadeUTE_custo_de_operacao, double()));
							}

						} // if ((preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_custo_de_operacao) == sim_operacional) || (preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_custo_de_operacao) == sim_premissa)) {

					} // if (getSize1Matriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_custo_de_operacao) == 0) {

					if (getSize1Matriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_custo_de_operacao) > 0) {

						if (vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_unidade).size() > 0)
							preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_custo_de_operacao) = nao_operacional_informado;

						else if ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_custo_de_operacao) == sim_premissa) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_custo_de_operacao) == sim_operacional))
							preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_custo_de_operacao) = nao_premissa_informado;

						else
							throw std::invalid_argument("O atributo " + getFullString(AttMatrizUnidadeUTE_custo_de_operacao) + " nao deve ser informado em " + getFullString(idTermeletrica) + " " + getFullString(idUnidadeUTE));

					} // if (getSize1Matriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_custo_de_operacao) > 0) {


					if ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_custo_de_operacao) == sim_premissa) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_custo_de_operacao) == sim_operacional)) {
						for (Periodo periodo = custo_de_operacao_termeletrica.getIteradorInicial(); periodo <= custo_de_operacao_termeletrica.getIteradorFinal(); custo_de_operacao_termeletrica.incrementarIterador(periodo)) {
							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++)
								custo_de_operacao_termeletrica.at(periodo).at(idPatamarCarga) += getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_custo_de_operacao, periodo, idPatamarCarga, double());
						}
					} // if ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_custo_de_operacao) == sim_premissa) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_custo_de_operacao) == sim_operacional)){


					////TRAJETORIA DE ACIONAMENTO E DESLIGAMENTO

					if (getSizeVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_rampa_acionamento) > 0)
						preencher_AttVetorUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttVetorUnidadeUTE_rampa_acionamento) = nao_operacional_informado;

					if (getSizeVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_rampa_desligamento) > 0)
						preencher_AttVetorUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttVetorUnidadeUTE_rampa_desligamento) = nao_operacional_informado;




					// ----------------------------------
					//
					// Preencher disponibilidade_termeletrica
					//
					// ----------------------------------

					if ((preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_disponibilidade) == sim_premissa) || (preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_disponibilidade) == sim_operacional)) {
						for (Periodo periodo = disponibilidade_termeletrica.getIteradorInicial(); periodo <= disponibilidade_termeletrica.getIteradorFinal(); disponibilidade_termeletrica.incrementarIterador(periodo)) {

							double potencia_maxima_unidade = 0.0;

							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++)
								potencia_maxima_unidade += getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima, periodo, idPatamarCarga, double()) * getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());

							disponibilidade_termeletrica.at(periodo) += getElementoVetor(idTermeletrica, idUnidadeUTE, AttVetorUnidadeUTE_disponibilidade, periodo, double()) * potencia_maxima_unidade;
						}
					} // if ((preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_disponibilidade) == sim_premissa) || (preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_disponibilidade) == sim_operacional)){


					// -----------------------------------
					//
					// AttMatrizUnidadeUTE_potencia_util
					//
					// -----------------------------------

					if (preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(AttMatrizUnidadeUTE_potencia_util) == sim_operacional)
						if ((getSize1Matriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima) > 0) && (getSize1Matriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_minima) > 0))
							for (Periodo periodo = vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).getIterador1Inicial(AttMatrizUnidadeUTE_potencia_maxima, Periodo()); periodo <= vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).getIterador1Final(AttMatrizUnidadeUTE_potencia_maxima, Periodo()); horizonte_estudo.incrementarIterador(periodo))
								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++)
									vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).addElemento(AttMatrizUnidadeUTE_potencia_util, periodo, idPatamarCarga, (getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima, periodo, idPatamarCarga, double()) - getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_minima, periodo, idPatamarCarga, double())));

				} // for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= maiorIdUnidadeUTE; idUnidadeUTE++) {


				// -------------------------------------------
				//
				// AttMatrizTermeletrica_potencia_minima
				//
				// -------------------------------------------

				if (((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_minima) == sim_premissa) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_minima) == sim_operacional)) && (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_minima) == 0))
					vetorTermeletrica.att(idTermeletrica).setMatriz(AttMatrizTermeletrica_potencia_minima, potencia_minima_termeletrica);

				// -------------------------------------------
				//
				// AttMatrizTermeletrica_potencia_maxima
				//
				// -------------------------------------------

				if (((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_maxima) == sim_premissa) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_maxima) == sim_operacional)) && (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_maxima) == 0))
					vetorTermeletrica.att(idTermeletrica).setMatriz(AttMatrizTermeletrica_potencia_maxima, potencia_maxima_termeletrica);


				// -----------------------------------
				//
				// AttMatrizTermeletrica_potencia_util
				//
				// -----------------------------------

				if (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_util) == sim_operacional)
					if ((getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_maxima) > 0) && (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_minima) > 0))
						for (Periodo periodo = vetorTermeletrica.att(idTermeletrica).getIterador1Inicial(AttMatrizTermeletrica_potencia_maxima, Periodo()); periodo <= vetorTermeletrica.att(idTermeletrica).getIterador1Final(AttMatrizTermeletrica_potencia_maxima, Periodo()); horizonte_estudo.incrementarIterador(periodo))
							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++)
								vetorTermeletrica.att(idTermeletrica).addElemento(AttMatrizTermeletrica_potencia_util, periodo, idPatamarCarga, (getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_maxima, periodo, idPatamarCarga, double()) - getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_minima, periodo, idPatamarCarga, double())));


				// ------------------------------------
				//
				// AttVetorTermeletrica_disponibilidade
				//
				// ------------------------------------

				if ((preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_disponibilidade) == sim_premissa) || (preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_disponibilidade) == sim_operacional)) {

					for (Periodo periodo = disponibilidade_termeletrica.getIteradorInicial(); periodo <= disponibilidade_termeletrica.getIteradorFinal(); disponibilidade_termeletrica.incrementarIterador(periodo)) {

						double potencia_maxima = 0.0;

						for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= getMaiorId(idTermeletrica, IdUnidadeUTE()); idUnidadeUTE++) {
							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++)
								potencia_maxima += getElementoMatriz(idTermeletrica, idUnidadeUTE, AttMatrizUnidadeUTE_potencia_maxima, periodo, idPatamarCarga, double()) * getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
						}

						if (potencia_maxima > 0.0)
							disponibilidade_termeletrica.at(periodo) /= potencia_maxima;

						else
							disponibilidade_termeletrica.at(periodo) = 0.0;

					} // for (Periodo periodo = disponibilidade_termeletrica.getIteradorInicial(); periodo <= disponibilidade_termeletrica.getIteradorFinal(); disponibilidade_termeletrica.incrementarIterador(periodo)) {

					vetorTermeletrica.att(idTermeletrica).setVetor(AttVetorTermeletrica_disponibilidade, disponibilidade_termeletrica);

				} // if ((preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_disponibilidade) == sim_premissa) || (preencher_AttVetorTermeletrica.at(idTermeletrica).at(AttVetorTermeletrica_disponibilidade) == sim_operacional)) {

				if (true) {

					// -------------------------------------------
					//
					// AttMatrizTermeletrica_potencia_disponivel_maxima
					//
					// -------------------------------------------

					if (((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_disponivel_maxima) == sim_premissa) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_disponivel_maxima) == sim_operacional)) && (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_maxima) == 0)) {

						SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> potencia_disponivel_maxima(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>());

						for (Periodo periodo = vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).getIteradorInicial(); periodo <= vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).getIteradorFinal(); vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).incrementarIterador(periodo)) {

							const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

							const double disponibilidade = getElementoVetor(idTermeletrica, AttVetorTermeletrica_disponibilidade, periodo, double());
							const double fator_de_capacidade = getElementoVetor(idTermeletrica, AttVetorTermeletrica_fator_de_capacidade, periodo, double());

							potencia_disponivel_maxima.setElemento(periodo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(maiorIdPatamarCarga, 0.0)));

							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

								const double potencia_minima = getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_minima, periodo, idPatamarCarga, double());
								const double potencia_util = getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_util, periodo, idPatamarCarga, double());

								const double potencia_maxima = potencia_minima + potencia_util;

								potencia_disponivel_maxima.at(periodo).at(idPatamarCarga) = potencia_maxima * disponibilidade * fator_de_capacidade;

							} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						} // for (Periodo periodo = vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).getIteradorInicial(); periodo <= vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).getIteradorFinal(); vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).incrementarIterador(periodo)) {

						vetorTermeletrica.att(idTermeletrica).setMatriz(AttMatrizTermeletrica_potencia_disponivel_maxima, potencia_disponivel_maxima);

					} // if (((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_disponivel_maxima) == sim_premissa) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_disponivel_maxima) == sim_operacional)) && (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_maxima) == 0)) {


					// -------------------------------------------
					//
					// AttMatrizTermeletrica_potencia_disponivel_minima
					//
					// -------------------------------------------

					if (((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_disponivel_minima) == sim_premissa) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_disponivel_minima) == sim_operacional))) {

						SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> potencia_disponivel_minima(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>());

						for (Periodo periodo = vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).getIteradorInicial(); periodo <= vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).getIteradorFinal(); vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).incrementarIterador(periodo)) {

							const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

							const double disponibilidade = getElementoVetor(idTermeletrica, AttVetorTermeletrica_disponibilidade, periodo, double());

							potencia_disponivel_minima.setElemento(periodo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(maiorIdPatamarCarga, 0.0)));

							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

								const double potencia_minima = getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_minima, periodo, idPatamarCarga, double());

								potencia_disponivel_minima.at(periodo).at(idPatamarCarga) = potencia_minima * disponibilidade;

								if (potencia_disponivel_minima.at(periodo).at(idPatamarCarga) > getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_maxima, periodo, idPatamarCarga, double()))
									potencia_disponivel_minima.at(periodo).at(idPatamarCarga) = getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_maxima, periodo, idPatamarCarga, double());

							} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						} // for (Periodo periodo = vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).getIteradorInicial(); periodo <= vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).getIteradorFinal(); vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).incrementarIterador(periodo)) {

						vetorTermeletrica.att(idTermeletrica).setMatriz(AttMatrizTermeletrica_potencia_disponivel_minima, potencia_disponivel_minima);

					} // if (((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_disponivel_minima) == sim_premissa) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_potencia_disponivel_minima) == sim_operacional))) {

					std::vector<IdRestricaoEletrica> vetor_idRestricaoEletrica_comUTE;

					if ((getAtributo(AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_eletricas, bool())) && (a_imprimir_atributos_sem_recarregar)) {
						for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= getMaiorId(IdRestricaoEletrica()); idRestricaoEletrica++) {
							if ((getAtributo(idRestricaoEletrica, IdElementoSistema_1, AttComumElementoSistema_termeletrica, IdTermeletrica()) == idTermeletrica) && (getAtributo(idRestricaoEletrica, IdElementoSistema_1, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa()) == TipoVariavelRestricaoOperativa_Nenhum) && (isRestricaoEletrica_simples(idRestricaoEletrica)))
								vetor_idRestricaoEletrica_comUTE.push_back(idRestricaoEletrica);
						}
					} // if (getAtributo(AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_eletricas, bool())) {

					if (vetor_idRestricaoEletrica_comUTE.size() > 0) {

						for (Periodo periodo = vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).getIteradorInicial(); periodo <= vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).getIteradorFinal(); vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).incrementarIterador(periodo)) {

							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {

								for (int pos = 0; pos < int(vetor_idRestricaoEletrica_comUTE.size()); pos++) {

									if (getElementoMatriz(vetor_idRestricaoEletrica_comUTE.at(pos), AttMatrizRestricaoEletrica_potencia_minima, periodo, idPatamarCarga, double()) > getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_minima, periodo, idPatamarCarga, double()))
										vetorTermeletrica.att(idTermeletrica).setElemento(AttMatrizTermeletrica_potencia_disponivel_minima, periodo, idPatamarCarga, getElementoMatriz(vetor_idRestricaoEletrica_comUTE.at(pos), AttMatrizRestricaoEletrica_potencia_minima, periodo, idPatamarCarga, double()));

									if (getElementoMatriz(vetor_idRestricaoEletrica_comUTE.at(pos), AttMatrizRestricaoEletrica_potencia_maxima, periodo, idPatamarCarga, double()) < getElementoMatriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_maxima, periodo, idPatamarCarga, double()))
										vetorTermeletrica.att(idTermeletrica).setElemento(AttMatrizTermeletrica_potencia_disponivel_maxima, periodo, idPatamarCarga, getElementoMatriz(vetor_idRestricaoEletrica_comUTE.at(pos), AttMatrizRestricaoEletrica_potencia_maxima, periodo, idPatamarCarga, double()));

								}//for (int pos = 0; pos < int(vetor_idRestricaoEletrica_comUTE.size()); pos++) {

							} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						} // for (Periodo periodo = vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).getIteradorInicial(); periodo <= vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).getIteradorFinal(); vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_usina).incrementarIterador(periodo)) {
					} // if (vetor_idRestricaoOperativaUHE_comUHE_vazao_defluente.size() > 0) {

				} // if (true){

				
				// ----------------------------------------
				//
				// AttVetorTermeletrica_custo_de_operacao
				//
				// ---------------------------------------
				/*
				if ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_custo_de_operacao) == sim_premissa) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(AttMatrizTermeletrica_custo_de_operacao) == sim_operacional)) {

					//
					// Anula custo de opera��o em caso de potencia_disponivel_pre_comandada
					//

					if (getAtributo(idTermeletrica, AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada, int()) > 0) {

						const SmartEnupla<Periodo, double> potencia_disponivel_pre_comandada = getVetor(idTermeletrica, AttVetorTermeletrica_potencia_disponivel_pre_comandada, Periodo(), double());

						if (potencia_disponivel_pre_comandada.size() > 0) {

							for (Periodo periodo_estudo = horizonte_estudo.getIteradorInicial(); periodo_estudo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo_estudo)) {

								double percentual_valor = 1.0;

								for (Periodo periodo_pre_comando = potencia_disponivel_pre_comandada.getIteradorInicial(); periodo_pre_comando <= potencia_disponivel_pre_comandada.getIteradorFinal(); potencia_disponivel_pre_comandada.incrementarIterador(periodo_pre_comando)) {

									const double sobreposicao = periodo_estudo.sobreposicao(periodo_pre_comando);
									if (sobreposicao > 0.0)
										percentual_valor -= sobreposicao;

								} // for (Periodo periodo_pre_comando = potencia_disponivel_pre_comandada.getIteradorInicial(); periodo_pre_comando <= potencia_disponivel_pre_comandada.getIteradorFinal(); potencia_disponivel_pre_comandada.incrementarIterador(periodo_pre_comando)) {

								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(idTermeletrica, AttMatrizTermeletrica_custo_de_operacao, periodo_estudo, IdPatamarCarga()); idPatamarCarga++) {

									const double custo_de_operacao = custo_de_operacao_termeletrica.at(periodo_estudo).at(idPatamarCarga) * percentual_valor;

									custo_de_operacao_termeletrica.at(periodo_estudo).at(idPatamarCarga) = custo_de_operacao;

								} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= a_dados.getIterador2Final(idTermeletrica, AttMatrizTermeletrica_custo_de_operacao, periodo_estudo, IdPatamarCarga()); idPatamarCarga++) {

							} // for (Periodo periodo_estudo = horizonte_estudo.getIteradorInicial(); periodo_estudo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo_estudo)) {

						} // if (potencia_disponivel_pre_comandada.size() > 0) {

					} // if (a_dados.getAtributo(idTermeletrica, AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada, int()) > 0) {

					vetorTermeletrica.att(idTermeletrica).setMatriz(AttMatrizTermeletrica_custo_de_operacao, custo_de_operacao_termeletrica);
				}

				validarTermeletricaComandada(idTermeletrica, preencher_AttVetorTermeletrica.at(idTermeletrica), preencher_AttMatrizTermeletrica.at(idTermeletrica), a_imprimir_atributos_sem_recarregar);
				*/
				// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				//
				//                                                                                            Impress�o de Atributos
				//
				// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

				if (idProcesso == IdProcesso_mestre) {

					// --------------------
					//
					// AttComumTermeletrica
					//
					// --------------------

					if (a_imprimir_atributos_sem_recarregar) {

						a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

						a_entradaSaidaDados.setAppendArquivo(impresso_AttComumTermeletrica);
						const std::vector<AttComumTermeletrica> lista_TERMELETRICA_AttComumOperacional{ AttComumTermeletrica_idTermeletrica,    AttComumTermeletrica_nome, AttComumTermeletrica_submercado, AttComumTermeletrica_codigo_usina, AttComumTermeletrica_lag_mensal_potencia_disponivel_comandada, AttComumTermeletrica_tipo_detalhamento_producao, AttComumTermeletrica_representacao_discreta_producao, AttComumTermeletrica_penalidade_violacao_potencia, AttComumTermeletrica_penalidade_violacao_comando };
						a_entradaSaidaDados.imprimirArquivoCSV_AttComum("TERMELETRICA_AttComumOperacional.csv", idTermeletrica, *this, lista_TERMELETRICA_AttComumOperacional);
						impresso_AttComumTermeletrica = true;

					} // if (a_imprimir_atributos_sem_recarregar) {

					// --------------------
					//
					// AttVetorTermeletrica
					//
					// --------------------

					for (AttVetorTermeletrica attVetorTermeletrica = AttVetorTermeletrica(AttVetorTermeletrica_Nenhum + 1); attVetorTermeletrica < AttVetorTermeletrica_Excedente; attVetorTermeletrica++) {

						if ((preencher_AttVetorTermeletrica.at(idTermeletrica).at(attVetorTermeletrica) == sim_premissa) || ((preencher_AttVetorTermeletrica.at(idTermeletrica).at(attVetorTermeletrica) == nao_premissa_informado) && (a_imprimir_atributos_sem_recarregar))) {

							a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

							a_entradaSaidaDados.setAppendArquivo(impresso_AttVetorTermeletrica_PorPeriodo.at(0));
							a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("TERMELETRICA_AttVetorPremissa_PorPeriodo.csv", idTermeletrica, *this, periodo_estudo_inicial, periodo_final_estudo, attVetorTermeletrica);
							impresso_AttVetorTermeletrica_PorPeriodo.at(0) = true;

							vetorTermeletrica.att(idTermeletrica).clear(attVetorTermeletrica);

						} // if ((preencher_AttVetorTermeletrica.at(idTermeletrica).at(attVetorTermeletrica) == sim_premissa) || ((preencher_AttVetorTermeletrica.at(idTermeletrica).at(attVetorTermeletrica) == nao_premissa_informado) && (a_imprimir_atributos_sem_recarregar))) {

						else if ((preencher_AttVetorTermeletrica.at(idTermeletrica).at(attVetorTermeletrica) == sim_operacional) || ((preencher_AttVetorTermeletrica.at(idTermeletrica).at(attVetorTermeletrica) == nao_operacional_informado) && (a_imprimir_atributos_sem_recarregar))) {

							a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

							a_entradaSaidaDados.setAppendArquivo(impresso_AttVetorTermeletrica_PorPeriodo.at(1));
							a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("TERMELETRICA_AttVetorOperacional_PorPeriodo.csv", idTermeletrica, *this, periodo_estudo_inicial, periodo_final_estudo, attVetorTermeletrica);
							impresso_AttVetorTermeletrica_PorPeriodo.at(1) = true;

							if (!a_imprimir_atributos_sem_recarregar) {
								vetorTermeletrica.att(idTermeletrica).clear(attVetorTermeletrica);
								recarregar_AttVetorTermeletrica_PorPeriodo = true;
							}

						} // else if ((preencher_AttVetorTermeletrica.at(idTermeletrica).at(attVetorTermeletrica) == sim_operacional) || ((preencher_AttVetorTermeletrica.at(idTermeletrica).at(attVetorTermeletrica) == nao_operacional_informado) && (a_imprimir_atributos_sem_recarregar))) {

					} // for (AttVetorTermeletrica attVetorTermeletrica = AttVetorTermeletrica(AttVetorTermeletrica_Nenhum + 1); attVetorTermeletrica < AttVetorTermeletrica_Excedente; attVetorTermeletrica++) {

					// --------------------
					//
					// AttMatrizTermeletrica
					//
					// --------------------

					for (AttMatrizTermeletrica attMatrizTermeletrica = AttMatrizTermeletrica(AttMatrizTermeletrica_Nenhum + 1); attMatrizTermeletrica < AttMatrizTermeletrica_Excedente; attMatrizTermeletrica++) {

						if ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(attMatrizTermeletrica) == sim_premissa) || ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(attMatrizTermeletrica) == nao_premissa_informado) && (a_imprimir_atributos_sem_recarregar))) {

							a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

							a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizTermeletrica_PorPeriodoPorIdPatamarCarga.at(0));
							a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("TERMELETRICA_AttMatrizPremissa_PorPeriodoPorIdPatamarCarga.csv", idTermeletrica, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, attMatrizTermeletrica);
							impresso_AttMatrizTermeletrica_PorPeriodoPorIdPatamarCarga.at(0) = true;

							vetorTermeletrica.att(idTermeletrica).clear(attMatrizTermeletrica);

						} // if ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(attMatrizTermeletrica) == sim_premissa) || ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(attMatrizTermeletrica) == nao_premissa_informado) && (a_imprimir_atributos_sem_recarregar))) {

						else if ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(attMatrizTermeletrica) == sim_operacional) || ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(attMatrizTermeletrica) == nao_operacional_informado) && (a_imprimir_atributos_sem_recarregar))) {

							if (attMatrizTermeletrica == AttMatrizTermeletrica_potencia_disponivel_comandada) {

								a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

								a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizTermeletrica_COMANDO_PorPeriodoPorIdPatamarCarga.at(1));

								const Periodo periodo_comandada_inicial = getIterador1Inicial(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_comandada, Periodo());
								const Periodo periodo_comandada_final   = getIterador1Final(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_comandada, Periodo());

								a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("TERMELETRICA_COMANDO_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", idTermeletrica, *this, periodo_comandada_inicial, periodo_comandada_final, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, attMatrizTermeletrica);
								impresso_AttMatrizTermeletrica_COMANDO_PorPeriodoPorIdPatamarCarga.at(1) = true;

								if (!a_imprimir_atributos_sem_recarregar) {
									vetorTermeletrica.att(idTermeletrica).clear(attMatrizTermeletrica);
									recarregar_AttMatrizTermeletrica_COMANDO_PorPeriodoPorIdPatamarCarga = true;
								}
							}

							else {

								a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

								a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizTermeletrica_PorPeriodoPorIdPatamarCarga.at(1));
								a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("TERMELETRICA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", idTermeletrica, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, attMatrizTermeletrica);
								impresso_AttMatrizTermeletrica_PorPeriodoPorIdPatamarCarga.at(1) = true;

								if (!a_imprimir_atributos_sem_recarregar) {
									vetorTermeletrica.att(idTermeletrica).clear(attMatrizTermeletrica);
									recarregar_AttMatrizTermeletrica_PorPeriodoPorIdPatamarCarga = true;
								}

							}

						} // else if ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(attMatrizTermeletrica) == sim_operacional) || ((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(attMatrizTermeletrica) == nao_operacional_informado) && (a_imprimir_atributos_sem_recarregar))) {

						// Imprime Vetores Médios como Info
						if (((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(attMatrizTermeletrica) == sim_operacional) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(attMatrizTermeletrica) == nao_operacional_informado))) {

							const AttVetorTermeletrica attVetorTermeletrica_media = getAttVetorTermeletricaFromChar(std::string(getString(attMatrizTermeletrica) + "_media").c_str());

							if (attVetorTermeletrica_media != AttVetorTermeletrica_Nenhum) {

								SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz = getMatriz(idTermeletrica, attMatrizTermeletrica, Periodo(), IdPatamarCarga(), double());

								SmartEnupla<Periodo, double> vetor(matriz, 0.0);

								for (Periodo periodo = matriz.getIteradorInicial(); periodo <= periodo_final_estudo; matriz.incrementarIterador(periodo)) {
									for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= matriz.at(periodo).getIteradorFinal(); idPatamarCarga++)
										vetor.at(periodo) += matriz.at(periodo).at(idPatamarCarga) * getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
								}

								vetorTermeletrica.att(idTermeletrica).setVetor_forced(attVetorTermeletrica_media, vetor);

								a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

								a_entradaSaidaDados.setAppendArquivo(impresso_AttVetorTermeletrica_PorPeriodo.at(2));
								a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_TERMELETRICA_AttVetorPremissa_PorPeriodo.csv", idTermeletrica, *this, periodo_estudo_inicial, periodo_final_estudo, attVetorTermeletrica_media);
								impresso_AttVetorTermeletrica_PorPeriodo.at(2) = true;

								vetorTermeletrica.att(idTermeletrica).clear(attVetorTermeletrica_media);

							}

						} // if (((preencher_AttMatrizTermeletrica.at(idTermeletrica).at(attMatrizTermeletrica) == sim_operacional) || (preencher_AttMatrizTermeletrica.at(idTermeletrica).at(attMatrizTermeletrica) == nao_operacional_informado))) {

					} // for (AttMatrizTermeletrica attMatrizTermeletrica = AttMatrizTermeletrica(AttMatrizTermeletrica_Nenhum + 1); attMatrizTermeletrica < AttMatrizTermeletrica_Excedente; attMatrizTermeletrica++) {


					if ((getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_meta) > 0) && (a_imprimir_atributos_sem_recarregar)) {

						a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

						a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizTermeletrica_PorIdCenarioPorPeriodo.at(1));
						a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("TERMELETRICA_AttMatrizOperacional_PorCenarioPorPeriodo.csv", idTermeletrica, *this, AttMatrizTermeletrica_potencia_disponivel_meta);
						impresso_AttMatrizTermeletrica_PorIdCenarioPorPeriodo.at(1) = true;

					}//if (getSize1Matriz(idTermeletrica, AttMatrizTermeletrica_potencia_disponivel_meta) > 0) {

					const IdUnidadeUTE maiorIdUnidadeUTE = getMaiorId(idTermeletrica, IdUnidadeUTE());
					for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= maiorIdUnidadeUTE; idUnidadeUTE++) {

						// ------------------
						//
						// AttComumUnidadeUTE
						//
						// ------------------

						if ((vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_unidade).size() > 0) && (a_imprimir_atributos_sem_recarregar)) {

							a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

							a_entradaSaidaDados.setAppendArquivo(impresso_AttComumUnidadeUTE);
							const std::vector<AttComumUnidadeUTE> lista_TERMELETRICA_CONJUNTO_UNIDADE_AttComumOperacional{ AttComumUnidadeUTE_idUnidadeUTE, AttComumUnidadeUTE_nome, AttComumUnidadeUTE_submercado, AttComumUnidadeUTE_status, AttComumUnidadeUTE_t_on, AttComumUnidadeUTE_t_of,\
																															AttComumUnidadeUTE_unidade_equivalente, AttComumUnidadeUTE_geracao_inicial, AttComumUnidadeUTE_horas_neste_status, AttComumUnidadeUTE_min_neste_status };

							a_entradaSaidaDados.imprimirArquivoCSV_AttComum("TERMELETRICA_UNIDADE_AttComumOperacional.csv", idTermeletrica, idUnidadeUTE, *this, lista_TERMELETRICA_CONJUNTO_UNIDADE_AttComumOperacional);
							impresso_AttComumUnidadeUTE = true;

						} // if ((vetor_zero.at(TipoDetalhamentoProducaoTermeletrica_por_unidade).size() > 0) && (a_imprimir_atributos_sem_recarregar)) {

						// --------------------------
						//
						// AttVetorUnidadeUTE
						//
						// --------------------------

						for (AttVetorUnidadeUTE attVetorUnidadeUTE = AttVetorUnidadeUTE(AttVetorUnidadeUTE_Nenhum + 1); attVetorUnidadeUTE < AttVetorUnidadeUTE_Excedente; attVetorUnidadeUTE++) {

							if ((preencher_AttVetorUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(attVetorUnidadeUTE) == sim_premissa) || ((preencher_AttVetorUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(attVetorUnidadeUTE) == nao_premissa_informado) && (a_imprimir_atributos_sem_recarregar))) {

								a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

								if ((attVetorUnidadeUTE == AttVetorUnidadeUTE_rampa_acionamento) || (attVetorUnidadeUTE == AttVetorUnidadeUTE_rampa_desligamento)) {

									a_entradaSaidaDados.setAppendArquivo(impresso_AttVetorUnidadeUTE_PorInteiro.at(0));
									a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("TERMELETRICA_UNIDADE_AttVetorPremissa_PorInteiro.csv", idTermeletrica, idUnidadeUTE, *this, 0, 100, attVetorUnidadeUTE);
									impresso_AttVetorUnidadeUTE_PorInteiro.at(0) = true;

								} // if ((attVetorUnidadeUTE == AttVetorUnidadeUTE_rampa_acionamento) || (attVetorUnidadeUTE == AttVetorUnidadeUTE_rampa_desligamento)) {

								else {

									a_entradaSaidaDados.setAppendArquivo(impresso_AttVetorUnidadeUTE_PorPeriodo.at(0));
									a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("TERMELETRICA_UNIDADE_AttVetorPremissa_PorPeriodo.csv", idTermeletrica, idUnidadeUTE, *this, periodo_estudo_inicial, periodo_final_estudo, attVetorUnidadeUTE);
									impresso_AttVetorUnidadeUTE_PorPeriodo.at(0) = true;

								}

								vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).clear(attVetorUnidadeUTE);

							}// if ((preencher_AttVetorUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(attVetorUnidadeUTE) == sim_premissa) || ((preencher_AttVetorUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(attVetorUnidadeUTE) == nao_premissa_informado) && (a_imprimir_atributos_sem_recarregar))) {

							else if ((preencher_AttVetorUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(attVetorUnidadeUTE) == sim_operacional) || ((preencher_AttVetorUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(attVetorUnidadeUTE) == nao_operacional_informado) && (a_imprimir_atributos_sem_recarregar))) {

								a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

								if ((attVetorUnidadeUTE == AttVetorUnidadeUTE_rampa_acionamento) || (attVetorUnidadeUTE == AttVetorUnidadeUTE_rampa_desligamento)) {

									a_entradaSaidaDados.setAppendArquivo(impresso_AttVetorUnidadeUTE_PorInteiro.at(0));
									a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("TERMELETRICA_UNIDADE_AttVetorOperacional_PorInteiro.csv", idTermeletrica, idUnidadeUTE, *this, 0, 100, attVetorUnidadeUTE);
									impresso_AttVetorUnidadeUTE_PorInteiro.at(0) = true;

								} // if ((attVetorUnidadeUTE == AttVetorUnidadeUTE_rampa_acionamento) || (attVetorUnidadeUTE == AttVetorUnidadeUTE_rampa_desligamento)) {

								else {
									a_entradaSaidaDados.setAppendArquivo(impresso_AttVetorUnidadeUTE_PorPeriodo.at(1));
									a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("TERMELETRICA_UNIDADE_AttVetorOperacional_PorPeriodo.csv", idTermeletrica, idUnidadeUTE, *this, periodo_estudo_inicial, periodo_final_estudo, attVetorUnidadeUTE);
									impresso_AttVetorUnidadeUTE_PorPeriodo.at(1) = true;

									if (!a_imprimir_atributos_sem_recarregar) {
										vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).clear(attVetorUnidadeUTE);
										recarregar_AttVetorUnidadeUTE_PorPeriodo = true;
									}
								}
							} //  else if ((preencher_AttVetorUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(attVetorUnidadeUTE) == sim_operacional) || ((preencher_AttVetorUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(attVetorUnidadeUTE) == nao_operacional_informado) && (a_imprimir_atributos_sem_recarregar))) {

						} // for (AttVetorUnidadeUTE attVetorUnidadeUTE = AttVetorUnidadeUTE(AttVetorUnidadeUTE_Nenhum + 1); attVetorUnidadeUTE < AttVetorUnidadeUTE_Excedente; attVetorUnidadeUTE++) {


						// --------------------------
						//
						// AttMatrizUnidadeUTE
						//
						// --------------------------

						for (AttMatrizUnidadeUTE attMatrizUnidadeUTE = AttMatrizUnidadeUTE(AttMatrizUnidadeUTE_Nenhum + 1); attMatrizUnidadeUTE < AttMatrizUnidadeUTE_Excedente; attMatrizUnidadeUTE++) {

							if ((preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(attMatrizUnidadeUTE) == sim_premissa) || ((preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(attMatrizUnidadeUTE) == nao_premissa_informado) && (a_imprimir_atributos_sem_recarregar))) {

								a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

								a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizUnidadeUTE_PorPeriodoPorIdPatamarCarga.at(0));
								a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("TERMELETRICA_UNIDADE_AttMatrizPremissa_PorPeriodoPorIdPatamarCarga.csv", idTermeletrica, idUnidadeUTE, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, attMatrizUnidadeUTE);
								impresso_AttMatrizUnidadeUTE_PorPeriodoPorIdPatamarCarga.at(0) = true;

								vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).clear(attMatrizUnidadeUTE);

							} // if ((preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(attMatrizUnidadeUTE) == sim_premissa) || ((preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(attMatrizUnidadeUTE) == nao_premissa_informado) && (a_imprimir_atributos_sem_recarregar))) {

							else if ((preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(attMatrizUnidadeUTE) == sim_operacional) || ((preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(attMatrizUnidadeUTE) == nao_operacional_informado) && (a_imprimir_atributos_sem_recarregar))) {

								a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

								a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizUnidadeUTE_PorPeriodoPorIdPatamarCarga.at(1));
								a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("TERMELETRICA_UNIDADE_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", idTermeletrica, idUnidadeUTE, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, attMatrizUnidadeUTE);
								impresso_AttMatrizUnidadeUTE_PorPeriodoPorIdPatamarCarga.at(1) = true;

								if (!a_imprimir_atributos_sem_recarregar) {
									vetorTermeletrica.att(idTermeletrica).vetorUnidadeUTE.att(idUnidadeUTE).clear(attMatrizUnidadeUTE);
									recarregar_AttMatrizUnidadeUTE_PorPeriodoPorIdPatamarCarga = true;
								}

							} // else if ((preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(attMatrizUnidadeUTE) == sim_operacional) || ((preencher_AttMatrizUnidadeUTE.at(idTermeletrica).at(idUnidadeUTE).at(attMatrizUnidadeUTE) == nao_operacional_informado) && (a_imprimir_atributos_sem_recarregar))) {

						} // for (AttMatrizUnidadeUTE attMatrizUnidadeUTE = AttMatrizUnidadeUTE(AttMatrizUnidadeUTE_Nenhum + 1); attMatrizUnidadeUTE < AttMatrizUnidadeUTE_Excedente; attMatrizUnidadeUTE++) {

					} //for (IdUnidadeUTE idUnidadeUTE = IdUnidadeUTE_1; idUnidadeUTE <= maiorIdUnidadeUTE; idUnidadeUTE++) {

				} //if (idProcesso == IdProcesso_mestre) {


			} //if (getAtributo(idTermeletrica, AttComumTermeletrica_considerar_usina, bool())) {


		} // for (IdTermeletrica idTermeletrica = getMenorId(IdTermeletrica()); idTermeletrica <= maiorIdTermeletrica; vetorTermeletrica.incr(idTermeletrica)) {


		// VALIDA AS UNIDADES TERMEL�TRICAS
		validaUnidadeUTE();

		// VALIDA AS TERMEL�TRICAS 
		validaTermeletrica();


		if (!a_imprimir_atributos_sem_recarregar) {

			if (true) {
				int barreira = 0;

				if (arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso()) == IdProcesso_mestre) {
					for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++)
						MPI_Send(&barreira, 1, MPI_INT, getRank(idProcesso), 0, MPI_COMM_WORLD);
				}
				else
					MPI_Recv(&barreira, 1, MPI_INT, getRank(IdProcesso_mestre), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}


			//
			// Recarrega Atributos Operacionais
			//

			a_entradaSaidaDados.setDiretorioEntrada(a_diretorio_att_operacional);

			if (recarregar_AttVetorTermeletrica_PorPeriodo)
				a_entradaSaidaDados.carregarArquivoCSV_AttVetor("TERMELETRICA_AttVetorOperacional_PorPeriodo.csv", *this, TipoAcessoInstancia_membro);

			if (recarregar_AttVetorTermeletrica_PorInteiro)
				a_entradaSaidaDados.carregarArquivoCSV_AttVetor("TERMELETRICA_AttVetorOperacional_PorInteiro.csv", *this, TipoAcessoInstancia_membro);

			if (recarregar_AttMatrizTermeletrica_PorPeriodoPorIdPatamarCarga)
				a_entradaSaidaDados.carregarArquivoCSV_AttMatriz("TERMELETRICA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);

			if (recarregar_AttMatrizTermeletrica_COMANDO_PorPeriodoPorIdPatamarCarga)
				a_entradaSaidaDados.carregarArquivoCSV_AttMatriz("TERMELETRICA_COMANDO_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);

			if (recarregar_AttVetorUnidadeUTE_PorPeriodo)
				a_entradaSaidaDados.carregarArquivoCSV_AttVetor("TERMELETRICA_UNIDADE_AttVetorOperacional_PorPeriodo.csv", *this, TipoAcessoInstancia_membroMembroMembro);

			if (recarregar_AttMatrizUnidadeUTE_PorPeriodoPorIdPatamarCarga)
				a_entradaSaidaDados.carregarArquivoCSV_AttMatriz("TERMELETRICA_UNIDADE_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membroMembroMembro);

		} // (!a_imprimir_atributos_sem_recarregar) {


	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validacao_operacional_Termeletrica(a_entradaSaidaDados, " + a_diretorio_att_operacional + "," + a_diretorio_att_premissa + "," + getString(a_imprimir_atributos_sem_recarregar) + "): \n" + std::string(erro.what())); }

} // void Dados::validacao_operacional_Termeletrica(){


void Dados::validaHidreletrica() {
	try {

		// Valida se AttComuns "submercado e bacia" correspondem ao cadastrado em Dados.
		// Adiciona elementos em AttVetor montante.


		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_estudo_inicial = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();

		const IdHidreletrica menorIdHidreletrica = getMenorId(IdHidreletrica());
		const IdHidreletrica maiorIdHidreletrica = getMaiorId(IdHidreletrica());

		for (IdHidreletrica idHidreletrica = menorIdHidreletrica; idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

			const TipoDetalhamentoProducaoHidreletrica tipo_detalhamento_producao = getAtributo(idHidreletrica, AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica());

			const bool representacao_discreta_producao = getAtributo(idHidreletrica, AttComumHidreletrica_representacao_discreta_producao, bool());


			for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

				// VALIDA��O DOS ATRIBUTOS VETOR DA HIDRELETRICA	
				if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_minima) != 0 && getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_maxima) != 0) {
					if (getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_minima, periodo, double()) > getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_maxima, periodo, double()))
						throw std::invalid_argument("Turbinamento minimo da  " + getFullString(idHidreletrica) + " no " + getFullString(periodo) + "  maior que o turbinamento maximo");
				}//if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_minima) != 0 && getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_maxima) != 0) {

				const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

				for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

					// VALIDA��O DOS ATRIBUTOS MATRIZ DA HIDRELETRICA
					if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_minima) != 0 && getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_maxima) != 0) {
						if (getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_minima, periodo, idPatamarCarga, double()) > getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_maxima, periodo, idPatamarCarga, double()))
							throw std::invalid_argument("Potencia minima da  " + getFullString(idHidreletrica) + " no " + getFullString(periodo) + " no " + getFullString(idPatamarCarga) + "  maior que a potencia maxima");
					}// if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_minima) != 0 && getSize1Matriz(idHidreletrica, AttMatrizConjuntoHidraulico_potencia_disponivel_maxima) != 0) {

					if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_minima) != 0 && getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima) != 0) {
						if (getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_minima, periodo, idPatamarCarga, double()) > getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima, periodo, idPatamarCarga, double()))
							throw std::invalid_argument("Turbinamento minimo disponivel da  " + getFullString(idHidreletrica) + " no " + getFullString(periodo) + " no " + getFullString(idPatamarCarga) + "  maior que o turbinamento maximo disponivel");
					}//if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_disponivel_minima) != 0 && getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_disponivel_maxima) != 0) {

					if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_defluente_minima) != 0 && getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_defluente_maxima) != 0) {
						if (getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_defluente_minima, periodo, idPatamarCarga, double()) > getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_defluente_maxima, periodo, idPatamarCarga, double()))
							throw std::invalid_argument("Defluencia minima da  " + getFullString(idHidreletrica) + " no " + getFullString(periodo) + " no " + getFullString(idPatamarCarga) + "  maior que o defluencia maxima");
					}//if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_disponivel_minima) != 0 && getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_disponivel_maxima) != 0) {

					if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_desviada_minima) != 0 && getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_desviada_maxima) != 0) {
						if (getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_desviada_minima, periodo, idPatamarCarga, double()) > getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_desviada_maxima, periodo, idPatamarCarga, double()))
							throw std::invalid_argument("Vazao_desviada minima da  " + getFullString(idHidreletrica) + " no " + getFullString(periodo) + " no " + getFullString(idPatamarCarga) + "  maior que o vazao_desviada maximo");
					}//if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_minima) != 0 && getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_desviada_maxima) != 0) {


					if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_conjunto) {

						IdConjuntoHidraulico maiorIdConjuntoHidraulico = getMaiorId(idHidreletrica, IdConjuntoHidraulico());

						double potencia_minima_conjuntos = 0.0;
						for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {

							if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_minima) != 0)
								potencia_minima_conjuntos += getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_minima, periodo, idPatamarCarga, double());

							// VALIDA��O DOS ATRIBUTOS MATRIZ DA CONJUNTO HIDRAULICO
							if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_minima) != 0 && getSize1Matriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_maxima) != 0) {
								if (getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_minima, periodo, idPatamarCarga, double()) > getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_maxima, periodo, idPatamarCarga, double()))
									throw std::invalid_argument("Potencia minima da  " + getFullString(idHidreletrica) + " do  " + getFullString(idConjuntoHidraulico) + " no " + getFullString(periodo) + " no " + getFullString(idPatamarCarga) + "  maior que a potencia maxima");
							}

						}// for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {

						if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_maxima) != 0 && !representacao_discreta_producao)
							if (potencia_minima_conjuntos > getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_maxima, periodo, idPatamarCarga, double()))
								throw std::invalid_argument("Potencia minima dos conjuntos hidraulicos da  " + getFullString(idHidreletrica) + " no " + getFullString(periodo) + " no " + getFullString(idPatamarCarga) + "  maior que a potencia maxima da Hidreletrica");

					} // if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto).size() > 0) {


					if (tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_por_unidade) {

						IdConjuntoHidraulico maiorIdConjuntoHidraulico = getMaiorId(idHidreletrica, IdConjuntoHidraulico());

						double potencia_minima_conjuntos = 0.0;
						double potencia_minima_unidades = 0.0;
						for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {
							IdUnidadeUHE maiorIdUnidadeUHE = getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE());

							for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= maiorIdUnidadeUHE; idUnidadeUHE++) {

								if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_maxima) != 0)
									potencia_minima_unidades += getElementoMatriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_maxima, periodo, idPatamarCarga, double());

								// VALIDA��O DOS ATRIBUTOS MATRIZ DA UNIDADE
								if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_minima) != 0 && getSize1Matriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_maxima) != 0) {
									if (getElementoMatriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_minima, periodo, idPatamarCarga, double()) > getElementoMatriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_maxima, periodo, idPatamarCarga, double()))
										throw std::invalid_argument("Potencia minima da  " + getFullString(idHidreletrica) + " do  " + getFullString(idConjuntoHidraulico) + " da  " + getFullString(idUnidadeUHE) + " no " + getFullString(periodo) + " no " + getFullString(idPatamarCarga) + "  maior que a potencia maxima");
								}

							} // for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= maiorIdUnidadeUHE; idUnidadeUHE++) {

							if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_minima) != 0 && !representacao_discreta_producao)
								potencia_minima_conjuntos += getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_minima, periodo, idPatamarCarga, double());

							if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_maxima) != 0 && !representacao_discreta_producao)
								if (potencia_minima_unidades > getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_maxima, periodo, idPatamarCarga, double()))
									throw std::invalid_argument("Potencia minima das Unidades geradoras do  " + getFullString(idConjuntoHidraulico) + "da" + getFullString(idHidreletrica) + " no " + getFullString(periodo) + " no " + getFullString(idPatamarCarga) + "  maior que a potencia maxima do conjunto hidraulico");

						}// for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {

						if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_maxima) != 0 && !representacao_discreta_producao)
							if (potencia_minima_conjuntos > getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_maxima, periodo, idPatamarCarga, double()))
								throw std::invalid_argument("Potencia minima dos conjuntos hidraulicos da  " + getFullString(idHidreletrica) + " no " + getFullString(periodo) + " no " + getFullString(idPatamarCarga) + "  maior que a potencia maxima da Hidreletrica");

					} // if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto).size() > 0) {

				}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {


			} // for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

		} // for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaHidreletrica(): \n" + std::string(erro.what())); }
} // void Dados::validaHidreletrica(){


void Dados::validaDefluencia() {
	try {

		const Periodo periodo_estudo_inicial = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio()).getIteradorInicial();

		const IdHidreletrica maiorIdHidreletrica = getMaiorId(IdHidreletrica());

		for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

			if (vetorHidreletrica.att(idHidreletrica).vetorDefluencia.isInstanciado(IdDefluencia_passada)) {

				Periodo periodo_teste(periodo_estudo_inicial.getTipoPeriodo(), getVetor(idHidreletrica, IdDefluencia_passada, AttVetorDefluencia_vazao_defluencia, Periodo(), double()).getIteradorFinal() + 1);

				if (periodo_teste != periodo_estudo_inicial)
					throw std::invalid_argument("Ultimo periodo da defluencia deve ser sequencial com o primeiro periodo do horizonte de estudo");

			}//if (vetorHidreletrica.att(idHidreletrica).vetorDefluencia.isInstanciado(IdDefluencia_passada)) {
			else if ((getAtributo(AttComumDados_estagio_inicial, IdEstagio()) == IdEstagio_1) && (getAtributo(idHidreletrica, AttComumHidreletrica_tempo_viagem_agua, int()) > 0)) {

				//Inicializa as defluencias passadas com valor 0

				//Horizonte de estudo
				const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

				//Nota: A defluencia passada vai considerar periodos_diarios de 15 dias anteriores ao horizonte de estudo (15 dias m�ximo tempo de viagem d'�gua registrado)
				const SmartEnupla<Periodo, bool> horizonte_defluencia_passada(Periodo(TipoPeriodo_diario, horizonte_estudo.getIteradorInicial()) - 15, std::vector<bool>(15, true));

				Defluencia defluencia;
				defluencia.setAtributo(AttComumDefluencia_idDefluencia, IdDefluencia_passada);
				defluencia.setAtributo(AttComumDefluencia_tipo_elemento_jusante, TipoElementoJusante_usina);

				vetorHidreletrica.att(idHidreletrica).vetorDefluencia.add(defluencia);
				vetorHidreletrica.att(idHidreletrica).vetorDefluencia.att(IdDefluencia_passada).setVetor(AttVetorDefluencia_vazao_defluencia, SmartEnupla<Periodo, double>(horizonte_defluencia_passada, 0.0));

			}//else if(getAtributo(idHidreletrica, AttComumHidreletrica_tempo_viagem_agua, int()) > 0) {

		} // for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaDefluencia(): \n" + std::string(erro.what())); }
} // void Dados::validaHidreletrica(){

void Dados::valida_considerar_tempo_viagem_agua(const IdHidreletrica a_idHidreletrica) {
	try {

		const Periodo periodo_inicial = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio()).getIteradorInicial();
		const Periodo periodo_final = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio()).getIteradorFinal();

		for (Periodo periodo = periodo_inicial; periodo <= periodo_final; getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio()).incrementarIterador(periodo)) {

			const double proporcao_tempo_viagem_no_periodo = getAtributo(a_idHidreletrica, AttComumHidreletrica_tempo_viagem_agua, int()) * std::pow(periodo.getHoras(), -1);

			if (proporcao_tempo_viagem_no_periodo >= getAtributo(AttComumDados_taxa_considerar_tempo_viagem_agua, double()))
				vetorHidreletrica.att(a_idHidreletrica).setAtributo(AttComumHidreletrica_considerar_tempo_viagem_agua, true);

		} // for (Periodo periodo = periodo_inicial; periodo <= periodo_final; getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio()).incrementarIterador(periodo)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::valida_considerar_tempo_viagem_agua(): " + getFullString(a_idHidreletrica) + std::string(erro.what())); }
} // void Dados::validaHidreletrica(){

bool Dados::isRestricaoEletrica_simples(const IdRestricaoEletrica a_idRestricaoEletrica){

	try{

		if ((a_idRestricaoEletrica <= IdRestricaoEletrica_Nenhum) || (a_idRestricaoEletrica >= IdRestricaoEletrica_Excedente))
			throw std::invalid_argument("Restricao Eletrica Invalida.");

		const IdElementoSistema maiorIdElementoSistema = getMaiorId(a_idRestricaoEletrica, IdElementoSistema());

		if (maiorIdElementoSistema == IdElementoSistema_Nenhum)
			throw std::invalid_argument("A restricao deve possuir ao menos um membro.");

		if (maiorIdElementoSistema > IdElementoSistema_1)
			return false;

		const TipoVariavelRestricaoOperativa tipoVariavelRestricaoOperativa = getAtributo(a_idRestricaoEletrica, IdElementoSistema_1, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());
		if (tipoVariavelRestricaoOperativa != TipoVariavelRestricaoOperativa_Nenhum)
			throw std::invalid_argument("Argumento " + getFullString(AttComumElementoSistema_tipoVariavelRestricaoOperativa) + " invalido.");

		if (getSize1Matriz(a_idRestricaoEletrica, IdElementoSistema_1, AttMatrizElementoSistema_fator_participacao) > 0) {
			const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());
			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
				for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {
					if (getElementoMatriz(a_idRestricaoEletrica, IdElementoSistema_1, AttMatrizElementoSistema_fator_participacao, periodo, idPatamarCarga, double()) != 1.0)
						return false;
				}
			} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
		} // if (getSize1Matriz(a_idRestricaoEletrica, IdElementoSistema_1, AttMatrizElementoSistema_fator_participacao) > 0) {

		else if (getSizeVetor(a_idRestricaoEletrica, IdElementoSistema_1, AttVetorElementoSistema_fator_participacao) > 0) {
			const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());
			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
				if (getElementoVetor(a_idRestricaoEletrica, IdElementoSistema_1, AttVetorElementoSistema_fator_participacao, periodo, double()) != 1.0)
					return false;
		} // else if (getSizeVetor(a_idRestricaoEletrica, IdElementoSistema_1, AttVetorElementoSistema_fator_participacao) > 0) {

		else
			if (getAtributo(a_idRestricaoEletrica, IdElementoSistema_1, AttComumElementoSistema_fator_participacao, double()) != 1.0)
				return false;

		return true;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::isRestricaoEletrica_simples(" + getFullString(a_idRestricaoEletrica) + "): \n" + std::string(erro.what())); }
} // bool Dados::isRestricaoEletrica_simples(const IdRestricaoEletrica a_idRestricao_eletrica){

bool Dados::isRestricaoOperativaUHE_simples(const IdRestricaoOperativaUHE a_idRestricaoOperativaUHE){

	try {

		if ((a_idRestricaoOperativaUHE <= IdRestricaoOperativaUHE_Nenhum) || (a_idRestricaoOperativaUHE >= IdRestricaoOperativaUHE_Excedente))
			throw std::invalid_argument("Restricao Eletrica Invalida.");

		const IdElementoSistema maiorIdElementoSistema = getMaiorId(a_idRestricaoOperativaUHE, IdElementoSistema());

		if (maiorIdElementoSistema == IdElementoSistema_Nenhum)
			throw std::invalid_argument("A restricao deve possuir ao menos um membro.");

		const TipoRestricaoHidraulica tipoRestricaoHidraulica = getAtributo(a_idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, TipoRestricaoHidraulica());

		if (tipoRestricaoHidraulica == TipoRestricaoHidraulica_variacao_percentual || tipoRestricaoHidraulica == TipoRestricaoHidraulica_variacao_absoluta)
			return false;

		if (maiorIdElementoSistema > IdElementoSistema_1)
			return false;

		const TipoVariavelRestricaoOperativa tipoVariavelRestricaoOperativa = getAtributo(a_idRestricaoOperativaUHE, IdElementoSistema_1, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa());
		if (tipoVariavelRestricaoOperativa == TipoVariavelRestricaoOperativa_Nenhum)
			throw std::invalid_argument("Argumento " + getFullString(AttComumElementoSistema_tipoVariavelRestricaoOperativa) + " invalido.");

		if ((tipoVariavelRestricaoOperativa != TipoVariavelRestricaoOperativa_vazao_defluente) && (tipoVariavelRestricaoOperativa != TipoVariavelRestricaoOperativa_vazao_desviada) && (tipoVariavelRestricaoOperativa != TipoVariavelRestricaoOperativa_vazao_bombeada) && (tipoVariavelRestricaoOperativa != TipoVariavelRestricaoOperativa_vazao_turbinada) && (tipoVariavelRestricaoOperativa != TipoVariavelRestricaoOperativa_volume_util))
			return false;

		if (getSize1Matriz(a_idRestricaoOperativaUHE, IdElementoSistema_1, AttMatrizElementoSistema_fator_participacao) > 0) {
			const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());
			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
				for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {
					if (getElementoMatriz(a_idRestricaoOperativaUHE, IdElementoSistema_1, AttMatrizElementoSistema_fator_participacao, periodo, idPatamarCarga, double()) != 1.0)
						return false;
				}
			} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
		} // if (getSize1Matriz(a_idRestricaoOperativaUHE, IdElementoSistema_1, AttMatrizElementoSistema_fator_participacao) > 0) {

		else if (getSizeVetor(a_idRestricaoOperativaUHE, IdElementoSistema_1, AttVetorElementoSistema_fator_participacao) > 0) {
			const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());
			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
				if (getElementoVetor(a_idRestricaoOperativaUHE, IdElementoSistema_1, AttVetorElementoSistema_fator_participacao, periodo, double()) != 1.0)
					return false;
		} // else if (getSizeVetor(a_idRestricaoOperativaUHE, IdElementoSistema_1, AttVetorElementoSistema_fator_participacao) > 0) {

		else
			if (getAtributo(a_idRestricaoOperativaUHE, IdElementoSistema_1, AttComumElementoSistema_fator_participacao, double()) != 1.0)
				return false;

		return true;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::isRestricaoOperativaUHE_simples(" + getFullString(a_idRestricaoOperativaUHE) + "): \n" + std::string(erro.what())); }
} // bool Dados::isRestricaoOperativaUHE_simples(const IdRestricaoOperativaUHE a_idRestricaoOperacionalUHE){


void Dados::validaPatamarDeficitEmSubmercado() {
	try {

		const IdSubmercado maiorIdSubmercado = getMaiorId(IdSubmercado());

		for (IdSubmercado idSubmercado = getMenorId(IdSubmercado()); idSubmercado <= maiorIdSubmercado; vetorSubmercado.incr(idSubmercado))
			validaPatamarDeficitEmSubmercado(idSubmercado);

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaPatamarDeficitEmSubmercado(): \n" + std::string(erro.what())); }
} // void Dados::validaPatamarDeficitEmSubmercado() {


void Dados::validaPatamarDeficitEmSubmercado(const IdSubmercado a_idSubmercado) {
	try {

		// Valida se os percentuais dos patamares est�o corretos.

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final = horizonte_estudo.getIteradorFinal();

		const IdPatamarDeficit maiorIdPatamarDeficit = getMaiorId(a_idSubmercado, IdPatamarDeficit());

		if (maiorIdPatamarDeficit != IdPatamarDeficit_Nenhum) {

			if (true) {

				double percentual_acumulado = 0.0;
				for (IdPatamarDeficit idPatamarDeficit = IdPatamarDeficit_1; idPatamarDeficit <= maiorIdPatamarDeficit; idPatamarDeficit++)
					percentual_acumulado += getAtributo(a_idSubmercado, idPatamarDeficit, AttComumPatamarDeficit_percentual, double());

				if ((percentual_acumulado > 0.0) && (!doubleCompara(percentual_acumulado, 1.0)))
					throw std::invalid_argument("A soma dos intervalos percentuais dos patamares de deficit do submercado " + getAtributo(a_idSubmercado, AttComumSubmercado_nome, string()) + " difere de 1.0 dada a precisao de " + getString(PRECISAO_COMPARACAO_DOUBLE) + ".");

			} // if (true) {

			for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

				double percentual_acumulado_por_periodo = 0.0;
				for (IdPatamarDeficit idPatamarDeficit = IdPatamarDeficit_1; idPatamarDeficit <= maiorIdPatamarDeficit; idPatamarDeficit++) {

					if (getSizeVetor(a_idSubmercado, idPatamarDeficit, AttVetorPatamarDeficit_percentual) > 0)
						percentual_acumulado_por_periodo += getElementoVetor(a_idSubmercado, idPatamarDeficit, AttVetorPatamarDeficit_percentual, periodo, double());

				} // for (IdPatamarDeficit idPatamarDeficit = IdPatamarDeficit_1; idPatamarDeficit <= maiorIdPatamarDeficit; idPatamarDeficit++) {

				if ((percentual_acumulado_por_periodo > 0.0) && (!doubleCompara(percentual_acumulado_por_periodo, 1.0)))
					throw std::invalid_argument("A soma dos intervalos percentuais dos patamares de deficit do submercado " + getAtributo(a_idSubmercado, AttComumSubmercado_nome, string()) + " difere de 1.0 em " + getString(periodo) + " dada a precisao de " + getString(PRECISAO_COMPARACAO_DOUBLE) + ".");

				const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
				for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

					double percentual_acumulado_por_periodo_por_patamarCarga = 0.0;
					for (IdPatamarDeficit idPatamarDeficit = IdPatamarDeficit_1; idPatamarDeficit <= maiorIdPatamarDeficit; idPatamarDeficit++) {

						if (getSize1Matriz(a_idSubmercado, idPatamarDeficit, AttMatrizPatamarDeficit_percentual) > 0)
							percentual_acumulado_por_periodo_por_patamarCarga += getElementoMatriz(a_idSubmercado, idPatamarDeficit, AttMatrizPatamarDeficit_percentual, periodo, idPatamarCarga, double());

					} // for (IdPatamarDeficit idPatamarDeficit = IdPatamarDeficit_1; idPatamarDeficit <= maiorIdPatamarDeficit; idPatamarDeficit++) {

					if ((percentual_acumulado_por_periodo_por_patamarCarga > 0.0) && (!doubleCompara(percentual_acumulado_por_periodo_por_patamarCarga, 1.0)))
						throw std::invalid_argument("A soma dos intervalos percentuais dos patamares de deficit do submercado " + getAtributo(a_idSubmercado, AttComumSubmercado_nome, string()) + " difere de 1.0 em " + getString(periodo) + " em " + getFullString(idPatamarCarga) + " dada a precisao de " + getString(PRECISAO_COMPARACAO_DOUBLE) + ".");

				} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

			} // for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

		} // if (maiorIdPatamarDeficit != IdPatamarDeficit_Nenhum) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaPatamarDeficitEmSubmercado(" + getFullString(a_idSubmercado) + "): \n" + std::string(erro.what())); }

} // void Dados::validaPatamarDeficitEmSubmercado() {


void Dados::validaConjuntoHidraulicoEmHidreletrica() {
	try {

		const IdSubmercado maiorIdSubmercado = getMaiorId(IdSubmercado());
		if (maiorIdSubmercado == IdSubmercado_Nenhum)
			throw std::invalid_argument("Necessaria a adicao dos dados de " + getString(Submercado()) + " em Dados para a validacao de AttComum de " + getString(ConjuntoHidraulico()) + ".");

		const IdEstagio idEstagioFinal = getIteradorFinal(AttVetorDados_horizonte_otimizacao, IdEstagio());;

		const IdHidreletrica maiorIdHidreletrica = getMaiorId(IdHidreletrica());

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		for (IdHidreletrica idUHE = getMenorId(IdHidreletrica()); idUHE <= maiorIdHidreletrica; vetorHidreletrica.incr(idUHE)) {

			const bool  considerar_usina = getAtributo(idUHE, AttComumHidreletrica_considerar_usina, bool());

			if (considerar_usina) {

				const IdConjuntoHidraulico maiorIdConjuntoHidraulico = getMaiorId(idUHE, IdConjuntoHidraulico());
				for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {

					const IdSubmercado idSubmercado = getAtributo(idUHE, idConjuntoHidraulico, AttComumConjuntoHidraulico_submercado, IdSubmercado());
					if (idSubmercado > maiorIdSubmercado)
						throw std::invalid_argument("O Submercado " + getString(idSubmercado) + " cadastrado no " + getFullString(idConjuntoHidraulico) + " de " + getFullString(idUHE) + " nao consta no cadastro do Dados.");

					if (idSubmercado == IdSubmercado_Nenhum)
						throw std::invalid_argument("Necessario adicao do submercado no " + getFullString(idConjuntoHidraulico) + " da " + getFullString(idUHE) + ".");

				} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {

			} // if (considerar_usina) {
		} // for (IdHidreletrica idUHE = getMenorId(IdHidreletrica()); idUHE <= maiorIdHidreletrica; idUHE++) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaConjuntoHidraulicoEmHidreletrica(): \n" + std::string(erro.what())); }
} // void Dados::validaConjuntoHidraulicoEmHidreletrica(){


void Dados::validaUnidadeUHE() {
	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_estudo_inicial = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();

		const IdEstagio idEstagioFinal = getIteradorFinal(AttVetorDados_horizonte_otimizacao, IdEstagio());;

		const IdHidreletrica maiorIdHidreletrica = getMaiorId(IdHidreletrica());

		const int numeroPeriodos = getSizeVetor(AttVetorDados_horizonte_estudo);



		for (IdHidreletrica idUHE = getMenorId(IdHidreletrica()); idUHE <= maiorIdHidreletrica; vetorHidreletrica.incr(idUHE)) {

			const bool  considerar_usina = getAtributo(idUHE, AttComumHidreletrica_considerar_usina, bool());

			if (considerar_usina) {

				const IdConjuntoHidraulico maiorIdConjuntoHidraulico = getMaiorId(idUHE, IdConjuntoHidraulico());

				for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {

					const IdUnidadeUHE maiorIdUnidadeUHE = getMaiorId(idUHE, idConjuntoHidraulico, IdUnidadeUHE());

					for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= maiorIdUnidadeUHE; idUnidade++) {

						for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

							const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

							for (IdPatamarCarga idPatamar = IdPatamarCarga_1; idPatamar <= maiorIdPatamarCarga; idPatamar++) {


							} // for (IdPatamarCarga idPatamar = IdPatamarCarga_1; idPatamar <= maiorIdPatamarCarga; idPatamar++) {

						} // for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

					} // for (IdUnidadeUHE idUnidade = IdUnidadeUHE_1; idUnidade <= maiorIdUnidadeUHE; idUnidade++) {

				} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {

			} // if (considerar_usina) {

		} // for (IdHidreletrica idUHE = getMenorId(IdHidreletrica()); idUHE <= maiorIdHidreletrica; idUHE++) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaUnidadeUHE(): \n" + std::string(erro.what())); }
} // void Dados::validaUnidadeUHE(){



void Dados::validacao_operacional_Hidreletrica(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar) {

	try {

		validaHidreletrica();

		validaReservatorioEmHidreletrica();

		validaConjuntoHidraulicoEmHidreletrica();

		validaUnidadeUHE();

		adicionaHidreletricasMontante();

		adicionaHidreletricasMontanteDesvio();

		validaDefluencia();

		const IdProcesso idProcesso = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_estudo_inicial = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();

		IdPatamarCarga maiorIdPatamarCarga_horizonte = IdPatamarCarga_Nenhum;
		for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {
			const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
			if (maiorIdPatamarCarga > maiorIdPatamarCarga_horizonte)
				maiorIdPatamarCarga_horizonte = maiorIdPatamarCarga;
		} // for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {


		bool impresso_AttVetorDefluencia_PorPeriodo = false;

		bool impresso_AttVetorFuncaoProducaoHidreletrica_por_usina = false;
		bool impresso_AttMatrizFuncaoProducaoHidreletrica_por_usina = false;
		bool impresso_AttVetorFuncaoProducaoHidreletrica_por_conjunto = false;
		bool impresso_AttMatrizFuncaoProducaoHidreletrica_por_conjunto = false;
		bool impresso_AttVetorFuncaoProducaoHidreletrica_por_unidade = false;
		bool impresso_AttMatrizFuncaoProducaoHidreletrica_por_unidade = false;

		bool recarregar_AttMatrizFuncaoProducaoHidreletrica_por_usina = false;
		bool recarregar_AttMatrizFuncaoProducaoHidreletrica_por_conjunto = false;
		bool recarregar_AttMatrizFuncaoProducaoHidreletrica_por_unidade = false;

		bool impresso_PolinomioCotaJusante = false;

		bool impresso_AttComumReservatorio = false;
		std::vector<bool> impresso_AttVetorReservatorio_PorIdMes(3, false);
		std::vector<bool> impresso_AttVetorReservatorio_PorPeriodo(3, false);
		std::vector<bool> impresso_AttMatrizReservatorio_PorCenarioPorPeriodo(3, false);

		bool recarregar_AttVetorReservatorio_PorIdMes = false;
		bool recarregar_AttVetorReservatorio_PorPeriodo = false;
		bool recarregar_AttMatrizReservatorio_PorCenarioPorPeriodo = false;


		bool impresso_AttComumHidreletrica = false;
		std::vector<bool> impresso_AttVetorHidreletrica_PorPeriodo(3, false);
		std::vector<bool> impresso_AttVetorHidreletrica_PorInteiro(3, false);
		std::vector<bool> impresso_AttMatrizHidreletrica_PorPeriodoPorIdPatamarCarga(3, false);
		std::vector<bool> impresso_AttMatrizHidreletrica_PorIdCenarioPorPeriodo(3, false);

		bool recarregar_AttVetorHidreletrica_PorPeriodo = false;
		bool recarregar_AttVetorHidreletrica_PorInteiro = false;
		bool recarregar_AttMatrizHidreletrica_PorPeriodoPorIdPatamarCarga = false;
		bool recarregar_AttMatrizHidreletrica_PorIdCenarioPorPeriodo = false;

		std::vector<bool> impresso_AttComumConjuntoHidraulico(3, false);
		std::vector<bool> impresso_AttVetorConjuntoHidraulico_PorPeriodo(3, false);
		std::vector<bool> impresso_AttMatrizConjuntoHidraulico_PorPeriodoPorIdPatamarCarga(3, false);

		bool recarregar_AttVetorConjuntoHidraulico_PorPeriodo = false;
		bool recarregar_AttMatrizConjuntoHidraulico_PorPeriodoPorIdPatamarCarga = false;

		std::vector<bool> impresso_AttComumUnidadeUHE(3, false);
		std::vector<bool> impresso_AttVetorUnidadeUHE_PorPeriodo(3, false);
		std::vector<bool> impresso_AttMatrizUnidadeUHE_PorPeriodoPorIdPatamarCarga(3, false);

		bool recarregar_AttVetorUnidadeUHE_PorPeriodo = false;
		bool recarregar_AttMatrizUnidadeUHE_PorPeriodoPorIdPatamarCarga = false;

		const IdHidreletrica maiorIdHidreletrica = getMaiorId(IdHidreletrica());
		const IdHidreletrica menorIdHidreletrica = getMenorId(IdHidreletrica());

		const int numIdHidreletrica = int(maiorIdHidreletrica - menorIdHidreletrica) + 1;

		SmartEnupla<IdHidreletrica, SmartEnupla<AttVetorReservatorio, PreencherAtributo>> preencher_AttVetorReservatorio(menorIdHidreletrica, std::vector<SmartEnupla<AttVetorReservatorio, PreencherAtributo>>(numIdHidreletrica, SmartEnupla<AttVetorReservatorio, PreencherAtributo>()));
		SmartEnupla<IdHidreletrica, SmartEnupla<AttMatrizReservatorio, PreencherAtributo>> preencher_AttMatrizReservatorio(menorIdHidreletrica, std::vector<SmartEnupla<AttMatrizReservatorio, PreencherAtributo>>(numIdHidreletrica, SmartEnupla<AttMatrizReservatorio, PreencherAtributo>()));

		SmartEnupla<IdHidreletrica, SmartEnupla<AttVetorHidreletrica, PreencherAtributo>> preencher_AttVetorHidreletrica(menorIdHidreletrica, std::vector<SmartEnupla<AttVetorHidreletrica, PreencherAtributo>>(numIdHidreletrica, SmartEnupla<AttVetorHidreletrica, PreencherAtributo>()));
		SmartEnupla<IdHidreletrica, SmartEnupla<AttMatrizHidreletrica, PreencherAtributo>> preencher_AttMatrizHidreletrica(menorIdHidreletrica, std::vector<SmartEnupla<AttMatrizHidreletrica, PreencherAtributo>>(numIdHidreletrica, SmartEnupla<AttMatrizHidreletrica, PreencherAtributo>()));

		SmartEnupla<IdHidreletrica, SmartEnupla<IdConjuntoHidraulico, SmartEnupla<AttVetorConjuntoHidraulico, PreencherAtributo>>> preencher_AttVetorConjuntoHidraulico(menorIdHidreletrica, std::vector<SmartEnupla<IdConjuntoHidraulico, SmartEnupla<AttVetorConjuntoHidraulico, PreencherAtributo>>>(numIdHidreletrica, SmartEnupla<IdConjuntoHidraulico, SmartEnupla<AttVetorConjuntoHidraulico, PreencherAtributo>>()));
		SmartEnupla<IdHidreletrica, SmartEnupla<IdConjuntoHidraulico, SmartEnupla<AttMatrizConjuntoHidraulico, PreencherAtributo>>> preencher_AttMatrizConjuntoHidraulico(menorIdHidreletrica, std::vector<SmartEnupla<IdConjuntoHidraulico, SmartEnupla<AttMatrizConjuntoHidraulico, PreencherAtributo>>>(numIdHidreletrica, SmartEnupla<IdConjuntoHidraulico, SmartEnupla<AttMatrizConjuntoHidraulico, PreencherAtributo>>()));

		SmartEnupla<IdHidreletrica, SmartEnupla<IdConjuntoHidraulico, SmartEnupla<IdUnidadeUHE, SmartEnupla<AttVetorUnidadeUHE, PreencherAtributo>>>> preencher_AttVetorUnidadeUHE(menorIdHidreletrica, std::vector<SmartEnupla<IdConjuntoHidraulico, SmartEnupla<IdUnidadeUHE, SmartEnupla<AttVetorUnidadeUHE, PreencherAtributo>>>>(numIdHidreletrica, SmartEnupla<IdConjuntoHidraulico, SmartEnupla<IdUnidadeUHE, SmartEnupla<AttVetorUnidadeUHE, PreencherAtributo>>>()));
		SmartEnupla<IdHidreletrica, SmartEnupla<IdConjuntoHidraulico, SmartEnupla<IdUnidadeUHE, SmartEnupla<AttMatrizUnidadeUHE, PreencherAtributo>>>> preencher_AttMatrizUnidadeUHE(menorIdHidreletrica, std::vector<SmartEnupla<IdConjuntoHidraulico, SmartEnupla<IdUnidadeUHE, SmartEnupla<AttMatrizUnidadeUHE, PreencherAtributo>>>>(numIdHidreletrica, SmartEnupla<IdConjuntoHidraulico, SmartEnupla<IdUnidadeUHE, SmartEnupla<AttMatrizUnidadeUHE, PreencherAtributo>>>()));

		SmartEnupla<IdHidreletrica, SmartEnupla<IdConjuntoHidraulico, SmartEnupla<IdUnidadeUHE, SmartEnupla<AttVetorFuncaoProducaoHidreletrica, PreencherAtributo>>>> preencher_AttVetorFuncaoProducaoHidreletrica(menorIdHidreletrica, std::vector<SmartEnupla<IdConjuntoHidraulico, SmartEnupla<IdUnidadeUHE, SmartEnupla<AttVetorFuncaoProducaoHidreletrica, PreencherAtributo>>>>(numIdHidreletrica, SmartEnupla<IdConjuntoHidraulico, SmartEnupla<IdUnidadeUHE, SmartEnupla<AttVetorFuncaoProducaoHidreletrica, PreencherAtributo>>>()));
		SmartEnupla<IdHidreletrica, SmartEnupla<IdConjuntoHidraulico, SmartEnupla<IdUnidadeUHE, SmartEnupla<AttMatrizFuncaoProducaoHidreletrica, PreencherAtributo>>>> preencher_AttMatrizFuncaoProducaoHidreletrica(menorIdHidreletrica, std::vector<SmartEnupla<IdConjuntoHidraulico, SmartEnupla<IdUnidadeUHE, SmartEnupla<AttMatrizFuncaoProducaoHidreletrica, PreencherAtributo>>>>(numIdHidreletrica, SmartEnupla<IdConjuntoHidraulico, SmartEnupla<IdUnidadeUHE, SmartEnupla<AttMatrizFuncaoProducaoHidreletrica, PreencherAtributo>>>()));

		SmartEnupla<IdHidreletrica, SmartEnupla<IdPolinomioJusante, SmartEnupla<AttVetorPolinomioJusante, PreencherAtributo>>> preencher_AttVetorPolinomioJusante(menorIdHidreletrica, std::vector<SmartEnupla<IdPolinomioJusante, SmartEnupla<AttVetorPolinomioJusante, PreencherAtributo>>>(numIdHidreletrica, SmartEnupla<IdPolinomioJusante, SmartEnupla<AttVetorPolinomioJusante, PreencherAtributo>>()));


		for (IdHidreletrica idHidreletrica = menorIdHidreletrica; idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

			if (getAtributo(idHidreletrica, AttComumHidreletrica_considerar_usina, bool())) {

				bool hidreletrica_com_unidade = true;
				for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {
					if (getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()) != IdUnidadeUHE_Nenhum) {
						hidreletrica_com_unidade = true;
						break;
					}
				}

				try {

					preencher_AttVetorReservatorio.at(idHidreletrica) = SmartEnupla<AttVetorReservatorio, PreencherAtributo>(AttVetorReservatorio(AttVetorReservatorio_Nenhum + 1), std::vector<PreencherAtributo>(AttVetorReservatorio(AttVetorReservatorio_Excedente - 1), nao_sem_utilizacao));
					preencher_AttMatrizReservatorio.at(idHidreletrica) = SmartEnupla<AttMatrizReservatorio, PreencherAtributo>(AttMatrizReservatorio(AttMatrizReservatorio_Nenhum + 1), std::vector<PreencherAtributo>(AttMatrizReservatorio(AttMatrizReservatorio_Excedente - 1), nao_sem_utilizacao));

					// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
					//
					//                                                                                   Reservatorio
					//
					// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

					if ((getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_enchendo_volume_morto) > 0) && (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_taxa_enchimento_volume_morto) > 0))
						if (getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_enchendo_volume_morto, horizonte_estudo.getIteradorInicial(), int()) == 1)
							preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_meta_enchimento_volume_morto) = sim_operacional;


					if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_morto_completo) == 0) {
						vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_volume_morto_completo, SmartEnupla<Periodo, int>(horizonte_estudo, 1));
						preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_volume_morto_completo) = sim_premissa;
					}
					else
						preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_volume_morto_completo) = nao_operacional_informado;

					if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_enchendo_volume_morto) == 0) {
						vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_enchendo_volume_morto, SmartEnupla<Periodo, int>(horizonte_estudo, 1));
						preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_enchendo_volume_morto) = sim_operacional;
					}
					else
						preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_enchendo_volume_morto) = nao_operacional_informado;

					if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo) == 0) {
						vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_volume_minimo, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_minimo, double())));
						preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_volume_minimo) = sim_operacional;
					}
					else
						preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_volume_minimo) = nao_operacional_informado;

					//CALCULA O VOLUME META QUANDO ESTA ENCHENDO O VOLUME MORTO
					if ((preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_meta_enchimento_volume_morto) == sim_operacional) && (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_taxa_enchimento_volume_morto) > 0)) {
						for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {
							double volume_meta = 0;
							if (periodo == periodo_estudo_inicial)
								volume_meta = getAtributo(idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_morto, double());
							else {
								Periodo periodo_anterior = periodo;
								horizonte_estudo.decrementarIterador(periodo_anterior);
								volume_meta = getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_meta_enchimento_volume_morto, periodo_anterior, double());
							}
							volume_meta += getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_taxa_enchimento_volume_morto, periodo, double()) * getElementoVetor(AttVetorDados_conversor_vazao_volume, periodo, double());

							const double volume_minimo = getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo, periodo, double());

							if (volume_meta > volume_minimo) { volume_meta = volume_minimo; }
							vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).addElemento(AttVetorReservatorio_meta_enchimento_volume_morto, periodo, volume_meta);
						}
					}


			

					// CALCULO DO VOLUME �TIL M�XIMO
					
					if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo) == 0) {

						preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_volume_util_maximo) = sim_operacional;

						if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_maximo) == 0) {
							preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_volume_maximo) = sim_premissa;
							vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_volume_maximo, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idHidreletrica, IdReservatorio_1, AttComumReservatorio_volume_maximo, double())));
						}
						else
							preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_volume_maximo) = nao_premissa_informado;

						if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_percentual_volume_util_maximo) == 0) {
							vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_percentual_volume_util_maximo, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idHidreletrica, IdReservatorio_1, AttComumReservatorio_percentual_volume_util_maximo, double())));
							preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_percentual_volume_util_maximo) = sim_premissa;
						}
						else
							preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_percentual_volume_util_maximo) = nao_premissa_informado;

						vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_volume_util_maximo, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));

						for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

							const double volume_minimo = getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo, periodo, double());
							const double volume_maximo = getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_maximo, periodo, double());

							if (volume_minimo > volume_maximo)
								throw std::invalid_argument("Volume Minimo = " + getString(volume_minimo) + " maior que Volume Maximo = " + getString(volume_maximo) + " em " + getFullString(idHidreletrica) + " em " + getString(periodo));

							const double percentual_volume_util_maximo = getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_percentual_volume_util_maximo, periodo, double());

							double volume_util_maximo = (volume_maximo - volume_minimo) * percentual_volume_util_maximo;

							if (getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_morto_completo, periodo, int()) != 1)
								volume_util_maximo = 0.0;

							// CASO ESTEJA ENCHENDO VOLUME MORTO O VOLUME UTIL � IGUAL AO VOLUME M�XIMO
							if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_enchendo_volume_morto) > 0)
								if (getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_enchendo_volume_morto, horizonte_estudo.getIteradorInicial(), int()) == 1)
									vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_util_maximo, periodo, volume_maximo);

							vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_util_maximo, periodo, volume_util_maximo);

						} // for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)){

					} // if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo) == 0) {

					else if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo) > 0) {
						preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_volume_util_minimo) = nao_operacional_informado;

						if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_maximo) > 0)
							throw std::invalid_argument("O atributo " + getString(AttVetorReservatorio_volume_maximo) + " nao deve ser informado na presenca de " + getString(AttVetorReservatorio_volume_util_minimo) + " em " + getFullString(idHidreletrica));

						if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_percentual_volume_util_maximo) > 0)
							throw std::invalid_argument("O atributo " + getString(AttVetorReservatorio_percentual_volume_util_maximo) + " nao deve ser informado na presenca de " + getString(AttVetorReservatorio_volume_util_minimo) + " em " + getFullString(idHidreletrica));

					} // else if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo) > 0) {


					// CALCULO DO VOLUME �TIL M�NIMO
					if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_minimo) == 0) {

						if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_percentual_volume_util_minimo) > 0)
							preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_percentual_volume_util_minimo) = nao_premissa_informado;
					
						else {
							vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_percentual_volume_util_minimo, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idHidreletrica, IdReservatorio_1, AttComumReservatorio_percentual_volume_util_minimo, double())));
							preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_percentual_volume_util_minimo) = sim_premissa;
						}
					
						for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

							const double volume_minimo = getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo, periodo, double());
							const double volume_maximo = getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_maximo, periodo, double());

							if (volume_minimo > volume_maximo)
								throw std::invalid_argument("Volume Minimo = " + getString(volume_minimo) + " maior que Volume Maximo = " + getString(volume_maximo) + " em " + getFullString(idHidreletrica) + " em " + getString(periodo));

							const double percentual_volume_util_minimo = getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_percentual_volume_util_minimo, periodo, double());

							const double volume_util_minimo = (volume_maximo - volume_minimo) * percentual_volume_util_minimo;

							const double volume_util_maximo = getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, periodo, double());

							if (volume_util_minimo < getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, periodo, double()))
								vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).addElemento(AttVetorReservatorio_volume_util_minimo, periodo, volume_util_minimo);
							else
								vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).addElemento(AttVetorReservatorio_volume_util_minimo, periodo, volume_util_maximo);

						} // for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

						preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_volume_util_minimo) = sim_operacional;

					} // if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_minimo) == 0) {

					else if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_minimo) > 0) {
						preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_volume_util_minimo) = nao_operacional_informado;

						if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_percentual_volume_util_minimo) > 0)
							throw std::invalid_argument("O atributo " + getString(AttVetorReservatorio_percentual_volume_util_minimo) + " nao deve ser informado na presenca de " + getString(AttVetorReservatorio_volume_util_minimo) + " em " + getFullString(idHidreletrica));

					} // else if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_minimo) > 0) {

					if (((volume_inicial_carregado_from_premissa) && (!volume_inicial_carregado_from_operacional)) || \
						((volume_inicial_carregado_from_operacional) && (volume_inicial_carregado_from_premissa) && (getAtributo(idHidreletrica, IdReservatorio_1, AttComumReservatorio_percentual_volume_util_inicial, double()) > 0.0))) {

						double percentual_volume_util_maximo = 1.0;

						if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_percentual_volume_util_maximo) > 0)
							percentual_volume_util_maximo = getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_percentual_volume_util_maximo, horizonte_estudo.getIteradorInicial(), double());

						const double volume_util = getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, horizonte_estudo.getIteradorInicial(), double());
						const double volume_util_inicial = (volume_util * getAtributo(idHidreletrica, IdReservatorio_1, AttComumReservatorio_percentual_volume_util_inicial, double())) / percentual_volume_util_maximo;

						vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setAtributo(AttComumReservatorio_volume_util_inicial, volume_util_inicial);

					}// if (volume_inicial_carregado_from_premissa)


					if (true) {

						std::vector<IdRestricaoOperativaUHE> vetor_idRestricaoOperativaUHE_comUHE_volume_util;

						if ((getAtributo(AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_hidraulicas, bool())) && (a_imprimir_atributos_sem_recarregar)) {
							for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= getMaiorId(IdRestricaoOperativaUHE()); idRestricaoOperativaUHE++) {
								if ((getAtributo(idRestricaoOperativaUHE, IdElementoSistema_1, AttComumElementoSistema_hidreletrica, IdHidreletrica()) == idHidreletrica) && (getAtributo(idRestricaoOperativaUHE, IdElementoSistema_1, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa()) == TipoVariavelRestricaoOperativa_volume_util) && (isRestricaoOperativaUHE_simples(idRestricaoOperativaUHE)))
									vetor_idRestricaoOperativaUHE_comUHE_volume_util.push_back(idRestricaoOperativaUHE);
							}
						} // if (getAtributo(AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_hidraulicas, bool())) {

						if (vetor_idRestricaoOperativaUHE_comUHE_volume_util.size() > 0) {
							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {

									for (int pos = 0; pos < int(vetor_idRestricaoOperativaUHE_comUHE_volume_util.size()); pos++) {

										if (getElementoMatriz(vetor_idRestricaoOperativaUHE_comUHE_volume_util.at(pos), AttMatrizRestricaoOperativaUHE_limite_inferior, periodo, idPatamarCarga, double()) > getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_minimo, periodo, double()))
											vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_util_minimo, periodo, getElementoMatriz(vetor_idRestricaoOperativaUHE_comUHE_volume_util.at(pos), AttMatrizRestricaoOperativaUHE_limite_inferior, periodo, idPatamarCarga, double()));

										if (getElementoMatriz(vetor_idRestricaoOperativaUHE_comUHE_volume_util.at(pos), AttMatrizRestricaoOperativaUHE_limite_superior, periodo, idPatamarCarga, double()) < getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, periodo, double()))
											vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttVetorReservatorio_volume_util_maximo, periodo, getElementoMatriz(vetor_idRestricaoOperativaUHE_comUHE_volume_util.at(pos), AttMatrizRestricaoOperativaUHE_limite_superior, periodo, idPatamarCarga, double()));

									}//for (int pos = 0; pos < int(vetor_idRestricaoOperativaUHE_comUHE.size()); pos++) {
								} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {
							} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
						} // if (vetor_idRestricaoOperativaUHE_comUHE_volume_util.size() > 0) {

					} // if (true) {


					//////////////////////

					if ((getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_coef_linear_evaporacao_0) == 0) && (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_coef_linear_evaporacao_1) == 0)) {

						if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_volume_0) > 0)
							preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_poli_cota_volume_0) = nao_premissa_informado;
						else
							throw std::invalid_argument("Necessario atributo " + getFullString(AttVetorReservatorio_poli_cota_volume_0) + " para calculo de evaporacao");

						if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_volume_1) > 0)
							preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_poli_cota_volume_1) = nao_premissa_informado;
						else
							throw std::invalid_argument("Necessario atributo " + getFullString(AttVetorReservatorio_poli_cota_volume_1) + " para calculo de evaporacao");

						if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_volume_2) > 0)
							preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_poli_cota_volume_2) = nao_premissa_informado;
						else
							throw std::invalid_argument("Necessario atributo " + getFullString(AttVetorReservatorio_poli_cota_volume_2) + " para calculo de evaporacao");

						if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_volume_3) > 0)
							preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_poli_cota_volume_3) = nao_premissa_informado;
						else
							throw std::invalid_argument("Necessario atributo " + getFullString(AttVetorReservatorio_poli_cota_volume_3) + " para calculo de evaporacao");

						if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_volume_4) > 0)
							preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_poli_cota_volume_4) = nao_premissa_informado;
						else
							throw std::invalid_argument("Necessario atributo " + getFullString(AttVetorReservatorio_poli_cota_volume_4) + " para calculo de evaporacao");

						if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_area_0) > 0)
							preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_poli_cota_area_0) = nao_premissa_informado;
						else
							throw std::invalid_argument("Necessario atributo " + getFullString(AttVetorReservatorio_poli_cota_area_0) + " para calculo de evaporacao");

						if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_area_1) > 0)
							preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_poli_cota_area_1) = nao_premissa_informado;
						else
							throw std::invalid_argument("Necessario atributo " + getFullString(AttVetorReservatorio_poli_cota_area_1) + " para calculo de evaporacao");

						if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_area_2) > 0)
							preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_poli_cota_area_2) = nao_premissa_informado;
						else
							throw std::invalid_argument("Necessario atributo " + getFullString(AttVetorReservatorio_poli_cota_area_2) + " para calculo de evaporacao");

						if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_area_3) > 0)
							preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_poli_cota_area_3) = nao_premissa_informado;
						else
							throw std::invalid_argument("Necessario atributo " + getFullString(AttVetorReservatorio_poli_cota_area_3) + " para calculo de evaporacao");

						if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_poli_cota_area_4) > 0)
							preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_poli_cota_area_4) = nao_premissa_informado;
						else
							throw std::invalid_argument("Necessario atributo " + getFullString(AttVetorReservatorio_poli_cota_area_4) + " para calculo de evaporacao");

						if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_evaporacao) == 0) {
							vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setVetor(AttVetorReservatorio_evaporacao, SmartEnupla<IdMes, double>(IdMes_1, std::vector<double>(IdMes_12, getAtributo(idHidreletrica, IdReservatorio_1, AttComumReservatorio_evaporacao, double()))));
							preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_evaporacao) = sim_premissa;
						}
						else
							preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_evaporacao) = nao_premissa_informado;

						preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_coef_linear_evaporacao_0) = sim_operacional;
						preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_coef_linear_evaporacao_1) = sim_operacional;

						for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {
							const double volume_minimo = getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo, periodo, double());
							const double volume_util = getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, periodo, double());
							vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).calculaAproximacaoLinearEvaporacao(volume_minimo, volume_minimo + volume_util, periodo);
						}

					}
					else if ((getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_coef_linear_evaporacao_0) == 0) || (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_coef_linear_evaporacao_1) == 0))
						throw std::invalid_argument("Ambos atributos " + getFullString(AttVetorReservatorio_coef_linear_evaporacao_0) + " e " + getFullString(AttVetorReservatorio_coef_linear_evaporacao_1) + " devem ser informados em " + getFullString(idHidreletrica));

					else {
						preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_coef_linear_evaporacao_0) = nao_operacional_informado;
						preencher_AttVetorReservatorio.at(idHidreletrica).at(AttVetorReservatorio_coef_linear_evaporacao_1) = nao_operacional_informado;
					}

					if ((getSize1Matriz(idHidreletrica, IdReservatorio_1, AttMatrizReservatorio_volume_meta) > 0) && (getAtributo(AttComumDados_tipo_estudo, TipoEstudo()) == TipoEstudo_simulacao)) {
						if (getAtributo(AttComumDados_numero_cenarios, int()) != getSize1Matriz(idHidreletrica, IdReservatorio_1, AttMatrizReservatorio_volume_meta))
							throw std::invalid_argument("Numero de cenarios em " + getFullString(AttMatrizReservatorio_volume_meta) + " diferente de " + getFullString(AttComumDados_numero_cenarios));
						preencher_AttMatrizReservatorio.at(idHidreletrica).at(AttMatrizReservatorio_volume_meta) = nao_operacional_informado;

						//V�lida se o volume_util_maximo <= volume_meta (alguns problemas de arredondamento encontrados ao ler o volume_meta)
						const IdCenario menorIdCenario = getIterador1Inicial(idHidreletrica, IdReservatorio_1, AttMatrizReservatorio_volume_meta, IdCenario());
						const IdCenario maiorIdCenario = getIterador1Final(idHidreletrica, IdReservatorio_1, AttMatrizReservatorio_volume_meta, IdCenario());

						for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							const double volume_util_maximo = getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, periodo, double());

							for (IdCenario idCenario = menorIdCenario; idCenario <= maiorIdCenario; idCenario++) {
								const double volume_meta = getElementoMatriz(idHidreletrica, IdReservatorio_1, AttMatrizReservatorio_volume_meta, idCenario, periodo, double());

								if ((volume_meta != getdoubleFromChar("max")) && (volume_meta > volume_util_maximo)) {

									const double diferenca_volume = volume_meta - volume_util_maximo;

									if (diferenca_volume > 0.1) //Toler�ncia de 0.1 hm3
										throw std::invalid_argument("Atributo " + getFullString(AttMatrizReservatorio_volume_meta) + " em " + getFullString(idHidreletrica) + " " + getFullString(idCenario) + " " + getFullString(periodo) + " maior ao volume_espera");

									vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).setElemento(AttMatrizReservatorio_volume_meta, idCenario, periodo, volume_util_maximo);

								}//if ((volume_meta != getdoubleFromChar("max")) && (volume_meta > volume_util_maximo)) {

							}//for (IdCenario idCenario = menorIdCenario; idCenario <= maiorIdCenario; idCenario++) {

						}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

					}
					else if (getSize1Matriz(idHidreletrica, IdReservatorio_1, AttMatrizReservatorio_volume_meta) > 0)
						throw std::invalid_argument("Atributo " + getFullString(AttMatrizReservatorio_volume_meta) + " em " + getFullString(idHidreletrica) + " nao compativel com " + getFullString(getAtributo(AttComumDados_tipo_estudo, TipoEstudo())));



				} // try{
				catch (const std::exception& erro) { throw std::invalid_argument("RESERVATORIO em " + getFullString(idHidreletrica) + ": \n" + std::string(erro.what())); }



				// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				//
				//                                                                                   Hidreletrica
				//
				// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

				preencher_AttVetorHidreletrica.at(idHidreletrica) = SmartEnupla<AttVetorHidreletrica, PreencherAtributo>(AttVetorHidreletrica(AttVetorHidreletrica_Nenhum + 1), std::vector<PreencherAtributo>(AttVetorHidreletrica(AttVetorHidreletrica_Excedente - 1), nao_sem_utilizacao));
				preencher_AttMatrizHidreletrica.at(idHidreletrica) = SmartEnupla<AttMatrizHidreletrica, PreencherAtributo>(AttMatrizHidreletrica(AttMatrizHidreletrica_Nenhum + 1), std::vector<PreencherAtributo>(AttMatrizHidreletrica(AttMatrizHidreletrica_Excedente - 1), nao_sem_utilizacao));


				// ------------------------------------
				//
				// AttVetorHidreletrica_tipo_detalhamento_producao
				//
				// ------------------------------------

				if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_tipo_detalhamento_producao) == 0) {

					const TipoDetalhamentoProducaoHidreletrica tipo_detalhamento_producao = getAtributo(idHidreletrica, AttComumHidreletrica_tipo_detalhamento_producao, TipoDetalhamentoProducaoHidreletrica());

					if ((tipo_detalhamento_producao != TipoDetalhamentoProducaoHidreletrica_sem_producao) && (!hidreletrica_com_unidade))
						throw std::invalid_argument("O atributo " + getFullString(AttComumHidreletrica_tipo_detalhamento_producao) + " nao deve ser " + getFullString(tipo_detalhamento_producao) + " caso nao sejam informadas unidades na hidreletrica.");

					preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_tipo_detalhamento_producao) = sim_operacional;
					vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_tipo_detalhamento_producao, SmartEnupla<Periodo, TipoDetalhamentoProducaoHidreletrica>(horizonte_estudo, tipo_detalhamento_producao));

				} // if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_tipo_detalhamento_producao) == 0) {

				else if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_tipo_detalhamento_producao) > 0)
					preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_tipo_detalhamento_producao) = nao_operacional_informado;

				SmartEnupla<TipoDetalhamentoProducaoHidreletrica, SmartEnupla<Periodo, double>>  vetor_zero(TipoDetalhamentoProducaoHidreletrica(1), std::vector<SmartEnupla<Periodo, double>>(TipoDetalhamentoProducaoHidreletrica(TipoDetalhamentoProducaoHidreletrica_Excedente - 1), SmartEnupla<Periodo, double>()));
				SmartEnupla<TipoDetalhamentoProducaoHidreletrica, SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>> matriz_zero(TipoDetalhamentoProducaoHidreletrica(1), std::vector<SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>>(TipoDetalhamentoProducaoHidreletrica(TipoDetalhamentoProducaoHidreletrica_Excedente - 1), SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>()));

				for (TipoDetalhamentoProducaoHidreletrica tipo_detalhamento_producao = TipoDetalhamentoProducaoHidreletrica(1); tipo_detalhamento_producao < TipoDetalhamentoProducaoHidreletrica_Excedente; tipo_detalhamento_producao++) {

					int contador_encontro = 0;
					bool tipo_encontrado = false;

					for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

						const TipoDetalhamentoProducaoHidreletrica tipo_detalhamento_producao_carregado = getElementoVetor(idHidreletrica, AttVetorHidreletrica_tipo_detalhamento_producao, periodo, TipoDetalhamentoProducaoHidreletrica());

						const SmartEnupla<IdPatamarCarga, double> vetor_zero_patamar(IdPatamarCarga_1, std::vector<double>(getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()), 0.0));

						if ((tipo_detalhamento_producao == tipo_detalhamento_producao_carregado) && (!tipo_encontrado)) {
							tipo_encontrado = true;
							contador_encontro++;
							if (contador_encontro > 1)
								throw std::invalid_argument("Os elementos de " + getFullString(AttVetorHidreletrica_tipo_detalhamento_producao) + " devem ser informados em blocos, sem repeticao de blocos ao longo do horizonte.");
							vetor_zero.at(tipo_detalhamento_producao).addElemento(periodo, 0.0);
							matriz_zero.at(tipo_detalhamento_producao).addElemento(periodo, vetor_zero_patamar);
						}

						else if ((tipo_detalhamento_producao == tipo_detalhamento_producao_carregado) && (tipo_encontrado)) {
							vetor_zero.at(tipo_detalhamento_producao).addElemento(periodo, 0.0);
							matriz_zero.at(tipo_detalhamento_producao).addElemento(periodo, vetor_zero_patamar);
						}

						else if (tipo_detalhamento_producao != tipo_detalhamento_producao_carregado)
							tipo_encontrado = false;

					} // for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

					if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_sem_producao).size() > 0) {
						if ((tipo_detalhamento_producao == TipoDetalhamentoProducaoHidreletrica_sem_producao) && (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_sem_producao).size() != horizonte_estudo.size()))
							throw std::invalid_argument("Os elementos " + getFullString(TipoDetalhamentoProducaoHidreletrica_sem_producao) + " de " + getFullString(AttVetorHidreletrica_tipo_detalhamento_producao) + " quando informados devem constar em todo o horizonte.");
					}

				} // for (TipoDetalhamentoProducaoHidreletrica tipo_detalhamento_producao = TipoDetalhamentoProducaoHidreletrica(1); tipo_detalhamento_producao < TipoDetalhamentoProducaoHidreletrica_Excedente; tipo_detalhamento_producao++) {


				// -------------------------------------
				//
				// AttMatrizHidreletrica_potencia_minima
				//
				// -------------------------------------

				SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> potencia_minima_hidreletrica;

				if ((getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_minima) == 0) && (hidreletrica_com_unidade)) {

					if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0) {
						preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_minima) = sim_operacional;
						potencia_minima_hidreletrica = matriz_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina);
					} // if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0) {

				} // if ((getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_minima) == 0) && (hidreletrica_com_unidade)) {

				if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_minima) > 0) {

					if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_sem_producao).size() >= horizonte_estudo.size())
						throw std::invalid_argument("O atributo " + getFullString(AttMatrizHidreletrica_potencia_minima) + " nao deve ser informado em " + getFullString(idHidreletrica));
					else
						preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_minima) = nao_operacional_informado;
				}

				// -------------------------------------
				//
				// AttMatrizHidreletrica_potencia_maxima
				//
				// -------------------------------------

				SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> potencia_maxima_hidreletrica;

				if ((getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_maxima) == 0) && (hidreletrica_com_unidade)) {

					if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0) {
						preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_maxima) = sim_operacional;
						potencia_maxima_hidreletrica = matriz_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina);
					} // if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0) {

				} // if ((getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_maxima) == 0) && (hidreletrica_com_unidade)) {

				if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_maxima) > 0) {

					if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_sem_producao).size() >= horizonte_estudo.size())
						throw std::invalid_argument("O atributo " + getFullString(AttMatrizHidreletrica_potencia_maxima) + " nao deve ser informado em " + getFullString(idHidreletrica));
					else
						preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_maxima) = nao_operacional_informado;

				}


				// -------------------------------------
				//
				// AttMatrizHidreletrica_potencia_disponivel_minima
				//
				// -------------------------------------

				if ((getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_minima) == 0) && (hidreletrica_com_unidade)) {

					if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0)
						preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_disponivel_minima) = sim_operacional;

				} // if ((getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_minima) == 0) && (hidreletrica_com_unidade)) {

				if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_minima) > 0) {

					if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_sem_producao).size() >= horizonte_estudo.size())
						throw std::invalid_argument("O atributo " + getFullString(AttMatrizHidreletrica_potencia_disponivel_minima) + " nao deve ser informado em " + getFullString(idHidreletrica));
					else
						preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_disponivel_minima) = nao_operacional_informado;
				}

				// -------------------------------------
				//
				// AttMatrizHidreletrica_potencia_disponivel_maxima
				//
				// -------------------------------------

				if ((getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_maxima) == 0) && (hidreletrica_com_unidade)) {

					if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0)
						preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_disponivel_maxima) = sim_operacional;

				} // if ((getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_maxima) == 0) && (hidreletrica_com_unidade)) {

				if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_maxima) > 0) {

					if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_sem_producao).size() >= horizonte_estudo.size())
						throw std::invalid_argument("O atributo " + getFullString(AttMatrizHidreletrica_potencia_disponivel_maxima) + " nao deve ser informado em " + getFullString(idHidreletrica));
					else
						preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_disponivel_maxima) = nao_operacional_informado;
				}

				// ------------------------------------
				//
				// AttVetorHidreletrica_disponibilidade
				//
				// ------------------------------------
				// 

				SmartEnupla<Periodo, double> disponibilidade_hidreletrica;

				if ((getSizeVetor(idHidreletrica, AttVetorHidreletrica_disponibilidade) == 0) && (hidreletrica_com_unidade)) {

					if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0) {

						preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_disponibilidade) = sim_operacional;

						disponibilidade_hidreletrica = vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina);

					} // if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0) {

				} // if ((getSizeVetor(idHidreletrica, AttVetorHidreletrica_disponibilidade) == 0) && (hidreletrica_com_unidade)) {

				else if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_disponibilidade) > 0) {

					if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0)
						preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_disponibilidade) = nao_operacional_informado;

					else
						throw std::invalid_argument("O atributo " + getFullString(AttVetorHidreletrica_disponibilidade) + " nao deve ser informado em " + getFullString(idHidreletrica));

				} // else if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_disponibilidade) > 0) {


				// ----------------------------------------
				//
				// AttVetorHidreletrica_vazao_turbinada_minima
				//
				// ----------------------------------------

				SmartEnupla<Periodo, double> vazao_turbinada_minima_hidreletrica;

				if ((getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_minima) == 0) && (hidreletrica_com_unidade)) {
					if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0) {
						preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_turbinada_minima) = sim_operacional;
						vazao_turbinada_minima_hidreletrica = vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina);
					}
				} // if ((getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_minima) == 0) && (hidreletrica_com_unidade)) {

				else if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_minima) > 0) {
					if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0)
						preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_turbinada_minima) = nao_operacional_informado;
					else
						throw std::invalid_argument("O atributo " + getFullString(AttVetorHidreletrica_vazao_turbinada_minima) + " nao deve ser informado em " + getFullString(idHidreletrica));
				}

				// ----------------------------------------
				//
				// AttVetorHidreletrica_vazao_turbinada_maxima
				//
				// ----------------------------------------

				SmartEnupla<Periodo, double> vazao_turbinada_maxima_hidreletrica;

				if ((getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_maxima) == 0) && (hidreletrica_com_unidade)) {
					if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0) {
						preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_turbinada_maxima) = sim_operacional;
						vazao_turbinada_maxima_hidreletrica = vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina);
					}
				} // if ((getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_maxima) == 0) && (hidreletrica_com_unidade)) {

				else if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_maxima) > 0) {
					if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0)
						preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_turbinada_maxima) = nao_operacional_informado;
					else
						throw std::invalid_argument("O atributo " + getFullString(AttVetorHidreletrica_vazao_turbinada_maxima) + " nao deve ser informado em " + getFullString(idHidreletrica));
				}



				// --------------------------------------
				//
				// AttVetorHidreletrica_vazao_defluente_minima
				//
				// --------------------------------------
				// 

				if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_defluente_minima) == 0) {
					vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_vazao_defluente_minima, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idHidreletrica, AttComumHidreletrica_vazao_defluente_minima, double())));
					preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_defluente_minima) = sim_operacional;
				}
				else
					preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_defluente_minima) = nao_operacional_informado;


				// --------------------------------------
				//
				// AttVetorHidreletrica_vazao_defluente_maxima
				//
				// --------------------------------------
				// 

				if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_defluente_maxima) == 0) {
					vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_vazao_defluente_maxima, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idHidreletrica, AttComumHidreletrica_vazao_defluente_maxima, double())));
					preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_defluente_maxima) = sim_operacional;
				}
				else
					preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_defluente_maxima) = nao_operacional_informado;


				// --------------------------------------
				//
				// AttVetorHidreletrica_vazao_retirada
				//
				// --------------------------------------

				if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_retirada) == 0) {

					vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_vazao_retirada, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idHidreletrica, AttComumHidreletrica_vazao_retirada, double())));
					preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_retirada) = sim_operacional;
				}
				else
					preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_retirada) = nao_operacional_informado;

				// ------------------------------------
				//
				// AttVetorHidreletrica_regularizacao
				//
				// ------------------------------------
				// 

				if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_regularizacao) == 0)
					preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_regularizacao) = sim_operacional;
				else
					preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_regularizacao) = nao_operacional_informado;


				// ----------------------------------------
				//
				// AttMatrizHidreletrica_vazao_defluente_minima
				//
				// ----------------------------------------

				if ((getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_defluente_minima) == 0) && (a_imprimir_atributos_sem_recarregar)) {
					preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_defluente_minima) = sim_operacional;
					if (preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_defluente_minima) == sim_operacional)
						preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_defluente_minima) = sim_premissa;
					else if (preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_defluente_minima) == nao_operacional_informado)
						preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_defluente_minima) = nao_premissa_informado;
				} // if ((getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_defluente_minima) == 0) && (hidreletrica_com_unidade)) {

				else if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_defluente_minima) > 0)
					preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_defluente_minima) = nao_operacional_informado;

				// ----------------------------------------
				//
				// AttMatrizHidreletrica_vazao_defluente_maxima
				//
				// ----------------------------------------

				if ((getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_defluente_maxima) == 0) && (a_imprimir_atributos_sem_recarregar)) {
					preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_defluente_maxima) = sim_operacional;
					if (preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_defluente_maxima) == sim_operacional)
						preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_defluente_maxima) = sim_premissa;
					else if (preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_defluente_maxima) == nao_operacional_informado)
						preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_defluente_maxima) = nao_premissa_informado;
				} // if ((getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_defluente_maxima) == 0) && (hidreletrica_com_unidade)) {

				else if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_defluente_maxima) > 0)
					preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_defluente_maxima) = nao_operacional_informado;


				// ----------------------------------------
				//
				// AttMatrizHidreletrica_vazao_turbinada_disponivel_minima
				//
				// ----------------------------------------

				if ((getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_minima) == 0) && (hidreletrica_com_unidade)) {
					if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0)
						preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_turbinada_disponivel_minima) = sim_operacional;
				} // if ((getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_disponivel_minima) == 0) && (hidreletrica_com_unidade)) {

				else if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_minima) > 0) {
					if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0)
						preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_turbinada_disponivel_minima) = nao_operacional_informado;
					else
						throw std::invalid_argument("O atributo " + getFullString(AttMatrizHidreletrica_vazao_turbinada_disponivel_minima) + " nao deve ser informado em " + getFullString(idHidreletrica));
				}

				// ----------------------------------------
				//
				// AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima
				//
				// ----------------------------------------

				if ((getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima) == 0) && (hidreletrica_com_unidade)) {
					if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0)
						preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima) = sim_operacional;
				} // if ((getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_disponivel_maxima) == 0) && (hidreletrica_com_unidade)) {

				else if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima) > 0) {
					if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0)
						preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima) = nao_operacional_informado;
					else
						throw std::invalid_argument("O atributo " + getFullString(AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima) + " nao deve ser informado em " + getFullString(idHidreletrica));
				}


				// Vazao Desviada

				if (getAtributo(idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica()) != IdHidreletrica_Nenhum) {

					std::vector<IdRestricaoOperativaUHE> vetor_idRestricaoOperativaUHE_comUHE_vazao_desviada;

					if ((getAtributo(AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_hidraulicas, bool())) && (a_imprimir_atributos_sem_recarregar)) {
						for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= getMaiorId(IdRestricaoOperativaUHE()); idRestricaoOperativaUHE++) {
							if ((getAtributo(idRestricaoOperativaUHE, IdElementoSistema_1, AttComumElementoSistema_hidreletrica, IdHidreletrica()) == idHidreletrica) && (getAtributo(idRestricaoOperativaUHE, IdElementoSistema_1, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa()) == TipoVariavelRestricaoOperativa_vazao_desviada) && (isRestricaoOperativaUHE_simples(idRestricaoOperativaUHE)))
								vetor_idRestricaoOperativaUHE_comUHE_vazao_desviada.push_back(idRestricaoOperativaUHE);
						}
					} // if (getAtributo(AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_hidraulicas, bool())) {

					// --------------------------------------
					//
					// AttMatrizHidreletrica_vazao_desviada_minima
					//
					// --------------------------------------
					// 

					if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_desviada_minima) == 0) {

						if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_desviada_minima) == 0) {


							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								for (IdPatamarCarga idPatamarCarga_aux = IdPatamarCarga_1; idPatamarCarga_aux <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga_aux++) {
									vetorHidreletrica.att(idHidreletrica).addElemento(AttMatrizHidreletrica_vazao_desviada_minima, periodo, idPatamarCarga_aux, getAtributo(idHidreletrica, AttComumHidreletrica_vazao_desviada_minima, double()));

								}//for (IdPatamarCarga idPatamarCarga_aux = IdPatamarCarga_1; idPatamarCarga_aux <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga_aux++) {

							}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						}//if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_desviada_minima) == 0) {							
						else {

							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								for (IdPatamarCarga idPatamarCarga_aux = IdPatamarCarga_1; idPatamarCarga_aux <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga_aux++)
									vetorHidreletrica.att(idHidreletrica).addElemento(AttMatrizHidreletrica_vazao_desviada_minima, periodo, idPatamarCarga_aux, getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_desviada_minima, periodo, double()));

							}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						}//else {

						preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_desviada_minima) = sim_operacional;
					}
					else if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_desviada_minima) > 0)
						preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_desviada_minima) = nao_operacional_informado;

					// --------------------------------------
					//
					// AttMatrizHidreletrica_vazao_desviada_maxima
					//
					// --------------------------------------
					// 

					if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_desviada_maxima) == 0) {

						if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_desviada_maxima) == 0) {

							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								for (IdPatamarCarga idPatamarCarga_aux = IdPatamarCarga_1; idPatamarCarga_aux <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga_aux++) {
									vetorHidreletrica.att(idHidreletrica).addElemento(AttMatrizHidreletrica_vazao_desviada_maxima, periodo, idPatamarCarga_aux, getAtributo(idHidreletrica, AttComumHidreletrica_vazao_desviada_maxima, double()));

								}//for (IdPatamarCarga idPatamarCarga_aux = IdPatamarCarga_1; idPatamarCarga_aux <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga_aux++) {

							}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						}//if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_vazao_desviada_maxima) == 0) {							
						else {

							for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

								for (IdPatamarCarga idPatamarCarga_aux = IdPatamarCarga_1; idPatamarCarga_aux <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga_aux++)
									vetorHidreletrica.att(idHidreletrica).addElemento(AttMatrizHidreletrica_vazao_desviada_maxima, periodo, idPatamarCarga_aux, getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_desviada_maxima, periodo, double()));

							}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						}//else {

						preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_desviada_maxima) = sim_operacional;
					}
					else if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_desviada_maxima) > 0)
						preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_desviada_maxima) = nao_operacional_informado;

					if (vetor_idRestricaoOperativaUHE_comUHE_vazao_desviada.size() > 0) {

						for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {

								for (int pos = 0; pos < int(vetor_idRestricaoOperativaUHE_comUHE_vazao_desviada.size()); pos++) {

									if (getElementoMatriz(vetor_idRestricaoOperativaUHE_comUHE_vazao_desviada.at(pos), AttMatrizRestricaoOperativaUHE_limite_inferior, periodo, idPatamarCarga, double()) > getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_desviada_minima, periodo, idPatamarCarga, double()))
										vetorHidreletrica.att(idHidreletrica).setElemento(AttMatrizHidreletrica_vazao_desviada_minima, periodo, idPatamarCarga, getElementoMatriz(vetor_idRestricaoOperativaUHE_comUHE_vazao_desviada.at(pos), AttMatrizRestricaoOperativaUHE_limite_inferior, periodo, idPatamarCarga, double()));

									if (getElementoMatriz(vetor_idRestricaoOperativaUHE_comUHE_vazao_desviada.at(pos), AttMatrizRestricaoOperativaUHE_limite_superior, periodo, idPatamarCarga, double()) < getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_desviada_maxima, periodo, idPatamarCarga, double()))
										vetorHidreletrica.att(idHidreletrica).setElemento(AttMatrizHidreletrica_vazao_desviada_maxima, periodo, idPatamarCarga, getElementoMatriz(vetor_idRestricaoOperativaUHE_comUHE_vazao_desviada.at(pos), AttMatrizRestricaoOperativaUHE_limite_superior, periodo, idPatamarCarga, double()));

								}//for (int pos = 0; pos < int(vetor_idRestricaoOperativaUHE_comUHE.size()); pos++) {

							} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
					} // if (vetor_idRestricaoOperativaUHE_comUHE_vazao_desviada.size() > 0) {

				} // if (getAtributo(idHidreletrica, AttComumHidreletrica_jusante_desvio, IdHidreletrica()) != IdHidreletrica_Nenhum) {

				// -----------------------------------------------
				//
				// AttMatrizHidreletrica_potencia_disponivel_meta
				//
				// ----------------------------------------------
				// 

				if ((getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_meta) > 0) && (getAtributo(AttComumDados_tipo_estudo, TipoEstudo()) == TipoEstudo_simulacao)) {
					if (getAtributo(AttComumDados_numero_cenarios, int()) != getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_meta))
						throw std::invalid_argument("Numero de cenarios em " + getFullString(AttMatrizHidreletrica_potencia_disponivel_meta) + " diferente de " + getFullString(AttComumDados_numero_cenarios));
					//preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_disponivel_meta) = nao_operacional_informado;
				}
				else if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_meta) > 0)
					throw std::invalid_argument("Atributo " + getFullString(AttMatrizHidreletrica_potencia_disponivel_meta) + " em " + getFullString(idHidreletrica) + " nao compativel com " + getFullString(getAtributo(AttComumDados_tipo_estudo, TipoEstudo())));


				// -----------------------------------------------------
				//
				// AttMatrizHidreletrica_vazao_turbinada_disponivel_meta
				//
				// -----------------------------------------------------
				// 

				if ((getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_meta) > 0) && (getAtributo(AttComumDados_tipo_estudo, TipoEstudo()) == TipoEstudo_simulacao)) {
					if (getAtributo(AttComumDados_numero_cenarios, int()) != getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_meta))
						throw std::invalid_argument("Numero de cenarios em " + getFullString(AttMatrizHidreletrica_vazao_turbinada_disponivel_meta) + " diferente de " + getFullString(AttComumDados_numero_cenarios));
					if (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_disponivel_meta) == nao_operacional_informado)
						throw std::invalid_argument("Atributo " + getFullString(AttMatrizHidreletrica_vazao_turbinada_disponivel_meta) + " em " + getFullString(idHidreletrica) + " nao compativel com " + getFullString(AttMatrizHidreletrica_potencia_disponivel_meta));
					//preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_turbinada_disponivel_meta) = nao_operacional_informado;
				}
				else if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_meta) > 0)
					throw std::invalid_argument("Atributo " + getFullString(AttMatrizHidreletrica_vazao_turbinada_disponivel_meta) + " em " + getFullString(idHidreletrica) + " nao compativel com " + getFullString(getAtributo(AttComumDados_tipo_estudo, TipoEstudo())));


				// -----------------------------------------------
				//
				// AttMatrizHidreletrica_vazao_vertida_meta
				//
				// ----------------------------------------------
				// 

				if ((getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_vertida_meta) > 0) && (getAtributo(AttComumDados_tipo_estudo, TipoEstudo()) == TipoEstudo_simulacao)) {
					if (getAtributo(AttComumDados_numero_cenarios, int()) != getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_vertida_meta))
						throw std::invalid_argument("Numero de cenarios em " + getFullString(AttMatrizHidreletrica_vazao_vertida_meta) + " diferente de " + getFullString(AttComumDados_numero_cenarios));
					preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_vertida_meta) = nao_operacional_informado;
				}
				else if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_vertida_meta) > 0)
					throw std::invalid_argument("Atributo " + getFullString(AttMatrizHidreletrica_vazao_vertida_meta) + " em " + getFullString(idHidreletrica) + " nao compativel com " + getFullString(getAtributo(AttComumDados_tipo_estudo, TipoEstudo())));

				// -----------------------------------------------
				//
				// AttMatrizHidreletrica_vazao_evaporada_meta
				//
				// ----------------------------------------------
				// 

				if ((getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_evaporada_meta) > 0) && (getAtributo(AttComumDados_tipo_estudo, TipoEstudo()) == TipoEstudo_simulacao)) {
					if (getAtributo(AttComumDados_numero_cenarios, int()) != getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_evaporada_meta))
						throw std::invalid_argument("Numero de cenarios em " + getFullString(AttMatrizHidreletrica_vazao_evaporada_meta) + " diferente de " + getFullString(AttComumDados_numero_cenarios));
				}
				else if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_evaporada_meta) > 0)
					throw std::invalid_argument("Atributo " + getFullString(AttMatrizHidreletrica_vazao_evaporada_meta) + " em " + getFullString(idHidreletrica) + " nao compativel com " + getFullString(getAtributo(AttComumDados_tipo_estudo, TipoEstudo())));



				const IdConjuntoHidraulico maiorIdConjuntoHidraulico = getMaiorId(idHidreletrica, IdConjuntoHidraulico());

				preencher_AttVetorFuncaoProducaoHidreletrica.at(idHidreletrica) = SmartEnupla<IdConjuntoHidraulico, SmartEnupla<IdUnidadeUHE, SmartEnupla<AttVetorFuncaoProducaoHidreletrica, PreencherAtributo>>>(true, IdConjuntoHidraulico_Nenhum, std::vector<SmartEnupla<IdUnidadeUHE, SmartEnupla<AttVetorFuncaoProducaoHidreletrica, PreencherAtributo>>>(int(maiorIdConjuntoHidraulico) + 1, SmartEnupla<IdUnidadeUHE, SmartEnupla<AttVetorFuncaoProducaoHidreletrica, PreencherAtributo>>()));
				preencher_AttMatrizFuncaoProducaoHidreletrica.at(idHidreletrica) = SmartEnupla<IdConjuntoHidraulico, SmartEnupla<IdUnidadeUHE, SmartEnupla<AttMatrizFuncaoProducaoHidreletrica, PreencherAtributo>>>(true, IdConjuntoHidraulico_Nenhum, std::vector<SmartEnupla<IdUnidadeUHE, SmartEnupla<AttMatrizFuncaoProducaoHidreletrica, PreencherAtributo>>>(int(maiorIdConjuntoHidraulico) + 1, SmartEnupla<IdUnidadeUHE, SmartEnupla<AttMatrizFuncaoProducaoHidreletrica, PreencherAtributo>>()));

				preencher_AttVetorFuncaoProducaoHidreletrica.at(idHidreletrica).at(IdConjuntoHidraulico_Nenhum) = SmartEnupla<IdUnidadeUHE, SmartEnupla<AttVetorFuncaoProducaoHidreletrica, PreencherAtributo>>(true, IdUnidadeUHE_Nenhum, std::vector<SmartEnupla<AttVetorFuncaoProducaoHidreletrica, PreencherAtributo>>(1, SmartEnupla<AttVetorFuncaoProducaoHidreletrica, PreencherAtributo>()));
				preencher_AttMatrizFuncaoProducaoHidreletrica.at(idHidreletrica).at(IdConjuntoHidraulico_Nenhum) = SmartEnupla<IdUnidadeUHE, SmartEnupla<AttMatrizFuncaoProducaoHidreletrica, PreencherAtributo>>(true, IdUnidadeUHE_Nenhum, std::vector<SmartEnupla<AttMatrizFuncaoProducaoHidreletrica, PreencherAtributo>>(1, SmartEnupla<AttMatrizFuncaoProducaoHidreletrica, PreencherAtributo>()));


				// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				//
				//                                                                                   ConjuntoHidraulico
				//
				// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

				if (maiorIdConjuntoHidraulico == IdConjuntoHidraulico_Nenhum) {

					if ((vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto).size() > 0) || (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0))
						throw std::invalid_argument("Para " + getFullString(TipoDetalhamentoProducaoHidreletrica_por_conjunto) + " ou " + getFullString(TipoDetalhamentoProducaoHidreletrica_por_unidade) + " deve haver ao menos um conjunto hidraulico.");

				} // if (maiorIdConjuntoHidraulico == IdConjuntoHidraulico_Nenhum) {

				else if (maiorIdConjuntoHidraulico != IdConjuntoHidraulico_Nenhum) {

					preencher_AttVetorConjuntoHidraulico.at(idHidreletrica) = SmartEnupla<IdConjuntoHidraulico, SmartEnupla<AttVetorConjuntoHidraulico, PreencherAtributo>>(IdConjuntoHidraulico_1, std::vector<SmartEnupla<AttVetorConjuntoHidraulico, PreencherAtributo>>(maiorIdConjuntoHidraulico, SmartEnupla<AttVetorConjuntoHidraulico, PreencherAtributo>()));
					preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica) = SmartEnupla<IdConjuntoHidraulico, SmartEnupla<AttMatrizConjuntoHidraulico, PreencherAtributo>>(IdConjuntoHidraulico_1, std::vector<SmartEnupla<AttMatrizConjuntoHidraulico, PreencherAtributo>>(maiorIdConjuntoHidraulico, SmartEnupla<AttMatrizConjuntoHidraulico, PreencherAtributo>()));

					preencher_AttVetorUnidadeUHE.at(idHidreletrica) = SmartEnupla<IdConjuntoHidraulico, SmartEnupla<IdUnidadeUHE, SmartEnupla<AttVetorUnidadeUHE, PreencherAtributo>>>(IdConjuntoHidraulico_1, std::vector<SmartEnupla<IdUnidadeUHE, SmartEnupla<AttVetorUnidadeUHE, PreencherAtributo>>>(maiorIdConjuntoHidraulico, SmartEnupla<IdUnidadeUHE, SmartEnupla<AttVetorUnidadeUHE, PreencherAtributo>>()));
					preencher_AttMatrizUnidadeUHE.at(idHidreletrica) = SmartEnupla<IdConjuntoHidraulico, SmartEnupla<IdUnidadeUHE, SmartEnupla<AttMatrizUnidadeUHE, PreencherAtributo>>>(IdConjuntoHidraulico_1, std::vector<SmartEnupla<IdUnidadeUHE, SmartEnupla<AttMatrizUnidadeUHE, PreencherAtributo>>>(maiorIdConjuntoHidraulico, SmartEnupla<IdUnidadeUHE, SmartEnupla<AttMatrizUnidadeUHE, PreencherAtributo>>()));

				} // else if (maiorIdConjuntoHidraulico != IdConjuntoHidraulico_Nenhum) {

				for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {

					const IdUnidadeUHE maiorIdUnidadeUHE = getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE());

					bool conjunto_com_unidade = false;

					if (maiorIdUnidadeUHE != IdUnidadeUHE_Nenhum)
						conjunto_com_unidade = true;

					preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).setElemento(idConjuntoHidraulico, SmartEnupla<AttVetorConjuntoHidraulico, PreencherAtributo>(AttVetorConjuntoHidraulico(AttVetorConjuntoHidraulico_Nenhum + 1), std::vector<PreencherAtributo>(AttVetorConjuntoHidraulico(AttVetorConjuntoHidraulico_Excedente - 1), nao_sem_utilizacao)));
					preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).setElemento(idConjuntoHidraulico, SmartEnupla<AttMatrizConjuntoHidraulico, PreencherAtributo>(AttMatrizConjuntoHidraulico(AttMatrizConjuntoHidraulico_Nenhum + 1), std::vector<PreencherAtributo>(AttMatrizConjuntoHidraulico(AttMatrizConjuntoHidraulico_Excedente - 1), nao_sem_utilizacao)));

					preencher_AttVetorFuncaoProducaoHidreletrica.at(idHidreletrica).at(idConjuntoHidraulico) = SmartEnupla<IdUnidadeUHE, SmartEnupla<AttVetorFuncaoProducaoHidreletrica, PreencherAtributo>>(true, IdUnidadeUHE_Nenhum, std::vector<SmartEnupla<AttVetorFuncaoProducaoHidreletrica, PreencherAtributo>>(int(maiorIdUnidadeUHE) + 1, SmartEnupla<AttVetorFuncaoProducaoHidreletrica, PreencherAtributo>()));
					preencher_AttMatrizFuncaoProducaoHidreletrica.at(idHidreletrica).at(idConjuntoHidraulico) = SmartEnupla<IdUnidadeUHE, SmartEnupla<AttMatrizFuncaoProducaoHidreletrica, PreencherAtributo>>(true, IdUnidadeUHE_Nenhum, std::vector<SmartEnupla<AttMatrizFuncaoProducaoHidreletrica, PreencherAtributo>>(int(maiorIdUnidadeUHE) + 1, SmartEnupla<AttMatrizFuncaoProducaoHidreletrica, PreencherAtributo>()));


					// ------------------------------------------
					//
					// AttVetorConjuntoHidraulico_disponibilidade
					//
					// ------------------------------------------

					SmartEnupla<Periodo, double> disponibilidade_conjunto;

					if ((getSizeVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_disponibilidade) == 0) && (conjunto_com_unidade)) {

						if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto).size() > 0) {
							preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_disponibilidade) = sim_operacional;
							disponibilidade_conjunto = vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto);
						}

						else if ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_disponibilidade) == sim_premissa) || (preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_disponibilidade) == sim_operacional)) {
							preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_disponibilidade) = sim_premissa;
							disponibilidade_conjunto = SmartEnupla<Periodo, double>(disponibilidade_hidreletrica, 0.0);
						}

					} // if ((getSizeVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_disponibilidade) == 0) && (conjunto_com_unidade)) {

					else if (getSizeVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_disponibilidade) > 0) {

						if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto).size() > 0)
							preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_disponibilidade) = nao_operacional_informado;

						else if ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_disponibilidade) == sim_premissa) || (preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_disponibilidade) == sim_operacional))
							preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_disponibilidade) = nao_premissa_informado;

						else
							throw std::invalid_argument("O atributo " + getFullString(AttVetorConjuntoHidraulico_disponibilidade) + " nao deve ser informado em " + getFullString(idHidreletrica) + " " + getFullString(idConjuntoHidraulico));

					} // else if (getSizeVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_disponibilidade) > 0) {


					// -------------------------------------------
					//
					// AttMatrizConjuntoHidraulico_potencia_disponivel_minima
					//
					// -------------------------------------------

					if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_minima) > 0) {

						if ((vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto).size() > 0) || (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0))
							preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_disponivel_minima) = nao_operacional_informado;

						else
							throw std::invalid_argument("O atributo " + getFullString(AttMatrizConjuntoHidraulico_potencia_disponivel_minima) + " nao deve ser informado em " + getFullString(idHidreletrica) + " " + getFullString(idConjuntoHidraulico));

					} // if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_minima) > 0) {


					// -------------------------------------------
					//
					// AttMatrizConjuntoHidraulico_potencia_disponivel_maxima
					//
					// -------------------------------------------


					if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_maxima) > 0) {

						if ((vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto).size() > 0) || (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0))
							preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_disponivel_maxima) = nao_operacional_informado;

						else
							throw std::invalid_argument("O atributo " + getFullString(AttMatrizConjuntoHidraulico_potencia_disponivel_maxima) + " nao deve ser informado em " + getFullString(idHidreletrica) + " " + getFullString(idConjuntoHidraulico));

					} //  if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_disponivel_maxima) > 0) {


					// -------------------------------------------
					//
					// AttMatrizConjuntoHidraulico_potencia_minima
					//
					// -------------------------------------------


					SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> potencia_minima_conjunto;

					if ((getSize1Matriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima) == 0) && (conjunto_com_unidade)) {

						if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto).size() > 0) {
							preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_minima) = sim_operacional;
							potencia_minima_conjunto = matriz_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto);
						}

						else if ((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_minima) == sim_premissa) || (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_minima) == sim_operacional)) {
							preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_minima) = sim_premissa;
							potencia_minima_conjunto = SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(potencia_minima_hidreletrica, SmartEnupla<IdPatamarCarga, double>());
							for (Periodo periodo = potencia_minima_conjunto.getIteradorInicial(); periodo <= potencia_minima_conjunto.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
								potencia_minima_conjunto.at(periodo) = SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()), 0.0));
						}

					} // if ((getSize1Matriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima) == 0) && (conjunto_com_unidade)) {

					else if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima) > 0) {

						if ((vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto).size() > 0) || (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0))
							preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_minima) = nao_operacional_informado;
						else
							throw std::invalid_argument("O atributo " + getFullString(AttMatrizConjuntoHidraulico_potencia_minima) + " nao deve ser informado em " + getFullString(idHidreletrica) + " " + getFullString(idConjuntoHidraulico));

					} // else if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima) > 0) {


					// -------------------------------------------
					//
					// AttMatrizConjuntoHidraulico_potencia_maxima
					//
					// -------------------------------------------

					SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> potencia_maxima_conjunto;

					if ((getSize1Matriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_maxima) == 0) && (conjunto_com_unidade)) {

						if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto).size() > 0) {
							preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_maxima) = sim_operacional;
							potencia_maxima_conjunto = matriz_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto);
						}

						else if ((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_maxima) == sim_premissa) || (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_maxima) == sim_operacional)) {
							preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_maxima) = sim_premissa;
							potencia_maxima_conjunto = SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(potencia_maxima_hidreletrica, SmartEnupla<IdPatamarCarga, double>());
							for (Periodo periodo = potencia_maxima_conjunto.getIteradorInicial(); periodo <= potencia_maxima_conjunto.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
								potencia_maxima_conjunto.at(periodo) = SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()), 0.0));
						}

					} // if ((getSize1Matriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_maxima) == 0) && (conjunto_com_unidade)) {

					else if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_maxima) > 0) {

						if ((vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto).size() > 0) || (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0))
							preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_maxima) = nao_operacional_informado;

						else
							throw std::invalid_argument("O atributo " + getFullString(AttMatrizConjuntoHidraulico_potencia_maxima) + " nao deve ser informado em " + getFullString(idHidreletrica) + " " + getFullString(idConjuntoHidraulico));

					} // else if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_maxima) > 0) {


					// ----------------------------------------------
					//
					// AttVetorConjuntoHidraulico_vazao_turbinada_minima
					//
					// ----------------------------------------------

					SmartEnupla<Periodo, double> vazao_turbinada_minima_conjunto;

					if ((getSizeVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_minima) == 0) && (conjunto_com_unidade)) {

						if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto).size() > 0) {
							preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_minima) = sim_operacional;
							vazao_turbinada_minima_conjunto = vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto);
						}

						else if ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_turbinada_minima) == sim_premissa) || (preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_turbinada_minima) == sim_operacional)) {
							preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_minima) = sim_premissa;
							vazao_turbinada_minima_conjunto = SmartEnupla<Periodo, double>(vazao_turbinada_minima_hidreletrica, 0.0);
						}

					} // if ((getSizeVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_minima) == 0) && (conjunto_com_unidade)) {

					else if (getSizeVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_minima) > 0) {

						if ((vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto).size() > 0) || (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0))
							preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_minima) = nao_operacional_informado;

						else
							throw std::invalid_argument("O atributo " + getFullString(AttVetorConjuntoHidraulico_vazao_turbinada_minima) + " nao deve ser informado em " + getFullString(idHidreletrica) + " " + getFullString(idConjuntoHidraulico));

					} // else if (getSizeVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_minima) > 0) {


					// ----------------------------------------------
					//
					// AttVetorConjuntoHidraulico_vazao_turbinada_maxima
					//
					// ----------------------------------------------

					SmartEnupla<Periodo, double> vazao_turbinada_maxima_conjunto;

					if ((getSizeVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_maxima) == 0) && (conjunto_com_unidade)) {

						if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto).size() > 0) {
							preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_maxima) = sim_operacional;
							vazao_turbinada_maxima_conjunto = vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto);
						}

						else if ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_turbinada_maxima) == sim_premissa) || (preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_turbinada_maxima) == sim_operacional)) {
							preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_maxima) = sim_premissa;
							vazao_turbinada_maxima_conjunto = SmartEnupla<Periodo, double>(vazao_turbinada_maxima_hidreletrica, 0.0);
						}

					} // if ((getSizeVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_maxima) == 0) && (conjunto_com_unidade)) {

					else if (getSizeVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_maxima) > 0) {

						if ((vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto).size() > 0) || (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0))
							preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_maxima) = nao_operacional_informado;

						else
							throw std::invalid_argument("O atributo " + getFullString(AttVetorConjuntoHidraulico_vazao_turbinada_maxima) + " nao deve ser informado em " + getFullString(idHidreletrica) + " " + getFullString(idConjuntoHidraulico));

					} // else if (getSizeVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_maxima) > 0) {


					// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
					//
					//                                                                                            UnidadeUHE
					//
					// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


					if (maiorIdUnidadeUHE == IdUnidadeUHE_Nenhum) {

						if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0)
							throw std::invalid_argument("Para " + getFullString(TipoDetalhamentoProducaoHidreletrica_por_unidade) + " deve haver ao menos uma unidade hidreletrica.");

					} // if (maiorIdUnidadeUHE == IdUnidadeUHE_Nenhum) {

					else if (maiorIdUnidadeUHE != IdUnidadeUHE_Nenhum) {

						preencher_AttVetorUnidadeUHE.at(idHidreletrica).setElemento(idConjuntoHidraulico, SmartEnupla<IdUnidadeUHE, SmartEnupla<AttVetorUnidadeUHE, PreencherAtributo>>(IdUnidadeUHE_1, std::vector<SmartEnupla<AttVetorUnidadeUHE, PreencherAtributo>>(maiorIdUnidadeUHE, SmartEnupla<AttVetorUnidadeUHE, PreencherAtributo>())));
						preencher_AttMatrizUnidadeUHE.at(idHidreletrica).setElemento(idConjuntoHidraulico, SmartEnupla<IdUnidadeUHE, SmartEnupla<AttMatrizUnidadeUHE, PreencherAtributo>>(IdUnidadeUHE_1, std::vector<SmartEnupla<AttMatrizUnidadeUHE, PreencherAtributo>>(maiorIdUnidadeUHE, SmartEnupla<AttMatrizUnidadeUHE, PreencherAtributo>())));

					} // else if (maiorIdUnidadeUHE != IdUnidadeUHE_Nenhum) {

					for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= maiorIdUnidadeUHE; idUnidadeUHE++) {

						preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).setElemento(idUnidadeUHE, SmartEnupla<AttVetorUnidadeUHE, PreencherAtributo>(AttVetorUnidadeUHE(AttVetorUnidadeUHE_Nenhum + 1), std::vector<PreencherAtributo>(AttVetorUnidadeUHE(AttVetorUnidadeUHE_Excedente - 1), nao_sem_utilizacao)));
						preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).setElemento(idUnidadeUHE, SmartEnupla<AttMatrizUnidadeUHE, PreencherAtributo>(AttMatrizUnidadeUHE(AttMatrizUnidadeUHE_Nenhum + 1), std::vector<PreencherAtributo>(AttMatrizUnidadeUHE(AttMatrizUnidadeUHE_Excedente - 1), nao_sem_utilizacao)));


						// ----------------------------------
						//
						// AttVetorUnidadeUHE_disponibilidade
						//
						// ----------------------------------

						if (getSizeVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_disponibilidade) == 0) {

							SmartEnupla<Periodo, double> disponibilidade_unidade;

							if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0) {
								preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_disponibilidade) = sim_operacional;
								disponibilidade_unidade = vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade);
							}

							else if ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_disponibilidade) == sim_premissa) || (preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_disponibilidade) == sim_operacional)) {
								preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_disponibilidade) = sim_premissa;
								disponibilidade_unidade = SmartEnupla<Periodo, double>(disponibilidade_conjunto, 0.0);
							}

							else if ((preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_minima) == sim_premissa) || (preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_minima) == sim_operacional)) {
								preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttMatrizUnidadeUHE_potencia_minima) = sim_premissa;
								for (Periodo periodo = potencia_minima_conjunto.getIteradorInicial(); periodo <= potencia_minima_conjunto.getIteradorFinal(); potencia_minima_conjunto.incrementarIterador(periodo))
									disponibilidade_unidade.addElemento(periodo, 0.0);
							}

							else if ((preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_maxima) == sim_premissa) || (preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_maxima) == sim_operacional)) {
								preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttMatrizUnidadeUHE_potencia_maxima) = sim_premissa;
								for (Periodo periodo = potencia_maxima_conjunto.getIteradorInicial(); periodo <= potencia_maxima_conjunto.getIteradorFinal(); potencia_maxima_conjunto.incrementarIterador(periodo))
									disponibilidade_unidade.addElemento(periodo, 0.0);
							}

							if ((preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_disponibilidade) == sim_premissa) || (preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_disponibilidade) == sim_operacional)) {

								// AttVetorHidreletrica_indisponibilidade_forcada

								if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada) == 0) {
									vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_forcada, SmartEnupla<Periodo, double>(disponibilidade_unidade, getAtributo(idHidreletrica, AttComumHidreletrica_indisponibilidade_forcada, double())));
									preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_indisponibilidade_forcada) = sim_premissa;
								}
								else
									preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_indisponibilidade_forcada) = nao_premissa_informado;

								// AttVetorHidreletrica_indisponibilidade_programada

								if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada) == 0) {
									vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_indisponibilidade_programada, SmartEnupla<Periodo, double>(disponibilidade_unidade, getAtributo(idHidreletrica, AttComumHidreletrica_indisponibilidade_programada, double())));
									preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_indisponibilidade_programada) = sim_premissa;
								}
								else
									preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_indisponibilidade_programada) = nao_premissa_informado;

								for (Periodo periodo = disponibilidade_unidade.getIteradorInicial(); periodo <= disponibilidade_unidade.getIteradorFinal(); disponibilidade_unidade.incrementarIterador(periodo)) {

									const double indisponibilidade_forcada = getElementoVetor(idHidreletrica, AttVetorHidreletrica_indisponibilidade_forcada, periodo, double());
									const double indisponibilidade_programada = getElementoVetor(idHidreletrica, AttVetorHidreletrica_indisponibilidade_programada, periodo, double());
									const double disponibilidade = (1.0 - indisponibilidade_forcada) * (1.0 - indisponibilidade_programada);

									vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).addElemento(AttVetorUnidadeUHE_disponibilidade, periodo, disponibilidade);

								} // for (Periodo periodo = disponibilidade_unidade.getIteradorInicial(); periodo <= disponibilidade_unidade.getIteradorFinal(); disponibilidade_unidade.incrementarIterador(periodo)) {

							} // if ((preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_disponibilidade) == sim_premissa) || (preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_disponibilidade) == sim_operacional)) {

						} // if (getSizeVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_disponibilidade) == 0) {

						else if (getSizeVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_disponibilidade) > 0) {

							if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0)
								preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_disponibilidade) = nao_operacional_informado;

							else if ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_disponibilidade) == sim_premissa) || (preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_disponibilidade) == sim_operacional))
								preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_disponibilidade) = nao_premissa_informado;

							else if ((preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_minima) == sim_premissa) || (preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_minima) == sim_operacional))
								preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_disponibilidade) = nao_premissa_informado;

							else if ((preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_maxima) == sim_premissa) || (preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_maxima) == sim_operacional))
								preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_disponibilidade) = nao_premissa_informado;

							else
								throw std::invalid_argument("O atributo " + getFullString(AttVetorUnidadeUHE_disponibilidade) + " nao deve ser informado em " + getFullString(idHidreletrica) + " " + getFullString(idConjuntoHidraulico) + " " + getFullString(idUnidadeUHE));

						} // else if (getSizeVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_disponibilidade) > 0) {

						// -----------------------------------
						//
						// AttMatrizUnidadeUHE_potencia_disponivel_minima
						//
						// -----------------------------------

						if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_minima) > 0) {

							if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0)
								preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttMatrizUnidadeUHE_potencia_disponivel_minima) = nao_operacional_informado;

							else
								throw std::invalid_argument("O atributo " + getFullString(AttMatrizUnidadeUHE_potencia_disponivel_minima) + " nao deve ser informado em " + getFullString(idHidreletrica) + " " + getFullString(idConjuntoHidraulico) + " " + getFullString(idUnidadeUHE));

						} // else if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_minima) > 0) {


						// -----------------------------------
						//
						// AttMatrizUnidadeUHE_potencia_disponivel_maxima
						//
						// -----------------------------------

						if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_maxima) > 0) {

							if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0)
								preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttMatrizUnidadeUHE_potencia_disponivel_maxima) = nao_operacional_informado;

							else
								throw std::invalid_argument("O atributo " + getFullString(AttMatrizUnidadeUHE_potencia_disponivel_maxima) + " nao deve ser informado em " + getFullString(idHidreletrica) + " " + getFullString(idConjuntoHidraulico) + " " + getFullString(idUnidadeUHE));

						} // else if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_disponivel_maxima) > 0) {


						// -----------------------------------
						//
						// AttMatrizUnidadeUHE_potencia_minima
						//
						// -----------------------------------

						if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_minima) == 0) {

							SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> potencia_minima_unidade;

							if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0) {
								preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttMatrizUnidadeUHE_potencia_minima) = sim_operacional;
								potencia_minima_unidade = matriz_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade);
							}

							else if ((preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_minima) == sim_premissa) || (preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_minima) == sim_operacional)) {
								preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttMatrizUnidadeUHE_potencia_minima) = sim_premissa;
								potencia_minima_unidade = SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(potencia_minima_conjunto, SmartEnupla<IdPatamarCarga, double>());
								for (Periodo periodo = potencia_minima_unidade.getIteradorInicial(); periodo <= potencia_minima_unidade.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
									potencia_minima_unidade.at(periodo) = SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()), 0.0));
							}

							if ((preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttMatrizUnidadeUHE_potencia_minima) == sim_premissa) || (preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttMatrizUnidadeUHE_potencia_minima) == sim_operacional)) {

								for (Periodo periodo = potencia_minima_unidade.getIteradorInicial(); periodo <= potencia_minima_unidade.getIteradorFinal(); potencia_minima_unidade.incrementarIterador(periodo)) {

									for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {

										if (getElementoVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_disponibilidade, periodo, double()) > 0.0)
											vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).addElemento(AttMatrizUnidadeUHE_potencia_minima, periodo, idPatamarCarga, getAtributo(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttComumUnidadeUHE_potencia_minima, double()));
										else
											vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).addElemento(AttMatrizUnidadeUHE_potencia_minima, periodo, idPatamarCarga, 0.0);

									} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {

								} // for (Periodo periodo = potencia_minima_unidade.getIteradorInicial(); periodo <= potencia_minima_unidade.getIteradorFinal(); potencia_minima_unidade.incrementarIterador(periodo)) {

							} // if ((preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttMatrizUnidadeUHE_potencia_minima) == sim_premissa) || (preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttMatrizUnidadeUHE_potencia_minima) == sim_operacional)) {

						} // if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_minima) == 0) {

						else if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_minima) > 0) {

							if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0)
								preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttMatrizUnidadeUHE_potencia_minima) = nao_operacional_informado;

							else if ((preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_minima) == sim_premissa) || (preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_minima) == sim_operacional))
								preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttMatrizUnidadeUHE_potencia_minima) = nao_premissa_informado;

							else
								throw std::invalid_argument("O atributo " + getFullString(AttMatrizUnidadeUHE_potencia_minima) + " nao deve ser informado em " + getFullString(idHidreletrica) + " " + getFullString(idConjuntoHidraulico) + " " + getFullString(idUnidadeUHE));

						} // else if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_minima) > 0) {

						if ((preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_minima) == sim_premissa) || (preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_minima) == sim_operacional)) {
							for (Periodo periodo = potencia_minima_conjunto.getIteradorInicial(); periodo <= potencia_minima_conjunto.getIteradorFinal(); potencia_minima_conjunto.incrementarIterador(periodo)) {
								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++)
									potencia_minima_conjunto.at(periodo).at(idPatamarCarga) += getElementoMatriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_minima, periodo, idPatamarCarga, double());
							}
						} // if ((preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_minima) == sim_premissa) || (preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_minima) == sim_operacional)) {


						// -----------------------------------
						//
						// AttMatrizUnidadeUHE_potencia_maxima
						//
						// -----------------------------------

						if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_maxima) == 0) {

							SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> potencia_maxima_unidade;

							if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0) {
								preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttMatrizUnidadeUHE_potencia_maxima) = sim_operacional;
								potencia_maxima_unidade = matriz_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade);
							}

							else if ((preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_maxima) == sim_premissa) || (preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_maxima) == sim_operacional)) {
								preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttMatrizUnidadeUHE_potencia_maxima) = sim_premissa;
								potencia_maxima_unidade = SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>>(potencia_maxima_conjunto, SmartEnupla<IdPatamarCarga, double>());
								for (Periodo periodo = potencia_maxima_unidade.getIteradorInicial(); periodo <= potencia_maxima_unidade.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
									potencia_maxima_unidade.at(periodo) = SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()), 0.0));

							}

							else if ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_disponibilidade) == sim_premissa) || (preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_disponibilidade) == sim_operacional)) {
								preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttMatrizUnidadeUHE_potencia_maxima) = sim_premissa;
								for (Periodo periodo = disponibilidade_conjunto.getIteradorInicial(); periodo <= disponibilidade_conjunto.getIteradorFinal(); disponibilidade_conjunto.incrementarIterador(periodo))
									potencia_maxima_unidade.addElemento(periodo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(1, 0.0)));
							}

							if ((preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttMatrizUnidadeUHE_potencia_maxima) == sim_premissa) || (preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttMatrizUnidadeUHE_potencia_maxima) == sim_operacional)) {

								for (Periodo periodo = potencia_maxima_unidade.getIteradorInicial(); periodo <= potencia_maxima_unidade.getIteradorFinal(); potencia_maxima_unidade.incrementarIterador(periodo)) {

									for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {

										if (getElementoVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_disponibilidade, periodo, double()) > 0.0)
											vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).addElemento(AttMatrizUnidadeUHE_potencia_maxima, periodo, idPatamarCarga, getAtributo(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttComumUnidadeUHE_potencia_maxima, double()));
										else
											vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).addElemento(AttMatrizUnidadeUHE_potencia_maxima, periodo, idPatamarCarga, 0.0);

									} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {

								} // for (Periodo periodo = potencia_maxima_unidade.getIteradorInicial(); periodo <= potencia_maxima_unidade.getIteradorFinal(); potencia_maxima_unidade.incrementarIterador(periodo)) {

							} // if ((preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttMatrizUnidadeUHE_potencia_maxima) == sim_premissa) || (preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttMatrizUnidadeUHE_potencia_maxima) == sim_operacional)) {

						} // if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_maxima) == 0) {

						else if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_maxima) > 0) {

							if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0)
								preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttMatrizUnidadeUHE_potencia_maxima) = nao_operacional_informado;

							else if ((preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_maxima) == sim_premissa) || (preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_maxima) == sim_operacional))
								preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttMatrizUnidadeUHE_potencia_maxima) = nao_premissa_informado;

							else if ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_disponibilidade) == sim_premissa) || (preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_disponibilidade) == sim_operacional))
								preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttMatrizUnidadeUHE_potencia_maxima) = nao_premissa_informado;

							else
								throw std::invalid_argument("O atributo " + getFullString(AttMatrizUnidadeUHE_potencia_maxima) + " nao deve ser informado em " + getFullString(idHidreletrica) + " " + getFullString(idConjuntoHidraulico) + " " + getFullString(idUnidadeUHE));

						} // else if (getSize1Matriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_maxima) > 0) {

						if ((preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_maxima) == sim_premissa) || (preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_maxima) == sim_operacional)) {
							for (Periodo periodo = potencia_maxima_conjunto.getIteradorInicial(); periodo <= potencia_maxima_conjunto.getIteradorFinal(); potencia_maxima_conjunto.incrementarIterador(periodo)) {
								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++)
									potencia_maxima_conjunto.at(periodo).at(idPatamarCarga) += getElementoMatriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_maxima, periodo, idPatamarCarga, double());
							}
						} // if ((preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_maxima) == sim_premissa) || (preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_maxima) == sim_operacional)) {


						// --------------------------------------
						//
						// AttVetorUnidadeUHE_vazao_turbinada_minima
						//
						// --------------------------------------

						if (getSizeVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_vazao_turbinada_minima) == 0) {

							SmartEnupla<Periodo, double> vazao_turbinada_minima_unidade;

							if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0) {
								preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_vazao_turbinada_minima) = sim_operacional;
								vazao_turbinada_minima_unidade = vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade);
							}

							else if ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_minima) == sim_premissa) || (preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_minima) == sim_operacional)) {
								preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_vazao_turbinada_minima) = sim_premissa;
								vazao_turbinada_minima_unidade = SmartEnupla<Periodo, double>(vazao_turbinada_minima_conjunto, 0.0);
							}

							if ((preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_vazao_turbinada_minima) == sim_operacional) || (preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_vazao_turbinada_minima) == sim_premissa)) {

								for (Periodo periodo = vazao_turbinada_minima_unidade.getIteradorInicial(); periodo <= vazao_turbinada_minima_unidade.getIteradorFinal(); vazao_turbinada_minima_unidade.incrementarIterador(periodo)) {

									if (getElementoVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_disponibilidade, periodo, double()) > 0.0)
										vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).addElemento(AttVetorUnidadeUHE_vazao_turbinada_minima, periodo, getAtributo(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttComumUnidadeUHE_vazao_turbinada_minima, double()));
									else
										vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).addElemento(AttVetorUnidadeUHE_vazao_turbinada_minima, periodo, 0.0);

								} // for (Periodo periodo = vazao_turbinada_minima_unidade.getIteradorInicial(); periodo <= vazao_turbinada_minima_unidade.getIteradorFinal(); vazao_turbinada_minima_unidade.incrementarIterador(periodo)) {

							} // if ((preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_vazao_turbinada_minima) == sim_operacional) || (preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_vazao_turbinada_minima) == sim_premissa)) {

						} // if (getSizeVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_vazao_turbinada_minima) == 0) {

						if (getSizeVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_vazao_turbinada_minima) > 0) {

							if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0)
								preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_vazao_turbinada_minima) = nao_operacional_informado;

							else if ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_minima) == sim_premissa) || (preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_minima) == sim_operacional))
								preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_vazao_turbinada_minima) = nao_premissa_informado;

							else
								throw std::invalid_argument("O atributo " + getFullString(AttVetorUnidadeUHE_vazao_turbinada_minima) + " nao deve ser informado em " + getFullString(idHidreletrica) + " " + getFullString(idConjuntoHidraulico) + " " + getFullString(idUnidadeUHE));

						} // if (getSizeVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_vazao_turbinada_minima) > 0) {


						if ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_minima) == sim_premissa) || (preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_minima) == sim_operacional)) {
							for (Periodo periodo = vazao_turbinada_minima_conjunto.getIteradorInicial(); periodo <= vazao_turbinada_minima_conjunto.getIteradorFinal(); vazao_turbinada_minima_conjunto.incrementarIterador(periodo))
								vazao_turbinada_minima_conjunto.at(periodo) += getElementoVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_vazao_turbinada_minima, periodo, double());
						} // if ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_minima) == sim_premissa) || (preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_minima) == sim_operacional)){


						// --------------------------------------
						//
						// AttVetorUnidadeUHE_vazao_turbinada_maxima
						//
						// --------------------------------------

						if (getSizeVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_vazao_turbinada_maxima) == 0) {

							SmartEnupla<Periodo, double> vazao_turbinada_maxima_unidade;

							if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0) {
								preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_vazao_turbinada_maxima) = sim_operacional;
								vazao_turbinada_maxima_unidade = vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade);
							}

							else if ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_maxima) == sim_premissa) || (preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_maxima) == sim_operacional)) {
								preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_vazao_turbinada_maxima) = sim_premissa;
								vazao_turbinada_maxima_unidade = SmartEnupla<Periodo, double>(vazao_turbinada_maxima_conjunto, 0.0);
							}

							if ((preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_vazao_turbinada_maxima) == sim_operacional) || (preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_vazao_turbinada_maxima) == sim_premissa)) {

								for (Periodo periodo = vazao_turbinada_maxima_unidade.getIteradorInicial(); periodo <= vazao_turbinada_maxima_unidade.getIteradorFinal(); vazao_turbinada_maxima_unidade.incrementarIterador(periodo)) {

									if (getElementoVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_disponibilidade, periodo, double()) > 0.0)
										vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).addElemento(AttVetorUnidadeUHE_vazao_turbinada_maxima, periodo, getAtributo(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttComumUnidadeUHE_vazao_turbinada_maxima, double()));
									else
										vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).addElemento(AttVetorUnidadeUHE_vazao_turbinada_maxima, periodo, 0.0);

								} // for (Periodo periodo = vazao_turbinada_maxima_unidade.getIteradorInicial(); periodo <= vazao_turbinada_maxima_unidade.getIteradorFinal(); vazao_turbinada_maxima_unidade.incrementarIterador(periodo)) {

							} // if ((preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_vazao_turbinada_maxima) == sim_operacional) || (preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_vazao_turbinada_maxima) == sim_premissa)) {

						} // if (getSizeVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_vazao_turbinada_maxima) == 0) {

						if (getSizeVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_vazao_turbinada_maxima) > 0) {

							if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0)
								preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_vazao_turbinada_maxima) = nao_operacional_informado;

							else if ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_maxima) == sim_premissa) || (preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_maxima) == sim_operacional))
								preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorUnidadeUHE_vazao_turbinada_maxima) = nao_premissa_informado;

							else
								throw std::invalid_argument("O atributo " + getFullString(AttVetorUnidadeUHE_vazao_turbinada_maxima) + " nao deve ser informado em " + getFullString(idHidreletrica) + " " + getFullString(idConjuntoHidraulico) + " " + getFullString(idUnidadeUHE));

						} // if (getSizeVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_vazao_turbinada_maxima) > 0) {


						if ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_maxima) == sim_premissa) || (preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_maxima) == sim_operacional)) {
							for (Periodo periodo = vazao_turbinada_maxima_conjunto.getIteradorInicial(); periodo <= vazao_turbinada_maxima_conjunto.getIteradorFinal(); vazao_turbinada_maxima_conjunto.incrementarIterador(periodo))
								vazao_turbinada_maxima_conjunto.at(periodo) += getElementoVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_vazao_turbinada_maxima, periodo, double());
						} // if ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_maxima) == sim_premissa) || (preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_maxima) == sim_operacional)){


						// ----------------------------------
						//
						// Preencher disponibilidade_conjunto
						//
						// ----------------------------------

						if ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_disponibilidade) == sim_premissa) || (preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_disponibilidade) == sim_operacional)) {
							for (Periodo periodo = disponibilidade_conjunto.getIteradorInicial(); periodo <= disponibilidade_conjunto.getIteradorFinal(); disponibilidade_conjunto.incrementarIterador(periodo)) {

								double potencia_maxima_unidade = 0.0;

								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++)
									potencia_maxima_unidade += getElementoMatriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_maxima, periodo, idPatamarCarga, double()) * getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());

								disponibilidade_conjunto.at(periodo) += getElementoVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_disponibilidade, periodo, double()) * potencia_maxima_unidade;
							}
						} // if ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_disponibilidade) == sim_premissa) || (preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_disponibilidade) == sim_operacional)){


						// ------------------------------------
						//
						// Funcao Producao Hidreletrica Por Unidade UHE
						//
						// ------------------------------------

						if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0) {

							preencher_AttVetorFuncaoProducaoHidreletrica.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE) = SmartEnupla<AttVetorFuncaoProducaoHidreletrica, PreencherAtributo>(AttVetorFuncaoProducaoHidreletrica(AttVetorFuncaoProducaoHidreletrica_Nenhum + 1), std::vector<PreencherAtributo>(AttVetorFuncaoProducaoHidreletrica(AttVetorFuncaoProducaoHidreletrica_Excedente - 1), nao_sem_utilizacao));
							preencher_AttMatrizFuncaoProducaoHidreletrica.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE) = SmartEnupla<AttMatrizFuncaoProducaoHidreletrica, PreencherAtributo>(AttMatrizFuncaoProducaoHidreletrica(AttMatrizFuncaoProducaoHidreletrica_Nenhum + 1), std::vector<PreencherAtributo>(AttMatrizFuncaoProducaoHidreletrica(AttMatrizFuncaoProducaoHidreletrica_Excedente - 1), nao_sem_utilizacao));

							if (getMaiorId(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica()) == IdFuncaoProducaoHidreletrica_Nenhum)
								vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).vetorFuncaoProducaoHidreletrica.add(FuncaoProducaoHidreletrica());

							validaFuncaoProducaoHidreletrica(vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1), vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade), idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, preencher_AttVetorFuncaoProducaoHidreletrica.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE), preencher_AttMatrizFuncaoProducaoHidreletrica.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE), preencher_AttVetorHidreletrica.at(idHidreletrica), preencher_AttVetorReservatorio.at(idHidreletrica));

						} // if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0) {

					} // for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= maiorIdUnidadeUHE; idUnidadeUHE++) {


					// -------------------------------------------
					//
					// AttMatrizConjuntoHidraulico_potencia_minima
					//
					// -------------------------------------------

					if ((preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_minima) == sim_premissa) || (preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_minima) == sim_operacional))
						vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).setMatriz(AttMatrizConjuntoHidraulico_potencia_minima, potencia_minima_conjunto);

					// -------------------------------------------
					//
					// AttMatrizHidreletrica_potencia_minima
					//
					// -------------------------------------------

					if ((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_minima) == sim_premissa) || (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_minima) == sim_operacional)) {
						for (Periodo periodo = potencia_minima_hidreletrica.getIteradorInicial(); periodo <= potencia_minima_hidreletrica.getIteradorFinal(); potencia_minima_hidreletrica.incrementarIterador(periodo)) {
							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++)
								potencia_minima_hidreletrica.at(periodo).at(idPatamarCarga) += getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_minima, periodo, idPatamarCarga, double());
						}
					} // if ((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_minima) == sim_premissa) || (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_minima) == sim_operacional)) {

					// -------------------------------------------
					//
					// AttMatrizConjuntoHidraulico_potencia_maxima
					//
					// -------------------------------------------

					if ((preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_maxima) == sim_premissa) || (preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttMatrizConjuntoHidraulico_potencia_maxima) == sim_operacional))
						vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).setMatriz(AttMatrizConjuntoHidraulico_potencia_maxima, potencia_maxima_conjunto);

					// -------------------------------------------
					//
					// AttMatrizHidreletrica_potencia_maxima
					//
					// -------------------------------------------

					if ((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_maxima) == sim_premissa) || (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_maxima) == sim_operacional)) {
						for (Periodo periodo = potencia_maxima_hidreletrica.getIteradorInicial(); periodo <= potencia_maxima_hidreletrica.getIteradorFinal(); potencia_maxima_hidreletrica.incrementarIterador(periodo)) {
							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++)
								potencia_maxima_hidreletrica.at(periodo).at(idPatamarCarga) += getElementoMatriz(idHidreletrica, idConjuntoHidraulico, AttMatrizConjuntoHidraulico_potencia_maxima, periodo, idPatamarCarga, double());
						}
					} // if ((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_maxima) == sim_premissa) || (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_maxima) == sim_operacional)) {


					// ----------------------------------------------
					//
					// AttVetorConjuntoHidraulico_vazao_turbinada_minima
					//
					// ----------------------------------------------

					if ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_minima) == sim_premissa) || (preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_minima) == sim_operacional))
						vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).setVetor(AttVetorConjuntoHidraulico_vazao_turbinada_minima, vazao_turbinada_minima_conjunto);



					// ----------------------------------------------
					//
					// AttVetorHidreletrica_vazao_turbinada_minima
					//
					// ----------------------------------------------

					if ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_turbinada_minima) == sim_premissa) || (preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_turbinada_minima) == sim_operacional)) {
						for (Periodo periodo = vazao_turbinada_minima_hidreletrica.getIteradorInicial(); periodo <= vazao_turbinada_minima_hidreletrica.getIteradorFinal(); vazao_turbinada_minima_hidreletrica.incrementarIterador(periodo))
							vazao_turbinada_minima_hidreletrica.at(periodo) += getElementoVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_minima, periodo, double());
					} // if ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_turbinada_minima) == sim_premissa) || (preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_turbinada_minima) == sim_operacional)) {

					// ----------------------------------------------
					//
					// AttVetorConjuntoHidraulico_vazao_turbinada_maxima
					//
					// ----------------------------------------------

					if ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_maxima) == sim_premissa) || (preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_vazao_turbinada_maxima) == sim_operacional))
						vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).setVetor(AttVetorConjuntoHidraulico_vazao_turbinada_maxima, vazao_turbinada_maxima_conjunto);

					// ----------------------------------------------
					//
					// AttVetorHidreletrica_vazao_turbinada_maxima
					//
					// ----------------------------------------------

					if ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_turbinada_maxima) == sim_premissa) || (preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_turbinada_maxima) == sim_operacional)) {
						for (Periodo periodo = vazao_turbinada_maxima_hidreletrica.getIteradorInicial(); periodo <= vazao_turbinada_maxima_hidreletrica.getIteradorFinal(); vazao_turbinada_maxima_hidreletrica.incrementarIterador(periodo))
							vazao_turbinada_maxima_hidreletrica.at(periodo) += getElementoVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_maxima, periodo, double());
					} // if ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_turbinada_maxima) == sim_premissa) || (preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_turbinada_maxima) == sim_operacional)) {


					// -------------------------------------------
					//
					// AttVetorConjuntoHidraulico_disponibilidade
					//
					// -------------------------------------------

					if ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_disponibilidade) == sim_premissa) || (preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_disponibilidade) == sim_operacional)) {
						for (Periodo periodo = disponibilidade_conjunto.getIteradorInicial(); periodo <= disponibilidade_conjunto.getIteradorFinal(); disponibilidade_conjunto.incrementarIterador(periodo)) {

							double potencia_maxima_por_conjunto = 0.0;

							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {
								for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++)
									potencia_maxima_por_conjunto += getElementoMatriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_maxima, periodo, idPatamarCarga, double()) * getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
							}

							if (potencia_maxima_por_conjunto > 0.0) {

								double disponibilidade_conjunto_periodo = disponibilidade_conjunto.at(periodo) / potencia_maxima_por_conjunto;

								if (doubleCompara(1e-6, disponibilidade_conjunto_periodo, 1.0))
									disponibilidade_conjunto_periodo = 1.0;

								disponibilidade_conjunto.at(periodo) = disponibilidade_conjunto_periodo;

							}//if (potencia_maxima_por_conjunto > 0.0) {								
							else
								disponibilidade_conjunto.at(periodo) = 0.0;
						}
						vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).setVetor(AttVetorConjuntoHidraulico_disponibilidade, disponibilidade_conjunto);

					} // if ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_disponibilidade) == sim_premissa) || (preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(AttVetorConjuntoHidraulico_disponibilidade) == sim_operacional)) {

					// ------------------------------------
					//
					// AttVetorHidreletrica_disponibilidade
					//
					// ------------------------------------

					if ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_disponibilidade) == sim_premissa) || (preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_disponibilidade) == sim_operacional)) {
						for (Periodo periodo = disponibilidade_hidreletrica.getIteradorInicial(); periodo <= disponibilidade_hidreletrica.getIteradorFinal(); disponibilidade_hidreletrica.incrementarIterador(periodo)) {

							double potencia_maxima_por_conjunto = 0.0;

							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {
								for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++)
									potencia_maxima_por_conjunto += getElementoMatriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_maxima, periodo, idPatamarCarga, double()) * getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
							}

							disponibilidade_hidreletrica.at(periodo) += getElementoVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_disponibilidade, periodo, double()) * potencia_maxima_por_conjunto;
						}
					} // if ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_disponibilidade) == sim_premissa) || (preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_disponibilidade) == sim_operacional)) {


					// ------------------------------------
					//
					// Funcao Producao Hidreletrica Por Conjunto Hidraulico
					//
					// ------------------------------------

					if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto).size() > 0) {

						preencher_AttVetorFuncaoProducaoHidreletrica.at(idHidreletrica).at(idConjuntoHidraulico).at(IdUnidadeUHE_Nenhum) = SmartEnupla<AttVetorFuncaoProducaoHidreletrica, PreencherAtributo>(AttVetorFuncaoProducaoHidreletrica(AttVetorFuncaoProducaoHidreletrica_Nenhum + 1), std::vector<PreencherAtributo>(AttVetorFuncaoProducaoHidreletrica(AttVetorFuncaoProducaoHidreletrica_Excedente - 1), nao_sem_utilizacao));
						preencher_AttMatrizFuncaoProducaoHidreletrica.at(idHidreletrica).at(idConjuntoHidraulico).at(IdUnidadeUHE_Nenhum) = SmartEnupla<AttMatrizFuncaoProducaoHidreletrica, PreencherAtributo>(AttMatrizFuncaoProducaoHidreletrica(AttMatrizFuncaoProducaoHidreletrica_Nenhum + 1), std::vector<PreencherAtributo>(AttMatrizFuncaoProducaoHidreletrica(AttMatrizFuncaoProducaoHidreletrica_Excedente - 1), nao_sem_utilizacao));

						if (getMaiorId(idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica()) == IdFuncaoProducaoHidreletrica_Nenhum)
							vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorFuncaoProducaoHidreletrica.add(FuncaoProducaoHidreletrica());

						validaFuncaoProducaoHidreletrica(vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1), vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto), idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE_Nenhum, preencher_AttVetorFuncaoProducaoHidreletrica.at(idHidreletrica).at(idConjuntoHidraulico).at(IdUnidadeUHE_Nenhum), preencher_AttMatrizFuncaoProducaoHidreletrica.at(idHidreletrica).at(idConjuntoHidraulico).at(IdUnidadeUHE_Nenhum), preencher_AttVetorHidreletrica.at(idHidreletrica), preencher_AttVetorReservatorio.at(idHidreletrica));

					} // if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0) {

				} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {


				// -------------------------------------
				//
				// AttMatrizHidreletrica_potencia_minima
				//
				// -------------------------------------

				if ((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_minima) == sim_premissa) || (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_minima) == sim_operacional))
					vetorHidreletrica.att(idHidreletrica).setMatriz(AttMatrizHidreletrica_potencia_minima, potencia_minima_hidreletrica);


				// -------------------------------------
				//
				// AttMatrizHidreletrica_potencia_maxima
				//
				// -------------------------------------

				if ((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_maxima) == sim_premissa) || (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_maxima) == sim_operacional))
					vetorHidreletrica.att(idHidreletrica).setMatriz(AttMatrizHidreletrica_potencia_maxima, potencia_maxima_hidreletrica);


				// CONDI��O PARA QUANDO A HIDRELETRICA ESTA ENCHENDO VOLUME MORTO
				for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo))
					if (getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_enchendo_volume_morto, periodo, int()) == 1)
						for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++)
							vetorHidreletrica.att(idHidreletrica).setElemento(AttMatrizHidreletrica_potencia_maxima, periodo, idPatamarCarga, 0.0);

				// ----------------------------------------
				//
				// AttVetorHidreletrica_vazao_turbinada_minima
				//
				// ----------------------------------------

				if ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_turbinada_minima) == sim_premissa) || (preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_turbinada_minima) == sim_operacional))
					vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_vazao_turbinada_minima, vazao_turbinada_minima_hidreletrica);


				// ----------------------------------------
				//
				// AttVetorHidreletrica_vazao_turbinada_maxima
				//
				// ---------------------------------------

				if ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_turbinada_maxima) == sim_premissa) || (preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_vazao_turbinada_maxima) == sim_operacional))
					vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_vazao_turbinada_maxima, vazao_turbinada_maxima_hidreletrica);



				// ------------------------------------
				//
				// AttVetorHidreletrica_disponibilidade
				//
				// ------------------------------------

				if ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_disponibilidade) == sim_premissa) || (preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_disponibilidade) == sim_operacional)) {

					for (Periodo periodo = disponibilidade_hidreletrica.getIteradorInicial(); periodo <= disponibilidade_hidreletrica.getIteradorFinal(); disponibilidade_hidreletrica.incrementarIterador(periodo)) {

						double potencia_maxima = 0.0;

						for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= getMaiorId(idHidreletrica, IdConjuntoHidraulico()); idConjuntoHidraulico++) {
							for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {
								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++)
									potencia_maxima += getElementoMatriz(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_maxima, periodo, idPatamarCarga, double()) * getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
							}
						}

						if (potencia_maxima > 0.0) {

							double disponibilidade_hidreletrica_periodo = disponibilidade_hidreletrica.at(periodo) / potencia_maxima;

							if (doubleCompara(1e-6, disponibilidade_hidreletrica_periodo, 1.0))
								disponibilidade_hidreletrica_periodo = 1.0;

							disponibilidade_hidreletrica.at(periodo) = disponibilidade_hidreletrica_periodo;

						}//if (potencia_maxima > 0.0) {				
						else
							disponibilidade_hidreletrica.at(periodo) = 0.0;

					} // for (Periodo periodo = disponibilidade_hidreletrica.getIteradorInicial(); periodo <= disponibilidade_hidreletrica.getIteradorFinal(); disponibilidade_hidreletrica.incrementarIterador(periodo)) {

					vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_disponibilidade, disponibilidade_hidreletrica);

				} // if ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_disponibilidade) == sim_premissa) || (preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_disponibilidade) == sim_operacional)) {



				// ------------------------------------
				//
				// AttVetorHidreletrica_regularizacao
				//
				// ------------------------------------
				// 

				if ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_regularizacao) == sim_premissa) || (preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_regularizacao) == sim_operacional)) {


					for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

						double vazao_turbinada_maxima = 0.0;

						if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0 && hidreletrica_com_unidade)
							vazao_turbinada_maxima = getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_maxima, periodo, double());

						if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto).size() > 0 && hidreletrica_com_unidade) {
							for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++)
								vazao_turbinada_maxima += getElementoVetor(idHidreletrica, idConjuntoHidraulico, AttVetorConjuntoHidraulico_vazao_turbinada_maxima, periodo, double());
						}//if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto).size() > 0 && hidreletrica_com_unidade) {

						if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0 && hidreletrica_com_unidade) {
							for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {
								IdUnidadeUHE maiorIdUnidadeUHE = getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE());
								for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= maiorIdUnidadeUHE; idUnidadeUHE++)
									vazao_turbinada_maxima += getElementoVetor(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttVetorUnidadeUHE_vazao_turbinada_maxima, periodo, double());
							}
						} // if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0 && hidreletrica_com_unidade) {

						const double volume_util = getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, periodo, double());

						if (Reservatorio::isReservatorioDeRegularizacao(periodo, volume_util, vazao_turbinada_maxima))
							vetorHidreletrica.att(idHidreletrica).addElemento(AttVetorHidreletrica_regularizacao, periodo, 1);
						else
							vetorHidreletrica.att(idHidreletrica).addElemento(AttVetorHidreletrica_regularizacao, periodo, 0);

					} // for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {				

				} // if ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_regularizacao) == sim_premissa) || (preencher_AttVetorHidreletrica.at(idHidreletrica).at(AttVetorHidreletrica_regularizacao) == sim_operacional)) {

				// ------------------------------------
				//
				// Funcao Producao Hidreletrica Por Hidreletrica
				//
				// ------------------------------------

				if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0) {

					preencher_AttVetorFuncaoProducaoHidreletrica.at(idHidreletrica).at(IdConjuntoHidraulico_Nenhum).at(IdUnidadeUHE_Nenhum) = SmartEnupla<AttVetorFuncaoProducaoHidreletrica, PreencherAtributo>(AttVetorFuncaoProducaoHidreletrica(AttVetorFuncaoProducaoHidreletrica_Nenhum + 1), std::vector<PreencherAtributo>(AttVetorFuncaoProducaoHidreletrica(AttVetorFuncaoProducaoHidreletrica_Excedente - 1), nao_sem_utilizacao));
					preencher_AttMatrizFuncaoProducaoHidreletrica.at(idHidreletrica).at(IdConjuntoHidraulico_Nenhum).at(IdUnidadeUHE_Nenhum) = SmartEnupla<AttMatrizFuncaoProducaoHidreletrica, PreencherAtributo>(AttMatrizFuncaoProducaoHidreletrica(AttMatrizFuncaoProducaoHidreletrica_Nenhum + 1), std::vector<PreencherAtributo>(AttMatrizFuncaoProducaoHidreletrica(AttMatrizFuncaoProducaoHidreletrica_Excedente - 1), nao_sem_utilizacao));

					if (getMaiorId(idHidreletrica, IdFuncaoProducaoHidreletrica()) == IdFuncaoProducaoHidreletrica_Nenhum)
						vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.add(FuncaoProducaoHidreletrica());

					validaFuncaoProducaoHidreletrica(vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1), vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina), idHidreletrica, IdConjuntoHidraulico_Nenhum, IdUnidadeUHE_Nenhum, preencher_AttVetorFuncaoProducaoHidreletrica.at(idHidreletrica).at(IdConjuntoHidraulico_Nenhum).at(IdUnidadeUHE_Nenhum), preencher_AttMatrizFuncaoProducaoHidreletrica.at(idHidreletrica).at(IdConjuntoHidraulico_Nenhum).at(IdUnidadeUHE_Nenhum), preencher_AttVetorHidreletrica.at(idHidreletrica), preencher_AttVetorReservatorio.at(idHidreletrica));

				} // if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0) {valida_vazao_defluente_minima

				//
				// Vazões turbinadas disponíveis
				//

				if (true) {

					// -------------------------------------------
					//
					// AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima
					//
					// -------------------------------------------

					if ((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima) == sim_premissa) || (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima) == sim_operacional)) {

						SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> vazao_turbinada_disponivel_maxima(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>());

						for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

							vazao_turbinada_disponivel_maxima.setElemento(periodo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(maiorIdPatamarCarga, 0.0)));

							const double vazao_turbinada_maxima = getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_maxima, periodo, double());
							const double disponibilidade = getElementoVetor(idHidreletrica, AttVetorHidreletrica_disponibilidade, periodo, double());

							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

								vazao_turbinada_disponivel_maxima.at(periodo).at(idPatamarCarga) = vazao_turbinada_maxima * disponibilidade;

							} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						vetorHidreletrica.att(idHidreletrica).setMatriz(AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima, vazao_turbinada_disponivel_maxima);

					} // if (((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima) == sim_premissa) || (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima) == sim_operacional)) && (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima) == 0)) {


					// -------------------------------------------
					//
					// AttMatrizHidreletrica_vazao_turbinada_disponivel_minima
					//
					// -------------------------------------------

					if (((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_turbinada_disponivel_minima) == sim_premissa) || (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_turbinada_disponivel_minima) == sim_operacional))) {

						SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> vazao_turbinada_disponivel_minima(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>());

						for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

							vazao_turbinada_disponivel_minima.setElemento(periodo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(maiorIdPatamarCarga, 0.0)));

							const double disponibilidade = getElementoVetor(idHidreletrica, AttVetorHidreletrica_disponibilidade, periodo, double());
							const double vazao_turbinada_minima = getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_turbinada_minima, periodo, double());

							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

								vazao_turbinada_disponivel_minima.at(periodo).at(idPatamarCarga) = vazao_turbinada_minima * disponibilidade;

								if (vazao_turbinada_disponivel_minima.at(periodo).at(idPatamarCarga) > getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima, periodo, idPatamarCarga, double()))
									vazao_turbinada_disponivel_minima.at(periodo).at(idPatamarCarga) = getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima, periodo, idPatamarCarga, double());

							} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						vetorHidreletrica.att(idHidreletrica).setMatriz(AttMatrizHidreletrica_vazao_turbinada_disponivel_minima, vazao_turbinada_disponivel_minima);

					} // if (((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_turbinada_disponivel_minima) == sim_premissa) || (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_turbinada_disponivel_minima) == sim_operacional))) {

					std::vector<IdRestricaoOperativaUHE> vetor_idRestricaoOperativaUHE_comUHE_vazao_turbinada;

					if ((getAtributo(AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_hidraulicas, bool())) && (a_imprimir_atributos_sem_recarregar)) {
						for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= getMaiorId(IdRestricaoOperativaUHE()); idRestricaoOperativaUHE++) {
							if ((getAtributo(idRestricaoOperativaUHE, IdElementoSistema_1, AttComumElementoSistema_hidreletrica, IdHidreletrica()) == idHidreletrica) && (getAtributo(idRestricaoOperativaUHE, IdElementoSistema_1, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa()) == TipoVariavelRestricaoOperativa_vazao_turbinada) && (isRestricaoOperativaUHE_simples(idRestricaoOperativaUHE)))
								vetor_idRestricaoOperativaUHE_comUHE_vazao_turbinada.push_back(idRestricaoOperativaUHE);
						}
					} // if (getAtributo(AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_hidraulicas, bool())) {

					if (vetor_idRestricaoOperativaUHE_comUHE_vazao_turbinada.size() > 0) {

						for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {

								for (int pos = 0; pos < int(vetor_idRestricaoOperativaUHE_comUHE_vazao_turbinada.size()); pos++) {

									if (getElementoMatriz(vetor_idRestricaoOperativaUHE_comUHE_vazao_turbinada.at(pos), AttMatrizRestricaoOperativaUHE_limite_inferior, periodo, idPatamarCarga, double()) > getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_minima, periodo, idPatamarCarga, double()))
										vetorHidreletrica.att(idHidreletrica).setElemento(AttMatrizHidreletrica_vazao_turbinada_disponivel_minima, periodo, idPatamarCarga, getElementoMatriz(vetor_idRestricaoOperativaUHE_comUHE_vazao_turbinada.at(pos), AttMatrizRestricaoOperativaUHE_limite_inferior, periodo, idPatamarCarga, double()));

									if (getElementoMatriz(vetor_idRestricaoOperativaUHE_comUHE_vazao_turbinada.at(pos), AttMatrizRestricaoOperativaUHE_limite_superior, periodo, idPatamarCarga, double()) < getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima, periodo, idPatamarCarga, double()))
										vetorHidreletrica.att(idHidreletrica).setElemento(AttMatrizHidreletrica_vazao_turbinada_disponivel_maxima, periodo, idPatamarCarga, getElementoMatriz(vetor_idRestricaoOperativaUHE_comUHE_vazao_turbinada.at(pos), AttMatrizRestricaoOperativaUHE_limite_superior, periodo, idPatamarCarga, double()));

								}//for (int pos = 0; pos < int(vetor_idRestricaoOperativaUHE_comUHE.size()); pos++) {

							} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
					} // if (vetor_idRestricaoOperativaUHE_comUHE_vazao_turbinada.size() > 0) {

				} // if (true) {

				//
				// Vazões defluentes disponíveis
				//

				if (true) {

					// -------------------------------------------
					//
					// AttMatrizHidreletrica_vazao_defluente_disponivel_maxima
					//
					// -------------------------------------------

					if ((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_defluente_maxima) == sim_premissa) || (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_defluente_maxima) == sim_operacional)) {

						SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> vazao_defluente_maxima_porPatamar(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>());

						for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

							vazao_defluente_maxima_porPatamar.setElemento(periodo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(maiorIdPatamarCarga, 0.0)));

							const double vazao_defluente_maxima = getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_defluente_maxima, periodo, double());

							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

								vazao_defluente_maxima_porPatamar.at(periodo).at(idPatamarCarga) = vazao_defluente_maxima;

							} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						vetorHidreletrica.att(idHidreletrica).setMatriz(AttMatrizHidreletrica_vazao_defluente_maxima, vazao_defluente_maxima_porPatamar);

					} // if (((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_defluente_disponivel_maxima) == sim_premissa) || (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_defluente_disponivel_maxima) == sim_operacional)) && (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_defluente_disponivel_maxima) == 0)) {


					// -------------------------------------------
					//
					// AttMatrizHidreletrica_vazao_defluente_disponivel_minima
					//
					// -------------------------------------------

					if (((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_defluente_minima) == sim_premissa) || (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_defluente_minima) == sim_operacional))) {

						SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> vazao_defluente_minima_porPatamar(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>());

						for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

							vazao_defluente_minima_porPatamar.setElemento(periodo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(maiorIdPatamarCarga, 0.0)));

							const double vazao_defluente_minima = getElementoVetor(idHidreletrica, AttVetorHidreletrica_vazao_defluente_minima, periodo, double());

							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

								if (vazao_defluente_minima > getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_defluente_maxima, periodo, idPatamarCarga, double()))
									throw std::invalid_argument("Elemento de " + getFullString(AttVetorHidreletrica_vazao_defluente_minima) + " maior que " + getFullString(AttMatrizHidreletrica_vazao_defluente_maxima) + " em " + getFullString(idHidreletrica));

								vazao_defluente_minima_porPatamar.at(periodo).at(idPatamarCarga) = vazao_defluente_minima;
							}

						} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						vetorHidreletrica.att(idHidreletrica).setMatriz(AttMatrizHidreletrica_vazao_defluente_minima, vazao_defluente_minima_porPatamar);

					} // if (((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_defluente_disponivel_minima) == sim_premissa) || (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_vazao_defluente_disponivel_minima) == sim_operacional))) {

					std::vector<IdRestricaoOperativaUHE> vetor_idRestricaoOperativaUHE_comUHE_vazao_defluente;

					if ((getAtributo(AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_hidraulicas, bool())) && (a_imprimir_atributos_sem_recarregar)) {
						for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= getMaiorId(IdRestricaoOperativaUHE()); idRestricaoOperativaUHE++) {
							if ((getAtributo(idRestricaoOperativaUHE, IdElementoSistema_1, AttComumElementoSistema_hidreletrica, IdHidreletrica()) == idHidreletrica) && (getAtributo(idRestricaoOperativaUHE, IdElementoSistema_1, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa()) == TipoVariavelRestricaoOperativa_vazao_defluente) && (isRestricaoOperativaUHE_simples(idRestricaoOperativaUHE)))
								vetor_idRestricaoOperativaUHE_comUHE_vazao_defluente.push_back(idRestricaoOperativaUHE);
						}
					} // if (getAtributo(AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_hidraulicas, bool())) {

					if (vetor_idRestricaoOperativaUHE_comUHE_vazao_defluente.size() > 0) {

						for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {

								for (int pos = 0; pos < int(vetor_idRestricaoOperativaUHE_comUHE_vazao_defluente.size()); pos++) {

									if (getElementoMatriz(vetor_idRestricaoOperativaUHE_comUHE_vazao_defluente.at(pos), AttMatrizRestricaoOperativaUHE_limite_inferior, periodo, idPatamarCarga, double()) > getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_defluente_minima, periodo, idPatamarCarga, double()))
										vetorHidreletrica.att(idHidreletrica).setElemento(AttMatrizHidreletrica_vazao_defluente_minima, periodo, idPatamarCarga, getElementoMatriz(vetor_idRestricaoOperativaUHE_comUHE_vazao_defluente.at(pos), AttMatrizRestricaoOperativaUHE_limite_inferior, periodo, idPatamarCarga, double()));

									if (getElementoMatriz(vetor_idRestricaoOperativaUHE_comUHE_vazao_defluente.at(pos), AttMatrizRestricaoOperativaUHE_limite_superior, periodo, idPatamarCarga, double()) < getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_vazao_defluente_maxima, periodo, idPatamarCarga, double()))
										vetorHidreletrica.att(idHidreletrica).setElemento(AttMatrizHidreletrica_vazao_defluente_maxima, periodo, idPatamarCarga, getElementoMatriz(vetor_idRestricaoOperativaUHE_comUHE_vazao_defluente.at(pos), AttMatrizRestricaoOperativaUHE_limite_superior, periodo, idPatamarCarga, double()));

								}//for (int pos = 0; pos < int(vetor_idRestricaoOperativaUHE_comUHE.size()); pos++) {

							} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
					} // if (vetor_idRestricaoOperativaUHE_comUHE_vazao_defluente.size() > 0) {

				} // if (true) {

				if (true) {

					// -------------------------------------------
					//
					// AttMatrizHidreletrica_potencia_disponivel_maxima
					//
					// -------------------------------------------

					if ((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_disponivel_maxima) == sim_premissa) || (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_disponivel_maxima) == sim_operacional)) {

						SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> potencia_disponivel_maxima(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>());

						for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

							const double disponibilidade = getElementoVetor(idHidreletrica, AttVetorHidreletrica_disponibilidade, periodo, double());

							potencia_disponivel_maxima.setElemento(periodo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(maiorIdPatamarCarga, 0.0)));

							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

								const double potencia_maxima = getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_maxima, periodo, idPatamarCarga, double());

								potencia_disponivel_maxima.at(periodo).at(idPatamarCarga) = potencia_maxima * disponibilidade;

							} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						vetorHidreletrica.att(idHidreletrica).setMatriz(AttMatrizHidreletrica_potencia_disponivel_maxima, potencia_disponivel_maxima);

					} // if (((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_disponivel_maxima) == sim_premissa) || (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_disponivel_maxima) == sim_operacional)) && (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_maxima) == 0)) {


					// -------------------------------------------
					//
					// AttMatrizHidreletrica_potencia_disponivel_minima
					//
					// -------------------------------------------

					if (((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_disponivel_minima) == sim_premissa) || (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_disponivel_minima) == sim_operacional))) {

						SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> potencia_disponivel_minima(horizonte_estudo, SmartEnupla<IdPatamarCarga, double>());

						for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

							const double disponibilidade = getElementoVetor(idHidreletrica, AttVetorHidreletrica_disponibilidade, periodo, double());

							potencia_disponivel_minima.setElemento(periodo, SmartEnupla<IdPatamarCarga, double>(IdPatamarCarga_1, std::vector<double>(maiorIdPatamarCarga, 0.0)));

							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

								const double potencia_minima = getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_minima, periodo, idPatamarCarga, double());

								potencia_disponivel_minima.at(periodo).at(idPatamarCarga) = potencia_minima * disponibilidade;

								if (potencia_disponivel_minima.at(periodo).at(idPatamarCarga) > getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_maxima, periodo, idPatamarCarga, double()))
									potencia_disponivel_minima.at(periodo).at(idPatamarCarga) = getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_maxima, periodo, idPatamarCarga, double());

							} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						vetorHidreletrica.att(idHidreletrica).setMatriz(AttMatrizHidreletrica_potencia_disponivel_minima, potencia_disponivel_minima);

					} // if (((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_disponivel_minima) == sim_premissa) || (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(AttMatrizHidreletrica_potencia_disponivel_minima) == sim_operacional))) {

					std::vector<IdRestricaoEletrica> vetor_idRestricaoEletrica;

					if ((getAtributo(AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_eletricas, bool())) && (a_imprimir_atributos_sem_recarregar)) {
						for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= getMaiorId(IdRestricaoEletrica()); idRestricaoEletrica++) {
							if ((getAtributo(idRestricaoEletrica, IdElementoSistema_1, AttComumElementoSistema_hidreletrica, IdHidreletrica()) == idHidreletrica) && (getAtributo(idRestricaoEletrica, IdElementoSistema_1, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa()) == TipoVariavelRestricaoOperativa_Nenhum) && (isRestricaoEletrica_simples(idRestricaoEletrica)))
								vetor_idRestricaoEletrica.push_back(idRestricaoEletrica);
						}
					} // if ((getAtributo(AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_eletricas, bool())) && (a_imprimir_atributos_sem_recarregar)) {

					if (vetor_idRestricaoEletrica.size() > 0) {

						for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++) {

								for (int pos = 0; pos < int(vetor_idRestricaoEletrica.size()); pos++) {

									if (getElementoMatriz(vetor_idRestricaoEletrica.at(pos), AttMatrizRestricaoEletrica_potencia_minima, periodo, idPatamarCarga, double()) > getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_minima, periodo, idPatamarCarga, double()))
										vetorHidreletrica.att(idHidreletrica).setElemento(AttMatrizHidreletrica_potencia_disponivel_minima, periodo, idPatamarCarga, getElementoMatriz(vetor_idRestricaoEletrica.at(pos), AttMatrizRestricaoEletrica_potencia_minima, periodo, idPatamarCarga, double()));

									if (getElementoMatriz(vetor_idRestricaoEletrica.at(pos), AttMatrizRestricaoEletrica_potencia_maxima, periodo, idPatamarCarga, double()) < getElementoMatriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_maxima, periodo, idPatamarCarga, double()))
										vetorHidreletrica.att(idHidreletrica).setElemento(AttMatrizHidreletrica_potencia_disponivel_maxima, periodo, idPatamarCarga, getElementoMatriz(vetor_idRestricaoEletrica.at(pos), AttMatrizRestricaoEletrica_potencia_maxima, periodo, idPatamarCarga, double()));

								}//for (int pos = 0; pos < int(vetor_idRestricaoOperativaUHE_comUHE.size()); pos++) {

							} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
					} // if (vetor_idRestricaoOperativaUHE_comUHE_vazao_defluente.size() > 0) {

				} // if (true) {


				// ------------------------------------
				//
				// AttComumHidreletrica
				//
				// ------------------------------------
				valida_considerar_tempo_viagem_agua(idHidreletrica);

				// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				//
				//                                                                                            Impress�o de Atributos
				//
				// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

				if (idProcesso == IdProcesso_mestre) {

					// --------------------
					//
					// AttComumReservatorio
					//
					// --------------------

					if ((a_imprimir_atributos_sem_recarregar) && (getAtributo(AttComumDados_estagio_inicial, IdEstagio()) == IdEstagio_1)) {

						a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

						a_entradaSaidaDados.setAppendArquivo(impresso_AttComumReservatorio);
						const std::vector<AttComumReservatorio> lista_HIDRELETRICA_RESERVATORIO_AttComumOperacional{ AttComumReservatorio_idReservatorio,    AttComumReservatorio_nome_usina, AttComumReservatorio_volume_util_inicial };

						a_entradaSaidaDados.imprimirArquivoCSV_AttComum("HIDRELETRICA_RESERVATORIO_AttComumOperacional.csv", idHidreletrica, IdReservatorio_1, *this, lista_HIDRELETRICA_RESERVATORIO_AttComumOperacional);

						a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);
						const std::vector<AttComumReservatorio> lista_HIDRELETRICA_RESERVATORIO_AttComumPremissa{ AttComumReservatorio_idReservatorio,    AttComumReservatorio_nome_usina, AttComumReservatorio_percentual_volume_util_inicial };
						a_entradaSaidaDados.imprimirArquivoCSV_AttComum("HIDRELETRICA_RESERVATORIO_AttComumPremissa.csv", idHidreletrica, IdReservatorio_1, *this, lista_HIDRELETRICA_RESERVATORIO_AttComumPremissa);

						a_entradaSaidaDados.imprimirArquivoCSV_AttComum("HIDRELETRICA_RESERVATORIO_AttComumPremissaFull.csv", idHidreletrica, IdReservatorio_1, *this);

						impresso_AttComumReservatorio = true;

					} // if ((a_imprimir_atributos_sem_recarregar) || (recarregar_AttComumReservatorio)) {

					// --------------------
					//
					// AttVetorReservatorio
					//
					// --------------------

					for (AttVetorReservatorio attVetorReservatorio = AttVetorReservatorio(AttVetorReservatorio_Nenhum + 1); attVetorReservatorio < AttVetorReservatorio_Excedente; attVetorReservatorio++) {

						if ((preencher_AttVetorReservatorio.at(idHidreletrica).at(attVetorReservatorio) == sim_premissa) || ((preencher_AttVetorReservatorio.at(idHidreletrica).at(attVetorReservatorio) == nao_premissa_informado) && (a_imprimir_atributos_sem_recarregar))) {

							a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

							if (attVetorReservatorio == AttVetorReservatorio_evaporacao) {
								a_entradaSaidaDados.setAppendArquivo(impresso_AttVetorReservatorio_PorIdMes.at(0));
								a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("HIDRELETRICA_RESERVATORIO_AttVetorPremissa_PorIdMes.csv", idHidreletrica, IdReservatorio_1, *this, IdMes_1, IdMes_12, attVetorReservatorio);
								impresso_AttVetorReservatorio_PorIdMes.at(0) = true;

							}

							else {
								a_entradaSaidaDados.setAppendArquivo(impresso_AttVetorReservatorio_PorPeriodo.at(0));
								a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("HIDRELETRICA_RESERVATORIO_AttVetorPremissa_PorPeriodo.csv", idHidreletrica, IdReservatorio_1, *this, periodo_estudo_inicial, periodo_final_estudo, attVetorReservatorio);
								impresso_AttVetorReservatorio_PorPeriodo.at(0) = true;
							}

							if ((attVetorReservatorio != AttVetorReservatorio_volume_minimo) && (attVetorReservatorio != AttVetorReservatorio_poli_cota_volume_0) && (attVetorReservatorio != AttVetorReservatorio_poli_cota_volume_1) && \
								(attVetorReservatorio != AttVetorReservatorio_poli_cota_volume_2) && (attVetorReservatorio != AttVetorReservatorio_poli_cota_volume_3) && (attVetorReservatorio != AttVetorReservatorio_poli_cota_volume_4))
								vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).clear(attVetorReservatorio);

						} // if ((preencher_AttVetorReservatorio.at(idHidreletrica).at(attVetorReservatorio) == sim_premissa) || ((preencher_AttVetorReservatorio.at(idHidreletrica).at(attVetorReservatorio) == nao_premissa_informado) && (a_imprimir_atributos_sem_recarregar))) {

						else if ((preencher_AttVetorReservatorio.at(idHidreletrica).at(attVetorReservatorio) == sim_operacional) || ((preencher_AttVetorReservatorio.at(idHidreletrica).at(attVetorReservatorio) == nao_operacional_informado) && (a_imprimir_atributos_sem_recarregar))) {

							a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

							a_entradaSaidaDados.setAppendArquivo(impresso_AttVetorReservatorio_PorPeriodo.at(1));
							a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("HIDRELETRICA_RESERVATORIO_AttVetorOperacional_PorPeriodo.csv", idHidreletrica, IdReservatorio_1, *this, periodo_estudo_inicial, periodo_final_estudo, attVetorReservatorio);
							impresso_AttVetorReservatorio_PorPeriodo.at(1) = true;

							if (!a_imprimir_atributos_sem_recarregar) {
								vetorHidreletrica.att(idHidreletrica).vetorReservatorio.att(IdReservatorio_1).clear(attVetorReservatorio);
								recarregar_AttVetorReservatorio_PorPeriodo = true;
							}

						} // else if ((preencher_AttVetorReservatorio.at(idHidreletrica).at(attVetorReservatorio) == sim_operacional) || ((preencher_AttVetorReservatorio.at(idHidreletrica).at(attVetorReservatorio) == nao_operacional_informado) && (a_imprimir_atributos_sem_recarregar))) {

					} // for (AttVetorReservatorio attVetorReservatorio = AttVetorReservatorio(AttVetorReservatorio_Nenhum + 1); attVetorReservatorio < AttVetorReservatorio_Excedente; attVetorReservatorio++) {

					// --------------------
					//
					// AttMatrizReservatorio
					//
					// --------------------

					if ((getSize1Matriz(idHidreletrica, IdReservatorio_1, AttMatrizReservatorio_volume_meta) > 0) && (a_imprimir_atributos_sem_recarregar)) {

						a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

						a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizReservatorio_PorCenarioPorPeriodo.at(1));
						a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("HIDRELETRICA_RESERVATORIO_AttMatrizOperacional_PorCenarioPorPeriodo.csv", idHidreletrica, IdReservatorio_1, *this, AttMatrizReservatorio_volume_meta);
						impresso_AttMatrizReservatorio_PorCenarioPorPeriodo.at(1) = true;


					}//if (getSize1Matriz(idHidreletrica, IdReservatorio_1, AttMatrizReservatorio_volume_meta) > 0) {

					// --------------------
					//
					// AttComumHidreletrica
					//
					// --------------------

					if (a_imprimir_atributos_sem_recarregar) {

						a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

						a_entradaSaidaDados.setAppendArquivo(impresso_AttComumHidreletrica);
						const std::vector<AttComumHidreletrica> lista_HIDRELETRICA_AttComumOperacional{ AttComumHidreletrica_idHidreletrica,    AttComumHidreletrica_nome, AttComumHidreletrica_codigo_usina, AttComumHidreletrica_codigo_posto, AttComumHidreletrica_submercado, AttComumHidreletrica_bacia, AttComumHidreletrica_considerar_usina, AttComumHidreletrica_representacao_discreta_producao,       AttComumHidreletrica_jusante,                   AttComumHidreletrica_jusante_desvio,                       AttComumHidreletrica_tempo_viagem_agua, \
																										AttComumHidreletrica_vertimento_maximo, AttComumHidreletrica_penalidade_afluencia_incremental, AttComumHidreletrica_penalidade_vertimento, AttComumHidreletrica_penalidade_turbinamento, AttComumHidreletrica_penalidade_desvio_agua, AttComumHidreletrica_penalidade_turbinamento_minimo, AttComumHidreletrica_penalidade_vazao_defluente_minima, AttComumHidreletrica_penalidade_vazao_defluente_maxima,\
							AttComumHidreletrica_penalidade_vazao_desviada_minima, AttComumHidreletrica_penalidade_volume_minimo, AttComumHidreletrica_penalidade_potencia_minima, AttComumHidreletrica_tipo_detalhamento_producao,AttComumHidreletrica_penalidade_evaporacao, AttComumHidreletrica_penalidade_volume_util_minimo, AttComumHidreletrica_penalidade_vazao_retirada };
						a_entradaSaidaDados.imprimirArquivoCSV_AttComum("HIDRELETRICA_AttComumOperacional.csv", idHidreletrica, *this, lista_HIDRELETRICA_AttComumOperacional);
						a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);
						a_entradaSaidaDados.imprimirArquivoCSV_AttComum("HIDRELETRICA_AttComumPremissa.csv", idHidreletrica, *this);
						impresso_AttComumHidreletrica = true;

					} // if (a_imprimir_atributos_sem_recarregar) {

					// --------------------
					//
					// AttVetorHidreletrica
					//
					// --------------------

					for (AttVetorHidreletrica attVetorHidreletrica = AttVetorHidreletrica(AttVetorHidreletrica_Nenhum + 1); attVetorHidreletrica < AttVetorHidreletrica_Excedente; attVetorHidreletrica++) {

						if ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(attVetorHidreletrica) == sim_premissa) || ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(attVetorHidreletrica) == nao_premissa_informado) && (a_imprimir_atributos_sem_recarregar))) {

							a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

							a_entradaSaidaDados.setAppendArquivo(impresso_AttVetorHidreletrica_PorPeriodo.at(0));
							a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("HIDRELETRICA_AttVetorPremissa_PorPeriodo.csv", idHidreletrica, *this, periodo_estudo_inicial, periodo_final_estudo, attVetorHidreletrica);
							impresso_AttVetorHidreletrica_PorPeriodo.at(0) = true;

							vetorHidreletrica.att(idHidreletrica).clear(attVetorHidreletrica);

						} // if ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(attVetorHidreletrica) == sim_premissa) || ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(attVetorHidreletrica) == nao_premissa_informado) && (a_imprimir_atributos_sem_recarregar))) {

						else if ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(attVetorHidreletrica) == sim_operacional) || ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(attVetorHidreletrica) == nao_operacional_informado) && (a_imprimir_atributos_sem_recarregar))) {

							a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

							if (attVetorHidreletrica == AttVetorHidreletrica_montante) {
								a_entradaSaidaDados.setAppendArquivo(impresso_AttVetorHidreletrica_PorInteiro.at(1));
								a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("HIDRELETRICA_AttVetorOperacional_PorInteiro.csv", idHidreletrica, *this, 1, 10, attVetorHidreletrica);
								impresso_AttVetorHidreletrica_PorInteiro.at(1) = true;

								if (!a_imprimir_atributos_sem_recarregar) {
									vetorHidreletrica.att(idHidreletrica).clear(attVetorHidreletrica);
									recarregar_AttVetorHidreletrica_PorInteiro = true;
								}

							}
							else {
								a_entradaSaidaDados.setAppendArquivo(impresso_AttVetorHidreletrica_PorPeriodo.at(1));
								a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("HIDRELETRICA_AttVetorOperacional_PorPeriodo.csv", idHidreletrica, *this, periodo_estudo_inicial, periodo_final_estudo, attVetorHidreletrica);
								impresso_AttVetorHidreletrica_PorPeriodo.at(1) = true;

								if (!a_imprimir_atributos_sem_recarregar) {
									vetorHidreletrica.att(idHidreletrica).clear(attVetorHidreletrica);
									recarregar_AttVetorHidreletrica_PorPeriodo = true;
								}

							}

						} // else if ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(attVetorHidreletrica) == sim_operacional) || ((preencher_AttVetorHidreletrica.at(idHidreletrica).at(attVetorHidreletrica) == nao_operacional_informado) && (a_imprimir_atributos_sem_recarregar))) {

					} // for (AttVetorHidreletrica attVetorHidreletrica = AttVetorHidreletrica(AttVetorHidreletrica_Nenhum + 1); attVetorHidreletrica < AttVetorHidreletrica_Excedente; attVetorHidreletrica++) {

					// --------------------
					//
					// AttMatrizHidreletrica
					//
					// --------------------

					for (AttMatrizHidreletrica attMatrizHidreletrica = AttMatrizHidreletrica(AttMatrizHidreletrica_Nenhum + 1); attMatrizHidreletrica < AttMatrizHidreletrica_Excedente; attMatrizHidreletrica++) {

						if ((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(attMatrizHidreletrica) == sim_premissa) || ((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(attMatrizHidreletrica) == nao_premissa_informado) && (a_imprimir_atributos_sem_recarregar))) {

							a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

							a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizHidreletrica_PorPeriodoPorIdPatamarCarga.at(0));
							a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("HIDRELETRICA_AttMatrizPremissa_PorPeriodoPorIdPatamarCarga.csv", idHidreletrica, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, attMatrizHidreletrica);
							impresso_AttMatrizHidreletrica_PorPeriodoPorIdPatamarCarga.at(0) = true;

							vetorHidreletrica.att(idHidreletrica).clear(attMatrizHidreletrica);

						} // if ((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(attMatrizHidreletrica) == sim_premissa) || ((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(attMatrizHidreletrica) == nao_premissa_informado) && (a_imprimir_atributos_sem_recarregar))) {

						else if ((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(attMatrizHidreletrica) == sim_operacional) || ((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(attMatrizHidreletrica) == nao_operacional_informado) && (a_imprimir_atributos_sem_recarregar))) {

							a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

							a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizHidreletrica_PorPeriodoPorIdPatamarCarga.at(1));
							a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("HIDRELETRICA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", idHidreletrica, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, attMatrizHidreletrica);
							impresso_AttMatrizHidreletrica_PorPeriodoPorIdPatamarCarga.at(1) = true;

							if (!a_imprimir_atributos_sem_recarregar) {
								vetorHidreletrica.att(idHidreletrica).clear(attMatrizHidreletrica);
								recarregar_AttMatrizHidreletrica_PorPeriodoPorIdPatamarCarga = true;
							}

						} // else if ((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(attMatrizHidreletrica) == sim_operacional) || ((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(attMatrizHidreletrica) == nao_operacional_informado) && (a_imprimir_atributos_sem_recarregar))) {

						else if (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(attMatrizHidreletrica) == sim_operacional_info) {
							a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

							a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizHidreletrica_PorPeriodoPorIdPatamarCarga.at(2));
							a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("_info_HIDRELETRICA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", idHidreletrica, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, attMatrizHidreletrica);
							impresso_AttMatrizHidreletrica_PorPeriodoPorIdPatamarCarga.at(2) = true;

						} // else if (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(attMatrizHidreletrica) == sim_operacional_info){

						// Imprime Vetores Médios como Info
						if (((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(attMatrizHidreletrica) == sim_operacional) || (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(attMatrizHidreletrica) == nao_operacional_informado))) {

							const AttVetorHidreletrica attVetorHidreletrica_media = getAttVetorHidreletricaFromChar(std::string(getString(attMatrizHidreletrica) + "_media").c_str());

							if (attVetorHidreletrica_media != AttVetorHidreletrica_Nenhum) {

								SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz = getMatriz(idHidreletrica, attMatrizHidreletrica, Periodo(), IdPatamarCarga(), double());

								SmartEnupla<Periodo, double> vetor(matriz, 0.0);

								for (Periodo periodo = matriz.getIteradorInicial(); periodo <= periodo_final_estudo; matriz.incrementarIterador(periodo)) {
									for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= matriz.at(periodo).getIteradorFinal(); idPatamarCarga++)
										vetor.at(periodo) += matriz.at(periodo).at(idPatamarCarga) * getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
								}

								vetorHidreletrica.att(idHidreletrica).setVetor_forced(attVetorHidreletrica_media, vetor);

								a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

								a_entradaSaidaDados.setAppendArquivo(impresso_AttVetorHidreletrica_PorPeriodo.at(2));
								a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_HIDRELETRICA_AttVetorPremissa_PorPeriodo.csv", idHidreletrica, *this, periodo_estudo_inicial, periodo_final_estudo, attVetorHidreletrica_media);
								impresso_AttVetorHidreletrica_PorPeriodo.at(2) = true;

								vetorHidreletrica.att(idHidreletrica).clear(attVetorHidreletrica_media);

							}

						} // if (((preencher_AttMatrizHidreletrica.at(idHidreletrica).at(attMatrizHidreletrica) == sim_operacional) || (preencher_AttMatrizHidreletrica.at(idHidreletrica).at(attMatrizHidreletrica) == nao_operacional_informado))) {

					} // for (AttMatrizHidreletrica attMatrizHidreletrica = AttMatrizHidreletrica(AttMatrizHidreletrica_Nenhum + 1); attMatrizHidreletrica < AttMatrizHidreletrica_Excedente; attMatrizHidreletrica++) {

					if ((getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_meta) > 0) && (a_imprimir_atributos_sem_recarregar)) {

						a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

						a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizHidreletrica_PorIdCenarioPorPeriodo.at(1));
						a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("HIDRELETRICA_AttMatrizOperacional_PorCenarioPorPeriodo.csv", idHidreletrica, *this, AttMatrizHidreletrica_potencia_disponivel_meta);
						impresso_AttMatrizHidreletrica_PorIdCenarioPorPeriodo.at(1) = true;

					}//if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_meta) > 0) {

					if ((getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_turbinada_disponivel_meta) > 0) && (a_imprimir_atributos_sem_recarregar)) {

						a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

						a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizHidreletrica_PorIdCenarioPorPeriodo.at(1));
						a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("HIDRELETRICA_AttMatrizOperacional_PorCenarioPorPeriodo.csv", idHidreletrica, *this, AttMatrizHidreletrica_vazao_turbinada_disponivel_meta);
						impresso_AttMatrizHidreletrica_PorIdCenarioPorPeriodo.at(1) = true;

					}//if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_potencia_disponivel_meta) > 0) {

					if ((getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_vertida_meta) > 0) && (a_imprimir_atributos_sem_recarregar)) {

						a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

						a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizHidreletrica_PorIdCenarioPorPeriodo.at(1));
						a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("HIDRELETRICA_AttMatrizOperacional_PorCenarioPorPeriodo.csv", idHidreletrica, *this, AttMatrizHidreletrica_vazao_vertida_meta);
						impresso_AttMatrizHidreletrica_PorIdCenarioPorPeriodo.at(1) = true;

					}//if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_vertida_meta) > 0) {

					if ((getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_evaporada_meta) > 0) && (a_imprimir_atributos_sem_recarregar)) {

						a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

						a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizHidreletrica_PorIdCenarioPorPeriodo.at(1));
						a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("HIDRELETRICA_AttMatrizOperacional_PorCenarioPorPeriodo.csv", idHidreletrica, *this, AttMatrizHidreletrica_vazao_evaporada_meta);
						impresso_AttMatrizHidreletrica_PorIdCenarioPorPeriodo.at(1) = true;

					}//if (getSize1Matriz(idHidreletrica, AttMatrizHidreletrica_vazao_evaporada_meta) > 0) {

					// ----------------------------------
					//
					// AttComumFuncaoProducaoHidreletrica porUsina
					//
					// ----------------------------------

					if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0) {

						if ((preencher_AttVetorFuncaoProducaoHidreletrica.at(idHidreletrica).at(IdConjuntoHidraulico_Nenhum).at(IdUnidadeUHE_Nenhum).at(AttVetorFuncaoProducaoHidreletrica_RHS) == sim_premissa) || (a_imprimir_atributos_sem_recarregar)) {

							if (preencher_AttVetorFuncaoProducaoHidreletrica.at(idHidreletrica).at(IdConjuntoHidraulico_Nenhum).at(IdUnidadeUHE_Nenhum).at(AttVetorFuncaoProducaoHidreletrica_RHS) != nao_sem_utilizacao) {

								a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

								a_entradaSaidaDados.setAppendArquivo(impresso_AttVetorFuncaoProducaoHidreletrica_por_usina);

								const std::vector<AttVetorFuncaoProducaoHidreletrica> atributosVetor = { AttVetorFuncaoProducaoHidreletrica_RHS, AttVetorFuncaoProducaoHidreletrica_FC, AttVetorFuncaoProducaoHidreletrica_VH, AttVetorFuncaoProducaoHidreletrica_QH, AttVetorFuncaoProducaoHidreletrica_SH };

								a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("HIDRELETRICA_FPH_AttVetorPremissa_PorInteiro.csv", idHidreletrica, IdFuncaoProducaoHidreletrica_1, *this, 1, 50, atributosVetor);
								impresso_AttVetorFuncaoProducaoHidreletrica_por_usina = true;

							}

						}

						if ((preencher_AttMatrizFuncaoProducaoHidreletrica.at(idHidreletrica).at(IdConjuntoHidraulico_Nenhum).at(IdUnidadeUHE_Nenhum).at(AttMatrizFuncaoProducaoHidreletrica_RHS) == sim_operacional) || (a_imprimir_atributos_sem_recarregar)) {

							a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

							a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizFuncaoProducaoHidreletrica_por_usina);

							const std::vector<AttMatrizFuncaoProducaoHidreletrica> atributosMatriz = { AttMatrizFuncaoProducaoHidreletrica_RHS, AttMatrizFuncaoProducaoHidreletrica_FC, AttMatrizFuncaoProducaoHidreletrica_VH, AttMatrizFuncaoProducaoHidreletrica_QH, AttMatrizFuncaoProducaoHidreletrica_SH };

							a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("HIDRELETRICA_FPH_AttMatrizOperacional_PorPeriodoPorInteiro.csv", idHidreletrica, IdFuncaoProducaoHidreletrica_1, *this, getIterador1Inicial(idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, Periodo()), getIterador1Final(idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, Periodo()), 1, 50, atributosMatriz);

							a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

							a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizFuncaoProducaoHidreletrica_por_usina);
							const std::vector<AttVetorFuncaoProducaoHidreletrica> atributosVetor = { AttVetorFuncaoProducaoHidreletrica_volume_minimo, AttVetorFuncaoProducaoHidreletrica_volume_maximo, AttVetorFuncaoProducaoHidreletrica_percentual_volume_minimo, AttVetorFuncaoProducaoHidreletrica_percentual_volume_maximo, };

							a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("HIDRELETRICA_FPH_AttVetorPremissa_PorPeriodo.csv", idHidreletrica, IdFuncaoProducaoHidreletrica_1, *this, getIterador1Inicial(idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, Periodo()), getIterador1Final(idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, Periodo()), atributosVetor);

							impresso_AttMatrizFuncaoProducaoHidreletrica_por_usina = true;


							if (!a_imprimir_atributos_sem_recarregar) {
								recarregar_AttMatrizFuncaoProducaoHidreletrica_por_usina = true;
								if (vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.isInstanciado(IdFuncaoProducaoHidreletrica_1))
									vetorHidreletrica.att(idHidreletrica).vetorFuncaoProducaoHidreletrica.rem(IdFuncaoProducaoHidreletrica_1);
							}

						}

					} // if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0) {


					// ----------------------------------
					//
					// AttVetorDefluencia porUsina
					//
					// ----------------------------------

					if ((a_imprimir_atributos_sem_recarregar) && (getAtributo(AttComumDados_estagio_inicial, IdEstagio()) == IdEstagio_1)) {

						if (getAtributo(idHidreletrica, AttComumHidreletrica_tempo_viagem_agua, int()) > 0) {

							a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

							a_entradaSaidaDados.setAppendArquivo(impresso_AttVetorDefluencia_PorPeriodo);

							a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("HIDRELETRICA_DEFLUENCIA_AttVetorOperacional_PorPeriodo.csv", idHidreletrica, IdDefluencia_Nenhum, *this, AttVetorDefluencia_vazao_defluencia);
							impresso_AttVetorDefluencia_PorPeriodo = true;

						}//if (getAtributo(idHidreletrica, AttComumHidreletrica_tempo_viagem_agua, int()) > 0) {

					} // if ((a_imprimir_atributos_sem_recarregar) && (vetorHidreletrica.att(idHidreletrica).vetorDefluencia.isInstanciado(IdDefluencia_passada))) {

					//POLINOMIO COTA JUSANTE

					for (IdPolinomioJusante idPolinomioJusante = IdPolinomioJusante_1; idPolinomioJusante <= getMaiorId(idHidreletrica, IdPolinomioJusante()); idPolinomioJusante++) {
						a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);
						a_entradaSaidaDados.setAppendArquivo(impresso_PolinomioCotaJusante);
						a_entradaSaidaDados.imprimirArquivoCSV_AttComum("HIDRELETRICA_POLINOMIO_JUSANTE_AttComumPremissa.csv", idHidreletrica, idPolinomioJusante, *this);
						for (AttVetorPolinomioJusante attVetorPolinomioJusante = AttVetorPolinomioJusante(AttVetorPolinomioJusante_Nenhum + 1); attVetorPolinomioJusante < AttVetorPolinomioJusante_Excedente; attVetorPolinomioJusante++) {
							a_entradaSaidaDados.setAppendArquivo(impresso_PolinomioCotaJusante);
							a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("HIDRELETRICA_POLINOMIO_JUSANTE_AttVetorPremissa_PorPeriodo.csv", idHidreletrica, idPolinomioJusante, *this, periodo_estudo_inicial, periodo_final_estudo, attVetorPolinomioJusante);
							impresso_PolinomioCotaJusante = true;
						}
					}

					for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {

						// --------------------------
						//
						// AttComumConjuntoHidraulico
						//
						// --------------------------

						if (((vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto).size() > 0) || (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0)) && (a_imprimir_atributos_sem_recarregar)) {

							a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

							a_entradaSaidaDados.setAppendArquivo(impresso_AttComumConjuntoHidraulico.at(1));
							a_entradaSaidaDados.imprimirArquivoCSV_AttComum("HIDRELETRICA_CONJUNTO_AttComumOperacional.csv", idHidreletrica, idConjuntoHidraulico, *this);
							impresso_AttComumConjuntoHidraulico.at(1) = true;

						} // if (((vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto).size() > 0) || (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0)) && (a_imprimir_atributos_sem_recarregar)) {

						else if ((hidreletrica_com_unidade) && (a_imprimir_atributos_sem_recarregar)) {

							a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

							a_entradaSaidaDados.setAppendArquivo(impresso_AttComumConjuntoHidraulico.at(0));
							a_entradaSaidaDados.imprimirArquivoCSV_AttComum("HIDRELETRICA_CONJUNTO_AttComumPremissa.csv", idHidreletrica, idConjuntoHidraulico, *this);
							impresso_AttComumConjuntoHidraulico.at(0) = true;

						}

						// --------------------------
						//
						// AttVetorConjuntoHidraulico
						//
						// --------------------------

						for (AttVetorConjuntoHidraulico attVetorConjuntoHidraulico = AttVetorConjuntoHidraulico(AttVetorConjuntoHidraulico_Nenhum + 1); attVetorConjuntoHidraulico < AttVetorConjuntoHidraulico_Excedente; attVetorConjuntoHidraulico++) {

							if ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(attVetorConjuntoHidraulico) == sim_premissa) || ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(attVetorConjuntoHidraulico) == nao_premissa_informado) && (a_imprimir_atributos_sem_recarregar))) {

								a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

								a_entradaSaidaDados.setAppendArquivo(impresso_AttVetorConjuntoHidraulico_PorPeriodo.at(0));
								a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("HIDRELETRICA_CONJUNTO_AttVetorPremissa_PorPeriodo.csv", idHidreletrica, idConjuntoHidraulico, *this, periodo_estudo_inicial, periodo_final_estudo, attVetorConjuntoHidraulico);
								impresso_AttVetorConjuntoHidraulico_PorPeriodo.at(0) = true;

								vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).clear(attVetorConjuntoHidraulico);

							} // if ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(attVetorConjuntoHidraulico) == sim_premissa) || ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(attVetorConjuntoHidraulico) == nao_premissa_informado) && (a_imprimir_atributos_sem_recarregar))) {

							else if ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(attVetorConjuntoHidraulico) == sim_operacional) || ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(attVetorConjuntoHidraulico) == nao_operacional_informado) && (a_imprimir_atributos_sem_recarregar))) {

								a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

								a_entradaSaidaDados.setAppendArquivo(impresso_AttVetorConjuntoHidraulico_PorPeriodo.at(1));
								a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("HIDRELETRICA_CONJUNTO_AttVetorOperacional_PorPeriodo.csv", idHidreletrica, idConjuntoHidraulico, *this, periodo_estudo_inicial, periodo_final_estudo, attVetorConjuntoHidraulico);
								impresso_AttVetorConjuntoHidraulico_PorPeriodo.at(1) = true;

								if (!a_imprimir_atributos_sem_recarregar) {
									vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).clear(attVetorConjuntoHidraulico);
									recarregar_AttVetorConjuntoHidraulico_PorPeriodo = true;
								}

							} // else if ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(attVetorConjuntoHidraulico) == sim_operacional) || ((preencher_AttVetorConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(attVetorConjuntoHidraulico) == nao_operacional_informado) && (a_imprimir_atributos_sem_recarregar))) {

						} // for (AttVetorConjuntoHidraulico attVetorConjuntoHidraulico = AttVetorConjuntoHidraulico(AttVetorConjuntoHidraulico_Nenhum + 1); attVetorConjuntoHidraulico < AttVetorConjuntoHidraulico_Excedente; attVetorConjuntoHidraulico++) {


						// --------------------------
						//
						// AttMatrizConjuntoHidraulico
						//
						// --------------------------

						for (AttMatrizConjuntoHidraulico attMatrizConjuntoHidraulico = AttMatrizConjuntoHidraulico(AttMatrizConjuntoHidraulico_Nenhum + 1); attMatrizConjuntoHidraulico < AttMatrizConjuntoHidraulico_Excedente; attMatrizConjuntoHidraulico++) {

							if ((preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(attMatrizConjuntoHidraulico) == sim_premissa) || ((preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(attMatrizConjuntoHidraulico) == nao_premissa_informado) && (a_imprimir_atributos_sem_recarregar))) {

								a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

								a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizConjuntoHidraulico_PorPeriodoPorIdPatamarCarga.at(0));
								a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("HIDRELETRICA_CONJUNTO_AttMatrizPremissa_PorPeriodoPorIdPatamarCarga.csv", idHidreletrica, idConjuntoHidraulico, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, attMatrizConjuntoHidraulico);
								impresso_AttMatrizConjuntoHidraulico_PorPeriodoPorIdPatamarCarga.at(0) = true;

								vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).clear(attMatrizConjuntoHidraulico);

							} // if ((preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(attMatrizConjuntoHidraulico) == sim_premissa) || ((preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(attMatrizConjuntoHidraulico) == nao_premissa_informado) && (a_imprimir_atributos_sem_recarregar))) {

							else if ((preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(attMatrizConjuntoHidraulico) == sim_operacional) || ((preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(attMatrizConjuntoHidraulico) == nao_operacional_informado) && (a_imprimir_atributos_sem_recarregar))) {

								a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

								a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizConjuntoHidraulico_PorPeriodoPorIdPatamarCarga.at(1));
								a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("HIDRELETRICA_CONJUNTO_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", idHidreletrica, idConjuntoHidraulico, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, attMatrizConjuntoHidraulico);
								impresso_AttMatrizConjuntoHidraulico_PorPeriodoPorIdPatamarCarga.at(1) = true;

								if (!a_imprimir_atributos_sem_recarregar) {
									vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).clear(attMatrizConjuntoHidraulico);
									recarregar_AttMatrizConjuntoHidraulico_PorPeriodoPorIdPatamarCarga = true;
								}

							} // else if ((preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(attMatrizConjuntoHidraulico) == sim_operacional) || ((preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(attMatrizConjuntoHidraulico) == nao_operacional_informado) && (a_imprimir_atributos_sem_recarregar))) {

							else if (preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(attMatrizConjuntoHidraulico) == sim_operacional_info) {
								a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

								a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizConjuntoHidraulico_PorPeriodoPorIdPatamarCarga.at(2));
								a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("_info_HIDRELETRICA_CONJUNTO_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", idHidreletrica, idConjuntoHidraulico, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, attMatrizConjuntoHidraulico);
								impresso_AttMatrizConjuntoHidraulico_PorPeriodoPorIdPatamarCarga.at(2) = true;

							} // else if (preencher_AttMatrizConjuntoHidraulico.at(idHidreletrica).at(idConjuntoHidraulico).at(attMatrizConjuntoHidraulico) == sim_operacional_info){

						} // for (AttMatrizConjuntoHidraulico attMatrizConjuntoHidraulico = AttMatrizConjuntoHidraulico(AttMatrizConjuntoHidraulico_Nenhum + 1); attMatrizConjuntoHidraulico < AttMatrizConjuntoHidraulico_Excedente; attMatrizConjuntoHidraulico++) {

						// ----------------------------------
						//
						// AttComumFuncaoProducaoHidreletrica porConjuntoHidraulico
						//
						// ----------------------------------

						if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto).size() > 0) {

							if ((preencher_AttVetorFuncaoProducaoHidreletrica.at(idHidreletrica).at(idConjuntoHidraulico).at(IdUnidadeUHE_Nenhum).at(AttVetorFuncaoProducaoHidreletrica_RHS) == sim_premissa) || (a_imprimir_atributos_sem_recarregar)) {

								if (preencher_AttVetorFuncaoProducaoHidreletrica.at(idHidreletrica).at(idConjuntoHidraulico).at(IdUnidadeUHE_Nenhum).at(AttVetorFuncaoProducaoHidreletrica_RHS) != nao_sem_utilizacao) {

									a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

									a_entradaSaidaDados.setAppendArquivo(impresso_AttVetorFuncaoProducaoHidreletrica_por_conjunto);

									const std::vector<AttVetorFuncaoProducaoHidreletrica> atributosVetor = { AttVetorFuncaoProducaoHidreletrica_RHS, AttVetorFuncaoProducaoHidreletrica_FC, AttVetorFuncaoProducaoHidreletrica_VH, AttVetorFuncaoProducaoHidreletrica_QH, AttVetorFuncaoProducaoHidreletrica_SH };

									a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("HIDRELETRICA_CONJUNTO_FPH_AttVetorPremissa_PorInteiro.csv", idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, *this, 1, 50, atributosVetor);
									impresso_AttVetorFuncaoProducaoHidreletrica_por_conjunto = true;

								}

							}

							if ((preencher_AttMatrizFuncaoProducaoHidreletrica.at(idHidreletrica).at(idConjuntoHidraulico).at(IdUnidadeUHE_Nenhum).at(AttMatrizFuncaoProducaoHidreletrica_RHS) == sim_operacional) || (a_imprimir_atributos_sem_recarregar)) {

								a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

								a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizFuncaoProducaoHidreletrica_por_conjunto);

								const std::vector<AttMatrizFuncaoProducaoHidreletrica> atributosMatriz = { AttMatrizFuncaoProducaoHidreletrica_RHS, AttMatrizFuncaoProducaoHidreletrica_FC, AttMatrizFuncaoProducaoHidreletrica_VH, AttMatrizFuncaoProducaoHidreletrica_QH, AttMatrizFuncaoProducaoHidreletrica_SH };

								a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("HIDRELETRICA_CONJUNTO_FPH_AttMatrizOperacional_PorPeriodoPorInteiro.csv", idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, *this, getIterador1Inicial(idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, Periodo()), getIterador1Final(idHidreletrica, idConjuntoHidraulico, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, Periodo()), 1, 50, atributosMatriz);
								impresso_AttMatrizFuncaoProducaoHidreletrica_por_conjunto = true;

								if (!a_imprimir_atributos_sem_recarregar) {
									recarregar_AttMatrizFuncaoProducaoHidreletrica_por_usina = true;
									if (vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorFuncaoProducaoHidreletrica.isInstanciado(IdFuncaoProducaoHidreletrica_1))
										vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorFuncaoProducaoHidreletrica.rem(IdFuncaoProducaoHidreletrica_1);
								}

							}

						} // if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto).size() > 0) {

						const IdUnidadeUHE maiorIdUnidadeUHE = getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE());
						for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= maiorIdUnidadeUHE; idUnidadeUHE++) {

							// ------------------
							//
							// AttComumUnidadeUHE
							//
							// ------------------

							if ((vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0) && (a_imprimir_atributos_sem_recarregar)) {

								a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

								a_entradaSaidaDados.setAppendArquivo(impresso_AttComumUnidadeUHE.at(1));

								a_entradaSaidaDados.imprimirArquivoCSV_AttComum("HIDRELETRICA_CONJUNTO_UNIDADE_AttComumOperacional.csv", idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, *this);
								impresso_AttComumUnidadeUHE.at(1) = true;

							} // if ((vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0) && (a_imprimir_atributos_sem_recarregar)) {

							else if ((hidreletrica_com_unidade) && (a_imprimir_atributos_sem_recarregar)) {

								a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

								a_entradaSaidaDados.setAppendArquivo(impresso_AttComumUnidadeUHE.at(0));

								a_entradaSaidaDados.imprimirArquivoCSV_AttComum("HIDRELETRICA_CONJUNTO_UNIDADE_AttComumPremissa.csv", idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, *this);
								impresso_AttComumUnidadeUHE.at(0) = true;

							}

							// --------------------------
							//
							// AttVetorUnidadeUHE
							//
							// --------------------------

							for (AttVetorUnidadeUHE attVetorUnidadeUHE = AttVetorUnidadeUHE(AttVetorUnidadeUHE_Nenhum + 1); attVetorUnidadeUHE < AttVetorUnidadeUHE_Excedente; attVetorUnidadeUHE++) {

								if ((preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(attVetorUnidadeUHE) == sim_premissa) || ((preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(attVetorUnidadeUHE) == nao_premissa_informado) && (a_imprimir_atributos_sem_recarregar))) {

									a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

									a_entradaSaidaDados.setAppendArquivo(impresso_AttVetorUnidadeUHE_PorPeriodo.at(0));
									a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("HIDRELETRICA_CONJUNTO_UNIDADE_AttVetorPremissa_PorPeriodo.csv", idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, *this, periodo_estudo_inicial, periodo_final_estudo, attVetorUnidadeUHE);
									impresso_AttVetorUnidadeUHE_PorPeriodo.at(0) = true;

									vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).clear(attVetorUnidadeUHE);

								} // if ((preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(attVetorUnidadeUHE) == sim_premissa) || ((preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(attVetorUnidadeUHE) == nao_premissa_informado) && (a_imprimir_atributos_sem_recarregar))) {

								else if ((preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(attVetorUnidadeUHE) == sim_operacional) || ((preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(attVetorUnidadeUHE) == nao_operacional_informado) && (a_imprimir_atributos_sem_recarregar))) {

									a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

									a_entradaSaidaDados.setAppendArquivo(impresso_AttVetorUnidadeUHE_PorPeriodo.at(1));
									a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("HIDRELETRICA_CONJUNTO_UNIDADE_AttVetorOperacional_PorPeriodo.csv", idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, *this, periodo_estudo_inicial, periodo_final_estudo, attVetorUnidadeUHE);
									impresso_AttVetorUnidadeUHE_PorPeriodo.at(1) = true;

									if (!a_imprimir_atributos_sem_recarregar) {
										vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).clear(attVetorUnidadeUHE);
										recarregar_AttVetorUnidadeUHE_PorPeriodo = true;
									}

								} // else if ((preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(attVetorUnidadeUHE) == sim_operacional) || ((preencher_AttVetorUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(attVetorUnidadeUHE) == nao_operacional_informado) && (a_imprimir_atributos_sem_recarregar))) {

							} // for (AttVetorUnidadeUHE attVetorUnidadeUHE = AttVetorUnidadeUHE(AttVetorUnidadeUHE_Nenhum + 1); attVetorUnidadeUHE < AttVetorUnidadeUHE_Excedente; attVetorUnidadeUHE++) {


							// --------------------------
							//
							// AttMatrizUnidadeUHE
							//
							// --------------------------

							for (AttMatrizUnidadeUHE attMatrizUnidadeUHE = AttMatrizUnidadeUHE(AttMatrizUnidadeUHE_Nenhum + 1); attMatrizUnidadeUHE < AttMatrizUnidadeUHE_Excedente; attMatrizUnidadeUHE++) {

								if ((preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(attMatrizUnidadeUHE) == sim_premissa) || ((preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(attMatrizUnidadeUHE) == nao_premissa_informado) && (a_imprimir_atributos_sem_recarregar))) {

									a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

									a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizUnidadeUHE_PorPeriodoPorIdPatamarCarga.at(0));
									a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("HIDRELETRICA_CONJUNTO_UNIDADE_AttMatrizPremissa_PorPeriodoPorIdPatamarCarga.csv", idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, attMatrizUnidadeUHE);
									impresso_AttMatrizUnidadeUHE_PorPeriodoPorIdPatamarCarga.at(0) = true;

									vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).clear(attMatrizUnidadeUHE);

								} // if ((preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(attMatrizUnidadeUHE) == sim_premissa) || ((preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(attMatrizUnidadeUHE) == nao_premissa_informado) && (a_imprimir_atributos_sem_recarregar))) {

								else if ((preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(attMatrizUnidadeUHE) == sim_operacional) || ((preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(attMatrizUnidadeUHE) == nao_operacional_informado) && (a_imprimir_atributos_sem_recarregar))) {

									a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

									a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizUnidadeUHE_PorPeriodoPorIdPatamarCarga.at(1));
									a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("HIDRELETRICA_CONJUNTO_UNIDADE_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, attMatrizUnidadeUHE);
									impresso_AttMatrizUnidadeUHE_PorPeriodoPorIdPatamarCarga.at(1) = true;

									if (!a_imprimir_atributos_sem_recarregar) {
										vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).clear(attMatrizUnidadeUHE);
										recarregar_AttMatrizUnidadeUHE_PorPeriodoPorIdPatamarCarga = true;
									}

								} // else if ((preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(attMatrizUnidadeUHE) == sim_operacional) || ((preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(attMatrizUnidadeUHE) == nao_operacional_informado) && (a_imprimir_atributos_sem_recarregar))) {

								else if (preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(attMatrizUnidadeUHE) == sim_operacional_info) {
									a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

									a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizUnidadeUHE_PorPeriodoPorIdPatamarCarga.at(2));
									a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("_info_HIDRELETRICA_CONJUNTO_UNIDADE_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, attMatrizUnidadeUHE);
									impresso_AttMatrizUnidadeUHE_PorPeriodoPorIdPatamarCarga.at(2) = true;

								} // else if (preencher_AttMatrizUnidadeUHE.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(attMatrizUnidadeUHE) == sim_operacional_info){

							} // for (AttMatrizUnidadeUHE attMatrizUnidadeUHE = AttMatrizUnidadeUHE(AttMatrizUnidadeUHE_Nenhum + 1); attMatrizUnidadeUHE < AttMatrizUnidadeUHE_Excedente; attMatrizUnidadeUHE++) {

							// ----------------------------------
							//
							// AttComumFuncaoProducaoHidreletrica porUnidade
							//
							// ----------------------------------

							if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0) {

								if ((preencher_AttVetorFuncaoProducaoHidreletrica.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorFuncaoProducaoHidreletrica_RHS) == sim_premissa) || (a_imprimir_atributos_sem_recarregar)) {

									if (preencher_AttVetorFuncaoProducaoHidreletrica.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttVetorFuncaoProducaoHidreletrica_RHS) != nao_sem_utilizacao) {

										a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

										a_entradaSaidaDados.setAppendArquivo(impresso_AttVetorFuncaoProducaoHidreletrica_por_unidade);

										const std::vector<AttVetorFuncaoProducaoHidreletrica> atributosVetor = { AttVetorFuncaoProducaoHidreletrica_RHS, AttVetorFuncaoProducaoHidreletrica_FC, AttVetorFuncaoProducaoHidreletrica_VH, AttVetorFuncaoProducaoHidreletrica_QH, AttVetorFuncaoProducaoHidreletrica_SH };

										a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("HIDRELETRICA_CONJUNTO_FPH_AttVetorPremissa_PorInteiro.csv", idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, *this, 1, 50, atributosVetor);
										impresso_AttVetorFuncaoProducaoHidreletrica_por_unidade = true;

									}

								}

								if ((preencher_AttMatrizFuncaoProducaoHidreletrica.at(idHidreletrica).at(idConjuntoHidraulico).at(idUnidadeUHE).at(AttMatrizFuncaoProducaoHidreletrica_RHS) == sim_operacional) || (a_imprimir_atributos_sem_recarregar)) {

									a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

									a_entradaSaidaDados.setAppendArquivo(impresso_AttMatrizFuncaoProducaoHidreletrica_por_unidade);

									const std::vector<AttMatrizFuncaoProducaoHidreletrica> atributosMatriz = { AttMatrizFuncaoProducaoHidreletrica_RHS, AttMatrizFuncaoProducaoHidreletrica_FC, AttMatrizFuncaoProducaoHidreletrica_VH, AttMatrizFuncaoProducaoHidreletrica_QH, AttMatrizFuncaoProducaoHidreletrica_SH };

									a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("HIDRELETRICA_CONJUNTO_FPH_AttMatrizOperacional_PorPeriodoPorInteiro.csv", idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, *this, getIterador1Inicial(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, Periodo()), getIterador1Final(idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, IdFuncaoProducaoHidreletrica_1, AttMatrizFuncaoProducaoHidreletrica_RHS, Periodo()), 1, 50, atributosMatriz);
									impresso_AttMatrizFuncaoProducaoHidreletrica_por_unidade = true;

									if (!a_imprimir_atributos_sem_recarregar) {
										recarregar_AttMatrizFuncaoProducaoHidreletrica_por_usina = true;
										if (vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).vetorFuncaoProducaoHidreletrica.isInstanciado(IdFuncaoProducaoHidreletrica_1))
											vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.att(idConjuntoHidraulico).vetorUnidadeUHE.att(idUnidadeUHE).vetorFuncaoProducaoHidreletrica.rem(IdFuncaoProducaoHidreletrica_1);
									}

								}

							} // if (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_unidade).size() > 0) {

						} // for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= maiorIdUnidadeUHE; idUnidadeUHE++) {

						if (((vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0) || (vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_conjunto).size() > 0)) && (!a_imprimir_atributos_sem_recarregar))
							vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico.at(idConjuntoHidraulico).vetorUnidadeUHE = VetorUnidadeUHEEmConjuntoHidraulico();

					} // for (IdConjuntoHidraulico idConjuntoHidraulico = IdConjuntoHidraulico_1; idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {

					if ((vetor_zero.at(TipoDetalhamentoProducaoHidreletrica_por_usina).size() > 0) && (!a_imprimir_atributos_sem_recarregar))
						vetorHidreletrica.att(idHidreletrica).vetorConjuntoHidraulico = VetorConjuntoHidraulicoEmHidreletrica();

				} // if (idProcesso == IdProcesso_mestre) {

			} // if (getAtributo(idHidreletrica, AttComumHidreletrica_considerar_usina, bool())) {

		} // for (IdHidreletrica idHidreletrica = menorIdHidreletrica; idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

		instanciaCotaJusanteUsinaJusante();
		validaHidreletrica();

		if (!a_imprimir_atributos_sem_recarregar) {

			if (true) {
				int barreira = 0;

				if (arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso()) == IdProcesso_mestre) {
					for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++)
						MPI_Send(&barreira, 1, MPI_INT, getRank(idProcesso), 0, MPI_COMM_WORLD);
				}
				else
					MPI_Recv(&barreira, 1, MPI_INT, getRank(IdProcesso_mestre), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}


			//
			// Recarrega Atributos Operacionais
			//

			a_entradaSaidaDados.setDiretorioEntrada(a_diretorio_att_operacional);

			if (recarregar_AttVetorReservatorio_PorPeriodo)
				a_entradaSaidaDados.carregarArquivoCSV_AttVetor("HIDRELETRICA_RESERVATORIO_AttVetorOperacional_PorPeriodo.csv", *this, TipoAcessoInstancia_membroMembro);

			if (recarregar_AttVetorHidreletrica_PorPeriodo)
				a_entradaSaidaDados.carregarArquivoCSV_AttVetor("HIDRELETRICA_AttVetorOperacional_PorPeriodo.csv", *this, TipoAcessoInstancia_membro);

			if (recarregar_AttVetorHidreletrica_PorInteiro)
				a_entradaSaidaDados.carregarArquivoCSV_AttVetor("HIDRELETRICA_AttVetorOperacional_PorInteiro.csv", *this, TipoAcessoInstancia_membro);

			if (recarregar_AttMatrizHidreletrica_PorPeriodoPorIdPatamarCarga)
				a_entradaSaidaDados.carregarArquivoCSV_AttMatriz("HIDRELETRICA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);

			if (recarregar_AttMatrizFuncaoProducaoHidreletrica_por_usina)
				a_entradaSaidaDados.carregarArquivoCSV_AttMatriz("HIDRELETRICA_FPH_AttMatrizOperacional_PorPeriodoPorInteiro.csv", *this, TipoAcessoInstancia_membroMembro);

			if (recarregar_AttVetorConjuntoHidraulico_PorPeriodo)
				a_entradaSaidaDados.carregarArquivoCSV_AttVetor("HIDRELETRICA_CONJUNTO_AttVetorOperacional_PorPeriodo.csv", *this, TipoAcessoInstancia_membroMembro);

			if (recarregar_AttMatrizConjuntoHidraulico_PorPeriodoPorIdPatamarCarga)
				a_entradaSaidaDados.carregarArquivoCSV_AttMatriz("HIDRELETRICA_CONJUNTO_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membroMembro);

			if (recarregar_AttMatrizFuncaoProducaoHidreletrica_por_conjunto)
				a_entradaSaidaDados.carregarArquivoCSV_AttMatriz("HIDRELETRICA_CONJUNTO_FPH_AttMatrizOperacional_PorPeriodoPorInteiro.csv", *this, TipoAcessoInstancia_membroMembroMembro);

			if (recarregar_AttVetorUnidadeUHE_PorPeriodo)
				a_entradaSaidaDados.carregarArquivoCSV_AttVetor("HIDRELETRICA_CONJUNTO_UNIDADE_AttVetorOperacional_PorPeriodo.csv", *this, TipoAcessoInstancia_membroMembroMembro);

			if (recarregar_AttMatrizUnidadeUHE_PorPeriodoPorIdPatamarCarga)
				a_entradaSaidaDados.carregarArquivoCSV_AttMatriz("HIDRELETRICA_CONJUNTO_UNIDADE_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membroMembroMembro);

			if (recarregar_AttMatrizFuncaoProducaoHidreletrica_por_unidade)
				a_entradaSaidaDados.carregarArquivoCSV_AttMatriz("HIDRELETRICA_CONJUNTO_UNIDADE_FPH_AttMatrizOperacional_PorPeriodoPorInteiro.csv", *this, TipoAcessoInstancia_membroMembroMembroMembro);

		} // (!a_imprimir_atributos_sem_recarregar) {


	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validacao_operacional_Hidreletrica(a_entradaSaidaDados, " + a_diretorio_att_operacional + "," + a_diretorio_att_premissa + "): \n" + std::string(erro.what())); }

} // void Dados::validacao_operacional_Hidreletrica(){



void Dados::validacao_operacional_Submercado(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar) {

	try {

		const IdProcesso idProcesso = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());

		bool calcular_atributos_operacionais_submercado = false;
		bool calcular_atributos_operacionais_patamar_deficit = false;
		bool calcular_atributos_operacionais_usinaNaoSimulada = false;

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_estudo_inicial = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();

		IdPatamarCarga maiorIdPatamarCarga_horizonte = IdPatamarCarga_Nenhum;
		for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {
			const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
			if (maiorIdPatamarCarga > maiorIdPatamarCarga_horizonte)
				maiorIdPatamarCarga_horizonte = maiorIdPatamarCarga;
		} // for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

		validaSubmercado();

		const IdSubmercado maiorIdSubmercado = getMaiorId(IdSubmercado());
		for (IdSubmercado idSubmercado = getMenorId(IdSubmercado()); idSubmercado <= maiorIdSubmercado; vetorSubmercado.incr(idSubmercado)) {

			if (getSize1Matriz(idSubmercado, AttMatrizSubmercado_demanda) == 0) {

				calcular_atributos_operacionais_submercado = true;

				if (getSizeVetor(idSubmercado, AttVetorSubmercado_demanda) == 0)
					vetorSubmercado.att(idSubmercado).setVetor(AttVetorSubmercado_demanda, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idSubmercado, AttComumSubmercado_demanda, double())));

				bool preencher_percentual_variacao_patamar_carga = false;
				if (getSize1Matriz(idSubmercado, AttMatrizSubmercado_percentual_variacao_patamar_carga) == 0)
					preencher_percentual_variacao_patamar_carga = true;

				for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

					const double demanda = getElementoVetor(idSubmercado, AttVetorSubmercado_demanda, periodo, double());

					const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
					for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						if (preencher_percentual_variacao_patamar_carga)
							vetorSubmercado.att(idSubmercado).addElemento(AttMatrizSubmercado_percentual_variacao_patamar_carga, periodo, idPatamarCarga, 1.0);

						const double percentual_variacao = getElementoMatriz(idSubmercado, AttMatrizSubmercado_percentual_variacao_patamar_carga, periodo, idPatamarCarga, double());

						vetorSubmercado.att(idSubmercado).addElemento(AttMatrizSubmercado_demanda, periodo, idPatamarCarga, demanda * percentual_variacao);

					} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
				} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

			} // if (getSize1Matriz(idSubmercado, AttMatrizSubmercado_demanda) == 0) {

			for (AttMatrizSubmercado attMatrizSubmercado = AttMatrizSubmercado(1); attMatrizSubmercado < AttMatrizSubmercado_Excedente; attMatrizSubmercado++) {

				const AttVetorSubmercado attVetorSubmercado_media = getAttVetorSubmercadoFromChar(std::string(getString(attMatrizSubmercado) + "_media").c_str());

				if (attVetorSubmercado_media != AttVetorSubmercado_Nenhum) {

					SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz = getMatriz(idSubmercado, attMatrizSubmercado, Periodo(), IdPatamarCarga(), double());

					if (matriz.size() > 0) {

						SmartEnupla<Periodo, double> vetor(matriz, 0.0);

						for (Periodo periodo = matriz.getIteradorInicial(); periodo <= periodo_final_estudo; matriz.incrementarIterador(periodo)) {
							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= matriz.at(periodo).getIteradorFinal(); idPatamarCarga++)
								vetor.at(periodo) += matriz.at(periodo).at(idPatamarCarga) * getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
						}

						vetorSubmercado.att(idSubmercado).setVetor_forced(attVetorSubmercado_media, vetor);

					} // if (matriz.size() > 0) {

				} // if (attVetorSubmercado_media != AttVetorSubmercado_Nenhum) {

			} // for (AttMatrizSubmercado attMatrizSubmercado = AttMatrizSubmercado(1); attMatrizSubmercado < AttMatrizSubmercado_Excedente; attMatrizSubmercado++) {

			const IdPatamarDeficit maiorIdPatamarDeficit = getMaiorId(idSubmercado, IdPatamarDeficit());

			validaPatamarDeficitEmSubmercado(idSubmercado);

			for (IdPatamarDeficit idPatamarDeficit = IdPatamarDeficit_1; idPatamarDeficit <= maiorIdPatamarDeficit; idPatamarDeficit++) {

				if (getSize1Matriz(idSubmercado, idPatamarDeficit, AttMatrizPatamarDeficit_potencia_maxima) == 0) {

					calcular_atributos_operacionais_patamar_deficit = true;

					bool preencher_percentual = false;
					if (getSize1Matriz(idSubmercado, idPatamarDeficit, AttMatrizPatamarDeficit_percentual) == 0) {
						preencher_percentual = true;
						if (getSizeVetor(idSubmercado, idPatamarDeficit, AttVetorPatamarDeficit_percentual) == 0)
							vetorSubmercado.att(idSubmercado).vetorPatamarDeficit.att(idPatamarDeficit).setVetor(AttVetorPatamarDeficit_percentual, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idSubmercado, idPatamarDeficit, AttComumPatamarDeficit_percentual, double())));
					}

					bool preencher_custo = false;
					if (getSize1Matriz(idSubmercado, idPatamarDeficit, AttMatrizPatamarDeficit_custo) == 0) {
						preencher_custo = true;
						if (getSizeVetor(idSubmercado, idPatamarDeficit, AttVetorPatamarDeficit_custo) == 0)
							vetorSubmercado.att(idSubmercado).vetorPatamarDeficit.att(idPatamarDeficit).setVetor(AttVetorPatamarDeficit_custo, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idSubmercado, idPatamarDeficit, AttComumPatamarDeficit_custo, double())));
					}

					for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

						const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
						for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

							if (preencher_percentual)
								vetorSubmercado.att(idSubmercado).vetorPatamarDeficit.att(idPatamarDeficit).addElemento(AttMatrizPatamarDeficit_percentual, periodo, idPatamarCarga, getElementoVetor(idSubmercado, idPatamarDeficit, AttVetorPatamarDeficit_percentual, periodo, double()));

							if (preencher_custo)
								vetorSubmercado.att(idSubmercado).vetorPatamarDeficit.att(idPatamarDeficit).addElemento(AttMatrizPatamarDeficit_custo, periodo, idPatamarCarga, getElementoVetor(idSubmercado, idPatamarDeficit, AttVetorPatamarDeficit_custo, periodo, double()));

							const double demanda = getElementoMatriz(idSubmercado, AttMatrizSubmercado_demanda, periodo, idPatamarCarga, double());
							const double percentual = getElementoMatriz(idSubmercado, idPatamarDeficit, AttMatrizPatamarDeficit_percentual, periodo, idPatamarCarga, double());

							vetorSubmercado.att(idSubmercado).vetorPatamarDeficit.att(idPatamarDeficit).addElemento(AttMatrizPatamarDeficit_potencia_maxima, periodo, idPatamarCarga, demanda * percentual);

						} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

					} // for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

				} // if (getSize1Matriz(idSubmercado, idPatamarDeficit, AttMatrizPatamarDeficit_potencia_maxima) == 0) {

				for (AttMatrizPatamarDeficit attMatrizPatamarDeficit = AttMatrizPatamarDeficit(1); attMatrizPatamarDeficit < AttMatrizPatamarDeficit_Excedente; attMatrizPatamarDeficit++) {

					const AttVetorPatamarDeficit attVetorPatamarDeficit_media = getAttVetorPatamarDeficitFromChar(std::string(getString(attMatrizPatamarDeficit) + "_media").c_str());

					if (attVetorPatamarDeficit_media != AttVetorPatamarDeficit_Nenhum) {

						SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz = getMatriz(idSubmercado, idPatamarDeficit, attMatrizPatamarDeficit, Periodo(), IdPatamarCarga(), double());

						if (matriz.size() > 0) {

							SmartEnupla<Periodo, double> vetor(matriz, 0.0);

							for (Periodo periodo = matriz.getIteradorInicial(); periodo <= periodo_final_estudo; matriz.incrementarIterador(periodo)) {
								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= matriz.at(periodo).getIteradorFinal(); idPatamarCarga++)
									vetor.at(periodo) += matriz.at(periodo).at(idPatamarCarga) * getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
							}

							vetorSubmercado.att(idSubmercado).vetorPatamarDeficit.att(idPatamarDeficit).setVetor_forced(attVetorPatamarDeficit_media, vetor);

						} // if (matriz.size() > 0) {

					} // if (attVetorPatamarDeficit_media != AttVetorPatamarDeficit_Nenhum) {

				} // for (AttMatrizPatamarDeficit attMatrizPatamarDeficit = AttMatrizPatamarDeficit(1); attMatrizPatamarDeficit < AttMatrizPatamarDeficit_Excedente; attMatrizPatamarDeficit++) {

			} // for (IdPatamarDeficit idPatamarDeficit = IdPatamarDeficit_1; idPatamarDeficit <= maiorIdPatamarDeficit; idPatamarDeficit++) {

			validaUsinaNaoSimuladaEmSubmercado(idSubmercado);

			const IdUsinaNaoSimulada maiorIdUsinaNaoSimulada = getMaiorId(idSubmercado, IdUsinaNaoSimulada());
			for (IdUsinaNaoSimulada idUsinaNaoSimulada = IdUsinaNaoSimulada(1); idUsinaNaoSimulada <= maiorIdUsinaNaoSimulada; idUsinaNaoSimulada++) {

				if (vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.isInstanciado(idUsinaNaoSimulada)) {

					if ((getSize1Matriz(idSubmercado, idUsinaNaoSimulada, AttMatrizUsinaNaoSimulada_potencia_minima) == 0) || (getSize1Matriz(idSubmercado, idUsinaNaoSimulada, AttMatrizUsinaNaoSimulada_potencia_maxima) == 0)) {

						calcular_atributos_operacionais_usinaNaoSimulada = true;

						bool preencher_percentual_variacao_patamar_carga = false;
						if (getSize1Matriz(idSubmercado, idUsinaNaoSimulada, AttMatrizUsinaNaoSimulada_percentual_variacao_patamar_carga) == 0)
							preencher_percentual_variacao_patamar_carga = true;

						bool preencher_potencia_minima = false;
						if (getSize1Matriz(idSubmercado, idUsinaNaoSimulada, AttMatrizUsinaNaoSimulada_potencia_minima) == 0) {
							preencher_potencia_minima = true;
							if (getSizeVetor(idSubmercado, idUsinaNaoSimulada, AttVetorUsinaNaoSimulada_potencia_minima) == 0)
								vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).setVetor(AttVetorUsinaNaoSimulada_potencia_minima, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idSubmercado, idUsinaNaoSimulada, AttComumUsinaNaoSimulada_potencia_minima, double())));
						}

						bool preencher_potencia_maxima = false;
						if (getSize1Matriz(idSubmercado, idUsinaNaoSimulada, AttMatrizUsinaNaoSimulada_potencia_maxima) == 0) {
							preencher_potencia_maxima = true;
							if (getSizeVetor(idSubmercado, idUsinaNaoSimulada, AttVetorUsinaNaoSimulada_potencia_maxima) == 0)
								vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).setVetor(AttVetorUsinaNaoSimulada_potencia_maxima, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idSubmercado, idUsinaNaoSimulada, AttComumUsinaNaoSimulada_potencia_maxima, double())));
						}

						for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

							const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

								const double percentual_variacao_patamar_carga = getElementoMatriz(idSubmercado, idUsinaNaoSimulada, AttMatrizUsinaNaoSimulada_percentual_variacao_patamar_carga, periodo, idPatamarCarga, double());

								if (preencher_potencia_minima)
									vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).addElemento(AttMatrizUsinaNaoSimulada_potencia_minima, periodo, idPatamarCarga, getElementoVetor(idSubmercado, idUsinaNaoSimulada, AttVetorUsinaNaoSimulada_potencia_minima, periodo, double()) * percentual_variacao_patamar_carga);

								if (preencher_potencia_maxima)
									vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).addElemento(AttMatrizUsinaNaoSimulada_potencia_maxima, periodo, idPatamarCarga, getElementoVetor(idSubmercado, idUsinaNaoSimulada, AttVetorUsinaNaoSimulada_potencia_maxima, periodo, double()) * percentual_variacao_patamar_carga);

								const double potencia_minima = getElementoMatriz(idSubmercado, idUsinaNaoSimulada, AttMatrizUsinaNaoSimulada_potencia_minima, periodo, idPatamarCarga, double());
								const double potencia_maxima = getElementoMatriz(idSubmercado, idUsinaNaoSimulada, AttMatrizUsinaNaoSimulada_potencia_maxima, periodo, idPatamarCarga, double());

								if (potencia_minima > potencia_maxima)
									throw std::invalid_argument("Potencia Minima maior que Potencia Maxima em " + getString(periodo) + " em " + getFullString(idPatamarCarga) + " em " + getFullString(idSubmercado) + " em " + getFullString(idUsinaNaoSimulada));

							} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						} // for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

					} // if ((getSize1Matriz(idSubmercado, idUsinaNaoSimulada, AttMatrizUsinaNaoSimulada_potencia_minima) == 0) || (getSize1Matriz(idSubmercado, idUsinaNaoSimulada, AttMatrizUsinaNaoSimulada_potencia_maxima) == 0)) {

					for (AttMatrizUsinaNaoSimulada attMatrizUsinaNaoSimulada = AttMatrizUsinaNaoSimulada(1); attMatrizUsinaNaoSimulada < AttMatrizUsinaNaoSimulada_Excedente; attMatrizUsinaNaoSimulada++) {

						const AttVetorUsinaNaoSimulada attVetorUsinaNaoSimulada_media = getAttVetorUsinaNaoSimuladaFromChar(std::string(getString(attMatrizUsinaNaoSimulada) + "_media").c_str());

						if (attVetorUsinaNaoSimulada_media != AttVetorUsinaNaoSimulada_Nenhum) {

							SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz = getMatriz(idSubmercado, idUsinaNaoSimulada, attMatrizUsinaNaoSimulada, Periodo(), IdPatamarCarga(), double());

							if (matriz.size() > 0) {

								SmartEnupla<Periodo, double> vetor(matriz, 0.0);

								for (Periodo periodo = matriz.getIteradorInicial(); periodo <= periodo_final_estudo; matriz.incrementarIterador(periodo)) {
									for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= matriz.at(periodo).getIteradorFinal(); idPatamarCarga++)
										vetor.at(periodo) += matriz.at(periodo).at(idPatamarCarga) * getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
								}

								vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).setVetor_forced(attVetorUsinaNaoSimulada_media, vetor);

							} // if (matriz.size() > 0) {

						} // if (attVetorUsinaNaoSimulada_media != AttVetorUsinaNaoSimulada_Nenhum) {

					} // for (AttMatrizUsinaNaoSimulada attMatrizUsinaNaoSimulada = AttMatrizUsinaNaoSimulada(1); attMatrizUsinaNaoSimulada < AttMatrizUsinaNaoSimulada_Excedente; attMatrizUsinaNaoSimulada++) {

				} // if (vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.isInstanciado(idUsinaNaoSimulada)){

			} // for (IdUsinaNaoSimulada idUsinaNaoSimulada = IdUsinaNaoSimulada_1; idUsinaNaoSimulada <= maiorIdUsinaNaoSimulada; idUsinaNaoSimulada++) {

		} // for (IdSubmercado idSubmercado = getMenorId(IdSubmercado()); idSubmercado <= maiorIdSubmercado; vetorSubmercado.incr(idSubmercado)) {

		if (idProcesso == IdProcesso_mestre) {

			if (true) {
				bool impresso = false;
				for (AttMatrizSubmercado attMatrizSubmercado = AttMatrizSubmercado(1); attMatrizSubmercado < AttMatrizSubmercado_Excedente; attMatrizSubmercado++) {

					const AttVetorSubmercado attVetorSubmercado_media = getAttVetorSubmercadoFromChar(std::string(getString(attMatrizSubmercado) + "_media").c_str());

					if (attVetorSubmercado_media != AttVetorSubmercado_Nenhum) {

						a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

						a_entradaSaidaDados.setAppendArquivo(impresso);
						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_SUBMERCADO_AttVetorPremissa_PorPeriodo.csv", IdSubmercado_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, attVetorSubmercado_media);
						impresso = true;

						for (IdSubmercado idSubmercado = getMenorId(IdSubmercado()); idSubmercado <= maiorIdSubmercado; vetorSubmercado.incr(idSubmercado))
							vetorSubmercado.att(idSubmercado).clear(attVetorSubmercado_media);

					}

				} // for (AttMatrizSubmercado attMatrizSubmercado = AttMatrizSubmercado(1); attMatrizSubmercado < AttMatrizSubmercado_Nenhum; attMatrizSubmercado++) {
			} // if (true) {

			if (true) {
				bool impresso = false;
				for (AttMatrizPatamarDeficit attMatrizPatamarDeficit = AttMatrizPatamarDeficit(1); attMatrizPatamarDeficit < AttMatrizPatamarDeficit_Excedente; attMatrizPatamarDeficit++) {

					const AttVetorPatamarDeficit attVetorPatamarDeficit_media = getAttVetorPatamarDeficitFromChar(std::string(getString(attMatrizPatamarDeficit) + "_media").c_str());

					if (attVetorPatamarDeficit_media != AttVetorPatamarDeficit_Nenhum) {

						a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);
						a_entradaSaidaDados.setAppendArquivo(impresso);
						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_SUBMERCADO_PATAMAR_DEFICIT_AttVetorPremissa_PorPeriodo.csv", IdSubmercado_Nenhum, IdPatamarDeficit_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, attVetorPatamarDeficit_media);
						impresso = true;

						for (IdSubmercado idSubmercado = getMenorId(IdSubmercado()); idSubmercado <= maiorIdSubmercado; vetorSubmercado.incr(idSubmercado))
							for (IdPatamarDeficit idPatamarDeficit = IdPatamarDeficit_1; idPatamarDeficit <= getMaiorId(idSubmercado, IdPatamarDeficit()); idPatamarDeficit++)
								vetorSubmercado.att(idSubmercado).vetorPatamarDeficit.att(idPatamarDeficit).clear(attVetorPatamarDeficit_media);

					}

				} // for (AttMatrizPatamarDeficit attMatrizPatamarDeficit = AttMatrizPatamarDeficit(1); attMatrizPatamarDeficit < AttMatrizPatamarDeficit_Nenhum; attMatrizPatamarDeficit++) {
			} // if (true) {

			if (true) {
				bool impresso = false;
				for (AttMatrizUsinaNaoSimulada attMatrizUsinaNaoSimulada = AttMatrizUsinaNaoSimulada(1); attMatrizUsinaNaoSimulada < AttMatrizUsinaNaoSimulada_Excedente; attMatrizUsinaNaoSimulada++) {

					const AttVetorUsinaNaoSimulada attVetorUsinaNaoSimulada_media = getAttVetorUsinaNaoSimuladaFromChar(std::string(getString(attMatrizUsinaNaoSimulada) + "_media").c_str());

					if (attVetorUsinaNaoSimulada_media != AttVetorUsinaNaoSimulada_Nenhum) {

						a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

						a_entradaSaidaDados.setAppendArquivo(impresso);
						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_SUBMERCADO_USINA_NAO_SIMULADA_AttVetorPremissa_PorPeriodo.csv", IdSubmercado_Nenhum, IdUsinaNaoSimulada_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, attVetorUsinaNaoSimulada_media);
						impresso = true;

						for (IdSubmercado idSubmercado = getMenorId(IdSubmercado()); idSubmercado <= maiorIdSubmercado; vetorSubmercado.incr(idSubmercado)) {
							for (IdUsinaNaoSimulada idUsinaNaoSimulada = IdUsinaNaoSimulada_1; idUsinaNaoSimulada <= getMaiorId(idSubmercado, IdUsinaNaoSimulada()); idUsinaNaoSimulada++) {
								if (vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.isInstanciado(idUsinaNaoSimulada))
									vetorSubmercado.att(idSubmercado).vetorUsinaNaoSimulada.att(idUsinaNaoSimulada).clear(attVetorUsinaNaoSimulada_media);
							}
						}

					} // if (attVetorUsinaNaoSimulada_media != AttVetorUsinaNaoSimulada_Nenhum) {

				} // for (AttMatrizUsinaNaoSimulada attMatrizUsinaNaoSimulada = AttMatrizUsinaNaoSimulada(1); attMatrizUsinaNaoSimulada < AttMatrizUsinaNaoSimulada_Nenhum; attMatrizUsinaNaoSimulada++) {
			} // if (true) {

		} // if (idProcesso == IdProcesso_mestre) {

		if ((calcular_atributos_operacionais_submercado) || (calcular_atributos_operacionais_patamar_deficit) || (calcular_atributos_operacionais_usinaNaoSimulada) || (a_imprimir_atributos_sem_recarregar)) {

			if (idProcesso == IdProcesso_mestre) {

				if (calcular_atributos_operacionais_patamar_deficit) {

					//
					// Imprime Atributos Premissas
					//
					a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

					a_entradaSaidaDados.setAppendArquivo(false);
					a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("SUBMERCADO_PATAMAR_DEFICIT_AttVetorPremissa_PorPeriodo.csv", IdSubmercado_Nenhum, IdPatamarDeficit_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, AttVetorPatamarDeficit_percentual);
					a_entradaSaidaDados.setAppendArquivo(true);
					a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("SUBMERCADO_PATAMAR_DEFICIT_AttVetorPremissa_PorPeriodo.csv", IdSubmercado_Nenhum, IdPatamarDeficit_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, AttVetorPatamarDeficit_custo);

				} // if (calcular_atributos_operacionais_patamar_deficit) {

				if (calcular_atributos_operacionais_submercado) {

					//
					// Imprime Atributos Premissas
					//
					a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

					a_entradaSaidaDados.setAppendArquivo(false);
					a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("SUBMERCADO_AttVetorPremissa_PorPeriodo.csv", IdSubmercado_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, AttVetorSubmercado_demanda);

					a_entradaSaidaDados.setAppendArquivo(false);
					a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("SUBMERCADO_AttMatrizPremissa_PorPeriodoPorIdPatamarCarga.csv", IdSubmercado_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizSubmercado_percentual_variacao_patamar_carga);

				} // if (calcular_atributos_operacionais_submercado) {

				if (calcular_atributos_operacionais_usinaNaoSimulada) {

					//
					// Imprime Atributos Premissas
					//
					a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

					a_entradaSaidaDados.setAppendArquivo(false);
					a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("SUBMERCADO_USINA_NAO_SIMULADA_AttVetorPremissa_PorPeriodo.csv", IdSubmercado_Nenhum, IdUsinaNaoSimulada_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, AttVetorUsinaNaoSimulada_potencia_minima);
					a_entradaSaidaDados.setAppendArquivo(true);
					a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("SUBMERCADO_USINA_NAO_SIMULADA_AttVetorPremissa_PorPeriodo.csv", IdSubmercado_Nenhum, IdUsinaNaoSimulada_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, AttVetorUsinaNaoSimulada_potencia_maxima);

					a_entradaSaidaDados.setAppendArquivo(false);
					a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("SUBMERCADO_USINA_NAO_SIMULADA_AttMatrizPremissa_PorPeriodoPorIdPatamarCarga.csv", IdSubmercado_Nenhum, IdUsinaNaoSimulada_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizUsinaNaoSimulada_percentual_variacao_patamar_carga);

				} // if (calcular_atributos_operacionais_usinaNaoSimulada) {


				//
				// Imprime Atributos Operacionais
				//
				a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

				a_entradaSaidaDados.setAppendArquivo(false);
				a_entradaSaidaDados.imprimirArquivoCSV_AttComum("SUBMERCADO_AttComumOperacional.csv", IdSubmercado_Nenhum, *this, std::vector<AttComumSubmercado> {AttComumSubmercado_idSubmercado, AttComumSubmercado_nome });

				a_entradaSaidaDados.setAppendArquivo(false);
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("SUBMERCADO_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", IdSubmercado_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizSubmercado_demanda);

				a_entradaSaidaDados.setAppendArquivo(false);
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("SUBMERCADO_PATAMAR_DEFICIT_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", IdSubmercado_Nenhum, IdPatamarDeficit_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizPatamarDeficit_custo);
				a_entradaSaidaDados.setAppendArquivo(true);
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("SUBMERCADO_PATAMAR_DEFICIT_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", IdSubmercado_Nenhum, IdPatamarDeficit_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizPatamarDeficit_potencia_maxima);

				a_entradaSaidaDados.setAppendArquivo(false);
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("SUBMERCADO_USINA_NAO_SIMULADA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", IdSubmercado_Nenhum, IdUsinaNaoSimulada_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizUsinaNaoSimulada_potencia_minima);
				a_entradaSaidaDados.setAppendArquivo(true);
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("SUBMERCADO_USINA_NAO_SIMULADA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", IdSubmercado_Nenhum, IdUsinaNaoSimulada_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizUsinaNaoSimulada_potencia_maxima);

			} // if (idProcesso == IdProcesso_mestre){

			if (!a_imprimir_atributos_sem_recarregar) {

				if (true) {
					int barreira = 0;

					if (arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso()) == IdProcesso_mestre) {
						for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++)
							MPI_Send(&barreira, 1, MPI_INT, getRank(idProcesso), 0, MPI_COMM_WORLD);
					}
					else
						MPI_Recv(&barreira, 1, MPI_INT, getRank(IdProcesso_mestre), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}

				//
				// Esvazia Atributos
				//

				vetorSubmercado = VetorSubmercadoEmDados();

				//
				// Recarrega Atributos Operacionais
				//		
				a_entradaSaidaDados.setDiretorioEntrada(a_diretorio_att_operacional);

				a_entradaSaidaDados.carregarArquivoCSV_AttComum("SUBMERCADO_AttComumOperacional.csv", *this, TipoAcessoInstancia_membro);

				a_entradaSaidaDados.carregarArquivoCSV_AttMatriz("SUBMERCADO_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);

				a_entradaSaidaDados.carregarArquivoCSV_AttMatriz("SUBMERCADO_PATAMAR_DEFICIT_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membroMembro);

				a_entradaSaidaDados.carregarArquivoCSV_AttMatriz("SUBMERCADO_USINA_NAO_SIMULADA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membroMembro);

			} // if (!a_imprimir_atributos_sem_recarregar) {

		} // if ((calcular_atributos_operacionais_submercado) || (calcular_atributos_operacionais_patamar_deficit) || (calcular_atributos_operacionais_usinaNaoSimulada) || (a_imprimir_atributos_sem_recarregar)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validacao_operacional_Submercado(a_entradaSaidaDados, " + a_diretorio_att_operacional + "," + a_diretorio_att_premissa + "): \n" + std::string(erro.what())); }

} // void Dados::validacao_operacional_Submercado(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar){



void Dados::validacao_operacional_Intercambio(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar) {

	try {

		const IdProcesso idProcesso = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());

		bool calcular_att_operacionais_intercambio = false;

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_estudo_inicial = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();

		IdPatamarCarga maiorIdPatamarCarga_horizonte = IdPatamarCarga_Nenhum;
		for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {
			const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
			if (maiorIdPatamarCarga > maiorIdPatamarCarga_horizonte)
				maiorIdPatamarCarga_horizonte = maiorIdPatamarCarga;
		} // for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

		validaIntercambio();

		const IdIntercambio maiorIdIntercambio = getMaiorId(IdIntercambio());
		for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= maiorIdIntercambio; idIntercambio++) {

			if ((getSize1Matriz(idIntercambio, AttMatrizIntercambio_potencia_minima) == 0) || (getSize1Matriz(idIntercambio, AttMatrizIntercambio_potencia_maxima) == 0)) {

				calcular_att_operacionais_intercambio = true;

				bool preencher_potencia_minima = false;
				if (getSize1Matriz(idIntercambio, AttMatrizIntercambio_potencia_minima) == 0) {
					preencher_potencia_minima = true;
					if (getSizeVetor(idIntercambio, AttVetorIntercambio_potencia_minima) == 0)
						vetorIntercambio.att(idIntercambio).setVetor(AttVetorIntercambio_potencia_minima, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idIntercambio, AttComumIntercambio_potencia_minima, double())));
				}

				bool preencher_potencia_maxima = false;
				if (getSize1Matriz(idIntercambio, AttMatrizIntercambio_potencia_maxima) == 0) {
					preencher_potencia_maxima = true;
					if (getSizeVetor(idIntercambio, AttVetorIntercambio_potencia_maxima) == 0)
						vetorIntercambio.att(idIntercambio).setVetor(AttVetorIntercambio_potencia_maxima, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idIntercambio, AttComumIntercambio_potencia_maxima, double())));
				}

				bool preencher_percentual_variacao_patamar_carga = false;
				if (getSize1Matriz(idIntercambio, AttMatrizIntercambio_percentual_variacao_patamar_carga) == 0)
					preencher_percentual_variacao_patamar_carga = true;

				for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

					const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
					for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						if (preencher_percentual_variacao_patamar_carga)
							vetorIntercambio.att(idIntercambio).addElemento(AttMatrizIntercambio_percentual_variacao_patamar_carga, periodo, idPatamarCarga, 1.0);

						const double percentual_variacao = getElementoMatriz(idIntercambio, AttMatrizIntercambio_percentual_variacao_patamar_carga, periodo, idPatamarCarga, double());

						if (preencher_potencia_minima)
							vetorIntercambio.att(idIntercambio).addElemento(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, getElementoVetor(idIntercambio, AttVetorIntercambio_potencia_minima, periodo, double()) * percentual_variacao);

						if (preencher_potencia_maxima)
							vetorIntercambio.att(idIntercambio).addElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, getElementoVetor(idIntercambio, AttVetorIntercambio_potencia_maxima, periodo, double()) * percentual_variacao);

						const double potencia_minima = getElementoMatriz(idIntercambio, AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, double());
						const double potencia_maxima = getElementoMatriz(idIntercambio, AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, double());

						if (potencia_minima > potencia_maxima)
							throw std::invalid_argument("Potencia Minima maior que Potencia Maxima em " + getString(periodo) + " em " + getString(idPatamarCarga) + " em " + getString(idIntercambio));

					} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
				} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

			} // if (getSize1Matriz(idIntercambio, AttMatrizIntercambio_potencia_minima) == 0) || (getSize1Matriz(idIntercambio, AttMatrizIntercambio_potencia_maxima) == 0) {

			//Teste restriçoes elétricas simples
			std::vector<IdRestricaoEletrica> vetor_idRestricaoEletrica_comINT;

			if (getAtributo(AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_eletricas, bool())) {
				for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= getMaiorId(IdRestricaoEletrica()); idRestricaoEletrica++) {
					if ((getAtributo(idRestricaoEletrica, IdElementoSistema_1, AttComumElementoSistema_intercambio, IdIntercambio()) == idIntercambio) && (isRestricaoEletrica_simples(idRestricaoEletrica)))
						vetor_idRestricaoEletrica_comINT.push_back(idRestricaoEletrica);
				}
			} // if (getAtributo(AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_eletricas, bool())) {

			if (int(vetor_idRestricaoEletrica_comINT.size()) > 0) {

				for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

					const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

					for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						for (int pos = 0; pos < int(vetor_idRestricaoEletrica_comINT.size()); pos++) {

							//potencia_disponivel_minima
							if (getElementoMatriz(vetor_idRestricaoEletrica_comINT.at(pos), AttMatrizRestricaoEletrica_potencia_minima, periodo, idPatamarCarga, double()) > getElementoMatriz(idIntercambio, AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, double()))
								vetorIntercambio.att(idIntercambio).setElemento(AttMatrizIntercambio_potencia_minima, periodo, idPatamarCarga, getElementoMatriz(vetor_idRestricaoEletrica_comINT.at(pos), AttMatrizRestricaoEletrica_potencia_minima, periodo, idPatamarCarga, double()));

							//potencia_disponivel_maxima
							if (getElementoMatriz(vetor_idRestricaoEletrica_comINT.at(pos), AttMatrizRestricaoEletrica_potencia_maxima, periodo, idPatamarCarga, double()) < getElementoMatriz(idIntercambio, AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, double()))
								vetorIntercambio.att(idIntercambio).setElemento(AttMatrizIntercambio_potencia_maxima, periodo, idPatamarCarga, getElementoMatriz(vetor_idRestricaoEletrica_comINT.at(pos), AttMatrizRestricaoEletrica_potencia_maxima, periodo, idPatamarCarga, double()));

						}//for (int pos = 0; pos < int(vetor_idRestricaoEletrica_comUTE.size()); pos++) {

					}//for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga_horizonte; idPatamarCarga++) {

				}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

			}//if (int(vetor_idRestricaoEletrica_comUHE.size()) > 0) {


			for (AttMatrizIntercambio attMatrizIntercambio = AttMatrizIntercambio(1); attMatrizIntercambio < AttMatrizIntercambio_Excedente; attMatrizIntercambio++) {

				const AttVetorIntercambio attVetorIntercambio_media = getAttVetorIntercambioFromChar(std::string(getString(attMatrizIntercambio) + "_media").c_str());

				if (attVetorIntercambio_media != AttVetorIntercambio_Nenhum) {

					SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz = getMatriz(idIntercambio, attMatrizIntercambio, Periodo(), IdPatamarCarga(), double());

					if (matriz.size() > 0) {

						SmartEnupla<Periodo, double> vetor(matriz, 0.0);

						for (Periodo periodo = matriz.getIteradorInicial(); periodo <= periodo_final_estudo; matriz.incrementarIterador(periodo)) {
							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= matriz.at(periodo).getIteradorFinal(); idPatamarCarga++)
								vetor.at(periodo) += matriz.at(periodo).at(idPatamarCarga) * getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
						}

						vetorIntercambio.att(idIntercambio).setVetor_forced(attVetorIntercambio_media, vetor);

					} // if (matriz.size() > 0){

				} // if (attVetorIntercambio_media != AttVetorIntercambio_Nenhum) {

			} // for (AttMatrizIntercambio attMatrizIntercambio = AttMatrizIntercambio(1); attMatrizIntercambio < AttMatrizIntercambio_Excedente; attMatrizIntercambio++) {

		} // for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= maiorIdIntercambio; idIntercambio++) {

		if (idProcesso == IdProcesso_mestre) {

			bool impresso = false;
			for (AttMatrizIntercambio attMatrizIntercambio = AttMatrizIntercambio(1); attMatrizIntercambio < AttMatrizIntercambio_Excedente; attMatrizIntercambio++) {

				const AttVetorIntercambio attVetorIntercambio_media = getAttVetorIntercambioFromChar(std::string(getString(attMatrizIntercambio) + "_media").c_str());

				if (attVetorIntercambio_media != AttVetorIntercambio_Nenhum) {

					a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

					a_entradaSaidaDados.setAppendArquivo(impresso);
					a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_INTERCAMBIO_AttVetorPremissa_PorPeriodo.csv", IdIntercambio_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, attVetorIntercambio_media);
					impresso = true;

					for (IdIntercambio idIntercambio = IdIntercambio_1; idIntercambio <= maiorIdIntercambio; idIntercambio++)
						vetorIntercambio.att(idIntercambio).clear(attVetorIntercambio_media);
				}

			} // for (AttMatrizIntercambio attMatrizIntercambio = AttMatrizIntercambio(1); attMatrizIntercambio < AttMatrizIntercambio_Nenhum; attMatrizIntercambio++) {

		} // if (idProcesso == IdProcesso_mestre) {

		if ((calcular_att_operacionais_intercambio) || (a_imprimir_atributos_sem_recarregar)) {

			if (idProcesso == IdProcesso_mestre) {

				if (calcular_att_operacionais_intercambio) {

					//
					// Imprime Atributos Premissas
					//
					a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

					a_entradaSaidaDados.setAppendArquivo(false);
					a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("INTERCAMBIO_AttVetorPremissa_PorPeriodo.csv", IdIntercambio_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, AttVetorIntercambio_potencia_minima);
					a_entradaSaidaDados.setAppendArquivo(true);
					a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("INTERCAMBIO_AttVetorPremissa_PorPeriodo.csv", IdIntercambio_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, AttVetorIntercambio_potencia_maxima);

					a_entradaSaidaDados.setAppendArquivo(false);
					a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("INTERCAMBIO_AttMatrizPremissa_PorPeriodoPorIdPatamarCarga.csv", IdIntercambio_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizIntercambio_percentual_variacao_patamar_carga);


				} // if (calcular_att_operacionais_intercambio) {

				//
				// Imprime Atributos Operacionais
				//
				a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

				a_entradaSaidaDados.setAppendArquivo(false);
				a_entradaSaidaDados.imprimirArquivoCSV_AttComum("INTERCAMBIO_AttComumOperacional.csv", IdIntercambio_Nenhum, *this, std::vector<AttComumIntercambio> {AttComumIntercambio_idIntercambio, AttComumIntercambio_nome, AttComumIntercambio_submercado_origem, AttComumIntercambio_submercado_destino, AttComumIntercambio_penalidade_intercambio, AttComumIntercambio_penalidade_intercambio_minimo});

				a_entradaSaidaDados.setAppendArquivo(false);
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("INTERCAMBIO_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", IdIntercambio_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizIntercambio_potencia_minima);
				a_entradaSaidaDados.setAppendArquivo(true);
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("INTERCAMBIO_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", IdIntercambio_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizIntercambio_potencia_maxima);


			} // if (idProcesso == IdProcesso_mestre){

			if (!a_imprimir_atributos_sem_recarregar) {

				if (true) {
					int barreira = 0;

					if (arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso()) == IdProcesso_mestre) {
						for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++)
							MPI_Send(&barreira, 1, MPI_INT, getRank(idProcesso), 0, MPI_COMM_WORLD);
					}
					else
						MPI_Recv(&barreira, 1, MPI_INT, getRank(IdProcesso_mestre), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}

				//
				// Esvazia Atributos
				//

				vetorIntercambio = VetorIntercambioEmDados();

				//
				// Recarrega Atributos Operacionais
				//		
				a_entradaSaidaDados.setDiretorioEntrada(a_diretorio_att_operacional);

				a_entradaSaidaDados.carregarArquivoCSV_AttComum("INTERCAMBIO_AttComumOperacional.csv", *this, TipoAcessoInstancia_membro);

				a_entradaSaidaDados.carregarArquivoCSV_AttMatriz("INTERCAMBIO_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);

			} // if (!a_imprimir_atributos_sem_recarregar) {

		} // if ((calcular_att_operacionais_intercambio) || (a_imprimir_atributos_sem_recarregar)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validacao_operacional_Intercambio(a_entradaSaidaDados, " + a_diretorio_att_operacional + "," + a_diretorio_att_premissa + "): \n" + std::string(erro.what())); }

} // void Dados::validacao_operacional_Intercambio(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar){


void Dados::validacao_operacional_BaciaHidrografica(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar) {

	try {

		const IdProcesso idProcesso = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());

		validaBaciaHidrografica();

		if (a_imprimir_atributos_sem_recarregar) {

			if (idProcesso == IdProcesso_mestre) {

				//
				// Imprime Atributos Operacionais
				//
				a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

				a_entradaSaidaDados.setAppendArquivo(false);

				a_entradaSaidaDados.imprimirArquivoCSV_AttComum("BACIA_HIDROGRAFICA_AttComumOperacional.csv", IdBaciaHidrografica_Nenhum, *this, std::vector<AttComumBaciaHidrografica> {AttComumBaciaHidrografica_idBaciaHidrografica, AttComumBaciaHidrografica_nome});

			} // if (idProcesso == IdProcesso_mestre){

			if (!a_imprimir_atributos_sem_recarregar) {

				if (true) {
					int barreira = 0;

					if (arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso()) == IdProcesso_mestre) {
						for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++)
							MPI_Send(&barreira, 1, MPI_INT, getRank(idProcesso), 0, MPI_COMM_WORLD);
					}
					else
						MPI_Recv(&barreira, 1, MPI_INT, getRank(IdProcesso_mestre), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}

				//
				// Esvazia Atributos
				//

				vetorBaciaHidrografica = VetorBaciaHidrograficaEmDados();

				//
				// Recarrega Atributos Operacionais
				//		
				a_entradaSaidaDados.setDiretorioEntrada(a_diretorio_att_operacional);

				a_entradaSaidaDados.carregarArquivoCSV_AttComum("BACIA_HIDROGRAFICA_AttComumOperacional.csv", *this, TipoAcessoInstancia_membro);

			} // if (!a_imprimir_atributos_sem_recarregar) {

		} // if ((calcular_att_operacionais_restricao_eletrica) || (a_imprimir_atributos_sem_recarregar)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validacao_operacional_BaciaHidrografica(a_entradaSaidaDados, " + a_diretorio_att_operacional + "," + a_diretorio_att_premissa + "): \n" + std::string(erro.what())); }

} // void Dados::validacao_operacional_BaciaHidrografica(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar){


void Dados::validacao_operacional_Contrato(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar) {

	try {

		const IdProcesso idProcesso = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_estudo_inicial = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();

		IdPatamarCarga maiorIdPatamarCarga_horizonte = IdPatamarCarga_Nenhum;
		for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {
			const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
			if (maiorIdPatamarCarga > maiorIdPatamarCarga_horizonte)
				maiorIdPatamarCarga_horizonte = maiorIdPatamarCarga;
		} // for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

		validaContrato();



		if (idProcesso == IdProcesso_mestre) {

			IdContrato maiorIdContrato = getMaiorId(IdContrato());
			//
			// Imprime Atributos Operacionais
			//
			if (maiorIdContrato != IdContrato_Nenhum) {
				a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

				a_entradaSaidaDados.setAppendArquivo(false);
				a_entradaSaidaDados.imprimirArquivoCSV_AttComum("CONTRATO_AttComumOperacional.csv", IdContrato_Nenhum, *this);

				a_entradaSaidaDados.setAppendArquivo(false);
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("CONTRATO_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", IdContrato_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizContrato_energia_minima);
				a_entradaSaidaDados.setAppendArquivo(true);
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("CONTRATO_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", IdContrato_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizContrato_energia_maxima);
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("CONTRATO_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", IdContrato_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizContrato_preco_energia_imp_exp);

				a_entradaSaidaDados.setAppendArquivo(false);
				a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("CONTRATO_AttVetorOperacional_PorPeriodo.csv", IdContrato_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, AttVetorContrato_variacao_decrescimo_maxima);
				a_entradaSaidaDados.setAppendArquivo(true);
				a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("CONTRATO_AttVetorOperacional_PorPeriodo.csv", IdContrato_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, AttVetorContrato_variacao_acrescimo_maxima);
			}
		} // if (idProcesso == IdProcesso_mestre){




	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validacao_operacional_RestricaoEletrica(a_entradaSaidaDados, " + a_diretorio_att_operacional + "," + a_diretorio_att_premissa + "): \n" + std::string(erro.what())); }

} // void Dados::validacao_operacional_RestricaoEletrica(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar){


void Dados::validacao_operacional_DemandaEspecial(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar) {

	try {

		const IdProcesso idProcesso = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());
		const IdDemandaEspecial maiorIdDemandaEspecial = getMaiorId(IdDemandaEspecial());
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_estudo_inicial = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();

		IdPatamarCarga maiorIdPatamarCarga_horizonte = IdPatamarCarga_Nenhum;
		for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {
			const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
			if (maiorIdPatamarCarga > maiorIdPatamarCarga_horizonte)
				maiorIdPatamarCarga_horizonte = maiorIdPatamarCarga;
		} // for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {


		for (IdDemandaEspecial idDemandaEspecial = IdDemandaEspecial_1; idDemandaEspecial <= maiorIdDemandaEspecial; idDemandaEspecial++) {
			if (getSize1Matriz(idDemandaEspecial, AttMatrizDemandaEspecial_demanda) == 0)
				throw std::invalid_argument("Necessario informar a demanda em " + getFullString(idDemandaEspecial));
		}

		if (idProcesso == IdProcesso_mestre) {

			//
			// Imprime Atributos Operacionais
			//
			if (maiorIdDemandaEspecial != IdDemandaEspecial_Nenhum) {

				a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

				a_entradaSaidaDados.setAppendArquivo(false);
				a_entradaSaidaDados.imprimirArquivoCSV_AttComum("DEMANDA_ESPECIAL_AttComumOperacional.csv", IdDemandaEspecial_Nenhum, *this);

				a_entradaSaidaDados.setAppendArquivo(false);
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("DEMANDA_ESPECIAL_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", IdDemandaEspecial_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizDemandaEspecial_demanda);
			}
		} // if (idProcesso == IdProcesso_mestre){

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validacao_operacional_DemandaEspecial(a_entradaSaidaDados, " + a_diretorio_att_operacional + "," + a_diretorio_att_premissa + "): \n" + std::string(erro.what())); }

} // void Dados::validacao_operacional_RestricaoEletrica(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar){


void Dados::validacao_operacional_Eolica(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar) {

	try {

		const IdProcesso idProcesso = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());
		const IdUsinaEolica maiorIdUsinaEolica = getMaiorId(IdUsinaEolica());
		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_estudo_inicial = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();

		IdPatamarCarga maiorIdPatamarCarga_horizonte = IdPatamarCarga_Nenhum;
		for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {
			const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
			if (maiorIdPatamarCarga > maiorIdPatamarCarga_horizonte)
				maiorIdPatamarCarga_horizonte = maiorIdPatamarCarga;
		} // for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {


		for (IdUsinaEolica idUsinaEolica = IdUsinaEolica_1; idUsinaEolica <= maiorIdUsinaEolica; idUsinaEolica++) {
			if (getSize1Matriz(idUsinaEolica, AttMatrizUsinaEolica_geracao) == 0)
				throw std::invalid_argument("Necessario informar a geracao em " + getFullString(idUsinaEolica));
		}

		if (idProcesso == IdProcesso_mestre) {

			//
			// Imprime Atributos Operacionais
			//
			if (maiorIdUsinaEolica != IdUsinaEolica_Nenhum) {

				a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

				a_entradaSaidaDados.setAppendArquivo(false);
				a_entradaSaidaDados.imprimirArquivoCSV_AttComum("USINA_EOLICA_AttComumOperacional.csv", IdUsinaEolica_Nenhum, *this);

				a_entradaSaidaDados.setAppendArquivo(false);
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("USINA_EOLICA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", IdUsinaEolica_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizUsinaEolica_geracao);
			}
		} // if (idProcesso == IdProcesso_mestre){

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validacao_operacional_Eolica(a_entradaSaidaDados, " + a_diretorio_att_operacional + "," + a_diretorio_att_premissa + "): \n" + std::string(erro.what())); }

} // void Dados::validacao_operacional_RestricaoEletrica(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar){




void Dados::validacao_operacional_RestricaoEletrica(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar) {

	try {

		const IdProcesso idProcesso = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());

		bool calcular_att_operacionais_restricao_eletrica = false;
		bool calcular_att_operacionais_elemento_sistema = false;

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_estudo_inicial = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();

		IdPatamarCarga maiorIdPatamarCarga_horizonte = IdPatamarCarga_Nenhum;
		for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {
			const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
			if (maiorIdPatamarCarga > maiorIdPatamarCarga_horizonte)
				maiorIdPatamarCarga_horizonte = maiorIdPatamarCarga;
		} // for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

		validaRestricaoEletrica();

		IdRestricaoEletrica maiorIdRestricaoEletrica_operacional = IdRestricaoEletrica_Nenhum;

		const IdRestricaoEletrica maiorIdRestricaoEletrica = getMaiorId(IdRestricaoEletrica());
		for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= maiorIdRestricaoEletrica; idRestricaoEletrica++) {

			if ((getSize1Matriz(idRestricaoEletrica, AttMatrizRestricaoEletrica_potencia_minima) == 0) || (getSize1Matriz(idRestricaoEletrica, AttMatrizRestricaoEletrica_potencia_maxima) == 0)) {

				calcular_att_operacionais_restricao_eletrica = true;

				bool preencher_potencia_minima = false;
				if (getSize1Matriz(idRestricaoEletrica, AttMatrizRestricaoEletrica_potencia_minima) == 0) {
					preencher_potencia_minima = true;
					if (getSizeVetor(idRestricaoEletrica, AttVetorRestricaoEletrica_potencia_minima) == 0)
						vetorRestricaoEletrica.att(idRestricaoEletrica).setVetor(AttVetorRestricaoEletrica_potencia_minima, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idRestricaoEletrica, AttComumRestricaoEletrica_potencia_minima, double())));
				}

				bool preencher_potencia_maxima = false;
				if (getSize1Matriz(idRestricaoEletrica, AttMatrizRestricaoEletrica_potencia_maxima) == 0) {
					preencher_potencia_maxima = true;
					if (getSizeVetor(idRestricaoEletrica, AttVetorRestricaoEletrica_potencia_maxima) == 0)
						vetorRestricaoEletrica.att(idRestricaoEletrica).setVetor(AttVetorRestricaoEletrica_potencia_maxima, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idRestricaoEletrica, AttComumRestricaoEletrica_potencia_maxima, double())));
				}


				bool preencher_percentual_variacao_patamar_carga = false;
				if (getSize1Matriz(idRestricaoEletrica, AttMatrizRestricaoEletrica_percentual_variacao_patamar_carga) == 0)
					preencher_percentual_variacao_patamar_carga = true;

				for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

					const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
					for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						if (preencher_percentual_variacao_patamar_carga)
							vetorRestricaoEletrica.att(idRestricaoEletrica).addElemento(AttMatrizRestricaoEletrica_percentual_variacao_patamar_carga, periodo, idPatamarCarga, 1.0);

						const double percentual_variacao = getElementoMatriz(idRestricaoEletrica, AttMatrizRestricaoEletrica_percentual_variacao_patamar_carga, periodo, idPatamarCarga, double());

						if (preencher_potencia_minima)
							vetorRestricaoEletrica.att(idRestricaoEletrica).addElemento(AttMatrizRestricaoEletrica_potencia_minima, periodo, idPatamarCarga, getElementoVetor(idRestricaoEletrica, AttVetorRestricaoEletrica_potencia_minima, periodo, double()) * percentual_variacao);

						if (preencher_potencia_maxima)
							vetorRestricaoEletrica.att(idRestricaoEletrica).addElemento(AttMatrizRestricaoEletrica_potencia_maxima, periodo, idPatamarCarga, getElementoVetor(idRestricaoEletrica, AttVetorRestricaoEletrica_potencia_maxima, periodo, double()) * percentual_variacao);

						const double potencia_minima = getElementoMatriz(idRestricaoEletrica, AttMatrizRestricaoEletrica_potencia_minima, periodo, idPatamarCarga, double());
						const double potencia_maxima = getElementoMatriz(idRestricaoEletrica, AttMatrizRestricaoEletrica_potencia_maxima, periodo, idPatamarCarga, double());

						if (potencia_minima > potencia_maxima)
							throw std::invalid_argument("Potencia Minima maior que Potencia Maxima em " + getString(periodo) + " em " + getString(idPatamarCarga) + " em " + getString(idRestricaoEletrica));

					} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
				} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

			} // if ((getSize1Matriz(idRestricaoEletrica, AttMatrizRestricaoEletrica_potencia_minima) == 0) || (getSize1Matriz(idRestricaoEletrica, AttMatrizRestricaoEletrica_potencia_maxima) == 0)) {

			const IdElementoSistema maiorIdElementoSistema = getMaiorId(idRestricaoEletrica, IdElementoSistema());

			for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorIdElementoSistema; idElementoSistema++) {

				if (TipoVariavelRestricaoOperativa_Nenhum != getAtributo(idRestricaoEletrica, IdElementoSistema_1, AttComumElementoSistema_tipoVariavelRestricaoOperativa, TipoVariavelRestricaoOperativa()))
					throw std::invalid_argument(getFullString(idRestricaoEletrica) + " com " + getFullString(AttComumElementoSistema_tipoVariavelRestricaoOperativa) + " invalido.");

				if (getSize1Matriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao) == 0) {

					calcular_att_operacionais_elemento_sistema = true;

					if (getSizeVetor(idRestricaoEletrica, idElementoSistema, AttVetorElementoSistema_fator_participacao) == 0)
						vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.att(idElementoSistema).setVetor(AttVetorElementoSistema_fator_participacao, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_fator_participacao, double())));

					for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

						const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
						for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++)
							vetorRestricaoEletrica.att(idRestricaoEletrica).vetorElementoSistema.att(idElementoSistema).addElemento(AttMatrizElementoSistema_fator_participacao, periodo, idPatamarCarga, getElementoVetor(idRestricaoEletrica, idElementoSistema, AttVetorElementoSistema_fator_participacao, periodo, double()));

					} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				} // if (getSize1Matriz(idRestricaoEletrica, idElementoSistema, AttMatrizElementoSistema_fator_participacao) == 0) {

			} // for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorIdElementoSistema; idElementoSistema++) {

			const bool restricao_simples = isRestricaoEletrica_simples(idRestricaoEletrica);

			if (getAtributo(AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_eletricas, bool()) && restricao_simples && !a_imprimir_atributos_sem_recarregar)
				throw std::invalid_argument("Nao considerar " + getFullString(idRestricaoEletrica) + " com " + getFullString(AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_eletricas) + " positivo.");
			else if (getAtributo(AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_eletricas, bool()) && restricao_simples) {}
			else
				maiorIdRestricaoEletrica_operacional = idRestricaoEletrica;

			for (AttMatrizRestricaoEletrica attMatrizRestricaoEletrica = AttMatrizRestricaoEletrica(1); attMatrizRestricaoEletrica < AttMatrizRestricaoEletrica_Excedente; attMatrizRestricaoEletrica++) {

				const AttVetorRestricaoEletrica attVetorRestricaoEletrica_media = getAttVetorRestricaoEletricaFromChar(std::string(getString(attMatrizRestricaoEletrica) + "_media").c_str());

				if (attVetorRestricaoEletrica_media != AttVetorRestricaoEletrica_Nenhum) {

					SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz = getMatriz(idRestricaoEletrica, attMatrizRestricaoEletrica, Periodo(), IdPatamarCarga(), double());

					if (matriz.size() > 0) {

						SmartEnupla<Periodo, double> vetor(matriz, 0.0);

						for (Periodo periodo = matriz.getIteradorInicial(); periodo <= periodo_final_estudo; matriz.incrementarIterador(periodo)) {
							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= matriz.at(periodo).getIteradorFinal(); idPatamarCarga++)
								vetor.at(periodo) += matriz.at(periodo).at(idPatamarCarga) * getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
						}

						vetorRestricaoEletrica.att(idRestricaoEletrica).setVetor_forced(attVetorRestricaoEletrica_media, vetor);

					} // if (matriz.size() > 0) {

				} // if (attVetorRestricaoEletrica_media != AttVetorRestricaoEletrica_Nenhum) {

			} // for (AttMatrizRestricaoEletrica attMatrizRestricaoEletrica = AttMatrizRestricaoEletrica(1); attMatrizRestricaoEletrica < AttMatrizRestricaoEletrica_Excedente; attMatrizRestricaoEletrica++) {

		} // for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= maiorIdRestricaoEletrica; idRestricaoEletrica++) {

		const IdRestricaoEletrica menorIdRestricaoEletrica_nao_operacional = IdRestricaoEletrica(maiorIdRestricaoEletrica_operacional + 1);

		if (idProcesso == IdProcesso_mestre) {

			if (true) {
				bool impresso = false;

				for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= maiorIdRestricaoEletrica_operacional; idRestricaoEletrica++) {

					for (AttMatrizRestricaoEletrica attMatrizRestricaoEletrica = AttMatrizRestricaoEletrica(1); attMatrizRestricaoEletrica < AttMatrizRestricaoEletrica_Excedente; attMatrizRestricaoEletrica++) {

						const AttVetorRestricaoEletrica attVetorRestricaoEletrica_media = getAttVetorRestricaoEletricaFromChar(std::string(getString(attMatrizRestricaoEletrica) + "_media").c_str());

						if (attVetorRestricaoEletrica_media != AttVetorRestricaoEletrica_Nenhum) {

							a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

							a_entradaSaidaDados.setAppendArquivo(impresso);
							a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_RESTRICAO_ELETRICA_AttVetorPremissa_PorPeriodo.csv", idRestricaoEletrica, *this, periodo_estudo_inicial, periodo_final_estudo, attVetorRestricaoEletrica_media);
							impresso = true;

							vetorRestricaoEletrica.att(idRestricaoEletrica).clear(attVetorRestricaoEletrica_media);

						}

					} // for (AttMatrizRestricaoEletrica attMatrizRestricaoEletrica = AttMatrizRestricaoEletrica(1); attMatrizRestricaoEletrica < AttMatrizRestricaoEletrica_Nenhum; attMatrizRestricaoEletrica++) {

				} // for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= maiorIdRestricaoEletrica_operacional; idRestricaoEletrica++) {

			} // if (true) {
		} // if (idProcesso == IdProcesso_mestre) {

		if ((calcular_att_operacionais_restricao_eletrica) || (calcular_att_operacionais_elemento_sistema) || (a_imprimir_atributos_sem_recarregar)) {

			if (idProcesso == IdProcesso_mestre) {

				if (calcular_att_operacionais_restricao_eletrica) {

					//
					// Imprime Atributos Premissas
					//
					a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

					for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= maiorIdRestricaoEletrica; idRestricaoEletrica++) {

						if (idRestricaoEletrica == IdRestricaoEletrica_1)
							a_entradaSaidaDados.setAppendArquivo(false);
						else
							a_entradaSaidaDados.setAppendArquivo(true);

						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("RESTRICAO_ELETRICA_AttVetorPremissa_PorPeriodo.csv", idRestricaoEletrica, *this, periodo_estudo_inicial, periodo_final_estudo, AttVetorRestricaoEletrica_potencia_minima);
						a_entradaSaidaDados.setAppendArquivo(true);
						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("RESTRICAO_ELETRICA_AttVetorPremissa_PorPeriodo.csv", idRestricaoEletrica, *this, periodo_estudo_inicial, periodo_final_estudo, AttVetorRestricaoEletrica_potencia_maxima);

						if (idRestricaoEletrica == IdRestricaoEletrica_1)
							a_entradaSaidaDados.setAppendArquivo(false);
						else
							a_entradaSaidaDados.setAppendArquivo(true);

						a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("RESTRICAO_ELETRICA_AttMatrizPremissa_PorPeriodoPorIdPatamarCarga.csv", idRestricaoEletrica, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizRestricaoEletrica_percentual_variacao_patamar_carga);

					} // for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= maiorIdRestricaoEletrica; idRestricaoEletrica++) {

				} // if (calcular_att_operacionais_restricao_eletrica) {

				if (calcular_att_operacionais_elemento_sistema) {

					//
					// Imprime Atributos Premissas
					//
					a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

					for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= maiorIdRestricaoEletrica; idRestricaoEletrica++) {

						if (idRestricaoEletrica == IdRestricaoEletrica_1)
							a_entradaSaidaDados.setAppendArquivo(false);
						else
							a_entradaSaidaDados.setAppendArquivo(true);

						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("RESTRICAO_ELETRICA_ELEMENTO_SISTEMA_AttVetorPremissa_PorPeriodo.csv", idRestricaoEletrica, IdElementoSistema_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, AttVetorElementoSistema_fator_participacao);

					}

				} // if (calcular_att_operacionais_elemento_sistema) {

				//
				// Imprime Atributos Operacionais
				//

				for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= maiorIdRestricaoEletrica; idRestricaoEletrica++) {

					if (idRestricaoEletrica < menorIdRestricaoEletrica_nao_operacional)
						a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);
					else
						a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

					if ((idRestricaoEletrica == IdRestricaoEletrica_1) || (idRestricaoEletrica == menorIdRestricaoEletrica_nao_operacional))
						a_entradaSaidaDados.setAppendArquivo(false);
					else
						a_entradaSaidaDados.setAppendArquivo(true);

					a_entradaSaidaDados.imprimirArquivoCSV_AttComum("RESTRICAO_ELETRICA_AttComumOperacional.csv", idRestricaoEletrica, *this, std::vector<AttComumRestricaoEletrica> {AttComumRestricaoEletrica_idRestricaoEletrica, AttComumRestricaoEletrica_nome, AttComumRestricaoEletrica_penalidade_restricao_eletrica});

					if (idRestricaoEletrica == IdRestricaoEletrica_1)
						a_entradaSaidaDados.setAppendArquivo(false);
					else
						a_entradaSaidaDados.setAppendArquivo(true);

					a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("RESTRICAO_ELETRICA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", idRestricaoEletrica, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizRestricaoEletrica_potencia_minima);
					a_entradaSaidaDados.setAppendArquivo(true);
					a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("RESTRICAO_ELETRICA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", idRestricaoEletrica, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizRestricaoEletrica_potencia_maxima);

					if (idRestricaoEletrica == IdRestricaoEletrica_1)
						a_entradaSaidaDados.setAppendArquivo(false);
					else
						a_entradaSaidaDados.setAppendArquivo(true);

					a_entradaSaidaDados.imprimirArquivoCSV_AttComum("RESTRICAO_ELETRICA_ELEMENTO_SISTEMA_AttComumOperacional.csv", idRestricaoEletrica, IdElementoSistema_Nenhum, *this);

					if (idRestricaoEletrica == IdRestricaoEletrica_1)
						a_entradaSaidaDados.setAppendArquivo(false);
					else
						a_entradaSaidaDados.setAppendArquivo(true);

					a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("RESTRICAO_ELETRICA_ELEMENTO_SISTEMA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", idRestricaoEletrica, IdElementoSistema_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizElementoSistema_fator_participacao);

				}

			} // if (idProcesso == IdProcesso_mestre){

			if (!a_imprimir_atributos_sem_recarregar) {

				if (true) {
					int barreira = 0;

					if (arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso()) == IdProcesso_mestre) {
						for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++)
							MPI_Send(&barreira, 1, MPI_INT, getRank(idProcesso), 0, MPI_COMM_WORLD);
					}
					else
						MPI_Recv(&barreira, 1, MPI_INT, getRank(IdProcesso_mestre), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}

				//
				// Esvazia Atributos
				//

				vetorRestricaoEletrica = VetorRestricaoEletricaEmDados();

				//
				// Recarrega Atributos Operacionais
				//		
				a_entradaSaidaDados.setDiretorioEntrada(a_diretorio_att_operacional);

				a_entradaSaidaDados.carregarArquivoCSV_AttComum("RESTRICAO_ELETRICA_AttComumOperacional.csv", *this, TipoAcessoInstancia_membro);

				a_entradaSaidaDados.carregarArquivoCSV_AttVetor("RESTRICAO_ELETRICA_AttVetorOperacional_PorInteiro.csv", *this, TipoAcessoInstancia_membro);

				a_entradaSaidaDados.carregarArquivoCSV_AttMatriz("RESTRICAO_ELETRICA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);

				// Elemento Sistema

				a_entradaSaidaDados.carregarArquivoCSV_AttComum("RESTRICAO_ELETRICA_ELEMENTO_SISTEMA_AttComumOperacional.csv", *this, TipoAcessoInstancia_membroMembro);

				a_entradaSaidaDados.carregarArquivoCSV_AttMatriz("RESTRICAO_ELETRICA_ELEMENTO_SISTEMA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membroMembro);

			} // if (!a_imprimir_atributos_sem_recarregar) {

		} // if ((calcular_att_operacionais_restricao_eletrica) || (a_imprimir_atributos_sem_recarregar)) {



	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validacao_operacional_RestricaoEletrica(a_entradaSaidaDados, " + a_diretorio_att_operacional + "," + a_diretorio_att_premissa + "): \n" + std::string(erro.what())); }

} // void Dados::validacao_operacional_RestricaoEletrica(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar){


void Dados::validacao_operacional_AgrupamentoIntercambio(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar) {

	try {

		const IdProcesso idProcesso = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());

		bool calcular_att_operacionais_agrupamento_intercambio = false;

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_estudo_inicial = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();

		IdPatamarCarga maiorIdPatamarCarga_horizonte = IdPatamarCarga_Nenhum;
		for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {
			const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
			if (maiorIdPatamarCarga > maiorIdPatamarCarga_horizonte)
				maiorIdPatamarCarga_horizonte = maiorIdPatamarCarga;
		} // for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

		validaAgrupamentoIntercambio();

		const IdAgrupamentoIntercambio maiorIdAgrupamentoIntercambio = getMaiorId(IdAgrupamentoIntercambio());
		for (IdAgrupamentoIntercambio idAgrupamentoIntercambio = IdAgrupamentoIntercambio_1; idAgrupamentoIntercambio <= maiorIdAgrupamentoIntercambio; idAgrupamentoIntercambio++) {

			if ((getSize1Matriz(idAgrupamentoIntercambio, AttMatrizAgrupamentoIntercambio_potencia_minima) == 0) || (getSize1Matriz(idAgrupamentoIntercambio, AttMatrizAgrupamentoIntercambio_potencia_maxima) == 0)) {

				calcular_att_operacionais_agrupamento_intercambio = true;

				bool preencher_potencia_minima = false;
				if (getSize1Matriz(idAgrupamentoIntercambio, AttMatrizAgrupamentoIntercambio_potencia_minima) == 0) {
					preencher_potencia_minima = true;
					if (getSizeVetor(idAgrupamentoIntercambio, AttVetorAgrupamentoIntercambio_potencia_minima) == 0)
						vetorAgrupamentoIntercambio.att(idAgrupamentoIntercambio).setVetor(AttVetorAgrupamentoIntercambio_potencia_minima, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idAgrupamentoIntercambio, AttComumAgrupamentoIntercambio_potencia_minima, double())));
				}

				bool preencher_potencia_maxima = false;
				if (getSize1Matriz(idAgrupamentoIntercambio, AttMatrizAgrupamentoIntercambio_potencia_maxima) == 0) {
					preencher_potencia_maxima = true;
					if (getSizeVetor(idAgrupamentoIntercambio, AttVetorAgrupamentoIntercambio_potencia_maxima) == 0)
						vetorAgrupamentoIntercambio.att(idAgrupamentoIntercambio).setVetor(AttVetorAgrupamentoIntercambio_potencia_maxima, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idAgrupamentoIntercambio, AttComumAgrupamentoIntercambio_potencia_maxima, double())));
				}


				bool preencher_percentual_variacao_patamar_carga = false;
				if (getSize1Matriz(idAgrupamentoIntercambio, AttMatrizAgrupamentoIntercambio_percentual_variacao_patamar_carga) == 0)
					preencher_percentual_variacao_patamar_carga = true;

				for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

					const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
					for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						if (preencher_percentual_variacao_patamar_carga)
							vetorAgrupamentoIntercambio.att(idAgrupamentoIntercambio).addElemento(AttMatrizAgrupamentoIntercambio_percentual_variacao_patamar_carga, periodo, idPatamarCarga, 1.0);

						const double percentual_variacao = getElementoMatriz(idAgrupamentoIntercambio, AttMatrizAgrupamentoIntercambio_percentual_variacao_patamar_carga, periodo, idPatamarCarga, double());

						if (preencher_potencia_minima)
							vetorAgrupamentoIntercambio.att(idAgrupamentoIntercambio).addElemento(AttMatrizAgrupamentoIntercambio_potencia_minima, periodo, idPatamarCarga, getElementoVetor(idAgrupamentoIntercambio, AttVetorAgrupamentoIntercambio_potencia_minima, periodo, double()) * percentual_variacao);

						if (preencher_potencia_maxima)
							vetorAgrupamentoIntercambio.att(idAgrupamentoIntercambio).addElemento(AttMatrizAgrupamentoIntercambio_potencia_maxima, periodo, idPatamarCarga, getElementoVetor(idAgrupamentoIntercambio, AttVetorAgrupamentoIntercambio_potencia_maxima, periodo, double()) * percentual_variacao);

						const double potencia_minima = getElementoMatriz(idAgrupamentoIntercambio, AttMatrizAgrupamentoIntercambio_potencia_minima, periodo, idPatamarCarga, double());
						const double potencia_maxima = getElementoMatriz(idAgrupamentoIntercambio, AttMatrizAgrupamentoIntercambio_potencia_maxima, periodo, idPatamarCarga, double());

						if (potencia_minima > potencia_maxima)
							throw std::invalid_argument("Potencia Minima maior que Potencia Maxima em " + getString(periodo) + " em " + getString(idPatamarCarga) + " em " + getString(idAgrupamentoIntercambio));

					} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
				} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

			} // if ((getSize1Matriz(idAgrupamentoIntercambio, AttMatrizAgrupamentoIntercambio_potencia_minima) == 0) || (getSize1Matriz(idAgrupamentoIntercambio, AttMatrizAgrupamentoIntercambio_potencia_maxima) == 0)) {

		} // for (IdAgrupamentoIntercambio idAgrupamentoIntercambio = IdAgrupamentoIntercambio_1; idAgrupamentoIntercambio <= maiorIdAgrupamentoIntercambio; idAgrupamentoIntercambio++) {


		if ((calcular_att_operacionais_agrupamento_intercambio) || (a_imprimir_atributos_sem_recarregar)) {

			if (idProcesso == IdProcesso_mestre) {

				if (calcular_att_operacionais_agrupamento_intercambio) {

					//
					// Imprime Atributos Premissas
					//
					a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

					a_entradaSaidaDados.setAppendArquivo(false);
					a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("AGRUPAMENTO_INTERCAMBIO_AttVetorPremissa_PorPeriodo.csv", IdAgrupamentoIntercambio_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, AttVetorAgrupamentoIntercambio_potencia_minima);
					a_entradaSaidaDados.setAppendArquivo(true);
					a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("AGRUPAMENTO_INTERCAMBIO_AttVetorPremissa_PorPeriodo.csv", IdAgrupamentoIntercambio_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, AttVetorAgrupamentoIntercambio_potencia_maxima);

					a_entradaSaidaDados.setAppendArquivo(false);
					a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("AGRUPAMENTO_INTERCAMBIO_AttMatrizPremissa_PorPeriodoPorIdPatamarCarga.csv", IdAgrupamentoIntercambio_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizAgrupamentoIntercambio_percentual_variacao_patamar_carga);


				} // if (calcular_att_operacionais_agrupamento_intercambio) {

				//
				// Imprime Atributos Operacionais
				//
				a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

				a_entradaSaidaDados.setAppendArquivo(false);
				a_entradaSaidaDados.imprimirArquivoCSV_AttComum("AGRUPAMENTO_INTERCAMBIO_AttComumOperacional.csv", IdAgrupamentoIntercambio_Nenhum, *this, std::vector<AttComumAgrupamentoIntercambio> {AttComumAgrupamentoIntercambio_idAgrupamentoIntercambio, AttComumAgrupamentoIntercambio_nome});

				a_entradaSaidaDados.setAppendArquivo(false);
				a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("AGRUPAMENTO_INTERCAMBIO_AttVetorOperacional_PorInteiro.csv", IdAgrupamentoIntercambio_Nenhum, *this, 1, 100, AttVetorAgrupamentoIntercambio_intercambio);
				a_entradaSaidaDados.setAppendArquivo(true);
				a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("AGRUPAMENTO_INTERCAMBIO_AttVetorOperacional_PorInteiro.csv", IdAgrupamentoIntercambio_Nenhum, *this, 1, 100, AttVetorAgrupamentoIntercambio_fator_ponderacao);

				a_entradaSaidaDados.setAppendArquivo(false);
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("AGRUPAMENTO_INTERCAMBIO_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", IdAgrupamentoIntercambio_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizAgrupamentoIntercambio_potencia_minima);
				a_entradaSaidaDados.setAppendArquivo(true);
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("AGRUPAMENTO_INTERCAMBIO_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", IdAgrupamentoIntercambio_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizAgrupamentoIntercambio_potencia_maxima);

			} // if (idProcesso == IdProcesso_mestre){

			if (!a_imprimir_atributos_sem_recarregar) {

				if (true) {
					int barreira = 0;

					if (arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso()) == IdProcesso_mestre) {
						for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++)
							MPI_Send(&barreira, 1, MPI_INT, getRank(idProcesso), 0, MPI_COMM_WORLD);
					}
					else
						MPI_Recv(&barreira, 1, MPI_INT, getRank(IdProcesso_mestre), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}

				//
				// Esvazia Atributos
				//

				vetorAgrupamentoIntercambio = VetorAgrupamentoIntercambioEmDados();

				//
				// Recarrega Atributos Operacionais
				//		
				a_entradaSaidaDados.setDiretorioEntrada(a_diretorio_att_operacional);

				a_entradaSaidaDados.carregarArquivoCSV_AttComum("AGRUPAMENTO_INTERCAMBIO_AttComumOperacional.csv", *this, TipoAcessoInstancia_membro);

				a_entradaSaidaDados.carregarArquivoCSV_AttVetor("AGRUPAMENTO_INTERCAMBIO_AttVetorOperacional_PorInteiro.csv", *this, TipoAcessoInstancia_membro);

				a_entradaSaidaDados.carregarArquivoCSV_AttMatriz("AGRUPAMENTO_INTERCAMBIO_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);

			} // if (!a_imprimir_atributos_sem_recarregar) {

		} // if ((calcular_att_operacionais_agrupamento_intercambio) || (a_imprimir_atributos_sem_recarregar)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validacao_operacional_AgrupamentoIntercambio(a_entradaSaidaDados, " + a_diretorio_att_operacional + "," + a_diretorio_att_premissa + "): \n" + std::string(erro.what())); }

} // void Dados::validacao_operacional_AgrupamentoIntercambio(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar){


int Dados::isCalculoAttOperacionaisProcessoEstocasticoHidrologicoNecessario(ProcessoEstocastico& a_processo_estocastico, const IdProcessoEstocastico a_tipo_processo_estocastico) {

	try {

		const SmartEnupla<IdEstagio, Periodo> horizonte_otimizacao = getVetor(AttVetorDados_horizonte_otimizacao, IdEstagio(), Periodo());

		const IdEstagio estagio_inicial = getAtributo(AttComumDados_estagio_inicial, IdEstagio());
		const IdEstagio estagio_final = getAtributo(AttComumDados_estagio_final, IdEstagio());

		const IdProcesso idProcesso = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());

		const Periodo periodo_inicial_otimizacao = horizonte_otimizacao.getElemento(estagio_inicial);
		const Periodo periodo_final_otimizacao = horizonte_otimizacao.getElemento(estagio_final);

		const IdHidreletrica maiorIdHidreletrica = getMaiorId(IdHidreletrica());

		if (a_processo_estocastico.getAtributo(AttComumProcessoEstocastico_idProcessoEstocastico, IdProcessoEstocastico()) != a_tipo_processo_estocastico)
			return 1;

		const SmartEnupla<IdCenario, SmartEnupla<Periodo, IdRealizacao>> mapeamento_espaco_amostral = a_processo_estocastico.getMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario(), Periodo(), IdRealizacao());

		if (mapeamento_espaco_amostral.size() == 0)
			return 2;

		if (mapeamento_espaco_amostral.getIteradorInicial() != IdCenario_1)
			return 3;

		if (mapeamento_espaco_amostral.at(IdCenario_1).size() == 0)
			return 4;

		Periodo periodo_inicial_espaco_amostral = mapeamento_espaco_amostral.at(IdCenario_1).getIteradorInicial();
		Periodo periodo_final_espaco_amostral = mapeamento_espaco_amostral.at(IdCenario_1).getIteradorFinal();

		SmartEnupla<Periodo, IdRealizacao> maior_realizacao_espaco_amostral(mapeamento_espaco_amostral.at(IdCenario_1), IdRealizacao_Nenhum);

		for (IdCenario idCenario = IdCenario_2; idCenario <= mapeamento_espaco_amostral.getIteradorFinal(); idCenario++) {

			if (mapeamento_espaco_amostral.at(idCenario).size() == 0)
				return 5;

			if (mapeamento_espaco_amostral.at(idCenario).getIteradorInicial() != periodo_inicial_espaco_amostral)
				return 6;

			if (mapeamento_espaco_amostral.at(idCenario).getIteradorFinal() != periodo_final_espaco_amostral)
				return 7;

			for (Periodo periodo = periodo_inicial_espaco_amostral; periodo <= periodo_final_espaco_amostral; mapeamento_espaco_amostral.at(IdCenario_1).incrementarIterador(periodo)) {
				if (mapeamento_espaco_amostral.at(idCenario).at(periodo) > maior_realizacao_espaco_amostral.at(periodo))
					maior_realizacao_espaco_amostral.at(periodo) = mapeamento_espaco_amostral.at(idCenario).at(periodo);
			}

		} // 

		if (periodo_inicial_espaco_amostral > periodo_inicial_otimizacao)
			return 8;

		SmartEnupla<IdEstagio, std::vector<Periodo>> periodos_em_estagio(estagio_inicial, std::vector<std::vector<Periodo>>(int(estagio_final - estagio_inicial) + 1, std::vector<Periodo>()));
		for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {
			const Periodo periodo_decomposicao = getElementoVetor(AttVetorDados_horizonte_otimizacao, idEstagio, Periodo());
			periodos_em_estagio.at(idEstagio) = mapeamento_espaco_amostral.at(IdCenario_1).getIteradores(periodo_decomposicao);

			if (periodos_em_estagio.at(idEstagio).size() > 0) {

				if (Periodo(TipoPeriodo_minuto, periodo_decomposicao) != Periodo(TipoPeriodo_minuto, periodos_em_estagio.at(idEstagio).at(0)))
					return 81;

				if (Periodo(TipoPeriodo_minuto, periodo_decomposicao + 1) != Periodo(TipoPeriodo_minuto, periodos_em_estagio.at(idEstagio).at(periodos_em_estagio.at(idEstagio).size() - 1) + 1))
					return 82;

				for (IdCenario idCenario = IdCenario_1; idCenario <= mapeamento_espaco_amostral.getIteradorFinal(); idCenario++) {
					const IdRealizacao idRealizacao = mapeamento_espaco_amostral.at(idCenario).at(periodos_em_estagio.at(idEstagio).at(0));
					for (int i = 1; i < int(periodos_em_estagio.at(idEstagio).size()); i++) {
						if (idRealizacao != mapeamento_espaco_amostral.at(idCenario).at(periodos_em_estagio.at(idEstagio).at(i)))
							return 83;
					}
				}
			}

		}


		const SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> probabilidade_realizacao = a_processo_estocastico.getMatriz(AttMatrizProcessoEstocastico_probabilidade_realizacao, Periodo(), IdRealizacao(), double());

		Periodo periodo_prob_realizacao = probabilidade_realizacao.getIteradorInicial();

		Periodo periodo_espaco_amostral = periodo_inicial_espaco_amostral;
		for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

			const Periodo periodo_otimizacao = horizonte_otimizacao.getElemento(idEstagio);

			const double sobreposicao = periodo_espaco_amostral.sobreposicao(periodo_otimizacao);

			if ((0.0 < sobreposicao) && (sobreposicao < 1.0))
				return 91;

			else if (sobreposicao == 1.0) {

				if (periodo_prob_realizacao.sobreposicao(periodo_espaco_amostral) > 0) {

					if (periodo_prob_realizacao != periodo_espaco_amostral)
						return 92;
					if (periodo_prob_realizacao < probabilidade_realizacao.getIteradorFinal())
						probabilidade_realizacao.incrementarIterador(periodo_prob_realizacao);
				}

				if (periodo_espaco_amostral < periodo_final_espaco_amostral)
					mapeamento_espaco_amostral.at(IdCenario_1).incrementarIterador(periodo_espaco_amostral);

			}

		} // for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

		bool isHorizontePEmenor = false;

		if (periodo_final_espaco_amostral < periodo_final_otimizacao)
			isHorizontePEmenor = true;

		Periodo periodo_final_tendencia_minuto_ref = Periodo(TipoPeriodo_minuto, periodo_inicial_espaco_amostral) - 1;

		for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= a_processo_estocastico.vetorVariavelAleatoria.getMaiorId(); idVariavelAleatoria++) {

			const SmartEnupla<Periodo, SmartEnupla<int, double>> coeficiente_linear_auto_correlacao = a_processo_estocastico.getMatriz(idVariavelAleatoria, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, Periodo(), int(), double());

			if (true) {

				const SmartEnupla<Periodo, TipoRelaxacaoVariavelAleatoria> tipo_relaxacao = a_processo_estocastico.getVetor(idVariavelAleatoria, AttVetorVariavelAleatoria_tipo_relaxacao, Periodo(), TipoRelaxacaoVariavelAleatoria());

				const SmartEnupla<Periodo, SmartEnupla<IdRealizacao, double>> residuo_espaco_amostral = a_processo_estocastico.getMatriz(idVariavelAleatoria, AttMatrizVariavelAleatoria_residuo_espaco_amostral, Periodo(), IdRealizacao(), double());

				if (coeficiente_linear_auto_correlacao.size() == 0)
					return 101;

				if (tipo_relaxacao.size() == 0)
					return 102;

				if (residuo_espaco_amostral.size() == 0)
					return 103;

				Periodo periodo_coef_linear = coeficiente_linear_auto_correlacao.getIteradorInicial();
				Periodo periodo_tipo_rel = tipo_relaxacao.getIteradorInicial();
				Periodo periodo_residuo = residuo_espaco_amostral.getIteradorInicial();
				for (Periodo periodo_espaco_amostral = periodo_inicial_espaco_amostral; periodo_espaco_amostral <= periodo_final_espaco_amostral; mapeamento_espaco_amostral.at(IdCenario_1).incrementarIterador(periodo_espaco_amostral)) {

					if (periodo_coef_linear != periodo_espaco_amostral)
						return 111;

					if (periodo_tipo_rel != periodo_espaco_amostral)
						return 112;

					if (periodo_residuo != periodo_espaco_amostral)
						return 113;

					if (coeficiente_linear_auto_correlacao.at(periodo_coef_linear).size() == 0)
						return 121;

					if (coeficiente_linear_auto_correlacao.at(periodo_coef_linear).size() > 1) {
						if (coeficiente_linear_auto_correlacao.at(periodo_coef_linear).at(coeficiente_linear_auto_correlacao.at(periodo_coef_linear).getIteradorFinal()) == 0.0)
							return 122;
					}

					if (residuo_espaco_amostral.at(periodo_residuo).size() == 0)
						return 123;

					if (residuo_espaco_amostral.at(periodo_residuo).getIteradorInicial() != IdRealizacao_1)
						return 124;

					if (residuo_espaco_amostral.at(periodo_residuo).getIteradorFinal() > maior_realizacao_espaco_amostral.at(periodo_residuo))
						return 1240;

					if (residuo_espaco_amostral.at(periodo_residuo).size() == 1) {
						if ((probabilidade_realizacao.getIteradorInicial() <= periodo_residuo) && (periodo_residuo <= probabilidade_realizacao.getIteradorFinal())) {
							if (probabilidade_realizacao.at(periodo_residuo).size() > 1)
								return 125;
							else if (probabilidade_realizacao.at(periodo_residuo).size() == 1) {
								if (probabilidade_realizacao.at(periodo_residuo).getIteradorInicial() != IdRealizacao_1)
									return 126;
							}
						}
					}

					else if (residuo_espaco_amostral.at(periodo_residuo).size() > 1) {
						if (probabilidade_realizacao.at(periodo_residuo).getIteradorInicial() != IdRealizacao_1)
							return 127;
						if (residuo_espaco_amostral.at(periodo_residuo).size() != probabilidade_realizacao.at(periodo_residuo).size())
							return 128;

						a_processo_estocastico.validar_probabilidade_realizacao(periodo_residuo);
					}

					if (periodo_prob_realizacao.sobreposicao(periodo_residuo) > 0.0) {
						probabilidade_realizacao.incrementarIterador(periodo_prob_realizacao); //refazer
					}

					coeficiente_linear_auto_correlacao.incrementarIterador(periodo_coef_linear);
					tipo_relaxacao.incrementarIterador(periodo_tipo_rel);
					residuo_espaco_amostral.incrementarIterador(periodo_residuo);

				}

				if (coeficiente_linear_auto_correlacao.getIteradorFinal() != periodo_final_espaco_amostral)
					return 131;

				if (tipo_relaxacao.getIteradorFinal() != periodo_final_espaco_amostral)
					return 132;

				if (residuo_espaco_amostral.getIteradorFinal() != periodo_final_espaco_amostral)
					return 133;

			} // if (true) {

			for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= a_processo_estocastico.getMaiorId(idVariavelAleatoria, IdVariavelAleatoriaInterna()); idVariavelAleatoriaInterna++) {

				if ((a_tipo_processo_estocastico == IdProcessoEstocastico_hidrologico_hidreletrica) || (a_tipo_processo_estocastico == IdProcessoEstocastico_hidrologico_bacia)) {
					if (!vetorHidreletrica.isInstanciado(a_processo_estocastico.getIdFisicoFromIdVariavelAleatoriaIdVariavelAleatoriaInterna(idVariavelAleatoria, idVariavelAleatoriaInterna, IdHidreletrica())))
						return 14;
				}

				SmartEnupla<Periodo, double> coeficiente_participacao = a_processo_estocastico.getVetor(idVariavelAleatoria, idVariavelAleatoriaInterna, AttVetorVariavelAleatoriaInterna_coeficiente_participacao, Periodo(), double());

				if (coeficiente_participacao.size() == 0)
					return 15;

				Periodo periodo_coef_part = coeficiente_participacao.getIteradorInicial();
				for (Periodo periodo_espaco_amostral = periodo_inicial_espaco_amostral; periodo_espaco_amostral <= periodo_final_espaco_amostral; mapeamento_espaco_amostral.at(IdCenario_1).incrementarIterador(periodo_espaco_amostral)) {

					if (periodo_coef_part != periodo_espaco_amostral)
						return 151;

				
					coeficiente_participacao.incrementarIterador(periodo_coef_part);

				}

				if (coeficiente_participacao.getIteradorFinal() != periodo_final_espaco_amostral)
					return 152;

				SmartEnupla<Periodo, double> tendencia_temporal = a_processo_estocastico.getVetor(idVariavelAleatoria, idVariavelAleatoriaInterna, AttVetorVariavelAleatoriaInterna_tendencia_temporal, Periodo(), double());

				if (estagio_inicial == IdEstagio_1) {

					if (tendencia_temporal.size() == 0)
						return 16;

					if (periodo_final_tendencia_minuto_ref != Periodo(TipoPeriodo_minuto, tendencia_temporal.getIteradorFinal() + 1) - 1)
						return 17;

					int lag = 0;
					for (lag = coeficiente_linear_auto_correlacao.at(periodo_inicial_espaco_amostral).size(); lag > 1; lag--) {
						if (coeficiente_linear_auto_correlacao.at(periodo_inicial_espaco_amostral).at(lag) > 0.0)
							break;
					}

					const Periodo periodo_minimo_tendencia = periodo_inicial_espaco_amostral -lag;

					if (tendencia_temporal.getIteradorInicial() > periodo_minimo_tendencia)
						return 18;

				}
			
			} // for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= maiorIdVariavelAleatoriaInterna; idVariavelAleatoriaInterna++) {

			for (Periodo periodo_espaco_amostral = periodo_inicial_espaco_amostral; periodo_espaco_amostral <= periodo_final_espaco_amostral; mapeamento_espaco_amostral.at(IdCenario_1).incrementarIterador(periodo_espaco_amostral)) {
				double coef_part_acumulado = 0.0;
				for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= a_processo_estocastico.getMaiorId(idVariavelAleatoria, IdVariavelAleatoriaInterna()); idVariavelAleatoriaInterna++) {
					coef_part_acumulado += a_processo_estocastico.getElementoVetor(idVariavelAleatoria, idVariavelAleatoriaInterna, AttVetorVariavelAleatoriaInterna_coeficiente_participacao, periodo_espaco_amostral, double());
				}
				if (coef_part_acumulado > 1.0)
					return 19;
			}


		} // for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= maiorIdVariavelAleatoria; idVariavelAleatoria++) {

		if (isHorizontePEmenor)
			return -1;

		return 0;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::isCalculoAttOperacionaisProcessoEstocasticoHidrologicoNecessario(a_processo_estocastico," + getFullString(a_tipo_processo_estocastico) + "): \n" + std::string(erro.what())); }

} // bool Dados::isCalculoAttOperacionaisProcessoEstocasticoHidrologicoNecessario(){


void Dados::validacao_operacional_ProcessoEstocasticoHidrologico(EntradaSaidaDados& a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const std::string a_diretorio_exportacao_pos_estudo, const bool a_imprimir_atributos_sem_recarregar) {

	try {

		const IdProcesso idProcesso = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());

		bool calcular_att_operacionais_historico_hidrologico = false;
		bool calcular_att_operacionais_tendencia_hidrologica = false;

		const IdProcessoEstocastico tipo_processo_estocastico_hidrologico = getAtributo(AttComumDados_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

		const TipoEspacoAmostral   tipo_espaco_amostral_hidrologico = getAtributo(AttComumDados_tipo_espaco_amostral_geracao_cenario_hidrologico, TipoEspacoAmostral());

		const TipoModeloGeracaoSinteticaCenario tipo_modelo_geracao_sintetica = getAtributo(AttComumDados_tipo_modelo_geracao_cenario_hidrologico, TipoModeloGeracaoSinteticaCenario());
		const TipoValor     tipo_coeficiente_auto_correlacao = getAtributo(AttComumDados_tipo_coeficiente_auto_correlacao_geracao_cenario_hidrologico, TipoValor());
		const TipoCorrelacaoVariaveisAleatorias tipo_correlacao_variaveis_aleatorias = getAtributo(AttComumDados_tipo_correlacao_geracao_cenario_hidrologico, TipoCorrelacaoVariaveisAleatorias());
		const double correlacao_dominante = getAtributo(AttComumDados_correlacao_dominante_geracao_cenario_hidrologico, double());

		const int  ordem_maxima_auto_correlacao_hidrologica = getAtributo(AttComumDados_ordem_maxima_auto_correlacao_geracao_cenario_hidrologico, int());

		const TipoTendenciaEstocastica tipo_tendencia_hidrologica = getAtributo(AttComumDados_tipo_tendencia_hidrologica, TipoTendenciaEstocastica());

		const TipoEstudo tipo_estudo = getAtributo(AttComumDados_tipo_estudo, TipoEstudo());

		const SmartEnupla<IdEstagio, Periodo> horizonte_otimizacao = getVetor(AttVetorDados_horizonte_otimizacao, IdEstagio(), Periodo());

		const IdEstagio estagio_inicial = getAtributo(AttComumDados_estagio_inicial, IdEstagio());
		const IdEstagio estagio_final = getAtributo(AttComumDados_estagio_final, IdEstagio());

		const IdCenario cenario_inicial = arranjoResolucao.getAtributo(idProcesso, AttComumProcesso_menor_cenario, IdCenario());
		const IdCenario cenario_final   = arranjoResolucao.getAtributo(idProcesso, AttComumProcesso_maior_cenario, IdCenario());

		int calcular_att_operacionais_processo_estocastico_hidrologico = isCalculoAttOperacionaisProcessoEstocasticoHidrologicoNecessario(processoEstocastico_hidrologico, tipo_processo_estocastico_hidrologico);

		if (arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso()) == IdProcesso_mestre) {
			for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++) {
				int calcular_att_operacionais_processo_estocastico_hidrologico_rank = 0;
				MPI_Recv(&calcular_att_operacionais_processo_estocastico_hidrologico_rank, 1, MPI_INT, getRank(idProcesso), 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

				if ((calcular_att_operacionais_processo_estocastico_hidrologico == 0) && (calcular_att_operacionais_processo_estocastico_hidrologico_rank > 0))
					calcular_att_operacionais_processo_estocastico_hidrologico = calcular_att_operacionais_processo_estocastico_hidrologico_rank;

				else if ((calcular_att_operacionais_processo_estocastico_hidrologico == 0) && (calcular_att_operacionais_processo_estocastico_hidrologico_rank < 0))
					calcular_att_operacionais_processo_estocastico_hidrologico = calcular_att_operacionais_processo_estocastico_hidrologico_rank;

			} // for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++) {
		}
		else
			MPI_Send(&calcular_att_operacionais_processo_estocastico_hidrologico, 1, MPI_INT, getRank(IdProcesso_mestre), 1, MPI_COMM_WORLD);

		if (arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso()) == IdProcesso_mestre) {
			for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++)
				MPI_Send(&calcular_att_operacionais_processo_estocastico_hidrologico, 1, MPI_INT, getRank(idProcesso), 1, MPI_COMM_WORLD);
		}
		else
			MPI_Recv(&calcular_att_operacionais_processo_estocastico_hidrologico, 1, MPI_INT, getRank(IdProcesso_mestre), 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


		bool imprimir_parametros_hidrologico = false;
		bool imprimir_r2_historico_natural = false;
		bool imprimir_espaco_amostral_hidrologico = false;

		if (idProcesso == IdProcesso_mestre) {
			imprimir_parametros_hidrologico = true;
			imprimir_r2_historico_natural = true;
			imprimir_espaco_amostral_hidrologico = getAtributo(AttComumDados_imprimir_espaco_amostral_geracao_cenario_hidrologico, bool());
		} // if (idProcesso == IdProcesso_mestre) {

		const bool imprimir_cenarios_hidrologicos = getAtributo(AttComumDados_imprimir_geracao_cenario_hidrologico, bool());


		if (calcular_att_operacionais_processo_estocastico_hidrologico > 0) {

			if (idProcesso == IdProcesso_mestre)
				std::cout << "Novo Processo Estocastico Instanciado - " << getString(calcular_att_operacionais_processo_estocastico_hidrologico) << std::endl;

			if (!valida_historico_AfluenciaEmHidreletrica(AttVetorAfluencia_incremental_historico)) {
				if (valida_historico_AfluenciaEmHidreletrica(AttVetorAfluencia_natural_historico)) {
					calcular_historico_afluencia_incremental_com_natural();
					calcular_att_operacionais_historico_hidrologico = true;
				}
				else
					throw std::invalid_argument("Necessario informar atributo operacional " + getFullString(AttVetorAfluencia_incremental_historico) + " ou atributo premissa " + getFullString(AttVetorAfluencia_natural_historico));
			} // if (!valida_historico_AfluenciaEmHidreletrica(AttVetorAfluencia_incremental_historico)) {

			if (tipo_tendencia_hidrologica == TipoTendenciaEstocastica_serie_informada) {
				if (!valida_tendencia_AfluenciaEmHidreletrica(AttVetorAfluencia_incremental_tendencia)) {
					if (valida_tendencia_AfluenciaEmHidreletrica(AttVetorAfluencia_natural_tendencia)) {
						calcular_tendencia_afluencia_incremental_com_natural();
						calcular_att_operacionais_tendencia_hidrologica = true;
					}
					else
						throw std::invalid_argument("Necessario informar atributo operacional " + getFullString(AttVetorAfluencia_incremental_tendencia) + " ou atributo premissa " + getFullString(AttVetorAfluencia_natural_tendencia));
				} // if (!valida_tendencia_AfluenciaEmHidreletrica(AttMatrizAfluencia_incremental_tendencia)) {
			} // if (tipo_tendencia_hidrologica == TipoTendenciaEstocastica_serie_informada) {
			else if (tipo_tendencia_hidrologica == TipoTendenciaEstocastica_historico) {
				adicionarTendenciaHidrologicaHistorica();
				calcular_att_operacionais_tendencia_hidrologica = true;
			}

		} // if (calcular_att_operacionais_processo_estocastico_hidrologico > 0) {
		else if (calcular_att_operacionais_processo_estocastico_hidrologico < 0) {

			if (idProcesso == IdProcesso_mestre)
				std::cout << "Novo Processo Estocastico Instanciado e Anexado ao Anterior." << std::endl;

		}


		//
		// Imprime Atributos Premissas ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//

		if ((calcular_att_operacionais_processo_estocastico_hidrologico != 0) || (a_imprimir_atributos_sem_recarregar)) {

			if (idProcesso == IdProcesso_mestre) {

				a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

				bool impresso_historico = false;
				if (getSizeVetor(getMenorId(IdHidreletrica()), IdAfluencia_vazao_afluente, AttVetorAfluencia_natural_historico) > 0) {
					a_entradaSaidaDados.setAppendArquivo(impresso_historico);
					a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("HIDRELETRICA_AFLUENCIA_AttVetorPremissa_PorPeriodo.csv", IdHidreletrica_Nenhum, IdAfluencia_vazao_afluente, *this, AttVetorAfluencia_natural_historico);
					impresso_historico = true;
				}
				bool impresso_tendencia = false;
				if (getSizeVetor(getMenorId(IdHidreletrica()), IdAfluencia_vazao_afluente, AttVetorAfluencia_natural_tendencia) > 0) {
					a_entradaSaidaDados.setAppendArquivo(impresso_tendencia);
					a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("HIDRELETRICA_AFLUENCIA_AttVetorPremissa_PorCenarioPorPeriodo.csv", IdHidreletrica_Nenhum, IdAfluencia_vazao_afluente, *this, AttVetorAfluencia_natural_tendencia);
					impresso_tendencia = true;
				}

			} // if (idProcesso == IdProcesso_mestre) {

		} // if ((calcular_att_operacionais_processo_estocastico_hidrologico > 0) || (a_imprimir_atributos_sem_recarregar)) {

		//
		// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- Imprime Atributos Premissas
		//

		if (calcular_att_operacionais_processo_estocastico_hidrologico != 0) {

			a_entradaSaidaDados.setAppendArquivo(false);

			a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

			if (calcular_att_operacionais_processo_estocastico_hidrologico > 0)
				processoEstocastico_hidrologico = ProcessoEstocastico();

			instanciarProcessoEstocasticoHidrologicoComHistoricoAfluenciaIncremental(tipo_processo_estocastico_hidrologico, processoEstocastico_hidrologico);

			a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa + "//ProcessoEstocasticoHidrologico");

			//
			// Identifica se deve ser realizada uma redução do espaço amostral
			//

			bool realizar_reducao_espaco_amostral = false;

			
			for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {
				if (idEstagio != estagio_inicial && getElementoVetor(AttVetorDados_numero_aberturas, idEstagio, int()) < getAtributo(AttComumDados_numero_aberturas, int())) {
					realizar_reducao_espaco_amostral = true;
					break;
				}//if (idEstagio != estagio_inicial && getElementoVetor(AttVetorDados_numero_aberturas, idEstagio, int()) < getAtributo(AttComumDados_numero_aberturas, int())) {
			}//for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

			//
			// Parametrizacao do Modelo do Processo Estocastico Hidrologico
			//

			processoEstocastico_hidrologico.parametrizarModelo(a_entradaSaidaDados, imprimir_parametros_hidrologico, tipo_modelo_geracao_sintetica, tipo_coeficiente_auto_correlacao, ordem_maxima_auto_correlacao_hidrologica, tipo_correlacao_variaveis_aleatorias, correlacao_dominante);


			if (idProcesso == IdProcesso_mestre) {

				if (realizar_reducao_espaco_amostral)
					processoEstocastico_hidrologico.avaliarModeloViaSerieSintetica(a_entradaSaidaDados, getHorizonteEspacoAmostralHidrologico(estagio_inicial, estagio_final, IdRealizacao(getAtributo(AttComumDados_numero_aberturas, int())), true), getAtributo(AttComumDados_tipo_sorteio_espaco_amostral_geracao_cenario_hidrologico, TipoSorteio()), getAtributo(AttComumDados_numero_periodos_avaliacao_geracao_cenario_hidrologico, int()));
				else
					processoEstocastico_hidrologico.avaliarModeloViaSerieSintetica(a_entradaSaidaDados, getHorizonteEspacoAmostralHidrologico(estagio_inicial, estagio_final), getAtributo(AttComumDados_tipo_sorteio_espaco_amostral_geracao_cenario_hidrologico, TipoSorteio()), getAtributo(AttComumDados_numero_periodos_avaliacao_geracao_cenario_hidrologico, int()));

			}//if (idProcesso == IdProcesso_mestre) {

			//
			// Geracao de Amostra Comum do Processo Estocastico Hidrologico
			//
			int semente_espaco_amostral_hidrologico = getAtributo(AttComumDados_semente_espaco_amostral_geracao_cenario_hidrologico, int());

			if (realizar_reducao_espaco_amostral)
				processoEstocastico_hidrologico.gerarEspacoAmostralPorSorteio(a_entradaSaidaDados, imprimir_espaco_amostral_hidrologico, TipoRelaxacaoVariavelAleatoria_truncamento_penalizacao, getHorizonteEspacoAmostralHidrologico(estagio_inicial, estagio_final, IdRealizacao(getAtributo(AttComumDados_numero_aberturas, int())), true), getAtributo(AttComumDados_tipo_sorteio_espaco_amostral_geracao_cenario_hidrologico, TipoSorteio()), semente_espaco_amostral_hidrologico);
			else
				processoEstocastico_hidrologico.gerarEspacoAmostralPorSorteio(a_entradaSaidaDados, imprimir_espaco_amostral_hidrologico, TipoRelaxacaoVariavelAleatoria_truncamento_penalizacao, getHorizonteEspacoAmostralHidrologico(estagio_inicial, estagio_final), getAtributo(AttComumDados_tipo_sorteio_espaco_amostral_geracao_cenario_hidrologico, TipoSorteio()), semente_espaco_amostral_hidrologico);



			//
			// Reducao Espaco Amostral
			//
			if (realizar_reducao_espaco_amostral) {

				const TipoEstudo tipo_estudo = getAtributo(AttComumDados_tipo_estudo, TipoEstudo());

				std::string diretorio_reducao_cenarios = "";

				if (tipo_estudo == TipoEstudo_simulacao)
					diretorio_reducao_cenarios = a_entradaSaidaDados.getDiretorioEntrada() + "//Simulacao//ReducaoCenarios";
				else
					diretorio_reducao_cenarios = a_entradaSaidaDados.getDiretorioEntrada() + "//Otimizacao//ReducaoCenarios";

				TipoSolver tipo_solver_para_reducao = getAtributo(AttComumDados_tipo_solver, TipoSolver());
				processoEstocastico_hidrologico.reducao_espaco_amostral(a_entradaSaidaDados, diretorio_reducao_cenarios, idProcesso, getVetor(AttVetorDados_numero_aberturas, IdEstagio(), int()), getVetor(AttVetorDados_horizonte_otimizacao, IdEstagio(), Periodo()), getAtributo(AttComumDados_estagio_acoplamento_pre_estudo, IdEstagio()), estagio_final, tipo_solver_para_reducao);

			}//if (realizar_reducao_espaco_amostral) {

			//
			// Geracao de Cenarios Via Amostra Comum do Processo Estocastico Hidrologico
			//

			int semente_geracao_cenario_hidrologico = getAtributo(AttComumDados_semente_geracao_cenario_hidrologico, int());
			processoEstocastico_hidrologico.gerarCenariosPorSorteio(a_entradaSaidaDados, imprimir_cenarios_hidrologicos, true, true, getAtributo(AttComumDados_numero_cenarios, int()), cenario_inicial, cenario_final, TipoSorteio_uniforme, semente_geracao_cenario_hidrologico);



		} // if (calcular_att_operacionais_processo_estocastico_hidrologico) {



		if ((calcular_att_operacionais_processo_estocastico_hidrologico != 0) || (a_imprimir_atributos_sem_recarregar)) {


			//
			// Imprime Atributos Operacionais
			//

			a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional + "//ProcessoEstocasticoHidrologico");

			a_entradaSaidaDados.setAppendArquivo(false);

			if (idProcesso == IdProcesso_mestre) {

				a_entradaSaidaDados.imprimirArquivoCSV_AttComum("PROCESSO_ESTOCASTICO_AttComumOperacional.csv", processoEstocastico_hidrologico);

				if (processoEstocastico_hidrologico.getSizeMatriz(AttMatrizProcessoEstocastico_probabilidade_realizacao) > 0)
					a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("PROCESSO_ESTOCASTICO_" + getString(AttMatrizProcessoEstocastico_probabilidade_realizacao) + ".csv", processoEstocastico_hidrologico, AttMatrizProcessoEstocastico_probabilidade_realizacao);

				//a_entradaSaidaDados.imprimirArquivoCSV_AttComum("VARIAVEL_ALEATORIA_AttComumOperacional.csv", IdVariavelAleatoria_Nenhum, processoEstocastico_hidrologico);

				a_entradaSaidaDados.imprimirArquivoCSV_AttComum("VARIAVEL_ALEATORIA_INTERNA_AttComumOperacional.csv", IdVariavelAleatoria_Nenhum, IdVariavelAleatoriaInterna_Nenhum, processoEstocastico_hidrologico, std::vector<AttComumVariavelAleatoriaInterna>{AttComumVariavelAleatoriaInterna_idVariavelAleatoriaInterna, AttComumVariavelAleatoriaInterna_nome, AttComumVariavelAleatoriaInterna_grau_liberdade});

				if (processoEstocastico_hidrologico.getSize1Matriz(IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_residuo_espaco_amostral) > 0)
					a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("VARIAVEL_ALEATORIA_" + getString(AttMatrizVariavelAleatoria_residuo_espaco_amostral) + ".csv", IdVariavelAleatoria_Nenhum, processoEstocastico_hidrologico, AttMatrizVariavelAleatoria_residuo_espaco_amostral);

				if (processoEstocastico_hidrologico.getSize1Matriz(IdVariavelAleatoria_1, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao) > 0) {
					const SmartEnupla<Periodo, SmartEnupla<int, double>> horizonte_coeficiente_linear_auto_correlacao = processoEstocastico_hidrologico.vetorVariavelAleatoria.att(IdVariavelAleatoria_1).getMatriz(AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao, Periodo(), int(), double());
					int maior_ordem_horizonte = 1;
					for (Periodo periodo = horizonte_coeficiente_linear_auto_correlacao.getIteradorInicial(); periodo <= horizonte_coeficiente_linear_auto_correlacao.getIteradorFinal(); horizonte_coeficiente_linear_auto_correlacao.incrementarIterador(periodo)) {
						const int maior_ordem = processoEstocastico_hidrologico.getMaiorOrdemAutocorrelacaoLinear(periodo);
						if (maior_ordem > maior_ordem_horizonte)
							maior_ordem_horizonte = maior_ordem;
					}
					a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("VARIAVEL_ALEATORIA_" + getString(AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao) + ".csv", IdVariavelAleatoria_Nenhum, processoEstocastico_hidrologico, horizonte_coeficiente_linear_auto_correlacao.getIteradorInicial(), horizonte_coeficiente_linear_auto_correlacao.getIteradorFinal(), 1, maior_ordem_horizonte, AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao);
				}

				a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("VARIAVEL_ALEATORIA_" + getString(AttVetorVariavelAleatoria_tipo_relaxacao) + ".csv", IdVariavelAleatoria_Nenhum, processoEstocastico_hidrologico, AttVetorVariavelAleatoria_tipo_relaxacao);

				a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("VARIAVEL_ALEATORIA_INTERNA_" + getString(AttVetorVariavelAleatoriaInterna_coeficiente_participacao) + ".csv", IdVariavelAleatoria_Nenhum, IdVariavelAleatoriaInterna_Nenhum, processoEstocastico_hidrologico, AttVetorVariavelAleatoriaInterna_coeficiente_participacao);

				if (processoEstocastico_hidrologico.getSizeVetor(IdVariavelAleatoria_1, IdVariavelAleatoriaInterna_1, AttVetorVariavelAleatoriaInterna_tendencia_temporal) > 0)
					a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("VARIAVEL_ALEATORIA_INTERNA_" + getString(AttVetorVariavelAleatoriaInterna_tendencia_temporal) + ".csv", IdVariavelAleatoria_Nenhum, IdVariavelAleatoriaInterna_Nenhum, processoEstocastico_hidrologico, AttVetorVariavelAleatoriaInterna_tendencia_temporal);

				if (processoEstocastico_hidrologico.getSizeMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral) > 0)
					a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("PROCESSO_ESTOCASTICO_" + getString(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral) + ".csv", processoEstocastico_hidrologico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral);

			} // if (idProcesso == IdProcesso_mestre) {

			MPI_Barrier(MPI_COMM_WORLD);

			a_entradaSaidaDados.setDiretorioEntrada(a_diretorio_att_operacional);

		} // if (calcular_att_operacionais_processo_estocastico_hidrologico) {	


		if (getAtributo(AttComumDados_imprimir_exportacao_pos_estudo, bool())) {

			const IdVariavelAleatoria maiorIdVariavelAleatoria = processoEstocastico_hidrologico.getMaiorId(IdVariavelAleatoria());

			const IdProcessoEstocastico  tipo_processo_estocastico_hidrologico = processoEstocastico_hidrologico.getAtributo(AttComumProcessoEstocastico_idProcessoEstocastico, IdProcessoEstocastico());

			if (calcular_att_operacionais_processo_estocastico_hidrologico == 0) {

				if (processoEstocastico_hidrologico.vetorVariavelAleatoria.att(IdVariavelAleatoria_1).vetorVariavelAleatoriaInterna.att(IdVariavelAleatoriaInterna_1).getSizeMatriz(AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral) == 0) {

					int semente_geracao_cenario_hidrologico = getAtributo(AttComumDados_semente_geracao_cenario_hidrologico, int());
					processoEstocastico_hidrologico.gerarCenariosPorSorteio(a_entradaSaidaDados, false, true, true, getAtributo(AttComumDados_numero_cenarios, int()), cenario_inicial, cenario_final, TipoSorteio_uniforme, semente_geracao_cenario_hidrologico);

				} // if (processoEstocastico_hidrologico.vetorVariavelAleatoria.att(IdVariavelAleatoria_1).vetorVariavelAleatoriaInterna.att(IdVariavelAleatoriaInterna_1).getSizeMatriz(AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral) == 0) {

			} // if (calcular_att_operacionais_processo_estocastico_hidrologico == 0) {

			//const IdCenario maiorIdCenario = processoEstocastico_hidrologico.vetorVariavelAleatoria.att(IdVariavelAleatoria_1).vetorVariavelAleatoriaInterna.att(IdVariavelAleatoriaInterna_1).getMatriz(AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral, IdCenario(), Periodo(), double()).getIteradorFinal();
			const SmartEnupla <Periodo, double> horizonte_afluencia = processoEstocastico_hidrologico.vetorVariavelAleatoria.att(IdVariavelAleatoria_1).vetorVariavelAleatoriaInterna.att(IdVariavelAleatoriaInterna_1).getMatriz(AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral, IdCenario(), Periodo(), double()).getElemento(cenario_inicial);

			const IdHidreletrica maiorIdHidreletrica = getMaiorId(IdHidreletrica());

			for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= maiorIdVariavelAleatoria; idVariavelAleatoria++) {

				const IdVariavelAleatoriaInterna maiorIdVariavelAleatoriaInterna = processoEstocastico_hidrologico.getMaiorId(idVariavelAleatoria, IdVariavelAleatoriaInterna());

				for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= maiorIdVariavelAleatoriaInterna; idVariavelAleatoriaInterna++) {

					IdHidreletrica idHidreletrica_YHF = IdHidreletrica_Nenhum;

					for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {
						if (getFullString(idHidreletrica) == processoEstocastico_hidrologico.vetorVariavelAleatoria.att(idVariavelAleatoria).vetorVariavelAleatoriaInterna.att(idVariavelAleatoriaInterna).getAtributo(AttComumVariavelAleatoriaInterna_nome, std::string())) {
							idHidreletrica_YHF = idHidreletrica;
							break;
						} // if (getFullString(idHidreletrica) == getAtributo(tipo_processo_estocastico_hidrologico, idVariavelAleatoria, idVariavelAleatoriaInterna, AttComumVariavelAleatoriaInterna_nome, std::string())) {
					} // for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

					if (idHidreletrica_YHF == IdHidreletrica_Nenhum)
						throw std::invalid_argument("Nao ha hidreletrica compativel com " + getFullString(idVariavelAleatoriaInterna) + " em " + getFullString(idVariavelAleatoria));
					if (!vetorHidreletrica.att(idHidreletrica_YHF).vetorAfluencia.isInstanciado(IdAfluencia_vazao_afluente)) {
						Afluencia afluencia;
						afluencia.setAtributo(AttComumAfluencia_idAfluencia, IdAfluencia_vazao_afluente);
						vetorHidreletrica.att(idHidreletrica_YHF).vetorAfluencia.add(afluencia);
					}


					SmartEnupla<IdCenario, SmartEnupla<Periodo, double>> cenarios_realizacao_espaco_amostral = processoEstocastico_hidrologico.vetorVariavelAleatoria.att(idVariavelAleatoria).vetorVariavelAleatoriaInterna.att(idVariavelAleatoriaInterna).getMatriz(AttMatrizVariavelAleatoriaInterna_cenarios_realizacao_espaco_amostral, IdCenario(), Periodo(), double());

					//Inicializa AttMatrizAfluencia_incremental_tendencia	
					vetorHidreletrica.att(idHidreletrica_YHF).vetorAfluencia.att(IdAfluencia_vazao_afluente).setMatriz_forced(AttMatrizAfluencia_incremental, cenarios_realizacao_espaco_amostral);

					//Inicializa AttMatrizAfluencia_natural_tendencia			
					vetorHidreletrica.att(idHidreletrica_YHF).vetorAfluencia.att(IdAfluencia_vazao_afluente).setMatriz_forced(AttMatrizAfluencia_natural, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>(cenario_inicial, std::vector <SmartEnupla<Periodo, double>>(int(cenario_final - cenario_inicial) + 1, SmartEnupla<Periodo, double>(horizonte_afluencia, 0.0))));

				}//	for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= maiorIdVariavelAleatoriaInterna; idVariavelAleatoriaInterna++) {

			}//for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= maiorIdVariavelAleatoria; idVariavelAleatoria++) {

			////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Atualiza AttMatrizAfluencia_natural_tendencia com os valores de AttMatrizAfluencia_incremental_tendencia
			////////////////////////////////////////////////////////////////////////////////////////////////////////////

			for (Periodo periodo = horizonte_afluencia.getIteradorInicial(); periodo <= horizonte_afluencia.getIteradorFinal(); horizonte_afluencia.incrementarIterador(periodo)) {

				for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

					for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

						const double afluencia_incremental = getElementoMatriz(idHidreletrica, IdAfluencia_vazao_afluente, AttMatrizAfluencia_incremental, idCenario, periodo, double());

						//Atualiza afluencia_natural para a pr�pria usina e para todas as usinas a jusante da idHidreletrica

						IdHidreletrica idhidreletrica_jusante = idHidreletrica;

						while (idhidreletrica_jusante != IdHidreletrica_Nenhum) {

							double afluencia_natural = getElementoMatriz(idhidreletrica_jusante, IdAfluencia_vazao_afluente, AttMatrizAfluencia_natural, idCenario, periodo, double());
							afluencia_natural += afluencia_incremental;

							vetorHidreletrica.att(idhidreletrica_jusante).vetorAfluencia.att(IdAfluencia_vazao_afluente).setElemento(AttMatrizAfluencia_natural, idCenario, periodo, afluencia_natural);

							idhidreletrica_jusante = vetorHidreletrica.att(idhidreletrica_jusante).getAtributo(AttComumHidreletrica_jusante, IdHidreletrica());

						}//while (idhidreletrica_jusante != IdHidreletrica_Nenhum) {

					}//for (Periodo periodo = horizonte_afluencia_tendencia.getIteradorInicial(); periodo <= horizonte_afluencia_tendencia.getIteradorFinal(); horizonte_afluencia_tendencia.incrementarIterador(periodo)) {

				}//for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

			}//if (is_possivel_contabilizar_incremental && !is_idHidreletrica_contabilizada.getElemento(idHidreletrica)) {		

			if ((getSizeVetor(getMenorId(IdHidreletrica()), IdAfluencia_vazao_afluente, AttVetorAfluencia_natural_historico) == 0) && (getSizeVetor(getMenorId(IdHidreletrica()), IdAfluencia_vazao_afluente, AttVetorAfluencia_incremental_historico) > 0)) {

				const SmartEnupla<Periodo, double> horizonte_historico = SmartEnupla<Periodo, double>(vetorHidreletrica.att(getMenorId(IdHidreletrica())).vetorAfluencia.att(IdAfluencia_vazao_afluente).getVetor(AttVetorAfluencia_incremental_historico, Periodo(), double()), 0.0);

				for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica))
					vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).setVetor_forced(AttVetorAfluencia_natural_historico, horizonte_historico);

				for (Periodo periodo = horizonte_historico.getIteradorInicial(); periodo <= horizonte_historico.getIteradorFinal(); horizonte_historico.incrementarIterador(periodo)) {

					for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

						const double afluencia_incremental = vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).getElementoVetor(AttVetorAfluencia_incremental_historico, periodo, double());

						//Atualiza afluencia_natural para a pr�pria usina e para todas as usinas a jusante da idHidreletrica

						IdHidreletrica idhidreletrica_jusante = idHidreletrica;

						while (idhidreletrica_jusante != IdHidreletrica_Nenhum) {

							double afluencia_natural = vetorHidreletrica.att(idhidreletrica_jusante).vetorAfluencia.att(IdAfluencia_vazao_afluente).getElementoVetor(AttVetorAfluencia_natural_historico, periodo, double());
							afluencia_natural += afluencia_incremental;

							vetorHidreletrica.att(idhidreletrica_jusante).vetorAfluencia.att(IdAfluencia_vazao_afluente).setElemento(AttVetorAfluencia_natural_historico, periodo, afluencia_natural);

							idhidreletrica_jusante = vetorHidreletrica.att(idhidreletrica_jusante).getAtributo(AttComumHidreletrica_jusante, IdHidreletrica());

						}//while (idhidreletrica_jusante != IdHidreletrica_Nenhum) {

					}//for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

				}//for (Periodo periodo = horizonte_historico.getIteradorInicial(); periodo <= horizonte_historico.getIteradorFinal(); horizonte_historico.incrementarIterador(periodo)) {

			} // if ((getSizeVetor(getMenorId(IdHidreletrica()), IdAfluencia_vazao_afluente, AttVetorAfluencia_natural_historico) == 0) && (getSizeVetor(getMenorId(IdHidreletrica()), IdAfluencia_vazao_afluente, AttVetorAfluencia_incremental_historico) > 0)) {

			////////////////////////////////////////////////////////////////////////////////////////////////////////////

			MPI_Barrier(MPI_COMM_WORLD);

			a_entradaSaidaDados.setDiretorioSaida(a_diretorio_exportacao_pos_estudo);

			if ((idProcesso == IdProcesso_mestre) && (getSizeVetor(getMenorId(IdHidreletrica()), IdAfluencia_vazao_afluente, AttVetorAfluencia_natural_historico) > 0))
				a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("HIDRELETRICA_AFLUENCIA_AttVetorPremissa_PorPeriodo.csv", IdHidreletrica_Nenhum, IdAfluencia_Nenhum, *this, AttVetorAfluencia_natural_historico);

			if ((idProcesso == IdProcesso_mestre) && (getSizeVetor(getMenorId(IdHidreletrica()), IdAfluencia_vazao_afluente, AttVetorAfluencia_incremental_historico) > 0))
				a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_HIDRELETRICA_AFLUENCIA_AttVetorPremissa_PorPeriodo.csv", IdHidreletrica_Nenhum, IdAfluencia_Nenhum, *this, AttVetorAfluencia_incremental_historico);

			int imprimir_HIDRELETRICA_AFLUENCIA_AttMatrizPremissa = 0;

			a_entradaSaidaDados.setAppendArquivo(true);

			if (idProcesso == IdProcesso_mestre) {
				imprimir_HIDRELETRICA_AFLUENCIA_AttMatrizPremissa = 1;
				a_entradaSaidaDados.setAppendArquivo(false);
			}
			else
				MPI_Recv(&imprimir_HIDRELETRICA_AFLUENCIA_AttMatrizPremissa, 1, MPI_INT, getRank(IdProcesso(idProcesso - 1)), getRank(idProcesso), MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			if (imprimir_HIDRELETRICA_AFLUENCIA_AttMatrizPremissa) {
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("HIDRELETRICA_AFLUENCIA_AttMatrizPremissa_PorCenarioPorPeriodo.csv", IdHidreletrica_Nenhum, IdAfluencia_Nenhum, *this, AttMatrizAfluencia_natural);
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("_info_HIDRELETRICA_AFLUENCIA_AttMatrizPremissa_PorCenarioPorPeriodo.csv", IdHidreletrica_Nenhum, IdAfluencia_Nenhum, *this, AttMatrizAfluencia_incremental);
			}
			else
				throw std::invalid_argument("Erro impressao de " + getFullString(AttMatrizAfluencia_natural) + " em " + getFullString(idProcesso));

			if (arranjoResolucao.getMaiorId(IdProcesso()) != idProcesso)
				MPI_Send(&imprimir_HIDRELETRICA_AFLUENCIA_AttMatrizPremissa, 1, MPI_INT, getRank(IdProcesso(idProcesso + 1)), getRank(IdProcesso(idProcesso + 1)), MPI_COMM_WORLD);

			MPI_Barrier(MPI_COMM_WORLD);

		}// if (getAtributo(AttComumDados_imprimir_exportacao_pos_estudo, bool())) {

		//
		// Esvazia Atributos
		//

		for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= getMaiorId(IdHidreletrica()); vetorHidreletrica.incr(idHidreletrica))
			vetorHidreletrica.att(idHidreletrica).vetorAfluencia.~VetorAfluenciaEmHidreletrica();

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validacao_operacional_ProcessoEstocasticoHidrologico(a_entradaSaidaDados, " + a_diretorio_att_operacional + "," + a_diretorio_att_premissa + "): \n" + std::string(erro.what())); }

} // void Dados::validacao_operacional_ProcessoEstocasticoHidrologico(EntradaSaidaDados & a_entrada_saida_dados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar){



void Dados::instanciarProcessoEstocasticoHidrologicoComHistoricoAfluenciaIncremental(const IdProcessoEstocastico a_tipo_processo_estocastico, ProcessoEstocastico& a_processo_estocastico) {
	try {

		if ((a_tipo_processo_estocastico != IdProcessoEstocastico_hidrologico_bacia) && (a_tipo_processo_estocastico != IdProcessoEstocastico_hidrologico_hidreletrica))
			throw std::invalid_argument("O metodo serve apenas para " + getFullString(IdProcessoEstocastico_hidrologico_bacia) + " e " + getFullString(IdProcessoEstocastico_hidrologico_hidreletrica));

		bool isSerieTemporalAdicionadaEmVarExistente = false;
		bool isTendenciaTemporalAdicionadaEmVarExistente = false;

		bool isProcEstocasticoJaInstanciado = false;
		if (a_processo_estocastico.getMaiorId(IdVariavelAleatoria()) != IdVariavelAleatoria_Nenhum)
			isProcEstocasticoJaInstanciado = true;

		if (a_tipo_processo_estocastico == IdProcessoEstocastico_hidrologico_hidreletrica) {

			a_processo_estocastico.setAtributo(AttComumProcessoEstocastico_idProcessoEstocastico, IdProcessoEstocastico_hidrologico_hidreletrica);

			for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= getMaiorId(IdHidreletrica()); vetorHidreletrica.incr(idHidreletrica)) {

				// Instancia variaveis aleatórias e variaveis aleatórias internas
				if (!isProcEstocasticoJaInstanciado) {

					const SmartEnupla<int, IdHidreletrica>                variavel_aleatoria_interna(1, std::vector<IdHidreletrica>(1, idHidreletrica));
					const SmartEnupla<int, SmartEnupla<Periodo, double>>  serie_temporal_variavel_aleatoria_interna(1, std::vector<SmartEnupla<Periodo, double>>(1, getVetor(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_incremental_historico, Periodo(), double())));
					const SmartEnupla<int, SmartEnupla<Periodo, double>>  tendencia_temporal_variavel_aleatoria_interna(1, std::vector<SmartEnupla<Periodo, double>>(1, getVetor(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_incremental_tendencia, Periodo(), double())));

					a_processo_estocastico.addSeriesTemporais(idHidreletrica, variavel_aleatoria_interna, serie_temporal_variavel_aleatoria_interna);
					a_processo_estocastico.addTendenciasTemporais(idHidreletrica, variavel_aleatoria_interna, tendencia_temporal_variavel_aleatoria_interna);

				} // if (a_processo_estocastico.getMaiorId(IdVariavelAleatoria()) != IdVariavelAleatoria_Nenhum) {

				else {

					IdVariavelAleatoria        idVar = IdVariavelAleatoria_Nenhum;
					IdVariavelAleatoriaInterna idVarInterna = IdVariavelAleatoriaInterna_Nenhum;

					a_processo_estocastico.getIdVariavelAleatoriaIdVariavelAleatoriaInternaFromIdFisico(idVar, idVarInterna, idHidreletrica);

					if (strCompara(a_processo_estocastico.getAtributo(idVar, AttComumVariavelAleatoria_nome, std::string()), "nenhum"))
						a_processo_estocastico.vetorVariavelAleatoria.att(idVar).setAtributo(AttComumVariavelAleatoria_nome, getFullString(idHidreletrica));

					if (a_processo_estocastico.getSizeVetor(idVar, idVarInterna, AttVetorVariavelAleatoriaInterna_serie_temporal) == 0) {
						a_processo_estocastico.vetorVariavelAleatoria.att(idVar).addSerieTemporalVariavelAleatoriaInterna(idVarInterna, getVetor(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_incremental_historico, Periodo(), double()));
						isSerieTemporalAdicionadaEmVarExistente = true;
					}


					if (a_processo_estocastico.getSizeVetor(idVar, idVarInterna, AttVetorVariavelAleatoriaInterna_tendencia_temporal) == 0) {
						a_processo_estocastico.vetorVariavelAleatoria.att(idVar).addTendenciaTemporalVariavelAleatoriaInterna(idVarInterna, getVetor(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_incremental_tendencia, Periodo(), double()));
						isTendenciaTemporalAdicionadaEmVarExistente = true;
					}

				}

			} // for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= getMaiorId(IdHidreletrica()); vetorHidreletrica.incr(idHidreletrica)) {

		} // if (a_tipo_processo_estocastico == IdProcessoEstocastico_hidrologico_hidreletrica) {

		else if (a_tipo_processo_estocastico == IdProcessoEstocastico_hidrologico_bacia) {

			a_processo_estocastico.setAtributo(AttComumProcessoEstocastico_idProcessoEstocastico, IdProcessoEstocastico_hidrologico_bacia);

			const IdBaciaHidrografica maiorIdBaciaHidrografica = getMaiorId(IdBaciaHidrografica());

			for (IdBaciaHidrografica idBaciaHidrografica = IdBaciaHidrografica_1; idBaciaHidrografica <= maiorIdBaciaHidrografica; idBaciaHidrografica++) {

				SmartEnupla<int, IdHidreletrica>               variavel_aleatoria_interna;
				SmartEnupla<int, SmartEnupla<Periodo, double>> serie_temporal_variavel_aleatoria_interna;
				SmartEnupla<int, SmartEnupla<Periodo, double>> tendencia_temporal_variavel_aleatoria_interna;

				int contador = 1;
				for (IdHidreletrica idHidreletrica = getMaiorId(IdHidreletrica()); idHidreletrica <= getMaiorId(IdHidreletrica()); vetorHidreletrica.incr(idHidreletrica)) {

					if (getAtributo(idHidreletrica, AttComumHidreletrica_bacia, IdBaciaHidrografica()) == idBaciaHidrografica) {

						// Instancia variaveis aleatórias e variaveis aleatórias internas
						if (!isProcEstocasticoJaInstanciado) {

							variavel_aleatoria_interna.addElemento(contador, idHidreletrica);
							serie_temporal_variavel_aleatoria_interna.addElemento(contador, getVetor(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_incremental_historico, Periodo(), double()));
							tendencia_temporal_variavel_aleatoria_interna.addElemento(contador, getVetor(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_incremental_tendencia, Periodo(), double()));

						} // if (a_processo_estocastico.getMaiorId(IdVariavelAleatoria()) != IdVariavelAleatoria_Nenhum) {

						else {

							IdVariavelAleatoria        idVar = IdVariavelAleatoria_Nenhum;
							IdVariavelAleatoriaInterna idVarInterna = IdVariavelAleatoriaInterna_Nenhum;

							a_processo_estocastico.getIdVariavelAleatoriaIdVariavelAleatoriaInternaFromIdFisico(idVar, idVarInterna, idHidreletrica);

							if (strCompara(a_processo_estocastico.getAtributo(idVar, AttComumVariavelAleatoria_nome, std::string()), "nenhum"))
								a_processo_estocastico.vetorVariavelAleatoria.att(idVar).setAtributo(AttComumVariavelAleatoria_nome, getFullString(idBaciaHidrografica));

							if (a_processo_estocastico.getSizeVetor(idVar, idVarInterna, AttVetorVariavelAleatoriaInterna_serie_temporal) == 0) {
								a_processo_estocastico.vetorVariavelAleatoria.att(idVar).addSerieTemporalVariavelAleatoriaInterna(idVarInterna, getVetor(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_incremental_historico, Periodo(), double()));
								isSerieTemporalAdicionadaEmVarExistente = true;
							}


							if (a_processo_estocastico.getSizeVetor(idVar, idVarInterna, AttVetorVariavelAleatoriaInterna_tendencia_temporal) == 0) {
								a_processo_estocastico.vetorVariavelAleatoria.att(idVar).addTendenciaTemporalVariavelAleatoriaInterna(idVarInterna, getVetor(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_incremental_tendencia, Periodo(), double()));
								isTendenciaTemporalAdicionadaEmVarExistente = true;
							}

						}

						contador++;

					} // if (getAtributo(idHidreletrica, AttComumHidreletrica_bacia, IdBaciaHidrografica()) == idBaciaHidrografica){

				} // for (IdHidreletrica idHidreletrica = getMaiorId(IdHidreletrica()); idHidreletrica <= getMaiorId(IdHidreletrica()); vetorHidreletrica.incr(idHidreletrica)) {

				if (variavel_aleatoria_interna.size() > 0) {
					a_processo_estocastico.addSeriesTemporais(idBaciaHidrografica, variavel_aleatoria_interna, serie_temporal_variavel_aleatoria_interna);
					a_processo_estocastico.addTendenciasTemporais(idBaciaHidrografica, variavel_aleatoria_interna, tendencia_temporal_variavel_aleatoria_interna);
				}

			} // for (IdBaciaHidrografica idBaciaHidrografica = IdBaciaHidrografica_1; idBaciaHidrografica <= maiorIdBaciaHidrografica; idBaciaHidrografica++) {

		} // else if (a_tipo_processo_estocastico == IdProcessoEstocastico_hidrologico_bacia) {

		if (isSerieTemporalAdicionadaEmVarExistente || isTendenciaTemporalAdicionadaEmVarExistente) {

			for (IdVariavelAleatoria idVar = a_processo_estocastico.getMenorId(IdVariavelAleatoria()); idVar <= a_processo_estocastico.getMaiorId(IdVariavelAleatoria()); idVar++) {

				a_processo_estocastico.vetorVariavelAleatoria.att(idVar).setAtributo(AttComumVariavelAleatoria_idVariavelAleatoria_determinacao, idVar);

				if (isTendenciaTemporalAdicionadaEmVarExistente)
					a_processo_estocastico.vetorVariavelAleatoria.att(idVar).calcularTendenciaTemporal();

				if (isSerieTemporalAdicionadaEmVarExistente)
					a_processo_estocastico.vetorVariavelAleatoria.att(idVar).calcularSerieTemporal();

			} // for (IdVariavelAleatoria idVar = a_processo_estocastico.getMenorId(IdVariavelAleatoria()); idVar <= a_processo_estocastico.getMaiorId(IdVariavelAleatoria()); idVar++) {

		} // if (isSerieTemporalAdicionadaEmVarExistente || isTendenciaTemporalAdicionadaEmVarExistente) {


	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::instanciarProcessoEstocasticoHidrologicoComHistoricoAfluenciaIncremental(): \n" + std::string(erro.what())); }
} // void Dados::instanciarProcessoEstocasticoHidrologico(const IdProcessoEstocastico a_tipo_processo_estocastico) {


void Dados::calcularDeterminacaoEspacialFromHistoricoAfluenciaNatural(EntradaSaidaDados a_entradaSaidaDados, const bool a_imprimir, const double a_valor_r2, ProcessoEstocastico& a_processo_estocastico){

	try {

		if ((a_valor_r2 <= 0.0) || (1.0 <= a_valor_r2))
			return;

		if ((getSizeVetor(getMenorId(IdHidreletrica()), IdAfluencia_vazao_afluente, AttVetorAfluencia_natural_historico) == 0) && (getSizeVetor(getMenorId(IdHidreletrica()), IdAfluencia_vazao_afluente, AttVetorAfluencia_incremental_historico) > 0)) {

			const SmartEnupla<Periodo, double> horizonte_historico = SmartEnupla<Periodo, double>(vetorHidreletrica.att(getMenorId(IdHidreletrica())).vetorAfluencia.att(IdAfluencia_vazao_afluente).getVetor(AttVetorAfluencia_incremental_historico, Periodo(), double()), 0.0);

			for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= getMaiorId(IdHidreletrica()); vetorHidreletrica.incr(idHidreletrica))
				vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).setVetor_forced(AttVetorAfluencia_natural_historico, horizonte_historico);

			for (Periodo periodo = horizonte_historico.getIteradorInicial(); periodo <= horizonte_historico.getIteradorFinal(); horizonte_historico.incrementarIterador(periodo)) {

				for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= getMaiorId(IdHidreletrica()); vetorHidreletrica.incr(idHidreletrica)) {

					const double afluencia_incremental = vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).getElementoVetor(AttVetorAfluencia_incremental_historico, periodo, double());

					//Atualiza afluencia_natural para a pr�pria usina e para todas as usinas a jusante da idHidreletrica

					IdHidreletrica idhidreletrica_jusante = idHidreletrica;

					while (idhidreletrica_jusante != IdHidreletrica_Nenhum) {

						double afluencia_natural = vetorHidreletrica.att(idhidreletrica_jusante).vetorAfluencia.att(IdAfluencia_vazao_afluente).getElementoVetor(AttVetorAfluencia_natural_historico, periodo, double());
						afluencia_natural += afluencia_incremental;

						vetorHidreletrica.att(idhidreletrica_jusante).vetorAfluencia.att(IdAfluencia_vazao_afluente).setElemento(AttVetorAfluencia_natural_historico, periodo, afluencia_natural);

						idhidreletrica_jusante = vetorHidreletrica.att(idhidreletrica_jusante).getAtributo(AttComumHidreletrica_jusante, IdHidreletrica());

					}//while (idhidreletrica_jusante != IdHidreletrica_Nenhum) {

				}//for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

			}//for (Periodo periodo = horizonte_historico.getIteradorInicial(); periodo <= horizonte_historico.getIteradorFinal(); horizonte_historico.incrementarIterador(periodo)) {

		} // if ((getSizeVetor(getMenorId(IdHidreletrica()), IdAfluencia_vazao_afluente, AttVetorAfluencia_natural_historico) == 0) && (getSizeVetor(getMenorId(IdHidreletrica()), IdAfluencia_vazao_afluente, AttVetorAfluencia_incremental_historico) > 0)) {

		SmartEnupla<IdHidreletrica, double> inicializacao(getMenorId(IdHidreletrica()), std::vector<double>(int(getMaiorId(IdHidreletrica()) - getMenorId(IdHidreletrica())) + 1, 0.0));
		for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= getMaiorId(IdHidreletrica()); vetorHidreletrica.incr(idHidreletrica)) {
			
			vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).setVetor_forced(AttVetorAfluencia_r2_natural_historico, inicializacao);
			vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).setVetor_forced(AttVetorAfluencia_r2_incremental_historico, inicializacao);

			for (IdHidreletrica idHidreletrica_b = getMenorId(IdHidreletrica()); idHidreletrica_b <= getMaiorId(IdHidreletrica()); vetorHidreletrica.incr(idHidreletrica_b)) {

				if (idHidreletrica == idHidreletrica_b) {
					vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).setElemento(AttVetorAfluencia_r2_natural_historico, idHidreletrica_b, 1.0);
					vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).setElemento(AttVetorAfluencia_r2_incremental_historico, idHidreletrica_b, 1.0);
				}

				else if (getSizeVetor(idHidreletrica_b, IdAfluencia_vazao_afluente, AttVetorAfluencia_r2_natural_historico) > 0) {
					vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).setElemento(AttVetorAfluencia_r2_natural_historico, idHidreletrica_b, getElementoVetor(idHidreletrica_b, IdAfluencia_vazao_afluente, AttVetorAfluencia_r2_natural_historico, idHidreletrica, double()));
					vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).setElemento(AttVetorAfluencia_r2_incremental_historico, idHidreletrica_b, getElementoVetor(idHidreletrica_b, IdAfluencia_vazao_afluente, AttVetorAfluencia_r2_incremental_historico, idHidreletrica, double()));
				}
				else {
					vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).setElemento(AttVetorAfluencia_r2_natural_historico, idHidreletrica_b, getRQuadradoNormalizadoMaior(getVetor(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_natural_historico, Periodo(), double()), getVetor(idHidreletrica_b, IdAfluencia_vazao_afluente, AttVetorAfluencia_natural_historico, Periodo(), double())));
					vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).setElemento(AttVetorAfluencia_r2_incremental_historico, idHidreletrica_b, getRQuadradoNormalizadoMaior(getVetor(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_incremental_historico, Periodo(), double()), getVetor(idHidreletrica_b, IdAfluencia_vazao_afluente, AttVetorAfluencia_incremental_historico, Periodo(), double())));
				}
			}

		} // for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= getMaiorId(IdHidreletrica()); vetorHidreletrica.incr(idHidreletrica)) {

		if (a_imprimir) {
			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_HIDRELETRICA_AFLUENCIA_AttVetorPremissa_PorHidreletrica.csv", IdHidreletrica_Nenhum, IdAfluencia_Nenhum, *this, AttVetorAfluencia_r2_natural_historico);
			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_HIDRELETRICA_AFLUENCIA_AttVetorPremissa_PorHidreletrica2.csv", IdHidreletrica_Nenhum, IdAfluencia_Nenhum, *this, AttVetorAfluencia_r2_incremental_historico);
			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_HIDRELETRICA_AFLUENCIA_AttVetorPremissa_PorPeriodo.csv", IdHidreletrica_Nenhum, IdAfluencia_Nenhum, *this, AttVetorAfluencia_natural_historico);
		}
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::calcularDeterminacaoEspacialFromHistoricoAfluenciaNatural(" + getFullString(a_valor_r2) + "): \n" + std::string(erro.what())); }

} // void Dados::definirEquivalenciaEspacialFromHistoricoAfluenciaNatural(const double a_valor_r2, ProcessoEstocastico& a_processo_estocastico){




void Dados::validaReservatorioEmHidreletrica() {
	try {

		const IdHidreletrica maiorIdHidreletrica = getMaiorId(IdHidreletrica());

		const IdEstagio idEstagioFinal = getIteradorFinal(AttVetorDados_horizonte_otimizacao, IdEstagio());;

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

			const bool  considerar_usina = getAtributo(idHidreletrica, AttComumHidreletrica_considerar_usina, bool());

			if (considerar_usina) {

				const IdReservatorio maiorIdReservatorio = getMaiorId(idHidreletrica, IdReservatorio());

				if (maiorIdReservatorio > IdReservatorio_1)
					throw std::invalid_argument("Apenas um " + getString(Reservatorio()) + " deve ser adicionado na " + getString(Hidreletrica()) + " " + getString(idHidreletrica) + ".");

				if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_minimo) > 0 && getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo) > 0) {

					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
						if (getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_minimo, periodo, double()) > getElementoVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, periodo, double()))
							throw std::invalid_argument("Volume_util minima da  " + getFullString(idHidreletrica) + " no " + getFullString(periodo) + "  maior que o volume_util maxima");
					}//for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				}//if (getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_minimo) > 0 && getSizeVetor(idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo) > 0) {

			} // if (considerar_usina) {
		} // for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaReservatorioEmHidreletrica(): \n" + std::string(erro.what())); }
} // void Dados::validaReservatorioEmHidreletrica(){


bool Dados::validaFuncaoProducaoHidreletrica(FuncaoProducaoHidreletrica& a_funcaoProducaoHidreletrica, SmartEnupla<Periodo, double> a_horizonte_estudo, const IdHidreletrica a_idHidreletrica, const IdConjuntoHidraulico a_idConjuntoHidraulico, const IdUnidadeUHE a_idUnidadeUHE, SmartEnupla<AttVetorFuncaoProducaoHidreletrica, PreencherAtributo>& a_preencher_AttVetorFuncaoProducaoHidreletrica, SmartEnupla<AttMatrizFuncaoProducaoHidreletrica, PreencherAtributo>& a_preencher_AttMatrizFuncaoProducaoHidreletrica, SmartEnupla<AttVetorHidreletrica, PreencherAtributo>& a_preencher_AttVetorHidreletrica, SmartEnupla<AttVetorReservatorio, PreencherAtributo>& a_preencher_AttVetorReservatorio) {

	try {


		// Verifica se matrizes j� foram alocadas

		const int sizeMatriz_RHS = a_funcaoProducaoHidreletrica.getSizeMatriz(AttMatrizFuncaoProducaoHidreletrica_RHS);

		if (a_funcaoProducaoHidreletrica.getSizeMatriz(AttMatrizFuncaoProducaoHidreletrica_FC) != sizeMatriz_RHS)
			throw std::invalid_argument("O atributo " + getFullString(AttMatrizFuncaoProducaoHidreletrica_FC) + " deve possuir a mesma dimensao que " + getFullString(AttMatrizFuncaoProducaoHidreletrica_RHS));

		if (a_funcaoProducaoHidreletrica.getSizeMatriz(AttMatrizFuncaoProducaoHidreletrica_VH) != sizeMatriz_RHS)
			throw std::invalid_argument("O atributo " + getFullString(AttMatrizFuncaoProducaoHidreletrica_VH) + " deve possuir a mesma dimensao que " + getFullString(AttMatrizFuncaoProducaoHidreletrica_RHS));

		if (a_funcaoProducaoHidreletrica.getSizeMatriz(AttMatrizFuncaoProducaoHidreletrica_QH) != sizeMatriz_RHS)
			throw std::invalid_argument("O atributo " + getFullString(AttMatrizFuncaoProducaoHidreletrica_QH) + " deve possuir a mesma dimensao que " + getFullString(AttMatrizFuncaoProducaoHidreletrica_RHS));

		if (a_funcaoProducaoHidreletrica.getSizeMatriz(AttMatrizFuncaoProducaoHidreletrica_SH) != sizeMatriz_RHS)
			throw std::invalid_argument("O atributo " + getFullString(AttMatrizFuncaoProducaoHidreletrica_SH) + " deve possuir a mesma dimensao que " + getFullString(AttMatrizFuncaoProducaoHidreletrica_RHS));


		if (getAtributo(AttComumDados_estagio_final, IdEstagio()) > IdEstagio_45)
			vetorHidreletrica.att(a_idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).setAtributo(AttComumFuncaoProducaoHidreletrica_percentual_volume, 1.0);

		if (getSizeVetor(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttVetorFuncaoProducaoHidreletrica_percentual_volume_minimo) == 0)
			vetorHidreletrica.att(a_idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).setVetor(AttVetorFuncaoProducaoHidreletrica_percentual_volume_minimo, SmartEnupla<Periodo, double>(a_horizonte_estudo, getAtributo(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttComumFuncaoProducaoHidreletrica_percentual_volume, double())));

		if (getSizeVetor(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttVetorFuncaoProducaoHidreletrica_percentual_volume_maximo) == 0)
			vetorHidreletrica.att(a_idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).setVetor(AttVetorFuncaoProducaoHidreletrica_percentual_volume_maximo, SmartEnupla<Periodo, double>(a_horizonte_estudo, getAtributo(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttComumFuncaoProducaoHidreletrica_percentual_volume, double())));

		if (getSizeVetor(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttVetorFuncaoProducaoHidreletrica_volume_minimo) == 0) {
			vetorHidreletrica.att(a_idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).setVetor(AttVetorFuncaoProducaoHidreletrica_volume_minimo, SmartEnupla<Periodo, double>(a_horizonte_estudo, double(0.0)));

			for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo)) {
				double volume = getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo, periodo, double()) + getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, periodo, double());

				if (getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_regularizacao, periodo, int()) == 1) {
					volume = getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo, periodo, double()) + (getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_percentual_volume_util_inicial, double()) - \
						getElementoVetor(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttVetorFuncaoProducaoHidreletrica_percentual_volume_minimo, periodo, double())) * \
						getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, periodo, double());

					if (volume < getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo, periodo, double()))
						(volume = getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo, periodo, double()));
				}

				vetorHidreletrica.att(a_idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).setElemento(AttVetorFuncaoProducaoHidreletrica_volume_minimo, periodo, volume);
			}
		}//if (getSizeVetor(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttVetorFuncaoProducaoHidreletrica_volume_maximo) == 0) {


		if (getSizeVetor(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttVetorFuncaoProducaoHidreletrica_volume_maximo) == 0) {
			vetorHidreletrica.att(a_idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).setVetor(AttVetorFuncaoProducaoHidreletrica_volume_maximo, SmartEnupla<Periodo, double>(a_horizonte_estudo, double(0.0)));

			for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo)) {
				const double volumeMaximo = getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo, periodo, double()) + getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, periodo, double());
				double volume = volumeMaximo;

				if (getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_regularizacao, periodo, int()) == 1) {
					volume = getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo, periodo, double()) + (getElementoVetor(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttVetorFuncaoProducaoHidreletrica_percentual_volume_maximo, periodo, double()) + \
						getAtributo(a_idHidreletrica, IdReservatorio_1, AttComumReservatorio_percentual_volume_util_inicial, double())) * \
						getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, periodo, double());

					if (volume > volumeMaximo)
						(volume = volumeMaximo);
				}
				vetorHidreletrica.att(a_idHidreletrica).vetorFuncaoProducaoHidreletrica.att(IdFuncaoProducaoHidreletrica_1).setElemento(AttVetorFuncaoProducaoHidreletrica_volume_maximo, periodo, volume);
			}
		}//if (getSizeVetor(a_idHidreletrica, IdFuncaoProducaoHidreletrica_1, AttVetorFuncaoProducaoHidreletrica_volume_maximo) == 0) {


		if (sizeMatriz_RHS > 0) {

			for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo)) {

				const int plano_inicial = a_funcaoProducaoHidreletrica.getIterador2Inicial(AttMatrizFuncaoProducaoHidreletrica_RHS, periodo, int());
				const int plano_final = a_funcaoProducaoHidreletrica.getIterador2Final(AttMatrizFuncaoProducaoHidreletrica_RHS, periodo, int());

				if (plano_inicial != 1)
					throw std::invalid_argument("O plano inicial em " + getFullString(AttMatrizFuncaoProducaoHidreletrica_RHS) + " deve ser 1");

				if (a_funcaoProducaoHidreletrica.getIterador2Inicial(AttMatrizFuncaoProducaoHidreletrica_FC, periodo, int()) != plano_inicial)
					throw std::invalid_argument("O plano inicial em " + getFullString(AttMatrizFuncaoProducaoHidreletrica_FC) + " deve ser igual a " + getFullString(AttMatrizFuncaoProducaoHidreletrica_RHS));

				if (a_funcaoProducaoHidreletrica.getIterador2Inicial(AttMatrizFuncaoProducaoHidreletrica_VH, periodo, int()) != plano_inicial)
					throw std::invalid_argument("O plano inicial em " + getFullString(AttMatrizFuncaoProducaoHidreletrica_VH) + " deve ser igual a " + getFullString(AttMatrizFuncaoProducaoHidreletrica_RHS));

				if (a_funcaoProducaoHidreletrica.getIterador2Inicial(AttMatrizFuncaoProducaoHidreletrica_QH, periodo, int()) != plano_inicial)
					throw std::invalid_argument("O plano inicial em " + getFullString(AttMatrizFuncaoProducaoHidreletrica_QH) + " deve ser igual a " + getFullString(AttMatrizFuncaoProducaoHidreletrica_RHS));

				if (a_funcaoProducaoHidreletrica.getIterador2Inicial(AttMatrizFuncaoProducaoHidreletrica_SH, periodo, int()) != plano_inicial)
					throw std::invalid_argument("O plano inicial em " + getFullString(AttMatrizFuncaoProducaoHidreletrica_SH) + " deve ser igual a " + getFullString(AttMatrizFuncaoProducaoHidreletrica_RHS));

				if (a_funcaoProducaoHidreletrica.getIterador2Final(AttMatrizFuncaoProducaoHidreletrica_FC, periodo, int()) != plano_final)
					throw std::invalid_argument("O plano final em " + getFullString(AttMatrizFuncaoProducaoHidreletrica_FC) + " deve ser igual a " + getFullString(AttMatrizFuncaoProducaoHidreletrica_RHS));

				if (a_funcaoProducaoHidreletrica.getIterador2Final(AttMatrizFuncaoProducaoHidreletrica_VH, periodo, int()) != plano_final)
					throw std::invalid_argument("O plano final em " + getFullString(AttMatrizFuncaoProducaoHidreletrica_VH) + " deve ser igual a " + getFullString(AttMatrizFuncaoProducaoHidreletrica_RHS));

				if (a_funcaoProducaoHidreletrica.getIterador2Final(AttMatrizFuncaoProducaoHidreletrica_QH, periodo, int()) != plano_final)
					throw std::invalid_argument("O plano final em " + getFullString(AttMatrizFuncaoProducaoHidreletrica_QH) + " deve ser igual a " + getFullString(AttMatrizFuncaoProducaoHidreletrica_RHS));

				if (a_funcaoProducaoHidreletrica.getIterador2Final(AttMatrizFuncaoProducaoHidreletrica_SH, periodo, int()) != plano_final)
					throw std::invalid_argument("O plano final em " + getFullString(AttMatrizFuncaoProducaoHidreletrica_SH) + " deve ser igual a " + getFullString(AttMatrizFuncaoProducaoHidreletrica_RHS));

				if (a_funcaoProducaoHidreletrica.getSizeVetor(AttVetorFuncaoProducaoHidreletrica_RHS) > 0)
					throw std::invalid_argument("Os elementos de " + getFullString(AttVetorFuncaoProducaoHidreletrica_RHS) + " nao devem ser informados.");

				if (a_funcaoProducaoHidreletrica.getSizeVetor(AttVetorFuncaoProducaoHidreletrica_FC) > 0)
					throw std::invalid_argument("Os elementos de " + getFullString(AttVetorFuncaoProducaoHidreletrica_FC) + " nao devem ser informados.");

				if (a_funcaoProducaoHidreletrica.getSizeVetor(AttVetorFuncaoProducaoHidreletrica_VH) > 0)
					throw std::invalid_argument("Os elementos de " + getFullString(AttVetorFuncaoProducaoHidreletrica_VH) + " nao devem ser informados.");

				if (a_funcaoProducaoHidreletrica.getSizeVetor(AttVetorFuncaoProducaoHidreletrica_QH) > 0)
					throw std::invalid_argument("Os elementos de " + getFullString(AttVetorFuncaoProducaoHidreletrica_QH) + " nao devem ser informados.");

				if (a_funcaoProducaoHidreletrica.getSizeVetor(AttVetorFuncaoProducaoHidreletrica_SH) > 0)
					throw std::invalid_argument("Os elementos de " + getFullString(AttVetorFuncaoProducaoHidreletrica_SH) + " nao devem ser informados.");

				a_preencher_AttVetorFuncaoProducaoHidreletrica = SmartEnupla<AttVetorFuncaoProducaoHidreletrica, PreencherAtributo>(AttVetorFuncaoProducaoHidreletrica(1), std::vector<PreencherAtributo>(int(AttVetorFuncaoProducaoHidreletrica_Excedente - 1), nao_sem_utilizacao));
				a_preencher_AttMatrizFuncaoProducaoHidreletrica = SmartEnupla<AttMatrizFuncaoProducaoHidreletrica, PreencherAtributo>(AttMatrizFuncaoProducaoHidreletrica(1), std::vector<PreencherAtributo>(int(AttMatrizFuncaoProducaoHidreletrica_Excedente - 1), nao_operacional_informado));

			} // for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo)) {

			return true;

		} // if (sizeMatriz_RHS > 0) {

		else if (sizeMatriz_RHS == 0) {

			a_preencher_AttMatrizFuncaoProducaoHidreletrica = SmartEnupla<AttMatrizFuncaoProducaoHidreletrica, PreencherAtributo>(AttMatrizFuncaoProducaoHidreletrica(1), std::vector<PreencherAtributo>(int(AttMatrizFuncaoProducaoHidreletrica_Excedente - 1), sim_operacional));

			const int sizeVetor_RHS = a_funcaoProducaoHidreletrica.getSizeVetor(AttVetorFuncaoProducaoHidreletrica_RHS);

			if (sizeVetor_RHS > 0) {

				const int plano_inicial = a_funcaoProducaoHidreletrica.getIteradorInicial(AttVetorFuncaoProducaoHidreletrica_RHS, int());
				const int plano_final = a_funcaoProducaoHidreletrica.getIteradorFinal(AttVetorFuncaoProducaoHidreletrica_RHS, int());

				if (plano_inicial != 1)
					throw std::invalid_argument("O plano inicial em " + getFullString(AttVetorFuncaoProducaoHidreletrica_RHS) + " deve ser 1");

				if (a_funcaoProducaoHidreletrica.getIteradorInicial(AttVetorFuncaoProducaoHidreletrica_FC, int()) != plano_inicial)
					throw std::invalid_argument("O plano inicial em " + getFullString(AttVetorFuncaoProducaoHidreletrica_FC) + " deve ser igual a " + getFullString(AttVetorFuncaoProducaoHidreletrica_RHS));

				if (a_funcaoProducaoHidreletrica.getIteradorInicial(AttVetorFuncaoProducaoHidreletrica_VH, int()) != plano_inicial)
					throw std::invalid_argument("O plano inicial em " + getFullString(AttVetorFuncaoProducaoHidreletrica_VH) + " deve ser igual a " + getFullString(AttVetorFuncaoProducaoHidreletrica_RHS));

				if (a_funcaoProducaoHidreletrica.getIteradorInicial(AttVetorFuncaoProducaoHidreletrica_QH, int()) != plano_inicial)
					throw std::invalid_argument("O plano inicial em " + getFullString(AttVetorFuncaoProducaoHidreletrica_QH) + " deve ser igual a " + getFullString(AttVetorFuncaoProducaoHidreletrica_RHS));

				if (a_funcaoProducaoHidreletrica.getIteradorInicial(AttVetorFuncaoProducaoHidreletrica_SH, int()) != plano_inicial)
					throw std::invalid_argument("O plano inicial em " + getFullString(AttVetorFuncaoProducaoHidreletrica_SH) + " deve ser igual a " + getFullString(AttVetorFuncaoProducaoHidreletrica_RHS));

				if (a_funcaoProducaoHidreletrica.getIteradorFinal(AttVetorFuncaoProducaoHidreletrica_FC, int()) != plano_final)
					throw std::invalid_argument("O plano final em " + getFullString(AttVetorFuncaoProducaoHidreletrica_FC) + " deve ser igual a " + getFullString(AttVetorFuncaoProducaoHidreletrica_RHS));

				if (a_funcaoProducaoHidreletrica.getIteradorFinal(AttVetorFuncaoProducaoHidreletrica_VH, int()) != plano_final)
					throw std::invalid_argument("O plano final em " + getFullString(AttVetorFuncaoProducaoHidreletrica_VH) + " deve ser igual a " + getFullString(AttVetorFuncaoProducaoHidreletrica_RHS));

				if (a_funcaoProducaoHidreletrica.getIteradorFinal(AttVetorFuncaoProducaoHidreletrica_QH, int()) != plano_final)
					throw std::invalid_argument("O plano final em " + getFullString(AttVetorFuncaoProducaoHidreletrica_QH) + " deve ser igual a " + getFullString(AttVetorFuncaoProducaoHidreletrica_RHS));

				if (a_funcaoProducaoHidreletrica.getIteradorFinal(AttVetorFuncaoProducaoHidreletrica_SH, int()) != plano_final)
					throw std::invalid_argument("O plano final em " + getFullString(AttVetorFuncaoProducaoHidreletrica_SH) + " deve ser igual a " + getFullString(AttVetorFuncaoProducaoHidreletrica_RHS));

				a_preencher_AttVetorFuncaoProducaoHidreletrica = SmartEnupla<AttVetorFuncaoProducaoHidreletrica, PreencherAtributo>(AttVetorFuncaoProducaoHidreletrica(1), std::vector<PreencherAtributo>(int(AttVetorFuncaoProducaoHidreletrica_Excedente - 1), nao_premissa_informado));

				a_funcaoProducaoHidreletrica.setMatriz_forced(AttMatrizFuncaoProducaoHidreletrica_RHS, SmartEnupla<Periodo, SmartEnupla<int, double>>(a_horizonte_estudo, a_funcaoProducaoHidreletrica.getVetor(AttVetorFuncaoProducaoHidreletrica_RHS, int(), double())));
				a_funcaoProducaoHidreletrica.setMatriz_forced(AttMatrizFuncaoProducaoHidreletrica_FC, SmartEnupla<Periodo, SmartEnupla<int, double>>(a_horizonte_estudo, a_funcaoProducaoHidreletrica.getVetor(AttVetorFuncaoProducaoHidreletrica_FC, int(), double())));
				a_funcaoProducaoHidreletrica.setMatriz_forced(AttMatrizFuncaoProducaoHidreletrica_VH, SmartEnupla<Periodo, SmartEnupla<int, double>>(a_horizonte_estudo, a_funcaoProducaoHidreletrica.getVetor(AttVetorFuncaoProducaoHidreletrica_VH, int(), double())));
				a_funcaoProducaoHidreletrica.setMatriz_forced(AttMatrizFuncaoProducaoHidreletrica_QH, SmartEnupla<Periodo, SmartEnupla<int, double>>(a_horizonte_estudo, a_funcaoProducaoHidreletrica.getVetor(AttVetorFuncaoProducaoHidreletrica_QH, int(), double())));
				a_funcaoProducaoHidreletrica.setMatriz_forced(AttMatrizFuncaoProducaoHidreletrica_SH, SmartEnupla<Periodo, SmartEnupla<int, double>>(a_horizonte_estudo, a_funcaoProducaoHidreletrica.getVetor(AttVetorFuncaoProducaoHidreletrica_SH, int(), double())));

			} // if (sizeVetor_RHS > 0) {

			else if (sizeVetor_RHS == 0) {

				a_preencher_AttVetorFuncaoProducaoHidreletrica = SmartEnupla<AttVetorFuncaoProducaoHidreletrica, PreencherAtributo>(AttVetorFuncaoProducaoHidreletrica(1), std::vector<PreencherAtributo>(int(AttVetorFuncaoProducaoHidreletrica_Excedente - 1), nao_sem_utilizacao));

				SmartEnupla<Periodo, SmartEnupla<int, double>> coeficiente_QH(a_horizonte_estudo, SmartEnupla<int, double>(1, std::vector<double>(1, 0.0)));

				for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo)) {

					double ponderacao_potencia_maxima_usina = 0.0;

					IdConjuntoHidraulico menorIdConjuntoHidraulico = a_idConjuntoHidraulico;
					IdConjuntoHidraulico maiorIdConjuntoHidraulico = a_idConjuntoHidraulico;

					if (a_idConjuntoHidraulico == IdConjuntoHidraulico_Nenhum) {
						if (a_idUnidadeUHE != IdUnidadeUHE_Nenhum)
							throw std::invalid_argument("Ambos conjunto e unidade nao devem possuir id valido.");
						menorIdConjuntoHidraulico = IdConjuntoHidraulico_1;
						maiorIdConjuntoHidraulico = getMaiorId(a_idHidreletrica, IdConjuntoHidraulico());

						if (maiorIdConjuntoHidraulico == IdConjuntoHidraulico_Nenhum)
							throw std::invalid_argument("A " + getFullString(a_idHidreletrica) + " necessita ao menos de um conjunto hidraulico para o calculo da produtibilidade.");

						for (IdConjuntoHidraulico idConjuntoHidraulico = menorIdConjuntoHidraulico; idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {
							for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {
								double potencia_maxima_unidade = 0.0;
								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++)
									potencia_maxima_unidade += getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_maxima, periodo, idPatamarCarga, double()) * getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
								ponderacao_potencia_maxima_usina += potencia_maxima_unidade;
							} // for (IdUnidadeUHE idUnidadeUHE = IdUnidadeUHE_1; idUnidadeUHE <= getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()); idUnidadeUHE++) {
						} // for (IdConjuntoHidraulico idConjuntoHidraulico = menorIdConjuntoHidraulico; idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {
					} // if (a_idConjuntoHidraulico == IdConjuntoHidraulico_Nenhum) {

					for (IdConjuntoHidraulico idConjuntoHidraulico = menorIdConjuntoHidraulico; idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {

						double ponderacao_potencia_maxima_conjunto = 0.0;

						IdUnidadeUHE menorIdUnidadeUHE = a_idUnidadeUHE;
						IdUnidadeUHE maiorIdUnidadeUHE = a_idUnidadeUHE;

						if (a_idUnidadeUHE == IdUnidadeUHE_Nenhum) {
							menorIdUnidadeUHE = IdUnidadeUHE_1;
							maiorIdUnidadeUHE = getMaiorId(a_idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE());

							if (maiorIdUnidadeUHE == IdUnidadeUHE_Nenhum)
								throw std::invalid_argument("A " + getFullString(a_idHidreletrica) + " necessita ao menos de uma unidade hidreletrica em " + getFullString(idConjuntoHidraulico) + " para o calculo da produtibilidade.");

							if (a_idConjuntoHidraulico != IdConjuntoHidraulico_Nenhum) {
								for (IdUnidadeUHE idUnidadeUHE = menorIdUnidadeUHE; idUnidadeUHE <= maiorIdUnidadeUHE; idUnidadeUHE++) {
									double potencia_maxima_unidade = 0.0;
									for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++)
										potencia_maxima_unidade += getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_maxima, periodo, idPatamarCarga, double()) * getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
									ponderacao_potencia_maxima_conjunto += potencia_maxima_unidade;
								} // for (IdUnidadeUHE idUnidadeUHE = menorIdUnidadeUHE; idUnidadeUHE <= maiorIdUnidadeUHE; idUnidadeUHE++) {
							} // if (a_idConjuntoHidraulico != IdConjuntoHidraulico_Nenhum) {
						} // if (a_idUnidadeUHE == IdUnidadeUHE_Nenhum) {

						for (IdUnidadeUHE idUnidadeUHE = menorIdUnidadeUHE; idUnidadeUHE <= maiorIdUnidadeUHE; idUnidadeUHE++) {

							if (getSizeVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_cota_referencia) == 0) {
								a_preencher_AttVetorReservatorio.at(AttVetorReservatorio_cota_referencia) = sim_premissa;
								const double volume_minimo = getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo, periodo, double());
								const double volume_maximo = getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_maximo, periodo, double());
								vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).addElemento(AttVetorReservatorio_cota_referencia, periodo, vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getCotaMedia(periodo, volume_minimo, volume_maximo));
							} // if (getSizeVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_cota_referencia) == 0) {
							else if (getIteradorFinal(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_cota_referencia, Periodo()) < periodo) {
								a_preencher_AttVetorReservatorio.at(AttVetorReservatorio_cota_referencia) = sim_premissa;
								const double volume_minimo = getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_minimo, periodo, double());
								const double volume_maximo = getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_volume_maximo, periodo, double());
								vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).addElemento(AttVetorReservatorio_cota_referencia, periodo, vetorHidreletrica.att(a_idHidreletrica).vetorReservatorio.att(IdReservatorio_1).getCotaMedia(periodo, volume_minimo, volume_maximo));
							}

							if (getSizeVetor(a_idHidreletrica, AttVetorHidreletrica_canal_fuga_medio) == 0) {
								a_preencher_AttVetorHidreletrica.at(AttVetorHidreletrica_canal_fuga_medio) = sim_premissa;
								vetorHidreletrica.att(a_idHidreletrica).addElemento(AttVetorHidreletrica_canal_fuga_medio, periodo, getAtributo(a_idHidreletrica, AttComumHidreletrica_canal_fuga_medio, double()));
							} // if (getSizeVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_cota_referencia) == 0) {
							else if (getIteradorFinal(a_idHidreletrica, AttVetorHidreletrica_canal_fuga_medio, Periodo()) < periodo) {
								a_preencher_AttVetorHidreletrica.at(AttVetorHidreletrica_canal_fuga_medio) = sim_premissa;
								vetorHidreletrica.att(a_idHidreletrica).addElemento(AttVetorHidreletrica_canal_fuga_medio, periodo, getAtributo(a_idHidreletrica, AttComumHidreletrica_canal_fuga_medio, double()));
							}

							const TipoPerdaHidraulica tipo_perda_hidraulica = getAtributo(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttComumUnidadeUHE_tipo_de_perda_hidraulica, TipoPerdaHidraulica());
							const double perda_hidraulica = getAtributo(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttComumUnidadeUHE_perda_hidraulica, double());
							const double fator_de_producao = getAtributo(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttComumUnidadeUHE_fator_de_producao, double());

							double ponderacao_potencia_maxima_unidade = 0.0;

							if (a_idUnidadeUHE == IdUnidadeUHE_Nenhum) {
								for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga()); idPatamarCarga++)
									ponderacao_potencia_maxima_unidade += getElementoMatriz(a_idHidreletrica, idConjuntoHidraulico, idUnidadeUHE, AttMatrizUnidadeUHE_potencia_maxima, periodo, idPatamarCarga, double()) * getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
							} // if (a_idUnidadeUHE == IdUnidadeUHE_Nenhum) {

							const double canal_fuga_medio = getElementoVetor(a_idHidreletrica, AttVetorHidreletrica_canal_fuga_medio, periodo, double());

							const double cota_referencia = getElementoVetor(a_idHidreletrica, IdReservatorio_1, AttVetorReservatorio_cota_referencia, periodo, double());

							const double cota = cota_referencia - canal_fuga_medio;

							if (ponderacao_potencia_maxima_usina > 0.0)
								coeficiente_QH.at(periodo).at(1) += vetorHidreletrica.att(a_idHidreletrica).calcularProdutibilidade(tipo_perda_hidraulica, perda_hidraulica, fator_de_producao, cota) * (ponderacao_potencia_maxima_unidade / ponderacao_potencia_maxima_usina);
							else if (ponderacao_potencia_maxima_usina == 0.0) {
								if (ponderacao_potencia_maxima_conjunto > 0.0)
									coeficiente_QH.at(periodo).at(1) += vetorHidreletrica.att(a_idHidreletrica).calcularProdutibilidade(tipo_perda_hidraulica, perda_hidraulica, fator_de_producao, cota) * (ponderacao_potencia_maxima_unidade / ponderacao_potencia_maxima_conjunto);
								else if (ponderacao_potencia_maxima_conjunto == 0.0)
									coeficiente_QH.at(periodo).at(1) += vetorHidreletrica.att(a_idHidreletrica).calcularProdutibilidade(tipo_perda_hidraulica, perda_hidraulica, fator_de_producao, cota);
							} // else if (ponderacao_potencia_maxima_usina == 0.0) {

						} // for (IdUnidadeUHE idUnidadeUHE = menorIdUnidadeUHE; idUnidadeUHE <= maiorIdUnidadeUHE; idUnidadeUHE++) {
					} // for (IdConjuntoHidraulico idConjuntoHidraulico = menorIdConjuntoHidraulico; idConjuntoHidraulico <= maiorIdConjuntoHidraulico; idConjuntoHidraulico++) {

				} // for (Periodo periodo = a_horizonte_estudo.getIteradorInicial(); periodo <= a_horizonte_estudo.getIteradorFinal(); a_horizonte_estudo.incrementarIterador(periodo)) {

				a_funcaoProducaoHidreletrica.setMatriz_forced(AttMatrizFuncaoProducaoHidreletrica_QH, coeficiente_QH);
				a_funcaoProducaoHidreletrica.setMatriz_forced(AttMatrizFuncaoProducaoHidreletrica_RHS, SmartEnupla<Periodo, SmartEnupla<int, double>>(a_horizonte_estudo, SmartEnupla<int, double>(1, std::vector<double>(1, 0.0))));
				a_funcaoProducaoHidreletrica.setMatriz_forced(AttMatrizFuncaoProducaoHidreletrica_FC, SmartEnupla<Periodo, SmartEnupla<int, double>>(a_horizonte_estudo, SmartEnupla<int, double>(1, std::vector<double>(1, 1.0))));
				a_funcaoProducaoHidreletrica.setMatriz_forced(AttMatrizFuncaoProducaoHidreletrica_VH, SmartEnupla<Periodo, SmartEnupla<int, double>>(a_horizonte_estudo, SmartEnupla<int, double>(1, std::vector<double>(1, 0.0))));
				a_funcaoProducaoHidreletrica.setMatriz_forced(AttMatrizFuncaoProducaoHidreletrica_SH, SmartEnupla<Periodo, SmartEnupla<int, double>>(a_horizonte_estudo, SmartEnupla<int, double>(1, std::vector<double>(1, 0.0))));

			} // else if (sizeVetor_RHS == 0) {

			return true;

		} // else if (sizeMatriz_RHS == 0) {




		return false;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaFuncaoProducaoHidreletrica(a_funcaoProducaoHidreletrica," + getFullString(a_idHidreletrica) + "," + getFullString(a_idConjuntoHidraulico) + "," + getFullString(a_idUnidadeUHE) + "): \n" + std::string(erro.what())); }

} // bool Dados::validaFuncaoProducaoHidreletrica(const FuncaoProducaoHidreletrica& a_funcaoProducaoHidreletrica){


void Dados::validaPatamarCarga() {
	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_inicial = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final = horizonte_estudo.getIteradorFinal();

		for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {
			double duracaoTotalPatamar = 0.0;

			const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

			for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
				const double duracao_patamar = getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
				duracaoTotalPatamar += duracao_patamar;
			}

			if (!doubleCompara(duracaoTotalPatamar, 1.0))
				throw std::invalid_argument("A soma de " + getFullString(AttMatrizDados_percentual_duracao_patamar_carga) + " em " + getString(periodo) + " difere de 1.0 dada a precisao de " + getString(PRECISAO_COMPARACAO_DOUBLE) + ".");

		} // for (Periodo periodo = periodo_inicial; periodo <= periodo_final; horizonte_estudo.incrementarIterador(periodo)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaPatamarCarga(): \n" + std::string(erro.what())); }
} // void Dados::validaPatamarCarga(){


void Dados::validaContrato() {
	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const IdContrato maiorIdContrato = getMaiorId(IdContrato());

		for (IdContrato idContrato = IdContrato_1; idContrato <= maiorIdContrato; idContrato++) {

			if (getAtributo(idContrato, AttComumContrato_tipo_unidade, TipoUnidadeRestricaoContrato()) == TipoUnidadeRestricaoContrato_MW) {

				if (getSize1Matriz(idContrato, AttMatrizContrato_energia_minima) == 0)
					throw std::invalid_argument("Necessario informar a potencia minima para contrato do tipo  TipoUnidadeRestricaoContrato_MW em " + getFullString(idContrato));

				if (getSize1Matriz(idContrato, AttMatrizContrato_energia_maxima) == 0)
					throw std::invalid_argument("Necessario informar a potencia maxima para contrato do tipo  TipoUnidadeRestricaoContrato_MW em " + getFullString(idContrato));

				for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
					const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
					for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++)
						if (getElementoMatriz(idContrato, AttMatrizContrato_energia_minima, periodo, idPatamarCarga, double()) > getElementoMatriz(idContrato, AttMatrizContrato_energia_maxima, periodo, idPatamarCarga, double()))
							throw std::invalid_argument("Potencia Minima maior que Potencia Maxima em " + getString(periodo) + " em " + getFullString(idPatamarCarga) + " em " + getFullString(idContrato));
				} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
			}//if (getAtributo(idContrato, AttComumContrato_tipo_unidade, TipoUnidadeRestricaoContrato()) == TipoUnidadeRestricaoContrato_MW) {

			if (getAtributo(idContrato, AttComumContrato_tipo_unidade, TipoUnidadeRestricaoContrato()) == TipoUnidadeRestricaoContrato_percentual) {

				if (getSizeVetor(idContrato, AttVetorContrato_variacao_decrescimo_maxima) == 0)
					throw std::invalid_argument("Necessario informar a variacao maxima de decrescimo para contrato do tipo  TipoUnidadeRestricaoContrato_MW em " + getFullString(idContrato));

				if (getSizeVetor(idContrato, AttVetorContrato_variacao_acrescimo_maxima) == 0)
					throw std::invalid_argument("Necessario informar a variacao maxima de acrescimo para contrato do tipo  TipoUnidadeRestricaoContrato_MW em " + getFullString(idContrato));

			}//if (getAtributo(idContrato, AttComumContrato_tipo_unidade, TipoUnidadeRestricaoContrato()) == TipoUnidadeRestricaoContrato_MW) {


		}//for (IdContrato idContrato = IdContrato_1; idContrato <= maiorIdContrato; idContrato++){

	} // try{

	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaContrato(): \n" + std::string(erro.what())); }
} // void Dados::validaContrato(){


void Dados::validaRestricaoEletrica() {
	try {

		validaElementoSistemaRestricaoEletrica();

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_estudo_inicial = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();

		const IdRestricaoEletrica maiorIdRestricaoEletrica = getMaiorId(IdRestricaoEletrica());

		for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= maiorIdRestricaoEletrica; idRestricaoEletrica++) {

			const IdElementoSistema maiorIdElementoSistema = getMaiorId(idRestricaoEletrica, IdElementoSistema());

			if (maiorIdElementoSistema == IdElementoSistema_Nenhum)
				throw std::invalid_argument("Necessario incluir elementosSistema em " + getFullString(idRestricaoEletrica));

			if ((getSize1Matriz(idRestricaoEletrica, AttMatrizRestricaoEletrica_potencia_minima) > 0) && (getSize1Matriz(idRestricaoEletrica, AttMatrizRestricaoEletrica_potencia_maxima) > 0)) {

				for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

					const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
					for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						const double potencia_minima = getElementoMatriz(idRestricaoEletrica, AttMatrizRestricaoEletrica_potencia_minima, periodo, idPatamarCarga, double());
						const double potencia_maxima = getElementoMatriz(idRestricaoEletrica, AttMatrizRestricaoEletrica_potencia_maxima, periodo, idPatamarCarga, double());

						if (potencia_minima > potencia_maxima)
							throw std::invalid_argument("Potencia Minima maior que Potencia Maxima em " + getString(periodo) + " em " + getFullString(idPatamarCarga) + " em " + getFullString(idRestricaoEletrica));

					} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

				} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

			} // if ((getSize1Matriz(idRestricaoEletrica, AttMatrizRestricaoEletrica_potencia_minima) > 0) && (getSize1Matriz(idRestricaoEletrica, AttMatrizRestricaoEletrica_potencia_maxima) > 0)) {

		} // for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= maiorIdRestricaoEletrica; idRestricaoEletrica++) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaRestricaoEletrica(): \n" + std::string(erro.what())); }
} // void Dados::validaRestricaoEletrica(){


void Dados::validaElementoSistemaRestricaoEletrica() {

	try {

		const IdRestricaoEletrica maiorIdRestricaoEletrica = getMaiorId(IdRestricaoEletrica());

		for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= maiorIdRestricaoEletrica; idRestricaoEletrica++) {

			const IdElementoSistema maiorIdElementoSistema = getMaiorId(idRestricaoEletrica, IdElementoSistema());

			for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorIdElementoSistema; idElementoSistema++) {

				const TipoElementoSistema tipoElementoSistema = getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_tipo_elemento, TipoElementoSistema());

				bool elemento_identificado = false;

				//
				// Valida ConjuntoHidraulico e Hidreletrica
				//

				if (getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_conjuntohidraulico, IdConjuntoHidraulico()) != IdConjuntoHidraulico_Nenhum) {

					if (tipoElementoSistema != TipoElementoSistema_conjunto_hidraulico)
						throw std::invalid_argument("Valor " + getFullString(tipoElementoSistema) + " nao compativel com " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoEletrica));

					const IdHidreletrica       idHidreletrica = getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
					const IdConjuntoHidraulico idConjuntoHidraulico = getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_conjuntohidraulico, IdConjuntoHidraulico());

					if (idHidreletrica != IdHidreletrica_Nenhum) {
						if (idHidreletrica > getMaiorId(IdHidreletrica()))
							throw std::invalid_argument("Hidreletrica nao instanciada em " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoEletrica));
						else if (idConjuntoHidraulico > getMaiorId(idHidreletrica, IdConjuntoHidraulico()))
							throw std::invalid_argument("Conjunto Hidraulico nao instanciado em " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoEletrica));
						elemento_identificado = true;
					}
					else
						throw std::invalid_argument("Necessario informar a Hidreletrica associada ao ConjuntoHidraulico em " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoEletrica));

				}
				else if (getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica()) != IdHidreletrica_Nenhum) {
					if (tipoElementoSistema != TipoElementoSistema_hidreletrica)
						throw std::invalid_argument("Valor " + getFullString(tipoElementoSistema) + " nao compativel com " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoEletrica));

					elemento_identificado = true;
				}

				//
				// Valida UnidadeTermeletrica e Termeletrica
				//

				if ((elemento_identificado) && (getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_unidadeTermeletrica, IdUnidadeUTE()) != IdUnidadeUTE_Nenhum))
					throw std::invalid_argument("Multiplos atributos atribuidos para a instancia " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoEletrica));
				else if (getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_unidadeTermeletrica, IdUnidadeUTE()) != IdUnidadeUTE_Nenhum) {

					if (tipoElementoSistema != TipoElementoSistema_unidade_termeletrica)
						throw std::invalid_argument("Valor " + getFullString(tipoElementoSistema) + " nao compativel com " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoEletrica));

					if (getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_termeletrica, IdTermeletrica()) != IdTermeletrica_Nenhum)
						elemento_identificado = true;
					else
						throw std::invalid_argument("Necessario informar a Termeletrica associada a UnidadeTermeletrica em " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoEletrica));
				}
				else if ((elemento_identificado) && (getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_termeletrica, IdTermeletrica()) != IdTermeletrica_Nenhum))
					throw std::invalid_argument("Multiplos atributos atribuidos para a instancia " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoEletrica));
				else if (getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_termeletrica, IdTermeletrica()) != IdTermeletrica_Nenhum) {
					if (tipoElementoSistema != TipoElementoSistema_termeletrica)
						throw std::invalid_argument("Valor " + getFullString(tipoElementoSistema) + " nao compativel com " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoEletrica));

					elemento_identificado = true;
				}

				//
				// Valida Contrato
				//

				if ((elemento_identificado) && (getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_contrato, IdContrato()) != IdContrato_Nenhum))
					throw std::invalid_argument("Multiplos atributos atribuidos para a instancia " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoEletrica));
				else if (getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_contrato, IdContrato()) != IdContrato_Nenhum) {
					if (tipoElementoSistema != TipoElementoSistema_contrato)
						throw std::invalid_argument("Valor " + getFullString(tipoElementoSistema) + " nao compativel com " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoEletrica));

					elemento_identificado = true;
				}

				//
				// Valida Intercambio
				//

				if ((elemento_identificado) && (getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_intercambio, IdIntercambio()) != IdIntercambio_Nenhum))
					throw std::invalid_argument("Multiplos atributos atribuidos para a instancia " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoEletrica));
				else if (getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_intercambio, IdIntercambio()) != IdIntercambio_Nenhum) {
					if (tipoElementoSistema != TipoElementoSistema_intercambio)
						throw std::invalid_argument("Valor " + getFullString(tipoElementoSistema) + " nao compativel com " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoEletrica));

					elemento_identificado = true;
				}

				//
				// Valida UsinaEolica
				//

				if ((elemento_identificado) && (getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_eolica, IdUsinaEolica()) != IdUsinaEolica_Nenhum))
					throw std::invalid_argument("Multiplos atributos atribuidos para a instancia " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoEletrica));
				else if (getAtributo(idRestricaoEletrica, idElementoSistema, AttComumElementoSistema_eolica, IdUsinaEolica()) != IdUsinaEolica_Nenhum) {
					if (tipoElementoSistema != TipoElementoSistema_eolica)
						throw std::invalid_argument("Valor " + getFullString(tipoElementoSistema) + " nao compativel com " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoEletrica));

					elemento_identificado = true;
				}

			} // for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorIdElementoSistema; idElementoSistema++) {

		} // for (IdRestricaoEletrica idRestricaoEletrica = IdRestricaoEletrica_1; idRestricaoEletrica <= maiorIdRestricaoEletrica; idRestricaoEletrica++) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaRestricaoEletrica(): \n" + std::string(erro.what())); }

} // void Dados::validaElementoSistemaRestricaoEletrica(){


void Dados::validaAgrupamentoIntercambio() {
	try {

		const IdAgrupamentoIntercambio maiorIdAgrupamentoIntercambio = getMaiorId(IdAgrupamentoIntercambio());
		const IdIntercambio maiorIdIntercambio = getMaiorId(IdIntercambio());

		for (IdAgrupamentoIntercambio idAgrupamentoIntercambio = IdAgrupamentoIntercambio_1; idAgrupamentoIntercambio <= maiorIdAgrupamentoIntercambio; idAgrupamentoIntercambio++) {

			if (getSizeVetor(idAgrupamentoIntercambio, AttVetorAgrupamentoIntercambio_intercambio) > 0) {

				if (getIteradorInicial(idAgrupamentoIntercambio, AttVetorAgrupamentoIntercambio_intercambio, int()) != 1)
					throw std::invalid_argument("O " + getFullString(idAgrupamentoIntercambio) + " possui iterador inicial diferente de 1 em " + getFullString(AttVetorAgrupamentoIntercambio_intercambio) + ".");

				if (maiorIdIntercambio == IdIntercambio_Nenhum)
					throw std::invalid_argument("Necessaria a adicao dos dados de " + getString(Intercambio()) + " em Dados para a validacao de " + getFullString(AttVetorAgrupamentoIntercambio_intercambio) + ".");

				const int iteradorFinal_intercambio = getIteradorFinal(idAgrupamentoIntercambio, AttVetorAgrupamentoIntercambio_intercambio, int());
				const int iteradorFinal_fator_ponderacao = getIteradorFinal(idAgrupamentoIntercambio, AttVetorAgrupamentoIntercambio_fator_ponderacao, int());
				if (iteradorFinal_intercambio != iteradorFinal_fator_ponderacao)
					throw std::invalid_argument("O Numero de elementos de INTERCAMBIO e FATOR_PONDERACAO devem ser iguais no " + getFullString(idAgrupamentoIntercambio) + ".");

				for (int i = 1; i <= iteradorFinal_intercambio; i++) {
					const IdIntercambio idIntercambio = getElementoVetor(idAgrupamentoIntercambio, AttVetorAgrupamentoIntercambio_intercambio, i, IdIntercambio());
					if (idIntercambio > maiorIdIntercambio)
						throw std::invalid_argument("O " + getFullString(idIntercambio) + " do " + getFullString(idAgrupamentoIntercambio) + " nao consta em Dados.");
				} // for (int i = 1; i <= iteradorFinal_intercambio; i++) {

			} // if (getSizeVetor(idAgrupamentoIntercambio, AttVetorAgrupamentoIntercambio_intercambio) > 0) {

		} // for (IdAgrupamentoIntercambio idAgrupamentoIntercambio = IdAgrupamentoIntercambio_1; idAgrupamentoIntercambio <= maiorIdAgrupamentoIntercambio; idAgrupamentoIntercambio++) {
	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaAgrupamentoIntercambio(): \n" + std::string(erro.what())); }
} // void Dados::validaAgrupamentoIntercambio(){


void Dados::validaSubmercado() {
	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const IdSubmercado maiorIdSubmercado = getMaiorId(IdSubmercado());
		for (IdSubmercado idSubmercado = getMenorId(IdSubmercado()); idSubmercado <= maiorIdSubmercado; vetorSubmercado.incr(idSubmercado))
			validaSubmercado(idSubmercado);

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaSubmercado(): \n" + std::string(erro.what())); }
} // void Dados::valida_percentual_variacao_patamar_carga_Submercado() {


void Dados::validaSubmercado(const IdSubmercado a_idSubmercado) {

	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		if (getSize1Matriz(a_idSubmercado, AttMatrizSubmercado_percentual_variacao_patamar_carga) > 0) {

			for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				const IdPatamarCarga maiorIdPatamarCargaReferencia = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

				const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(a_idSubmercado, AttMatrizSubmercado_percentual_variacao_patamar_carga, periodo, IdPatamarCarga());

				if (maiorIdPatamarCargaReferencia != maiorIdPatamarCarga)
					throw std::invalid_argument("O maior PatamarCarga no Submercado " + getString(a_idSubmercado) + " deve ser igual ao cadastrado em Dados.");

				double percentualVariacaoPonderado = 0.0;

				for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
					const double percentual_duracao = getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
					const double percentual_variacao = getElementoMatriz(a_idSubmercado, AttMatrizSubmercado_percentual_variacao_patamar_carga, periodo, idPatamarCarga, double());
					percentualVariacaoPonderado += percentual_duracao * percentual_variacao;
				} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

				if ((!doubleCompara(0.02, percentualVariacaoPonderado, 1.0)) && (percentualVariacaoPonderado != 0.0))
					throw std::invalid_argument("A ponderacao da Duracao pela Variacao do patamar de carga em " + getString(periodo) + " no Submercado " + getString(a_idSubmercado) + " difere de 1.0 dada a precisao de 0.02.");

			} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

		} // if (getSize1Matriz(a_idSubmercado, AttMatrizSubmercado_percentual_variacao_patamar_carga) > 0){

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaSubmercado(" + getFullString(a_idSubmercado) + "): \n" + std::string(erro.what())); }

} // void Dados::validaSubmercado(const IdSubmercado a_idSubmercado){


void Dados::validaUsinaNaoSimuladaEmSubmercado() {

	try {

		const IdSubmercado maiorIdSubmercado = getMaiorId(IdSubmercado());
		for (IdSubmercado idSubmercado = getMenorId(IdSubmercado()); idSubmercado <= maiorIdSubmercado; vetorSubmercado.incr(idSubmercado))
			validaUsinaNaoSimuladaEmSubmercado(idSubmercado);

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaUsinaNaoSimuladaEmSubmercado(): \n" + std::string(erro.what())); }

} // void Dados::validaUsinaNaoSimuladaEmSubmercado(){


void Dados::validaUsinaNaoSimuladaEmSubmercado(const IdSubmercado a_idSubmercado) {

	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const IdUsinaNaoSimulada maiorIdUsinaNaoSimulada = getMaiorId(a_idSubmercado, IdUsinaNaoSimulada());

		for (IdUsinaNaoSimulada idUsinaNaoSimulada = IdUsinaNaoSimulada(1); idUsinaNaoSimulada <= maiorIdUsinaNaoSimulada; idUsinaNaoSimulada++) {

			if (vetorSubmercado.att(a_idSubmercado).vetorUsinaNaoSimulada.isInstanciado(idUsinaNaoSimulada)) {

				if (getSize1Matriz(a_idSubmercado, idUsinaNaoSimulada, AttMatrizUsinaNaoSimulada_percentual_variacao_patamar_carga) > 0) {

					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

						const IdPatamarCarga maiorIdPatamarCargaReferencia = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());

						const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(a_idSubmercado, idUsinaNaoSimulada, AttMatrizUsinaNaoSimulada_percentual_variacao_patamar_carga, periodo, IdPatamarCarga());

						if (maiorIdPatamarCargaReferencia != maiorIdPatamarCarga)
							throw std::invalid_argument("O maior PatamarCarga em " + getFullString(idUsinaNaoSimulada) + " em " + getFullString(a_idSubmercado) + " deve ser igual ao cadastrado em Dados.");

						double percentualVariacaoPonderado = 0.0;

						for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
							const double percentual_duracao = getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
							const double percentual_variacao = getElementoMatriz(a_idSubmercado, idUsinaNaoSimulada, AttMatrizUsinaNaoSimulada_percentual_variacao_patamar_carga, periodo, idPatamarCarga, double());
							percentualVariacaoPonderado += percentual_duracao * percentual_variacao;
						} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						if (!doubleCompara(0.02, percentualVariacaoPonderado, 1.0))
							throw std::invalid_argument("A ponderacao da Duracao pela Variacao do patamar de carga em " + getString(periodo) + " em " + getFullString(idUsinaNaoSimulada) + " em " + getFullString(a_idSubmercado) + " difere de 1.0 dada a precisao de 0.02.");

					} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				} // if (getSize1Matriz(a_idSubmercado, idUsinaNaoSimulada, AttMatrizUsinaNaoSimulada_percentual_variacao_patamar_carga) > 0) {

			} // if (vetorSubmercado.att(a_idSubmercado).vetorUsinaNaoSimulada.isInstanciado(idUsinaNaoSimulada)){

		} // for (IdUsinaNaoSimulada idUsinaNaoSimulada = IdUsinaNaoSimulada(1); idUsinaNaoSimulada <= maiorIdUsinaNaoSimulada; idUsinaNaoSimulada++) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaUsinaNaoSimuladaEmSubmercado(" + getFullString(a_idSubmercado) + "): \n" + std::string(erro.what())); }

} // void Dados::validaUsinaNaoSimuladaEmSubmercado(const IdSubmercado a_idSubmercado){


bool Dados::valida_historico_AfluenciaEmHidreletrica(const AttVetorAfluencia a_attVetorAfluencia) {

	try {

		if ((a_attVetorAfluencia != AttVetorAfluencia_incremental_historico) && (a_attVetorAfluencia != AttVetorAfluencia_natural_historico))
			throw std::invalid_argument("O metodo serve apenas para " + getFullString(AttVetorAfluencia_incremental_historico) + " e " + getFullString(AttVetorAfluencia_natural_historico));

		bool historico_afluencia_carregado = false;

		Periodo periodo_inicial_historico;
		Periodo periodo_final_historico;
		if (getSizeVetor(getMenorId(IdHidreletrica()), IdAfluencia_vazao_afluente, a_attVetorAfluencia) > 0) {
			historico_afluencia_carregado = true;
			periodo_inicial_historico = getIteradorInicial(getMenorId(IdHidreletrica()), IdAfluencia_vazao_afluente, a_attVetorAfluencia, Periodo());
			periodo_final_historico = getIteradorFinal(getMenorId(IdHidreletrica()), IdAfluencia_vazao_afluente, a_attVetorAfluencia, Periodo());
			if (periodo_inicial_historico.getTipoPeriodo() != periodo_final_historico.getTipoPeriodo())
				throw std::invalid_argument("Os periodos inicial e final do atributo " + getFullString(a_attVetorAfluencia) + " devem ser do mesmo tipo.");
		}

		const IdHidreletrica maiorIdHidreletrica = getMaiorId(IdHidreletrica());

		IdHidreletrica idHidreletrica_seguinte = getMenorId(IdHidreletrica());
		vetorHidreletrica.incr(idHidreletrica_seguinte);

		for (IdHidreletrica idHidreletrica = idHidreletrica_seguinte; idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

			// Verifica se historico foi carregado

			if ((getSizeVetor(idHidreletrica, IdAfluencia_vazao_afluente, a_attVetorAfluencia) > 0) && (historico_afluencia_carregado)) {

				if (getIteradorInicial(idHidreletrica, IdAfluencia_vazao_afluente, a_attVetorAfluencia, Periodo()) != periodo_inicial_historico)
					throw std::invalid_argument("O periodo inicial do atributo " + getFullString(a_attVetorAfluencia) + " de " + getFullString(idHidreletrica) + " deve ser " + getFullString(periodo_inicial_historico) + " para " + getFullString(IdAfluencia_vazao_afluente) + " ao inves de " + getFullString(getIteradorInicial(idHidreletrica, IdAfluencia_vazao_afluente, a_attVetorAfluencia, Periodo())));

				if (getIteradorFinal(idHidreletrica, IdAfluencia_vazao_afluente, a_attVetorAfluencia, Periodo()) != periodo_final_historico)
					throw std::invalid_argument("O periodo inicial do atributo " + getFullString(a_attVetorAfluencia) + " de " + getFullString(idHidreletrica) +" deve ser " + getFullString(periodo_final_historico) + " para " + getFullString(IdAfluencia_vazao_afluente) + " ao inves de " + getFullString(getIteradorFinal(idHidreletrica, IdAfluencia_vazao_afluente, a_attVetorAfluencia, Periodo())));
			}
			else if ((getSizeVetor(idHidreletrica, IdAfluencia_vazao_afluente, a_attVetorAfluencia) == 0) && (!historico_afluencia_carregado)) {}
			else
				throw std::invalid_argument("Os elementos do atributo " + getFullString(a_attVetorAfluencia) + ", quando informados, devem ser informados para " + getFullString(IdAfluencia_vazao_afluente) + " em todas as hidreletricas.");

		} // for (IdHidreletrica idHidreletrica = idHidreletrica_seguinte; idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

		if (!historico_afluencia_carregado)
			return false;

		const int ordem_maxima_historico = int(Periodo::getMaiorEstacao(periodo_inicial_historico.getTipoPeriodo())) - 1;

		if (getAtributo(AttComumDados_ordem_maxima_auto_correlacao_geracao_cenario_hidrologico, int()) > ordem_maxima_historico)
			throw std::invalid_argument("O valor do atributo " + getFullString(AttComumDados_ordem_maxima_auto_correlacao_geracao_cenario_hidrologico) + " nao pode ser maior que a ordem maxima: " + getString(ordem_maxima_historico));

		return true;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::valida_historico_AfluenciaEmHidreletrica(" + getFullString(a_attVetorAfluencia) + "): \n" + std::string(erro.what())); }

} // void Dados::valida_natural_historico_AfluenciaEmHidreletrica(const IdAfluencia a_idAfluencia){


void Dados::calcular_historico_afluencia_incremental_com_natural() {

	try {

		// Calcula historico de vazao afluente incremental com base na natural

		if (valida_historico_AfluenciaEmHidreletrica(AttVetorAfluencia_natural_historico)) {

			adicionaHidreletricasMontante();

			const IdHidreletrica maiorIdHidreletrica = getMaiorId(IdHidreletrica());
			const IdHidreletrica menorIdHidreletrica = getMenorId(IdHidreletrica());

			const SmartEnupla<Periodo, double> horizonte_historico = vetorHidreletrica.att(menorIdHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).getVetor(AttVetorAfluencia_natural_historico, Periodo(), double());

			const Periodo periodo_inicial_historico = getIteradorInicial(menorIdHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_natural_historico, Periodo());
			const Periodo periodo_final_historico = getIteradorFinal(menorIdHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_natural_historico, Periodo());

			for (Periodo periodo = periodo_inicial_historico; periodo <= periodo_final_historico; horizonte_historico.incrementarIterador(periodo)) {

				SmartEnupla<IdHidreletrica, double> afluencia(menorIdHidreletrica, std::vector<double>(int (maiorIdHidreletrica - menorIdHidreletrica) +1, 0.0));

				for (IdHidreletrica idHidreletrica = menorIdHidreletrica; idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica))
					afluencia.setElemento(idHidreletrica, getElementoVetor(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_natural_historico, periodo, double()));

				const SmartEnupla<IdHidreletrica, double> afluencia_incremental = calculaAfluenciaIncremental(afluencia);

				for (IdHidreletrica idHidreletrica = menorIdHidreletrica; idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica))
					vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).addElemento(AttVetorAfluencia_incremental_historico, periodo, afluencia_incremental.at(idHidreletrica));

			} // for (Periodo periodo = periodo_inicial_historico; periodo <= periodo_final_historico; horizonte_estudo.incrementarIterador(periodo)) {

		} // if (valida_historico_AfluenciaEmHidreletrica(AttVetorAfluencia_natural_historico)) {
		else
			throw std::invalid_argument("Atributo " + getFullString(AttVetorAfluencia_natural_historico) + " nao validado.");

		valida_historico_AfluenciaEmHidreletrica(AttVetorAfluencia_incremental_historico);

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::calcular_historico_afluencia_incremental_com_natural(): \n" + std::string(erro.what())); }

} // void Dados::calcular_historico_afluencia_incremental_com_natural(){


bool Dados::valida_tendencia_AfluenciaEmHidreletrica(const AttVetorAfluencia a_attVetorAfluencia) {

	try {

		if ((a_attVetorAfluencia != AttVetorAfluencia_incremental_tendencia) && (a_attVetorAfluencia != AttVetorAfluencia_natural_tendencia))
			throw std::invalid_argument("O metodo serve apenas para " + getFullString(AttVetorAfluencia_incremental_tendencia) + " e " + getFullString(AttVetorAfluencia_natural_tendencia));

		bool tendencia_afluencia_carregado = false;

		Periodo periodo_inicial_tendencia;
		Periodo periodo_final_tendencia;

		bool alguma_hidreletrica_sem_tendencia = false;

		//
		// Valida de Todas hidrel�tricas possuem atributos com os mesmos iteradores.
		//

		const IdHidreletrica maiorIdHidreletrica = getMaiorId(IdHidreletrica());
		for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

			if (getSizeVetor(idHidreletrica, IdAfluencia_vazao_afluente, a_attVetorAfluencia) > 0) {

				if (!tendencia_afluencia_carregado) {
					periodo_inicial_tendencia = getIteradorInicial(idHidreletrica, IdAfluencia_vazao_afluente, a_attVetorAfluencia, Periodo());
					periodo_final_tendencia = getIteradorFinal(idHidreletrica, IdAfluencia_vazao_afluente, a_attVetorAfluencia, Periodo());

					tendencia_afluencia_carregado = true;
				} // if (!tendencia_afluencia_carregado) {

			} // if (getSize1Matriz(idHidreletrica, IdAfluencia_vazao_afluente, a_attVetorAfluencia) > 0) {

			else
				alguma_hidreletrica_sem_tendencia = true;

		} // for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

		// Calcula afluencia incremental caso a mesma n�o tenha sido carregada e haja afluencia natural.

		if (!tendencia_afluencia_carregado)
			return false;

		if (alguma_hidreletrica_sem_tendencia)
			adicionarTendenciaHidrologicaHistorica();

		const IdEstagio estagio_inicial = getAtributo(AttComumDados_estagio_inicial, IdEstagio());

		const Periodo periodo_otimizacao_estagio_inicial = getElementoVetor(AttVetorDados_horizonte_otimizacao, estagio_inicial, Periodo());

		const Periodo periodo_tendencia_seguinte = Periodo(periodo_otimizacao_estagio_inicial.getTipoPeriodo(), periodo_final_tendencia + 1);


		if (periodo_otimizacao_estagio_inicial != periodo_tendencia_seguinte)
			throw std::invalid_argument("Periodo final da tendencia hidrologica " + getString(periodo_final_tendencia) + " nao acoplavel com periodo de otimizacao do estagio: " + getFullString(estagio_inicial) + " " + getString(periodo_otimizacao_estagio_inicial));

		return true;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::valida_tendencia_AfluenciaEmHidreletrica(" + getFullString(a_attVetorAfluencia) + "): \n" + std::string(erro.what())); }

} // void Dados::valida_tendencia_AfluenciaEmHidreletrica(const IdAfluencia a_idAfluencia) {


void Dados::calcular_tendencia_afluencia_incremental_com_natural() {

	try {

		if (valida_tendencia_AfluenciaEmHidreletrica(AttVetorAfluencia_natural_tendencia)) {

			const IdHidreletrica maiorIdHidreletrica = getMaiorId(IdHidreletrica());
			const IdHidreletrica menorIdHidreletrica = getMenorId(IdHidreletrica());

			SmartEnupla<Periodo, double> horizonte_tendencia;

			for (IdHidreletrica idHidreletrica = menorIdHidreletrica; idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {
				if (getSizeVetor(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_natural_tendencia) > 0) {
					horizonte_tendencia = vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).getVetor(AttVetorAfluencia_natural_tendencia, Periodo(), double());
					break;
				}
				else
					throw std::invalid_argument("Atributo " + getFullString(AttVetorAfluencia_natural_tendencia) + " nao informado em " + getFullString(idHidreletrica) + " .");

			}

			const Periodo   periodo_inicial_tendencia = getIteradorInicial(menorIdHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_natural_tendencia, Periodo());
			const Periodo   periodo_final_tendencia = getIteradorFinal(menorIdHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_natural_tendencia, Periodo());

			for (Periodo periodo = periodo_inicial_tendencia; periodo <= periodo_final_tendencia; horizonte_tendencia.incrementarIterador(periodo)) {

				SmartEnupla<IdHidreletrica, double> afluencia_natural(menorIdHidreletrica, std::vector<double>(int(maiorIdHidreletrica - menorIdHidreletrica) + 1, 0.0));

				for (IdHidreletrica idHidreletrica = menorIdHidreletrica; idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica))
					afluencia_natural.setElemento(idHidreletrica, getElementoVetor(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_natural_tendencia, periodo, double()));

				const SmartEnupla<IdHidreletrica, double> afluencia_incremental = calculaAfluenciaIncremental(afluencia_natural);

				for (IdHidreletrica idHidreletrica = menorIdHidreletrica; idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica))
					vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).addElemento(AttVetorAfluencia_incremental_tendencia, periodo, afluencia_incremental.at(idHidreletrica));

			} // for (Periodo periodo = periodo_inicial_tendencia; periodo <= periodo_final_tendencia; horizonte_estudo.incrementarIterador(periodo)) {

		} //  if (valida_tendencia_AfluenciaEmHidreletrica(AttMatrizAfluencia_natural)){
		else
			throw std::invalid_argument("Atributo " + getFullString(AttVetorAfluencia_natural_tendencia) + " nao validado.");

		valida_tendencia_AfluenciaEmHidreletrica(AttVetorAfluencia_incremental_tendencia);

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::calcular_tendencia_afluencia_incremental_com_natural(): \n" + std::string(erro.what())); }

} // void Dados::calcular_tendencia_afluencia_incremental_com_natural(){


void Dados::adicionarTendenciaHidrologicaNula() {

	try {

		const IdHidreletrica maioridHidreletrica = getMaiorId(IdHidreletrica());

		IdHidreletrica idHidreletrica_tendencia_encontrada = IdHidreletrica_Nenhum;

		SmartEnupla<Periodo, double> tendencia_hidrologica;

		for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maioridHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

			if (getSizeVetor(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_incremental_tendencia) == 0) {

				if (idHidreletrica_tendencia_encontrada == IdHidreletrica_Nenhum) {

					for (IdHidreletrica idUHE = getMenorId(IdHidreletrica()); idUHE <= maioridHidreletrica; vetorHidreletrica.incr(idUHE)) {

						if (getSizeVetor(idUHE, IdAfluencia_vazao_afluente, AttVetorAfluencia_incremental_tendencia) > 0) {

							idHidreletrica_tendencia_encontrada = idUHE;

							tendencia_hidrologica = vetorHidreletrica.att(idUHE).vetorAfluencia.att(IdAfluencia_vazao_afluente).getVetor(AttVetorAfluencia_incremental_tendencia, Periodo(), double());

							break;

						} // if (getSize1Matriz(idUHE, IdAfluencia_vazao_afluente, AttMatrizAfluencia_incremental_tendencia) > 0) {

					} // for (IdHidreletrica idUHE = getMenorId(IdHidreletrica()); idUHE <= maioridHidreletrica; idUHE++) {

					if (idHidreletrica_tendencia_encontrada == IdHidreletrica_Nenhum)
						throw std::invalid_argument("Nenhuma hidreletrica possui informacao em " + getFullString(AttVetorAfluencia_incremental_tendencia));

				} // if (idHidreletrica_tendencia_encontrada == IdHidreletrica_Nenhum) {

				for (Periodo periodo = tendencia_hidrologica.getIteradorInicial(); periodo <= tendencia_hidrologica.getIteradorFinal(); tendencia_hidrologica.incrementarIterador(periodo))
					tendencia_hidrologica.setElemento(periodo, 0.0);

				vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).setVetor_forced(AttVetorAfluencia_incremental_tendencia, tendencia_hidrologica);

			}// if (getSize1Matriz(idHidreletrica, IdAfluencia_vazao_afluente, AttMatrizAfluencia_incremental_tendencia) == 0) {

		} // for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maioridHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {


	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::adicionarTendenciaHidrologicaNula(): \n" + std::string(erro.what())); }

} // void Dados::adicionarTendenciaHidrologicaNula() {


void Dados::adicionarTendenciaHidrologicaHistorica() {

	try {

		//
		// Calcula tendencia em caso de tipo_tendencia_hidrologica ser TipoTendenciaEstocastica_historico
		//

		if (getAtributo(AttComumDados_tipo_tendencia_hidrologica, TipoTendenciaEstocastica()) == TipoTendenciaEstocastica_historico) {

			const IdEstagio estagio_inicial_horizonte_otimizacao = getIteradorInicial(AttVetorDados_horizonte_otimizacao, IdEstagio());

			const Periodo periodoInicialHorizonte = getElementoVetor(AttVetorDados_horizonte_otimizacao, estagio_inicial_horizonte_otimizacao, Periodo());

			const Periodo periodo_tendencia_hidrologica_historica = getAtributo(AttComumDados_periodo_tendencia_hidrologica_historica, Periodo());

			const Periodo periodo_final_historico_afluencia_incremental = getIteradorFinal(getMenorId(IdHidreletrica()), IdAfluencia_vazao_afluente, AttVetorAfluencia_incremental_historico, Periodo());

			if (periodo_tendencia_hidrologica_historica.getTipoPeriodo() != periodoInicialHorizonte.getTipoPeriodo())
				throw std::invalid_argument("O TipoPeriodo de " + getFullString(AttComumDados_periodo_tendencia_hidrologica_historica) + " devem ser o mesmo do inicio do horizonte: " + getString(periodoInicialHorizonte.getTipoPeriodo()) + " .");

			else if (periodo_tendencia_hidrologica_historica.getTipoPeriodo() != periodo_final_historico_afluencia_incremental.getTipoPeriodo())
				throw std::invalid_argument("O TipoPeriodo de " + getFullString(AttComumDados_periodo_tendencia_hidrologica_historica) + " devem ser o mesmo do historico: " + getString(periodo_final_historico_afluencia_incremental.getTipoPeriodo()) + " .");

			else if (periodo_tendencia_hidrologica_historica > periodo_final_historico_afluencia_incremental)
				throw std::invalid_argument("Periodo historico de afluencia inicial maior que o periodo final do historico.");

			int lag_inicial = 0;
			if (getElementoVetor(AttVetorDados_numero_aberturas, estagio_inicial_horizonte_otimizacao, int()) > 1) {
				lag_inicial = 1;
				if (periodo_tendencia_hidrologica_historica.getEstacao() != (periodoInicialHorizonte - 1).getEstacao())
					throw std::invalid_argument("A estacao do periodo de tendencia hidrologica historica deve igual a estacao do periodo " + getString(periodoInicialHorizonte - 1));
			}
			else if (getElementoVetor(AttVetorDados_numero_aberturas, estagio_inicial_horizonte_otimizacao, int()) == 1) {
				if (periodo_tendencia_hidrologica_historica.getEstacao() != periodoInicialHorizonte.getEstacao())
					throw std::invalid_argument("A estacao do periodo de tendencia hidrologica historica deve igual a estacao do periodo " + getString(periodoInicialHorizonte));
			}

			const int numero_estacoes = Periodo::getMaiorEstacao(periodoInicialHorizonte.getTipoPeriodo());

			for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= getMaiorId(IdHidreletrica()); vetorHidreletrica.incr(idHidreletrica)) {
				SmartEnupla<Periodo, double> incremental_tendencia;
				for (int p = numero_estacoes; p >= 0; p--) {
					const double incremental_historico = getElementoVetor(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_incremental_historico, periodo_tendencia_hidrologica_historica - p, double());
					incremental_tendencia.addElemento(periodoInicialHorizonte - lag_inicial - p, incremental_historico);
				} // for (int p = numero_estacoes; p >= lag_inicial; p--) {
				vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).setVetor(AttVetorAfluencia_incremental_tendencia, incremental_tendencia);
			} // for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= getMaiorId(IdHidreletrica()); vetorHidreletrica.incr(idHidreletrica)) {

		} // if (getAtributo(AttComumDados_tipo_tendencia_hidrologica, TipoTendenciaEstocastica()) == TipoTendenciaEstocastica_historico) {


		else if (getAtributo(AttComumDados_tipo_tendencia_hidrologica, TipoTendenciaEstocastica()) == TipoTendenciaEstocastica_serie_informada) {

			const IdHidreletrica maioridHidreletrica = getMaiorId(IdHidreletrica());

			IdHidreletrica idHidreletrica_tendencia_encontrada = IdHidreletrica_Nenhum;

			SmartEnupla<Periodo, double> tendencia_hidrologica;

			for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maioridHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

				if (getSizeVetor(idHidreletrica, IdAfluencia_vazao_afluente, AttVetorAfluencia_natural_tendencia) == 0) {

					if (idHidreletrica_tendencia_encontrada == IdHidreletrica_Nenhum) {

						for (IdHidreletrica idUHE = getMenorId(IdHidreletrica()); idUHE <= maioridHidreletrica; vetorHidreletrica.incr(idUHE)) {

							if (getSizeVetor(idUHE, IdAfluencia_vazao_afluente, AttVetorAfluencia_natural_tendencia) > 0) {

								idHidreletrica_tendencia_encontrada = idUHE;

								tendencia_hidrologica = vetorHidreletrica.att(idUHE).vetorAfluencia.att(IdAfluencia_vazao_afluente).getVetor(AttVetorAfluencia_natural_tendencia, Periodo(), double());

								break;

							} // if (getSize1Matriz(idUHE, IdAfluencia_vazao_afluente, AttMatrizAfluencia_natural_tendencia) > 0) {

						} // for (IdHidreletrica idUHE = getMenorId(IdHidreletrica()); idUHE <= maioridHidreletrica; idUHE++) {

						if (idHidreletrica_tendencia_encontrada == IdHidreletrica_Nenhum)
							throw std::invalid_argument("Nenhuma hidreletrica possui informacao em " + getFullString(AttVetorAfluencia_natural_tendencia));

					} // if (idHidreletrica_tendencia_encontrada == IdHidreletrica_Nenhum) {

					const SmartEnupla<Periodo, double> historico_hidrologico = vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).getVetor(AttVetorAfluencia_natural_historico, Periodo(), double());

					const Periodo maior_periodo_historico = historico_hidrologico.getIteradorFinal();

					for (Periodo periodo = tendencia_hidrologica.getIteradorInicial(); periodo <= tendencia_hidrologica.getIteradorFinal(); tendencia_hidrologica.incrementarIterador(periodo))
						tendencia_hidrologica.setElemento(periodo, 0.0);

					for (Periodo periodo_tendencia = tendencia_hidrologica.getIteradorInicial(); periodo_tendencia <= tendencia_hidrologica.getIteradorFinal(); tendencia_hidrologica.incrementarIterador(periodo_tendencia)) {

						SmartEnupla<Periodo, double> periodos_composicao;

						for (Periodo periodo_historico = historico_hidrologico.getIteradorFinal() - Periodo::getMaiorEstacao(maior_periodo_historico.getTipoPeriodo()); periodo_historico <= tendencia_hidrologica.getIteradorFinal(); periodo_historico++) {

							const double sobreposicao = periodo_tendencia.sobreposicao(periodo_historico);

							if (sobreposicao > 0.0)
								periodos_composicao.addElemento(periodo_historico, sobreposicao);

						} // for (Periodo periodo_historico = historico_hidrologico.getIteradorFinal() - Periodo::getMaiorEstacao(maior_periodo_historico.getTipoPeriodo()); periodo_historico <= tendencia_hidrologica.getIteradorFinal(); periodo_historico++) {

						for (Periodo periodo = periodos_composicao.getIteradorInicial(); periodo <= periodos_composicao.getIteradorFinal(); periodo++) {

							if (periodo <= maior_periodo_historico)
								tendencia_hidrologica.at(periodo_tendencia) += periodos_composicao.at(periodo) * historico_hidrologico.at(periodo);
							else
								tendencia_hidrologica.at(periodo_tendencia) += periodos_composicao.at(periodo) * getMedia(historico_hidrologico.getElementos(periodo.getEstacao()).at(periodo.getTipoPeriodo()));

						} // for (Periodo periodo = periodos_composicao.getIteradorInicial(); periodo <= periodos_composicao.getIteradorFinal(); periodo++) {

					} // for (Periodo periodo_tendencia = tendencia_hidrologica.getIteradorInicial(); periodo_tendencia <= tendencia_hidrologica.getIteradorFinal(); tendencia_hidrologica.incrementarIterador(periodo_tendencia)) {

					vetorHidreletrica.att(idHidreletrica).vetorAfluencia.att(IdAfluencia_vazao_afluente).setVetor_forced(AttVetorAfluencia_natural_tendencia, tendencia_hidrologica);
				}// if (getSize1Matriz(idHidreletrica, IdAfluencia_vazao_afluente, AttMatrizAfluencia_natural_tendencia) == 0) {

			} // for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maioridHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

		} // else if (getAtributo(AttComumDados_tipo_tendencia_hidrologica, TipoTendenciaEstocastica()) == TipoTendenciaEstocastica_serie_informada) {

		else
			throw std::invalid_argument("O atributo " + getFullString(AttComumDados_tipo_tendencia_hidrologica) + " deve ser " + getFullString(TipoTendenciaEstocastica_historico) + " ou " + getFullString(TipoTendenciaEstocastica_serie_informada));

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::adicionarTendenciaHidrologicaHistorica(): \n" + std::string(erro.what())); }

} // void Dados::adicionarTendenciaHidrologicaHistorica() {


void Dados::validacao_mapeamento_cenarios_aberturas(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar, bool& a_mapeamento_cenarios_e_aberturas_carregado) {

	try {



		const IdProcesso idProcesso = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());

		a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);

		if (idProcesso == IdProcesso_mestre) {
			if ((!a_mapeamento_cenarios_e_aberturas_carregado) && (!a_imprimir_atributos_sem_recarregar)) {
				a_entradaSaidaDados.setAppendArquivo(false);
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("PROCESSO_ITERACAO_AttMatrizOperacional_PorCenarioPorEstagio.csv", IdIteracao_Nenhum, IdProcesso_Nenhum, arranjoResolucao, AttMatrizProcesso_cenario_estado_por_cenario);
				a_entradaSaidaDados.setAppendArquivo(true);
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("PROCESSO_ITERACAO_AttMatrizOperacional_PorCenarioPorEstagio.csv", IdIteracao_Nenhum, IdProcesso_Nenhum, arranjoResolucao, AttMatrizProcesso_menor_abertura_por_cenario_estado);
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("PROCESSO_ITERACAO_AttMatrizOperacional_PorCenarioPorEstagio.csv", IdIteracao_Nenhum, IdProcesso_Nenhum, arranjoResolucao, AttMatrizProcesso_maior_abertura_por_cenario_estado);
			} // if ((!mapeamento_aberturas_carregado) && (!a_imprimir_atributos_sem_recarregar)) {
		}

	}
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validacao_mapeamento_cenarios_aberturas(a_entradaSaidaDados, " + a_diretorio_att_operacional + "," + a_diretorio_att_premissa + "," + getString(a_imprimir_atributos_sem_recarregar) + "): \n" + std::string(erro.what())); }
} // void Dados::validacao_mapeamento_cenarios_aberturas(EntradaSaidaDados a_entrada_saida_dados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_att_operacionais_sem_recarregar){



void Dados::definirCenariosPorProcessosEmArranjoResolucao(){

	try {

		const IdProcesso maior_processo = arranjoResolucao.getMaiorId(IdProcesso());
		const int numero_processos = int(maior_processo);

		const int numero_cenarios = getAtributo(AttComumDados_numero_cenarios, int());

		arranjoResolucao.setAtributo(AttComumArranjoResolucao_maior_cenario, IdCenario(numero_cenarios));

		const int numero_cenarios_balanceados_por_processo = numero_cenarios / numero_processos;
		const int numero_cenarios_desbalanceados = numero_cenarios - (numero_cenarios_balanceados_por_processo * numero_processos);
		const IdProcesso maior_processo_desbalanceado = IdProcesso(numero_cenarios_desbalanceados);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Filosofia: Os cenarios desbalanceados sao distribuidos, um para cada processo, comecando do processo mestre.


		for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= maior_processo; idProcesso++) {

			arranjoResolucao.vetorProcesso.att(idProcesso).setAtributo(AttComumProcesso_menor_cenario, IdCenario_Nenhum);
			arranjoResolucao.vetorProcesso.att(idProcesso).setAtributo(AttComumProcesso_maior_cenario, IdCenario_Nenhum);

			if (idProcesso <= maior_processo_desbalanceado) {//Processos desbalanceados

				arranjoResolucao.vetorProcesso.att(idProcesso).setAtributo(AttComumProcesso_menor_cenario, IdCenario(numero_cenarios_balanceados_por_processo * int(idProcesso - 1) + int(idProcesso - 1) + 1)); //+ int(idProcesso - 1) sao os cenarios desbalanceados dos Processos anteriores			
				arranjoResolucao.vetorProcesso.att(idProcesso).setAtributo(AttComumProcesso_maior_cenario, IdCenario(numero_cenarios_balanceados_por_processo * int(idProcesso) + int(idProcesso))); //+int(idProcesso) eh o cenario desbalanceado

			}//if (idProcesso <= maior_processo_desbalanceado) {

			else if ((idProcesso > maior_processo_desbalanceado) && (numero_cenarios_balanceados_por_processo > 0)) {////Processos balanceados

				arranjoResolucao.vetorProcesso.att(idProcesso).setAtributo(AttComumProcesso_menor_cenario, IdCenario(numero_cenarios_balanceados_por_processo * int(idProcesso - 1) + 1 + numero_cenarios_desbalanceados));
				arranjoResolucao.vetorProcesso.att(idProcesso).setAtributo(AttComumProcesso_maior_cenario, IdCenario(numero_cenarios_balanceados_por_processo * int(idProcesso) + numero_cenarios_desbalanceados));

			}// // else if ((idProcesso > maior_processo_desbalanceado) && (numero_cenarios_por_processo_otimizacao > 0)) {

		} // for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= maior_processo; idProcesso++) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::iniciarArranjoResolucao(): \n" + std::string(erro.what())); }

} // void Dados::iniciarArranjoResolucao() {



void Dados::mapearCenariosAberturasPorIteracaoEmArranjoResolucao() {

	try {

		//
		// Definicao iteracoes do estudo
		//

		const TipoEstudo tipo_estudo = getAtributo(AttComumDados_tipo_estudo, TipoEstudo());
		const bool visitar_todos_cenarios_por_iteracao = getAtributo(AttComumDados_visitar_todos_cenarios_por_iteracao, bool());

		const int numero_processos = int(arranjoResolucao.getMaiorId(IdProcesso()));

		int numero_maximo_iteracoes = getAtributo(AttComumDados_numero_maximo_iteracoes, int());
		IdIteracao iteracao_inicial = getAtributo(AttComumDados_iteracao_inicial, IdIteracao());
		IdIteracao iteracao_numero_maximo = IdIteracao(numero_maximo_iteracoes + 1);
		IdIteracao iteracao_final = iteracao_numero_maximo;

		int numero_iteracoes_com_divisao_cenarios = numero_maximo_iteracoes;

		if (tipo_estudo == TipoEstudo_otimizacao) {
			if (iteracao_inicial > iteracao_final)
				throw std::invalid_argument("Iteracao inicial " + getFullString(iteracao_inicial) + "  maior que final " + getFullString(iteracao_final) + ".");
			if ((numero_maximo_iteracoes == 0) && (iteracao_inicial > IdIteracao_0))
				throw std::invalid_argument("Iteracao inicial deve ser " + getFullString(IdIteracao_0) + "  para " + getFullString(AttComumDados_numero_maximo_iteracoes) + " igual a 0.");
		}
		else if (tipo_estudo == TipoEstudo_otimizacao_e_simulacao) {
			iteracao_final++;
			if (iteracao_inicial > iteracao_final)
				throw std::invalid_argument("Iteracao inicial " + getFullString(iteracao_inicial) + "  maior que final " + getFullString(iteracao_final) + ".");
			if ((numero_maximo_iteracoes == 0) && (iteracao_inicial > IdIteracao_0))
				throw std::invalid_argument("Iteracao inicial deve ser " + getFullString(IdIteracao_0) + "  para " + getFullString(AttComumDados_numero_maximo_iteracoes) + " igual a 0.");
			else if ((numero_maximo_iteracoes == 0) && (iteracao_inicial == IdIteracao_0))
				iteracao_final = iteracao_inicial;
		}
		else if (tipo_estudo == TipoEstudo_simulacao) {
			iteracao_inicial = IdIteracao_0;
			iteracao_numero_maximo = IdIteracao_0;
			iteracao_final = IdIteracao_0;
			numero_maximo_iteracoes = 0;
		}

		if ((numero_maximo_iteracoes > 0) && (visitar_todos_cenarios_por_iteracao))
			numero_iteracoes_com_divisao_cenarios = 0;

		arranjoResolucao.setAtributo(AttComumArranjoResolucao_iteracao_inicial, iteracao_inicial);
		arranjoResolucao.setAtributo(AttComumArranjoResolucao_iteracao_numero_maximo, iteracao_numero_maximo);
		arranjoResolucao.setAtributo(AttComumArranjoResolucao_iteracao_final, iteracao_final);


		//
		// Computar menor e maior cenarios por iteracao
		//

		for (IdIteracao idIteracao = iteracao_inicial; idIteracao <= iteracao_final; idIteracao++) {

			Iteracao iteracao;
			iteracao.setAtributo(AttComumIteracao_idIteracao, idIteracao);

			// Simulação Inicial, Final e iteracoes com todos cenarios
			if ((idIteracao == IdIteracao_0) || (iteracao_numero_maximo < idIteracao) || (numero_iteracoes_com_divisao_cenarios == 0)) {

				iteracao.setAtributo(AttComumIteracao_menor_cenario, IdCenario_1);
				iteracao.setAtributo(AttComumIteracao_maior_cenario, arranjoResolucao.getAtributo(AttComumArranjoResolucao_maior_cenario, IdCenario()));

			} // if ((idIteracao == IdIteracao_0) || (iteracao_numero_maximo < idIteracao) || (numero_iteracoes_com_divisao_cenarios == 0)) {

			else {

				const int numero_cenarios = int(arranjoResolucao.getAtributo(AttComumArranjoResolucao_maior_cenario, IdCenario()) - IdCenario_1) + 1;

				const int numero_cenarios_balanceados_por_iteracao = numero_cenarios / numero_iteracoes_com_divisao_cenarios;
				const int numero_cenarios_desbalanceados = numero_cenarios - (numero_cenarios_balanceados_por_iteracao * numero_iteracoes_com_divisao_cenarios);
				const IdIteracao maior_iteracao_desbalanceada = IdIteracao(numero_cenarios_desbalanceados + 1); // + 1 por conta da Iteracao_0

				if (idIteracao <= maior_iteracao_desbalanceada) {//idIteracao desbalanceados

					iteracao.setAtributo(AttComumIteracao_menor_cenario, IdCenario(numero_cenarios_balanceados_por_iteracao * int(idIteracao - 2) + int(idIteracao - 2) + 1)); //+ int(idIteracao - 2) sao os cenarios desbalanceados dos idIteracao anteriores			
					iteracao.setAtributo(AttComumIteracao_maior_cenario, IdCenario(numero_cenarios_balanceados_por_iteracao * int(idIteracao - 1) + int(idIteracao - 1))); //+int(idIteracao - 1) eh o cenario desbalanceado

				}//if (idIteracao <= maior_iteracao_desbalanceada) {

				else if ((idIteracao > maior_iteracao_desbalanceada) && (numero_cenarios_balanceados_por_iteracao > 0)) {////idIteracao balanceados

					iteracao.setAtributo(AttComumIteracao_menor_cenario, IdCenario(numero_cenarios_balanceados_por_iteracao * int(idIteracao - 2) + 1 + numero_cenarios_desbalanceados));
					iteracao.setAtributo(AttComumIteracao_maior_cenario, IdCenario(numero_cenarios_balanceados_por_iteracao * int(idIteracao - 1) + numero_cenarios_desbalanceados));

				}// // else if ((idIteracao > maior_iteracao_desbalanceada) && (numero_cenarios_balanceados_por_iteracao > 0)) {

				else if ((idIteracao > maior_iteracao_desbalanceada) && (numero_cenarios_balanceados_por_iteracao == 0))
					throw std::invalid_argument("Numero de cenarios insuficiente para o numero de iteracoes.");

			} // else {

			arranjoResolucao.vetorIteracao.add(iteracao);

			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++) {

				Processo processo;
				processo.setAtributo(AttComumProcesso_idProcesso, idProcesso);
				processo.setAtributo(AttComumProcesso_menor_cenario, IdCenario_Nenhum);
				processo.setAtributo(AttComumProcesso_maior_cenario, IdCenario_Nenhum);


				const IdCenario menor_cenario_iteracao = arranjoResolucao.getAtributo(idIteracao, AttComumIteracao_menor_cenario, IdCenario());
				const IdCenario maior_cenario_iteracao = arranjoResolucao.getAtributo(idIteracao, AttComumIteracao_maior_cenario, IdCenario());

				const int numero_cenarios_da_iteracao = int(maior_cenario_iteracao - menor_cenario_iteracao) + 1;

				const int numero_cenarios_balanceados_da_iteracao_por_processo = numero_cenarios_da_iteracao / numero_processos;
				const int numero_cenarios_desbalanceados_da_iteracao = numero_cenarios_da_iteracao - (numero_cenarios_balanceados_da_iteracao_por_processo * numero_processos);
				const IdProcesso maior_processo_desbalanceado = IdProcesso(numero_cenarios_desbalanceados_da_iteracao); // + 1 por conta da Iteracao_0

				if (idProcesso <= maior_processo_desbalanceado) {//idIteracao desbalanceados

					processo.setAtributo(AttComumProcesso_menor_cenario, IdCenario(int(menor_cenario_iteracao - 1) + numero_cenarios_balanceados_da_iteracao_por_processo * int(idProcesso - 1) + int(idProcesso - 1) + 1)); //+ int(idProcesso - 2) sao os cenarios desbalanceados dos idProcessos anteriores			
					processo.setAtributo(AttComumProcesso_maior_cenario, IdCenario(int(menor_cenario_iteracao - 1) + numero_cenarios_balanceados_da_iteracao_por_processo * int(idProcesso) + int(idProcesso))); //+int(idProcesso - 1) eh o cenario desbalanceado

				}//if (idProcesso <= maior_processo_desbalanceado) {

				else if ((idProcesso > maior_processo_desbalanceado) && (numero_cenarios_balanceados_da_iteracao_por_processo > 0)) {////idIteracao balanceados

					processo.setAtributo(AttComumProcesso_menor_cenario, IdCenario(int(menor_cenario_iteracao - 1) + numero_cenarios_balanceados_da_iteracao_por_processo * int(idProcesso - 1) + 1 + numero_cenarios_desbalanceados_da_iteracao));
					processo.setAtributo(AttComumProcesso_maior_cenario, IdCenario(int(menor_cenario_iteracao - 1) + numero_cenarios_balanceados_da_iteracao_por_processo * int(idProcesso) + numero_cenarios_desbalanceados_da_iteracao));

				}// // else if ((idProcesso > maior_processo_desbalanceado) && (numero_cenarios_balanceados_da_iteracao_por_processo > 0)) {

				arranjoResolucao.vetorIteracao.att(idIteracao).vetorProcesso.add(processo);

			} // for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++) {

		} // for (IdIteracao idIteracao = iteracao_inicial; idIteracao <= iteracao_final; idIteracao++) {

		//
		// Mapeamento cenarios por iteracao
		//

		const TipoProcessamentoParalelo tipo_processamento = getAtributo(AttComumDados_tipo_processamento_paralelo, TipoProcessamentoParalelo());

		const IdEstagio estagio_inicial = getAtributo(AttComumDados_estagio_inicial, IdEstagio());
		const IdEstagio estagio_final = getAtributo(AttComumDados_estagio_final, IdEstagio());

		SmartEnupla<IdEstagio, int> agrupar_aberturas(estagio_inicial, std::vector<int>(int(estagio_final - estagio_inicial) + 1, 0));

		SmartEnupla<IdCenario, SmartEnupla<Periodo, IdRealizacao>> mapeamento_espaco_amostral = processoEstocastico_hidrologico.getMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, IdCenario(), Periodo(), IdRealizacao());
		SmartEnupla<IdEstagio, Periodo> periodos(estagio_inicial, std::vector<Periodo>(int(estagio_final - estagio_inicial) + 1, Periodo()));
		for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

			const int cortes_multiplos = getElementoVetor(AttVetorDados_cortes_multiplos, idEstagio, int());

			if ((tipo_processamento == TipoProcessamentoParalelo_por_abertura) && (cortes_multiplos != 1))
				agrupar_aberturas.at(idEstagio) = 1;

			periodos.at(idEstagio) = mapeamento_espaco_amostral.at(mapeamento_espaco_amostral.getIteradorInicial()).getIteradores(getElementoVetor(AttVetorDados_horizonte_otimizacao, idEstagio, Periodo())).at(0);

		} // for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

		const IdProcesso idProcesso_local = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());

		arranjoResolucao.setVetor_forced(AttVetorArranjoResolucao_agrupar_aberturas, agrupar_aberturas);

		SmartEnupla<IdEstagio, IdCenario> enupla_inicial(estagio_inicial, std::vector<IdCenario>(int(estagio_final - estagio_inicial) + 1, IdCenario_Nenhum));

		for (IdIteracao idIteracao = iteracao_inicial; idIteracao <= iteracao_final; idIteracao++) {

			const IdCenario menor_cenario_iteracao_local = arranjoResolucao.getAtributo(idIteracao, idProcesso_local, AttComumProcesso_menor_cenario, IdCenario());
			const IdCenario maior_cenario_iteracao_local = arranjoResolucao.getAtributo(idIteracao, idProcesso_local, AttComumProcesso_maior_cenario, IdCenario());

			std::vector<int> valores;

			if (menor_cenario_iteracao_local != IdCenario_Nenhum) {

				if (((idIteracao > iteracao_inicial) && (numero_iteracoes_com_divisao_cenarios == 0)) || ((iteracao_numero_maximo < idIteracao) && (iteracao_inicial == IdIteracao_0))) {}
				else {

					const int tamanho = (int(estagio_final - estagio_inicial) + 1) * (int(maior_cenario_iteracao_local - menor_cenario_iteracao_local) + 1);

					valores = std::vector<int>(tamanho, 0);

					int i = 0;
					for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

						const int cortes_multiplos = getElementoVetor(AttVetorDados_cortes_multiplos, idEstagio, int());

						for (IdCenario idCenario_iteracao = menor_cenario_iteracao_local; idCenario_iteracao <= maior_cenario_iteracao_local; idCenario_iteracao++) {

							IdCenario idCenario_mesmo_passado = IdCenario_Excedente;
							IdCenario idCenario_mesma_trajetoria = IdCenario_Excedente;

							if (idEstagio == estagio_inicial) {
								idCenario_mesmo_passado = arranjoResolucao.getAtributo(idIteracao, AttComumIteracao_menor_cenario, IdCenario());
								idCenario_mesma_trajetoria = idCenario_mesmo_passado;
							}

							else {
								for (IdProcesso idProcesso_aux = IdProcesso_mestre; idProcesso_aux <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso_aux++) {
									if (arranjoResolucao.getAtributo(idIteracao, idProcesso_aux, AttComumProcesso_menor_cenario, IdCenario()) != IdCenario_Nenhum) {
										for (IdCenario idCenario = arranjoResolucao.getAtributo(idIteracao, idProcesso_aux, AttComumProcesso_menor_cenario, IdCenario()); idCenario <= arranjoResolucao.getAtributo(idIteracao, idProcesso_aux, AttComumProcesso_maior_cenario, IdCenario()); idCenario++) {
											for (IdEstagio idEstagio_past = estagio_inicial; idEstagio_past <= idEstagio; idEstagio_past++) {
												if ((IdAbertura(processoEstocastico_hidrologico.getElementoMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, idCenario_iteracao, periodos.at(idEstagio_past), IdRealizacao())) !=
													IdAbertura(processoEstocastico_hidrologico.getElementoMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, idCenario, periodos.at(idEstagio_past), IdRealizacao()))))
													break;
												else if ((idEstagio_past == IdEstagio(idEstagio - 1)) && (idCenario < idCenario_mesmo_passado))
													idCenario_mesmo_passado = idCenario;
												else if ((idEstagio_past == idEstagio) && (idCenario < idCenario_mesma_trajetoria))
													idCenario_mesma_trajetoria = idCenario;
											}
										} // for (IdCenario idCenario = arranjoResolucao.getAtributo(idIteracao, idProcesso_aux, AttComumProcesso_menor_cenario, IdCenario()); idCenario <= arranjoResolucao.getAtributo(idIteracao, idProcesso_aux, AttComumProcesso_maior_cenario, IdCenario()); idCenario++) {
									} // if (arranjoResolucao.getAtributo(idIteracao, idProcesso_aux, AttComumProcesso_menor_cenario, IdCenario()) != IdCenario_Nenhum) {
								} // for (IdProcesso idProcesso_aux = IdProcesso_mestre; idProcesso_aux <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso_aux++) {
							}

							// Resolver cenario: priorizado resolver sempre o cenario de menor id daqueles que são iguais
							if (idCenario_iteracao <= idCenario_mesma_trajetoria) {

								IdCenario idCenario_estado = idCenario_iteracao;
								if (idCenario_mesmo_passado < idCenario_estado)
									idCenario_estado = idCenario_mesmo_passado;

								valores.at(i) = int(idCenario_estado);

							} // if (idCenario_iteracao <= idCenario_mesma_trajetoria) {
							i++;
						} // for (IdCenario idCenario_iteracao = menor_cenario_iteracao; idCenario_iteracao <= menor_cenario_iteracao; idCenario_iteracao++) {
					} // for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

				} // Condicoes para mapeamento de cenarios

			} // if (menor_cenario_iteracao != IdCenario_Nenhum)

			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++) {

				const IdCenario menor_cenario_iteracao = arranjoResolucao.getAtributo(idIteracao, idProcesso, AttComumProcesso_menor_cenario, IdCenario());
				const IdCenario maior_cenario_iteracao = arranjoResolucao.getAtributo(idIteracao, idProcesso, AttComumProcesso_maior_cenario, IdCenario());

				if (menor_cenario_iteracao != IdCenario_Nenhum) {

					if (((idIteracao > iteracao_inicial) && (numero_iteracoes_com_divisao_cenarios == 0)) || ((iteracao_numero_maximo < idIteracao) && (iteracao_inicial == IdIteracao_0)))
						arranjoResolucao.vetorIteracao.att(idIteracao).vetorProcesso.att(idProcesso).setMatriz_forced(AttMatrizProcesso_cenario_estado_por_cenario, arranjoResolucao.vetorIteracao.att(iteracao_inicial).vetorProcesso.att(idProcesso).getMatriz(AttMatrizProcesso_cenario_estado_por_cenario, IdCenario(), IdEstagio(), IdCenario()));

					else{

						arranjoResolucao.vetorIteracao.att(idIteracao).vetorProcesso.att(idProcesso).setMatriz_forced(AttMatrizProcesso_cenario_estado_por_cenario, SmartEnupla<IdCenario, SmartEnupla<IdEstagio, IdCenario>>(menor_cenario_iteracao, std::vector<SmartEnupla<IdEstagio, IdCenario>>(int(maior_cenario_iteracao - menor_cenario_iteracao) + 1, enupla_inicial)));

						const int tamanho = (int(estagio_final - estagio_inicial) + 1) * (int(maior_cenario_iteracao - menor_cenario_iteracao) + 1);

						std::vector<int> valores_recv(tamanho, 0);

						if (idProcesso != idProcesso_local)
							MPI_Recv(&valores_recv[0], tamanho, MPI_INT, getRank(idProcesso), getRank(idProcesso_local), MPI_COMM_WORLD, MPI_STATUS_IGNORE);
						else {
							for (IdProcesso idProcesso_aux = IdProcesso_mestre; idProcesso_aux <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso_aux++) {
								if (idProcesso_aux != idProcesso_local)
									MPI_Send(&valores[0], int(valores.size()), MPI_INT, getRank(idProcesso_aux), getRank(idProcesso_aux), MPI_COMM_WORLD);
							}
							valores_recv = valores;
						}

						int i = 0;
						for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {
							for (IdCenario idCenario_iteracao = menor_cenario_iteracao; idCenario_iteracao <= maior_cenario_iteracao; idCenario_iteracao++) {
								arranjoResolucao.vetorIteracao.att(idIteracao).vetorProcesso.att(idProcesso).setElemento(AttMatrizProcesso_cenario_estado_por_cenario, idCenario_iteracao, idEstagio, IdCenario(valores_recv.at(i)));
								i++;
							}
						}

					} // Condicoes para mapeamento de cenarios

				} // if (menor_cenario_iteracao != IdCenario_Nenhum)

			} // for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= maior_processo; idProcesso++) {

		} // for (IdIteracao idIteracao = iteracao_inicial; idIteracao <= iteracao_final; idIteracao++) {


		//
		// Mapeamento aberturas por iteracao
		//

		SmartEnupla<IdEstagio, IdAbertura> enupla_inicial_abertura(estagio_inicial, std::vector<IdAbertura>(int(estagio_final - estagio_inicial) + 1, IdAbertura_Nenhum));

		for (IdIteracao idIteracao = iteracao_inicial; idIteracao <= iteracao_final; idIteracao++) {

			if ((IdIteracao_0 < idIteracao) && (idIteracao <= iteracao_numero_maximo)) {

				for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++) {

					if ((idIteracao > IdIteracao_1) && (numero_iteracoes_com_divisao_cenarios == 0)) {
						for (IdProcesso idProcesso_aux = IdProcesso_mestre; idProcesso_aux <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso_aux++) {
							arranjoResolucao.vetorIteracao.att(idIteracao).vetorProcesso.att(idProcesso_aux).setMatriz_forced(AttMatrizProcesso_menor_abertura_por_cenario_estado, arranjoResolucao.vetorIteracao.att(IdIteracao_1).vetorProcesso.att(idProcesso_aux).getMatriz(AttMatrizProcesso_menor_abertura_por_cenario_estado, IdCenario(), IdEstagio(), IdAbertura()));
							arranjoResolucao.vetorIteracao.att(idIteracao).vetorProcesso.att(idProcesso_aux).setMatriz_forced(AttMatrizProcesso_maior_abertura_por_cenario_estado, arranjoResolucao.vetorIteracao.att(IdIteracao_1).vetorProcesso.att(idProcesso_aux).getMatriz(AttMatrizProcesso_maior_abertura_por_cenario_estado, IdCenario(), IdEstagio(), IdAbertura()));
						}
					} // if ((idIteracao > IdIteracao_1) && (numero_iteracoes_com_divisao_cenarios == 0)) {

					else {

						SmartEnupla<IdCenario, SmartEnupla<IdEstagio, IdAbertura>> map_menor_abertura;
						SmartEnupla<IdCenario, SmartEnupla<IdEstagio, IdAbertura>> map_maior_abertura;

						// O mapeamento das aberturas engloba cenarios_estados de outros processos quando o processamento é realizado por abertura, exceto se cortes_multiplos == 1
						for (IdProcesso idProcesso_aux = IdProcesso_mestre; idProcesso_aux <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso_aux++) {

							for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

								const int cortes_multiplos = getElementoVetor(AttVetorDados_cortes_multiplos, idEstagio, int());

								if (((tipo_processamento == TipoProcessamentoParalelo_por_abertura) && (cortes_multiplos != 1)) || (idProcesso_aux == idProcesso)) {

									std::vector<IdCenario> lista_cenario_estado = arranjoResolucao.getIdsCenarioEstadoFromCenarios(idProcesso_aux, idIteracao, idEstagio);

									if (lista_cenario_estado.size() > 0) {

										for (int c = 0; c < int(lista_cenario_estado.size()); c++) {

											const IdCenario idCenario_estado = lista_cenario_estado.at(c);

											if (map_menor_abertura.size() == 0) {
												map_menor_abertura.addElemento(idCenario_estado, enupla_inicial_abertura);
												map_maior_abertura.addElemento(idCenario_estado, enupla_inicial_abertura);
											}

											else if ((idCenario_estado < map_menor_abertura.getIteradorInicial())) {
												for (IdCenario idCenario = IdCenario(map_menor_abertura.getIteradorInicial() - 1); idCenario >= idCenario_estado; idCenario--) {
													map_menor_abertura.addElemento(idCenario, enupla_inicial_abertura);
													map_maior_abertura.addElemento(idCenario, enupla_inicial_abertura);
												}
											}
											else if ((map_menor_abertura.getIteradorFinal() < idCenario_estado)) {
												for (IdCenario idCenario = IdCenario(map_menor_abertura.getIteradorFinal() + 1); idCenario <= idCenario_estado; idCenario++) {
													map_menor_abertura.addElemento(idCenario, enupla_inicial_abertura);
													map_maior_abertura.addElemento(idCenario, enupla_inicial_abertura);
												}
											}

											// Em caso de cortes_multiplos == 1, preenche-se com a abertura do mapeamento do espaço amostral.
											if (cortes_multiplos == 1) {
												const IdAbertura idAbertura = IdAbertura(processoEstocastico_hidrologico.getElementoMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, idCenario_estado, periodos.at(idEstagio), IdRealizacao()));

												map_menor_abertura.at(idCenario_estado).at(idEstagio) = idAbertura;
												map_maior_abertura.at(idCenario_estado).at(idEstagio) = idAbertura;
											} // if (cortes_multiplos == 1) {

											else if (tipo_processamento == TipoProcessamentoParalelo_por_cenario) {
												map_menor_abertura.at(idCenario_estado).at(idEstagio) = IdAbertura_1;
												map_maior_abertura.at(idCenario_estado).at(idEstagio) = IdAbertura(getElementoVetor(AttVetorDados_numero_aberturas, idEstagio, int()));
											}
											else {
												map_menor_abertura.at(idCenario_estado).at(idEstagio) = IdAbertura_Excedente;
												map_maior_abertura.at(idCenario_estado).at(idEstagio) = IdAbertura_Excedente;
											}

										} // for (int c = 0; c < int(lista_cenario_estado.size()); c++) {
									} // if (lista_cenario_estado.size() > 0){
								} // if (((tipo_processamento == TipoProcessamentoParalelo_por_abertura) && (cortes_multiplos != 1)) || (idProcesso_aux == idProcesso)) {
							} // if (arranjoResolucao.getSize1Matriz(idProcesso, idIteracao, AttMatrizIteracao_menor_cenario) > 0) {
						} // for (IdProcesso idProcesso_aux = IdProcesso_mestre; idProcesso_aux <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso_aux++) {


						for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

							const int cortes_multiplos = getElementoVetor(AttVetorDados_cortes_multiplos, idEstagio, int());

							if ((tipo_processamento == TipoProcessamentoParalelo_por_abertura) && (cortes_multiplos != 1)) {

								const int numero_aberturas = getElementoVetor(AttVetorDados_numero_aberturas, idEstagio, int());

								const int numero_processos = int(arranjoResolucao.getMaiorId(IdProcesso()));

								const int numero_aberturas_balanceados_por_processo = numero_aberturas / numero_processos;
								const int numero_aberturas_desbalanceados = numero_aberturas - (numero_aberturas_balanceados_por_processo * numero_processos);
								const IdProcesso maior_processo_desbalanceado = IdProcesso(numero_aberturas_desbalanceados);

								///////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//Filosofia: Os cenarios desbalanceados sao distribuidos, um para cada processo, comecando do processo mestre.

								IdAbertura menor_abertura = IdAbertura_Nenhum;
								IdAbertura maior_abertura = IdAbertura_Nenhum;

								if (idProcesso <= maior_processo_desbalanceado) {//Processos desbalanceados

									menor_abertura = IdAbertura(numero_aberturas_balanceados_por_processo * int(idProcesso - 1) + int(idProcesso - 1) + 1); //+ int(idProcesso - 1) sao as aberturas desbalanceados dos Processos anteriores			
									maior_abertura = IdAbertura(numero_aberturas_balanceados_por_processo * int(idProcesso) + int(idProcesso)); //+int(idProcesso) eh a abertura desbalanceado

								}//if (idProcesso <= maior_processo_desbalanceado) {

								else if ((idProcesso > maior_processo_desbalanceado) && (numero_aberturas_balanceados_por_processo > 0)) {////Processos balanceados

									menor_abertura = IdAbertura(numero_aberturas_balanceados_por_processo * int(idProcesso - 1) + 1 + numero_aberturas_desbalanceados);
									maior_abertura = IdAbertura(numero_aberturas_balanceados_por_processo * int(idProcesso) + numero_aberturas_desbalanceados);

								}// // else if ((idProcesso > maior_processo_desbalanceado) && (numero_cenarios_por_processo_otimizacao > 0)) {

								for (IdCenario idCenario_estado = map_menor_abertura.getIteradorInicial(); idCenario_estado <= map_menor_abertura.getIteradorFinal(); idCenario_estado++) {

									if (map_menor_abertura.at(idCenario_estado).at(idEstagio) == IdAbertura_Excedente) {
										map_menor_abertura.at(idCenario_estado).at(idEstagio) = menor_abertura;
										map_maior_abertura.at(idCenario_estado).at(idEstagio) = maior_abertura;
									}
									//else if (map_menor_abertura.at(idCenario_estado).at(idEstagio) != IdAbertura_Nenhum)
										//throw std::invalid_argument("Mapeamento Aberturas Invalido para corte multiplo unitário");

								} // for (IdCenario idCenario_estado = map_menor_abertura.getIteradorInicial(); idCenario_estado <= map_menor_abertura.getIteradorFinal(); idCenario_estado++) {

							} // if ((tipo_processamento == TipoProcessamentoParalelo_por_abertura) && (cortes_multiplos != 1)) {

						} // for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

						arranjoResolucao.vetorIteracao.att(idIteracao).vetorProcesso.att(idProcesso).setMatriz_forced(AttMatrizProcesso_menor_abertura_por_cenario_estado, map_menor_abertura);
						arranjoResolucao.vetorIteracao.att(idIteracao).vetorProcesso.att(idProcesso).setMatriz_forced(AttMatrizProcesso_maior_abertura_por_cenario_estado, map_maior_abertura);

					} // else {

				} // for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++) {
			} // if ((IdIteracao_0 < idIteracao) && (idIteracao <= iteracao_numero_maximo)) {
		} // for (IdIteracao idIteracao = iteracao_inicial; idIteracao <= iteracao_final; idIteracao++) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::mapearCenariosAberturasPorIteracaoEmArranjoResolucao(): \n" + std::string(erro.what())); }

}

void Dados::adicionaHidreletricasMontante() {

	try {

		const IdHidreletrica maiorIdHidreletrica = getMaiorId(IdHidreletrica());

		for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

			if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_montante) == 0) {

				for (IdHidreletrica idUHE_montante = getMenorId(IdHidreletrica()); idUHE_montante <= maiorIdHidreletrica; vetorHidreletrica.incr(idUHE_montante)) {

					if (getAtributo(idUHE_montante, AttComumHidreletrica_jusante, IdHidreletrica()) == idHidreletrica)
						vetorHidreletrica.att(idHidreletrica).addElemento(AttVetorHidreletrica_montante, getSizeVetor(idHidreletrica, AttVetorHidreletrica_montante) + 1, idUHE_montante);

				} // for (IdHidreletrica idUHE_montante = getMenorId(IdHidreletrica()); idUHE_montante <= maiorIdHidreletrica; idUHE++) {

			}

		} // for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::adicionaHidreletricasMontante(): \n" + std::string(erro.what())); }

} // void Dados::adicionaHidreletricasMontante(const IdHidreletrica a_idHidreletrica){


void Dados::adicionaHidreletricasMontanteDesvio() {

	try {

		const IdHidreletrica maiorIdHidreletrica = getMaiorId(IdHidreletrica());

		for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

			if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_montante_desvio) == 0) {

				for (IdHidreletrica idUHE_montante_desvio = getMenorId(IdHidreletrica()); idUHE_montante_desvio <= maiorIdHidreletrica; vetorHidreletrica.incr(idUHE_montante_desvio)) {

					if (getAtributo(idUHE_montante_desvio, AttComumHidreletrica_jusante_desvio, IdHidreletrica()) == idHidreletrica)
						vetorHidreletrica.att(idHidreletrica).addElemento(AttVetorHidreletrica_montante_desvio, getSizeVetor(idHidreletrica, AttVetorHidreletrica_montante_desvio) + 1, idUHE_montante_desvio);

				} // for (IdHidreletrica idUHE_montante_desvio = getMenorId(IdHidreletrica()); idUHE_montante_desvio <= maiorIdHidreletrica; idUHE_montante_desvio++) {

			}

		} // for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::adicionaHidreletricasMontanteDesvio(): \n" + std::string(erro.what())); }

} // void Dados::adicionaHidreletricasMontanteDesvio(const IdHidreletrica a_idHidreletrica){

SmartEnupla<IdHidreletrica, double> Dados::calculaAfluenciaIncremental(const SmartEnupla<IdHidreletrica, double>& a_afluencia_natural) {

	try {

		const IdHidreletrica menorIdUHE = a_afluencia_natural.getIteradorInicial();
		const IdHidreletrica maiorIdUHE = a_afluencia_natural.getIteradorFinal();

		SmartEnupla<IdHidreletrica, double> afluencia_incremental_retorno(menorIdUHE, std::vector<double>(int(maiorIdUHE - menorIdUHE) + 1, NAN));

		for (IdHidreletrica idUHE = menorIdUHE; idUHE <= maiorIdUHE; vetorHidreletrica.incr(idUHE)) {

			// Inicializa Aflu�ncia Incremental com a Pr�pria Aflu�ncia Natural.
			double afluencia_incremental = a_afluencia_natural.getElemento(idUHE);

			// Verifica se UHE possui usina � Montante.
			const int numMontante = getSizeVetor(idUHE, AttVetorHidreletrica_montante);

			for (int m = 1; m <= numMontante; m++) {
				const IdHidreletrica idUHE_montante = getElementoVetor(idUHE, AttVetorHidreletrica_montante, m, IdHidreletrica());

				// Atualiza Aflu�ncia Incremental final descontando a Aflu�ncia Natural da UHE � Montante.
				const double afluencia_natural_montante = a_afluencia_natural.getElemento(idUHE_montante);

				afluencia_incremental -= afluencia_natural_montante;

			} // for (int m = 1; m <= numMontante; m++) {

			afluencia_incremental_retorno.setElemento(idUHE, afluencia_incremental);

		} // for (IdHidreletrica idUHE = menorIdUHE; idUHE <= maiorIdUHE; idUHE++) {

		return afluencia_incremental_retorno;

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::calculaAfluenciaIncremental(a_afluenciaNatural): \n" + std::string(erro.what())); }

} // SmartEnupla<IdHidreletrica, double> Dados::calculaAfluenciaIncremental(const SmartEnupla<IdHidreletrica, double> &a_afluenciaNatural) {



void Dados::validacao_operacional_RestricaoOperativaUHE(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar) {

	try {

		const IdProcesso idProcesso = arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso());

		bool calcular_att_operacionais_restricao_operativa_UHE = false;
		bool calcular_att_operacionais_elemento_sistema = false;

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_estudo_inicial = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();

		IdPatamarCarga maiorIdPatamarCarga_horizonte = IdPatamarCarga_Nenhum;
		for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {
			const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
			if (maiorIdPatamarCarga > maiorIdPatamarCarga_horizonte)
				maiorIdPatamarCarga_horizonte = maiorIdPatamarCarga;
		} // for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

		validaRestricaoOperativaUHE();

		IdRestricaoOperativaUHE maiorIdRestricaoOperativaUHE_operacional = IdRestricaoOperativaUHE_Nenhum;

		const IdRestricaoOperativaUHE maiorIdRestricaoOperativaUHE = getMaiorId(IdRestricaoOperativaUHE());
		for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= maiorIdRestricaoOperativaUHE; idRestricaoOperativaUHE++) {

			if ((getSize1Matriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior) == 0) || (getSize1Matriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior) == 0)) {

				calcular_att_operacionais_restricao_operativa_UHE = true;

				bool preencher_limite_inferior = false;
				if (getSize1Matriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior) == 0) {
					preencher_limite_inferior = true;
					if (getSizeVetor(idRestricaoOperativaUHE, AttVetorRestricaoOperativaUHE_limite_inferior) == 0)
						vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).setVetor(AttVetorRestricaoOperativaUHE_limite_inferior, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_limite_inferior, double())));
				}

				bool preencher_limite_superior = false;
				if (getSize1Matriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior) == 0) {
					preencher_limite_superior = true;
					if (getSizeVetor(idRestricaoOperativaUHE, AttVetorRestricaoOperativaUHE_limite_superior) == 0)
						vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).setVetor(AttVetorRestricaoOperativaUHE_limite_superior, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_limite_superior, double())));
				}

				for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

					const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
					for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						if (preencher_limite_inferior)
							vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).addElemento(AttMatrizRestricaoOperativaUHE_limite_inferior, periodo, idPatamarCarga, getElementoVetor(idRestricaoOperativaUHE, AttVetorRestricaoOperativaUHE_limite_inferior, periodo, double()));

						if (preencher_limite_superior)
							vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).addElemento(AttMatrizRestricaoOperativaUHE_limite_superior, periodo, idPatamarCarga, getElementoVetor(idRestricaoOperativaUHE, AttVetorRestricaoOperativaUHE_limite_superior, periodo, double()));

						const double limite_inferior = getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, periodo, idPatamarCarga, double());
						const double limite_superior = getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, periodo, idPatamarCarga, double());

						if (limite_inferior > limite_superior)
							throw std::invalid_argument("limite_inferior maior que limite_superior em " + getString(periodo) + " em " + getString(idPatamarCarga) + " em " + getString(idRestricaoOperativaUHE));

					} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {
				} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

			} // if ((getSize1Matriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior) == 0) || (getSize1Matriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior) == 0)) {

			if (getSizeVetor(idRestricaoOperativaUHE, AttVetorRestricaoOperativaUHE_limite_superior_folga_inferior) == 0) {
				vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).setVetor(AttVetorRestricaoOperativaUHE_limite_superior_folga_inferior, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_limite_superior_folga_inferior, double())));
			}//if (getSizeVetor(AttVetorRestricaoOperativaUHE_limite_superior_folga_inferior) == 0) {

			const IdElementoSistema maiorIdElementoSistema = getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());

			for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorIdElementoSistema; idElementoSistema++) {

				if (getSize1Matriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao) == 0) {

					calcular_att_operacionais_elemento_sistema = true;

					if (getSizeVetor(idRestricaoOperativaUHE, idElementoSistema, AttVetorElementoSistema_fator_participacao) == 0)
						vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).vetorElementoSistema.att(idElementoSistema).setVetor(AttVetorElementoSistema_fator_participacao, SmartEnupla<Periodo, double>(horizonte_estudo, getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_fator_participacao, double())));

					for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

						const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
						for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++)
							vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).vetorElementoSistema.att(idElementoSistema).addElemento(AttMatrizElementoSistema_fator_participacao, periodo, idPatamarCarga, getElementoVetor(idRestricaoOperativaUHE, idElementoSistema, AttVetorElementoSistema_fator_participacao, periodo, double()));

					} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

				} // if (getSize1Matriz(idRestricaoOperativaUHE, idElementoSistema, AttMatrizElementoSistema_fator_participacao) == 0) {

			} // for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorIdElementoSistema; idElementoSistema++) {

			const bool restricao_simples = isRestricaoOperativaUHE_simples(idRestricaoOperativaUHE);

			if (getAtributo(AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_hidraulicas, bool()) && restricao_simples && !a_imprimir_atributos_sem_recarregar)
				throw std::invalid_argument("Nao considerar " + getFullString(idRestricaoOperativaUHE) + " com " + getFullString(AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_hidraulicas) + " positivo.");
			else if (getAtributo(AttComumDados_nao_utilizar_restricoes_simples_em_restricoes_hidraulicas, bool()) && restricao_simples) {}
			else
				maiorIdRestricaoOperativaUHE_operacional = idRestricaoOperativaUHE;

			for (AttMatrizRestricaoOperativaUHE attMatrizRestricaoOperativaUHE = AttMatrizRestricaoOperativaUHE(1); attMatrizRestricaoOperativaUHE < AttMatrizRestricaoOperativaUHE_Excedente; attMatrizRestricaoOperativaUHE++) {

				const AttVetorRestricaoOperativaUHE attVetorRestricaoOperativaUHE_media = getAttVetorRestricaoOperativaUHEFromChar(std::string(getString(attMatrizRestricaoOperativaUHE) + "_media").c_str());

				if (attVetorRestricaoOperativaUHE_media != AttVetorRestricaoOperativaUHE_Nenhum) {

					SmartEnupla<Periodo, SmartEnupla<IdPatamarCarga, double>> matriz = getMatriz(idRestricaoOperativaUHE, attMatrizRestricaoOperativaUHE, Periodo(), IdPatamarCarga(), double());

					if (matriz.size() > 0) {

						SmartEnupla<Periodo, double> vetor(matriz, 0.0);

						for (Periodo periodo = matriz.getIteradorInicial(); periodo <= periodo_final_estudo; matriz.incrementarIterador(periodo)) {
							for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= matriz.at(periodo).getIteradorFinal(); idPatamarCarga++)
								vetor.at(periodo) += matriz.at(periodo).at(idPatamarCarga) * getElementoMatriz(AttMatrizDados_percentual_duracao_patamar_carga, periodo, idPatamarCarga, double());
						}

						vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).setVetor_forced(attVetorRestricaoOperativaUHE_media, vetor);

					} // if (matriz.size() > 0) {

				} // if (attVetorRestricaoOperativaUHE_media != AttVetorRestricaoOperativaUHE_Nenhum) {

			} // for (AttMatrizRestricaoOperativaUHE attMatrizRestricaoOperativaUHE = AttMatrizRestricaoOperativaUHE(1); attMatrizRestricaoOperativaUHE < AttMatrizRestricaoOperativaUHE_Excedente; attMatrizRestricaoOperativaUHE++) {

		} // for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= maiorIdRestricaoOperativaUHE; idRestricaoOperativaUHE++) {

		const IdRestricaoOperativaUHE menorIdRestricaoOperativaUHE_nao_operacional = IdRestricaoOperativaUHE(maiorIdRestricaoOperativaUHE_operacional + 1);

		if (idProcesso == IdProcesso_mestre) {

			if (true) {
				bool impresso = false;

				for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= maiorIdRestricaoOperativaUHE_operacional; idRestricaoOperativaUHE++) {

					for (AttMatrizRestricaoOperativaUHE attMatrizRestricaoOperativaUHE = AttMatrizRestricaoOperativaUHE(1); attMatrizRestricaoOperativaUHE < AttMatrizRestricaoOperativaUHE_Excedente; attMatrizRestricaoOperativaUHE++) {

						const AttVetorRestricaoOperativaUHE attVetorRestricaoOperativaUHE_media = getAttVetorRestricaoOperativaUHEFromChar(std::string(getString(attMatrizRestricaoOperativaUHE) + "_media").c_str());

						if (attVetorRestricaoOperativaUHE_media != AttVetorRestricaoOperativaUHE_Nenhum) {

							a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

							a_entradaSaidaDados.setAppendArquivo(impresso);
							a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("_info_RESTRICAO_OPERATIVA_UHE_AttVetorPremissa_PorPeriodo.csv", idRestricaoOperativaUHE, *this, periodo_estudo_inicial, periodo_final_estudo, attVetorRestricaoOperativaUHE_media);
							impresso = true;

							vetorRestricaoOperativaUHE.att(idRestricaoOperativaUHE).clear(attVetorRestricaoOperativaUHE_media);

						} // if (attVetorRestricaoOperativaUHE_media != AttVetorRestricaoOperativaUHE_Nenhum) {
					} // for (AttMatrizRestricaoOperativaUHE attMatrizRestricaoOperativaUHE = AttMatrizRestricaoOperativaUHE(1); attMatrizRestricaoOperativaUHE < AttMatrizRestricaoOperativaUHE_Nenhum; attMatrizRestricaoOperativaUHE++) {
				} // for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= maiorIdRestricaoOperativaUHE_operacional; idRestricaoOperativaUHE++) {
			} // if (true) {
		} // if (idProcesso == IdProcesso_mestre) {

		if ((calcular_att_operacionais_restricao_operativa_UHE) || (calcular_att_operacionais_elemento_sistema) || (a_imprimir_atributos_sem_recarregar)) {

			if (idProcesso == IdProcesso_mestre) {

				if (calcular_att_operacionais_restricao_operativa_UHE) {

					//
					// Imprime Atributos Premissas
					//
					a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

					for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= maiorIdRestricaoOperativaUHE; idRestricaoOperativaUHE++) {

						if (idRestricaoOperativaUHE == IdRestricaoOperativaUHE_1)
							a_entradaSaidaDados.setAppendArquivo(false);
						else
							a_entradaSaidaDados.setAppendArquivo(true);

						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("RESTRICAO_OPERATIVA_UHE_AttVetorPremissa_PorPeriodo.csv", idRestricaoOperativaUHE, *this, periodo_estudo_inicial, periodo_final_estudo, AttVetorRestricaoOperativaUHE_limite_inferior);
						a_entradaSaidaDados.setAppendArquivo(true);
						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("RESTRICAO_OPERATIVA_UHE_AttVetorPremissa_PorPeriodo.csv", idRestricaoOperativaUHE, *this, periodo_estudo_inicial, periodo_final_estudo, AttVetorRestricaoOperativaUHE_limite_superior);

					}

				} // if (calcular_att_operacionais_restricao_eletrica) {

				if (calcular_att_operacionais_elemento_sistema) {

					//
					// Imprime Atributos Premissas
					//
					a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);

					for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= maiorIdRestricaoOperativaUHE; idRestricaoOperativaUHE++) {

						if (idRestricaoOperativaUHE == IdRestricaoOperativaUHE_1)
							a_entradaSaidaDados.setAppendArquivo(false);
						else
							a_entradaSaidaDados.setAppendArquivo(true);

						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("RESTRICAO_OPERATIVA_UHE_ELEMENTO_SISTEMA_AttVetorPremissa_PorPeriodo.csv", idRestricaoOperativaUHE, IdElementoSistema_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, AttVetorElementoSistema_fator_participacao);

					}

				} // if (calcular_att_operacionais_elemento_sistema) {

				//
				// Imprime Atributos Operacionais
				//

				for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= maiorIdRestricaoOperativaUHE; idRestricaoOperativaUHE++) {

					if (idRestricaoOperativaUHE < menorIdRestricaoOperativaUHE_nao_operacional)
						a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_operacional);
					else
						a_entradaSaidaDados.setDiretorioSaida(a_diretorio_att_premissa);			

					if ((idRestricaoOperativaUHE == IdRestricaoOperativaUHE_1) || (idRestricaoOperativaUHE == menorIdRestricaoOperativaUHE_nao_operacional))
						a_entradaSaidaDados.setAppendArquivo(false);
					else
						a_entradaSaidaDados.setAppendArquivo(true);

					a_entradaSaidaDados.imprimirArquivoCSV_AttComum("RESTRICAO_OPERATIVA_UHE_AttComumOperacional.csv", idRestricaoOperativaUHE, *this, std::vector<AttComumRestricaoOperativaUHE> {AttComumRestricaoOperativaUHE_idRestricaoOperativaUHE, AttComumRestricaoOperativaUHE_nome, AttComumRestricaoOperativaUHE_tipoRestricaoHidraulica, AttComumRestricaoOperativaUHE_penalidade_restricao_operativa_UHE});

					if (idRestricaoOperativaUHE == IdRestricaoOperativaUHE_1)
						a_entradaSaidaDados.setAppendArquivo(false);
					else
						a_entradaSaidaDados.setAppendArquivo(true);

					a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("RESTRICAO_OPERATIVA_UHE_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", idRestricaoOperativaUHE, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizRestricaoOperativaUHE_limite_inferior);
					a_entradaSaidaDados.setAppendArquivo(true);
					a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("RESTRICAO_OPERATIVA_UHE_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", idRestricaoOperativaUHE, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizRestricaoOperativaUHE_limite_superior);


					if (idRestricaoOperativaUHE == IdRestricaoOperativaUHE_1)
						a_entradaSaidaDados.setAppendArquivo(false);
					else
						a_entradaSaidaDados.setAppendArquivo(true);

					a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("RESTRICAO_OPERATIVA_UHE_AttVetorOperacional_PorPeriodo.csv", idRestricaoOperativaUHE, *this, periodo_estudo_inicial, periodo_final_estudo, AttVetorRestricaoOperativaUHE_limite_superior_folga_inferior);


					if (idRestricaoOperativaUHE == IdRestricaoOperativaUHE_1)
						a_entradaSaidaDados.setAppendArquivo(false);
					else
						a_entradaSaidaDados.setAppendArquivo(true);

					a_entradaSaidaDados.imprimirArquivoCSV_AttComum("RESTRICAO_OPERATIVA_UHE_ELEMENTO_SISTEMA_AttComumOperacional.csv", idRestricaoOperativaUHE, IdElementoSistema_Nenhum, *this, std::vector<AttComumElementoSistema> {AttComumElementoSistema_idElementoSistema, AttComumElementoSistema_tipo_elemento, AttComumElementoSistema_unidadehidraulica, AttComumElementoSistema_conjuntohidraulico, AttComumElementoSistema_hidreletrica, AttComumElementoSistema_termeletrica, AttComumElementoSistema_unidadeTermeletrica, AttComumElementoSistema_contrato, AttComumElementoSistema_intercambio, AttComumElementoSistema_eolica, AttComumElementoSistema_usina_elevatoria, AttComumElementoSistema_tipoVariavelRestricaoOperativa});

					if (idRestricaoOperativaUHE == IdRestricaoOperativaUHE_1)
						a_entradaSaidaDados.setAppendArquivo(false);
					else
						a_entradaSaidaDados.setAppendArquivo(true);

					a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("RESTRICAO_OPERATIVA_UHE_ELEMENTO_SISTEMA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", idRestricaoOperativaUHE, IdElementoSistema_Nenhum, *this, periodo_estudo_inicial, periodo_final_estudo, IdPatamarCarga_1, maiorIdPatamarCarga_horizonte, AttMatrizElementoSistema_fator_participacao);

				}

			} // if (idProcesso == IdProcesso_mestre){

			if (!a_imprimir_atributos_sem_recarregar) {

				if (true) {
					int barreira = 0;

					if (arranjoResolucao.getAtributo(AttComumArranjoResolucao_idProcesso, IdProcesso()) == IdProcesso_mestre) {
						for (IdProcesso idProcesso = IdProcesso_1; idProcesso <= arranjoResolucao.getMaiorId(IdProcesso()); idProcesso++)
							MPI_Send(&barreira, 1, MPI_INT, getRank(idProcesso), 0, MPI_COMM_WORLD);
					}
					else
						MPI_Recv(&barreira, 1, MPI_INT, getRank(IdProcesso_mestre), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}

				//
				// Esvazia Atributos
				//

				vetorRestricaoOperativaUHE = VetorRestricaoOperativaUHEEmDados();

				//
				// Recarrega Atributos Operacionais
				//		
				a_entradaSaidaDados.setDiretorioEntrada(a_diretorio_att_operacional);

				a_entradaSaidaDados.carregarArquivoCSV_AttComum("RESTRICAO_OPERATIVA_UHE_AttComumOperacional.csv", *this, TipoAcessoInstancia_membro);

				a_entradaSaidaDados.carregarArquivoCSV_AttVetor("RESTRICAO_OPERATIVA_UHE_AttVetorOperacional_PorInteiro.csv", *this, TipoAcessoInstancia_membro);
				
				a_entradaSaidaDados.carregarArquivoCSV_AttVetor("RESTRICAO_OPERATIVA_UHE_AttVetorOperacional_PorPeriodo.csv", *this, TipoAcessoInstancia_membro);

				a_entradaSaidaDados.carregarArquivoCSV_AttMatriz("RESTRICAO_OPERATIVA_UHE_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membro);

				// Elemento Sistema

				a_entradaSaidaDados.carregarArquivoCSV_AttComum("RESTRICAO_OPERATIVA_UHE_ELEMENTO_SISTEMA_AttComumOperacional.csv", *this, TipoAcessoInstancia_membroMembro);

				a_entradaSaidaDados.carregarArquivoCSV_AttMatriz("RESTRICAO_OPERATIVA_UHE_ELEMENTO_SISTEMA_AttMatrizOperacional_PorPeriodoPorIdPatamarCarga.csv", *this, TipoAcessoInstancia_membroMembro);

			} // if (!a_imprimir_atributos_sem_recarregar) {

		} // if ((calcular_att_operacionais_restricao_operativa_UHE) || (calcular_att_operacionais_elemento_sistema) || (a_imprimir_atributos_sem_recarregar)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validacao_operacional_RestricaoOperativaUHE(a_entradaSaidaDados, " + a_diretorio_att_operacional + "," + a_diretorio_att_premissa + "): \n" + std::string(erro.what())); }

} // void Dados::validacao_operacional_RestricaoOperativaUHE(EntradaSaidaDados a_entradaSaidaDados, const std::string a_diretorio_att_operacional, const std::string a_diretorio_att_premissa, const bool a_imprimir_atributos_sem_recarregar){



void Dados::validaRestricaoOperativaUHE() {
	try {

		validaElementoSistemaRestricaoOperativaUHE();

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		const Periodo periodo_estudo_inicial = horizonte_estudo.getIteradorInicial();
		const Periodo periodo_final_estudo = horizonte_estudo.getIteradorFinal();

		const IdRestricaoOperativaUHE maiorIdRestricaoOperativaUHE = getMaiorId(IdRestricaoOperativaUHE());

		for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= maiorIdRestricaoOperativaUHE; idRestricaoOperativaUHE++) {

			const IdElementoSistema maiorIdElementoSistema = getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());

			if (maiorIdElementoSistema == IdElementoSistema_Nenhum)
				throw std::invalid_argument("Necessario incluir elementosSistema em " + getFullString(idRestricaoOperativaUHE));

			if ((getSize1Matriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior) > 0) && (getSize1Matriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior) > 0)) {

				for (Periodo periodo = periodo_estudo_inicial; periodo <= periodo_final_estudo; horizonte_estudo.incrementarIterador(periodo)) {

					const IdPatamarCarga maiorIdPatamarCarga = getIterador2Final(AttMatrizDados_percentual_duracao_patamar_carga, periodo, IdPatamarCarga());
					for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

						const double limite_inferior = getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior, periodo, idPatamarCarga, double());
						const double limite_superior = getElementoMatriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior, periodo, idPatamarCarga, double());

						if (limite_inferior > limite_superior)
							throw std::invalid_argument("limite_inferior maior que limite_superior em " + getString(periodo) + " em " + getFullString(idPatamarCarga) + " em " + getFullString(idRestricaoOperativaUHE));

					} // for (IdPatamarCarga idPatamarCarga = IdPatamarCarga_1; idPatamarCarga <= maiorIdPatamarCarga; idPatamarCarga++) {

				} // for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {

			} // if ((getSize1Matriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_inferior) > 0) && (getSize1Matriz(idRestricaoOperativaUHE, AttMatrizRestricaoOperativaUHE_limite_superior) > 0)) {

		} // for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= maiorIdRestricaoOperativaUHE; idRestricaoOperativaUHE++) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaRestricaoOperativaUHE(): \n" + std::string(erro.what())); }
} // void Dados::validaRestricaoOperativaUHE(){


void Dados::validaElementoSistemaRestricaoOperativaUHE() {

	try {

		const IdRestricaoOperativaUHE maiorIdRestricaoOperativaUHE = getMaiorId(IdRestricaoOperativaUHE());

		for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= maiorIdRestricaoOperativaUHE; idRestricaoOperativaUHE++) {

			const IdElementoSistema maiorIdElementoSistema = getMaiorId(idRestricaoOperativaUHE, IdElementoSistema());

			for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorIdElementoSistema; idElementoSistema++) {

				const TipoElementoSistema tipoElementoSistema = getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_tipo_elemento, TipoElementoSistema());

				bool elemento_identificado = false;

				//
				// Valida Hidreletrica / ConjuntoHidraulico / UnidadeUHE 
				//

				if (getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica()) != IdHidreletrica_Nenhum) {

					if (tipoElementoSistema != TipoElementoSistema_hidreletrica)
						throw std::invalid_argument("Valor " + getFullString(tipoElementoSistema) + " nao compativel com " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoOperativaUHE));

					const IdHidreletrica	idHidreletrica = getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());

					if (idHidreletrica != IdHidreletrica_Nenhum) {

						if (idHidreletrica > getMaiorId(IdHidreletrica()))
							throw std::invalid_argument("Hidreletrica nao instanciada em " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoOperativaUHE));

						elemento_identificado = true;

					}//if (idHidreletrica != IdHidreletrica_Nenhum) {
					else
						throw std::invalid_argument("Necessario informar a Hidreletrica em " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoOperativaUHE));

				}//if (getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica()) != IdHidreletrica_Nenhum) {
				else if (getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_conjuntohidraulico, IdConjuntoHidraulico()) != IdConjuntoHidraulico_Nenhum) {

					if (tipoElementoSistema != TipoElementoSistema_conjunto_hidraulico)
						throw std::invalid_argument("Valor " + getFullString(tipoElementoSistema) + " nao compativel com " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoOperativaUHE));

					const IdHidreletrica       idHidreletrica = getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
					const IdConjuntoHidraulico idConjuntoHidraulico = getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_conjuntohidraulico, IdConjuntoHidraulico());

					if (idHidreletrica != IdHidreletrica_Nenhum) {

						if (idHidreletrica > getMaiorId(IdHidreletrica()))
							throw std::invalid_argument("Hidreletrica nao instanciada em " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoOperativaUHE));
						else if (idConjuntoHidraulico > getMaiorId(idHidreletrica, IdConjuntoHidraulico()))
							throw std::invalid_argument("Conjunto Hidraulico nao instanciado em " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoOperativaUHE));

						elemento_identificado = true;

					}//if (idHidreletrica != IdHidreletrica_Nenhum) {
					else
						throw std::invalid_argument("Necessario informar a Hidreletrica associada ao ConjuntoHidraulico em " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoOperativaUHE));

				}//else if (getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_conjuntohidraulico, IdConjuntoHidraulico()) != IdConjuntoHidraulico_Nenhum) {
				else if (getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_unidadehidraulica, IdUnidadeUHE()) != IdUnidadeUHE_Nenhum) {

					if (tipoElementoSistema != TipoElementoSistema_unidade_hidreletrica)
						throw std::invalid_argument("Valor " + getFullString(tipoElementoSistema) + " nao compativel com " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoOperativaUHE));

					const IdHidreletrica       idHidreletrica = getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_hidreletrica, IdHidreletrica());
					const IdConjuntoHidraulico idConjuntoHidraulico = getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_conjuntohidraulico, IdConjuntoHidraulico());
					const IdUnidadeUHE         idUnidadeUHE = getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_unidadehidraulica, IdUnidadeUHE());

					if (idHidreletrica != IdHidreletrica_Nenhum) {

						if (idConjuntoHidraulico != IdConjuntoHidraulico_Nenhum) {

							if (idHidreletrica > getMaiorId(IdHidreletrica()))
								throw std::invalid_argument("Hidreletrica nao instanciada em " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoOperativaUHE));
							else if (idConjuntoHidraulico > getMaiorId(idHidreletrica, IdConjuntoHidraulico()))
								throw std::invalid_argument("Conjunto Hidraulico nao instanciado em " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoOperativaUHE));
							else if (idUnidadeUHE > getMaiorId(idHidreletrica, idConjuntoHidraulico, IdUnidadeUHE()))
								throw std::invalid_argument("UnidadeUHE nao instanciado em " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoOperativaUHE));

						}//if (idConjuntoHidraulico != IdConjuntoHidraulico_Nenhum) {
						else
							throw std::invalid_argument("Necessario informar o ConjuntoHidraulico associada a UnidadeUHE em " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoOperativaUHE));

						elemento_identificado = true;

					}//if (idHidreletrica != IdHidreletrica_Nenhum) {
					else
						throw std::invalid_argument("Necessario informar a Hidreletrica associada a UnidadeUHE em " + getFullString(idElementoSistema) + " em " + getFullString(idRestricaoOperativaUHE));

				}//else if (getAtributo(idRestricaoOperativaUHE, idElementoSistema, AttComumElementoSistema_unidadehidraulica, IdUnidadeUHE()) != IdUnidadeUHE_Nenhum) {

			} // for (IdElementoSistema idElementoSistema = IdElementoSistema_1; idElementoSistema <= maiorIdElementoSistema; idElementoSistema++) {

		} // for (IdRestricaoOperativaUHE idRestricaoOperativaUHE = IdRestricaoOperativaUHE_1; idRestricaoOperativaUHE <= maiorIdRestricaoOperativaUHE; idRestricaoOperativaUHE++) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::validaElementoSistemaRestricaoOperativaUHE(): \n" + std::string(erro.what())); }

} // void Dados::validaElementoSistemaRestricaoOperativaUHE(){


void Dados::valida_preconfig_hidraulica(const SmartEnupla<IdHidreletrica, IdHidreletrica> a_lista_jusante_hidreletrica, const SmartEnupla<IdHidreletrica, IdHidreletrica> a_lista_jusante_desvio_hidreletrica) {

	try {

		const IdHidreletrica maiorIdHidreletrica = getMaiorId(IdHidreletrica());

		for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

			if (vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_jusante, IdHidreletrica()) != a_lista_jusante_hidreletrica.getElemento(idHidreletrica))
				throw std::invalid_argument("Inconsistencia na preconfiguracao hidraulica - usina " + vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_nome, std::string()) + " deve ter como jusante " + vetorHidreletrica.att(a_lista_jusante_hidreletrica.getElemento(idHidreletrica)).getAtributo(AttComumHidreletrica_nome, std::string()));

			if (vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_jusante_desvio, IdHidreletrica()) != a_lista_jusante_desvio_hidreletrica.getElemento(idHidreletrica))
				throw std::invalid_argument("Inconsistencia na preconfiguracao hidraulica - usina " + vetorHidreletrica.att(idHidreletrica).getAtributo(AttComumHidreletrica_nome, std::string()) + " deve ter como jusante_desvio " + vetorHidreletrica.att(a_lista_jusante_desvio_hidreletrica.getElemento(idHidreletrica)).getAtributo(AttComumHidreletrica_nome, std::string()));

		}//for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::valida_preconfig_hidraulica(): \n" + std::string(erro.what())); }

}//void Dados::valida_preconfig_hidraulica(const SmartEnupla<IdHidreletrica, IdHidreletrica> a_lista_jusante_hidreletrica, const SmartEnupla<IdHidreletrica, IdHidreletrica> a_lista_jusante_desvio_hidreletrica) {

void Dados::instanciaCotaJusanteUsinaJusante() {

	try {

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());
		const IdHidreletrica maiorIdHidreletrica = getMaiorId(IdHidreletrica());

		for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

			//AttVetorHidreletrica_cota_montante_usina_jusante
			if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_cota_montante_usina_jusante) > 0) {
				vetorHidreletrica.att(idHidreletrica).setVetor(AttVetorHidreletrica_cota_montante_usina_jusante, SmartEnupla<Periodo, double>(horizonte_estudo, 0.0));
				IdHidreletrica idHidreletricaJusante = getAtributo(idHidreletrica, AttComumHidreletrica_jusante, IdHidreletrica());

				if (IdHidreletrica_Nenhum != idHidreletricaJusante) {
					for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {
						double volumeMinimo = 0;
						if (getSizeVetor(idHidreletricaJusante, IdReservatorio_1, AttVetorReservatorio_volume_minimo) > 0)
							volumeMinimo = getElementoVetor(idHidreletricaJusante, IdReservatorio_1, AttVetorReservatorio_volume_minimo, periodo, double());
						else
							volumeMinimo = getAtributo(idHidreletricaJusante, IdReservatorio_1, AttComumReservatorio_volume_minimo, double());

						double volumeUtil = 0;
						if (getSizeVetor(idHidreletricaJusante, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo) > 0)
							volumeUtil = getElementoVetor(idHidreletricaJusante, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, periodo, double());

						else if (getSizeVetor(idHidreletricaJusante, IdReservatorio_1, AttVetorReservatorio_volume_maximo) > 0) {
							const double volumeMaximo = getElementoVetor(idHidreletricaJusante, IdReservatorio_1, AttVetorReservatorio_volume_maximo, periodo, double());
							volumeUtil = volumeMaximo - volumeMinimo;
						}
						else {
							const double volumeMaximo = getAtributo(idHidreletricaJusante, IdReservatorio_1, AttComumReservatorio_volume_maximo, double());
							volumeUtil = volumeMaximo - volumeMinimo;
						}
						//const double volumeInicial = volumeMinimo + volumeUtil * getAtributo(idHidreletricaJusante, IdReservatorio_1, AttComumReservatorio_percentual_volume_util_inicial, double());

						double volumeInicial = volumeMinimo;

						if (periodo == horizonte_estudo.getIteradorInicial())
							volumeInicial += getAtributo(idHidreletricaJusante, IdReservatorio_1, AttComumReservatorio_volume_util_inicial, double());
						else
							volumeInicial += 0.5 * (getElementoVetor(idHidreletricaJusante, IdReservatorio_1, AttVetorReservatorio_volume_util_maximo, periodo, double()) + getElementoVetor(idHidreletricaJusante, IdReservatorio_1, AttVetorReservatorio_volume_util_minimo, periodo, double()));

						vetorHidreletrica.att(idHidreletrica).setElemento(AttVetorHidreletrica_cota_montante_usina_jusante, periodo, \
							vetorHidreletrica.att(idHidreletricaJusante).vetorReservatorio.att(IdReservatorio_1).getCota(volumeInicial));

					}//	for (Periodo periodo = horizonte_estudo.getIteradorInicial(); periodo <= horizonte_estudo.getIteradorFinal(); horizonte_estudo.incrementarIterador(periodo)) {				
				}//if (IdHidreletrica_Nenhum != idHidreletricaJusante) {
			}//if (getSizeVetor(idHidreletrica, AttVetorHidreletrica_cota_montante_usina_jusante) > 0) {

		}//for (IdHidreletrica idHidreletrica = getMenorId(IdHidreletrica()); idHidreletrica <= maiorIdHidreletrica; vetorHidreletrica.incr(idHidreletrica)) {

	} // try{
	catch (const std::exception& erro) { throw std::invalid_argument("Dados::instanciaCotaJusanteUsinaJusante(): \n" + std::string(erro.what())); }

}

IdVariavelAleatoriaInterna Dados::getIdVariavelAleatoriaInternaFromIdVariavelAleatoriaIdHidreletrica(const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria, const IdHidreletrica a_hidreletrica) {

	try {

		for (IdVariavelAleatoriaInterna idVarInterna = IdVariavelAleatoriaInterna_1; idVarInterna <= getMaiorId(a_idProcessoEstocastico, a_idVariavelAleatoria, IdVariavelAleatoriaInterna()); idVarInterna++) {
			if (getIdHidreletricaFromIdProcessoEstocasticoIdVariavelAleatoriaIdVariavelAleatoriaInterna(a_idProcessoEstocastico, a_idVariavelAleatoria, idVarInterna) == a_hidreletrica)
				return idVarInterna;
		}

		throw std::invalid_argument("Hidreletrica nao encontrada.");

	}
	catch (const std::exception& erro) { throw std::invalid_argument("Dados(" + getString(getIdObjeto()) + ")::getIdVariavelAleatoriaInternaFromIdVariavelAleatoriaIdHidreletrica(" + getFullString(a_idProcessoEstocastico) + "," + getFullString(a_idVariavelAleatoria) + "," + getFullString(a_hidreletrica) + "): \n" + std::string(erro.what())); }

}

IdHidreletrica Dados::getIdHidreletricaFromIdProcessoEstocasticoIdVariavelAleatoriaIdVariavelAleatoriaInterna(const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria, const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna) {

	try {


		std::string valor = processoEstocastico_hidrologico.vetorVariavelAleatoria.att(a_idVariavelAleatoria).vetorVariavelAleatoriaInterna.att(a_idVariavelAleatoriaInterna).getAtributo(AttComumVariavelAleatoriaInterna_nome, std::string());

		const size_t pos_ = valor.find("_");
		if (pos_ != std::string::npos)
			valor = valor.substr(pos_ + 1, valor.length());

		const IdHidreletrica idHidreletrica = getIdHidreletricaFromChar(valor.c_str());
		if (idHidreletrica == IdHidreletrica_Nenhum)
			throw std::invalid_argument("Hidreletrica invalida em " + getFullString(a_idVariavelAleatoriaInterna));

		return idHidreletrica;

	}
	catch (const std::exception& erro) { throw std::invalid_argument("Dados(" + getString(getIdObjeto()) + ")::getIdHidreletricaFromIdProcessoEstocasticoIdVariavelAleatoriaIdVariavelAleatoriaInterna(" + getFullString(a_idProcessoEstocastico) + "," + getFullString(a_idVariavelAleatoria) + "," + getFullString(a_idVariavelAleatoriaInterna) + "): \n" + std::string(erro.what())); }

}

void Dados::getIdVariavelAleatoriaIdVariavelAleatoriaInternaFromIdHidreletrica(const IdProcessoEstocastico a_idProcessoEstocastico, IdVariavelAleatoria& a_idVariavelAleatoria, IdVariavelAleatoriaInterna& a_idVariavelAleatoriaInterna, const IdHidreletrica a_hidreletrica) {

	try {

		

		for (IdVariavelAleatoria idVar = IdVariavelAleatoria_1; idVar <= processoEstocastico_hidrologico.getMaiorId(IdVariavelAleatoria()); idVar++) {

			const IdVariavelAleatoriaInterna maiorIdVariavelAleatoriaInterna = processoEstocastico_hidrologico.getMaiorId(idVar, IdVariavelAleatoriaInterna());

			for (IdVariavelAleatoriaInterna idVarInterna = IdVariavelAleatoriaInterna_1; idVarInterna <= maiorIdVariavelAleatoriaInterna; idVarInterna++) {
				if (getIdHidreletricaFromIdProcessoEstocasticoIdVariavelAleatoriaIdVariavelAleatoriaInterna(a_idProcessoEstocastico, idVar, idVarInterna) == a_hidreletrica) {
					a_idVariavelAleatoria = idVar;
					a_idVariavelAleatoriaInterna = idVarInterna;
					return;
				}
			}
		}


		throw std::invalid_argument("Hidreletrica nao encontrada.");

	}
	catch (const std::exception& erro) { throw std::invalid_argument("Dados(" + getString(getIdObjeto()) + ")::getIdProcessoEstocasticoIdVariavelAleatoriaIdVariavelAleatoriaInternaFromIdHidreletrica(" + getFullString(a_idProcessoEstocastico) + "," + getFullString(a_idVariavelAleatoria) + "," + getFullString(a_idVariavelAleatoriaInterna) + "," + getFullString(a_hidreletrica) + "): \n" + std::string(erro.what())); }

}