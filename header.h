#ifndef MATRIX_OPTIMIZATION_HEADER
#define MATRIX_OPTIMIZATION_HEADER

#include <string.h>
#include <immintrin.h>

#define WIDTH 1024
#define HEIGHT 1024

#define LINE_SIZE 64
#define L1_SIZE (32*1024)
#define L2_SIZE (256*1024)
#define L3_SIZE (6144*1024)
#define L4_SIZE (131072*1024)

#define NUM_OF_OPTIMIZATIONS 10

extern int compare_matrix(const double* sample, const double* reference);

extern void optimization_naive(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void optimization_openmp(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void optimization_simd(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void optimization_cache_block(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void optimization_loop_unroll(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void optimization_register_block(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void optimization_openmp_simd(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void optimization_openmp_simd_cache_block(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void optimization_openmp_simd_cache_block_loop_unroll(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void optimization_openmp_simd_cache_register_block_loop_unroll(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

#endif
