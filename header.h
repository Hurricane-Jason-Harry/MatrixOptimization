#ifndef MATRIX_OPTIMIZATION_HEADER
#define MATRIX_OPTIMIZATION_HEADER

#include <string.h>
#include <immintrin.h>

#define WIDTH 1024
#define HEIGHT 1024

extern int compare_matrix(double* sample, double* reference);

extern void optimization_naive(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void optimization_openmp(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void optimization_simd(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void optimization_cache_blocking(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void optimization_loop_unrolling(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void optimization_register_blocking(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void optimization_openmp_simd(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);
#endif
