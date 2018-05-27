/*
 *  Exemplo de programa MPI que transporta uma mensagem em um pipeline formado por processos de 0 a NP-1 
 *  rwfazul@inf.ufsm.br
 */

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

void print_info_start(int p);
void print_info_send(int myrank, int dest, int msg);
void print_info_receive(int myrank, int source, int msg);

/* Com esta opcao ativa todo a comunicacao realizada entre os processos sera detalhada.
Os prints do trace podem ocorrem em ordem invertida, isto NAO ocorre por erros na
comunicacao entre os processos e sim pelo fato de que multiplos processadores podem realizar 
seus respectivos outputs no console ao mesmo tempo e um delay na pool pode vir a inverter
a ordem do print das mensagens no terminal */
#define PRINT_TRACE    // se definido = verbose

// Funcao principal
int main(int argc, char **argv) {
    int myrank;          // "rank" do processo (0 a P-1)
    int p;               // número de processos
    int source;          // "rank" do processo remetente
    int dest;            // "rank" do processo destinatário
    int tag = 0;         // "etiqueta" da mensagem
    int msg;	         // a mensagem (numero inteiro)
    MPI_Status status;   // "status" de uma operação efetuada


    // MPI_Init deve ser invocado antes de qualquer outra chamada MPI
    MPI_Init(&argc, &argv);
    // Descobre o "rank" do processo
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    // Descobre o número de processos
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (myrank == 0) {
        print_info_start(p);
        msg = 0;
        dest = myrank + 1;
        print_info_send(myrank, dest, msg);
        MPI_Send(&msg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD); // 1 = qtd elementos
    } else if ( myrank == (p - 1) ) {
        source = myrank - 1;
        MPI_Recv(&msg, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status); 
        print_info_receive(myrank, source, msg);
        printf("[FIM] Processo %d obteve o resultado %d\n\n", myrank, msg);
    } else {
        source = myrank - 1;
        MPI_Recv(&msg, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status); 
        print_info_receive(myrank, source, msg);
        msg++;
        dest = myrank + 1;
        print_info_send(myrank, dest, msg);
        MPI_Send(&msg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD); // 1 = qtd elementos
    }

    MPI_Finalize(); // finaliza MPI

    return EXIT_SUCCESS;
}

void print_info_start(int p) {
    printf("\n\t*** Pipeline de %d processos (msg inicia em 0) ***\n\n", p);
    #ifdef PRINT_TRACE
    printf("\t-- Verbose mode (PRINT_TRACE esta definido) --\n");
    #endif
}

void print_info_send(int myrank, int dest, int msg) {
    #ifdef PRINT_TRACE
    printf("Processo [%d] envia mensagem para Processo [%d]. Msg = %d\n", myrank, dest, msg);
    #endif
}

void print_info_receive(int myrank, int source, int msg) {
    #ifdef PRINT_TRACE
    printf("\t-> Processo [%d] recebe mensagem do Processo [%d]. Msg = %d\n", myrank, source, msg);
    #endif
}
