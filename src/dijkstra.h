//------------------------------------------------------------------------------
// DIJKSTRA
//------------------------------------------------------------------------------

#ifndef _DIJKSTRA_H
#define _DIJKSTRA_H

#include "graph.h"
#include <limits.h>

// Unreachable length (all lengths are positive or null)
#define INF_LENGTH -1
#define MAX_LENGTH INT_MAX // the larger value a 32 bits signed integer can hold

//------------------------------------------------------------------------------

int extractMinimumNaive(bool*, int*, int);
int* dijkstraNaive(Graph* g, int s);
int* dijkstra (Graph* g, int s);

#endif
