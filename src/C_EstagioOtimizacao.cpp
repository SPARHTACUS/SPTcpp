#include "C_EstagioOtimizacao.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Estagio::Estagio(){
	try { 
		INICIALIZA_SMART_ELEMENTO(Estagio, SMART_ELEMENTO_ESTAGIO_OTIMIZACAO)

		vetorPtrSolver = std::vector<Solver*>(TipoSubproblemaSolver_Excedente, nullptr);

	}
	catch (const std::exception&erro) { throw std::invalid_argument("Estagio::Estagio(): \n" + std::string(erro.what())); }
}

Estagio::Estagio(const Estagio &instanciaCopiar){
	try { 
		COPIA_SMART_ELEMENTO(Estagio, SMART_ELEMENTO_ESTAGIO_OTIMIZACAO)

			if (vetorPtrSolver.size() != TipoSubproblemaSolver_Excedente)
				vetorPtrSolver = std::vector<Solver*>(TipoSubproblemaSolver_Excedente, nullptr);

			for (TipoSubproblemaSolver tipoSubproblemaSolver = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tipoSubproblemaSolver < TipoSubproblemaSolver_Excedente; tipoSubproblemaSolver++) {
				if (instanciaCopiar.vetorPtrSolver.at(tipoSubproblemaSolver) != nullptr)
					throw std::invalid_argument("Nao eh possivel copiar uma instancia de Estagio cujo solver ja foi instanciado.");
				vetorPtrSolver.at(tipoSubproblemaSolver) = nullptr;
			}
	}
	catch (const std::exception&erro) { throw std::invalid_argument("Estagio::Estagio(const Estagio &instanciaCopiar): \n" + std::string(erro.what())); }
}

void Estagio::esvaziar() {
	try {
		excluirInstanciasSolver();

		EXCLUI_SMART_ELEMENTO(Estagio, SMART_ELEMENTO_ESTAGIO_OTIMIZACAO)

	}
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::esvaziar(): \n" + std::string(erro.what())); }
}

Estagio::~Estagio(){

	excluirInstanciasSolver();

	EXCLUI_SMART_ELEMENTO(Estagio, SMART_ELEMENTO_ESTAGIO_OTIMIZACAO)
} // Estagio::~Estagio(){


IdVariavelEstado Estagio::addVariavelEstado(const TipoSubproblemaSolver a_TSS, const string a_nome, const int a_idVariavelDecisao, const int a_idVariavelDecisaoEstagioAnterior, bool is_externa) {

	try {

		const IdVariavelEstado maiorIdVariavelEstado = getMaiorId(IdVariavelEstado());

		for (IdVariavelEstado idVariavelEstado = maiorIdVariavelEstado; idVariavelEstado > IdVariavelEstado_Nenhum; idVariavelEstado--) {

			if (vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

				if (strCompara(getAtributo(idVariavelEstado, AttComumVariavelEstado_nome, string()), a_nome)) {

					if ((getElementoVetor(idVariavelEstado, AttVetorVariavelEstado_idVariavelDecisao, a_TSS, int()) == a_idVariavelDecisao) && (getAtributo(idVariavelEstado, AttComumVariavelEstado_tipoSubproblemaSolverEstagioAnterior, TipoSubproblemaSolver()) == a_TSS) &&
						(getAtributo(idVariavelEstado, AttComumVariavelEstado_idVariavelDecisaoEstagioAnterior, int()) == a_idVariavelDecisaoEstagioAnterior))
						return idVariavelEstado;

					if (getElementoVetor(idVariavelEstado, AttVetorVariavelEstado_idVariavelDecisao, a_TSS, int()) != -1)
						throw std::invalid_argument("O atributo " + getFullString(AttVetorVariavelEstado_idVariavelDecisao) + " ja foi instanciado com varDecisao " + getString(getElementoVetor(idVariavelEstado, AttVetorVariavelEstado_idVariavelDecisao, a_TSS, int())) + ".");

					vetorVariavelEstado.at(idVariavelEstado).setElemento(AttVetorVariavelEstado_idVariavelDecisao, a_TSS, a_idVariavelDecisao);

					return idVariavelEstado;

				} // if (strCompara(getAtributo(idVariavelEstado, AttComumVariavelEstado_nome, string()), a_nome)) {

			} // if (vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

		} // for (IdVariavelEstado idVariavelEstado = maiorIdVariavelEstado; idVariavelEstado >= menorIdVariavelEstado; idVariavelEstado--) {

		const IdVariavelEstado idVariavelEstado = IdVariavelEstado(maiorIdVariavelEstado + 1);

		VariavelEstado variavelEstado;
		variavelEstado.setAtributo(AttComumVariavelEstado_idVariavelEstado, idVariavelEstado);

		vetorVariavelEstado.add(variavelEstado);

		vetorVariavelEstado.at(idVariavelEstado).setAtributo(AttComumVariavelEstado_nome, a_nome);
		vetorVariavelEstado.at(idVariavelEstado).setAtributo(AttComumVariavelEstado_tipoSubproblemaSolverEstagioAnterior, a_TSS);
		vetorVariavelEstado.at(idVariavelEstado).setAtributo(AttComumVariavelEstado_idVariavelDecisaoEstagioAnterior, a_idVariavelDecisaoEstagioAnterior);
		vetorVariavelEstado.at(idVariavelEstado).setAtributo(AttComumVariavelEstado_is_externa, is_externa);

		vetorVariavelEstado.at(idVariavelEstado).setVetor_forced(AttVetorVariavelEstado_idVariavelDecisao, SmartEnupla<TipoSubproblemaSolver, int>(TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1), std::vector<int>(TipoSubproblemaSolver(TipoSubproblemaSolver_Excedente - 1), -1)));

		vetorVariavelEstado.at(idVariavelEstado).setElemento(AttVetorVariavelEstado_idVariavelDecisao, a_TSS, a_idVariavelDecisao);

		return idVariavelEstado;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio(" + getString(getIdObjeto()) + ")::addVariavelEstado(" + getString(a_TSS) + "," + a_nome + "," + getString(a_idVariavelDecisao) + "," + getString(a_idVariavelDecisaoEstagioAnterior) + "," + getString(is_externa) + "): \n" + std::string(erro.what())); }

} // IdVariavelEstado Estagio::adicionarVariavelEstado(const string a_nome, const IdEstagio a_idEstagio, const int a_indice_variavel_decisao, const IdEstagio a_idEstagioDE, const int a_indice_variavel_decisaoDE){


IdVariavelRealizacao Estagio::addVariavelRealizacao(const TipoSubproblemaSolver a_TSS, const string a_nome, const int a_idVariavelDecisao, const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria, const Periodo a_periodo, const double a_fator) {

	try {

		const IdVariavelRealizacao maiorIdVariavelRealizacao = getMaiorId(IdVariavelRealizacao());

		for (IdVariavelRealizacao idVariavelRealizacao = IdVariavelRealizacao(IdVariavelRealizacao_Nenhum + 1); idVariavelRealizacao <= maiorIdVariavelRealizacao; idVariavelRealizacao++) {

			if (strCompara(getAtributo(idVariavelRealizacao, AttComumVariavelRealizacao_nome, string()), a_nome)) {

				if (getElementoVetor(idVariavelRealizacao, AttVetorVariavelRealizacao_idVariavelDecisao, a_TSS, int()) != -1)
					throw std::invalid_argument("O atributo " + getFullString(AttVetorVariavelRealizacao_idVariavelDecisao) + " ja foi instanciado.");

				vetorVariavelRealizacao.at(idVariavelRealizacao).setElemento(AttVetorVariavelRealizacao_idVariavelDecisao, a_TSS, a_idVariavelDecisao);

				return idVariavelRealizacao;

			} // if (strCompara(getAtributo(idVariavelRealizacao, AttComumVariavelRealizacao_nome, string()), a_nome)) {

		} // for (IdVariavelRealizacao idVariavelRealizacao = IdVariavelRealizacao(IdVariavelRealizacao_Nenhum + 1); idVariavelRealizacao <= maiorIdVariavelRealizacao; idVariavelRealizacao++) {

		const IdVariavelRealizacao idVariavelRealizacao = IdVariavelRealizacao(maiorIdVariavelRealizacao + 1);

		VariavelRealizacao variavelRealizacao;
		variavelRealizacao.setAtributo(AttComumVariavelRealizacao_idVariavelRealizacao, idVariavelRealizacao);

		vetorVariavelRealizacao.add(variavelRealizacao);

		vetorVariavelRealizacao.at(idVariavelRealizacao).setAtributo(AttComumVariavelRealizacao_nome, a_nome);
		vetorVariavelRealizacao.at(idVariavelRealizacao).setAtributo(AttComumVariavelRealizacao_idProcessoEstocastico, a_idProcessoEstocastico);
		vetorVariavelRealizacao.at(idVariavelRealizacao).setAtributo(AttComumVariavelRealizacao_idVariavelAleatoria,   a_idVariavelAleatoria);
		vetorVariavelRealizacao.at(idVariavelRealizacao).setAtributo(AttComumVariavelRealizacao_periodo,               a_periodo);

		vetorVariavelRealizacao.at(idVariavelRealizacao).setVetor_forced(AttVetorVariavelRealizacao_idVariavelDecisao, SmartEnupla<TipoSubproblemaSolver, int>(TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1), std::vector<int>(TipoSubproblemaSolver(TipoSubproblemaSolver_Excedente - 1), -1)));

		vetorVariavelRealizacao.at(idVariavelRealizacao).setElemento(AttVetorVariavelRealizacao_idVariavelDecisao, a_TSS, a_idVariavelDecisao);

		return idVariavelRealizacao;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio(" + getString(getIdObjeto()) + ")::addVariavelRealizacao(" + getString(a_TSS) + "," + a_nome + "," + getString(a_idVariavelDecisao) + "," + getFullString(a_idProcessoEstocastico) + "," + getFullString(a_idVariavelAleatoria) + "): \n" + std::string(erro.what())); }

} // IdVariavelRealizacao Estagio::adicionarVariavelRealizacao(const string a_nome, const IdEstagio a_idEstagio, const int a_idVariavelDecisao, const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria){

IdRestricaoCenario Estagio::addRestricaoCenario(const TipoSubproblemaSolver a_TSS, const string a_nome, const int a_idRestricao, const SmartEnupla<IdCenario, double>& a_rhs, const SmartEnupla<int, SmartEnupla<IdCenario, double>> &a_coeficiente) {
	
	try{

		const IdRestricaoCenario maiorIdRestricaoCenario = getMaiorId(IdRestricaoCenario());

		const IdRestricaoCenario idRestricaoCenario = IdRestricaoCenario(maiorIdRestricaoCenario + 1);

		RestricaoCenario variavelRealizacao;
		variavelRealizacao.setAtributo(AttComumRestricaoCenario_idRestricaoCenario, idRestricaoCenario);

		vetorRestricaoCenario.add(variavelRealizacao);

		vetorRestricaoCenario.at(idRestricaoCenario).setAtributo(AttComumRestricaoCenario_nome, a_nome);

		vetorRestricaoCenario.at(idRestricaoCenario).setVetor_forced(AttVetorRestricaoCenario_idRestricao, SmartEnupla<TipoSubproblemaSolver, int>(TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1), std::vector<int>(TipoSubproblemaSolver(TipoSubproblemaSolver_Excedente - 1), -1)));

		vetorRestricaoCenario.at(idRestricaoCenario).setElemento(AttVetorRestricaoCenario_idRestricao, a_TSS, a_idRestricao);

		vetorRestricaoCenario.at(idRestricaoCenario).setVetor_forced(AttVetorRestricaoCenario_rhs, a_rhs);
		vetorRestricaoCenario.at(idRestricaoCenario).setMatriz_forced(AttMatrizRestricaoCenario_coeficiente, a_coeficiente);

		return idRestricaoCenario;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio(" + getString(getIdObjeto()) + ")::addVariavelRealizacao(" + getString(a_TSS) + "," + a_nome + "," + getString(a_idRestricao) + "): \n" + std::string(erro.what())); }

}


IdVariavelRealizacaoInterna Estagio::addVariavelRealizacaoInterna(const TipoSubproblemaSolver a_TSS, const string a_nome, const int a_idVariavelDecisao, const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria, const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const Periodo a_periodo, const double a_fator, const TipoValor a_tipo_valor, const double a_percentual_inicial, const double a_percentual_passo){

	try {

		if ((a_percentual_inicial == 1.0) && (a_percentual_inicial + a_percentual_passo <= 1.0))
			throw std::invalid_argument("Para percentual inicial igual a 1.0, o percentual passo deve ser positivo.");

		const IdVariavelRealizacaoInterna maiorIdVariavelRealizacaoInterna = getMaiorId(IdVariavelRealizacaoInterna());

		for (IdVariavelRealizacaoInterna idVariavelRealizacaoInterna = IdVariavelRealizacaoInterna(IdVariavelRealizacaoInterna_Nenhum + 1); idVariavelRealizacaoInterna <= maiorIdVariavelRealizacaoInterna; idVariavelRealizacaoInterna++) {

			if (strCompara(getAtributo(idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_nome, string()), a_nome)) {

				if (getElementoVetor(idVariavelRealizacaoInterna, AttVetorVariavelRealizacaoInterna_idVariavelDecisao, a_TSS, int()) != -1)
					throw std::invalid_argument("O atributo " + getFullString(AttVetorVariavelRealizacaoInterna_idVariavelDecisao) + " ja foi instanciado.");

				vetorVariavelRealizacaoInterna.at(idVariavelRealizacaoInterna).setElemento(AttVetorVariavelRealizacaoInterna_idVariavelDecisao, a_TSS, a_idVariavelDecisao);

				return idVariavelRealizacaoInterna;

			} // if (strCompara(getAtributo(idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_nome, string()), a_nome)) {

		} // for (IdVariavelRealizacaoInterna idVariavelRealizacaoInterna = IdVariavelRealizacaoInterna(IdVariavelRealizacaoInterna_Nenhum + 1); idVariavelRealizacaoInterna <= maiorIdVariavelRealizacaoInterna; idVariavelRealizacaoInterna++) {

		const IdVariavelRealizacaoInterna idVariavelRealizacaoInterna = IdVariavelRealizacaoInterna(maiorIdVariavelRealizacaoInterna + 1);

		VariavelRealizacaoInterna variavelRealizacaoInterna;
		variavelRealizacaoInterna.setAtributo(AttComumVariavelRealizacaoInterna_idVariavelRealizacaoInterna, idVariavelRealizacaoInterna);

		vetorVariavelRealizacaoInterna.add(variavelRealizacaoInterna);

		vetorVariavelRealizacaoInterna.at(idVariavelRealizacaoInterna).setAtributo(AttComumVariavelRealizacaoInterna_nome, a_nome);
		vetorVariavelRealizacaoInterna.at(idVariavelRealizacaoInterna).setAtributo(AttComumVariavelRealizacaoInterna_idVariavelAleatoriaInterna,   a_idVariavelAleatoriaInterna);
		vetorVariavelRealizacaoInterna.at(idVariavelRealizacaoInterna).setAtributo(AttComumVariavelRealizacaoInterna_idVariavelAleatoria,          a_idVariavelAleatoria);
		vetorVariavelRealizacaoInterna.at(idVariavelRealizacaoInterna).setAtributo(AttComumVariavelRealizacaoInterna_idProcessoEstocastico,        a_idProcessoEstocastico);
		vetorVariavelRealizacaoInterna.at(idVariavelRealizacaoInterna).setAtributo(AttComumVariavelRealizacaoInterna_periodo,                      a_periodo);
		vetorVariavelRealizacaoInterna.at(idVariavelRealizacaoInterna).setAtributo(AttComumVariavelRealizacaoInterna_tipo_valor,                   a_tipo_valor);
		vetorVariavelRealizacaoInterna.at(idVariavelRealizacaoInterna).setAtributo(AttComumVariavelRealizacaoInterna_percentual_inicial,           a_percentual_inicial);
		vetorVariavelRealizacaoInterna.at(idVariavelRealizacaoInterna).setAtributo(AttComumVariavelRealizacaoInterna_percentual_passo,             a_percentual_passo);
		vetorVariavelRealizacaoInterna.at(idVariavelRealizacaoInterna).setAtributo(AttComumVariavelRealizacaoInterna_percentual_a_utilizar,        a_percentual_passo);

		vetorVariavelRealizacaoInterna.at(idVariavelRealizacaoInterna).setVetor_forced(AttVetorVariavelRealizacaoInterna_idVariavelDecisao, SmartEnupla<TipoSubproblemaSolver, int>(TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1), std::vector<int>(TipoSubproblemaSolver(TipoSubproblemaSolver_Excedente - 1), -1)));

		vetorVariavelRealizacaoInterna.at(idVariavelRealizacaoInterna).setElemento(AttVetorVariavelRealizacaoInterna_idVariavelDecisao, a_TSS, a_idVariavelDecisao);

		return idVariavelRealizacaoInterna;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio(" + getString(getIdObjeto()) + ")::addVariavelRealizacaoInterna(" + getString(a_TSS) + "," + a_nome + "," + getString(a_idVariavelDecisao) + "," + getFullString(a_idProcessoEstocastico) + "," + getFullString(a_idVariavelAleatoria) + "," + getFullString(a_idVariavelAleatoriaInterna) + "," + getFullString(a_periodo) + "," + getFullString(a_tipo_valor) + "," + getFullString(a_percentual_inicial) + "," + getFullString(a_percentual_passo) + "): \n" + std::string(erro.what())); }


} // IdVariavelRealizacaoInterna Estagio::addVariavelRealizacaoInterna(const string a_nome, const int a_idVariavelDecisao, const IdProcessoEstocastico a_idProcessoEstocastico, const IdVariavelAleatoria a_idVariavelAleatoria, const IdVariavelAleatoriaInterna a_idVariavelAleatoriaInterna, const TipoValor a_tipo_valor, const double percentual_inicial, const double percentual_incremento){



void Estagio::addValorVariavelEstado(const IdVariavelEstado a_idVariavelEstado, const IdCenario a_idCenario, const IdCenario a_menorIdCenario, const IdCenario a_maiorIdCenario, const double a_valor){
	try {

		if ((getSizeVetor(a_idVariavelEstado, AttVetorVariavelEstado_valor) == 0) && (a_menorIdCenario != IdCenario_Nenhum))
			vetorVariavelEstado.at(a_idVariavelEstado).setVetor_forced(AttVetorVariavelEstado_valor, SmartEnupla<IdCenario, double>(a_menorIdCenario, std::vector<double>(int(a_maiorIdCenario - a_menorIdCenario) + 1, 0.0)));

		vetorVariavelEstado.at(a_idVariavelEstado).setElemento(AttVetorVariavelEstado_valor, a_idCenario, a_valor);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::addValorVariavelEstado(" + getFullString(a_idVariavelEstado) + "," + getFullString(a_idCenario) + "," + getFullString(a_menorIdCenario) + "," + getFullString(a_maiorIdCenario) + "," + getString(a_valor) + "): \n" + std::string(erro.what())); }

} // void Estagio::adicionarValorVariavelEstado(const IdVariavelEstado a_idVariavelEstado, const IdIteracao a_idIteracao, const IdCenario a_idCenario, const double a_valor){

void Estagio::alocarVariaveisEstado(const IdCenario a_menorIdCenario, const IdCenario a_maiorIdCenario){

	try {

		for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= getMaiorId(IdVariavelEstado()); idVariavelEstado++) {

			if (vetorVariavelEstado.isInstanciado(idVariavelEstado)) {
				vetorVariavelEstado.at(idVariavelEstado).setVetor_forced(AttVetorVariavelEstado_valor, SmartEnupla<IdCenario, double>(a_menorIdCenario, std::vector<double>(int(a_maiorIdCenario - a_menorIdCenario) + 1, 0.0)));
			}
		}

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::alocarVariaveisEstado(" + getFullString(a_menorIdCenario) + "," + getFullString(a_maiorIdCenario) + "): \n" + std::string(erro.what())); }

}


std::vector<std::string> Estagio::getNomeVariavelEstado(const IdVariavelEstado a_idVariavelEstado){

	try{

		std::vector<std::string> vetor_retorno;

		std::string nome = getAtributo(a_idVariavelEstado, AttComumVariavelEstado_nome, std::string());
		
		const size_t id_ = nome.find("Id");

		std::string valor;

		size_t pos = 0;

		bool nome_var = false;
		while (pos != std::string::npos) {

			pos = nome.find(",");

			valor = nome.substr(0, pos).c_str();

			if (nome_var) {
				const size_t pos_ = valor.find("_");
				if ((pos_ != std::string::npos) && (id_ != std::string::npos))
					vetor_retorno.push_back(valor.substr(pos_ + 1, valor.length()));
				else
					vetor_retorno.push_back(valor);
			}
			else {
				vetor_retorno.push_back(valor);
				nome_var = true;
			}

			nome = nome.substr(pos + 1, nome.length());

			if (nome == "")
				break;

		} // while (pos != string::npos) {

		return vetor_retorno;

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("Estagio::getNomeVariavelEstado(" + getFullString(a_idVariavelEstado) + "): \n" + std::string(erro.what())); }

} // std::vector<std::string> Estagio::getNomeVariavelEstado(const IdVariavelEstado a_idVariavelEstado){


void Estagio::setVariavelDecisaoEmVariavelEstado(const IdVariavelEstado a_idVariavelEstado, const TipoSubproblemaSolver a_TSS, const int a_idVariavelDecisao) {

	try { 
		
		if (getSizeVetor(a_idVariavelEstado, AttVetorVariavelEstado_idVariavelDecisao) == 0)
			vetorVariavelEstado.at(a_idVariavelEstado).setVetor_forced(AttVetorVariavelEstado_idVariavelDecisao, SmartEnupla<TipoSubproblemaSolver, int>(TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1), std::vector<int>(TipoSubproblemaSolver_Excedente, -1)));

		vetorVariavelEstado.at(a_idVariavelEstado).setElemento(AttVetorVariavelEstado_idVariavelDecisao, a_TSS, a_idVariavelDecisao);
	
	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("Estagio::setVariavelDecisaoEmVariavelEstado(" + getFullString(a_idVariavelEstado) + "," + getFullString(a_TSS) + "," + getFullString(a_idVariavelDecisao) + "): \n" + std::string(erro.what())); }

} // void Estagio::setVariavelDecisaoAnteriorEmVariavelEstado(const IdVariavelEstado a_idVariavelEstado){


void Estagio::setVariavelDecisaoAnteriorEmVariavelEstado(const IdVariavelEstado a_idVariavelEstado, const TipoSubproblemaSolver a_TSS, const int a_idVariavelDecisao_anterior){

	try { 
		vetorVariavelEstado.at(a_idVariavelEstado).setAtributo(AttComumVariavelEstado_idVariavelDecisaoEstagioAnterior, a_idVariavelDecisao_anterior);
		vetorVariavelEstado.at(a_idVariavelEstado).setAtributo(AttComumVariavelEstado_tipoSubproblemaSolverEstagioAnterior, a_TSS);
	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("Estagio::setVariavelDecisaoAnteriorEmVariavelEstado(" + getFullString(a_idVariavelEstado) + "," + getFullString(a_TSS) + "," + getFullString(a_idVariavelDecisao_anterior) + "): \n" + std::string(erro.what())); }

} // void Estagio::setVariavelDecisaoAnteriorEmVariavelEstado(const IdVariavelEstado a_idVariavelEstado){


double Estagio::calcularValorVariavelRealizacaoInterna(const IdVariavelRealizacaoInterna idVariavelRealizacaoInterna, const double a_valor){

	try{

		const TipoValor tipo_valor = getAtributo(idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_tipo_valor, TipoValor());

		const double percentual_a_utilizar = getAtributo(idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_percentual_a_utilizar, double());

		vetorVariavelRealizacaoInterna.at(idVariavelRealizacaoInterna).setAtributo(AttComumVariavelRealizacaoInterna_percentual_a_utilizar, percentual_a_utilizar + getAtributo(idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_percentual_passo, double()));

		if (tipo_valor == TipoValor_positivo_e_negativo)
			return 0.0;
		else if ((tipo_valor == TipoValor_negativo) && (a_valor <= 0.0))
			return 0.0;
		else if ((tipo_valor == TipoValor_negativo) && (a_valor > 0.0) && (percentual_a_utilizar > 1.0))
			return -a_valor;
		else if ((tipo_valor == TipoValor_positivo) && (a_valor >= 0.0))
			return 0.0;
		else if ((tipo_valor == TipoValor_positivo) && (a_valor < 0.0) && (percentual_a_utilizar > 1.0))
			return -a_valor;

		if (doubleCompara(1e-3, percentual_a_utilizar, 1.0))
			return -a_valor;

		const double valor_utilizar = -a_valor * percentual_a_utilizar;

		return valor_utilizar;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::calcularValorVariavelRealizacaoInterna(" + getFullString(idVariavelRealizacaoInterna) + "," + getFullString(a_valor) + "): \n" + std::string(erro.what())); }

} // void Estagio::calcularValorVariavelRealizacaoInterna(const IdVariavelRealizacaoInterna idVariavelRealizacaoInterna, const double a_valor){


void Estagio::resetarValorVariavelRealizacaoInterna(const TipoSubproblemaSolver a_TSS, const IdVariavelRealizacaoInterna idVariavelRealizacaoInterna){

	try {

		const int idVariavelDecisao = getElementoVetor(idVariavelRealizacaoInterna, AttVetorVariavelRealizacaoInterna_idVariavelDecisao, a_TSS, int());

		getSolver(a_TSS)->setLimInferior(idVariavelDecisao, 0.0);
		getSolver(a_TSS)->setLimSuperior(idVariavelDecisao, 0.0);

		vetorVariavelRealizacaoInterna.at(idVariavelRealizacaoInterna).setAtributo(AttComumVariavelRealizacaoInterna_percentual_a_utilizar, getAtributo(idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_percentual_inicial, double()));
	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::calcularValorVariavelRealizacaoInterna(" + getFullString(a_TSS) + "," + getFullString(idVariavelRealizacaoInterna) + "): \n" + std::string(erro.what())); }

} // void Estagio::resetarValorVariavelRealizacaoInterna(const IdVariavelRealizacaoInterna idVariavelRealizacaoInterna){

void Estagio::selecaoSolucaoProxy(const int a_numero_aberturas_solucao_proxy){

	try {

		const int numero_aberturas = int(getAtributo(AttComumEstagio_maiorIdRealizacao, IdRealizacao()));

		if (a_numero_aberturas_solucao_proxy <= 0)
			throw std::invalid_argument("O numero de aberturas para solucoes proxy deve ser maior que 0.");
		else if (a_numero_aberturas_solucao_proxy > numero_aberturas)
			throw std::invalid_argument("O numero de aberturas para solucoes proxy deve ser menor ou igual ao o numero de realizacoes.");

		int limite_inferior_aberturas = numero_aberturas % a_numero_aberturas_solucao_proxy;

		int espacamento_aberturas = numero_aberturas / a_numero_aberturas_solucao_proxy;

		SmartEnupla<int, int> selecao_solucao_proxy(1, std::vector<int>(numero_aberturas, 0));

		int cont = 0;
		for (int i = numero_aberturas; i >= 1; i--) {

			if (i == limite_inferior_aberturas)
				break;

			else if ((i == numero_aberturas) || (cont == espacamento_aberturas)){
				selecao_solucao_proxy.setElemento(i , 1);
				cont = 0;
			}

			cont++;

		} // for (int i = numero_aberturas; i >= 1; i--) {

		setVetor(AttVetorEstagio_selecao_solucao_proxy, selecao_solucao_proxy);

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("Estagio::selecaoSolucaoProxy(" + getFullString(a_numero_aberturas_solucao_proxy) + "): \n" + std::string(erro.what())); }

} // void Estagio::selecaoSolucaoProxy(const int a_numero_aberturas_solucao_proxy){


void Estagio::instanciarCorteBenders(const SmartEnupla<IdRealizacao, double> &a_rhs, const SmartEnupla<IdRealizacao, double> &a_rhs_primal, const SmartEnupla<IdRealizacao, double> &a_rhs_estado, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> &a_coeficiente, const SmartEnupla<IdVariavelEstado, double> &a_estado){

	try {

		const IdCorteBenders idCorteBenders = IdCorteBenders(vetorCorteBenders.getMaiorId() + 1);

		CorteBenders corteBenders;
		corteBenders.setAtributo(AttComumCorteBenders_idCorteBenders, idCorteBenders);

		vetorCorteBenders.add(corteBenders);

		vetorCorteBenders.at(idCorteBenders).setVetor_forced(AttVetorCorteBenders_rhs, a_rhs);

		vetorCorteBenders.at(idCorteBenders).setVetor_forced(AttVetorCorteBenders_rhs_primal, a_rhs_primal);
		vetorCorteBenders.at(idCorteBenders).setVetor_forced(AttVetorCorteBenders_rhs_estado, a_rhs_estado);

		vetorCorteBenders.at(idCorteBenders).setVetor_forced(AttVetorCorteBenders_estado, a_estado);

		vetorCorteBenders.at(idCorteBenders).setMatriz_forced(AttMatrizCorteBenders_coeficiente, a_coeficiente);

		avaliarDominanciaCorteBenders(idCorteBenders);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::instanciarCorteBenders(a_rhs,a_rhs_primal,a_rhs_estado,a_coeficiente,a_estado): \n" + std::string(erro.what())); }

} // void Estagio::instanciaCorteBenders(const IdRealizacao a_idRealizacao, double & a_rhs, double * a_array_coeficiente, double * a_array_estado){

SmartEnupla<IdCorteBenders, IdCorteBenders> Estagio::sequenciarCorteBenders(){
	try{

		const IdCorteBenders maiorIdCorteBenders = vetorCorteBenders.getMaiorId();

		if (maiorIdCorteBenders < IdCorteBenders(IdCorteBenders_Excedente - 1))
			return SmartEnupla<IdCorteBenders, IdCorteBenders>();

		VetorCorteBendersEmEstagio vetorCorteBendersEmEstagio(vetorCorteBenders);

		vetorCorteBenders.esvaziar();

		SmartEnupla<IdCorteBenders, IdCorteBenders> mapeamento_cortes;

		for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {
			if (vetorCorteBendersEmEstagio.isInstanciado(idCorteBenders)) {

				const IdCorteBenders idCorteBenders_sequencial = IdCorteBenders(vetorCorteBenders.getMaiorId() + 1);

				mapeamento_cortes.addElemento(idCorteBenders_sequencial, idCorteBenders);

				vetorCorteBendersEmEstagio.at(idCorteBenders).setAtributo(AttComumCorteBenders_idCorteBenders, idCorteBenders_sequencial);
				vetorCorteBenders.add(vetorCorteBendersEmEstagio.at(idCorteBenders));

				//
				// Atualiza Ids de matrizes de seleção e deseleção de cortes
				//

				if (getSizeMatriz(AttMatrizEstagio_cortes_selecionados) > 0) {
					for (IdRealizacao idReal = IdRealizacao_1; idReal <= getIterador1Final(AttMatrizEstagio_cortes_selecionados, IdRealizacao()); idReal++) {
						if (getSizeMatriz(AttMatrizEstagio_cortes_selecionados, idReal) > 0) {
							for (int i = getIterador2Inicial(AttMatrizEstagio_cortes_selecionados, idReal, int()); i <= getIterador2Final(AttMatrizEstagio_cortes_selecionados, idReal, int()); i++) {
								const IdCorteBenders idCorteBenders_old = getElementoMatriz(AttMatrizEstagio_cortes_selecionados, idReal, i, IdCorteBenders());
								if (idCorteBenders_old == idCorteBenders)
									setElemento(AttMatrizEstagio_cortes_selecionados, idReal, i, idCorteBenders_sequencial);
							}
						} // if (getSizeMatriz(AttMatrizEstagio_cortes_selecionados, idReal) > 0) {
					} // for (IdRealizacao idReal = IdRealizacao_1; idReal <= getIterador1Final(AttMatrizEstagio_cortes_selecionados, IdRealizacao()); idReal++) {
				} // if (getSizeMatriz(AttMatrizEstagio_cortes_selecionados) > 0) {

				if (getSizeMatriz(AttMatrizEstagio_cortes_deselecionados) > 0) {
					for (IdRealizacao idReal = IdRealizacao_1; idReal <= getIterador1Final(AttMatrizEstagio_cortes_deselecionados, IdRealizacao()); idReal++) {
						if (getSizeMatriz(AttMatrizEstagio_cortes_deselecionados, idReal) > 0) {
							for (int i = getIterador2Inicial(AttMatrizEstagio_cortes_deselecionados, idReal, int()); i <= getIterador2Final(AttMatrizEstagio_cortes_deselecionados, idReal, int()); i++) {
								const IdCorteBenders idCorteBenders_old = getElementoMatriz(AttMatrizEstagio_cortes_deselecionados, idReal, i, IdCorteBenders());
								if (idCorteBenders_old == idCorteBenders)
									setElemento(AttMatrizEstagio_cortes_deselecionados, idReal, i, idCorteBenders_sequencial);
							}
						} // if (getSizeMatriz(AttMatrizEstagio_cortes_deselecionados, idReal) > 0) {
					} // for (IdRealizacao idReal = IdRealizacao_1; idReal <= getIterador1Final(AttMatrizEstagio_cortes_deselecionados, IdRealizacao()); idReal++) {
				} // if (getSizeMatriz(AttMatrizEstagio_cortes_deselecionados) > 0) {

			} // if (vetorCorteBendersEmEstagio.isInstanciado(idCorteBenders)) {
		} // for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {

		for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= vetorCorteBenders.getMaiorId(); idCorteBenders++)
			vetorCorteBenders.at(idCorteBenders).sequencializaOrdenacaoCorteDominante(mapeamento_cortes);

		return mapeamento_cortes;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::sequenciarCorteBenders(): \n" + std::string(erro.what())); }
} // bool Estagio::renumerarCorteBenders(){

void Estagio::instanciarCorteBenders(const SmartEnupla<IdRealizacao, double>& a_rhs, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>>& a_coeficiente, const SmartEnupla<IdVariavelEstado, double>& a_estado) {

	try {

		const IdCorteBenders idCorteBenders = IdCorteBenders(vetorCorteBenders.getMaiorId() + 1);

		CorteBenders corteBenders;
		corteBenders.setAtributo(AttComumCorteBenders_idCorteBenders, idCorteBenders);

		vetorCorteBenders.add(corteBenders);

		vetorCorteBenders.at(idCorteBenders).setVetor_forced(AttVetorCorteBenders_rhs, a_rhs);

		if (a_estado.size() > 0)
			vetorCorteBenders.at(idCorteBenders).setVetor_forced(AttVetorCorteBenders_estado, a_estado);

		vetorCorteBenders.at(idCorteBenders).setMatriz_forced(AttMatrizCorteBenders_coeficiente, a_coeficiente);

		avaliarDominanciaCorteBenders(idCorteBenders);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::instanciarCorteBenders(a_rhs,a_rhs_primal,a_rhs_estado,a_coeficiente,a_estado): \n" + std::string(erro.what())); }

} // void Estagio::instanciaCorteBenders(const IdRealizacao a_idRealizacao, double & a_rhs, double * a_array_coeficiente, double * a_array_estado){


void Estagio::requestCorteBenders(const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders, double & a_rhs, SmartEnupla<IdVariavelEstado, double>& a_coeficiente) {

	try {

		a_rhs         = getElementoVetor  (a_idCorteBenders, AttVetorCorteBenders_rhs,          a_idRealizacao, double());
		a_coeficiente = getElementosMatriz(a_idCorteBenders, AttMatrizCorteBenders_coeficiente, a_idRealizacao, IdVariavelEstado(), double());

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::requestCorteBenders(" + getFullString(a_idRealizacao) + "," + getFullString(a_idCorteBenders) + "a_rhs,a_coeficiente): \n" + std::string(erro.what())); }

} // void Estagio::requestCorteBenders(const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders, double & a_rhs, SmartEnupla<IdVariavelEstado, double>& a_coeficiente){


void Estagio::requestCorteBendersFinalizado(const IdRealizacao a_idRealizacao){

	try {

		setElemento(AttVetorEstagio_request_cortes_selecionados, a_idRealizacao, 0);
		
		const int numero_cortes_selecionados = getIterador2Final(AttMatrizEstagio_cortes_selecionados, a_idRealizacao, int());

		for (int i = 1; i <= numero_cortes_selecionados; i++)
			setElemento(AttMatrizEstagio_acao_cortes_selecionados, a_idRealizacao, i, -1);

		setElementos(AttMatrizEstagio_cortes_deselecionados, a_idRealizacao, SmartEnupla<int, IdCorteBenders>());

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::requestCorteBendersFinalizado(" + getFullString(a_idRealizacao) + "): \n" + std::string(erro.what())); }

} // void Estagio::requestCorteBendersRealizado(const IdRealizacao a_idRealizacao){


void Estagio::anularVariavelEstadoCorteBenders(const IdVariavelEstado a_idVariavelEstado){

	try {

		for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= getMaiorId(IdCorteBenders()); idCorteBenders++) {
			if (vetorCorteBenders.isInstanciado(idCorteBenders))
				vetorCorteBenders.at(idCorteBenders).anularVariavelEstado(a_idVariavelEstado);
		}

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("Estagio::anularVariavelEstadoCorteBenders(" + getFullString(a_idVariavelEstado) + "): \n" + std::string(erro.what())); }

} // void Estagio::anularVariavelEstadoCorteBenders(const IdVariavelEstado a_idVariavelEstado){

void Estagio::anularVariavelEstadoCorteBenders(const IdVariavelEstado a_idVariavelEstado, const IdCorteBenders a_idCorteBenders) {

	try {

			if (vetorCorteBenders.isInstanciado(a_idCorteBenders))
				vetorCorteBenders.at(a_idCorteBenders).anularVariavelEstado(a_idVariavelEstado);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::anularVariavelEstadoCorteBenders(" + getFullString(a_idVariavelEstado) +  "," + getFullString(a_idCorteBenders) + "): \n" + std::string(erro.what())); }

} // void Estagio::anularVariavelEstadoCorteBenders(const IdVariavelEstado a_idVariavelEstado){


void Estagio::alocarCorteBenders(const int a_numero_objetos){
	try{
		vetorCorteBenders.alocar(a_numero_objetos);
	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::anularVariavelEstadoCorteBenders(" + getFullString(a_numero_objetos) + "): \n" + std::string(erro.what())); }
}


void Estagio::selecionarCorteBenders(){

	try{

		const IdCorteBenders maiorIdCorteBenders = getMaiorId(IdCorteBenders());
		if (maiorIdCorteBenders == IdCorteBenders_Nenhum)
			return;

		IdRealizacao maiorIdRealizacao_corte = getAtributo(AttComumEstagio_maiorIdRealizacao, IdRealizacao());

		const int cortes_multiplos = getAtributo(AttComumEstagio_cortes_multiplos, int());

		if (cortes_multiplos == 0)
			maiorIdRealizacao_corte = IdRealizacao_1;

		const int nivel_dominancia = getAtributo(AttComumEstagio_selecao_cortes_nivel_dominancia, int());

		if (getSizeMatriz(AttMatrizEstagio_cortes_selecionados) == 0) {
			setVetor_forced (AttVetorEstagio_request_cortes_selecionados, SmartEnupla<IdRealizacao, int>(IdRealizacao_1, std::vector<int>(maiorIdRealizacao_corte, 0)));
			setMatriz_forced(AttMatrizEstagio_cortes_selecionados,        SmartEnupla<IdRealizacao, SmartEnupla<int, IdCorteBenders>>(IdRealizacao_1, std::vector<SmartEnupla<int, IdCorteBenders>>(maiorIdRealizacao_corte, SmartEnupla<int, IdCorteBenders>(1000))));
			setMatriz_forced(AttMatrizEstagio_acao_cortes_selecionados, SmartEnupla<IdRealizacao, SmartEnupla<int, int>>(IdRealizacao_1, std::vector<SmartEnupla<int, int>>(maiorIdRealizacao_corte, SmartEnupla<int, int>(1000))));

			if (nivel_dominancia > 0) {

				for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {
					if (vetorCorteBenders.isInstanciado(idCorteBenders)) {
						if (getSize1Matriz(idCorteBenders, AttMatrizCorteBenders_ordenacao_cortes_dominantes) == 0)
							avaliarDominanciaCorteBendersParaEstado(idCorteBenders, idCorteBenders);
					}
				}

			} // if (nivel_dominancia > 0) {

		} // if (getSizeMatriz(AttMatrizEstagio_cortes_selecionados) == 0) {

		if (getSizeMatriz(AttMatrizEstagio_cortes_deselecionados) == 0)
			setMatriz_forced(AttMatrizEstagio_cortes_deselecionados, SmartEnupla<IdRealizacao, SmartEnupla<int, IdCorteBenders>>(IdRealizacao_1, std::vector<SmartEnupla<int, IdCorteBenders>>(maiorIdRealizacao_corte, SmartEnupla<int, IdCorteBenders>(100))));

		for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao_corte; idRealizacao++) {

			//
			// Seleção de todos os cortes
			//

			if (nivel_dominancia == 0) {

				try {

					int numero_cortes_selecionados = 0;

					IdCorteBenders menorIdCorteBenders = IdCorteBenders_1;

					if (getSizeMatriz(AttMatrizEstagio_cortes_selecionados, idRealizacao) > 0) {
						numero_cortes_selecionados = getIterador2Final(AttMatrizEstagio_cortes_selecionados, idRealizacao, int());
						menorIdCorteBenders = IdCorteBenders(getElementoMatriz(AttMatrizEstagio_cortes_selecionados, idRealizacao, numero_cortes_selecionados, IdCorteBenders()) + 1);
					}
					
					for (IdCorteBenders idCorteBenders = menorIdCorteBenders; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {
						if (vetorCorteBenders.isInstanciado(idCorteBenders)) {
							numero_cortes_selecionados++;
							setElemento(AttVetorEstagio_request_cortes_selecionados, idRealizacao, 1);
							addElemento(AttMatrizEstagio_cortes_selecionados, idRealizacao, numero_cortes_selecionados, idCorteBenders);
							addElemento(AttMatrizEstagio_acao_cortes_selecionados, idRealizacao, numero_cortes_selecionados, 0);
						}
					}

				} // try
				catch (const std::exception& erro) { throw std::invalid_argument("Erro na selecao de todos os cortes. \n" + std::string(erro.what())); }

			} // if (nivel_dominancia == 0) {

			//
			// Seleção dos cortes dominantes
			//
			else if (nivel_dominancia > 0) {

				//
				// Seleciona os cortes ordenados como dominantes
				//
				SmartEnupla<int, IdCorteBenders> cortes_selecionados_novos(100);
				
				for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {
					if (vetorCorteBenders.isInstanciado(idCorteBenders)) {
						if ((getIterador1Inicial(idCorteBenders, AttMatrizCorteBenders_coeficiente, IdRealizacao()) <= idRealizacao) && (idRealizacao <= getIterador1Final(idCorteBenders, AttMatrizCorteBenders_coeficiente, IdRealizacao()))) {
							if (getSize2Matriz(idCorteBenders, AttMatrizCorteBenders_ordenacao_cortes_dominantes, idRealizacao) > 0) {
								for (int i = 1; i <= getIterador2Final(idCorteBenders, AttMatrizCorteBenders_ordenacao_cortes_dominantes, idRealizacao, int()); i++)
									selecionarCorteBenders(getElementoMatriz(idCorteBenders, AttMatrizCorteBenders_ordenacao_cortes_dominantes, idRealizacao, i, IdCorteBenders()), cortes_selecionados_novos);
							}
						}
					}
				} // for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {

				if (cortes_selecionados_novos.size() > 0) {

					SmartEnupla<int, int> acao_cortes_selecionados_novos(1, std::vector<int>(cortes_selecionados_novos.size(), 0));

					const SmartEnupla<int, IdCorteBenders> cortes_selecionados_anteriormente = getElementosMatriz(AttMatrizEstagio_cortes_selecionados, idRealizacao, int(), IdCorteBenders());
					const SmartEnupla<int, int>            acao_cortes_selecionados_anteriormente = getElementosMatriz(AttMatrizEstagio_acao_cortes_selecionados, idRealizacao, int(), int());

					if (cortes_selecionados_anteriormente.size() == 0) {
						setElemento(AttVetorEstagio_request_cortes_selecionados, idRealizacao, 1);
						setElementos(AttMatrizEstagio_cortes_selecionados, idRealizacao, cortes_selecionados_novos);
						setElementos(AttMatrizEstagio_acao_cortes_selecionados, idRealizacao, acao_cortes_selecionados_novos);
					} // if (cortes_selecionados_anteriormente.size() == 0) {

					else if (cortes_selecionados_anteriormente.size() > 0) {

						SmartEnupla<int, IdCorteBenders> cortes_deselecionados(100);

						int cortes_anteriores_na_nova_selecao = 0;
						int cortes_anteriores_deselecionados = 0;

						for (int i = 1; i <= cortes_selecionados_anteriormente.size(); i++) {

							const int posicao_corte_anterior_na_nova_selecao = isCorteBendersSelecionado(cortes_selecionados_anteriormente.at(i), cortes_selecionados_novos);

							// Corte anterior não foi utilizado na nova seleção
							if (posicao_corte_anterior_na_nova_selecao == 0) {
								cortes_anteriores_deselecionados++;
								cortes_deselecionados.addElemento(cortes_anteriores_deselecionados, cortes_selecionados_anteriormente.at(i));
							}

							// Corte anterior foi utilizado na nova seleção
							else if (posicao_corte_anterior_na_nova_selecao > 0) {
								cortes_anteriores_na_nova_selecao++;
								acao_cortes_selecionados_novos.at(posicao_corte_anterior_na_nova_selecao) = acao_cortes_selecionados_anteriormente.at(i);
							}

						} // for (int i = 1; i <= cortes_selecionados_anteriormente.size(); i++) {

						if (cortes_anteriores_na_nova_selecao != cortes_selecionados_novos.size()) {

							setElemento(AttVetorEstagio_request_cortes_selecionados, idRealizacao, 1);
							setElementos(AttMatrizEstagio_cortes_selecionados, idRealizacao, cortes_selecionados_novos);
							setElementos(AttMatrizEstagio_cortes_deselecionados, idRealizacao, cortes_deselecionados);
							setElementos(AttMatrizEstagio_acao_cortes_selecionados, idRealizacao, acao_cortes_selecionados_novos);

						} // if (cortes_anteriores_na_nova_selecao != cortes_selecionados_novos.size()) {

					} // else if (cortes_selecionados_anteriormente.size() > 0) {

				} // if (cortes_selecionados_novos.size() > 0) {

			} // else if (nivel_dominancia > 0) {

		} // for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao; idRealizacao++) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::selecionarCorteBenders(): \n" + std::string(erro.what())); }

} // void Estagio::selecionarCorteBenders(){


void Estagio::selecionarCorteBenders(const IdCorteBenders a_idCorteBenders, SmartEnupla<int, IdCorteBenders>& a_cortes_selecionados){

	try {

		if (a_cortes_selecionados.size() == 0)
			a_cortes_selecionados.addElemento(1, a_idCorteBenders);

		else {

			for (int i = 1; i <= a_cortes_selecionados.size(); i++)
				if (a_cortes_selecionados.at(i) == a_idCorteBenders)
					return;

			a_cortes_selecionados.addElemento(a_idCorteBenders);

		} // else {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::selecionarCorteBenders(" + getFullString(a_idCorteBenders)  + ",a_cortes_selecionados): \n" + std::string(erro.what())); }

} // void Estagio::selecionarCorteBenders(const IdCorteBenders a_idCorteBenders, SmartEnupla<int, IdCorteBenders>& a_cortes_selecionados){


int Estagio::isCorteBendersSelecionado(const IdCorteBenders a_idCorteBenders, const SmartEnupla<int, IdCorteBenders>& a_cortes_selecionados) {

	try {

		for (int i = 1; i <= a_cortes_selecionados.size(); i++)
			if (a_idCorteBenders == a_cortes_selecionados.at(i))
				return i;

		return 0;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::isCorteBendersSelecionado(" + getFullString(a_idCorteBenders) + ",a_cortes_selecionados): \n" + std::string(erro.what())); }

} // bool Estagio::isCorteBendersSelecionado(const IdCorteBenders a_idCorteBenders, const SmartEnupla<int, IdCorteBenders>& a_cortes_selecionados){


void Estagio::avaliarDominanciaCorteBenders(){

	try {

		for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= getMaiorId(IdCorteBenders()); idCorteBenders++) {
			if (vetorCorteBenders.isInstanciado(idCorteBenders))
				avaliarDominanciaCorteBenders(idCorteBenders);
		}

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("Estagio::avaliarDominanciaCorteBenders(): \n" + std::string(erro.what())); }

} // void Estagio::avaliarDominanciaCorteBenders(){


void Estagio::avaliarDominanciaCorteBenders(const IdCorteBenders a_idCorteBenders) {

	try {

		const int nivel_dominancia = getAtributo(AttComumEstagio_selecao_cortes_nivel_dominancia, int());
		if (nivel_dominancia == 0)
			return;

		avaliarDominanciaCorteBendersParaEstado(a_idCorteBenders, a_idCorteBenders);

		for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= getMaiorId(IdCorteBenders()); idCorteBenders++) {

			if ((vetorCorteBenders.isInstanciado(idCorteBenders)) && (idCorteBenders != a_idCorteBenders)) {


				//
				// Identifica os cortes dominantes para o estado visitado no corte a_idCorteBenders.
				//

				try {
					avaliarDominanciaCorteBendersParaEstado(idCorteBenders, a_idCorteBenders);
				} // try
				catch (const std::exception & erro) { throw std::invalid_argument("Erro na identificacao dos cortes dominantes para o estado calculado. \n" + std::string(erro.what())); }


				//
				// Identifica se o corte a_idCorteBenders é dominante em algum estado já visitado.
				//

				try {
					avaliarDominanciaCorteBendersParaEstado(a_idCorteBenders, idCorteBenders);
				} // try
				catch (const std::exception & erro) { throw std::invalid_argument("Erro na avaliacao da dominancia do corte calculado " + getFullString(a_idCorteBenders) + " em " + getFullString(idCorteBenders) + ". \n" + std::string(erro.what())); }

			} // if ((vetorCorteBenders.isInstanciado(idCorteBenders)) && (idCorteBenders != a_idCorteBenders)) {

		} // for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("Estagio::avaliarDominanciaCorteBenders(" + getFullString(a_idCorteBenders) + "): \n" + std::string(erro.what())); }

} // void Estagio::avaliarDominanciaCorteBenders(const int a_indice_corte){


void Estagio::setElementosCorteBenders(const IdCorteBenders a_idCorteBenders, const SmartEnupla<IdVariavelEstado, double>& a_estado, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>>& a_coeficiente){

	try {

		vetorCorteBenders.at(a_idCorteBenders).setVetor_forced(AttVetorCorteBenders_estado, a_estado);

		vetorCorteBenders.at(a_idCorteBenders).setMatriz_forced(AttMatrizCorteBenders_coeficiente, a_coeficiente);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::setElementosCorteBendersEmFuncaoVariavelEstado(a_estado, a_coeficiente): \n" + std::string(erro.what())); }

} // void Estagio::setElementosCorteBenders(const SmartEnupla<IdVariavelEstado, double>& a_estado, const SmartEnupla<IdVariavelEstado, SmartEnupla<IdRealizacao, double>>& a_coeficiente){

void Estagio::avaliarDominanciaCorteBendersParaEstado(const IdCorteBenders a_idCorteBenders_a_ser_avaliado, const IdCorteBenders a_idCorteBenders_com_estado) {

	try {

		IdRealizacao maiorIdRealizacao_corte = getAtributo(AttComumEstagio_maiorIdRealizacao, IdRealizacao());

		const int cortes_multiplos = getAtributo(AttComumEstagio_cortes_multiplos, int());

		if (cortes_multiplos == 0)
			maiorIdRealizacao_corte = IdRealizacao_1;

		const IdRealizacao menorIdRealizacao_corte_a_ser_avaliado = getIterador1Inicial(a_idCorteBenders_a_ser_avaliado, AttMatrizCorteBenders_coeficiente, IdRealizacao());
		const IdRealizacao maiorIdRealizacao_corte_a_ser_avaliado = getIterador1Final(a_idCorteBenders_a_ser_avaliado, AttMatrizCorteBenders_coeficiente, IdRealizacao());

		const int nivel_dominancia = getAtributo(AttComumEstagio_selecao_cortes_nivel_dominancia, int());

		if (nivel_dominancia == 0)
			return;

		SmartEnupla<IdRealizacao, SmartEnupla<int, double>>         custos_ordenados = vetorCorteBenders.at(a_idCorteBenders_com_estado).getMatriz(AttMatrizCorteBenders_custo_cortes_dominantes, IdRealizacao(), int(), double());
		SmartEnupla<IdRealizacao, SmartEnupla<int, IdCorteBenders>> cortes_ordenados = vetorCorteBenders.at(a_idCorteBenders_com_estado).getMatriz(AttMatrizCorteBenders_ordenacao_cortes_dominantes, IdRealizacao(), int(), IdCorteBenders());

		SmartEnupla<IdRealizacao, int> lista_is_corte_dominado_realizacao = vetorCorteBenders.at(a_idCorteBenders_com_estado).getVetor(AttVetorCorteBenders_is_corte_dominado, IdRealizacao(), int());

		if (custos_ordenados.size() == 0)
			custos_ordenados = SmartEnupla<IdRealizacao, SmartEnupla<int, double>>(IdRealizacao_1, std::vector<SmartEnupla<int, double>>(maiorIdRealizacao_corte, SmartEnupla<int, double>()));

		if (cortes_ordenados.size() == 0)
			cortes_ordenados = SmartEnupla<IdRealizacao, SmartEnupla<int, IdCorteBenders>>(IdRealizacao_1, std::vector<SmartEnupla<int, IdCorteBenders>>(maiorIdRealizacao_corte, SmartEnupla<int, IdCorteBenders>()));

		if (lista_is_corte_dominado_realizacao.size() == 0)
			lista_is_corte_dominado_realizacao = SmartEnupla<IdRealizacao, int>(IdRealizacao_1, std::vector<int>(maiorIdRealizacao_corte, 0));

		int is_corte_dominado = 1;

		for (IdRealizacao idRealizacao = menorIdRealizacao_corte_a_ser_avaliado; idRealizacao <= maiorIdRealizacao_corte_a_ser_avaliado; idRealizacao++) {

			if (getSize2Matriz(a_idCorteBenders_a_ser_avaliado, AttMatrizCorteBenders_coeficiente, idRealizacao) > 0) {

				const double custo_corte = getCustoCorteBendersFromEstado(a_idCorteBenders_a_ser_avaliado, idRealizacao, a_idCorteBenders_com_estado);

				if (cortes_ordenados.at(idRealizacao).size() < nivel_dominancia) {
					custos_ordenados.at(idRealizacao).addElemento(custos_ordenados.at(idRealizacao).size() + 1, custo_corte);
					cortes_ordenados.at(idRealizacao).addElemento(cortes_ordenados.at(idRealizacao).size() + 1, a_idCorteBenders_a_ser_avaliado);
					is_corte_dominado = 0;
				} // if (cortes_ordenados.at(idRealizacao).size() < nivel_dominancia) {

				else {

					SmartEnupla<int, double>         custos_ordenados_temp(nivel_dominancia);
					SmartEnupla<int, IdCorteBenders> cortes_ordenados_temp(nivel_dominancia);

					for (int i = 1; i <= cortes_ordenados.at(idRealizacao).size(); i++) {

						if (custos_ordenados.at(idRealizacao).at(i) < custo_corte) {

							for (int i_ant = 1; i_ant < i; i_ant++) {
								custos_ordenados_temp.addElemento(i_ant, custos_ordenados.at(idRealizacao).at(i_ant));
								cortes_ordenados_temp.addElemento(i_ant, cortes_ordenados.at(idRealizacao).at(i_ant));
							} // for (int i_ant = 0; i_ant < i; i_ant++) {

							custos_ordenados_temp.addElemento(i, custo_corte);
							cortes_ordenados_temp.addElemento(i, a_idCorteBenders_a_ser_avaliado);

							for (int i_pos = i; i_pos <= cortes_ordenados.at(idRealizacao).size() - 1; i_pos++) {
								custos_ordenados_temp.addElemento(i_pos + 1, custos_ordenados.at(idRealizacao).at(i_pos));
								cortes_ordenados_temp.addElemento(i_pos + 1, cortes_ordenados.at(idRealizacao).at(i_pos));
							} // for (int i_pos = i; i_pos <= cortes_ordenados.at(idRealizacao).size() - 1; i_pos++) {

							// Verifica se o corte com estado iá sair da dominânica
							if (cortes_ordenados.at(idRealizacao).at(cortes_ordenados.at(idRealizacao).size()) == a_idCorteBenders_com_estado)
								lista_is_corte_dominado_realizacao.setElemento(idRealizacao, 1);

							break;

						} // if (custos_ordenados.at(idRealizacao).at(i) <= custo_corte) {

					} // for (int i = 1; i <= cortes_ordenados.at(idRealizacao).size(); i++) {

					if (custos_ordenados_temp.size() > 0) {
						custos_ordenados.at(idRealizacao) = custos_ordenados_temp;
						cortes_ordenados.at(idRealizacao) = cortes_ordenados_temp;
					} // if (custos_ordenados_temp.size() > 0) {

					if (lista_is_corte_dominado_realizacao.getElemento(idRealizacao) != 1)
						is_corte_dominado = 0;

					if (cortes_ordenados.at(idRealizacao).size() < nivel_dominancia)
						throw std::invalid_argument("Nao foram selecionados cortes para o nivel de dominancia especificado: " + getString(nivel_dominancia));

				} // else {

			} // if (getSize2Matriz(a_idCorteBenders_a_ser_avaliado, AttMatrizCorteBenders_coeficiente, idRealizacao) > 0){
		} // for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao; idRealizacao++) {

		vetorCorteBenders.at(a_idCorteBenders_com_estado).setAtributo(AttComumCorteBenders_is_corte_dominado, is_corte_dominado);

		vetorCorteBenders.at(a_idCorteBenders_com_estado).setVetor_forced(AttVetorCorteBenders_is_corte_dominado, lista_is_corte_dominado_realizacao);

		vetorCorteBenders.at(a_idCorteBenders_com_estado).setMatriz_forced(AttMatrizCorteBenders_custo_cortes_dominantes, custos_ordenados);
		vetorCorteBenders.at(a_idCorteBenders_com_estado).setMatriz_forced(AttMatrizCorteBenders_ordenacao_cortes_dominantes, cortes_ordenados);

		return;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::avaliarDominanciaCorteBendersParaEstado(" + getFullString(a_idCorteBenders_a_ser_avaliado) + "," + getFullString(a_idCorteBenders_com_estado) + "): \n" + std::string(erro.what())); }

} // void Estagio::avaliarDominanciaCorteBendersParaEstado(const IdCorteBenders a_idCorteBenders_a_ser_avaliado, const IdCorteBenders a_idCorteBenders_com_estado){


void Estagio::exportarVersaoAlternativaCorteBenders(Estagio& a_estagio, const std::string a_idProcessoEstocasticoNovo, const std::string a_idProcessoEstocasticoNovo_compacto, const SmartEnupla<IdVariavelAleatoria, std::vector<std::string>>& a_lista_nome_idVariavelAleatoriaNova, const SmartEnupla<IdVariavelAleatoria, std::vector<std::string>>& a_lista_nome_compacto_idVariavelAleatoriaNova, const SmartEnupla<IdVariavelAleatoria, std::vector<double>>& a_lista_participacao_idVariavelAleatoriaNova){

	try {

		const IdCorteBenders maiorIdCorteBenders = getMaiorId(IdCorteBenders());

		const std::string idEstagio_str          = getFullString(getAtributo(AttComumEstagio_idEstagio, IdEstagio()));
		const std::string idEstagio_compacto_str = getString(getAtributo(AttComumEstagio_idEstagio, IdEstagio()));

		a_estagio.setAtributo(AttComumEstagio_idEstagio, getAtributo(AttComumEstagio_idEstagio, IdEstagio()));
		a_estagio.setAtributo(AttComumEstagio_selecao_cortes_nivel_dominancia, 0);
		a_estagio.setAtributo(AttComumEstagio_periodo_otimizacao, getAtributo(AttComumEstagio_periodo_otimizacao, Periodo()));

		a_estagio.vetorCorteBenders.alocar(int(maiorIdCorteBenders));
		for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {
			if (vetorCorteBenders.isInstanciado(idCorteBenders)) {
				CorteBenders corte;
				corte.setAtributo(AttComumCorteBenders_idCorteBenders, idCorteBenders);
				a_estagio.vetorCorteBenders.add(corte);
				a_estagio.vetorCorteBenders.at(idCorteBenders).setVetor_forced(AttVetorCorteBenders_rhs, getVetor(idCorteBenders, AttVetorCorteBenders_rhs, IdRealizacao(), double()));
			}
		}

		for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= getMaiorId(IdVariavelEstado()); idVariavelEstado++) {

			const std::vector<std::string> nome = getNomeVariavelEstado(idVariavelEstado);

			std::vector<std::string> nome_novo;

			std::vector<double> participacao;

			const bool nome_compacto = getSolver(getAtributo(idVariavelEstado, AttComumVariavelEstado_tipoSubproblemaSolverEstagioAnterior, TipoSubproblemaSolver()))->isNomeSimplificado();

			//
			// Afluencia Processo Estocastico
			//

			if (nome.at(0) == "varYP") {

				const IdVariavelAleatoria idVariavelAleatoria = getIdVariavelAleatoriaFromChar(nome.at(4).c_str());

				std::string lag_str;
				
				if (nome_compacto)
					lag_str = getString(getintFromChar(nome.at(5).c_str()));
				else
					lag_str = getFullString(getintFromChar(nome.at(5).c_str()));

				for (int i = 0; i < int(a_lista_nome_idVariavelAleatoriaNova.at(idVariavelAleatoria).size()); i++) {
					
					if (nome_compacto)
						nome_novo.push_back("varYP," + idEstagio_compacto_str + "," + nome.at(2) + "," + a_idProcessoEstocasticoNovo_compacto + "," + a_lista_nome_compacto_idVariavelAleatoriaNova.at(idVariavelAleatoria).at(i) + "," + lag_str);
					else
						nome_novo.push_back("varYP," + idEstagio_str + "," + nome.at(2) + "," + a_idProcessoEstocasticoNovo + "," + a_lista_nome_idVariavelAleatoriaNova.at(idVariavelAleatoria).at(i) + "," + lag_str);
					
					
					participacao.push_back(a_lista_participacao_idVariavelAleatoriaNova.at(idVariavelAleatoria).at(i));
				}

			} // if (nome.at(0) == "varYP") {

			else {
				nome_novo.push_back(getAtributo(idVariavelEstado, AttComumVariavelEstado_nome, std::string()));
				participacao.push_back(1.0);
			}

			for (int i = 0; i < int(nome_novo.size()); i++) {

				a_estagio.addVariavelEstado(TipoSubproblemaSolver_geral, nome_novo.at(i), 0, 0);

				const IdVariavelEstado idVariavelEstadoNovo = a_estagio.getMaiorId(IdVariavelEstado());

				for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {

					if (vetorCorteBenders.isInstanciado(idCorteBenders)) {

						a_estagio.vetorCorteBenders.at(idCorteBenders).addElemento(AttVetorCorteBenders_estado, idVariavelEstadoNovo, getElementoVetor(idCorteBenders, AttVetorCorteBenders_estado, idVariavelEstado, double()) * participacao.at(i));

						const IdRealizacao menorIdRealizacao = getIterador1Inicial(idCorteBenders, AttMatrizCorteBenders_coeficiente, IdRealizacao());
						const IdRealizacao maiorIdRealizacao = getIterador1Final(idCorteBenders, AttMatrizCorteBenders_coeficiente, IdRealizacao());

						for (IdRealizacao idRealizacao = menorIdRealizacao; idRealizacao <= maiorIdRealizacao; idRealizacao++)
							a_estagio.vetorCorteBenders.at(idCorteBenders).addElemento(AttMatrizCorteBenders_coeficiente, idRealizacao, idVariavelEstadoNovo, getElementoMatriz(idCorteBenders, AttMatrizCorteBenders_coeficiente, idRealizacao, idVariavelEstado, double()));

					} // if (vetorCorteBenders.isInstanciado(idCorteBenders)) {

				} // for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {

			} // for (int i = 0; i < int(nome_novo.size()); i++) {

		} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= getMaiorId(, IdVariavelEstado()); idVariavelEstado++) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::exportarVersaoAlternativaCorteBenders(, a_lista_var): \n" + std::string(erro.what())); }

} // void Estagio::exportarVersaoAlternativaCorteBenders(Estagio& a_estagio, const SmartEnupla<IdVariavelAleatoria, std::vector<std::string>>& a_lista_var){


void Estagio::removerTodosCorteBenders(){

	try{

		vetorCorteBenders = VetorCorteBendersEmEstagio();

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::removerTodosCorteBenders(): \n" + std::string(erro.what())); }

}



double Estagio::getCustoCorteBendersFromEstado(const IdCorteBenders a_idCorteBenders_a_ser_avaliado, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders_com_estado){

	try{

		if (getSize2Matriz(a_idCorteBenders_a_ser_avaliado, AttMatrizCorteBenders_coeficiente, a_idRealizacao) == 0)
			return 0.0;

		const IdVariavelEstado maiorIdVariavelEstado = vetorCorteBenders.at(a_idCorteBenders_com_estado).getIteradorFinal(AttVetorCorteBenders_estado, IdVariavelEstado());

		double custo = 0.0;
		for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {
			const double estado      =  getElementoVetor(a_idCorteBenders_com_estado,     AttVetorCorteBenders_estado,       idVariavelEstado, double());
			const double coeficiente = getElementoMatriz(a_idCorteBenders_a_ser_avaliado, AttMatrizCorteBenders_coeficiente, a_idRealizacao, idVariavelEstado, double());
			custo -= estado * coeficiente;
		} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

		return getElementoVetor(a_idCorteBenders_a_ser_avaliado, AttVetorCorteBenders_rhs, a_idRealizacao, double()) - custo;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::getCustoCorteBendersFromEstado(" + getFullString(a_idCorteBenders_a_ser_avaliado) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCorteBenders_com_estado) + "): \n" + std::string(erro.what())); }

} // double Estagio::getCustoCorteBendersFromEstado(const IdCorteBenders a_idCorteBenders_a_ser_avaliado, const IdCorteBenders a_idCorteBenders_com_estado){


void Estagio::instanciarSolver(const TipoSubproblemaSolver a_TSS, TipoSolver& a_tipoSolver){

	try {

		if ((a_TSS <= TipoSubproblemaSolver_Nenhum) || (a_TSS >= TipoSubproblemaSolver_Excedente))
			throw std::invalid_argument("Tipo de subproblema invalido");


		excluirInstanciasSolver(a_TSS);

#ifdef GRB

		if (a_tipoSolver == TipoSolver_gurobi) {
			try {
				vetorPtrSolver.at(a_TSS) = new SolverGRB();
				lista_tipoSubproblemaSolver.push_back(a_TSS);
				return;
			}
			catch (...) {
				std::cout << "Nao foi possivel instanciar modelo com " + getString(TipoSolver_gurobi) << ", alterando para " + getString(TipoSolver_clp) << std::endl;
				a_tipoSolver = TipoSolver_clp;
			}
		}

#endif

		if (a_tipoSolver == TipoSolver_clp) {
			vetorPtrSolver.at(a_TSS) = new SolverCLP();
			lista_tipoSubproblemaSolver.push_back(a_TSS);
			return;
		}

		throw std::invalid_argument("Tipo de Solver Invalido");

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::instanciarSolver(" + getFullString(a_TSS) + "," + getFullString(a_tipoSolver) + "): \n" + std::string(erro.what())); }

}

Solver* Estagio::getSolver(const TipoSubproblemaSolver a_TSS){

	try {

		if (vetorPtrSolver.at(a_TSS) == nullptr)
			throw std::invalid_argument("Solver nao instanciado.");

		return vetorPtrSolver.at(a_TSS);

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::getSolver(" + getFullString(a_TSS) + "): \n" + std::string(erro.what())); }

} // Solver* Estagio::getSolver(const TipoSubproblemaSolver a_TSS){

bool Estagio::isSolverInstanciado(const TipoSubproblemaSolver a_TSS){

	try {

		if (vetorPtrSolver.at(a_TSS) != nullptr)
			return true;

		return false;

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::isSolverInstanciado(" + getFullString(a_TSS) + "): \n" + std::string(erro.what())); }

}

void Estagio::excluirInstanciasSolver(){

	try {

		for (TipoSubproblemaSolver tipoSubproblemaSolver = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tipoSubproblemaSolver < TipoSubproblemaSolver_Excedente; tipoSubproblemaSolver++) 
			excluirInstanciasSolver(tipoSubproblemaSolver);

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::excluirInstanciasSolver(): \n" + std::string(erro.what())); }

} // void Estagio::excluirInstanciasSolver(){

void Estagio::excluirInstanciasSolver(const TipoSubproblemaSolver a_TSS){

	try {

		if (vetorPtrSolver.at(a_TSS) != nullptr) {
			delete vetorPtrSolver.at(a_TSS);
			vetorPtrSolver.at(a_TSS) = nullptr;
		}

	} // try {
	catch (const std::exception& erro) { throw std::invalid_argument("Estagio::excluirInstanciasSolver(" + getFullString(a_TSS) + "): \n" + std::string(erro.what())); }

}

