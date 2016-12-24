//------------------------------------------------------------------------------
// FIBONACCI HEAPS
//------------------------------------------------------------------------------
// Implementation of Fibonacci heaps.
// Note: the acronym "CDLL" stands for Circular Double-Linked List
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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
#define MAX_NODE_KEY 5

//------------------------------------------------------------------------------

Node** generateFixedNodesArray (int nb_nodes)
{
	Node** generated_nodes = malloc(nb_nodes * sizeof(Node*));
	CHECK_MALLOC(generated_nodes);

	for (int i = 0; i < nb_nodes; i++)
		generated_nodes[i] = createIsolatedNode(i, i);
	
	return generated_nodes;
}

Node** generateRandomNodesArray (int nb_nodes)
{
	Node** generated_nodes = malloc(nb_nodes * sizeof(Node*));
	CHECK_MALLOC(generated_nodes);

	for (int i = 0; i < nb_nodes; i++)
	{
		int random_value = rand() % (MAX_NODE_VALUE - MIN_NODE_VALUE + 1) + MIN_NODE_VALUE;
		int random_key   = rand() % (MAX_NODE_KEY - MIN_NODE_KEY + 1) + MIN_NODE_KEY;

		generated_nodes[i] = createIsolatedNode(random_value, random_key);
	}

	return generated_nodes;
}

void deleteNodesArray (Node** node_array, int size)
{
	for (int i = 0; i < size; i++)
		freeNode(node_array[i]);

	free(node_array);
}

void testNodes_1 ()
{
	printProgressMessage("\n--------- TEST OF NODES ---------\n");

	//---------- Creation, insertion, extraction, fusion, deletion of node lists ----------

	printProgressMessage("[An array of 10 nodes is created]\n");
	Node** nodes = generateFixedNodesArray(10);

	printProgressMessage("[Two lists of 5 nodes are formed]\n");
	Node* node_list_1 = nodes[0];
	Node* node_list_2 = nodes[5];

	for (int i = 1; i <= 4; i++)
	{
		insertNodeInList(nodes[i], node_list_1);
		insertNodeInList(nodes[i + 5], node_list_2);
	}

	printListOfNodes(node_list_1);
	printListOfNodes(node_list_2);
	printf("\n");

	printProgressMessage("[The last element of each list is extracted]\n");
	extractNodeFromList(nodes[4]);
	Node* extracted_node_from_list_1 = nodes[4];
	extractNodeFromList(nodes[9]);
	Node* extracted_node_from_list_2 = nodes[9];

	printNodeDetails(nodes[0]);
	printListOfNodes(node_list_1);
	printf("\n");
	printNodeDetails(nodes[5]);
	printListOfNodes(node_list_2);
	printf("\n");

	printProgressMessage("[Both nodes become child of the first node of the first list]\n");
	insertNodeAsChild(extracted_node_from_list_1, nodes[0]);
	insertNodeAsChild(extracted_node_from_list_2, nodes[0]);

	printNodeDetails(nodes[0]);
	printListOfNodes(node_list_1);
	printf("\n");
	printNodeDetails(nodes[5]);
	printListOfNodes(node_list_2);
	printf("\n");

	printProgressMessage("[List 1 is merged into list two]\n");
	mergeListsOfNodes(node_list_1, node_list_2);

	printListOfNodes(node_list_2);
	printf("\n");

	printProgressMessage("[The last node is extracted, then inserted as another child]\n");
	extractNodeFromList(nodes[8]);
	Node* extracted_node = nodes[8];

	insertNodeAsChild(extracted_node, nodes[0]);

	printNodeDetails(nodes[0]);
	printListOfNodes(node_list_2);
	printf("\n");

	printProgressMessage("[The first two children of the parent are extracted]\n");
	extractNodeFromList(extracted_node_from_list_1);
	extractNodeFromList(extracted_node_from_list_2);

	printf("Extracted children:\n");
	printNodeDetails(extracted_node_from_list_1);
	printNodeDetails(extracted_node_from_list_2);
	
	printf("\nParent node:\n");
	printNodeDetails(nodes[0]);
	printf("\n");

	//---------- Cleaning ----------

	printProgressMessage("[All nodes are deleted]\n");
	deleteNodesArray(nodes, 10);
}

void testGraph_1 ()
{
	printProgressMessage("[Enter a graph (in the right format)]\n");
	Graph* g = createGraphFromFile(stdin);
	int compt = 0;

	if (graphIsConnected(g,&compt))
	{
		printProgressMessage("[The graph is connected]\n");
		printf("fait en %d étapes \n" , compt);

		compt = 0;
	
		int origin = 0;
		printProgressMessage("[Distances from vertice 0]\n");

		int* res= dijkstraNaive(g, origin, &compt);
		for (int i = 0; i < g->nb_vertexes; i++)
			printf("Distance from %d to %d is: %d\n", origin, i, res[i]);
		printf("fait en %d étapes \n" , compt);
	}
	else
	{
		printProgressMessage("[The graph is *NOT* connected]\n");
		printf("fait en %d étapes \n" , compt);
	}
}

void testFibonacciHeaps_1 ()
{
printProgressMessage("\n--------- TEST OF FIBONACCI HEAPS 1 ---------\n");

	//---------- Creation, insertion, min extraction (+ consolidatation) ----------

	printProgressMessage("[An array of 5 nodes is created]\n");
	Node** nodes = generateFixedNodesArray(5);

	printProgressMessage("[A Fibonacci heap is created]\n");
	FiboHeap* fibo_heap = createFiboHeap();
	printFiboHeap(fibo_heap);

	printProgressMessage("[All the nodes are inserted]\n");
	for (int i = 0; i < 5; i++)
		insertRootInFiboHeap(fibo_heap, nodes[i]);

	printFiboHeap(fibo_heap);
	printf("\n");

	printProgressMessage("[The minimum elements is extracted]\n");

	Node* extracted_nodes[3];
	for (int i = 0; i < 3; i++)
	{
		extracted_nodes[i] = extractMinFromFiboHeap(fibo_heap);
		
		printf("After extraction %d:\n", i + 1);
		printFiboHeap(fibo_heap);
	}

	printProgressMessage("[Extracted minimum elements are the following]\n");
	for (int i = 0; i < 3; i++)
	{
		printf("Extracted min %d:\n", i + 1);
		printNodeDetails(extracted_nodes[i]);
	}

	//---------- Cleaning ----------

	printProgressMessage("[The Fibonacci heap (and the nodes) are deleted]\n");
	freeFiboHeap(fibo_heap);
	free(nodes);
}

void testFibonacciHeaps_2 ()
{
printProgressMessage("\n--------- TEST OF FIBONACCI HEAPS 2 ---------\n");

	//---------- Extraction of several minimums with random nodes ----------

	printProgressMessage("[An array of 50 random nodes is created]\n");
	Node** nodes = generateRandomNodesArray(50);

	printProgressMessage("[A Fibonacci heap is created]\n");
	FiboHeap* fibo_heap = createFiboHeap();

	printProgressMessage("[All the nodes are inserted]\n");
	for (int i = 0; i < 50; i++)
		insertRootInFiboHeap(fibo_heap, nodes[i]);
	printFiboHeap(fibo_heap);

	printProgressMessage("[15 minimum elements are extracted]\n");

	Node* extracted_nodes[15];
	for (int i = 0; i < 15; i++)
	{
		extracted_nodes[i] = extractMinFromFiboHeap(fibo_heap);
		// printFiboHeap(fibo_heap);
	}

	printProgressMessage("[Extracted minimum elements are the following]\n");
	for (int i = 0; i < 15; i++)
		printf("Extracted min %d: (%p : %d)\n",
			i + 1, (void*) extracted_nodes[i], extracted_nodes[i]->key);
	
	//---------- Cleaning ----------

	printProgressMessage("[The Fibonacci heap (and the nodes) are deleted]\n");
	freeFiboHeap(fibo_heap);
	free(nodes);
}

int main ()
{
	srand(time(0));

	// testGraph_1();
	// testNodes_1();
	// testFibonacciHeaps_1();
	testFibonacciHeaps_2();

	return 0;
}
