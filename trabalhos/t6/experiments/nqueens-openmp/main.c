#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>

#include "nqueens.h"

#define N 5

long wtime();

int main(int argc, char **argv) {
	int size = N;
	int qtd_threads = omp_get_num_procs();
	long start_time, end_time;

	if (argc > 1) size = atoi(argv[1]);
	if (argc > 2) qtd_threads = atoi(argv[2]);
	
	start_time = wtime();
	int solutions = find_queens(size, qtd_threads);
	end_time = wtime();
	
	printf("Nqueens (%d threads); size: %d; time: %ld; solutions: %d\n", 
			 qtd_threads, size, (long) (end_time - start_time), solutions);
	return 0;
}

/* wtime */
long wtime() {
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec*1000000 + t.tv_usec;
}
