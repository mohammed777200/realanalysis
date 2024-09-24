// plot.h

#ifndef PLOT_H
#define PLOT_H

#include <plplot/plplot.h>
#include "function.h"



#define BLACK         0
#define RED           1
#define YELLOW        2
#define GREEN         3
#define AQUAMARINE    4
#define PINK          5
#define WHEAT         6
#define GREY          7
#define BROWN         8
#define BLUE          9
#define bLUEvIOLETA   10 
#define CYAN          11 
#define TURQUOISE     12 
#define MAGENTA       13 
#define SALMON        14 
#define WHITE         15 

void pl_plot(Function *func);
void pl_two_functions(Function *f1, Function *f2, const char *xlabel, const char *ylabel, const char *glabel);
void pl_plot_points(Set *domain, Set *codomain, Point *intersection, const char *xlabel, const char *ylabel, const char *glabel);
void pl_setting_labels(const char *xlabel, const char *ylabel, const char *glabel);

#endif
