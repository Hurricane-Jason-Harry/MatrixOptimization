#ifndef MATRIX_OPTIMIZATIONS_H
#define MATRIX_OPTIMIZATIONS_H

void optimize(double* r, const double* m1, const double* m2);

void matmul_naive(double* r,
		const double* m1, const double* m2);

void matmul_omp(double* r, const double* m1, const double* m2);

void matmul_simd(double* r, const double* m1, const double* m2);

void matmul_cb(double* r, const double* m1, const double* m2);

void matmul_lu(double* r, const double* m1, const double* m2);

void matmul_rb(double* r, const double* m1, const double* m2);

void matmul_omp_simd(double* r, const double* m1, const double* m2);

void matmul_omp_simd_cb(double* r, const double* m1, const double* m2);

void malmul_omp_simd_cb_lu(double* r, const double* m1, const double* m2);

void matmul_omp_simd_cb_lu_rb(double* r, const double* m1, const double* m2);

#endif
