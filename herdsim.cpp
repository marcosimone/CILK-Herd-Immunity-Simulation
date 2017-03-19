// Function declarations file
#include "herdsim.h"

int *infectedcount;

using namespace std;

int main(int argc, char **argv)
{
	unsigned int n;
	unsigned int iter;
	int *a; //infected?
  int *b; //vaccinated?
  int arrsize;
  long t1, t2;
	// Size calculation

	// Initialize life matrix through either reading from standard input or initializing as required.

	if(argc < 3)
	{
		printf("Usage : %s [r] <n> <number of iterations>\n", argv[0]);
		exit(-1);
	}

	// Read from file
	if(argv[1][0] == 'r')
	{
		n = (unsigned int)atoi(argv[2]);
		iter = (unsigned int)atoi(argv[3]);
    /*
    int size_in_bits=n*n;
    int roundedupbits=(size_in_bits + [size of int in bits] - 1) & ~([size of int in bits] - 1);
    int size_in_bytes=roundedupbits/CHAR_BIT;
    */
    int size_in_bytes=((n*n + sizeof(int)*CHAR_BIT - 1) & ~(sizeof(int)*CHAR_BIT - 1))/CHAR_BIT;
		a = (int *)malloc(size_in_bytes);
    b = (int *)malloc(size_in_bytes);
    arrsize=size_in_bytes/sizeof(int);

    printf("size of array(ints): %d\nsize of array(bytes): %d\n", arrsize, size_in_bytes);
		if(a == NULL) {
			printf("Malloc failed .. Exiting\n");
			exit(-1);
		}

		readpopulation(a,b,n, argv[4]);
	}
	// Generate random data
	else
	{
		n = (unsigned int)atoi(argv[1]);
		iter = (unsigned int)atoi(argv[2]);
    int size_in_bytes=((n*n + sizeof(int)*CHAR_BIT - 1) & ~(sizeof(int)*CHAR_BIT - 1))/CHAR_BIT;
		a = (int *)malloc(size_in_bytes);
    b = (int *)malloc(size_in_bytes);
    arrsize=size_in_bytes/sizeof(int);
		if(a == NULL) {
			printf("Malloc failed .. Exiting\n");
			exit(-1);
		}

    printf("size of array(ints): %d\nsize of array(bytes): %d\n", arrsize, size_in_bytes);
		genpopulation(a,b,n);

		int infected = countinfected(a,n, arrsize);
	}
/*
	//Debug array
	infectedcount = (int *)malloc(sizeof(int)*n);

	//Initialize infectedcount array
	#if DEBUG == 1
		for(int i = 0; i < 10; i++)
			infectedcount[i] = 0;
	#endif
*/


	//t1 = example_get_time();
  //herdsim(a,n,iter,infectedcount);
  //t2 = example_get_time();
  //cout << "Standard library function time for life: " << t2 - t1 << endl;

/*
	// Print the infectedcount array
	#if DEBUG == 1

		for(int i = 0; i < 10; i++)
			printf("%d ",infectedcount[i]);
		printf("\n");
	#endif
*/
  printf("infected: %d\n", countinfected(a, n, arrsize));
	return 0;

}

//The countlive function to be used for calculating the number of live cells.
int countinfected(int *a, unsigned int n, int arrsize)
{
	int sum = 0;
	for(int i = 0; i < arrsize; i++)
	{
		sum += __builtin_popcount(a[i]);
	}

	return sum;
}
