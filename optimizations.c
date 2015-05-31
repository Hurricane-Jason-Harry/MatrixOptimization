#include "header.h"

void optimization_naive(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {
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



#define WIDTH_BLOCK 32
#define HEIGHT_BLOCK 32

void optimization_cache_blocking(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {

	memset(result, 0, WIDTH*HEIGHT*sizeof(double));
	for (int w = 0; w < WIDTH; w += WIDTH_BLOCK) {
		for (int h = 0; h < HEIGHT; h += HEIGHT_BLOCK) {
			for (int i = 0; i < WIDTH; i++)
			{
				for (int j = h; j < h + HEIGHT_BLOCK; j++)
				{
					double temp = result[i*WIDTH+j];
					for (int k = w; k < w + WIDTH_BLOCK; k++)
					{
						temp += matrix1[i*WIDTH+k]*matrix2[k*WIDTH+j];
					}
					result[i*WIDTH+j] = temp;
				}
			}
		}
	}
}

