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

O programa [mpi_dotprod.c](mpi/mpi_dotprod.c) utiliza MPI para realizar o cálculo do produto escalar. Uma das mudanças realizadas em comparação ao código do trabalho [t3](../t3) diz respeito a forma com que os dois vetores ('a' e 'b') são alocados.

O tamanho dos vetores nos programas [openmp_dotprod.c](../t3/openmp/openmp_dotprod.c) e [pthreads_dotprod.c](../t3/pthreads_dotprod/pthreads_dotprod.c) é relacionado ao número de _threads_ utilizadas, seguindo a lógica:

``` c
   dotdata.a = (double *) malloc(wsize * nthreads * sizeof(double));
   dotdata.b = (double *) malloc(wsize * nthreads * sizeof(double));
```

Desta forma é possível realizar uma divisão de carga entre as _threads_, de modo que cada _thread_ atue em seu intervalo específico (baseado em seu identificador). Já no programa com MPI isto não é necessário, pois tendo em vista que cada processo tem suas próprias copias de cada variável do programa, os vetores podem ser alocados baseados apenas no _worksize_ do problema (cada processo atua da posição 0 à N-1 de cada vetor), como pode ser visto a seguir.

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

Os argumentos para execução do programa são o _worksize_ do problema e a quantidade de repetições a serem realizadas. Logo, para executar pode-se utilizar um dos seguintes comandos:

``` bash
	$ mpiexec -np <numero de processos> mpi_dotprod <worksize> <repetitions> 
	// OU
	$ mpirun -np <numero de processos> mpi_dotprod <worksize> <repetitions>
```

Um exemplo de saída pode ser visto a seguir:


``` 
	$ mpiexec -np 5 mpi_dotprod 250000 2000
	dot product: 10000.000000
	the result is correct
	number of processes: 5, wall time: 4540971 usec, wall time MPI: 4540962.934494 usec
```

A linha que informa a situação do resultado é apenas uma verificação extra feita pela função _check\_result()_. As duas informações relacionadas ao tempo são, respectivamente, obtidas por meio das funções _gettimeofday()_, pertencente a biblioteca _sys/time.h_, e _MPI\_Wtime()_,  que foi projetada para ser um _elapsed/wall clock_ de alta resolução, pertencente as rotinas de gerenciamento de ambiente do MPI. Perceba que seus resultados são similares.

### Desempenho
+ Avalie o desempenho do programa conforme as instruções do trabalho [t3](../t3), mas usando processos ao invés de threads.

A análise foi realizada com base nos tempos obtidos 


<!-- PARTE 2 -->
## Parte 2

### _Pipeline_ de mensagens com MPI
+ Implemente um programa MPI que transporte uma mensagem em um _pipeline_ formado por processos de 0 a NP-1 (processo 0 envia para 1, processo 1 envia para 2, ..., processo NP-1 mostra o resultado). A mensagem é um número inteiro que é incrementado antes de ser passado adiante.

O programa [mpi_pipeline.c](mpi/mpi_pipeline.c) implementa a lógica deste _pipeline_. A execução é dada pelo comando:

``` bash
	$ mpiexec -np <numero de processos no pipeline> mpi_pipeline
```

A saída do programa varia em função da definição da macro _PRINT\_TRACE_. Caso esta macro não esteja definida o _output_, será o seguinte:

```
	$ mpiexec -np 5 mpi_pipeline
	*** Pipeline de 5 processos (msg inicia em 0) ***
	
	[FIM] Processo 4 obteve o resultado 3
```

Caso contrário, toda a comunicação será detalhada:

```
	$ mpiexec -np 5 mpi_pipeline

	*** Pipeline de 5 processos (msg inicia em 0) ***

	-- Verbose mode (PRINT_TRACE esta definido) --
	
	Processo [0] envia mensagem para Processo [1]. Msg = 0
		-> Processo [1] recebe mensagem do Processo [0]. Msg = 0
	Processo [1] envia mensagem para Processo [2]. Msg = 1
		-> Processo [2] recebe mensagem do Processo [1]. Msg = 1
	Processo [2] envia mensagem para Processo [3]. Msg = 2
		-> Processo [3] recebe mensagem do Processo [2]. Msg = 2
	Processo [3] envia mensagem para Processo [4]. Msg = 3
		-> Processo [4] recebe mensagem do Processo [3]. Msg = 3
	[FIM] Processo 4 obteve o resultado 3
```

Perceba que as impressões do _trace_ podem vir a ocorrerem em ordem invertida. A especificação do MPI não determina como a saída padrão de diferentes processos devem ser coletadas para serem exibidas no processo inicial da execução (o console). Quando múltiplos processos tentam realizar suas impressões o _output_ pode acabar ficando misturado de uma forma não especificada. Isto não ocorre por erros na comunicacao entre os processos e sim pelo fato de que multiplos processadores podem realizar seus respectivos _outputs_ no console ao mesmo tempo e, um atraso na _pool_ pode vir a inverter a ordem das impressões. Ainda assim, na maioria das vezes, a saída obtida será similiar ao _trace_ apresentado anteriormente, estando de acordo com o esperado.

<!-- PARTE 3 -->
## Parte 3

### Primeiro programa
+ O programa [mpi_errado1.c](examples/mpi_errado1.c) deveria realizar a troca de mensagens entre 2 processos, mas ele não funciona como esperado. Identifique o erro e corrija-o.

O erro está relacionado aos identificadores das menssagens (conhecidos como _tags_) utilizados nas chamadas das rotinas _MPI\_Send()_ e _MPI\_Recv_). A correção pode ser vista no programa [mpi_certo1.c](mpi/mpi-certo1.c). 

### Segundo programa
+ O programa [mpi_errado2.c](examples/mpi_errado2.c) deveria realizar a troca de mensagens entre 2 processos, mas também não funciona como esperado. Identifique o erro e corrija-o.

A rotina _MPI\_Finalize()_ deve ser chamada em todo programa MPI, ou seja, todos os procesos devem chamá-la antes de seu término. O programa [mpi_certo2.c](mpi/mpi-certo2.c) implementa esta correção.

<!-- REFERÊNCIAS -->
## Referências 
