
// plot.c

#include "plot.h"
#include "equation.h"


bool pl_is_init=false;

#define PLOT(type, _xmin, _xmax, _ymin, _ymax, _y, _x, length, x, y)\
   /* get x interval */\
  _xmin=*(type*)set_first_mem(func->domain);\
  _xmax=*(type*)set_last_mem(func->domain);\
   /* get y interval */\
  _ymin=*(type*)set_first_mem(func->codomain);\
  _ymax=*(type*)set_last_mem(func->codomain);\
  length=func->domain->cardinal;\
  Node *_domain=set_get_members(func->domain);\
  Node *_codomain=set_get_members(func->codomain);\
  int i=0;\
  while(_domain!=NULL && _codomain!=NULL)\
  {\
    _x=*(type*)_domain->data;\
    _y=*(type*)_codomain->data;\
    x[i] = _x;\
    y[i] = _y; \
    _domain=_domain->next;\
    _codomain=_codomain->next;\
    i++;\
  }\

void pl_plot(Function *func)
{
 double _xmax=0, _xmin=0, _ymax=0, _ymin=0;
 double _x=0;
 double _y=0;
 unsigned int _cardinal=func->domain->cardinal;
 double x[_cardinal], y[_cardinal];

 switch(func->domain->data_type)
 {
   case INT:
     {
       PLOT(int, _xmin, _xmax, _ymin, _ymax, _y, _x, _cardinal, x, y)
     }
     break;
   case FLOAT:
     {
       PLOT(float, _xmin, _xmax, _ymin, _ymax, _y, _x, _cardinal, x, y)
     }
     break;
   case DOUBLE:
     {
       PLOT(double, _xmin, _xmax, _ymin, _ymax, _y, _x, _cardinal, x, y)
     }
     break;
   case LONG_DOUBLE:
     {
       PLOT(long double, _xmin, _xmax, _ymin, _ymax, _y, _x, _cardinal, x, y)
     }
     break;
   default:
     {
       PLOT(float, _xmin, _xmax, _ymin, _ymax, _y, _x, _cardinal, x, y)
     }
     return;
 }

#ifndef PL_INIT
#define PL_INIT
plsdev("xcairo");
plinit();
#endif

 plfont(2);
 plcol0( 15 );
 if(_ymax<_ymin)
   _ymax=_ymin;

 // this function set the maximum unit of numbers
 plsetopt( "fflen", "2" );
 plenv(_xmin, _xmax, 0, _ymax+10, 0,1);
 plcol0( 4 );


 plcol0(2);
 plline(_cardinal, x, y);
 plcol0(4);
 plflush();


#ifndef PL_END
#define PL_END
plend();
#endif
}


static void _pl_init_points(Function *f, unsigned int cardinal, PLFLT *x, PLFLT *y, Point *solution)
{
  // plotting first function
  printf("\n\nthe cardinal is as follow %u\n\n", cardinal);
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
  x[i]=*(double*)solution->x;
  y[i]=*(double*)solution->y;
  ll_delete_entire_list(domain);
  ll_delete_entire_list(codomain);
}

void pl_two_functions(Function *f1, Function *f2, const char *xlabel, const char *ylabel, const char *glabel)
{
  if(f1==NULL||f2==NULL) return;
  Point *_solution_ptr=eq_linear(f1->polynomial->polynomial, f2->polynomial->polynomial, f1->domain->data_type); // get solution of two linear functions
  if(_solution_ptr==NULL)
  {
    Function *f[2]={ f1, f2 };
    func_plots(f, 2, NULL, NULL, NULL, NULL);
    return;
  }
plsdev("xcairo");
plinit();
plfont(2);

  // get the max value of interval of x axix
  const double _f1_xmax=*(double*)set_last_mem(f1->domain);
  const double _f2_xmax=*(double*)set_last_mem(f2->domain);

  // determine which is min(_f1_max, _f2_max)
  double _xmax=(_f1_xmax<=_f2_xmax)?_f1_xmax:_f2_xmax; // get the smallest max for x

  // get the max value of interval of y axix
  const double _f1_ymax=*(double*)set_last_mem(f1->codomain);
  const double _f2_ymax=*(double*)set_last_mem(f2->codomain);

  /* // determine which is min(_f1_min, _f2_min) */
  double _ymax=(_f1_ymax>=_f2_ymax)?_f1_ymax:_f2_ymax; // get the smallest min for y

  const unsigned int _cardinal=(f1->domain->cardinal<=f2->domain->cardinal)?f1->domain->cardinal+1:f2->domain->cardinal+1; // get the smallest cardianlity of f1 and f2

  PLFLT x[_cardinal], y[_cardinal]; // where the points are stores for plotting


plcol0(15);
plwidth( 1.5 );
plenv(0, _xmax+5, 0, _ymax+1, 0, 0);
if(xlabel==NULL||ylabel==NULL||glabel==NULL)
  pllab("(x)", "(y)", "f(x)");
else
  pllab(xlabel, ylabel, glabel);


  // plotting first function
plwidth( 3 );
  _pl_init_points(f1, _cardinal, x, y, _solution_ptr);
  plcol0(CYAN);
  plline(_cardinal, x, y);


  // plotting second function
plwidth( 3 );
  _pl_init_points(f2, _cardinal, x, y, _solution_ptr);
  plcol0(MAGENTA);
  plline(_cardinal, x, y);



  // label for intersection point
plwidth( 1.5 );
  plcol0(2);
  char _string[32];
  sprintf(_string, "(%.2lf, %.2lf)", *(double*)_solution_ptr->x, *(double*)_solution_ptr->y);
  plptex(*(double*)_solution_ptr->x, *(double*)_solution_ptr->y, 0, 0, -0.2, _string);
  PLFLT _f=0;
  void *_fake=&_f;
  plpoin(1, _solution_ptr->x, _solution_ptr->y, 17);
  plpoin(1, _solution_ptr->x, _fake, 17); // 17
  plpoin(1, _fake, _solution_ptr->y, 17);
  // draw solution intersection
  plpath(_cardinal, *(double*)_solution_ptr->x, 0, *(double*)_solution_ptr->x, *(double*)_solution_ptr->y);
  plpath(_cardinal, *(double*)_solution_ptr->x, *(double*)_solution_ptr->y, 0, *(double*)_solution_ptr->y);

  free(_solution_ptr->x);free(_solution_ptr->y);
  free(_solution_ptr);
plend();
}

void pl_setting_labels(const char *xlabel, const char *ylabel, const char *glabel)
{
#define _len 64
  char _x[_len]="(x)", _y[_len]="(y)", _g[_len]="f(x)";
  if(xlabel!=NULL) strncpy(_x, xlabel, strlen(xlabel)+1);
  if(ylabel!=NULL) strncpy(_y, ylabel, strlen(ylabel)+1);
  if(glabel!=NULL) strncpy(_g, glabel, strlen(glabel)+1);

  pllab(_x, _y, _g);
}

void pl_plot_points(Set *domain, Set *codomain, Point *intersection, const char *xlabel, const char *ylabel, const char *glabel)
{
  // get function input and output
  Set *_domain=domain;
  Set *_codomain=codomain;

  // get members of sets
  Node *_x=set_get_members(_domain);
  Node *_y=set_get_members(_codomain);
  
  Node *_xtmp=_x, *_ytmp=_y;

  double _xmax, _xmin, _ymax, _ymin;
  
  {
    void *_max=set_least_upper_bound(domain);
    void *_min=set_greatest_lower_bound(domain);
    double max=*(double*)_max;
    double min=*(double*)_min;
    free(_max);
    free(_min);

    // y min and max value, in other words, the interval in y axix
    _xmin=min;
    _xmax=max;
  }

  {
    void *_max=set_least_upper_bound(codomain);
    void *_min=set_greatest_lower_bound(codomain);
    double max=*(double*)_max;
    double min=*(double*)_min;
    free(_max);
    free(_min);

    // y min and max value, in other words, the interval in y axix
    _ymin=min;
    _ymax=max;
  }

  unsigned int _cardinal=_domain->cardinal;
  double x[_cardinal], y[_cardinal];

  plsdev("xcairo");
  plinit();
  plcol0(WHITE);
  plenv(_xmin,_xmax, _ymin, _ymax, 0, 1);
  pl_setting_labels(xlabel, ylabel, glabel);

  int i=0;
  while(_xtmp!=NULL&&_ytmp!=NULL)
  {
    x[i]=*(double*)_xtmp->data;
    y[i]=*(double*)_ytmp->data;
    int _i=(i<0)?i*-1:i;
    unsigned int _cc=(_i%15==0||_i%15==1)?(_i+2)%15:_i%15;
    plcol0(_cc);
    /* plpoin(1, x+i, y+i, 17); */

    _xtmp=_xtmp->next;
    _ytmp=_ytmp->next;
    i++;
  }
  plline(_cardinal, x, y);
  ll_delete_entire_list(_x);
  ll_delete_entire_list(_y);
  if(intersection!=NULL)
  {
    char _string[64];
    plcol0(RED);
    plwidth(0.3);
    sprintf(_string, "equilibrium (%.2lf, %.2lf)", *(double*)intersection->x, *(double*)intersection->y);
    /* plptex((*(double*)intersection->x), (*(double*)intersection->y), 0, 0, 1, _string); */
    PLFLT _f=0;
    void *_fake=&_f;
    plpoin(1, intersection->x, intersection->y, 9);
    plpoin(1, intersection->x, _fake, 17); // 17
    plpoin(1, _fake, intersection->y, 17);
    // draw solution intersection
    plpath(_cardinal, *(double*)intersection->x, 0, *(double*)intersection->x, *(double*)intersection->y);
    plpath(_cardinal, *(double*)intersection->x, *(double*)intersection->y, 0, *(double*)intersection->y);
    plwidth(1.7);
  }

  plend();
  return;
}
