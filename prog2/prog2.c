// Completed by: Jimmy Lin and Tony Li

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int **init_matrix(int n) {
	int **m = malloc(sizeof(int*) * n);
	for (int i = 0; i < n; i++) {
		m[i] = malloc(sizeof(int) * n);
		for (int j = 0; j < n; j++){
			m[i][j] = 0;
		}
	}
	return m;
}

void free_matrix(int** m, int n){
	for (int i =0; i<n; i++){
		free(m[i]);
	}
	free(m);
}

void arithmetic_matrix(int flag, int **m1, int **m2, int **m3, int n, int a1, int b1, int a2, int b2, int a3, int b3) {
	if (flag == 1) {
		// addition
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				m3[i+a3][j+b3] = m1[a1 + i][b1 + j] + m2[a2 + i][b2 + j];
			}
		}
	}
	else {
		// subtraction
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				m3[i+a3][j+b3] = m1[a1 + i][b1 + j] - m2[a2 + i][b2 + j];
			}
		}
	}
}

void matrix_mult(int **m1, int **m2, int **m3, int n, int a1, int b1, int a2, int b2) {
	for (int i = 0; i < n; i++) {
		for (int k = 0; k < n; k++) {
			for (int j = 0; j < n; j++) {
				m3[i][j] += m1[i+a1][k+b1] * m2[k+a2][j+b2];
			}
		}
	}
}

void clear_matrix(int **m, int n){
	for (int i =0; i< n; i++){
		for (int j = 0; j <n; j++){
			m[i][j] = 0;
		}
	}
}

void prettyprinter(int **m, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%i ", m[i][j]);
		}
		printf("\n");
	}
}

int *** setup_strassens(int **m1, int **m2, int n, int a1, int b1, int a2, int b2, int n0);

void strassens(int **m1, int **m2, int **m3, int n, int a1, int b1, int a2, int b2, int n0) {
	if (n <= n0) {
		// printf("hi\n");
		// printf("and then we multiply like 7 times\n");
		printf("hi\n");
		matrix_mult(m1, m2, m3, n, a1, b1, a2, b2);
	}
	else {
		printf("Look at us Strassening and shit\n");
		int*** p = setup_strassens(m1, m2, n, a1, b1, a2, b2, n0);
		// Top Left Quadrant
		arithmetic_matrix(1, p[4], p[5], p[5], n/2, 0,0,0,0,0,0);
		arithmetic_matrix(1, p[3], p[5], p[5], n/2, 0,0,0,0,0,0);
		arithmetic_matrix(-1, p[5], p[1], m3, n/2, 0,0,0,0,0,0);
		free_matrix(p[5], n/2);

		// Bottom Right Quadrant
		arithmetic_matrix(-1, p[4], p[6], p[6], n/2, 0,0,0,0,0,0);
		arithmetic_matrix(-1, p[6], p[2], p[6], n/2, 0,0,0,0,0,0);
		arithmetic_matrix(1, p[0], p[6], m3, n/2, 0,0,0,0, n/2,n/2);
		free_matrix(p[6],n/2);
		free_matrix(p[4],n/2);

		// Top Right Quadrant
		arithmetic_matrix(1, p[0], p[1], m3, n/2, 0,0,0,0,0,n/2);
		free_matrix(p[0],n/2);
		free_matrix(p[1],n/2);

		// Bottom Left Quadrant;
		arithmetic_matrix(1, p[2], p[3], m3, n/2, 0,0,0,0,n/2,0);
		free_matrix(p[2],n/2);
		free_matrix(p[3],n/2);

		free(p);
		// printf("hi\n");
	}
}


int*** setup_strassens(int **m1, int **m2, int n, int a1, int b1, int a2, int b2, int n0) {
	int ***p = malloc(sizeof(int**) * 7);
	for (int i = 0; i < 7; i++) {
		p[i] = init_matrix(n/2);
	}
	int **temp_matrix1 = init_matrix(n/2);

	arithmetic_matrix(-1, m2, m2, temp_matrix1, n/2, a2, b2+n/2, a2+n/2, b2+n/2,0,0);
	strassens(m1, temp_matrix1, p[0], n/2, a1,b1, 0,0, n0);


	arithmetic_matrix(1, m1, m1, temp_matrix1, n/2, a1,b1, a1,b1+n/2,0,0);
	strassens(temp_matrix1, m2, p[1],n/2, 0,0, a2+(n/2),b2+(n/2), n0);

	arithmetic_matrix(1,m1,m1,temp_matrix1, n/2, a1+n/2, b1, a1+n/2,b1+n/2,0,0);
	strassens(temp_matrix1, m2, p[2],n/2, 0,0, a2,b2, n0);

	arithmetic_matrix(-1, m2, m2, temp_matrix1, n/2, a2+n/2,b2,a2,b2,0,0);	
	strassens(m1, temp_matrix1, p[3], n/2, a1+(n/2),b1+(n/2), 0,0, n0);
	
	int **temp_matrix2 = init_matrix(n/2);

	arithmetic_matrix(1, m1, m1, temp_matrix1, n/2, a1,b1, a1+n/2,b1+n/2,0,0);
	arithmetic_matrix(1, m2, m2, temp_matrix2, n/2, a2,b2, a2+n/2,b2+n/2,0,0);
	strassens(temp_matrix1,temp_matrix2, p[4], n/2, 0,0, 0,0, n0);
	
	arithmetic_matrix(-1, m1, m1, temp_matrix1, n/2, a1, b1+n/2, a1+n/2,b1+n/2,0,0);
	arithmetic_matrix(1, m2, m2, temp_matrix2, n/2, a2+n/2, b2, a2+n/2, b2+n/2,0,0);
	strassens(temp_matrix1,temp_matrix2, p[5], n/2, 0,0, 0,0, n0);
	
	arithmetic_matrix(-1, m1, m1, temp_matrix1, n/2, a1,b1, a1+n/2, b1,0,0);
	arithmetic_matrix(1, m2, m2, temp_matrix2, n/2, a2,b2, a2, b2+n/2,0,0);
	strassens(temp_matrix1,temp_matrix2, p[6], n/2, 0,0, 0,0, n0);
	free_matrix(temp_matrix1,n/2);
	free_matrix(temp_matrix2,n/2);

	return p;
}

int **identity(int n) {
	int **return_matrix = malloc(sizeof(int*) * n);
	for (int i = 0; i < n; i++) {
		return_matrix[i] = malloc(sizeof(int)*n);
		for (int j = 0; j < n; j++) {
			if (i == j) {
				return_matrix[i][j] = 1;
			}
			else {
				return_matrix[i][j] = 0;
			}
		}
	}

	return return_matrix;
}

int **rand_matrix(int n) {
	int **m = init_matrix(n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			m[i][j] = rand() % 3 - 1;
		}
	}
	return m;
}

void diagonal(int** m, int n){
	for (int i = 0; i < n; i++){
		printf("%i\n", m[i][i]);
	}
}

int *** filematrices(char* filename, int d){
	FILE *f = fopen(filename, "r");
	int ***matrices = malloc(sizeof(int**)*2);
	matrices[0] = init_matrix(d);
	matrices[1] = init_matrix(d);
	for(int k = 0; k <2; k++){
		for (int i = 0; i < d; i++){
			for (int j = 0; j<d; j++){
				fscanf(f, "%i", &(matrices[k][i][j]));
			}
		}
	}
	fclose(f);
	return matrices;
}

int main(int argc, char* argv[]) {
	int** res = init_matrix(atoi(argv[3]));
	int ***ms= filematrices(argv[2],atoi(argv[3]));
	prettyprinter(ms[0],atoi(argv[3]));
	prettyprinter(ms[1],atoi(argv[3]));
	strassens(ms[0], ms[1], res, atoi(argv[3]), 0,0,0,0, 1);
	// diagonal(res, atoi(argv[3]));
	prettyprinter(res,atoi(argv[3]));
	free_matrix(res, atoi(argv[3]));
	free_matrix(ms[0], atoi(argv[3]));
	free_matrix(ms[1], atoi(argv[3]));
	
}
