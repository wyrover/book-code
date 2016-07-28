// Listing 64-14. Example of Using std::bitset
#include <bitset>
#include <cstdlib>
#include <iostream>
#include <string>

/** Find the first 1 bit in a bitset, starting from the most significant bit.
 * @param bitset The bitset to examine
 * @return A value in the range [0, bitset.size()-1) or
 *         size_t(-1) if bitset.none() is true.
 */
template<std::size_t N>
std::size_t first(std::bitset<N> const& bitset)
{
   for (std::size_t i{bitset.size()}; i-- != 0;)
      if (bitset.test(i))
         return i;
   return std::size_t(-1);
}

int main()
{
   std::bitset<50> lots_o_bits{std::string{"1011011101111011111011111101111111"}};
   std::cout << "bitset: " << lots_o_bits << '\n';
   std::cout << "first 1 bit: " << first(lots_o_bits) << '\n';
   std::cout << "count of 1 bits: " << lots_o_bits.count() << '\n';
   lots_o_bits[first(lots_o_bits)] = false;
   std::cout << "new first 1 bit: " << first(lots_o_bits) << '\n';
   lots_o_bits.flip();
   std::cout << "bitset: " << lots_o_bits << '\n';
   std::cout << "first 1 bit: " << first(lots_o_bits) << '\n';
}
