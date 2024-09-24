
#include "point.h"
#include "linkedlist.h"

Point *pt_delete(Point *p)
{
  if(p==NULL) return NULL;
  free(p->x); free(p->y); p->x=p->y=NULL;
  free(p); p=NULL;
  return p;
}

Node *pt_delete_points(Node *pts)
{
  Node *_tmp=pts;
  while(_tmp!=NULL)
  {
    _tmp->data=pt_delete(_tmp->data);
    _tmp=_tmp->next;
  }

  pts=ll_delete_entire_list(pts);
  return pts;
}
