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

	// Total number of nodes
	unsigned int nb_nodes;

} FiboHeap;

/******************************************************************************/



#endif
