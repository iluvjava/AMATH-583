//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//
#ifndef AMATH583_CSRMATRIX_HPP
#define AMATH583_CSRMATRIX_HPP

#include "Matrix.hpp"
#include "Vector.hpp"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

#include <future>
#include <omp.h>

class CSRMatrix {
public:
  CSRMatrix(size_t M, size_t N) : is_open(false), num_rows_(M), num_cols_(N), row_indices_(num_rows_ + 1, 0) {}

  void open_for_push_back() { is_open = true; }

  void close_for_push_back() {
    is_open = false;
    for (size_t i = 0; i < num_rows_; ++i) {
      row_indices_[i + 1] += row_indices_[i];
    }
    for (size_t i = num_rows_; i > 0; --i) {
      row_indices_[i] = row_indices_[i - 1];
    }
    row_indices_[0] = 0;
  }

  void push_back(size_t i, size_t j, double value) {
    assert(is_open);
    assert(i < num_rows_ && i >= 0);
    assert(j < num_cols_ && j >= 0);

    ++row_indices_[i];
    col_indices_.push_back(j);
    storage_.push_back(value);
  }

  void clear() {
    col_indices_.clear();
    storage_.clear();
    std::fill(row_indices_.begin(), row_indices_.end(), 0);
  }

  size_t num_rows() const { return num_rows_; }
  size_t num_cols() const { return num_cols_; }
  size_t num_nonzeros() const { return storage_.size(); }

  void stream_coordinates(std::ostream& output_file) const {
    for (size_t i = 0; i < num_rows_; ++i) {
      for (size_t j = row_indices_[i]; j < row_indices_[i + 1]; ++j) {
        output_file << i << " ";
        output_file << col_indices_[j] << " ";
        output_file << storage_[j];
        output_file << std::endl;
      }
    }
  }

  void matvec(const Vector& x, Vector& y) const 
  {
    #pragma omp parallel for
    for (size_t i = 0; i < num_rows_; ++i) 
    {    
      for (size_t j = row_indices_[i]; j < row_indices_[i + 1]; ++j) 
      {
        y(i) += storage_[j] * x(col_indices_[j]);
      }
    }
  }

  void t_matvec(const Vector& x, Vector& y) const 
  {

    // size_t NumThreads    = omp_get_max_threads();
    // size_t PartitionSize = num_rows_/NumThreads;
    // #pragma omp parallel
    // {
    //   size_t ThreadNum = omp_get_thread_num();
    //   size_t Start     = ThreadNum*PartitionSize;
    //   size_t End       = (ThreadNum + 1)*PartitionSize;
    //   if (ThreadNum == NumThreads - 1) End = num_rows_;
    //   for (size_t II = Start; II < End; ++II) 
    //   {
    //     for (size_t Idx = row_indices_[II]; Idx < row_indices_[II + 1]; ++Idx) 
    //     { 
    //       #pragma omp critical
    //       y(col_indices_[Idx]) += storage_[Idx] * x(II);
    //     }
    //   }
    // }
    for (size_t II = 0; II < num_rows_; ++II) 
    {
      for (size_t Idx = row_indices_[II]; Idx < row_indices_[II + 1]; ++Idx) 
      { 
        y(col_indices_[Idx]) += storage_[Idx] * x(II);
      }
    }
  }

  void matmat(const Matrix& B, Matrix& C) const 
  {
      #pragma omp parallel for
      for (size_t i = 0; i < num_rows_; ++i) 
      {
        for (size_t j = row_indices_[i]; j < row_indices_[i + 1]; ++j) 
        {
          for (size_t k = 0; k < B.num_cols(); ++k) 
          {
            C(i, k) += storage_[j] * B(col_indices_[j], k);
          }
        }
      }
  }

  void stochastify() {
    for (size_t i = 0; i < num_rows_; ++i) {
      double degree = static_cast<double>(row_indices_[i + 1] - row_indices_[i]);
      for (size_t j = row_indices_[i]; j < row_indices_[i + 1]; ++j) {
        storage_[j] = 1. / degree;
      }
    }
  }

private:
  bool                is_open;
  size_t              num_rows_, num_cols_;
  std::vector<size_t> row_indices_, col_indices_;
  std::vector<double> storage_;
};

#endif    // AMATH583_CSRMATRIX_HPP
