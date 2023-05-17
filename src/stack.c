#include "stack.h"
#include "admem.h"
#include <stddef.h>
#include <assert.h>
#define T stack_T
struct T
{
    int count;
    struct elem
    {
        void *x;
        struct elem *prev
    } *head;
};

T Stack_new(void)
{
    T stk;
    ADNEW(stk);
    stk->count = 0;
    stk->head = NULL;
    return stk;
}

int Stack_empty(T stk)
{
    assert(stk);
    return stk->count == 0;
}

void *Stack_pop(T stk)
{
    void *x;
    struct elem *tmp;
    assert(stk);
    assert(stk->count > 0);
    x = stk->head->x;
    tmp = stk->head;
    stk->head = tmp->prev;
    stk->count--;
    ADFREE(tmp);
    return x;
}

void Stack_push(T stk, void *x)
{
    assert(stk);
    struct elem *t;
    ADNEW(t);
    t->x = x;
    t->prev = stk->head;
    stk->head = t;
    stk->count++;
}

void Stack_free(T *stk)
{
    struct elem *t,*prev;
    assert(stk && *stk);
    for (t = (*stk)->head; t; t = prev)
    {
        prev = t->prev;
        ADFREE(t);
    }
    ADFREE(*stk);
}