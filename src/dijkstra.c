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
#include "complexity.h"
#include "graph.h"
#include "dijkstra.h"

//------------------------------------------------------------------------------
// NAIVE DIJKSTRA (WITH AN ARRAY)
//------------------------------------------------------------------------------

int extractMinimumNaive (bool* seen, int* lengths, int nb_vertexes)
{
	int i;
	int min = -1;

	// Find the minimum
	for (i = 0; i < nb_vertexes; i++)
	{	
		if (lengths[i] != INF_LENGTH
		&& !seen[i])
		{
			if (min == -1)
			{
			min = i;
			} 
			else 
			{
				if (lengths[min] > lengths[i])
				{
					min = i;
				}
			}
		}
	}

	return min;
}

int* dijkstraNaive (Graph* g, int s) // O(#A²)
{
	// Initialization
	int* lengths = malloc(g->nb_vertexes * sizeof(int));
	CHECK_MALLOC(lengths);
	bool seen[g->nb_vertexes];

	for (int i = 0 ; i < g->nb_vertexes; i++)
	{
		seen[i]    = false;
		lengths[i] = INF_LENGTH;
	}

	lengths[s] = 0;


	// Main loop (over the priority queue structure)
	for (int i = 0; i < g->nb_vertexes; i++)
	{
		// The element with the smallest distance is extracted
		int min_elt = extractMinimumNaive(seen, lengths, g->nb_vertexes);
		seen[min_elt] = true;

		// Iteration over all min_elt's neighbours
		Edge* current_edge = g->edges[min_elt];
		while (current_edge != NULL)
		{
			int neighbour = current_edge->destination;

			// Improve the lengths if possible
			int weight = current_edge->weight;

			if (lengths[neighbour] == INF_LENGTH
			||  lengths[neighbour] > lengths[min_elt] + weight)
			{
				lengths[neighbour] = lengths[min_elt] + weight;
			}
			current_edge = current_edge->next;
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

/*	
		printFiboHeap(fibo_heap);
		printf("Extracted min number %d:\n", i);
		assert(extracted_min != NULL);
*/
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
