//------------------------------------------------------------------------------
// TOOLBOX
//------------------------------------------------------------------------------

#ifndef _MAIN_TEST_H
#define _MAIN_TEST_H

#include "fibonacci_heap.h"

Node** generateFixedNodesArray (int nb_nodes);
Node** generateRandomNodesArray (int nb_nodes);
void deleteNodesArray (Node** node_array, int size);

int main ();

#endif
