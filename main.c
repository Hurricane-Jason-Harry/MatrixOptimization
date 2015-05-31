/*
 ============================================================================
 Name        : main.c
 Author      : Jason Su and Harry He
 Version     : 1.0.0
 Copyright   : GNU
 Description : 512*512 double-precision matrix multliplication
 ============================================================================
 */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>

#include "header.h"

/* Return the current system time in micro seconds */
static inline uint64_t timestamp_us()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return 1000000L * tv.tv_sec + tv.tv_usec;
}

int main(int argc, char *argv[])
{
	double* matrix1 = malloc(WIDTH*HEIGHT*sizeof(double));
	double* matrix2 = malloc(WIDTH*HEIGHT*sizeof(double));

	/* Initialize matrix with random double-precision floating number in (0,1) range */
	srand (time(NULL));

	for(int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			matrix1[i*WIDTH + j] = ((double)rand())/RAND_MAX;
			matrix2[i*WIDTH + j] = ((double)rand())/RAND_MAX;
		}
	}

	uint64_t start;
	double naive_time, openmp_time, simd_time, cache_blocking_time,
		loop_unrolling_time, register_blocking_time;
	int openmp_error, simd_error, cache_blocking_error, loop_unrolling_error,
		register_blocking_error;
	double* naive_result;
	double* openmp_result;
	double* simd_result;
	double* cache_blocking_result;
	double* loop_unrolling_result;
	double* register_blocking_result;

	/* Do calculations */
	naive_result = malloc(WIDTH*HEIGHT*sizeof(double));
	start = timestamp_us();
	optimization_naive(naive_result, matrix1, matrix2);
	naive_time = (timestamp_us() - start) / 1000000.0;
	printf("%-20s%.6f\n", "naive:", naive_time);


	openmp_result = malloc(WIDTH*HEIGHT*sizeof(double));
	start = timestamp_us();
	optimization_openmp(openmp_result, matrix1, matrix2);
	openmp_time = (timestamp_us() - start) / 1000000.0;
	printf("%-20s%.6f speedup: %.6f\n", "openmp:", openmp_time, naive_time/openmp_time);
	openmp_error = compare_matrix(openmp_result, naive_result);
	free(openmp_result);

	simd_result = malloc(WIDTH*HEIGHT*sizeof(double));
	start = timestamp_us();
	optimization_simd(simd_result, matrix1, matrix2);
	simd_time = (timestamp_us() - start) / 1000000.0;
	printf("%-20s%.6f speedup: %.6f\n", "simd:", simd_time, naive_time/simd_time);
	simd_error = compare_matrix(simd_result, naive_result);
	free(simd_result);

	cache_blocking_result = malloc(WIDTH*HEIGHT*sizeof(double));
	start = timestamp_us();
	optimization_cache_blocking(cache_blocking_result, matrix1, matrix2);
	cache_blocking_time = (timestamp_us() - start) / 1000000.0;
	printf("%-20s%.6f speedup: %.6f\n", "cache blocking:", cache_blocking_time, naive_time/cache_blocking_time);
	cache_blocking_error = compare_matrix(cache_blocking_result, naive_result);
	free(cache_blocking_result);

	loop_unrolling_result = malloc(WIDTH*HEIGHT*sizeof(double));
	start = timestamp_us();
	optimization_loop_unrolling(loop_unrolling_result, matrix1, matrix2);
	loop_unrolling_time = (timestamp_us() - start) / 1000000.0;
	printf("%-20s%.6f speedup: %.6f\n", "loop unrolling:", loop_unrolling_time, naive_time/loop_unrolling_time);
	loop_unrolling_error = compare_matrix(loop_unrolling_result, naive_result);
	free(loop_unrolling_result);

	register_blocking_result = malloc(WIDTH*HEIGHT*sizeof(double));
	start = timestamp_us();
	optimization_register_blocking(register_blocking_result, matrix1, matrix2);
	register_blocking_time = (timestamp_us() - start) / 1000000.0;
	printf("%-20s%.6f speedup: %.6f\n", "register blocking:", register_blocking_time, naive_time/register_blocking_time);
	register_blocking_error = compare_matrix(register_blocking_result, naive_result);
	free(register_blocking_result);

	/* Error handling*/
	if (openmp_error) {
		printf("The result of openmp is wrong\n");
	}
	if (simd_error) {
		printf("The result of simd is wrong\n");
	}
	if (cache_blocking_error) {
		printf("The result of cache blocking is wrong\n");
	}
	if (loop_unrolling_error) {
		printf("The result of loop unrolling is wrong\n");
	}
	if (register_blocking_error) {
		printf("The result of register blocking is wrong\n");
	}

	/* Clean up */
	free(matrix1);
	free(matrix2);
	free(naive_result);
	return 0;
}
