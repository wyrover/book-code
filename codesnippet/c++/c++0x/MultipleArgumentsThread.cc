#include <stdio.h>
#include <stdlib.h>
#include <iostream> 
#include <thread> 

void sum(const int& a, const int& b) {
    int s = a + b;
    std::cout << "the sum of " << a 
              <<  " and " << b << " is " 
              << s<< std::endl;
}

int main(int argc, const char *argv[])
{
    std::thread t(sum, 1, 2);
    t.join();

    return EXIT_SUCCESS;
}
