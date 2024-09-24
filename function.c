// function.c
#include <math.h>
#include <plplot/plplot.h>

#include "function.h"
#include "plot.h"
#include "structs.h"

// private functions
static void _func_set_interval(Set **s, void *a, void *b);
static void _func_polynomial(char *poly, Function **func);
static void _func_init_points(Function *f, unsigned int cardinal, PLFLT *x, PLFLT *y);
  
Function *func_create(Type dt, char *expr, void *a, void *b)
{
  Function *_new_func=(Function*)alloc_type(FUNCTION, 1);
  _new_func->solution=NULL;

  _new_func->domain=set_create(dt);  // need to free
  _new_func->codomain=set_create(dt); // need to free
                              
  if(expr==NULL||strlen(expr)==0)
  {
    char _expr[128];
    printf("\nenter the polynomial:\t");
    scanf("%s", _expr);
    expr=_expr;
  }

  _func_set_interval(&_new_func->domain, a, b); // setting the interval for a domain
  _func_polynomial(expr, &_new_func);
  return _new_func;
}


Set *func_get_pairs(Function *func)
{
  if(func==NULL)
  {
    printf("function does not defined!\n");
    return NULL;
  }
  printf("\n");
  Set *res=set_create(POINT);
  Node *_table1[20], *_table2[20];
  for(int i=0; i<20; i++)
  {
   _table1[i]=func->domain->members->table[i];
   _table2[i]=func->codomain->members->table[i];
  }
  Point _p;
  int _inx=0, _jnx=0;
  Node *_index=func->domain->members->sorts;
  Node *_jndex=func->codomain->members->sorts;
  Node *_tmp=NULL;
  while(_index!=NULL && _jndex!=NULL)
  {
    _inx=*(int*)_index->data;
    _tmp=func->domain->members->table[_inx];
    if(_tmp!=NULL)
    {
      _p.x=_tmp->data;
      func->domain->members->table[_inx]=func->domain->members->table[_inx]->next;
    }
    _jnx=*(int*)_jndex->data;
    _tmp=func->codomain->members->table[_jnx];
    if(_tmp!=NULL)
    {
      _p.y=_tmp->data;
      func->codomain->members->table[_jnx]=func->codomain->members->table[_jnx]->next;
    }
    res=set_insert(res, &_p);
    _index=_index->next;
    _jndex=_jndex->next;
  }
  for(int i=0; i<20; i++)
  {
    func->domain->members->table[i]   =_table1[i];
    func->codomain->members->table[i] =_table2[i];
  }
  return res;
}



Function *func_destroy(Function *f)
{
  if(f==NULL) { printf("there is no function to free!\n"); return NULL; }
  if(f->domain!=NULL) { f->domain=set_destroy(f->domain);                      f->domain=NULL; }
  if(f->codomain!=NULL) { f->codomain=set_destroy(f->codomain);                f->codomain=NULL;}
  if(f->polynomial!=NULL) { f->polynomial=poly_destroy(f->polynomial);      f->polynomial=NULL; }
  if(f->solution!=NULL) { free(f->solution);                                 f->solution=NULL; }
  free(f); f=NULL;
  return f;
}



// function calculate for any given value of x
void *func_calc(Function *func, void *x)
{
  double *result=(double*)alloc_type(DOUBLE, 1);
  *result=0;
  Node *_terms=func->polynomial->terms;
  while(_terms!=NULL)
  {
    *result+=((Term*)_terms->data)->coefficient*pow(((Term*)_terms->data)->x_pos+*(double*)x, ((Term*)_terms->data)->power);
    _terms=_terms->next;
  }
  return result;
}

double func_slope(Function *func)
{
  double _slope=set_calc_slope(func->domain, func->codomain);
  return _slope;
}

// plot given function, with labels or use default labels
void func_plot(Function *f, const char *xlabel, Point *intersection, const char *ylabel, const char *glabel) 
{ 
  pl_plot_points(f->domain, f->codomain, intersection, xlabel, ylabel, glabel);
}

void func_plots(Function *f[], size_t size, Point **eq, const char *xlabel, const char *ylabel, const char *glabel)
{
plsdev("xcairo");
plinit();
plfont(2);

  double *_f_xmaxes[size];
  double *_f_ymaxes[size];

  double *_f_xmines[size];
  double *_f_ymines[size];
  unsigned int _f_cardianls[size];
  for(unsigned int i=0; i<size; ++i)
  {
    // get the max values of interval of x axix
    _f_xmaxes[i]=(double*)set_least_upper_bound(f[i]->domain);

    // get the max value of interval of y axix
    _f_ymaxes[i]=(double*)set_least_upper_bound(f[i]->codomain);


    // get the min values of interval of x axix
    _f_xmines[i]=(double*)set_greatest_lower_bound(f[i]->domain);

    // get the min value of interval of y axix
    _f_ymines[i]=(double*)set_greatest_lower_bound(f[i]->codomain);

    // get cardianl for each function and insert it in a vector array
    _f_cardianls[i]=f[i]->domain->cardinal;
  }

  double _xmax=*_f_xmaxes[0];
  double _ymax=*_f_ymaxes[0];

  double _xmin=*_f_xmines[0];
  double _ymin=*_f_ymines[0];

  for(unsigned int i=0; i<size; ++i)
  {
    // find the largest max value among all functions
    if(*_f_xmaxes[i]>=_xmax)
      _xmax=*_f_xmaxes[i];

    if(*_f_ymaxes[i]>=_ymax)
      _ymax=*_f_ymaxes[i];

    // find the smallest min value among all functions
    if(*_f_xmines[i]<=_xmin)
      _xmin=*_f_xmines[i];

    if(*_f_ymines[i]<=_ymin)
      _ymin=*_f_ymines[i];
  }

  unsigned int _cardinal=_f_cardianls[0];
  for(unsigned int i=0; i<size; ++i)
    if(_f_cardianls[i]>=_cardinal)
      _cardinal=_f_cardianls[i];


  PLFLT x[_cardinal], y[_cardinal]; // where the points are stores for plotting


plcol0(15);
plenv(_xmin, _xmax, _ymin, _ymax, 0, 1);
pl_setting_labels(xlabel, ylabel, glabel);
plwidth( 1.5 );

  // plotting functions
 
  unsigned int _cc=0;
  unsigned int i=0;
  for(int i=0; i<size; ++i)
  {
    // plotting first function
    plwidth( 2 );
    _func_init_points(f[i], _cardinal, x, y);

    _cc+=(i%15==0||i%15==1)?(i+2)%15:i+2;
    plcol0(_cc);
    /* plpoin(_cardinal, x+i, y+i, 17); */
    plline(_cardinal, x, y);

    if(eq!=NULL)
    {
      for(int j=i; j<size; ++j)
      {
        if(f[i]==f[j]) continue;
        /* calculate intersection for linear relations */
        Point *_solution=eq[j];
        if(_solution==NULL) 
          continue;
        plcol0(_cc+j);
        char _string[32];
        sprintf(_string, "(%.2lf, %.2lf)", *(double*)_solution->x, *(double*)_solution->y);
        plptex(*(double*)_solution->x, *(double*)_solution->y-1, 0, 0, 2, _string);
        PLFLT _f=0;
        void *_fake=&_f;
        plpoin(1, _solution->x, _solution->y, 9);
        plpoin(1, _solution->x, _fake, 17); // 17
        plpoin(1, _fake, _solution->y, 17);
        // draw solution intersection
        plpath(_cardinal, *(double*)_solution->x, 0, *(double*)_solution->x, *(double*)_solution->y);
        plpath(_cardinal, *(double*)_solution->x, *(double*)_solution->y, 0, *(double*)_solution->y);
      }
    }
  }
  for(unsigned int i=0; i<size; ++i)
  {
    free(_f_xmaxes[i]); free(_f_ymaxes[i]);
    free(_f_xmines[i]); free(_f_ymines[i]);
  }
  plend();
}

// ---------------- private functions -------------------------------

static void _func_init_points(Function *f, unsigned int cardinal, PLFLT *x, PLFLT *y)
{
  // plotting first function
  Node *domain=set_get_members(f->domain);
  Node *codomain=set_get_members(f->codomain);
  Node *_domain=domain, *_codomain=codomain;
  unsigned int i=0;
  while(_domain!=NULL)
  {
    x[i]=*(double*)_domain->data;
    y[i]=*(double*)_codomain->data;

    ++i;
    _domain=_domain->next;
    _codomain=_codomain->next;
  }
  ll_delete_entire_list(domain);
  ll_delete_entire_list(codomain);
}

static void _func_set_interval(Set **s, void *a, void *b)
{

#define SET_INTERVAL(type, format)\
  type _a, _b;\
  if(a==NULL) { printf("\nEnter the point a :\t"); scanf(format, &_a); }\
  else _a=*(type*)a;\
  if(b==NULL) { printf("\nEnter the point a :\t"); scanf(format, &_b); }\
  else _b=*(type*)b;\
  type _i=1;\
  *_delta_ptr=_i;\
  if(_a==_b) _a=0, _b=1;\
  for(type x=_a; x<=_b; x++)\
    *s=set_insert(*s, &x);

#define SET_INTERVAL_REAL(type, format)\
  type _a, _b;\
  if(a==NULL) { printf("\nEnter the point a :\t"); scanf(format, &_a); }\
  else _a=*(type*)a;\
  if(b==NULL) { printf("\nEnter the point a :\t"); scanf(format, &_b); }\
  else _b=*(type*)b;\
  if(_a==_b) _a=0, _b=1;\
  type _dist=_b-_a;\
  type _delta=_dist/100;\
  *_delta_ptr=_delta;\
  (*s)->delta=_delta_ptr;\
  for(type x=_a; x<=_b; x+=_delta)\
  {\
    if(x<0&&x+_delta>0)\
      x=0;\
    *s=set_insert(*s, &x);\
  }\

  switch((*s)->data_type)
  {
    case CHAR:        { char *_delta_ptr=(char*)alloc_type(CHAR, 1); SET_INTERVAL(char, "%c"); }                                   break;
    case INT:         { int *_delta_ptr=(int*)alloc_type(INT, 1); SET_INTERVAL(int, "%d"); }                                       break;
    case FLOAT:       { float *_delta_ptr=(float*)alloc_type(FLOAT, 1); SET_INTERVAL_REAL(float, "%f") }                           break;
    case DOUBLE:      { double *_delta_ptr=(double*)alloc_type(DOUBLE, 1); SET_INTERVAL_REAL(double, "%lf") }                      break;
    case LONG_DOUBLE: { long double *_delta_ptr=(long double*)alloc_type(LONG_DOUBLE, 1); SET_INTERVAL_REAL(long double, "%LF") }  break;
    default: break;
  }
}


static void _func_polynomial(char *poly, Function **func)
{
#define _FUNC_POLYNOMIAL(type, p, domain, codomain, format)\
  type result=0;\
  while(_memberstmp!=NULL)\
  {\
    Node *_terms=p->terms;\
    while(_terms!=NULL)\
    {\
      result+= ((Term*)_terms->data)->coefficient *pow(((Term*)_terms->data)->x_pos + *(type*)_memberstmp->data, ((Term*)_terms->data)->power);\
      _terms=_terms->next;\
    }\
    if(result==INFINITY||result==-INFINITY)\
    {\
      printf("undefined at x="format"\n", *(type*)_memberstmp->data);\
      result=0;\
      codomain=set_insert(codomain, &result);\
    }\
    else\
      codomain=set_insert(codomain, &result);\
    result=0;\
    _memberstmp=_memberstmp->next;\
  }

  Polynomial *p=poly_create(poly, (*func)->domain->data_type);
  Node *_members=set_get_members((*func)->domain);
  Node *_memberstmp=_members;

  switch((*func)->domain->data_type)
  {
    case CHAR:
      {
        _FUNC_POLYNOMIAL(char, p, (*func)->domain, (*func)->codomain, "%c")
      }
      break;
    case INT:
      {
        _FUNC_POLYNOMIAL(int, p, (*func)->domain, (*func)->codomain, "%d")
      }
    case FLOAT:
      {
        _FUNC_POLYNOMIAL(float, p, (*func)->domain, (*func)->codomain, "%.1f")
      }
    case DOUBLE:
      {
        _FUNC_POLYNOMIAL(double, p, (*func)->domain, (*func)->codomain, "%lf")
      }
      break;
    case LONG_DOUBLE:
      {
        _FUNC_POLYNOMIAL(long double, p, (*func)->domain, (*func)->codomain, "%Lf")
      }
      break;
    default:
      {
        _FUNC_POLYNOMIAL(int, p, (*func)->domain, (*func)->codomain, "%d")
      }
      break;
  }

  (*func)->polynomial=p;
  _members=ll_delete_entire_list(_members);
}

