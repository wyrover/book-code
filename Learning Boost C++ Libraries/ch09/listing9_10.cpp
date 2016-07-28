#include <iostream>
#include <boost/filesystem.hpp>
#include <exception>

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


void copyDirectory(const fs::path& src, const fs::path& target)
{
    if (!is_directory(src)
        || (exists(target) && !is_directory(target))
        || !is_directory(absolute(target).parent_path())
        || commonPrefix(src, target) == src) {
        throw std::runtime_error("Preconditions not satisfied");
    }

    boost::system::error_code ec;
    fs::path effectiveTarget = target;

    if (exists(target)) {
        effectiveTarget /= src.filename();
    }

    create_directory(effectiveTarget);
    fs::directory_iterator iter(src), end;

    while (iter != end) {
        auto status = iter->symlink_status();
        auto currentTarget = effectiveTarget /
                             iter->path().filename();

        if (status.type() == fs::regular_file) {
            copy_file(*iter, currentTarget,
                      fs::copy_option::overwrite_if_exists);
        } else if (status.type() == fs::symlink_file) {
            copy_symlink(*iter, currentTarget);
        } else if (status.type() == fs::directory_file) {
            copyDirectory(*iter, effectiveTarget);
        } // else do nothing

        ++iter;
    }
}

int main() {}
