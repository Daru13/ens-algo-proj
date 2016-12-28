//------------------------------------------------------------------------------
// COMPLEXITY
//------------------------------------------------------------------------------

#ifndef _COMPLEXITY_H_
#define _COMPLEXITY_H_

#include "list.h"
#include "graph.h"
#include "fibonacci_heap.h"
#include "dijkstra.h"

// Global variable which should be used as an accumulator for complexity computation
extern unsigned int COMPLEXITY;

// Functions from file "list.c"
int ComplexityOf_createList ();
int ComplexityOf_listIsEmpty ();
int ComplexityOf_addElementToList ();
int ComplexityOf_popFromList ();
int ComplexityOf_concatTwoLists (List* L1, List* L2);


// Functions from file "graph.c"
int ComplexityOf_createEdge ();
int ComplexityOf_createEmptyGraph ();
int ComplexityOf_addUndirectedEdgeToGraph ();
int ComplexityOf_createGraphFromFile ();
int ComplexityOf_graphIsConnected (Graph* g);

// Functions from file "fibonacci_heap.c"
int ComplexityOf_createNode (NodeValue value, int key);
int ComplexityOf_createIsolatedNode (NodeValue value, int key);
int ComplexityOf_freeNode (Node* node);
int ComplexityOf_freeNodeTree (Node* root);
int ComplexityOf_printNodeDetails (Node* node);
int ComplexityOf_printListOfNodes (Node* node);
int ComplexityOf_createFiboHeap ();
int ComplexityOf_freeFiboHeap (FiboHeap* fibo_heap);
int ComplexityOf_printFiboHeap (FiboHeap* fibo_heap);
int ComplexityOf_extractNodeFromList (Node* node);
int ComplexityOf_insertNodeInList (Node* node_to_insert, Node* destination);
int ComplexityOf_mergeListsOfNodes (Node* to_merge, Node* destination);
int ComplexityOf_insertNodeAsChild (Node* child, Node* father);
int ComplexityOf_insertRootInFiboHeap (FiboHeap* fibo_heap, Node* new_root);


// Functions from file "dijkstra.c"
int ComplexityOf_extractMinimumNaive (bool* seen, int* lengths, int nb_vertexes);
int ComplexityOf_dijkstraNaive (Graph* g, int s);

#endif
