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
		printf("Node %d has %d sibling(s) and %d father.\n",
			i,
			getNbNodesOfList(nodes[i]) - 1,
			nodes[i]->father != NULL ? 1 : 0);
}

void mainTest_1 ()
{
	//---------- Nodes of CDLLs ----------
	
	NodeValue node_val_0 = 0;
	NodeValue node_val_1 = 42;
	NodeValue node_val_2 = -128;

	Node* nodes[3];
	nodes[0] = createIsolatedNode(node_val_0);
	nodes[1] = createIsolatedNode(node_val_1);
	nodes[2] = createIsolatedNode(node_val_2);

	printNodesInfo(nodes, 3);

	printf("[Nodes 0, 1 and 2 are merged]\n");
	mergeNodeLists(nodes[0], nodes[1]);
	mergeNodeLists(nodes[0], nodes[2]);

	printNodesInfo(nodes, 3);

	printf("[Node 1 is extracted]\n");
	extractNodeFromList(nodes[1]);

	printNodesInfo(nodes, 3);

	//---------- Fibonacci heap ----------

	FiboHeap* fibo_heap = createFiboHeap();
	insertNodeInFiboHeap(fibo_heap, nodes[1]);

	printf("Fibonacci heap has a total of %d node(s).\n", fibo_heap->nb_nodes);

	Node* fibo_heap_min_element = getMinimumElement(fibo_heap);
	printf("Value of its minimum element: %d\n",
			fibo_heap_min_element != NULL ? fibo_heap_min_element->value : -1);

	// Cleaning
	for(int i = 0; i < 3; i++)
		freeNodeTree(nodes[i]);

	freeFiboHeap(fibo_heap);
}

int main ()
{
	mainTest_1();

	return 0;
}
