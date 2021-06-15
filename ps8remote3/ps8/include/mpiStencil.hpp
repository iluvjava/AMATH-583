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

// #define PRODUCTION

void HaloUpdate(Grid&y)
{
    int myrank = MPI::COMM_WORLD.Get_rank();
    int mysize = MPI::COMM_WORLD.Get_size();
    size_t  Width = y.num_y();
    size_t Height = y.num_x();
    int Sendup = 1, SendDown = 0;


    // COMMUTE and Hallo Exchange
    if (mysize != 1)
    {
        
        if (myrank == 0 || myrank == mysize - 1)
        {   
            // Bottom or top row for current process's grid
            if (myrank == 0)
            {
                // Send and receive bottom row with: rank  + 1
                MPI::COMM_WORLD.Recv(&y(Height - 1, 0), Width, MPI::DOUBLE, 1, Sendup);
                MPI::COMM_WORLD.Send(&y(Height - 2, 0), Width, MPI::DOUBLE, 1, SendDown); 
            }   
            else
            {
                // Send and receive top row with: rank - 1
                MPI::COMM_WORLD.Send(&y(1, 0), Width, MPI::DOUBLE, myrank - 1, Sendup); 
                MPI::COMM_WORLD.Recv(&y(0, 0), Width, MPI::DOUBLE, myrank - 1, SendDown);
            }
        }
        else
        {   
            // Bottom and the top row for current process's grid. 
            MPI::COMM_WORLD.Send(&y(1, 0),   Width, MPI::DOUBLE,        myrank - 1, Sendup);
            MPI::COMM_WORLD.Recv(&y(Height - 1, 0), Width,MPI::DOUBLE,  myrank + 1, Sendup); 
            MPI::COMM_WORLD.Send(&y(Height - 2, 0), Width, MPI::DOUBLE, myrank + 1, SendDown);
            MPI::COMM_WORLD.Recv(&y(0, 0),   Width, MPI::DOUBLE,        myrank - 1, SendDown); 
        }
    }

}

void mult(const mpiStencil& A, const Grid& x, Grid& y) 
{
    // !! Write me Ghost cell (halo) update goes here
    // !! Boundaries are row 0 and row x.num_x() - 1
    // Perform Stencil operations. 

    // Sync up the boundary for each of the partition using the copied value of X grid. 
    Grid Xcpy = x;
    HaloUpdate(Xcpy);
    for (size_t i = 1; i < x.num_x() - 1; ++i) 
    {
        for (size_t j = 1; j < x.num_y() - 1; ++j) 
        {
            y(i, j) = Xcpy(i, j) - (Xcpy(i - 1, j) + Xcpy(i + 1, j) + Xcpy(i, j - 1) + Xcpy(i, j + 1)) / 4.0;
        }
    }

}


Grid operator*(const mpiStencil& A, const Grid& x) {
    Grid y(x);
    mult(A, x, y);
    return y;
}


#endif // AMATH583_MPI_STENCIL_HPP
