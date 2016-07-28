#include <boost/phoenix/core.hpp>
#include <boost/phoenix/statement/if.hpp>
#include <boost/phoenix/operator.hpp>
#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
    namespace phx = boost::phoenix;
    using namespace phx;
    using phx::arg_names::arg1;
    std::vector<std::string> names{"Daltrey", "Townshend",
                                   "Entwistle", "Moon"};
    std::for_each(names.begin(), names.end(),
                  if_(arg1 == "Daltrey") [
                      std::cout << arg1 << ", vocalist" << '\n'
                  ].else_[
                      std::cout << arg1 << ", instrumentalist" << '\n'
                  ]
                 );
}

