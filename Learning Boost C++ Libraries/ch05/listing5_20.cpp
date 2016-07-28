#include <boost/assign.hpp>
#include <boost/rational.hpp>
#include <iterator>

using namespace boost::assign;

int main()
{
    std::cout << "Catalan numbers:\n";
    const std::vector<int> catalan = list_of(1)(1)(2)(5)
                                     (14)(42)(132)(429)(1430)(4862);
    std::ostream_iterator<int>os(std::cout, " ");
    std::copy(catalan.begin(), catalan.end(), os);
    std::cout << "\nBernoulli numbers:\n";
    const std::map<int, boost::rational<int>>bernoulli =
            map_list_of(0, boost::rational<int>(1))
            (1, boost::rational<int>(1, 2))
            (2, boost::rational<int>(1, 6))
            (3, boost::rational<int>(0))
            (4, boost::rational<int>(-1, 30))
            (5, boost::rational<int>(0))
            (6, boost::rational<int>(1, 42))
            (7, boost::rational<int>(0));

    for (auto&b : bernoulli) {
        std::cout << 'B' << b.first << ": " << b.second << ", ";
    }

    std::cout << '\n';
}

