// Listing 23-2. Using a Lambda to Access Local Variables
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main()
{
   std::vector<int> data{};

   std::cout << "Multiplier: ";
   int multiplier{};
   std::cin >> multiplier;
   std::cout << "Data:\n";
   std::copy(std::istream_iterator<int>(std::cin),
      std::istream_iterator<int>(),
      std::back_inserter(data));

   std::transform(data.begin(), data.end(), data.begin(),
      [multiplier](int i){ return i * multiplier; });

   std::copy(data.begin(), data.end(),
      std::ostream_iterator<int>(std::cout, "\n"));
}
