[Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2018a) > Trabalhos

# T2: Experiências com Profilers

Disciplina: ELC139 - Programação Paralela <br/>
Aluno: Rhauani Weber Aita Fazul

## Sumário

- [Parte 1](#parte-1)
	- [Variação de perfil](#variacao)
	- [Possível paralelização](#paralelizacao)
- [Parte 2](#parte-2)
	- [OProfile](#oprofile)
		- [Instalação](#opinstalacao)
		- [Configuração e funcionamento](#opconfig)
		- [Resultados](#opresultados)
	- [Callgrind](#callgrind)
		- [Instalação](#cginstalacao)
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
<p align="justify">As opções de configuração afetam diretamente o tempo gasto em cada chamada de função realizada pelo programa. Já no que tange o número e a ordem dessas chamadas, o perfil se mantém o mesmo independente das configurações.</p>

<p align="justify">A figura abaixo ilustra um exemplo de perfil gerado com parâmetros de configuração altos (vetor com 30000000 posições e 90 repetições do cálculo). Perceba que, caso fosse gerado um perfil a partir de uma configuração menor (ex. vetor com 3000 posições e 10 repetições do cálculo), o número de chamadas de cada função (coluna <i>calls</i>) se manteria o mesmo. A variação ocorreria no tempo gasto em cada função, conforme exemplos observados no <a href="#">perfil I</a> e <a href="#">perfil II</a>.</p>

<p align="center">
  <img src="https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t2/imagens/dotprod_seq-exemploPerfil.png" alt="Exemplo de perfil gerado." width="70%"/>
</p>

<p align="justify">Realizando várias execuções do programa (através do script <a href="https://github.com/rwfazul/elc139-2018a/blob/master/trabalhos/t2/dotprod_seq/run_tests.sh"><i>run_tests.sh</i></a>), foi possível gerar o gráfico abaixo, que ilustra como diferentes parâmetros de configuração afetam o tempo de execução do programa. Duas relações podem ser observadas: i) impacto do aumento do tamanho do vetor para o cálculo do produto escalar e ii) impacto do aumento do número de repetições do cálculo.</p>

<p align="center">
  <img src="https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t2/imagens/dotprod_seq-graficoTempos.png" alt="Tempo gasto pelo programa com diferentes configurações." width="80%"/>
</p>

<p align="justify">Uma alternativa para diminuir estes tempo é realizar a execução em paralelo de certos trechos do programa. A seção seguinte discute como essa paralelização poderia ser realizada.</p>
	
<a name="paralelizacao"></a>	
### Possível paralelização 
```
Pelo perfil de execução, há alguma função que poderia ser candidata a paralelização? Por quê?
```
<p align="justify">Em uma análise 'black box', levando em consideração o perfil apresentado anteriormente, percebe-se que a função <i>dot_product()</i> é a responsável por 92.74% do tempo total de execução do programa. Logo, sendo o 'gargalo' de tempo, seria a opção mais lógica a ser analisada.</p>

<p align="justify">Já ao observar o trecho de código referente a essa função, percebe-se que a parelização poderia ser sim, de fato, realizada. Em um cenário paralelo do programa analisado, a divisão de carga entre os <i>threads</i>/processos poderia ocorrer de diversas maneiras.</p>

<p align="justify">Sendo **r** o número de repetições do cálculo do produto escalar realizada pela função <i>dot_product()</i>, **v** o tamanho dos vetores utilizados no cálculo e **t** o número de <i>threads</i>/processos a serem criados. Três das opções de parelização possíves seriam:</p>

<p align="justify">- **Paralelizar as repetições**: fazer com que cada unidade de paralelização seja responsável por (**r** / **t**) das repetições do cálculo do produto escalar. Logo, desconsiderando o <i>overhead</i> de criação/gerência das <i>threads</i>/processos e considerando que seja possível realizar a execução 100% do tempo em paralelo, o tempo gasto na execução do programa reduziria em **t**;</p>
<p align="justify">- **Paralelizar o cálculo do produto escalar**: fazer com que, dentro de uma repetição, sejam realizadas somas parcias do produto dos vetores. Desta forma, a divisão (**v** / **t**) faria com que cada unidade executasse somente os cálculos de seu intervalo (<i>range</i> de atuação) e o resultado final do cálculo fosse obtido ao somar os resultados parciais (ex. no <i>join</i> das <i>threads</i>). Logo, desconsiderando o <i>overhead</i> de criação/gerência das unidades e considerando que seja possível realizar a execução 100% do tempo em paralelo, o tempo gasto na execução do programa em cada iteração do cálculo reduziria em **t**;</p>
<p align="justify">- **Parelelizar ambos**: Como não há dependência entre os dois laços (repetições e cálculo), poderia-se dividir as iterações em **t¹** unidades de paralelização e, em cada uma das iterações, realizar a divisão em **t²** unidades com base no intervalo do cálculo do produto escalar. Em um mundo ideial, o tempo de execução do programa seria reduzido em (**t¹** x **t²**), na prática, o desempenho possivelmente seria melhorado em uma ordem inferior.</p>

<p align="justify">Em complemento, a função <i>init_vectors()</i>, embora execute em uma parcela muito pequena do programa (2.79%), também poderia ser paralelizada. Caberia analisar se o <i>overhead</i> de criação/gerência das unidades de paralelização seria diluído no ganho de desempenho no momento de inicialização do vetor (vetores muito grandes tendem a compensar esta parelização).</p>

   
<!-- PARTE 2 -->
## Parte 2

### OProfile
```
<p align="justify">OProfile é um projeto <i>open source</i> que inclui uma profiler estático para sistemas Linux (<i>operf</i>), capaz de realizar o <i>profiling</i> de aplicações com um baixo <i>overhead</i>. A ferramenta utiliza o hardware de monitoramento de desempenho do(s) processador(es) para recuperar informações sobre o kernel e os executáveis do sistema. OProfile também é capaz de gerar o perfil de aplicações que rodam em uma máquina virtual Java (JVM).</p>
```
<p align="justify">Características:</p>

	- Categoria: Amostragem;
	- Análise dos resultados: <i>Post mortem</i> com um dos utilitários inclusos (ex. <i>opreport</i>);
	- Não necessita de recompilação;
	- O perfil pode ser gerado para todo o código rodando no sistema ou para processos individuais;	
	- Consegue analisar os eventos da aplicação corrente, de um conjunto de processos ou threads, sub-conjunto de processadores ou do sistema inteiro;
	- Trabalha bem com aplicações que realizam <i>fork</i>-><i>execs</i> e aplicações <i>multithreaded</i>;
	- O <i>overhead</i> típico gira em torno de 1-8%, dependendo da frequência de amostragem e da carga de tabalho.

<a name="opinstalacao"></a>	
#### Instalação 
```
	$ sudo apt-get update
	$ sudo apt-get install oprofile
```

<a name="opconfig"></a>	
#### Configuração e funcionamento

<p align="justify">Realizar o <i>profiling</i> com o <i>operf</i> permite direcionar, com precisão, o perfil desejado (ex. único processo ou múltiplos). Formas de controlar o profiler e a listagem de parâmetros de configuração podem ser encontradas em detalhes <a href="http://oprofile.sourceforge.net/doc/controlling-profiler.html">aqui</a>. Para utilizar a ferramenta com uma configuração básica (sem opções definidas) é utilizado o seguinte comando:</p>

```
	$ operf ./meu_programa <meus_args>
```

<p align="justify">Quando ./meu_programa finalizar (ou quando Crtl+C for pressionado), o <i>profiling</i> finaliza (o diretório <i>oprofile_data</i> é criado) e o resultado pode ser analisado com as ferramentas de pós-processamento. O utilitário <i>opreport</i> é a principal ferramenta para obter os resultados formatados a partir do Oprofile.</p>

<p align="justify">A ferramenta pode ser utilizada para analisar o resultado das mais variadas formas, como por exemplo, comparar resultados de dois perfis diferentes, gerar um XML com os dados e exibir o grafo de chamadas das funções. A listagem de parâmetros de configuração podem ser encontradas em detalhes <a href="http://oprofile.sourceforge.net/doc/opreport.html">aqui</a>. O seguinte comando resulta na forma mais básica de análise dos resultados:</p>

```
	$ opreport
```

<p align="justify">Este comando deve ser executado no mesmo diretório onde o comando para realizar o perfil foi executado (por <i>default</i> no mesmo diretório de ./meu_programa).</p>

<a name="opresultados"></a>	
#### Resultados


### Callgrind
```
<p align="justify">Callgrind é uma ferramenta de <i>profiling</i> inclusa no <i>framework open source</i> Valgrind. Por ser uma framework muito bem consolidado e com grande apoio da comunidade, possui uma documentação extensiva e diversas outras ferramentas para apoio e funcionalidades extras.</p>
```
<p align="justify">Características:</p>

	- Categoria: Instrumentação;
	- Análise dos resultados: <i>Post mortem</i>;
	- Não necessita de recompilação;
	- Também consegue realizar o perfil de bibliotecas compartilhadas, plugins;
	- Trabalha bem com aplicações <i>multithreaded</i>;
	- Possui ferramentas muito boas para visualização dos dados dos perfis gerados;
	- O desempenho do programa é bastante prejudicado (instrumentação gera maior <i>overhead</i> que amostragem) ao realizar o <i>profiling</i>;
	
<a name="cginstalacao"></a>	
#### Instalação 
```
	$ sudo apt-get install valgrind kcachegrind graphviz
```

O comando acima inclui a ferramenta para visualização <i>KCachegrind</i> (que por sua vez necessita da <i>graphviz</i>).

<a name="cgconfig"></a>	
#### Configuração e funcionamento

<p align="justify">Primeiramente deve-se realizar o <i>profiling</i> da aplicação com o Callgrind. Formas de controlar o profiler e a listagem de parâmetros de configuração podem ser encontradas em detalhes <a href="http://valgrind.org/docs/manual/cl-manual.html#cl-manual.options">aqui</a>. Para utilizar a ferramenta com uma configuração básica é utilizado o seguinte comando:</p>

```
	$ valgrind --tool=callgrind ./meu_programa <meus_args>
```


<p align="justify">O resultado será salvo em um arquivo de saída (por padrão no mesmo diretório). Este arquivo pode ser lido em um editor de texto qualquer, porém é aconselhado o uso de uma ferramenta específica para a visualização, como o <i>KCachegrind</i>.</p>

```
	$ kcachegrind callgrind.out.PID
```

<p align="justify">Detalhes dos recursos da ferramenta são abordados na seção seguinte.</p>

<a name="cgresultados"></a>	
#### Resultados
If you are running a multi-threaded application and specify the command line option --separate-threads=yes, every thread will be profiled on its own and will create its own profile dump. Thus, the last two methods will only generate one dump of the currently running thread. With the other methods, you will get multiple dumps (one for each thread) on a dump request.

### Extra
	- Dúvidas e/ou dificuldades encontradas
- Dificuldade com vizualização dos resultados com outro profiler testado (gperftools). Documentação não muito boa.

<!-- REFERÊNCIAS -->
<a name="referencias"></a>
## Referências 

- The Geek Stuff. <i>GPROF Tutorial – How to use Linux GNU GCC Profiling Tool.</i> https://goo.gl/unADsP
- sourceware. <i>GNU gprof.</i> https://goo.gl/A5ie8Y

- OProfile. <i>OProfile - A System Profiler for Linux.</i> http://oprofile.sourceforge.net/news/
- IBM. <i>Getting started with OProfile.</i> https://goo.gl/SYkFXL
- Oprofile. <i>Chapter 3. Controlling the profiler.</i> http://oprofile.sourceforge.net/doc/controlling-profiler.html
- Oprofile. <i>Image summaries and symbol summaries (opreport).</i> http://oprofile.sourceforge.net/doc/opreport.html

- Valgrind. <i>Callgrind: a call-graph generating cache and branch prediction profiler.</i> http://valgrind.org/docs/manual/cl-manual.html
https://baptiste-wicht.com/posts/2011/09/profile-c-application-with-callgrind-kcachegrind.html
http://valgrind.org/docs/manual/cl-manual.html#cl-manual.options
