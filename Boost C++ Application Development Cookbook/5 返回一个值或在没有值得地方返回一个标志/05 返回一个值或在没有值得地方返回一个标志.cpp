#include<boost/optional.hpp>
#include<iostream>
#include<stdlib.h>
#include <time.h>

class locked_device
{
    explicit locked_device(const char* /*param*/)
    {
        //我们拥有对设备的独占式访问
        std::cout << "Device is locked\n";//设备被锁定
    }
public:
    ~locked_device()
    {
        //释放设备锁
    }
    void use()
    {
        std::cout << "Success!\n";//成功
    }
    static boost::optional<locked_device> try_lock_device()
    {
        if (rand() % 2) {
            //未能锁定设备
            return boost::none;
        }

        //成功！
        return locked_device("device name");//设备名
    }
};

int main()
{
    //Boost有一个名为Random的库。如果你想知道为什么
    //当stdlib.h中有rand()函数时还要编写它，参见90
    srand((unsigned)time(NULL));

    for (unsigned i = 0; i < 10; ++i) {
        boost::optional<locked_device> t = locked_device::try_lock_device();

        //optional能够转化为bool类型
        if (t) {
            t->use();
            system("pause");
            return 0;
        } else {
            std::cout << "...trying again\n";//重试
        }
    }

    std::cout << "Failure!\n";//失败
    system("pause");
    return -1;
}