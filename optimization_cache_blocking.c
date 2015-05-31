#include "header.h"

#define WIDTH_BLOCK 128
#define HEIGHT_BLOCK 128

void optimization_cache_blocking(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {

	for (int w = 0; w < WIDTH; w += WIDTH_BLOCK) {
		for (int h = 0; h < HEIGHT; h += HEIGHT_BLOCK) {
			for (int i = w; i < w + WIDTH_BLOCK; i++)
			{
				for (int j = h; j < h + HEIGHT_BLOCK; j++)
				{
					double temp = 0;
					for (int k = 0; k < WIDTH; k++)
					{
						temp += matrix1[i*WIDTH+k]*matrix2[k*WIDTH+j];
					}
					result[i*WIDTH+j] = temp;
				}
			}
		}
	}
}
