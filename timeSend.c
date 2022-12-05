
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

int main(int argc, char const *argv[])
{
    MPI_Init(NULL, NULL);
    int size, rank, length;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int root = 0;
    long double datum;
    double sendTime;
    int msgSize = 1;
    int dataTag = 0;
    int timeTag = 1;
    if(rank == 0)
    {
        int dataTag = 0;
        for(int i = 0; i < size-1; i++)
        {
            MPI_Recv(&datum, msgSize, MPI_LONG_DOUBLE, MPI_ANY_SOURCE, dataTag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        double maxTime = 0;
        double minTime = INT_MAX;
        for(int i = 0; i < size-1; i++)
        {
            MPI_Recv(&sendTime, msgSize, MPI_DOUBLE, MPI_ANY_SOURCE, timeTag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if(sendTime > maxTime)
            {
                maxTime = sendTime;
            }
            if(sendTime < minTime)
            {
                minTime = sendTime;
            }
        }
        double maxTimeDifference = maxTime-minTime;
        printf("The biggest time difference was %f nanoseconds.\n", maxTimeDifference);
    }
    else
    {
        double nanoMultiplier = 1000000000;
        double startTime = MPI_Wtime();
        long double datum = 0;
        MPI_Send(&datum, msgSize, MPI_LONG_DOUBLE, root, dataTag, MPI_COMM_WORLD);
        double endTime = MPI_Wtime();
        sendTime = (endTime-startTime)*nanoMultiplier;
        MPI_Send(&sendTime, msgSize, MPI_DOUBLE, root, timeTag, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
