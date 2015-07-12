#ifndef MATRIX_OPTIMIZATIONS_H
#define MATRIX_OPTIMIZATIONS_H

void optimize(double* r, const double* m1, const double* m2);

void naive(double* r,
		const double* m1, const double* m2);

void openmp(double* r, const double* m1, const double* m2);

void simd(double* r, const double* m1, const double* m2);

void cacheBlock(double* r, const double* m1, const double* m2);

void loopUnroll(double* r, const double* m1, const double* m2);

void registerBlock(double* r, const double* m1, const double* m2);

void openmp_simd(double* r, const double* m1, const double* m2);

void openmp_simd_cacheBlock(double* r, const double* m1, const double* m2);

void openmp_simd_cacheBlock_loopUnroll(double* r, const double* m1, const double* m2);

void openmp_simd_cacheBlock_loopUnroll_registerBlock(double* r, const double* m1, const double* m2);

#endif
