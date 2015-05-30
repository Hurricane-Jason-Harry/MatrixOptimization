CFLAGS=-Wall -mavx2 -mfma -O2 -std=c99 -fopenmp
all: 
	gcc $(CFLAGS) main.c -lm -o main