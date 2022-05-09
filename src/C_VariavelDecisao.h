#ifndef VARIAVEL_DECISAO
#define VARIAVEL_DECISAO


// ---------------------------------------------------------------------------------------
//
//                                Variaveis de Decisão Continuas 1
//
// ---------------------------------------------------------------------------------------

#define DECLARAR_VARIAVEL_DECISAO_1(Nome, TIt, ImprimirPrimal, ImprimirDual)    \
std::vector<SmartEnupla<TIt, std::vector<int>>> idx_var_##Nome##_1 = std::vector<SmartEnupla<TIt, std::vector<int>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, std::vector<int>>());        \
std::string getNomeVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1) { \
	if (vetorEstagio.att(a_it1).getSolver(a_TSS)->isNomeSimplificado()) \
		return std::string("var" + std::string(#Nome) + "," + getString(a_it1)); \
	else \
		return std::string("var" + std::string(#Nome) + "," + getFullString(a_it1)); \
};\
std::vector<bool> is_var_decisao_##Nome##_1_instanciada = std::vector<bool>(TipoSubproblemaSolver_Excedente, false);  \
int addVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const double a_valor_inferior, const double a_valor_superior, const double a_valor_objetivo){ \
	try{ \
		const int idVariavelDecisao = vetorEstagio.att(a_it1).getSolver(a_TSS)->addVar(a_valor_inferior, a_valor_superior, a_valor_objetivo, getNomeVarDecisao_##Nome(a_TSS, a_it1)); \
		if (idx_var_##Nome##_1.at(a_TSS).size() == 0) \
			idx_var_##Nome##_1.at(a_TSS).addElemento(a_it1, std::vector<int>(1, idVariavelDecisao)); \
		else if (idx_var_##Nome##_1.at(a_TSS).getIteradorFinal() < a_it1){ \
			for (TIt it1 = TIt(idx_var_##Nome##_1.at(a_TSS).getIteradorFinal() + 1); it1 < a_it1; it1++)\
				idx_var_##Nome##_1.at(a_TSS).addElemento(it1, std::vector<int>()); \
			idx_var_##Nome##_1.at(a_TSS).addElemento(a_it1, std::vector<int>(1, idVariavelDecisao)); \
		} \
		else if (idx_var_##Nome##_1.at(a_TSS).at(a_it1).size() == 0) \
			idx_var_##Nome##_1.at(a_TSS).setElemento(a_it1, std::vector<int>(1, idVariavelDecisao)); \
		else \
			throw std::invalid_argument("Variavel ja existente no modelo."); \
		if (!is_var_decisao_##Nome##_1_instanciada.at(a_TSS)){ \
			is_var_decisao_##Nome##_1_instanciada.at(a_TSS) = true;\
			lista_variaveis_instanciadas.at(a_TSS).push_back(std::string(std::string(#Nome) + "_1")); \
		} \
		return idVariavelDecisao; \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("addVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getString(a_valor_inferior) + "," + getString(a_valor_superior) + "," + getString(a_valor_objetivo) + "): \n" + std::string(erro.what())); } \
}; \
int getVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1){ \
	try{ return idx_var_##Nome##_1.at(a_TSS).at(a_it1).at(0); } \
	catch (const std::exception& erro) { throw std::invalid_argument("getVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "): \n" + std::string(erro.what())); } \
}; \
int getVarDecisao_##Nome##seExistir(const TipoSubproblemaSolver a_TSS, const TIt a_it1){ \
	try{ \
		if (idx_var_##Nome##_1.at(a_TSS).size() == 0) \
			return -1; \
		else if (idx_var_##Nome##_1.at(a_TSS).getIteradorInicial() > a_it1) \
			return -1; \
		else if (idx_var_##Nome##_1.at(a_TSS).getIteradorFinal() < a_it1) \
			return -1; \
		else if (idx_var_##Nome##_1.at(a_TSS).at(a_it1).size() == 0) \
			return -1; \
		else \
			return idx_var_##Nome##_1.at(a_TSS).at(a_it1).at(0); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("getVarDecisao_" + std::string(#Nome) + "seExistir(" + getFullString(a_it1) + "): \n" + std::string(erro.what())); } \
}; \
std::vector<SmartEnupla<TIt, SmartEnupla<IdCenario, double>>> vlrP_e_c_var_##Nome##_1 = std::vector<SmartEnupla<TIt, SmartEnupla<IdCenario, double>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<IdCenario, double>>()); \
void inicializarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1){ \
	try{ \
		if (getboolFromChar(#ImprimirPrimal)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrP_e_c_var_##Nome##_1.at(a_TSS).size() == 0) \
				vlrP_e_c_var_##Nome##_1.at(a_TSS).addElemento(a_it1, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			else if (vlrP_e_c_var_##Nome##_1.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrP_e_c_var_##Nome##_1.at(a_TSS).getIteradorFinal();\
				idx_var_##Nome##_1.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_1.at(a_TSS).incrementarIterador(it1))\
					vlrP_e_c_var_##Nome##_1.at(a_TSS).addElemento(it1, SmartEnupla<IdCenario, double>()); \
				vlrP_e_c_var_##Nome##_1.at(a_TSS).addElemento(a_it1, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorPrimalPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt, SmartEnupla<IdCenario, double>>> vlrD_e_c_var_##Nome##_1 = std::vector<SmartEnupla<TIt, SmartEnupla<IdCenario, double>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<IdCenario, double>>()); \
void inicializarValorDualPorEstagioPorCenarioVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1){ \
	try{ \
		if (getboolFromChar(#ImprimirDual)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrD_e_c_var_##Nome##_1.at(a_TSS).size() == 0) \
				vlrD_e_c_var_##Nome##_1.at(a_TSS).addElemento(a_it1, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			else if (vlrD_e_c_var_##Nome##_1.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrD_e_c_var_##Nome##_1.at(a_TSS).getIteradorFinal();\
				idx_var_##Nome##_1.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_1.at(a_TSS).incrementarIterador(it1))\
					vlrD_e_c_var_##Nome##_1.at(a_TSS).addElemento(it1, SmartEnupla<IdCenario, double>()); \
				vlrD_e_c_var_##Nome##_1.at(a_TSS).addElemento(a_it1, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorDualPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>> vlrP_e_c_r_var_##Nome##_1 = std::vector<SmartEnupla<TIt, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
void inicializarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1){ \
	try{ \
		if (getboolFromChar(#ImprimirPrimal)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const IdRealizacao maiorIdRealizacao = getAtributo(a_it1, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrP_e_c_r_var_##Nome##_1.at(a_TSS).size() == 0) \
				vlrP_e_c_r_var_##Nome##_1.at(a_TSS).addElemento(a_it1, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			else if (vlrP_e_c_r_var_##Nome##_1.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrP_e_c_r_var_##Nome##_1.at(a_TSS).getIteradorFinal();\
				idx_var_##Nome##_1.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_1.at(a_TSS).incrementarIterador(it1))\
					vlrP_e_c_r_var_##Nome##_1.at(a_TSS).addElemento(it1, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>()); \
				vlrP_e_c_r_var_##Nome##_1.at(a_TSS).addElemento(a_it1, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>> vlrD_e_c_r_var_##Nome##_1 = std::vector<SmartEnupla<TIt, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
void inicializarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1){ \
	try{ \
		if (getboolFromChar(#ImprimirDual)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const IdRealizacao maiorIdRealizacao = getAtributo(a_it1, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrD_e_c_r_var_##Nome##_1.at(a_TSS).size() == 0) \
				vlrD_e_c_r_var_##Nome##_1.at(a_TSS).addElemento(a_it1, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			else if (vlrD_e_c_r_var_##Nome##_1.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrD_e_c_r_var_##Nome##_1.at(a_TSS).getIteradorFinal();\
				idx_var_##Nome##_1.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_1.at(a_TSS).incrementarIterador(it1))\
					vlrD_e_c_r_var_##Nome##_1.at(a_TSS).addElemento(it1, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>()); \
				vlrD_e_c_r_var_##Nome##_1.at(a_TSS).addElemento(a_it1, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_1(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario){ \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		if (idx_var_##Nome##_1.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_1.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_1.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		else if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1) > -1) {\
			inicializarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome(a_TSS, a_it1); \
			vlrP_e_c_var_##Nome##_1.at(a_TSS).at(a_it1).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getValorPrimal(getVarDecisao_##Nome(a_TSS, a_it1)); \
		} \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorPrimalPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_1(" + getFullString(a_it1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorDualPorEstagioPorCenarioVarDecisao_##Nome##_1(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario){ \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		if (idx_var_##Nome##_1.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_1.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_1.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		else if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1) > -1){ \
			inicializarValorDualPorEstagioPorCenarioVarDecisao_##Nome(a_TSS, a_it1); \
			vlrD_e_c_var_##Nome##_1.at(a_TSS).at(a_it1).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getReducedCost(a_TSS, getVarDecisao_##Nome(a_it1)); \
		} \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorDualPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_1(" + getFullString(a_it1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_1(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao){ \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		if (idx_var_##Nome##_1.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_1.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_1.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		else if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1) > -1){ \
			inicializarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(a_TSS, a_it1); \
			vlrP_e_c_r_var_##Nome##_1.at(a_TSS).at(a_it1).at(a_idRealizacao).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getValorPrimal(getVarDecisao_##Nome(a_TSS, a_it1)); \
		} \
	}\
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_1(" + getFullString(a_it1) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_1(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao){ \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		if (idx_var_##Nome##_1.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_1.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_1.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		else if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1) > -1){ \
			inicializarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(a_TSS, a_it1); \
			vlrD_e_c_r_var_##Nome##_1.at(a_TSS).at(a_it1).at(a_idRealizacao).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getReducedCost(getVarDecisao_##Nome(a_TSS, a_it1)); \
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_1(" + getFullString(a_it1) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
};\
void imprimirValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_1(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados);\
void imprimirValorDualPorEstagioPorCenarioVarDecisao_##Nome##_1(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_1(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_1(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void consolidarResultadosVarDecisao_##Nome##_1(const TipoSubproblemaSolver a_TSS, const std::string a_variavel, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_1(Nome, TIt, ImprimirPrimal, ImprimirDual) \
void ModeloOtimizacao::imprimirValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_1(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_1"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_1_primal_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrP_e_c_var_##Nome##_1.at(a_TSS))); \
		vlrP_e_c_var_##Nome##_1.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<IdCenario, double>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorPrimalPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_1(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\
void ModeloOtimizacao::imprimirValorDualPorEstagioPorCenarioVarDecisao_##Nome##_1(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_1"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_1_dual_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrD_e_c_var_##Nome##_1.at(a_TSS))); \
		vlrD_e_c_var_##Nome##_1.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<IdCenario, double>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorDualPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_1(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_1(Nome, TIt, ImprimirPrimal, ImprimirDual) \
imprimirValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_1(TSS, getFullString(a_idProcesso), a_entradaSaidaDados); \
imprimirValorDualPorEstagioPorCenarioVarDecisao_##Nome##_1(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);  

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_1(Nome, TIt, ImprimirPrimal, ImprimirDual) \
void ModeloOtimizacao::imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_1(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_1"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_1_primal_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrP_e_c_r_var_##Nome##_1.at(a_TSS))); \
		vlrP_e_c_r_var_##Nome##_1.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>> (); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_1(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\
void ModeloOtimizacao::imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_1(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_1"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_1_dual_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrD_e_c_r_var_##Nome##_1.at(a_TSS))); \
		vlrD_e_c_r_var_##Nome##_1.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>> (); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_1(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_1(Nome, TIt, ImprimirPrimal, ImprimirDual) \
imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_1(TSS, getFullString(a_idProcesso), a_entradaSaidaDados); \
imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_1(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);  


#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_1(Nome, TIt, ImprimirPrimal, ImprimirDual) \
armazenarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_1(a_TSS, a_idEstagio, a_idCenario); \
armazenarValorDualPorEstagioPorCenarioVarDecisao_##Nome##_1(a_TSS, a_idEstagio, a_idCenario);

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_1(Nome, TIt, ImprimirPrimal, ImprimirDual) \
armazenarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_1(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao); \
armazenarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_1(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao); 

#define DECLARAR_CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_1(Nome, TIt, ImprimirPrimal, ImprimirDual) \
void ModeloOtimizacao::consolidarResultadosVarDecisao_##Nome##_1(const TipoSubproblemaSolver a_TSS, const std::string a_variavel, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (std::string(std::string(#Nome) + "_1") != a_variavel) \
			return; \
		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida(); \
		if (getboolFromChar(#ImprimirPrimal)){ \
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//var" + std::string(#Nome) + "_1"); \
			const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_1_primal" + ".csv"); \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string("var" + std::string(#Nome) + "_1_primal_" + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoHorizontalCSV(nome_arquivo, lista_arquivos, 1, true); \
		} \
		if (getboolFromChar(#ImprimirDual)){ \
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//var" + std::string(#Nome) + "_1"); \
			const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_1_dual" + ".csv"); \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string("var" + std::string(#Nome) + "_1_dual_" + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoHorizontalCSV(nome_arquivo, lista_arquivos, 1, true); \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::consolidarResultadosVarDecisao_" + std::string(#Nome) + "_1(" + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_1(Nome, TIt, ImprimirPrimal, ImprimirDual) \
consolidarResultadosVarDecisao_##Nome##_1(a_TSS, variavel_str, a_maiorIdProcesso, a_entradaSaidaDados);

// ---------------------------------------------------------------------------------------
//
//                                Variaveis de Decisão Continuas 2
//
// ---------------------------------------------------------------------------------------

#define DECLARAR_VARIAVEL_DECISAO_2(Nome, TIt, TIt2, ImprimirPrimal, ImprimirDual)   \
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, std::vector<int>>>> idx_var_##Nome##_2 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, std::vector<int>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, std::vector<int>>>()); \
std::vector<bool> is_var_decisao_##Nome##_2_instanciada = std::vector<bool>(TipoSubproblemaSolver_Excedente, false);  \
std::string getNomeVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2) { \
	if (vetorEstagio.att(a_it1).getSolver(a_TSS)->isNomeSimplificado()) \
		return std::string("var" + std::string(#Nome) + "," + getString(a_it1) + "," + getString(a_it2)); \
	else \
		return std::string("var" + std::string(#Nome) + "," + getFullString(a_it1) + "," + getFullString(a_it2)); \
}; \
int addVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const double a_valor_inferior, const double a_valor_superior, const double a_valor_objetivo){ \
	try{ \
		const int idVariavelDecisao = vetorEstagio.att(a_it1).getSolver(a_TSS)->addVar(a_valor_inferior, a_valor_superior, a_valor_objetivo, getNomeVarDecisao_##Nome(a_TSS, a_it1, a_it2)); \
		if (idx_var_##Nome##_2.at(a_TSS).size() == 0) \
			idx_var_##Nome##_2.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, std::vector<int>>()); \
		else if (idx_var_##Nome##_2.at(a_TSS).getIteradorFinal() < a_it1){ \
			for (TIt it1 = TIt(idx_var_##Nome##_2.at(a_TSS).getIteradorFinal() + 1); it1 < a_it1; it1++)\
				idx_var_##Nome##_2.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, std::vector<int>>()); \
			idx_var_##Nome##_2.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, std::vector<int>>()); \
		} \
		if (idx_var_##Nome##_2.at(a_TSS).at(a_it1).size() == 0) \
			idx_var_##Nome##_2.at(a_TSS).at(a_it1).addElemento(a_it2, std::vector<int>(1, idVariavelDecisao)); \
		else if (idx_var_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2){ \
			for (TIt2 it2 = TIt2(idx_var_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal() + 1); it2 < a_it2; it2++)\
				idx_var_##Nome##_2.at(a_TSS).at(a_it1).addElemento(it2, std::vector<int>()); \
			idx_var_##Nome##_2.at(a_TSS).at(a_it1).addElemento(a_it2, std::vector<int>(1, idVariavelDecisao)); \
		} \
		else if (idx_var_##Nome##_2.at(a_TSS).at(a_it1).at(a_it2).size() == 0)\
			idx_var_##Nome##_2.at(a_TSS).at(a_it1).setElemento(a_it2, std::vector<int>(1, idVariavelDecisao)); \
		else \
			throw std::invalid_argument("Variavel ja existente no modelo."); \
		if (!is_var_decisao_##Nome##_2_instanciada.at(a_TSS)){ \
			is_var_decisao_##Nome##_2_instanciada.at(a_TSS) = true;\
			lista_variaveis_instanciadas.at(a_TSS).push_back(std::string(std::string(#Nome) + "_2")); \
		} \
		return idVariavelDecisao; \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("addVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getString(a_valor_inferior) + "," + getString(a_valor_superior) + "," + getString(a_valor_objetivo) + "): \n" + std::string(erro.what())); } \
}; \
int getVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2){ \
	try{ return idx_var_##Nome##_2.at(a_TSS).at(a_it1).at(a_it2).at(0); } \
	catch (const std::exception& erro) { throw std::invalid_argument("getVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "): \n" + std::string(erro.what())); } \
}; \
int getVarDecisao_##Nome##seExistir(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2){ \
	try{ \
		if (idx_var_##Nome##_2.at(a_TSS).size() == 0) \
			return -1; \
		else if (idx_var_##Nome##_2.at(a_TSS).getIteradorInicial() > a_it1) \
			return -1; \
		else if (idx_var_##Nome##_2.at(a_TSS).getIteradorFinal() < a_it1) \
			return -1; \
		else if (idx_var_##Nome##_2.at(a_TSS).at(a_it1).size() == 0) \
			return -1; \
		else if (idx_var_##Nome##_2.at(a_TSS).at(a_it1).getIteradorInicial() > a_it2) \
			return -1; \
		else if (idx_var_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) \
			return -1; \
		else if (idx_var_##Nome##_2.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
			return -1; \
		else \
			return idx_var_##Nome##_2.at(a_TSS).at(a_it1).at(a_it2).at(0); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("getVarDecisao_" + std::string(#Nome) + "seExistir(" + getFullString(a_it1) + "," + getFullString(a_it2) + "): \n" + std::string(erro.what())); } \
}; \
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<IdCenario, double>>>> vlrP_e_c_var_##Nome##_2 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<IdCenario, double>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<IdCenario, double>>>()); \
void inicializarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2){ \
	try{ \
		if (getboolFromChar(#ImprimirPrimal)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrP_e_c_var_##Nome##_2.at(a_TSS).size() == 0) \
				vlrP_e_c_var_##Nome##_2.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<IdCenario, double>>()); \
			else if (vlrP_e_c_var_##Nome##_2.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrP_e_c_var_##Nome##_2.at(a_TSS).getIteradorFinal(); \
				idx_var_##Nome##_2.at(a_TSS).incrementarIterador(it1_inicial); \
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_2.at(a_TSS).incrementarIterador(it1))\
					vlrP_e_c_var_##Nome##_2.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<IdCenario, double>>()); \
				vlrP_e_c_var_##Nome##_2.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<IdCenario, double>>()); \
			}\
			if (vlrP_e_c_var_##Nome##_2.at(a_TSS).at(a_it1).size() == 0) \
				vlrP_e_c_var_##Nome##_2.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			else if (vlrP_e_c_var_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = vlrP_e_c_var_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal(); \
				idx_var_##Nome##_2.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial); \
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_var_##Nome##_2.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					vlrP_e_c_var_##Nome##_2.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<IdCenario, double>()); \
				vlrP_e_c_var_##Nome##_2.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorPrimalPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<IdCenario, double>>>> vlrD_e_c_var_##Nome##_2 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<IdCenario, double>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<IdCenario, double>>>()); \
void inicializarValorDualPorEstagioPorCenarioVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2){ \
	try{ \
		if (getboolFromChar(#ImprimirDual)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrD_e_c_var_##Nome##_2.at(a_TSS).size() == 0) \
				vlrD_e_c_var_##Nome##_2.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<IdCenario, double>>()); \
			else if (vlrD_e_c_var_##Nome##_2.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrD_e_c_var_##Nome##_2.at(a_TSS).getIteradorFinal(); \
				idx_var_##Nome##_2.at(a_TSS).incrementarIterador(it1_inicial); \
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_2.at(a_TSS).incrementarIterador(it1))\
					vlrD_e_c_var_##Nome##_2.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<IdCenario, double>>()); \
				vlrD_e_c_var_##Nome##_2.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<IdCenario, double>>()); \
			}\
			if (vlrD_e_c_var_##Nome##_2.at(a_TSS).at(a_it1).size() == 0) \
				vlrD_e_c_var_##Nome##_2.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			else if (vlrD_e_c_var_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = vlrD_e_c_var_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal(); \
				idx_var_##Nome##_2.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial); \
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_var_##Nome##_2.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					vlrD_e_c_var_##Nome##_2.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<IdCenario, double>()); \
				vlrD_e_c_var_##Nome##_2.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorDualPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>> vlrP_e_c_r_var_##Nome##_2 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
void inicializarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2){ \
	try{ \
		if (getboolFromChar(#ImprimirPrimal)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const IdRealizacao maiorIdRealizacao = getAtributo(a_it1, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrP_e_c_r_var_##Nome##_2.at(a_TSS).size() == 0) \
				vlrP_e_c_r_var_##Nome##_2.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			else if (vlrP_e_c_r_var_##Nome##_2.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrP_e_c_r_var_##Nome##_2.at(a_TSS).getIteradorFinal(); \
				idx_var_##Nome##_2.at(a_TSS).incrementarIterador(it1_inicial); \
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_2.at(a_TSS).incrementarIterador(it1))\
					vlrP_e_c_r_var_##Nome##_2.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
				vlrP_e_c_r_var_##Nome##_2.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (vlrP_e_c_r_var_##Nome##_2.at(a_TSS).at(a_it1).size() == 0) \
				vlrP_e_c_r_var_##Nome##_2.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			else if (vlrP_e_c_r_var_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = vlrP_e_c_r_var_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal(); \
				idx_var_##Nome##_2.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial); \
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_var_##Nome##_2.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					vlrP_e_c_r_var_##Nome##_2.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>()); \
				vlrP_e_c_r_var_##Nome##_2.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>> vlrD_e_c_r_var_##Nome##_2 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
void inicializarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2){ \
	try{ \
		if (getboolFromChar(#ImprimirDual)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const IdRealizacao maiorIdRealizacao = getAtributo(a_it1, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrD_e_c_r_var_##Nome##_2.at(a_TSS).size() == 0) \
				vlrD_e_c_r_var_##Nome##_2.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			else if (vlrD_e_c_r_var_##Nome##_2.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrD_e_c_r_var_##Nome##_2.at(a_TSS).getIteradorFinal(); \
				idx_var_##Nome##_2.at(a_TSS).incrementarIterador(it1_inicial); \
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_2.at(a_TSS).incrementarIterador(it1))\
					vlrD_e_c_r_var_##Nome##_2.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
				vlrD_e_c_r_var_##Nome##_2.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (vlrD_e_c_r_var_##Nome##_2.at(a_TSS).at(a_it1).size() == 0) \
				vlrD_e_c_r_var_##Nome##_2.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			else if (vlrD_e_c_r_var_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = vlrD_e_c_r_var_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal(); \
				idx_var_##Nome##_2.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial); \
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_var_##Nome##_2.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					vlrD_e_c_r_var_##Nome##_2.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>()); \
				vlrD_e_c_r_var_##Nome##_2.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_2(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario) { \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		if (idx_var_##Nome##_2.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_2.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_2.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_var_##Nome##_2.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_var_##Nome##_2.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_var_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_var_##Nome##_2.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1, idIterador2) > -1) {\
				inicializarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome(a_TSS, a_it1, idIterador2); \
				vlrP_e_c_var_##Nome##_2.at(a_TSS).at(a_it1).at(idIterador2).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getValorPrimal(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2)); \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorPrimalPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_2(" + getFullString(a_it1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorDualPorEstagioPorCenarioVarDecisao_##Nome##_2(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario){ \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		if (idx_var_##Nome##_2.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_2.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_2.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_var_##Nome##_2.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_var_##Nome##_2.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_var_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_var_##Nome##_2.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1, idIterador2) > -1){ \
				inicializarValorDualPorEstagioPorCenarioVarDecisao_##Nome(a_TSS, a_it1, idIterador2); \
				vlrD_e_c_var_##Nome##_2.at(a_TSS).at(a_it1).at(idIterador2).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getReducedCost(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2)); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorDualPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_2(" + getFullString(a_it1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_2(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		if (idx_var_##Nome##_2.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_2.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_2.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_var_##Nome##_2.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_var_##Nome##_2.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_var_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_var_##Nome##_2.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1, idIterador2) > -1) {\
				inicializarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(a_TSS, a_it1, idIterador2); \
				vlrP_e_c_r_var_##Nome##_2.at(a_TSS).at(a_it1).at(idIterador2).at(a_idRealizacao).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getValorPrimal(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2)); \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_2(" + getFullString(a_it1) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_2(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		if (idx_var_##Nome##_2.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_2.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_2.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_var_##Nome##_2.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_var_##Nome##_2.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_var_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_var_##Nome##_2.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1, idIterador2) > -1){ \
				inicializarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(a_TSS, a_it1, idIterador2); \
				vlrD_e_c_r_var_##Nome##_2.at(a_TSS).at(a_it1).at(idIterador2).at(a_idRealizacao).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getReducedCost(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2)); \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_2(" + getFullString(a_it1) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
};\
void imprimirValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_2(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados);\
void imprimirValorDualPorEstagioPorCenarioVarDecisao_##Nome##_2(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_2(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_2(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void consolidarResultadosVarDecisao_##Nome##_2(const TipoSubproblemaSolver a_TSS, const std::string a_variavel, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_2(Nome, TIt, TIt2, ImprimirPrimal, ImprimirDual) \
void ModeloOtimizacao::imprimirValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_2(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_2"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_2_primal_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrP_e_c_var_##Nome##_2.at(a_TSS))); \
		vlrP_e_c_var_##Nome##_2.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<IdCenario, double>>>();\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorPrimalPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_2(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\
void ModeloOtimizacao::imprimirValorDualPorEstagioPorCenarioVarDecisao_##Nome##_2(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_2"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_2_dual_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrD_e_c_var_##Nome##_2.at(a_TSS))); \
		vlrD_e_c_var_##Nome##_2.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<IdCenario, double>>>();\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorDualPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_2(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_2(Nome, TIt, TIt2, ImprimirPrimal, ImprimirDual) \
imprimirValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_2(TSS, getFullString(a_idProcesso), a_entradaSaidaDados); \
imprimirValorDualPorEstagioPorCenarioVarDecisao_##Nome##_2(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);  \


#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_2(Nome, TIt, TIt2, ImprimirPrimal, ImprimirDual) \
void ModeloOtimizacao::imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_2(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_2"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_2_primal_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrP_e_c_r_var_##Nome##_2.at(a_TSS))); \
		vlrP_e_c_r_var_##Nome##_2.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_2(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\
void ModeloOtimizacao::imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_2(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_2"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_2_dual_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrD_e_c_r_var_##Nome##_2.at(a_TSS))); \
		vlrD_e_c_r_var_##Nome##_2.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_2(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_2(Nome, TIt, TIt2, ImprimirPrimal, ImprimirDual) \
imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_2(TSS, getFullString(a_idProcesso), a_entradaSaidaDados); \
imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_2(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);  \

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_2(Nome, TIt, TIt2, ImprimirPrimal, ImprimirDual) \
armazenarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_2(a_TSS, a_idEstagio, a_idCenario); \
armazenarValorDualPorEstagioPorCenarioVarDecisao_##Nome##_2(a_TSS, a_idEstagio, a_idCenario);

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_2(Nome, TIt, TIt2, ImprimirPrimal, ImprimirDual) \
armazenarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_2(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao); \
armazenarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_2(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao); 

#define DECLARAR_CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_2(Nome, TIt, TIt2, ImprimirPrimal, ImprimirDual) \
void ModeloOtimizacao::consolidarResultadosVarDecisao_##Nome##_2(const TipoSubproblemaSolver a_TSS, const std::string a_variavel, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (std::string(std::string(#Nome) + "_2") != a_variavel) \
			return; \
		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida(); \
		if (getboolFromChar(#ImprimirPrimal)){ \
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//var" + std::string(#Nome) + "_2"); \
			const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_2_primal" + ".csv"); \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string("var" + std::string(#Nome) + "_2_primal_" + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoHorizontalCSV(nome_arquivo, lista_arquivos, 2, true); \
		} \
		if (getboolFromChar(#ImprimirDual)){ \
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//var" + std::string(#Nome) + "_2"); \
			const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_2_dual" + ".csv"); \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string("var" + std::string(#Nome) + "_2_dual_" + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoHorizontalCSV(nome_arquivo, lista_arquivos, 2, true); \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::consolidarResultadosVarDecisao_" + std::string(#Nome) + "_2(" + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_2(Nome, TIt, TIt2, ImprimirPrimal, ImprimirDual) \
consolidarResultadosVarDecisao_##Nome##_2(a_TSS, variavel_str, a_maiorIdProcesso, a_entradaSaidaDados);

// ---------------------------------------------------------------------------------------
//
//                                Variaveis de Decisão Continuas 3
//
// ---------------------------------------------------------------------------------------

#define DECLARAR_VARIAVEL_DECISAO_3(Nome, TIt, TIt2, TIt3, ImprimirPrimal, ImprimirDual)   \
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, std::vector<int>>>>> idx_var_##Nome##_3 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, std::vector<int>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, std::vector<int>>>>());        \
std::vector<SmartEnupla<TIt, bool>> is_var_decisao_##Nome##_3_pre_alocada = std::vector<SmartEnupla<TIt, bool>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, bool>());  \
std::vector<bool> is_var_decisao_##Nome##_3_instanciada = std::vector<bool>(TipoSubproblemaSolver_Excedente, false);  \
void alocarVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const SmartEnupla<TIt2, SmartEnupla<TIt3, bool>> &a_enupla_alocacao){ \
	try{ \
		if (is_var_decisao_##Nome##_3_pre_alocada.at(a_TSS).size() == 0) \
			is_var_decisao_##Nome##_3_pre_alocada.at(a_TSS).addElemento(a_it1, true); \
		else if (is_var_decisao_##Nome##_3_pre_alocada.at(a_TSS).getIteradorFinal() < a_it1){ \
			for (TIt it1 = TIt(is_var_decisao_##Nome##_3_pre_alocada.at(a_TSS).getIteradorFinal() + 1); it1 < a_it1; it1++)\
				is_var_decisao_##Nome##_3_pre_alocada.at(a_TSS).addElemento(it1, false); \
			is_var_decisao_##Nome##_3_pre_alocada.at(a_TSS).addElemento(a_it1, true); \
		} \
		if (idx_var_##Nome##_3.at(a_TSS).size() == 0) \
			idx_var_##Nome##_3.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, std::vector<int>>>()); \
		else if (idx_var_##Nome##_3.at(a_TSS).getIteradorFinal() < a_it1){ \
			for (TIt it1 = TIt(idx_var_##Nome##_3.at(a_TSS).getIteradorFinal() + 1); it1 < a_it1; it1++)\
				idx_var_##Nome##_3.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, std::vector<int>>>()); \
			idx_var_##Nome##_3.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, std::vector<int>>>()); \
		} \
		for (TIt2 it2 = a_enupla_alocacao.getIteradorInicial(); it2 <= a_enupla_alocacao.getIteradorFinal(); a_enupla_alocacao.incrementarIterador(it2)){ \
			idx_var_##Nome##_3.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, std::vector<int>>()); \
			for (TIt3 it3 = a_enupla_alocacao.at(it2).getIteradorInicial(); it3 <= a_enupla_alocacao.at(it2).getIteradorFinal(); a_enupla_alocacao.at(it2).incrementarIterador(it3)) \
				idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(it2).addElemento(it3, std::vector<int>()); \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("alocarVarDecisao_" + std::string(#Nome) + "(a_enupla_alocacao): \n" + std::string(erro.what())); } \
}\
std::string getNomeVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3) { \
if (vetorEstagio.att(a_it1).getSolver(a_TSS)->isNomeSimplificado()) \
	return std::string("var" + std::string(#Nome) + "," + getString(a_it1) + "," + getString(a_it2) + "," + getString(a_it3)); \
else \
	return std::string("var" + std::string(#Nome) + "," + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3)); \
}; \
int addVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const double a_valor_inferior, const double a_valor_superior, const double a_valor_objetivo){ \
	try{ \
		const int idVariavelDecisao = vetorEstagio.att(a_it1).getSolver(a_TSS)->addVar(a_valor_inferior, a_valor_superior, a_valor_objetivo, getNomeVarDecisao_##Nome(a_TSS, a_it1, a_it2, a_it3)); \
		bool var_pre_alocada = false; \
		if (is_var_decisao_##Nome##_3_pre_alocada.at(a_TSS).size() > 0){ \
			if ((is_var_decisao_##Nome##_3_pre_alocada.at(a_TSS).getIteradorInicial() <= a_it1) && (a_it1 <= is_var_decisao_##Nome##_3_pre_alocada.at(a_TSS).getIteradorFinal())) \
				var_pre_alocada = is_var_decisao_##Nome##_3_pre_alocada.at(a_TSS).getElemento(a_it1); \
		} \
		if (!var_pre_alocada) { \
			if (idx_var_##Nome##_3.at(a_TSS).size() == 0) \
				idx_var_##Nome##_3.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, std::vector<int>>>()); \
			else if (idx_var_##Nome##_3.at(a_TSS).getIteradorFinal() < a_it1){ \
				for (TIt it1 = TIt(idx_var_##Nome##_3.at(a_TSS).getIteradorFinal() + 1); it1 < a_it1; it1++)\
					idx_var_##Nome##_3.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, std::vector<int>>>()); \
				idx_var_##Nome##_3.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, std::vector<int>>>()); \
			} \
			if (idx_var_##Nome##_3.at(a_TSS).at(a_it1).size() == 0) \
				idx_var_##Nome##_3.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, std::vector<int>>()); \
			else if (idx_var_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2){ \
				for (TIt2 it2 = TIt2(idx_var_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal() + 1); it2 < a_it2; it2++) \
					idx_var_##Nome##_3.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, std::vector<int>>()); \
				idx_var_##Nome##_3.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, std::vector<int>>()); \
			} \
			if (idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, std::vector<int>(1, idVariavelDecisao)); \
			else if (idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3){ \
				for (TIt3 it3 = TIt3(idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() + 1); it3 < a_it3; it3++)\
					idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, std::vector<int>()); \
				idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, std::vector<int>(1, idVariavelDecisao)); \
			} \
			else if (idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0)\
				idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).setElemento(a_it3, std::vector<int>(1, idVariavelDecisao)); \
			else \
				throw std::invalid_argument("Variavel ja existente no modelo."); \
		} \
		else {\
			if (idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0)\
				idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).setElemento(a_it3, std::vector<int>(1, idVariavelDecisao)); \
			else \
				throw std::invalid_argument("Variavel ja existente no modelo."); \
		} \
		if (!is_var_decisao_##Nome##_3_instanciada.at(a_TSS)){ \
			is_var_decisao_##Nome##_3_instanciada.at(a_TSS) = true;\
			lista_variaveis_instanciadas.at(a_TSS).push_back(std::string(std::string(#Nome) + "_3")); \
		} \
		return idVariavelDecisao; \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("addVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getString(a_valor_inferior) + "," + getString(a_valor_superior) + "," + getString(a_valor_objetivo) + "): \n" + std::string(erro.what())); } \
}; \
int getVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3){ \
	try{ return idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(0); } \
	catch (const std::exception& erro) { throw std::invalid_argument("getVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "): \n" + std::string(erro.what())); } \
}; \
int getVarDecisao_##Nome##seExistir(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3){ \
	try{ \
		if (idx_var_##Nome##_3.at(a_TSS).size() == 0) \
			return -1; \
		else if (idx_var_##Nome##_3.at(a_TSS).getIteradorInicial() > a_it1) \
			return -1; \
		else if (idx_var_##Nome##_3.at(a_TSS).getIteradorFinal() < a_it1) \
			return -1; \
		else if (idx_var_##Nome##_3.at(a_TSS).at(a_it1).size() == 0) \
			return -1; \
		else if (idx_var_##Nome##_3.at(a_TSS).at(a_it1).getIteradorInicial() > a_it2) \
			return -1; \
		else if (idx_var_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) \
			return -1; \
		else if (idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
			return -1; \
		else if (idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).getIteradorInicial() > a_it3) \
			return -1; \
		else if (idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) \
			return -1; \
		else if (idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
			return -1; \
		else \
			return idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(0); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("getVarDecisao_" + std::string(#Nome) + "seExistir(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "): \n" + std::string(erro.what())); } \
}; \
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>>>> vlrP_e_c_var_##Nome##_3 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>>>()); \
void inicializarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3){ \
	try{ \
		if (getboolFromChar(#ImprimirPrimal)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrP_e_c_var_##Nome##_3.at(a_TSS).size() == 0) \
				vlrP_e_c_var_##Nome##_3.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>>()); \
			else if (vlrP_e_c_var_##Nome##_3.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrP_e_c_var_##Nome##_3.at(a_TSS).getIteradorFinal();\
				idx_var_##Nome##_3.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_3.at(a_TSS).incrementarIterador(it1))\
					vlrP_e_c_var_##Nome##_3.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>>()); \
				vlrP_e_c_var_##Nome##_3.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (vlrP_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).size() == 0) \
				vlrP_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>()); \
			else if (vlrP_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = vlrP_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_var_##Nome##_3.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_var_##Nome##_3.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					vlrP_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>()); \
				vlrP_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>()); \
			}\
			if (vlrP_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				vlrP_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			else if (vlrP_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = vlrP_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					vlrP_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<IdCenario, double>()); \
				vlrP_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorPrimalPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>>>> vlrD_e_c_var_##Nome##_3 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>>>()); \
void inicializarValorDualPorEstagioPorCenarioVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3){ \
	try{ \
		if (!getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, bool())) \
			return; \
		if (getboolFromChar(#ImprimirDual)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrD_e_c_var_##Nome##_3.at(a_TSS).size() == 0) \
				vlrD_e_c_var_##Nome##_3.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>>()); \
			else if (vlrD_e_c_var_##Nome##_3.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrD_e_c_var_##Nome##_3.at(a_TSS).getIteradorFinal();\
				idx_var_##Nome##_3.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_3.at(a_TSS).incrementarIterador(it1))\
					vlrD_e_c_var_##Nome##_3.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>>()); \
				vlrD_e_c_var_##Nome##_3.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (vlrD_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).size() == 0) \
				vlrD_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>()); \
			else if (vlrD_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = vlrD_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_var_##Nome##_3.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_var_##Nome##_3.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					vlrD_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>()); \
				vlrD_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>()); \
			}\
			if (vlrD_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				vlrD_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			else if (vlrD_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = vlrD_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					vlrD_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<IdCenario, double>()); \
				vlrD_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorDualPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>> vlrP_e_c_r_var_##Nome##_3 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
void inicializarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3){ \
	try{ \
		if (!getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario_por_realizacao, bool())) \
			return; \
		if (getboolFromChar(#ImprimirPrimal)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const IdRealizacao maiorIdRealizacao = getAtributo(a_it1, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrP_e_c_r_var_##Nome##_3.at(a_TSS).size() == 0) \
				vlrP_e_c_r_var_##Nome##_3.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			else if (vlrP_e_c_r_var_##Nome##_3.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrP_e_c_r_var_##Nome##_3.at(a_TSS).getIteradorFinal();\
				idx_var_##Nome##_3.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_3.at(a_TSS).incrementarIterador(it1))\
					vlrP_e_c_r_var_##Nome##_3.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
				vlrP_e_c_r_var_##Nome##_3.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			}\
			if (vlrP_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).size() == 0) \
				vlrP_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			else if (vlrP_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = vlrP_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_var_##Nome##_3.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_var_##Nome##_3.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					vlrP_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
				vlrP_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (vlrP_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				vlrP_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			else if (vlrP_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = vlrP_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					vlrP_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>()); \
				vlrP_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>> vlrD_e_c_r_var_##Nome##_3 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
void inicializarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3){ \
	try{ \
		if (!getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario_por_realizacao, bool())) \
			return; \
		if (getboolFromChar(#ImprimirDual)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const IdRealizacao maiorIdRealizacao = getAtributo(a_it1, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrD_e_c_r_var_##Nome##_3.at(a_TSS).size() == 0) \
				vlrD_e_c_r_var_##Nome##_3.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			else if (vlrD_e_c_r_var_##Nome##_3.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrD_e_c_r_var_##Nome##_3.at(a_TSS).getIteradorFinal();\
				idx_var_##Nome##_3.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_3.at(a_TSS).incrementarIterador(it1))\
					vlrD_e_c_r_var_##Nome##_3.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
				vlrD_e_c_r_var_##Nome##_3.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			}\
			if (vlrD_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).size() == 0) \
				vlrD_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			else if (vlrD_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = vlrD_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_var_##Nome##_3.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_var_##Nome##_3.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					vlrD_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
				vlrD_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (vlrD_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				vlrD_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			else if (vlrD_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = vlrD_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					vlrD_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>()); \
				vlrD_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_3(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario) { \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		if (idx_var_##Nome##_3.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_3.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_3.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_var_##Nome##_3.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_var_##Nome##_3.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_var_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_var_##Nome##_3.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3) > -1) {\
						inicializarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3); \
						vlrP_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getValorPrimal(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3)); \
					}\
				} \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorPrimalPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_3(" + getFullString(a_it1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorDualPorEstagioPorCenarioVarDecisao_##Nome##_3(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario){ \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		if (idx_var_##Nome##_3.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_3.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_3.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_var_##Nome##_3.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_var_##Nome##_3.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_var_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_var_##Nome##_3.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3) > -1) {\
						inicializarValorDualPorEstagioPorCenarioVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3); \
						if (std::string(#Nome) == "VI_$/hm3/h") \
							vlrD_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getReducedCost(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3)) / getNumeroHorasFromIterador(idIterador2); \
						else \
							vlrD_e_c_var_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getReducedCost(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3)); \
					}\
				} \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorDualPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_3(" + getFullString(a_it1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_3(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		if (idx_var_##Nome##_3.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_3.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_3.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_var_##Nome##_3.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_var_##Nome##_3.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_var_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_var_##Nome##_3.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3) > -1) {\
						inicializarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3); \
						vlrP_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(a_idRealizacao).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getValorPrimal(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3)); \
					} \
				}\
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_3(" + getFullString(a_it1) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_3(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		if (idx_var_##Nome##_3.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_3.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_3.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_var_##Nome##_3.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_var_##Nome##_3.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_var_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_var_##Nome##_3.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3) > -1) {\
						inicializarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3); \
						if (std::string(#Nome) == "VI_$/hm3/h") \
							vlrD_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(a_idRealizacao).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getReducedCost(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3)) / getNumeroHorasFromIterador(idIterador2); \
						else \
							vlrD_e_c_r_var_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(a_idRealizacao).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getReducedCost(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3)); \
					} \
				}\
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_3(" + getFullString(a_it1) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
};\
void imprimirValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_3(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados);\
void imprimirValorDualPorEstagioPorCenarioVarDecisao_##Nome##_3(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_3(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_3(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void consolidarResultadosVarDecisao_##Nome##_3(const TipoSubproblemaSolver a_TSS, const std::string a_variavel, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_3(Nome, TIt, TIt2, TIt3, ImprimirPrimal, ImprimirDual) \
void ModeloOtimizacao::imprimirValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_3(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_3"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_3_primal_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrP_e_c_var_##Nome##_3.at(a_TSS))); \
		vlrP_e_c_var_##Nome##_3.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorPrimalPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_3(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\
void ModeloOtimizacao::imprimirValorDualPorEstagioPorCenarioVarDecisao_##Nome##_3(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_3"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_3_dual_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrD_e_c_var_##Nome##_3.at(a_TSS))); \
		vlrD_e_c_var_##Nome##_3.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorDualPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_3(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_3(Nome, TIt, TIt2, TIt3, ImprimirPrimal, ImprimirDual) \
imprimirValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_3(TSS, getFullString(a_idProcesso), a_entradaSaidaDados); \
imprimirValorDualPorEstagioPorCenarioVarDecisao_##Nome##_3(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);  

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_3(Nome, TIt, TIt2, TIt3, ImprimirPrimal, ImprimirDual) \
void ModeloOtimizacao::imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_3(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_3"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_3_primal_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrP_e_c_r_var_##Nome##_3.at(a_TSS))); \
		vlrP_e_c_r_var_##Nome##_3.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_3(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\
void ModeloOtimizacao::imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_3(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_3"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_3_dual_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrD_e_c_r_var_##Nome##_3.at(a_TSS))); \
		vlrD_e_c_r_var_##Nome##_3.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_3(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_3(Nome, TIt, TIt2, TIt3, ImprimirPrimal, ImprimirDual) \
imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_3(TSS, getFullString(a_idProcesso), a_entradaSaidaDados); \
imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_3(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);  \


#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_3(Nome, TIt, TIt2, TIt3, ImprimirPrimal, ImprimirDual) \
armazenarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_3(a_TSS, a_idEstagio, a_idCenario); \
armazenarValorDualPorEstagioPorCenarioVarDecisao_##Nome##_3(a_TSS, a_idEstagio, a_idCenario);

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_3(Nome, TIt, TIt2, TIt3, ImprimirPrimal, ImprimirDual) \
armazenarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_3(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao); \
armazenarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_3(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao); 

#define DECLARAR_CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_3(Nome, TIt, TIt2, TIt3, ImprimirPrimal, ImprimirDual) \
void ModeloOtimizacao::consolidarResultadosVarDecisao_##Nome##_3(const TipoSubproblemaSolver a_TSS, const std::string a_variavel, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (std::string(std::string(#Nome) + "_3") != a_variavel) \
			return; \
		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida(); \
		if (getboolFromChar(#ImprimirPrimal)){ \
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//var" + std::string(#Nome) + "_3"); \
			const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_3_primal" + ".csv"); \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string("var" + std::string(#Nome) + "_3_primal_" + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoHorizontalCSV(nome_arquivo, lista_arquivos, 3, true); \
		} \
		if (getboolFromChar(#ImprimirDual)){ \
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//var" + std::string(#Nome) + "_3"); \
			const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_3_dual" + ".csv"); \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string("var" + std::string(#Nome) + "_3_dual_" + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoHorizontalCSV(nome_arquivo, lista_arquivos, 3, true); \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::consolidarResultadosVarDecisao_" + std::string(#Nome) + "_3(" + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_3(Nome, TIt, TIt2, TIt3, ImprimirPrimal, ImprimirDual) \
consolidarResultadosVarDecisao_##Nome##_3(a_TSS, variavel_str, a_maiorIdProcesso, a_entradaSaidaDados);

// ---------------------------------------------------------------------------------------
//
//                                Variaveis de Decisão Continuas 4
//
// ---------------------------------------------------------------------------------------

#define DECLARAR_VARIAVEL_DECISAO_4(Nome, TIt, TIt2, TIt3, TIt4, ImprimirPrimal, ImprimirDual)   \
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, std::vector<int>>>>>> idx_var_##Nome##_4 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, std::vector<int>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, std::vector<int>>>>>());        \
std::vector<bool> is_var_decisao_##Nome##_4_instanciada = std::vector<bool>(TipoSubproblemaSolver_Excedente, false);  \
std::string getNomeVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4) { \
if (vetorEstagio.att(a_it1).getSolver(a_TSS)->isNomeSimplificado()) \
	return std::string("var" + std::string(#Nome) + "," + getString(a_it1) + "," + getString(a_it2) + "," + getString(a_it3) + "," + getString(a_it4)); \
else \
	return std::string("var" + std::string(#Nome) + "," + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4)); \
}; \
int addVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const double a_valor_inferior, const double a_valor_superior, const double a_valor_objetivo){ \
	try{ \
		const int idVariavelDecisao = vetorEstagio.att(a_it1).getSolver(a_TSS)->addVar(a_valor_inferior, a_valor_superior, a_valor_objetivo, getNomeVarDecisao_##Nome(a_TSS, a_it1, a_it2, a_it3, a_it4)); \
		if (idx_var_##Nome##_4.at(a_TSS).size() == 0) \
			idx_var_##Nome##_4.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, std::vector<int>>>>()); \
		else if (idx_var_##Nome##_4.at(a_TSS).getIteradorFinal() < a_it1){ \
			for (TIt it1 = TIt(idx_var_##Nome##_4.at(a_TSS).getIteradorFinal() + 1); it1 < a_it1; it1++)\
				idx_var_##Nome##_4.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, std::vector<int>>>>()); \
			idx_var_##Nome##_4.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, std::vector<int>>>>()); \
		} \
		if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).size() == 0) \
			idx_var_##Nome##_4.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, std::vector<int>>>()); \
		else if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2){ \
			for (TIt2 it2 = TIt2(idx_var_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal() + 1); it2 < a_it2; it2++) \
				idx_var_##Nome##_4.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, std::vector<int>>>()); \
			idx_var_##Nome##_4.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, std::vector<int>>>()); \
		} \
		if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
			idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, std::vector<int>>()); \
		else if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3){ \
			for (TIt3 it3 = TIt3(idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() + 1); it3 < a_it3; it3++)\
				idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, std::vector<int>>()); \
			idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, std::vector<int>>()); \
		} \
		if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
			idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, std::vector<int>(1, idVariavelDecisao)); \
		else if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4){ \
			for (TIt4 it4 = TIt4(idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() + 1); it4 < a_it4; it4++)\
				idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, std::vector<int>()); \
			idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, std::vector<int>(1, idVariavelDecisao)); \
		} \
		else if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0)\
			idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).setElemento(a_it4, std::vector<int>(1, idVariavelDecisao)); \
		else \
			throw std::invalid_argument("Variavel ja existente no modelo."); \
		if (!is_var_decisao_##Nome##_4_instanciada.at(a_TSS)){ \
			is_var_decisao_##Nome##_4_instanciada.at(a_TSS) = true;\
			lista_variaveis_instanciadas.at(a_TSS).push_back(std::string(std::string(#Nome) + "_4")); \
		} \
		return idVariavelDecisao; \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("addVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getString(a_valor_inferior) + "," + getString(a_valor_superior) + "," + getString(a_valor_objetivo) + "): \n" + std::string(erro.what())); } \
}; \
int getVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4){ \
	try{ return idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(0); } \
	catch (const std::exception& erro) { throw std::invalid_argument("getVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "): \n" + std::string(erro.what())); } \
}; \
int getVarDecisao_##Nome##seExistir(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4){ \
	try{ \
		if (idx_var_##Nome##_4.at(a_TSS).size() == 0) \
			return -1; \
		else if (idx_var_##Nome##_4.at(a_TSS).getIteradorInicial() > a_it1) \
			return -1; \
		else if (idx_var_##Nome##_4.at(a_TSS).getIteradorFinal() < a_it1) \
			return -1; \
		else if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).size() == 0) \
			return -1; \
		else if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).getIteradorInicial() > a_it2) \
			return -1; \
		else if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) \
			return -1; \
		else if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
			return -1; \
		else if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).getIteradorInicial() > a_it3) \
			return -1; \
		else if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) \
			return -1; \
		else if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
			return -1; \
		else if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorInicial() > a_it4) \
			return -1; \
		else if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) \
			return -1; \
		else if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
			return -1; \
		else \
			return idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(0); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("getVarDecisao_" + std::string(#Nome) + "seExistir(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "): \n" + std::string(erro.what())); } \
}; \
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>>>> vlrP_e_c_var_##Nome##_4 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>>>()); \
void inicializarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4){ \
	try{ \
		if (getboolFromChar(#ImprimirPrimal)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrP_e_c_var_##Nome##_4.at(a_TSS).size() == 0) \
				vlrP_e_c_var_##Nome##_4.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>>()); \
			else if (vlrP_e_c_var_##Nome##_4.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrP_e_c_var_##Nome##_4.at(a_TSS).getIteradorFinal();\
				idx_var_##Nome##_4.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_4.at(a_TSS).incrementarIterador(it1))\
					vlrP_e_c_var_##Nome##_4.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>>()); \
				vlrP_e_c_var_##Nome##_4.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>>()); \
			}\
			if (vlrP_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).size() == 0) \
				vlrP_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>()); \
			else if (vlrP_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = vlrP_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_var_##Nome##_4.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_var_##Nome##_4.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					vlrP_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>()); \
				vlrP_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (vlrP_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				vlrP_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>()); \
			else if (vlrP_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = vlrP_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					vlrP_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>()); \
				vlrP_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>()); \
			}\
			if (vlrP_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
				vlrP_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			else if (vlrP_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) { \
				TIt4 it4_inicial = vlrP_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal();\
				idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4_inicial);\
				for (TIt4 it4 = it4_inicial; it4 < a_it4; idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4))\
					vlrP_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<IdCenario, double>()); \
				vlrP_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorPrimalPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>>>> vlrD_e_c_var_##Nome##_4 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>>>()); \
void inicializarValorDualPorEstagioPorCenarioVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4){ \
	try{ \
		if (getboolFromChar(#ImprimirDual)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrD_e_c_var_##Nome##_4.at(a_TSS).size() == 0) \
				vlrD_e_c_var_##Nome##_4.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>>()); \
			else if (vlrD_e_c_var_##Nome##_4.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrD_e_c_var_##Nome##_4.at(a_TSS).getIteradorFinal();\
				idx_var_##Nome##_4.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_4.at(a_TSS).incrementarIterador(it1))\
					vlrD_e_c_var_##Nome##_4.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>>()); \
				vlrD_e_c_var_##Nome##_4.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>>()); \
			}\
			if (vlrD_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).size() == 0) \
				vlrD_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>()); \
			else if (vlrD_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = vlrD_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_var_##Nome##_4.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_var_##Nome##_4.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					vlrD_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>()); \
				vlrD_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (vlrD_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				vlrD_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>()); \
			else if (vlrD_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = vlrD_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					vlrD_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>()); \
				vlrD_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>()); \
			}\
			if (vlrD_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
				vlrD_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			else if (vlrD_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) { \
				TIt4 it4_inicial = vlrD_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal();\
				idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4_inicial);\
				for (TIt4 it4 = it4_inicial; it4 < a_it4; idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4))\
					vlrD_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<IdCenario, double>()); \
				vlrD_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorDualPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>> vlrP_e_c_r_var_##Nome##_4 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
void inicializarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4){ \
	try{ \
		if (getboolFromChar(#ImprimirPrimal)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const IdRealizacao maiorIdRealizacao = getAtributo(a_it1, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrP_e_c_r_var_##Nome##_4.at(a_TSS).size() == 0) \
				vlrP_e_c_r_var_##Nome##_4.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
			else if (vlrP_e_c_r_var_##Nome##_4.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrP_e_c_r_var_##Nome##_4.at(a_TSS).getIteradorFinal();\
				idx_var_##Nome##_4.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_4.at(a_TSS).incrementarIterador(it1))\
					vlrP_e_c_r_var_##Nome##_4.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
				vlrP_e_c_r_var_##Nome##_4.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
			}\
			if (vlrP_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).size() == 0) \
				vlrP_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			else if (vlrP_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = vlrP_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_var_##Nome##_4.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_var_##Nome##_4.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					vlrP_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
				vlrP_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			}\
			if (vlrP_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				vlrP_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			else if (vlrP_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = vlrP_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					vlrP_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
				vlrP_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (vlrP_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
				vlrP_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			else if (vlrP_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) { \
				TIt4 it4_inicial = vlrP_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal();\
				idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4_inicial);\
				for (TIt4 it4 = it4_inicial; it4 < a_it4; idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4))\
					vlrP_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>()); \
				vlrP_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>> vlrD_e_c_r_var_##Nome##_4 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
void inicializarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4){ \
	try{ \
		if (getboolFromChar(#ImprimirDual)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const IdRealizacao maiorIdRealizacao = getAtributo(a_it1, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrD_e_c_r_var_##Nome##_4.at(a_TSS).size() == 0) \
				vlrD_e_c_r_var_##Nome##_4.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
			else if (vlrD_e_c_r_var_##Nome##_4.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrD_e_c_r_var_##Nome##_4.at(a_TSS).getIteradorFinal();\
				idx_var_##Nome##_4.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_4.at(a_TSS).incrementarIterador(it1))\
					vlrD_e_c_r_var_##Nome##_4.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
				vlrD_e_c_r_var_##Nome##_4.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
			}\
			if (vlrD_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).size() == 0) \
				vlrD_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			else if (vlrD_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = vlrD_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_var_##Nome##_4.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_var_##Nome##_4.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					vlrD_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
				vlrD_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			}\
			if (vlrD_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				vlrD_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			else if (vlrD_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = vlrD_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					vlrD_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
				vlrD_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (vlrD_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
				vlrD_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			else if (vlrD_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) { \
				TIt4 it4_inicial = vlrD_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal();\
				idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4_inicial);\
				for (TIt4 it4 = it4_inicial; it4 < a_it4; idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4))\
					vlrD_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>()); \
				vlrD_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_4(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario) { \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return;\
		if (idx_var_##Nome##_4.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_4.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_4.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_var_##Nome##_4.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).size() > 0){ \
						const TIt4 menorIdIterador4 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorInicial(); \
						const TIt4 maiorIdIterador4 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorFinal(); \
						for (TIt4 idIterador4 = menorIdIterador4; idIterador4 <= maiorIdIterador4; idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).incrementarIterador(idIterador4)) { \
							if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3, idIterador4) > -1){ \
								inicializarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4); \
								vlrP_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getValorPrimal(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4)); \
							}\
						}\
					}\
				} \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorPrimalPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_4(" + getFullString(a_it1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorDualPorEstagioPorCenarioVarDecisao_##Nome##_4(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario){ \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return;\
		if (idx_var_##Nome##_4.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_4.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_4.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_var_##Nome##_4.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).size() > 0){ \
						const TIt4 menorIdIterador4 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorInicial(); \
						const TIt4 maiorIdIterador4 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorFinal(); \
						for (TIt4 idIterador4 = menorIdIterador4; idIterador4 <= maiorIdIterador4; idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).incrementarIterador(idIterador4)) { \
							if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3, idIterador4) > -1){ \
								inicializarValorDualPorEstagioPorCenarioVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4); \
								if (std::string(#Nome) == "PL") \
									vlrD_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getReducedCost(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4)) / getNumeroHorasFromIteradores(idIterador2, idIterador3); \
								else \
									vlrD_e_c_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getReducedCost(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4)); \
							}\
						}\
					}\
				} \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorDualPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_4(" + getFullString(a_it1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_4(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return;\
		if (idx_var_##Nome##_4.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_4.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_4.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_var_##Nome##_4.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).size() > 0){ \
						const TIt4 menorIdIterador4 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorInicial(); \
						const TIt4 maiorIdIterador4 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorFinal(); \
						for (TIt4 idIterador4 = menorIdIterador4; idIterador4 <= maiorIdIterador4; idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).incrementarIterador(idIterador4)) { \
							if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3, idIterador4) > -1){ \
								inicializarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4); \
								vlrP_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(a_idRealizacao).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getValorPrimal(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4)); \
							}\
						}\
					}\
				} \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_4(" + getFullString(a_it1) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_4(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return;\
		if (idx_var_##Nome##_4.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_4.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_4.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_var_##Nome##_4.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).size() > 0){ \
						const TIt4 menorIdIterador4 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorInicial(); \
						const TIt4 maiorIdIterador4 = idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorFinal(); \
						for (TIt4 idIterador4 = menorIdIterador4; idIterador4 <= maiorIdIterador4; idx_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).incrementarIterador(idIterador4)) { \
							if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3, idIterador4) > -1){ \
								inicializarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4); \
								if (std::string(#Nome) == "PL") \
									vlrD_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(a_idRealizacao).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getReducedCost(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4)) / getNumeroHorasFromIteradores(idIterador2, idIterador3); \
								else \
									vlrD_e_c_r_var_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(a_idRealizacao).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getReducedCost(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4)); \
							}\
						}\
					}\
				} \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_4(" + getFullString(a_it1) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
};\
void imprimirValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_4(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados);\
void imprimirValorDualPorEstagioPorCenarioVarDecisao_##Nome##_4(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_4(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_4(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void consolidarResultadosVarDecisao_##Nome##_4(const TipoSubproblemaSolver a_TSS, const std::string a_variavel, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_4(Nome, TIt, TIt2, TIt3, TIt4, ImprimirPrimal, ImprimirDual) \
void ModeloOtimizacao::imprimirValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_4(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_4"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_4_primal_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrP_e_c_var_##Nome##_4.at(a_TSS))); \
		vlrP_e_c_var_##Nome##_4.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorPrimalPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_4(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\
void ModeloOtimizacao::imprimirValorDualPorEstagioPorCenarioVarDecisao_##Nome##_4(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_4"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_4_dual_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrD_e_c_var_##Nome##_4.at(a_TSS))); \
		vlrD_e_c_var_##Nome##_4.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorDualPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_4(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_4(Nome, TIt, TIt2, TIt3, TIt4, ImprimirPrimal, ImprimirDual) \
imprimirValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_4(TSS, getFullString(a_idProcesso), a_entradaSaidaDados); \
imprimirValorDualPorEstagioPorCenarioVarDecisao_##Nome##_4(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);  

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_4(Nome, TIt, TIt2, TIt3, TIt4, ImprimirPrimal, ImprimirDual) \
void ModeloOtimizacao::imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_4(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_4"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_4_primal_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrP_e_c_r_var_##Nome##_4.at(a_TSS))); \
		vlrP_e_c_r_var_##Nome##_4.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_4(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\
void ModeloOtimizacao::imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_4(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_4"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_4_dual_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrD_e_c_r_var_##Nome##_4.at(a_TSS))); \
		vlrD_e_c_r_var_##Nome##_4.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_4(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_4(Nome, TIt, TIt2, TIt3, TIt4, ImprimirPrimal, ImprimirDual) \
imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_4(TSS, getFullString(a_idProcesso), a_entradaSaidaDados); \
imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_4(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);  


#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_4(Nome, TIt, TIt2, TIt3, TIt4, ImprimirPrimal, ImprimirDual) \
armazenarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_4(a_TSS, a_idEstagio, a_idCenario); \
armazenarValorDualPorEstagioPorCenarioVarDecisao_##Nome##_4(a_TSS, a_idEstagio, a_idCenario);

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_4(Nome, TIt, TIt2, TIt3, TIt4, ImprimirPrimal, ImprimirDual) \
armazenarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_4(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao); \
armazenarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_4(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao); 

#define DECLARAR_CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_4(Nome, TIt, TIt2, TIt3, TIt4, ImprimirPrimal, ImprimirDual) \
void ModeloOtimizacao::consolidarResultadosVarDecisao_##Nome##_4(const TipoSubproblemaSolver a_TSS, const std::string a_variavel, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (std::string(std::string(#Nome) + "_4") != a_variavel) \
			return; \
		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida(); \
		if (getboolFromChar(#ImprimirPrimal)){ \
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//var" + std::string(#Nome) + "_4"); \
			const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_4_primal" + ".csv"); \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string("var" + std::string(#Nome) + "_4_primal_" + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoHorizontalCSV(nome_arquivo, lista_arquivos, 4, true); \
		} \
		if (getboolFromChar(#ImprimirDual)){ \
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//var" + std::string(#Nome) + "_4"); \
			const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_4_dual" + ".csv"); \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string("var" + std::string(#Nome) + "_4_dual_" + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoHorizontalCSV(nome_arquivo, lista_arquivos, 4, true); \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::consolidarResultadosVarDecisao_" + std::string(#Nome) + "_4(" + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_4(Nome, TIt, TIt2, TIt3, TIt4, ImprimirPrimal, ImprimirDual) \
consolidarResultadosVarDecisao_##Nome##_4(a_TSS, variavel_str, a_maiorIdProcesso, a_entradaSaidaDados);

// ---------------------------------------------------------------------------------------
//
//                                Variaveis de Decisão Continuas 5
//
// ---------------------------------------------------------------------------------------

#define DECLARAR_VARIAVEL_DECISAO_5(Nome, TIt, TIt2, TIt3, TIt4,TIt5, ImprimirPrimal, ImprimirDual)   \
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>>>>> idx_var_##Nome##_5 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>>>>());        \
std::vector<bool> is_var_decisao_##Nome##_5_instanciada = std::vector<bool>(TipoSubproblemaSolver_Excedente, false);  \
std::string getNomeVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5) { \
if (vetorEstagio.att(a_it1).getSolver(a_TSS)->isNomeSimplificado()) \
	return std::string("var" + std::string(#Nome) + "," + getString(a_it1) + "," + getString(a_it2) + "," + getString(a_it3) + "," + getString(a_it4) + "," + getString(a_it5)); \
else \
	return std::string("var" + std::string(#Nome) + "," + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5)); \
}; \
int addVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const double a_valor_inferior, const double a_valor_superior, const double a_valor_objetivo){ \
	try{ \
		const int idVariavelDecisao = vetorEstagio.att(a_it1).getSolver(a_TSS)->addVar(a_valor_inferior, a_valor_superior, a_valor_objetivo, getNomeVarDecisao_##Nome(a_TSS, a_it1, a_it2, a_it3, a_it4, a_it5)); \
		if (idx_var_##Nome##_5.at(a_TSS).size() == 0) \
			idx_var_##Nome##_5.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>>>()); \
		else if (idx_var_##Nome##_5.at(a_TSS).getIteradorFinal() < a_it1){ \
			for (TIt it1 = TIt(idx_var_##Nome##_5.at(a_TSS).getIteradorFinal() + 1); it1 < a_it1; it1++)\
				idx_var_##Nome##_5.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>>>()); \
			idx_var_##Nome##_5.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>>>()); \
		} \
		if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).size() == 0) \
			idx_var_##Nome##_5.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>>()); \
		else if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2){ \
			for (TIt2 it2 = TIt2(idx_var_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal() + 1); it2 < a_it2; it2++) \
				idx_var_##Nome##_5.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>>()); \
			idx_var_##Nome##_5.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>>()); \
		} \
		if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
			idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>()); \
		else if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3){ \
			for (TIt3 it3 = TIt3(idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() + 1); it3 < a_it3; it3++)\
				idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>()); \
			idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>()); \
		} \
		if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
			idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, std::vector<int>>()); \
		else if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4){ \
			for (TIt4 it4 = TIt4(idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() + 1); it4 < a_it4; it4++)\
				idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<TIt5, std::vector<int>>()); \
			idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, std::vector<int>>()); \
		} \
		if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
			idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, std::vector<int>(1, idVariavelDecisao)); \
		else if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5){ \
			for (TIt5 it5 =TIt5(idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() + 1); it5 < a_it5; it5++)\
				idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(it5, std::vector<int>()); \
			idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, std::vector<int>(1, idVariavelDecisao)); \
		} \
		else if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).size() == 0)\
			idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).setElemento(a_it5, std::vector<int>(1, idVariavelDecisao)); \
		else \
			throw std::invalid_argument("Variavel ja existente no modelo."); \
		if (!is_var_decisao_##Nome##_5_instanciada.at(a_TSS)){ \
			is_var_decisao_##Nome##_5_instanciada.at(a_TSS) = true;\
			lista_variaveis_instanciadas.at(a_TSS).push_back(std::string(std::string(#Nome) + "_5")); \
		} \
		return idVariavelDecisao; \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("addVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "," + getString(a_valor_inferior) + "," + getString(a_valor_superior) + "," + getString(a_valor_objetivo) + "): \n" + std::string(erro.what())); } \
}; \
int getVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5){ \
	try{ return idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(0); } \
	catch (const std::exception& erro) { throw std::invalid_argument("getVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "): \n" + std::string(erro.what())); } \
}; \
int getVarDecisao_##Nome##seExistir(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5){ \
	try{ \
		if (idx_var_##Nome##_5.at(a_TSS).size() == 0) \
			return -1; \
		else if (idx_var_##Nome##_5.at(a_TSS).getIteradorInicial() > a_it1) \
			return -1; \
		else if (idx_var_##Nome##_5.at(a_TSS).getIteradorFinal() < a_it1) \
			return -1; \
		else if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).size() == 0) \
			return -1; \
		else if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).getIteradorInicial() > a_it2) \
			return -1; \
		else if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) \
			return -1; \
		else if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
			return -1; \
		else if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).getIteradorInicial() > a_it3) \
			return -1; \
		else if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) \
			return -1; \
		else if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
			return -1; \
		else if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorInicial() > a_it4) \
			return -1; \
		else if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) \
			return -1; \
		else if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
			return -1; \
		else if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorInicial() > a_it5) \
			return -1; \
		else if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5) \
			return -1; \
		else if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).size() == 0) \
			return -1; \
		else \
			return idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(0); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("getVarDecisao_" + std::string(#Nome) + "seExistir(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "): \n" + std::string(erro.what())); } \
}; \
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>>>>> vlrP_e_c_var_##Nome##_5 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>>>>()); \
void inicializarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5){ \
	try{ \
		if (getboolFromChar(#ImprimirPrimal)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrP_e_c_var_##Nome##_5.at(a_TSS).size() == 0) \
				vlrP_e_c_var_##Nome##_5.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>>>()); \
			else if (vlrP_e_c_var_##Nome##_5.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrP_e_c_var_##Nome##_5.at(a_TSS).getIteradorFinal();\
				idx_var_##Nome##_5.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_5.at(a_TSS).incrementarIterador(it1))\
					vlrP_e_c_var_##Nome##_5.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>>>()); \
				vlrP_e_c_var_##Nome##_5.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>>>()); \
			}\
			if (vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).size() == 0) \
				vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>>()); \
			else if (vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_var_##Nome##_5.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_var_##Nome##_5.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>>()); \
				vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>>()); \
			}\
			if (vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>()); \
			else if (vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>()); \
				vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
				vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>()); \
			else if (vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) { \
				TIt4 it4_inicial = vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal();\
				idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4_inicial);\
				for (TIt4 it4 = it4_inicial; it4 < a_it4; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4))\
					vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>()); \
				vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>()); \
			}\
			if (vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
				vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			else if (vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5) { \
				TIt5 it5_inicial = vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal();\
				idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5_inicial);\
				for (TIt5 it5 = it5_inicial; it5 < a_it5; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5))\
					vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(it5, SmartEnupla<IdCenario, double>()); \
				vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorPrimalPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>>>>> vlrD_e_c_var_##Nome##_5 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>>>>()); \
void inicializarValorDualPorEstagioPorCenarioVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5){ \
	try{ \
		if (getboolFromChar(#ImprimirDual)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrD_e_c_var_##Nome##_5.at(a_TSS).size() == 0) \
				vlrD_e_c_var_##Nome##_5.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>>>()); \
			else if (vlrD_e_c_var_##Nome##_5.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrD_e_c_var_##Nome##_5.at(a_TSS).getIteradorFinal();\
				idx_var_##Nome##_5.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_5.at(a_TSS).incrementarIterador(it1))\
					vlrD_e_c_var_##Nome##_5.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>>>()); \
				vlrD_e_c_var_##Nome##_5.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>>>()); \
			}\
			if (vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).size() == 0) \
				vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>>()); \
			else if (vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_var_##Nome##_5.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_var_##Nome##_5.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>>()); \
				vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>>()); \
			}\
			if (vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>()); \
			else if (vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>()); \
				vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
				vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>()); \
			else if (vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) { \
				TIt4 it4_inicial = vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal();\
				idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4_inicial);\
				for (TIt4 it4 = it4_inicial; it4 < a_it4; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4))\
					vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>()); \
				vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>()); \
			}\
			if (vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
				vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			else if (vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5) { \
				TIt5 it5_inicial = vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal();\
				idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5_inicial);\
				for (TIt5 it5 = it5_inicial; it5 < a_it5; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5))\
					vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(it5, SmartEnupla<IdCenario, double>()); \
				vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorDualPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>> vlrP_e_c_r_var_##Nome##_5 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>()); \
void inicializarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5){ \
	try{ \
		if (getboolFromChar(#ImprimirPrimal)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const IdRealizacao maiorIdRealizacao = getAtributo(a_it1, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrP_e_c_r_var_##Nome##_5.at(a_TSS).size() == 0) \
				vlrP_e_c_r_var_##Nome##_5.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
			else if (vlrP_e_c_r_var_##Nome##_5.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrP_e_c_r_var_##Nome##_5.at(a_TSS).getIteradorFinal();\
				idx_var_##Nome##_5.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_5.at(a_TSS).incrementarIterador(it1))\
					vlrP_e_c_r_var_##Nome##_5.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
				vlrP_e_c_r_var_##Nome##_5.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
			}\
			if (vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).size() == 0) \
				vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
			else if (vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_var_##Nome##_5.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_var_##Nome##_5.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
				vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
			}\
			if (vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			else if (vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
				vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			}\
			if (vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
				vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			else if (vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) { \
				TIt4 it4_inicial = vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal();\
				idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4_inicial);\
				for (TIt4 it4 = it4_inicial; it4 < a_it4; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4))\
					vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
				vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
				vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			else if (vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5) { \
				TIt5 it5_inicial = vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal();\
				idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5_inicial);\
				for (TIt5 it5 = it5_inicial; it5 < a_it5; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5))\
					vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(it5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>()); \
				vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>> vlrD_e_c_r_var_##Nome##_5 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>()); \
void inicializarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5){ \
	try{ \
		if (!getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario_por_realizacao, bool())) \
			return; \
		if (getboolFromChar(#ImprimirDual)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const IdRealizacao maiorIdRealizacao = getAtributo(a_it1, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrD_e_c_r_var_##Nome##_5.at(a_TSS).size() == 0) \
				vlrD_e_c_r_var_##Nome##_5.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
			else if (vlrD_e_c_r_var_##Nome##_5.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrD_e_c_r_var_##Nome##_5.at(a_TSS).getIteradorFinal();\
				idx_var_##Nome##_5.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_5.at(a_TSS).incrementarIterador(it1))\
					vlrD_e_c_r_var_##Nome##_5.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
				vlrD_e_c_r_var_##Nome##_5.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
			}\
			if (vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).size() == 0) \
				vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
			else if (vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_var_##Nome##_5.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_var_##Nome##_5.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
				vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
			}\
			if (vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			else if (vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
				vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			}\
			if (vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
				vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			else if (vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) { \
				TIt4 it4_inicial = vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal();\
				idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4_inicial);\
				for (TIt4 it4 = it4_inicial; it4 < a_it4; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4))\
					vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
				vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
				vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			else if (vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5) { \
				TIt5 it5_inicial = vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal();\
				idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5_inicial);\
				for (TIt5 it5 = it5_inicial; it5 < a_it5; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5))\
					vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(it5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>()); \
				vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_5(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario) { \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		if (idx_var_##Nome##_5.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_5.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_5.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_var_##Nome##_5.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).size() > 0){ \
						const TIt4 menorIdIterador4 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorInicial(); \
						const TIt4 maiorIdIterador4 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorFinal(); \
						for (TIt4 idIterador4 = menorIdIterador4; idIterador4 <= maiorIdIterador4; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).incrementarIterador(idIterador4)) { \
							if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).size() > 0){ \
								const TIt5 menorIdIterador5 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorInicial(); \
								const TIt5 maiorIdIterador5 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorFinal(); \
								for (TIt5 idIterador5 = menorIdIterador5; idIterador5 <= maiorIdIterador5; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).incrementarIterador(idIterador5)) { \
									if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5) > -1){ \
										inicializarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5); \
										vlrP_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getValorPrimal(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5)); \
									}\
								}\
							}\
						}\
					}\
				} \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorPrimalPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_5(" + getFullString(a_it1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorDualPorEstagioPorCenarioVarDecisao_##Nome##_5(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario){ \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		if (idx_var_##Nome##_5.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_5.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_5.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_var_##Nome##_5.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).size() > 0){ \
						const TIt4 menorIdIterador4 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorInicial(); \
						const TIt4 maiorIdIterador4 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorFinal(); \
						for (TIt4 idIterador4 = menorIdIterador4; idIterador4 <= maiorIdIterador4; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).incrementarIterador(idIterador4)) { \
							if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).size() > 0){ \
								const TIt5 menorIdIterador5 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorInicial(); \
								const TIt5 maiorIdIterador5 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorFinal(); \
								for (TIt5 idIterador5 = menorIdIterador5; idIterador5 <= maiorIdIterador5; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).incrementarIterador(idIterador5)) { \
									if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5) > -1){ \
										inicializarValorDualPorEstagioPorCenarioVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5); \
										vlrD_e_c_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getReducedCost(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5)); \
									}\
								}\
							}\
						}\
					}\
				} \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorDualPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_5(" + getFullString(a_it1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_5(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		if (idx_var_##Nome##_5.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_5.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_5.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_var_##Nome##_5.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).size() > 0){ \
						const TIt4 menorIdIterador4 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorInicial(); \
						const TIt4 maiorIdIterador4 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorFinal(); \
						for (TIt4 idIterador4 = menorIdIterador4; idIterador4 <= maiorIdIterador4; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).incrementarIterador(idIterador4)) { \
							if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).size() > 0){ \
								const TIt5 menorIdIterador5 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorInicial(); \
								const TIt5 maiorIdIterador5 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorFinal(); \
								for (TIt5 idIterador5 = menorIdIterador5; idIterador5 <= maiorIdIterador5; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).incrementarIterador(idIterador5)) { \
									if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5) > -1){ \
										inicializarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5); \
										vlrP_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).at(a_idRealizacao).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getValorPrimal(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5)); \
									}\
								}\
							}\
						}\
					}\
				} \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_5(" + getFullString(a_it1) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_5(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		if (idx_var_##Nome##_5.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_5.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_5.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_var_##Nome##_5.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).size() > 0){ \
						const TIt4 menorIdIterador4 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorInicial(); \
						const TIt4 maiorIdIterador4 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorFinal(); \
						for (TIt4 idIterador4 = menorIdIterador4; idIterador4 <= maiorIdIterador4; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).incrementarIterador(idIterador4)) { \
							if (idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).size() > 0){ \
								const TIt5 menorIdIterador5 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorInicial(); \
								const TIt5 maiorIdIterador5 = idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorFinal(); \
								for (TIt5 idIterador5 = menorIdIterador5; idIterador5 <= maiorIdIterador5; idx_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).incrementarIterador(idIterador5)) { \
									if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5) > -1){ \
										inicializarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5); \
										vlrD_e_c_r_var_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).at(a_idRealizacao).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getReducedCost(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5)); \
									}\
								}\
							}\
						}\
					}\
				} \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_5(" + getFullString(a_it1) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
};\
void imprimirValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_5(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados);\
void imprimirValorDualPorEstagioPorCenarioVarDecisao_##Nome##_5(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_5(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_5(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void consolidarResultadosVarDecisao_##Nome##_5(const TipoSubproblemaSolver a_TSS, const std::string a_variavel, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_5(Nome, TIt, TIt2, TIt3, TIt4,TIt5, ImprimirPrimal, ImprimirDual) \
void ModeloOtimizacao::imprimirValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_5(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_5"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_5_primal_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrP_e_c_var_##Nome##_5.at(a_TSS))); \
		vlrP_e_c_var_##Nome##_5.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorPrimalPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_5(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\
void ModeloOtimizacao::imprimirValorDualPorEstagioPorCenarioVarDecisao_##Nome##_5(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_5"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_5_dual_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrD_e_c_var_##Nome##_5.at(a_TSS))); \
		vlrD_e_c_var_##Nome##_5.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdCenario, double>>>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorDualPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_5(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_5(Nome, TIt, TIt2, TIt3, TIt4,TIt5, ImprimirPrimal, ImprimirDual) \
imprimirValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_5(TSS, getFullString(a_idProcesso), a_entradaSaidaDados); \
imprimirValorDualPorEstagioPorCenarioVarDecisao_##Nome##_5(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);  \

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_5(Nome, TIt, TIt2, TIt3, TIt4,TIt5, ImprimirPrimal, ImprimirDual) \
void ModeloOtimizacao::imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_5(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_5"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_5_primal_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrP_e_c_r_var_##Nome##_5.at(a_TSS))); \
		vlrP_e_c_r_var_##Nome##_5.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_5(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\
void ModeloOtimizacao::imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_5(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_5"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_5_dual_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrD_e_c_r_var_##Nome##_5.at(a_TSS))); \
		vlrD_e_c_r_var_##Nome##_5.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_5(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_5(Nome, TIt, TIt2, TIt3, TIt4,TIt5, ImprimirPrimal, ImprimirDual) \
imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_5(TSS, getFullString(a_idProcesso), a_entradaSaidaDados); \
imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_5(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);  \

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_5(Nome, TIt, TIt2, TIt3, TIt4,TIt5, ImprimirPrimal, ImprimirDual) \
armazenarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_5(a_TSS, a_idEstagio, a_idCenario); \
armazenarValorDualPorEstagioPorCenarioVarDecisao_##Nome##_5(a_TSS, a_idEstagio, a_idCenario);

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_5(Nome, TIt, TIt2, TIt3, TIt4,TIt5, ImprimirPrimal, ImprimirDual) \
armazenarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_5(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao); \
armazenarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_5(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao); 

#define DECLARAR_CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_5(Nome, TIt, TIt2, TIt3, TIt4,TIt5, ImprimirPrimal, ImprimirDual) \
void ModeloOtimizacao::consolidarResultadosVarDecisao_##Nome##_5(const TipoSubproblemaSolver a_TSS, const std::string a_variavel, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (std::string(std::string(#Nome) + "_5") != a_variavel) \
			return; \
		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida(); \
		if (getboolFromChar(#ImprimirPrimal)){ \
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//var" + std::string(#Nome) + "_5"); \
			const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_5_primal" + ".csv"); \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string("var" + std::string(#Nome) + "_5_primal_" + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoHorizontalCSV(nome_arquivo, lista_arquivos, 5, true); \
		} \
		if (getboolFromChar(#ImprimirDual)){ \
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//var" + std::string(#Nome) + "_5"); \
			const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_5_dual" + ".csv"); \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string("var" + std::string(#Nome) + "_5_dual_" + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoHorizontalCSV(nome_arquivo, lista_arquivos, 5, true); \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::consolidarResultadosVarDecisao_" + std::string(#Nome) + "_5(" + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_5(Nome, TIt, TIt2, TIt3, TIt4,TIt5, ImprimirPrimal, ImprimirDual) \
consolidarResultadosVarDecisao_##Nome##_5(a_TSS, variavel_str, a_maiorIdProcesso, a_entradaSaidaDados);

// ---------------------------------------------------------------------------------------
//
//                                Variaveis de Decisão Continuas 6
//
// ---------------------------------------------------------------------------------------

#define DECLARAR_VARIAVEL_DECISAO_6(Nome, TIt, TIt2, TIt3, TIt4,TIt5,TIt6, ImprimirPrimal, ImprimirDual)   \
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>>>>> idx_var_##Nome##_6 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>>>>());        \
std::vector<bool> is_var_decisao_##Nome##_6_instanciada = std::vector<bool>(TipoSubproblemaSolver_Excedente, false);  \
std::string getNomeVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6) { \
if (vetorEstagio.att(a_it1).getSolver(a_TSS)->isNomeSimplificado()) \
	return std::string("var" + std::string(#Nome) + "," + getString(a_it1) + "," + getString(a_it2) + "," + getString(a_it3) + "," + getString(a_it4) + "," + getString(a_it5) + "," + getString(a_it6) );\
else \
	return std::string("var" + std::string(#Nome) + "," + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "," + getFullString(a_it6) );\
}; \
int addVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6, const double a_valor_inferior, const double a_valor_superior, const double a_valor_objetivo){ \
	try{ \
		const int idVariavelDecisao = vetorEstagio.att(a_it1).getSolver(a_TSS)->addVar(a_valor_inferior, a_valor_superior, a_valor_objetivo, getNomeVarDecisao_##Nome(a_TSS, a_it1, a_it2, a_it3, a_it4, a_it5, a_it6)); \
		if (idx_var_##Nome##_6.at(a_TSS).size() == 0) \
			idx_var_##Nome##_6.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>>>()); \
		else if (idx_var_##Nome##_6.at(a_TSS).getIteradorFinal() < a_it1){ \
			for (TIt it1 = TIt(idx_var_##Nome##_6.at(a_TSS).getIteradorFinal() + 1); it1 < a_it1; it1++)\
				idx_var_##Nome##_6.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>>>()); \
			idx_var_##Nome##_6.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>>>()); \
		} \
		if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).size() == 0) \
			idx_var_##Nome##_6.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>>()); \
		else if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2){ \
			for (TIt2 it2 = TIt2(idx_var_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal() + 1); it2 < a_it2; it2++) \
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>>()); \
			idx_var_##Nome##_6.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>>()); \
		} \
		if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
			idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>()); \
		else if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3){ \
			for (TIt3 it3 = TIt3(idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() + 1); it3 < a_it3; it3++)\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>()); \
			idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>()); \
		} \
		if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
			idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>()); \
		else if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4){ \
			for (TIt4 it4 = TIt4(idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() + 1); it4 < a_it4; it4++)\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>()); \
			idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>()); \
		} \
		if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
			idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<TIt6, std::vector<int>>()); \
		else if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5){ \
			for (TIt5 it5 =TIt5(idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() + 1); it5 < a_it5; it5++)\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(it5, SmartEnupla<TIt6, std::vector<int>>()); \
			idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<TIt6, std::vector<int>>()); \
		} \
		if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).size() == 0) \
			idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(a_it6, std::vector<int>(1, idVariavelDecisao)); \
		else if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal() < a_it6){ \
			for (TIt6 it6 =TIt6(idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal() + 1); it6 < a_it6; it6++)\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(it6, std::vector<int>()); \
			idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(a_it6, std::vector<int>(1, idVariavelDecisao)); \
		} \
		else if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).size() == 0)\
			idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).setElemento(a_it6, std::vector<int>(1, idVariavelDecisao)); \
		else \
			throw std::invalid_argument("Variavel ja existente no modelo."); \
		if (!is_var_decisao_##Nome##_6_instanciada.at(a_TSS)){ \
			is_var_decisao_##Nome##_6_instanciada.at(a_TSS) = true;\
			lista_variaveis_instanciadas.at(a_TSS).push_back(std::string(std::string(#Nome) + "_6")); \
		} \
		return idVariavelDecisao; \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("addVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "," + getFullString(a_it6)  + "," + getString(a_valor_inferior) + "," + getString(a_valor_superior) + "," + getString(a_valor_objetivo) + "): \n" + std::string(erro.what())); } \
}; \
int getVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6){ \
	try{ return idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).at(0); } \
	catch (const std::exception& erro) { throw std::invalid_argument("getVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "," + getFullString(a_it6)  + "): \n" + std::string(erro.what())); } \
}; \
int getVarDecisao_##Nome##seExistir(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6){ \
	try{ \
		if (idx_var_##Nome##_6.at(a_TSS).size() == 0) \
			return -1; \
		else if (idx_var_##Nome##_6.at(a_TSS).getIteradorInicial() > a_it1) \
			return -1; \
		else if (idx_var_##Nome##_6.at(a_TSS).getIteradorFinal() < a_it1) \
			return -1; \
		else if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).size() == 0) \
			return -1; \
		else if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).getIteradorInicial() > a_it2) \
			return -1; \
		else if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) \
			return -1; \
		else if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
			return -1; \
		else if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).getIteradorInicial() > a_it3) \
			return -1; \
		else if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) \
			return -1; \
		else if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
			return -1; \
		else if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorInicial() > a_it4) \
			return -1; \
		else if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) \
			return -1; \
		else if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
			return -1; \
		else if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorInicial() > a_it5) \
			return -1; \
		else if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5) \
			return -1; \
		else if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).size() == 0) \
			return -1; \
		else if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorInicial() > a_it6) \
			return -1; \
		else if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal() < a_it6) \
			return -1; \
		else if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).size() == 0) \
			return -1; \
		else \
			return idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).at(0); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("getVarDecisao_" + std::string(#Nome) + "seExistir(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "," + getFullString(a_it6)  + "): \n" + std::string(erro.what())); } \
}; \
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>>>> vlrP_e_c_var_##Nome##_6 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>>>()); \
void inicializarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6){ \
	try{ \
		if (getboolFromChar(#ImprimirPrimal)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrP_e_c_var_##Nome##_6.at(a_TSS).size() == 0) \
				vlrP_e_c_var_##Nome##_6.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>>()); \
			else if (vlrP_e_c_var_##Nome##_6.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrP_e_c_var_##Nome##_6.at(a_TSS).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_6.at(a_TSS).incrementarIterador(it1))\
					vlrP_e_c_var_##Nome##_6.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>>()); \
				vlrP_e_c_var_##Nome##_6.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>>()); \
			}\
			if (vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).size() == 0) \
				vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>()); \
			else if (vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_var_##Nome##_6.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>()); \
				vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>()); \
			}\
			if (vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>()); \
			else if (vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>()); \
				vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>()); \
			}\
			if (vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
				vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>()); \
			else if (vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) { \
				TIt4 it4_inicial = vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4_inicial);\
				for (TIt4 it4 = it4_inicial; it4 < a_it4; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4))\
					vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>()); \
				vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
				vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>()); \
			else if (vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5) { \
				TIt5 it5_inicial = vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5_inicial);\
				for (TIt5 it5 = it5_inicial; it5 < a_it5; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5))\
					vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(it5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>()); \
				vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>()); \
			}\
			if (vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).size() == 0) \
				vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(a_it6, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			else if (vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal() < a_it6) { \
				TIt6 it6_inicial = vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).incrementarIterador(it6_inicial);\
				for (TIt6 it6 = it6_inicial; it6 < a_it6; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).incrementarIterador(it6))\
					vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(it6, SmartEnupla<IdCenario, double>()); \
				vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(a_it6, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorPrimalPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "," + getFullString(a_it6)  + "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>>>> vlrD_e_c_var_##Nome##_6 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>>>()); \
void inicializarValorDualPorEstagioPorCenarioVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6){ \
	try{ \
		if (getboolFromChar(#ImprimirDual)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrD_e_c_var_##Nome##_6.at(a_TSS).size() == 0) \
				vlrD_e_c_var_##Nome##_6.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>>()); \
			else if (vlrD_e_c_var_##Nome##_6.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrD_e_c_var_##Nome##_6.at(a_TSS).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_6.at(a_TSS).incrementarIterador(it1))\
					vlrD_e_c_var_##Nome##_6.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>>()); \
				vlrD_e_c_var_##Nome##_6.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>>()); \
			}\
			if (vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).size() == 0) \
				vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>()); \
			else if (vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_var_##Nome##_6.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>()); \
				vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>()); \
			}\
			if (vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>()); \
			else if (vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>()); \
				vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>()); \
			}\
			if (vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
				vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>()); \
			else if (vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) { \
				TIt4 it4_inicial = vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4_inicial);\
				for (TIt4 it4 = it4_inicial; it4 < a_it4; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4))\
					vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>()); \
				vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
				vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>()); \
			else if (vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5) { \
				TIt5 it5_inicial = vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5_inicial);\
				for (TIt5 it5 = it5_inicial; it5 < a_it5; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5))\
					vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(it5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>()); \
				vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>()); \
			}\
			if (vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).size() == 0) \
				vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(a_it6, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			else if (vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal() < a_it6) { \
				TIt6 it6_inicial = vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).incrementarIterador(it6_inicial);\
				for (TIt6 it6 = it6_inicial; it6 < a_it6; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).incrementarIterador(it6))\
					vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(it6, SmartEnupla<IdCenario, double>()); \
				vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(a_it6, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorDualPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "," + getFullString(a_it6)  + "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>> vlrP_e_c_r_var_##Nome##_6 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>()); \
void inicializarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6){ \
	try{ \
		if (getboolFromChar(#ImprimirPrimal)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const IdRealizacao maiorIdRealizacao = getAtributo(a_it1, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrP_e_c_r_var_##Nome##_6.at(a_TSS).size() == 0) \
				vlrP_e_c_r_var_##Nome##_6.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>()); \
			else if (vlrP_e_c_r_var_##Nome##_6.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrP_e_c_r_var_##Nome##_6.at(a_TSS).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_6.at(a_TSS).incrementarIterador(it1))\
					vlrP_e_c_r_var_##Nome##_6.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>()); \
				vlrP_e_c_r_var_##Nome##_6.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>()); \
			}\
			if (vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).size() == 0) \
				vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
			else if (vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_var_##Nome##_6.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
				vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
			}\
			if (vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
			else if (vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
				vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
			}\
			if (vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
				vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			else if (vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) { \
				TIt4 it4_inicial = vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4_inicial);\
				for (TIt4 it4 = it4_inicial; it4 < a_it4; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4))\
					vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
				vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			}\
			if (vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
				vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			else if (vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5) { \
				TIt5 it5_inicial = vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5_inicial);\
				for (TIt5 it5 = it5_inicial; it5 < a_it5; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5))\
					vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(it5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
				vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).size() == 0) \
				vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(a_it6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			else if (vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal() < a_it6) { \
				TIt6 it6_inicial = vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).incrementarIterador(it6_inicial);\
				for (TIt6 it6 = it6_inicial; it6 < a_it6; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).incrementarIterador(it6))\
					vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(it6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>()); \
				vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(a_it6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "," + getFullString(a_it6)  + "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>> vlrD_e_c_r_var_##Nome##_6 = std::vector<SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>()); \
void inicializarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6){ \
	try{ \
		if (getboolFromChar(#ImprimirDual)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const IdRealizacao maiorIdRealizacao = getAtributo(a_it1, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (vlrD_e_c_r_var_##Nome##_6.at(a_TSS).size() == 0) \
				vlrD_e_c_r_var_##Nome##_6.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>()); \
			else if (vlrD_e_c_r_var_##Nome##_6.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt it1_inicial = vlrD_e_c_r_var_##Nome##_6.at(a_TSS).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt it1 = it1_inicial; it1 < a_it1; idx_var_##Nome##_6.at(a_TSS).incrementarIterador(it1))\
					vlrD_e_c_r_var_##Nome##_6.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>()); \
				vlrD_e_c_r_var_##Nome##_6.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>()); \
			}\
			if (vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).size() == 0) \
				vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
			else if (vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_var_##Nome##_6.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
				vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
			}\
			if (vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
			else if (vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
				vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
			}\
			if (vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
				vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			else if (vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) { \
				TIt4 it4_inicial = vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4_inicial);\
				for (TIt4 it4 = it4_inicial; it4 < a_it4; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4))\
					vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
				vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			}\
			if (vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
				vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			else if (vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5) { \
				TIt5 it5_inicial = vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5_inicial);\
				for (TIt5 it5 = it5_inicial; it5 < a_it5; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5))\
					vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(it5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
				vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).size() == 0) \
				vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(a_it6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			else if (vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal() < a_it6) { \
				TIt6 it6_inicial = vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal();\
				idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).incrementarIterador(it6_inicial);\
				for (TIt6 it6 = it6_inicial; it6 < a_it6; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).incrementarIterador(it6))\
					vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(it6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>()); \
				vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(a_it6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "," + getFullString(a_it6)  + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_6(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario) { \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		if (idx_var_##Nome##_6.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_6.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_6.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_var_##Nome##_6.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).size() > 0){ \
						const TIt4 menorIdIterador4 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorInicial(); \
						const TIt4 maiorIdIterador4 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorFinal(); \
						for (TIt4 idIterador4 = menorIdIterador4; idIterador4 <= maiorIdIterador4; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).incrementarIterador(idIterador4)) { \
							if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).size() > 0){ \
								const TIt5 menorIdIterador5 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorInicial(); \
								const TIt5 maiorIdIterador5 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorFinal(); \
								for (TIt5 idIterador5 = menorIdIterador5; idIterador5 <= maiorIdIterador5; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).incrementarIterador(idIterador5)) { \
									if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).size() > 0){ \
										const TIt6 menorIdIterador6 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).getIteradorInicial(); \
										const TIt6 maiorIdIterador6 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).getIteradorFinal(); \
										for (TIt6 idIterador6 = menorIdIterador6; idIterador6 <= maiorIdIterador6; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).incrementarIterador(idIterador6)) { \
											if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6) > -1) {\
												inicializarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6); \
												vlrP_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).at(idIterador6).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getValorPrimal(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6)); \
											}\
										}\
									}\
								}\
							}\
						}\
					}\
				} \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorPrimalPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_6(" + getFullString(a_it1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorDualPorEstagioPorCenarioVarDecisao_##Nome##_6(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario){ \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		if (idx_var_##Nome##_6.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_6.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_6.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_var_##Nome##_6.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).size() > 0){ \
						const TIt4 menorIdIterador4 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorInicial(); \
						const TIt4 maiorIdIterador4 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorFinal(); \
						for (TIt4 idIterador4 = menorIdIterador4; idIterador4 <= maiorIdIterador4; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).incrementarIterador(idIterador4)) { \
							if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).size() > 0){ \
								const TIt5 menorIdIterador5 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorInicial(); \
								const TIt5 maiorIdIterador5 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorFinal(); \
								for (TIt5 idIterador5 = menorIdIterador5; idIterador5 <= maiorIdIterador5; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).incrementarIterador(idIterador5)) { \
									if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).size() > 0){ \
										const TIt6 menorIdIterador6 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).getIteradorInicial(); \
										const TIt6 maiorIdIterador6 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).getIteradorFinal(); \
										for (TIt6 idIterador6 = menorIdIterador6; idIterador6 <= maiorIdIterador6; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).incrementarIterador(idIterador6)) { \
											if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6) > -1) {\
												inicializarValorDualPorEstagioPorCenarioVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6); \
												vlrD_e_c_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).at(idIterador6).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getReducedCost(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6)); \
											}\
										}\
									}\
								}\
							}\
						}\
					}\
				} \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorDualPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_6(" + getFullString(a_it1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_6(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		if (idx_var_##Nome##_6.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_6.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_6.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_var_##Nome##_6.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).size() > 0){ \
						const TIt4 menorIdIterador4 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorInicial(); \
						const TIt4 maiorIdIterador4 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorFinal(); \
						for (TIt4 idIterador4 = menorIdIterador4; idIterador4 <= maiorIdIterador4; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).incrementarIterador(idIterador4)) { \
							if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).size() > 0){ \
								const TIt5 menorIdIterador5 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorInicial(); \
								const TIt5 maiorIdIterador5 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorFinal(); \
								for (TIt5 idIterador5 = menorIdIterador5; idIterador5 <= maiorIdIterador5; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).incrementarIterador(idIterador5)) { \
									if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).size() > 0){ \
										const TIt6 menorIdIterador6 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).getIteradorInicial(); \
										const TIt6 maiorIdIterador6 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).getIteradorFinal(); \
										for (TIt6 idIterador6 = menorIdIterador6; idIterador6 <= maiorIdIterador6; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).incrementarIterador(idIterador6)) { \
											if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6) > -1) {\
												inicializarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6); \
												vlrP_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).at(idIterador6).at(a_idRealizacao).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getValorPrimal(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6)); \
											}\
										}\
									}\
								}\
							}\
						}\
					}\
				} \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_6(" + getFullString(a_it1) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_6(const TipoSubproblemaSolver a_TSS, const TIt a_it1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		if (idx_var_##Nome##_6.at(a_TSS).size() == 0) \
			return; \
		if (idx_var_##Nome##_6.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_var_##Nome##_6.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_var_##Nome##_6.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).size() > 0){ \
						const TIt4 menorIdIterador4 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorInicial(); \
						const TIt4 maiorIdIterador4 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorFinal(); \
						for (TIt4 idIterador4 = menorIdIterador4; idIterador4 <= maiorIdIterador4; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).incrementarIterador(idIterador4)) { \
							if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).size() > 0){ \
								const TIt5 menorIdIterador5 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorInicial(); \
								const TIt5 maiorIdIterador5 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorFinal(); \
								for (TIt5 idIterador5 = menorIdIterador5; idIterador5 <= maiorIdIterador5; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).incrementarIterador(idIterador5)) { \
									if (idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).size() > 0){ \
										const TIt6 menorIdIterador6 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).getIteradorInicial(); \
										const TIt6 maiorIdIterador6 = idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).getIteradorFinal(); \
										for (TIt6 idIterador6 = menorIdIterador6; idIterador6 <= maiorIdIterador6; idx_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).incrementarIterador(idIterador6)) { \
											if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6) > -1) {\
												inicializarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6); \
												vlrD_e_c_r_var_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).at(idIterador6).at(a_idRealizacao).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getReducedCost(getVarDecisao_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6)); \
											}\
										}\
									}\
								}\
							}\
						}\
					}\
				} \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_6(" + getFullString(a_it1) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
};\
void imprimirValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_6(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados);\
void imprimirValorDualPorEstagioPorCenarioVarDecisao_##Nome##_6(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_6(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_6(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void consolidarResultadosVarDecisao_##Nome##_6(const TipoSubproblemaSolver a_TSS, const std::string a_variavel, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_6(Nome, TIt, TIt2, TIt3, TIt4,TIt5,TIt6, ImprimirPrimal, ImprimirDual) \
void ModeloOtimizacao::imprimirValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_6(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_6"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_6_primal_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrP_e_c_var_##Nome##_6.at(a_TSS))); \
		vlrP_e_c_var_##Nome##_6.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorPrimalPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_6(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\
void ModeloOtimizacao::imprimirValorDualPorEstagioPorCenarioVarDecisao_##Nome##_6(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_6"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_6_dual_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrD_e_c_var_##Nome##_6.at(a_TSS))); \
		vlrD_e_c_var_##Nome##_6.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorDualPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_6(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_6(Nome, TIt, TIt2, TIt3, TIt4,TIt5,TIt6, ImprimirPrimal, ImprimirDual) \
imprimirValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_6(TSS, getFullString(a_idProcesso), a_entradaSaidaDados); \
imprimirValorDualPorEstagioPorCenarioVarDecisao_##Nome##_6(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);  

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_6(Nome, TIt, TIt2, TIt3, TIt4,TIt5,TIt6, ImprimirPrimal, ImprimirDual) \
void ModeloOtimizacao::imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_6(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_6"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_6_primal_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrP_e_c_r_var_##Nome##_6.at(a_TSS))); \
		vlrP_e_c_r_var_##Nome##_6.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_6(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\
void ModeloOtimizacao::imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_6(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_6"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_6_dual_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrD_e_c_r_var_##Nome##_6.at(a_TSS))); \
		vlrD_e_c_r_var_##Nome##_6.at(a_TSS) = SmartEnupla<TIt, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_6(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_6(Nome, TIt, TIt2, TIt3, TIt4,TIt5,TIt6, ImprimirPrimal, ImprimirDual) \
imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_6(TSS, getFullString(a_idProcesso), a_entradaSaidaDados); \
imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_6(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);  

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_6(Nome, TIt, TIt2, TIt3, TIt4,TIt5,TIt6, ImprimirPrimal, ImprimirDual) \
armazenarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_6(a_TSS, a_idEstagio, a_idCenario); \
armazenarValorDualPorEstagioPorCenarioVarDecisao_##Nome##_6(a_TSS, a_idEstagio, a_idCenario);

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_6(Nome, TIt, TIt2, TIt3, TIt4,TIt5,TIt6, ImprimirPrimal, ImprimirDual) \
armazenarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_6(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao); \
armazenarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_6(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao); 

#define DECLARAR_CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_6(Nome, TIt, TIt2, TIt3, TIt4,TIt5,TIt6, ImprimirPrimal, ImprimirDual) \
void ModeloOtimizacao::consolidarResultadosVarDecisao_##Nome##_6(const TipoSubproblemaSolver a_TSS, const std::string a_variavel, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (std::string(std::string(#Nome) + "_6") != a_variavel) \
			return; \
		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida(); \
		if (getboolFromChar(#ImprimirPrimal)){ \
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//var" + std::string(#Nome) + "_6"); \
			const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_6_primal" + ".csv"); \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string("var" + std::string(#Nome) + "_6_primal_" + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoHorizontalCSV(nome_arquivo, lista_arquivos, 6, true); \
		} \
		if (getboolFromChar(#ImprimirDual)){ \
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//var" + std::string(#Nome) + "_6"); \
			const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_6_dual" + ".csv"); \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string("var" + std::string(#Nome) + "_6_dual_" + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoHorizontalCSV(nome_arquivo, lista_arquivos, 6, true); \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::consolidarResultadosVarDecisao_" + std::string(#Nome) + "_6(" + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_6(Nome, TIt, TIt2, TIt3, TIt4,TIt5,TIt6, ImprimirPrimal, ImprimirDual) \
consolidarResultadosVarDecisao_##Nome##_6(a_TSS, variavel_str, a_maiorIdProcesso, a_entradaSaidaDados);

#endif 
