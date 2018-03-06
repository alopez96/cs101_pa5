//Arturo Lopez
//cs101, pa5
//FindComponents.c

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Graph.h"

int main(int argc, char* argv[]){

	FILE *in, *out;

	if(argc != 3){
		printf("error argc != 3\n");
		printf("Usage: %s <input file> <outputfile>\n", argv[0]);
		exit(1);
	}
	//open files for reading and writing
	in = fopen(argv[1], "r");
	out = fopen(argv[2], "w");
	if(in == NULL){
		printf("Unable to open file %s for reading\n", argv[1]);
		exit(1);
	}
	if(out == NULL){
		printf("unable to open file %s for writing\n", argv[2]);
		exit(1);
	}

	int n = 0;
	fscanf(in, "%d", &n);
	Graph G = newGraph(n);
	int u = 0, v = 0;
	while (fgetc(in) != EOF)
	{  
		fscanf (in, "%d %d", &u, &v);      
		if(u != 0){
			addArc(G,u,v);
		}
	}
	fprintf(out, "Adjacency list representation of G:\n");
	printGraph(out, G);
	fprintf(out, "\n");
	List S = newList();
	for(int i = 1; i <= getOrder(G); i++){
		append(S,i);
	}
	DFS(G,S);
	Graph T  = transpose(G);
	DFS(T,S);				//List S is now sorted in decreasing finish time
	List C = newList();
	if(length(S) > 0){
		int j = 0;
		moveBack(S);
		while(index(S) != -1){
			if(getParent(T,get(S)) == NIL){
				j++;
			}
			movePrev(S);
		}
		fprintf(out, "G contains %d strongly connected components:\n", j);
		j = 0;
		moveBack(S);
		while(index(S) != -1){
			prepend(C,get(S));
			if(getParent(T,get(S)) == NIL){
				j++;
				fprintf(out, "Component %d: ", j);
				printList(out, C);
				fprintf(out, "\n");	
				clear(C);
			}
			movePrev(S);
		}
	}
	//free memory
	freeGraph(&G);
	freeGraph(&T);
	freeList(&S);
	freeList(&C);
	//close files
	fclose(in);
	fclose(out);

	return(0);
}
