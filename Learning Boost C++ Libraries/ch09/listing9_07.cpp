#include <boost/filesystem.hpp>
#include <iostream>
#include <boost/date_time.hpp>
namespace fs = boost::filesystem;
namespace pxtm = boost::posix_time;

void printFileProperties(const fs::directory_entry& entry,
                         int indent = 0)
{
    const fs::path& path = entry.path();
    fs::file_status stat = entry.symlink_status();
    std::cout << std::string(2 * indent, ' ');

    try {
        if (is_symlink(path)) {
            auto origin = read_symlink(path);
            std::cout << " L " << " -  - "
                      << path.filename().string() << " -> "
                      << origin.string();
        } else if (is_regular_file(path)) {
            std::cout << " F " << " "
                      << file_size(path) << " " << " "
                      << pxtm::from_time_t(last_write_time(path))
                      << "  " << path.filename().string();
        } else if (is_directory(path)) {
            std::cout << " D " << " – " << " "
                      << pxtm::from_time_t(last_write_time(path))
                      << "  " << path.filename().string();
        } else {
            switch (stat.type()) {
            case fs::character_file:
                std::cout << " C ";
                break;

            case fs::block_file:
                std::cout << " B ";
                break;

            case fs::fifo_file:
                std::cout << " P ";
                break;

            case fs::socket_file:
                std::cout << " S ";
                break;

            default:
                std::cout << " - ";
                break;
            }

            std::cout << pxtm::from_time_t(last_write_time(path))
                      << "  ";
            std::cout << path.filename().string();
        }

        std::cout << '\n';
    } catch (std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << '\n';
    }
}

int main() {}
