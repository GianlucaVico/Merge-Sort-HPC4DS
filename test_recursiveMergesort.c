#include <stdio.h>
#include "recursiveMergesort.h"
#include "utils.h"

void testMerge(int *p, int len) {
    printf("-----------------\n");
    printf("Array: ");
    printIntArray(p, len);
    merge2(p, 0, len/2, len);
    printf("Merged: ");
    printIntArray(p, len);
    printf("-----------------\n");
}

void testSort(int *p, int len) {
    printf("-----------------\n");
    printf("Array: ");
    printIntArray(p, len);
    recursiveMergesort(p, 0, len);
    printf("Sorted: ");
    printIntArray(p, len);
    printf("-----------------\n");
}

int main(int argc, char const *argv[])
{   
    const int L = 10;
    printf("#############\n");
    printf("Merge part\n");
    int m1[10] = {1,2,3,4,5,6,7,8,9,10};
    int m2[10] = {6,7,8,9,10,1,2,3,4,5};
    int m3[10] = {1,3,5,7,9,2,4,6,8,10};
    int m4[10] = {1,3,5,7,9,2,4,6,8};
    
    testMerge(m1, L);
    testMerge(m2, L);
    testMerge(m3, L);
    testMerge(m4, L-1);

    printf("#############\n");
    printf("Sort part\n");
    int s1[10] = {10,9,8,7,6,5,4,3,2,1};
    int s2[10] = {10,4,3,7,6,1,2,8,5,9};
    int s3[10] = {4,2,2,4};
    testSort(m1, L);
    testSort(m4, L - 1);
    testSort(s1, L);
    testSort(s2, L);
    testSort(s3, 4);

    return 0;
}
