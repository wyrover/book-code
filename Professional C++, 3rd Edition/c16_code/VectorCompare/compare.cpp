#include <vector>
#include <iostream>
using namespace std;

int main()
{
    vector<int> vectorOne(10);
    vector<int> vectorTwo(10);

    if (vectorOne == vectorTwo) {
        cout << "equal!" << endl;
    } else {
        cout << "not equal!" << endl;
    }

    vectorOne[3] = 50;

    if (vectorOne < vectorTwo) {
        cout << "vectorOne is less than vectorTwo" << endl;
    } else {
        cout << "vectorOne is not less than vectorTwo" << endl;
    }

    return 0;
}
