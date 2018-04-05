[Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2018a) > Trabalhos

# T3: Programação paralela multithread

Disciplina: ELC139 - Programação Paralela <br/>
Aluno: Rhauani Weber Aita Fazul

## Sumário

- [Pthreads](#pthreads)
	- [Questão 1](#questao1)
	- [Questão 2](#questao2)
	- [Questão 3](#questao3)
	- [Questão 4](#questao4)
	- [Questão 5](#questao5)
- [OpenMP]P(#openmp)
	- [Programa utilizado (SeriesPi)](#seriespi)
	- [_OProfile_](#oprofile)
- [Extra](#extra)
- [Referências](#referencias)

<!-- Pthreads -->
## Pthreads

### Questão 1
- Explique como se encontram implementadas as 4 etapas de projeto: particionamento, comunicação, aglomeração, mapeamento (use trechos de código para ilustrar a explicação).

### Questão 2
- Considerando o tempo (em segundos) mostrado na saída do programa, qual foi a aceleração com o uso de threads?

### Questão 3
- A aceleração (speedup) se sustenta para outros tamanhos de vetores, números de threads e repetições? Para responder a essa questão, você terá que realizar diversas execuções, variando o tamanho do problema (tamanho dos vetores e número de repetições) e o número de threads (1, 2, 4, 8..., dependendo do número de núcleos). Cada caso deve ser executado várias vezes, para depois calcular-se um tempo de processamento médio para cada caso. Atenção aos fatores que podem interferir na confiabilidade da medição: uso compartilhado do computador, tempos muito pequenos, etc.

Para realizar as comparações apresentadas a seguir, utilizou-se as seguintes baterias de testes (30 execuções de cada configuração):

|   THREADS   |					TAMANHO DOS VETORES  				    | 		     REPETIÇÕES 		|
| ----------- | ----------------------------------------------------------------------------------  | ----------------------------------------- |
|     1	      |	\[ 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 \] | \[ 10, 50, 100, 500, 1000, 5000, 10000 \] |
|     2	      |	\[ 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 \] | \[ 10, 50, 100, 500, 1000, 5000, 10000 \] |
|     4       |	\[ 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 \] | \[ 10, 50, 100, 500, 1000, 5000, 10000 \] |
|     8       |	\[ 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 \] | \[ 10, 50, 100, 500, 1000, 5000, 10000 \] |
|     16      |	\[ 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 \] | \[ 10, 50, 100, 500, 1000, 5000, 10000 \] |
|     32      |	\[ 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 \] | \[ 10, 50, 100, 500, 1000, 5000, 10000 \] |
|     64      |	\[ 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 \] | \[ 10, 50, 100, 500, 1000, 5000, 10000 \] |
|     128     |	\[ 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 \] | \[ 10, 50, 100, 500, 1000, 5000, 10000 \] |
|     256     |	\[ 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 \] | \[ 10, 50, 100, 500, 1000, 5000, 10000 \] |
|     512     |	\[ 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 \] | \[ 10, 50, 100, 500, 1000, 5000, 10000 \] |
|     1024    |	\[ 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 \] | \[ 10, 50, 100, 500, 1000, 5000, 10000 \] |

### Questão 4
- Elabore um gráfico/tabela de aceleração a partir dos dados obtidos no exercício anterior.

### Questão 5
- Explique as diferenças entre [pthreads_dotprod.c](pthreads_dotprod/pthreads_dotprod.c) e [pthreads_dotprod2.c](pthreads_dotprod/pthreads_dotprod2.c). Com as linhas removidas, o programa está correto? 


<a name="variacao"></a>
### Variaçao de perfil 
	
<a name="paralelizacao"></a>	

<!-- OpenMP -->
## OpenMP
openmp 4.0

<a name="seriespi"></a>
### Programa utilizado (SeriesPi)

### _OProfile_


## Extra

<!-- REFERÊNCIAS -->
<a name="referencias"></a>
## Referências 
- **_ _**
	-
	-
	-
- **_ _**
	-
	-
	-
