//set.h

#ifndef SET_H
#define SET_H
#include "hashtable.h"
#include <math.h>
#include "point.h"
#include "type.h"
#define MAX(A, B) (A<B)? B : A
#define MIN(A, B) (A>B)? B : A

#define _M_ 20

// basic
Set *set_create(Type dt);
Set *set_insert(Set *s, void *member);
Set *set_add_const(Set *s, double c);
Node *set_get_members(Set *s);
void set_print(Set *s);
void set_print_point(Set *s, Type dt);
Set *set_destroy(Set *s);
void set_delete_member(Set **s, void *value, Type dt);

// set operations
Set *set_intersect(Set *res, Set *s1, Set *s2);
Set *set_union(Set *res, Set *s1, Set *s2);
Set *set_power(Set *res, Set *s);
Set *set_complement(Set *res, Set *universal, Set *s);
Set *set_cart_prod(Set *res, Set *s1, Set *s2);
Set *set_difference(Set *res, Set *s1, Set *s2);
Set *set_get_copy(Set *s);
Set *set_get_pairs(Set *domain, Set *codomain);
double set_calc_slope(Set *s1, Set *s2);
void *set_first_mem(Set *s);
void *set_last_mem(Set *s);
void *set_least_upper_bound(Set *s);
void *set_greatest_lower_bound(Set *s);
void set_plot(Set *domain, Set *codomain, Point *intersection, const char *xlabel, const char *ylabel, const char *glabel);
void set_plots(Set *domains[], Set *codomains[], size_t size, Node *pts, const char *xlabel, const char *ylabel, const char *glabel);



bool set_equality(Set *s1, Set *s2);
bool set_is_subset(Set *s1, Set *s2); // aka, is s1 a subset of s2
bool set_is_equivalent(Set *s1, Set *s2);
bool set_is_proper_subset(Set *s1, Set *s2);
bool set_belong(Set *s, void *member);
bool set_is_empty(Set *s);


// private functions
static void _set_print_by_type(void *n, Type dt);

#endif
