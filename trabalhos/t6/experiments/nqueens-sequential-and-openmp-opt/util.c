#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "util.h"

position_t get_pos(int i, int j){
    position_t pos;
    pos.i = i;
    pos.j = j;
    return pos;
}

list_t* list_create(){
    return NULL;
}

list_t* list_add(list_t* l, position_t pos){
    if (l == NULL){
        l = (list_t*)malloc(sizeof(list_t));
        l->data = pos;
        l->next = NULL;
    }else{
        list_t* cpy = l;
        while (cpy->next != NULL){
            position_t cpos = cpy->data;
            if (cpos.i == pos.i && cpos.j == pos.j)
                return l; //não permite que adicione elementos repetidos
            cpy = cpy->next;
        }
        list_t* nl = (list_t*)malloc(sizeof(list_t));    
        nl->data = pos;
        nl->next = NULL;
        cpy->next = nl;
    }
    return l;
}

list_t* list_find(list_t* l, position_t pos){
    while(l != NULL){
        if (l->data.i == pos.i && l->data.j == pos.j)
            return l;
        l = l->next;    
    }
    return NULL;
}

bool list_contains(list_t* l, position_t pos){
    while(l != NULL){
        if (l->data.i == pos.i && l->data.j == pos.j)
            return true;
        l = l->next;    
    }
    return false;
}

list_t* list_rem(list_t* l, position_t pos){
    if (l != NULL){
        list_t* cpy = l;
        list_t* last = NULL;
        do{
            position_t lpos = cpy->data;
            if (lpos.i == pos.i && lpos.j == pos.j){
                if (last != NULL){
                    last->next = cpy->next;
                }else{
                    l = cpy->next;
                }
                free(cpy);
                break;
            }
            cpy = cpy->next;
        }while(cpy != NULL);
    }
    return l;
}

position_t list_get_last(list_t* l){
    position_t p;
    if (l != NULL){
        while (l->next != NULL)
            l = l->next;
        p.i = l->data.i;
        p.j = l->data.j;
    }
    return p;
}

list_t* list_rem_last(list_t* l){
    if (l == NULL)
        return l;
    if (l->next == NULL){
        free(l);
        return NULL;
    }
    list_t* prim = l;
    list_t* ant = NULL;
    while (l->next != NULL){
        ant = l;
        l = l->next;
    }
    if (ant != NULL){
        ant->next = NULL;
        free(l);
    }
    return prim;
}

void list_print(list_t* l){
    while (l != NULL){
        printf("(i:%d,j:%d)\n",l->data.i, l->data.j);
        l = l->next;
    }
    printf("\n");
}

void list_destroy(list_t* l){
    while(l != NULL){
        list_t* d = l;
        l = l->next;
        free(d);
    }
}

stack_t* stack_create(){
    stack_t* s = (stack_t*)malloc(sizeof(stack_t));
    s->l = list_create();
    return s;
}

void stack_destroy(stack_t* s){
    list_destroy(s->l);
    free(s);
}

void stack_push(stack_t* s, position_t pos){
    s->l = list_add(s->l, pos);
}

position_t statck_pop(stack_t* s){
    if (s->l == NULL)
        printf("PILHA VAZIA!\n");
    position_t p = list_get_last(s->l);
    s->l = list_rem_last(s->l);
    return p;
}

bool stack_empty(stack_t* s){
    return (s->l == NULL);
}