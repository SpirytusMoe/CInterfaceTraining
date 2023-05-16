#include <stddef.h>
#include<string.h>
#include "except.h"
#include "admem.h"
#include <stdlib.h>
#include <assert.h>
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
typedef struct inode inode;
struct inode
{
    inode *next;
    inode *nfree;
    const void *ptr;
    long size;
    const char *file;
    int line;
};
const Except_T Mem_Failed = {"Alloc Failed"};
const Except_T Access_Failed = {"Access_Failed"};
#define HTABSIZE 1024
#define NIPRELOAD 512
#define NALLOC (4096 + sizeof(union align) - 1) / \
                   sizeof(union align) * sizeof(union align)
#define hash(p, t) \
    (((unsigned long)(p) >> 3) & (sizeof(t) / sizeof(t)[0] - 1))
static inode *htab[HTABSIZE];
static inode freelist = {.nfree = &freelist};
// assume paras of private func are always match rules
static inode *ifind(const void *ptr)
{
    inode *p = htab[hash(ptr, htab)];
    while (p && p->ptr != ptr)
        p = p->next;
    return p;
}
static inode *ialloc(void *ptr, long size, const char *file, int line)
{
    static inode *p;
    static int nleft = 0;
    if (nleft <= 0)
    {
        p = (inode *)malloc(sizeof(*p) * NIPRELOAD);
        if (p == NULL)
            return NULL;
        nleft = NIPRELOAD;
    }
    p->ptr = ptr;
    p->size = size;
    p->file = file;
    p->line = line;
    p->nfree = p->next = NULL;
    nleft--;
    return p++;
}
void *Admem_alloc(long nbytes, const char *file, int line)
{
    inode *p, *newp;
    void *ptr;
    nbytes = (nbytes + sizeof(union align) - 1) /
             sizeof(union align) * sizeof(union align);
    for (p = freelist.nfree; p; p = p->nfree)
    {
        if (p->size >= nbytes)
        {
            p->size -= nbytes;
            ptr = (void *)((char *)p->ptr + p->size);
            if ((newp = ialloc(ptr, nbytes, file, line)) != NULL){
                unsigned int h = hash(ptr,htab);
                newp->next =htab[h];
                htab[h] = newp;
                return ptr;
            }
            else{
                if(file==NULL){
                    RAISE(Mem_Failed);
                }
                else{
                    Except_raise(&Mem_Failed,file,line);
                }
            }
        }
        if (p == &freelist)
        {
            if( (ptr = malloc(nbytes+NALLOC)) == NULL ||
            (newp = ialloc(ptr, nbytes+NALLOC, __FILE__, __LINE__)) == NULL
            ){
                if(file==NULL){
                    RAISE(Mem_Failed);
                }
                else{
                    Except_raise(&Mem_Failed,file,line);
                }
            }
            newp->nfree = freelist.nfree;
            freelist.nfree = newp;
        }
    }
    assert(0);
    return NULL;
}
void *Admem_calloc(long count, long nbytes, const char *file, int line)
{
    void *ptr;
    assert(count>0);
    assert(nbytes>0);
    ptr = Admem_alloc(count*nbytes,file,line);
    memset(ptr,'\0',count*nbytes);
    return ptr;
}
void Admem_free(void *ptr, const char *file, int line)
{
    if (ptr)
    {
        inode *p;
        if ((p = ifind(ptr)) == NULL || p->nfree)
        {
            Except_raise(&Access_Failed, file, line);
        }
        p->nfree = freelist.nfree;
        freelist.nfree = p;
    }
}
void *Admem_resize(void *ptr, long nbytes, const char *file, int line)
{
    void *newp;
    inode *p;
    assert(ptr);
    assert(nbytes>0);
    if((p = ifind(ptr)) == NULL || p->nfree
    ){
        Except_raise(&Access_Failed, file, line);
    }
    newp = Admem_alloc(nbytes,file,line);
    memcpy(newp,ptr,p->size >nbytes? nbytes:p->size);
    Admem_free(ptr,file,line);
    return newp;
}
