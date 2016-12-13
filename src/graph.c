//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//            Usual fonctions graph
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graph.h"

// First : Fonctions for List

List* createList () // O(1)
{
	List* res = malloc(sizeof(List));
	res->first = NULL;
	return res;
}

bool isEmptyList (List* L) // O(1)
//the empty List is List with first = NULL
{
	return (L->first == NULL);
}


void addElt (List* L,int x) // O(1)
{
	ListElt* new_first = malloc(sizeof(ListElt));
	new_first->val = x;
	new_first->next = L->first;
	L->first = &new_first;
}

int popList (List* L) // O(1)
{
	ListElt* elt = L->first;
	(L->first) = (elt->next);
	return elt->val;
}

void ConcatList (List* L1 , List* L2) // O(|L2|) 
{
	while ( !( isEmptyList (L2)))
	{
		int elt = popList(L2);
		addElt(L1 , elt);
	}
}

// Then : Functions for PrioList i.e. Dump prioririty structures

PrioList* createPrioList () // O(1)
{
	PrioList* res = malloc(sizeof(PrioList));
	res->first = NULL;
	return res;
}

bool isEmptyPrioList (PrioList* P) // O(1)
{
	return (P->first == NULL);
}


void addPrioList (PrioList* P, Prio x ) // O(1)
{
	PrioListElt* new_elt = malloc(sizeof(PrioListElt));
	new_elt->val = &x;
	new_elt->next = P->first;
	P->first = new_elt;
}


Prio extractMin (PrioList* P) // O(|P|)
{
	PrioListElt* min = malloc(sizeof(PrioListElt));
	min = P->first;
	P->first = min->next;
	if (isEmptyPrioList( P))
	{
		return *min->val;
	}
	else
	{
		Prio min2 = extractMin(P);
		if (min2.key <= min->val->key)
		{
			addPrioList (P,min2);
			return *min->val;
		}
		else
		{
			addPrioList (P, *min->val);
			return min2;
		}
	}
}


// Functions for EdgeList

bool EListIsEmpty (EList* EL)
{
	return (EL->first == NULL);
}

Edge popEList (EList* EL)
{
	EListElt* elt = EL->first;
	EL->first = elt->next;
	return *elt->val;
}

void addEList (EList* EL, Edge ed)
{
	EListElt* new_elt = malloc(sizeof(EListElt));
	new_elt-> val = &ed;
	new_elt->next = EL->first;
	EL->first = new_elt;
}


// Then : Functions for Graph

bool isConnexe (Graph g ) // O(#A) where G=(S,A)
{
	List* waiting = createList();
	addElt ( waiting , 0);
	bool seen[g.CardV];
	for (int i = 0 ; i < g.CardV ; i++)
	{
		seen[i]= false;
	}
	while ( !(isEmptyList (waiting) ) )
	{
		int nd = popList(waiting) ;
		if ( !seen[nd])
		{
			seen[nd] = true;
			EList* EL = (g.edges[nd]).links;
			while ( !(EListIsEmpty (EL)))
			{
				Edge edge = popEList(EL);
				addElt (waiting, edge.linked);
			}

		}
	}
	bool res = true;
	for (int i=0; i<g.CardV ; i++)
	{
		res = res && (seen[i]);
	}
	return res;
}

int* dijkstraNaif (Graph g ,int s) // O(#A²)
{
	int res[g.CardV];
	bool seen[g.CardV];
	for (int i = 0 ; i < g.CardV ; i++)
	{
		seen[i]= false;
	}
	PrioList* F = createPrioList();
	Prio p = { s , 0};
	addPrioList (F , p);
	while ( ! (isEmptyPrioList(F)))
	{
		Prio x = extractMin(F);
		if ( !seen[x.elt] )
		{
			int n = x.elt;
			res[n] = x.key;
			seen[n] = true;
			EList* EL = (g.edges[n]).links;
			while (! EListIsEmpty(EL))
			{
				Edge ed = popEList(EL);
				Prio p = {ed.linked , res[n] + ed.dist};
				addPrioList(F,p);
			}
		}
	}
	return res;
}



int main(int argc, char *argv[])
{
	int cardV, cardE;
	scanf("%d %d", &cardV, &cardE);
	Graph g;
	g.CardV = cardV;
	g.edges = malloc(cardV*sizeof(Vertex));
	for (int i = 0; i <g.CardV; i++)
	{
		(g.edges[i]).val_node = 1;
		(g.edges[i]).links = NULL;
	}
	for (int i = 0; i < cardE; i++)
	{
		int x, y, d;
		scanf ("%d %d %d", &x, &y, &d);
		Edge* edge_x = malloc(sizeof(Edge));
		Edge* edge_y = malloc(sizeof(Edge));
		edge_x->linked = y;
		edge_y->linked = x;
		edge_x->dist = d;
		edge_x->dist = d;
		addEList((g.edges[x]).links,*edge_x);
		addEList((g.edges[y]).links,*edge_y);
	}
	if (isConnexe(g))
	{
		int* res = dijkstraNaif(g,0);
		for (int i= 0; i < cardV; i++)
		{
			printf("%d : %d",i ,res[i]);
		}
	}	
	free(g.edges);
	return 0;
}



