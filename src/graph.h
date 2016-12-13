// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%			
//               GRAPH STRUCTURES
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%			


// First : chained list
 
typedef struct ListElt 
{
	int val;
	struct ListElt* next;
}ListElt; 

typedef struct List
{
	struct ListELt* first;
}List;


// Priority List


typedef struct Prio
{
	int elt;
	int key;
}Prio;

typedef struct PrioListElt
{
	struct Prio* val;
	struct PrioListElt* next;

}PrioListElt;

typedef struct PrioList
{
	struct PrioListElt* first;
}PrioList;

// Then : Graph


typedef struct Edge
{
	int linked;
	int dist;
}Edge;

typedef struct EListElt
{
	struct Edge* val;
	struct EListElt* next;
}EListElt;

typedef struct EList
{
	struct EListElt* first;
}EList ;


typedef struct Vertex
{
	int val_node ;
	struct EList* links ;
}Vertex;

typedef struct Graph
{
	struct Vertex* edges;
	int CardV;
}Graph;


// Finally : a graph is an array of node :
// graph[n] = node ; such as node.val_node = n
