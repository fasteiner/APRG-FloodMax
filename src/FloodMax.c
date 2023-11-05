#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <time.h>

int electLeader(int rank, int size, int diameter, int *leader)
{
    // send uid to all neighbours
    int calls = 0;
    for (int round = 0; round < diameter; round++)
    {
        for (int i = 0; i < size; i++)
        {
            if (i != rank)
            {
                // i == neighbour rank
                calls++;
                MPI_Send(leader, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }
        // receive from all neighbours
        for (int i = 0; i < size; i++)
        {
            if (i != rank)
            {
                // i == neighbour rank
                int data;
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

    if (argc > 1)
    {
        // Check if a command-line argument for diameter is provided
        diameter = atoi(argv[1]);
    }
    else
    {
        // If no argument is provided, set a default diameter
        diameter = 10; // Change to your desired value
    }

    int leader = rank;
    clock_t time = clock();
    int count = electLeader(rank, size, diameter, &leader);
    time = clock() - time;
    if (rank != leader)
    {
        MPI_Send(&count, 1, MPI_INT, leader, 0, MPI_COMM_WORLD);
    }

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
        printf("================== Run Start ===========================\n");
        printf("=============== Normal Floodmax ========================\n");
        printf("Number of processes: %d\n", size);
        printf("Network Diameter: %d\n", diameter);
        printf("Leader is %d\n", leader);
        printf("Time taken: %f\n", ((double)time) / CLOCKS_PER_SEC);
        printf("Total Messages send: %d\n", sumCount);
        printf("==================  Run End  ===========================\n\n");
    }
    MPI_Finalize();
    return 0;
}