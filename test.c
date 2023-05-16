
#include<stdio.h>
#include<stdlib.h>
#define NALLOC (4096 + sizeof(union align) - 1) / \
                   sizeof(union align) * sizeof(union align)
union align {
#ifdef MAXALIGN
	char pad[MAXALIGN];
#else
	int i;
	long l;
	long *lp;
	void *p;
	void (*fp)(void);
	float f;
	double d;
	long double ld;
#endif
};
int main(){
    void *p;
    printf("%d\n",sizeof(union align));
    for(int i=1;i<10;i++){
        p = malloc(i*sizeof(union align) + NALLOC);
        if(p){
            printf("p=%ul,p mod =%ul\n",
            (unsigned long)p, 
            ((unsigned long)p)%(sizeof(union align)));
        }
    }
    
}