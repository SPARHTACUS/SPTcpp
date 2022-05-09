#ifndef DEFLUENCIA_H
#define DEFLUENCIA_H

#include "C_SmartElemento.h"


#define ATT_COMUM_DEFLUENCIA(m)  \
	  m(Defluencia,  AttComum,               idDefluencia,         IdDefluencia,         min,         max,           min,      nao) \
	  m(Defluencia,  AttComum,      tipo_elemento_jusante,  TipoElementoJusante,      Nenhum,         max,        Nenhum,      nao)
//       c_classe,   smrtAtt,                nomeAtributo,                 tipo,  lowerBound,  upperBound,  initialValue, mustRead?

#define ATT_VETOR_DEFLUENCIA(m)  \
      m(Defluencia,  AttVetor,           vazao_defluencia,               double,          0,          max,             0,    Periodo) 
//        c_classe,   smrtAtt,               nomeAtributo,                 Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador

#define SMART_ELEMENTO_DEFLUENCIA(m) \
	m(Defluencia, AttComum, ATT_COMUM_DEFLUENCIA) \
    m(Defluencia, AttVetor, ATT_VETOR_DEFLUENCIA)


DEFINE_SMART_ELEMENTO(Defluencia, SMART_ELEMENTO_DEFLUENCIA)


class Defluencia : public SmartDados {

	friend class Dados;
	friend class Hidreletrica;

public:
	 
	Defluencia();
	Defluencia(const Defluencia &instanciaCopiar);
	void esvaziar();
	virtual ~Defluencia();

	DECLARA_SMART_ELEMENTO(Defluencia, SMART_ELEMENTO_DEFLUENCIA)

};

GET_STRING_FROM_CLASS(Defluencia)

#endif // AFLUENCIA_H
