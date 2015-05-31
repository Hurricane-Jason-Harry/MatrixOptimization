CFLAGS=-Wall -mavx2 -mfma -std=c99 -fopenmp
CFILES= main.c header.h utils.c optimizations.c
all: 
	gcc $(CFLAGS) $(CFILES) -lm -o main