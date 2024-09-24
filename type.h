#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "structs.h"

void *alloc_type(Type dt, unsigned int n);
unsigned int dt_size(Type dt);

#endif
