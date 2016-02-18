#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HEAPEMPTY -1
typedef graph_node{
	float x;
	float y;
	float z;
	float w;
	float dist;
}graph_node;

typedef struct graph {
	graph_node** adjl;
}graph;

typedef struct node {
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

void swap(node* p, node* c) {
	c->parent = p->parent;
	p->parent = c;
	node* tempcleft = c->left;
	node* tempcright = c->right;
	if (p->left == c) {
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

void percolate(minheap* h, node* n) {
	node* curr = h->root;
	// loop_indicate: -1=end loop; 1=left child exists; 2=right; 3=both exist
	// used to prevent segfaults
	int loop_indicate = 1;
	while (loop_indicate != 0) {
		if (curr->left) {
			if (curr->right) {
				// has both children
				if (curr->left->val < curr->right->val) {
					if (curr->val > curr->left->val) {
						// swap, check, continue
						swap(curr, curr->left);
						if !(curr->left || curr->right) {
							loop_indicate = 0;
						}
					}
					else {
						// curr is the furthest we can put it, we're done
						loop_indicate = 0;
					}
				}
				else {
					if (curr->val > curr->right->val) {
						// swap, check, continue
						swap(curr, curr->right);
						if !(curr->left || curr->right) {
							loop_indicate = 0;
						}
					}
					else {
						// curr is the furthest we can put it, we're done
						loop_indicate = 0;
					}
				}
			}
			else {
				// only has left child
				if (curr->val > curr->left->val) {
					// swap, check, continue
					swap(curr, curr->left);
					if !(curr->left || curr->right) {
						loop_indicate = 0;
					}
				}
				else {
					// curr is the furthest we can put it, we're done
					loop_indicate = 0;
				}
			}
		}
		else if (curr->right) {
			// only has right child
			if (curr->val > curr->right->val) {
				// swap, check, continue
				swap(curr, curr->right);
				if !(curr->left || curr->right) {
					loop_indicate = 0;
				}
			}
			else {
				// curr is the furthest we can put it, we're done
				loop_indicate = 0;
			}
		}
	}
}

float deletemin(minheap* h) {
	if (h->empty) {
		return HEAPEMPTY;
	}
	float x = h->root->val;
	if (!(h->root->left || h->root->right)) {
		free(h->root);
		h->root = NULL;
		h->bottom = NULL;
		h->empty = 1;
	}
	else if (!h->root->left && h->root->right) {
		node* temp = h->root->right;
		free(h->root);
		h->root = temp;
	}
	else if (h->root->left && !h->root->right) {
		node* temp = h->root->left;
		free(h->root);
		h->root = temp;
	}
	else {
		if (h->root->left->val > h->root->right->val) {
			node* temp_left = h->root->left;
			node* temp_right = h->root->right;
			free(h->root);
			node* curr = temp_left;
			while (curr->left || curr->right) {
				if (curr->left) {
					if (curr->right) {
						// both children exist, go down through larger child
						if (curr->left->val > curr->right->val) {
							curr = curr->left;
						}
						else {
							curr = curr->right;
						}
					}
					else {
						// only left child exists
						curr = curr->left;
					}
				}
				else {
					// only right child exists
					curr = curr->right;
				}
			}

			// now curr is a leaf - make it the root and percolate
			temp_left->parent = curr;
			temp_right->parent = curr;
			curr->left = temp_left;
			curr->right = temp_right;
			curr->parent = NULL;
			h->root = curr;
			percolate(h);
		}
	}
}

graph initiate_graph(int n, int dim) {
	// seed pseudorandom number generator
	srand(time(NULL));
	graph g;

	if (dim == 0) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				g[i][j] = rand();
			}
		}
	}
	else if (dim == 2) {
		for (int i = 0; i < n; i++) {
			
		}
	}
	else if (dim == 3){

	}
	else {

	}
}

int main(int argc, char* argv[]) {
	if (argc != 4) {
		printf("Check number of arguments!\n");
		abort;
	}
	int numpoints = argv[1];
	int numtrials = argv[2];
	int dimension = argv[3];

}