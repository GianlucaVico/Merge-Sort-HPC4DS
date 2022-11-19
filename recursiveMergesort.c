#include <stdlib.h>
#include "recursiveMergesort.h"
/*
Merge 2 sorted subarray in a sorted way

Args:
    int *p: input array containing the 2 sub / output the sorted array
    int start: starting index of first subarray
    int mid: starting index of the second subarray
    int end: ending position of the second subarray (not included in the array)
*/
void merge2(int *p, int first, int second, int end) {
    int* tmp = malloc(sizeof(int) * (second - first));
    int i, j, k;  // Iterators for the subarrays and the array
    int l1 = second - first; // Lengths of the arrays
    
    // Move the first subarray
    for(i = 0; i < l1; i++) {
        tmp[i] = p[first + i];
    }

    // Store the result in p
    i = 0;
    j = second;
    k = first;
    while((i < l1) && (j < end)) {
        if(tmp[i] < p[j]) {
            p[k] = tmp[i];
            i++;

        } else {
            p[k] = p[j];
            j++;
        }
        k++;
    }

    while(i < l1) {
        p[k] = tmp[i];
        i++;
        k++;
    }

    // Probably not needed: the second subarray is already sorted and in p
    // while(j < end) {
    //     p[k] = p[j];
    //     j++;
    //     k++;
    // }
    free(tmp);
};

/*
Serial recursive implementation of Merge Sort

Args:
    int *p: input array to be sorted / output sorted array
    int start: starting index of the array to sort
    int end: last index of the array to sort
*/
void recursiveMergesort_(int *p, int start, int end) {   
    if((end - start) <= 1) {  // TODO: use diff. sort algo if end - start < threshold
        return;
    } else {
        int mid = (start + end) / 2;
        recursiveMergesort_(p, start, mid);
        recursiveMergesort_(p, mid, end);
        merge2(p, start, mid, end);
    }
};

/*
Serial recursive implementation of Merge Sort

Args:
    int *p: input array to be sorted / output sorted array
    int len: length of the array
*/
void recursiveMergesort(int *p, int len) {
    recursiveMergesort_(p, 0, len);
}
