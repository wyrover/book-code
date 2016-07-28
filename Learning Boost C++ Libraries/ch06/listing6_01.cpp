#include <set>
#include <iostream>
#include <string>

struct PersonEntry {
    std::string name;
    std::string phoneNumber;
    std::string city;
};

bool operator<(const PersonEntry& left,
               const PersonEntry& right)
{
    return left.name < right.name;
}


int main()
{
    std::multiset<PersonEntry> directory;
    PersonEntry p1{"Arindam Mukherjee", "550 888 9999", "Pune"};
    PersonEntry p2{"Arindam Mukherjee", "990 770 2458",
                   "Calcutta"};
    directory.insert(p1);
    directory.insert(p2);
    auto it1 = directory.lower_bound(
                   PersonEntry{ "Arindam Mukherjee", "", "" });
    auto it2 = directory.upper_bound(
                   PersonEntry{ "Arindam Mukherjee", "", "" });

    while (it1 != it2) {
        std::cout << "Found: [" << it1->name << ", "
                  << it1->phoneNumber << ", " << it1->city << "]\n";
        ++it1;
    }
}

