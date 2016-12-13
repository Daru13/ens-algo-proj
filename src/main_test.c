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

//------------------------------------------------------------------------------

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

void mainTest_1 ()
{
	printProgressMessage("\n--------- MAIN TEST 1 ---------\n");

	//---------- Nodes of CDLLs ----------
	
	NodeValue node_val_0 = 0;
	NodeValue node_val_1 = 42;
	NodeValue node_val_2 = -128;

	printProgressMessage("[3 nodes are created]\n");
	Node* nodes[3];
	nodes[0] = createIsolatedNode(node_val_0);
	nodes[1] = createIsolatedNode(node_val_1);
	nodes[2] = createIsolatedNode(node_val_2);

	printNodesInfo(nodes, 3);

	printProgressMessage("[Nodes 0, 1 and 2 are merged]\n");
	mergeNodeLists(nodes[0], nodes[1]);
	mergeNodeLists(nodes[0], nodes[2]);

	printNodesInfo(nodes, 3);

	printProgressMessage("[Node 1 is extracted]\n");
	extractNodeFromList(nodes[1]);

	printNodesInfo(nodes, 3);

	//---------- Fibonacci heap ----------

	printf("\n");

	printProgressMessage("[A Fibonacci heap is created]\n");
	FiboHeap* fibo_heap = createFiboHeap();

	printProgressMessage("[Node 1 is inserted in the Fibonacci heap]\n");
	insertSingleRootInFiboHeap(fibo_heap, nodes[1]);

	printfFiboHeapInfo(fibo_heap);

	printProgressMessage("[Node 2 is extracted, then inserted in the Fibonacci heap]\n");
	extractNodeFromList(nodes[2]);
	insertSingleRootInFiboHeap(fibo_heap, nodes[2]);

	printNodesInfo(nodes, 3);
	printfFiboHeapInfo(fibo_heap);

	printProgressMessage("[Extracting minimum value, two times]\n");
	Node* min_node_1 =  extractMinFromFiboHeap(fibo_heap);
	Node* min_node_2 =  extractMinFromFiboHeap(fibo_heap);
	printf("Extracted values are %d and %d\n", min_node_1->value, min_node_2->value);
	printfFiboHeapInfo(fibo_heap);

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

	#define NB_GEN_NODES 10
	#define MIN_VALUE 0
	#define MAX_VALUE 20

	printProgressMessage("[Nodes with random values are created]\n");
	printf("%d nodes created, values ranging from %d to %d\n",
			NB_GEN_NODES, MIN_VALUE, MAX_VALUE);

	Node* generated_nodes[NB_GEN_NODES];
	for (int i = 0; i < NB_GEN_NODES; i++)
	{
		int random_value = rand() % (MAX_VALUE - MIN_VALUE + 1) + MIN_VALUE;
		// Debug :
		random_value = i;
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

int main ()
{
	srand(time(0));

	// mainTest_1();
	mainTest_2();

	return 0;
}
