//---------------------------------------------------------------
//     Test Automatized file
//---------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>
#include "graph_gen.h"
#include "complexity.h"
#include "fibonacci_heap.h"
#include "graph.h"
#include "dijkstra.h"


void printUsageAndExit (char* argv[])
{
	fprintf(stderr,
			"Usage: %s <nb vertices [1-]> <density (0-1)> <min weight> <max weight> <nb_try>\n",
			argv[0]);
	exit(1);
}

Graph* createRandomGraph(int nb_vertices,double density,int min_weight,int max_weight)
{
	
	FILE* fp = fopen("file_a_la_con.txt","w+");
	int no_edge_weight = min_weight - 1;

	int** edges = generateRandomGraph(nb_vertices, density, no_edge_weight,
									  min_weight, max_weight);

	// Print the generated graph on the standard output
	int nb_edges = computeNbEdges(nb_vertices, density);
	printGraph(fp, edges, nb_vertices, nb_edges, no_edge_weight);
	return createGraphFromFile(fp);
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
	int nb_try;

	nb_read_values += sscanf(argv[1], "%d",  &nb_vertices);
	nb_read_values += sscanf(argv[2], "%lf", &density);
	nb_read_values += sscanf(argv[3], "%d",  &min_weight);
	nb_read_values += sscanf(argv[4], "%d",  &max_weight);
	nb_read_values += sscanf(argv[5], "%d",  &nb_try);

	if (nb_read_values != 4)
		printUsageAndExit(argv);
	if (density < 0 || density > 1 || nb_vertices <= 0)
		printUsageAndExit(argv);

	int res_con = 0;
	int res_dij_f = 0;
	int res_dij_n = 0;
	int nb_con = 0;


	for(int i=0;i<nb_try;i++)
	{
		printf("Graphe en cours num %d",i);		
		Graph* g = createRandomGraph(nb_vertices,density,min_weight,max_weight);
			
	
		res_con += ComplexityOf_graphIsConnected(g);
		if (graphIsConnected(g))
		{
			
			nb_con ++;
			
			//int* distances = dijkstra(graph, origin);
			res_dij_f += ComplexityOf_dijkstra(g, 0);
			
			//int* distances = dijkstraNaive(graph, origin);
			nb_con += ComplexityOf_dijkstraNaive(g, 0);

		
		}
		
	}

	double ratio_con = nb_con/nb_try;
	double com_moy_con = res_con/nb_try;
	double com_moy_dij_n = res_dij_n/nb_try;	
	double com_moy_dij_f = res_dij_f/nb_try;

	fprintf(stderr,"nb de tests   : %d\n",nb_try);
	fprintf(stderr,"nb de sommets : %d\n",nb_vertices);
	fprintf(stderr,"densité       : %lf\n",density);
	fprintf(stderr,"taux de graphes connexes      : %lf\n",ratio_con);
	fprintf(stderr,"compléxité moyenne pour le test de connexité : %lf\n",com_moy_con);
	fprintf(stderr,"compléxité moyenne pour le l'algo naîf       : %lf\n",com_moy_dij_n);
	fprintf(stderr,"compléxité moyenne pour le les tas de fibo   : %lf\n",com_moy_dij_f);
	return 0;
}












































