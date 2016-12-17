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
	printProgressMessage("\n--------- TEST OF NODES  ---------\n");

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

	testNodes_1();

	return 0;
}
