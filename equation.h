
#pragma once

#include "structs.h"
/* void eq_linear_at_zero(Function **fun); */

void eq_linear_at_zero(Polynomial *poly, Type dt);
Point *eq_linear(const char *f1, const char *f2, Type dt);
