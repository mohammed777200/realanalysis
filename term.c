
#include "term.h"
#include "type.h"

Term *term_create()
{
  Term *_term=alloc_type(TERM, 1);
  _term->coefficient=_term->power=_term->x_pos=0;
  return _term;
}

Term *term_destroy(Term *term)
{
  free(term);   term=NULL;
  return term;
}
