#ifndef USINA_ELEVATORIA_H
#define USINA_ELEVATORIA_H

#include "C_SmartElemento.h"


#define ATT_COMUM_USINA_ELEVATORIA(m)  \
	  m(UsinaElevatoria,  AttComum,             idUsinaElevatoria,   IdUsinaElevatoria,         min,         max,           min,      sim) \
	  m(UsinaElevatoria,  AttComum,                          nome,              string,         min,         max,           min,      sim) \
	  m(UsinaElevatoria,  AttComum,                    submercado,        IdSubmercado,      Nenhum,         max,        Nenhum,      sim) \
	  m(UsinaElevatoria,  AttComum,               taxa_de_consumo,              double,           0,         max,             0,      sim) \
	  m(UsinaElevatoria,  AttComum,         vazao_bombeada_minima,              double,           0,         max,             0,      nao) \
	  m(UsinaElevatoria,  AttComum,         vazao_bombeada_maxima,              double,           0,         max,             0,      nao) \
	  m(UsinaElevatoria,  AttComum,  vazao_bombeada_disponivel_minima,          double,           0,       50000,             0,      nao) \
	  m(UsinaElevatoria,  AttComum,  vazao_bombeada_disponivel_maxima,          double,           0,         max,           max,      nao) \
	  m(UsinaElevatoria,  AttComum,      usina_origem_bombeamento,      IdHidreletrica,      Nenhum,         max,        Nenhum,      sim) \
	  m(UsinaElevatoria,  AttComum,     usina_destino_bombeamento,      IdHidreletrica,      Nenhum,         max,        Nenhum,      sim) \
      m(UsinaElevatoria,  AttComum,         fator_disponibilidade,              double,           0,         max,             1,      nao) \
      m(UsinaElevatoria,  AttComum,  penalidade_vazao_bombeada_minima,          double,           0,         max,          1000,      sim)

//             c_classe,   smrtAtt,       nomeAtributo,                tipo,  lowerBound,  upperBound,  initialValue, mustRead?


#define ATT_VETOR_USINA_ELEVATORIA(m)  \
	  m(UsinaElevatoria,  AttVetor,     fator_disponibilidade,                double,          0,            1,             1,      Periodo) \
	  m(UsinaElevatoria,  AttVetor,     vazao_bombeada_minima,                double,          0,        50000,             0,      Periodo) \
	  m(UsinaElevatoria,  AttVetor,     vazao_bombeada_maxima,                double,          0,          max,           max,      Periodo) \
	  m(UsinaElevatoria,  AttVetor,     vazao_bombeada_disponivel_minima,     double,          0,        50000,             0,      Periodo) \
	  m(UsinaElevatoria,  AttVetor,     vazao_bombeada_disponivel_maxima,     double,          0,          max,           max,      Periodo) \
	  m(UsinaElevatoria,  AttVetor,     vazao_bombeada_disponivel_minima_media,     double,          0,          max,           max,      Periodo) \
	  m(UsinaElevatoria,  AttVetor,     vazao_bombeada_disponivel_maxima_media,     double,          0,          max,           max,      Periodo) 
//             c_classe,   smrtAtt,           nomeAtributo,       Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador


#define ATT_MATRIZ_USINA_ELEVATORIA(m)  \
      m(UsinaElevatoria,  AttMatriz,    vazao_bombeada_disponivel_minima,     double,          0,        50000,             0,  Periodo, IdPatamarCarga) \
      m(UsinaElevatoria,  AttMatriz,    vazao_bombeada_disponivel_maxima,     double,          0,          max,           max,  Periodo, IdPatamarCarga)

#define SMART_ELEMENTO_USINA_ELEVATORIA(m) \
	m(UsinaElevatoria, AttComum,  ATT_COMUM_USINA_ELEVATORIA) \
	m(UsinaElevatoria, AttVetor,  ATT_VETOR_USINA_ELEVATORIA) \
    m(UsinaElevatoria, AttMatriz, ATT_MATRIZ_USINA_ELEVATORIA)

DEFINE_SMART_ELEMENTO(UsinaElevatoria, SMART_ELEMENTO_USINA_ELEVATORIA)


class UsinaElevatoria : public SmartDados {

	friend class Dados;
	friend class Hidreletrica;

public:

	UsinaElevatoria();
	UsinaElevatoria(const UsinaElevatoria& instanciaCopiar);
	void esvaziar();
	virtual ~UsinaElevatoria();

	DECLARA_SMART_ELEMENTO(UsinaElevatoria, SMART_ELEMENTO_USINA_ELEVATORIA)

};

GET_STRING_FROM_CLASS(UsinaElevatoria)

#endif // AFLUENCIA_H
