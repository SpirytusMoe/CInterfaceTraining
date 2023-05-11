#include "mmem.h"
#include<stdio.h>
int main(){
    int *p = mmalloc(sizeof(int)*16);
    if(p){
        for(int i=0;i<16;i++)
            p[i]=i;
    }
    for(int i=0;i<16;i++)
        printf("%d ",p[i]);
}