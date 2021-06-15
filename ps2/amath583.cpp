//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include "amath583.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"
#include <cmath>
//#include <corecrt_math.h>
#include <cstddef>
//#include <vcruntime.h>

double dot(const Vector& x, const Vector& y); // proto func

void zeroize(Vector& v) 
{
  for (size_t II = 0; II < v.num_rows(); ++II)
  {
    v(II) = 0;
  }
}

double one_norm(const Vector& x) { 
  double sum = 0.0;
  for (size_t i = 0; i < x.num_rows(); ++i) {
    sum += std::abs(x(i));
  }
  return sum;
}


double two_norm(const Vector& x) 
{
  // assert(x.num_rows() >= 1 && "Twonorm can't accept zero length vector. ");
  return sqrt(dot(x, x));
}

double inf_norm(const Vector& x) 
{
  double result = 0.0;
  for (size_t II = 0; II < x.num_rows(); ++II)
  {
    result = fmax(result, std::abs(x(II)));
  }
  return result;
}

//! 5.5 COMPLETED. 
double dot(const Vector& x, const Vector& y) {
  // ALGORITHM: Khan Summation, this takes the credit. 
  double Sum          = 0.0;
  double Compensation = 0.0;
  for (size_t II = 0; II < fmin(x.num_rows(), y.num_rows()); ++II)
  {
    double Sumee        = x(II)*y(II);
    double t            = Sum + Sumee;
           Compensation = (t - Sum) - Sumee;
           Sum          = t;
  }
  return Sum;
}

double two_norm_d(const Vector& x) 
{ 
  return sqrt(dot(x, x));  
}

double one_norm(const Matrix& A) {
  double sum = 0.0;
  for (size_t j = 0; j < A.num_cols(); ++j) {    // for each column
    double tmp = 0.0;
    for (size_t i = 0; i < A.num_rows(); ++i) {    // compute column sum
      tmp += std::abs(A(i, j));
    }
    sum = std::max(sum, tmp);
  }
  return sum;
}


double inf_norm(const Matrix& A) 
{
  double MaxColumnSum = 0.0;
  for (size_t II = 0; II < A.num_rows(); ++II)
  {
    double ColSum = 0;
    for (size_t JJ = 0; JJ < A.num_cols(); ++JJ)
    {
      ColSum += std::abs(A(II, JJ));
    }
    MaxColumnSum = fmax(ColSum, MaxColumnSum);
  }
  return MaxColumnSum;
}


double f_norm(const Matrix& A)
{
  // ALGO: Khan Summation. 
  double Sum        = 0.0;
  double Compensate = 0.0;
  for (size_t II = 0; II < A.num_rows(); ++II)
  {
    for (size_t JJ = 0; JJ < A.num_cols(); ++JJ)
    {
      double Sumee      = A(II, JJ) - Compensate;
      double t          = Sum + Sumee;
             Compensate = (t - Sum) - Sumee;
             Sum        = t;
    }
  }
  return sqrt(Sum);
}