#include <windows.h>
#include <iostream>
using namespace std;

class Singleton
{
public:
    static Singleton* Instance()
    {
        if (!_instance) {
            _instance = new Singleton();
        }

        return _instance;
    }
private:
    Singleton()
    {
        cout << "Singleton..." << endl;
    }
    static Singleton* _instance;
};
Singleton* Singleton::_instance = nullptr;

int main()
{
    SetConsoleOutputCP(65001);
    Singleton *singleton1 = Singleton::Instance();
    cout << singleton1 << endl;
    Singleton *singleton2 = Singleton::Instance();
    cout << singleton2 << endl;
    delete Singleton::Instance();
    std::cout << std::endl;
    std::cout << "中文测试" << std::endl;
    system("pause");
    return 0;
}
