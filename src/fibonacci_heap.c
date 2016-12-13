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
	new_fibo_heap->degree 		= 0;
	new_fibo_heap->nb_nodes 	= 0;

	new_fibo_heap->max_root_degree = 0;

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

void printFiboHeap (FiboHeap* const fibo_heap)
{
	Node* min_element = fibo_heap->min_element;
	Node* current_node = min_element;

	if (current_node == NULL)
	{
		printf("This Fibonacci heap has no (min) element.\n");
		return;
	}

	int counter = 0;
	printf("\n");
	do
	{
		printf("%s%d", counter == 0 ? "" : " <-> ", current_node->value);
		current_node = current_node->next;
		counter++;
	} while (current_node != min_element);
	printf(" [%d elements]\n", counter);
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

void printCDLL (Node* cdll_node)
{
	if (cdll_node == NULL)
	{
		printf("(No list to display)\n");
		return;
	}

	Node* current_node = cdll_node;
	do {
		printf("(%x | %d) --> ", (int) current_node, current_node->value);
		current_node = current_node->next;
	} while (current_node != cdll_node);
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
// Both lists must be different CDLLs; else, the result is undefined
// Note that pointers to father are *not* updated
void mergeNodeLists (Node* source, Node* destination)
{
	// Get the last node of each list (assuming the given nodes are the "first" ones)
	Node* source_last_node 		= source->previous;
	Node* destination_last_node = destination->previous;

	// Update the first and last elements of each list
	destination->previous 	= source_last_node;
	source->previous 		= destination_last_node;

	destination_last_node->next = source;
	source_last_node->next 		= destination;

	return;
}

// Insert a node as the child of a given node (supposed not a child of the father)
// This function also updates the fields which must be changed
void insertNodeAsChild (Node* child, Node* father)
{
	// Insert the node in the CDLL
	if (father->child == NULL)
		father->child = child;
	else
		mergeNodeLists(child, father->child);

	// Increase the degree of the father
	(father->degree)++;
}

//------------------------------------------------------------------------------
// Operations on Fibonacci heaps
//------------------------------------------------------------------------------

// Insert a *single* node into a Fibonacci heap, as a heap root
void insertSingleRootInFiboHeap (FiboHeap* fibo_heap, Node* node)
{
	// printf("Inserting node %x\n", (int) node);

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

	// Update the total number of nodes and the degree of the heap
	(fibo_heap->degree)++;
	(fibo_heap->nb_nodes)++;

	if (node->degree > fibo_heap->max_root_degree)
		fibo_heap->max_root_degree = node->degree;
}

// Move a sub-heap of a Fibonacci heap to the root level
// The Fibonacci heap is assumed to contain sub_root (and thus to be non-empty)
// and sub_root must not already be a root (otherwise, behaviour is undefined)
void setSubHeapAsRoot (FiboHeap* fibo_heap, Node* sub_root)
{
	// Add sub_root to the list of heap roots
	mergeNodeLists(sub_root, fibo_heap->min_element);

	// Update the minimum element if necessary
	NodeValue new_value = sub_root->value;
	if (new_value < fibo_heap->min_element->value)
		fibo_heap->min_element = sub_root;

	// The new root has no father anymore
	sub_root->father = NULL;

	// And the degree of the Fibonacci heap is increased
	(fibo_heap->degree)++;
}

// Merge two Fibonacci heaps
// The two heaps must be different; otherwise, the behaviour is undefined
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

	// Set the degree and the total number of nodes of the new Fibonacci heap
	new_fibo_heap->degree = fibo_heap_1->degree
						  + fibo_heap_2->degree;

	new_fibo_heap->nb_nodes = fibo_heap_1->nb_nodes
							+ fibo_heap_2->nb_nodes;

	// Free the merged Fibonacci heaps
	// This is done WITHOUT FREEING THE NODES THEY CONTAIN!
	free(fibo_heap_1);
	free(fibo_heap_2);

	return new_fibo_heap;
}

// Make a root node of a Fibonacci heap become the child of another root node
void linkRootNodes (FiboHeap* fibo_heap, Node* child, Node* father)
{
	printf("> (Link) node %x (%d) becomes child of %x (%d)\n",
			(int) child, child->value, (int) father, father->value);

	// The child node is extracted from the list of roots
	extractNodeFromList(child);

	// Decreases the number of roots of the Fibonacci heap
	// Since a node of each existent degree is kept as a root, the maximum
	// degree of the root node does not have to be changed
	(fibo_heap->degree)--;

	// Then, it is inserted as the child of the father node
	insertNodeAsChild(child, father);

	// Set the tag of the new child to false
	child->is_tagged = false;
}

// Consolidate a Fibonacci heap, i.e. force all the roots of the heaps to have
// different degrees by rearranging the structure of the Fibonacci heap
void consolidateFiboHeap (FiboHeap* fibo_heap)
{
	// If the Fibonacci heap is empty, nothing has to be done
	if (fibo_heap->min_element == NULL)
		return;

	Node* min_element = fibo_heap->min_element;

	// Create and init an array of nodes indexed on degrees
	// Degrees may potentially grow to twice the current max?
	//Node** roots_of_degree = malloc(fibo_heap->degree * sizeof(Node*));
	//CHECK_MALLOC(roots_of_degree);
	Node* roots_of_degree[fibo_heap->degree];

	for (unsigned int i = 0; i <= fibo_heap->degree; i++)
		roots_of_degree[i] = NULL;

	// Browse the list of heap roots, so that all those with the same degrees
	// are "linked" together until their degrees all differ
	Node* current_node_ref = min_element;
	if (current_node_ref != NULL)
		do 
		{
			printf("\nCONSOLIDATE cur node : %x (%d)\n",
				(int) current_node_ref, current_node_ref->value);
			
			Node* 		 current_node 	= current_node_ref;
			unsigned int current_degree = current_node->degree;

			current_node_ref = current_node_ref->next;

			// Make sure there is only one root of degree current_degree
			while (roots_of_degree[current_degree] != NULL)
			{
				Node* current_deg_root = roots_of_degree[current_degree];

				// The node with the highest value becomes the child of the other one
				// (the degree of the new father is also updated)

				//printf("current node : %x (%d)\n",
				//(int) current_node, current_node->value);
				//printf("current deg root : %x (%d)\n",
				//(int) current_deg_root, current_deg_root->value);

				if (current_node->value > current_deg_root->value)
				{
					Node* current_node_copy = current_node;
					current_node 			= current_deg_root;
					current_deg_root 		= current_node_copy;
				}

				// "Link" the two nodes together, in the right order
				linkRootNodes(fibo_heap, current_deg_root, current_node);

				// Re-init the single root associated to the current degree
				// (since it just has been increased by "linking")
				roots_of_degree[current_degree] = NULL;

				current_degree++;
			}

			// Set current_node as the only node of current_degree
			roots_of_degree[current_degree] = current_node;

			printf("Next elt of current_node: %x (%d)\n", (int) current_node->next,
				current_node->next->value);
			printf("Prev elt of current_node: %x (%d)\n", (int) current_node->previous,
				current_node->previous->value);

			// Update the minimum element if necessary
			if (current_node->value < min_element->value)
				min_element = current_node;

			// current_node_ref = current_node_ref->next;
		} while (current_node_ref != fibo_heap->min_element);

	// Finally, the minimum element is updated
	fibo_heap->min_element = min_element;

	// free(roots_of_degree);
}

// Extracts the minimum node of a Fibonacci heap, and returns it
// The structure of the heap is modified so it remains correct
Node* extractMinFromFiboHeap (FiboHeap* fibo_heap)
{
	// Get the minimum
	Node* min_element = fibo_heap->min_element;

	// If the minimum element does not exist, immediately returns NULL
	if (min_element == NULL)
		return NULL;

	printf("> Min element exists\n");
	printFiboHeap(fibo_heap);

	// If it exists, all its children (if any) become new roots of the heap
	Node* current_child = min_element->child;
	if (current_child != NULL)
	{
		Node* next_child = NULL;
		do {
			/*
			printf("CHILD BECOMES ROOT (cur child : %x with value %d\n",
				(int) current_child, current_child->value);
			printCDLL(current_child);
			*/

////////////////////////////////////////////////////////////////////////////////
			// PROBLEME DANS L'EXTRACTION !!!
			// NOEUDS NON REMIS EN RACINE ?
////////////////////////////////////////////////////////////////////////////////

			// Get the next child first...
			next_child = current_child->next;

			// ...then extract the current child from the minimum's children...
			extractNodeFromList(current_child);
			printf("> Current extracted child: %x\n", (int) current_child);

			// ...and finally insert it as a new root
			setSubHeapAsRoot(fibo_heap, current_child);

			current_child = next_child;
		}
		while (next_child != min_element->child);
	}

	printFiboHeap(fibo_heap);

	// The minimum node is extracted from the list of heaps
	Node* min_element_next = min_element->next;
	extractNodeFromList(min_element);

	// The degree and the total number of nodes of the heap are both decreased
	(fibo_heap->degree)--;
	(fibo_heap->nb_nodes)--;

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

	return min_element;
}
