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
#include <limits.h>
#include "list.h"
#include "toolbox.h"
#include "complexity.h"
#include "graph.h"
#include "dijkstra.h"

// Unreachable length (all lengths are positive or null)
#define INF_LENGTH -1
#define MAX_LENGTH INT_MAX // the larger value a 32 bits signed integer can hold

//------------------------------------------------------------------------------
// NAIVE DIJKSTRA (WITH AN ARRAY)
//------------------------------------------------------------------------------

int extractMinimumNaive (bool* seen, int* lengths, int nb_vertexes, int* compt)
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

	for (int i = 0 ; i < g->nb_vertexes; i++)
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

//------------------------------------------------------------------------------
// OPTIMIZED DIJKSTRA (WITH A FIBONACCI HEAP)
//------------------------------------------------------------------------------

int* dijkstra (Graph* g, int s)
{
	// Initialization
	int* lengths = malloc(g->nb_vertexes * sizeof(int));
	CHECK_MALLOC(lengths);

	FiboHeap* fibo_heap = createFiboHeap();

	Node* vertexes[g->nb_vertexes];
	int key;
	for (int i = 0 ; i < g->nb_vertexes; i++)
	{
		// Every vertex but the origin s is assigned a very big key
		key = i == s ? 0 : MAX_LENGTH;

		vertexes[i] = createIsolatedNode(i, key);
		insertRootInFiboHeap(fibo_heap, vertexes[i]);
	}

	// Main loop (over the Fibonacci heap's content, i.e. unseen vertexes)
	for (int i = 0; i < g->nb_vertexes; i++)
	{
		// The element with the smallest distance is extracted
		Node* extracted_min = extractMinFromFiboHeap(fibo_heap);
		printFiboHeap(fibo_heap);
		printf("Extracted min number %d:\n", i);
		assert(extracted_min != NULL);
		int min_vertex = extracted_min->value;

		// Iteration over all min_vertex's neighbours
		Edge* current_edge = g->edges[min_vertex];
		while (current_edge != NULL)
		{
			int neighbour = current_edge->destination;

			// Improve the lengths if possible (and update the heap if required)
			int weight = current_edge->weight;
			int new_length = lengths[min_vertex] + weight;

			if (lengths[neighbour] == MAX_LENGTH
			||  lengths[neighbour] > new_length)
			{
				lengths[neighbour] = new_length;
				decreaseKeyInFiboHeap(fibo_heap, vertexes[neighbour], new_length);
			}

			current_edge = current_edge->next;
		}
	}

	freeFiboHeap(fibo_heap);
	return lengths;
}
