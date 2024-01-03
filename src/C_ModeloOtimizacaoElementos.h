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

#define AT_CONST_1_10 .at(1).at(1).at(1).at(1).at(1).at(1).at(1).at(1).at(1)
#define AT_CONST_2_10 .at(1).at(1).at(1).at(1).at(1).at(1).at(1).at(1)
#define AT_CONST_3_10 .at(1).at(1).at(1).at(1).at(1).at(1).at(1)
#define AT_CONST_4_10 .at(1).at(1).at(1).at(1).at(1).at(1)
#define AT_CONST_5_10 .at(1).at(1).at(1).at(1).at(1)
#define AT_CONST_6_10 .at(1).at(1).at(1).at(1)
#define AT_CONST_7_10 .at(1).at(1).at(1)
#define AT_CONST_8_10 .at(1).at(1)
#define AT_CONST_9_10 .at(1)
#define AT_CONST_10_10 

#define ARGS_ITERS(nIt, TIt) , a_##TIt##_##nIt 
#define ARGS_CONST_1_10  , 1, 1, 1, 1, 1, 1, 1, 1, 1
#define ARGS_CONST_2_10  , 1, 1, 1, 1, 1, 1, 1, 1
#define ARGS_CONST_3_10  , 1, 1, 1, 1, 1, 1, 1
#define ARGS_CONST_4_10  , 1, 1, 1, 1, 1, 1
#define ARGS_CONST_5_10  , 1, 1, 1, 1, 1
#define ARGS_CONST_6_10  , 1, 1, 1, 1
#define ARGS_CONST_7_10  , 1, 1, 1
#define ARGS_CONST_8_10  , 1, 1
#define ARGS_CONST_9_10  , 1
#define ARGS_CONST_10_10 


#define DECLARA_TITERS(nIt, TIt) TIt(),
#define DECLARA_TITERS_CONST_1_10 int(),  int(),  int(),  int(),  int(),  int(),  int(),  int(),  int(), 
#define DECLARA_TITERS_CONST_2_10 int(),  int(),  int(),  int(),  int(),  int(),  int(),  int(), 
#define DECLARA_TITERS_CONST_3_10 int(),  int(),  int(),  int(),  int(),  int(),  int(), 
#define DECLARA_TITERS_CONST_4_10 int(),  int(),  int(),  int(),  int(),  int(), 
#define DECLARA_TITERS_CONST_5_10 int(),  int(),  int(),  int(),  int(), 
#define DECLARA_TITERS_CONST_6_10 int(),  int(),  int(),  int(), 
#define DECLARA_TITERS_CONST_7_10 int(),  int(),  int(), 
#define DECLARA_TITERS_CONST_8_10 int(),  int(), 
#define DECLARA_TITERS_CONST_9_10 int(), 
#define DECLARA_TITERS_CONST_10_10 


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

#define DECLARAR_ISVAR_VarDecisao  true
#define DECLARAR_ISVAR_EquLinear  false
#define DECLARAR_ISVAR_IneLinear  false

#define DECLARAR_CONDICAO_RETORNO_ImprimirPrimal_nao return;
#define DECLARAR_CONDICAO_RETORNO_ImprimirPrimal_sim
#define DECLARAR_CONDICAO_RETORNO_ImprimirDual_nao return;
#define DECLARAR_CONDICAO_RETORNO_ImprimirDual_sim
#define DECLARAR_CONDICAO_RETORNO_NormDual_nao return 1.0;
#define DECLARAR_CONDICAO_RETORNO_NormDual_sim

//
//  Chamada Geral Declaracao
//

#define DECLARAR_METODOS_ELEMENTO(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual, NormDual)\
std::vector<int> is_##Elem##_##Nome##_##Nro##_instanciada = std::vector<int>(TipoSubproblemaSolver_Excedente, 0); \
std::vector< Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> idx_##Elem##_##Nome##_##Nro = \
std::vector< Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> (TipoSubproblemaSolver_Excedente,\
             Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>()); \
std::vector< Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> normD_##Elem##_##Nome##_##Nro = \
std::vector< Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> (TipoSubproblemaSolver_Excedente,\
             Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>()); \
std::vector< Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> vlrP_e_c_##Elem##_##Nome##_##Nro = \
std::vector< Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> (TipoSubproblemaSolver_Excedente,\
             Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>());\
std::vector< Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> vlrP_e_c_r_##Elem##_##Nome##_##Nro = \
std::vector< Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> (TipoSubproblemaSolver_Excedente,\
             Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>());\
std::vector< Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> vlrD_e_c_##Elem##_##Nome##_##Nro =\
std::vector< Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> (TipoSubproblemaSolver_Excedente,\
             Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>());\
std::vector< Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> vlrD_e_c_r_##Elem##_##Nome##_##Nro =\
std::vector< Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>> (TipoSubproblemaSolver_Excedente,\
             Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>());\
std::string getNome##Elem##_##Nome(const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS) ){\
if (vetorEstagio.att(a_IdEstagio_1).getSolver(a_TSS)->isNomeSimplificado()) \
	return std::string(std::string(#Elem) + std::string(#Nome) Valores(GET_STRING_ELEMENTO)); \
else \
	return std::string(std::string(#Elem) + std::string(#Nome) Valores(GET_FULL_STRING_ELEMENTO)); \
};\
int get##Elem##_##Nome(const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS)){ \
	try{ return int(idx_##Elem##_##Nome##_##Nro.at(a_TSS) Valores(AT_A_ITERS) AT_CONST_##Nro##_10 .at(IdRealizacao_1).at(IdCenario_1)); } \
	catch (const std::exception& erro) { throw std::invalid_argument("get" + std::string(#Elem) +  "_" + std::string(#Nome) + "(" Valores(GET_FULL_STRING_ELEMENTO) + "): \n" + std::string(erro.what())); } \
}; \
int get##Elem##_##Nome##seExistir(const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS)){ \
	try{ \
		if (!is_##Elem##_##Nome##_##Nro##_instanciada.at(a_TSS)) return -1; \
		double conteudo = -1; \
		getConteudoIters_12(idx_##Elem##_##Nome##_##Nro.at(a_TSS), conteudo Valores(ARGS_ITERS) ARGS_CONST_##Nro##_10, IdRealizacao_1, IdCenario_1); \
		return int(conteudo); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("get" + std::string(#Elem) + "_" + std::string(#Nome) + "seExistir(" Valores(GET_FULL_STRING_ELEMENTO) + "): \n" + std::string(erro.what())); } \
}; \
int add##Elem##_##Nome(const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS) DECLARAR_ADD_ELEMENTO_##Elem##_1){ \
	try{ \
		if (get##Elem##_##Nome##seExistir(a_TSS Valores(ARGS_ITERS)) > -1) \
			throw std::invalid_argument("Conteudo ja existente."); \
		double id##Elem = double(vetorEstagio.att(a_IdEstagio_1).getSolver(a_TSS)-> DECLARAR_ADD_ELEMENTO_##Elem##_2 getNome##Elem##_##Nome(a_TSS Valores(ARGS_ITERS)))); \
		addConteudoIters_12(idx_##Elem##_##Nome##_##Nro.at(a_TSS), id##Elem Valores(ARGS_ITERS) ARGS_CONST_##Nro##_10, IdRealizacao_1, IdCenario_1 ); \
		if (!is_##Elem##_##Nome##_##Nro##_instanciada.at(a_TSS)){ \
			is_##Elem##_##Nome##_##Nro##_instanciada.at(a_TSS) = true;\
			lista_##Elem##_instanciadas.at(a_TSS).push_back(std::string(std::string(#Nome) + "_" + std::string(#Nro))); \
		} \
		return int(id##Elem); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("add" + std::string(#Elem) + "_" + std::string(#Nome) + "(" Valores(GET_FULL_STRING_ELEMENTO) DECLARAR_ADD_ELEMENTO_##Elem##_3 + "): \n" + std::string(erro.what())); } \
}; \
void setNormalizacaoDual##Elem##_##Nome(const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS) , const double a_normMultiplicacao){ \
	try{ \
		if (!getboolFromChar(#NormDual)) throw std::invalid_argument("Elemento nao passivel de normalizacao dual."); \
		if (get##Elem##_##Nome##seExistir(a_TSS Valores(ARGS_ITERS)) == -1) throw std::invalid_argument("Elemento nao existente."); \
		addConteudoIters_12(normD_##Elem##_##Nome##_##Nro.at(a_TSS), a_normMultiplicacao Valores(ARGS_ITERS) ARGS_CONST_##Nro##_10, IdRealizacao_1, IdCenario_1); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("setNormalizacaoDual" + std::string(#Elem) + "_" + std::string(#Nome) + "(" Valores(GET_FULL_STRING_ELEMENTO) + "): \n" + std::string(erro.what())); } \
}; \
double getNormalizacaoDual##Elem##_##Nome(const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS)){ \
	try{ \
		DECLARAR_CONDICAO_RETORNO_NormDual_##NormDual \
		double conteudo = 1.0; \
		getConteudoIters_12(normD_##Elem##_##Nome##_##Nro.at(a_TSS), conteudo Valores(ARGS_ITERS) ARGS_CONST_##Nro##_10, IdRealizacao_1, IdCenario_1); \
		return conteudo; \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("getNormalizacaoDual" + std::string(#Elem) + "_" + std::string(#Nome) + "(" Valores(GET_FULL_STRING_ELEMENTO) + "): \n" + std::string(erro.what())); } \
}; \
void armazenarValorPrimalPorEstagioPorCenario##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_IdEstagio_1, const IdCenario a_idCenario) { \
	try{ \
		DECLARAR_CONDICAO_RETORNO_ImprimirPrimal_##ImprimirPrimal \
		if (!is_##Elem##_##Nome##_##Nro##_instanciada.at(a_TSS)) return; \
		varredurasIters_##Nro(vlrP_e_c_##Elem##_##Nome##_##Nro.at(a_TSS), idx_##Elem##_##Nome##_##Nro.at(a_TSS), normD_##Elem##_##Nome##_##Nro.at(a_TSS), a_TSS, a_IdEstagio_1, a_idIteracao, DECLARAR_ISVAR_##Elem, true, Valores(DECLARA_TITERS) DECLARA_TITERS_CONST_##Nro##_10 IdRealizacao_Nenhum, a_idCenario);\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorPrimalPorEstagioPorCenario" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_idIteracao) + "," + getFullString(a_IdEstagio_1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
}; \
void imprimirValorPrimalPorEstagioPorCenario##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados);\
void armazenarValorDualPorEstagioPorCenario##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_IdEstagio_1, const IdCenario a_idCenario) { \
	try{ \
		DECLARAR_CONDICAO_RETORNO_ImprimirDual_##ImprimirDual \
		if (!is_##Elem##_##Nome##_##Nro##_instanciada.at(a_TSS)) return; \
		varredurasIters_##Nro(vlrD_e_c_##Elem##_##Nome##_##Nro.at(a_TSS), idx_##Elem##_##Nome##_##Nro.at(a_TSS), normD_##Elem##_##Nome##_##Nro.at(a_TSS), a_TSS, a_IdEstagio_1, a_idIteracao, DECLARAR_ISVAR_##Elem, false, Valores(DECLARA_TITERS) DECLARA_TITERS_CONST_##Nro##_10 IdRealizacao_Nenhum, a_idCenario);\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorDualPorEstagioPorCenario" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_IdEstagio_1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
}; \
void imprimirValorDualPorEstagioPorCenario##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados);\
void armazenarValorPrimalPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_IdEstagio_1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
	try{ \
		DECLARAR_CONDICAO_RETORNO_ImprimirPrimal_##ImprimirPrimal \
		if (!is_##Elem##_##Nome##_##Nro##_instanciada.at(a_TSS)) return; \
		varredurasIters_##Nro(vlrP_e_c_r_##Elem##_##Nome##_##Nro.at(a_TSS), idx_##Elem##_##Nome##_##Nro.at(a_TSS), normD_##Elem##_##Nome##_##Nro.at(a_TSS), a_TSS, a_IdEstagio_1, a_idIteracao, DECLARAR_ISVAR_##Elem, true, Valores(DECLARA_TITERS) DECLARA_TITERS_CONST_##Nro##_10 a_idRealizacao, a_idCenario);\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorPrimalPorEstagioPorCenarioPorRealizacao" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_IdEstagio_1) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
}; \
void imprimirValorPrimalPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void armazenarValorDualPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_IdEstagio_1, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) { \
	try{ \
		DECLARAR_CONDICAO_RETORNO_ImprimirDual_##ImprimirDual \
		if (!is_##Elem##_##Nome##_##Nro##_instanciada.at(a_TSS)) return; \
		varredurasIters_##Nro(vlrD_e_c_r_##Elem##_##Nome##_##Nro.at(a_TSS), idx_##Elem##_##Nome##_##Nro.at(a_TSS), normD_##Elem##_##Nome##_##Nro.at(a_TSS), a_TSS, a_IdEstagio_1, a_idIteracao, DECLARAR_ISVAR_##Elem, false, Valores(DECLARA_TITERS) DECLARA_TITERS_CONST_##Nro##_10 a_idRealizacao, a_idCenario);\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorDualPorEstagioPorCenarioPorRealizacao" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_IdEstagio_1) + "," + getString(a_idCenario) + "," + getString(a_idRealizacao) + "): \n" + std::string(erro.what())); } \
}; \
void imprimirValorDualPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void consolidarResultados##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_variavel, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);

//
// ARMAZENAR VALOR POR ESTAGIO POR CENARIO
//

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual, NormDual) \
armazenarValorPrimalPorEstagioPorCenario##Elem##_##Nome##_##Nro(a_TSS, a_idIteracao, a_idEstagio, a_idCenario);\
armazenarValorDualPorEstagioPorCenario##Elem##_##Nome##_##Nro(a_TSS, a_idIteracao, a_idEstagio, a_idCenario);

//
// ARMAZENAR VALOR POR ESTAGIO POR CENARIO POR REALIZACAO
//

#define ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual, NormDual) \
armazenarValorPrimalPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao);\
armazenarValorDualPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao);

//
// IMPRIMIR VALOR PRIMAL POR ESTAGIO POR CENARIO
//

#define DECLARAR_IMPRIMIR_VALOR(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual, NormDual) \
void ModeloOtimizacao::imprimirValorPrimalPorEstagioPorCenario##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		DECLARAR_CONDICAO_RETORNO_ImprimirPrimal_##ImprimirPrimal \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//" + std::string(#Elem)  + std::string(#Nome) + "_" + std::string(#Nro)); \
		const std::string nome_arquivo = std::string(std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro) + "_primal_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(Nro, vlrP_e_c_##Elem##_##Nome##_##Nro.at(a_TSS))); \
		vlrP_e_c_##Elem##_##Nome##_##Nro.at(a_TSS) = \
		Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>();\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorPrimalPorEstagioPorCenario" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\
void ModeloOtimizacao::imprimirValorDualPorEstagioPorCenario##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		DECLARAR_CONDICAO_RETORNO_ImprimirDual_##ImprimirDual \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//" + std::string(#Elem)  + std::string(#Nome) + "_" + std::string(#Nro)); \
		const std::string nome_arquivo = std::string(std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro) + "_dual_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(Nro, vlrD_e_c_##Elem##_##Nome##_##Nro.at(a_TSS))); \
		vlrD_e_c_##Elem##_##Nome##_##Nro.at(a_TSS) = \
		Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>();\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorDualPorEstagioPorCenario" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\
void ModeloOtimizacao::imprimirValorPrimalPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		DECLARAR_CONDICAO_RETORNO_ImprimirPrimal_##ImprimirPrimal \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//" + std::string(#Elem)  + std::string(#Nome) + "_" + std::string(#Nro)); \
		const std::string nome_arquivo = std::string(std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro) + "_primal_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(12, vlrP_e_c_r_##Elem##_##Nome##_##Nro.at(a_TSS))); \
		vlrP_e_c_r_##Elem##_##Nome##_##Nro.at(a_TSS) = \
		Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>();\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorPrimalPorEstagioPorCenarioPorRealizacao" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\
void ModeloOtimizacao::imprimirValorDualPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		DECLARAR_CONDICAO_RETORNO_ImprimirDual_##ImprimirDual \
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//" + std::string(#Elem)  + std::string(#Nome) + "_" + std::string(#Nro)); \
		const std::string nome_arquivo = std::string(std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro) + "_dual_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV_SmartEnupla(nome_arquivo, SmartEnupla<int,int>::getDadosAsString(12, vlrD_e_c_r_##Elem##_##Nome##_##Nro.at(a_TSS))); \
		vlrD_e_c_r_##Elem##_##Nome##_##Nro.at(a_TSS) = \
		Valores(DECLARAR_ALOCAR_ENUPLA_ITER) DECLARAR_ALOCAR_ENUPLA_CONST_##Nro SmartEnupla<IdRealizacao, SmartEnupla<IdCenario, double>>>>>>>>>>>>();\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorDualPorEstagioPorCenarioPorRealizacao" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual, NormDual) \
imprimirValorPrimalPorEstagioPorCenario##Elem##_##Nome##_##Nro(TSS, getFullString(a_idProcesso), a_entradaSaidaDados); \
imprimirValorDualPorEstagioPorCenario##Elem##_##Nome##_##Nro(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);

#define IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual, NormDual) \
imprimirValorPrimalPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(TSS, getFullString(a_idProcesso), a_entradaSaidaDados); \
imprimirValorDualPorEstagioPorCenarioPorRealizacao##Elem##_##Nome##_##Nro(TSS, getFullString(a_idProcesso), a_entradaSaidaDados);

//
// CONSOLIDAR RESULTADOS
//

#define DECLARAR_CONSOLIDAR_RESULTADOS(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual, NormDual)  \
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
			a_entradaSaidaDados.imprimirConsolidacaoHorizontalCSV(nome_arquivo, lista_arquivos, Nro, true); \
		} \
		if (getboolFromChar(#ImprimirDual)){ \
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//" + std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro)); \
			const std::string nome_arquivo = std::string(std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro) + "_dual" + ".csv"); \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string(std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro) + "_dual_" + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoHorizontalCSV(nome_arquivo, lista_arquivos, Nro, true); \
		} \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::consolidarResultados" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\


#define CONSOLIDAR_RESULTADOS(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual, NormDual) consolidarResultados##Elem##_##Nome##_##Nro(a_TSS, Elem##_str, a_maiorIdProcesso, a_entradaSaidaDados);



#endif 
