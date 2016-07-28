#include <iostream> 
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread> 

class PrintThis {
  public:
    void operator()() { std::cout << "this = "<< this << std::endl; } 
};

int main(int argc, const char *argv[])
{
    PrintThis pt;
    pt(); 

    std::thread t1(std::ref(pt));
    t1.join(); 
    std::thread t2(pt);
    t2.join();
    
    return EXIT_SUCCESS;
}
