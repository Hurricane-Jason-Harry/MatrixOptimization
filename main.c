/*
 ============================================================================
 Name        : main.c
 Author      : Jason Su and Harry He
 Version     : 1.0.0
 Copyright   : GNU
 Description : 1024*1024 double-precision matrix multiply
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
#include <papi.h>

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
	srand (time(NULL));
	omp_set_num_threads(8);

	double* matrices1[NUM_OF_OPTIMIZATIONS] = {}; /* Store the data of matrix2 */
	double* matrices2[NUM_OF_OPTIMIZATIONS] = {}; /* Store the data of matrix2 */
	double* results[NUM_OF_OPTIMIZATIONS] = {}; /* Store the outputs of optimizations */
	double* references[NUM_OF_OPTIMIZATIONS] = {}; /* Store the reference result of convolution */
	double  times[NUM_OF_OPTIMIZATIONS] = {}; /* Store the time measurement of optimizations */
	int  errors[NUM_OF_OPTIMIZATIONS] = {}; /* Store whether the optimization has the correct result */

	void (*functions[NUM_OF_OPTIMIZATIONS])(double*, const double*, const double*) ={ /* The optimization functions */
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

	const char*   names[NUM_OF_OPTIMIZATIONS] ={ /* optimization names */
		 "naive",
		 "openmp",
		 "simd",
		 "cacheBlock",
		 "loopUnroll",
		 "registerBlock",
		 "openmp&simd",
		 "openmp&simd&cacheBlock",
		 "openmp&simd&cacheBlock&loopUnroll",
		 "openmp&simd&cacheBlock&loopUnroll&registerBlock"};

	int events[NUM_OF_EVENTS] = {PAPI_TOT_INS, PAPI_TOT_CYC, PAPI_BR_CN, PAPI_L1_DCM, PAPI_L3_TCM}; /* PAPI events */
	const char* event_names[NUM_OF_EVENTS] = { /* PAPI event counters */
			"Total instruction executed",
			"Total cycles executed",
			"Conditional branch instructions executed",
			"Level 1 data cache misses",
			"Level 3 data cache misses",
	};
	long long event_values[NUM_OF_EVENTS][NUM_OF_OPTIMIZATIONS] = {}; /* Store the result of PAPI event counters. */

	/* Do multiple experiments. Measure the average runtime. */
	const int NUM_OF_EXPERIMENTS = 20;

	for (int i = 0; i < NUM_OF_EXPERIMENTS; i++) {
		for (int j = 0; j < NUM_OF_OPTIMIZATIONS; j++) {
			matrices1[j] = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
			matrices2[j] = _mm_malloc((WIDTH*HEIGHT)*sizeof(double), 64);
			for (int k = 0; k < WIDTH*HEIGHT; k++) { /* initialize matrix1 and matrix2 with random double-precision numbers */
				matrices1[j][k] = rand()/(double)RAND_MAX;
				matrices2[j][k] = rand()/(double)RAND_MAX;
			}
			results[j] = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);
			references[j] = _mm_malloc(WIDTH*HEIGHT*sizeof(double), 64);

			PAPI_start_counters(events, NUM_OF_EVENTS); /* start performance counter. */

			uint64_t start = timestamp_us();
			functions[j](results[j], matrices1[j], matrices2[j]); /* run the optimization functions. */
			times[j] += (timestamp_us() - start) / 1000000.0 / NUM_OF_EXPERIMENTS; /* Record the time to execute. */
			long long event_read[NUM_OF_EVENTS] = {};
			PAPI_stop_counters(event_read, NUM_OF_EVENTS); /* stop performance counter. Store the result */

			for (int k = 0; k < NUM_OF_EVENTS; k++) {
				event_values[k][j] += event_read[k] / NUM_OF_EXPERIMENTS;
			}

			if (i == 0) {
				reference(references[j], matrices1[j], matrices2[j]);
				errors[j] = compare_matrix(results[j], references[j]);
			}
			_mm_free(matrices1[j]);
			_mm_free(matrices2[j]);
			_mm_free(results[j]);
			_mm_free(references[j]);
		}
	}

	/* Show the result of execution times */
	printf("Total Execution Time:\n");
	for (int i = 0; i < NUM_OF_OPTIMIZATIONS; i++) {
		/* Output time measurement results */
		printf("%-50s:%.5f speedup: %.4f\n", names[i], times[i], times[0]/times[i]);

		/* Error Handling */
		if (errors[i]) {
			printf ("The result of %s is wrong\n", names[i]);
		}
	}
	printf("\n");

	/* Show the data from PAPI */
	for (int i = 0; i < NUM_OF_EVENTS; i++) {
		printf("%s:\n", event_names[i]);
		for (int j = 0; j < NUM_OF_OPTIMIZATIONS; j++) {
				printf("%-50s:%11lld ratio: %.4f\n", names[j], event_values[i][j],
						event_values[i][j] / (double)event_values[i][0]);
		}
		printf("\n");
	}

	return 0;
}
