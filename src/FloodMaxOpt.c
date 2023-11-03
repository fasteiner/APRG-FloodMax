#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "mpi.h"

void electLeader(int rank, int size, int diameter, int *leader)
{
    int i;
    // send uid to all neighbours
    int data;
    bool changed = false;
    int updatedBy = -1;
    for (int round = 0; round <= diameter; round++)
    {

        data = *leader;
        for (int i = 0; i < size; i++)
        {
            if ((i != rank && i != *leader && i != updatedBy) && changed)
            {
                // i == neighbour rank
                MPI_Send(&data, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }
        changed = false;
        updatedBy = -1;
        // receive from all neighbours
        for (int i = 0; i < size; i++)
        {
            if (i != rank)
            {
                // i == neighbour rank
                MPI_Recv(&data, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (data > *leader)
                {
                    changed = true;
                    *leader = data;
                    updatedBy = i;
                }
            }
        }
    }
}

int main(argc, argv)
int argc;
char **argv;
{
    int rank, size, diameter;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    diameter = atoi(argv[0]);

    // code here
    int leader = rank;
    electLeader(rank, size, diameter, &leader);
    // get diameter

    // loop i< diameter
    //      1. send to all
    //      2. receive from all
    //      3. compare and update

    // print max
    if (rank == leader)
    {
        printf("Leader is %d\n", leader);
    }
    MPI_Finalize();
    return 0;
}