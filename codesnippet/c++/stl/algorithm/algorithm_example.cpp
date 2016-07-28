#include <iostream>
#include <algorithm>
#include <vector>

template<typename T>
class Double {
  public :
    void operator()(T & data) {
        data = data + data;
    } 
};

template<typename T>
inline void doubleFun(T& data) 
{
    data = data + data;
}

template<typename T>
inline void print(const T& data) 
{
    std::cout << data << " ";    
}


int main(int argc, const char *argv[])
{
    // Item 46. Consider function objects instead of functions as algorithm parameters. [ effective stl ] 
    // because we can define function operator as inline function, and the compiler is not optimize for functions even if you 
    // specify it as inline function
    int a[] = {1, 2, 3, 4};
    std::vector<int> ivec(a, a + sizeof(a)/sizeof(int));
    std::for_each(ivec.begin(), ivec.end(), Double<int>()); 
    std::for_each(ivec.begin(), ivec.end(), print<int>);   
    std::cout << std::endl;
    std::for_each(ivec.begin(), ivec.end(), doubleFun<int>); 
    std::for_each(ivec.begin(), ivec.end(), print<int>);   
    std::cout << std::endl;

    // liner search function, find and find_if      
    // for random iterator, find algorithm use loop expand to optimize       

    typedef std::vector<int>::iterator IVecInterator;
    IVecInterator iter = find(ivec.begin(), ivec.end(), 5); 
    if (iter == ivec.end()) {
        std::cout << "could not find 5 in the vector ivec" << std::endl;
    }

    // similar with assocative container, they both have count and find member function. there is also count algorithm
    // count, count_if 
    
    std::cout << "there is " << std::count(ivec.begin(), ivec.end(), 5) << " in the vector ivec" << std::endl;

    // search the first same or last same value in the two ranges 
    int b[] = {4, 3, 3, 5, 4}; 
    std::vector<int> ivec2(b, b + sizeof(b)/sizeof(int));
    // return the first element in range 2 which occur in the range 1, 
    // in this example, range1 is "4, 8, 12, 16", range 2 is "4, 3, 3, 5, 4"
    // so return 4
    IVecInterator firstIter = find_first_of(ivec.begin(), ivec.end(), ivec2.begin(), ivec2.end());
    if (firstIter != ivec.end()) {
        std::cout << *firstIter << std::endl;
    }

    IVecInterator secondIter = find_end(ivec.begin(), ivec.end(), ivec2.begin(), ivec2.end()); 
    if (secondIter != ivec.end()) {
        std::cout << *secondIter << std::endl;
    }

    // find subsequence algorithm
    // adjacent_find ,return the first duplicated element which are adjacent,otherwise return end
    IVecInterator adjacent = adjacent_find(ivec2.begin(), ivec2.end()); 
    if (adjacent != ivec2.end()) {
        std::cout << *adjacent << std::endl;
    }

    // search subsequence 
    int c[] = {3, 5, 4};
  
    std::vector<int> ivec3(c, c + sizeof(c)/sizeof(int)); 
    IVecInterator subsequence = search(ivec2.begin(), ivec2.end(), ivec3.begin(), ivec3.end()); 
    if (subsequence != ivec2.end()) {
        std::cout << *subsequence << std::endl;
    }

//    IVecInterator n = search_n(ivec3.begin(), ivec3.end(), 

    // binary search          
   
    // write only alogrithm  
    // fill_n(dest, cnt, val)

    // generate_n(dest, cnt, Gen)

    // copy(beg, end, dest)

    // transform(beg, end, dest, unaryOp)
    // transform(beg, end, beg2, dest, binaryOp)

    // merge(beg1, end1, beg2, end2, dest)
    // merge(beg1, end1, beg2, end2, dest, comp)

    // sort and partition algorithm    
    // partition, stable_partition
    // stable_partition(beg, end, unaryPred)
    // partition(beg, end, unaryPred)
  
   
    // sort, stable_sort, partitial_sort
    
    // nth_element  

    // general reorder algorithm
    // unique, reverse, rotate, remove, remove_if

    // remove_copy, unique_copy, rotate_copy, random_shuffle

    // permutation alogrithm
    // prev_permutation(beg, end), next_permutation(beg, end)
    int a5[] = {1, 2, 3, 4};
    std::vector<int> ivec5(a5, a5 + 4);

    // alogrithm for next_permutation :     
    std::next_permutation(ivec5.begin(), ivec5.end());
    std::for_each(ivec5.begin(), ivec5.end(), print<int>);
    std::cout << std::endl;
    std::next_permutation(ivec5.begin(), ivec5.end());
    std::for_each(ivec5.begin(), ivec5.end(), print<int>);
    std::cout << std::endl;


    // ordered set algorithm : set_union, set_intersection, set_difference, set_symmetric_difference

    // compare algorithm
    // min(val1, val2), max(val1, val2), min_element, max_element(beg, end)

    // math algorithm
    
    return 0;
}
