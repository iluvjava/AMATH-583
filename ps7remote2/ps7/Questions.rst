Questions
=========


hello_omp
---------

* How many omp threads are reported as being available?  Try increasing the number of cpus-per-task.  Do you always get a corresponding number of omp threads?  Is there a limit to how many omp threads you can request?


* What is the reported hardware concurrency and available omp threads if you execute ompi_info.exe on the login node?


norm
----

* What are the max Gflop/s reported when you run norm_parfor.exe with 8 cores?  How much speedup is that over 1 core?
How does that compare to what you had achieved with your laptop?



matvec
------

* What are the max Gflop/s reported when you run pmatvec.exe with 16 cores?  How does that compare to what you had achieved with your laptop?



pagerank
--------

* How much speedup (ratio of elapsed time for pagerank) do you get when running on 8 cores?



cu_axpy
-------

* How many more threads are run in version 2 compared to version 1? How
much speedup might you expect as a result? How much speedup do you see in your plot?



* How many more threads are run in version 3 compared to version 2? How
much speedup might you expect as a result? How much speedup do you see in your plot?
(Hint: Is the speedup a function of the number of threads launched or the number of available cores, or both?)



* (AMATH 583)
The cu_axpy_t also accepts as a second command line argument the size of the blocks to be used.
Experiment with different block sizes with, a few different problem sizes (around :math:`2^{24}` plus or minus).  What block size seems to give the best performance?  



nvprof
------

* (AMATH 583) Looking at some of the metrics reported by nvprof, how do metrics such as occupancy and efficiency compare to the ratio of threads launched between versions 1, 2, and 3?



Striding
--------

* Think about how we do strided partitioning for task-based parallelism (e.g., OpenMP or C++ tasks) with strided partitioning for GPU.  Why is it bad in the former case but good (if it is) in the latter case?



norm_cuda
---------

* What is the max number of Gflop/s that you were able to achieve from the GPU?  Overall (GPU vs CPU)?

