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
	double* volatile result  = calloc(WIDTH*HEIGHT, sizeof(double));
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

	uint64_t start = timestamp_us();

	/* calculate the correct result */
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			for (int k = 0; k < WIDTH; k++)
			{
				result[i*WIDTH+j] += matrix1[i*WIDTH+k]*matrix2[k*WIDTH+j];
				reference[i*WIDTH+j] += matrix1[i*WIDTH+k]*matrix2[k*WIDTH+j];
			}
		}
	}
	printf("Time: %.6f\n", (timestamp_us() - start) / 1000000.0);
	printf("%d", compare_matrix(result, reference));
	free(matrix1);
	free(matrix2);
	free(result);
	free(reference);
	return 0;
}
