#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
typedef std::vector<std::string> NameVec;

NameVec getNamesShorterThan(const NameVec& names,
                            size_t maxSize)
{
    NameVec shortNames;
    std::copy_if(names.begin(), names.end(),
                 std::back_inserter(shortNames),
    [maxSize](const std::string & name) {
        return name.size() <= maxSize;
    }
                );
    return shortNames;
}

int main()
{}
