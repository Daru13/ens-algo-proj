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

Node* createNode (const NodeValue value);
Node* createIsolatedNode (const NodeValue value);
void freeNode (Node* node);
void freeNodeTree (Node* root);

FiboHeap* createFiboHeap ();
void freeFiboHeap (FiboHeap* fibo_heap);
void printFiboHeap (FiboHeap* const fibo_heap);
/*
bool hasSibling (const Node* node);
bool hasFather (const Node* node);
bool hasChild (const Node* node);
unsigned int getNbNodesOfList (Node* const node);
void printCDLL (Node* cdll_node);
*/
void extractNodeFromList (Node* node);
void mergeNodeLists (Node* source, Node* destination);
void insertNodeAsChild (Node* child, Node* father);

void insertSingleRootInFiboHeap (FiboHeap* fibo_heap, Node* node);
void setSubHeapAsRoot (FiboHeap* fibo_heap, Node* sub_root);
void linkRootNodes (FiboHeap* fibo_heap, Node* child, Node* father);
void consolidateFiboHeap (FiboHeap* fibo_heap);
Node* extractMinFromFiboHeap (FiboHeap* fibo_heap);

#endif
