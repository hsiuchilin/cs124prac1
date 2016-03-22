// Completed by: Jimmy Lin and Tony Li

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void arithmetic_matrix(int flag, int **m1, int **m2, int **m3, int a1, int b1, int c1, int d1, int a2, int b2, int c2, int d2) {
	int n = c1 - a1 + 1;
	if (flag == 1) {
		// addition
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				m3[i][j] = m1[a1 + i][b1 + j] + m2[a2 + i][b2 + j];
			}
		}
	}
	else {
		// subtraction
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				m3[i][j] = m1[a1 + i][b1 + j] - m2[a2 + i][b2 + j];
			}
		}
	}
}

void matrix_mult(int **m1, int **m2, int **m3, int a1, int b1, int c1, int a2, int b2) {
	int n = c1-a1+1;
	for (int i = 0; i < n; i++) {
		for (int k = 0; k < n; k++) {
			for (int j = 0; j < n; j++) {
				// printf("m3: [%i, %i] = %i\n", i, j, m3[i][j]);
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

void prettyprinter(int **m, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%i ", m[i][j]);
		}
		printf("\n");
	}
}

int*** setup_strassens(int **m1, int **m2, int n, int a1, int b1, int a2, int b2, int n0) {
	int ***p = malloc(sizeof(int**) * 7);
	for (int i = 0; i < 7; i++) {
		p[i] = init_matrix(n/2);
	}
	int **temp_matrix1 = init_matrix(n/2);
	arithmetic_matrix(-1, m2, m2, temp_matrix1, a1, b1+n/2, a1 + n/2-1, b1+n, a2+n/2, b2+n/2, a2+n-1, b2+n-1);
	multiplier(m1, temp_matrix1, p[0], a1, b1, a1+n/2-1, a2,b2);

	arithmetic_matrix(1, m1, m1, temp_matrix1, a1,b1,a1+n/2-1, b1+n/2-1, a2,b2+n/2, a2+n/2-1, b2+n);
	matrix_mult(temp_matrix1, m2, p[1], a1,b1,a1+n/2-1, a2+n/2,b2+n/2);

	arithmetic_matrix(1,m1,m1,temp_matrix1, a1+n/2, b1, a1+n-1, b1+ n/2-1, a2+n/2,b2+n/2, a2+n,b2+n);
	matrix_mult(temp_matrix1, m2, p[2],a1,b1,a1+n/2-1, a2,b2);

	arithmetic_matrix(-1, m2, m2, temp_matrix1, a1+n/2,b1,a1+n-1, b1+n/2-1, a2,b2, a2+n/2-1,b2+n/2-1);
	matrix_mult(m1, temp_matrix1, p[3], a1+n/2,b1+n/2, a1+n-1, a2,b2);
	
	int **temp_matrix2 = init_matrix(n/2);
	
	arithmetic_matrix(1, m1, m1, temp_matrix1, a1,b1,a1+n/2-1, b1+n/2-1, a2+n/2,b2+n/2, a2+n, b2+n);
	arithmetic_matrix(1, m2, m2, temp_matrix2, a1,b1,a1+n/2-1, b1+n/2-1, a2+n/2,b2+n/2, a2+n, b2+n);
	matrix_mult(temp_matrix1,temp_matrix2, p[4], a1,b1,a1+n/2-1, a2,b2);
	
	arithmetic_matrix(-1, m1, m1, temp_matrix1, a1, b1+n/2, a1+n/2-1, b1+n, a2+n/2,b2+n/2, a2+n, b2+n);
	arithmetic_matrix(1, m2, m2, temp_matrix2, a1+n/2, b1, a1+n-1, b1+n/2-1, a2+n/2, b2+n/2, a2+n, b2+n);
	matrix_mult(temp_matrix1,temp_matrix2, p[5], a1,b1,a1+n/2-1, a2,b2);
	
	arithmetic_matrix(-1, m1, m1, temp_matrix1, a1,b1,a1+n/2-1, b1+n/2-1, a2+n/2, b2, a2+n, b2+n/2-1);
	arithmetic_matrix(1, m2, m2, temp_matrix2, a1,b1, a1+n/2-1,b1+n/2-1, a2, b2+n/2, a2+n/2-1, b2+n);
	matrix_mult(temp_matrix1,temp_matrix2, p[6], a1,b1,a1+n/2-1, a2,b2);

	return p;
}

void strassens(int **m1, int **m2, int **m3, int n, int a1, int b1, int a2, int b2) {
	// int n = c1-a1 +1;
	int ***ps = setup_strassens(m1, m2, n, a1, b1, a2, b2);
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

void multiplier(int **m1, int **m2, int **m3, int n, int a1, int b1, int a2, int b2, int n0) {
	// int n = c - a + 1;
	if (n < n0) {
		matrix_mult(m1, m2, m3, a, b, c, d);
	}
	else {
		strassens(m1, m2, m3, n, a1, b1, a2, b2);
	}
}

int main(int argc, char* argv[]) {
	// int **m1 = rand_matrix(4);
	// prettyprinter(m1, 4);
	// int **m2 = rand_matrix(4);
	// prettyprinter(m2, 4);
	// int **m3 = init_matrix(4);
	// printf("\n");
	// matrix_mult(m1, m2, m3, 0,0,3, 0,0);
	// prettyprinter(m3, 4);
	int** id1 = identity(4);
	int** id2 = identity(4);
	printf("Identities \n");
	int*** ps = setup_strassens(id1, id2, 4,0,0,0,0);
	printf("Strassens \n");
	for (int i = 0; i < 7; i++){
		prettyprinter(ps[i], 2);
	}
}
