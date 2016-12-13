// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%			
//               GRAPH STRUCTURES
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%			


// First : chained list

typedef struct ListElt ListElt; 
struct ListElt
{
	int val;
	// we use a pointor it recognize the empty List
	ListElt* next;
}; 

typedef struct List List;
struct List
{
	ListELt* first;
};


// Priority List


typedef struct Prio Prio;
struct Prio
{
	int elt;
	int key;
};

typedef struct PrioListElt PrioListElt;
struct PrioListElt
{
	Prio* val;
	PrioListElt* next;

};

typedef struct PrioList PrioList;
struct PrioList
{
	PrioListElt* first;
};

// Then : Graph


typedef struct Edge Edge;
struct Edge 
{
	int linked;
	int dist;
};

typedef struct EListElt EListElt;
struct EListElt
{
	Edge* val;
	EListElt* next;
};

typedef struct EList EList;
struct EList
{
	EListElt* first;
};


typedef struct Vertex Vertex;
struct Vertex
{
	int val_node ;
	EList* links ;
};

typedef struct Graph Graph;
struct Graph 
{
	vertex* edges;
	int CardV;
};


// Finally : a graph is an array of node :
// graph[n] = node ; such as node.val_node = n
