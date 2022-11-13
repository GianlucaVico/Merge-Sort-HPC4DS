#ifndef MERGESORT_H
#define MERGESORT_H

int* mergeN(int* p, int len, int n_heads) ;
int* merge2(int* p1, int l1, int* p2, int l2) ;
int* parallelMerge(int* p, int len, int rank, int world);
void parallelMergesort1(int* p, int size, int rank, int world) ; 
void parallelMergesort2(int* p, int size, int rank, int world) ; 

#endif
