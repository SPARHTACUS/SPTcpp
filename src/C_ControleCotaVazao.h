#ifndef CONTROLE_COTA_VAZAO_H
#define CONTROLE_COTA_VAZAO_H

#include "C_SmartElemento.h"

#define ATT_COMUM_CONTROLE_COTA_VAZAO(m)  \
	  m(ControleCotaVazao,  AttComum,        idControleCotaVazao,     IdControleCotaVazao,         min,         max,           min,      sim) \
	  m(ControleCotaVazao,  AttComum,             nome,        string,         min,         max,        Nenhum,      sim) \
	  m(ControleCotaVazao,  AttComum,       penalidade,        double,           0,         max,         10000,      sim) \
	  m(ControleCotaVazao,  AttComum,    coeficiente_0,        double,         min,       10000,             0,      nao) \
	  m(ControleCotaVazao,  AttComum,    coeficiente_1,        double,         min,       10000,             0,      nao) \
	  m(ControleCotaVazao,  AttComum,    coeficiente_2,        double,         min,       10000,             0,      nao) \
	  m(ControleCotaVazao,  AttComum,    coeficiente_3,        double,         min,       10000,             0,      nao) \
	  m(ControleCotaVazao,  AttComum,    coeficiente_4,        double,         min,       10000,             0,      nao) \
	  m(ControleCotaVazao,  AttComum,    coeficiente_5,        double,         min,       10000,             0,      nao) \
	  m(ControleCotaVazao,  AttComum,    coeficiente_6,        double,         min,       10000,             0,      nao) 
//     c_classe,   smrtAtt,     nomeAtributo,          tipo,  lowerBound,  upperBound,  initialValue, mustRead?


#define ATT_VETOR_CONTROLE_COTA_VAZAO(m)  \
	  m(ControleCotaVazao,  AttVetor,               cota_anterior,           double,           0,         max,             0,  Periodo)\
	  m(ControleCotaVazao,  AttVetor,          nivel_dia_anterior,           double,           0,         max,             0,  Periodo)\
	  m(ControleCotaVazao,  AttVetor,       hidreletrica_montante,   IdHidreletrica,         min,         max,           min,      int)\
	  m(ControleCotaVazao,  AttVetor,                hidreletrica,   IdHidreletrica,         min,         max,           min,      int)\
	  m(ControleCotaVazao,  AttVetor,    coef_linear_cota_vazao_0,           double,         min,         max,             0,  Periodo)\
	  m(ControleCotaVazao,  AttVetor,    coef_linear_cota_vazao_1,           double,         min,         max,             0,  Periodo)\
	  m(ControleCotaVazao,  AttVetor,     variacao_maxima_horaria,           double,           0,         max,             0,  Periodo)\
	  m(ControleCotaVazao,  AttVetor,      variacao_maxima_diaria,           double,           0,         max,             0,  Periodo)
//     c_classe,   smrtAtt,                nomeAtributo,             Tipo,  lowerBound,  upperBound,  initialValue,  TipoIterador


#define ATT_MATRIZ_CONTROLE_COTA_VAZAO(m)  \
	  m(ControleCotaVazao,  AttMatriz,            num_horas_lag,           double,         0,         max,             1,  Periodo, int)\
	  m(ControleCotaVazao,  AttMatriz,              var_abs_inf,           double,         min,         0,             0,  Periodo, int)\
	  m(ControleCotaVazao,  AttMatriz,              var_abs_sup,           double,         0,         max,             0,  Periodo, int)

#define SMART_ELEMENTO_CONTROLE_COTA_VAZAO(m) \
	  m(ControleCotaVazao, AttComum, ATT_COMUM_CONTROLE_COTA_VAZAO)\
	  m(ControleCotaVazao, AttVetor, ATT_VETOR_CONTROLE_COTA_VAZAO) \
	  m(ControleCotaVazao, AttMatriz, ATT_MATRIZ_CONTROLE_COTA_VAZAO) 

DEFINE_SMART_ELEMENTO(ControleCotaVazao, SMART_ELEMENTO_CONTROLE_COTA_VAZAO)

class ControleCotaVazao : public SmartDados {
public:

	ControleCotaVazao();
	ControleCotaVazao(const ControleCotaVazao &instanciaCopiar);
	void esvaziar();
	virtual ~ControleCotaVazao();

	DECLARA_SMART_ELEMENTO(ControleCotaVazao, SMART_ELEMENTO_CONTROLE_COTA_VAZAO)

};

GET_STRING_FROM_CLASS(ControleCotaVazao)

#endif // !defined(CONTROLE_COTA_VAZAO_H)
