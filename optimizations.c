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
		for (int j = 0; j < HEIGHT; j += 4)
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
		for (int w = 0; w < WIDTH/BLOCK*BLOCK; w += BLOCK) {
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
			for (int k = 0; k < WIDTH; k += 8)
			{
				int iwk = i*WIDTH+k;
				int kwj = k*WIDTH+j;
				sum += matrix1[iwk]*matrix2[kwj];
				sum += matrix1[iwk+1]*matrix2[kwj+WIDTH];
				sum += matrix1[iwk+2]*matrix2[kwj+2*WIDTH];
				sum += matrix1[iwk+3]*matrix2[kwj+3*WIDTH];
				sum += matrix1[iwk+4]*matrix2[kwj+4*WIDTH];
				sum += matrix1[iwk+5]*matrix2[kwj+5*WIDTH];
				sum += matrix1[iwk+6]*matrix2[kwj+6*WIDTH];
				sum += matrix1[iwk+7]*matrix2[kwj+7*WIDTH];
			}
			result[i*WIDTH+j] = sum;
		}
	}
}

void optimization_register_blocking(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {

	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT/10*10; j += 10)
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
			for (int w = 0; w < WIDTH/BLOCK*BLOCK; w += BLOCK)
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

void optimization_openmp_simd_cache_blocking_loop_unrolling(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {

	const int BLOCK = 8;
	memset(result, 0, WIDTH*HEIGHT*sizeof(double));

	#pragma omp parallel
	{
		#pragma omp for
		for (int i = 0; i < WIDTH; i++) {
			for (int w = 0; w < WIDTH/BLOCK*BLOCK; w += BLOCK)
			{
				for (int j = 0; j < HEIGHT; j += 4)
				{
					__m256d sum = _mm256_load_pd(result+i*WIDTH+j);
					const double* mat1 = matrix1+i*WIDTH+w;
					const double* mat2 = matrix2+w*WIDTH+j;
					__m256d m1;
					__m256d m2;
					m1 = _mm256_broadcast_sd(mat1);
					m2 = _mm256_load_pd(mat2);
					sum = _mm256_fmadd_pd(m1, m2, sum);
					m1 = _mm256_broadcast_sd(mat1+1);
					m2 = _mm256_load_pd(mat2+WIDTH);
					sum = _mm256_fmadd_pd(m1, m2, sum);
					m1 = _mm256_broadcast_sd(mat1+2);
					m2 = _mm256_load_pd(mat2+2*WIDTH);
					sum = _mm256_fmadd_pd(m1, m2, sum);
					m1 = _mm256_broadcast_sd(mat1+3);
					m2 = _mm256_load_pd(mat2+3*WIDTH);
					sum = _mm256_fmadd_pd(m1, m2, sum);
					m1 = _mm256_broadcast_sd(mat1+4);
					m2 = _mm256_load_pd(mat2+4*WIDTH);
					sum = _mm256_fmadd_pd(m1, m2, sum);
					m1 = _mm256_broadcast_sd(mat1+5);
					m2 = _mm256_load_pd(mat2+5*WIDTH);
					sum = _mm256_fmadd_pd(m1, m2, sum);
					m1 = _mm256_broadcast_sd(mat1+6);
					m2 = _mm256_load_pd(mat2+6*WIDTH);
					sum = _mm256_fmadd_pd(m1, m2, sum);
					m1 = _mm256_broadcast_sd(mat1+7);
					m2 = _mm256_load_pd(mat2+7*WIDTH);
					sum = _mm256_fmadd_pd(m1, m2, sum);
					_mm256_store_pd(result+i*WIDTH+j, sum);
				}
			}
		}
	}
}

void optimization_openmp_simd_cache_register_blocking_loop_unrolling(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {

	memset(result, 0, WIDTH*HEIGHT*sizeof(double));

	const int BLOCK = 12;
	const int BLOCK2 = 8;

	#pragma omp parallel
	{
		#pragma omp for
		for (int i = 0; i < WIDTH; i++) {
			for (int w = 0; w < WIDTH/BLOCK*BLOCK; w += BLOCK)
			{
				const double* mat1 = matrix1+i*WIDTH+w;
				__m256d m1_0 = _mm256_broadcast_sd(mat1);
				__m256d m1_1 = _mm256_broadcast_sd(mat1+1);
				__m256d m1_2 = _mm256_broadcast_sd(mat1+2);
				__m256d m1_3 = _mm256_broadcast_sd(mat1+3);
				__m256d m1_4 = _mm256_broadcast_sd(mat1+4);
				__m256d m1_5 = _mm256_broadcast_sd(mat1+5);
				__m256d m1_6 = _mm256_broadcast_sd(mat1+6);
				__m256d m1_7 = _mm256_broadcast_sd(mat1+7);
				__m256d m1_8 = _mm256_broadcast_sd(mat1+8);
				__m256d m1_9 = _mm256_broadcast_sd(mat1+9);
				__m256d m1_10 = _mm256_broadcast_sd(mat1+10);
				__m256d m1_11 = _mm256_broadcast_sd(mat1+11);

				for (int j = 0; j < HEIGHT; j += BLOCK2) {

						double* dest = result+i*WIDTH+j;
						__m256d sum0 = _mm256_load_pd(dest);
						__m256d sum1 = _mm256_load_pd(dest+4);

						const double* mat2 = matrix2+w*WIDTH+j;
						__m256d m2_0, m2_1;

						m2_0 = _mm256_load_pd(mat2);
						m2_1 = _mm256_load_pd(mat2+4);
						sum0 = _mm256_fmadd_pd(m1_0, m2_0, sum0);
						sum1 = _mm256_fmadd_pd(m1_0, m2_1, sum1);

						m2_0 = _mm256_load_pd(mat2+WIDTH);
						m2_1 = _mm256_load_pd(mat2+4+WIDTH);
						sum0 = _mm256_fmadd_pd(m1_1, m2_0, sum0);
						sum1 = _mm256_fmadd_pd(m1_1, m2_1, sum1);

						m2_0 = _mm256_load_pd(mat2+WIDTH*2);
						m2_1 = _mm256_load_pd(mat2+4+WIDTH*2);
						sum0 = _mm256_fmadd_pd(m1_2, m2_0, sum0);
						sum1 = _mm256_fmadd_pd(m1_2, m2_1, sum1);

						m2_0 = _mm256_load_pd(mat2+WIDTH*3);
						m2_1 = _mm256_load_pd(mat2+4+WIDTH*3);
						sum0 = _mm256_fmadd_pd(m1_3, m2_0, sum0);
						sum1 = _mm256_fmadd_pd(m1_3, m2_1, sum1);

						m2_0 = _mm256_load_pd(mat2+WIDTH*4);
						m2_1 = _mm256_load_pd(mat2+4+WIDTH*4);
						sum0 = _mm256_fmadd_pd(m1_4, m2_0, sum0);
						sum1 = _mm256_fmadd_pd(m1_4, m2_1, sum1);

						m2_0 = _mm256_load_pd(mat2+WIDTH*5);
						m2_1 = _mm256_load_pd(mat2+4+WIDTH*5);
						sum0 = _mm256_fmadd_pd(m1_5, m2_0, sum0);
						sum1 = _mm256_fmadd_pd(m1_5, m2_1, sum1);

						m2_0 = _mm256_load_pd(mat2+WIDTH*6);
						m2_1 = _mm256_load_pd(mat2+4+WIDTH*6);
						sum0 = _mm256_fmadd_pd(m1_6, m2_0, sum0);
						sum1 = _mm256_fmadd_pd(m1_6, m2_1, sum1);

						m2_0 = _mm256_load_pd(mat2+WIDTH*7);
						m2_1 = _mm256_load_pd(mat2+4+WIDTH*7);
						sum0 = _mm256_fmadd_pd(m1_7, m2_0, sum0);
						sum1 = _mm256_fmadd_pd(m1_7, m2_1, sum1);

						m2_0 = _mm256_load_pd(mat2+WIDTH*8);
						m2_1 = _mm256_load_pd(mat2+4+WIDTH*8);
						sum0 = _mm256_fmadd_pd(m1_8, m2_0, sum0);
						sum1 = _mm256_fmadd_pd(m1_8, m2_1, sum1);

						m2_0 = _mm256_load_pd(mat2+WIDTH*9);
						m2_1 = _mm256_load_pd(mat2+4+WIDTH*9);
						sum0 = _mm256_fmadd_pd(m1_9, m2_0, sum0);
						sum1 = _mm256_fmadd_pd(m1_9, m2_1, sum1);

						m2_0 = _mm256_load_pd(mat2+WIDTH*10);
						m2_1 = _mm256_load_pd(mat2+4+WIDTH*10);
						sum0 = _mm256_fmadd_pd(m1_10, m2_0, sum0);
						sum1 = _mm256_fmadd_pd(m1_10, m2_1, sum1);

						m2_0 = _mm256_load_pd(mat2+WIDTH*11);
						m2_1 = _mm256_load_pd(mat2+4+WIDTH*11);
						sum0 = _mm256_fmadd_pd(m1_11, m2_0, sum0);
						sum1 = _mm256_fmadd_pd(m1_11, m2_1, sum1);

						_mm256_store_pd(dest, sum0);
						_mm256_store_pd(dest+4, sum1);
				}
			}
			for (int w = WIDTH/BLOCK*BLOCK; w < WIDTH; w ++) {
				const double* mat1 = matrix1+i*WIDTH+w;
				__m256d m1_0 = _mm256_broadcast_sd(mat1);
				for (int j = 0; j < HEIGHT; j += BLOCK2) {

						double* dest = result+i*WIDTH+j;
						__m256d sum0 = _mm256_load_pd(dest);
						__m256d sum1 = _mm256_load_pd(dest+4);

						const double* mat2 = matrix2+w*WIDTH+j;
						__m256d m2_0, m2_1;

						m2_0 = _mm256_load_pd(mat2);
						m2_1 = _mm256_load_pd(mat2+4);
						sum0 = _mm256_fmadd_pd(m1_0, m2_0, sum0);
						sum1 = _mm256_fmadd_pd(m1_0, m2_1, sum1);

						_mm256_store_pd(dest, sum0);
						_mm256_store_pd(dest+4, sum1);
				}
			}
		}
	}
}
