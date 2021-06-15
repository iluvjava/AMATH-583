#!/bin/bash

#SBATCH --account=niac
#SBATCH --partition=ckpt
#SBATCH --nodes=1
#SBATCH --cpus-per-task=20
#SBATCH --time=00:05:00
#SBATCH --gres=gpu:2080ti

bash script.bash
