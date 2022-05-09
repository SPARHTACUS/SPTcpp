#ifndef POLINOMIO_JUSANTE_H
#define POLINOMIO_JUSANTE_H

#include "C_SmartElemento.h"

#define ATT_COMUM_POLINOMIO_JUSANTE(m)  \
	  m(PolinomioJusante,  AttComum,          idPolinomioJusante, IdPolinomioJusante,        min,          max,           min,      sim) \
	  m(PolinomioJusante,  AttComum,                  altura_ref,             double,          0,         1000,             0,      sim) \
	  m(PolinomioJusante,  AttComum,          altura_jusante_ref,             double,          0,         1000,             0,      sim) \
	  m(PolinomioJusante,  AttComum,           defluencia_minima,             double,          0,          max,             0,      sim) \
	  m(PolinomioJusante,  AttComum,           defluencia_maxima,             double,          0,          max,           max,      sim) \
	  m(PolinomioJusante,  AttComum,           numero_polinomios,                int,          0,           10,             0,      sim) \
	  m(PolinomioJusante,  AttComum,               coeficiente_0,             double,       -200,          max,             0,      sim) \
	  m(PolinomioJusante,  AttComum,               coeficiente_1,             double,        min,          max,             0,      sim) \
	  m(PolinomioJusante,  AttComum,               coeficiente_2,             double,        min,          max,             0,      sim) \
	  m(PolinomioJusante,  AttComum,               coeficiente_3,             double,        min,          max,             0,      sim) \
	  m(PolinomioJusante,  AttComum,               coeficiente_4,             double,        min,          max,             0,      sim) \
	  m(PolinomioJusante,  AttComum,                        nome,             string,        min,          max,             0,      nao) \
      m(PolinomioJusante,  AttComum,                 curva_chave,                int,          0,          100,             0,      nao)


//              c_classe,    smrtAtt,               nomeAtributo,               tipo, lowerBound,   upperBound,  initialValue, mustRead?

#define ATT_VETOR_POLINOMIO_JUSANTE(m)  \
      m(PolinomioJusante,  AttVetor,                   altura_ref,               double,        0,         1000,           0,        Periodo) \
      m(PolinomioJusante,  AttVetor,           altura_jusante_ref,               double,      0.0,         1000,           0,        Periodo) \
	  m(PolinomioJusante,  AttVetor,                coeficiente_0,               double,     -200,          max,           0,        Periodo) \
      m(PolinomioJusante,  AttVetor,                coeficiente_1,               double,       -1,          max,           0,        Periodo) \
      m(PolinomioJusante,  AttVetor,                coeficiente_2,               double,       -1,          max,           0,        Periodo) \
      m(PolinomioJusante,  AttVetor,                coeficiente_3,               double,       -1,          max,           0,        Periodo) \
      m(PolinomioJusante,  AttVetor,                coeficiente_4,               double,       -1,          max,           0,        Periodo)

//          c_classe,   smrtAtt,                       nomeAtributo,         Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador


#define SMART_ELEMENTO_POLINOMIO_JUSANTE(m) \
	m(PolinomioJusante, AttComum, ATT_COMUM_POLINOMIO_JUSANTE) \
    m(PolinomioJusante, AttVetor, ATT_VETOR_POLINOMIO_JUSANTE)

DEFINE_SMART_ELEMENTO(PolinomioJusante, SMART_ELEMENTO_POLINOMIO_JUSANTE)

class PolinomioJusante : public SmartDados {

public:
	
	PolinomioJusante();			// Construtor
	PolinomioJusante(const PolinomioJusante &instanciaCopiar);
	void esvaziar();
	virtual ~PolinomioJusante();	// Destrutor

	DECLARA_SMART_ELEMENTO(PolinomioJusante, SMART_ELEMENTO_POLINOMIO_JUSANTE)

};

GET_STRING_FROM_CLASS(PolinomioJusante)

#endif // POLINOMIO_JUSANTE_H
