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


void prefetch_matrices() {

}

int main(int argc, char *argv[])
{
	srand (time(NULL));
	omp_set_num_threads(8);

	uint64_t start;
	double naive_time = 0, openmp_time = 0, simd_time = 0, cache_blocking_time = 0,
		loop_unrolling_time = 0 , register_blocking_time = 0, openmp_simd_time = 0,
		openmp_simd_cache_blocking_time = 0,
		openmp_simd_cache_blocking_loop_unrolling_time = 0,
		openmp_simd_cache_register_blocking_loop_unrolling_time = 0;
	int openmp_error = 0, simd_error = 0, cache_blocking_error = 0, loop_unrolling_error = 0,
		register_blocking_error = 0, openmp_simd_error = 0,
		openmp_simd_cache_blocking_error = 0,
		openmp_simd_cache_blocking_loop_unrolling_error = 0,
		openmp_simd_cache_register_blocking_loop_unrolling_error = 0;
	double* naive_result;
	double* openmp_result;
	double* simd_result;
	double* cache_blocking_result;
	double* loop_unrolling_result;
	double* register_blocking_result;
	double* openmp_simd_result;
	double* openmp_simd_cache_blocking_result;
	double* openmp_simd_cache_blocking_loop_unrolling_result;
	double* openmp_simd_cache_register_blocking_loop_unrolling_result;

	int openmp_enabled = 1,
		simd_enabled = 1,
		cache_blocking_enabled = 1,
		loop_unrolling_enabled = 1,
		register_blocking_enabled = 1,
		openmp_simd_enabled = 1,
		openmp_simd_cache_blocking_enabled = 1,
		openmp_simd_cache_blocking_loop_unrolling_enabled = 1,
		openmp_simd_cache_register_blocking_loop_unrolling_enabled = 1;

	const int NUM_OF_EXPERIMENTS = 20;
	for (int i = 0; i < NUM_OF_EXPERIMENTS; i++) {

		double* matrix1 = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
		double* matrix2 = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);

		/* Initialize matrix with random double-precision floating number in (0,1) range */
		for(int i = 0; i < WIDTH; i++)
		{
			for (int j = 0; j < HEIGHT; j++)
			{
				matrix1[i*WIDTH + j] = ((double)rand())/RAND_MAX;
				matrix2[i*WIDTH + j] = ((double)rand())/RAND_MAX;
			}
		}

		/* Do calculations */
		for (int i = 0; i < WIDTH*HEIGHT; i += 8) {
			_mm_prefetch(matrix1 + i, _MM_HINT_T2);
			_mm_prefetch(matrix2 + i, _MM_HINT_T2);
		}

		naive_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
		start = timestamp_us();
		optimization_naive(naive_result, matrix1, matrix2);
		naive_time += (timestamp_us() - start) / 1000000.0;

		if (openmp_enabled) {
			for (int i = 0; i < WIDTH*HEIGHT; i += 8) {
				_mm_prefetch(matrix1 + i, _MM_HINT_T2);
				_mm_prefetch(matrix2 + i, _MM_HINT_T2);
			}
			openmp_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
			start = timestamp_us();
			optimization_openmp(openmp_result, matrix1, matrix2);
			openmp_time += (timestamp_us() - start) / 1000000.0;
			if (i == 0)
				openmp_error = compare_matrix(openmp_result, naive_result);
		}

		if (simd_enabled) {
			for (int i = 0; i < WIDTH*HEIGHT; i += 8) {
				_mm_prefetch(matrix1 + i, _MM_HINT_T2);
				_mm_prefetch(matrix2 + i, _MM_HINT_T2);
			}
			simd_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
			start = timestamp_us();
			optimization_simd(simd_result, matrix1, matrix2);
			simd_time += (timestamp_us() - start) / 1000000.0;
			if (i == 0)
				simd_error = compare_matrix(simd_result, naive_result);
		}

		if (cache_blocking_enabled) {
			for (int i = 0; i < WIDTH*HEIGHT; i += 8) {
				_mm_prefetch(matrix1 + i, _MM_HINT_T2);
				_mm_prefetch(matrix2 + i, _MM_HINT_T2);
			}
			cache_blocking_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
			start = timestamp_us();
			optimization_cache_blocking(cache_blocking_result, matrix1, matrix2);
			cache_blocking_time += (timestamp_us() - start) / 1000000.0;
			if (i == 0)
				cache_blocking_error = compare_matrix(cache_blocking_result, naive_result);
		}

		if (loop_unrolling_enabled) {
			for (int i = 0; i < WIDTH*HEIGHT; i += 8) {
				_mm_prefetch(matrix1 + i, _MM_HINT_T2);
				_mm_prefetch(matrix2 + i, _MM_HINT_T2);
			}
			loop_unrolling_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
			start = timestamp_us();
			optimization_loop_unrolling(loop_unrolling_result, matrix1, matrix2);
			loop_unrolling_time += (timestamp_us() - start) / 1000000.0;
			if (i == 0)
				loop_unrolling_error = compare_matrix(loop_unrolling_result, naive_result);
		}

		if (register_blocking_enabled) {
			for (int i = 0; i < WIDTH*HEIGHT; i += 8) {
				_mm_prefetch(matrix1 + i, _MM_HINT_T2);
				_mm_prefetch(matrix2 + i, _MM_HINT_T2);
			}
			register_blocking_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
			start = timestamp_us();
			optimization_register_blocking(register_blocking_result, matrix1, matrix2);
			register_blocking_time += (timestamp_us() - start) / 1000000.0;
			if (i == 0)
				register_blocking_error = compare_matrix(register_blocking_result, naive_result);
		}

		if (openmp_simd_enabled) {
			for (int i = 0; i < WIDTH*HEIGHT; i += 8) {
				_mm_prefetch(matrix1 + i, _MM_HINT_T2);
				_mm_prefetch(matrix2 + i, _MM_HINT_T2);
			}
			openmp_simd_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
			start = timestamp_us();
			optimization_openmp_simd(openmp_simd_result, matrix1, matrix2);
			openmp_simd_time += (timestamp_us() - start) / 1000000.0;
			if (i == 0)
				openmp_simd_error = compare_matrix(openmp_simd_result, naive_result);
		}

		if (openmp_simd_cache_blocking_enabled) {
			for (int i = 0; i < WIDTH*HEIGHT; i += 8) {
				_mm_prefetch(matrix1 + i, _MM_HINT_T2);
				_mm_prefetch(matrix2 + i, _MM_HINT_T2);
			}
			openmp_simd_cache_blocking_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
			start = timestamp_us();
			optimization_openmp_simd_cache_blocking(openmp_simd_cache_blocking_result, matrix1, matrix2);
			openmp_simd_cache_blocking_time += (timestamp_us() - start) / 1000000.0;
			if (i == 0)
				openmp_simd_cache_blocking_error = compare_matrix(openmp_simd_cache_blocking_result, naive_result);
		}

		if (openmp_simd_cache_blocking_loop_unrolling_enabled) {
			for (int i = 0; i < WIDTH*HEIGHT; i += 8) {
				_mm_prefetch(matrix1 + i, _MM_HINT_T2);
				_mm_prefetch(matrix2 + i, _MM_HINT_T2);
			}
			openmp_simd_cache_blocking_loop_unrolling_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
			start = timestamp_us();
			optimization_openmp_simd_cache_blocking_loop_unrolling(openmp_simd_cache_blocking_loop_unrolling_result, matrix1, matrix2);
			openmp_simd_cache_blocking_loop_unrolling_time += (timestamp_us() - start) / 1000000.0;
			if (i == 0)
				openmp_simd_cache_blocking_loop_unrolling_error = compare_matrix(openmp_simd_cache_blocking_loop_unrolling_result, naive_result);

		}

		if (openmp_simd_cache_register_blocking_loop_unrolling_enabled) {
			for (int i = 0; i < WIDTH*HEIGHT; i += 8) {
				_mm_prefetch(matrix1 + i, _MM_HINT_T2);
				_mm_prefetch(matrix2 + i, _MM_HINT_T2);
			}
			openmp_simd_cache_register_blocking_loop_unrolling_result = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
			start = timestamp_us();
			optimization_openmp_simd_cache_register_blocking_loop_unrolling(openmp_simd_cache_register_blocking_loop_unrolling_result, matrix1, matrix2);
			openmp_simd_cache_register_blocking_loop_unrolling_time += (timestamp_us() - start) / 1000000.0;
			if (i == 0)
				openmp_simd_cache_register_blocking_loop_unrolling_error = compare_matrix(openmp_simd_cache_register_blocking_loop_unrolling_result, naive_result);
		}

		if (openmp_enabled)
			_mm_free(openmp_result);
		if (simd_enabled)
			_mm_free(simd_result);
		if (cache_blocking_enabled)
			_mm_free(cache_blocking_result);
		if (loop_unrolling_enabled)
			_mm_free(loop_unrolling_result);
		if (register_blocking_enabled)
			_mm_free(register_blocking_result);
		if (openmp_simd_enabled)
			_mm_free(openmp_simd_result);
		if (openmp_simd_cache_blocking_enabled)
			_mm_free(openmp_simd_cache_blocking_result);
		if (openmp_simd_cache_blocking_loop_unrolling_enabled)
			_mm_free(openmp_simd_cache_blocking_loop_unrolling_result);
		if (openmp_simd_cache_register_blocking_loop_unrolling_enabled)
			_mm_free(openmp_simd_cache_register_blocking_loop_unrolling_result);

		_mm_free(naive_result);
		_mm_free(matrix1);
		_mm_free(matrix2);
	}

	/* average the results */
	naive_time /= NUM_OF_EXPERIMENTS;
	openmp_time /= NUM_OF_EXPERIMENTS;
	simd_time /= NUM_OF_EXPERIMENTS;
	cache_blocking_time /= NUM_OF_EXPERIMENTS;
	loop_unrolling_time /= NUM_OF_EXPERIMENTS;
	register_blocking_time /= NUM_OF_EXPERIMENTS;
	openmp_simd_time /= NUM_OF_EXPERIMENTS;
	openmp_simd_cache_blocking_time /= NUM_OF_EXPERIMENTS;
	openmp_simd_cache_blocking_loop_unrolling_time /= NUM_OF_EXPERIMENTS;
	openmp_simd_cache_register_blocking_loop_unrolling_time /= NUM_OF_EXPERIMENTS;

	/* output benchmark result */
	printf("%-60s%.6f\n", "naive:", naive_time);
	if (openmp_enabled)
		printf("%-60s%.6f speedup: %.6f\n", "openmp:", openmp_time, naive_time/openmp_time);
	if (simd_enabled)
		printf("%-60s%.6f speedup: %.6f\n", "simd:", simd_time, naive_time/simd_time);
	if (cache_blocking_enabled)
		printf("%-60s%.6f speedup: %.6f\n", "cache blocking:", cache_blocking_time, naive_time/cache_blocking_time);
	if (loop_unrolling_enabled)
		printf("%-60s%.6f speedup: %.6f\n", "loop unrolling:", loop_unrolling_time, naive_time/loop_unrolling_time);
	if (register_blocking_enabled)
		printf("%-60s%.6f speedup: %.6f\n", "register blocking:", register_blocking_time, naive_time/register_blocking_time);
	if (openmp_simd_enabled)
		printf("%-60s%.6f speedup: %.6f\n", "openmp+simd:", openmp_simd_time, naive_time/openmp_simd_time);
	if (openmp_simd_cache_blocking_enabled)
		printf("%-60s%.6f speedup: %.6f\n", "openmp+simd+cache blocking:", openmp_simd_cache_blocking_time, naive_time/openmp_simd_cache_blocking_time);
	if (openmp_simd_cache_blocking_loop_unrolling_enabled)
			printf("%-60s%.6f speedup: %.6f\n", "openmp+simd+cache blocking+loop unrolling:", openmp_simd_cache_blocking_loop_unrolling_time, naive_time/openmp_simd_cache_blocking_loop_unrolling_time);
	if (openmp_simd_cache_register_blocking_loop_unrolling_enabled)
		printf("%-60s%.6f speedup: %.6f\n", "openmp+simd+cache blocking+loop unrolling+register blocking:", openmp_simd_cache_register_blocking_loop_unrolling_time, naive_time/openmp_simd_cache_register_blocking_loop_unrolling_time);

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
	if (openmp_simd_cache_blocking_error) {
		printf("The result of openmp+simd+cache blocking is wrong\n");
	}
	if (openmp_simd_cache_blocking_loop_unrolling_error) {
		printf("The result of openmp+simd+cache blocking+loop unrolling is wrong\n");
	}
	if (openmp_simd_cache_register_blocking_loop_unrolling_error) {
		printf("The result of openmp+simd+cache blocking+loop unrolling+register blocking is wrong\n");
	}

	return 0;
}
