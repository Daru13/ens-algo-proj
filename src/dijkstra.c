//------------------------------------------------------------------------------
// DIJKSTRA'S ALGORITHM
//------------------------------------------------------------------------------
// Implementation of Dijkstra's algorithm, in two versions:
// - a naive one with a basic priority structure
// - an optimized one with a Fibonacci heap
//------------------------------------------------------------------------------

/*
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
	addElementToPrioList (F , p);
	while ( ! (prioListIsEmpty(F)))
	{
		Prio x = extractMinFromPrioList(F);
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
				addElementToPrioList(F,p);
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

*/
