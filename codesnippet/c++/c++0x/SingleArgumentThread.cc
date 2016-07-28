#include <iostream> 
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread> 

void greeting(const std::string& message) {
    std::cout << message << std::endl;
}

int main(int argc, const char *argv[])
{
    std::thread t(greeting, "hi");
    // std::thread t(std::bind(greeting, "hi"));
    t.join();

    return EXIT_SUCCESS;
}
