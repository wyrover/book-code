#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

// Function template to populate a container of ints.
// The container must support push_back().
template<typename Container>
void populateContainer(Container& cont)
{
    int num;

    while (true) {
        cout << "Enter a number (0 to quit): ";
        cin >> num;

        if (num == 0) {
            break;
        }

        cont.push_back(num);
    }
}

void print(int elem)
{
    cout << elem << " ";
}

int main()
{
    vector<int> myVector;
    populateContainer(myVector);
    cout << "The vector contents are:" << endl;
    for_each(myVector.begin(), myVector.end(), &print);
    cout << endl;
    transform(myVector.begin(), myVector.end(), myVector.begin(),
              bind2nd(plus<int>(), 100));
    cout << "The vector contents are:" << endl;
    for_each(myVector.begin(), myVector.end(), &print);
    cout << endl;
    return 0;
}
