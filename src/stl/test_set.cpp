#include <iostream>
#include <set>
#include <signal.h>

using namespace std;

int main(void)
{
    std::set<int> _signals;
    _signals.insert(SIGTERM);
    _signals.insert(SIGINT);
    _signals.insert(SIGCHLD);

    for (std::set<int>::const_iterator i = _signals.begin(); i != _signals.end(); ++i)
        cout << *i << endl;

    _signals.clear();
    return 0;
}