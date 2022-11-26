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
//TODO check if mergeN is called on subarrays with different lengths
void mergeN(int* p, int len, int nHeads) {
    int* heads = (int*)malloc(sizeof(int) * nHeads); // Positions on the subarrays    
    int* headValues = malloc(sizeof(int) * nHeads); // Values of the arrays


// start Benjamin  
    // Count how many elements are needed to make the array divisible. 
    int appendCount = 0;
    while((len+appendCount)%nHeads != 0)
    {
        appendCount++;
    }
    int subSize = (len+appendCount)/nHeads; // The length of each sub-array.
    int* adaptedP = malloc(sizeof(int)*(len+appendCount)); // An array with elements appended to make it divisible.
    int* merged = malloc(sizeof(int)*(len+appendCount)); // The final array.
    int skip = 0; // How many elements have been appended.
    // Goes through the original array backwards appending INT_MAX at the end of sub-arrays until the array is divisible.  
    int index = len-1; // Keeps track of position in the original array.
    for(int i = (len+appendCount)-1; i >= 0; i--)
    {
        // Checks if the position is the end of a sub-array and it is still necissary to append.
        if((i+1)%subSize == 0 && skip < appendCount)
        {
            adaptedP[i] = INT_MAX;
            skip++;
        }
        // Otherwise copy from the original array.
        else
        {
            adaptedP[i] = p[index];
            index--;
        }
    }
// end Benjamin


    int i;
    for(i = 0; i < nHeads; i++) {
        heads[i] = 0;
        headValues[i] = adaptedP[subSize*i];
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
            headValues[minHead] = adaptedP[minHead*subSize+heads[minHead]];
        }
    }
    

// start Benjamin
    // Remove the unnecessary elements from the end of the array which were appended earlier.
    //int* trimmedMerged = malloc(sizeof(int)*len);
    // for(int i = 0; i < len; i++)    
    // {
    //     trimmedMerged[i] = merged[i];
    // }
    //copyArray(merged, trimmedMerged, len);    
// end Benjamin
    // This should do the same of the previous block
    copyArray(merged, p, len);
    
    free(heads);
    free(headValues);
    free(merged);
    free(adaptedP);
    //free(trimmedMerged);
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
void parallelMerge2(int* p1, int l1, int* p2, int l2, int* merged) {
    int i = 0, j = 0, k = 0; // Iterators
    while((i < l1) && (j < l2)) {
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
    copyArray(p1+i, merged+k, l1- i);
    k += l1 - i;
    // Remaining part of p2    
    copyArray(p2+j, merged+k, l2- j);
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
void parallelMerge(int* p, int len, int rank, int world, int* out) {
    int depth = (int)ceil(log2(world));  // Depth of the tree
    int sendTo;     // Process where to send the array
    int recvFrom;   // Process that will send an array
    int i = 1;  // Iterator as powers of 2: 2^0, 2^1, 2^2 ...
    MPI_Status status;
    int* buff;  // To receive the array
    int count;  // How many elements we are receiving
    int* tmp;    

    int* subarray = malloc(sizeof(int) * len);  // Use to store the merged array at every iteration
    copyArray(p, subarray, len);

    while(i < pow(2, depth)) {
        if(rank % (2 * i) == 0) { // Receive from rank + 2^(i)            
            recvFrom = rank + i;      
            if(recvFrom < world) {    // The sender exists
                // Get size
                MPI_Probe(recvFrom, 0, MPI_COMM_WORLD, &status);
                MPI_Get_count(&status, MPI_INT, &count);
                
                // Receive msg
                buff = malloc(sizeof(int) * count);
                MPI_Recv(buff, count, MPI_INT, recvFrom, 0, MPI_COMM_WORLD, &status);                              
                
                // Merge                
                tmp = malloc(sizeof(int) * (len + count));
                parallelMerge2(subarray, len, buff, count, tmp);  // Merge the 2 arrays                
                free(subarray); 

                // Move result in subarray
                len += count;
                subarray = malloc(sizeof(int) * (len));                
                copyArray(tmp, subarray, len);                                                
                free(tmp);
                free(buff);
            }
        }else { // Send to rank - 2^(i)            
            sendTo = rank - i;                        
            MPI_Send(subarray, len, MPI_INT, sendTo, 0, MPI_COMM_WORLD);            
            break;        
        }   
        i*=2;    
    }
    if(rank == 0) {
        copyArray(subarray, out, len);
    }
    free(subarray);
}

/*
Parallel merge sort: parallel split, serial merge
Args:
    int* p: array to sort (in/out)
    int size: size of the array
    int rank: rank of the process
    int world: size of the communicator
*/
void parallelMergesort1_(int* p, int size, int rank, int world) {
    // Split
    int subsize = size / world;    // Size of the subarray
    int* subarray = malloc(sizeof(int) * subsize); 

    MPI_Scatter(p, subsize, MPI_INT, subarray, subsize, MPI_INT, 0, MPI_COMM_WORLD);

    // Sort
    //recursiveMergesort(subarray, subsize); // Use serial merge sort.
    quickSort(subarray, subsize); // Use serial quick sort.
    
    //Merge
    MPI_Gather(subarray, subsize, MPI_INT, p, subsize, MPI_INT, 0, MPI_COMM_WORLD);
    free(subarray);
    
    if (rank == 0) {
        int i;
        int tail = size - subsize * world;            // Length 
        int* extra = malloc(sizeof(int) * (tail));    // Store the tail
        for(i = 0; i < tail; i++) {
            extra[i] = p[size - tail + i];
        }
        //recursiveMergesort(extra, tail); // Use serial merge sort.
        quickSort(extra, tail); // Use serial quick sort.
        mergeN(p, subsize * world, world);
        int* tmp = malloc(sizeof(int) * size);
        parallelMerge2(p, subsize * world, extra, tail, tmp);
        copyArray(tmp, p, size);
        free(tmp);
        free(extra);
    }
}

/*
Parallel merge sort: parallel split, parallel merge
Args:
    int* p: array to sort (in/out)
    int size: size of the array
    int rank: rank of the process
    int world: size of the communicator
*/
void parallelMergesort2_(int* p, int size, int rank, int world) {
    // Split       
    int subsize = size / world;    // Size of the subarray
    int* subarray = malloc(sizeof(int) * subsize);    

    MPI_Scatter(p, subsize, MPI_INT, subarray, subsize, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Sort
    //recursiveMergesort(subarray, subsize); // Use serial merge sort.
    quickSort(subarray, subsize); // Use serial quick sort.    

    //Merge    
    if(rank == 0) {     // Processes 0 receives the results
        int i;
        // Sort the part of the array that wasn't scattered
        int tail = size - subsize * world;            // Length 
        int* extra = malloc(sizeof(int) * (tail));    // Store the tail
        for(i = 0; i < tail; i++) {
            extra[i] = p[size - tail + i];
        }
        //recursiveMergesort(extra, tail); // Use serial merge sort.
        quickSort(extra, tail); // Use serial quick sort.
        int* tmp = malloc(sizeof(int) * subsize * world);
        parallelMerge(subarray, subsize, rank, world, tmp);    
        
        // Store the results in the original array        
        parallelMerge2(tmp, subsize * world, extra, tail, p);         
        free(tmp);
        free(extra);
    }else{
        // The other processes send their results
        parallelMerge(subarray, subsize, rank, world, NULL);
    }
    free(subarray);
}

void parallelMergesort1(int* p, int size) {
    int rank, world;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);    
    parallelMergesort1_(p, size, rank, world);
} 
 
void parallelMergesort2(int* p, int size) {
    int rank, world;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);
    parallelMergesort2_(p, size, rank, world);
}
