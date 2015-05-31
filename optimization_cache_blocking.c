#include "header.h"

#define WIDTH_BLOCK 32
#define HEIGHT_BLOCK 32

void optimization_cache_blocking(double* restrict result,
		const double* restrict matrix1, const double* restrict matrix2) {

	memset(result, 0, WIDTH*HEIGHT*sizeof(double));
	for (int w = 0; w < WIDTH; w += WIDTH_BLOCK) {
		for (int h = 0; h < HEIGHT; h += HEIGHT_BLOCK) {
			for (int i = 0; i < WIDTH; i++)
			{
				for (int j = h; j < h + HEIGHT_BLOCK; j++)
				{
					double temp = result[i*WIDTH+j];
					for (int k = w; k < w + WIDTH_BLOCK; k++)
					{
						temp += matrix1[i*WIDTH+k]*matrix2[k*WIDTH+j];
					}
					result[i*WIDTH+j] = temp;
				}
			}
		}
	}
}
