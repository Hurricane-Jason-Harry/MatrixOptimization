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
	double* volatile result_naive  = calloc(WIDTH*HEIGHT, sizeof(double));
	double* volatile reference = calloc(WIDTH*HEIGHT, sizeof(double));

	/* initialize matrix with random double-precision floating number in (0,1) range */
	srand (time(NULL));

	for(int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			matrix1[i*WIDTH + j] = ((double)rand())/RAND_MAX;
			matrix2[i*WIDTH + j] = ((double)rand())/RAND_MAX;
		}
	}

	/* calculate the reference.*/
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

	uint64_t start = timestamp_us();
	/* calculate the correct result */
	optimization_naive(result_naive, matrix1, matrix2);
	printf("Time: %.6f\n", (timestamp_us() - start) / 1000000.0);
	printf("%d", compare_matrix(result_naive, reference));
	free(matrix1);
	free(matrix2);
	free(result_naive);
	free(reference);
	return 0;
}
