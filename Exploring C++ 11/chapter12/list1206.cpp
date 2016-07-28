// Listing 12-6. Demonstrating How C++ Compares Strings
#include <iostream>
#include <string>

int main()
{
   std::string a{"abc"}, b{"abc"};
   if (a != b) std::cout << "wrong: abc != abc\n";
   if (a < b)  std::cout << "wrong: abc < abc\n";
   if (a > b)  std::cout << "wrong: abc > abc\n";
   if (a == b) std::cout << "okay: abc == abc\n";
   if (a >= b) std::cout << "okay: abc >= abc\n";
   if (a <= b) std::cout << "okay: abc <= abc\n";

   a.push_back('d');
   if (a != b) std::cout << "okay: abcd != abc\n";
   if (a < b)  std::cout << "wrong: abcd < abc\n";
   if (a > b)  std::cout << "okay: abcd > abc\n";
   if (a == b) std::cout << "wrong: abcd == abc\n";
   if (a >= b) std::cout << "okay: abcd >= abc\n";
   if (a <= b) std::cout << "wrong: abcd <= abc\n";

   b.push_back('e');
   if (a != b) std::cout << "okay: abcd != abce\n";
   if (a < b)  std::cout << "okay: abcd < abce\n";
   if (a > b)  std::cout << "wrong: abcd > abce\n";
   if (a == b) std::cout << "wrong: abcd == abce\n";
   if (a >= b) std::cout << "wrong: abcd >= abce\n";
   if (a <= b) std::cout << "okay: abcd <= abce\n";
}
