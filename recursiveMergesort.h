#ifndef RECURSIVE_H
#define RECURSIVE_H

int THRESHOLD;
void merge2(int *p, int first, int second, int end);
void recursiveMergesort_(int *p, int start, int end);
void recursiveMergesort(int *p, int len);
void quickSort_(int *p, int start, int len);
void quickSort(int *p, int len);

#endif
