/*
    TODO : add licence
*/
#include <iostream>
#include <vector>

template<class T>
class PlusOne {
  public :
    PlusOne() {}
    void operator()(T& data) { data += 1; }
};

template<class T>
void print(const T& data) 
{
    std::cout << data << " ";
}

int main(int argc, const char *argv[])
{
    // Constructor 
    // default constructor
    // Though we can use many type containers, but we shuold avoid use vector<bool> 
    // Item 18. Avoid using vector<bool>.[ effective stl ]
    std::vector<int> ivec;
    // fill constructor 
    std::vector<float> fvec(10, 1.0);
    // range constructor
    // in the range constructor, the iterator just need be an InputIterator
    std::vector<float> fvec1(fvec.begin(), fvec.begin() + 5);
    float f[] = {1.0, 2.0, 3.0};
    std::vector<float> fvec2(f, f + sizeof (f) / sizeof (f[0]));
    // copy constructor
    std::vector<float> fvec3(fvec);

    // Iterator   
    std::vector<int>::iterator iter = ivec.begin();  
    const std::vector<int> civec(5, 5);
    std::vector<int>::const_iterator iter1 = civec.begin();
    // 
    typedef std::vector<float>::iterator FloatVecIterator;
    for (FloatVecIterator iter = fvec.begin(); iter != fvec.end(); ++iter) {
        std::cout << *iter << " ";
    }
    std::cout << std::endl;

    for_each(fvec.begin(), fvec.end(), PlusOne<float>());     
    for (FloatVecIterator iter = fvec.begin(); iter != fvec.end(); ++iter) {
        std::cout << *iter << " ";
    }
    std::cout << std::endl;

    for_each(fvec2.begin(), fvec2.end(), print<float>);
    std::cout << std::endl;

    // Capacity 
    // when the capacity can't hold the elements, it will double the current capacity
    // size is the number elements in container
    std::cout << "size of ivec = " << ivec.size() << std::endl;
    std::cout << "capacity of ivec = " << ivec.capacity() << std::endl; 
    std::cout << "max size of ivec = " << ivec.max_size() << std::endl; 
    ivec.push_back(0);
    std::cout << "size of ivec = " << ivec.size() << std::endl;
    std::cout << "capacity of ivec = " << ivec.capacity() << std::endl; 
    std::cout << "max size of ivec = " << ivec.max_size() << std::endl; 
    ivec.push_back(1);
    std::cout << "size of ivec = " << ivec.size() << std::endl;
    std::cout << "capacity of ivec = " << ivec.capacity() << std::endl; 
    std::cout << "max size of ivec = " << ivec.max_size() << std::endl; 
    ivec.push_back(2);
    std::cout << "size of ivec = " << ivec.size() << std::endl;
    std::cout << "capacity of ivec = " << ivec.capacity() << std::endl; 
    std::cout << "max size of ivec = " << ivec.max_size() << std::endl; 
    // if the new size is less then the current size, will truncate the extra elements,
    // else will add the default value of the value_type of container
    ivec.resize(2);
    std::cout << "size of ivec = " << ivec.size() << std::endl;
    std::cout << "capacity of ivec = " << ivec.capacity() << std::endl; 
    std::cout << "max size of ivec = " << ivec.max_size() << std::endl; 
    for_each(ivec.begin(), ivec.end(), print<int>);
    std::cout << std::endl;
    ivec.resize(5);
    std::cout << "size of ivec = " << ivec.size() << std::endl;
    std::cout << "capacity of ivec = " << ivec.capacity() << std::endl; 
    std::cout << "max size of ivec = " << ivec.max_size() << std::endl; 
    for_each(ivec.begin(), ivec.end(), print<int>);
    std::cout << std::endl;

    // due to the double capacity feature, if you know the max size of your container
    // you can use reserve to save memory
    std::vector<int> ivec1;
    // Item 14. Use reserve to avoid unnecessary reallocations.[ effective stl ]
    ivec1.reserve(100);
    std::cout << "size of ivec1 = " << ivec1.size() << std::endl;
    std::cout << "capacity of ivec1 = " << ivec1.capacity() << std::endl; 
    std::cout << "max size of ivec1 = " << ivec1.max_size() << std::endl; 

    // empty, we have many ways to judge if a container is empty 
    // Item 4. Call empty instead of checking size() against zero. [ effective stl ]
    // Why : because the time complexity of some implementation for list is O(N)
    std::vector<double> dvec;
    // i : call empty function
    if (dvec.empty()) {
        std::cout << "dvec is an empty container" << std::endl;
    }
    // ii : check the size 
    if (dvec.size() == 0) {
        std::cout << "dvec is an empty container" << std::endl;
    }
    // iii : check the iterator begin and end, infact, it's the code for empty function
    if (dvec.begin() == dvec.end()) {
        std::cout << "dvec is an empty container" << std::endl;
    }

    // Element Access
    // operator[] has the same meaning with at function, but at function check if the range is valid
    for (std::vector<int>::size_type i = 0; i < ivec.size(); ++i) {
        std::cout << ivec[i] << " ";
        ivec[i] = i;
    }
    std::cout << std::endl;
    for (std::vector<int>::size_type i = 0; i < ivec.size(); ++i) {
        std::cout << ivec.at(i) << " ";
    }
    std::cout << std::endl;
    // vector container also provide the function to access the first and last elements
    std::cout << ivec.front() << std::endl;
    std::cout << ivec.back() << std::endl;
 
    // Modifiers 
    // assign causes an automatic reallocation of the allocated storage space if and only if 
    // the new vector size surpassed the current capacity
    std::vector<int> ivec2;
    ivec2.assign(ivec.begin(), ivec.end());
    std::cout << "size of ivec2 = " << ivec2.size() << std::endl;
    std::cout << "capacity of ivec2 = " << ivec2.capacity() << std::endl;

    ivec2.assign(10, 10); 
    std::cout << "size of ivec2 = " << ivec2.size() << std::endl;
    std::cout << "capacity of ivec2 = " << ivec2.capacity() << std::endl;
    

    // add element at the end 
    // will cause an automatic reallocation of the allocated storage space if and only if
    // the new vector size surpassed the current capacity
    ivec2.push_back(11); // may cause reallocation
    std::cout << "size of ivec2 = " << ivec2.size() << std::endl;
    std::cout << "capacity of ivec2 = " << ivec2.capacity() << std::endl;
    ivec2.pop_back(); // only change the size
    std::cout << "size of ivec2 = " << ivec2.size() << std::endl;
    std::cout << "capacity of ivec2 = " << ivec2.capacity() << std::endl;
    
   
    // clear
    // Removes all elements from the vector (which are destroyed), 
    // leaving the container with a size of 0.
    // A reallocation is not guaranteed to happen, and the vector capacity 
    // is not guaranteed to change due to calling this function.
    // A typical alternative that forces a reallocation is to use swap: 
    // vector<T>().swap(x);   // clear x reallocating 
    ivec2.clear();
    std::cout << "size of ivec2 = " << ivec2.size() << std::endl;
    std::cout << "capacity of ivec2 = " << ivec2.capacity() << std::endl;

    // swap
    // Item 17. Use "the swap trick" to trim excess capacity. [ effective stl ]
    std::vector<int>(ivec2).swap(ivec2); // "shrink to fit"
    std::cout << "size of ivec2 = " << ivec2.size() << std::endl;
    std::cout << "capacity of ivec2 = " << ivec2.capacity() << std::endl;
    std::vector<int>().swap(ivec2);
    std::cout << "size of ivec2 = " << ivec2.size() << std::endl;
    std::cout << "capacity of ivec2 = " << ivec2.capacity() << std::endl;

    // erase
    // An iterator pointing to the new location of the element that followed the last element 
    // erased by the function call. This is the container endl  
    // if the operation erased the last element in the sequence.
    std::vector<int> ivec3(3, 3);    
    ivec3.erase(ivec3.begin()); 
    ivec3.erase(ivec3.begin()); 
    std::cout << "size of ivec3 = " << ivec3.size() << std::endl;
    std::cout << "capacity of ivec3 = " << ivec3.capacity() << std::endl;
    // if the iterator is end() or surpassed end(), which will cause segementation error
    // ivec3.erase(ivec3.end()); 
    // Iterators specifying a range within the vector] to be removed: [first,last). 
    std::cout << "size of ivec3 = " << ivec3.size() << std::endl;
    std::cout << "capacity of ivec3 = " << ivec3.capacity() << std::endl;
    ivec3.assign(5, 4);
    std::cout << "size of ivec3 = " << ivec3.size() << std::endl;
    std::cout << "capacity of ivec3 = " << ivec3.capacity() << std::endl;
    ivec3.erase(ivec3.begin(), ivec3.begin() + 3);
    std::cout << "size of ivec3 = " << ivec3.size() << std::endl;
    std::cout << "capacity of ivec3 = " << ivec3.capacity() << std::endl;

    // remove a specific value from a vector
    std::vector<int> ivec4;
    ivec4.reserve(10);
    ivec4.push_back(3);
    ivec4.push_back(4);
    ivec4.push_back(3);
    ivec4.push_back(2);
    ivec4.push_back(1);
    ivec4.push_back(3);
    std::cout << "size of ivec4 = " << ivec4.size() << std::endl;
    std::cout << "capacity of ivec4 = " << ivec4.capacity() << std::endl;
    // remove all the 3 from the vector 
    typedef std::vector<int>::iterator IntVecIter;
    
    for (IntVecIter iter = ivec4.begin(); iter != ivec4.end(); ) {
        if (*iter == 3) {
           iter = ivec4.erase(iter); 
        } else {
            ++iter;
        }
    }
    std::cout << "size of ivec4 = " << ivec4.size() << std::endl;
    std::cout << "capacity of ivec4 = " << ivec4.capacity() << std::endl;

    // tem 13. Prefer vector and string to dynamically allocated arrays. [ effective stl ]     
    // i. you must delete the allocated arrays and delete only once
    // ii. you must delete the array correctly use delete []
    
    // insert        
    // The vector is extended by inserting new elements before the element at the specified position, 
    // effectively increasing the container size by the number of elements inserted.
    // This causes an automatic reallocation of the allocated storage space 
    // if -and only if- the new vector size surpasses the current vector capacity.
    // insert may cause reallocation, so the iterator may invalid
  
    std::vector<int> ivec5(5, 5); 
    ivec5.insert(ivec5.begin(), 4, 4);
    ivec5.insert(ivec5.begin(), 3, 3);
    ivec5.insert(ivec5.begin(), 2, 2);
    ivec5.insert(ivec5.begin(), 1);
    for_each(ivec5.begin(), ivec5.end(), print<int>);
    std::cout << std::endl;
    int a1[] = {6, 6, 6, 6, 6, 6, 6}; 
    ivec5.insert(ivec5.end(), a1, a1 + sizeof(a1) / sizeof (a1[0]));
    for_each(ivec5.begin(), ivec5.end(), print<int>);
    std::cout << std::endl;

    // Item 5. Prefer range member functions to their single-element counterparts. [ effective stl ]   
    // we insert 5 zeros into the half of ivec5
    std::vector<int> ivec6(5, 0);
    IntVecIter insertLoc = ivec5.begin() + ivec5.size() / 2;
    ivec5.insert(insertLoc, ivec6.begin(), ivec6.end()); 
    for_each(ivec5.begin(), ivec5.end(), print<int>);
    std::cout << std::endl;
    /*   
    for (int i = 0; i < 5; ++i) {
       insertLoc = ivec5.insert(insertLoc, 0);
    }
    */

    return 0;
}
