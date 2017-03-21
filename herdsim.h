#include<cilk/cilk.h>
#include<cilk/reducer_opadd.h>
#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<numeric>
#include<fstream>
#include <limits.h>
#include <cmath>

#include "example_util_gettime.h"

#define SetBit(A,k)     ( A[(k/(CHAR_BIT*sizeof(int)))] |=  (1 << (k%(CHAR_BIT*sizeof(int)))) )
#define TestBit(A,k)    (( A[(k/(CHAR_BIT*sizeof(int)))] &   (1 << (k%(CHAR_BIT*sizeof(int)))) ) != 0)
#define ClearBit(A,k)   ( A[(k/(CHAR_BIT*sizeof(int)))] &= ~(1 << (k%(CHAR_BIT*sizeof(int)))) )

#define SetBitsDttl(A,k,n)  { \
    if (n & 1) {              \
      SetBit(A, (k*4+0));         \
    }else{                    \
      ClearBit(A, (k*4+0));       \
    }                         \
    if (n & 2) {              \
      SetBit(A, (k*4+1));       \
    }else{                    \
      ClearBit(A, (k*4+1));     \
    }                         \
    if (n & 4) {              \
      SetBit(A, (k*4+2));       \
    }else{                    \
      ClearBit(A, (k*4+2));     \
    }                         \
    if (n & 8) {              \
      SetBit(A, (k*4+3));       \
    }else{                    \
      ClearBit(A, (k*4+3));     \
    }                         \
}
#define GetBitsDttl(A,k) (TestBit(A, (k*4+0)) + (TestBit(A, (k*4+1))<<1) + (TestBit(A, (k*4+2))<<2) + (TestBit(A, (k*4+3))<<3))

#define VAC_RATE .5
#define INFECTION_SEED 1
#define INFECTION_SEED_RATIO .01
#define VAC_INFECTION_CHANCE .1
#define UNVAC_INFECTION_CHANCE .9
#define DISEASE_TTL 8
#define INTERACTIVE 1

#define RED  "\x1B[31m"
#define DRKRED  "\x1B[38;2;127;27;27m"
#define GRN  "\x1B[32m"
#define DRKGRN  "\x1B[38;2;10;124;64m"
#define WHT  "\x1B[37m"
#define MAG  "\x1b[35m"
#define NRM  "\x1B[0m"

void readpopulation(int *a, int *b, int *c, unsigned int n, char *filename);
void genpopulation(int *a, int *b, int *c, unsigned int n);
int* herdsim(int *a, int *b, int *c, unsigned int n, unsigned int iter, int *livecount, int arrsize);
int countinfected(int *a, unsigned int n, int arrsize);
