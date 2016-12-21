//------------------------------------------------------------------------------
// FIBONACCI HEAPS
//------------------------------------------------------------------------------

#ifndef _FIBONACCI_HEAPS_H_
#define _FIBONACCI_HEAPS_H_

#include "toolbox.h"
#include <stdbool.h>

// Type of the value of a node of the Fibonacci heap
typedef int NodeValue;

// Node of a circular double-linked list (node in one of the heaps)
typedef struct Node {
	// Related nodes
	struct Node* next;
	struct Node* previous;
	struct Node* father;
	struct Node* child;

	// Key and value
	NodeValue value;
	int 	  key;

	// Degree (number of children)
	unsigned int degree;

	// Tag
	bool is_tagged;
} Node;

// Collection of heaps roots forming a Fibonacci heap
typedef struct FiboHeap {
	// Root with the smallest key of
	Node* min_element;

	// Degree (number of heap roots)
	unsigned int degree;

	// Total number of nodes
	unsigned int nb_nodes;
} FiboHeap;

//------------------------------------------------------------------------------

Node* createNode (NodeValue value, int key);
Node* createIsolatedNode (NodeValue value, int key);
void freeNode (Node* node);
void freeNodeTree (Node* root);
void printNodeDetails (Node* node);
void printListOfNodes (Node* node);

FiboHeap* createFiboHeap ();
void freeFiboHeap (FiboHeap* fibo_heap);
void printFiboHeap (FiboHeap* fibo_heap);

void extractNodeFromList (Node* node);
void insertNodeInList (Node* node_to_insert, Node* destination);
void mergeListsOfNodes (Node* to_merge, Node* destination);
void insertNodeAsChild (Node* child, Node* father);

void insertRootInFiboHeap (FiboHeap* fibo_heap, Node* new_root);
void moveSubHeapToRoot (FiboHeap* fibo_heap, Node* sub_heap);
void linkRootNodes (FiboHeap* fibo_heap, Node* root_child, Node* root_father);

void consolidateFiboHeap (FiboHeap* fibo_heap);
Node* extractMinFromFiboHeap (FiboHeap* fibo_heap);

#endif
