/*
 ============================================================================
 Name        : bch_atlas.c
 Author      : Brian Hoenes
 Version     : 03.18.2012
 Description : Implementation of square matrix multiplication for computer
                architecture optimization research
 ============================================================================
 */

void my_dgemm(const int n, const double A[], const double B[], double C[]) {
  int i, j, k;
  for (i=0; i<n; i++)
    for (j=0; j<n; j++)
      for (k=0; k<n; k++)
        C[i*n+j]=C[i*n+j]+A[i*n+k]*B[k*n+j];
}
