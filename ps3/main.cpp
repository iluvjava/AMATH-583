//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2020
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//

#include <iostream>
#include <string>
#include <system_error>

int main(int argc, char* argv[])  // second argument: array of pointers. 
{
  if (argc != 2)
  {
    std::cerr << "usage " << argv[0] << " size" << std::endl; 
    return -1; 
  }

  std::string s(argv[1]);
  long size = std::stol(s, nullptr, 10);
  std::cout << "size is " << size << std::endl;
  return  0;
}
