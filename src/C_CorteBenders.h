#ifndef CORTE_BENDERS
#define CORTE_BENDERS

#include "C_SmartElemento.h"

#define ATT_COMUM_CORTE_BENDERS(m)  \
	  m(CorteBenders,  AttComum,     idCorteBenders,   IdCorteBenders,         min,         max,           min,      sim) \
	  m(CorteBenders,  AttComum,     is_corte_dominado,           int,           0,           1,             0,      nao) 
//          c_classe,   smrtAtt,       nomeAtributo,             tipo,  lowerBound,  upperBound,  initialValue, mustRead?


#define ATT_VETOR_CORTE_BENDERS(m)  \
	  m(CorteBenders,  AttVetor,                         rhs,          double,        min,          max,             0,  IdRealizacao)     \
	  m(CorteBenders,  AttVetor,                  rhs_primal,          double,        min,          max,             0,  IdRealizacao)     \
	  m(CorteBenders,  AttVetor,                  rhs_estado,          double,        min,          max,             0,  IdRealizacao)     \
	  m(CorteBenders,  AttVetor,           is_corte_dominado,             int,          0,            1,             0,  IdRealizacao)     \
	  m(CorteBenders,  AttVetor,                      estado,          double,        min,          max,             0,  IdVariavelEstado) 


#define ATT_MATRIZ_CORTE_BENDERS(m)  \
	  m(CorteBenders,  AttMatriz,                 coeficiente,          double,        min,          max,             0,  IdRealizacao, IdVariavelEstado)\
	  m(CorteBenders,  AttMatriz,     custo_cortes_dominantes,          double,        min,          max,             0,  IdRealizacao, int) \
	  m(CorteBenders,  AttMatriz, ordenacao_cortes_dominantes,  IdCorteBenders,        min,          max,           min,  IdRealizacao, int) 
//          c_classe,    smrtAtt,                nomeAtributo,            Tipo, lowerBound,   upperBound,  initialValue,  TipoIterador1, TipoIterador2


#define SMART_ELEMENTO_CORTE_BENDERS(m) \
	m(CorteBenders, AttComum,  ATT_COMUM_CORTE_BENDERS) \
	m(CorteBenders, AttVetor,  ATT_VETOR_CORTE_BENDERS) \
	m(CorteBenders, AttMatriz, ATT_MATRIZ_CORTE_BENDERS) 

DEFINE_SMART_ELEMENTO(CorteBenders, SMART_ELEMENTO_CORTE_BENDERS)

class CorteBenders : public SmartDados {
public:

	CorteBenders();
	CorteBenders(const CorteBenders &instanciaCopiar);
	void esvaziar();
	virtual ~CorteBenders();

	DECLARA_SMART_ELEMENTO(CorteBenders, SMART_ELEMENTO_CORTE_BENDERS)


	void anularVariavelEstado(const IdVariavelEstado a_idVariavelEstado);

	void sequencializaOrdenacaoCorteDominante(const SmartEnupla<IdCorteBenders, IdCorteBenders> &a_mapeamento_cortes);

};

GET_STRING_FROM_CLASS(CorteBenders)

#endif 
