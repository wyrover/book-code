#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

void makeSymLink(const fs::path& target, const fs::path& link)
{
    boost::system::error_code ec;

    if (is_directory(target)) {
        create_directory_symlink(target, link);
    } else {
        create_symlink(target, link);
    }
}

int main()
{}
