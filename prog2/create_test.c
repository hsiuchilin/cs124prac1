#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
	FILE *f = fopen("test3.txt", "w");
	// for (int i = 0; i < 1024*1024; i++) {
		for (int i = 0; i < 100*2; i++) {
			fprintf(f, "%i\n",rand() % 3 - 1);
		}
	// }
	

}