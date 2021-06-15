//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef AMATH583_FINAL_HPP
#define AMATH583_FINAL_HPP

#include <iomanip>
#include <iostream>

#include "Grid.hpp"
#include "Stencil.hpp"
#include "mpiStencil.hpp"

#include <mpi.h>



double mpi_dot(const Grid& X, const Grid& Y) 
{
	double sum = 0.0;

	// Parallelize me
	for (size_t i = 0; i < X.num_x(); ++i) 
	{
		for (size_t j = 0; j < X.num_y(); ++j) 
		{
			sum += X(i, j) * Y(i, j);
		}
	}
    double PartialSum = sum; 
    MPI::COMM_WORLD.Allreduce(&PartialSum, &sum, 1, MPI::DOUBLE, MPI::SUM);

	return sum;
}


size_t jacobi(const mpiStencil& A, Grid& x, const Grid& b, size_t maxiter, double tol, bool debug = false) 
{

	Grid y = b;
	swap(x, y); // x is now b, y is x 
    // initial guess is b.
    
	for (size_t iter = 0; iter < maxiter; ++iter) 
	{
        
        double rho = 0;

        for (size_t i = 1; i < x.num_x() - 1; ++i) 
        {
            for (size_t j = 1; j < x.num_y() - 1; ++j) 
            {
                y(i, j) = (x(i - 1, j) + x(i + 1, j) + x(i, j - 1) + x(i, j + 1)) / 4.0;
                rho += (y(i, j) - x(i, j)) * (y(i, j) - x(i, j)); // Delta
            }
        }
        double PartialRho = rho;
        // !! Accumulate Rho important reeeeee
        MPI::COMM_WORLD.Allreduce(&PartialRho, &rho, 1, MPI::DOUBLE, MPI::SUM);

        if (debug && MPI::COMM_WORLD.Get_rank() == 0) 
        {
            std::cout << std::setw(4) << iter << ": ";
            std::cout << "||r|| = " << std::sqrt(rho) << std::endl;
        }
        if (std::sqrt(rho) < tol) 
        {
            return iter;
        }
       
        swap(x, y); // x is x, y is b 

        HaloUpdate(x);

	}
		
	return maxiter;
}


// Parallelize me
size_t ir(const mpiStencil& A, Grid& x, const Grid& b, size_t max_iter, double tol, bool debug = false) {
	for (size_t iter = 0; iter < max_iter; ++iter) {
		Grid r = b - A*x;

		double sigma = dot(r, r);

		if (debug && MPI::COMM_WORLD.Get_rank() == 0) {
			std::cout << std::setw(4) << iter << ": ";
			std::cout << "||r|| = " << std::sqrt(sigma) << std::endl;
		}

		if (std::sqrt(sigma) < tol) {
			return iter;
		}

		x += r;
        HaloUpdate(x);
	
	}
	return max_iter;
}


// Parallelize me
size_t cg(const mpiStencil& A, Grid& x, const Grid& b, size_t max_iter, double tol, bool debug = false) {
	size_t myrank = MPI::COMM_WORLD.Get_rank();

	Grid r = b - A*x, p(b);
	double rho = dot(r, r), rho_1 = 0.0;

	for (size_t iter = 0; iter < max_iter; ++iter) {
		if (debug && 0 == myrank) {
			std::cout << std::setw(4) << iter << ": ";
			std::cout << "||r|| = " << std::sqrt(rho) << std::endl;
		}

		if (iter == 0) {
			p = r;
		} else {
			double beta = (rho / rho_1);
			p = r +  beta * p;
		}
		
		Grid q = A*p;

		double alpha = rho / dot(p, q);
		
		x += alpha * p;
		
		rho_1 = rho;
		r -= alpha * q;
		rho = dot(r, r);

		if (rho < tol) return iter;
	}

	return max_iter;
}

#endif // AMATH583_FINAL_HPP
