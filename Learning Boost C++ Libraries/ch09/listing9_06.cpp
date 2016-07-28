#include <boost/filesystem.hpp>
#include <iostream>
#include <exception>

namespace fs = boost::filesystem;

void printFileProperties(const fs::path&, int)
{}

void traverseRecursive(const fs::path& path)
{
    if (!exists(path) || !is_directory(path)) {
        return;
    }

    try {
        fs::recursive_directory_iterator it(path), end;

        while (it != end) {
            printFileProperties(*it, it.level());

            if (!is_symlink(it->path())
                && is_directory(it->path())
                && it->path().filename() == "foo") {
                it.no_push();
            }

            boost::system::error_code ec;
            it.increment(ec);

            if (ec) {
                std::cerr << "Skipping entry: "
                          << ec.message() << '\n';
            }
        }
    } catch (std::exception& e) {
        std::cout << "Exception caught: " << e.what() << '\n';
    }
}

int main()
{
}
