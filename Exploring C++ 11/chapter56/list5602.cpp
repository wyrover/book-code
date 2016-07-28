// Listing 56-2. Implementing the Debug Function
#include <fstream>
#include <ostream>
#include <stdexcept>

#include <string>
#include "debug.hpp"

void debug(std::string const& str)
{
   std::ofstream stream{"debug.txt", std::ios_base::out | std::ios_base::app};
   if (not stream)
      throw std::runtime_error("cannot open debug.txt");
   stream.exceptions(std::ios_base::failbit);
   stream << str << '\n';
   stream.close();
}

