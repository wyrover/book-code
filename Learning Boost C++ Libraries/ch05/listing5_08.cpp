#include <vector>
#include <cassert>

int main()
{
    std::vector<int>v{1, 2, 3, 5};
    auto first = v.begin();
    auto last = first + v.size() - 1;
    assert(*last == 5);
    v.insert(last, 4);

    // *last = 10;  // undefined behavior, invalid iterator
    for (int i = 0; i < 1000; ++i) {
        v.push_back(i);
    }

    // *first = 0; // likely invalidated
}

