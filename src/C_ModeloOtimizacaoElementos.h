#ifndef MODELO_OTIMIZACAO_ELEMENTOS
#define MODELO_OTIMIZACAO_ELEMENTOS


//
// DEFINICOES GERAIS
//

#define ITERS_ARGS(nIt, TIt) , const TIt a_##TIt##_##nIt
#define ARGS_ITERS(nIt, TIt) , a_##TIt##_##nIt 

#define GET_STRING_TIPO_ELEMENTO(nIt, TIt)  + getStringTipo(TIt()) + ";"
#define GET_STRING_TIPO_ELEMENTO2(nIt, TIt)  + getStringTipo(TIt()) + "_"

#define GET_STRING_ELEMENTO(nIt, TIt)  + getString(a_##TIt##_##nIt) + ";"
#define GET_STRING_ELEMENTO2(nIt, TIt)  + getString(a_##TIt##_##nIt) + ","
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

#define DECLARAR_TESTE_DUAL_VarDecisao true
#define DECLARAR_TESTE_DUAL_EquLinear true
#define DECLARAR_TESTE_DUAL_IneLinear std::sqrt(vlrD * vlrD) > vetorEstagio.at(a_IdEstagio_1).getSolver(a_TSS)->getToleranciaOtimalidade()

#define DECLARAR_ISVAR_VarDecisao  true
#define DECLARAR_ISVAR_EquLinear  false
#define DECLARAR_ISVAR_IneLinear  false


//
//  Chamada Geral Declaracao
//

#define DECLARAR_METODOS_ELEMENTO(Elem, Nome, Nro, Valores)\
SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<std::string>>> name_##Elem##_##Nome##_##Nro;\
SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<int>>>         indx_##Elem##_##Nome##_##Nro;\
SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<double>>>      nrmD_##Elem##_##Nome##_##Nro;\
SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<std::string>>> vlrV_##Elem##_##Nome##_##Nro;\
SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<std::string>>> vlrF_##Elem##_##Nome##_##Nro;\
SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<std::string>>> vlrC_##Elem##_##Nome##_##Nro;\
bool isF_##Elem##_##Nome##_##Nro = false;\
bool isPrintFw_##Elem##_##Nome##_##Nro = true;\
std::string getNome##Elem##_##Nome(const TipoSubproblemaSolver a_TSS Valores(ITERS_ARGS) ){\
	std::string name = std::string(std::string(#Nome) + "," Valores(GET_STRING_ELEMENTO2)); \
	name.erase(name.size() -1);\
	return name;\
};\
void alocVlrF##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio){ \
	try{\
		if (DECLARAR_ISVAR_##Elem){\
			if (isF_##Elem##_##Nome##_##Nro){\
				if (vlrF_##Elem##_##Nome##_##Nro.size() == 0)	\
					vlrF_##Elem##_##Nome##_##Nro = SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<std::string>>>(TipoSubproblemaSolver(1), std::vector<SmartEnupla<IdEstagio, std::vector<std::string>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<IdEstagio, std::vector<std::string>>())); \
				if (vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).size() == 0) {	\
					vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).addElemento(a_idEstagio, std::vector<std::string>());\
				}\
				else if (a_idEstagio < vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial()) {	\
					for (IdEstagio idEstagio = IdEstagio(vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial() - 1); idEstagio >= a_idEstagio; idEstagio--)\
						vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).addElemento(idEstagio, std::vector<std::string>());\
				}\
				else if (vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal() < a_idEstagio) {	\
					for (IdEstagio idEstagio = IdEstagio(vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal() + 1); idEstagio <= a_idEstagio; idEstagio++)\
						vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).addElemento(idEstagio, std::vector<std::string>());\
				}\
			}\
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("alocVlr" + std::string(#Elem) +  "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); } \
}; \
void alocVlrVar##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio){ \
	try{\
		if (DECLARAR_ISVAR_##Elem){\
			if (vlrV_##Elem##_##Nome##_##Nro.size() == 0)	\
				vlrV_##Elem##_##Nome##_##Nro = SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<std::string>>>(TipoSubproblemaSolver(1), std::vector<SmartEnupla<IdEstagio, std::vector<std::string>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<IdEstagio, std::vector<std::string>>())); \
			if (vlrV_##Elem##_##Nome##_##Nro.at(a_TSS).size() == 0) {	\
				vlrV_##Elem##_##Nome##_##Nro.at(a_TSS).addElemento(a_idEstagio, std::vector<std::string>());\
			}\
			else if (a_idEstagio < vlrV_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial()) {	\
				for (IdEstagio idEstagio = IdEstagio(vlrV_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial() - 1); idEstagio >= a_idEstagio; idEstagio--)\
					vlrV_##Elem##_##Nome##_##Nro.at(a_TSS).addElemento(idEstagio, std::vector<std::string>());\
			}\
			else if (vlrV_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal() < a_idEstagio) {	\
				for (IdEstagio idEstagio = IdEstagio(vlrV_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal() + 1); idEstagio <= a_idEstagio; idEstagio++)\
					vlrV_##Elem##_##Nome##_##Nro.at(a_TSS).addElemento(idEstagio, std::vector<std::string>());\
			}\
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("alocVlr" + std::string(#Elem) +  "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); } \
}; \
void alocVlrConstr##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio){ \
	try{\
		if (DECLARAR_ISVAR_##Elem){return;}\
		if (vlrC_##Elem##_##Nome##_##Nro.size() == 0)	\
			vlrC_##Elem##_##Nome##_##Nro = SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<std::string>>>(TipoSubproblemaSolver(1), std::vector<SmartEnupla<IdEstagio, std::vector<std::string>>>(TipoSubproblemaSolver_Excedente, SmartEnupla<IdEstagio, std::vector<std::string>>())); \
		if (vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).size() == 0)	\
			vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).addElemento(a_idEstagio, std::vector<std::string>());\
		else if (a_idEstagio < vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial()) {	\
			for (IdEstagio idEstagio = IdEstagio(vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial() - 1); idEstagio >= a_idEstagio; idEstagio--)\
				vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).addElemento(idEstagio, std::vector<std::string>());\
		}\
		else if (vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal() < a_idEstagio) {	\
			for (IdEstagio idEstagio = IdEstagio(vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal() + 1); idEstagio <= a_idEstagio; idEstagio++)\
				vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).addElemento(idEstagio, std::vector<std::string>());\
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
			lista_##Elem##_impressao.at(a_TSS).push_back(std::string(std::string(#Nome) + "_" + std::string(#Nro))); \
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
		const std::string name = std::string(std::string(#Nome) + ";" Valores(GET_STRING_ELEMENTO));\
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
		name_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).push_back(std::string(std::string(#Nome) + ";" Valores(GET_STRING_ELEMENTO)));\
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
			alocVlrVar##Elem##_##Nome##_##Nro(a_TSS, a_IdEstagio_1); \
			if (a_idReal == IdRealizacao_Nenhum){\
				if ((isF_##Elem##_##Nome##_##Nro) && (!isPrintFw_##Elem##_##Nome##_##Nro)) { vlrF_##Elem##_##Nome##_##Nro = SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<std::string>>>();}\
				isPrintFw_##Elem##_##Nome##_##Nro = true;\
				const Periodo period = getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_espaco_amostral_hidrologico, a_IdEstagio_1, Periodo());\
				a_idReal = getElementoMatriz(getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico()), AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, period, IdRealizacao());\
			} \
			else {\
				if ((isF_##Elem##_##Nome##_##Nro) && (isPrintFw_##Elem##_##Nome##_##Nro)){vlrF_##Elem##_##Nome##_##Nro = SmartEnupla<TipoSubproblemaSolver, SmartEnupla<IdEstagio, std::vector<std::string>>>();}\
				isPrintFw_##Elem##_##Nome##_##Nro = false; \
			}\
			bool normD = false;\
			if (nrmD_##Elem##_##Nome##_##Nro.size() > 0){\
				if (nrmD_##Elem##_##Nome##_##Nro.at(a_TSS).size() > 0){\
					if (nrmD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size() > 0)\
						normD = true;;\
				}\
			}\
			const std::string strIdCenIdReal = getString(a_idCenario) + ";" + getString(a_idReal) + ";";\
			if (vlrV_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size() == 0) \
				vlrV_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).push_back(std::string(std::string(#Elem) + ";" Valores(GET_STRING_TIPO_ELEMENTO) + "IdCenario;IdRealizacao;VlrPrimal;VlrDual;VlrInf;VlrSup"));\
			for (int pos = 0; pos < int(indx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size()); pos++) {	\
				const double vlrP = vetorEstagio.at(a_IdEstagio_1).getSolver(a_TSS)->getValorPrimal(indx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos));\
				double vlrD = vetorEstagio.at(a_IdEstagio_1).getSolver(a_TSS)->getReducedCost(indx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos));\
				if (normD)\
					vlrD *= nrmD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos); \
				const double vlrI = vetorEstagio.at(a_IdEstagio_1).getSolver(a_TSS)->getLimInferior(indx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos));\
				const double vlrS = vetorEstagio.at(a_IdEstagio_1).getSolver(a_TSS)->getLimSuperior(indx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos));\
				vlrV_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).push_back(std::string(name_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos) + strIdCenIdReal + getString(vlrP) + ";" + getString(vlrD) + ";" + getString(vlrI) + ";" + getString(vlrS))); \
				if (isF_##Elem##_##Nome##_##Nro){\
					if (std::sqrt(vlrP * vlrP)  > vetorEstagio.at(a_IdEstagio_1).getSolver(a_TSS)->getToleranciaViabilidade()){\
						alocVlrF##Elem##_##Nome##_##Nro(a_TSS, a_IdEstagio_1);\
						if (vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size() == 0) \
							vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).push_back(std::string(std::string(#Elem) + ";" Valores(GET_STRING_TIPO_ELEMENTO) + "IdCenario;IdRealizacao;VlrPrimal;VlrDual;VlrInf;VlrSup"));\
						vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).push_back(std::string(name_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos) + strIdCenIdReal + getString(vlrP) + ";" + getString(vlrD) + ";" + getString(vlrI) + ";" + getString(vlrS))); \
					}\
				}\
			}\
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorPrimal" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_idIteracao) + "," + getFullString(a_IdEstagio_1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
}; \
void armazenarValorDual##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_IdEstagio_1, const IdCenario a_idCenario, IdRealizacao a_idReal) { \
	try{ \
		if (DECLARAR_ISVAR_##Elem){return;}\
		if (indx_##Elem##_##Nome##_##Nro.size() == 0) { return; }\
		if (indx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size() == 0) { return; }\
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
		const std::string strIdCenIdReal = getString(a_idCenario) + ";" + getString(a_idReal) + ";";\
		for (int pos = 0; pos < int(indx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size()); pos++) {	\
			double vlrD = vetorEstagio.at(a_IdEstagio_1).getSolver(a_TSS)->getMultiplicador(indx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos));\
			if (DECLARAR_TESTE_DUAL_##Elem){\
				std::vector<std::vector<double>> lista_var_coef = vetorEstagio.at(a_IdEstagio_1).getSolver(a_TSS)->getConstrCoefsByVar(indx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos));\
				if (lista_var_coef.size() > 0) { \
					alocVlrConstr##Elem##_##Nome##_##Nro(a_TSS, a_IdEstagio_1);\
					if (vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size() == 0) \
						vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).push_back(std::string(std::string(#Elem) + ";" Valores(GET_STRING_TIPO_ELEMENTO) + "IdCenario;IdRealizacao;RHS;Dual;Type;sum(Coef,Var)"));\
					if (normD)\
						vlrD *= nrmD_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos); \
					const std::string rhs = getString(vetorEstagio.at(a_IdEstagio_1).getSolver(a_TSS)->getRHSRestricao(indx_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos))) + ";" + getString(vlrD) + ";";\
					vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).push_back(name_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos) + strIdCenIdReal + rhs + "nome;");\
					vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).push_back(name_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(pos) + strIdCenIdReal + rhs + "valr;");\
					for (int i = 0; i < int(lista_var_coef.size()); i++){\
						std::string lInf = "";\
						std::string lSup = "";\
						const int idx_var = int(lista_var_coef.at(i).at(0));\
						const double vlrP = vetorEstagio.at(a_IdEstagio_1).getSolver(a_TSS)->getValorPrimal(idx_var);\
						const double vlrI = vetorEstagio.at(a_IdEstagio_1).getSolver(a_TSS)->getLimInferior(idx_var);\
						const double vlrS = vetorEstagio.at(a_IdEstagio_1).getSolver(a_TSS)->getLimSuperior(idx_var);\
						if (vlrP + vetorEstagio.at(a_IdEstagio_1).getSolver(a_TSS)->getToleranciaViabilidade() >= vlrS)\
							lSup = "|";\
						if (vlrP - vetorEstagio.at(a_IdEstagio_1).getSolver(a_TSS)->getToleranciaViabilidade() <= vlrI)\
							lInf = "|";\
						std::string name_var = vetorEstagio.at(a_IdEstagio_1).getSolver(a_TSS)->getNomeVariavel(idx_var);\
						const std::string coef_var = getString(lista_var_coef.at(i).at(1));\
						vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size()-2) += coef_var + ";" + lInf + name_var + lSup + ";"; \
						vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).at(vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_IdEstagio_1).size()-1) += coef_var + ";" + getString(vlrP) + ";"; \
					}\
				}\
			}\
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("armazenarValorDual" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_IdEstagio_1) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); } \
}; \
void imprimirVariaveis##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void imprimirRestricoes##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados); \
void consolidarResultados##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_variavel, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados);



//
// ARMAZENAR VALOR POR ESTAGIO POR CENARIO POR REALIZACAO
//

#define ARMAZENAR_VALOR(Elem, Nome, Nro, Valores) \
armazenarValorPrimal##Elem##_##Nome##_##Nro(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao);\
armazenarValorDual##Elem##_##Nome##_##Nro(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao);

//
// IMPRIMIR VALOR PRIMAL POR ESTAGIO POR CENARIO
//

#define DECLARAR_IMPRIMIR_VALOR(Elem, Nome, Nro, Valores) \
void ModeloOtimizacao::imprimirVariaveis##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (DECLARAR_ISVAR_##Elem){\
			const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida(); \
			if (vlrV_##Elem##_##Nome##_##Nro.size() == 0){ return;}\
			if (vlrV_##Elem##_##Nome##_##Nro.at(a_TSS).size() == 0){return;}\
			if (a_idEstagio < vlrV_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial()){return;}\
			if (vlrV_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal() < a_idEstagio){return;}\
			if (vlrV_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_idEstagio).size() == 0){return;}\
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//" + std::string(#Elem)  + std::string(#Nome)); \
			a_entradaSaidaDados.setAppendArquivo(true);\
			if ((isPrintFw_##Elem##_##Nome##_##Nro) && (a_idEstagio == vlrV_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial()))\
				a_entradaSaidaDados.setAppendArquivo(false); \
			else if ((!isPrintFw_##Elem##_##Nome##_##Nro) && (a_idEstagio == vlrV_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal()))\
				a_entradaSaidaDados.setAppendArquivo(false); \
			const std::string nome_arquivo = std::string(std::string(#Elem) + std::string(#Nome) + "_" Valores(GET_STRING_TIPO_ELEMENTO2) + a_nome_arquivo + ".csv"); \
			a_entradaSaidaDados.imprimirArquivoCSV(nome_arquivo, vlrV_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_idEstagio), true); \
			vlrV_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_idEstagio) = std::vector<string>();\
			if (vlrF_##Elem##_##Nome##_##Nro.size() == 0){ return;}\
			if (vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).size() == 0){return;}\
			if (a_idEstagio < vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial()){return;}\
			if (vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal() < a_idEstagio){return;}\
			if (vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_idEstagio).size() == 0){return;}\
			a_entradaSaidaDados.setDiretorioSaida(diretorio); \
			a_entradaSaidaDados.setAppendArquivo(true);\
			if ((isPrintFw_##Elem##_##Nome##_##Nro) && (a_idEstagio == vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial()))\
				a_entradaSaidaDados.setAppendArquivo(false); \
			else if ((!isPrintFw_##Elem##_##Nome##_##Nro) && (a_idEstagio == vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal()))\
				a_entradaSaidaDados.setAppendArquivo(false); \
			a_entradaSaidaDados.imprimirArquivoCSV(nome_arquivo, vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_idEstagio), true); \
			vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_idEstagio) = std::vector<string>();\
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirVariaveis" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\
void ModeloOtimizacao::imprimirRestricoes##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const std::string a_nome_arquivo, EntradaSaidaDados a_entradaSaidaDados){ \
	try{ \
		if (DECLARAR_ISVAR_##Elem){return;}\
		if (vlrC_##Elem##_##Nome##_##Nro.size() == 0){ return;}\
		if (vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).size() == 0){return;}\
		if (a_idEstagio < vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial()){return;}\
		if (vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal() < a_idEstagio){return;}\
		if (vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_idEstagio).size() == 0){return;}\
		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//" + std::string(#Elem)  + std::string(#Nome)); \
		a_entradaSaidaDados.setAppendArquivo(true);\
		if ((isPrintFw_##Elem##_##Nome##_##Nro) && (a_idEstagio == vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorInicial()))\
			a_entradaSaidaDados.setAppendArquivo(false); \
		else if ((!isPrintFw_##Elem##_##Nome##_##Nro) && (a_idEstagio == vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).getIteradorFinal()))\
			a_entradaSaidaDados.setAppendArquivo(false); \
		std::string nome_arquivo = std::string(std::string(#Elem) + std::string(#Nome) + "_" Valores(GET_STRING_TIPO_ELEMENTO2) + a_nome_arquivo + ".csv"); \
		a_entradaSaidaDados.imprimirArquivoCSV(nome_arquivo, vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_idEstagio), true); \
		vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).at(a_idEstagio) = std::vector<string>();\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("imprimirRestricoes" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + a_nome_arquivo + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\

#define IMPRIMIR_VALOR(Elem, Nome, Nro, Valores) \
imprimirVariaveis##Elem##_##Nome##_##Nro(TSS, a_idEstagio, getFullString(a_idProcesso), a_entradaSaidaDados); \
imprimirRestricoes##Elem##_##Nome##_##Nro(TSS, a_idEstagio, getFullString(a_idProcesso), a_entradaSaidaDados);

//
// CONSOLIDAR RESULTADOS
//

#define DECLARAR_CONSOLIDAR_RESULTADOS(Elem, Nome, Nro, Valores)  \
void ModeloOtimizacao::consolidarResultados##Elem##_##Nome##_##Nro(const TipoSubproblemaSolver a_TSS, const std::string a_variavel, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados) {	\
	try{ \
		if (std::string(std::string(#Nome) + "_" + std::string(#Nro)) != a_variavel) \
			return; \
		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida(); \
		if (DECLARAR_ISVAR_##Elem){\
			if (vlrV_##Elem##_##Nome##_##Nro.size() == 0){return;}\
			if (vlrV_##Elem##_##Nome##_##Nro.at(a_TSS).size() == 0){return;}\
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//" + std::string(#Elem) + std::string(#Nome)); \
			std::string nome_arquivo = std::string(std::string(#Elem) + std::string(#Nome) + "_" Valores(GET_STRING_TIPO_ELEMENTO2));\
			nome_arquivo.erase(nome_arquivo.size()-1);\
			nome_arquivo+= ".csv"; \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, "");\
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
				lista_arquivos.at(getRank(idProcesso)) = std::string(std::string(#Elem) + std::string(#Nome) + "_" Valores(GET_STRING_TIPO_ELEMENTO2) + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoVerticalCSV(nome_arquivo, lista_arquivos, true, true); \
			if (vlrF_##Elem##_##Nome##_##Nro.size() > 0){\
				if (vlrF_##Elem##_##Nome##_##Nro.at(a_TSS).size() > 0){\
					a_entradaSaidaDados.setDiretorioSaida(diretorio); \
					a_entradaSaidaDados.imprimirConsolidacaoVerticalCSV(nome_arquivo, lista_arquivos, true, true); \
				}\
			}\
		} \
		else{\
			if (vlrC_##Elem##_##Nome##_##Nro.size() == 0){return;}\
			if (vlrC_##Elem##_##Nome##_##Nro.at(a_TSS).size() == 0){return;}\
			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//" + std::string(#Elem) + std::string(#Nome)); \
			std::string nome_arquivo = std::string(std::string(#Elem) + std::string(#Nome)  + "_" Valores(GET_STRING_TIPO_ELEMENTO2));\
			nome_arquivo.erase(nome_arquivo.size()-1);\
			nome_arquivo+= ".csv"; \
			std::vector<std::string> lista_arquivos(a_maiorIdProcesso, ""); \
			for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) \
			lista_arquivos.at(getRank(idProcesso)) = std::string(std::string(#Elem) + std::string(#Nome) + "_" Valores(GET_STRING_TIPO_ELEMENTO2) + getFullString(idProcesso) + ".csv"); \
			a_entradaSaidaDados.imprimirConsolidacaoVerticalCSV(nome_arquivo, lista_arquivos, true, true); \
		}\
	} \
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao::consolidarResultados" + std::string(#Elem) + "_" + std::string(#Nome) + "_" + std::string(#Nro) + "(" + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); } \
};\


#define CONSOLIDAR_RESULTADOS(Elem, Nome, Nro, Valores) consolidarResultados##Elem##_##Nome##_##Nro(a_TSS, Elem##_str, a_maiorIdProcesso, a_entradaSaidaDados);



#endif 
