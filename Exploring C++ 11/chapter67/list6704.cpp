// Listing 67-4. Using Character Traits When Copying Input to Output
#include <iostream>
#include <istream>
#include <ostream>
#include <string>        // for char_traits

int main()
{
   typedef std::char_traits<char> char_traits; // for brevity and clarity
   char_traits::int_type c{};
   while (c = std::cin.get(), not char_traits::eq_int_type(c, char_traits::eof()))
      std::cout.put(char_traits::to_char_type(c));
}
