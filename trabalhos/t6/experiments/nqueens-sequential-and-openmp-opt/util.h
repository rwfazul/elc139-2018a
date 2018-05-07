#include <stdbool.h>

typedef struct{
    int i,j;
}position_t;

struct list_s{
    position_t data;
    struct list_s* next;
};

typedef struct list_s list_t;

typedef struct{
    list_t* l;
}stack_t;

position_t get_pos(int i, int j);

list_t* list_create();

list_t* list_add(list_t* l, position_t pos);

list_t* list_find(list_t* l, position_t pos);

bool list_contains(list_t* l, position_t pos);

list_t* list_rem(list_t* l, position_t pos);

position_t list_get_last(list_t* l);

list_t* list_rem_last(list_t* l);

void list_print(list_t* l);

void list_destroy(list_t* l);

stack_t* stack_create();

void stack_destroy(stack_t* s);

void stack_push(stack_t* s, position_t pos);

position_t statck_pop(stack_t* s);

bool stack_empty(stack_t* s);