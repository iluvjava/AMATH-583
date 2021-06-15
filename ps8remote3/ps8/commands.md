**Load Modules from Hyak**

> `module load gcc/10.2.0`

> `module load cuda/11.2.2`

> `module load intel/oneAPI`

> `module load intel/oneAPI && module load cuda/11.2.2 && module load gcc/10.2.0`



---
**Sync to/from Hyak:** 

> `rsync -avuzb ps8 hongdali@klone.hyak.uw.edu:` Sync to

> `rsync -avuzb hongdali@klone.hyak.uw.edu:ps8 ./` Sync from remote, making a new empty directory for it will be better. 

---
**Get Host name:**  

> `srun hostname`

---
**Srun Multiple Processes:**

> `srun -A niac --tasks=8 hostname`

> `srun --nodes=8 --tasks=2 hostname`

> `srun --nodes=4 --tasks=2 --cpus-per-task=8 ./ompi_info.exe`

> `srun --mpi=pmi2 --nodes=4 --tasks=2 hello.exe`

---
**Sbatch**

> `sbatch hello_script.bash`

> `sbatch --nodes=4 hello_script.bash`

This won't run jobs in parallel like what `srun` did. the `--nodes=4` denotes the amount of resources to be used and has nothing to do with the number of processes instances to run. 

Inside the batch script we can run MPI, or srun on a certain program, and that will get executed with multiple instance. 

---
**Inspect Submitted Sbatch Jobs**

> `squeue --job 147081`

**Cancel A Job**

> `scancel 147081`


---
**Warning**

* never run `mpirun` on one node without allocating resouces (Don't invoke it manually in the terminal). Always run it under the `sbatch` command, with a batch script that invoke the MPI run. 


---
**Concepts**

### **MPI is Just An Environment for Processes Communication**

In one of the scripts above, we used mpirun to launch a program that didn’t have any MPI function calls in it. Again, in some sense, there is no such thing as “an MPI program.” There are only processes, some of which may use MPI library functions to communicate with each other. But mpirun does not in any way inspect what the processes actually do, so it will launch programs without MPI just as well as programs that do call MPI.

What it does do is set up a communication runtime system that enables programs that do make MPI function calls to be able to execute those function calls.

For compiling we have: `mpixx` as the wrapper on GCC

For running we have `mpirun` exectues the program with multiple process instances. 