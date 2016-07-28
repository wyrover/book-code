#include <boost/lexical_cast.hpp>
#include <cstring>
#include <cstdlib>
#include <string>

namespace boost
{
template <>
double lexical_cast<double, std::string>(
    const std::string& str)
{
    const char *numstr = str.c_str();
    char *end = nullptr;
    double ret = strtod(numstr, &end);

    if (end && *end != '\0') {
        throw boost::bad_lexical_cast();
    }

    return ret;
}
}

int main()
{
    std::string num = "3.14159259";
    double d = boost::lexical_cast<double>(num);
}

