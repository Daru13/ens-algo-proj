//------------------------------------------------------------------------------
// COMPLEXITY
//------------------------------------------------------------------------------
// Computation of the complexities of almost all "technical" functions.
// A concerned function "f" has an associated function "ComplexityOf_f".
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "list.h"
#include "graph.h"
#include "fibonacci_heap.h"
#include "dijkstra.h"
#include "complexity.h"

//------------------------------------------------------------------------------

// Declaration and initialization of the global complexity variable
unsigned int COMPLEXITY = 0;

//------------------------------------------------------------------------------
// FUNCTIONS FROM FILE "list.c"
//------------------------------------------------------------------------------

int ComplexityOf_createList () { return 4; }
int ComplexityOf_listIsEmpty () { return 2; }
int ComplexityOf_addElementToList () { return 5; }
int ComplexityOf_popFromList () { return 3; }

int ComplexityOf_concatTwoLists (List* L1 ,List* L2)
{
	int complexity 		  = 0;
	ListElt* list_element = L2->first;

	while (list_element != NULL)
	{
		complexity += ComplexityOf_listIsEmpty();
		complexity += ComplexityOf_popFromList();
		complexity += 1;
		complexity += ComplexityOf_addElementToList();
		list_element = list_element->next;
	}	

	return complexity;
}


//------------------------------------------------------------------------------
// FUNCTIONS FROM FILE "graph.c"
//------------------------------------------------------------------------------

int ComplexityOf_createEdge () {return 0; }
int ComplexityOf_createEmptyGraph () {return 0; }
int ComplexityOf_addUndirectedEdgeToGraph () {return 0; }
int ComplexityOf_createGraphFromFile () {return 0; }

int ComplexityOf_graphIsConnected (Graph* g) 
{
	int compt = 0; 
    List* waiting = createList();
    compt = compt +  ComplexityOf_createList ();
	
	addElementToList(waiting, 0);
	compt +=  ComplexityOf_addElementToList ();
 
    bool seen[g->nb_vertexes];
	compt += g->nb_vertexes;

    for (int i = 0 ; i < g->nb_vertexes; i++)
     {
         seen[i] = false;
         (compt)++;
     }
     while (! listIsEmpty(waiting))
     {
         int nd = popFromList(waiting) ;
         compt = compt +2;
         if ( !seen[nd])
         {
            seen[nd] = true;
            Edge* EL = g->edges[nd];
            compt = compt +2;
            while ( EL != NULL)
            {
                addElementToList (waiting, EL->destination);
				compt += ComplexityOf_addElementToList();
                EL = EL->next;
                (compt)++;
            }
 
        }
    }

    bool res = true;
    (compt)++;
    for (int i=0; i<g->nb_vertexes ; i++)
    {
         res = res && (seen[i]);
         compt++;
    }
    return compt;
}                                                                                            


//------------------------------------------------------------------------------
// FUNCTIONS FROM FILE "fibonacci_heap.c"
//------------------------------------------------------------------------------

// TODO :)

//------------------------------------------------------------------------------
// FUNCTIONS FROM FILE "dijkstra.c"
//------------------------------------------------------------------------------

#define INF_LENGTH -1

int ComplexityOf_extractMinimumNaive (bool* seen, int* lengths, int nb_vertexes)
{
	int compt = 0;
	int i;
	int min = -1;
	compt += 2;

	for (i = 0; i < nb_vertexes; i++)
	{
		compt += 2;
		if (lengths[i] != INF_LENGTH
		&& !seen[i])
		{
			compt++;
			if (min == -1)
			{
				min = i;
				compt++;
			} 
			else 
			{
				compt++;
				if (lengths[min] > lengths[i])
				{
					min = i;
					compt++;
				}
			}
		}
	}


	return compt;
}



int ComplexityOf_dijkstraNaive (Graph* g, int s) // O(#A²)
{
	// Initialization
	int compt = 0;
	int* lengths = malloc(g->nb_vertexes * sizeof(int));
	CHECK_MALLOC(lengths);
	bool seen[g->nb_vertexes];
	compt += g->nb_vertexes;
	
	for (int i = 0 ; i < g->nb_vertexes ; i++)
	{
		seen[i]    = false;
		lengths[i] = INF_LENGTH;
		compt += 2;
	}
	lengths[s]  = 0;
	compt++;
	
	// Main loop (over the priority queue structure)
	for (int i = 0; i<g->nb_vertexes;i++)
	{
		// The element with the smallest distance is extracted
		int* truc;
		int min_elt = extractMinimumNaive(seen,
			lengths, g->nb_vertexes, truc);
		compt += ComplexityOf_extractMinimumNaive(
			seen,lengths, g->nb_vertexes);
		seen[min_elt] = true;
		compt++;
		// Iteration over all min_elt's neighbours
		Edge* current_edge = g->edges[min_elt];
		compt++;
		while (current_edge != NULL)
		{
			// Each neighbour is added to the priority structure
			int neighbour = current_edge->destination;
			compt++;
			// Improve the lengths if possible
			int weight = current_edge->weight;
			compt++;
					
			compt += 2;
			if (lengths[neighbour] == INF_LENGTH
			||  lengths[neighbour] > lengths[min_elt] + weight)
			{
				lengths[neighbour] = lengths[min_elt] + weight;
				compt++;
			}
			current_edge = current_edge->next;
			compt++;
		}
	}
	return compt;
}
