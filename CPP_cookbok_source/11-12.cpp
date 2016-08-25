#include <boost/random.hpp>
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace boost;

typedef boost::mt19937 BaseGenerator;
typedef boost::uniform_real<double> Distribution;
typedef boost::variate_generator<BaseGenerator, Distribution> Generator;

double boostDoubleRand()
{
    static BaseGenerator base;
    static Distribution dist;
    static Generator rng(base, dist);
    return rng();
}

int main()
{
    cout << "expect 5 numbers within the interval [0,1)" << endl;

    for (int i = 0; i < 5; i++) {
        cout << boostDoubleRand() << "\n";
    }

    cout << endl;
}