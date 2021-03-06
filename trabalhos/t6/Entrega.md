[Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2018a) > Trabalhos

# T6: N-Rainhas com OpenMP

Disciplina: ELC139 - Programação Paralela <br/>
Aluno: Rhauani Weber Aita Fazul e Roger Eduardo Couto


### Primeira solução

- Esta solução é baseada no programa [nqueens-sequential-base](experiments/nqueens-sequential-base). Os arquivos de código fonte da versão paralelizada estão disponíveis em [nqueens-openmp](experiments/nqueens-openmp).

### Segunda solução

- Esta solução varia o tipo de escalonamento adotado pelo programa anterior. Para utilizá-la, basta descomentar a [linha 70](experiments/nqueens-openmp/nqueens.c#L70) do arquivo principal e comentar a [linha 71](experiments/nqueens-openmp/nqueens.c#L71).

### Terceira solução
	
- A solução, disponível em [nqueens-openmp-alt](experiments/nqueens-openmp-alt), varia algumas das estratégias para paralelização utilizadas nas soluções anteriores.

### Quarta solução

- Em contraste com o programa base, está solução faz uso de uma implementação própria, que pode ser encontrada em [nqueens-sequential-and-openmp-opt](experiments/nqueens-sequential-and-openmp-opt).

### Apresentação

- Os _slides_ podem ser encontrados no arquivo de [apresentação](apresentacao.pdf).

### Referências
- FSU Department of Science Computing. <i>C++ Examples of Parallel Programming with OpenMP</i>. https://goo.gl/sqmujr
- Google Optimization Tools. <i>The N-queens Problem</i>. https://goo.gl/fqS6sW 
- Lawrence Livermore National Laboratory. <i>OpenMP</i>. https://goo.gl/o2wTxR
- Mark Bull. <i>OpenMP Tips, Tricks and Gotchas</i>. https://goo.gl/L9Xhyp
- OpenMP. <i>OpenMP C and C++ Application Program Interface</i>. https://goo.gl/wPbQCn
- OpenMP. <i>Summary of OpenMP 3.0 C/C++ Syntax</i>. https://goo.gl/VdvSpi
- Texas Advanced Computing Center. <i>OpenMP topic: Reductions</i>. https://goo.gl/BbMrFP
- Wikipedia. <i>Eight queens puzzle</i>. https://goo.gl/wnDqp2
