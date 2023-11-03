#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <time.h>

int electLeader(int rank, int size, int diameter, int *leader)
{
    // send uid to all neighbours
    int data;
    int calls = 0;
    for (int round = 0; round < diameter; round++)
    {
        data = *leader;
        for (int i = 0; i < size; i++)
        {
            if (i != rank)
            {
                // i == neighbour rank
                calls++;
                MPI_Send(&data, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }
        // receive from all neighbours
        for (int i = 0; i < size; i++)
        {
            if (i != rank)
            {
                // i == neighbour rank
                MPI_Recv(&data, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (data > *leader)
                {
                    *leader = data;
                }
            }
        }
    }
    return calls;
}

int main(argc, argv)
int argc;
char **argv;
{
    int rank, size, diameter;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // diameter = atoi(argv[1]);
    diameter = size;

    // code here
    int leader = rank;
    clock_t time = clock();
    int count = electLeader(rank, size, diameter, &leader);
    if (rank != leader)
    {
        MPI_Send(&count, 1, MPI_INT, leader, 0, MPI_COMM_WORLD);
    }
    time = clock() - time;
    // get diameter

    // loop i< diameter
    //      1. send to all
    //      2. receive from all
    //      3. compare and update

    // print max
    if (rank == leader)
    {
        int sumCount = count;
        for (int i = 0; i < size; i++)
        {
            int subCount;
            if (i != leader)
            {

                MPI_Recv(&subCount, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                sumCount += subCount;
            }
        }
        printf("Leader is %d\n", leader);
        printf("Time taken: %f\n", ((double)time) / CLOCKS_PER_SEC);
        printf("Total calls: %d\n", sumCount);
    }
    MPI_Finalize();
    return 0;
}