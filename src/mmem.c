#include "mmem.h"
#include <stddef.h>
#include <unistd.h>
#define T Mem_node
#define MINALLOC 1024
typedef long Align;
typedef union T T;
union T
{
    struct
    {
        unsigned int size;
        T *next;
    } s;
    Align x;
};
T *more_mem(unsigned int units);
static T first;
static T *Free_list = NULL;
void *mmalloc(unsigned int size)
{
    unsigned int units;
    T *prev, *p;
    if (size == 0)
        return NULL;
    units = (size - 1 + sizeof(T)) / (sizeof(T)) + 1;
    if ((prev = Free_list) == NULL)
    {
        first.s.next = prev = Free_list = &first;
        first.s.size = 0;
    }
    for (p = prev->s.next;; prev = p, p = p->s.next)
    {
        if (p->s.size >= units)
        {
            if (p->s.size == units)
            {
                prev->s.next = p->s.next;
            }
            else
            {
                p->s.size -= units;
                p += p->s.size;
                p->s.size = units;
            }
            Free_list = prev;
            return (void *)(p + 1);
        }
        if (p == Free_list)
        {
            if (more_mem(units) == NULL)
                return NULL;
        }
    }
}

/* put mem back to Free_list */
void mfree(void *ptr)
{
    T *bnode, *p;
    bnode = (T *)ptr - 1;
    for (p = Free_list; !(bnode>p && bnode<p->s.next); p = p->s.next)
        if(p>=p->s.next && (bnode>p || bnode<p->s.next))
            break;
    if(bnode + bnode->s.size == p->s.next){
        bnode->s.size += p->s.next->s.size;
        bnode->s.next = p->s.next->s.next;
    }
    else{
        bnode->s.next = p->s.next;
    }
    if(p+p->s.size==bnode){
        p->s.size += bnode->s.size;
        p->s.next = bnode->s.next;
    }
    else{
        p->s.next = bnode;
    }
    Free_list = p;
}

T *more_mem(unsigned int units)
{
    T *New_node;
    void *ptr;
    if (units < MINALLOC)
        units = MINALLOC;
    ptr = sbrk(units * sizeof(T));
    if (ptr == (void *)-1)
    {
        return NULL;
    }
    else
    {
        New_node = (T *)ptr;
        New_node->s.size = units;
        mfree(New_node + 1);
        return New_node;
    }
}
#undef T