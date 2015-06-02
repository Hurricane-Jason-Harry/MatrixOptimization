#ifndef MATRIX_HEADER
#define MATRIX_HEADER

#include <string.h>
#include <immintrin.h>

#define ENABLE 1
#define DISABLE 0

#define WIDTH 1024
#define HEIGHT 1024

#define LINE_SIZE 64
#define L1_SIZE (32*1024)
#define L2_SIZE (256*1024)
#define L3_SIZE (6144*1024)
#define L4_SIZE (131072*1024)

#define NUM_OF_OPTIMIZATIONS 10


extern int compare_matrix(const double* sample, const double* reference);

extern void naive(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void openmp(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void simd(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void cacheBlock(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void loopUnroll(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void registerBlock(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void openmp_simd(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void openmp_simd_cacheBlock(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void openmp_simd_cacheBlock_loopUnroll(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void openmp_simd_cacheBlock_loopUnroll_registerBlock(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

#endif
