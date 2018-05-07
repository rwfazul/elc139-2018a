#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
#include "util.h"

#define TAM_PADRAO 4

typedef struct{
    int** pos;
    int tam;
}tabuleiro_t;

/* wtime */
long wtime() {
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec*1000000 + t.tv_usec;
}

tabuleiro_t* cria_tab(int tam){
    tabuleiro_t* t = (tabuleiro_t*)malloc(sizeof(tabuleiro_t));
    t->tam = tam;
    t->pos = (int**)malloc(t->tam*sizeof(int*));
	for (int i = 0; i < t->tam; i++){
		t->pos[i] = (int*)malloc(t->tam*sizeof(int));
		for (int j = 0; j < t->tam; j++){
			t->pos[i][j] = 0;
		}
	}
    return t;
}

void destroi_tab(tabuleiro_t* t){
    for (int i = 0; i < t->tam; i++)
		free(t->pos[i]);
	free(t->pos);
    free(t);
}

void imprime_tab(tabuleiro_t* t){
    for (int i = 0; i < t->tam; i++){
		for(int j = 0; j < t->tam; j ++){
			printf("%d ", t->pos[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

bool verifica_pos(tabuleiro_t* t, position_t pos){
    int k, l, m;
	for (k = pos.j-1, l= pos.i-1, m = pos.i+1; k >= 0; k--, l--, m++){
		if (l >= 0 && t->pos[l][k] == 1)
			return false;
		if (t->pos[pos.i][k])
			return false;
		if (m < t->tam && t->pos[m][k] == 1)
			return false;
	}
	return true;
}

void marca_pos(tabuleiro_t* t, position_t pos){
	for (int k = 0; k < t->tam; k++)
		t->pos[k][pos.j] = (k == pos.i) ? 1 : 0;
}

bool ultima_pos(tabuleiro_t* t, position_t pos){
    return (pos.i == t->tam-1 && pos.j == t->tam-1);
}

void limpa_col(tabuleiro_t* t, int j){
	for (int i = 0; i < t->tam; i++)
		t->pos[i][j] = 0;
}

bool tem_prox(tabuleiro_t* t, position_t pos){
    return (pos.i < t->tam-1);
}

position_t get_prox(position_t pos){
    position_t prox;
    prox.i = pos.i+1;
    prox.j = pos.j;
    return prox;
}

int resolve(tabuleiro_t* t, position_t p_ini){
    int sol = 0; //numero de soluções
    stack_t* bs = stack_create(); //pilha para voltar
    stack_push(bs, p_ini);
    while(!stack_empty(bs)){
        //list_print(bs->l);
        position_t pos = statck_pop(bs);
        limpa_col(t, pos.j);
        int check = t->tam-pos.j;
        for (int j = pos.j; j < t->tam; j++){
            bool ver = false;//indica se ele achou uma posicao ok
            for (int i = (j == pos.j)?pos.i:0; i < t->tam; i++){
                position_t vpos = get_pos(i, j);
                if (verifica_pos(t, vpos)){
                    marca_pos(t, vpos);
                    ver = true;
                    if (tem_prox(t, vpos)){
                        stack_push(bs, get_prox(vpos));
                    }
                    check--;
                    break;//passa pra próxima coluna
                }
            }
            if (!ver){
                //volta uma posição salva
                break;
            }
        }
        if (check == 0)
            sol++;
        //imprime_tab(t);
    }
    stack_destroy(bs);
    return sol;
}

int main(int argc, char** argv){
    int tam = TAM_PADRAO;
    long start_time, end_time, t1, t2;
    if (argc > 1){
        tam = atoi(argv[1]);
    }
    int sol;
    //Execução serial
    sol = 0;
    start_time = wtime();
    {
        tabuleiro_t* tab = cria_tab(tam);
        int res = resolve(tab, get_pos(0, 0));
        sol += res;
        destroi_tab(tab);
    }
    end_time = wtime();
    t1 = (end_time - start_time);
	printf("Nrainhas   (serial) - size: %d; time: %ld; solutions: %d\n", 
			 tam, (long) (end_time - start_time), sol);
    //Execução em paralelo         
    sol = 0;
    start_time = wtime();
    #pragma omp parallel shared(sol) num_threads(tam)
    {
        tabuleiro_t* tab = cria_tab(tam);
        tab->pos[omp_get_thread_num()][0] = 1;
        int res = resolve(tab, get_pos(0, 1));
        #pragma omp critical
        sol += res;
        destroi_tab(tab);
    }
    end_time = wtime();
    t2 = (end_time - start_time);
	printf("Nrainhas (paralelo) - size: %d; time: %ld; solutions: %d\n", 
			 tam, (long) (end_time - start_time), sol);
    printf("Speedup: %0.02f\n", (double)t1/(double)t2);
    return 0;
}