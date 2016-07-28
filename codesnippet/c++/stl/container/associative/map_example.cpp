#include <iostream>
#include <map>
#include <utility>
#include <vector>

class CustomizeClass
{
  public :
    CustomizeClass() { // must have the default constructor 
        std::cout << "call default constructor" << std::endl;
    } 
    CustomizeClass(const int& d) { 
        std::cout << "call copy constructor : " << d << std::endl;
    }
    CustomizeClass& operator=(const int& d) { 
        std::cout << "call assignment operator : " << d << std::endl;
        return *this;
    }
    ~CustomizeClass() {
        std::cout << "call destructor" << std::endl;
    }
}; 

void print(const std::pair<int, int> &p)
{
    std::cout << "key = " << p.first << " value = " << p.second << std::endl;
}

int main(int argc, const char *argv[])
{
    // typedef pair<const Key, T> value_type; from the value type, 
    // map is something like dictionary, which store a key and the value associated,
    // and it has the dictionary feature,which is ordered. the low level data structure
    // is red-black tree. we can add key-compare function as parameter into the Constructor
   
    // Constructor 
    std::map<int, int> imap; 
    std::map<std::string, int> smap;
    
    std::vector<std::pair<int, int> > ivec; 
    ivec.push_back(std::make_pair(1, 1));
    ivec.push_back(std::make_pair(3, 3));
    ivec.push_back(std::make_pair(2, 2));
    ivec.push_back(std::make_pair(4, 4));

    std::map<int, int> imap1(ivec.begin(), ivec.end());
    for_each(imap1.begin(), imap1.end(), print);

    std::map<int, int> imap2(imap1);
    
    //Iterator, we can use iterator to iterate the elements in map in order       
    typedef std::map<int, int>::iterator IMapIterator;
    for (IMapIterator iter = imap2.begin(); iter != imap2.end(); ++iter) {
        print(*iter);
    }

    // Capacity 
    if (imap1.empty()) {
        std::cout << "imap1 is not an empty map" << std::endl; 
    } else {
        std::cout << "size of imap1 = " << imap1.size() << std::endl;
    } 

    // Access
    // operator[] has the "add or update" meaning.
    // if the key is not exsit, then create one and construct the value use the default Constructor 
    // of the value(here the value type is not the value type of map) type
    // else return the value associated with the key, so we often use it by mistake
    // _Tp& operator[](const key_type& __k) {
    // iterator __i = lower_bound(__k);
    // // __i->first is greater than or equivalent to __k.
    // if (__i == end() || key_comp()(__k, (*__i).first))
    //   __i = insert(__i, value_type(__k, _Tp()));
    // return (*__i).second;
    // }

    imap1[3] = 33; // modify the value associated with key 3, because the key 3 exists in the map
    std::cout << imap1[5] << std::endl; // we expect to output 0, because the key 5 doesn't exist in the map
    std::cout << imap1[3] << std::endl; // we expect the output 33.
    
    // Item 24. Choose carefully between map::operator[] and map-insert when efficiency is important. [ effective stl ] 
    // If you're updating an existing map element, operator[] is preferable,
    // but if you're adding a new element, insert has the edge.     
    std::map<int, CustomizeClass> cmap;
    std::cout << "-------------------------" << std::endl;     
    cmap[1] = 1;  // 1. create new map entry and get the Iterator iter =  m.insert(std::pair(1, CustomizeClass()));
                  // 2. iter->second = 1 
                  // cost = defaut constructor + assignment operator
    std::cout << "-------------------------" << std::endl;     
    cmap.insert(std::pair<int, CustomizeClass>(2, 2));
    std::cout << "-------------------------" << std::endl;     
    // updating
    cmap[1] = 2;
    std::cout << "-------------------------" << std::endl;     
    cmap.insert(std::pair<int, CustomizeClass>(2, 1));
    std::cout << "-------------------------" << std::endl;     

    // insert a range
    
   
    // erase  

    // erase a specific value
    // erase a Iterator, key, a range
    cmap.erase(1);
    cmap.erase(cmap.begin());
    cmap.erase(cmap.begin(), cmap.end());
    
    // clear : remove all the elements in the map
    cmap.clear();

    // swap : exchange two map's content    
    std::map<int, CustomizeClass>().swap(cmap); // equal to clear

    // Operation
    // find 
    std::map<int, int>::iterator iter = imap1.find(3);
    if (iter != imap1.end()) {
        std::cout << "3 is the key of imap1" << std::endl;
        std::cout << "the value for key 3 is " << iter->second << std::endl;
    }

    // count equal to find for map, return 1 if exists, otherwise 0
    if (imap1.count(4) == 0) {
        std::cout << "4 is not the key of imap1" << std::endl;
    }

    // lower_bound, upper_bound
    // when we need know all the elements less than or greater than x, we can use these two functions

    // equal_range : return the iterator range which keys equal to key k, 
    // this functions should be more useful for multimap
    // return value is std::pair<Iterator, Iterator>, the first is the lower_bound, the second is the upper_bound
       
    std::pair<IMapIterator, IMapIterator> range = imap1.equal_range(2);    
    if (range.first->first == 2) {
       std::cout << "2 is the key of imap1" << std::endl; 
    }
        
    return 0;
}
