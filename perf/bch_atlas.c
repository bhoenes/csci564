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

// basic triple loop per the project description
void my_dgemm(const int n, const double A[], const double B[], double C[]) {
  int i, j, k;
  for (i=0; i<n; i++)
    for (j=0; j<n; j++)
      for (k=0; k<n; k++)
        C[i*n+j]=C[i*n+j]+A[i*n+k]*B[k*n+j];
}

// steps below taken from Parello “On increasing architecture awareness in 
// program optimizations to bridge the gap between peak and sustained processor
// performance: matrix-multiply revisited”, Parello, D., Temam, O., Verdun, J.,
// Proceedings of the 2002 ACM/IEEE conference on Supercomputing.

// basic starting algorithm for baseline comparison
// this result will be speedup = 1.00
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

// tile matrix to optimize L1 cache utilization 
void step01(const int n, const double A[], const double B[], double C[]) {
	int i, j, k, jj, kk, in, kn;
	int t = 32;
	double r;
	for(kk=0; kk<n; kk+=t) {
		for(jj=0; jj<n; jj+=t) {
			for(i=0; i<n; i++) {
				in = i * n;
				for(k=kk; k < MIN(kk+t, n); k++) {
					kn = k*n;
					r = B[in+k];
					for(j=jj; j < MIN(jj+t, n); j++) {
						C[in+j]+=r*A[kn+j];
					}
				}
			}					
		}
	}
}

// reorder index access to 
void step02(const int n, const double A[], const double B[], double C[]) {
	int i, j, k, ii, jj, kk, in , kn;
	int t = 12;
	double r;
	for(ii=0; ii<n; ii+=t) {
		for(kk=0; kk<n; kk+=t) {
			for(jj=0; jj<n; jj+=t) {
				for(i=ii; i < MIN(ii+t,n); i++) {
					in = i * n;
					for(k=kk; k < MIN(kk+t,n); k++) {
						kn = k * n;
						r = B[in+k];
						for(j=jj; j < MIN(jj+t, n); j++) {
							C[in+j] += r * A[kn+j];
						}
					}
				}
			}
		}
	}
}

// create store queue to reduce number of writes in inner loop
void step03(const int n, const double A[], const double B[], double C[]) {
	int i, j, k, ii, jj, kk, in;
	int t = 32;
	double r;
	for(ii=0; ii<n; ii+=t) {
		for(kk=0; kk<n; kk+=t) {
			for(jj=0; jj<n; jj+=t) {
				for(i=ii; i < MIN(ii+t,n); i++) {
					in = i * n;
					for(j=jj; j < MIN(jj+t,n); j++) {
						r = C[in+j];
						for(k=kk; k < MIN(kk+t,n); k++) {
							r += B[in+k] * A[k*n+j];
						}
						C[in+j] = r;
					}
				}
			}
		}
	}
}

// unroll inner loop to acheive increase instruction level parallelism
void step04(const int n, const double A[], const double B[], double C[]) {
	int i, j, k, ii, jj, kk, in;
	double r;
	int t = 32;
	for(ii=0; ii<n; ii+=t) {
		for(kk=0; kk<n; kk+=t) {
			for(jj=0; jj<n; jj+=t) {
				for(i=ii; i < MIN(ii+t,n); i++) {
					in = i * n;
					for(j=jj; j < MIN(jj+t,n); j++) {
						r = C[in+j];
						if(kk+t < n) {
							for(k=kk; k < kk+t; k+=8) {
								int inpk = in+k;
								r += B[inpk] * A[k*n+j];
								r += B[inpk+1] * A[(k+1)*n+j];
								r += B[inpk+2] * A[(k+2)*n+j];
								r += B[inpk+3] * A[(k+3)*n+j];
								r += B[inpk+4] * A[(k+4)*n+j];
								r += B[inpk+5] * A[(k+5)*n+j];
								r += B[inpk+6] * A[(k+6)*n+j];
								r += B[inpk+7] * A[(k+7)*n+j];
							}
						} else {
							for(k=kk; k < n; k++) {
								r += B[in+k] * A[k*n+j];
							}
						}
						C[in+j] = r;
					}
				}
			}
		}
	}
}

// like step 5 below but allow the compiler to unroll the inner loop
void step05a(const int n, const double A[], const double B[], double C[]) {
	int i, j, k, ii, jj, kk, i0n, i1n;
	double Rc00, Rc10, Rc01, Rc11, Rb00, Rb10, Ra0, Ra1;	
	int t = 64;
	for(ii=0; ii<n; ii+=t) {
		for(kk=0; kk<n; kk+=t) {
			for(jj=0; jj<n; jj+=t) {
				for(i=ii; i < MIN(ii+t,n); i+=2) {
					i0n = i * n;
					i1n = (i+1) * n;
					for(j=jj; j < MIN(jj+t,n); j+=2) {
						Rc00 = C[i0n+j];
						Rc10 = C[i1n+j]; 
						Rc01 = C[i0n+j+1];
						Rc11 = C[i1n+j+1];
						for(k=kk; k < MIN(kk+t,n); k++) {
								Rb00 = B[i0n+k];
								Rb10 = B[i1n+k];
								Ra0 = A[k*n+j];
								Ra1 = A[k*n+j+1];
								Rc00 += Rb00 * Ra0;
								Rc10 += Rb10 * Ra0;
								Rc01 += Rb00 * Ra1;
								Rc11 += Rb10 * Ra1;
						}
						C[i0n+j] = Rc00;
						C[i1n+j] = Rc10;
						C[i0n+j+1] = Rc01;
						C[i1n+j+1] = Rc11;
					}
				}
			}
		}
	}
}

// unroll the outer loops to use registers as top level of memory
// and unroll inner loops for ILP
void step05(const int n, const double A[], const double B[], double C[]) {
	int i, j, k, ii, jj, kk, i0n, i1n;
	double Rc00, Rc10, Rc01, Rc11, Rb00, Rb10, Ra0, Ra1;	
	int t = 32;
	for(ii=0; ii<n; ii+=t) {
		for(kk=0; kk<n; kk+=t) {
			for(jj=0; jj<n; jj+=t) {
				for(i=ii; i < MIN(ii+t,n); i+=2) {
					i0n = i * n;
					i1n = (i+1) * n;
					for(j=jj; j < MIN(jj+t,n); j+=2) {
						Rc00 = C[i0n+j];
						Rc10 = C[i1n+j]; 
						Rc01 = C[i0n+j+1];
						Rc11 = C[i1n+j+1];
						for(k=kk; k < MIN(kk+t,n); k+=4) {
								Rb00 = B[i0n+k];
								Rb10 = B[i1n+k];
								Ra0 = A[k*n+j];
								Ra1 = A[k*n+j+1];
								Rc00 += Rb00 * Ra0;
								Rc10 += Rb10 * Ra0;
								Rc01 += Rb00 * Ra1;
								Rc11 += Rb10 * Ra1;
								Rb00 = B[i0n+k+1];
								Rb10 = B[i1n+k+1];
								Ra0 = A[(k+1)*n+j];
								Ra1 = A[(k+1)*n+j+1];
								Rc00 += Rb00 * Ra0;
								Rc10 += Rb10 * Ra0;
								Rc01 += Rb00 * Ra1;
								Rc11 += Rb10 * Ra1;
								Rb00 = B[i0n+k+2];
								Rb10 = B[i1n+k+2];
								Ra0 = A[(k+2)*n+j];
								Ra1 = A[(k+2)*n+j+1];
								Rc00 += Rb00 * Ra0;
								Rc10 += Rb10 * Ra0;
								Rc01 += Rb00 * Ra1;
								Rc11 += Rb10 * Ra1;
								Rb00 = B[i0n+k+3];
								Rb10 = B[i1n+k+3];
								Ra0 = A[(k+3)*n+j];
								Ra1 = A[(k+3)*n+j+1];
								Rc00 += Rb00 * Ra0;
								Rc10 += Rb10 * Ra0;
								Rc01 += Rb00 * Ra1;
								Rc11 += Rb10 * Ra1;
						}
						C[i0n+j] = Rc00;
						C[i1n+j] = Rc10;
						C[i0n+j+1] = Rc01;
						C[i1n+j+1] = Rc11;
					}
				}
			}
		}
	}
}
