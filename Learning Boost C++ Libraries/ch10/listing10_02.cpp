#include <boost/thread.hpp>
#include <string>
#include <vector>
#include <iostream>

typedef std::vector<std::string> strvec;

void printGreets(const strvec& greets)
{
    for (const auto& greet : greets) {
        std::cout << greet << '\n';
    }
}

int main()
{
    strvec angloSaxon{"Guten Morgen!", "Godmorgen!",
                      "Good morning!", "goedemorgen"};
    strvec romance{"Buenos dias!", "Bonjour!",
                   "Bom dia!", "Buongiorno!"};
    boost::thread t1(printGreets, romance);
    printGreets(angloSaxon);
    t1.join();
}

