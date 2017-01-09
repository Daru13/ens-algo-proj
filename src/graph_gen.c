//------------------------------------------------------------------------------
// RANDOM GRAPH GENERATOR
//------------------------------------------------------------------------------
// Implementation of a simple random undirected, weighted graph generator,
// parameterized by the number of vertices and the density of the graph.
// The graph is printed on the standard output.
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>
#include "graph_gen.h"
#include "toolbox.h"

//------------------------------------------------------------------------------

void swapValues (int* x, int* y)
{
	int temp = *x;
	*x = *y;
	*y = temp;
}

int getRandomBoundedInt (int min, int max)
{
	return rand() % (max - min + 1) + min;
}

int computeNbEdges (int nb_vertices, double density)
{
	return (int) 0.5 + (((nb_vertices * (nb_vertices + 1)) / 2) * density);
}

// If remove_edge is true, the edge is not added, but actually removed
// This is used in the case of the generation of highly dense graphs
void generateNewRandomEdge (int** edges, int nb_vertices, int no_edge_weight,
							int min_weight, int max_weight, bool remove_edge)
{
	// The origin and destination are picked at random, until an unseen
	// combination is found
	int origin, destination, weight;

	do
	{
		origin 		= getRandomBoundedInt(0, nb_vertices - 1);
		destination = getRandomBoundedInt(0, nb_vertices - 1);

		if (destination > origin)
			swapValues(&origin, &destination);
	}
	while ((   remove_edge  && edges[origin][destination] == no_edge_weight)
	   ||  ((! remove_edge) && edges[origin][destination] != no_edge_weight));

	// Add or remove the edge, depending on the flag value
	if (remove_edge)
		edges[origin][destination] = no_edge_weight;
	else
	{
		weight = getRandomBoundedInt(min_weight, max_weight);
		edges[origin][destination] = weight;
	}
}

// This function may consume a lot of memory!
int** generateRandomGraph (int nb_vertices, double density, int no_edge_weight,
						   int min_weight, int max_weight)
{
	// The density gives the number of edges...
	int nb_edges = computeNbEdges(nb_vertices, density);

	// ...and the generation mode: in order to be (way) more performant, if the graph is dense,
	// i.e. density > 0.5, the graph is initialized with all the edges (with random weights),
	// and edges are removed from it until the right number of edges is reached
	bool remove_edges = density > 0.5;

	// Edges matrix allocation and initialization
	// This assumes that the following always holds true: (origin >= destination)
	int** edges = malloc(nb_vertices * sizeof(int*));
	CHECK_MALLOC(edges);

	if (remove_edges)
	{
		for (int i = 0; i < nb_vertices; i++)
		{
			edges[i] = malloc((i + 1) * sizeof(int));
			CHECK_MALLOC(edges[i]);

			for (int j = 0; j < (i + 1); j++)
				edges[i][j] = getRandomBoundedInt(min_weight, max_weight);
		}
	}

	else
	{
		for (int i = 0; i < nb_vertices; i++)
		{
			edges[i] = malloc((i + 1) * sizeof(int));
			CHECK_MALLOC(edges[i]);

			for (int j = 0; j < (i + 1); j++)
				edges[i][j] = no_edge_weight;
		}
	}

	// Random edge addition (or deletion)
	int nb_iterations = remove_edges
					  ? ((nb_vertices * (nb_vertices + 1)) / 2) - nb_edges
					  : nb_edges;

	for (int i = 0; i < nb_iterations; i++)
		generateNewRandomEdge(edges, nb_vertices, no_edge_weight,
							  min_weight, max_weight, remove_edges);

	// Finally return the edges of the newly built random graph
	return edges;
}

void freeGraph (int** edges, int nb_vertices)
{
	for (int i = 0; i < nb_vertices; i++)
		free(edges[i]);

	free(edges);
}

void printGraph (FILE* output, int** edges, int nb_vertices, int nb_edges, int no_edge_weight)
{
	// First, print the number of vertices and the number of edges
	fprintf(output, "%d %d\n", nb_vertices, nb_edges);

	// Then, print all the edges and their weights
	for (int i = 0; i < nb_vertices; i++)
		for (int j = 0; j < (i + 1); j++)
			if (edges[i][j] != no_edge_weight)
				fprintf(output, "%d %d %d\n", i, j, edges[i][j]);
}

void printUsageAndExit (char* argv[])
{
	fprintf(stderr,
			"Usage: %s <nb vertices [1-]> <density (0-1)> <min weight> <max weight>\n",
			argv[0]);
	exit(1);
}

int main (int argc, char* argv[])
{
	srand(time(0) + clock());

	// Try to get the arguments; print usage and exit if it fails
	if (argc != 1 + 4)
		printUsageAndExit(argv);

	int nb_read_values = 0;
	int nb_vertices, min_weight, max_weight;
	double density;

	nb_read_values += sscanf(argv[1], "%d",  &nb_vertices);
	nb_read_values += sscanf(argv[2], "%lf", &density);
	nb_read_values += sscanf(argv[3], "%d",  &min_weight);
	nb_read_values += sscanf(argv[4], "%d",  &max_weight);

	if (nb_read_values != 4)
		printUsageAndExit(argv);
	if (density < 0 || density > 1 || nb_vertices <= 0)
		printUsageAndExit(argv);

	// Generate a random graph according to the given parameters
	// The weight of an absent edge is set to min_weight - 1
	int no_edge_weight = min_weight - 1;

	int** edges = generateRandomGraph(nb_vertices, density, no_edge_weight,
									  min_weight, max_weight);

	// Print the generated graph on the standard output
	int nb_edges = computeNbEdges(nb_vertices, density);
	printGraph(stdout, edges, nb_vertices, nb_edges, no_edge_weight);

	// Free the graph and exit
	freeGraph(edges, nb_vertices);

	return 0;
}
