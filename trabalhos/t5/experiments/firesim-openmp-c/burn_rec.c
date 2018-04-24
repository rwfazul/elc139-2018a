#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h> 
#include <time.h>
#include "ctimer.h"

#define ST_INICIAL -1
#define ST_NORMAL 0
#define ST_QUEIMADA 1

typedef struct{
    int l, a, total, queimadas;
    int** pos;
}floresta_t;

typedef struct{
    int i,j;
}pos_t;

//variaveis globais
double prob;

floresta_t* floresta_cria(int largura, int altura){
    floresta_t* f = (floresta_t*)malloc(sizeof(floresta_t));
    f->l = largura;
    f->a = altura;
    f->total = largura*altura;
    f->queimadas = 0;
    f->pos = (int**)malloc(largura*sizeof(int*));
    for(int i = 0; i < largura; i++){
        f->pos[i] = (int*)malloc(altura*sizeof(int));
        for (int j = 0; j < altura; j++){
            f->pos[i][j] = ST_INICIAL;
        }
    }
    return f;
}

floresta_t* floresta_reseta(floresta_t* f){
    f->queimadas = 0;
    for(int i = 0; i < f->l; i++){
        for (int j = 0; j < f->a; j++){
            f->pos[i][j] = ST_INICIAL;
        }
    }
    return f;
}

void floresta_mostra(floresta_t* f){
    for (int i = 0; i < f->l; i++){
        for (int j = 0; j < f->a; j++){
            char c;
            switch(f->pos[i][j]){
                case ST_INICIAL: 
                    c = '#'; 
                    break;
                case ST_NORMAL:
                    c = '@';
                    break;
                case ST_QUEIMADA: 
                    c = '.';
                    break;    
            }
            printf(" %c ",c);
        }
        printf("\n");
    }
    printf("\n");
}


void floresta_destroi(floresta_t* f){
    for (int i = 0; i < f->l; i++){
        free(f->pos[i]);
    }
    free(f->pos);
    free(f);
}

pos_t get_pos(int i, int j){
    pos_t p;
    p.i = i;
    p.j = j;
    return p;
}

pos_t get_centro(floresta_t* f){
    pos_t p;
    p.i = f->l / 2;
    p.j = f->a / 2;
    return p;
}
/**
 * Retorna um boolean para indicar se arvore vai queimar ou nao
 * */
bool queimou(){
    return rand()%100 < prob;
}

void espalha_incendio(floresta_t* f, pos_t p){
    if (queimou()){
        f->pos[p.i][p.j] = ST_QUEIMADA;  
        f->queimadas++;  
        //cima
        if (p.i > 0 && f->pos[p.i-1][p.j] == ST_INICIAL)
            espalha_incendio(f, get_pos(p.i-1, p.j));
        //direita    
        if (p.j < (f->a-1) && f->pos[p.i][p.j+1] == ST_INICIAL)
            espalha_incendio(f, get_pos(p.i, p.j+1));
        //baixo
        if (p.i < (f->l-1) && f->pos[p.i+1][p.j] == ST_INICIAL)
            espalha_incendio(f, get_pos(p.i+1, p.j));
        //esquerda    
        if (p.j > 0 && f->pos[p.i][p.j-1] == ST_INICIAL)
            espalha_incendio(f, get_pos(p.i, p.j-1));                
    }else{
        f->pos[p.i][p.j] = ST_NORMAL;    
    }
}

void comeca_incendio(floresta_t* f){
    pos_t c = get_centro(f);
    f->pos[c.i][c.j] = ST_QUEIMADA;
    f->queimadas++;
    pos_t adj[4];
    adj[0] = get_pos(c.i-1, c.j); //cima
    adj[1] = get_pos(c.i, c.j+1); //direita
    adj[2] = get_pos(c.i+1, c.j); //baixo
    adj[3] = get_pos(c.i, c.j-1); //esquerda
    for (int i = 0; i < 4; i++){
        if (f->pos[adj[i].i][adj[i].j] == ST_INICIAL)
            espalha_incendio(f, adj[i]);
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
        floresta_t* f = floresta_cria(tam_prob, tam_prob);
        comeca_incendio(f);
        printf("Probabilidade: %0.0f, Arvores: %d, Queimadas: %d\n",prob, f->total, f->queimadas);
        floresta_destroi(f);
        prob += inc;
    }
    timer_end(t);
    return 0;
}