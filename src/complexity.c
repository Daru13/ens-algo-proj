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
// unsigned int COMPLEXITY = 0;

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
	Node* current_node = to_merge;

	do
	{
		complexity += 3;
		current_node = current_node->next;
	}
	while (current_node != to_merge_last_node);

	return complexity;
}

int ComplexityOf_insertNodeAsChild (Node* child, Node* father)
{
	int complexity = 4;

	if (father->child == NULL)
		complexity += 2;
	else
		complexity += ComplexityOf_insertNodeInList(child, father->child);

	return complexity;
}

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

int ComplexityOf_moveSubHeapToRoot (FiboHeap* fibo_heap, Node* sub_heap)
{
	return ComplexityOf_extractNodeFromList(sub_heap)
		 + ComplexityOf_insertNodeInList(sub_heap, fibo_heap->min_element)
		 + 3;
}

int ComplexityOf_linkRootNodes (FiboHeap* fibo_heap, Node* root_child, Node* root_father)
{
	return ComplexityOf_extractNodeFromList(root_child)
		 + ComplexityOf_insertNodeAsChild(root_child, root_father)
		 + 2;
}

/* NE TERMINE ET FONCTIONNE PROBABLEMENT PAS (besoin de réelles opérations) ! */
int ComplexityOf_consolidateFiboHeap (FiboHeap* fibo_heap)
{
	int complexity = 6;

	Node* roots_of_degree[fibo_heap->nb_nodes];
	for (unsigned int i = 0; i <= fibo_heap->nb_nodes; i++)
	{
		roots_of_degree[i] = NULL;
		complexity++;
	}	

	Node* current_node = fibo_heap->min_element;

	// Variables used to control the upcoming loop
	unsigned int nb_roots 		  = fibo_heap->degree;
	unsigned int nb_visited_roots = 0;
	bool node_has_been_linked 	  = false;

	while (nb_visited_roots < nb_roots
	   ||  node_has_been_linked)
	{
		complexity += 8;

		if (nb_visited_roots >= nb_roots)
		{
			nb_visited_roots = 0;
			node_has_been_linked = false;
			complexity += 2;
		}

		unsigned int current_degree = current_node->degree;

		while (roots_of_degree[current_degree] != NULL
		   &&  roots_of_degree[current_degree] != current_node
		   /*&&  current_node->father == NULL*/)
		{
			complexity += 7;

			Node* current_degree_root = roots_of_degree[current_degree];

			if (current_node->key > current_degree_root->key)
			{
				Node* current_node_copy = current_node;
				current_node 			= current_degree_root;
				current_degree_root 	= current_node_copy;

				complexity += 3;	
			}

			complexity += ComplexityOf_linkRootNodes(fibo_heap,
				current_degree_root, current_node);

			roots_of_degree[current_degree] = NULL;
			current_degree++;
			node_has_been_linked = true;
			// nb_roots--;
		}

		roots_of_degree[current_degree] = current_node;

		current_node = current_node->next;
		nb_visited_roots++;
	}

	complexity++;
	for (unsigned int i = 0; i < fibo_heap->nb_nodes; i++)
	{
		complexity += 3;
		if (roots_of_degree[i] != NULL)
		{
			complexity++;
			if (roots_of_degree[i]->key < fibo_heap->min_element->key)
			{
				fibo_heap->min_element = roots_of_degree[i];
				complexity++;
			}
		}
	}

	return complexity;
}

/* NE TERMINE ET FONCTIONNE PROBABLEMENT PAS (besoin de réelles opérations) ! */
int ComplexityOf_extractMinFromFiboHeap (FiboHeap* fibo_heap)
{
	int complexity = 9;

	Node* min_element = fibo_heap->min_element;

	// If the minimum element does not exist, nothing else to do
	if (min_element == NULL)
		return complexity + 1;

	Node* current_child 	 = min_element->child;
	unsigned int nb_children = min_element->degree;

	complexity++;
	for (unsigned int i = 0; i < nb_children; i++)
	{
		current_child = current_child->next;
		complexity += ComplexityOf_moveSubHeapToRoot(fibo_heap, current_child->previous)
					+ 3;
	}

	complexity += ComplexityOf_extractNodeFromList(min_element);

	// If the Fibonacci heap only had one root, it now is empty
	if (fibo_heap->degree == 0)
		complexity++;
	
	// Otherwise, the Fibonacci heap must be consolidated
	else
	{
		complexity += ComplexityOf_consolidateFiboHeap(fibo_heap)
				    + 1;
	}

	return complexity;
}

/* FONCTIONS DE DIMINUTION DE NOEUD MANQUANTES */

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
