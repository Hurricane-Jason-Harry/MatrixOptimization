#ifdef __riscv

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "mm_malloc.h"
#include "config.h"
#include "boost.h"

void matmul_omp(double* restrict prod,
		const double* restrict matA, const double* restrict matB) {
#ifdef __AVX2__
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
#endif
}


void __riscv_simd(void);
void matmul_simd(double* restrict prod,
		const double* restrict matA, const double* restrict matB) {
    __asm__ volatile ("vsetcfg 2, 1\n");
	memset(prod, 0, WIDTH*HEIGHT*sizeof(double));
	for (int i = 0; i < WIDTH; i++)
	{
		for (int k = 0; k < WIDTH; k++)
		{
			volatile int64_t vector_size;
			__asm__ volatile ("  vmss vs1, %0": :"r"(matA[i*WIDTH+k]));
			for (int j = 0; j < HEIGHT; j+= vector_size)
			{
				__asm__ volatile ("  vsetvl %0, %1\n": "=r"(vector_size) : "r"(HEIGHT-j));
				__asm__ volatile ("  vmsa va0, %0"::"r"(matB+k*WIDTH+j));
				__asm__ volatile ("  vmsa va1, %0"::"r"(prod+i*WIDTH+j));
				__asm__ volatile("vf 0(%0)\n": :"r"(&__riscv_simd):"memory");
			}
		}
	}
}

// __riscv_simd
__asm__ (
		".align 3\n"
		"__riscv_simd:\n"
		"  vld vv0, va0\n"
		"  vld vv1, va1\n"
		"  vfmadd.d vv1, vs1, vv0, vv1\n"
		"  vsd vv1, va1\n"
		"  vstop\n");

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

	#define REG_BLOCK1 8
	#define REG_BLOCK2 2
	memset(prod, 0, WIDTH*HEIGHT*sizeof(double));
	for (int i = 0; i < WIDTH; i++)
	{
		for (int k = 0; k < WIDTH/REG_BLOCK1*REG_BLOCK1; k+=REG_BLOCK1)
		{
			#define CODE1(n) double scalarA##n=matA[i*WIDTH+k+n];
			UNROLL(CODE1, REG_BLOCK1)
			#undef CODE1

			for (int j = 0; j < HEIGHT/REG_BLOCK2*REGBLOCK2; j+= REG_BLOCK2) {
				#define CODE2(n) double vecP##n = prod[i*WIDTH+j+n];
				UNROLL(CODE2, REG_BLOCK2)

				#define CODE3(n1, n2) vecP##n1 += scalarA##n2*matB[k*WIDTH+j+n1+n2*WIDTH];
				UNROLL2(CODE3, REG_BLOCK1, REG_BLOCK2)

				#define CODE4(n) prod[i*WIDTH+j+n] = vecP##n;
				UNROLL(CODE4, REG_BLOCK2)

				#undef CODE2
				#undef CODE3
				#undef CODE4
			}
		}
		for (int k = WIDTH/REG_BLOCK1*REG_BLOCK1; k < WIDTH; k++)
		{
			double scalarA=matA[i*WIDTH+k];
			for (int j = 0; j < HEIGHT; j++) {
				prod[i*WIDTH+j+n] += scalarA*matB[k*WIDTH+j];
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
    __asm__ volatile ("vsetcfg 2, 1\n");
	memset(prod, 0, WIDTH*HEIGHT*sizeof(double));
	//#pragma omp parallel for
	for (int i = 0; i < WIDTH; i++)
	{
		for (int k = 0; k < WIDTH; k++)
		{
			volatile int64_t vector_size;
			__asm__ volatile ("  vmss vs1, %0": :"r"(matA[i*WIDTH+k]));
			for (int j = 0; j < HEIGHT; j+= vector_size)
			{
				__asm__ volatile ("  vsetvl %0, %1\n": "=r"(vector_size) : "r"(HEIGHT-j));
				__asm__ volatile ("  vmsa va0, %0"::"r"(matB+k*WIDTH+j));
				__asm__ volatile ("  vmsa va1, %0"::"r"(prod+i*WIDTH+j));
				__asm__ volatile("vf 0(%0)\n": :"r"(&__riscv_simd):"memory");
			}
		}
	}
}

// Matrix Multiply openmp simd cacheBlock
void matmul_omp_simd_cb(double* restrict prod,
		const double* restrict matA, const double* restrict matB) {
	const int CACHE_BLOCK = 32;
	memset(prod, 0, WIDTH*HEIGHT*sizeof(double));
    __asm__ volatile ("vsetcfg 2, 1\n");
	memset(prod, 0, WIDTH*HEIGHT*sizeof(double));
//#pragma omp parallel
//{
	for (int kk = 0; kk < WIDTH; kk += CACHE_BLOCK)
	{
		//#pragma omp parallel for
		for (int i = 0; i < WIDTH; i++)
		{
			for (int k = kk; k < kk+CACHE_BLOCK; k++)
			{
				volatile int64_t vector_size;
				__asm__ volatile ("  vmss vs1, %0": :"r"(matA[i*WIDTH+k]));
				for (int j = 0; j < HEIGHT; j+= vector_size)
				{
					__asm__ volatile ("  vsetvl %0, %1\n": "=r"(vector_size) : "r"(HEIGHT-j));
					__asm__ volatile ("  vmsa va0, %0"::"r"(matB+k*WIDTH+j));
					__asm__ volatile ("  vmsa va1, %0"::"r"(prod+i*WIDTH+j));
					__asm__ volatile( "  vf 0(%0)\n": :"r"(&__riscv_simd):"memory");
				}
			}
		}
	}
//}
}

void __riscv_omp_simd_cb_lu();
// Matrix Multiply openmp simd cacheBlock loopUnroll
void matmul_omp_simd_cb_lu(double* restrict prod,
		const double* restrict matA, const double* restrict matB) {
	// CACHE_BLCOK needs to be a multiple of UNROLL_COUNT
	// UNROLL_COUNT <= 31
	const int CACHE_BLOCK = 31;
	#define UNROLL_COUNT 31
	memset(prod, 0, WIDTH*HEIGHT*sizeof(double));
    __asm__ volatile ("vsetcfg 2, 1\n");
//#pragma omp parallel
//{
	for (int kk = 0; kk < WIDTH/CACHE_BLOCK*CACHE_BLOCK; kk += CACHE_BLOCK)
	{
		//#pragma omp parallel for
		for (int i = 0; i < WIDTH; i++)
		{
			for (int k = kk; k < kk+CACHE_BLOCK; k+=UNROLL_COUNT)
			{
				volatile int64_t vector_size;
				#define CODE1(n) __asm__ volatile ("  vmss vs"#n", %0": :"r"(matA[i*WIDTH+k+n-1]));
				UNROLLZ(CODE1, 1, UNROLL_COUNT)
				for (int j = 0; j < HEIGHT; j+= vector_size)
				{
					__asm__ volatile ("  vsetvl %0, %1\n": "=r"(vector_size) : "r"(HEIGHT-j));
					__asm__ volatile ("  vmsa va0, %0"::"r"(prod+i*WIDTH+j));
					#define CODE2(n) __asm__ volatile ("  vmsa va"#n", %0"::"r"(matB+k*WIDTH+j+(n-1)*WIDTH));
					UNROLLZ(CODE2, 1, UNROLL_COUNT)
					__asm__ volatile("vf 0(%0)\n": :"r"(&__riscv_omp_simd_cb_lu):"memory");
				}
			}
		}

		for (int k = WIDTH/CACHE_BLOCK*CACHE_BLOCK; k < WIDTH; k++)
		{
			volatile int64_t vector_size;
			__asm__ volatile ("  vmss vs1, %0": :"r"(matA[i*WIDTH+k]));
			for (int j = 0; j < HEIGHT; j+= vector_size)
			{
				__asm__ volatile ("  vsetvl %0, %1\n": "=r"(vector_size) : "r"(HEIGHT-j));
				__asm__ volatile ("  vmsa va0, %0"::"r"(matB+k*WIDTH+j));
				__asm__ volatile ("  vmsa va1, %0"::"r"(prod+i*WIDTH+j));
				__asm__ volatile("   vf 0(%0)\n": :"r"(&__riscv_simd):"memory");
			}
		}
	}
}

// __riscv_omp_simd_cb_lu
__asm__ (
		".align 3\n"
		"__riscv_omp_simd_cb_lu:\n"
		"  vld vv0, va0\n");
#define CODE3(n) __asm__ (		"  vld vv1, va"#n"\n" \
								"  vfmadd.d vv0, vs"#n", vv1, vv0\n");
UNROLLZ(CODE3, 1, UNROLL_COUNT)

__asm__ (
		"  vsd vv0, va0\n"
		"  vstop\n");

#undef UNROLL_COUNT
#undef CODE1
#undef CODE2
#undef CODE3


// Matrix Multiply openmp simd cacheBlock loopUnroll registerBlock
void __riscv_omp_simd_cb_lu_rb();
void matmul_omp_simd_cb_lu_rb(double* restrict prod,
		const double* restrict matA, const double* restrict matB) {
	matmul_omp_simd_cb_lu(prod, matA, matB);
}




// ENDIF
#endif
