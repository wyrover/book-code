// Listing 67-2. Example Program to Use the my_advance Function
#include <fstream>
#include <iostream>
#include <istream>
#include <iterator>
#include <list>
#include <ostream>
#include <string>
#include <vector>

#include "advance.hpp" // Listing 67-1

int main()
{
   std::vector<int> vector{ 10, 20, 30, 40 };
   std::list<int> list(vector.begin(), vector.end());
   std::vector<int>::iterator vector_iterator{vector.begin()};
   std::list<int>::iterator list_iterator{list.begin()};
   std::ifstream file{"advance.hpp"};
   std::istream_iterator<std::string> input_iterator{file};

   my_advance(input_iterator, 2);
   my_advance(list_iterator, 2);
   my_advance(vector_iterator, 2);
}

