#include <boost/container/slist.hpp>
#include <string>
#include <iostream>

typedef boost::container::slist<std::string> list_type;
typedef list_type::iterator iter_type;

int main()
{
    list_type dinos;
    iter_type last = dinos.before_begin();
    const char *dinoarray[] = {"Elasmosaurus", "Fabrosaurus",
                               "Galimimus", "Hadrosaurus", "Iguanodon",
                               "Appatosaurus", "Brachiosaurus",
                               "Corythosaurus", "Dilophosaurus"
                              };

    // fill the slist
    for (const char *dino : dinoarray) {
        dinos.insert_after(last, dino);
        ++last;
    }

    // find the pivot
    last = dinos.begin();
    iter_type iter = last;

    while (++iter != dinos.end()) {
        if (*last > *iter) {
            break;
        }

        ++last;
    }

    // find the end of the tail
    auto itend = last;

    while (iter != dinos.end()) {
        ++itend;
        ++iter;
    }

    // splice after
    dinos.splice_after(dinos.before_begin(), dinos,
                       last, itend);

    for (const auto& str : dinos) {
        std::cout << str << '\n';
    }
}

