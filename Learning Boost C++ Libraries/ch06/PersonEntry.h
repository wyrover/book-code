#ifndef PERSONENTRY_H
#define PERSONENTRY_H

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

bool operator==(const PersonEntry& left,
                const PersonEntry& right)
{
    return left.name == right.name
           && left.city == right.city;
}


#endif /* PERSONENTRY_H */


