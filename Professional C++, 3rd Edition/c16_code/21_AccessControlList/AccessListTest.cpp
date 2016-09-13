#include "AccessList.h"
#include <iostream>

using namespace std;

int main()
{
    AccessList fileX = { "pvw", "mgregoire", "baduser" };
    fileX.removeUser("baduser");

    if (fileX.isAllowed("mgregoire")) {
        cout << "mgregoire has permissions" << endl;
    }

    if (fileX.isAllowed("baduser")) {
        cout << "baduser has permissions" << endl;
    }

    auto users = fileX.getAllUsers();

    for (const auto& user : users) {
        cout << user << "  ";
    }

    cout << endl;
    return 0;
}
