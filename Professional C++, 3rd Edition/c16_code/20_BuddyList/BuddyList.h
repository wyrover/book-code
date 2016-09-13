#pragma once

#include <map>
#include <string>
#include <list>

class BuddyList
{
public:
    // Adds buddy as a friend of name.
    void addBuddy(const std::string& name, const std::string& buddy);

    // Removes buddy as a friend of name
    void removeBuddy(const std::string& name, const std::string& buddy);

    // Returns true if buddy is a friend of name, false otherwise.
    bool isBuddy(const std::string& name, const std::string& buddy) const;

    // Retrieves a list of all the friends of name.
    std::list<std::string> getBuddies(const std::string& name) const;

private:
    std::multimap<std::string, std::string> mBuddies;
};
