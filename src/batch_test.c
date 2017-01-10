//---------------------------------------------------------------
// BATCH PERFORMANCES TESTS
//---------------------------------------------------------------

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h> // Assert precise time measurement is available
#include <assert.h>
#include <stdbool.h>
#include "complexity.h"
#include "graph.h"
#include "dijkstra.h"

// About graph paths
#define PATH_LENGTH 256
#define GRAPH_FILE_PREFIX "graph_"

//---------------------------------------------------------------

void printUsageAndExit (char* argv[])
{
	fprintf(stderr,
			"Usage: %s <graph base path> <nb graphs>\n",
			argv[0]);
	exit(1);
}

char* getFullGraphPath (char* basepath, int graph_num)
{
	char* path = malloc(PATH_LENGTH * sizeof(char));
	CHECK_MALLOC(path);

	int nb_written_var = sprintf(path, "%s%s%d", basepath, GRAPH_FILE_PREFIX, graph_num);
	// printf("%s\n", path);
	assert(nb_written_var >= 3);

	return path;
}

Graph* getGraphFromFile (char* basepath, int graph_num)
{
	// Compute the file path, and open it
	char* graph_path = getFullGraphPath(basepath, graph_num);
	
	FILE* graph_file = fopen(graph_path, "r");
	assert(graph_file != NULL);

	// Create a graph from this file, close it, and return the created graph
	Graph* graph = createGraphFromFile(graph_file);
	
	fclose(graph_file);
	free(graph_path);

	return graph;
}

double getTimeDifferenceInMs (struct timespec* start, struct timespec* end)
{
	return (1000 * ((double) end->tv_sec))   + (((double) end->tv_nsec) / 1000000)
		 - (1000 * ((double) start->tv_sec)) + (((double) start->tv_nsec) / 1000000);
}

int main (int argc, char* argv[])
{
	// Try to get the arguments; print usage and exit if it fails
	if (argc != 1 + 2)
		printUsageAndExit(argv);

	int nb_read_values = 0;
	char* basepath = argv[1];
	int nb_graphs;

	nb_read_values += sscanf(argv[2], "%d",  &nb_graphs);

	if (nb_read_values != 1)
		printUsageAndExit(argv);
	if (nb_graphs < 0)
		printUsageAndExit(argv);

	// Each graph is opened, algorithms are applied, and the performances are recorded
	// Computed complexities
	long connectivity_total_complexity 		= 0;
	long naive_dijkstra_total_complexity    = 0;
	long optimized_dijkstra_total_complexity = 0;

	// Required time (in ms)
	long connectivity_total_time 	  = 0;
	long naive_dijkstra_total_time 	  = 0;
	long optimized_dijkstra_total_time = 0;

	// Other interesting values
	int nb_connected_graphs = 0;

	// Precise time measurement structures
	struct timespec start_time, end_time;
	start_time.tv_sec  = 0;
	start_time.tv_nsec = 0;
	end_time.tv_sec    = 0;
	end_time.tv_nsec   = 0;

	// Note: it is important to iterate from zero to n-1, given that graph paths are
	// computed using this loop variable value!
	bool is_connected;
	int origin_vertex = 0;

	for (int i = 0; i < nb_graphs; i++)
	{
		// Get the current graph
		Graph* graph = getGraphFromFile(basepath, i);

		// Connectivity test
		clock_gettime(CLOCK_REALTIME, &start_time);
		is_connected = graphIsConnected(graph);
		clock_gettime(CLOCK_REALTIME, &end_time);

		connectivity_total_time 	  += getTimeDifferenceInMs(&start_time, &end_time);
		connectivity_total_complexity += COMPLEXITY;

		if (is_connected)
		{
			nb_connected_graphs++;

			// Naive Dijkstra's algorithm
			clock_gettime(CLOCK_REALTIME, &start_time);
			dijkstraNaive(graph, origin_vertex);
			clock_gettime(CLOCK_REALTIME, &start_time);

			naive_dijkstra_total_time 		+= getTimeDifferenceInMs(&start_time, &end_time);
			naive_dijkstra_total_complexity += COMPLEXITY;


			// Optimized Dijkstra's algorithm
			// clock_gettime(CLOCK_REALTIME, &start_time);
			// dijkstra(graph, origin_vertex);
			// clock_gettime(CLOCK_REALTIME, &start_time);

			// optimized_dijkstra_total_time 	+= getTimeDifferenceInMs(&start_time, &end_time);
			optimized_dijkstra_total_complexity += ComplexityOf_dijkstra(graph, origin_vertex);
		}
	}

	// Average values to print
	double average_connectivity_complexity =
		(double) connectivity_total_complexity / (double) nb_graphs;
	double average_naive_dijkstra_complexity =
		(double) naive_dijkstra_total_complexity / (double) nb_connected_graphs;
	double average_optimized_dijkstra_complexity =
		(double) optimized_dijkstra_total_complexity / (double) nb_connected_graphs;

	double average_connectivity_time =
		(double) connectivity_total_time / (double) nb_graphs;
	double average_naive_dijkstra_time =
		(double) naive_dijkstra_total_time / (double) nb_connected_graphs;
	double average_optimized_dijkstra_time =
		(double) optimized_dijkstra_total_time / (double) nb_connected_graphs;

	double connected_graphs_ratio = (double) nb_connected_graphs / (double) nb_graphs;

	// Final printing
	fprintf(stderr, "----- Results of batch testing on %d graphs -----\n", nb_graphs);
	fprintf(stderr, "(Source path: %s)\n\n", basepath);

	fprintf(stderr, "-- VARIOUS INFORMATION --\n");
	fprintf(stderr, "Ratio of connected graphs: %lf\n\n", connected_graphs_ratio);

	fprintf(stderr, "-- AVERAGE COMPLEXITIES --\n");
	fprintf(stderr, "Average connectivity test complexity: %lf\n",
		average_connectivity_complexity);
	fprintf(stderr, "Average naive Dijkstra complexity   : %lf\n",
		average_naive_dijkstra_complexity);
	fprintf(stderr, "Average optimized test complexity   : %lf\n\n",
		average_optimized_dijkstra_complexity);

	fprintf(stderr, "-- AVERAGE EXEC TIMES (milliseconds) --\n");
	fprintf(stderr, "Average connectivity test time: %lf\n",
		average_connectivity_time);
	fprintf(stderr, "Average naive Dijkstra time   : %lf\n",
		average_naive_dijkstra_time);
	fprintf(stderr, "Average optimized test time   : %lf\n",
		average_optimized_dijkstra_time);

	fprintf(stderr, "\n(Test) Value of COMPLEXITY variable: %ld\n", COMPLEXITY);

	return 0; 
}
