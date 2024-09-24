#include <stdio.h>
#include <limits.h>
#include <plplot/plplot.h>
#include <float.h>
#include "graph.h"
#include "set.h"

void starting();
void exiting();

void __attribute__ ((constructor)) starting();
void __attribute__ ((destructor)) exiting();


void starting()
{
  printf("Booting process initiated!\n");
}

void exiting()
{
  printf("exit program!\n");
}

float domain1[10]  ={ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
float codomain1[10]={ 9, 8.3, 3.4, 4.1, 5.99, 7, 1, 3, 5, 9 };


float domain2[10]  ={ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
float codomain2[10]={ 4, 9, 3, 1, 7, 6.1, 12, 1, 8, 12 };


int main(int argc, char **argv)
{
  Graph *g1=graph_create();
  Graph *g2=graph_create();

  for(int i=0; i<10; ++i)
    g1=graph_insert(g1, domain1[i], codomain1[i]);

  for(int i=0; i<10; ++i)
    g2=graph_insert(g2, domain2[i], codomain2[i]);


  Set *_domains[2]   = { g1->domain, g2->domain };
  Set *_codomains[2] = { g1->codomain, g2->codomain };

  Node *pts=graph_intersects(g1, g2);

  pt_delete_points(pts);

  /* set_plots(_domains, _codomains, 2, pts, "X", "Y", "Coordinate"); */
  graph_destroy(g1);
  graph_destroy(g2);

  getchar();
  return 0;
}
