// Listing 3-2. Print Average, Testing for a Zero Count
/// Read integers and print their average.
/// Print nothing if the input is empty.

#include <iostream>

int main()
{
   int sum{0};
   int count{};
   int x;
   while (std::cin >> x)
   {
       sum = sum + x;
       count = count + 1;
   }
 
   if (count != 0)
       std::cout << "average = " << sum / count << '\n';
}
