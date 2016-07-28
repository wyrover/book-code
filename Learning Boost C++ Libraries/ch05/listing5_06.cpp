#include <boost/container/slist.hpp>
#include <iostream>
#include <string>
#include <cassert>

int main()
{
    boost::container::slist<std::string> eras;
    boost::container::slist<std::string>::iterator last =
        eras.before_begin();
    const char *era_names [] = {"Cambrian", "Ordovician",
                                "Silurian", "Devonian", "Carboniferous",
                                "Permian", "Triassic", "Jurassic",
                                "Cretaceous", "Paleogene", "Neogene"
                               };

    for (const char *period : era_names) {
        eras.emplace_after(last, period);
        ++last;
    }

    int i = 0;

    for (const auto& str : eras) {
        assert(str == era_names[i++]);
    }
}

