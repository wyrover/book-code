#include <iostream>
#include <forward_list>

using namespace std;

int main()
{
    // Create 3 forward lists and use an initializer_list
    // to initialize their elements (uniform initialization).
    forward_list<int> lst1 = { 5, 6 };
    forward_list<int> lst2 = { 1, 2, 3, 4 };
    forward_list<int> lst3 = { 7, 8, 9 };
    // Insert lst2 at the front of lst1 using splice.
    lst1.splice_after(lst1.before_begin(), lst2);
    // Add number 0 at the beginning of the lst1.
    lst1.push_front(0);
    // Insert lst3 at the end of lst1.
    // For this, we first need an iterator to the last element.
    auto iter = lst1.before_begin();
    auto iterTemp = iter;

    while (++iterTemp != end(lst1)) {
        ++iter;
    }

    lst1.insert_after(iter, cbegin(lst3), cend(lst3));

    // Output the contents of lst1.
    for (auto& i : lst1) {
        cout << i << ' ';
    }

    return 0;
}
