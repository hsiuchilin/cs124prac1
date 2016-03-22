// Completed by: Jimmy Lin and Tony Li

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void arithmetic_matrix(int flag, int *m1, int *m2, int *m3, int n) {
	if (flag == 1) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				m3[i][j] = m1[i][j] + m2[i][j];
			}
		}
	}
	else {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				m3[i][j] = m1[i][j] - m2[i][j];
			}
		}
	}
}

int*** setup_strassens(int *m1, int *m2, int n) {
	int ***p = malloc(sizeof(int**) * 7);
	for (int i = 0; i < 7; i++) {
		p[i] = init_matrix(n/2);
	}
	p[0] = arithmetic_matrix()
}

void strassens(int **m1, int **m2, int **m3, int a, int b, int c, int d) {
	int *p1 = malloc(sizeof(int) * 7);
}

void matrix_mult(int **m1, int **m2, int **m3, int a, int b, int c, int d) {
	for (int i = a; i <= c; i++) {
		for (int k = a; k <= c; k++) {
			for (int j = b; j <= d; j++) {
				// printf("m3: [%i, %i] = %i\n", i, j, m3[i][j]);
				m3[i][j] += m1[i][k-a+b] * m2[k][j];
			}
		}
	}
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

void prettyprinter(int **m, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%i ", m[i][j]);
		}
		printf("\n");
	}
}

int **init_matrix(int n) {
	int **m = malloc(sizeof(int*) * n);
	for (int i = 0; i < n; i++) {
		m[i] = malloc(sizeof(int) * n);
	}
	return m;
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

void multiplier(int **m1, int **m2, int **m3, int a, int b, int c, int d, int n0) {
	int n = c - a + 1;
	if (n < n0) {
		matrix_mult(m1, m2, m3, a, b, c, d);
	}
	else {
		strassens(m1, m2, m3, a, b, c, d);
	}
}

int main(int argc, char* argv[]) {
	int **m1 = rand_matrix(3);
	// prettyprinter(m1, 3);
	int **m2 = rand_matrix(3);
	// prettyprinter(m2, 3);
	int **m3 = init_matrix(3);
	printf("\n");
	matrix_mult(m1, m2, m3, 0,0,2,2);
	prettyprinter(m3, 3);
}
