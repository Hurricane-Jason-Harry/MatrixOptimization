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

#define MATRIX_WIDTH 512
#define MATRIX_HEIGHT 512

static inline uint64_t timestamp_us()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return 1000000L * tv.tv_sec + tv.tv_usec;
}

int main(int argc, char *argv[])
{

	double volatile matrix1[MATRIX_WIDTH][MATRIX_HEIGHT];
	double volatile matrix2[MATRIX_WIDTH][MATRIX_HEIGHT];
	double volatile result[MATRIX_WIDTH][MATRIX_HEIGHT] = {};
	//double result_ref[MATRIX_WIDTH][MATRIX_HEIGHT] = {};

	/* initialize matrix with random double-precision floating number in (0,1) range */
	srand (time(NULL));

	for(int i = 0; i < MATRIX_WIDTH; i++)
	{
		for (int j = 0; j < MATRIX_HEIGHT; j++)
		{
			matrix1[i][j] = ((double)rand())/RAND_MAX;
			matrix2[i][j] = ((double)rand())/RAND_MAX;
		}
	}

	uint64_t start = timestamp_us();

	/* calculate the correct result */
	for (int i = 0; i < MATRIX_WIDTH; i++)
	{
		for (int j = 0; j < MATRIX_HEIGHT; j++)
		{
			for (int k = 0; k < MATRIX_WIDTH; k++)
			{
				result[i][j] += matrix1[i][k] * matrix2[k][j];
			}
		}
	}
	printf("Time: %.6f\n", (timestamp_us() - start) / 1000000.0);
	return 0;
}
