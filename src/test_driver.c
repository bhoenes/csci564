/*
 ============================================================================
 Name        : test_driver.c
 Author      : Brian Hoenes
 Version     : 03.18.2012
 Description : Driver for square matrix multiplication to test optimization
                strategies
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <cblas.h>
#include "bch_atlas.h"
#include "stopwatch.h"


#define TRUE 1
#define FALSE 0
#define NSTEPS 10

// ***********************Utility functions*********************

void rand_square_double_matrix (const int seed, const int n, double a[]) {
  int i, j;
  srand(seed);
  for(i = 0; i < n; i++) {
    for(j = 0; j < n; j++) {
      a[i*n + j] = ((double)rand()/(double)RAND_MAX);
    }
  }
}

void zero_square_double_matrix (const int n, double a[]) {
  int i;
  for(i = 0; i < n*n; i++) {
      a[i] = 0.0;
  }
}

void assert_equal(const int n, const double a[], const double b[], const double epsilon) {
	int i;
	for (i = 0; i < n*n; i++) {
		assert(fabs(a[i] - b[i]) < epsilon);
	}
}

void print_square_matrix(const int n, const double a[]) {
  int i, j;
  for (i = 0; i < n; i++) {
    int row = i*n;
    for(j = 0; j < n; j++) {
      printf("%4.2f ", a[row+j]);
    }
    printf("\n");
  }
}

// *******************Test Driver********************************

int main(void) {
	const int size[NSTEPS] = {2, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000};  // specification of array sizes
	static const double maxtime = 2.0; // number of seconds to time each step size
	static const double epsilon = 0.00001; // maximum floating point error allowed between doubles to be considered equal
	
	int mult_count;  // count of number of multiplications  
	int i;  // index variables for looping
	int mflops[2];  // megaflops calculation 0 is test case, 1 is base case
	double* a;
	double* b;
	double* ctest;
	double* cbase;
	stopwatch* sw = stopwatch_new();
	
	for(i = 0; i < 1; i++) {
	  int n = size[i];
	  int n2 = n * n;
	  a = (double*) malloc(n2*sizeof(double));
 	  b = (double*) malloc(n2*sizeof(double));
 	  ctest = (double*) malloc(n2*sizeof(double));
 	  cbase = (double*) malloc(n2*sizeof(double));
 	  rand_square_double_matrix(i, n, a);
 	  rand_square_double_matrix(i+1, n, b);
    zero_square_double_matrix(n, ctest);
	  zero_square_double_matrix(n, cbase);
	  	  
	  mult_count = 0;
	  stopwatch_restart(sw);
	  while(stopwatch_time(sw) < maxtime) {
      my_dgemm(n, a, b, ctest);
      mult_count++;
		}
	  stopwatch_stop(sw);
	  mflops[0] = (int)((0.000002*n2*mult_count)/stopwatch_time(sw));
	  
	  mult_count = 0;
	  stopwatch_restart(sw);
	  while(stopwatch_time(sw) < maxtime) {
      cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, a, n, b, n, 1.0, cbase, n);
      mult_count++;
		}
	  stopwatch_stop(sw);
	  
	  printf("A\n");
	  print_square_matrix(n, a);
	  printf("B\n");
	  print_square_matrix(n, b);	  
	  printf("my_dgemm\n");
	  print_square_matrix(n, ctest);
	  printf("cdgemm\n");
	  print_square_matrix(n, cbase);
	  
	  assert_equal(n, ctest, cbase, epsilon);
	  mflops[1] = (int)((0.000002*n2*mult_count)/stopwatch_time(sw));
	  
	  printf("%d, %d, %d\n", n, mflops[0], mflops[1]);

    free(a);
    free(b);
    free(ctest);
    free(cbase);
  }
	stopwatch_delete(sw);
	return 0;
}

