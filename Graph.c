//Arturo Lopez
//cs101,pa5
//Graph.c

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include "Graph.h"

//structs
//private GraphObj type
typedef struct GraphObj{
	List* neighbors;
	char* color;			//white, gray, or black
	int* parent;			//parent of vertex i
	int* discoverTime;		//discover time of vertex u
	int* finishTime;		//discover time of vertex u
	int order;			//number of vertices
	int size;			//number of edges
	int time;
} GraphObj;

void visit(Graph G, List S, int x);

//Cconstructor- Desctructor
//newGraph() will return a reference to a new graph object containing n vertices and no edges. 
Graph newGraph(int n){
	if(n < 1){
		printf("error: calling newGraph() with n < 1\n");
		exit(1);
	}
	Graph G = malloc(sizeof(GraphObj));
	G->neighbors = calloc(n+1, sizeof(List));
	G->color = calloc(n+1, sizeof(char));
	G->parent = calloc(n+1, sizeof(int));
	G->discoverTime = calloc(n+1, sizeof(int));
	G->finishTime = calloc(n+1, sizeof(int));
	G->order = n;					//order- number of vetices
	G->size = 0;					//size- number of edges
	G->time = 0;
	for(int i = 1; i <= n; i++){
		G->neighbors[i] = newList();
		G->color[i] = 'w';
		G->parent[i] = NIL;
		G->discoverTime[i] = UNDEF;
		G->finishTime[i] = UNDEF;
	}
	return G;
}

//freeGraph() frees all heap memory and sets Graph argument to NULL
void freeGraph(Graph* pG){
	//free all lists
	if(pG != NULL && *pG != NULL){
		for(int i = 1; i <= (*pG)->order; i++){
			freeList(&((*pG)->neighbors[i]));
		}
		free((*pG)->neighbors);
		free((*pG)->color);
		free((*pG)->parent);
		free((*pG)->discoverTime);
		free((*pG)->finishTime);
		free(*pG);
		*pG = NULL;
	}
}

/* Access functions */
//getOrder() returns number of vertices
int getOrder(Graph G){
	if(G == NULL){							//check graph is not null
		printf("error: calling getOrder() with NULL reference on Graph");
		exit(1);
	}
	return G->order;	
}

//getSize() returns number of edges
int getSize(Graph G){
	if(G == NULL){
		printf("error: calling getSize() on NULL reference on Graph");
		exit(1);
	}
	return G->size;
}

//getParent() returns the parent of vertex u
/* Pre: 1<=u<=n=getOrder(G) */
int getParent(Graph G, int u){
	if(G == NULL){							//check graph is not null
		printf("error: calling getParent() with NULL reference on Graph");
		exit(1);
	}
	if(!(1 <= u && u <=getOrder(G))){
		printf("error: calling getParent when !(1<=u<=getOrder(G)) ");
		exit(1);
	}
	return G->parent[u];					

} 
//pre: 1<=u<=n=getOrder(G)
int getDiscover(Graph G, int u){
	if(G == NULL){							//check graph is not null
		printf("error: calling getDiscover() with NULL reference on Graph");
		exit(1);
	}
	if(!(1 <= u && u <=getOrder(G))){
		printf("error: calling getDiscover when !(1<=u<=getOrder(G)) ");
		exit(1);
	}
	return G->discoverTime[u];
}

//pre: 1<=u<=n=getOrder(G)
int getFinish(Graph G, int u){
	if(G == NULL){							//check graph is not null
		printf("error: calling getFinish() with NULL reference on Graph");
		exit(1);
	}
	if(!(1 <= u && u <=getOrder(G))){
		printf("error: calling getFinish when !(1<=u<=getOrder(G)) ");
		exit(1);
	}
	return G->finishTime[u];
}

// Manipulation procedures
//Pre: 1<=u<=n, 1<=v<=n
//addArc() inserts a new directed edge from u to v
//i.e. v is added to the adjacency List of u (but not u to the adjacency List of v)
void addArc(Graph G, int u, int v){
	if(!(1 <= u && u <= getOrder(G)) || !(1 <= v && v <= getOrder(G))){
		printf("error: calling addArc when either u or v is not between 1 and getOrder ");
		exit(1);
	}
	if(length(G->neighbors[u]) > 0){
		moveFront(G->neighbors[u]);				//move adjacency Lists, of u,to the front
		while(index(G->neighbors[u]) != -1){
			if(v > back(G->neighbors[u])){
				append(G->neighbors[u],v);
				break;
			}
			else if(v < get(G->neighbors[u])){
				insertBefore(G->neighbors[u], v);
				break;
			}
			else if (v == get(G->neighbors[u])){
				break;
			}
			moveNext(G->neighbors[u]);
		}
	}else{								//length == 0
		append(G->neighbors[u],v);
	}	
	G->size++;
}
//Pre: 1<=u<=n, 1<=v<=n
//addEdge() inserts a new edge joining u to v
// i.e. u is added to the adjacency List of v, and v to the adjacency List of u
void addEdge(Graph G, int u, int v){
	if(!(1 <= u && u <=getOrder(G)) || !(1 <= v && v << getOrder(G))){
		printf("error: calling adEdge when either u or v is not between 1 and getOrder ");
		exit(1);
	}
	addArc(G,u,v);
	addArc(G,v,u);
	G->size--;
}
//Pre: length(S) == getOrder(G)
void DFS(Graph G, List S){
	if(length(S) != getOrder(G)){
		printf("error: calling DFS when length(S) != getOrder(S)");
		exit(1);
	}
	for(int x = 1; x <= length(S); x++){
		G->color[x] = 'w';
		G->parent[x] = NIL;
	}
	G->time = 0;
	List C = copyList(S);
	moveFront(C);
	for(int x = 1; x <= length(S); x++){
		int a = get(C);
		if(G->color[a] == 'w'){
			visit(G, S, a);
		}
		moveNext(C);
	}
	freeList(&C);
}


//visit is private helper function of the Graph ADT
void visit(Graph G, List S, int x){
	G->color[x] = 'g';
	G->discoverTime[x] = ++G->time;
	if(length(G->neighbors[x]) > 0){
		moveFront(G->neighbors[x]);
		while(index(G->neighbors[x]) != -1){
			int y = get(G->neighbors[x]);
			if(G->color[y] == 'w'){
				G->parent[y] = x;
				visit(G,S,y);
			}
		moveNext(G->neighbors[x]);
		}
	}	
	G->color[x] = 'b';
	G->finishTime[x] = ++G->time;
	prepend(S,x);
	deleteBack(S);
}


//Other Functions
//transpose() returns reference to new graph object representing the transpose of G
Graph transpose(Graph G){
	int n = G->order;
	Graph T = newGraph(n);
	for(int u = 1; u <= n; u++){				//increment through each vertex of G
		if(length(G->neighbors[u]) != 0){
			moveFront(G->neighbors[u]);
			while(index(G->neighbors[u]) != -1){
				int v = get(G->neighbors[u]);
				addArc(T,v,u);
				moveNext(G->neighbors[u]);
			}
		}else						//adj List of vertex u is empty, so do nothing
			continue;
	}
	return T;
}

//cpoyGraph() returns reference to new graph object which is a copy of G
Graph copyGraph(Graph G){
	int n = G->order;
	Graph C = newGraph(n);
	for (int u = 1; u <= n; u++){
		if(length(G->neighbors[u]) != 0){
			moveFront(G->neighbors[u]);
			while(index(G->neighbors[u]) != -1){
				int v = get(G->neighbors[u]);
				addArc(C,u,v);
				moveNext(G->neighbors[u]);
			}
		}else
			continue;
	}
	return C;
}

void printGraph(FILE* out , Graph G){
	if(G == NULL){
		printf("error calling printGraph with NULL reference on graph G");
		exit(1);
	}
	for(int i = 1; i <= G->order; i++){
		fprintf(out,"%d: ", i);
		printList(out, G->neighbors[i]);
		fprintf(out, "\n");
	}
}


//test function
void testFunc(Graph G){
	for(int i = 1; i <= G->order; i++){
		//		printf("length of adj list of vertex %d", i);
		//		printf(" : %d \n", length(G->neighbors[i]));
		printf("length of adj list of vertex %d : %d\n", i, length(G->neighbors[i]));
	}
}
