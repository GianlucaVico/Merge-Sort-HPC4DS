#include "benchmark.h"
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include "recursiveMergesort.h"
#include "mergesort.h"

int main(int argc, char const *argv[])
{
    MPI_Init(NULL, NULL);
    int rank = 0, world = 1;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);

    int size, rep, sortAlgo, seed;
    getArg(argc, argv, SIZE_INDEX, &size);
    getArg(argc, argv, REPETITIONS_INDEX, &rep);
    getArg(argc, argv, SORT_INDEX, &sortAlgo);
    getArg(argc, argv, SEED_INDEX, &seed);
    
    srand(seed);
    
    double avg, std;
    sort mergeSort;
    switch (sortAlgo)
    {
    case 0:
        mergeSort = recursiveMergesort;
        break;
    case 1:
        mergeSort = parallelMergesort1;
        break;
    case 2:
        mergeSort = parallelMergesort2;
        break;
    default:
        printf("Invalid merge sort: %d\n", sortAlgo);
        MPI_Finalize();
        return 1;
    }    
    runTest(size, rep, mergeSort, &avg, &std, rank, world);
    if(rank == 0) {
        printf("Mean: %5.6e\n", avg);
        printf("Std: %5.6e\n", std);
    }
    MPI_Finalize();
    return 0;
}
