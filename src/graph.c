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
	List res = {NULL};
	return &res;
}

bool isEmptyList (List* L) // O(1)
//the empty List is List with first = NULL
{
	return (L<-first == NULL);
}


void addElt (List* L,int x) // O(1)
{
	ListElt new_first = {x , L<-first};
	L<-first = &new_first;
}

ListElt popList (List* L) // O(1)
{
	ListElt elt = (L<-first)*;
	L<-first = elt.next;
	return elt;
}

void ConcatList (List* L1 , List* L2) // O(|L2|) 
{
	while ( !( IsEmptyList (L2)))
	{
		ListElt elt = popList(L2);
		addElt(L1 , elt.val);
	}
}

// Then : Functions for PrioList i.e. Dump prioririty structures

PrioList* createPrioList () // O(1)
{
	PrioList res = {NULL};
	return &res;
}

bool isEmptyPrioList (PrioList* P) // O(1)
{
	return (P<-first == NULL);
}


void AddPrioList (PrioList* P, Prio x ) // O(1)
{
	PrioListElt new_elt = {&x, P<-first};
	P<-first = &new_elt;
}


Prio ExtractMin (PrioList* P) // O(|P|)
{
	Prio min = P<-first;
	P<-first = Min.next;
	if (isEmptyPrioList( P))
	{
		return *min;
	}
	else
	{
		Prio min2 = ExtractMin(P);
		if (min2<-key <= min<-key)
		{
			addPrioList (P,min2);
			return *min;
		}
		else
		{
			addPrioList (P, min);
			return *min2;
		}
}


// Functions for EdgeList

bool EListIsEmpty (EList* EL)
{
	return (EL<-first == NULL);
}

Edge popEList (EList* EL)
{
	EListElt elt = EL<-first;
	EL<-first = elt.next;
	return elt.val;
}

void addEList (EList* EL, edge ed)
{
	EListElt new_elt = {&ed, EL<-first};
	EL<-first = &new_elt;
}


// Then : Functions for Graph

bool IsConnexe (Graph g, int n) // O(#A) where G=(S,A)
{
	List* waiting = createList();
	waiting = addElt ( waiting , 0);
	bool seen[n];
	for (int i = 0 ; i < n ; i++)
	{
		seen[i]= false;
	};
	while ( !(IsEmptyList (waiting) ) )
	{
		int nd = popList(waiting) ;
		if ( !seen[nd])
		{
			seen[nd] = true;
			EList EL = g.edges[nd];
			while ( !(EListIsEmpty (EL)))
			{
				EListELt edge = popEltList(EL);
				addList (waiting, edge<-linked);
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

int* DijKstraNaif (Graph g ,int s) // O(#A²)
{
	int res[g.cardV];
	bool seen[g.cardV];
	for (int i = 0 ; i < n ; i++)
	{
		seen[i]= false;
	};
	PrioList* F = createPrioList();
	Prio p = { s , 0};
	AddPrioList (F , p);
	while ( ! (IsEmptyPrioList(F)))
	{
		Prio x = ExtractMin(F);
		if ( !seen[x.elt] )
		{
			int n = x.elt;
			res[n] = x.key;
			seen[n] = true;
			EList EL = (g.edges[n]).links			
			while (! EListIsEmpty(EL))
			{
				Edge ed = popEList(EL);
				Prio p = {ed.linked , res[n] + ed.dist};
				AddPrioList(F,p);
			};
		};
	};
	return res;
}



int main()
{
	int cardV, cardE;
	scanf("%d %d", &cardV, &cardE);
	Graph g;
	g.CardV = cardV;
	g.edges = (struct Vertex*) malloc(cardV*sizeof(struct Vertex));
	for (int i = 0; i < cardE, i++)
	{
		int x, y, d;
		scanf ("%d %d %d", &x, &y, &d);
		Edge edge_x = {y,d};
		Edge edge_y = {x,d};
		addEList(g.adges[x],edge_x);
		addEList(g.egdes[y],edge_y);
	};
	if isCoonnexe(g)
	{
		int* res = dijkstrNaif(g,0);
		for (int i= 0, i< cardV, i++)
		{
			printf("%d : %d",i ,res[i];
		};
	};
	free(g.edges);
	return 0;
}



