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


void optimization_loop_unrolling(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			double temp = 0;
			for (int k = 0; k < WIDTH; k+=16)
			{
				temp += matrix1[i*WIDTH+k]*matrix2[k*WIDTH+j];
				temp += matrix1[i*WIDTH+k+1]*matrix2[(k+1)*WIDTH+j];
				temp += matrix1[i*WIDTH+k+2]*matrix2[(k+2)*WIDTH+j];
				temp += matrix1[i*WIDTH+k+3]*matrix2[(k+3)*WIDTH+j];
				temp += matrix1[i*WIDTH+k+4]*matrix2[(k+4)*WIDTH+j];
				temp += matrix1[i*WIDTH+k+5]*matrix2[(k+5)*WIDTH+j];
				temp += matrix1[i*WIDTH+k+6]*matrix2[(k+6)*WIDTH+j];
				temp += matrix1[i*WIDTH+k+7]*matrix2[(k+7)*WIDTH+j];
				temp += matrix1[i*WIDTH+k+8]*matrix2[(k+8)*WIDTH+j];
				temp += matrix1[i*WIDTH+k+9]*matrix2[(k+9)*WIDTH+j];
				temp += matrix1[i*WIDTH+k+10]*matrix2[(k+10)*WIDTH+j];
				temp += matrix1[i*WIDTH+k+11]*matrix2[(k+11)*WIDTH+j];
				temp += matrix1[i*WIDTH+k+12]*matrix2[(k+12)*WIDTH+j];
				temp += matrix1[i*WIDTH+k+13]*matrix2[(k+13)*WIDTH+j];
				temp += matrix1[i*WIDTH+k+14]*matrix2[(k+14)*WIDTH+j];
				temp += matrix1[i*WIDTH+k+15]*matrix2[(k+15)*WIDTH+j];
			}
			result[i*WIDTH+j] = temp;
		}
	}
}

