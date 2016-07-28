// Listing 64-15. The find_pair Function and Test Program
#include <bitset>
#include <cassert>
#include <cstdlib>
#include <iostream>

template<std::size_t N>
std::size_t find_pair(std::bitset<N> const& bitset, bool value)
{
   if (bitset.size() >= 2)
      for (std::size_t i{bitset.size()}; i-- != 1; )
         if (bitset[i] == value and bitset[i-1] == value)
            return i;
   return std::size_t(-1);
}

int main()
{
   std::size_t const not_found{~0u};
   std::bitset<0> bs0{};
   std::bitset<1> bs1{};
   std::bitset<2> bs2{};
   std::bitset<3> bs3{};
   std::bitset<100> bs100{};

   assert(find_pair(bs0, false) == not_found);
   assert(find_pair(bs0, true) == not_found);
   assert(find_pair(bs1, false) == not_found);
   assert(find_pair(bs1, true) == not_found);
   assert(find_pair(bs2, false) == 1);
   assert(find_pair(bs2, true) == not_found);
   bs2[0] = true;
   assert(find_pair(bs2, false) == not_found);
   assert(find_pair(bs2, true) == not_found);
   bs2.flip();
   assert(find_pair(bs2, false) == not_found);
   assert(find_pair(bs2, true) == not_found);
   bs2[0] = true;
   assert(find_pair(bs2, false) == not_found);
   assert(find_pair(bs2, true) == 1);
   assert(find_pair(bs3, false) == 2);
   assert(find_pair(bs3, true) == not_found);
   bs3[2].flip();
   assert(find_pair(bs3, false) == 1);
   assert(find_pair(bs3, true) == not_found);
   bs3[1].flip();
   assert(find_pair(bs3, false) == not_found);
   assert(find_pair(bs3, true) == 2);
   assert(find_pair(bs100, false) == 99);
   assert(find_pair(bs100, true) == not_found);
   bs100[50] = true;
   assert(find_pair(bs100, true) == not_found);
   bs100[51] = true;
   assert(find_pair(bs100, true) == 51);
}
