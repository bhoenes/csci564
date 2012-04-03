/*
 ============================================================================
 Name        : bch_atlas.c
 Author      : Brian Hoenes
 Version     : 03.18.2012
 Description : Implementation of square matrix multiplication for computer
                architecture optimization research
 ============================================================================
 */

#include "bch_atlas.h"

static int T = 33;
static int ifactor = 3;
static int jfactor = 4;
static int kfactor = 8;

void setT(int newT) {
	T = newT;
}

void my_dgemm(const int n, const double A[], const double B[], double C[]) {
  int i, j, k;
  for (i=0; i<n; i++)
    for (j=0; j<n; j++)
      for (k=0; k<n; k++)
        C[i*n+j]=C[i*n+j]+A[i*n+k]*B[k*n+j];
}

void step00(const int n, const double A[], const double B[], double C[]) {
	int i, j, k, in, inpj, knpj;
	double r;
	for(i=0; i<n; i++) {
		in = i * n;
		for(k=0; k<n; k++) {
			knpj = k * n;
			r = B[in+k];
			inpj = in;
			for(j=0; j<n; j++) {
				C[inpj++]+=r*A[knpj++];
			}
		}
	}
}


void step01(const int n, const double A[], const double B[], double C[]) {
	int i, j, k, jj, kk, in, kn;
	double r;
	for(kk=0; kk<n; kk+=T) {
		for(jj=0; jj<n; jj+=T) {
			for(i=0; i<n; i++) {
				in = i * n;
				for(k=kk; k < MIN(kk+T, n); k++) {
					kn = k*n;
					r = B[in+k];
					for(j=jj; j < MIN(jj+T, n); j++) {
						C[in+j]+=r*A[kn+j];
					}
				}
			}					
		}
	}
}

void step02(const int n, const double A[], const double B[], double C[]) {
	int i, j, k, ii, jj, kk, in , kn;
	double r;
	for(ii=0; ii<n; ii+=T) {
		for(kk=0; kk<n; kk+=T) {
			for(jj=0; jj<n; jj+=T) {
				for(i=ii; i < MIN(ii+T,n); i++) {
					in = i * n;
					for(k=kk; k < MIN(kk+T,n); k++) {
						kn = k * n;
						r = B[in+k];
						for(j=jj; j < MIN(jj+T, n); j++) {
							C[in+j] += r * A[kn+j];
						}
					}
				}
			}
		}
	}
}

void step03(const int n, const double A[], const double B[], double C[]) {
	int i, j, k, ii, jj, kk, in;
	double r;
	for(ii=0; ii<n; ii+=T) {
		for(kk=0; kk<n; kk+=T) {
			for(jj=0; jj<n; jj+=T) {
				for(i=ii; i < MIN(ii+T,n); i++) {
					in = i * n;
					for(j=jj; j < MIN(jj+T,n); j++) {
						r = C[in+j];
						for(k=kk; k < MIN(kk+T,n); k++) {
							r += B[in+k] * A[k*n+j];
						}
						C[in+j] = r;
					}
				}
			}
		}
	}
}

void step04(const int n, const double A[], const double B[], double C[]) {
	int i, j, k, ii, jj, kk, in;
	double r;
	int t = T;
	for(ii=0; ii<n; ii+=t) {
		for(kk=0; kk<n; kk+=t) {
			for(jj=0; jj<n; jj+=t) {
				for(i=ii; i < MIN(ii+t,n); i++) {
					in = i * n;
					for(j=jj; j < MIN(jj+t,n); j++) {
						r = C[in+j];
						if(
						for(k=kk; k < kk+t; k+=kfactor) {
							r += B[in+k] * A[k*n+j];
						}
						for(k=kk; k < n; k++) {
							r += B[in+k] * A[k*n+j];
						}

						C[in+j] = r;
					}
				}
			}
		}
	}
}
