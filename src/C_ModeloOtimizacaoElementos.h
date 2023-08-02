#ifndef MODELO_OTIMIZACAO_ELEMENTOS
#define MODELO_OTIMIZACAO_ELEMENTOS


//
// DEFINICOES GERAIS
//

#define ITERS_ARGS(nIt, TIt) , const TIt a_##TIt##_##nIt
#define GET_STRING_ELEMENTO(nIt, TIt)  + "," + getString(a_##TIt##_##nIt)
#define GET_FULL_STRING_ELEMENTO(nIt, TIt)  + "," + getFullString(a_##TIt##_##nIt)

#define AT_A_ITERS(nIt, TIt).at(a_##TIt##_##nIt)
#define AT_ITERS(nIt, TIt).at(_##TIt##_##nIt)

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

#define ARGS_ITERS(nIt, TIt) , a_##TIt##_##nIt 
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

#define _ITERS(nIt, TIt) , _##TIt##_##nIt 
#define DECLARA_ITERS(nIt, TIt) TIt _##TIt##_##nIt;
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

#define DECLARAR_ALOCAR_ENUPLA_ITER(nIt, TIt) SmartEnupla<TIt, 

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



#define DECLARAR_ADD_ELEMENTO_VarDecisao_1 , const double a_valor_inferior, const double a_valor_superior, const double a_valor_objetivo
#define DECLARAR_ADD_ELEMENTO_EquLinear_1
#define DECLARAR_ADD_ELEMENTO_IneLinear_1

#define DECLARAR_ADD_ELEMENTO_VarDecisao_2 addVar(a_valor_inferior, a_valor_superior, a_valor_objetivo,
#define DECLARAR_ADD_ELEMENTO_EquLinear_2 addConstrIgual(
#define DECLARAR_ADD_ELEMENTO_IneLinear_2 addConstrMaior(

#define DECLARAR_ADD_ELEMENTO_VarDecisao_3 + "," + getFullString(a_valor_inferior) + "," + getFullString(a_valor_superior) + "," + getFullString(a_valor_objetivo)
#define DECLARAR_ADD_ELEMENTO_EquLinear_3 
#define DECLARAR_ADD_ELEMENTO_IneLinear_3 

#define DECLARAR_ARMAZENAR_VALOR_DUAL_VarDecisao getReducedCost
#define DECLARAR_ARMAZENAR_VALOR_DUAL_EquLinear getMultiplicador
#define DECLARAR_ARMAZENAR_VALOR_DUAL_IneLinear getMultiplicador

//
//  Chamada Geral Declaracao
//

#define DECLARAR_METODOS_ELEMENTO(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual)\
std::vector<int> is_##Elem##_##Nome##_##Nro##_instanciada = std::vector<int>(TipoSubproblemaSolver_Excedente, 0); \
std::vector< Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<int, int>>>>>>>>>>>> idx_##Elem##_##Nome##_##Nro = \
std::vector< Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<int, int>>>>>>>>>>>> (TipoSubproblemaSolver_Excedente,\
             Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<int, int>>>>>>>>>>>()); \
std::vector< Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdCenario, double>>>>>>>>>>>> vlrP_e_c_##Elem##_##Nome##_##Nro = \
std::vector< Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdCenario, double>>>>>>>>>>>> (TipoSubproblemaSolver_Excedente,\
             Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdCenario, double>>>>>>>>>>>());\
std::vector< Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> vlrP_e_c_r_##Elem##_##Nome##_##Nro = \
std::vector< Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> (TipoSubproblemaSolver_Excedente,\
             Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>());\
std::vector< Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdCenario, double>>>>>>>>>>>> vlrD_e_c_##Elem##_##Nome##_##Nro =\
std::vector< Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdCenario, double>>>>>>>>>>>> (TipoSubproblemaSolver_Excedente,\
             Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdCenario, double>>>>>>>>>>>());\
std::vector< Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> vlrD_e_c_r_##Elem##_##Nome##_##Nro =\
std::vector< Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> (TipoSubproblemaSolver_Excedente,\
             Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>());\
template<typename TIt1, typename TIt2, typename TIt3> \
void alocar##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const TIt1 a_it1, const SmartEnupla<TIt2, SmartEnupla<TIt3, bool>> &a_enupla_alocacao){ \
	try{ \
		alocConteudoIter(idx_##Elem##_##Nome##_##Nro.at(a_TSS), a_it1);\
		for (TIt2 it2 = a_enupla_alocacao.getIteradorInicial(); it2 <= a_enupla_alocacao.getIteradorFinal(); a_enupla_alocacao.incrementarIterador(it2)){ \
			alocConteudoIter(idx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_it1), it2);\
			for (TIt3 it3 = a_enupla_alocacao.at(it2).getIteradorInicial(); it3 <= a_enupla_alocacao.at(it2).getIteradorFinal(); a_enupla_alocacao.at(it2).incrementarIterador(it3)) \
				alocConteudoIter(idx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_it1).at(it2), it3);\
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("alocar" + std::string(#Elem) + "_" + std::string(#Nome) + std::string(#Nro) + "(a_enupla_alocacao): \n" + std::string(erro.what())); } \
}; \
std::string getNome##Elem##_##Nome(const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS) ){\
if (vetorEstagio.att(a_IdEstagio_1).getSolver(a_TSS)->isNomeSimplificado()) \
	return std::string(std::string(#Elem) + std::string(#Nome) Valores(GET_STRING_ELEMENTO)); \
else \
	return std::string(std::string(#Elem) + std::string(#Nome) Valores(GET_FULL_STRING_ELEMENTO)); \
};\
int get##Elem##_##Nome(const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS)){ \
	try{ return idx_##Elem##_##Nome##_##Nro.at(a_TSS) Valores(AT_A_ITERS) AT_CONST_##Nro##_11; } \
	catch (const std::exception& erro) { throw std::invalid_argument("get" + std::string(#Elem) +  "_" + std::string(#Nome) + "(" Valores(GET_FULL_STRING_ELEMENTO) + "): \n" + std::string(erro.what())); } \
}; \
int get##Elem##_##Nome##seExistir(const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS)){ \
	try{ \
		if (!is_##Elem##_##Nome##_##Nro##_instanciada.at(a_TSS)) return -1; \
		int conteudo = -1; \
		getConteudoIters_11(idx_##Elem##_##Nome##_##Nro.at(a_TSS), conteudo Valores(ARGS_ITERS) ARGS_CONST_##Nro##_11); \
		return conteudo; \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("get" + std::string(#Elem) + "_" + std::string(#Nome) + "seExistir(" Valores(GET_FULL_STRING_ELEMENTO) + "): \n" + std::string(erro.what())); } \
}; \
int add##Elem##_##Nome(const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS) DECLARAR_ADD_ELEMENTO_##Elem##_1){ \
	try{ \
		if (get##Elem##_##Nome##seExistir(a_TSS Valores(ARGS_ITERS)) > -1) \
			throw std::invalid_argument("Conteudo ja existente."); \
		int id##Elem = vetorEstagio.att(a_IdEstagio_1).getSolver(a_TSS)-> DECLARAR_ADD_ELEMENTO_##Elem##_2 getNome##Elem##_##Nome(a_TSS Valores(ARGS_ITERS))); \
		addConteudoIters_11(idx_##Elem##_##Nome##_##Nro.at(a_TSS), id##Elem Valores(ARGS_ITERS) ARGS_CONST_##Nro##_11); \
		if (!is_##Elem##_##Nome##_##Nro##_instanciada.at(a_TSS)){ \
			is_##Elem##_##Nome##_##Nro##_instanciada.at(a_TSS) = true;\
			lista_##Elem##_instanciadas.at(a_TSS).push_back(std::string(std::string(#Nome) + "_" + std::string(#Nro))); \
		} \
		return id##Elem; \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("add" + std::string(#Elem) + "_" + std::string(#Nome) + "(" Valores(GET_FULL_STRING_ELEMENTO) DECLARAR_ADD_ELEMENTO_##Elem##_3 + "): \n" + std::string(erro.what())); } \
}; \
void armazenarValorPrimalPorEstagioPorCenario##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_IdEstagio_1, const IdCenario a_idCenario) { \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) return; \
		if (!is_##Elem##_##Nome##_##Nro##_instanciada.at(a_TSS)) return; \
		if (idx_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial() > a_IdEstagio_1) return; \
		if (idx_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal() < a_IdEstagio_1) return; \
		Valores(DECLARA_ITERS) DECLARA_ITERS_CONST_##Nro##_10\
		_IdEstagio_1 = a_IdEstagio_1;\
		std::vector<int> estados(10, -1); estados.at(0) = 0; bool parada = false;\
		for (int i = 0; i <= 1000000; i++){ \
			if (varredurasIters_10(std::string(std::string(#Nome) + "_" + std::string(#Nro)), idx_##Elem##_##Nome##_##Nro.at(a_TSS), estados, 1, parada Valores(_ITERS) INT_ARG_##Nro##_10)){\
				double conteudo = vetorEstagio.att(a_IdEstagio_1).getSolver(a_TSS)->getValorPrimal(idx_##Elem##_##Nome##_##Nro.at(a_TSS) Valores(AT_ITERS) AT_INT_##Nro##_10.at(1)); \
				addConteudoIters_11(vlrP_e_c_##Elem##_##Nome##_##Nro.at(a_TSS), conteudo Valores(_ITERS) INT_ARG_##Nro##_10 , a_idCenario); \
			} \
			if (parada) break; \
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorPrimalPorEstagioPorCenario" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_IdEstagio_1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
}; \
void imprimirValorPrimalPorEstagioPorCenario##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados);\
void armazenarValorDualPorEstagioPorCenario##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_IdEstagio_1, const IdCenario a_idCenario) { \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) return; \
		if (!is_##Elem##_##Nome##_##Nro##_instanciada.at(a_TSS)) return; \
		if (idx_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial() > a_IdEstagio_1) return; \
		if (idx_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal() < a_IdEstagio_1) return; \
		Valores(DECLARA_ITERS) DECLARA_ITERS_CONST_##Nro##_10 \
		_IdEstagio_1 = a_IdEstagio_1;\
		std::vector<int> estados(10, -1); estados.at(0) = 0; bool parada = false;\
		for (int i = 0; i <= 1000000; i++){ \
			if (varredurasIters_10(std::string(std::string(#Nome) + "_" + std::string(#Nro)), idx_##Elem##_##Nome##_##Nro.at(a_TSS), estados, 1, parada Valores(_ITERS) INT_ARG_##Nro##_10)){\
				double conteudo = vetorEstagio.att(a_IdEstagio_1).getSolver(a_TSS)->DECLARAR_ARMAZENAR_VALOR_DUAL_##Elem(idx_##Elem##_##Nome##_##Nro.at(a_TSS)Valores(AT_ITERS)AT_INT_##Nro##_10.at(1)); \
				addConteudoIters_11(vlrD_e_c_##Elem##_##Nome##_##Nro.at(a_TSS), conteudo Valores(_ITERS) INT_ARG_##Nro##_10 , a_idCenario); \
			} \
			if (parada) break; \
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorDualPorEstagioPorCenario" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_IdEstagio_1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
}; \
void imprimirValorDualPorEstagioPorCenario##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados);\
void armazenarValorPrimalPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_IdEstagio_1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
	try{ \
		if (!getboolFromChar(#ImprimirPrimal)) return; \
		if (!is_##Elem##_##Nome##_##Nro##_instanciada.at(a_TSS)) return; \
		if (idx_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial() > a_IdEstagio_1) return; \
		if (idx_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal() < a_IdEstagio_1) return; \
		Valores(DECLARA_ITERS) DECLARA_ITERS_CONST_##Nro##_10 \
		_IdEstagio_1 = a_IdEstagio_1;\
		std::vector<int> estados(10, -1); estados.at(0) = 0; bool parada = false;\
		for (int i = 0; i <= 1000000; i++){ \
			if (varredurasIters_10(std::string(std::string(#Nome) + "_" + std::string(#Nro)), idx_##Elem##_##Nome##_##Nro.at(a_TSS), estados, 1, parada Valores(_ITERS) INT_ARG_##Nro##_10)){\
				double conteudo = vetorEstagio.att(a_IdEstagio_1).getSolver(a_TSS)->getValorPrimal(idx_##Elem##_##Nome##_##Nro.at(a_TSS)Valores(AT_ITERS)AT_INT_##Nro##_10.at(1)); \
				addConteudoIters_12(vlrP_e_c_r_##Elem##_##Nome##_##Nro.at(a_TSS), conteudo Valores(_ITERS) INT_ARG_##Nro##_10 , a_idRealizacao, a_idCenario); \
			} \
			if (parada) break; \
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorPrimalPorEstagioPorCenarioPorRealizacao" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_IdEstagio_1) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
}; \
void imprimirValorPrimalPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void armazenarValorDualPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_IdEstagio_1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
	try{ \
		if (!getboolFromChar(#ImprimirDual)) return; \
		if (!is_##Elem##_##Nome##_##Nro##_instanciada.at(a_TSS)) return; \
		if (idx_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial() > a_IdEstagio_1) return; \
		if (idx_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal() < a_IdEstagio_1) return; \
		Valores(DECLARA_ITERS) DECLARA_ITERS_CONST_##Nro##_10 \
		_IdEstagio_1 = a_IdEstagio_1;\
		std::vector<int> estados(10, -1); estados.at(0) = 0; bool parada = false;\
		for (int i = 0; i <= 1000000; i++){ \
			if (varredurasIters_10(std::string(std::string(#Nome) + "_" + std::string(#Nro)), idx_##Elem##_##Nome##_##Nro.at(a_TSS), estados, 1, parada Valores(_ITERS) INT_ARG_##Nro##_10)){\
				double conteudo = vetorEstagio.att(a_IdEstagio_1).getSolver(a_TSS)->DECLARAR_ARMAZENAR_VALOR_DUAL_##Elem(idx_##Elem##_##Nome##_##Nro.at(a_TSS)Valores(AT_ITERS)AT_INT_##Nro##_10.at(1)); \
				addConteudoIters_12(vlrD_e_c_r_##Elem##_##Nome##_##Nro.at(a_TSS), conteudo Valores(_ITERS) INT_ARG_##Nro##_10 , a_idRealizacao, a_idCenario); \
			} \
			if (parada) break; \
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorDualPorEstagioPorCenarioPorRealizacao" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_IdEstagio_1) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
}; \
void imprimirValorDualPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void consolidarResultados##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_variavel, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);

//
// ARMAZENAR VALOR POR ESTAGIO POR CENARIO
//

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) \
armazenarValorPrimalPorEstagioPorCenario##Elem##_##Nome##_##Nro(a_TSS, a_idEstagio, a_idCenario);\
armazenarValorDualPorEstagioPorCenario##Elem##_##Nome##_##Nro(a_TSS, a_idEstagio, a_idCenario);

//
// ARMAZENAR VALOR POR ESTAGIO POR CENARIO POR REALIZACAO
//

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) \
armazenarValorPrimalPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao);\
armazenarValorDualPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(a_TSS, a_idEstagio, a_idCenario, a_idRealizacao);

//
// IMPRIMIR VALOR PRIMAL POR ESTAGIO POR CENARIO
//

#define DECLARAR_IMPRIMIR_VALOR(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) \
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
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorPrimalPorEstagioPorCenario" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\
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
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorDualPorEstagioPorCenario" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\
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
};\
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

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) \
imprimirValorPrimalPorEstagioPorCenario##Elem##_##Nome##_##Nro(TSS, getFullString(a_idProcesso), a_entradaSaidaDados); \
imprimirValorDualPorEstagioPorCenario##Elem##_##Nome##_##Nro(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) \
imprimirValorPrimalPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(TSS, getFullString(a_idProcesso), a_entradaSaidaDados); \
imprimirValorDualPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);

//
// CONSOLIDAR RESULTADOS
//

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
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::consolidarResultados" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\


#define CONSOLIDAR_RESULTADOS(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual) consolidarResultados##Elem##_##Nome##_##Nro(a_TSS, Elem##_str, a_maiorIdProcesso, a_entradaSaidaDados);



#endif 
