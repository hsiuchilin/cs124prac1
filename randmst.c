#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <limits.h>

#define HEAPEMPTY -1

typedef struct graph_node {
	float x;
	float y;
	float z;
	float w;
}graph_node;

// typedef struct graph {
// 	graph_node** adjl;
// }graph;

typedef struct node {
	float val;
	struct node* parent;
	struct node* left;
	struct node* right;
}node;

typedef struct edge {
	int source;
	int target;
	float weight;
}edge;

typedef struct minheap {
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
			if (curr->right && curr->left) {
				// curr has both children
				if (n->val > curr->right->val && n->val > curr->left->val) {
					if (curr->right->val < curr->left->val) {
						curr = curr->left;
					}
					else {
						curr = curr->right;
					}
				}
				else if (n->val < curr->left->val && curr->left->val > curr->right->val) {
					n->parent = curr->parent;
					n->left = curr;
					curr->parent = n;
					active = 0;
				}
				else if (n->val < curr->right->val && curr->right->val > curr->left->val) {
					n->parent = curr->parent;
					n->right = curr;
					curr->parent = n;
					active = 0;
				}
				else {
					// curr should be swapped with n
					n->parent = curr->parent;
					n->left = curr;
					curr->parent = n;
					active = 0;
				}
			}
			else if (curr->left) {
				// curr only has left child
				curr->right = n;
				n->parent = curr;
				active = 0;
			}
			else {
				// curr only has right child or no children
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



edge **initiate_graph(int n_points, int dim, graph_node* point_array) {
	// seed pseudorandom number generator
	srand(time(NULL));
	edge** g = malloc(sizeof(edge*)*n_points);
	point_array= malloc(sizeof(graph_node) * n_points);
	if (dim == 0) {
		for (int i = 0; i < n_points; i++) {
			g[i] = malloc(sizeof(edge)*n_points);
			for (int j = 0; j < n_points; j++) {
				g[i][j].weight = rand() / (float)RAND_MAX;
				g[i][j].source = i;
				g[i][j].target = j;
			}
		}
	}
	else if (dim == 1) {
		for (int i = 0; i < n_points; i++) {
			point_array[i].x = rand() / (float)RAND_MAX;
			point_array[i].y = rand() / (float)RAND_MAX;
			point_array[i].z = 0;
			point_array[i].w = 0;
		}
		for (int i = 0; i < n_points; i++) {
			g[i] = malloc(sizeof(edge)*n_points);
			for (int j = 0; j < n_points; j++) {
				g[i][j].weight = sqrt(pow(point_array[i].x- point_array[j].x, 2)
					+ pow(point_array[i].y-point_array[j].y, 2));
				g[i][j].source = i;
				g[i][j].target = j;
			}
		}
	}
	else if (dim == 3) {
		for (int i = 0; i < n_points; i++) {
			point_array[i].x = rand() / (float)RAND_MAX;
			point_array[i].y = rand() / (float)RAND_MAX;
			point_array[i].z = rand() / (float)RAND_MAX;
			point_array[i].w = 0;
		}
		for (int i = 0; i < n_points; i++) {
			g[i] = malloc(sizeof(float)*n_points);
			for (int j = 0; j < n_points; j++) {
				g[i][j].weight = sqrt(pow(point_array[i].x- point_array[j].x, 2)
					+ pow(point_array[i].y-point_array[j].y, 2)
					+ pow(point_array[i].z-point_array[j].z, 2));
				g[i][j].source = i;
				g[i][j].target = j;
			}
		}
	}
	else {
		for (int i = 0; i < n_points; i++) {
			point_array[i].x = rand() / (float)RAND_MAX;
			point_array[i].y = rand() / (float)RAND_MAX;
			point_array[i].z = rand() / (float)RAND_MAX;
			point_array[i].w = rand() / (float)RAND_MAX;
		}
		for (int i = 0; i < n_points; i++) {
			g[i] = malloc(sizeof(float)*n_points);
			for (int j = 0; j < n_points; j++) {
				g[i][j].weight = sqrt(pow(point_array[i].x - point_array[j].x, 2)
					+ pow(point_array[i].y - point_array[j].y, 2)
					+ pow(point_array[i].z - point_array[j].z, 2)
					+ pow(point_array[i].w - point_array[j].w, 2));
				g[i][j].source = i;
				g[i][j].target = j;
			}
		}
	}

	return g;
}

edge *prim(edge **g, graph_node* point_array, int numpoints, edge *edge_array) {
	// initialize heap
	minheap* m = malloc(sizeof(minheap));
	minheap temp = {NULL, NULL, 1};
	memcpy(m, &temp, sizeof(minheap));

	int dist[num_v]; 
	graph_node prev[num_v];

	for (int i = 0; i < num_v; i++;) {
		dist[i] = INT_MAX;
		prev[i] = NULL;
	}


}

int main(int argc, char* argv[]) {
	if (argc != 4) {
		printf("Check number of arguments!\n");
		// abort;
	}

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
	insert(m, create_node(6.));
	insert(m, create_node(333.));
	insert(m, create_node(5.));
	deletemin(m);

	printf("printing new heap\n");
	heap_printer(m->root);
	heap_checker(m->root);


}