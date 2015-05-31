CFLAGS=-Wall -mavx2 -mfma -std=c99 -fopenmp
CFILES= main.c header.h utils.c optimization_naive.c optimization_openmp.c optimization_simd.c optimization_cache_blocking.c
all: 
	gcc $(CFLAGS) $(CFILES) -lm -o main