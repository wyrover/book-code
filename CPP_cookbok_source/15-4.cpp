#include <iostream>
#include <string>

class RecordSet
{
public:
    bool getFieldVal(int i, std::string& s) <userinput>const < / userinput >;
    // ...
};

bool RecordSet::getFieldVal(int i, std::string& s) <userinput>const < / userinput >
{
    // In here, you can't modify any nonmutable data
    // members (see discussion)
}

void displayRecords(const RecordSet& rs)
{
    // Here, you can only invoke const member functions
    // on rs
}