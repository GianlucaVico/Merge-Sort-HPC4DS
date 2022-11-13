#include "mergesort.h"
#include "recursiveMergesort.h"
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <limits.h>
#include "utils.h"
#include <math.h>

/*
Merge a single array made up of several concatenated subarrays in a sorted way

Args:
    int* p: concatenated subarrays
    int len: total length
    int nHeads: number of subarray
Retuns
    Pointer to the marged array
*/
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

/*
Merge 2 arrays

Args:
    int* p1: first array
    int l1: length of the first array
    int* p2: second array
    int l2: length of the second array
Returns
    Pointer to the merged array
*/
int* merge2(int* p1, int l1, int* p2, int l2) {
    int* merged = malloc(sizeof(int) * (l1, + l2));    
    int i = 0, j = 0, k = 0; // Iterators
    while(i < l1, j < l2) {
        if(p1[i] < p2[j]) {
            merged[k] = p1[i];
            i++;
        } else {
            merged[k] = p2[j];
            j++;
        }
        k++;
    }
    // Remaining part of p1
    while(i < l1) {
        merged[k] = p1[i];
        i++;
        k++;
    }
    // Remaining part of p2
    while(j < l2) {
        merged[k] = p2[j];
        j++;
        k++;
    }
    return merged;
}

/*
Merge the subarrays in a parallel way

Args:
    int* p: subarray
    int len: length of the subarray (it should be the same for each process)
    int rank: rank of the process
    int world: size of the WORD communicator
Returns:
    The pointer to the merged arrat if rank is 0, NULL otherwise.

Communication pattern:
    depth 0: 0
             | \_____
    depth 1: 0       4          i = 3 -> 2^i=8
             | \     | \
    depth 2: 0   2   4   6      i = 2 -> 2^2=4 
             |\  |\  |\  |\
    depth 3: 0 1 2 3 4 5 6 7    i = 1 -> 2^1=2

A process finishes when it sends the data.
*/
int* parallelMerge(int* p, int len, int rank, int world) {
    int depth = (int)ceil(log2(world));
    int sendTo;
    int recvFrom;
    int i = 1;  // 2^i
    MPI_Status status;
    int* buff, tmp;

    // Use 2^i instead of the actual iterator
    while(i < pow(2, depth)) {
        if(rank % (2 * i) == 0) { // Receive from rank + 2^(i - 1)
            recvFrom = rank + i;      
            if(recvFrom < world) {    // The sender exists
                MPI_Recv(buff, len * i, MPI_INT, recvFrom, 0, MPI_COMM_WORLD, &status);
                tmp = p;
                p = merge2(tmp, len * i, buff, len * i);
                free(tmp);
                free(buff);
            }
        }else { // Send to rank - 2^(i-1)
            sendTo = rank - i;
            MPI_Send(p, len * i, MPI_INT, sendTo, 0, MPI_COMM_WORLD);
            free(p);
            return NULL;
        }   
        i*=2;    
    }
    return p;
}

void parallelMergesort1(int* p, int size, int rank, int world) {
    // Split
    int* subarray = malloc(sizeof(int) * size);

    MPI_Scatter(p, size, MPI_INT, subarray, size, MPI_INT, 0, MPI_COMM_WORLD);

    // Sort
    recursiveMergesort(subarray, 0, size);
    
    //Merge
    MPI_Gather(subarray, size, MPI_INT, p, size, MPI_INT, 0, MPI_COMM_WORLD);
    free(subarray);
    
    if (rank == 0) {
        p = mergeN(p, size * world, world);
    }
}

// TODO use len/size instead of start and end
void parallelMergesort2(int* p, int size, int rank, int world) {
    // Split    
    int* subarray = malloc(sizeof(int) * size);

    MPI_Scatter(p, size, MPI_INT, subarray, size, MPI_INT, 0, MPI_COMM_WORLD);

    // Sort
    recursiveMergesort(subarray, 0, size);
    
    //Merge
    p = parallelMerge(subarray, size, rank, world);
    free(subarray);
}
