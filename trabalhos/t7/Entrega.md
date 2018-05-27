[Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2018a) > Trabalhos

# T7: Primeiros passos com MPI

Disciplina: ELC139 - Programação Paralela <br/>
Aluno: Rhauani Weber Aita Fazul

## Sumário

- [Parte 1](#parte-1)
	- [Produto escalar com MPI](#produto-escalar-com-mpi)
	- [Desempenho](#desempenho)
- [Parte 2](#parte-2)
	- [_Pipeline_ de mensagens com MPI](#pipeline-de-mensagens-com-mpi)
- [Parte 3](#parte-3)
	- [Primeiro progama](#primeiro-programa)
	- [Segundo programa](#segundo-programa)
- [Referências](#referências)

<!-- PARTE 1 -->
## Parte 1

### Produto escalar com MPI
+ Implemente uma versão em MPI do programa do trabalho [t3](../t3), que calcula repetidas vezes o produto escalar entre 2 vetores. Os argumentos do programa devem ser: o número de elementos para cada processo e o número de repetições.

O programa [mpi_dotprod.c](mpi/mpi_dotprod.c) utiliza MPI para realizar o cálculo do produto escalar. Uma das mudanças realizadas em comparação ao código do trabalho [t3](../t3) diz respeito a forma com que os vetores ('a' e 'b') são alocados.

O tamanho dos vetores nos programas [openmp_dotprod.c](https://github.com/rwfazul/elc139-2018a/blob/master/trabalhos/t3/openmp/openmp_dotprod.c) e [pthreads_dotprod.c](https://github.com/rwfazul/elc139-2018a/blob/master/trabalhos/t3/pthreads_dotprod/pthreads_dotprod.c) é relacionado ao número de _threads_ utilizadas, seguindo a lógica:

``` c
   dotdata.a = (double *) malloc(wsize * nthreads * sizeof(double));
   dotdata.b = (double *) malloc(wsize * nthreads * sizeof(double));
```

Desta forma é possível realizar uma divisão de carga entre as _threads_, de modo que cada uma atue em seu intervalo específico baseado em seu identificador. Já no programa com MPI isto não é necessário, pois tendo em vista que cada processo tem suas proprias copias de cada variavel do programa, os vetores podem ser alocados baseados apenas no _worksize_ do problema (cada processo atua da posição 0 à N-1 de cada vetor), como pode ser visto a seguir.

``` c
    dotdata.a = (double*) malloc(wsize * sizeof(double));
    dotdata.b = (double*) malloc(wsize * sizeof(double));
```

Com isto em mente, o programa [mpi_dotprod.c](mpi/mpi_dotprod.c) foi implementado. Ele pode ser compilado utilizando o arquivo [_Makefile_](mpi/Makefile) com:

``` make
	$ make
```

Ou utilizando diretamente o comando:

``` bash
	$ mpicc -o mpi_dotprod mpi_dotprod.c -Wall
```

Os argumentos para execução do programa são o _worksize_ e a quantidade de repetições. Logo, para executar pode-se usar um dos seguintes comandos:


``` bash
	$ mpiexec -np <numero de processos> mpi_dotprod <worksize> <repetitions> 
	// OU
	$ mpirun -np <numero de processos> mpi_dotprod <worksize> <repetitions>
```

Um exemplo de saída pode ser visto a seguir:


``` bash
	$ mpiexec -np 5 mpi_dotprod 250000 2000
	dot product: 10000.000000
	the result is correct
	number of processes: 5, wall time: 4540971 usec, wall time MPI: 4540962.934494 usec
```

### Desempenho
+ Avalie o desempenho do programa conforme as instruções do trabalho [t3](../t3), mas usando processos ao invés de threads.

A análise foi realizada com base nos tempos obtidos 


<!-- PARTE 2 -->
## Parte 2

### _Pipeline_ de mensagens com MPI
+ Implemente um programa MPI que transporte uma mensagem em um _pipeline_ formado por processos de 0 a NP-1 (processo 0 envia para 1, processo 1 envia para 2, ..., processo NP-1 mostra o resultado). A mensagem é um número inteiro que é incrementado antes de ser passado adiante.


<!-- PARTE 3 -->
## Parte 3

### Primeiro programa
+ O programa [mpi_errado1.c](mpi_errado1.c) deveria realizar a troca de mensagens entre 2 processos, mas ele não funciona como esperado. Identifique o erro e corrija-o.

### Segundo programa
+ O programa [mpi_errado2.c](mpi_errado2.c) deveria realizar a troca de mensagens entre 2 processos, mas também não funciona como esperado. Identifique o erro e corrija-o.



<!-- REFERÊNCIAS -->
## Referências 
