//------------------------------------------------------------------------------
// GRAPH
//------------------------------------------------------------------------------
// Implementation of a graph structure
// Includes reading from file + checking connectivity.
//------------------------------------------------------------------------------

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"
#include "toolbox.h"
#include "complexity.h"
#include "graph.h"

//------------------------------------------------------------------------------
// BASIC OPERATIONS
//------------------------------------------------------------------------------

Edge* createEdge (int origin, int destination, int weight, Edge* next)
{
	Edge* new_edge = malloc(sizeof(Edge));
	CHECK_MALLOC(new_edge);

	new_edge->origin 	  = origin;
	new_edge->destination = destination;
	new_edge->weight 	  = weight;
	new_edge->next 		  = next;

	return new_edge;
}

Graph* createEmptyGraph (int nb_vertexes)
{
	Graph* new_graph = malloc(sizeof(Graph));
	CHECK_MALLOC(new_graph);

	new_graph->nb_vertexes = nb_vertexes;
	
	new_graph->edges = malloc(nb_vertexes * sizeof(Edge*));
	for (int i = 0; i < nb_vertexes; i++)
		new_graph->edges[i] = NULL;

	return new_graph;
}

void addUndirectedEdgeToGraph (Graph* graph, int origin, int destination, int weight)
{
	// Add an edge from the origin to the destination
	Edge* current_source_edge = graph->edges[origin];

	Edge* new_source_edge = createEdge(origin, destination, weight, current_source_edge);
	graph->edges[origin] = new_source_edge;

	// Add an edge from the destination to the origin
	Edge* current_destination_edge 	= graph->edges[destination];
	
	Edge* new_destination_edge = 
			createEdge(destination, origin, weight, current_destination_edge);
	graph->edges[destination] = new_destination_edge;
}

Graph* createGraphFromFile (FILE* file)
{
	// The first line contains:
	// - the number of vertexes
	// - the number of edges
	int nb_vertexes, nb_edges;

	int nb_var_read = fscanf(file, "%d %d", &nb_vertexes, &nb_edges);
	if (nb_var_read != 2)
	{
		fprintf(stderr, "Error: graph file has a bad syntax.\n");
		exit(1);
	}

	// Create an empty graph of nb_vertexes vertexes
	Graph* new_graph = createEmptyGraph(nb_vertexes);

	// Then, there are nb_edges lines. Each line contains :
	// - an origin
	// - a destination
	// - the weight of the edge
	int origin, destination, weight;

	for (int line = 0; line < nb_edges; line++)
	{
		nb_var_read = fscanf(file, "%d %d %d", &origin, &destination, &weight);
		if (nb_var_read != 3)
		{
			fprintf(stderr, "Error: graph file has a bad syntax.\n");
			exit(1);
		}

		// Add the edges (both ways) to the graph
		addUndirectedEdgeToGraph(new_graph, origin, destination, weight);
	}

	return new_graph;
}

//------------------------------------------------------------------------------
// GRAPH CONNECTIVITY
//------------------------------------------------------------------------------

bool graphIsConnected (Graph* g, int* compt)
{
	List* waiting = createList();
	addElementToList(waiting, 0);
	*compt = (*compt)+2;

	bool seen[g->nb_vertexes];
	for (int i = 0 ; i < g->nb_vertexes; i++)
	{
		seen[i] = false;
		(*compt)++;
	}
	while (! listIsEmpty(waiting))
	{
		int nd = popFromList(waiting) ;
		*compt = *compt +2;
		if ( !seen[nd])
		{
			seen[nd] = true;
			Edge* EL = g->edges[nd];
			*compt = *compt +2;
			while ( EL != NULL)
			{
				addElementToList (waiting, EL->destination);
				EL = EL->next;
				(*compt)++;
			}

		}
	}

	bool res = true;
	(*compt)++;
	for (int i=0; i<g->nb_vertexes ; i++)
	{
		res = res && (seen[i]);
		(*compt)++;
	}
	return res;
}
