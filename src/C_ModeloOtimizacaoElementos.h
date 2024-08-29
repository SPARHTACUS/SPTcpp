#ifndef MODELO_OTIMIZACAO_ELEMENTOS
#define MODELO_OTIMIZACAO_ELEMENTOS


//
// DEFINICOES GERAIS
//

#define ITERS_ARGS(nIt, TIt) , const TIt a_##TIt##_##nIt
#define ARGS_ITERS(nIt, TIt) , a_##TIt##_##nIt 

#define GET_STRING_TIPO_ELEMENTO(nIt, TIt)  + getStringTipo(TIt()) + ";"

#define GET_STRING_ELEMENTO(nIt, TIt)  + getString(a_##TIt##_##nIt) + ";"
#define GET_FULL_STRING_ELEMENTO(nIt, TIt)  + "," + getFullString(a_##TIt##_##nIt)

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

#define DECLARAR_TESTE_DUAL_VarDecisao true
#define DECLARAR_TESTE_DUAL_EquLinear true
#define DECLARAR_TESTE_DUAL_IneLinear vlrD != 0.0

#define DECLARAR_ISVAR_VarDecisao  true
#define DECLARAR_ISVAR_EquLinear  false
#define DECLARAR_ISVAR_IneLinear  false

#define DECLARAR_CONDICAO_RETORNO_ImprimirDual_nao return;
#define DECLARAR_CONDICAO_RETORNO_ImprimirDual_sim


//
//  Chamada Geral Declaracao
//

#define DECLARAR_METODOS_ELEMENTO(Elem, Nome, Nro, Valores, ImprimirDual)\
SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<std::string>>> name_##Elem##_##Nome##_##Nro;\
SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<int>>>         indx_##Elem##_##Nome##_##Nro;\
SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<double>>>      nrmD_##Elem##_##Nome##_##Nro;\
SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<std::string>>> vlrP_##Elem##_##Nome##_##Nro;\
SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<std::string>>> vlrD_##Elem##_##Nome##_##Nro;\
SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<std::string>>> vlrF_##Elem##_##Nome##_##Nro;\
bool isF_##Elem##_##Nome##_##Nro = false;\
bool isPrintFw_##Elem##_##Nome##_##Nro = true;\
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
			if (isF_##Elem##_##Nome##_##Nro){\
				if (vlrF_##Elem##_##Nome##_##Nro.size() == 0)	\
					vlrF_##Elem##_##Nome##_##Nro = SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<std::string>>>(TipoSubproblemaSolver(1), std::vector<SmartEnupla<IdEstagio, std::vector<std::string>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<IdEstagio, std::vector<std::string>>())); \
				if (vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).size() == 0) {	\
					const IdEstagio stageIni = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio()); \
					const int num_stage = int(getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio()) - stageIni) + 1; \
					vlrF_##Elem##_##Nome##_##Nro.at(a_TSS) = SmartEnupla<IdEstagio, std::vector<std::string>>(stageIni, std::vector<std::vector<std::string>>(num_stage, std::vector<std::string>())); \
				}\
			}\
		}\
		if (vlrD_##Elem##_##Nome##_##Nro.size() == 0)	\
			vlrD_##Elem##_##Nome##_##Nro = SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<std::string>>>(TipoSubproblemaSolver(1), std::vector<SmartEnupla<IdEstagio, std::vector<std::string>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<IdEstagio, std::vector<std::string>>())); \
		if (vlrD_##Elem##_##Nome##_##Nro.at(a_TSS).size() == 0) {	\
			const IdEstagio stageIni = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio()); \
			const int num_stage = int(getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio()) - stageIni) + 1; \
			vlrD_##Elem##_##Nome##_##Nro.at(a_TSS) = SmartEnupla<IdEstagio, std::vector<std::string>>(stageIni, std::vector<std::vector<std::string>>(num_stage, std::vector<std::string>())); \
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("alocVlr" + std::string(#Elem) +  "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); } \
}; \
void aloc##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio){ \
	try{\
		if (name_##Elem##_##Nome##_##Nro.size() == 0){\
			const std::string name_str(#Nome);\
			if ((name_str.find("FINF") != std::string::npos) || (name_str.find("FSUP") != std::string::npos)){\
				isF_##Elem##_##Nome##_##Nro = true;\
			}\
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
int get##Elem##_##Nome(const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS)){ \
	try{\
		const int indx = get##Elem##_##Nome##seExistir(a_TSS Valores(ARGS_ITERS));\
		if (indx < 0) {throw std::invalid_argument("Invalid element");}\
		return indx;\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("get" + std::string(#Elem) +  "_" + std::string(#Nome) + "(" + Valores(GET_FULL_STRING_ELEMENTO) + "): \n" + std::string(erro.what())); } \
}; \
int get##Elem##_##Nome##seExistir(const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS)){ \
	try{ \
		const int pos = getPos##Elem##_##Nome##seExistir(a_TSS Valores(ARGS_ITERS));\
		if (pos < 0) { return -1;}\
		return indx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos);\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("get" + std::string(#Elem) + "_" + std::string(#Nome) + "seExistir(" + Valores(GET_FULL_STRING_ELEMENTO) + "): \n" + std::string(erro.what())); } \
}; \
int add##Elem##_##Nome(const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS) DECLARAR_ADD_ELEMENTO_##Elem##_1){ \
	try{ \
		if (get##Elem##_##Nome##seExistir(a_TSS Valores(ARGS_ITERS)) > -1) \
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
void setNormalizacaoDual##Elem##_##Nome(const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS) , const double a_normMultiplicacao){ \
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
				isPrintFw_##Elem##_##Nome##_##Nro = true;\
				const Periodo period = getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, a_IdEstagio_1, Periodo());\
				a_idReal = getElementoMatriz(getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico()), AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, period, IdRealizacao());\
			} \
			else { isPrintFw_##Elem##_##Nome##_##Nro = false; }\
			if (vlrP_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size() == 0) \
				vlrP_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).push_back(std::string(std::string(#Elem) + ";" Valores(GET_STRING_TIPO_ELEMENTO) + "VlrPrimal;" + "IdCenario;" + "IdRealizacao"));\
			for (int pos = 0; pos < int(indx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size()); pos++) {	\
				const double vlrP = vetorEstagio.at(a_IdEstagio_1).getSolver(a_TSS)->getValorPrimal(indx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos));\
				vlrP_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).push_back(std::string(name_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos) + getString(vlrP) + ";" + getString(a_idCenario) + ";" + getString(a_idReal))); \
				if (isF_##Elem##_##Nome##_##Nro){\
					if (vlrP > vetorEstagio.at(a_IdEstagio_1).getSolver(a_TSS)->getToleranciaViabilidade()){\
						if (vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size() == 0) \
							vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).push_back(std::string(std::string(#Elem) + ";" Valores(GET_STRING_TIPO_ELEMENTO) + "VlrPrimal;" + "IdCenario;" + "IdRealizacao"));\
						vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).push_back(std::string(name_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos) + getString(vlrP) + ";" + getString(a_idCenario) + ";" + getString(a_idReal))); \
					}\
				}\
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
			isPrintFw_##Elem##_##Nome##_##Nro = true;\
			const Periodo period = getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, a_IdEstagio_1, Periodo());\
			a_idReal = getElementoMatriz(getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico()), AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, period, IdRealizacao());\
		} \
		else { isPrintFw_##Elem##_##Nome##_##Nro = false; }\
		bool normD = false;\
		if (nrmD_##Elem##_##Nome##_##Nro.size() > 0){\
			if (nrmD_##Elem##_##Nome##_##Nro.at(a_TSS).size() > 0){\
				if (nrmD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size() > 0)\
						normD = true;;\
			}\
		}\
		for (int pos = 0; pos < int(indx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size()); pos++) {	\
			double vlrD = vetorEstagio.at(a_IdEstagio_1).getSolver(a_TSS)->DECLARAR_GET_ELEMENTO_##Elem(indx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos));\
			if (DECLARAR_TESTE_DUAL_##Elem){\
				if (vlrD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size() == 0) \
					vlrD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).push_back(std::string(std::string(#Elem) + ";" Valores(GET_STRING_TIPO_ELEMENTO) + "VlrDual;" + "IdCenario;" + "IdRealizacao"));\
				if (normD)\
					vlrD *= nrmD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos); \
				vlrD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).push_back(std::string(name_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos) + getString(vlrD) + ";" + getString(a_idCenario) + ";" + getString(a_idReal))); \
			}\
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

#define ARMAZENAR_VALOR(Elem, Nome, Nro, Valores, ImprimirDual) \
armazenarValorPrimal##Elem##_##Nome##_##Nro(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao);\
armazenarValorDual##Elem##_##Nome##_##Nro(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao);

//
// IMPRIMIR VALOR PRIMAL POR ESTAGIO POR CENARIO
//

#define DECLARAR_IMPRIMIR_VALOR(Elem, Nome, Nro, Valores, ImprimirDual) \
void ModeloOtimizacao::imprimirValorPrimal##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (DECLARAR_ISVAR_##Elem){\
			const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida(); \
			if (vlrP_##Elem##_##Nome##_##Nro.size() == 0){ return;}\
			if (vlrP_##Elem##_##Nome##_##Nro.at(a_TSS).size() == 0){return;}\
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//" + std::string(#Elem)  + std::string(#Nome)); \
			a_entradaSaidaDados.setAppendArquivo(true);\
			if (isPrintFw_##Elem##_##Nome##_##Nro) { \
				for (IdEstagio idEstagio = vlrP_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial(); idEstagio <= vlrP_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal(); idEstagio++){\
					if (vlrP_##Elem##_##Nome##_##Nro.at(a_TSS).at(idEstagio).size() > 0){\
						if (a_idEstagio == idEstagio)\
							a_entradaSaidaDados.setAppendArquivo(false); \
						break;\
					}\
				} \
			}\
			else if (!isPrintFw_##Elem##_##Nome##_##Nro){\
				for (IdEstagio idEstagio = vlrP_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal(); idEstagio >= vlrP_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial(); idEstagio--){\
					if (vlrP_##Elem##_##Nome##_##Nro.at(a_TSS).at(idEstagio).size() > 0){\
						if (a_idEstagio == idEstagio)\
							a_entradaSaidaDados.setAppendArquivo(false); \
						break;\
					}\
				} \
			}\
			const std::string nome_arquivo = std::string(std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro) + "_primal_" + a_nome_arquivo + ".csv"); \
			a_entradaSaidaDados.imprimirArquivoCSV(nome_arquivo, vlrP_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_idEstagio), true); \
			vlrP_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_idEstagio) = std::vector<string>();\
			if (vlrF_##Elem##_##Nome##_##Nro.size() == 0){ return;}\
			if (vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).size() == 0){return;}\
			if (vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_idEstagio).size() == 0){return;}\
			a_entradaSaidaDados.setDiretorioSaida(diretorio); \
			a_entradaSaidaDados.setAppendArquivo(true);\
			if (isPrintFw_##Elem##_##Nome##_##Nro) { \
				for (IdEstagio idEstagio = vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial(); idEstagio <= vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal(); idEstagio++){\
					if (vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).at(idEstagio).size() > 0){\
						if (a_idEstagio == idEstagio)\
							a_entradaSaidaDados.setAppendArquivo(false); \
						break;\
					}\
				} \
			}\
			else if (!isPrintFw_##Elem##_##Nome##_##Nro){\
				for (IdEstagio idEstagio = vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal(); idEstagio >= vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial(); idEstagio--){\
					if (vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).at(idEstagio).size() > 0){\
						if (a_idEstagio == idEstagio)\
							a_entradaSaidaDados.setAppendArquivo(false); \
						break;\
					}\
				} \
			}\
			a_entradaSaidaDados.imprimirArquivoCSV(nome_arquivo, vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_idEstagio), true); \
			vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_idEstagio) = std::vector<string>();\
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorPrimal" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\
void ModeloOtimizacao::imprimirValorDual##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		DECLARAR_CONDICAO_RETORNO_ImprimirDual_##ImprimirDual \
		if (vlrD_##Elem##_##Nome##_##Nro.size() == 0){ return;}\
		if (vlrD_##Elem##_##Nome##_##Nro.at(a_TSS).size() == 0){return;}\
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//" + std::string(#Elem)  + std::string(#Nome)); \
		a_entradaSaidaDados.setAppendArquivo(true);\
		if (isPrintFw_##Elem##_##Nome##_##Nro) { \
			for (IdEstagio idEstagio = vlrD_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial(); idEstagio <= vlrD_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal(); idEstagio++){\
				if (vlrD_##Elem##_##Nome##_##Nro.at(a_TSS).at(idEstagio).size() > 0){\
					if (a_idEstagio == idEstagio)\
						a_entradaSaidaDados.setAppendArquivo(false); \
					break;\
				}\
			} \
		}\
		else if (!isPrintFw_##Elem##_##Nome##_##Nro){\
			for (IdEstagio idEstagio = vlrD_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal(); idEstagio >= vlrD_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial(); idEstagio--){\
				if (vlrD_##Elem##_##Nome##_##Nro.at(a_TSS).at(idEstagio).size() > 0){\
					if (a_idEstagio == idEstagio)\
						a_entradaSaidaDados.setAppendArquivo(false); \
					break;\
				}\
			} \
		}\
		const std::string nome_arquivo = std::string(std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro) + "_dual_" + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV(nome_arquivo, vlrD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_idEstagio), true); \
		vlrD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_idEstagio) = std::vector<string>();\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirValorDual" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR(Elem, Nome, Nro, Valores, ImprimirDual) \
imprimirValorPrimal##Elem##_##Nome##_##Nro(TSS, a_idEstagio, getFullString(a_idProcesso), a_entradaSaidaDados); \
imprimirValorDual##Elem##_##Nome##_##Nro(TSS, a_idEstagio, getFullString(a_idProcesso), a_entradaSaidaDados);

//
// CONSOLIDAR RESULTADOS
//

#define DECLARAR_CONSOLIDAR_RESULTADOS(Elem, Nome, Nro, Valores, ImprimirDual)  \
void ModeloOtimizacao::consolidarResultados##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_variavel, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (std::string(std::string(#Nome) + "_" + std::string(#Nro)) != a_variavel) \
			return; \
		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida(); \
		if (DECLARAR_ISVAR_##Elem){\
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//" + std::string(#Elem) + std::string(#Nome)); \
			const std::string nome_arquivo = std::string(std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro) + "_primal" + ".csv"); \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string(std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro) + "_primal_" + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoVerticalCSV(nome_arquivo, lista_arquivos, true, true); \
			if (vlrF_##Elem##_##Nome##_##Nro.size() > 0){\
				if (vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).size() > 0){\
					a_entradaSaidaDados.setDiretorioSaida(diretorio); \
					a_entradaSaidaDados.imprimirConsolidacaoVerticalCSV(nome_arquivo, lista_arquivos, true, true); \
				}\
			}\
		} \
		DECLARAR_CONDICAO_RETORNO_ImprimirDual_##ImprimirDual \
		a_entradaSaidaDados.setDiretorioSaida(diretorio + "//" + std::string(#Elem) + std::string(#Nome)); \
		const std::string nome_arquivo = std::string(std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro) + "_dual" + ".csv"); \
		std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
		for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
			lista_arquivos.at(getRank(idProcesso)) = std::string(std::string(#Elem) + std::string(#Nome) + "_" + std::string(#Nro) + "_dual_" + getFullString(idProcesso) + ".csv"); \
		a_entradaSaidaDados.imprimirConsolidacaoVerticalCSV(nome_arquivo, lista_arquivos, true, true); \
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::consolidarResultados" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\


#define CONSOLIDAR_RESULTADOS(Elem, Nome, Nro, Valores, ImprimirDual) consolidarResultados##Elem##_##Nome##_##Nro(a_TSS, Elem##_str, a_maiorIdProcesso, a_entradaSaidaDados);



#endif 
