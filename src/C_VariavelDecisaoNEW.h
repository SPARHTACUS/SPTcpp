#ifndef VARIAVEL_DECISAO_NEW
#define VARIAVEL_DECISAO_NEW


// ---------------------------------------------------------------------------------------
//
//                                Variaveis de Decisão Continuas 3
//
// ---------------------------------------------------------------------------------------

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









//
//  Chamada Geral Declaracao
//

#define DECLARAR_ELEMENTO_TESTE(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual)   \
DECLARAR_ALOCAR_ENUPLAS (Elem, Nome, Nro, Valores) \
DECLARAR_ALOCAR_ELEMENTO(Elem, Nome, Nro, Valores) \
DECLARAR_GET_NOME_ELEMENTO(Elem, Nome, Nro, Valores) \
DECLARAR_ADD_ELEMENTO(Elem, Nome, Nro, Valores) \
DECLARAR_GET_ELEMENTO(Elem, Nome, Nro, Valores) \
DECLARAR_GET_ELEMENTO_SE_EXISTIR(Elem, Nome, Nro, Valores) \
DECLARAR_ARMAZENAR_VALOR(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) \

//
// DEFINICOES GERAIS
//

#define ITERS_ARGS(TIt) , const TIt a_##TIt
#define GET_STRING_ELEMENTO(TIt)  + "," + getString(a_##TIt)
#define GET_FULL_STRING_ELEMENTO(TIt)  + "," + getFullString(a_##TIt)

#define AT_A_ITERS(TIt).at(a_##TIt)
#define AT_ITERS(TIt).at(_##TIt)

#define AT_CONST_1_11 .at(1).at(1).at(1).at(1).at(1).at(1).at(1).at(1).at(1).at(1)
#define AT_CONST_2_11 .at(1).at(1).at(1).at(1).at(1).at(1).at(1).at(1).at(1)
#define AT_CONST_3_11 .at(1).at(1).at(1).at(1).at(1).at(1).at(1).at(1)
#define AT_CONST_4_11 .at(1).at(1).at(1).at(1).at(1).at(1).at(1)
#define AT_CONST_5_11 .at(1).at(1).at(1).at(1).at(1).at(1)
#define AT_CONST_6_11 .at(1).at(1).at(1).at(1).at(1)
#define AT_CONST_7_11 .at(1).at(1).at(1).at(1)
#define AT_CONST_8_11 .at(1).at(1).at(1)
#define AT_CONST_9_11 .at(1).at(1)
#define AT_CONST_10_11 .at(1)
#define AT_CONST_11_11

#define ARGS_ITERS(TIt) , a_##TIt 
#define ARGS_CONST_1_11  , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
#define ARGS_CONST_2_11  , 1, 1, 1, 1, 1, 1, 1, 1, 1
#define ARGS_CONST_3_11  , 1, 1, 1, 1, 1, 1, 1, 1
#define ARGS_CONST_4_11  , 1, 1, 1, 1, 1, 1, 1
#define ARGS_CONST_5_11  , 1, 1, 1, 1, 1, 1
#define ARGS_CONST_6_11  , 1, 1, 1, 1, 1
#define ARGS_CONST_7_11  , 1, 1, 1, 1
#define ARGS_CONST_8_11  , 1, 1, 1
#define ARGS_CONST_9_11  , 1, 1
#define ARGS_CONST_10_11 , 1
#define ARGS_CONST_11_11 , 1

#define _ITERS(TIt) , _##TIt 
#define DECLARA_ITERS(TIt) TIt _##TIt;
#define DECLARA_ITERS_CONST_1_10 int it2; int it3; int it4; int it5; int it6; int it7; int it8; int it9; int it10;
#define DECLARA_ITERS_CONST_2_10 int it3; int it4; int it5; int it6; int it7; int it8; int it9; int it10;
#define DECLARA_ITERS_CONST_3_10 int it4; int it5; int it6; int it7; int it8; int it9; int it10;
#define DECLARA_ITERS_CONST_4_10 int it5; int it6; int it7; int it8; int it9; int it10;
#define DECLARA_ITERS_CONST_5_10 int it6; int it7; int it8; int it9; int it10;
#define DECLARA_ITERS_CONST_6_10 int it7; int it8; int it9; int it10;
#define DECLARA_ITERS_CONST_7_10 int it8; int it9; int it10;
#define DECLARA_ITERS_CONST_8_10 int it9; int it10;
#define DECLARA_ITERS_CONST_9_10 int it10;
#define DECLARA_ITERS_CONST_10_10 

#define INT_ARG_1_10 ,it2,it3,it4,it5,it6,it7,it8,it9,it10
#define INT_ARG_2_10 ,it3,it4,it5,it6,it7,it8,it9,it10
#define INT_ARG_3_10 ,it4,it5,it6,it7,it8,it9,it10
#define INT_ARG_4_10 ,it5,it6,it7,it8,it9,it10
#define INT_ARG_5_10 ,it6,it7,it8,it9,it10
#define INT_ARG_6_10 ,it7,it8,it9,it10
#define INT_ARG_7_10 ,it8,it9,it10
#define INT_ARG_8_10 ,it9,it10
#define INT_ARG_9_10 ,it10
#define INT_ARG_10_10 

#define AT_INT_1_10 .at(it2).at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).at(it9).at(it10)
#define AT_INT_2_10 .at(it3).at(it4).at(it5).at(it6).at(it7).at(it8).at(it9).at(it10)
#define AT_INT_3_10 .at(it4).at(it5).at(it6).at(it7).at(it8).at(it9).at(it10)
#define AT_INT_4_10 .at(it5).at(it6).at(it7).at(it8).at(it9).at(it10)
#define AT_INT_5_10 .at(it6).at(it7).at(it8).at(it9).at(it10)
#define AT_INT_6_10 .at(it7).at(it8).at(it9).at(it10)
#define AT_INT_7_10 .at(it8).at(it9).at(it10)
#define AT_INT_8_10 .at(it9).at(it10)
#define AT_INT_9_10 .at(it10)
#define AT_INT_10_10 

//
// ALOCAR ENUPLAS
//

#define DECLARAR_ALOCAR_ENUPLA_ITER(TIt) SmartEnupla<TIt, 

#define DECLARAR_ALOCAR_ENUPLA_CONST_1  SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, 
#define DECLARAR_ALOCAR_ENUPLA_CONST_2  SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, 
#define DECLARAR_ALOCAR_ENUPLA_CONST_3  SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, 
#define DECLARAR_ALOCAR_ENUPLA_CONST_4  SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, 
#define DECLARAR_ALOCAR_ENUPLA_CONST_5  SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, 
#define DECLARAR_ALOCAR_ENUPLA_CONST_6  SmartEnupla<int, SmartEnupla<int, SmartEnupla<int, SmartEnupla<int,  
#define DECLARAR_ALOCAR_ENUPLA_CONST_7  SmartEnupla<int, SmartEnupla<int, SmartEnupla<int,  
#define DECLARAR_ALOCAR_ENUPLA_CONST_8  SmartEnupla<int, SmartEnupla<int,  
#define DECLARAR_ALOCAR_ENUPLA_CONST_9  SmartEnupla<int,  
#define DECLARAR_ALOCAR_ENUPLA_CONST_10   

#define DECLARAR_ALOCAR_ENUPLAS_PRIMAL (Elem, Nome, Nro, Valores) \
std::vector<Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdCenario, double>>>>>>>>>>>> vlrP_e_c_##Elem##_##Nome##_##Nro =\
std::vector<Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdCenario, double>>>>>>>>>>>> (TipoSubproblemaSolver_Excedente,\
            Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdCenario, double>>>>>>>>>>>());\
std::vector<Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> vlrP_e_c_r_##Elem##_##Nome##_##Nro =\
std::vector<Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> (TipoSubproblemaSolver_Excedente,\
            Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>());\

#define DECLARAR_ALOCAR_ENUPLAS_DUAL (Elem, Nome, Nro, Valores) \
std::vector<Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdCenario, double>>>>>>>>>>>> vlrD_e_c_##Elem##_##Nome##_##Nro =\
std::vector<Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdCenario, double>>>>>>>>>>>> (TipoSubproblemaSolver_Excedente,\
            Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdCenario, double>>>>>>>>>>>());\
std::vector<Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> vlrD_e_c_r_##Elem##_##Nome##_##Nro =\
std::vector<Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> (TipoSubproblemaSolver_Excedente,\
            Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>());\

#define DECLARAR_ALOCAR_ENUPLAS_VarDecisao (Elem, Nome, Nro, Valores) DECLARAR_ALOCAR_ENUPLAS_PRIMAL (Elem, Nome, Nro, Valores) DECLARAR_ALOCAR_ENUPLAS_DUAL   (Elem, Nome, Nro, Valores) 
#define DECLARAR_ALOCAR_ENUPLAS_EquLinear  (Elem, Nome, Nro, Valores) DECLARAR_ALOCAR_ENUPLAS_DUAL   (Elem, Nome, Nro, Valores) 
#define DECLARAR_ALOCAR_ENUPLAS_IneLinear  (Elem, Nome, Nro, Valores) DECLARAR_ALOCAR_ENUPLAS_DUAL   (Elem, Nome, Nro, Valores) 

#define DECLARAR_ALOCAR_ENUPLAS (Elem, Nome, Nro, Valores) \
std::vector<int> is_##Elem##_##Nome##_##Nro##_instanciada = std::vector<int>(TipoSubproblemaSolver_Excedente, 0);  \
std::vector<DECLARAR_ALOCAR_ENUPLA_ITER DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<int, int>>>>>>>>>>>> idx_##Elem##_##Nome##_##Nro =\
std::vector<DECLARAR_ALOCAR_ENUPLA_ITER DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<int, int>>>>>>>>>>>> (TipoSubproblemaSolver_Excedente,\
            DECLARAR_ALOCAR_ENUPLA_ITER DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<int, int>>>>>>>>>>>());\
DECLARAR_ALOCAR_ENUPLAS_##Elem (Elem, Nome, Nro, Valores)

//
// ALOCAR ELEMENTO (Funcionando somente para 3)
//

#define DECLARAR_ALOCAR_ELEMENTO(Elem, Nome, Nro, Valores) \ DECLARAR_ALOCAR_ELEMENTO_##Nro(Elem, Nome, Nro, Valores)
#define DECLARAR_ALOCAR_ELEMENTO_1(Elem, Nome, Nro, Valores) 
#define DECLARAR_ALOCAR_ELEMENTO_2(Elem, Nome, Nro, Valores) 
#define DECLARAR_ALOCAR_ELEMENTO_3(Elem, Nome, Nro, Valores) \
void alocar##Elem_##Nome(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const SmartEnupla<TIt2, SmartEnupla<TIt3, bool>> &a_enupla_alocacao){ \
	try{ \
		alocConteudoIter(idx_var_##Nome##_3.at(a_TSS), a_it1);\
		for (TIt2 it2 = a_enupla_alocacao.getIteradorInicial(); it2 <= a_enupla_alocacao.getIteradorFinal(); a_enupla_alocacao.incrementarIterador(it2)){ \
			alocConteudoIter(idx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_it1), it2);\
			for (TIt3 it3 = a_enupla_alocacao.at(it2).getIteradorInicial(); it3 <= a_enupla_alocacao.at(it2).getIteradorFinal(); a_enupla_alocacao.at(it2).incrementarIterador(it3)) \
				alocConteudoIter(idx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_it1).at(it2), it3);\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("alocar" + std::string(#Elem) + "_" + std::string(#Nome) + std::string(#Nro) + "(a_enupla_alocacao): \n" + std::string(erro.what())); } \
}
#define DECLARAR_ALOCAR_ELEMENTO_4(Elem, Nome, Nro, Valores) 
#define DECLARAR_ALOCAR_ELEMENTO_5(Elem, Nome, Nro, Valores) 
#define DECLARAR_ALOCAR_ELEMENTO_6(Elem, Nome, Nro, Valores) 
#define DECLARAR_ALOCAR_ELEMENTO_7(Elem, Nome, Nro, Valores) 
#define DECLARAR_ALOCAR_ELEMENTO_8(Elem, Nome, Nro, Valores) 
#define DECLARAR_ALOCAR_ELEMENTO_9(Elem, Nome, Nro, Valores) 
#define DECLARAR_ALOCAR_ELEMENTO_10(Elem, Nome, Nro, Valores) 

//
// GET NOME ELEMENTO
//

#define DECLARAR_GET_NOME_ELEMENTO(Elem, Nome, Nro, Valores) \
std::string getNome##Elem##_##Nome(const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS) ){\
if (vetorEstagio.att(a_IdEstagio).getSolver(a_TSS)->isNomeSimplificado()) \
	return std::string(std::string(#Elem) + std::string(#Nome) Valores(GET_STRING_ELEMENTO); \
else \
	return std::string(std::string(#Elem) + std::string(#Nome) Valores(GET_FULL_STRING_ELEMENTO); \
};

//
// ADD ELEMENTO
//

#define DECLARAR_ADD_ELEMENTO_VarDecisao_1 , const double a_valor_inferior, const double a_valor_superior, const double a_valor_objetivo
#define DECLARAR_ADD_ELEMENTO_EquLinear_1
#define DECLARAR_ADD_ELEMENTO_IneLinear_1

#define DECLARAR_ADD_ELEMENTO_VarDecisao_2 addVar(a_valor_inferior, a_valor_superior, a_valor_objetivo,
#define DECLARAR_ADD_ELEMENTO_EquLinear_2 addConstrIgual(
#define DECLARAR_ADD_ELEMENTO_IneLinear_2 addConstrMaior(

#define DECLARAR_ADD_ELEMENTO_VarDecisao_3 + "," + getFullString(a_valor_inferior) + "," + getFullString(a_valor_superior) + "," + getFullString(a_valor_objetivo)
#define DECLARAR_ADD_ELEMENTO_EquLinear_3 
#define DECLARAR_ADD_ELEMENTO_IneLinear_3 

#define DECLARAR_ADD_ELEMENTO(Elem, Nome, Nro, Valores) \
int add##Elem##_##Nome(const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS) DECLARAR_ADD_ELEMENTO_##Elem##_1){ \
	try{ \
		if (get##Elem##_##Nome##seExistir(a_TSS Valores(ARGS_ITERS)) > -1) \
			throw std::invalid_argument("Conteudo ja existente."); \
		int id##Elem = vetorEstagio.att(a_IdEstagio).getSolver(a_TSS)-> DECLARAR_ADD_ELEMENTO_##Elem##_2 getNome##Elem##_##Nome(a_TSS Valores(ARGS_ITERS))); \
		addConteudoIters_11(idx_##Elem##_##Nome##_3.at(a_TSS), id##Elem Valores(ARGS_ITERS) ARGS_CONST_##Nro##_11); \
		if (!is_##Elem##_##Nome##_##Nro##_instanciada.at(a_TSS)){ \
			is_##Elem##_##Nome##_##Nro##_instanciada.at(a_TSS) = true;\
			lista_##Elem##_instanciadas.at(a_TSS).push_back(std::string(std::string(#Nome) + "_" + std::string(#Nro))); \
		} \
		return id##Elem; \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("add" + std::string(#Elem) + "_" + std::string(#Nome) + "(" Valores(GET_FULL_STRING_ELEMENTO) DECLARAR_ADD_ELEMENTO_##Elem##_3 + "): \n" + std::string(erro.what())); } \
}; \

//
// GET ELEMENTO
//

#define DECLARAR_GET_ELEMENTO(Elem, Nome, Nro, Valores) \
int get##Elem##_##Nome(const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS)){ \
	try{ return idx_##Elem##_##Nome##_##Nro.at(a_TSS)Valores(AT_A_ITERS)Valores(AT_CONST_##Nro##_11); } \
	catch (const std::exception& erro) { throw std::invalid_argument("get" + std::string(#Elem) +  "_" + std::string(#Nome) + "(" Valores(GET_FULL_STRING_ELEMENTO) + "): \n" + std::string(erro.what())); } \
}; \

//
// GET ELEMENTO SE EXISTIR
//

#define DECLARAR_GET_ELEMENTO_SE_EXISTIR(Elem, Nome, Nro, Valores) \
int get##Elem##_##Nome##seExistir(const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS)){ \
	try{ \
		if (!is_##Elem##_##Nome##_##Nro_instanciada.at(a_TSS)) return -1; \
		int conteudo = -1; \
		getConteudoIters_11(idx_##Elem##_##Nome##_##Nro.at(a_TSS), conteudo Valores(ARGS_ITERS) Valores(ARGS_CONST_##Nro##_11); \
		return conteudo; \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("get" + std::string(#Elem) + "_" + std::string(#Nome) + "seExistir(" Valores(GET_FULL_STRING_ELEMENTO) + "): \n" + std::string(erro.what())); } \
}; \

//
// ARMAZENAR VALOR PRIMAL POR ESTAGIO POR CENARIO
//

#define DECLARAR_ARMAZENAR_VALOR_PRIMAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, Valores, ImprimirPrimal) \
void armazenarValorPrimalPorEstagioPorCenario##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_IdEstagio, const IdCenario a_idCenario) { \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) return; \
		if (!is_##Elem##_##Nome##_##Nro##_instanciada.at(a_TSS)) return; \
		if (idx_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial() > a_IdEstagio) return; \
		if (idx_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal() < a_IdEstagio) return; \
		Valores(DECLARA_ITERS) Valores(DECLARA_ITERS_CONST_##Nro##_10) \
		_IdEstagio = a_IdEstagio;\
		std::vector<int> estados(10, -1); estados.at(0) = 0; bool parada = false;\
		for (int i = 0; i <= 1000000; i++){ \
			if (varredurasIters_10(std::string(std::string(#Nome) + "_" + std::string(#Nro), idx_##Elem##_##Nome##_##Nro.at(a_TSS), estados, 1, parada Valores(_ITERS) INT_ARG_##Nro##_10)){\
				double conteudo = vetorEstagio.att(a_IdEstagio).getSolver(a_TSS)->getValorPrimal(idx_##Elem##_##Nome##_##Nro.at(a_TSS)Valores(AT_ITERS)AT_INT_##Nro##_10.at(1)); \
				addConteudoIters_11(vlrP_e_c_##Elem##_##Nome##_##Nro.at(a_TSS), conteudo Valores(_ITERS) INT_ARG_##Nro##_10 , a_idCenario); \
			} \
			if (parada) break; \
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorPrimalPorEstagioPorCenario" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_IdEstagio) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
}; \
void imprimirValorPrimalPorEstagioPorCenario##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados);

//
// ARMAZENAR VALOR DUAL POR ESTAGIO POR CENARIO
//

#define DECLARAR_ARMAZENAR_VALOR_DUAL_VarDecisao getReducedCost
#define DECLARAR_ARMAZENAR_VALOR_DUAL_EquLinear getMultiplicador
#define DECLARAR_ARMAZENAR_VALOR_DUAL_IneLinear getMultiplicador

#define DECLARAR_ARMAZENAR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, Valores, ImprimirDual) \
void armazenarValorDualPorEstagioPorCenario##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_IdEstagio, const IdCenario a_idCenario) { \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) return; \
		if (!is_##Elem##_##Nome##_##Nro##_instanciada.at(a_TSS)) return; \
		if (idx_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial() > a_IdEstagio) return; \
		if (idx_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal() < a_IdEstagio) return; \
		Valores(DECLARA_ITERS) Valores(DECLARA_ITERS_CONST_##Nro##_10) \
		_IdEstagio = a_IdEstagio;\
		std::vector<int> estados(10, -1); estados.at(0) = 0; bool parada = false;\
		for (int i = 0; i <= 1000000; i++){ \
			if (varredurasIters_10(std::string(std::string(#Nome) + "_" + std::string(#Nro), idx_##Elem##_##Nome##_##Nro.at(a_TSS), estados, 1, parada Valores(_ITERS) INT_ARG_##Nro##_10)){\
				double conteudo = vetorEstagio.att(a_IdEstagio).getSolver(a_TSS)->DECLARAR_ARMAZENAR_VALOR_DUAL_##Elem(idx_##Elem##_##Nome##_##Nro.at(a_TSS)Valores(AT_ITERS)AT_INT_##Nro##_10.at(1)); \
				addConteudoIters_11(vlrD_e_c_##Elem##_##Nome##_##Nro.at(a_TSS), conteudo Valores(_ITERS) INT_ARG_##Nro##_10 , a_idCenario); \
			} \
			if (parada) break; \
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorDualPorEstagioPorCenario" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_IdEstagio) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
}; \
void imprimirValorDualPorEstagioPorCenario##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados);


#define DECLARAR_ARMAZENAR_VALOR_PRIMAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, Valores, ImprimirPrimal) \
void armazenarValorPrimalPorEstagioPorCenario##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_IdEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) return; \
		if (!is_##Elem##_##Nome##_##Nro##_instanciada.at(a_TSS)) return; \
		if (idx_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial() > a_IdEstagio) return; \
		if (idx_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal() < a_IdEstagio) return; \
		Valores(DECLARA_ITERS) Valores(DECLARA_ITERS_CONST_##Nro##_10) \
		_IdEstagio = a_IdEstagio;\
		std::vector<int> estados(10, -1); estados.at(0) = 0; bool parada = false;\
		for (int i = 0; i <= 1000000; i++){ \
			if (varredurasIters_10(std::string(std::string(#Nome) + "_" + std::string(#Nro), idx_##Elem##_##Nome##_##Nro.at(a_TSS), estados, 1, parada Valores(_ITERS) INT_ARG_##Nro##_10)){\
				double conteudo = vetorEstagio.att(a_IdEstagio).getSolver(a_TSS)->getValorPrimal(idx_##Elem##_##Nome##_##Nro.at(a_TSS)Valores(AT_ITERS)AT_INT_##Nro##_10.at(1)); \
				addConteudoIters_12(vlrP_e_c_##Elem##_##Nome##_##Nro.at(a_TSS), conteudo Valores(_ITERS) INT_ARG_##Nro##_10 , a_idRealizacao, a_idCenario); \
			} \
			if (parada) break; \
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorPrimalPorEstagioPorCenario" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_IdEstagio) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
}; \
void imprimirValorPrimalPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados);

#define DECLARAR_ARMAZENAR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, Valores, ImprimirDual) \
void armazenarValorDualPorEstagioPorCenario##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_IdEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) return; \
		if (!is_##Elem##_##Nome##_##Nro##_instanciada.at(a_TSS)) return; \
		if (idx_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial() > a_IdEstagio) return; \
		if (idx_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal() < a_IdEstagio) return; \
		Valores(DECLARA_ITERS) Valores(DECLARA_ITERS_CONST_##Nro##_10) \
		_IdEstagio = a_IdEstagio;\
		std::vector<int> estados(10, -1); estados.at(0) = 0; bool parada = false;\
		for (int i = 0; i <= 1000000; i++){ \
			if (varredurasIters_10(std::string(std::string(#Nome) + "_" + std::string(#Nro), idx_##Elem##_##Nome##_##Nro.at(a_TSS), estados, 1, parada Valores(_ITERS) INT_ARG_##Nro##_10)){\
				double conteudo = vetorEstagio.att(a_IdEstagio).getSolver(a_TSS)->DECLARAR_ARMAZENAR_VALOR_DUAL_##Elem(idx_##Elem##_##Nome##_##Nro.at(a_TSS)Valores(AT_ITERS)AT_INT_##Nro##_10.at(1)); \
				addConteudoIters_12(vlrD_e_c_##Elem##_##Nome##_##Nro.at(a_TSS), conteudo Valores(_ITERS) INT_ARG_##Nro##_10 , a_idRealizacao, a_idCenario); \
			} \
			if (parada) break; \
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorDualPorEstagioPorCenario" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_IdEstagio) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
}; \
void imprimirValorDualPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados);


#define DECLARAR_ARMAZENAR_VALOR(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) \
DECLARAR_ARMAZENAR_VALOR_##Elem(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) \
void consolidarResultados##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_variavel, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);

#define DECLARAR_ARMAZENAR_VALOR_VarDecisao(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) \
DECLARAR_ARMAZENAR_VALOR_PRIMAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, Valores, ImprimirPrimal) \
DECLARAR_ARMAZENAR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, Valores, ImprimirDual) \
DECLARAR_ARMAZENAR_VALOR_PRIMAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, Valores, ImprimirPrimal) \
DECLARAR_ARMAZENAR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, Valores, ImprimirDual) 

#define DECLARAR_ARMAZENAR_VALOR_EquLinear(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) \
DECLARAR_ARMAZENAR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, Valores, ImprimirDual) \
DECLARAR_ARMAZENAR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, Valores, ImprimirDual) 

#define DECLARAR_ARMAZENAR_VALOR_IneLinear(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) \
DECLARAR_ARMAZENAR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, Valores, ImprimirDual) \
DECLARAR_ARMAZENAR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, Valores, ImprimirDual) 


#define DECLARAR_IMPRIMIR_VALOR_PRIMAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, Valores, ImprimirPrimal) \
void ModeloOtimizacao::imprimirValorPrimalPorEstagioPorCenario##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//" + std::string(#Elem)  + std::string(#Nome) + "_" + std::string(#Nro)); \
		const std::string nome_arquivo = std::string(std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro) + "_primal_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(##Nro, vlrP_e_c_##Elem##_##Nome##_##Nro.at(a_TSS))); \
		vlrP_e_c_##Elem##_##Nome##_##Nro.at(a_TSS) = \
		Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdCenario, double>>>>>>>>>>>();\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorPrimalPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};

#define DECLARAR_IMPRIMIR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, Valores, ImprimirDual) \
void ModeloOtimizacao::imprimirValorDualPorEstagioPorCenario##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//" + std::string(#Elem)  + std::string(#Nome) + "_" + std::string(#Nro)); \
		const std::string nome_arquivo = std::string(std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro) + "_dual_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(##Nro, vlrD_e_c_##Elem##_##Nome##_##Nro.at(a_TSS))); \
		vlrD_e_c_##Elem##_##Nome##_##Nro.at(a_TSS) = \
		Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdCenario, double>>>>>>>>>>>();\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorDualPorEstagioPorCenarioVarDecisao_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\


#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VarDecisao(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) DECLARAR_IMPRIMIR_VALOR_PRIMAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, Valores, ImprimirPrimal) DECLARAR_IMPRIMIR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, Valores, ImprimirDual)
#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_EquLinear(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) DECLARAR_IMPRIMIR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, Valores, ImprimirDual) 
#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_IneLinear(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) DECLARAR_IMPRIMIR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, Valores, ImprimirDual)

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_##Elem(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual)

#define IMPRIMIR_VALOR_PRIMAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, ImprimirDual) imprimirValorPrimalPorEstagioPorCenario##Elem##_##Nome##_##Nro(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);
#define IMPRIMIR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, ImprimirDual) imprimirValorDualPorEstagioPorCenario##Elem##_##Nome##_##Nro(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VarDecisao(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) IMPRIMIR_VALOR_PRIMAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, ImprimirPrimal) IMPRIMIR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, ImprimirDual)
#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_EquLinear(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual)  IMPRIMIR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, ImprimirDual)
#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_IneLinear(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual)  IMPRIMIR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, ImprimirDual)

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_##Elem(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual)

#define DECLARAR_IMPRIMIR_VALOR_PRIMAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, Valores, ImprimirPrimal) \
void ModeloOtimizacao::imprimirValorPrimalPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//" + std::string(#Elem)  + std::string(#Nome) + "_" + std::string(#Nro)); \
		const std::string nome_arquivo = std::string(std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro) + "_primal_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(##Nro, vlrP_e_c_r_##Elem##_##Nome##_##Nro.at(a_TSS))); \
		vlrP_e_c_r_##Elem##_##Nome##_##Nro.at(a_TSS) = \
		Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>();\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorPrimalPorEstagioPorCenarioPorRealizacao" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};

#define DECLARAR_IMPRIMIR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, Valores, ImprimirDual) \
void ModeloOtimizacao::imprimirValorDualPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) \
			return; \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//" + std::string(#Elem)  + std::string(#Nome) + "_" + std::string(#Nro)); \
		const std::string nome_arquivo = std::string(std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro) + "_dual_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(##Nro, vlrD_e_c_r_##Elem##_##Nome##_##Nro.at(a_TSS))); \
		vlrD_e_c_r_##Elem##_##Nome##_##Nro.at(a_TSS) = \
		Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>();\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorDualPorEstagioPorCenarioPorRealizacao" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\


#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VarDecisao(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) DECLARAR_IMPRIMIR_VALOR_PRIMAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, Valores, ImprimirPrimal) DECLARAR_IMPRIMIR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, Valores, ImprimirDual)
#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_EquLinear(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) DECLARAR_IMPRIMIR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, Valores, ImprimirDual) 
#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_IneLinear(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) DECLARAR_IMPRIMIR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, Valores, ImprimirDual)

#define DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_##Elem(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual)

#define IMPRIMIR_VALOR_PRIMAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, ImprimirDual) imprimirValorPrimalPorEstagioPorCenario##Elem##_##Nome##_##Nro(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);
#define IMPRIMIR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, ImprimirDual) imprimirValorDualPorEstagioPorCenario##Elem##_##Nome##_##Nro(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VarDecisao(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) IMPRIMIR_VALOR_PRIMAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, ImprimirPrimal) IMPRIMIR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, ImprimirDual)
#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_EquLinear(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual)  IMPRIMIR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, ImprimirDual)
#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_IneLinear(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual)  IMPRIMIR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, ImprimirDual)

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_##Elem(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual)


#define ARMAZENAR_VALOR_PRIMAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro) armazenarValorPrimalPorEstagioPorCenario##Elem##_##Nome##_##Nro(a_TSS, a_idEstagio, a_idCenario);
#define ARMAZENAR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro) armazenarValorDualPorEstagioPorCenario##Elem##_##Nome##_##Nro(a_TSS, a_idEstagio, a_idCenario);

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VarDecisao(Elem, Nome, Nro) ARMAZENAR_VALOR_PRIMAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro) ARMAZENAR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro)
#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_EquLinear(Elem, Nome, Nro) ARMAZENAR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro)
#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_IneLinear(Elem, Nome, Nro) ARMAZENAR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro)

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) ARMAZENAR_VALOR_PRIMAL_POR_ESTAGIO_POR_CENARIO_##Elem(Elem, Nome, Nro)

#define ARMAZENAR_VALOR_PRIMAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) armazenarValorPrimalPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao);
#define ARMAZENAR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) armazenarValorDualPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao);

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VarDecisao(Elem, Nome, Nro) ARMAZENAR_VALOR_PRIMAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro) ARMAZENAR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro)
#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_EquLinear(Elem, Nome, Nro) ARMAZENAR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro)
#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_IneLinear(Elem, Nome, Nro) ARMAZENAR_VALOR_DUAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro)

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) ARMAZENAR_VALOR_PRIMAL_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_##Elem(Elem, Nome, Nro)



#define DECLARAR_CONSOLIDAR_RESULTADOS(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual)  \
void ModeloOtimizacao::consolidarResultados##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_variavel, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (std::string(std::string(#Nome) + "_" + std::string(#Nro)) != a_variavel) \
			return; \
		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida(); \
		if (getboolFromChar(#ImprimirPrimal)){ \
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//" + std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro)); \
			const std::string nome_arquivo = std::string(std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro) + "_primal" + ".csv"); \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string(std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro) + "_primal_" + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoHorizontalCSV(nome_arquivo, lista_arquivos, ##Nro, true); \
		} \
		if (getboolFromChar(#ImprimirDual)){ \
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//" + std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro)); \
			const std::string nome_arquivo = std::string(std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro) + "_dual" + ".csv"); \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string(std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro) + "_dual_" + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoHorizontalCSV(nome_arquivo, lista_arquivos, ##Nro, true); \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::consolidarResultadosVarDecisao_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\


#define CONSOLIDAR_RESULTADOS(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) consolidarResultados##Elem##_##Nome##_##Nro(a_TSS, variavel_str, a_maiorIdProcesso, a_entradaSaidaDados);




/*
#define DECLARAR_VARIAVEL_DECISAO_3_TESTE(Nome, TIt1, TIt2, TIt3, ImprimirPrimal, ImprimirDual)   \
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
*/




#endif 
