#ifndef PATAMAR_DEFICIT_H
#define PATAMAR_DEFICIT_H

#include <string>

#include "C_SmartElemento.h"

#define ATT_COMUM_PATAMAR_DEFICIT(m)  \
	  m(PatamarDeficit,  AttComum,     idPatamarDeficit,   IdPatamarDeficit,        min,          max,           min,      sim) \
      m(PatamarDeficit,  AttComum,           percentual,             double,          0,            1,             0,      sim) \
	  m(PatamarDeficit,  AttComum,                custo,             double,          0,        50000,             0,      sim) \
      m(PatamarDeficit,  AttComum,                 nome,             string,         min,         max,        Nenhum,      nao)
//            c_classe,   smrtAtt,         nomeAtributo,               tipo,  lowerBound,  upperBound,  initialValue, mustRead?


#define ATT_VETOR_PATAMAR_DEFICIT(m)  \
	  m(PatamarDeficit,  AttVetor,                 custo,               double,            0,           max,             0,  Periodo) \
	  m(PatamarDeficit,  AttVetor,            percentual,               double,            0,             1,             0,  Periodo) \
	  m(PatamarDeficit,  AttVetor,           custo_media,               double,            0,           max,             0,  Periodo) \
	  m(PatamarDeficit,  AttVetor, potencia_maxima_media,               double,            0,             1,             0,  Periodo) 

#define ATT_MATRIZ_PATAMAR_DEFICIT(m)  \
	  m(PatamarDeficit,  AttMatriz,                potencia_maxima,               double,            0,           max,             0,  Periodo, IdPatamarCarga) \
      m(PatamarDeficit,  AttMatriz,                     percentual,               double,            0,             1,             0,  Periodo, IdPatamarCarga) \
      m(PatamarDeficit,  AttMatriz,                          custo,               double,            0,           max,             0,  Periodo, IdPatamarCarga) 

#define SMART_ELEMENTO_PATAMAR_DEFICIT(m) \
	m(PatamarDeficit, AttComum, ATT_COMUM_PATAMAR_DEFICIT)  \
	m(PatamarDeficit, AttVetor, ATT_VETOR_PATAMAR_DEFICIT)  \
	m(PatamarDeficit, AttMatriz, ATT_MATRIZ_PATAMAR_DEFICIT)  

DEFINE_SMART_ELEMENTO(PatamarDeficit, SMART_ELEMENTO_PATAMAR_DEFICIT)

class PatamarDeficit : public SmartDados {
public:

	PatamarDeficit();
	PatamarDeficit(const PatamarDeficit &instanciaCopiar);
	void esvaziar();
	virtual ~PatamarDeficit();

	DECLARA_SMART_ELEMENTO(PatamarDeficit, SMART_ELEMENTO_PATAMAR_DEFICIT)

};

GET_STRING_FROM_CLASS(PatamarDeficit)

#endif // !defined(C_PATAMAR_DEFICIT_H)
