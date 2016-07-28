#include <iostream> 
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread> 

class SayHello {
  public:
    SayHello(const std::string& msg):message(msg){ };
    void operator()() { std::cout << message << std::endl; } 
  private:
    std::string message;
};

int main(int argc, const char *argv[])
{
    std::thread t(SayHello("hello"));
    // std::thread t(std::bind(greeting, "hi"));
    t.join();
    return EXIT_SUCCESS;
}
