//------------------------------------------------------------------------------
// MAIN PROGRAM : 2 IMPLEMENTATIONS OF DIJKSTRA's ALGORITHM 
//------------------------------------------------------------------------------
// Interface to use the two Dijkstra's algorithms, by inputing a graph.
// The complexity computation functions are also called, and the results are
// finally displayed in the terminal.
//
// The version using Fibonacci heaps is very likely to fail!
// Besides, the computed complexity are not realistic (you can read more about
// this in the related module/the readme file).
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "toolbox.h"
#include "complexity.h"
#include "fibonacci_heap.h"
#include "graph.h"
#include "dijkstra.h"

//------------------------------------------------------------------------------

// The version of the algorithm to use has to be specified at compile-time
// This allows a more realistic analysis of the complexity/time of the program,
// so that two exec files can be compiled...

// The USE_OPTIMIZED_VERSION preprocessor constant must be defined at compile-time
// to use the version using Fibonacci heaps (even though it doesn't work well/at all?)
int main ()
{
	int connectivity_complexity = 0;
	int dijkstra_complexity 	= 0;

	printProgressMessage("\n[Please enter a graph in the right format]\n");
	Graph* graph = createGraphFromFile(stdin);

	printProgressMessage("\n[Please enter an origin for Dijkstra's algorithm]\n");
	int origin;
	int nb_var_read;
	do
		nb_var_read = scanf("%d", &origin);
	while (nb_var_read != 1);

	connectivity_complexity += ComplexityOf_graphIsConnected(graph);
	if (graphIsConnected(graph))
	{
		printProgressMessage("[The graph is connected]\n");

		#ifdef USE_OPTIMIZED_VERSION
			printProgressMessage("[OPTIMIZED ALGORITHM - Distances from the given vertice]\n");
			int* distances = dijkstra(graph, origin);
			dijkstra_complexity += ComplexityOf_dijkstra(graph, origin);
		#else
			printProgressMessage("[NAIVE ALGORITHM - Distances from the given vertice]\n");
			int* distances = dijkstraNaive(graph, origin);
			dijkstra_complexity += ComplexityOf_dijkstraNaive(graph, origin);
		#endif

		for (int i = 0; i < graph->nb_vertexes; i++)
			printf("Distance from %d to %d is: %d\n", origin, i, distances[i]);
	}
	else
	{
		printProgressMessage("[The graph is NOT connected - stopping here]\n");
	}

	printProgressMessage("\n[Computed complexity for the given input]\n");
	printf("Connectivity test   : %d\n", connectivity_complexity);
	printf("Dijkstra's algorithm: %d\n", dijkstra_complexity);
	printf("Total               : %d\n", connectivity_complexity + dijkstra_complexity);

	return 0;
}
