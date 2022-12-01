#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#include "recursiveMergesort.h"

int THRESHOLD = 20;
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
int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}
void recursiveMergesort_(int *p, int start, int end) {   
    if((end - start) <= 1) {  // TODO: use diff. sort algo if end - start < threshold
        return;
    } else {     
            if((end - start) < THRESHOLD) {
                qsort(p+start, end - start, sizeof(int), compare);
            }else {
                int mid = (start + end) / 2;
                recursiveMergesort_(p, start, mid);
                recursiveMergesort_(p, mid, end);
                merge2(p, start, mid, end);
            }
    }
};

/*
Serial implementation of Quick Sort

Args:
    int *p: input array to be sorted / output sorted array
    int len: length of the p
*/
void quickSort_(int *p, int start, int len) 
{

    //printf("length %d\n", len);
    //for(int i = start; i < start+len; i++)
    //{
    //    printf("%d ", p[i]);
    //}
    //printf("\n");

    int left_index = start;
    int right_index = (left_index+len)-1;
    // A base case. 
    if(len == 2)
    {
        if(p[left_index] > p[right_index])
        {
            int temp = p[left_index];
            p[left_index] = p[right_index];
            p[right_index] = temp;
        }
    }
    // General case.  No sorting is necissary for size 1 arrays.
    else if (len > 1)
    {
        //srand(time(0)); // Use the current time as a seed to generate a random number.
        int pivot_index = (rand()%(len))+left_index; // Choose the pivot index randomly.    
        int pivot_value = p[pivot_index];
        int left_value = p[left_index];
        int right_value = p[right_index];
        while(left_index < right_index)
        {
            // Find the leftmost value greater than the pivot.
            while(left_value <= pivot_value)
            {
                // If no swaps are made, break the loops. Otherwise increment.
                if(left_index >= right_index)
                {
                    goto PASS;
                }
                    left_index++;
                    left_value = p[left_index];
                }
            // Find the rightmost value less than the pivot.
            while(right_value >= pivot_value)
            {
                // If no swaps are made, break the loops. Otherwise increment.
                if(right_index <= left_index)
                {
                    goto PASS;
                }
                    right_index--;
                    right_value = p[right_index];
                }
            // Swap the elements and continue.
                p[left_index] = right_value;
                p[right_index] = left_value;
                left_index++;
                left_value = p[left_index];
                right_index--;
                right_value = p[right_index];
            // If all swaps have been made, break the loops.
            if(left_index >= right_index)
            {
                goto PASS;
            }
        }        
        PASS:;
        // Find the index to be swapped with the pivot based on index proximity and value comparison.
        int swap_index;
        if(left_index < pivot_index && left_value < pivot_value)
        {
            swap_index = left_index+1;
        }
        else if(left_index > pivot_index && left_value > pivot_value)
        {
            swap_index = left_index-1;
        }
        else
        {
            swap_index = pivot_index;
        }
        // Swap the pivot element with the element at left and right index's stopping place.
        p[pivot_index] = p[swap_index];
        p[swap_index] = pivot_value;
        pivot_index = swap_index;
        // Recursively sort the left and right partitions.
        quickSort_(p, start, pivot_index-start);
        quickSort_(p, pivot_index+1, (len-(pivot_index-start))-1); 
    }
}

/*
Serial recursive implementation of Merge Sort
Args:
    int *p: input array to be sorted / output sorted array
    int len: length of the array
*/
void recursiveMergesort(int *p, int len) {    
    recursiveMergesort_(p, 0, len);
}

/*
Serial recursive implementation of Quick Sort
Args:
    int *p: input array to be sorted / output sorted array
    int len: length of the array
*/
void quickSort(int *p, int len) {
    quickSort_(p, 0, len);
}
