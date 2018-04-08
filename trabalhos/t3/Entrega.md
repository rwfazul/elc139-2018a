[Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2018a) > Trabalhos

# T3: Programação paralela multithread

Disciplina: ELC139 - Programação Paralela <br/>
Aluno: Rhauani Weber Aita Fazul

## Sumário

- [_Pthreads_](#pthreads)
	- [Questão 1](#questão-1)
	- [Questão 2](#questão-2)
	- [Questão 3](#questão-3)
	- [Questão 4](#questão-4)
	- [Questão 5](#questão-5)
- [_OpenMP_](#openmp)
	- [Implementação](#implementação)
	- [Desempenho](#desempenho)
- [Referências](#referencias)

<!-- Pthreads -->
## _Pthreads_

### Questão 1
- Explique como se encontram implementadas as 4 etapas de projeto: particionamento, comunicação, aglomeração, mapeamento (use trechos de código para ilustrar a explicação).

### Questão 2
- Considerando o tempo (em segundos) mostrado na saída do programa, qual foi a aceleração com o uso de _threads_?

Tomando como base os seguintes resultados:

- Execução sequencial:

```
./pthreads_dotprod 1 1000000 2000
10000.000000
1 thread(s), 8837103 usec
```

- Execução com _threads_:

```
./pthreads_dotprod2 1 500000 2000
5000.000000
1 thread(s), 4420286 usec
```
O fator de aceleração (_speedup_) pode ser calculado da seguinte maneira:

<p align="center"><img src="https://latex.codecogs.com/svg.latex?S(p)%3D%5Cfrac%7Bt_s%7D%7Bt_p%7D" alt="speedup formula" /></p>

Onde _p_ equivale a quantidade de processadores, <i>t<sub>s</sub></i> equivale ao tempo de execução serial e <i>t<sub>p</sub></i> ao tempo de execução paralelo. Logo, considerando que o computador onde os testes foram realizados possui dois núcleos de processamento:

<p align="center"><img src="https://latex.codecogs.com/svg.latex?S(2)%3D%5Cfrac%7B8837103%7D%7B4420286%7D%3D%7B1.999215209%7D" alt="speedup linear"/></p>

Sendo _S(2)_ &cong; 2, obteve-se um _speedup_ próximo ao ideal (_speedup_ linear), o que demonstra que o programa alcançou um forte ganho de desempenho com a paralelização realizada. Ainda nesse contexto, realizou-se uma nova execução do programa em que os resultados obtidos foram:

- Execução sequencial:	

```
./pthreads_dotprod 1 1000000 2000
10000.000000
1 thread(s), 8989450 usec

```

- Execução com _threads_:

```
./pthreads_dotprod2 1 500000 2000
5000.000000
1 thread(s), 4421713 usec
```

<p align="center"><img src="https://latex.codecogs.com/svg.latex?S(2)%3D%5Cfrac%7B8989450%7D%7B4421713%7D%3D%7B2.033024305%7D" alt="speedup superlinear"></p>

Sendo _S(2)_ > 2, obteve-se um _superlinear speedup_. Uma possível explicação para isso é o efeito da memória cache resultante das diferentes hierarquias de memória do computador.

Portando, neste cenário, em ambos os casos apresentados, conclui-se que o algorítimo de cálculo de produto escalar paralelizado em  [pthreads_dotprod.c](pthreads_dotprod/pthreads_dotprod.c) teve um ganho de desempenho substâncial quando comparado a sua execução puramente sequencial.

### Questão 3
- A aceleração (_speedup_) se sustenta para outros tamanhos de vetores, números de _threads_ e repetições? Para responder a essa questão, você terá que realizar diversas execuções, variando o tamanho do problema (tamanho dos vetores e número de repetições) e o número de _threads_ (1, 2, 4, 8..., dependendo do número de núcleos). Cada caso deve ser executado várias vezes, para depois calcular-se um tempo de processamento médio para cada caso. Atenção aos fatores que podem interferir na confiabilidade da medição: uso compartilhado do computador, tempos muito pequenos, etc.

Para realizar a análise, o parâmetro referente ao tamanho dos vetores foi utilizado com base no número de _threads_. Para exemplificar, tomando como exemplo a seguinte execução:

```
	$ ./pthreads_dotprod 1 10000 10
```

Os argumentos são, respectivamente, <_nthreads_>, <_worksize_> e <_repetitions_>. Se, por exemplo, fosse utilizado 4 _threads_, o _worksize_ precisaria ser recalculado:
 
```
	$ ./pthreads_dotprod 4 2500 10
```

Sendo assim, _worksize_(_thread_<sub><i>i</i></sub>) = <i>worksize</i><sub>total</sub> &divide; _nthreads_. Esse cuidado foi tomado em todas baterias de testes realizadas. 

Abaixo estão ilustrando os valores médios (em segundos) de cada execução, para obtenção desses valores foram realizadas 30 execuções de cada  configuração de teste, onde foi realizada uma média aritimética simples dos tempos de execução individual, em que: _T(s)_ = (&Sigma;<sub><i>n</i>=1..30</sub> T<sub>n</sub>) &divide; 30. 

O _script_ [calc_time.sh](https://github.com/rwfazul/elc139-2018a/blob/master/trabalhos/t3/pthreads_dotprod/calc_time.sh) foi utilizado para realizar as execuções e calcular as médias de tempo apresentadas a seguir.

- (_Pthreads_) Tempos de execução em segundos com 1 _thread_:

| <i>worksize</i><sub>total</sub>  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- | 
| 1.000      | 	0.000663	| 0.001471	   | 0.008478	      | 0.013945	 |
| 10.000     |	0.001450	| 0.008326	   | 0.047678	      |	0.088564         |
| 100.000    |	0.008683	| 0.050750	   | 0.427975	      |	0.841297         |
| 1.000.000  |	0.051537	| 0.449532         | 4.439322         |	8.848498	 |
| 10.000.000 |	0.448649	| 4.424125         | 44.10462	      |	88.10289         |

- (_Pthreads_) Tempos de execução em segundos com 2 _threads_:

| <i>worksize</i><sub>total</sub>  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- | 
| 1.000      | 0.000537		| 0.000885         | 0.004165	      |	0.007521         |
| 10.000     | 0.000996		| 0.004357         | 0.024239         |	0.047905	 |
| 100.000    | 0.004397		| 0.026254	   | 0.214525	      |	0.428951	 |
| 1.000.000  | 0.032828		| 0.232192         | 2.266735	      | 4.456499	 |
| 10.000.000 | 0.222738		| 2.282915         | 23.45902         |	49.11765	 |

- (_Pthreads_) Tempos de execução em segundos com 4 _threads_:

| <i>worksize</i><sub>total</sub> |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- | 
| 1.000      | 0.000586		| 0.000788	   | 0.003156	      | 0.005268	 |
| 10.000     | 0.000806		| 0.003186	   | 0.019892	      |	0.039089	 |
| 100.000    | 0.003641		| 0.020139	   | 0.193465	      |	0.389723	 |
| 1.000.000  | 0.020849		| 0.201132	   | 1.985106	      | 4.153111	 |
| 10.000.000 | 0.199382		| 2.056176	   | 22.08599	      |	47.47029	 |

-(_Pthreads_) Tempos de execução em segundos com 8 _threads_:

| <i>worksize</i><sub>total</sub>  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- | 
| 1.000      | 0.000860		| 0.000902	   | 0.003259	      | 0.005180	 |
| 10.000     | 0.000968		| 0.003318	   | 0.020204	      | 0.039533	 |
| 100.000    | 0.003443		| 0.020226         | 0.194986	      |	0.388560	 |
| 1.000.000  | 0.020576		| 0.201583         | 1.990362	      | 4.245865	 |
| 10.000.000 | 0.204278		| 1.997682	   | 21.58099	      |	47.04251	 |

- (_Pthreads_) Tempos de execução em segundos com 16 _threads_:

| <i>worksize</i><sub>total</sub>  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- | 
| 1.000      | 0.001215		| 0.001305	   | 0.003341	      | 0.005331	 |
| 10.000     | 0.001307		| 0.003279	   | 0.020541	      |	0.040552	 |
| 100.000    | 0.003403		| 0.020585	   | 0.195229         |	0.386702	 |
| 1.000.000  | 0.021161		| 0.199856	   | 1.972717	      | 3.948927	 |
| 10.000.000 | 0.201536		| 1.990246	   | 24.31045	      |	47.22075	 |

- (_Pthreads_) Tempos de execução em segundos com 32 _threads_:

| <i>worksize</i><sub>total</sub> |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- |  
| 1.000      | 0.001876		| 0.001960	   | 0.003343	      | 0.005474	 |
| 10.000     | 0.002094		| 0.003418         | 0.020361	      |	0.040006 	 |
| 100.000    | 0.003479		| 0.020657         | 0.194581	      |	0.386753	 |
| 1.000.000  | 0.020745		| 0.195829	   | 1.947502         |	4.092186	 |
| 10.000.000 | 0.201797		| 1.990973	   | 22.79035	      |	47.98167	 |

- (_Pthreads_) Tempos de execução em segundos com 1024 _threads_:

| <i>worksize</i><sub>total</sub> |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- |  
| 1.000      | -		| -	   	   | -      	      | -	 	 |
| 10.000     | 0.036666		| 0.034348         | 0.042275	      |	0.059018 	 |
| 100.000    | 0.037224		| 0.041402         | 0.212358	      |	0.404867 	 |
| 1.000.000  | 0.040600		| 0.209328	   | 1.986754         |	4.105810 	 |
| 10.000.000 | 0.213893		| 1.943035	   | 21.17412	      |	46.40787	 |

Constatou-se que, para poucas repetições e _worksizes_ muito pequenos, o _overhead_ gerado pelas _threads_, embora baixo, pode fazer com que o _speedup_ ganhado pela parelização não se sustente. Porém, a medida que o valor de pelo menos um destes parâmetros aumente, o _speedup_ será mantido independente do número de _threads_.

Isso se comprova pois, mesmo utilizando um valor de _threads_ muito mais alto que o recomendado (número de unidades de processamento/_cores_), como nos testes em que _nthreads_ = 1024, o _overhead_ de criação e gerenciamento dessas unidades é diluído no tempo. Logo, a média de tempos de execução neste cenário acaba por ser inferior do que os valores obtidos na execução puramente sequencial do programa.

Neste sentido, observou-se alguns resultados que não eram esperados. Tendo em vista que o computador onde os testes foram realizados possui um processador _Dual-core_ (ou seja, dois núcleos), acreditava-se que o maior desempenho seria obtido nos cenários onde foram utilizadas 2 _threads_ ou, no máximo, 4 _threads_ (devido a tecnologia _Hyper-Threading_, que faz com que cada núcleo do processador execute mais de uma _thread_ "simultaneamente"). Porém, houve testes em que obteve-se um maior desempenho mesmo com _nthreads_ > 4.

Ainda que _user space threads_ sejam _lightweight_, esperava-se que o _overhead_ gerado em alguns casos (ex. _nthreads_ = 32 ou 1024) fosse prejudicar o desempenho do programa. Acredita-se que [_pthreads_dotprod.c_](pthreads_dotprod/pthreads_dotprod.c) implemente uma lógica que, indiretamente, tenha favorecido a parelização com um número de _threads_ maior, em outras palavras, o cálculo realizado pelo programa faz com que o _overhead_ gerado (ex. o número maior de trocas de contexto/estado gerado pelo uso de mais _threads_), não seja significativo a ponto de afetar os tempos obtidos.

Com base nesses resultados, os seguintes gráficos podem ser gerados para facilitar a visualização:

- Desempenho do programa variando o número de _threads_ e repetiçoes com <i>worksize</i><sub>total</sub> de 1.000:

<p align="center">
  <img src="https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t3/pthreads/charts/pthreads_dotprod-worksize1000.png" alt="Tempo gasto pelo programa com diferentes configurações." width="60%"/>
</p>

- Desempenho do programa variando o número de _threads_ e repetiçoes com <i>worksize</i><sub>total</sub> de 10.000:

<p align="center">
  <img src="https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t3/pthreads/charts/pthreads/pthreads_dotprod-worksize10000-1.png" alt="Tempo gasto pelo programa com diferentes configurações." width="60%"/>
</p>
<p align="center">
  <img src="https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t3/pthreads/charts/pthreads/pthreads_dotprod-worksize10000-2.png" alt="Tempo gasto pelo programa com diferentes configurações." width="60%"/>
</p>

- Desempenho do programa variando o número de _threads_ e repetiçoes com <i>worksize</i><sub>total</sub> de 100.000:

<p align="center">
  <img src="https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t3/pthreads/charts/pthreads/pthreads_dotprod-worksize100000-1.png" alt="Tempo gasto pelo programa com diferentes configurações." width="60%"/>
</p>
<p align="center">
  <img src="https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t3/pthreads/charts/pthreads/pthreads_dotprod-worksize100000-2.png" alt="Tempo gasto pelo programa com diferentes configurações." width="60%"/>
</p>

- Desempenho do programa variando o número de _threads_ e repetiçoes <i>worksize</i><sub>total</sub> de 1.000.000:

<p align="center">
  <img src="https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t3/pthreads/charts/pthreads/pthreads_dotprod-worksize1000000-1.png" alt="Tempo gasto pelo programa com diferentes configurações." width="60%"/>
</p>
<p align="center">
  <img src="https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t3/pthreads/charts/pthreads/pthreads_dotprod-worksize1000000-2.png" alt="Tempo gasto pelo programa com diferentes configurações." width="60%"/>
</p>

- Desempenho do programa variando o número de _threads_ e repetiçoes com <i>worksize</i><sub>total</sub> de 10.000.000:

<p align="center">
  <img src="https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t3/pthreads/charts/pthreads/pthreads_dotprod-worksize10000000-1.png" alt="Tempo gasto pelo programa com diferentes configurações." width="60%"/>
</p>
<p align="center">
  <img src="https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t3/pthreads/charts/pthreads/pthreads_dotprod-worksize10000000-2.png" alt="Tempo gasto pelo programa com diferentes configurações." width="60%"/>
</p>

### Questão 4
- Elabore um gráfico/tabela de aceleração a partir dos dados obtidos no exercício anterior.

As tabelas a seguir representam o _speedup_ alcançado com a parelização. Para cada um dos cenários, utilizou-se as médias de tempo apresentadas anteriormente. Todos os cálculos utilizam como base os tempos da execução sequencial (<i>t<sub>s</sub></i>) já apresentados nas tabelas anteriores.

- (_Pthreads_) _Speedup_ obtido com 2 _threads_.

| <i>worksize</i><sub>total</sub>  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- | 
| 1.000      | 1.234636872	| 1.662146893	   | 2.035534214      |	1.854141736 	 |
| 10.000     | 1.455823293	| 1.910947900	   | 1.966995338      |	1.848742302	 |
| 100.000    | 1.974755515	| 1.933038775	   | 1.994988929      |	1.961289285 	 |
| 1.000.000  | 1.569909833	| 1.936035695	   | 1.958465370      |	1.985526755      |
| 10.000.000 | 2.014245436	| 1.937928044	   | 1.880070864      |	1.793711426      |

- (_Pthreads_) _Speedup_ obtido com 4 _threads_.

| <i>worksize</i><sub>total</sub>  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- | 
| 1.000      | 1.131399317	| 1.866751269	   | 2.686311787      | 2.647114655      |
| 10.000     | 1.799007444	| 2.613308223	   | 2.396842952      |	2.265701348      |	
| 100.000    | 2.384784400	| 2.519986097	   | 2.212157238      |	2.158705029      |	
| 1.000.000  | 2.471917118	| 2.235009844	   | 2.236314837      |	2.130571035      |	
| 10.000.000 | 2.250198112	| 2.151627584	   | 1.996950103      |	1.855958537      |

- (_Pthreads_) _Speedup_ obtido com 8 _threads_.

| <i>worksize</i><sub>total</sub>  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- | 
| 1.000      | 0.770930233	| 1.630820399	   | 2.601411476      | 2.692084942	 |
| 10.000     | 1.497933884	| 2.509342978	   | 2.359829737      |	2.240254977	 |	
| 100.000    | 2.521928551	| 2.509146643	   | 2.194901172      |	2.165166255	 |
| 1.000.000  | 2.504714230	| 2.230009475	   | 2.230409343      |	2.084027165	 |
| 10.000.000 | 2.196266852	| 2.214629255	   | 2.043679182      |	1.872835654	 |

- (_Pthreads_) _Speedup_ obtido com 16 _threads_.

| <i>worksize</i><sub>total</sub> |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- | 
| 1.000      | 0.545679012	| 1.127203065	   | 2.537563604      | 2.615831926      |
| 10.000     | 1.109410865	| 2.539188777	   | 2.321113870      |	2.183961334	 |
| 100.000    | 2.551572142	| 2.465387418	   | 2.192169196      |	2.175569301	 |
| 1.000.000  | 2.435470913	| 2.249279481	   | 2.250359276      |	2.240734762	 |
| 10.000.000 | 2.226148182	| 2.222903489	   | 1.814224747      |	1.865766427	 |

- (_Pthreads_) _Speedup_ obtido com 32 _threads_.

| <i>worksize</i><sub>total</sub>  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- | 
| 1.000      | 0.353411514	| 0.750510204	   | 2.536045468      | 2.547497260      |
| 10.000     | 0.692454632	| 2.435927443	   | 2.341633515      |	2.213767935      |
| 100.000    | 2.495832136	| 2.456794307	   | 2.199469630      |	2.175282415      |	
| 1.000.000  | 2.484309472	| 2.295533348	   | 2.279495477      |	2.162291255      |
| 10.000.000 | 2.223268929	| 2.222091912	   | 1.935232236      |	1.836178065      |

- (_Pthreads_) _Speedup_ obtido com 1024 _threads_.

| <i>worksize</i><sub>total</sub> |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- |  
| 1.000      | -		| -		   | -		      | -		 |
| 10.000     | 0.039546174	| 0.242401304	   | 1.127806032      | 1.500626927      |
| 100.000    | 0.233263486	| 1.225786194	   | 2.015346726      |	2.077958935      |
| 1.000.000  | 1.269384236	| 2.147500573	   | 2.234459827      |	2.155116286      |	
| 10.000.000 | 2.097539424	| 2.276914724	   | 2.082949374      |	1.898447181      |

A partir dos _speedups_ apresentados com as diferentes configurações dos parâmetros de execução, pode-se também calcular a efiência da seguinte maneira:

<p align="center"><img src="https://latex.codecogs.com/svg.latex?E%3D%5Cfrac%7BS(p)%7D%7Bp%7D" alt="efficiency formula" /></p>

A eficiência é uma métrica que fornece uma noção básica da utilização dos recursos do sistema. Seu valor varia tipicamente entre 0 e 1, onde programas com aceleração linear e programas executados em um único processador têm uma eficiência de 1. Como no cenário de testes realizado obteve-se diversos _speedups_ superlineares, o valor de eficiência encontrado nesses casos seria superior aos valores comuns.

### Questão 5
- Explique as diferenças entre [pthreads_dotprod.c](pthreads_dotprod/pthreads_dotprod.c) e [pthreads_dotprod2.c](pthreads_dotprod/pthreads_dotprod2.c). Com as linhas removidas, o programa está correto?

A única diferença entre os programas é que [pthreads_dotprod2.c](pthreads_dotprod/pthreads_dotprod2.c) faz uso de _locks_ (_mutexes_), para controle de concorrência (prevenir _race conditions_) envolvendo a varíavel da estrutura compartilhada _dotdata_, que recebe os resultados das somas parciais do cálculo do produto escalar realizadas pelas _threads_.

```
   pthread_mutex_lock (&mutexsum);
   dotdata.c += mysum;
   pthread_mutex_unlock (&mutexsum);
```

Neste caso, o uso de _mutexsum_ irá garantir um acesso serializado para a atualização de valores em _dotdata.c_. Ou seja, se uma _thread_ _t1_ estiver nesta seção crítica, a entrada de outras _threads_ só será permitada após _t1_ liberar o acesso. Como o acesso a varíavel da _struct_ _dotdata_ é compartilhada entre todas as _threads_, o uso do _mutex_ evita _race conditions_. 

Tendo em vista que na maioria das CPUs modernas a operação de leitura e escrita em objetos alinhados seja atômica, é interessante ressaltar que dificilmente a instrução '_dotdata.c += mysum_' causaria algum problema de sincronização. De todo modo, arquiteturas diferentes têm regras diferentes, logo é mais seguro incluir o _mutex_ para garantir a correteza do programa, independente da ordem de escalonamento/intercalação das _threads_.
 
<!-- OpenMP -->
## _OpenMP_

### Implementação
- Implemente um programa equivalente a [pthreads_dotprod.c](pthreads_dotprod/pthreads_dotprod.c) usando _OpenMP_. 

O programa [openmp_dotprod.c](openmp/openmp_dotprod.c) foi paralelizado com _OpenMP_. Ele pode ser compilado utilizando o arquivo [_Makefile_](openmp/Makefile.sh) com:

```
	$ make
```

Ou utilizando diretamente o comando:

```
	$ gcc -fopenmp -o openmp_dotprod openmp_dotprod.c
```

Os argumentos para execução do programa são os mesmos utilizados na versão com _Pthreads_. Logo:


```
	$ ./openmp_dotprod <nthreads> <worksize> <repetitions>
```

Como a cláusula _num\_threads_ foi definida no próprio programa, não é necessário setar a variável ambiente _OMP\_NUM\_THREADS_ para a execução.

### Desempenho
- Avalie o desempenho do programa em OpenMP, usando os mesmos dados/argumentos do programa com threads POSIX. 

Os testes foram realizados com _GNU Compiler Collection_ (GCC) versão 5.4.0, que possui a versão 4.0 da API _OpenMp_. O _script_ [calc_time.sh](https://github.com/rwfazul/elc139-2018a/blob/master/trabalhos/t3/openmp/calc_time.sh) foi utilizado para realizar as execuções e calcular as médias de tempo apresentadas a seguir.

- (_OpenMP_) Tempos de execução em segundos com 1 _thread_:

| <i>worksize</i><sub>total</sub>  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- | 
| 1.000      | 	0.000000	| 0.000000	   | 0.000000	      | 0.000000	 |
| 10.000     |	0.000000	| 0.000000	   | 0.000000	      |	0.000000         |
| 100.000    |	0.000000	| 0.000000	   | 0.000000	      |	0.000000         |
| 1.000.000  |	0.000000	| 0.000000         | 0.000000         |	0.000000	 |
| 10.000.000 |	0.000000	| 0.000000         | 0.000000	      |	0.000000         |

- (_OpenMP_) Tempos de execução em segundos com 2 _threads_:

| <i>worksize</i><sub>total</sub>  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- | 
| 1.000      | 	0.000000	| 0.000000	   | 0.000000	      | 0.000000	 |
| 10.000     |	0.000000	| 0.000000	   | 0.000000	      |	0.000000         |
| 100.000    |	0.000000	| 0.000000	   | 0.000000	      |	0.000000         |
| 1.000.000  |	0.000000	| 0.000000         | 0.000000         |	0.000000	 |
| 10.000.000 |	0.000000	| 0.000000         | 0.000000	      |	0.000000         |

- (_OpenMP_) Tempos de execução em segundos com 4 _threads_:

| <i>worksize</i><sub>total</sub> |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- | 
| 1.000      | 	0.000000	| 0.000000	   | 0.000000	      | 0.000000	 |
| 10.000     |	0.000000	| 0.000000	   | 0.000000	      |	0.000000         |
| 100.000    |	0.000000	| 0.000000	   | 0.000000	      |	0.000000         |
| 1.000.000  |	0.000000	| 0.000000         | 0.000000         |	0.000000	 |
| 10.000.000 |	0.000000	| 0.000000         | 0.000000	      |	0.000000         |

- (_OpenMP_) Tempos de execução em segundos com 8 _threads_:

| <i>worksize</i><sub>total</sub>  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- | 
| 1.000      | 	0.000000	| 0.000000	   | 0.000000	      | 0.000000	 |
| 10.000     |	0.000000	| 0.000000	   | 0.000000	      |	0.000000         |
| 100.000    |	0.000000	| 0.000000	   | 0.000000	      |	0.000000         |
| 1.000.000  |	0.000000	| 0.000000         | 0.000000         |	0.000000	 |
| 10.000.000 |	0.000000	| 0.000000         | 0.000000	      |	0.000000         |

- (_OpenMP_) Tempos de execução em segundos com 16 _threads_:

| <i>worksize</i><sub>total</sub>  |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- | 
| 1.000      | 	0.000000	| 0.000000	   | 0.000000	      | 0.000000	 |
| 10.000     |	0.000000	| 0.000000	   | 0.000000	      |	0.000000         |
| 100.000    |	0.000000	| 0.000000	   | 0.000000	      |	0.000000         |
| 1.000.000  |	0.000000	| 0.000000         | 0.000000         |	0.000000	 |
| 10.000.000 |	0.000000	| 0.000000         | 0.000000	      |	0.000000         |

- (_OpenMP_) Tempos de execução em segundos com 32 _threads_:

| <i>worksize</i><sub>total</sub> |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- |  
| 1.000      | 	0.000000	| 0.000000	   | 0.000000	      | 0.000000	 |
| 10.000     |	0.000000	| 0.000000	   | 0.000000	      |	0.000000         |
| 100.000    |	0.000000	| 0.000000	   | 0.000000	      |	0.000000         |
| 1.000.000  |	0.000000	| 0.000000         | 0.000000         |	0.000000	 |
| 10.000.000 |	0.000000	| 0.000000         | 0.000000	      |	0.000000         |

- (_OpenMP_) Tempos de execução em segundos com 1024 _threads_:

| <i>worksize</i><sub>total</sub> |  10 repetições   |  100 repetições  | 1.000 repetições | 2.000 repetições |
| ---------- | ---------------- | ---------------- | ---------------- | ---------------- |  
| 1.000      | 	-		| -		   | -		      | -		 |
| 10.000     |	0.000000	| 0.000000	   | 0.000000	      |	0.000000         |
| 100.000    |	0.000000	| 0.000000	   | 0.000000	      |	0.000000         |
| 1.000.000  |	0.000000	| 0.000000         | 0.000000         |	0.000000	 |
| 10.000.000 |	0.000000	| 0.000000         | 0.000000	      |	0.000000         |

Com base nos resultados apresentados podemos realizar uma comparação entre as execuções utilizando _Pthreads_ e as execuções usando _OpenMP_. Sendo assim, os seguintes gráficos foram gerados:

<!-- REFERÊNCIAS -->
<a name="referencias"></a>
## Referências 
- Cenapad. <i>Introdução ao OpenMP</i>. https://goo.gl/cC4nCm
- CSE Department. <i>Parallel Programming with OpenMP</i>. https://goo.gl/zeyntk
- FCUP. <i>Introdução ao OpenMP</i>. https://goo.gl/E3N6m4
- GCC. <i>GNU libgomp</i>. https://goo.gl/HPTK4q
- GCC. <i>OpenMP</i>. https://goo.gl/YHjuhM
- GCC. <i>Runtime Library Routines</i>. https://goo.gl/NjUn8Y
- IBM. <i>Learning the OpenMP framework with GCC</i>. https://goo.gl/vSXAX9
- IBM. <i>#pragma omp critical</i>. https://goo.gl/cJtH68
- IBM. <i>Using OpenMP directives</i>. https://goo.gl/pt8d9p
- Intel. <i>Getting Started with OpenMP*</i>. https://goo.gl/Eb83FP
- Intek. <i>Threading Models for High-Performance Computing: Pthreads or OpenMP?</i>. https://goo.gl/8H8KKN
- LLNL. <i>OpenMP</i>. https://goo.gl/Euq38F
- LLNL. <i>POSIX Threads Programming</i>. https://goo.gl/AAWKW4
- OpenMP. <i>A "Hands-on" Introduction to OpenMP*</i>. https://goo.gl/55LPSF
- OpenMP. <i>OpenMP Application Program Interface</i>. https://goo.gl/8M7uD4
- TACC. <i>Introduction to Parallel Computing</i>. https://goo.gl/6iGQhX
- TACC. <i>OpenMP topic: Loop parallelism</i>. https://goo.gl/VKxEH9
- TACC. <i>OpenMP topic: Reductions</i>. https://goo.gl/8X5pdH
- UMW. <i>OpenMP Scheduling</i>. https://goo.gl/qhiRDD
- Wikipédia. <i>Lock (computer science)</i>. https://goo.gl/aq94RA
- Wikipédia. <i>Mutual exclusion</i>. https://goo.gl/7ZR6hs
- Wikipédia. <i>Speedup</i>. https://goo.gl/V18LPb
