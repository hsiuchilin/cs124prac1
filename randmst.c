#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HEAPEMPTY -1

// heeere's tony's laptop

typedef struct node{
	float val;
	struct node* parent;
	struct node* left;
	struct node* right;
}node;

typedef struct minheap{
	node* root;
	node* bottom;
	int empty;
}minheap;

void swap(node* p, node* c){
	c->parent = p->parent;
	p->parent = c;
	node* tempcleft = c->left;
	node* tempcright = c->right;
	if (p->left == c){
		c->left = p;
		c->right = p->right;
		p->left= tempcleft;
		p->right = tempcright;
	}
	else{
		c->left = p->left;
		c->right= p;
		p->left= tempcleft;
		p->right tempcright;
	}
}

void percolate(minheap* h, node* n){

}

float deletemin(minheap* h){
	if (h->empty){
		return HEAPEMPTY;
	}
	x = h->root->val;
	if (!(h->root->left||h->root->right)){
		free(h->root);
		h->root = NULL;
		h->bottom = NULL;
		h->empty = 1;
	}
	else if (!h->root->left && h->root->right){
		node* temp = h->root->right;
		free(h->root);
		h->root = temp;
	}
	else if (h->root->left && !h->root->right){
		node* temp = h->root->left;
		free(h->root);
		h->root = temp;
	}
	else{
		if (h->root->left->val > h->root->right->val){
			node* temp_left = h->root->left;
			node* temp_right = h->root->right;
			free(h->root);
			h->
		}
	}
}

int main(int argc, char* argv[]){
	if (argc != 4){
		printf("Check number of arguments!\n");
		abort;
	}
	int numpoints = argv[1];
	int numtrials = argv[2];
	int dimension = argv[3];

}