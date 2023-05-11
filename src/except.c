#include "except.h"
#include <stdlib.h>
#include<stddef.h>
#include<stdio.h>
#define T Except_T
Except_Frame *Except_stack = NULL;
void Except_raise(const T *e, const char *file, int line){
    Except_Frame *p = Except_stack;
    if(p==NULL){
        fprintf(stderr, "Uncaught exception:\n");
        if(e->reason)
            fprintf(stderr,"%s",e->reason);
        else
            fprintf(stderr,"%s","at 0x%p",e);
        if(file&&line)
            fprintf(stderr," raised at %s:%d\n",file,line);
        fflush(stderr);
        abort();
    }
}
#undef T