[Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2018a) > Trabalhos

# T4: Scheduling com OpenMP

Disciplina: ELC139 - Programação Paralela <br/>
Aluno: Rhauani Weber Aita Fazul

## Sumário

- [Implementação](#implementação)
	- [Seção crítica](#critical)
- [_Outputs_](#outputs)
	- [_Runtime Schedule_](#runtime-schedule)
	- [_Static Schedule_](#static-schedule)
		- [Caso de teste 1](#caso-de-teste-1)
		- [Caso de teste 2](#caso-de-teste-2)
		- [Caso de teste 2b](#caso-de-teste-2b)
	- [_Dynamic Schedule_](#dynamic-schedule)
		- [Caso de teste 3](#caso-de-teste-3)
		- [Caso de teste 4](#caso-de-teste-4)
		- [Caso de teste 4b](#caso-de-teste-4b)
	- [_Guided Schedule_](#guided-schedule)
		- [Caso de teste 5](#caso-de-teste-5)
		- [Caso de teste 6](#caso-de-teste-6)
		- [Caso de teste 6b](#caso-de-teste-6b)
	- [_Auto Schedule_](#auto-schedule)
		- [Caso de teste 7](#caso-de-teste-7)
		- [Caso de teste 8](#caso-de-teste-8)
- [Referências](#referencias)

<!-- Implementacao -->
## Implementação

O programa [ThreadOmpABC.cpp](ThreadABC/ThreadOmpABC.cpp) utiliza _OpenMP_ para testar os diferentes tipos de escalonamento disponíveis. Os casos de teste realizados foram desenvolvidos para ilustrar diferentes cenários, dentre estes:

- Problemas de sincronização, decorrentes do não uso de exclusão mútua entre as _threads_;
- Particionamento de carga entre as _threads_ com valores pré-definidos, levando em consideração o contexto do programa;
- Particionamento de carga entre as _therads_ utilizando os valores _default_ do próprio _OpenMP_.

<a name="critical"></a>
### Seção crítica

Para a computação ser realizada de uma maneira correta, é necessário realizar o controle de concorrência envolvendo as variáveis compartilhadas entre as _threads_, o que diz respeito a um vetor e seu respectivo contador. Com isso, cada caso de teste define a variável booleana '_usecritical_' para escolha de uso da diretiva '_omp critical_', que identifica e demarca uma seção crítica do código, conforme observado abaixo.

``` cpp
   void addChar(char c) {
      if (usecritical) { 
         #pragma omp critical 
         {
            do_operations(c); 
         } 
      } else {
         do_operations(c);
      }
   }
```

Onde as operações que devem ser executadas de maneira serializada estão definidas no método '_do\_operations()_'. 

``` cpp 
   void do_operations(char c) {
      array[index] = c; 
      spendSomeTime();
      index++;
   }
```

Com isso, caso a variável '_usecritical_' seja verdadeira, e uma _thread_ t1 estiver nesta seção crítica, a entrada de outras threads só será permitada após t1 liberar o acesso. Como o acesso do _array_ e o incremento do _index_ é compartilhado entre todas as threads, o uso da diretiva evita possíveis _race conditions_.

<!-- Outputs -->
## _Outputs_

O arquivo [_out.txt_](ThreadABC/out.txt) exemplifica a saída do programa. Abaixo estão descritos os casos de teste realizados.

<!-- Runtime -->
### _Runtime Schedule_
O tipo de escalonamento (_static_, _dynamic_ ou _guided_) e o _chunk size_ são definidos em tempo de execução com base em uma varíavel de controle interno ('_run-sched-var_'), que foi setada anteriormente no programa com a rotina '_omp\_set\_schedule()_'. O trecho de código abaixo ilustra o uso do _runtime schedule_ no programa (é ilegal definir _chunk size_ para este tipo de escalonamento).

``` cpp
      #pragma omp for schedule(runtime)  
      for (int i = 0; i < nThreads * nTimes; i++) {
         array->addChar(c);    
      }  
```

<p>Com isso, o tipo de escalonamento pode ser definido em tempo de execução, seguindo o protótipo:</p>

``` cpp
	void omp_set_schedule(omp_sched_t kind, int chunk_size);
```

<!-- Static -->
### _Static Schedule_
As iterações do _loop_ são divididas em janelas com tamanhos definido pelo _chunk size_ e, após, são atribuídas estaticamente para as _threads_ que fazem parte do time (similar a um _round-robin_ baseado no _TID_). Quando o _chunk size_ não é definido, as iterações são, na medida do possível, divididas em _chunks_ do mesmo tamanho (iterações &divide; _nThreads_) de maneira contígua entre as _threads_. <!-- Deste modo, um _chunk_ de tamanho 1 faria as iterações serem intercaladas. --> 
 
#### Caso de teste 1
Neste caso de teste não foi feito uso de exclusão mútua (via '_omp critical_', conforme apresentado anteriormente). Foi definido um _chunk_ size equivalente a _nTimes_ (quantidade de inserções a serem realizadas por cada _thread_). O resultado obtido, disponível em [out.txt](ThreadABC/out.txt), é apresentado a seguir:

```              
* Case 1: no omp critical (expecting wrong results)
	schedule_type: omp_sched_static, chunk_size: 20
   BACBACBACBAACBACBACBACBAACBACBACBACABACBACBACABCAB-CBCBCBCB-
                       A=19 B=20 C=19 
````

Como visto, o resultado obtido sem exclusão mútia é incorreto, mesmo com _chunk size_ = _nTimes_. 

#### Caso de teste 2
Em contraponto, para obtenção de resultado correto com o _static schedule_, utilizou-se exclusão mútua:

```
* Case 2: using omp critical (expecting correct results)
	schedule_type: omp_sched_static, chunk_size: 20
   CBBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCAAAAAAAAAAAAAAAAAAAA
                       A=20 B=20 C=20 
```

### Caso de teste 2b
O Caso 2b é particularmente interessante. Perceba que mesmo com _chunk size_ em 1, que faz as atribuições dos blocos serem intercaladas, o resultado se mantém correto. 

```
* Case 2b: using omp critical with chunk size = 1
	schedule_type: omp_sched_static, chunk_size: 1
   CAAAAAAAAAAAAAAAAAAAACCCCCCCCCCCCCCCCCCCBBBBBBBBBBBBBBBBBBBB
                       A=20 B=20 C=20 
```

Isto se dá pelo fato de que, com o escalonamento estático, cada _thread_ recebe o mesmo número de _chunks_, logo, cria-se _nTimes_ * _nThreads_ _chunks_ de tamanho 1 e cada _thread_ recebe _nTimes_ _chunks_.

<!-- Dynamic -->
### _Dynamic Schedule_
Cada _thread_ executa um _chunk_ de iterações e, quando completo, outro _chunk_ é dinamicamente atríbuido a mesma. Isso ocorre até todos os _chunks_ serem distríbuidos. Acredita-se que um auto nível de requisições/atribuições de _chunks_ cause um _overhead_ significativo.
  
#### Caso de teste 3
Este caso utiliza o escalonamento dinâmico, sem exclusão mútua e com um _chunk size_ de _nTimes_.

```
* Case 3: no omp critical (expecting wrong results)
	schedule_type: omp_sched_dynamic, chunk_size: 20
ABCABCABCABCABCACBACBACBACBACBACABCABCABCABCABCABCABCABCAB--
                       A=20 B=19 C=19 
```

O resultado obtido, como esperado, foi incorreto.

#### Caso de teste 4
Para ilustar o resultado correto com o _chunk size_ pré-definido realizou-se o Caso 4:

```
* Case 4: using omp critical (expecting correct results)
	schedule_type: omp_sched_dynamic, chunk_size: 20
   CAAAAAAAAAAAAAAAAAAAACCCCCCCCCCCCCCCCCCCBBBBBBBBBBBBBBBBBBBB
                       A=20 B=20 C=20 
```

Como observado cada _thread_ inseriu exatamente 20 caracteres. Mesmo que estes estejam em posições diferentes do Caso 2, o resultado continua correto, já que o posicionamento depende do escalonamento das _threads_.

#### Caso de teste 4b
Para fins de estudo do comportamento do programa, foi realizado um caso de teste em que o tamanho do _chunk_ não foi previamente definido. Deste modo, o valor _default_ do _OpenMP_ para o escalonamento dinâmico é utilizado, o que equivale a 1.

```
* Case 4b: using omp critical but chunk_size is the default value used by OpenMP
	schedule_type: omp_sched_dynamic, chunk_size: 1 (default)
   BCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCBA
                       A=1 B=2 C=57  
```

Perceba que, ao contário do Caso 2b, o resultado foi incorreto, mesmo utilizando exclusão mútua. Isso acontece porque, ao utilizar o mapeamento dinâmico com _chunks_ de tamanho _default_ (1), as _threads_ acabam por não serem designadas a atuar sobre a mesma quantidade de _chunks_, já que isto é feito de maneira dinâmica, ou seja, quando uma _thread_ completar as iterações, esta requisita um novo _chunk_.

<!-- Guided -->
### _Guided Schedule_
Similar ao _dynamic schedule_, mas o _chunk_ começa maior e diminui até _chunk size_ (define o tamanho mínimo do bloco) para compensar algum desequilíbrio de carga entre as iterações. Quando _chunk size_ não é definido, o tamanho inicial do _chunk_ é proporcional a (interações &divide; _nThreads_). Os demais blocos seguem a proporção de (iterações_restantes &divide; _nThreads_).

#### Caso de teste 5
Conforme resultado em [out.txt](ThreadABC/out.txt), o Caso 5 fez uso do _guided schedule_ com definição prévia do _chunk size_ e sem exclusão mútua:

```
* Case 5: no omp critical (expecting wrong results)
	schedule_type: omp_sched_guided, chunk_size: 20
   ABCABCABCABCABCABCABCABCABCABCABCABCACBACABCABCABCABCABCAB--
                       A=20 B=19 C=19 
```

#### Caso de teste 6
Para constatar a obtenção do resultado correto utilizando '_omp critical_', realizou-se o Caso 6.

```
* Case 6: using omp critical (expecting correct results)
	schedule_type: omp_sched_guided, chunk_size: 20
   CAAAAAAAAAAAAAAAAAAAACCCCCCCCCCCCCCCCCCCBBBBBBBBBBBBBBBBBBBB
                       A=20 B=20 C=20 
```

#### Caso de teste 6b
Tendo em vista que no _guided schedule_ o tamanho do _chunk_ delimita o mínimo tamanho do bloco de iterações a ser delegado para as _threads_, desejou-se observar o comportamento do programa utilizando o valor _default_ para o tamanho do _chunk size_, o que equivale a 1.

```
* Case 6b: using omp critical but chunk_size is the default value used by OpenMP
	schedule_type: omp_sched_guided, chunk_size: 1 (default)
   CAAAAAAAAAAAAAAAAAAAAAAAAAACCCCCCCCCCCCCCCCCCCBBBBBBBBBBBBBB
                       A=26 B=14 C=20 
```

Aqui o _chunk size_ diz respeito ao tamanho mínimo do _chunk_ a ser criado. Similar ao Caso 4b, ao utilizar o valor _default_, o resultado obtido é incorreto.

<!-- Auto -->
### _Auto Schedule_
Em geral, a decisão sobre o tipo de escalonamento será feita pelo compilador. Utilizando o _auto schedule_ o compilador fica livre para escolher qualquer mapeamento de iterações possível para as _threads_ que fazem parte do time. O argumento de _chunk size_ é ignorado com este tipo de escalonamento.

#### Caso de teste 7
O Caso 7 utiliza _auto schedule_ sem exclusão mútua, note que para esse tipo de escalonamento a definição prévia de _chunk size_, passada por parâmetro para a rotina '_omp\_set\_schedule()_', não é utilizada.

```
* Case 7: no omp critical (expecting wrong results)
	schedule_type: omp_sched_auto, chunk_size:  - 
   ACBABCABCABCABCABCABCACBABCABCACBACBACBACBACBACBACBACBACBA--
                       A=20 B=19 C=19 
```

#### Caso de teste 8
Conforme observado abaixo, fazendo uso da diretiva '_omp critical_' para definição da região que deve ser executada de maneira serializada, obteve-se um resultado correto.

```
* Case 8: using omp critical (expecting correct results)
	schedule_type: omp_sched_auto, chunk_size:  - 
   CAAAAAAAAAAAAAAAAAAAACCCCCCCCCCCCCCCCCCCBBBBBBBBBBBBBBBBBBBB
                       A=20 B=20 C=20 
```

Como o tipo de escalonamento _auto_ ignora o argumento _chunk size_, não é necessário realizar um caso de teste com valor o _default_.

<!-- REFERÊNCIAS -->
<a name="referencias"></a>
## Referências 
- Deep Learning Garden. <i>Parallel programming on Ubuntu using OpenMP with C/C++</i>. https://goo.gl/X9Wpj2
- FSU Department of Science Computing. <i>C++ Examples of Parallel Programming with OpenMP</i>. https://goo.gl/sqmujr
- GCC GNU. <i>omp_set_schedule – Set the runtime scheduling method</i>. https://goo.gl/X8gEZW
- IBM Knowledge Centet. <i>omp_get_schedule(kind, modifier)</i>. https://goo.gl/NqrPJq
- IBM Knowledge Center. <i>omp_set_schedule</i>. https://goo.gl/KrqFCB
- IBM Knowledge Center. <i>omp_set_schedule(kind, modifier)</i>. https://goo.gl/4kVuQy
- Intel Software - Developer Zone. <i>OpenMP* Loop Scheduling</i>. https://goo.gl/NkZ2Vv
- Jaka's Corner. <i>OpenMP: For & Scheduling</i>. https://goo.gl/Ubdmai
- Joel Yliluoma. <i>Guide into OpenMP: Easy multithreading programming for C++</i>. https://goo.gl/PhMVXW
- Lawrence Livermore National Laboratory. <i>OpenMP</i>. https://goo.gl/o2wTxR
- Mark Bull. <i>OpenMP Tips, Tricks and Gotchas</i>. https://goo.gl/L9Xhyp
- OpenMP. <i>OpenMP C and C++ Application Program Interface</i>. https://goo.gl/wPbQCn
- OpenMP. <i>Summary of OpenMP 3.0 C/C++ Syntax</i>. https://goo.gl/VdvSpi
- OpenMP Forum. <i>Schedule and Chunk Size</i>. https://goo.gl/wHz3Sp
- The Ultimate Computer Technology Blog. <i>Simple Tutorial with OpenMP: How to Use Parallel Block in C/C++ using OpenMP?</i>. https://goo.glmDnE5H
- UFSC Departamento de Informática e Estatística. <i>OpenMP Scheduling</i>. https://goo.gl/dB2Hcd
