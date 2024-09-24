

#include "graph.h"

static bool plot_established=false;

static double _abs(double x)
{
  return (x<0)?x*-1:x;
}

Graph *graph_create()
{
  Graph *_new_graph=(Graph*)alloc_type(GRAPH, 1);
  _new_graph->domain=set_create(DOUBLE);
  _new_graph->codomain=set_create(DOUBLE);
  return _new_graph;
}

Graph *graph_insert(Graph *graph, double x, double y)
{
  graph->domain=set_insert(graph->domain, &x);
  graph->codomain=set_insert(graph->codomain, &y);
  return graph;
}

static bool _is_not_func(Node *domain)
{
  // check if function is not a function
  bool _is_same_domain=false;
  Node *_tmp_domain=domain->next, *_tmp_domain_prev=domain;
  while(_tmp_domain!=NULL)
  {
    if(*(double*)_tmp_domain->data==*(double*)_tmp_domain_prev->data)
      _is_same_domain=true;
    else 
    {
      _is_same_domain=false;
      break;
    }
    _tmp_domain_prev=_tmp_domain;
    _tmp_domain=_tmp_domain->next;
  }
  return _is_same_domain;
}

static bool _is_same_domain(Node *domain1, Node *domain2)
{
  bool _is_same=true;
  Node *_tmp_d1=domain1, *_tmp_d2=domain2;
  while(_tmp_d1!=NULL&&_tmp_d2!=NULL)
  {
    if(*(double*)_tmp_d1->data!=*(double*)_tmp_d2->data)
    {
      _is_same=false;
      break;
    }
    _tmp_d1=_tmp_d1->next;
    _tmp_d2=_tmp_d2->next;
  }
  return _is_same;
}

typedef struct Line
{
  double m;
  double p;
}Line;

// x_a=3, x_b=1.5, y_a=8, y_b=4
Line *_line_form(double x_a, double x_b, double y_a, double y_b)
{
  double _m=(y_b-y_a)/(x_b-x_a);
  double _p=y_a-_m*(x_a);
  Line *_l=malloc(sizeof(Line));
  _l->m=_m; _l->p=_p;
  return _l;
}

double _solve_equation(Line *l1, Line *l2)
{
  double _solution=((l1->p-l2->p)/(l1->m-l2->m))*-1;
  return _solution;
}

double _equation_calc(Line *l, double x)
{
  double _solution=l->m*x+l->p;
  return _solution;
}

Set *_codomain_generate_pts(Set *res, Node *domain, Node *codomain, Node *unified_domain)
{
  Line *_line_tmp=NULL;
  Node *_codomain=codomain;
  Node *_domain=domain;
  Node *_new_domain_tmp=unified_domain;

  while(_codomain->next!=NULL&&_new_domain_tmp!=NULL&&_domain!=NULL)
  {
    double _x_a=*(double*)_domain->data;
    double _x_b=*(double*)_domain->next->data;

    double _y_a=*(double*)_codomain->data;
    double _y_b=*(double*)_codomain->next->data;

    _line_tmp=_line_form(_x_a, _x_b, _y_a, _y_b);

    double _solution=_equation_calc(_line_tmp, *(double*)_new_domain_tmp->data);

    res=set_insert(res, &_solution);

    /* if(!set_belong(_new_codomain, &_solution)) */
    if(*(double*)_domain->next->data>*(double*)_new_domain_tmp->data)
    {
      _solution=_equation_calc(_line_tmp, *(double*)_new_domain_tmp->data);
      if(!set_belong(res, &_solution))
        res=set_insert(res, &_solution);
      _new_domain_tmp=_new_domain_tmp->next;
    }
    else
    {
      _codomain=_codomain->next;
      _domain=_domain->next;
      _new_domain_tmp=_new_domain_tmp->next;
    }
    free(_line_tmp);
  }
  return res;
}

// return linkedlist of intersection points in graph f and g, we can free the list using
// pt_delete_points function to free points memory
Node *graph_intersects(Graph *f, Graph *g)
{
  Node *_points=NULL;

  Node *_f_domain   =set_get_members(f->domain); // need to free
  Node *_g_domain   =set_get_members(g->domain); // need to free

  Node *_f_codomain =set_get_members((f->codomain)); // need to free 
  Node *_g_codomain =set_get_members((g->codomain)); // need to free

  // check if function is not a function

  bool _is_f_constant_function=_is_not_func(_f_codomain);
  bool _is_g_constant_function=_is_not_func(_g_codomain);

  if(_is_f_constant_function&&_is_g_constant_function)
  {
    printf("flag 1 \n");
    Set *__new_domain=set_create(DOUBLE);
    Set *__new_codomain=set_create(DOUBLE);
      
    __new_domain=set_union(__new_domain, f->domain, g->domain);

    _f_domain=ll_delete_entire_list(_f_domain);   _f_domain=NULL;
    _g_domain=ll_delete_entire_list(_g_domain);   _g_domain=NULL;

    _f_domain=set_get_members(__new_domain);
    _g_domain=ll_get_copy(_f_domain, DOUBLE);

    unsigned int __new_cardinal=__new_domain->cardinal, i=0;
    while(i<__new_cardinal)
    {
      __new_codomain=set_insert(__new_codomain, _f_codomain->data);
      i++;
    }

    _f_codomain=ll_delete_entire_list(_f_codomain); _f_codomain=NULL;
    _g_codomain=ll_delete_entire_list(_g_codomain); _g_codomain=NULL;
    
    _f_codomain=set_get_members(__new_codomain);
    _g_codomain=ll_get_copy(_f_codomain, DOUBLE);

    __new_codomain=set_destroy(__new_codomain);
    __new_domain=set_destroy(__new_domain);
  }

  bool _is_g_not_func=_is_not_func(_g_domain);
  bool _is_f_not_func=_is_not_func(_f_domain);

  // if nor f a function neither g a function
  if(_is_f_not_func&&_is_g_not_func)
  {
    Node *_swap=_f_domain;
    _f_domain=_f_codomain;
    _f_codomain=_swap;

    _swap=_g_domain;
    _g_domain=_g_codomain;
    _g_codomain=_swap;
  }
  else
  {
    // if f is not a functionn and g is a function
    if(_is_g_not_func)  // f become g and g become f
    {
      Node *__swap=_g_codomain;
      _g_codomain=_f_codomain;
      _f_codomain=__swap;

      __swap=_g_domain;
      _g_domain=_f_domain;
      _f_domain=__swap;
    }
    _is_f_not_func=_is_not_func(_f_domain);
    if(_is_f_not_func)
    {
      _f_codomain=ll_delete_entire_list(_f_codomain);   _f_codomain=NULL;
      _f_codomain=ll_get_copy(_f_domain, DOUBLE);
    }
    _is_g_constant_function=_is_not_func(_g_codomain);
    if(_is_f_not_func&&_is_g_constant_function)
    {
      Point _p;
      double *_x=alloc_type(DOUBLE, 1), *_y=alloc_type(DOUBLE, 1);
      *_x=*(double*)_f_domain->data;
      *_y=*(double*)_g_codomain->data;
      _p.x=_x;
      _p.y=_y;
      _points=ll_insert(_points, &_p, POINT);
      return _points;
    }
  }

  if(*(double*)_f_codomain->data<*(double*)_g_codomain->data)
  {
    Node *__swap=_g_codomain;
    _g_codomain=_f_codomain;
    _f_codomain=__swap;

    __swap=_f_domain;
    _f_domain=_g_domain;
    _g_domain=__swap;
  }


  // ----------- unify domain --------------
  if((!_is_f_not_func&&!_is_g_not_func)&&(!_is_f_constant_function&&!_is_g_constant_function))
  {
    // unify the domain with each other

    Set *_new_domain_set=set_create(DOUBLE);
    _new_domain_set=set_union(_new_domain_set, f->domain, g->domain);
    Node *_new_domain=set_get_members(_new_domain_set);
    ll_sort(&_new_domain);

    // calculating the points 
    Set *_new_f_codomain=set_create(DOUBLE);
    Set *_new_g_codomain=set_create(DOUBLE);
    _new_f_codomain=_codomain_generate_pts(_new_f_codomain, _f_domain, _f_codomain, _new_domain);
    _new_g_codomain=_codomain_generate_pts(_new_g_codomain, _g_domain, _g_codomain, _new_domain);

    // clean old result for another new result
    ll_delete_entire_list(_f_domain);   _f_domain=NULL;
    ll_delete_entire_list(_g_domain);   _g_domain=NULL;
    ll_delete_entire_list(_f_codomain); _f_codomain=NULL;
    ll_delete_entire_list(_g_codomain); _g_codomain=NULL;

    // refeeding the slots to new data
    _f_domain=ll_get_copy(_new_domain, DOUBLE);
    _f_codomain=set_get_members(_new_f_codomain);
    _g_codomain=set_get_members(_new_g_codomain);

    // clean things up
    _new_domain=ll_delete_entire_list(_new_domain); _new_domain=NULL;
    _new_domain_set=set_destroy(_new_domain_set);
    _new_f_codomain=set_destroy(_new_f_codomain); _new_f_codomain=NULL;
    _new_g_codomain=set_destroy(_new_g_codomain); _new_g_codomain=NULL;
  }

  // if this domain is unused, then free
  if(_g_domain!=NULL) ll_delete_entire_list(_g_domain);

  // start calculation after above checking

  Node *_domain_top=ll_get_copy(_f_domain, DOUBLE); // need to free
  ll_inverse_list(&_domain_top);

  Node *_f_cod_tmp=_f_codomain, *_g_cod_tmp=_g_codomain;

  Node *_domain_tmp=_f_domain;
  Node *_domain_top_tmp=_domain_top;

  Node *_f_cod_prev_tmp=_f_cod_tmp;
  Node *_g_cod_prev_tmp=_g_cod_tmp;


  Node *_domain_prev_tmp=_domain_tmp;
  Node *_domain_prev_top_tmp=_domain_top_tmp;

  while(_f_cod_tmp!=NULL&&_g_cod_tmp!=NULL)
  {
    double _g_codomain_num=*(double*)_g_cod_tmp->data;
    double _f_codomain_num=*(double*)_f_cod_tmp->data;
    double _ref=_f_codomain_num-_g_codomain_num;

    if(_ref<=0)
    {
      Point _p;
      double *_x=alloc_type(DOUBLE, 1);
      double *_y=alloc_type(DOUBLE, 1);

      if(_ref==0)
      {
        *_x=*(double*)_domain_tmp->data;
        _p.x=_x;

        *_y=*(double*)_g_cod_tmp->data;
        _p.y=_y;
      }
      else if(_ref<0)
      {
        // start calculation derivative
        double _epsilon=0.00001f;

        // limits in x
        {
          double _domain_delta_y=(*(double*)_domain_tmp->data-*(double*)_domain_prev_tmp->data);
          double _f_x_delta=(*(double*)_f_cod_prev_tmp->data-*(double*)_g_cod_prev_tmp->data)*-1;
          double _g_x_delta=(*(double*)_f_cod_tmp->data-*(double*)_g_cod_tmp->data)*-1;
          double _f_slope_inverse=(_domain_delta_y/_f_x_delta)*_epsilon;
          double _g_slope_inverse=(_domain_delta_y/_g_x_delta)*_epsilon;

          double _f_inv_val=*(double*)_domain_prev_tmp->data;
          double _g_inv_val=*(double*)_domain_tmp->data;

          while(_f_inv_val!=_g_inv_val)
          {
            _f_inv_val+=_g_slope_inverse;
            _g_inv_val+=_f_slope_inverse;
            if(_g_inv_val-_f_inv_val<_epsilon)
            {
              *_x=_g_inv_val;
              _p.x=_x;
              break;
            }
          }
        }

        // limits in y
        {
          double _g_val=*(double*)_g_cod_prev_tmp->data;
          double _f_val=*(double*)_f_cod_prev_tmp->data;
          
          double _delta_x=*(double*)_domain_prev_top_tmp->data-*(double*)_domain_top_tmp->data;

          double _g_delta_y=*(double*)_g_cod_prev_tmp->next->data-*(double*)_g_cod_prev_tmp->data;

          double _f_delta_y=*(double*)_f_cod_prev_tmp->next->data-*(double*)_f_cod_prev_tmp->data;

          double _g_slope=(_g_delta_y/_delta_x)*_epsilon;
          double _f_slope=(_f_delta_y/_delta_x)*_epsilon;

          while((double)_g_val!=(double)_f_val)
          {
            _g_val+=_g_slope;
            _f_val+=_f_slope;
            if((_f_val-_g_val)<_epsilon)
              break;
          }
          *_y=_g_val;
          _p.y=_y;
        }
        if(_is_f_not_func)
        {
          void *_tmp_x=_p.x;
          _p.x=_p.y;
          _p.y=_tmp_x;
        }
        if(_abs(*(double*)_p.x)!=INFINITY&&_abs(*(double*)_p.y)!=INFINITY)
          _points=ll_insert(_points, &_p, POINT);

      }
      Node *_swap=_g_cod_tmp;
      _g_cod_tmp=_f_cod_tmp;
      _f_cod_tmp=_swap;
    }

    _domain_prev_top_tmp=_domain_top_tmp;
    _domain_top_tmp=_domain_top_tmp->next;

    _domain_prev_tmp=_domain_tmp;
    _domain_tmp=_domain_tmp->next;

    _f_cod_prev_tmp=_f_cod_tmp;
    _g_cod_prev_tmp=_g_cod_tmp;

    _f_cod_tmp=_f_cod_tmp->next;
    _g_cod_tmp=_g_cod_tmp->next;
    
  }
  // clean things up
  if(_f_domain!=NULL)   ll_delete_entire_list(_f_domain);
  if(_domain_top!=NULL) ll_delete_entire_list(_domain_top);
  if(_f_codomain!=NULL) ll_delete_entire_list(_f_codomain);
  if(_g_codomain!=NULL) ll_delete_entire_list(_g_codomain);
  return _points;
}

Graph *graph_destroy(Graph *graph)
{
  if(graph==NULL) return NULL;
  if(graph->domain!=NULL) { graph->domain=set_destroy(graph->domain);                      graph->domain=NULL; }
  if(graph->codomain!=NULL) { graph->codomain=set_destroy(graph->codomain);                graph->codomain=NULL;}
  free(graph); graph=NULL;
  return graph;
}

void graph_plot(Graph *graph)
{
  if(!plot_established)
  {
    plot_established=true;
  }
}
