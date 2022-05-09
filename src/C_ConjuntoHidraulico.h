#ifndef CONJUNTO_HIDRAULICO_H
#define CONJUNTO_HIDRAULICO_H

#include "C_SmartElemento.h"

#include "C_FuncaoProducaoHidreletrica.h"
#include "C_UnidadeUHE.h"

#define ATT_COMUM_CONJUNTO_HIDRAULICO(m)  \
	  m(ConjuntoHidraulico,  AttComum,        idConjuntoHidraulico,            IdConjuntoHidraulico,         min,          max,           min,         sim) \
	  m(ConjuntoHidraulico,  AttComum,            queda_referencia,                          double,           0,         1000,             0,         nao) \
	  m(ConjuntoHidraulico,  AttComum,                  submercado,                    IdSubmercado,      Nenhum,          max,        Nenhum,         sim) \
	  m(ConjuntoHidraulico,  AttComum,                        nome,                          string,         min,          max,           min,         sim) 
//                c_classe,   smrtAtt,                nomeAtributo,                            tipo,  lowerBound,   upperBound,  initialValue,    mustRead?

#define ATT_VETOR_CONJUNTO_HIDRAULICO(m)  \
	  m(ConjuntoHidraulico,  AttVetor,             vazao_turbinada_minima,           double,          0,        50000,             0,    Periodo) \
	  m(ConjuntoHidraulico,  AttVetor,             vazao_turbinada_maxima,           double,          0,        50000,             0,    Periodo) \
	  m(ConjuntoHidraulico,  AttVetor,                    disponibilidade,           double,          0,            1,             0,    Periodo) 
//                c_classe,   smrtAtt,                       nomeAtributo,             Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador

#define ATT_MATRIZ_CONJUNTO_HIDRAULICO(m)  \
	  m(ConjuntoHidraulico,  AttMatriz,                         potencia_minima,           double,        0,        20000,             0,    Periodo,     IdPatamarCarga) \
	  m(ConjuntoHidraulico,  AttMatriz,                         potencia_maxima,           double,        0,        20000,             0,    Periodo,     IdPatamarCarga) \
	  m(ConjuntoHidraulico,  AttMatriz,              potencia_disponivel_minima,           double,        0,        20000,             0,    Periodo,     IdPatamarCarga) \
	  m(ConjuntoHidraulico,  AttMatriz,              potencia_disponivel_maxima,           double,        0,        20000,             0,    Periodo,     IdPatamarCarga)
//                c_classe,    smrtAtt,                 nomeAtributo,            Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador

#define MEMBRO_CONJUNTO_HIDRAULICO(m)                  \
	m(ConjuntoHidraulico, FuncaoProducaoHidreletrica)  \
	m(ConjuntoHidraulico, UnidadeUHE) 

#define SMART_ELEMENTO_CONJUNTO_HIDRAULICO(m) \
	m(ConjuntoHidraulico,  AttComum, ATT_COMUM_CONJUNTO_HIDRAULICO)   \
	m(ConjuntoHidraulico,  AttVetor, ATT_VETOR_CONJUNTO_HIDRAULICO)   \
    m(ConjuntoHidraulico, AttMatriz, ATT_MATRIZ_CONJUNTO_HIDRAULICO) \
	m(ConjuntoHidraulico,    Membro, MEMBRO_CONJUNTO_HIDRAULICO)

DEFINE_SMART_ELEMENTO(ConjuntoHidraulico, SMART_ELEMENTO_CONJUNTO_HIDRAULICO)


class ConjuntoHidraulico : public SmartDados {

public:

	ConjuntoHidraulico();			// Construtor
	ConjuntoHidraulico(const ConjuntoHidraulico &instanciaCopiar);
	void esvaziar();
	virtual ~ConjuntoHidraulico();	// Destrutor

	DECLARA_SMART_ELEMENTO(ConjuntoHidraulico, SMART_ELEMENTO_CONJUNTO_HIDRAULICO)

};

GET_STRING_FROM_CLASS(ConjuntoHidraulico)

#endif // C_CONJUNTOHIDRAULICO_H
