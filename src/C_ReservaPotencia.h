#ifndef RESERVA_POTENCIA_H
#define RESERVA_POTENCIA_H

#include "C_SmartElemento.h"

#define ATT_COMUM_RESERVA_POTENCIA(m)  \
	  m(ReservaPotencia,  AttComum,   idReservaPotencia,     IdReservaPotencia,      Nenhum,          max,        Nenhum,      sim) \
	  m(ReservaPotencia,  AttComum,                nome,                string,         min,          max,           min,      nao) \
	  m(ReservaPotencia,  AttComum,                area,                IdArea,      Nenhum,          max,        Nenhum,      nao) 
//               c_classe,   smrtAtt,        nomeAtributo,                tipo,  lowerBound,   upperBound,  initialValue, mustRead?


#define ATT_VETOR_RESERVA_POTENCIA(m)  \
	  m(ReservaPotencia,  AttVetor,            hidreletrica,  IdHidreletrica,     Nenhum,          max,        Nenhum,      int) \
	  m(ReservaPotencia,  AttVetor,            termeletrica,  IdHidreletrica,     Nenhum,          max,        Nenhum,      int) \
	  m(ReservaPotencia,  AttVetor,  recebimento_submercado,    IdSubmercado,     Nenhum,          max,        Nenhum,      int) \
	  m(ReservaPotencia,  AttVetor,          reserva_minima,          double,          0,          max,             0,  Periodo) 
//             c_classe,   smrtAtt,            nomeAtributo,            Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador


#define SMART_ELEMENTO_RESERVA_POTENCIA(m) \
	m(ReservaPotencia, AttComum, ATT_COMUM_RESERVA_POTENCIA) \
	m(ReservaPotencia, AttVetor, ATT_VETOR_RESERVA_POTENCIA)

DEFINE_SMART_ELEMENTO(ReservaPotencia, SMART_ELEMENTO_RESERVA_POTENCIA)

class ReservaPotencia : public SmartDados {
public:

	ReservaPotencia();
	ReservaPotencia(const ReservaPotencia &instanciaCopiar);
	void esvaziar();
	virtual ~ReservaPotencia();

	DECLARA_SMART_ELEMENTO(ReservaPotencia, SMART_ELEMENTO_RESERVA_POTENCIA)

};

GET_STRING_FROM_CLASS(ReservaPotencia)

#endif // !defined(C_RESTRICAO_ELETRICA_H)
