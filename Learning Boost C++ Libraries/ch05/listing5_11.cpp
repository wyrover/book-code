#include <boost/unordered_set.hpp>
#include <boost/functional/hash.hpp>
#include <iostream>
#include <cassert>
#include "Person.h" // struct Person definition

bool operator==(const Person& left, const Person& right)
{
    return (left.name == right.name
            && left.age == right.age
            && left.profession == right.profession
            && left.nationality == right.nationality);
}

namespace boost
{
template <>
struct hash<Person> {
    size_t operator()(const Person& person) const
    {
        size_t hash = 0;
        boost::hash_combine(hash,
                            boost::hash_value(person.name));
        boost::hash_combine(hash,
                            boost::hash_value(person.nationality));
        return hash;
    }
};
}

int main()
{
    boost::unordered_set<Person> persons;
    Person p{"Ned Land", 40, "Harpooner", "Canada"};
    persons.insert(p); // succeeds
    Person p1{"Ned Land", 32, "C++ Programmer", "Canada"};
    persons.insert(p1);  // succeeds
    assert(persons.find(p) != persons.end());
    assert(persons.find(p1) != persons.end());
    Person p2 = p;
    persons.insert(p2);   // fails
    assert(persons.size() == 2);
}

