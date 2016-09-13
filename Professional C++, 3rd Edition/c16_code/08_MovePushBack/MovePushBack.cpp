#include <vector>
#include <string>

using namespace std;

class Element
{
public:
    Element(int i, const string& str) : mI(i), mStr(str) {}

private:
    int mI;
    string mStr;
};

int main()
{
    vector<Element> vec;
    // push_back will trigger copying
    Element myElement(12, "Twelve");
    vec.push_back(myElement);
    // no copying, due to move semantics
    vec.push_back(move(myElement));
    vec.push_back(Element(12, "Twelve"));
    vec.push_back({12, "Twelve"});
    return 0;
}
