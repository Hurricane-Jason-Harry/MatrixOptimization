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
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

#include "config.h"
#include "optimizations.h"
#include "utils.h"

#ifdef __AVX2__
#include <omp.h>
#include <xmmintrin.h>
#include <immintrin.h>
#include <x86intrin.h>
#else
#include "mm_malloc.h"
#endif

int main(int argc, char *argv[])
{
	double* matrix1 = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
	double* matrix2 = _mm_malloc((WIDTH*HEIGHT)*sizeof(double), 64);
	double* result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
	double* result_ref = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
	int read_flag;

	#ifndef OP_CONSTANT_DIMENSION
	read_flag = read_matrix_dimension(TEST_FILENAME, &w1, &w2h1, &h2);
	#else
	int temp;
	read_flag = read_matrix_dimension(TEST_FILENAME, &temp, &temp, &temp);
	#endif

	read_flag = read_matrix(TEST_FILENAME, matrix1, matrix2, result_ref);
	if (read_flag == 1)
		printf("Cannot open test file\n");
	else if (read_flag == 2)
		printf("Error while reading data from test file");
	else if (read_flag == 3)
		printf("Error while closing the test file");
	if (read_flag)
		return 0;

	uint64_t start = timestamp_us();
	optimize(result, matrix1, matrix2); /* run the optimization functions. */
	uint64_t time = timestamp_us() - start;
	if (compare_matrix(result, result_ref)) {
		printf("incorrect");
		return 0;
	}
	printf("%llu", time);
	return 0;
}
