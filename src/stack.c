#include "stack.h"

#define T stack_T
struct T
{
    int count;
    struct elem
    {
        void *x;
        struct elem *prev
    }*head;
};

T Stack_new(void){
}

int Stack_empty(T stk){
}

void* Stack_pop(T stk){

}

void Stack_push(T stk,void *x){

}

void Stack_free(T stk){
    
}