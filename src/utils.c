#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "config.h"

/*
 * Compare two matrices. Return 0 if they are the same, 1 otherwise.
 */
int compare_matrix(const double* sample, const double* reference) {
	for (int i = 0; i < HEIGHT*WIDTH; i++) {
		if (abs(sample[i] - reference[i]) > 1e-10 ||
				!(-1.0e308 < sample[i] && sample[i] < 1.0e308)) {
			printf("difference at pos %d sample value: %f \n"
				   "reference value: %f difference: %f\n", i, sample[i], reference[i],
				   sample[i]-reference[i]);
			return 1;
		}
	}
	return 0;
}

int read_matrix_dimension(const char* filename, int* w1, int* h1w2,
		int* h2) {
	FILE* file = fopen(filename, "rb");
	if (!file) return 1;
	if (!fread(w1, sizeof(int), 1, file)) return 2;
	if (!fread(h1w2, sizeof(int), 1, file)) return 2;
	if (!fread(h2, sizeof(int), 1, file)) return 2;
	if (fclose(file)) return 3;
	return 0;
}

int read_matrix(const char* filename, double* prod, double* matA, double* matB) {
	FILE* file = fopen(filename, "rb");
	fseek(file, 3*sizeof(int), SEEK_SET); // disable for chisel
	if (!file) return 1;
	if (fread(prod, sizeof(double), W1*H2, file) < W1*H2) return 2;
	if (fread(matA, sizeof(double), W1*H1W2, file) < W1*H1W2) return 2;
	if (fread(matB, sizeof(double), H1W2*H2, file) < H1W2*H2) return 2;
	if (fclose(file)) return 3;
	return 0;
}
