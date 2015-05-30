#include "header.h"

/*
 * Compare two matrices. Return 0 if they are the same, 1 otherwise.
 */
int compare_matrix(double* sample, double* reference) {
	for (int i = 0; i < HEIGHT*WIDTH; i++) {
		if (sample[i] != reference[i]) {
			return 1;
		}
	}
	return 0;
}
