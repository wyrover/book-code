#include <string>

using namespace std;

class Foo
{
public:
    Foo() : counter_(0), str_(NULL) {}
    Foo(int c, string* p) :
        counter_(c), str_(p) {}
private:
    int counter_;
    string* str_;
};

int main()
{
    string s = "bar";
    Foo(2, &s);
}