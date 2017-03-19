#include<cilk/cilk.h>
#include<cilk/reducer_opadd.h>
#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<numeric>
#include<fstream>
#include <limits.h>

#include "example_util_gettime.h"

#define SetBit(A,k)     ( A[(k/(CHAR_BIT*sizeof(int)))] |=  (1 << (k%(CHAR_BIT*sizeof(int)))) )
#define TestBit(A,k)    ( A[(k/(CHAR_BIT*sizeof(int)))] &   (1 << (k%(CHAR_BIT*sizeof(int)))) )
#define ClearBit(A,k)   ( A[(k/(CHAR_BIT*sizeof(int)))] &= ~(1 << (k%(CHAR_BIT*sizeof(int)))) )

#define VAC_RATE .9
#define INFECTION_SEED 1
#define INFECTION_SEED_RATIO .01

void readpopulation(int *a, int *b, unsigned int n, char *filename);
void genpopulation(int *a, int *b, unsigned int n);
void herdsim(int *a, int *b, unsigned int n, unsigned int iter, int *livecount);
int countinfected(int *a, unsigned int n, int arrsize);
