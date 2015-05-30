#include "header.h"

void optimization_simd(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {
	
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j+= 4)
		{
			__m256d sum = _mm256_setzero_pd();
			for (int k = 0; k < WIDTH; k ++)
			{
				__m256d m1 = _mm256_broadcast_sd(matrix1+i*WIDTH+k);
				__m256d m2 = _mm256_loadu_pd(matrix2+k*WIDTH+j);
				sum = _mm256_fmadd_pd(m1, m2, sum);
			}
			_mm256_storeu_pd(result+i*WIDTH+j, sum);
		}
	}
}
