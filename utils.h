#ifndef UTILS_H
#define UTILS_H

void printIntArray(int *p, int len) ;
void generatateRandom(int* p, int len, int min, int max) ;
int argmin(int* p, int len) ;
void copyArray(int* source, int* dest, int len);
void getMean(double* p, int len, double* avg) ;
void getStd(double* p, int len,double mean, double* std) ;
#endif
