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

	uint64_t start;

	double* results[NUM_OF_OPTIMIZATIONS] = {};
	double times[NUM_OF_OPTIZATIONS] = {}
	double naive_time = 0, openmp_time = 0, simd_time = 0, cache_block_time = 0,
		loop_unroll_time = 0 , register_block_time = 0, openmp_simd_time = 0,
		openmp_simd_cache_block_time = 0,
		openmp_simd_cache_block_loop_unroll_time = 0,
		openmp_simd_cache_register_block_loop_unroll_time = 0;
	int openmp_error = 0, simd_error = 0, cache_block_error = 0, loop_unroll_error = 0,
		register_block_error = 0, openmp_simd_error = 0,
		openmp_simd_cache_block_error = 0,
		openmp_simd_cache_block_loop_unroll_error = 0,
		openmp_simd_cache_register_block_loop_unroll_error = 0;
	double* naive_result;
	double* openmp_result;
	double* simd_result;
	double* cache_block_result;
	double* loop_unroll_result;
	double* register_block_result;
	double* openmp_simd_result;
	double* openmp_simd_cache_block_result;
	double* openmp_simd_cache_block_loop_unroll_result;
	double* openmp_simd_cache_register_block_loop_unroll_result;

	int openmp_enabled = 1,
		simd_enabled = 1,
		cache_block_enabled = 1,
		loop_unroll_enabled = 1,
		register_block_enabled = 1,
		openmp_simd_enabled = 1,
		openmp_simd_cache_block_enabled = 1,
		openmp_simd_cache_block_loop_unroll_enabled = 1,
		openmp_simd_cache_register_block_loop_unroll_enabled = 1;

	const int NUM_OF_EXPERIMENTS = 5;
	for (int i = 0; i < NUM_OF_EXPERIMENTS; i++) {

		double* matrix1 = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
		double* matrix2 = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);

		/* Initialize matrix with random double-precision float number in (0,1) range */
		for(int i = 0; i < WIDTH; i++)
		{
			for (int j = 0; j < HEIGHT; j++)
			{
				matrix1[i*WIDTH + j] = ((double)rand())/RAND_MAX;
				matrix2[i*WIDTH + j] = ((double)rand())/RAND_MAX;
			}
		}

		/* Do calculations */
		naive_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
		invalid_caches(large_array);
		start = timestamp_us();
		optimization_naive(naive_result, matrix1, matrix2);
		naive_time += (timestamp_us() - start) / 1000000.0;

		if (openmp_enabled) {
			openmp_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
			invalid_caches(large_array);
			start = timestamp_us();
			optimization_openmp(openmp_result, matrix1, matrix2);
			openmp_time += (timestamp_us() - start) / 1000000.0;
			if (i == 0)
				openmp_error = compare_matrix(openmp_result, naive_result);
		}

		if (simd_enabled) {
			simd_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
			invalid_caches(large_array);
			start = timestamp_us();
			optimization_simd(simd_result, matrix1, matrix2);
			simd_time += (timestamp_us() - start) / 1000000.0;
			if (i == 0)
				simd_error = compare_matrix(simd_result, naive_result);
		}

		if (cache_block_enabled) {
			cache_block_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
			invalid_caches(large_array);
			start = timestamp_us();
			optimization_cache_block(cache_block_result, matrix1, matrix2);
			cache_block_time += (timestamp_us() - start) / 1000000.0;
			if (i == 0)
				cache_block_error = compare_matrix(cache_block_result, naive_result);
		}

		if (loop_unroll_enabled) {
			loop_unroll_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
			invalid_caches(large_array);
			start = timestamp_us();
			optimization_loop_unroll(loop_unroll_result, matrix1, matrix2);
			loop_unroll_time += (timestamp_us() - start) / 1000000.0;
			if (i == 0)
				loop_unroll_error = compare_matrix(loop_unroll_result, naive_result);
		}

		if (register_block_enabled) {
			register_block_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
			invalid_caches(large_array);
			start = timestamp_us();
			optimization_register_block(register_block_result, matrix1, matrix2);
			register_block_time += (timestamp_us() - start) / 1000000.0;
			if (i == 0)
				register_block_error = compare_matrix(register_block_result, naive_result);
		}

		if (openmp_simd_enabled) {
			openmp_simd_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
			invalid_caches(large_array);
			start = timestamp_us();
			optimization_openmp_simd(openmp_simd_result, matrix1, matrix2);
			openmp_simd_time += (timestamp_us() - start) / 1000000.0;
			if (i == 0)
				openmp_simd_error = compare_matrix(openmp_simd_result, naive_result);
		}

		if (openmp_simd_cache_block_enabled) {
			openmp_simd_cache_block_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
			invalid_caches(large_array);
			start = timestamp_us();
			optimization_openmp_simd_cache_block(openmp_simd_cache_block_result, matrix1, matrix2);
			openmp_simd_cache_block_time += (timestamp_us() - start) / 1000000.0;
			if (i == 0)
				openmp_simd_cache_block_error = compare_matrix(openmp_simd_cache_block_result, naive_result);
		}

		if (openmp_simd_cache_block_loop_unroll_enabled) {
			openmp_simd_cache_block_loop_unroll_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
			invalid_caches(large_array);
			start = timestamp_us();
			optimization_openmp_simd_cache_block_loop_unroll(openmp_simd_cache_block_loop_unroll_result, matrix1, matrix2);
			openmp_simd_cache_block_loop_unroll_time += (timestamp_us() - start) / 1000000.0;
			if (i == 0)
				openmp_simd_cache_block_loop_unroll_error = compare_matrix(openmp_simd_cache_block_loop_unroll_result, naive_result);

		}

		if (openmp_simd_cache_register_block_loop_unroll_enabled) {
			openmp_simd_cache_register_block_loop_unroll_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
			invalid_caches(large_array);
			start = timestamp_us();
			optimization_openmp_simd_cache_register_block_loop_unroll(openmp_simd_cache_register_block_loop_unroll_result, matrix1, matrix2);
			openmp_simd_cache_register_block_loop_unroll_time += (timestamp_us() - start) / 1000000.0;
			if (i == 0)
				openmp_simd_cache_register_block_loop_unroll_error = compare_matrix(openmp_simd_cache_register_block_loop_unroll_result, naive_result);
		}

		if (openmp_enabled)
			_mm_free(openmp_result);
		if (simd_enabled)
			_mm_free(simd_result);
		if (cache_block_enabled)
			_mm_free(cache_block_result);
		if (loop_unroll_enabled)
			_mm_free(loop_unroll_result);
		if (register_block_enabled)
			_mm_free(register_block_result);
		if (openmp_simd_enabled)
			_mm_free(openmp_simd_result);
		if (openmp_simd_cache_block_enabled)
			_mm_free(openmp_simd_cache_block_result);
		if (openmp_simd_cache_block_loop_unroll_enabled)
			_mm_free(openmp_simd_cache_block_loop_unroll_result);
		if (openmp_simd_cache_register_block_loop_unroll_enabled)
			_mm_free(openmp_simd_cache_register_block_loop_unroll_result);

		_mm_free(naive_result);
		_mm_free(matrix1);
		_mm_free(matrix2);
	}

	/* average the results */
	naive_time /= NUM_OF_EXPERIMENTS;
	openmp_time /= NUM_OF_EXPERIMENTS;
	simd_time /= NUM_OF_EXPERIMENTS;
	cache_block_time /= NUM_OF_EXPERIMENTS;
	loop_unroll_time /= NUM_OF_EXPERIMENTS;
	register_block_time /= NUM_OF_EXPERIMENTS;
	openmp_simd_time /= NUM_OF_EXPERIMENTS;
	openmp_simd_cache_block_time /= NUM_OF_EXPERIMENTS;
	openmp_simd_cache_block_loop_unroll_time /= NUM_OF_EXPERIMENTS;
	openmp_simd_cache_register_block_loop_unroll_time /= NUM_OF_EXPERIMENTS;

	/* output benchmark result */
	printf("%-60s%.6f\n", "naive:", naive_time);
	if (openmp_enabled)
		printf("%-60s%.6f speedup: %.6f\n", "openmp:", openmp_time, naive_time/openmp_time);
	if (simd_enabled)
		printf("%-60s%.6f speedup: %.6f\n", "simd:", simd_time, naive_time/simd_time);
	if (cache_block_enabled)
		printf("%-60s%.6f speedup: %.6f\n", "cache block:", cache_block_time, naive_time/cache_block_time);
	if (loop_unroll_enabled)
		printf("%-60s%.6f speedup: %.6f\n", "loop unroll:", loop_unroll_time, naive_time/loop_unroll_time);
	if (register_block_enabled)
		printf("%-60s%.6f speedup: %.6f\n", "register block:", register_block_time, naive_time/register_block_time);
	if (openmp_simd_enabled)
		printf("%-60s%.6f speedup: %.6f\n", "openmp+simd:", openmp_simd_time, naive_time/openmp_simd_time);
	if (openmp_simd_cache_block_enabled)
		printf("%-60s%.6f speedup: %.6f\n", "openmp+simd+cache block:", openmp_simd_cache_block_time, naive_time/openmp_simd_cache_block_time);
	if (openmp_simd_cache_block_loop_unroll_enabled)
			printf("%-60s%.6f speedup: %.6f\n", "openmp+simd+cache block+loop unroll:", openmp_simd_cache_block_loop_unroll_time, naive_time/openmp_simd_cache_block_loop_unroll_time);
	if (openmp_simd_cache_register_block_loop_unroll_enabled)
		printf("%-60s%.6f speedup: %.6f\n", "openmp+simd+cache block+loop unroll+register block:", openmp_simd_cache_register_block_loop_unroll_time, naive_time/openmp_simd_cache_register_block_loop_unroll_time);

	/* Error handl*/
	if (openmp_error) {
		printf("The result of openmp is wrong\n");
	}
	if (simd_error) {
		printf("The result of simd is wrong\n");
	}
	if (cache_block_error) {
		printf("The result of cache block is wrong\n");
	}
	if (loop_unroll_error) {
		printf("The result of loop unroll is wrong\n");
	}
	if (register_block_error) {
		printf("The result of register block is wrong\n");
	}
	if (openmp_simd_error) {
		printf("The result of openmp simd is wrong\n");
	}
	if (openmp_simd_cache_block_error) {
		printf("The result of openmp+simd+cache block is wrong\n");
	}
	if (openmp_simd_cache_block_loop_unroll_error) {
		printf("The result of openmp+simd+cache block+loop unroll is wrong\n");
	}
	if (openmp_simd_cache_register_block_loop_unroll_error) {
		printf("The result of openmp+simd+cache block+loop unroll+register block is wrong\n");
	}


	_mm_free(large_array);

	return 0;
}
