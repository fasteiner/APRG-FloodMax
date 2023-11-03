#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

#define TIMEOUT_MS 100 // Timeout in milliseconds

bool check_timeout(clock_t start)
{
    clock_t current_time = clock();
    double elapsed_time_ms = 1000.0 * (current_time - start) / CLOCKS_PER_SEC;
    return elapsed_time_ms >= TIMEOUT_MS;
}

int electLeader(int rank, int size, int diameter, int *leader)
{
    int calls = 0;
    // send uid to all neighbours
    int data;
    bool changed = true;
    int updatedBy = -1;
    for (int round = 0; round <= diameter; round++)
    {

        data = *leader;
        for (int i = 0; i < size; i++)
        {
            if ((i != rank && i != *leader && i != updatedBy) && changed)
            {
                // i == neighbour rank
                calls++;
                MPI_Send(&data, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }

        changed = false;
        updatedBy = -1;

        // receive from all neighbours with timeout
        MPI_Request requests[size];
        MPI_Status statuses[size];
        int data[size];
        bool active_requests[size];
        int num_active_requests = 0;

        for (int i = 0; i < size; i++)
        {
            // For each node set active request to false
            active_requests[i] = false;
            if (i != rank)
            {
                // Send a nonblocking request to every node except current rank (self)
                MPI_Irecv(&data[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &requests[i]);
                // Set Active request
                active_requests[i] = true;
                num_active_requests++;
            }
        }
        // start timer
        clock_t start_time = clock();
        // check_timeout(start_time) function checks if a certain amount of time has passed since start_time. The num_active_requests variable keeps track of how many requests are still active.
        while (num_active_requests > 0 && !check_timeout(start_time))
        {
            for (int i = 0; i < size; i++)
            {
                if (active_requests[i])
                {
                    int flag;
                    // MPI_Test is a non-blocking function that checks if a specific MPI request has completed
                    MPI_Test(&requests[i], &flag, &statuses[i]);
                    if (flag)
                    {
                        if (data[i] > *leader)
                        {
                            changed = true;
                            *leader = data[i];
                            updatedBy = i;
                        }
                        active_requests[i] = false;
                        num_active_requests--;
                    }
                }
            }
        }

        // Cancel any remaining active requests
        for (int i = 0; i < size; i++)
        {
            if (active_requests[i])
            {
                MPI_Cancel(&requests[i]);
                MPI_Request_free(&requests[i]);
            }
        }
    }
    return calls;
}

int main(int argc, char **argv)
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
        printf("Leader is %d\n", leader);
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
        /*for (int i = 0; i < size; i++)
        {
            int subCount;
            if (i != leader)
            {

                MPI_Recv(&subCount, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                sumCount += subCount;
            }
        }*/
        printf("Leader is %d\n", leader);
        printf("Time taken: %f\n", ((double)time) / CLOCKS_PER_SEC);
        printf("Total calls: %d\n", sumCount);
    }
    MPI_Finalize();
    return 0;
}