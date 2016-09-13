#include "BuddyList.h"
#include <iostream>
using namespace std;

int main()
{
    BuddyList buddies;
    buddies.addBuddy("Harry Potter", "Ron Weasley");
    buddies.addBuddy("Harry Potter", "Hermione Granger");
    buddies.addBuddy("Harry Potter", "Hagrid");
    buddies.addBuddy("Harry Potter", "Draco Malfoy");
    // That's not right! Remove Draco.
    buddies.removeBuddy("Harry Potter", "Draco Malfoy");
    buddies.addBuddy("Hagrid", "Harry Potter");
    buddies.addBuddy("Hagrid", "Ron Weasley");
    buddies.addBuddy("Hagrid", "Hermione Granger");
    auto harryBuds = buddies.getBuddies("Harry Potter");
    cout << "Harry's friends: " << endl;

    for (const auto& name : harryBuds) {
        cout << "\t" << name << endl;
    }

    return 0;
}
