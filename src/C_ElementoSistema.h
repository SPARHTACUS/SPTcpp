#ifndef ELEMENTOS_SISTEMA_H
#define ELEMENTOS_SISTEMA_H

#include "C_SmartElemento.h"


#define ATT_COMUM_ELEMENTO_SISTEMA(m)  \
	  m(ElementoSistema,  AttComum,                 idElementoSistema,                IdElementoSistema,         min,         max,           min,      sim) \
	  m(ElementoSistema,  AttComum,                     tipo_elemento,              TipoElementoSistema,         min,         max,           min,      sim) \
	  m(ElementoSistema,  AttComum,                 unidadehidraulica,                     IdUnidadeUHE,      Nenhum,         max,        Nenhum,      sim) \
	  m(ElementoSistema,  AttComum,                conjuntohidraulico,             IdConjuntoHidraulico,      Nenhum,         max,        Nenhum,      sim) \
	  m(ElementoSistema,  AttComum,                      hidreletrica,                   IdHidreletrica,      Nenhum,         max,        Nenhum,      sim) \
	  m(ElementoSistema,  AttComum,                      termeletrica,                   IdTermeletrica,      Nenhum,         max,        Nenhum,      sim) \
	  m(ElementoSistema,  AttComum,                   demandaEspecial,                IdDemandaEspecial,      Nenhum,         max,        Nenhum,      nao) \
	  m(ElementoSistema,  AttComum,               unidadeTermeletrica,                     IdUnidadeUTE,      Nenhum,         max,        Nenhum,      sim) \
	  m(ElementoSistema,  AttComum,                          contrato,                       IdContrato,      Nenhum,         max,        Nenhum,      sim) \
	  m(ElementoSistema,  AttComum,                       intercambio,                    IdIntercambio,      Nenhum,         max,        Nenhum,      sim) \
	  m(ElementoSistema,  AttComum,                            eolica,                    IdUsinaEolica,      Nenhum,         max,        Nenhum,      sim) \
      m(ElementoSistema,  AttComum,                  usina_elevatoria,                IdUsinaElevatoria,      Nenhum,         max,        Nenhum,      sim) \
      m(ElementoSistema,  AttComum,   tipoVariavelRestricaoOperativa,    TipoVariavelRestricaoOperativa,      Nenhum,         max,        Nenhum,      sim) \
	  m(ElementoSistema,  AttComum,                fator_participacao,                           double,         -10,          10,             0,      nao)
//              c_classe,   smrtAtt,          nomeAtributo,          tipo,  lowerBound,  upperBound,  initialValue, mustRead?


#define ATT_VETOR_ELEMENTO_SISTEMA(m)  \
	  m(ElementoSistema,  AttVetor,  fator_participacao,     double,        min,          max,           0,      Periodo) 
//               c_classe,   smrtAtt,     nomeAtributo,        Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador


#define ATT_MATRIZ_ELEMENTO_SISTEMA(m)  \
	  m(ElementoSistema,  AttMatriz, fator_participacao,          double,         min,          max,             0,  Periodo, IdPatamarCarga) 
//               c_classe,    smrtAtt,                      nomeAtributo,       Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador


#define SMART_ELEMENTO_ELEMENTO_SISTEMA(m) \
	m(ElementoSistema,  AttComum,  ATT_COMUM_ELEMENTO_SISTEMA) \
    m(ElementoSistema,  AttVetor,  ATT_VETOR_ELEMENTO_SISTEMA) \
    m(ElementoSistema,  AttMatriz, ATT_MATRIZ_ELEMENTO_SISTEMA) 

DEFINE_SMART_ELEMENTO(ElementoSistema, SMART_ELEMENTO_ELEMENTO_SISTEMA)


class ElementoSistema : public SmartDados {

	friend class Dados;

public:
	 
	ElementoSistema();
	ElementoSistema(const ElementoSistema &instanciaCopiar);
	void esvaziar();
	virtual ~ElementoSistema();

	DECLARA_SMART_ELEMENTO(ElementoSistema, SMART_ELEMENTO_ELEMENTO_SISTEMA)

};

GET_STRING_FROM_CLASS(ElementoSistema)

#endif // AFLUENCIA_H
