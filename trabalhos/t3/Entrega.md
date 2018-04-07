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

Tomando como base os seguintes resultados:

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

<p align="center">![speedupformula](https://latex.codecogs.com/svg.latex?S(n)%3D%5Cfrac%7Bt_s%7D%7Bt_p%7D)</p>

Onde _n_ equivale a quantidade de processadores, _t<sub>s</sub>_ equivale ao tempo de execução serial e _t<sub>p</sub>_ ao tempo de execução paralelo. Logo, considerando que o computador onde os testes foram realizados possui dois núcleos de processamento:

<p align="center">![speeduplinear](https://latex.codecogs.com/svg.latex?S(2)%3D%5Cfrac%7B8837103%7D%7B4420286%7D%3D%7B1.999215209%7D)</p>

Sendo _S(2)_ &cong; _2_, obteve-se um _speedup_ próximo ao ideal (_speedup_ linear), o que demonstra que o programa possui um forte ganho com a paralelização realizada. Ainda nesse contexto, realizou-se uma nova execução do programa em que os resultados obtidos foram:

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

![speedupsuperlinear](https://latex.codecogs.com/svg.latex?S(2)%3D%5Cfrac%7B8989450%7D%7B4421713%7D%3D%7B2.033024305%7D)

Sendo _S(2)_ > _2_, obteve-se um _superlinear speedup_. Uma possível explicação para isso é o efeito da memória cache resultante das diferentes hierarquias de memória do computador.

Portando, neste cenário, em ambos os casos apresentados, conclui-se que o algorítimo de cálculo de produto escalar paralelizado em _pthreads\_dotprod_ teve um ganho de desempenho substâncial quando comparado a execução sequencial.

### Questão 3
- A aceleração (speedup) se sustenta para outros tamanhos de vetores, números de threads e repetições? Para responder a essa questão, você terá que realizar diversas execuções, variando o tamanho do problema (tamanho dos vetores e número de repetições) e o número de threads (1, 2, 4, 8..., dependendo do número de núcleos). Cada caso deve ser executado várias vezes, para depois calcular-se um tempo de processamento médio para cada caso. Atenção aos fatores que podem interferir na confiabilidade da medição: uso compartilhado do computador, tempos muito pequenos, etc.

Para realizar as comparações apresentadas a seguir, utilizou-se as seguintes baterias de testes (30 execuções de cada configuração):

- NUM\_THREADS = 1			

| TAM\_VETS  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| -------------------------------------------------------------------------------------- | 
| 1.000      | 	563		|		   |		      |			 |
| 10.000     |	1371		|		   |		      |			 |
| 100.000    |	7481		|		   |		      |			 |
| 1.000.000  |	51763		|		   |		      |			 |
| 10.000.000 |	448580		|		   |		      |			 |

- NUM\_THREADS = 2		

| TAM\_VETS  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| -------------------------------------------------------------------------------------- | 
| 1.000      | 778		|		   |		      |			 |
| 10.000     | 1698		|		   |		      |			 |
| 100.000    | 9186		|		   |		      |			 |
| 1.000.000  | 72660		|		   |		      |			 |
| 10.000.000 | 453612		|		   |		      |			 |

- NUM\_THREADS = 4	

| TAM\_VETS  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| -------------------------------------------------------------------------------------- | 
| 1.000      | 573		|		   |		      |			 |
| 10.000     | 1587		|		   |		      |			 |
| 100.000    | 8850		|		   |		      |			 |
| 1.000.000  | 80817		|		   |		      |			 |
| 10.000.000 | 792414		|		   |		      |			 |

- NUM\_THREADS = 8			        

| TAM\_VETS  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| -------------------------------------------------------------------------------------- | 
| 1.000      | 			|		   |		      |			 |
| 10.000     |			|		   |		      |			 |
| 100.000    |			|		   |		      |			 |
| 1.000.000  |			|		   |		      |			 |
| 10.000.000 |			|		   |		      |			 |

- NUM\_THREADS = 16	

| TAM\_VETS  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| -------------------------------------------------------------------------------------- | 
| 1.000      | 			|		   |		      |			 |
| 10.000     |			|		   |		      |			 |
| 100.000    |			|		   |		      |			 |
| 1.000.000  |			|		   |		      |			 |
| 10.000.000 |			|		   |		      |			 |

-  NUM\_THREADS = 32	       	                   

| TAM\_VETS  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| -------------------------------------------------------------------------------------- | 
| 1.000      | 			|		   |		      |			 |
| 10.000     |			|		   |		      |			 |
| 100.000    |			|		   |		      |			 |
| 1.000.000  |			|		   |		      |			 |
| 10.000.000 |			|		   |		      |			 |

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
