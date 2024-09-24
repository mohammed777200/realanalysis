// set.c

#include "set.h"
#include "linkedlist.h"
#include "structs.h"
#include "type.h"
#include "plot.h"

extern bool pl_is_init;

#define HT_LENGTH 20

Set *set_create(Type dt)
{
  Set *_s=(Set*)alloc_type(SET, 1);
  if(_s==NULL)
  {
    printf("failure in allocating memory for set\n");
    free(_s);  _s = NULL;
    return NULL;
  }

  // get hash function
  _s->members = ht_create_table();
  if(!_s->members)
  {
    printf("failure in allocating memory for members\n");
    free(_s->members); _s->members = NULL;
    free(_s);          _s = NULL;
    return NULL;
  }
  _s->cardinal=0;
  _s->data_type=dt;
  _s->delta=NULL;
  return _s;
}

Set *set_insert(Set *s, void *member)
{
  s->members=ht_insert(s->members, member, s->data_type);
  s->cardinal++;
  return s;
}

Set *set_get_copy(Set *s)
{
  if(s==NULL)
  {
    printf("set is undefined!\n");
    return NULL;
  }
  Set *_s=set_create(s->data_type);
  if(s->cardinal==0)
    return _s;

  Node *_members=set_get_members(s);
  Node *members=_members;

  while(members!=NULL)
  {
    _s=set_insert(_s, members->data);
    members=members->next;
  }
  ll_delete_entire_list(_members);
  return _s;
}

Set *set_get_pairs(Set *domain, Set *codomain)
{
  if(domain==NULL||codomain==NULL)
  {
    printf("either one of the sets not defined!\n");
    return NULL;
  }
  if(domain->cardinal!=codomain->cardinal)
  {
    printf("domain and codomain are not match!\n");
    return NULL;
  }

  printf("\n");

  Set *res=set_create(POINT);
  Node *_table1[HT_LENGTH], *_table2[HT_LENGTH];
  for(int i=0; i<HT_LENGTH; i++)
  {
   _table1[i]=domain->members->table[i];
   _table2[i]=codomain->members->table[i];
  }
  Point _p;
  int _inx=0, _jnx=0;
  Node *_index=domain->members->sorts;
  Node *_jndex=codomain->members->sorts;
  Node *_tmp=NULL;
  while(_index!=NULL && _jndex!=NULL)
  {
    _inx=*(int*)_index->data;
    _tmp=domain->members->table[_inx];
    if(_tmp!=NULL)
    {
      _p.x=_tmp->data;
      domain->members->table[_inx]=domain->members->table[_inx]->next;
    }
    _jnx=*(int*)_jndex->data;
    _tmp=codomain->members->table[_jnx];
    if(_tmp!=NULL)
    {
      _p.y=_tmp->data;
      codomain->members->table[_jnx]=codomain->members->table[_jnx]->next;
    }
    res=set_insert(res, &_p);
    _index=_index->next;
    _jndex=_jndex->next;
  }
  for(int i=0; i<HT_LENGTH; i++)
  {
    domain->members->table[i]   =_table1[i];
    codomain->members->table[i] =_table2[i];
  }
  return res;
}


Set *set_add_const(Set *s, double c)
{
  Set *_tmpset=set_create(s->data_type);
  Node *members=set_get_members(s);
  Node *_members=members;
  double _member=0;
  while(_members!=NULL)
  {
    _member=(*(double*)_members->data)+c;
    _tmpset=set_insert(_tmpset, &_member);
    _members=_members->next;
  }
  set_destroy(s); s=NULL;
  ll_delete_entire_list(members); members=NULL;
  s=_tmpset;  _tmpset=NULL;
  return s;
}

Node *set_get_members(Set *s)
{
  if(s==NULL)
  {
    printf("set is not defined\n");
    return NULL;
  }
  Node *table[HT_LENGTH];
  for(int i=0; i<HT_LENGTH; i++)
    table[i]=s->members->table[i];
  Node *list=NULL;
  unsigned int inx=0;
  Node *index=s->members->sorts;
  Node *tmp=NULL;
  while(index!=NULL)
  {
    inx=*(int*)index->data;
    tmp=s->members->table[inx];
    if(tmp!=NULL)
    {
      list=ll_insert(list, tmp->data, s->data_type);
      s->members->table[inx]=s->members->table[inx]->next;
    }
    index=index->next;
  }
  for(int i=0; i<HT_LENGTH; i++)
    s->members->table[i]=table[i];
  return list;
}

Set *set_destroy(Set *s)
{
  if(s==NULL)
  {
    /* printf("Set is empty!\n"); */
    return NULL;
  }

  if(s->members!=NULL) 
  { 
    if(s->members->sorts!=NULL) ll_delete_entire_list(s->members->sorts); 
    s->members->sorts = NULL; 
    ht_delete(s->members);  s->members=NULL;
  }
  if(s->delta!=NULL) { free(s->delta); s->delta=NULL; }
  free(s);   s=NULL;
  return s;
}

// set operations
Set *set_intersect(Set *res, Set *s1, Set *s2)
{
  if(s1==NULL||s2==NULL)
  {
    printf("some set is not defined!\n");
    return NULL;
  }
  if(res==NULL)
  {
    res=set_create(s1->data_type);
  }
  Node *table[s1->members->table_count];
  for(int i=0; i<s1->members->table_count; i++)
    table[i]=s1->members->table[i];
  int inx=0;
  Node *index=s1->members->sorts;
  Node *tmp = NULL;
  while(index!=NULL)
  {
    inx=*(int*)index->data;
    tmp = s1->members->table[inx];
    if(tmp!=NULL)
    {
      if(set_belong(s2, tmp->data) && !set_belong(res, tmp->data))
        res = set_insert(res, tmp->data);
      s1->members->table[inx]=s1->members->table[inx]->next;
    }
    index=index->next;
  }
  for(int i=0; i<s1->members->table_count; i++)
    s1->members->table[i]=table[i];
  return res;
}

Set *set_union(Set *res, Set *s1, Set *s2)
{
  if(res==NULL)
  {
    printf("create a set fist\n");
    return NULL;
  }

  Node *table1[s1->members->table_count], *table2[s2->members->table_count];
  for(int i=0; i<s1->members->table_count; i++)
  {
    table1[i]=s1->members->table[i];
    table2[i]=s2->members->table[i];
  }
  int inx=0, jnx=0;
  Node *index=s1->members->sorts;
  Node *jndex=s2->members->sorts;
  Node *tmp = NULL;
  while(index!=NULL)
  {
    inx=*(int*)index->data;
    tmp = s1->members->table[inx];
    if(tmp!=NULL)
    {
      if(!set_belong(res, tmp->data))
        res = set_insert(res, tmp->data);
      s1->members->table[inx]=s1->members->table[inx]->next;
    }
    if(jndex!=NULL)
    {
      jnx=*(int*)jndex->data;
      tmp = s2->members->table[jnx];
      if(tmp!=NULL)
      {
        if(!set_belong(res, tmp->data))
          res = set_insert(res, tmp->data);
        s2->members->table[jnx]=s2->members->table[jnx]->next;
        jndex=jndex->next;
      }
    }
    index=index->next;
  }

  for(int i=0; i<s1->members->table_count; i++)
  {
    s1->members->table[i]=table1[i];
    s2->members->table[i]=table2[i];
  }
  return res;
}

void *set_first_mem(Set *s)
{
  if(s==NULL)
  {
    printf("set is not defined!\n");
    return NULL;
  }
  unsigned int _inx=*(unsigned int*)s->members->sorts->data;

  void *_data=s->members->table[_inx]->data;
  return _data;
}

void *set_last_mem(Set *s)
{
  if(s==NULL)
  {
    printf("set is not defined!\n");
    return NULL;
  }
  unsigned int _inx=*(unsigned int*)ll_get_last_Node(s->members->sorts)->data;
  Node *_last=ll_get_last_Node(s->members->table[_inx]);
  void *_data=_last->data;
  return _data;
}


void *set_least_upper_bound(Set *s)
{
  double *_max=(double*)alloc_type(DOUBLE, 1);
  Node *_members=set_get_members(s);
  Node *_tmp=_members;

  double max=*(double*)_tmp->data;
  while(_tmp!=NULL)
  {
    if(*(double*)_tmp->data>=max)
      max=*(double*)_tmp->data;
    _tmp=_tmp->next;
  }
  *_max=max;
  ll_delete_entire_list(_members);
  return _max;
}

void *set_greatest_lower_bound(Set *s)
{
  double *_min=(double*)alloc_type(DOUBLE, 1);
  Node *_members=set_get_members(s);
  Node *_tmp=_members;
  double min=*(double*)_tmp->data;

  double _cur;
  while(_tmp!=NULL)
  {
    _cur=*(double*)_tmp->data;
    if(_cur<=min)
      min=_cur;

    _tmp=_tmp->next;
  }
  *_min=min;
  ll_delete_entire_list(_members);
  return _min;
}


double set_calc_slope(Set *s1, Set *s2)
{
  if(s1==NULL||s2==NULL) return INFINITY;

  double _xa=*(double*)set_first_mem(s1);
  double _xb=*(double*)set_last_mem(s1);

  
  double _ya=*(double*)set_first_mem(s2);
  double _yb=*(double*)set_last_mem(s2);

  double _slope=(_yb-_ya)/(_xb-_xa);
  return _slope;
}


//++++++++++++++++++++++++++++++++++++++++++++++++ PLOT SETS ON GRAPHS ++++++++++++++++++++++++++++++++++++++++++++++++++

// plot a set on a visual graph
void set_plot(Set *domain, Set *codomain, Point *intersection, const char *xlabel, const char *ylabel, const char *glabel) 
{ 
  pl_plot_points(domain, codomain, intersection, xlabel, ylabel, glabel); 
}

static void _set_init_points(Set *domain, Set *codomain, unsigned int cardinal, PLFLT *x, PLFLT *y)
{
  // plotting first function
  Node *l_domain=set_get_members(domain);
  Node *l_codomain=set_get_members(codomain);
  Node *_domain=l_domain, *_codomain=l_codomain;
  unsigned int i=0;
  while(_domain!=NULL)
  {
    x[i]=*(double*)_domain->data;
    y[i]=*(double*)_codomain->data;

    ++i;
    _domain=_domain->next;
    _codomain=_codomain->next;
  }
  ll_delete_entire_list(l_domain);
  ll_delete_entire_list(l_codomain);
}

void set_plots(Set *domains[], Set *codomains[], size_t size, Node *pts, const char *xlabel, const char *ylabel, const char *glabel)
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
    _f_xmaxes[i]=(double*)set_least_upper_bound(domains[i]);

    // get the max value of interval of y axix
    _f_ymaxes[i]=(double*)set_least_upper_bound(codomains[i]);


    // get the min values of interval of x axix
    _f_xmines[i]=(double*)set_greatest_lower_bound(domains[i]);

    // get the min value of interval of y axix
    _f_ymines[i]=(double*)set_greatest_lower_bound(codomains[i]);

    // get cardianl for each function and insert it in a vector array
    _f_cardianls[i]=domains[i]->cardinal;
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
  /* plenv(_xmin, _xmax, _ymin, _ymax, 0, 1); */
  plenv(0, _xmax+1, 0, _ymax+1, 0, 1);
  pl_setting_labels(xlabel, ylabel, glabel);
  plwidth( 1.5 );

  // plotting sets

  unsigned int _cc=0;
  unsigned int i=0;
  for(int i=0; i<size; ++i)
  {
    // plotting first set
    plwidth( 2 );
    _set_init_points(domains[i], codomains[i], _cardinal, x, y);

    _cc=(i%15==0)?MAGENTA:CYAN;
    plcol0(_cc);
    double _p1=*(double*)set_last_mem(domains[i]);
    double _p2=*(double*)set_last_mem(codomains[i]);
    plpoin(_cardinal, x+i, y+i, 17);
    plline(_cardinal, x, y);
  }

  _cc=0;
  Node *_tmp_pts=pts;
  while(_tmp_pts!=NULL)
  {
    _cc=(_cc%15==0)?_cc+1:_cc;
    Point *_solution=(Point*)_tmp_pts->data;
    plcol0(_cc);
    char _string[32];
    sprintf(_string, "#d#d#fn(%.2lf, %.2lf)", *(double*)_solution->x, *(double*)_solution->y);
    plptex(*(double*)_solution->x, *(double*)_solution->y-.4, 0, 0, 0, _string);
    PLFLT _f=0;
    void *_fake=&_f;
    plpoin(1, _solution->x, _solution->y, 9);
    /* plpoin(1, _solution->x, _fake, 17); // 17 */
    /* plpoin(1, _fake, _solution->y, 17); */
    /* // draw solution intersection */
    /* plpath(_cardinal, *(double*)_solution->x, 0, *(double*)_solution->x, *(double*)_solution->y); */
    /* plpath(_cardinal, *(double*)_solution->x, *(double*)_solution->y, 0, *(double*)_solution->y); */
    _tmp_pts=_tmp_pts->next;
    _cc++;
  }

  // clean things up
  for(unsigned int i=0; i<size; ++i)
  {
    free(_f_xmaxes[i]); free(_f_ymaxes[i]);
    free(_f_xmines[i]); free(_f_ymines[i]);
  }
  plend();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

bool set_equality(Set *s1, Set *s2)
{
  if(set_is_subset(s1, s2) && set_is_subset(s2, s1))
    return true;
  return false;
}

bool set_is_equivalent(Set *s1, Set *s2)
{
  if(s1->cardinal == s2->cardinal)
    return true;
  return false;
}
  
bool set_is_proper_subset(Set *s1, Set *s2)
{
  if(set_is_subset(s1, s2) && !set_is_subset(s2, s1))
    return true;
  return false;
}

bool set_is_subset(Set *s1, Set *s2)
{
  if(set_is_empty(s1))
    return true;
  int i=0;
  bool t=true;
  Node *tmp = NULL;
  while(i<s1->members->table_count)
  {
    tmp = s1->members->table[i];
    while(tmp!=NULL)
    {
      if(!set_belong(s2,tmp->data))
      {
        t=false;
        break;
      }
      tmp = tmp->next;
    }
    if(!t)
      break;
    i++;
  }
  return t;
}

Set *set_power(Set *res, Set *s)
{
  Set ss=*s;
  Node *table[20];
  for(int i=0; i<20; i++)
    table[i]=s->members->table[i];
  res = set_create(SET);
  res = set_insert(res, res);
  res = set_insert(res, s);
  //---------------------------------------------------------------
  int inx = 0;
  Node *index = s->members->sorts;
  Node *tmp = NULL;
  Set *tmpset = NULL;
  while(index!=NULL)
  {
    inx = *(int*)index->data;
    tmp=s->members->table[inx];
    tmpset=set_create(s->data_type);
    if(tmp!=NULL)
    {
      tmpset=set_insert(tmpset, tmp->data);
      res=set_insert(res, tmpset);
      if(ll_count_of_list(tmp)>1)
        s->members->table[inx]=s->members->table[inx]->next;
    }

    index=index->next;
  }
  for(int i=0; i<20; i++)
    s->members->table[i]=table[i];
  tmpset=NULL;
  //---------------------------------------------------------------



  
  // still maintaining
  int jnx=0, i=0;
  inx=0;
  index=s->members->sorts;
  tmp=NULL;
  tmpset=NULL;
  void *tmpmember=NULL;

  tmpset=set_create(s->data_type);
  // loop through set S and set S', where S=S'
  while(index->next!=NULL)  // S
  {
    inx=*(int*)index->data;
    tmp=s->members->table[inx];
    if(tmp!=NULL)
    {
      tmpmember=tmp->data;
      Node *jndex=ss.members->sorts->next;
      while(jndex!=NULL)  // S'
      {
        jnx=*(int*)jndex->data;
        Node *tmp2=table[jnx];
        tmpset=set_create(s->data_type);
        tmpset=set_insert(tmpset, tmpmember);
        while(!set_belong(tmpset, tmp2->data))
        {
          tmpset=set_insert(tmpset, tmp2->data);
          res=set_insert(res, tmpset);
          if(ll_count_of_list(tmp2)>1)
            ss.members->table[jnx]=ss.members->table[jnx]->next;
        }
        jndex=jndex->next;
      }
      if(ll_count_of_list(tmp)>1)
        s->members->table[inx]=s->members->table[inx]->next;
    }
    index=index->next;
  }

  for(int i=0; i<20; i++)
    s->members->table[i]=table[i];
  return res;
}


Set *set_complement(Set *res, Set *universal, Set *s)
{
  if(universal==NULL || s==NULL)
  {
    printf("universal set is empty!\n");
    return NULL;
  }
  res=set_difference(res, universal, s);
  return res;
}


Set *set_cart_prod(Set *res, Set *s1, Set *s2)
{
  if(s1 == NULL || s2 == NULL)
  {
    printf("Set is Null!\n");
    return NULL;
  }
  Node *i_list[20], *j_list[20];
  for (int i=0; i<20; i++)
  {
    i_list[i] = s1->members->table[i];
    j_list[i] = s2->members->table[i];
  }
  unsigned int inx=0, jnx=0;
  Node *i_index = s1->members->sorts, *j_index = s2->members->sorts;
  Node *a = NULL;
  Node *b = NULL;
  Point p;
  while(i_index!=NULL)
  {
    inx = *(int*)i_index->data;
    a = s1->members->table[inx];
    while(a!=NULL)
    {
      while(j_index!=NULL)
      {
        jnx = *(int*)j_index->data;
        b = s2->members->table[jnx];
        if(b!=NULL)
        {
          p.x = a->data;
          p.y = b->data;
          res = set_insert(res, &p);
          s2->members->table[jnx] = s2->members->table[jnx]->next;
          b=b->next;
        }
        j_index=j_index->next;
      }
      for(int i=0; i<20; i++)
        s2->members->table[i] = j_list[i];
      j_index=s2->members->sorts;
      if(ll_count_of_list(a)>1)
      {
        s1->members->table[inx] = s1->members->table[inx]->next;
        break;
      }
      a = a->next;
    }
    i_index=i_index->next;
  }
  for(int i=0; i<20; i++)
  {
    s1->members->table[i] = i_list[i];
    s2->members->table[i] = j_list[i];
  }
  return res;
}

Set *set_difference(Set *res, Set *s1, Set *s2)
{
  if(s1==NULL || s2==NULL)
  {
    printf("some of the sets are given is not defined!\n");
    return NULL;
  }
  Node *list[20];
  for(int i=0; i<s1->members->table_count; i++)
    list[i] = s1->members->table[i];
  unsigned int inx=0;
  Node *index=s1->members->sorts;
  Node *tmp=NULL;
  while(index!=NULL)
  {
    inx=*(int*)index->data;
    tmp=s1->members->table[inx];
    if(tmp!=NULL)
    {
      if(!set_belong(s2,tmp->data))
        res=set_insert(res, tmp->data);
      s1->members->table[inx]=s1->members->table[inx]->next;
    }
    index=index->next;
  }
  for(int i=0; i<s1->members->table_count; i++)
    s1->members->table[i]=list[i];
  return res;
}


void set_delete_member(Set **s, void *value, Type dt)
{
  (*s)->members=ht_delete_node((*s)->members, value, dt);
  (*s)->members->sorts=ll_delete_first_Node((*s)->members->sorts);
}

bool set_belong(Set *s, void *member)
{
  void *p=ht_search(s->members, member, s->data_type);
  if(p==NULL)
    return 0;
  return 1;
}

bool set_is_empty(Set *s)
{
  bool v=(s->cardinal)? false : true;
  return v;
}

static void _print_point_by_type(Point *p, Type dt)
{
  switch(dt)
  {
    case INT:
      {
        int _x_res=*(int*)p->x;
        if(_x_res==0)
          printf("\e[0;31m\e[5m(%d, %d)\e[0m", _x_res, *(int*)p->y);
        else
          printf("(%d, %d)", _x_res, *(int*)p->y);
      }
      break;
    case FLOAT:
      {
        float _x_res=*(float*)p->x;
        if(_x_res==0)
          printf("\e[0;31m\e[5m(%.2f, %.2f)\e[0m", _x_res, *(float*)p->y);
        else
          printf("(%.2f, %.2f)", _x_res, *(float*)p->y);
      }
      break;
    case DOUBLE:
      {
        double _x_res=*(double*)p->x;
        if(_x_res==0)
          printf("\e[0;31m\e[5m(%.2f, %.2f)\e[0m", _x_res, *(double*)p->y);
        else
          printf("(%.2f, %.2f)", _x_res, *(double*)p->y);
      }
      break;
    default:
    case LONG_DOUBLE:
      {
        long double _x_res=*(long double*)p->x;
        if(_x_res==0)
          printf("\e[0;31m\e[5m(%.2Lf, %.2Lf)\e[0m", _x_res, *(long double*)p->y);
        else
          printf("(%.2Lf, %.2Lf)", _x_res, *(long double*)p->y);
      }
      break;
      printf("(%c, %c)", *(char*)p->x, *(char*)p->y);
      break;
  }
}

void set_print_point(Set *s, Type dt)
{
  if(s == NULL)
  {
    printf("set is empty!\n");
    return;
  }
  printf("{ ");
  Node *members=set_get_members(s);
  Node *tmp=members;
  while(tmp!=NULL)
  {
    Point p = *(Point*)tmp->data;
    void *_x=p.x;
    void *_y=p.y;
    //_print_point_by_type(&p, dt);
    putchar('(');
    _set_print_by_type(_x, dt);
    printf(", ");
    _set_print_by_type(_y, dt);
    putchar(')');
    if(tmp!=NULL)
      printf(", ");
    tmp=tmp->next;
  }
  printf("\b\b }\n");
  members=ll_delete_entire_list(members);
}

void set_print(Set *s)
{
  if(s==NULL)
  {
    printf("set is not defined\n");
    return;
  }
  if(s->cardinal == 0)
  {
    printf("󰟢 ");
    return;
  }
  // recurisive function
  switch(s->data_type)
  {
    case SET:
      {
        Node *tmp=set_get_members(s);
        printf("{ ");
        while(tmp!=NULL)
        {
          set_print((Set*)tmp->data);
          printf(", ");
          tmp=tmp->next;
        }
        printf("\b\b }");
        return;
      }
    default:
      break;
  }

  Node *members=set_get_members(s);
  Node *tmp=members;
  printf("{ ");
  while(tmp!=NULL)
  {
    _set_print_by_type(tmp->data, s->data_type);
    if(tmp != NULL)
      printf(", ");
    tmp=tmp->next;
  }
  printf("\b\b }");
  members=ll_delete_entire_list(members);
}

static void _set_print_by_type(void *n, Type dt)
{
  switch(dt)
  {
    case CHAR:
      printf("%c", *(char*)n);
      break;
    case INT:
      {
        int _v=*(int*)n;
        if(_v==0)
          printf("\e[0;31m\e[5m%d\e[0m", *(int*)n);
        else
        printf("%d", *(int*)n);
      }
      break;
    case FLOAT:
      {
        float _v=*(float*)n;
        float _v_abs=(_v<0)?_v*-1:_v;
        if(_v==0)
          printf("\e[0;31m\e[5m%.1f\e[0m", _v);
        else if(_v_abs<0.00005)
          printf("%.9f", _v);
        else
          printf("%.2f", _v);
      }
      break;
    case DOUBLE:
    {
      double _v=*(double*)n;
      double _v_abs=(_v<0)?_v*-1:_v;
      if(_v==0)
        printf("\e[0;31m\e[5m%.1lf\e[0m", _v);
      else if(_v_abs<0.00005)
        printf("%.9lf", _v);
      else
        printf("%.2lf", _v);
    }
    break;
    case LONG_DOUBLE:
    {
      long double _v=*(long double*)n;
      long double _v_abs=(_v<0)?_v*-1:_v;
      if(_v==0)
        printf("\e[0;31m\e[5m%.1Lf\e[0m", _v);
      else if(_v_abs<0.00005)
        printf("%.9Lf", _v);
      else
        printf("%.2Lf", _v);
    }
    break;
    default:
      printf("%d", *(int*)n);
      break;
  }
  return;
}
