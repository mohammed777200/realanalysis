#ifndef FUNCTION_H
#define FUNCTION_H
#include <stdbool.h>
#include "set.h"
#include "polynomial.h"

typedef unsigned int uint;

// basic operations
Function *func_create(Type dt, char *expr, void *a, void *b);
Function *func_destroy(Function *f);
Set *func_get_pairs(Function *func);
void *func_calc(Function *func, void *x);
double func_slope(Function *func);
void func_plot(Function *f, const char *xlabel, Point *intersection, const char *ylabel, const char *glabel);
void func_plots(Function *f[], size_t size, Point **eq, const char *xlabel, const char *ylabel, const char *glabel);

#endif
