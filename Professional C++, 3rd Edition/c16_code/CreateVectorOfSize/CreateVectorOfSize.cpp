#include <cstddef>
#include <vector>

using namespace std;

vector<int> createVectorOfSize(size_t size)
{
    vector<int> vec(size);
    int contents = 0;

    for (auto& i : vec) {
        i = contents++;
    }

    return vec;
}

int main()
{
    vector<int> myVector;
    myVector = createVectorOfSize(123);
    return 0;
}
