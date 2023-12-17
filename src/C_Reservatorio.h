#ifndef RESERVATORIO_H
#define RESERVATORIO_H

#include "C_SmartElemento.h"

#define ATT_COMUM_RESERVATORIO(m)  \
	  m(Reservatorio,  AttComum,                  idReservatorio,    IdReservatorio,        min,          min,           min,      sim) \
	  m(Reservatorio,  AttComum,                      nome_usina,            string,        min,          max,        Nenhum,      sim) \
	  m(Reservatorio,  AttComum,                     desvio_agua,            double,          0,        50000,             0,      nao) \
	  m(Reservatorio,  AttComum,                   volume_minimo,            double,          0,        50000,             0,      nao) \
	  m(Reservatorio,  AttComum,                   volume_maximo,            double,          0,       100000,             0,      nao) \
      m(Reservatorio,  AttComum,             volume_util_inicial,            double,          0,       100000,             0,      nao) \
	  m(Reservatorio,  AttComum,        volume_minimo_vertimento,            double,          0,        50000,             0,      nao) \
      m(Reservatorio,  AttComum,            volume_minimo_desvio,            double,          0,        50000,             0,      nao) \
	  m(Reservatorio,  AttComum,  percentual_volume_util_inicial,            double,          0,            1,             0,      nao) \
	  m(Reservatorio,  AttComum,   percentual_altura_equivalente,            double,          0,            1,             0,      nao) \
	  m(Reservatorio,  AttComum,   percentual_volume_util_minimo,            double,          0,            1,             0,      nao) \
	  m(Reservatorio,  AttComum,   percentual_volume_util_maximo,            double,          0,            1,             1,      nao) \
	  m(Reservatorio,  AttComum,                     cota_minimo,            double,          0,        10000,             0,      nao) \
	  m(Reservatorio,  AttComum,                     cota_maximo,            double,          0,        10000,             0,      nao) \
	  m(Reservatorio,  AttComum,                 cota_referencia,            double,          0,        10000,             0,      nao) \
	  m(Reservatorio,  AttComum,              poli_cota_volume_0,            double,        min,          max,             0,      nao) \
	  m(Reservatorio,  AttComum,              poli_cota_volume_1,            double,        min,          max,             0,      nao) \
	  m(Reservatorio,  AttComum,              poli_cota_volume_2,            double,        min,          max,             0,      nao) \
	  m(Reservatorio,  AttComum,              poli_cota_volume_3,            double,        min,          max,             0,      nao) \
	  m(Reservatorio,  AttComum,              poli_cota_volume_4,            double,        min,          max,             0,      nao) \
	  m(Reservatorio,  AttComum,                poli_cota_area_0,            double,        min,          max,             0,      nao) \
	  m(Reservatorio,  AttComum,                poli_cota_area_1,            double,        min,          max,             0,      nao) \
	  m(Reservatorio,  AttComum,                poli_cota_area_2,            double,        min,          max,             0,      nao) \
	  m(Reservatorio,  AttComum,                poli_cota_area_3,            double,        min,          max,             0,      nao) \
	  m(Reservatorio,  AttComum,                poli_cota_area_4,            double,        min,          max,             0,      nao) \
	  m(Reservatorio,  AttComum,           enchendo_volume_morto,              int,          0,            1,             0,      nao) \
	  m(Reservatorio,  AttComum,                    volume_morto,            double,          0,          max,             0,      nao) \
	  m(Reservatorio,  AttComum,                      evaporacao,            double,          0,          max,             0,      nao) \
      m(Reservatorio,  AttComum,              volume_util_minimo,            double,          0,       100000,             0,      nao) \
      m(Reservatorio,  AttComum,              volume_util_maximo,            double,          0,       100000,             0,      nao)

//          c_classe,   smrtAtt,                    nomeAtributo,              tipo,  lowerBound,   upperBound,  initialValue, mustRead?

#define ATT_VETOR_RESERVATORIO(m)  \
	  m(Reservatorio,  AttVetor,                              evaporacao,            double,        min,          max,             0,          IdMes) \
	  m(Reservatorio,  AttVetor,                coef_linear_evaporacao_0,            double,        min,          max,             0,        Periodo) \
	  m(Reservatorio,  AttVetor,                coef_linear_evaporacao_1,            double,        min,          max,             0,        Periodo) \
	  m(Reservatorio,  AttVetor,                   volume_morto_completo,              int,          0,            1,             0,        Periodo) \
	  m(Reservatorio,  AttVetor,                   enchendo_volume_morto,              int,          0,            1,             0,        Periodo) \
	  m(Reservatorio,  AttVetor,                         cota_referencia,            double,          0,        10000,             0,        Periodo) \
	  m(Reservatorio,  AttVetor,                           volume_minimo,            double,          0,        50000,             0,        Periodo) \
	  m(Reservatorio,  AttVetor,                           volume_maximo,            double,          0,       100000,             0,        Periodo) \
	  m(Reservatorio,  AttVetor,            meta_enchimento_volume_morto,            double,          0,          max,             0,        Periodo) \
	  m(Reservatorio,  AttVetor,            taxa_enchimento_volume_morto,            double,          0,          max,             0,        Periodo) \
      m(Reservatorio,  AttVetor,                      poli_cota_volume_0,            double,        min,          max,             0,        Periodo) \
      m(Reservatorio,  AttVetor,                      poli_cota_volume_1,            double,        min,          max,             0,        Periodo) \
      m(Reservatorio,  AttVetor,                      poli_cota_volume_2,            double,        min,          max,             0,        Periodo) \
      m(Reservatorio,  AttVetor,                      poli_cota_volume_3,            double,        min,          max,             0,        Periodo) \
      m(Reservatorio,  AttVetor,                      poli_cota_volume_4,            double,        min,          max,             0,        Periodo) \
      m(Reservatorio,  AttVetor,                        poli_cota_area_0,            double,        min,          max,             0,        Periodo) \
      m(Reservatorio,  AttVetor,                        poli_cota_area_1,            double,        min,          max,             0,        Periodo) \
      m(Reservatorio,  AttVetor,                        poli_cota_area_2,            double,        min,          max,             0,        Periodo) \
      m(Reservatorio,  AttVetor,                        poli_cota_area_3,            double,        min,          max,             0,        Periodo) \
      m(Reservatorio,  AttVetor,                        poli_cota_area_4,            double,        min,          max,             0,        Periodo) \
      m(Reservatorio,  AttVetor,           percentual_volume_util_maximo,            double,          0,            1,             1,        Periodo) \
      m(Reservatorio,  AttVetor,           percentual_volume_util_minimo,            double,          0,            1,             0,        Periodo) \
      m(Reservatorio,  AttVetor,                      volume_util_minimo,            double,          0,       100000,             0,        Periodo) \
      m(Reservatorio,  AttVetor,                      volume_util_maximo,            double,          0,       100000,             0,        Periodo)
//          c_classe,   smrtAtt,                nomeAtributo,              Tipo, lowerBound,   upperBound,  initialValue,      TipoIterador

#define ATT_MATRIZ_RESERVATORIO(m)  \
      m(Reservatorio,  AttMatriz,                          volume_meta,     double,          0,      max,         0,  IdCenario, Periodo) 

#define SMART_ELEMENTO_RESERVATORIO(m) \
	m(Reservatorio, AttComum, ATT_COMUM_RESERVATORIO) \
	m(Reservatorio, AttVetor, ATT_VETOR_RESERVATORIO) \
	m(Reservatorio, AttMatriz, ATT_MATRIZ_RESERVATORIO) 

DEFINE_SMART_ELEMENTO(Reservatorio, SMART_ELEMENTO_RESERVATORIO)

class Reservatorio : public SmartDados {

public:

	Reservatorio();			// Construtor
	Reservatorio(const Reservatorio& instanciaCopiar);
	void esvaziar();
	virtual ~Reservatorio();	// Destrutor

	DECLARA_SMART_ELEMENTO(Reservatorio, SMART_ELEMENTO_RESERVATORIO)

		virtual void validacaoDadosAttComum();

	// Calcula Cota a partir do Volume fornecido
	double getCota(const double a_volume) const;

	double getCota(const Periodo a_periodo, const double a_volume) const;

	// Calcula Cota média a partir de de dois volumes
	double getCotaMedia(const double a_volume_minimo, double a_volume_maximo) const;

	double getCotaMedia(const Periodo a_periodo, const double a_volume_minimo, double a_volume_maximo) const;

	// Calcula a Area a partir da Cota ou Volume fornecido
	double getAreaFromCota(const Periodo a_periodo, const double a_cota) const;

	double getAreaFromVolume(const Periodo a_periodo, const double a_volume) const;

	void calculaAproximacaoLinearEvaporacao(const double a_volumeMinimo, const double a_volumeMaximo, const Periodo a_periodo);

	void calculaAproximacaoLinearEvaporacao(const double a_volumeUtil, const Periodo a_periodo);

	void calculaAproximacaoLinearEvaporacao(const Periodo a_periodo);


	static bool isReservatorioDeRegularizacao(const Periodo a_periodo, const double a_volume_util_operacional, const double a_turbinamento_maximo);

};

GET_STRING_FROM_CLASS(Reservatorio)

#endif // RESERVATORIO_H
