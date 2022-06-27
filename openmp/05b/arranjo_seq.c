#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


void help() {
    fprintf(stderr, "### Possible Arguments ###\n");
    fprintf(stderr, "-f <filename>: file that contains X, N and the set S.\n");
    fprintf(stderr, "-n <N>: size of the set S.\n");
    fprintf(stderr, "-x <X>: value(int) to be searched in set S.\n\n");
    fprintf(stderr, "Examples:\n");
    fprintf(stderr, "\t1) ./executable -f fin.txt\n");
    fprintf(stderr, "\t2) ./executable -x 93 -n 30\n\n");
	exit(1);
}

int main(int argc, char **argv) {
	////////////////////////////////////
	// Parse arguments
	////////////////////////////////////

	int x = -1;
	int N = 0;
	int *S;
	char *filename = NULL;
	FILE *fin = NULL;
	int LINE_BUFFER = 64;
	char line[LINE_BUFFER+1];
	
	int i = 1;
	while (i+1 < argc) {
		if (strcmp(argv[i], "-f") == 0) {
			filename = argv[++i];
		}
		else if (strcmp(argv[i], "-x") == 0) {
			x = atoi(argv[++i]);
		}
		else if (strcmp(argv[i], "-n") == 0){
			N = atoi(argv[++i]);
		}

		i++;
	}

	// must have filename or (x and N)
	if (!filename && (N == 0 || x == -1)) help();
	else if (filename && (x != -1 || N != 0)) help();


	// Get x and N from file
	if (filename) {
		fin = fopen(filename, "r");
		if (!fin) {
			perror("Error");
		}

		// first line is x
		fgets(line, LINE_BUFFER, fin);
		
		char *tok = strtok(line, " ");
		if (!tok) {
			printf("Error: First line of file must be: <x> <N>\n");
		}
		x = atoi(tok);

		tok = strtok(NULL, " ");
		if (!tok) {
			printf("Error: First line of file must be: <x> <N>\n");
		}
		N = atoi(tok);
	}

	/////////////////////
	// Read or Generate S
	/////////////////////

	S = (int*)malloc(N*sizeof(N));
	if (fin) { // read S from file
		printf("Reading S from %s\n", filename);
		i = 0;
		int n;
		while (fgets(line, LINE_BUFFER, fin)) {
			n = strlen(line);
			if (line[--n] == '\n') line[n] = '\0'; // remove '\n'
			if (line[--n] == '\r') line[n] = '\0'; // remove '\r'

			if (!(i < N)) {
				printf("Error: File %s has more items than expected. Expected %d\n", filename, N);
				fclose(fin);
			}
			S[i] = atoi(line);
			i++;
		}
		fclose(fin);
		if (i != N) {
			printf("Error: Expect number of items in set S is %d, got %d\n", N, i);
		}
	}
	else { // generate random S
		printf("Generating random S\n");
		srand(time(NULL));
		
		for (i = 0; i < N; i++) {
			S[i] = rand() % 1000;
		}
	}

	// printf("S: ");
	// for (i = 0; i < N; i++) {
	// 	printf("%d ", S[i]);
	// }
	// printf("\n\n");


	int found = 0;
	clock_t initial = clock();
	for (int i = 0; i < N; i++) {
		if (S[i] == x) {
			printf("%d found in position %d.\n", x, i);
			found = 1;
			break;
		}
	}

	if (!found) {
		printf("%d not found.\n", x);
	}

    clock_t final = clock();
    double result = ((double)(final-initial)/CLOCKS_PER_SEC);
	printf("Calculation Execution Time: %lf\n", result);

	return 0;
}