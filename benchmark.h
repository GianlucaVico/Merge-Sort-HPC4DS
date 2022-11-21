#ifndef BENCHMARK_H
#define BENCHMARK_H

typedef void (*sort)(int*, int);

const int SIZE_INDEX;
const int REPETITIONS_INDEX;
const int SORT_INDEX;
const int SEED_INDEX;

void getArg(int argv, const char* argc[], int index, int* arg) ;

void runTest(int size, int repetitions, sort mergeSort, double* avg, double* std, int rank, int world) ;

#endif
