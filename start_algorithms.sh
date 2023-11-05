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

if [ -n "$SLURM_JOB_ID" ]; then
    # Running on the Slurm cluster
    echo "Running on the Slurm cluster..."
    echo "Running normal and optimized Floodmax with $num_tasks MPI tasks and diameter $diameter..."
    srun -n $num_tasks --mpi=pmi2 ./out/mpi_floodmax $diameter >> $output_file
    srun -n $num_tasks --mpi=pmi2 ./out/mpi_floodmax_opt $diameter >> $output_file
else
    # Running locally
    echo "Running locally..."
    echo "Running normal and optimized Floodmax with $num_tasks MPI tasks and diameter $diameter..."
    mpirun -n $num_tasks ./out/mpi_floodmax $diameter >> $output_file
    mpirun -n $num_tasks ./out/mpi_floodmax_opt $diameter >> $output_file
fi

echo "Completed."