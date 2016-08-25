#include <iostream>

class UniqueID
{
protected:
    static int nextID;
public:
    int id;
    UniqueID();
    UniqueID(const UniqueID& orig);
    UniqueID& operator=(const UniqueID& orig);
};

int UniqueID::nextID = 0;

UniqueID::UniqueID()
{
    id = ++nextID;
}

UniqueID::UniqueID(const UniqueID& orig)
{
    id = orig.id;
}

UniqueID& UniqueID::operator=(const UniqueID& orig)
{
    id = orig.id;
    return (*this);
}

int main()
{
    UniqueID a;
    std::cout << a.id << std::endl;
    UniqueID b;
    std::cout << b.id << std::endl;
    UniqueID c;
    std::cout << c.id << std::endl;
}