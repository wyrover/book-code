#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/assign.hpp>
#include <boost/function_output_iterator.hpp>

struct StringCat {
    StringCat(std::string& str) : result_(str) {}

    void operator()(const std::string& arg)
    {
        if (arg.find_first_of(" \t") != std::string::npos) {
            result_ += " \"" + arg + "\"";
        } else {
            result_ += " " + arg;
        }
    }

    std::string& result_;
};

int main()
{
    std::vector<std::string> dirs{"photos", "videos",
                                  "books", "personal docs"};
    std::string dirString = "";
    std::copy(dirs.begin(), dirs.end(),
              boost::make_function_output_iterator(
                  StringCat(dirString)));
    std::cout << dirString << '\n';
}

