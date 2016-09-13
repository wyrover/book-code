#include <vector>
#include <string>
using namespace std;

int main()
{
    vector<string> stringVector(10, "hello");

    for (auto it = begin(stringVector); it != end(stringVector); ++it) {
        it->append(" there");
    }

    /*
        // Using Range-Based for loop
        for (auto& str : stringVector) {
            str.append(" there");
        }
    */
    return 0;
}
