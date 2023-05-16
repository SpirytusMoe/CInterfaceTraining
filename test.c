
#include<stdio.h>
#include<stdlib.h>
#include<admem.h>
#include<except.h>
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
int main(){
    int i,cnt,size;
    void *p;
    cnt=0;
    srand(3342);
    for(i=0;i<100;i++){
        size = rand()%3000;
        p = ADALLOC(size);
        if(((unsigned long)p)%(sizeof(union align))!=0)
            cnt++;
        
    }
    printf("cnt:%d\n",cnt);
}