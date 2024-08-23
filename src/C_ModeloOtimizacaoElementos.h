#ifndef MODELO_OTIMIZACAO_ELEMENTOS
#define MODELO_OTIMIZACAO_ELEMENTOS


//
// DEFINICOES GERAIS
//

#define ITERS_ARGS(nIt, TIt) , const TIt a_##TIt##_##nIt

#define GET_STRING_TIPO_ELEMENTO(nIt, TIt)  + getStringTipo(TIt()) + ";"

#define GET_STRING_ELEMENTO(nIt, TIt)  + getString(a_##TIt##_##nIt) + ";"
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

#define DECLARAR_GET_ELEMENTO_VarDecisao getReducedCost
#define DECLARAR_GET_ELEMENTO_EquLinear getMultiplicador
#define DECLARAR_GET_ELEMENTO_IneLinear getMultiplicador

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
SmartEnupla<TipoSubproblemaSolver, IdEstagio> idEstagioIni_vlrP_##Elem##_##Nome##_##Nro;\
SmartEnupla<TipoSubproblemaSolver, IdEstagio> idEstagioIni_vlrD_##Elem##_##Nome##_##Nro;\
SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<std::string>>> name_##Elem##_##Nome##_##Nro;\
SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<int>>>         indx_##Elem##_##Nome##_##Nro;\
SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<double>>>      nrmD_##Elem##_##Nome##_##Nro;\
SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<std::string>>> vlrP_##Elem##_##Nome##_##Nro;\
SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<std::string>>> vlrD_##Elem##_##Nome##_##Nro;\
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
	return std::string(std::string(#Nome) + ";" Valores(GET_STRING_ELEMENTO)); \
};\
void alocVlr##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio){ \
	try{\
		if (DECLARAR_ISVAR_##Elem){\
			if (vlrP_##Elem##_##Nome##_##Nro.size() == 0)	\
				vlrP_##Elem##_##Nome##_##Nro = SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<std::string>>>(TipoSubproblemaSolver(1), std::vector<SmartEnupla<IdEstagio, std::vector<std::string>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<IdEstagio, std::vector<std::string>>())); \
			if (vlrP_##Elem##_##Nome##_##Nro.at(a_TSS).size() == 0) {	\
				const IdEstagio stageIni = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio()); \
				const int num_stage = int(getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio()) - stageIni) + 1; \
				vlrP_##Elem##_##Nome##_##Nro.at(a_TSS) = SmartEnupla<IdEstagio, std::vector<std::string>>(stageIni, std::vector<std::vector<std::string>>(num_stage, std::vector<std::string>())); \
			}\
			if (idEstagioIni_vlrP_##Elem##_##Nome##_##Nro.size() == 0) {\
				idEstagioIni_vlrP_##Elem##_##Nome##_##Nro = SmartEnupla<TipoSubproblemaSolver, IdEstagio>(TipoSubproblemaSolver(1), std::vector<IdEstagio>(TipoSubproblemaSolver_Excedente, IdEstagio_Nenhum));\
				idEstagioIni_vlrP_##Elem##_##Nome##_##Nro.at(a_TSS) = a_idEstagio;\
			 }\
			if (a_idEstagio < idEstagioIni_vlrP_##Elem##_##Nome##_##Nro.at(a_TSS)){idEstagioIni_vlrP_##Elem##_##Nome##_##Nro.at(a_TSS) = a_idEstagio;}\
		}\
		if (vlrD_##Elem##_##Nome##_##Nro.size() == 0)	\
			vlrD_##Elem##_##Nome##_##Nro = SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<std::string>>>(TipoSubproblemaSolver(1), std::vector<SmartEnupla<IdEstagio, std::vector<std::string>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<IdEstagio, std::vector<std::string>>())); \
		if (vlrD_##Elem##_##Nome##_##Nro.at(a_TSS).size() == 0) {	\
			const IdEstagio stageIni = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio()); \
			const int num_stage = int(getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio()) - stageIni) + 1; \
			vlrD_##Elem##_##Nome##_##Nro.at(a_TSS) = SmartEnupla<IdEstagio, std::vector<std::string>>(stageIni, std::vector<std::vector<std::string>>(num_stage, std::vector<std::string>())); \
		}\
		if (idEstagioIni_vlrD_##Elem##_##Nome##_##Nro.size() == 0) {\
			idEstagioIni_vlrD_##Elem##_##Nome##_##Nro = SmartEnupla<TipoSubproblemaSolver, IdEstagio>(TipoSubproblemaSolver(1), std::vector<IdEstagio>(TipoSubproblemaSolver_Excedente, IdEstagio_Nenhum));\
			idEstagioIni_vlrD_##Elem##_##Nome##_##Nro.at(a_TSS) = a_idEstagio;\
		 }\
		if (a_idEstagio < idEstagioIni_vlrD_##Elem##_##Nome##_##Nro.at(a_TSS)) { idEstagioIni_vlrD_##Elem##_##Nome##_##Nro.at(a_TSS) = a_idEstagio; }\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("alocVlr" + std::string(#Elem) +  "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); } \
}; \
void aloc##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio){ \
	try{\
		if (name_##Elem##_##Nome##_##Nro.size() == 0){\
			lista_##Elem##_instanciadas.at(a_TSS).push_back(std::string(std::string(#Nome) + "_" + std::string(#Nro))); \
			name_##Elem##_##Nome##_##Nro = SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<std::string>>> (TipoSubproblemaSolver(1), std::vector<SmartEnupla<IdEstagio, std::vector<std::string>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<IdEstagio, std::vector<std::string>>()));\
			indx_##Elem##_##Nome##_##Nro = SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<int>>>         (TipoSubproblemaSolver(1), std::vector<SmartEnupla<IdEstagio, std::vector<int>>>        (TipoSubproblemaSolver_Excedente, SmartEnupla<IdEstagio, std::vector<int>>()));\
		}\
		if (name_##Elem##_##Nome##_##Nro.at(a_TSS).size() == 0){\
			const IdEstagio stageIni = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio()); \
			const int num_stage = int(getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio()) - stageIni) + 1;\
			name_##Elem##_##Nome##_##Nro.at(a_TSS) = SmartEnupla<IdEstagio, std::vector<std::string>>(stageIni, std::vector<std::vector<std::string>>(num_stage, std::vector<std::string>()));\
			indx_##Elem##_##Nome##_##Nro.at(a_TSS) = SmartEnupla<IdEstagio, std::vector<int>>        (stageIni, std::vector<std::vector<int>>        (num_stage, std::vector<int>())); \
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("aloc" + std::string(#Elem) +  "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); } \
}; \
int getPos##Elem##_##Nome(const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS)){ \
	try{\
		const int pos = getPos##Elem##_##Nome##seExistir(a_TSS Valores(ARGS_ITERS));\
		if (pos < 0) {throw std::invalid_argument("Invalid element");}\
		return pos;\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("getPos" + std::string(#Elem) +  "_" + std::string(#Nome) + "(" Valores(GET_FULL_STRING_ELEMENTO) + "): \n" + std::string(erro.what())); } \
}; \
int getPos##Elem##_##Nome##seExistir(const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS)){ \
	try{ \
		if (name_##Elem##_##Nome##_##Nro.size() == 0){ return -1;}\
		if (name_##Elem##_##Nome##_##Nro.at(a_TSS).size() == 0){ return -1;}\
		if (name_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size() == 0){ return -1;}\
		const std::string name = getNome##Elem##_##Nome(a_TSS Valores(ARGS_ITERS));\
		return findStringSensNoVetorReturnPos(name, name_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1)); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("getPos" + std::string(#Elem) + "_" + std::string(#Nome) + "seExistir(" Valores(GET_FULL_STRING_ELEMENTO) + "): \n" + std::string(erro.what())); } \
}; \
int get##Elem##_##Nome(const bool a_isAlocMode, const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS)){ \
	try{\
		const int indx = get##Elem##_##Nome##seExistir(a_isAlocMode, a_TSS Valores(ARGS_ITERS));\
		if (indx < 0) {throw std::invalid_argument("Invalid element");}\
		return indx;\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("get" + std::string(#Elem) +  "_" + std::string(#Nome) + "(" + Valores(GET_FULL_STRING_ELEMENTO) + "): \n" + std::string(erro.what())); } \
}; \
int get##Elem##_##Nome##seExistir(const bool a_isAlocMode, const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS)){ \
	try{ \
		const int pos = getPos##Elem##_##Nome##seExistir(a_TSS Valores(ARGS_ITERS));\
		if (pos < 0) { return -1;}\
		if (a_isAlocMode) { return 0; };\
		return indx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos);\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("get" + std::string(#Elem) + "_" + std::string(#Nome) + "seExistir(" + Valores(GET_FULL_STRING_ELEMENTO) + "): \n" + std::string(erro.what())); } \
}; \
int add##Elem##_##Nome(const bool a_isAlocMode, const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS) DECLARAR_ADD_ELEMENTO_##Elem##_1){ \
	try{ \
		if (get##Elem##_##Nome##seExistir(a_isAlocMode, a_TSS Valores(ARGS_ITERS)) > -1) \
			throw std::invalid_argument("Conteudo ja existente."); \
		aloc##Elem##_##Nome##_##Nro(a_TSS, a_IdEstagio_1);\
		name_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).push_back(getNome##Elem##_##Nome(a_TSS Valores(ARGS_ITERS)));\
		const int indx = vetorEstagio.at(a_IdEstagio_1).getSolver(a_TSS)-> DECLARAR_ADD_ELEMENTO_##Elem##_2 getNome##Elem##_##Nome(a_TSS Valores(ARGS_ITERS))); \
		indx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).push_back(indx);\
		if (nrmD_##Elem##_##Nome##_##Nro.size() > 0) { nrmD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).push_back(1.0);}\
		return indx; \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("add" + std::string(#Elem) + "_" + std::string(#Nome) + "(" + Valores(GET_FULL_STRING_ELEMENTO) DECLARAR_ADD_ELEMENTO_##Elem##_3 + "): \n" + std::string(erro.what())); } \
}; \
void setNormalizacaoDual##Elem##_##Nome(const bool a_isAlocMode, const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS) , const double a_normMultiplicacao){ \
	try{ \
		if (nrmD_##Elem##_##Nome##_##Nro.size() == 0) { \
			nrmD_##Elem##_##Nome##_##Nro = SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<double>>>      (TipoSubproblemaSolver(1), std::vector<SmartEnupla<IdEstagio, std::vector<double>>>     (TipoSubproblemaSolver_Excedente, SmartEnupla<IdEstagio, std::vector<double>>())); \
			const IdEstagio stageIni = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio()); \
			const int num_stage = int(getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio()) - stageIni) + 1;\
			nrmD_##Elem##_##Nome##_##Nro.at(a_TSS) = SmartEnupla<IdEstagio, std::vector<double>>(stageIni, std::vector<std::vector<double>>(num_stage, std::vector<double>())); \
		}\
		if (nrmD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size() == 0) { nrmD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1) = std::vector<double>(name_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size(), 1.0);}\
		const int pos = getPos##Elem##_##Nome(a_TSS Valores(ARGS_ITERS));\
		nrmD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos) = a_normMultiplicacao;\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("setNormalizacaoDual" + std::string(#Elem) + "_" + std::string(#Nome) + "(" + Valores(GET_FULL_STRING_ELEMENTO) + "): \n" + std::string(erro.what())); } \
}; \
double getNormalizacaoDual##Elem##_##Nome(const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS)){ \
	try{ \
		if (nrmD_##Elem##_##Nome##_##Nro.size() == 0) { return 1.0; }\
		if (nrmD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size() == 0) { return 1.0;}\
		const int pos = getPos##Elem##_##Nome(a_TSS Valores(ARGS_ITERS));\
		return nrmD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos);\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("getNormalizacaoDual" + std::string(#Elem) + "_" + std::string(#Nome) + "(" Valores(GET_FULL_STRING_ELEMENTO) + "): \n" + std::string(erro.what())); } \
}; \
void armazenarValorPrimal##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_IdEstagio_1, const IdCenario a_idCenario, IdRealizacao a_idReal) { \
	try{ \
		if (DECLARAR_ISVAR_##Elem){\
			if (indx_##Elem##_##Nome##_##Nro.size() == 0) { return; }\
			if (indx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size() == 0) { return; }\
			alocVlr##Elem##_##Nome##_##Nro(a_TSS, a_IdEstagio_1); \
			if (a_idReal == IdRealizacao_Nenhum){\
				const Periodo period = getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, a_IdEstagio_1, Periodo());\
				a_idReal = getElementoMatriz(getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico()), AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, period, IdRealizacao());\
			} \
			if (vlrP_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size() == 0) \
				vlrP_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).push_back(std::string(std::string(#Elem) + ";" Valores(GET_STRING_TIPO_ELEMENTO) + "VlrPrimal;" + "IdCenario;" + "IdRealizacao"));\
			for (int pos = 0; pos < int(indx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size()); pos++) {	\
				const int idx = indx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos);\
				vlrP_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).push_back(std::string(name_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos) + getString(vetorEstagio.at(a_IdEstagio_1).getSolver(a_TSS)->getValorPrimal(idx)) + ";" + getString(a_idCenario) + ";" + getString(a_idReal))); \
			}\
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorPrimal" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_idIteracao) + "," + getFullString(a_IdEstagio_1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
}; \
void armazenarValorDual##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_IdEstagio_1, const IdCenario a_idCenario, IdRealizacao a_idReal) { \
	try{ \
		DECLARAR_CONDICAO_RETORNO_ImprimirDual_##ImprimirDual \
		if (indx_##Elem##_##Nome##_##Nro.size() == 0) { return; }\
		if (indx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size() == 0) { return; }\
		alocVlr##Elem##_##Nome##_##Nro(a_TSS, a_IdEstagio_1); \
		if (a_idReal == IdRealizacao_Nenhum){\
			const Periodo period = getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, a_IdEstagio_1, Periodo());\
			a_idReal = getElementoMatriz(getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico()), AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, period, IdRealizacao());\
		} \
		if (vlrD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size() == 0) \
			vlrD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).push_back(std::string(std::string(#Elem) + ";" Valores(GET_STRING_TIPO_ELEMENTO) + "VlrDual;" + "IdCenario;" + "IdRealizacao"));\
		bool normD = false;\
		if (nrmD_##Elem##_##Nome##_##Nro.size() > 0){\
			if (nrmD_##Elem##_##Nome##_##Nro.at(a_TSS).size() > 0){\
				if (nrmD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size() > 0)\
						normD = true;;\
			}\
		}\
		for (int pos = 0; pos < int(indx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size()); pos++) {	\
			const int idx = indx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos);\
			if (normD)\
				vlrD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).push_back(std::string(name_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos) + getString(vetorEstagio.at(a_IdEstagio_1).getSolver(a_TSS)->DECLARAR_GET_ELEMENTO_##Elem(idx) * nrmD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos)) + ";" + getString(a_idCenario) + ";" + getString(a_idReal))); \
			else\
				vlrD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).push_back(std::string(name_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos) + getString(vetorEstagio.at(a_IdEstagio_1).getSolver(a_TSS)->DECLARAR_GET_ELEMENTO_##Elem(idx)) + ";" + getString(a_idCenario) + ";" + getString(a_idReal))); \
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorDual" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_IdEstagio_1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
}; \
void imprimirValorPrimal##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void imprimirValorDual##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void consolidarResultados##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_variavel, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);



//
// ARMAZENAR VALOR POR ESTAGIO POR CENARIO POR REALIZACAO
//

#define ARMAZENAR_VALOR(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual, NormDual) \
armazenarValorPrimal##Elem##_##Nome##_##Nro(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao);\
armazenarValorDual##Elem##_##Nome##_##Nro(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao);

//
// IMPRIMIR VALOR PRIMAL POR ESTAGIO POR CENARIO
//

#define DECLARAR_IMPRIMIR_VALOR(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual, NormDual) \
void ModeloOtimizacao::imprimirValorPrimal##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (DECLARAR_ISVAR_##Elem){\
			if (vlrP_##Elem##_##Nome##_##Nro.size() == 0){ return;}\
			if (vlrP_##Elem##_##Nome##_##Nro.at(a_TSS).size() == 0){return;}\
			if (a_idEstagio == idEstagioIni_vlrP_##Elem##_##Nome##_##Nro.at(a_TSS)) {a_entradaSaidaDados.setAppendArquivo(false);}\
			else {a_entradaSaidaDados.setAppendArquivo(true);}\
			a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//" + std::string(#Elem)  + std::string(#Nome) + "_" + std::string(#Nro)); \
			const std::string nome_arquivo = std::string(std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro) + "_primal_" + a_nome_arquivo + ".csv"); \
			a_entradaSaidaDados.imprimirArquivoCSV(nome_arquivo, vlrP_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_idEstagio), true); \
			vlrP_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_idEstagio) = std::vector<string>();\
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorPrimal" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\
void ModeloOtimizacao::imprimirValorDual##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		DECLARAR_CONDICAO_RETORNO_ImprimirDual_##ImprimirDual \
		if (vlrD_##Elem##_##Nome##_##Nro.size() == 0){ return;}\
		if (vlrD_##Elem##_##Nome##_##Nro.at(a_TSS).size() == 0){return;}\
		if (a_idEstagio == idEstagioIni_vlrD_##Elem##_##Nome##_##Nro.at(a_TSS)) { a_entradaSaidaDados.setAppendArquivo(false); }\
		else { a_entradaSaidaDados.setAppendArquivo(true); }\
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//" + std::string(#Elem)  + std::string(#Nome) + "_" + std::string(#Nro)); \
		const std::string nome_arquivo = std::string(std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro) + "_dual_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV(nome_arquivo, vlrD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_idEstagio), true); \
		vlrD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_idEstagio) = std::vector<string>();\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorDual" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR(Elem, Nome, Nro, Valores, ImprimirPrimal, ImprimirDual, NormDual) \
imprimirValorPrimal##Elem##_##Nome##_##Nro(TSS, a_idEstagio, getFullString(a_idProcesso), a_entradaSaidaDados); \
imprimirValorDual##Elem##_##Nome##_##Nro(TSS, a_idEstagio, getFullString(a_idProcesso), a_entradaSaidaDados);

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
