//------------------------------------------------------------------------------
// COMPLEXITY
//------------------------------------------------------------------------------
// Computation of the complexities of almost all "technical" functions.
// A concerned function "f" has an associated function "ComplexityOf_f".
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "list.h"
#include "graph.h"
#include "fibonacci_heap.h"
#include "dijkstra.h"
#include "complexity.h"

//------------------------------------------------------------------------------
// FUNCTIONS FROM FILE "list.c"
//------------------------------------------------------------------------------

int ComplexityOf_createList () { return 4; }
int ComplexityOf_listIsEmpty () { return 2; }
int ComplexityOf_addElementToList () { return 5; }
int ComplexityOf_popFromList () { return 3; }

int ComplexityOf_concatTwoLists (List* L2)
{
	int complexity 		  = 0;
	ListElt* list_element = L2->first;

	while (list_element != NULL)
	{
		complexity += ComplexityOf_listIsEmpty();
		complexity += ComplexityOf_popFromList();
		complexity += 1;
		complexity += ComplexityOf_addElementToList();
		list_element = list_element->next;
	}	

	return complexity;
}

// TODO : la suite :) !

//------------------------------------------------------------------------------
// FUNCTIONS FROM FILE "graph.c"
//------------------------------------------------------------------------------

// TODO :)

//------------------------------------------------------------------------------
// FUNCTIONS FROM FILE "fibonacci_heap.c"
//------------------------------------------------------------------------------

// TODO :)

//------------------------------------------------------------------------------
// FUNCTIONS FROM FILE "dijkstra.c"
//------------------------------------------------------------------------------

// TODO :)
