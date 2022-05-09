#ifndef INEQUACAO_LINEAR
#define INEQUACAO_LINEAR


// ---------------------------------------------------------------------------------------
//
//Restrições Ineações Lineares 1
//
// ---------------------------------------------------------------------------------------

#define DECLARAR_RESTRICAO_INEQUACAO_LINEAR_1(Nome, TIt1, ImprimirMultiplicador) \
std::vector<SmartEnupla<TIt1, std::vector<int>>> idx_ine_##Nome##_1 = std::vector<SmartEnupla<TIt1, std::vector<int>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt1, std::vector<int>>());\
std::vector<bool> is_ine_linear_##Nome##_1_instanciada = std::vector<bool>(TipoSubproblemaSolver_Excedente, false);\
std::string getNomeIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1) { \
if (vetorEstagio.att(a_it1).getSolver(a_TSS)->isNomeSimplificado()) \
	return std::string("ine" + std::string(#Nome) + "," + getString(a_it1) );\
else \
	return std::string("ine" + std::string(#Nome) + "," + getFullString(a_it1) );\
}; \
int addIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1){ \
	try{ \
		const int idInequacaoLinear = vetorEstagio.att(a_it1).getSolver(a_TSS)->addConstrMaior(getNomeIneLinear_##Nome(a_TSS, a_it1)); \
		if (idx_ine_##Nome##_1.at(a_TSS).size() == 0) \
			idx_ine_##Nome##_1.at(a_TSS).addElemento(a_it1, std::vector<int>(1, idInequacaoLinear)); \
		else if (idx_ine_##Nome##_1.at(a_TSS).getIteradorFinal() < a_it1){ \
			for (TIt1 it1 = TIt1(idx_ine_##Nome##_1.at(a_TSS).getIteradorFinal() + 1); it1 < a_it1; it1++)\
				idx_ine_##Nome##_1.at(a_TSS).addElemento(it1, std::vector<int>()); \
			idx_ine_##Nome##_1.at(a_TSS).addElemento(a_it1, sstd::vector<int>(1, idInequacaoLinear)); \
		} \
		else if (idx_ine_##Nome##_1.at(a_TSS).at(a_it1).size() == 0)\
			idx_ine_##Nome##_1.at(a_TSS).setElemento(a_it1, std::vector<int>(1, idInequacaoLinear)); \
		else \
			throw std::invalid_argument("Inequacao Linear ja existente no modelo."); \
		if (!is_ine_linear_##Nome##_1_instanciada.at(a_TSS)){ \
			is_ine_linear_##Nome##_1_instanciada.at(a_TSS) = true;\
			lista_inequacoes_instanciadas.at(a_TSS).push_back(std::string(std::string(#Nome) + "_1")); \
		} \
		return idInequacaoLinear; \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("addIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "): \n" + std::string(erro.what())); } \
}; \
int getIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1){ \
	try{ return idx_ine_##Nome##_1.at(a_TSS).at(a_it1).at(0); } \
	catch (const std::exception& erro) { throw std::invalid_argument("getIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1)+ "): \n" + std::string(erro.what())); } \
}; \
int getIneLinear_##Nome##seExistir(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1){ \
	try{ \
		if (idx_ine_##Nome##_1.at(a_TSS).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_1.at(a_TSS).getIteradorInicial() > a_it1) \
			return -1; \
		else if (idx_ine_##Nome##_1.at(a_TSS).getIteradorFinal() < a_it1) \
			return -1; \
		else if (idx_ine_##Nome##_1.at(a_TSS).at(a_it1).size() == 0) \
			return -1; \
		else \
			return idx_ine_##Nome##_1.at(a_TSS).at(a_it1).at(0); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("getIneLinear_" + std::string(#Nome) + "seExistir(" + getFullString(a_it1)+ "): \n" + std::string(erro.what())); } \
}; \
std::vector<SmartEnupla<TIt1, SmartEnupla<IdCenario, double>>> mltp_e_c_ine_##Nome##_1 = std::vector<SmartEnupla<TIt1, SmartEnupla<IdCenario, double>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt1, SmartEnupla<IdCenario, double>>()); \
void inicializarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1){ \
	try{ \
		if (getboolFromChar(#ImprimirMultiplicador)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (mltp_e_c_ine_##Nome##_1.at(a_TSS).size() == 0) \
				mltp_e_c_ine_##Nome##_1.at(a_TSS).addElemento(a_it1, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			else if (mltp_e_c_ine_##Nome##_1.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt1 it1_inicial = mltp_e_c_ine_##Nome##_1.at(a_TSS).getIteradorFinal();\
				idx_ine_##Nome##_1.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt1 it1 = it1_inicial; it1 < a_it1; idx_ine_##Nome##_1.at(a_TSS).incrementarIterador(it1))\
					mltp_e_c_ine_##Nome##_1.at(a_TSS).addElemento(it1, SmartEnupla<IdCenario, double>()); \
				mltp_e_c_ine_##Nome##_1.at(a_TSS).addElemento(a_it1, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarMultiplicadorPorEstagioPorCenarioIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1)+ "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt1, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>> mltp_e_c_r_ine_##Nome##_1 = std::vector<SmartEnupla<TIt1, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt1, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
void inicializarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1){ \
	try{ \
		if (getboolFromChar(#ImprimirMultiplicador)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const IdRealizacao maiorIdRealizacao = getAtributo(a_it1, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (mltp_e_c_r_ine_##Nome##_1.at(a_TSS).size() == 0) \
				mltp_e_c_r_ine_##Nome##_1.at(a_TSS).addElemento(a_it1, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			else if (mltp_e_c_r_ine_##Nome##_1.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt1 it1_inicial = mltp_e_c_r_ine_##Nome##_1.at(a_TSS).getIteradorFinal();\
				idx_ine_##Nome##_1.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt1 it1 = it1_inicial; it1 < a_it1; idx_ine_##Nome##_1.at(a_TSS).incrementarIterador(it1))\
					mltp_e_c_r_ine_##Nome##_1.at(a_TSS).addElemento(it1, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>()); \
				mltp_e_c_r_ine_##Nome##_1.at(a_TSS).addElemento(a_it1, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1)+ "): \n" + std::string(erro.what())); } \
};\
void armazenarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_1(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const IdCenario a_idCenario){ \
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		if (getIneLinear_##Nome##seExistir(a_TSS, a_it1) > -1) {\
			inicializarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome(a_TSS, a_it1); \
			mltp_e_c_ine_##Nome##_1.at(a_TSS).at(a_it1).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getMultiplicador(getIneLinear_##Nome(a_TSS, a_it1)); \
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarMultiplicadorPorEstagioPorCenarioIneLinear_" + std::string(#Nome) + "_1(" + getFullString(a_it1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
};\
void armazenarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_1(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		if (getIneLinear_##Nome##seExistir(a_TSS, a_it1) > -1) {\
			inicializarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome(a_TSS, a_it1); \
			mltp_e_c_r_ine_##Nome##_1.at(a_TSS).at(a_it1).at(a_idRealizacao).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getMultiplicador(getIneLinear_##Nome(a_TSS, a_it1)); \
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_" + std::string(#Nome) + "_1(" + getFullString(a_it1) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
};\
void imprimirMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_1(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_1(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void consolidarResultadosIneLinear_##Nome##_1(const TipoSubproblemaSolver a_TSS, const std::string a_restricao, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_1(Nome, TIt1, ImprimirMultiplicador) \
void ModeloOtimizacao::imprimirMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_1(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//ine" + std::string(#Nome) + "_1"); \
		const std::string nome_arquivo = std::string("ine" + std::string(#Nome) + "_1_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, mltp_e_c_ine_##Nome##_1.at(a_TSS))); \
		mltp_e_c_ine_##Nome##_1.at(a_TSS) = SmartEnupla<TIt1, SmartEnupla<IdCenario, double>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirMultiplicadorPorEstagioPorCenarioIneLinear_" + std::string(#Nome) + "_1(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_1(Nome, TIt1, ImprimirMultiplicador) \
imprimirMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_1(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_1(Nome, TIt1, ImprimirMultiplicador) \
void ModeloOtimizacao::imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_1(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//ine" + std::string(#Nome) + "_1"); \
		const std::string nome_arquivo = std::string("ine" + std::string(#Nome) + "_1_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, mltp_e_c_r_ine_##Nome##_1.at(a_TSS))); \
		mltp_e_c_r_ine_##Nome##_1.at(a_TSS) = SmartEnupla<TIt1, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_" + std::string(#Nome) + "_1(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_1(Nome, TIt1, ImprimirMultiplicador) \
imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_1(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_1(Nome, TIt1, ImprimirMultiplicador) \
armazenarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_1(a_TSS, a_idEstagio, a_idCenario);

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_1(Nome, TIt1, ImprimirMultiplicador) \
armazenarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_1(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao); 

#define DECLARAR_CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_1(Nome, TIt1, ImprimirMultiplicador) \
void ModeloOtimizacao::consolidarResultadosIneLinear_##Nome##_1(const TipoSubproblemaSolver a_TSS, const std::string a_restricao, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (std::string(std::string(#Nome) + "_1") != a_restricao) \
			return; \
		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida(); \
		if (getboolFromChar(#ImprimirMultiplicador)){ \
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//ine" + std::string(#Nome) + "_1"); \
			const std::string nome_arquivo = std::string("ine" + std::string(#Nome) + "_1" + ".csv"); \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string("ine" + std::string(#Nome) + "_1_" + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoHorizontalCSV(nome_arquivo, lista_arquivos, 1, true); \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::consolidarResultadosIneLinear_" + std::string(#Nome) + "_1(" + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_1(Nome, TIt1, ImprimirMultiplicador) \
consolidarResultadosIneLinear_##Nome##_1(a_TSS, restricao_str, a_maiorIdProcesso, a_entradaSaidaDados);




// ---------------------------------------------------------------------------------------
//
//Restrições Ineações Lineares 2
//
// ---------------------------------------------------------------------------------------

#define DECLARAR_RESTRICAO_INEQUACAO_LINEAR_2(Nome, TIt1, TIt2, ImprimirMultiplicador) \
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, std::vector<int>>>> idx_ine_##Nome##_2 = std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, std::vector<int>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt1, SmartEnupla<TIt2, std::vector<int>>>());\
std::vector<bool> is_ine_linear_##Nome##_2_instanciada = std::vector<bool>(TipoSubproblemaSolver_Excedente, false);\
std::string getNomeIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2) { \
if (vetorEstagio.att(a_it1).getSolver(a_TSS)->isNomeSimplificado()) \
	return std::string("ine" + std::string(#Nome) + "," + getString(a_it1) + "," + getString(a_it2) );\
else \
	return std::string("ine" + std::string(#Nome) + "," + getFullString(a_it1) + "," + getFullString(a_it2) );\
}; \
int addIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2){ \
	try{ \
		const int idInequacaoLinear = vetorEstagio.att(a_it1).getSolver(a_TSS)->addConstrMaior(getNomeIneLinear_##Nome(a_TSS, a_it1, a_it2)); \
		if (idx_ine_##Nome##_2.at(a_TSS).size() == 0) \
			idx_ine_##Nome##_2.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, std::vector<int>>()); \
		else if (idx_ine_##Nome##_2.at(a_TSS).getIteradorFinal() < a_it1){ \
			for (TIt1 it1 = TIt1(idx_ine_##Nome##_2.at(a_TSS).getIteradorFinal() + 1); it1 < a_it1; it1++)\
				idx_ine_##Nome##_2.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, std::vector<int>>()); \
			idx_ine_##Nome##_2.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, std::vector<int>>()); \
		} \
		if (idx_ine_##Nome##_2.at(a_TSS).at(a_it1).size() == 0) \
			idx_ine_##Nome##_2.at(a_TSS).at(a_it1).addElemento(a_it2, std::vector<int>(1, idInequacaoLinear)); \
		else if (idx_ine_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2){ \
			for (TIt2 it2 = TIt2(idx_ine_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal() + 1); it2 < a_it2; it2++) \
				idx_ine_##Nome##_2.at(a_TSS).at(a_it1).addElemento(it2, std::vector<int>()); \
			idx_ine_##Nome##_2.at(a_TSS).at(a_it1).addElemento(a_it2, std::vector<int>(1, idInequacaoLinear)); \
		} \
		else if (idx_ine_##Nome##_2.at(a_TSS).at(a_it1).at(a_it2).size() == 0)\
			idx_ine_##Nome##_2.at(a_TSS).at(a_it1).setElemento(a_it2, std::vector<int>(1, idInequacaoLinear)); \
		else \
			throw std::invalid_argument("Inequacao Linear ja existente no modelo."); \
		if (!is_ine_linear_##Nome##_2_instanciada.at(a_TSS)){ \
			is_ine_linear_##Nome##_2_instanciada.at(a_TSS) = true;\
			lista_inequacoes_instanciadas.at(a_TSS).push_back(std::string(std::string(#Nome) + "_2")); \
		} \
		return idInequacaoLinear; \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("addIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "): \n" + std::string(erro.what())); } \
}; \
int getIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2){ \
	try{ return idx_ine_##Nome##_2.at(a_TSS).at(a_it1).at(a_it2).at(0); } \
	catch (const std::exception& erro) { throw std::invalid_argument("getIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2)+ "): \n" + std::string(erro.what())); } \
}; \
int getIneLinear_##Nome##seExistir(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2){ \
	try{ \
		if (idx_ine_##Nome##_2.at(a_TSS).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_2.at(a_TSS).getIteradorInicial() > a_it1) \
			return -1; \
		else if (idx_ine_##Nome##_2.at(a_TSS).getIteradorFinal() < a_it1) \
			return -1; \
		else if (idx_ine_##Nome##_2.at(a_TSS).at(a_it1).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_2.at(a_TSS).at(a_it1).getIteradorInicial() > a_it2) \
			return -1; \
		else if (idx_ine_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) \
			return -1; \
		else if (idx_ine_##Nome##_2.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
			return -1; \
		else \
			return idx_ine_##Nome##_2.at(a_TSS).at(a_it1).at(a_it2).at(0); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("getIneLinear_" + std::string(#Nome) + "seExistir(" + getFullString(a_it1) + "," + getFullString(a_it2)+ "): \n" + std::string(erro.what())); } \
}; \
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<IdCenario, double>>>> mltp_e_c_ine_##Nome##_2 = std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<IdCenario, double>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<IdCenario, double>>>()); \
void inicializarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2){ \
	try{ \
		if (getboolFromChar(#ImprimirMultiplicador)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (mltp_e_c_ine_##Nome##_2.at(a_TSS).size() == 0) \
				mltp_e_c_ine_##Nome##_2.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<IdCenario, double>>()); \
			else if (mltp_e_c_ine_##Nome##_2.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt1 it1_inicial = mltp_e_c_ine_##Nome##_2.at(a_TSS).getIteradorFinal();\
				idx_ine_##Nome##_2.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt1 it1 = it1_inicial; it1 < a_it1; idx_ine_##Nome##_2.at(a_TSS).incrementarIterador(it1))\
					mltp_e_c_ine_##Nome##_2.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<IdCenario, double>>()); \
				mltp_e_c_ine_##Nome##_2.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<IdCenario, double>>()); \
			}\
			if (mltp_e_c_ine_##Nome##_2.at(a_TSS).at(a_it1).size() == 0) \
				mltp_e_c_ine_##Nome##_2.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			else if (mltp_e_c_ine_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = mltp_e_c_ine_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_ine_##Nome##_2.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_ine_##Nome##_2.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					mltp_e_c_ine_##Nome##_2.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<IdCenario, double>()); \
				mltp_e_c_ine_##Nome##_2.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarMultiplicadorPorEstagioPorCenarioIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2)+ "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>> mltp_e_c_r_ine_##Nome##_2 = std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
void inicializarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2){ \
	try{ \
		if (getboolFromChar(#ImprimirMultiplicador)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const IdRealizacao maiorIdRealizacao = getAtributo(a_it1, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (mltp_e_c_r_ine_##Nome##_2.at(a_TSS).size() == 0) \
				mltp_e_c_r_ine_##Nome##_2.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			else if (mltp_e_c_r_ine_##Nome##_2.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt1 it1_inicial = mltp_e_c_r_ine_##Nome##_2.at(a_TSS).getIteradorFinal();\
				idx_ine_##Nome##_2.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt1 it1 = it1_inicial; it1 < a_it1; idx_ine_##Nome##_2.at(a_TSS).incrementarIterador(it1))\
					mltp_e_c_r_ine_##Nome##_2.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
				mltp_e_c_r_ine_##Nome##_2.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (mltp_e_c_r_ine_##Nome##_2.at(a_TSS).at(a_it1).size() == 0) \
				mltp_e_c_r_ine_##Nome##_2.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			else if (mltp_e_c_r_ine_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = mltp_e_c_r_ine_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_ine_##Nome##_2.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_ine_##Nome##_2.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					mltp_e_c_r_ine_##Nome##_2.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>()); \
				mltp_e_c_r_ine_##Nome##_2.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2)+ "): \n" + std::string(erro.what())); } \
};\
void armazenarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_2(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const IdCenario a_idCenario){ \
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		if (idx_ine_##Nome##_2.at(a_TSS).size() == 0) \
			return; \
		if (idx_ine_##Nome##_2.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_ine_##Nome##_2.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_ine_##Nome##_2.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_ine_##Nome##_2.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_ine_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_ine_##Nome##_2.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (getIneLinear_##Nome##seExistir(a_TSS, a_it1, idIterador2) > -1) {\
				inicializarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome(a_TSS, a_it1, idIterador2); \
				mltp_e_c_ine_##Nome##_2.at(a_TSS).at(a_it1).at(idIterador2).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getMultiplicador(getIneLinear_##Nome(a_TSS, a_it1, idIterador2)); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarMultiplicadorPorEstagioPorCenarioIneLinear_" + std::string(#Nome) + "_2(" + getFullString(a_it1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
};\
void armazenarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_2(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		if (idx_ine_##Nome##_2.at(a_TSS).size() == 0) \
			return; \
		if (idx_ine_##Nome##_2.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_ine_##Nome##_2.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_ine_##Nome##_2.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_ine_##Nome##_2.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_ine_##Nome##_2.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_ine_##Nome##_2.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (getIneLinear_##Nome##seExistir(a_TSS, a_it1, idIterador2) > -1) {\
				inicializarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome(a_TSS, a_it1, idIterador2); \
				mltp_e_c_r_ine_##Nome##_2.at(a_TSS).at(a_it1).at(idIterador2).at(a_idRealizacao).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getMultiplicador(getIneLinear_##Nome(a_TSS, a_it1, idIterador2)); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_" + std::string(#Nome) + "_2(" + getFullString(a_it1) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
};\
void imprimirMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_2(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_2(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void consolidarResultadosIneLinear_##Nome##_2(const TipoSubproblemaSolver a_TSS, const std::string a_restricao, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_2(Nome, TIt1, TIt2, ImprimirMultiplicador) \
void ModeloOtimizacao::imprimirMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_2(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//ine" + std::string(#Nome) + "_2"); \
		const std::string nome_arquivo = std::string("ine" + std::string(#Nome) + "_2_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, mltp_e_c_ine_##Nome##_2.at(a_TSS))); \
		mltp_e_c_ine_##Nome##_2.at(a_TSS) = SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<IdCenario, double>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirMultiplicadorPorEstagioPorCenarioIneLinear_" + std::string(#Nome) + "_2(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_2(Nome, TIt1, TIt2, ImprimirMultiplicador) \
imprimirMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_2(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_2(Nome, TIt1, TIt2, ImprimirMultiplicador) \
void ModeloOtimizacao::imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_2(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//ine" + std::string(#Nome) + "_2"); \
		const std::string nome_arquivo = std::string("ine" + std::string(#Nome) + "_2_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, mltp_e_c_r_ine_##Nome##_2.at(a_TSS))); \
		mltp_e_c_r_ine_##Nome##_2.at(a_TSS) = SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_" + std::string(#Nome) + "_2(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_2(Nome, TIt1, TIt2, ImprimirMultiplicador) \
imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_2(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_2(Nome, TIt1, TIt2, ImprimirMultiplicador) \
armazenarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_2(a_TSS, a_idEstagio, a_idCenario);

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_2(Nome, TIt1, TIt2, ImprimirMultiplicador) \
armazenarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_2(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao); 

#define DECLARAR_CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_2(Nome, TIt1, TIt2, ImprimirMultiplicador) \
void ModeloOtimizacao::consolidarResultadosIneLinear_##Nome##_2(const TipoSubproblemaSolver a_TSS, const std::string a_restricao, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (std::string(std::string(#Nome) + "_2") != a_restricao) \
			return; \
		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida(); \
		if (getboolFromChar(#ImprimirMultiplicador)){ \
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//ine" + std::string(#Nome) + "_2"); \
			const std::string nome_arquivo = std::string("ine" + std::string(#Nome) + "_2" + ".csv"); \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string("ine" + std::string(#Nome) + "_2_" + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoHorizontalCSV(nome_arquivo, lista_arquivos, 2, true); \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::consolidarResultadosIneLinear_" + std::string(#Nome) + "_2(" + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_2(Nome, TIt1, TIt2, ImprimirMultiplicador) \
consolidarResultadosIneLinear_##Nome##_2(a_TSS, restricao_str, a_maiorIdProcesso, a_entradaSaidaDados);




// ---------------------------------------------------------------------------------------
//
//Restrições Ineações Lineares 3
//
// ---------------------------------------------------------------------------------------

#define DECLARAR_RESTRICAO_INEQUACAO_LINEAR_3(Nome, TIt1, TIt2, TIt3, ImprimirMultiplicador) \
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, std::vector<int>>>>> idx_ine_##Nome##_3 = std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, std::vector<int>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, std::vector<int>>>>());\
std::vector<bool> is_ine_linear_##Nome##_3_instanciada = std::vector<bool>(TipoSubproblemaSolver_Excedente, false);\
std::string getNomeIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3) { \
if (vetorEstagio.att(a_it1).getSolver(a_TSS)->isNomeSimplificado()) \
	return std::string("ine" + std::string(#Nome) + "," + getString(a_it1) + "," + getString(a_it2) + "," + getString(a_it3) );\
else \
	return std::string("ine" + std::string(#Nome) + "," + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) );\
}; \
int addIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3){ \
	try{ \
		const int idInequacaoLinear = vetorEstagio.att(a_it1).getSolver(a_TSS)->addConstrMaior(getNomeIneLinear_##Nome(a_TSS, a_it1, a_it2, a_it3)); \
		if (idx_ine_##Nome##_3.at(a_TSS).size() == 0) \
			idx_ine_##Nome##_3.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, std::vector<int>>>()); \
		else if (idx_ine_##Nome##_3.at(a_TSS).getIteradorFinal() < a_it1){ \
			for (TIt1 it1 = TIt1(idx_ine_##Nome##_3.at(a_TSS).getIteradorFinal() + 1); it1 < a_it1; it1++)\
				idx_ine_##Nome##_3.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, std::vector<int>>>()); \
			idx_ine_##Nome##_3.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, std::vector<int>>>()); \
		} \
		if (idx_ine_##Nome##_3.at(a_TSS).at(a_it1).size() == 0) \
			idx_ine_##Nome##_3.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, std::vector<int>>()); \
		else if (idx_ine_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2){ \
			for (TIt2 it2 = TIt2(idx_ine_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal() + 1); it2 < a_it2; it2++) \
				idx_ine_##Nome##_3.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, std::vector<int>>()); \
			idx_ine_##Nome##_3.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, std::vector<int>>()); \
		} \
		if (idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
			idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, std::vector<int>(1, idInequacaoLinear)); \
		else if (idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3){ \
			for (TIt3 it3 = TIt3(idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() + 1); it3 < a_it3; it3++)\
				idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, std::vector<int>()); \
			idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, std::vector<int>(1, idInequacaoLinear)); \
		} \
		else if (idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0)\
			idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).setElemento(a_it3, std::vector<int>(1, idInequacaoLinear)); \
		else \
			throw std::invalid_argument("Inequacao Linear ja existente no modelo."); \
		if (!is_ine_linear_##Nome##_3_instanciada.at(a_TSS)){ \
			is_ine_linear_##Nome##_3_instanciada.at(a_TSS) = true;\
			lista_inequacoes_instanciadas.at(a_TSS).push_back(std::string(std::string(#Nome) + "_3")); \
		} \
		return idInequacaoLinear; \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("addIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "): \n" + std::string(erro.what())); } \
}; \
int getIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3){ \
	try{ return idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(0); } \
	catch (const std::exception& erro) { throw std::invalid_argument("getIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3)+ "): \n" + std::string(erro.what())); } \
}; \
int getIneLinear_##Nome##seExistir(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3){ \
	try{ \
		if (idx_ine_##Nome##_3.at(a_TSS).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_3.at(a_TSS).getIteradorInicial() > a_it1) \
			return -1; \
		else if (idx_ine_##Nome##_3.at(a_TSS).getIteradorFinal() < a_it1) \
			return -1; \
		else if (idx_ine_##Nome##_3.at(a_TSS).at(a_it1).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_3.at(a_TSS).at(a_it1).getIteradorInicial() > a_it2) \
			return -1; \
		else if (idx_ine_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) \
			return -1; \
		else if (idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).getIteradorInicial() > a_it3) \
			return -1; \
		else if (idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) \
			return -1; \
		else if (idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
			return -1; \
		else \
			return idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(0); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("getIneLinear_" + std::string(#Nome) + "seExistir(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3)+ "): \n" + std::string(erro.what())); } \
}; \
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>>>> mltp_e_c_ine_##Nome##_3 = std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>>>()); \
void inicializarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3){ \
	try{ \
		if (getboolFromChar(#ImprimirMultiplicador)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (mltp_e_c_ine_##Nome##_3.at(a_TSS).size() == 0) \
				mltp_e_c_ine_##Nome##_3.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>>()); \
			else if (mltp_e_c_ine_##Nome##_3.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt1 it1_inicial = mltp_e_c_ine_##Nome##_3.at(a_TSS).getIteradorFinal();\
				idx_ine_##Nome##_3.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt1 it1 = it1_inicial; it1 < a_it1; idx_ine_##Nome##_3.at(a_TSS).incrementarIterador(it1))\
					mltp_e_c_ine_##Nome##_3.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>>()); \
				mltp_e_c_ine_##Nome##_3.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (mltp_e_c_ine_##Nome##_3.at(a_TSS).at(a_it1).size() == 0) \
				mltp_e_c_ine_##Nome##_3.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>()); \
			else if (mltp_e_c_ine_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = mltp_e_c_ine_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_ine_##Nome##_3.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_ine_##Nome##_3.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					mltp_e_c_ine_##Nome##_3.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>()); \
				mltp_e_c_ine_##Nome##_3.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>()); \
			}\
			if (mltp_e_c_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				mltp_e_c_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			else if (mltp_e_c_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = mltp_e_c_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					mltp_e_c_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<IdCenario, double>()); \
				mltp_e_c_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarMultiplicadorPorEstagioPorCenarioIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3)+ "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>> mltp_e_c_r_ine_##Nome##_3 = std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
void inicializarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3){ \
	try{ \
		if (getboolFromChar(#ImprimirMultiplicador)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const IdRealizacao maiorIdRealizacao = getAtributo(a_it1, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (mltp_e_c_r_ine_##Nome##_3.at(a_TSS).size() == 0) \
				mltp_e_c_r_ine_##Nome##_3.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			else if (mltp_e_c_r_ine_##Nome##_3.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt1 it1_inicial = mltp_e_c_r_ine_##Nome##_3.at(a_TSS).getIteradorFinal();\
				idx_ine_##Nome##_3.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt1 it1 = it1_inicial; it1 < a_it1; idx_ine_##Nome##_3.at(a_TSS).incrementarIterador(it1))\
					mltp_e_c_r_ine_##Nome##_3.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
				mltp_e_c_r_ine_##Nome##_3.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			}\
			if (mltp_e_c_r_ine_##Nome##_3.at(a_TSS).at(a_it1).size() == 0) \
				mltp_e_c_r_ine_##Nome##_3.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			else if (mltp_e_c_r_ine_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = mltp_e_c_r_ine_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_ine_##Nome##_3.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_ine_##Nome##_3.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					mltp_e_c_r_ine_##Nome##_3.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
				mltp_e_c_r_ine_##Nome##_3.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (mltp_e_c_r_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				mltp_e_c_r_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			else if (mltp_e_c_r_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = mltp_e_c_r_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					mltp_e_c_r_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>()); \
				mltp_e_c_r_ine_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3)+ "): \n" + std::string(erro.what())); } \
};\
void armazenarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_3(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const IdCenario a_idCenario){ \
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		if (idx_ine_##Nome##_3.at(a_TSS).size() == 0) \
			return; \
		if (idx_ine_##Nome##_3.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_ine_##Nome##_3.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_ine_##Nome##_3.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_ine_##Nome##_3.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_ine_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_ine_##Nome##_3.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (getIneLinear_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3) > -1) {\
						inicializarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome(a_TSS, a_it1, idIterador2, idIterador3); \
						mltp_e_c_ine_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getMultiplicador(getIneLinear_##Nome(a_TSS, a_it1, idIterador2, idIterador3)); \
					}\
				}\
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarMultiplicadorPorEstagioPorCenarioIneLinear_" + std::string(#Nome) + "_3(" + getFullString(a_it1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
};\
void armazenarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_3(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		if (idx_ine_##Nome##_3.at(a_TSS).size() == 0) \
			return; \
		if (idx_ine_##Nome##_3.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_ine_##Nome##_3.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_ine_##Nome##_3.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_ine_##Nome##_3.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_ine_##Nome##_3.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_ine_##Nome##_3.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_ine_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (getIneLinear_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3) > -1) {\
						inicializarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome(a_TSS, a_it1, idIterador2, idIterador3); \
						mltp_e_c_r_ine_##Nome##_3.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(a_idRealizacao).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getMultiplicador(getIneLinear_##Nome(a_TSS, a_it1, idIterador2, idIterador3)); \
					}\
				}\
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_" + std::string(#Nome) + "_3(" + getFullString(a_it1) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
};\
void imprimirMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_3(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_3(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void consolidarResultadosIneLinear_##Nome##_3(const TipoSubproblemaSolver a_TSS, const std::string a_restricao, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_3(Nome, TIt1, TIt2, TIt3, ImprimirMultiplicador) \
void ModeloOtimizacao::imprimirMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_3(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//ine" + std::string(#Nome) + "_3"); \
		const std::string nome_arquivo = std::string("ine" + std::string(#Nome) + "_3_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, mltp_e_c_ine_##Nome##_3.at(a_TSS))); \
		mltp_e_c_ine_##Nome##_3.at(a_TSS) = SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdCenario, double>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirMultiplicadorPorEstagioPorCenarioIneLinear_" + std::string(#Nome) + "_3(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_3(Nome, TIt1, TIt2, TIt3, ImprimirMultiplicador) \
imprimirMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_3(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_3(Nome, TIt1, TIt2, TIt3, ImprimirMultiplicador) \
void ModeloOtimizacao::imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_3(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//ine" + std::string(#Nome) + "_3"); \
		const std::string nome_arquivo = std::string("ine" + std::string(#Nome) + "_3_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, mltp_e_c_r_ine_##Nome##_3.at(a_TSS))); \
		mltp_e_c_r_ine_##Nome##_3.at(a_TSS) = SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_" + std::string(#Nome) + "_3(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_3(Nome, TIt1, TIt2, TIt3, ImprimirMultiplicador) \
imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_3(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_3(Nome, TIt1, TIt2, TIt3, ImprimirMultiplicador) \
armazenarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_3(a_TSS, a_idEstagio, a_idCenario);

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_3(Nome, TIt1, TIt2, TIt3, ImprimirMultiplicador) \
armazenarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_3(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao); 

#define DECLARAR_CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_3(Nome, TIt1, TIt2, TIt3, ImprimirMultiplicador) \
void ModeloOtimizacao::consolidarResultadosIneLinear_##Nome##_3(const TipoSubproblemaSolver a_TSS, const std::string a_restricao, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (std::string(std::string(#Nome) + "_3") != a_restricao) \
			return; \
		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida(); \
		if (getboolFromChar(#ImprimirMultiplicador)){ \
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//ine" + std::string(#Nome) + "_3"); \
			const std::string nome_arquivo = std::string("ine" + std::string(#Nome) + "_3" + ".csv"); \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string("ine" + std::string(#Nome) + "_3_" + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoHorizontalCSV(nome_arquivo, lista_arquivos, 3, true); \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::consolidarResultadosIneLinear_" + std::string(#Nome) + "_3(" + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_3(Nome, TIt1, TIt2, TIt3, ImprimirMultiplicador) \
consolidarResultadosIneLinear_##Nome##_3(a_TSS, restricao_str, a_maiorIdProcesso, a_entradaSaidaDados);



// ---------------------------------------------------------------------------------------
//
//Restrições Ineações Lineares 4
//
// ---------------------------------------------------------------------------------------

#define DECLARAR_RESTRICAO_INEQUACAO_LINEAR_4(Nome, TIt1, TIt2, TIt3, TIt4, ImprimirMultiplicador) \
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, std::vector<int>>>>>> idx_ine_##Nome##_4 = std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, std::vector<int>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, std::vector<int>>>>>());\
std::vector<bool> is_ine_linear_##Nome##_4_instanciada = std::vector<bool>(TipoSubproblemaSolver_Excedente, false);\
std::string getNomeIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4) { \
if (vetorEstagio.att(a_it1).getSolver(a_TSS)->isNomeSimplificado()) \
	return std::string("ine" + std::string(#Nome) + "," + getString(a_it1) + "," + getString(a_it2) + "," + getString(a_it3) + "," + getString(a_it4) );\
else \
	return std::string("ine" + std::string(#Nome) + "," + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) );\
}; \
int addIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4){ \
	try{ \
		const int idInequacaoLinear = vetorEstagio.att(a_it1).getSolver(a_TSS)->addConstrMaior(getNomeIneLinear_##Nome(a_TSS, a_it1, a_it2, a_it3, a_it4)); \
		if (idx_ine_##Nome##_4.at(a_TSS).size() == 0) \
			idx_ine_##Nome##_4.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, std::vector<int>>>>()); \
		else if (idx_ine_##Nome##_4.at(a_TSS).getIteradorFinal() < a_it1){ \
			for (TIt1 it1 = TIt1(idx_ine_##Nome##_4.at(a_TSS).getIteradorFinal() + 1); it1 < a_it1; it1++)\
				idx_ine_##Nome##_4.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, std::vector<int>>>>()); \
			idx_ine_##Nome##_4.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, std::vector<int>>>>()); \
		} \
		if (idx_ine_##Nome##_4.at(a_TSS).at(a_it1).size() == 0) \
			idx_ine_##Nome##_4.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, std::vector<int>>>()); \
		else if (idx_ine_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2){ \
			for (TIt2 it2 = TIt2(idx_ine_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal() + 1); it2 < a_it2; it2++) \
				idx_ine_##Nome##_4.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, std::vector<int>>>()); \
			idx_ine_##Nome##_4.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, std::vector<int>>>()); \
		} \
		if (idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
			idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, std::vector<int>>()); \
		else if (idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3){ \
			for (TIt3 it3 = TIt3(idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() + 1); it3 < a_it3; it3++)\
				idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, std::vector<int>>()); \
			idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, std::vector<int>>()); \
		} \
		if (idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
			idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, std::vector<int>(1, idInequacaoLinear)); \
		else if (idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4){ \
			for (TIt4 it4 = TIt4(idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() + 1); it4 < a_it4; it4++)\
				idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, std::vector<int>()); \
			idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, std::vector<int>(1, idInequacaoLinear)); \
		} \
		else if (idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0)\
			idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).setElemento(a_it4, std::vector<int>(1, idInequacaoLinear)); \
		else \
			throw std::invalid_argument("Inequacao Linear ja existente no modelo."); \
		if (!is_ine_linear_##Nome##_4_instanciada.at(a_TSS)){ \
			is_ine_linear_##Nome##_4_instanciada.at(a_TSS) = true;\
			lista_inequacoes_instanciadas.at(a_TSS).push_back(std::string(std::string(#Nome) + "_4")); \
		} \
		return idInequacaoLinear; \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("addIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "): \n" + std::string(erro.what())); } \
}; \
int getIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4){ \
	try{ return idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(0); } \
	catch (const std::exception& erro) { throw std::invalid_argument("getIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4)+ "): \n" + std::string(erro.what())); } \
}; \
int getIneLinear_##Nome##seExistir(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4){ \
	try{ \
		if (idx_ine_##Nome##_4.at(a_TSS).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_4.at(a_TSS).getIteradorInicial() > a_it1) \
			return -1; \
		else if (idx_ine_##Nome##_4.at(a_TSS).getIteradorFinal() < a_it1) \
			return -1; \
		else if (idx_ine_##Nome##_4.at(a_TSS).at(a_it1).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_4.at(a_TSS).at(a_it1).getIteradorInicial() > a_it2) \
			return -1; \
		else if (idx_ine_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) \
			return -1; \
		else if (idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).getIteradorInicial() > a_it3) \
			return -1; \
		else if (idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) \
			return -1; \
		else if (idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorInicial() > a_it4) \
			return -1; \
		else if (idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) \
			return -1; \
		else if (idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
			return -1; \
		else \
			return idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(0); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("getIneLinear_" + std::string(#Nome) + "seExistir(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4)+ "): \n" + std::string(erro.what())); } \
}; \
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>>>> mltp_e_c_ine_##Nome##_4 = std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>>>()); \
void inicializarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4){ \
	try{ \
		if (getboolFromChar(#ImprimirMultiplicador)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (mltp_e_c_ine_##Nome##_4.at(a_TSS).size() == 0) \
				mltp_e_c_ine_##Nome##_4.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>>()); \
			else if (mltp_e_c_ine_##Nome##_4.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt1 it1_inicial = mltp_e_c_ine_##Nome##_4.at(a_TSS).getIteradorFinal();\
				idx_ine_##Nome##_4.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt1 it1 = it1_inicial; it1 < a_it1; idx_ine_##Nome##_4.at(a_TSS).incrementarIterador(it1))\
					mltp_e_c_ine_##Nome##_4.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>>()); \
				mltp_e_c_ine_##Nome##_4.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>>()); \
			}\
			if (mltp_e_c_ine_##Nome##_4.at(a_TSS).at(a_it1).size() == 0) \
				mltp_e_c_ine_##Nome##_4.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>()); \
			else if (mltp_e_c_ine_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = mltp_e_c_ine_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_ine_##Nome##_4.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_ine_##Nome##_4.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					mltp_e_c_ine_##Nome##_4.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>()); \
				mltp_e_c_ine_##Nome##_4.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (mltp_e_c_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				mltp_e_c_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>()); \
			else if (mltp_e_c_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = mltp_e_c_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					mltp_e_c_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>()); \
				mltp_e_c_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>()); \
			}\
			if (mltp_e_c_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
				mltp_e_c_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			else if (mltp_e_c_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) { \
				TIt4 it4_inicial = mltp_e_c_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal();\
				idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4_inicial);\
				for (TIt4 it4 = it4_inicial; it4 < a_it4; idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4))\
					mltp_e_c_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<IdCenario, double>()); \
				mltp_e_c_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarMultiplicadorPorEstagioPorCenarioIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4)+ "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>> mltp_e_c_r_ine_##Nome##_4 = std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
void inicializarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4){ \
	try{ \
		if (getboolFromChar(#ImprimirMultiplicador)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const IdRealizacao maiorIdRealizacao = getAtributo(a_it1, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (mltp_e_c_r_ine_##Nome##_4.at(a_TSS).size() == 0) \
				mltp_e_c_r_ine_##Nome##_4.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
			else if (mltp_e_c_r_ine_##Nome##_4.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt1 it1_inicial = mltp_e_c_r_ine_##Nome##_4.at(a_TSS).getIteradorFinal();\
				idx_ine_##Nome##_4.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt1 it1 = it1_inicial; it1 < a_it1; idx_ine_##Nome##_4.at(a_TSS).incrementarIterador(it1))\
					mltp_e_c_r_ine_##Nome##_4.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
				mltp_e_c_r_ine_##Nome##_4.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
			}\
			if (mltp_e_c_r_ine_##Nome##_4.at(a_TSS).at(a_it1).size() == 0) \
				mltp_e_c_r_ine_##Nome##_4.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			else if (mltp_e_c_r_ine_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = mltp_e_c_r_ine_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_ine_##Nome##_4.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_ine_##Nome##_4.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					mltp_e_c_r_ine_##Nome##_4.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
				mltp_e_c_r_ine_##Nome##_4.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			}\
			if (mltp_e_c_r_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				mltp_e_c_r_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			else if (mltp_e_c_r_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = mltp_e_c_r_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					mltp_e_c_r_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
				mltp_e_c_r_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (mltp_e_c_r_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
				mltp_e_c_r_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			else if (mltp_e_c_r_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) { \
				TIt4 it4_inicial = mltp_e_c_r_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal();\
				idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4_inicial);\
				for (TIt4 it4 = it4_inicial; it4 < a_it4; idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4))\
					mltp_e_c_r_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>()); \
				mltp_e_c_r_ine_##Nome##_4.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4)+ "): \n" + std::string(erro.what())); } \
};\
void armazenarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_4(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const IdCenario a_idCenario){ \
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		if (idx_ine_##Nome##_4.at(a_TSS).size() == 0) \
			return; \
		if (idx_ine_##Nome##_4.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_ine_##Nome##_4.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_ine_##Nome##_4.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_ine_##Nome##_4.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_ine_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_ine_##Nome##_4.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).size() > 0){ \
						const TIt4 menorIdIterador4 = idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorInicial(); \
						const TIt4 maiorIdIterador4 = idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorFinal(); \
						for (TIt4 idIterador4 = menorIdIterador4; idIterador4 <= maiorIdIterador4; idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).incrementarIterador(idIterador4)) { \
							if (getIneLinear_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3, idIterador4) > -1) {\
								inicializarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4); \
								mltp_e_c_ine_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getMultiplicador(getIneLinear_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4)); \
							}\
						}\
					}\
				} \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarMultiplicadorPorEstagioPorCenarioIneLinear_" + std::string(#Nome) + "_4(" + getFullString(a_it1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
};\
void armazenarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_4(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		if (idx_ine_##Nome##_4.at(a_TSS).size() == 0) \
			return; \
		if (idx_ine_##Nome##_4.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_ine_##Nome##_4.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_ine_##Nome##_4.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_ine_##Nome##_4.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_ine_##Nome##_4.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_ine_##Nome##_4.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).size() > 0){ \
						const TIt4 menorIdIterador4 = idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorInicial(); \
						const TIt4 maiorIdIterador4 = idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorFinal(); \
						for (TIt4 idIterador4 = menorIdIterador4; idIterador4 <= maiorIdIterador4; idx_ine_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).incrementarIterador(idIterador4)) { \
							if (getIneLinear_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3, idIterador4) > -1) {\
								inicializarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4); \
								mltp_e_c_r_ine_##Nome##_4.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(a_idRealizacao).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getMultiplicador(getIneLinear_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4)); \
							}\
						}\
					}\
				} \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_" + std::string(#Nome) + "_4(" + getFullString(a_it1) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
};\
void imprimirMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_4(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_4(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void consolidarResultadosIneLinear_##Nome##_4(const TipoSubproblemaSolver a_TSS, const std::string a_restricao, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_4(Nome, TIt1, TIt2, TIt3, TIt4, ImprimirMultiplicador) \
void ModeloOtimizacao::imprimirMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_4(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//ine" + std::string(#Nome) + "_4"); \
		const std::string nome_arquivo = std::string("ine" + std::string(#Nome) + "_4_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, mltp_e_c_ine_##Nome##_4.at(a_TSS))); \
		mltp_e_c_ine_##Nome##_4.at(a_TSS) = SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdCenario, double>>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirMultiplicadorPorEstagioPorCenarioIneLinear_" + std::string(#Nome) + "_4(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_4(Nome, TIt1, TIt2, TIt3, TIt4, ImprimirMultiplicador) \
imprimirMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_4(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_4(Nome, TIt1, TIt2, TIt3, TIt4, ImprimirMultiplicador) \
void ModeloOtimizacao::imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_4(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//ine" + std::string(#Nome) + "_4"); \
		const std::string nome_arquivo = std::string("ine" + std::string(#Nome) + "_4_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, mltp_e_c_r_ine_##Nome##_4.at(a_TSS))); \
		mltp_e_c_r_ine_##Nome##_4.at(a_TSS) = SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_" + std::string(#Nome) + "_4(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_4(Nome, TIt1, TIt2, TIt3, TIt4, ImprimirMultiplicador) \
imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_4(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_4(Nome, TIt1, TIt2, TIt3, TIt4, ImprimirMultiplicador) \
armazenarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_4(a_TSS, a_idEstagio, a_idCenario);

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_4(Nome, TIt1, TIt2, TIt3, TIt4, ImprimirMultiplicador) \
armazenarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_4(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao); 

#define DECLARAR_CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_4(Nome, TIt1, TIt2, TIt3, TIt4, ImprimirMultiplicador) \
void ModeloOtimizacao::consolidarResultadosIneLinear_##Nome##_4(const TipoSubproblemaSolver a_TSS, const std::string a_restricao, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (std::string(std::string(#Nome) + "_4") != a_restricao) \
			return; \
		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida(); \
		if (getboolFromChar(#ImprimirMultiplicador)){ \
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//ine" + std::string(#Nome) + "_4"); \
			const std::string nome_arquivo = std::string("ine" + std::string(#Nome) + "_4" + ".csv"); \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string("ine" + std::string(#Nome) + "_4_" + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoHorizontalCSV(nome_arquivo, lista_arquivos, 4, true); \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::consolidarResultadosIneLinear_" + std::string(#Nome) + "_4(" + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_4(Nome, TIt1, TIt2, TIt3, TIt4, ImprimirMultiplicador) \
consolidarResultadosIneLinear_##Nome##_4(a_TSS, restricao_str, a_maiorIdProcesso, a_entradaSaidaDados);



// ---------------------------------------------------------------------------------------
//
//Restrições Ineações Lineares 5
//
// ---------------------------------------------------------------------------------------

#define DECLARAR_RESTRICAO_INEQUACAO_LINEAR_5(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, ImprimirMultiplicador) \
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>>>>> idx_ine_##Nome##_5 = std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>>>>());\
std::vector<bool> is_ine_linear_##Nome##_5_instanciada = std::vector<bool>(TipoSubproblemaSolver_Excedente, false);\
std::string getNomeIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5) { \
if (vetorEstagio.att(a_it1).getSolver(a_TSS)->isNomeSimplificado()) \
	return std::string("ine" + std::string(#Nome) + "," + getString(a_it1) + "," + getString(a_it2) + "," + getString(a_it3) + "," + getString(a_it4) + "," + getString(a_it5) );\
else \
	return std::string("ine" + std::string(#Nome) + "," + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) );\
}; \
int addIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5){ \
	try{ \
		const int idInequacaoLinear = vetorEstagio.att(a_it1).getSolver(a_TSS)->addConstrMaior(getNomeIneLinear_##Nome(a_TSS, a_it1, a_it2, a_it3, a_it4, a_it5)); \
		if (idx_ine_##Nome##_5.at(a_TSS).size() == 0) \
			idx_ine_##Nome##_5.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>>>()); \
		else if (idx_ine_##Nome##_5.at(a_TSS).getIteradorFinal() < a_it1){ \
			for (TIt1 it1 = TIt1(idx_ine_##Nome##_5.at(a_TSS).getIteradorFinal() + 1); it1 < a_it1; it1++)\
				idx_ine_##Nome##_5.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>>>()); \
			idx_ine_##Nome##_5.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>>>()); \
		} \
		if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).size() == 0) \
			idx_ine_##Nome##_5.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>>()); \
		else if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2){ \
			for (TIt2 it2 = TIt2(idx_ine_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal() + 1); it2 < a_it2; it2++) \
				idx_ine_##Nome##_5.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>>()); \
			idx_ine_##Nome##_5.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>>()); \
		} \
		if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
			idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>()); \
		else if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3){ \
			for (TIt3 it3 = TIt3(idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() + 1); it3 < a_it3; it3++)\
				idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>()); \
			idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, std::vector<int>>>()); \
		} \
		if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
			idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, std::vector<int>>()); \
		else if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4){ \
			for (TIt4 it4 = TIt4(idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() + 1); it4 < a_it4; it4++)\
				idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<TIt5, std::vector<int>>()); \
			idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, std::vector<int>>()); \
		} \
		if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
			idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, std::vector<int>(1, idInequacaoLinear)); \
		else if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5){ \
			for (TIt5 it5 =TIt5(idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() + 1); it5 < a_it5; it5++)\
				idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(it5, std::vector<int>()); \
			idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, std::vector<int>(1, idInequacaoLinear)); \
		} \
		else if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).size() == 0)\
			idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).setElemento(a_it5, std::vector<int>(1, idInequacaoLinear)); \
		else \
			throw std::invalid_argument("Inequacao Linear ja existente no modelo."); \
		if (!is_ine_linear_##Nome##_5_instanciada.at(a_TSS)){ \
			is_ine_linear_##Nome##_5_instanciada.at(a_TSS) = true;\
			lista_inequacoes_instanciadas.at(a_TSS).push_back(std::string(std::string(#Nome) + "_5")); \
		} \
		return idInequacaoLinear; \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("addIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "): \n" + std::string(erro.what())); } \
}; \
int getIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5){ \
	try{ return idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(0); } \
	catch (const std::exception& erro) { throw std::invalid_argument("getIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5)+ "): \n" + std::string(erro.what())); } \
}; \
int getIneLinear_##Nome##seExistir(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5){ \
	try{ \
		if (idx_ine_##Nome##_5.at(a_TSS).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_5.at(a_TSS).getIteradorInicial() > a_it1) \
			return -1; \
		else if (idx_ine_##Nome##_5.at(a_TSS).getIteradorFinal() < a_it1) \
			return -1; \
		else if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).getIteradorInicial() > a_it2) \
			return -1; \
		else if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) \
			return -1; \
		else if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).getIteradorInicial() > a_it3) \
			return -1; \
		else if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) \
			return -1; \
		else if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorInicial() > a_it4) \
			return -1; \
		else if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) \
			return -1; \
		else if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorInicial() > a_it5) \
			return -1; \
		else if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5) \
			return -1; \
		else if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).size() == 0) \
			return -1; \
		else \
			return idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(0); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("getIneLinear_" + std::string(#Nome) + "seExistir(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5)+ "): \n" + std::string(erro.what())); } \
}; \
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5,SmartEnupla<IdCenario, double>>>>>>> mltp_e_c_ine_##Nome##_5 = std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5,SmartEnupla<IdCenario, double>>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5,SmartEnupla<IdCenario, double>>>>>>()); \
void inicializarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5){ \
	try{ \
		if (getboolFromChar(#ImprimirMultiplicador)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (mltp_e_c_ine_##Nome##_5.at(a_TSS).size() == 0) \
				mltp_e_c_ine_##Nome##_5.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5,SmartEnupla<IdCenario, double>>>>>()); \
			else if (mltp_e_c_ine_##Nome##_5.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt1 it1_inicial = mltp_e_c_ine_##Nome##_5.at(a_TSS).getIteradorFinal();\
				idx_ine_##Nome##_5.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt1 it1 = it1_inicial; it1 < a_it1; idx_ine_##Nome##_5.at(a_TSS).incrementarIterador(it1))\
					mltp_e_c_ine_##Nome##_5.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5,SmartEnupla<IdCenario, double>>>>>()); \
				mltp_e_c_ine_##Nome##_5.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5,SmartEnupla<IdCenario, double>>>>>()); \
			}\
			if (mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).size() == 0) \
				mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5,SmartEnupla<IdCenario, double>>>>()); \
			else if (mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_ine_##Nome##_5.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_ine_##Nome##_5.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5,SmartEnupla<IdCenario, double>>>>()); \
				mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5,SmartEnupla<IdCenario, double>>>>()); \
			}\
			if (mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5,SmartEnupla<IdCenario, double>>>()); \
			else if (mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<TIt5,SmartEnupla<IdCenario, double>>>()); \
				mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5,SmartEnupla<IdCenario, double>>>()); \
			}\
			if (mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
				mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5,SmartEnupla<IdCenario, double>>()); \
			else if (mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) { \
				TIt4 it4_inicial = mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal();\
				idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4_inicial);\
				for (TIt4 it4 = it4_inicial; it4 < a_it4; idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4))\
					mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<TIt5,SmartEnupla<IdCenario, double>>()); \
				mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5,SmartEnupla<IdCenario, double>>()); \
			}\
			if (mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
				mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5,SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			else if (mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5) { \
				TIt5 it5_inicial = mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal();\
				idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5_inicial);\
				for (TIt5 it5 = it5_inicial; it5 < a_it5; idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5))\
					mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(it5,SmartEnupla<IdCenario, double>()); \
				mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5,SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarMultiplicadorPorEstagioPorCenarioIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5)+ "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>> mltp_e_c_r_ine_##Nome##_5 = std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>()); \
void inicializarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5){ \
	try{ \
		if (getboolFromChar(#ImprimirMultiplicador)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const IdRealizacao maiorIdRealizacao = getAtributo(a_it1, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (mltp_e_c_r_ine_##Nome##_5.at(a_TSS).size() == 0) \
				mltp_e_c_r_ine_##Nome##_5.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
			else if (mltp_e_c_r_ine_##Nome##_5.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt1 it1_inicial = mltp_e_c_r_ine_##Nome##_5.at(a_TSS).getIteradorFinal();\
				idx_ine_##Nome##_5.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt1 it1 = it1_inicial; it1 < a_it1; idx_ine_##Nome##_5.at(a_TSS).incrementarIterador(it1))\
					mltp_e_c_r_ine_##Nome##_5.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
				mltp_e_c_r_ine_##Nome##_5.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
			}\
			if (mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).size() == 0) \
				mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
			else if (mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_ine_##Nome##_5.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_ine_##Nome##_5.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
				mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
			}\
			if (mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			else if (mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
				mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			}\
			if (mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
				mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			else if (mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) { \
				TIt4 it4_inicial = mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal();\
				idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4_inicial);\
				for (TIt4 it4 = it4_inicial; it4 < a_it4; idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4))\
					mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
				mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
				mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			else if (mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5) { \
				TIt5 it5_inicial = mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal();\
				idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5_inicial);\
				for (TIt5 it5 = it5_inicial; it5 < a_it5; idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5))\
					mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(it5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>()); \
				mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5)+ "): \n" + std::string(erro.what())); } \
};\
void armazenarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_5(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const IdCenario a_idCenario){ \
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		if (idx_ine_##Nome##_5.at(a_TSS).size() == 0) \
			return; \
		if (idx_ine_##Nome##_5.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_ine_##Nome##_5.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_ine_##Nome##_5.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_ine_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_ine_##Nome##_5.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).size() > 0){ \
						const TIt4 menorIdIterador4 = idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorInicial(); \
						const TIt4 maiorIdIterador4 = idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorFinal(); \
						for (TIt4 idIterador4 = menorIdIterador4; idIterador4 <= maiorIdIterador4; idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).incrementarIterador(idIterador4)) { \
							if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).size() > 0){ \
								const TIt5 menorIdIterador5 = idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorInicial(); \
								const TIt5 maiorIdIterador5 = idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorFinal(); \
								for (TIt5 idIterador5 = menorIdIterador5; idIterador5 <= maiorIdIterador5; idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).incrementarIterador(idIterador5)) { \
									if (getIneLinear_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5) > -1) {\
										inicializarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5); \
										mltp_e_c_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getMultiplicador(getIneLinear_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5)); \
									}\
								}\
							}\
						}\
					}\
				} \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarMultiplicadorPorEstagioPorCenarioIneLinear_" + std::string(#Nome) + "_5(" + getFullString(a_it1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
};\
void armazenarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_5(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		if (idx_ine_##Nome##_5.at(a_TSS).size() == 0) \
			return; \
		if (idx_ine_##Nome##_5.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_ine_##Nome##_5.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_ine_##Nome##_5.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_ine_##Nome##_5.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_ine_##Nome##_5.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).size() > 0){ \
						const TIt4 menorIdIterador4 = idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorInicial(); \
						const TIt4 maiorIdIterador4 = idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorFinal(); \
						for (TIt4 idIterador4 = menorIdIterador4; idIterador4 <= maiorIdIterador4; idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).incrementarIterador(idIterador4)) { \
							if (idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).size() > 0){ \
								const TIt5 menorIdIterador5 = idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorInicial(); \
								const TIt5 maiorIdIterador5 = idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorFinal(); \
								for (TIt5 idIterador5 = menorIdIterador5; idIterador5 <= maiorIdIterador5; idx_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).incrementarIterador(idIterador5)) { \
									if (getIneLinear_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5) > -1) {\
										inicializarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5); \
										mltp_e_c_r_ine_##Nome##_5.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).at(a_idRealizacao).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getMultiplicador(getIneLinear_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5)); \
									}\
								}\
							}\
						}\
					}\
				} \
			} \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_" + std::string(#Nome) + "_5(" + getFullString(a_it1) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
};\
void imprimirMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_5(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_5(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void consolidarResultadosIneLinear_##Nome##_5(const TipoSubproblemaSolver a_TSS, const std::string a_restricao, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_5(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, ImprimirMultiplicador) \
void ModeloOtimizacao::imprimirMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_5(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//ine" + std::string(#Nome) + "_5"); \
		const std::string nome_arquivo = std::string("ine" + std::string(#Nome) + "_5_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, mltp_e_c_ine_##Nome##_5.at(a_TSS))); \
		mltp_e_c_ine_##Nome##_5.at(a_TSS) = SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5,SmartEnupla<IdCenario, double>>>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirMultiplicadorPorEstagioPorCenarioIneLinear_" + std::string(#Nome) + "_5(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_5(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, ImprimirMultiplicador) \
imprimirMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_5(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_5(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, ImprimirMultiplicador) \
void ModeloOtimizacao::imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_5(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//ine" + std::string(#Nome) + "_5"); \
		const std::string nome_arquivo = std::string("ine" + std::string(#Nome) + "_5_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, mltp_e_c_r_ine_##Nome##_5.at(a_TSS))); \
		mltp_e_c_r_ine_##Nome##_5.at(a_TSS) = SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_" + std::string(#Nome) + "_5(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_5(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, ImprimirMultiplicador) \
imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_5(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_5(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, ImprimirMultiplicador) \
armazenarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_5(a_TSS, a_idEstagio, a_idCenario);

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_5(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, ImprimirMultiplicador) \
armazenarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_5(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao); 

#define DECLARAR_CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_5(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, ImprimirMultiplicador) \
void ModeloOtimizacao::consolidarResultadosIneLinear_##Nome##_5(const TipoSubproblemaSolver a_TSS, const std::string a_restricao, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (std::string(std::string(#Nome) + "_5") != a_restricao) \
			return; \
		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida(); \
		if (getboolFromChar(#ImprimirMultiplicador)){ \
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//ine" + std::string(#Nome) + "_5"); \
			const std::string nome_arquivo = std::string("ine" + std::string(#Nome) + "_5" + ".csv"); \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string("ine" + std::string(#Nome) + "_5_" + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoHorizontalCSV(nome_arquivo, lista_arquivos, 5, true); \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::consolidarResultadosIneLinear_" + std::string(#Nome) + "_5(" + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_5(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, ImprimirMultiplicador) \
consolidarResultadosIneLinear_##Nome##_5(a_TSS, restricao_str, a_maiorIdProcesso, a_entradaSaidaDados);



// ---------------------------------------------------------------------------------------
//
//Restrição de Desigualdade Linear 6
//
// ---------------------------------------------------------------------------------------

#define DECLARAR_RESTRICAO_INEQUACAO_LINEAR_6(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, TIt6, ImprimirMultiplicador) \
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>>>>> idx_ine_##Nome##_6 = std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>>>>());\
std::vector<bool> is_ine_linear_##Nome##_6_instanciada = std::vector<bool>(TipoSubproblemaSolver_Excedente, false);\
std::string getNomeIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6) { \
if (vetorEstagio.att(a_it1).getSolver(a_TSS)->isNomeSimplificado()) \
	return std::string("ine" + std::string(#Nome) + "," + getString(a_it1) + "," + getString(a_it2) + "," + getString(a_it3) + "," + getString(a_it4) + "," + getString(a_it5) + "," + getString(a_it6) );\
else \
	return std::string("ine" + std::string(#Nome) + "," + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "," + getFullString(a_it6) );\
}; \
int addIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6){ \
	try{ \
		const int idInequacaoLinear = vetorEstagio.att(a_it1).getSolver(a_TSS)->addConstrMaior(getNomeIneLinear_##Nome(a_TSS, a_it1, a_it2, a_it3, a_it4, a_it5, a_it6)); \
		if (idx_ine_##Nome##_6.at(a_TSS).size() == 0) \
			idx_ine_##Nome##_6.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>>>()); \
		else if (idx_ine_##Nome##_6.at(a_TSS).getIteradorFinal() < a_it1){ \
			for (TIt1 it1 = TIt1(idx_ine_##Nome##_6.at(a_TSS).getIteradorFinal() + 1); it1 < a_it1; it1++)\
				idx_ine_##Nome##_6.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>>>()); \
			idx_ine_##Nome##_6.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>>>()); \
		} \
		if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).size() == 0) \
			idx_ine_##Nome##_6.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>>()); \
		else if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2){ \
			for (TIt2 it2 = TIt2(idx_ine_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal() + 1); it2 < a_it2; it2++) \
				idx_ine_##Nome##_6.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>>()); \
			idx_ine_##Nome##_6.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>>()); \
		} \
		if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
			idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>()); \
		else if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3){ \
			for (TIt3 it3 = TIt3(idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() + 1); it3 < a_it3; it3++)\
				idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>()); \
			idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>>()); \
		} \
		if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
			idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>()); \
		else if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4){ \
			for (TIt4 it4 = TIt4(idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() + 1); it4 < a_it4; it4++)\
				idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>()); \
			idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<TIt6, std::vector<int>>>()); \
		} \
		if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
			idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<TIt6, std::vector<int>>()); \
		else if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5){ \
			for (TIt5 it5 =TIt5(idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() + 1); it5 < a_it5; it5++)\
				idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(it5, SmartEnupla<TIt6, std::vector<int>>()); \
			idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<TIt6, std::vector<int>>()); \
		} \
		if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).size() == 0) \
			idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(a_it6, std::vector<int>(1, idInequacaoLinear)); \
		else if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal() < a_it6){ \
			for (TIt6 it6 =TIt6(idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal() + 1); it6 < a_it6; it6++)\
				idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(it6, std::vector<int>()); \
			idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(a_it6, std::vector<int>(1, idInequacaoLinear)); \
		} \
		else if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).size() == 0)\
			idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).setElemento(a_it6, std::vector<int>(1, idInequacaoLinear)); \
		else \
			throw std::invalid_argument("Inequacao Linear ja existente no modelo."); \
		if (!is_ine_linear_##Nome##_6_instanciada.at(a_TSS)){ \
			is_ine_linear_##Nome##_6_instanciada.at(a_TSS) = true;\
			lista_inequacoes_instanciadas.at(a_TSS).push_back(std::string(std::string(#Nome) + "_6")); \
		} \
		return idInequacaoLinear; \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("addIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "," + getFullString(a_it6) + "): \n" + std::string(erro.what())); } \
}; \
int getIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6){ \
	try{ return idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).at(0); } \
	catch (const std::exception& erro) { throw std::invalid_argument("getIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "," + getFullString(a_it6)+ "): \n" + std::string(erro.what())); } \
}; \
int getIneLinear_##Nome##seExistir(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6){ \
	try{ \
		if (idx_ine_##Nome##_6.at(a_TSS).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_6.at(a_TSS).getIteradorInicial() > a_it1) \
			return -1; \
		else if (idx_ine_##Nome##_6.at(a_TSS).getIteradorFinal() < a_it1) \
			return -1; \
		else if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).getIteradorInicial() > a_it2) \
			return -1; \
		else if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) \
			return -1; \
		else if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).getIteradorInicial() > a_it3) \
			return -1; \
		else if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) \
			return -1; \
		else if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorInicial() > a_it4) \
			return -1; \
		else if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) \
			return -1; \
		else if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorInicial() > a_it5) \
			return -1; \
		else if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5) \
			return -1; \
		else if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorInicial() > a_it6) \
			return -1; \
		else if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal() < a_it6) \
			return -1; \
		else if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).size() == 0) \
			return -1; \
		else \
			return idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).at(0); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("getIneLinear_" + std::string(#Nome) + "seExistir(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "," + getFullString(a_it6)+ "): \n" + std::string(erro.what())); } \
}; \
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>>>> mltp_e_c_ine_##Nome##_6 = std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>>>()); \
void inicializarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6){ \
	try{ \
		if (getboolFromChar(#ImprimirMultiplicador)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (mltp_e_c_ine_##Nome##_6.at(a_TSS).size() == 0) \
				mltp_e_c_ine_##Nome##_6.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>>()); \
			else if (mltp_e_c_ine_##Nome##_6.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt1 it1_inicial = mltp_e_c_ine_##Nome##_6.at(a_TSS).getIteradorFinal();\
				idx_ine_##Nome##_6.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt1 it1 = it1_inicial; it1 < a_it1; idx_ine_##Nome##_6.at(a_TSS).incrementarIterador(it1))\
					mltp_e_c_ine_##Nome##_6.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>>()); \
				mltp_e_c_ine_##Nome##_6.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>>()); \
			}\
			if (mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).size() == 0) \
				mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>()); \
			else if (mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_ine_##Nome##_6.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_ine_##Nome##_6.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>()); \
				mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>()); \
			}\
			if (mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>()); \
			else if (mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>()); \
				mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>()); \
			}\
			if (mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
				mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>()); \
			else if (mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) { \
				TIt4 it4_inicial = mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal();\
				idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4_inicial);\
				for (TIt4 it4 = it4_inicial; it4 < a_it4; idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4))\
					mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>()); \
				mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
				mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>()); \
			else if (mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5) { \
				TIt5 it5_inicial = mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal();\
				idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5_inicial);\
				for (TIt5 it5 = it5_inicial; it5 < a_it5; idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5))\
					mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(it5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>()); \
				mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>()); \
			}\
			if (mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).size() == 0) \
				mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(a_it6, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			else if (mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal() < a_it6) { \
				TIt6 it6_inicial = mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal();\
				idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).incrementarIterador(it6_inicial);\
				for (TIt6 it6 = it6_inicial; it6 < a_it6; idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).incrementarIterador(it6))\
					mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(it6, SmartEnupla<IdCenario, double>()); \
				mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(a_it6, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarMultiplicadorPorEstagioPorCenarioIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "," + getFullString(a_it6)+ "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>> mltp_e_c_r_ine_##Nome##_6 = std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>()); \
void inicializarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6){ \
	try{ \
		if (getboolFromChar(#ImprimirMultiplicador)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const IdRealizacao maiorIdRealizacao = getAtributo(a_it1, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (mltp_e_c_r_ine_##Nome##_6.at(a_TSS).size() == 0) \
				mltp_e_c_r_ine_##Nome##_6.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>()); \
			else if (mltp_e_c_r_ine_##Nome##_6.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt1 it1_inicial = mltp_e_c_r_ine_##Nome##_6.at(a_TSS).getIteradorFinal();\
				idx_ine_##Nome##_6.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt1 it1 = it1_inicial; it1 < a_it1; idx_ine_##Nome##_6.at(a_TSS).incrementarIterador(it1))\
					mltp_e_c_r_ine_##Nome##_6.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>()); \
				mltp_e_c_r_ine_##Nome##_6.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>()); \
			}\
			if (mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).size() == 0) \
				mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
			else if (mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_ine_##Nome##_6.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_ine_##Nome##_6.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
				mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
			}\
			if (mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
			else if (mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
				mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
			}\
			if (mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
				mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			else if (mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) { \
				TIt4 it4_inicial = mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal();\
				idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4_inicial);\
				for (TIt4 it4 = it4_inicial; it4 < a_it4; idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4))\
					mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
				mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			}\
			if (mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
				mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			else if (mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5) { \
				TIt5 it5_inicial = mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal();\
				idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5_inicial);\
				for (TIt5 it5 = it5_inicial; it5 < a_it5; idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5))\
					mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(it5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
				mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).size() == 0) \
				mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(a_it6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			else if (mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal() < a_it6) { \
				TIt6 it6_inicial = mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal();\
				idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).incrementarIterador(it6_inicial);\
				for (TIt6 it6 = it6_inicial; it6 < a_it6; idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).incrementarIterador(it6))\
					mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(it6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>()); \
				mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(a_it6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "," + getFullString(a_it6)+ "): \n" + std::string(erro.what())); } \
};\
void armazenarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_6(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const IdCenario a_idCenario){ \
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		if (idx_ine_##Nome##_6.at(a_TSS).size() == 0) \
			return; \
		if (idx_ine_##Nome##_6.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_ine_##Nome##_6.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_ine_##Nome##_6.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_ine_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_ine_##Nome##_6.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).size() > 0){ \
						const TIt4 menorIdIterador4 = idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorInicial(); \
						const TIt4 maiorIdIterador4 = idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorFinal(); \
						for (TIt4 idIterador4 = menorIdIterador4; idIterador4 <= maiorIdIterador4; idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).incrementarIterador(idIterador4)) { \
							if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).size() > 0){ \
								const TIt5 menorIdIterador5 = idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorInicial(); \
								const TIt5 maiorIdIterador5 = idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorFinal(); \
								for (TIt5 idIterador5 = menorIdIterador5; idIterador5 <= maiorIdIterador5; idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).incrementarIterador(idIterador5)) { \
									if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).size() > 0){ \
										const TIt6 menorIdIterador6 = idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).getIteradorInicial(); \
										const TIt6 maiorIdIterador6 = idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).getIteradorFinal(); \
										for (TIt6 idIterador6 = menorIdIterador6; idIterador6 <= maiorIdIterador6; idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).incrementarIterador(idIterador6)) { \
											if (getIneLinear_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6) > -1) {\
												inicializarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6); \
												mltp_e_c_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).at(idIterador6).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getMultiplicador(getIneLinear_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6)); \
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
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarMultiplicadorPorEstagioPorCenarioIneLinear_" + std::string(#Nome) + "_6(" + getFullString(a_it1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
};\
void armazenarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_6(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		if (idx_ine_##Nome##_6.at(a_TSS).size() == 0) \
			return; \
		if (idx_ine_##Nome##_6.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_ine_##Nome##_6.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_ine_##Nome##_6.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_ine_##Nome##_6.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_ine_##Nome##_6.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).size() > 0){ \
						const TIt4 menorIdIterador4 = idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorInicial(); \
						const TIt4 maiorIdIterador4 = idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorFinal(); \
						for (TIt4 idIterador4 = menorIdIterador4; idIterador4 <= maiorIdIterador4; idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).incrementarIterador(idIterador4)) { \
							if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).size() > 0){ \
								const TIt5 menorIdIterador5 = idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorInicial(); \
								const TIt5 maiorIdIterador5 = idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorFinal(); \
								for (TIt5 idIterador5 = menorIdIterador5; idIterador5 <= maiorIdIterador5; idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).incrementarIterador(idIterador5)) { \
									if (idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).size() > 0){ \
										const TIt6 menorIdIterador6 = idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).getIteradorInicial(); \
										const TIt6 maiorIdIterador6 = idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).getIteradorFinal(); \
										for (TIt6 idIterador6 = menorIdIterador6; idIterador6 <= maiorIdIterador6; idx_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).incrementarIterador(idIterador6)) { \
											if (getIneLinear_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6) > -1) {\
												inicializarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6); \
												mltp_e_c_r_ine_##Nome##_6.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).at(idIterador6).at(a_idRealizacao).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getMultiplicador(getIneLinear_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6)); \
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
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_" + std::string(#Nome) + "_6(" + getFullString(a_it1) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
};\
void imprimirMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_6(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_6(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void consolidarResultadosIneLinear_##Nome##_6(const TipoSubproblemaSolver a_TSS, const std::string a_restricao, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_6(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, TIt6, ImprimirMultiplicador) \
void ModeloOtimizacao::imprimirMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_6(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//ine" + std::string(#Nome) + "_6"); \
		const std::string nome_arquivo = std::string("ine" + std::string(#Nome) + "_6_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, mltp_e_c_ine_##Nome##_6.at(a_TSS))); \
		mltp_e_c_ine_##Nome##_6.at(a_TSS) = SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdCenario, double>>>>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirMultiplicadorPorEstagioPorCenarioIneLinear_" + std::string(#Nome) + "_6(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_6(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, TIt6, ImprimirMultiplicador) \
imprimirMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_6(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_6(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, TIt6, ImprimirMultiplicador) \
void ModeloOtimizacao::imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_6(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//ine" + std::string(#Nome) + "_6"); \
		const std::string nome_arquivo = std::string("ine" + std::string(#Nome) + "_6_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, mltp_e_c_r_ine_##Nome##_6.at(a_TSS))); \
		mltp_e_c_r_ine_##Nome##_6.at(a_TSS) = SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_" + std::string(#Nome) + "_6(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_6(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, TIt6, ImprimirMultiplicador) \
imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_6(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_6(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, TIt6, ImprimirMultiplicador) \
armazenarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_6(a_TSS, a_idEstagio, a_idCenario);

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_6(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, TIt6, ImprimirMultiplicador) \
armazenarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_6(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao); 

#define DECLARAR_CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_6(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, TIt6, ImprimirMultiplicador) \
void ModeloOtimizacao::consolidarResultadosIneLinear_##Nome##_6(const TipoSubproblemaSolver a_TSS, const std::string a_restricao, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (std::string(std::string(#Nome) + "_6") != a_restricao) \
			return; \
		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida(); \
		if (getboolFromChar(#ImprimirMultiplicador)){ \
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//ine" + std::string(#Nome) + "_6"); \
			const std::string nome_arquivo = std::string("ine" + std::string(#Nome) + "_6" + ".csv"); \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string("ine" + std::string(#Nome) + "_6_" + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoHorizontalCSV(nome_arquivo, lista_arquivos, 6, true); \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::consolidarResultadosIneLinear_" + std::string(#Nome) + "_6(" + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_6(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, TIt6, ImprimirMultiplicador) \
consolidarResultadosIneLinear_##Nome##_6(a_TSS, restricao_str, a_maiorIdProcesso, a_entradaSaidaDados);


// ---------------------------------------------------------------------------------------
//
//Restrição de Desigualdade Linear 7
//
// ---------------------------------------------------------------------------------------


#define DECLARAR_RESTRICAO_INEQUACAO_LINEAR_7(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, TIt6, TIt7, ImprimirMultiplicador) \
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, std::vector<int>>>>>>>>> idx_ine_##Nome##_7 = std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, std::vector<int>>>>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, std::vector<int>>>>>>>>());\
std::vector<bool> is_ine_linear_##Nome##_7_instanciada = std::vector<bool>(TipoSubproblemaSolver_Excedente, false);\
std::string getNomeIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6, const TIt7 a_it7) { \
if (vetorEstagio.att(a_it1).getSolver(a_TSS)->isNomeSimplificado()) \
	return std::string("ine" + std::string(#Nome) + "," + getString(a_it1) + "," + getString(a_it2) + "," + getString(a_it3) + "," + getString(a_it4) + "," + getString(a_it5) + "," + getString(a_it6) + "," + getString(a_it7) );\
else \
	return std::string("ine" + std::string(#Nome) + "," + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "," + getFullString(a_it6) + "," + getFullString(a_it7) );\
}; \
int addIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6, const TIt7 a_it7){ \
	try{ \
		const int idInequacaoLinear = vetorEstagio.att(a_it1).getSolver(a_TSS)->addConstrMaior(getNomeIneLinear_##Nome(a_TSS, a_it1, a_it2, a_it3, a_it4, a_it5, a_it6, a_it7)); \
		if (idx_ine_##Nome##_7.at(a_TSS).size() == 0) \
			idx_ine_##Nome##_7.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, std::vector<int>>>>>>>()); \
		else if (idx_ine_##Nome##_7.at(a_TSS).getIteradorFinal() < a_it1){ \
			for (TIt1 it1 = TIt1(idx_ine_##Nome##_7.at(a_TSS).getIteradorFinal() + 1); it1 < a_it1; it1++)\
				idx_ine_##Nome##_7.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, std::vector<int>>>>>>>()); \
			idx_ine_##Nome##_7.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, std::vector<int>>>>>>>()); \
		} \
		if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).size() == 0) \
			idx_ine_##Nome##_7.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, std::vector<int>>>>>>()); \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2){ \
			for (TIt2 it2 = TIt2(idx_ine_##Nome##_7.at(a_TSS).at(a_it1).getIteradorFinal() + 1); it2 < a_it2; it2++) \
				idx_ine_##Nome##_7.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, std::vector<int>>>>>>()); \
			idx_ine_##Nome##_7.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, std::vector<int>>>>>>()); \
		} \
		if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
			idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, std::vector<int>>>>>()); \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3){ \
			for (TIt3 it3 = TIt3(idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() + 1); it3 < a_it3; it3++)\
				idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, std::vector<int>>>>>()); \
			idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, std::vector<int>>>>>()); \
		} \
		if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
			idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, std::vector<int>>>>()); \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4){ \
			for (TIt4 it4 = TIt4(idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() + 1); it4 < a_it4; it4++)\
				idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, std::vector<int>>>>()); \
			idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, std::vector<int>>>>()); \
		} \
		if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
			idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<TIt6, SmartEnupla<TIt7, std::vector<int>>>()); \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5){ \
			for (TIt5 it5 =TIt5(idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() + 1); it5 < a_it5; it5++)\
				idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(it5, SmartEnupla<TIt6, SmartEnupla<TIt7, std::vector<int>>>()); \
			idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<TIt6, SmartEnupla<TIt7, std::vector<int>>>()); \
		} \
		if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).size() == 0) \
			idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(a_it6, SmartEnupla<TIt7, std::vector<int>>()); \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal() < a_it6){ \
			for (TIt6 it6 =TIt6(idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal() + 1); it6 < a_it6; it6++)\
				idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(it6, SmartEnupla<TIt7, std::vector<int>>()); \
			idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(a_it6, SmartEnupla<TIt7, std::vector<int>>()); \
		} \
		if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).size() == 0) \
			idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).addElemento(a_it7, std::vector<int>(1, idInequacaoLinear)); \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).getIteradorFinal() < a_it7){ \
			for (TIt7 it7 = TIt7(idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).getIteradorFinal() + 1); it7 < a_it7; it7++)\
				idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).addElemento(it7, std::vector<int>()); \
			idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).addElemento(a_it7, std::vector<int>(1, idInequacaoLinear)); \
		} \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).at(a_it7).size() == 0)\
			idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).setElemento(a_it7, std::vector<int>(1, idInequacaoLinear)); \
		else \
			throw std::invalid_argument("Inequacao Linear ja existente no modelo."); \
		if (!is_ine_linear_##Nome##_7_instanciada.at(a_TSS)){ \
			is_ine_linear_##Nome##_7_instanciada.at(a_TSS) = true;\
			lista_inequacoes_instanciadas.at(a_TSS).push_back(std::string(std::string(#Nome) + "_7")); \
		} \
		return idInequacaoLinear; \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("addIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "," + getFullString(a_it6) + "," + getFullString(a_it7) + "): \n" + std::string(erro.what())); } \
}; \
int getIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6, const TIt7 a_it7){ \
	try{ return idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).at(a_it7).at(0); } \
	catch (const std::exception& erro) { throw std::invalid_argument("getIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "," + getFullString(a_it6) + "," + getFullString(a_it7)+ "): \n" + std::string(erro.what())); } \
}; \
int getIneLinear_##Nome##seExistir(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6, const TIt7 a_it7){ \
	try{ \
		if (idx_ine_##Nome##_7.at(a_TSS).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_7.at(a_TSS).getIteradorInicial() > a_it1) \
			return -1; \
		else if (idx_ine_##Nome##_7.at(a_TSS).getIteradorFinal() < a_it1) \
			return -1; \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).getIteradorInicial() > a_it2) \
			return -1; \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) \
			return -1; \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).getIteradorInicial() > a_it3) \
			return -1; \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) \
			return -1; \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorInicial() > a_it4) \
			return -1; \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) \
			return -1; \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorInicial() > a_it5) \
			return -1; \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5) \
			return -1; \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorInicial() > a_it6) \
			return -1; \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal() < a_it6) \
			return -1; \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).size() == 0) \
			return -1; \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).getIteradorInicial() > a_it7) \
			return -1; \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).getIteradorFinal() < a_it7) \
			return -1; \
		else if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).at(a_it7).size() == 0) \
			return -1; \
		else \
			return idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).at(a_it7).at(0); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("getIneLinear_" + std::string(#Nome) + "seExistir(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "," + getFullString(a_it6) + "," + getFullString(a_it7)+ "): \n" + std::string(erro.what())); } \
}; \
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7,SmartEnupla<IdCenario, double>>>>>>>>> mltp_e_c_ine_##Nome##_7 = std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdCenario, double>>>>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdCenario, double>>>>>>>>()); \
void inicializarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6, const TIt7 a_it7){ \
	try{ \
		if (getboolFromChar(#ImprimirMultiplicador)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (mltp_e_c_ine_##Nome##_7.at(a_TSS).size() == 0) \
				mltp_e_c_ine_##Nome##_7.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdCenario, double>>>>>>>()); \
			else if (mltp_e_c_ine_##Nome##_7.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt1 it1_inicial = mltp_e_c_ine_##Nome##_7.at(a_TSS).getIteradorFinal();\
				idx_ine_##Nome##_7.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt1 it1 = it1_inicial; it1 < a_it1; idx_ine_##Nome##_7.at(a_TSS).incrementarIterador(it1))\
					mltp_e_c_ine_##Nome##_7.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdCenario, double>>>>>>>()); \
				mltp_e_c_ine_##Nome##_7.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdCenario, double>>>>>>>()); \
			}\
			if (mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).size() == 0) \
				mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdCenario, double>>>>>>()); \
			else if (mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_ine_##Nome##_7.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdCenario, double>>>>>>()); \
				mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdCenario, double>>>>>>()); \
			}\
			if (mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdCenario, double>>>>>()); \
			else if (mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdCenario, double>>>>>()); \
				mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdCenario, double>>>>>()); \
			}\
			if (mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
				mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdCenario, double>>>>()); \
			else if (mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) { \
				TIt4 it4_inicial = mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal();\
				idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4_inicial);\
				for (TIt4 it4 = it4_inicial; it4 < a_it4; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4))\
					mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdCenario, double>>>>()); \
				mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdCenario, double>>>>()); \
			}\
			if (mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
				mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdCenario, double>>>()); \
			else if (mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5) { \
				TIt5 it5_inicial = mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal();\
				idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5_inicial);\
				for (TIt5 it5 = it5_inicial; it5 < a_it5; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5))\
					mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(it5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdCenario, double>>>()); \
				mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).size() == 0) \
				mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(a_it6, SmartEnupla<TIt7, SmartEnupla<IdCenario, double>>()); \
			else if (mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal() < a_it6) { \
				TIt6 it6_inicial = mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal();\
				idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).incrementarIterador(it6_inicial);\
				for (TIt6 it6 = it6_inicial; it6 < a_it6; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).incrementarIterador(it6))\
					mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(it6, SmartEnupla<TIt7, SmartEnupla<IdCenario, double>>()); \
				mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(a_it6, SmartEnupla<TIt7, SmartEnupla<IdCenario, double>>()); \
			}\
			if (mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).size() == 0) \
				mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).addElemento(a_it7, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			else if (mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).getIteradorFinal() < a_it7) { \
				TIt7 it7_inicial = mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).getIteradorFinal();\
				idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).incrementarIterador(it7_inicial);\
				for (TIt7 it7 = it7_inicial; it7 < a_it7; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).incrementarIterador(it7))\
					mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).addElemento(it7, SmartEnupla<IdCenario, double>()); \
				mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).addElemento(a_it7, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarMultiplicadorPorEstagioPorCenarioIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "," + getFullString(a_it6) + "," + getFullString(a_it7)+ "): \n" + std::string(erro.what())); } \
};\
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>> mltp_e_c_r_ine_##Nome##_7 = std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>()); \
void inicializarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const TIt4 a_it4, const TIt5 a_it5, const TIt6 a_it6, const TIt7 a_it7){ \
	try{ \
		if (getboolFromChar(#ImprimirMultiplicador)){ \
			const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
			const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
			const IdRealizacao maiorIdRealizacao = getAtributo(a_it1, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); \
			const int numero_cenarios = int(cFim) - int(cIni) + 1; \
			if (mltp_e_c_r_ine_##Nome##_7.at(a_TSS).size() == 0) \
				mltp_e_c_r_ine_##Nome##_7.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>()); \
			else if (mltp_e_c_r_ine_##Nome##_7.at(a_TSS).getIteradorFinal() < a_it1) { \
				TIt1 it1_inicial = mltp_e_c_r_ine_##Nome##_7.at(a_TSS).getIteradorFinal();\
				idx_ine_##Nome##_7.at(a_TSS).incrementarIterador(it1_inicial);\
				for (TIt1 it1 = it1_inicial; it1 < a_it1; idx_ine_##Nome##_7.at(a_TSS).incrementarIterador(it1))\
					mltp_e_c_r_ine_##Nome##_7.at(a_TSS).addElemento(it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>()); \
				mltp_e_c_r_ine_##Nome##_7.at(a_TSS).addElemento(a_it1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>()); \
			}\
			if (mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).size() == 0) \
				mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>()); \
			else if (mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).getIteradorFinal() < a_it2) { \
				TIt2 it2_inicial = mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).getIteradorFinal();\
				idx_ine_##Nome##_7.at(a_TSS).at(a_it1).incrementarIterador(it2_inicial);\
				for (TIt2 it2 = it2_inicial; it2 < a_it2; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).incrementarIterador(it2))\
					mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).addElemento(it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>()); \
				mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).addElemento(a_it2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>()); \
			}\
			if (mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).size() == 0) \
				mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
			else if (mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal() < a_it3) { \
				TIt3 it3_inicial = mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).getIteradorFinal();\
				idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3_inicial);\
				for (TIt3 it3 = it3_inicial; it3 < a_it3; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).incrementarIterador(it3))\
					mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).addElemento(it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
				mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).addElemento(a_it3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>()); \
			}\
			if (mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).size() == 0) \
				mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
			else if (mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal() < a_it4) { \
				TIt4 it4_inicial = mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).getIteradorFinal();\
				idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4_inicial);\
				for (TIt4 it4 = it4_inicial; it4 < a_it4; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).incrementarIterador(it4))\
					mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
				mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).addElemento(a_it4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>()); \
			}\
			if (mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).size() == 0) \
				mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			else if (mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal() < a_it5) { \
				TIt5 it5_inicial = mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).getIteradorFinal();\
				idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5_inicial);\
				for (TIt5 it5 = it5_inicial; it5 < a_it5; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).incrementarIterador(it5))\
					mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(it5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
				mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).addElemento(a_it5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>()); \
			}\
			if (mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).size() == 0) \
				mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(a_it6, SmartEnupla<TIt7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			else if (mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal() < a_it6) { \
				TIt6 it6_inicial = mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).getIteradorFinal();\
				idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).incrementarIterador(it6_inicial);\
				for (TIt6 it6 = it6_inicial; it6 < a_it6; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).incrementarIterador(it6))\
					mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(it6, SmartEnupla<TIt7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
				mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).addElemento(a_it6, SmartEnupla<TIt7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>()); \
			}\
			if (mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).size() == 0) \
				mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).addElemento(a_it7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			else if (mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).getIteradorFinal() < a_it7) { \
				TIt7 it7_inicial = mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).getIteradorFinal();\
				idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).incrementarIterador(it7_inicial);\
				for (TIt7 it7 = it7_inicial; it7 < a_it7; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).incrementarIterador(it7))\
					mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).addElemento(it7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>()); \
				mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(a_it4).at(a_it5).at(a_it6).addElemento(a_it7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))))); \
			}\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getFullString(a_it4) + "," + getFullString(a_it5) + "," + getFullString(a_it6) + "," + getFullString(a_it7)+ "): \n" + std::string(erro.what())); } \
};\
void armazenarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_7(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const IdCenario a_idCenario){ \
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		if (idx_ine_##Nome##_7.at(a_TSS).size() == 0) \
			return; \
		if (idx_ine_##Nome##_7.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_ine_##Nome##_7.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).size() > 0){ \
						const TIt4 menorIdIterador4 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorInicial(); \
						const TIt4 maiorIdIterador4 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorFinal(); \
						for (TIt4 idIterador4 = menorIdIterador4; idIterador4 <= maiorIdIterador4; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).incrementarIterador(idIterador4)) { \
							if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).size() > 0){ \
								const TIt5 menorIdIterador5 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorInicial(); \
								const TIt5 maiorIdIterador5 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorFinal(); \
								for (TIt5 idIterador5 = menorIdIterador5; idIterador5 <= maiorIdIterador5; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).incrementarIterador(idIterador5)) { \
									if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).size() > 0){ \
										const TIt6 menorIdIterador6 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).getIteradorInicial(); \
										const TIt6 maiorIdIterador6 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).getIteradorFinal(); \
										for (TIt6 idIterador6 = menorIdIterador6; idIterador6 <= maiorIdIterador6; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).incrementarIterador(idIterador6)) { \
											if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).at(idIterador6).size() > 0){ \
												const TIt7 menorIdIterador7 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).at(idIterador6).getIteradorInicial(); \
												const TIt7 maiorIdIterador7 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).at(idIterador6).getIteradorFinal(); \
												for (TIt7 idIterador7 = menorIdIterador7; idIterador7 <= maiorIdIterador7; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).at(idIterador6).incrementarIterador(idIterador7)) { \
													if (getIneLinear_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6, idIterador7) > -1) {\
														inicializarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6, idIterador7); \
														mltp_e_c_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).at(idIterador6).at(idIterador7).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getMultiplicador(getIneLinear_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6, idIterador7)); \
													}\
												}\
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
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarMultiplicadorPorEstagioPorCenarioIneLinear_" + std::string(#Nome) + "_7(" + getFullString(a_it1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
};\
void armazenarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_7(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		if (idx_ine_##Nome##_7.at(a_TSS).size() == 0) \
			return; \
		if (idx_ine_##Nome##_7.at(a_TSS).getIteradorInicial() > a_it1) \
			return; \
		if (idx_ine_##Nome##_7.at(a_TSS).getIteradorFinal() < a_it1) \
			return; \
		if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).size() == 0) \
			return; \
		const TIt2 menorIdIterador2 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).getIteradorInicial(); \
		const TIt2 maiorIdIterador2 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).getIteradorFinal(); \
		for (TIt2 idIterador2 = menorIdIterador2; idIterador2 <= maiorIdIterador2; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).incrementarIterador(idIterador2)) { \
			if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).size() > 0){ \
				const TIt3 menorIdIterador3 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).getIteradorInicial(); \
				const TIt3 maiorIdIterador3 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).getIteradorFinal(); \
				for (TIt3 idIterador3 = menorIdIterador3; idIterador3 <= maiorIdIterador3; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).incrementarIterador(idIterador3)) { \
					if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).size() > 0){ \
						const TIt4 menorIdIterador4 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorInicial(); \
						const TIt4 maiorIdIterador4 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).getIteradorFinal(); \
						for (TIt4 idIterador4 = menorIdIterador4; idIterador4 <= maiorIdIterador4; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).incrementarIterador(idIterador4)) { \
							if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).size() > 0){ \
								const TIt5 menorIdIterador5 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorInicial(); \
								const TIt5 maiorIdIterador5 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).getIteradorFinal(); \
								for (TIt5 idIterador5 = menorIdIterador5; idIterador5 <= maiorIdIterador5; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).incrementarIterador(idIterador5)) { \
									if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).size() > 0){ \
										const TIt6 menorIdIterador6 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).getIteradorInicial(); \
										const TIt6 maiorIdIterador6 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).getIteradorFinal(); \
										for (TIt6 idIterador6 = menorIdIterador6; idIterador6 <= maiorIdIterador6; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).incrementarIterador(idIterador6)) { \
											if (idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).at(idIterador6).size() > 0){ \
												const TIt7 menorIdIterador7 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).at(idIterador6).getIteradorInicial(); \
												const TIt7 maiorIdIterador7 = idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).at(idIterador6).getIteradorFinal(); \
												for (TIt7 idIterador7 = menorIdIterador7; idIterador7 <= maiorIdIterador7; idx_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).at(idIterador6).incrementarIterador(idIterador7)) { \
													if (getIneLinear_##Nome##seExistir(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6, idIterador7) > -1) {\
														inicializarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6, idIterador7); \
														mltp_e_c_r_ine_##Nome##_7.at(a_TSS).at(a_it1).at(idIterador2).at(idIterador3).at(idIterador4).at(idIterador5).at(idIterador6).at(idIterador7).at(a_idRealizacao).at(a_idCenario) = vetorEstagio.att(a_it1).getSolver(a_TSS)->getMultiplicador(getIneLinear_##Nome(a_TSS, a_it1, idIterador2, idIterador3, idIterador4, idIterador5, idIterador6, idIterador7)); \
													}\
												}\
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
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_" + std::string(#Nome) + "_7(" + getFullString(a_it1) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
};\
void imprimirMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_7(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_7(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void consolidarResultadosIneLinear_##Nome##_7(const TipoSubproblemaSolver a_TSS, const std::string a_restricao, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_7(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, TIt6, TIt7, ImprimirMultiplicador) \
void ModeloOtimizacao::imprimirMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_7(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//ine" + std::string(#Nome) + "_7"); \
		const std::string nome_arquivo = std::string("ine" + std::string(#Nome) + "_7_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, mltp_e_c_ine_##Nome##_7.at(a_TSS))); \
		mltp_e_c_ine_##Nome##_7.at(a_TSS) = SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdCenario, double>>>>>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirMultiplicadorPorEstagioPorCenarioIneLinear_" + std::string(#Nome) + "_7(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_7(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, TIt6, TIt7, ImprimirMultiplicador) \
imprimirMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_7(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_7(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, TIt6, TIt7, ImprimirMultiplicador) \
void ModeloOtimizacao::imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_7(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (!getboolFromChar(#ImprimirMultiplicador)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//ine" + std::string(#Nome) + "_7"); \
		const std::string nome_arquivo = std::string("ine" + std::string(#Nome) + "_7_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, mltp_e_c_r_ine_##Nome##_7.at(a_TSS))); \
		mltp_e_c_r_ine_##Nome##_7.at(a_TSS) = SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<TIt4, SmartEnupla<TIt5, SmartEnupla<TIt6, SmartEnupla<TIt7, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>(); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_" + std::string(#Nome) + "_7(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_7(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, TIt6, TIt7, ImprimirMultiplicador) \
imprimirMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_7(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_7(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, TIt6, TIt7, ImprimirMultiplicador) \
armazenarMultiplicadorPorEstagioPorCenarioIneLinear_##Nome##_7(a_TSS, a_idEstagio, a_idCenario);

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_7(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, TIt6, TIt7, ImprimirMultiplicador) \
armazenarMultiplicadorPorEstagioPorCenarioPorRealizacaoIneLinear_##Nome##_7(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao); 

#define DECLARAR_CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_7(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, TIt6, TIt7, ImprimirMultiplicador) \
void ModeloOtimizacao::consolidarResultadosIneLinear_##Nome##_7(const TipoSubproblemaSolver a_TSS, const std::string a_restricao, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (std::string(std::string(#Nome) + "_7") != a_restricao) \
			return; \
		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida(); \
		if (getboolFromChar(#ImprimirMultiplicador)){ \
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//ine" + std::string(#Nome) + "_7"); \
			const std::string nome_arquivo = std::string("ine" + std::string(#Nome) + "_7" + ".csv"); \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string("ine" + std::string(#Nome) + "_7_" + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoHorizontalCSV(nome_arquivo, lista_arquivos, 7, true); \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::consolidarResultadosIneLinear_" + std::string(#Nome) + "_7(" + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_7(Nome, TIt1, TIt2, TIt3, TIt4, TIt5, TIt6, TIt7, ImprimirMultiplicador) \
consolidarResultadosIneLinear_##Nome##_7(a_TSS, restricao_str, a_maiorIdProcesso, a_entradaSaidaDados);


#endif 
