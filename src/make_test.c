#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include <errno.h>

/*
 * This program generates matrices to fill its value by random double
 * precision number between 0 and 1. The matrices's size can be determined by the user.
 * Then generate the result matrix by doing matrix multiplication of two matrices.
 * Then program creates a binary data file, recording the information of
 * the width and height and the data of both input matrices and result matrices.
 */


/*
 * usage:
 * make test-file [width1] [height1 and width2] [height] [output file location]
 *
 * width1: optional. The width of first input matrix. must be positive integer. default: 1024
 * height1 and width2: The height of first input matrix and the width of second one.
 * height2: optional. The height of second input matrix. Must be positive integer. default: 1024
 * output file location: optional. must be positive. default: "test.out"
 *
 */

/*
 * output file format:
 * The output file stores the all data in binary.
 * The first 4 bytes record the width of first matrix.
 * The second 4 bytes record the height of first matrix
 * , which is also the width of second matrix.
 * The third 4 bytes record the height of matrices.
 * The following bytes records all double precision (8 bytes) elements of first input
 * matrix continuously. Starting from (0, 0). The next one is (0, 1). The following
 * are (0, 2), ....(0, height1 - 1), (1, 0), ......(1, height1-1), ...
 */


void write_error() {
	fprintf(stderr, "Error while writing");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {

	int w1 = W1;
	int h1w2 = H1W2;
	int h2 = H2;
	char* filename = TEST_FILENAME;

	if (argc >= 2)
		w1 = strtol(argv[0], NULL, 10);
	if (argc >= 3)
		h1w2 = strtol(argv[1], NULL, 10);
	if (argc >= 4)
		h2 = strtol(argv[2], NULL, 10);
	if (argc >= 5)
		filename = argv[3];

	/* allocate memory space for two input matrices and the result matrix. */
	double* input1 = calloc(w1*h1w2, sizeof(double));
	double* input2 = calloc(h1w2*h2, sizeof(double));
	double* result = calloc(w1*h2, sizeof(double));
	srand(time(0));

	for (int i = 0; i < w1*h1w2; i++) {
		input1[i] = rand()/((double)RAND_MAX);
	}

	for (int i = 0; i < w1*h1w2; i++) {
		input2[i] = rand()/((double)RAND_MAX);
	}

	/* calculate the result matrix */
	for (int i = 0; i < h1w2; i++)
	{
		for (int k = 0; k < h2; k++)
		{
			double t = input1[i*w1+k];
			for (int j = 0; j < h2; j++)
			{
				result[i*w1+j] += t*input2[k*h1w2+j];
			}
		}
	}

	FILE* file = fopen(filename, "wb");
	if (!file) write_error();
	if (!fwrite(&w1, sizeof(int), 1, file)) write_error();
	if (!fwrite(&h1w2, sizeof(int), 1, file)) write_error();
	if (!fwrite(&h2, sizeof(int), 1, file)) write_error();
	if (fwrite(input1, sizeof(double), w1*h1w2, file)<w1*h1w2) write_error();
	if (fwrite(input2, sizeof(double), h1w2*h2, file)<h1w2*h2) write_error();
	if (fwrite(result, sizeof(double), w1*h2, file)<w1*h2) write_error();
	if (fclose(file)) write_error();

	free(input1);
	free(input2);
	free(result);
	return EXIT_SUCCESS;
}
