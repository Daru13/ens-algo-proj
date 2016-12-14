//------------------------------------------------------------------------------
// LINKED & PRIORITY LISTS
//------------------------------------------------------------------------------

#ifndef _LIST_H
#define _LIST_H

#include <stdbool.h>

// Linked list
typedef struct ListElt 
{
	int val;
	struct ListElt* next;
} ListElt; 
 
typedef struct List
{
	ListElt* first;
} List;

// Priority List
typedef struct Prio
{
	int elt;
	int key;
} Prio;

typedef struct PrioListElt
{
	Prio* val;
	struct PrioListElt* next;

} PrioListElt;

typedef struct PrioList
{
	PrioListElt* first;
} PrioList;

//------------------------------------------------------------------------------

List* createList ();
bool listIsEmpty (List* L);
void addElementToList (List* L,int x);
int popFromList (List* L);
void concatTwoLists (List* L1 , List* L2);

PrioList* createPrioList ();
bool prioListIsEmpty (PrioList* P);
void addElementToPrioList (PrioList* P, Prio x );
Prio extractMinFromPrioList (PrioList* P);

#endif
