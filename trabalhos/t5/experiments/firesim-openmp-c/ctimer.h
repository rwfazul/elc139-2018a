#include <sys/time.h>

typedef struct{
    long start, end;
}ctimer_t;

/*
 * Tempo (wallclock) em microssegundos
 */ 
long wtime(){
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return tv.tv_sec*1000000 + tv.tv_usec;
}

ctimer_t timer_start(){
    ctimer_t t;
    t.start = wtime();
    return t;
}

void timer_end(ctimer_t t){
    t.end = wtime();
    printf("Tempo de execução: %ld usec\n", (long) (t.end - t.start));
    t.start = 0;
    t.end = 0;
}