#include <cstddef>
#include <vector>
#include <iostream>
#include <limits>
using namespace std;

int main()
{
    vector<double> doubleVector(10); // Create a vector of 10 doubles
    double max = -numeric_limits<double>::infinity(); // Initialize to smallest number

    for (size_t i = 0; i < doubleVector.size(); i++) {
        cout << "Enter score " << i + 1 << ": ";
        cin >> doubleVector[i];

        if (doubleVector[i] > max) {
            max = doubleVector[i];
        }
    }

    max /= 100.0;

    for (auto& element : doubleVector) {
        element /= max;
        cout << element << " ";
    }

    cout << endl;
    return 0;
}
