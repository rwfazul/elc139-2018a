/*
 *  Exemplo de programa para calculo de produto escalar em paralelo, usando MPI.
 *  rwfazul@inf.ufsm.br
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "mpi.h"

typedef struct {
   double *a;
   double *b;
   int wsize;
   int repeat; 
 } dotdata_t;

dotdata_t dotdata;

void check_parameters(int argc, char **argv, int *wsize, int *repeat);
double dotprod_mpi(int wsize, int repeat);
void create_arrays(int wsize, int repeat);
void fill(double *a, int size, double value);
long wtime();
void show_info(double total_dotprod, int wsize, int p, long start_time, long end_time);
void check_result(double total_dotprod, int wsize, int qtd_proc);

#define VALUE_ARRAY_A 		0.01
#define VALUE_ARRAY_B 		1

// Funcao principal
int main(int argc, char **argv) {
	int myrank;         // "rank" do processo (0 a P-1)
	int p;              // número de processos
	int source;         // "rank" do processo remetente
	int dest = 0;       // "rank" do processo destinatário
	int tag = 0;        // "etiqueta" da mensagem
	double result;      // a mensagem (resultado do calculo)
	MPI_Status status;  // "status" de uma operação efetuada
	int wsize, repeat;
	long start_time, end_time;

	start_time = wtime();

	// Criacao e controle da estrutura master-slave
	// MPI_Init deve ser invocado antes de qualquer outra chamada MPI
	MPI_Init(&argc, &argv);
	// Descobre o "rank" do processo
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	// Descobre o número de processos
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	check_parameters(argc, argv, &wsize, &repeat);

	if (myrank != 0) { 	/* SLAVES */
		result = dotprod_mpi(wsize, repeat);
		// envia mensagem ao processo 0
		MPI_Send(&result, 1, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD); // 1 = qtd elementos
	} else {            /* MASTER */
		double total_dotprod = 0;
		for(source = 1; source < p; source++) {
			// recebe P-1 mensagens
			MPI_Recv(&result, 1, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &status); 
			total_dotprod += result;
		}
		end_time = wtime();
	    show_info(total_dotprod, wsize, p, start_time, end_time);
	}
	MPI_Finalize(); // finaliza MPI

   return EXIT_SUCCESS;
}

// Valida argumentos do programa
void check_parameters(int argc, char **argv, int *wsize, int *repeat) {
	if ( (argc != 3) ) {
		fprintf(stderr,"Usage: %s <worksize> <repetitions>\n", argv[0]);
		exit(EXIT_FAILURE);
	} 
	*wsize = atoi(argv[1]);
	*repeat = atoi(argv[2]);
	if ( (*wsize <= 0) || (*repeat <= 0) ) {
		fprintf(stderr, "%s: worksize and repetitions should be > 0.\n", argv[0]);
		exit(EXIT_FAILURE);
	}
}

// Funcao do calculo do produto escalar executada pelos processos 'slaves'
double dotprod_mpi(int wsize, int repeat) {
	create_arrays(wsize, repeat);
	double *a = dotdata.a;
    double *b = dotdata.b;  
    double mysum;
    for (int k = 0; k < repeat; k++) {
    	mysum = 0.0;
    	for (int i = 0; i < wsize; i++) {
        	mysum += (a[i] * b[i]);
    	}
    }
	free(dotdata.a);
	free(dotdata.b);
	return mysum; 
}

// Cria vetores (cada processo tem suas proprias copias de cada variavel, incluindo globais)
void create_arrays(int wsize, int repeat) {
	dotdata.a = (double*) malloc(wsize * sizeof(double));
	fill(dotdata.a, wsize, VALUE_ARRAY_A);
	dotdata.b = (double*) malloc(wsize * sizeof(double));
	fill(dotdata.b, wsize, VALUE_ARRAY_B);
	dotdata.wsize = wsize;
	dotdata.repeat = repeat;
}

// Preenche vetor
void fill(double *a, int size, double value) {  
	for (int i = 0; i < size; i++)
		a[i] = value;
}

// Tempo (wallclock) em microssegundos 
long wtime() {
	struct timeval t;
	gettimeofday(&t, NULL);
	return t.tv_sec*1000000 + t.tv_usec;
}

// Mostra resultado e estatisticas da execucao
void show_info(double total_dotprod, int wsize, int p, long start_time, long end_time) {
	printf("dot product: %f\n", total_dotprod);
    check_result(total_dotprod, wsize, p);
    printf("number of processes: %d, wall time: %ld usec\n", p, (long) (end_time - start_time));
    fflush(stdout);
}

// Confere resultado do produto escalar dado respectivo wsize e qtd de processos
void check_result(double total_dotprod, int wsize, int qtd_proc) {
	char *awnser[] = { "wrong", "correct" };
	double expected_result =  VALUE_ARRAY_A * VALUE_ARRAY_B * wsize * (qtd_proc - 1);
	int awnser_is_ok = ((int) total_dotprod == (int) expected_result);
	printf("the result is %s", awnser[awnser_is_ok]);
	(awnser_is_ok) ? printf("\n") : printf(" (espected value is %f)\n", expected_result);
}