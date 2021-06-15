
Final Questions
===============

Eigenfaces
----------

* Given that forming the covariance matrix with 500 images takes approximately 10
seconds, how long (approximately) would forming the covariance matrix take if we used
all 262,781 images in the data file?

Given that the time grows linear wrt to the number of pictures, this implies that 262,781 images will take 525 times more, which is about 87.5 minutes. Which is enough to watch a movie, and this is assuming we don't count in the time for reading the images. 


Sequential Optimization
-----------------------

* What optimizations did you apply to improve eigenfaces_opt?
Explain why these optimizations would result in better performance.

Implemented Techniques: 
1. Outter loops goes over row, inner loops goes over columns. 
2. `x(II)` is hoist out of the for loop. 
3. Unrolling Loop. 

Explaination: 
1. Increase data locality and reduce interruption of cache line when reading the elements from the vector. Elements from the vector are read with unit stride from the memory, reducing cache misses. 
2. Hoisting constant variable in the forloop helps the compiler optimize for SIMD. 
3. No actual improvement is made from loop unrolling, a 30 percent decrease in Gflops/s is observed and hence it's not implemented. 

Note: 
1. No implementation for `gen_covariance` method, nothing useful is found for optimization. 


Parallelization
---------------

* How did you parallelize your code?
On 5000 faces, 
how much speedup were you able to get on 2, 4, 10, and 20 cores (cpus-per-task)?
Does the speedup improve if you load 15000 faces rather than 5000 faces?

1. I applied OMP on `gen_covariance`, each thread work on a substet of images. 
2. For 2, 4, 10, 20, the speedup is about: 2, 2, 2, 2. There is no speedup as more threads from OMP are used. Max Gflops is about 3.7. 
3. The performance didn't increase for 1500 images. 

Note: 
1. The Gflops results is really fickle, I get 6, 3, 9, sporadically, I am not sure what is happened. 

Blocking (AMATH 583)
--------------------

* Explain your blocking approach.
On 5000 faces and 10 (or 20) cores (say), 
how much speedup were you able to get over the non-blocked case?
How much speedup were you able to get over the non-blocked case for 50000 face?

* I partitioned the outter product such that each thread work on different blocks of rows for the matrix, for the same image. This is done via the OMP macros for parallel forloop. 
* Another implementation where used. Where the forloop dynamically schedule the block for each image. However, this implementation only computes the lower triangular. And then after the computation, the lower triangular matrix is copied to the upper triangular part. 

1. 10 cores, 5000 faces, block gives 8 Gflops/s, compare to Par, which was 9.6 Gflops/s for the same setup, there is a speeddown. 20 cores, with 5000 faces, block gives 29 Gflops/s, compare to 9.1 Gflops given by par, the speed up is 3. For the second implementation, the speed up for 10 cores for 5000 is 6 times, 57 Gflops/s, and for 20 cores, it's 90 Gflop/s, and the speed up is 10 times. That is comparing to 10 and 20 cores of Par. 

2. For 50,000 faces, block has the same Gflops/s for different n, and par implemented failed to complete within time limit of one minute. 

DGEMM (Extra Credit)
--------------------

* What single core performance do you get with eigenfaces_dgemm?  
How does your performance scale for 2, 5, 10, 20 cores?  
How does your performance scale with number of faces (500, 5k, 50k)? 
(You may even try 40 cores and 50k or more faces.)



MPI
---

* How does the performance of your eigenfaces_mpi.exe scale for 1, 2, 4, 8 nodes?  (With 10 cores.)




Max
---

* What configuration of nodes and cores per node gives you the very best performance?
(And what was your best performance?)

