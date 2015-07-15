#ifndef MATRIX_CONFIG_H
#define MATRIX_CONFIG_H

#define ENABLE 1
#define DISABLE 0

#define WIDTH 256
#define HEIGHT 256

#define W1 (WIDTH)
#define H1W2 (HEIGHT)
#define W2H1 (HEIGHT)
#define H2 (HEIGHT)

#define TEST_FILENAME "test/test.out"

#define LINE_SIZE 64
#define L1_SIZE (32*1024)
#define L2_SIZE (256*1024)
#define L3_SIZE (6144*1024)
#define L4_SIZE (131072*1024)

#define NUM_OF_OPTIMIZATIONS 10

#define NUM_OF_EVENTS 5

extern int w1;
extern int w2h1;
extern int h2;

#endif
