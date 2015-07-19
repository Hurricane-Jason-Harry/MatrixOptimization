/*
 ============================================================================
 Name        : main.c
 Author      : Jason Su and Harry He
 Version     : 1.0.0
 Copyright   : GNU
 Description : 1024*1024 double-precision matrix multiply
 ============================================================================
 */
#include <stdio.h>

#ifdef __AVX2__
#include <xmmintrin.h>
#else
#include "mm_malloc.h"
#endif

#include "config.h"
#include "optimizations.h"
#include "utils.h"

int main(int argc, char *argv[])
{
	double* matA = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
	double* matB = _mm_malloc((WIDTH*HEIGHT)*sizeof(double), 64);
	double* prod = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
	double* prod_ref = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);

	int read_flag = read_matrix(TEST_FILENAME, prod_ref, matA, matB);
	if (read_flag == 1)
		printf("Cannot open test file\n");
	else if (read_flag == 2)
		printf("Error while reading data from test file");
	else if (read_flag == 3)
		printf("Error while closing the test file");
	if (read_flag)
		return 0;

	uint64_t start = timestamp_us();
	matmul_optimize(prod, matA, matB); /* run the optimization functions. */
	uint64_t time = timestamp_us() - start;

	if (compare_matrix(prod, prod_ref)) {
		printf("%lu incorrect\n", time);
	} else {
		printf("%lu\n", time);
	}
	_mm_free(prod_ref);
	_mm_free(prod);
	_mm_free(matB);
	_mm_free(matA);
	return 0;
}
