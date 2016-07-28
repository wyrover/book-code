#include <fstream>
#include <memory>
#include <boost/timer/timer.hpp>
#include <string>
#include <boost/filesystem.hpp>
using std::ios;

std::unique_ptr<char[]> readFile(const std::string& file_name,
                                 std::streampos& size)
{
    std::unique_ptr<char[]> buffer;
    std::ifstream file(file_name, ios::binary);

    if (file) {
        size = boost::filesystem::file_size(file_name);

        if (size > 0) {
            buffer.reset(new char[size]);
            boost::timer::cpu_timer timer;
            file.read(buffer.get(), size);
            timer.stop();
            std::cerr << "file size = " << size
                      << ": time = " << timer.format();
        }
    }

    return buffer;
}

int main()
{
}
