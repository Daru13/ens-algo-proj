//------------------------------------------------------------------------------
// COMPLEXITY
//------------------------------------------------------------------------------
// Computation of the complexities of almost all "technical" functions.
// A concerned function "f" has an associated function "ComplexityOf_f".
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "list.h"
#include "graph.h"
#include "fibonacci_heap.h"
#include "dijkstra.h"
#include "complexity.h"

//------------------------------------------------------------------------------

// Declaration and initialization of the global complexity variable
unsigned long COMPLEXITY = 0;

//------------------------------------------------------------------------------
// FUNCTIONS FROM FILE "list.c"
//------------------------------------------------------------------------------

int ComplexityOf_createList () { return 4; }
int ComplexityOf_listIsEmpty () { return 2; }
int ComplexityOf_addElementToList () { return 5; }
int ComplexityOf_popFromList () { return 3; }

int ComplexityOf_concatTwoLists (List* L1 ,List* L2)
{
	int complexity 		  = 0;
	ListElt* list_element = L2->first;

	while (list_element != NULL)
	{
		complexity += ComplexityOf_listIsEmpty();
		complexity += ComplexityOf_popFromList();
		complexity += 1;
		complexity += ComplexityOf_addElementToList();
		list_element = list_element->next;
	}	

	return complexity;
}

//------------------------------------------------------------------------------
// FUNCTIONS FROM FILE "graph.c"
//------------------------------------------------------------------------------


int ComplexityOf_createEdge () { return 0; }
int ComplexityOf_createEmptyGraph () { return 0; }
int ComplexityOf_addUndirectedEdgeToGraph () { return 0; }
int ComplexityOf_createGraphFromFile () { return 0; }

int numberOfEdges (Graph* g)
{
	int res = 0;
	for (int i = 0; i< g->nb_vertexes; i++)
	{
		Edge* courant = g->edges[i];
		while ( courant != NULL)
		{
			res++;
			courant = courant-> next;
		}
	}
	return res/2;
}

int ComplexityOf_graphIsConnected (Graph* g) 
{
	int res = 0;
	int S = g->nb_vertexes;
	int A = numberOfEdges(g);
	res = 2+3*S
		+2*A*(3+ComplexityOf_popFromList()+
			  ComplexityOf_addElementToList())
		+S*2
		+1+S;
	return res;
}                                                                                            

//------------------------------------------------------------------------------
// FUNCTIONS FROM FILE "fibonacci_heap.c"
//------------------------------------------------------------------------------

int ComplexityOf_createNode (NodeValue value, int key) { return 10; }

int ComplexityOf_createIsolatedNode (NodeValue value, int key)
{
	return ComplexityOf_createNode(value, key)
		 + 3;
}
int ComplexityOf_freeNode (Node* node) { return 1; }

int ComplexityOf_freeNodeTree (Node* root)
{
	unsigned int complexity = 2;
	Node* current_child = root->child;

	if (current_child != NULL)
	{
		complexity++;
		do
		{
			complexity += ComplexityOf_freeNodeTree(current_child) + 2;
			current_child = current_child->next;
		}
		while (current_child != root->child);
	}

	return complexity;
}

int ComplexityOf_printNodeDetails (Node* node) { return 0; }

int ComplexityOf_printListOfNodes (Node* node) { return 0; }

int ComplexityOf_createFiboHeap () { return 6; }

int ComplexityOf_freeFiboHeap (FiboHeap* fibo_heap)
{
	int complexity = 2;
	Node* current_root = fibo_heap->min_element;

	if (current_root != NULL)
	{
		complexity++;
		do
		{
			complexity += ComplexityOf_freeNodeTree(current_root) + 2;
			current_root = current_root->next;
		}
		while (current_root != fibo_heap->min_element);
	}

	return complexity;
}

int ComplexityOf_printFiboHeap (FiboHeap* fibo_heap) { return 0; }

// Upper-bound: 11
int ComplexityOf_extractNodeFromList (Node* node)
{	
	int complexity = 2;

	if (node->father != NULL)
	{
		if (node->next != node)
			complexity += 5;
		else
			complexity += 4;
	}
	
	if (node->next != node)
		complexity += 4;

	return complexity;
}

int ComplexityOf_insertNodeInList (Node* node_to_insert, Node* destination) { return 5; }

int ComplexityOf_mergeListsOfNodes (Node* to_merge, Node* destination)
{
	int complexity = 8;

	Node* to_merge_last_node = to_merge->previous;
	Node* current_node 		 = to_merge_last_node;

	do
	{
		complexity += 3;
		current_node = current_node->next;
	}
	while (current_node != to_merge_last_node);

	return complexity;
}

// Upper-bound: 9
int ComplexityOf_insertNodeAsChild (Node* child, Node* father)
{
	int complexity = 4;

	if (father->child == NULL)
		complexity += 2;
	else
		complexity += ComplexityOf_insertNodeInList(child, father->child);

	return complexity;
}

// Upper-bound: 11
int ComplexityOf_insertRootInFiboHeap (FiboHeap* fibo_heap, Node* new_root)
{
	int complexity = 3;

	if (fibo_heap->min_element == NULL)
		complexity += 1;
	else
	{
		complexity += ComplexityOf_insertNodeInList(new_root, fibo_heap->min_element) + 2;

		if (new_root->key < fibo_heap->min_element->key)
			complexity += 1;
	}

	return complexity;
}

// Upper-bound: 19
int ComplexityOf_moveSubHeapToRoot (FiboHeap* fibo_heap, Node* sub_heap)
{
	return ComplexityOf_extractNodeFromList(sub_heap) // max. 11
		 + ComplexityOf_insertNodeInList(sub_heap, fibo_heap->min_element) // max. 5
		 + 3;
}

// Upper-bound: 22
int ComplexityOf_linkRootNodes (FiboHeap* fibo_heap, Node* root_child, Node* root_father)
{
	return ComplexityOf_extractNodeFromList(root_child) // max. 11
		 + ComplexityOf_insertNodeAsChild(root_child, root_father) // max. 9
		 + 2;
}

// Worst-case complexity computation
// Estimating the actual number of operations requires heavy operations on the data structure
int ComplexityOf_consolidateFiboHeap (FiboHeap* fibo_heap)
{
	int complexity = 6;

	// Initialization
	complexity += fibo_heap->nb_nodes;

	// Main loop
	// Includes worst-case upper bound of the second loop (over roots_of_degree)
	complexity += fibo_heap->nb_nodes
				* fibo_heap->nb_nodes * (10 + 22 /* max of linkRootNodes */);

	// Minimum-element update
	complexity += 1 + (fibo_heap->nb_nodes * 5);

	return complexity;
}

// Worst-case complexity computation
int ComplexityOf_extractMinFromFiboHeap (FiboHeap* fibo_heap)
{
	int complexity = 1;
	Node* min_element = fibo_heap->min_element;

	if (min_element == NULL)
		return complexity + 1;

	complexity += 8;

	unsigned int nb_children = min_element->degree;

	// Upper bound for the loop over all the children
	complexity += 1
				+ nb_children * (19 + 3); 
	complexity += ComplexityOf_extractNodeFromList(min_element);

	if (fibo_heap->degree == 0)
		complexity++;
	else
		complexity += ComplexityOf_consolidateFiboHeap(fibo_heap)
					+ 1;

	return complexity;
}

// Upper-bound: 21
int ComplexityOf_cutNodeInFiboHeap (FiboHeap* fibo_heap, Node* node_to_cut)
{
	return 2
		 + ComplexityOf_moveSubHeapToRoot(fibo_heap, node_to_cut);
}

// Upper-bound: ???
// Using the total number of nodes would totally fake the results...
int ComplexityOf_recursiveCutsInFiboHeap (FiboHeap* fibo_heap, Node* node_to_cut)
{
	int complexity = 2;
	Node* node_father = node_to_cut->father;
	
	if (node_father == NULL)
		return complexity;

	complexity++;
	if (node_to_cut->is_tagged == false)
		complexity++;
	else
	{
		complexity += ComplexityOf_cutNodeInFiboHeap(fibo_heap, node_to_cut)
					+ ComplexityOf_recursiveCutsInFiboHeap(fibo_heap, node_father);
	}

	return complexity;
}

// Upper-bound: ??? (because of recursion, see above function)
// Using the total number of nodes would totally fake the results...
int ComplexityOf_decreaseKeyInFiboHeap (FiboHeap* fibo_heap, Node* node, int new_key)
{
	int complexity = 6;
	Node* node_father = node->father;

	if (node_father != NULL
	&&  new_key < node_father->key)
	{
		complexity += ComplexityOf_cutNodeInFiboHeap(fibo_heap, node)
					+ ComplexityOf_recursiveCutsInFiboHeap(fibo_heap, node_father);
	}

	if (new_key < fibo_heap->min_element->key)
		complexity++;

	return complexity;
}

//------------------------------------------------------------------------------
// FUNCTIONS FROM FILE "dijkstra.c"
//------------------------------------------------------------------------------

int ComplexityOf_extractMinimumNaive (bool*seen, int*lengths, int nb_vertexes)
{
	int res = 0;
	int S = nb_vertexes;
	res = 1+S*6;
	return res;
}

int ComplexityOf_dijkstraNaive (Graph* g, int s) // O(#A²)
{
	int res = 0;
	int S = g->nb_vertexes;
	int A = numberOfEdges(g);
	res = S*2
		+2
		+S*(ComplexityOf_extractMinimumNaive(NULL,NULL,S)+4)
		+2*A*(5);
	return res;
}

// Upper bound of the real complexity!
// Partially using worst-case complexities (because of Fibonacci heaps)

/* Please note:

   This complexity computation is INCOMPLETE and (thus) UNREALISTIC!
   It misses two upper-bound approximations (relying on data structures which
   are not built here). The computed complexity thus is probably below the real
   one, given that the two ommited complexities concern important functions
   called by this optimized algorithm!
*/
int ComplexityOf_dijkstra (Graph* g, int s)
{
	// Initialization
	int complexity = 2
				   + ComplexityOf_createFiboHeap()
				   + 2
				   + 1
				   + g->nb_vertexes * (5 // Fibonacci heap initialization
				   					+  ComplexityOf_createIsolatedNode(0, 0)
				   					+  11 /* max. of insertRootInFiboHeap */);

	// Main loop of Dijkstra's algorithm
	complexity += 1
				+ g->nb_vertexes * (1
								 /* MISSING max. of extractMinFromFiboHeap */
								 +  2);

	// Simulate the while loop (each vertex is reviewed once)
	int nb_edges = numberOfEdges(g);
	complexity += nb_edges * (1
						   +  6
						   +  1
						   /* MISSING max. of decreaseKeyInFiboHeap */
						   +  1);

	// Approximation for Fibonacci heap freeing (since none is built)
	complexity += g->nb_vertexes * (5 + ComplexityOf_freeNode(NULL)) + 1;

	return complexity;
}
