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

typedef struct edge {
	int source;
	int target;
	float weight;
}edge;

typedef struct node {
	float val;
	struct node* parent;
	struct node* left;
	struct node* right;
	edge* assoc_edge;
}node;

typedef struct minheap {
	node* root;
	node* bottom;
	int empty;
}minheap;

node* create_node(edge* e) {
	node* new = malloc(sizeof(node));
	node tempnew ={e->weight, NULL, NULL, NULL, e};
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
						if (loop_indicate == -1) {
							h->root = curr->left;
							loop_indicate = 1;
						}
						swap(curr, curr->left);
						curr = curr->left;
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

node* deletemin(minheap* h) {
	if (h->empty) {
		return NULL;
	}
	node* return_node = malloc(sizeof(node));
	memcpy(return_node, h->root, sizeof(node));
	// float x = h->root->val;
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

	return return_node;
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

	printf("initiate\n");

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

// int array_inclusion(graph_node* node_array, int num_v, graph_node* check) {
// 	for (int i = 0; i < num_v; i++) {
// 		if (node_array[i].x == check->x && node_array[i].y == check->y && node_array[i].z == check->z && node_array[i].w == check->w) {
// 			return 1;
// 		}
// 	}
// 	return 0;
// }

float prim(edge** g, graph_node* point_array, int numpoints, int v_index) {
	printf("prim\n");
	// initialize heap
	minheap* m = malloc(sizeof(minheap));
	minheap temp = {NULL, NULL, 1};
	memcpy(m, &temp, sizeof(minheap));

	// initialize heap with self-loop to root vertex
	edge *start_edge = malloc(sizeof(edge));
	start_edge->weight = 0;
	start_edge->source = v_index;
	start_edge->target = v_index;
	insert(m, create_node(start_edge));

	// instantiate list of edges to return
	// edge return_edges[num_edges];

	// int num_edges = numpoints * (numpoints-1) / 2;


	// S
	int explored_v[numpoints];
	for (int i = 0; i < numpoints; i++) {
		explored_v[i] = 0;
	}
	// int explored_i = 0;

	float return_weight = 0.0;

	int dist[numpoints];
	// graph_node prev[numpoints];

	for (int i = 0; i < numpoints; i++) {
		dist[i] = INT_MAX - 1;
		// prev[i] = NULL;
	}

	dist[v_index] = 0;

	while (!m->empty) {
		printf("hi\n");

		node* deleted = deletemin(m);
		// explored_v[explored_i] = point_array[deleted->assoc_edge->target];
		// explored_i++;
		int e = deleted->assoc_edge->target;
		printf("e: %d\n", e);
		printf("explored?: %i\n", !explored_v[e]);
		if (!explored_v[e]) {
			explored_v[e] = 1;
			printf("wait %i\n", dist[e]);
			if (dist[e] > deleted->val) {
				dist[e] = deleted->val;
				return_weight += deleted->val;
				for (int i =0; i<numpoints; i++) {
					printf("check source: %i\n", g[e][i].source);
					if (g[e][i].source == e) {
						printf("insert this: %f from %i to %i\n", g[e][i].weight, g[e][i].source, g[e][i].target);
						insert(m, create_node(&g[e][i]));
					}
				}
			}
		}	
	}

	return return_weight;
}

int main(int argc, char* argv[]) {
	if (argc != 4) {
		printf("Check number of arguments!\n");
		// abort;
	}
	int numpoints= atoi(argv[1]);
	int numtrials= atoi(argv[2]);
	int dim= atoi(argv[3]);
	printf("numpoints %i numtrials %i dim %i\n", numpoints, numtrials, dim);
	graph_node* parray = malloc(sizeof(graph_node) * numpoints);
	edge** g =initiate_graph(numpoints, dim, parray);
	printf("%f: from %i to %i\n", g[0][0].weight, g[0][0].source, g[0][0].target);
	printf("%f: from %i to %i\n", g[0][1].weight, g[0][1].source, g[0][1].target);
	printf("%f: from %i to %i\n", g[1][0].weight, g[1][0].source, g[1][0].target);
	printf("%f: from %i to %i\n", g[1][1].weight, g[1][1].source, g[1][1].target);

	float total = prim(g, parray, numpoints, 0);
	printf("%f heavy fat woman\n", total);
	// minheap* m = malloc(sizeof(minheap));
	// minheap temp = {NULL, NULL, 1};
	// memcpy(m, &temp, sizeof(minheap));
	// printf("printing heap\n");
	// insert(m, create_node(1.));
	// heap_printer(m->root);	
	// insert(m, create_node(2.));
	// insert(m, create_node(3.));
	// insert(m, create_node(5.));
	// insert(m, create_node(4.));
	// deletemin(m);
	// deletemin(m);
	// deletemin(m);
	// insert(m, create_node(6.));
	// insert(m, create_node(333.));
	// insert(m, create_node(5.));
	// deletemin(m);

	// printf("printing new heap\n");
	// heap_printer(m->root);
	// heap_checker(m->root);


}