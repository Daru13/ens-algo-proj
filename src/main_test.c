//------------------------------------------------------------------------------
// FIBONACCI HEAPS
//------------------------------------------------------------------------------
// Implementation of Fibonacci heaps.
// Note: the acronym "CDLL" stands for Circular Double-Linked List
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "toolbox.h"
#include "fibonacci_heap.h"
#include "main_test.h"
#include "graph.h"
#include "dijkstra.h"

// Random nodes generation parameters
#define NB_GEN_NODES 8

#define MIN_NODE_VALUE 0
#define MAX_NODE_VALUE 10

#define MIN_NODE_KEY 0
#define MAX_NODE_KEY 20

//------------------------------------------------------------------------------

/*
void printNodesInfo (Node* const nodes[], const int n)
{
	for (int i = 0; i < n; i++)
		if (nodes[i] == NULL)
			printf("Node %d is NULL\n", i);
		else
			printf("Node %d has value %d, is in a list of %d node(s) and has %d father.\n",
				i,
				nodes[i]->value,
				getNbNodesOfList(nodes[i]),
				nodes[i]->father != NULL ? 1 : 0);
}

void printfFiboHeapInfo (FiboHeap* const fibo_heap)
{
	// Number of nodes
	printf("The Fibonacci heap has a total of %d node(s), and a degree of %d.\n",
			fibo_heap->nb_nodes, fibo_heap->degree);

	// Minimum element
	Node* fibo_heap_min_element = fibo_heap->min_element;
	printf("Value of its minimum element: %d\n",
			fibo_heap_min_element != NULL ? fibo_heap_min_element->value : -1);
}
*/

Node** generateFixedNodesArray (int nb_nodes)
{
	Node* generated_nodes = malloc(nb_nodes * sizeof(Node*));
	CHECK_MALLOC(generated_nodes);

	for (int i = 0; i < nb_nodes; i++)
		generated_nodes[i] = createIsolatedNode(i, i);
	
	return generated_nodes;
}

Node** generateRandomNodesArray (int nb_nodes)
{
	Node* generated_nodes = malloc(nb_nodes * sizeof(Node*));
	CHECK_MALLOC(generated_nodes);

	for (int i = 0; i < nb_nodes; i++)
	{
		int random_value = rand() % (MAX_NODE_VALUE - MIN_NODE_VALUE + 1) + MIN_NODE_VALUE;
		int random_key   = rand() % (MAX_NODE_KEY - MIN_NODE_KEY + 1) + MIN_NODE_KEY;

		generated_nodes[i] = createIsolatedNode(random_value, random_key);
	}

	return generated_nodes;
}

void testNodes_1 ()
{
	printProgressMessage("\n--------- MAIN TEST 1 ---------\n");

	//---------- Creation, insertion, extraction, deletion of nodes ----------

	

	//---------- Cleaning ----------

	printf("\n");

	printProgressMessage("[The Fibonacci heap is deleted]\n");
	freeFiboHeap(fibo_heap);
}

void mainTest_2 ()
{
	printProgressMessage("\n--------- MAIN TEST 2 ---------\n");

	// Many nodes are created, the inserted in an empty Fibonacci heap
	// Their values are randomly selected in a small range (to force collisions)

	printProgressMessage("[Nodes with random values are created]\n");
	printf("%d nodes created, values ranging from %d to %d\n",
			NB_GEN_NODES, MIN_VALUE, MAX_VALUE);

	Node* generated_nodes[NB_GEN_NODES];
	for (int i = 0; i < NB_GEN_NODES; i++)
	{
		int random_value = rand() % (MAX_VALUE - MIN_VALUE + 1) + MIN_VALUE;
		random_value = i; // DEBUG
		generated_nodes[i] = createIsolatedNode(random_value);
	}

	printNodesInfo(generated_nodes, NB_GEN_NODES);

	//---------- Fibonacci heap ----------

	printf("\n");

	printProgressMessage("[A Fibonacci heap is created]\n");
	FiboHeap* fibo_heap = createFiboHeap();
	printFiboHeap(fibo_heap);

	printProgressMessage("[All nodes are inserted in the Fibonacci heap]\n");
	for (int i = 0; i < NB_GEN_NODES; i++)
		insertSingleRootInFiboHeap(fibo_heap, generated_nodes[i]);

	printfFiboHeapInfo(fibo_heap);
	printFiboHeap(fibo_heap);

	printProgressMessage("[Half of the minimum values are extracted]\n");
	Node* extracted_min_nodes[NB_GEN_NODES / 2];
	for (int i = 0; i < NB_GEN_NODES / 2; i++)
		extracted_min_nodes[i] = extractMinFromFiboHeap(fibo_heap);

	printProgressMessage("[The extracted nodes are the following]\n");

	printNodesInfo(extracted_min_nodes, NB_GEN_NODES / 2);

	printfFiboHeapInfo(fibo_heap);
	printFiboHeap(fibo_heap);

	//---------- Cleaning ----------

	printf("\n");

	printProgressMessage("[The Fibonacci heap is deleted]\n");
	freeFiboHeap(fibo_heap);
}

void mainTest_3 ()
{
	printProgressMessage("[Enter a graph (in the right format)]\n");
	Graph* g = createGraphFromFile(stdin);

	if (graphIsConnected(g))
	{
		printProgressMessage("[The graph is connected]\n");

		int origin = 0;
		printProgressMessage("[Distances from vertice 0]\n");

		int* res= dijkstraNaive(g, origin);
		for (int i = 0; i < g->nb_vertexes; i++)
			printf("Distance from %d to %d is: %d ", origin, i, res[i]);
	}
	else
		printProgressMessage("[The graph is *NOT* connected]\n");
}

int main ()
{
	srand(time(0));

	// mainTest_1();
	// mainTest_2();
	mainTest_3();

	return 0;
}
