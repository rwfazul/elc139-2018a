[Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2018a) > Trabalhos

# T3: Programação paralela multithread

Disciplina: ELC139 - Programação Paralela <br/>
Aluno: Rhauani Weber Aita Fazul

## Sumário

- [Pthreads](#pthreads)
	- [Questão 1](#questao-1)
	- [Questão 2](#questao-2)
	- [Questão 3](#questao-3)
	- [Questão 4](#questao-4)
	- [Questão 5](#questao-5)
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

Considerando os seguintes resultados:
	- Execução sequencial:
```
./pthreads_dotprod 1 1000000 2000
10000.000000
1 thread(s), 8837103 usec
```
	- Execução com threads:
```
./pthreads_dotprod2 1 500000 2000
5000.000000
1 thread(s), 4420286 usec
```
O fator de aceleração (_speedup_) pode ser calculado da seguinte maneira:
![\Large S(n)=\frac{tempo de execução serial}{tempo de execução paralelo}\](https://latex.codecogs.com/svg.latex?x%3D%5Cfrac%7B-b%5Cpm%5Csqrt%7Bb%5E2-4ac%7D%7D%7B2a%7D)

Considerando que o computador onde os testes foram realizados possui dois núcleos de processamento:

![\Large S(2)=\frac{8837103}{4420286}=\1.999215209\](https://latex.codecogs.com/svg.latex?x%3D%5Cfrac%7B-b%5Cpm%5Csqrt%7Bb%5E2-4ac%7D%7D%7B2a%7D)

Sendo S(2) &cong; 2, obteve-se um _speedup_ próximo ao ideal (_speedup_ linear). E, considerando os resultados obtidos em outra execução do programa, conforme apresentado abaixo.
	- Execução sequencial:
```
./pthreads_dotprod 1 1000000 2000
10000.000000
1 thread(s), 8989450 usec

```
	- Execução com threads:
```
./pthreads_dotprod2 1 500000 2000
5000.000000
1 thread(s), 4421713 usec
```

![\Large S(2)=\frac{8989450}{4421713}=\2.033024305\](https://latex.codecogs.com/svg.latex?x%3D%5Cfrac%7B-b%5Cpm%5Csqrt%7Bb%5E2-4ac%7D%7D%7B2a%7D)

Tendo em vista que S(2) > 2, obteve-se um Obteve-se um _superlinear speedup_. Uma possível explicação para isso é o efeito da memória cache resultante das diferentes hierarquias de memória do computador.

Portando, em ambos os cenários apresentados, conclui-se que o algorítimo de cálculo de produto escalar paralelizado em _pthreads\_dotprod_ teve um ganho de desempenho substâncial quando comparado a uma execução sequencial.

### Questão 3
- A aceleração (speedup) se sustenta para outros tamanhos de vetores, números de threads e repetições? Para responder a essa questão, você terá que realizar diversas execuções, variando o tamanho do problema (tamanho dos vetores e número de repetições) e o número de threads (1, 2, 4, 8..., dependendo do número de núcleos). Cada caso deve ser executado várias vezes, para depois calcular-se um tempo de processamento médio para cada caso. Atenção aos fatores que podem interferir na confiabilidade da medição: uso compartilhado do computador, tempos muito pequenos, etc.

Para realizar as comparações apresentadas a seguir, utilizou-se as seguintes baterias de testes (30 execuções de cada configuração):

|   THREADS   |					TAMANHO DOS VETORES  				    |	     REPETIÇÕES 	  |
| ----------- | ----------------------------------------------------------------------------------  | --------------------------- |
|     1	      |	\[ 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 \] | \[ 100, 1000, 2000, 3000 \] |
|     2	      |	\[ 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 \] | \[ 100, 1000, 2000, 3000 \] |
|     4       |	\[ 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 \] | \[ 100, 1000, 2000, 3000 \] |
|     8       |	\[ 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 \] | \[ 100, 1000, 2000, 3000 \] |
|     16      |	\[ 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 \] | \[ 100, 1000, 2000, 3000 \] |
|     32      |	\[ 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 \] | \[ 100, 1000, 2000, 3000 \] |
|     64      |	\[ 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 \] | \[ 100, 1000, 2000, 3000 \] |
|     128     |	\[ 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 \] | \[ 100, 1000, 2000, 3000 \] |
|     256     |	\[ 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 \] | \[ 100, 1000, 2000, 3000 \] |
|     512     |	\[ 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 \] | \[ 100, 1000, 2000, 3000 \] |
|     1024    |	\[ 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 \] | \[ 100, 1000, 2000, 3000 \] |

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
