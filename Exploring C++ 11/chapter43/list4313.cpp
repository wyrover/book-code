// Listing 43-13. Erasing Elements from a Vector
#include <algorithm>
#include "data.hpp"
#include "intrange.hpp"

int main()
{
  intvector data{};
  read_data(data);
  data.erase(std::remove(data.begin(), data.end(), 0), data.end());
  intvector copy{};
  std::remove_copy_if(data.begin(), data.end(), std::back_inserter(copy),
                      [](int x) { return  x< 24 or x > 42; });
  std::sort(copy.begin(), copy.end());
  copy.erase(std::unique(copy.begin(), copy.end()), copy.end());
  write_data(copy);
}
