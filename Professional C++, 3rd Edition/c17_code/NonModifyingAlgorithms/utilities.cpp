#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

int main()
{
    // all_of()
    vector<int> vec2 = { 1, 1, 1, 1 };

    if (all_of(cbegin(vec2), cend(vec2), [](int i) {
    return i == 1;
})) {
        cout << "All elements are == 1" << endl;
    }
    else {
        cout << "Not all elements are == 1" << endl;
    }

    // any_of()
    vector<int> vec3 = { 0, 0, 1, 0 };

    if (any_of(cbegin(vec3), cend(vec3), [](int i) {
    return i == 1;
})) {
        cout << "At least one element == 1" << endl;
    }
    else {
        cout << "No elements are == 1" << endl;
    }

    // none_of()
    vector<int> vec4 = { 0, 0, 0, 0 };

    if (none_of(cbegin(vec4), cend(vec4), [](int i) {
    return i == 1;
})) {
        cout << "All elements are != 1" << endl;
    }
    else {
        cout << "Some elements are == 1" << endl;
    }

    return 0;
}
