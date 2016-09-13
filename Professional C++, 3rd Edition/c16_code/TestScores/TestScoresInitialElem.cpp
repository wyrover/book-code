#include <cstddef>
#include <vector>
#include <iostream>
#include <limits>
using namespace std;

int main()
{
    vector<double> doubleVector(10, 0.0); // creates vector of 10 doubles of value 0.0
    // Initialize max to smallest number
    double max = numeric_limits<double>::lowest();

    // No need to initialize each element: the constructor did it for you.

    for (size_t i = 0; i < 10; i++) {
        cout << "Enter score " << i + 1 << ": ";
        cin >> doubleVector[i];

        if (doubleVector[i] > max) {
            max = doubleVector[i];
        }
    }

    max /= 100.0;

    for (size_t i = 0; i < 10; i++) {
        doubleVector[i] /= max;
        cout << doubleVector[i] << " ";
    }

    cout << endl;
    return 0;
}
