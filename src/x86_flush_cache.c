#include <stdlib.h>
// Should be compiled without compiler optimization
int main() {
#pragma omp parallel
	{
     const int size = 64*1024*1024; // Allocate 64M for each core.
                                    // Set much larger then L2
     int* c = (int*)malloc(size);
     for (int i = 0; i < 8; i++)
       for (int j = 0; j < size/sizeof(int); j++)
         c[j] = i*j;
     free(c);
	}
     return 0;
 }
