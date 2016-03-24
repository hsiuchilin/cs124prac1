#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
	FILE *f = fopen("bigtest.txt", "w");
	// for (int i = 0; i < 1024*1024; i++) {
		for (int i = 0; i < 2*1024*1024; i++) {
			fprintf(f, "%i\n",rand() % 3);
		}
	// }
	

}