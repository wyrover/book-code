#include <bitset>
#include <iostream>
using namespace std;

int main()
{
    auto str1 = "0011001100";
    auto str2 = "0000111100";
    bitset<10> bitsOne(str1);
    bitset<10> bitsTwo(str2);
    auto bitsThree = bitsOne & bitsTwo;
    cout << bitsThree << endl;
    bitsThree <<= 4;
    cout << bitsThree << endl;
    return 0;
}
