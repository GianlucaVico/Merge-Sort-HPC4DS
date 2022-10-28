# Merge-Sort-HPC4DS

## Approach

* Compare different array sizes
* Compare different number of MPI processes (including 1/serial)
* Compare with other implementations (e.g. [this](https://github.com/pouyarz/Parallel--Merge-Sort-Using-MPI) and [this](https://github.com/GlenGGG/FastParallelMergeSort))
* Use other sorting algorithms for small arrays (e.g. bubble sort if the length of the array is <20)
* Try different merging strategies
* Compare MPI C implementation vs MPI Python

## Algorithm ideas

### Splitting

* Use MPI_Scatter

### Sorting

* Each process sort its subarray

### Merging

* Approach 1: all the processes send the sorted array to process 0 which will do the merge
* Approach 2: parallel merge in a tree-like structure (similar to MPI_Reduce), like in this schema
 
 ```
 depth 0: 0
          | \_____
 depth 1: 0       4
          | \     | \
 depth 2: 0   2   4   6
          |\  |\  |\  |\
 depth 3: 0 1 2 3 4 5 6 7
 
 Send to rank - 2^d if rank % 2^{d+1} != 0
 Recv from rank + 2^d if rank % 2^{d+1} == 0 and the sender exists
 ```
 
 
