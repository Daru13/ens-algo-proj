//------------------------------------------------------------------------------
// DIJKSTRA'S ALGORITHM
//------------------------------------------------------------------------------
// Implementation of Dijkstra's algorithm, in two versions:
// - a naive one with a basic priority structure
// - an optimized one with a Fibonacci heap
//------------------------------------------------------------------------------

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"
#include "toolbox.h"
#include "graph.h"
#include "dijkstra.h"

int* dijkstraNaif (Graph* g ,int s) // O(#A²)
{
	int res[g->nb_vertexes];
	bool seen[g->nb_vertexes];
	for (int i = 0 ; i < g->vertexes ; i++)
	{
		seen[i]= false;
	}
	PrioList* F = createPrioList();
	Prio p = { s , 0};
	addElementToPrioList (F , p);
	while ( ! (prioListIsEmpty(F)))
	{
		Prio x = extractMinFromPrioList(F);
		if ( !seen[x.elt] )
		{
			int n = x.elt;
			res[n] = x.key;
			seen[n] = true;
			Edge* EL = g.egdes[n];
			while (EL != NULL)
			{
				Edge ed = popEList(EL);
				Prio p = {EL->destination , res[n] +EL->weight};
				addElementToPrioList(F,p);
				EL = EL->next;
			}
		}
	}
	return res;
}


