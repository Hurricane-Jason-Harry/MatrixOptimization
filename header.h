#ifndef MATRIX_OPTIMIZATION_HEADER
#define MATRIX_OPTIMIZATION_HEADER

#include <string.h>

#define WIDTH 512
#define HEIGHT 512

extern int compare_matrix(double* sample, double* reference);

extern void optimization_naive(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

extern void optimization_openmp(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2);

#endif
