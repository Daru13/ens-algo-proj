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


#define CHECK_MALLOC(ptr) assert(ptr != NULL);

//------------------------------------------------------------------------------
// Basic operations
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
	// Free all the root's children
	Node* current_child = root->child;
	if (current_child != NULL)
	{
		Node* next_child = current_child->next;
		while (next_child != root->child)
		{
			current_child 	= next_child;
			next_child 		= next_child->next;

			freeNodeTree(current_child);
		}

		freeNodeTree(root->child);
	}

	// Free the root itself
	free(root);
}

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

	// Else, nodes must be counted
	unsigned int nb_siblings = 0;

	Node* current_node = node;
	while (current_node->next != NULL // Useless ?
	   &&  current_node->next != node)
	{
		current_node = current_node->next;
		nb_siblings++;
	}

	return nb_siblings;
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

// Free a Fibonacci heap
void freeFiboHeap (FiboHeap* fibo_heap)
{
	// Free all the (sub-)heaps
	Node* current_heap_root = fibo_heap->min_element;
	if (current_heap_root != NULL)
	{
		Node* next_heap_root = current_heap_root->next;
		while (next_heap_root != fibo_heap->min_element)
		{
			current_heap_root 	= next_heap_root;
			next_heap_root 		= next_heap_root->next;

			freeNodeTree(current_heap_root);
		}

		freeNodeTree(fibo_heap->min_element);
	}

	// Free the Fibonacci heap itself
	free(fibo_heap);
}

//------------------------------------------------------------------------------
// Operations on nodes
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

	// Update the pointers to the father
	Node* current_node = source;
	while (current_node != destination_last_node)
	{
		current_node->father = destination->father;
		current_node = current_node->next;
	}

	// Update the degree of the father, if any

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



//------------------------------------------------------------------------------

void printTestingInfo (Nodes* const nodes, const unsigned int n)
{
	for (int i = 0; i < n; i++)
		printf("Node %d has %d sibling(s) and %d father.\n",
			i,
			getNbNodesOfList(nodes[i]) - 1,
			nodes[i]->father != NULL ? 1 : 0);
}

int main ()
{
	//---------- Nodes of CDLLs ----------
	
	NodeValue node_val_0 = 0;
	NodeValue node_val_1 = 42;
	NodeValue node_val_2 = -128;

	Node* nodes[3];
	nodes[0] = createIsolatedNode(node_val_0);
	nodes[1] = createIsolatedNode(node_val_1);
	nodes[2] = createIsolatedNode(node_val_2);

	printTestingInfo(nodes, 3);

	printf("[Nodes 0, 1 and 2 are merged]\n");
	mergeNodeLists(nodes[0], nodes[1]);
	mergeNodeLists(nodes[0], nodes[2]);

	printTestingInfo(nodes, 3);

	printf("[Node 1 is extracted]\n");
	extractNodeFromList(nodes[1]);

	printTestingInfo(nodes, 3);

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

	return 0;
}
