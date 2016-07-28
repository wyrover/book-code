// Listing 66-7. Rewriting string_pool to Use hash<>
#include <iostream>
#include <istream>
#include <utility>
#include "hash.hpp"        // Listing 66-6

#include "string_pool.hpp" // Copied from Listing 66-5


int main()
{
   string_pool pool{};
   std::string str{};
   hash<std::string> hasher{};
   while (std::cin >> str)
   {
      std::cout << "hash of \"" << str << "\" = " << hasher(str) << '\n';
      pool.add(std::move(str));
   }
}
