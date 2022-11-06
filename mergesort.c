#include "mergesort.h"
#include "recursiveMergesort.h"
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <limits.h>
#include "utils.h"

int* mergeN(int* p, int len, int nHeads) {
    int* heads = (int*)malloc(sizeof(int) * nHeads); // Positions on the subarrays    
    int* headValues = malloc(sizeof(int) * nHeads); // Values of the arrays
    int* merged = malloc(sizeof(int) * len);    

    int subSize = len / nHeads; // Size of each subarray
    int i;
    for(i = 0; i < nHeads; i++) {
        heads[i] = 0;
        headValues[i] = p[subSize * i];
    }

    int minHead;
    for(i = 0; i < len; i++) {
        minHead = argmin(headValues, nHeads);  // Find head with min value
        merged[i] = headValues[minHead];    // Store value
        heads[minHead]++;    // Move pointer on the min subarray
        // Update head values
        if (heads[minHead] >= subSize) {    // End of the subarray
            headValues[minHead] = INT_MAX;
        } else {
            headValues[minHead] = p[minHead * subSize + heads[minHead]];  // Next value on the subarray
        }
    }
    
    free(heads);
    free(headValues);
    return merged;
}

void parallelMergesort1(int* p, int start, int end, int rank, int word) {
    // Split
    int size = (end - start) / word;
    int* subarray = malloc(sizeof(int) * size);

    p = p + start;  //align to start -> this does not change the array outside the function
    MPI_Scatter(p, size, MPI_INT, subarray, size, MPI_INT, 0, MPI_COMM_WORLD);

    // Sort
    recursiveMergesort(subarray, 0, size);
    
    //Merge
    MPI_Gather(subarray, size, MPI_INT, p, size, MPI_INT, 0, MPI_COMM_WORLD);
    free(subarray);
    
    if (rank == 0) {
        p = mergeN(p, size * word, word);
    }
}
