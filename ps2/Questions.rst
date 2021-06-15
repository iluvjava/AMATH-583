
PS2 Questions
=============

Add your answers to this file in plain text after each question.  Leave a blank line between the text of the question and the text of your answer and at least two lines between your answer and the next question.

.. role:: cpp(code)
   :language: c++


Inner Product
-------------

1. How would you use :cpp:`dot` to implement :cpp:`two_norm`?

- I ues it like :cpp:`sqrt(dot(x, x))` where :cpp:`x` is the input vector for the norm function. It's also better to add a proto proto func `double dot(Vector&, Vector&)` at the start of file so we can use the function `dot` after that line. 

Tensors
-------

2. What is the formula that we would use for indexing if we wanted "slab-major" ordering?  Hint:  What is the size of a slab?  It might be helpful to draw a picture of the data in 3D and see what the size is of a slab.

- A row major indexing strategies were used for matrix, and elements of the same rows are stored together in the memory. In the case of slab major indexing, all the elements stored in the same slab be in the sequential memory in RAM.  So when we do memory addressing, we enter slab, and then the rows, and the columns. Let k, m, n denotes the shape in(Number of elements) `slab, rows, columns`, let (i, j, k) be the indexer inputs, then the indexing strategy will be: :cpp:`k*m*n + j*m + i`. This goes to the k th page (or slab) and then the ith row and the j th column of the tensor. 

3. (Extra credit.) I claim that there are six distinct indexing formulas.  Show that this claim is correct or that it is incorrect.

- It's correct because every unique permutations of 3 elements corresponds to an indexing strategies for the tensor. And in general, the way to index a n dimensional tensor will be `n!`, which is a lot. Let's count all of them in the case of a tensor: `page -> row -> col`, `page -> col -> row`, `row -> page -> col`, `row -> col -> page`, `col -> row -> page`, `col -> page -> row`, six of them in total. 
