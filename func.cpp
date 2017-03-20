#include "herdsim.h"
#include <time.h>
#include <fstream>

#define INFECTED 1
#define VACCINATED 0

//Generate the matrix
void genpopulation(int *a, int *b, int *c, unsigned int n) {
  // LET's do this sheeeettt
  srand(time(NULL));

  //init infected
  for (int i = 0; i < n*n; i++) {
     if(((rand() % 10000)/10000.) < INFECTION_SEED_RATIO){
        SetBit(a,i);
     }
  }

  //init unvaccinated
  for (int i = 0; i < n*n; i++) {
     if(((rand() % 10000)/10000.) > VAC_RATE){
        SetBit(b,i);
     }
  }

}

//Read the matrix from a file
void readpopulation(int *a, int *b, int *c, unsigned int n, char *filename) {
  // STYLE POINTSSSSSSSS //og comments yo
  std::ifstream file;
  file.open(filename);

  srand(time(NULL));
  int current=0;
 //init infections
  for(int i = 0; i < n*n; i++){
    file >> current;
    if(current==1){
      SetBit(a, i);
    }
  }
  //init vaccinations
  for (int i = 0; i < n*n; i++) {
     if(((rand() % 10000)/10000.) > VAC_RATE){
        SetBit(b,i);

     }
  }

  file.close();
}

// Inline functions for calculating index of neighbors
// b=-1 -> right, b=0 -> middle, b=1 ->left
inline int nborRow(int *a, int x, int b, int n){
	int nborindex = (x/n * n) + (x % n +(n-b)) % n;
	return TestBit(a, nborindex);//a[nborindex];

} // Neighbors in current row

inline int nborTop(int *a, int x, int b, int n){
	int nborindex = (((x/n * n) + n*n - n) % (n*n)) + (x % n +(n-b)) % n;
  return TestBit(a, nborindex);//a[nborindex];
} // Neighbors in top row

inline int nborBot(int *a, int x, int b, int n){
	int nborindex = (((x/n * n) + n) % (n*n)) + (x % n +(n-b)) % n;
  return TestBit(a, nborindex);//a[nborindex];
} // Neighbors in bottom row

inline void liveOrDie(int *a, int *a_tmp, int *b, int *c, unsigned int n, int index){

  int currentVal = TestBit(a, index);
  if(currentVal == INFECTED){

    SetBit(a_tmp, index);
    return;
  }else{
    int nborcount = 0;
    nborcount += nborTop(a, index, -1, n);
    nborcount += nborTop(a, index, 0, n);
    nborcount += nborTop(a, index, 1, n);
    nborcount += nborRow(a, index, -1, n);
    nborcount += nborRow(a, index, 1, n);
    nborcount += nborBot(a, index, -1, n);
    nborcount += nborBot(a, index, 0, n);
    nborcount += nborBot(a, index, 1, n);

    if(TestBit(b, index) == VACCINATED){
      for(int i = 0; i<nborcount; i++){
        if(((rand() % 10000)/10000.) < VAC_INFECTION_CHANCE){
           SetBit(a_tmp,index);
        }
      }

    }else{
      for(int i = 0; i<nborcount; i++){
        if(((rand() % 10000)/10000.) < UNVAC_INFECTION_CHANCE){
           SetBit(a_tmp,index);
        }
      }

    }
  }

}

void debug(int *a, int *a_tmp, int *b, int *c, unsigned int n, int arrsize){
  for (size_t i = 0; i < arrsize; i++) {
    printf("%d \n", a[i]);
  }
}
int* herdsim(int *a, int *b, int *c, unsigned int n, unsigned int iter, int *infectedcount, int arrsize) {
//doesnt get much more dope than this tbh
//print initail
  printf("----------------init----------------\n");
  for(int i=0; i<n*n; i++){
      if(TestBit(a,i)){
        printf("%s1 ", RED);
      }else{
        if(TestBit(b,i)){
          printf("%s0 ", WHT);
        }else{
          printf("%s0 ", GRN);
        }
      }
      if(i%n==n-1){
        printf("\n");
      }
  }
  printf("\n");
  printf("%s", NRM);
  printf("-------------------------------------\n");

  int *a_tmp;
  a_tmp = (int *)malloc(((n*n + sizeof(int)*CHAR_BIT - 1) & ~(sizeof(int)*CHAR_BIT - 1))/CHAR_BIT);
  int cnt = 0;

  for(int i = 0; i < iter; i++){
    //debug(a,a_tmp,b,n, arrsize);//--------------------------------------------------------------------------------------------
    // Phase 1: Check neighbors of each cell, update cells in temp array
    for(int j = 0; j < n*n; j++){
      liveOrDie(a, a_tmp, b, c, n, j);
    }
    // Phase 2: swap tmp and normal
    a=(int *)((uintptr_t)a_tmp^(uintptr_t)a);
    a_tmp=(int *)((uintptr_t)a_tmp^(uintptr_t)a);
    a=(int *)((uintptr_t)a^(uintptr_t)a_tmp);

    // Phase 3: display
    if(INTERACTIVE){
      system("clear");
    }
    printf("infected: %d iteration %d\n", countinfected(a ,n, arrsize), i);


    for(int i=0; i<n*n; i++){
        if(TestBit(a,i)){
          printf("%s1 ", RED);
        }else{
          if(TestBit(b,i)){
            printf("%s0 ", WHT);
          }else{
            printf("%s0 ", GRN);
          }
        }
        if(i%n==n-1){
          printf("\n");
        }
    }
    printf("\n");
    printf("%s", NRM);
    if(INTERACTIVE){
      getchar();
    }

  }
  return a;

}
