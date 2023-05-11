#ifndef EXCEPT_INCLUDED
#define EXCEPT_INCLUDED
#include<setjmp.h>
#define T Except_T
typedef struct T
{
    const char *reason;
} T;
typedef struct Except_Frame Except_Frame;
struct Except_Frame{
    jmp_buf context;
    Except_Frame *prev;
    const char *file;
    int line;
    const T *exception;
};
#define RAISE(e) Except_raise(&(e),__FILE__,__LINE__)
void Except_raise(const T *e, const char *file, int line);
#undef T
#endif