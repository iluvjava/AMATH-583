//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include <cmath>
#include <iostream>

#include "Timer.hpp"
#include "Vector.hpp"
#include "amath583.hpp"

#include "omp.h"

double norm_seq(const Vector& x) {
  double sum = 0;

  for (size_t i = 0; i < x.num_rows(); ++i) {
    sum += x(i) * x(i);
  }
  return std::sqrt(sum);
}
/**
 * 
 * 
 */
double norm_block_reduction(const Vector& x) 
{
  double sum = 0;
  // spwan thread and directly put the blow code into each threads. 
  // and then runs it, protec race conditions on the sum variable. 
  // Note: Reduction should be directly applied on the variable we 
  // want to reduce on. 
  #pragma omp parallel reduction(+:sum)
  {
    size_t tid       = omp_get_thread_num();
    size_t parts     = omp_get_num_threads();
    size_t blocksize = x.num_rows() / parts;
    size_t begin     = tid * blocksize;
    size_t end       = (tid + 1) * blocksize;
    if (tid == parts - 1) 
    {
      end = x.num_rows();
    }

    for (size_t i = begin; i < end; ++i) 
    {
      sum += x(i) * x(i);
    }
  }
  return std::sqrt(sum);
}

double norm_block_critical(const Vector& x) 
{
  double sum = 0;

  // Spawn the thread to work on each of the own works first and then. 
  // combine the final works in a critical section. 
  #pragma omp parallel
  {
    size_t tid       = omp_get_thread_num();
    size_t parts     = omp_get_num_threads();
    size_t blocksize = x.num_rows() / parts;
    size_t begin     = tid * blocksize;
    size_t end       = (tid + 1) * blocksize;
    if (tid == parts - 1) 
    {
      end = x.num_rows();
    }
    double PartialSum = 0;
    
    #pragma omp simd
    for (size_t i = begin; i < end; ++i) 
    {
      PartialSum += x(i) * x(i);
    }
    
    #pragma omp critical
    {
      sum += PartialSum;
    }
  }
  return std::sqrt(sum);
}

double norm_cyclic_critical(const Vector& x) 
{
  double sum = 0;
  // Spawn threads, for each thread, work on their own task and thn combine the 
  // results in a critical section 
  #pragma omp parallel
  {
    size_t ThreadNum = omp_get_thread_num();
    size_t NumThread = omp_get_num_threads();
    double PartialSum = 0;
    for (size_t II = ThreadNum; II < x.num_rows(); II+=NumThread)
    {
      PartialSum += x(II)*x(II);
    }

    #pragma omp critical 
    {
      sum += PartialSum;
    }
  }
  return std::sqrt(sum);
}

double norm_cyclic_reduction(const Vector& x) 
{

  double sum = 0;
  #pragma omp parallel reduction(+:sum)
  {
    size_t ThreadNum = omp_get_thread_num();
    size_t NumThread = omp_get_num_threads();
    
    for (size_t II = ThreadNum; II < x.num_rows(); II+=NumThread)
    {
      sum += x(II)*x(II);
    }
  
  }
  return std::sqrt(sum);
}

double norm_parfor(const Vector& x) {
  double sum = 0;

  /* Parallelize me  */
  #pragma omp parallel for reduction(+:sum)
  for (size_t i = 0; i < x.num_rows(); ++i) {  // Don't
    sum += x(i) * x(i);                        // change
  }                                            // me

  return std::sqrt(sum);
}
