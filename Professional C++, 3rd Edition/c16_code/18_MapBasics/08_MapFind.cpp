#include <map>
#include <iostream>
using namespace std;

class Data
{
public:
    explicit Data(int val = 0)
    {
        mVal = val;
    }
    int getVal() const
    {
        return mVal;
    }
    void setVal(int val)
    {
        mVal = val;
    }

private:
    int mVal;
};

int main()
{
    map<int, Data> dataMap;
    dataMap[1] = Data(4);
    dataMap[1] = Data(6);
    auto it = dataMap.find(1);

    // If your compiler does not support the C++11 auto keyword:
    //map<int, Data>::iterator it = dataMap.find(1);
    if (it != end(dataMap)) {
        it->second.setVal(100);
    }

    return 0;
}
