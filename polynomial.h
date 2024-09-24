
#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#include "type.h"
#include "helper.h"
#include "linkedlist.h"
#include "set.h"


// this will expect a char* as polynomial, then it will translate that string into linked list of powers and coefficients and signs
Polynomial *poly_create(char *expr, Type dt);

// remove Polynomial intity from memory alllocated before
Polynomial *poly_destroy(Polynomial *poly);

// add term to already exist polynomial, get simplified if the term find same power
void poly_add_term(Polynomial **poly, double);


// private functions
uint n_length(const char *n);
float *term(const char *_n);
void polynomial(char *p, Polynomial **poly, Type dt);

#endif
