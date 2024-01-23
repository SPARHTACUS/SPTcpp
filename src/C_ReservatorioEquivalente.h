#ifndef ReservatorioEquivalente_H
#define ReservatorioEquivalente_H

#include "C_SmartElemento.h"

#define ATT_COMUM_REE(m)  \
	  m(ReservatorioEquivalente,  AttComum,    idReservatorioEquivalente,     IdReservatorioEquivalente,         min,         max,           min,      sim)

//     c_classe,   smrtAtt,     nomeAtributo,          tipo,  lowerBound,  upperBound,  initialValue, mustRead?


#define ATT_MATRIZ_REE(m)  \
      m(ReservatorioEquivalente,  AttMatriz,     conversao_ENA_acoplamento_0,     double,       min,        max,              0,  Periodo, IdCenario) \
      m(ReservatorioEquivalente,  AttMatriz,     conversao_ENA_acoplamento_1,     double,       min,        max,              0,  Periodo, IdCenario)

#define SMART_ELEMENTO_REE(m) \
	  m(ReservatorioEquivalente, AttComum, ATT_COMUM_REE)\
	  m(ReservatorioEquivalente, AttMatriz, ATT_MATRIZ_REE) 

DEFINE_SMART_ELEMENTO(ReservatorioEquivalente, SMART_ELEMENTO_REE)

class ReservatorioEquivalente : public SmartDados {
public:

	ReservatorioEquivalente();
	ReservatorioEquivalente(const ReservatorioEquivalente &instanciaCopiar);
	void esvaziar();
	virtual ~ReservatorioEquivalente();

	DECLARA_SMART_ELEMENTO(ReservatorioEquivalente, SMART_ELEMENTO_REE)


};

GET_STRING_FROM_CLASS(ReservatorioEquivalente)

#endif
