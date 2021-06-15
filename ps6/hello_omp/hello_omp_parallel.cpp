//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//
//
#include <omp.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include "getenv.hpp"

int main() {

  std::cout << "================================================================" << std::endl;
  std::cout << "#pragma omp parallel" << std::endl;
  std::cout << "================================================================" << std::endl;
  std::string envName = "OMP_NUM_THREADS";
  std::cout << envName << "        = " << getenv(envName) << std::endl;
  std::cout << "hardware_concurrency() = " << std::thread::hardware_concurrency() << std::endl;
  std::cout << "omp_get_num_threads()  = " << omp_get_num_threads() << std::endl;
  std::cout << "================================================================" << std::endl;

  // Such a use of omp api doesn't take care of the race conditions on threads. 
  // The outputs will display artifacts from race conditions. 
  #pragma omp parallel
  {
    //Thread num is literally the number of thread spawned, it's between 1 and the number of threads involvevd by open 
    // mp. 
    std::cout << "Hello! I am thread " << omp_get_thread_num() << " of " << omp_get_num_threads() << std::endl;
    std::cout << "My C++ std::thread id is " << std::this_thread::get_id() << std::endl;
  }

  std::cout << "================================================================" << std::endl;

  return 0;
}
