#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

void printIntArray(int *p, int len) {
    int i;
    for(i = 0; i < len; i++) {
        printf("%d ", p[i]);
    }
    printf("\n");
};

/*
Generate a random integer array of the given size

Args:
    int* p: output array
    int len: size of the array
    int min: minimum value in the array (inclusive)
    int max: maximum value in the array (inclusive)
*/
void generatate_random(int** p, int len, int min, int max) {
    int i;
    int range = max - min;
    for(i = 0; i < len; i++) {
        *p[i] = rand() % range + min;
    }
};

/*
Args;
    int* p: input array
    int len: length of the array
Return:
    index with the minimum index
*/
int argmin(int* p, int len) {
    int min = p[0];
    int arg = 0;
    int i;
    for(i = 1; i < len; i++) {
        if(p[i] < min) {
            min = p[i];
            arg = i;
        }
    }
    return arg;
}
