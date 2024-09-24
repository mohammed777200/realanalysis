
#include "alloc.h"
#include <stdio.h>
void *ex_malloc(size_t size)
{
  void *p = malloc(size);
  printf("malloc: %lu %p\n", size, p);
  return p;
}

void *ex_calloc(size_t nmemb, size_t size)
{
  void *p = calloc(nmemb, size);
  printf("calloc: %lu %p\n", size, p);
  return p;
}

void ex_free(void *p)
{
  printf("free: %p\n", p);
  free(p);
}
