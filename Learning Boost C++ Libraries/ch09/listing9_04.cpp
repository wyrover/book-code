#include <boost/filesystem.hpp>
#include <iostream>
namespace fs = boost::filesystem;

fs::path commonPrefix(const fs::path& first,
                      const fs::path& second)
{
    auto prefix =
    [](const fs::path & p1, const fs::path & p2) {
        auto result =
            std::mismatch(p1.begin(), p1.end(), p2.begin());
        fs::path ret;
        std::for_each(p2.begin(), result.second,
        [&ret](const fs::path & p) {
            ret /= p;
        });
        return ret;
    };
    size_t n1 = std::distance(first.begin(), first.end());
    size_t n2 = std::distance(second.begin(), second.end());
    return (n1 < n2) ? prefix(first, second)
           : prefix(second, first);
}

int main()
{
}
