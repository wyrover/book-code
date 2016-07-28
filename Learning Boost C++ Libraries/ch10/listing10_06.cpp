#define BOOST_THREAD_PROVIDES_FUTURE
#include <boost/thread.hpp>
#include <boost/thread/future.hpp>
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

std::vector<char> diffContent(const std::vector<char>&,
                              const std::vector<char>&)
{
    return std::vector<char>();
}

std::vector<char> diffFiles(const std::string& file1,
                            const std::string& file2)
{
    // set up the promise-future pair
    boost::promise<std::vector<char> > promised_value;
    boost::future<std::vector<char> > future_result
        = promised_value.get_future();
    // spawn a reader thread for file2
    boost::thread reader(
    [&promised_value, &file2]() {
        try {
            auto content = readFromFile(file2);
            promised_value.set_value(content);
        } catch (std::exception& e) {
            promised_value.set_exception(
                boost::copy_exception(e));
        }
    });
    // ...
    std::vector<char> content1, // initialized from file1
        content2; // initialized from file2
    std::vector<char> diff;

    try {
        auto content2 = future_result.get(); // this blocks
        diff = diffContent(content1, content2);
    } catch (std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << '\n';
    }

    reader.join();
    return diff;
}

int main()
{
}

