[Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2018a) > Trabalhos

# T8: Comunicação coletiva em MPI

Disciplina: ELC139 - Programação Paralela <br/>
Aluno: Rhauani Weber Aita Fazul

## Parte 1

+  No código fornecido, são feitas várias chamadas de send/recv que podem ser trocadas por outras funções. Você deve descobrir quais são essas funções, fazer a alteração e testar se o resultado é o mesmo. (O programa gera sempre as mesmas matrizes, então o resultado deve ser sempre o mesmo).

### Primeira Solução

O programa [matrix_mult_sr_alt1.c](matrix_mult/matrix_mult_sr_alt1.c) utiliza as rotinas _MPI\_Bcast_(), _MPI\_Scatter_() e _MPI\_Gather_() para aplicar a lógica do programa.

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
	MPI_Scatter(void* send_data, int send_count, MPI_Datatype send_datatype, void* recv_data, int recv_count, MPI_Datatype recv_datatype, int root, MPI_Comm communicator);
```

Para esta matriz apenas as linhas atribuídas a cada processo precisam ser enviadas, logo o número de elementos a serem enviados é definido por (número total de elementos &divide; número de processos do grupo). 

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

<p>Pereceba que, com o _buffer_ não posicionado, as linhas da matriz A são preenchidas a partir do índice 0. Como, nessa versão, o número de linhas é proporcional ao número de processos, o laço percorre de 0 ao número de linhas atribuídas a cada processo (fatia) definido pelo (número total de linhas da matriz &divide; número de processos do grupo).</p>

Por fim, a coleta do resultado (matriz C) é realizada:

``` c
    MPI_Gather(void* send_data, int send_count, MPI_Datatype send_datatype, void* recv_data, int recv_count, MPI_Datatype recv_datatype, int root, MPI_Comm communicator);
```

Similar ao _MPI\_Scatter_, o _MPI\_Gather_ pega os elementos de cada processo e os reúne no processo _root_, este que é o único processo que necessita ter um _buffer_ de recebimento válido.
    
``` c
    MPI_Gather(C, SIZE * SIZE / nproc, MPI_INT, C, SIZE * SIZE / nproc, MPI_INT, 0, MPI_COMM_WORLD);
```

Nesta solução o _buffer_ de envio também não foi posicionado. Note que o parâmetro referente a _recv\_count_ é o contador de elementos recebidos por processo, não o total de todos os processos, logo (número total de elementos &divide; número de processos do grupo). 
        
### Segunda Solução

O programa [matrix_mult_sr_alt2.c](matrix_mult/matrix_mult_sr_alt2.c) utiliza as mesmas rotinas da solução anterior, com algumas diferenças em seu uso.

O _broadcast_ da matriz B é feito da mesma forma. A diferença está no posicionamento dos _buffers_ nas funções de distribuição e coleta.

``` c
	MPI_Scatter(A, SIZE * SIZE / nproc, MPI_INT, &A[from], SIZE * SIZE / nproc, MPI_INT, 0, MPI_COMM_WORLD);
```
Note que agora o _buffer_ de recebimento é posicionado de acordo com o _rank_ do processo, onde _from_ = _rank_ do processo * número de linhas da matriz / quantidade de processos no grupo. Logo, p
ara realizar as multiplicações entre as matrizes utiliza-se a seguinte lógica:

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
<p>Pereceba que, com o _buffer_ posicionado, as linhas da matriz A são preenchidas a partir do índice referente ao calculo baseado no _rank_ do processo. Como, nessa versão, o número de linhas é proporcional ao número de processos, o laço percorre desse valor (_from_) até este valor acrescido do número de linhas atribuídas a cada processo (fatia). Isto também poed ser escrito da forma _to_ = (_rank_ do processo + 1) &plus; número total de linhas da matriz &divide; número de processos do grupo.</p>

Nesta solução, a coleta também é feita com o posicionamento do _buffer_ de envio.

``` c
	MPI_Gather(C[from], SIZE * SIZE / nproc, MPI_INT, C, SIZE * SIZE / nproc, MPI_INT, 0, MPI_COMM_WORLD);
```

Sendo assim, os elementos definidos em _send\_count_ são enviados a partir da posição definida na varíavel _from_ de cada processo.

## Parte 2

+ Implemente também uma versão usando quantidades de envio/recebimento variáveis, o que permite que o número de linhas da matriz A não precise ser proporcional ao número de processos.

### Primeira Solução

### Segunda Solução

## Referências
http://www.math-cs.gordon.edu/courses/cps343/presentations/MPI_Collective.pdf
