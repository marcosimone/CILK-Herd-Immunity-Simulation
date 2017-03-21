#include "herdsim.h"
#include <time.h>
#include <fstream>

#define INFECTED 1
#define VACCINATED 0

//Generate the matrix
void genpopulation(int *a, int *b, int *c, unsigned int n) {
  // LET's do this sheeeettt
  srand(time(NULL));

  //init infected + diseasettl
  for (int i = 0; i < n*n; i++) {
     if(((rand() % 10000)/10000.) < INFECTION_SEED_RATIO){
        SetBit(a,i);
        SetBitsDttl(c,i,DISEASE_TTL);
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
      SetBitsDttl(c,i,DISEASE_TTL);
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
	if(!TestBit(a, nborindex)){
      return nborindex;
  }else{
    return -1;
  }
  //a[nborindex];

} // Neighbors in current row

inline int nborTop(int *a, int x, int b, int n){
	int nborindex = (((x/n * n) + n*n - n) % (n*n)) + (x % n +(n-b)) % n;
  if(!TestBit(a, nborindex)){
      return nborindex;
  }else{
    return -1;
  }
  //a[nborindex];
} // Neighbors in top row

inline int nborBot(int *a, int x, int b, int n){
	int nborindex = (((x/n * n) + n) % (n*n)) + (x % n +(n-b)) % n;
  if (!TestBit(a, nborindex)){
    return nborindex;
  }else{
    return -1;
  }
} // Neighbors in bottom row

inline void liveOrDie(int *a, int *a_tmp, int *b, int *c, unsigned int n, int index){
  int currentVal = TestBit(a, index);
  if(currentVal == INFECTED && GetBitsDttl(c, index)>0 ){
    //infect others
    int nbors [8] = {nborTop(a, index, -1, n), nborTop(a, index, 0, n), nborTop(a, index, 1, n),
                     nborRow(a, index, -1, n), nborRow(a, index, 1, n),
                     nborBot(a, index, -1, n), nborBot(a, index, 0, n), nborBot(a, index, 1, n)};
    int to_infect = 0;
    for (int i = 0; i < 8; i++) {
      if(nbors[i] !=-1){
        to_infect++;
        if(TestBit(b, nbors[i]) == VACCINATED){
          if(((rand() % 10000)/10000.) > VAC_INFECTION_CHANCE){
             nbors[i] = -1;
             to_infect--;
          }
        }else{
          if(((rand() % 10000)/10000.) > UNVAC_INFECTION_CHANCE){
               nbors[i] = -1;
               to_infect--;
          }
        }
      }
    }
    int dttl=GetBitsDttl(c, index);
    int left_to_infect=to_infect;
    for (int i = 0; i < 8; i++) {
      if(nbors[i] != -1){
          if(left_to_infect-dttl<0){
            //printf("%d infected %d\n", index, nbors[i]);
            SetBit(a_tmp, nbors[i]);
            SetBitsDttl(c, nbors[i], dttl - (int)floor(log2(to_infect)));
            left_to_infect--;
          }

      }
    }
    //decrement own disease ttl
    SetBitsDttl(c, index, GetBitsDttl(c, index)+1);
    SetBit(a_tmp, index);
    return;
  }else{
    return;
  }

}

void display(int *a, int *b, int *c, unsigned int n, int arrsize){
  for(int i=0; i<n*n; i++){
      if(TestBit(a,i)){
        if(GetBitsDttl(c,i) > 0){
          printf("%s1 ", MAG);
        }else{
          printf("%s1 ", RED);
        }
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
  /*
  for (int i = 0; i < n*n; i++) {
    printf("%s", NRM);
    printf("\n%d: %d ttl\n", i, GetBitsDttl(c,i));
  }
  */
  printf("\n");
  printf("%s", NRM);

}
int* herdsim(int *a, int *b, int *c, unsigned int n, unsigned int iter, int *infectedcount, int arrsize) {
//doesnt get much more dope than this tbh
//print initail
  if(INTERACTIVE){
    printf("----------------init----------------\n");
    display(a,b,c,n, arrsize);
    printf("-------------------------------------\n");
    getchar();
  }
  int *a_tmp;
  a_tmp = (int *)malloc(((n*n + sizeof(int)*CHAR_BIT - 1) & ~(sizeof(int)*CHAR_BIT - 1))/CHAR_BIT);
  int cnt = 0;

  for(int i = 0; i < iter; i++){

    // Phase 1: Check neighbors of each cell, update cells in temp array
    cilk_for(int j = 0; j < n*n; j++){
      liveOrDie(a, a_tmp, b, c, n, j);
    }
    // Phase 2: swap tmp and normal
    a=(int *)((uintptr_t)a_tmp^(uintptr_t)a);
    a_tmp=(int *)((uintptr_t)a_tmp^(uintptr_t)a);
    a=(int *)((uintptr_t)a^(uintptr_t)a_tmp);

    // Phase 3: display
    if(INTERACTIVE){
      system("clear");
      printf("infected: %d iteration %d\n", countinfected(a ,n, arrsize), i);
      display(a,b,c,n, arrsize);
      getchar();
    }

  }
  return a;

}
