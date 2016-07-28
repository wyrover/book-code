#include <thread>  
#include <iostream>  
class SayHello {  
  public:  
    void greeting() const  
    {  
      std::cout<<"hello"<<std::endl;  
    }  
};  
int main()  
{  
    SayHello x;  
    std::thread t(&SayHello::greeting,&x);  
    t.join();  

    return 0;
}  
