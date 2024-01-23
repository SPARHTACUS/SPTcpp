#pragma once

template <typename Tipo>
struct EstruturaResultados {
	int   bloco = 0;
	int   conta = 0;
	int   total = 0;
	std::vector<int> displ;
	std::vector<int> sizeT;
	std::vector<int> size1;
	std::vector<int> size2;
	std::vector<int> size3;
	std::vector<Tipo> vetor;
	EstruturaResultados() {};
	EstruturaResultados(const int a_bloco) {
		inicializar(a_bloco);
	}; // EstruturaResultados(const int a_bloco) {
	~EstruturaResultados() { esvaziar(); };
	EstruturaResultados& operator=(EstruturaResultados<Tipo>& instanciaCopiar) {
		esvaziar();
		bloco = instanciaCopiar.bloco;
		conta = instanciaCopiar.conta;
		total = instanciaCopiar.total;
		displ = instanciaCopiar.displ;
		sizeT = instanciaCopiar.sizeT;
		size1 = instanciaCopiar.size1;
		size2 = instanciaCopiar.size2;
		size3 = instanciaCopiar.size3;
		vetor = instanciaCopiar.vetor;
		instanciaCopiar.zerar();
		return *this;
	};
	void inicializar(const int a_bloco) {
		if (a_bloco > 0) {
			esvaziar();
			bloco = a_bloco;
			displ = std::vector<int>(bloco, 0);
			sizeT = std::vector<int>(bloco, 0);
			size1 = std::vector<int>(bloco, 0);
			size2 = std::vector<int>(bloco, 0);
			size3 = std::vector<int>(bloco, 0);
		} // if (a_bloco > 0) {
	}
	void setSize(const int a_idx, int a_size1, int a_size2 = -1, int a_size3 = -1) {
		if ((a_idx < 0) || (a_idx >= bloco))
			throw std::invalid_argument("EstruturaResultados::setSize(): a_idx invalido.");
		if (a_size1 < 0)
			throw std::invalid_argument("EstruturaResultados::setSize(): a_size1 negativo.");
		if ((a_size1 == 0) || (a_size2 == 0) || (a_size3 == 0)) {
			a_size1 = 0;
			a_size2 = 0;
			a_size3 = 0;
		} // if ((a_size1 == 0) || (a_size2 == 0) || (a_size3 == 0)) {
		if (a_size2 < 0) {
			a_size2 = 0;
			a_size3 = 0;
		} // if (a_size2 < 0) {
		if (a_size3 < 0)
			a_size3 = 0;
		size1.at(a_idx) = a_size1;
		size2.at(a_idx) = a_size2;
		size3.at(a_idx) = a_size3;
		sizeT.at(a_idx) = a_size1;
		if (a_size2 > 0)
			sizeT.at(a_idx) *= a_size2;
		if (a_size3 > 0)
			sizeT.at(a_idx) *= a_size3;
		if (a_idx > 0)
			displ.at(a_idx) = displ.at(a_idx - 1) + sizeT.at(a_idx - 1);
		if (a_idx == bloco - 1) {
			total = displ.at(a_idx) + sizeT.at(a_idx);
			vetor = std::vector<Tipo>(total, Tipo());
		}
	};
	void esvaziar() {
		bloco = 0;
		conta = 0;
		total = 0;
		displ = std::vector<int>();
		sizeT = std::vector<int>();
		size1 = std::vector<int>();
		size2 = std::vector<int>();
		size3 = std::vector<int>();
		vetor = std::vector<Tipo>();
	};
	void zerar() {
		bloco = 0;
		conta = 0;
		total = 0;
		displ = std::vector<int>();
		sizeT = std::vector<int>();
		size1 = std::vector<int>();
		size2 = std::vector<int>();
		size3 = std::vector<int>();
		vetor = std::vector<Tipo>();
	}
	void add(const Tipo a_valor) {
		if ((conta >= 0) && (conta < total)) {
			vetor.at(conta) = a_valor;
			conta++;
		}
		else
			throw std::invalid_argument("EstruturaResultados::add(): nao foi possivel adicionar valor.");
	}
};