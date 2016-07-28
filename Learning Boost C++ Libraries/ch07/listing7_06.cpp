#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

int main()
{
    std::string concat;
    char startCh = 'M';
    std::vector<std::string> names{"Meredith", "Guinnevere", "Mabel"
                                   , "Myrtle", "Germaine", "Gwynneth", "Mirabelle"};
    std::for_each(names.begin(), names.end(),
    [&concat, startCh](const std::string & name) {
        if (name.size() > 0 && name[0] == startCh) {
            concat += name + ", ";
        }
    });
    std::cout << concat << '\n';
}

