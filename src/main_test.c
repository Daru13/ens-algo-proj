//------------------------------------------------------------------------------
// FIBONACCI HEAPS
//------------------------------------------------------------------------------
// Implementation of Fibonacci heaps.
// Note: the acronym "CDLL" stands for Circular Double-Linked List
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "toolbox.h"
#include "fibonacci_heap.h"
#include "main_test.h"

//------------------------------------------------------------------------------

void printNodesInfo (Node* const nodes[], const int n)
{
	for (int i = 0; i < n; i++)
		printf("Node %d is in a list of %d node(s) and has %d father.\n",
			i,
			getNbNodesOfList(nodes[i]),
			nodes[i]->father != NULL ? 1 : 0);
}

void printfFiboHeapInfo (FiboHeap* const fibo_heap)
{
	// Number of nodes
	printf("The Fibonacci heap has a total of %d node(s).\n", fibo_heap->nb_nodes);

	// Minimum element
	Node* fibo_heap_min_element = getMinimumElement(fibo_heap);
	printf("Value of its minimum element: %d\n",
			fibo_heap_min_element != NULL ? fibo_heap_min_element->value : -1);
}

void mainTest_1 ()
{
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

	//---------- Cleaning ----------

	printf("\n");

	printProgressMessage("[The Fibonacci heap is deleted]\n");
	freeFiboHeap(fibo_heap);
}

int main ()
{
	mainTest_1();

	return 0;
}
