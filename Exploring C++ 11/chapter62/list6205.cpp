// Listing 62-5. Using Pointer Arithmetic to Determine the Length of a C String
#include <cstddef>

std::size_t my_std_strlen(char const* str)
{
   char const* start{str};      // remember the start of the string
   while (*str != 0)            // while not at the end of the string
      ++str;                    // advance to the next character
   return str - start;          // compute string length by subtracting pointers
}
