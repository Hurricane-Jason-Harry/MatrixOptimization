#ifndef MATRIX_OPTIMIZATIONS_H
#define MATRIX_OPTIMIZATIONS_H

void matmul_optimize(double* prod, const double* matA, const double* matB);

void matmul_naive(double* prod,
		const double* matA, const double* matB);

void matmul_omp(double* prod, const double* matA, const double* matB);

void matmul_simd(double* prod, const double* matA, const double* matB);

void matmul_cb(double* prod, const double* matA, const double* matB);

void matmul_lu(double* prod, const double* matA, const double* matB);

void matmul_rb(double* prod, const double* matA, const double* matB);

void matmul_omp_simd(double* prod, const double* matA, const double* matB);

void matmul_omp_simd_cb(double* prod, const double* matA, const double* matB);

void malmul_omp_simd_cb_lu(double* prod, const double* matA, const double* matB);

void matmul_omp_simd_cb_lu_rb(double* prod, const double* matA, const double* matB);

#endif
