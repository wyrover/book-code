#include <iostream>
#include <map>
#include <string>

using namespace std;

class Person
{
    friend class PersonLessThan;
public:
    Person(const string& first, const string& last) :
        lastName_(last), firstName_(first) {}
    // ...
    string getFirstName() const
    {
        return (firstName_);
    }
    string getLastName() const
    {
        return (lastName_);
    }
private:
    string lastName_;
    string firstName_;
};

class PersonLessThan
{
public:
    bool operator()(const Person& per1,
                    const Person& per2) const
    {
        if (per1.lastName_ < per2.lastName_)          // Compare last
            return (true);                             // names, then
        else if (per1.lastName_ == per2.lastName_)    // first
            return (per1.firstName_ < per2.firstName_);
        else
            return (false);
    }
};

int main()
{
    map<Person, string, PersonLessThan> personMap;
    Person per1("Billy", "Silly"),
           per2("Johnny", "Goofball"),
           per3("Frank", "Stank"),
           per4("Albert", "Goofball");
    personMap[per1] = "cool";
    personMap[per2] = "not cool";
    personMap[per3] = "not cool";
    personMap[per4] = "cool";

    for (map<Person, string, PersonLessThan>::const_iterator p =
             personMap.begin(); p != personMap.end(); ++p) {
        cout << p->first.getFirstName() << " " << p->first.getLastName()
             << " is " << p->second << endl;
    }
}
