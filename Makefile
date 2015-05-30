CFLAGS=-Wall -mavx2 -mfma -O2 -std=c99 -fopenmp
CFILES= main.c header.h utils.c optimization_naive.c optimization_openmp.c
all: 
	gcc $(CFLAGS) $(CFILES) -lm -o main