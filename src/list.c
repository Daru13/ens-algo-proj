//------------------------------------------------------------------------------
// LINKED & PRIORITY LISTS
//------------------------------------------------------------------------------
// Implementation of two kinds of linked lists:
// - basic linked lists
// - simple priority (linked) lists
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "toolbox.h"
#include "list.h"

//------------------------------------------------------------------------------
// Linked list
//------------------------------------------------------------------------------

List* createList () // O(1)
{
	List* res = malloc(sizeof(List));
	CHECK_MALLOC(res);

	res->first = NULL;
	return res;
}

bool listIsEmpty (List* L) // O(1)
//the empty List is List with first = NULL
{
	return (L->first == NULL);
}


void addElementToList (List* L, int x) // O(1)
{
	ListElt* new_first = malloc(sizeof(ListElt));
	CHECK_MALLOC(new_first);

	new_first->val  = x;
	new_first->next = L->first;
	L->first = new_first;
}

int popFromList (List* L) // O(1)
{
	ListElt* elt = L->first;
	L->first 	 = elt->next;

	return elt->val;
}

void concatTwoLists (List* L1 , List* L2) // O(|L2|) 
{
	while (! listIsEmpty (L2))
	{
		int elt = popFromList(L2);
		addElementToList(L1, elt);
	}
}

//------------------------------------------------------------------------------
// Basic (linked) priority list
//------------------------------------------------------------------------------

/*
PrioList* createPrioList () // O(1)
{
	PrioList* res = malloc(sizeof(PrioList));
	CHECK_MALLOC(res);

	res->first = NULL;
	return res;
}

bool prioListIsEmpty (PrioList* P) // O(1)
{
	return (P->first == NULL);
}

void addElementToPrioList (PrioList* P, Prio x ) // O(1)
{
	PrioListElt* new_elt = malloc(sizeof(PrioListElt));
	CHECK_MALLOC(new_elt);

	new_elt->val = &x;
	new_elt->next = P->first;
	P->first = new_elt;
}

Prio extractMinFromPrioList (PrioList* P) // O(|P|)
{
	PrioListElt* min = malloc(sizeof(PrioListElt));
	CHECK_MALLOC(min);

	min = P->first;
	P->first = min->next;
	if (prioListIsEmpty( P))
	{
		return *min->val;
	}
	else
	{
		Prio min2 = extractMinFromPrioList(P);
		if (min2.key <= min->val->key)
		{
			addElementToPrioList(P,min2);
			return *min->val;
		}
		else
		{
			addElementToPrioList(P, *min->val);
			return min2;
		}
	}
}
*/
