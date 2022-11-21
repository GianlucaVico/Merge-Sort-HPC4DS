#include "benchmark.h"
#include "utils.h"
#include <stdlib.h>
#include <mpi.h>

const int SIZE_INDEX = 1;
const int REPETITIONS_INDEX = 2;
const int SORT_INDEX = 3;
const int SEED_INDEX = 4;


/*
Get an int argument

Args:
    int argv: number of arguments
    char* argc[]: arguments
    int index: index of the arguments
    int* arg: output argument
*/
void getArg(int argv, const char* argc[], int index, int* arg)  {
    if(argv <= index) { // Repetitions not given
        *arg = 1;
    } else {
        *arg = atoi(argc[index]);
    }
}

/*
Run the benchmark

Args:
    int size: size of the array
    int repetitions: number of repetitions of the experiment
    void* sort: sorting method
    double* avg: output avg time
    double* std: output std of the time
*/
void runTest(int size, int repetitions, sort mergeSort, double* avg, double* std, int rank, int world) {
    int i;  // Iterator
    double* times = malloc(sizeof(double) * repetitions); // Measurements
    double startTime;   // Start time of each experiment
    int* p = NULL;      // Array to sort

    for(i = 0; i < repetitions; i++) {
        if(rank == 0) {
            generatateRandom(p, size, 0, 1000);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        startTime = MPI_Wtime();
        mergeSort(p, size);
        times[i] = MPI_Wtime() - startTime;
        if(rank == 0) {
            free(p);
        }
    }

    // All the processes executes this -> it does not affect the benchmark
    getMean(times, repetitions, avg);
    getStd(times, repetitions, *avg, std);
    free(times);
}
