#include <assert.h>
#include "optimizations.h"
#include "config.h"

#ifdef __AVX2__
#include "optimizations_x86.c"
#elif defined __riscv
#include "optimizations_riscv.c"
#endif

void matmul_naive(double* restrict prod,
		const double* restrict matA, const double* restrict matB) {
	memset(prod, 0, WIDTH*HEIGHT*sizeof(double));

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

void matmul_optimize(double* prod, const double* matA, const double* matB) {
	#ifdef MATMUL_NAIVE
		matmul_naive(prod, matA, matB);
	#elif defined (MATMUL_OMP)
		matmul_omp(prod, matA, matB);
	#elif defined (MATMUL_SIMD)
		matmul_simd(prod, matA, matB);
	#elif defined (MATMUL_CB)
		matmul_cb(prod, matA, matB);
	#elif defined (MATMUL_LU)
		matmul_lu(prod, matA, matB);
	#elif defined (MATMUL_RB)
		matmul_rb(prod, matA, matB);
	#elif defined (MATMUL_OMP_SIMD)
		matmul_omp_simd(prod, matA, matB);
	#elif defined (MATMUL_OMP_SIMD_CB)
		matmul_omp_simd_cb(prod, matA, matB);
	#elif defined (MATMUL_OMP_SIMD_CB_LU)
		matmul_omp_simd_cb_lu(prod, matA, matB);
	#elif defined (MATMUL_OMP_SIMD_CB_LU_RB)
		matmul_omp_simd_cb_lu_rb(prod, matA, matB);
	#else
		assert(0);
	#endif
}
