#ifdef __AVX2__
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include <xmmintrin.h>
#include <immintrin.h>
#include <x86intrin.h>

#include "config.h"
#include "optimizations.h"
#include "boost.h"

void matmul_omp(double* restrict prod,
		const double* restrict matA, const double* restrict matB) {
	memset(prod, 0, WIDTH*HEIGHT*sizeof(double));
	#pragma omp parallel for
	for (int i = 0; i < WIDTH; i++)
	{
		for (int k = 0; k < WIDTH; k++)
		{
			double scalarA = matA[i*WIDTH+k];
			for (int j = 0; j < HEIGHT; j++)
			{
				prod[i*WIDTH+j] += scalarA*matB[k*WIDTH+j];
			}
		}
	}
}

void matmul_simd(double* restrict prod,
		const double* restrict matA, const double* restrict matB) {
	memset(prod, 0, WIDTH*HEIGHT*sizeof(double));
	for (int i = 0; i < WIDTH; i++)
	{
		for (int k = 0; k < WIDTH; k++)
		{
			__m256d scalarA = _mm256_broadcast_sd(matA+i*WIDTH+k);
			for (int j = 0; j < HEIGHT; j+=4)
			{
				__m256d vecP = _mm256_load_pd(prod+i*WIDTH+j);
				__m256d vecB = _mm256_load_pd(matB+k*WIDTH+j);
				vecP = _mm256_fmadd_pd(scalarA, vecB,  vecP);
				_mm256_store_pd(prod+i*WIDTH+j, vecP);
			}
		}
	}
}

// Matrix Multiply cacheBlock
void matmul_cb(double* restrict prod,
		const double* restrict matA, const double* restrict matB) {
	const int CACHE_BLOCK = 32;
	memset(prod, 0, WIDTH*HEIGHT*sizeof(double));

	for (int kk = 0; kk < WIDTH; kk += CACHE_BLOCK) {
		for (int i = 0; i < WIDTH; i++)
		{
			for (int k = kk; k < kk+CACHE_BLOCK; k++)
			{
				double scalarA = matA[i*WIDTH+k];
				for (int j = 0; j < HEIGHT;j++) {
					prod[i*WIDTH+j] += scalarA*matB[k*WIDTH+j];
				}
			}
		}
	}
}


// Matrix Multiply loopUnroll
void matmul_lu(double* restrict prod,
		const double* restrict matA, const double* restrict matB) {
	#define UNROLL_COUNT 32
	memset(prod, 0, WIDTH*HEIGHT*sizeof(double));
	for (int i = 0; i < WIDTH; i++)
	{
		for (int k = 0; k < WIDTH; k++)
		{
			double scalarA = matA[i*WIDTH+k];
			for (int j = 0; j < HEIGHT; j+=UNROLL_COUNT)
			{
				#define CODE(n)  prod[i*WIDTH+j+n] += scalarA*matB[k*WIDTH+j+n];
				UNROLL(CODE, UNROLL_COUNT)
				#undef CODE
			}
		}
	}
	#undef UNROLL_COUNT
}

// Matrix Multiply registerBlock
void matmul_rb(double* restrict prod,
		const double* restrict matA, const double* restrict matB) {
	#define REG_BLOCK1 4
	#define REG_BLOCK2 4
	memset(prod, 0, WIDTH*HEIGHT*sizeof(double));
	for (int i = 0; i < WIDTH; i++)
	{
		for (int k = 0; k < WIDTH; k+=REG_BLOCK1)
		{
			#define CODE1(n) double scalarA##n=matA[i*WIDTH+k+n];
			UNROLL(CODE1, REG_BLOCK1)
			#undef CODE1

			for (int j = 0; j < HEIGHT; j+= REG_BLOCK2) {
				#define CODE2(n) double vecP##n = prod[i*WIDTH+j+n];
				UNROLL(CODE2, REG_BLOCK2)

				#define CODE3(n1, n2) vecP##n1 += scalarA##n2*matB[k*WIDTH+j+WIDTH*n2+n1];
				UNROLL2(CODE3, REG_BLOCK1, REG_BLOCK2)

				#define CODE4(n) prod[i*WIDTH+j+n] = vecP##n;
				UNROLL(CODE4, REG_BLOCK2)

				#undef CODE2
				#undef CODE3
				#undef CODE4
			}
		}
	}
	#undef REG_BLOCK1
	#undef REG_BLOCK2
}

// Matrix Multiply openmp simd
void matmul_omp_simd(double* restrict prod,
		const double* restrict matA, const double* restrict matB) {
	memset(prod, 0, WIDTH*HEIGHT*sizeof(double));

	#pragma omp parallel for
	for (int i = 0; i < WIDTH; i++)
	{
		for (int k = 0; k < WIDTH; k++)
		{
			__m256d scalarA = _mm256_broadcast_sd(matA+i*WIDTH+k);
			for (int j = 0; j < HEIGHT; j+=X86_VECTOR_LENGTH)
			{
				__m256d vecP = _mm256_load_pd(prod+i*WIDTH+j);
				__m256d vecB = _mm256_load_pd(matB+k*WIDTH+j);
				vecP = _mm256_fmadd_pd(scalarA, vecB,  vecP);
				_mm256_store_pd(prod+i*WIDTH+j, vecP);
			}
		}
	}
}

// Matrix Multiply openmp simd cacheBlock
void matmul_omp_simd_cb(double* restrict prod,
		const double* restrict matA, const double* restrict matB) {
	const int CACHE_BLOCK = 32;
	memset(prod, 0, WIDTH*HEIGHT*sizeof(double));

	#pragma omp parallel
	{
		for (int kk = 0; kk < WIDTH; kk += CACHE_BLOCK) {
			#pragma omp for
			for (int i = 0; i < WIDTH; i++)
			{
				for (int k = kk; k < kk+CACHE_BLOCK; k++)
				{
					__m256d scalarA = _mm256_broadcast_sd(matA+i*WIDTH+k);
					for (int j = 0; j < HEIGHT; j+=X86_VECTOR_LENGTH)
					{
						__m256d vecP = _mm256_load_pd(prod+i*WIDTH+j);
						__m256d vecB = _mm256_load_pd(matB+k*WIDTH+j);
						vecP = _mm256_fmadd_pd(scalarA, vecB,  vecP);
						_mm256_store_pd(prod+i*WIDTH+j, vecP);
					}
				}
			}
		}
	}
}

// Matrix Multiply openmp simd cacheBlock loopUnroll
void malmul_omp_simd_cb_lu(double* restrict prod,
		const double* restrict matA, const double* restrict matB) {
	const int CACHE_BLOCK = 32;
	#define UNROLL_COUNT 8
	memset(prod, 0, WIDTH*HEIGHT*sizeof(double));

	#pragma omp parallel
	{
		for (int kk = 0; kk < WIDTH; kk += CACHE_BLOCK) {
			#pragma omp for
			for (int i = 0; i < WIDTH; i++)
			{
				for (int k = kk; k < kk+CACHE_BLOCK; k++)
				{
					__m256d scalarA = _mm256_broadcast_sd(matA+i*WIDTH+k);
					for (int j = 0; j < HEIGHT; j+=X86_VECTOR_LENGTH*UNROLL_COUNT)
					{
						__m256d vecB, vecP;
						#define CODE(n) vecP = _mm256_load_pd(prod+i*WIDTH+j+n*X86_VECTOR_LENGTH);\
								vecB = _mm256_load_pd(matB+k*WIDTH+j+n*X86_VECTOR_LENGTH);\
								vecP = _mm256_fmadd_pd(scalarA, vecB, vecP);\
								_mm256_store_pd(prod+i*WIDTH+j+n*X86_VECTOR_LENGTH, vecP);

						UNROLL(CODE, UNROLL_COUNT)

						#undef CODE
					}
				}
			}
		}
	}
	#undef UNROLL_COUNT
}

// Matrix Multiply openmp simd cacheBlock loopUnroll registerBlock
void matmul_omp_simd_cb_lu_rb(double* restrict prod,
		const double* restrict matA, const double* restrict matB) {
	const int CACHE_BLOCK = 32;
	#define REG_BLOCK2 8
	#define REG_BLOCK1 8
	memset(prod, 0, WIDTH*HEIGHT*sizeof(double));

	#pragma omp parallel
	{
		for (int kk = 0; kk < WIDTH; kk += CACHE_BLOCK) {
			#pragma omp for
			for (int i = 0; i < WIDTH; i++)
			{
				for (int k = kk; k < kk+CACHE_BLOCK; k+=REG_BLOCK1)
				{
					#define CODE1(n) __m256d scalarA##n = _mm256_broadcast_sd(matA+i*WIDTH+k+n);
					UNROLL(CODE1, REG_BLOCK1);
					#undef CODE1

					for (int j = 0; j < HEIGHT; j+=X86_VECTOR_LENGTH*REG_BLOCK2)
					{
						#define CODE2(n) __m256d vecP##n = _mm256_load_pd(prod+i*WIDTH+j+n*X86_VECTOR_LENGTH);
						UNROLL(CODE2, REG_BLOCK2)

						#define CODE3(n1, n2) vecP##n1 = _mm256_fmadd_pd(\
							_mm256_load_pd(matB+k*WIDTH+j+X86_VECTOR_LENGTH*n1+WIDTH*n2), scalarA##n2, vecP##n1);
						UNROLL2(CODE3, REG_BLOCK1, REG_BLOCK2)

						#define CODE4(n) _mm256_store_pd(prod+i*WIDTH+j+n*X86_VECTOR_LENGTH, vecP##n);
						UNROLL(CODE4, REG_BLOCK2)

						#undef CODE2
						#undef CODE3
						#undef CODE4
					}
				}
			}
		}
	}
	#undef REG_BLOCK1
	#undef REG_BLOCK2
}

#endif
