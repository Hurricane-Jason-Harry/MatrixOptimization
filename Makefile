CFLAGS=-Wall -mavx2 -mfma -O1 -std=c99 -fopenmp -g
CFILES= main.c header.h utils.c optimizations.c
LIB=/usr/local/lib/libpapi.a
all: 
	gcc $(CFLAGS) $(CFILES) -lm -o main ${LIB}
