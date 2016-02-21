#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <limits.h>

typedef struct graph_node {
	float x;
	float y;
	float z;
	float w;
}graph_node;

typedef struct edge {
	int source;
	int target;
	float weight;
}edge;

typedef struct heap {
	// unsigned int length;
	unsigned int heap_size;
	edge* h;
}heap;

int parent(int i) {
	return (i-1)/2;
}

int left(int i) {
	return 2*i+1;
}
int right(int i) {
	return 2*i +2;  
}

void insert(heap* h, edge* new) {
	h->heap_size += 1;
	int index = h->heap_size -1;
	while (index>0 && h->h[parent(index)].weight > new->weight) {
		h->h[index] = h->h[parent(index)];
		index = parent(index);
	}
	h->h[index] = *new;
}

void swap(heap* h, int i, int j) {
	edge a= h->h[i];
	h->h[i] = h->h[j];
	h->h[j] = a;
}

void min_heapify(heap* h, int i) {
	int l = left(i);
	int r = right(i);
	int small;
	if (l < h->heap_size && h->h[l].weight < h->h[r].weight){
		small = l;
	}
	else{
		small= i;
	}
	if (r< h->heap_size && h->h[r].weight < h->h[small].weight){
		small = r;
	}
	if (small != i){
		swap(h, i, small);
		min_heapify(h, small);
	}
}

// Check for empty before calling deletemin
edge deletemin(heap* h) {
	edge deleted = (h->h[0]);
	h->heap_size -= 1;
	h->h[0] = h->h[h->heap_size];
	min_heapify(h, 0);
	return deleted;
}

void heap_printer (heap* h) {
	for (int i = 0; i < h->heap_size; i++){
		printf("Index %i: Source %i Dest %i Weight %f\n", i,
			h->h[i].source, h->h[i].target, h->h[i].weight);
	}
}

edge **initiate_graph(int n_points, int dim, graph_node* point_array) {
	// seed pseudorandom number generator
	srand(time(NULL));

	printf("initiate\n");

	edge** g = malloc(sizeof(edge*)*n_points);
	point_array= malloc(sizeof(graph_node) * n_points);
	for (int i = 0; i < n_points; i++) {
		g[i] = malloc(sizeof(edge)*n_points);
	}

	if (dim == 0) {

		for (int i = 0; i < n_points; i++) {
			for (int j = i; j < n_points; j++) {
				g[i][j].weight = rand() / (float)RAND_MAX;
				g[i][j].source = i;
				g[i][j].target = j;

				g[j][i].weight = g[i][j].weight;
				g[j][i].source = j;
				g[j][i].target = i;
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
			for (int j = i; j < n_points; j++) {
				g[i][j].weight = sqrt(pow(point_array[i].x- point_array[j].x, 2)
					+ pow(point_array[i].y-point_array[j].y, 2));
				g[i][j].source = i;
				g[i][j].target = j;

				g[j][i].weight = g[i][j].weight;
				g[j][i].source = j;
				g[j][i].target = i;
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
			for (int j = i; j < n_points; j++) {
				g[i][j].weight = sqrt(pow(point_array[i].x- point_array[j].x, 2)
					+ pow(point_array[i].y-point_array[j].y, 2)
					+ pow(point_array[i].z-point_array[j].z, 2));
				g[i][j].source = i;
				g[i][j].target = j;

				g[j][i].weight = g[i][j].weight;
				g[j][i].source = j;
				g[j][i].target = i;
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
			for (int j = i; j < n_points; j++) {
				g[i][j].weight = sqrt(pow(point_array[i].x - point_array[j].x, 2)
					+ pow(point_array[i].y - point_array[j].y, 2)
					+ pow(point_array[i].z - point_array[j].z, 2)
					+ pow(point_array[i].w - point_array[j].w, 2));
				g[i][j].source = i;
				g[i][j].target = j;

				g[j][i].weight = g[i][j].weight;
				g[j][i].source = j;
				g[j][i].target = i;
			}
		}
	}

	return g;
}

float prim(edge** g, graph_node* point_array, int numpoints, int v_index) {
	// printf("Beginning Prim\n");
	// initialize heap
	int numedges = numpoints *(numpoints-1)/2;
	heap* m = malloc(sizeof(heap));
	m->heap_size =0;
	m->h = malloc(sizeof(edge)*numedges);
	insert(m, &g[v_index][v_index]);

	// S
	int explored_v[numpoints];

	float return_weight = 0.0;

	for (int i = 0; i < numpoints; i++) {
		explored_v[i] = 0;
	}

	while (m->heap_size!=0) {
		// printf("Printing Heap \n");
		// heap_printer(m);
		edge deleted = deletemin(m);
		// printf("Deleted: Source %i Target %i Weight %f\n", deleted.source, deleted.target, deleted.weight);
		int e = deleted.target;
		if (!explored_v[e]) {
			explored_v[e] = 1;
			// if (dist[e] > deleted->val) {
				// dist[e] = deleted->val;
			for (int i = 0; i < numpoints; i++) {
				if (!explored_v[i]) {
					// printf("pushing node %i, of value %f\n", i, g[e][i].weight);
					insert(m, &g[e][i]);
				}	
			}
			if (e!= deleted.source){
				return_weight += deleted.weight;
			}
			// printf("return weight: %f\n", return_weight);
			// }
		}	
	}

	return return_weight;
}


int main(int argc, char* argv[]) {
	if (argc != 4) {
		printf("Check number of arguments!\n");
		// abort;
	}

	int numpoints= atoi(argv[2]);
	int numtrials= atoi(argv[3]);
	int dim= atoi(argv[4]);

	graph_node* parray = malloc(sizeof(graph_node) * numpoints);
	// int numedges= numpoints * (numpoints-1)/2;
	edge** g=initiate_graph(numpoints, dim, parray);
	// printf("%f: from %i to %i\n", g[0][0].weight, g[0][0].source, g[0][0].target);
	// printf("%f: from %i to %i\n", g[0][1].weight, g[0][1].source, g[0][1].target);
	// printf("%f: from %i to %i\n", g[1][0].weight, g[1][0].source, g[1][0].target);
	// printf("%f: from %i to %i\n", g[1][1].weight, g[1][1].source, g[1][1].target);

	// printf("%f: from %i to %i\n", g[0][2].weight, g[0][2].source, g[0][2].target);
	// printf("%f: from %i to %i\n", g[0][3].weight, g[0][3].source, g[0][3].target);
	// printf("%f: from %i to %i\n", g[1][2].weight, g[1][2].source, g[1][2].target);
	// printf("%f: from %i to %i\n", g[2][0].weight, g[2][0].source, g[2][0].target);
	// printf("%f: from %i to %i\n", g[2][1].weight, g[2][1].source, g[2][1].target);
	// printf("%f: from %i to %i\n", g[2][2].weight, g[2][2].source, g[2][2].target);

	
	// printf("%f: from %i to %i\n", g[1][3].weight, g[1][3].source, g[1][3].target);
	// printf("%f: from %i to %i\n", g[2][3].weight, g[2][3].source, g[2][3].target);
	// printf("%f: from %i to %i\n", g[3][0].weight, g[3][0].source, g[3][0].target);
	// printf("%f: from %i to %i\n", g[3][1].weight, g[3][1].source, g[3][1].target);
	// printf("%f: from %i to %i\n", g[3][2].weight, g[3][2].source, g[3][2].target);
	// printf("%f: from %i to %i\n", g[3][3].weight, g[3][3].source, g[3][3].target);

	float final = 0.0;
	for (int trial = 0; trial < numtrials; trial++) {
		final += prim(g,parray, numpoints, 0);
	}
	final = final / numtrials;

	printf("%f %i %i %i\n", final, numpoints, numtrials, dim);

	return 0;
}