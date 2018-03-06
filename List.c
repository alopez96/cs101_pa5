//Arturo Lopez
//cs101,pa5
//List.c
//implementation file

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include "List.h"

//structs
//private NodeObj type
typedef struct NodeObj{
	int data;
	struct NodeObj* next;
	struct NodeObj* previous;
}	NodeObj;

//private Node type
typedef NodeObj* Node;

//private ListObj* type
typedef struct ListObj{
	Node front;
	Node back;
	Node cursor;
	int length;
	int index;
}ListObj;

//constructors-Destructors ---------------------------------------------
//new Node constructor
Node newNode(int data){
	Node N = malloc(sizeof(NodeObj));
	N->data = data;
	N->next = N->previous = NULL;
	return N;
}
//free Node
void freeNode(Node* pN){
	if(pN != NULL && *pN != NULL){
		free(*pN);
		*pN = NULL;
	}
}

List newList(void){
	List L;
	L = malloc(sizeof(ListObj));
	L->front = L->back = L->cursor = NULL;
	L->length = 0;
	L->index = -1;
	return L;
}

// Frees all heap memory associated with List *pL, and sets *pL to NULL.S
void freeList(List* pL){
	if(pL!=NULL && *pL!=NULL){
		clear(*pL);
		free (*pL);
		*pL = NULL;
	}
}

//Access functions--------------------------------------------------
//returns the length of L
int length(List L){
	if(L == NULL){
		printf("error: calling length() on NULL List reference\n");
		exit(1);
	}
	return(L->length);
}

// If cursor is defined, returns the index of the cursor element,
// otherwise returns -1.
int index(List L){
	if(L->cursor == NULL){
		return -1;
	}	
	return(L->index);
}

//Pre: length()>0
int front(List L){
	if(L == NULL){
		printf("List error: calling front() on NULL List reference\n");
		exit(1);
	}
	if(L->length <= 0){
		printf("error: calling front() when length is not greater than 0");
		exit(1);
	}
	return (L->front->data);
}


//Pre: length()>0
int back(List L){
	if(L == NULL){
		printf("List error: calling back() on NULL List reference\n");
		exit(1);
	}
	if(L->length <= 0){
		printf("error: calling back() when length is not greater than 0");
		exit(1);
	}
	return (L->back->data);
}


//Pre: length()>0, index()>=0
int get(List L){
	if(L == NULL){
		printf("List error: calling get() on NULL List reference\n");
		exit(1);
	}
	if(L->cursor == NULL){
		printf("error: calling get() when length is not greater than 0\n");	
		printf("and index is not greater than or equal to 0\n");
		exit(1);
	}
	return (L->cursor->data);
}

//returns 1 if List A and B are identical, otherwise 0
int equals(List A, List B){
	int eq = 0;
	if(A == NULL || B == NULL){	
		printf("error: calling equals() with either A or B with NULL reference\n");
		exit(1);
	}
	eq = (A->length == B->length);
	Node a = A->front;
	Node b = B->front;
	while(eq && a!=NULL){
		eq = a->data == b->data;
		a = a->next;
		b = b->next;
	}	
	return eq;
}


//Manipulation procedures -----------------------------------------------
//clear() resets the List to its original empty state
void clear(List L){
	//create a loop and free every node
	while(L->length != 0){
		deleteFront(L);
	}
}

//If List is non-empty, places the cursor under the front element,
//otherwise does nothing.
void moveFront(List L){
	if(L == NULL){
		printf("List error: calling moveFront() on NULL List reference\n");
		exit(1);
	}
	L->cursor = L->front;
	L->index = 0;
}

//If List is non-empty, places the cursor under the back element,
//otherwise does nothing.
void moveBack(List L){
	if(L == NULL){
		printf("List error: calling moveBack() on NULL List reference\n");
		exit(1);
	}
	L->cursor = L->back;
	L->index = L->length - 1;
}

// If cursor is defined and not at front, moves cursor one step toward
// front of this List, if cursor is defined and at front, cursor becomes
// undefined, if cursor is undefined does nothing.
void movePrev(List L){
	if(L == NULL){
		printf("Lidt error: calling movePrev() on NULL List reference\n");
		exit(1);
	} 
	if(L->cursor != NULL){
		if(L->length > 0 && L->cursor != L->front){
			L->cursor = L->cursor->previous;
			L->index--;
		}	
		else if(L->cursor != NULL && L->cursor == L->front){
			L->cursor = NULL;
			L->index = -1;
		}
	}
}

// If cursor is defined and not at back, moves cursor one step toward
// back of this List, if cursor is defined and at back, cursor becomes
// undefined, if cursor is undefined does nothing.
void moveNext(List L){
	if(L == NULL){
		printf("List error: calling moveNext() on NULL List reference\n");
		exit(1);
	}
	if(L->cursor != NULL){
		if(L->length > 0 && L->cursor != L->back){
			L->cursor = L->cursor->next;
			L->index++;
		} else if(L->cursor == L->back){
			L->cursor = NULL;
			L->index = -1;
		}
	}
}

// Insert new element into this List. If List is non-empty,
// insertion takes place after back element.
void append(List L, int data){
	Node N = newNode(data);
	if(L == NULL){
		printf("List Error: calling append() on NULL List reference\n");
		exit(1); 
	}
	if(L->length == 0){
		L->front = L->back = N;
	}else{
		N->previous = L->back;
		L->back->next = N;
		L->back = N;
	}
	L->length++;
}


// Insert new element into this List. If List is non-empty,
// insertion takes place before front element.
void prepend(List L, int data){
	Node N = newNode(data);
	if(L->length == 0){
		L->front = L->back  = N;
	}else{	
		N->next = L->front;
		L->front->previous = N;
		L->front = N;
		N->previous = NULL;
		if(L->index != -1){	
			L->index++;
		}
	}	
	L->length++;
}

// Insert new element before cursor.
// Pre: length()>0, index()>=0
void insertBefore(List L, int data){
	if(L == NULL){
		printf("List error: calling insertBefore() on NULL List reference\n");
		exit(1);
	}
	if(L->length <= 0 && L->index < 0){
		printf("List error: calling insertBefore with length not greater than 0\n");
		printf("or index not greater than or equal to 0\n");
		exit(1);
	}
	if(L->front == L->cursor){
		prepend(L,data);
		return;
	}else{	
		Node A = newNode(data);
		A->next = L->cursor;
		A->previous = L->cursor->previous;
		L->cursor->previous->next = A;
		L->cursor->previous = A;
		L->length++;
		L->index++;
	}
}

// Insert new element after cursor.
// Pre: length()>0, index()>=0
void insertAfter(List L, int data){
	if(L->length <= 0 && L->index < 0){
		printf("List error: calling insertBefore with length not greater than 0\n");
		printf("or index not greater than or equal to 0\n");
		exit(1);
	}
	if(L->back == L->cursor || L->length == 1){
		append(L,data);
		return;
	}
	else{	
		Node A = newNode(data);
		A->previous = L->cursor; 
		A->next = L->cursor->next;
		L->cursor->next->previous = A->next;
		L->cursor->next = A;
		L->length++;
	}
}

// Deletes the front element. Pre: length()>0
void deleteFront(List L){
	Node N = NULL;
	if(L == NULL){
		printf("error: deleteFront() L set to NULL\n");
		exit(1);
	}
	if(L->length <= 0){
		printf("List error: deleteFront() called with empty length\n");
		exit(1);
	}	
	N = L->front;
	if(L->cursor == L->front){
		L->front = L->front->next;
		L->cursor = NULL;
		L->index = -1;
		L->length--;
	}else{
		L->front = L->front->next;
		L->index--;
		L->length--;
	}
	freeNode(&N);
}

// Deletes the back element. Pre: length()>0
void deleteBack(List L){
	Node N = L->back;
	if(L == NULL){
		printf("error: deleteBack() List L set to NULL\n");
		exit(1);
	}
	if (L->length <= 0){
		printf("List error: deleteFront() called with length !> 0\n");
		exit(1);
	}
	if(length(L) > 1){
		if(L->cursor == L->back){
			L->back = L->back->previous;
			L->back ->next ->previous = NULL;
			L->back->next = NULL;
			L->cursor = NULL;
			L->index = -1;
		}else{
			L->back = L->back->previous;
			L->back->next = NULL;
		}
	}else{
		if(L->cursor != NULL){
			L->cursor = NULL;
			L->index = -1;
		}
		L->back = L->front =NULL;
	}
	L->length--;
	freeNode(&N);
}

// Deletes cursor element, making cursor undefined.
// Pre: length()>0, index()>=0
void delete(List L){
	Node A = NULL;
	if(L == NULL){
		printf("List error: calling delete() on NULL List reference\n");
	}
	if(L->cursor == L->front){
		deleteFront(L);
		return;
	}	
	if(L->cursor == L->back){
		deleteBack(L);
		return;
	}
	if(L->cursor == NULL){
		//printf("delete() called and cursor set to NULL\n");
		//exit(1);
		return;
	}
	if(L->length <= 0 && L->index < 0){
		printf("List error: delete() called with length !> 0\n");
		printf("List error: deleteFront() called with length !> 0\n");
		exit(1);

	}else{
		A = L->cursor;
		L->cursor->previous->next = L->cursor->next;
		L->cursor->next->previous = L->cursor->previous;
		L->cursor = NULL;
		L->index = -1;
		L->length--;
	}
	freeNode(&A);
}

// Other functions ------------------------------------------------------------
void printList(FILE*out, List L)
{
	Node N = NULL;
	if(L == NULL){
		printf("List Error: calling printList() on NULL List reference\n");
		exit(1);
	}
	for(N = L->front; N != NULL; N = N->next){
		if(N == L->front){
			fprintf(out, "%d", N->data);
			continue;
		}
		fprintf(out, " %d", N->data);
	}
}


List copyList(List L){
	List A = newList();
	Node B = L->front;
	if(L->length > 0){
		while(B != NULL){
			append(A, B->data);
			B = B->next;
		}
	}
	return A;
}


