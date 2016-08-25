#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

double doubleRand()
{
    return double(rand()) / (double(RAND_MAX) + 1.0);
}

int main()
{
    srand(static_cast<unsigned int>(clock()));
    cout << "expect 5 numbers within the interval [0.0, 1.0)" << endl;

    for (int i = 0; i < 5; i++) {
        cout << doubleRand() << "\n";
    }

    cout << endl;
}