//
// This file is part of the course materials for AMATH 483/583 at the University of Washington
// Spring 2021
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include "amath583.hpp"
#include "omp.h"
#include <iostream>


// Basic (original) outer -- Optimize and parallelize me
void outer(Matrix& A, const Vector& x) 
{
  // No need to implement this is just not significant enough for parallelization. 
  for (size_t i = 0; i < A.num_rows(); ++i) 
  {
    double a = x(i);
    #pragma omp simd
    for (size_t j = 0; j < A.num_cols(); ++j) 
    {
      A(i, j) += a * x(j);
    }
  }

  // #pragma omp parallel for
  // for (size_t i = 0; i < A.num_rows(); ++i) 
  // {
  //   double a = x(i);
  //   for (size_t j = 0; j < A.num_cols(); ++j) 
  //   {
  //     A(i, j) += a * x(j);
  //   }
  // }
}

auto gen_covariance(const std::vector<Vector>& z) {
  size_t sz = z[0].num_rows();
  size_t n = z.size();
  Matrix C(sz, sz);

  size_t NumThreads    = omp_get_max_threads();
  size_t PartitionSize = n/NumThreads;

  #pragma omp parallel
  {
    size_t ThreadNum = omp_get_thread_num();
    size_t Start     = ThreadNum*PartitionSize;
    size_t End       = (ThreadNum + 1)*PartitionSize;
    if (ThreadNum == NumThreads - 1) End = n;
    Matrix PartialC(sz, sz);
    for (size_t k = Start; k < End; ++k) 
    {
      outer(PartialC, z[k]);
    }
    #pragma omp critical
    for (size_t II = 0; II < C.num_rows(); ++II)
    {
      for (size_t JJ = 0; JJ < C.num_cols(); ++JJ)
      {
        C(II, JJ) += PartialC(II, JJ);
      }
    }
  }  

  // for (size_t k = 0; k < n; ++k) 
  // {
  //   outer(C, z[k]);
  // }
  
  // Normalize
  for (size_t i = 0; i < C.num_rows(); ++i) 
  {
    for (size_t j = 0; j < C.num_cols(); ++j) 
    {
      C(i,j) = C(i,j) / z.size();
    }
  }

  return C;
}

const char* tag = "par";

#include "main.hpp"
