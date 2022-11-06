#ifndef MERGESORT_H
#define MERGESORT_H

int* mergeN(int* p, int len, int n_heads) ;

void parallelMergesort1(int* p, int start, int end, int rank, int word) ; 

#endif
