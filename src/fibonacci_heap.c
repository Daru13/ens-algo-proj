//------------------------------------------------------------------------------
// FIBONACCI HEAPS
//------------------------------------------------------------------------------
// Implementation of Fibonacci heaps.
// Note: the acronym "CDLL" stands for Circular Double-Linked List
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "toolbox.h"
#include "fibonacci_heap.h"

//------------------------------------------------------------------------------
// Basic operations on nodes
//------------------------------------------------------------------------------

// Returns a pointer to a new node
Node* createNode (NodeValue value, int key)
{
	Node* new_node = malloc(sizeof(Node));
	CHECK_MALLOC(new_node)

	new_node->next 		= NULL;
	new_node->previous 	= NULL;
	new_node->father 	= NULL;
	new_node->child 	= NULL;
	new_node->value 	= value;
	new_node->key 		= key;
	new_node->degree 	= 0;
	new_node->is_tagged = false;

	return new_node;
}

// Returns a pointer to a new, isolated node (first of a CDLL)
Node* createIsolatedNode (NodeValue value, int key)
{
	Node* new_isolated_node = createNode(value, key);

	new_isolated_node->next 	= new_isolated_node;
	new_isolated_node->previous = new_isolated_node;

	return new_isolated_node;
}

// Free a node
void freeNode (Node* node)
{
	free(node);
}

// Recursively free nodes forming a tree, from a given root
void freeNodeTree (Node* root)
{
	// Free all the root's children (recursively, DFS-like), if any
	Node* current_child = root->child;
	if (current_child != NULL)
	{
		// Free all the siblings of the pointed child, if any
		Node* next_child;
		do
		{
			next_child = next_child->next;
			freeNodeTree(current_child);

			current_child = next_child;
		}
		while (current_child != root->child);
	}

	// Free the root itself
	free(root);
}

void printNodeDetails (Node* node)
{
	// Check if the node is NULL
	if (node == NULL)
	{
		printf("(Nothing to print, node is NULL)\n");
		return;
	}

	// Else, print various fields of the node
	printf("Info about node (%p : %d)\n", (void*) node, node->key);
	printf("| Degree: %d\n", node->degree);
	printf("| Value: %d\n", node->value);
	printf("| Next: (%p : %d) and Previous: (%p : %d)\n",
		(void*) node->next, node->next->key,
		(void*) node->previous, node->previous->key);
	printf("| Father: (%p : %d) and Child: (%p : %d)\n",
		(void*) node->father, node->father != NULL ? node->father->key : -1,
		(void*) node->child, node->child != NULL ? node->child->key : -1);
	printf("| Tagged: %s\n", node->is_tagged ? "true" : "false");
}

// Print a node and all its siblings (forming a CDLL)
void printListOfNodes (Node* node)
{
	// Check if the list is empty (node is NULL)
	if (node == NULL)
	{
		printf("(Nothing to print, node is NULL)\n");
		return;
	}

	// Else, print all the values of all the nodes as well as their number
	Node* current_node = node;
	int nb_nodes = 0;

	do
	{
		// printf("%s(%p : %d)", nb_nodes == 0 ? "" : " - ",
		// 	(void*) current_node, current_node->key);

		printf("(%p : %d) of degree %d\n", (void*) current_node,
			current_node->key, current_node->degree);

		current_node = current_node->next;
		nb_nodes++;
	}
	while (current_node != node);

	printf(" (%d elements)\n", nb_nodes);
}

//------------------------------------------------------------------------------
// Basic operations on Fibonacci heaps
//------------------------------------------------------------------------------

// Returns a pointer to a new, empty Fibonacci heap
FiboHeap* createFiboHeap ()
{
	FiboHeap* new_fibo_heap = malloc(sizeof(FiboHeap));
	CHECK_MALLOC(new_fibo_heap)

	new_fibo_heap->min_element 	= NULL;
	new_fibo_heap->degree 		= 0;
	new_fibo_heap->nb_nodes 	= 0;

	return new_fibo_heap;
}

// Free a Fibonacci heap (including all the nodes)
void freeFiboHeap (FiboHeap* fibo_heap)
{
	// Free all the rooted trees, if any
	Node* current_root = fibo_heap->min_element;
	if (current_root != NULL)
	{
		// Free all the siblings of the minimum element, if any
		Node* next_root;
		do
		{
			next_root = next_root->next;
			freeNodeTree(current_root);

			current_root = next_root;
		}
		while (current_root != fibo_heap->min_element);
	}

	// Free the Fibonacci heap structure itself
	free(fibo_heap);
}

void printFiboHeap (FiboHeap* fibo_heap)
{
	Node* min_element = fibo_heap->min_element;
	if (min_element == NULL)
	{
		printf("(Nothing to print, minimum element is NULL)\n");
		return;
	}

	printf("This Fibonacci heap contains %d node(s) and has a degree of %d.\n",
		fibo_heap->nb_nodes, fibo_heap->degree);
	printListOfNodes(min_element);
}

//------------------------------------------------------------------------------
// Advanced operations on nodes
//------------------------------------------------------------------------------

// Extracts a node from a CDLL while conserving the CDLL structure
void extractNodeFromList (Node* node)
{	
	// If the node has a father, it must be updated
	if (node->father != NULL)
	{
		// printf("ExtractNode: father found!\n");

		(node->father->degree)--;

		// The father must point to another node is the list is not empty,
		// or to NULL otherwise
		node->father->child = NULL;
		if (node->next != node)
			node->father->child = node->next;

		// The extracted node now have no father (useful or not?)
		node->father = NULL;
	}
	
	// If the node has siblings, the CDLL structure must be updated
	if (node->next != node)
	{
		// printf("ExtractNode: sibling found!\n");

		node->next->previous = node->previous;
		node->previous->next = node->next;

		// Safer: the node becomes a single-element CDLL
		node->next 		= node;
		node->previous 	= node;
	}
}

// Insert a single node in a CDLL
void insertNodeInList (Node* node_to_insert, Node* destination)
{
	// Update the inserted node node pointers
	node_to_insert->previous = destination->previous;
	node_to_insert->next 	 = destination;

	// Update the list pointers
	destination->previous->next = node_to_insert;
	destination->previous 		= node_to_insert;

	// Update the inserted node's father
	node_to_insert->father = destination->father;
}

// Merge two non-NULL CDLLs, by adding to_merge CDLL's nodes in the destination CDLL
// Both lists must be different; else, the result is undefined!
void mergeListsOfNodes (Node* to_merge, Node* destination)
{
	// Get the last node of each list
	// (assuming the given nodes are the "first" ones)
	Node* to_merge_last_node 	= to_merge->previous;
	Node* destination_last_node = destination->previous;

	// Update the pointers of the first and last elements of each list
	destination->previous = to_merge_last_node;
	to_merge->previous 	  = destination_last_node;

	destination_last_node->next = to_merge;
	to_merge_last_node->next 	= destination;

	// Update all the inserted nodes' fathers
	Node* new_father   = destination->father;
	Node* current_node = to_merge;

	do
	{
		current_node->father = new_father;
		current_node 		 = current_node->next;
	}
	while (current_node != to_merge_last_node);
}

// Insert a node as the child of a given node (supposed not a child of the father)
// This function also updates the fields which must be changed
void insertNodeAsChild (Node* child, Node* father)
{
	// The child must not already have the given father
	assert(child->father != father);
	assert(father->child != child);

	// Insert the node in the CDLL
	if (father->child == NULL)
	{
		father->child = child;
		child->father = father;
	}
	else
		insertNodeInList(child, father->child);

	// Increase the degree of the father
	(father->degree)++;
}

//------------------------------------------------------------------------------
// Advanced operations on Fibonacci heaps
//------------------------------------------------------------------------------

// Insert a *single* heap root into a Fibonacci heap
void insertRootInFiboHeap (FiboHeap* fibo_heap, Node* new_root)
{
	if (fibo_heap->min_element == NULL)
		fibo_heap->min_element = new_root;
	else
	{
		insertNodeInList(new_root, fibo_heap->min_element);

		// Update the minimum element if necessary
		int node_key = new_root->key;
		if (node_key < fibo_heap->min_element->key)
			fibo_heap->min_element = new_root;
	}

	// Update the degree of the heap
	(fibo_heap->degree)++;
	(fibo_heap->nb_nodes)++;
}

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

	// 2. Add sub_heap to the list of roots
	// This also increase the degree of the Fibonacci heap
	insertNodeInList(sub_heap, fibo_heap->min_element);
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

	// Create and init an array of nodes indexed on degrees
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

	while (nb_visited_roots < fibo_heap->degree
	   ||  node_has_been_linked)
	{
		// Reset the control variable(s)
		node_has_been_linked = false;
		if (nb_visited_roots >= nb_roots)
			nb_visited_roots = 0;

		// Next node of the current one
		next_node = current_node->next;

		// Initial degree of the current iteration
		unsigned int current_degree = current_node->degree;

		while (roots_of_degree[current_degree] != NULL)
		{
			// Root node having the same degree
			Node* current_degree_root = roots_of_degree[current_degree];

			// The node with the highest key becomes the child of the other one
			if (current_node->key > current_degree_root->key)
			{
				Node* current_node_copy = current_node;
				current_node 			= current_degree_root;
				current_degree_root 	= current_node_copy;
			}

			linkRootNodes(fibo_heap, current_degree_root, current_node);

			// The current degree don't have any associated node anymore,
			// but the next degree must be checked
			roots_of_degree[current_degree] = NULL;
			current_degree++;

			// Update of the control variables
			node_has_been_linked = true;
			//nb_roots--;
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

	printf("Fin de la consolidation :\n");
	printListOfNodes(fibo_heap->min_element);
	printf("-----\n");
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

	printf("\nExtract min (%p : %d) of degree %d\n",
		(void*) min_element, min_element->key, min_element->degree);

	for (unsigned int i = 0; i < nb_children; i++)
	{
		current_child = current_child->next;
		moveSubHeapToRoot(fibo_heap, current_child->previous);

		printf("Liste enfants après extraction %d (extract min):\n", i);
		printListOfNodes(current_child);
	}

	// The minimum element is extracted from the roots
	Node* min_element_next = min_element->next;
	extractNodeFromList(min_element);

	// If the Fibonacci heap only had one root, it now is empty
	if (fibo_heap->degree == 0)
	{
		printf("> Fibo vide\n");
		fibo_heap->min_element = NULL;
	}
	
	// Otherwise, the Fibonacci heap must be consolidated
	else
	{
		printf("> Consolidation\n");
		// A (temporary) fake minimum element is used
		fibo_heap->min_element = min_element_next;

		consolidateFiboHeap(fibo_heap);
	}

	// The total number of nodes must be updated here
	(fibo_heap->nb_nodes)--;

	return min_element;
}

/*
// Extracts the minimum node of a Fibonacci heap, and returns it
// The structure of the heap is modified so it remains correct
Node* extractMinFromFiboHeap (FiboHeap* fibo_heap)
{
	// Get the minimum
	Node* min_element = fibo_heap->min_element;

	// If the minimum element does not exist, immediately returns NULL
	if (min_element == NULL)
		return NULL;

	printf("\n\n---------- Min element about to be extracted ----------\n");
	// printFiboHeap(fibo_heap);

	// If it exists, all its children (if any) become new roots of the heap
	Node* current_child = min_element->child;
	if (current_child != NULL)
	{
		Node* next_child = NULL;
		do {

			// Get the next child first...
			next_child = current_child->next;

			printf("Avant extraction, noeud parent (%d) : %d fils\n",
				current_child->value, min_element->degree);

			// ...then extract the current child from the minimum's children...
			extractNodeFromList(current_child);

			// ...and finally insert it as a new root
			setSubHeapAsRoot(fibo_heap, current_child);

			printf("Après extract/réinsertion, noeud parent : %d fils\n",
				min_element->degree);
			// printFiboHeap(fibo_heap);

			//extractNodeFromList(current_child);
			printf("Après extraction, noeud parent (%x) : %d fils\n", (int) min_element, min_element->degree);
			printf("Parent noeud actuel : %x\n", (int) current_child->father);

			printf("cur : %x ; next : %x\n", (int) current_child, (int) next_child);
			current_child = next_child;
		}
		while (min_element->degree > 0);
	}

	printf(">>>> Avant extraction min\n");
	printFiboHeap(fibo_heap);

	// The minimum node is extracted from the list of heaps
	Node* min_element_next = min_element->next;
	extractNodeFromList(min_element);

	// If the Fibonacci heap only had one root, its new minimum is NULL
	if (min_element_next == min_element) {
		printf("> Fibo vide\n");
		fibo_heap->min_element = NULL;
	}
	
	// Otherwise, the heap is consolidated  after another node
	// is set as a temporarily, "fake" minimum
	else
	{
		printf("> Consolidation\n");
		fibo_heap->min_element = min_element_next;
		consolidateFiboHeap(fibo_heap);
	}

	// The degree and the total number of nodes of the heap are both decreased
	(fibo_heap->degree)--;
	(fibo_heap->nb_nodes)--;

	printf(">>>> Après extraction min (= %d)\n", min_element->value);
	printFiboHeap(fibo_heap);

	return min_element;
}
*/
