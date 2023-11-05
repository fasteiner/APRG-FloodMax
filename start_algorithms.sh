#!/bin/bash

# Check for the number of tasks and diameter arguments
if [ "$#" -gt 3 ]; then
    echo "Usage: $0 [<runtype>] [<number_of_tasks>] [<diameter>]"
    exit 1
fi

# Set default values if arguments are not provided
run_type=${1:-"local"}
num_tasks=${2:-5}
diameter=${3:-10}

# Output file to save results
output_file="./floodmax_overall_results.txt"

if [ "$run_type" == "cluster" ]; then
    # Running on the Slurm cluster
    echo "Running on the Slurm cluster..."
    echo "Running normal and optimized Floodmax with $num_tasks MPI tasks and diameter $diameter..."
    srun -n $num_tasks --mpi=pmi2 ./out/mpi_floodmax $diameter >> $output_file
    srun -n $num_tasks --mpi=pmi2 ./out/mpi_floodmax_opt $diameter >> $output_file
elif [ "$run_type" == "local" ]; then
    # Running locally
    echo "Running locally..."
    echo "Running normal and optimized Floodmax with $num_tasks MPI tasks and diameter $diameter..."
    mpirun -np $num_tasks ./out/mpi_floodmax $diameter >> $output_file
    mpirun -np $num_tasks ./out/mpi_floodmax_opt $diameter >> $output_file
else
    echo "Invalid run type. Use 'local' or 'cluster'."
    exit 1
fi

echo "Completed."