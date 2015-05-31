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

int main(int argc, char *argv[])
{
	double* matrix1 = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
	double* matrix2 = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);

	/* Initialize matrix with random double-precision floating number in (0,1) range */
	srand (time(NULL));
	omp_set_num_threads(8);

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
		loop_unrolling_time, register_blocking_time, openmp_simd_time;
	int openmp_error, simd_error, cache_blocking_error, loop_unrolling_error,
		register_blocking_error, openmp_simd_error;
	double* naive_result;
	double* openmp_result;
	double* simd_result;
	double* cache_blocking_result;
	double* loop_unrolling_result;
	double* register_blocking_result;
	double* openmp_simd_result;

	/* Do calculations */
	naive_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
	start = timestamp_us();
	optimization_naive(naive_result, matrix1, matrix2);
	naive_time = (timestamp_us() - start) / 1000000.0;
	printf("%-20s%.6f\n", "naive:", naive_time);


	openmp_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
	start = timestamp_us();
	optimization_openmp(openmp_result, matrix1, matrix2);
	openmp_time = (timestamp_us() - start) / 1000000.0;
	printf("%-20s%.6f speedup: %.6f\n", "openmp:", openmp_time, naive_time/openmp_time);
	openmp_error = compare_matrix(openmp_result, naive_result);
	_mm_free(openmp_result);

	simd_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
	start = timestamp_us();
	optimization_simd(simd_result, matrix1, matrix2);
	simd_time = (timestamp_us() - start) / 1000000.0;
	printf("%-20s%.6f speedup: %.6f\n", "simd:", simd_time, naive_time/simd_time);
	simd_error = compare_matrix(simd_result, naive_result);
	_mm_free(simd_result);

	cache_blocking_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
	start = timestamp_us();
	optimization_cache_blocking(cache_blocking_result, matrix1, matrix2);
	cache_blocking_time = (timestamp_us() - start) / 1000000.0;
	printf("%-20s%.6f speedup: %.6f\n", "cache blocking:", cache_blocking_time, naive_time/cache_blocking_time);
	cache_blocking_error = compare_matrix(cache_blocking_result, naive_result);
	_mm_free(cache_blocking_result);

	loop_unrolling_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
	start = timestamp_us();
	optimization_loop_unrolling(loop_unrolling_result, matrix1, matrix2);
	loop_unrolling_time = (timestamp_us() - start) / 1000000.0;
	printf("%-20s%.6f speedup: %.6f\n", "loop unrolling:", loop_unrolling_time, naive_time/loop_unrolling_time);
	loop_unrolling_error = compare_matrix(loop_unrolling_result, naive_result);
	_mm_free(loop_unrolling_result);

	register_blocking_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
	start = timestamp_us();
	optimization_register_blocking(register_blocking_result, matrix1, matrix2);
	register_blocking_time = (timestamp_us() - start) / 1000000.0;
	printf("%-20s%.6f speedup: %.6f\n", "register blocking:", register_blocking_time, naive_time/register_blocking_time);
	register_blocking_error = compare_matrix(register_blocking_result, naive_result);
	_mm_free(register_blocking_result);

	openmp_simd_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
	start = timestamp_us();
	optimization_openmp_simd(openmp_simd_result, matrix1, matrix2);
	openmp_simd_time = (timestamp_us() - start) / 1000000.0;
	printf("%-20s%.6f speedup: %.6f\n", "openmp simd:", openmp_simd_time, naive_time/openmp_simd_time);
	openmp_simd_error = compare_matrix(openmp_simd_result, naive_result);
	_mm_free(openmp_simd_result);

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
	if (openmp_simd_error) {
		printf("The result of openmp simd is wrong\n");
	}

	/* Clean up */
	_mm_free(matrix1);
	_mm_free(matrix2);
	_mm_free(naive_result);
	return 0;
}
