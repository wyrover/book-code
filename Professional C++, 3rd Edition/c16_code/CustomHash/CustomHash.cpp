#include <cstddef>
#include <unordered_set>
#include <functional>
#include <iostream>

using namespace std;

class IntWrapper
{
public:
    IntWrapper(int i) : mI(i) {}
    int getValue() const
    {
        return mI;
    }

    friend bool operator==(const IntWrapper& lhs, const IntWrapper& rhs);

private:
    int mI;
};

bool operator==(const IntWrapper& lhs, const IntWrapper& rhs)
{
    return lhs.mI == rhs.mI;
}

template<> class hash<IntWrapper> : public unary_function<IntWrapper, size_t>
{
public:
    size_t operator()(const IntWrapper& f) const
    {
        return std::hash<int>()(f.getValue());
    }
};

int main()
{
    unordered_set<IntWrapper> s;
    s.insert(IntWrapper(1));
    s.insert(IntWrapper(2));

    for (const auto& element : s) {
        cout << element.getValue() << endl;
    }

    return 0;
}
