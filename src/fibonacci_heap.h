//------------------------------------------------------------------------------
// FIBONACCI HEAPS
//------------------------------------------------------------------------------

#ifndef _FIBONACCI_HEAPS_H_
#define _FIBONACCI_HEAPS_H_

#include "toolbox.h"

// Type of the value of a node of the Fibonacci heap
typedef int NodeValue;

// Node of a circular double-linked list (node in one of the heaps)
typedef struct Node {
	// Sibling nodes
	struct Node* next;
	struct Node* previous;

	// Parent node
	struct Node* father;

	// One child of the node
	struct Node* child;

	// Value of the node
	NodeValue value;

	// Degree of the node (number of children)
	unsigned int degree;

	// Tag of the node
	bool is_tagged;
} Node;

// Collection of heaps forming a Fibonacci heap (+ additional information)
typedef struct FiboHeap {
	// Root with the smallest value
	// This is sufficient to access the collection of heaps!
	Node* min_element;

	// Degree of the Fibonacci heap (i.e. number of heap roots)
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

bool hasSibling (const Node* node);
bool hasFather (const Node* node);
bool hasChild (const Node* node);
unsigned int getNbNodesOfList (Node* const node);

void extractNodeFromList (Node* node);
void mergeNodeLists (Node* source, Node* destination);
void insertNodeAsChild (Node* child, Node* father);

void insertSingleRootInFiboHeap (FiboHeap* fibo_heap, Node* node);
Node* getMinimumElement (const FiboHeap* fibo_heap);
void linkRootNodes (Node* child, Node* father);
void consolidateFiboHeap (FiboHeap* fibo_heap);

#endif
