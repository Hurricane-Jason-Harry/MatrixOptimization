#include "header.h"

void optimization_naive(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			double sum = 0;
			for (int k = 0; k < WIDTH; k++)
			{
				sum += matrix1[i*WIDTH+k]*matrix2[k*WIDTH+j];
			}
			result[i*WIDTH+j] = sum;
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
				double sum = 0;
				for (int k = 0; k < WIDTH; k++)
				{
					sum += matrix1[i*WIDTH+k]*matrix2[k*WIDTH+j];
				}
				result[i*WIDTH+j] = sum;
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
				__m256d m2 = _mm256_load_pd(matrix2+k*WIDTH+j);
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
					double sum = result[i*WIDTH+j];
					for (int k = w; k < w + WIDTH_BLOCK; k++)
					{
						sum += matrix1[i*WIDTH+k]*matrix2[k*WIDTH+j];
					}
					result[i*WIDTH+j] = sum;
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
			double sum = 0;
			for (int k = 0; k < WIDTH; k+=16)
			{
				int iwk = i*WIDTH+k;
				int kw = k*WIDTH;
				sum += matrix1[iwk]*matrix2[kw+j];
				sum += matrix1[iwk+1]*matrix2[kw+WIDTH+j];
				sum += matrix1[iwk+2]*matrix2[kw+2*WIDTH+j];
				sum += matrix1[iwk+3]*matrix2[kw+3*WIDTH+j];
				sum += matrix1[iwk+4]*matrix2[kw+4*WIDTH+j];
				sum += matrix1[iwk+5]*matrix2[kw+5*WIDTH+j];
				sum += matrix1[iwk+6]*matrix2[kw+6*WIDTH+j];
				sum += matrix1[iwk+7]*matrix2[kw+7*WIDTH+j];
				sum += matrix1[iwk+8]*matrix2[kw+8*WIDTH+j];
				sum += matrix1[iwk+9]*matrix2[kw+9*WIDTH+j];
				sum += matrix1[iwk+10]*matrix2[kw+10*WIDTH+j];
				sum += matrix1[iwk+11]*matrix2[kw+11*WIDTH+j];
				sum += matrix1[iwk+12]*matrix2[kw+12*WIDTH+j];
				sum += matrix1[iwk+13]*matrix2[kw+13*WIDTH+j];
				sum += matrix1[iwk+14]*matrix2[kw+14*WIDTH+j];
				sum += matrix1[iwk+15]*matrix2[kw+15*WIDTH+j];
			}
			result[i*WIDTH+j] = sum;
		}
	}
}

void optimization_register_blocking(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {

	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT/10*10; j+=10)
		{
			double sum0 = 0;
			double sum1 = 0;
			double sum2 = 0;
			double sum3 = 0;
			double sum4 = 0;
			double sum5 = 0;
			double sum6 = 0;
			double sum7 = 0;
			double sum8 = 0;
			double sum9 = 0;
			for (int k = 0; k < WIDTH; k++)
			{
				double m = matrix1[i*WIDTH+k];
				int kwj = k*WIDTH+j;
				sum0 += m*matrix2[kwj];
				sum1 += m*matrix2[kwj+1];
				sum2 += m*matrix2[kwj+2];
				sum3 += m*matrix2[kwj+3];
				sum4 += m*matrix2[kwj+4];
				sum5 += m*matrix2[kwj+5];
				sum6 += m*matrix2[kwj+6];
				sum7 += m*matrix2[kwj+7];
				sum8 += m*matrix2[kwj+8];
				sum9 += m*matrix2[kwj+9];
			}
			int iwj = i*WIDTH+j;
			result[iwj]   = sum0;
			result[iwj+1] = sum1;
			result[iwj+2] = sum2;
			result[iwj+3] = sum3;
			result[iwj+4] = sum4;
			result[iwj+5] = sum5;
			result[iwj+6] = sum6;
			result[iwj+7] = sum7;
			result[iwj+8] = sum8;
			result[iwj+9] = sum9;
		}
	}

	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = HEIGHT/10*10; j < HEIGHT; j++)
		{
			double sum = 0;
			for (int k = 0; k < WIDTH; k++)
			{
				sum += matrix1[i*WIDTH+k]*matrix2[k*WIDTH+j];
			}
			result[i*WIDTH+j] = sum;
		}
	}
}

void optimization_openmp_simd(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {

	#pragma omp parallel
	{
		#pragma omp for
		for (int i = 0; i < WIDTH; i++)
		{
			for (int j = 0; j < HEIGHT; j+= 4)
			{
				__m256d sum = _mm256_setzero_pd();
				for (int k = 0; k < WIDTH; k ++)
				{
					__m256d m1 = _mm256_broadcast_sd(matrix1+i*WIDTH+k);
					__m256d m2 = _mm256_load_pd(matrix2+k*WIDTH+j);
					sum = _mm256_fmadd_pd(m1, m2, sum);
				}
				_mm256_storeu_pd(result+i*WIDTH+j, sum);
			}
		}
	}
}

void optimization_openmp_simd_cache_blocking(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {

	memset(result, 0, WIDTH*HEIGHT*sizeof(double));
	#pragma omp parallel
	{
		#pragma omp for
		for (int i = 0; i < WIDTH; i++) {
			for (int h = 0; h < HEIGHT; h += HEIGHT_BLOCK) {
				for (int w = 0; w < WIDTH; w += WIDTH_BLOCK)
				{
					for (int j = h; j < h+ HEIGHT_BLOCK; j += 4)
					{
						__m256d sum = _mm256_loadu_pd(result+i*WIDTH+j);
						for (int k = w; k < w+WIDTH_BLOCK; k++)
						{
							__m256d m1 = _mm256_broadcast_sd(matrix1+i*WIDTH+k);
							__m256d m2 = _mm256_load_pd(matrix2+k*WIDTH+j);
							sum = _mm256_fmadd_pd(m1, m2, sum);
						}
						_mm256_storeu_pd(result+i*WIDTH+j, sum);
					}
				}
			}
		}
	}
}
