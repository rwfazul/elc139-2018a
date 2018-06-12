[Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2018a) > Trabalhos

# T8: Comunicação coletiva em MPI

Disciplina: ELC139 - Programação Paralela <br/>
Aluno: Rhauani Weber Aita Fazul

## Sumário

- [Parte 1](#parte-1)
	- [Primeira Solução](#parte1-primeira-solucao)
	- [Segunda Solução](#parte1-segunda-solucao)
- [Parte 2](#parte-2)
	- [Primeira Solução](#parte2-primeira-solucao)
	- [Segunda Solução](#parte2-segunda-solucao)
- [Referências](#referências)

<!-- PARTE 1 -->
## Parte 1

+  No código fornecido, são feitas várias chamadas de send/recv que podem ser trocadas por outras funções. Você deve descobrir quais são essas funções, fazer a alteração e testar se o resultado é o mesmo. (O programa gera sempre as mesmas matrizes, então o resultado deve ser sempre o mesmo).

<a name="parte1-primeira-solucao"></a>
### Primeira Solução

O programa [matrix_mult_sr_alt1.c](matrix_mult/matrix_mult_sr_alt1.c) utiliza as rotinas _MPI\_Bcast_(), _MPI\_Scatter_() e _MPI\_Gather_() para aplicar a lógica da multiplicação.

Primeiramente, é realizado a difusão da matriz B, seguindo o protótipio:

``` c
MPI_Bcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator);
```

Neste caso, o processo com _rank_ definido pelo parâmetro _root_ irá enviar o mesmo dado (numéro de entradas (_count_) a partir do endereço inicial do _buffer_ _data_), para todos os processos do grupo (definidos pelo comunicador). Logo, seguindo o escopo do programa:

``` c
MPI_Bcast(B, SIZE * SIZE, MPI_INT, 0, MPI_COMM_WORLD);
```

_MPI\_Bcast_ possui vantagens em relação ao uso de _Send/Recv_. Ao invés de usar apenas um canal de comunicação para envio dos dados (origem -> destino), o _MPI\_Bcast_ utiliza uma comunicação baseada em árvore, que funciona como um _pipeline_ onde até mesmo os processos que não são a origem (_root_) ajudam a difundir os dados para os processos restantes.

Em seguida, a matriz A é distribuída seguindo o protótipo:

``` c
MPI_Scatter(void* send_data, int send_count, MPI_Datatype send_datatype, void* recv_data, 
            int recv_count, MPI_Datatype recv_datatype, int root, MPI_Comm communicator);
```

Para esta matriz, apenas as linhas atribuídas a cada processo precisam ser enviadas, logo o número de elementos a serem enviados é definido pelo número total de elementos &divide; número de processos do grupo, ou seja, (_SIZE_ * _SIZE_ / _nproc_), onde a multiplicação das dimensões da matriz equivalem ao número total de elementos. 

``` c
MPI_Scatter(A, SIZE * SIZE / nproc, MPI_INT, A, SIZE * SIZE / nproc, MPI_INT, 0, MPI_COMM_WORLD);
```

_MPI\_Scatter_ envia _chuncks_ do _buffer_ de envio (matriz A) para os diferentes processos, mesmo que o processo _root_ (no caso, _rank_ = zero) contenha todos os dados existentes na matriz A, a função irá copiar apenas os elementos apropriados para o _buffer_ de recebimento (no caso, também é a matriz A). 

Nessa versão do programa o _buffer_ de recebimento não é posicionado de acordo com o _rank_ de cada processo. Logo, para realizar as multiplicações entre as matrizes utiliza-se a seguinte lógica:

``` c
  for (i = 0; i < SIZE / nproc; i++) {
      for (j = 0; j < SIZE; j++) {
          C[i][j] = 0;
          for (k = 0; k < SIZE; k++) {
              C[i][j] += A[i][k] * B[k][j];
          }
      }
  }
```

<p>Perceba que, com o <em>buffer</em> não posicionado, as linhas da matriz A são preenchidas a partir do índice 0. Como, nessa versão, o número de linhas é proporcional ao número de processos, o laço percorre de 0 ao número de linhas atribuídas a cada processo (fatia), definido pelo número total de linhas da matriz &divide; número de processos do grupo, ou seja, (<em>SIZE</em> / <em>nproc</em>).</p>

Por fim, a coleta do resultado (matriz C) é realizada:

``` c
MPI_Gather(void* send_data, int send_count, MPI_Datatype send_datatype, void* recv_data, 
           int recv_count, MPI_Datatype recv_datatype, int root, MPI_Comm communicator);
```

Similar ao _MPI\_Scatter_, o _MPI\_Gather_ pega os elementos de cada processo e os reúne no processo _root_, este que é o único processo que necessita ter um _buffer_ de recebimento válido.
    
``` c
MPI_Gather(C, SIZE * SIZE / nproc, MPI_INT, C, SIZE * SIZE / nproc, MPI_INT, 0, MPI_COMM_WORLD);
```

Nesta solução o _buffer_ de envio também não foi posicionado. Note que o parâmetro referente a _recv\_count_ é o contador de elementos recebidos por processo, não o total de todos os processos, logo este valor equivale ao número total de elementos &divide; número de processos do grupo. 
        
<a name="parte1-segunda-solucao"></a>        
### Segunda Solução

O programa [matrix_mult_sr_alt2.c](matrix_mult/matrix_mult_sr_alt2.c) utiliza as mesmas rotinas da solução anterior, com algumas diferenças em seu uso.

O _broadcast_ da matriz B é feito da mesma forma. A diferença está no posicionamento dos _buffers_ nas funções de distribuição e coleta.

``` c
MPI_Scatter(A, SIZE * SIZE / nproc, MPI_INT, &A[from], SIZE * SIZE / nproc, MPI_INT, 0, MPI_COMM_WORLD);
```
Note que agora o _buffer_ de recebimento é posicionado de acordo com o _rank_ do processo, onde _from_ = (_rank_ do processo * número de linhas da matriz / quantidade de processos no grupo), ou seja, _from_ = (_myrank_ * _SIZE_ / _nproc_). Logo, para realizar as multiplicações entre as matrizes utiliza-se a seguinte lógica:

``` c
  for (i = from; i < to; i++) {
      for (j = 0; j < SIZE; j++) {
          C[i][j] = 0;
          for (k = 0; k < SIZE; k++) {
              C[i][j] += A[i][k] * B[k][j];
          }
      }
  }
```
<p>Perceba que, com o <em>buffer</em> posicionado, as linhas da matriz A são preenchidas a partir do índice referente ao cálculo baseado no <em>rank</em> do processo. Como, nessa versão, o número de linhas é proporcional ao número de processos, o laço percorre desse valor (<em>from</em>) até este valor acrescido do número de linhas atribuídas a cada processo (fatia). Isto também pode ser escrito da forma <em>to</em> = ((<em>rank</em> do processo + 1) &plus; número total de linhas da matriz &divide; número de processos do grupo), ou seja, <em>to</em> = ((<em>myrank</em> + 1) * <em>SIZE</em> / <em>nproc</em>).</p>

Nesta solução, a coleta também é feita com o posicionamento do _buffer_ de envio.

``` c
MPI_Gather(C[from], SIZE * SIZE / nproc, MPI_INT, C, SIZE * SIZE / nproc, MPI_INT, 0, MPI_COMM_WORLD);
```

Sendo assim, o número de elementos definidos em _send\_count_ são enviados a partir da posição definida na varíavel _from_ de cada processo.

<!-- PARTE 2 -->
## Parte 2

+ Implemente também uma versão usando quantidades de envio/recebimento variáveis, o que permite que o número de linhas da matriz A não precise ser proporcional ao número de processos.

<a name="parte2-primeira-solucao"></a>
### Primeira Solução

O programa [matrix_mult_sr_alt3.c](matrix_mult/matrix_mult_sr_alt3.c) utiliza as rotinas _MPI\_Scatterv_() e _MPI\_Gatherv_(), que extendem as operações apresentadas anteriormente ao permitir o envio de uma quantidade de dados varíaveis para cada processo. Isso permite que o número de linhas da matriz A não precise necessariamente ser divisível pelo número de processos.

Para tal, a variavel _sendcounts_ (utilizada nas rotinas anteriores) deve ser tornar um vetor, em que cada posição representa o número de elementos a serem enviados para cada processo (sendo que este número pode ser variável). Além disso, um outro vetor, chamado _displs_, é necessário. Esse vetor representa o deslocamento de cada processo em relação ao ínicio do _buffer_ de dados, o que possibilita maior flexibilidade na comunicação.

Para a definição destes valores (número de elementos e deslocamento) de cada processo, a seguinte lógica foi empregada:

``` c
rows_slice = (int) SIZE / nproc; // "fatia" que representa o número base de linhas de cada processo
rest = SIZE % nproc;             // caso o número de linhas não seja proporcional ao número de processos, haverá resto
lastrow = 0;                     // auxilía no controle da última linha mapeada (designada a um processo qualquer)
   
for (i = 0; i < nproc; i++) {   // para cada processo...
    displs[i] = lastrow;        // deslocamento do processo 'i' equivale a última linha mapeada
    lastrow += rows_slice;      // atualiza a última linha mapeada
    sendcounts[i] = rows_slice; // a princípio o processo irá atuar sobre a fatia de linhas base
    if (rest != 0) {            // caso a divisão não seja exata...
        sendcounts[i]++;        // processo 'i' recebe uma linha a mais na sua fatia
        lastrow++;              // atualiza o ponteiro para a ultima linha mapeada
        rest--;                 // uma linha a menos (do resto) para distribuir entre os processos restantes
    } 
    /* IMPORTANTE: agora é necessário converter o número de linhas do processo 'i' para o número
       de elementos que esse processo irá receber pela rotina MPI_Scatterv. Também é necessário 
       converter o deslocamento (que também está baseado no número de linhas) para o número de
       elementos a serem deslocados a partir do inicio do buffer de envio
    */
    sendcounts[i] *= SIZE;     // número de elementos = linhas da matriz * número de elementos em cada linha
    displs[i] *= SIZE;         // número de elementos = linhas da matriz * número de elementos em cada linha
}
```

<p>Agora é possível realizar a distribuição de cada processo seguindo o protótipo:</p>

``` c
MPI_Scatterv(const void *sendbuf, const int *sendcounts, const int *displs, MPI_Datatype sendtype, 
             void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
```

Como os vetores _sendcounts_ e _displs_ já foram devidamente preenchidos e, tendo em vista que, _recvcount_ pode ser baseado na posição do vetor de elementos de cada processo (indexado a partir do _rank_), pode-se utilizar a seguinte chamada:
             
``` c             
MPI_Scatterv(A, sendcounts, displs, MPI_INT, A, sendcounts[myrank], MPI_INT, 0, MPI_COMM_WORLD);
```

Perceba que, nesta versão do programa, o _buffer_ de recebimento não é posicionado de acordo com o _rank_ de cada processo.

Após o envio, a multiplicação pode ser realizada. Agora é necessário transformar os elementos do contator de elementos (_sendcounts_) de cada processo para o número de linhas que este irá atuar. Para isso basta seguir a mesma lógica que já havia sido utilizado anteriormente, onde número de linhas de cada processo = número de elementos do processo &divide; número total de linhas da matriz. Essa conversão está ilustrada na condição de parada do laço abaixo:

``` c
  for (i = 0; i < sendcounts[myrank] / SIZE; i++) {
      for (j = 0; j < SIZE; j++) {
          C[i][j] = 0;
          for (k = 0; k < SIZE; k++) {
              C[i][j] += A[i][k] * B[k][j];
          }
      }
  }
```

<p>Analogamente a rotina <em>MPI_Scatterv</em>(), a rotina <em>MPI_Gatherv</em>() é utilizada:</p>

``` c
MPI_Gatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, 
            const int *recvcounts, const int *displs, MPI_Datatype recvtype, int root, MPI_Comm comm)
```    

Para definir o número de elementos enviados de cada processo, basta acessar o vetor _sendcounts_ na posição referente ao seu _rank_. Logo:

``` c
MPI_Gatherv(C, sendcounts[myrank], MPI_INT, C, sendcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);
```

<a name="parte2-segunda-solucao"></a>
### Segunda Solução

O programa [matrix_mult_sr_alt4.c](matrix_mult/matrix_mult_sr_alt4.c) utiliza as mesmas rotinas da solução anterior, porém posiciona os _buffers_ de envio e recebimento baseando-se no _rank_ de cada processo.

``` c
MPI_Scatterv(A, sendcounts, displs, MPI_INT, &A[from_row], sendcounts[myrank], MPI_INT, 0, MPI_COMM_WORLD)
```

<p>A partir do posicionamento, basta adequar os intervalos de atuação no laço. Para isso utiliza-se as varíaveis <em>from_row</em> e <em>to_row</em>, que são calculadas de forma já apresentada na <a href="#parte1-segunda-solucao">Segunda Solução da Parte 1</a>.</p>

``` c
  for (i = from_row; i < to_row; i++) {
      for (j = 0; j < SIZE; j++) {
          C[i][j] = 0;
          for (k = 0; k < SIZE; k++) {
              C[i][j] += A[i][k] * B[k][j];
          }
      }
  }
```

<p>E, por fim, na coleta também é feito o posicionamento do <em>buffer</em> de envio.</p>

``` c
MPI_Gatherv(C[from_row], sendcounts[myrank], MPI_INT, C, sendcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);
```

## Referências
- Gordon College. <em>MPI Collective communication</em>. https://goo.gl/AfEiy6
- MPI Forum. <em>Examples using MPI_SCATTER, MPI_SCATTERV</em>. https://goo.gl/1kyA7i
- MPI Forum.<em>MPI: A Message-Passing Interface Standard</em>. https://goo.gl/U9sgoM
- MPI Tutorial. <em>MPI Broadcast and Collective Communication</em>. https://goo.gl/kuwVfF
- MPI Tutorial. <em>MPI Scatter, Gather, and Allgather</em>. https://goo.gl/P8d4xN
- MPICH. <em>MPI_Bcast</em>. https://goo.gl/UHy7Sz
- MPICH. <em>MPI_Gather</em>. https://goo.gl/p4Hqta
- MPICH. <em>MPI_Gatherv</em>. https://goo.gl/Ews6EZ
- MPICH. <em>MPI_Scatter</em>. https://goo.gl/3FUt7g
- MPICH. <em>MPI_Scatterv</em>. https://goo.gl/eR7wPq
