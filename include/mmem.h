#ifndef MMEM_INCLUDED
#define MMEM_INCLUDED
void *mmalloc(unsigned int size);
void mfree(void *ptr);
#endif