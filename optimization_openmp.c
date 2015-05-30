#include "header.h"

void optimization_openmp(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {
	memset(result, 0, WIDTH*HEIGHT*sizeof(double));
	#pragma omp parallel for
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			for (int k = 0; k < WIDTH; k++)
			{
				result[i*WIDTH+j] += matrix1[i*WIDTH+k]*matrix2[k*WIDTH+j];
			}
		}
	}
}