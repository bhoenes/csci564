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

int calc_mflops(const int flops_per_iteration, const int n, const double elapsed_time) {
  return (int)((0.000001*n*n*n*flops_per_iteration)/elapsed_time);
}

// *******************Test Driver********************************

void testFunc(void (*func) (const int n, const double A[], const double B[], double C[])) {
	const int size[NSTEPS] = {500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000};  // specification of array sizes
	static const double epsilon = 0.00001; // maximum floating point error allowed between doubles to be considered equal
	const double flops_per_iteration = 2.0;
	
	int i;  // index variables for looping
	double time[2]; // elapsed time. 0 is test case, 1 is base case
	int mflops[2]; // calculated mflops.  0 is test case, 1 is base case
	double* a;
	double* b;
	double* ctest;
	double* cbase;
	stopwatch* sw = stopwatch_new();
	
	for(i = 0; i < 3; i++) {
	  int n = size[i];
	  int n2 = n * n;
	  a = (double*) malloc(n2*sizeof(double));
 	  b = (double*) malloc(n2*sizeof(double));
 	  ctest = (double*) malloc(n2*sizeof(double));
 	  cbase = (double*) malloc(n2*sizeof(double));
 	  rand_square_double_matrix(i+10, n, a);
 	  rand_square_double_matrix(i+11, n, b);
    zero_square_double_matrix(n, ctest);
	  zero_square_double_matrix(n, cbase);
	  	  
	  stopwatch_restart(sw);
    func(n, a, b, ctest);
	  stopwatch_stop(sw);
    time[0] = stopwatch_time(sw);
	  
	  stopwatch_restart(sw);
    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, a, n, b, n, 1.0, cbase, n);
	  stopwatch_stop(sw);
	  
	  //printf("A\n");
	  //print_square_matrix(n, a);
	  //printf("B\n");
	  //print_square_matrix(n, b);	  
	  //printf("my_dgemm\n");
	  //print_square_matrix(n, ctest);
	  //printf("cdgemm\n");
	  //print_square_matrix(n, cbase);
	  
	  assert_equal(n, ctest, cbase, epsilon);
    time[1] = stopwatch_time(sw);
    mflops[0] = calc_mflops(flops_per_iteration, n, time[0]);
    mflops[1] = calc_mflops(flops_per_iteration, n, time[1]);
    	  
	  printf("%d, %5.2f, %d, %5.2f, %d\n", n, time[0], mflops[0], time[1], mflops[1]);

    free(a);
    free(b);
    free(ctest);
    free(cbase);
  }
	stopwatch_delete(sw);
}

int main(void) {
	printf("***step00***\n");
	testFunc(&step00);
	printf("***step01***\n");
	testFunc(&step01);
	printf("***step02***\n");
	testFunc(&step02);
	printf("***step03***\n");
	testFunc(&step03);

	return 0;	
}

