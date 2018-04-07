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
<p align="justify">- Explique como se encontram implementadas as 4 etapas de projeto: particionamento, comunicação, aglomeração, mapeamento (use trechos de código para ilustrar a explicação).</p>

### Questão 2
<p align="justify">- Considerando o tempo (em segundos) mostrado na saída do programa, qual foi a aceleração com o uso de threads?</p>

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

<p align="center"><img src="https://latex.codecogs.com/svg.latex?S(n)%3D%5Cfrac%7Bt_s%7D%7Bt_p%7D" alt="speedup formula" /></p>

<p align="justify">Onde _n_ equivale a quantidade de processadores, _t<sub>s</sub>_ equivale ao tempo de execução serial e _t<sub>p</sub>_ ao tempo de execução paralelo. Logo, considerando que o computador onde os testes foram realizados possui dois núcleos de processamento:</p>

<p align="center"><img src="https://latex.codecogs.com/svg.latex?S(2)%3D%5Cfrac%7B8837103%7D%7B4420286%7D%3D%7B1.999215209%7D" alt="speedup linear"/></p>

<p align="justify">Sendo _S(2)_ &cong; _2_, obteve-se um _speedup_ próximo ao ideal (_speedup_ linear), o que demonstra que o programa possui um forte ganho com a paralelização realizada. Ainda nesse contexto, realizou-se uma nova execução do programa em que os resultados obtidos foram:</p>

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

<p align="center"><img src="https://latex.codecogs.com/svg.latex?S(2)%3D%5Cfrac%7B8989450%7D%7B4421713%7D%3D%7B2.033024305%7D" alt="speedup superlinear"></p>

<p align="justify">Sendo _S(2)_ > _2_, obteve-se um _superlinear speedup_. Uma possível explicação para isso é o efeito da memória cache resultante das diferentes hierarquias de memória do computador.</p>

<p align="justify">Portando, neste cenário, em ambos os casos apresentados, conclui-se que o algorítimo de cálculo de produto escalar paralelizado em _pthreads\_dotprod_ teve um ganho de desempenho substâncial quando comparado a execução sequencial.</p>

### Questão 3
<p align="justify">- A aceleração (speedup) se sustenta para outros tamanhos de vetores, números de threads e repetições? Para responder a essa questão, você terá que realizar diversas execuções, variando o tamanho do problema (tamanho dos vetores e número de repetições) e o número de threads (1, 2, 4, 8..., dependendo do número de núcleos). Cada caso deve ser executado várias vezes, para depois calcular-se um tempo de processamento médio para cada caso. Atenção aos fatores que podem interferir na confiabilidade da medição: uso compartilhado do computador, tempos muito pequenos, etc.</p>

<p align="justify">Para realizar a análise, o parâmetro referente ao tamanho dos vetores foi utilizado com base no número de threads. Para exemplificar, tomando como exemplo a seguinte execução:</p>

```
	$ ./pthreads_dotprod 1 10000 10
```

Os argumentos são, respectivamente, <_nthreads_>, <_worksize_> e <_repetitions_>. Se fosse utilizado 4 _threads_, o _worksize_ deve ser recalculado:
 
```
	$ ./pthreads_dotprod 4 2500 10
```

<p align="justify">Sendo assim, <i>worksize(thread<sub>i</sub>)</i> = _tam\_vets_ &divide; _nthreads_. Esse cuidado foi tomado em todas baterias de testes realizadas. Abaixo estão ilustrando os valores médios (em segundos) de cada execução, para obtenção desses valores foram realizadas 30 execuções de cada configuração de teste, onde foi realizada uma média aritimética simples dos valores de execução individual, onde: Tempo(s) = &Sigma;<sub>n=1..30</sub> execucao<sub>n</sub> / 30.</p>

- **_nthreads_** = 1

<p align="center">
| tam\_vets  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- | 
| 1.000      | 	0.000663	| 0.004471	   | 0.008478	      | 0.013945	 |
| 10.000     |	0.001450	| 0.008326	   | 0.047678	      |	0.088564         |
| 100.000    |	0.008683	| 0.050750	   | 0.427975	      |	0.841297         |
| 1.000.000  |	0.051537	| 0.449532         | 4.439322         |	8.848498	 |
| 10.000.000 |	0.448649	| 4.424125         | 44.10462	      |	88.10289         |
</p>

- **_nthreads_** = 2

<p align="center">
| tam\_vets  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- | 
| 1.000      | 0.000537		| 0.000885         | 0.004165	      |	0.007521         |
| 10.000     | 0.000996		| 0.004357         | 0.024239         |	0.047905	 |
| 100.000    | 0.004397		| 0.026254	   | 0.214525	      |	0.428951	 |
| 1.000.000  | 0.032828		| 0.232192         | 2.266735	      | 4.556499	 |
| 10.000.000 | 0.222738		| 2.282915         | 23.45902         |	49.11765	 |
</p>

- **_nthreads_** = 4	

<p align="center">
| tam\_vets  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- | 
| 1.000      | 0.000586		| 0.000788	   | 0.003156	      | 0.005268	 |
| 10.000     | 0.000806		| 0.003186	   | 0.019892	      |	0.039089	 |
| 100.000    | 0.003641		| 0.020139	   | 0.193465	      |	0.389723	 |
| 1.000.000  | 0.020849		| 0.201132	   | 1.985106	      | 4.153111	 |
| 10.000.000 | 0.199382		| 2.056176	   | 22.08599	      |	47.47029	 |
</p>

- **_nthreads_** = 8

<p align="center">
| tam\_vets  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- | 
| 1.000      | 0.000860		| 0.000902	   | 0.003259	      | 0.005180	 |
| 10.000     | 0.000968		| 0.003318	   | 0.020204	      | 0.039533	 |
| 100.000    | 0.003443		| 0.020226         | 0.194986	      |	0.388560	 |
| 1.000.000  | 0.020576		| 0.201583         | 1.990362	      | 4.345865	 |
| 10.000.000 | 0.204278		| 1.997682	   | 21.58099	      |	47.04251	 |
</p>

- **_nthreads_** = 16	

<p align="center">
| tam\_vets  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- | 
| 1.000      | 0.001215		| 0.001305	   | 0.003341	      | 0.005331	 |
| 10.000     | 0.001307		| 0.003279	   | 0.020541	      |	0.040552	 |
| 100.000    | 0.003403		| 0.020585	   | 0.195229         |	0.386702	 |
| 1.000.000  | 0.021161		| 0.199856	   | 1.972717	      | 3.948927	 |
| 10.000.000 | 0.201536		| 20.81036	   | 24.31045	      |	49.22075	 |
</p>

-  **_nthreads_** = 32

<p align="center">
| tam\_vets  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- |  
| 1.000      | 0.001876		| 0.001960	   | 0.003343	      | 0.005474	 |
| 10.000     | 0.002094		| 0.003418         | 0.020361	      |	0.040006 	 |
| 100.000    | 0.003479		| 0.020657         | 0.194581	      |	0.386753	 |
| 1.000.000  | 0.020745		| 0.195829	   | 1.947502         |	4.122186	 |
| 10.000.000 | 0.201797		| 1.990973	   | 22.79035	      |	47.98167	 |
</p>

-  **_nthreads_** = 1024

<p align="center">
| TAM\_VETS  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- |  
| 1.000      | -		| -	   	   | -	      	      | -	 	 |
| 10.000     | 0.036666		| 0.034348         | 0.042275	      |	0.059018 	 |
| 100.000    | 0.037224		| 0.041402         | 0.212358	      |	0.404867 	 |
| 1.000.000  | 0.040600		| 0.209328	   | 1.986754         |	3.948843 	 |
| 10.000.000 | 0.213893		| 1.943035	   | 20.17412	      |	44.40787	 |
</p>

### Questão 4
<p align="justify">- Elabore um gráfico/tabela de aceleração a partir dos dados obtidos no exercício anterior.</p>

### Questão 5
<p align="justify">- Explique as diferenças entre [pthreads_dotprod.c](pthreads_dotprod/pthreads_dotprod.c) e [pthreads_dotprod2.c](pthreads_dotprod/pthreads_dotprod2.c). Com as linhas removidas, o programa está correto?</p>


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
