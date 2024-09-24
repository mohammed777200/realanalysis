
#include "equation.h"
#include "function.h"
#include "structs.h"
#include "type.h"
#include "linkedlist.h"
#include <math.h>

bool flag=0;


static bool non_linear(Polynomial *poly)
{
  bool _t=0;
  Node *_terms=poly->terms;
  while(_terms!=NULL)
  {
    if(((Term*)_terms->data)->power>1||((Term*)_terms->data)->power<0||((Term*)_terms->data)->power>0&&((Term*)_terms->data)->power<1) flag=1;
    _terms=_terms->next;
  }
  return _t;
}

#define EQ_LINEAR_AT_ZERO(type)\
  if(non_linear(poly)) return;\
  if(ll_count_of_list(poly->terms)==1 && ((Term*)poly->terms->data)->power==1)\
  {\
    type _sol=0;\
    *x=_sol;\
    return;\
  }\
  else if(ll_count_of_list(poly->terms)==1 && ((Term*)poly->terms->data)->power==0)\
  {\
    type _sol=1;\
    *x=_sol;\
    return;\
  }\
\
  type a=((Term*)poly->terms->next->data)->coefficient*(-1);\
  type b=((Term*)poly->terms->data)->coefficient;\

void eq_linear_at_zero(Polynomial *poly, Type dt)
{
  if(non_linear(poly))
  {
    printf("\nnon-linear function\n");
    return;
  }
  switch(dt)
  {
    case INT:
      {
        int*x=(int*)alloc_type(INT, 1);\
        EQ_LINEAR_AT_ZERO(int);
        *x=a/b;
      }
      break;
    case FLOAT:
      {
        float*x=(float*)alloc_type(FLOAT, 1);\
        EQ_LINEAR_AT_ZERO(float);
        *x=a/b;
      }
      break;
    case DOUBLE:
      {
        double *x=(double*)alloc_type(DOUBLE, 1);\
        EQ_LINEAR_AT_ZERO(double);
        *x=a/b;
      }
      break;
    case LONG_DOUBLE:
      {
        long double*x=(long double*)alloc_type(LONG_DOUBLE, 1);\
        EQ_LINEAR_AT_ZERO(long double);
        *x=a/b;
      }
      break;
    default:
      {
        char*x=(char*)alloc_type(CHAR, 1);\
        EQ_LINEAR_AT_ZERO(char);
        *x=a/b;
      }
      break;
  }
}

#define EQ_LINEAR(type, TYPE)\
  type _b=0;\
  type _d=0;\
  type _a=((Term*)_p1->terms->data)->coefficient;\
  type _c=((Term*)_p2->terms->data)->coefficient;\
  if((ll_count_of_list(_p1->terms)>1 && ((Term*)_p1->terms->next->data)->power)==0)\
    _b=((Term*)_p1->terms->next->data)->coefficient;\
  if(ll_count_of_list(_p2->terms)>1 && ((Term*)_p2->terms->next->data)->power==0)\
    _d=((Term*)_p2->terms->next->data)->coefficient;\
    \
  if(ll_count_of_list(_p2->terms)==1 && ((Term*)_p2->terms->data)->power==0)\
  {\
    _c=0;\
    _d=((Term*)_p2->terms->data)->coefficient;\
  }\
  if(ll_count_of_list(_p1->terms)==1&&((Term*)_p1->terms->data)->power==0)\
  {\
    _a=0;\
    _b=((Term*)_p1->terms->data)->coefficient;\
  }\
  const type p=(_b-_d);\
  const type q=(_a-_c);\
  if(q==0)\
  {\
    printf("\nno solution for this function!\n");\
    poly_destroy(_p1); _p1=NULL;\
    poly_destroy(_p2); _p2=NULL;\
    return NULL;\
  }\
  type *_s=(type*)alloc_type(TYPE, 1);\
  *_s=(p/q)*-1;\
  Point *point=(Point*)alloc_type(POINT, 1);\
  type *result=(type*)alloc_type(TYPE, 1);\
  Node *_terms=_p1->terms;\
  while(_terms!=NULL)\
  {\
    *result+=((Term*)_terms->data)->coefficient  *pow(*_s, ((Term*)_terms->data)->power);\
    _terms=_terms->next;\
  }\
  point->x=_s;\
  point->y=result;\
  poly_destroy(_p1); _p1=NULL;\
  poly_destroy(_p2); _p2=NULL;\
  return point;\

/* Point *eq_linear(Function *f1, Function *f2) */
Point *eq_linear(const char *expr1, const char *expr2, Type dt)
{
  if(expr1==NULL||expr2==NULL) return NULL;
  Polynomial *_p1=poly_create((char*)expr1, dt);
  Polynomial *_p2=poly_create((char*)expr2, dt);

  if(non_linear(_p1)||non_linear(_p2))
  {
    printf("\n\e[0;32meither one of functions non-linear\n");
    poly_destroy(_p1); _p1=NULL;
    poly_destroy(_p2); _p2=NULL;
    return NULL;
  }

  // sloving
  switch(dt)
  {
    case INT:         { EQ_LINEAR(int, INT); }                  break;
    case FLOAT:       { EQ_LINEAR(float, FLOAT); }              break;
    case DOUBLE:      { EQ_LINEAR(double, DOUBLE); }            break;
    case LONG_DOUBLE: { EQ_LINEAR(long double, LONG_DOUBLE); }  break;
    default: break;
  }
    poly_destroy(_p1); _p1=NULL;
    poly_destroy(_p2); _p2=NULL;
  return NULL;
}
