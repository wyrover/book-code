#include <iostream>
#include <array>

using namespace std;

int main()
{
    // Create array of 3 integers and initialize them
    // with the given initializer_list using uniform initialization.
    array<int, 3> arr = { 9, 8, 7 };
    // Output the size of the array.
    cout << "Array size = " << arr.size() << endl;

    // Output the contents using the range-based for loop.
    for (const auto& i : arr) {
        cout << i << endl;
    }

    cout << "Performing arr.fill(3)..." << endl;
    // Use the fill method to change the contents of the array.
    arr.fill(3);

    // Output the contents of the array using iterators.
    for (auto iter = cbegin(arr); iter != cend(arr); ++iter) {
        cout << *iter << endl;
    }

    return 0;
}
