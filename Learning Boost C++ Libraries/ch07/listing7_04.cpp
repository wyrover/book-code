#include <vector>
#include <algorithm>
#include <cassert>

int main()
{
    std::vector<int> vec{2, 4, 6, 8, 9, 1};
    auto it = std::find_if(vec.begin(), vec.end(),
                           [](const int& num) -> bool
    {  return num % 2 != 0; }
                          );
    assert(it != vec.end() && *it == 9);
}

