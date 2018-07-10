/*
 *  Exemplo de programa para calculo de produto escalar em paralelo, usando POSIX threads.
 *  professora: andrea@inf.ufsm.br
 *  aluno: rwfazul@inf.ufsm.br
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#define TEST_MODE

typedef struct {
   double *a;
   double *b;
   double c; 
   int wsize;
   int repeat; 
   int nthreads;
} dotdata_t;

// Variaveis globais, acessiveis por todas threads
dotdata_t dotdata;
pthread_mutex_t mutexsum;

/*
 * Funcao executada por uma thread
 */
void *dotprod_worker(void *arg) {
   int i, k;
   long offset = (long) arg;
   double *a = dotdata.a;
   double *b = dotdata.b;     
   int repeat = dotdata.repeat;
   int wsize = dotdata.wsize;
   int slice = (int) wsize / dotdata.nthreads;
   int start = offset * slice;
   int end = (offset < dotdata.nthreads - 1) ? start + slice : wsize;
   double mysum;

   for (k = 0; k < repeat; k++) {
      mysum = 0.0;
      for (i = start; i < end ; i++)  {
         mysum += (a[i] * b[i]);
      }
   }

   pthread_mutex_lock(&mutexsum);
   dotdata.c += mysum;
   pthread_mutex_unlock(&mutexsum);

   pthread_exit((void*) 0);
}


/* 
 * Distribui o trabalho entre nthreads
 */
void dotprod_threads(int nthreads) {
   long int i;
   pthread_t *threads;
   pthread_attr_t attr;

   threads = (pthread_t *) malloc(nthreads * sizeof(pthread_t));
   pthread_mutex_init(&mutexsum, NULL);

   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   for (i = 0; i < nthreads; i++)
      pthread_create(&threads[i], &attr, dotprod_worker, (void *) i);
   pthread_attr_destroy(&attr);
   for (i = 0; i < nthreads; i++)
      pthread_join(threads[i], NULL);
   free(threads);
}


/*
 * Tempo (wallclock) em microssegundos
 */ 
long wtime() {
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec*1000000 + t.tv_usec;
}

/*
 * Preenche vetor
 */ 
void fill(double *a, int size, double value) {  
   int i;
   for (i = 0; i < size; i++)
      a[i] = value;
}

/*
 * Funcao principal
 */ 
int main(int argc, char **argv) {
   int nthreads, wsize, repeat;
   long start_time, end_time;

   if ((argc != 4)) {
      printf("Uso: %s <nthreads> <worksize> <repetitions>\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   nthreads = atoi(argv[1]); 
   wsize = atoi(argv[2]);  // worksize = tamanho do vetor de cada thread
   repeat = atoi(argv[3]); // numero de repeticoes dos calculos (para aumentar carga)

   // Cria vetores
   dotdata.a = (double *) malloc(wsize * sizeof(double));
   fill(dotdata.a, wsize, 0.01);
   dotdata.b = (double *) malloc(wsize * sizeof(double));
   fill(dotdata.b, wsize, 1.0);
   dotdata.c = 0.0;
   dotdata.wsize = wsize;
   dotdata.repeat = repeat;
   dotdata.nthreads = nthreads;

   // Calcula c = a . b em nthreads, medindo o tempo
   start_time = wtime();
   dotprod_threads(nthreads);
   end_time = wtime();

   // Mostra resultado e estatisticas da execucao
   #ifndef TEST_MODE
   printf("%f\n", dotdata.c);
   printf("%d thread(s), %ld usec\n", nthreads, (long) (end_time - start_time));
   #else
   printf("%ld\n", (long) (end_time - start_time));
   #endif
   
   fflush(stdout);
   free(dotdata.a);
   free(dotdata.b);

   return EXIT_SUCCESS;
}
