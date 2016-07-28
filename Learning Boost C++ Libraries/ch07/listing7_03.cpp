#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

struct ConcatIfStartsWith {
    ConcatIfStartsWith(char c) : startCh(c) {}

    void operator()(const std::string& name)
    {
        if (name.size() > 0 && name.at(0) == startCh) {
            csNames += name + ", ";
        }
    }

    std::string getConcat() const
    {
        return csNames;
    }

    void reset()
    {
        csNames = "";
    }

private:
    char startCh;
    std::string csNames;
};

int main()
{
    std::vector<std::string> names{"Meredith", "Guinnevere",
                                   "Mabel", "Myrtle", "Germaine", "Gwynneth", "Mirabelle"};
    const auto& fe = std::for_each(names.begin(), names.end(),
                                   ConcatIfStartsWith('G'));
    std::cout << fe.getConcat() << '\n';
}

