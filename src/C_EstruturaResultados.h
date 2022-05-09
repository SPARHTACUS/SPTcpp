#pragma once

template <typename Tipo>
struct EstruturaResultados {
	int   bloco = 0;
	int   conta = 0;
	int   total = 0;
	int* displ = nullptr;
	int* sizeT = nullptr;
	int* size1 = nullptr;
	int* size2 = nullptr;
	int* size3 = nullptr;
	Tipo* vetor = nullptr;
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
			displ = new int[bloco];
			sizeT = new int[bloco];
			size1 = new int[bloco];
			size2 = new int[bloco];
			size3 = new int[bloco];
			for (int i = 0; i < bloco; i++) {
				displ[i] = 0;
				sizeT[i] = 0;
				size1[i] = 0;
				size2[i] = 0;
				size3[i] = 0;
			}
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
		size1[a_idx] = a_size1;
		size2[a_idx] = a_size2;
		size3[a_idx] = a_size3;
		sizeT[a_idx] = a_size1;
		if (a_size2 > 0)
			sizeT[a_idx] *= a_size2;
		if (a_size3 > 0)
			sizeT[a_idx] *= a_size3;
		if (a_idx > 0)
			displ[a_idx] = displ[a_idx - 1] + sizeT[a_idx - 1];
		if (a_idx == bloco - 1) {
			total = displ[a_idx] + sizeT[a_idx];
			vetor = new Tipo[total];
		}
	};
	void esvaziar() {
		bloco = 0;
		conta = 0;
		total = 0;
		delete[] displ;
		displ = nullptr;
		delete[] sizeT;
		sizeT = nullptr;
		delete[] size1;
		size1 = nullptr;
		delete[] size2;
		size2 = nullptr;
		delete[] size3;
		size3 = nullptr;
		delete[] vetor;
		vetor = nullptr;
	};
	void zerar() {
		bloco = 0;
		conta = 0;
		total = 0;
		displ = nullptr;
		sizeT = nullptr;
		size1 = nullptr;
		size2 = nullptr;
		size3 = nullptr;
		vetor = nullptr;
	}
	void add(const Tipo a_valor) {
		if ((conta >= 0) && (conta < total)) {
			vetor[conta] = a_valor;
			conta++;
		}
		else
			throw std::invalid_argument("EstruturaResultados::add(): nao foi possivel adicionar valor.");
	}
};