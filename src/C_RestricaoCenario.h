#ifndef RESTRICAO_CENARIO
#define RESTRICAO_CENARIO

#include "C_SmartElemento.h"

#define ATT_COMUM_RESTRICAO_CENARIO(m)  \
	  m(RestricaoCenario,  AttComum,        idRestricaoCenario,      IdRestricaoCenario,         min,          max,           min,      sim) \
	  m(RestricaoCenario,  AttComum,                        nome,                    string,         min,          max,           min,      sim)

//     c_classe,   smrtAtt,                           nomeAtributo,                      tipo,  lowerBound,   upperBound,  initialValue, mustRead?

#define ATT_VETOR_RESTRICAO_CENARIO(m)  \
	  m(RestricaoCenario,   AttVetor,          idRestricao,     int,        -1,          max,             -1,  TipoSubproblemaSolver) \
	  m(RestricaoCenario,   AttVetor,                rhs,     double,     min,         max,            0.0,  IdCenario) 
//            c_classe,    smrtAtt,       nomeAtributo,    Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador

#define ATT_MATRIZ_RESTRICAO_CENARIO(m)  \
      m(RestricaoCenario,  AttMatriz,          coeficiente,     double,        min,     max,          0,  int, IdCenario) 

#define SMART_ELEMENTO_RESTRICAO_CENARIO(m) \
	m(RestricaoCenario, AttComum, ATT_COMUM_RESTRICAO_CENARIO)  \
	m(RestricaoCenario, AttVetor, ATT_VETOR_RESTRICAO_CENARIO)  \
	m(RestricaoCenario, AttMatriz, ATT_MATRIZ_RESTRICAO_CENARIO)  
 

DEFINE_SMART_ELEMENTO(RestricaoCenario, SMART_ELEMENTO_RESTRICAO_CENARIO)

class RestricaoCenario : public SmartDados {

public:

	RestricaoCenario();
	RestricaoCenario(const RestricaoCenario &instanciaCopiar);
	void esvaziar();
	virtual ~RestricaoCenario();

	DECLARA_SMART_ELEMENTO(RestricaoCenario, SMART_ELEMENTO_RESTRICAO_CENARIO)

};

GET_STRING_FROM_CLASS(RestricaoCenario)

#endif 
