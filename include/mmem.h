#ifndef MMEM_INCLUDED
#define MMEM_INCLUDED
extern void *mmalloc(unsigned int size);
extern void mfree(void *ptr);
#endif