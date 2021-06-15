//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef AMATH583_CNORM_HPP
#define AMATH583_CNORM_HPP

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


double cyclic_two_norm(const Vector& x, size_t partitions) {
  double sum = 0.0;
  std::vector<std::future<double>> Promises(partitions);
  for (size_t P = 0; P < partitions; ++P)
  {
    Promises[P] = std::async(
      std::launch::async, 
      [&x, &partitions](size_t PartitionID) -> double {
        double Result = 0; 
        for (size_t II = PartitionID; II < x.num_rows(); II+= partitions)
        {
          Result += x(II)*x(II);
        }
        return Result;
      }, 
      P
    );
  }
  for (size_t P = 0; P < partitions; ++P)
  {
    sum += Promises[P].get();
  }
  return std::sqrt(sum);
}

#endif    // AMATH583_CNORM_HPP
