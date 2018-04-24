#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "list.h"
#include "ctimer.h"

typedef struct{
    int tam;
    int num_arvores;
    int num_queimando;
    int num_queimadas;
    list_t* queimando;
    list_t* queimadas;
}floresta_t;

//Variaveis globais
double prob = 0;

floresta_t cria_floresta(int tam){
    floresta_t f;
    f.tam = tam;
    f.num_arvores = tam * tam;
    f.num_queimadas = 0;
    f.num_queimando = 0;
    f.queimando = list_create();
    f.queimadas = list_create();
    return f;
}

void destroi_floresta(floresta_t* f){
    list_destroy(f->queimando);
    list_destroy(f->queimadas);
}

/**
 * Retorna um boolean para indicar se arvore vai queimar ou nao
 * */
bool queimou(){
    return rand()%100 < (int)prob;
}

void inicia_fogo(floresta_t* f){
    int meio = f->tam / 2;
    position_t pos = get_pos(meio, meio);
    f->queimadas = list_add(f->queimadas, pos);
    f->num_queimadas++;
    f->queimando = list_add(f->queimando, get_pos(pos.i-1, pos.j));
    f->queimando = list_add(f->queimando, get_pos(pos.i, pos.j+1));
    f->queimando = list_add(f->queimando, get_pos(pos.i+1, pos.j));
    f->queimando = list_add(f->queimando, get_pos(pos.i, pos.j-1));
    f->num_queimando += 4;
    while (f->num_queimando > 0){
        list_t* prox_queimar = list_create();
        int num_pq = 0;
        while (f->queimando != NULL){ 
            position_t pos = f->queimando->data;
            if (queimou()){
                f->queimadas = list_add(f->queimadas, pos);
                f->num_queimadas++;
                position_t pcima = get_pos(pos.i-1, pos.j);
                if (pcima.i >= 0  &&  (!list_contains(f->queimadas, pcima)&&!list_contains(f->queimando, pcima)&&!list_contains(prox_queimar, pcima))){
                    prox_queimar = list_add(prox_queimar, pcima);
                    num_pq++;
                }
                position_t pdir = get_pos(pos.i, pos.j+1);
                if (pdir.j < f->tam &&(!list_contains(f->queimadas, pdir)&&!list_contains(f->queimando, pdir)&&!list_contains(prox_queimar, pdir))){
                    prox_queimar = list_add(prox_queimar, pdir);
                    num_pq++;
                }
                position_t pbaixo = get_pos(pos.i+1, pos.j);
                if (pbaixo.i < f->tam && (!list_contains(f->queimadas, pbaixo)&&!list_contains(f->queimando, pbaixo)&&!list_contains(prox_queimar, pbaixo))){
                    prox_queimar = list_add(prox_queimar, pbaixo);
                    num_pq++;
                }
                position_t pesq = get_pos(pos.i, pos.j-1);
                if (pesq.j >= 0 && (!list_contains(f->queimadas, pesq)&&!list_contains(f->queimando, pesq)&&!list_contains(prox_queimar, pesq))){
                    prox_queimar = list_add(prox_queimar, pesq);
                    num_pq++;
                }
            }
            f->queimando = f->queimando->next;
        }
        list_destroy(f->queimando);
        f->num_queimando = 0;
        if (num_pq > 0){
            f->queimando = prox_queimar;
            f->num_queimando = num_pq;
        }
    }

}

int main(int argc, char** argv){
    srand(time(NULL));
    int tam_prob = 30;
    int num_exec = 1000;
    int prob_max = 100;
    int prob_min = 0;
    prob = 0;
    if (argc > 1)
        tam_prob = atoi(argv[1]);
    if (argc > 2)
        num_exec = atoi(argv[2]);
    if (argc > 3)
        prob_max = atoi(argv[3]);        
    double inc = (prob_max - prob_min)/(double)(num_exec-1);
    ctimer_t t = timer_start();
    #pragma omp parallel for num_threads(4) schedule(dynamic)
    for (int i = 0; i < num_exec; i++){
        floresta_t f = cria_floresta(tam_prob);
        inicia_fogo(&f);
        printf("Probabilidade: %0.0f, Arvores: %d, Queimadas: %d\n",prob, f.num_arvores, f.num_queimadas);
        destroi_floresta(&f);
        prob += inc;
    }
    timer_end(t);
    return 0;
}
