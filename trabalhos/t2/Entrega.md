[Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2018a) > Trabalhos

# T2: Experiências com Profilers

Disciplina: ELC139 - Programação Paralela <br/>
Aluno: Rhauani Weber Aita Fazul

## Sumário

- [Parte 1](#parte-1)
	- [Variação de perfil](#variacao)
	- [Possível paralelização](#paralelizacao)
- [Parte 2](#parte-2)
	- [Programa utilizado (SeriesPi)](#seriespi)
	- [_OProfile_](#oprofile)
		- [Instalação](#opinstalacao)
		- [Características](#opcaracteristicas)
		- [Configuração e funcionamento](#opconfig)
		- [Resultados](#opresultados)
	- [_Callgrind_](#callgrind)
		- [Instalação](#cginstalacao)
		- [Características](#cgcaracteristicas)
		- [Configuração e funcionamento](#cgconfig)
		- [Resultados](#cgresultados)
- [Extra](#extra)
- [Referências](#referencias)

<!-- PARTE 1 -->
## Parte 1 

<a name="variacao"></a>
### Variaçao de perfil 
```
O perfil é afetado pelas opções de configuração?
```
As opções de configuração afetam diretamente o tempo gasto em cada chamada de função realizada pelo programa. Já no que tange o número e a ordem dessas chamadas, o perfil se mantém o mesmo independente das configurações.

A figura abaixo ilustra um exemplo de perfil gerado com parâmetros de configuração altos (vetor com 30000000 posições e 90 repetições do cálculo). Perceba que, caso fosse gerado um perfil a partir de uma configuração menor (ex. vetor com 3000000 posições e 15 repetições do cálculo), o número de chamadas de cada função (coluna _calls_) se manteria o mesmo. A variação ocorreria no tempo gasto em cada função, conforme exemplos observados nos arquivos <a href="https://github.com/rwfazul/elc139-2018a/blob/master/trabalhos/t2/parte_1/perfis/gprof_profile1.txt" alt="Perfil gprof exemplo um">_gprof_profile1_</a> e <a href="https://github.com/rwfazul/elc139-2018a/blob/master/trabalhos/t2/parte_1/perfis/gprof_profile2.txt" alt="Perfil gprof exemplo dois">_gprof_profile2_</a>.

<p align="center">
  <img src="https://github.com/rwfazul/elc139-2018a/blob/master/trabalhos/t2/parte_1/imagens/dotprod_seq-exemploPerfil.png" alt="Exemplo de perfil gerado." width="70%"/>
</p>

Realizando várias execuções do programa (através do _script_ <a href="https://github.com/rwfazul/elc139-2018a/blob/master/trabalhos/t2/dotprod_seq/run_tests.sh">_run\_tests.sh_</a>), foi possível gerar o gráfico abaixo, que ilustra como diferentes parâmetros de configuração afetam o tempo de execução do programa. Duas relações podem ser observadas: i) impacto do aumento do tamanho do vetor para o cálculo do produto escalar e ii) impacto do aumento do número de repetições do cálculo.

<p align="center">
  <img src="https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t2/parte_1/imagens/dotprod_seq-graficoTempos.png" alt="Tempo gasto pelo programa com diferentes configurações." width="80%"/>
</p>

Uma alternativa para diminuir estes tempo é realizar a execução em paralelo de certos trechos do programa. A seção seguinte discute como essa paralelização poderia ser realizada.
	
<a name="paralelizacao"></a>	
### Possível paralelização 
```
Pelo perfil de execução, há alguma função que poderia ser candidata a paralelização? Por quê?
```
Em uma análise '_black box_', levando em consideração o perfil apresentado anteriormente, percebe-se que a função **_dot\_product()_** é a responsável por **92.74%** do tempo total de execução do programa. Logo, sendo o 'gargalo' de tempo, seria a opção mais lógica a ser analisada, pois  um tempo de execução elevado tende a elevar as chances de melhorias de desempenho via paralelização.

Já ao observar o trecho de código referente a essa função, percebe-se que a paralelização poderia ser sim, de fato, realizada. Em um cenário paralelo do programa analisado, a divisão de carga entre os _threads_/processos poderia ocorrer de diversas maneiras.

Sendo **_r_** o número de repetições do cálculo do produto escalar realizada pela função _dot\_product()_, **_v_** o tamanho dos vetores utilizados no cálculo e **_t_** o número de _threads_/processos a serem criados. Três das opções de paralelização possíves seriam:

- **Paralelizar as repetições**: fazer com que cada unidade de paralelização seja responsável por (**_r_** / **_t_**) das repetições do cálculo do produto escalar. Logo, desconsiderando o _overhead_ de criação/gerência das <i>threads</i>/processos e considerando que seja possível realizar a execução 100% do tempo em paralelo, o tempo gasto na execução do programa reduziria em **_t_**;
- **Paralelizar o cálculo do produto escalar**: fazer com que, dentro de uma repetição, sejam realizadas somas parcias do produto dos vetores. Desta forma, a divisão (**_v_** / **_t_**) faria com que cada unidade executasse somente os cálculos de seu intervalo (_range_ de atuação) e o resultado final do cálculo fosse obtido ao somar os resultados parciais (ex. no _join_ das _threads_). Logo, desconsiderando o <i>overhead</i> de criação/gerência das unidades e considerando que seja possível realizar a execução 100% do tempo em paralelo, o tempo gasto na execução do programa em cada iteração do cálculo reduziria em **_t_**;
- **Parelelizar ambos**: Como não há dependência entre os dois laços (repetições e cálculo), poderia-se dividir as iterações em **_t¹_** unidades de paralelização e, em cada uma das iterações, realizar a divisão em **_t²_** unidades com base no intervalo do cálculo do produto escalar. Em um mundo ideial, o tempo de execução do programa seria reduzido em (**_t¹_** &times; **_t²_**), na prática, o desempenho possivelmente seria melhorado em uma ordem inferior.

Em complemento, a função **_init_vectors()_**, embora execute em uma parcela de tempo menor (**2.79%**, conforme exemplo anterior), também poderia ser paralelizada. Caberia analisar se o _overhead_ de criação/gerência das unidades de paralelização seria diluído no ganho de desempenho no momento de inicialização do vetor (vetores muito grandes tendem a compensar esta paralelização).

<!-- PARTE 2 -->
## Parte 2

<a name="seriespi"></a>
### Programa utilizado (SeriesPi)
O programa utilizado para os testes (<a href="https://github.com/rwfazul/elc139-2018a/blob/master/trabalhos/t2/seriesPi/seriesPi.c">_seriesPi.c_</a>) é uma aplicação _multithreaded_ que utiliza _Pthreads_. Seu funcionamento é descrito abaixo:

- Consiste em um programa que calcula o valor de Pi através de quatro séries matemáticas distintas (_viete_, _nilakantha_, _wallis_ e _leibniz_);
- É criado um processo (através da chamada de sistema _fork_) para cada uma das séries (estrutura _master/slave_, onde o mestre é o processo inicial _./seriesPi_);
- Cada processo escravo cria **_n_** _threads_, sendo este o parâmetro recebido pela linha de comando (_argc_);
- Cada _thread_ realiza uma parte do cálculo de sua respectiva série. A divisão de carga de cada _thread_ utiliza como base o número total de iterações necessárias para o cálculo de cada uma das seríe. O total é distribuído em intervalos de mesmo tamanho entre as **_n_** _threads_;
- Os resultados parciais são unidos e calculados nos _joins_ das _threads_.

Supondo **_n_** = 4, para cada processo gerado pelo programa (total de quatro processos) cria-se quatro threads para realizar os cálculos de cada série. Logo, ao total seriam criados 4 processos e 16 _threads_ (em complemento com a _thread_ inicial de todos os processos). A figura abaixo ilustra a estrutura criada pelo programa:

<p align="center">
  <img src="https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t2/parte_2/imagens/estrutura-programa.png" alt="Estrutura do programa com quatro threads." width="40%"/>
</p>

Um exemplo de saída da execução do programa com **_n_** = 4 é observado a seguir:

<p align="center">
  <img src="https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t2/parte_2/imagens/output-programa.png" alt="Exemplo de output do programa." width="45%" id="output"/>
</p>

Espera-se que um ponto interessante de análise seja o fato de que as séries de _viete_ e _nilakantha_ são calculadas com poucas iterações (15 e 500 iterações, respectivamente), enquanto as seríes de _wallis_ e _leibniz_ são muito mais onerosas (ambas com 900000000 de iterações). Sendo assim, existe a possibilidade de que perfis alcançados por **amostragem** não consigam monitorar alguns dos cálculos.

Em contrapartida, em _profilers_ que atuam por **instrumentação**, espera-se que as séries de  _wallis_ e _leibniz_ não sejam analisadas com um desempenho satisfatório, tendo em vista que esta categoria de _profilers_ tende a ser extremamente intrusiva no momento de execução do programa. 

### _OProfile_
_OProfile_ é um projeto _open source_ que inclui um profiler estático para sistemas Linux (_operf_), capaz de realizar o _profiling_ de aplicações com um **baixo _overhead_**. A ferramenta utiliza o _hardware_ de monitoramento de desempenho do(s) processador(es) da máquina para recuperar informações sobre o _kernel_ e os executáveis do sistema. _OProfile_ também é capaz de gerar o perfil de aplicações que rodam em uma máquina virtual Java (JVM).

<a name="opcaracteristicas"></a>	
#### Características

- Categoria: **Amostragem**;
- Análise dos resultados: **_Post mortem_** com um dos utilitários inclusos (ex. _opreport_);
- Não necessita de recompilação;
- O perfil pode ser gerado para todo o código rodando no sistema ou para processos individuais;	
- Consegue analisar os eventos da aplicação corrente, de um conjunto de processos ou threads, sub-conjunto de processadores ou do sistema inteiro;
- Trabalha bem com aplicações que realizam _fork_&rarr;_execs_ e aplicações **_multithreaded_**;
- Pode atuar sobre aplicações que já estão rodando, bastanto apontar seu PID e finalizar o _profiling_ com _CTRL+C_;
- O _overhead_ típico gira em torno de 1-8%, dependendo da frequência de amostragem e da carga de tabalho.

<a name="opinstalacao"></a>	
#### Instalação 
```
	$ sudo apt-get update
	$ sudo apt-get install oprofile
```

<a name="opconfig"></a>	
#### Configuração e funcionamento

Realizar o _profiling_ com o **_operf_** permite direcionar, com precisão, o perfil desejado (ex. único processo ou múltiplos). Formas de controlar o _profiler_ e a listagem de parâmetros de configuração podem ser encontradas em detalhes <a href="http://oprofile.sourceforge.net/doc/controlling-profiler.html">aqui</a>. Para utilizar a ferramenta com uma configuração básica (sem opções definidas) é utilizado o seguinte comando:

```
	$ operf ./meu_programa <meus_args>
```

Quando _./meu\_programa_ finalizar (ou quando _Crtl+C_ for pressionado), o _profiling_ finaliza (o diretório _oprofile\_data_ é criado) e o resultado pode ser analisado com as ferramentas de pós-processamento. O utilitário **_opreport_** é a principal ferramenta para obter os resultados formatados a partir do _Oprofile_.

A ferramenta pode ser utilizada para analisar o resultado das mais variadas formas, como por exemplo, comparar resultados de dois perfis diferentes, gerar um XML com os dados e exibir o grafo de chamadas das funções. A listagem de parâmetros de configuração podem ser encontradas em detalhes <a href="http://oprofile.sourceforge.net/doc/opreport.html">aqui</a>. O seguinte comando resulta na forma mais básica de análise dos resultados:

```
	$ opreport
```

Este comando deve ser executado no mesmo diretório onde o comando para realizar o perfil foi executado (por _default_ no mesmo diretório de _./meu\_programa_).

<a name="opresultados"></a>	
#### Resultados
Conforme já apresentado, o programa <a href="https://github.com/rwfazul/elc139-2018a/blob/master/trabalhos/t2/seriesPi/seriesPi.c">_seriesPi.c_</a> é uma aplicação _multithreaded_ que utiliza _Pthreads_. Como a aplicação possui mais de uma _thread_ (para os testes, utilizou-se **_n_** = 4), as configurações para execução do _profiler_ foram as seguintes:

```
	$ operf --separate-thread --separate-cpu --callgraph ./seriesPi 4
```

Existem diversas opções para visualização dos resultados, algumas das análises possíveis seriam:

```
	$ opreport --merge tgid
	$ opreport --merge tgid,cpu
	$ opreport --merge tgid,cpu --details --exclude-dependent --xml
	$ opreport --merge tgid --details --exclude-dependent
```

A figura abaixo ilustra o resultado obtido analisando o desempenho do programa entre as diferentes CPUs (para obtenção dos valores, _threads_ foram unidas com a opção **_'--merge tgid'_**). Apenas as informações importantes para o exemplo foram mantidas, o resultado completo pode ser encontrado em <a href="https://github.com/rwfazul/elc139-2018a/blob/master/trabalhos/t2/parte_2/perfis/oprofile/oprofile_profile1.txt" alt="Perfil OProfile exemplo um.">_oprofile\_profile1_</a>.
        
<p align="center">
	<img src="https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t2/parte_2/imagens/oprofile_tgdi.png" alt="OProfile com união de threads." width="80%"/>
</p>

A figura abaixo ilustra o resultado obtido em um contexto global (para obtenção dos valores, _threads_ e CPUs foram unidas com a opção **_'--merge tgid,cpu'_**). Apenas as informações importantes para o exemplo foram mantidas, o resultado completo pode ser encontrado em <a href="https://github.com/rwfazul/elc139-2018a/blob/master/trabalhos/t2/parte_2/perfis/oprofile/oprofile_profile2.txt" alt="Perfil OProfile exemplo dois.">_oprofile\_profile2_</a>.
       
<p align="center">
	<img src="https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t2/parte_2/imagens/oprofile_tgdi_cpu.png" alt="OProfile com união de threads e CPUs." width="40%"/>
</p>

Como esperado, o cálculo das outras duas séries (_viete_ e _nilakantha_) ocorreram tão rapidamente que o _profiler_ não conseguiu obter as suas respectivas amostras. Este é um dos problemas de realizar a análise de aplicações _multithreaded_ em _profilers_ da que atuam com amostragem. 

Ferramentas de visualização gráficas podem ser utilizadas para facilitar a análise e visualizar o grafo de chamadas das funções (_call graph_), durante a apresentação do próximo _profiler_ uma destas possíveis ferramentas será apresentada.

### _Callgrind_
<i>Callgrind</i> é uma ferramenta de _profiling_ inclusa no _framework open source_ **_Valgrind_**. Por ser uma _framework_ muito bem consolidado e que recebe grande suporte da comunidade, possui uma documentação extensiva e diversas outras ferramentas para apoio e funcionalidades extras.

<a name="cgcaracteristicas"></a>	
#### Características:

- Categoria: **Instrumentação**;
- Análise dos resultados: **_Post mortem_**;
- Não necessita de recompilação;
- Também consegue realizar o perfil de bibliotecas compartilhadas, _plugins_ e demais recursos;
- Trabalha bem com aplicações que realizam _fork_&rarr;_execs_ e aplicações **_multithreaded_**;
- Possui ferramentas muito boas para visualização dos dados dos perfis gerados;
- O desempenho do programa é bastante prejudicado (instrumentação gera maior _overhead_ que amostragem) ao realizar o _profiling_;
	
<a name="cginstalacao"></a>	
#### Instalação 
```
	$ sudo apt-get install valgrind kcachegrind graphviz
```

- O comando acima inclui a ferramenta para visualização **_KCachegrind_** (que por sua vez necessita do pacote _graphviz_).

<a name="cgconfig"></a>	
#### Configuração e funcionamento

Primeiramente deve-se realizar o _profiling_ da aplicação com o _Callgrind_. Formas de controlar o _profiler_ e a listagem de parâmetros de configuração podem ser encontradas em detalhes <a href="http://valgrind.org/docs/manual/cl-manual.html#cl-manual.options">aqui</a>. Para utilizar a ferramenta com uma configuração básica é utilizado o seguinte comando:

```
	$ valgrind --tool=callgrind ./meu_programa <meus_args>
```


O resultado da execução do programa será salvo em um arquivo de saída (por padrão no mesmo diretório) para cada processo/_thread_, sufixado com seu PID. Este arquivo pode ser lido em um editor de texto qualquer, porém é aconselhado o uso de uma ferramenta específica para a visualização, como o **_KCachegrind_**. Para isso, o seguinte comando pode ser executado:

```
	$ kcachegrind callgrind.out.PID
```

Detalhes dos recursos da ferramenta são abordados na seção seguinte.

<a name="cgresultados"></a>	
#### Resultados

O programa <a href="https://github.com/rwfazul/elc139-2018a/blob/master/trabalhos/t2/seriesPi/seriesPi.c">_seriesPi.c_</a> também foi utilizado para os testes com o _callgrind_. O comando utilizado para execução foi:

```
	$ valgrind --tool=callgrind --separate-threads=yes ./seriesPi 4
```

A opção **'--_separate-threads=yes_'** faz com que um perfil seja gerado para cada _thread_. Sendo assim, pode-se observar o perfil individual de cada uma das _threads_ e, por consequência, de cada um dos processos. Para análise de todos os perfis gerados, utilizou-se:

```
	$ kcachegrind callgrind.out.*
```

Ao contrário do _Oprofile_, o _Callgrind_, que atua através de instrumentação, consegue traçar o perfil de funções que acabam sua execução muito rapidamente. A figura abaixo ilustra um exemplo de perfil analisado com o _KCachegrind_ referente a série de _viete_, esta série realiza 15 iterações ao total, sendo que pelo uso das 4 _threads_, a divisão de carga para cada _thread_ resulta em 3 _threads_ com 4 iterações cada e 1 _thread_ com apenas 3 iterações.

<p align="center">
	<img src="https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t2/parte_2/imagens/callgrind_viete.png" alt="Exemplo de visualização no KCachegrind." width="80%"/>
</p>

Perceba, que mesmo com esta carga de trabalho tão pequena, o _Callgrind_ consegue realizar uma análise fiel. No caso da figura acima, a _thread_ 2 do processo com PID 7353 é a _thread_ que executou apenas 3 das iterações da série.

Um recurso interessante da ferramenta _Kcachegrind_ é a geração de um gráfico de chamadas (_call graph_). Abaixo é ilustrado o gráfico de chamadas referente a série de _wallis_.

<p align="center">
	<img src="https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t2/parte_2/imagens/callgrind_callgraph.png" alt="KCachegrind visualização call graph." width="80%"/>
</p>

Em contrapartida com a completude e corretude de perfis gerados com ferramentas como o _Callgrind_, algumas desvantagens são aparentes. Conforme esperado, a instrumentação realizada pelo _Callgrind_ afeta, de fato, o desempenho do programa. Houve dificuldades em executar o programa com suas configurações normais por conta do número de iterações realizadas pelas duas últimas séries e, em específico, da série de _leibniz_, que possui um cálculo muito mais oneroso e demorado. Abaixo é realizado uma comparação dos tempos de execução dos cálculos, com e sem o uso dos _profilers_:

<p align="center">
	<img src="https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t2/parte_2/imagens/profilers_overhead.png" alt="Overhead gerado pelos profilers." width="40%"/>
</p>

O _overhead_ gerado pelo _Callgrind_ afetou o programa em níveis tão grandes que o cálculo da série de _leibniz_ não conseguiu ser finalizado. Ferramentas do próprio _Callgrind_ permitem analisar melhor este tipo de problema. Para tal, o seguinte comando foi executado enquanto o _profiling_ estava sendo realizado:

```
	$ callgrind_control -e -b
```

Este comando permite observar e controlar interativamente o status de um programa atualmente em execução sob o controle do Callgrind, sem interromper o programa. Assim, é possível obter informações de estatísticas, bem como o rastreamento de pilha atual e solicitar zeramento de contadores ou despejo de dados do perfil. 

<p align="center">
	<img src="https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t2/parte_2/imagens/callgrind_overhead-threads.png" alt="Overhead threads callgrind." width="45%"/>
</p>

Com isso, percebeu-se que as _threads_ responsáveis pelo cálculo da série de _leibniz_ estavam sobrecarregadas (_'Ir'_ diz respeito aos contadores de eventos de instruções lidas). Ao analisar as _threads_ individualmente (ex. _thread_ 4) obteve-se:

<p align="center">
	<img src="https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t2/parte_2/imagens/callgrind_overhead-thread4.png" alt="Análise de overhead em thread callgrind." width="45%"/>
</p>

No exemplo, a função _pow()_ foi chamada 19.173.093 vezes (somente nessa _thread_ referente ao processo específico do cálculo de _leibniz_) e, em cada uma das chamadas, o _Callgrind_ estava realizando o monitoramento. Deste modo, tentou-se zerar todos os contadores de eventos com o comando:

```
	$ callgrind_control --zero
```

Ainda assim, a execução do programa (em específico do cálculo referente a esta série), não conseguiu ser finalizado em mais de 30 minutos de execução do programa com os contadores sendo zerados periodicamente a cada 3 minutos.


### Extra
	- Dúvidas e/ou dificuldades encontradas
- Dificuldade com visualização dos resultados com outro profiler testado (gperftools). Documentação não muito boa.

<!-- REFERÊNCIAS -->
<a name="referencias"></a>
## Referências 
- **_gprof_**
	- sourceware. <i>GNU gprof.</i> https://goo.gl/A5ie8Y
	- The Geek Stuff. <i>GPROF Tutorial – How to use Linux GNU GCC Profiling Tool.</i> https://goo.gl/unADsP
- **_OProfile_**
	- IBM. <i>Getting started with OProfile.</i> https://goo.gl/SYkFXL
	- OProfile. <i>OProfile - A System Profiler for Linux.</i> https://goo.gl/xsQMst
	- Oprofile. <i>Chapter 3. Controlling the profiler.</i> https://goo.gl/HQgHm6
	- Oprofile. <i>Image summaries and symbol summaries (opreport).</i> https://goo.gl/C2apr3
- **_Callgrind_**
	- Baptiste. <i>How to profile C++ application with Callgrind / KCacheGrind.</i> https://goo.gl/K4KwUQ
	- Gernot Klingler. <i>gprof, Valgrind and gperftools - an evaluation of some tools for application level CPU profiling on Linux.</i> https://goo.gl/jcnukg
	- KCachegrind. <i>KCachegrind: Call Graph Viewer.</i> https://goo.gl/kqua7t
	- Valgrind. <i>Callgrind: a call-graph generating cache and branch prediction profiler.</i> https://goo.gl/DWCL9G
