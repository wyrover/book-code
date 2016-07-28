# C++0x中的多线程编程

## 并行和并发
并行具有同时的含义,并发则指在一段时间内同时发生。对于多核或者多处理器来说，可以同时运行多个任务,多个任务之间时并行关系。对于单CPU的分时系统来说，在一段时间内，多个任务交错进行，可以看成并发,并发还有不确定顺序的含义。

## 多线程编程
传统的多线程编程使用pthread library来进行。但是在c++0x中，在语言层面支持了多线程编程。

## 简单的例子

    #include <thread>  
    #include <iostream>  
 
    void my_thread_func()  
    {  
      std::cout<<"hello"<<std::endl;  
    }  
 
    int main()  
    {  
      std::thread t(my_thread_func);  
      t.join();  
    }  

如果没有t.join()这句，程序的行为时不确定的。因为main函数将可能在线程执行前或者执行的过程中退出了。如果要确保线程在某个点要完成，则必须调用join函数。如果没有调用join()函数或者显示的调用detach()，则当线程析构的时候会调用std::terminate()函数。

## 函数对象构造线程

    #include <thread>  
    #include <iostream>  
 
    class SayHello  
    {  
    public:  
      void operator()() cons  
      {  
        std::cout<<"hello"<<std::endl;  
      }  
    };  
 
    int main()  
    {  
      std::thread t((SayHello()));  
      t.join();  
    }  

如果只能通过void (*f)(void)来构造线程，将会非常不方便，因为如果要传递信息给线程，只能通过全局变量。和c++的标准模版库类似，也可通过函数对象构造线程。
  
  三种不同的写法:

      int main()  
      {  
        SayHello hello;  
        std::thread t(hello);  
        t.join();  
      }  

      int main()  
      {  
        std::thread t=std::thread(SayHello());  
        t.join();  
      }  

      int main()  
      {  
        std::thread t{SayHello()};  
        t.join();  
      }  

传递参数给函数对象。

    #include <thread>  
    #include <iostream>  
    #include <string>  
 
    class Greeting  
    {  
      std::string message;  
    public:  
      explicit Greeting(std::string const& message_):  
        message(message_)  
        {}  
        void operator()() const  
        {  
          std::cout<<message<<std::endl;  
        }  
    };  
 
    int main()  
    {  
      std::thread t(Greeting("goodbye"));  
      t.join();  
    }  

传递参数给函数

    #include <thread>  
    #include <iostream>  
    #include <string>  
    #include <functional>  
    void greeting(std::string const& message)  
    {  
      std::cout<<message<<std::endl;  
    }  
    int main()  
    {  
      std::thread t(std::bind(greeting,"hi!"));  
      t.join();  
    }  


    #include <thread>  
    #include <iostream>  
    #include <string>  
    void greeting(std::string const& message)  
    {  
      std::cout<<message<<std::endl;  
    }  
    int main()  
    {  
      std::thread t(greeting,"hi!");  
      t.join();  
    }  

也可以传递多个参数。

    #include <thread>  
    #include <iostream>  
    void write_sum(int x,int y)  
    {  
      std::cout<<x<<" + "<<y<<" =  
         "<<(x+y)<<std::endl;  
    }  
 
    int main()  
    {  
      std::thread t(write_sum,123,456);  
      t.join();  
    }  


也可以通过成员函数构造线程

    #include <thread>  
    #include <iostream>  
    class SayHello  
    {  
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
    }  

## 对共享数据的保护




