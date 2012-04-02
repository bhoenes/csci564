/*
 ============================================================================
 Name        : bch_atlas.c
 Author      : Brian Hoenes
 Version     : 03.18.2012
 Description : Implementation of square matrix multiplication for computer
                architecture optimization research
 ============================================================================
 */

#ifndef BCHATLAS_H_
#define BCHATLAS_H_
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
void my_dgemm(const int n, const double A[], const double B[], double C[]);
void step00(const int n, const double A[], const double B[], double C[]);
void step01(const int n, const double A[], const double B[], double C[]);
void step01_tile(const int n, const int t, const double A[], const double B[], double C[]);
void step02(const int n, const double A[], const double B[], double C[]);
void step03(const int n, const double A[], const double B[], double C[]);
void step04(const int n, const double A[], const double B[], double C[]);
void step05(const int n, const double A[], const double B[], double C[]);
#endif /* BCHATLAS_H_ */


