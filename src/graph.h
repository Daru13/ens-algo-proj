// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%			
//               GRAPH STRUCTURES
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%			


// First : chained list

typedef struct List List; 
struct List
{
	int* val;
	// we use a pointerr ito recognize the empty List
	List* next;
} ; 

// Priority List


typedef struct Prio Prio;
struct Prio
{
	int elt;
	int key;
};

typedef struct PrioList PrioList;
struct PrioList
{
	Prio* val;
	PrioList* next;

};


// Then : Graph


typedef struct edge edge;
struct edge 
{
	int linked;
	int dist;
};

typedef struct EList EList;
struct EList
{
	edge* val;
	EList* next;
};

typedef struct node node;
struct node
{
	int val_node ;
	EList* links ;
} ;

// Finally : a graph is an array of node :
// graph[n] = node ; such as node.val_node = n
