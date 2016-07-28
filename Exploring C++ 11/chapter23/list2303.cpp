// Listing 23-3. Using a Lambda to Access Local Variables
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main()
{
   std::vector<int> data{ 1, 2, 3 };

   int multiplier{3};
   auto times = [multiplier](int i) { return i * multiplier; };

   std::transform(data.begin(), data.end(), data.begin(), times);

   multiplier = 20;
   std::transform(data.begin(), data.end(), data.begin(), times);

   std::copy(data.begin(), data.end(),
             std::ostream_iterator<int>(std::cout, "\n"));
}
