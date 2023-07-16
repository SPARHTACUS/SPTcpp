#ifndef VARIAVEL_DECISAO_NEW
#define VARIAVEL_DECISAO_NEW


// ---------------------------------------------------------------------------------------
//
//                                Variaveis de Decisão Continuas 3
//
// ---------------------------------------------------------------------------------------

//std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int,int>>>>> idx_var_##Nome##_3_NEW = std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int,int>>>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int,int>>>>()); \

//std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, int>>>>>>>>>>> idx_var_##Nome##_3_NEW =\
  std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, int>>>>>>>>>>> (TipoSubproblemaSolver_Excedente,\
              SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, int>>>>>>>>>>());        \

#define DECLARAR_VARIAVEL_DECISAO_3(Nome, TIt1, TIt2, TIt3, ImprimirPrimal, ImprimirDual)   \
std::vector<int> is_var_decisao_##Nome##_3_instanciada = std::vector<int>(TipoSubproblemaSolver_Excedente, 0);  \
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, int>>>>>>>>>>>> idx_var_##Nome##_3 =\
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, int>>>>>>>>>>>> (TipoSubproblemaSolver_Excedente,\
            SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, int>>>>>>>>>>>());\
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<IdCenario, double>>>>>>>>>>>> vlrP_e_c_var_##Nome##_3 =\
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<IdCenario, double>>>>>>>>>>>> (TipoSubproblemaSolver_Excedente,\
            SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<IdCenario, double>>>>>>>>>>>());\
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<IdCenario, double>>>>>>>>>>>> vlrD_e_c_var_##Nome##_3 =\
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<IdCenario, double>>>>>>>>>>>> (TipoSubproblemaSolver_Excedente,\
            SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<IdCenario, double>>>>>>>>>>>());\
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> vlrP_e_c_r_var_##Nome##_3 =\
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> (TipoSubproblemaSolver_Excedente,\
            SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>());\
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> vlrD_e_c_r_var_##Nome##_3 =\
std::vector<SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> (TipoSubproblemaSolver_Excedente,\
            SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>());\
void alocarVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const SmartEnupla<TIt2, SmartEnupla<TIt3, bool>> &a_enupla_alocacao){ \
	try{ \
		alocConteudoIter(idx_var_##Nome##_3.at(a_TSS), a_it1);\
		for (TIt2 it2 = a_enupla_alocacao.getIteradorInicial(); it2 <= a_enupla_alocacao.getIteradorFinal(); a_enupla_alocacao.incrementarIterador(it2)){ \
			alocConteudoIter(idx_var_##Nome##_3.at(a_TSS).at(a_it1), it2);\
			for (TIt3 it3 = a_enupla_alocacao.at(it2).getIteradorInicial(); it3 <= a_enupla_alocacao.at(it2).getIteradorFinal(); a_enupla_alocacao.at(it2).incrementarIterador(it3)) \
				alocConteudoIter(idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(it2), it3);\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("alocarVarDecisao_" + std::string(#Nome) + "(a_enupla_alocacao): \n" + std::string(erro.what())); } \
}\
std::string getNomeVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3) { \
if (vetorEstagio.att(a_it1).getSolver(a_TSS)->isNomeSimplificado()) \
	return std::string("var" + std::string(#Nome) + "," + getString(a_it1) + "," + getString(a_it2) + "," + getString(a_it3)); \
else \
	return std::string("var" + std::string(#Nome) + "," + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3)); \
}; \
int addVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3, const double a_valor_inferior, const double a_valor_superior, const double a_valor_objetivo){ \
	try{ \
		if (getVarDecisao_##Nome##seExistir(a_TSS, a_it1, a_it2, a_it3) > -1) \
			throw std::invalid_argument("Conteudo ja existente."); \
		int idVariavelDecisao = vetorEstagio.att(a_it1).getSolver(a_TSS)->addVar(a_valor_inferior, a_valor_superior, a_valor_objetivo, getNomeVarDecisao_##Nome(a_TSS, a_it1, a_it2, a_it3)); \
		addConteudoIters_11(idx_var_##Nome##_3.at(a_TSS), idVariavelDecisao, a_it1, a_it2, a_it3, 1, 1, 1, 1, 1, 1, 1, 1); \
		if (!is_var_decisao_##Nome##_3_instanciada.at(a_TSS)){ \
			is_var_decisao_##Nome##_3_instanciada.at(a_TSS) = true;\
			lista_variaveis_instanciadas.at(a_TSS).push_back(std::string(std::string(#Nome) + "_3")); \
		} \
		return idVariavelDecisao; \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("addVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "," + getString(a_valor_inferior) + "," + getString(a_valor_superior) + "," + getString(a_valor_objetivo) + "): \n" + std::string(erro.what())); } \
}; \
int getVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3){ \
	try{ return idx_var_##Nome##_3.at(a_TSS).at(a_it1).at(a_it2).at(a_it3).at(1).at(1).at(1).at(1).at(1).at(1).at(1).at(1); } \
	catch (const std::exception& erro) { throw std::invalid_argument("getVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "): \n" + std::string(erro.what())); } \
}; \
int getVarDecisao_##Nome##seExistir(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3){ \
	try{ \
		if (!is_var_decisao_##Nome##_3_instanciada.at(a_TSS)) return -1; \
		int conteudo = -1; \
		getConteudoIters_11(idx_var_##Nome##_3.at(a_TSS), conteudo, a_it1, a_it2, a_it3, 1, 1, 1, 1, 1, 1, 1, 1);\
		return conteudo; \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("getVarDecisao_" + std::string(#Nome) + "seExistir(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "): \n" + std::string(erro.what())); } \
}; \
void inicializarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3){ \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return;\
		const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
		const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
		const int numero_cenarios = int(cFim) - int(cIni) + 1; \
		SmartEnupla<IdCenario, double> conteudo(cIni, std::vector<double>(numero_cenarios, NAN)); \
		addConteudoIters_10(vlrP_e_c_var_##Nome##_3.at(a_TSS), conteudo, a_it1, a_it2, a_it3, 1, 1, 1, 1, 1, 1, 1); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorPrimalPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "): \n" + std::string(erro.what())); } \
};\
void inicializarValorDualPorEstagioPorCenarioVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3){ \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
		const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
		const int numero_cenarios = int(cFim) - int(cIni) + 1; \
		SmartEnupla<IdCenario, double> conteudo(cIni, std::vector<double>(numero_cenarios, NAN)); \
		addConteudoIters_10(vlrD_e_c_var_##Nome##_3.at(a_TSS), conteudo, a_it1, a_it2, a_it3, 1, 1, 1, 1, 1, 1, 1); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorDualPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "): \n" + std::string(erro.what())); } \
};\
void inicializarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3){ \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return;\
		const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
		const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
		const IdRealizacao maiorIdRealizacao = getAtributo(a_it1, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); \
		const int numero_cenarios = int(cFim) - int(cIni) + 1; \
		SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>> conteudo(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))));\
		addConteudoIters_10(vlrP_e_c_r_var_##Nome##_3.at(a_TSS), conteudo, a_it1, a_it2, a_it3, 1, 1, 1, 1, 1, 1, 1); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "): \n" + std::string(erro.what())); } \
};\
void inicializarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const TIt2 a_it2, const TIt3 a_it3){ \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return;\
		const IdCenario cIni = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario()); \
		const IdCenario cFim = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario()); \
		const IdRealizacao maiorIdRealizacao = getAtributo(a_it1, AttComumEstagio_maiorIdRealizacao, IdRealizacao()); \
		const int numero_cenarios = int(cFim) - int(cIni) + 1; \
		SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>> conteudo(IdRealizacao_1, std::vector<SmartEnupla<IdCenario, double>>(maiorIdRealizacao, SmartEnupla<IdCenario, double>(cIni, std::vector<double>(numero_cenarios, NAN))));\
		addConteudoIters_10(vlrD_e_c_r_var_##Nome##_3.at(a_TSS), conteudo, a_it1, a_it2, a_it3, 1, 1, 1, 1, 1, 1, 1); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("inicializarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "(" + getFullString(a_it1) + "," + getFullString(a_it2) + "," + getFullString(a_it3) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_3(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const IdCenario a_idCenario) { \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) return; \
		if (!is_var_decisao_##Nome##_3_instanciada.at(a_TSS)) return; \
		if (idx_var_##Nome##_3.at(a_TSS).getIteradorInicial() > a_it1) return; \
		if (idx_var_##Nome##_3.at(a_TSS).getIteradorFinal() < a_it1) return; \
		TIt1 it1 = a_it1; TIt2 it2; TIt3 it3; int it4; int it5; int it6; int it7; int it8; int it9; int it10; \
		std::vector<int> estados(10, -1); estados.at(0) = 0; bool parada = false;\
		for (int i = 0; i <= 1000000; i++){ \
			if (varredurasIters_10(std::string(std::string(#Nome) + "_3"), idx_var_##Nome##_3.at(a_TSS), estados, 1, parada, it1, it2, it3, it4, it5, it6, it7, it8, it9, it10)){\
				double conteudo = vetorEstagio.att(a_it1).getSolver(a_TSS)->getValorPrimal(idx_var_##Nome##_3.at(a_TSS).at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).at(it9).at(it10).at(1)); \
				addConteudoIters_11(vlrP_e_c_var_##Nome##_3.at(a_TSS), conteudo, it1, it2, it3, it4, it5, it6, it7, it8, it9, it10, a_idCenario); \
			} \
			if (parada) break; \
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorPrimalPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_3(" + getFullString(a_it1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorDualPorEstagioPorCenarioVarDecisao_##Nome##_3(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const IdCenario a_idCenario){ \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) return; \
		if (!is_var_decisao_##Nome##_3_instanciada.at(a_TSS)) return; \
		if (idx_var_##Nome##_3.at(a_TSS).getIteradorInicial() > a_it1) return; \
		if (idx_var_##Nome##_3.at(a_TSS).getIteradorFinal() < a_it1) return; \
		TIt1 it1 = a_it1; TIt2 it2; TIt3 it3; int it4; int it5; int it6; int it7; int it8; int it9; int it10; \
		std::vector<int> estados(10, -1); estados.at(0) = 0; bool parada = false;\
		for (int i = 0; i <= 1000000; i++){ \
			if (varredurasIters_10(std::string(std::string(#Nome) + "_3"), idx_var_##Nome##_3.at(a_TSS), estados, 1, parada, it1, it2, it3, it4, it5, it6, it7, it8, it9, it10)){\
				double conteudo = vetorEstagio.att(a_it1).getSolver(a_TSS)->getValorPrimal(idx_var_##Nome##_3.at(a_TSS).at(it1).at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).at(it9).at(it10).at(1)); \
				addConteudoIters_11(vlrD_e_c_var_##Nome##_3.at(a_TSS), conteudo, it1, it2, it3, it4, it5, it6, it7, it8, it9, it10, a_idCenario); \
			} \
			if (parada) break; \
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorDualPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_3(" + getFullString(a_it1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
};\
void armazenarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_3(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
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
void armazenarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_3(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
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

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_3(Nome, TIt1, TIt2, TIt3, ImprimirPrimal, ImprimirDual) \
void ModeloOtimizacao::imprimirValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_3(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_3"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_3_primal_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(3, vlrP_e_c_var_##Nome##_3.at(a_TSS))); \
		vlrP_e_c_var_##Nome##_3.at(a_TSS) = \
		SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<IdCenario, double>>>>>>>>>>>();\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorPrimalPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_3(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\
void ModeloOtimizacao::imprimirValorDualPorEstagioPorCenarioVarDecisao_##Nome##_3(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_3"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_3_dual_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(3, vlrD_e_c_var_##Nome##_3.at(a_TSS))); \
		vlrD_e_c_var_##Nome##_3.at(a_TSS) = \
		SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<IdCenario, double>>>>>>>>>>>();\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorDualPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_3(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_3(Nome, TIt1, TIt2, TIt3, ImprimirPrimal, ImprimirDual) \
imprimirValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_3(TSS, getFullString(a_idProcesso), a_entradaSaidaDados); \
imprimirValorDualPorEstagioPorCenarioVarDecisao_##Nome##_3(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);  

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_3(Nome, TIt1, TIt2, TIt3, ImprimirPrimal, ImprimirDual) \
void ModeloOtimizacao::imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_3(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//var" + std::string(#Nome) + "_3"); \
		const std::string nome_arquivo = std::string("var" + std::string(#Nome) + "_3_primal_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(true, vlrP_e_c_r_var_##Nome##_3.at(a_TSS))); \
		vlrP_e_c_r_var_##Nome##_3.at(a_TSS) = \
		SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>();\
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
		vlrD_e_c_r_var_##Nome##_3.at(a_TSS) = \
		SmartEnupla<TIt1, SmartEnupla<TIt2, SmartEnupla<TIt3, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>();\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_" + std::string(#Nome) + "_3(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_3(Nome, TIt1, TIt2, TIt3, ImprimirPrimal, ImprimirDual) \
imprimirValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_3(TSS, getFullString(a_idProcesso), a_entradaSaidaDados); \
imprimirValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_3(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);  \


#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_3(Nome, TIt1, TIt2, TIt3, ImprimirPrimal, ImprimirDual) \
armazenarValorPrimalPorEstagioPorCenarioVarDecisao_##Nome##_3(a_TSS, a_idEstagio, a_idCenario); \
armazenarValorDualPorEstagioPorCenarioVarDecisao_##Nome##_3(a_TSS, a_idEstagio, a_idCenario);

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_3(Nome, TIt1, TIt2, TIt3, ImprimirPrimal, ImprimirDual) \
armazenarValorPrimalPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_3(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao); \
armazenarValorDualPorEstagioPorCenarioPorRealizacaoVarDecisao_##Nome##_3(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao); 

#define DECLARAR_CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_3(Nome, TIt1, TIt2, TIt3, ImprimirPrimal, ImprimirDual) \
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

#define CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_3(Nome, TIt1, TIt2, TIt3, ImprimirPrimal, ImprimirDual) \
consolidarResultadosVarDecisao_##Nome##_3(a_TSS, variavel_str, a_maiorIdProcesso, a_entradaSaidaDados);


#endif 
