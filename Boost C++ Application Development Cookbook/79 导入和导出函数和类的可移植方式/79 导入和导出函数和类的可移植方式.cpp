#include "my_library.hpp"
#include <cassert>

int main()
{
    assert(foo() == 0);
    bar b;

    try {
        b.meow();
        assert(false);
    } catch (const bar_exception&) {
    }

    system("pause");
    return 0;
}