#include <iostream>
#include <list>
template<class T>
void print(const T& data)
{
    std::cout << data << " ";
}

bool isEvenNumber(const int& data)
{
    return data % 2 ? false : true;
}

bool isNear(const int& a, const int& b)
{
    return abs(a-b) <= 1 ? true : false;
}

int main(int argc, const char *argv[])
{
    // most operation will not invalid the iterator except the one which is operated
    std::list<int> l1;
    std::list<int> l2(10, 5);
    int a[] = {1, 2, 3};
    std::list<int> l3(a, a + 3);
    std::list<int> l4(l3);

    if (l1.empty()) {
        std::cout << "list1 is an empty list" << std::endl;
    }
    std::cout << "size of list1 = " << l1.size() << std::endl;
    std::cout << "size of list2 = " << l2.size() << std::endl;
    std::cout << "size of list3 = " << l3.size() << std::endl;
    std::cout << "size of list4 = " << l4.size() << std::endl;

    typedef std::list<int>::iterator IntListIter;
    
    for (IntListIter iter = l3.begin(); iter != l3.end(); ++iter) {
        std::cout << *iter << " ";
    }
    std::cout << std::endl;
    
    std::cout << "first element of list4 = " << l4.front() << std::endl; 
    std::cout << "last element of list4 = " << l4.back() << std::endl; 


    // front, back, push_front, pop_front, push_back, pop_back


    // Operation
    // reverse the list
    l4.reverse();
    for_each(l4.begin(), l4.end(), print<int>);
    std::cout << std::endl; 

    // remove specific value
    l4.remove(5);
    for_each(l4.begin(), l4.end(), print<int>);
    std::cout << std::endl; 

    // remove the value which satisfy some condition
   
    l4.remove_if(isEvenNumber);
    for_each(l4.begin(), l4.end(), print<int>);
    std::cout << std::endl; 

    // sort the list and use the customize compare function
    l4.sort(); 
    for_each(l4.begin(), l4.end(), print<int>);
    std::cout << std::endl; 
    
    l4.push_front(1);
    l4.push_back(3);
    l4.push_back(4);
    for_each(l4.begin(), l4.end(), print<int>);
    std::cout << std::endl; 

    // uniq which will remove the element if the adjacent elements equal or satisfy some condition
    l4.unique(); 
    for_each(l4.begin(), l4.end(), print<int>);
    std::cout << std::endl; 
    l4.unique(isNear);
    for_each(l4.begin(), l4.end(), print<int>);
    std::cout << std::endl; 

    // merge the sorted lists
    std::list<int> l5(2, 2); 
    for_each(l5.begin(), l5.end(), print<int>);
    std::cout << std::endl; 

    l4.merge(l5); // after merged, the l5 will be an empty list
    for_each(l4.begin(), l4.end(), print<int>);
    std::cout << std::endl; 

    // splice interface which can used to merge two unordered list into one list
    // transfer the element of one list 
    l4.splice(l4.begin(), l2); 
    for_each(l4.begin(), l4.end(), print<int>);
    std::cout << std::endl; 
    // l4.splice(l4.begin(), l2, l2.begin());
    for_each(l4.begin(), l4.end(), print<int>);
    std::cout << std::endl; 
    //l4.splice(l4.begin(), l3, l3.begin(), l3.end());
    for_each(l4.begin(), l4.end(), print<int>);
    std::cout << std::endl; 

    return 0;
}
