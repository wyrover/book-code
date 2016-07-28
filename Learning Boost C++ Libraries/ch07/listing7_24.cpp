#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

int main()
{
    using boost::phoenix::arg_names::arg1;
    std::vector<std::string> vec{"Lambda", "Iota",
                                 "Sigma", "Alpha"};
    std::for_each(vec.begin(), vec.end(),
                  std::cout << arg1 << '\n');
}

