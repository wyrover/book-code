#pragma once

#include <set>
#include <string>
#include <list>
#include <initializer_list>

class AccessList
{
public:
    // Default constructor
    AccessList() = default;

    // Constructor to support uniform initialization.
    AccessList(const std::initializer_list<std::string>& initlst);

    // Adds the user to the permissions list.
    void addUser(const std::string& user);

    // Removes the user from the permissions list.
    void removeUser(const std::string& user);

    // Returns true if the user is in the permissions list.
    bool isAllowed(const std::string& user) const;

    // Returns a list of all the users who have permissions.
    std::list<std::string> getAllUsers() const;

private:
    std::set<std::string> mAllowed;
};
