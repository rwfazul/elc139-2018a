[Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2018a) > Trabalhos

# Trabalho final: Chapel e X10

Disciplina: ELC139 - Programação Paralela <br/>
Aluno: Rhauani Weber Aita Fazul

## Sumário

- [Chapel](#chapel)
	- [O que é Chapel?](#lp-chapel)
	- [Instalação](#chapel-install)
	- [Compilação e execução](#chapel-exec)
- [X10](#x10)
	- [O que é X10?](#lp-x10)
	- [Instalação](#x10-install)
	- [Compilação e execução](#x10-exec)
- [Análise: _dot product_](#analise)
	- [Desempenho](#desempenho)
		- [Sequencial](#sequencial)
		- [Pthreads](#pthreads)
		- [OpenMP](#openmp)
		- [MPI](#mpi)
		- [Chapel](#dotprod-chapel)
		- [X10](#dotprod-x10)
	- [Comparação](#comparacao)
		- [_Speedup_](#speedup)
- [Referências](#referências)

<!-- CHAPEL -->
# Chapel

<a name="lp-chapel"></a>
## O que é Chapel?

Uma lingaguem de programação paralela moderna (e _open-souce_) baseada em Java, C/C++, Python e Matlab. Visa aumentar a produtividade no desenvolvimento de programas concorrentes e paralelos (vários tipos de paralelismo em um único programa de maneira simples), sendo portável (roda em notebooks com diferentes SOs, _clusters_, _cloud_ e sistemas de HPC) e escalável (e.g. sistemas de memória distribúida). 

<a name="chapel-install"></a>
## Instalação

Baseado no [guia oficial](https://chapel-lang.org/docs/usingchapel/QUICKSTART.html) de instalação:

+ Alguns pré-requisitos básicos podem ser vistos [aqui](https://chapel-lang.org/docs/usingchapel/prereqs.html#readme-prereqs) (a maioria são pacotes básicos que muitas distribuições _Unix-like_ já possuem por _default_).

+ O download do _.tar_ contendo o código fonte pode ser feito [aqui](https://chapel-lang.org/download.html) (atualmente na versão 1.17.1).

Em seguida:
 
``` bash
	$ tar -xzf chapel-1.17.1.tar.gz && cd chapel-1.17.1 && source util/quickstart/setchplenv.bash
```

O _build_ e a verificação da instalação são realizados com um arquivo Makefile:

``` bash
	$ make && make check
```

Se tudo correu bem: "SUCCESS: 'make check' passed!".

<a name="chapel-exec"></a>
## Compilação e execução

Básico:

``` bash
	$ chpl -o file file.chpl
	$ ./file
```

- Compilar com a flag '<em>--fast</em>' ativa uma série de otimizações;
- Se no programa estiver definido uma variável de configuração (ex. <em>config const name = "value";</em>), é possível setá-la com novos valores:
	
``` bash
	$ ./file --name="novoValor"
``` 

<!-- X10 -->
# X10

<a name="lp-x10"></a>
## O que é X10?

<a name="x10-install"></a>
## Instalação


<a name="x10-exec"></a>
## Compilação e execução


<!-- ANALISE -->
<a name="analise"></a>
# Análise: _dot product_

## Desempenho

### Sequencial

### Pthreads

### OpenMP

### MPI

<a name="dotprod-chapel"></a>
### Chapel

<a name="dotprod-x10"></a>
### X10

## Comparação

### Speedup

# Referências
- Pthreads
	+
	+
	+
- OpenMP
	+
	+
	+
- MPI
	+
	+
	+
- Chapel
	+
	+
	+
- X10
	+
	+
	+
