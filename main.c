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
	double* volatile matrix1 = calloc(WIDTH*HEIGHT, sizeof(double));
	double* volatile matrix2 = calloc(WIDTH*HEIGHT, sizeof(double));
	double* volatile result  = calloc(WIDTH*HEIGHT, sizeof(double));
	double* volatile reference = calloc(WIDTH*HEIGHT, sizeof(double));

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

	/* Calculate the reference.*/
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			for (int k = 0; k < WIDTH; k++)
			{
				reference[i*WIDTH+j] += matrix1[i*WIDTH+k]*matrix2[k*WIDTH+j];
			}
		}
	}


	uint64_t start;
	double naive_time, openmp_time, simd_time;
	int naive_error, openmp_error, simd_error;

	/* Do calculations */
	start = timestamp_us();
	/* calculate the correct result */
	optimization_naive(result, matrix1, matrix2);
	naive_time = (timestamp_us() - start) / 1000000.0;
	printf("naive:  %.6f\n", naive_time);
	naive_error = compare_matrix(result, reference);

	start = timestamp_us();
	optimization_openmp(result, matrix1, matrix2);
	openmp_time = (timestamp_us() - start) / 1000000.0;
	printf("openmp: %.6f\n", openmp_time);
	openmp_error = compare_matrix(result, reference);

	start = timestamp_us();
	optimization_simd(result, matrix1, matrix2);
	simd_time = (timestamp_us() - start) / 1000000.0;
	printf("simd: %.6f\n", simd_time);
	simd_error = compare_matrix(result, reference);

	/* Error handling*/
	if (naive_error) {
		printf("The result of naive is wrong\n");
	}
	if (openmp_error) {
		printf("The result of openmp is wrong\n");
	}
	if (simd_error) {
		printf("The result of simd is wrong\n");
	}

	/* Clean up */
	free(matrix1);
	free(matrix2);
	free(result);
	free(reference);
	return 0;
}
