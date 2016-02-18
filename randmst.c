#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define HEAPEMPTY -1

typedef struct graph_node {
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

node* create_node(float val){
	node* new = malloc(sizeof(node));
	node tempnew ={val, NULL, NULL, NULL};
	memcpy(new, &tempnew, sizeof(node));
	return new;
}

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
		p->right= tempcright;
	}
}

void insert (minheap* h, node* n){
	if (h->empty) {
		h->root = n;
		h->empty = 0;
	}
	else {
		node* curr = h->root;
		int active =1;
		while (active){
			if (curr->right && curr->left){
				if (curr->right->val < curr->left->val){
					curr = curr->left;
				}
				else{
					curr = curr->right;
				}
			}
			else if (curr->right){
				curr->left = n;
				n->parent = curr;
				active = 0;
			}
			else if (curr->left){
				curr->right = n;
				n->parent = curr;
				active = 0;
			}
			else{
				curr-> left = n;
				n->parent = curr;
				active = 0;
			}
		}
	}
}

void percolate(minheap* h) {
	node* curr = h->root;
	// loop_indicate: -1= first iteration 0=end loop; 1=child exists;
	// used to prevent segfaults
	int loop_indicate = -1;
	while (loop_indicate != 0) {
		if (curr->left) {
			if (curr->right) {
				// has both children
				if (curr->left->val < curr->right->val) {
					if (curr->val > curr->left->val) {
						// swap, check, continue
						if (loop_indicate == -1){
							h->root = curr->left;
							loop_indicate = 1;
						}
						swap(curr, curr->left);
						if (!(curr->left || curr->right)) {
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
						if (loop_indicate == -1){
							h->root = curr-> right;
							loop_indicate = 1;
						}
						swap(curr, curr->right);
						if (!(curr->left || curr->right)) {
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
					if (loop_indicate == -1){
						h->root = curr->left;
						loop_indicate = 1;
					}
					swap(curr, curr->left);
					if (!(curr->left || curr->right)){
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
				if (loop_indicate == -1){
					h->root = curr->right;
					loop_indicate = 1;
				}
				swap(curr, curr->right);
				if (!(curr->left || curr->right)) {
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
		node* temp_left;
		node* temp_right;
		free(h->root);
		node* curr; 
		if (h->root->left->val > h->root->right->val) {
			temp_left = h->root->left;	
			temp_right = h->root->right;
		}
		else{
			temp_left = h->root->right;
			temp_right = h->root->left;
		}
		curr = temp_left;
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
		if (curr->parent && curr->parent != h->root){
			if (curr->parent->left == curr){
				curr->parent->left = NULL;
			}
			else{
				curr->parent ->right = NULL;
			}
		}
		curr->right = temp_right;
		temp_right->parent = curr;
		if (curr != temp_left){
			curr->left = temp_left;
		}
		curr->parent = NULL;
		h->root = curr;
		percolate(h);
	}

	return x;
}

void heap_printer (node* n){
	printf("%f\n", n->val);
	if(n->left){
		heap_printer(n->left);
	}
	if (n-> right){
		heap_printer(n->right);
	}
}

void heap_checker (node* n){
	if (n->right){
		if (n->right->val < n->val){
			printf("you done goofed \n");
		}
		heap_checker(n->right);
	}
	if (n->left){
		if(n->left->val <n->val){
			printf("youdone goofed\n");
		}
		heap_checker(n->left);
	}
}



// graph initiate_graph(int n, int dim) {
// 	// seed pseudorandom number generator
// 	srand(time(NULL));
// 	graph g;

// 	if (dim == 0) {
// 		for (int i = 0; i < n; i++) {
// 			for (int j = 0; j < n; j++) {
// 				g[i][j] = rand();
// 			}
// 		}
// 	}
// 	else if (dim == 2) {
// 		for (int i = 0; i < n; i++) {
			
// 		}
// 	}
// 	else if (dim == 3){

// 	}
// 	else {

// 	}
// }

int main(int argc, char* argv[]) {
	if (argc != 4) {
		printf("Check number of arguments!\n");
		// abort;
	}
	// int numpoints = argv[1];
	// int numtrials = argv[2];
	// int dimension = argv[3];

	// node lltemp = {4, NULL, NULL, NULL};
	// node ltemp= {3, NULL, NULL , NULL};
	// node rtemp= {2, NULL, NULL, NULL};
	// node ntemp= {1, NULL, NULL, NULL};
	// node newtemp = {7, NULL, NULL, NULL};
	// node* l = malloc(sizeof(node));
	// node* r  = malloc(sizeof(node));
	// node* n = malloc(sizeof(node));
	// node* ll = malloc(sizeof(node));
	// node* new = malloc(sizeof(node));
	// memcpy(ll, &lltemp, sizeof(node));
	// memcpy(l, &ltemp, sizeof(node));
	// memcpy(r, &rtemp, sizeof(node));
	// memcpy(n, &ntemp, sizeof(node));
	// memcpy(new, &newtemp, sizeof(node));
	// n->left = l;
	// n->right= r;
	// l->parent = n;
	// r->parent = n;
	// l->right = ll;
	// ll->parent =l;
	minheap* m = malloc(sizeof(minheap));
	minheap temp = {NULL, NULL, 1};
	memcpy(m, &temp, sizeof(minheap));
	printf("printing heap\n");
	insert(m, create_node(1.));
	heap_printer(m->root);	
	insert(m, create_node(2.));
	insert(m, create_node(3.));
	insert(m, create_node(5.));
	insert(m, create_node(4.));
	deletemin(m);
	deletemin(m);
	deletemin(m);
	// insert(m, create_node(6.));
	insert(m, create_node(333.));
	insert(m, create_node(1.));
	// deletemin(m);

	printf("printing new heap\n");
	heap_printer(m->root);
	heap_checker(m->root);


}