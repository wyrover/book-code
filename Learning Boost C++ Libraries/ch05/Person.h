#ifndef PERSON_H
#define PERSON_H
#include <string>
#include <tuple>

struct Person  {
    std::string name;
    int age;
    std::string profession;
    std::string nationality;
};

bool operator < (const Person& p1, const Person& p2)
{
    return std::tie(p1.nationality, p1.name, p1.age)
           < std::tie(p2.nationality, p2.name, p2.age);
}
#endif /* PERSON_H */

