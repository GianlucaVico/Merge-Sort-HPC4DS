#include <mpi.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

// #PBS -l select=<n_1>:ncpus=<n_2>:mem=<n_3>gb -l place=pack:excl

int main(int argc, char const *argv[])
{
    int length = 25;
    int max = 20;
    int min = -20;
    int range = max-min;
    int *unsorted = malloc(sizeof(int)*(length));
    srand(time(0));
    for(int i = 0; i < length; i++)
    {
        unsorted[i] = rand()%range+min;
    }
    MPI_Init(NULL, NULL);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(size < length+1)
    {
        if(rank == 0)
        {
            printf("The number of processes must must be greater than the length of the list to be sorted.\n");
        }
    }
    else
    {
        double startTimeSort = MPI_Wtime();
        long double shift = 0;
        long double reduction_factor = 5;
        long double nano_multiplier = 1000000000;
        for(int i = 0; i < length; i++)
        {
            if(unsorted[i] < shift)
            {
                shift = (long double)unsorted[i];
            }
        }
        long double *unsortedProcessed = malloc(sizeof(long double)*(length+1));
        for(int i = 1; i < length+1; i++)
        {
            unsortedProcessed[i] = (long double)((unsorted[i-1]-shift)/reduction_factor);
        }
        free(unsorted);
        int root = 0;
        long double element;
        int msg_size = 1;
        MPI_Scatter(unsortedProcessed, msg_size, MPI_LONG_DOUBLE, &element, msg_size, MPI_LONG_DOUBLE, root, MPI_COMM_WORLD);
        free(unsortedProcessed);
        if(rank == root)
        {
            int *sorted = malloc(sizeof(int)*length);            
            for(int i = 0; i < length; i++)
            {
                MPI_Recv(&element, msg_size, MPI_LONG_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                int processedElement = round((element*reduction_factor)+shift);
                sorted[i] = processedElement;
            }
            double endTimeSort = MPI_Wtime();
            double timeSort = endTimeSort-startTimeSort;
            for(int i = 0; i < length; i++)
            {
                printf("%d, ", sorted[i]);
            }
            printf("\nSorting was completed in %f seconds.\n", timeSort);
            bool ordered = true;
            int i = 1;
            int erronous_index = 0;
            while(i < length && ordered)
            {
                if(sorted[i-1] > sorted[i])
                {
                    ordered = false;
                    erronous_index = i-1;
                }
                i++;
            }
            if(ordered)
            {
                printf("The array was correctly sorted.\n");
            }
            else
            {
                printf("The array was incorrectly sorted. Index %d is erronous.\n", erronous_index);
            }
            free(sorted);
        }
        else if(rank < length+1)
        {
            int seconds = element;
            int nanoseconds = (element-seconds)*nano_multiplier;
            struct timespec t_1, t_2;
            t_1.tv_sec = seconds;
            t_1.tv_nsec = nanoseconds;
            nanosleep(&t_1, &t_2);
            int tag = 0;
            MPI_Send(&element, msg_size, MPI_LONG_DOUBLE, root, tag, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
    return 0;
}
