// Listing 10-5. Doubling and Squaring Input Values in a Vector by Using a Range-Based Loop
#include <iomanip>
#include <iostream>
#include <vector>

int main()
{
   std::vector<int> data{};
   int x{};

   while (std::cin >> x)
      data.push_back(x);

   std::cout.fill(' ');
   for (int element : data)
      std::cout << std::setw(2) << element <<
         std::setw(3) << element * 2 <<
         std::setw(4) << element * element << '\n';
}
