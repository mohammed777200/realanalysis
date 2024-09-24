// polynomial.c

#include "polynomial.h"
#include "linkedlist.h"

static void _polynomial_simpilification(Polynomial **poly);

Polynomial *poly_create(char *expr, Type dt)
{
  if(strlen(expr)==0||expr==NULL)
  {
    printf("you must give a polynomial expression!\n");
    return NULL;
  }
  Polynomial *_poly=alloc_type(POLYNOMIAL, 1);
  _poly->terms=NULL;
  _poly->polynomial=alloc_type(CHAR, strlen(expr)+1); // +1 is for \0 termination character
  strncpy(_poly->polynomial, expr, strlen(expr));
  polynomial(expr, &_poly , dt);
  _polynomial_simpilification(&_poly);
  return _poly;
}

Polynomial *poly_destroy(Polynomial *poly)
{
  if(poly==NULL)
  {
    printf("no polynomial defined!\n");
    return NULL;
  }
  poly->terms=ll_delete_entire_list(poly->terms);   poly->terms=NULL;
  if(poly->polynomial!=NULL)
  {
    free(poly->polynomial); poly->polynomial=NULL;
  }
  free(poly); poly=NULL;
  return poly;
}

uint n_length(const char *n)
{
  uint counter=0;
  uint i=0;
  while(isdigit(n[i]))
  {
    ++i;
    ++counter;
  }
  return counter;
}

#define N_VALUE(type, n, count)\
  type v=0;\
  if(count==0)\
    v=1;\
  for(uint i=0; i<count ; ++i)\
    v+=chartoint(n[count-i])*pow(10, i);\
    if((n[0]=='-'))\
    {\
      v=v*-1;\
      _is_negative=true;\
    }\
    else\
      _is_negative=false;\


#define GET_VALUE(type, n)\
  length=n_length(n+1);\
  type p=0;\
  if(length==0)\
    p=0;\
  else\
  {\
    N_VALUE(type, n, length);\
    p=v;\
    new_index+=length;\
  }

#define TERM(type, _expr)\
  bool _is_negative=false;\
  unsigned int length=0;\
  unsigned int _len=strlen(_expr);\
  char n[_len+1];\
  for(int i=0; i<_len+1; ++i)\
    n[i]='\0';\
  if(_expr[0]=='-'||_expr[0]=='+')\
    strcpy(n, _expr);\
  else\
  {\
    n[0]='+';\
    strncat(n, _expr, strlen(_expr)-_len-1);\
  }\
  unsigned int new_index=1;\
  length=n_length(n+new_index);\
  type p=0;\
  N_VALUE(type, n, length);\
  p=v;\
  new_index+=length;\
  if(n[new_index]=='.')\
  {\
    unsigned short _decimallen=0;\
    bool _is_p_negative=_is_negative;\
    _decimallen=n_length(n+new_index+1);\
    N_VALUE(type, (n+new_index), _decimallen);\
    p=(_is_p_negative)?p-(v/pow(10, _decimallen)):p+(v/pow(10, _decimallen));\
    new_index+=_decimallen+1;\
  }\
  if(n[new_index]=='/')\
  {\
    unsigned short _quotienlen=n_length(n+new_index+1);\
    N_VALUE(type, (n+new_index), _quotienlen);\
    p=p/v;\
    new_index+=_quotienlen+1;\
  }\
  type coe=(p==0)?0:p;\
  /* printf("The coefficient is equal to %f\n", coe);\ */\
  int _astrisk=0;\
  if(n[new_index]=='*')\
  {\
    _astrisk++;\
    new_index+=_astrisk;\
  }\
  type xpos=(type)0;\
  if(n[new_index]=='(')\
  {\
    if(n[new_index+2]==')')\
      new_index+=2;\
    else\
    {\
      new_index+=2;\
      GET_VALUE(type, (n+new_index));\
      xpos=p;\
      if(n[new_index+1]=='.')\
      {\
        unsigned int _decimallen=n_length(n+new_index+2);\
        GET_VALUE(type, (n+new_index+1));\
        xpos=(xpos<0)?xpos-(p/pow(10, _decimallen)):xpos+(p/pow(10, _decimallen));\
        new_index+=1;\
      }\
      else if(n[new_index+1]=='/')\
      {\
        unsigned int _decimallen=n_length(n+new_index+2);\
        GET_VALUE(type, (n+new_index+1));\
        xpos=xpos/p;\
        new_index+=1;\
      }\
      new_index+=1;\
    }\
    /* printf("the x position is %f\n", xpos);\ */\
  }\
\
  type _scale=1;\
  type e=1;\
  unsigned int _inx=new_index;\
  if(n[_inx]==EOF||n[_inx]=='\0'||n[_inx]==(char)0)\
    e=0;\
  else\
  {\
    new_index+=1;\
    if(n[new_index]=='^')\
    {\
      if(n[new_index+1]=='-')\
      {\
        new_index+=2;\
        _scale=-1;\
      }\
      else\
        new_index++;\
\
      length=n_length(n+new_index);\
      N_VALUE(type, (n+new_index-1), length)\
      e=v;\
      e=e*_scale*_scale;  /* I multiply it twice to override the negative condition in n_value function */\
\
      new_index+=length;\
      if(n[new_index]=='.')\
      {\
        unsigned int _decimallen=n_length(n+new_index+1);\
        GET_VALUE(type, (n+new_index));\
        e=(e<0)?e-(p/pow(10, _decimallen)):e+(p/pow(10, _decimallen));\
      }\
      else if(n[new_index]=='/')\
      {\
        GET_VALUE(type, (n+new_index));\
        e=e/p;\
      }\
    }\
  }\
\
  /* printf("The power is equal to %f\n", e);\ */\
\
  type*_tup=calloc(3, sizeof(type));\
  _tup[0]=coe;\
  _tup[1]=e;\
  _tup[2]=xpos;\

void polynomial(char *p, Polynomial **poly, Type dt)
{
// -------------------- split polynomial --------------------------
  unsigned int counter=0;
  char *_p=NULL;
  // set sign to the whole polynomial
  if(p[0]!='-'&&p[0]!='+')
  {
    _p=(char*)calloc(sizeof(char), strlen((char*)p)+strlen((char*)p)+1);
    _p[0]='+';
    strncat((char*)_p,(char*) p, strlen((char*)p)-strlen((char*)p)-1); 
  }
  else
  {
    _p=(char*)calloc(sizeof(char), strlen((char*)p)+1);
    strcpy((char*)_p,(char*) p);
  }

  for(register int i=0; i<strlen(_p); ++i)
  {
    if((i>1&&_p[i-2]=='(')||(_p[i]=='^'&&((_p[i+1]=='-')||(_p[i+1]=='+'))))
    {
      ++i;
      continue;
    }
    if(_p[i]=='-'||_p[i]=='+')
      counter++;
  }

  char _terms[counter][16];
  int k=0;
  for(register int i=0; i<counter; ++i)
  {
    for(register int j=0; j<16; ++j)
    {
      _terms[i][j]=_p[k];
      ++k;
      // for sign of power
      if(k<strlen(_p))
      {
        if((_p[k]=='^'&&((_p[k+1]=='-')||(_p[k+1]=='+'))))
        {
          ++j;
          _terms[i][j]=_p[k];
          ++k;
          continue;
        }
      }

      // check whether there is a parenthesis in x term
      if(k>1)
        if(_p[k-2]=='(')
        {
          ++j;
          _terms[i][j]=_p[k];
          k++;
        }

      // end the term if there is + or - sign as a term
      if(_p[k]=='+' || _p[k]=='-' || k>= strlen(_p))
      {
        _terms[i][j+1]='\0';
        break;
      }
    }
  }
  /* printf("terms :\n"); */
  /* for(int i=0; i<counter; i++) */
  /*   printf("%s\n", _terms[i]); */
// -------------------- split polynomial done --------------------------
  
  Term _term;
  for(int i=0; i<counter; ++i)
  {
    TERM(double, _terms[i])
    _term.coefficient=_tup[0];
    _term.power=_tup[1];
    _term.x_pos=_tup[2];
    (*poly)->terms=ll_insert((*poly)->terms, &_term, TERM);
    free(_tup);
  }

  free(_p);
}

void poly_add_term(Polynomial **poly, double c)
{
  if(c==0) return;
  double _c=c;
  if(_c<0) _c*=-1;

  char *_cc=inttostr(_c);
  int _clen=orderofnumber(_c);

  int _len=strlen((*poly)->polynomial)+_clen+2;

  char *_new_expr=alloc_type(CHAR, _len);

  strcat(_new_expr, (*poly)->polynomial);

  if(c<0) strcat(_new_expr, "-");
  else strcat(_new_expr, "+");

  strncat(_new_expr, _cc, _clen);
  free(_cc);

  free((*poly)->polynomial);  (*poly)->polynomial=NULL;
  (*poly)->polynomial=_new_expr;
}

//------------------------ private functions -----------------------------------

// combine the same term power and simplify coefficients
static void _polynomial_simpilification(Polynomial **poly)
{
  // new terms that is simplified
  Node *_new_terms=NULL;
  Set *_powers_set=set_create(DOUBLE); // freed
  Node *_terms=(*poly)->terms;

  Node *_ls_terms=_terms;        // start from first term

  while(_ls_terms!=NULL)
  {
    if(!set_belong(_powers_set, &((Term*)_ls_terms->data)->power))
      set_insert(_powers_set, &((Term*)_ls_terms->data)->power);
    _ls_terms=_ls_terms->next;
  }
  
  Node *_powers=set_get_members(_powers_set); // freed
  Node *_tmp_powers=_powers;

  set_destroy(_powers_set);

  Node *_uni_terms=NULL;
  Node *_uni_terms_tmp;

  _ls_terms=_terms;
  while(_tmp_powers!=NULL)
  {
    while(_ls_terms!=NULL)
    {
      if(*(double*)_tmp_powers->data==((Term*)_ls_terms->data)->power)
        _uni_terms=ll_insert(_uni_terms, _ls_terms->data, TERM);
      _ls_terms=_ls_terms->next;
    }

    Term _t;
    double _coe=0, _pow=0, _xpo=0;
    _uni_terms_tmp=_uni_terms;
    while(_uni_terms_tmp!=NULL)
    {
      _coe+=(((Term*)_uni_terms_tmp->data)->coefficient);
      _pow=(((Term*)_uni_terms_tmp->data)->power);
      _xpo+=(((Term*)_uni_terms_tmp->data)->x_pos);
      _uni_terms_tmp=_uni_terms_tmp->next;
    }

    _t.coefficient=_coe;
    _t.power=_pow;
    _t.x_pos=_xpo;

    _new_terms=ll_insert(_new_terms, &_t, TERM);
    _ls_terms=_terms;
    _tmp_powers=_tmp_powers->next;
    ll_delete_entire_list(_uni_terms);
    _uni_terms=NULL;
  }

  _powers=ll_delete_entire_list(_powers);  _powers=NULL;


  ll_delete_entire_list((*poly)->terms); (*poly)->terms=NULL;
  (*poly)->terms=_new_terms;
}

