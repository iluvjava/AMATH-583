#!/bin/bash

#SBATCH --job-name=pingpong
#SBATCH --account=niac
#SBATCH --partition=compute
#SBATCH --time=00:00:15   		# Time limit hrs:min:sec

mpirun ./pingpong.exe ${*}
