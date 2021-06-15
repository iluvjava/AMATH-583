#!/bin/bash

#SBATCH --job-name=mpi-grid		# Job name
#SBATCH --time=00:05:00   		# Time limit hrs:min:sec
#SBATCH --ntasks-per-node=2		# How many tasks on each node
#SBATCH --cpus-per-task=1		# Number of cores per MPI rank
#SBATCH --distribution=cyclic:cyclic 	# Distribute tasks cyclically on nodes and sockets 

export PATH=/efs/home/al75/tau2/x86_64/bin:$PATH

echo "Starting parallel grid"

mpirun tau_exec -T MPI ./grid.exe -p ${*}

echo "Done with parallel grid"
