//------------------------------------------------------------------------------
// FIBONACCI HEAPS
//------------------------------------------------------------------------------
// Implementation of Fibonacci heaps.
// Note: the acronym "CDLL" stands for Circular Double-Linked List
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "toolbox.h"
#include "fibonacci_heap.h"

//------------------------------------------------------------------------------
// Creation and deletion of objects
//------------------------------------------------------------------------------

// Returns a pointer to a new node
Node* createNode (const NodeValue value)
{
	Node* new_node = malloc(sizeof(Node));
	CHECK_MALLOC(new_node)

	new_node->next 		= NULL;
	new_node->previous 	= NULL;
	new_node->father 	= NULL;
	new_node->child 	= NULL;
	new_node->value 	= value;
	new_node->degree 	= 0;
	new_node->is_tagged = false;

	return new_node;
}

// Returns a pointer to a new, isolated node (first of a CDLL)
Node* createIsolatedNode (const NodeValue value)
{
	Node* new_isolated_node = createNode(value);

	new_isolated_node->next 	= new_isolated_node;
	new_isolated_node->previous = new_isolated_node;

	return new_isolated_node;
}

// Free a node
void freeNode (Node* node)
{
	free(node);
}

// Recursively free a tree of nodes, from a given root
void freeNodeTree (Node* root)
{
	// Free all the root's children (recursively, DFS-like), if any
	Node* current_child = root->child;
	if (current_child != NULL)
	{
		// Free all the siblings of the pointed child, if any
		Node* next_child = current_child->next;
		while (next_child != root->child)
		{
			current_child 	= next_child;
			next_child 		= next_child->next;

			freeNodeTree(current_child);
		}

		// Free the pointed child
		freeNodeTree(root->child);
	}

	// Free the root itself
	free(root);
}

//------------------------------------------------------------------------------

// Returns a pointer to a new, empty Fibonacci heap
FiboHeap* createFiboHeap ()
{
	FiboHeap* new_fibo_heap = malloc(sizeof(FiboHeap));
	CHECK_MALLOC(new_fibo_heap)

	new_fibo_heap->min_element 	= NULL;
	new_fibo_heap->nb_nodes 	= 0;

	return new_fibo_heap;
}

// Free a Fibonacci heap (including all its nodesS)
void freeFiboHeap (FiboHeap* fibo_heap)
{
	// Free all the (sub-)heaps, if any
	Node* current_heap_root = fibo_heap->min_element;
	if (current_heap_root != NULL)
	{
		// Free all the minimum element's heap siblings, if any
		Node* next_heap_root = current_heap_root->next;
		while (next_heap_root != fibo_heap->min_element)
		{
			current_heap_root 	= next_heap_root;
			next_heap_root 		= next_heap_root->next;

			freeNodeTree(current_heap_root);
		}

		// Free the minimum element's heap
		freeNodeTree(fibo_heap->min_element);
	}

	// Free the Fibonacci heap structure itself
	free(fibo_heap);
}

//------------------------------------------------------------------------------
// Basic operations on nodes
//------------------------------------------------------------------------------

// Returns 1 if a node has at least one sibling, 0 otherwise
bool hasSibling (const Node* node)
{
	return node->next != node;
}

// Return 1 if a node has a father, 0 otherwise
bool hasFather (const Node* node)
{
	return node->father != NULL;
}

// Returns 1 if a node has at least one child, 0 otherwise
bool hasChild (const Node* node)
{
	return node->degree > 0;
}

// Returns the number of nodes of a CDLL
unsigned int getNbNodesOfList (Node* const node)
{
	// If the node has a father, return its degree
	if (hasFather(node))
		return node->father->degree;

	// Else, nodes of the CDLL must be counted
	unsigned int nb_siblings = 0;

	Node* current_node = node;
	while (current_node->next != NULL // Useless ?
	   &&  current_node->next != node)
	{
		current_node = current_node->next;
		nb_siblings++;
	}

	return nb_siblings + 1;
}

//------------------------------------------------------------------------------
// Advanced operations on nodes
//------------------------------------------------------------------------------

// Extracts a node from a CDLL
// If the node had siblings, they form a new CDLL without the extracted node
void extractNodeFromList (Node* node)
{	
	if (hasFather(node))
	{
		// If the node has a father, it must be updated
		(node->father->degree)--;

		// If the father pointed to the extracted node, it is updated
		node->father->child = NULL;
		if (hasSibling(node))
			node->father->child = node->next;
	}
	
	if (hasSibling(node))
	{
		// Immediate siblings must be updated
		node->next->previous = node->previous;
		node->previous->next = node->next;

		// The node becomes a single-element CDLL
		// (If the node has no sibling, it must alreayd be the case)
		node->next 		= node;
		node->previous 	= node;
	}
}

// Merge two CDLLs of nodes, by adding source nodes in the same list as destination
// Both lists must be different CDLLs!
void mergeNodeLists (Node* source, Node* destination)
{
	// Get the last node of each list (assuming the given nodes are the "first" ones)
	Node* source_last_node 		= source->previous;
	Node* destination_last_node = destination->previous;

	// Update the first and last elements of each list
	// Scheme: "LAST_DEST--------FIRST_DEST-LAST_SRC--------------FIRST_SRC"
	destination->next 	= source;
	source->previous 	= destination;

	destination_last_node->previous = source_last_node;
	source_last_node->next 			= destination_last_node;

	// TODO: nothing more here?
	return;
/*
	// Update the pointers to the father
	Node* current_node = source;
	while (current_node != destination_last_node)
	{
		current_node->father = destination->father;
		current_node = current_node->next;
	}

	// Update the degree of the father, if any
*/
}

/*
// Add a list of nodes to the children of a father node
void insertListOfNodesAsChildren (Node* node, Node* father)
{
	// TODO
	return;

	// If the father has no child, the nodes must be a CDLL
	if (! hasChild(father))
	{
		// Update the pointers to the father
		Node* current_node = source;
		while (current_node != destination_last_node)
		{
			current_node->father = destination->father;
			current_node = current_node->next;
		}

		// Update the father's pointer to one child
		father->child = node;
	}
	else
		mergeNodeLists(node, father->child);
	
	// The new child is updated
	node->is_tagged = false;

	/// The father is updated
	(father->degree) += ;
}*/

//------------------------------------------------------------------------------
// Operations on Fibonacci heaps
//------------------------------------------------------------------------------

// Insert a *single* node into a Fibonacci heap
void insertNodeInFiboHeap (FiboHeap* fibo_heap, Node* node)
{
	if (fibo_heap->min_element == NULL)
		fibo_heap->min_element = node;
	else
	{
		mergeNodeLists(node, fibo_heap->min_element);

		// Update the minimum element if necessary
		NodeValue new_value = node->value;
		if (new_value < fibo_heap->min_element->value)
			fibo_heap->min_element = node;
	}

	// Update the total number of nodes of the heap
	(fibo_heap->nb_nodes)++;
}

// Returns the minimum element of a Fibonacci heap
Node* getMinimumElement (const FiboHeap* fibo_heap)
{
	return fibo_heap->min_element;
}

FiboHeap* mergeFiboHeaps (FiboHeap* fibo_heap_1, FiboHeap* fibo_heap_2)
{
	FiboHeap* new_fibo_heap = createFiboHeap();

	// Merge the lists of nodes
	mergeNodeLists(fibo_heap_1->min_element, fibo_heap_2->min_element);

	// Set the minimum element of the new Fibonaccci heap
	Node* min_element_1 = fibo_heap_1->min_element;
	Node* min_element_2 = fibo_heap_2->min_element;

	new_fibo_heap->min_element = min_element_1;
	if (min_element_1 == NULL
	|| (min_element_2 != NULL && min_element_2->value < min_element_1->value))
		new_fibo_heap->min_element = min_element_2;

	// Set the total number of nodes of the new Fibonacci heap
	new_fibo_heap->nb_nodes = fibo_heap_1->nb_nodes
							+ fibo_heap_2->nb_nodes;

	// Free the merged Fibonacci heaps
	// This is done WITHOUT FREEING THE NODES THEY CONTAIN!
	free(fibo_heap_1);
	free(fibo_heap_2);

	return new_fibo_heap;
}
