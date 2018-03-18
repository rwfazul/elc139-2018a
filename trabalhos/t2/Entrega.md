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
As opções de configuração afetam diretamente o tempo gasto em cada chamada de função realizada pelo programa. Já no que tange o número e a ordem dessas chamadas, o perfil se mantém o mesmo independente das configurações.

A figura abaixo ilustra um exemplo de perfil gerado com parâmetros de configuração altos (vetor com 30000000 posições e 90 repetições do cálculo). Perceba que, caso fosse gerado um perfil a partir de uma configuração menor (ex. vetor com 3000 posições e 10 repetições do cálculo), o número de chamadas de cada função (coluna _calls_) se manteria o mesmo. A variação ocorreria no tempo gasto em cada função, conforme exemplos observados no <a href="#">perfil I</a> e <a href="#">perfil II</a>.

<p align="center">
  <img src="https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t2/imagens/dotprod_seq-exemploPerfil.png" alt="Exemplo de perfil gerado." width="70%"/>
</p>

Realizando várias execuções do programa (através do _script_ <a href="https://github.com/rwfazul/elc139-2018a/blob/master/trabalhos/t2/dotprod_seq/run_tests.sh">_run\_tests.sh_</a>), foi possível gerar o gráfico abaixo, que ilustra como diferentes parâmetros de configuração afetam o tempo de execução do programa. Duas relações podem ser observadas: i) impacto do aumento do tamanho do vetor para o cálculo do produto escalar e ii) impacto do aumento do número de repetições do cálculo.

<p align="center">
  <img src="https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t2/imagens/dotprod_seq-graficoTempos.png" alt="Tempo gasto pelo programa com diferentes configurações." width="80%"/>
</p>

Uma alternativa para diminuir estes tempo é realizar a execução em paralelo de certos trechos do programa. A seção seguinte discute como essa paralelização poderia ser realizada.
	
<a name="paralelizacao"></a>	
### Possível paralelização 
```
Pelo perfil de execução, há alguma função que poderia ser candidata a paralelização? Por quê?
```
Em uma análise '_black box_', levando em consideração o perfil apresentado anteriormente, percebe-se que a função _dot\_product()_ é a responsável por 92.74% do tempo total de execução do programa. Logo, sendo o 'gargalo' de tempo, seria a opção mais lógica a ser analisada.

Já ao observar o trecho de código referente a essa função, percebe-se que a parelização poderia ser sim, de fato, realizada. Em um cenário paralelo do programa analisado, a divisão de carga entre os _threads_/processos poderia ocorrer de diversas maneiras.

Sendo **r** o número de repetições do cálculo do produto escalar realizada pela função _dot\_product()_, **v** o tamanho dos vetores utilizados no cálculo e **t** o número de _threads_/processos a serem criados. Três das opções de parelização possíves seriam:

	- **Paralelizar as repetições**: fazer com que cada unidade de paralelização seja responsável por (**r** / **t**) das repetições do cálculo do produto escalar. Logo, desconsiderando o _overhead_ de criação/gerência das <i>threads</i>/processos e considerando que seja possível realizar a execução 100% do tempo em paralelo, o tempo gasto na execução do programa reduziria em **t**;
	- **Paralelizar o cálculo do produto escalar**: fazer com que, dentro de uma repetição, sejam realizadas somas parcias do produto dos vetores. Desta forma, a divisão (**v** / **t**) faria com que cada unidade executasse somente os cálculos de seu intervalo (_range_ de atuação) e o resultado final do cálculo fosse obtido ao somar os resultados parciais (ex. no _join_ das _threads_). Logo, desconsiderando o <i>overhead</i> de criação/gerência das unidades e considerando que seja possível realizar a execução 100% do tempo em paralelo, o tempo gasto na execução do programa em cada iteração do cálculo reduziria em **t**;
	- **Parelelizar ambos**: Como não há dependência entre os dois laços (repetições e cálculo), poderia-se dividir as iterações em **t¹** unidades de paralelização e, em cada uma das iterações, realizar a divisão em **t²** unidades com base no intervalo do cálculo do produto escalar. Em um mundo ideial, o tempo de execução do programa seria reduzido em (**t¹** &times; **t²**), na prática, o desempenho possivelmente seria melhorado em uma ordem inferior.

Em complemento, a função <i>init_vectors()</i>, embora execute em uma parcela muito pequena do programa (2.79%), também poderia ser paralelizada. Caberia analisar se o <i>overhead</i> de criação/gerência das unidades de paralelização seria diluído no ganho de desempenho no momento de inicialização do vetor (vetores muito grandes tendem a compensar esta parelização).

   
<!-- PARTE 2 -->
## Parte 2

### OProfile
_OProfile_ é um projeto _open source_ que inclui uma profiler estático para sistemas Linux (_operf_), capaz de realizar o _profiling_de aplicações com um baixo _overhead_. A ferramenta utiliza o hardware de monitoramento de desempenho do(s) processador(es) para recuperar informações sobre o kernel e os executáveis do sistema. OProfile também é capaz de gerar o perfil de aplicações que rodam em uma máquina virtual Java (JVM).


Características:

	- Categoria: Amostragem;
	- Análise dos resultados: _Post mortem_ com um dos utilitários inclusos (ex. _opreport_);
	- Não necessita de recompilação;
	- O perfil pode ser gerado para todo o código rodando no sistema ou para processos individuais;	
	- Consegue analisar os eventos da aplicação corrente, de um conjunto de processos ou threads, sub-conjunto de processadores ou do sistema inteiro;
	- Trabalha bem com aplicações que realizam _fork_->_execs_ e aplicações _multithreaded_;
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

Quando _./meu/_programa_ finalizar (ou quando _Crtl+C_ for pressionado), o _profiling_ finaliza (o diretório _oprofile\_data_ é criado) e o resultado pode ser analisado com as ferramentas de pós-processamento. O utilitário **_opreport_** é a principal ferramenta para obter os resultados formatados a partir do _Oprofile_.

A ferramenta pode ser utilizada para analisar o resultado das mais variadas formas, como por exemplo, comparar resultados de dois perfis diferentes, gerar um XML com os dados e exibir o grafo de chamadas das funções. A listagem de parâmetros de configuração podem ser encontradas em detalhes <a href="http://oprofile.sourceforge.net/doc/opreport.html">aqui</a>. O seguinte comando resulta na forma mais básica de análise dos resultados:

```
	$ opreport
```

Este comando deve ser executado no mesmo diretório onde o comando para realizar o perfil foi executado (por _default_ no mesmo diretório de _./meu\_programa_).

<a name="opresultados"></a>	
#### Resultados


### Callgrind
<i>Callgrind</i> é uma ferramenta de _profiling_ inclusa no _framework open source_ **_Valgrind_**. Por ser uma _framework_ muito bem consolidado e que recebe grande apoio da comunidade, possui uma documentação extensiva e diversas outras ferramentas para apoio e funcionalidades extras.

Características:

	- Categoria: Instrumentação;
	- Análise dos resultados: _Post mortem_;
	- Não necessita de recompilação;
	- Também consegue realizar o perfil de bibliotecas compartilhadas, _plugins_ e demais recursos;
	- Trabalha bem com aplicações _multithreaded_;
	- Possui ferramentas muito boas para visualização dos dados dos perfis gerados;
	- O desempenho do programa é bastante prejudicado (instrumentação gera maior _overhead_ que amostragem) ao realizar o _profiling_;
	
<a name="cginstalacao"></a>	
#### Instalação 
```
	$ sudo apt-get install valgrind kcachegrind graphviz
```

O comando acima inclui a ferramenta para visualização **_KCachegrind_** (que por sua vez necessita do pacote _graphviz_).

<a name="cgconfig"></a>	
#### Configuração e funcionamento

Primeiramente deve-se realizar o _profiling_ da aplicação com o _Callgrind_. Formas de controlar o _profiler_ e a listagem de parâmetros de configuração podem ser encontradas em detalhes <a href="http://valgrind.org/docs/manual/cl-manual.html#cl-manual.options">aqui</a>. Para utilizar a ferramenta com uma configuração básica é utilizado o seguinte comando:

```
	$ valgrind --tool=callgrind ./meu_programa <meus_args>
```


O resultado será salvo em um arquivo de saída (por padrão no mesmo diretório). Este arquivo pode ser lido em um editor de texto qualquer, porém é aconselhado o uso de uma ferramenta específica para a visualização, como o _KCachegrind_.

```
	$ kcachegrind callgrind.out.PID
```

Detalhes dos recursos da ferramenta são abordados na seção seguinte.

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
