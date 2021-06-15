//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef AMATH583_FNORM_HPP
#define AMATH583_FNORM_HPP

#include "Timer.hpp"
#include "Vector.hpp"
#include "amath583.hpp"
#include "norm_utils.hpp"

#include <cassert>
#include <cmath>
#include <future>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>

// Write us 
double worker_a(const Vector& x, size_t begin, size_t end) {
  double TheSum = 0.0;
  for (size_t II = begin; II < end; ++II)
  {
    TheSum += x(II)*x(II);
  }
  return TheSum;
}

double partitioned_two_norm_a(const Vector& x, size_t partitions) {
  double sum = 0.0;
  std::vector<std::future<double>> Promises(partitions);
  size_t PartitionSize = x.num_rows()/partitions;
  assert (x.num_rows()%PartitionSize == 0); // Added myself just to make sure. 
  for (size_t P = 0; P < partitions; ++P)
  {
    Promises[P] = std::async(
        std::launch::async, 
        worker_a, 
        std::cref(x), 
        P*PartitionSize, 
        (P + 1)*PartitionSize
      );
  }
  for (size_t II = 0; II < Promises.size(); ++II)
  {
    sum += Promises[II].get();
  }
  return std::sqrt(sum);
}

double partitioned_two_norm_b(const Vector& x, size_t partitions) {
  double sum = 0.0;
  std::vector<std::future<double>> Promises(partitions);
  size_t PartitionSize = x.num_rows()/partitions;
  assert (x.num_rows()%PartitionSize == 0); // Added myself just to make sure. 
  for (size_t P = 0; P < partitions; ++P)
  {
    Promises[P] = std::async(
        std::launch::deferred, 
        worker_a, 
        std::cref(x), 
        P*PartitionSize, 
        (P + 1)*PartitionSize
      );
  }
  for (size_t II = 0; II < Promises.size(); ++II)
  {
    sum += Promises[II].get();
  }
  return std::sqrt(sum);
}

#endif    // AMATH_583_FNORM_HPP
