//------------------------------------------------------------------------------
// GRAPH
//------------------------------------------------------------------------------

typedef struct Edge
{
	int origin;
	int destination;
	int weight;
	struct Edge* next;
} Edge;

typedef struct Graph
{
	Edge** edges;
	int nb_vertexes;
} Graph;

//------------------------------------------------------------------------------

Edge* createEdge (int origin, int destination, int weight, Edge* next);
Graph* createEmptyGraph (int nb_vertexes);
void addUndirectedEdgeToGraph (Graph* graph, int source, int destination, int weight);
Graph* createGraphFromFile (FILE* file);
bool graphIsConnected (Graph* graph);
