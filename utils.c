#include "header.h"

/*
 * Compare two matrices. Return 0 if they are the same, 1 otherwise.
 */
int compare_matrix(const double* sample, const double* reference) {
	for (int i = 0; i < HEIGHT*WIDTH; i++) {
		if (abs(sample[i] - reference[i]) > 1e-15) {
			return 1;
		}
	}
	return 0;
}
