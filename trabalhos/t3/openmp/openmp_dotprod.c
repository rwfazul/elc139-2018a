/*
 *  Exemplo de programa para calculo de produto escalar em paralelo, usando OpenMP
 *  professora: andrea@inf.ufsm.br
 *  aluno: rwfazul@inf.ufsm.br
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>

typedef struct 
 {
   double *a;
   double *b;
   double c; 
   int wsize;
   int repeat; 
 } dotdata_t;

// Variavel global, acessivel por todas threads
dotdata_t dotdata;

/*
 * Funcao paralelizada com OpenMP
 */
void dotprod_omp(int nthreads)
{
   int i, k;
   int repetitions = dotdata.repeat;
   int wsize_total = dotdata.wsize * nthreads; // sera particionado entre as threads
   double *a = dotdata.a;
   double *b = dotdata.b;     
   double mysum;

   /* - gera um grupo de nthreads.
      - loop counter (k) nao esta sujeito a uma worksharing directive, logo privado. */ 
   #pragma omp parallel private(k) num_threads(nthreads)
   {
      for (k = 0; k < repetitions; k++) {
         mysum = 0.0;
         /* - divide iterações ( por default: schedule(static) ) deste loop entre os segmentos gerados anteriormente.
            - reduction faz com que uma cópia local da variável de redução seja criada em cada thread. no final da construção paralela do loop, todas threads adicionam o valor local calculado para thread mestre com a variável "global" 'mysum'. */
         #pragma omp for private(i) reduction(+:mysum)
         for (i = 0; i < wsize_total ; i++) {
	    // printf("Thread %d, i=%d\n", omp_get_thread_num(), i); // printa TID, worksize(pos) 
	    mysum += (a[i] * b[i]);
         }
      }
      /* identifica uma seção de código (critical section) que deve ser executada de maneira serializada */
      #pragma omp critical
      {
         dotdata.c += mysum;
      }
   }
}

/*
 * Tempo (wallclock) em microssegundos
 */ 
long wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec*1000000 + t.tv_usec;
}

/*
 * Preenche vetor
 */ 
void fill(double *a, int size, double value)
{  
   int i;
   for (i = 0; i < size; i++) {
      a[i] = value;
   }
}

/*
 * Funcao principal
 */ 
int main(int argc, char **argv)
{
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
   dotdata.a = (double *) malloc(wsize*nthreads*sizeof(double));
   fill(dotdata.a, wsize*nthreads, 0.01);
   dotdata.b = (double *) malloc(wsize*nthreads*sizeof(double));
   fill(dotdata.b, wsize*nthreads, 1.0);
   dotdata.c = 0.0;
   dotdata.wsize = wsize;
   dotdata.repeat = repeat;

   // Calcula c = a . b em nthreads, medindo o tempo
   start_time = wtime();
   dotprod_omp(nthreads);
   end_time = wtime();

   // Mostra resultado e estatisticas da execucao
   printf("%f\n", dotdata.c);
   printf("%d thread(s), %ld usec\n", nthreads, (long) (end_time - start_time));
   fflush(stdout);

   free(dotdata.a);
   free(dotdata.b);

   return EXIT_SUCCESS;
}
