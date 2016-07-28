#include <boost/filesystem.hpp>
#include <iostream>

namespace fs = boost::filesystem;

int main()
{
    // Get the current working directory
    fs::path cwd = fs::current_path();
    // Print the path to stdout
    std::cout << "generic: " << cwd.generic_string() << '\n';
    std::cout << "native: " << cwd.string() << '\n';
    std::cout << "quoted: " << cwd << '\n';
    std::cout << "Components: \n";

    for (const auto& dir : cwd) {
        std::cout << '[' << dir.string() << ']'; // each part
    }

    std::cout << '\n';
}

