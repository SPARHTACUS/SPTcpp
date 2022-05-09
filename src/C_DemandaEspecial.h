#ifndef DEMANDA_ESPECIAL_H
#define DEMANDA_ESPECIAL_H

#include "C_SmartElemento.h"


#define ATT_COMUM_DEMANDA_ESPECIAL(m)  \
	  m(DemandaEspecial,  AttComum,                idDemandaEspecial,             IdDemandaEspecial,      Nenhum,         max,        Nenhum,       sim) \
	  m(DemandaEspecial,  AttComum,                   numero_demanda,                           int,           0,         max,               0,     nao) 
//       c_classe,   smrtAtt,                    nomeAtributo,                         tipo,  lowerBound,  upperBound,  initialValue, mustRead?


#define ATT_MATRIZ_DEMANDA_ESPECIAL(m)  \
	  m(DemandaEspecial,  AttMatriz,      demanda,     double,          0.0,          max,             0.0,  Periodo, IdPatamarCarga) 
//       c_classe,   smrtAtt,               nomeAtributo,       Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador

#define SMART_ELEMENTO_DEMANDA_ESPECIAL(m) \
	m(DemandaEspecial, AttComum,   ATT_COMUM_DEMANDA_ESPECIAL) \
	m(DemandaEspecial, AttMatriz,   ATT_MATRIZ_DEMANDA_ESPECIAL) 

DEFINE_SMART_ELEMENTO(DemandaEspecial, SMART_ELEMENTO_DEMANDA_ESPECIAL)


class DemandaEspecial : public SmartDados {

	friend class Dados;

public:
	 
	DemandaEspecial();
	DemandaEspecial(const DemandaEspecial &instanciaCopiar);
	void esvaziar();
	virtual ~DemandaEspecial();

	DECLARA_SMART_ELEMENTO(DemandaEspecial, SMART_ELEMENTO_DEMANDA_ESPECIAL)

};

GET_STRING_FROM_CLASS(DemandaEspecial)

#endif 
