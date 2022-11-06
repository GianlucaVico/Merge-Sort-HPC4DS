#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "mergesort.h"
#include "utils.h"

void testMerge(int *p, int len) {
    int* tmp;
    printf("-----------------\n");
    printf("Array: ");
    printIntArray(p, len);
    tmp = mergeN(p, 16, 4);
    printf("Merged: ");
    printIntArray(tmp, len);
    printf("-----------------\n");
    free(tmp);
}

void testParallelMergeSort1(int *p, int len) {   
    int rank, word;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &word);

    if(rank == 0) {
        printf("-----------------\n");
        printf("Array: ");
        printIntArray(p, len);
    }
    parallelMergesort1(p, 0, len, rank, word);
    if(rank == 0) {
        printf("Sorted: ");
        printIntArray(p, len);
        printf("-----------------\n");
    }
}

int main(int argc, char const *argv[])
{
    MPI_Init(NULL, NULL);
    int rank, word;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &word);
    
    const int L = 16;
    int m1[16] = {1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16};
    int m2[16] = {5,6,7,8, 1,2,3,4, 13,14,15,16, 9,10,11,12};
    int m3[16] = {1,5,9,13, 2,6,10,14, 3,7,11,15, 4,8,12,16};
    int m4[15] = {1,5,9,13, 2,6,10,14, 3,7,11,15, 4,8,12};
    
    if(rank == 0) {
        printf("#############\n");
        printf("Merge part\n");
        testMerge(m1, L);
        testMerge(m2, L);
        testMerge(m3, L);
        testMerge(m4, L-1);
    }

    if(rank == 0) {
        printf("#############\n");
        printf("Sort part\n");
    }
    testParallelMergeSort1(m1, L);
    testParallelMergeSort1(m2, L);
    testParallelMergeSort1(m3, L);
    //FIX: it does not work if the length of the array is not divisible by the number of processes
    testParallelMergeSort1(m4, L-1); 
    return 0;
}
