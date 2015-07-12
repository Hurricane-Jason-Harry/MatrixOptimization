#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

int compare_matrix(const double* sample, const double* reference);

int read_matrix_dimension(const char* filename, int* w1, int* w2h1, int* h2);

int read_matrix(const char* filename, double* r, double* m1, double* m2);

/* Return the current system time in micro seconds */
inline static uint64_t timestamp_us()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return 1000000L * tv.tv_sec + tv.tv_usec;
}

#endif
