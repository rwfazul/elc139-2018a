typedef struct{
    int i,j;
}position_t;

position_t get_pos(int i, int j){
    position_t pos;
    pos.i = i;
    pos.j = j;
    return pos;
}

struct list_s{
    position_t data;
    struct list_s* next;
};

typedef struct list_s list_t;

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
                    last->next == cpy->next;
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