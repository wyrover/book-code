#include <boost/filesystem.hpp>
#include <iostream>
#include <algorithm>
namespace fs = boost::filesystem;

void traverse(const fs::path& dirpath)
{
    if (!exists(dirpath) || ! is_directory(dirpath)) {
        return;
    }

    fs::directory_iterator dirit(dirpath), end;
    std::for_each(dirit, end, [](const fs::directory_entry & entry) {
        std::cout << entry.path().string() << '\n';
    });
}

int main(int argc, char *argv[1])
{
    if (argc > 1) {
        traverse(argv[1]);
    }
}

