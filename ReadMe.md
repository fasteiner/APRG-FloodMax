# Flood Max

The goal of this project was to develop the floodmax algorithm using mpi and running it on a slearn cluster, as well as optimizing the algorithm to reduce the number of messages sent. The final step was to compare the performance of the two algorithms.

## Authors

- Elias Marcon MSc. | @eliasmarcon
- Ing. Fabian Steiner BSc.| @fasteiner
- Jan Langela Regincos | @janpilu

## How to build

### Prerequisites

- Ubuntu 20.04
- mpich

### Build

Generate all binaries

```sh
make all
```

### Usage

```sh
./mpi_floodmax <diameter>
./mpi_floodmax_opt <diameter>
```

diameter: the diameter of the network (optional, default 4)

Run on the cluster:

```sh
# Not optimized with diameter 4
srun -n 64 --mpi=pmi2 ./mpi_floodmax 4
# Optimized with diameter 4
srun -n 64 --mpi=pmi2 ./mpi_floodmax_opt 4
```

## Normal Algorithm

The floodmax algorithm is used to elect a leader in a network. The algorithm works as follows:

1. The diameter of the network is known
2. For diameter count:
    1. Each node sends a message to all its neighbors with its id, or the id of the current highest known id
    2. Each node receives messages from its neighbors
3. In the end, the leader is the node with the highest id

Unfortunately, this algorithm is not very efficient. The amount of messages sent is $O(diameter*n*e)$ where n is the amount of nodes in the network.

## Optimized Algorithm

In the optimized version of the floodmax algorithm, we aimed to reduce the amount of total messages sent.
We did this by adding checks before sending messages. The checks are as follows:

- Dont send a message to the node that currently has the highest id
- Dont send a message to a node that has already notified me about my current highest rank
- Only send new messages when a higher id has been received

An issue we ran into with this implementation was that the `MPI_Recv` function is blocking and we don't have the guarantee that all neighbors will send a message. To work around this issue we used the `MPI_Irecv` function which is non-blocking. To ensure that all messages are received we added a timeout before checking if all messages have been received with `MPI_Test`.
