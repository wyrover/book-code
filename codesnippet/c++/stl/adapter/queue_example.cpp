#include <iostream>
#include <deque>
#include <list>
#include <queue>
#include <vector>

int main(int argc, const char *argv[])
{

    /*
        ueues are a type of container adaptor, specifically designed to operate in a FIFO context (first-in first-out),
         where elements are inserted into one end of the container and extracted from the other.
        queues are implemented as containers adaptors, which are classes that use an encapsulated object of a specific container 
        class as its underlying container, providing a specific set of member functions to access its elements. 
        Elements are pushed into the "back" of the specific container and popped from its "front".

        The underlying container may be one of the standard container class template or some other specifically designed container class.
        This underlying container shall support at least the following operations:
                empty
                size
                front
                back
                push_back
                pop_front

        The standard container classes deque and list fulfill these requirements. By default, 
        if no container class is specified for a particular queue class instantiation, the standard container deque is used.
    */

    std::deque<int> mydeck(3,100);        // deque with 3 elements

    std::queue<int> first;                // empty queue
    std::queue<int> second(mydeck);       // queue initialized to copy of deque

    int a[] = {1, 2, 3, 4, 5, 6};
    std::list<int> mylist(a, a + sizeof(a)/sizeof(a[0]));
    std::queue<int, std::list<int> > third(mylist);

    std::cout << "the first elements of third queue : " << third.front() << std::endl;
    std::cout << "the last elements of third queue : " << third.back() << std::endl;
    third.pop();
    std::cout << "the first elements of third queue : " << third.front() << std::endl;
    std::cout << "size of first: " << first.size() << std::endl;
    std::cout << "size of second: " << second.size() << std::endl;
    std::cout << "size of third: " << third.size() << std::endl;

    return 0;
}
