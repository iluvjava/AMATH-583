//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef AMATH583_HPP
#define AMATH583_HPP

#include "Matrix.hpp"
#include "Vector.hpp"
#include <cstddef>

void   zeroize(Vector& x); // ! 5.3 COMPLETED. 
double one_norm(const Vector& x); 
double two_norm(const Vector& x); // ! 5.4 COMPLETED. 
double inf_norm(const Vector& x); // ! 5.4 COMPLETED.

// Also need to add dot()
double dot(const Vector&, const Vector&);
// and optionally two_norm_d()

double two_norm_d(const Vector &x);

double one_norm(const Matrix& x); // !

double inf_norm(const Matrix& A); // ! 5.6 COMPLETED 

double f_norm(const Matrix& A); // ! 5.6.1 COMPLETED

#endif    // AMATH583_HPP
