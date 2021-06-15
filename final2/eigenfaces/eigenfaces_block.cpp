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

// Parallelize, optimize, and cache block me
auto gen_covariance(const std::vector<Vector>& z) 
{
  size_t sz = z[0].num_rows();
  Matrix C(sz, sz);
  for (size_t KK = 0; KK < z.size(); ++KK) 
  {
    // Partition by blocks of rows. 
    #pragma omp parallel for schedule(dynamic, 32)
    for (size_t II = 0; II < C.num_rows(); ++II) 
    {
      double a = z[KK](II); 
      for (size_t JJ = 0; JJ < II; ++JJ)   
      {
	      C(II, JJ) += a * z[KK](JJ);
      }
    }
  }
  #pragma omp parallel for 
  for (size_t II = 0; II < C.num_rows(); ++II)
  {
    for (size_t JJ = II + 1; JJ < C.num_cols(); ++JJ)
    {
      C(II, JJ) += C(JJ, II);
    }
  }

  // Normalize
  #pragma omp parallel for
  for (size_t i = 0; i < C.num_rows(); ++i) {
    for (size_t j = 0; j < C.num_cols(); ++j) {
      C(i,j) = C(i,j) / z.size();
    }
  }

  return C;
}


const char* tag = "block";

#include "main.hpp"
