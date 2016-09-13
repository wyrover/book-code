#include "AccessList.h"
using namespace std;

AccessList::AccessList(const initializer_list<string>& initlst)
{
    for (auto& user : initlst) {
        addUser(user);
    }
}

void AccessList::addUser(const string& user)
{
    mAllowed.insert(user);
}

void AccessList::removeUser(const string& user)
{
    mAllowed.erase(user);
}

bool AccessList::isAllowed(const string& user) const
{
    return (mAllowed.count(user) != 0);
}

list<string> AccessList::getAllUsers() const
{
    list<string> users;
    users.insert(end(users), begin(mAllowed), end(mAllowed));
    return users;
}
