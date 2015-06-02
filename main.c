/*
 ============================================================================
 Name        : main.c
 Author      : Jason Su and Harry He
 Version     : 1.0.0
 Copyright   : GNU
 Description : 1024*1024 double-precision matrix multliplication
 ============================================================================
 */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include <immintrin.h>
#include <xmmintrin.h>

#include "header.h"

/* Return the current system time in micro seconds */
static inline uint64_t timestamp_us()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return 1000000L * tv.tv_sec + tv.tv_usec;
}

/* invalid all caches, including L1, L2, L3 and L4 */
void invalid_caches(char* large_array) {
	#pragma omp parallel
	{
		for (int i = 0; i < L1_SIZE+L2_SIZE+L3_SIZE+L4_SIZE; i += LINE_SIZE) {
			large_array[i]++;
		}
	}

}

int main(int argc, char *argv[])
{
	srand (time(NULL));
	omp_set_num_threads(8);

	char* large_array = _mm_malloc(L1_SIZE+L2_SIZE+L3_SIZE+L4_SIZE, 64);
	double* matrix1 = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
	double* matrix2 = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
	double* reference = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);

	/* Initialize matrix with random double-precision float number in (0,1) range */
	for(int i = 0; i < WIDTH*HEIGHT; i++)
	{
		matrix1[i] = ((double)rand())/RAND_MAX;
		matrix2[i] = ((double)rand())/RAND_MAX;
	}

	/* Calculate the reference */
	naive(reference, matrix1, matrix2);


	double* results[NUM_OF_OPTIMIZATIONS] = {}; /* Store the outputs of optimizations */
	double  times[NUM_OF_OPTIMIZATIONS] = {}; /* Store the time measurement of optimizations */
	double  errors[NUM_OF_OPTIMIZATIONS] = {}; /* Store whether the optimization has the correct result */

	void (*functions[NUM_OF_OPTIMIZATIONS])(double*, const double*, const double*) ={
		 naive,
		 openmp,
		 simd,
		 cacheBlock,
		 loopUnroll,
		 registerBlock,
		 openmp_simd,
		 openmp_simd_cacheBlock,
		 openmp_simd_cacheBlock_loopUnroll,
		 openmp_simd_cacheBlock_loopUnroll_registerBlock};

	const char*   names[NUM_OF_OPTIMIZATIONS] ={
		 "naive",
		 "openmp",
		 "simd",
		 "cache block",
		 "loop unroll",
		 "register block",
		 "openmp & simd",
		 "openmp & simd & cache block",
		 "openmp & simd & cache block & loop unroll",
		 "openmp & simd & cache block & loop unroll & register block"};

	const int enables[NUM_OF_OPTIMIZATIONS] = {
		 ENABLE,
		 ENABLE,
		 ENABLE,
		 ENABLE,
		 ENABLE,
		 ENABLE,
	     ENABLE,
		 ENABLE,
		 ENABLE,
		 ENABLE};

	/* Do multiple experiments. Measure the average runtime. */
	const int NUM_OF_EXPERIMENTS = 5;

	for (int i = 0; i < NUM_OF_EXPERIMENTS; i++) {

		for (int j = 0; j < NUM_OF_OPTIMIZATIONS; j++) {
			if (enables[j]) {
				results[j] = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
				invalid_caches(large_array);
				uint64_t start = timestamp_us();
				functions[j](results[j], matrix1, matrix2);
				times[j] += (timestamp_us() - start) / 1000000.0 / NUM_OF_EXPERIMENTS;
				if (i == 0)
					errors[j] = compare_matrix(results[j], reference);
			}
		}

		for (int j = 0; j < NUM_OF_OPTIMIZATIONS; j++) {
			if (enables[j]) {
				_mm_free(results[j]);
				results[j] = NULL;
			}
		}
	}

	for (int i = 0; i < NUM_OF_OPTIMIZATIONS; i++) {
		if (enables[i]) {
			if (!enables[0])
				printf("%-65s:%.4f\n", names[i], times[i]);
			else
				printf("%-65s:%.4f speedup: %.4f\n", names[i], times[i], times[0]/times[i]);
		}

		/* Error Handling */
		if (errors[i]) {
			printf ("The result of %s is wrong\n", names[i]);
		}
	}

	/* Clean Up */
	_mm_free(large_array);
	_mm_free(matrix1);
	_mm_free(matrix2);
	_mm_free(reference);

	return 0;
}
