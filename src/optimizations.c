#include "optimizations.h"


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

void optimize(double* prod, const double* matA, const double* matB) {
	#ifdef _OP_NAIVE
		matmul_naive(prod, matA, matB);
	#elif defined (_OP_OPENMP)
		matmul_omp(product, matA, matB);
	#elif defined (_OP_SIMD)
		matmul_simd(prod, matA, matB);
	#elif defined (_OP_CACHEBLOCK)
		matmul_cb(prod, matA, matB);
	#elif defined (_OP_LOOPUNROLL)
		matmul_lu(prod, matA, matB);
	#elif defined (_OP_REGISTERBLOCK)
		matmul_rb(prod, matA, matB);
	#elif defined (_OP_OPENMP_SIMD)
		matmul_omp_simd(prod, matA, matB);
	#elif defined (_OP_OPENMP_SIMD_CACHEBLOCK)
		matmul_omp_simd_cb(prod, matA, matB);
	#elif defined (_OP_OPENMP_SIMD_CACHEBLOCK_LOOPUNROLL)
		matmul_omp_simd_cb_lu(prod, matA, matB);
	#elif defined (_OP_OPENMP_SIMD_CACHEBLOCK_LOOPUNROLL_REGISTERBLOCK)
		matmul_omp_simd_cb_lu_rb(prod, matA, matB);
	#endif
}
