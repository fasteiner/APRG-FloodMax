# Flood Max

The goal of this project was to develop the floodmax algorithm using mpi and running it on a slurm cluster, as well as optimizing the algorithm to reduce the number of messages sent. The final step was to compare the performance of the two algorithms and compare them.

## Folder Structure

-  `./src`: 

    - `FloodMax.c`: primary source of the MPI program (contains the source code that implements the FloodMax Leader Election algorithm)

     - `FloodMaxOpt.c`: primary source of the MPI program (contains the source code that implements the optimized FloodMax Leader Election algorithm)

- `./out`: contains the executable files `mpi_floodmax` and `mpi_optimized_floodmax` created through the build process (starts the MPI program in the distributed environment and execute the Leader Election algorithm)

- `Makefile` : configuration file that controls the build process of the MPI program (contains rules and commands to compile the project and create the executable file `mpi_floodmax` and `mpi_optimized_floodmax`)

- `start_algorithms.sh`: automates the execution of two MPI programs (normal and optimized FloodMax) with user-specified number of processes and diameter (saves the results of both runs in an output file for monitoring and analysis)

- `floodmax_overall_results.txt`: contains the results of both the normal and optimied floodmax algorithms


## How to build

### Prerequisites

- Ubuntu 20.04
- mpich

### Build

Generate all binaries into the out folder (`mpi_floodmax` and `mpi_optimized_floodmax`)

```sh
make all
```

### Usage

To use the provided Bash script for running MPI FloodMax algorithms, follow these steps:

1. Ensure that MPI is installed on your system.

2. Open a terminal and navigate to the directory containing this bash script.

3. Run Local Usage (does not save the output into the `floodmax_overall_results.txt` file):

```sh
mpirun -n 64 --mpi=pmi2 ./out/mpi_floodmax <diameter>
mpirun -n 64 --mpi=pmi2 ./out/mpi_floodmax_opt <diameter>
```

    diameter: the diameter of the network (optional, default 10)

4. Run on the cluster:

```sh

# Not optimized with diameter 4
srun -n 64 --mpi=pmi2 ./out/mpi_floodmax 4

# Optimized with diameter 4
srun -n 64 --mpi=pmi2 ./out/mpi_floodmax_opt 4

# Run both algorithms and save the results into the floodmax_overall_results.txt file
./start_algorithms.sh <number_of_processes> <diameter> (replace `<number_of_processes>` and `<diameter>` with the actual values)

```

## Normal Algorithm

The floodmax algorithm is used to elect a leader in a network. The algorithm works as follows:

1. The diameter of the network is known
2. For diameter count:
    1. Each node sends a message to all its neighbors with its id, or the id of the current highest known id
    2. Each node receives messages from its neighbors
3. In the end, the leader is the node with the highest id

Unfortunately, this algorithm is not very efficient. The amount of messages sent is $O(diameter \cdot n \cdot e)$ where n is the amount of nodes in the network.

## Optimized Algorithm

In the optimized version of the floodmax algorithm, we aimed to reduce the amount of total messages sent.
We did this by adding checks before sending messages. The checks are as follows:

- Dont send a message to the node that currently has the highest id
- Dont send a message to a node that has already notified me about my current highest rank
- Only send new messages when a higher id has been received

An issue we ran into with this implementation was that the `MPI_Recv` function is blocking and we don't have the guarantee that all neighbors will send a message. To work around this issue we used the `MPI_Irecv` function which is non-blocking. To ensure that all messages are received we added a timeout before checking if all messages have been received with `MPI_Test`.


## Authors

- Elias Marcon MSc. | [@eliasmarcon](https://github.com/eliasmarcon)
- Ing. Fabian Steiner BSc.| [@fasteiner](https://github.com/fasteiner/)
- Jan Langela Regincos BSc. | [@janpilu](https://github.com/janpilu)
