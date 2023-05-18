
#include <stdio.h>
#include <stdlib.h>
#include <except.h>
#include "arena.h"
#include <string.h>
int main()
{
    Arena_T pool = Arena_new();
    int *pint = Arena_alloc(pool, sizeof(int), __FILE__, __LINE__);
    *pint = 24;
    char *pchar = (char*)Arena_alloc(pool, sizeof(char) * 20, __FILE__, __LINE__);
    strcpy(pchar,"hello arena");
    printf("int:%d, char *: %s\n",*pint,pchar);
    Arena_free(pool);
    printf("int:%d, char *: %s\n",*pint,pchar);
    Arena_dispose(&pool);
    printf("pool:%ul",pool);
}