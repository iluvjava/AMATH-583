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

class mpiStencil { }; // this class only have operators. 


void mult(const mpiStencil& A, const Grid& x, Grid& y) 
{
    // !! Write me Ghost cell (halo) update goes here
    // !! Boundaries are row 0 and row x.num_x() - 1

    // Perform Stencil operations. 
    for (size_t i = 1; i < x.num_x() - 1; ++i) 
    {
        for (size_t j = 1; j < x.num_y() - 1; ++j) 
        {
            y(i, j) = x(i, j) - (x(i - 1, j) + x(i + 1, j) + x(i, j - 1) + x(i, j + 1)) / 4.0;
        }
    }
    size_t myrank = MPI::COMM_WORLD.Get_rank();
    size_t mysize = MPI::COMM_WORLD.Get_size();

    size_t  Width  = y.num_x();
	size_t Height = y.num_y();

    // SYNC Boundaries. 
    if (myrank == 0 || myrank == mysize - 1)
    {   
        // Bottom or top row for current process's grid
        std::vector<double> OneExtraRow(Width); 
        size_t RowToUpdate;
        if (myrank == 0)
        {
            // Send and receive bottom row with: rank  + 1
            MPI::COMM_WORLD.Isend(&y(Height, 1), Width, MPI::DOUBLE, 1, 0); 
            MPI::COMM_WORLD.Recv(&OneExtraRow[0], Width, MPI::DOUBLE, 1, 0);
            RowToUpdate = Height;
        }
        else
        {
            // Send and receive top row with: rank - 1
            MPI::COMM_WORLD.Isend(&y(0, 1), Width, MPI::DOUBLE, myrank - 1, 0); 
            MPI::COMM_WORLD.Recv(&OneExtraRow[0], Width, MPI::DOUBLE, myrank - 1, 0);
            RowToUpdate = 0;
        }
        for (size_t II = 0; II < Width; ++II)
        {
            y(RowToUpdate, II + 1) = OneExtraRow[II];
        }
    }
    else
    {   
        // Bottom and the top row for current process's grid. 
        std::vector<double> TopRow(Width); 
        std::vector<double> BottomRow(Width);
        MPI::COMM_WORLD.Isend(&y(0, 1), Width, MPI::DOUBLE, myrank - 1, 0);
        MPI::COMM_WORLD.Isend(&y(Height, 1), Width, MPI::DOUBLE, myrank + 1, 0);
        MPI::COMM_WORLD.Recv(&TopRow[0], Width, MPI::DOUBLE, myrank - 1, 0); 
        MPI::COMM_WORLD.Recv(&BottomRow[0], Width,MPI::DOUBLE, myrank + 1, 0); 
        for (size_t II = 0; II < Width; ++II)
        {
            y(0, II + 1) = TopRow[II]; 
            y(Height, II + 1) = BottomRow[II]; 
        }
    }
}


Grid operator*(const mpiStencil& A, const Grid& x) {
    Grid y(x);
    mult(A, x, y);
    return y;
}


#endif // AMATH583_MPI_STENCIL_HPP
