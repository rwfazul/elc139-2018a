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
	- [_Dynamic Schedule_](#dynamic-schedule)
		- [Caso de teste 3](#caso-de-teste-3)
		- [Caso de teste 4](#caso-de-teste-4)
		- [Caso de teste 4b](#caso-de-teste-4b)
	- [_Guided Schedule_](#guided_schedule)
		- [Caso de teste 5](#caso-de-teste-5)
		- [Caso de teste 6](#caso-de-teste-6)
		- [Caso de teste 6b](#caso-de-teste-6b)
	- [_Auto Schedule_](#auto_schedule)
		- [Caso de teste 7](#caso-de-teste-7)
		- [Caso de teste 8](#caso-de-teste-8)
- [Referências](#referencias)

<!-- Implementacao -->
## Implementação
Ideia... 
O programa pode ser visto em...

<a name="#critical">
### Seção crítica

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

Onde:

``` cpp 
   void do_operations(char c) {
      array[index] = c; 
      spendSomeTime();
      index++;
   }
```

<!-- Outputs -->
## _Outputs_

A saída pode ser obervada em...

<!-- Runtime -->
### _Runtime Schedule_

O tipo de escalonamento (_static_, _dynamic_ ou _guided_) e o _chunk size_ são definidos em tempo de execução com base em uma varíavel de controle interno (_run-sched-var_), que foi setada anteriormente no programa com a rotina _omp\_set\_schedule_. A figura abaixo ilustra o uso do _runtime schedule_ no programa (é ilegal definir _chunk size_ para este tipo de escalonamento).

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
Neste caso de teste não foi feito uso de exclusão mútua (via _omp critical_, conforme apresentado anteriormente). Foi definido um _chunk_ size equivalente a _nTimes_ (quantidade de inserções a serem realizadas por cada _thread_). O resultado obtido, disponível em [out.txt](ThreadABC/out.txt) é apresentado a seguir:

```
* Case 1: no omp critical (expecting wrong results)
	schedule_type: omp_sched_static, chunk_size: 20
   BABCABCABCAABCABCABCABCAABCABCABCABACABCABCABCABAC-BCBCBCBC-
                       A=19 B=20 C=19 
````

#### Caso de teste 2
Em contraponto, para obtenção de resultado correto com _static schedule_, utilizou-se exclusão mútua:

```
* Case 2: using omp critical (expecting correct results)
	schedule_type: omp_sched_static, chunk_size: 20
   CCCCCCCCCCCCCCCCCCCCBBBBBBBBBBBBBBBBBBBBAAAAAAAAAAAAAAAAAAAA
                       A=20 B=20 C=20 
```

<!-- Dynamic -->
### _Dynamic Schedule_
Cada _thread_ executa um _chunk_ de iterações e, quando completo, outro _chunk_ é dinamicamente atríbuido a mesma. Isso ocorre até todos os _chunks_ serem distríbuidos. Acredita-se que um auto nível de requisições/atribuições de _chunks_ cause um _overhead_ significativo.
  
#### Caso de teste 3
Este caso utiliza o escalonamento dinâmico, sem exclusão mútua e com um _chunk size_ de _nTimes_.

```
* Case 3: no omp critical (expecting wrong results)
	schedule_type: omp_sched_dynamic, chunk_size: 20
   ABACBCABCABBCABCABCABCABBCABCABCABCABBCABCABCABCAB-CACACACA-
                       A=20 B=19 C=19 
```

#### Caso de teste 4
Para ilustar o resultado correto com o _chunk size_ pré-definido realizou-se o Caso 4:


```
* Case 4: using omp critical (expecting correct results)
	schedule_type: omp_sched_dynamic, chunk_size: 20
   ACCCCCCCCCCCCCCCCCCCCBBBBBBBBBBBBBBBBBBBBAAAAAAAAAAAAAAAAAAA
                       A=20 B=20 C=20 
```

#### Caso de teste 4b
Para fins de estudo do comportamento do programa, foi realizado um caso de teste em que o tamanho do _chunk_ não foi previamente definido. Deste modo, o valor _default_ do OpenMP é utilizado, o que equivale a 1.

```
* Case 4b: using omp critical but chunk_size is the default value used by OpenMP
	schedule_type: omp_sched_static, chunk_size: 1 (default)
   BAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCC
                       A=20 B=20 C=20 
```

Observou-se que o resultado gerado foi correto.

<!-- Guided -->
### _Guided Schedule_
Similar ao _dynamic schedule_, mas o _chunk_ começa maior e diminui até _chunk size_ (define o tamanho mínimo do bloco) para compensar algum desequilíbrio de carga entre as iterações. Quando _chunk size_ não é definido, o tamanho inicial do _chunk_ é proporcional a (interações &divide; _nThreads_). Os demais blocos seguem a proporção de (iterações_restantes &divide; _nThreads_).

#### Caso de teste 5
Conforme resultado em [out.txt](ThreadABC/out.txt), o Caso 5 fez uso do _guided schedule_ com definição prévia do _chunk size_ e sem exclusão mútua:

```
* Case 5: no omp critical (expecting wrong results)
	schedule_type: omp_sched_guided, chunk_size: 20
   ACBACBACBACCBACBACBACABCCABCBACBACBACCBACBACBACCBA-BABABABA-
                       A=20 B=19 C=19 
```

#### Caso de teste 6
Este caso foi utilizado para constatar a obtenção do resultado correto utilizando _omp critical_.

```
* Case 6: using omp critical (expecting correct results)
	schedule_type: omp_sched_guided, chunk_size: 20
   BAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCC
                       A=20 B=20 C=20 
```

#### Caso de teste 6b
Tendo em vista que no _guided schedule_ o tamanho do _chunk_ delimita o mínimo tamanho do bloco de iterações a ser delegado para as _threads_, desejou-se observar o comportamento do programa utilizando o valor de _chunk size default_, o que equivale a 1.

```
* Case 6b: using omp critical but chunk_size is the default value used by OpenMP
	schedule_type: omp_sched_static, chunk_size: 1 (default)
   CBBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCAAAAAAAAAAAAAAAAAAAA
                       A=20 B=20 C=20 
```

<!-- Auto -->
### _Auto Schedule_
Em geral, a decisão sobre o tipo de escalonamento será feita pelo compilador. Utilizando o _auto schedule_ o compilador fica livre para escolher qualquer mapeamento de iterações possível para as _threads_ que fazem parte do time. O argumento de _chunk size_ é ignorado.

#### Caso de teste 7
O Caso 7 utiliza _auto schedule_ sem exclusão mútua, note que para esse tipo de escalonamento a definição prévia de _chunk size_, passada por parâmetro para a rotina _omp\_set\_schedule_, não é utilizada.

```
* Case 7: no omp critical (expecting wrong results)
	schedule_type: omp_sched_auto, chunk_size:  - 
   ABCABCABCABBCABCABCABCABBCABCABCABCABBCABCABCABCAB-CACACACA-
                       A=20 B=19 C=19 
```

#### Caso de teste 8
Conforme observado abaixo, fazendo uso da diretiva _omp critical_ para definição da região que deve ser executada de maneira serializada, obteve-se um resultado correto.

```
* Case 8: using omp critical (expecting correct results)
	schedule_type: omp_sched_auto, chunk_size:  - 
   CAAAAAAAAAAAAAAAAAAAACCCCCCCCCCCCCCCCCCCBBBBBBBBBBBBBBBBBBBB
                       A=20 B=20 C=20 
```

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
