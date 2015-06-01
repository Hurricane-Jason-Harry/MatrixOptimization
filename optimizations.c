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

void optimization_cache_blocking(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {

	const int BLOCK = 8;
	memset(result, 0, WIDTH*HEIGHT*sizeof(double));

	for (int i = 0; i < WIDTH; i++)
	{
		for (int w = 0; w < WIDTH; w += BLOCK) {
			for (int j = 0; j < HEIGHT; j++) {
				double sum = result[i*WIDTH+j];
				for (int k = w; k < w + BLOCK; k++)
				{
					sum += matrix1[i*WIDTH+k]*matrix2[k*WIDTH+j];
				}
				result[i*WIDTH+j] = sum;
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
			for (int k = 0; k < WIDTH; k+=4)
			{
				int iwk = i*WIDTH+k;
				int kw = k*WIDTH;
				sum += matrix1[iwk]*matrix2[kw+j];
				sum += matrix1[iwk+1]*matrix2[kw+WIDTH+j];
				sum += matrix1[iwk+2]*matrix2[kw+2*WIDTH+j];
				sum += matrix1[iwk+3]*matrix2[kw+3*WIDTH+j];
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

	const int BLOCK = 8;
	memset(result, 0, WIDTH*HEIGHT*sizeof(double));

	#pragma omp parallel
	{
		#pragma omp for
		for (int i = 0; i < WIDTH; i++) {
				for (int w = 0; w < WIDTH; w += BLOCK)
				{
					for (int j = 0; j < HEIGHT; j += 4)
					{
						__m256d sum = _mm256_load_pd(result+i*WIDTH+j);
						for (int k = w; k < w+BLOCK; k++)
						{
							__m256d m1 = _mm256_broadcast_sd(matrix1+i*WIDTH+k);
							__m256d m2 = _mm256_load_pd(matrix2+k*WIDTH+j);
							sum = _mm256_fmadd_pd(m1, m2, sum);
						}
						_mm256_store_pd(result+i*WIDTH+j, sum);
					}

				}
			}
	}
}

void optimization_openmp_simd_cache_register_blocking(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {

	memset(result, 0, WIDTH*HEIGHT*sizeof(double));

	const int BLOCK = 16;
	const int BLOCK2 = 56;

	#pragma omp parallel
	{
		#pragma omp for
		for (int i = 0; i < WIDTH; i++) {
			for (int w = 0; w < WIDTH; w += BLOCK)
			{
				for (int j = 0; j < HEIGHT/BLOCK2*BLOCK2; j += BLOCK2) {
						double* dest = result+i*WIDTH+j;
						__m256d sum0 = _mm256_load_pd(dest);
						__m256d sum1 = _mm256_load_pd(dest+4);
						__m256d sum2 = _mm256_load_pd(dest+8);
						__m256d sum3 = _mm256_load_pd(dest+12);
						__m256d sum4 = _mm256_load_pd(dest+16);
						__m256d sum5 = _mm256_load_pd(dest+20);
						__m256d sum6 = _mm256_load_pd(dest+24);
						__m256d sum7 = _mm256_load_pd(dest+28);
						__m256d sum8 = _mm256_load_pd(dest+32);
						__m256d sum9 = _mm256_load_pd(dest+36);
						__m256d sum10 = _mm256_load_pd(dest+40);
						__m256d sum11 = _mm256_load_pd(dest+44);
						__m256d sum12 = _mm256_load_pd(dest+48);
						__m256d sum13 = _mm256_load_pd(dest+52);

						for (int k = w; k < w+BLOCK; k++)
						{
							__m256d m1 = _mm256_broadcast_sd(matrix1+i*WIDTH+k);
							__m256d m2;
							const double* src = matrix2+k*WIDTH+j;

							m2 = _mm256_load_pd(src);
							sum0 = _mm256_fmadd_pd(m1, m2, sum0);
							m2 = _mm256_load_pd(src+4);
							sum1 = _mm256_fmadd_pd(m1, m2, sum1);
							m2 = _mm256_load_pd(src+8);
							sum2 = _mm256_fmadd_pd(m1, m2, sum2);
							m2 = _mm256_load_pd(src+12);
							sum3 = _mm256_fmadd_pd(m1, m2, sum3);
							m2 = _mm256_load_pd(src+16);
							sum4 = _mm256_fmadd_pd(m1, m2, sum4);
							m2 = _mm256_load_pd(src+20);
							sum5 = _mm256_fmadd_pd(m1, m2, sum5);
							m2 = _mm256_load_pd(src+24);
							sum6 = _mm256_fmadd_pd(m1, m2, sum6);
							m2 = _mm256_load_pd(src+28);
							sum7 = _mm256_fmadd_pd(m1, m2, sum7);
							m2 = _mm256_load_pd(src+32);
							sum8 = _mm256_fmadd_pd(m1, m2, sum8);
							m2 = _mm256_load_pd(src+36);
							sum9 = _mm256_fmadd_pd(m1, m2, sum9);
							m2 = _mm256_load_pd(src+40);
							sum10 = _mm256_fmadd_pd(m1, m2, sum10);
							m2 = _mm256_load_pd(src+44);
							sum11 = _mm256_fmadd_pd(m1, m2, sum11);
							m2 = _mm256_load_pd(src+48);
							sum12 = _mm256_fmadd_pd(m1, m2, sum12);
							m2 = _mm256_load_pd(src+52);
							sum13 = _mm256_fmadd_pd(m1, m2, sum13);
						}
						_mm256_store_pd(dest, sum0);
						_mm256_store_pd(dest+4, sum1);
						_mm256_store_pd(dest+8, sum2);
						_mm256_store_pd(dest+12, sum3);
						_mm256_store_pd(dest+16, sum4);
						_mm256_store_pd(dest+20, sum5);
						_mm256_store_pd(dest+24, sum6);
						_mm256_store_pd(dest+28, sum7);
						_mm256_store_pd(dest+32, sum8);
						_mm256_store_pd(dest+36, sum9);
						_mm256_store_pd(dest+40, sum10);
						_mm256_store_pd(dest+44, sum11);
						_mm256_store_pd(dest+48, sum12);
						_mm256_store_pd(dest+52, sum13);
				}

				for (int j = HEIGHT/BLOCK2*BLOCK2; j < HEIGHT; j+=4) {
					__m256d sum = _mm256_load_pd(result+i*WIDTH+j);
					for (int k = w; k < w+BLOCK; k++)
					{
						__m256d m1 = _mm256_broadcast_sd(matrix1+i*WIDTH+k);
						__m256d m2 = _mm256_load_pd(matrix2+k*WIDTH+j);
						sum = _mm256_fmadd_pd(m1, m2, sum);
					}
					_mm256_store_pd(result+i*WIDTH+j, sum);
				}
			}
		}
	}
}
