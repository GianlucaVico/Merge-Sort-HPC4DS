#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "mergesort.h"
#include "utils.h"

void testMergeN(int *p, int len) {
    int* tmp = malloc(sizeof(int) * len);
    copyArray(p, tmp, len);
    printf("-----------------\n");
    printf("Array: ");
    printIntArray(tmp, len);
    //tmp = mergeN(p, 16, 4);
    mergeN(tmp, len, 4);
    printf("Merged: ");
    printIntArray(tmp, len);
    printf("-----------------\n");
    free(tmp);
}

void testParallelMergeSort1(int *p, int len) {   
    int rank, world;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);
    int* tmp = malloc(sizeof(int) * len);
    copyArray(p, tmp, len);
    if(rank == 0) {
        printf("-----------------\n");
        printf("Array: ");
        printIntArray(tmp, len);
    }
    parallelMergesort1(tmp, len, rank, world);
    if(rank == 0) {
        printf("Sorted: ");
        printIntArray(tmp, len);
        printf("-----------------\n");
    }
    free(tmp);
}

void testParallelMergeSort2(int *p, int len) {   
    int rank, world;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);
    int* tmp = malloc(sizeof(int) * len);
    copyArray(p, tmp, len);
    if(rank == 0) {
        printf("-----------------\n");
        printf("Array: ");
        printIntArray(tmp, len);
        fflush(stdout);
    }
    parallelMergesort2(tmp, len, rank, world);
    if(rank == 0) {
        printf("Sorted: ");
        printIntArray(tmp, len);
        printf("-----------------\n");
    }
    free(tmp);
}

// void testParallelMerge(int *p, int len) {    
//     int rank, world;
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &world);
//     int* tmp = malloc(sizeof(int) * len);
//     int* out = malloc(sizeof(int) * len);
//     copyArray(p, tmp, len);
//     if(rank == 0) {
//         printf("-----------------\n");
//     }
//     //printf("Array %d/%d: ", rank, world);
//     //printIntArray(p, len);    
//     parallelMerge(tmp, len, rank, world, out);
//     if(rank == 0) {
//         printf("Merged (%d): ", len * world);
//         printIntArray(out, len * world);
//         printf("-----------------\n");
//     }
//     free(out);
//     free(tmp);
// }

int main(int argc, char const *argv[]) {
    MPI_Init(NULL, NULL);
    int rank, world;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);
    
    const int L = 16;
    int m1[16] = {1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16};
    int m2[16] = {5,6,7,8, 1,2,3,4, 13,14,15,16, 9,10,11,12};
    int m3[16] = {1,5,9,13, 2,6,10,14, 3,7,11,15, 4,8,12,16};
    int m4[15] = {1,5,9,13, 2,6,10,14, 3,7,11,15, 4,8,12};
    
    if(rank == 0) {
        printf("#############\n");
        printf("Merge part\n");
        testMergeN(m1, L);
        testMergeN(m2, L);
        testMergeN(m3, L);
        testMergeN(m4, L-1);
    }

    if(rank == 0) {
        printf("#############\n");
        printf("ParallelMergeSort1 part\n");
    }
    testParallelMergeSort1(m1, L);
    testParallelMergeSort1(m2, L);
    testParallelMergeSort1(m3, L);
    testParallelMergeSort1(m4, L-1);
    
    int pm1[4] = {1,2,3,4};
    int i;
    for(i = 0; i < 4; i++) {
        pm1[i] *= rank + 1;
    }

    int pm2[4] = {1,5,9};
    for(i = 0; i < 4; i++) {
        pm2[i] += rank;
    }

    if(rank == 0) {
        printf("#############\n");
        printf("Parallel Merge\n");
    }
    // testParallelMerge(pm1, 4);
    // testParallelMerge(pm2, 3);

    if(rank == 0) {
        printf("#############\n");
        printf("ParallelMergeSort2 part\n");
    }

    int m5[14] = {1,5,9,13, 2,6,10,14, 3,7,11,15, 4,8};
    testParallelMergeSort2(pm1, L);
    testParallelMergeSort2(pm2, L);
    testParallelMergeSort2(m3, L);
    testParallelMergeSort2(m4, L-1);
    testParallelMergeSort2(m5, L-2); 
    
    
    MPI_Barrier(MPI_COMM_WORLD); //Wait for print
    
    MPI_Finalize();
    return 0;
}
