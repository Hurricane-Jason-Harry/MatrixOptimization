#include "header.h"

void optimization_openmp(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {
	#pragma omp parallel
	{
		#pragma omp for
		for (int i = 0; i < WIDTH; i++)
		{
			for (int j = 0; j < HEIGHT; j++)
			{
				double temp = 0;
				for (int k = 0; k < WIDTH; k++)
				{
					temp += matrix1[i*WIDTH+k]*matrix2[k*WIDTH+j];
				}
				result[i*WIDTH+j] = temp;
			}
		}
	}
}
