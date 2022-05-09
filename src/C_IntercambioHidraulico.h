#ifndef INTERCAMBIO_HIDRAULICO
#define INTERCAMBIO_HIDRAULICO

#include "C_SmartElemento.h"


#define ATT_COMUM_INTERCAMBIO_HIDRAULICO(m)  \
	  m(IntercambioHidraulico,  AttComum,          idIntercambioHidraulico,   IdIntercambioHidraulico,         min,          max,           min,      sim) \
      m(IntercambioHidraulico,  AttComum,      tipo_intercambio_hidraulico, TipoIntercambioHidraulico,         min,          max,           min,      sim) \
	  m(IntercambioHidraulico,  AttComum,              hidreletrica_origem,            IdHidreletrica,         min,          max,           min,      sim) \
	  m(IntercambioHidraulico,  AttComum,             hidreletrica_destino,            IdHidreletrica,      Nenhum,          max,        Nenhum,      sim) \
	  m(IntercambioHidraulico,  AttComum,      desnivel_entre_reservatorio,                    double,           0,          max,             0,          nao) \
      m(IntercambioHidraulico,  AttComum,               desvio_agua_minimo,                    double,         min,          max,             0,      sim) \
      m(IntercambioHidraulico,  AttComum,               desvio_agua_maximo,                    double,         min,          max,           max,      sim) \
      m(IntercambioHidraulico,  AttComum,             volume_minimo_desvio,                    double,           0,          max,           max,      sim) \
      m(IntercambioHidraulico,  AttComum,     percentual_retorno_do_desvio,                    double,           0,            1,             0,      sim) \
      m(IntercambioHidraulico,  AttComum,              penalidade_retirada,                    double,           0,          max,          5000,      sim) 
//       c_classe,             smrtAtt,                     nomeAtributo,                    tipo,   lowerBound,  upperBound,  initialValue,  mustRead?


#define ATT_VETOR_INTERCAMBIO_HIDRAULICO(m)  \
	  m(IntercambioHidraulico,  AttVetor,                      desnivel,      double,          0,          max,            0,           int)  \
	  m(IntercambioHidraulico,  AttVetor,            nivel_reservatorio,      double,          0,          max,            0,           int)  \
      m(IntercambioHidraulico,  AttVetor,            desvio_agua_minimo,      double,        min,          max,          min,        Periodo) \
      m(IntercambioHidraulico,  AttVetor,            desvio_agua_maximo,      double,        min,          max,          max,        Periodo) 
//       c_classe,             smrtAtt,                 nomeAtributo,       Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador


#define ATT_MATRIZ_INTERCAMBIO_HIDRAULICO(m)  \
	  m(IntercambioHidraulico,  AttMatriz,                     vazao,      double,          0,          max,            0,          int, int)  
//       c_classe,             smrtAtt,                 nomeAtributo,       Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador


#define SMART_ELEMENTO_INTERCAMBIO_HIDRAULICO(m) \
	m(IntercambioHidraulico, AttComum, ATT_COMUM_INTERCAMBIO_HIDRAULICO) \
	m(IntercambioHidraulico, AttVetor, ATT_VETOR_INTERCAMBIO_HIDRAULICO) \
	m(IntercambioHidraulico, AttMatriz, ATT_MATRIZ_INTERCAMBIO_HIDRAULICO) 


DEFINE_SMART_ELEMENTO(IntercambioHidraulico, SMART_ELEMENTO_INTERCAMBIO_HIDRAULICO)


class IntercambioHidraulico : public SmartDados {

	friend class Dados;
	friend class Hidreletrica;

public:
	 
	IntercambioHidraulico();
	IntercambioHidraulico(const IntercambioHidraulico &instanciaCopiar);
	void esvaziar();
	virtual ~IntercambioHidraulico();

	DECLARA_SMART_ELEMENTO(IntercambioHidraulico, SMART_ELEMENTO_INTERCAMBIO_HIDRAULICO)

};

GET_STRING_FROM_CLASS(IntercambioHidraulico)

#endif // AFLUENCIA_H
