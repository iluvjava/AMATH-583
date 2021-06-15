//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2017
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

double pi_helper(size_t begin, size_t end, double h) {
  double pi_i = 0.0;
  for (size_t i = begin; i < end; ++i)
    pi_i += (h * 4.0) / (1.0 + (i * h * i * h));
  return pi_i;
}

int main(size_t argc, char* argv[]) {
  size_t partitions = magically_get_P();
  size_t my_id      = magically_get_id();

  size_t N          = -1;
  if (my_id == 0) {
    N = atol(argv[1]);
  }
  size_t block_size = N / partitions;
  size_t begin      = block_size * my_id;
  size_t end        = block_size * (my_id + 1);
  double h          = 1.0 / (double) N;

  for (size_t i = begin; i < end; ++i)
    pi += (h * 4.0) / (1.0 + (i * h * i * h));

  if (my_id == 0) {
    std::cout << "pi is ~ " << pi << std::endl;
  }

  return 0;
}


int main() {
  size_t N = 1024 * 1024;
  double pi = 0.0; double h = 1./(double) N;
  for (size_t i = 0; i < N/4; ++i)
    pi += (h * 4.0) / (1.0 + (i * h * i * h));
  std::cout << "pis is ~ " <<  pi << std::endl

  return 0;
}



int main() {
  double pi = 0.0; double h = 1./(double) N;
  for (size_t i = 0; i < N/4; ++i)
    pi += (h * 4.0) / (1.0 + (i * h * i * h));
  std::cout << "pis is ~ " <<  pi << std::endl

  return 0;
}
