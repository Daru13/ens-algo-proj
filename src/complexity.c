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
unsigned int COMPLEXITY = 0;

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

int ComplexityOf_graphIsConnected (Graph* g) 
{
	int compt = 0; 
    List* waiting = createList();
    compt = compt +  ComplexityOf_createList ();
	
	addElementToList(waiting, 0);
	compt +=  ComplexityOf_addElementToList ();
 
    bool seen[g->nb_vertexes];
	compt += g->nb_vertexes;

    for (int i = 0 ; i < g->nb_vertexes; i++)
     {
         seen[i] = false;
         (compt)++;
     }
     while (! listIsEmpty(waiting))
     {
         int nd = popFromList(waiting) ;
         compt = compt +2;
         if ( !seen[nd])
         {
            seen[nd] = true;
            Edge* EL = g->edges[nd];
            compt = compt +2;
            while ( EL != NULL)
            {
                addElementToList (waiting, EL->destination);
				compt += ComplexityOf_addElementToList();
                EL = EL->next;
                (compt)++;
            }
 
        }
    }

    bool res = true;
    (compt)++;
    for (int i=0; i<g->nb_vertexes ; i++)
    {
         res = res && (seen[i]);
         compt++;
    }
    return compt;
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

/*

// Move a *non-root* sub-heap of a Fibonacci heap to the root level
// The Fibonacci heap is assumed to contain sub_root (and thus to be non-empty)
void moveSubHeapToRoot (FiboHeap* fibo_heap, Node* sub_heap)
{
	// Sub_heap must not be at the root level (nor just extracted!)
	assert(sub_heap->father != NULL);
	// The FIbonacci heap must not be empty either
	assert(fibo_heap->min_element != NULL);

	// 1. Extract sub_heap node from its CDLL
	extractNodeFromList(sub_heap);

	// 2.1. Add sub_heap to the list of roots
	insertNodeInList(sub_heap, fibo_heap->min_element);

	// 2.2. Increase the degree of the Fibonacci heap
	(fibo_heap->degree)++;
}

// Move a rooted heap of a Fibonacci heap as a child of another root
void linkRootNodes (FiboHeap* fibo_heap, Node* root_child, Node* root_father)
{
	// 1.1. Extract the heap node from the root CDLL
	extractNodeFromList(root_child);

	// 1.2. Decrease the degree of the Fibonacci heap (since it looses one root)
	(fibo_heap->degree)--;

	// 2.1. Insert the new child as a child of the other root node
	insertNodeAsChild(root_child, root_father);

	// 2.2. Set the tag of the new child to false (by definition of the tag)
	root_child->is_tagged = false;
}

// Consolidate a non-empty Fibonacci heap, i.e. force all the roots of the heaps
// to have different degrees by rearranging the structure of the Fibonacci heap
void consolidateFiboHeap (FiboHeap* fibo_heap)
{
	// The Fibonacci heap should not be empty
	assert(fibo_heap->min_element != NULL);

	// Create an array of nodes indexed on degrees
	// The highest reachable degree is related to the total number of nodes...
	Node* roots_of_degree[fibo_heap->nb_nodes];
	for (unsigned int i = 0; i <= fibo_heap->nb_nodes; i++)
		roots_of_degree[i] = NULL;

	// The loop iterates over the number of root nodes, but it will be repeated
	// endlessy until there has been an iteration with no change, meaning the
	// Fibonacci heap has been consolidated (no two roots of the same degree)

	// Current node being processed in the loop, and auxiliary (next) node
	Node* current_node = fibo_heap->min_element;
	Node* next_node;

	// Variables used to control the upcoming loop
	unsigned int nb_roots 		  = fibo_heap->degree;
	unsigned int nb_visited_roots = 0;
	bool node_has_been_linked 	  = false;

	while (nb_visited_roots < nb_roots
	   ||  node_has_been_linked)
	{
		// Reset the control variable(s) and the per-degree nodes array
		if (nb_visited_roots >= nb_roots)
		{
			nb_visited_roots = 0;
			node_has_been_linked = false;
		}

		// printf("\n\nWhile 1: %d/%d [current_node info] ", nb_visited_roots, nb_roots);
		// printNodeDetails(current_node);

		// Next node of the current one
		next_node = current_node->next;

		// Initial degree of the current iteration
		unsigned int current_degree = current_node->degree;

		while (roots_of_degree[current_degree] != NULL
		   &&  roots_of_degree[current_degree] != current_node
		   &&  current_node->father == NULL)
		{
			// Root node having the same degree
			Node* current_degree_root = roots_of_degree[current_degree];

			// The node with the highest key becomes the child of the other one
			if (current_node->key > current_degree_root->key)
			{
				Node* current_node_copy = current_node;
				current_node 			= current_degree_root;
				current_degree_root 	= current_node_copy;

				// Update the next node to visit to reflect this inversion
				next_node = current_node->next;				
			}

			// printf("\nWhile 2 (current degree = %d) ", current_degree);
			// printNodeDetails(current_node);
			// printNodeDetails(current_degree_root);

			linkRootNodes(fibo_heap, current_degree_root, current_node);

			// The current degree don't have any associated node anymore,
			// but the next degree must be checked
			roots_of_degree[current_degree] = NULL;
			current_degree++;

			// Update of the control variables
			node_has_been_linked = true;
			nb_roots--;

			// printf("** Fin while 2 **\n");
			// printFiboHeap(fibo_heap);
		}

		// Set current_node as the only node of current_degree
		roots_of_degree[current_degree] = current_node;

		// The next root becomes the new current one
		current_node = next_node;

		// Update of the control variables
		nb_visited_roots++;
	}

	// The actual min element of the Fibonacci heap is finally updated
	for (unsigned int i = 0; i < fibo_heap->nb_nodes; i++)
		if (roots_of_degree[i] != NULL)
		{
			if (roots_of_degree[i]->key < fibo_heap->min_element->key)
				fibo_heap->min_element = roots_of_degree[i];
		}

	// printf("\nFin de la consolidation :\n");
	printFiboHeap(fibo_heap);
	printf("----------------------------------------------------------\n");
}

Node* extractMinFromFiboHeap (FiboHeap* fibo_heap)
{
	Node* min_element = fibo_heap->min_element;

	// If the minimum element does not exist, nothing else to do
	if (min_element == NULL)
		return NULL;

	// If it exists, all its children (if any) become new roots of the heap
	Node* current_child 	 = min_element->child;
	unsigned int nb_children = min_element->degree;

	// printf("\nExtract min (%p : %d) of degree %d\n",
	//	(void*) min_element, min_element->key, min_element->degree);

	for (unsigned int i = 0; i < nb_children; i++)
	{
		current_child = current_child->next;
		moveSubHeapToRoot(fibo_heap, current_child->previous);

		// printf("Liste enfants après extraction %d (extract min):\n", i);
		// printListOfNodes(current_child);
	}

	// The minimum element is extracted from the roots
	Node* min_element_next = min_element->next;
	extractNodeFromList(min_element);

	// The total number of nodes and the Fibonacci heap degree must be updated
	(fibo_heap->nb_nodes)--;
	(fibo_heap->degree)--;

	// If the Fibonacci heap only had one root, it now is empty
	if (fibo_heap->degree == 0)
		fibo_heap->min_element = NULL;
	
	// Otherwise, the Fibonacci heap must be consolidated
	else
	{
		// A (temporary) fake minimum element is used
		fibo_heap->min_element = min_element_next;

		consolidateFiboHeap(fibo_heap);
	}

	return min_element;
}

void cutNodeInFiboHeap (FiboHeap* fibo_heap, Node* node_to_cut)
{
	// The given node must have a father to be extracted from its children
	assert(node_to_cut->father != NULL);

	moveSubHeapToRoot(fibo_heap, node_to_cut);
	node_to_cut->is_tagged = false;
}

void recursiveCutsInFiboHeap (FiboHeap* fibo_heap, Node* node_to_cut)
{
	Node* node_father = node_to_cut->father;
	
	// If the root is reached, the recursion must end
	if (node_father == NULL) return;

	if (node_to_cut->is_tagged == false)
		node_to_cut->is_tagged = true;
	else
	{
		cutNodeInFiboHeap(fibo_heap, node_to_cut);
		recursiveCutsInFiboHeap(fibo_heap, node_father);
	}
}

void decreaseKeyInFiboHeap (FiboHeap* fibo_heap, Node* node, int new_key)
{
	// The new key must be smaller than the current one
	assert(new_key <= node->key);
	node->key = new_key;

	Node* node_father = node->father;

	if (node_father != NULL
	&&  node->key < node_father->key)
	{
		cutNodeInFiboHeap(fibo_heap, node);
		recursiveCutsInFiboHeap(fibo_heap, node_father);
	}

	// If the root has become a room, it may be the new minimum element
	if (new_key < fibo_heap->min_element->key)
		fibo_heap->min_element = node;
}

*/

//------------------------------------------------------------------------------
// FUNCTIONS FROM FILE "dijkstra.c"
//------------------------------------------------------------------------------

int ComplexityOf_extractMinimumNaive (bool* seen, int* lengths, int nb_vertexes)
{
	int compt = 0;
	int i;
	int min = -1;
	compt += 2;

	for (i = 0; i < nb_vertexes; i++)
	{
		compt += 2;
		if (lengths[i] != INF_LENGTH
		&& !seen[i])
		{
			compt++;
			if (min == -1)
			{
				min = i;
				compt++;
			} 
			else 
			{
				compt++;
				if (lengths[min] > lengths[i])
				{
					min = i;
					compt++;
				}
			}
		}
	}


	return compt;
}

int ComplexityOf_dijkstraNaive (Graph* g, int s) // O(#A²)
{
	// Initialization
	int compt = 0;
	int* lengths = malloc(g->nb_vertexes * sizeof(int));
	CHECK_MALLOC(lengths);
	bool seen[g->nb_vertexes];
	compt += g->nb_vertexes;
	
	for (int i = 0 ; i < g->nb_vertexes ; i++)
	{
		seen[i]    = false;
		lengths[i] = INF_LENGTH;
		compt += 2;
	}
	lengths[s]  = 0;
	compt++;
	
	// Main loop (over the priority queue structure)
	for (int i = 0; i<g->nb_vertexes;i++)
	{
		// The element with the smallest distance is extracted
		int* useless;
		int min_elt = extractMinimumNaive(seen,
			lengths, g->nb_vertexes, useless);
		compt += ComplexityOf_extractMinimumNaive(
			seen,lengths, g->nb_vertexes);
		seen[min_elt] = true;
		compt++;
		// Iteration over all min_elt's neighbours
		Edge* current_edge = g->edges[min_elt];
		compt++;
		while (current_edge != NULL)
		{
			// Each neighbour is added to the priority structure
			int neighbour = current_edge->destination;
			compt++;
			// Improve the lengths if possible
			int weight = current_edge->weight;
			compt++;
					
			compt += 2;
			if (lengths[neighbour] == INF_LENGTH
			||  lengths[neighbour] > lengths[min_elt] + weight)
			{
				lengths[neighbour] = lengths[min_elt] + weight;
				compt++;
			}
			current_edge = current_edge->next;
			compt++;
		}
	}
	return compt;
}
