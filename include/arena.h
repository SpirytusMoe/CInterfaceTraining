#ifndef ARENA_INCLUDED
#define ARENA_INCLUDED
#include "except.h"
#define T Arena_T
typedef struct T *T;
extern const Except_T Arena_NewFailed;
extern const Except_T Arena_AllocFailed;
extern T Arena_new(void);
extern void Arena_dispose(T* ap);
extern void* Arena_alloc(T arena, long nbytes, char* file, int line);
extern void* Arena_calloc(T arena, long count, long nbytes, char* file, int line);
extern void Arena_free(T arena);
#undef T
#endif