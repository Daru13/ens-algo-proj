//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//            Usual fonctions graph
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graph.h"

// First : Fonctions for List

List createList () // O(1)
{
	List res = {NULL , NULL};
	return res;
}

bool IsEmptyList (List L) // O(1)
//the empty List is List with val = NULL
{
	int* value = L.val;
	return (value == NULL);
}


List addElt (List L,int x) // O(1)
{
	int* value;
	List* nxt ;
	value = &x;
	nxt->val = L.val;
	nxt->next= L.next;
	List res = {value, nxt};
	return res;
}

void ConcatList (List L1 , List L2) // O(#L2) 
{
	while ( !( IsEmptyList (L2)))
	{
		L1 = addElt(L1,*L2.val);
		L2 = *L2.next;
	}
}

// Then : Functions for PrioList

PrioList createPrioList () // O(1)
{
	PrioList res = {NULL , NULL};
	return res;
}

bool IsEmptyPrioList (PrioList P) // O(1)
{
	Prio* p = P.val;
	return (p == NULL);
}

Prio ExtractMin (PrioList P) // O(#P)
{
	if ( IsEmptyPrioList (*P.next))
	{
		return *P.val;
	}
	else
	{
		PrioList P2 = *P.next;
		Prio min = ExtractMin (P2);
		if (min.key < (*P.val).key )
		{
			P.next = &P2;
			return min;
		}
		else
		{
			P = *P.next;
			return *P.val;
		}
	}
}

void AddPrioList (PrioList P, Prio x ) // O(1)
{
	PrioList* P2 = &P;
	Prio* p = &x;
	P.next = P2;
	P.val = p;
}

// Functions for EdgeList

bool EListIsEmpty (EList EL)
{
	edge* e = EL.val;
	return (e == NULL);
}

// Then : Functions for Graph

bool IsConnexe (node graph[], int n) // O(#A) where G=(S,A)
{
	List waiting = createList();
	waiting = addElt ( waiting , 0);
	bool seen[n];
	for (int i = 0 ; i < n ; i++)
	{
		seen[i]= false;
	};
	while ( !(IsEmptyList (waiting) ) )
	{
		node nd = graph[ *waiting.val];
		waiting = *waiting.next;
		if ( !seen[n])
		{
			seen[n] = true;
			EList EL = *nd.links;
			while ( !(EListIsEmpty (EL)))
			{
				edge e = *EL.val;
				waiting = addElt (waiting, e.linked);
				EL = *EL.next;
			};

		};
	};
	bool res = true;
	for (int i=0; i<n ; i++)
	{
		res = res && (seen[i]);
	};
	return res;
}


int* DijKstraNaif (node Graph[] ,int n ,int s) // O(#A²)
{
	int res[n];
	bool seen[n];
	for (int i = 0 ; i < n ; i++)
	{
		seen[i]= false;
	};
	PrioList F = createPrioList ();
	Prio p = { 0 , 0};
	AddPrioList (F , p);
	while ( ! (IsEmptyPrioList (F)))
	{
		Prio x = ExtractMin (F);
		if ( !seen[x.elt] )
		{
			int n = x.elt;
			res[n] = x.key;
			seen[n] = true;
			EList* EL = Graph[n].links;
			
			while (! EListIsEmpty (*EL))
			{
				edge ed = *EL->val;
				Prio p = {ed.linked , res[n] + ed.dist};
				AddPrioList (F,p);
				EL = EL->next;
			}

		}
		
	}
	return res;
}







