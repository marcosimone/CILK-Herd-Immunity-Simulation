#include "herdsim.h"
#include <time.h>
#include <fstream>

#define BIRTH 2
#define DEATH 3

//Generate the life matrix any way you want. We would highly recommend that you print the generated
//matrix into a file, so that you can share it with other teams for checking correctness.
void genpopulation(int *a, int *b, unsigned int n) {
  // LET's do this sheeeettt
  srand(time(NULL));
  for (int i = 0; i < n*n; i++) {
     if(rand() & 1){
        SetBit(a,i);
     }
  }

}

//Read the life matrix from a file
void readpopulation(int *a, int *b, unsigned int n, char *filename) {
  // STYLE POINTSSSSSSSS
  std::ifstream file;
  file.open(filename);
  a={0};
  int current=0;
  for(int i = 0; i < n*n; i++){
    file >> current;
    printf("in:%d\n", current);
    if(current==1){
      SetBit(a, i);
    }
  }
  file.close();
}

// Inline functions for calculating index of neighbors
// b=-1 -> right, b=0 -> middle, b=1 ->left
inline int nborRow(int *a, int x, int b, int n){
	int nborindex = (x/n * n) + (x % n +(n-b)) % n;
	int nborvalue = a[nborindex];
  if(nborvalue == 1 || nborvalue == DEATH){
    return 1;
  }else if(nborvalue == 0 || nborvalue == BIRTH){
    return 0;
  }
} // Neighbors in current row

inline int nborTop(int *a, int x, int b, int n){
	int nborindex = (((x/n * n) + n*n - n) % (n*n)) + (x % n +(n-b)) % n;
  int nborvalue = a[nborindex];
  if(nborvalue == 1 || nborvalue == DEATH){
    return 1;
  }else if(nborvalue == 0 || nborvalue == BIRTH){
    return 0;
  }
} // Neighbors in top row

inline int nborBot(int *a, int x, int b, int n){
	int nborindex = (((x/n * n) + n) % (n*n)) + (x % n +(n-b)) % n;
  int nborvalue = a[nborindex];
  if(nborvalue == 1 || nborvalue == DEATH){
    return 1;
  }else if(nborvalue == 0 || nborvalue == BIRTH){
    return 0;
  }
} // Neighbors in bottom row

inline void liveOrDie(int *a, unsigned int n, int index){
  int nborcount = 0;
  nborcount += nborTop(a, index, -1, n);
  nborcount += nborTop(a, index, 0, n);
  nborcount += nborTop(a, index, 1, n);
  nborcount += nborRow(a, index, -1, n);
  nborcount += nborRow(a, index, 1, n);
  nborcount += nborBot(a, index, -1, n);
  nborcount += nborBot(a, index, 0, n);
  nborcount += nborBot(a, index, 1, n);
  int currentVal = a[index];
  if(currentVal == 0){
    if(nborcount == 3){
      a[index] = BIRTH;
    }
  }else if(currentVal == 1){
    if(!(nborcount == 2 || nborcount == 3)){
      a[index] = DEATH;
    }
  }
}


void herdsim(int *a, int *b, unsigned int n, unsigned int iter, int *infectedcount) {

  int cnt = 0;
  for(int i = 0; i < iter; i++){
    // Phase 1: Check neighbors of each cell, update cells to be b or d
    cilk_for(int j = 0; j < n*n; j++){
      liveOrDie(a, n, j);
    }
    // Phase 2: Go through array, replace b's with 1's, d's with 0's
    cilk_for(int j = 0; j < n*n; j++){
      if(a[j] == BIRTH){
        a[j] = 1;
      }else if(a[j] == DEATH){
        a[j] = 0;
      }
    }
    // printf("countlive: %d iteration %d\n", countlive(a ,n), i);
    // Phase 3: Call infectedcount every 1/10(iter) times if DEBUG == 1
    /*
    if(DEBUG == 1){
      if((i+1) % (iter/10) == 0){
        infectedcount[cnt] = countinfected(a, n);
        cnt++;
      }
    }
    */
  }

}
