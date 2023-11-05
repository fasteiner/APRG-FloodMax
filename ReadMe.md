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

3. Run the bash script by providing the run_type (cluster, local), the number of MPI tasks and the desired diameter as command-line arguments. 

- Default Parameters:
    - run_type = local
    - number of tasks = 5
    - diameter = 10

```sh
./start_algorithms.sh <run_type> <number_of_tasks> <diameter> (replace `<run_type>` `<number_of_tasks>` and `<diameter>` with the actual values)

./start_algorithms.sh local 5 5

./start_algorithms.sh cluster 64 10

```

4. Run Files separately local (does not save the output into the `floodmax_overall_results.txt` file):
```sh
mpirun -np 5 ./out/mpi_floodmax <diameter>
mpirun -np 5 ./out/mpi_floodmax_opt <diameter>
```

5. Run Files separately cluster (does not save the output into the `floodmax_overall_results.txt` file):
```sh
srun -n 5 --mpi=pmi2 ./out/mpi_floodmax <diameter>
srun -n 5 --mpi=pmi2 ./out/mpi_floodmax_opt <diameter>

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


## Comparison of results

These results compare the performance of the "Normal Floodmax" and "Optimized Floodmax" algorithm with varying numbers of processes and network diameters (Results are in `floodmax_overall_results.txt` file). 
The key observations from these comparisons are:

- For a fixed network diameter:
	- "Optimized Floodmax" consistently outperforms "Normal Floodmax" in terms of the total number of messages sent.
	- The difference in message count between the two algorithms becomes more significant as the number of processes increases.

- For a fixed number of processes:
	- "Optimized Floodmax" substantially reduces the total message count, resulting in more efficient communication, especially in larger networks.

These results demonstrate the advantage of using the "Optimized Floodmax" algorithm, especially when dealing with a large number of processes and a substantial network diameter. The optimization leads to significant message reduction, which can be beneficial in distributed systems with communication constraints.


## Authors

- Elias Marcon MSc. | [@eliasmarcon](https://github.com/eliasmarcon)
- Ing. Fabian Steiner BSc.| [@fasteiner](https://github.com/fasteiner/)
- Jan Langela Regincos BSc. | [@janpilu](https://github.com/janpilu)
