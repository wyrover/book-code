#include <iostream>
#include <string>
#include <set>


int main(int argc, const char *argv[])
{
    // the low level data structure is also red-black tree
    // so the interface set is very similar with map
    // Constructor
    int a[] = {1, 2, 3, 3}; 
    std::set<int> iset;
    std::set<int> iset1(a, a + sizeof(a)/sizeof(a[0]));
    std::set<int> iset2(iset1);
    std::set<std::string> sset;
   
    // iterator, begin(), end(), rbegin(), rend()
    typedef std::set<int>::iterator ISetIterator; 

    for (ISetIterator iter = iset2.begin(); iter != iset2.end(); ++iter) {
        std::cout << *iter << std::endl;
    }

    // Capacity, empty, size
    if (iset.empty()) {
        std::cout << "iset is empty" << std::endl;
    }
    std::cout << "size of set1 = " << iset1.size() << std::endl; 

    // Modifiers, insert, erase, swap, clear 
    std::pair<ISetIterator, bool> result = iset2.insert(2); 
    if (result.second == false) {
        std::cout << "2 is already in the set2" << std::endl;
    }

    // Operations, find, count, lower_bound, upper_bound, equal_range
    if (iset2.find(3) != iset2.end()) {
        std::cout << "3 is in the set2" << std::endl;
    }

    if (iset2.count(4) == 0) {
        std::cout << "4 is not in the set2" << std::endl;
    }

    iset2.erase(3); // erase a range
    if (iset2.find(3) == iset2.end()) {
        std::cout << "3 is not in the set2" << std::endl;
    }

    iset2.clear();  
    std::set<int>().swap(iset2); 
    // lower_bound, upper_bound, equal_range is similar with map 
        
    return 0;
}


