#include <iostream>

class Session {};

class SessionFactory
{

public:
    Session Create();
    Session* CreatePtr();
    void Create(Session*& p);
    // ...
};

// Return a copy of a stack object
Session SessionFactory::Create()
{
    Session s;
    return (s);
}

// Return a pointer to a heap object
Session* SessionFactory::CreatePtr()
{
    return (new Session());
}

// Update the caller's pointer with the address
// of a new object
void SessionFactory::Create(Session*& p)
{
    p = new Session();
}

static SessionFactory f; // The one factory object

int main()
{
    Session* p1;
    Session* p2 = new Session();
    *p2 = f.Create();    // Just assign the object returned from Create
    p1 = f.CreatePtr();  // or return a pointer to a heap object
    f.Create(p1);       // or update the pointer with the new address
}