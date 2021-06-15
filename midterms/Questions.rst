Extra Notes: 

I am attatching the write up for PS4 for midterm, because I used the roofline
graph I obtained there in the midterm, and I will refer to that whenver 
I used that graph. 

================================================================================

My own question: I think that, AOS should be faster than COO, but it turns out 
it's only faster when the matrix is small. What could be the cause of this? 

When the matrix is small, the memory locality of AOS is greater, because 
the indices, and the values are packed together inside a tuple.

The best explanation I can come up for a slower performance on AOS 
matrix vec is because of the extra memory overhead created by the 
tuple data type. Which needs more scrutinization.

Another important thing that might be the cause of it is that the L2 could 
potentially speed things up because it's shared amoung multiple cores, and with
hyperthreading (or whatever the AMD equivalent is called), it speed things up 
for matrix in the size of ``128^2`` and ``256^2``. 

That is my new hypothesis which is not easy to verify. 

================================================================================

1. How does the performance (in GFLOP/s) for sparse-matrix by vector
product compare to what you previously achieved for dense-matrix by
dense-matrix product?  Explain, and quantify if you can, (e.g., using
the roofline model).

The performance is way less compare to results obtained for dense matrix 
multipliction.

The best performance is ``4.44748`` GFLOPs/s, coming from CSR and CSC^T matrix 
vector multiplication. 

Sparse matrix multiplication has memory usage of around O(N*l) where N is the 
size of the row and l is the maximum number of elements in that row. The 
computations needs to compute on all the double, which suggest lower 
memory locality compare to the dense matrix multiplication, decreasing 
the numerical intensity of the sparse matrix vector algorithm.  

The sparse Matrix Vec has one int for indexing, 3 doubles for matrix 
vector multiplication, and 2 floating point operations inside the 
for loop. 

Which is ``2/(24+4) flops/byte``, 1/14. For dense matrix vector algorithm, it's 
``1/4 flops/byte``. I look back to PS4, and it suggests that a perfect use of L1
should get us to ``6 flops/s``. 
And for dense matrix vector it should be capped at ``47.2 Gflops/s`` if L1 is 
used perfectly. 

I make the statement that dense matrix vec is ``1/4 Flops/bytes`` because,
not counting the running index, there are ``(2N^2)`` flops in total and there 
are ``8*(N^2 + N)`` bytes in total. Dividing then gives something slightly less 
than ``1/4``.

================================================================================

2. Referring to the roofline model and the sparse matrix-vector and
dense matrix-matrix algorithms, what performance ratio would you theoretically
expect to see if neither algorithm was able to obtain any reuse from cache?

I took a look at the roofline graph from PS4, if make use of DRAM, then 
``1/14 flops/byte`` should give around ``1.1`` flops/sec. 

The dense matrix vector mulptication is about ``(1/4)*14`` times more intense 
compare to sparse matrix vector multiplication. Which suggest 2 times more 
performance, if no cache reuse.

================================================================================

3. How does the performance (in GFLOP/s) for sparse-matrix by vector
product for COO compare to CSR?  Explain, and quantify if you can,
(e.g., using the roofline model).

COO and CSR asymptoptically approaches the same Gflops as the size ``N`` 
increases. 

COO produces ``2.24873`` flops and CSR produces ``4.44748`` at ``N = 64``. 

CSR is 2 times faster because it has higher numerical intensity. 

It hoisted the ``column_nums_`` out of the fastest running for loop, 
reducing the number of floats double during the computation from 3, to 2. 

Therefore the numerical intensity will be given as: ``2/16``, ``1/8``. 

However it turns out to be an under estimate on the speed improvement
but close nonetheless. 

================================================================================

4. How does the performance (in GFLOP/s) for sparse-matrix by dense
matrix product (**SPMM**) compare to sparse-matrix by vector product
(**SPMV**)? The performance for SPMM should be about the same as for
SPMV in the case of a 1 column dense matrix.  What is the trend with
increasing numbers of columns?  Explain, and quantify if you can,
using the roofline model.

I computed values of ``nrhs`` in ``1, 2, 4... 64``. They are stored in the file
``matmat.log``.

Is the performance for ``nrhs = 1`` the same as `mat_vec`? Absolute, the results
in ``Gflops/s`` is close. 

Fix the size of grid to be 64,
For ``nrhs = 1... 4``, CSR outperforms all other method. After ``nrhs = 4``, AOS
performs the best. 

The trend of increasing ``nrhs`` is increasing ``Gflops/s``. This is expected 
because dense matrix increases the memory locality by increasing the numerical 
intensity of the algorithm. The entry, ``a_{i, j}`` is reused across all the
columns of the dense matrix. 

AOS and COO obtained the best performance because the for loop I wrote take one 
``a`` from ``A``, and then accumates using the ``a`` onto the row of ``C`` 
using the row from ``B``. 

Let's do some math: 

Let ``N`` be the size of the 2d Laplacian Matrix. 

The trend of increasing ``Gflops/s`` is due to reusing the same ``a`` at 
``(i, j)`` of ``A`` to compute the ``i`` row of matirx C, using the ``j`` th 
row of matrix ``B``. This requires ``2*(2*rhs + 1)`` operations for each element
``a`` in ``A``. The number of flops is ``4*rhs + 2``. 

If we only focus on one runs of the inner forloop. In my implementation, it 
takes ``a`` from  ``A`` and then multiply it with all elements in row ``j`` in 
matrix ``B``, and store each one into the ``i`` th row of matrix ``C``. 

This requires: ``2*(nrhs)`` operations. 

This uses ``(1 + 2*nrhs)`` double floating points. Then the numerical intensity
is ``2*(nrhs)/(8*(1 + 2*nrhs))``, a substitution of ``nrhs = 1`` yieds ``1/14``, 
which is consistent with the result for mat vec. 

we can simplify it to ``nrhs/(4 + 8*nrhs)``, let ``nrhs`` to be really big, 
the ratio appraoches ``1/8``. About ``0.125``

And, assuming that everything fits into the L1 perfect, using the roofline model
I had from PS4, it's expected to reach a top speed of about ``10.1`` at top. And
this is indeed the case for AOS at ``nrhs = 32``, where it reached about 
`10.4 Gflops/s`. 

However, this is only applicable for small matrices, because we are only 
computing the numerical intensity in the inner most forloop, to get the full
picture, we need to get the numerical intensity for the whole matrix.

Let ``N`` be the size of the 2D Laplacian Matrix, let ``r`` be the number of 
columns we multiply on the rhs, then the number of entries in matrix ``A``
will be ``5N``. And the number of entries in matrix ``C, D`` will be 
``2rN``. The total number of iterations on these matrices will be: 

``(10Nr)/(8(5N + 2rN))``, simplying we get: ``5r/(20 + 8r)``. This suggests 
a limit of ``5/8`` numerical intensity as ``r`` approaches something huge. 

This suggest an asympototic speed of `6 Gfops/s` on my CPU. Which unfortunately
is not true. A ``r = 65536`` suggests about ``2.5`` Gflops/s. 

Which, in fact, my previous computation will holds true, where I only computed
the numerial intensity on the hottest loop for the matrix matrix multiplication.

If I assume that all the computations are moved to DRAM for ``r = 65536`` 
then I will get around ``2.5`` Gflops/s

================================================================================

5. How does the performance of sparse matrix by dense matrix product (in
GFLOP/s) compare to the results you got dense matrix-matrix product in
previous assignments?  Explain, and quantify if you can, using the
roofline model.

Peak performance is about a third of the Gflops compare to the dense matrix 
multiplication, measured in term of Gflops/s. 

Asymptoptically it's very close to the ``mult_ikj`` method. 

It's the same under the hood compare to the ``mult_ikj``, where we sweep 
through the rows of matrix ``B, C``, to improve the memory locality of the 
hotest running forloop. 

================================================================================

6. The most important thing I learned: 

To use the roofline model, we must make the assumption about where the data is 
stored, so we know which storage bandwidth to work with, in addition,
we have to use the  hottest running loop to get the numerical intensity.
Which is totally different compare to the memory locality of the algorithm and 
they measure different thing. 

Memory Locality only gives the potential on how much reuse can be made on the 
algorithm, but to get the actual numerical intensity, we need to look at how 
the algorithm behaves at the most computational intensive part, aka, the 
hottest running for loop. And usually it's ok to just ignore the running 
index when we figure that part out. 

Roofline model is always an underestimate. 

I did some further digging on my CPU specs, and I think WSL has lower my 
memory bandwidth cause on windows with professional tools I were able to get
the performance matching what is said on wikichip.