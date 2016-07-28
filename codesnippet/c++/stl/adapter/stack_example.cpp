#include <iostream>
#include <deque>
#include <list>
#include <stack>
#include <vector>

int main(int argc, const char *argv[])
{

    /*
        stacks are a type of container adaptor, specifically designed to operate in a LIFO context (last-in first-out),
         where elements are inserted and extracted only from one end of the container.
        stacks are implemented as containers adaptors, which are classes that use an encapsulated object of a specific container 
        class as its underlying container, providing a specific set of member functions to access its elements. 
        Elements are pushed/popped from the "back" of the specific container, which is known as the top of the stack.

        The underlying container may be any of the standard container class templates or some other specifically designed container class. 
        The container shall support the following operations:
            empty
            size
            back
            push_back
            pop_back

        The standard container classes vector, deque and list fulfill these requirements. 
        By default, if no container class is specified for a particular stack class instantiation, the standard container deque is used.
    */
    std::deque<int> mydeque (3,100);          // deque with 3 elements
    std::vector<int> myvector (2,200);        // vector with 2 elements

    std::stack<int> first;                    // empty stack

    if (first.empty()) {
        std::cout << "first is a empty stack" << std::endl;
    }

    first.push(1);
    std::cout << "the top element of first stack is " << first.top() << std::endl;
    std::cout << "the size of first stack is " << first.size() << std::endl;
    first.pop();
    std::cout << "the size of first stack is " << first.size() << std::endl;
     
    std::stack<int> second (mydeque);         // stack initialized to copy of deque
    std::stack<int,std::vector<int> > third;  // empty stack using vector
    std::stack<int,std::vector<int> > fourth (myvector);

    std::cout << "size of first: " << first.size() << std::endl;
    std::cout << "size of second: " << second.size() << std::endl;
    std::cout << "size of third: " << third.size() << std::endl;
    std::cout << "size of fourth: " << fourth.size() << std::endl;    

    return 0;
}

