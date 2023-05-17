
#include <stdio.h>
#include <stdlib.h>
#include <admem.h>
#include <except.h>
#include <stack.h>

int main()
{
    stack_T s = Stack_new();
    stack_T *p = &s;
    int tmp = 1;
    int *a = &tmp;
    char *b = "hello";
    Stack_push(s, a);
    Stack_push(s, b);
    printf("stack is empty? %d\n",Stack_empty(s));
    printf("b=%d\na=%s\n",*((int*)Stack_pop(s)),(char*)Stack_pop(s));
    Stack_free(p);
}