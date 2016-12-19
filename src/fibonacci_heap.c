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
		printf("%s(%p : %d)", nb_nodes == 0 ? "" : " - ",
			(void*) current_node, current_node->key);

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
		printf("(Nothing to print, Fibonacci heap is NULL)\n");
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

		// The father must point to another node is the list is not empty
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

// Insert a *single* node into a Fibonacci heap, as a heap root
void insertSingleRootInFiboHeap (FiboHeap* fibo_heap, Node* node)
{
	// printf("Inserting node %x\n", (int) node);

	if (fibo_heap->min_element == NULL)
		fibo_heap->min_element = node;
	else
	{
		insertNodeInList(node, fibo_heap->min_element);

		// Update the minimum element if necessary
		NodeValue new_value = node->value;
		if (new_value < fibo_heap->min_element->value)
			fibo_heap->min_element = node;
	}

	// Update the total number of nodes and the degree of the heap
	(fibo_heap->degree)++;
	(fibo_heap->nb_nodes)++;

	//if (node->degree > fibo_heap->max_root_degree)
	//	fibo_heap->max_root_degree = node->degree;
}

// Move a sub-heap of a Fibonacci heap to the root level
// The Fibonacci heap is assumed to contain sub_root (and thus to be non-empty)
// and sub_root must not already be a root (otherwise, behaviour is undefined)
void setSubHeapAsRoot (FiboHeap* fibo_heap, Node* sub_root)
{
	// Add sub_root to the list of heap roots
	mergeListsOfNodes(sub_root, fibo_heap->min_element);

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
	mergeListsOfNodes(fibo_heap_1->min_element, fibo_heap_2->min_element);

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
	//printf("> (Link) node %x (%d) becomes child of %x (%d)\n",
	//		(int) child, child->value, (int) father, father->value);

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

	// Create and init an array of nodes indexed on degrees
	Node* roots_of_degree[fibo_heap->nb_nodes];
	for (unsigned int i = 0; i <= fibo_heap->nb_nodes; i++)
		roots_of_degree[i] = NULL;

	Node* start_node   = fibo_heap->min_element;
	Node* current_node = start_node;
	Node* next_node;

	if (start_node != NULL)
	{
		do
		{
			printf("*** début de la consolidation ***\n");
			unsigned int current_degree = current_node->degree;
			next_node = current_node->next;

			while (roots_of_degree[current_degree] != NULL)
			{
				// printf("Màj tableau par degré avec noeud courant = %x (%d))\n",
				//	(int) current_node, current_node->value);

				Node* current_deg_root = roots_of_degree[current_degree];

				// The node with the highest value becomes the child of the other one
				if (current_node->value > current_deg_root->value)
				{
					Node* current_node_copy = current_node;
					current_node 			= current_deg_root;
					current_deg_root 		= current_node_copy;
				}
				
				//if (start_node == current_deg_root)
				//		start_node = current_node;

				// "Link" the two nodes together, in the right order
				linkRootNodes(fibo_heap, current_deg_root, current_node);

				// Re-init the single root associated to the current degree
				// (since it just has been increased by "linking")
				roots_of_degree[current_degree] = NULL;

				current_degree++;
			}

			// Set current_node as the only node of current_degree
			roots_of_degree[current_degree] = current_node;

			// current_node_ref = current_node_ref->next;
			//printf("******** VALEUR NEXT CURRENT NODE : %x\n", current_node->next);
			current_node = next_node;

		} while (current_node != start_node);
	}

	// The Fibonacci heap is reconstructed
	fibo_heap->min_element = NULL;
	fibo_heap->degree = 0;

	for (unsigned int i = 0; i <= fibo_heap->nb_nodes; i++)
		if (roots_of_degree[i] == NULL)
		{
			setSubHeapAsRoot(fibo_heap, roots_of_degree[i]);
			if (fibo_heap->min_element == NULL
			||  fibo_heap->min_element->value > roots_of_degree[i]->value)
				fibo_heap->min_element = roots_of_degree[i];
		}
/*
	// Finally, the minimum element is updated
	//fibo_heap->min_element = min_element;
	for (unsigned int i = 0; i <= fibo_heap->degree; i++)
	{
		if (roots_of_degree[i] == NULL) continue;
		if (fibo_heap->min_element->value > roots_of_degree[i]->value)
				fibo_heap->min_element = roots_of_degree[i];
	}
*/
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

	printf("\n\n---------- Min element about to be extracted ----------\n");
	// printFiboHeap(fibo_heap);

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
/*
			//extractNodeFromList(current_child);
			printf("Après extraction, noeud parent (%x) : %d fils\n", (int) min_element, min_element->degree);
			printf("Parent noeud actuel : %x\n", (int) current_child->father);

			printf("cur : %x ; next : %x\n", (int) current_child, (int) next_child);
*/			current_child = next_child;
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
