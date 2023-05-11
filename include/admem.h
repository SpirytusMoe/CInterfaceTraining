#ifndef ADMEM_INCLUDED
#define ADMEM_INCLUDED
extern void *Admem_alloc(long nbytes, const char *file, int line);
extern void *Admem_calloc(long count, long nbytes, const char *file, int line);
extern void Admem_free(void *ptr, const char *file, int line);
extern void Admem_resize(void *ptr, long nbytes, const char *file, int line);
#define ADALLOC(nbytes) Admem_alloc((nbytes), __FILE__, __LINE__)
#define ADCALLOC(count, nbytes) Admem_calloc((count), (nbytes), __FILE__, __LINE__)
#define ADNEW(p) ((p) = ADALLOC((long)sizeof*(p)))
#define ADNEW0(p) ((p) = ADCALLOC(1, (long)sizeof*(p)))
#define ADFREE(p) ( Admem_free((p), __FILE__, __LINE__), (p)=(void*)0)
#define ADRESIZE(p,nbytes) ((p) = Admem_resize((p), (nbytes), __FILE__, __LINE__))
#endif