Needs: 
1. Lecture 9: Sparse Matrix. 
2. The Markov Transition and the Page rank algorithm 

---
### **Tasks**

*  Matrix Vector Products
*  Transpoes Matrix Vector Products.

---
### 3.1 
TODO-1

---
### **4**
Complete function for `matvec.exe`

### **4.1  COO Matrix** 

Storing the coordinates, and the value in an array. 

**Implement** the Matrix vector and the transpose matrix vector function. 

### **4.1  CSR Matrix**

Compressed storage of the sparse row matrix. 

Sort the elements in COO by row index. Then compress it, where each element represent how many numbers there are in the row of the matrix while we are iterating through it. 

`row_indices_`: The running total. 

`row_indices_[i]`: The total number of elements from the `1` st row to the `i - 1` th row. 

**Implement** the Matrix vector multiplication and the Matrix transpose vector multiplication function. 

### **4.2  CSC Matrix**

The same as the Compressed row matrix, but this time we sort the vector that stores the coordinates of the column, and then compress them. 

### **4.3  Array of Structs**

* Using tuples inside of array instead of separate arrays for each of the data. 


### **4.4 Sparse Matrix Vector Conceptual Question**

To-Do 5: Is the hypothesis that, AOS matrix without the transpose is the fastest? 
