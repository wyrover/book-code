#include <boost/algorithm/string_regex.hpp>
#include <boost/regex.hpp>
#include <string>
#include <iostream>
#include <vector>

int main()
{
    std::string line = "All that you touch";
    std::vector<std::string> words;
    boost::find_all_regex(words, line, boost::regex("\\w+"));
    std::string record = "Pigs on the Wing|Dogs| Pigs| Sheep";
    std::vector<std::string> fields;
    boost::split_regex(fields, record, boost::regex("[\\|]"));

    for (auto word : words) {
        std::cout << word << ",";
    }

    std::cout << '\n';

    for (auto field : fields) {
        std::cout << field << ",";
    }
}

