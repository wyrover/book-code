#include <vector>
#include <memory>
using namespace std;

class Element
{
public:
    Element() {}
    virtual ~Element() {}
};

int main()
{
    auto elementVector = make_unique<vector<Element>>(10);
    return 0;
}
