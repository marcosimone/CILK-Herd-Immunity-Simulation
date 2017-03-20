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
#define TestBit(A,k)    (( A[(k/(CHAR_BIT*sizeof(int)))] &   (1 << (k%(CHAR_BIT*sizeof(int)))) ) > 0)
#define ClearBit(A,k)   ( A[(k/(CHAR_BIT*sizeof(int)))] &= ~(1 << (k%(CHAR_BIT*sizeof(int)))) )

#define VAC_RATE .9
#define INFECTION_SEED 1
#define INFECTION_SEED_RATIO .01
#define VAC_INFECTION_CHANCE .1
#define UNVAC_INFECTION_CHANCE .9

#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define WHT  "\x1B[37m"

void readpopulation(int *a, int *b, unsigned int n, char *filename);
void genpopulation(int *a, int *b, unsigned int n);
int* herdsim(int *a, int *b, unsigned int n, unsigned int iter, int *livecount, int arrsize);
int countinfected(int *a, unsigned int n, int arrsize);
