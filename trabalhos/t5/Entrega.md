[Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2018a) > Trabalhos

# T5: Aplicação do método de Monte Carlo em OpenMP

Disciplina: ELC139 - Programação Paralela <br/>
Aluno: Rhauani Weber Aita Fazul e Roger Eduardo Couto

### Versão em C++

- Primeira solução

	+ Esta solução é baseada no programa [firesim-sequencial-cpp](experiments/firesim-sequencial-cpp). Os arquivos de código fonte da versão paralelizada estão disponíveis em [firesim-openmp-cpp](experiments/firesim-openmp-cpp).

- Segunda solução

	+ Esta solução varia o tipo de escalonamento adotado pelo programa anterior. Para utilizá-la, basta comentar a [linha 60](experiments/firesim-openmp-cpp/firesim.cpp#L60) do arquivo principal e descomentar a [linha 59](experiments/firesim-openmp-cpp/firesim.cpp#L59).

### Versão em C

- Primeira solução

	+ Os arquivos de código fonte estão disponíveis em [firesim-openmp-c](experiments/firesim-openmp-c), a primeira versão (recursiva) faz uso do arquivo [burn_rec](experiments/firesim-openmp-c/burn_rec.c).

- Segunda solução

	+ Esta solução utiliza matrizes esparsas, [burn_me](experiments/firesim-openmp-c/burn_me.c) é o arquivo principal.

### Apresentação

Os _slides_ podem ser encontrados no arquivo [apresentacao](apresentacao.pdf).

### Referências
- FSU Department of Science Computing. <i>C++ Examples of Parallel Programming with OpenMP</i>. https://goo.gl/sqmujr
- Imperial College London. <i>Monte Carlo Simulation & Parallel Computing</i>. https://goo.gl/N35vdu
- Lawrence Livermore National Laboratory. <i>OpenMP</i>. https://goo.gl/o2wTxR
- Mark Bull. <i>OpenMP Tips, Tricks and Gotchas</i>. https://goo.gl/L9Xhyp
- OpenMP. <i>OpenMP C and C++ Application Program Interface</i>. https://goo.gl/wPbQCn
- OpenMP. <i>Summary of OpenMP 3.0 C/C++ Syntax</i>. https://goo.gl/VdvSpi
- Shodor. <i>Interactivate: Fire!!</i>. https://goo.gl/gL9ft6
- Wikipedia. <i>Monte Carlo method</i>. https://goo.gl/pM5nGs
