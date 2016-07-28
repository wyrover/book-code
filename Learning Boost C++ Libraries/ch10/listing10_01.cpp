#include <iostream>
#include <string>
#include <vector>

int main()
{
    typedef std::vector<std::string> strvec;
    strvec angloSaxon{"Guten Morgen!", "Godmorgen!",
                      "Good morning!", "goedemorgen"};
    strvec romance{"Buenos dias!", "Bonjour!",
                   "Bom dia!", "Buongiorno!"};
    size_t max1 = angloSaxon.size(), max2 = romance.size();
    size_t i = 0, j = 0;

    while (i < max1 || j < max2) {
        if (i < max1)
            std::cout << angloSaxon[i++] << '\n';

        if (j < max2)
            std::cout << romance[j++] << '\n';
    }
}

