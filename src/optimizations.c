#include <assert.h>
#include <string.h>
#include "optimizations.h"
#include "config.h"

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

// Matrix Multiply cacheBlock
#ifdef MATMUL_CB_PROFILING

__attribute__((hot))
void matmul_cb_profiling(double* restrict prod,
		const double* restrict matA, const double* restrict matB) {
	const int C1 = CACHE1; // need to prefined CACHE1,2,3 in gcc flags
	const int C2 = CACHE2;
	const int C3 = CACHE3;
	memset(prod, 0, WIDTH*HEIGHT*sizeof(double));


	for (int ii = 0; ii < WIDTH; ii += C1) {
		for (int kk = 0; kk < WIDTH; kk += C2) {
			for (int jj = 0; jj < HEIGHT; jj += C3) {
				for (int i = ii; i < ii+C1; i++)
				{
					for (int k = kk; k < kk+C2; k++)
					{
						double scalarA = matA[i*WIDTH+k];
						for (int j = jj; j < jj+C3;j++) {
							prod[i*WIDTH+j] += scalarA*matB[k*WIDTH+j];
						}
					}
				}
			}
		}
	}
}

#endif


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
	#elif defined (MATMUL_CB_PROFILING)
		matmul_cb_profiling(prod, matA, matB);
	#else
		#error "No such optimization."
	#endif
}
