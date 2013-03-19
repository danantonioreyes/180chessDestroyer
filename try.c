#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int global = -1;

typedef struct vertex VERTEX;
struct vertex
{
	int depth;
	VERTEX* parent;
	VERTEX* children[1];
};

typedef struct node NODE;
struct node
{
	VERTEX* v; 	
	NODE* next;
};
void getParentMove(VERTEX* arg) {
	VERTEX* alpha = arg;
	while(alpha->parent != NULL) {
		alpha = arg->parent;
	}
	printf("Parent depth %d\n", alpha->depth);
}

void expansion(VERTEX* arg) {
	int i;
	for(i=0; i<1; i++) {
		arg->children[i] = (VERTEX*)malloc(sizeof(VERTEX));
		arg->children[i]->depth = arg->depth + 1;
		arg->children[i]->parent = arg;
		//printf("Generating child %d parent depth %d\n", arg->children[i]->depth, arg->children[i]->parent->depth);
		getParentMove(arg->children[i]);
	}
}

void insert(NODE* list, VERTEX* arg) {
	NODE* temp = (NODE*)malloc(sizeof(NODE));
	temp->v = arg;
	temp->next = NULL;
	
	NODE* alpha = list;
	while(alpha->next != NULL) alpha = alpha->next;
	alpha->next = temp;
}



void main() {

	VERTEX* root = (VERTEX*)malloc(sizeof(VERTEX));
	root->depth = 0;
	root->parent = NULL;
	
	NODE* expansionQ = (NODE*)malloc(sizeof(NODE));
	expansionQ->v = root;
	expansionQ->next = NULL;

	int i;
	expansion(root);
	for(i=0; i<1; i++) insert(expansionQ, root->children[i]);
	expansionQ = expansionQ->next;
	
	VERTEX* alpha;
	NODE* beta;

	while(expansionQ->v->depth < 69)
	{

		alpha = expansionQ->v;
		expansion(alpha);
		for(i=0; i<1; i++) {
			insert(expansionQ, alpha->children[i]);
		}
		expansionQ = expansionQ->next;
	}
	

}