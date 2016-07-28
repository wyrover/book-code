#include <future>
#include <thread>
#include <vector>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>

std::vector<char> readFromFile(const std::string& filepath)
{
    std::ifstream ifs(filepath);
    size_t length = boost::filesystem::file_size(filepath);
    std::vector<char> content(length);
    ifs.read(content.data(), length);
    return content;
}

std::vector<char> diffContent(const std::vector<char>& file1,
                              const std::vector<char>& file2)
{
    return std::vector<char>();
}

std::vector<char> diffFiles(const std::string& file1,
                            const std::string file2)
{
    typedef std::vector<char> buffer_t;
    std::packaged_task<buffer_t(const std::string&)>
    readerTask(readFromFile);
    auto future = readerTask.get_future();

    try {
        std::thread thread2(std::move(readerTask), file2);
        auto content1 = readFromFile(file1);
        std::cout << "Read from file " << file1 << " completed.\n";
        auto content2 = future.get();
        thread2.detach();
        return diffContent(content1, content2);
    } catch (std::exception& e) {
        std::cout << "Exception caught: " << e.what() << '\n';
    }

    return std::vector<char>();
}

int main()
{ }
