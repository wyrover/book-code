#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
using namespace std;

template<typename T>
void printVector(const vector<T>& v)
{
    for (auto& element : v) {
        cout << element << " ";
    }

    cout << endl;
}

int main()
{
    vector<int> vectorOne = { 1, 2, 3, 5 };
    vector<int> vectorTwo;
    // Oops, we forgot to add 4. Insert it in the correct place
    vectorOne.insert(cbegin(vectorOne) + 3, 4);

    // Add elements 6 through 10 to vectorTwo
    for (int i = 6; i <= 10; i++) {
        vectorTwo.push_back(i);
    }

    printVector(vectorOne);
    printVector(vectorTwo);
    // Add all the elements from vectorTwo to the end of vectorOne
    vectorOne.insert(cend(vectorOne), cbegin(vectorTwo), cend(vectorTwo));
    printVector(vectorOne);
    // Now erase the numbers 2 through 5 in vectorOne
    vectorOne.erase(cbegin(vectorOne) + 1, cbegin(vectorOne) + 5);
    printVector(vectorOne);
    // Clear vectorTwo entirely
    vectorTwo.clear();
    // And add 10 copies of the value 100
    vectorTwo.insert(cbegin(vectorTwo), 10, 100);
    // Decide we only want 9 elements
    vectorTwo.pop_back();
    printVector(vectorTwo);
    return 0;
}
