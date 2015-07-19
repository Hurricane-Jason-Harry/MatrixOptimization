#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <sys/time.h>
#include <stdint.h>

int compare_matrix(const double* sample, const double* reference);

int read_matrix_dimension(const char* filename, int* w1, int* w2h1, int* h2);

int read_matrix(const char* filename, double* prod, double* matA, double* matB);

/* Return the current system time in micro seconds */
inline static uint64_t timestamp_us()
{
	struct timeval tv;
	gettimeofday(&tv, 0);
	return 1000000L * tv.tv_sec + tv.tv_usec;
}

#endif
