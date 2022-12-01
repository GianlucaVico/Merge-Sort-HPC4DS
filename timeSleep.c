
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(int argc, char const *argv[])
{
    MPI_Init(NULL, NULL);
    int size, rank, length;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int root = 0;
    double currentTime;
    int msgSize = 1;
    int nameLength;
    char processorName[MPI_MAX_PROCESSOR_NAME];
    MPI_Get_processor_name(processorName, &nameLength);
    srand((unsigned)time(NULL)+rank*size);
    if(rank == 0)
    {
        double maxTimeDifference = 0;
        for(int i = 0; i < size-1; i++)
        {
            MPI_Recv(&currentTime, msgSize, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if(currentTime > maxTimeDifference)
            {
                maxTimeDifference = currentTime;
            }
        }
        printf("The maximum time difference was %f nanoseconds.\n", maxTimeDifference);
    }
    else
    {
        int nanosecondMultiplier;
        int minNanoseconds = 0;
        int maxNanoseconds = 999999999;
        int rangeNanoseconds = maxNanoseconds-minNanoseconds;
        int nanoseconds = rand()%rangeNanoseconds+minNanoseconds;
        double nanoMultiplier = 1000000000;
        struct timespec t1, t2;
        t1.tv_sec = 0;
        t1.tv_nsec = nanoseconds;
        double startTime = MPI_Wtime();
        nanosleep(&t1, &t2);
        double endTime = MPI_Wtime();
        currentTime = abs((double)nanoseconds*nanoMultiplier-(endTime-startTime));
        int tag = 0;
        MPI_Send(&currentTime, msgSize, MPI_DOUBLE, root, tag, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
