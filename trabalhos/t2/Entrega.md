[Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2018a) > Trabalhos

# T2: Experiências com Profilers

Disciplina: ELC139 - Programação Paralela <br/>
Aluno: Rhauani Weber Aita Fazul

## Sumário

- [Parte 1](#parte-1)
	- [Variação de perfil](#variacao)
	- [Possível paralelização](#paralelizacao)
- [Parte 2](#parte-2)
	- [Profilers escolhidos](#profilers-escolhidos)
	- [Instalação](#instalacao)
	- [Configuração e funcionamento](#config)
	- [Resultados](#resultados)
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

A figura abaixo ilustra um exemplo de perfil gerado com parâmetros de configuração altos (tamanho do vetor = 30000000 e repetições = 90). Perceba que, caso fosse gerado um perfil a partir de uma configuração menor (ex. tamanho do vetor = 3000 e repetições = 10), o número de chamadas de cada função (coluna <i>calls</i>) se manteria o mesmo. A variação ocorreria no tempo gasto em cada função.

![Exemplo de perfil gerado.](https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t2/imagens/dotprod_seq-exemploPerfil.png)

O gráfico abaixo ilustra como os parâmetros de configuração referentes ao tamanho do vetor e ao número de repetições afetam o tempo de execução do programa.

![Tempo gasto pelo programa com diferentes configurações.](https://raw.githubusercontent.com/rwfazul/elc139-2018a/master/trabalhos/t2/imagens/dotprod_seq-graficoTempos.png)

Uma alternativa para diminuir estes tempo é realizar a execução em paralelo de certos trechos do programa. A seção seguinte discute como essa paralelização poderia ser realizada.
	
<a name="paralelizacao"></a>	
### Possível paralelização 
```
Pelo perfil de execução, há alguma função que poderia ser candidata a paralelização? Por quê?
```
Em uma análise 'black box', levando em consideração o perfil apresentado anteriormente, percebe-se que a função <i>dot_product()</i> é a responsável por 92.74% do tempo total de execução do programa. Logo, sendo o 'gargalo' de tempo, seria a opção mais lógica a ser analisada.

Já ao observar o trecho de código referente a essa função, percebe-se que a parelização poderia ser sim, de fato, realizada. Em um cenário paralelo do programa analisado, a divisão de carga entre os <i>threads</i>/processos poderia ocorrer de diversas maneiras.

Sendo **r** o número de repetições do cálculo do produto escalar realizada pela função <i>dot_product()</i>, **v** o tamanho dos vetores utilizados e **t** o número de <i>threads</i>/processos a serem criados. Três das opções de parelização possíves seriam:

- **Paralelizar as repetições**: fazer com que cada unidade seja responsável por (**r** / **t**) das repetições do cálculo do produto escalar. Logo, desconsiderando o <i>overhead</i> de criação/gerência das <i>threads</i>/processos e considerando que seja possível realizar a execução 100% do tempo em paralelo, o tempo gasto na execução do programa reduziria em **t**;
- **Paralelizar o cálculo do produto escalar**: fazer com que, dentro de uma repetição, sejam realizadas somas parcias do produto dos vetores. Desta forma, a divisão (**v** / **t**) faria com que cada <i>thread</i> executasse somente os cálculos de seu intervalo (<i>range</i> de atuação) e o resultado final do cálculo fosse obtido no <i>join</i> das mesmas. Logo, desconsiderando o <i>overhead</i> de criação/gerência das <i>threads</i> e considerando que seja possível realizar a execução 100% do tempo em paralelo, o tempo gasto na execução do programa em cada iteração do cálculo reduziria em **t**;
- **Parelelizar ambos**: Como não há dependência entre os dois laços (repetições e cálculo), poderia-se dividir as iterações em **t¹** unidades e, em cada uma destas iterações, realizar a divisão com base no intervalo do cálculo para **t²** unidades. Em um mundo ideial, o tempo de execução do programa seria reduzido em (**t¹** * **t²**), na prática, o desempenho tenderia a ser melhorado em uma ordem inferior.

Em complemento, a função <i>init_vectors()</i>, embora execute em uma parcela muito pequena do programa (2.79%), também poderia ser paralelizada. Caberia analisar se o <i>overhead</i> de criação/gerência das unidades de paralelização seria diluído no ganho de desempenho no momento de inicialização do vetor (vetores muito grandes tendem a compensar esta parelização). 

   
<!-- PARTE 2 -->
## Parte 2

### Profilers escolhidos
```
Profiller 1 - link \\

Profiller 2 - link
```

<a name="instalacao"></a>
### Instalação 
	- Particularidades de instalação
```
Profiller 1
```

```
Profiller 2
```

<a name="config"></a>
### Configuração e funcionamento 
	- Recursos ou funcionamento dos profilers
```
Profiller 1
```

```
Profiller 2
```

### Resultados
	- Resultados obtidos
```
Profiller 1
```

```
Profiller 2
```

### Extra
	- Dúvidas e/ou dificuldades encontradas

<!-- REFERÊNCIAS -->
<a name="referencias"></a>
## Referências 
- Autor (nome de pessoa ou instituição). <i>Título.</i> URL.
- Autor (nome de pessoa ou instituição). <i>Título.</i> URL.
- Autor (nome de pessoa ou instituição). <i>Título.</i> URL.
