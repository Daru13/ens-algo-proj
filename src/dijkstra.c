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

/*
int* dijkstraNaive (Graph* g, int s) // O(#A²)
{
	int* lengths = malloc(g->nb_vertexes * sizeof(int));
	CHECK_MALLOC(lengths);

	bool seen[g->nb_vertexes];
	for (int i = 0 ; i < g->nb_vertexes ; i++)
		seen[i]= false;
	
	PrioList* F = createPrioList();
	Prio p = {s , 0};
	addElementToPrioList(F, p);

	while (! prioListIsEmpty(F))
	{
		Prio x = extractMinFromPrioList(F);
		if (! seen[x.elt])
		{
			int n = x.elt;
			lengths[n] = x.key;
			seen[n] = true;
			Edge* EL = g->edges[n];

			while (EL != NULL)
			{
				Edge ed = popFromList(EL);
				Prio p  = {EL->destination, lengths[n] + EL->weight};
				addElementToPrioList(F, p);
				EL = EL->next;
			}
		}
	}

	return lengths;
}
*/

#define INF_LENGTH -1

int extractMinimumNaive (bool* seen, int* lengths, int nb_vertexes, int*compt)
{
	int i;
	int min = -1;
	*compt = *compt +2;

	// Find the minimum
	for (i = 0; i < nb_vertexes; i++)
	{
		
		*compt = *compt +2;
		if (lengths[i] != INF_LENGTH
		&& !seen[i])
		{
			*compt = *compt + 1;
			if (min == -1)
			{
			min = i;
			*compt = *compt + 1;
			} 
			else 
			{
				*compt = *compt + 1;
				if (lengths[min] > lengths[i])
				{
				min = i;
				*compt = *compt + 1;
				}
			}

		}
	}


	return min;
}

int* dijkstraNaive (Graph* g, int s, int* compt) // O(#A²)
{
	// Initialization
	int* lengths = malloc(g->nb_vertexes * sizeof(int));
	CHECK_MALLOC(lengths);
	bool seen[g->nb_vertexes];
	*compt = *compt + g->nb_vertexes;

	for (int i = 0 ; i < g->nb_vertexes ; i++)
	{
		seen[i]    = false;
		lengths[i] = INF_LENGTH;
		*compt = *compt +2;
	}

	lengths[s] 		  = 0;
	*compt = *compt + 1;


	// Main loop (over the priority queue structure)
	for (int i = 0; i<g->nb_vertexes;i++)
	{
		// The element with the smallest distance is extracted
		int min_elt = extractMinimumNaive(seen,
			lengths, g->nb_vertexes, compt);
		seen[min_elt] = true;
		*compt = *compt + 1;
		// Iteration over all min_elt's neighbours
		Edge* current_edge = g->edges[min_elt];
		*compt = *compt + 1;
		while (current_edge != NULL)
		{
			// Each neighbour is added to the priority structure
			int neighbour = current_edge->destination;
			*compt = *compt + 1;
			// Improve the lengths if possible
			int weight = current_edge->weight;
			*compt = *compt + 1;
				
			*compt = *compt + 2;
			if (lengths[neighbour] == INF_LENGTH
			||  lengths[neighbour] > lengths[min_elt] + weight)
			{
				lengths[neighbour] = lengths[min_elt] + weight;
				*compt = *compt + 1;
			}
			current_edge = current_edge->next;
			*compt = *compt + 1;
		}
	}
	return lengths;
}

