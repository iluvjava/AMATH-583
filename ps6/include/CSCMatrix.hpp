//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//
#ifndef AMATH583_CSCMATRIX_HPP
#define AMATH583_CSCMATRIX_HPP

#include "Vector.hpp"
#include <algorithm>
#include <cassert>
#include <vector>

#include "omp.h"

class CSCMatrix {
public:
  CSCMatrix(size_t M, size_t N) : is_open(false), num_rows_(M), num_cols_(N), col_indices_(num_cols_ + 1, 0) {}

  void open_for_push_back() { is_open = true; }

  void close_for_push_back() {
    is_open = false;
    for (size_t i = 0; i < num_cols_; ++i) {
      col_indices_[i + 1] += col_indices_[i];
    }
    for (size_t i = num_cols_; i > 0; --i) {
      col_indices_[i] = col_indices_[i - 1];
    }
    col_indices_[0] = 0;
  }

  void push_back(size_t i, size_t j, double value) { /* Write Me */
    assert(is_open);
    assert(i < num_rows_ && i >= 0);
    assert(j < num_cols_ && j >= 0);

    ++col_indices_[j];
    row_indices_.push_back(i);
    storage_.push_back(value);
  }

  void clear() {
    row_indices_.clear();
    storage_.clear();
    std::fill(col_indices_.begin(), col_indices_.end(), 0);
  }

  size_t num_rows() const { return num_rows_; }
  size_t num_cols() const { return num_cols_; }
  size_t num_nonzeros() const { return storage_.size(); }

  void stream_coordinates(std::ostream& output_file) const {
    for (size_t i = 0; i < num_cols_; ++i) {
      for (size_t j = col_indices_[i]; j < col_indices_[i + 1]; ++j) {
        output_file << row_indices_[j] << " ";
        output_file << i << " ";
        output_file << storage_[j];
        output_file << std::endl;
      }
    }
  }

  void matvec(const Vector& x, Vector& y) const 
  {
    
    // #pragma omp parallel 
    // {
    //   size_t NumThreads    = omp_get_num_threads();
    //   size_t ThreadNum     = omp_get_thread_num();
    //   size_t PartitionSize = num_cols_/NumThreads;
    //   size_t Start         = ThreadNum*PartitionSize;
    //   size_t End           = Start + PartitionSize;
    //   if (ThreadNum == NumThreads - 1) End = num_cols_;
    //   for (size_t JJ = Start; JJ < End; ++JJ) 
    //   {
    //     for (size_t Idx = col_indices_[JJ]; Idx < col_indices_[JJ + 1]; ++Idx) 
    //     {
    //       #pragma omp critical
    //       y(row_indices_[Idx]) += storage_[Idx] * x(JJ);
    //     }
    //   }
    // }
    for (size_t JJ = 0; JJ < num_cols_; ++JJ) 
    {
      for (size_t Idx = col_indices_[JJ]; Idx < col_indices_[JJ + 1]; ++Idx) 
      {
        y(row_indices_[Idx]) += storage_[Idx] * x(JJ);
      }
    }
  }

  void t_matvec(const Vector& x, Vector& y) const 
  {
    #pragma omp parallel for schedule(dynamic, 1024)
    for (size_t i = 0; i < num_cols_; ++i)
    {
      for (size_t j = col_indices_[i]; j < col_indices_[i + 1]; ++j) 
      {
        y(i) += storage_[j] * x(row_indices_[j]);
      }
    }
  }

  void matmat(const Matrix& B, Matrix& C) const 
  {
    // size_t NumThreads    = omp_get_max_threads();
    // size_t PartitionSize = num_cols_/NumThreads;
    // #pragma omp parallel
    // {
    //   size_t ThreadNum = omp_get_thread_num();
    //   size_t Start     = ThreadNum*PartitionSize;
    //   size_t End       = Start + PartitionSize;
    //   if (ThreadNum == NumThreads - 1)
    //   End = num_cols_;
      
    //   for (size_t i = Start; i < End; ++i) 
    //   {
    //     for (size_t j = col_indices_[i]; j < col_indices_[i + 1]; ++j) 
    //     {
    //       std::vector<double> Temp(C.num_cols());
    //       for (size_t k = 0; k < B.num_cols(); ++k) 
    //       {
            
    //         C(row_indices_[j], k) += storage_[j] * B(i, k);
    //       }
    //     }
    //   }
    // }
    for (size_t i = 0; i < num_cols_; ++i)
    {
      for (size_t j = col_indices_[i]; j < col_indices_[i + 1]; ++j) 
      { 
        for (size_t k = 0; k < B.num_cols(); ++k) 
        {
          C(row_indices_[j], k) += storage_[j] * B(i, k);
        }
      }
    }
  }

  void stochastify() {
    std::vector<size_t> degree(num_rows_);
    for (size_t i = 0; i < num_cols_; ++i) {
      for (size_t j = col_indices_[i]; j < col_indices_[i + 1]; ++j) {
        ++degree[row_indices_[j]];
      }
    }
    for (size_t i = 0; i < num_cols_; ++i) {
      for (size_t j = col_indices_[i]; j < col_indices_[i + 1]; ++j) {
        storage_[j] = 1. / degree[row_indices_[j]];
      }
    }
  }

private:
  bool                is_open;
  size_t              num_rows_, num_cols_;
  std::vector<size_t> col_indices_, row_indices_;
  std::vector<double> storage_;
};

#endif    // AMATH583_CSCMATRIX_HPP
