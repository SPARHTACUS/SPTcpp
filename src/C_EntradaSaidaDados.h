#ifndef C_ENTRADA_SAIDA_DADOS_H
#define C_ENTRADA_SAIDA_DADOS_H

#include "C_Auxiliar.h"
#include "C_SmartElemento.h"

#include <string>
#include <vector>

#define TIPO_ACESSO_INSTANCIA(m)\
	m(TipoAcessoInstancia, direto)\
	m(TipoAcessoInstancia, m1)\
	m(TipoAcessoInstancia, m2)\
	m(TipoAcessoInstancia, m3) \
	m(TipoAcessoInstancia, m4) 
DEFINE_SMART_ENUM(TipoAcessoInstancia, TIPO_ACESSO_INSTANCIA)


class EntradaSaidaDados{

private:

	bool imprimirNaTela_original = false;
	bool appendArquivo_original  = false;

	std::string separadorCSV_original     = ";";
	std::string fimDeArquivo_original     = "";

	std::string diretorioSaida_original = "DadosSaida";
	std::string diretorioEntrada_original = "DadosEntrada";

	bool imprimirNaTela;
	bool appendArquivo;

	std::string separadorCSV;
	std::string fimDeArquivo;
	std::string diretorioSaida;
	std::string diretorioEntrada;

	void imprimirArquivoCSV_AttComum_(const std::string a_nomeArquivo, const std::string a_m1, const std::string a_m2, const std::string a_m3, const std::string a_m4, SmartDados &a_objetoDados, const std::vector<std::string> a_vetorAttComum)const;

	void imprimirArquivoCSV_AttVetor_(const std::string a_nomeArquivo, const std::string a_m1, const std::string a_m2, const std::string a_m3, const std::string a_m4, SmartDados &a_objetoDados, const std::string a_iteradorInicial, const std::string a_iteradorFinal, const std::vector<std::string> a_vetorAttVetor)const;
	
	void imprimirArquivoCSV_AttMatriz_(const std::string a_nomeArquivo, const std::string a_m1, const std::string a_m2, const std::string a_m3, const std::string a_m4, SmartDados &a_objetoDados, const std::string a_iterador1Inicial, const std::string a_iterador1Final, const std::string a_iterador2Inicial, const std::string a_iterador2Final, const std::vector<std::string> a_vetorAttVetor)const;

	void getMembroIdMembro(const std::string a_string, std::string &a_m1, std::string &a_idM1)const;

public:

	EntradaSaidaDados();
	EntradaSaidaDados(const bool a_append, const bool a_imprimirNaTela, const std::string a_diretorioEntrada, const std::string a_diretorioSaida, const std::string a_fimDeArquivo, const std::string a_separadorCSV);

	~EntradaSaidaDados();

	void setAppendArquivo(const bool a_append);
	void setImprimirNaTela(const bool a_imprimirNaTela);
	void setDiretorioEntrada(const std::string a_diretorioEntrada);
	void setDiretorioSaida(const std::string a_diretorioSaida);
	void setFimDeArquivo(const std::string a_fimDeArquivo);
	void setSeparadorCSV(const std::string a_separadorCSV);

	bool getAppendArquivo()const;
	bool getImprimirNaTela()const;
	std::string getDiretorioEntrada()const;
	std::string getDiretorioSaida()const;
	std::string getFimDeArquivo()const;
	std::string getSeparadorCSV()const;

	void restauraImprimirNaTela();
	void restauraAppendArquivo();
	void restauraDiretorioEntrada();
	void restauraDiretorioSaida();
	void restauraFimDeArquivo();
	void restauraSeparadorCSV();

	//
	// Leitura de Atributos
	//

	// AttComum
	void carregarArquivoCSV_AttComum(const std::string a_nomeArquivo, SmartDados &a_objetoDados, const TipoAcessoInstancia a_tipoAcesso)const;
	bool carregarArquivoCSV_AttComum_seExistir(const std::string a_nomeArquivo, SmartDados &a_objetoDados, const TipoAcessoInstancia a_tipoAcesso)const;

	// AttVetor
	void carregarArquivoCSV_AttVetor(const std::string a_nomeArquivo, SmartDados &a_objetoDados, const TipoAcessoInstancia a_tipoAcesso)const;
	bool carregarArquivoCSV_AttVetor_seExistir(const std::string a_nomeArquivo, SmartDados &a_objetoDados, const TipoAcessoInstancia a_tipoAcesso)const;

	// AttMatriz
	void carregarArquivoCSV_AttMatriz(const std::string a_nomeArquivo, SmartDados &a_objetoDados, const TipoAcessoInstancia a_tipoAcesso)const;
	bool carregarArquivoCSV_AttMatriz_seExistir(const std::string a_nomeArquivo, SmartDados &a_objetoDados, const TipoAcessoInstancia a_tipoAcesso)const;


	void renomearArquivo(const std::string a_nomeArquivo_antigo, const std::string a_nomeArquivo_novo);

	//
	// Impressao de Atributos
	//

	void imprimirConsolidacaoHorizontalCSV(const std::string a_nome_arquivo_resultante, const std::vector<std::string>& a_lista_arquivos_originais, const int a_numero_colunas_suprimir, const bool a_apagar_arquivos_originais)const;

	void imprimirConsolidacaoVerticalCSV(const std::string a_nome_arquivo_resultante, const std::vector<std::string>& a_lista_arquivos_originais, const bool a_apagar_arquivos_originais, const bool a_remover_cabecalho) const;


	std::string sequenciarNomeArquivoImpressao(const std::string a_nome_arquivo, const std::string a_extensao, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso);

	void imprimirArquivoCSV_SmartEnupla(const std::string a_nomeArquivo, const std::vector<std::vector<std::string>>& a_matriz_string)const;
	void imprimirArquivoCSV_SmartEnupla(const std::string a_nomeArquivo, const std::vector<std::vector<std::string>>& a_matriz_string, const bool a_incluir_cabecalho)const;
	void imprimirArquivoCSV(const std::string a_nomeArquivo, const std::vector<std::string>& a_matriz_string, const bool a_incluir_cabecalho)const;

	// AttComum 

	// AttComum Direto

	void imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, SmartDados &a_objetoDados)const;

	template<typename TipoAtributo>
	void imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, SmartDados &a_objetoDados, const TipoAtributo a_attComum)const;

	template<typename TipoAtributo>
	void imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, SmartDados &a_objetoDados, const std::vector<TipoAtributo> a_attComum)const;

	// AttComum Membro

	template<class Membro>
	void imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, SmartDados &a_objetoDados)const;

	template<class Membro, typename TipoAtributo>
	void imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, SmartDados &a_objetoDados, const TipoAtributo a_attComum)const;

	template<class Membro, typename TipoAtributo>
	void imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, SmartDados &a_objetoDados, const std::vector<TipoAtributo> a_attComum)const;

	// AttComum M2

	template<class Membro, class M2>
	void imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, SmartDados &a_objetoDados)const;

	template<class Membro, class M2, typename TipoAtributo>
	void imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, SmartDados &a_objetoDados, const TipoAtributo a_attComum)const;

	template<class Membro, class M2, typename TipoAtributo>
	void imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, SmartDados &a_objetoDados, const std::vector<TipoAtributo> a_attComum)const;

	// AttComum M3

	template<class Membro, class M2, class M3>
	void imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, SmartDados &a_objetoDados)const;

	template<class Membro, class M2, class M3, typename TipoAtributo>
	void imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, SmartDados &a_objetoDados, TipoAtributo a_attComum)const;

	template<class Membro, class M2, class M3, typename TipoAtributo>
	void imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, SmartDados &a_objetoDados, const std::vector<TipoAtributo> a_attComum)const;

	// AttComum M4

	template<class Membro, class M2, class M3, class M4>
	void imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, const M4 a_m4, SmartDados &a_objetoDados)const;

	template<class Membro, class M2, class M3, class M4, typename TipoAtributo>
	void imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, const M4 a_m4, SmartDados &a_objetoDados, TipoAtributo a_attComum)const;

	template<class Membro, class M2, class M3, class M4, typename TipoAtributo>
	void imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, const M4 a_m4, SmartDados &a_objetoDados, const std::vector<TipoAtributo> a_attComum)const;

	// AttVetor

	// AttVetor Direto

	template<typename TipoAtributo>
	void imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, SmartDados &a_objetoDados, const TipoAtributo a_attVetor)const;
	
	template<typename TipoIterador, typename TipoAtributo>
	void imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, SmartDados &a_objetoDados, const TipoIterador a_iteradorInicial, const TipoIterador a_iteradorFinal, const TipoAtributo a_attVetor)const;

	template<typename TipoAtributo>
	void imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, SmartDados &a_objetoDados, const std::vector<TipoAtributo> a_attVetor)const;
	
	template<typename TipoIterador, typename TipoAtributo>
	void imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, SmartDados &a_objetoDados, const TipoIterador a_iteradorInicial, const TipoIterador a_iteradorFinal, const std::vector<TipoAtributo> a_attVetor)const;

	// AttVetor Membro

	template<class Membro, typename TipoAtributo>
	void imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, SmartDados &a_objetoDados, const TipoAtributo a_attVetor)const;
	
	template<class Membro, typename TipoIterador, typename TipoAtributo>
	void imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, SmartDados &a_objetoDados, const TipoIterador a_iteradorInicial, const TipoIterador a_iteradorFinal, const TipoAtributo a_attVetor)const;

	template<class Membro, typename TipoAtributo>
	void imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, SmartDados &a_objetoDados, const std::vector<TipoAtributo> a_attVetor)const;
	
	template<class Membro, typename TipoIterador, typename TipoAtributo>
	void imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, SmartDados &a_objetoDados, const TipoIterador a_iteradorInicial, const TipoIterador a_iteradorFinal, const std::vector<TipoAtributo> a_attVetor)const;

	// AttVetor M2

	template<class Membro, class M2, typename TipoAtributo>
	void imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, SmartDados &a_objetoDados, const TipoAtributo a_attVetor)const;
	
	template<class Membro, class M2, typename TipoIterador, typename TipoAtributo>
	void imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, SmartDados &a_objetoDados, const TipoIterador a_iteradorInicial, const TipoIterador a_iteradorFinal, const TipoAtributo a_attVetor)const;

	template<class Membro, class M2, typename TipoAtributo>
	void imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, SmartDados &a_objetoDados, const std::vector<TipoAtributo> a_attVetor)const;
	
	template<class Membro, class M2, typename TipoIterador, typename TipoAtributo>
	void imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, SmartDados &a_objetoDados, const TipoIterador a_iteradorInicial, const TipoIterador a_iteradorFinal, const std::vector<TipoAtributo> a_attVetor)const;

	// AttVetor M3

	template<class Membro, class M2, class M3, typename TipoAtributo>
	void imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, SmartDados &a_objetoDados, const TipoAtributo a_attVetor)const;

	template<class Membro, class M2, class M3, typename TipoIterador, typename TipoAtributo>
	void imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, SmartDados &a_objetoDados, const TipoIterador a_iteradorInicial, const TipoIterador a_iteradorFinal, const TipoAtributo a_attVetor)const;
	
	template<class Membro, class M2, class M3, typename TipoAtributo>
	void imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, SmartDados &a_objetoDados, const std::vector<TipoAtributo> a_attVetor)const;
	
	template<class Membro, class M2, class M3, typename TipoIterador, typename TipoAtributo>
	void imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, SmartDados &a_objetoDados, const TipoIterador a_iteradorInicial, const TipoIterador a_iteradorFinal, const std::vector<TipoAtributo> a_attVetor)const;

	// AttVetor M4

	template<class Membro, class M2, class M3, class M4, typename TipoAtributo>
	void imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, const M4 a_m4, SmartDados &a_objetoDados, const TipoAtributo a_attVetor)const;
	
	template<class Membro, class M2, class M3, class M4, typename TipoIterador, typename TipoAtributo>
	void imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, const M4 a_m4, SmartDados &a_objetoDados, const TipoIterador a_iteradorInicial, const TipoIterador a_iteradorFinal, const TipoAtributo a_attVetor)const;

	template<class Membro, class M2, class M3, class M4, typename TipoAtributo>
	void imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, const M4 a_m4, SmartDados &a_objetoDados, const std::vector<TipoAtributo> a_attVetor)const;
	
	template<class Membro, class M2, class M3, class M4, typename TipoIterador, typename TipoAtributo>
	void imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, const M4 a_m4, SmartDados &a_objetoDados, const TipoIterador a_iteradorInicial, const TipoIterador a_iteradorFinal, const std::vector<TipoAtributo> a_attVetor)const;

	// AttMatriz

	// AttMatriz Direto

	template<typename TipoAtributo>
	void imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, SmartDados &a_objetoDados, const TipoAtributo a_attMatriz)const;
	
	template<typename TipoIterador1, typename TipoIterador2, typename TipoAtributo>
	void imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, SmartDados &a_objetoDados, const TipoIterador1 a_iterador1Inicial, const TipoIterador1 a_iterador1Final, const TipoIterador2 a_iterador2Inicial, const TipoIterador2 a_iterador2Final, const TipoAtributo a_attMatriz)const;

	template<typename TipoAtributo>
	void imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, SmartDados &a_objetoDados, const std::vector<TipoAtributo> a_attMatriz)const;
	
	template<typename TipoIterador1, typename TipoIterador2, typename TipoAtributo>
	void imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, SmartDados &a_objetoDados, const TipoIterador1 a_iterador1Inicial, const TipoIterador1 a_iterador1Final, const TipoIterador2 a_iterador2Inicial, const TipoIterador2 a_iterador2Final, const std::vector<TipoAtributo> a_attMatriz)const;

	// AttMatriz Membro

	template<class Membro, typename TipoAtributo>
	void imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, SmartDados &a_objetoDados, const TipoAtributo a_attMatriz)const;
	
	template<class Membro, typename TipoIterador1, typename TipoIterador2, typename TipoAtributo>
	void imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, SmartDados &a_objetoDados, const TipoIterador1 a_iterador1Inicial, const TipoIterador1 a_iterador1Final, const TipoIterador2 a_iterador2Inicial, const TipoIterador2 a_iterador2Final, const TipoAtributo a_attMatriz)const;

	template<class Membro, typename TipoAtributo>
	void imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, SmartDados &a_objetoDados, const std::vector<TipoAtributo> a_attMatriz)const;
	
	template<class Membro, typename TipoIterador1, typename TipoIterador2, typename TipoAtributo>
	void imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, SmartDados &a_objetoDados, const TipoIterador1 a_iterador1Inicial, const TipoIterador1 a_iterador1Final, const TipoIterador2 a_iterador2Inicial, const TipoIterador2 a_iterador2Final, const std::vector<TipoAtributo> a_attMatriz)const;

	// AttMatriz M2

	template<class Membro, class M2, typename TipoAtributo>
	void imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, SmartDados &a_objetoDados, const TipoAtributo a_attMatriz)const;
	
	template<class Membro, class M2, typename TipoIterador1, typename TipoIterador2, typename TipoAtributo>
	void imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, SmartDados &a_objetoDados, const TipoIterador1 a_iterador1Inicial, const TipoIterador1 a_iterador1Final, const TipoIterador2 a_iterador2Inicial, const TipoIterador2 a_iterador2Final, const TipoAtributo a_attMatriz)const;

	template<class Membro, class M2, typename TipoAtributo>
	void imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, SmartDados &a_objetoDados, const std::vector<TipoAtributo> a_attMatriz)const;
	
	template<class Membro, class M2, typename TipoIterador1, typename TipoIterador2, typename TipoAtributo>
	void imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, SmartDados &a_objetoDados, const TipoIterador1 a_iterador1Inicial, const TipoIterador1 a_iterador1Final, const TipoIterador2 a_iterador2Inicial, const TipoIterador2 a_iterador2Final, const std::vector<TipoAtributo> a_attMatriz)const;

	// AttMatriz M3

	template<class Membro, class M2, class M3, typename TipoAtributo>
	void imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, SmartDados &a_objetoDados, const TipoAtributo a_attMatriz)const;
	
	template<class Membro, class M2, class M3, typename TipoIterador1, typename TipoIterador2, typename TipoAtributo>
	void imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, SmartDados &a_objetoDados, const TipoIterador1 a_iterador1Inicial, const TipoIterador1 a_iterador1Final, const TipoIterador2 a_iterador2Inicial, const TipoIterador2 a_iterador2Final, const TipoAtributo a_attMatriz)const;

	template<class Membro, class M2, class M3, typename TipoAtributo>
	void imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, SmartDados &a_objetoDados, const std::vector<TipoAtributo> a_attMatriz)const;
	
	template<class Membro, class M2, class M3, typename TipoIterador1, typename TipoIterador2, typename TipoAtributo>
	void imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, SmartDados &a_objetoDados, const TipoIterador1 a_iterador1Inicial, const TipoIterador1 a_iterador1Final, const TipoIterador2 a_iterador2Inicial, const TipoIterador2 a_iterador2Final, const std::vector<TipoAtributo> a_attMatriz)const;

	// AttMatriz M4

	template<class Membro, class M2, class M3, class M4, typename TipoAtributo>
	void imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, const M4 a_m4, SmartDados &a_objetoDados, const TipoAtributo a_attMatriz)const;
	
	template<class Membro, class M2, class M3, class M4, typename TipoIterador1, typename TipoIterador2, typename TipoAtributo>
	void imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, const M4 a_m4, SmartDados &a_objetoDados, const TipoIterador1 a_iterador1Inicial, const TipoIterador1 a_iterador1Final, const TipoIterador2 a_iterador2Inicial, const TipoIterador2 a_iterador2Final, const TipoAtributo a_attMatriz)const;

	template<class Membro, class M2, class M3, class M4, typename TipoAtributo>
	void imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, const M4 a_m4, SmartDados &a_objetoDados, const std::vector<TipoAtributo> a_attMatriz)const;
	
	template<class Membro, class M2, class M3, class M4, typename TipoIterador1, typename TipoIterador2, typename TipoAtributo>
	void imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, const M4 a_m4, SmartDados &a_objetoDados, const TipoIterador1 a_iterador1Inicial, const TipoIterador1 a_iterador1Final, const TipoIterador2 a_iterador2Inicial, const TipoIterador2 a_iterador2Final, const std::vector<TipoAtributo> a_attMatriz)const;

};


#endif 


// AttComum Direto

inline void EntradaSaidaDados::imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, SmartDados & a_objetoDados)const{
	imprimirArquivoCSV_AttComum_(a_nomeArquivo, "", "", "", "", a_objetoDados, std::vector<std::string>());
}


template<typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, SmartDados & a_objetoDados, const TipoAtributo a_attComum)const{
	imprimirArquivoCSV_AttComum(a_nomeArquivo, a_objetoDados, std::vector<TipoAtributo>{a_attComum});
}


template<typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, SmartDados & a_objetoDados, const std::vector<TipoAtributo> a_attComum)const{
	std::vector<std::string> vetorAttComumStr = std::vector<std::string>(a_attComum.size(), "");
	for (int i = 0; i < int(a_attComum.size()); i++)
		vetorAttComumStr.at(i) = getString(a_attComum.at(i));
	imprimirArquivoCSV_AttComum_(a_nomeArquivo, "", "", "", "", a_objetoDados, vetorAttComumStr);
}

// AttComum Membro

template<class Membro>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, SmartDados & a_objetoDados)const{
	imprimirArquivoCSV_AttComum_(a_nomeArquivo, getFullString(a_m1), "", "", "", a_objetoDados, std::vector<std::string>());
}

template<class Membro, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, SmartDados & a_objetoDados, const TipoAtributo a_attComum)const{
	imprimirArquivoCSV_AttComum(a_nomeArquivo, a_m1, a_objetoDados, std::vector<TipoAtributo>{a_attComum});
}

template<class Membro, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, SmartDados & a_objetoDados, const std::vector<TipoAtributo> a_attComum)const{
	std::vector<std::string> vetorAttComumStr = std::vector<std::string>(a_attComum.size(), "");
	for (int i = 0; i < int(a_attComum.size()); i++)
		vetorAttComumStr.at(i) = getString(a_attComum.at(i));
	imprimirArquivoCSV_AttComum_(a_nomeArquivo, getFullString(a_m1), "", "", "", a_objetoDados, vetorAttComumStr);
}

// AttComum M2

template<class Membro, class M2>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, SmartDados & a_objetoDados)const{
	imprimirArquivoCSV_AttComum_(a_nomeArquivo, getFullString(a_m1), getFullString(a_m2), "", "", a_objetoDados, std::vector<std::string>());
}

template<class Membro, class M2, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, SmartDados & a_objetoDados, const TipoAtributo a_attComum)const{
	imprimirArquivoCSV_AttComum(a_nomeArquivo, a_m1, a_m2, a_objetoDados, std::vector<TipoAtributo>{a_attComum});
}

template<class Membro, class M2, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, SmartDados & a_objetoDados, const std::vector<TipoAtributo> a_attComum)const{
	std::vector<std::string> vetorAttComumStr = std::vector<std::string>(a_attComum.size(), "");
	for (int i = 0; i < int(a_attComum.size()); i++)
		vetorAttComumStr.at(i) = getString(a_attComum.at(i));
	imprimirArquivoCSV_AttComum_(a_nomeArquivo, getFullString(a_m1), getFullString(a_m2), "", "", a_objetoDados, vetorAttComumStr);
}

// AttComum M3

template<class Membro, class M2, class M3>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, SmartDados & a_objetoDados)const{
	imprimirArquivoCSV_AttComum_(a_nomeArquivo, getFullString(a_m1), getFullString(a_m2), getFullString(a_m3), "", a_objetoDados, std::vector<std::string>());
}

template<class Membro, class M2, class M3, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, SmartDados & a_objetoDados, TipoAtributo a_attComum)const{
	imprimirArquivoCSV_AttComum(a_nomeArquivo, a_m1, a_m2, a_m3, a_objetoDados, std::vector<TipoAtributo>{a_attComum});
}

template<class Membro, class M2, class M3, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, SmartDados & a_objetoDados, const std::vector<TipoAtributo> a_attComum)const{
	std::vector<std::string> vetorAttComumStr = std::vector<std::string>(a_attComum.size(), "");
	for (int i = 0; i < int(a_attComum.size()); i++)
		vetorAttComumStr.at(i) = getString(a_attComum.at(i));
	imprimirArquivoCSV_AttComum_(a_nomeArquivo, getFullString(a_m1), getFullString(a_m2), getFullString(a_m3), "", a_objetoDados, vetorAttComumStr);
}

// AttComum M4

template<class Membro, class M2, class M3, class M4>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, const M4 a_m4, SmartDados & a_objetoDados)const{
	imprimirArquivoCSV_AttComum_(a_nomeArquivo, getFullString(a_m1), getFullString(a_m2), getFullString(a_m3), getFullString(a_m4), a_objetoDados, std::vector<std::string>());
}

template<class Membro, class M2, class M3, class M4, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, const M4 a_m4, SmartDados & a_objetoDados, TipoAtributo a_attComum)const{
	imprimirArquivoCSV_AttComum(a_nomeArquivo, a_m1, a_m2, a_m3, a_m4, a_objetoDados, std::vector<TipoAtributo>{a_attComum});
}

template<class Membro, class M2, class M3, class M4, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttComum(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, const M4 a_m4, SmartDados & a_objetoDados, const std::vector<TipoAtributo> a_attComum)const{
	std::vector<std::string> vetorAttComumStr = std::vector<std::string>(a_attComum.size(), "");
	for (int i = 0; i < int(a_attComum.size()); i++)
		vetorAttComumStr.at(i) = getString(a_attComum.at(i));
	imprimirArquivoCSV_AttComum_(a_nomeArquivo, getFullString(a_m1), getFullString(a_m2), getFullString(a_m3), getFullString(a_m4), a_objetoDados, vetorAttComumStr);
}


// AttVetor Direto

template<typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, SmartDados & a_objetoDados, const TipoAtributo a_attVetor)const{
	imprimirArquivoCSV_AttVetor(a_nomeArquivo, a_objetoDados, std::vector<TipoAtributo>{a_attVetor});
}

template<typename TipoIterador, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, SmartDados & a_objetoDados, const TipoIterador a_iteradorInicial, const TipoIterador a_iteradorFinal, const TipoAtributo a_attVetor)const{
	imprimirArquivoCSV_AttVetor(a_nomeArquivo, a_objetoDados, a_iteradorInicial, a_iteradorFinal, std::vector<TipoAtributo>{a_attVetor});
}

template<typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, SmartDados & a_objetoDados, const std::vector<TipoAtributo> a_attVetor)const{
	std::vector<std::string> vetorAttVetorStr = std::vector<std::string>(a_attVetor.size(), "");
	for (int i = 0; i < int(a_attVetor.size()); i++)
		vetorAttVetorStr.at(i) = getString(a_attVetor.at(i));
	imprimirArquivoCSV_AttVetor_(a_nomeArquivo, "", "", "", "", a_objetoDados, "", "", vetorAttVetorStr);
}

template<typename TipoIterador, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, SmartDados & a_objetoDados, const TipoIterador a_iteradorInicial, const TipoIterador a_iteradorFinal, const std::vector<TipoAtributo> a_attVetor)const{
	std::vector<std::string> vetorAttVetorStr = std::vector<std::string>(a_attVetor.size(), "");
	for (int i = 0; i < int(a_attVetor.size()); i++)
		vetorAttVetorStr.at(i) = getString(a_attVetor.at(i));
	imprimirArquivoCSV_AttVetor_(a_nomeArquivo, "", "", "", "", a_objetoDados, getString(a_iteradorInicial), getString(a_iteradorFinal), vetorAttVetorStr);
}

// AttVetor Membro

template<class Membro, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, SmartDados & a_objetoDados, const TipoAtributo a_attVetor)const{
	imprimirArquivoCSV_AttVetor(a_nomeArquivo, a_m1, a_objetoDados, std::vector<TipoAtributo>{a_attVetor});
}

template<class Membro, typename TipoIterador, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, SmartDados & a_objetoDados, const TipoIterador a_iteradorInicial, const TipoIterador a_iteradorFinal, const TipoAtributo a_attVetor)const{
	imprimirArquivoCSV_AttVetor(a_nomeArquivo, a_m1, a_objetoDados, a_iteradorInicial, a_iteradorFinal, std::vector<TipoAtributo>{a_attVetor});
}

template<class Membro, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, SmartDados & a_objetoDados, const std::vector<TipoAtributo> a_attVetor)const{
	std::vector<std::string> vetorAttVetorStr = std::vector<std::string>(a_attVetor.size(), "");
	for (int i = 0; i < int(a_attVetor.size()); i++)
		vetorAttVetorStr.at(i) = getString(a_attVetor.at(i));
	imprimirArquivoCSV_AttVetor_(a_nomeArquivo, getFullString(a_m1), "", "", "", a_objetoDados, "", "", vetorAttVetorStr);
}

template<class Membro, typename TipoIterador, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, SmartDados & a_objetoDados, const TipoIterador a_iteradorInicial, const TipoIterador a_iteradorFinal, const std::vector<TipoAtributo> a_attVetor)const{
	std::vector<std::string> vetorAttVetorStr = std::vector<std::string>(a_attVetor.size(), "");
	for (int i = 0; i < int(a_attVetor.size()); i++)
		vetorAttVetorStr.at(i) = getString(a_attVetor.at(i));
	imprimirArquivoCSV_AttVetor_(a_nomeArquivo, getFullString(a_m1), "", "", "", a_objetoDados, getString(a_iteradorInicial), getString(a_iteradorFinal), vetorAttVetorStr);
}

// AttVetor M2

template<class Membro, class M2, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, SmartDados & a_objetoDados, const TipoAtributo a_attVetor)const{
	imprimirArquivoCSV_AttVetor(a_nomeArquivo, a_m1, a_m2, a_objetoDados, std::vector<TipoAtributo>{a_attVetor});
}

template<class Membro, class M2, typename TipoIterador, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, SmartDados & a_objetoDados, const TipoIterador a_iteradorInicial, const TipoIterador a_iteradorFinal, const TipoAtributo a_attVetor)const{
	imprimirArquivoCSV_AttVetor(a_nomeArquivo, a_m1, a_m2, a_objetoDados, a_iteradorInicial, a_iteradorFinal, std::vector<TipoAtributo>{a_attVetor});
}

template<class Membro, class M2, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, SmartDados & a_objetoDados, const std::vector<TipoAtributo> a_attVetor)const{
	std::vector<std::string> vetorAttVetorStr = std::vector<std::string>(a_attVetor.size(), "");
	for (int i = 0; i < int(a_attVetor.size()); i++)
		vetorAttVetorStr.at(i) = getString(a_attVetor.at(i));
	imprimirArquivoCSV_AttVetor_(a_nomeArquivo, getFullString(a_m1), getFullString(a_m2), "", "", a_objetoDados, "", "", vetorAttVetorStr);
}

template<class Membro, class M2, typename TipoIterador, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, SmartDados & a_objetoDados, const TipoIterador a_iteradorInicial, const TipoIterador a_iteradorFinal, const std::vector<TipoAtributo> a_attVetor)const{
	std::vector<std::string> vetorAttVetorStr = std::vector<std::string>(a_attVetor.size(), "");
	for (int i = 0; i < int(a_attVetor.size()); i++)
		vetorAttVetorStr.at(i) = getString(a_attVetor.at(i));
	imprimirArquivoCSV_AttVetor_(a_nomeArquivo, getFullString(a_m1), getFullString(a_m2), "", "", a_objetoDados, getString(a_iteradorInicial), getString(a_iteradorFinal), vetorAttVetorStr);
}

// AttVetor M3

template<class Membro, class M2, class M3, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, SmartDados & a_objetoDados, const TipoAtributo a_attVetor)const{
	imprimirArquivoCSV_AttVetor(a_nomeArquivo, a_m1, a_m2, a_m3, a_objetoDados, std::vector<TipoAtributo>{a_attVetor});
}

template<class Membro, class M2, class M3, typename TipoIterador, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, SmartDados & a_objetoDados, const TipoIterador a_iteradorInicial, const TipoIterador a_iteradorFinal, const TipoAtributo a_attVetor)const{
	imprimirArquivoCSV_AttVetor(a_nomeArquivo, a_m1, a_m2, a_m3, a_objetoDados, a_iteradorInicial, a_iteradorFinal, std::vector<TipoAtributo>{a_attVetor});
}

template<class Membro, class M2, class M3, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, SmartDados & a_objetoDados, const std::vector<TipoAtributo> a_attVetor)const{
	std::vector<std::string> vetorAttVetorStr = std::vector<std::string>(a_attVetor.size(), "");
	for (int i = 0; i < int(a_attVetor.size()); i++)
		vetorAttVetorStr.at(i) = getString(a_attVetor.at(i));
	imprimirArquivoCSV_AttVetor_(a_nomeArquivo, getFullString(a_m1), getFullString(a_m2), getFullString(a_m3), "", a_objetoDados, "", "", vetorAttVetorStr);
}

template<class Membro, class M2, class M3, typename TipoIterador, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, SmartDados & a_objetoDados, const TipoIterador a_iteradorInicial, const TipoIterador a_iteradorFinal, const std::vector<TipoAtributo> a_attVetor)const{
	std::vector<std::string> vetorAttVetorStr = std::vector<std::string>(a_attVetor.size(), "");
	for (int i = 0; i < int(a_attVetor.size()); i++)
		vetorAttVetorStr.at(i) = getString(a_attVetor.at(i));
	imprimirArquivoCSV_AttVetor_(a_nomeArquivo, getFullString(a_m1), getFullString(a_m2), getFullString(a_m3), "", a_objetoDados, getString(a_iteradorInicial), getString(a_iteradorFinal), vetorAttVetorStr);
}

// AttVetor M4

template<class Membro, class M2, class M3, class M4, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, const M4 a_m4, SmartDados & a_objetoDados, const TipoAtributo a_attVetor)const{
	imprimirArquivoCSV_AttVetor(a_nomeArquivo, a_m1, a_m2, a_m3, a_m4, a_objetoDados, std::vector<TipoAtributo>{a_attVetor});
}

template<class Membro, class M2, class M3, class M4, typename TipoIterador, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, const M4 a_m4, SmartDados & a_objetoDados, const TipoIterador a_iteradorInicial, const TipoIterador a_iteradorFinal, const TipoAtributo a_attVetor)const{
	imprimirArquivoCSV_AttVetor(a_nomeArquivo, a_m1, a_m2, a_m3, a_m4, a_objetoDados, a_iteradorInicial, a_iteradorFinal, std::vector<TipoAtributo>{a_attVetor});
}

template<class Membro, class M2, class M3, class M4, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, const M4 a_m4, SmartDados & a_objetoDados, const std::vector<TipoAtributo> a_attVetor)const{
	std::vector<std::string> vetorAttVetorStr = std::vector<std::string>(a_attVetor.size(), "");
	for (int i = 0; i < int(a_attVetor.size()); i++)
		vetorAttVetorStr.at(i) = getString(a_attVetor.at(i));
	imprimirArquivoCSV_AttVetor_(a_nomeArquivo, getFullString(a_m1), getFullString(a_m2), getFullString(a_m3), getFullString(a_m4), a_objetoDados, "", "", vetorAttVetorStr);
}

template<class Membro, class M2, class M3, class M4, typename TipoIterador, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttVetor(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, const M4 a_m4, SmartDados & a_objetoDados, const TipoIterador a_iteradorInicial, const TipoIterador a_iteradorFinal, const std::vector<TipoAtributo> a_attVetor)const{
	std::vector<std::string> vetorAttVetorStr = std::vector<std::string>(a_attVetor.size(), "");
	for (int i = 0; i < int(a_attVetor.size()); i++)
		vetorAttVetorStr.at(i) = getString(a_attVetor.at(i));
	imprimirArquivoCSV_AttVetor_(a_nomeArquivo, getFullString(a_m1), getFullString(a_m2), getFullString(a_m3), getFullString(a_m4), a_objetoDados, getString(a_iteradorInicial), getString(a_iteradorFinal), vetorAttVetorStr);
}

// AttMatriz Direto

template<typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, SmartDados & a_objetoDados, const TipoAtributo a_attMatriz)const{
	imprimirArquivoCSV_AttMatriz(a_nomeArquivo, a_objetoDados, std::vector<TipoAtributo>{a_attMatriz});
}

template<typename TipoIterador1, typename TipoIterador2, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, SmartDados & a_objetoDados, const TipoIterador1 a_iterador1Inicial, const TipoIterador1 a_iterador1Final, TipoIterador2 a_iterador2Inicial, const TipoIterador2 a_iterador2Final, const TipoAtributo a_attMatriz)const{
	imprimirArquivoCSV_AttMatriz(a_nomeArquivo, a_objetoDados, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, std::vector<TipoAtributo>{a_attMatriz});
}

template<typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, SmartDados & a_objetoDados, const std::vector<TipoAtributo> a_attMatriz)const{
	std::vector<std::string> vetorAttMatrizStr = std::vector<std::string>(a_attMatriz.size(), "");
	for (int i = 0; i < int(a_attMatriz.size()); i++)
		vetorAttMatrizStr.at(i) = getString(a_attMatriz.at(i));
	imprimirArquivoCSV_AttMatriz_(a_nomeArquivo, "", "", "", "", a_objetoDados, "", "", "", "", vetorAttMatrizStr);
}

template<typename TipoIterador1, typename TipoIterador2, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, SmartDados & a_objetoDados, const TipoIterador1 a_iterador1Inicial, const TipoIterador1 a_iterador1Final, TipoIterador2 a_iterador2Inicial, const TipoIterador2 a_iterador2Final, const std::vector<TipoAtributo> a_attMatriz)const{
	std::vector<std::string> vetorAttMatrizStr = std::vector<std::string>(a_attMatriz.size(), "");
	for (int i = 0; i < int(a_attMatriz.size()); i++)
		vetorAttMatrizStr.at(i) = getString(a_attMatriz.at(i));
	imprimirArquivoCSV_AttMatriz_(a_nomeArquivo, "", "", "", "", a_objetoDados, getString(a_iterador1Inicial), getString(a_iterador1Final), getString(a_iterador2Inicial), getString(a_iterador2Final), vetorAttMatrizStr);
}

// AttMatriz Membro

template<class Membro, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, SmartDados & a_objetoDados, const TipoAtributo a_attMatriz)const{
	imprimirArquivoCSV_AttMatriz(a_nomeArquivo, a_m1, a_objetoDados, std::vector<TipoAtributo>{a_attMatriz});
}

template<class Membro, typename TipoIterador1, typename TipoIterador2, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, SmartDados & a_objetoDados, const TipoIterador1 a_iterador1Inicial, const TipoIterador1 a_iterador1Final, TipoIterador2 a_iterador2Inicial, const TipoIterador2 a_iterador2Final, const TipoAtributo a_attMatriz)const{
	imprimirArquivoCSV_AttMatriz(a_nomeArquivo, a_m1, a_objetoDados, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, std::vector<TipoAtributo>{a_attMatriz});
}

template<class Membro, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, SmartDados & a_objetoDados, const std::vector<TipoAtributo> a_attMatriz)const{
	std::vector<std::string> vetorAttMatrizStr = std::vector<std::string>(a_attMatriz.size(), "");
	for (int i = 0; i < int(a_attMatriz.size()); i++)
		vetorAttMatrizStr.at(i) = getString(a_attMatriz.at(i));
	imprimirArquivoCSV_AttMatriz_(a_nomeArquivo, getFullString(a_m1), "", "", "", a_objetoDados, "", "", "", "", vetorAttMatrizStr);
}

template<class Membro, typename TipoIterador1, typename TipoIterador2, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, SmartDados & a_objetoDados, const TipoIterador1 a_iterador1Inicial, const TipoIterador1 a_iterador1Final, TipoIterador2 a_iterador2Inicial, const TipoIterador2 a_iterador2Final, const std::vector<TipoAtributo> a_attMatriz)const{
	std::vector<std::string> vetorAttMatrizStr = std::vector<std::string>(a_attMatriz.size(), "");
	for (int i = 0; i < int(a_attMatriz.size()); i++)
		vetorAttMatrizStr.at(i) = getString(a_attMatriz.at(i));
	imprimirArquivoCSV_AttMatriz_(a_nomeArquivo, getFullString(a_m1), "", "", "", a_objetoDados, getString(a_iterador1Inicial), getString(a_iterador1Final), getString(a_iterador2Inicial), getString(a_iterador2Final), vetorAttMatrizStr);
}

// AttMatriz M2

template<class Membro, class M2, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, SmartDados & a_objetoDados, const TipoAtributo a_attMatriz)const{
	imprimirArquivoCSV_AttMatriz(a_nomeArquivo, a_m1, a_m2, a_objetoDados, std::vector<TipoAtributo>{a_attMatriz});
}

template<class Membro, class M2, typename TipoIterador1, typename TipoIterador2, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, SmartDados & a_objetoDados, const TipoIterador1 a_iterador1Inicial, const TipoIterador1 a_iterador1Final, TipoIterador2 a_iterador2Inicial, const TipoIterador2 a_iterador2Final, const TipoAtributo a_attMatriz)const{
	imprimirArquivoCSV_AttMatriz(a_nomeArquivo, a_m1, a_m2, a_objetoDados, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, std::vector<TipoAtributo>{a_attMatriz});
}

template<class Membro, class M2, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, SmartDados & a_objetoDados, const std::vector<TipoAtributo> a_attMatriz)const{
	std::vector<std::string> vetorAttMatrizStr = std::vector<std::string>(a_attMatriz.size(), "");
	for (int i = 0; i < int(a_attMatriz.size()); i++)
		vetorAttMatrizStr.at(i) = getString(a_attMatriz.at(i));
	imprimirArquivoCSV_AttMatriz_(a_nomeArquivo, getFullString(a_m1), getFullString(a_m2), "", "", a_objetoDados, "", "", "", "", vetorAttMatrizStr);
}

template<class Membro, class M2, typename TipoIterador1, typename TipoIterador2, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, SmartDados & a_objetoDados, const TipoIterador1 a_iterador1Inicial, const TipoIterador1 a_iterador1Final, TipoIterador2 a_iterador2Inicial, const TipoIterador2 a_iterador2Final, const std::vector<TipoAtributo> a_attMatriz)const{
	std::vector<std::string> vetorAttMatrizStr = std::vector<std::string>(a_attMatriz.size(), "");
	for (int i = 0; i < int(a_attMatriz.size()); i++)
		vetorAttMatrizStr.at(i) = getString(a_attMatriz.at(i));
	imprimirArquivoCSV_AttMatriz_(a_nomeArquivo, getFullString(a_m1), getFullString(a_m2), "", "", a_objetoDados, getString(a_iterador1Inicial), getString(a_iterador1Final), getString(a_iterador2Inicial), getString(a_iterador2Final), vetorAttMatrizStr);
}

// AttMatriz M3

template<class Membro, class M2, class M3, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, SmartDados & a_objetoDados, const TipoAtributo a_attMatriz)const{
	imprimirArquivoCSV_AttMatriz(a_nomeArquivo, a_m1, a_m2, a_m3, a_objetoDados, std::vector<TipoAtributo>{a_attMatriz});
}

template<class Membro, class M2, class M3, typename TipoIterador1, typename TipoIterador2, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, SmartDados & a_objetoDados, const TipoIterador1 a_iterador1Inicial, const TipoIterador1 a_iterador1Final, TipoIterador2 a_iterador2Inicial, const TipoIterador2 a_iterador2Final, const TipoAtributo a_attMatriz)const{
	imprimirArquivoCSV_AttMatriz(a_nomeArquivo, a_m1, a_m2, a_m3, a_objetoDados, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, std::vector<TipoAtributo>{a_attMatriz});
}

template<class Membro, class M2, class M3, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, SmartDados & a_objetoDados, const std::vector<TipoAtributo> a_attMatriz)const{
	std::vector<std::string> vetorAttMatrizStr = std::vector<std::string>(a_attMatriz.size(), "");
	for (int i = 0; i < int(a_attMatriz.size()); i++)
		vetorAttMatrizStr.at(i) = getString(a_attMatriz.at(i));
	imprimirArquivoCSV_AttMatriz_(a_nomeArquivo, getFullString(a_m1), getFullString(a_m2), getFullString(a_m3), "", a_objetoDados, "", "", "", "", vetorAttMatrizStr);
}

template<class Membro, class M2, class M3, typename TipoIterador1, typename TipoIterador2, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, SmartDados & a_objetoDados, const TipoIterador1 a_iterador1Inicial, const TipoIterador1 a_iterador1Final, TipoIterador2 a_iterador2Inicial, const TipoIterador2 a_iterador2Final, const std::vector<TipoAtributo> a_attMatriz)const{
	std::vector<std::string> vetorAttMatrizStr = std::vector<std::string>(a_attMatriz.size(), "");
	for (int i = 0; i < int(a_attMatriz.size()); i++)
		vetorAttMatrizStr.at(i) = getString(a_attMatriz.at(i));
	imprimirArquivoCSV_AttMatriz_(a_nomeArquivo, getFullString(a_m1), getFullString(a_m2), getFullString(a_m3), "", a_objetoDados, getString(a_iterador1Inicial), getString(a_iterador1Final), getString(a_iterador2Inicial), getString(a_iterador2Final), vetorAttMatrizStr);
}

// AttMatriz M4

template<class Membro, class M2, class M3, class M4, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, const M4 a_m4, SmartDados & a_objetoDados, const TipoAtributo a_attMatriz)const{
	imprimirArquivoCSV_AttMatriz(a_nomeArquivo, a_m1, a_m2, a_m3, a_m4, a_objetoDados, std::vector<TipoAtributo>{a_attMatriz});
}

template<class Membro, class M2, class M3, class M4, typename TipoIterador1, typename TipoIterador2, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, const M4 a_m4, SmartDados & a_objetoDados, const TipoIterador1 a_iterador1Inicial, const TipoIterador1 a_iterador1Final, TipoIterador2 a_iterador2Inicial, const TipoIterador2 a_iterador2Final, const TipoAtributo a_attMatriz)const{
	imprimirArquivoCSV_AttMatriz(a_nomeArquivo, a_m1, a_m2, a_m3, a_m4, a_objetoDados, a_iterador1Inicial, a_iterador1Final, a_iterador2Inicial, a_iterador2Final, std::vector<TipoAtributo>{a_attMatriz});
}

template<class Membro, class M2, class M3, class M4, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, const M4 a_m4, SmartDados & a_objetoDados, const std::vector<TipoAtributo> a_attMatriz)const{
	std::vector<std::string> vetorAttMatrizStr = std::vector<std::string>(a_attMatriz.size(), "");
	for (int i = 0; i < int(a_attMatriz.size()); i++)
		vetorAttMatrizStr.at(i) = getString(a_attMatriz.at(i));
	imprimirArquivoCSV_AttMatriz_(a_nomeArquivo, getFullString(a_m1), getFullString(a_m2), getFullString(a_m3), getFullString(a_m4), a_objetoDados, "", "", "", "", vetorAttMatrizStr);
}

template<class Membro, class M2, class M3, class M4, typename TipoIterador1, typename TipoIterador2, typename TipoAtributo>
inline void EntradaSaidaDados::imprimirArquivoCSV_AttMatriz(const std::string a_nomeArquivo, const Membro a_m1, const M2 a_m2, const M3 a_m3, const M4 a_m4, SmartDados & a_objetoDados, const TipoIterador1 a_iterador1Inicial, const TipoIterador1 a_iterador1Final, TipoIterador2 a_iterador2Inicial, const TipoIterador2 a_iterador2Final, const std::vector<TipoAtributo> a_attMatriz)const{
	std::vector<std::string> vetorAttMatrizStr = std::vector<std::string>(a_attMatriz.size(), "");
	for (int i = 0; i < int(a_attMatriz.size()); i++)
		vetorAttMatrizStr.at(i) = getString(a_attMatriz.at(i));
	imprimirArquivoCSV_AttMatriz_(a_nomeArquivo, getFullString(a_m1), getFullString(a_m2), getFullString(a_m3), getFullString(a_m4), a_objetoDados, getString(a_iterador1Inicial), getString(a_iterador1Final), getString(a_iterador2Inicial), getString(a_iterador2Final), vetorAttMatrizStr);
}


