#include "header.h"

void naive(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {
	memset(result, 0, WIDTH*HEIGHT*sizeof(double));
	for (int i = 0; i < WIDTH; i++)
	{
		for (int k = 0; k < WIDTH; k++)
		{
			double temp = matrix1[i*WIDTH+k];
			for (int j = 0; j < HEIGHT; j++)
			{
				result[i*WIDTH+j] += temp*matrix2[k*WIDTH+j];
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
			double temp = matrix1[i*WIDTH+k];
			for (int j = 0; j < HEIGHT; j++)
			{
				result[i*WIDTH+j] += temp*matrix2[k*WIDTH+j];
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
			__m256d temp = _mm256_broadcast_sd(matrix1+i*WIDTH+k);
			for (int j = 0; j < HEIGHT; j+=4)
			{
				__m256d r = _mm256_load_pd(result+i*WIDTH+j);
				r = _mm256_fmadd_pd(_mm256_load_pd(matrix2+k*WIDTH+j), temp, r);
				_mm256_store_pd(result+i*WIDTH+j, r);
			}
		}
	}
}

void cacheBlock(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {
	memset(result, 0, WIDTH*HEIGHT*sizeof(double));
	const int BLOCK1 = 512;
	for (int kk = 0; kk < WIDTH/BLOCK1*BLOCK1; kk+=BLOCK1) {
		for (int i = 0; i < WIDTH; i++)
		{
			for (int k = kk; k < kk+BLOCK1; k++)
			{
				double temp = matrix1[i*WIDTH+k];
				for (int j = 0; j < HEIGHT;j++) {
					result[i*WIDTH+j] += temp*matrix2[k*WIDTH+j];
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
		for (int k = 0; k < WIDTH; k++)
		{
			double temp = matrix1[i*WIDTH+k];
			for (int j = 0; j < HEIGHT; j+=32)
			{
				result[i*WIDTH+j] += temp*matrix2[k*WIDTH+j];
				result[i*WIDTH+j+1] += temp*matrix2[k*WIDTH+j+1];
				result[i*WIDTH+j+2] += temp*matrix2[k*WIDTH+j+2];
				result[i*WIDTH+j+3] += temp*matrix2[k*WIDTH+j+3];
				result[i*WIDTH+j+4] += temp*matrix2[k*WIDTH+j+4];
				result[i*WIDTH+j+5] += temp*matrix2[k*WIDTH+j+5];
				result[i*WIDTH+j+6] += temp*matrix2[k*WIDTH+j+6];
				result[i*WIDTH+j+7] += temp*matrix2[k*WIDTH+j+7];
				result[i*WIDTH+j+8] += temp*matrix2[k*WIDTH+j+8];
				result[i*WIDTH+j+9] += temp*matrix2[k*WIDTH+j+9];
				result[i*WIDTH+j+10] += temp*matrix2[k*WIDTH+j+10];
				result[i*WIDTH+j+11] += temp*matrix2[k*WIDTH+j+11];
				result[i*WIDTH+j+12] += temp*matrix2[k*WIDTH+j+12];
				result[i*WIDTH+j+13] += temp*matrix2[k*WIDTH+j+13];
				result[i*WIDTH+j+14] += temp*matrix2[k*WIDTH+j+14];
				result[i*WIDTH+j+15] += temp*matrix2[k*WIDTH+j+15];
				result[i*WIDTH+j+16] += temp*matrix2[k*WIDTH+j+16];
				result[i*WIDTH+j+17] += temp*matrix2[k*WIDTH+j+17];
				result[i*WIDTH+j+18] += temp*matrix2[k*WIDTH+j+18];
				result[i*WIDTH+j+19] += temp*matrix2[k*WIDTH+j+19];
				result[i*WIDTH+j+20] += temp*matrix2[k*WIDTH+j+20];
				result[i*WIDTH+j+21] += temp*matrix2[k*WIDTH+j+21];
				result[i*WIDTH+j+22] += temp*matrix2[k*WIDTH+j+22];
				result[i*WIDTH+j+23] += temp*matrix2[k*WIDTH+j+23];
				result[i*WIDTH+j+24] += temp*matrix2[k*WIDTH+j+24];
				result[i*WIDTH+j+25] += temp*matrix2[k*WIDTH+j+25];
				result[i*WIDTH+j+26] += temp*matrix2[k*WIDTH+j+26];
				result[i*WIDTH+j+27] += temp*matrix2[k*WIDTH+j+27];
				result[i*WIDTH+j+28] += temp*matrix2[k*WIDTH+j+28];
				result[i*WIDTH+j+29] += temp*matrix2[k*WIDTH+j+29];
				result[i*WIDTH+j+30] += temp*matrix2[k*WIDTH+j+30];
				result[i*WIDTH+j+31] += temp*matrix2[k*WIDTH+j+31];
			}
		}
	}
}

void registerBlock(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {

	memset(result, 0, WIDTH*HEIGHT*sizeof(double));
	for (int i = 0; i < WIDTH; i++)
	{
		for (int k = 0; k < WIDTH; k+=4)
		{
			double temp0 = matrix1[i*WIDTH+k];
			double temp1 = matrix1[i*WIDTH+k+1];
			double temp2 = matrix1[i*WIDTH+k+2];
			double temp3 = matrix1[i*WIDTH+k+3];
			double* dest = result+i*WIDTH;
			const double* mat2 = matrix2+k*WIDTH;
			for (int j = 0; j < HEIGHT; j+=4) {
				double sum0 = dest[j];
				double sum1 = dest[j+1];
				double sum2 = dest[j+2];
				double sum3 = dest[j+3];
				sum0 += temp0*mat2[j];
				sum1 += temp0*mat2[j+1];
				sum2 += temp0*mat2[j+2];
				sum3 += temp0*mat2[j+3];
				sum0 += temp1*mat2[j+WIDTH];
				sum1 += temp1*mat2[j+WIDTH+1];
				sum2 += temp1*mat2[j+WIDTH+2];
				sum3 += temp1*mat2[j+WIDTH+3];
				sum0 += temp2*mat2[j+WIDTH*2];
				sum1 += temp2*mat2[j+WIDTH*2+1];
				sum2 += temp2*mat2[j+WIDTH*2+2];
				sum3 += temp2*mat2[j+WIDTH*2+3];
				sum0 += temp3*mat2[j+WIDTH*3];
				sum1 += temp3*mat2[j+WIDTH*3+1];
				sum2 += temp3*mat2[j+WIDTH*3+2];
				sum3 += temp3*mat2[j+WIDTH*3+3];
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
			__m256d temp = _mm256_broadcast_sd(matrix1+i*WIDTH+k);
			for (int j = 0; j < HEIGHT; j+=4)
			{
				__m256d r;
				r = _mm256_load_pd(result+i*WIDTH+j);
				r = _mm256_fmadd_pd(_mm256_load_pd(matrix2+k*WIDTH+j), temp, r);
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
					__m256d temp = _mm256_broadcast_sd(matrix1+i*WIDTH+k);
					for (int j = 0; j < HEIGHT; j+=4)
					{
						__m256d r = _mm256_load_pd(result+i*WIDTH+j);
						r = _mm256_fmadd_pd(_mm256_load_pd(matrix2+k*WIDTH+j), temp, r);
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
					__m256d temp = _mm256_broadcast_sd(matrix1+i*WIDTH+k);
					for (int j = 0; j < HEIGHT; j+=32)
					{
						double* dest = result+i*WIDTH+j;
						const double* mat2 = matrix2+k*WIDTH+j;
						__m256d r;
						r = _mm256_load_pd(dest);
						r = _mm256_fmadd_pd(_mm256_load_pd(mat2), temp, r);
						_mm256_store_pd(dest, r);

						r = _mm256_load_pd(dest+4);
						r = _mm256_fmadd_pd(_mm256_load_pd(mat2+4), temp, r);
						_mm256_store_pd(dest+4, r);

						r = _mm256_load_pd(dest+8);
						r = _mm256_fmadd_pd(_mm256_load_pd(mat2+8), temp, r);
						_mm256_store_pd(dest+8, r);

						r = _mm256_load_pd(dest+12);
						r = _mm256_fmadd_pd(_mm256_load_pd(mat2+12), temp, r);
						_mm256_store_pd(dest+12, r);

						r = _mm256_load_pd(dest+16);
						r = _mm256_fmadd_pd(_mm256_load_pd(mat2+16), temp, r);
						_mm256_store_pd(dest+16, r);

						r = _mm256_load_pd(dest+20);
						r = _mm256_fmadd_pd(_mm256_load_pd(mat2+20), temp, r);
						_mm256_store_pd(dest+20, r);

						r = _mm256_load_pd(dest+24);
						r = _mm256_fmadd_pd(_mm256_load_pd(mat2+24), temp, r);
						_mm256_store_pd(dest+24, r);

						r = _mm256_load_pd(dest+28);
						r = _mm256_fmadd_pd(_mm256_load_pd(mat2+28), temp, r);
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
					__m256d temp0 = _mm256_broadcast_sd(mat1+k);
					__m256d temp1 = _mm256_broadcast_sd(mat1+k+1);
					__m256d temp2 = _mm256_broadcast_sd(mat1+k+2);
					__m256d temp3 = _mm256_broadcast_sd(mat1+k+3);
					__m256d temp4 = _mm256_broadcast_sd(mat1+k+4);
					__m256d temp5 = _mm256_broadcast_sd(mat1+k+5);
					__m256d temp6 = _mm256_broadcast_sd(mat1+k+6);
					__m256d temp7 = _mm256_broadcast_sd(mat1+k+7);

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

						sum0 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j), temp0, sum0);
						sum1 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+4), temp0, sum1);
						sum2 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+8), temp0, sum2);
						sum3 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+12), temp0, sum3);
						sum4 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+16), temp0, sum4);
						sum5 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+20), temp0, sum5);
						sum6 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+24), temp0, sum6);
						sum7 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+28), temp0, sum7);

						sum0 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+WIDTH), temp1, sum0);
						sum1 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+4+WIDTH), temp1, sum1);
						sum2 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+8+WIDTH), temp1, sum2);
						sum3 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+12+WIDTH), temp1, sum3);
						sum4 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+16+WIDTH), temp1, sum4);
						sum5 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+20+WIDTH), temp1, sum5);
						sum6 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+24+WIDTH), temp1, sum6);
						sum7 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+28+WIDTH), temp1, sum7);

						sum0 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+WIDTH*2), temp2, sum0);
						sum1 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+4+WIDTH*2), temp2, sum1);
						sum2 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+8+WIDTH*2), temp2, sum2);
						sum3 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+12+WIDTH*2), temp2, sum3);
						sum4 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+16+WIDTH*2), temp2, sum4);
						sum5 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+20+WIDTH*2), temp2, sum5);
						sum6 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+24+WIDTH*2), temp2, sum6);
						sum7 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+28+WIDTH*2), temp2, sum7);

						sum0 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+WIDTH*3), temp3, sum0);
						sum1 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+4+WIDTH*3), temp3, sum1);
						sum2 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+8+WIDTH*3), temp3, sum2);
						sum3 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+12+WIDTH*3), temp3, sum3);
						sum4 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+16+WIDTH*3), temp3, sum4);
						sum5 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+20+WIDTH*3), temp3, sum5);
						sum6 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+24+WIDTH*3), temp3, sum6);
						sum7 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+28+WIDTH*3), temp3, sum7);

						sum0 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+WIDTH*4), temp4, sum0);
						sum1 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+4+WIDTH*4), temp4, sum1);
						sum2 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+8+WIDTH*4), temp4, sum2);
						sum3 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+12+WIDTH*4), temp4, sum3);
						sum4 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+16+WIDTH*4), temp4, sum4);
						sum5 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+20+WIDTH*4), temp4, sum5);
						sum6 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+24+WIDTH*4), temp4, sum6);
						sum7 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+28+WIDTH*4), temp4, sum7);

						sum0 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+WIDTH*5), temp5, sum0);
						sum1 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+4+WIDTH*5), temp5, sum1);
						sum2 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+8+WIDTH*5), temp5, sum2);
						sum3 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+12+WIDTH*5), temp5, sum3);
						sum4 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+16+WIDTH*5), temp5, sum4);
						sum5 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+20+WIDTH*5), temp5, sum5);
						sum6 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+24+WIDTH*5), temp5, sum6);
						sum7 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+28+WIDTH*5), temp5, sum7);

						sum0 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+WIDTH*6), temp6, sum0);
						sum1 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+4+WIDTH*6), temp6, sum1);
						sum2 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+8+WIDTH*6), temp6, sum2);
						sum3 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+12+WIDTH*6), temp6, sum3);
						sum4 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+16+WIDTH*6), temp6, sum4);
						sum5 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+20+WIDTH*6), temp6, sum5);
						sum6 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+24+WIDTH*6), temp6, sum6);
						sum7 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+28+WIDTH*6), temp6, sum7);

						sum0 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+WIDTH*7), temp7, sum0);
						sum1 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+4+WIDTH*7), temp7, sum1);
						sum2 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+8+WIDTH*7), temp7, sum2);
						sum3 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+12+WIDTH*7), temp7, sum3);
						sum4 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+16+WIDTH*7), temp7, sum4);
						sum5 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+20+WIDTH*7), temp7, sum5);
						sum6 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+24+WIDTH*7), temp7, sum6);
						sum7 = _mm256_fmadd_pd(_mm256_load_pd(mat2+j+28+WIDTH*7), temp7, sum7);

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
