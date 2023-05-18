#include "arena.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "except.h"
#include <assert.h>
#define T Arena_T
#define MAXFREE 10
struct T
{
    T prev;
    char *start;
    char *end;
};
union align
{
    int i;
    long l;
    long *lp;
    void *p;
    void (*fp)(void);
    float f;
    double d;
    long double ld;
};
union header
{
    struct T b;
    union align x;
};
const Except_T Arena_NewFailed = {"Can't create new arena"};
const Except_T Arena_AllocFailed = {"Can't alloc from cur arena"};
static T freelist;
static int nfree;
T Arena_new(void)
{
    T arena;
    arena = malloc(sizeof(*arena));
    if (arena == NULL)
        RAISE(Arena_NewFailed);
    arena->prev = NULL;
    arena->start = arena->end = NULL;
    return arena;
}
void Arena_dispose(T *ap)
{
    assert(ap && *ap);
    Arena_free(*ap);
    free(*ap);
    *ap = NULL;
}
void *Arena_alloc(T arena, long nbytes, char *file, int line)
{
    assert(arena);
    assert(nbytes > 0);
    nbytes = (nbytes + sizeof(union align) - 1) /
             (sizeof(union align)) * (sizeof(union align));
    while (nbytes > arena->end - arena->start)
    {
        T ptr;
        char *end;
        if ((ptr = freelist) != NULL)
        {
            freelist = freelist->prev;
            nfree--;
            end = ptr->end;
        }
        else
        {
            long m = sizeof(union header) + nbytes + 10 * 1024;
            ptr = malloc(m);
            if (ptr == NULL)
            {
                if (file == NULL)
                    RAISE(Arena_AllocFailed);
                else
                    Except_raise(&Arena_AllocFailed, file, line);
            }
            end = (char*)ptr + m;
        }
        *ptr = *arena;
        arena->prev = ptr;
        arena->start = (char *)((union header *)ptr + 1);
        arena->end = end;
    }
    arena->start += nbytes;
    return (void *)(arena->start - nbytes);
}
void *Arena_calloc(T arena, long count, long nbytes, char *file, int line)
{
    void *ptr;
    assert(count > 0);
    ptr = Arena_alloc(arena, count * nbytes, file, line);
    memset(ptr, '\0', count * nbytes);
    return ptr;
}
void Arena_free(T arena)
{
    assert(arena);
    while(arena->prev){
        struct T tmp = *(arena->prev);
        if(nfree<MAXFREE){
            arena->prev->prev = freelist;
            freelist = arena->prev;
            nfree++;
            freelist->end = arena->end;
            //clear new head of freelist
            //memset((char*)((union header*)freelist+1),'\0',freelist->end - (char*)((union header*)freelist+1));
        }
        else{
            free(arena->prev);
        }
        *arena = tmp;
    }
    assert(arena->start==NULL);
    assert(arena->end==NULL);
}