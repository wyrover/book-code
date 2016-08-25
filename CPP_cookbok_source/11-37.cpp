#include "bitset_arithmetic.hpp"

#include <bitset>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    bitset<10> bits1(string("100010001"));
    bitset<10> bits2(string("000000011"));
    bitsetAdd(bits1, bits2);
    cout << bits1.to_string<char, char_traits<char>, allocator<char> >() << endl;
}