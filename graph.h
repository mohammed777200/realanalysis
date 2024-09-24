
#pragma once
#include "structs.h"
#include "type.h"
#include "set.h"

Graph *graph_create();
Graph *graph_insert(Graph *, double x, double y);
Node *graph_intersects(Graph *f, Graph *g);
Graph *graph_destroy(Graph *graph);
void graph_plot(Graph *);
