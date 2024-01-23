#ifndef VARIAVEL_ESTADO
#define VARIAVEL_ESTADO

#include "C_SmartElemento.h"

#define ATT_COMUM_VARIAVEL_ESTADO(m)  \
	  m(VariavelEstado,  AttComum,                     idVariavelEstado,      IdVariavelEstado,         min,          max,           min,      sim) \
	  m(VariavelEstado,  AttComum,                                 nome,                string,         min,          max,        Nenhum,      sim) \
	  m(VariavelEstado,  AttComum, tipoSubproblemaSolverEstagioAnterior, TipoSubproblemaSolver,      Nenhum,          max,        Nenhum,      sim) \
	  m(VariavelEstado,  AttComum,     idVariavelDecisaoEstagioAnterior,                   int,          -1,          max,            -1,      sim) 
//     c_classe,   smrtAtt,                                nomeAtributo,                  tipo,  lowerBound,   upperBound,  initialValue, mustRead?


#define ATT_VETOR_VARIAVEL_ESTADO(m)  \
	  m(VariavelEstado,   AttVetor,  idVariavelDecisao,     int,        -1,          max,             -1,  TipoSubproblemaSolver)\
	  m(VariavelEstado,   AttVetor,              valor,  double,        min,          max,             0,  IdCenario)
//            c_classe,    smrtAtt,       nomeAtributo,    Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador


#define SMART_ELEMENTO_VARIAVEL_ESTADO(m) \
	m(VariavelEstado, AttComum,   ATT_COMUM_VARIAVEL_ESTADO)  \
    m(VariavelEstado, AttVetor,   ATT_VETOR_VARIAVEL_ESTADO)  

DEFINE_SMART_ELEMENTO(VariavelEstado, SMART_ELEMENTO_VARIAVEL_ESTADO)

class VariavelEstado : public SmartDados {

public:

	VariavelEstado();
	VariavelEstado(const VariavelEstado &instanciaCopiar);
	void esvaziar();
	virtual ~VariavelEstado();

	DECLARA_SMART_ELEMENTO(VariavelEstado, SMART_ELEMENTO_VARIAVEL_ESTADO)


};

GET_STRING_FROM_CLASS(VariavelEstado)

#endif 
