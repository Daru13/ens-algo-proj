//------------------------------------------------------------------------------
// RANDOM GRAPH GENERATOR
//------------------------------------------------------------------------------

#ifndef _GRAPH_GEN_H
#define _GRAPH_GEN_H

//------------------------------------------------------------------------------

void swapValues (int* x, int* y);
int getRandomBoundedInt (int min, int max);
int computeNbEdges (int nb_vertices, double density);
void generateNewRandomEdge (int** edges, int nb_vertices, int no_edge_weight,
							int min_weight, int max_weight, bool remove_edge);
int** generateRandomGraph (int nb_vertices, double density, int no_edge_weight,
						   int min_weight, int max_weight);
void freeGraph (int** edges, int nb_vertices);
void printGraph (FILE* output, int** edges, int nb_vertices, int nb_edges, int no_edge_weight);
void printUsageAndExit (char* argv[]);

#endif
