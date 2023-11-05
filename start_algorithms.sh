#!/bin/bash

# Check for the number of tasks and diameter arguments
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <number_of_tasks> <diameter>"
    exit 1
fi

# Get the number of tasks and diameter from the command line
num_tasks=$1
diameter=$2

# Output file to save results
output_file="./floodmax_overall_results.txt"


# Run the MPI program with srun, passing the diameter as an argument
echo "Running normal Floodmax with $num_tasks MPI tasks and diameter $diameter..."
srun -n $num_tasks --mpi=pmi2 ./out/mpi_floodmax $diameter >> $output_file
echo "Completed."


# Run the MPI program with srun, passing the diameter as an argument
echo "Running optimized Floodmax with $num_tasks tasks and diameter $diameter..."
srun -n $num_tasks --mpi=pmi2 ./out/mpi_floodmax_opt $diameter >> $output_file
echo "Completed."
