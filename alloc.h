
#ifndef ALLOC_H
#define ALLOC_H
#include <stdlib.h>
void *ex_malloc(size_t size);
void *ex_calloc(size_t nmemb, size_t size);
void ex_free(void *p);

#endif
