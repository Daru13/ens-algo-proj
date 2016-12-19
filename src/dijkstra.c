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

int extractMinimumNaive (int* queue, int* lengths, int nb_elements)
{
	int i;
	int current_vertex = queue[0];

	// Find the minimum
	for (i = 1; i < nb_elements; i++)
		if (lengths[queue[i]] != INF_LENGTH
		&&	lengths[queue[i]]  < lengths[current_vertex])
		{
			int sauvegarde = current_vertex;
			current_vertex = queue[i];
			queue[i] = sauvegarde;
		}
	// Shift the table elements
	 for (int j = i; j < nb_elements - 1; j++)
		queue[j] = queue[j + 1];
	


	return current_vertex;
}

int* dijkstraNaive (Graph* g, int s) // O(#A²)
{
	// Initialization
	int* lengths = malloc(g->nb_vertexes * sizeof(int));
	CHECK_MALLOC(lengths);
	bool seen[g->nb_vertexes];

	for (int i = 0 ; i < g->nb_vertexes ; i++)
	{
		seen[i]    = false;
		lengths[i] = INF_LENGTH;
	}

	int priority_queue[g->nb_vertexes];
	priority_queue[0] = s;
	lengths[s] 		  = 0;

	int priority_queue_nb_elts = 1;

	// Main loop (over the priority queue structure)
	while (priority_queue_nb_elts > 0)
	{
		// The element with the smallest distance is extracted
		int min_elt = extractMinimumNaive(priority_queue,
			lengths, priority_queue_nb_elts);
		priority_queue_nb_elts--;
		// If it is the first time the vertex is encountered...
		if (! seen[min_elt])
		{
			seen[min_elt] = true;

			// Iteration over all min_elt's neighbours
			Edge* current_edge = g->edges[min_elt];
			while (current_edge != NULL)
			{
				// Each neighbour is added to the priority structure
				int neighbour = current_edge->destination;
				priority_queue[priority_queue_nb_elts] = neighbour;
				priority_queue_nb_elts++;

				// Improve the lengths if possible
				int weight = current_edge->weight;
				if (lengths[neighbour] == INF_LENGTH
				||  lengths[neighbour] > lengths[min_elt] + weight)
					lengths[neighbour] = lengths[min_elt] + weight;

				current_edge = current_edge->next;
			}
		}
	}

	return lengths;
}
