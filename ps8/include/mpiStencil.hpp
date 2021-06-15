//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef AMATH583_MPI_STENCIL_HPP
#define AMATH583_MPI_STENCIL_HPP

#include "Grid.hpp"

#include <mpi.h>
#include <vector>

class mpiStencil { };


void mult(const mpiStencil& A, const Grid& x, Grid& y) {
  size_t myrank = MPI::COMM_WORLD.Get_rank();
  size_t mysize = MPI::COMM_WORLD.Get_size();

  // Write me Ghost cell (halo) update goes here
  // Boundaries are row 0 and row x.num_x() - 1


  // SPMD stencil application
  for (size_t i = 1; i < x.num_x() - 1; ++i) {
    for (size_t j = 1; j < x.num_y() - 1; ++j) {
      y(i, j) = x(i, j) - (x(i - 1, j) + x(i + 1, j) + x(i, j - 1) + x(i, j + 1)) / 4.0;
    }
  }
}


Grid operator*(const mpiStencil& A, const Grid& x) {
  Grid y(x);
  mult(A, x, y);
  return y;
}


#endif // AMATH583_MPI_STENCIL_HPP
