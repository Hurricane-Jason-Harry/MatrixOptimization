#include "header.h"

void naive(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {
	memset(result, 0, WIDTH*HEIGHT*sizeof(double));
	for (int i = 0; i < WIDTH; i++)
	{
		for (int k = 0; k < WIDTH; k++)
		{
			double t = matrix1[i*WIDTH+k];
			for (int j = 0; j < HEIGHT; j++)
			{
				result[i*WIDTH+j] += t*matrix2[k*WIDTH+j];
			}
		}
	}
}


void openmp(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {
	memset(result, 0, WIDTH*HEIGHT*sizeof(double));
	#pragma omp parallel for
	for (int i = 0; i < WIDTH; i++)
	{
		for (int k = 0; k < WIDTH; k++)
		{
			double t = matrix1[i*WIDTH+k];
			for (int j = 0; j < HEIGHT; j++)
			{
				result[i*WIDTH+j] += t*matrix2[k*WIDTH+j];
			}
		}
	}
}



void simd(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {
	memset(result, 0, WIDTH*HEIGHT*sizeof(double));
	for (int i = 0; i < WIDTH; i++)
	{
		for (int k = 0; k < WIDTH; k++)
		{
			__m256d t = _mm256_broadcast_sd(matrix1+i*WIDTH+k);
			for (int j = 0; j < HEIGHT; j+=4)
			{
				__m256d r = _mm256_load_pd(result+i*WIDTH+j);
				r = _mm256_fmadd_pd(_mm256_load_pd(matrix2+k*WIDTH+j), t, r);
				_mm256_store_pd(result+i*WIDTH+j, r);
			}
		}
	}
}

void cacheBlock(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {
	memset(result, 0, WIDTH*HEIGHT*sizeof(double));
	const int BLOCK1 = 512;
	for (int kk = 0; kk < WIDTH; kk+=BLOCK1) {
		for (int i = 0; i < WIDTH; i++)
		{
			for (int k = kk; k < kk+BLOCK1; k++)
			{
				double t = matrix1[i*WIDTH+k];
				for (int j = 0; j < HEIGHT;j++) {
					result[i*WIDTH+j] += t*matrix2[k*WIDTH+j];
				}
			}
		}
	}
}


void loopUnroll(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {
	memset(result, 0, WIDTH*HEIGHT*sizeof(double));
	for (int i = 0; i < WIDTH; i++)
	{
		double* dest = result + i*WIDTH;
		for (int k = 0; k < WIDTH; k++)
		{
			double t = matrix1[i*WIDTH+k];
			const double* mat2 = matrix2+k*WIDTH;
			for (int j = 0; j < HEIGHT; j+=32)
			{
				dest[j] += t*mat2[j];
				dest[j+1] += t*mat2[j+1];
				dest[j+2] += t*mat2[j+2];
				dest[j+3] += t*mat2[j+3];
				dest[j+4] += t*mat2[j+4];
				dest[j+5] += t*mat2[j+5];
				dest[j+6] += t*mat2[j+6];
				dest[j+7] += t*mat2[j+7];
				dest[j+8] += t*mat2[j+8];
				dest[j+9] += t*mat2[j+9];
				dest[j+10] += t*mat2[j+10];
				dest[j+11] += t*mat2[j+11];
				dest[j+12] += t*mat2[j+12];
				dest[j+13] += t*mat2[j+13];
				dest[j+14] += t*mat2[j+14];
				dest[j+15] += t*mat2[j+15];
				dest[j+16] += t*mat2[j+16];
				dest[j+17] += t*mat2[j+17];
				dest[j+18] += t*mat2[j+18];
				dest[j+19] += t*mat2[j+19];
				dest[j+20] += t*mat2[j+20];
				dest[j+21] += t*mat2[j+21];
				dest[j+22] += t*mat2[j+22];
				dest[j+23] += t*mat2[j+23];
				dest[j+24] += t*mat2[j+24];
				dest[j+25] += t*mat2[j+25];
				dest[j+26] += t*mat2[j+26];
				dest[j+27] += t*mat2[j+27];
				dest[j+28] += t*mat2[j+28];
				dest[j+29] += t*mat2[j+29];
				dest[j+30] += t*mat2[j+30];
				dest[j+31] += t*mat2[j+31];
			}
		}
	}
}

void registerBlock(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {

	memset(result, 0, WIDTH*HEIGHT*sizeof(double));
	for (int i = 0; i < WIDTH; i++)
	{
		const double* mat1 = matrix1+i*WIDTH;
		double* dest = result+i*WIDTH;
		for (int k = 0; k < WIDTH; k+=4)
		{
			double t0 = mat1[k];
			double t1 = mat1[k+1];
			double t2 = mat1[k+2];
			double t3 = mat1[k+3];
			const double* mat2 = matrix2+k*WIDTH;
			for (int j = 0; j < HEIGHT; j+=4) {
				double sum0 = dest[j];
				double sum1 = dest[j+1];
				double sum2 = dest[j+2];
				double sum3 = dest[j+3];
				sum0 += t0*mat2[j];
				sum1 += t0*mat2[j+1];
				sum2 += t0*mat2[j+2];
				sum3 += t0*mat2[j+3];
				sum0 += t1*mat2[j+WIDTH];
				sum1 += t1*mat2[j+WIDTH+1];
				sum2 += t1*mat2[j+WIDTH+2];
				sum3 += t1*mat2[j+WIDTH+3];
				sum0 += t2*mat2[j+WIDTH*2];
				sum1 += t2*mat2[j+WIDTH*2+1];
				sum2 += t2*mat2[j+WIDTH*2+2];
				sum3 += t2*mat2[j+WIDTH*2+3];
				sum0 += t3*mat2[j+WIDTH*3];
				sum1 += t3*mat2[j+WIDTH*3+1];
				sum2 += t3*mat2[j+WIDTH*3+2];
				sum3 += t3*mat2[j+WIDTH*3+3];
				dest[j] = sum0;
				dest[j+1] = sum1;
				dest[j+2] = sum2;
				dest[j+3] = sum3;
			}
		}
	}
}

void openmp_simd(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {

	memset(result, 0, WIDTH*HEIGHT*sizeof(double));

	#pragma omp parallel for
	for (int i = 0; i < WIDTH; i++)
	{
		for (int k = 0; k < WIDTH; k++)
		{
			__m256d t = _mm256_broadcast_sd(matrix1+i*WIDTH+k);
			for (int j = 0; j < HEIGHT; j+=4)
			{
				__m256d r = _mm256_load_pd(result+i*WIDTH+j);
				r = _mm256_fmadd_pd(_mm256_load_pd(matrix2+k*WIDTH+j), t, r);
				_mm256_store_pd(result+i*WIDTH+j, r);
			}
		}
	}
}

void openmp_simd_cacheBlock(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {

	const int BLOCK = 512;
	memset(result, 0, WIDTH*HEIGHT*sizeof(double));

	#pragma omp parallel
	{
		for (int kk = 0; kk < WIDTH; kk += BLOCK) {
			#pragma omp for
			for (int i = 0; i < WIDTH; i++)
			{
				for (int k = kk; k < kk+BLOCK; k++)
				{
					__m256d t = _mm256_broadcast_sd(matrix1+i*WIDTH+k);
					for (int j = 0; j < HEIGHT; j+=4)
					{
						__m256d r = _mm256_load_pd(result+i*WIDTH+j);
						r = _mm256_fmadd_pd(_mm256_load_pd(matrix2+k*WIDTH+j), t, r);
						_mm256_store_pd(result+i*WIDTH+j, r);
					}
				}
			}
		}
	}
}

void openmp_simd_cacheBlock_loopUnroll(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {

	const int BLOCK = 512;
	memset(result, 0, WIDTH*HEIGHT*sizeof(double));

	#pragma omp parallel
	{
		for (int kk = 0; kk < WIDTH; kk += BLOCK) {
			#pragma omp for
			for (int i = 0; i < WIDTH; i++)
			{
				for (int k = kk; k < kk+BLOCK; k++)
				{
					__m256d t = _mm256_broadcast_sd(matrix1+i*WIDTH+k);
					for (int j = 0; j < HEIGHT; j+=32)
					{
						double* dest = result+i*WIDTH+j;
						const double* mat2 = matrix2+k*WIDTH+j;
						__m256d r;
						r = _mm256_load_pd(dest);
						r = _mm256_fmadd_pd(_mm256_load_pd(mat2), t, r);
						_mm256_store_pd(dest, r);

						r = _mm256_load_pd(dest+4);
						r = _mm256_fmadd_pd(_mm256_load_pd(mat2+4), t, r);
						_mm256_store_pd(dest+4, r);

						r = _mm256_load_pd(dest+8);
						r = _mm256_fmadd_pd(_mm256_load_pd(mat2+8), t, r);
						_mm256_store_pd(dest+8, r);

						r = _mm256_load_pd(dest+12);
						r = _mm256_fmadd_pd(_mm256_load_pd(mat2+12), t, r);
						_mm256_store_pd(dest+12, r);

						r = _mm256_load_pd(dest+16);
						r = _mm256_fmadd_pd(_mm256_load_pd(mat2+16), t, r);
						_mm256_store_pd(dest+16, r);

						r = _mm256_load_pd(dest+20);
						r = _mm256_fmadd_pd(_mm256_load_pd(mat2+20), t, r);
						_mm256_store_pd(dest+20, r);

						r = _mm256_load_pd(dest+24);
						r = _mm256_fmadd_pd(_mm256_load_pd(mat2+24), t, r);
						_mm256_store_pd(dest+24, r);

						r = _mm256_load_pd(dest+28);
						r = _mm256_fmadd_pd(_mm256_load_pd(mat2+28), t, r);
						_mm256_store_pd(dest+28, r);
					}
				}
			}
		}
	}
}

void openmp_simd_cacheBlock_loopUnroll_registerBlock(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {

	const int BLOCK = 512;
	memset(result, 0, WIDTH*HEIGHT*sizeof(double));

	#pragma omp parallel
	{
		for (int kk = 0; kk < WIDTH; kk += BLOCK) {
			#pragma omp for
			for (int i = 0; i < WIDTH; i++)
			{
				const double* mat1 = matrix1+i*WIDTH;
				double* dest = result+i*WIDTH;
				for (int k = kk; k < kk+BLOCK; k+=8)
				{
					const double* mat2 = matrix2+k*WIDTH;
					__m256d t0 = _mm256_broadcast_sd(mat1+k);
					__m256d t1 = _mm256_broadcast_sd(mat1+k+1);
					__m256d t2 = _mm256_broadcast_sd(mat1+k+2);
					__m256d t3 = _mm256_broadcast_sd(mat1+k+3);
					__m256d t4 = _mm256_broadcast_sd(mat1+k+4);
					__m256d t5 = _mm256_broadcast_sd(mat1+k+5);
					__m256d t6 = _mm256_broadcast_sd(mat1+k+6);
					__m256d t7 = _mm256_broadcast_sd(mat1+k+7);

					for (int j = 0; j < HEIGHT; j+= 32)
					{
						__m256d sum0 = _mm256_load_pd(dest+j);
						__m256d sum1 = _mm256_load_pd(dest+j+4);
						__m256d sum2 = _mm256_load_pd(dest+j+8);
						__m256d sum3 = _mm256_load_pd(dest+j+12);
						__m256d sum4 = _mm256_load_pd(dest+j+16);
						__m256d sum5 = _mm256_load_pd(dest+j+20);
						__m256d sum6 = _mm256_load_pd(dest+j+24);
						__m256d sum7 = _mm256_load_pd(dest+j+28);

						sum0 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j), t0, sum0);
						sum1 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+4), t0, sum1);
						sum2 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+8), t0, sum2);
						sum3 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+12), t0, sum3);
						sum4 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+16), t0, sum4);
						sum5 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+20), t0, sum5);
						sum6 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+24), t0, sum6);
						sum7 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+28), t0, sum7);

						sum0 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+WIDTH), t1, sum0);
						sum1 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+4+WIDTH), t1, sum1);
						sum2 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+8+WIDTH), t1, sum2);
						sum3 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+12+WIDTH), t1, sum3);
						sum4 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+16+WIDTH), t1, sum4);
						sum5 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+20+WIDTH), t1, sum5);
						sum6 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+24+WIDTH), t1, sum6);
						sum7 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+28+WIDTH), t1, sum7);

						sum0 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+WIDTH*2), t2, sum0);
						sum1 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+4+WIDTH*2), t2, sum1);
						sum2 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+8+WIDTH*2), t2, sum2);
						sum3 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+12+WIDTH*2), t2, sum3);
						sum4 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+16+WIDTH*2), t2, sum4);
						sum5 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+20+WIDTH*2), t2, sum5);
						sum6 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+24+WIDTH*2), t2, sum6);
						sum7 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+28+WIDTH*2), t2, sum7);

						sum0 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+WIDTH*3), t3, sum0);
						sum1 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+4+WIDTH*3), t3, sum1);
						sum2 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+8+WIDTH*3), t3, sum2);
						sum3 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+12+WIDTH*3), t3, sum3);
						sum4 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+16+WIDTH*3), t3, sum4);
						sum5 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+20+WIDTH*3), t3, sum5);
						sum6 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+24+WIDTH*3), t3, sum6);
						sum7 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+28+WIDTH*3), t3, sum7);

						sum0 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+WIDTH*4), t4, sum0);
						sum1 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+4+WIDTH*4), t4, sum1);
						sum2 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+8+WIDTH*4), t4, sum2);
						sum3 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+12+WIDTH*4), t4, sum3);
						sum4 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+16+WIDTH*4), t4, sum4);
						sum5 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+20+WIDTH*4), t4, sum5);
						sum6 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+24+WIDTH*4), t4, sum6);
						sum7 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+28+WIDTH*4), t4, sum7);

						sum0 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+WIDTH*5), t5, sum0);
						sum1 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+4+WIDTH*5), t5, sum1);
						sum2 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+8+WIDTH*5), t5, sum2);
						sum3 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+12+WIDTH*5), t5, sum3);
						sum4 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+16+WIDTH*5), t5, sum4);
						sum5 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+20+WIDTH*5), t5, sum5);
						sum6 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+24+WIDTH*5), t5, sum6);
						sum7 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+28+WIDTH*5), t5, sum7);

						sum0 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+WIDTH*6), t6, sum0);
						sum1 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+4+WIDTH*6), t6, sum1);
						sum2 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+8+WIDTH*6), t6, sum2);
						sum3 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+12+WIDTH*6), t6, sum3);
						sum4 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+16+WIDTH*6), t6, sum4);
						sum5 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+20+WIDTH*6), t6, sum5);
						sum6 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+24+WIDTH*6), t6, sum6);
						sum7 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+28+WIDTH*6), t6, sum7);

						sum0 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+WIDTH*7), t7, sum0);
						sum1 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+4+WIDTH*7), t7, sum1);
						sum2 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+8+WIDTH*7), t7, sum2);
						sum3 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+12+WIDTH*7), t7, sum3);
						sum4 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+16+WIDTH*7), t7, sum4);
						sum5 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+20+WIDTH*7), t7, sum5);
						sum6 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+24+WIDTH*7), t7, sum6);
						sum7 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+28+WIDTH*7), t7, sum7);

						_mm256_store_pd(dest+j, sum0);
						_mm256_store_pd(dest+j+4, sum1);
						_mm256_store_pd(dest+j+8, sum2);
						_mm256_store_pd(dest+j+12, sum3);
						_mm256_store_pd(dest+j+16, sum4);
						_mm256_store_pd(dest+j+20, sum5);
						_mm256_store_pd(dest+j+24, sum6);
						_mm256_store_pd(dest+j+28, sum7);
					}
				}
			}
		}
	}
}
