#include <string>
#include <vector>
#include <boost/assign.hpp>
#include <cassert>

using namespace boost::assign;

int main()
{
    std::vector<std::string>greetings;
    greetings += "Good morning", "Buenos dias", "Bongiorno";
    greetings += "Boker tov", "Guten Morgen", "Bonjour";
    assert(greetings.size() == 6);
}

