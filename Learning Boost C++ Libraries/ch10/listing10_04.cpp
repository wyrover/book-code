#include <vector>
#include <string>
#include <fstream>
#include <boost/filesystem.hpp>
#define BOOST_THREAD_PROVIDES_FUTURE
#include <boost/thread.hpp>
#include <boost/thread/future.hpp>

std::vector<char> readFromFile(const std::string& filepath)
{
    std::ifstream ifs(filepath);
    size_t length = boost::filesystem::file_size(filepath);
    std::vector<char> content(length);
    ifs.read(content.data(), length);
    return content;
}

std::vector<char> diffContent(const std::vector<char>& c1,
                              const std::vector<char>& c2)
{
    // stub - returns an empty vector
    return std::vector<char>();
}

std::vector<char> diffFiles(const std::string& file1,
                            const std::string& file2)
{
    // set up the promise-future pair
    boost::promise<std::vector<char>> promised_value;
    boost::future<std::vector<char>> future_result
                                  = promised_value.get_future();
    // spawn a reader thread for file2
    boost::thread reader(
    [&promised_value, &file2]() {
        std::cout << "Reading " << file2 << '\n';
        auto content = readFromFile(file2);
        promised_value.set_value(content);
        std::cout << "Read of " << file2
                  << " completed.\n";
    });
    std::cout << "Reading " << file1 << '\n';
    auto content1 = readFromFile(file1);
    std::cout << "Read of " << file1 << " completed.\n";
    auto content2 = future_result.get(); // this blocks
    auto diff = diffContent(content1, content2);
    reader.join();
    return diff;
}

int main()
{
}
