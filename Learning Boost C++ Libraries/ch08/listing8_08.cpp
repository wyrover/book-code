#include <boost/chrono/chrono.hpp>
#include <boost/chrono/chrono_io.hpp>
#include <iostream>
#include <cstdint>
namespace chrono = boost::chrono;

int main()
{
    chrono::duration<int64_t, boost::ratio<1, 100>> csec(10);
    std::cout << csec.count() << '\n';
    std::cout << csec << '\n';
    chrono::seconds sec(10);
    chrono::milliseconds sum = sec + chrono::milliseconds(20);
    // chrono::seconds sum1 = sec + chrono::milliseconds(20);
    chrono::milliseconds msec = sec;
    // chrono::seconds sec2 = sum;
    chrono::seconds sec2 =
        chrono::duration_cast<chrono::seconds>(sum);
}

